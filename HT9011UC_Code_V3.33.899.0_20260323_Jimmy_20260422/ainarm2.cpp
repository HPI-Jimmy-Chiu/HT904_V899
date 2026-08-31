#include "MachineDefine.h"
#pragma hdrstop

#include "ainarm2.h"

#include "aArmHeader.h"
#include "note.h"
#include "csystem.h"
#include "cinitial.h"
#include "mymessbox.h"
#include "mymotor.h"
#include "cmydef.h"
#include "atester.h"
#include "common.h"
#include "mycylin.h"
#include "cprod.h"
#include "MyKitSuck.h"

//#include "OCR.h"
//#include "cMyDB.h"
#include "aRotateKIT.h"
//#include "fRotate.h"
#include "main.h"
//#include "uLotInfo.h"
#include "mysensor.h"
#include "Note.h"
//#include "cSortCT.h"
//#include "SCK_ART.h"
#include "cTrayMapping.h"                                                       //wei 20161219 (Steven) Tray Mapping
//#include "InOutArmZteach.h"
#include "uhome.h"
#include "cOffSet.h"
//#include "cSocket.h"
//#include "acarry.h"
#include "cObserver.h"
//#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
//#include "SmartSetup.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//==============================================================================
bool flag1_1x2_2[2];
strAUTOSITEMAP InArmSiteMapData;                                                //Steven 20211209 : 紀錄Site map資料
bool bPlaceToHotplate=false;
bool bPickFromHotplate=false;
bool bPlaceShuttle=false;
bool bPickFromLoader=false;                                                     //Steven 20171226 (Wei) : 確認Loader吸取完成
//bool bDestoryOnSht=false;                                                     //RogerYang 20251021 : 搬去cmydef  //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
bool bPlaceToShuttle2Step=false;                                                //Steven 20160721 (Wei): 避免放一半歸零,會Hangup
bool bPlaceToCleanKit=false;                                                    //Steven 20171204 (Wei) : 避免放到一半要讓位不回來原本位置放
bool bDoPreciser=false;

bool bInArmHasHotIC=false;                                                      //Sam 20211012 : Debug 用

int iPickPlate[2] ={0, 0}, iPickPlateX[2] ={0, 0}, iPickPlateY[2] ={0, 0};
int iPlacePlate[2]={0, 0}, iPlacePlateX[2]={0, 0}, iPlacePlateY[2]={0, 0};

int iBackupPlate=0, iBackupPlateR=0, iBackupPlateC=0;                           //ChungHung 20120206 Hotplate check

int iInArmPickFromHotPlateTask=1;
int iInArmPlaceToShuttleTask=1;
int iInArmTryPickFromHotPlateTask=1;                                            //ChungHung 20120206 Hotplate check
bool bBackMMTrayY_Car=false;
bool bBackMMTrayY=false;

