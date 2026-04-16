#include "MachineDefine.h"
#pragma hdrstop

#include "atester.h"
#include "atester_32Site.h"
#include "atester_ProcessCount.h"
#include "aTester_Front.h"
#include "aTester_Rear.h"

#include "MachineType.h"
#include "MyMotor.h"
#include "mysensor.h"
#include "myswitch.h"
#include "csetup.h"
#include "rs232.h"
#include "mycylin.h"                                                            //Richard 20220321 : 渠梁Side Push
#include "cTestCategory.h"
#include "cObserver.h"
#include "cSocket.h"
#include "cContactCT.h"
#include "cinitial.h"
#include "csystem.h"
#include "main.h"
#include "note.h"
#include "mymessbox.h"
#include "AutoClean.h"
#include "cContact.h"
#include "cAuthority.h"
#include "cprod.h"
#include "adam6024.h"
#include "cMyDB.h"
#include "iosetview.h"
#include "automation.h"
#include "uShowMessage.h"
#include "common.h"
#include "CCDInterface.h"
#include "uLotInfo.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20160509 :add New ATC Interface
#include "cpublic.h"
#include "Socket_ASE_KR.h"
#include "InterfaceSYS.h"
#include "BarCode.h"
#include "MyKitSuck.h"
#include "cmydef.h"
#include "ckernel.h"
#include "MessageDef.h"
#include "ainarm2.h"
#include "uTemp_Set.h"
#include "uHGemEquipment.h"
#include "uHGemHT9045.h"
#include "cConfiguration.h"
#include "HS_Function.h"
#include "cShowBinSelect.h"
#include "MyMotor.h"
#include "acarry.h"
#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
#include "uHeaterThread.h"
#include "TesterTCP.h"
#include "cSecurity.h"
#include "cContact.h"
#include "SCK_ART.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

const int CCDTimeOutSec=10;
const int TESTZ1UP=0;
const int TESTZ2UP=1;
const int MaxDIO=40;
const int CheckFailDownCT=1;                                                    //kevin 20131121   10 來不及偵測掉料

extern int iPickFromLoadStageTask;                                              //Steven 20110706
extern int iInArmPickFromHotPlateTask;                                          //Steven 20110706

extern void InitInArmTask();
extern bool MoveInArm2XYToWait();
extern ScanBtn *ScanBtnThd;

FILE *EPOut;                                                                    //Ifor 20150707
HANDLE  hTestChild;

DWORD dwEndShuttle1Soak;                                                        //JerryYang 20181001 (Steven) : fix Shuttle soak time 倒數秒數異常
DWORD dwStartShuttle1Soak;
DWORD dwEndShuttle2Soak;
DWORD dwStartShuttle2Soak;
DWORD dwTestChildID;

TQPF_Timer NULL_Delay;
TQPF_Timer DutDelay;                                                            //Steven 20180808 (wei) : TTL的時間單位改成microsecond(μs)
TQPF_Timer SoftContactTim;                                                      //kevin 20130608 soft 模式 需等ep 穩定
TQPF_Timer TSDDelay;
TQPF_Timer DoGiveWayDelay;                                                      //Ifor 20190723 : add
TQPF_Timer Noise_Delay;
TQPF_Timer HangTime;                                                            //Steven 20090827 : Hang Up dectector
TQPF_Timer hIndexSoakTime;                                                      //2013-11-27   Dell    需要做Index soak time
TQPF_Timer HAfterTestedDelay;                                                   //ChungHung 20140730 add for ATK function after tested delay time
TQPF_Timer DropContactTimer1;                                                   //JerryYang 20170503 (wei) drop contact的index cycle time分成三段來計時
TQPF_Timer DropContactTimer2;
TQPF_Timer DropContactTimer3;
TQPF_Timer dwStartInitialCount;
TQPF_Timer dwEndInitialCount;                                                   //kevin 20180926 add
TQPF_Timer hStartModeWaitTime;

int iStartModeEvent1=0;
int iStartModeEvent2=1;
int iStartModeEvent3=2;
int iDoEndMode=1;
int iDoStartMode=1;
int iCCDTimeOutCount=0;
int iStartStep=0;
int iTempICTask=1;
int iWhichAxisTestIC=0;
int iSendSMITimeOutLimit=1;
int iSocketSenSosPos1=0, iSocketSenSosPos2=0;                                   //kevin 20150613 關arm 設定可判斷位置
int iReContactCnt[MAX_SOCKET_ROW][MAX_SOCKET_COL];                              //Steven 20231205 : 計算某site contact 次數
int iTesterBIN[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{-1, -1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1, -1},
                                                {-1, -1, -1, -1, -1, -1, -1, -1}};

bool GetIndexTime_flag=false;
bool ScanPort[MaxDIO];
bool bFTestSuckDrop=false;
bool bBTestSuckDrop=false;
bool bZ1NeedTest=false, bZ2NeedTest=false;
bool bDoubleContact=false;
bool bNeedUpDonwOneTome=false;
bool fRearCheckSuckICPass=false;
bool bIndexPickUpErrMoveSht1=false;                                             //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
bool bIndexPickUpErrMoveSht2=false;                                             //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
bool bIndexWaitingInArmAway=true;                                               //Steven 20171228 (Wei) : Index在等In Arm讓開
bool bNeedCheckRTCReport=false;                                                 //Ifor 20190723 : add
bool SetNoiseDelay=false;
bool bHangTimePause=false;                                                      //Steven 20090827 : Hang Up dectector
bool bInitialSackTime=false;                                                    //kevin 20131112 第一次吸取ic等待時間
bool bFirstZ1UPZ2Down=true;                                                     //kevin 20131112 加熱時z1在shuttle 1上面 z2在下
bool bNeedIndexSoakTime=false;                                                  //2013-11-27   Dell    需要做Index soak time
bool bATCSiteTest[32]={false, false, false, false, false, false, false, false,
                       false, false, false, false, false, false, false, false,
                       false, false, false, false, false, false, false, false,
                       false, false, false, false, false, false, false, false}; //Ifor 20160510 add ATC Test Site
//==============================================================================
void InitDoStartMode()
{
    iDoStartMode=1;
}
//==============================================================================
void InitDoEndMode()
{
    iDoEndMode=1;
}
//------------------------------------------------------------------------------
void InitIndexEveryTimeCheckEP();
bool IndexEveryTimeCheckEP();
//------------------------------------------------------------------------------
bool IndexAlarmInArmAway()                                                      //Steven 20130613 : Index異常時, In Arm要先讓位功能
{
    bool ret=true;
    if(IniConfig.bIndexJamInArmAway==true)                                      //Steven 20110607 : Index Jam, In Arm要移開
    {
        bIndexWaitingInArmAway=true;                                            //Steven 20171228 (Wei) : : Index在等In Arm讓開

        if(CheckInArmFinishAllPickerAction()==false)                            //Steven 20171226 (Wei) : 修改in arm讓開的flag
        {                                                                       //kevin 20131119 Z軸正在吸取不能被中斷
            return false;
        }

        bInArmNeedToSafePos=true;                                               //Steven 20130819
        InitInArmTask();

        if(MoveInArm2XYToWait()==false)                                         //如果要移開,而且還沒移到定位
        {
            ret=false;                                                          //先離開等In Arm
        }
        else
        {
            ret=true;
            bIndexWaitingInArmAway=false;                                       //Steven 20171228 (Wei) : : Index在等In Arm讓開
        }
    }
    return ret;
}
//---------------------------------------------------------------------------
bool DoStartMode(int mode)
{
    int &Task=iDoStartMode;
    if(mode==0)                                                                 //Mode 0 return true
    {
        return true;
    }
    else if(mode==1)                                                            //Mode 1 向host send "PRODUCTION_REQUEST 0003" return true;
    {
        fAutomation->DoCommandBuffer("PRODUCTION_REQUEST", "", "0003", 0, "");
        return true;
    }
    else if(mode==2)                                                            //Mode 2
    {
        switch(Task)
        {
            case 1:                                                             //向host send "PRODUCTION_REQUEST 0003"
                fAutomation->ClearEvent(iStartModeEvent1);                      //釋放資源
                fAutomation->GetEventNum(iStartModeEvent1, "PRODUCTION_REPLY",   "0003");//取得編號 設定條件字串
                fAutomation->ClearEvent(iStartModeEvent2);                      //釋放資源
                fAutomation->GetEventNum(iStartModeEvent2, "PRODUCTION_REQUEST", "0007");//取得編號 設定條件字串
                fAutomation->ClearEvent(iStartModeEvent3);                      //釋放資源
                fAutomation->GetEventNum(iStartModeEvent3, "PRODUCTION_REQUEST", "0008");//取得編號 設定條件字串
                fAutomation->DoCommandBuffer("PRODUCTION_REQUEST", "","0003", 0, "");
                hStartModeWaitTime.SetSecAndOn(10);                             //設定計數
                Task=50;
            case 50:                                                            //等待Host回覆"PRODUCTION_REPLY 0003"
                if(fAutomation->GetEventResult(iStartModeEvent1)==true)         //Host回覆"PRODUCTION_REPLY 0003"
                {
                    fAutomation->ClearEvent(iStartModeEvent1);                  //釋放資源
                    hStartModeWaitTime.SetSecAndOn(10);
                    Task=100;
                }
                else                                                            //Host還未回覆"PRODUCTION_REPLY 0003"
                {
                    if(hStartModeWaitTime.Off())
                    {
                        Task=1;
                        fAutomation->ClearEvent(iStartModeEvent1);              //釋放資源
                        return true;
                    }
                    break;
                }
            case 100:                                                           //Host回應"PRODUCTION_REQUEST 0007" OK or "PRODUCTION_REQUEST 0008" NG
                if(iStartModeEvent1==-1)                                        //Sam 20190429 : Add CC_PTI_NEWWORK
                {
                   iStartModeEvent1=0;
                   Task=1;
                   break;
                }

                if(fAutomation->GetEventResult(iStartModeEvent2)==true)         //Host 有回應
                {
                    fAutomation->ClearEvent(iStartModeEvent2);                  //Sam 20190429 : Add CC_PTI_NEWWORK
                    fAutomation->DoCommandBuffer("PRODUCTION_REPLY", "","0007", 0, "");
                    bStartModeComplete=true;                                    //Sam 20190429 : Add CC_PTI_NEWWORK
                    Task=1;                                                     //Sam 20190429 : Add CC_PTI_NEWWORK
                    return true;
                }
                else if(fAutomation->GetEventResult(iStartModeEvent3)==true)    //NG
                {
                    fAutomation->ClearEvent(iStartModeEvent3);                  //Sam 20190429 : Add CC_PTI_NEWWORK
                    fAutomation->DoCommandBuffer("PRODUCTION_REPLY", "","0008", 0, "");
                    Task=1;                                                     //Sam 20190429 : Add CC_PTI_NEWWORK
                }
                else                                                            //Host 還未回覆
                {
                    if(hStartModeWaitTime.Off())                                //逾時 ShowAlarm Message "Wait PRODUCTION_REPLY 0007 Over Time" Retry or Skip
                    {
                        Task=1;                                                 //Sam 20190429 : Add CC_PTI_NEWWORK
                        return true;
                    }
                }
                break;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer hEndModeWaitTime;
int iEndModeEvent1=0;
bool DoEndMode(int mode)
{
    int &Task=iDoEndMode;

    if(bStartModeComplete==false)
    {
        return true;
    }

    if(mode==0)                                                                 //Mode 0 return true
    {
        return true;
    }

    if(mode==1)                                                                 //Mode 1 向host send "TEST_RESULT_REQUEST" return true;
    {
        fAutomation->DoCommandBuffer("TEST_RESULT_REQUEST", "", "", 0, "");
        return true;
    }

    switch(Task)                                                                //Mode 2
    {
        case 1:                                                                 //向host send "TEST_RESULT_REQUEST"
            fAutomation->DoCommandBuffer("TEST_RESULT_REQUEST", "", "", 0, "");
            hEndModeWaitTime.SetSecAndOn(10);                                   //設定計數
            fAutomation->GetEventNum(iEndModeEvent1, "TEST_RESULT_REPLY", "");  //取得編號 設定條件字串
            Task=50;
        case 50:                                                                //等待Host回覆"TEST_RESULT_REPLY"
            if(fAutomation->GetEventStrResult(iEndModeEvent1)==3)               //Host回覆"PRODUCTION_REPLY 0003"
            {
                fAutomation->ClearEvent(iEndModeEvent1);                        //釋放資源
                bStartModeComplete=false;
                Task=1;
                return true;
            }
            else                                                                //Host還未回覆"TEST_RESULT_REPLY"
            {
                if(hEndModeWaitTime.Off())
                {
                    fAutomation->ClearEvent(iEndModeEvent1);
                    Task=1;
                    return true;
                }
                break;
            }
    }
    return false;
}
//==============================================================================
bool ScanCCDProgram()
{
    HWND HCCDWnd=FindWindow(NULL,"Identification");
    if(HCCDWnd==NULL)
    {
        CCDInterfaceForm->bAtestScanCCDProgram=false;                           //kevin 20110811
        return false;
    }
    else
    {
        CCDInterfaceForm->bAtestScanCCDProgram=true;                            //kevin 20110811
        return true;
    }
}
//==============================================================================
bool GetTempICResult();
const int OverEncoderDelay=50;

int iGetTestDataDelayTask=1;
int iRecordOldBin[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{-1, -1, -1, -1, -1, -1, -1, -1},
                                                   {-1, -1, -1, -1, -1, -1, -1, -1},
                                                   {-1, -1, -1, -1, -1, -1, -1, -1},
                                                   {-1, -1, -1, -1, -1, -1, -1, -1}};

//==============================================================================
int GetRTCSiteMap()
{
//                    ========================================================
//                    第5碼        Site 分佈樣式
//                    ========================================================
//                    asSiteMap    0         1 x 1
//                                 1         1 x 2
//                                 2         1 x 4
//                                 3         2 x 2
//                                 4         2 x 4
//                                 5         2 x 6
//                                 6         2 x 8
//                    ========================================================
    int iSiteMapRTC=-1;

    if(TestIF_File.iTestMode==SingleSite)                                       //1x1
    {
        iSiteMapRTC=0;
    }
    else if(TestIF_File.iTestMode==DualSite)                                    //1x2
    {
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==TriSite1X3)                                  //Frank 20160329 add for 1x3_4
    {
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==QualSite1X4 ||
            TestIF_File.iTestMode==_8Site1X4)                                   //ChungHung 20150528 add for 海思 _8Site1x4 //1x4
    {
        iSiteMapRTC=2;
    }
    else if(TestIF_File.iTestMode==DualSite2x1)                                 //2x1
    {
        iSiteMapRTC=1;
    }
    else if(TestIF_File.iTestMode==QualSite2X2 ||                               //2x2
            TestIF_File.iTestMode==QualSite2X2N)                                //Frank 20200520 2X2NN Mod
    {
        iSiteMapRTC=3;
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||                                 //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        iSiteMapRTC=7;
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||                                 //2x4
            TestIF_File.iTestMode==_8Site2X4N ||                                //Wei 20231211 : 2X4NN Mode
            TestIF_File.iTestMode==_16Site4X4)                                  //Sam 20190226 : 16Site4X4
    {
        iSiteMapRTC=4;
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //2x5  //wei 20190614 10 site
    {
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //2x6
    {
        iSiteMapRTC=5;
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //2x8  //Eliot 2009_12_25
    {
        iSiteMapRTC=6;
    }
    else if(TestIF_File.iTestMode==_32Site4X8M ||                               //4x8
            TestIF_File.iTestMode==_32Site4X8N)                                 //ChungHung 20130627 alter TestIF--->TestIF_File 修正無法跑32Site
    {
        iSiteMapRTC=6;
    }
    return iSiteMapRTC;
}
//==============================================================================
//marc 2008/04/ start
//clean1 start signal hi/low invert
//clean2 start signal
//clean3 NULL Pin
//******************************************************************************
//
//  注意!! ArrayConvertSite內容影響Site Mapping與開關Site, 修改時要小心驗證
//
//******************************************************************************
int ArrayConvertSite(int X, int Y)
{
    int Site=-1;

    if(TestIF.iTestMode>=_6Site2X3)                                             //Alick 20161011 (Steven) : TTL支援8Site
    {
        Site=X*4+Y+1;
    }
    else if(TestIF.iTestMode>=DualSite2x1)
    {
        Site=X*2+Y+1;
    }
    else
    {
        Site=Y+1;
    }
    return Site-1;
}
//------------------------------------------------------------------------------
bool CheckBackError()
{
    int ArmCmdPos[4], i;
    ArmCmdPos[0]=MOT[MTestY1].Gali_ReadPos();
    ArmCmdPos[1]=MOT[MTestZ1].Gali_ReadPos();
    ArmCmdPos[2]=MOT[MTestZ2].Gali_ReadPos();
    ArmCmdPos[3]=MOT[MTestY2].Gali_ReadPos();
    for(i=0; i<4; i++)
    {
        if(ArmCmdPos[i]>=Prod.TestZ1_Safe+30 ||
           ArmCmdPos[i]<=Prod.TestZ1_Safe-30)
            return true;
    }
    return false;
}
//==============================================================================
bool TestArmBackPos()
{
    bool bError=false;
    static int Temp[2]={0, 0};
    #ifndef SOFT_SIMULTE
    int Z1CmdPos=MOT[MTestZ1].Gali_ReadPos();
    int Z2CmdPos=MOT[MTestZ2].Gali_ReadPos();

    if(Z1CmdPos>=Prod.TestZ1_Safe+50 ||
       Z1CmdPos<=Prod.TestZ1_Safe-50)
    {
        Temp[0]++;
        if(Temp[0]>1000)                                                        //Steven 20170104 : 加上Delay, 等機構穩定
        {
            ShowMyMessage("Index Motor Z1 is not at home position!! [D51]", "Z1馬達未在上方位置!!");  //Steven 20091004
            Temp[0]=0;
            return false;
        }
        else
        {
            bError=true;
        }
    }

    if(Z2CmdPos>=Prod.TestZ2_Safe+50 || Z2CmdPos<=Prod.TestZ2_Safe<=-50)
    {
        Temp[1]++;
        if(Temp[1]>1000)
        {
            ShowMyMessage("Index Motor Z2 is not at home position!! [D51]", "Z2馬達未在上方位置!!");  //Steven 20091004
            Temp[1]=0;
            return false;
        }
        else
        {
            bError=true;
        }
    }
    #endif

    if(bError==false)
    {
        if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, 50000, __FUNC__))
        {
            Temp[0]=0;
            Temp[1]=0;
            IndexStatus=IndexIsBack;
            return true;
        }
    }
    return false;
}
//==============================================================================
int iTestTask=1;
void InitTestTask()
{
    iTestTask=1;
}
TQPF_Timer TestTimeOut;
TQPF_Timer HTestDeley;

//******************************************************************************
//
//  注意!! GetOneByOneData為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
extern bool bEcho, bExist, bUnderTest;
extern bool bEchoStop;                                                          //ChungHung 20130326 add
extern unsigned int iBin[4][8];
extern bool bGPIBError;
int HexData[5]={0x01, 0x02, 0x04, 0x08, 0x10};
//------------------------------------------------------------------------------
int IOCT[]={4,      //_4Bit=0
            8,      //_8Bit=1
            4,      //_5BitPE
            9,      //_10BitPE
            5,      //_5Bit
            10,     //_10Bit
            4,      //_5BitPO
            9};     //_10BitPO
                    //_3Bit=8;
//------------------------------------------------------------------------------
int GetOneByOneData(int X, int Y)
{
    int iResult=-1;
    int Data;
    int iSite=ArrayConvertSite(X, Y);
    int sum=0;

    if(Prod.DIOCfg.iCateBitLength==_8Bit || Prod.DIOCfg.iCateBitLength==_10Bit ||
       Prod.DIOCfg.iCateBitLength==_10BitPE || Prod.DIOCfg.iCateBitLength==_10BitPO)
    {
        Data=iSite*10;
        if(CosFunction.bTTLCanUse8Site==true)                                   //Alick 20161011 (Steven) : TTL支援8Site
        {
            if(iSite>=4)
                return -1;
        }
        else
        {
            if(iSite>=2)
                return -1;
        }
    }
    else
    {
        Data=iSite*5;
    }

    for(int i=0; i<IOCT[Prod.DIOCfg.iCateBitLength]; i++)
    {
        if(ScanPort[i+Data])                                                    //jou ??? over array
        {
            sum++;
            iResult=i;
        }
    }

    if(iResult!=-1)
    {
        if(sum!=1)
            return iTestBinCount;                                               //Jou 20210126 : 16 --> iTestBinCount
        return iResult+1;
    }
    return -1;
}
//******************************************************************************
//
//  注意!! GetBinaryData為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
int GetBinaryData(int X, int Y)
{
    int Sum=0;
    int Data;
    int iSite=ArrayConvertSite(X, Y);
    if(Prod.DIOCfg.iCateBitLength==_8Bit || Prod.DIOCfg.iCateBitLength==_10Bit ||
       Prod.DIOCfg.iCateBitLength==_10BitPE || Prod.DIOCfg.iCateBitLength==_10BitPO)
    {
        Data=iSite*10;
        if(iSite>=2)
            return -1;
    }
    else
    {
        Data=iSite*5;
    }

    for(int i=0; i<IOCT[Prod.DIOCfg.iCateBitLength]; i++)
        Sum+=ScanPort[i+Data]*HexData[i];
    if(Sum==0)
        return -1;
    else
        return Sum;
}
//******************************************************************************
//
//  注意!! Sim_TTL_Single為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
void Sim_TTL_Single(int i, int j)                                               //Steven 20091031 Start : for TTL offline testing
{
    int iSenBitStatus[10]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TComboBox *ComboCH[4]={fMain->cbSimuBinSite0, fMain->cbSimuBinSite1, fMain->cbSimuBinSite2, fMain->cbSimuBinSite3};

    int iSite=ArrayConvertSite(i, j);
    int iCH=TestIF.iSiteMap[i][j]-1;

    if(ComboCH[iCH]->Text=="1")
    {
        iSenBitStatus[0]=1;
    }
    else if(ComboCH[iCH]->Text=="2")
    {
        iSenBitStatus[1]=1;
    }
    else if(ComboCH[iCH]->Text=="3")
    {
        iSenBitStatus[0]=1;
        iSenBitStatus[1]=1;
    }
    else if(ComboCH[iCH]->Text=="4")
    {
        iSenBitStatus[2]=1;
    }
    else if(ComboCH[iCH]->Text=="5")
    {
        iSenBitStatus[0]=1;
        iSenBitStatus[2]=1;
    }
    else if(ComboCH[iCH]->Text=="6")
    {
        iSenBitStatus[1]=1;
        iSenBitStatus[2]=1;
    }
    else if(ComboCH[iCH]->Text=="7")
    {
        iSenBitStatus[0]=1;
        iSenBitStatus[1]=1;
        iSenBitStatus[2]=1;
    }
    else if(ComboCH[iCH]->Text=="8")
    {
        iSenBitStatus[3]=1;
    }
    else if(ComboCH[iCH]->Text=="9")
    {
        iSenBitStatus[0]=1;
        iSenBitStatus[3]=1;
    }
    for(int k=0; k<5; k++)
    {
        ScanPort[k+iSite*5]=iSenBitStatus[k];
    }
}
//******************************************************************************
//
//  注意!! Test_GetTestResulSub為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
int Test_GetTestResulSub(int X, int Y)
{
#ifndef SOFT_SIMULTE
    for(int i=0; i<10; i++)
    {
        ScanPort[i]     =Sen[SenBit0+i].Status();
        ScanPort[i+10]  =Sen[SenBit10+i].Status();
        ScanPort[i+20]  =Sen[SenBit20+i].Status();                              //Alick 20161011 (Steven) : TTL支援8Site
        ScanPort[i+30]  =Sen[SenBit30+i].Status();
    }
#endif
    for(int i=0; i<10; i++)
    {
        if(ScanPort[i] && SetNoiseDelay==false)
        {
            Noise_Delay.SetSecAndOn(0.1);
            SetNoiseDelay=true;
        }
    }

    if(Prod.DIOCfg.iCateDataType==CHOneByOne)
        return GetOneByOneData(X, Y);
    else
        return GetBinaryData(X, Y);
}
//******************************************************************************
//
//  注意!! Test_GetTestResul為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
int Test_GetTestResul(int X, int Y)                                             //990517 保護機制,重複確認50次
{
    int iOld=-1, iBuffer=-1;

    iOld=Test_GetTestResulSub(X, Y);
    if(iOld==-1)
        return -1;

    for(int i=0; i<50; i++)
    {
        iBuffer=Test_GetTestResulSub(X, Y);
        if(iOld!=iBuffer)
            return -1;
    }

    return iOld;
}
//---------------------------------------------------------------------------
//jou 2011-11-14 end
/*
HT-9045 Test site design rule(stander)
    (16Site)
    a   b   c   d   e   f   g   h
   |==============================
  A|1   3   5   7   9  11  13  15
  B|2   4   6   8  10  12  14  16

    (8Site)
    a   b   c   d
   |==============
  A|1   3   5   7
  B|2   4   6   8

    (4Site 1*4)
    a   b   c   d
   |==============
  A|1   2   3   4

  (4Site 2*2)
    a   b
   |==============
  A|1   3
  B|2   4

    (2Site)
    a   b
   |==============
  A|1   2
*/
//******************************************************************************
//
//  注意!! GetTesterResult為Handler收發BIN相關, 修改時要小心!!
//
//******************************************************************************
TQPF_Timer TestWaitTime;
TQPF_Timer MyTSDTimer;                                                          //Steven 20161208 : Record TSD time data
TQPF_Timer LBBoostTimeOutTimer;
TQPF_Timer TestStopMotorTimer;
bool bLBBoostTimeOut=false;                                                     //Steven 20181222 : Add LB升溫的Time out
bool GetTesterResult(int Type)
{
    static int iBarCodeMessSize=0;                                              //kevin 20160401  //JerryYang 20180629 (wei) : Mark掉,移到上一層函式
    static bool flag[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static bool bEPaddMSec=false;
    static Byte BarCodeMess[2048];
    static double dGetSec=0.0;
    static double dTestTime=0.0;
    static AnsiString StrBarCodeAll;

    int &Task=iTestTask, ret, iCH;
    int iEPaddSec=0;
    int iSiteOn[4]={0, 0, 0, 0};
    int iTempBin=0;
    int iSiteNo;                                                                //kevin 20140317
    DWORD dwTestTime=0;                                                         //jou 2013-09-25 Testing Need Stop All Motor
    char str2[256];
    bool bHasError=false;
    bool bGetTestArmTorque=false;
    bool flag2[MAX_SOCKET_TOTAL];
    ZeroMemory(flag2, sizeof(flag2));
    double dEPaddMSec=0.0;
    AnsiString StrBarCode[32];
    AnsiString Str, Str1, sLog;

    switch(Task)
    {
        case 1:
            bTestingStopAllMotor=false;                                         //jou 2013-09-25 Testing Need Stop All Motor
            iGetTestDataDelayTask=1;                                            // 2010/05/15 lee/joye
            bLBBoostTimeOut=false;
            if(TestIF_File.bEnableReadAndCheckTorque)                           //kevin 20210804 change //jou 2012-06-12 即時更新扭力值不能開，不然會衝突
            {
                if(Type==1)                                                     //ARM2
                {
                    bNeedCheckIndexToque=false;
                    bNeedCheckIndexToque1=false;
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=true;
                    fMain->edTorue1->Text="";
                    COM2->InitReadTorueTask();                                  //kevin 20211028 add HPCOM initial
                    bRetryReadToqu = false;                                     //kevin 20210419 重讀扭力
                }
                else
                {
                    bNeedCheckIndexToque=false;
                    bNeedCheckIndexToque1=false;
                    fMain->chkReadTorque1->Checked=true;
                    fMain->chkReadTorque2->Checked=false;
                    fMain->edTorue0->Text="";
                    COM2->InitReadTorueTask();                                  //kevin 20211028 add HPCOM initial
                    bRetryReadToqu = false;                                     //kevin 20210419 重讀扭力
                }
            }                                                                   //kevin 20211109 mark
            Task=10;                                                            //do not add break;
        case 10:
            for(int i=0; i<TestSocket.iMaxRow; i++)
            {
                for(int j=0; j<TestSocket.iMaxCol; j++)
                {
                    flag[i][j]=false;
                    iTesterBIN[i][j]=-1;
                    iRecordOldBin[i][j]=-1;
                    bDutflag[i*MAX_SOCKET_COL+j]=false;                         //ChungHung 20140611 fix 紀錄測試的Dut位置 對應錯誤
                }
            }

            if((BAR_CODE_INSTALL!=ebctUninstall &&
                TestIF_File.bEnableBarCode) ||
               (INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction))              //wei 20160922 增加OCR Function
            {
                memset(BarCodeMess, '\0', sizeof(BarCodeMess));
                iBarCodeMessSize=0;
            }

            for(int i=0; i<MAX_SOCKET_TOTAL; i++)                               //Jimmychiu 20231016 : 32 ->MAX_SOCKET_TOTAL
            {
                if(TestIF.iGpibMode==InterfaceType_SPEA_Type)                   //wei 20151120
                {
                    StrBarCode[i].sprintf("0,");
                }
                else
                {
                    if(CUSTOMER_CODE==CC_FMSH && BAR_CODE_INSTALL==ebcUseOCR)   //Ifor 20200924 add:AMD Barcode 資料改由GPIB 處理
                    {
                        StrBarCode[i].sprintf("NULL");
                    }
                    else
                    {
                        StrBarCode[i].sprintf("0");
                        fMain->tBarCodeList->Strings[i]="0";
                    }
                }
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(TestSocket.Item[i][j]!=NULL_IC &&
                       TestSocket.Item[i][j]<TEST_PASS &&
                       TestSocket.Item[i][j]!=HAS_NULL_IC)
                    {
                        if(TestIF_File.iCloseSiteOnHPDontTest!=0 &&             //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
                           LastSet.bUseTestSocket[Type][i][j]==false)           //Steven 20250604 : 關site的位置有IC不測試送指定 bin
                        {
//                            if(TestIF_File.iCloseSiteOnHPDontTest==2)           //不能在這邊設定bin, 會造成hang up
//                            {
//                                TestSocket.SetItemData(i, j, TEST_PASS+TestIF_File.iCloseSiteBin);
//                                TestSocket.iBinData[i][j]=TestIF_File.iCloseSiteBin;
//                                TestSocket.PordRec[i][j].AddTestResultRecord(TestIF_File.iCloseSiteBin, TestSocket.cSBin[i][j], "NonTestToSettedBin");
//                            }
//                            else
                            {
                                TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                TestSocket.iBinData[i][j]=iTestBinCount;
                                if(TestIF_File.iCloseSiteOnHPDontTest==2)
                                    TestSocket.PordRec[i][j].AddTestResultRecord(iTestBinCount, TestSocket.cSBin[i][j], "NonTestToSettedBin");
                                else
                                    TestSocket.PordRec[i][j].AddTestResultRecord(iTestBinCount, TestSocket.cSBin[i][j], "NonTestToRBin");
                            }
                        }
                        else
                        {
                            if((CosFunction.bBarcodeErrNoTestAndShowH==true ||  //jou 20191007 : Barcode Error No Test & Show "H"
                                TestIF_File.iNoCodeDeviceToErr==2) &&           //Steven 20200909 : 將2DID all site fail變成選項
                               TestIF_File.bEnableBarCode==true &&
                               (TestSocket.cDeviceInf[i][j]==asBarCodeErrorSend ||
                                TestSocket.cDeviceInf[i][j]==""))
                            {
                                TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                                TestSocket.iBinData[i][j]=iTestBinCount;
                                if(LastSet.iTester==_2D_SORT)                   //JerryYang 20230803 : 2D SORT區分ERROR原因
                                {
                                    TestSocket.PordRec[i][j].AddTestResultRecord(iTestBinCount, TestSocket.cSBin[i][j], "BarcodeReadError");
                                }
                                else
                                {
                                    TestSocket.PordRec[i][j].AddTestResultRecord(iTestBinCount, TestSocket.cSBin[i][j], "NonTestToRBin");
                                }
                            }
                            else
                            {
                                flag[i][j]=true;
                            }
                        }

                        if(TestIF.iSiteMap[i][j]!=-1 &&
                           TestIF.iSiteMap[i][j]!=0)                            //kevin 20141013  ChungHung 20140616 沒卡掉會溢位
                        {
                            if((CosFunction.bBarcodeErrNoTestAndShowH==true ||  //jou 20191007 : Barcode Error No Test & Show "H"
                                TestIF_File.iNoCodeDeviceToErr==2) &&           //Steven 20200909 : 將2DID all site fail變成選項
                               TestIF_File.bEnableBarCode==true &&
                               (TestSocket.cDeviceInf[i][j]==asBarCodeErrorSend ||
                                TestSocket.cDeviceInf[i][j]==""))
                            {
                                //bDutflag[TestIF.iSiteMap[i][j]-1]=false;
                            }
                            else
                            {
                                bDutflag[TestIF.iSiteMap[i][j]-1]=true;         //ChungHung 20140611 fix Dut 紀錄 對應錯誤
                            }

                            if(LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false)
                            {
                                iAutoSiteRecordIC[0]=i;                         //kevin 20150114   記錄目前有ic位置
                                iAutoSiteRecordIC[1]=j;                         //kevin 20150114
                            }
                        }

                        //gpib 傳過來是  0 2 4 6 8
                        //               1 3 5 7 9
                        if(IniConfig.bNewResetFunction)
                        {
                            TestSocket.SetItemData(i, j, HAS_TESTING_IC);
                        }
                    }

                    if(CUSTOMER_CODE==CC_Murata &&
                       IniConfig.bN23_1_Enable2DIDCompare)                      //Steven 20200611 : for Murata, 2DID NG不測試
                    {
                        if(TestSocket.Item[i][j]!=NULL_IC &&
                           TestSocket.Item[i][j]!=HAS_NULL_IC &&
                           TestSocket.b2DIDNG[i][j]==true)                      //Steven 20200611 : for Murata, 2DID NG不測試
                        {
                            flag[i][j]=false;
                            TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                            TestSocket.iBinData[i][j]=iTestBinCount;
                        }
                    }

                    if((BAR_CODE_INSTALL!=ebctUninstall && TestIF.bEnableBarCode) ||
                       (INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction))
                    {
                        iSiteNo=TestIF.iSiteMap[i][j]-1;
                        if(iSiteNo>=0)
                        {
                            if(flag[i][j])
                            {
                                if(TestIF.iGpibMode==InterfaceType_SPEA_Type)   //wei 20151120
                                {
                                    StrBarCode[iSiteNo].sprintf("%s,", TestSocket.cDeviceInf[i][j]);
                                }
                                else
                                {
                                    if(TestSocket.Item[i][j]!=NULL_IC     &&
                                       TestSocket.Item[i][j]<TEST_PASS    &&
                                       TestSocket.Item[i][j]!=HAS_NULL_IC &&
                                       (TestSocket.cDeviceInf[i][j]=="" ||
                                        TestSocket.cDeviceInf[i][j]=="0"))
                                    {
                                        TestSocket.cDeviceInf[i][j]=asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                    }
                                    StrBarCode[iSiteNo].sprintf("%s", TestSocket.cDeviceInf[i][j]);
                                }
                            }
                            else
                            {
                                if(TestIF.iGpibMode==InterfaceType_SPEA_Type)   //wei 20151120
                                {
                                    StrBarCode[iSiteNo].sprintf("0,");
                                }
                                else
                                {
                                    if(TestSocket.Item[i][j]!=NULL_IC     &&
                                       TestSocket.Item[i][j]<TEST_PASS    &&
                                       TestSocket.Item[i][j]!=HAS_NULL_IC &&
                                       TestSocket.cDeviceInf[i][j]=="")
                                    {
                                        StrBarCode[iSiteNo].sprintf(asBarCodeErrorSend.c_str());        //wei 20160318 Barcode Error依客戶設定
                                    }
                                    else
                                    {
                                        if(CUSTOMER_CODE==CC_FMSH &&
                                           BAR_CODE_INSTALL==ebcUseOCR)         //Ifor 20200924 add:AMD Barcode 資料改由GPIB 處理
                                        {
                                            StrBarCode[iSiteNo].sprintf("NULL");//Ifor 20210906 OCR 尾數盤異常 i=>iSiteNo
                                        }
                                        else
                                        {
                                            StrBarCode[iSiteNo].sprintf("0");
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            //StrBarCode[iSiteNo].sprintf("0,");   記憶體溢位
                        }
                    }
                }
            }

            StrBarCodeAll="";
            for(int i=0; i<MAX_SOCKET_TOTAL; i++)                               //Jimmychiu 20231016 : 32 ->MAX_SOCKET_TOTAL
            {
                StrBarCodeAll+=StrBarCode[i];
                fMain->tBarCodeList->Strings[MAX_SOCKET_TOTAL-1-i]=StrBarCode[i];       //wei 20151120   //Jimmychiu 20231016 : 31 ->MAX_SOCKET_TOTAL-1
            }

            SW[SwTesterPower].On();
            if(TestSocket.HasRealIC())                                          //Steven 20190313 : No IC no delay
            {
                if(Prod.bUseOtherArmToTestAfterInitialDelay==false)
                    CheckInitialStartDelayInSocket();                           //JerryYang 20180629 (wei) : Initial delay判斷包成函式
            }

            bATC_SITE_2ND_CHECK[Type]=true;                                     //Ifor 20160516 add Index 測試前開啟 ATC 第二點溫度偵測

            for(int i=0; i<iATC_Use_Heat_Count; i++)                            //Ifor 20160516 修改ATC Heat 設定數
            {
                if(Type==0)
                {
                    if(i<(iATC_Use_Heat_Count/2))
                        bATCSiteTest[i]=bATC_EnablesChannel[i];
                    else
                        bATCSiteTest[i]=false;
                }
                else
                {
                    if(i>=(iATC_Use_Heat_Count/2))
                        bATCSiteTest[i]=bATC_EnablesChannel[i];
                    else
                        bATCSiteTest[i]=false;
                }
            }

            if(Temperature.bBoostFuncttion || Temperature.bLBTempFunction)
            {
                iTriggerBoostFunction=CheckToBoostIndexTemp();
            }

            #ifdef SOFT_SIMULTE
                iTriggerBoostFunction=-1;
            #endif

            if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70 &&
               Temperature.bATC7TSDFunction==true &&                            //Steven 20160604 : by site TSD
               LastSet.iRealDummy==REALLY &&
               LastSet.iTester==ON_LINE)                                        //Steven 20161025 : On Line && Has IC才跑TSD
            {
                MyTSDTimer.LatchCycleTime(true);                                //Steven 20161208 : Record TSD time data
                Task=20000;
                break;
            }
            else if((Temperature.bBoostFuncttion ||
                     Temperature.bLBTempFunction) &&
                    iTriggerBoostFunction!=-1)                                  //Steven 20180817 : Boost Function
            {
                Task=30000;
                iTriggerBoostFuncBack=-1;
                Str.sprintf("Trigger boost duration function! (%d)", iTriggerBoostFunction);
                RecordProcess(Str);
                if(fContact->fShow)
                    fContact->labDelayStatus->Caption="Start boost function";
                break;
            }
            else
            {
                Task=20;
            }
        case 20:
            HangTime.SetSecAndOn(Prod.iHangupMaxTime);                          //JerryYang 20230803 : fix initial start delay 太長誤發hang up

            if(SystemStart==false)                                              //JerryYang 20180828 (Steven) : fix 按暫停卻送出測試訊號
            {
                SetTestTimeOutTimer(Type);                                      //JerryYang 20200623 避免誤發timeout
                break;
            }

            if(bUseInitTempOffset &&
               IniConfig.bL28TempOfsUseReadyTempRange &&
               bEnable_KLT_Function==false &&
               fHeaterOK==false &&
               Temperature.iTempReadyRange!=0 &&                                //Sam 20231214 : Temp offset use ready temp range
              (iInitContactCount<(Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset)))
            {
                break;
            }

            if(TestSocket.HasRealIC())                                          //Steven 20190313 : No IC no delay
            {
                if(bUseInitDelay &&                                             //Steven 20171219 (Wei) : 只有加熱模式要跑Initial Delay
                   bNeedInitialTestDelay)
                {
                    if(NeedResetInitialDelay()==true)                           //JerryYang 20180828 (Steven) : 預熱過程中又觸發預熱的話, 就重新執行預熱
                    {
                        dwStartInitialCount.LatchCycleTime(true);
                        break;
                    }

                    if(iInitialCount>0)                                         //ChungHung 20140801 add Korea Want to count down in main status
                    {
                        iInitialCount=Prod.iInitialDelay-(dwStartInitialCount.LatchCycleTime()/1000);   //kevin 20181102 (Steven) : HangUp卡在case20 加入絕對值保護
                        break;
                    }
                }
                else
                {
                    iInitialCount=0;
                }
                bNeedInitialTestDelay=false;

                if(bHPCleanout)                                                 //wei 20160624 Hotplate clean out
                {
                    HTestDeley.SetSecAndOn(0.1);
                }
                else if(IniConfig.bInitialStartDelayCount)                      //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                {
                    if(bUseInitDelay)
                    {
                        if(TestIF.iEnStartDelayCount!=0)
                        {
                            iInitStartDelayTimeCT++;

                            if(iInitStartDelayTimeCT>TestIF.iStartDelayCount[iAddInitStartDelayCT]) //kevin 20180307
                            {
                                if(iAddInitStartDelayCT<4)
                                    iAddInitStartDelayCT++;                     //kevin 20180307 add delay count InitStartDelayTime
                                iInitStartDelayTimeCT=1;
                                iInitStartDelayDec=0;
                            }

                            if(bFinishInitStartDelay==false &&
                               (iAddInitStartDelayCT<TestIF.iEnStartDelayCount) &&
                               (iInitStartDelayTimeCT<=TestIF.iStartDelayCount[iAddInitStartDelayCT])) //kevin 20180307 change parm
                            {
                                if(CosFunction.bHaveIndexContactDelay &&
                                   IniConfig.bD29EnableIndexContactDelay)       //Steven 20140519 : [D29] Index下壓後多Delay 0.4秒 (For SPIL Low Yield)
                                {
                                    HTestDeley.SetSecAndOn(TestIF.dInitStartDelayTime+0.4);
                                }
                                else
                                {
                                    if(TestIF.dInitialStartDelayDec[iAddInitStartDelayCT]!=0)        //kevin 20180307 change parm     //kevin 20161214 (Steven) 第幾個 Count開始執行送訊號delay
                                    {
                                        iInitStartDelayDec=TestIF.dInitStartDelayTime+(iInitStartDelayTimeCT*TestIF.dInitialStartDelayDec[iAddInitStartDelayCT]);//kevin 20180307 change parm
                                        if(iInitStartDelayDec<=0)
                                            iInitStartDelayDec=0;
                                    }
                                    else
                                    {
                                        iInitStartDelayDec=TestIF.dInitStartDelayTime;
                                    }
                                }
                                bInitStartDelayTime=true;                       //wei 20171020 (jou) InitStartDelayTime秒數倒數
                            }

                            if(iAddInitStartDelayCT>=4)
                            {
                               bFinishInitStartDelay=true;                      //kevin 20180308 add 動作完成
                               iAddInitStartDelayCT=0;
                            }
                            iInitStartDelayCount=iInitStartDelayDec;
                            dwStartInitialCount.LatchCycleTime(true);
                            HTestDeley.SetSecAndOn(iInitStartDelayDec);
                        }
                        else
                        {
                            if(iInitStartDelayTimeCT<TestIF.iInitStartDelayTimeCT)
                            {
                                iInitStartDelayTimeCT++;
                                if(CosFunction.bHaveIndexContactDelay &&
                                   IniConfig.bD29EnableIndexContactDelay)       //Steven 20140519 : [D29] Index下壓後多Delay 0.4秒 (For SPIL Low Yield)
                                {
                                    HTestDeley.SetSecAndOn(TestIF.dInitStartDelayTime+0.4);
                                }
                                else
                                {
                                    HTestDeley.SetSecAndOn(TestIF.dInitStartDelayTime);
                                }
                                bInitStartDelayTime=true;                       //wei 20171020 (jou) InitStartDelayTime秒數倒數
                                dwStartInitialCount.LatchCycleTime(true);
                                iInitStartDelayCount=TestIF.dInitStartDelayTime;
                            }
                            else
                            {
                                if(CosFunction.bHaveIndexContactDelay && IniConfig.bD29EnableIndexContactDelay)  //Steven 20140519 : [D29] Index下壓後多Delay 0.4秒 (For SPIL Low Yield)
                                    HTestDeley.SetSecAndOn(Prod.dTesterStartDelayTime+0.4); //Steven 20140620 : Fix Start Delay Time
                                else
                                    HTestDeley.SetSecAndOn(Prod.dTesterStartDelayTime);
                            }
                        }
                    }
                    else
                    {
                        if(CosFunction.bHaveIndexContactDelay && IniConfig.bD29EnableIndexContactDelay)  //Steven 20140519 : [D29] Index下壓後多Delay 0.4秒 (For SPIL Low Yield)
                            HTestDeley.SetSecAndOn(Prod.dTesterStartDelayTime+0.4); //Steven 20140620 : Fix Start Delay Time
                        else
                            HTestDeley.SetSecAndOn(Prod.dTesterStartDelayTime);
                    }
                }
                else
                {
                    if(CosFunction.bHaveIndexContactDelay && IniConfig.bD29EnableIndexContactDelay)  //Steven 20140519 : [D29] Index下壓後多Delay 0.4秒 (For SPIL Low Yield)
                        HTestDeley.SetSecAndOn(Prod.dTesterStartDelayTime+0.4); //Steven 20140620 : Fix Start Delay Time
                    else
                        HTestDeley.SetSecAndOn(Prod.dTesterStartDelayTime);     //等 motor 穩定 時間 , 應該加到別處 ?????????????
                }
            }
            else
            {
                HTestDeley.SetSecAndOn(0.1);
            }

            if(IniConfig.bG12ContractModeManualMessage)                         //kevin 20180222 Arm 1 Arm2 吸取IC 做CONTRACT MODE
            {
                bContractModeTest=true;                                         //kevin 20180222 contract mode 秀手動送測試訊號
                if(WaitManualStartKey()==false)
                    return false;
                bContractModeTest=false;                                        //kevin 20180222 contract mode 秀手動送測試訊號
            }

            if(IniConfig.bVTESTFunction==true &&                                //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
               IniConfig.bGetRcsCheckingResult==true)
            {
                if(fMesSystem->GetRcsCheckingResult(false)==false)
                    break;
            }

            Task=50;
        case 50:
            if(bInitStartDelayTime)                                             //wei 20171020 (jou) InitStartDelayTime秒數倒數
            {
                if(iInitStartDelayCount>0)
                {
                    if(bFinishInitStartDelay==false)                            //kevin 20180308
                        iInitStartDelayCount=iInitStartDelayDec-dwStartInitialCount.LatchCycleTime()/1000;
                    else
                        iInitStartDelayCount=TestIF.dInitStartDelayTime-dwStartInitialCount.LatchCycleTime()/1000;
                    break;
                }
            }

            if(HTestDeley.Off())
            {
                if(fMain->pgMain->ActivePageIndex==1 &&
                   fMain->pcCommView->ActivePageIndex==0 &&
                   fMain->pgcTorque->ActivePageIndex==1)                        //Steven 20210524 : 連續讀取扭力
                {
                    iCheckTorqueCount++;
                }

                if(fContact->fShow==false)
                    bInitStartDelayNotFinish=false;                             //Ifor 20181220 : add Init Start Delay Time Not Finish

                if(ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true)                         //Ifor 20190130 add 測試前送出Start/End Test 訊息給 ATC
                {
                    ATC_InterfaceForm->StartTesting();                          //JerryYang 20220815 : send ATC start testing
                }

                if(CheckAndRecodrEP(Type))                                      //Steven 20190114 : EP Alarm換位置
                {
                    SetTestTimeOutTimer(Type);                                  //Steven 20230216 : 避免誤發timeout
                    break;
                }

                if(TestIF.iTestType==GPIB_MODE ||
                   LastSet.iTester==OFF_LINE ||
                   TestIF.iTestType==TCP_IP_MODE)                               //wei 20211027 open short TCP/IP
                {
                    if(fMain->bFind==true)
                    {
                        Task=55;
                    }
                    else
                    {
                        TestTimeOut.SetSecAndOn(3);
                        Task=9999;
                        break;
                    }
                }
                else if(TestIF.iTestType==TTL_MODE && (TTL_CARD_TYPE<2))        //Isaac 20200903 :TTL RS232通訊  //Isaac 20210309 :TTL RS232兩塊板子
                {
                    Task=100;
                }
                else //if(TestIF.iTestType==RS232_MODE)  //TTL_CARD_TYPE==2     //Isaac 20200903 :TTL RS232通訊
                {
                    if(IniConfig.bC04EnableTestTempIC)
                    {
                        Task=500;
                        iTempICTask=1;
                    }
                    else
                    {
                        Task=55;                                                //Steven 20100721
                    }
                }
            }

            if(Task!=55)
                break;
//GPIB mode start
        case 55:
            if(SoftStop)
                break;

            if(fMain->bFind==false)                                             //kevin 20150428 避免GPIB關掉
            {
                iSiteNo=0;
                break;
            }

            if(TestSocket.AlreadyTestNotIncludeErrorBin()==false ||             //Steven 20200612 : 修正避免連續測兩次
               (Prod.bD22SupportMultiDoubleContact &&                           //Steven 20201024 : Fixed for double contact
                 //IniConfig.bD22VerifyMode) &&                                 //Sam 20231117 : 整合到 QA 模式  //Sam 20221012 : 新增 VerifyMode 功能
                bDoubleContact))
            {
                IsTest=true;
                if(IniConfig.bI31_1GPIBLotEnd)                                  //wei 20160624 GPIB Lot End Command    //wei 20160726 TSMC GPIB Lot End
                    bGPIBLotEndCommand=true;
                else
                    bGPIBLotEndCommand=false;
                bGPIBLotStartCommand=true;                                      //kevin 20190613 add
                if(TestSocket.HasRealIC())                                      //Steven 20210218 : 修正測試時間的紀錄
                    RecordStartTestTime();

                if(DeviceForm.ContactMode==DropContact)                         //JerryYang 20170503 (wei) 第三段drop contact計時,到達contact高度後吸真空+start delay
                    fObserver->AddTimeData(20, DropContactTimer3.LatchCycleTime()/1000.0);
                if(ATC_SYSTEM==eATC60 || ATC_SYSTEM==eATC30)                    //20141204 ChungHung add for ATC3.0  //2014-05-30    Dell    for ATC6.0
                {
                    ATCInterfaceForm->ATC_60_SYS.SendTestStart(1);
                }

                if(ATC_SYSTEM==eNewATCSystem)                                   //Ifor 20160516 修改ATC Heat 設定數
                    ATC_InterfaceForm->SiteTesting(iATC_Use_Heat_Count, bATCSiteTest);

                for(int i=0; i<MAX_SOCKET_TOTAL; i++)
                {
                    #ifdef ASE_KaohSiung
                    if(IniConfig.bUseAutoSiteMapping && LastSet.iRunStartMode==rsmAutoSiteMap) //kevin 20150113
                    {
                        if(TestIF.iSiteMap[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW]-1>=0)
                            *(flag2+(TestIF.iSiteMap[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW]-1))=(TestIF_File.iSiteMap[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW]>0);
                    }                                                           //送全部測試資料 好判斷 測試機 測試順序
                    else
                    {
                        if(TestIF.iSiteMap[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW]-1>=0)
                            *(flag2+(TestIF.iSiteMap[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW]-1))=flag[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW];
                    }
                    #else
                        if(TestIF.iSiteMap[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW]-1>=0)
                            *(flag2+(TestIF.iSiteMap[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW]-1))=flag[i%MAX_SOCKET_ROW][i/MAX_SOCKET_ROW];
                    #endif
                }

                if(TestIF.iGpibMode==InterfaceType_SPEA_Type)
                {
                    if((BAR_CODE_INSTALL==ebctInShtIntel ||
                        BAR_CODE_INSTALL==ebctUseCCDMode ||
                        BAR_CODE_INSTALL==ebctEtherNetCCD ||                    //Ifor 20190129 : add Cognex EtherNet 通訊
                        BAR_CODE_INSTALL==ebcUseOCR) &&                         //Ifor 20210407 add: 自製OCR
                        TestIF_File.bEnableBarCode)
                    {
                        _RunTestProgram_BarMess(sizeof(flag2), flag2, StrBarCodeAll.Length(), StrBarCodeAll.c_str());
                    }
                    else
                    {
                        _RunTestProgram(sizeof(flag2), flag2);
                    }
                }
                else
                {
                    fMain->RunTestProgram(true, flag2);
                }

                if(ATC_SYSTEM==eATCHonPrecType)                                 //Steven 20120410 : Hontech ATC
                {
                    ATCInterfaceForm->SendTestStart(Type);
                }
            }
            else                                                                //Steven 20200618 : 紀錄被連續測兩次的Bin
            {
                Str="";
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestSocket.Item[i][j]!=NULL_IC &&
                           TestSocket.Item[i][j]!=HAS_NULL_IC)
                        {
                            Str1.sprintf("Site[%d][%d]=%d,", i, j, TestSocket.Item[i][j]);
                            Str+=Str1;
                        }
                    }
                }
                RecordProcess(Str);
            }

            SetTestTimeOutTimer(Type);                                          //JerryYang 20200623 避免誤發timeout
            SOTPauseIntervalsTime.LatchCycleTime(true);                         //kevin 20181102 (Steven) : 取得機台停止時間
            bSOTToLongStopBlowAir=true;                                         //kevin 20181102 (Steven) : add 上次測試訊號太久 需停止吹氣

            bEPaddMSec=false;                                                   //jou 20171026 (wei) : 測試中加壓EP
            TestStopMotorTimer.LatchCycleTime(true);
            if(LastSet.iTester==OFF_LINE)
            {
                IsTest=true;                                                    //Steven 20130703
                iCurrentTime=0;
                NULL_Delay.SetSecAndOn(Prod.iTesterDummyTime);
                Task=65;
                break;
            }
            else
            {
                Task=60;
            }
        case 60:
            if(bPauseTester)
            {
                bPauseTester=false;
                SetTestTimeOutTimer(Type);                                      //Steven 20200407 : 整合Time Out時間設定
            }

            if(IniConfig.bEnableTestingNeedStopAllMotor==true &&
               IniConfig.bI24TestingNeedStopAllMotor==true)                     //jou 2013-09-25 Testing Need Stop All Motor
            {
                dwTestTime=TestStopMotorTimer.LatchCycleTime()/100.0;
                if(dwTestTime>=(unsigned int)((TestIF.dInitWaitTime-0.5)*10) && dwTestTime<(unsigned int)((TestIF.dInitWaitTime+TestIF.dTestingWaitTime)*10))
                {
                    bTestingStopAllMotor=true;                                  //jou 2013-09-25 Testing Need Stop All Motor
                }
                else
                {
                    bTestingStopAllMotor=false;                                 //jou 2013-09-25 Testing Need Stop All Motor
                }
            }

            if(CosFunction.bUploadTestArmTorqueLog)                             //Ifor 20190912 :add 海思 V02.30 版 Record Torque
            {
                bGetTestArmTorque=false;
                if(bFrontTestArmTorqueFinish==false)
                {
                    if(fMain->edTorue0->Text!="")
                    {
                        bFrontTestArmTorqueFinish=true;
                        bGetTestArmTorque=true;
                    }
                }
                if(bRearTestArmTorqueFinish==false)
                {
                    if(fMain->edTorue1->Text!="")
                    {
                        bRearTestArmTorqueFinish=true;
                        bGetTestArmTorque=true;
                    }
                }

                if(bGetTestArmTorque==true)
                {
                    sArmTestInfoEvenLogFile=FormHS->GetLastFileLogName_HS(5);
                    FormHS->RecordArmTestInfoLog_HS(sArmTestInfoEvenLogFile);
                }
            }

            if(IniConfig.bIndexAddPressEP==true)                                //jou 20171026 (wei) : 測試中加壓EP
            {
                bTestEPaddKg=true;
                dTestTime=StrToFloat(FormatFloat("0.0", double(TestStopMotorTimer.LatchCycleTime())/1000.0));

                if(dGetSec!=dTestTime && dTestTime<=IniConfig.iIndexAddPressEP_Time && dTestTime>0)
                {
                    if(DeviceForm.fAireForce!=0)
                    {
                        dGetSec=dTestTime;
                        iEPaddSec=dTestTime*10.0/10.0;
                        bEPaddMSec=!bEPaddMSec;
                        if(bEPaddMSec==true)
                            dEPaddMSec=+IniConfig.dIndexVibrateEP_Kg;
                        else
                            dEPaddMSec=-IniConfig.dIndexVibrateEP_Kg;

                        ADAM_WriteVoltage(DeviceForm.fAireForce+iEPaddSec*IniConfig.dIndexAddPressEP_Kg+dEPaddMSec);
                    }
                }
            }

            if(bEcho &&
               bTimeOutForNoFullSite==false)                                    //Steven 20141016 : FullSite的Test Time Out
            {
                iContractCount++;                                               //kevin 20180928 add contract count
                if(IniConfig.bIndexAddPressEP==true)                            //jou 20171026 (wei) : 測試中加壓EP
                {
                    bTestEPaddKg=false;
                    if(DeviceForm.fAireForce!=0)
                        ADAM_WriteVoltage(DeviceForm.fAireForce);
                }

                //iSendGpibTestHome=false;                                      //kevin 20150626 送出測試訊號 等收到資料才能歸home
                bTestingStopAllMotor=false;                                     //jou 2013-09-25 Testing Need Stop All Motor

                if(ATC_SYSTEM==eATCHonPrecType)                                 // 2011.05.24 , Joye , ATC ----------- //Steven 20120410 : Hontech ATC
                {
                    ATCInterfaceForm->SendTestEnd(Type);
                }

                if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70 &&
                   Temperature.bATC7TSDFunction==true)                          //Steven 20160604 : by site TSD
                {
                    iSiteOn[0]=0;
                    iSiteOn[1]=0;
                    iSiteOn[2]=0;
                    iSiteOn[3]=0;
                    ATC_InterfaceForm->UseTSD_Function(4, iSiteOn);             //Ifor 20160823 add Site Count
                }

                if(ATC_SYSTEM==eATC60 ||                                        //2014-05-30    Dell    for ATC6.0
                   ATC_SYSTEM==eATC30)                                          //20141204 ChungHung add for ATC3.0
                {
                    ATCInterfaceForm->ATC_60_SYS.SendTestStart(0);
                }

                bEcho=false;
                if(bGPIBError)
                {
                    ShowMyMessage("GPIB Format Error", "GPIB 格式錯誤");
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            iTesterBIN[i][j]=iTestBinCount;                     //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        }
                    }
                    return true;
                }

                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestIF_File.iCloseSiteOnHPDontTest==2 &&             //Steven 20250604 : 關site的位置有IC不測試送指定 bin
                           TestSocket.iBinData[i][j]==iTestBinCount &&
                           TestSocket.PordRec[i][j].asBuffer->Strings[eErrorCode].AnsiPos("NonTestToSettedBin")!=0)
                        {
                            iBin[i][j]=TestIF_File.iCloseSiteBin;
                            iTesterBIN[i][j]=TestIF_File.iCloseSiteBin;
                        }
                        else if(iBin[i][j]==257)                                //kevin 20140319  bin0
                        {
                            iTesterBIN[i][j]=0;
//                            bIndexDutflag[Type][i][j]=true;                   //kevin 20140318 紀錄目前index有IC需測試
                        }
                        else if(iBin[i][j]==0)                                  //kevin 20140319  bin0
                        {
                            iTesterBIN[i][j]=-1;
                        }
                        else if(iBin[i][j]==-1)                                 //Steven 20150408 : 針對-1 Bin進行處理
                        {
                            bHasError=true;
                            iTesterBIN[i][j]=iTestBinCount;
                        }
                        else
                        {
                            if(TestIF_File.bEnableBarCode)
                            {
                                if(LastSet.iTester==_2D_SORT &&
                                   CosFunction.bSortingBy2DList &&
                                   TestIF_File.bSortingBy2DIDList)              //Frank 20221122 : 2DID sorting for ATK
                                {
                                    Str.sprintf("%s", TestSocket.cDeviceInf[i][j]);
                                    if(Str==asBarCodeErrorSend || Str=="")      //wei 20160318 Barcode Error依客戶設定
                                    {
                                        iBin[i][j]=iTestBinCount;
                                    }
                                    else
                                    {
                                        iTempBin=fBarCode->GotBinFrom2DSortList(Str);   //Steven 20240515 : modified for 2D sort

                                        if(iTempBin==iTestBinCount)
                                        {
                                            TestSocket.PordRec[i][j].AddErrorRecordNoSave("2DID is not in sorting list");

                                            if(TestIF_File.iActionOf2DNotInList==0)     //Steven 20250707 : Action Of 2D Not In List
                                            {
                                                ShowErrorMessage("WAR16329", K_SKIP, MMSystem);
                                                fMain->BtnOneCycleClick(fMain);
                                            }

                                            if(IniConfig.bEnable_SECS_GEM==true)
                                            {
                                                EventReport(SECS_EVENT.UnexpectedUNITIDRead);
                                            }
                                        }
                                        iBin[i][j]=iTempBin;
                                    }
                                }
                                else
                                {
                                    if(TestIF_File.bSearch2DIDByLot &&
                                       LastSet.iTester==OFF_LINE)               //Frank 20170316 (wei) add Search 2DID By Lot
                                    {
                                        ret=fBarCode->b2DIDIsInsideList(i, j);
                                        if(ret==0)                              //沒找到
                                        {
                                            iBin[i][j]=TestIF_File.b2DIDListErrorBin;
                                        }
                                        else if(ret==2)                         //BarcodeError
                                        {
                                            iBin[i][j]=iTestBinCount;
                                        }
                                    }
                                    else if(TestIF_File.b2DIDNotExist2Error &&
                                            LastSet.iTester==OFF_LINE)          //JerryYang 20231218 : 2DID黑名單功能
                                    {
                                        ret=fBarCode->b2DIDIsInsideToErrorBin(i, j);
                                        if(ret==1)                              //有找到
                                        {
                                            iBin[i][j]=iTestBinCount;
                                            TestSocket.PordRec[i][j].AddErrorRecordNoSave("Search 2DID in the ERR list");
                                        }
                                    }

                                    if(TestIF_File.iNoCodeDeviceToErr!=0)       //Steven 20151221 : 將讀取異常的IC放到Error Bin
                                    {
                                        Str.sprintf("%s", TestSocket.cDeviceInf[i][j]);
                                        if(Str==asBarCodeErrorSend || Str=="" || Str==asBarCodeErrorCheckSum)  //KaiChen 20191121 ：中壢日月光 2D Check Sum      //wei 20160318 Barcode Error依客戶設定
                                        {
                                            iBin[i][j]=iTestBinCount;
                                        }
                                    }
                                }
                            }

                            iTesterBIN[i][j]=iBin[i][j];                        //-1;
//                            bIndexDutflag[Type][i][j]=true;                     //kevin 20140318 紀錄目前index有IC需測試
                        }
                        iBin[i][j]=0;                                           //Steven 20150306 : 使用完畢就重置變數，避免以後又發生分Bin異常
                    }
                }

                if(bHasError)                                                   //Steven 20150408 : 針對-1 Bin進行處理
                {
                    ShowMyMessage("GPIB Format Error", "GPIB 格式錯誤");
                    for(int i=0; i<MAX_SOCKET_ROW; i++)
                    {
                        for(int j=0; j<MAX_SOCKET_COL; j++)
                        {
                            iTesterBIN[i][j]=iTestBinCount;                     //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        }
                    }
                }
                return true;
            }

            if(TestTimeOut.Off())
            {
                bTestingStopAllMotor=false;                                     //jou 2013-09-25 Testing Need Stop All Motor
                if(TestIF.iTestType==TTL_MODE && (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))    //Isaac 20200903 :TTL RS232通訊
                {
                    if(IniConfig.bI12TesterTimerOutNotNeedReTest)//No resend SOT
                    {
                        Task=60;
                    }
                    else
                    {
                        Task=55;
                    }
                }
                else
                {
                    if(bExist==false)
                    {
    //                    fMain->WakeupGPIB("GetTesterResult");                 //wei 20150703 因為RS232會斷線
                        Task=55;
                    }
                }
            }
            break;
        case 65:
            if(IniConfig.bEnableTestingNeedStopAllMotor==true &&
               IniConfig.bI24TestingNeedStopAllMotor==true)                     //jou 2013-09-25 Testing Need Stop All Motor
            {
                dwTestTime=TestStopMotorTimer.LatchCycleTime()/100.0;
                if(dwTestTime>=(unsigned int)((TestIF.dInitWaitTime-0.5)*10) &&
                   dwTestTime<(unsigned int)((TestIF.dInitWaitTime+TestIF.dTestingWaitTime)*10))
                {
                    bTestingStopAllMotor=true;                                  //jou 2013-09-25 Testing Need Stop All Motor
                }
                else
                {
                    bTestingStopAllMotor=false;                                 //jou 2013-09-25 Testing Need Stop All Motor
                }
            }

            if(CosFunction.bUploadTestArmTorqueLog)                             //Ifor 20190912 :add 海思 V02.30 版 Record Torque
            {
                bGetTestArmTorque=false;
                if(bFrontTestArmTorqueFinish==false)
                {
                    if(fMain->edTorue0->Text!="")
                    {
                        bFrontTestArmTorqueFinish=true;
                        bGetTestArmTorque=true;
                    }
                }

                if(bRearTestArmTorqueFinish==false)
                {
                    if(fMain->edTorue1->Text!="")
                    {
                        bRearTestArmTorqueFinish=true;
                        bGetTestArmTorque=true;
                    }
                }

                if(bGetTestArmTorque==true)
                {
                    sArmTestInfoEvenLogFile = FormHS->GetLastFileLogName_HS(5);
                    FormHS->RecordArmTestInfoLog_HS(sArmTestInfoEvenLogFile);
                }
            }

            if(IniConfig.bIndexAddPressEP==true)                                //Ifor 20190912 :add 海思 V02.30 版 Record Torque
            {                                                                   //jou 20171026 (wei) : 測試中加壓EP
                bTestEPaddKg=true;
                dTestTime=StrToFloat(FormatFloat("0.0", double(TestStopMotorTimer.LatchCycleTime())/1000.0));

                if(dGetSec!=dTestTime &&
                   dTestTime<=IniConfig.iIndexAddPressEP_Time && dTestTime>0)
                {
                    if(DeviceForm.fAireForce!=0)
                    {
                        dGetSec=dTestTime;
                        iEPaddSec=dTestTime*10.0/10.0;
                        bEPaddMSec=!bEPaddMSec;
                        if(bEPaddMSec==true)
                            dEPaddMSec=+IniConfig.dIndexVibrateEP_Kg;
                        else
                            dEPaddMSec=-IniConfig.dIndexVibrateEP_Kg;

                        ADAM_WriteVoltage(DeviceForm.fAireForce+iEPaddSec*IniConfig.dIndexAddPressEP_Kg+dEPaddMSec);
                    }
                }
            }

            if(NULL_Delay.Off())
            {
                EndTestTimeStamp(1);
                IsTest=false;                                                   //Steven 20130703
                Task=60;
            }
            break;
//GPIB mode end
//TTL mode start
        case 100:                                                               // TTL use 應該事先清
            if(ATC_SYSTEM==eATC60 || ATC_SYSTEM==eATC30)                        //20141204 ChungHung add for ATC3.0   //2014-05-30    Dell    for ATC6.0
            {
                ATCInterfaceForm->ATC_60_SYS.SendTestStart(1);
            }

            if(LastSet.iTester==OFF_LINE)                                       //Steven 20091031 Add for TTL offline testing
            {
                Task=300;
                break;
            }
            SetNoiseDelay=false;
            SW[SwClear0].On();
            SW[SwClear4].On();                                                  //Alick 20161011 (Steven) : TTL支援8Site

            TTLLog("GetTesterResult 100");                                      //Steven 20151123 : Log for TTL
            Task=200;
            break;
        case 200:
//            SW[TTL_ClearData[0]].Off();
            SW[SwClear0].Off();                                                 //Frank 20170424 (Steven) : 修正TTL Clear訊號 On --> Off
            SW[SwClear4].Off();                                                 //Alick 20161011 (Steven) : TTL支援8Site

            TTLLog("GetTesterResult 200");                                      //Steven 20151123 : Log for TTL
            Task=250;

            for(int i=0; i<1000; i++)                                           //2008/10/02 lee start // only delay
            {
                ;
            }
            //break;                                                            //2008/10/02 lee start
        case 250:
//            SW[TTL_ClearData[0]].On();
            SW[SwClear0].On();
            SW[SwClear4].On();                                                  //Alick 20161011 (Steven) : TTL支援8Site

            TTLLog("GetTesterResult 250");                                      //Steven 20151123 : Log for TTL
#ifndef SOFT_SIMULTE
            for(int i=0; i<TestSocket.iShtRow; i++)                             //Steven 20241015 : InArmSuck --> TestSocket
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(Test_GetTestResul(i, j)!=-1)
                    {
                        fMain->Pause("TTL Clear");
                        ShowMyMessage("Interface Board can not clear", "TTL介面卡無法清空資料");
                        Task=100;
//                        bPauseTest=true;
                        return false;
                    }
                }
            }
#endif
            if(TestSocket.HasRealIC())                                          //Steven 20210218 : 修正測試時間的紀錄
                RecordStartTestTime();

            bATC_SITE_2ND_CHECK[Type]=true;                                     //Ifor 20160509 add ATC 測試時開啟第二點溫度監控
            if(ATC_SYSTEM==eNewATCSystem)                                       //Ifor 20160516 修改ATC Heat 設定數
            {
                ATC_InterfaceForm->SiteTesting(iATC_Use_Heat_Count, bATCSiteTest);
            }
            else if(ATC_SYSTEM==eATCHonPrecType)
            {
                ATCInterfaceForm->SendTestStart(Type);
            }
            Task=260;
        case 260:
            if(CUSTOMER_CODE==CC_AMKOR_Korea && SystemStart==false)             //Steven 20151214 : 先針對ATK做修改, 避免影響別人
                return false;

            if(Prod.DIOCfg.iDutType!=DUTNONE)                                   //use Dut signal
            {
                iStartStep=0;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        iCH=j+i*MAX_Index_Row;
                        if(Prod.DIOCfg.iDutType==DUTPosPluse ||
                           Prod.DIOCfg.iDutType==DUTPosLevel)
                        {
                            if(flag[i][j])
                                SW[TTL_Dut[iCH]].Off();                         // start Test
                        }
                        else
                        {
                            if(flag[i][j])
                                SW[TTL_Dut[iCH]].On();                          // start Test
                        }
                    }
                }

                if(CosFunction.bTTLUseUSec)                                     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
                    DutDelay.SetUSAndOn(Prod.DIOCfg.iDutBfOnTime);
                else
                    DutDelay.SetMSAndOn(Prod.DIOCfg.iDutBfOnTime);
                Task=265;
            }
            else
            {
                Task=270;
            }
            TTLLog("GetTesterResult 260");                                      //Steven 20151123 : Log for TTL
        case 265:
            if(DutDelay.Off())
                Task=270;
            break;
        case 270:
            iStartStep=1;
            if(Prod.DIOCfg.iStartType==CHSingle)
            {
                bClearSrtart[0]=true;
                if(CosFunction.bTTLUseUSec)                                     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
                {
                    MyTTLSOTTimer.SetUSAndOn(Prod.DIOCfg.iSTPluseWidth);
                }
                else
                {
                    MyTTLSOTTimer.SetMSAndOn(Prod.DIOCfg.iSTPluseWidth);
                }

                SW[TTL_StartData[Prod.DIOCfg.iOneSTChannel]].On();              //only this signal if handler request other please change wire in hardware
            }
            else
            {
                fMain->mmo1->Lines->Clear();
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(flag[i][j])
                        {
                            if(TestIF.iTestMode>=_6Site2X3)                     //Alick 20161011 (Steven) : TTL支援8Site
                                iCH=j+i*4;
                            else
                                iCH=j+i*MAX_Index_Row;

                            bClearSrtart[iCH]=true;

                            if(Prod.DIOCfg.iSTPluseWidth>=500)                  //Eliot 2008_06_10  100->3000  //Steven 20170106 (Jou) : 3000 --> 500
                                Prod.DIOCfg.iSTPluseWidth=500;
                            if(Prod.DIOCfg.iSTPluseWidth<10)                    //Steven 20170106 (Jou) : 1 --> 10
                                Prod.DIOCfg.iSTPluseWidth=10;

                            if(CosFunction.bTTLUseUSec)                         //Steven 20180808 (wei) : TTL的時間單位改成microsecond
                            {
                                MyTTLSOTTimer.SetUSAndOn(Prod.DIOCfg.iSTPluseWidth);
                            }
                            else
                            {
                                MyTTLSOTTimer.SetMSAndOn(Prod.DIOCfg.iSTPluseWidth);
                            }

                            SW[TTL_StartData[iCH]].On();                        // start Test
                            SW[TTL_StartData[iCH]].On();                        // start Test
                            SW[TTL_StartData[iCH]].On();                        // start Test

                            sprintf(str2, "Start[%d][%d]  %x %01d", i, j, SW[TTL_StartData[iCH]].Port, SW[TTL_StartData[iCH]].Bit);
                            fMain->mmo1->Lines->Add(str2);                      //Lee 2010-03-23 start : Record TTL data
                            fMain->mmo1->Lines->Add("");
                            for(int k=0; k<5; k++)
                            {
                                sprintf(str2, "Sensor[%d]  %x %01d", k, Sen[TTL_Sensor[iCH][k]].Port, Sen[TTL_Sensor[iCH][k]].Bit);
                                fMain->mmo1->Lines->Add(str2);
                            }
                            fMain->mmo1->Lines->Add( "");
#ifdef SOFT_SIMULTE
                            Sim_TTL_Single(i, j);
#endif
                        }
                    }
                }
            }
            TTLLog("GetTesterResult 270-1");                                    //Steven 20151123 : Log for TTL

            while(1)
            {
                for(int i=0; i<8; i++)                                          //Alick 20161011 (Steven) : TTL支援8Site
                {
                    if(bClearSrtart[i])
                    {
                        if(MyTTLSOTTimer.Off())                                 //Steven 20180808 (wei) : TTL的時間單位改成microsecond
                        {
                            bClearSrtart[i]=false;
                            SW[TTL_StartData[i]].Off();
                            if(Prod.DIOCfg.iDutType==DUTPosPluse || Prod.DIOCfg.iDutType==DUTNegPluse)
                            {
                                if(CosFunction.bTTLUseUSec)                     //Steven 20180808 (wei) : TTL的時間單位改成microsecond
                                    DutDelay.SetUSAndOn(Prod.DIOCfg.iDutAfOffTime);
                                else
                                    DutDelay.SetMSAndOn(Prod.DIOCfg.iDutAfOffTime);
                            }
                        }
                    }
                }

                Application->ProcessMessages();
                if(bClearSrtart[0]==false && bClearSrtart[1]==false && bClearSrtart[2]==false && bClearSrtart[3]==false &&        //Alick 20161011 (Steven) : TTL支援8Site
                   bClearSrtart[4]==false && bClearSrtart[5]==false && bClearSrtart[6]==false && bClearSrtart[7]==false)
                    break;
            }

            TTLLog("GetTesterResult 270-2");                                    //Steven 20151123 : Log for TTL
            for(int i=0; i<8; i++)                                              //Steven 20161011 : TTL支援8Site 4 --> 8
            {
                SW[TTL_StartData[i]].Off();
                bClearSrtart[i]=false;
            }
            TTLLog("GetTesterResult 270-3");                                    //Steven 20151123 : Log for TTL
            Task=300;
            break;
        case 300:
            if(ATC_SYSTEM==eATC60 || ATC_SYSTEM==eATC30)                        //20141204 ChungHung add for ATC3.0   //2014-05-30    Dell    for ATC6.0
            {
                ATCInterfaceForm->ATC_60_SYS.SendTestStart(0);
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(flag[i][j]==true)
                    {
                        if(Test_GetTestResul(i, j)==-1)                         // test not finish
                            return false;
                    }
                }
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
                for(int j=0; j<TestSocket.iShtCol; j++)
                    if(flag[i][j])
                        iTesterBIN[i][j]=Test_GetTestResul(i, j);               //jou 2014-06-26 修正TTL BIN1,Handler出現BIN0 errir

            for(int i=0; i<8; i++)                                              //Alick 20161011 (Steven) : TTL支援8Site
            {
                SW[TTL_StartData[i]].Off();
                bClearSrtart[i]=false;
            }

            if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70 &&
               Temperature.bATC7TSDFunction==true)                              //Steven 20160604 : by site TSD
            {
                iSiteOn[0]=0;
                iSiteOn[1]=0;
                iSiteOn[2]=0;
                iSiteOn[3]=0;
                ATC_InterfaceForm->UseTSD_Function(4, iSiteOn);                 //Ifor 20160823 add Site Count
            }

            TTLLog("GetTesterResult 300-3");                                    //Steven 20151123 : Log for TTL
            return true;
//TTL mode end
//Temp RS232 start
        case 500:
            if(GetTempICResult())
            {
                return true;
            }
            break;
//Temp RS232 end
        case 9999:
            if(TestTimeOut.Off())
            {
                Task=1;
            }
            break;
//Steven 20160604 start: by site TSD
        case 20000:
            TSDDelay.SetSecAndOn(Temperature.dATC7TSDTimeOut);
            iSiteOn[0]=(Type==0 && (FTestSuck.Item[0][0]==HAS_IC || FTestSuck.Item[0][0]==HAS_HOT_IC))?1:0;
            iSiteOn[1]=(Type==0 && (FTestSuck.Item[0][1]==HAS_IC || FTestSuck.Item[0][1]==HAS_HOT_IC))?1:0;//Ifor 20160804 FTestSuck.Item[0][0] ==> FTestSuck.Item[0][1]
            iSiteOn[2]=(Type==1 && (BTestSuck.Item[0][0]==HAS_IC || BTestSuck.Item[0][0]==HAS_HOT_IC))?1:0;//Ifor 20160804 BTestSuck.Item[0][1] ==> BTestSuck.Item[0][0]
            iSiteOn[3]=(Type==1 && (BTestSuck.Item[0][1]==HAS_IC || BTestSuck.Item[0][1]==HAS_HOT_IC))?1:0;
            ATC_InterfaceForm->UseTSD_Function(4, iSiteOn);                     //Ifor 20160823 add Site Count
            Task=20010;
            break;
        case 20010:
            if(ATC_InterfaceForm->bGetATC_SEND_TEMP_READY==true)
            {
                HTestDeley.SetSecAndOn(0.01);
                Task=50;                                                        //Steven 20161019 : 20 --> 50 for TSD
                Str.sprintf("%6.3f", MyTSDTimer.LatchCycleTime()/1000.0);
                fMain->memoTSD->AddTextWithDateTime(Str);                       //Steven 20161208 (jou) : Record TSD time data

                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        TestSocket.PordRec[i][j].AddTSDTime(Str);
                    }
                }

                if(bNeedInitialTestDelay)                                       //Steven 20161201 : Fixed for ATC7.0 hangup with TSD function
                {
                    if(iInitialCount>0)
                    {
                        iInitialCount=(Prod.iInitialDelay)-dwStartInitialCount.LatchCycleTime()/1000;
                        break;
                    }
                }
                bNeedInitialTestDelay=false;
            }
            else if(TSDDelay.Off())
            {
                ret=ShowErrorMessage("WAR15183", K_RETRY|K_SKIP, MTestY1+Type);
                if(ret==K_RETRY)
                {
                    Task=20000;
                    SetTestTimeOutTimer(Type);                                  //Steven 20200407 : 整合Time Out時間設定
                    HangTime.SetSecAndOn(Prod.iHangupMaxTime);                  //Steven 20180829 : #P180827-ATK-H9-03 It happened "Tester Time Up Error" after "TSD Wait Time Error".
                }
                else
                {
                    HTestDeley.SetSecAndOn(0.01);

                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            iTesterBIN[i][j]=iTestBinCount;                     //Steven 20161025 : 無完成TSD的,要送去Err Bin
                        }
                    }
                    return true;
                }
            }
            break;
//Steven 20160604 End: by site TSD
        //Steven 20180817 : Boost Function
        //==>
        case 30000:
            if(bUnderTest)
            {
                Task=10;
                break;
            }

            iBoostFuncStep=0;
            iBoostEotToSotTime=TestIntervalsBoostTime.LatchCycleTime()/1000.0;  //JerryYang 20181122 (Steven) : 將不同function計時器分開
            if(iTriggerBoostFunction==Temperature.eBMid)
            {
                Temperature.dBoostOffset[iTriggerBoostFunction]         =((Temperature.dBoostOffset[0]-Temperature.dBoostOffset[2])/(Temperature.dBoostIdleTime[0]-Temperature.dBoostIdleTime[2]))*(iBoostEotToSotTime-Temperature.dBoostIdleTime[2])+Temperature.dBoostOffset[2];
                Temperature.dBoostDuration[iTriggerBoostFunction]       =((Temperature.dBoostDuration[0]-Temperature.dBoostDuration[2])/(Temperature.dBoostIdleTime[0]-Temperature.dBoostIdleTime[2]))*(iBoostEotToSotTime-Temperature.dBoostIdleTime[2])+Temperature.dBoostDuration[2];
                Temperature.dPostBoostDuration[iTriggerBoostFunction]   =((Temperature.dPostBoostDuration[0]-Temperature.dPostBoostDuration[2])/(Temperature.dBoostIdleTime[0]-Temperature.dBoostIdleTime[2]))*(iBoostEotToSotTime-Temperature.dBoostIdleTime[2])+Temperature.dPostBoostDuration[2];
            }

            fTemp_Set->edtIdleTime_Mid->Text        =FormatFloat("0.0", iBoostEotToSotTime);
            fTemp_Set->edtBoostOffset_Mid->Text     =FormatFloat("0.0", Temperature.dBoostOffset[iTriggerBoostFunction]);
            fTemp_Set->edtBoostDuration_Mid->Text   =FormatFloat("0.0", Temperature.dBoostDuration[iTriggerBoostFunction]);
            fTemp_Set->edtPostBoost_Mid->Text       =FormatFloat("0.0", Temperature.dPostBoostDuration[iTriggerBoostFunction]);

            fLotInfo->SetATCOffset(true);                                       //Steven 20180817 : Boost Function
//            bSetTempChange=true;
            fHeaterOK=false;
            if(Temperature.bATCActiveCooling)
                HTestDeley.SetSecAndOn(5);
            else
                HTestDeley.SetSecAndOn(10);

            if(fContact->fShow)
                fContact->pnlHandlerSatus->Visible=true;

            if(iTriggerBoostFunction==Temperature.eBLBL)
            {
                Task=30001;
                LBBoostTimeOutTimer.SetSecAndOn(Temperature.dBoostTimeOut);     //Steven 20181222 : Add LB升溫的Time out
            }
            else if(Temperature.iBoostFunctionMode==0)                          //No heater wait
            {
                Task=30100;
            }
            else if(Temperature.iBoostFunctionMode==1)                          //Wait Heat up and down
            {
                Task=31100;
            }
            else if(Temperature.iBoostFunctionMode==2)                          //Wait Heat up only
            {
                Task=32100;
            }
            else
            {
                Task=32100;
            }

            break;
        case 30001:                                                             //Steven 20181102 : LB Temp Function
            if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60)
                fTemp_Set->ControlATC60AirFlow(1);

            Task=30002;
        case 30002:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定

            if(UN150Read[tcLB]>=Temperature.dBoostIdleTime[Temperature.eBLBL]+Temperature.dThreshold)  //Steven 20190927 : 要比L/B溫度多1.5度, 比較不會在一開始溫度比較低的時候降過頭
            {
                iTriggerBoostFunction=Temperature.eBLBI;
                Task=30000;
            }
            else if(LBBoostTimeOutTimer.Off())                                  //Steven 20181222 : Add LB升溫的Time out
            {
                bLBBoostTimeOut=true;
                if(IniConfig.bSPILFunction)                                     //JerryYang 20220126 : 矽品要求boost timeout要跳異常訊息
                {
                    ShowMyMessage("LB boost time out.","");
                }
                else
                {
                    RecordProcess("LB boost time out.");
                }
                Task=10;
            }
            break;
        case 30100:                                                             //不等Heater OK
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            RecordProcess("Start boost duration.");
            if(fContact->fShow)
                fContact->labDelayStatus->Caption="Start boost duration.";
            iInitialCount=Temperature.dBoostDuration[iTriggerBoostFunction];
            HTestDeley.SetSecAndOn(Temperature.dBoostDuration[iTriggerBoostFunction]);
            TSDDelay.LatchCycleTime(true);
            iBoostFuncStep=0;                                                   //補Offset
            Task=30200;
            break;
        case 30200:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            if(HTestDeley.Off())
            {
                iBoostFuncStep=3;
                RecordProcess("Start post boost duration.");
                if(fContact->fShow)
                    fContact->labDelayStatus->Caption="Start post boost duration.";
                fHeaterOK=false;
                bSetTempChange=true;
                TSDDelay.LatchCycleTime(true);
                HTestDeley.SetSecAndOn(Temperature.dPostBoostDuration[iTriggerBoostFunction]);
                Task=30300;
            }
            else
            {
                iInitialCount=Temperature.dBoostDuration[iTriggerBoostFunction]-TSDDelay.LatchCycleTime()/1000;
                if(fContact->fShow)
                    fContact->lblCountDown->Caption=iInitialCount;

                iBoostFuncStep=0;
            }
            break;
        case 30300:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            if(HTestDeley.Off())
            {
                RecordProcess("Boost duration finish.");
                iTriggerBoostFunction=-1;
                iBoostFuncStep=5;
                HTestDeley.SetSecAndOn(0.01);
                Task=50;
                if(fContact->fShow)
                    fContact->pnlHandlerSatus->Visible=false;
            }
            else
            {
                iInitialCount=Temperature.dPostBoostDuration[iTriggerBoostFunction]-TSDDelay.LatchCycleTime()/1000;
                if(fContact->fShow)
                    fContact->lblCountDown->Caption=iInitialCount;
                iBoostFuncStep=4;
            }
            break;
        case 31100:                                                             //兩個都要等
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            fHeaterOK=false;
            if(HTestDeley.Off())
            {
                Task=31150;
            }
            break;
        case 31150:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            if(fHeaterOK && HTestDeley.Off())
            {
                RecordProcess("Start boost duration.");
                if(fContact->fShow)
                    fContact->labDelayStatus->Caption="Start boost duration.";
                iInitialCount=Temperature.dBoostDuration[iTriggerBoostFunction];
                HTestDeley.SetSecAndOn(Temperature.dBoostDuration[iTriggerBoostFunction]);
                TSDDelay.LatchCycleTime(true);
                iBoostFuncStep=1;
                Task=31200;
            }
            else
            {
                if(fContact->fShow)
                    fContact->lblCountDown->Caption="Heater wait.";
            }
            break;
        case 31200:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            if(fHeaterOK && HTestDeley.Off())
            {
                iBoostFuncStep=3;
                RecordProcess("Finish boost duration.");
                fHeaterOK=false;
                bSetTempChange=true;
                if(Temperature.bATCActiveCooling)
                    HTestDeley.SetSecAndOn(5);
                else
                    HTestDeley.SetSecAndOn(10);
                Task=31300;
            }
            else
            {
                iInitialCount=Temperature.dBoostDuration[iTriggerBoostFunction]-TSDDelay.LatchCycleTime()/1000;
                if(fContact->fShow)
                    fContact->lblCountDown->Caption=iInitialCount;

                iBoostFuncStep=1;
            }
            break;
        case 31300:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            fHeaterOK=false;
            if(HTestDeley.Off())
            {
                Task=31350;
            }
            if(fContact->fShow)
                fContact->labDelayStatus->Caption="Heater Wait";
            break;
        case 31350:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            if(fHeaterOK)
            {
                iBoostFuncStep=4;
                RecordProcess("Start post boost duration.");
                TSDDelay.LatchCycleTime(true);
                HTestDeley.SetSecAndOn(Temperature.dPostBoostDuration[iTriggerBoostFunction]);
                Task=31400;
            }
            break;
        case 31400:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定
            if(HTestDeley.Off())
            {
                RecordProcess("Boost duration finish.");
                iTriggerBoostFunction=-1;
                iBoostFuncStep=5;
                HTestDeley.SetSecAndOn(0.01);
                Task=50;
                if(fContact->fShow)
                    fContact->pnlHandlerSatus->Visible=false;
            }
            else
            {
                iInitialCount=Temperature.dPostBoostDuration[iTriggerBoostFunction]-TSDDelay.LatchCycleTime()/1000;
                if(fContact->fShow)
                    fContact->lblCountDown->Caption=iInitialCount;
                iBoostFuncStep=4;
            }
            break;
        case 32100:                                                             //只等升溫
            fHeaterOK=false;
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定

            if(HTestDeley.Off())
            {
                bFlagBelowTurnOfValve=true;
                iBoostFuncStep=0;
                Task=32150;
            }
            break;
        case 32150:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定

            ret=CheckToBoostIndexTemp();                                        //Steven 20181222 : Add 當升溫到一半,發現LB溫度不足,要重新補溫度
            if(ret==4)
            {
                Task=10;
                iTriggerBoostFunction=-1;
                iTriggerBoostFuncBack=-1;
                break;
            }

            if(fHeaterOK && HTestDeley.Off())
            {
                bFlagBelowTurnOfValve=false;
                if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60)
                    fTemp_Set->ControlATC60AirFlow(0);

                RecordProcess("Start boost duration.");
                if(fContact->fShow)
                    fContact->labDelayStatus->Caption="Start boost duration.";
                iInitialCount=Temperature.dBoostDuration[iTriggerBoostFunction];
                HTestDeley.SetSecAndOn(Temperature.dBoostDuration[iTriggerBoostFunction]);
                TSDDelay.LatchCycleTime(true);
                iBoostFuncStep=1;
                Task=32200;
            }
            else
            {
                if(fContact->fShow)
                    fContact->lblCountDown->Caption="Heater wait.";
            }
            break;
        case 32200:
            SetTestTimeOutTimer(Type);                                          //Steven 20200407 : 整合Time Out時間設定

            ret=CheckToBoostIndexTemp();                                        //Steven 20181222 : Add 當升溫到一半,發現LB溫度不足,要重新補溫度
            if(ret==4)
            {
                Task=10;
                iTriggerBoostFunction=-1;
                iTriggerBoostFuncBack=-1;
                break;
            }

            if(HTestDeley.Off())
            {
                iTriggerBoostFuncBack=iTriggerBoostFunction;
                BoostCoolTime.SetSecAndOn(Temperature.dPostBoostDuration[iTriggerBoostFuncBack]);
                iBoostFuncStep=10;
                iTriggerBoostFunction=-1;
                if(fContact->fShow)
                    fContact->pnlHandlerSatus->Visible=false;
                HTestDeley.SetSecAndOn(0.01);
                BoostCoolCountDown.LatchCycleTime(true);
                dBoostCoolSec=0.0;
                BoostCoolStepTimer.SetSecAndOn(1);
                iBoostCountDown=Temperature.dPostBoostDuration[iTriggerBoostFuncBack];
                Task=50;
            }
            else
            {
                iInitialCount=Temperature.dBoostDuration[iTriggerBoostFunction]-TSDDelay.LatchCycleTime()/1000;
                if(fContact->fShow)
                    fContact->lblCountDown->Caption=iInitialCount;

                iBoostFuncStep=1;
            }
            break;
    }
    return false;
}
//******************************************************************************
//
//  注意!! GetTesterResult為Handler收發BIN相關, 修改時要小心!!
//
//******************************************************************************
//******************************************************************************
//
//  注意!! Check_TTL_Status為TTL收發BIN相關, 修改時要小心!!
//
//******************************************************************************
bool Check_TTL_Status(int Type)
{
    GetTesterResult(Type);
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC &&
               TestSocket.Item[i][j]!=HAS_NULL_IC &&
               TestSocket.Item[i][j]<TEST_PASS)
            {
                if(iTesterBIN[i][j]<0 ||
                   iTesterBIN[i][j]>iTestBinCount)                              //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    return false;
            }
        }
    }
    return true;
}
//==============================================================================
bool CheckTestSocketIsError(int i, int j)
{
    if(iTesterBIN[i][j]>=iTestBinCount ||                                       //jou 980818 add test result no set,result set to Interface Error
       iTesterBIN[i][j]<0 ||                                                    //Steven 20120821 : 記憶體溢位
       Prod.iT6CatData[iTesterBIN[i][j]]<0 ||                                   //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
       Prod.iT6CatData[iTesterBIN[i][j]]>=eTrayCount ||                         //JerryYang 20221215 : Magazine最大到14
       Prod.iTrayType[Prod.iT6CatData[iTesterBIN[i][j]]]==tNotUse)
    {
        if(LastSet.iTester==_2D_SORT &&
           CosFunction.bSortingBy2DList &&
           TestIF_File.bSortingBy2DIDList &&                                    //Frank 20221122 : 2DID sorting for ATK
           TestIF_File.iActionOf2DNotInList==1)                                 //Steven 20250806 : Action Of 2D Not In List
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}
//==============================================================================
//******************************************************************************
//
//  注意!! ProcessTestResult為Handler收發BIN相關, 修改時要小心!!
//
//******************************************************************************
//==============================================================================
//  Ver : 2003_07
//
// return  0: not ready       1: OK        2: interface error
//==============================================================================
TQPF_Timer ProcessTestResultDelay;
int ProcessTestResult(int Index)
{
    static int iSiteMappingErrorCT=0;                                           //Ifor 20180417 : add ASE_M要求錯誤三次才Alarm
    static int iOldErrorSite=0;                                                 //Ifor 20180417 : add 避免未清除錯誤次數

    int ret;
    bool bCanSetBin=false;
    bool SomeDataEnter=false;
    bool bAddSitemapStep=false;
    bool bHasTestIC=false;
    bool bSiteMappingHasError=false;                                            //Ifor 20180417 : add 目前Site Maping位置
    bool bInterfaceError=false, flag=true;
    bool bInterfacePos0[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                         {false, false, false, false, false, false, false, false},
                                                         {false, false, false, false, false, false, false, false},
                                                         {false, false, false, false, false, false, false, false}};

    AnsiString Str;
    bool bDone=false;

    RecordPiggyBackStartEnd(false);

    if(bNeedReplunge_RFMD==true)
        return 1;

    if(bDone==true)
        GetTesterResult(Index);
    else
        bDone=GetTesterResult(Index);

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC &&
               TestSocket.Item[i][j]<TEST_PASS &&
               TestSocket.Item[i][j]!=HAS_NULL_IC)
            {
                if(iTesterBIN[i][j]!=-1)
                {
                    SomeDataEnter=true;
                    if(iRecordOldBin[i][j]==-1)
                        iRecordOldBin[i][j]=iTesterBIN[i][j];
                }
            }
        }
    }

    if(TestIF.iTestType==TTL_MODE)
    {
        switch(iGetTestDataDelayTask)
        {
            case 1:
                if(SomeDataEnter==true)
                {
                    ProcessTestResultDelay.SetMSAndOn(1);
                    iGetTestDataDelayTask=100;
                }
                else
                {
                    break;
                }
            case 100:
                if(ProcessTestResultDelay.Off())
                {
                    bCanSetBin=true;
                }
                break;
        }
    }
    else
    {
        if(fContact->fShow && iTestTask>=30000)
        {
            bCanSetBin=false;
        }
        else
        {
            bCanSetBin=true;
        }
    }

    if(bDone==true)
        GetTesterResult(Index);
    else
        bDone=GetTesterResult(Index);
    bInterfaceError=false;
    if(bCanSetBin==true)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {                                                                   // ====================================== 2010/05/15 lee/joye
                if(TestSocket.Item[i][j]!=NULL_IC &&
                   TestSocket.Item[i][j]<TEST_PASS &&
                   TestSocket.Item[i][j]!=HAS_NULL_IC)
                {
                    if(iTesterBIN[i][j]!=-1)
                    {
                        if(iRecordOldBin[i][j]!=iTesterBIN[i][j])               // 2010/05/15 lee/joye
                        {
                            iRecordOldBin[i][j]=-1;
                            iGetTestDataDelayTask=1;
                            return 0;
                        }

                        if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                   //jou 2010-11-11 改成只有福雷會出現GPIB format error
                           CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                 //Steven 20131101 : Add ASE-K12
                        {
                            if(iTesterBIN[i][j]>=iTestBinCount)                 //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                                bBin16HangUp=true;
                        }

                        Str.sprintf("%s", TestSocket.cDeviceInf[i][j]);
                        if(TestIF_File.bEnableBarCode &&
                           TestIF_File.iNoCodeDeviceToErr==1 &&                 //Steven 20151221 : 將讀取異常的IC放到Error Bin
                          (Str==asBarCodeErrorSend ||                           //wei 20160318 Barcode Error依客戶設定
                           Str=="" ||
                           Str==asBarCodeErrorCheckSum))                        //KaiChen 20191121 ：中壢日月光 2D Check Sum
                        {
                            TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                            TestSocket.iBinData[i][j]=iTestBinCount;
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTestBinCount, TestSocket.cSBin[i][j], "No2DCodeDevice"); //Frank 20160505 add
                        }
                        else if(TestIF_File.iCloseSiteOnHPDontTest==2 &&        //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
                                TestSocket.iBinData[i][j]==iTestBinCount &&     //Steven 20250604 : 關site的位置有IC不測試送指定 bin
//                                LastSet.bUseTestSocket[Type][i][j]==false)
                                TestSocket.PordRec[i][j].asBuffer->Strings[eErrorCode].AnsiPos("NonTestToSettedBin")!=0)
                        {
                            TestSocket.SetItemData(i, j, TEST_PASS+TestIF_File.iCloseSiteBin);
                            TestSocket.iBinData[i][j]=TestIF_File.iCloseSiteBin;
                            TestSocket.PordRec[i][j].AddTestResultRecord(TestIF_File.iCloseSiteBin, TestSocket.cSBin[i][j], "NonTestToSettedBin");
                        }
                        else if(myIAR_Test.GetSuckTempErr(i, j))                //Steven 20250102 : fixed for [I54]
                        {
                            TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                            TestSocket.iBinData[i][j]=iTestBinCount;
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTestBinCount, TestSocket.cSBin[i][j], "Temperature Error"); //Frank 20160505 add
                        }
                        else
                        {
                            TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                            TestSocket.iBinData[i][j]=iTesterBIN[i][j];
                            TestSocket.PordRec[i][j].AddTestResultRecord(TestSocket.iBinData[i][j], TestSocket.cSBin[i][j]); //Frank 20160505 add
                            if(CosFunction.IntervalYieldCount)                  //wei 20180606 Interval Low Yield By Site
                            {
                                if(Prod.bFailAlarmIntervalLowYieldBySite)
                                {
                                    if(CheckTestSocketIsError(i, j)==true)
                                    {
                                        bIntervalYieldIsPass[Index][i][j][iYieldSiteCount[Index]]=false;
                                    }
                                    else
                                    {
                                        bIntervalYieldIsPass[Index][i][j][iYieldSiteCount[Index]]=Prod.bIsPassBin[Prod.iT6CatData[TestSocket.iBinData[i][j]]];
                                    }
                                }

                                if(Prod.bFailAlarmIntervalLowYieldByTotal)
                                {
                                    fShowBinSelect->IntervalByTotalCount->Caption=iYieldTotalCount;

                                    if(CheckTestSocketIsError(i, j)==true)
                                    {
                                        bYieldTotalBinIsPass[iYieldTotalCount%Prod.iIntervalLowYieldCountByTotal]=false;
                                    }
                                    else
                                    {
                                        bYieldTotalBinIsPass[iYieldTotalCount%Prod.iIntervalLowYieldCountByTotal]=Prod.bIsPassBin[Prod.iT6CatData[TestSocket.iBinData[i][j]]];
                                    }

                                    iYieldTotalCount++;
                                    if((iYieldTotalCount>=Prod.iIntervalLowYieldCountByTotal) &&
                                       (iYieldTotalCount%Prod.iIntervalLowYieldCountByTotal==0))
                                    {
                                        bYieldTotalBin=true;
                                    }
                                }
                                bYieldSiteBinCheck=true;
                            }
                        }

                        if(fBarCode->bGPIBTestBarCodeError)
                        {
                            TestSocket.cReDeviceInf[i][j]="";
                            TestSocket.cReDeviceInf[i][j]=fBarCode->iBinReturnMess[i*2+j];
                        }

                        if(CheckTestSocketIsError(i, j)==true)
                        {
                            if((TestIF_File.bEnableBarCode && TestIF_File.iNoCodeDeviceToErr==1) ||    //Steven 20151221 : 將讀取異常的IC放到Error Bin
                               (TestIF_File.bEnableBarCode && TestIF_File.bSearch2DIDByLot && LastSet.iTester==OFF_LINE))    //Frank 20170316 (wei) add Search 2DID By Lot
                            {
                                Str.sprintf("%s", TestSocket.cDeviceInf[i][j]);
                                if(Str==asBarCodeErrorSend ||                   //wei 20160318 Barcode Error依客戶設定
                                   Str=="" ||
                                   Str==asBarCodeErrorCheckSum)                 //KaiChen 20191121 ：中壢日月光 2D Check Sum
                                {

                                }
                                else
                                {
                                    if(TestIF_File.bEnableBarCode &&
                                       TestIF_File.b2DIDNotExist2Error &&
                                       LastSet.iTester==OFF_LINE)               //JerryYang 20231218 : 2DID黑名單功能
                                    {
                                        int k=fBarCode->b2DIDIsInsideToErrorBin(i, j);
                                        if(k==1)                                //有找到
                                        {

                                        }
                                        else
                                        {
                                            if(bBin16HangUp==false)
                                                bInterfaceError=true;
                                        }
                                    }
                                    else
                                    {
                                        if(bBin16HangUp==false)
                                            bInterfaceError=true;
                                    }
                                }
                            }
                            else if(TestIF_File.bEnableBarCode &&
                                    TestIF_File.b2DIDNotExist2Error &&
                                    LastSet.iTester==OFF_LINE)                  //JerryYang 20231218 : 2DID黑名單功能
                            {
                                int k=fBarCode->b2DIDIsInsideToErrorBin(i, j);
                                if(k==1)                                        //有找到
                                {

                                }
                                else
                                {
                                    if(bBin16HangUp==false)
                                        bInterfaceError=true;
                                }
                            }
                            else
                            {
                                if(bBin16HangUp==false)
                                    bInterfaceError=true;
                            }

                            if((iHWFix_BinBox==1 || bCancelErrorBin))           //kevin 20160819 error bin 要放到 Bin Box
                            {
                                if(Index==0)
                                    sArm1BinError=asRecordTestResult;           //kevin 20160725 記錄 Error Bin
                                else
                                    sArm2BinError=asRecordTestResult;           //kevin 20160725 記錄 Error Bin
                                bBinError[Index]=true;                          //kevin 20160725 Error Bin 發生arm
                                bInterfaceError=false;
                            }

                            bInterfacePos0[i][j]=true;
                            TestSocket.bPass[i][j]=false;
                            TestSocket.bNeedReTest[i][j]=false;
                            LastSet.iTestIgnore++;                              //RogerYang 20250923 : 瑞薩FT-CT
                        }
                        else
                        {
                            if(Prod.bIsPassBin[iTesterBIN[i][j]])
                            {
                                TestSocket.bPass[i][j]=true;
                                if(IniConfig.bD22_4_PassBinCanDoubleContact)    //JerryYang 20230909 : pass bin也可以設定Double contact
                                {
                                    if(Prod.DBContact[iTesterBIN[i][j]]!=0)
                                        TestSocket.bNeedReTest[i][j]=true;
                                    else
                                        TestSocket.bNeedReTest[i][j]=false;
                                }
                                else
                                {
                                    TestSocket.bNeedReTest[i][j]=false;
                                }
                            }
                            else
                            {
                                TestSocket.bPass[i][j]=false;
                                if(Prod.DBContact[iTesterBIN[i][j]]!=0)
                                    TestSocket.bNeedReTest[i][j]=true;
                                else
                                    TestSocket.bNeedReTest[i][j]=false;
                            }
                            LastSet.iTesterMatch++;                             //RogerYang 20250923 : 瑞薩FT-CT TesterMatch表示正常測試結束
                        }
                    }
                    else
                    {
                        flag=false;
                    }
                }
                else if(TestIF_File.iCloseSiteOnHPDontTest==2 &&                //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
                        iTestTask==60 &&                                        //Steven 20251022 : hang up
                        bDone &&
                        TestSocket.Item[i][j]!=NULL_IC &&
                        TestSocket.Item[i][j]==(TEST_PASS+iTestBinCount) &&
                        TestSocket.iBinData[i][j]==iTestBinCount &&             //Steven 20251022 : 關site的位置有IC不測試送指定 bin
                        TestSocket.PordRec[i][j].asBuffer->Strings[eErrorCode].AnsiPos("NonTestToSettedBin")!=0)
                {
                    TestSocket.SetItemData(i, j, TEST_PASS+TestIF_File.iCloseSiteBin);
                    TestSocket.iBinData[i][j]=TestIF_File.iCloseSiteBin;
                    TestSocket.PordRec[i][j].AddTestResultRecord(TestIF_File.iCloseSiteBin, TestSocket.cSBin[i][j], "NonTestToSettedBin");
                }
            }
        }

        if(CosFunction.bCheckTempDuringIndexArmTesting==true &&
           IniConfig.bI54_Enable==true &&
           IniConfig.bI54_1_AllICErr==true &&
           myIAR_Test.GetArmTempErr())                                          //Steven 20250102 : fixed for [I54]
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(TestSocket.Item[i][j]!=NULL_IC &&
                       TestSocket.Item[i][j]!=HAS_NULL_IC)
                    {
                        TestSocket.SetItemData(i, j, TEST_PASS+iTestBinCount);
                        TestSocket.iBinData[i][j]=iTestBinCount;
                        TestSocket.PordRec[i][j].AddTestResultRecord(iTestBinCount, TestSocket.cSBin[i][j], "Temperature Error"); //Frank 20160505 add
                    }
                }
            }
        }
    }
    else
    {
        flag=false;                                                             // 2010/05/15 lee/joye
    }

    if(bBin16HangUp)                                                            //Steven 20100412
    {
        DoCheckHasTestTempChange();                                             //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
        if(TestIF.iTestType==GPIB_MODE || TestIF.iTestType==TCP_IP_MODE)        //wei 20211027 open short TCP/IP
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung || CosFunction.bIndexAreaOnlyCanUseSkip==true)  //kevin 20141008 測試問題不能重測 //Steven 20141105 : Index內的所有異常都只能用Skip
                ret=ShowErrorMessage("WAR07320", K_SKIP, MMInterface, false, asRecordTestResult);
            else
                ret=ShowErrorMessage("WAR07320", K_RESET|K_SKIP, MMInterface, false, asRecordTestResult);
        }
        else if(TestIF.iTestType==TTL_MODE)
        {
            ret=ShowErrorMessage("WAR07319", K_RESET|K_SKIP, MMInterface, false, asRecordTestResult);
        }
        else
        {
            ret=ShowErrorMessage("WAR07318", K_RESET|K_SKIP, MMInterface, false, asRecordTestResult);
        }

        if(ret==K_SKIP)                                                         //kevin 20161228 (jou) add data format err bin
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(TestSocket.Item[i][j]!=HAS_NULL_IC &&                    //kevin 20120618  沒有IC 就不要設定避免畫面被誤解
                       TestSocket.Item[i][j]!=NULL_IC     &&                    //Steven 20121115 : 沒IC的地方,設定數字進去會Hang Up!
                       CheckTestSocketIsError(i, j)==true)
                    {
                        if(IniConfig.bI26TestCloseSiteHaveBin)                  //kevin 20150202
                        {
                            if(Index==0)
                                bTestBinDataError=1;                            //kevin 20150202 測試bin 別沒設定或關site 有bin 資料
                            else
                                bTestBinDataError=2;                            //kevin 20150202 測試bin 別沒設定或關site 有bin 資料
                        }
                        else
                        {
                            iTesterBIN[i][j]=999;
                            TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                            TestSocket.iBinData[i][j]=iTestBinCount;            //Steven 20190116 : 修正顯示錯誤
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTesterBIN[i][j], TestSocket.cSBin[i][j], "TestBinDataError"); //Frank 20160505 add
                        }
                    }
                }
            }
            bBin16HangUp=false;
            if(iHWFix_BinBox==1 || bCancelErrorBin)                             //kevin 20161228 (jou) error bin 要放到 Bin Box
            {
                RecordProcess("Format error Use iHWFix_BinBox");
            }
            else
            {
                if(ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true)                         //JerryYang 20220815 : send ATC start testing
                {
                    ATC_InterfaceForm->TestFinish();
                    ATC_InterfaceForm->HandlerArm(-1);                      //Ifor 20220107 add:通知ATC目前哪隻Arm再Socket
                }
                return 1;
            }
        }
    }

    if(bInterfaceError)
    {
        DoCheckHasTestTempChange();                                             //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
        SetNoiseDelay=false;
        fMain->CloseGpibProgram(__FUNC__);
        if(IniConfig.bA03UseAfterHomeCarryAndSuckIcToRBin && _bHomeNeedOnecycle)
        {
            ret=K_SKIP;
        }
        else
        {
            if(TestIF.iTestType==GPIB_MODE ||
               TestIF.iTestType==TCP_IP_MODE)                                   //wei 20211027 open short TCP/IP
            {
                if(CUSTOMER_CODE==CC_ASE_CL)                                    //jou 2010-11-25 start : 中壢ASE要Bin 0自動skip，不alarm
                {
                    int k=0;
                    int l=0;
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(TestSocket.Item[i][j]!=NULL_IC &&
                               TestSocket.Item[i][j]!=HAS_NULL_IC)
                            {
                                k++;
                                if(iTesterBIN[i][j]>=iTestBinCount)             //Steven 20230929 : 15 --> iTestBinCount
                                {
                                    l++;
                                }
                            }
                        }
                    }

                    if(k==l)
                    {
                        if(CosFunction.bTestTimeOutShowSkipAndHome && IniConfig.iI22TestTimeOutOption==3)
                        {
                            ret=ShowErrorMessage("WAR07356", K_HOME, MMInterface, false, asRecordTestResult);
                        }
                        else if(CosFunction.bIndexAreaOnlyCanUseSkip)           //Steven 20141105 : Index內的所有異常都只能用Skip
                        {
                            ret=ShowErrorMessage("WAR07356", K_SKIP, MMInterface, false, asRecordTestResult);
                        }
                        else
                        {
                            ret=ShowErrorMessage("WAR07356", K_RETRY|K_SKIP|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);
                        }
                    }
                    else
                    {
                        MyDBIProcess("Message", asRecordTestResult);
                        ret=K_SKIP;
                    }
                }
                else if(CosFunction.bIndexAreaOnlyCanUseSkip ||
                        CUSTOMER_CODE==CC_AMKOR_Korea ||                        //Steven 20141105 : Index內的所有異常都只能用Skip
                        CUSTOMER_CODE==CC_SCC)                                  //Steven 20220309 : Add JSCC
                {
                    if(CosFunction.bTestTimeOutShowSkipAndHome && IniConfig.iI22TestTimeOutOption==3)
                    {
                        ret=ShowErrorMessage("WAR07356", K_HOME, MMInterface, false, asRecordTestResult);
                    }
                    else
                    {
                        ret=ShowErrorMessage("WAR07356", K_SKIP, MMInterface, false, asRecordTestResult);
                    }
                }
                else if(CUSTOMER_CODE==CC_ASE_KaohSiung)                        //kevin 20141008 測試問題不能重測
                {
                    if(IniConfig.bI26TestCloseSiteHaveBin)
                    {
                        ret=ShowErrorMessage("WAR07359",K_SKIP|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);
                    }
                    else
                    {
                        ret=ShowErrorMessage("WAR07356",K_SKIP|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);
                    }
                }
                else if(CUSTOMER_CODE==CC_KYEC_LEE)                             //Ifor 20180920 (Steven) : Add KYEC 要求TACS and ATN Check Time Out 直接分Err Bin
                {
                    if(asRecordTestResult=="Tester time up error")
                    {
                        asRecordTestResult="";
                        ret=ShowErrorMessage("WAR07352", K_SKIP, MMInterface, false);
                    }
                    else
                    {
                        ret=ShowErrorMessage("WAR07356", K_SKIP|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);//Ifor 20200106 add: Category setting error取消RETRY選項
                    }
                }
                else
                {
                    if(IniConfig.iI22TestTimeOutOption==0)
                    {
                        ret=ShowErrorMessage("WAR07356", K_SKIP|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);
                    }
                    else if(IniConfig.iI22TestTimeOutOption==1)
                    {
                        ret=ShowErrorMessage("WAR07356", K_RETRY|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);
                    }
                    else if(IniConfig.iI22TestTimeOutOption==2)
                    {
                        ret=ShowErrorMessage("WAR07356", K_SKIP|K_RETRY|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);
                    }
                    else
                    {
                        ret=ShowErrorMessage("WAR07356", K_HOME|K_CLEAN_OUT, MMInterface, false, asRecordTestResult);
                    }
                }
            }
            else
            {
                if(CosFunction.bIndexAreaOnlyCanUseSkip ||                      //Steven 20141105 : Index內的所有異常都只能用Skip
                   CUSTOMER_CODE==CC_AMKOR_Korea ||
                   CUSTOMER_CODE==CC_SCC)                                       //Steven 20220309 : Add JSCC
                {
                    ret=ShowErrorMessage("WAR07356", K_SKIP, MMInterface, false, " ");
                }
                else
                {
                    if(IniConfig.iI22TestTimeOutOption==0)
                    {
                        ret=ShowErrorMessage("WAR07356", K_SKIP|K_CLEAN_OUT, MMInterface, false, " ");
                    }
                    else if(IniConfig.iI22TestTimeOutOption==1)
                    {
                        ret=ShowErrorMessage("WAR07356", K_RETRY|K_CLEAN_OUT, MMInterface, false, " ");
                    }
                    else if(IniConfig.iI22TestTimeOutOption==2)
                    {
                        ret=ShowErrorMessage("WAR07356", K_SKIP|K_RETRY|K_CLEAN_OUT, MMInterface, false, " ");
                    }
                    else
                    {
                        ret=ShowErrorMessage("WAR07356", K_HOME|K_CLEAN_OUT, MMInterface, false, " ");
                    }
                }
            }
        }

        if(ret==K_RETRY)
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(bInterfacePos0[i][j]==true && TestSocket.Item[i][j]!=NULL_IC)
                    {
                        TestSocket.Item[i][j]=HAS_IC;
                    }
                }
            }
            InitTestTask();
            if(ATC_SYSTEM==eNewATCSystem &&
               Temperature.bATCActiveCooling==true)                             //JerryYang 20220815 : send ATC start testing
            {
                ATC_InterfaceForm->TestFinish();
                ATC_InterfaceForm->HandlerArm(-1);                          //Ifor 20220107 add:通知ATC目前哪隻Arm再Socket
                TriggerATC_FFC_Function();                                      //Ifor 20240507 add :FFC Trigger Even Off
            }
            return 2;
        }

        if(ret==K_SKIP || ret==K_HOME)
        {
            if(ret==K_HOME)
            {
                fAllMotorHome=false;
                bI22_NeedHomeDelay=true;
            }
            bAutoSiteMapWaitTestResult=false;                                   //JerryYang 20190419 fix Auto site mapping發生category error後按SKIP會hang up
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(TestSocket.Item[i][j]!=HAS_NULL_IC &&                    //kevin 20120618  沒有IC 就不要設定避免畫面被誤解
                       TestSocket.Item[i][j]!=NULL_IC     &&                    //Steven 20121115 : 沒IC的地方,設定數字進去會Hang Up!
                       CheckTestSocketIsError(i, j)==true)
                    {
                        if(IniConfig.bI26TestCloseSiteHaveBin)                  //kevin 20150202
                        {
                            if(Index==0)
                                bTestBinDataError=1;                            //kevin 20150202 測試bin 別沒設定或關site 有bin 資料
                            else
                                bTestBinDataError=2;                            //kevin 20150202 測試bin 別沒設定或關site 有bin 資料
                        }
                        else
                        {
                            iTesterBIN[i][j]=999;
                            TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                            TestSocket.iBinData[i][j]=iTestBinCount;            //Steven 20190116 : 修正顯示錯誤
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTesterBIN[i][j], TestSocket.cSBin[i][j], "TestBinDataError"); //Frank 20160505 add
                        }
                    }
                }
            }

            if(ATC_SYSTEM==eNewATCSystem && Temperature.bATCActiveCooling==true)//JerryYang 20220815 : send ATC start testing
            {
                ATC_InterfaceForm->TestFinish();
                ATC_InterfaceForm->HandlerArm(-1);                          //Ifor 20220107 add:通知ATC目前哪隻Arm再Socket
                TriggerATC_FFC_Function();                                      //Ifor 20240507 add :FFC Trigger Even Off
            }
            return 1;
        }
    }

    if(flag==true)
    {
        bAddSitemapStep=false;                                                  //Ifor 20181112 : add
        bHasTestIC=false;
        if(bChangeTest_TempOffset!=0 && bDoATCTempRise==false)                  //Ifor 20230504 add: ATC 回溫功能需等待回溫結束才可切回生產溫度
        {
            DoCheckHasTestTempChange();                                         //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
        }
        else
        {
            bChangeTest_TempAlarm=false;                                        //Ifor 20210623 add: Test Temp Change
        }

        if(IniConfig.bUseAutoSiteMapping &&                                     //jou 2011-03-24 start : Auto Site Mapping
           iContactMode==CONTACT_NORMAL)                                        //JerryYang 20170417 (Steven) contact test時不需要做auto site map check
        {
            if(LastSet.iRunStartMode==rsmAutoSiteMap &&
               bSiteMappingCHKOK==false)
            {
                if(IniConfig.bVTESTFunction==true &&
                   CosFunction.bAutoSiteMappingSetPassBIN==true)                //jou 20230221 : Auto Site Mapping Set Pass BIN
                {
                    if(iAutoSiteMapRunStartMode==1)                             //RT    //jou 20230224 : 修正auto site mapping hang up
                        bAutoSiteMapWaitTestPass=true;
                    else                                                        //FT
                        bAutoSiteMapWaitTestPass=false;
                }

                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestSocket.Item[i][j]!=HAS_NULL_IC  &&
                           TestSocket.Item[i][j]!=NULL_IC      &&               //Steven 20140918 : Fix for Auto Site Mapping
                           TestSocket.Item[i][j]-1000!=NULL_IC)
                        {
                            bHasTestIC=true;
                            if(iTesterBIN[i][j]>=0 &&
                               iTesterBIN[i][j]<=iTestBinCount)                 //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                            {
                                if(IniConfig.bVTESTFunction==true &&            //jou 2016-10-28 JCET 要求Site Mapping 必須測試到pass bin才能通過
                                   CosFunction.bAutoSiteMappingSetPassBIN==true)//jou 20230221 : Auto Site Mapping Set Pass BIN
                                {
                                    if(iAutoSiteMapRunStartMode==1)             //RT
                                    {
                                        if(iTesterBIN[i][j]==Prod.iOpenBin)
                                            bAutoSiteMapWaitTestPass=false;
                                    }
                                    else                                        //FT
                                    {
                                        if(fMesSystem->AutoSiteMapPass(iTesterBIN[i][j]))
                                            bAutoSiteMapWaitTestPass=true;
                                    }
                                }
                                else if(CosFunction.bUSEJCETSiteMapMode==true)
                                {
                                    iGetTestData[i][j]=iTesterBIN[i][j];
                                    if(IniConfig.bI21AutoSiteMappingFailBinSetting==true)
                                    {                                           //Ifor 20171128 (wei) : add Auto Site Mapping Fail Bin Setting
                                        if(CosFunction.bAutoSiteMappingSetOpenBIN)
                                        {                                       //Steven 20230213 : [I21-9]的OS Bin跟著工作檔
                                            if(iTesterBIN[i][j]==Prod.iOpenBin)
                                                bAutoSiteMapWaitTestPass=false;
                                            else
                                                bAutoSiteMapWaitTestPass=true;
                                        }
                                        else if(iTesterBIN[i][j]==IniConfig.iI21UseFailBinSetting)
                                        {
                                            bAutoSiteMapWaitTestPass=false;
                                        }
                                        else
                                        {
                                            bAutoSiteMapWaitTestPass=true;
                                        }
                                    }
                                    else
                                    {
                                        if(Prod.bIsPassBin[iTesterBIN[i][j]]==false)  //RogerYang 20250220 : fixed for auto site map
                                        {
                                            bAutoSiteMapWaitTestPass=false;
                                        }
                                        else
                                        {
                                            bAutoSiteMapWaitTestPass=true;
                                        }
                                    }

                                    if(CUSTOMER_CODE==CC_LEADYO &&
                                       bAutoSiteMapWaitTestPass==false)         //KenHsieh 20251003 : LEADYO 需要設定fail次數報Alarm
                                    {
                                        if(IniConfig.iI21FailRetryCount!=0)
                                        {
                                            if(iOldErrorSite!=iDoSiteMappingStep)
                                            {
                                                iOldErrorSite=iDoSiteMappingStep;
                                                iSiteMappingErrorCT=0;
                                            }
                                            iSiteMappingErrorCT++;
                                        }
                                        else
                                        {
                                            iSiteMappingErrorCT=0;
                                        }
                                    }
                                    else
                                    {
                                        iSiteMappingErrorCT=0;
                                    }

                                    if(bAutoSiteMapWaitTestPass==false)
                                    {
                                        bASMFinishOneCycle=false;
                                        bSiteMappingCHKOK=false;

                                        if(CUSTOMER_CODE!=CC_LEADYO       ||
                                          IniConfig.iI21FailRetryCount==0 ||
                                          (IniConfig.iI21FailRetryCount!=0 &&
                                           iSiteMappingErrorCT>IniConfig.iI21FailRetryCount))   //KenHsieh 20251003 : LEADYO 需要設定fail次數報Alarm
                                        {
                                            iSiteMappingErrorCT=0;
                                            #ifndef DEBUG_AutoSiteMap
                                            ShowMyMessage("Site Mapping Check Fail! Must Pass Bin,need Do again!",
                                                          "Site Mapping 確認失敗!必須 Pass Bin,需要再做一次!",
                                                          IndexSuckName[i][j]);     //Steven 20220526 : 紀錄Auto Site Map Fail的位置
                                            #endif
                                        }
                                    }

                                    if(CUSTOMER_CODE==CC_ASE_M)                 //Ztex 2025.04.30 Add ASE_M USEJCETSiteMapMode
                                        bAutoSiteMapAmbientResultCheck=true;
                                    DoJCETSiteMappingCHK(bAutoSiteMapWaitTestPass);

                                    if(IniConfig.bI19AuToSitMapPauseWaitBin)
                                        fMain->Pause("bI19AuToSitMapPauseWaitBin");
                                }
                                else if(CUSTOMER_CODE==CC_ASE_M && bGetOpenBin==false)
                                {                                               //Ifor 20180417 : add ASE_M 要求Auto Site Mapping 連續三次錯誤才Alarm(僅判斷未完成Site Mapping)
                                    if(i==0 && j==iDoSiteMappingStep)
                                    {
                                        bSiteMappingHasError=true;
                                    }
                                    else if(i==1 && j==(iDoSiteMappingStep-TestSocket.iShtCol))
                                    {
                                        bSiteMappingHasError=true;
                                    }
                                    else
                                    {
                                        bSiteMappingHasError=false;
                                    }

                                    iGetTestData[i][j]=iTesterBIN[i][j];
                                    if(iTesterBIN[i][j]!=1 && bSiteMappingHasError==true)
                                    {
                                        if(iOldErrorSite!=iDoSiteMappingStep)
                                        {
                                            iOldErrorSite=iDoSiteMappingStep;
                                            iSiteMappingErrorCT=0;
                                        }
                                        bAutoSiteMapWaitTestPass=false;
                                        bASMFinishOneCycle=false;
                                        bSiteMappingCHKOK=false;

                                        iSiteMappingErrorCT++;
                                    }
                                    else
                                    {
                                        bAutoSiteMapWaitTestPass=true;
                                    }
                                    if(iTesterBIN[i][j]==1 && bSiteMappingHasError==true)
                                    {
                                        bAutoSiteMapAmbientResultCheck=true;    //Ifor 20190528 : add Site Mapping Ambient Check
                                    }
                                    else
                                    {
                                        bAutoSiteMapAmbientResultCheck=false;   //Ifor 20190528 : add Site Mapping Ambient Check
                                    }

                                    if(iSiteMappingErrorCT==0)                  //Ifor 20181112 避免 iDoSiteMappingStep 一直++
                                    {
                                        bAddSitemapStep=true;
                                    }
                                    else
                                    {
                                        bAddSitemapStep=false;
                                        if(iSiteMappingErrorCT>=3)              //Ifor 20180417 : add ASE_M要求錯誤三次才Alarm
                                        {
                                            iSiteMappingErrorCT=0;
                                            #ifndef DEBUG_AutoSiteMap
                                            ShowMyMessage("Site Mapping Check Fail! Must Pass Bin,need Do again!",
                                                          "Site Mapping 確認失敗!必須 Pass Bin,需要再做一次!",
                                                          IndexSuckName[i][j]); //Steven 20220526 : 紀錄Auto Site Map Fail的位置
                                            #endif
                                        }
                                    }

                                    if(IniConfig.bI19AuToSitMapPauseWaitBin)
                                        fMain->Pause("bI19AuToSitMapPauseWaitBin");                         //kevin 20150119 buff
                                }
                                else
                                {
                                    if(bGetOpenBin==true)                       //checkautoSitemap
                                    {
                                        if(IniConfig.bI21ASMNeedCheckEachSiteOpen &&    //Steven 20120726 : AutoSiteMapping, 當確認Open Bin時,同時也要檢查是不是所有Dut都Open
                                           CUSTOMER_CODE!=CC_SCS)               //jou 20220926 : 修正JSCS auto site mapping hang up
                                        {
                                            if(iOpenBin==-1)
                                            {
                                                if(LastSet.iTester==OFF_LINE)   //Steven 20150713 : 整理LastSet.iTester
                                                    iOpenBin=2;                 //Steven 20110422 : 離線模式強制為2
                                                else
                                                    iOpenBin=iTesterBIN[i][j];
                                            }
                                            else
                                            {
                                                if(LastSet.iTester==ON_LINE && iOpenBin!=iTesterBIN[i][j])
                                                {
                                                    fMain->ReStartAutoSiteMapping(true);
                                                    fMain->AutoSiteMappignCleanOut(true);
                                                    ShowMyMessage("Site mapping get open bin fail! Need check load board!",
                                                                  "Site Mapping 取得Open Bin失敗, 需要檢查Load board!");

                                                    if(ATC_SYSTEM==eNewATCSystem && Temperature.bATCActiveCooling==true)  //JerryYang 20220815 : send ATC start testing
                                                    {
                                                        ATC_InterfaceForm->TestFinish();
                                                        ATC_InterfaceForm->HandlerArm(-1);  //Ifor 20220107 add:通知ATC目前哪隻Arm再Socket
                                                    }
                                                    TriggerATC_FFC_Function();  //Ifor 20240507 add :FFC Trigger Even Off
                                                    return 1;
                                                }
                                            }
                                        }

                                        if(LastSet.iTester==OFF_LINE)           //Steven 20150713 : 整理LastSet.iTester
                                            Prod.iOpenBin=2;                    //Steven 20110422 : 離線模式強制為2
                                        else
                                            Prod.iOpenBin=iTesterBIN[i][j];
                                        fMain->SetOpenBin();

                                        if(IniConfig.bI19AuToSitMapPauseWaitBin)
                                            fMain->Pause("bI19AuToSitMapPauseWaitBin");                     //kevin 20150119 buff
                                    }
                                    else
                                    {
                                        iGetTestData[i][j]=iTesterBIN[i][j]+1;

                                        if(iTesterBIN[i][j]==Prod.iOpenBin)
                                        {
                                            #ifndef ASE_KaohSiung               //不是高雄的話
                                                fMain->ReStartAutoSiteMapping(true);
                                                fMain->AutoSiteMappignCleanOut(true);
                                                ShowMyMessage("Site Mapping Check Fail! Have Open Bin,need Do again!",
                                                              "Site Mapping Check 失敗!有Open Bin,需要再做一次!",
                                                              IndexSuckName[i][j]);             //Steven 20220526 : 紀錄Auto Site Map Fail的位置
                                            #else                               //kevin 20150113
                                                ShowMyMessage("Site Mapping Check Fail! Have Open Bin,need Do again!",
                                                              "Site Mapping Check 失敗!有Open Bin,需要再做一次!",
                                                              IndexSuckName[i][j]);             //Steven 20220526 : 紀錄Auto Site Map Fail的位置
                                                bShowAutoSiteMappingError=true; //20150115
                                                bSiteMappingCHKOK=false;
                                            #endif
                                        }
                                        if(IniConfig.bI19AuToSitMapPauseWaitBin)
                                            fMain->Pause("bI19AuToSitMapPauseWaitBin");                     //kevin 20150119 buff
                                    }
                                }
                            }
                        }
                    }
                }

                if(CUSTOMER_CODE==CC_ASE_M && bHasTestIC==true)
                {
                    DoJCETSiteMappingCHK(bAddSitemapStep);                      //Ifor 20181106 往下移至迴圈外避免Step錯亂
                }
                else if(IniConfig.bVTESTFunction==true)
                {
                    if(bHasTestIC==true)
                    {
                        bAutoSiteMapWaitTestResult=false;

                        if(bAutoSiteMapWaitTestPass==false)
                        {
                            bASMFinishOneCycle=false;
                            bSiteMappingCHKOK=false;
                            bAddSitemapStep=false;
                            ShowMyMessage("Site Mapping Check Fail! Must Pass Bin,need Do again!",
                                          "Site Mapping 確認失敗!必須 Pass Bin,需要再做一次!");
                            iAutoSiteMappingErrCT++;
                            if(iAutoSiteMappingErrCT>=IniConfig.iI21AutoSiteMappingErrCT)
                            {
                                iAutoSiteMappingErrCT=0;
                                fMain->BtnOneCycleClick(fMain->BtnOneCycle);
                            }
                        }
                        else
                        {
                            iAutoSiteMappingErrCT=0;
                            bAddSitemapStep=true;
                        }
                        DoJCETSiteMappingCHK(bAddSitemapStep);                  //Ifor 20181106 往下移至迴圈外避免Step錯亂
                    }
                    else if(bHasTestIC==false && bAddSitemapStep==false && iAutoSiteMappingErrCT==0 && iAutoSiteMapRunStartMode==0)    //FT
                    {
                        bAutoSiteMapWaitTestPass=true;
                    }
                }
            }
        }
        else
        {
            if(bInterfaceError==false)                                          //Steven 20190326 : QA Sampling
            {
                ProcessQASampling(Index);
            }
        }

        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.GetTestResult);                              //26     Get Test Result
        if(ATC_SYSTEM==eNewATCSystem && Temperature.bATCActiveCooling==true)    //JerryYang 20220815 : send ATC start testing
        {
            ATC_InterfaceForm->TestFinish();
            ATC_InterfaceForm->HandlerArm(-1);                                  //Ifor 20220107 add:通知ATC目前哪隻Arm再Socket
            TriggerATC_FFC_Function();                                          //Ifor 20240507 add :FFC Trigger Even Off
        }

        if(IniConfig.bL22Enable3SigmaTempMonitor==true &&                       //kevin 20200521 add 3Sigma Copy
           Temperature.b3SigmaTempMonitior_Enable==true)                        //Hmy 20200510 Add Enable 3 Sigma Temp Monitor
        {
            #ifdef SOFT_SIMULTE
                fLotInfo->WhenTestRecordTemperatureLog_3Sigma("D:\\HT9045_Log\\Sigma",Index);
            #else
                if(iATCOnLine==1 && (ATC_SYSTEM==1 || ATC_SYSTEM==eNewATCSystem)) //kevin 20200618 mark
                    fLotInfo->WhenTestRecordTemperatureLog_3Sigma("D:\\HT9045_Log\\Sigma", Index);
            #endif
        }
        return 1;
    }

    if(TestSocket.All_HAS_NULL_IC())                                            //Steven 20180910 : Prevent hang up
    {
        DoCheckHasTestTempChange();                                             //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
        if(ATC_SYSTEM==eNewATCSystem && Temperature.bATCActiveCooling==true)    //JerryYang 20220815 : send ATC start testing
        {
            ATC_InterfaceForm->TestFinish();
            ATC_InterfaceForm->HandlerArm(-1);                                  //Ifor 20220107 add:通知ATC目前哪隻Arm再Socket
            TriggerATC_FFC_Function();                                          //Ifor 20240507 add :FFC Trigger Even Off
        }
        return 1;
    }
    return 0;
}
//******************************************************************************
//
//  注意!! ProcessTestResult為Handler收發BIN相關, 修改時要小心!!
//
//******************************************************************************
//******************************************************************************
//
//  注意!! ProcessTesterTimeOut為Handler收發BIN相關, 修改時要小心!!
//
//******************************************************************************
//==============================================================================
// return  0: not ready       1: OK        2: interface error
//==============================================================================
bool bTestDuplicateErr=false;                                                   //Steven 20120102 : 測試ok要重置Flag
int ProcessTesterTimeOut(int Index)
{
    int ret=0, i=0, j=0;
    AnsiString asTestTimeOut="";
    int iMot=MTestZ1;

    if(Index==1)                                                                //kevin 20161105 add
        iMot=MTestZ2;

    if(CUSTOMER_CODE==CC_KYEC_LEE ||
       CUSTOMER_CODE==CC_KYEC_XILINX ||
       CUSTOMER_CODE==CC_KYEC_CHEN ||
       CUSTOMER_CODE==CC_SIGURD_ChungXing ||                                    //wei 20150226 Time Out 只能SKIP
       CUSTOMER_CODE==CC_ASE_SG ||                                              //Ifor 20250926 add ASESG
       CosFunction.bTempAlarmBinNeedToError)                                    //Steven 20251022 : Temp alarm need put to error bin
    {
//        fMain->RunTestProgram(false);                                         //wei 20150226 Time Out 就停止測試
//        ret=ShowErrorMessage("WAR07352", K_SKIP, MMInterface, bTestDuplicateErr);
        if(bATCHasAlarmBinNeedToError==true)                                    //Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
        {
            bATCHasAlarmBinNeedToError=false;
            RecordProcess("During Test,ATC Alarm Set to Error Bin");
            ret=K_SKIP;
        }
        else
        {
            fMain->RunTestProgram(false);                                       //wei 20150226 Time Out 就停止測試
            ret=ShowErrorMessage("WAR07352", K_SKIP, MMInterface, bTestDuplicateErr);
        }
    }
    else if(CosFunction.bIndexAreaOnlyCanUseSkip)                               //Steven 20141105 : Index內的所有異常都只能用Skip
    {
        if(IniConfig.bI36TestTimeOut)                                           //kevin 20161105 沒有收到測試資料強制取出ic
            ret=ShowErrorMessage("WAR0716", K_SKIP, iMot, bTestDuplicateErr);
        else if(CosFunction.bTestTimeOutShowSkipAndHome && IniConfig.iI22TestTimeOutOption==3)
            ret=ShowErrorMessage("WAR07352", K_HOME, MMInterface, bTestDuplicateErr);
        else
            ret=ShowErrorMessage("WAR07352", K_SKIP, MMInterface, bTestDuplicateErr);
    }
    else
    {
        if(IniConfig.iI22TestTimeOutOption==0)
        {
            ret=ShowErrorMessage("WAR07352", K_SKIP, MMInterface, bTestDuplicateErr);
        }
        else if(IniConfig.iI22TestTimeOutOption==1)
        {
            ret=ShowErrorMessage("WAR07352", K_RETRY, MMInterface, bTestDuplicateErr);
        }
        else if(IniConfig.iI22TestTimeOutOption==2)
        {
            ret=ShowErrorMessage("WAR07352", K_SKIP|K_RETRY, MMInterface, bTestDuplicateErr);
        }
        else
        {
            ret=ShowErrorMessage("WAR07352", K_HOME, MMInterface, bTestDuplicateErr);
        }
    }

    bATCHasAlarmBinNeedToError=false;                                           //kevin 20181011  Ifor 20160726 add 發生 ATC 異常時需將測中IC放至Error Bin
    bTestDuplicateErr=true;
    if(ret==K_RETRY)
    {
        if((TestIF.iTestType==GPIB_MODE ||
            TestIF.iTestType==TCP_IP_MODE) &&                                   //wei 20211027 open short TCP/IP
           LastSet.iTester==ON_LINE)
        {
            if(CUSTOMER_CODE!=CC_ASE_KaohSiung &&
               CUSTOMER_CODE!=CC_ASE_KaohSiung_K12)                             //Steven 20131101 : Add ASE-K12
            {
                if(bEcho && bTimeOutForNoFullSite==false)                       //ChungHung 20141017 fix Full Site Test Time Out Problem
                {
                    iTestTask=60;
                    GetTesterResult(Index);
                    ProcessTestResult(Index);
                    bTestDuplicateErr=false;
                    if((CosFunction.bUSEJCETSiteMapMode==true ||
                        CUSTOMER_CODE==CC_ASE_M) &&                             //Ifor 20180417 : add ASE_M
                       LastSet.iRunStartMode==rsmAutoSiteMap)                   //JerryYang 20170502 (Steven) 修正auto site map hang up問題
                        bAvoidAddDoSiteMappingStep=true;                        //JerryYang 20170316 (Steven) 避免tester time out時Retry會重複進入DoJCETSiteMappingCHK(), 造成auto site mapping一次跳兩顆
                    return 1;
                }
                else                                                            //Jou 20101018 Start
                {
                    if(IniConfig.bI12TesterTimerOutNotNeedReTest==false)
                    {
                        IsTest=false;                                           //Steven 20110722 Start : Skip時,要重置GPIB測試狀態
                        if(TestIF.iGpibMode==InterfaceType_SPEA_Type)
                        {
                            bool flag[32]={false};
                            if(BAR_CODE_INSTALL!=ebctUninstall && TestIF_File.bEnableBarCode)
                                _RunTestProgram_BarMess(sizeof(flag), flag, 0, "");
                            else
                                _RunTestProgram(sizeof(flag), flag);
                        }
                        else
                        {
                            fMain->RunTestProgram(false);
                        }

                        if(CUSTOMER_CODE==CC_AMKOR_China ||                     //Steven 20110125
                           CUSTOMER_CODE==CC_QUALCOMM)                          //JerryYang 20170412 (Steven) add QUALCOMM
                        {
                            fMain->CloseGpibProgram(__FUNC__);
                        }
                        InitTestTask();
                        fMain->SendMSG_CMD(MSG_CMD_TimeOutRetrySend);           //Steven 20150304 : Add GPIB LOG
                    }
                    else
                    {
                        fMain->SendMSG_CMD(MSG_CMD_TimeOutRetryWait);           //Steven 20150304 : Add GPIB LOG
                    }
                }
            }
            return 2;
        }
        else if(TestIF_File.iTestType==TTL_MODE &&
                (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))                         //Isaac 20210309 :TTL RS232通訊
        {
            if(Check_TTL_Status(Index))                                         //get TTL data
            {                                                                   //Steven 20101228 Start: TTL TimeOut Hang
                for(i=0; i<TestSocket.iShtRow; i++)
                {
                    for(j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestSocket.Item[i][j]!=NULL_IC &&
                           TestSocket.Item[i][j]!=HAS_NULL_IC &&
                           iTesterBIN[i][j]!=-1 &&
                           TestSocket.Item[i][j]<TEST_PASS)
                        {
                            TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                            TestSocket.iBinData[i][j]=iTestBinCount;            //Steven 20190116 : 修正顯示錯誤
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTesterBIN[i][j], TestSocket.cSBin[i][j], "TestTimeOut_RETRY"); //Frank 20160505 add
                        }
                    }
                }
                bTestDuplicateErr=false;
                return 1;
            }

            if(IniConfig.bI12TesterTimerOutNotNeedReTest==false)                //resend SOT
            {
                IsTest=false;                                                   //Steven 20110722 Start : Skip時,要重置GPIB測試狀態
                fMain->RunTestProgram(false);
                InitTestTask();
                SendTTLRS232CSOTsignal();
                fMain->SendMSG_CMD(MSG_CMD_TimeOutRetrySend);                   //Steven 20150304 : Add GPIB LOG
            }
            else
            {
                fMain->SendMSG_CMD(MSG_CMD_TimeOutRetryWait);                   //Steven 20150304 : Add GPIB LOG
            }
            return 2;
        }
        else
        {
            if(Check_TTL_Status(Index))                                         //get TTL data
            {                                                                   //Steven 20101228 Start: TTL TimeOut Hang
                for(i=0; i<TestSocket.iShtRow; i++)
                {
                    for(j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(TestSocket.Item[i][j]!=NULL_IC &&
                           TestSocket.Item[i][j]!=HAS_NULL_IC &&
                           iTesterBIN[i][j]!=-1 &&
                           TestSocket.Item[i][j]<TEST_PASS)
                        {
                            TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                            TestSocket.iBinData[i][j]=iTestBinCount;            //Steven 20190116 : 修正顯示錯誤
                            TestSocket.PordRec[i][j].AddTestResultRecord(iTesterBIN[i][j], TestSocket.cSBin[i][j], "TestTimeOut_RETRY"); //Frank 20160505 add
                        }
                    }
                }
                bTestDuplicateErr=false;
                return 1;
            }
            else
            {
                return 2;
            }
        }
    }
    else                                                                        //ret==2==Skip
    {
        if(ret==K_HOME)
        {
            fAllMotorHome=false;
            bI22_NeedHomeDelay=true;
        }

        TestProcessSetToErr("Test Time Out Skip All Place to R bin");

        if(TestIF.iGpibMode==InterfaceType_SPEA_Type)                           //Steven 20141209 : Fixed Time Out Skip
        {
            bool flag[32]={false};
            if(BAR_CODE_INSTALL!=ebctUninstall && TestIF_File.bEnableBarCode)   //Ifor 20190129 : add Cognex EtherNet 通訊  //Ifor 20210407 add: 自製OCR
                _RunTestProgram_BarMess(sizeof(flag), flag, 0, "");
            else
                _RunTestProgram(sizeof(flag), flag);
        }
        else if(TestIF_File.iTestType==TTL_MODE &&
                (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))                         //Isaac 20210309 :TTL RS232通訊
        {
            fMain->RunTestProgram(false);
            SendTTLRS232CSOTsignal();
        }
        else
        {
            fMain->RunTestProgram(false);
        }

        fMain->SendMSG_CMD(MSG_CMD_TimeOutSkip);                                //Steven 20150304 : Add GPIB LOG
    }

    if(IniConfig.bI49_TesterTimeOutResetAllIC)                                  //Sam 20240215 : Tester time out show reset all ic
    {
        bResetNotMsg=true;
        ret=ShowMyMessageBox_YES_NO("Do you want to enable off-line clean unit mode.", "你確定要執行離線清料作業。");
        if(ret==1)                                                              //Sam 20240701 : Offline Clean out ic 新增權限設定
        {
            fMain->cbUserSelectChange(NULL);
            if(fSecurity->Insufficient(177)==false)
                ret=0;
        }

        if(ret==1)
        {
            if(DeviceForm.ContactMode==DirectContactMode || DeviceForm.ContactMode==DropContact)
            {
                TrayForm.bAutoFeed=true;
                if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)
                {
                    LastSet.bBreakSCKART=true;
                    TestIF_File.bSCKART_LotDeviceCheck=false;
                }

                bRestModeBackupParm=true;                                       //Sam 20250820 : [I49] 清料時 Contact Heigh 要拉高
                AnsiString S="";
                AnsiString szDir="";
                S=GetLastOpenFN();
                szDir=DataPath+S;
                szDir+="\\Contact.Data";
                iRestModeBackContactMode=ReadIniData(szDir, "Mode", "Contact", 0);
                dRestModeBackContactHeigh1=ReadIniData(szDir, "Test Arm1", "Contact", 0.0);
                dRestModeBackContactHeigh2=ReadIniData(szDir, "Test Arm2", "Contact", 0.0);
                WriteIniData(szDir, "Mode", "Contact",    DirectContactMode);
                WriteIniData(szDir, "Test Arm1", "Contact", dRestModeBackContactHeigh1+IniConfig.fI49_ChangeAboveSocket);
                WriteIniData(szDir, "Test Arm2", "Contact", dRestModeBackContactHeigh2+IniConfig.fI49_ChangeAboveSocket);
                fContact->ReadFile();
            }
            fMain->Reset("Reset by ProcessTesterTimeOut");
        }
        bResetNotMsg=false;
    }
    return 0;
}
//******************************************************************************
//
//  注意!! DoInterFaceErrorStep為Handler收發BIN相關, 修改時要小心!!
//
//******************************************************************************
int iDoInterFaceErrorStepTask=1;
bool DoInterFaceErrorStep(int ZAxisSelect)
{
    AnsiString asChinese=AnsiString("下壓次數已經超過設定值，請打開Chamber側門並清潔Socket");                   //Steven 20230104 : 直接寫死
    AnsiString asEnglish=AnsiString("Contact over setting # Please Open Chamber Side Door and clean socket"); //ChungHung 20121029 alter 客戶會誤解。
    AnsiString ErrPart;                                                         //kevin 20161105
    bool flag=false, flag1=false;
    int iMot=MTestZ1;

    int &Task=iDoInterFaceErrorStepTask;

    switch(Task)
    {
        case 1:
            if(ZAxisSelect==TESTZ1UP)
            {
                if(MOT[MTestZ2].Gali_ReadPos()==Prod.TestZ2_Safe)
                {
                    bD52IndexArmUp=false;                                       //JerryYang 20200804 : fix D52 & Index arm在shuttle高度預熱功能同時啟用時，發生tester timeout時會誤發handler hang up
                    if(bContactCTOverCHK)
                    {
                        bContactTimeOverStep=true;
                        if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed))
                        {
                            Task=100;
                        }
                    }
                    else
                    {
                        bNowDoInterFaceErrorStep=true;
                        if(MOT[MTestZ1].Gali_MotMove(Prod.All_TestZ_Test_Safe, MOT[MTestZ1].GailSpeed))
                            Task=100;
                    }
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_ReadPos()==Prod.TestZ1_Safe)
                {
                    bD52IndexArmUp=false;                                       //JerryYang 20200804 : fix D52 & Index arm在shuttle高度預熱功能同時啟用時，發生tester timeout時會誤發handler hang up
                    if(bContactCTOverCHK)
                    {
                        bContactTimeOverStep=true;
                        if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, MOT[MTestZ2].GailSpeed))
                        {
                            Task=100;
                        }
                    }
                    else
                    {
                        bNowDoInterFaceErrorStep=true;
                        if(MOT[MTestZ2].Gali_MotMove(Prod.All_TestZ_Test_Safe, MOT[MTestZ2].GailSpeed))
                            Task=100;
                    }
                }
            }
            break;
        case 100:
            if(bContactCTOverCHK)
            {
                MyMessageBox->lblChineseMsg->Font->Size=10;
                ShowMyMessage(asEnglish, asChinese);                            //jou 2012-06-05
                MyMessageBox->lblChineseMsg->Font->Size=12;
                Task=150;
            }
            else
            {
                if(bEchoStop==true)                                             //ChungHung 20130326 add
                {
                    bEchoStop=false;
                    ShowMyMessage("Receive ECHOSTOP Form TESTER");
                    Task=200;
                }
                else
                {
                    ProcessTesterTimeOut(ZAxisSelect);
                    SetNoiseDelay=false;
                    TestISTimeOut=false;
                    ProcessCount(ZAxisSelect);
                    if(ZAxisSelect==TESTZ1UP)
                    {
                        if(TestSocket.UseSiteHasIC())                           //jou 20230828 : 修正index all drop error
                            FTestSuck.MoveAllItem(TestSocket);
                    }
                    else
                    {
                        if(TestSocket.UseSiteHasIC())                           //jou 20230828 : 修正index all drop error
                            BTestSuck.MoveAllItem(TestSocket);
                    }

                    RecordHistroy(ZAxisSelect);
                    bFinshTest=true;
                    bInitStartDelayNotFinish=true;                              //Ifor 20181220 : add Init Start Delay Time Not Finish
                    bTJControlMode=false;                                       //Ifor 20190328 : add TJ Temp Over Range
                    ATC_InterfaceForm->SendHandler2DID(0, false);
                    SW[SwTesterPower].Off();
                    if(IniConfig.bI36TestTimeOut)                               //kevin 20161105 沒有收到測試資料強至取出ic
                        Task=110;
                    else
                        Task=200;
                }
                break;
            }
            break;
        case 110:                                                               //kevin 20161105 確認ic是否強制取走
            ErrPart=" ";
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    flag1=false;
                    if(ZAxisSelect==TESTZ1UP)
                    {
                        flag1=FTestSuck.Suck[i][j].GetStatus();
                        if(flag1==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i+IsNNMode()][j];            //Steven 20230712 : 修正NN mode alarm顯示
                        }
                    }
                    else
                    {
                        iMot=MTestZ2;
                        flag1=BTestSuck.Suck[i][j].GetStatus();
                        if(flag1==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i][j];
                        }
                    }
                }
            }

            if(flag==false)
            {
                if(ZAxisSelect==TESTZ1UP)
                {
                    if(TestSocket.UseSiteHasIC())                               //jou 20230828 : 修正index all drop error
                        FTestSuck.MoveAllItem(TestSocket);
                }
                else
                {
                    if(TestSocket.UseSiteHasIC())                               //jou 20230828 : 修正index all drop error
                        BTestSuck.MoveAllItem(TestSocket);
                }
                Task=200;
            }
            else
            {
                if(IndexAlarmInArmAway()==false)                                //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return false;
                }
                ShowErrorMessage("WAR0715", K_RETRY, iMot, false, ErrPart);
            }
            break;
        case 150:
            if(bContactCTOverCHK==false)                                        //已經開門了
            {
                Task=170;
            }
            else
            {
                MyMessageBox->lblChineseMsg->Font->Size=10;
                ShowMyMessage(asEnglish, asChinese);                            //jou 2012-06-05
                MyMessageBox->lblChineseMsg->Font->Size=12;
            }
            break;
        case 170:
            if(ZAxisSelect==TESTZ1UP)
            {
                if(MOT[MTestZ2].Gali_ReadPos()==Prod.TestZ2_Safe)
                {
                    if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test, MOT[MTestZ1].GailSpeed))
                    {
                        bContactTimeOverStep=false;
                        return true;
                    }
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_ReadPos()==Prod.TestZ1_Safe)
                {
                    if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test, MOT[MTestZ2].GailSpeed))
                    {
                        bContactTimeOverStep=false;
                        return true;
                    }
                }
            }
            break;
        case 200:
            if(ZAxisSelect==TESTZ1UP)
            {
                if(MOT[MTestZ2].Gali_ReadPos()==Prod.TestZ2_Safe)
                {
                    if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test, MOT[MTestZ1].GailSpeed))
                    {
                        bNowDoInterFaceErrorStep=false;
                        return true;
                    }
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_ReadPos()==Prod.TestZ1_Safe)
                {
                    if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test, MOT[MTestZ2].GailSpeed))
                    {
                        bNowDoInterFaceErrorStep=false;
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}
//marc 2007/10/17 end
//------------------------------------------------------------------------------
bool CheckIndexStatus(AnsiString str)
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
        static int iRetryCT=0;
        long lPos[4]={0, 0, 0, 0};                                              //kevin 20150915
        iRetryCT++;
        if(IndexStatus==Z1Up_Z2Down)
        {
            if(MOT[MTestZ1].ISZ1Up_Z2DownNoWait())
            {
                iRetryCT=0;
                return true;
            }
            else
            {
                if(iRetryCT>20)
                {
                    lPos[0]=IndexStatus;                                        //kevin 20150915
                    RecordIndexPositionError(AnsiString("CheckIndexStatus()Z1Up_Z2Down_")+str,true,false,false,false,&lPos[0]); //kevin 20150915 record
                    ShowIndexMotorError(AnsiString("CheckIndexStatus1"));
                    iRetryCT=0;                                                 //kevin 20110628 發生alarm 需清為0否則要關程式 //Steven 20180730 : 增加保護
                }
                return false;
            }
        }
        else if(IndexStatus==Z1Down_Z2Up)
        {
            if(MOT[MTestZ1].ISZ1Down_Z2UpNoWait())
            {
                iRetryCT=0;
                return true;
            }
            else
            {
                if(iRetryCT>20)
                {
                    lPos[0]=IndexStatus;                                        //kevin 20150915
                    RecordIndexPositionError(AnsiString("CheckIndexStatus()Z1Down_Z2Up_")+str, true,false,false,false, &lPos[0]); //kevin 20150915 record
                    ShowIndexMotorError(AnsiString("CheckIndexStatus2"));
                    iRetryCT=0;                                                 //kevin 20110628 發生alarm 需清為0否則要關程式 //Steven 20180730 : 增加保護
                }
                return false;
            }
        }
        else if(IndexStatus==IndexIsBack)
        {
            if(CheckBackError())
            {
                if(iRetryCT>20)
                {
                    lPos[0]=IndexStatus;                                        //kevin 20150915
                    RecordIndexPositionError(AnsiString("CheckIndexStatus()IndexIsBack_")+str,true,false,false,false,&lPos[0]); //kevin 20150915 record
                    ShowIndexMotorError(AnsiString("CheckIndexStatus3"));
                    iRetryCT=0;                                                 //kevin 20110628 發生alarm 需清為0否則要關程式 //Steven 20180730 : 增加保護
                }
                return false;
            }
        }
        else if(bUseTwoArm32Site==true  &&
                IndexStatus==Z1_Z2_Normal)
        {
            if(MOT[MTestZ1].ISZ1Up_Z2Up())
            {
                iRetryCT=0;
                return true;
            }
            else
            {
                if(iRetryCT>20)
                {
                    lPos[0]=IndexStatus;                                        //kevin 20150915
                    RecordIndexPositionError(AnsiString("CheckIndexStatus()_32Site4X8N Z1_Z2_Normal_")+str,true,false,false,false,&lPos[0]); //kevin 20150915 record
                    ShowIndexMotorError(AnsiString("CheckIndexStatus4"));
                    iRetryCT=0;                                                 //kevin 20110628 發生alarm 需清為0否則要關程式 //Steven 20180730 : 增加保護
                }
                return false;
            }
        }
        else if(bUseTwoArm32Site==true  &&
                IndexStatus==Z1_Z2_Down)
        {
            if(MOT[MTestZ1].ISZ1Down_Z2Down())
            {
                iRetryCT=0;
                return true;
            }
            else
            {
                if(iRetryCT>20)
                {
                    lPos[0]=IndexStatus;                                        //kevin 20150915
                    RecordIndexPositionError(AnsiString("CheckIndexStatus()_32Site4X8N Z1_Z2_Down_")+str, true, false, false, false, &lPos[0]); //kevin 20150915 record
                    ShowIndexMotorError(AnsiString("CheckIndexStatus5"));
                    iRetryCT=0;                                                 //kevin 20110628 發生alarm 需清為0否則要關程式 //Steven 20180730 : 增加保護
                }
                return false;
            }
        }
        else
        {
            if(MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe)==false)
            {
                if(iRetryCT>20)
                {
                    lPos[0]=Prod.TestZ1_Safe;                                   //kevin 20150915
                    RecordIndexPositionError(AnsiString("CheckIndexStatus()Prod.TestZ1_Safe_")+str, true, false, false, false, &lPos[0]); //kevin 20150915 record

                    ShowIndexMotorError(AnsiString("CheckIndexStatus6"));
                    iRetryCT=0;                                                 //kevin 20110628 發生alarm 需清為0否則要關程式 //Steven 20180730 : 增加保護
                }
                return false;
            }
            if(MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe)==false)
            {
                if(iRetryCT>20)
                {
                    lPos[0]=Prod.TestZ2_Safe;                                   //kevin 20150915
                    RecordIndexPositionError(AnsiString("CheckIndexStatus()Prod.TestZ2_Safe_")+str, true, false, false, false, &lPos[0]); //kevin 20150915 record

                    ShowIndexMotorError(AnsiString("CheckIndexStatus7"));
                    iRetryCT=0;                                                 //kevin 20110628 發生alarm 需清為0否則要關程式 //Steven 20180730 : 增加保護
                }
                return false;
            }
            IndexStatus=Z1_Z2_Normal;
        }
        return true;
    #endif
}
//------------------------------------------------------------------------------
bool CheckShuttlePos()
{
    for(int i=0; i<2; i++)
    {
        if(MOT[MInShuttle1+i].CompareCommandPos(Prod.InSHT[i].iLeft,  2)!=1 &&  //Sam 20230621 : Gap容許誤差改為1>2 //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條 //Ifor 20221117 add: 簡化流程
           MOT[MInShuttle1+i].CompareCommandPos(Prod.InSHT[i].iRight, 2)!=1)
        {
            return false;
        }

    }
    return true;
}
//------------------------------------------------------------------------------
bool CheckPlaceOutShuttle(int iShuttle)                                         //Steven 20181228 : Add Index Action  //ChungHung 20171116 modify for Index Action
{
    if(iShuttle==0)
    {
        if((FTestSuck.UseSiteHasIC() &&
            FTestSuck.AlreadyTest()) ||
           FTestNeedDestroy())
        {
            if(FRCarryKit.UseSiteHasIC())
                return false;

            MOT[MInShuttle1].ScanMotorStatus();
            if(InShtInLF(0)==false || MOT[MInShuttle1].Led[iInposLed])
                return false;

            MOT[MInShuttle1].fCanMoveM=false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if((BTestSuck.UseSiteHasIC() &&
            BTestSuck.AlreadyTest()) ||
           BTestNeedDestroy())
        {
            if(BRCarryKit.UseSiteHasIC())
                return false;

            MOT[MInShuttle2].ScanMotorStatus();
            if(InShtInLF(1)==false || MOT[MInShuttle2].Led[iInposLed])
                return false;

            MOT[MInShuttle2].fCanMoveM=false;
        }
        else
        {
            return false;
        }
    }
    return true;
}
//==============================================================================
//  Ver : 2003_07
//==============================================================================
int iCheckSocketHasIC;
void InitCheckSocketHasIC()
{
    iCheckSocketHasIC=1;
}
//------------------------------------------------------------------------------
TQPF_Timer CheckSocketHasICDelay;
TQPF_Timer CheckSocketHasICHangUpCheck;                                         //Steven 20210413 : 死雞保護
bool DoCheckSocketHasIC(int iSelArm)                                            //jou 20180802 : index pick error need index vaccum check
{
    static int iSiteCount=0;                                                    //kevin 20190709  index check one by one
    static AnsiString ErrPart="";

    int iIndexUpPos=0;
    int &Task=iCheckSocketHasIC;
    int iIndexCheckOffSet=IniConfig.fIndexCheckOffset*100;                      //ChungHung 20140807 add for ATK TestZ_Test + fIndexCheckOffset
    bool flag, flag2;
    AnsiString str;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
    {
        iIndexArmCheck_SG_Arm1=Offset.iIndexArmContact[0];
        iIndexArmCheck_SG_Arm2=Offset.iIndexArmContact[1];
    }
    else
    {
        iIndexArmCheck_SG_Arm1=0;
        iIndexArmCheck_SG_Arm2=0;
    }

    if(Task!=1)                                                                 //Steven 20210413 : 死雞保護
    {
        if(bHandlerPause)
            CheckSocketHasICHangUpCheck.SetSecAndOn(300);
        if(CheckSocketHasICHangUpCheck.Off())
        {
            RecordProcess("Auto State Record by DoCheckSocketHasIC");
            if(Task<2000)
                fMain->DoStateRecord(1);                                        //Steven 20220716 : 把State record獨立出來, 避免抓圖的時候被Alarm擋住
            else
                fMain->DoStateRecord(2);                                        //Steven 20220716 : 把State record獨立出來, 避免抓圖的時候被Alarm擋住
        }
    }

    switch(Task)
    {
        case 1:
            if(LastSet.bD41TestSocketICCheckSkip==true || bCheckIndex==true)
                return true;

            CheckSocketHasICHangUpCheck.SetSecAndOn(300);                       //Steven 20210413 : 死雞保護
            if(TestIF_File.iShuttleMode==1 && IniConfig.bShuttleMode50==false && TestIF_File.iShuttle_Sel==1 && Z1Safe==0)    //jou 2014-12-19 增加index arm轉換保護
                Task=2000;
            else if(iSelArm==2)                                                 //jou 20180802 : index pick error need index vaccum check
                Task=2000;
            else
                Task=1000;
            break;
        case 1000:                                                              //Arm 1 start
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoCheckSocketHasIC 319"))
            {
                Task=1001;
            }
            break;
        case 1001:                                                              //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoCheckSocketHasIC 1010"))
            {
                if(IniConfig.bC08_SocketSensor && TestIF_File.bEnSocketSensor &&
                   (TestIF_File.bSocketDisibleinitialcheck==false ||            //JerryYang 20201101 : Index check下壓前, 要先檢查socket Sensor
                    CUSTOMER_CODE==CC_GIGAS))                                   //Isaac 20220126 : 全智要求Index arm在上要強制偵測sensor(應該為off)
                {
                    flag=false;
                    str="check Socket Sensor state Up off,sensor : ";
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(Sen[SThreadPara.iSocketSensor[i]].Enable && Sen[SThreadPara.iSocketSensor[i]].IsOn()) //kevin 20150429
                        {
                            flag=true;
                            bIsSocketSensor=true;
                            str+=IntToStr(i+1);
                        }
                    }

                    if(flag)
                    {
                        ShowErrorMessage("WAR0322", K_RETRY, MTestZ1,false, str);//kevin 20130504 socket sensor
                        break;                                                  //kevin 20150429確認 socket sensor是否正常
                    }
                }
                Task=1010;
            }
            break;
        case 1010:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoCheckSocketHasIC 1010"))
            {
                Task=1020;
                if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                   INDEX_SUCKER_TYPE==1)                                        //kevin 20190912 add 一次開4 site check
                {
                    Task=1050;
                }
            }
            break;
        case 1020:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexCheckOffSet-iIndexArmCheck_SG_Arm1, iSpeedSlow))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                IndexStatus=Z1Down_Z2Up;
                Task=1030;
            }
            break;
        case 1030:
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    FTestSuck.Suck[i][j].On();
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        fiosetview->bIndexSuck[0][i][j]=true;
                        bIndexCheckNoStopVaccum=true;
                    }
                }
            }

            CheckSocketHasICDelay.SetSecAndOn(0.5);
            Task=1040;
            break;
        case 1040:                                                              //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            if(INDEX_SUCKER_TYPE==1)
            {
                fiosetview->ProcessIndexSuckDestroy1();
            }

            if(CheckSocketHasICDelay.Off())
            {
                Task=1050;
            }
            break;
        case 1050:
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                    //Steven 20140620 : 整合為Function

            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos+iIndexCheckOffSet-iIndexArmCheck_SG_Arm1, iSpeedFast))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                IndexStatus=Z1Down_Z2Up;                                        //kevin 20190912 ???
                if(ArmSpeed_File[IndexArm].bDevicConfirm &&                     //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE
                   INDEX_SUCKER_TYPE==1)                                        //kevin 20190530 add index check
                {
                    iSiteCount=0;
                    Task=1055;
                }
                else
                {
                    Task=1060;
                }

                if(CUSTOMER_CODE==CC_Greatek)                                   //Wei 20160413
                    CheckSocketHasICDelay.SetSecAndOn(5);                       //Steven 20110908 : 上來後也要Delay一下
                else
                    CheckSocketHasICDelay.SetSecAndOn(0.5);                     //Steven 20110908 : 上來後也要Delay一下
            }
            break;
        case 1055:                                                              //kevin 20190709 add onecycle index check 4 Site
            if(ArmSpeed_File[IndexArm].bDevicConfirm && INDEX_SUCKER_TYPE==1)   //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE   //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 0, iSiteCount);
                Task=1056;
            }
            break;
        case 1056:                                                              //kevin 20190709 20190531 index check 4 Site
            if(IndexCheck4Site(false, 0, iSiteCount))
            {
                iSiteCount++;
                if(iSiteCount<4)
                {
                    Task=1055;
                }
                else
                {
                    Task=1060;
                    iSiteCount=0;
                    if(CUSTOMER_CODE==CC_Greatek)                               //Wei 20160413
                        CheckSocketHasICDelay.SetSecAndOn(5);                   //Steven 20110908 : 上來後也要Delay一下
                    else
                        CheckSocketHasICDelay.SetSecAndOn(0.5);                 //Steven 20110908 : 上來後也要Delay一下
                }
            }
            break;
        case 1060:
            if(CheckSocketHasICDelay.Off())
            {
                flag=false;
                ErrPart=" ";
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        flag2=false;
                        FTestSuck.CheckVaccumIsIniaialON(i, j, flag2);
                        if(flag2==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i+IsNNMode()][j];            //Steven 20230712 : 修正NN mode alarm顯示

                            if(CUSTOMER_CODE==CC_SCS)                           //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                                FTestSuck.Suck[i][j].Off();
                        }
                    }
                }

                if((flag || TotalErrPart !="") && LastSet.iRealDummy==REALLY)   //kevin 20220422 add Cleanout index check (一次開4個SITE) 檢查有IC不會SHOW ALARM
                {
                    Task=1070;                                                  //fail
                }
                else
                {
                    if(TestIF_File.iShuttleMode==1 &&
                       IniConfig.bShuttleMode50==false &&
                       TestIF_File.iShuttle_Sel==0 &&
                       Z2Safe==0)                                               //jou 2014-12-19 增加index arm轉換保護
                        Task=3000;
                    else if(iSelArm==1)                                         //jou 20180802 : index pick error need index vaccum check
                        return true;
                    else
                        Task=2000;                                              //pass

                    if(IniConfig.bIndexArm2SupplyLight==true ||                 //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
                       TestIF_File.bForEgisTecTest==true     ||                 //Steven 20140922 : Arm2當作指紋測試
                       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&         //kevin 20150127 Arm1 下壓 arm2 測試
                        TestIF_File.bArm1PickPlaceArm2Test==true))              //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                    {
                        Task=3000;
                    }
                }
            }
            break;
        case 1070:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoCheckSocketHasIC 1070"))
            {
                if(CUSTOMER_CODE==CC_SCS)                                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                Task=1080;
            }
            break;
        case 1080:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoCheckSocketHasIC 1080"))
            {
                Task=1090;
            }
            break;
        case 1090:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bIsTestSitICFallDown=true;                                      //Steven 20130613
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)            //Steven 20130604 : Socket殘料要按Z1
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, ErrPart);
                }
                else
                {
                    ShowMyMessage("Arm1 detect Test Socket has IC error", "Arm 1偵測到Socket有IC殘留!!", "DoCheckSocketHasIC 1090");
                }
                TotalErrPart="";                                                //kevin 20220422
                ErrPart="";
                Task=1;
            }
            break;
        case 2000:                                                              //Arm 2 start
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoCheckSocketHasIC 2000"))
            {
                Task=2001;
            }
            break;
        case 2001:                                                              //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoCheckSocketHasIC 2010"))
            {
                if(IniConfig.bC08_SocketSensor &&
                   TestIF_File.bEnSocketSensor &&
                   (TestIF_File.bSocketDisibleinitialcheck==false ||            //JerryYang 20201101 : Index check下壓前, 要先檢查socket Sensor
                    CUSTOMER_CODE==CC_GIGAS))                                   //Isaac 20220126 : 全智要求Index arm在上要強制偵測sensor(應該為off)
                {
                    flag=false;
                    str="check Socket Sensor state Up off,sensor : ";
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                           Sen[SThreadPara.iSocketSensor[i]].IsOn())            //kevin 20150429
                        {
                            flag=true;
                            bIsSocketSensor=true;
                            str+=IntToStr(i+1);
                        }
                    }

                    if(flag)
                    {
                        ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, str);//kevin 20130504 socket sensor
                        break;                                                  //kevin 20150429確認 socket sensor是否正常
                    }
                }
                Task=2010;
            }
            break;
        case 2010:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoCheckSocketHasIC 2010"))
            {
                Task=2020;
                if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                   INDEX_SUCKER_TYPE==1)                                        //kevin 20190912 add 一次開4 site check
                    Task=2050;
            }
            break;
        case 2020:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexCheckOffSet-iIndexArmCheck_SG_Arm2, iSpeedSlow)) //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                IndexStatus=Z1Up_Z2Down;
                Task=2030;
            }
            break;
        case 2030:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    BTestSuck.Suck[i][j].On();
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        fiosetview->bIndexSuck[1][i][j]=true;
                        bIndexCheckNoStopVaccum=true;
                    }
                }
            }

            CheckSocketHasICDelay.SetSecAndOn(1);
            Task=2040;
            break;
        case 2040:                                                              //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            if(INDEX_SUCKER_TYPE==1)
            {
                fiosetview->ProcessIndexSuckDestroy2();
            }

            if(CheckSocketHasICDelay.Off())
            {
                Task=2050;
            }
            break;
        case 2050:
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                    //Steven 20140620 : 整合為Function

            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos+iIndexCheckOffSet-iIndexArmCheck_SG_Arm2, iSpeedFast))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                if(CUSTOMER_CODE==CC_Greatek)                                   //Wei 20160413
                    CheckSocketHasICDelay.SetSecAndOn(5);                       //Steven 20110908 : 上來後也要Delay一下
                else
                    CheckSocketHasICDelay.SetSecAndOn(0.5);                     //Steven 20110908 : 上來後也要Delay一下
                IndexStatus=Z1Up_Z2Down;

                if(ArmSpeed_File[IndexArm].bDevicConfirm &&                     //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE
                   INDEX_SUCKER_TYPE==1)                                        //kevin 20190530 add index check
                {
                    iSiteCount=0;
                    Task=2055;
                }
                else
                {
                    Task=2060;
                }
            }
            break;
        case 2055:                                                              //kevin 20190709 add onecycle index check 4 Site
            if(ArmSpeed_File[IndexArm].bDevicConfirm && INDEX_SUCKER_TYPE==1)   //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE   //kevin 20190530 add index check
            {
                IndexCheck4Site(true,1,iSiteCount);
                Task=2056;
            }
            break;
        case 2056:                                                              //kevin 20190709 20190531 index check 4 Site
            if(IndexCheck4Site(false, 1, iSiteCount))
            {
                iSiteCount++;
                if(iSiteCount<4)
                {
                    Task=2055;
                }
                else
                {
                    Task=2060;
                    iSiteCount=0;
                    if(CUSTOMER_CODE==CC_Greatek)                               //Wei 20160413
                        CheckSocketHasICDelay.SetSecAndOn(5);                   //Steven 20110908 : 上來後也要Delay一下
                    else
                        CheckSocketHasICDelay.SetSecAndOn(0.5);                 //Steven 20110908 : 上來後也要Delay一下
                }
            }
            break;
        case 2060:
            if(CheckSocketHasICDelay.Off())
            {
                flag=false;
                ErrPart=" ";
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        flag2=false;
                        BTestSuck.CheckVaccumIsIniaialON(i, j, flag2);
                        if(flag2==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i][j];

                            if(CUSTOMER_CODE==CC_SCS)                           //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                                BTestSuck.Suck[i][j].Off();
                        }
                    }
                }

                if((flag || TotalErrPart!="") && LastSet.iRealDummy==REALLY)    //kevin 20220422 add Cleanout index check (一次開4個SITE) 檢查有IC不會SHOW ALARM
                    Task=2070;                                                  //fail
                else if(iSelArm==2)
                    return true;
                else
                    Task=3000;                                                  //pass
            }
            break;
        case 2070:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoCheckSocketHasIC 2070"))
            {
                if(CUSTOMER_CODE==CC_SCS)                                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                Task=2080;
            }
            break;
        case 2080:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoCheckSocketHasIC 2080"))
            {
                Task=2090;
            }
            break;
        case 2090:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bIsTestSitICFallDown=true;                                      //Steven 20130613
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)            //Steven 20130604 : Socket殘料要按Z1
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY2, false, ErrPart);
                }
                else
                {
                    ShowMyMessage("Arm2 detect Test Socket has IC error", "Arm 2偵測到Socket有IC殘留!!", "DoCheckSocketHasIC 2090");
                }
                Task=1;
                ErrPart="";
                TotalErrPart="";                                                //kevin 20220422
            }
            break;
        case 3000:                                                              //finish
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoCheckSocketHasIC 3000"))
            {
                bFTestSuckDrop=false;
                bBTestSuckDrop=false;
                return true;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
int iTestYTask=1;
void InitTestYTask()
{
    iTestYTask=1;
}
//------------------------------------------------------------------------------
void DoTestY()
{
    static bool fFront=false;

    int &Task=iTestYTask;
    int Type1=0, Type2=0;
    int iSpeedZ=0;
    bool flag;

    switch(Task)
    {
        case 1:
            dTestSec=TestIF_File.iInitialMaxTime;                               //2013-11-27    Dell Add Index soak time
            bNeedIndexSoakTime=false;                                           //2013-11-27    Dell Add Index soak time
            if(IsInArmCleanOutFinish() ||
               IsInArmOneCycleFinish() ||                                       //Steven 20131029 : 解決Index Position Error
               bCanNotDisableOneCycle)
            {
                if(CanYieldAlarmRemainInSHT() &&                                //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                   (bCanNotDisableOneCycle ||
                   (TestHeadHasIC()==false)))
                {
                    bCanNotDisableOneCycle=true;

                    if(IndexStatus!=Z1_Z2_Normal)
                    {
                        if(bCheckIndex==false)                                  //jou 2011-12-08 做piggy-back時，不用做兩次socket check
                        {
                            InitCheckSocketHasIC();
                            Task=20;
                        }
                        else
                        {
                            Task=25;
                        }
//                        break;                                                //Steven 20180813 : add index arm speed
                    }
                    else
                    {
                        Task=25;
//                        break;
                    }
                }

                if(bCanNotDisableOneCycle ||
                   (TestHeadHasIC()==false &&
                    InputShuttleHasIC()==false &&
                    InArmSuck.HasRealIC()==false))
                {
                    bCanNotDisableOneCycle=true;

                    if(IndexStatus!=Z1_Z2_Normal &&
                       IndexStatus!=IndexIsBack)                                //Steven 20221027 : Fixed D51 hang up
                    {
                        if(bCheckIndex==false)                                  //jou 2011-12-08 做piggy-back時，不用做兩次socket check
                        {
                            InitCheckSocketHasIC();
                            Task=20;
                        }
                        else
                        {
                            Task=25;
                        }
//                        break;                                                //Steven 20180813 : add index arm speed
                    }
                    else
                    {
                        if(LastSet.iTemperature==Tempture_Hot      &&           //Steven 20220428 : Fixed for one cycle hang up
                           TestHeadHasIC()==false  &&
                           InputShuttleHasIC()==false &&
                           InArmSuck.HasType(HAS_HOT_IC)==false)
                        {
                            if(IndexStatus==Z1_Z2_Normal ||
                               IndexStatus==IndexIsBack)                        //Steven 20221027 : Fixed D51 hang up
                            {
                                if(FRCarryKit.UseSiteHasIC())
                                {
                                    MOT[MInShuttle1].fCanMoveM=true;
                                    iIndexTakeDeviceChk1=0;                     //kevin 20190103 回吸檢測狀態
                                }

                                if(BRCarryKit.UseSiteHasIC())
                                {
                                    MOT[MInShuttle2].fCanMoveM=true;
                                    iIndexTakeDeviceChk2=0;                     //kevin 20190103 回吸檢測狀態
                                }
                            }
                            Task=1;
                            break;
                        }
                        Task=25;
//                        break;
                    }
                }
            }

            if(bCanNotDisableOneCycle==false)
            {
                if(bUseTwoArm32Site==true)
                    Task=260;                                                   //InitTestYTwoArm32Site()
                else
                    Task=50;
            }

//            if(Task!=20)                                                      //Steven 20180813 : add index arm speed
                break;
        case 20:
            if(CosFunction.bIndexCheckCanTurnOff &&                             //Isaac 20211019 : 可選擇做index check的時機
               ((IniConfig.iD71IndexCheckOnOffMode==0 && bLotStartEndNeedIndexCheck==false) ||
                (IniConfig.iD71IndexCheckOnOffMode==1 && bIndexJamNeedIndexcheck==false) ||
                 IniConfig.iD71IndexCheckOnOffMode==2))
            {
                Task=25;                                                        //不檢查
            }
            else if(IniConfig.bD55DisableIndexCheck &&                          //ChungHung 20120606 DisableIndexCheck
                    REAL_TIME_CCD==true && COM2->bCCDDummyRum==false)           //Steven 20150723 : Fixed for SCK
            {
                Task=25;
            }
            else if(CosFunction.bBeforeAutoCleanOnlyUseRTC==true &&             //JerryYang 20161216 (Steven) auto clean的前後只靠RTC來檢查socket,不做index下壓至socket吸真空
                    bIsAutoOneCycle==true &&
                    REAL_TIME_CCD==true &&
                    COM2->bCCDDummyRum==false)
            {
                Task=25;
            }
            else if(CosFunction.bAfterAutoCleanNoIndexCheck &&                  //Steven 20191212 : 劉仁洲說Auto Clean只要作一次Index Check
                    IniConfig.iD69IndexCheckModeForAutoClean==2 &&
                    bIsAutoOneCycle==true)
            {
                Task=25;
            }
            else
            {
                if(DoCheckSocketHasIC()==true)
                {
                    Task=25;
                }
            }

//            if(Task!=25)                                                      //Steven 20180813 : add index arm speed
                break;
        case 25:
            iSpeedZ=(INDEX_PRESS_TYPE==e240KG || INDEX_PRESS_TYPE==e400KG || INDEX_PRESS_TYPE==e260KG || INDEX_PRESS_TYPE==e360KG)?200000:50000;        //Steven 20110503 : 240KG加速 //Steven 20131007 : Index 1.5KW, 400KG
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedZ, "DoTestY 25"))
            {
                Task=30;
                if(CosFunction.bIndexCheckCanTurnOff)                           //Isaac 20211019 : 可選擇做index check的時機，旗標重置
                {
                    bLotStartEndNeedIndexCheck=false;
                    bIndexJamNeedIndexcheck=false;
                }
            }
//            if(Task!=30)                                                      //Steven 20180813 : add index arm speed
                break;
        case 30:
            if(IniConfig.bD51UseOnecycleCleanOutFinishTestArmAtRear)
            {
                Task=45;
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front_EndWaitPos, Prod.TestY2_Rear, 50000, "DoTestY 30")) //981118 jou Y1 +2000 easy change kit
                    Task=40;
            }
//            if(Task!=40)                                                      //Steven 20180813 : add index arm speed
                break;
        case 40:
            IndexStatus=Z1_Z2_Normal;
            bCanNotDisableOneCycle=false;
            Task=1;
            break;
        case 45:
            if(TestArmBackPos())
            {
                IndexStatus=IndexIsBack;
                bCanNotDisableOneCycle=false;
                Task=1;
            }
            break;
        case 50:
            flag=false;
            if(FTestSuck.UseSiteHasIC())
            {
                if(FTestSuck.AlreadyTest())
                    flag=true;
            }

            if(FTestSuck.UseSiteHasIC()==false)
                Type1=0;
            else if(flag==true)
                Type1=2;
            else
                Type1=1;

            flag=false;

            if(BTestSuck.UseSiteHasIC())
            {
                if(BTestSuck.AlreadyTest())
                {
                    flag=true;
                }
            }

            if(BTestSuck.UseSiteHasIC()==false)
                Type2=0;
            else if(flag==true)
                Type2=2;
            else
                Type2=1;

            if(FTestSuck.PartAlreadyTest())
            {
                fFront=false;
            }
            else if(BTestSuck.PartAlreadyTest())
            {
                fFront=true;
            }
            else
            {
                fFront=true;
                if(     Type1==2) fFront=true;
                else if(Type2==2) fFront=false;
                else if(Type1==0) fFront=true;
                else if(Type2==0) fFront=false;
                else if(Type1==1) fFront=false;
                else if(Type2==1) fFront=true;
                if(Type1==0 && Type2==0)
                {
                    if(FLCarryKit.UseSiteHasIC())
                    {
                        fFront=true;
                    }
                    else
                    {
                        if(BLCarryKit.UseSiteHasIC())                           //need check chang else
                            fFront=false;
                    }
                }
            }

            if(fFront)
                Task=100;
            else
                Task=200;
            break;
        case 60:
            if(IniConfig.bD47EnableSocketPurgeFunction &&
               IniConfig.iD47SocketPurgeCount!=0 &&
               iOneCycle==0 &&
               iCleanOut==0)
            {
                if(LastSet.iD47SocketTestedCount>=IniConfig.iD47SocketPurgeCount)
                {
                    LastSet.iD47SocketTestedCount=0;
                    iClearSocketFunction=1;
                    iOneCycle=1;
                    fMain->DebugOneCycleHotPlate("DoTestY_60");                 //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
                }
            }

            if(IsInArmOneCycleFinish() ||
               IsInArmCleanOutFinish())
            {
                if(IniConfig.bIndexArm2SupplyLight==true ||                     //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
                   TestIF_File.bForEgisTecTest==true     ||                     //Steven 20140922 : Arm2當作指紋測試
                   (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&             //kevin 20150127 Arm1 下壓 arm2 測試
                    TestIF_File.bArm1PickPlaceArm2Test==true))                  //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                {
                    if(FTestSuck.UseSiteHasIC()==false &&
                       BTestSuck.UseSiteHasIC()==false &&
                       FLCarryKit.UseSiteHasIC()==false &&
                       InArmSuck.HasRealIC()==false)
                    {
                        Task=1;
                        break;
                    }
                }

                if(CanYieldAlarmRemainInSHT()==true)                            //JerryYang 20220923 : yield alarm時觸發half one cycle(shuttle保留IC不測試跳ONE CYCLE FINISH)
                {
                    if((FTestSuck.UseSiteHasIC()==false || FTestSuck.HasRealIC()==false) &&
                       (BTestSuck.UseSiteHasIC()==false || BTestSuck.HasRealIC()==false))
                    {
                        FTestSuck.ClearAll();
                        BTestSuck.ClearAll();
                        Task=1;
                        break;
                    }
                }

                if((FTestSuck.UseSiteHasIC()==false  || FTestSuck.HasRealIC()==false) &&
                   (BTestSuck.UseSiteHasIC()==false  || BTestSuck.HasRealIC()==false) &&
                   (FLCarryKit.UseSiteHasIC()==false || FLCarryKit.HasRealIC()==false) &&      //Steven 20191213 : 修正One Cycle的動作
                   (BLCarryKit.UseSiteHasIC()==false || BLCarryKit.HasRealIC()==false) &&
                   InArmSuck.HasRealIC()==false)
                {
                    FTestSuck.ClearAll();
                    BTestSuck.ClearAll();
                    FLCarryKit.ClearAll();
                    BLCarryKit.ClearAll();

                    Task=1;
                    break;
                }
            }

            if(iCleanOut!=0 && IndexHasIC()==false && InputShuttleHasIC()==false)
                break;

            if(Prod.bWhenHappenTestedTimeBelowUseInitialDelay)                  //ChungHung 20140425 add for TSMC Device
            {
                if(dTestSec<Prod.iEveryFirstDeviceUseInitialDelay)
                {
                    if(bUseInitTempOffset)                                      //Steven 20150707 : Add log for initial offset
                    {
                        RecordProcess("After short test time trigger initial offset function.");
                    }

                    iInitContactCount=0;                                        //Steven 20141117 : 起測時溫度要補Offset
                    if(CosFunction.bFuncStateStopFirtDelay==true)               //jou 2014-09-03 Function State Stop Firt Initital Delay Time
                    {
                        if(bPiggyBackIndexCheck==false && LastSet.iRunStartMode!=rsmAutoSiteMap)
                        {
                            bNeedInitialTestDelay=true;
                            Prod.iInitialDelay=TestIF.iInitialDelay_4;          //ChungHung 20141210 add for SCK want to every event have delay
                        }
                    }
                    else
                    {
                        if(bUseInitDelay)                                       //ChungHung 20141027 add 只有加熱模式需要Initial start delay count  //Ifor 20180109 (Steven) : add KYEC 常高溫都要跑Initial Delay
                        {
                            bDoWhenHappenTestedTimeBlowUseInitialDelay=true;    //ChungHung 20140105 add for SCK have order
                        }
                        else
                        {
                            bDoWhenHappenTestedTimeBlowUseInitialDelay=false;   //ChungHung 20140105 add for SCK have order
                        }
                    }
                }
            }

            if(LastSet.iTemperature==Tempture_Hot &&
               Temperature.iIndexSoakTime>0)                                    //2013-11-27   Dell    需要做Index soak time
            {
                if((dTestSec<Temperature.iOSTime) || bNeedIndexSoakTime)        //Steven 20151105 : 改回原本的
                {
                    hIndexSoakTime.SetSecAndOn(Temperature.iIndexSoakTime);
                    iSoakTimer = Temperature.iIndexSoakTime;
                    fMain->tSoakTimer.LatchCycleTimeSec(true);
                    bNeedIndexSoakTime = false;
                    Task=65;
                }
                else
                {
                    Task=70;
                }
            }
            else
            {
                Task=70;
            }
            break;
        case 65:                                                                //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            if(hIndexSoakTime.Off())
                Task=70;
            break;
        case 70:
            if(CosFunction.bOEEFunction)                                        //Steven 20180417 (Jou) : OEE功能
            {
                fObserver->bTestIndexZ=true;
            }
            fFront=!fFront;
            if(fFront)
            {
                Task=100;
            }
            else
            {
                Task=200;
            }

            if(Task!=100)                                                       //Steven 20180813 : add index arm speed
                break;
        case 100:
            InitTestYFrontTask();
            //----- by dell ccd realtime-------------
            bRealCCDSendArm=true;
            //---------------------------------------
            Task=110;
        case 110:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                bNeedIndexSoakTime=true;                                        //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            }

            if(DoTestYFront())
            {
                if(IsInArmOneCycleFinish() ||
                   IsInArmCleanOutFinish())
                {
                    if(LastSet.iTemperature==Tempture_Hot &&
                       FTestSuck.UseSiteHasIC()==false   &&
                       BTestSuck.UseSiteHasIC()==false   &&
                       FLCarryKit.UseSiteHasIC()==false  &&
                       BLCarryKit.UseSiteHasIC()==false  &&
                       FRCarryKit.UseSiteHasIC()==false  &&                     //Sam 20230809 : OneCycle 最後一次 IndexArm 動作，需要等待 OutShuttle 動作做完才能做 IndexCheck，避免黏料壓壞 IC
                       InArmSuck.HasType(HAS_HOT_IC)==false)
                    {
                        if(IndexStatus==Z1_Z2_Normal)
                        {
                            if(FRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle1].fCanMoveM=true;
                                iIndexTakeDeviceChk1=0;                         //kevin 20190103 回吸檢測狀態
                            }

                            if(BRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle2].fCanMoveM=true;
                                iIndexTakeDeviceChk2=0;                         //kevin 20190103 回吸檢測狀態
                            }
                            Task=1;
                            break;
                        }
                    }
                }
                Task=60;
            }
            break;
        case 200:
            InitTestYRearTask();
            //----- by dell ccd realtime-------------
            bRealCCDSendArm=true;
            //---------------------------------------
            Task=210;
        case 210:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                bNeedIndexSoakTime=true;                                        //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            }

            if(DoTestYRear())
            {
                if(IsInArmOneCycleFinish() ||
                   IsInArmCleanOutFinish())                                     //Steven 20220426 : 避免One Cycle發生index position error
                {
                    if(LastSet.iTemperature==Tempture_Hot &&
                       FTestSuck.UseSiteHasIC()==false   &&
                       BTestSuck.UseSiteHasIC()==false   &&
                       FLCarryKit.UseSiteHasIC()==false  &&
                       BLCarryKit.UseSiteHasIC()==false  &&
                       BRCarryKit.UseSiteHasIC()==false  &&                     //Sam 20230809 : OneCycle 最後一次 IndexArm 動作，需要等待 OutShuttle 動作做完才能做 IndexCheck，避免黏料壓壞 IC
                       InArmSuck.HasType(HAS_HOT_IC)==false)
                    {
                        if(IndexStatus==Z1_Z2_Normal)
                        {
                            if(FRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle1].fCanMoveM=true;
                                iIndexTakeDeviceChk1=0;                         //kevin 20190103 回吸檢測狀態
                            }

                            if(BRCarryKit.UseSiteHasIC())
                            {
                                MOT[MInShuttle2].fCanMoveM=true;
                                iIndexTakeDeviceChk2=0;                         //kevin 20190103 回吸檢測狀態
                            }
                        }
                        Task=1;
                        break;
                    }
                }
                Task=60;
            }
            break;
        case 260:
            if(IniConfig.bD47EnableSocketPurgeFunction &&
               IniConfig.iD47SocketPurgeCount!=0 &&
               iOneCycle==0 &&
               iCleanOut==0)
            {
                if(LastSet.iD47SocketTestedCount>=IniConfig.iD47SocketPurgeCount)
                {
                    LastSet.iD47SocketTestedCount=0;
                    iClearSocketFunction=1;
                    iOneCycle=1;
                    fMain->DebugOneCycleHotPlate("DoTestY_260");                //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
                }
            }

            if(IsInArmOneCycleFinish() ||
               IsInArmCleanOutFinish())                                         //Steven 20220426 : 避免One Cycle發生index position error
            {
                if(FTestSuck.UseSiteHasIC()==false  &&
                   BTestSuck.UseSiteHasIC()==false  &&
                   FLCarryKit.UseSiteHasIC()==false &&
                   BLCarryKit.UseSiteHasIC()==false)
                {
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(InArmSuck.HasType(HAS_HOT_IC)==false)
                        {
                            Task=1;
                            break;
                        }
                    }
                    else                                                        //Steven 20220514 : 修正Clean Out死雞
                    {
                        if(IsNNMode()!=NN_2Row)
                        {
                            FTestSuck.ClearAll();
                            BTestSuck.ClearAll();
                        }

                        if(InArmSuck.HasIC()==false)
                        {
                            Task=1;
                            break;
                        }
                    }
                }
            }

            if(iCleanOut!=0 && IndexHasIC()==false && InputShuttleHasIC()==false)
                break;

            Task=300;
            break;
        case 300:
            if(CosFunction.bOEEFunction)                                        //Steven 20180417 (Jou) : OEE功能
            {
                fObserver->bTestIndexZ=true;
            }
            InitTestYTwoArm32SiteTask();
            //----- by dell ccd realtime-------------
            bRealCCDSendArm=true;
            //---------------------------------------
            Task=310;
//            break;                                                            //Steven 20180813 : add index arm speed
        case 310:
            if(CheckHeaterOK()==false)                                          //Steven 20250116 : 確認HeaterOK
            {
                bHangTimePause=true;
                bNeedIndexSoakTime=true;                                        //2013-11-27    Dell Add Index soak time "當測試秒數太短(ex open/shot)" or "fHeaterOK==false"
            }

            if(DoTestY_TwoArm32Site())
                Task=260;
            break;
    }
}
//------------------------------------------------------------------------------
int iTestHeadMotorTask=1;
int iProcessIndexSuckDestroyCnt=0;                                              //RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
void InitialTestHeadMotorTask()
{
    iTestHeadMotorTask=1;
    if(iProcessIndexSuckDestroyCnt>15 || TestIF_File.bRENESAS_EnableFTCT==false)//RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
        iProcessIndexSuckDestroyCnt=0;

    if(MOT[MTestY1].Motor!=NULL)                                                //Sam 20220718 : 防止 OneCycle and CleanOut IndexArm 偷跑
        MOT[MTestY1].Gali_Command("VS0;SP0,0,0,0;", __FUNC__);
}
//------------------------------------------------------------------------------
bool IsIndexRunCycle()
{
    if(iTestHeadMotorTask==600)
    {
        if(bCanNotDisableOneCycle==true)
            return false;
        return true;
    }
    else
    {
        return false;
    }
}
extern int CheckSuckInitialStatus(TMyKitSuck &Ptr, int iR, int iC);
//------------------------------------------------------------------------------
bool CheckIndexArmInitState()
{
    int ret1[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};
    int ret2[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};
    bool bHasFail1=false, bHasFail2=false;
    AnsiString errPart1="at Index Arm 1";
    AnsiString errPart2="at Index Arm 2";

    for(int i=0; i<FTestSuck.iShtRow; i++)
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j] ||                               //ChungHung 20130910 alter for SCK can close site by Index
               LastSet.bUseTestSocket[1][i][j])                                 //Steven 20100824 : 關Site的位置不做檢查
            {
                ret1[i][j]=CheckSuckInitialStatus(FTestSuck, i, j);

                if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                    TestIF_File.bArm1PickPlaceArm2Test==true) &&
                    TestIF_File.bCheckArm2Vacuum==false)                        //Steven 20150129 : 需要確認Arm2有沒有粘料   //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
                {
                    ret2[i][j]=0;                                               //kevin 20150128
                }
                else
                {
                    ret2[i][j]=CheckSuckInitialStatus(BTestSuck, i, j);
                }
            }
            else
            {
                ret1[i][j]=0;
                ret2[i][j]=0;
            }

            if(ret1[i][j]!=0)
            {
                bHasFail1=true;
                errPart1+=IndexSuckName[i+IsNNMode()][j];                       //Steven 20230712 : 修正NN mode alarm顯示
            }
            if(ret2[i][j]!=0)
            {
                bHasFail2=true;
                errPart2+=IndexSuckName[i][j];
            }
        }
    }

    if(bHasFail1)
        ShowErrorMessage("WAR0320", K_SKIP, MTestZ1, false, errPart1);          //jou 2011-11-08 retry -> skip字義上比較恰當
    if(bHasFail2)
        ShowErrorMessage("WAR0320", K_SKIP, MTestZ2, false, errPart2);

    for(int i=0; i<FTestSuck.iShtRow; i++)
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            if(ret1[i][j]==Vaccum_Initial_Off)
                FTestSuck.SetItemData(i, j, HAS_NULL_IC);
            if(ret2[i][j]==Vaccum_Initial_Off)
                BTestSuck.SetItemData(i, j, HAS_NULL_IC);
        }
    }
    return (bHasFail1 || bHasFail2);
}
//------------------------------------------------------------------------------
// V2.0
//------------------------------------------------------------------------------
//jou 2014-06-24 RTC 自動進行Model驗證
bool CheckTwoArmSiteMap()
{
    bool bFlag=false;
    for(int i=0; i<FTestSuck.iShtRow; i++)
    {
        for(int j=0; j<FTestSuck.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j] ^ LastSet.bUseTestSocket[1][i][j])
            {
                bFlag=true;
            }
        }
    }
    return bFlag;
}
//------------------------------------------------------------------------------
//jou 2014-06-24 RTC 自動進行Model驗證
//iSelArm = 0 , Arm1 & Arm2
//iSelArm = 1 , Arm1
//iSelArm = 2 , Arm2
int SendSiteMapToRTC(bool bSendToRTC, int iSelArm)
{
//    Handler 控制開關Site :
//    1~4 碼   @MAP
//      5 碼   0 ~ 6
//                    ========================================================
//                    第5碼        Site 分佈樣式
//                    ========================================================
//                    asSiteMap    0         1 x 1
//                                 1         1 x 2
//                                 2         1 x 4
//                                 3         2 x 2
//                                 7         2 x 3
//                                 4         2 x 4
//                                 5         2 x 5
//                                 5         2 x 6
//                                 6         2 x 8
//                    ========================================================
//    6~13碼   asRowA       11111111  Aa ~ Ah  0 代表關Site , 1 代表開Site
//   14~21碼   asRowB       11111111  Ba ~ Bh  0 代表關Site , 1 代表開Site
//      22碼   +

    int iSite1=0, iSite2=0, iTotalSite=0;
    AnsiString asRow1="", asRow2="";

    int iMode=GetRTCSiteMap();

    if(iSelArm==0 || IniConfig.bA09_ByArmCloseSite==false)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)                 //JerryYang 20250818 : fix 格式錯誤造成timeout
        {
            asRow1+=(LastSet.bUseTestSocket[0][0][j] || LastSet.bUseTestSocket[1][0][j])?"1":"0";
            asRow2+=(LastSet.bUseTestSocket[0][1][j] || LastSet.bUseTestSocket[1][1][j])?"1":"0";

            if(LastSet.bUseTestSocket[0][0][j] || LastSet.bUseTestSocket[1][0][j])
                iSite1++;
            if(LastSet.bUseTestSocket[0][1][j] || LastSet.bUseTestSocket[1][1][j])
                iSite2++;
        }
    }
    else if(iSelArm==1)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)                 //JerryYang 20250818 : fix 格式錯誤造成timeout
        {
            asRow1+=(LastSet.bUseTestSocket[0][0][j])?"1":"0";
            asRow2+=(LastSet.bUseTestSocket[0][1][j])?"1":"0";

            if(LastSet.bUseTestSocket[0][0][j])
                iSite1++;
            if(LastSet.bUseTestSocket[0][1][j])
                iSite2++;
        }
    }
    else if(iSelArm==2)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)                 //JerryYang 20250818 : fix 格式錯誤造成timeout
        {
            asRow1+=(LastSet.bUseTestSocket[1][0][j])?"1":"0";
            asRow2+=(LastSet.bUseTestSocket[1][1][j])?"1":"0";

            if(LastSet.bUseTestSocket[1][0][j])
                iSite1++;
            if(LastSet.bUseTestSocket[1][1][j])
                iSite2++;
        }
    }

    if(bSendToRTC==true)
    {
        COM2->sRealTimeCom_Send[COM2->rtSiteMap] = "@MAP"+AnsiString(iMode)+asRow1+asRow2+"+";
        COM2->SendCommToVision(COM2->rtSiteMap, true);
        return iMode;
    }
    else
    {
        iTotalSite=iSite1+iSite2;
        return iTotalSite;
    }
}
//------------------------------------------------------------------------------
const int iCASE_REAL_CCD2=40200;
const int iCASE_REAL_CCD3=40300;
const int iCASE_REAL_CCD4=40400;
const int iCASE_REAL_CCD5=40500;
const int iCASE_REAL_CCD6=40510;
TQPF_Timer DoTestHeadMotorDelay;
TQPF_Timer DoTestHeadMotorDelay2;
TQPF_Timer ReadTorqueDelay;                                                     //kevin 20210824 read torque wait alarm time
TQPF_Timer DoTestHeadMotorDelay3;
TQPF_Timer DoUseSocketHeating;                                                  //Ztex 2024.09.07 Add Use Socket Heating
//------------------------------------------------------------------------------
void DoTestHeadMotor()
{
    static int iRetryCount=0;
    static int iSiteCount=0;
    static int iToqureCount=0;
    static bool bOneTimeFlag=true;
    static bool bFirstTime=true;
    static bool bFlag[2]={false, false};
    static bool bintered1=true;                                                 //Isaac 20200922 : 紀錄indexArmY encoder值和command值
    static DWORD startTick=-1, endTick=0, nowTick=0;
    static AnsiString ErrPart="";

    int &Task=iTestHeadMotorTask;
    int ret, TorqueData, iIndexUpPos=0, ccRet;
    int sp, iMaxPreasure=0, iIndexArm[3]={0, 0, 0};
    bool flag=false, flag2, bIndexSuckCheck;
    bool TMode=false;
    AnsiString str;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
    {
        iIndexArmCheck_SG_Arm1=Offset.iIndexArmContact[0];
        iIndexArmCheck_SG_Arm2=Offset.iIndexArmContact[1];
    }
    else
    {
        iIndexArmCheck_SG_Arm1=0;
        iIndexArmCheck_SG_Arm2=0;
    }

    if(IniConfig.bEnableCCDUSETCPIP)                                            //kevin 20110811 start
    {
        if(ScanCCDProgram())
        {
            CCDInterfaceForm->CCDRunExec();
        }
    }                                                                           //kevin 20110811 End

    if(iPauseBackUp!=-1 &&
       FTestSuck.IsShtSuckFinish()==true &&                                     //ChungHung 20110901 add
       FTestSuck.IsShtDestroyFinish()==true   &&
       BTestSuck.IsShtSuckFinish()==true &&
       BTestSuck.IsShtDestroyFinish()==true)
    {
        return;
    }

    if(USE_16_HEATER==eht16Heater     ||
       USE_16_HEATER==eht16HeaterEJ1N ||
       USE_16_HEATER==eht32HeaterEJ1N ||                                        //Steven 20140923 : Index使用EJ1N版32組加熱器
       USE_16_HEATER==eht32HeaterKT4H ||                                        //Steven 20150211 : Index使用KT4H版32組加熱器
       USE_16_HEATER==eht16HeaterDTME08 ||                                      //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
       USE_16_HEATER==eht32HeaterDTME08)                                        //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
    {
        ccRet=CheckIndexConnect();                                              //20111130  Dell    Connect Check Start
        if(ccRet>0)
        {
            ret=ShowErrorMessage("WAR0360", K_RETRY, MTestZ1+(ccRet-1));
        }
    }
    //20111130  Dell    Connect Check End

    if(bIndexCheckState)                                                        //wei 20150903 如果有Auto clean 就重頭開始
    {
        bIndexCheckState=false;
        Task=1;
    }

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&                                        //ChungHung 20210113 add for Alignment CCD start
       TestIF.bEnableAutoAlignment==true &&                                     //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
       (LastSet.iRealDummy==HAS_TRAY ||
        LastSet.iRealDummy==REALLY))
    {
        if(lInArmAutoAlignmentCKTimingFlag!=0x00   || bRunInArmAutoAlignment ||
           lOutArmAutoAlignmentCKTimeingFlag!=0x00 || bRunOutArmAutoAlignment)
            return;
    }

    if((bSht1LoseICErr==true ||
        bSht2LoseICErr==true) &&
       LastSet.iRealDummy==REALLY)                                              //Jimmychiu 20230921 : add index arm stop when outshuttle lose ic
    {
        return;
    }

    switch(Task)
    {
        case 1:                                                                 //確認Index Arm 吸嘴狀態
            ReadWriteBinCountMode(true);                                        //kevin 20210825 寫 Bin 1  Bin 2...資料記錄
            if(CUSTOMER_CODE==CC_ASE_KaohSiung && IniConfig.bG11ASEReport)
                ReadWriteTrayID(true);                                          // kevin 20220618 read Tray ID

            if(Prod.TestZ1_Test==0)
                iSocketSenSosPos1=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos1=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000;

            if(Prod.TestZ2_Test==0)
                iSocketSenSosPos2=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos2=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000;

            if(bUseInitTempOffset)                                              //Steven 20141117 : 起測時溫度要補Offset
            {
                RecordProcess("After index check trigger initial offset function.(Temp)");
                iInitContactCount=0;                                            //Steven 20141117 : 起測時溫度要補Offset
                fHeaterOK=false;
            }

            if(INSTALL_SOCKET_CLAMP)                                            //JerryYang 20160607 機台選用分離機構 需偵測socket sensor
            {
                if(Sen[SnSocketHasClamp1].IsOn() || Sen[SnSocketHasClamp2].IsOn())
                {
                    ShowMyMessage("Please check the socket sensor","socket sensor偵測異常");
                    break;
                }
            }
            Task=200000;
            break;
        case 200000:                                                            //Steven 20160318 : 真空產生器自檢功能
            FTestSuck.ResetAll();
            BTestSuck.ResetAll();
            for(int i=0; i<FTestSuck.iShtRow; i++)                              //JerryYang 20160727 修正沒使用的真空產生器也吸真空
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(INDEX_SUCKER_TYPE==1)                                    //Steven 20111202
                    {
                        fiosetview->bIndexSuck[0][i][j]=true;
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].On();                              //kevin 20110504 check 掉料
                    }
                }
            }

            if(iProcessIndexSuckDestroyCnt==0 ||                                //RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
                iProcessIndexSuckDestroyCnt>15)
            {
                iProcessIndexSuckDestroyCnt=0;
            bIndexCheck1=true;                                                  //kevin 20170120
            Task=300000;
            }
            else
            {
                Task=600000;
            }

            if(TestIF_File.bRENESAS_EnableFTCT==true)                           //RogerYang 20250930 : RogerYang 瑞薩FT-CT 真空產生器自檢功能，一段時間後才重置，避免掉壓
                iProcessIndexSuckDestroyCnt++;
            break;
        case 300000:
            if(INDEX_SUCKER_TYPE==0 || fiosetview->ProcessIndexSuckDestroy1())
            {
                bIndexCheck1=false;                                             //kevin 20170120
                Task=400000;
            }
            break;
        case 400000:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(INDEX_SUCKER_TYPE==1)                                    //Steven 20111202
                    {
                        fiosetview->bIndexSuck[1][i][j]=true;
                    }
                    else
                    {
                        BTestSuck.Suck[i][j].On();                              //kevin 20110504 check 掉料
                    }
                }
            }
            bIndexCheck2=true;                                                  //kevin 20170123 (Steven) 幫忙關閉真空 AUTOCLEAN 流程不會去關
            Task=500000;
        case 500000:
            if(INDEX_SUCKER_TYPE==0 || fiosetview->ProcessIndexSuckDestroy2())
            {
                bIndexCheck2=false;                                             //kevin 20170123 (Steven) 幫忙關閉真空 AUTOCLEAN 流程不會去關
                Task=600000;
            }
            break;
        case 600000:
            flag=CheckIndexArmInitState();
            if(flag==true)
            {
                Task=1;
                break;
            }
            else                                                                //狀態正常
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==NULL_IC)                       //jou 2016-04-29 修正回Home Index drop error
                            FTestSuck.Suck[i][j].Normal();
                        if(BTestSuck.Item[i][j]==NULL_IC)
                            BTestSuck.Suck[i][j].Normal();
                    }
                }
                bIndexCheck1=false;                                             //kevin 20170123 (Steven) 幫忙關閉真空 AUTOCLEAN 流程不會去關
                bIndexCheck2=false;                                             //kevin 20170123 (Steven) 幫忙關閉真空 AUTOCLEAN 流程不會去關

                if(IniConfig.bF16CheckShuttleSensorBroken)                      //Steven 20221213 : 確認shuttle 有沒有斷線
                {
                    bDoingF16=true;
                    Task=10000;
                }
                else
                {
                    Task=11;
                }

                if(IniConfig.bEnableCCDUSETCPIP)
                {
                    if(CCDInterfaceForm->bAtestScanCCDProgram)                  //if(ScanCCDProgram())  kevin 20110811
                    {
                        CCDInterfaceForm->CloseCCDForm();
                        RecordProcess("Stop CCD check.");
                    }
                }
                else
                {
                    if(REAL_TIME_CCD && !COM2->bCCDDummyRum)
                    {
                        COM2->SendCommToVision(COM2->rtLightOn, true);          //jou 2012-03-29 RTC啟動時,自動將燈箱打開
                        DoTestHeadMotorDelay.SetSecAndOn(3);
                    }
                }

                if(CosFunction.bHiSiliconFunction==true)                        //kevin 20200110 add initial
                    bHISIInitiayDelay=true;                                     //kevin 20200110 add 海司強至initial delay

                if(Prod.bEveryFirstDeviceUseInitialDelay)                       //ChungHung 20140425 add for TSMC Device
                {
                    if(bUseInitTempOffset)                                      //Steven 20150707 : Add log for initial offset  //Ifor 20180116 (Steven) : add KYEC 常溫使用 Initial start delay
                    {
                        RecordProcess("After first device trigger initial offset function.");
                    }

                    iInitContactCount=0;                                        //Steven 20141117 : 起測時溫度要補Offset
                    if(CosFunction.bFuncStateStopFirtDelay==true)               //jou 2014-09-03 Function State Stop Firt Initital Delay Time
                    {
                        if(bPiggyBackIndexCheck==false &&
                           LastSet.iRunStartMode!=rsmAutoSiteMap)
                        {
                            bNeedInitialTestDelay=true;
                            Prod.iInitialDelay=TestIF.iInitialDelay;            //ChungHung 20141210 add for SCK want to every event have delay
                        }
                    }
                    else
                    {
                        if(bUseInitDelay)                                       //ChungHung 20141030 add must know Initial start or not //ChungHung 20141027 add 只有加熱模式需要Initial start delay count  //Ifor 20180116 (Steven) : add KYEC 常溫使用 Initial start delay
                        {
//                            if(IniConfig.bSPILFunction && (HasICUnderMachine() || HasAnyICInMachine()))     //JerryYang 20251106 : fix initial start的auto clean預熱秒數短    //JerryYang 20241118 : 矽品明仁 auto clean執行後預熱時間錯誤
//                            {
//                                if(bDoAfterAutoCleanFunctionUseInitialDelay)
//                                {
//                                    bDoEveryFirstDeviceFunctionUseInitialDelay=false;
//                                }
//                                else
//                                {
//                                    bDoEveryFirstDeviceFunctionUseInitialDelay=true;
//                                }
//                            }
//                            else
                            {
                                bDoEveryFirstDeviceFunctionUseInitialDelay=true;   //ChungHung 20140105 add for SCK have order
                            }
                        }
                        else
                        {
                            bDoEveryFirstDeviceFunctionUseInitialDelay=false;   //ChungHung 20140105 add for SCK have order
                        }
                    }
                }

                if(bUseInitDelay)                                               //kevin 20160311 取得測試機間隔時間    //Ifor 20180116 (Steven) : add KYEC 常溫使用 Initial start delay
                {
                    if(Prod.bTestFinishToNextTestOver)
                    {
                        iInitContactCount=0;                                    //Steven 20160519 : 起測時溫度要補Offset
                        bTestFinishToNextTestOver=true;
                        bFirstTest=true;
                        bTestOverTimeTempOffsetF=false;
                    }
                    else
                    {
                        bTestFinishToNextTestOver=false;
                    }

                    if(Prod.bTestStartToNextTestStart)
                    {
                        iInitContactCount=0;                                    //Steven 20160519 : 起測時溫度要補Offset
                        bTestStartToNextTestStart =true;                        //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41
                        bFirstTest=true;
                        bTestOverTimeTempOffsetF=false;
                    }
                    else
                    {
                        bTestStartToNextTestStart =false;                       //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41
                    }
                }
                SendCommand_ESD(ESD_DECAY_TEST);                                //Steven 20140722 : For ESD
            }
            break;
        case 10000:                                                             //Steven 20221213 : 確認shuttle 有沒有斷線
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 10000"))
            {
                bFlag[0]=false;
                bFlag[1]=false;
                CheckShuttleSensorBroken_1(true, false);
                CheckShuttleSensorBroken_2(true, false);
                Task=10010;
            }
            break;
        case 10010:
            if(TestIF_File.iTestMode==SingleSite  || TestIF_File.iTestMode==DualSite    ||  TestIF_File.iTestMode==TriSite1X3   ||     //Steven 20230508 : 針對IC比較大的檢查位置要多動一點
               TestIF_File.iTestMode==DualSite2x1 || TestIF_File.iTestMode==QualSite2X2 ||  TestIF_File.iTestMode==QualSite2X2N ||
               TestIF_File.iTestMode== _6Site2X3  || TestIF_File.iTestMode==_6Site2X3N  ||
               TestIF_File.iTestMode==_8Site2X4N)                               //Wei 20231211 : 2X4NN Mode
            {
                ccRet=1000;
            }
            else
            {
                ccRet=500;
            }

            if(bFlag[0]==false)
                bFlag[0]=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft+ccRet);
            if(bFlag[1]==false)
                bFlag[1]=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft+ccRet);
            if(bFlag[0] && bFlag[1])
            {
                CheckShuttleSensorBroken_1(false, false);
                CheckShuttleSensorBroken_2(false, false);
                bFlag[0]=false;
                bFlag[1]=false;
                DoTestHeadMotorDelay.SetSecAndOn(1);
                Task=10020;
            }
            break;
        case 10020:
            if(DoTestHeadMotorDelay.Off())
            {
                if(bFlag[0]==false)
                    bFlag[0]=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft);
                if(bFlag[1]==false)
                    bFlag[1]=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft);
            }

            if(bFlag[0] && bFlag[1])
            {
                bFlag[0]=false;
                bFlag[1]=false;
                Task=10030;
            }
            break;
        case 10030:
            if(FLCarryKit.HasRealIC()==false)
                bFlag[0]=CheckShuttleSensorBroken_1(false, true);
            if(BLCarryKit.HasRealIC()==false)
                bFlag[1]=CheckShuttleSensorBroken_2(false, true);

            if(bFlag[0] || bFlag[1])
            {
                Task=10000;
            }
            else
            {
                Task=11;
            }
            break;
        case 11:
            bDoingF16=false;                                                    //Steven 20221213 : 確認shuttle 有沒有斷線
            if(CUSTOMER_CODE==CC_SCK &&                                         //ChungHung 20141111 modify for SCK request
               LastSet.iTemperature==Tempture_Hot &&
               iTesterDucking>0)
            {
                return;
            }

            if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)                      //jou 2012-03-29 RTC啟動時,自動將燈箱打開
            {
                if(COM2->bRealTimeCom_ReceiveOK[COM2->rtLightOn]==false)
                {
                    if(IniConfig.bSPILFunction==false)                          //Steven 20140516 : 暫時不關RTC的燈   //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                    {
                        if(DoTestHeadMotorDelay.Off())
                        {
                            if(COM2->OpenRTCComPortAgain())                     //ChungHung 20140520 add 不要第一次TimeOut就秀錯誤訊息
                                ShowMyMessage("RTC Light On Fail!!","RTC光源開啟失敗!!");
                            Task=1;
                        }
                        return;
                    }
                }
            }

            if(bPlaceToShuttleFirst)                                            //是否先放置Shuttle
            {
                Task=20000;
                break;
            }

            iRetryCount=0;
            bReadFrontTestArmTorque=false;
            bReadRearTestArmTorque=false;

            if(CosFunction.bIndexCheckCanTurnOff &&                             //Isaac 20211019 : 可選擇做index check的時機
               ((IniConfig.iD71IndexCheckOnOffMode==0 && bLotStartEndNeedIndexCheck==false) ||
                (IniConfig.iD71IndexCheckOnOffMode==1 && bIndexJamNeedIndexcheck==false) ||
                 IniConfig.iD71IndexCheckOnOffMode==2))
            {
                if(IniConfig.bUseAutoSiteMapping &&
                   CosFunction.bUSEJCETSiteMapMode==false &&
                   LastSet.iRunStartMode==rsmAutoSiteMap &&
                   bSiteMappingCHKOK==false)                                    //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                    Task=150;
                else
                    Task=1500;                                                  //Steven 20231208 : 1550 --> 1500
            }
            else if(CosFunction.bAfterAutoCleanNoIndexCheck &&                  //Steven 20191212 : 劉仁洲說Auto Clean只要作一次Index Check
                    (IniConfig.iD69IndexCheckModeForAutoClean==1 ||
                     IniConfig.iD69IndexCheckModeForAutoClean==2) &&
                    bAutoCleanFinishOnlyUseRTC==true)
            {
                bAutoCleanFinishOnlyUseRTC=false;
                if(IniConfig.bUseAutoSiteMapping &&
                   CosFunction.bUSEJCETSiteMapMode==false &&
                   LastSet.iRunStartMode==rsmAutoSiteMap &&
                   bSiteMappingCHKOK==false)                                    //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                    Task=150;
                else
                    Task=1500;                                                  //Steven 20231208 : 1550 --> 1500
            }
            else if(IniConfig.bEnableCCDUSETCPIP)
            {
                CCDInterfaceForm->CCDTimerOnOff(IniConfig.bC02InstallCCD);      //Steven 20110809
                if(IniConfig.bC02InstallCCD==true)
                {
                    fMain->LightOn();
                    DoTestHeadMotorDelay.SetSecAndOn(3);
                    iCCDTimeOutCount=0;
                    Task=2;
                    break;
                }
                else
                {
                    Task=9;
                    break;
                }
            }
            else
            {
                if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)                  //ChungHung 20121011 鉬FullView 謖IndexCheck
                {
                    COM2->DoReleaseAndInspEnd();
                    Task=iCASE_REAL_CCD2;
                }
                else
                {
                    Task=9;
                }
            }
            break;
        case 2:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 2"))
            {
                Task=3;
            }
            break;
        case 3:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 3"))
            {
                Task=4;
            }
            break;
        case 4:
            if(IniConfig.bC02InstallCCD==false)
            {
                Task=9;
                break;
            }
            fMain->lbCCDStatus->Visible=true;
            if(DoTestHeadMotorDelay.Off()==false)
                break;
            Task=5;
            CCDInterfaceForm->CCDIdentificationOpen();                          //開啟CCD程式
            CCDInterfaceForm->CCDIdentification();                              // kevin 20110811
            RecordProcess("Start CCD check.");
            DoTestHeadMotorDelay.SetSecAndOn(20);                               //CCDTimeOutSec);  20110810 設定5秒太短會一直取像
            DoTestHeadMotorDelay2.SetMSAndOn(200);
            break;
        case 5:
            if(DoTestHeadMotorDelay2.Off()==false ||                            //ChungHung 20121127 add 等待程式開啟
               CCDInterfaceForm->bCCDProgramExistence==false)
            {
                if(DoTestHeadMotorDelay2.Off()==true)
                {
                    DoTestHeadMotorDelay2.SetMSAndOn(200);
                    CCDInterfaceForm->CCDIdentificationOpen();
                    DoTestHeadMotorDelay.SetSecAndOn(20);                       //CCDTimeOutSec);  20110810 設定5秒太短會一直取像
                }
                return;
            }

            if(CCDInterfaceForm->bIdentificationFinish==true)
            {
                if(CCDInterfaceForm->iIdentificationStatus==1)                  //0:未測試 1:Pass 2:Fail
                {
                    CCDInterfaceForm->bIdentificationFinish=false;
                    CCDInterfaceForm->iIdentificationStatus=0;
                    SW[SwCCDLight].Off();
                    Task=9;
                }
                else if(CCDInterfaceForm->iIdentificationStatus==2)
                {
                    RecordProcess("CCD check socket FAIL !!!!!!!!");
                    Task=6;
                }
                else
                {
                    Task=4;
                }
                fShowMessage->FormClick(fShowMessage);
                fMain->lbCCDStatus->Visible=false;
                break;
            }

            if(DoTestHeadMotorDelay.Off())
            {
                iCCDTimeOutCount++;
                CCDInterfaceForm->RetrunUpStep();
                if(iCCDTimeOutCount>(ChangeToFloatNonPcnt((double)(100), (double)(CCDTimeOutSec))))
                {
                    CCDInterfaceForm->CloseCCDForm();
                    RecordProcess("Stop CCD check.");
                    ShowMyMessage("CCD Time out", "CCD判斷時間過長!!");
                    iCCDTimeOutCount=0;
                }
                Task=2;
                break;
            }
            break;
        case 6:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 6"))
            {
                Task=7;
            }
            break;
        case 7:
            if(IndexAlarmInArmAway()==false)                                    //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                return;
            }
            fMain->Pause("DoTestHeadMotor");
            CCDInterfaceForm->CCDIdentification();                              // kevin 20110811
            CCDInterfaceForm->bCCDProgramExistence=true;
            Task=1;
            break;
        case 9:                                                                 //IndexZ1 and IndexZ2 皆移至安全位置
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 9"))
            {
                if(TestIF_File.iTestType==TTL_MODE &&
                   (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))                      //Isaac 20210309 :TTL RS232通訊
                    SendTTLRS232CSOTsignal();

                CCDInterfaceForm->CCDTimerOnOff(false);                         //Steven 20110809
                iCCDTimeOutCount=0;
                Task=10;

                if(IniConfig.bD24EnableEPCheckFuntion==true &&
                   IniConfig.bIndexEveryTimeCheckEP==true)
                {
                    if(CUSTOMER_CODE!=CC_VTEST_Shanghai)// &&                   //jou 20210911 : 上海無錫 張冬冬 index check 不檢查EP
//                       CUSTOMER_CODE!=CC_SCC)                                 //Steven 20240926 : JSCC 李小川 index check 不檢查EP
                    {
                        InitIndexEveryTimeCheckEP();
                        Task=30000;
                    }
                }
                else
                {
                    bIndexEveryTimeCheckEPing=false;
                }
            }
            break;
        case 10:                                                                //判斷IndexZ1 and IndexZ2 是否已在安全位置
            if(MOT[MTestZ1].ISNormal()==false)
            {
                iRetryCount++;
                if(iRetryCount>3)
                {
                    ShowIndexMotorError(AnsiString("DoTestHeadMotor10"));
                    iRetryCount=0;
                }
                else
                {
                    Task=9;
                }
                return ;
            }
            Task=15;
            break;
        case 15:                                                                //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 15"))
            {
                Task=20;
            }
            break;
        case 20:
            if(IniConfig.bC08_SocketSensor && TestIF_File.bEnSocketSensor &&    //kevin 20160209 //kevin 20130504 socket sensor detect error
               (TestIF_File.bSocketDisibleinitialcheck==false ||                //JerryYang 20170426 (Steven) 修正Disable socket sensor detect
                CUSTOMER_CODE==CC_GIGAS))                                       //Isaac 20220126 : 全智要求Index arm在上要強制偵測sensor(應該為off)
            {
                str="check socket sensor state must off, sensor: ";
                for(int i=0; i<TestIF_File.iSocketCount; i++)
                {
                    if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                       Sen[SThreadPara.iSocketSensor[i]].IsOn())                //kevin 20150429 : Arm在上, 檢查無遮斷, 要Off
                    {
                        flag=true;
                        bIsSocketSensor=true;
                        str+=IntToStr(i+1);
                    }
                }

                if(flag)
                {
                    ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, str);  //kevin 20130504 socket sensor
                    return;                                                     //kevin 20150429確認 socket sensor是否正常
                }
            }
            Task=21;
            break;
        case 21:                                                                //Index1 or Index2 是否需要拋下IC
//            bContSoftEpSwitch(0, true);                                         //ARM1 先浮動頭充氣  //kevin 20240816
//            bContSoftEpSwitch(1, true);                                         //ARM2 先浮動頭充氣  //kevin 20240816
            EPSwitchOnOff(eEPSwBoth);                                           //Steven 20250417 : fixed for [D58]

            if(bFTestSuckDrop==true)
            {
                Task=30;
            }
            else if(bBTestSuckDrop==true)
            {
                Task=60;
            }
            else
            {
                if(CosFunction.bSortingBy2DList==true &&
                   LastSet.iTester==_2D_SORT &&
                   TestIF_File.bSortingBy2DIDList==true)                        //Frank 20221122 : 2DID sorting for ATK
                    Task=115;
                else
                    Task=100;
            }
            break;
        case 30:                                                                //Index1移至中間 Index2移至後面
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 30"))
                Task=40;
            break;
        case 40:                                                                //移至下拋高度
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Down_Z2Up;
                Task=50;
            }
            break;
        case 50:                                                                //拋下IC
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(FTestSuck.Item[i][j]==HAS_IC)
                    {
                        FTestSuck.Suck[i][j].On();
                        if(INDEX_SUCKER_TYPE==1)                                //Steven 20111202
                        {
                            fiosetview->bIndexSuck[0][i][j]=true;
                        }
                    }
                    DoTestHeadMotorDelay.SetSecAndOn(0.3);                      // delay 0.3 sec for ic down
                }
            }
            Task=55;
            break;
        case 55:
            if(DoTestHeadMotorDelay.Off())
            {
                bFTestSuckDrop=false;
                Task=1;
            }
            break;
        case 60:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 60"))
                Task=70;
            break;
        case 70:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Up_Z2Down;
                Task=80;
            }
            break;
        case 80:
            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(BTestSuck.Item[i][j]==HAS_IC)
                    {
                        BTestSuck.Suck[i][j].On();
                        if(INDEX_SUCKER_TYPE==1)                                //Steven 20111202
                        {
                            fiosetview->bIndexSuck[1][i][j]=true;
                        }
                    }
                    DoTestHeadMotorDelay.SetMSAndOn(100);                       // delay 0.3 sec for ic down
                }
            }
            Task=90;
            break;
        case 90:
            if(DoTestHeadMotorDelay.Off())
            {
                bBTestSuckDrop=false;
                Task=1;
            }
            break;
        case 100:                                                               //Index2 上IC是否有掉落
            if(bRearHeadICFallDown)
            {
                Task=110;
                break;
            }                                                                   //Index1 移至中間 Index2 移至後面

            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 100"))
            {
                Task=120;
            }
            break;
        case 110:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 110"))
                Task=112;
            break;
        case 112:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Up_Z2Down;
                Task=141;
            }
            break;
        case 115:                                                               //JerryYang 20230322 : 2D SORT模式index arm不用下壓到socket
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 115"))
                Task=116;
            break;
        case 116:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ1_Safe, iSpeedSlow))
            {
                IndexStatus=Z1_Z2_Normal;
                Task=120;
            }
            break;
        case 120:                                                               //初始化馬達扭力及確認
            InitWriteAndCheckMotorTorqueTask();
            fMain->lbArm0Torque->Caption="1:Writing";
            fMain->lbArm1Torque->Caption="---";
            Task=12000;
        case 12000:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            #ifdef SOFT_SIMULTE
                ret=1;
            #else
                ret=COM2->iWriteAndCheckMotorTorque(0, Prod.iMaxPreasure);
            #endif

            if(ret==1)                                                          //扭力設定OK
            {
                iToqureCount=0;
                Task=12100;
            }
            else if(ret==2)                                                     //扭力設定NG
            {
                ShowMyMessage("Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 12000");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 12100:
            Task=12101;
        case 12101:
            iIndexUpPos=0;

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12101"))
                {
                    IndexStatus=Z1_Z2_Normal;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(0);
                        Task=12300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=12102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                        }
                        else
                        {
                            Task=12110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);                //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                            bFirstTime=true;
                        }
                    #endif
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedSlow)) //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Down_Z2Up;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(0);
                        Task=12300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=12102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                            bFirstTime=true;
                        }
                        else
                        {
                            Task=12110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);                //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                            bFirstTime=true;
                        }
                    #endif
                    ReadTorqueDelay.SetSecAndOn(10);                            //kevin 20210824 read torque wait alarm time
                }
            }
            break;
        case 12102:                                                             //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
            if(DoTestHeadMotorDelay.Off())
            {
                Task=12110;
                DoTestHeadMotorDelay.SetSecAndOn(1);
                bFirstTime=true;
            }
            break;
        case 12110:
            fMain->lbArm0Torque->Caption="1:Reading";

            if(DoTestHeadMotorDelay.Off()==false)
            {
                if(bFirstTime)
                {
                    bFirstTime=false;
                    fMain->chkReadTorque1->Checked=true;
                    fMain->chkReadTorque2->Checked=false;
                    fMain->edTorue0->Text="";
                    COM2->InitReadTorueTask();
                    return;                                                     //kevin 20210824 clean Tourqe
                }
            }

            if(fMain->edTorue0->Text=="")
            {
                if(ReadTorqueDelay.Off())                                       //kevin 20210824 read torque wait alarm time
                {
                    if(COM2->GetReadTorueTask()==999)                           //jou 2011-11-29防止Read Torue後數值被清掉，還傻傻的在那邊等
                    {
                        ShowMyMessage("The test head 1 Motor torque read error", "馬達扭力讀取錯誤", "DoTestHeadMotor 12110");
                        fAllMotorHome=false;
                        Task=1;
                    }
                }
                return;
            }
            else
            {
                 ShowMainScreenPresure(0);                                      //kevin 20210413
            }

            if(fMain->lbArm0Torque->Caption=="1:Reading")                       //kevin 20210413
            {
                InitWriteAndCheckMotorTorqueTask();
                Task=120;
                RecordProcess("Arm1 Torque Reading.");
                return;
            }

            if(IniConfig.bD30EnableSiteModeSelect &&
               (TestIF.iShuttleMode==0 ||
               (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)))             //KEVIN 20150613
            {
                if(IniConfig.bC08_SocketSensor &&
                   TestIF_File.bEnSocketSensor &&
                   TestIF_File.bSocketDisibleinitialcheck==false)               //JerryYang 20170426 (Stven) 修正Disable socket sensor detect
                {
                    str="index check socket sensor down sensor must on, ";
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(IsNNMode()==NN_2Row)
                        {
                            if(i>=2 &&
                               Sen[SThreadPara.iSocketSensor[i]].Enable &&
                               Sen[SThreadPara.iSocketSensor[i]].IsOff())       //kevin 20150429 : Arm 1在下, 檢查遮斷, 要On
                            {
                                flag=true;
                                bIsSocketSensor=true;
                                str+=IntToStr(i+1);
                            }
                        }
                        else
                        {
//                            if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&     //Jimmychiu 20250809 : Mark for Gigas反應檢測功能失效
//                               TestIF_File.bArm1PickPlaceArm2Test==true)
//                            {
                                if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //kevin 20150429 : Arm 1在下, 檢查遮斷, 要On
                                {
                                    flag=true;
                                    bIsSocketSensor=true;
                                    str+=IntToStr(i+1);
                                }
//                            }
//                            else
//                            {
//                                if(TestIF_File.iSensorCheckType[i]==2 &&
//                                   Sen[SThreadPara.iSocketSensor[i]].Enable &&
//                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //Arm 1在下, 手臂上沒IC, 只檢查floating sensor必須遮到
//                                {
//                                    flag=true;
//                                    bIsSocketSensor=true;
//                                    str+=IntToStr(i+1);
//                                }
//                            }
                        }
                    }

                    if(flag)
                    {
                        ErrPart=str;                                            //Steven 20220721 : Alarm之前, Index要先讓開
                        Task=12150;
//                        ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, str);//kevin 20130504 socket sensor
                        return;                                                 //kevin 20150429確認 socket sensor是否正常
                    }
                }
            }
            TorqueData=atoi(fMain->edTorue0->Text.c_str());
            TorqueData=abs(TorqueData);

            if(IniConfig.bControlTorque)                                        //jou 2013-11-05 Index Control Torque
                iMaxPreasure=DeviceForm.iIndexTorqueMax;
            else
                iMaxPreasure=Prod.iMaxPreasure;

            if(CUSTOMER_CODE==CC_KYEC_XILINX &&
               IniConfig.bD01EnableReadTorque &&                                //Frank 20171030 (Steven) add Floting Shuttle Read Torque Delay
               IniConfig.bChangeKitNoHardStop==true &&
               IniConfig.bRemeberAutoHeight==true)
            {
                if(TorqueData>=(IniConfig.dD01ReadTorque+DeviceForm_File.dZ1Torue))
                {
                    ShowMyMessage("The test head 1, contact force over error", "Index 1 壓力過重錯誤", "DoTestHeadMotor 14110");
                    Task=12111;                                                 //kevin 20130418
                    return;
                }
            }
            else
            {
                if(TorqueData>=iMaxPreasure)
                {
                    if(iToqureCount>10)                                         //jou 981130 double check Torque
                    {
                        iToqureCount=0;
                        ShowMyMessage("The test head 1, contact force over error", "Index 1 壓力過重錯誤", "DoTestHeadMotor 12110");
                        Task=12111;                                             //kevin 20130418
                        return ;
                    }
                    else
                    {
                        iToqureCount++;
                        Task=12100;
                        break;
                    }
                }
            }

            ShowMainScreenPresure(0);
            InitWriteAndCheckMotorTorqueTask();

            if(IniConfig.bVTESTFunction==true)                                  //jou 20231102 : VTEST 增加 Torque log
                CheckAndRecodrTorque(0);

            Task=12200;
            break;
        case 12111:                                                             //kevin 20130418
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12111"))
                Task=12112;
            break;
        case 12112:
            ErrPart="The test head 1, contact force over error";                //kevin 20130418
            bIsContactforce=true;                                               //kevin 20130418 contact force over 需開們確認
            if(CosFunction.bIndexAreaOnlyCanUseSkip)                            //Steven 20141105 : Index內的所有異常都只能用Skip
                ShowErrorMessage("WAR0321", K_SKIP, MTestZ1, false, ErrPart);
            else
                ShowErrorMessage("WAR0321", K_RETRY, MTestZ1, false, ErrPart);
            Task=9;
            break;
        case 12150:                                                             //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12150"))
            {
                Task=12151;
            }
            break;
        case 12151:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY))
            {
                Task=12152;
            }
            break;
        case 12152:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, ErrPart);  //kevin 20130504 socket sensor
                Task=9;
            }
            break;
        case 12200:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            ret=COM2->iWriteAndCheckMotorTorque(0, 300);
            if(ret==1)
            {
                fMain->chkReadTorque1->Checked=false;
                fMain->chkReadTorque2->Checked=false;
                Task=12300;
                iSiteCount=0;                                                   //kevin 20190530
            }
            else if(ret==2)
            {
                ShowMyMessage("The test head 1 Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 12200");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 12300:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190709 add 20190629 回吸檢測一次 4 個 SITE
            {
                 Task=12301;
            }
            else if(LastSet.bD41TestSocketICCheckSkip==false)
            {
                if(IniConfig.bSPILFunction==true)                               //jou 2014-08-18 SPIL 關arm 不檢查真空值 //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==0))
                        Task=121;
                    else
                        Task=130;
                }
                else
                {
                    if(CosFunction.bUseOneByOneIndexCheck==true &&              //Ifor 20180322 : add Use One By One Index Check
                       INDEX_SUCKER_TYPE==1)
                    {
                        if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==true) ||
                           (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==false))            //Above Socket
                        {
                            FTestSuck.ResetAll();
                            iSiteCount=0;
                            IndexCheckOneByOne(true, 0, iSiteCount);            //Ifor 20200617 : add Use One By One Index Check Function 整合
                            Task=50000;
                        }
                        else
                        {
                            Task=121;
                        }
                    }
                    else
                    {
                        Task=121;
                    }
                }
            }
            else
            {
                Task=130;
            }
            break;
        case 12301:
            if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==true) ||
               (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==false))                        //Above Socket
            {
                iIndexUpPos=0;
                if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                    IniConfig.bTestIcCheckInContact==true) ||                   //kevin 20190708 add
                    (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                     IniConfig.bTestIcCheckInContact==false))                   //Above Socket
                {
                    iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);            //kevin yang 20201014 : 2 --> 1
                }

                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedSlow))    //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Down_Z2Up;
                    if(ArmSpeed_File[IndexArm].bDevicConfirm &&                 //kevin 20190629 回吸檢測一次 4 個 SITE
                       INDEX_SUCKER_TYPE==1)                                    //kevin 20190530 add index check
                    {
                        IndexCheck4Site(true, 0, iSiteCount);
                        Task=12305;
                    }
                    else
                    {
                        Task=121;
                    }
                }
                break;
            }
            else
            {
                if(IniConfig.bSPILFunction==true)                               //jou 2014-08-18 SPIL 關arm 不檢查真空值 //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==0))
                        Task=121;
                    else
                        Task=130;
                }
                else
                {
                    Task=12302;
                }
            }
            break;
        case 12302:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&                         //kevin 20190629 回吸檢測一次 4 個 SITE
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190530 add index check
            {
                IndexCheck4Site(true,0,iSiteCount);
                Task=12305;
            }
            else
            {
                Task=121;
            }
            break;
        case 12305:                                                             //kevin 20190531 index check 4 Site
            if(IndexCheck4Site(false, 0, iSiteCount))
            {
                iSiteCount++;
                if(iSiteCount<4)
                {
                    Task=12301;
                }
                else
                {
                    Task=122100;
                    iSiteCount=0;
                }
            }
            break;
        case 121:
            if(CosFunction.bUseOneByOneIndexCheck==true &&
               INDEX_SUCKER_TYPE==1)                                            //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                FTestSuck.ResetAll();
                iSiteCount=0;
                IndexCheckOneByOne(true, 0, iSiteCount);                        //Ifor 20200617 : add Use One By One Index Check Function 整合
                Task=80000;
                break;
            }

            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[0][i][j]==true)
                            {
                                fiosetview->bIndexSuck[0][i][j]=true;
                                bIndexCheckNoStopVaccum=true;
                            }
                        }
                        else
                        {
                            fiosetview->bIndexSuck[0][i][j]=true;
                            bIndexCheckNoStopVaccum=true;
                        }
                    }
                    else
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[0][i][j]==true)
                            {
                                FTestSuck.Suck[i][j].On();
                            }
                        }
                        else
                        {
                            FTestSuck.Suck[i][j].On();
                        }
                    }
                }
            }

            DoTestHeadMotorDelay.SetSecAndOn(0.5);
            Task=122100;
            break;
        case 122100:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                fiosetview->ProcessIndexSuckDestroy1();
            }

            if(DoTestHeadMotorDelay.Off())
            {
                Task=122110;
            }
            break;
        case 122110:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                fiosetview->ProcessIndexSuckDestroy1();
            }

            iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);                    //Steven 20140620 : 整合為Function
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12101"))
                {
                    IndexStatus=Z1_Z2_Normal;
                    if(CUSTOMER_CODE==CC_Greatek)                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                  //Steven 20110908 : 上來後也要Delay一下

                   if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                      TotalErrPart!="")                                         //kevin 20190629 add error show
                        Task=123;
                    else
                        Task=122;
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedFast))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    if(CUSTOMER_CODE==CC_Greatek)                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                  //Steven 20110908 : 上來後也要Delay一下

                    if(CosFunction.bUseOneByOneIndexCheck==true &&
                       ArmSpeed_File[IndexArm].bDevicConfirm &&
                       TotalErrPart!="")                                        //kevin 20190629 add error show
                        Task=123;
                    else if(CosFunction.bUseOneByOneIndexCheck==true &&
                            TotalErrPart!="")                                   //KaiChen 20210104:
                        Task=123;
                    else
                        Task=122;
                }
            }
            break;
        case 122:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bIndexSuckCheck=false;
                bIndexSuckCheck=fiosetview->ProcessIndexSuckDestroy1();
            }
            else
            {
                bIndexSuckCheck=true;
            }

            if(DoTestHeadMotorDelay.Off() && bIndexSuckCheck==true)
            {
                bIndexCheckNoStopVaccum=false;                                  //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
                flag=false;
                ErrPart=" ";
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        flag2=false;
                        FTestSuck.CheckVaccumIsIniaialON(i, j, flag2);
                        if(flag2==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i+IsNNMode()][j];            //Steven 20230712 : 修正NN mode alarm顯示

                            if(CUSTOMER_CODE==CC_SCS)                           //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                                FTestSuck.Suck[i][j].Off();
                        }
                    }
                }

                if(flag && LastSet.iRealDummy==REALLY)                          //Steven 20120726 : 有跑IC才檢查Socket
                    Task=123;
                else
                    Task=130;
            }
            break;
        case 123:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 123"))
            {
                if(CUSTOMER_CODE==CC_SCS)                                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                {
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                Task=124;
            }
            break;
        case 124:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 124"))
                Task=125;
            break;
        case 125:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bIsTestSitICFallDown=true;                                      //Steven 20130613
                if(CosFunction.bUseOneByOneIndexCheck==true &&
                   ArmSpeed_File[IndexArm].bDevicConfirm)                       //kevin 20190629 add error show
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, TotalErrPart);
                }
                else if(CosFunction.bUseOneByOneIndexCheck==true &&
                        INDEX_SUCKER_TYPE==1 &&
                        TotalErrPart!="")                                       //KaiChen 20210104:
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, TotalErrPart);
                }
                else if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)       //Steven 20130604 : Socket殘料要按Z1
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, ErrPart);
                }
                else
                {
                    ShowMyMessage("Arm1 detect Test Socket has IC error", "Arm 1偵測到Socket有IC殘留!!", "DoTestHeadMotor 125");
                }
                ErrPart="";
                TotalErrPart="";
                Task=1;
            }
            break;
        case 130:
            Task=15000;
            break;
        case 135:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 135"))
                Task=140;
            break;
        case 140:
            InitWriteAndCheckMotorTorqueTask();
            fMain->lbArm1Torque->Caption="2:Writing";
            Task=14000;
        case 14000:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            #ifdef SOFT_SIMULTE
                ret=1;
            #else
                ret=COM2->iWriteAndCheckMotorTorque(1, Prod.iMaxPreasure);
            #endif

            if(ret==1)
            {
                iToqureCount=0;
                Task=14100;
            }
            else if(ret==2)
            {
                ShowMyMessage("The test head 2 Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 14000");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 14100:
            Task=14101;
        case 14101:
            iIndexUpPos=0;

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ1_Safe, iSpeedSlow))
                {
                    IndexStatus=Z1_Z2_Normal;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(1);
                        Task=14300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&                    //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=14102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                        }
                        else
                        {
                            Task=14110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);
                            bFirstTime=true;
                        }
                    #endif
                }
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedSlow))   //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Up_Z2Down;
                    #ifdef SOFT_SIMULTE
                        ShowMainScreenPresure(1);
                        Task=14300;
                    #else
                        if(CUSTOMER_CODE==CC_KYEC_XILINX &&
                           IniConfig.bD01EnableReadTorque &&                    //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
                           IniConfig.bChangeKitNoHardStop==true &&
                           IniConfig.bRemeberAutoHeight==true)
                        {
                            Task=14102;
                            DoTestHeadMotorDelay.SetSecAndOn(IniConfig.dD01ReadTorqueDelayTime);
                        }
                        else
                        {
                            Task=14110;
                            DoTestHeadMotorDelay.SetSecAndOn(1);
                            bFirstTime=true;
                        }
                    #endif
                    ReadTorqueDelay.SetSecAndOn(10);                            //kevin 20210824 read torque wait alarm time
                }
            }
            break;
        case 14102:                                                             //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
            if(DoTestHeadMotorDelay.Off())
            {
                Task=14110;
                DoTestHeadMotorDelay.SetSecAndOn(1);
                bFirstTime=true;
            }
            break;
        case 14110:
            fMain->lbArm1Torque->Caption="2:Reading";

            if(DoTestHeadMotorDelay.Off()==false)
            {
                if(bFirstTime)
                {
                    bFirstTime=false;
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=true;
                    fMain->edTorue1->Text="";
                    COM2->InitReadTorueTask();
                    bReadMCU2=true;                                             //kevin 20220225 read MCU DATA
                    return;                                                     //kevin 20210824 clean Tourqe
                }
            }

            if(fMain->edTorue1->Text=="")                                       //jou 2011-11-29防止Read Torue後數值被清掉，還傻傻的在那邊等
            {
                if(ReadTorqueDelay.Off())                                       //kevin 20210824 read torque wait alarm time
                {
                    if(COM2->GetReadTorueTask()==999)
                    {
                        ShowMyMessage("The test head 2 Motor torque read error", "馬達扭力讀取錯誤", "DoTestHeadMotor 14110");
                        fAllMotorHome=false;
                        Task=1;
                    }
                }
                return;
            }
            else
            {
                 ShowMainScreenPresure(1);                                      //kevin 20210413
            }

            if(fMain->lbArm1Torque->Caption=="2:Reading")                       //kevin 20210413
            {
                InitWriteAndCheckMotorTorqueTask();
                Task=140;
                RecordProcess("Arm2 Torque Reading.");
                 return;
            }
            //Rear Arm Only
            if(IniConfig.bD30EnableSiteModeSelect &&
               (TestIF.iShuttleMode==0 ||
               (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)))             //KEVIN 20150613
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                   TestIF_File.bArm1PickPlaceArm2Test==true)                    //Steven 20200604 : Arm1丟 Arm2測的時候, 只要檢查Arm1
                {

                }
                else if(IniConfig.bC08_SocketSensor &&
                        TestIF_File.bEnSocketSensor &&
                        TestIF_File.bSocketDisibleinitialcheck==false)          //JerryYang 20170426 (Steven) 修正Disable socket sensor detect
                {
                    str="check socket sensor down off,";
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(IsNNMode()==NN_2Row)
                        {
                            if(i<=1 &&
                               Sen[SThreadPara.iSocketSensor[i]].Enable &&
                               Sen[SThreadPara.iSocketSensor[i]].IsOff())       //kevin 20150429 : Arm 2在下, 檢查遮斷, 要On
                            {
                                flag=true;
                                bIsSocketSensor=true;
                                str+=IntToStr(i+1);
                            }
                        }
                        else
                        {
//                            if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&        //Jimmychiu 20250809 : Mark for Gigas反應檢測功能失效
//                               TestIF_File.bArm1PickPlaceArm2Test==true)
//                            {
                                if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //kevin 20150429 : Arm 2在下, 檢查遮斷, 要On
                                {
                                    flag=true;
                                    bIsSocketSensor=true;
                                    str+=IntToStr(i+1);
                                }
//                            }
//                            else
//                            {
//                                if(TestIF_File.iSensorCheckType[i]==2 &&
//                                   Sen[SThreadPara.iSocketSensor[i]].Enable &&
//                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())   //Arm 2在下, 手臂上沒IC, 只檢查floating sensor必須遮到
//                                {
//                                    flag=true;
//                                    bIsSocketSensor=true;
//                                    str+=IntToStr(i+1);
//                                }
//                            }
                        }
                    }
                    if(flag)
                    {
                        ErrPart=str;                                            //Steven 20220721 : Alarm之前, Index要先讓開
                        Task=14150;
//                        ShowErrorMessage("WAR0322", K_RETRY, MTestZ2, false, str);//kevin 20130504 socket sensor
                        return;                                                 //kevin 20150429確認 socket sensor是否正常
                    }
                }
            }
            TorqueData=atoi(fMain->edTorue1->Text.c_str());
            TorqueData=abs(TorqueData);

            if(IniConfig.bControlTorque)                                        //jou 2013-11-05 Index Control Torque
                iMaxPreasure=DeviceForm.iIndexTorqueMax;
            else
                iMaxPreasure=Prod.iMaxPreasure;

            if(CUSTOMER_CODE==CC_KYEC_XILINX &&
               IniConfig.bD01EnableReadTorque &&                                //Frank 20171030 (Steven) add Floting Shuttle Read Torque Delay
               IniConfig.bChangeKitNoHardStop==true &&
               IniConfig.bRemeberAutoHeight==true)
            {
                if(TorqueData>=(IniConfig.dD01ReadTorque+DeviceForm_File.dZ2Torue))
                {
                    ShowMyMessage("The test head 2, contact force over error", "Index 2 壓力過重錯誤", "DoTestHeadMotor 14110");
                    Task=14111;                                                 //kevin 20130418
                    return;
                }
            }
            else
            {
                if(TorqueData>=iMaxPreasure)
                {
                    if(iToqureCount>10)                                         //jou 981130 double check Torque
                    {
                        iToqureCount=0;
                        ShowMyMessage("The test head 2, contact force over error", "Index 2 壓力過重錯誤", "DoTestHeadMotor 14110");
                        Task=14111;                                             //kevin 20130418
                        return;
                    }
                    else
                    {
                        iToqureCount++;
                        Task=14100;
                        break;
                    }
                }
            }

            if(IniConfig.bControlTorque)                                        //jou 2013-11-05 Index Control Torque
            {
                iIndexArm[0]=atoi(fMain->edTorue0->Text.c_str());
                iIndexArm[1]=atoi(fMain->edTorue1->Text.c_str());
                iIndexArm[2]=abs(iIndexArm[0]-iIndexArm[1]);
                if(TestIF_File.iShuttleMode==0 &&                               //Steven 20240716 : 關arm的時候不檢查兩arm的壓力差
                   iIndexArm[2]>DeviceForm.iIndexTorqueCmp)
                {
                    ShowMyMessage("The test head contact force% difference over error", "Index 壓力%差距過大錯誤", "DoTestHeadMotor 14110");
                }
            }

            ShowMainScreenPresure(1);
            InitWriteAndCheckMotorTorqueTask();

            if(IniConfig.bVTESTFunction==true)                                  //jou 20231102 : VTEST 增加 Torque log
                CheckAndRecodrTorque(1);

            Task=14200;
            break;
        case 14111:                                                             //kevin 20130418
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeedSlow, "DoTestHeadMotor 14111"))
                Task++;
            break;
        case 14112:
            ErrPart="The test head 2, contact force over error";//kevin 20130418
            bIsContactforce=true;                                               //kevin 20130418 contact force over 需開們確認

            if(CosFunction.bIndexAreaOnlyCanUseSkip)                            //Steven 20141105 : Index內的所有異常都只能用Skip
                ShowErrorMessage("WAR0321", K_SKIP, MTestZ1, false, ErrPart);
            else
                ShowErrorMessage("WAR0321", K_RETRY, MTestZ2, false, ErrPart);

            Task=135;
            break;
        case 14150:                                                             //Steven 20220721 : Alarm之前, Index要先讓開
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeedSlow, "DoTestHeadMotor 14150"))
            {
                Task=14151;
            }
            break;
        case 14151:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY))
            {
                Task=14152;
            }
            break;
        case 14152:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                ShowErrorMessage("WAR0322", K_RETRY, MTestZ2, false, ErrPart);  //kevin 20130504 socket sensor
                Task=9;
            }
            break;
        case 14200:
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            ret=COM2->iWriteAndCheckMotorTorque(1, 300);
            if(ret==1)
            {
                fMain->chkReadTorque1->Checked=false;
                fMain->chkReadTorque2->Checked=false;
                Task=14300;
            }
            else if(ret==2)
            {
                ShowMyMessage("Motor torque set error", "馬達扭力設定錯誤", "DoTestHeadMotor 14200");
                fAllMotorHome=false;
                Task=1;
                return ;
            }
            break;
        case 14300:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&                         //kevin 20190629 回吸檢測一次 4 個 SITE
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 1, iSiteCount);
                Task=14301;
            }
            else if(LastSet.bD41TestSocketICCheckSkip==false)                   //jou 2014-08-18 SPIL 關arm 不檢查真空值
            {
                if(IniConfig.bSPILFunction==true)                               //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||
                       (TestIF_File.iShuttleMode==1 &&
                        TestIF_File.iShuttle_Sel==1))
                        Task=141;
                    else
                        Task=150;
                }
                else
                {
                    if(CosFunction.bUseOneByOneIndexCheck==true &&              //Ifor 20180322 : add Use One By One Index Check
                       INDEX_SUCKER_TYPE==1)
                    {
                        if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==true) ||
                           (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                            IniConfig.bTestIcCheckInContact==false))            //Above Socket
                        {
                            BTestSuck.ResetAll();
                            iSiteCount=0;
                            IndexCheckOneByOne(true, 1, iSiteCount);            //Ifor 20200617 : add Use One By One Index Check Function 整合
                            Task=60000;
                        }
                        else
                        {
                            Task=141;
                        }
                    }
                    else if(ArmSpeed_File[IndexArm].bDevicConfirm &&            //kevin 20190629 回吸檢測一次 4 個 SITE
                            INDEX_SUCKER_TYPE==1)                               //kevin 20190530 add index check
                    {
                        IndexCheck4Site(true,1,iSiteCount);
                        Task=14305;
                    }
                    else
                    {
                        Task=141;
                    }
                }
            }
            else
            {
                Task=150;
            }

            if(IniConfig.bIndexArm2SupplyLight==true ||                         //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
               TestIF_File.bForEgisTecTest==true     ||                         //Steven 20140922 : Arm2當作指紋測試
               (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                 //kevin 20150127 Arm1 下壓 arm2 測試
                TestIF_File.bArm1PickPlaceArm2Test==true))                      //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                Task=150;
            }
            break;
        case 14301:
            if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==true) ||
               (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                IniConfig.bTestIcCheckInContact==false))                        //Above Socket
            {
                iIndexUpPos=0;
                if((DeviceForm.iSocketInitialICCheckPosition==1 &&
                    IniConfig.bTestIcCheckInContact==true) ||                   //kevin 20190708 add
                    (IniConfig.iD41SocketInitialICCheckPosition==1 &&
                     IniConfig.bTestIcCheckInContact==false))                   //Above Socket
                {
                    iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);
                }
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedSlow))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Down_Z2Up;
                    if(ArmSpeed_File[IndexArm].bDevicConfirm &&                 //kevin 20190629 回吸檢測一次 4 個 SITE
                       INDEX_SUCKER_TYPE==1)                                    //kevin 20190530 add index check
                    {
                        IndexCheck4Site(true,0,iSiteCount);
                        Task=14305;
                    }
                    else
                    {
                        Task=141;
                    }
                }
                break;
            }
            else
            {
                if(IniConfig.bSPILFunction==true)                               //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                {
                    if(TestIF_File.iShuttleMode==0 ||                           //jou 2014-08-18 SPIL 關arm 不檢查真空值
                       (TestIF_File.iShuttleMode==1  &&
                        TestIF_File.iShuttle_Sel==0))
                        Task=141;
                    else
                        Task=150;
                }
                else
                {
                    Task=14302;
                }
            }
            break;
        case 14302:
            if(ArmSpeed_File[IndexArm].bDevicConfirm &&                         //kevin 20190629 回吸檢測一次 4 個 SITE
               INDEX_SUCKER_TYPE==1)                                            //kevin 20190530 add index check
            {
                IndexCheck4Site(true, 0, iSiteCount);
                Task=14305;
            }
            else
            {
                Task=141;
            }
            break;
        case 14305:                                                             //kevin 20190531 index check 4 Site
            if(IndexCheck4Site(false, 1, iSiteCount))
            {
                iSiteCount++;
                if(iSiteCount<4)
                    Task=14301;
                else
                    Task=142100;
            }
            break;
        case 141:
            if((IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                TestIF_File.bArm1PickPlaceArm2Test==true) &&                    //Steven 20150129 : 需要確認Arm2有沒有粘料
                TestIF_File.bCheckArm2Vacuum==false)                            //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                Task=150;
                break;
            }

            if(CosFunction.bUseOneByOneIndexCheck==true &&
               INDEX_SUCKER_TYPE==1)                                            //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                BTestSuck.ResetAll();
                iSiteCount=0;
                IndexCheckOneByOne(true, 1, iSiteCount);                        //Ifor 20200617 : add Use One By One Index Check Function 整合
                Task=80010;
                break;
            }

            for(int i=0; i<BTestSuck.iShtRow; i++)
            {
                for(int j=0; j<BTestSuck.iShtCol; j++)
                {
                    if(INDEX_SUCKER_TYPE==1)
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[1][i][j]==true)
                            {
                                fiosetview->bIndexSuck[1][i][j]=true;
                                bIndexCheckNoStopVaccum=true;
                            }
                        }
                        else
                        {
                            fiosetview->bIndexSuck[1][i][j]=true;
                            bIndexCheckNoStopVaccum=true;
                        }
                    }
                    else
                    {
                        if(CosFunction.bInitTestHeadByTestSiteUse)              //JerryYang 20151016 : TestSuck檢查 關Site時就不開真空偵測
                        {
                            if(bTestSiteUse[1][i][j]==true)
                            {
                                BTestSuck.Suck[i][j].On();
                            }
                        }
                        else
                        {
                            BTestSuck.Suck[i][j].On();
                        }
                    }
                }
            }

            DoTestHeadMotorDelay.SetSecAndOn(0.5);
            Task=142100;
            break;
        case 142100:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                fiosetview->ProcessIndexSuckDestroy2();
            }

            if(DoTestHeadMotorDelay.Off())
                Task=142110;
            break;
        case 142110:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            {
                fiosetview->ProcessIndexSuckDestroy2();
            }

            iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                    //Steven 20140620 : 整合為Function
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 12101"))
                {
                    IndexStatus=Z1_Z2_Normal;
                    if(CUSTOMER_CODE==CC_Greatek)                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                  //Steven 20110908 : 上來後也要Delay一下

                    if(ArmSpeed_File[IndexArm].bDevicConfirm &&
                       TotalErrPart!="")                                        //kevin 20190629 add error show
                        Task=143;
                    else
                        Task=142;
                }
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedFast))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
                {
                    IndexStatus=Z1Up_Z2Down;                                    //kevin 20190118 add index z states
                    if(CUSTOMER_CODE==CC_Greatek)                               //Wei 20160413
                        DoTestHeadMotorDelay.SetSecAndOn(5);                    //Steven 20110908 : 上來後也要Delay一下
                    else
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                  //Steven 20110908 : 上來後也要Delay一下

                    if(CosFunction.bUseOneByOneIndexCheck==true &&
                       ArmSpeed_File[IndexArm].bDevicConfirm &&
                       TotalErrPart!="")                                        //kevin 20190629 add error show
                        Task=143;
                    else if(CosFunction.bUseOneByOneIndexCheck==true &&
                            TotalErrPart!="")                                   //KaiChen 20210104:
                        Task=143;
                    else
                        Task=142;
                }
            }
            break;
        case 142:
            if(INDEX_SUCKER_TYPE==1)                                            //jou 2012-01-04 需確認Index suck已經完整做完
            {
                bIndexSuckCheck=false;
                bIndexSuckCheck=fiosetview->ProcessIndexSuckDestroy2();
            }
            else
            {
                bIndexSuckCheck=true;
            }

            if(DoTestHeadMotorDelay.Off() && bIndexSuckCheck==true)
            {
                bIndexCheckNoStopVaccum=false;                                  //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
                flag=false;
                ErrPart=" ";
                for(int i=0; i<BTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<BTestSuck.iShtCol; j++)
                    {
                        flag2=false;
                        BTestSuck.CheckVaccumIsIniaialON(i, j, flag2);
                        if(flag2==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i][j];

                            if(CUSTOMER_CODE==CC_SCS)                           //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                                BTestSuck.Suck[i][j].Off();
                        }
                    }
                }

                if(flag && LastSet.iRealDummy==REALLY)                          //Steven 20120726 : 有跑IC才檢查Socket
                    Task=143;
                else
                    Task=150;
            }
            break;
        case 143:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 143"))
            {
                IndexStatus=Z1_Z2_Normal;                                       //kevin 20190118 add index z states
                if(CUSTOMER_CODE==CC_SCS)                                       //jou 20170516 (Steven) : SCS要求index check偵測到device時需吹氣
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                    }
                }
                Task=144;
            }
            break;
        case 144:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 144"))
                Task=145;
            break;
        case 145:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                bIsTestSitICFallDown=true;                                      //Steven 20130613
                if(CosFunction.bUseOneByOneIndexCheck==true &&
                   ArmSpeed_File[IndexArm].bDevicConfirm)                       //kevin 20190629 add error show
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY1, false, TotalErrPart);
                }
                else if(CosFunction.bUseOneByOneIndexCheck==true &&
                        INDEX_SUCKER_TYPE==1 &&
                        TotalErrPart!="")                                       //KaiChen 20210104:
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY2, false, TotalErrPart);
                }
                else if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)       //Steven 20130604 : Socket殘料要按Z1
                {
                    ShowErrorMessage("WAR0310", K_RETRY, MTestY2, false, ErrPart);
                }
                else
                {
                    ShowMyMessage("Arm2 detect Test Socket has IC error", "Arm 2偵測到Socket有IC殘留!!", "DoTestHeadMotor 145");
                }
                ErrPart="";

                Task=1;
            }
            break;
        case 150:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 150"))
            {
                 IndexStatus=Z1_Z2_Normal;                                      //kevin 20190118 add index states

                if(IniConfig.bUseAutoSiteMapping &&                             //jou 2011-03-24 start : Auto Site Mapping
                   (CosFunction.bUSEJCETSiteMapMode==false ||                   //jou 2016-10-28 JCET 要求Site Mapping 必須測試到pass bin才能通過
                    CosFunction.bAutoSiteMappingSetOpenBIN==true))              //jou 20230216 : 修正 bAutoSiteMappingSetOpenBIN 失效
                {
                    if(LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false)
                    {
                        if(CosFunction.bAutoSiteMappingSetOpenBIN==true)        //jou 20200928 : Auto Site Mapping Set Open BIN
                        {
                            bGetOpenBin=false;
                            Prod.iOpenBin=TestIF_File.iOpenBin;
                            fMain->SetOpenBin();
                            Task=1500;
                        }
                        else
                        {
                            flag=true;
                            for(int i=0; i<BTestSuck.iShtRow; i++)
                            {
                                for(int j=0; j<BTestSuck.iShtCol; j++)
                                {
                                    if(flag==true ||
                                       IniConfig.bI21ASMNeedCheckEachSiteOpen)  //Steven 20120726 : AutoSiteMapping, 當確認Open Bin時,同時也要檢查是不是所有Dut都Open
                                    {
                                        if(IsNNMode()==NN_1Row)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j]==true)
                                            {
                                                if(i<1)
                                                {
                                                    BTestSuck.SetItemData(i, j, HAS_IC);
                                                    BTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);    //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                else
                                                {
                                                    FTestSuck.SetItemData(i-1, j, HAS_IC);
                                                    FTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);    //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                flag=false;
                                            }
                                        }
                                        else if(IsNNMode()==NN_2Row)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j]==true)
                                            {
                                                if(i<2)
                                                {
                                                    BTestSuck.SetItemData(i, j, HAS_IC);
                                                    BTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);    //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                else
                                                {
                                                    FTestSuck.SetItemData(i-2, j, HAS_IC);
                                                    FTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);    //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                flag=false;
                                            }
                                        }
                                        else
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j]==true ||     //ChungHung 20130910 alter for SCK can close site by Index
                                               LastSet.bUseTestSocket[1][i][j]==true)
                                            {
                                                if(TestIF_File.iShuttleMode==1 &&   //Ifor 20171005 (Steven) : add 避免Auto Site mapping 關ARM1 導致Hangup
                                                   TestIF_File.iShuttle_Sel==1)
                                                {
                                                    BTestSuck.SetItemData(i, j, HAS_IC);
                                                    BTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);    //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                else
                                                {
                                                    FTestSuck.SetItemData(i, j, HAS_IC);
                                                    FTestSuck.cDeviceInf[i][j].sprintf("AutoSiteMap%02d%02d", i+1, j+1);    //Steven 20190313 : Add dummy 2DID for Auto Site Map
                                                }
                                                flag=false;
                                            }
                                        }
                                    }
                                }
                            }

                            if(IsNNMode()==NN_2Row)
                            {
                                InitTestSuckTestIC_TwoArm32Site_Task();
                                Task=170;
                            }
                            else
                            {
                                if(TestIF_File.iShuttleMode==1 &&
                                   TestIF_File.iShuttle_Sel==1)                 //jou 20200827 : 修正auto site mapping 只開arm2異常
                                    InitBTestSuckTestICTask();
                                else
                                    InitFTestSuckTestICTask();
                                Task=160;
                            }

                            bGetOpenBin=true;
                            Prod.iOpenBin=0;                                    //kevin 20160513
                        }
                        RecordProcess("Auto Site Map Start");                   //kevin 20160125
                        break;
                    }
                    else
                    {
                        Task=1500;
                    }
                }
                else
                {
                    Task=1500;
                }
            }
            break;
        case 160:
            if(TestIF_File.iShuttleMode==1 &&
               TestIF_File.iShuttle_Sel==1)                                     //kevin 20160414 add 只使用ARM2 Auto SiteMap
            {
                 if(DoBTestSuckTestIC())
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            BTestSuck.Item[i][j]=NULL_IC;
                        }
                    }
                    Task=1500;
                    bGetOpenBin=false;
                }
            }
            else
            {
                if(DoFTestSuckTestIC())
                {
                    for(int i=0; i<BTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<BTestSuck.iShtCol; j++)
                        {
                            FTestSuck.Item[i][j]=NULL_IC;
                        }
                    }
                    Task=1500;
                    bGetOpenBin=false;
                }
            }
            break;
        case 170:                                                               //Steven 20140512: For HT-9047
            if(DoTestSuckTestIC_TwoArm32Site())
            {
                FTestSuck.SetAllToNullIC();
                BTestSuck.SetAllToNullIC();
                Task=1500;
                bGetOpenBin=false;
            }
            break;
        case 1500:
            if(bUseTwoArm32Site==true)
                Task=1650;
            else
                Task=1550;
            break;
        case 1530:                                                              //kevin 20131112
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 1530"))
            {
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    {
                        MOT[MInShuttle1].fCanMoveM=false;
                        MOT[MInShuttle2].fCanMoveM=false;
                        fFrontNeedTest=true;
                    }
                    iInitStartDelayTimeCT=0;                                    //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                    iInitStartDelayTimeDetCount=0;                              //kevin 20161214 (Steven) 等待Delay time 加減時間
                    Task=1535;
                }
                else
                {
                    if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    {
                        MOT[MInShuttle1].fCanMoveM=false;
                        MOT[MInShuttle2].fCanMoveM=false;
                        fFrontNeedTest=true;
                    }
                    Task=1535;
                }
            }
            break;
        case 1535:                                                              //kevin 20131112 add
            if(bOneTimeFlag)
            {
                bOneTimeFlag=false;
                iBackUpZ1DownPosition=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset;
            }

            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                bOneTimeFlag=true;
                IndexStatus=Z1Up_Z2Down;
                bRearHeadICFallDown=false;
                InitTestYTask();
                bCanNotDisableOneCycle=false;
                fMain->chkReadTorque1->Checked=false;
                fMain->chkReadTorque2->Checked=false;
                if(bInitialStartIndexCheckDone==false)                          //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                    bInitialStartIndexCheckDone=true;
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    if(bOneTimeWait==false || iOneCycle)
                    {
                        if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                            Task=1700;                                          //if tested half do home，need check arm is has device
                        else
                            Task=1720;

                        bInitialSackTime=true;                                  //kevin 20131112 第一次吸取ic等待時間
                        DoTestHeadMotorDelay2.SetSecAndOn(1);                   //kevin 20131112 不等待
                        iInitStartDelayTimeCT=0;                                //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                        iInitStartDelayTimeDetCount=0;                          //kevin 20161214 (Steven) 等待Delay time 加減時間
                    }
                }
                else
                {
                    Task=600;                                                   // 到此為止 必須 Z1 Down Z2 Up;
                }
            }
            break;
        case 1550:
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 1550"))
                {
                    if(bInitialStartIndexCheckDone==false)                      //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(Temperature.bZ2DownSocket)                           //Steven 20140827 : 第一次吸取ic等待時間
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                        else if(bOneTimeWait==false || iOneCycle)               //kevin 20131112 tem
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                    }
                    else
                    {
                        if(DeviceForm.bShuttleWaitingOutSiteChamber)
                        {
                            MOT[MInShuttle1].fCanMoveM=false;
                            MOT[MInShuttle2].fCanMoveM=false;
                            fFrontNeedTest=true;
                        }
                        Task=1600;
                    }
                }
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 1550"))
                {
                    if(bInitialStartIndexCheckDone==false)                      //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;

                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(Temperature.bZ2DownSocket)                           //Steven 20140827 : 第一次吸取ic等待時間
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                        else if(bOneTimeWait==false || iOneCycle)               //kevin 20131112 tem
                        {
                            if(DeviceForm.bShuttleWaitingOutSiteChamber)
                            {
                                MOT[MInShuttle1].fCanMoveM=false;
                                MOT[MInShuttle2].fCanMoveM=false;
                                fFrontNeedTest=true;
                            }
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                            Task=1600;
                        }
                    }
                    else
                    {
                        if(DeviceForm.bShuttleWaitingOutSiteChamber)
                        {
                            MOT[MInShuttle1].fCanMoveM=false;
                            MOT[MInShuttle2].fCanMoveM=false;
                            fFrontNeedTest=true;
                        }
                        Task=1600;
                    }
                }
            }
            break;
        case 1600:
            if(bOneTimeFlag)
            {
                bOneTimeFlag=false;
                iBackUpZ1DownPosition=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset;
            }

            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 1600"))
                {
                    bOneTimeFlag=true;
                    IndexStatus=Z1_Z2_Normal;
                    bRearHeadICFallDown=false;
                    InitTestYTask();
                    bCanNotDisableOneCycle=false;
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=false;
                    if(bInitialStartIndexCheckDone==false)                      //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(bOneTimeWait==false || iOneCycle)
                        {
                            if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                                Task=1700;                                      //if tested half do home，need check arm is has device
                            else
                                Task=1720;

                            if(Temperature.iInitialStart1Time!=0)
                                bInitialSackTime=true;                          //kevin 20131112 第一次吸取ic等待時間
                            DoTestHeadMotorDelay2.SetSecAndOn(1);               //kevin 20131112 不等待
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                        }
                    }
                    else
                    {
                        Task=600;                                               // 到此為止 必須 Z1 Down Z2 Up;
                    }
                }
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, iSpeedSlow))//Marc
                {
                    bOneTimeFlag=true;
                    IndexStatus=Z1Down_Z2Up;
                    bRearHeadICFallDown=false;
                    InitTestYTask();
                    bCanNotDisableOneCycle=false;
                    fMain->chkReadTorque1->Checked=false;
                    fMain->chkReadTorque2->Checked=false;
                    if(bInitialStartIndexCheckDone==false)                      //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                        bInitialStartIndexCheckDone=true;
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(bOneTimeWait==false || iOneCycle)
                        {
                            if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                                Task=1700;                                      //if tested half do home，need check arm is has device
                            else
                                Task=1720;

                            if(Temperature.iInitialStart1Time!=0)
                                bInitialSackTime=true;                          //kevin 20131112 第一次吸取ic等待時間
                            DoTestHeadMotorDelay2.SetSecAndOn(1);               //kevin 20131112 不等待
                            iInitStartDelayTimeCT=0;                            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
                            iInitStartDelayTimeDetCount=0;                      //kevin 20161214 (Steven) 等待Delay time 加減時間
                        }
                    }
                    else
                    {
                        Task=600;                                               // 到此為止 必須 Z1 Down Z2 Up;
                    }
                }
            }
            break;
        case 1650:                                                              //2013-01-15    Dell    Add nn Mode
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 1650"))
            {
                Task=1660;
            }
            break;
        case 1660:
            if(bOneTimeFlag)
            {
                bOneTimeFlag=false;
                iBackUpZ1DownPosition=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset;
                iBackUpZ2DownPosition=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset;
            }

            if(DeviceForm.ContactMode==TMove ||
               DeviceForm.ContactMode==TMoveDrop ||                             //jou 2012-02-03 新增T Move Drop
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)                        //Steven 20160130 : TMove Soft contact
            {
                TMode=true;
            }
            else
            {
                if(IniConfig.bIndexPickupErrStop==true &&                       //jou 2012-02-29 index pick up error,index arm move to center & alarm
                   bIndexArm2PickupErrStop==true)
                {
                    TMode=true;
                }
                else
                {
                    TMode=false;
                }
            }

            if(bintered1==true)                                                 //Isaac 20200922 : 紀錄indexArmY encoder值和command值
            {
                bintered1=false;
                RecordIndexPosition(1, 1);                                      //Isaac 20200922 : 紀錄indexArmY encoder值和command值Arm1/Socket
                EncoderTeachingMaxMinCount(1);                                  //Isaac 20201012 : 每次完成動作，比較紀錄Encoder和Teaching點的差值
            }

            #ifdef INDEX_PROTECT_TMOVE
            if(bOverRangeDoTMode==true && bTriger4Indexhome==false)             //Isaac 20201012 : index Y超過範圍，做一次Tmode
            {
                bTriger4Indexhome=true;
                bOverRange4Indexhome=true;
                TrigerIndexAxisHome();                                          //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
                return;
            }
            #endif

            if(MOT[MTestY1].Gali_nnMode_Z1Z2_Down(iSpeedY, TMode))
            {
                Task=1670;
            }
            break;
        case 1670:
            if(bInitialStartIndexCheckDone==false)                              //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                bInitialStartIndexCheckDone=true;
            if(LastSet.iTemperature==Tempture_Hot)
            {
                if(bOneTimeWait==false || iOneCycle)
                {
                    if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    {
                        MOT[MInShuttle1].fCanMoveM=false;
                        MOT[MInShuttle2].fCanMoveM=false;
                        fFrontNeedTest=true;
                    }
                    Task=1680;
                }
            }
            else
            {
                if(DeviceForm.bShuttleWaitingOutSiteChamber)
                {
                    MOT[MInShuttle1].fCanMoveM=false;
                    MOT[MInShuttle2].fCanMoveM=false;
                    fFrontNeedTest=true;
                }
                Task=1680;
            }

            if(Prod.bUseSocketHeating==true && bUseSocketHeating_Wait==false)   //Ztex 2024.09.07 Add Use Socket Heating
            {
                iInitStartDelayTimeCT=0;                                        //Ztex 2024.09.06 Add 4x4 Mode Clear iInitStartDelayTimeCT
                iInitStartDelayTimeDetCount=0;                                  //Ztex 2024.09.06 Add 4x4 Mode Clear iInitStartDelayTimeCT
                DoUseSocketHeating.SetSecAndOn(Prod.iUseSocketHeating);
                DoUseSocketHeating.LatchCycleTime(true);
                bUseSocketHeating_Wait=true;
                Task=1675;
            }
            break;
        case 1675:                                                              //Ztex 2024.09.07 Add Use Socket Heating
            iUseSocketHeating_Time=DoUseSocketHeating.LatchCycleTime()/1000;
            fMain->labUser->Caption=AnsiString(iUseSocketHeating_Time);
            if(DoUseSocketHeating.Off()==true)
            {
                fMain->labUser->Caption="User";
                bDoWhenPressStopOverUseInitialDelay=false;
                Task=1680;
            }
            break;
        case 1680:
            bOneTimeFlag=true;
            IndexStatus=Z1_Z2_Down;
            bRearHeadICFallDown=false;
            InitTestYTask();
            bCanNotDisableOneCycle=false;
            fMain->chkReadTorque1->Checked=false;
            fMain->chkReadTorque2->Checked=false;
            if(bInitialStartIndexCheckDone==false)                              //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
                bInitialStartIndexCheckDone=true;
            if(LastSet.iTemperature==Tempture_Hot)
            {
                if(bOneTimeWait==false || iOneCycle)
                {
                    if(DeviceForm.bSuckShuttleDeviceAfterTested && DeviceForm.bShuttleWaitingOutSiteChamber)
                        Task=1700;                                              //if tested half do home，need check arm is has device
                    else
                        Task=1720;
                    if(Temperature.iInitialStart1Time!=0)
                        bInitialSackTime=true;                                  //kevin 20131112 第一次吸取ic等待時間
                    DoTestHeadMotorDelay2.SetSecAndOn(1);                       //kevin 20131112 不等待
                }
            }
            else
            {
                Task=600;                                                       // 到此為止 必須 Z1 Z2 Up ;
            }
            break;
        case 1700:
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    FTestSuck.Suck[i][j].On();
                    if(INDEX_SUCKER_TYPE==1)                                    //Steven 20111202
                    {
                        fiosetview->bIndexSuck[0][i][j]=true;
                    }
                }
            }

            DoTestHeadMotorDelay.SetMSAndOn(150);
            Task=1710;
            break;
        case 1710:
            if(DoTestHeadMotorDelay.Off())
            {
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        FTestSuck.CheckVaccumIsIniaialON(i, j, flag);
                    }
                }
                Task=1720;
            }
            break;
        case 1720:
            if(DoTestHeadMotorDelay2.Off())
            {
                if(DeviceForm.bShuttleWaitingOutSiteChamber)
                    MOT[MInShuttle1].fCanMoveM=true;

                Task=600;
            }
            break;
        case 600:
            DoTestY();
            break;
        case 15000:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 15000"))
            {
                IndexStatus=Z1_Z2_Normal;                                       //kevin 20190118 add
                Task=15100;
            }
            break;
        case 15100:
            if(CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //Frank 20221122 : 2DID sorting for ATK
            {
                Task=140;
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 15100"))
                    Task=140;
            }
            break;
        case 20000:
            Task=20100;
            break;
        case 20100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor 20100"))
                Task=20200;
            break;
        case 20200:
            if(FrontTestHeadHasIC())
            {
                Task=20300;
            }
            else if(RearTestHeadHasIC())
            {
                Task=21000;
            }
            else
            {
                bPlaceToShuttleFirst=false;
                Task=20210;
                startTick=MyTickCount();
                endTick=MyTickCount();
            }
            break;
        case 20210:
            nowTick=MyTickCount();
            if((nowTick-endTick)>1000)
            {
                startTick=MyTickCount();
                endTick=MyTickCount();
            }
            else
            {
                endTick=MyTickCount();
                if((endTick-startTick)>2000)                                    //Steven 20190304 : 20000 --> 2000
                {
                    Task=1;
                }
            }
            if(ShuttleHasIC()==false &&
               MOT[MInShuttle1].fCanMoveR==true &&
               MOT[MInShuttle2].fCanMoveR==true)
            {
                SetInitialICCheck();
                Task=1;
            }
            break;
        case 20300:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoTestHeadMotor 20300"))
            {
                Task=20400;
                iFrontTestDestroyICTask=1;
            }
            break;
        case 20400:
            sp=GetIndexZSpeed(1);                                               //Steven 20160524 : Index Z軸速度整合為Function
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, sp))
                Task=20500;
            break;
        case 20500:
            if(DoFrontTestDestroyIC(false))
                Task=20100;
            break;
        case 21000:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor 21000"))
            {
                iRearTestDestroyICTask=1;
                Task=21400;
            }
            break;
        case 21400:
            sp=GetIndexZSpeed(0);                                               //Steven 20160524 : Index Z軸速度整合為Function
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe,sp))
                Task=21500;
            break;
       case 21500:
            if(DoRearTestDestroyIC(false))
                Task=20100;
            break;
       case 30000:
            if(IndexEveryTimeCheckEP())                                         //jou 2011-04-28 start : Index每一次都確認EP是否有充飽氣。
            {
                Task=10;
            }
            break;
        case iCASE_REAL_CCD2:                                                   //Steven 20120222 Start: Index Check時,如果有IC,可以Skip重作ROI或是Retry再檢查一次, 以下整段換位置
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotor iCASE_REAL_CCD2"))
            {
                Task=iCASE_REAL_CCD3;
            }
            break;
        case iCASE_REAL_CCD3:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotor iCASE_REAL_CCD3"))
            {
                if((IniConfig.bHaveRTCCheckSiteMap &&
                    IniConfig.bD35RTCCheckSiteMap) ||                           //Steven 20140513
                   (CosFunction.bRTCAutoModelVerify==true &&
                    IniConfig.bD36EnableRTCAutoModelVerify==true))              //JerryYang 20230324 : 有開Auto verify要傳map給RTC
                {
                    Task=40310;
                }
                else
                {
                    Task=40330;
                }
            }
            break;
        case 40310:
            if(SendSiteMapToRTC(true, 0)!=-1)                                   //jou 2014-05-06 add RTC site map
            {
                bDoFRTCAutoModelVerify=false;
                bDoBRTCAutoModelVerify=false;
                DoTestHeadMotorDelay.SetSecAndOn(10);
                Task=40320;
            }
            else
            {
                Task=40330;
            }
            break;
        case 40320:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtSiteMap])
            {
                Task=40330;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())                                 //Steven 20161206 (wei) : Add if
                    ShowMyMessage("RTC Site Map Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 40330:
            if(bTriggerRTC_AutoSTD)                                             //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD, 不要做Full view check
            {
                ret=ShowMyMessageBox_YES_NO("即將執行RTC Calibration model流程, 請確認Socket中沒有IC或異物", "Ready to do RTC Calibration model process, please check there is no abnormail object in socket!");
                if(ret!=1)
                {
                    break;
                }

                Task=iCASE_REAL_CCD6;
                fContact->InitROILearningTask();
            }
            else
            {
                COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG]=false;
                COM2->SendCommToVision(COM2->rtFullTOK, true);
                DoTestHeadMotorDelay.SetSecAndOn(10);
                Task=iCASE_REAL_CCD4;
            }
            break;
        case iCASE_REAL_CCD4:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTOK])
            {
                COM2->DoReleaseAndInspEnd();
                bRTCFullViewError=false;                                        //Steven 20120206 : RTC重複錯誤
                if(CosFunction.bIndexCheckCanTurnOff &&                         //Isaac 20211019 : 可選擇做index check的時機
                  ((IniConfig.iD71IndexCheckOnOffMode==0 && bLotStartEndNeedIndexCheck==false) ||
                   (IniConfig.iD71IndexCheckOnOffMode==1 && bIndexJamNeedIndexcheck==false) ||
                    IniConfig.iD71IndexCheckOnOffMode==2))
                {
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(IniConfig.bD55DisableIndexCheck &&
                        REAL_TIME_CCD==true &&
                        COM2->bCCDDummyRum==false)                              //Steven 20150723 : Fixed for SCK
                {
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(CosFunction.bAfterAutoCleanNoIndexCheck &&              //Steven 20191212 : 劉仁洲說Auto Clean只要作一次Index Check
                        (IniConfig.iD69IndexCheckModeForAutoClean==1 ||
                         IniConfig.iD69IndexCheckModeForAutoClean==2) &&
                        bAutoCleanFinishOnlyUseRTC==true)
                {
                    bAutoCleanFinishOnlyUseRTC=false;
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(CosFunction.bBeforeAutoCleanOnlyUseRTC==true &&         //JerryYang 20161216 (Steven) auto clean的前後只靠RTC來檢查socket,不做index下壓至socket吸真空
                        bAutoCleanFinishOnlyUseRTC==true &&
                        REAL_TIME_CCD==true &&
                        COM2->bCCDDummyRum==false)
                {
                    bAutoCleanFinishOnlyUseRTC=false;
                    if(IniConfig.bUseAutoSiteMapping &&
                       CosFunction.bUSEJCETSiteMapMode==false &&
                       LastSet.iRunStartMode==rsmAutoSiteMap &&
                       bSiteMappingCHKOK==false)                                //Steven 20190313 : Fixed for auto site mpa no open bin when enable [D55]
                        Task=150;
                    else
                        Task=1500;                                              //Steven 20231208 : 1550 --> 1500
                }
                else if(IniConfig.bD75OneCycleFinishedAlawayLearnRTCGolden ||   //Sam 20240117 : OneCycle 完成做完 Full view check 後都需要做 RTC Learning golden
                        bTriggerRTC_AutoSTD)                                    //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD
                {
                    Task=iCASE_REAL_CCD6;
                    fContact->InitROILearningTask();
                }
                else
                {
                    Task=9;                                                     //2013-01-15    Dell
                }
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtFullTNG])
            {
                COM2->DoReleaseAndInspEnd();
                Task=iCASE_REAL_CCD5;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())                                 //ChungHung 20121005 add
                    ShowErrorMessage("WAR0337", 0, MMIndex, 0, __FUNC__);       //RTC FullT Time Out Error!
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case iCASE_REAL_CCD5:
            if(IndexAlarmInArmAway()==true)                                     //Steven 20130613 : Index異常時, In Arm要先讓位功能
            {
                if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)            //Steven 20151022 : add for MAXIM
                    bIsTestSitICFallDown=true;

                COM2->DoReleaseAndInspEnd();
                if(CosFunction.bRTCFullViewErrorOnlyRetry)                      //Steven 20150304 : bRTCFullViewErrorOnlyRetry預設改為True
                    ret=ShowErrorMessage("WAR0343", K_RETRY, MMCCD, bRTCFullViewError, ErrPart);            //RTC FullView Index Check Error!
                else
                    ret=ShowErrorMessage("WAR0343", K_RETRY|K_SKIP, MMCCD, bRTCFullViewError, ErrPart);     //RTC FullView Index Check Error!
                bRTCFullViewError=true;                                         //Steven 20120206 : RTC重複錯誤

                if(ret==K_RETRY)
                {
                    Task=1;
                }
                else
                {
                    Task=iCASE_REAL_CCD6;
                    fContact->InitROILearningTask();
                    bRTCFullViewError=false;
                }
            }
            break;
        case iCASE_REAL_CCD6:
            if(fContact->Do_ROILearning(true))                                  //ChungHung 20121011 鉬FullView 謖IndexCheck
            {
                Task=9;
            }
            break;
        case 50000:                                                             //Index 1 至 Sock 吸料
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset-iIndexArmCheck_SG_Arm1, iSpeedSlow))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                if(IndexCheckOneByOne(false, 0, iSiteCount, true))              //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=50100;
                }
            }
            break;
        case 50100:                                                             //Index 1 至 Drop 位置確認真空
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ1_Test);
             if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm1, iSpeedFast)) //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                Task=50110;
                CheckSocketHasICDelay.SetSecAndOn(0.5);
            }
            break;
        case 50110:
            if(CheckSocketHasICDelay.Off())
            {
                if(IndexCheckOneByOne(false, 0, iSiteCount))                    //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=50000;
                    iSiteCount++;
                    if(iSiteCount>=(FTestSuck.iShtCnt))
                    {
                        iSiteCount=0;
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                  //Steven 20110908 : 上來後也要Delay一下
                        //Task=122;
                        Task=122100;
                    }
                }
            }
            break;
        case 60000:                                                             //Index 2  至 Sock 吸料
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset-iIndexArmCheck_SG_Arm2, iSpeedSlow))  //KaiChen 20200826 ：矽格-湖口，要求IndexCheck使用Contact高度不使用Offset
            {
                if(IndexCheckOneByOne(false, 1, iSiteCount, true))              //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=60100;
                }
            }
            break;
        case 60100:                                                             //Index 2  至 Sock 吸料
            iIndexUpPos=GetSocketCheckPos(Prod.TestZ2_Test);                    //Steven 20140620 : 整合為Function
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+iIndexUpPos-iIndexArmCheck_SG_Arm2, iSpeedFast))
            {
                Task=60110;
                CheckSocketHasICDelay.SetSecAndOn(0.5);
            }
            break;
        case 60110:
            if(CheckSocketHasICDelay.Off())
            {
                if(IndexCheckOneByOne(false, 1, iSiteCount))                    //Ifor 20200617 : add Use One By One Index Check Function 整合
                {
                    Task=60000;
                    iSiteCount++;
                    if(iSiteCount>=FTestSuck.iShtCnt)
                    {
                        iSiteCount=0;
                        Task=142100;
                        DoTestHeadMotorDelay.SetSecAndOn(0.5);                  //Steven 20110908 : 上來後也要Delay一下
                    }
                }
            }
            break;
        case 80000:                                                             //Ifor 20200617 : add Use One By One Index Check Function 整合
            if(IndexCheckOneByOne(false, 0, iSiteCount))                        //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                iSiteCount++;
                if(iSiteCount>=FTestSuck.iShtCnt)
                {
                    Task=122100;
                }
            }
            break;
        case 80010:                                                             //Ifor 20200617 : add Use One By One Index Check Function 整合
            if(IndexCheckOneByOne(false, 1, iSiteCount))                        //Ifor 20200617 : add Use One By One Index Check Function 整合
            {
                iSiteCount++;
                if(iSiteCount>=FTestSuck.iShtCnt)
                {
                    Task=142100;
                }
            }
            break;
    }
}
//------------------------------------------------------------------------------
//Richard 20220321 : 渠梁Side Push
//==>
int DoTesterSidePush(bool bPush)
{
    if(bPush==true)
    {
        #ifdef SOFT_SIMULTE
        return 1;
        #else
        if(Cylinder[C_TesterSidePush].Push())
        {
            return 1;
        }
        #endif
    }
    else if(bPush==false)
    {
        #ifdef SOFT_SIMULTE
        return 10;
        #else
        if(Cylinder[C_TesterSidePush].Pop())
        {
            return 10;
        }
        #endif
    }
    return 0 ;
}
//<==
//==============================================================================
int iDoAllPassVerifyTask=1;
bool DoAllPassVerifyRTC(bool bInitial)
{
    static bool bRTCRetry=false;                                                //Ifor 20251023 add:RTC無回應Retry 一次
    if(bInitial==true)
    {
        iDoAllPassVerifyTask=1;
        return false;
    }

    int &Task=iDoAllPassVerifyTask;
    switch(Task)
    {
        case 1:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease]=false;
            COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd]=false;
            COM2->DoReleaseAndInspEnd();
            DoTestHeadMotorDelay.SetSecAndOn(10);
            Task=41010;
            break;
        case 41010:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease] && COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd] && COM2->bRealTimeCom_ReceiveOK[COM2->rtInspStart])  //JerryYang 20220215 : Release跟InspEnd一起送
            {
                Task=41050;
                bRTCRetry=false;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(bRTCRetry==true)
                {
                     bRTCRetry=false;
                    if(COM2->OpenRTCComPortAgain())
                        ShowMyMessage("RTC Re-start Time out of Verify RTC");  //JerryYang 20220215 : Release跟InspEnd一起送
                    COM2->DoReleaseAndInspEnd();
                }
                else
                {
                    bRTCRetry=true;
                }
                Task=1;
            }
            break;
        case 41050:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtOPENVERIFYNG]=false;
            COM2->SendCommToVision(COM2->rtOPENVERIFYOK, true);
            DoTestHeadMotorDelay.SetSecAndOn(10);
            Task=41100;
            break;
        case 41100:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtOPENVERIFYOK])
            {
                Task=41200;
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtOPENVERIFYNG])
            {
                ShowMyMessage("RTC Open Verify NG");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Open Verify Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 41200:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSNG]=false;
            COM2->SendCommToVision(COM2->rtALLPASSOK, true);
            DoTestHeadMotorDelay.SetSecAndOn(10);
            Task=41300;
            break;
        case 41300:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSOK])
            {
                Task=42000;  //JerryYang 20220215 : Release跟InspEnd一起送
//                return true;
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSNG])
            {
                Task=41400;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(COM2->OpenRTCComPortAgain())
                    ShowMyMessage("RTC Verify All Pass Time out");
                COM2->DoReleaseAndInspEnd();
                Task=41200;
            }
            break;
        case 41400:
            if(IndexAlarmInArmAway()==true)
            {
                bIsTestSitICFallDown=true;
                ShowMyMessage("RTC Verify All Pass NG!!");
                Task=1;
            }
            break;
        case 42000:
            COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease]=false;
            COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd]=false;
            COM2->DoReleaseAndInspEnd();
            DoTestHeadMotorDelay.SetSecAndOn(10);
            Task=42100;
            break;
        case 42100:
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtRelease] && COM2->bRealTimeCom_ReceiveOK[COM2->rtInspEnd] && COM2->bRealTimeCom_ReceiveOK[COM2->rtInspStart])
            {
                bRTCRetry=false;
                return true;
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                if(bRTCRetry==true)
                {
                     bRTCRetry=false;
                    if(COM2->OpenRTCComPortAgain())
                        ShowMyMessage("RTC Re-start Time out of Verify RTC");
                    COM2->DoReleaseAndInspEnd();
                }
                else
                {
                    bRTCRetry=true;
                }
                Task=42000;
            }
            break;
    }
    return false;
}
//==============================================================================
int iSetupTask=1;
TQPF_Timer DoSetupTestDelay;
bool DoSetupTest(int iContactArm)
{
    int &Task=iSetupTask, ret;
    int Pos1, Pos2;
    int iNN=IsNNMode();

    switch(Task)
    {
        case 1:
            Pos1=MOT[MTestZ1].Gali_ReadPos();
            Pos2=MOT[MTestZ2].Gali_ReadPos();
            TestSocket.ClearAll();

            if(IniConfig.bIndexArm2SupplyLight==true ||                         //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
               TestIF_File.bForEgisTecTest==true     ||                         //Steven 20140922 : Arm2當作指紋測試
               (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                 //kevin 20150127 Arm1 下壓 arm2 測試
                TestIF_File.bArm1PickPlaceArm2Test==true))                      //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                IndexStatus=Z1Down_Z2Up;
                iIndexArm=0;
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_IC)
                        {
                            if(BAR_CODE_INSTALL!=ebctUninstall &&
                               TestIF_File.bEnableBarCode)                      //Steven 20160123 : For 2D function do contact test
                            {
                                TestSocket.SetItemData(i, j, HAS_IC);           //Steven 20160223 : 修正Contact Test時,連續測試的問題
                                TestSocket.cDeviceInf[i][j]=FTestSuck.cDeviceInf[i][j];
                            }
                            else
                            {
                                TestSocket.SetItemData(i, j, HAS_IC);
                            }
                        }
                    }
                }
            }
            else
            {
                if(bUseTwoArm32Site==true)
                {
                    iIndexArm=0;
                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(FTestSuck.Item[i][j]==HAS_IC)
                            {
                                if(BAR_CODE_INSTALL!=ebctUninstall &&
                                   TestIF_File.bEnableBarCode)                  //Steven 20160123 : For 2D function do contact test
                                {
                                    TestSocket.SetItemData(i+iNN, j, HAS_IC);   //Steven 20160223 : 修正Contact Test時,連續測試的問題
                                    TestSocket.cDeviceInf[i+iNN][j]=FTestSuck.cDeviceInf[i][j];
                                }
                                else
                                {
                                    TestSocket.SetItemData(i+iNN, j, HAS_IC);
                                }
                            }

                            if(BTestSuck.Item[i][j]==HAS_IC)                    //Steven 20160216 : 修正Contact Test可能不會測試的問題
                            {
                                if(BAR_CODE_INSTALL!=ebctUninstall &&
                                   TestIF_File.bEnableBarCode)                  //Steven 20160123 : For 2D function do contact test
                                {
                                    TestSocket.SetItemData(i, j, HAS_IC);       //Steven 20160223 : 修正Contact Test時,連續測試的問題
                                    TestSocket.cDeviceInf[i][j]=BTestSuck.cDeviceInf[i][j];
                                }
                                else
                                {
                                    TestSocket.SetItemData(i, j, HAS_IC);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(Pos1>Pos2)
                    {
                        IndexStatus=Z1Up_Z2Down;
                        iIndexArm=iContactArm;
                        for(int i=0; i<BTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<BTestSuck.iShtCol; j++)
                            {
                                if(BTestSuck.Item[i][j]==HAS_IC)                //Steven 20160216 : 修正Contact Test可能不會測試的問題
                                {
                                    if(BAR_CODE_INSTALL!=ebctUninstall &&
                                       TestIF_File.bEnableBarCode)              //Steven 20160123 : For 2D function do contact test
                                    {
                                        TestSocket.SetItemData(i, j, HAS_IC);   //Steven 20160223 : 修正Contact Test時,連續測試的問題
                                        TestSocket.cDeviceInf[i][j]=BTestSuck.cDeviceInf[i][j];
                                    }
                                    else
                                    {
                                        TestSocket.SetItemData(i, j, HAS_IC);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        IndexStatus=Z1Down_Z2Up;
                        iIndexArm=iContactArm;
                        for(int i=0; i<FTestSuck.iShtRow; i++)
                        {
                            for(int j=0; j<FTestSuck.iShtCol; j++)
                            {
                                if(FTestSuck.Item[i][j]==HAS_IC)                //Steven 20160216 : 修正Contact Test可能不會測試的問題
                                {
                                    if(BAR_CODE_INSTALL!=ebctUninstall &&
                                       TestIF_File.bEnableBarCode)              //Steven 20160123 : For 2D function do contact test
                                    {
                                        TestSocket.SetItemData(i, j, HAS_IC);   //Steven 20160223 : 修正Contact Test時,連續測試的問題
                                        TestSocket.cDeviceInf[i][j]=FTestSuck.cDeviceInf[i][j];
                                    }
                                    else
                                    {
                                        TestSocket.SetItemData(i, j, HAS_IC);
                                    }
                                }
                            }
                        }
                    }
                }
            }
#ifdef SOFT_SIMULTE
  #ifdef DEBUG
            if(fContact->fShow==true)
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                    for(int j=0; j<TestSocket.iShtCol; j++)
                        TestSocket.SetItemData(i, j, HAS_IC);
            }
  #endif
#endif
            Task=2200;
        case 2200:
            ProcessStartTestData(iIndexArm);
            DoSetupTestDelay.SetSecAndOn(TestIF.iMaxTime);
            InitTestTask();
            Task=2400;
        case 2400:
            ret=ProcessTestResult(iIndexArm);

            if(fContact->fShow==true &&
               iContactMode==CONTACT_DEVICE_MAP_CHECK &&
               fContact->bPlaceLoad==true)
            {
                ret=1;
            }

            if(ret==1)
            {
                bATC_SITE_2ND_CHECK[iIndexArm]=false;                           //Ifor 20160509 add ATC 測試時開啟第二點溫度監控
                if(ATC_SYSTEM==eNewATCSystem)
                {
                    for(int i=0; i<iATC_Use_Heat_Count; i++)                    //Ifor 20160516 修改ATC Heat 設定數
                        bATCSiteTest[i]=false;
                    ATC_InterfaceForm->SiteTesting(iATC_Use_Heat_Count, bATCSiteTest);
                }
                else if(ATC_SYSTEM==eATCHonPrecType)
                {
                    ATCInterfaceForm->SendTestStart(iIndexArm);
                }
                SetNoiseDelay=false;
                TestISTimeOut=false;
                ProcessCount(iIndexArm);
                TestSocket.ClearAll();
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer TempICTimeOut;
TQPF_Timer TempICDelay;
bool GetTempICResult()
{
    static bool bContactOK=false;
    int &Task=iTempICTask;
    AnsiString asWriteComm=":StartTest+";
    if(bPauseTester)
    {
        bPauseTester=false;
        TempICTimeOut.SetSecAndOn(Prod.iTesterDummyTime);
    }

    switch(Task)
    {
        case 1:
            if(SoftStop)
                break;
            IsTest=true;
            bContactOK=false;

            bTempComm6ReceiveOK=false;
            TempICTimeOut.SetSecAndOn(Prod.iTesterDummyTime);
            Task=10;
            break;
        case 10:                                                                //Send Rs232 command
            COM2->TempComm6->WriteCommData(asWriteComm.c_str(), asWriteComm.Length());
            bTempComm6ReceiveOK=false;
            Task=60;
            break;
        case 60:
            if(bTempComm6ReceiveOK)
            {
                bContactOK=true;
                TempICDelay.SetSecAndOn(0.2);
                Task=70;
            }

            if(TempICTimeOut.Off())
            {
                if(bContactOK)
                {
                    if(bTempComm6ReceiveOK)
                    {
                        Task=100;
                    }
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 70:
            if(TempICDelay.Off())
            {
                Task=10;
            }
            break;
        case 100:
            for(int i=0; i<TestSocket.iShtRow; i++)
                for(int j=0; j<TestSocket.iShtCol; j++)
                    iTesterBIN[i][j]=0;
            Task=1;                                                             //kevin 20130808
            return true;
    }

    return false;
}
//------------------------------------------------------------------------------
// jou 981214
// Contect & OneCycle & CleanOut & Home 完成時，index Y axis servo off
// contech & tech & AutoRun 時，檢查index是否有被動到
//------------------------------------------------------------------------------
int iIndexServoOffEncodeBackup[2]={0,0};
int iIndexServoOffEncode[2]={0,0};
int iIndexYAxisServoOnStateTask=1;
bool bIndexYAxisServoOnState=false;
//------------------------------------------------------------------------------
//void IndexYAxisServoOff()
//{
//    if(bIndexYAxisServoOnState==false)
//    {
//        iIndexServoOffEncodeBackup[0]=MOT[MTestY1].Gali_ReadEncoderPos();
//        iIndexServoOffEncodeBackup[1]=MOT[MTestY2].Gali_ReadEncoderPos();
//        MOT[MTestY1].ServoOnOff(false);
//        MOT[MTestY2].ServoOnOff(false);
//        iIndexYAxisServoOnStateTask=1;
//        bIndexYAxisServoOnState=true;
//    }
//}
//------------------------------------------------------------------------------
//bool CheckIndexYAxisServoOnState()
//{
//    int iDiffPos[2]={0,0};
//    bool bFlag[2]={false,false};
//    int &Task=iIndexYAxisServoOnStateTask;
//    switch(Task)
//    {
//        case 1:
//            if(bIndexYAxisServoOnState==true)
//            {
//                Task=50;
//            }
//            else
//            {
//                return true;
//            }
//            break;
//        case 50:
//            MOT[MTestY1].ServoOnOff(true);
//            MOT[MTestY2].ServoOnOff(true);
//            iIndexServoOffEncode[0]=MOT[MTestY1].Gali_ReadEncoderPos();
//            iIndexServoOffEncode[1]=MOT[MTestY2].Gali_ReadEncoderPos();
//            Task=100;
//            break;
//        case 100:
//            iDiffPos[0]=abs(iIndexServoOffEncodeBackup[0]-iIndexServoOffEncode[0]);
//            iDiffPos[1]=abs(iIndexServoOffEncodeBackup[1]-iIndexServoOffEncode[1]);
//            if((iDiffPos[0] > 10) || (iDiffPos[1] > 10))
//            {
//                MOT[MTestY1].iGali_SingalHomeTask=1;
//                MOT[MTestY2].iGali_SingalHomeTask=1;
//                bFlag[0]=false;
//                bFlag[1]=false;
//                Task=200;
//            }
//            else
//                Task=300;
//            break;
//        case 200:
//            if(bFlag[0]==false)
//                bFlag[0]=MOT[MTestY1].Gali_SingalHome();
//            if(bFlag[1]==false)
//                bFlag[1]=MOT[MTestY2].Gali_SingalHome();
//            if(bFlag[0] && bFlag[1])
//            {
//                iIndexServoOffEncodeBackup[0]=0;
//                iIndexServoOffEncodeBackup[1]=0;
//                Task=300;
//            }
//            break;
//        case 300:
//            return true;
//    }
//    return false;
//}
//------------------------------------------------------------------------------
//jou 2011-04-28 start : Index每一次都確認EP是否有充飽氣。
//------------------------------------------------------------------------------
int iIndexEveryTimeCheckEPTask=1;
void InitIndexEveryTimeCheckEP()
{
    iIndexEveryTimeCheckEPTask=1;
}
//------------------------------------------------------------------------------
TQPF_Timer CheckEPTimer;
TQPF_Timer ReleaseEPTimer;
bool IndexEveryTimeCheckEP()
{
    #ifndef SOFT_SIMULTE
    int ret=0;
    bool bAdamAlarm[2]={false, false};
    #endif
    int &Task=iIndexEveryTimeCheckEPTask;

    switch(Task)
    {
        case 1:
            if(LastSet.iRealDummy==REALLY && (EP_Install==3 || EP_Install==5))  //JerryYang 20171030 (wei) fix D24 EP check功能無效問題
            {
                bIndexEveryTimeCheckEPing=true;
                Task=100;
            }
            else
            {
                return true;
            }
            break;
        case 100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, MOT[MTestZ1].GailSpeed, "IndexEveryTimeCheckEP 100"))
            {
                ADAM_DirectWriteData((EP_MAXKPA<=500)?4095:2275, 0);            //ChungHung 20140508 有些氣球會破
                CheckEPTimer.SetSecAndOn(5);                                    //Steven 20240618 : 3 --> 5
                Task=200;
            }
            break;
        case 200:
            ADAM_DirectWriteData((EP_MAXKPA<=500)?4095:2275, 0);                //ChungHung 20140508 有些氣球會破
            #ifdef SOFT_SIMULTE
                return true;
            #else
                if(CheckEPTimer.Off())
                {
                    if((EP_Install==3 || EP_Install==5))                        //20111217 ChungHung
                    {
                        ADAM_Rang(IniConfig.iD26EPEncoderRange);
                        if(EP_Install==5)
                        {
                            bAdamAlarm[0]=ADAM_Alarm(0);
                            bAdamAlarm[1]=ADAM_Alarm(1);
                        }
                        else
                        {
                            bAdamAlarm[0]=ADAM_Alarm();
                            bAdamAlarm[1]=false;
                        }

                        if(bAdamAlarm[0]==false && bAdamAlarm[1]==false)
                        {
                            ADAM_WriteVoltage(DeviceForm.dPress);
                            bIndexEveryTimeCheckEPing=false;
                            ReleaseEPTimer.SetSecAndOn(1);                      //JerryYang 20171030 (wei) delay1秒 等待EP洩氣
                            Task=300;
                        }
                        else
                        {
                            ret=ShowErrorMessage("WAR1605", K_RETRY|K_SKIP, 0, MMSystem, "IndexEveryTimeCheckEP_200");  //"請檢查EP是否漏氣!"
                            if(ret==K_RETRY)
                            {
                                Task=1;
                            }
                            else
                            {
                                bIndexEveryTimeCheckEPing=false;
                                return true;
                            }
                        }
                    }
                    else
                    {
                        if(Sen[SnEPAlarm].IsOff()==false)                       //9045 lan=0 ip=6 port=3 bit=1
                        {
                            ADAM_WriteVoltage(DeviceForm.dPress);
                            bIndexEveryTimeCheckEPing=false;
                            ReleaseEPTimer.SetSecAndOn(1);                      //JerryYang 20171030 (wei) delay1秒 等待EP洩氣
                            Task=300;
                        }
                        else
                        {
                            ret=ShowErrorMessage("WAR1605", K_RETRY|K_SKIP, 0, MMSystem, "IndexEveryTimeCheckEP_200");  //"請檢查EP是否漏氣!"
                            if(ret==K_RETRY)
                            {
                                Task=1;
                            }
                            else
                            {
                                bIndexEveryTimeCheckEPing=false;
                                return true;
                            }
                        }
                    }
                }
            break;
            #endif
        case 300:
            if(ReleaseEPTimer.Off())                                            //JerryYang 20171030 (wei) delay1秒 等待EP洩氣
            {
                return true;
            }
            break;
    }
    return false;
}
//jou 2011-04-28 end
//------------------------------------------------------------------------------
bool CheckAndRecodrEP(int iArm)                                                 //Steven 20190114 : EP Alarm換位置
{
    AnsiString sBufferT;

    if(IniConfig.bD26EnableEPLog==true)                                         //Ifor 20150818 EP Log 程式修改
    {
        MyForceDirectories(asEPLogPath);
        sBufferT.sprintf("%s\\%04d%02d\\", asEPLogPath, SystemYear, SystemMonth);
        MyForceDirectories(sBufferT);
        String EPDate=Now().FormatString("yyyy-mm-dd");
        String EPName=sBufferT+EPDate+".csv";                                   //kevin 20160106
        String NowEPData="";

        String sHisiFileName="";                                                // kevin 20191210 add
        AnsiString sEPCopyPath="",sEPCopyFileName="";                           //kevin 20191210 add
        sEPCopyPath=asEPLogPath+"\\Current\\";
        sEPCopyFileName = sEPCopyPath;
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                     //kevin 20191210 add HISI log file name
        {
            beKeepLotNumber();                                                  //kevin 20191018 ASE KH read lot ID LOG
            sHisiFileName   =sBufferT +"ASE_EP_"+sHandleID+"_"+sInsertion+"_"+sDeviceType+"_"+sTestProgram+"_"+sHiLotID+"_"+sOSATLotID+"_"+ GetDateInfoByString() + ".csv";
            sEPCopyFileName =sEPCopyFileName + "ASE_EP_"+sHandleID+"_"+sInsertion+"_"+sDeviceType+"_"+sTestProgram+"_"+sHiLotID+"_"+sOSATLotID+"_"+ GetDateInfoByString() + ".csv";
        }

        if(!FileExists(EPName))
        {
            NowEPData = NowEPData+
            "Time"      +","+
            "Index"     +","+
            "Setting"   +","+
            "Kpa"       +","+
            "Kg"        +","+
            "Alarm";
            WriteDataToFile(EPName, NowEPData);                                 //Steven 20160604 : add protect of fopen
        }

        if(CosFunction.bHiSiliconFunction==true &&                              //kevin 20191212 add HISI LOG
           CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            if(sHisiFileName!="" && !FileExists(sHisiFileName))
            {
                NowEPData ="Ver:"+asHandlerVersion+"\n";
                NowEPData = NowEPData+
                "Time"      +","+
                "Index"     +","+
                "Setting"   +","+
                "Kpa"       +","+
                "Kg"        +","+
                "Alarm";
                NowEPData=NowEPData+",Lot ID ,OSAT Lot number " ;               //kevin 20191210 add
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 20191210 add HISI log file name
                {
                    Del_Tree(sEPCopyPath);
                    bBuildFilter(sEPCopyPath,asEPLogPath);
                }
                WriteDataToFile(sHisiFileName, NowEPData);                      //Steven 20160604 : add protect of fopen
            }
        }

        if((TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==iArm) || TestIF_File.iShuttleMode==0)
        {
            bool bAdamAlarm=false, bAdamEPAlarm[2]={false, false};              //jou 20170413 (Steven) : Read Adam EP 提升UPH

            ADAM_Rang(IniConfig.iD26EPEncoderRange);
            if(EP_Install==5)
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==iArm)
                {
                    bAdamEPAlarm[0]=ADAM_Alarm(iArm);
                    bAdamEPAlarm[1]=false;
                }
                else
                {
                    bAdamEPAlarm[0]=ADAM_Alarm(0);
                    bAdamEPAlarm[1]=ADAM_Alarm(1);
                }
            }
            else
            {
                bAdamEPAlarm[0]=ADAM_Alarm();                                   //JerryYang 20171024 (wei) add 單顆浮動頭誤差範圍,依照Mars定義給海思的資料
                bAdamEPAlarm[1]=false;
            }

            if(bAdamEPAlarm[0]==false && bAdamEPAlarm[1]==false)
                bAdamAlarm=false;
            else
                bAdamAlarm=true;

            NowEPData=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");
            if(iArm==0)
            {
                NowEPData +=",1,";                                              //Index 1
            }
            else
            {
                NowEPData +=",2,";                                              //Index 2
            }

            NowEPData +=fContact->edSetKg->Text+",";

            AnsiString StrData = IntToStr(iReadAdamEP);
            fContact->lblEPValueKpa->Caption = StrData;                         //Ifor 20160303 Add Test Arm2 EP pa Value
            NowEPData +=  StrData+",";

            double dbTransferKg=KpaTransferKG(iReadAdamEP)*dfComplianceUnit;    //kevin 20200313 add  dfComplianceUnit
            StrData = FormatFloat("0.0000", dbTransferKg);
            fContact->lblEPValueKg->Caption = StrData;                          //Ifor 20160303 Add Test Arm2 EP Kg Value
            NowEPData +=  StrData+",";

            if(bAdamAlarm==true)
            {
                NowEPData +="Fail";                                             //kevin 20200120
            }
            else
            {
                NowEPData +="Pass";
            }
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                beKeepLotNumber();                                              //kevin 20191018 ASE KH lot ID LOG
                NowEPData = NowEPData+","+sHisiAddTempLog(false);               //kevin 20191230 ASE KH HIS add Lot LOG EP
                WriteDataToFile(sHisiFileName.c_str(), NowEPData.c_str());
            }
            WriteDataToFile(EPName, NowEPData);                                 //Steven 20160604 : add protect of fopen
            if(CosFunction.bHiSiliconFunction &&
               CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 201912010  copy 海思檔案到上傳路徑
            {
                MyForceDirectories(sEPCopyPath);
                CopyFile(sHisiFileName.c_str(), sEPCopyFileName.c_str(), FALSE);//kevin 20191107 add
            }

            if(IniConfig.bD24EnableEPCheckFuntion ||                            //Steven 20240516 : 換位置, 先紀錄再alarm
               IniConfig.bD26EnableEPEncoderRange)                              //kevin 20200803 add EP alarm
            {
                if(bAdamAlarm &&
                   SystemStart==true)                                           //Steven 20220914 : 增加判斷,造免Alarm卡死
                {
                    ShowErrorMessage("WAR1605", K_RETRY, MMSystem, 0, "CheckAndRecodrEP");  //"請檢查EP是否漏氣!" //jou 20171120 (Steven) : 修正EP alarm位置錯誤
                    return true;
                }
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool CheckAndRecodrTorque(int iArm)                                             //kevin 20201027 Arm  扭力log
{
    AnsiString sBufferT;
    //Ifor 20150818 EP Log 程式修改    kevin 20191211
    if(IniConfig.bD26EnableEPLog==true)
    {
        MyForceDirectories(asTorqLogPath);
        sBufferT.sprintf("%s\\%04d%02d\\", asTorqLogPath, SystemYear, SystemMonth); //kevin 20160106 start
        MyForceDirectories(sBufferT);
        String EPDate=Now().FormatString("yyyy-mm-dd");
        String EPName=sBufferT+EPDate+".csv";                                   //kevin 20160106
        String NowEPData="";

        if(!FileExists(EPName))
        {
            NowEPData = NowEPData+
            "Time"      +","+
            "Index"     +","+
            "Torque";
            WriteDataToFile(EPName, NowEPData); //Steven 20160604 : add protect of fopen
        }

        if((TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==iArm) || TestIF_File.iShuttleMode==0)
        {
            NowEPData=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");
            if(iArm==0)
            {
                NowEPData +=",1,";                          //Index 1
                sBufferT.sprintf("%s%s",NowEPData,fMain->lbArm0Torque->Caption);
            }
            else
            {
                NowEPData +=",2,";                          //Index 2
                sBufferT.sprintf("%s%s",NowEPData,fMain->lbArm1Torque->Caption);
            }
            /*
            if(bAdamAlarm==true)
            {
                //NowEPData +="Alarm";
                NowEPData +="Fail";    //kevin 20200120
            }
            else
            {
                NowEPData +="Pass";
            } */

            WriteDataToFile(EPName, sBufferT);                                  //Steven 20160604 : add protect of fopen
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool NewCheckAndRecodeTorque(int iArm)                                          //KaiHuang 20201222
{
    static bool bFirstRun=true;
    static double dDelayTime_Backup;

    //Delay Time 變更需要重設標準值
    if(bFirstRun==true)
    {
        bFirstRun=false;
        dDelayTime_Backup=TestIF_File.dReadTorqueDelayTime;                     //kevin 20210804 等待時間去讀取扭力 change by setup
    }
    else
    {
        if(TestIF_File.dReadTorqueDelayTime!=dDelayTime_Backup)                 //kevin 20210804 等待時間去讀取扭力 change by setup
        {
            dDelayTime_Backup=TestIF_File.dReadTorqueDelayTime;                 //kevin 20210804 等待時間去讀取扭力 change by setup
            bResetArm1Value=true;
            bResetArm2Value=true;
        }
    }

    if(iArm==0)
    {
        if(FTestSuck.UseSiteHasIC()==true)
        {
            if(fMain->edTorue0->Text=="")                                       //還沒收到扭力值  kevin 20210118
            {
                if(fMain->chkReadTorque1->Checked==true)                        //還在讀取中
                {
                    return false;
                }
                else
                {
                    //Torque Delay 還沒跑完
                    #ifdef SOFT_SIMULTE
                        if(bReadArm1_Torque==true)
                        {
                            bReadArm1_Torque=false;
                            fMain->chkReadTorque1->Checked=false;
                            fMain->edTorue0->Text="1.3";
                            return false;
                        }
                    #else
                        if(bReadArm1_Torque==true)
                        {
                            bReadArm1_Torque=false;
                            fMain->chkReadTorque1->Checked=true;
                            return false;
                        }
                    #endif
                }
            }
            else
            {
                if(bResetArm1Value==true)
                {
                    if(iReadTorqueError==0)
                    {
                        bResetArm1Value=false;
                        //dSetArm1TorqueValue=(double)StrToFloat(fMain->edTorue0->Text.c_str());  //kevin 20210526 mark
                    }
                    else                                                        //讀取有錯誤不能當標準值
                    {
                        ;
                    }
                    //SaveTorque(1, "Arm1", fMain->edTorue0->Text.c_str(), IntToStr(iReadTorqueError)); //kevin 20210118
                    bResetRecordSetArm1Value=true;   //kevin 20210118
                    return true;
                }
                else
                {
                    if(bResetRecordSetArm1Value)                                //kevin 20210118 : Reset 標準值)
                    {
                        dSetArm1TorqueValue=(double)StrToFloat(fMain->edTorue0->Text.c_str());

                        if(dSetArm1TorqueValue==0.0)                            //kevin 20210303 扭力 = 0 不記錄
                        {
                            bResetRecordSetArm1Value=true;                      //kevin 20210223 change SET Value Return
                        }
                        else
                        {
                            bResetRecordSetArm1Value=false;
                            sSetTorquValue[0]=fMain->edTorue0->Text;            //kevin 20210421 扭力設定讀取值
                            SaveTorque(1, "Arm1", fMain->edTorue0->Text.c_str(), IntToStr(iReadTorqueError),sSetTorquValue[0]);  //kevin 20210118
                        }
                    }
                    else
                    {
                        SaveTorque(2, "Arm1", fMain->edTorue0->Text.c_str(), IntToStr(iReadTorqueError),sSetTorquValue[0]);
                    }

                    double dValue=fabs(dSetArm1TorqueValue-(double)StrToFloat(fMain->edTorue0->Text.c_str()));
                    AnsiString strError="";
                    if(iReadTorqueError==0)
                    {
                        if(dValue>TestIF_File.dReadTorque)                      //kevin 20210804 等待時間去讀取扭力 change by setup
                        {
                            iSetTorqueAlarm[0]++;                               //kevin 20210505 扭力連續幾次ALARM
                            if(iSetTorqueAlarm[0]>1)
                            {
                                iSetTorqueAlarm[0]=0;
                                strError.sprintf("Now:%.1f > Set:%.1f", dValue, TestIF_File.dReadTorque);           //kevin 20210804 等待時間去讀取扭力 change by setup
                                ShowErrorMessage("WAR0361", K_SKIP, MTestZ1, false, strError);                      //Steven 20211123 : WAR0370 --> WAR0361
                            }
                        }
                        else
                        {
                            iSetTorqueAlarm[0]=0;
                        }
                    }
                    return true;
                }
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        if(BTestSuck.UseSiteHasIC()==true)
        {
            if(fMain->edTorue1->Text=="")                                       //已經測完了還沒收到扭力值
            {
                if(fMain->chkReadTorque2->Checked==true)                        //還在讀取中
                {
                    return false;
                }
                else
                {
                    //測試結果回傳了, Torque Delay 還沒跑完
                    #ifdef SOFT_SIMULTE
                        if(bReadArm2_Torque==true)
                        {
                            bReadArm2_Torque=false;
                            fMain->chkReadTorque2->Checked=false;
                            fMain->edTorue1->Text="2.3";
                            return false;
                        }
                    #else
                        if(bReadArm2_Torque==true)
                        {
                            bReadArm2_Torque=false;
                            fMain->chkReadTorque2->Checked=true;
                            return false;
                        }
                    #endif
                }
            }
            else
            {
                if(bResetArm2Value==true)
                {
                    if(iReadTorqueError==0)
                    {
                        bResetArm2Value=false;
                        //dSetArm2TorqueValue=(double)StrToFloat(fMain->edTorue1->Text.c_str());  //kevin 20210118 mark
                    }
                    else                                                        //讀取有錯誤不能當標準值
                    {
                        ;
                    }
                    bResetRecordSetArm2Value = true;                            //kevin 20210118 : Reset 標準值
                    //SaveTorque(1, "Arm2", fMain->edTorue1->Text.c_str(), IntToStr(iReadTorqueError));  kevin 20210118
                    return true;
                }
                else
                {
                    if(bResetRecordSetArm2Value)                                //kevin 20210118 : Reset 標準值)
                    {
                        dSetArm2TorqueValue=(double)StrToFloat(fMain->edTorue1->Text.c_str());

                        if(dSetArm2TorqueValue==0.0)                            //kevin 20210303 扭力 = 0 不記錄
                        {
                            bResetRecordSetArm2Value=true;                      //kevin 20210223 change SET Value Return
                        }
                        else
                        {
                            bResetRecordSetArm2Value = false;
                            sSetTorquValue[1]=fMain->edTorue1->Text;            //kevin 20210421 扭力設定讀取值
                            SaveTorque(1, "Arm2", fMain->edTorue1->Text.c_str(), IntToStr(iReadTorqueError),sSetTorquValue[1]);  //kevin 20210118
                        }
                    }
                    else
                    {
                        SaveTorque(2, "Arm2", fMain->edTorue1->Text.c_str(), IntToStr(iReadTorqueError),sSetTorquValue[1]);
                    }

                    double dValue=fabs(dSetArm2TorqueValue-(double)StrToFloat(fMain->edTorue1->Text.c_str()));
                    AnsiString strError="";
                    if(iReadTorqueError==0)
                    {
                        if(dValue>TestIF_File.dReadTorque)                      //kevin 20210804 等待時間去讀取扭力 change by setup
                        {
                            iSetTorqueAlarm[1]++;                               //kevin 20210505 扭力連續幾次ALARM
                            if(iSetTorqueAlarm[1]>1)
                            {
                                iSetTorqueAlarm[1]=0;
                                strError.sprintf("Now:%.1f > Set:%.1f", dValue, TestIF_File.dReadTorque);           //kevin 20210804 等待時間去讀取扭力 change by setup
                                ShowErrorMessage("WAR0362", K_SKIP, MTestZ2, false, strError);                      //Steven 20211123 : WAR0371 --> WAR0362
                            }
                        }
                        else
                        {
                            iSetTorqueAlarm[1]=0;                               //kevin 20210505 扭力連續幾次ALARM
                        }
                    }
                    return true;
                }
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void SaveTorque(int iPosition, AnsiString asArm, AnsiString asValue, AnsiString asAlarmCount, AnsiString asSetValue)   //KaiHuang 20201222 Add : For ASE 高雄 扭力值存Log
{
    TStringList *SL;
    SL=new TStringList();
    GetTimeInfo();
    AnsiString  asTorqueDataPath, asTorqueDataFileName;
    AnsiString strPosition[5]={"IndexCheck", "SetValue", "Contact", "ShuttlePick", "ShuttlePlace"};
    AnsiString sFileName = sHandleID+"_"+sInsertion+"_"+sDeviceType+"_"+sTestProgram+"_"+sHiLotID+"_"+sOSATLotID;
    asTorqueDataPath.sprintf("D:\\HT9045_Log\\TorqueLog\\Check And Recode\\%04d\\%02d", SystemYear, SystemMonth);  //kevin 20210330

    asTorqueDataFileName.sprintf("%s-%s-%s_%s_%s.csv", Now().FormatString("yyyy"), Now().FormatString("mm"), Now().FormatString("dd"), "TorqueLog",sFileName);

    SL->Clear();
    SL->Add(strPosition[iPosition]);
    SL->Add(asArm);
    SL->Add(asValue);
    SL->Add(asAlarmCount);
    SL->Add(DeviceForm_File.dPress);                                            //kevin 20210330  add Force
    SL->Add(asSetValue);                                                        //kevin 20210421 add setValue

    fMain->slTorqueLog->AddTextWithDateTime(SL->CommaText);
    fMain->slTorqueLog->MySaveFileByFileName(asTorqueDataPath, asTorqueDataFileName);

    SL->Clear();                                                                //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete SL;
}
//------------------------------------------------------------------------------
//kevin 20130608 Index : 0 Arm1 1 Arm2        Arm :true 沒有ep 充氣
//------------------------------------------------------------------------------
void __fastcall bContSoftEpSwitch(int Index, bool Arm)
{
    if(fiosetview->fShow==true)                                                 //IO畫面
    {
        if(Index==0)
        {
            if(SW[SwEpArm1].Enable==true)                                       //Steven 20110708
            {
                if(FrontTestHeadHasIC())                                        //Index 1 有IC的話,狀態不可以改變
                    SW[SwEpArm1].OnOff(Arm);
            }
        }
        else if(SW[SwEpArm2].Enable==true)                                      //Index 2 有IC的話,狀態不可以改變
        {
            if(RearTestHeadHasIC())
                SW[SwEpArm2].OnOff(Arm);
        }
    }
    else
    {
        if(Index==0)
        {
            if(TestIF_File.iShuttleMode==1 &&
               TestIF_File.iShuttle_Sel==1 &&
               MachineTypeChoice==Type_HT9046_LS)
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&              //Steven 20250417 : fixed for [D58]
                   TestIF_File.bArm1PickPlaceArm2Test==true)
                {
                }
                else
                {
                    SW[SwEpArm1].OnOff(false);                                  //kevin 20220312 有電磁閥沒使用關ARM 關電磁閥
                    return;
                }
            }

            if(SW[SwEpArm1].Enable==true)                                       //Steven 20110708
            {
                SW[SwEpArm1].OnOff(Arm);
            }

            if(SW[SwIndEpArm1].Enable==true)                                    //Steven 20110708
            {
                SW[SwIndEpArm1].OnOff(Arm);
            }
        }
        else
        {
            if(TestIF_File.iShuttleMode==1 &&
               TestIF_File.iShuttle_Sel==0 &&
               MachineTypeChoice==Type_HT9046_LS)                               //ARM 2 close Arm1
            {
                if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&              //Steven 20250417 : fixed for [D58]
                   TestIF_File.bArm1PickPlaceArm2Test==true)
                {
                }
                else
                {
                    SW[SwEpArm2].OnOff(false);                                  //kevin 20220312 有電磁閥沒使用關ARM 關電磁閥
                    return;
                }
            }

            if(SW[SwEpArm2].Enable==true)
            {
                SW[SwEpArm2].OnOff(Arm);
            }

            if(SW[SwIndEpArm2].Enable==true)                                    //Steven 20110708
            {
                SW[SwIndEpArm2].OnOff(Arm);
            }
        }
    }
}
//------------------------------------------------------------------------------
int GetSocketCheckPos(int IndexPos)                                             //Steven 20140620 : 整合為Function
{
    int iIndexUpPos=0;

    if(IndexPos==0)                                                             //Steven 20160303 : Fixed for close arm offset will crash
    {
        iIndexUpPos=0;
    }
    else if(CosFunction.bIndexZDownToAboveSocket==true)
    {
        iIndexUpPos=IniConfig.dD41SocketInitialCheckOffset*100;
    }
    else if((DeviceForm.iSocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==true) ||
            (IniConfig.iD41SocketInitialICCheckPosition==1 && IniConfig.bTestIcCheckInContact==false))//Above Socket
    {
        if(IniConfig.bTestIcCheckInContact==true)
            iIndexUpPos=DeviceForm.fSocketInitialICCheckPositionOffset*100;     //Steven 210100818
        else
            iIndexUpPos=IniConfig.fIndexCheckOffset*100;                        //Steven 210100818  //Jimmychiu 20230922 : fixed D41 offset
    }
    return iIndexUpPos;
}
//------------------------------------------------------------------------------
int GetIndexZSpeed(int index)                                                   //Steven 20160524 : Index Z軸速度整合為Function
{
    int iScale, sp=1000;
    if(IniConfig.bD54SlowDown)
    {
        if(IniConfig.iD54SlowDownScale<1)
            iScale=1;
        else
            iScale=IniConfig.iD54SlowDownScale;
        sp=MOT[MTestZ1+index].Motor->PJogHighSpeed*iScale/100;
    }
    else
    {
        sp=MOT[MTestZ1+index].GailSpeed;
    }
    return sp;
}
//------------------------------------------------------------------------------
//KaiChen 20171225 (Steven)：Add Speed Display
//==>
void IndexAddSpeedDisplay()
{
    iIndexSpeed1++;
}
void IndexSubSpeedDisplay()
{
    iIndexSpeed1--;
}
//<==
//KaiChen 20171225 (Steven)：Add Speed Display
//------------------------------------------------------------------------------
int iIndexCheckinitial=0;
//------------------------------------------------------------------------------
bool IndexCheckOneByOne(bool bReset, int iWhichArm, int iSiteCount, bool bNeedUpCheck)  //Ifor 20200617 : add Use One By One Index Check Function 整合
{
    static AnsiString ErrPart="";

    int &iTask=iIndexCheckinitial;
    int iWhichRow=0, iWhichCol=0;
    bool flag2=true;
    bool bSuckOK=false;

    if(bReset)
    {
        iTask=1;
        TotalErrPart="";
        return false;
    }

    if(iSiteCount<TestSocket.iShtCol)
    {
        iWhichRow=0;
        iWhichCol=iSiteCount;
    }
    else
    {
        iWhichRow=1;
        iWhichCol=iSiteCount-TestSocket.iShtCol;
    }

    switch(iTask)
    {
        case 1:
            bIndexCheckNoStopVaccum=true;                                       //KaiChen 20210104:
            fiosetview->bIndexSuck[iWhichArm][iWhichRow][iWhichCol]=true;
            DoTestHeadMotorDelay.SetSecAndOn(0.5);
            iTask=2;
            break;
        case 2:                                                                 //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            if(INDEX_SUCKER_TYPE==1)
            {
                if(iWhichArm==0)
                    bSuckOK=fiosetview->ProcessIndexSuckDestroy1();
                else
                    bSuckOK=fiosetview->ProcessIndexSuckDestroy2();
            }

            if(DoTestHeadMotorDelay.Off() && bSuckOK==true)
            {
                iTask=3;
                if(bNeedUpCheck==true)                                          //Above 模式檢查直接離開 上升後才檢查
                {
                    return true;
                }
            }
            break;
        case 3:
            ErrPart=" ";
            flag2=false;
            if(iWhichArm==0)
                FTestSuck.CheckVaccumIsIniaialON(iWhichRow, iWhichCol, flag2);
            else
                BTestSuck.CheckVaccumIsIniaialON(iWhichRow, iWhichCol, flag2);

            bIndexCheckNoStopVaccum=false;                                      //KaiChen 20210104:

            if(flag2==true)
            {
                if(iWhichArm==0)
                {
                    ErrPart+=IndexSuckName[iWhichRow+IsNNMode()][iWhichCol];
                }
                else
                {
                    ErrPart+=IndexSuckName[iWhichRow][iWhichCol];
                }
                TotalErrPart+= ErrPart;
            }
            iTask=1;
            return true;
    }
    return false;
}
//<==
//KEVIN 20190530 ADD INDEX OPEN 4 SITE CHECK
int Taskinitial=0;
//------------------------------------------------------------------------------
bool IndexCheck4Site(bool bReset,int iWhichArm, int iSiteCount)
{
    int &iTask=Taskinitial;
    static AnsiString ErrPart="";

    int iSuckcount=iSiteCount*2;
    int iTotal=TestSocket.iShtCnt;
    bool flag=false, flag2=true;

    if(bReset)
    {
        iTask=1;
        return false;
    }

    if(iSuckcount>iTotal)
        iSuckcount=iTotal;

    switch(iTask)
    {
        case 1:
            if(iWhichArm==0)
            {
                for(int i=iSuckcount; i<iSuckcount+2; i++)
                {
                    fiosetview->bIndexSuck[0][0][i]=true;
                    fiosetview->bIndexSuck[0][1][i]=true;
                }
            }
            else if(iWhichArm==1)
            {
                for(int i=iSuckcount; i<iSuckcount+2; i++)
                {
                    fiosetview->bIndexSuck[1][0][i]=true;
                    fiosetview->bIndexSuck[1][1][i]=true;
                }
            }
            DoTestHeadMotorDelay.SetSecAndOn(0.5);
            iTask=2;
            break;
        case 2:                                                                 //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
            if(INDEX_SUCKER_TYPE==1)
            {
                if(iWhichArm==0)
                    fiosetview->ProcessIndexSuckDestroy1();
                else
                    fiosetview->ProcessIndexSuckDestroy2();
            }

            if(DoTestHeadMotorDelay.Off())
            {
                bIndexCheckNoStopVaccum=false;                                  //jou 2012-05-09 Index Check時,如果中途按暫停,也要繼續把suck()做完,避免負壓掉ic
                flag=false;
                ErrPart=" ";
                for(int i=0; i<FTestSuck.iShtRow; i++)
                {
                    for(int j=0; j<FTestSuck.iShtCol; j++)
                    {
                        flag2=false;
                        if(iWhichArm==0)
                            FTestSuck.CheckVaccumIsIniaialON(i, j, flag2);
                        else
                            BTestSuck.CheckVaccumIsIniaialON(i, j, flag2);

                        if(flag2==true)
                        {
                            flag=true;
                            ErrPart+=IndexSuckName[i][j];
                            TotalErrPart+=ErrPart;
                        }
                    }
                }
                if(flag)                                                        //kevin 20190819 add
                    TotalErrPart+=ErrPart;

                iTask=3;
            }
            break;
        case 3:
            iTask=1;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void CheckInitialStartDelayInSocket()                                           //JerryYang 20180629 (wei) Initial delay判斷包成函式
{
    static int iCountError=0;
    int iTempMaxWaitTime=0;                                                     //kevin 20160906 溫度加熱最大等待時間  //JerryYang 20180828 修正delay time錯誤問題, static int -> int
    double TestIntervals=0;                                                     //kevin 20160401 取得測試機間隔時間
    AnsiString sBuffer="";                                                      //kevin 20160414

    if(bUseInitDelay)                                                           //Steven 20171219 : 只有加熱模式要跑Initial Delay //Ifor 20180109 (Steven) : add KYEC 常高溫都要跑Initial Delay
    {
        if(CosFunction.bHiSiliconFunction==true)                                //kevin 20200110 add initial
        {
            if(bHISIInitiayDelay)                                               //kevin 20200110 add 海司強至initial delay
            {
                Prod.iInitialDelay=TestIF.dInitStartDelayTime;
                bNeedInitialTestDelay=true;
                iTempMaxWaitTime=Prod.iInitialDelay;
                bHISIInitiayDelay=false;
                bInitialTestDelayStatus[11]=true;                               //Steven 20221214 : Add 主畫面顯示
                sBuffer.sprintf("%d sec", iTempMaxWaitTime);
                RecordProcess("When onecycle KL use initial delay", sBuffer);
            }
        }

        if(bDoWhenPressStopOverUseInitialDelay)                                 //ChungHung 20150526 add for ATK want to even stop over will use initial delay
        {
            bDoWhenPressStopOverUseInitialDelay=false;
            bNeedInitialTestDelay=true;
            if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT)     //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                Prod.iInitialDelay=TestIF.dInitialDelay_6_RT;
            else
                Prod.iInitialDelay=TestIF.iInitialDelay_6;

            if(Prod.iInitialDelay>=iTempMaxWaitTime)
                iTempMaxWaitTime=Prod.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            bInitialTestDelayStatus[6]=true;                                    //wei 20171020 (jou) 延遲狀態顯示

            sBuffer.sprintf("%d sec", iTempMaxWaitTime);
            RecordProcess("When press stop time over use initial delay", sBuffer);
        }

        if(bDoAfterAutoCleanFunctionUseInitialDelay)
        {
            bDoAfterAutoCleanFunctionUseInitialDelay=false;
            bNeedInitialTestDelay=true;
            if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT)     //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                Prod.iInitialDelay=TestIF.dInitialDelay_3_RT;
            else
                Prod.iInitialDelay=TestIF.iInitialDelay_3;                      //ChungHung 20141210 add for SCK want to every event have delay

            if(Prod.iInitialDelay>=iTempMaxWaitTime)
                iTempMaxWaitTime=Prod.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            bInitialTestDelayStatus[3]=true;                                    //wei 20171020 (jou) 延遲狀態顯示

            sBuffer.sprintf("%d sec", iTempMaxWaitTime);
            RecordProcess("After auto clean use initial delay", sBuffer);
        }

        if(bDoAfterShowAlarmMessageUseInitialDelay)
        {
            bDoAfterShowAlarmMessageUseInitialDelay=false;
            bNeedInitialTestDelay=true;
            if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT)     //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                Prod.iInitialDelay=TestIF.dInitialDelay_2_RT;
            else
                Prod.iInitialDelay=TestIF.iInitialDelay_2;

            if(Prod.iInitialDelay>=iTempMaxWaitTime)
                iTempMaxWaitTime=Prod.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            bInitialTestDelayStatus[2]=true;                                    //wei 20171020 (jou) 延遲狀態顯示

            sBuffer.sprintf("%d sec", iTempMaxWaitTime);
            RecordProcess("After show alarm message use initial delay", sBuffer);
        }

        if(bDoAfterOpenHeatDoorUseInitialDelay)
        {
            bDoAfterOpenHeatDoorUseInitialDelay=false;
            bNeedInitialTestDelay=true;
            if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT)     //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                Prod.iInitialDelay=TestIF.dInitialDelay_5_RT;
            else
                Prod.iInitialDelay=TestIF.iInitialDelay_5;                      //ChungHung 20141210 add for SCK want to every event have delay

            if(Prod.iInitialDelay>=iTempMaxWaitTime)
                iTempMaxWaitTime=Prod.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            bInitialTestDelayStatus[4]=true;                                    //wei 20171020 (jou) 延遲狀態顯示

            sBuffer.sprintf("%d sec", iTempMaxWaitTime);
            RecordProcess("After open heat door use initial delay", sBuffer);

        }

        if(bDoWhenHappenTestedTimeBlowUseInitialDelay)
        {
            bDoWhenHappenTestedTimeBlowUseInitialDelay=false;
            bNeedInitialTestDelay=true;
            if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT)     //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                Prod.iInitialDelay=TestIF.dInitialDelay_4_RT;
            else
                Prod.iInitialDelay=TestIF.iInitialDelay_4;                      //ChungHung 20141210 add for SCK want to every event have delay

            if(Prod.iInitialDelay>=iTempMaxWaitTime)
                iTempMaxWaitTime=Prod.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            bInitialTestDelayStatus[5]=true;                                    //wei 20171020 (jou) 延遲狀態顯示

            sBuffer.sprintf("%d sec", iTempMaxWaitTime);
            RecordProcess("When happen tested time blow use initial delay", sBuffer);
        }

        if(bDoEveryFirstDeviceFunctionUseInitialDelay)
        {
            bDoEveryFirstDeviceFunctionUseInitialDelay=false;
            if(bFirstDeviceInitialTestDelayWhichOutAfterAutoClean==false)
            {
                bNeedInitialTestDelay=true;                                     //ChungHung 20140425 add for TSMC Device
                if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT) //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                    Prod.iInitialDelay=TestIF.dInitialDelay_1_RT;
                else
                    Prod.iInitialDelay=TestIF.iInitialDelay;                    //ChungHung 20141210 add for SCK want to every event have delay

                if(Prod.iInitialDelay>=iTempMaxWaitTime)
                    iTempMaxWaitTime=Prod.iInitialDelay;                        //kevin 20160906 溫度加熱最大等待時間
                bInitialTestDelayStatus[1]=true;                                //wei 20171020 (jou) 延遲狀態顯示

                if(CosFunction.bUseInitialDelayAsSoakTime &&                    //Steven 20170511 (wei) : 使用initial delay當 Soak time
                   Temperature.bUseInitialDelayAsSoakTime &&
                   bFirstInputForIndex && iTempMaxWaitTime<Temperature.fSoakTime)
                {
                    iTempMaxWaitTime=Temperature.fSoakTime;
                    bFirstInputForIndex=false;
                }

                sBuffer.sprintf("%d sec", iTempMaxWaitTime);
                RecordProcess("Every fisrt device delay use initial delay", sBuffer);
            }
            bFirstDeviceInitialTestDelayWhichOutAfterAutoClean=false;
        }

        if(bDoWhenNoFullSiteUseInitialDelay)                                    //wei 20151228 No FullSite delay
        {
            bDoWhenNoFullSiteUseInitialDelay=false;
            bNeedInitialTestDelay=true;
            if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT)     //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                Prod.iInitialDelay=TestIF.dInitialDelay_7_RT;
            else
                Prod.iInitialDelay=TestIF.iInitialDelay_7;

            if(Prod.iInitialDelay>=iTempMaxWaitTime)
                iTempMaxWaitTime=Prod.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            bInitialTestDelayStatus[7]=true;                                    //wei 20171020 (jou) 延遲狀態顯示

            sBuffer.sprintf("%d sec", iTempMaxWaitTime);
            RecordProcess("When no fullSite use initial delay", sBuffer);

        }

        if(bDoOTDOffUseInitialDelay)                                            //Steven 20160818 : OTD打開Delay
        {
            bDoOTDOffUseInitialDelay=false;
            bNeedInitialTestDelay=true;
            if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT)     //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                Prod.iInitialDelay=TestIF.dInitialDelay_9_RT;
            else
                Prod.iInitialDelay=TestIF.iInitialDelay_9;
            if(Prod.iInitialDelay>=iTempMaxWaitTime)
                iTempMaxWaitTime=Prod.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            bInitialTestDelayStatus[9]=true;                                    //wei 20171020 (jou) 延遲狀態顯示

            sBuffer.sprintf("%d sec", iTempMaxWaitTime);
            RecordProcess("OTD unlock use initial delay", sBuffer);

        }

        if(bTestFinishToNextTestOver)                                           //JerryYang 20181120 (Steven) : (Steven) : EOT 預熱功能bug,先還原成舊的
        {
            if(bFirstTest==false)                                               //kevin 20160311
            {
                TestIntervals=TestIntervalsTime.LatchCycleTime(false)/1000.0;
            }
            else
            {
                bFirstTest=false;
                iCountError=0;                                                  //kevin 20160310 連續幾次發警告
            }

            if(TestIntervals>=TestIF.iTestFinishToNextTestOver)                 //kevin 20160310
            {
                iInitContactCount=0;
                bNeedInitialTestDelay=true;
                if(IniConfig.bI13InitStartDelayHasFTandRT && iRunStartMode==RT) //Steven 20190313 : Initial Start Delay use different setting in FT and RT
                    Prod.iInitialDelay=TestIF.dInitialDelay_8_RT;
                else
                    Prod.iInitialDelay=TestIF.iInitialDelay_8;
                if(Prod.iInitialDelay>=iTempMaxWaitTime)
                    iTempMaxWaitTime=Prod.iInitialDelay;                        //kevin 20160906 溫度加熱最大等待時間
                bTestOverTimeTempOffsetF=true;
                iCountError++;                                                  //kevin 20160310 連續幾次發警告
                bInitialTestDelayStatus[8]=true;                                //wei 20171020 (jou) 延遲狀態顯示

                sBuffer.sprintf("EOT monitor time over delay %0.2f sec", TestIntervals);  //kevin 20160414 顯示超過時間
                RecordProcess(sBuffer, AnsiString(iTempMaxWaitTime)+AnsiString(" sec"));
            }
            else
            {
                iCountError=0;                                                  //kevin 20160310 連續幾次發警告
            }

            if(iCountError>5)
            {
                iCountError=0;                                                  //kevin 20160310 連續幾次發警告
                ShowErrorMessage("MES0714", K_RETRY, MMSystem);                 //kevin 20160318 EOT monitor time 時間設太短
            }
        }

        if(bTestStartToNextTestStartDelay)                                      //kevin 20181122 add SOT start SRQ41 send next SRQ 41)
        {
            iCountError++;                                                      //kevin 20160310 連續幾次發警告
            SW[SwPurgeAir].Off();                                               //kevin 20180928 add blower load board
            iContractCount=0;
        }
    }

    if(bNeedInitialTestDelay)                                                   //ChungHung 20140425 add for TSMC Device
    {
        if(bDoubleContact)                                                      //ChungHung 20140801 add Korea Want to count down in main status
        {
            Prod.iInitialDelay=TestIF.iInitialDelay;                            //kevin 20160906 溫度加熱最大等待時間
            iInitialCount=Prod.iInitialDelay;
        }
        else
        {
            Prod.iInitialDelay=iTempMaxWaitTime;                                //kevin 20160906 溫度加熱最大等待時間
            iInitialCount=Prod.iInitialDelay;
        }
        dwStartInitialCount.LatchCycleTime(true);
        iInitContactCount=0;                                                    //Steven 20151123 : 起測時溫度要補Offset
    }
}
//------------------------------------------------------------------------------
bool NeedResetInitialDelay()                                                    //JerryYang 20180828 (Steven) : 預熱過程中又觸發預熱的話, 就重新執行預熱
{
    bool bflag=false;

    if(CosFunction.bHiSiliconFunction==true)                                    //kevin 20200110 add initial
    {
        if(bHISIInitiayDelay)                                                   //kevin 20200110 add 海司強至initial delay
        {
            RecordProcess("Reset by When press stop time over use HISI initial delay");
            bHISIInitiayDelay=false;
            bflag=true;
        }
    }

    if(bDoWhenPressStopOverUseInitialDelay)
    {
        RecordProcess("Reset by When press stop time over use initial delay");
        bDoWhenPressStopOverUseInitialDelay=false;
        bflag=true;
    }

    if(bDoAfterAutoCleanFunctionUseInitialDelay)
    {
        RecordProcess("Reset by After auto clean use initial delay");
        bDoAfterAutoCleanFunctionUseInitialDelay=false;
        bflag=true;
    }

    if(bDoAfterShowAlarmMessageUseInitialDelay)
    {
        RecordProcess("Reset by After show alarm message use initial delay");
        bDoAfterShowAlarmMessageUseInitialDelay=false;
        bflag=true;
    }

    if(bDoAfterOpenHeatDoorUseInitialDelay)
    {
        RecordProcess("Reset by After open heat door use initial delay");
        bDoAfterOpenHeatDoorUseInitialDelay=false;
        bflag=true;
    }

    if(bDoWhenHappenTestedTimeBlowUseInitialDelay)
    {
        RecordProcess("Reset by When happen tested time blow use initial delay");
        bDoWhenHappenTestedTimeBlowUseInitialDelay=false;
        bflag=true;
    }

    if(bDoEveryFirstDeviceFunctionUseInitialDelay)
    {
        bDoEveryFirstDeviceFunctionUseInitialDelay=false;
        RecordProcess("Reset by Every fisrt device delay use initial delay");
        bflag=true;
    }

    if(bDoWhenNoFullSiteUseInitialDelay)
    {
        RecordProcess("Reset by When no fullSite use initial delay");
        bDoWhenNoFullSiteUseInitialDelay=false;
        bflag=true;
    }

    if(bDoOTDOffUseInitialDelay)
    {
        RecordProcess("Reset by OTD unlock use initial delay");
        bDoOTDOffUseInitialDelay=false;
        bflag=true;
    }
    if(bTestStartToNextTestStartDelay)                                          //kevin 20181102
    {
        RecordProcess("Reset by SOTk use initial delay");
        bTestStartToNextTestStartDelay=false;
        bflag=true;
    }
    return bflag;
}
//------------------------------------------------------------------------------
int CheckToBoostIndexTemp()                                                     //Steven 20180817 : Boost Function
{
    int QQ=-1;
    double dEotToSotTime=TestIntervalsBoostTime.LatchCycleTime()/1000.0;        //JerryYang 20181122 (Steven) :  (Steven) : 將不同function計時器分開
    if(((ATC_SYSTEM==eNewATCSystem &&
         ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60) ||
        CosFunction.bNonATCSupportLBFunction) &&                                //JerryYang 20220126: non ATC也支援LB function
       (Temperature.bBoostFuncttion ||                                          //Steven 20190627 : Boost func加上保護
        Temperature.bLBTempFunction))                                           //Steven 20181102 : LB Temp Function
    {
//        if(bUnderTest)
//        {
//            QQ=-1;
//        }
//        else
//        {
            if(Temperature.bLBTempFunction)
            {
                if(bLBBoostTimeOut==false && UN150Read[tcLB]<Temperature.dBoostIdleTime[Temperature.eBLBL])
                {
                    QQ=4;
                }
//                else if(Temperature.bEnableBoostOffset[Temperature.eBLBB] && UN150Read[tcLB]<Temperature.dBoostIdleTime[Temperature.eBLBB])
//                {
//                    QQ=5;
//                }
                else if(Temperature.bEnableBoostOffset[Temperature.eBLBI] && dEotToSotTime>=Temperature.dBoostIdleTime[Temperature.eBLBI])
                {
                    QQ=3;
                }
            }
            else if(bBoostFirstBoost)
            {
                bBoostFirstBoost=false;
                QQ=0;
            }
            else if(dEotToSotTime>=Temperature.dBoostIdleTime[Temperature.eBMax])
            {
                QQ=0;
            }
            else if(dEotToSotTime>Temperature.dBoostIdleTime[Temperature.eBMin])
            {
                QQ=1;
            }
            else if(dEotToSotTime==Temperature.dBoostIdleTime[Temperature.eBMin])
            {
                QQ=2;
            }
            else
            {
                QQ=-1;
            }
//        }
    }

//    iBoostFuncStep=0;

    if(bUnderTest && QQ!=-1)
    {
        QQ=-1;
    }

    return QQ;
}
//------------------------------------------------------------------------------
//kevin 20190906 add  Load cell for ASKH
//------------------------------------------------------------------------------
void DoTestHeadMotorLoadCell()
{
//    static int iRetryCount=0;
    //static bool bOneTimeFlag=true;
    //static DWORD startTick=-1, endTick, nowTick;
    //static int iToqureCount=0;
    static bool bFirstTime=true;
//    static AnsiString ErrPart="";
    //bool TMode=false;
    //static int iSiteCount=0;
    int &Task=iTestHeadMotorTask;//, iColCounts=0, iRowCounts=0;
//    int i, j;//, ret, TorqueData, iIndexUpPos=0, ccRet;
//    bool flag=false;//, flag2, bIndexSuckCheck;
//    int sp, iMaxPreasure=0, iIndexArm[3]={0, 0, 0};
    AnsiString str;
//    static char  aLoadCell[4][8]={'0'};   //kevin 20190306 add
    AnsiString SData="@e02019Arm1,sideA";

    switch(Task)
    {
        case 1: //確認Index Arm 吸嘴狀態
            if(Prod.TestZ1_Test==0)
                iSocketSenSosPos1=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos1=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000;

            if(Prod.TestZ2_Test==0)
                iSocketSenSosPos2=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos2=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000;

            if(bUseInitTempOffset)                                              //Steven 20141117 : 起測時溫度要補Offset
            {
                RecordProcess("After index check trigger initial offset function.(Temp)");
                iInitContactCount=0;                                            //Steven 20141117 : 起測時溫度要補Offset
                fHeaterOK=false;
            }

            if(INSTALL_SOCKET_CLAMP)                                            //JerryYang 20160607 機台選用分離機構 需偵測socket sensor
            {
                if(Sen[SnSocketHasClamp1].IsOn() || Sen[SnSocketHasClamp2].IsOn())
                {
                    ShowMyMessage("Please check the socket sensor","socket sensor偵測異常");
                    break;
                }
            }

            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //kevin 20190318 use arm 2
                Task=400;
            else
                Task=200;
            break;
        case 200:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotorLoadCell 200"))
            {
                Task=210;
            }
            break;
        case 210:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotorLoadCell 210"))
            {
                Task=215;
            }
            break;
        case 215:            //IndexZ1 and IndexZ2 皆移至安全位置
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotorLoadCell 215"))
            {
                Task=216;
            }
            break;
        case 216: //判斷IndexZ1 and IndexZ2 是否已在安全位置
            Task=260;
            break;
        case 260:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.iLoadCellY1, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotorLoadCell 260")) //ARM 1 on load cell
            {
                Task=290;
            }
            break;
        case 290://120: //Arm 1 on load cell DOWN
            if(MOT[MTestZ1].Gali_MotMove(Prod.dLoadCellZ1Down, iSpeedSlow))
            {
                IndexStatus=Z1Down_Z2Up;
                bloadcellRece=false;
                SData="@e02019Arm1,sideA";
                RespondASECom(SData);       //kevin 20190906
                Task=312;
                DoTestHeadMotorDelay.SetSecAndOn(5);//wait 3 SEC
                bFirstTime=true;
                bloadcellRece=false;
                sLoadCellReceData="";
                iloadcellRece=0;           //kevin 20190906
            }
            break;
        case 312://12102:
            if(bloadcellRece)   //kevin 20190906
            {
                Task=313;
                if(iloadcellRece==2)                  //kevin 20190906 add load cell command
                {
                    //kevin 20190906 add load cell NG
                    ShowMessage(sLoadCellReceData);
                    iloadcellRece=0;
                }
            }
            break;
        case 313: //12110:   kevin 判斷LOAD CEEL 訊號是否 PASS FAIL
            Task=314;
            break;
        case 314://12111: //kevin 20130418
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotorLoadCell 314"))
                Task=315;
            break;
        case 315://12112:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.iLoadCellY1+TestIF.dSiteYPitch, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotorLoadCell 315")) //ARM 1 on load cell+ ypitch
            {
                Task=316;
            }
            break;
        case 316://12200:     //Arm 1 on load cell DOWN
            if(MOT[MTestZ1].Gali_MotMove(Prod.dLoadCellZ1Down, iSpeedSlow))
            {
                IndexStatus=Z1Down_Z2Up;
                Task=317;
                SData="@e02019Arm1,sideB";
                RespondASECom(SData);       //kevin 20190906
                bloadcellRece=false;
                sLoadCellReceData="";
                iloadcellRece=0;           //kevin 20190906
                DoTestHeadMotorDelay.SetSecAndOn(5);//wait 3 SEC
                bFirstTime=true;
                //kevin 送output 訊號給load cell
            }
             //SW[SwLoadCellB].On();     //kevin 送output 訊號給load cell
            break;
        case 317:
            if(bloadcellRece)   //kevin 20190906
            {
                Task=318;

                if(iloadcellRece ==2)                  //kevin 20190906 add load cell command
                {
                       //kevin 20190906 add load cell NG
                    ShowMessage(sLoadCellReceData);
                    iloadcellRece=0;
                }
            }
            break;
        case 318: //12110:   kevin 判斷LOAD CEEL 訊號是否 PASS FAIL
            Task=319;
            break;
        case 319://12111:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotorLoadCell 319"))
                Task=400;
            break;
        case 400:// arm 2
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotorLoadCell 400"))
                Task=401;
            break;
        case 401://122110:
             SW[SwLoadCellA].Off();
             SW[SwLoadCellB].Off();
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.iLoadCellY2, iSpeedY, "DoTestHeadMotorLoadCell 401"))
                Task=402;
            break;
        case 402://140: Arm 2 on load cell
            if(MOT[MTestZ2].Gali_MotMove(Prod.dLoadCellZ2Down, iSpeedSlow))
            {

                  IndexStatus=Z1Up_Z2Down;
                  Task= 403;//14110;
                  SData="@e02019Arm2,sideB";
                  RespondASECom(SData);       //kevin 20190906
                  bloadcellRece=false;
                  sLoadCellReceData="";
                  iloadcellRece=0;           //kevin 20190906
                  DoTestHeadMotorDelay.SetSecAndOn(5);
                  bFirstTime=true;
                  //kevin 20190306 送訊號給load cell
            }
            //SW[SwLoadCellA].On();
            break;
        case 403://14102:                         //Frank 20171030 (Steven) add Floating Shuttle Read Torque Delay
            /*
            if(DoTestHeadMotorDelay.Off())
            {
                Task= 404;
                if(Sen[SnLoadCell1].Status()==true)
                    aLoadCell[2][0]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell2].Status()==true)
                    aLoadCell[2][1]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell3].Status()==true)
                    aLoadCell[2][2]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell4].Status()==true)
                    aLoadCell[2][3]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell5].Status()==true)
                    aLoadCell[2][4]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell6].Status()==true)
                    aLoadCell[2][5]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell7].Status()==true)
                    aLoadCell[2][6]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell8].Status()==true)
                    aLoadCell[2][7]='P';   //kevin 20190306 add

                SW[SwLoadCellA].Off();     //kevin 送output 訊號給load cell
            } */
            if(bloadcellRece)   //kevin 20190906
            {
                Task= 404;

                if(iloadcellRece ==2)                  //kevin 20190906 add load cell command
                {
                    iloadcellRece=0;
                       //kevin 20190906 add load cell NG
                    ShowMessage(sLoadCellReceData);
                }
            }
            break;
        case 404: //12110:   kevin 判斷LOAD CEEL 訊號是否 PASS FAIL
            Task=405;
            break;

       case 405://12111: //kevin 20130418
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotorLoadCell 405"))
                Task=406;
            break;
        case 406://12112:             //ARM 2 on load cell+ ypitch
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.iLoadCellY2-TestIF.dSiteYPitch, iSpeedY, "DoTestHeadMotorLoadCell 406"))
            {
                Task=407;
            }
            break;
        case 407://12200:     //Arm 1 on load cell DOWN
            if(MOT[MTestZ2].Gali_MotMove(Prod.dLoadCellZ2Down, iSpeedSlow))
            {
                bloadcellRece=false;   //kevin 20190906
                IndexStatus=Z1Up_Z2Down;
                Task=408;
                SData="@e02019Arm1,sideB";
                RespondASECom(SData);       //kevin 20190906
                bloadcellRece=false;
                sLoadCellReceData="";
                iloadcellRece=0;           //kevin 20190906
                DoTestHeadMotorDelay.SetSecAndOn(5);//wait 3 SEC
                //kevin 送output 訊號給load cell
            }
            //SW[SwLoadCellA].Off();     //kevin 送output 訊號給load cell
            break;
        case 408:

           /*
            if(DoTestHeadMotorDelay.Off())
            {
                Task= 409;
                 if(Sen[SnLoadCell1].Status()==true)
                    aLoadCell[3][0]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell2].Status()==true)
                    aLoadCell[3][1]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell3].Status()==true)
                    aLoadCell[3][2]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell4].Status()==true)
                    aLoadCell[3][3]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell5].Status()==true)
                    aLoadCell[3][4]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell6].Status()==true)
                    aLoadCell[3][5]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell7].Status()==true)
                    aLoadCell[3][6]='P';   //kevin 20190306 add
                if(Sen[SnLoadCell8].Status()==true)
                    aLoadCell[3][7]='P';   //kevin 20190306 add

                SW[SwLoadCellB].Off();     //kevin 送output 訊號給load cell
            }*/
            if(bloadcellRece)   //kevin 20190906
            {
                Task=409;

                if(iloadcellRece ==2)                  //kevin 20190906 add load cell command
                {
                       //kevin 20190906 add load cell NG
                    ShowMessage(sLoadCellReceData);
                }
            }
            break;
        case 409: //12110:   kevin 判斷LOAD CEEL 訊號是否 PASS FAIL
            Task= 410;
            break;
        case 410://12111:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoTestHeadMotorLoadCell 410"))
                Task=411;
            break;
        case 411:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoTestHeadMotorLoadCell 411"))
            {
                bLoadCellTest=false;
                bIsAutoOneCycle=true;
                fMain->BtnOneCycleClick(fMain->BtnOneCycle);
                //fMain->Pause();
                Task=1;
            }
            break;
    }
}
//==============================================================================
void SetTestTimeOutTimer(int Index)                                             //Steven 20200407 : 整合Time Out時間設定
{
    if(Index==0)
    {
        if(bInitialMaxTime==true)                                               //jou 2011-11-09 增加initial max time set
        {
            hFTestTimeOutDelay.SetSecAndOn(TestIF.iInitialMaxTime);
        }
        else
        {
            hFTestTimeOutDelay.SetSecAndOn(TestIF.iMaxTime);
        }
    }
    else if(Index==1)
    {
        if(bInitialMaxTime==true)
        {
            hBTestTimeOutDelay.SetSecAndOn(TestIF.iInitialMaxTime);
        }
        else
        {
            hBTestTimeOutDelay.SetSecAndOn(TestIF.iMaxTime);
        }
    }
//    else                                                                      //Steven 20210827 : Test Time Out要跟Index Arm同步設定
//    {
        if(bInitialMaxTime==true)                                               //Steven 20190119 : 避免Time out retry -> pause後,沒送出SOT會再發Time out.
        {
            TestTimeOut.SetSecAndOn(TestIF.iInitialMaxTime+10);
        }
        else
        {
            TestTimeOut.SetSecAndOn(TestIF.iMaxTime+10);
        }
//    }
}
//==============================================================================
bool CheckSocketSensor(int iArm, AnsiString Func, bool bInit, bool bCheckArmHieght)        //Steven 20200615 : Socket Sensor整合成Function
{                                                                               //jimmychiu 20230830 : add switch check arm height in check socket sensor function
    static int iSocketSensorCT[2][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

    int iAZ1=0, iAZ2=0;
    bool bHasErr=false, bArmHeightIsEnough=false;
    AnsiString str1, sBufferT;

    if(bInit)
    {
        iShowSocketSensor=0;
        for(int i=0; i<TestIF_File.iSocketCount; i++)
            iSocketSensorCT[iArm][i]=0;
    }
    sSocketSensorErr="";

    if(IniConfig.bC08_SocketSensor && TestIF_File.bEnSocketSensor)              //kevin 20130504 socket sensor detect error
    {
        if(bCheckArmHieght==true)                                               //jimmychiu 20230830 : add switch check arm height in check socket sensor function
        {
            iAZ1=MOT[MTestZ1].Gali_ReadEncoderPos();                            //JerryYang 20190327 fix socket sensor誤判問題, 要讀Encoder位置
            iAZ2=MOT[MTestZ2].Gali_ReadEncoderPos();

            if(Prod.TestZ1_Test==0)
                iSocketSenSosPos1=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos1=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000;

            if(Prod.TestZ2_Test==0)
                iSocketSenSosPos2=-1000;                                        //kevin 20150613 關arm 設定可判斷位置
            else
                iSocketSenSosPos2=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000;
                bArmHeightIsEnough=(iAZ1>iSocketSenSosPos1) && (iAZ2>iSocketSenSosPos2);
        }
        else
        {
            bArmHeightIsEnough=true;
        }

        if(bArmHeightIsEnough)                                                  //kevin 20140508 socket sensor
        {
            if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
               TestIF_File.bArm1PickPlaceArm2Test==true)                        //Steven 20200604 : Arm1丟 Arm2測的時候, Arm2只檢查置偏
            {
                if(bHasICinSocket)
                {
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(TestIF_File.iSensorCheckType[i]==2 &&
                           Sen[SThreadPara.iSocketSensor[i]].Enable &&
                           Sen[SThreadPara.iSocketSensor[i]].IsOn())            //On的有置偏
                        {
                            MOT[MTestZ1+iArm].Gali_Command("ST", Func+AnsiString("1"));
                            bHasErr=true;
                            iShowSocketSensor=2;
                            sSocketSensorErr+=IntToStr(i+1)+",";
                        }
                    }
                }
                else
                {
                    for(int i=0; i<TestIF_File.iSocketCount; i++)
                    {
                        if(TestIF_File.iSensorCheckType[i]==1 &&
                           Sen[SThreadPara.iSocketSensor[i]].Enable &&
                           Sen[SThreadPara.iSocketSensor[i]].IsOn())            //On的有殘料
                        {
                            MOT[MTestZ1+iArm].Gali_Command("ST", Func+AnsiString("2"));
                            bHasErr=true;
                            iShowSocketSensor=1;
                            sSocketSensorErr+=IntToStr(i+1)+",";
                        }
                    }
                }
            }
            else
            {
                for(int i=0; i<TestIF_File.iSocketCount; i++)
                {
                    if(Sen[SThreadPara.iSocketSensor[i]].Enable &&
                       Sen[SThreadPara.iSocketSensor[i]].IsOn())                //On的有殘料
                    {
                        MOT[MTestZ1+iArm].Gali_Command("ST", Func+AnsiString("3"));
                        bHasErr=true;
                        iShowSocketSensor=1;
                        sSocketSensorErr+=IntToStr(i+1)+",";
                    }
                }
            }

            if(bHasErr)
            {
                if(REAL_TIME_CCD==true)
                {
                    ScanBtnThd->Stop();
                }

                str1.sprintf("%s, iShowSocketSensor:%d, %s", Func, iShowSocketSensor, sSocketSensorErr);
                if(iShowSocketSensor<1)
                    iShowSocketSensor=1;

                RecordProcess(str1);
                if(iArm==0)
                    sBufferT="Z1UpZ2Down: Z1Pos socket sensor detect error"+IntToStr(iAZ1)+">"+IntToStr(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000)+" Z2Pos "+IntToStr(iAZ2)+">"+IntToStr(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000);
                else
                    sBufferT="Z1DownZ2Up: Z1Pos "+IntToStr(iAZ1)+">"+IntToStr(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000)+" Z2Pos "+IntToStr(iAZ1)+">"+IntToStr(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000);

                RecordProcess(sBufferT);//kevin 20150506
            }
        }

//        if(CUSTOMER_CODE==CC_JCET &&                                          //Steven 20230214 : 強制要檢查Socket Sensor有沒有遮斷的效果
        if(bHasErr==false)                                                      //jou 2016-10-03 江陰長電要求Socket sensor要real time檢查遮斷效果,下壓的時候必須on
        {
            if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
               TestIF_File.bArm1PickPlaceArm2Test==true)                        //Steven 20200604 : Arm1丟 Arm2測的時候, Arm2只檢查置偏
            {
                if(bCheckArmHieght==true)                                       //jimmychiu 20230830 : add switch check arm height in check socket sensor function
                {
                    iAZ1=MOT[MTestZ1].Gali_ReadEncoderPos();                    //JerryYang 20190327 fix socket sensor誤判問題, 要讀Encoder位置
                    iAZ2=MOT[MTestZ2].Gali_ReadEncoderPos();
                    if(Prod.TestZ1_Test==0)
                        iSocketSenSosPos1=-1000;                                //kevin 20150613 關arm 設定可判斷位置
                    else
                        iSocketSenSosPos1=Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+2000;

                    if(Prod.TestZ2_Test==0)
                        iSocketSenSosPos2=-1000;                                //kevin 20150613 關arm 設定可判斷位置
                    else
                        iSocketSenSosPos2=Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+2000;
                    bArmHeightIsEnough=(iAZ1>iSocketSenSosPos1) && (iAZ2>iSocketSenSosPos2);
                }
                else
                {
                    bArmHeightIsEnough=true;
                }

                if(bArmHeightIsEnough)                                          //jimmychiu 20230830 : add switch check arm height in check socket sensor function
                {
                    if(bHasICinSocket)
                    {
                        for(int i=0; i<TestIF_File.iSocketCount; i++)
                        {
                            if(TestIF_File.iSensorCheckType[i]==2 &&
                               Sen[SThreadPara.iSocketSensor[i]].Enable &&
                               Sen[SThreadPara.iSocketSensor[i]].IsOn())        //On的有至偏
                            {
                                iSocketSensorCT[iArm][i]++;
                                if(iSocketSensorCT[iArm][i]>=1)
                                {
                                    iSocketSensorCT[iArm][i]=0;
                                    MOT[MTestZ1+iArm].Gali_Command("ST", Func+AnsiString("4"));
                                    bHasErr=true;
                                    iShowSocketSensor=2;
                                    sSocketSensorErr+=IntToStr(i+1)+",";
                                }
                            }
                        }
                    }
                    else
                    {
                        for(int i=0; i<TestIF_File.iSocketCount; i++)
                        {
                            if(TestIF_File.iSensorCheckType[i]==1 &&
                               Sen[SThreadPara.iSocketSensor[i]].Enable &&
                               Sen[SThreadPara.iSocketSensor[i]].IsOn())        //On的有殘料
                            {
                                iSocketSensorCT[iArm][i]++;
                                if(iSocketSensorCT[iArm][i]>=1)
                                {
                                    iSocketSensorCT[iArm][i]=0;
                                    MOT[MTestZ1+iArm].Gali_Command("ST", Func+AnsiString("5"));
                                    bHasErr=true;
                                    iShowSocketSensor=1;
                                    sSocketSensorErr+=IntToStr(i+1)+",";
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(iArm==0)
                {
                    if(IniConfig.bD30EnableSiteModeSelect &&
                       (TestIF.iShuttleMode==0 ||
                        (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)))
                    {
                        if(bCheckArmHieght==true)
                        {
                            bArmHeightIsEnough=(iAZ1<=(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset));
                        }
                        else
                        {
                            bArmHeightIsEnough=true;
                        }

                        if(bArmHeightIsEnough)                                  //kevin 20140508 socket sensor
                        {
                            sSocketSensorErr="index check Socket Sensor down sensor off,";
                            for(int i=0; i<TestIF_File.iSocketCount; i++)
                            {
                                if(((i==0 && FTestSuck.ArmRow0HaveRealIC()) ||
                                    (i==1 && FTestSuck.ArmRow1HaveRealIC())) && //Steven 20230410 : 單排關site, socket sensor會誤判
                                   FTestSuck.HasRealIC() &&
                                   Sen[SThreadPara.iSocketSensor[i]].Enable &&
                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())
                                {
                                    iSocketSensorCT[iArm][i]++;
                                    if(iSocketSensorCT[iArm][i]>=1)
                                    {
                                        iSocketSensorCT[iArm][i]=0;
                                        MOT[MTestZ1+iArm].Gali_Command("ST", Func+AnsiString("6"));
                                        bHasErr=true;
                                        iShowSocketSensor=1;
                                        sSocketSensorErr+=IntToStr(i+1)+",";
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(IniConfig.bD30EnableSiteModeSelect &&
                       (TestIF.iShuttleMode==0 ||
                        (TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)))
                    {
                        if(bCheckArmHieght==true)                               //jimmychiu 20230830 : add switch check arm height in check socket sensor function
                        {
                            bArmHeightIsEnough=(iAZ2<=(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset));
                        }
                        else
                        {
                            bArmHeightIsEnough=true;
                        }

                        if(bArmHeightIsEnough)                                  //kevin 20140508 socket sensor
                        {
                            sSocketSensorErr="index check Socket Sensor down sensor off,";
                            for(int i=0; i<TestIF_File.iSocketCount; i++)
                            {
                                if(((i==0 && BTestSuck.ArmRow0HaveRealIC()) ||  //Richard 20230417 : FTestSuck>BTestSuck
                                    (i==1 && BTestSuck.ArmRow1HaveRealIC())) && //Steven 20230410 : 單排關site, socket sensor會誤判
                                   BTestSuck.HasRealIC() &&
                                   Sen[SThreadPara.iSocketSensor[i]].Enable &&
                                   Sen[SThreadPara.iSocketSensor[i]].IsOff())
                                {
                                    iSocketSensorCT[iArm][i]++;
                                    if(iSocketSensorCT[iArm][i]>=1)
                                    {
                                        iSocketSensorCT[iArm][i]=0;
                                        MOT[MTestZ2].Gali_Command("ST", Func+AnsiString("7"));
                                        bHasErr=true;
                                        iShowSocketSensor=1;
                                        sSocketSensorErr+=IntToStr(i+1)+",";
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if(bHasErr)
            {
                if(REAL_TIME_CCD==true)
                {
                    ScanBtnThd->Stop();
                }
                str1.sprintf("%s_real time, iShowSocketSensor:%d, %s", Func, iShowSocketSensor, sSocketSensorErr);
                RecordProcess(str1);
                if(iShowSocketSensor<1)
                    iShowSocketSensor=1;
            }
        }
    }
    return bHasErr;
}
//==============================================================================
void CheckICExistInSocket(AnsiString Func)                                      //Jimmychiu 20250827 : Gigas 要求加入Index下壓時確認IC存在
{
    if(CosFunction.bSocketSensorCheckICAtArmDown==false ||
       IniConfig.bD82CheckIndexHasIC==false ||
       LastSet.iRealDummy!=REALLY)
    {
        return;
    }

    #ifdef SOFT_SIMULTE
    return;
    #else
    for(int i=0; i<TestIF_File.iSocketCount; i++)
    {
        if(TestIF_File.iSensorCheckType[i]==1 &&
           Sen[SThreadPara.iSocketSensor[i]].Enable &&
           Sen[SThreadPara.iSocketSensor[i]].IsOff()==false)                    //帶著IC下壓Off的有殘料
        {
            return;                                                             //Gigas要求只要有一顆Sensor亮就過關
        }
    }
    AnsiString str=AnsiString().sprintf("All socket sensors are off when the index arm goes down and picks up the IC. %s",Func);
    ShowMyMessage(str);
    #endif
}
//==============================================================================
int iDoIndexSocketCheckTask=1;
TQPF_Timer ESCDelay;
int iESCError=0;
//==============================================================================
void InitialIndexSocketCheckTask()                                              //Steven 20201022 : For RFMD Empty Socket Check Funstion.
{
    iDoIndexSocketCheckTask=1;
    iESCError=0;
}
//==============================================================================
void DoIndexSocketCheck()                                                       //Steven 20201022 : For RFMD Empty Socket Check Funstion.
{
    int &Task=iDoIndexSocketCheckTask;

    if(iOneCycle==0)
    {
        bDoEmptySocketOneCycle=false;
    }

    switch(Task)
    {
        case 1:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(0, iSpeedSlow))
            {
                Task=10;
            }
            break;
        case 10:
            if(IniConfig.bD30EnableSiteModeSelect && TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedSlow, __FUNC__))
                {
                    ESCDelay.SetSecAndOn(0.5);
                    Task=20;
                }
            }
            else
            {
                if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedSlow, __FUNC__))
                {
                    ESCDelay.SetSecAndOn(0.5);
                    Task=50;
                }
            }
            break;
        case 20:
            if(ESCDelay.Off())
            {
                Task=30;
            }
            break;
        case 30:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                ESCDelay.SetSecAndOn(0.5);
                Task=200;
            }
            break;
        case 50:
            if(ESCDelay.Off())
            {
                Task=60;
            }
            break;
        case 60:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset, iSpeedSlow))
            {
                ESCDelay.SetSecAndOn(0.5);
                Task=200;
            }
            break;
        case 200:
            if(ESCDelay.Off())
            {
                if(LastSet.iTester==0)
                {
                    Task=500;
                    break;
                }
//                ESCDelay.SetSecAndOn(iMaxTestWaitTime);
                SetTestTimeOutTimer(0);
                iGetESCResult=0;
                fMain->SendMSG_CMD(MSG_CMD_ESC);
                Task=300;
            }
            break;
        case 300:
            if(iGetESCResult==1)
            {
                Task=400;
            }
            else if(iGetESCResult==2)
            {
                iESCError=1;
                Task=400;
            }
            else if(TestTimeOut.Off())
            {
                iESCError=2;
                Task=400;
            }
            break;
        case 400:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(0, iSpeedSlow))
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedSlow, __FUNC__))
            {
                if(iESCError!=0)
                    Task=600;
                else
                    Task=700;
            }
            break;
        case 600:
            if(iESCError==1)
            {
                ShowErrorMessage("WAR0352", K_SKIP, MMIndex);
            }
            else
            {
                ShowErrorMessage("WAR0353", K_SKIP, MMIndex);
            }
            COM2->SendCommToVision(COM2->rtInspEnd, false);  //Jimmychiu 20231130 : Fixed for #P231120-ATK-H9-04 , V3.32.810_Beta & V3.32.811_Beta , RTC2 Communication error
            COM2->InitRealTimeCCDPara();                     //Jimmychiu 20231130 : Fixed for #P231120-ATK-H9-04 , V3.32.810_Beta & V3.32.811_Beta , RTC2 Communication error
            bDoEmptySocketCheck=false;
            break;
        case 700:
            iESCError=0;
            bDoEmptySocketCheck=false;
            RecordProcess("EmptySocketCheck Finish");
            COM2->SendCommToVision(COM2->rtInspEnd, false);  //Jimmychiu 20231130 : Fixed for #P231120-ATK-H9-04 , V3.32.810_Beta & V3.32.811_Beta , RTC2 Communication error
            COM2->InitRealTimeCCDPara();                     //Jimmychiu 20231130 : Fixed for #P231120-ATK-H9-04 , V3.32.810_Beta & V3.32.811_Beta , RTC2 Communication error
            return;
    }
}
//<==
//Steven 20201022 : For RFMD Empty Socket Check Funstion.
//------------------------------------------------------------------------------
int iDoHalfViewAllPassVerifyTask=1;
bool DoHalfViewAllPassVerifyRTC(bool bInitial)  //JerryYang 20220215 : RTC Auto Verify half view check
{
    if(bInitial==true)
    {
        iDoHalfViewAllPassVerifyTask=1;
        return false;
    }
    bool TMode=false;

    int &Task=iDoHalfViewAllPassVerifyTask;
    switch(Task)
    {
        case 1:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoHalfViewAllPassVerifyRTC 1"))
            {
                Task=100;
            }
            break;
        case 100:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoHalfViewAllPassVerifyRTC 1010"))
            {
                if(iContactMode==CONTACT_NORMAL)
                {
                    bHalfViewVerifyNeedAboveSocket=false;
                    DoSetupSystemToProd();
                }
                else
                {
                    Prod.TestZ1_Test=Get0_01MMType(fContact->edContactHeight1->Text.c_str())+Get0_01MMType(fContact->edDropOffset1->Text.c_str());
                    Prod.TestZ2_Test=Get0_01MMType(fContact->edContactHeight2->Text.c_str())+Get0_01MMType(fContact->edDropOffset2->Text.c_str());
                }
                Task=200;
            }
            break;
        case 200:
            if(iContactMode==CONTACT_NORMAL)
            {
                bHalfViewVerifyNeedAboveSocket=false;
                DoSetupSystemToProd();
            }
            else
            {
                Prod.TestZ1_Test=Get0_01MMType(fContact->edContactHeight1->Text.c_str())+Get0_01MMType(fContact->edDropOffset1->Text.c_str());
                Prod.TestZ2_Test=Get0_01MMType(fContact->edContactHeight2->Text.c_str())+Get0_01MMType(fContact->edDropOffset2->Text.c_str());
            }
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Up_Z2Down;
                Task=300;
            }
            break;
        case 300:   //通知Half view auto verify : Arm1
            COM2->SendCommToVision(COM2->rtArm1AllPassVerify, true);
            DoTestHeadMotorDelay.SetSecAndOn(10);
            DoTestHeadMotorDelay3.SetSecAndOn(0.5);
            Task=400;
            break;
        case 400:
//            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtArm1AllPassVerify])
            if(Sen[SnRealTimeCCDIndexArm].IsOff())
            {
                if(DoTestHeadMotorDelay3.Off())
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSOK]=false;
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSNG]=false;
                    Task=500;
                }
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                ShowMyMessage("RTC Arm 1 Half Veiw all pass verify timeout");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 500:
            if(DeviceForm.ContactMode==TMove ||
               DeviceForm.ContactMode==TMoveDrop ||  //jou 2012-02-03 新增T Move Drop
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)    //Steven 20160130 : TMove Soft contact
            {
                TMode=true;
            }
            else
            {
                //jou 2012-02-29 index pick up error,index arm move to center & alarm
                if(IniConfig.bIndexPickupErrStop==true && bIndexArm2PickupErrStop==true)
                {
                    TMode=true;
                }
                else
                {
                    TMode=false;
                }
            }

            if(MOT[MTestY1].Z1DownZ2Up(MOT[MTestZ1].GailSpeed, TMode, false))
            {
                Task=600;
            }
            break;
        case 600:   //等待RTC回傳verify結果
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSOK])
            {
                Task=700;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSNG])
            {
                Task=41400;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                COM2->SendCommToVision(COM2->rtArmFinish, false);
                ShowMyMessage("RTC Verify All Pass Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 700:   //通知Half view auto verify : Arm2
            COM2->SendCommToVision(COM2->rtArm2AllPassVerify, true);
            DoTestHeadMotorDelay.SetSecAndOn(10);
            DoTestHeadMotorDelay3.SetSecAndOn(0.5);
            Task=800;
            break;
        case 800:
//            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtArm2AllPassVerify])
            if(Sen[SnRealTimeCCDIndexArm].IsOn())
            {
                if(DoTestHeadMotorDelay3.Off())
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSOK]=false;
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSNG]=false;
                    Task=900;
                }
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                ShowMyMessage("RTC Arm 2 Half Veiw all pass verify timeout");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 900:
            if(MOT[MTestY1].Z1UpZ2Down(MOT[MTestZ2].GailSpeed, TMode, false))
            {
                Task=1000;
            }
            break;
        case 1000:   //等待RTC回傳verify結果
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSOK])
            {
                Task=1100;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLPASSNG])
            {
                Task=41400;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                COM2->SendCommToVision(COM2->rtArmFinish, false);
                ShowMyMessage("RTC Verify All Pass Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 1100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoHalfViewAllPassVerifyRTC 1100"))
            {
                Task=1200;
            }
            break;
        case 1200:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoHalfViewAllPassVerifyRTC 1200"))
            {
                Task=1;
                return true;
            }
            break;
        case 41400:
            if(IndexAlarmInArmAway()==true)
            {
                bIsTestSitICFallDown=true;
                ShowMyMessage("RTC Half Veiw Verify All Pass NG!!");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
    }
    return false;
}
//==============================================================================
int iDoHalfViewAllFailVerifyTask=1;
bool DoHalfViewAllFailVerifyRTC(bool bInitial)
{
    if(bInitial==true)
    {
        iDoHalfViewAllFailVerifyTask=1;
        return false;
    }
    bool TMode=false;

    int &Task=iDoHalfViewAllFailVerifyTask;
    switch(Task)
    {
        case 1:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoHalfViewAllFailVerifyRTC 1"))
            {
                Task=100;
            }
            break;
        case 100:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, "DoHalfViewAllFailVerifyRTC 100"))
            {
                if(iContactMode==CONTACT_NORMAL)
                {
                    bHalfViewVerifyNeedAboveSocket=true;
                    DoSetupSystemToProd();
                }
                else
                {
                    Prod.TestZ1_Test=Get0_01MMType(fContact->edContactHeight1->Text.c_str())+Get0_01MMType(fContact->edDropOffset1->Text.c_str())+3000;
                    Prod.TestZ2_Test=Get0_01MMType(fContact->edContactHeight2->Text.c_str())+Get0_01MMType(fContact->edDropOffset2->Text.c_str())+3000;
                }
                Task=200;
            }
            break;
        case 200:
            if(iContactMode==CONTACT_NORMAL)
            {
                bHalfViewVerifyNeedAboveSocket=true;
                DoSetupSystemToProd();
            }
            else
            {
                Prod.TestZ1_Test=Get0_01MMType(fContact->edContactHeight1->Text.c_str())+Get0_01MMType(fContact->edDropOffset1->Text.c_str())+3000;
                Prod.TestZ2_Test=Get0_01MMType(fContact->edContactHeight2->Text.c_str())+Get0_01MMType(fContact->edDropOffset2->Text.c_str())+3000;
            }
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset, iSpeedSlow))
            {
                IndexStatus=Z1Up_Z2Down;
                Task=300;
            }
            break;
        case 300:   //通知Half view auto verify : Arm1
            COM2->SendCommToVision(COM2->rtArm1AllFailVerify, true);
            DoTestHeadMotorDelay.SetSecAndOn(10);
            DoTestHeadMotorDelay3.SetSecAndOn(0.5);
            Task=400;
            break;
        case 400:
//            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtArm1AllFailVerify])
            if(Sen[SnRealTimeCCDIndexArm].IsOff())
            {
                if(DoTestHeadMotorDelay3.Off())
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILOK]=false;
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILNG]=false;
                    Task=500;
                }
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                ShowMyMessage("RTC Arm 1 Half Veiw all fail verify timeout");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 500:
            if(DeviceForm.ContactMode==TMove ||
               DeviceForm.ContactMode==TMoveDrop ||  //jou 2012-02-03 新增T Move Drop
               DeviceForm.ContactMode==TMoveDropSlowContact ||
               DeviceForm.ContactMode==TMoveSlowContact)    //Steven 20160130 : TMove Soft contact
            {
                TMode=true;
            }
            else
            {
                //jou 2012-02-29 index pick up error,index arm move to center & alarm
                if(IniConfig.bIndexPickupErrStop==true && bIndexArm2PickupErrStop==true)
                {
                    TMode=true;
                }
                else
                {
                    TMode=false;
                }
            }

            if(MOT[MTestY1].Z1DownZ2Up(MOT[MTestZ1].GailSpeed, TMode, false))
            {
                Task=600;
            }
            break;
        case 600:   //等待RTC回傳verify結果
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILOK])
            {
                Task=700;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILNG])
            {
                Task=41400;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                COM2->SendCommToVision(COM2->rtArmFinish, false);
                ShowMyMessage("RTC Verify All Fail Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 700:   //通知Half view auto verify : Arm2
            COM2->SendCommToVision(COM2->rtArm2AllFailVerify, true);
            DoTestHeadMotorDelay.SetSecAndOn(10);
            DoTestHeadMotorDelay3.SetSecAndOn(0.5);
            Task=800;
            break;
        case 800:
//            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtArm2AllFailVerify])
            if(Sen[SnRealTimeCCDIndexArm].IsOn())
            {
                if(DoTestHeadMotorDelay3.Off())
                {
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILOK]=false;
                    COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILNG]=false;
                    Task=900;
                }
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                ShowMyMessage("RTC Arm 2 Half Veiw all fail verify timeout");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 900:
            if(MOT[MTestY1].Z1UpZ2Down(MOT[MTestZ2].GailSpeed, TMode, false))
            {
                Task=1000;
            }
            break;
        case 1000:   //等待RTC回傳verify結果
            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILOK])
            {
                Task=1100;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(COM2->bRealTimeCom_ReceiveOK[COM2->rtALLFAILNG])
            {
                Task=41400;
                COM2->SendCommToVision(COM2->rtArmFinish, false);
            }
            else if(DoTestHeadMotorDelay.Off())
            {
                COM2->SendCommToVision(COM2->rtArmFinish, false);
                ShowMyMessage("RTC Verify All Fail Time out");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
        case 1100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "DoHalfViewAllFailVerifyRTC 1100"))
            {
                Task=1200;
            }
            break;
        case 1200:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedY, "DoHalfViewAllFailVerifyRTC 1200"))
            {
                Task=1;
//                if(iContactMode==CONTACT_NORMAL)
//                {
                    bHalfViewVerifyNeedAboveSocket=false;
                    DoSetupSystemToProd();
//                }
//                else
//                {
//                    Prod.TestZ1_Test=Get0_01MMType(fContact->edContactHeight1->Text.c_str());
//                    Prod.TestZ2_Test=Get0_01MMType(fContact->edContactHeight2->Text.c_str());
//                }
                return true;
            }
            break;
        case 41400:
            if(IndexAlarmInArmAway()==true)
            {
                bIsTestSitICFallDown=true;
                ShowMyMessage("RTC Half Veiw Verify All Fail NG!!");
                COM2->DoReleaseAndInspEnd();
                Task=1;
            }
            break;
    }
    return false;
}
//==============================================================================
void DoTemperatureRise(int iArm, bool bTemperatureRise)                         //Ifor 20230418 add
{
    double dbSetATCTemp=0;
    int i,j;

    if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
        dbSetATCTemp=Temperature.fWorkTemperBase;
    else
        dbSetATCTemp=IniConfig.dATCAmbientTemperature;

    if(bTemperatureRise==true)
    {
        j=iArm*iATC_Use_Heat_Count/2;

        for(i=0; i<iATC_Use_Heat_Count/2; i++)
        {
            if(bATC_EnablesChannel[i+j]==true)
            {
                ATC_InterfaceForm->SetSingleTemp(i+j,Temperature.dTempRiseTemp);
            }
        }

        bChangeTest_TempOffset=1;
        bChangeTest_TempAlarm=true;
        bDoATCTempRise=true;
    }
    else
    {
        bChangeTest_TempOffset=0;
        bChangeTest_TempAlarm=false;
        ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
        bDoATCTempRise=false;
    }
}
//==============================================================================
void TriggerATC_FFC_Function(bool bEnabled)                                     //Ifor 20240507 add :FFC Trigger Even
{
    if(ATC_SYSTEM==eNewATCSystem &&
         Temperature.bATCActiveCooling==true &
         Temperature.bATC_FFCEnable==true)
    {
        if(IndexStatus==Z1Down_Z2Up) //ARM1 在下
            ATC_InterfaceForm->ChannelFFCTrigger(iATC_Use_Heat_Count, 0, bEnabled);
        else if(IndexStatus==Z1Up_Z2Down) //ARM2 在下
            ATC_InterfaceForm->ChannelFFCTrigger(iATC_Use_Heat_Count, 1, bEnabled);
        else
            ATC_InterfaceForm->ChannelFFCTrigger(iATC_Use_Heat_Count, 0, false);
    }
}
//==============================================================================
void DoCheckHasTestTempChange()                                                 //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
{
    double dbSetATCTemp=0;

    if(bChangeTest_TempOffset!=0)
    {
        if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)//kevin 20140918 恆溫控制
            dbSetATCTemp=Temperature.fWorkTemperBase;                           //Ifor 20160111 : ATC高溫的設定溫度
        else
            dbSetATCTemp=IniConfig.dATCAmbientTemperature;                      //Ifor 20160111 : [L11] ATC常溫的設定溫度

        bChangeTest_TempOffset=0;
        ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
        bChangeTest_TempAlarm=false;
    }
    else
    {
        bChangeTest_TempAlarm=false;                                            //Ifor 20210623 add: Test Temp Change
    }
    fLotInfo->SetATCOffset(true);                                               //Ifor 20241118 : 測試中變溫
}
//==============================================================================
void TestProcessSetToErr(AnsiString sErrorLog)                                  //JerryYang 20231208 : 包起來
{
    AnsiString asTestTimeOut="";
    bTestDuplicateErr=false;
    int iTest=0;
    int iUnTest=0;
    int iNullIC=0;
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC &&                                //jou 2011-04-19 只要按SKIP就全部IC歸R道
               TestSocket.Item[i][j]!=HAS_NULL_IC)
            {
                if(iTesterBIN[i][j]==-1 && TestSocket.Item[i][j]<TEST_PASS)
                {
                    iUnTest++;
                }
                else
                {
                    iTest++;
                }

                iTesterBIN[i][j]=999;
                TestSocket.SetItemData(i, j, TEST_PASS+iTesterBIN[i][j]);
                TestSocket.iBinData[i][j]=iTestBinCount;                        //Steven 20190116 : 修正顯示錯誤
                TestSocket.PordRec[i][j].AddTestResultRecord(iTesterBIN[i][j], TestSocket.cSBin[i][j], "TestTimeOut_SKIP"); //Frank 20160505 add
            }
            else
            {
                iNullIC++;
            }
        }
    }

    IsTest=false;                                                               //Steven 20110722 Start : Skip時,要重置GPIB測試狀態

    asTestTimeOut.printf("%s : Test=%d ; UnTest=%d ; NullIC=%d", sErrorLog, iTest, iUnTest, iNullIC);
    MyDBIProcess("Message", asTestTimeOut);
    MyDBIProcess("Message", asRecordTestResult);

    bTestingStopAllMotor=false;                                                 //jou 2013-09-25 Testing Need Stop All Motor
    if((CosFunction.bUSEJCETSiteMapMode==true || CUSTOMER_CODE==CC_ASE_M) && LastSet.iRunStartMode==rsmAutoSiteMap)    //JerryYang 20170316 (Steven) 避免auto site mapping時發生tester time out後沒有清除旗標,造成in arm沒有重新吸料會hang up//Ifor 20180417 : add ASE_M
    {
        DoJCETSiteMappingCHK(false);
    }
}
//==============================================================================
