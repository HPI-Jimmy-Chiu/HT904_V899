#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "ShuttleMove.h"
#include "myQwertyKeyBoard.h"
#include "main.h"
#include "iosetview.h"
#include "MyMotor.h"
#include "cmydef.h"
#include "cprod.h"
#include "cinitial.h"
#include "mymessbox.h"
#include "cinitial.h"
#include "ckernel.h"
#include "cContact.h"
#include "acarry.h"
#include "LtcSensor.h"
#include "cSecurity.h"
#include "cMyDB.h"
#include "MyCCLinkSensor.h"
#include "aoutarm.h"
#include "cpublic.h"
#include "MyLaneIo.h"
#include "mycylin.h"
#include "uHGemHT9045.h"
#include "uteach.h"
#include "ainarm2.h"
#include "BarCode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "HTray"
#pragma resource "*.dfm"
TfShuttleMove *fShuttleMove;
const int Sh1MoveToLeft        =1;
const int Sh1MoveToRight       =2;
const int Sh2MoveToLeft        =3;
const int Sh2MoveToRight       =4;
const int OutSh1CheckYSensor   =5;
const int OutSh1CheckZSensor   =6;
const int OutSh2CheckYSensor   =7;
const int OutSh2CheckZSensor   =8;
const int ScanDeviceOnOutSh1   =9;
const int ScanDeviceOnOutSh2   =10;
const int Sh1MoveToBarCodePos1 =11;
const int Sh2MoveToBarCodePos1 =12;
const int InSh1CheckSensor     =13;
const int InSh2CheckSensor     =14;
int iShMode=0;
//---------------------------------------------------------------------------
__fastcall TfShuttleMove::TfShuttleMove(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    bShuttleRetry=false;
    iMoveToTarget=-1;
}
//---------------------------------------------------------------------------
void __fastcall TfShuttleMove::FormShow(TObject *Sender)
{
    fShow=true;
    Left=100;
    Top =20;
    btShu1Left       ->Enabled=true;
    btShu1Right      ->Enabled=true;
    btShu2Left       ->Enabled=true;
    btShu2Right      ->Enabled=true;

    if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)    //JerryYang 20160901 shuttle使用one side時,shuttle maintain不給動
    {
        btShu2Left       ->Enabled=false;
        btShu2Right      ->Enabled=false;
    }
    else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)
    {
        btShu1Left       ->Enabled=false;
        btShu1Right      ->Enabled=false;
    }
    bShuttleRetry=false;
    gbBarCode->Visible=(BAR_CODE_INSTALL!=ebctUninstall);
    sbShuttleSensor->Visible=(SHUTTLE_SENSOR_TYPE==eSensorCCLink || SHUTTLE_SENSOR_TYPE==eSensorCCLink3 ||
                              SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3 ||
                              SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3);   //Steven 20191126 : 補上Canbus //Ifor 20250926 add:EtherCAT顯示

    meShuttleMaintain->Clear();
    #ifdef SOFT_SIMULTE
    btRetry->Visible=true;
    #else
    btRetry->Visible=false;
    #endif

    edInSH1Sen7DetectPos->Text      =Tech.iInSH1Sen7DetectPos;
    edInSH2Sen7DetectPos->Text      =Tech.iInSH2Sen7DetectPos;
    edInSH1BarCodePos->Text         =Tech.iInSH1BarCodePos;
    edInSH2BarCodePos->Text         =Tech.iInSH2BarCodePos;
    edOutSH1OneRowDetectPos->Text   =Tech.OutSH1ZOneRowDetectPos;
    edOutSH2OneRowDetectPos->Text   =Tech.OutSH2ZOneRowDetectPos;
    edOutSH1ZDetectPos->Text        =Tech.OutSH1ZDetectPos;
    edOutSH2ZDetectPos->Text        =Tech.OutSH2ZDetectPos;
    if(IniConfig.bSPILFunction==true)  //JerryYang 20170921 (Steven) SPIL守志要求鎖定
    {
        edInSH1Sen7DetectPos->Enabled=false;
        edInSH2Sen7DetectPos->Enabled=false;
        edInSH1BarCodePos->Enabled=false;
        edInSH2BarCodePos->Enabled=false;
        edOutSH1OneRowDetectPos->Enabled=false;
        edOutSH2OneRowDetectPos->Enabled=false;
        edOutSH1ZDetectPos->Enabled=false;
        edOutSH2ZDetectPos->Enabled=false;
    }
    ShowShuttleSensorPosition();
    palSh1Encoder->Caption=MOT[MInShuttle1].ReadEncoderPos();
    palSh2Encoder->Caption=MOT[MInShuttle2].ReadEncoderPos();

    sbSensorLatch->Visible= ENABLE_OUT_SHUTTLEY_LATCH; //kevin 20180129 add
    if(CosFunction.bAutoTeachOutShuttleLantch)
    {
        iMoveToTarget=-1;
        LastClickTime=0;
        LastClickX=-1;
        LastClickY=-1;
    }
}
//------------------------------------------------------------------------------
void TfShuttleMove::DoScanDeviceOnShuttle1()                                    //JerryYang 20160725 快速移動Out shuttle1並顯示偵測device結果
{
//    static int Task=1;
//    bool bflag;
    int &Task=iShuttleMoveTask;
    int i, j;
    AnsiString str1, str2, str3, ErrPart,ErrPart2;
    AnsiString strMin="", strMax="", strLatch="", strHasIC="";
    switch(Task)
    {
        case 1:
            if(InArmZSafe(DETECT_SENSOR_FLAG)==-1 && CheckOutArmZNeedHome()==-1 &&      //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
               MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe) &&
               MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe))
            {
                Task=5;
            }
            else
            {
                ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
                Task=50;
            }

            break;
        case 5:
            if(MoveOutArmXY_ToShuttleAlarmArea()==true)                         //Out arm離開shuttle
            {
                fLtcSensor->GetLtcSensor(0);
                MOT[MInShuttle1].SetSpeed(10);                                  //設定為慢速
                ShowShuttleMaintainInfo("Detect Device On Shuttle1 Start");
                Task=10;
            }
            break;
        case 10:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))                 //慢速移動到左側
            {
                fLtcSensor->GetLtcSensor(0);
                StartDetectMotorSensor(0);
                SetMotorScaleSpeed(MInShuttle1, SHSpeed.iSH1Sp);                //重新設定原本的速度
                Task=20;
            }
            break;
        case 20:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight))                //快速移動到右側
            {
                if(SThreadPara.bExeShuttleThread==false)
                {
                    fLtcSensor->GetLtcSensor(0);
                    for(i=0; i<8; i++)
                    {
                        SThreadPara.bSh1HasIC[0][i]=true;
                        SThreadPara.bSh1HasIC[1][i]=true;
                        SThreadPara.bInSh1HasIC[0][i]=true;
                        SThreadPara.bInSh1HasIC[1][i]=true;
                    }

                    if(SThreadPara.iScanSensor==1 &&
                       SThreadPara.bOutYUseLatch)                               //Steven 20140818 : 康泰克Out Sensor Y改用Latch
                    {
                        for(j=0; j<InArmSuck.iShtCol; j++)
                        {
                            for(i=0; i<fLtcSensor->LatchDataCnt5; i++)
                            {
                                if(fLtcSensor->LatchDataTable5[i]<SThreadPara.iCheckPosMaxY[0][j] &&
                                   fLtcSensor->LatchDataTable5[i]>SThreadPara.iCheckPosMinY[0][j])
                                {
                                    if(TestIF.iTestMode==_32Site4X8N)           //Steven 20140512 : For HT-9047
                                    {
                                        SThreadPara.bSh1HasIC[0][j]=false;
                                        SThreadPara.bSh1HasIC[1][j]=false;
                                    }
                                    else if(TestIF_File.iTestMode==_6Site2X3 &&
                                            Use_AxisY_Sensor_2x3mode==true)     //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
                                    {
                                        SThreadPara.bSh1HasIC[0][j]=false;
                                        SThreadPara.bSh1HasIC[1][j]=false;
                                    }
                                    else
                                    {
                                        SThreadPara.bSh1HasIC[0][j]=false;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        for(j=0; j<InArmSuck.iShtCol; j++)
                        {
                            for(i=0; i<fLtcSensor->LatchDataCnt1; i++)
                            {                                                   //先找第1顆,因為偏心只用這一顆
                                if(fLtcSensor->LatchDataTable1[i]<SThreadPara.iCheckPosMax[0][j] &&
                                   fLtcSensor->LatchDataTable1[i]>SThreadPara.iCheckPosMin[0][j])
                                {
                                    SThreadPara.bSh1HasIC[0][j]=false;          //這是A行
                                    break;
                                }
                            }

                            if(InArmSuck.iShtRow==2)                            //一次兩顆,所以要找第0顆
                            {
                                for(i=0; i<fLtcSensor->LatchDataCnt0; i++)
                                {
                                    if(fLtcSensor->LatchDataTable0[i]<SThreadPara.iCheckPosMax[0][j] &&
                                       fLtcSensor->LatchDataTable0[i]>SThreadPara.iCheckPosMin[0][j])
                                    {
                                        SThreadPara.bSh1HasIC[1][j]=false;      //這是B行
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(bReadAndCheckCPUName==true &&
                       CosFunction.bInShuttleDetectByLatch &&                   //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                       Prod.bF18InshuttleDetect &&
                       MOTION_CARD_TYPE==MotionCard_Contec &&
                       LastSet.iRealDummy==REALLY)                              //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                    {
                        for(j=0; j<InArmSuck.iShtCol; j++)
                        {
                            for(i=0; i<fLtcSensor->LatchDataCnt4; i++)
                            {
                                if(fLtcSensor->LatchDataTable4[i]<SThreadPara.iCheckInPosMaxY[0][j] &&
                                   fLtcSensor->LatchDataTable4[i]>SThreadPara.iCheckInPosMinY[0][j])
                                {
                                    if(TestIF.iTestMode==_32Site4X8N)           //Steven 20140512 : For HT-9047
                                    {
                                        SThreadPara.bInSh1HasIC[0][j]=false;
                                        SThreadPara.bInSh1HasIC[1][j]=false;
                                    }
                                    else
                                    {
                                        SThreadPara.bInSh1HasIC[0][j]=false;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    //檢查錯誤的位置
                    if(SThreadPara.iScanSensor==1 &&
                       SThreadPara.bOutYUseLatch)                               //Steven 20140818 : 康泰克Out Sensor Y改用Latch
                    {
                        if(TestIF.iTestMode==_32Site4X8N)                       //Steven 20140512 : For HT-9047
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料

                                if(SThreadPara.bSh1HasIC[0][j]==true)           //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[2][j]+" ";           //Steven 20151006 : For 32Site Out Shuttle Alarm 0/1改成 2/3
                                    ErrPart+=IndexSuckName[3][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                        }
                        else if(TestIF_File.iTestMode==_6Site2X3 &&
                                Use_AxisY_Sensor_2x3mode==true)                 //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
                        {
                            for(i=0; i<InArmSuck.iShtRow; i++)
                            {
                                for(j=0; j<InArmSuck.iShtCol; j++)
                                {
                                    //檢查殘料
                                    if(SThreadPara.bSh1HasIC[0][j]==true ||
                                       SThreadPara.bSh1HasIC[1][j]==true)       //有錯的位置
                                    {
                                        ErrPart+=IndexSuckName[i][j]+" ";
                                    }
                                    else
                                    {
                                        ErrPart+="X  ";                         //JerryYang 20160726 填入X,表示沒有偵測到device
                                    }
                                }
                                if(j>=InArmSuck.iShtCol)
                                    ErrPart+="\r\n";
                            }
                        }
                        else
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bSh1HasIC[0][j]==true)           //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[0][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                        }
                    }
                    else
                    {
                        for(i=0; i<InArmSuck.iShtRow; i++)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bSh1HasIC[i][j]==true)           //有錯的位置
                                {
                                    if(TestIF.iTestMode==_32Site4X8N)           //Steven 20151006 : For 32Site Out Shuttle Alarm
                                    {
                                        ErrPart+=IndexSuckName[i+2][j]+" ";
                                    }
                                    else
                                    {
                                        ErrPart+=IndexSuckName[i][j]+" ";
                                    }
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                            if(j>=InArmSuck.iShtCol)
                                ErrPart+="\r\n";
                        }
                    }
                    if(bReadAndCheckCPUName==true &&
                       CosFunction.bInShuttleDetectByLatch &&                   //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                       Prod.bF18InshuttleDetect &&
                       MOTION_CARD_TYPE==MotionCard_Contec &&
                       LastSet.iRealDummy==REALLY)                              //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                    {
                        if(TestIF.iTestMode==_32Site4X8N)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料

                                if(SThreadPara.bInSh1HasIC[0][j]==true)         //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[2][j]+" ";           //Steven 20151006 : For 32Site Out Shuttle Alarm 0/1改成 2/3
                                    ErrPart+=IndexSuckName[3][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                        }
                        else
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bInSh1HasIC[0][j]==true)         //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[0][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                        }
                    }
                    ShowShuttleMaintainInfo(ErrPart);
                }
                else
                {
                    if(TestIF_File.iTestMode==_6Site2X3 &&
                       Use_AxisY_Sensor_2x3mode==true)                          //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
                    {
                        for(i=0; i<InArmSuck.iShtRow; i++)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                if(bShuttleHasIC[0][0][j]==true ||
                                   bShuttleHasIC[0][1][j]==true)
                                {
                                    ErrPart+=IndexSuckName[i][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                            if(j>=InArmSuck.iShtCol)
                                ErrPart+="\r\n";
                        }
                    }
                    else
                    {
                        for(i=0; i<InArmSuck.iShtRow; i++)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                if(bShuttleHasIC[0][i][j]==true)
                                {
                                    ErrPart+=IndexSuckName[i][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                            if(j>=InArmSuck.iShtCol)
                                ErrPart+="\r\n";
                        }
                    }
                    ShowShuttleMaintainInfo(ErrPart);
                }
                ShowShuttleMaintainInfo("Detect Device On Shuttle1 Finish");
                Task=50;
            }
            break;
        case 50:
            fMain->Pause("DoScanDeviceOnShuttle1");
            break;
    }
}
//------------------------------------------------------------------------------
void TfShuttleMove::DoScanDeviceOnShuttle2()                                    //JerryYang 20160725 快速移動Out shuttle2並顯示偵測device結果
{
    int &Task=iShuttleMoveTask;
    int i, j;
    AnsiString str1, str2, str3, ErrPart,ErrPart2;
    AnsiString strMin="", strMax="", strLatch="", strHasIC="";

    switch(Task)
    {
        case 1:
            if(InArmZSafe(DETECT_SENSOR_FLAG)==-1 &&
               CheckOutArmZNeedHome()==-1 &&                                    //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
               MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe) &&
               MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe))
            {
                Task=5;
            }
            else
            {
                ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
            }
            break;
        case 5:
            if(MoveOutArmXY_ToShuttleAlarmArea()==true)                         //Out arm離開shuttle
            {
                MOT[MInShuttle2].SetSpeed(10);                                  //設定速度10
                fLtcSensor->GetLtcSensor(1);
                ShowShuttleMaintainInfo("Detect Device On Shuttle2 Start");
                Task=10;
            }
            break;
        case 10:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))                 //慢速移動到左側
            {
                SThreadPara.ErrPartSh2="";
                SetMotorScaleSpeed(MInShuttle2, SHSpeed.iSH2Sp);                //重新設定原本的速度
                StartDetectMotorSensor(1);
                fLtcSensor->GetLtcSensor(1);
                Task=20;
            }
            break;
        case 20:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))                //快速移動到右側
            {
                if(SThreadPara.bExeShuttleThread==false)
                {
                    fLtcSensor->GetLtcSensor(1);
                    for(i=0; i<8; i++)
                    {
                        SThreadPara.bSh2HasIC[0][i]=true;
                        SThreadPara.bSh2HasIC[1][i]=true;
                        SThreadPara.bInSh2HasIC[0][i]=true;
                        SThreadPara.bInSh2HasIC[1][i]=true;
                    }
                    if(SThreadPara.iScanSensor==1 &&
                       SThreadPara.bOutYUseLatch)                               //Steven 20140818 : 康泰克Out Sensor Y改用Latch
                    {
                        for(j=0; j<InArmSuck.iShtCol; j++)
                        {
                            for(i=0; i<fLtcSensor->LatchDataCnt7; i++)
                            {
                                if(fLtcSensor->LatchDataTable7[i]<SThreadPara.iCheckPosMaxY[0][j] &&
                                   fLtcSensor->LatchDataTable7[i]>SThreadPara.iCheckPosMinY[0][j])
                                {
                                    if(TestIF.iTestMode==_32Site4X8N)           //Steven 20140512 : For HT-9047
                                    {
                                        SThreadPara.bSh2HasIC[0][j]=false;
                                        SThreadPara.bSh2HasIC[1][j]=false;
                                    }
                                    else if(TestIF_File.iTestMode==_6Site2X3 &&
                                            Use_AxisY_Sensor_2x3mode==true)     //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
                                    {
                                        SThreadPara.bSh2HasIC[0][j]=false;
                                        SThreadPara.bSh2HasIC[1][j]=false;
                                    }
                                    else
                                    {
                                        SThreadPara.bSh2HasIC[0][j]=false;      //這是A行
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        for(j=0; j<InArmSuck.iShtCol; j++)
                        {
                            for(i=0; i<fLtcSensor->LatchDataCnt2; i++)
                            {                                                   //先找第2顆,因為偏心只用這一顆
                                if(fLtcSensor->LatchDataTable2[i]<SThreadPara.iCheckPosMax[1][j] &&
                                   fLtcSensor->LatchDataTable2[i]>SThreadPara.iCheckPosMin[1][j])
                                {
                                    if(InArmSuck.iShtRow==2)
                                        SThreadPara.bSh2HasIC[1][j]=false;      //這是B行
                                    else
                                        SThreadPara.bSh2HasIC[0][j]=false;      //這是A行
                                    break;
                                }
                            }

                            if(InArmSuck.iShtRow==2)                            //一次兩顆,所以要找第3顆
                            {
                                for(i=0; i<fLtcSensor->LatchDataCnt3; i++)
                                {
                                    if(fLtcSensor->LatchDataTable3[i]<SThreadPara.iCheckPosMax[1][j] &&
                                       fLtcSensor->LatchDataTable3[i]>SThreadPara.iCheckPosMin[1][j])
                                    {
                                        SThreadPara.bSh2HasIC[0][j]=false;      //這是A行
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if(bReadAndCheckCPUName==true &&
                       CosFunction.bInShuttleDetectByLatch &&                   //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                       Prod.bF18InshuttleDetect &&                              //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                       MOTION_CARD_TYPE==MotionCard_Contec &&
                       LastSet.iRealDummy==REALLY)
                    {
                        for(j=0; j<InArmSuck.iShtCol; j++)
                        {
                            for(i=0; i<fLtcSensor->LatchDataCnt6; i++)
                            {
                                if(fLtcSensor->LatchDataTable6[i]<SThreadPara.iCheckInPosMaxY[1][j] &&
                                   fLtcSensor->LatchDataTable6[i]>SThreadPara.iCheckInPosMinY[1][j])
                                {
                                    if(TestIF.iTestMode==_32Site4X8N)           //Steven 20140512 : For HT-9047
                                    {
                                        SThreadPara.bInSh2HasIC[0][j]=false;
                                        SThreadPara.bInSh2HasIC[1][j]=false;
                                    }
                                    else
                                    {
                                        SThreadPara.bInSh2HasIC[0][j]=false;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    //檢查錯誤的位置
                    if(SThreadPara.iScanSensor==1 &&
                       SThreadPara.bOutYUseLatch)                               //Steven 20140818 : 康泰克Out Sensor Y改用Latch
                    {
                        if(TestIF.iTestMode==_32Site4X8N)                       //Steven 20140512 : For HT-9047
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bSh2HasIC[0][j]==true)           //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[0][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";
                                }
                            }
                        }
                        else if(TestIF_File.iTestMode==_6Site2X3 &&
                                Use_AxisY_Sensor_2x3mode==true)                 //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
                        {
                            for(i=0; i<InArmSuck.iShtRow; i++)
                            {
                                for(j=0; j<InArmSuck.iShtCol; j++)
                                {
                                    //檢查殘料
                                    if(SThreadPara.bSh2HasIC[0][j]==true ||
                                       SThreadPara.bSh2HasIC[1][j]==true)       //有錯的位置
                                    {
                                        ErrPart+=IndexSuckName[i][j]+" ";
                                    }
                                    else
                                    {
                                        ErrPart+="X  ";
                                    }
                                }
                                if(j>=InArmSuck.iShtCol)
                                    ErrPart+="\r\n";
                            }
                        }
                        else
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bSh2HasIC[0][j]==true)           //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[0][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";
                                }
                            }
                        }
                    }
                    else
                    {
                        for(i=0; i<InArmSuck.iShtRow; i++)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bSh2HasIC[i][j]==true)           //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[i][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";
                                }
                            }
                            if(j>=InArmSuck.iShtCol)
                                ErrPart+="\r\n";
                        }
                    }

                    if(bReadAndCheckCPUName==true &&
                       CosFunction.bInShuttleDetectByLatch &&                   //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                       Prod.bF18InshuttleDetect &&                              //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
                       MOTION_CARD_TYPE==MotionCard_Contec &&
                       LastSet.iRealDummy==REALLY)
                    {
                        if(TestIF.iTestMode==_32Site4X8N)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bInSh2HasIC[0][j]==true)         //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[2][j]+" ";           //Steven 20151006 : For 32Site Out Shuttle Alarm 0/1改成 2/3
                                    ErrPart+=IndexSuckName[3][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }

                            }
                        }
                        else
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                //檢查殘料
                                if(SThreadPara.bInSh2HasIC[0][j]==true)         //有錯的位置
                                {
                                    ErrPart+=IndexSuckName[0][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";                             //JerryYang 20160726 填入X,表示沒有偵測到device
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(TestIF_File.iTestMode==_6Site2X3 &&
                       Use_AxisY_Sensor_2x3mode==true)                          //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
                    {
                        for(i=0; i<InArmSuck.iShtRow; i++)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                if(bShuttleHasIC[1][0][j]==true ||
                                   bShuttleHasIC[1][1][j]==true)
                                {
                                    ErrPart+=IndexSuckName[i][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";
                                }
                            }
                            if(j>=InArmSuck.iShtCol)
                                ErrPart+="\r\n";
                        }
                    }
                    else
                    {
                        for(i=0; i<InArmSuck.iShtRow; i++)
                        {
                            for(j=0; j<InArmSuck.iShtCol; j++)
                            {
                                if(bShuttleHasIC[1][i][j]==true)
                                {
                                    ErrPart+=IndexSuckName[i][j]+" ";
                                }
                                else
                                {
                                    ErrPart+="X  ";
                                }
                            }
                            if(j>=InArmSuck.iShtCol)
                                ErrPart+="\r\n";
                        }
                    }
                }
                ShowShuttleMaintainInfo(ErrPart);
                ShowShuttleMaintainInfo("Detect Device On Shuttle2 Finish");
                Task=50;
            }
            break;
        case 50:
            fMain->Pause("DoScanDeviceOnShuttle2");
            iMoveToTarget=-1;
            break;
    }
}
//---------------------------------------------------------------------------
void TfShuttleMove::DoShuttle1MoveToOutSensor(int iUseSensor,int iTarget)   //JerryYang 20160725 shuttle1 move to sensor by step
{
//  iUseSensor=0表示要Out shuttle要移動到Y Sensor位置
//  iUseSensor=1表示要Out shuttle要移動到Z Sensor位置
    int &Task=iShuttleMoveTask;
    static int iLineNo;
    switch(Task)
    {
        case 1:
            ShowShuttleSensorPosition();
            if(VerifySafetyPositionInOutIndex()==true)                          //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
            {
                Task=5;
            }
            else
            {
                ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
                Task=60;
            }
            break;
        case 5:
            if(MoveOutArmXY_ToShuttleAlarmArea()==true)                         //Out arm離開shuttle
            {
                if(iUseSensor==1 &&
                   (Tech.OutSH1ZDetectPos==0 ||
                   Tech.OutSH2ZDetectPos==0 ||
                   SThreadPara.iCHKStep==0))
                {
                    Task=60;
                    ShowMyMessage("Please check the teach position of OutShuttle1 8 site kit pos1 ", "Teach點位OutShuttle1 8 site kit pos1 為0, 請確認點位");
                }
                else if(iUseSensor==0 &&
                        (SThreadPara.iCHKStep==0 ||
                        Tech.OutSH1ZOneRowDetectPos==0 ||
                        Tech.OutSH2ZOneRowDetectPos==0))
                {
                    Task=60;
                    ShowMyMessage("Please check the teach position of OutShuttle1 One Row kit pos1 ", "Teach點位OutShuttle1 One Row kit pos1為0, 請確認點位");
                }
                else
                {
                    ShowShuttleMaintainInfo("Output Shuttle1 Fiber Sensor Position Check Start");
                    MOT[MInShuttle1].SetSpeed(10);
                    Task=10;
                }
            }

            break;
        case 10:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight))                //移動到右邊
            {
                iLineNo=0;
                Task=20;

                if(iUseSensor==0)                                               //Steven 20170110 : Add 顯示目前狀態
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 3, 2);
                }
                else
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 1, 2);
                }
            }
            break;
        case 20:
            if(iUseSensor==0)                                                   //Out shuttle要移動到Y Sensor位置
            {
                if(SThreadPara.iCHKStep==1)                                     //JerryYang 20160901 single site
                {
                    if(MOT[MInShuttle1].MotorMove(SThreadPara.base_posY[0]))
                    {
                        Task=30;
                    }
                }
                else if(MOT[MInShuttle1].MotorMove(GetSHCHKPos(iLineNo, SThreadPara.base_posY[0])))    //20160817 修正Step 移動到Y Sensor位置
                {
                    Task=30;
                }
            }
            else                                                                //Out shuttle要移動到Z Sensor位置
            {
                if(SThreadPara.iCHKStep==1)                                     //JerryYang 20160901 single site
                {
                    if(MOT[MInShuttle1].MotorMove(SThreadPara.base_pos[0]))
                    {
                        Task=30;
                    }
                }
                else if(MOT[MInShuttle1].MotorMove(GetSHCHKPos(iLineNo, SThreadPara.base_pos[0])))     //20160817 修正Step 移動到Z Sensor位置
                {
                    Task=30;
                }
            }
            break;
        case 30:
            if(WaitManualRetryKey() ||
               (iTarget!=-1 && iTarget!=iLineNo))
            {
                if(iUseSensor==0)                                               //Steven 20170110 : Add 顯示目前狀態
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 3, 3);
                }
                else
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 1, 3);
                }
                Task=40;
            }
            break;
        case 40:
            iLineNo++;
            if(iLineNo>=InArmSuck.iShtCol)
            {
                Task=50;
            }
            else
            {
                if(iUseSensor==0)                                               //Steven 20170110 : Add 顯示目前狀態
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 3, 2);
                }
                else
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 1, 2);
                }
                Task=20;
            }
            break;
        case 50:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))                 //kevin 20170529 (wei) add移動到左邊可以調整sensor
            {
                fMain->Pause("DoShuttle1MoveToOutSensor 50");
                ShowShuttleMaintainInfo("Output Shuttle1 Fiber Sensor Position Check Finish!");
                Task=55;
            }
            break;
        case 55:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight))                //移動到右邊
            {
                ShowShuttleMaintainInfo("Output Shuttle1 Fiber Sensor Position Check Finish!");
                Task=60;
            }
            break;
        case 60:
            fMain->Pause("DoShuttle1MoveToOutSensor 60");
            iMoveToTarget=-1;
            break;
    }
}
//---------------------------------------------------------------------------
void TfShuttleMove::DoShuttle2MoveToOutSensor(int iUseSensor,int iTarget)       //JerryYang 20160725 shuttle2 move to sensor by step
{
//  iUseSensor=0表示要Out shuttle要移動到Y Sensor位置
//  iUseSensor=1表示要Out shuttle要移動到Z Sensor位置
    int &Task=iShuttleMoveTask;
    static int iLineNo;
    switch(Task)
    {
        case 1:
            ShowShuttleSensorPosition();                                        //Steven 20170110 : Add 顯示目前狀態
            if(VerifySafetyPositionInOutIndex()==true)                          //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
            {
                Task=5;
            }
            else
            {
                ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
                Task=60;
            }
            break;
        case 5:
            if(MoveOutArmXY_ToShuttleAlarmArea()==true)                         //Out arm離開shuttle
            {
                if(iUseSensor==1 &&
                    (Tech.OutSH1ZDetectPos==0 ||
                    Tech.OutSH2ZDetectPos==0 ||
                    SThreadPara.iCHKStep==0))
                {
                    ShowMyMessage("Please check the teach position of OutShuttle2 8 site kit pos1 ", "Teach點位OutShuttle2 8 site kit pos1 為0, 請確認點位");
                    Task=60;
                }
                else if(iUseSensor==0 &&
                    (SThreadPara.iCHKStep==0 ||
                    Tech.OutSH1ZOneRowDetectPos==0 ||
                    Tech.OutSH2ZOneRowDetectPos==0))
                {
                    ShowMyMessage("Please check the teach position of OutShuttle2 One Row kit pos1 ", "Teach點位OutShuttle2 One Row kit pos1為0, 請確認點位");
                    Task=60;
                }
                else
                {
                    ShowShuttleMaintainInfo("Output Shuttle2 Fiber Sensor Position Check Start");
                    MOT[MInShuttle2].SetSpeed(10);
                    Task=10;
                }
            }
            break;
        case 10:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))                //移動到右邊
            {
                iLineNo=0;
                if(iUseSensor==0)                                               //Steven 20170110 : Add 顯示目前狀態
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 2, 2);
                }
                else
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 0, 2);
                }
                Task=20;
            }
            break;
        case 20:
            if(iUseSensor==0)                                                   //Out shuttle要移動到Y Sensor位置
            {
                if(SThreadPara.iCHKStep==1)                                     //JerryYang 20160901 single site
                {
                    if(MOT[MInShuttle2].MotorMove(SThreadPara.base_posY[1]))
                    {
                        Task=30;
                    }
                }
                else if(MOT[MInShuttle2].MotorMove(GetSHCHKPos(iLineNo, SThreadPara.base_posY[1])))    //20160817 修正Step 移動到Y Sensor位置
                {
                    Task=30;
                }
            }
            else
            {
                if(SThreadPara.iCHKStep==1)                                     //JerryYang 20160901 single site
                {
                    if(MOT[MInShuttle2].MotorMove(SThreadPara.base_pos[1]))
                    {
                        Task=30;
                    }
                }
                else if(MOT[MInShuttle2].MotorMove(GetSHCHKPos(iLineNo, SThreadPara.base_pos[1])))  //20160817 修正Step 移動到Z Sensor位置
                {
                    Task=30;
                }
            }
            break;
        case 30:
            if(WaitManualRetryKey() ||
               (iTarget!=-1 && iTarget!=iLineNo))
            {
                if(iUseSensor==0)                                               //Steven 20170110 : Add 顯示目前狀態
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 2, 3);
                }
                else
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 0, 3);
                }
                Task=40;
            }
            break;
        case 40:
            iLineNo++;
            if(iLineNo>=InArmSuck.iShtCol)
            {
                Task=50;
            }
            else
            {
                if(iUseSensor==0)                                               //Steven 20170110 : Add 顯示目前狀態
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 2, 2);
                }
                else
                {
                    mtOutSHDetectPos->SetCellColorIndex(iLineNo, 0, 2);
                }
                Task=20;
            }
            break;
        case 50:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))                 //kevin 20170529 (wei) add移動到左邊可以調整sensor
            {
                fMain->Pause("DoShuttle2MoveToOutSensor 50");
                ShowShuttleMaintainInfo("Output Shuttle2 Fiber Sensor Position Check Finish!");
                Task=55;
            }
            break;
        case 55:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))                //移動到右邊
            {
                ShowShuttleMaintainInfo("Output Shuttle2 Fiber Sensor Position Check Finish!");
                Task=60;
            }
            break;
        case 60:
            fMain->Pause("DoShuttle2MoveToOutSensor 60");
            break;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::btRetryClick(TObject *Sender)
{
    fShuttleMove->bShuttleRetry=true;
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::ShuttleMoveClick(TObject *Sender)                //JerryYang 20160726 選擇shuttle移動流程
{
    int Tag;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;

    if(SystemStart)
        return;

    iShMode=0;
    iShuttleMoveTask=1;
    bSetupStep=false;

    switch(Tag)
    {
        case 1:
            iShMode=Sh1MoveToLeft;                                              //Shuttle 1移到左邊
            break;
        case 2:
            iShMode=Sh1MoveToRight;                                             //Shuttle 1移到右邊
            break;
        case 3:
            iShMode=Sh2MoveToLeft;                                              //Shuttle 2移到左邊
            break;
        case 4:
            iShMode=Sh2MoveToRight;                                             //Shuttle 2移到右邊
            break;
        case 5:
            iShMode=OutSh1CheckYSensor;                                         //Shuttle1 Step移動到out shuttle Y sensor位置
            break;
        case 6:
            iShMode=OutSh1CheckZSensor;                                         //Shuttle1 Step移動到out shuttle Z sensor位置
            break;
        case 7:
            iShMode=OutSh2CheckYSensor;                                         //Shuttle2 Step移動到out shuttle Y sensor位置
            break;
        case 8:
            iShMode=OutSh2CheckZSensor;                                         //Shuttle2 Step移動到out shuttle Z sensor位置
            break;
        case 9:
            iShMode=ScanDeviceOnOutSh1;                                         //Out shuttle1 sensor檢查流程
            break;
        case 10:
            iShMode=ScanDeviceOnOutSh2;                                         //Out shuttle2 sensor檢查流程
            break;
        case 11:
            iShMode=Sh1MoveToBarCodePos1;                                       //shuttle1移動到Bar code第一個位置
            break;
        case 12:
            iShMode=Sh2MoveToBarCodePos1;                                       //shuttle2移動到Bar code第一個位置
            break;
        case 13:
            iShMode=InSh1CheckSensor;                                           //shuttle1 Step移動到第七顆sensor位置
            break;
        case 14:
            iShMode=InSh2CheckSensor;                                           //shuttle2 Step移動到第七顆sensor位置
            break;
    }
    fMain->Start("ShuttleMoveClick");
}
//---------------------------------------------------------------------------
void TfShuttleMove::DoShuttleMove()                                             //JerryYang 20160726 Shuttle相關點位確認流程
{
    int Mode=iShMode;

    palSh1Encoder->Caption=MOT[MInShuttle1].ReadEncoderPos();                   //Steven 20170110 : Add 顯示目前狀態
    palSh2Encoder->Caption=MOT[MInShuttle2].ReadEncoderPos();

    switch(Mode)
    {
        case Sh1MoveToLeft:
            DoShuttleMoveToSide(0, 0);                                          //Shuttle 1移到左邊
            break;
        case Sh1MoveToRight:
            DoShuttleMoveToSide(0, 1);                                          //Shuttle 1移到右邊
            break;
        case Sh2MoveToLeft:
            DoShuttleMoveToSide(1, 0);                                          //Shuttle 2移到左邊
            break;
        case Sh2MoveToRight:
            DoShuttleMoveToSide(1, 1);                                          //Shuttle 2移到右邊
            break;
        case OutSh1CheckYSensor:
            DoShuttle1MoveToOutSensor(0,iMoveToTarget);                         //Shuttle1 Step移動到out shuttle Y sensor位置
            break;
        case OutSh1CheckZSensor:
            DoShuttle1MoveToOutSensor(1,iMoveToTarget);                         //Shuttle1 Step移動到out shuttle Z sensor位置
            break;
        case OutSh2CheckYSensor:
            DoShuttle2MoveToOutSensor(0,iMoveToTarget);                         //Shuttle2 Step移動到out shuttle Y sensor位置
            break;
        case OutSh2CheckZSensor:
            DoShuttle2MoveToOutSensor(1,iMoveToTarget);                         //Shuttle2 Step移動到out shuttle Z sensor位置
            break;
        case ScanDeviceOnOutSh1:
            DoScanDeviceOnShuttle1();                                           //Out shuttle1 sensor檢查流程
            break;
        case ScanDeviceOnOutSh2:
            DoScanDeviceOnShuttle2();                                           //Out shuttle2 sensor檢查流程
            break;
        case Sh1MoveToBarCodePos1:
            DoShuttleMoveToBarCodePos1(0);                                      //shuttle1移動到Bar code第一個位置
            break;
        case Sh2MoveToBarCodePos1:
            DoShuttleMoveToBarCodePos1(1);                                      //shuttle2移動到Bar code第一個位置
            break;
        case InSh1CheckSensor:
            DoInShuttleCheckByStep(0);                                          //shuttle1 Step移動到sensor位置
            break;
        case InSh2CheckSensor:
            DoInShuttleCheckByStep(1);                                          //shuttle2 Step移動到sensor位置
            break;
    }
}
//---------------------------------------------------------------------------
void TfShuttleMove::DoShuttleMoveToSide(int iShuttle, int iSide)                //JerryYang 20160726 Shuttle移動到左右側
{
//iShuttle=1表示Shuttle1, iShuttle=2表示Shuttle2
//iSide=0表示移動到左側, iSide=1表示移動到右側
    int &Task=iShuttleMoveTask;
    AnsiString str1, str2, str3, ErrPart;

    switch(Task)
    {
        case 1:
            ShowShuttleSensorPosition();
            if(VerifySafetyPositionInOutIndex()==true)                          //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
            {

                Task=5;
            }
            else
            {
                ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
                Task=50;
            }
            break;
        case 5:
            if(MoveOutArmXY_ToShuttleAlarmArea()==true)                         //Out arm離開shuttle
            {
                if(iShuttle==0 && iSide==0)
                {
                    MOT[MInShuttle1].SetSpeed(10);
                    Task=10;
                }
                else if(iShuttle==0 && iSide==1)
                {
                    MOT[MInShuttle1].SetSpeed(10);
                    Task=20;
                }
                else if(iShuttle==1 && iSide==0)
                {
                    MOT[MInShuttle2].SetSpeed(10);
                    Task=30;
                }
                else if(iShuttle==1 && iSide==1)
                {
                    MOT[MInShuttle2].SetSpeed(10);
                    Task=40;
                }
            }
            break;
        case 10:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
            {
                ShowMyMessage("SH1 Move Left OK ", "SH1 移動完成");
                Task=50;
            }
            break;
        case 20:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight))
            {
                ShowMyMessage("SH1 Move Right OK ", "SH1 移動完成");
                Task=50;
            }
            break;
        case 30:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                ShowMyMessage("SH2 Move to left Finish", "SH2 移動完成");
                Task=50;
            }
            break;
        case 40:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))
            {
                ShowMyMessage("SH2 Move to Right Finish", "SH2 移動完成");
                Task=50;
            }
            break;
        case 50:
            fMain->Pause("DoShuttleMoveToSide 50");
            break;
    }
}
//------------------------------------------------------------------------------
void TfShuttleMove::DoShuttleMoveToBarCodePos1(int iSht)                        //JerryYang 20160727 In Shuttle1移動到Bar code第一個位置
{
    int &Task=iShuttleMoveTask;
    static int pos=0;                                                           //Steven 20191205 : 加上static
    AnsiString str1, str2, str3, ErrPart;

    switch(Task)
    {
        case 1:
            ShowShuttleSensorPosition();
            if(VerifySafetyPositionInOutIndex()==true)                          //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
            {
                Task=5;
            }
            else
            {
                ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
                Task=50;
            }
            break;
        case 5:
            if(MoveOutArmXY_ToShuttleAlarmArea()==true)                         //Out arm離開shuttle
            {
                Task=6;
            }
            break;
        case 6:
            if(MoveInArm2XYToLoaderWait()==true)
            {
                if(iSht==0)                                                     //Shuttle 1
                {
                    MOT[MInShuttle1].SetSpeed(10);
                    Task=10;
                }
                else if(iSht==1)                                                //Shuttle 2
                {
                    MOT[MInShuttle2].SetSpeed(10);
                    Task=30;
                }
            }
            break;
        case 10:
            mtInSHBarCodePos->SetCellColorIndex(0, 1, 2);                       //Steven 20170110 : Add 顯示目前狀態
            pos=fBarCode->GetMovePos(iSht, 0, 0)+Offset.iSHLeft2D[iSht];
            Task=15;
            break;
        case 15:
            if(MOT[MInShuttle1].MotorMove(pos))
            {
                mtInSHBarCodePos->SetCellColorIndex(0, 1, 3);                   //Steven 20170110 : Add 顯示目前狀態
                palSh1Encoder->Caption=MOT[MInShuttle1].ReadEncoderPos();
                palSh2Encoder->Caption=MOT[MInShuttle2].ReadEncoderPos();
                if(TestIF_File.bEnableMulti2D &&
                   (TestIF_File.iMulti2DType==e1x2In1CCD ||
                    TestIF_File.iMulti2DType==e2x2In1CCD ||
                    TestIF_File.iMulti2DType==e2x2In2CCD))                      //Steven 20240612 : for 對角2D
                {
                    ShowMyMessage("SH1 Move BarCode Pos1-1 OK ", "SH1 移動完成");
                    Task=20;
                }
                else
                {
                    ShowMyMessage("SH1 Move BarCode Pos1 OK ", "SH1 移動完成");
                    Task=50;
                }
            }
            break;
        case 20:
            if(WaitManualRetryKey() ||
               bSetupStep==true)
            {
                mtInSHBarCodePos->SetCellColorIndex(1, 1, 2);                   //Steven 20170110 : Add 顯示目前狀態
                pos=fBarCode->GetMovePos(iSht, 0, 1)+Offset.iSHLeft2D[iSht];
                Task=25;
            }
            break;
        case 25:
            if(MOT[MInShuttle1].MotorMove(pos))
            {
                mtInSHBarCodePos->SetCellColorIndex(1, 1, 3);                   //Steven 20170110 : Add 顯示目前狀態
                palSh1Encoder->Caption=MOT[MInShuttle1].ReadEncoderPos();
                palSh2Encoder->Caption=MOT[MInShuttle2].ReadEncoderPos();
                ShowMyMessage("SH1 Move BarCode Pos1-2 OK ", "SH1 移動完成");
                Task=50;
            }
            break;
        case 30:
            mtInSHBarCodePos->SetCellColorIndex(0, 0, 2);                       //Steven 20170110 : Add 顯示目前狀態
            pos=fBarCode->GetMovePos(iSht, 0, 0)+Offset.iSHLeft2D[iSht];
            Task=35;
            break;
        case 35:
            if(MOT[MInShuttle2].MotorMove(pos))
            {
                mtInSHBarCodePos->SetCellColorIndex(0, 0, 3);                   //Steven 20170110 : Add 顯示目前狀態
                palSh1Encoder->Caption=MOT[MInShuttle1].ReadEncoderPos();
                palSh2Encoder->Caption=MOT[MInShuttle2].ReadEncoderPos();
                if(TestIF_File.bEnableMulti2D &&
                   (TestIF_File.iMulti2DType==e1x2In1CCD ||
                    TestIF_File.iMulti2DType==e2x2In1CCD ||
                    TestIF_File.iMulti2DType==e2x2In2CCD))                      //Steven 20240612 : for 對角2D
                {
                    ShowMyMessage("SH2 Move BarCode Pos1-1 OK ", "SH2 移動完成");
                    Task=40;
                }
                else
                {
                    ShowMyMessage("SH2 Move BarCode Pos1 OK ", "SH2 移動完成");
                    Task=50;
                }
            }
            break;
        case 40:
            if(WaitManualRetryKey() ||
               bSetupStep==true)
            {
                mtInSHBarCodePos->SetCellColorIndex(1, 0, 2);                   //Steven 20170110 : Add 顯示目前狀態
                pos=fBarCode->GetMovePos(iSht, 0, 1)+Offset.iSHLeft2D[iSht];
                Task=45;
            }
            break;
        case 45:
            if(MOT[MInShuttle2].MotorMove(pos))
            {
                mtInSHBarCodePos->SetCellColorIndex(1, 0, 3);                   //Steven 20170110 : Add 顯示目前狀態
                palSh1Encoder->Caption=MOT[MInShuttle1].ReadEncoderPos();
                palSh2Encoder->Caption=MOT[MInShuttle2].ReadEncoderPos();
                ShowMyMessage("SH2 Move BarCode Pos1-2 OK ", "SH2 移動完成");
                Task=50;
            }
            break;
        case 50:
            palSh1Encoder->Caption=MOT[MInShuttle1].ReadEncoderPos();           //Steven 20170110 : Add 顯示目前狀態
            palSh2Encoder->Caption=MOT[MInShuttle2].ReadEncoderPos();
            fMain->Pause("DoShuttleMoveToBarCodePos1 50");
            break;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::sbShuttleSensorClick(TObject *Sender)
{
    sbShuttleSensor->Down=false;
    if(CUSTOMER_CODE!=CC_KYEC_LEE &&
       CUSTOMER_CODE!=CC_KYEC_XILINX &&
       CUSTOMER_CODE!=CC_ASE_KaohSiung)                                         //kevin 20220106 add by pass
    {
        if(fSecurity->Insufficient(45)==false)
            return;
    }
    NewRecordProcess("MES2193", "Enter CC-Link Form");
    fCCLink->ShowUseSensor(TestIF.iTestMode, TestIF.dSiteXPitch);
    fCCLink->Show();
}
//------------------------------------------------------------------------------
void TfShuttleMove::DoInShuttleCheckByStep(int iShuttle)                        //JerryYang 20160808 shuttle1 move to sensor by step
{
    int &Task=iShuttleMoveTask;
    static int iStep[2],iMax=0;
    static int CheckPos[2][9];
    AnsiString strSht;
    int i;
    switch(Task)
    {
        case 1:
            ShowShuttleSensorPosition();
            if(VerifySafetyPositionInOutIndex()==true)                          //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
            {
                MOT[MInShuttle1+iShuttle].SetSpeed(10);
                Task=5;
            }
            else
            {
                ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
                Task=60;
            }
            break;
        case 5:
            if(MoveOutArmXY_ToShuttleAlarmArea()==true)                         //Out arm離開shuttle
            {
                if(iShuttle==0 && Tech.iInSH1Sen7DetectPos==0)                  //JerryYang 20160829 Teach點位為0就不用移動
                {
                    Task=60;
                    ShowMyMessage("Please check the teach position of In Shuttle1 8 site kit pos1", "Teach點位In Shuttle1 8 site kit pos1為0, 請確認點位");
                }
                else if(iShuttle==1 && Tech.iInSH2Sen7DetectPos==0)
                {
                    Task=60;
                    ShowMyMessage("Please check the teach position of In Shuttle2 8 site kit pos1", "Teach點位OutShuttle2 One Row kit pos1為0, 請確認點位");
                }
                strSht.sprintf("Input Shuttle%d Fiber Sensor Position Check Start", iShuttle+1);
                ShowShuttleMaintainInfo(strSht);
                Task=10;
            }
            break;
        case 10:
            if(MOT[MInShuttle1+iShuttle].MotorMove(Prod.InSHT[iShuttle].iLeft))    //移動到左邊
            {
                if(TestIF.iTestMode==SingleSite ||
                   TestIF.iTestMode==DualSite2x1)
                {
                    iStep[iShuttle]=0;
                }
                else if(TestIF.iTestMode==DualSite       ||
                        TestIF.iTestMode==QualSite2X2    ||
                        TestIF.iTestMode==QualSite2X2N)                         //Frank 20200520 2X2NN Mode
                {
                    iStep[iShuttle]=1;
                }
                else if(TestIF.iTestMode==_6Site2X3  ||                         //ChungHung 20140115 add for 2x3_6
                        TestIF.iTestMode==TriSite1X3 ||
                        TestIF.iTestMode==_6Site2X3N)                           //Steven 20220425 : 2X3NN Mode
                {
                    iStep[iShuttle]=2;
                }
                else if(TestIF.iTestMode==_10Site2X5)                           //wei 20190614 10 site
                {
                    iStep[iShuttle]=5;
                }
                else if(TestIF.iTestMode==_12Site2X6)
                {
                    iStep[iShuttle]=6;
                }
                else if(TestIF.iTestMode==_16Site2X8 ||                         //2x8 Eliot 2009_12_28
                        TestIF.iTestMode==_32Site4X8N)                          //Steven 20140512 : For HT-9047
                {
                    iStep[iShuttle]=7;
                }
                else
                {
                    iStep[iShuttle]=3;
                }
                iMax=iStep[iShuttle];

                for(i=0; i<MAX_Index_Col; i++)
                {
                    if(i<=iStep[iShuttle])
                    {
                        if(TestIF.iTestMode==SingleSite ||
                           TestIF.iTestMode==DualSite2x1)
                        {
                            CheckPos[0][i]=Prod.iInSHSen7DetectPos1x1[0];
                            CheckPos[1][i]=Prod.iInSHSen7DetectPos1x1[1];
                        }
                        else if(TestIF.iTestMode==DualSite       ||             // 1x2 & 2x2
                                TestIF.iTestMode==QualSite2X2    ||
                                TestIF.iTestMode==QualSite2X2N)                 //Frank 20200520 2X2NN Mode
                        {
                            CheckPos[0][i]=Prod.iInSHSen7DetectPos1x2[0][i];
                            CheckPos[1][i]=Prod.iInSHSen7DetectPos1x2[1][i];
                        }
                        else if(TestIF.iTestMode==_6Site2X3  ||                 //ChungHung 20140115 add for 2x3_6
                                TestIF.iTestMode==TriSite1X3 ||
                                TestIF.iTestMode==_6Site2X3N)                   //Steven 20220425 : 2X3NN Mode
                        {
                            CheckPos[0][i]=Prod.iInSHSen7DetectPos2x3[0][i];
                            CheckPos[1][i]=Prod.iInSHSen7DetectPos2x3[0][i];
                        }
                        else if(TestIF.iTestMode==_10Site2X5)                   //wei 20190614 10 site
                        {
                            CheckPos[0][i]=Prod.iInSHSen9DetectPos2x5[0][i];
                            CheckPos[1][i]=Prod.iInSHSen9DetectPos2x5[0][i];
                        }
                        else if(TestIF.iTestMode==_12Site2X6)
                        {
                            CheckPos[0][i]=Prod.iInSHSen9DetectPos2x6[0][i];
                            CheckPos[1][i]=Prod.iInSHSen9DetectPos2x6[0][i];
                        }
                        else if(TestIF.iTestMode==_16Site2X8 ||                 //2x8 Eliot 2009_12_28
                                TestIF.iTestMode==_32Site4X8N)                  //Steven 20140512 : For HT-9047
                        {
                            CheckPos[0][i]=Prod.iInSHSen9DetectPos2x8[0][i];
                            CheckPos[1][i]=Prod.iInSHSen9DetectPos2x8[1][i];
                        }
                        else
                        {
                            CheckPos[0][i]=Prod.iInSHSen7DetectPos1x4[0][i];
                            CheckPos[1][i]=Prod.iInSHSen7DetectPos1x4[1][i];
                        }
                    }
                    else
                    {
                        CheckPos[0][i]=0;
                        CheckPos[1][i]=0;
                    }
                }
                Task=20;
                mtedInSHSen7DetectPos->SetCellColorIndex(iMax-iStep[iShuttle], 1-iShuttle, 2);   //Steven 20170110 : Add 顯示目前狀態
            }
            break;
        case 20:                                                                //In shuttle要移動到Z Sensor位置
            if(MOT[MInShuttle1+iShuttle].MotorMove(CheckPos[iShuttle][iMax-iStep[iShuttle]]))   //Step 移動到 sensor位置
            {
                Task=30;
            }
            break;
        case 30:
            if(WaitManualRetryKey())
                Task=40;
            break;
        case 40:
            mtedInSHSen7DetectPos->SetCellColorIndex(iMax-iStep[iShuttle], 1-iShuttle, 3);       //Steven 20170110 : Add 顯示目前狀態
            iStep[iShuttle]--;
            if(iStep[iShuttle]<0)
            {
                Task=50;
            }
            else
            {
                mtedInSHSen7DetectPos->SetCellColorIndex(iMax-iStep[iShuttle], 1-iShuttle, 2);   //Steven 20170110 : Add 顯示目前狀態
                Task=20;
            }
            break;
        case 50:
            if(MOT[MInShuttle1+iShuttle].MotorMove(Prod.InSHT[iShuttle].iLeft)) //移動到左邊
            {
                strSht.sprintf("Input Shuttle%d Fiber Sensor Position Check Finish!", iShuttle+1);
                ShowShuttleMaintainInfo(strSht);
                Task=60;
            }
            break;
        case 60:
            fMain->Pause("DoInShuttleCheckByStep");
            break;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::sbtExitClick(TObject *Sender)
{
    fAllMotorHome=false;
    fShow=false;
    Close();
}
//------------------------------------------------------------------------------
//Steven 20170110 : Add for可以直接調整Shuttle的Teaching
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::sbUpdateClick(TObject *Sender)
{
    Tech.iInSH1Sen7DetectPos    =atoi(edInSH1Sen7DetectPos->Text.c_str());
    Tech.iInSH2Sen7DetectPos    =atoi(edInSH2Sen7DetectPos->Text.c_str());
    Tech.iInSH1BarCodePos       =atoi(edInSH1BarCodePos->Text.c_str());
    Tech.iInSH2BarCodePos       =atoi(edInSH2BarCodePos->Text.c_str());
    Tech.OutSH1ZOneRowDetectPos =atoi(edOutSH1OneRowDetectPos->Text.c_str());
    Tech.OutSH2ZOneRowDetectPos =atoi(edOutSH2OneRowDetectPos->Text.c_str());
    Tech.OutSH1ZDetectPos       =atoi(edOutSH1ZDetectPos->Text.c_str());
    Tech.OutSH2ZDetectPos       =atoi(edOutSH2ZDetectPos->Text.c_str());

    fTeach->SaveFile(true);
    InitShuttleThreadParameter();                                               //Steven 20120921 : Teach做完要重新Init一次
    ShowShuttleSensorPosition();
}
//------------------------------------------------------------------------------
//Steven 20170110 : Add 顯示目前狀態
//------------------------------------------------------------------------------
void TfShuttleMove::ShowShuttleSensorPosition()
{
    SetTechDataToProd();                                                        //jimmychiu 20251204 : update sht position array before show info
    mtedInSHSen7DetectPos->XItem=InArmSuck.iShtCol;
    mtOutSHDetectPos->XItem=InArmSuck.iShtCol;

    for(int i=0; i<InArmSuck.iShtCol; i++)
    {
        mtOutSHDetectPos->SetCellNumber(i, 1, GetSHCHKPos(i, SThreadPara.base_pos[0]));
        mtOutSHDetectPos->SetCellNumber(i, 0, GetSHCHKPos(i, SThreadPara.base_pos[1]));
        mtOutSHDetectPos->SetCellNumber(i, 3, GetSHCHKPos(i, SThreadPara.base_posY[0]));
        mtOutSHDetectPos->SetCellNumber(i, 2, GetSHCHKPos(i, SThreadPara.base_posY[1]));

        if(InArmSuck.iShtCol==1)
        {
            mtedInSHSen7DetectPos->SetCellNumber(i, 1, Prod.iInSHSen7DetectPos1x1[0]);
            mtedInSHSen7DetectPos->SetCellNumber(i, 0, Prod.iInSHSen7DetectPos1x1[1]);
        }
        else if(InArmSuck.iShtCol==2)
        {
            mtedInSHSen7DetectPos->SetCellNumber(i, 1, Prod.iInSHSen7DetectPos1x2[0][i]);
            mtedInSHSen7DetectPos->SetCellNumber(i, 0, Prod.iInSHSen7DetectPos1x2[1][i]);
        }
        else if(InArmSuck.iShtCol==3)                                           //ChungHung 20140115 add for 2x3_6
        {
            mtedInSHSen7DetectPos->SetCellNumber(i, 1, Prod.iInSHSen7DetectPos2x3[0][i]);
            mtedInSHSen7DetectPos->SetCellNumber(i, 0, Prod.iInSHSen7DetectPos2x3[1][i]);
        }
        else if(InArmSuck.iShtCol==4)
        {
            mtedInSHSen7DetectPos->SetCellNumber(i, 1, Prod.iInSHSen7DetectPos1x4[0][i]);
            mtedInSHSen7DetectPos->SetCellNumber(i, 0, Prod.iInSHSen7DetectPos1x4[1][i]);
        }
        else if(InArmSuck.iShtCol==5)                                           //Steven 20221027 : Add for 2x5
        {
            mtedInSHSen7DetectPos->SetCellNumber(i, 1, Prod.iInSHSen9DetectPos2x5[0][i]);
            mtedInSHSen7DetectPos->SetCellNumber(i, 0, Prod.iInSHSen9DetectPos2x5[1][i]);
        }
        else if(InArmSuck.iShtCol==6)
        {
            mtedInSHSen7DetectPos->SetCellNumber(i, 1, Prod.iInSHSen9DetectPos2x6[0][i]);
            mtedInSHSen7DetectPos->SetCellNumber(i, 0, Prod.iInSHSen9DetectPos2x6[1][i]);
        }
        else if(InArmSuck.iShtCol==8)
        {
            mtedInSHSen7DetectPos->SetCellNumber(i, 1, Prod.iInSHSen9DetectPos2x8[0][i]);
            mtedInSHSen7DetectPos->SetCellNumber(i, 0, Prod.iInSHSen9DetectPos2x8[1][i]);
        }
        else
        {
            ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "TfShuttleMove::ShowShuttleSensorPosition");
        }
    }

    if(TestIF_File.bEnableMulti2D &&
       (TestIF_File.iMulti2DType==e1x2In1CCD ||
        TestIF_File.iMulti2DType==e2x2In1CCD ||
        TestIF_File.iMulti2DType==e2x2In2CCD))                                  //Steven 20240612 : for 對角2D
    {
        mtInSHBarCodePos->XItem=InArmSuck.iShtCol*2;

        for(int i=0; i<InArmSuck.iShtCol; i++)
        {
            mtInSHBarCodePos->SetCellNumber(i*2,   1, fBarCode->GetMovePos(0, i, 0, false));    //Sht1
            mtInSHBarCodePos->SetCellNumber(i*2+1, 1, fBarCode->GetMovePos(0, i, 1, false));

            mtInSHBarCodePos->SetCellNumber(i*2,   0, fBarCode->GetMovePos(1, i, 0, false));    //Sht2
            mtInSHBarCodePos->SetCellNumber(i*2+1, 0, fBarCode->GetMovePos(1, i, 1, false));
        }
        #ifdef SOFT_SIMULTE
        btnTStep->Visible=true;
        #else
        btnTStep->Visible=false;
        #endif
    }
    else
    {
        mtInSHBarCodePos->XItem=InArmSuck.iShtCol;

        for(int i=0; i<InArmSuck.iShtCol; i++)
        {
            mtInSHBarCodePos->SetCellNumber(i, 1, fBarCode->GetMovePos(0, i, 0, false));
            mtInSHBarCodePos->SetCellNumber(i, 0, fBarCode->GetMovePos(1, i, 0, false));
        }

        btnTStep->Visible=false;
    }
}
//------------------------------------------------------------------------------
bool TfShuttleMove::VerifyInArmSafetyPosition()
{
    bool _b=(InArmZSafe(DETECT_SENSOR_FLAG)==-1);                               //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
    return _b;
}
//---------------------------------------------------------------------------
bool TfShuttleMove::VerifyOutArmSafetyPosition()
{
    bool _b=(CheckOutArmZNeedHome()==-1);                                       //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
    return _b;
}
//---------------------------------------------------------------------------
bool TfShuttleMove::VerifyIndexArmSafetyPosition()                            
{
    bool _b=(MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe) &&
             MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe));
    return _b;
}
//---------------------------------------------------------------------------
bool TfShuttleMove::VerifySafetyPositionInOutIndex()
{
    bool _b=(VerifyInArmSafetyPosition()==true &&                               //JerryYang 20160727 確保In/Out arm及Index arm在安全位置
             VerifyOutArmSafetyPosition()==true &&
             VerifyIndexArmSafetyPosition()==true);
    if(_b==false)
    {
        ShowMyMessage("In/Out arm or Index arm are not in home position", "In/Out arm或是Index arm不在原點上");
    }
    return _b;
}
//---------------------------------------------------------------------------
void TfShuttleMove::ShowShuttleMaintainInfo(AnsiString sMsg)
{
    if(meShuttleMaintain->Lines->Count>1024)
        meShuttleMaintain->Clear();
    meShuttleMaintain->Lines->Add(sMsg);
}
//---------------------------------------------------------------------------
void __fastcall TfShuttleMove::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    SystemStart=false;                                                          //JerryYang 20171016 (wei) form close 時,機台要停住
    fShow=false;                                                                //JimmyChiu 20211116 AutoTeach
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::sbSensorLatchClick(TObject *Sender)
{
    sbSensorLatch->Down=false;
    fLtcSensor->ShowModal();
}
//------------------------------------------------------------------------------
void TfShuttleMove::SaveInOutArmZHeight()                                       //Frank 20171213 (Steven) modify
{
    Tech.iInArmLoadStagePickZ2  = iInArmAutoZHeight;
    Tech.iOutArmPlaceZ2         = iOutArmAutoZHeight;

    InArmOffSet_File[InOfsLoader]->SetPickUp(0);
    OutArmOffSet_File[OutOfsAuto1]->SetPlace(0);

    fTeach->SaveFile(true);
    InitShuttleThreadParameter();                                               //Steven 20120921 : Teach做完要重新Init一次
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::edInSH2Sen7DetectPosClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, MOT[MInShuttle2].Motor->PSoftLimitP, MOT[MInShuttle2].Motor->PSoftLimitN); //kevin 20170922 (wei) 小鍵盤上下限
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::edInSH1Sen7DetectPosClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, MOT[MOutShuttle1].Motor->PSoftLimitP, MOT[MOutShuttle1].Motor->PSoftLimitN);
}
//------------------------------------------------------------------------------
void __fastcall TfShuttleMove::btnTStepClick(TObject *Sender)
{
     bSetupStep=true;
     fMain->Start("btnTStepClick");
}
//---------------------------------------------------------------------------
void __fastcall TfShuttleMove::mtOutSHDetectPosMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CosFunction.bAutoTeachOutShuttleLantch && Button==mbLeft)
    {
        DWORD CurrentTime=GetTickCount();
        DWORD DoubleClickInterval=500;
        mtOutSHDetectPos->ConvertIndexCells(X,Y);
        int Col=X;
        int Row=Y;
        if (CurrentTime-LastClickTime<=DoubleClickInterval &&
            Col==LastClickX &&
            Row==LastClickY)
        {
            LastClickTime=0;
            int ret=ShowMyMessageBox_YES_NO("Do you confirm moving the shuttle to the position?", "確認移動Shuttle到指定位置?");
            if(ret==1)
            {
                iMoveToTarget=Col;
                iShuttleMoveTask=1;
                if(Row==0)btOutSH2ZDetectPos->Click();
                else if(Row==1)btOutSH1ZDetectPos->Click();
                else if(Row==2)btOutSH2OneRowDetectPos->Click();
                else if(Row==3)btOutSH1OneRowDetectPos->Click();
            }
        }
        else
        {
            LastClickTime=CurrentTime;
            LastClickX=Col;
            LastClickY=Row;
        }
    }
}
//------------------------------------------------------------------------------