const int cMaxInArmOrder=2;
int cMaxKit=1;
int iBackInArmHotCount,iArmTask=1;
int iHotCount=0;                                                                //   用以記錄同一次 Shuttle 的IC
int iHotPlateCount[2][50][50];                                                  //   plate 上的 iHotCount 值
int iHotInArmOrder[2][50][50];                                                  //   用以記錄 InArm 的前後組吸嘴
int iHotWhichKit[2][50][50];                                                    //   plate 上的 IC 預備要放到那一個 shuttle 左,或右組 Kit
int iHotWhichShuttle[2][50][50];                                                //   plate 上的 IC 預備要放到那一個 shuttle
bool InArmSuckUse[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
int  iXPosition[8]={0, 0, 0, 0, 0, 0, 0, 0}, iYPosition=0;

bool InArmXMoveSafe=false;
bool bShuttleShake=false;
bool bShuttleKnock=false;                                                       //Jou 2013-03-08 修改敲敲功能

int iNextPlateRow;
int iRecordTrayPickPosX[16][16];
int iRecordTrayPickPosY[16][16];
int iPickFromLoadStageTask=1;
TQPF_Timer InArmReleaseDelayToHot;                                              //JerryYang 20160127 for TSMC inarm release device前delay
TQPF_Timer InArmReleaseDelay;                                                   //JerryYang 20160127 for TSMC inarm release device前delay
TQPF_Timer VibrationShuttleDelay;                                               //JerryYang 20171205 (Steven) Shuttle 震動馬達 delay
TQPF_Timer VibrationOutShuttleDelay[2];                                         //JerryYang 20171205 (Steven) Shuttle 震動馬達 delay
TQPF_Timer MyInArmAtShuttleTimer;                                               //Steven 20151201
//ChungHung 20130402 alter
int iAutoCleanStart=0;
int iAutoCleanNum=0;
int iAutoCleanCnt=0;                                                            //JerryYang 20171127 (Steven) add auto clean清潔次數

bool bPitchOver12000=false;                                                     //jou 2010-01-20 Hotplate 8x12 Pitch=26.67 (Quad Site 2x2)，吸嘴over 12000

int iInArmWaitPosition=0;                                                       //Ifor 20191105 : add InArm 安全位置判斷 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
//----------------------------------------------------------------------------------------
// AUTOCLean kevin 20120217
bool bInArmToLoaderUsage[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
bool bInArmSuckActive[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
bool bInArmZNeedCheck[4]={false, false, false, false};
bool bRecordVacuumTim = false;                                                  //kevin 20210224 記錄計算真空產生器壽命

int iAutoCleanPickPlateX;
int iAutoCleanPickPlateY;
int iAutoCleanUseXPitch=0;                                                      //HOTPLATE使用幾個PITCH

bool bAutoCleanUse_13_24=false;                                                 //ChungHung 20131118 add for SCK AutoClean use HotPlate
int iInArmZCheckPosTask=1;                                                      //Richard 2017.11.1 After pause check arm Z in position
int iInArmDevicePosPrecise=1;                                                   //Frank 20180410 (Steven) : InArm Preciser Station

HTimer hInArmYpitchHomeTimer;                                                   //kevin 20180822  Ypitch home
HTimer hOutArmYpitchHomeTimer;                                                  //kevin 20180822  Ypitch home
const bool ZAxisDown=true;
const bool ZAxisNotDown=false;
//==============================================================================
int  iInXPToSht[X_PITCH_COUNT]={0};
int  iZPosToSht[MAX_ARM_Row][MAX_ARM_Col]={0};
bool bZFlgToSht[MAX_ARM_Row][MAX_ARM_Col]={false};
int  iZPosToCleanKit[MAX_ARM_Row][MAX_ARM_Col]={0};
bool bZFlgToCleanKit[MAX_ARM_Row][MAX_ARM_Col]={false};
//==============================================================================
void ResetInToShtFlag()
{
    ZeroMemory(iInXPToSht, sizeof(iInXPToSht));
    ZeroMemory(iZPosToSht, sizeof(iZPosToSht));
    ZeroMemory(bZFlgToSht, sizeof(bZFlgToSht));
}
//==============================================================================
// In Arm  main process for normal
//==============================================================================
bool CheckInArmFinishAllPickerAction()                                          //Steven 20171226 (Wei) : 修改in arm讓開的flag
{
    if(MOT[MInArmX].fCanMove==false &&                                          //jou 20240910 : 修正tray arm 讓位 hang up
       MOT[MInArmY].fCanMove==false)
    {
        if(IsMoveInArm2XYToWait())
            return true;
    }

    if(bPickFromLoader || bPlaceToHotplate ||                                   //JerryYang 20241118 : fix auto clean hang up
        bPickFromHotplate || bDestoryOnSht ||                                   //JerryYang 20210429 : 發生auto clean執行中bPlaceToHotplate為true造成hang up, 暫時先不卡auto clean
        bPlaceToShuttle2Step ||
        bWaitPreciserFinish ||
        bWaitRotateFinish)                                                     //Ifor 20180704 新增 Preciser
    {
        return false;
    }
    return true;
}
//==============================================================================
//caution:
//當InArm丟完hotplate時,才可把相關位置的IC設成HAS_NULL_IC
//==============================================================================
bool Check_QA_ModeCount()                                                       //Steven 20111019 : QA Mode //Dell 20111024
{
    if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)             //Steven 20111005 : QA Mode
    {
        if(iQAModeLoaderCT==Prod.iQAModeCount)                                  //數量到的話要先快速CleanOut
        {                                                                       //jou 2012-05-03 增加QA mode使用獨立的Loader Count
            if(bQAModeQuickCleanOut==false && bQAModeFinishCleanOut==false)
            {
                TrayForm.bAutoFeed=false;
                bQAModeQuickCleanOut=true;
                InitOneCycle("QA Mode");
                iOneCycle=1;
                return true;
            }
        }
        else if(iQAModeLoaderCT>Prod.iQAModeCount)                              //數量比設定值多，表示已經做完，所以就直接送Bin 1
        {
            if(bQAModeQuickCleanOut==true && bQAModeFinishCleanOut==false)
            {
                fMain->ModifyTester(OFF_LINE);                                  //Steven 20191218 : 整合修改LastSet.iTester
                NewRecordProcess("MES2155", "Change to Off_Line", "by Check_QA_ModeCount QAMode1"); //ChungHung 20140722 add add record

                ArmSpeed[InArm].bVariModeFIX=IniConfig.bBackUpInArmMode;
                bQAModeFinishCleanOut=true;
                bQAModeQuickCleanOut=false;

                if(iCleanOut==0)
                    InitCleanOutFunction();
            }
        }
        else if(iQAModeLoaderCT>Prod.iQAModeCount-20 &&
                iQAModeLoaderCT<Prod.iQAModeCount)                              //數量接近時，要改用Fix方式生產
        {
            ArmSpeed[InArm].bVariModeFIX=true;
        }
    }
    return false;
}
// =============================================================================
void SetInArmUseSuckToHasNullIC(int iSht, int iKit)
{
//    int iKit32=(iKit==1)?4:0;

    if(iInArmType==e9045_1x4_8_Hot ||                                           //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
//        iSht=0;
//        iKit32=0;
    }

    if(TestIF.iTestMode==SingleSite ||                                          //Steven 20140614 : for Auto Clean Single Site
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {

    }
    else if(bUseAxExPicker() ||                                                 //Steven 20241102 : Fixed for HP
            bUseAxxGPicker())
    {
        for(int i=0; i<InArmSuck.iPickRow; i++)
        {
            for(int j=0; j<InArmSuck.iPickCol; j++)
            {
                int j2=j*GetJStep();
                if(j2<InArmSuck.iMaxCol &&
                   InArmSuck.Item[i][j2]==NULL_IC)
                {
                    InArmSuck.SetItemData(i, j2, HAS_NULL_IC);
                }
            }
        }
    }
    else
    {
        for(int i=0; i<InArmSuck.iPickRow; i++)
        {
            for(int j=0; j<InArmSuck.iPickCol; j++)
            {
                if(InArmSuck.Item[i][j]==NULL_IC)
                    InArmSuck.SetItemData(i, j, HAS_NULL_IC);
            }
        }
    }

    if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 3, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 3, NULL_IC);
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 2, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 2, NULL_IC);
    }
}
// =============================================================================
void SetInArmUseSuckToHasTrySuckIC(int iSht, int iKit)
{
    int iKit32=(iKit==1)?4:0;

    if(iInArmType==e9045_1x4_8_Hot ||                                           //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
//        iSht=0;
        iKit32=0;
    }

    if(USE_PICKER_COUNT==ep1Picker)
    {
        InArmSuck.SetItemData(0, 0, HAS_TRY_SUCK_IC);
    }
    else
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                int j2=j+iKit32;
    //            if(Prod.fInArmSuck4x8[iSht][i][j2]==true &&
    //               InArmSuck.Item[i][j]==NULL_IC)
                if(InArmSuck.Item[i][j]==NULL_IC)                                   //JerryYang 20251013 : fix hot plate check 關Site位置錯誤
                {
                    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                               //wei 20160624 Hotplate clean out
                    {
                        if(Prod.iSiteMap[iSht][i][j2]>0)
                            InArmSuck.SetItemData(i, j, HAS_TRY_SUCK_IC);
                        else
                            InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                    }
                    else
                    {
                        InArmSuck.SetItemData(i, j, HAS_TRY_SUCK_IC);
                    }
                }
            }
        }
    }

    if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 3, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 3, NULL_IC);
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 2, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 2, NULL_IC);
    }
}
// =============================================================================
void SetInArm_Unuse_SuckToNullICForHP()
{
    int iShtHP=InArmSuck.iWhichShtPickFor32;
    int iKit32=(InArmSuck.iWhichKitPickFor32==1)?4:0;

    if(iInArmType==e9045_1x4_8_Hot ||                                           //Steven 20240119 : fixed for 2x4_16
       iInArmType==e9045_1x2_4_Hot ||
       iInArmType==e9045_2x2_8_Hot)
    {
//        iSht=0;
        iKit32=0;
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            int j2=j+iKit32;
            if(Prod.fInArmSuck4x8[iShtHP][i][j2]==false)
                InArmSuck.SetItemData(i, j, NULL_IC);
        }
    }

    if(iCloseSiteModeFor2x8==e2x8Run2x2_13)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 3, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 3, NULL_IC);
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        InArmSuck.SetItemData(0, 1, NULL_IC);
        InArmSuck.SetItemData(0, 2, NULL_IC);
        InArmSuck.SetItemData(1, 1, NULL_IC);
        InArmSuck.SetItemData(1, 2, NULL_IC);
    }
}
//==============================================================================
//bool AllRow2NotUse(bool bPlace)
//{
//    int iSht=InArmSuck.iWhichSht,
//    int iKit=(InArmSuck.iWhichKit==0)?0:4;
//
//    if(bCanAutoCloseSite &&                                                     //Steven 20200420 : 整合Auto Site Off
//       bPlace==false)                                                           //Steven 20150924 : 隨時開關Site功能
//    {
//        return false;
//    }
//
//    if(HotPlateForm.XDivision==6)
//    {
//        return false;
//    }
//    else
//    {
//        for(int j=0; j<4; j++)
//        {
//            if(Prod.fInArmSuck4x8[iSht][1][j+iKit])
//                return false;
//        }
//    }
//    return true;
//}
//==============================================================================
//Eliot 2009_03_06
//==============================================================================
int CloseSiteState(bool bPlace)
{
    int iState=0;
    bool bRow1HasUse=false, bRow2HasUse=false;

    if(bCanAutoCloseSite &&                                                     //Steven 20200420 : 整合Auto Site Off
       bPlace==false)                                                           //Steven 20150924 : 隨時開關Site功能
    {
        bRow1HasUse=true;
        bRow2HasUse=true;
    }
    else
    {
        if(bRunAutoClean && TestIF_File.iAutoClean_SelectArm<2)                 //JerryYang 20160219 Fix 8site 整排關site時hang up
        {
            if(CosFunction.bUseAutoCleanCloseSiteAlsoDo==true)                  //Ifor 20181222 add 新增Auto Clean Close Site 一樣執行
            {
                if(IsNNMode()==NN_2Row)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        if(TestIF.iSiteMap[0][j] || TestIF.iSiteMap[2][j])
                        {
                            bRow1HasUse=true;
                        }
                        if(TestIF.iSiteMap[1][j] || TestIF.iSiteMap[3][j])
                        {
                            bRow2HasUse=true;
                        }
                    }
                }
                else if(IsNNMode()==NN_1Row)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        if(TestIF.iSiteMap[0][j] || TestIF.iSiteMap[1][j])
                        {
                            bRow1HasUse=true;
                        }
                    }
                }
                else
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        if(TestIF.iSiteMap[0][j])
                        {
                            bRow1HasUse=true;
                        }
                        if(TestIF.iSiteMap[1][j])
                        {
                            bRow2HasUse=true;
                        }
                    }
                }
            }
            else
            {
                if(IsNNMode()==NN_2Row)
                {
                    for(int j=0; j<InArmSuck.iShtCol; j++)
                    {
                        if(Prod.fInArmSuck4x8[0][0][j] || Prod.fInArmSuck4x8[1][0][j])
                        {
                            bRow1HasUse=true;
                        }
                        if(Prod.fInArmSuck4x8[0][1][j] || Prod.fInArmSuck4x8[1][1][j])
                        {
                            bRow2HasUse=true;
                        }
                    }
                }
            }
        }
        else
        {
            if(IsNNMode()==NN_2Row)
            {
                for(int j=0; j<InArmSuck.iShtCol; j++)
                {
                    if(Prod.fInArmSuck4x8[0][0][j] || Prod.fInArmSuck4x8[1][0][j])
                    {
                        bRow1HasUse=true;
                    }
                    if(Prod.fInArmSuck4x8[0][1][j] || Prod.fInArmSuck4x8[1][1][j])
                    {
                        bRow2HasUse=true;
                    }
                }
            }
        }
    }

    if(bRunAutoSiteMapping==true ||                                             //Ifor 20180518 : add 簡化Site Mapping 旗標
       fContact->IsRun2DCheck()==true)                                          //JerryYang 20250220 : 2DID硬體順序檢查功能
    {                                                                           //Ifor 20180515 : add 避免Auto Site Mapping 模式錯誤導致Hang up
        for(int i=0; i<InArmSuck.iShtCol; i++)
        {
            if(LastSet.bUseTestSocket[0][0][i] || LastSet.bUseTestSocket[1][0][i])  //ChungHung 20130910 alter for SCK can close site by Index
            {
                bRow1HasUse=true;
            }

            if(LastSet.bUseTestSocket[0][1][i] || LastSet.bUseTestSocket[1][1][i])   //ChungHung 20130910 alter for SCK can close site by Index
            {
                bRow2HasUse=true;
            }
        }
    }

    if(i8PickerHPMode==iHPWideHP &&                                             //JerryYang 20161007 簡化Hotplate判斷式
       bRunAutoClean==false)                                                    //jou 2012-08-10 會造成Auto clean錯亂  //Steven 20151117 : 2x2 8Picker at Hot mode
    {
        if(HotPlateForm.XDivision==6 &&
           (iInArmType==e9045_2x4_8  ||
            iCloseSiteModeFor2x8>e2x8OneByOne  ||
            iCloseSiteModeFor2x6>e2x6OneByOne))
        {
            iState=0;
            return iState;
        }
    }

    if(bRunAutoClean==false &&                                                  //jou 2013-01-22 Auto Clean 16 site 不能直接return 0 , Hang up
       fContact->IsRun2DCheck()==false &&                                       //JerryYang 20250220 : 2DID硬體順序檢查功能                                                 //jou 2013-01-22 Auto Clean 16 site 不能直接return 0 , Hang up
       ((TestIF.iTestMode==_12Site2X6 && iCloseSiteModeFor2x6<=e2x6OneByOne) || // Lee 20100823 : 2x8關Site會Hang UP
       (TestIF.iTestMode==_16Site2X8 && iCloseSiteModeFor2x8<=e2x8OneByOne) ||
       TestIF.iTestMode==_32Site4X8N  ||                                        //Steven 20140512: For HT-9047
       iInArmType==e9045_2x4_8 ||                                               //Steven 20210420 : 修正12site關site跑2x4 mode, HP=6x11 hang up
       ((TestIF.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site4X4) &&        //Sam 20190226 : 16Site4X4                                    //Steven 20170321 (wei) : fixed for close row2 hang up
         ArmCanSuck4IC(0)==true)))                                               //DeviceForm_File.XDimension<20 && DeviceForm_File.YDimension<20)) //Sam 20190701 : 修正 Xpitch 60mm, ic 21.5X21.5mm 關 Site hang up  //kevin 20170626 (Steven) shuttle X Pitch 60 IC 28X28 關SITE 上排
        iState=0;
    else if(bRow1HasUse && bRow2HasUse)
        iState=0;                                                               // full site
    else if(bRow1HasUse && bRow2HasUse==false)
        iState=1;                                                               // only row1
    else if(bRow1HasUse==false && bRow2HasUse)
        iState=2;                                                               // only row2

    return iState;
}
//==============================================================================
bool CheckClearAllHotICThenPickLoadIC()                                         //ChungHung 201205104 Clear HotIC then Pick Load IC
{
    int iCT=0;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<HotPlateForm.XDivision; j++)
        {
            for(int k=0; k<HotPlateForm.YDivision; k++)
            {
                if(HotPlateForm.iPlateSelect & 2-i)                             //MMPlate1+0=NO 2 HP
                {
                    if(MOT[MMPlate1+i].Tray.Data[j][k]!=NULL_IC)
                        iCT++;
                }
            }
        }
    }

    int iLimit=0;
    if(InArmSuck.iPickRow==2)
    {
        if(HotPlateYPitchCanPutAll())
        {
            GetHotPlateYHalfPos();
            iLimit=iYHalf*2*HotPlateForm.XDivision;
        }
    }

    if(iLimit<=iCT)
        return true;

    return false;
}
//==============================================================================
// InArm pick IC from plate process
//==============================================================================
void InArmAddSpeed()
{
    if(AutoArmSpeed[InArm].iBodySP     <= (ArmSpeed[InArm].iBodySP-5) ||
       AutoArmSpeed[InArm].iACDCBodySP <= (ArmSpeed[InArm].iACDCBodySP-5))
    {
        if(AutoArmSpeed[InArm].iBodySP<=(ArmSpeed[InArm].iBodySP-5))
            AutoArmSpeed[InArm].iBodySP+=5;

        if(AutoArmSpeed[InArm].iACDCBodySP<=(ArmSpeed[InArm].iACDCBodySP-5))
            AutoArmSpeed[InArm].iACDCBodySP+=5;

        SetInArmSpeed(true);
    }
    else
    {
        if(AutoArmSpeed[InArm].iBodySP     < ArmSpeed[InArm].iBodySP ||
           AutoArmSpeed[InArm].iACDCBodySP < ArmSpeed[InArm].iACDCBodySP)
        {
            if(AutoArmSpeed[InArm].iBodySP<ArmSpeed[InArm].iBodySP )
                AutoArmSpeed[InArm].iBodySP=ArmSpeed[InArm].iBodySP;

            if(AutoArmSpeed[InArm].iACDCBodySP<ArmSpeed[InArm].iACDCBodySP)
                AutoArmSpeed[InArm].iACDCBodySP=ArmSpeed[InArm].iACDCBodySP;

            SetInArmSpeed(true);
        }
    }
}
//==============================================================================
void InArmSubSpeed()
{
    if(AutoArmSpeed[InArm].iBodySP>25 ||
       AutoArmSpeed[InArm].iACDCBodySP>25)
    {
        if(AutoArmSpeed[InArm].iBodySP>25)
            AutoArmSpeed[InArm].iBodySP-=5;

        if(AutoArmSpeed[InArm].iACDCBodySP>25)
            AutoArmSpeed[InArm].iACDCBodySP-=5;

        SetInArmSpeed(true);
    }
}
//==============================================================================
void InitInArmTryPickFromHotPlateTask()                                         //ChungHung 20120206 Hotplate check
{
    iInArmTryPickFromHotPlateTask=1;
}
//==============================================================================
void InitInArmTryPickFromHotPlateTask100()                                      //ChungHung 20120206 Hotplate check
{
    iInArmTryPickFromHotPlateTask=100;
}
//==============================================================================
void InitInArmPickFromHotPlateTask()
{
    iInArmPickFromHotPlateTask=1;
}
//==============================================================================
void InitInArmPickFromHotPlateTask340()                                         //ChungHung 20120502 HangUp 解除
{
    iInArmPickFromHotPlateTask=340;
}
//==============================================================================
void InitInArmPickFromHotPlateTask50()                                          //ChungHung 20120112 Hang Up 解除
{
    if(bPickFormHotplateRetry==true)                                            //Ifor 20160616 add Hot plate Pickup error Retry
    {
        iInArmPickFromHotPlateTask=190;
    }
    else
    {
        iInArmPickFromHotPlateTask=50;
    }
}
//==============================================================================
// InArm Place IC to shuttle process
//==============================================================================
void InitInArmPlaceToShuttleTask()
{
    iInArmPlaceToShuttleTask=1;
    if(TestIF_File.UseRotateForHT7000HPKit==false)                              //Sam 20250428 : 修正 UseRotateForHT7000HPKit 誤報警
        iInRotateFinish=2;                                                      //Ifor 20211220 add:避免Hotplate取料後丟至Roteta
}
//==============================================================================
void ResetShuttleWhichKit()                                                     //Steven 20140710
{
    if(LastSet.iTemperature!=Tempture_Hot)
    {
        AdjustShuttlePlaceOrder();                                              //Steven 20160213 : 解決Input擺放順序問題
    }
    else
    {
        if(MOT[MMPlate1].HasIC()==false &&
           MOT[MMPlate2].HasIC()==false)
        {
            AdjustShuttlePlaceOrder();                                          //Steven 20160213 : 解決Input擺放順序問題
        }
        else
        {
            ChangeHotPlateData();
        }
    }
}
//==============================================================================
void AdjustShuttlePlaceOrder_AutoSiteMapping()                                  //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
{
    if(CosFunction.bUSEJCETSiteMapMode)                                         //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
    {
        if(IniConfig.bUseAutoSiteMapping &&
           LastSet.iRunStartMode==rsmAutoSiteMap)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20220531 : for NN mode Auto Site Map
                return;

            if(TestIF_File.iShuttleMode==1)
            {
                if(TestIF_File.iShuttle_Sel==0)
                    InArmSuck.iWhichShtPickFor32=0;
                else if(TestIF_File.iShuttle_Sel==1)
                    InArmSuck.iWhichShtPickFor32=1;
            }
            else if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
                    TestIF_File.bArm1PickPlaceArm2Test==true &&
                    TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)
            {
                InArmSuck.iWhichShtPickFor32=0;
            }
            else
            {
                InArmSuck.iWhichShtPickFor32++;
                if(InArmSuck.iWhichShtPickFor32>=2)
                    InArmSuck.iWhichShtPickFor32=0;
            }
        }
    }
}
//==============================================================================
void AdjustShuttlePlaceOrder(int iReset)
{
    if(iReset==-1)
    {
        InArmSuck.iWhichKit=0;
    }

    if(TestIF_File.iShuttleMode==1)                                             //Steven 20160223 : TestIF --> TestIF_File
    {
        if(TestIF_File.iShuttle_Sel==0)
            InArmSuck.iWhichSht=0;
        else if(TestIF_File.iShuttle_Sel==1)
            InArmSuck.iWhichSht=1;
    }
    else if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                     //Jimmychiu 20250722 : Arm1 Only Place Arm2 Test And Suck
            TestIF_File.bArm1PickPlaceArm2Test==true &&
            TestIF_File.bArm1OnlyPlaceArm2TestAndSuck==true)
    {
        InArmSuck.iWhichSht=0;
    }
    else
    {
        if(iReset==-1)
        {
            InArmSuck.iWhichSht=0;
        }
        else if(iReset==0)
        {
            InArmSuck.iWhichSht=0;
        }
        else if(iReset==1)
        {
            InArmSuck.iWhichSht=1;
        }
        else
        {
            InArmSuck.iWhichSht=0;
        }
    }
}
//==============================================================================
void AdjustShuttlePlaceOrderForASM(bool bResetSht)                              //Steven 20250206 : fixed ASM之中作one cycle會造成關arm 1時異常
{
    if(TestIF_File.iShuttleMode==1)
    {
        if(TestIF_File.iShuttle_Sel==0)
            iAutoSiteMapHPToSht=0;
        else if(TestIF_File.iShuttle_Sel==1)
            iAutoSiteMapHPToSht=1;
    }
    else
    {
        if(bResetSht)
        {
            iAutoSiteMapHPToSht=0;
        }
        else
        {
            iAutoSiteMapHPToSht++;
            if(iAutoSiteMapHPToSht>=2)
                iAutoSiteMapHPToSht=0;
        }
    }
}
//==============================================================================
void AdjustShtOrderWhenPlaceToSht(int iMode)                                    //Steven 20180601 : 整合放完蝦頭後的調整
{
    if(CosFunction.bUseInitialDelayAsSoakTime &&                                //Steven 20170511 (wei) : 使用initial delay當 Soak time
       Temperature.bUseInitialDelayAsSoakTime &&
       LastSet.iTemperature==Tempture_Hot &&
       bFirstInput==true)
    {
        if(iMode==4)                                                            //16Site, Sht2, Kit1
        {
            if(InArmSuck.iWhichKit==1)
            {
                bFirstInput=false;
                bOneTimeWait=false;
            }
        }
        else if(iMode==3)                                                       //16Site, Sht1, Kit1
        {
            if(TestIF_File.iShuttleMode==1)
            {
                if(TestIF_File.iShuttle_Sel==0 && InArmSuck.iWhichKit==1)
                {
                    bFirstInput=false;
                    bOneTimeWait=false;
                }
            }
        }
        else if(iMode==1)                                                       //Sht1
        {
            if(TestIF_File.iShuttleMode==1)
            {
                if(TestIF_File.iShuttle_Sel==0)
                {
                    bFirstInput=false;
                    bOneTimeWait=false;
                }
            }
        }
        else if(iMode==2)                                                       //Sht2
        {
            bFirstInput=false;
            bOneTimeWait=false;
        }
        AdjustShuttleWhichKitOrder();
    }
    else if(LastSet.iTemperature==Tempture_Hot &&
            LastSet.iRunStartMode==rsmQAMode &&
            bQAModeFinishCleanOut==true)                                        //Steven 20180601 : 修正高溫QA mode放料
    {
        AdjustShuttleWhichKitOrder();
    }
    else if(LastSet.iTemperature==Tempture_Ambient ||
            LastSet.iTemperature==Tempture_AmbientHot ||
            (LastSet.iTemperature==Tempture_Hot &&
             LastSet.iRunStartMode==rsmAutoSiteMap &&
             CosFunction.bUSEJCETSiteMapMode==false))                           //kevin 20161124 add hot AutoSitmap hang up
    {
        AdjustShuttleWhichKitOrder();                                           //Steven 20160201 : 修正Shuttle放料順序
    }
    else if(USE_PICKER_COUNT==ep1Picker && LastSet.iTemperature==Tempture_Hot)
    {
        AdjustShuttleWhichKitOrder();
    }

    if((CosFunction.bUSEJCETSiteMapMode ||
        CUSTOMER_CODE==CC_SCS) &&                                               //jou 20230626 : 修正JSCS auto site mapping 2x8 hangup
       LastSet.iRunStartMode==rsmAutoSiteMap)                                   //Steven 20220913 : Fixed for ASM function
    {
        if(bUseTwoArm32Site==true)
        {

        }
        else
        {
            AdjustShuttlePlaceOrderForASM();                                    //Steven 20250206 : fixed ASM之中作one cycle會造成關arm 1時異常
        }
    }
}
//==============================================================================
void AdjustShuttleWhichKitOrder()                                               //Steven 20140626 : Modify to Function
{
    iInArmPutIcToSH[InArmSuck.iWhichSht][InArmSuck.iWhichKit]=0;                //Ifor 20171121 : Test 查看異常資料

    if(iInArmType==e9045_1x3_2_14 ||                                            //Steven 20220425 : 1x3_14
       iInArmType==e9045_1x4_2_14 ||
       iInArmType==e9045_2x3_6_14 ||                                            //Steven 20200219 : 2x3mode, Layout: 120x70mm
       iInArmType==e9045_2x4_4_13 ||
       iInArmType==e9045_2x4_4_14 ||                                            //Steven 20240221 : fixed for 1x4_2_14
       iInArmType==e9045_2x5_8    ||
       (iInArmType==e9045_2x6_8 && iCloseSiteModeFor2x6<=e2x6OneByOne) ||       //Steven 20240417 : 12site關中間4site
       (iInArmType==e9045_2x8_8 && iCloseSiteModeFor2x8<=e2x8OneByOne) ||
       iInArmType==e9045_2x8_32)
    {
        InArmSuck.iWhichKit++;
        if(InArmSuck.iWhichKit>=2)
        {
            //InArmSuck.iWhichKit=0;
            AdjustShuttlePlaceOrder(InArmSuck.iWhichSht+1);

            if(iInArmType==e9045_2x8_32)                                        //Sam 20250417 : 修正 CloseSite 異常，左邊半邊全關 右半邊沒有全關 Kit 要從1開始。
            {
                if(InArmSideAllCloseWithKit(InArmSuck.iWhichSht,0)==true &&
                   InArmSideAllCloseWithKit(InArmSuck.iWhichSht,1)==false)
                    InArmSuck.iWhichKit=1;
                else
                    InArmSuck.iWhichKit=0;
            }
            else
            {
                 InArmSuck.iWhichKit=0;
            }

            if(IniConfig.bG12ContractModeManualMessage)                         //kevin 20180222 (Steven) Arm 1 Arm2 吸取IC 做CONTRACT MODE
            {
                iContractModeTest++;                                            //kevin 20180222 (Steven) contract mode 手動送測試訊號
                if(iContractModeTest>=2)
                {
                    iContractModeTest=0;
                    fMain->CleanOut("AdjustShuttleWhichKitOrder 1");
                }
            }
        }
    }
    else
    {
        if(TestIF_File.iShuttleMode==1)                                         //JerryYang 20161107 修正關arm異常
        {
            AdjustShuttlePlaceOrder();
            if(IniConfig.bG12ContractModeManualMessage)                         //kevin 20180222 (Steven) Arm 1 Arm2 吸取IC 做CONTRACT MODE
            {
                fMain->CleanOut("AdjustShuttleWhichKitOrder 2");
            }
        }
        else
        {
            AdjustShuttlePlaceOrder(InArmSuck.iWhichSht+1);

            if(IniConfig.bG12ContractModeManualMessage)                         //kevin 20180222 (Steven) Arm 1 Arm2 吸取IC 做CONTRACT MODE
            {
                iContractModeTest++;                                            //kevin 20180222 (Steven) contract mode 手動送測試訊號
                if(iContractModeTest>=2)
                {
                    iContractModeTest=0;
                    fMain->CleanOut("AdjustShuttleWhichKitOrder 3");
                    bLoadContractModeTest=false;
                }
            }
        }
    }
    DoInArm_SuckerMap();
    bInArmPlaceToShuttleFinish=false;                                           //Ifor 20171121 : add iSht & iWhichKit 切換旗標 避免沒切換狀態造成Hangup
}
//==============================================================================
void ResetInArmParam()
{
    InitInArmTask();
}
//==============================================================================
void DoInArm_SuckerMapForCloseArm()
{
    if(TestIF_File.iShuttleMode==1)
    {
        if(TestIF_File.iShuttle_Sel==0)                                         //只用Arm 1
        {
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.fInArmSuck4x8[1][i][j]=false;
                }
            }
        }
        else                                                                    //只用Arm 2
        {
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.fInArmSuck4x8[0][i][j]=false;
                }
            }
        }
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            Prod.fNeedToCheckASM[0][i][j]=Prod.fInArmSuck4x8[0][i][j];
            Prod.fNeedToCheckASM[1][i][j]=Prod.fInArmSuck4x8[1][i][j];
        }
    }
    Prod.bInitialAutoSiteMap=false;
}
//==============================================================================
void DoInArm_SuckerMap()
{
    DoInArm_9045_Type();                                                        //Steven 20210420 : 修正12site關site跑2x4 mode, HP=6x11 hang up
    DoInArm_9045_SuckerMap();
    iCloseSiteState=CloseSiteState(true);                                       //Eliot 2009_03_06
    iCloseSiteStateForPick=CloseSiteState(false);                               //Steven 20150924 : 隨時開關Site功能
    return;
}
//==============================================================================
//  供料手臂從 Loader tray 取IC動作流程
//==============================================================================
void ChangeHotPlateData(bool bSwapSht)                                          //Steven 20100816 : 修正OneCycle時，會導致UPH降低的問題
{
    uHPSuckTeam *HTTeam;

    HTTeam=PickFromHPList->ExtractLastTeamHasData();
    if(HTTeam!=NULL)
    {
        InArmSuck.iWhichSht=HTTeam->iSht;
        InArmSuck.iWhichKit=HTTeam->iKit;
        AdjustShuttleWhichKitOrder();                                           //Steven 20160201 : 修正Shuttle放料順序
    }
    else
    {
        AdjustShuttlePlaceOrder();
    }
}
//==============================================================================
void ChangeHotPlateDataToOneShuttle()                                           //JerryYang 20171218 (Steven) fix 加熱模式雙arm切單arm hang up問題
{
    if(LastSet.iTemperature==Tempture_Hot)
    {
        if(IndexHasIC()==false || ShuttleHasIC()==false || InArmSuck.HasIC()==false)
        {
            if(TestIF.iShuttleMode==1)
            {
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<50; j++)
                    {
                        for(int k=0; k<50; k++)
                        {
                            iHotWhichShuttle[i][j][k]=TestIF_File.iShuttle_Sel;
                        }
                    }
                }
            }
        }
    }
}
//==============================================================================
void InitArmPickFromLoadStageTask()
{
    iPickFromLoadStageTask=1;
    bInArmPickErrFromLoader=false;                                              //Steven 20170828 (wei) : 確認是否有在Loader吸料異常
    CheckInArmDestroyActiveFinishInitial();
    bDoPreciser=false;                                                          //Frank 20180410 (Steven) : InArm Preciser Station
    PreSetInArmAdditionalFlag();
}
//==============================================================================
bool MoveInArmZToPlateSafe(int iTask)
{
    bool OK=true;
    int iAlarmSuck=0, iAlarmSuck1=0;
    static int iRetryCount=0;
    static int iRetryFail=0;
    int iMot=0;
    #ifndef SOFT_SIMULTE
    bool bHomeFlag=false;
    int iZPos=0;
    AnsiString sData="";
    #endif

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMot=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
            if(MOT[iMot].MotorMove(Prod.ZInArmSafe[i][j])==false)
            {
                OK=false;
                #ifndef SOFT_SIMULTE
                MOT[iMot].ScanMotorStatus();                                    //JerryYang 20200924 : Z軸失步偵測
                bHomeFlag=MOT[iMot].Led[iHomeLed];
                if(bHomeFlag)
                {
                    iZPos=MOT[iMot].ReadPos();
                    if(iZPos<-200)
                    {
                        sData.sprintf("%s pos %d home sensor on", MOT[iMot].Alias, iZPos);
                        if(CUSTOMER_CODE==CC_ATEC ||                            //Steven 20221224 : 艾科暫時不檢查Z軸Home Sensor
                           CUSTOMER_CODE==CC_KYEC_LEE)
                            MyDBIProcess("Motion", sData, "MoveInArmZToPlateSafe");
                        else
                            ShowMyMessage(sData, "", "MoveInArmZToPlateSafe");
                        SetInArmHome();
                    }
                }
                #endif
            }
            else                                                                //Steven 20220522 : 針對Z軸加上位置確認
            {
                #ifndef SOFT_SIMULTE
                iZPos=MOT[iMot].ReadPos();
                if(iZPos>Prod.ZInArmSafe[i][j]+100)
                {
                    sData.sprintf("Motor %s pos %d error!!", MOT[iMot].Alias, iZPos);
                    ShowMyMessage(sData, "");
                    SetInArmHome();
                }
                #endif
            }
        }
    }

    if(OK)
    {
        iAlarmSuck=InArmZSafe(DETECT_ALL_FLAG);
        if(iAlarmSuck!=-1)
        {
            if(iRetryCount>50)
            {
                iRetryCount=0;
                if(iRetryFail<3)
                {
                    iRetryFail++;
                    SetInArmHome();
                }
                else
                {
                    ShowErrorMessage("WAR0157", 0, MInArmX, false, MOT[iAlarmSuck].NumberAlias);       //入料手臂 Z 沒有在歸零的位
                    iRetryFail=0;
                }
                return false;
            }
            else
            {
                iRetryCount++;
                return false;
            }
        }
        iRetryFail=0;
        iRetryCount=0;
    }

    if(OK)
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iMot=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
                MOT[iMot].fCMD=false;
            }
        }
    }

    if(OK==true && iAlarmSuck==-1)                                              //Jou 2011-02-09
    {
        iAlarmSuck1=InArmZSafe(DETECT_SENSOR_FLAG);
        if(iAlarmSuck1==-1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool MoveInArmZToPlateSafeAndCheckLoaderTray(int iTask)                                     //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
{
    static bool bfirstIn=true;
    static int iCheckCount=0;
    int ret=0;
    if(MoveInArmZToPlateSafe(iTask))
    {
        if(bfirstIn==true)
        {
            bfirstIn=false;
            iCheckCount=0;
        }

        if(LastSet.iRealDummy>=HAS_TRAY && MOT[MMTrayY].fHasTray==true)
        {
            if(Sen[SnLoaderSureTray].IsOff()==true || Sen[SnLoaderPreDete].IsOff()==true ||
              (Cylinder[C_TrayY_Fixer].GetOutBit()==true && Sen[SnLoaderFixCyPush].IsOn()==true))
            {
                iCheckCount++;

                if(iCheckCount>=3)
                {
                    bfirstIn=true;
                    ret=ShowErrorMessage("JAM0929", K_RETRY|K_SKIP, MMTrayY, false, "CheckLoaderTray"); //Steven 20120925 : JAM0909 -> JAM0929
                    if(ret==K_SKIP)
                    {
                        MOT[MMTrayY].ClearTray(__FUNC__);
                        MOT[MMTrayY].fHasTray=false;
                        bfirstIn=true;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                return false;
            }
        }

        bfirstIn=true;
        return true;
    }
    return false;
}
//==============================================================================
// Z 軸安全位置判斷       kevin 20120217
//==============================================================================
bool CheckInArmZ()
{
    int iMot;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMot=(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)?MInArmZA:InArmSuck.Suck[i][j].iMotNo;
            if(MOT[iMot].ReadPos()!=Prod.ZInArmSafe[i][j])
            {
                SetInArmHome();
                return true;
            }
        }
    }
    return false;
}
extern int CheckSuckInitialStatus(TMyKitSuck &Ptr, int iR, int iC);
//==============================================================================
bool CheckInArmSuckInitial()                                                    //Steven 20100104 : 將所有錯誤整合只顯示一次
{
    int ret=0;                                                                  //Jimmychiu 20230307 initialized in the constructor
    int flag[MAX_ARM_Row][MAX_ARM_Col]={{0, 0, 0, 0}, {0, 0, 0, 0}};
    bool bHasFail=false;
    int iXpos, iYpos;
    AnsiString errSuck="at in arm Suck";
    ZeroMemory(flag, sizeof(flag));

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            flag[i][j]=CheckSuckInitialStatus(InArmSuck, i, j);
            if(flag[i][j]!=0)
            {
                iXpos=MOT[MInArmX].ReadPos();
                iYpos=MOT[MInArmY].ReadPos();
                bHasFail=true;
                errSuck+=InArmSuck.Suck[i][j].sName;
                InArmSuck.PordRec[i][j].AddErrorRecordNoSave("WAR0132", true, iXpos, iYpos);
            }
        }
    }
    if(bHasFail)
    {
        if(bDoPreciser==false &&                                                //Steven 20191113 : 使用HP當Preciser
           (TestIF_File.bEnableUsePreciser==true ||
            (TestIF_File.bEnableRTPreciser && LastSet.iRunStartMode==rsmContinuRetest)))
            bHasFail=false;
        else
            ret=ShowErrorMessage("WAR0132", K_RETRY|K_SKIP, MInArmX, false, errSuck);
    }

    if(ret==K_SKIP)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(flag[i][j]>=Vaccum_Initial_Off)
                {
                    iXpos=MOT[MInArmX].ReadPos();
                    iYpos=MOT[MInArmY].ReadPos();
                    InArmSuck.PordRec[i][j].AddErrorRecord("WAR0132", true, iXpos, iYpos);  //Steven 20160114 (jou) : Add Production Error Log

                    if(InArmSuck.Item[i][j]==HAS_HOT_IC)
                        InArmSuck.SetItemData(i, j, NULL_IC);
                    else
                        InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                }
            }
        }
    }
    return !bHasFail;
}

