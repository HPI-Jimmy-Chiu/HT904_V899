#include "MachineDefine.h"
#pragma hdrstop

#include "AutoRetest.h"

//---------------------------------------------------------------------------

#include "acatchtray.h"

#include "MachineType.h"
#include "note.h"
#include "mymessbox.h"
#include "asendic.h"
#include "mymotor.h"
#include "aArmHeader.h"
#include "mycylin.h"
#include "mysensor.h"
#include "MyKitSuck.h"
#include "main.h"
#include "cmydef.h"
#include "csystem.h"
#include "FTPClient.h"
#include "database.h"
#include "cCounterClear.h"
#include "uLotInfo.h"
#include "common.h"
#include "cinitial.h"
#include "SCK_ART.h"
#include "cTrayMapping.h"

#pragma package(smart_init)

int iAutoRetestTask                     =1;
int iTrayArm_AutoRT_Task                =1;
int iAuto_AutoRT_Task[MAX_AUTO_TRAY]    ={1};
int iLoader_AutoRT_Task                 =1;
int iTrayArm_PickFromAuto_AutoRT_Task   =1;
int iTrayArm_PlaceToLoad_AutoRT_Task    =1;
int iARTCatchAutoTrayFinish             =0;                                     //ChungHung 20141208 add fix ART catch Tray 與 Auto Tray Send to Rear 互搶
bool bTrayArmOK_AutoRT                  =false;
bool bAutoOK_AutoRT[MAX_AUTO_TRAY]      ={false};
bool bLoadOK_AutoRT                     =false;
bool bTrayArmPlaceToLoadFinish          =false;
TQPF_Timer DoARTFixTrayDelay;                                                   //Sam 20200203 : 搬完 Tray 後要夾
TQPF_Timer tCommandDelay;
//---------------------------------------------------------------------------
bool DoAutoRetest(bool bReset)
{
    bool bResult=false;
    int &Task=iAutoRetestTask;

    if(bReset==true)
    {
        Task=1;
        bTrayArmOK_AutoRT=false;
        bAutoOK_AutoRT[eAuto1]=(Prod.iTrayType[eAuto1]==tTrayAuto)?false:true;
        bAutoOK_AutoRT[eAuto2]=(Prod.iTrayType[eAuto2]==tTrayAuto)?false:true;
        bAutoOK_AutoRT[eAuto3]=(Prod.iTrayType[eAuto3]==tTrayAuto)?false:true;
        bAutoOK_AutoRT[eAuto4]=(Prod.iTrayType[eAuto4]==tTrayAuto)?false:true;
        bAutoOK_AutoRT[eAuto5]=(Prod.iTrayType[eAuto5]==tTrayAuto)?false:true;
        bAutoOK_AutoRT[eAuto6]=(Prod.iTrayType[eAuto6]==tTrayAuto)?false:true;
        bLoadOK_AutoRT   =false;
        bARTTrayArmOK    =false;
        SetMotorScaleSpeed(MTrayX, IniConfig.iARTTrayArmSpeed);                 //Steven 20170118 : For ART Tray Arm Speed
        if(TestIF_File.bRENESAS_EnableFTCT==true)                               //RogerYang 20251019 : 瑞薩FT-CT
            fMain->RENESAS_Server->iFTRTCntTmpFor70=TestIF_File.iSCKART_TryCnt;

        return bResult;
    }

    if(MOT[MInArmX].CompareCommandPos(Prod.iInArmSafeX, 2)!=1 ||                //Steven 20240110 : fixed for ART hang up
       MOT[MInArmY].CompareCommandPos(Prod.iInArmSafeY, 2)!=1)
    {
        if(MoveInArm2XYToWait()==false)
            return false;
    }

    if(MOT[MOutArmY].CompareCommandPos(Prod.iOutArmSafeY, 2)!=1)                //Steven 20240110 : fixed for ART hang up  //Steven 20211202 : 修正因為更換X安全位置導致ART hang up
    {
        if(MoveOutArmXY_ToFix_Tray_Full()==false)
            return false;
    }

    switch(Task)
    {
        case 1:
            Cylinder[C_HotplateVibration].Off();                                //JerryYang 20200311 修正ART搬tray時振動馬達一直震
            Cylinder[C_TrayVibration].Off();
            Task=2;
            break;
        case 2:
            fSCKART->iCurrent93KARTStep=6;
            if(IniConfig.bEnable_SECS_GEM==true)                                //ChungHung 20150511 modify
                EventReport(SECS_EVENT.ArtReceiveTraySTART);                    //60    開關Site

            if(CUSTOMER_CODE==CC_KYEC_XILINX)                                   //Frank 20160728 add
            {
                bAutoRetestMusic=true;
            }

            DoTrayArm_AutoRT(true);                                             //初始化 TrayArm 動作
            DoAuto_AutoRT(eAuto1, true);                                        //初始化 Auto1 入Tray 動作
            DoAuto_AutoRT(eAuto2, true);                                        //初始化 Auto2 入Tray 動作
            DoAuto_AutoRT(eAuto3, true);                                        //初始化 Auto3 入Tray 動作
            DoAuto_AutoRT(eAuto4, true);                                        //初始化 Auto4 入Tray 動作
            DoAuto_AutoRT(eAuto5, true);                                        //初始化 Auto5 入Tray 動作
            DoAuto_AutoRT(eAuto6, true);                                        //初始化 Auto6 入Tray 動作
            DoLoader_AutoRT(true);                                              //初始化 Loader 出Tray 動作
            iARTCatchAutoTrayFinish=0;                                          //ChungHung 20141208 add fix ART catch Tray 與 Auto Tray Send to Rear 互搶
            Task=100;
        case 100:
            fSCKART->iCurrent93KARTStep=6;
            if(bAutoOK_AutoRT[eAuto1]==false)
            {
                if(Prod.iTrayType[eAuto1]==tTrayAuto)
                    bAutoOK_AutoRT[eAuto1]=DoAuto_AutoRT(eAuto1);
                else
                    bAutoOK_AutoRT[eAuto1]=true;
            }

            if(bAutoOK_AutoRT[eAuto2]==false)
            {
                if(Prod.iTrayType[eAuto2]==tTrayAuto)
                    bAutoOK_AutoRT[eAuto2]=DoAuto_AutoRT(eAuto2);
                else
                    bAutoOK_AutoRT[eAuto2]=true;
            }

            if(bAutoOK_AutoRT[eAuto3]==false)
            {
                if(Prod.iTrayType[eAuto3]==tTrayAuto)
                    bAutoOK_AutoRT[eAuto3]=DoAuto_AutoRT(eAuto3);
                else
                    bAutoOK_AutoRT[eAuto3]=true;
            }

            if(bAutoOK_AutoRT[eAuto4]==false)
            {
                if(Prod.iTrayType[eAuto4]==tTrayAuto)
                    bAutoOK_AutoRT[eAuto4]=DoAuto_AutoRT(eAuto4);
                else
                    bAutoOK_AutoRT[eAuto4]=true;
            }

            if(bAutoOK_AutoRT[eAuto5]==false)
            {
                if(Prod.iTrayType[eAuto5]==tTrayAuto)
                    bAutoOK_AutoRT[eAuto5]=DoAuto_AutoRT(eAuto5);
                else
                    bAutoOK_AutoRT[eAuto5]=true;
            }

            if(bAutoOK_AutoRT[eAuto6]==false)
            {
                if(Prod.iTrayType[eAuto6]==tTrayAuto)
                    bAutoOK_AutoRT[eAuto6]=DoAuto_AutoRT(eAuto6);
                else
                    bAutoOK_AutoRT[eAuto6]=true;
            }

            if(bTrayArmOK_AutoRT==false)
            {
                bTrayArmOK_AutoRT=DoTrayArm_AutoRT();
            }

            if(bLoadOK_AutoRT==false)
            {
                bLoadOK_AutoRT=DoLoader_AutoRT();
            }

            if(bTrayArmOK_AutoRT==true)
            {
                iArt_AutoTotal[eAuto1][1]=LastSet.BinCT_ART[0][e3Auto1];        //kevin 20170830 (Steven) 記錄ART Auto Tray IC數量
                iArt_AutoTotal[eAuto2][1]=LastSet.BinCT_ART[0][e3Auto2];        //kevin 20170830 (Steven) 記錄ART Auto Tray IC數量
                iArt_AutoTotal[eAuto3][1]=LastSet.BinCT_ART[0][e3Auto3];        //kevin 20170830 (Steven) 記錄ART Auto Tray IC數量
                iArt_AutoTotal[eAuto4][1]=LastSet.BinCT_ART[0][e3Auto4];
                iArt_AutoTotal[eAuto5][1]=LastSet.BinCT_ART[0][e3Auto5];
                iArt_AutoTotal[eAuto6][1]=LastSet.BinCT_ART[0][e3Auto6];
            }

            if(bTrayArmOK_AutoRT &&
               bAutoOK_AutoRT[eAuto1] &&
               bAutoOK_AutoRT[eAuto2] &&
               bAutoOK_AutoRT[eAuto3] &&
               bAutoOK_AutoRT[eAuto4] &&
               bAutoOK_AutoRT[eAuto5] &&
               bAutoOK_AutoRT[eAuto6])
            {
                bARTTrayArmOK=true;                                             //wei 20151217 ART 回盤OK
            }

            if(bTrayArmOK_AutoRT &&
               bAutoOK_AutoRT[eAuto1] &&
               bAutoOK_AutoRT[eAuto2] &&
               bAutoOK_AutoRT[eAuto3] &&
               bAutoOK_AutoRT[eAuto4] &&
               bAutoOK_AutoRT[eAuto5] &&
               bAutoOK_AutoRT[eAuto6] &&
               bLoadOK_AutoRT)
            {
                if(CUSTOMER_CODE==CC_KYEC_XILINX)                               //Frank 20160728 add
                {
                    bAutoRetestMusic=false;
                }

                if(CosFunction.bAutoRetestGPIBmode==true)                       //jou 2015-10-02 Auto Retest GPIB mode
                {
                    InitLoaderTrackDetectICFloatingTask();                      //Sam 20200316 : Loader Detect Tray
                    Task=110;                                                   //Sam 20200316 : Loader Detect Tray
                }
                else
                {
                    bResult=true;
                }

                if(CUSTOMER_CODE==CC_TSMC_TAINAN)                               //wei 20170119 (jou) ATR FT/RT count
                {
                    iATRFtRtMode=1;
                }
            }
            break;
        case 110:                                                               //Sam 20200316 : Loader Detect Tray
            if(Sen[SnLoaderTrackDetect].Enable==true)
            {
                if(DoLoaderTrackDetectICFloating())
                {
                     Task=130;                                                  //Sam 20200320 : TrayY_Fixer 先勾推一下
                }
            }
            else
            {
                Task=130;                                                       //Sam 20200320 : TrayY_Fixer 先勾推一下
            }
            break;
       case 130:                                                                //Sam 20200320 : TrayY_Fixer 先勾推一下
            if(Cylinder[C_TrayY_Fixer].Push())
            {
                DoARTFixTrayDelay.SetSecAndOn(Ld_UldDelayTime.LD_FixTrayDely+0.5);
                Task=135;
            }
            break;
        case 135:
            if(DoARTFixTrayDelay.Off())
                Task=140;
            break;
        case 140:
            if(Cylinder[C_TrayY_Fixer].Pop())
                Task=150;
            break;
        case 150:
            Cylinder[C_LoaderEdgePush].On();
            DoARTFixTrayDelay.SetSecAndOn(0.2);
            Task=160;
            break;
        case 160:
            if(DoARTFixTrayDelay.Off())
                Task=170;
            break;
        case 170:
            if(Cylinder[C_TrayY_Fixer].Push())
            {
                Task=200;
            }
            break;
        case 200:                                                               //Sam 20201113 : ART 完要跑 Tray Map
            if(USE_TRAY_MAPPING==etmInstall &&
               TestIF_File.bEnableTrayMap)
            {
                Task=205;
            }
            else
            {
                Task=250;
            }
            break;
        case 205:
            if(IsMoveInArm2XYToWait())
            {
                InitialTrayMapTask(iTray_Map);
                bLoaderTrayAction=true;
                Task=215;
            }
            else
            {
                Task=210;
            }
            break;
        case 210:
            if(MoveInArm2XYToWait())
            {
                Task=205;
            }
            break;
        case 215:
            InitialTrayMapTask(iTray_Map);                                      //Sam 20200507 : 整合 Tray Map Function
            Task=220;
            break;
        case 220:
            if(fTrayMapping->DoTrayMapCCD(iTray_Map)==true)                     //Sam 20200507 : 整合 Tray Map Function
            {
                Task=225;
            }
            else if(TestIF_File.bEnableTrayMap==false)
            {
                Task=225;
            }
            break;
        case 221:
            if(Cylinder[C_TrayX_UpDown].Pop())                                  //Steven 20231026 : 針對TrayArm動作加上保護
                Task=225;
            else
                break;
        case 225:
            if(MTrayXCanSafeMove()==false)
            {
                Task=221;
                break;
            }

            if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
            {
                if(TrayArmMotorMove(Prod.iXTrayColor))
                {
                    MOT[MInArmX].fCanMove=true;
                    MOT[MInArmY].fCanMove=true;
                    bLoaderTrayAction=false;
                    Task=250;
                }
            }
            else
            {
                if(TrayArmMotorMove(Prod.iXTrayEmpty))
                {
                    MOT[MInArmX].fCanMove=true;
                    MOT[MInArmY].fCanMove=true;
                    bLoaderTrayAction=false;
                    Task=250;
                }
            }
            break;
        case 250:
            if(fMain->hanaART->IsHanaArtAvailable()==true)                      //JimmyChiu 20241023 HANA ART Function
            {
            }
            else if(CUSTOMER_CODE==CC_PTI &&
                    IniConfig.bB03_TesterReport &&                              //Sam 20240809 : PTI ART 模式
                    LastSet.bWaitEndLotAutoRetestGPIB==false)
            {
                break;
            }

            LastSet.bWaitStartLotAutoRetestGPIB=false;
            LastSet.bEndLotAutoRetestGPIB=false;
            fMain->SetLotState(4);                                              //ART RT Start
            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20220923 : add SPIL
            {
                ShowMyMessage("(1)Test summary lot end finished, please key in EAP GUI to do next process.\r\n(2)After put the IC on the handler loader and then press handler Start button to start test",
                            "(1)測試報表已結檔, 請操作EAP GUI 繼續下一步流程.\r\n(2)handler完成上料後按Start鍵 ,開始測試.");
            }
            Task=300;
            break;
        case 300:                                                               //jou 2015-10-02 Auto Retest GPIB mode
            if(LastSet.bWaitStartLotAutoRetestGPIB==true ||
               (fMain->hanaART->IsHanaArtAvailable()==true &&
                fMain->hanaART->IsContactAvailable()==true) ||
               (CosFunction.iAutoRetestTCPmode==2 &&                            //Rogeryang 20250918 : 瑞薩FT-CT
                TestIF_File.bRENESAS_EnableFTCT==true))
            {
                if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport)        //Sam 20240809 : PTI ART 模式
                {
                    //不要變更
                }
                else
                {
                    fLotInfo->cbRunMode->Text="RT";
                }

                if(CUSTOMER_CODE==CC_AMKOR_Korea)                               //Steven 20231214 : #R231026-ATK-H9-02 , V3.21.808.7 , Request Count Clear when ART from FT to RT.
                {
                    fMain->Clarn_Data(0, "Auto Retest");
                }

                fMain->Clarn_Data(7, "WaitStartLotAutoRetestGPIB Sorting Count");
                bResult=true;
                Task=400;
            }
            break;
    }

    if(bResult==true)                                                           //Steven 20170118 : For ART Tray Arm Speed
    {
        SetMotorScaleSpeed(MTrayX, ArmSpeed[TrayArm].iBodySP);
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool DoTrayArm_AutoRT(bool bReset)
{
    bool bResult=false;
    bool ret;
    int &Task=iTrayArm_AutoRT_Task;
    static int Pos;

    if(bReset==true)
    {
        Task=1;
        bTrayArmOK_AutoRT=false;
        bTrayArmPlaceToLoadFinish=false;
        DoPlaceToLoader_AutoRT(true);                                           //ChungHung 20140815 add AutoRetest

        for(int i=eAuto1; i<=iAutoRight; i++)
        {
            DoPickFromAuto_AutoRT(i, true);
            for(int j=0; j<2; j++)
                iArt_AutoTotal[i][j]=0;                                         //kevin 20170830 (Steven) 記錄ART Auto Tray 數量
        }
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(MOT[MTrayX].fHasTray)
            {
                bTrayArmPlaceToLoadFinish=false;
                Task=300;
            }
            else if((bAutoOK_AutoRT[eAuto1]==false ||
                     bAutoOK_AutoRT[eAuto2]==false ||
                     bAutoOK_AutoRT[eAuto3]==false ||
                     bAutoOK_AutoRT[eAuto4]==false ||
                     bAutoOK_AutoRT[eAuto5]==false ||
                     bAutoOK_AutoRT[eAuto6]==false) &&
                     MOT[MTrayX].fHasTray==false)
            {
                bTrayArmPlaceToLoadFinish=false;
                Task=100;
            }
            else                                                                //做完了
            {
                bTrayArmPlaceToLoadFinish=true;
                Task=400;
            }
            break;
        case 100:
            Pos=-1;
            for(int i=eAuto1; i<=iAutoRight; i++)
            {
                if(bAutoOK_AutoRT[i]==false && MOT[iMMAuto[i]].fHasTray)
                {
                    Pos=i;
                    break;                                                      //跳出迴圈
                }
            }

            if(Pos!=-1)
            {
                DoPickFromAuto_AutoRT(Pos, true);
                Task=200;
            }
            else
            {
                Task=1;
            }
            break;
        case 200:
            ret=DoPickFromAuto_AutoRT(Pos);
            if(ret)
            {
                if(Pos==0)                                                      //Steven 20220331 : Tray Arm動作加上紀錄
                {
                    NewRecordProcess("MES0653", "Tray Arm - Pick tray from Auto 1", "DoTrayArm_AutoRT");
                }
                else if(Pos==1)
                {
                    NewRecordProcess("MES0654", "Tray Arm - Pick tray from Auto 2", "DoTrayArm_AutoRT");
                }
                else
                {
                    NewRecordProcess("MES0655", "Tray Arm - Pick tray from Auto 3", "DoTrayArm_AutoRT");
                }
                iArt_AutoTotal[Pos][0]++;                                       //kevin 20170830 (Steven) 記錄ART Auto Tray 數量
                DoPlaceToLoader_AutoRT(true);
                Task=1;
            }
            break;
        case 300:
            ret=DoPlaceToLoader_AutoRT();
            if(ret)
            {
                bTrayArmPlaceToLoadFinish=true;
                Task=1;
            }
            break;
        case 400:
            ret=TrayArmMotorMove(Prod.iXTrayEmpty);
            if(ret)
            {
                bTrayArmPlaceToLoadFinish=true;
                bResult=true;
                Task=1;
            }
            break;
    }
    return bResult;
}
//---------------------------------------------------------------------------
TQPF_Timer DoPickFromAuto_AutoRTDelay[MAX_AUTO_TRAY];
//---------------------------------------------------------------------------
bool DoPickFromAuto_AutoRT(int Pos, bool bReset)
{
    bool bResult=false;
    static bool bDuplicateErr[MAX_AUTO_TRAY]={false};
    static bool bC_Auto_Fixer_Off[MAX_AUTO_TRAY]={false};
    bool ret;
    int AlarmRet;
    int &Task=iTrayArm_PickFromAuto_AutoRT_Task;

    if(bReset==true)
    {
        bDuplicateErr[Pos]=false;
        iRetestNoIC[Pos]=0;                                                     //wei 20160203 回盤為空盤不計數
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            iARTCatchAutoTrayFinish=1;                                          //ChungHung 20141208 add fix ART catch Tray 與 Auto Tray Send to Rear 互搶
            Task=100;
        case 100:                                                               //先確認Out Arm在安全點
            ret=MoveOutArmXY_ToFix_Tray_Full();
            if(ret)
            {
                DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT);
                Cylinder[C_TrayCover].Off();                                    //ChungHung 20140625 先讓蓋子在上
                Task=200;
            }
            break;
        case 200:                                                               //再確認汽缸在上
            ret=Cylinder[C_TrayX_UpDown].Pop();
            if(ret)
            {
                bC_Auto_Fixer_Off[Pos]=false;
                Task=300;
            }
            break;
        case 300:                                                               //移動到Auto 上面
            ret=TrayArmMotorMove(Prod.iXTrayAuto_ART[Pos]);                     //kevin 20170831 (Steven) add ART-offset
            Cylinder[C_AutoEdgePush[Pos]].Off();
            Cylinder[C_AutoUpPress[Pos]].Off();                                 //JerryYang 20190423 新增unloader壓tray
            bC_Auto_Fixer_Off[Pos]=Cylinder[C_AutoSide_Fixer[Pos]].Pop();

            if(ret && bC_Auto_Fixer_Off[Pos])
            {
                if(Cylinder[C_AutoUpPress[Pos]].Enable==true)                   //JerryYang 20190423 新增unloader壓tray
                {
                    DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(3);
                    Task=350;
                }
                else
                {
                    Task=400;
                }

                if(CUSTOMER_CODE==CC_KYEC_LEE &&
                   MOT[iMMAuto[Pos]].Tray.HasRealIC()==false)                   //wei 20160203 回盤為空盤不計數
                    iRetestNoIC[Pos]++;
            }
            break;
        case 350:
            if(Sen[SnAutoUpPress[Pos]].IsOff())                                 //JerryYang 20190423 新增unloader壓tray
            {
                Task=400;
            }

            if(DoPickFromAuto_AutoRTDelay[Pos].Off())
            {
                ShowMyMessage("Unloader up press pop error", "Unloader 上壓汽缸放開異常");
                Cylinder[C_AutoUpPress[Pos]].Off();                             //JerryYang 20190423 新增unloader壓tray
                DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(3);
                break;
            }
            break;
        case 400:                                                               //汽缸向下
            ret=Cylinder[C_TrayX_UpDown].Push();
            if(ret)
            {
                Task=150;                                                       //kevin 20150812
                if(USE_AUTO_RETEST==eartInstall)                                //ChungHung 20140625 add AutoRetest catch Tray use 2 Output
                    C_CatchTray_Fix_Puch(true);
                if(IniConfig.bSPILFunction &&
                   IniConfig.bC03UseCatchTray &&
                   IniConfig.bA10_AutoReTest)                                   //JerryYang 20220923 : for矽品ART TRAY ARM上下汽缸調整較慢，下降delay time太短會alarm
                {
                    DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT+1.5);
                }
                else
                {
                    DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT);
                }
            }
            break;
        case 150:                                                               //kevin 20150812 上蓋下來
            if(DoPickFromAuto_AutoRTDelay[Pos].Off()==false)
                break;

            Task=600;                                                           //ChungHung 20140626 先夾在蓋蓋子
            if(USE_AUTO_RETEST==eartInstall)                                    //ChungHung 20140625 add AutoRetest catch Tray use 2 Output
                C_CatchTray_Fix_Puch(true);
            DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT*2);
            break;
        case 600:                                                               //夾起來
            if(DoPickFromAuto_AutoRTDelay[Pos].Off()==false)
                break;

            if(LastSet.iRealDummy==DUMMY)                                       //Steven 20100618
            {
                ret=true;
            }
            else //if(IniConfig.bC03UseCatchTray)                               //use catch Tray Fix
            {
                if(USE_AUTO_RETEST==eartInstall)                                //ChungHung 20140625 add AutoRetest catch Tray use 2 Output
                    ret=C_CatchTray_Fix_Puch();
                else
                    ret=Cylinder[C_CatchTray_Fix].Push();

                if(ret)
                {
                    if(LastSet.iRealDummy!=DUMMY && Sen[SnCatchTrayFix1On].IsOn()==true && Sen[SnCatchTrayFix2On].IsOn()==true)
                    {
                        Task=700;
                        break;
                    }
                }
            }

            if(ret)
            {
                Task=650;
            }
            break;
        case 650:                                                               //Steven 20190314 : Fix ART catch auto tray hang up
            MOT[MTrayX].fHasTray=true;
            MOT[iMMAuto[Pos]].ClearTray(__FUNC__);
            CatchTraySuck.iWhichTray=3+Pos;                                     //Steven 20220331 : 紀錄Tray從哪來的
            CatchTraySuck.SetItemData(0, 0, 2);
            DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(ArmSpeed[TrayArm].dVacuumTI);
            if(CosFunction.bThickTrayNoNeedCover &&
               UserDefForm[0].ZDepth>1500)                                      //Steven 20200723 : 太厚的Tray不能蓋蓋子
            {

            }
            else
            {
                Cylinder[C_TrayCover].On();                                     //kevin 20150812
            }
            Task=500;                                                           //ChungHung 20140626 先夾在蓋蓋子
            break;
        case 500:                                                               //蓋蓋子
            #ifdef SOFT_SIMULTE
            Task=1000;
            break;
            #else
            if(DoPickFromAuto_AutoRTDelay[Pos].Off()==false)
                break;

            if(LastSet.iRealDummy==DUMMY)
            {
                ret=true;
            }
            else
            {
                if(CosFunction.bThickTrayNoNeedCover &&
                   UserDefForm[0].ZDepth>1500)                                  //Steven 20200723 : 太厚的Tray不能蓋蓋子
                {

                }
                else
                {
                    Cylinder[C_TrayCover].On();                                 //kevin 20150812
                }
                Task=900;                                                       //kevin 20150812 //ChungHung 20140626 先夾在蓋蓋子
            }
            break;
            #endif
        case 900:                                                               //上升
            if(DoPickFromAuto_AutoRTDelay[Pos].Off())
            {
                ret=Cylinder[C_TrayX_UpDown].Pop();
                if(ret)
                {

                    Task=1000;
                }
            }
            break;
        case 1000:                                                              //結束
            iARTCatchAutoTrayFinish=2;                                          //ChungHung 20141208 add fix ART catch Tray 與 Auto Tray Send to Rear 互搶
            bDuplicateErr[Pos]=false;
            bResult=true;
            break;