//==============================================================================
void SetInArmHome()
{
    AnsiString sHomeState="";                                                   //Sam 20230707 : 新增 InOutArm Z Home前Home sensor 狀態
    sHomeState=sInArmZHomeState();
    NewRecordProcess("", "InArm home sensor status", sHomeState);
    InitDoArmZHome();
    bNeedArmZHome=true;
    bIsInArmHome=true;

    bInArmPitchNeedHomeCheck=true;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            bInArmZNeedHome[i][j]=true;
        }
    }
}
//-----------------------------------------------------------------------------
bool IsMoveInArm2XYToWait()
{
    if(MOT[MInArmX].Led[iInposLed]==false &&
       MOT[MInArmY].Led[iInposLed]==false &&
       MOT[MInArmX].CompareEncoderPos(Prod.iInArmSafeX, 9)==1 &&                //Steven 20240110 : fixed for ART hang up   //Steven 20250605 : in arm 到位卡控 2 --> 9
       MOT[MInArmY].CompareEncoderPos(Prod.iInArmSafeY, 9)==1 &&                //Steven 20241130 : CompareCommandPos --> CompareEncoderPos
       MOT[MInArmX].CompareCommandPos(Prod.iInArmSafeX, 2)==1 &&
       MOT[MInArmY].CompareCommandPos(Prod.iInArmSafeY, 2)==1)                  //JerryYang 20250312 : 避免encoder在安全位置但Command pulse已經被改變
    {
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
bool MoveInArm2XYToWait()
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(bMoveInArm2XYToWaitOk==true)                                             //Steven 20131025 : 要先動到安全位置才可以再到別的地方
    {
        if(bDoTrayDeviceCheck)                                                  //wei 20170317 (steven) Device Remain 殘料檢測
        {
            return false;
        }

        if(IniConfig.bAlarmNeedServoOff)                                        //Steven 20111226 : In Arm 咖啡杯解除
        {
            if(fNote->bMyServoOffInArm)
            {
                return false;
            }
        }
    }

    if(IniConfig.bE61InArmStandbyPosOnLoader && bOneCycleInArmToLoader)         //JerryYang 20260129 : 移到外面  //JerryYang 20200206 one cycle, clean out, tray feed時in arm移動到loader
    {
        return false;
    }

    if(TRAY_ARM_MODE==eUnderCoveyor ||
       IsMoveInArm2XYToWait())
    {
        bMoveInArm2XYToWaitOk=true;                                             //Steven 20131025 : 要先動到安全位置才可以再到別的地方
        return true;
    }

    iXVariable[0]=MOT[MInArmPitch].ReadPos();
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iXVariable[2]=MOT[MInArmPitchX3].ReadPos();
        iXVariable[3]=MOT[MInArmPitchX4].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }

    iInArmWaitPosition=0;                                                       //Ifor 20191105 : add InArm 安全位置判斷 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
    bMoveInArm2XYToWaitOk=false;                                                //Steven 20131025 : 要先動到安全位置才可以再到別的地方
    if(InArmContinuousMove_9045(Prod.iInArmSafeX, Prod.iInArmSafeY, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        bMoveInArm2XYToWaitOk=true;                                             //Steven 20131025 : 要先動到安全位置才可以再到別的地方
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MoveInArm2XYToLoaderWait()                                                 //Steven 20120314 : 要移動到不熱的等待位置
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            return false;
        }
    }

    if(MOT[MTrayX].ReadPos()<Prod.iXTrayEmpty)                                  //JerryYang 20200206 one cycle, clean out, tray feed時in arm移動到loader
    {
        MOT[MInArmX].PCIL132_StopMotor();
        MOT[MInArmY].PCIL132_StopMotor();
//        if(MOT[MInArmY].ReadPos()<Prod.iInArmSafeY)                             //RogerYang 20250820 : InArmY到loader區才判斷
//            return false;
//        else
            ShowMyMessage("Tray arm not Safe pos", "MoveInArm2XYToLoaderWait");

        return false;
    }

    iXVariable[0]=MOT[MInArmPitch].ReadPos();
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iXVariable[2]=MOT[MInArmPitchX3].ReadPos();
        iXVariable[3]=MOT[MInArmPitchX4].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }

    iInArmWaitPosition=1;                                                       //Ifor 20191105 : add InArm 安全位置判斷 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
    if(InArmContinuousMove_9045(Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+6000, Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-10000, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool IsMoveInArm2XYToShuttle2Wait()                                             //Sam 20250522 : 增加保護，避免已經在跑殘料檢測時 InArm 在干涉區
{
    int iX=0, iY=0;
    iX=MOT[MInArmX].ReadPos();
    iY=MOT[MInArmY].ReadPos();
    if(iX==Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase] && iY==Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase])
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MoveInArm2XYToShuttle2Wait()
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            return false;
        }
    }

    iXVariable[0]=MOT[MInArmPitch].ReadPos();
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iXVariable[2]=MOT[MInArmPitchX3].ReadPos();
        iXVariable[3]=MOT[MInArmPitchX4].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }

    iInArmWaitPosition=3;                                                       //Ifor 20191105 : add InArm 安全位置判斷 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
    if(InArmContinuousMove_9045(Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase], Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase], iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MoveInArm2XYToDecayTeach()                                                 //Ifor 20151210 :新增InArm Move to Decay Teach 點位
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable=0;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            #ifdef DEBUG_AutoHomeLog
            if(fAllMotorHome==true)
            {
                NewRecordProcess("", "fAllMotorHome", "MoveInArm2XYToDecayTeach");
            }
            #endif

            fAllMotorHome=false;                                                //Ifor 20151211 須重新回home
            if(CUSTOMER_CODE==CC_SIGURD_PeiXing)
            {
                ShowMyMessage("Input Arm Servo Off, need home");                //JerryYang 20160328 for 矽格北興,觸發回home的地方補上log
            }
            return false;
        }
    }

    iXVariable[0]=MOT[MInArmPitch].ReadPos();
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //Steven for HT1032
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        iXVariable[1]=MOT[MInArmPitchX2].ReadPos();
        iXVariable[2]=MOT[MInArmPitchX3].ReadPos();
        iXVariable[3]=MOT[MInArmPitchX4].ReadPos();
        iYVariable   =MOT[MInArmPitchY].ReadPos();
    }

    iInArmWaitPosition=2;                                                       //Ifor 20191105 : add InArm 安全位置判斷 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
    if(InArmContinuousMove_9045(Prod.iInArmDecay_X, Prod.iInArmDecay_Y, iXVariable, iYVariable, bZFlag, iZPos, false))
    {
        bMoveInArm2XYToWaitOk=true;                                             //Steven 20131025 : 要先動到安全位置才可以再到別的地方
        return true;
    }
    return false;
}
//==============================================================================
int iInArmPlaceToHotPlateTask=1;
//==============================================================================
void InitInArmPlaceToHotPlateTask()
{
    iInArmPlaceToHotPlateTask=1;
}
//==============================================================================
void InitInArmPlaceToHotPlateTask400()                                          //ChungHung 20120502
{
    iInArmPlaceToHotPlateTask=400;
}
//==============================================================================
void InitInArmPlaceToHotPlateTask100()
{
    iInArmPlaceToHotPlateTask=100;
}
//------------------------------------------------------------------------------
bool RowCanDualSite()
{
    if((TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site1X4) && TestIF.iUseSuckMode==4) //ChungHung 20150528 add for 海思 _8Site1x4
        return false;
    if(ArmCanSuck4IC(0)==false)
        return true;
    if(HotPlateForm.XDivision==6)
        return false;

    return false;
}
//------------------------------------------------------------------------------
void InitInArmTask()
{
    bShuttleShake=false;                                                        //ChungHung 20120612 hangUp 解除
    bShuttleKnock=false;
    bContinusRotate=false;

    if(IniConfig.bP17InArmFullPickFromLoader)                                   //Steven 20111026 : In Arm Full Pick from Loader Tray
    {
        if(iArmTask==100 && InArmSuck.HasIC())                                  //如果In Arm吸到一半被中斷,要回來繼續吸
        {
            InitArmPickFromLoadStageTask();
        }
        else if(iArmTask==5100 && MOT[MInRotateKit].HasIC())                    //2013-04-12    Dell :旋轉站; 做rotate中途被歸零
        {
            InitialInArmRotateKIT();
            bContinusRotate=true;
        }
        else if(bPlaceShuttle==true && iArmTask==2000)
        {

        }
        else
        {
            iArmTask=1;
        }
    }
    else
    {
        if(bPlaceShuttle==true && iArmTask==2000)
        {

        }
        else
        {
            iArmTask=1;
        }
    }
}
//------------------------------------------------------------------------------
void DoInArm()
{
    if(bInitialStartIndexCheckDone==false)                                      //Sam 20221214 : 當機台 Initail Start 時需要先做 Index Check
    {
         return;
    }

    if(iHPHangUpCount!=0)                                                       //Steven 20161102 : 針對加熱盤Hang Up進行紀錄
    {
        RecordProcess("Auto State Record by in arm");
        fMain->DoStateRecord(0, false);                                         //KenHsieh 20230116 : 區分手動或自動(sbclick -> Function)
        ShowErrorMessage("WAR0150", 0, MInArmX, false, sHPHangUpFunc);
        ClearHotPlateHangUp();
        fAllMotorHome=false;
        return;
    }

    if(IniConfig.bF16CheckShuttleSensorBroken && bDoingF16)                     //Steven 20221213 : 確認shuttle 有沒有斷線
    {
        MOT[MInShuttle1].fCanMoveL=true;                                        //避免造成hang up
        MOT[MInShuttle2].fCanMoveL=true;
        MOT[MInShuttle1].fCanMoveM=true;
        MOT[MInShuttle2].fCanMoveM=true;
        return;
    }

    if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)             //Steven 20111005 : QA Mode
    {
        if(CosFunction.bQAModeUseUnloadCnt)                                     //JerryYang 20221004 : Maxim版本QA mode
        {

        }
        else
        {
            if(Check_QA_ModeCount())                                            //Steven 20111019 : QA Mode
            {
                int iKit=InArmSuck.iWhichKit;
                int iKit32=(iKit==1)?4:0;
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)                      //ChungHung 20130910 alter QA mode HangUp
                        if(Prod.fInArmSuck4x8[InArmSuck.iWhichSht][i][j+iKit32]==true &&
                           InArmSuck.Item[i][j]==NULL_IC)                       //ChungHung 20130910 alter for SCK can close site by Index
                            InArmSuck.SetItemData(i, j, HAS_NULL_IC);
                }
            }
        }
    }

    bool bDoInArmAutoAlignment=false;
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==true &&
       (LastSet.iRealDummy==HAS_TRAY || LastSet.iRealDummy==REALLY))            //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    {
        if(bLoaderNeedTrayMustFinish)
        {
            if(iCleanOut==1 && HasICUnderMachine()==false)                      //process clean out ,don't supply new tray
            {
                bLoaderNeedTrayMustFinish=false;
                return;
            }
            else
            {
                if(MOT[MMTrayY].fHasTray)
                {
                    bDoInArmAutoAlignment  = CheckInArmAutoAlignmentTrayModeBeUse(lInArmAutoAlignmentTrayTimingFlag, true);
                    bDoInArmAutoAlignment |= CheckInArmAutoAlignmentCKModeBeUse(lInArmAutoAlignmentCKTimingFlag, true);
                    bRunInArmAutoAlignment = bDoInArmAutoAlignment;

                    bLoaderNeedTrayMustFinish=false;
                    return;
                }
                else if(iCleanOut==1)                                           //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
                {
                    bDoInArmAutoAlignment  = CheckInArmAutoAlignmentTrayModeBeUse(lInArmAutoAlignmentTrayTimingFlag, true);
                    bDoInArmAutoAlignment |= CheckInArmAutoAlignmentCKModeBeUse(lInArmAutoAlignmentCKTimingFlag, true);
                    bRunInArmAutoAlignment = bDoInArmAutoAlignment;

                    bLoaderNeedTrayMustFinish=false;
                }
                else
                {
                    return;
                }
            }
        }
        else if(bRunInArmAutoAlignment || lOutArmAutoAlignmentFlag)
        {
            return;
        }
    }
    else
    {
        bLoaderNeedTrayMustFinish=false;
        bRunInArmAutoAlignment=false;
    }

    if(IniConfig.bIndexJamInArmAway)                                            //ChungHung 20110302
    {
        if(bInArmNeedToSafePos==true)                                           //Shuttle jam InArm 讓開
        {
            if(CheckInArmFinishAllPickerAction()==false)                        //Steven 20171226 (Wei) : 修改in arm讓開的flag
            {
                DoInArm_9045();
                return;
            }
            InitInArmTask();
            if(MoveInArm2XYToWait())
            {
                bInArmNeedToSafePos=false;
            }
        }
        else if(bShuttle1Pause==false && bShuttle2Pause==false)
        {
            if(CheckInArmFinishAllPickerAction()==false)                        //JerryYang 20180109 (Steven) fix in arm讓位後的hang up
            {
                DoInArm_9045();
                return;
            }
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip  &&                  //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
               LastSet.iTemperature==Tempture_Hot &&
               (InArmSuck.iWhichKitPickFor32==0 && bIndexPickUpErrMoveSht1==true ||
                InArmSuck.iWhichKitPickFor32==1 && bIndexPickUpErrMoveSht2==true) &&
               bPickFromLoader==false && bPlaceToHotplate==false && bDestoryOnSht==false)
            {
                return;
            }
            else
            {
                DoInArm_9045();
            }
        }
    }
    else
    {
        if(IniConfig.bD43IndexDropErrorCanRetryandSkip  &&                      //Steven 20171221 (Wei) : 修正[D43]當蝦頭退出來要回去前,如果In Arm補了HAS_NULL_IC在蝦頭上會造Hang up
           bIndexPickUpErrMoveSht1==true || bIndexPickUpErrMoveSht2==true)
        {
            return;
        }
        else
        {
            DoInArm_9045();
        }
    }
    return;
}
//------------------------------------------------------------------------------
void TransferHotPlateRatio(bool bPick, int *iXPos, int *iYPos)                  //Steven 20110324 : 加熱盤的軟體齒輪比
{
    double fi=0.0;
    int iOldX=0, iOldY=0, iRet=200;
    int iPlate=0;

    iOldX=*iXPos;
    iOldY=*iYPos;

    if(bPick==true)
    {
        iPlate=iPickPlate[0];
    }
    else
    {
        iPlate=iPlacePlate[0];
    }

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&                                        //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
       TestIF.bEnableAutoAlignment==true)                                       //KenHsieh 20210824 : add AOA功能開啟不補償
    {
        if(iPlate==0)
            CheckInArmXYScaleByAutoTeach(*iXPos, *iYPos, AOA_Plate2);                    //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
        else
            CheckInArmXYScaleByAutoTeach(*iXPos, *iYPos, AOA_Plate1);                    //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
    }
    else
    {
        if(iPlate==0)
        {
            if(Tri_Temp_Machine==1 && LastSet.iTemperature==Tempture_Hot)       //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
            {
                if(IniConfig.bE30_1InArmUseDifferentScale_Hot==true && Temperature.fWorkTemperBase>=26)
                {
                    fi=*iXPos-Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateXScale_Hot[1];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iXPos=fi+Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];

                    fi=*iYPos-Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateYScale_Hot[1];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iYPos=fi+Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
                }
                if(IniConfig.bE30_2InArmUseDifferentScale_Cold==true && Temperature.fWorkTemperBase<26)
                {
                    fi=*iXPos-Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateXScale_Cold[1];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iXPos=fi+Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];

                    fi=*iYPos-Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateYScale_Cold[1];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iYPos=fi+Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
                }
            }
            else                                                                //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
            {
                fi=*iXPos-Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];        //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
                if(IniConfig.bE30InArmUseDifferentScale)
                {
                    fi=fi*LastSet.fHotPlateXScale[1];
                }
                fi=fi*fHotPlateExpansionCoefficient;
                *iXPos=fi+Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];

                fi=*iYPos-Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
                if(IniConfig.bE30InArmUseDifferentScale)
                {
                    fi=fi*LastSet.fHotPlateYScale[1];
                }
                fi=fi*fHotPlateExpansionCoefficient;
                *iYPos=fi+Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase];
            }
        }
        else
        {
            if(Tri_Temp_Machine==1 && LastSet.iTemperature==Tempture_Hot)       //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
            {
                if(IniConfig.bE30_1InArmUseDifferentScale_Hot==true && Temperature.fWorkTemperBase>=26)
                {
                    fi=*iXPos-Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateXScale_Hot[0];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iXPos=fi+Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase];

                    fi=*iYPos-Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateYScale_Hot[0];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iYPos=fi+Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                }
                if(IniConfig.bE30_2InArmUseDifferentScale_Cold==true && Temperature.fWorkTemperBase<26)
                {
                    fi=*iXPos-Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateXScale_Cold[0];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iXPos=fi+Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase];

                    fi=*iYPos-Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                    if(IniConfig.bE30InArmUseDifferentScale)
                    {
                        fi=fi*LastSet.fHotPlateYScale_Cold[0];
                    }
                    fi=fi*fHotPlateExpansionCoefficient;
                    *iYPos=fi+Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                }
            }
            else                                                                //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
            {
                fi=*iXPos-Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                if(IniConfig.bE30InArmUseDifferentScale)
                {
                    fi=fi*LastSet.fHotPlateXScale[0];
                }
                fi=fi*fHotPlateExpansionCoefficient;
                *iXPos=fi+Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase];

                fi=*iYPos-Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
                if(IniConfig.bE30InArmUseDifferentScale)
                {
                    fi=fi*LastSet.fHotPlateYScale[0];
                }
                fi=fi*fHotPlateExpansionCoefficient;
                *iYPos=fi+Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase];
            }
        }

        if(abs(*iXPos-iOldX)>iRet)
            *iXPos=iOldX;
        if(abs(*iYPos-iOldY)>iRet)
            *iXPos=iOldX;
    }
}
//------------------------------------------------------------------------------
void TransferInShuttleRatio(int iShuttle, int *iXPos, int *iYPos, int iRow, int iCol)   //Steven 20110324 : In Shuttle的軟體齒輪比
{
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==true)     //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    {
        if(iShuttle==0)
            CheckInArmXYScaleByAutoTeach(*iXPos, *iYPos, AOA_InSht1);
        else
            CheckInArmXYScaleByAutoTeach(*iXPos, *iYPos, AOA_InSht2);
    }
    else                                                                        //KenHsieh 20210824 : add AOA功能開啟不補償
    {
        double fi=0.0;                                                          //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
        if(Tri_Temp_Machine==1 && LastSet.iTemperature==Tempture_Hot)
        {
            if(IniConfig.bE32_1ShuttleUseDifferentScale_Hot==true && Temperature.fWorkTemperBase>=26)
            {
                if(iShuttle==0)
                {
                    fi=*iXPos-Prod.XInArm_Shuttle1_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleXScale_Hot[0];
                    *iXPos=fi+Prod.XInArm_Shuttle1_Place[iRow][iCol];

                    fi=*iYPos-Prod.YInArm_Shuttle1_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleYScale_Hot[0];
                    *iYPos=fi+Prod.YInArm_Shuttle1_Place[iRow][iCol];
                }
                else
                {
                    fi=*iXPos-Prod.XInArm_Shuttle2_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleXScale_Hot[1];
                    *iXPos=fi+Prod.XInArm_Shuttle2_Place[iRow][iCol];

                    fi=*iYPos-Prod.YInArm_Shuttle2_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleYScale_Hot[1];
                    *iYPos=fi+Prod.YInArm_Shuttle2_Place[iRow][iCol];
                }
            }
            if(IniConfig.bE32_2ShuttleUseDifferentScale_Cold==true && Temperature.fWorkTemperBase<26)
            {
                if(iShuttle==0)
                {
                    fi=*iXPos-Prod.XInArm_Shuttle1_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleXScale_Cold[0];
                    *iXPos=fi+Prod.XInArm_Shuttle1_Place[iRow][iCol];

                    fi=*iYPos-Prod.YInArm_Shuttle1_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleYScale_Cold[0];
                    *iYPos=fi+Prod.YInArm_Shuttle1_Place[iRow][iCol];
                }
                else
                {
                    fi=*iXPos-Prod.XInArm_Shuttle2_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleXScale_Cold[1];
                    *iXPos=fi+Prod.XInArm_Shuttle2_Place[iRow][iCol];

                    fi=*iYPos-Prod.YInArm_Shuttle2_Place[iRow][iCol];
                    fi*=LastSet.fInShuttleYScale_Cold[1];
                    *iYPos=fi+Prod.YInArm_Shuttle2_Place[iRow][iCol];
                }
            }
        }                                                                       //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
        else if(IniConfig.bE32ShuttleUseDifferentScale)                         //jou 980716 start : add Scale
        {
            if(iShuttle==0)
            {
                fi=*iXPos-Prod.XInArm_Shuttle1_Place[iRow][iCol];
                fi*=LastSet.fInShuttleXScale[0];
                *iXPos=fi+Prod.XInArm_Shuttle1_Place[iRow][iCol];

                fi=*iYPos-Prod.YInArm_Shuttle1_Place[iRow][iCol];
                fi*=LastSet.fInShuttleYScale[0];
                *iYPos=fi+Prod.YInArm_Shuttle1_Place[iRow][iCol];
            }
            else
            {
                fi=*iXPos-Prod.XInArm_Shuttle2_Place[iRow][iCol];
                fi*=LastSet.fInShuttleXScale[1];
                *iXPos=fi+Prod.XInArm_Shuttle2_Place[iRow][iCol];

                fi=*iYPos-Prod.YInArm_Shuttle2_Place[iRow][iCol];
                fi*=LastSet.fInShuttleYScale[1];
                *iYPos=fi+Prod.YInArm_Shuttle2_Place[iRow][iCol];
            }
        }
    }
}
//------------------------------------------------------------------------------
void TransferLoaderRatio(int *iXPos, int *iYPos)                                //Steven 20141029 : Loader的軟體齒輪比
{
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&                                        //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
       TestIF.bEnableAutoAlignment==true)                                       //KenHsieh 20210824 : add AOA功能開啟不補償
    {
        CheckInArmXYScaleByAutoTeach(*iXPos, *iYPos, AOA_Loader);
    }
    else
    {
        double fi;                                                              //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
        if(LastSet.iTemperature==Tempture_Hot)
        {
            if(IniConfig.bE30_1InArmUseDifferentScale_Hot==true && Temperature.fWorkTemperBase>=26)
            {
                fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                fi*=LastSet.fLoaderTrayXScale_Hot;
                *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

                fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                fi*=LastSet.fLoaderTrayYScale_Hot;
                *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            }
            if(IniConfig.bE30_2InArmUseDifferentScale_Cold==true && Temperature.fWorkTemperBase<26)
            {
                fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                fi*=LastSet.fLoaderTrayXScale_Cold;
                *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

                fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
                fi*=LastSet.fLoaderTrayYScale_Cold;
                *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            }
        }                                                                       //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
        else if(IniConfig.bE30InArmUseDifferentScale)
        {
            fi=*iXPos-Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            fi*=LastSet.fLoaderTrayXScale;
            *iXPos=fi+Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase];

            fi=*iYPos-Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
            fi*=LastSet.fLoaderTrayYScale;
            *iYPos=fi+Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
        }
    }
}
//------------------------------------------------------------------------------
void BackupPlacePos()                                                           //ChungHung 20120206 Hotplate check
{
    iBackupPlate =iPlacePlate[0];
    iBackupPlateC=iPlacePlateX[0];
    iBackupPlateR=iPlacePlateY[0];
}
//------------------------------------------------------------------------------
void RestorePlacePos()                                                          //ChungHung 20120206 Hotplate check
{
    iPlacePlate[0] =iBackupPlate;
    iPlacePlateX[0]=iBackupPlateC;
    iPlacePlateY[0]=iBackupPlateR;
}
//------------------------------------------------------------------------------
bool IsHotPlateCheckFinsih()                                                    //ChungHung 20120206 Hotplate check
{
    int iCT=0, iDiscount=0, iCheckCT=0;
    int iHAS_IC=0;
    if(TestIF.iTestMode==_8Site2X4 && HotPlateForm_File.XPitch>80)              //Ifor 20151013 add For Xilinx
        iCheckCT=4;
    else if(TestIF.iTestMode==_8Site2X4 ||
            TestIF.iTestMode==_12Site2X6 ||
            TestIF.iTestMode==_10Site2X5)                                       //wei 20190614 10 site
        iCheckCT=8;
    else if(TestIF.iTestMode==QualSite2X2 ||
            TestIF.iTestMode==QualSite1X4 ||
            TestIF.iTestMode==_8Site1X4  ||                                     //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF.iTestMode==TriSite1X3 ||
            TestIF.iTestMode==_6Site2X3N ||                                     //Steven 20220425 : 2X3NN Mode
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
        iCheckCT=4;
    else if(TestIF.iTestMode==DualSite ||
            TestIF.iTestMode==DualSite2x1 ||
            TestIF.iTestMode==QualSite2X2N)                                     //Frank 20200520 2X2NN Mode
        iCheckCT=2;
    else
        iCheckCT=8;

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<HotPlateForm.XDivision; j++)
        {
            for(int k=0; k<HotPlateForm.YDivision;k++)
            {
                if(HotPlateForm.iPlateSelect & 2-i)                             //MMPlate1+0=NO 2 HP
                {
                    if(HotPlateForm.iPlateSelect==3 && i==0)
                        continue;
                    if(MOT[MMPlate1+i].Tray.Data[j][k]==NULL_IC)
                        iCT++;
                    if(MOT[MMPlate1+i].Tray.Data[j][k]==HAS_IC ||
                       MOT[MMPlate1+i].Tray.Data[j][k]==HAS_HOT_IC ||
                       MOT[MMPlate1+i].Tray.Data[j][k]==HAS_NULL_IC)
                        iHAS_IC++;
                }
            }
        }
    }

    if(bOneTimeHotPlateCheckAll)
    {
        iDiscount=0;
    }
    else
    {
        if(iPlacePlateX[0]==0 && iPlacePlateY[0]==0)
        {
            if((HotPlateForm.iPlateSelect & 2) && iPlacePlate[0]==0)
            {
                if(iHAS_IC!=0)
                    return true;
            }
            else if((HotPlateForm.iPlateSelect==1) && iPlacePlate[0]==1)
            {
                if(iHAS_IC!=0)
                    return true;
            }
            return false;
        }
        else
        {
            return false;
        }
    }
    if(TestIF.iTestMode==_8Site2X4 && HotPlateForm_File.XPitch>80)              //Ifor 20151013 add For Xilinx
    {
        if(iCT<=0)
            return true;
    }
    else if(bHPCleanout)                                                        //wei 20160624 Hotplate clean out
    {
        if(TestIF.iTestMode==_6Site2X3 ||
           TestIF.iTestMode==_8Site2X4 ||
           TestIF.iTestMode==_16Site4X4)                                        //Sam 20190226 : 16Site4X4
        {
            if(iCT<4)
                return true;
        }
        else
        {
            if(iCT<=0)
                return true;
        }
    }
    else
    {
        if(iCT-iDiscount<iCheckCT)
            return true;
    }

    return false;
}
//ChungHung 20110130 End
//------------------------------------------------------------------------------
//enum eShakeShuttle{essSetFlag=1,
//                   essMoveRight,
//                   essMoveLeft,
//                   essKnockOn,
//                   essKnockOff,
//                   essEnd};
int iShakeShuttleTask;
extern bool IndexZCanMove[2];
TQPF_Timer ShakeShuttleDelay;
TQPF_Timer ShakeDelay;
//AI(ht9045-v899) 20260831: TQPF_Timer 走的是掛鐘, 暫停期間照計時, 導致處理完 alarm 按 Start 會在 2~3 秒內誤報 Shake Shuttle Time Out
//AI(ht9045-v899) 20260831: 改由 resume 上升緣呼叫本函式重設, 讓 10 秒逾時只計算機台實際執行中的時間
void ResetShakeShuttleTimeOut()
{
    if(iShakeShuttleTask==essSetFlag)                                           //只在等互鎖放行的狀態重設, 避免影響共用同一個計時器的 Knock 流程
        ShakeShuttleDelay.SetMSAndOn(10000);
}
//------------------------------------------------------------------------------
bool DoShakeShuttle(int iShuttle, bool bNeedInitial)                            //Steven 20120801 : 修改抖抖功能
{
    static bool bInitial=true;
    static int iSCT;
    ishuttleShake=iShuttle;                                                     //kevin 20190107 add shuttle shake state
    if(bInitial || bNeedInitial)
    {
        iShakeShuttleTask=essSetFlag;
        bInitial=false;
        bShuttleKnock=true;                                                     //Jou 2013-03-08 修改敲敲功能
        ShakeShuttleDelay.SetMSAndOn(10000);                                    //Ifor 20180329 : add Shake Shuttle Time Out 判斷
        if(CUSTOMER_CODE==CC_KYEC_XILINX)
            RecordProcess("Shake Shuttle Initial");
        return false;
    }

    if(bRunAutoClean && TestIF.iAutoClean_Function &&
       TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)                            //Sam 20220705 : 會 Hang up AutoClean 不需要檢查。
    {
        if((iShuttle==0 && bZ1Isdownflag==true) || (iShuttle==1 && bZ2Isdownflag==true))
        {
            if(iShuttle==0)
                bZ1Isdownflag=false;
            else if(iShuttle==1)
                bZ2Isdownflag=false;
        }
    }
    else
    {
       if((iShuttle==0 && bZ1Isdownflag==true) ||
          (iShuttle==1 && bZ2Isdownflag==true))                                 //Isaac 20180307 (Steven) index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖
       {
           return false;
       }
    }

    switch(iShakeShuttleTask)
    {
        case essSetFlag:
            if((bRunAutoClean && TestIF.iAutoClean_Function && TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir) ||                 //Steven 20211213 : For Auto Clean置偏
               ((IndexZCanMove[0]==true && IndexZCanMove[1]==true) &&           //JerryYang 20190628 強制shuttle shake
                MOT[MInShuttle1+iShuttle].fCanMoveM==true &&
                MOT[MInShuttle1+iShuttle].fCanMove==true))                      //ChungHung 20150520 add fix Fix3 full 和 ShuttleShanke 衝突  // now Index Z1,Z2 not down
            {
                SetMotorAccelSpeed(MInShuttle1+iShuttle, SHSpeed_File.iShakeAccDec);      //Sam 20250326 : 新增 Shake 條件設定
                SetMotorScaleSpeed(MInShuttle1+iShuttle, IniConfig.iF01ShuttleShakeSpeed);

                IndexZCanMove[0]=false;
                IndexZCanMove[1]=false;
                bShuttleShake=true;                                             // disable all process exclude inarm
                SetShuttlefCanMoveL(iShuttle, true, __FUNC__, IntToStr(essSetFlag));
                iSCT=SHSpeed_File.iShakeCycles;                                 //Sam 20250326 : 新增 Shake 條件設定
                iShakeShuttleTask=essMoveRight;
                ShakeDelay.SetSecAndOn(0.01);                                   //Sam 20250326 : 新增 Shake 條件設定
                break;
            }
            else if(bIndexWaitingInArmAway==true &&                             //Steven 20171228 (Wei) : Index在等In Arm讓開
                   IndexZCanMove[0]==true &&
                   IndexZCanMove[1]==true &&
                   bUseFix3CylinderActive==false)
            {
                if(iShuttle==0 && MOT[MInShuttle1].fCanMoveM==false && InSHT1InLF())
                {
                    MOT[MInShuttle1].fCanMoveM=true;
                }

                if(iShuttle==1 && MOT[MInShuttle2].fCanMoveM==false && InSHT2InLF())
                {
                    MOT[MInShuttle2].fCanMoveM=true;
                }
            }
            else if(ShakeShuttleDelay.Off())                                    //Ifor 20180329 : add Shake Shuttle Time Out 判斷
            {
                AnsiString str1, str2, str3;
                str1.sprintf("Shake Shuttle Time Out!!");
                str2.sprintf("ShuttleShake:%d  Fix3Cylinder:%d", bShuttleShake, bUseFix3CylinderActive);
                //AI(ht9045-v899) 20260831: 補回漏掉的 % 避免 fCanMoveM/fCanMove 欄位與數值錯位, 並加印 iShuttle 以辨識是哪一支飛梭
                str3.sprintf("Shake Shuttle Time Out!!, Shuttle:%d, ShuttleShake:%d  Fix3Cylinder:%d, bIndexWaitingInArmAway:%d, IndexZCanMove[0]:%d, IndexZCanMove[1]:%d, bShuttleShake:%d, fCanMoveL:%d, fCanMoveM:%d, fCanMove:%d",
                             iShuttle+1, bShuttleShake, bUseFix3CylinderActive, bIndexWaitingInArmAway, IndexZCanMove[0], IndexZCanMove[1], bShuttleShake, MOT[MInShuttle1+iShuttle].fCanMoveL, MOT[MInShuttle1+iShuttle].fCanMoveM, MOT[MInShuttle1+iShuttle].fCanMove);
                RecordProcess(str3);
                ShowMyMessage(str1, str2);
                bUseFix3CylinderActive=false;
                ShakeShuttleDelay.SetMSAndOn(10000);                            //kevin 20190107 add time resetIfor 20180329 : add Shake Shuttle Time Out 判斷
                fMain->Pause("DoShakeShuttle");
            }
            break;
        case essMoveRight:
            if(ShakeDelay.Off()==false && SHSpeed_File.dShakeDelay!=0.0)        //Sam 20250326 : 新增 Shake 條件設定
                break;

            if(MOT[MInShuttle1+iShuttle].MotorMoveShuttleShake(Prod.InSHT[iShuttle].iLeft+SHSpeed_File.iShakeDistance*100)) //Sam 20250326 : 新增 Shake 條件設定
            {
                ShakeDelay.SetSecAndOn(SHSpeed_File.dShakeDelay);               //Sam 20250326 : 新增 Shake 條件設定
                iShakeShuttleTask=essMoveLeft;
            }
            break;
        case essMoveLeft:
            if(ShakeDelay.Off()==false && SHSpeed_File.dShakeDelay!=0.0)                                         //Sam 20250326 : 新增 Shake 條件設定
                break;

            if(MOT[MInShuttle1+iShuttle].MotorMoveShuttleShake(Prod.InSHT[iShuttle].iLeft))  //JerryYang 20190628 shuttle shake專用command
            {
                iSCT--;
                if(iSCT<=0)
                {
                    iShakeShuttleTask=essEnd;
                    ShakeShuttleDelay.SetMSAndOn(50);
                }
                else
                {
                    ShakeDelay.SetSecAndOn(SHSpeed_File.dShakeDelay);           //Sam 20250326 : 新增 Shake 條件設定
                    iShakeShuttleTask=essMoveRight;
                }
            }
            break;
        case essEnd:
            if(ShakeShuttleDelay.Off())
            {
                Cylinder[C_Shuttle_Knocker_1+iShuttle].Off();                   //Steven 20130108 : 避免蝦頭敲敲汽缸打開撞斷
                SetMotorAccelSpeed(MInShuttle1+iShuttle, SHSpeed.iSH1ACDCSp);   //Steven 20160601 : fix shuttle shake speed
                SetMotorScaleSpeed(MInShuttle1+iShuttle, SHSpeed.iSH1Sp);

                IndexZCanMove[0]=true;                                          //Steven 20200519 : fix for hang up
                IndexZCanMove[1]=true;
                bInitial=true;
                iShuttleLog=2;                                                  //JerryYang 20181121 (Steven) : add log,記錄in shuttle device是被震動馬達or搖搖功能導正
                DoKnockShuttle(iShuttle, true);
                SetShuttlefCanMoveL(iShuttle, false, __FUNC__, IntToStr(essEnd));
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iKnockShuttleTask;
bool DoKnockShuttle(int iShuttle, bool bNeedInitial)                            //Jou 2013-03-08 修改敲敲功能
{
    static bool bInitial=true;
    static int iSCT=3;
    if(bInitial || bNeedInitial)
    {
        iKnockShuttleTask=essSetFlag;
        bInitial=false;

        if(CUSTOMER_CODE==CC_KYEC_XILINX)
            RecordProcess("Knock Shuttle Initial");
        return false;
    }

    switch(iKnockShuttleTask)
    {
        case essSetFlag:
            if(MOT[MInShuttle1+iShuttle].fCanMoveM==true &&
               MOT[MInShuttle1+iShuttle].fCanMove==true)                        // now Index Z1,Z2 not down
            {
                IndexZCanMove[0]=false;
                IndexZCanMove[1]=false;
                bShuttleShake=true;                                             // disable all process exclude inarm
                SetShuttlefCanMoveL(iShuttle, true, __FUNC__, IntToStr(essSetFlag));
                iKnockShuttleTask=essMoveLeft;
                break;
            }
            break;
        case essMoveLeft:
            if(MOT[MInShuttle1+iShuttle].MotorMove(Prod.InSHT[iShuttle].iLeft))
            {
                if(IniConfig.bF14KnockShuttle &&
                   Cylinder[C_Shuttle_Knocker_1+iShuttle].Enable)               //Steven 20120801 : Shuttle敲敲
                {
                    iKnockShuttleTask=essKnockOn;
                    ShakeShuttleDelay.SetSecAndOn(IniConfig.dF14KnockShuttleInterval);
                    iSCT=IniConfig.iF14KnockShuttleNo;
                }
                else
                {
                    iKnockShuttleTask=essEnd;
                    ShakeShuttleDelay.SetMSAndOn(50);
                }
                SetShuttlefCanMoveL(iShuttle, false, __FUNC__, IntToStr(essMoveLeft));
            }
            break;
        case essKnockOn:
            if(ShakeShuttleDelay.Off())
            {
                Cylinder[C_Shuttle_Knocker_1+iShuttle].On();
                ShakeShuttleDelay.SetSecAndOn(IniConfig.dF14KnockShuttleInterval);
                iKnockShuttleTask=essKnockOff;
            }
            break;
        case essKnockOff:
            if(ShakeShuttleDelay.Off())
            {
                Cylinder[C_Shuttle_Knocker_1+iShuttle].Off();
                ShakeShuttleDelay.SetSecAndOn(IniConfig.dF14KnockShuttleInterval);
                iSCT--;
                if(iSCT<=0)
                {
                    iKnockShuttleTask=essEnd;
                }
                else
                {
                    iKnockShuttleTask=essKnockOn;
                }
            }
            break;
        case essEnd:
            if(ShakeShuttleDelay.Off())
            {
                Cylinder[C_Shuttle_Knocker_1+iShuttle].Off();                   //Steven 20130108 : 避免蝦頭敲敲汽缸打開撞斷
                bInitial=true;
                bShuttleKnock=false;
                IndexZCanMove[0]=true;                                          //Steven 20200519 : fix for hang up
                IndexZCanMove[1]=true;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iKnockShtFirstTask;
bool DoKnockShuttleFirst(int iShuttle, bool bNeedInitial)                       //jou 2015-12-09 SCS 要求 Shuttle 每次入料前 敲擊
{
    static bool bInitial=true;
    static int iSCT=3;
    if(bInitial || bNeedInitial)
    {
        iKnockShtFirstTask=essSetFlag;
        bInitial=false;
        return false;
    }

    switch(iKnockShtFirstTask)
    {
        case essSetFlag:
            if(MOT[MInShuttle1+iShuttle].fCanMoveM==true &&
               MOT[MInShuttle1+iShuttle].fCanMove==true)                        // now Index Z1,Z2 not down
            {
                IndexZCanMove[0]=false;
                IndexZCanMove[1]=false;
                bShuttleShake=true;                                             // disable all process exclude inarm
                SetShuttlefCanMoveL(iShuttle,true,__FUNC__,IntToStr(essSetFlag));
                iKnockShtFirstTask=essMoveLeft;
                break;
            }
            break;
        case essMoveLeft:
            if(MOT[MInShuttle1+iShuttle].MotorMove(Prod.InSHT[iShuttle].iLeft))
            {
                if(IniConfig.bF14_1KnockShuttleFirst && Cylinder[C_Shuttle_Knocker_1+iShuttle].Enable)
                {
                    iKnockShtFirstTask=essKnockOn;
                    ShakeShuttleDelay.SetSecAndOn(IniConfig.dF14KnockShuttleIntervalFirst);
                    iSCT=IniConfig.iF14KnockShuttleNoFirst;
                }
                else
                {
                    iKnockShtFirstTask=essEnd;
                    ShakeShuttleDelay.SetMSAndOn(50);
                }
            }
            break;
        case essKnockOn:
            if(ShakeShuttleDelay.Off())
            {
                Cylinder[C_Shuttle_Knocker_1+iShuttle].On();
                ShakeShuttleDelay.SetSecAndOn(IniConfig.dF14KnockShuttleIntervalFirst);
                iKnockShtFirstTask=essKnockOff;
                MOT[MInShuttle1+iShuttle].fCanMoveL=false;                      //Steven 20200422 : add protection
                SetShuttlefCanMoveL(iShuttle, false, __FUNC__, IntToStr(essKnockOn));
            }
            break;
        case essKnockOff:
            if(ShakeShuttleDelay.Off())
            {
                Cylinder[C_Shuttle_Knocker_1+iShuttle].Off();
                ShakeShuttleDelay.SetSecAndOn(IniConfig.dF14KnockShuttleIntervalFirst);
                iSCT--;
                if(iSCT<=0)
                {
                    iKnockShtFirstTask=essEnd;
                }
                else
                {
                    iKnockShtFirstTask=essKnockOn;
                }
            }
            break;
        case essEnd:
            if(ShakeShuttleDelay.Off())
            {
                Cylinder[C_Shuttle_Knocker_1+iShuttle].Off();
                bInitial=true;
                bShuttleKnock=false;
                IndexZCanMove[0]=true;                                          //Steven 20200519 : fix for hang up
                IndexZCanMove[1]=true;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
//   移動XY位置
//------------------------------------------------------------------------------
bool bMoveInArmXYToTeach(bool Reset, int ZNumber, int iInArm)
{                                                                               //目前使用吸嘴
    int iXPos=0, iYPos=0, iYPitchData=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =9000;
    int iMovePitchY =6000;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(Reset)
    {
        return false;
    }

    if(iInArm==0)                                                               //INARM
    {
        iYVariable=GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
        for(int i=0; i<X_PITCH_COUNT; i++)
            iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 (Steven) : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYPitchData=3300;
        }

        if(USE_IN_Y_IS_AUTO_PITCH==true && MACHINE_HAS_AUTO_ALIGNMENT_CCD==true)      //Ztex 2024.02.24 Add HT-1132 HT-1032AT  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            iYPitchData=4000;
        }

        if(ZNumber%2==0)                                                        //吸嘴 A C E G
        {
            iXPos=29659-3000*(ZNumber/2);                                       //Tech.iInArmTeachX-3000*(ZNumber/2);

            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+iInArmAutoYTeachOffset+iYPitchData;                //kevin 20181203//kevin 20171115 add    //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52130+iInArmAutoYTeachOffset;                            //kevin 20171115 add//Tech.iInArmTeachY;

        }
        else
        {
            iXPos=29659-3000*(ZNumber/2);

            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+6000+iInArmAutoYTeachOffset+iYPitchData;           //kevin 20181203//kevin 20171115 add   //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52130+6000+iInArmAutoYTeachOffset;                       //kevin 20171115 add
        }
        TransferHotPlateRatio(false, &iXPos, &iYPos);                           //Steven 20110324

        if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
            return true;
    }
    else
    {
        iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);
        for(int i=0; i<X_PITCH_COUNT; i++)
            iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 (Steven) : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYPitchData=3300;
        }

        if(USE_IN_Y_IS_AUTO_PITCH==true && MACHINE_HAS_AUTO_ALIGNMENT_CCD==true)      //Ztex 2024.02.24 Add HT-1132 HT-1032AT  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            iYPitchData=4000;
        }

        if(ZNumber%2==0)                                                        //吸嘴 A C E G
        {
            iXPos=-20193-3000*(ZNumber/2);
            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+iOutArmAutoYTeachOffset+iYPitchData;               //kevin 20181203 kevin 20171115 add     //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52526+iOutArmAutoYTeachOffset;                           //kevin 20171115 add
        }
        else
        {
            iXPos=-20193-3000*(ZNumber/2);
            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+6000+iOutArmAutoYTeachOffset+iYPitchData;          //kevin 20181203//kevin 20171115 add    //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52526+6000+iOutArmAutoYTeachOffset;                      //kevin 20171115 add
        }
        TransferHotPlateRatio(false, &iXPos, &iYPos);                           //Steven 20110324

        if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, false))
            return true;
    }
    #ifdef SOFT_SIMULTE
        return true;
    #else
        return false;
    #endif
}
//---------------------------------------------------------------------------
//kevin 20171117  Offset Clean
//---------------------------------------------------------------------------
void bOffsetClean()
{
    AnsiString str;
    AnsiString szDir="", szFilePath="";

    szDir=fOffSet->GetOffsetPath();                                             //Steven 20190109 : 整合Offset路徑
    szFilePath=szDir;
    MyForceDirectories(szDir);

    for(int iSelPartData=0; iSelPartData<=13; iSelPartData++)
    {
        szDir=szFilePath;                                                       //kevin 20150105
        if(LastSet.iTemperature==Tempture_Hot && ((iSelPartData==OfsInSh1  || iSelPartData==OfsInSh2 ||
                                                   iSelPartData==OfsOutSh1 || iSelPartData==OfsOutSh2) ||
                                                  (iSelPartData>=OfsInSh1LB && iSelPartData<=OfsInSh2RB)||    //Steven 20140811 : For 32 Site Loader and Shuttle Offset
                                                  (iSelPartData>=OfsOutSh1LB && iSelPartData<=OfsOutSh2RB)))  //Steven 20190802 : For 32 Site Out Shuttle Offset
            szDir+="\\Position Offset Hot.Data";
        else
            szDir+="\\Position Offset.Data";

        WriteIniData(szDir, CapStr[iSelPartData], "PickUp",   0.0);
        WriteIniData(szDir, CapStr[iSelPartData], "Place",    0.0);
        for(int i=0; i<InArmSuck.iMotRow; i++)                                  //8=In/out arm suction
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)                              //8=In/out arm suction
            {
                WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("PickUp %c", 'A'+i+j*2), 0.0);
                WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Place %c", 'A'+i+j*2),  0.0);
            }
        }
        szDir="";
    }
}
//------------------------------------------------------------------------------
void RecordAutoSiteMapStart()                                                   //Steven 20230117 : 修正Auto Site map的訊息
{
    if(IniConfig.bI21EnableASM)
    {
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.SiteMappingStart);                           //51     Site Mapping Start
        RecordProcess("Auto Site Map Start.");                                  //kevin 20160121  add
    }
}
//------------------------------------------------------------------------------
void RecordAutoSiteMapFinish()                                                  //Steven 20230117 : 修正Auto Site map的訊息
{
    if(IniConfig.bI21EnableASM)
    {
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.SiteMappingEnd);                             //52     Site Mapping End
        RecordProcess("Auto Site Map Finish.");                                 //kevin 20160121  add
        bAutoSiteMapHasPickHP=false;                                            //Steven 20230210 : Fixed for auto site map
    }
    else
    {
        RecordProcess("Auto Site Map Disabled.");
        bAutoSiteMapHasPickHP=false;                                            //Steven 20230210 : Fixed for auto site map
    }
}
//------------------------------------------------------------------------------
void DisableAutoSiteMapWhenCleanOut()                                           //Steven 20220902 : 修正Auto Site Map, Loader沒有IC
{
    if(iResetSiteMappingStep==2 && LastSet.iRunStartMode==rsmAutoSiteMap)
    {
        bSiteMappingCHKOK=true;
        SetRunStartMode(rsmContinuStart);

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<HotPlateForm.XDivision; j++)
            {
                for(int k=0; k<HotPlateForm.YDivision; k++)
                {
                    MOT[MMPlate1+i].Tray.SiteMapData[j][k]=0;                   //Ifor 20210913 add: 清除Auto Site Mapping 補回Has Null IC資料
                }
            }
        }
        bAutoSiteMapHotplateSave=false;
        InitInArmTask();

        fMain->ShowTestHeadComp(false);
    }
}
//------------------------------------------------------------------------------
//Ifor 20170928 (Steven) : Check Auto Site Mapping Position
//------------------------------------------------------------------------------
void DoCheckAutoSiteMappingPosition()
{
    if(bRunAutoSiteMapping==true &&
       bAutoSiteMapHotplateSave==true)                                          //Ifor 20180518 : add 簡化Site Mapping 旗標
    {
        bAutoSiteMapHotplateSave=false;
        if(bAutoSiteMapHotplateReady==true &&
           bSiteMappingCHKOK==false        )
        {
            if(CosFunction.bUSEJCETSiteMapMode==true ||                         //Ifor 20171013 (wei) : 長電Auto Site Mapping 需Pass才可測試下一顆
               CUSTOMER_CODE==CC_ASE_M)                                         //Ifor 20180417: add ASE_MAuto Site Mapping 需Pass才可測試下一顆
            {
                if(bAutoSiteMapWaitTestPass==true)
                    iDoSiteMappingStep++;
            }
            else
            {
                iDoSiteMappingStep++;
            }
            DoSiteMappingCHK();
        }
    }
}
//------------------------------------------------------------------------------
//wei 20170302 (Steven) Tray map data error
//Tray map data error需放回去
//------------------------------------------------------------------------------
int iProcessTrayMapDataErrorTask;
bool ProcessTrayMapDataError(bool bReset)
{
    AnsiString ErrPart="";
    bool bResult=false;
    if(bReset)
    {
        iProcessTrayMapDataErrorTask=1;
        bCheckTrayDevice=true;                                                  //wei 20170302 (Steven) Tray map data error
        return bResult;
    }

    int &Task=iProcessTrayMapDataErrorTask;
    switch(Task)
    {
        case 1:
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(bTrayMapDataError[i][j])
                    {
                        InArmSuck.Suck[i][j].Destroy();
                    }
                }
            }
            InArmReleaseDelay.SetSecAndOn(0.5);
            Task=100;
            break;
        case 100:
            if(InArmReleaseDelay.Off())
            {
                Task=150;
            }
            break;
        case 150:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=200;
            }
            break;
        case 200:
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(bTrayMapDataError[i][j])
                    {
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                    }
                }
            }
            ShowErrorMessage("WAR0122", K_SKIP, MInArmX, false, ErrPart);       //wei 20170317 (Steven) 因重覆所以修改 WAR0121 --> WAR0122

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(iTrayMapDataX[i][j]!=-1 && iTrayMapDataY[i][j]!=-1)
                    {
                        if(bTrayMapDataError[i][j])
                        {
                            MOT[MMTrayY].SetTraySingleData(iTrayMapDataX[i][j], iTrayMapDataY[i][j], NULL_IC);
                        }
                        bTrayMapDataError[i][j]=false;
                        iTrayMapDataX[i][j]=-1;
                        iTrayMapDataY[i][j]=-1;
                    }
                }
            }

            bResult=true;
            break;
    }
    return bResult;
}
//------------------------------------------------------------------------------
int LoadTrayCanUse8Suck()                                                       //kevin 20171120 (wei) 判斷LOAD TRAY Y PITCH是否是 ARM Y PITCH可以一次8吸嘴吸IC
{
    int Ld=TrayForm.Loader.iTrayType;
    int TrayYPitch=UserDefForm[Ld].YPitch;
    int iPitchY=TestIF.iARM_Y_PITCH%TrayYPitch;

    if(iPitchY==0)
    {
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
void InitInArmZCheckPos()
{
    iInArmZCheckPosTask=1;
}
//------------------------------------------------------------------------------
void InArmAddSpeedDisplay()                                                     //KaiChen 20171225 (Steven)：Add Speed Display
{
    iInArmSpeed1++;
}
//------------------------------------------------------------------------------
void InArmSubSpeedDisplay()                                                     //KaiChen 20171225 (Steven)：Add Speed Display
{
    iInArmSpeed1--;
}
//------------------------------------------------------------------------------
int iOneKitPreciser=0;
bool bOneKitPreciser=false;
void InitInArmDevicePosPrecise()                                                //Frank 20180410 (Steven) : InArm Preciser Station
{
    iInArmDevicePosPrecise=1;
}
//------------------------------------------------------------------------------
HTimer DoInArmDevicePosPreciseDelay;
int  iPrecise_XP[X_PITCH_COUNT];
int  iPrecise_YP=0;
int  iPrecise_MoveXP=12000;
int  iPrecise_MoveYP=6000;
int  iPrecise_Z[MAX_ARM_Row][MAX_ARM_Col];
bool bPrecise_XYZ=false;
bool bPreciseFlag=false;
bool bPrecise_Z[MAX_ARM_Row][MAX_ARM_Col];
bool bPrecise_SuckFlag[MAX_ARM_Row][MAX_ARM_Col];
//------------------------------------------------------------------------------
int iInitialFlag    =0;
int iMoveDownPlace  =1;
int iMoveDownAdd200 =2;
int iMoveDownPick   =3;
void SetPrecisorZPos(int iAction)                                               //Steven 20241009 : 重新整理Precisor Function
{
//    SetPrecisorZPos(iInitialFlag);

    if(iAction==iMoveDownPlace ||
       iAction==iMoveDownPick)
    {
        if(bOneKitPreciser==true)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(InArmSuck.Item[iOneKitPreciser][j]!=NULL_IC &&
                   InArmSuck.Item[iOneKitPreciser][j]!=HAS_NULL_IC)
                {
                    bPrecise_Z[iOneKitPreciser][j]=true;
                    if(iAction==iMoveDownPick)
                        iPrecise_Z[iOneKitPreciser][j]=Prod.iInArmPreciserPickUpZ[iOneKitPreciser][j];
                    else
                        iPrecise_Z[iOneKitPreciser][j]=Prod.iInArmPreciserPlaceZ[iOneKitPreciser][j];
                }
            }
        }
        else
        {
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Item[j][j]!=HAS_NULL_IC)
                    {
                        bPrecise_Z[i][j]=true;
                        if(iAction==iMoveDownPick)
                            iPrecise_Z[i][j]=Prod.iInArmPreciserPickUpZ[i][j];
                        else
                            iPrecise_Z[i][j]=Prod.iInArmPreciserPlaceZ[i][j];
                    }
                }
            }
        }
    }
    else if(iAction==iMoveDownAdd200)
    {
        if(bOneKitPreciser==true)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(InArmSuck.Item[iOneKitPreciser][j]!=NULL_IC &&
                   InArmSuck.Item[iOneKitPreciser][j]!=HAS_NULL_IC)
                {
                    bPrecise_Z[iOneKitPreciser][j]=true;
                    iPrecise_Z[iOneKitPreciser][j]=Prod.iInArmPreciserPickUpZ[iOneKitPreciser][j]+200;
                }
            }
        }
        else
        {
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Item[j][j]!=HAS_NULL_IC)
                    {
                        bPrecise_Z[i][j]=true;
                        iPrecise_Z[i][j]=Prod.iInArmPreciserPlaceZ[i][j]+200;
                    }
                }
            }
        }
    }
    else //if(iAction==iInitialFlag)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                bPrecise_Z[i][j]=false;
                iPrecise_Z[i][j]=ZSafePos;
            }
        }
    }
}
//------------------------------------------------------------------------------
bool DoInArmDevicePosPrecise()                                                  //Frank 20180410 (Steven) : InArm Preciser Station
{                                                                               //Steven 20241009 : 重新整理Precisor Function
    int &Task=iInArmDevicePosPrecise;

    static bool bDestroyflag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    double dOffset=0, dTrayTorence=0;
    int iXPos=0, iYPos=0;
    bool flag;

    bWaitPreciserFinish=true;                                                   //Ifor 20180703 (Steven) : : 等待 Preciser 完成
    switch(Task)
    {
        case 1:                                                                 //決定哪幾隻要下去做Precise
            if(TestIF_File.dPreciserYPitch==0 &&
               InArmSuck.iPickRow==2)
                bOneKitPreciser=true;
            else
                bOneKitPreciser=false;

            iOneKitPreciser=0;
            Task=100;
            break;
        case 80:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserOpenPitch);
                if(bPreciseFlag)
                    Task=100;
            }
            break;
        case 100:                                                               //Z軸回Safe位置
            if(MoveInArmZToPlateSafe(Task))
            {
                bPreciseFlag=false;
                bPrecise_XYZ=false;
                if(USE_PRECISER==2)                                             //Steven 20191113 : 使用HP當Preciser
                    Task=200;
                else if(Sen[SnPreciserDetect1].Enable==true ||
                        Sen[SnPreciserDetect2].Enable==true)
                    Task=150;
                else
                    Task=200;
            }
            break;
        case 150:
            if((Sen[SnPreciserDetect1].Enable && Sen[SnPreciserDetect1].IsOn()==false) ||
               (Sen[SnPreciserDetect2].Enable && Sen[SnPreciserDetect2].IsOn()==false))
            {
                ShowMyMessage("請將Preciser內IC移除");
            }
            else
            {
                bPreciseFlag=false;
                bPrecise_XYZ=false;
                Task=200;
            }
            break;
        case 200:                                                               //移動至Precise 位置  Precise打開 X Pitch Y Pitch 開合完成
            iXPos=Prod.iInArmPreciserX+InArmOffSet[InOfsPreciser]->GetX();
            iYPos=Prod.iInArmPreciserY+InArmOffSet[InOfsPreciser]->GetY();

            iPrecise_MoveYP=TestIF_File.iARM_Y_PITCH;
            if(bOneKitPreciser && iOneKitPreciser==1)
                iYPos+=(TestIF_File.dPreciserYPitch*100);

            if(USE_IN_Y_IS_AUTO_PITCH==true)                                          //Steven 20131002 : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                iYPos=(iYPos-TestIF_File.iARM_Y_PITCH);
            }

            PreciserPitchCalculate(iPrecise_MoveXP);
            for(int i=0; i<X_PITCH_COUNT; i++)
                iPrecise_XP[i]=GetInArmPitchX_9045(iPrecise_MoveXP, i, InOfsPreciser);
            iPrecise_YP=GetInArmPitchY_9045(iPrecise_MoveYP, InOfsPreciser);

            SetPrecisorZPos(iInitialFlag);

            if(USE_PRECISER==2)                                                 //Steven 20191113 : 使用HP當Preciser
                bPreciseFlag=true;
            else if(bPreciseFlag==false)
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserOpenPitch);

            if(bPrecise_XYZ==false)
                bPrecise_XYZ=InArmContinuousMove_9045(iXPos, iYPos, iPrecise_XP, iPrecise_YP, bPrecise_Z, iPrecise_Z, false);

            if(bPrecise_XYZ && bPreciseFlag)
            {
                Task=300;
            }
            break;
        case 300:                                                               //使用的Z軸下降
            SetPrecisorZPos(iMoveDownPlace);

            bPrecise_XYZ=InArmZMoveDown(bPrecise_Z, iPrecise_Z, false);
            if(bPrecise_XYZ==true)
            {
                Task=400;
            }
            break;
        case 400:                                                               //Precise 夾持
            if(USE_PRECISER==2)                                                 //Steven 20191113 : 使用HP當Preciser
            {
                bPreciseFlag=true;
            }
            else
            {
//              0.2mm/2.44mm=x/(Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)   //0.2mm為20條 = Tray盤Torence
                dTrayTorence=((Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)*0.2)/2.44;
                dOffset     =(Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)*(Offset.iPreciserOpen/244);
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserClosePitch+dTrayTorence+dOffset);
            }

            if(bPreciseFlag)
            {
                if(AccessLevel>=1 && Sen[SnRKManualTStart].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoInArmDevicePosPrecise");
                    Task=420;
                    break;
                }

                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                Task=1000;
            }
            break;
        case 420:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=1;
            }
            else
            {
                Task=400;
            }
            break;
        case 1000:
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    bDestroyflag[i][j]=!bPrecise_Z[i][j];
                }
            }
            Task=1050;
        case 1050:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                flag=true;
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if((bOneKitPreciser==false) ||
                           (bOneKitPreciser==true && i==iOneKitPreciser))
                        {
                            if(bDestroyflag[i][j]==false)
                            {
                                if(InArmSuck.Item[i][j]!=NULL_IC &&
                                   InArmSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    if(InArmSuck.Suck[i][j].Destroy())
                                    {
                                        bDestroyflag[i][j]=true;
                                    }
                                    else if(InArmSuck.Suck[i][j].Error==false)
                                    {
                                        flag=false;
                                    }
                                }
                                else
                                {
                                    bDestroyflag[i][j]=true;
                                }
                            }
                        }
                    }
                }

                if(flag==false)
                    break;
                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                Task=1100;
            }
            break;
        case 1100:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                SetPrecisorZPos(iMoveDownAdd200);
                bPrecise_XYZ=InArmZMoveDown(bPrecise_Z, iPrecise_Z, false);

                if(bPrecise_XYZ==true)
                {
                    for(int i=0; i<InArmSuck.iMaxRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            if(bPrecise_Z[i][j]==true)
                                InArmSuck.Suck[i][j].Normal();
                        }
                    }

                    Task=1150;

                }
            }
            break;
        case 1150:                                                              //Precise 夾持
            if(USE_PRECISER==2)                                                 //Steven 20191113 : 使用HP當Preciser
            {

                bPreciseFlag=true;
            }
            else
            {
                dOffset     =(Prod.iPreciserOpenPitch-Prod.iPreciserClosePitch)*(Offset.iPreciserClose/244);
                bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserClosePitch+dOffset);
            }

            if(bPreciseFlag)
            {
                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                bPreciseFlag=false;
                Task=1200;
            }
            break;
        case 1200:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                Task=1300;
            }
            break;
        case 1300:
            SetPrecisorZPos(iMoveDownPick);
            bPrecise_XYZ=InArmZMoveDown(bPrecise_Z, iPrecise_Z, false);

            if(bPrecise_XYZ==true)
            {
                InArmSuck.ClearAllError();                                      //Steven 20141107 : Add for Loader pickup Error
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        bDestroyflag[i][j]=!bPrecise_Z[i][j];
                    }
                }

                Task=1310;
            }
            break;
        case 1310:
            flag=true;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if((bOneKitPreciser==false) ||
                       (bOneKitPreciser==true && i==iOneKitPreciser))
                    {
                        if(bDestroyflag[i][j]==false)
                        {
                            if(InArmSuck.Item[i][j]!=NULL_IC &&
                               InArmSuck.Item[i][j]!=HAS_NULL_IC)
                            {
                                if(InArmSuck.Suck[i][j].Suck())
                                {
                                    bDestroyflag[i][j]=true;
                                }
                                else if(InArmSuck.Suck[i][j].Error==true)
                                {
                                    bDestroyflag[i][j]=true;
                                }
                            }
                            else
                            {
                                bDestroyflag[i][j]=true;
                            }
                        }
                    }
                }
            }

            if(flag==false)
                break;

            DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
            bPreciseFlag=false;

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Suck[i][j].Error)
                    {
                        Task=1400;
                    }
                }
            }
            break;
        case 1400:                                                              //Precise 打開
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                if(USE_PRECISER==2)                                             //Steven 20191113 : 使用HP當Preciser
                    bPreciseFlag=true;
                else
                    bPreciseFlag=MOT[MPreciser].MotorMove(Prod.iPreciserOpenPitch);

                if(bPreciseFlag)
                {
                    bPreciseFlag=false;
                    Task=1500;
                }
            }
            break;
        case 1500:
            if(MoveInArmZToPlateSafe(Task))
            {
                DoInArmDevicePosPreciseDelay.SetMSAndOn(100);
                CheckInArmSuckICFallDownToHasNullIC();
                bPreciseFlag=false;
                if(bOneKitPreciser==true)
                {
                    if(iOneKitPreciser==0)
                    {
                        iOneKitPreciser=1;
                        Task=80;
                    }
                    else
                    {
                        Task=2000;
                    }
                }
                else
                {
                    Task=2000;
                }
            }
            break;
        case 2000:
            if(DoInArmDevicePosPreciseDelay.Off())
            {
                bWaitPreciserFinish=false;                                      //Ifor 20180703 : 等待 Preciser 完成
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void PreciserPitchCalculate(int &iXPitch)
{
    int iPreciserXPitch=TestIF_File.dPreciserXPitch*100;

    iXPitch=iPreciserXPitch;
    if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==_8Site2X4 ||
       TestIF.iTestMode==_16Site2X8  || TestIF.iTestMode==_12Site2X6 ||
       TestIF.iTestMode==TriSite1X3  ||                                         //Frank 20160329 add for 1x3_4
       TestIF.iTestMode==_6Site2X3   ||                                         //ChungHung 20140115 add for 2x3
       TestIF.iTestMode==_8Site1X4   ||                                         //ChungHung 20150528 add for 海思 _8Site1x4
       TestIF.iTestMode==_16Site4X4  ||                                         //Sam 20190226 : 16Site4X4
       TestIF.iTestMode==_32Site4X8N)                                           //2013-01-15    Dell    Add nn Mode
    {
        if(TestIF_File.dPreciserXPitch<41.0)                                    //Steven 20210412 : 修正Preciser的Pitch錯誤問題
            iXPitch=iPreciserXPitch*3;
        else
            iXPitch=iPreciserXPitch+iPreciserXPitch/2;
    }
    else if(TestIF.iTestMode==DualSite2x1)
    {
//        return false;
    }
    else if(TestIF.iTestMode==QualSite2X2)
    {
        if(TestIF_File.dPreciserXPitch<41.0)                                    //Steven 20210412 : 修正Preciser的Pitch錯誤問題
            iXPitch=iPreciserXPitch*3;
    }
    else if(TestIF.iTestMode==DualSite)                                         //Steven 20150505 : 1x2加大支援X-Pitch 120mm
    {
        if(iPreciserXPitch>8000)
            iXPitch=12000;
        else
            iXPitch=iPreciserXPitch*1.5;
    }
}
//------------------------------------------------------------------------------
int iVibrateShuttleTask;
bool DoVibrateShuttle(int iShuttle, bool bNeedInitial)                          //JerryYang 20190123 shuttle震動馬達
{
    static bool bInitial=true;
    if(bInitial || bNeedInitial)
    {
        iVibrateShuttleTask=1;
        bInitial=false;

        if(CUSTOMER_CODE==CC_KYEC_XILINX)
            RecordProcess("Vibrate Shuttle Initial");
        return false;
    }
    AnsiString str;
    int iShuttleVibration=0;
    static int iShuttleVibraCount=0;
    if(iShuttle==0)
        iShuttleVibration=SwShuttleVibration1;
    else
        iShuttleVibration=SwShuttleVibration2;
    switch(iVibrateShuttleTask)
    {
        case 1:
            SW[iShuttleVibration].On();
            VibrationShuttleDelay.SetMSAndOn(IniConfig.iF23ShuttleVibrationTime*100);
            if(iShuttle==0)                                                     //JerryYang 20200612 振動馬達作動時間累計
            {
                IniConfig.iVibratorSht1=IniConfig.iVibratorSht1+int(IniConfig.iF23ShuttleVibrationTime/10.0);
            }
            else
            {
                IniConfig.iVibratorSht2=IniConfig.iVibratorSht2+int(IniConfig.iF23ShuttleVibrationTime/10.0);
            }
            iVibrateShuttleTask=100;
            iShuttleVibraCount++;
            break;
        case 100:
            if(VibrationShuttleDelay.Off())                                     //JerryYang 20171205 (Steven) shuttle震動馬達功能
            {
                SW[iShuttleVibration].Off();
                iVibrateShuttleTask=200;
                if(iShuttleVibraCount<IniConfig.iF23ShuttleVibrationCount)
                {
                    VibrationShuttleDelay.SetMSAndOn(1000);                     //Steven 20240427 : 移到裡面
                    str=IntToStr(iShuttleVibraCount);                           //JerryYang 20191111 shuttle振動馬達可以多震幾次
                    NewRecordProcess("MES2155", "Shuttle count", str);          //ChungHung 20140722 add add record
                    iVibrateShuttleTask=150;
                }
            }
            break;
        case 150:
            if(VibrationShuttleDelay.Off())                                     //JerryYang 20171205 (Steven) shuttle震動馬達功能
            {
                iVibrateShuttleTask=1;
            }
            break;
        case 200:
            if(VibrationShuttleDelay.Off())                                     //JerryYang 20171212 (Steven) 關閉震動後先delay 1秒,避免shuttle馬上移動IC會飛走
            {
                iShuttleLog=1;                                                  //JerryYang 20181121 (Steven) : add log,記錄in shuttle device是被震動馬達or搖搖功能導正
                iShuttleVibraCount=0;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iVibrateOutShuttleTask[2];
bool DoVibrateOutShuttle(int iShuttle, bool bNeedInitial)                       //JerryYang 20190123 shuttle震動馬達
{
    static bool bInitial=true;
    if(bInitial || bNeedInitial)
    {
        iVibrateOutShuttleTask[iShuttle]=1;
        bInitial=false;
        return false;
    }
    AnsiString str;
    int iShuttleVibration=0;
    static int iShuttleVibraCount=0;
    if(iShuttle==0)
        iShuttleVibration=SwShuttleVibration1;
    else
        iShuttleVibration=SwShuttleVibration2;
    switch(iVibrateOutShuttleTask[iShuttle])
    {
        case 1:
            SW[iShuttleVibration].On();
            VibrationOutShuttleDelay[iShuttle].SetMSAndOn(IniConfig.iF25VibrateTime*100);
            if(iShuttle==0)                                                     //JerryYang 20200612 振動馬達作動時間累計
            {
                IniConfig.iVibratorSht1=IniConfig.iVibratorSht1+int(IniConfig.iF25VibrateTime/10.0);
            }
            else
            {
                IniConfig.iVibratorSht2=IniConfig.iVibratorSht2+int(IniConfig.iF25VibrateTime/10.0);
            }
            iVibrateOutShuttleTask[iShuttle]=100;
            iShuttleVibraCount++;
            break;
        case 100:
            if(VibrationOutShuttleDelay[iShuttle].Off())                        //JerryYang 20171205 (Steven) shuttle震動馬達功能
            {
                SW[iShuttleVibration].Off();
                iVibrateOutShuttleTask[iShuttle]=200;
                VibrationOutShuttleDelay[iShuttle].SetMSAndOn(1000);
            }
            break;
        case 150:
            if(VibrationOutShuttleDelay[iShuttle].Off())                        //JerryYang 20171205 (Steven) shuttle震動馬達功能
            {
                iVibrateOutShuttleTask[iShuttle]=1;
            }
            break;
        case 200:
            if(VibrationOutShuttleDelay[iShuttle].Off())                        //JerryYang 20171212 (Steven) 關閉震動後先delay 1秒,避免shuttle馬上移動IC會飛走
            {
                iShuttleVibraCount=0;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool EnableTraymapCheckFunction(int iCheck)
{
    bool Result=false;

    if(iCheck==0)
    {
        if(bRTNoRunTrayID==false &&
           USE_TRAY_MAPPING!=etmUninstall &&
           (TestIF_File.bEnableTrayMap || (TestIF_File.bEnableTrayID && TestIF_File.bEnableTrayIDDownFTP)) &&
           TestIF_File.bEnableSuckMapCheck)                                     //wei 20170302 (Steven) Tray map data error
        {
            Result=true;
        }
    }
    else if(iCheck==1)
    {
        if(bRTNoRunTrayID==false &&
           USE_TRAY_MAPPING!=etmUninstall &&
           (TestIF_File.bEnableTrayMap || (TestIF_File.bEnableTrayID && TestIF_File.bEnableTrayIDDownFTP)) &&
           bTrayMapDataCheckErrorSkip)                                          //wei 20170302 Tray map data error
        {
            Result=true;
        }
    }
    return Result;
}
//------------------------------------------------------------------------------
void CheckTrayMapData(int iTrayRow, int iTrayCol)
{
    if(EnableTraymapCheckFunction(0))                                           //wei 20170302 (Steven) Tray map data error
    {
        if(MyDeCodeASCII(fTrayMapping->iTrayMappingDate[iTrayCol][iTrayRow])!=1)
        {
            if(TestIF_File.bEnableTrayMapDoubleCheck)                           //wei 20190506
            {
                if(MyDeCodeASCII(fTrayMapping->iTrayMappingDateCheck[iTrayCol][iTrayRow])!=1)
                    bTrayMapDataCheckErrorSkip=true;
            }
            else
            {
                bTrayMapDataCheckErrorSkip=true;
            }
        }
    }
}
//---------------------------------------------------------------------------
void RecordShtSuperflous(int iShuttle)                                          //JerryYang 20181121 (Steven) : add log,記錄in shuttle device是被震動馬達or搖搖功能導正
{
    if(IniConfig.bKoreaFunction)                                                //Steven 20230825 : 韓國客戶說不想看到這個log
        return;
    if(iShuttle==0)
    {
        if(iShuttleLog==1)
        {
            NewRecordProcess("MES0401", "In Shuttle 1 Device Un-superfluous By Vibration!");
        }
        else if(iShuttleLog==2)
        {
            NewRecordProcess("MES0404", "In Shuttle 1 Device Un-superfluous By Shaking!");
        }
    }
    else if(iShuttle==1)
    {
        if(iShuttleLog==1)
        {
            NewRecordProcess("MES0402", "In Shuttle 2 Device Unsuperfluous By Vibration!");
        }
        else if(iShuttleLog==2)
        {
            NewRecordProcess("MES0405", "In Shuttle 2 Device Unsuperfluous By Shaking!");
        }
    }
    iShuttleLog=0;
}
//---------------------------------------------------------------------------
void RestoreLoadeIC(int iLoaderY)                                               //JerryYang 20200422 Auto skip次數到達後, 自動再去最後一排吸吸看
{
    if(IniConfig.bE62TryPickLastRow && bTryPickLastRowIng==true)
    {
        if(TrayForm.Loader.Direction==0 || TrayForm.Loader.Direction==1)
        {
            if(iLoaderY==MOT[MMTrayY].Tray.YItem-1)
            {
                bTryPickLastRowIng=false;
                if(TrayForm.Loader.Direction==0)
                {
                    TrayForm.Loader.Direction=3;
                }
                else if(TrayForm.Loader.Direction==1)
                {
                    TrayForm.Loader.Direction=2;
                }

                for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                {
                    for(int j=0; j<MOT[MMTrayY].Tray.YItem-1; j++)
                    {
                        if(MOT[MMTrayY].Tray.BufferData[i][j]==HAS_IC)
                        {
                            if(MOT[MMTrayY].Tray.Data[i][j]==NULL_IC)
                            {
                                MOT[MMTrayY].SetTraySingleData(i, j, HAS_IC);
                            }
                        }
                    }
                }
            }
            else
            {

            }
        }
        else
        {
            if(iLoaderY==0)
            {
                bTryPickLastRowIng=false;
                if(TrayForm.Loader.Direction==2)
                {
                    TrayForm.Loader.Direction=1;
                }
                else if(TrayForm.Loader.Direction==3)
                {
                    TrayForm.Loader.Direction=0;
                }
                for(int i=0; i<MOT[MMTrayY].Tray.XItem; i++)
                {
                    for(int j=1; j<MOT[MMTrayY].Tray.YItem; j++)
                    {
                        if(MOT[MMTrayY].Tray.BufferData[i][j]==HAS_IC)
                        {
                            if(MOT[MMTrayY].Tray.Data[i][j]==NULL_IC)
                            {
                                MOT[MMTrayY].SetTraySingleData(i, j, HAS_IC);
                            }
                        }
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------------------
void VacuumOnOffLog(AnsiString sLog)
{
    AnsiString s;
    AnsiString Path, buffer;

    s.sprintf("%s", sLog);
    Path.sprintf("%s\\%04d_%02d_%02d", asVacuumOnOffLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);

    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asVacuumOnOffLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    if(FileExists(Path)==false)
    {
       buffer.sprintf("Time,Use Site,Test Time, Vacuum On,Vacuum Off");
       WriteDataToFile(Path, buffer);
    }

    WriteDataToFile(Path, s);
}
//------------------------------------------------------------------------------
bool MoveInArm2XYToClean()
{
    int iXVariable[X_PITCH_COUNT];
    int iYVariable =0;
    int iMovePitchX=12000;
    int iMovePitchY=6000;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            return false;
        }
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, InOfsRotate_In);
    iYVariable=GetInArmPitchY_9045(iMovePitchY, InOfsRotate_In);

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            bZFlag[i][j]=true;
            if(CUSTOMER_CODE==CC_TSMC_TAINAN)
                iZPos[i][j]=-(ArmSpeed_File[InArm].dDieCleanHeight*100);
            else
                iZPos[i][j]=0+(ArmSpeed_File[InArm].dDieCleanHeight*100);
        }
    }

    if(InArmContinuousMove_9045(Prod.iInArm_RotateX+InArmOffSet[InOfsRotate_In]->GetX(), Prod.iInArm_RotateY+InArmOffSet[InOfsRotate_In]->GetY(), iXVariable, iYVariable, bZFlag, iZPos, ZAxisDown))
    {
        return true;
    }
    return false;
}
//==============================================================================
// InArm Die Clean 主流程
//==============================================================================
int  InDieCleanTask=1;
HTimer DoInDieCleanDelay;
//------------------------------------------------------------------------------
void InitInDieCleanTask()                                                       //wei 20170418
{
    InDieCleanTask=1;
}
//------------------------------------------------------------------------------
bool DoInDieClean()                                                             //wei 20170418
{
    int &Task=InDieCleanTask;
    bool Result=false;

    switch(Task)
    {
        case 1:                                                                 // z safe
            if(MoveInArmZToPlateSafe(199))
            {
                Task=100;
            }
            break;
        case 100:                                                               //xy move clean pos
            if(MoveInArm2XYToClean())
            {
                if(AccessLevel>=1 && Sen[SnRKManualTStart].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoInDieClean");
                    Task=120;
                    break;
                }
                DoInDieCleanDelay.SetMSAndOn(500);
                Task=200;
            }
            break;
        case 120:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=1;
            }
            else
            {
                Task=100;
            }
            break;
        case 200:                                                               //到位延遲
            if(DoInDieCleanDelay.Off())
            {
                Task=300;
            }
            break;
        case 300:                                                               //開吹氣
            SW[SwDieClean].On();                                                //wei 20170525
            SW[SwDieCleanSuck].On();                                            //wei 20210326
            DoInDieCleanDelay.SetMSAndOn(ArmSpeed_File[InArm].dDieCleanDelay*1000);
            Task=400;
            break;
        case 400:                                                               //吹氣延遲
            if(DoInDieCleanDelay.Off())
            {
                Task=500;
            }
            break;
        case 500:
            if(MoveInArmZToPlateSafe(599))
            {
                Task=600;
            }
            break;
        case 600:                                                               //關吹氣
            SW[SwDieClean].Off();                                               //wei 20170525
            SW[SwDieCleanSuck].Off();                                           //wei 20210326
            Result=true;
            break;
    }

    return Result;
}
//------------------------------------------------------------------------------
bool bNeedPickupErrorICToRecycleBin()                                           //JimmyChiu 20220908 add Pickup Error Placement
{
    if(fMain->cInplace->InArmPlacementEnable()==false)
    {
        return false;
    }
    static bool bNeedRunRecycle=false;
    AnsiString asErrorPickup=AnsiString("");
    if(bNeedRunRecycle)
    {
        if(fMain->cInplace->DoSuckToRecycleFromLoader(false,asErrorPickup))
        {
            bNeedRunRecycle=false;
        }
    }
    else
    {
        bNeedRunRecycle=MOT[MMTrayY].HasOnlyDataICAndNullIC(HAS_SKIP_IC);
        if(bNeedRunRecycle)
        {
            fMain->cInplace->DoSuckToRecycleFromLoader(true,asErrorPickup);
        }
    }
    return bNeedRunRecycle;
}
//------------------------------------------------------------------------------
void SetInArmNeedDestory(bool bPlace, int iShtRow, int iShtCol, int iRow, int iCol)
{
    if(bPlace==true)
    {
        if(InArmSuck.Item[iRow][iCol]!=NULL_IC)
            InArmSuck.Suck[iRow][iCol].SetNeedDestroy(true);
    }
    else
    {
        InArmSuck.iNeedSuck[iRow][iCol]=ptrInSHT->Item[iShtRow][iShtCol];
    }
}
//------------------------------------------------------------------------------
void SetShuttlefCanMoveL(int iShuttle, bool bCanMoveL, AnsiString sFun, AnsiString sTask)
{
    AnsiString Str="", sflag="";
    static AnsiString sOldStr="";
    sflag=(bCanMoveL)?"true":"false";
    if(MOT[MInShuttle1+iShuttle].fCanMoveL!=bCanMoveL)
    {
        MOT[MInShuttle1+iShuttle].fCanMoveL=bCanMoveL;
        Str.sprintf("Sht%d CanMove = %s, %s-%s", iShuttle+1, sflag, sFun, sTask);
        sOldStr=Str;
    }
    else
    {
        Str.sprintf("Sht%d CanMove = %s, %s-%s", iShuttle+1, sflag, sFun, sTask);   //Sam 20250327 : 新增Log
        if(sOldStr!=Str)
        {
            sOldStr=Str;
        }
        else
        {
            return;
        }
    }

    #ifdef DEBUG_ShuttleCanMoveL
    AnsiString asLogPath, Log="", Path="";
    asLogPath.sprintf("D:\\HT9045_Log\\ShuttleCanMoveL\\%04d_%02d_%02d", SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(asLogPath, "SetShuttlefCanMoveL");

    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Str);
    Path.sprintf("%s\\%04d_%02d_%02d_%02d.txt", asLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);

    WriteDataToFile(Path, Log);
    #endif
}
//==============================================================================
// OCR InArm to Wait 主流程
//==============================================================================
int  iOCRWaitTask=1;
//------------------------------------------------------------------------------
void InitInOCRWaitTask()                                                        //wei 20170901
{
    iOCRWaitTask=1;
}
//------------------------------------------------------------------------------
bool OCRMoveInArm2XYToWait()                                                    //wei 20170901
{
    int &Task=iOCRWaitTask;
    bool Result=false;

    switch(Task)
    {
        case 1:
            if(MoveInArmZToPlateSafe(123))
            {
                Task=100;
            }
            break;
        case 100:
            if(MoveInArm2XYToWait())
            {
                Task=200;
            }
            break;
        case 200:
            if(NeedWaitTrayArm==false)
            {
                Result=true;
                Task=1;
            }
            break;
    }

    return Result;
}
//------------------------------------------------------------------------------