//例外處理----------------------------------
        case 700:                                                               //夾取錯誤先將氣缸升上來
            Cylinder[C_TrayX_UpDown].Off();
            DoPickFromAuto_AutoRTDelay[Pos].SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT*2);
            Task=750;
        case 750:                                                               //Steven 20190314 : Fix ART catch auto tray hang up
            if(DoPickFromAuto_AutoRTDelay[Pos].Off())
            {
                if(LastSet.iRealDummy!=DUMMY &&
                   Sen[SnCatchTrayFix1On].IsOn()==true &&
                   Sen[SnCatchTrayFix2On].IsOn()==true)
                {
                    Task=800;
                }
                else
                {
                    Task=650;
                }
            }
            break;
        case 800:                                                               //夾取錯誤Alarm
            AlarmRet=ShowErrorMessage("JAM0601", K_SKIP|K_RETRY, MTrayX, bDuplicateErr[Pos], __FUNC__);

            if(AlarmRet==K_RETRY)
            {
                bDuplicateErr[Pos]=true;
                Task=400;
            }
            else if(AlarmRet==K_SKIP)
            {
                bDuplicateErr[Pos]=false;
                Task=900;
                MOT[MTrayX].fHasTray=false;
                MOT[iMMAuto[Pos]].ClearTray(__FUNC__);
            }
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
TQPF_Timer DoPlaceToLoader_AutoRTDelay;
TQPF_Timer DoPlaceToLoader_TrayCoverTimeOut;                                    //Frank 20161212 (Jou) Cover Cylinder Time Out
//---------------------------------------------------------------------------
bool DoPlaceToLoader_AutoRT(bool bReset)
{
    bool bResult=false;
    bool ret;
    static bool bDuplicateErr=false;
    int AlarmRet;
    int &Task=iTrayArm_PlaceToLoad_AutoRT_Task;
    AnsiString Str;

    if(bReset==true)
    {
        bDuplicateErr=false;
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            Task=1;
        case 100:                                                               //先確認Iut Arm在安全點
            ret=MoveInArm2XYToWait();
            if(ret)
            {
                Task=200;
            }
            break;
        case 200:                                                               //再確認汽缸在上
            ret=Cylinder[C_TrayX_UpDown].Pop();
            if(ret)
            {
                Task=300;
            }
            break;
        case 300:                                                               //移動到Loader 上面
            ret=TrayArmMotorMove(Prod.iXTrayLoad_ART);                          //kevin 20170831 (Steven) add ART
            if(ret)
            {
                                                                                //ChungHung 20140625 add
                Cylinder[C_TrayY_Fixer].Off();
                Cylinder[C_LoaderEdgePush].Off();
                Cylinder[C_LoaderUpPress].Off();                                //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
                Task=400;
            }
            break;
        case 400:                                                               //汽缸向下
            if(MOT[MMTrayY].fHasTray==false &&
               MOT[MMTrayY_Car].fHasTray==false)                                //Sam 20210621 : 退出來時還要一直轉，不然會回車導致翻盤
            {
                ret=Cylinder[C_TrayX_UpDown].Push();
                if(ret)
                {
                    if(USE_AUTO_RETEST==eartInstall)                            //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
                        C_CatchTray_Fix_Pop(true);
                    if(IniConfig.bSPILFunction &&
                       IniConfig.bC03UseCatchTray &&
                       IniConfig.bA10_AutoReTest)                               //JerryYang 20220923 : for矽品ART TRAY ARM上下汽缸調整較慢，下降delay time太短會alarm
                    {
                        DoPlaceToLoader_AutoRTDelay.SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT+1.5);   //Steven 20110720 : 等待Hand Down Time
                    }
                    else
                    {
                        DoPlaceToLoader_AutoRTDelay.SetSecAndOn(ArmSpeed[TrayArm].dTAHeadDownIT);       //Steven 20110720 : 等待Hand Down Time
                    }
                    Task=500;
                }
            }
            break;
        case 500:                                                               //放開夾子

            if(DoPlaceToLoader_AutoRTDelay.Off())
            {
                if(LastSet.iRealDummy==DUMMY)                                   //Steven 20120417 : Dummy不要夾起來
                {
                    ret=true;
                }
                else //if(IniConfig.bC03UseCatchTray)
                {
                    if(USE_AUTO_RETEST==eartInstall)                            //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
                        ret=C_CatchTray_Fix_Pop();
                    else
                        ret=Cylinder[C_CatchTray_Fix].Pop();
                }

                if(ret)
                {
                    Task=600;
                    DoPlaceToLoader_AutoRTDelay.SetSecAndOn(ArmSpeed[TrayArm].dCTAirOn);  //Steven 20110720 : 等待Counter Air On Time
                    DoPlaceToLoader_TrayCoverTimeOut.SetSecAndOn(100);          //kevin 20170810 (Steven) 5->100                      //Frank 20161212 (Jou) Cover Cylinder Time Out
                }
            }
            break;
        case 600:          //開蓋子
            if(DoPlaceToLoader_AutoRTDelay.Off())
            {
                if(LastSet.iRealDummy==DUMMY)                                   //Steven 20100618
                {
                    ret=true;
                }
                else
                {
                    Cylinder[C_TrayCover].Off();                                //ChungHung 20140625 open Cover use Off
                    ret=true;                                                   //JerryYang 20170907 (Steven) 這裡不需要判斷
                }

                if(ret)
                {
                    Task=700;
                    DoPlaceToLoader_AutoRTDelay.SetSecAndOn(ArmSpeed[TrayArm].dCTAirOn);  //Steven 20110720 : 等待Counter Air On Time
                }
            }
            break;
        case 700:                                                               //上升
            if(DoPlaceToLoader_AutoRTDelay.Off())
            {
                ret=Cylinder[C_TrayX_UpDown].Pop();
                if(ret)
                {
                    if(LastSet.iRealDummy!=DUMMY &&
                       Sen[SnLoaderSureTray].IsOff())                           //沒有偵測到放tray
                    {
                        Task=800;
                    }
                    else
                    {
                        Task=900;
                    }
                }
            }
            break;
        case 900:                                                               //結束
            MOT[MTrayX].fHasTray=false;                                         //Steven 20170216 (wei) : Tray Arm資料交換改到case 900
            if(CatchTraySuck.iWhichTray!=-1)
                Str.sprintf("DoPlaceToLoader_AutoRT - From %s", sTrayPosName[CatchTraySuck.iWhichTray]);
            else
                Str.sprintf("DoPlaceToLoader_AutoRT - From Unkonwn");
            NewRecordProcess("MES0670", "Tray Arm - place tray to Loader", Str);//Steven 20220331 : Tray Arm動作加上紀錄
            CatchTraySuck.iWhichTray=-1;                                        //Steven 20220331 : 紀錄Tray從哪來的
            CatchTraySuck.SetItemData(0, 0, NULL_IC);
            MOT[MMTrayY].SetTray(HAS_IC, __FUNC__);
            bDuplicateErr=false;
            bResult=true;
            break;
//例外處理-------------------------------------
        case 800:                                                               //放到Loader上檢查Sensor錯誤
            AlarmRet=ShowErrorMessage("JAM0911", K_RETRY|K_SKIP, MMTrayY, bDuplicateErr, __FUNC__);

            if(AlarmRet==K_RETRY)
            {
                Task=700;
                bDuplicateErr=true;
            }
            else if(AlarmRet==K_SKIP)
            {
                bDuplicateErr=false;
                Task=900;
            }
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool DoAuto_AutoRT(int Pos, bool bReset)                                        //ChungHung 20141002 add for KYEC AutoRetest
{
    bool ret;
    bool bResult=false;
    int &Task=iAuto_AutoRT_Task[Pos];

    int bRet=0;                                                                 //kevin 20150613
    int iAutoTray[MAX_AUTO_TRAY]={0, 0, 0, 0, 0, 0};
    iAutoTray[eAuto2]=atoi(fMain->edtAuto2->Text.c_str());
    iAutoTray[eAuto3]=atoi(fMain->edtAuto3->Text.c_str());
    iAutoTray[eAuto4]=atoi(fMain->edtAuto4->Text.c_str());
    iAutoTray[eAuto5]=atoi(fMain->edtAuto5->Text.c_str());
    iAutoTray[eAuto6]=atoi(fMain->edtAuto6->Text.c_str());
    AnsiString Str1, Str2;
    static bool bflag[MAX_AUTO_TRAY]={false, false, false, false, false, false};//JerryYang 20220923 : add
    int ibuffer=0;
    bool bFlag=false;

    if(bReset==true)
    {
        Task=1;
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(Prod.bART6Tray[Pos]==false)                                      //是否有設定為需要Auto Retest
            {
                bResult=true;
            }
            else
            {
                #ifdef SOFT_SIMULTE                                             //ChungHung 20141002 add for KYEC AutoRetest
                if(iAutoTray[Pos]<=0 && CosFunction.bUseSCKART==false)
                {
                    if(MOT[iMMAuto[Pos]].fHasTray==false && MOT[iMMAuto_Car[Pos]].fHasTray==false)
                        bResult=true;
                    else if(MOT[iMMAuto[Pos]].fHasTray==false && MOT[iMMAuto_Car[Pos]].fHasTray==true)
                        Task=100;
                    else
                        bResult=false;
                }
                else
                {
                    if(MOT[iMMAuto[Pos]].fHasTray==true || MOT[iMMAuto_Car[Pos]].fHasTray==true)
                        break;
                    else
                        bResult=true;
                }
                #else
                if(LastSet.iRealDummy==DUMMY)
                {
                    if(fMain->CheckBox1->Checked==false &&
                       Sen[SnAutoTrayHasTray[Pos]].IsOn()==false &&
                       Sen[SnAutoTrayCar[Pos]].IsOn()    ==false &&
                       Sen[SnAutoTrayDetect[Pos]].IsOn() ==false &&
                       MOT[iMMAuto[Pos]].fHasTray        ==false &&
                       MOT[iMMAuto_Car[Pos]].fHasTray    ==false)
                    {
                        bResult=true;
                    }
                    else
                    {
                        Task=100;
                    }
                }
                else
                {
                    if(Sen[SnAutoTrayHasTray[Pos]].IsOn()==false &&
                       Sen[SnAutoTrayCar[Pos]].IsOn()    ==false &&
                       Sen[SnAutoTrayDetect[Pos]].IsOn() ==false &&
                       MOT[iMMAuto[Pos]].fHasTray        ==false &&
                       MOT[iMMAuto_Car[Pos]].fHasTray    ==false &&
                       (iAutoTrayCount[Pos]<=0 ||
                        CosFunction.bUseSCKART==true))                          //JerryYang 20220923 : add
                    {
                        bResult=true;
                    }
                    else
                    {
                        if(Cylinder[C_TrayX_UpDown].OffSensor() &&              //ChungHung 20140625 add
                           MOT[iMMAuto[Pos]].fHasTray==false &&
                           Sen[SnAutoTrayDetect[Pos]].IsOn())                   //Auto紀錄沒有Tray Auto偵測有Tray
                        {
                            ShowErrorMessage(sJAM1110[Pos], K_RETRY, iMMAuto[Pos]);
                            break;
                        }

                        if(CUSTOMER_CODE==CC_KYEC_LEE)
                        {
                            if(Sen[SnAutoTrayHasTray[Pos]].IsOn()==false &&
                               Sen[SnAutoTrayCar[Pos]].IsOn()    ==false &&
                               Sen[SnAutoTrayDetect[Pos]].IsOn() ==false &&
                               MOT[iMMAuto[Pos]].fHasTray        ==false &&
                               MOT[iMMAuto_Car[Pos]].fHasTray    ==false )
                            {
                                bResult=true;
                            }
                            else if(iAutoTrayCount[Pos]>0)
                            {
                                Task=100;
                            }
                            else if(Sen[SnAutoTrayHasTray[Pos]].IsOn())
                            {
                                Str1.sprintf("Auto %d Tray Over Count ,Please remove Auto %d Up Tray",Pos+1,Pos+1);
                                Str2.sprintf("Auto %d Tray超過數量,請將 Auto %d Tray盤移走",Pos+1,Pos+1);
                                ShowMyMessage(Str1, Str2);
                            }
                        }
                        else
                        {
                            if(iAutoTrayCount[Pos]>0 ||
                               CosFunction.bUseSCKART==true)                    //Steven 20170825 (wei) : Fixed for SCK ART hang up when unloader have tray
                            {
                                Task=100;
                            }
                            else if(iAutoTrayCount[Pos]==0)                     //kevin 20170824 (Steven) 有空tray
                            {
                                break;
                            }
                            else if(Sen[SnAutoTrayHasTray[Pos]].IsOn())
                            {
                                Str1.sprintf("Auto %d Tray Over Count ,Please remove Auto %d Up Tray",Pos+1,Pos+1);
                                Str2.sprintf("Auto %d Tray超過數量,請將 Auto %d Tray盤移走",Pos+1,Pos+1);
                                ShowMyMessage(Str1, Str2);
                            }
                        }
                    }
                }
                #endif
            }
            break;
        case 100:                                                               //確認要做哪個動作
            #ifdef SOFT_SIMULTE                                                 //ChungHung 20141002 add for KYEC AutoRetest
            if(CosFunction.bUseSCKART==false)
            {
                if(Pos==1)
                {
                    if(MOT[iMMAuto_Car[Pos]].fHasTray==false)
                    {
                        fMain->edtAuto2->Text=iAutoTray[1]-1;
                        MOT[iMMAuto_Car[Pos]].SetTray(HAS_IC, __FUNC__);
                    }
                }
                else if(Pos==2)
                {
                    if(MOT[iMMAuto_Car[Pos]].fHasTray==false)
                    {
                        fMain->edtAuto3->Text=iAutoTray[2]-1;
                        MOT[iMMAuto_Car[Pos]].SetTray(HAS_IC, __FUNC__);
                    }
                }
                else if(Pos==3)
                {
                    if(MOT[iMMAuto_Car[Pos]].fHasTray==false)
                    {
                        fMain->edtAuto4->Text=iAutoTray[3]-1;
                        MOT[iMMAuto_Car[Pos]].SetTray(HAS_IC, __FUNC__);
                    }
                }
                else if(Pos==4)
                {
                    if(MOT[iMMAuto_Car[Pos]].fHasTray==false)
                    {
                        fMain->edtAuto5->Text=iAutoTray[4]-1;
                        MOT[iMMAuto_Car[Pos]].SetTray(HAS_IC, __FUNC__);
                    }
                }
                else if(Pos==5)
                {
                    if(MOT[iMMAuto_Car[Pos]].fHasTray==false)
                    {
                        fMain->edtAuto6->Text=iAutoTray[5]-1;
                        MOT[iMMAuto_Car[Pos]].SetTray(HAS_IC, __FUNC__);
                    }
                }
            }
            #endif

            if(LastSet.iRealDummy==DUMMY)
            {
                if(MOT[iMMAuto[Pos]].fHasTray==false && MOT[iMMAuto_Car[Pos]].fHasTray==true)
                {
                    InitAutoTrayToRearTask(Pos);
                    Task=200;
                }
                else if(MOT[iMMAuto_Car[Pos]].fHasTray==false && fMain->CheckBox1->Checked)
                {
                    InitLoadNewAutoTrayToCarTask(Pos);
                    Task=300;
                }
                else
                {
                    Task=400;
                }
            }
            else
            {
                if(MOT[iMMAuto[Pos]].fHasTray==false &&
                   Sen[SnAutoTrayDetect[Pos]].IsOn()==false &&
                   iARTCatchAutoTrayFinish!=1 &&                                //ChungHung 20141208 add fix ART catch Tray 與 Auto Tray Send to Rear 互搶
                   (MOT[iMMAuto_Car[Pos]].fHasTray==true ||
                    Sen[SnAutoTrayCar[Pos]].IsOn()))                            //用||是為了亂槍打鳥, 避免錯誤
                {
                    InitAutoTrayToRearTask(Pos);
                    Task=200;
                }
                else if(MOT[iMMAuto_Car[Pos]].fHasTray==false &&
                        Sen[SnAutoTrayCar[Pos]].IsOn()==false &&
                        Sen[SnAutoTrayHasTray[Pos]].IsOn())
                {
                    InitLoadNewAutoTrayToCarTask(Pos);
                    Task=300;
                }
                else
                {
                    #ifdef SOFT_SIMULTE
                        InitAutoTrayToRearTask(Pos);
                        Task=200;
                    #else
                        Task=400;
                    #endif
                }
            }
            break;
        case 200:                                                               //入Tray到機台裡面
            ret=DoAutoTrayToRear(Pos);
            if(ret)
            {
                iAutoTrayCount[Pos]--;
                if(IniConfig.bP37bAutoCylinderUP)                               //JerryYang 20220923 : add for P37
                {
                    Task=210;
                }
                else
                {
                    Task=400;
                }
            }
            break;
        case 210:
            if(IniConfig.bP37bAutoCylinderUP)
            {
                Task=211;
                bflag[Pos]=false;                                               //升到接盤位置
                AutoCylinderLower(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);   //Steven 20140409 : AutoCylinderXX Add Reset
                AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);  //升到接盤位置
                AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos], true);      //升到接盤位置
            }
            break;
        case 211:
            if(Sen[SnAutoTrackDetect[Pos]].IsOn() ||
               Sen[SnAutoTrayCar[Pos]].IsOn())                                  //軌道上有tray
            {
                bflag[Pos]=true;                                                //kevin 20180726 Auto 12 3 up
                bFlag=true;
            }

            if(bFlag)
                Task=212;
            else
                Task=213;
            break;
        case 212:
            if(bflag[Pos])                                                      //kevin 20180726 Auto 12 3 up
            {
                ibuffer++;
                if(AutoCylinderMiddle(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))  //升到接盤位置
                {
                    Cylinder[C_AutoZ_Select[Pos]].On();
                    ibuffer=0;
                    bflag[Pos]=false;
                }
            }

            if(ibuffer==0)
                Task=213;
            break;
        case 213:
            if(AutoCylinderUp(Pos, C_Auto_Up[Pos], C_Auto_Selector[Pos]))       //kevin 20190723 change  //升到接盤位置
                Task=400;
            break;
        case 300:                                                               //把Tray分離到下方
            bRet=DoLoadNewEmptyTrayToCar_RT(Pos);                               //kevin 20150613
            if(bRet==1)
            {
                Task=400;
            }
            else if(bRet==2)
            {
                Task=1;
                bResult=true;
            }
            break;
        case 400:                                                               //結束
            Task=1;
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool DoLoader_AutoRT(bool bReset)
{
    bool ret;
    bool bResult=false;
    int AlarmRet=0;
    int &Task=iLoader_AutoRT_Task;

    if(bReset==true)
    {
        Task=1;
        iCountReceiveTray_ART=0;                                                //kevin 20150613 計算收回tray
        return bResult;
    }

    switch(Task)
    {
        case 1:
            if(bTrayArmOK_AutoRT==true &&
               bAutoOK_AutoRT[eAuto1]==true &&
               bAutoOK_AutoRT[eAuto2]==true &&
               bAutoOK_AutoRT[eAuto3]==true &&
               bAutoOK_AutoRT[eAuto4]==true &&
               bAutoOK_AutoRT[eAuto5]==true &&
               bAutoOK_AutoRT[eAuto6]==true &&
               Sen[SnLoaderCarHasTray].IsOn()==false &&
               Sen[SnLoaderSureTray].IsOn()  ==false &&
               MOT[MMTrayY].fHasTray         ==false &&
               MOT[MMTrayY_Car].fHasTray     ==false)
            {
                bResult=true;
            }
            else
            {
                Task=100;
            }
            break;
        case 100:  //確認要做哪個動作
            if(LastSet.iRealDummy==DUMMY)
            {
                if(MOT[MMTrayY].fHasTray     ==false &&
                   MOT[MMTrayY_Car].fHasTray ==false)
                {
                    Task=500;
                }
                else if(MOT[MMTrayY_Car].fHasTray)
                {
                    InitUnLoadNewLoaderTrayTask();
                    Task=200;
                }
                else if(MOT[MMTrayY].fHasTray)
                {
                    InitLoaderTrayToFrontTask();
                    LastSet.iInputLoaderCount++;                                //ChungHung 20141002 add for KYEC AutoRetest
                    Task=400;
                }
                else
                {
                    Task=500;
                }
            }
            else
            {
                if(Sen[SnLoaderCarHasTray].IsOn()==false &&
                   Sen[SnLoaderSureTray].IsOn()  ==false &&
                   MOT[MMTrayY].fHasTray         ==false &&
                   MOT[MMTrayY_Car].fHasTray     ==false)
                {
                    Task=500;
                }
                else if(MOT[MMTrayY_Car].fHasTray ||
                        Sen[SnLoaderCarHasTray].IsOn())                         //ChungHung 20140815 modify
                {
                    if(MOT[MMTrayY_Car].fHasTray &&
                       Sen[SnLoaderCarHasTray].IsOff()==false)
                    {
                        InitUnLoadNewLoaderTrayTask();
                        Task=200;
                    }
                    else if(MOT[MMTrayY_Car].fHasTray &&
                            Sen[SnLoaderCarHasTray].IsOff())
                    {
                        AlarmRet=ShowErrorMessage("WAR0915", K_RETRY|K_SKIP, MMTrayY_Car, 0, __FUNC__);
                        if(AlarmRet==K_SKIP)
                        {
                            MOT[MMTrayY_Car].fHasTray=false;
                            Task=1;
                        }
                    }
                }
                else if(MOT[MMTrayY].fHasTray || Sen[SnLoaderSureTray].IsOn())  //ChungHung 20140815 modify
                {
                    if(MOT[MMTrayY].fHasTray && Sen[SnLoaderSureTray].IsOff()==false)
                    {
                        InitLoaderTrayToFrontTask();
                        LastSet.iInputLoaderCount++;                            //ChungHung 20141002 add for KYEC AutoRetest

                        if(CosFunction.bUseSCKART &&                            //Steven 20161214 (wei) : For SCK ART
                           bAutoOK_AutoRT[eAuto1]==true  &&
                           bAutoOK_AutoRT[eAuto2]==true  &&
                           bAutoOK_AutoRT[eAuto3]==true  &&
                           bAutoOK_AutoRT[eAuto4]==true  &&
                           bAutoOK_AutoRT[eAuto5]==true  &&
                           bAutoOK_AutoRT[eAuto6]==true  &&
                           bTrayArmPlaceToLoadFinish==true)                     //Steven 20170216 (wei) : 最後一盤不用退
                        {
                            bResult=true;
                            Task=1;
                        }
                        else
                        {
                            Task=400;
                        }
                    }
                    else if(MOT[MMTrayY].fHasTray && Sen[SnLoaderSureTray].IsOff())
                    {
                        AlarmRet=ShowErrorMessage("WAR0914", K_RETRY|K_SKIP, MMTrayY, 0, __FUNC__);
                        if(AlarmRet==K_SKIP)
                        {
                            MOT[MMTrayY].fHasTray=false;
                            Task=1;
                        }
                    }
                }
                else
                {
                    Task=500;
                }
            }
            break;
        case 200:                                                               //把Tray送到上方
            ret=DoUnLoadNewLoaderToStack(false);
            if(ret)
            {
                MOT[MMTrayY_Car].ClearTray(__FUNC__);
                Task=300;
            }
            break;
        case 300:                                                               //確認有沒有滿Tray
            ret=Sen[SnLoaderIsFull].IsOn();
            if(ret)
            {
                ShowErrorMessage("MES0921", 0, MMTrayZ);
            }
            else
            {
                Task=500;
            }
            break;
        case 400:                                                               //退Tray到機台外面
            ret=DoLoaderTrayToFront();
            if(ret)
            {
                Task=500;
            }
            break;
        case 500:                                                               //結束
            Task=1;
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");//Steven 20120615 : 改成可用序號(Mode)或名稱(ModeText)進行模式改變
//---------------------------------------------------------------------------
int GPIB_RemoteCommand(AnsiString asCommand)                                    //jou 2015-09-21 Auto Retest function
{
    unsigned char HCACK=1;
    AnsiString S=asCommand, S2, S3;
    int iPos1,iPos2;
    RecordProcess(S);

    if(S.Pos("RCMD:PAUSE")!=0)                                                  //Steven 20190116 : Add GPIB下暫停
    {
        fMain->Pause("RCMD:PAUSE");
        HCACK=0;
    }
    else if(S.Pos("RCMD:S10F3")!=0)                                             //Steven 20190116 : Add GPIB下顯示Message
    {
        S3=S.SubString(11, S.Length());
        fMain->tGPIBMsg->Add(S3);
        HCACK=0;
    }
    else if(S.Pos("RCMD:TESTER_ERROR")!=0)                                      //Frank 20170601 (Steven) add TESTER ERROR COMMAND
    {
        iPos1=S.Pos(":");
        iPos2=S.Pos(";");
        S2=S.SubString(iPos1+1, iPos2-iPos1-1);

        if(S2!="")
        {
            S3.sprintf("TESTER ERROR : %s", S2);
            fMain->tGPIBMsg->Add(S3);
            HCACK=0;
        }
        else
        {
            HCACK=1;
        }
    }
    //以下的在機台運行中不可以動作!!----------------------------------------
    else if(SystemStart==true)
    {
        HCACK=2;
    }
    else if(S.Pos("RCMD:INITIAL_START_ART;")!=0)
    {
        if(HasICUnderMachine()==false)
        {
            SetRunStartMode(rsmInitial_ART);
            HCACK=0;
        }
        else
        {
            HCACK=2;
        }
    }
    else if(S.Pos("RCMD:DOWNLOAD_RECIPE_BY_FTP")!=0)
    {
        if(HasICUnderMachine()==false && fFTPClient->bControlBySECSGEM==false)
        {
            iPos1=S.Pos(",");
            iPos2=S.Pos(";");
            S2=S.SubString(iPos1+1, iPos2-iPos1-1);

            fFTPClient->bControlBySECSGEM=true;
            fFTPClient->aSetUpNameBySECSGEM = S2;
            fFTPClient->ShowFTPModal(0);

            if(fMain->cbSetupFileName->Text==S2)
                HCACK=0;
            else
                HCACK=1;
        }
        else if(fFTPClient->bControlBySECSGEM==true)
        {
            HCACK=1;
        }
        else
        {
            HCACK=2;
        }
    }

    return HCACK;
}
//---------------------------------------------------------------------------
AnsiString GPIB_QueryData(AnsiString asCommand)                                 //jou 2015-09-21 Auto Retest function
{
    int i=0, iTotalPassCT=0;
    AnsiString S=asCommand, S2, S3;
    AnsiString asResult="SVID:";
    AnsiString asString;

    if(S.Pos("\r\n")!=0)
    {
        S=S.SubString(1, S.Pos("\r\n")-1);
    }

    if(S.Pos("SVID:")!=0)
    {
        S2=S.SubString(S.Pos(":")+1, S.Length());
        do
        {
            if(S2.Pos(";")==0)                                                  //Jimmychiu 20241219 : fixed for tester command send no ；
            {
                S3=S2;
            }
            else
            {
                S3=S2.SubString(1, S2.Pos(";")-1);
            }

            if(S3=="1160")
            {
                for(i=0; i<eTrayCount; i++)
                {
                    if(Prod.iTrayType[i]==tNotUse)
                        continue;
                    if(Prod.iIsPassT6[i]==1)                                    //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6 //Steven 20240701 : 0 --> 1
                        iTotalPassCT+=LastSet.BinCT[0][iTo3Unload[i]];
                }
                asString=AnsiString(iTotalPassCT);
            }
            else if(S3=="35915")                                                //wei 20160824 Barcode GPIB      //Alick 2017018(jou) Remark for Remote Wakeup Barcode
            {
                if(BAR_CODE_INSTALL==2 || BAR_CODE_INSTALL==3)                  //Alick 20170202(jou) add use CCD
                    asString="1";
                else
                    asString="0";
            }
            else if(S3=="1011")                                                 //Frank 20170505 (Steven) : Add handler status for Xilinx ART
            {
                asString=AnsiString(fMain->palMainStatus->Caption);
            }
            else
            {
                asString=HSys.MyGem->HGemPtr->GetECDataValue(S3);
            }

            asResult=asResult+asString+";";
            S2=S2.SubString(S2.Pos(";")+1, S2.Length());
            if(S2.Pos(";")==0)                                                  //Jimmychiu 20241219 : fixed for tester command send no ；
            {
                break;
            }
        }while(S2.Length()>0);
    }
    return asResult;
}
//---------------------------------------------------------------------------
int GPIB_SetData(AnsiString asCommand)                                          //jou 2015-09-21 Auto Retest function
{
    int i=0,j=0;
    int ret, TempMode;
    AnsiString S=asCommand, S2, S3, S4, S5;
    AnsiString asResult="ECID:";
    AnsiString asString;
    char cStr[256];
    bool bFlag;
    unsigned char HCACK=1;

    if(S.Pos("\r\n")!=0)
    {
        S=S.SubString(1,S.Pos("\r\n")-1);
    }

    if(S.Pos("ECID:")!=0)
    {
        S2=S.SubString(S.Pos(":")+1, S.Length());
        do
        {
            S3=S2.SubString(1, S2.Pos(",")-1);
            S4=S2.SubString(S2.Pos(",")+1, S2.Length());
            S5=S4.SubString(1, S4.Pos(";")-1);

            if(S3=="1530")
            {
                strcpy(cStr, S5.c_str());
                for(i=0; i<MAX_SOCKET_ROW; i++)                                 //kevin 20141015 fix
                {
                    for(j=0; j<MAX_SOCKET_COL; j++)
                    {
                        if(TestIF.iSiteMap[i][j]>0)
                        {
                            if(cStr[TestIF.iSiteMap[i][j]-1]=='0')
                                bFlag=false;
                            else
                                bFlag=true;
                        }
                        else
                        {
                            bFlag=false;
                        }

                        if(bFlag==false)
                        {
                            bTestSiteUse[0][i][j]=bFlag;
                            bTestSiteUse[1][i][j]=bFlag;
                            LastSet.bUseTestSocket[0][i][j]=bFlag;
                            LastSet.bUseTestSocket[1][i][j]=bFlag;
                        }
                    }
                }

                fMain->ShowTestHeadComp(false);
                HCACK=0;
            }
            else if(S3=="1515" || S3=="1519")
            {
                asString=S5;
                i=atoi(S5.c_str());

                if(i>=20 && i<=150)
                {
                    if(i>25)
                        TempMode=Tempture_Hot;
                    else
                        TempMode=Tempture_Ambient;

                    fMain->edWorkTemperBase->Text=i;
                    ret=fMain->SetTemp(false, atof(fMain->edWorkTemperBase->Text.c_str()), atof(fMain->edSoakTime->Text.c_str()));

                    if(ret==0)
                    {
                        ret=fMain->ChangeTempMode(TempMode, false, bRefreshFunction);
                        if(ret==0)
                        {
                            if(i>25)
                                LastSetTemperature_NET=Tempture_Hot;
                            else
                                LastSetTemperature_NET=Tempture_Ambient;
                            Temperature_NET.fWorkTemperBase=i;
                            HCACK=0;
                        }
                    }
                }
            }
            else if(S3=="1516" || S3=="1520")
            {
                asString=S5;
                i=atoi(S5.c_str());
                if(i>=0 && i<=999)
                {
                    fMain->edSoakTime->Text=i;
                    ret=fMain->SetTemp(false, atof(fMain->edWorkTemperBase->Text.c_str()), atof(fMain->edSoakTime->Text.c_str()));     //Steven 20120730
                    if(ret==0)
                    {
                        Temperature_NET.fSoakTime=i;
                        HCACK=0;
                    }
                }
            }
            else if(S3=="35916")                                                //wei 20160824 Barcode GPIB
            {
                i=atoi(S5.c_str());
                if(BAR_CODE_INSTALL==2 || BAR_CODE_INSTALL==3)
                {
                    if(i==0)
                    {
                        TestIF_File.bEnableBarCode=false;
                    }
                    else
                    {
                        TestIF_File.bEnableBarCode=true;
                    }
                    AnsiString S="";
                    S=GetLastOpenFN();
                    AnsiString szDir="";
                    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
                    WriteIniData(szDir, "Configuration", "Bar Code", TestIF_File.bEnableBarCode);
                    HCACK=0;
                }
            }
            else if(S3=="37228")                                                //wei 20160824 Barcode GPIB
            {
                asString=S5;
                if(CUSTOMER_CODE==CC_KYEC_XILINX)
                {
                    if(asString.Length()==9)                                    //Alick 20170124(jou) add for "37228"小於九碼
                    {
                        DeleteFile(asBarCodeDownLot);
                        WriteDataToFile(asBarCodeDownLot, asString.c_str());
                        HCACK=0;
                    }
                    else
                    {
                        DeleteFile(asBarCodeDownLot);
                    }
                }
                else
                {
                    DeleteFile(asBarCodeDownLot);
                    WriteDataToFile(asBarCodeDownLot, asString.c_str());
                    HCACK=0;
                }
            }
            S2=S4.SubString(S4.Pos(";")+1, S4.Length());
        }while(S2.Length()>0);
    }
    return HCACK;
}
//---------------------------------------------------------------------------
