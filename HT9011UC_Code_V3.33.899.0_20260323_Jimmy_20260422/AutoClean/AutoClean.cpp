#include "MachineDefine.h"
#pragma hdrstop

#include "AutoClean.h"
#include "cCleanKitPickPlan.h"

#include "aArmHeader.h"
#include "mymotor.h"
#include "cprod.h"
#include "cmydef.h"
#include "note.h"
#include "main.h"
#include "mysensor.h"
#include "csystem.h"
#include "uhome.h"
#include "mymessbox.h"
#include "cinitial.h"
#include "acatchtray.h"
#include "adam6024.h"
#include "atester.h"
#include "MyKitSuck.h"
#include "uShowMessage.h"
#include "uCleaning.h"
#include "iosetview.h"
#include "acarry.h"
#include "cMyDB.h"
#include "cShowBinSelect.h"
#include "LtcSensor.h"
#include "uHGemHT9045.h"
#include "FTPClient.h"
#include "cContact.h"
#include "rs232.h"
#include "cUnitConvert.h"
#include "cContactCT.h"

#include "common.h"
#include "mycylin.h"

//------------------------------------------------------------------------------
#pragma package(smart_init)
//------------------------------------------------------------------------------

extern bool IndexZCanMove[2];
extern bool MNetLog(AnsiString Message);
int iAutoCleanPickFromCleanKitStageTask=1;                                      //jou 2012-05-22
int iAutoCleanPlaceToShuttleTask=1;
int iInXPos;
int iInYPos;
bool bFullViewCheckFinish=false;                                                //JerryYang 20160331
int SHT_Kit=0;
bool bInedxCleanFinish[2]={false,false};                                        //kevin 20170520 (wei) Autoclean index做完
TQPF_Timer DoTestYRearDelayAC;
extern void DoInArm_9045_Type();                                                //Steven 20201014 : 將DoInArm_9045_Type獨立,避免DoInArm_9045還沒執行就被使用
extern int GetNowSiteKitMode_All_1Pick(int iSht, bool bPlace);
//------------------------------------------------------------------------------
void CleanOnlyHasNullInShuttle()                                                //jimmychiu 20220624 add
{
    bool bflag=true;
    for(int i=0; i<InArmSuck.iShtRow; i++)
    {
        for(int j=0; j<InArmSuck.iShtCol; j++)
        {
            if(BLCarryKit.Item[i][j]==HAS_NULL_CLEAN_IC ||
               BLCarryKit.Item[i][j]==NULL_IC)
            {
            }
            else
            {
                bflag=false;
                break;
            }
        }
    }
    if(bflag)
    {
        BLCarryKit.SetAllToNullIC();
    }
}
//------------------------------------------------------------------------------
void InitialSet()
{
    CleanSetSpeed(true);                                                        //kevin 20141228 setup Autoclean馬達速度

    HotplatlXOffset             =TestIF_File.HotplatlXOffset;                   //kevin 20150209 add hotplate Xpos
    HotplatlYOffset             =TestIF_File.HotplatlYOffset;                   //kevin 20150209 add hotplate Ypos
    HotplatlPickOffset          =TestIF_File.HotplatlPickOffset;                //kevin 20150209 add hotplate Pick
    HotplatlPlaceOffset         =TestIF_File.HotplatlPlaceOffset;               //kevin 20150209 add hotplate Place

    iArmPickTrayPos             =TestIF_File.iPadThickness+HotplatlPickOffset;  //kevin 20120623 In out arm 吸取TRAY IC Offset
    iArmPlaceTrayPos            =TestIF_File.iPadThickness+HotplatlPlaceOffset; //kevin 20120623 In out arm 放TRAY IC Offset
    HotplatePitchOffset         =TestIF_File.HotplatlPitchOffset;               //kevin 20150209 add hotplate Pitch

    if(bUse_NewAutoCleanForm==1)                                                //kevin 20150720 使用 哪一arm clean
    {
       bUseCleanArm=TestIF_File.iAutoClean_SelectArm;
    }

    if(bUseCleanArm==0)                                                         //kevin 20140903 使用ARM1
    {
        iIndexPickShuttlePos    =Prod.TestZ1_Pick+TestIF_File.iPadThickness;    //kevin 20120623 Index 吸取 Shuttle IC Offset
        iIndexPlaceShuttlePos   =Prod.TestZ1_Place+TestIF_File.iPadThickness;   //TestIF_File.iAutoClean_Fix3PickOffset;//kevin 20120623 Index 放 Shuttle IC Offset
        iIndexWorkDownPos       =Prod.TestZ1_Test+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactCleanHeight;//kevin 20120623 Index Clean down pos
        //iIndexWorkUpPos        =Prod.TestZ1_Test+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactShiftHeight;//kevin 20120623 Index Clean down pos
        iIndexWorkUpPos         =(Prod.TestZ1_Test+TestIF_File.iPadThickness)/2;//kevin 20121114 Bill 說清潔高度最高到最低一半 Index Clean down pos
        if(iIndexWorkUpPos<Prod.TestZ1_Test)
            iIndexWorkUpPos     =Prod.TestZ1_Test+1000;

//         iArmPickShuttlePos     =TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle1PickOffset;//kevin 20120623 In out arm 吸取Shuttle IC Offset
//         iArmPlaceShuttlePos    =TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle1PlaceOffset;//kevin 20120623 In out arm 放Shuttle IC Offset
//         iArmXShuttle1OffsetPos =TestIF_File.iAutoClean_Shuttle1XOffset;
//         iArmYShuttle1OffsetPos =TestIF_File.iAutoClean_Shuttle1YOffset;
    }
    else if(bUseCleanArm==1)//kevin 20140903 使用ARM2
    {
        iIndexPickShuttlePos    =Prod.TestZ2_Pick+TestIF_File.iPadThickness;    //kevin 20120623 Index 吸取 Shuttle IC Offset
        iIndexPlaceShuttlePos   =Prod.TestZ2_Place+TestIF_File.iPadThickness;   //TestIF_File.iAutoClean_Fix3PickOffset;//kevin 20120623 Index 放 Shuttle IC Offset
        iIndexWorkDownPos       =Prod.TestZ2_Test+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactCleanHeight;//kevin 20120623 Index Clean down pos
        //iIndexWorkUpPos        =Prod.TestZ2_Test+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactShiftHeight;//kevin 20120623 Index Clean down pos
        iIndexWorkUpPos         =(Prod.TestZ2_Test+TestIF_File.iPadThickness)/2;//kevin 20121114 Bill 說清潔高度最高到最低一半 Index Clean down pos

        if(iIndexWorkUpPos<Prod.TestZ2_Test)
            iIndexWorkUpPos     =Prod.TestZ2_Test+1000;

//        iArmPickShuttlePos      =TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle2PickOffset;   //kevin 20120623 In out arm 吸取Shuttle IC Offset
//        iArmPlaceShuttlePos     =TestIF_File.iPadThickness+TestIF_File.iAutoClean_Shuttle2PlaceOffset;  //kevin 20120623 In out arm 放Shuttle IC Offset
//        iArmXShuttle1OffsetPos  =TestIF_File.iAutoClean_Shuttle2XOffset;
//        iArmYShuttle1OffsetPos  =TestIF_File.iAutoClean_Shuttle2YOffset;
    }
}
//------------------------------------------------------------------------------
void EnableAutoclean(bool Manual)
{
    if(Manual)
    {
        if(TestIF.iAutoClean_Function &&bIsAutoOneCycle==false && iOneCycle==0) //kevin 20140217
        {
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
            bIsAutoOneCycleAutoclean=true;
        }
    }
    else
    {
        fMain->AutoCleanContactCountLabel->Caption=iAutoClean_IndexContactCount;
        if(TestIF.iAutoClean_Function && TestIF.iAutoClean_IntervalContact!=0 &&iOneCycle==0 &&//kevin 20140217
           iAutoClean_IndexContactCount>=(TestIF.iAutoClean_IntervalContact))   //kevin 20121022   iCleanOut==0
        {
            AutoCleanWriteData("iAutoClean_IndexTime", iAutoClean_IndexContactCount);
            InitialAutoCleanAllTask();                                          //Sam 20230504 : 整理 InitialAutoCleanTask
            bIsAutoOneCycleAutoclean=true;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall AutoCleanWriteData(AnsiString Str, int Data)                    //kevin 20120710 記錄資料
{
    AnsiString S="";
    AnsiString szDir="";
    S=GetLastOpenFN();
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    WriteIniData(szDir, "Configuration", Str, Data);                            //kevin 20120623
}
//------------------------------------------------------------------------------
int GetAutoCleanPickCount()
{
    int iCount=4;
    if(TestIF.iTestMode==SingleSite ||                                          //Steven 20140614 : for Auto Clean Single Site
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iCount=1;
    }
    else if(bUseAxExPicker())
    {
        iCount=2;
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 往上移動
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
        iCount=2;
    }
    else
    {
        if(bCleanKitPitchLess4000==true)                                        //Steven 20250923 : HT9045CN 吸嘴最小是20x3mm
            iCount=1;
        else
            iCount=4;
    }
    return iCount;
}
//------------------------------------------------------------------------------
int GetAutoCleanPickStep(int iCol)
{
    int iSuckCol=-1;                                                            //Steven 20240918 : fixed for auto clean
    if(TestIF.iTestMode==SingleSite ||                                          //Steven 20140614 : for Auto Clean Single Site
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        if(iCol==0)
        {
            if(Prod.bSingleUseOtherSuck)
                iSuckCol=1;
            else
                iSuckCol=0;
        }
    }
    else if(bUseAxExPicker() ||
            iInArmType==e9045_1x2_4_Hot)                                        //Jimmychiu 20260109 Add 1x2_4 auto clean error
    {
        if(iCol==0)
            iSuckCol=0;
        else if(iCol==1)
            iSuckCol=2;
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 往上移動
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
        if(iCol==0)
            iSuckCol=0;
        else if(iCol==1)
            iSuckCol=3;
    }
    else
    {
        iSuckCol=iCol;
    }
    return iSuckCol;
}
//------------------------------------------------------------------------------
int CalculateAutoCleanXPitch()
{
    int iAutoCleanXPitch=1;

    if(TestIF.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iAutoCleanXPitch=1;
    }
    else if(bUseAxExPicker() ||
            i1x2_4UseACEGPicker==1)                                             //Jimmychiu 20260109 Add 1x2_4 auto clean error
    {
//        iAutoCleanXPitch=TestIF_File.iAutoClean_XDivision/2;
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/2.0);
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 往上移動
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
//        iAutoCleanXPitch=TestIF_File.iAutoClean_XDivision/2;
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/2.0);
    }
    else if(TestIF.iTestMode==QualSite1X4 ||
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
//        iAutoCleanXPitch=TestIF_File.iAutoClean_XDivision/4;
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/4.0);
    }
    else
    {
//        iAutoCleanXPitch=TestIF_File.iAutoClean_XDivision/4;
        iAutoCleanXPitch=(int)ceil(TestIF_File.iAutoClean_XDivision/4.0);
    }
    return iAutoCleanXPitch;
}
//------------------------------------------------------------------------------
int GetXPitchOfCleanKit_HP()
{
    double dPitch=6000;

    bCleanKitPitchOver12000=false;

    if(TestIF.iTestMode==SingleSite ||                                          //wei 20220823 Single Site使用C吸嘴判斷
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iAutoCleanUseXPitch=1;
        dPitch=iXpitchMaxX3;
    }
    else if(bUseAxExPicker())
    {
        iAutoCleanUseXPitch=2;
        dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;    //吸嘴1跟吸嘴3間，佔了4個HotPlateForm.XPitch
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;
            bCleanKitPitchOver12000=true;
        }
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 往上移動
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site)
    {
        iAutoCleanUseXPitch=2;
        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
            bCleanKitPitchOver12000=true;
        }
    }
    else if(TestIF.iAutoClean_DeveicePices<=4)                                  //jou 2013-10-02 修正Auto Clean Qual site 1x4 pitch <= 20mm , 會超出硬體極限.
    {
        iAutoCleanUseXPitch=1;                                                  //kevin 20120229
        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;                  //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch
    }
    else
    {
        if(TestIF.iAutoClean_XDivision>=8 &&
           TestIF.dAutoClean_XPitch<=2000)                                      //Steven 20251023 : fixed for 8x4 x-pitch=20mm auto clean
        {
            iAutoCleanUseXPitch=2;
        }
        else if(TestIF.dAutoClean_XPitch<2000)
        {
            iAutoCleanUseXPitch=2;                                              //Steven 20250930 : fixed for small pitch auto clean
        }
        else
        {
            iAutoCleanUseXPitch=1;                                              //kevin 20120229
        }
        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;                  //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=iXpitchMaxX3;                                                //JerryYang 20200622 : x-pitch超過12000會失步
            bCleanKitPitchOver12000=true;
        }
    }
    return int(dPitch);
}
//------------------------------------------------------------------------------
int GetXPitchOfCleanKit_Kit()
{
    double dPitch=6000;
    bCleanKitPitchOver12000=false;
    bCleanKitPitchLess4000=false;                                               //jou 2015-03-24 tray x pitch太小導致無法吸取

    if(TestIF.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        iAutoCleanUseXPitch=1;
        dPitch=iXpitchMaxX3;                                                    //Steven 20140614 : for Auto Clean Single Site   //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
    }
    else if(bUseAxExPicker() ||
            i1x2_4UseACEGPicker==1)                                             //Jimmychiu 20260109 Add 1x2_4 auto clean error
    {
        if(TestIF.iAutoClean_XDivision==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.iAutoClean_XDivision>2 &&                                //Steven 20240902 : 修正1x2 auto clean
                TestIF.iAutoClean_DeveicePices==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.bAutoClean_UseTray)                                      //Steven 20240902 : 修正1x2 auto clean
        {
            iAutoCleanUseXPitch=CalculateAutoCleanXPitch();
        }
        else
        {
            iAutoCleanUseXPitch=2;                                              //jou 2014-06-13 4 -> 2 修正 Dual site Clean kit 2x8 start:33 pitch:22 超過硬體極限
        }

        dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;    //吸嘴1跟吸嘴3間，佔了4個HotPlateForm.XPitch
        if(dPitch>(iXpitchMaxX3+300))                                           //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;
            bCleanKitPitchOver12000=true;
        }
        else if(dPitch<iXpitchMax)
        {
            if(TestIF.iAutoClean_XDivision_Kit==2)                              //JerryYang 20210224 : 修正dual site clean kit 1x2 x-pitch 25mm點位計算錯誤
            {
                iAutoCleanUseXPitch=2;
                dPitch=double(TestIF.dAutoClean_XPitch)*3.0;
            }
            else
            {
                iAutoCleanUseXPitch=4;                                          //Alick 20161107 修正使用CLEAN TRAY,TRAY X-PITCH<13.33取放位置計算異常,iAutoCleanUseXPitch=4,針對CleanIC數量4&8
                dPitch=double(TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch)/2.0*3.0;
            }
            bCleanKitPitchLess4000=true;
        }
    }
    else if(bUseAxxGPicker() ||                                                 //Steven 20240515 : 往上移動
            iCloseSiteModeFor1x4==e1x4CloseAbAc)                                //Steven 20241111 : for 1x4 close 2 site
    {
        if(TestIF.iAutoClean_XDivision==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.iAutoClean_XDivision==8)                                 //Steven 20250318 : fix for 1x2 / 2x2 auto clean with 8 col, x-pitch 25mm
        {
            iAutoCleanUseXPitch=4;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
            if(dPitch>iXpitchMaxX3)                                             //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
            {
                iAutoCleanUseXPitch=2;
            }
        }
        else if(TestIF.iAutoClean_XDivision>2 &&                                //Steven 20240902 : 修正1x2 auto clean
                TestIF.iAutoClean_DeveicePices==2)
        {
            iAutoCleanUseXPitch=1;
        }
        else if(TestIF.bAutoClean_UseTray)
        {
            iAutoCleanUseXPitch=CalculateAutoCleanXPitch();
        }
        else
        {
            iAutoCleanUseXPitch=2;                                              //jou 2014-06-13 4 -> 2 修正 Dual site Clean kit 2x8 start:33 pitch:22 超過硬體極限
        }

        dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;                    //吸嘴1跟吸嘴4間，佔了2個HotPlateForm.XPitch  //Steven 20180829 : #P180827-ATK-H9-01 Pickup position was wrong during Auto Clean in 2x2 mode.
        if(dPitch>iXpitchMaxX3)                                                 //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
        {
            iAutoCleanUseXPitch=1;
            dPitch=iXpitchMaxX3;                                                //Steven 20241111 : for 1x4 run 3 col tray auto clean
            bCleanKitPitchOver12000=true;
        }
        else if(dPitch<iXpitchMax)
        {
//            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*2;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch;
            bCleanKitPitchLess4000=true;                                        //jou 2015-03-24 tray x pitch太小導致無法吸取
        }
    }
    else
    {
        if(InArmSuck.iPickCol==4 &&
           TestIF.iAutoClean_XDivision==8 &&                                    //JerryYang 20241002 : X 數量8 跳格吸
           TestIF.iAutoClean_DeveicePices%8==0 &&                               //Sam 20250207 : 要跳格吸 Clean Pad 數量也要對
           TestIF.dAutoClean_XPitch*2<iXpitchMin)                               //Steven 20250923 : HT9045CN 吸嘴最小是20x3mm
        {
            iAutoCleanUseXPitch=4;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch
            bCleanKitPitchLess4000=true;

            if(dPitch>iXpitchMaxX3)
            {
                int dSumXPitch=0.0;
                iAutoCleanUseXPitch=2;
                for(int igap=0; igap<5; igap++)
                {
                    dSumXPitch=TestIF.dAutoClean_XPitch*igap;
                    if(dSumXPitch>(double)iXpitchMin)
                    {
                        iAutoCleanUseXPitch=igap;
                        break;
                    }
                }
                bCleanKitPitchLess4000=true;
                dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;          //JimmyChiu 20240415 : modified for auto clean
            }
        }
        else if(InArmSuck.iPickCol==4 &&
                TestIF.iAutoClean_XDivision==8 &&                               //JerryYang 20241002 : X 數量8 跳格吸
                TestIF.iAutoClean_DeveicePices%8==0 &&                          //Sam 20250207 : 要跳格吸 Clean Pad 數量也要對
                TestIF.dAutoClean_XPitch*2<=iXpitchMax)           
        {
            iAutoCleanUseXPitch=2;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch

            if(dPitch<iXpitchMax)
            {
                dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*2*3;
                bCleanKitPitchLess4000=true;
            }
        }
        else if(TestIF.bAutoClean_UseTray &&
                TestIF.dAutoClean_XPitch>=iXpitchMin)
        {
            iAutoCleanUseXPitch=1;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch

            if(dPitch>iXpitchMaxX3)                                             //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3
            {
                iAutoCleanUseXPitch=1;
                dPitch=iXpitchMaxX3;                                            //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch
                bCleanKitPitchOver12000=true;
            }
        }
        else if(TestIF.dAutoClean_XPitch<iXpitchMin)                            //Steven 20241111 : 1333 -> iXpitchMin
        {
            int dSumXPitch=0.0;
            iAutoCleanUseXPitch=2;
            for(int igap=0; igap<5; igap++)
            {
                dSumXPitch=TestIF.dAutoClean_XPitch*igap;
                if(dSumXPitch>(double)iXpitchMin)
                {
                    iAutoCleanUseXPitch=igap;
                    break;
                }
            }
            bCleanKitPitchLess4000=true;
            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //JimmyChiu 20240415 : modified for auto clean
        }
        else if(TestIF.dAutoClean_XPitch>iXpitchMax)                            //Steven 20240305 : 修正Auto Clean Pitch判斷
        {
            iAutoCleanUseXPitch=2;
            bCleanKitPitchOver12000=true;
            dPitch=TestIF.dAutoClean_XPitch*2*3;
        }
        else
        {
            if(InArmSuck.iPickCol==4 &&                                         //JerryYang 20241002 : X 數量8 跳格吸
               TestIF.iAutoClean_XDivision==8 &&
               TestIF.iAutoClean_DeveicePices%8==0 &&                           //Sam 20250207 : 要跳格吸 Clean Pad 數量也要對
               TestIF.dAutoClean_XPitch*2*3<iXpitchMaxX3)                       //Sam 20251020 : 跳格吸要在最大 PitcX 內
            {
                iAutoCleanUseXPitch=2;
            }
            else if(InArmSuck.iPickCol==4 &&                                    //JerryYang 20241002 : X 數量8 跳格吸
                    TestIF.iAutoClean_XDivision==12 &&
                    TestIF.iAutoClean_DeveicePices%12==0 &&                     //Sam 20250207 : 要跳格吸 Clean Pad 數量也要對
                    TestIF.dAutoClean_XPitch*2*3<iXpitchMaxX3)                  //Sam 20251020 : 跳格吸要在最大 PitcX 內
            {
                iAutoCleanUseXPitch=2;
            }
            else
            {
                iAutoCleanUseXPitch=1;
            }

            dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*3;              //吸嘴1跟吸嘴2間，佔了2個HotPlateForm.XPitch

            if(dPitch<iXpitchMax)
            {
                dPitch=TestIF.dAutoClean_XPitch*iAutoCleanUseXPitch*2*3;
                bCleanKitPitchLess4000=true;
            }
        }
    }
    return int(dPitch);
}
//------------------------------------------------------------------------------
int GetXPitchOfCleanKit()
{
    if(IniConfig.bE43AutoCleanUseHotplate ||
       TestIF_File.iTestMode==_6Site2X3)                                        //Steven 20171220 (Wei) : Fixed for 2x3 Auto Clean Hang Up
        return GetXPitchOfCleanKit_HP();
    else
        return GetXPitchOfCleanKit_Kit();
}
//------------------------------------------------------------------------------
int GetYPitchOfCleanKit()
{
    int iYVariable=TestIF.iARM_Y_PITCH;

    if(IniConfig.bE43AutoCleanUseHotplate && USE_IN_Y_IS_AUTO_PITCH==true)            //JerryYang 20251218 : IN/OUT ARM支援不同模組  //Steven 20160629
    {
        if(TestIF_File.dAutoClean_YPitch<IN_OUT_ARM_Y_PITCH_MIN)                //Steven 20180824 : 修正Y-Pitch小於15mm
        {
            iYVariable=TestIF_File.iARM_Y_PITCH;
        }
        else if(TestIF_File.dAutoClean_YPitch>IN_OUT_ARM_Y_PITCH_MAX)           //Steven 20240801 : fixed for auto clean.
        {
            iYVariable=TestIF_File.iARM_Y_PITCH;
        }
        else
        {
            iYVariable=TestIF_File.dAutoClean_YPitch;
        }
    }

    return iYVariable;
}
//------------------------------------------------------------------------------
//Steven 20220120 : DrawAutoClean()的判斷式要跟SetAutoCleanICCount()裡面的一樣
//這個做之前要做過LoadAutoCleanData(),資料才會正確
//------------------------------------------------------------------------------
void SetAutoCleanICCount(bool Work)                                             //ChungHung 20141027 add for SCK want to record AutoClean_pad count
{
    if(InitialOK==false)                                                        //Steven 20200423 : 加上保護機制
        return;

    int iXItem=0, iYItem=0, iSet=0, iStep=0;
    DoTestIFConvert();                                                          //Steven 20160630 : for auto clean, TestIF_File --> TestIF

    if(TestIF_File.iAutoClean_Function==false)                                  //Ifor 20171024 : TestIF.iAutoClean_Function => TestIF_File.iAutoClean_Function
    {
        for(int j=0; j<TestIF.iAutoClean_YDivision; j++)
        {
            for(int i=0; i<TestIF.iAutoClean_XDivision; i++)
            {
                MOT[MMAutoCleanKit].SetTraySingleData(i, j, NULL_IC);
            }
        }
        return;
    }

    if(bRunAutoClean==true && iAutoCleanAlarm!=2)                               //Steven 20180221 : 修正進去Auto Clean畫面資料會被清空的問題  //Ifor 20180727 (wei) ：Auto Clean Clean Count > Alarm Count 0:正常 1: Alarm 2:Clean Count
    {
        return;
    }

    SetAutoCleanTrayPosition();                                                 //Steven 20210825 : 重新整理成Function
    if(Work==false)                                                             //做完一次
    {
        fMain->AutoCleanStringGrid->ColCount=TestIF.iAutoClean_XDivision;
        fMain->AutoCleanStringGrid->RowCount=(TestIF.iAutoClean_YDivision+1)*3; //ChungHung 20140317 alter 如果設為1 RowCount 會不夠 TestIF_File.iAutoClean_YDivision*3 --->  (TestIF_File.iAutoClean_YDivision+1)*3

        fMain->tmyAutoClean->XItem=TestIF.iAutoClean_XDivision;
        fMain->tmyAutoClean->YItem=TestIF.iAutoClean_YDivision;
        MOT[MMAutoCleanKit].Tray.SetXYItem(TestIF.iAutoClean_XDivision, TestIF.iAutoClean_YDivision);     //Steven 20160614 : 設定Tray XY Item改用function加上保護

        GetXPitchOfCleanKit();

        if(TestIF.iTestMode==QualSite2X2 &&                                     //jou 20210712 : 修正 QualSite2X2 Tray pitch 小於2666 hang up
           bCleanKitPitchLess4000==true &&
           TestIF.iAutoClean_DeveicePices==4 &&
           TestIF.iAutoClean_XDivision==8)
        {
            for(int i=0; i<TestIF.iAutoClean_DeveicePices; i++)
            {
                iSet=i%2+1;
                int X=i*2;
                int Y=ChangeToFloatNonPcnt((double)(i), (double)(TestIF.iAutoClean_XDivision))+TestIF.iAutoClean_YDivision+2;
                SetAutoCleanStringGrid(X, Y, AnsiString(iSet));
            }
        }
        else
        {
            iStep=GetAutoCleanPickCount();                                      //ChungHung 20130711 add //一次使用幾隻吸嘴

            int iNumGroup=iAutoCleanUseXPitch*iStep;
            for(int i=0; i<TestIF.iAutoClean_DeveicePices; i++)
            {
                iSet=(ChangeToFloatNonPcnt((double)((i)), (double)(iNumGroup)))*(iAutoCleanUseXPitch);
                if((i+1)%iAutoCleanUseXPitch==0)
                    iSet=iSet+iAutoCleanUseXPitch;
                else
                    iSet=iSet+(i+1)%iAutoCleanUseXPitch;

                if(iSet>ChangeToFloatNonPcnt((double)(TestIF.iAutoClean_DeveicePices), (double)(iStep)))
                    iSet=ChangeToFloatNonPcnt((double)(TestIF.iAutoClean_DeveicePices), (double)(iStep));

                if(TestIF.iAutoClean_XDivision<=0 ||
                   TestIF.iAutoClean_YDivision<=0)                              //kevin 20140930
                    continue;

                int X=i%TestIF.iAutoClean_XDivision;
                int Y=ChangeToFloatNonPcnt((double)(i), (double)(TestIF.iAutoClean_XDivision))+TestIF.iAutoClean_YDivision+2;
                SetAutoCleanStringGrid(X, Y, AnsiString(iSet));
            }
        }
    }

    iXItem=TestIF.iAutoClean_XDivision;
    iYItem=TestIF.iAutoClean_YDivision;
    MOT[MMAutoCleanKit].Tray.ClearData();                                       //kevin 20190402 add clean autoclean set
    fCleaning->SetDeviceInTray(iXItem, iYItem, TestIF.iAutoClean_DeveicePices, fCleaning->eAutoCleanUsed);  //Jimmychiu 20221027 優化Cleanpad配置方式

    MOT[MMAutoCleanKit].Refresh();                                              //wei 20150422 Refresh
    ReadWriteAutoCleanCount(true);                                              //Jimmychiu 20250103 : fixed for auto clean count to 0 issue
    ReadWriteAutoCleanCount(false);                                             //Steven 20180524 : Fixed for clean count
}
//------------------------------------------------------------------------------
void SetAutoCleanTrayPosition()                                                 //Steven 20210825 : 重新整理成Function
{
    static int iTop=351;                                                        //fMain->tmyAutoClean->Top;
    static int iWidth=89;                                                       //fMain->tmyAutoClean->Width;
    static int iHeight=25;                                                      //fMain->tmyAutoClean->Height;
    if(IniConfig.bE43AutoCleanUseHotplate)                                      //ChungHung 20131120 AutoClean use Hotplate1
    {
        fMain->tmyAutoClean->Top    =fMain->mtPlate2->Top;
        fMain->tmyAutoClean->Width  =fMain->mtPlate2->Width;
        fMain->tmyAutoClean->Height =fMain->mtPlate2->Height;
    }
    else
    {
        fMain->tmyAutoClean->Top    =iTop;                                      //wei 20220728 clean kit固定位置
        fMain->tmyAutoClean->Width  =iWidth;                                    //wei 20220728 clean kit固定位置
        fMain->tmyAutoClean->Height =iHeight;                                   //wei 20220728 clean kit固定位置
    }
}
//------------------------------------------------------------------------------
void InitialAutoCleanAllTask()                                                  //Sam 20230504 : 整理 InitialAutoCleanTask
{
    InitialAutoCleanTask();
    InitialShuttleAutoCleanTask();
    InitialIndexAutoCleanTask();
    if(iOneCycle==1 && bIsAutoOneCycle==false && bManualOneCycle)               //Sam 20230309 : 避免觸發 OneCycle 後，OneCycle 未完成時又觸發 AutoClean 動作，AutoClean 做完並不會提示 OneCycle Finish
    {
        bBackupOneCycle_ByAutoClean=true;
        bManualOneCycle=false;
    }
    bIsAutoOneCycle=true;
    fMain->BtnOneCycleClick(fMain);
}
//------------------------------------------------------------------------------
void InitialAutoCleanTask()
{
    iDoAutoCleanTask=1;
    bPlaceToShuttleByAutoClean=false;                                           //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
    bPickFromShuttleByAutoClean=false;                                          //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
    bLockPlaceToShuttleByAutoClean=false;                                       //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
    bLockPickFromShuttleByAutoClean=false;                                      //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion

    if(CUSTOMER_CODE==CC_SPIL_TAICHUNG_LOGIC &&
       IniConfig.bFTPJamCodeUpload &&
       IniConfig.bEnableFTP)                                                    //Steven 20140917 : 台中SPIL要求Auto Clean要傳送Alarm Code
    {
        fNote->aJamCodeFilePath=fFTPClient->SaveJamCodeFile(IniConfig.SocketHandlerID, Now(), "MES1607", "Autoclean Start");
        fFTPClient->UploadFileToServer2(IniConfig.FtpUplaodPath, fNote->aJamCodeFilePath);    //Steven 20140513 : 移動到Form Show之前
    }
}
//------------------------------------------------------------------------------
void InitialShuttleAutoCleanTask()
{
    iDoShuttle1AutoCleanTask=1;
    iDoShuttle2AutoCleanTask=1;
    iDoShuttleAutoCleanTask=1;
}
//------------------------------------------------------------------------------
void InitialIndexAutoCleanTask()
{
    iDoIndexAutoCleanTask=1;
}
//------------------------------------------------------------------------------
void InitPickFromShuttleTask()
{
    iAutoCleanPickFromShuttleTask=1;
}
//------------------------------------------------------------------------------
void CleanSetSpeed(bool bBackup)
{
    UpdateMyKitSuckDelayTimeToProd();                                           //Steven 20250319 : 針對OnDelayTime轉換包成function
    if(bBackup==true)                                                           //Store and Set System Original Speed
    {
        ArmSpeed[InArm].iBodySP     =TestIF.iAutoClean_MotorSpeed[0];           //In Arm
        ArmSpeed[InArm].iACDCBodySP =TestIF.iAutoClean_MotorSpeed[0];           //In Arm
        ArmSpeed[InArm].iVariSP     =TestIF.iAutoClean_MotorSpeed[0];           //In arm pitch
        ArmSpeed[InArm].iACDCVariSP =TestIF.iAutoClean_MotorSpeed[0];           //In arm pitch
        SHSpeed.iSH1Sp              =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 1
        SHSpeed.iSH1ACDCSp          =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 1
        SHSpeed.iSH2Sp              =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 2
        SHSpeed.iSH2ACDCSp          =TestIF.iAutoClean_MotorSpeed[1];           //Shuttle 2
        ArmSpeed[InArm].iACDCZSP    =TestIF.iAutoClean_MotorSpeed[3];           //In Arm Z
        ArmSpeed[InArm].iZSP        =TestIF.iAutoClean_MotorSpeed[3];           //In Arm Z
    }
    else
    {
        ArmSpeed[InArm].iBodySP     =ArmSpeed_File[InArm].iBodySP;              //In Arm
        ArmSpeed[InArm].iACDCBodySP =ArmSpeed_File[InArm].iACDCBodySP;          //In Arm
        ArmSpeed[InArm].iVariSP     =ArmSpeed_File[InArm].iVariSP;              //In arm pitch
        ArmSpeed[InArm].iACDCVariSP =ArmSpeed_File[InArm].iACDCVariSP;          //In arm pitch
        SHSpeed.iSH1Sp              =SHSpeed_File.iSH1Sp;                       //Shuttle 1
        SHSpeed.iSH1ACDCSp          =SHSpeed_File.iSH1ACDCSp;                   //Shuttle 1
        SHSpeed.iSH2Sp              =SHSpeed_File.iSH2Sp;                       //Shuttle 2
        SHSpeed.iSH2ACDCSp          =SHSpeed_File.iSH2ACDCSp;                   //Shuttle 2
        ArmSpeed[InArm].iACDCZSP    =ArmSpeed_File[InArm].iACDCZSP;             //In Arm Z
        ArmSpeed[InArm].iZSP        =ArmSpeed_File[InArm].iZSP;                 //In Arm Z
        SetOutArmSpeed(false);                                                  //Steven 20130620 : 重置Out Arm速度
    }

    SetMotorAccelSpeed(MInArmX      ,ArmSpeed[InArm].iACDCBodySP);
    SetMotorAccelSpeed(MInArmY      ,ArmSpeed[InArm].iACDCBodySP);
    SetMotorScaleSpeed(MInArmX      ,ArmSpeed[InArm].iBodySP);
    SetMotorScaleSpeed(MInArmY      ,ArmSpeed[InArm].iBodySP);

    SetMotorAccelSpeed(MInArmPitch  ,ArmSpeed[InArm].iACDCVariSP);
    SetMotorScaleSpeed(MInArmPitch  ,ArmSpeed[InArm].iVariSP);

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                                  //JerryYang 20251218 : IN/OUT ARM支援不同模組  //Steven 20160630 : 限制Y-Pitch最低速度為80
    {
        if(ArmSpeed[InArm].iACDCVariSP<80)
            SetMotorAccelSpeed(MInArmPitchY, 80);
        else
            SetMotorAccelSpeed(MInArmPitchY, ArmSpeed[InArm].iACDCVariSP);

        if(ArmSpeed[InArm].iVariSP<80)
            SetMotorScaleSpeed(MInArmPitchY, 80);
        else
            SetMotorScaleSpeed(MInArmPitchY, ArmSpeed[InArm].iVariSP);
        SetMotorAccelSpeed(MInArmPitchX2, ArmSpeed[InArm].iACDCVariSP);
        SetMotorScaleSpeed(MInArmPitchX2, ArmSpeed[InArm].iVariSP);
    }

    int iMot=MInArmZA;
    if(InOutArmPickerUseMotor==eptUseMotCyn)
    {
        SetMotorAccelSpeed(MInArmZA, ArmSpeed[InArm].iACDCZSP);
        SetMotorScaleSpeed(MInArmZA, ArmSpeed[InArm].iZSP);
    }
    else
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iMot=InArmSuck.Suck[i][j].iMotNo;
                SetMotorAccelSpeed(iMot, ArmSpeed[InArm].iACDCZSP);
                SetMotorScaleSpeed(iMot, ArmSpeed[InArm].iZSP);
            }
        }
    }

    SetMotorAccelSpeed(MInShuttle1      ,SHSpeed.iSH1Sp);
    SetMotorScaleSpeed(MInShuttle1      ,SHSpeed.iSH1ACDCSp);
    SetMotorAccelSpeed(MInShuttle2      ,SHSpeed.iSH2Sp);
    SetMotorScaleSpeed(MInShuttle2      ,SHSpeed.iSH2ACDCSp);

    fShowMessage->ShowSpeed(IniConfig.bG05ShowSpeedMessage);
}
//------------------------------------------------------------------------------
void InOutArmSuckActiveSet()
{
    ZeroMemory(bInArmSuckActive, sizeof(bInArmSuckActive));
}
//------------------------------------------------------------------------------
// kevin 20120217 Z移動 到SHUTTLE 2 位置
//------------------------------------------------------------------------------
bool MoveInArmZToShuttlePlace(eWhichShuttle iSht, int iRowSel)                  //20140923 wei : For Shuttle Auto Clean
{
//    int iMot;
    int iShuttlePlace[MAX_ARM_Row][MAX_ARM_Col];
    bool flag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
    bool bNeedDown;

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(iSht==euShuttle1)
            {
                if(IniConfig.bEnableAutoCleanFunction &&
                   TestIF_File.iAutoClean_Function &&
                   bRunAutoClean &&
                   IniConfig.bE48_ShuttleUse4Offset_Autoclean &&                //20140923 wei : For Shuttle Auto Clean
                   TestIF.bEnableAutoAlignment==false)                          //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] -
                                        InArmOffSet[InOfsInSh1]->GetPlace() +
                                        InArmOffSet[InOfsInSh1_AutoClean]->GetPlace()+
                                        InArmOffSet[InOfsInSh1_AutoClean+iRowSel-1]->SingleOffSet->dPlaceOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 僅使用 Clean 設定 Offset 來補償
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF.iAutoClean_Shuttle1PlaceOffset-InArmOffSet[InOfsInSh1]->GetPlace();
                }
                else
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF.iAutoClean_Shuttle1PlaceOffset;
                }
            }
            else
            {
                if(IniConfig.bEnableAutoCleanFunction &&
                   TestIF_File.iAutoClean_Function &&
                   bRunAutoClean &&
                   IniConfig.bE48_ShuttleUse4Offset_Autoclean &&                //20140923 wei : For Shuttle Auto Clean
                   TestIF.bEnableAutoAlignment==false)                          //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] -
                                        InArmOffSet[InOfsInSh2]->GetPlace() +
                                        InArmOffSet[InOfsInSh2_AutoClean]->GetPlace()+
                                        InArmOffSet[InOfsInSh2_AutoClean+iRowSel-1]->SingleOffSet->dPlaceOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 僅使用 Clean 設定 Offset 來補償
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    if(IniConfig.bE34InOutArmPitchZOffsetSameOne)
                        iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF.iAutoClean_Shuttle2PlaceOffset-InArmOffSet[InOfsInSh1]->GetPlace();
                    else
                        iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF.iAutoClean_Shuttle2PlaceOffset-InArmOffSet[InOfsInSh2]->GetPlace();
                }
                else
                {
                    iShuttlePlace[i][j]=Prod.ZInArm_Shuttle2_Place[i][j]+TestIF.iAutoClean_Shuttle2PlaceOffset; //JerryYang 20160301 修正Offset,1-->2
                }
            }
        }
    }

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            bNeedDown=(InArmSuck.Item[i][j]!=NULL_IC &&
                       InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                       InArmSuck.Suck[i][j].GetNeedDestroyStatus());

            flag[i][j]=bNeedDown;

//            if(InArmSuck.Item[i][j]!=NULL_IC &&
//               InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
//               InArmSuck.Suck[i][j].GetNeedDestroyStatus())
//            {
//                iMot=InArmSuck.Suck[i][j].iMotNo;
//                flag[i][j]=MOT[iMot].MotorMove(iShuttlePlace[i][j]);
//            }
//            else
//            {
//                flag[i][j]=false;
//                iShuttlePlace[i][j]=ZSafePos;
//                flag[i][j]=true;
//            }
        }
    }

/*    if(USE_PICKER_COUNT==ep1Picker)
    {
        if(flag[0][0])
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
        if(flag[0][0] && flag[0][1] && flag[0][2] && flag[0][3] &&
           flag[1][0] && flag[1][1] && flag[1][2] && flag[1][3])
        {
            return true;
        }
        else
        {
            return false;
        }
    }     */

    bool bRet=InArmZMoveDown(flag, iShuttlePlace, false);                       //Steven 20251020 : modify for auto clean
    return bRet;
}
//------------------------------------------------------------------------------
//0 Full
//1 Row1 - Kit1
//2 Row2 - Kit1
//3 Row1 - Kit2
//4 Row2 - Kit2
int GetShuttleState(eWhichShuttle iSht, bool bPick)
{
    if(iSht==euShuttle1)
        ptrInSHT=&FLCarryKit;
    else
        ptrInSHT=&BLCarryKit;

    if(TestIF.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac)                                              //Steven 20200720 : 1x4只開site Ac
    {
        return GetShuttleState_1x1_1((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x2_2_14)                                         //Steven 20221027 : 修正Auto Clean
    {
        return GetShuttleState_1x2_2_14((int)iSht, bPick);
    }
    else if(TestIF.iTestMode==DualSite || TestIF.iTestMode==QualSite2X2N)
    {
        return GetShuttleState_1x2_2((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x3_2_14)
    {
        return GetShuttleState_1x3_2_14((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x3_4)
    {
        return GetShuttleState_1x3_4((int)iSht, bPick);
    }
    else if(iInArmType==e9045_1x4_2_14)
    {
        return GetShuttleState_1x4_2_14((int)iSht, bPick);
    }
    else if(TestIF.iTestMode==QualSite1X4 ||
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        return GetShuttleState_1x4_4((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x2_4_14)                                         //Steven 20221027 : 修正Auto Clean
    {
        return GetShuttleState_2x2_4_14((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==QualSite2X2)
    {
        return GetShuttleState_2x2_4((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x3_6)
    {
        return GetShuttleState_2x3_6((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x3_6_14)
    {
        return GetShuttleState_2x3_6_14((int)iSht, bPick);
    }
    else if(iInArmType==e9045_2x4_4_14)
    {
        return GetShuttleState_2x4_4((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4)
    {
        return GetShuttleState_2x4_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        return GetShuttleState_2x5_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        return GetShuttleState_2x6_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)
    {
        return GetShuttleState_2x8_8((int)iSht, bPick);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        return GetShuttleState_2x8_8((int)iSht, bPick);
    }
    else
    {
        ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "GetShuttleState");
    }
    return 0;
}
//------------------------------------------------------------------------------
bool bCleanZMotToPlace[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
bool bCleanZSuckToPlace[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};
bool MoveInOutArmZToKitPickPlace(int Pick, bool bReset, int iSht, int iShuttleRow)  //ChungHung 20150303 add iSht for Hotplate AutoClean
{
    static bool bflag[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};

    bool flag=false;
    int iZPos[MAX_ARM_Row][MAX_ARM_Col], iMot;

    if(bReset)
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                bflag[i][j]=false;
            }
        }
    }

    for(int j=0; j<InArmSuck.iMotCol; j++)
    {
        if(Pick==bAutoPick)
        {
            iZPos[0][j]=Prod.ZInArm_AutoClean_Pick[0][j]+iArmPickTrayPos;
            iZPos[1][j]=Prod.ZInArm_AutoClean_Pick[1][j]+iArmPickTrayPos;
        }
        else
        {
            iZPos[0][j]=Prod.ZInArm_AutoClean_Place[0][j]+iArmPlaceTrayPos;
            iZPos[1][j]=Prod.ZInArm_AutoClean_Place[1][j]+iArmPlaceTrayPos;
        }
    }

    if(Pick==bAutoPick)
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iMot=InArmSuck.Suck[i][j].iMotNo;
                if(bInArmSuckActive[i][j])
                {
                    if(bflag[i][j]==false)
                        bflag[i][j]=MOT[iMot].MotorMove(iZPos[i][j]);
                }
                else
                {
                    bflag[i][j]=true;
                }
            }
        }
    }
    else
    {
        uHPSuckTeam* ppInfo=PlaceToCleanList->ExtractFirstTeam();
        if(PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZMotToPlace)==false)        //RogerYang 20250624 Add HotPlate ErrMessage
        {
            ShowMyMessage(AnsiString().sprintf("%s %s Task=%d", __FUNC__, "No GetHPFirstTeamMotUse", 0));
            return false;
        }

        PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZSuckToPlace);                 //JerryYang 202050813 : fix錯誤吸嘴下降
        if(ppInfo!=NULL)
        {
            for(int i=0; i<InArmSuck.iMotRow; i++)
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    iMot=InArmSuck.Suck[i][j].iMotNo;
                    if(bCleanZMotToPlace[i][j])
                    {
                        if(bflag[i][j]==false)
                            bflag[i][j]=MOT[iMot].MotorMove(iZPos[i][j]);
                    }
                    else
                    {
                        bflag[i][j]=true;
                    }
                }
            }
        }
        else
        {
            ShowMyMessage("HPSuckTeamList has no data!");
            return false;
        }
    }

    flag=true;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(bflag[i][j]==false)
                flag=false;
        }
    }

    return flag;
}
//------------------------------------------------------------------------------
void SearchiAutoCleanNum()                                                      //Steven 20171212 (Wei) : 確認目前正要吸取的的Pad位置
{
    int iMin=TestIF_File.iAutoClean_AlarmCount+99999;                           //JerryYang 20210222 : 初始值改大一點//KevinYang 20200602 : 修正設定Alarm Count 1000會hang up的問題
    int iNow=0;
    for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
    {
        for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
        {
            if(MOT[MMAutoCleanKit].Tray.Data[X][Y]!=NULL_IC && MOT[MMAutoCleanKit].Tray.Data[X][Y]!=HAS_NULL_CLEAN_IC)
            {
                iNow=atoi(fMain->AutoCleanStringGrid->Cells[X][Y+1].c_str());
                if(iNow<iMin)
                {
                    iMin=iNow;
                    iAutoCleanNum=atoi(fMain->AutoCleanStringGrid->Cells[X][Y+TestIF_File.iAutoClean_YDivision+2].c_str());
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void SetAutoCleanStringGrid(int X, int Y, AnsiString Str)                       //Steven 20180524 : Fixed for clean count
{
    fMain->AutoCleanStringGrid->Cells[X][Y]=Str;
    if(Y>0 && Y<4)                                                              //Steven 20200428 : Fixed for auto clean
    {
        if(MOT[MMAutoCleanKit].Tray.Data[X][Y-1]!=NULL_IC)                      //Steven 20210127 : 修正Auto clean存檔時候, 會改變Count
            MOT[MMAutoCleanKit].Tray.iCleanCount[X][Y-1]=atoi(Str.c_str());
        else
            MOT[MMAutoCleanKit].Tray.iCleanCount[X][Y-1]=0;
    }
}
//------------------------------------------------------------------------------
bool RunAutoCleanByArmPickArm2Test()                                            //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
{
    return (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&
            TestIF_File.bArm1PickPlaceArm2Test==true &&
            TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean==true);
}
//------------------------------------------------------------------------------
void ReadWriteAutoCleanCount(bool bRead, bool bReset)                           //Steven 20180524 : Fixed for clean count
{
    AnsiString Str, S="", szDir="";
    int iTemp=0;
    if(bRunAutoClean==false ||                                                  //Ifor 20191024 : add Fix Auto Clean 關閉時Clean Count 修改不會改變
       (bRunAutoClean==true && bReset &&                                        //Steven 20211220 : 修正Clean Count不能歸零的問題
        FLCarryKit.UseSiteNoIC() &&
        BLCarryKit.UseSiteNoIC() &&
        FTestSuck.UseSiteNoIC() &&
        BTestSuck.UseSiteNoIC() &&
        InArmSuck.HasIC()==false))
    {
        if(IniConfig.bE43_1_AutoCleanCountSaveFolder)                           //Steven 20250527 : Save auto clean count to folder
        {
            szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean\\AutoCleanCount.Data");
        }
        else
        {
            S=GetLastOpenFN();
            szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        }

        for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
        {
            for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
            {
                if(bRead)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_FINSH_IC ||   //Jimmychiu 20250103 : fixed for auto clean count to 0 issue
                       MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC    ||
                       MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC       ||   //Isaac 20180417 (jou) : fix auto clean做到一半按暫停, clean count會歸零
                       MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_NULL_CLEAN_IC  )
                    {
                        Str.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
                        S=ReadIniData(szDir, "Configuration", Str, AnsiString("0"));
                        SetAutoCleanStringGrid(X, Y+1, S);
                    }
                }
                else
                {
                    Str.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
                    if(bReset)                                                  //Steven 20211220 : 修正Clean Count不能歸零的問題
                        SetAutoCleanStringGrid(X, Y+1, AnsiString("0"));

                    S=fMain->AutoCleanStringGrid->Cells[X][Y+1];
                    iTemp=atoi(S.c_str());                                      //Jimmychiu 20250103 : fixed for auto clean count to 0 issue
                    S=IntToStr(iTemp);
                    WriteIniDataNoLog(szDir, "Configuration", Str, S);
                }
            }
        }
        WriteIniDataNoLog(szDir, "Configuration", "iIndexArmAutoCleanCnt", TestIF_File.iIndexArmAutoCleanCnt);  //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
    }
}
//------------------------------------------------------------------------------
int SearchCleanNum()
{
    AnsiString Str;
    int iMin=9999;                                                              //Steven 20171211 (Wei) : 修正計算方式
    int iNow=0;
    if(TestIF_File.iAutoClean_Function==0)                                      //Ifor 20191024 : add 避免Auto Clean 關閉後 Clean Count會歸零
    {
        iMin=atoi(fCleaning->edCleaningCount->Text.c_str());
    }
    else
    {
        for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
        {
            for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
            {
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_FINSH_IC ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC    ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC       ||   //Isaac 20180417 (jou) : fix auto clean做到一半按暫停, clean count會歸零
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_NULL_CLEAN_IC  )
                {
                    iNow=atoi(fMain->AutoCleanStringGrid->Cells[X][Y+1].c_str());
                    if(iNow!=0 && iNow<iMin)
                    {
                        iMin=iNow;
                    }
                }
            }
        }

        if(iMin==9999)                                                          //Steven 20171211 (Wei) : 修正計算方式
            iMin=0;

        AnsiString sAutoCleanCount=IntToStr(iMin);                              //Jimmychiu 20250103 : fixed for auto clean count to 0 issue
        fShowBinSelect->ed_AutoCleanCount->Text=sAutoCleanCount;                //Steven 20180524 : Fixed for clean count
        fCleaning->edCleaningCount->Text=sAutoCleanCount;
        Str.sprintf("Cleaned Count %d / %d", iMin, TestIF_File.iAutoClean_AlarmCount);
        fMain->pnlCleanCount->Caption=Str;                                      //Steven 20240731 : add for auto clean count
        if(iMin>=TestIF_File.iAutoClean_AlarmCount)
        {
            fMain->pnlCleanCount->Font->Color=clRed;
        }
        else
        {
            fMain->pnlCleanCount->Font->Color=clNavy;
        }
    }
    return iMin;
}
//------------------------------------------------------------------------------
void SetShuttleIcForSpecialMode(eWhichShuttle iSht, int iType)                  //Steven 20221006 : 針對特殊模式進行Clean Pad置換
{
    if(iSht==euShuttle1)
        ptrInSHT=&FLCarryKit;
    else
        ptrInSHT=&BLCarryKit;

    if(iCloseSiteModeFor2x8==e2x8Run2x2_13 ||
       iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j!=0+iCloseSiteStep_2x8 &&
                   j!=2+iCloseSiteStep_2x8)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8CloseEven ||
            iCloseSiteModeFor2x8==e2x8CloseEven1By1)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j==1 || j==3 || j==5 || j==7)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8CloseOdd ||
            iCloseSiteModeFor2x8==e2x8CloseOdd1By1)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j==0 || j==2 || j==4 || j==6)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x4Standard)                           //Steven 20240801 : fixed for auto clean.
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j<iCloseSiteStep_2x8 ||
                   j>=iCloseSiteStep_2x8+4)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x6==e2x6Run2x4)                                   //Steven 20240801 : fixed for auto clean.
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(j<iCloseSiteStep_2x6 ||
                   j>=iCloseSiteStep_2x6+4)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x6==e2x6CloseCenter2x4)                           //Steven 20241025 : fixed for 2x6 close center site auto clean
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(Prod.bInSuckUse[iSht][i][j]==false)
                {
                    int iSiteCol=GetShuttleCol(i, j);
                    ptrInSHT->SetItemData(i, iSiteCol, iType);
                }
            }
        }
    }
    else if(iCloseSiteModeFor2x8>e2x8OneByOne ||
            iCloseSiteModeFor2x6>e2x6OneByOne)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(Prod.bInSuckUse[iSht][i][j]==false)
                {
                    ptrInSHT->SetItemData(i, j, iType);
                }
            }
        }
    }
    else if(iType==HAS_NULL_CLEAN_IC)
    {
        if(fCleaning->b1x2SiteAbClosePutDummy)                                  //Steven 20180903 : 1x2 close Ab Auto Clean //Steven 20190509 : Fixed
        {
            if(bUse8Picker)
            {
                if(iInArmType==e9045_1x2_2_14)
                {
                    if(InArmSuck.Item[0][3]==NULL_IC)
                        InArmSuck.SetItemData(0, 3, iType);
                }
                else if(InArmSuck.Item[0][2]==NULL_IC)
                {
                    InArmSuck.SetItemData(0, 2, iType);
                }
            }
            else
            {
                if(iInArmType==e9045_1x2_2_14)
                {
                    if(InArmSuck.Item[1][3]==NULL_IC)
                        InArmSuck.SetItemData(1, 3, iType);
                }
                else if(InArmSuck.Item[1][2]==NULL_IC)
                {
                    InArmSuck.SetItemData(1, 2, iType);
                }
            }
        }
        else if(iInArmType==e9045_1x4_1_Ac)
        {
            ptrInSHT->SetItemData(0, 0, iType);
            ptrInSHT->SetItemData(0, 1, iType);
            ptrInSHT->SetItemData(0, 3, iType);
        }
    }
    else
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                if(ptrInSHT->Item[i][j]==HAS_NULL_CLEAN_IC &&
                   (ptrInSHT->iAutoCleanRecX[i][j]==-1 ||
                    ptrInSHT->iAutoCleanRecY[i][j]==-1))                        //Steven 20211221 : 清除異常的Auto Clean資料
                {
                    ptrInSHT->SetItemData(i, j, NULL_IC);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
bool RestoreCleanKitData()                                                      //ChungHung 20130628 add 回復已被InArm吸走的CleanKit上的IC計數  請勿亂用
{
    int iXpos, iYpos;
    int iCount=0;

//    if(TestIF.iTestMode==_12Site2X6)                                          //KevinCheng 20251209 : Mark  //Steven 20140804 : 修正12Site Auto Clean超過極限問題
//    {
//        return true;
//    }

    if(InArmSuck.HasType(HAS_CLEAN_IC) || InArmSuck.HasType(HAS_NULL_CLEAN_IC)) //KevinCheng 20251209 : 增加條件避免InArm資料只有NULL_CLEAN_IC時不會近來清除
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                iXpos=InArmSuck.iAutoCleanRecX[i][j];
                iYpos=InArmSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(InArmSuck.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                iXpos=InArmSuck.iAutoCleanRecX[i][j];
                iYpos=InArmSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(FLCarryKit.HasType(HAS_CLEAN_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FLCarryKit.iAutoCleanRecX[i][j];
                iYpos=FLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(FLCarryKit.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FLCarryKit.iAutoCleanRecX[i][j];
                iYpos=FLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(BLCarryKit.HasType(HAS_CLEAN_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BLCarryKit.iAutoCleanRecX[i][j];
                iYpos=BLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(BLCarryKit.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BLCarryKit.iAutoCleanRecX[i][j];
                iYpos=BLCarryKit.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(BTestSuck.HasType(HAS_CLEAN_IC))                                         //JerryYang 20160825 回復被index arm吸走的CleanKit上CleanKit上的IC計數
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BTestSuck.iAutoCleanRecX[i][j];
                iYpos=BTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(FTestSuck.HasType(HAS_CLEAN_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FTestSuck.iAutoCleanRecX[i][j];
                iYpos=FTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        iCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                        iCount--;
                        SetAutoCleanStringGrid(iXpos, iYpos+1, AnsiString(iCount));
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, HAS_CLEAN_IC);
                    }
                }
            }
        }
    }

    if(BTestSuck.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=BTestSuck.iAutoCleanRecX[i][j];
                iYpos=BTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }

    if(FTestSuck.HasType(CLEAN_FINISH_IC))
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                iXpos=FTestSuck.iAutoCleanRecX[i][j];
                iYpos=FTestSuck.iAutoCleanRecY[i][j];
                if(iXpos>=0 && iYpos>=0)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]==HAS_NULL_CLEAN_IC)
                    {
                        MOT[MMAutoCleanKit].SetTraySingleData(iXpos, iYpos, CLEAN_FINISH_IC);
                    }
                }
            }
        }
    }
    ReadWriteAutoCleanCount(false);                                             //Steven 20180524 : Fixed for clean count
    return true;
}
//------------------------------------------------------------------------------
//==============================================================================
//AI(ht9045-v899) 20260407: cCleanKitPickPlan implementation
//  Replaces SearchCleanKitUpDown search+soft-limit+assign chain
//  with exhaustive plan enumeration.
//==============================================================================
//AI(ht9045-v899) 20260407: runtime switch for new/old auto clean search logic
//AI(ht9045-v899) 20260514: align comment with actual default (off; legacy SearchCleanKitUpDown is in use)
bool bUseCKPP = false;  // default: false -> use legacy SearchCleanKitUpDown; set true to enable cCleanKitPickPlan
//AI(ht9045-v899) 20260408: global plan instance shared across pick/move/check functions
cCleanKitPickPlan g_CKPlan;
cCleanKitPickPlan::cCleanKitPickPlan()
{
    m_iSht = 0;
    m_iShuttleRow = 0;
    m_iPickerCount = 0;
    m_iXPitchStep = 0;
    m_dMovePitchX = 0;
    m_dKitXPitch = 0;
    m_iBaseX = 0;
    m_iSLimN = 0;
    m_iSLimP = 0;
    m_iHPXOfs = 0;
    m_iXItem = 0;
    m_iYItem = 0;
    m_iRealRow = 0;
    m_iSuckRow = 0;
    m_iKitStep = 0;
    m_bRowHasIC = false;
    m_bFound = false;
    ZeroMemory(m_bDemand, sizeof(m_bDemand));
    ZeroMemory(&m_Best, sizeof(m_Best));
}
//------------------------------------------------------------------------------
int cCleanKitPickPlan::CalcX(int iPhySuck, int iKitCol)
{
    return m_iBaseX +
           (int)(m_dMovePitchX * (iInArmXBase - iPhySuck)) +
           (int)(m_dKitXPitch * iKitCol) +
           m_iHPXOfs;
}
//------------------------------------------------------------------------------
bool cCleanKitPickPlan::IsPadAt(int iKitCol, int iKitRow)
{
    if(iKitCol < 0 || iKitCol >= m_iXItem) return false;
    if(iKitRow < 0 || iKitRow >= m_iYItem) return false;
    return (MOT[MMAutoCleanKit].Tray.Data[iKitCol][iKitRow] == HAS_CLEAN_IC);
}
//------------------------------------------------------------------------------
void cCleanKitPickPlan::BuildDemand()
{
    ZeroMemory(m_bDemand, sizeof(m_bDemand));
    m_bRowHasIC = false;

    for(int j = 0; j < InArmSuck.iMaxCol; j++)
    {
        int iSuckCol = GetAutoCleanPickStep(j);
        if(iSuckCol == -1) continue;

        int iShtRow = m_iSuckRow;
        int iShtCol = iSuckCol + m_iKitStep;
        int iSiteCol = GetShuttleCol(iShtRow, iShtCol);
        int iSiteRow = 0;

        if(IsNNMode() == NN_2Row)
            iSiteRow = (m_iSht == 0) ? 2 : 0;
        else if(IsNNMode() == NN_1Row)
            iSiteRow = (m_iSht == 0) ? 1 : 0;
        else
            iSiteRow = 0;

        bool bSuckEmpty = (InArmSuck.Item[m_iRealRow][iSuckCol] == NULL_IC);

        if(i1x2_4UseACEGPicker == 1)
        {
            if(CosFunction.bUseAutoCleanCloseSiteAlsoDo == true)
                m_bDemand[iSuckCol] = (TestIF.iSiteMap[iSiteRow][iSiteCol] > 0 && bSuckEmpty);
            else
                m_bDemand[iSuckCol] = (Prod.bInSuckUse[m_iSht][iShtRow][iShtCol] && bSuckEmpty);
        }
        else if(Prod.bInSuckUse[m_iSht][iShtRow][iShtCol] == true)
        {
            if(CosFunction.bUseAutoCleanCloseSiteAlsoDo == true)
                m_bDemand[iSuckCol] = (TestIF.iSiteMap[iSiteRow][iSiteCol] > 0 && bSuckEmpty);
            else
                m_bDemand[iSuckCol] = (Prod.fInArmSuck4x8[m_iSht][iShtRow][iShtCol] && bSuckEmpty);
        }

        if(InArmSuck.Item[m_iRealRow][iSuckCol] != NULL_IC)
            m_bRowHasIC = true;
    }

    // second-pass: if bRowHasIC, re-evaluate demand with bRowHasIC logic
    if(m_bRowHasIC)
    {
        for(int j = 0; j < InArmSuck.iMaxCol; j++)
        {
            int iSuckCol = GetAutoCleanPickStep(j);
            if(iSuckCol == -1) continue;

            int iShtRow = m_iSuckRow;
            int iShtCol = iSuckCol + m_iKitStep;
            bool bSuckEmpty = (InArmSuck.Item[m_iRealRow][iSuckCol] == NULL_IC);

            if(bSuckEmpty)
                m_bDemand[iSuckCol] = (m_bDemand[iSuckCol] || Prod.bInSuckUse[m_iSht][iShtRow][iShtCol]);
            else
                m_bDemand[iSuckCol] = false;
        }
    }
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: legacy EvaluatePlan removed; enumeration delegated to cArmPickPlan
//AI(ht9045-v899) 20260514: IArmPickPlanContext implementation begins
int cCleanKitPickPlan::GetLogicalPickerCount()
{
    return m_iPickerCount;
}
//------------------------------------------------------------------------------
bool cCleanKitPickPlan::GetPhysicalSucker(int iLogical, int& rOutRow, int& rOutCol)
{
    int iPhys = GetAutoCleanPickStep(iLogical);
    if(iPhys < 0) return false;
    rOutRow = m_iRealRow;
    rOutCol = iPhys;
    return true;
}
//------------------------------------------------------------------------------
int cCleanKitPickPlan::GetTargetRowMax()
{
    return m_iYItem;
}
//------------------------------------------------------------------------------
int cCleanKitPickPlan::GetTargetColMax()
{
    return m_iXItem;
}
//------------------------------------------------------------------------------
bool cCleanKitPickPlan::ComputeSlot(int iLogical,
                                    int iLeadLogical,
                                    int iLeadTargetRow,
                                    int iLeadTargetCol,
                                    int& rOutTargetRow,
                                    int& rOutTargetCol,
                                    int& rOutX,
                                    int& rOutY)
{
    int iPhys = GetAutoCleanPickStep(iLogical);
    if(iPhys < 0) return false;
    rOutTargetRow = iLeadTargetRow;                                     // AutoClean: same kRow for all slots
    rOutTargetCol = iLeadTargetCol + m_iXPitchStep * (iLogical - iLeadLogical);
    rOutX = CalcX(iPhys, rOutTargetCol);
    rOutY = 0;                                                          // AutoClean: no Y-pitch by plan
    return true;
}
//------------------------------------------------------------------------------
bool cCleanKitPickPlan::IsReachable(int iX, int /*iY*/)
{
    return (iX >= m_iSLimN && iX <= m_iSLimP);
}
//------------------------------------------------------------------------------
bool cCleanKitPickPlan::IsTargetCandidate(int iTargetRow, int iTargetCol)
{
    return IsPadAt(iTargetCol, iTargetRow);
}
//------------------------------------------------------------------------------
bool cCleanKitPickPlan::IsSuckerDemanded(int iLogical)
{
    int iPhys = GetAutoCleanPickStep(iLogical);
    if(iPhys < 0 || iPhys >= 8) return false;
    return m_bDemand[iPhys];
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: preserve legacy "skip plans where lead has no pad / unreachable"
bool cCleanKitPickPlan::IsLeadValid(int iLeadLogical, int iLeadTargetRow, int iLeadTargetCol)
{
    int iLeadPhys = GetAutoCleanPickStep(iLeadLogical);
    if(iLeadPhys < 0) return false;
    if(!IsPadAt(iLeadTargetCol, iLeadTargetRow)) return false;
    int iLeadX = CalcX(iLeadPhys, iLeadTargetCol);
    if(iLeadX < m_iSLimN || iLeadX > m_iSLimP) return false;
    return true;
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: translate generic plan back into legacy TPickPlan/TPickSlot
void cCleanKitPickPlan::CopyGenericPlanToBest()
{
    ZeroMemory(&m_Best, sizeof(m_Best));
    if(!m_Engine.Found()) return;

    const TGenericPickPlan& gp = m_Engine.GetPlan();
    int iCount = gp.iSlotCount;
    if(iCount > CKPP_MAX_SLOTS) iCount = CKPP_MAX_SLOTS;

    m_Best.iSlotCount     = iCount;
    m_Best.iStartLogical  = gp.iLeadLogical;
    m_Best.iKitCol        = gp.iLeadTargetCol;
    m_Best.iKitRow        = gp.iLeadTargetRow;
    m_Best.iActiveCount   = gp.iActiveCount;

    for(int i = 0; i < iCount; i++)
    {
        const TGenericPickSlot& gs = gp.Slots[i];
        TPickSlot& s = m_Best.Slots[i];
        s.iLogical   = gs.iLogical;
        s.iPhysical  = gs.iPhysCol;
        s.iKitCol    = gs.iTargetCol;
        s.iXPos      = gs.iX;
        s.bReachable = gs.bReachable;
        s.bHasPad    = gs.bTargetOK;
        s.bDemanded  = gs.bDemanded;
        s.bActive    = gs.bActive;
    }
}
//------------------------------------------------------------------------------
void cCleanKitPickPlan::Init(int iSht, int iShuttleRow)
{
    m_iSht = iSht;
    m_iShuttleRow = iShuttleRow;
    m_iPickerCount = GetAutoCleanPickCount();
    m_iXPitchStep = iAutoCleanUseXPitch;
    m_dKitXPitch = TestIF_File.dAutoClean_XPitch;
    m_iBaseX = Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase];
    m_iSLimN = MOT[MInArmX].Motor->PSoftLimitN;
    m_iSLimP = MOT[MInArmX].Motor->PSoftLimitP;
    m_iHPXOfs = HotplatlXOffset;
    m_iXItem = MOT[MMAutoCleanKit].Tray.XItem;
    m_iYItem = MOT[MMAutoCleanKit].Tray.YItem;
    m_bFound = false;
    ZeroMemory(&m_Best, sizeof(m_Best));

    // compute dMovePitchX (same as MoveInArmXYPickCleanKit)
    int iRawPitch = GetXPitchOfCleanKit();
    if(USE_PICKER_COUNT == ep1Picker)
        m_dMovePitchX = 0;
    else if(USE_PICKER_COUNT == ep16Picker)
        m_dMovePitchX = double(iRawPitch) / 7.0;
    else
        m_dMovePitchX = double(iRawPitch) / 3.0;

    // determine real row
    m_iSuckRow = 0;
    m_iKitStep = 0;
    GetInarmSuckRow(iShuttleRow, m_iSuckRow, m_iKitStep);

    if(IniConfig.bE43AutoCleanUseHotplate || bUse8Picker)
        m_iRealRow = m_iSuckRow;
    else
        m_iRealRow = 1;

    BuildDemand();
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260514: Search() now delegates enumeration to generic cArmPickPlan
bool cCleanKitPickPlan::Search()
{
    m_bFound = false;
    ZeroMemory(&m_Best, sizeof(m_Best));

    m_Engine.Init(this);                                                // this implements IArmPickPlanContext
    m_bFound = m_Engine.Search();
    CopyGenericPlanToBest();

    if(m_bFound)
    {
        RecordProcess(AnsiString().sprintf(
            "CKPP_PLAN startL=%d kCol=%d kRow=%d active=%d/%d X0=%d Xn=%d",
            m_Best.iStartLogical, m_Best.iKitCol, m_Best.iKitRow,
            m_Best.iActiveCount, m_Best.iSlotCount,
            m_Best.Slots[0].iXPos,
            m_Best.Slots[m_Best.iSlotCount > 0 ? m_Best.iSlotCount-1 : 0].iXPos));
    }

    return m_bFound;
}
//------------------------------------------------------------------------------
void cCleanKitPickPlan::Apply()
{
    if(!m_bFound) return;

    iAutoCleanStart = m_Best.iStartLogical;
    iAutoCleanPickPlateX = m_Best.iKitCol;
    iAutoCleanPickPlateY = m_Best.iKitRow;

    // clear all sucker active flags first
    for(int r = 0; r < MAX_ARM_Row; r++)
        for(int c = 0; c < MAX_ARM_Col; c++)
            bInArmSuckActive[r][c] = false;

    // set active flags per plan
    for(int i = 0; i < m_Best.iSlotCount; i++)
    {
        TPickSlot &s = m_Best.Slots[i];
        if(s.iPhysical >= 0 && s.iPhysical < MAX_ARM_Col)
            bInArmSuckActive[m_iRealRow][s.iPhysical] = s.bActive;
    }
}
//------------------------------------------------------------------------------
//AI(ht9045-v899) 20260408: check if current shuttle group has any sucker demand
bool cCleanKitPickPlan::HasDemand()
{
    for(int j = 0; j < InArmSuck.iMaxCol; j++)
    {
        int iSuckCol = GetAutoCleanPickStep(j);
        if(iSuckCol >= 0 && iSuckCol < MAX_ARM_Col && m_bDemand[iSuckCol])
            return true;
    }
    return false;
}
//==============================================================================

bool SearchCleanKitRowCol(int& iKRow,int& iKCol)
{
    if(TestIF_File.iTestMode==_12Site2X6 &&                                     //Jimmychiu 20260109 : 依據特殊模式改變Auto Clean吸取順序
       fCleaning->iDeviceCount==16)
    {
        for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
        {
            for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)                 
            {
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC)
                {
                    iKCol=X;
                    iKRow=Y;
                    return true;
                }
            }
        }
    }
    else                                                                        //Jimmychiu 20250626 : 改變Auto Clean吸取順序
    {
        for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
        {
            for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
            {
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC)
                {
                    iKCol=X;
                    iKRow=Y;
                    return true;
                }
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void SearchCleanKitRowCol(eWhichShuttle iSht)
{
    if((TestIF.iShuttleMode==1 &&
        TestIF.iShuttle_Sel==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //kevin 20170126 (Steven) 關ARM 使用ARM 2
       (IniConfig.bA09_ByArmCloseSite==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //Isaac 20170601 (wei) add
       (CosFunction.bAutoCleanAutoSelIndexArm==true &&
        TestIF.bCleanIndexOtherArm==false &&
        TestIF_File.iShuttleMode==1 &&
        TestIF_File.iShuttle_Sel==1))                                           //JerryYang 20171017 (wei) 修正auto clean只開ARM2發生hang up
    {
        iSht=euShuttle2;
    }

    bool bFlag[8];
//    bool bRowHasIC=false;

    int iKitRow=0, iKitCol=0;
    int iRealRow=0;//, iKitStep=0;
    int iSuckCol=0, iShtRow=0, iShtCol=0;//, iSiteRow=0, iSiteCol=0, &iSuckRow=iPickerOrder;

    ZeroMemory(bFlag, sizeof(bFlag));
    DoInArm_SuckerMap();                                                        //jou 2013-11-06 開關site 吸嘴使用異常修正

    iRealRow=0;
    iSuckCol=0;

    GetNowInShuttleRowCol_All_1Picker(iSht, &iShtRow, &iShtCol, bAutoPlace);

    if(Prod.bInSuckUse[iSht][iShtRow][iSuckCol]==true)
    {
        bFlag[iSuckCol]=(Prod.fInArmSuck4x8[iSht][iShtRow][iShtCol] &&          //Steven 20241008 : fixed [iSuckCol+iKitStep]
                         InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);
//        if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
//            bRowHasIC=true;
    }

//    iShtRow =iSuckRow;
//    iShtCol =iSuckCol+iKitStep;                                               //Steven 20241010 : iSuckCol --> j

    if(InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC)
    {
        iAutoCleanStart=0;
    }

    int iKRow=0,iKCol=0;
    if(SearchCleanKitRowCol(iKRow,iKCol))
    {
        iAutoCleanPickPlateX=iKCol;
        iAutoCleanPickPlateY=iKRow;
        iSuckCol=0;                                                             //Steven 20240918 : fixed for auto clean
        iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(iSuckCol-iAutoCleanStart);
        iKitRow=iAutoCleanPickPlateY;

        if(MOT[MMAutoCleanKit].Tray.Data[iKitCol][iKitRow]==HAS_CLEAN_IC &&
           iKitCol<MOT[MMAutoCleanKit].Tray.XItem)
        {
            bInArmSuckActive[iRealRow][iSuckCol]=bFlag[iSuckCol];
        }
        else
        {
            bInArmSuckActive[iRealRow][iSuckCol]=false;
        }
        return;
    }
}
//------------------------------------------------------------------------------
void SearchCleanKitUpDown(int iRow, eWhichShuttle iSht)
{
    if(USE_PICKER_COUNT==ep1Picker)
    {
        SearchCleanKitRowCol(iSht);
        return;
    }

    if((TestIF.iShuttleMode==1 &&
        TestIF.iShuttle_Sel==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //kevin 20170126 (Steven) 關ARM 使用ARM 2
       (IniConfig.bA09_ByArmCloseSite==1 &&
        TestIF.iAutoClean_SelectArm==1) ||                                      //Isaac 20170601 (wei) add
       (CosFunction.bAutoCleanAutoSelIndexArm==true &&
        TestIF.bCleanIndexOtherArm==false &&
        TestIF_File.iShuttleMode==1 &&
        TestIF_File.iShuttle_Sel==1))                                           //JerryYang 20171017 (wei) 修正auto clean只開ARM2發生hang up
    {
        iSht=euShuttle2;
    }

    if(iInArmType==e9045_2x6_8 &&
       iCloseSiteModeFor2x6<e2x6OneByOne &&                                     //Steven 20241113 : for 2x6 auto clean
       (TestIF_File.iAutoClean_DeveicePices==12 ||
        TestIF_File.iAutoClean_DeveicePices==24))
    {
        if(iShuttleRowKit==1 ||
           iShuttleRowKit==2)
        {
            if(bUse8Picker)
            {
                InArmSuck.SetItemData(0, 3, HAS_NULL_CLEAN_IC);
                InArmSuck.SetItemData(1, 3, HAS_NULL_CLEAN_IC);
            }
            else
            {
                InArmSuck.SetItemData(1, 3, HAS_NULL_CLEAN_IC);
            }
        }
        else if(iShuttleRowKit==3 ||
                iShuttleRowKit==4)
        {
            if(bUse8Picker)
            {
                InArmSuck.SetItemData(0, 0, HAS_NULL_CLEAN_IC);
                InArmSuck.SetItemData(1, 0, HAS_NULL_CLEAN_IC);
            }
            else
            {
                InArmSuck.SetItemData(1, 0, HAS_NULL_CLEAN_IC);
            }
        }
    }

    //AI(ht9045-v899) 20260407: dual-path switch  bUseCKPP selects new class vs old logic
    if(bUseCKPP)
    {
        // ========== NEW PATH: cCleanKitPickPlan ==========                  
        //AI(ht9045-v899) 20260409: recycle CLEAN_FINISH_IC -> HAS_CLEAN_IC before search so returned pads are reusable
        for(int kY=0; kY<MOT[MMAutoCleanKit].Tray.YItem; kY++)
            for(int kX=0; kX<MOT[MMAutoCleanKit].Tray.XItem; kX++)
                if(MOT[MMAutoCleanKit].Tray.Data[kX][kY]==CLEAN_FINISH_IC)
                    MOT[MMAutoCleanKit].SetTraySingleData(kX, kY, HAS_CLEAN_IC);
        DoInArm_SuckerMap();
        g_CKPlan.Init((int)iSht, iRow);
        if(g_CKPlan.Search())
        {
            g_CKPlan.Apply();
            RecordProcess(AnsiString().sprintf(
                "AC_SEARCH [CKPP] iAutoCleanStart=%d iAutoCleanPickPlateX=%d Y=%d active=%d/%d XItem=%d",
                iAutoCleanStart, iAutoCleanPickPlateX,
                iAutoCleanPickPlateY, g_CKPlan.GetActiveCount(), g_CKPlan.GetSlotCount(), MOT[MMAutoCleanKit].Tray.XItem));
        }
        return;
    }

    // ========== OLD PATH: original SearchCleanKitUpDown logic ==========  
    bool bFlag[8];
    bool bRowHasIC=false;
    int iKitRow=0, iKitCol=0;
    int iKitStep=0, iRealRow=1;
    int &iSuckRow=iPickerOrder, iSuckCol=0, iSiteRow=0, iSiteCol=0, iShtRow=0, iShtCol=0;

    ZeroMemory(bFlag, sizeof(bFlag));
    DoInArm_SuckerMap();                                                        //jou 2013-11-06 開關site 吸嘴使用異常修正
    GetInarmSuckRow(iRow,iSuckRow,iKitStep);

    if(IniConfig.bE43AutoCleanUseHotplate ||                                    //ChungHung 20131120 AutoClean use Hotplate1
       bUse8Picker)                                                             //Steven 20201124 : fixed for 2x3 auto clean
    {
        iRealRow=iSuckRow;
    }
    else
    {
        iRealRow=1;
    }

    for(int j=0; j<InArmSuck.iMaxCol; j++)
    {
        iSuckCol=GetAutoCleanPickStep(j);                                       //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
        {
            continue;
        }

        iShtRow=iSuckRow;
        iShtCol=iSuckCol+iKitStep;                                              //Steven 20250917 : iSuckCol+iKitStep --> GetShuttleCol
        iSiteCol=GetShuttleCol(iShtRow, iShtCol);                               //Steven 20250917 : Fixed for 12site auto clean
        if(IsNNMode()==NN_2Row)
        {
            iSiteRow=(iSht==0)?2:0;
        }
        else if(IsNNMode()==NN_1Row)
        {
            iSiteRow=(iSht==0)?1:0;
        }
        else
        {
            iSiteRow=0;
        }

        if(i1x2_4UseACEGPicker==1)                                              //Steven 20230530 : 1x2_4改用Row A
        {
            if(CosFunction.bUseAutoCleanCloseSiteAlsoDo==true)                  //Ifor 20181222 add 新增Auto Clean Close Site 一樣執行
            {
                bFlag[iSuckCol]=(TestIF.iSiteMap[iSiteRow][iSiteCol]>0 &&
                                 InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                    bRowHasIC=true;
            }
            else
            {
                bFlag[iSuckCol]=(Prod.bInSuckUse[iSht][iShtRow][iShtCol] &&     //Steven 20241008 : fixed [iSuckCol+iKitStep]
                                 InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                    bRowHasIC=true;
            }
        }
        else if(Prod.bInSuckUse[iSht][iShtRow][iShtCol]==true)
        {
            {
                if(CosFunction.bUseAutoCleanCloseSiteAlsoDo==true)                  //Ifor 20181222 add 新增Auto Clean Close Site 一樣執行
                {
                    bFlag[iSuckCol]=(TestIF.iSiteMap[iSiteRow][iSiteCol]>0 &&
                                     InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                    if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                        bRowHasIC=true;
                }
                else
                {
                    bFlag[iSuckCol]=(Prod.fInArmSuck4x8[iSht][iShtRow][iShtCol] &&  //Steven 20241008 : fixed [iSuckCol+iKitStep]
                                     InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC);

                    if(InArmSuck.Item[iRealRow][iSuckCol]!=NULL_IC)
                        bRowHasIC=true;
                }
            }
        }
    }

    for(int j=0; j<InArmSuck.iMaxCol; j++)
    {
        iSuckCol=GetAutoCleanPickStep(j);                                       //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
            continue;

        if((iInArmType==e9045_1x3_4 ||                                          //Steven 20241101 : Fixed for 1x3 auto clean
            iInArmType==e9045_1x3_2_14) &&
           TestIF_File.iAutoClean_DeveicePices%3==0)
        {
            if(iKitStep!=0 && iSuckCol==0)
                continue;
        }

        iShtRow =iSuckRow;
        iShtCol =iSuckCol+iKitStep;                                             //Steven 20250917 : iSuckCol+iKitStep --> GetShuttleCol
        iSiteCol=GetShuttleCol(iShtRow, iShtCol);                               //Steven 20250917 : Fixed for 12site auto clean

        if(bRowHasIC==true)                                                     //Steven 20240829 : 判斷是不是第二次吸取
        {
            if((bFlag[iSuckCol] ||                                              //Steven 20240729 : fixed for auto clean pick 2 times from kit
                Prod.bInSuckUse[iSht][iShtRow][iShtCol]) &&                     //Steven 20241010 : iSuckCol --> iShtCol
               InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC)
            {
                iAutoCleanStart=j;
                break;
            }
        }
        else
        {
            if(InArmSuck.Item[iRealRow][iSuckCol]==NULL_IC)
            {
                iAutoCleanStart=j;
                break;
            }
        }
    }
    int iKRow=0,iKCol=0;
    if(SearchCleanKitRowCol(iKRow,iKCol))
    {
        iAutoCleanPickPlateX=iKCol;
        iAutoCleanPickPlateY=iKRow;
        for(int i=0; i<InArmSuck.iMaxCol; i++)
        {
            iSuckCol=GetAutoCleanPickStep(i);                           //Steven 20240918 : fixed for auto clean
            if(iSuckCol==-1)                                            //JerryYang 20250326 : fixed
            {

            }
            else if(i-iAutoCleanStart<0)
            {
                {
                    bInArmSuckActive[iRealRow][iSuckCol]=false;
                    continue;
                }
            }

            iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(i-iAutoCleanStart);
            iKitRow=iAutoCleanPickPlateY;

            if(MOT[MMAutoCleanKit].Tray.Data[iKitCol][iKitRow]==HAS_CLEAN_IC &&
               iKitCol<MOT[MMAutoCleanKit].Tray.XItem)
            {
                bInArmSuckActive[iRealRow][iSuckCol]=bFlag[iSuckCol];
            }
            else
            {
                bInArmSuckActive[iRealRow][iSuckCol]=false;
            }
        }
        return;
    }
}
//==============================================================================
//  移動XY到Clean Kit
//==============================================================================
bool MoveInArmXYPickCleanKit(int iPick, int iShuttleRow, eWhichShuttle iSht)    //ChungHung 20140709 add iSht for SCK CloseSiteByArm Autoclean
{
    int iXPos       =0;
    int iYPos       =0;
    int iMovePitchX =GetXPitchOfCleanKit();
    int iMovePitchY =GetYPitchOfCleanKit();
    int iOffsetPos  =InOfsAutoClean;
    int iSuckRow=0, iSuckCol=0;
    int iKitRow=0, iKitCol=0;
    double dMovePitchX=iMovePitchX;

    if(USE_PICKER_COUNT==ep1Picker)
    {
        dMovePitchX=0;
    }
    else if(USE_PICKER_COUNT==ep16Picker)                                            //基準為第四隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/7.0;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchBb ||                               //基準為第二隻吸嘴 //Steven for HT7080
            USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                       //Ztex 2024.02.24 Add HT-1132
    {
        dMovePitchX=double(iMovePitchX)/3.0;
    }
    else                                                                        //基準為第三隻吸嘴
    {
        dMovePitchX=double(iMovePitchX)/3.0;
    }

    ZeroMemory(bZFlgToCleanKit, sizeof(bZFlgToCleanKit));
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iZPosToCleanKit[i][j]=ZSafePos;
        }
    }

//    if(IniConfig.bE48_ShuttleUse4Offset_Autoclean &&                          //Sam 20240905 : 修正 AutoClean Offset Mark
//       TestIF.bEnableAutoAlignment==false)                                    //KenHsieh 20220923 : add AOA功能開啟不補償
//    {
//        iOffsetPos=InOfsAutoClean;
//    }
//    else
    {
        if(IniConfig.bE43AutoCleanUseHotplate)
        {
            iOffsetPos=InOfsHP1;
        }
        else
        {
            iOffsetPos=InOfsAutoClean;
        }
    }

    if(iPick==bAutoPick)
    {
        if(iPickerOrder==1 || iPickerOrder==3)
            iSuckRow=1;
        else
            iSuckRow=0;

        iKitRow=iAutoCleanPickPlateY;                                           //Steven 20240701 : 修正Auto Clean Kit Row異常
        iKitCol=iAutoCleanPickPlateX;

        if(Special_2X6_Tray_XItem7() && (iShuttleRow==3 || iShuttleRow==4))     //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
        {
            iKitCol=iKitCol-iAutoCleanUseXPitch;
        }
        iSuckCol=GetAutoCleanPickStep(iAutoCleanStart);                         //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
            return true;
    }
    else if(iPick==bAutoPlace)
    {
//        uHPSuckTeam* ppInfo=PlaceToCleanList->ExtractFirstTeam();
        if(PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZMotToPlace)==false)    //RogerYang 20250624 Add HotPlate ErrMessage
        {
            ShowMyMessage(AnsiString().sprintf("%s %s Task=%d", __FUNC__, "No GetHPFirstTeamMotUse", 0));
            return false;
        }
//        PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZMotToPlace);
        PlaceToCleanList->GetHPFirstTeamSuckUse(bCleanZSuckToPlace);

        iSuckCol=-1;
        for(int j=0; j<InArmSuck.iMotCol; j++)                                  //確認第一排最左邊
        {
            if(iSuckCol==-1 &&
               bCleanZSuckToPlace[0][j]==true)
            {
                iSuckRow=0;
                iSuckCol=j;
            }
        }

        if(iSuckCol==-1)                                                        //如果確認第一排最左邊沒有了, 就改找第二排
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                if(iSuckCol==-1 &&
                   bCleanZSuckToPlace[1][j]==true)
                {
                    iSuckRow=1;
                    iSuckCol=j;
                }
            }
        }

        if(iSuckCol==-1)
        {
            return true;
        }
        iKitRow=InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol];                   //Steven 20240701 : 修正Auto Clean Kit Row異常
        iKitCol=InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol];

        if(Special_2X6_Tray_XItem7())                                           //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
        {
            if(iKitCol>0)
                iKitCol=iKitCol-iAutoCleanUseXPitch*4;
        }
    }

    int iYVariable  =GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iInXPToSht[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    iXPos=Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+
          dMovePitchX*(0+iInArmXBase-iSuckCol)+                                 //A到基準, 減去使用的吸嘴, 就是使用中吸嘴對A吸嘴的距離
          TestIF_File.dAutoClean_XPitch*iKitCol+                                //使用的吸嘴移動到要吸取的位置
          HotplatlXOffset;                                                      //kevin 20150720 kit or TRAY autoclean X offset

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                            //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(iSuckRow==0)
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch-iMovePitchY;  //JerryYang 20240821 : fix auto clean點位錯誤
        else
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch;              //JerryYang 20240821 : fix auto clean點位錯誤
    }
    else
    {
        if(bUse8Picker==false)                                                  //Steven 20201014 : 整合8吸嘴auto clean
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch+iMovePitchY;
        else if(iSuckRow==1)
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch+iMovePitchY;
        else
            iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iKitRow*TestIF_File.dAutoClean_YPitch;
    }

    iYPos+=HotplatlYOffset;                                                     //kevin 20150720 kit or TRAY autoclean Y offset

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF_File.bEnableAutoAlignment)      //KenHsieh 20211214 : AOA add AutoClean
        CheckInArmXYScaleByAutoTeach(iXPos, iYPos, AOA_AutoClean);
    iInXPos=iXPos;
    iInYPos=iYPos;

    if(InArmContinuousMove_9045(iXPos, iYPos, iInXPToSht, iYVariable, bZFlgToCleanKit, iZPosToCleanKit, false))
    {
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
bool TrayHasCleanIC()
{
    bool bFlag=false;

    if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                            //Clean Kit
    {
        for(int Y=0; Y<TestIF_File.iAutoClean_YDivision; Y++)
            for(int X=0; X<TestIF_File.iAutoClean_XDivision; X++)
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC)
                    return true;

        if(bFlag==false)                                                        //Steven 20160218 : 避免Auto Clean 的Count不同導致Hang Up
        {
            for(int Y=0; Y<TestIF_File.iAutoClean_YDivision; Y++)
                for(int X=0; X<TestIF_File.iAutoClean_XDivision; X++)
                    if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC)    //Steven 20211201 : 造成2x4關整排時候Hang up, 改回原本的
                        MOT[MMAutoCleanKit].SetTraySingleData(X, Y, HAS_CLEAN_IC);
        }
    }
    return bFlag;
}
//------------------------------------------------------------------------------
int TrayHasCleanICCount()                                                       //wei 2015090
{
    int iFlag=0;

    if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit ||
       IniConfig.bE43AutoCleanUseHotplate==true)                                //Clean Kit
    {
        for(int Y=0; Y<TestIF.iAutoClean_YDivision; Y++)
        {
            for(int X=0; X<TestIF.iAutoClean_XDivision; X++)
            {
                if(MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_CLEAN_IC ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==CLEAN_FINISH_IC ||
                   MOT[MMAutoCleanKit].Tray.Data[X][Y]==HAS_NULL_CLEAN_IC)      //Steven 20210312 : 修正執行第二次auto clean時,出現Clean Pad數量錯誤問題
                {
                    iFlag++;
                }
            }
        }
    }
    return iFlag;
}
//------------------------------------------------------------------------------
bool CheckCleaningCount()
{
    int iXpos=-1, iYpos=0, iCleanCount=0;
    int iXItem;
    bool bFlag=false;
    if(CosFunction.bCleanCountAlarmByMin)                                       //Jimmychiu 20260212 : Gigas Clear alarms based on minimum usage count
    {
        iCleanCount=SearchCleanNum();                                           //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
        if(iCleanCount<TestIF.iAutoClean_AlarmCount)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    iXItem=MOT[MMAutoCleanKit].Tray.XItem;

    for(int i=0; i<TestIF.iAutoClean_DeveicePices; i++)
    {
        if(TestIF.iTestMode==QualSite2X2 &&                                     //jou 20210712 : 修正 QualSite2X2 Tray pitch 小於2666 hang up
           bCleanKitPitchLess4000==true &&
           TestIF.iAutoClean_DeveicePices==4 &&
           TestIF.iAutoClean_XDivision==8)
        {
            iXpos=i*2;
        }
        else
        {
            iXpos++;
        }

        if(iXpos>=iXItem)
        {
            iXpos=0;
            iYpos++;
        }

        if((TestIF_File.iTestMode==QualSite1X4 ||                               //Steven 20220218 : 修正1x4 auto clean for 不同X-Pitch
            TestIF_File.iTestMode==_8Site2X4N) &&
            TestIF.dAutoClean_XPitch<1333 &&                                    //Steven 20240305 : 修正Auto Clean Pitch判斷
           iInArmType!=e9045_1x4_1_Ac)
        {
            if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]!=NULL_IC)            //Steven 20210127 : 修正auto clean count到了不會alarm
            {
                iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                if(iCleanCount<0)                                               //Steven 20210127 : 修正auto clean count到了不會alarm
                    iCleanCount=0;
                if(iCleanCount<TestIF.iAutoClean_AlarmCount)                    //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
                {
                    bFlag=true;
                    break;
                }
            }
        }
        else if((TestIF.iTestMode!=DualSite     &&
                 TestIF.iTestMode!=QualSite2X2  &&
                 TestIF.iTestMode!=TriSite1X3   &&
                 TestIF.iTestMode!=_6Site2X3N)  &&                              //Steven 20220425 : 2X3NN Mode
                (iAutoCleanUseXPitch==2 || iAutoCleanUseXPitch==4) &&
                TestIF_File.iAutoClean_DeveicePices<=4 &&                       //JerryYang 20200701 修正auto clean次數到達時沒有發出alarm的問題
                TestIF_File.iAutoClean_XDivision>4)                             //Steven 20240305 : 修正Auto Clean Pitch判斷
        {
            if(iXpos+iYpos*MOT[MMAutoCleanKit].Tray.XItem<TestIF.iAutoClean_DeveicePices)
            {
                if(InArmSuck.iModeX==9 &&
                   TestIF_File.iAutoClean_DeveicePices==4 &&
                   iCloseSiteStep_2x8==3)                                       //KevinC 20260126 : 針對矽品中科增加特殊關Site判斷
                {
                    iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                    if(iCleanCount<0)
                        iCleanCount=0;
                    if(iCleanCount<TestIF.iAutoClean_AlarmCount)
                    {
                        bFlag=true;
                        break;
                    }
                }
                else if(MOT[MMAutoCleanKit].Tray.Data[iXpos*2][iYpos]==HAS_CLEAN_IC ||
                        MOT[MMAutoCleanKit].Tray.Data[iXpos*2][iYpos]==CLEAN_FINISH_IC)
                {
                    iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos*2][iYpos+1].c_str());
                    if(iCleanCount<0)                                           //Steven 20210127 : 修正auto clean count到了不會alarm
                        iCleanCount=0;
                    if(iCleanCount<TestIF.iAutoClean_AlarmCount)                //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
                    {
                        bFlag=true;
                        break;
                    }
                }
            }
        }
        else
        {
            if(MOT[MMAutoCleanKit].Tray.Data[iXpos][iYpos]!=NULL_IC)            //Steven 20210127 : 修正auto clean count到了不會alarm
            {
                iCleanCount=atoi(fMain->AutoCleanStringGrid->Cells[iXpos][iYpos+1].c_str());
                if(iCleanCount<0)                                               //Steven 20210127 : 修正auto clean count到了不會alarm
                    iCleanCount=0;
                if(iCleanCount<TestIF.iAutoClean_AlarmCount)                    //JerryYang 20170801 (wei) iCleanCount為0也要回傳true
                {
                    bFlag=true;
                    break;
                }
            }
        }
    }

    return bFlag;
}
//------------------------------------------------------------------------------
bool CheckInArmSuckFromCleanKitICFallDown(bool bRetry)
{
    bool bIsSuckICFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL]={false}, bHasErr=false;
    AnsiString ErrPart="";
    int ret=0;

    if(LastSet.iRealDummy==DUMMY)
        return false;

    if(LastSet.iRealDummy==REALLY)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(InArmSuck.Suck[i][j].Enable       &&                         //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                   InArmSuck.Suck[i][j].SenUsing!="" &&
                   InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                   InArmSuck.Item[i][j]!=NULL_IC)                               //wei 20160130
                {
                    if(InArmSuck.Suck[i][j].GetStatus()==false)
                    {
                        bIsSuckICFallDown[i][j]=true;
                        ErrPart+=InArmSuck.Suck[i][j].sName;
                        bHasErr=true;
                    }
                    else
                    {
                        bIsSuckICFallDown[i][j]=false;
                    }
                }
                else
                {
                    bIsSuckICFallDown[i][j]=false;
                }
            }
        }
    }

    if(ret)                                                                     //Debug用
    {
        bIsSuckICFallDown[1][0]=true;
        ErrPart+=InArmSuck.Suck[1][0].sName;
        bHasErr=true;
    }

    if((bUse8Picker ||
        IniConfig.bE43AutoCleanUseHotplate) &&
       bRetry)                                                                  //JerryYang 20160503 第二排吸取後,就不能讓第一排掉料retry
    {
        if(bIsSuckICFallDown[0][0] || bIsSuckICFallDown[0][1] || bIsSuckICFallDown[0][2]|| bIsSuckICFallDown[0][3])
        {
            if(InArmSuck.Item[1][0] && InArmSuck.Item[1][1] && InArmSuck.Item[1][2] && InArmSuck.Item[1][3])
            {
                for(int i=0; i<MAX_ARM_Col ; i++)
                    bIsSuckICFallDown[0][i]=false;

                if(bIsSuckICFallDown[1][0]==false && bIsSuckICFallDown[1][1]==false && bIsSuckICFallDown[1][2]==false && bIsSuckICFallDown[1][3]==false)
                {
                    ErrPart="";
                    bHasErr=false;
                }
            }
        }
    }

    if(bHasErr)
    {
        if(bRetry)
            ret=ShowErrorMessage("JAM0128", K_RETRY, MInArmX, false, ErrPart);  //JerryYang 20160511 JAM0126->JAM0128,將IC及Clean pad相關的alarm code分開
        else
            ret=ShowErrorMessage("JAM0128", K_SKIP, MInArmX, false, ErrPart);   //JerryYang 20160511 JAM0126->JAM0128,將IC及Clean pad相關的alarm code分開

        if(SoftStop)
            fMain->Pause("CheckInArmSuckFromCleanKitICFallDown");

        if(bRetry)
        {
            if(ret==K_RETRY)
            {
                for(int i=0; i<2; i++)                                          //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])
                        {
                            MOT[MMAutoCleanKit].SetTraySingleData(InArmSuck.iAutoCleanRecX[i][j], InArmSuck.iAutoCleanRecY[i][j], HAS_CLEAN_IC);    //JerryYang 20160210 修正auto clean 使用hot plate hang up問題
                            InArmSuck.SetItemData(i, j, NULL_IC);
                            bInArmSuckActive[i][j]=true;                        //JerryYang 20160404 [i][j]-->[1][j],修正Drop error按retry後沒有重吸的問題
                            iAutoCleanPickPlateY=InArmSuck.iAutoCleanRecY[i][j]; //JerryYang 20160428 修正掉料後hang up,記錄掉落的IC是哪個 Kit Y
                        }
                    }
                }
                return true;
            }
        }
        else
        {
            if(ret==K_SKIP)
            {
                for(int i=0; i<2; i++)                                          //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<MAX_ARM_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])
                        {
                            InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);     //wei 20160130
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void DoPlaceToKitSwapData(bool bPick, int iIC_Type, int iSuckRow, int iSuckCol, int iKitRow, int iKitCol)     //Steven 20170109 : 將放料到Kit資料交換改成Function
{
    if(bPick==bAutoPick)
    {
        int iCleanListType=-1;
        if(iIC_Type==HAS_CLEAN_IC)
        {
            iCleanListType=iIC_Type;
        }
        InspectInArmPosition(MMAutoCleanKit, iSuckRow, iSuckCol, iKitRow, iKitCol, true);
        PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, iCleanListType);      //Jimmychiu 20230417 : Record the position after placing the IC
        MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, iIC_Type);
        AnsiString sTime=AnsiString().sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        CleanKitRecord[iKitRow][iKitCol]=sTime;                         //Sam 20230619 : 新增 Clean吸放時間 Log
        bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] =false;
        InArmSuck.SetItemData(iSuckRow, iSuckCol, iIC_Type);
        InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol]  =iKitCol;
        InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol]  =iKitRow;
        InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitCol, iKitRow, iIC_Type); //Sam 20230616 : Add Auto Clean Record
    }
}
//==============================================================================
bool PickFromCleanKit(int iRowKit)
{
    bool flag1=true;
    int iSuckRow=0, iSuckCol=0, iKitRow=0, iKitCol=0;
//    int iSuckColStart=0;
//    if(iInArmType==e9045_1x1_1 &&
//       Prod.bSingleUseOtherSuck)
//        iSuckColStart=1;
//    int iPickCount=GetAutoCleanPickCount()+iSuckColStart;

    AnsiString sTime="";                                                        //Sam 20230619 : 新增 Clean吸放時間 Log
    GetTimeInfo();

    if(bUse8Picker)                                                             //Steven 20201014 : 整合8吸嘴auto clean
    {
        if(iRowKit==1 || iRowKit==3)
            iSuckRow=0;
        else if(iRowKit==2 || iRowKit==4)
            iSuckRow=1;
    }
    else
    {
        iSuckRow=1;
    }

    //AI(ht9045-v899) 20260408: plan-driven pick - iterate plan slots instead of hardcoded j=0..3
    if(bUseCKPP && g_CKPlan.Found())
    {
        const TPickPlan &plan = g_CKPlan.GetPlan();
        for(int i = 0; i < plan.iSlotCount; i++)
        {
            const TPickSlot &slot = plan.Slots[i];
            if(slot.iPhysical < 0) continue;
            iSuckCol = slot.iPhysical;
            iKitCol  = slot.iKitCol;
            iKitRow  = plan.iKitRow;

            if(iKitCol < 0 || iKitRow < 0 || iKitCol >= 50 || iKitRow >= 50)
                continue;

            if(InArmSuck.Suck[iSuckRow][iSuckCol].Error == true)
            {   // error already flagged, skip
            }
            else if(slot.bActive && InArmSuck.Item[iSuckRow][iSuckCol] == NULL_IC)
            {
                if(InArmSuck.Suck[iSuckRow][iSuckCol].Suck())
                {
                    if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))
                    {
                        if(iKitCol >= iAutoCleanUseXPitch)
                            iKitCol = iKitCol - iAutoCleanUseXPitch;
                    }
                    InspectInArmPosition(MMAutoCleanKit, iSuckRow, iSuckCol, iKitRow, iKitCol, true);
                    PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, HAS_CLEAN_IC);
                    bInArmSuckActive[iSuckRow][iSuckCol] = false;
                    MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                    sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                    CleanKitRecord[iKitRow][iKitCol] = sTime;
                    bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] = false;
                    InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_CLEAN_IC);
                    InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol] = iKitCol;
                    InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol] = iKitRow;
                    InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitRow, iKitCol, HAS_CLEAN_IC);
                }
                else if(InArmSuck.Suck[iSuckRow][iSuckCol].Error == false)
                {   flag1 = false; }
            }
            else if(!slot.bActive && InArmSuck.Item[iSuckRow][iSuckCol] == NULL_IC)
            {
                // inactive slot: mark phantom (pad consumed but not picked)
                if(slot.bHasPad && iKitCol < MOT[MMAutoCleanKit].Tray.XItem)
                {
                    if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))
                        iKitCol = iKitCol + iAutoCleanUseXPitch * 3;
                    PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, -1);
                    MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                    sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                    CleanKitRecord[iKitRow][iKitCol] = sTime;
                    bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] = false;
                    InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_NULL_CLEAN_IC);
                    InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol] = iKitCol;
                    InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol] = iKitRow;
                    InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                }
            }
        }
        return flag1;
    }

    // ========== OLD PATH: original PickFromCleanKit ==========
    for(int j=0; j<4; j++)
    {
        iSuckCol=GetAutoCleanPickStep(j);                                       //Steven 20240918 : fixed for auto clean
        if(iSuckCol==-1)
            continue;
        if(USE_PICKER_COUNT==ep1Picker && j>=1)
            continue;

        iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(j-iAutoCleanStart);
        iKitRow=iAutoCleanPickPlateY;

        if(iKitCol<0 || iKitRow<0 ||
           iKitCol>=50 || iKitRow>=50)
        {
            sTime="";
            continue;                                                           //Steven 20240729 : fixed for auto clean pick 2 times from kit
//            ShowMyMessage("CleanKitRecord error1", "CleanKitRecord 錯誤0");
//            return false;
        }

        if(InArmSuck.Suck[iSuckRow][iSuckCol].Error==true)                      //Steven 20220210 : fixed for Auto Clean吸料異常
        {
           // bInArmSuckActive[1][j]=false;                                     //Sam 20230619 : 需要 Mark 不然重吸可能會失敗。
        }
        else if(bInArmSuckActive[iSuckRow][iSuckCol] &&
                InArmSuck.Item[iSuckRow][iSuckCol]==NULL_IC)                    //ChungHung 20131120 AutoClean use Hotplate1
        {
            if(InArmSuck.Suck[iSuckRow][iSuckCol].Suck())                       //ChungHung 20131120 AutoClean use Hotplate1
            {
                if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))     //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
                {
                    if(iKitCol>=iAutoCleanUseXPitch)
                    {
                        iKitCol=iKitCol-iAutoCleanUseXPitch;
                    }
                }
                InspectInArmPosition(MMAutoCleanKit, iSuckRow, iSuckCol, iKitRow, iKitCol, true);
                PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, HAS_CLEAN_IC);      //Jimmychiu 20230417 : Record the position after placing the IC
                bInArmSuckActive[iSuckRow][iSuckCol] =false;
                MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                CleanKitRecord[iKitRow][iKitCol]=sTime;                         //Sam 20230619 : 新增 Clean吸放時間 Log
                bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] =false;
                InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_CLEAN_IC);
                InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol]  =iKitCol;
                InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol]  =iKitRow;
                InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitRow, iKitCol, HAS_CLEAN_IC);  //Sam 20230616 : Add Auto Clean Record
            }
            else if(InArmSuck.Suck[iSuckRow][iSuckCol].Error==false)
            {
                flag1=false;
            }
        }
        else if(bInArmSuckActive[iSuckRow][iSuckCol]==false &&
                InArmSuck.Item[iSuckRow][iSuckCol]==NULL_IC)
        {
            //AI(ht9045-v899) 20260416: revert phantom-marking skip per Gigas feedback - V898 did not have this guard
            if(MOT[MMAutoCleanKit].Tray.Data[iKitCol][iKitRow]==HAS_CLEAN_IC &&
               (iKitCol<MOT[MMAutoCleanKit].Tray.XItem))
            {
                if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))     //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
                {
                    iKitCol=iKitCol+iAutoCleanUseXPitch*3;
                }
                PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, -1);      //Jimmychiu 20230417 : Record the position after placing the IC
                MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                CleanKitRecord[iKitRow][iKitCol]=sTime;                         //Sam 20230619 : 新增 Clean吸放時間 Log
                bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] =false;
                InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_NULL_CLEAN_IC);
                InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol]  =iKitCol;
                InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol]  =iKitRow;
                InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitCol, iKitRow, HAS_NULL_CLEAN_IC); //Sam 20230616 : Add Auto Clean Record
            }
        }
    }
    return flag1;
}
//------------------------------------------------------------------------------
//Steven 20130620 : 避免Clean Count到的時候，被關Arm導致Hang Up，回傳值從bool改為int
// 0 : Not finish
// 1 : Finish
// 2 : Clean Count Alarm
//------------------------------------------------------------------------------
//從Clean Kit吸取clean pad
int DoAutoCleanPickfromCleanKit(eWhichShuttle iSht, bool Restart)
{
    int iResult=0, iXPos, iYPos;
    int &Task=iAutoCleanPickFromCleanKitStageTask, ret=0, iContectCount;
    bool flag=false;
//    int iPickCount=GetAutoCleanPickCount();
    static int iRetryCT=0;
    #ifdef DEBUG_AUTO_CLEAN
    static int iOldTask=-1;
    #endif

    static int iSuckRow=0;                                                      //ChungHung 20131120 AutoClean use Hotplate1   //ChungHung 20140317 alter 改為靜態變數

//    static int iShuttleRowKit=0;
    int  iKitRow, iKitCol;
    bool bIsSuckICFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL]={false};
    bool bHasDuplicateErr=false;
    AnsiString ErrPart="";
    AnsiString Message;
    int iStatus=0;
    int iCleanPadCount=0;
    int iSuckCol=0;
    bool bPauseWhenPick=false;

    QueueTaskList[40].CheckTaskChange();                                        //Steven 20220218 : Auto clean記錄Task變化

    if(Restart)
    {
        iAutoCleanNum=1;
        Task=1;
        #ifdef DEBUG_AUTO_CLEAN
            iOldTask=1;
            Message.sprintf("DoAutoCleanPickfromCleanKit initial task");
            fMain->AddAutoCleanMessage(Message);
        #endif
        return iResult;
    }

    #ifdef DEBUG_AUTO_CLEAN
    if(iOldTask!=Task)
    {
        Message.sprintf("DoAutoCleanPickfromCleanKit %d, %d, Go to Task, %d", iSht, iOldTask, Task);
        fMain->AddAutoCleanMessage(Message);
        iOldTask=Task;
    }
    #endif

    switch(Task)
    {
        case 1:
            MOT[MInArmX].PCIL132_StopMotor();
            MOT[MInArmY].PCIL132_StopMotor();
            if(MoveInArmZToPlateSafe(Task))                                     //In arm z軸移至安全位置
            {
                InOutArmSuckActiveSet();                                        //重置in arm吸嘴狀態=false
                Task=10;
            }
            break;
        case 10:
            if(TrayHasCleanIC())                                                //是否有CLEAN PAD
            {
                if(CheckCleaningCount()==false)                                 //Steven 20210127 : 修正auto clean count到了不會alarm //Steven 20220114 : 往上移動, 避免shuttle先被填入HAS_NULL_CLEAN_IC
                {
                    fCleaning->bResetCleanCount=true;
                    fShowBinSelect->btnCleanReset->Enabled=true;
                    Task=30;
                    break;
                }

                if(iInArmType==e9045_1x4_2_14)
                {
                    if(iSht==euShuttle1)
                        ptrInSHT=&FLCarryKit;
                    else
                        ptrInSHT=&BLCarryKit;
                    iShuttleRowKit=GetNeedSuckActive_1x4_2_14((int)iSht, bAutoPlace);
                }
                else if(USE_PICKER_COUNT==ep1Picker)
                {
                    iShuttleRowKit=GetNowSiteKitMode_All_1Pick(iSht, bAutoPlace);
                }
                else
                {
                    iShuttleRowKit=GetShuttleState(iSht, bAutoPlace);
                }
                Task=20;

                if(USE_PICKER_COUNT==ep1Picker)
                {
                    if(iSht==euShuttle1 && IsFLCarrKitAllHasIC())
                    {
                        Task=3300;
                    }
                    else if(iSht==euShuttle2 && IsBLCarrKitAllHasIC())
                    {
                        Task=3300;
                    }
                }
                else
                {
                    if(iShuttleRowKit==0)                                           //kevin 20180426 shuttle full (close all site arm)
                        Task=3300;
                }
            }
            else
            {
                if(InArmSuck.HasIC())                                           //已被In arm吸起來
                {
                    //AI(ht9045-v899) 20260408: kit empty but arm has partial pick - skip 3000-3100 loop, go finish (S6)
                    if(bUseCKPP)
                    {
                        PlaceToCleanList->AddHPSuckGroup();
                        Task=3300;
                    }
                    else
                    {
                        Task=3000;
                    }
                }
                else
                {
                    if(CheckCleaningCount())                                    //Clean Kit 沒有 clean pad 可以吸
                    {
                        iCleanPadCount=TrayHasCleanICCount();
                        //AI(ht9045-v899) 20260407: prevent hang when no clean pads remain
                        if(iCleanPadCount==0)
                        {
                            /*RecordProcess(AnsiString().sprintf("AC_NOPAD Kit empty XDiv=%d YDiv=%d XItem=%d YItem=%d",
                                TestIF_File.iAutoClean_XDivision, TestIF_File.iAutoClean_YDivision,
                                MOT[MMAutoCleanKit].Tray.XItem, MOT[MMAutoCleanKit].Tray.YItem));*/
                            Task=1;
                            iResult=1;
                        }
                        else if(CUSTOMER_CODE==CC_KYEC_LEE)
                        {
                            ShowMyMessage("Clean Pad Count Different Site Count", "End Auto Clean");
                        }
                    }
                    else
                    {
                        fCleaning->bResetCleanCount=true;
                        fShowBinSelect->btnCleanReset->Enabled=true;
                        Task=30;
                    }
                }
            }
            break;
        case 20:                                                                //Steven 20160630 : 分開避免In arm在Auto Clean時, Index alarm讓位後,回來出現異常
            SearchiAutoCleanNum();                                              //Steven 20171212 (Wei) : 確認目前正要吸取的的Pad位置
            SearchCleanKitUpDown(iShuttleRowKit, iSht);                         //In Arm Z要不要下去吸或放 bInArmSuckActive[i][j] //ChungHung 20140709 add iSht for SCK CloseSiteByArm Autoclean
            //AI(ht9045-v899) 20260408: safety net - if CKPP plan not found, return pick complete
            if(bUseCKPP && !g_CKPlan.Found())
            {
                //RecordProcess(AnsiString().sprintf("AC_PICK20_SKIP no plan iShuttleRowKit=%d iSht=%d", iShuttleRowKit, (int)iSht));
                PlaceToCleanList->AddHPSuckGroup();
                Task = 1;
                iResult = 1;
                break;
            }
            Task=21;
            //AI(ht9045-v899) 20260407: AC debug log - PickfromCleanKit Task20 result
           /* RecordProcess(AnsiString().sprintf("AC_PICK20 iShuttleRowKit=%d iSht=%d iAutoCleanStart=%d iAutoCleanPickPlateX=%d Y=%d iAutoCleanNum=%d",
                iShuttleRowKit, (int)iSht, iAutoCleanStart, iAutoCleanPickPlateX, iAutoCleanPickPlateY, iAutoCleanNum));*/
        case 21:
            if(MoveInArmXYPickCleanKit(bAutoPick, iShuttleRowKit, iSht))        //ChungHung 20140709 add iSht for SCK CloseSiteByArm Autoclean
            {
                MoveInOutArmZToKitPickPlace(bAutoPick, true, iSht, iShuttleRowKit);  //ChungHung 20150303 add iSht for Hotplate AutoClean //ChungHung 20131120 AutoClean use Hotplate1
                Task=200;
            }
            break;
        case 30:
            if(MoveInArm2XYToShuttle2Wait())                                    //Steven 20130620 : 避免Clean Count到的時候，被關Arm導致Hang Up，回傳值從bool改為int
            {
                if(CUSTOMER_CODE==CC_JCET)                                      //JerryYang 20170801 凌中心要求清掉clean pad使用次數才能繼續跑
                {
                    if(CheckCleaningCount()==false)
                    {
                        ShowErrorMessage("WAR1922", K_RETRY, MMAutoCleanKit);
                        iAutoCleanAlarm=1;                                      //Ifor 20180727 (wei) ： Auto Clean Clean Count > Alarm Count 0:正常 1: Alarm 2:Clean Count
                    }
                    else
                    {
                        Task=10;
                    }
                }
                else
                {
                    ShowErrorMessage("WAR1922", K_RETRY, MMAutoCleanKit);
                    iAutoCleanAlarm=1;                                          //Ifor 20180727 (wei) ： Auto Clean Clean Count > Alarm Count 0:正常 1: Alarm 2:Clean Count
                    iResult=2;
                    if(bUse_NewAutoCleanForm)                                   //kevin 20150525  需開門
                    {
                        bChangeCleanPad=true;
                        Task=31;
                    }
                    else
                    {
                        Task=10;                                                //Steven 20211217 : 修正無法清除Clean Pad數量
                    }
                }
            }
            break;
        case 31:
            if(bUse_NewAutoCleanForm)                                           //kevin 20150525  需開門
            {
                ShowErrorMessage("WAR1922", K_RETRY, MMAutoCleanKit);
                iAutoCleanAlarm=1;                                              //Ifor 20180727 (wei) ： Auto Clean Clean Count > Alarm Count 0:正常 1: Alarm 2:Clean Count
                if(bChangeCleanPad)
                    return false;
            }
            break;
        case 200:
            iXPos=MOT[MInArmX].ReadPos();
            iYPos=MOT[MInArmY].ReadPos();                                       //Frank 20190812 :Fix Auto clean head 跳舞問題
            if(iXPos>iInXPos+20 || iYPos>iInYPos+20 ||
               iXPos<iInXPos-20 || iYPos<iInYPos-20)
            {
                Message.sprintf("Auto Clean position error!! iXPos=%d, iInXPos=%d, iYPos=%d, iInYPos=%d", iXPos, iInXPos, iYPos, iInYPos);
                RecordProcess(Message);
                Task=21;
            }
            else
            {
                if(MoveInOutArmZToKitPickPlace(bAutoPick, false, iSht, iShuttleRowKit))  //ChungHung 20150303 add iSht for Hotplate AutoClean  //ChungHung 20131120 AutoClean use Hotplate1
                {
                    if(bPauseWhenPick  ||
                       (AccessLevel>=1 ||
                        CosFunction.bOPCanPressStepAndTStart) &&                //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                        Sen[SnRKManualTStart].IsOn())
                    {
                        bEnterOffset=false;
                        fMain->Pause("DoAutoCleanPickfromCleanKit");
                        Task=220;

                    }
                    else
                    {
                        InArmSuck.ResetAll();                                   //Steven 20160323 : 避免未開啟真空
                        PlaceToCleanList->UpdateHPSuckGroup(0, iAutoCleanPickPlateY, iAutoCleanPickPlateX, iSht, iShuttleRowKit);
                        Task=1000;
                    }
                }
            }
            break;
        case 220:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=21;
            }
            else
            {
                MoveInOutArmZToKitPickPlace(bAutoPick, true, iSht, iShuttleRowKit);  //ChungHung 20150303 add iSht for Hotplate AutoClean //ChungHung 20131120 AutoClean use Hotplate1
                Task=200;
            }
            break;
        case 300:
            if(MoveInOutArmZToKitPickPlace(bAutoPick, false, iSht, iShuttleRowKit)) //ChungHung 20150303 add iSht for Hotplate AutoClean //ChungHung 20131120 AutoClean use Hotplate1
            {
                InArmSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=1000;
            }
            break;
        case 1000:
            flag=PickFromCleanKit(iShuttleRowKit);

            if(flag==false)
                break;

            if(bUse8Picker)                                                     //Steven 20220210 : fixed for Auto Clean吸料異常
            {
                if(iShuttleRowKit==1 || iShuttleRowKit==3)
                    iSuckRow=0;
                else if(iShuttleRowKit==2 || iShuttleRowKit==4)
                    iSuckRow=1;
            }
            else
            {
                iSuckRow=1;
            }

            for(int j=0; j<4; j++)
            {
                if(InArmSuck.Suck[iSuckRow][j].Error)                           //ChungHung 20131120 AutoClean use Hotplate1
                {
                    Task=1050;
                    return iResult;
                }
            }

            if((iInArmType==e9045_1x3_4 ||                                      //Steven 20241101 : Fixed for 1x3 auto clean
                iInArmType==e9045_1x3_2_14) &&
               TestIF_File.iAutoClean_DeveicePices%3==0)
            {
                if(iShuttleRowKit==3 || iShuttleRowKit==4)
                {
                    InArmSuck.SetItemData(iSuckRow, 0, HAS_NULL_CLEAN_IC);
                }
            }

            iRetryCT=0;

            if(fCleaning->b1x2SiteAbClosePutDummy)                              //Steven 20180903 : 1x2 close Ab Auto Clean //Steven 20190509 : Fixed
            {
                SetShuttleIcForSpecialMode(iSht, HAS_NULL_CLEAN_IC);
            }

            Task=3000;
            break;
        case 1050:
            if(MoveInArmZToPlateSafe(Task))
            {
                bPickFromLoader=false;                                          //kevin 20180417 add 安全位置讓位
                iRetryCT++;
                Task=1100;
            }
            break;
        case 1100:
            if(iRetryCT>ArmSpeed[InArm].iRetryCT)                               //Steven 20120109 : 整合成一次Skip多顆
            {
                ErrPart=" ";
                bHasDuplicateErr=false;
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(bCleanKitSuckDuplicateErr[i][j])
                            bHasDuplicateErr=true;

                        if(InArmSuck.Suck[i][j].Error)
                        {
                            bCleanKitSuckDuplicateErr[i][j]=true;
                            ErrPart+=InArmSuck.Suck[i][j].sName;
                            bIsSuckICFallDown[i][j]=true;
                        }
                        else
                        {
                            bCleanKitSuckDuplicateErr[i][j]=false;
                        }
                    }
                }

                ret=0;
                if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
                   CUSTOMER_CODE==CC_KYEC_LEE)                                  //kevin 20150623
                    ret=ShowErrorMessage("JAM0110", K_RETRY|K_SKIP, MInArmX, bHasDuplicateErr, ErrPart); //Device pick-up error on Clean Kit
                else
                    ret=ShowErrorMessage("JAM0110", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart); //Device pick-up error on Clean Kit

                if(ret==K_SKIP)                                                 //kevin 20150623
                {
                    bAutoCleanCheckOpenDoor=true;                               //kevin 20121022 掉料開門需開門
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(iInArmType==e9045_1x1_1 &&
                           Prod.bSingleUseOtherSuck)
                            iSuckCol=1;
                        else
                            iSuckCol=j;

                        iKitRow=iAutoCleanPickPlateY;
                        iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(j-iAutoCleanStart);

                        if(bIsSuckICFallDown[iSuckRow][iSuckCol])
                        {
                            MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);
                            InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_NULL_CLEAN_IC);         //wei 20160130
                            InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol]=iKitCol;                 //ChungHung 20131120 AutoClean use Hotplate1
                            InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol]=iKitRow;                 // 掉料記錄TRAY位置
                            bInArmSuckActive[iSuckRow][iSuckCol]=false;                           //kevin 20150701
                            bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol]=false;
                        }
                    }
                    Task=1101;
                }
                else if(ret==K_RETRY)                                           //Steven 20220210 : fixed for Auto Clean吸料異常
                {
                    for(int i=0; i<InArmSuck.iMaxRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            if(InArmSuck.Suck[i][j].Error)
                            {
                                InArmSuck.Suck[i][j].Reset();
                                bInArmSuckActive[i][j]=true;
                            }
                        }
                    }

                    if(IniConfig.bInOutArmCanPushHome)
                        Task=1102;
                    else
                        Task=1101;
                }
                else if(ret==K_HOME)
                {
                    Task=1101;
                }
            }
            else
            {
                Task=21;
            }
            break;
        case 1101:
            SetInArmHome();
            Task=1102;
            break;
        case 1102:
            iRetryCT=0;
            Task=21;
            break;
        case 3000:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(CheckInArmSuckFromCleanKitICFallDown(true))
                {
                    iRetryCT=0;
                    Task=300;
                    break;
                }

                Task=3100;

                if(bUse8Picker)                                                 //Steven 20201014 : 整合8吸嘴auto clean
                {
                    if(iShuttleRowKit==1 || iShuttleRowKit==3)
                        iSuckRow=0;
                    else if(iShuttleRowKit==2 || iShuttleRowKit==4)
                        iSuckRow=1;
                }
                else
                {
                    iSuckRow=1;
                }
            }
            break;
        case 3100:
            //AI(ht9045-v899) 20260408: unified plan-driven contact count + completion check (S5)
            if(bUseCKPP && g_CKPlan.Found())
            {
                const TPickPlan &plan = g_CKPlan.GetPlan();
                bool bPickIncomplete = false;
                for(int i = 0; i < plan.iSlotCount; i++)
                {
                    const TPickSlot &slot = plan.Slots[i];
                    if(slot.iPhysical < 0) continue;
                    // contact counting: only for slots that have IC
                    if(InArmSuck.Item[iSuckRow][slot.iPhysical] == HAS_CLEAN_IC ||
                       InArmSuck.Item[iSuckRow][slot.iPhysical] == HAS_NULL_CLEAN_IC)
                    {
                        iKitRow = iAutoCleanPickPlateY + 1;
                        iKitCol = slot.iKitCol;
                        iContectCount = atoi(fMain->AutoCleanStringGrid->Cells[iKitCol][iKitRow].c_str());
                        iContectCount++;
                        SetAutoCleanStringGrid(iKitCol, iKitRow, AnsiString(iContectCount));
                    }
                    // completion check: only active slots must have IC
                    if(slot.bActive)
                    {
                        if(InArmSuck.Item[iSuckRow][slot.iPhysical] != HAS_CLEAN_IC &&
                           InArmSuck.Item[iSuckRow][slot.iPhysical] != HAS_NULL_CLEAN_IC)
                            bPickIncomplete = true;
                    }
                }
                if(bPickIncomplete)
                    Task = 10;

                if(Task != 10)
                {
                    if(USE_PICKER_COUNT == ep1Picker)
                        Task = 3300;
                    else if(bUse8Picker)
                        Task = 3200;
                    else
                        Task = 3300;
                }
                break;
            }
            if(TestIF.iTestMode==SingleSite ||                                  //2013-09-13    Dell    for TSMC Single site
               iInArmType==e9045_1x4_1_Ac ||
               USE_PICKER_COUNT==ep1Picker)                                      //Steven 20200720 : 1x4只開site Ac
            {
                iSuckCol=GetAutoCleanPickStep(0);                               //Steven 20240918 : fixed for auto clean
                if(iSuckCol==-1)
                    break;

                iKitRow=iAutoCleanPickPlateY+1;
                iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(iSuckCol-iAutoCleanStart);

                if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_CLEAN_IC ||
                   InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_CLEAN_IC)
                {
                    iContectCount=atoi(fMain->AutoCleanStringGrid->Cells[iKitCol][iKitRow].c_str());
                    iContectCount++;
                    SetAutoCleanStringGrid(iKitCol, iKitRow, AnsiString(iContectCount));
                }

                if(InArmSuck.Item[iSuckRow][iSuckCol]!=HAS_CLEAN_IC &&
                   InArmSuck.Item[iSuckRow][iSuckCol]!=HAS_NULL_CLEAN_IC)
                {
                    Task=10;
                }
            }
            else if(bUseAxExPicker() ||
                    i1x2_4UseACEGPicker==1)
            {
                for(int j=iAutoCleanStart; j<2; j++)
                {
                    iSuckCol=GetAutoCleanPickStep(j);                           //Steven 20240918 : fixed for auto clean
                    if(iSuckCol==-1)
                        continue;
                    iKitRow=iAutoCleanPickPlateY+1;
                    iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(j-iAutoCleanStart);

                    if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_CLEAN_IC ||
                       InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_CLEAN_IC)
                    {
                        iContectCount=atoi(fMain->AutoCleanStringGrid->Cells[iKitCol][iKitRow].c_str());
                        iContectCount++;
                        SetAutoCleanStringGrid(iKitCol, iKitRow, AnsiString(iContectCount));
                    }
                }

                //AI(ht9045-v899) 20260407: allow partial pick when inactive suckers have no pad (Fix H)
                if(bUseCKPP)
                {
                    bool bPickIncomplete=false;
                    for(int j=iAutoCleanStart; j<2; j++)
                    {
                        int sc=GetAutoCleanPickStep(j);
                        if(sc==-1) continue;
                        if(!bInArmSuckActive[iSuckRow][sc]) continue;
                        if(InArmSuck.Item[iSuckRow][sc]!=HAS_CLEAN_IC &&
                           InArmSuck.Item[iSuckRow][sc]!=HAS_NULL_CLEAN_IC)
                        {
                            bPickIncomplete=true;
                            break;
                        }
                    }
                    if(bPickIncomplete) Task=10;
                }
                else
                {
                    if((InArmSuck.Item[iSuckRow][0]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][0]!=HAS_NULL_CLEAN_IC) ||
                       (InArmSuck.Item[iSuckRow][2]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][2]!=HAS_NULL_CLEAN_IC))
                    {
                        Task=10;
                    }
                }
            }
            else if(bUseAxxGPicker() ||                                         //Steven 20240515 : 往上移動
                    iCloseSiteModeFor1x4==e1x4CloseAbAc)                        //Steven 20241111 : for 1x4 close 2 site)
            {
                for(int j=iAutoCleanStart; j<2; j++)
                {
                    iSuckCol=GetAutoCleanPickStep(j);                           //Steven 20240918 : fixed for auto clean
                    if(iSuckCol==-1)
                        continue;
                    iKitRow=iAutoCleanPickPlateY+1;
                    iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(j-iAutoCleanStart);

                    if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_CLEAN_IC ||
                       InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_CLEAN_IC)
                    {
                        iContectCount=atoi(fMain->AutoCleanStringGrid->Cells[iKitCol][iKitRow].c_str());
                        iContectCount++;
                        SetAutoCleanStringGrid(iKitCol, iKitRow, AnsiString(iContectCount));
                    }
                }

                //AI(ht9045-v899) 20260407: allow partial pick when inactive suckers have no pad (Fix H)
                if(bUseCKPP)
                {
                    bool bPickIncomplete=false;
                    for(int j=iAutoCleanStart; j<2; j++)
                    {
                        int sc=GetAutoCleanPickStep(j);
                        if(sc==-1) continue;
                        if(!bInArmSuckActive[iSuckRow][sc]) continue;
                        if(InArmSuck.Item[iSuckRow][sc]!=HAS_CLEAN_IC &&
                           InArmSuck.Item[iSuckRow][sc]!=HAS_NULL_CLEAN_IC)
                        {
                            bPickIncomplete=true;
                            break;
                        }
                    }
                    if(bPickIncomplete) Task=10;
                }
                else
                {
                    if((InArmSuck.Item[iSuckRow][0]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][0]!=HAS_NULL_CLEAN_IC) ||
                       (InArmSuck.Item[iSuckRow][3]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][3]!=HAS_NULL_CLEAN_IC))
                    {
                        Task=10;
                    }
                }
            }
            else
            {
                for(int j=iAutoCleanStart; j<4; j++)
                {
                    iSuckCol=GetAutoCleanPickStep(j);                           //Steven 20240918 : fixed for auto clean
                    if(iSuckCol==-1)
                        continue;
                    iKitRow=iAutoCleanPickPlateY+1;
                    iKitCol=iAutoCleanPickPlateX+iAutoCleanUseXPitch*(j-iAutoCleanStart);

                    if(InArmSuck.Item[iSuckRow][iSuckCol]==HAS_CLEAN_IC ||
                       InArmSuck.Item[iSuckRow][iSuckCol]==HAS_NULL_CLEAN_IC)
                    {
                        iContectCount=atoi(fMain->AutoCleanStringGrid->Cells[iKitCol][iKitRow].c_str());
                        if(CosFunction.bUseAutoCleanCloseSiteAlsoDo==true)
                        {
                            iContectCount++;
                        }
                        else
                        {
                            iContectCount++;
                        }
                        SetAutoCleanStringGrid(iKitCol, iKitRow, AnsiString(iContectCount));
                    }
                }

                //AI(ht9045-v899) 20260407: allow partial pick when inactive suckers have no pad (Fix H)
                if(bUseCKPP)
                {
                    bool bPickIncomplete=false;
                    for(int j=iAutoCleanStart; j<4; j++)
                    {
                        int sc=GetAutoCleanPickStep(j);
                        if(sc==-1) continue;
                        if(!bInArmSuckActive[iSuckRow][sc]) continue;
                        if(InArmSuck.Item[iSuckRow][sc]!=HAS_CLEAN_IC &&
                           InArmSuck.Item[iSuckRow][sc]!=HAS_NULL_CLEAN_IC)
                        {
                            bPickIncomplete=true;
                            break;
                        }
                    }
                    if(bPickIncomplete) Task=10;
                }
                else
                {
                    if((InArmSuck.Item[iSuckRow][0]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][0]!=HAS_NULL_CLEAN_IC) ||
                       (InArmSuck.Item[iSuckRow][1]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][1]!=HAS_NULL_CLEAN_IC) ||
                       (InArmSuck.Item[iSuckRow][2]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][2]!=HAS_NULL_CLEAN_IC) ||
                       (InArmSuck.Item[iSuckRow][3]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][3]!=HAS_NULL_CLEAN_IC)  )
                    {
                        Task=10;
                    }
                }
            }

            if(Task!=10)
            {
                if(USE_PICKER_COUNT==ep1Picker)
                    Task=3300;
                else if(bUse8Picker)                                                 //Steven 20201014 : 整合8吸嘴auto clean
                    Task=3200;
                else
                    Task=3300;
            }
            break;
        case 3200:
            iStatus=GetShuttleState(iSht, bAutoPlace);                          //Steven 20221007 : 修正Auto Clean關site Hang up

            if(iStatus!=0 && iShuttleRowKit-iStatus<=1)
            {
                Task=1;
            }

            if(Task!=1)
            {
                Task=3300;
            }
            break;
        case 3300:
            if(CheckInArmSuckFromCleanKitICFallDown(true))
            {
                iRetryCT=0;
                MoveInOutArmZToKitPickPlace(bAutoPick, true, iSht, iShuttleRowKit); //ChungHung 20150303 add iSht for Hotplate AutoClean  //ChungHung 20131120 AutoClean use Hotplate1
                Task=300;
            }
            else
            {
                Task=1;
            #ifdef DEBUG_AUTO_CLEAN
                Message.sprintf("DoAutoCleanPickfromCleanKit %d, Finish", iSht);
                fMain->AddAutoCleanMessage(Message);
            #endif
                PlaceToCleanList->AddHPSuckGroup();                             //放完了就加入一個新的Group
                iResult=1;
            }
            break;
    }
    return iResult;
}
//------------------------------------------------------------------------------
void InitPlaceToShuttleTask()
{
    iAutoCleanPlaceToShuttleTask=1;
}
//------------------------------------------------------------------------------
bool MoveInArmZ_Shuttle_Pick(eWhichShuttle iSht, int iSelRow)
{
    int iShuttlePick[MAX_ARM_Row][MAX_ARM_Col];//, iMot;
    bool flag[MAX_ARM_Row][MAX_ARM_Col]={{true, true, true, true}, {true, true, true, true}};
    bool bNeedDown=false;

    if(iSht==euShuttle1)
        ptrInSHT=&FLCarryKit;
    else
        ptrInSHT=&BLCarryKit;

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(iSht==euShuttle1)
            {
                if((CosFunction.bDeviceMapTest &&
                   fContact->fShow==true &&
                    iContactMode==CONTACT_DEVICE_MAP_CHECK) ||
                   fContact->IsRun2DCheck()==true)                              //JerryYang 20250220 : 2DID硬體順序檢查功能                      //Steven 20221114 :Add for device map function
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] -
                                       InArmOffSet[InOfsInSh1]->GetPlace() +
                                       InArmOffSet[InOfsInSh1]->GetPickUp() +
                                       InArmOffSet[InOfsInSh1+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(IniConfig.bEnableAutoCleanFunction &&
                        TestIF_File.iAutoClean_Function &&
                        bRunAutoClean &&
                        IniConfig.bE48_ShuttleUse4Offset_Autoclean &&           //20140923 wei : For Shuttle Auto Clean
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] -
                                       InArmOffSet[InOfsInSh1]->GetPlace() +
                                       InArmOffSet[InOfsInSh1_AutoClean]->GetPickUp() +
                                       InArmOffSet[InOfsInSh1_AutoClean+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 僅使用 Clean 設定 Offset 來補償
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j] +
                                       TestIF.iAutoClean_Shuttle1PickOffset -
                                       InArmOffSet[InOfsInSh1]->GetPlace();
                }
                else
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle1_Place[i][j]+TestIF.iAutoClean_Shuttle1PickOffset;
                }

                if(IniConfig.bE33InOutArmZOffsetSameOne)                        //JerryYang 20210811 : 修改使用E33功能時, shuttle的pick up高度被release offset影響的問題
                {
                    iShuttlePick[i][j]=iShuttlePick[i][j] -
                                       InArmOffSet[InOfsLoader]->GetPlace(i, j) +
                                       InArmOffSet[InOfsLoader]->GetPickUp(i, j);
                }
            }
            else
            {
                if(CosFunction.bDeviceMapTest &&
                   fContact->fShow==true &&
                   iContactMode==CONTACT_DEVICE_MAP_CHECK)                      //Steven 20221114 :Add for device map function
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] -
                                       InArmOffSet[InOfsInSh2]->GetPlace() +
                                       InArmOffSet[InOfsInSh2]->GetPickUp() +
                                       InArmOffSet[InOfsInSh2+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(IniConfig.bEnableAutoCleanFunction &&
                        TestIF_File.iAutoClean_Function &&
                        bRunAutoClean &&
                        IniConfig.bE48_ShuttleUse4Offset_Autoclean &&           //20140923 wei : For Shuttle Auto Clean
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] -
                                       InArmOffSet[InOfsInSh2]->GetPlace() +
                                       InArmOffSet[InOfsInSh2_AutoClean]->GetPickUp()+
                                       InArmOffSet[InOfsInSh2_AutoClean+iSelRow-1]->SingleOffSet->dPickUpOffSet[i][j];
                }
                else if(CosFunction.bAutoCleanOffsetUseSingleSetting &&         //Sam 20220720 : AutoClean Offset 僅使用 Clean 設定 Offset 來補償
                        TestIF.bEnableAutoAlignment==false)                     //KenHsieh 20220923 : add AOA功能開啟不補償
                {
                    if(IniConfig.bE34InOutArmPitchZOffsetSameOne)
                        iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] +
                                           TestIF.iAutoClean_Shuttle2PickOffset -
                                           InArmOffSet[InOfsInSh1]->GetPlace();
                    else
                        iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] +
                                           TestIF.iAutoClean_Shuttle2PickOffset -
                                           InArmOffSet[InOfsInSh2]->GetPlace();
                }
                else
                {
                    iShuttlePick[i][j]=Prod.ZInArm_Shuttle2_Place[i][j] +
                                       TestIF.iAutoClean_Shuttle2PickOffset;    //ChungHung 20141117 : 改為iAutoClean_Shuttle2PickOffset
                }
                if(IniConfig.bE33InOutArmZOffsetSameOne)                        //JerryYang 20210811 : 修改使用E33功能時, shuttle的pick up高度被release offset影響的問題
                {
                    iShuttlePick[i][j]=iShuttlePick[i][j] -
                                       InArmOffSet[InOfsLoader]->GetPlace(i, j) +
                                       InArmOffSet[InOfsLoader]->GetPickUp(i, j);
                }
            }
        }
    }

    if(bUse8Picker)                                                             //Steven 20240221 : 整合8吸嘴auto clean
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
//                iMot=InArmSuck.Suck[i][j].iMotNo;
                bNeedDown=(InArmSuck.Item[i][j]==NULL_IC &&                     //Steven 20240306 : 改用NeedSuckCleanPad
                           InArmSuck.iNeedSuck[i][j]!=NULL_IC &&
                           InArmSuck.iNeedSuck[i][j]!=HAS_NULL_CLEAN_IC &&
                           InArmSuck.iNeedSuck[i][j]!=HAS_NULL_IC);             //JerryYang 20250220 : 2DID硬體順序檢查功能

                flag[i][j]=bNeedDown;
//                if(bNeedDown)
//                {
//                    flag[i][j]=MOT[iMot].MotorMove(iShuttlePick[i][j]);
//                }
//                else
//                {
//                    flag[i][j]=true;
//                }
            }
        }
    }
    else
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
//            flag[0][j]=true;
//            iMot=InArmSuck.Suck[1][j].iMotNo;
            bNeedDown=(InArmSuck.Item[1][j]==NULL_IC &&                         //Steven 20240306 : 改用NeedSuckCleanPad
                       InArmSuck.iNeedSuck[1][j]!=NULL_IC &&
                       InArmSuck.iNeedSuck[1][j]!=HAS_NULL_CLEAN_IC &&
                       InArmSuck.iNeedSuck[1][j]!=HAS_NULL_IC);                 //JerryYang 20250220 : 2DID硬體順序檢查功能

            flag[1][j]=bNeedDown;
            flag[0][j]=false;                                                   //RogerYang 20251119 : bUse8Picker==false情況下，吸嘴不能全下
//            if(bNeedDown==true)
//            {
//                flag[1][j]=MOT[iMot].MotorMove(iShuttlePick[1][j]);
//            }
//            else
//            {
//                flag[1][j]=true;
//            }
        }
    }

//    if(flag[0][0] && flag[0][1] && flag[0][2] && flag[0][3] &&
//       flag[1][0] && flag[1][1] && flag[1][2] && flag[1][3])
//        return true;
//    else
//        return false;

    bool bRet=InArmZMoveDown(flag, iShuttlePick, false);                        //Steven 20251020 : modify for auto clean
    return bRet;
}
//------------------------------------------------------------------------------
bool CheckInSuckICFallDown(int KCode)
{
    int ret;
    AnsiString ErrPart="";                                                      //Steven 20110216 : 合併Alarm
    bool bHasErr=false;                                                         //Steven 20110216 : 合併Alarm
    bool bStatus;

    int iSuckRow=0;                                                             //ChungHung 20141121 add for Use HotPlate AutoClean

    if(IniConfig.bE43AutoCleanUseHotplate)
        iSuckRow=0;
    else
        iSuckRow=1;

    if(LastSet.iRealDummy!=REALLY)
        return false;

    for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                               //ChungHung 20141121 add for Use HotPlate AutoClean
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            #ifndef SOFT_SIMULTE
                if((InArmSuck.Item[i][j]==HAS_CLEAN_IC ||
                    InArmSuck.Item[i][j]==CLEAN_FINISH_IC ||
                    InArmSuck.Item[i][j]==HAS_CLEAN_FINSH_IC) &&
                    InArmSuck.Suck[i][j].GetStatus()==false)                    //ChungHung 20141121 add for Use HotPlate AutoClean
                {
                    ErrPart+=InArmSuck.Suck[i][j].sName;
                    bHasErr=true;
                }
            #else
                if(fMain->chkCleanPadPickErr->Checked==true)
                    bHasErr=true;
            #endif
        }
    }

    if(bHasErr==true)
    {
        ret=ShowErrorMessage("JAM0128", KCode, MInArmX, false, ErrPart);        //JerryYang 20160511 JAM0126->JAM0128,將IC及Clean pad相關的alarm code分開

        if(ret==K_RETRY)
        {
            for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                       //ChungHung 20141121 add for Use HotPlate AutoClean
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    bStatus=InArmSuck.Suck[i][j].GetStatus();
                    if((InArmSuck.Item[i][j]==HAS_CLEAN_IC ||
                        InArmSuck.Item[i][j]==CLEAN_FINISH_IC ||
                        InArmSuck.Item[i][j]==HAS_CLEAN_FINSH_IC) &&
                        bStatus==false)                                         //ChungHung 20141121 add for Use HotPlate AutoClean
                    {
                        InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);
                    }
                }
            }
            return true;
        }
        else
        {
            for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                       //ChungHung 20141121 add for Use HotPlate AutoClean
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    #ifdef SOFT_SIMULTE
                        if(fMain->chkCleanPadPickErr->Checked==true)
                            bStatus=(i==1 && j==0)?false:true;
                    #else
                        bStatus=InArmSuck.Suck[i][j].GetStatus();
                    #endif

                    if((InArmSuck.Item[i][j]==HAS_CLEAN_IC ||
                        InArmSuck.Item[i][j]==CLEAN_FINISH_IC ||
                        InArmSuck.Item[i][j]==HAS_CLEAN_FINSH_IC) &&
                       bStatus==false)                                          //ChungHung 20141121 add for Use HotPlate AutoClean
                    {
                        InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);
                    }
                }
            }
        }
    }
    return false;
}
//==============================================================================
bool CleanPad_PlaceToShuttle(int iSht)
{
    int flag=true;
    int iShtRow=0, iShtCol=0;
    int iPickKit32=(iShuttleRowKit==1 || iShuttleRowKit==2)?0:4;

    if(USE_PICKER_COUNT==ep1Picker)
    {
        GetNowInShuttleRowCol_All_1Picker(iSht, &iShtRow, &iShtCol, bAutoPlace);

        if(InArmSuck.Item[0][0]==HAS_NULL_CLEAN_IC ||
           (InArmSuck.Suck[0][0].GetNeedDestroyStatus() &&
            (InArmSuck.Item[0][0] &&
             InArmSuck.Suck[0][0].Destroy())))
        {
            if(iSht==euShuttle1)
                FLCarryKit.MoveSuckDataDiff(InArmSuck, 0, 0, iShtRow, iShtCol);
            else
                BLCarryKit.MoveSuckDataDiff(InArmSuck, 0, 0, iShtRow, iShtCol);

        }
        else if(InArmSuck.Item[0][0] &&
                InArmSuck.Suck[0][0].GetNeedDestroyStatus())
        {
            flag=false;
        }
    }
    else if(bUse8Picker)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(iCloseSiteModeFor2x8==e2x8_STMMode ||                        //KevinCheng 20251031 : STM與TW153模式拿掉 +iPickKit32 避免+4格造成放料錯位
                    iCloseSiteModeFor2x8==e2x8_TW153Mode)
                {
                    iShtRow=i;
                    iShtCol=GetShuttleCol(i, j);

                    if(InArmSuck.Suck[i][j].GetNeedDestroyStatus() &&           //KevinCheng 20251031 : 調換判斷順序 避免先丟HAS_NULL_CLEAN_IC影響GetShuttleState_2x8_8判斷 造成iShuttleRowKit流程錯誤
                        (InArmSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                        (InArmSuck.Item[i][j] &&
                        InArmSuck.Suck[i][j].Destroy())))
                    {
                        if(iShtCol<8)
                        {
                            if(iSht==euShuttle1)
                                FLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                            else
                                BLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                        }
                    }
                    else if(InArmSuck.Item[i][j] &&
                            InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                    {
                        flag=false;
                    }
                }
                else
                {
                    iShtRow=i;
                    iShtCol=GetShuttleCol(i, j+iPickKit32);

                    if(InArmSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                       (InArmSuck.Suck[i][j].GetNeedDestroyStatus() &&
                        (InArmSuck.Item[i][j] &&
                         InArmSuck.Suck[i][j].Destroy())))
                    {
                        if(iShtCol<8)
                        {
                            if(iSht==euShuttle1)
                                FLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                            else
                                BLCarryKit.MoveSuckDataDiff(InArmSuck, i, j, iShtRow, iShtCol);
                        }
                    }
                    else if(InArmSuck.Item[i][j] &&
                            InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                    {
                        flag=false;
                    }

                }
            }
        }
    }
    else
    {
        iShtRow=(iShuttleRowKit==2 || iShuttleRowKit==4)?1:0;
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            iShtCol=GetShuttleCol(iShtRow, j+iPickKit32);

            if(iCloseSiteModeFor2x8==e2x8_STMMode ||                            //KevinCheng 20251031 : STM與TW153模式拿掉 +iPickKit32 避免+4格造成放料錯位
                iCloseSiteModeFor2x8==e2x8_TW153Mode)
            {
                if(InArmSuck.Suck[1][j].GetNeedDestroyStatus() &&               //KevinCheng 20251031 : 調換判斷順序 避免先丟HAS_NULL_CLEAN_IC影響GetShuttleState_2x8_8判斷 造成iShuttleRowKit流程錯誤
                    (InArmSuck.Item[1][j]==HAS_NULL_CLEAN_IC ||
                   (InArmSuck.Item[1][j] &&
                    InArmSuck.Suck[1][j].Destroy())))
                {
                    if(iShtCol<8)
                    {
                        if(iSht==euShuttle1)
                            FLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                        else
                            BLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                    }
                }
                else if(InArmSuck.Item[1][j] &&
                        InArmSuck.Suck[1][j].GetNeedDestroyStatus())
                {
                    flag=false;
                }
            }
            else
            {
                if(InArmSuck.Item[1][j]==HAS_NULL_CLEAN_IC ||
                   (InArmSuck.Item[1][j] &&
                    InArmSuck.Suck[1][j].GetNeedDestroyStatus() &&
                    InArmSuck.Suck[1][j].Destroy()))
                {
                    if(iShtCol<8)
                    {
                        if(iSht==euShuttle1)
                            FLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                        else
                            BLCarryKit.MoveSuckDataDiff(InArmSuck, 1, j, iShtRow, iShtCol);
                    }
                }
                else if(InArmSuck.Item[1][j] &&
                        InArmSuck.Suck[1][j].GetNeedDestroyStatus())
                {
                    flag=false;
                }
            }
        }
    }

    return flag;
}
//------------------------------------------------------------------------------
bool DoPlaceToShuttle(eWhichShuttle iSht)
{
    bool flag;
    int ret=0, iKit=(iShuttleRowKit==1 || iShuttleRowKit==2)?0:1;
    int &Task=iAutoCleanPlaceToShuttleTask;
    AnsiString Message;
    QueueTaskList[41].CheckTaskChange();                                        //Steven 20220218 : Auto clean記錄Task變化

    #ifdef DEBUG_AUTO_CLEAN
    static int iOldTask=0;
    if(iOldTask!=Task)
    {
        Message.sprintf("DoPlaceToShuttle %d, %d, Go to Task, %d", iSht, iOldTask, Task);
        fMain->AddAutoCleanMessage(Message);
        iOldTask=Task;
    }
    #endif

    switch(Task)
    {
        case 1:
            bAutoCleanPlaceToSht=true;
            iShuttleRowKit=GetShuttleState(iSht, bAutoPlace);
            bAutoCleanPlaceToSht=false;
            Task=100;
        case 100:
            iKit=(iShuttleRowKit==1 || iShuttleRowKit==2)?0:1;                  //Steven 20240512 : 整合MoveInArmXYToShuttle
            flag=InArmSuck.ArmAll_HasICType(NULL_IC, HAS_NULL_CLEAN_IC);        //Steven 20220929 : 避免關Site時, In arm來回跑 //Steven 20250420 : fixed for auto clean
            if(flag ||
               MoveInArmXYToShuttle_9045(iSht, iKit, ZAxisNotDown, true))
            {
                Task=2000;
            }
            CheckInArmSuckFromCleanKitICFallDown(false);
            break;
        case 2000:
            MOT[MInShuttle1+iSht].ScanMotorStatus();
            if(MOT[MInShuttle1+iSht].Led[8]==false &&
               MOT[MInShuttle1+iSht].fCanMoveL==false)
            {
                MOT[MInShuttle1+iSht].fCanMoveL=true;
            }

            flag=false;
            if(iSht==euShuttle1)
                flag=InSHT1InLF();
            else
                flag=InSHT2InLF();

            if(flag)
            {
                InArmZNeedDown_9045(iSht, iKit, true);                          //Steven 20240512 : 整合InArmZNeedDown
                MOT[MInShuttle1+iSht].fCanMoveL=false;
                Task=2100;
            }
            break;
        case 2100:
            if(MOT[MInShuttle1+iSht].ReadPos()!=Prod.InSHT[iSht].iLeft)
            {
                Task=2110;
                break;
            }

            if(MoveInArmZToShuttlePlace(iSht, iShuttleRowKit))
            {
                if((AccessLevel>=1 ||
                    CosFunction.bOPCanPressStepAndTStart) &&                    //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualTStart].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoPlaceToShuttle 2100");
                    Task=2150;
                    break;
                }
                Task=2200;
            }
            break;
        case 2110:
            if(MoveInArmZToPlateSafe(Task))
            {
                MOT[MInShuttle1+iSht].fCanMoveL=true;
                Task=100;
            }
            break;
        case 2150:
            if(bEnterOffset==true)
            {
                bEnterOffset=false;
                Task=2160;
            }
            else
            {
                Task=2100;
            }
            break;
        case 2160:
            if(MoveInArmZToPlateSafe(Task))
            {
               Task=2170;
            }
            break;
        case 2170:
            if(MoveInArmXYToShuttle_9045(iSht, iKit, ZAxisNotDown, true))
            {
                Task=2100;
            }
            break;
        case 2200:
            flag=CleanPad_PlaceToShuttle(iSht);

            if(flag==false)                                                     //kevin 20131011 在下真空誤判, 換到上面來
                break;

            ret=CheckInArmDestroyICFail();                                      //Steven 20111223 : 檢查破壞錯誤
            if(ret==false)
                return false;

            for(int i=0; i<InArmSuck.iMotRow; i++)
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    if(InArmSuck.Item[i][j] &&
                       InArmSuck.Suck[i][j].GetNeedDestroyStatus())
                    {
                        return false;
                    }
                }
            }
            Task=2300;
            break;
        case 2300:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(USE_PICKER_COUNT==ep1Picker)
                {
                    if(iSht==euShuttle1)
                    {
                        for(int irow=0; irow<FLCarryKit.iShtRow; irow++)                    //JerryYang 20250901
                        {
                            for(int icol=0; icol<FLCarryKit.iShtCol; icol++)
                            {
                                if(FLCarryKit.Item[irow][icol]==NULL_IC &&
                                  LastSet.bUseTestSocket[0][irow][icol]==false &&
                                  LastSet.bUseTestSocket[1][irow][icol]==false)
                                {
                                    FLCarryKit.SetItemData(irow, icol, HAS_NULL_CLEAN_IC);
                                }
                            }
                        }
                    }
                    else
                    {
                        for(int irow=0; irow<BLCarryKit.iShtRow; irow++)                    //JerryYang 20250901
                        {
                            for(int icol=0; icol<BLCarryKit.iShtCol; icol++)
                            {
                                if(BLCarryKit.Item[irow][icol]==NULL_IC &&
                                  LastSet.bUseTestSocket[0][irow][icol]==false &&
                                  LastSet.bUseTestSocket[1][irow][icol]==false)
                                {
                                    BLCarryKit.SetItemData(irow, icol, HAS_NULL_CLEAN_IC);
                                }
                            }
                        }
                    }
                }

                if(InArmSuck.HasRealIC())
                {
                    bAutoCleanPlaceToSht=true;

                    if(bUse8Picker && (iCloseSiteModeFor2x8==e2x8_STMMode ||    //Sam 20200310 : 卡 STM
                                       iCloseSiteModeFor2x8==e2x8_TW153Mode))
                    {
                        iShuttleRowKit=GetShuttleState(iSht, bAutoPlace);       //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
                    }
                    bAutoCleanPlaceToSht=false;
                    Task=100;
                }
                else
                {
                    if(TestIF_File.iTestMode==_16Site2X8 &&
                      (iCloseSiteModeFor2x8==e2x8_STMMode ||
                       iCloseSiteModeFor2x8==e2x8_TW153Mode) &&
                       InArmSuck.HasIC())
                    {
                        bAutoCleanPlaceToSht=true;

                        if(bUse8Picker && (iCloseSiteModeFor2x8==e2x8_STMMode ||
                                           iCloseSiteModeFor2x8==e2x8_TW153Mode))
                        {
                            iShuttleRowKit=GetShuttleState(iSht, bAutoPlace);
                        }
                        bAutoCleanPlaceToSht=false;
                        Task=100;
                    }
                    else
                    {
                        if(iCloseSiteModeFor2x8>e2x8OneByOne ||
                           iCloseSiteModeFor2x6>e2x6OneByOne)
                            SetShuttleIcForSpecialMode(iSht, HAS_NULL_CLEAN_IC);

                        Task=2400;
                    }
                }
            }
            break;
        case 2400:
            #ifdef DEBUG_AUTO_CLEAN
                Message.sprintf("DoPlaceToShuttle %d, Finish", iSht);
                fMain->AddAutoCleanMessage(Message);
            #endif
            return true;
    }
    return false;
}
//==============================================================================
bool CleanPad_PickFromShuttle(int iSht, int iSelRow)
{
    bool flag=true;
    int iShtRow, iShtCol;
    int iPickKit32=(iSelRow==1 || iSelRow==2)?0:4;

    if(USE_PICKER_COUNT==ep1Picker)
    {
        GetNowInShuttleRowCol_All_1Picker(iSht, &iShtRow, &iShtCol, bAutoPick);
        if(ptrInSHT->Item[iShtRow][iShtCol] &&
           InArmSuck.iNeedSuck[0][0]!=NULL_IC)
        {
            if(ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_CLEAN_IC ||
               ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_IC ||         //JerryYang 20250220 : 2DID硬體順序檢查功能
               InArmSuck.Suck[0][0].Suck())
            {
                InArmSuck.MoveSuckDataDiff(*ptrInSHT, iShtRow, iShtCol, 0, 0);
                InArmSuck.PordRec[0][0].AddPickCleanPadFormShuttle(0, 0, iShtRow, iShtCol);  //Sam 20230616 : Add Auto Clean Record
                ptrInSHT->Item[iShtRow][iShtCol]=NULL_IC;
            }
            else if(InArmSuck.Item[0][0]==NULL_IC &&
                    InArmSuck.Suck[0][0].Error==false)
            {
                flag=false;
            }
        }
    }
    else if(bUse8Picker)                                                             //Steven 20201014 : 整合8吸嘴auto clean
    {
        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                iShtRow=i;
                iShtCol=GetShuttleCol(i, j+iPickKit32);

                if(iShtCol<8 &&
                   ptrInSHT->Item[iShtRow][iShtCol] &&
                   InArmSuck.iNeedSuck[i][j]!=NULL_IC)
                {
                    if(ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_CLEAN_IC ||
                       ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_IC ||         //JerryYang 20250220 : 2DID硬體順序檢查功能
                       InArmSuck.Suck[i][j].Suck())
                    {
                        InArmSuck.MoveSuckDataDiff(*ptrInSHT, iShtRow, iShtCol, i, j);
                        InArmSuck.PordRec[i][j].AddPickCleanPadFormShuttle(i, j, iShtRow, iShtCol);  //Sam 20230616 : Add Auto Clean Record
                        ptrInSHT->Item[iShtRow][iShtCol]=NULL_IC;
                    }
                    else if(InArmSuck.Item[i][j]==NULL_IC &&
                            InArmSuck.Suck[i][j].Error==false)
                    {
                        flag=false;
                    }
                }
            }
        }
    }
    else
    {
        iShtRow=(iSelRow==2 || iSelRow==4)?1:0;
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iShtCol=GetShuttleCol(iShtRow, j+iPickKit32);

            if(iShtCol<8 &&
               ptrInSHT->Item[iShtRow][iShtCol] &&
               InArmSuck.iNeedSuck[1][j]!=NULL_IC)
            {
                if(ptrInSHT->Item[iShtRow][iShtCol]==HAS_NULL_CLEAN_IC ||
                   InArmSuck.Suck[1][j].Suck())
                {
                    InArmSuck.MoveSuckDataDiff(*ptrInSHT, iShtRow, iShtCol, 1, j);
                    InArmSuck.PordRec[1][j].AddPickCleanPadFormShuttle(1, j, iShtRow, iShtCol);  //Sam 20230616 : Add Auto Clean Record
                }
                else if(InArmSuck.Item[1][j]==NULL_IC &&
                        InArmSuck.Suck[1][j].Error==false)
                {
                    flag=false;
                }
            }
        }
    }

    return flag;
}
//------------------------------------------------------------------------------
//TQPF_Timer DoPickFromShuttleT;
bool DoPickFromShuttle(eWhichShuttle iSht, int iSelRow)
{
    static int iRetryCT=0;
    static bool bSuckDuplicateErr[MAX_ARM_Row][MAX_ARM_Col]={{false, false, false, false}, {false, false, false, false}};  //Steven 20091218 : Avoid duplicate message

    int iShtRow=0, iShtCol=0, iKit=(iSelRow==1 || iSelRow==2)?0:1;
    int &Task=iAutoCleanPickFromShuttleTask;
    bool bACPickShtFlag;
    bool bACPickShtError=false;
    bool bHasDuplicateErr=false;
    AnsiString ErrPart="";
    AnsiString Message;
    QueueTaskList[52].CheckTaskChange();                                        //Steven 20220218 : Auto clean記錄Task變化

    #ifdef DEBUG_AUTO_CLEAN
    static int iOldTask=0;
    if(iOldTask!=Task)
    {
        Message.sprintf("DoPickFromShuttle %d, %d, Go to Task, %d", iSht, iOldTask, Task);
        fMain->AddAutoCleanMessage(Message);
        iOldTask=Task;
    }
    #endif

    if(iSht==0)
        ptrInSHT=&FLCarryKit;
    else
        ptrInSHT=&BLCarryKit;

    switch(Task)
    {
        case 1:
            if(fContact->fShow==true && iContactMode==CONTACT_DEVICE_MAP_CHECK) //Steven 20220510 : For QTI SD Device Map Function
            {

            }
            else
            {
                SetShuttleIcForSpecialMode(iSht, NULL_IC);                      //Steven 20221006 : 針對特殊模式進行Clean Pad置換

                if(USE_PICKER_COUNT==ep1Picker)
                {
                    if(iSht==euShuttle1)
                    {
                        for(int irow=0; irow<FLCarryKit.iShtRow; irow++)                    //JerryYang 20250901
                        {
                            for(int icol=0; icol<FLCarryKit.iShtCol; icol++)
                            {
                                if(FLCarryKit.Item[irow][icol]==NULL_IC &&
                                  LastSet.bUseTestSocket[0][irow][icol]==false &&
                                  LastSet.bUseTestSocket[1][irow][icol]==false)
                                {
                                    FLCarryKit.SetItemData(irow, icol, NULL_IC);
                                }
                            }
                        }
                    }
                    else
                    {
                        for(int irow=0; irow<BLCarryKit.iShtRow; irow++)                    //JerryYang 20250901
                        {
                            for(int icol=0; icol<BLCarryKit.iShtCol; icol++)
                            {
                                if(BLCarryKit.Item[irow][icol]==NULL_IC &&
                                  LastSet.bUseTestSocket[0][irow][icol]==false &&
                                  LastSet.bUseTestSocket[1][irow][icol]==false)
                                {
                                    BLCarryKit.SetItemData(irow, icol, NULL_IC);
                                }
                            }
                        }
                    }
                }
            }
            Task=10;
            break;
        case 10:
            if(MoveInArmXYToShuttle_9045(iSht, iKit, ZAxisNotDown, false))      //Steven 20240512 : 整合MoveInArmXYToShuttle
            {
                InArmZNeedDown_9045(iSht, iKit, false);                         //Steven 20240512 : 整合InArmZNeedDown
                Task=50;
            }
            break;
        case 50:
            if(MoveInArmZ_Shuttle_Pick(iSht, iSelRow))
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualTStart].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoPickFromShuttle 50");
                    Task=100;
                    break;
                }

                InArmSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=200;
            }
            break;
        case 100:
            if(bEnterOffset)
            {
                bEnterOffset=false;
                Task=1;
                return false;
            }
            else
            {
                Task=200;
                InArmSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
            }
            break;
        case 200:
            bACPickShtFlag=CleanPad_PickFromShuttle(iSht, iSelRow);

            if(bACPickShtFlag==false)
                return false;

            bACPickShtError=false;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Suck[i][j].Error==true)
                    {
                        bACPickShtError=true;
                    }
                }
            }

            if(bACPickShtError)
            {
                iRetryCT++;
                Task=300;
                return false;                                                   //JerryYang 20160227 修正吸取異常時Z軸不停上下動
            }

            iRetryCT=0;
            Task=400;
            break;
        case 300:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(iRetryCT>ArmSpeed[InArm].iRetryCT)                           //Steven 20120109 : 整合成一次Skip多顆
                {
                    ErrPart=" ";
                    bHasDuplicateErr=false;
                    for(int i=0; i<InArmSuck.iMaxRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            if(bSuckDuplicateErr[i][j])
                                bHasDuplicateErr=true;

                            if(InArmSuck.Suck[i][j].Error)
                            {
                                bSuckDuplicateErr[i][j]=true;
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                            }
                            else
                            {
                                bSuckDuplicateErr[i][j]=false;
                            }
                        }
                    }

                    iRetryCT=0;
                    if(iContactMode==CONTACT_DEVICE_MAP_CHECK ||                //JerryYang 20221121 : 修正alarm code錯誤
                       fContact->IsRun2DCheck())                                //JerryYang 20250220 : 2DID硬體順序檢查功能
                    {
                        ShowErrorMessage("JAM0111", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);  //Device pick-up error on Shuttle    //JerryYang 20160511 JAM0111->JAM0115,將IC及Clean pad相關的alarm code分開
                    }
                    else
                    {
                        ShowErrorMessage("JAM0115", K_RETRY, MInArmX, bHasDuplicateErr, ErrPart);  //Device pick-up error on Shuttle    //JerryYang 20160511 JAM0111->JAM0115,將IC及Clean pad相關的alarm code分開
                    }
                    Task=310;                                                   //jou 2015-08-23 Homing action needed during Auto clean for input picker
                }
                else
                {
                    Task=1;
                }
            }
            break;
        case 310:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=320;
            }
            break;
        case 320:
            SetInArmHome();                                                     //jou 2015-08-23 Homing action needed during Auto clean for input picker
            Task=1;
            break;
        case 400:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(CheckInSuckICFallDown(K_SKIP)==true)                         //JerryYang 20160219 when drop error only can skip
                {
                    Task=1;                                                     //kevin 20150625
                    break;
                }

                if(iCloseSiteModeFor2x8>e2x8OneByOne ||                         //Steven 20220816 : Add for TW153TK spec
                   iCloseSiteModeFor2x6>e2x6OneByOne)
                {
                    for(int i=0; i<InArmSuck.iMaxRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            iShtRow=i;
                            iShtCol=GetShuttleCol(i, j);

                            if(ptrInSHT->Item[iShtRow][iShtCol])
                            {
                                Task=1;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if(bUse8Picker==false)
                    {
                        if(iSelRow==1 || iSelRow==3)                            //Row 1
                            iShtRow=0;
                        else                                                    //Row 2
                            iShtRow=1;

                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            if(iSelRow==1 || iSelRow==2)
                                iShtCol=GetShuttleCol(iShtRow, j);
                            else                                                //Kit 1
                                iShtCol=GetShuttleCol(iShtRow, j+4);

                            if(ptrInSHT->Item[iShtRow][iShtCol])
                            {
                                Task=1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        for(int i=0; i<InArmSuck.iMaxRow; i++)
                        {
                            for(int j=0; j<InArmSuck.iMaxCol; j++)
                            {
                                iShtRow=i;
                                if(iSelRow==1 || iSelRow==2)
                                    iShtCol=GetShuttleCol(iShtRow, j);
                                else                                            //Kit 1
                                    iShtCol=GetShuttleCol(iShtRow, j+4);

                                if(ptrInSHT->Item[iShtRow][iShtCol])
                                {
                                    Task=1;
                                    break;
                                }
                            }
                        }
                    }
                }

                #ifdef DEBUG_AUTO_CLEAN
                    Message.sprintf("DoPickFromShuttle %d, Finish", iSht);
                    fMain->AddAutoCleanMessage(Message);
                #endif
                if(Task==1)                                                     //上面有回到1的就表示要繼續做
                    return false;
                else
                    Task=1;
                return true;
            }
            break;
    }
    return false;
}
//==============================================================================
bool PlaceToCleanKit()
{
    bool flag=true;
    int iSuckRow=0, iSuckCol=0, iKitRow=0, iKitCol=0;

    AnsiString sTime="";
    int iP=0;
    int iPlateR[MAX_ARM_Row][MAX_ARM_Col];                                      //Jimmychiu 20230417 : Record the position after placing the IC
    int iPlateC[MAX_ARM_Row][MAX_ARM_Col];
    bool bSuck[MAX_ARM_Row][MAX_ARM_Col];
    ZeroMemory(iPlateR, sizeof(iPlateR));
    ZeroMemory(iPlateC, sizeof(iPlateC));
    ZeroMemory(bSuck, sizeof(bSuck));
    GetTimeInfo();

    if(PlaceToCleanList->GetHPFirstTeam(&iP, iPlateR, iPlateC, bSuck))
    {
        if(PlaceToCleanList->GetHPFirstTeamMotUse(bCleanZMotToPlace)==false)    //RogerYang 20250624 Add HotPlate ErrMessage
        {
            ShowMyMessage(AnsiString().sprintf("%s %s Task=%d", __FUNC__, "No GetHPFirstTeamMotUse", 0));
            return false;
        }
        PlaceToCleanList->GetHPFirstTeamSuckUse(bCleanZSuckToPlace);

        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(bCleanZSuckToPlace[i][j]==true)
                {
                    iSuckRow=i;
                    iSuckCol=j;
                    iKitRow =InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol];
                    iKitCol =InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol];

                    if(iKitCol<0   || iKitRow<0 ||
                       iKitCol>=50 || iKitRow>=50)
                    {
                        ShowMyMessage("CleanKitRecord error1", "CleanKitRecord 錯誤3");
                        InArmSuck.SetItemData(i, j, NULL_IC);
                    }
                    else if((bCleanZMotToPlace[i][j]==false &&                  //資料要交換, 吸嘴不用下去的, 就是HAS_NULL_CLEAN_IC
                             InArmSuck.Item[i][j]!=NULL_IC) ||
                            (bCleanZMotToPlace[i][j]==true &&
                             InArmSuck.Item[i][j]!=NULL_IC &&
                             InArmSuck.Suck[i][j].Destroy()))
                    {
                        InArmSuck.SetItemData(i, j, NULL_IC);
                        InArmSuck.PordRec[i][j].AddPlaceCleanPad(i, j, InArmSuck.iAutoCleanRecY[i][j], InArmSuck.iAutoCleanRecX[i][j]);    //Sam 20230616 : Add Auto Clean Record
                        InArmSuck.PordRec[1][j].SaveRecordCleanPad();
                        MOT[MMAutoCleanKit].SetTraySingleData(InArmSuck.iAutoCleanRecX[i][j], InArmSuck.iAutoCleanRecY[i][j], CLEAN_FINISH_IC);   //JerryYang 20160210 修正auto clean 使用hot plate hang up問題
                        sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
                        CleanKitRecord[InArmSuck.iAutoCleanRecY[i][j]][InArmSuck.iAutoCleanRecX[i][j]]=sTime;   //Sam 20230619 : 新增 Clean吸放時間 Log
                        bInArmSuckActive[i][j]=false;
                        bInArmCheckDestroyACT[i][j]=true;                       //Sam 20240124 : AutoClean 補回黏偵測
                    }
                    else
                    {
                        if(InArmSuck.Item[i][j]!=NULL_IC &&
                           InArmSuck.Suck[1][j].Error==false)
                            flag=false;
                    }
                }
            }
        }
    }

//    flag=CheckInArmDestroyICFail();                                             //Steven 20111223 : 檢查破壞錯誤
    if(flag==false)
        return false;

    for(int j=0; j<InArmSuck.iMaxCol; j++)
    {
        iSuckCol=j;
        if(bInArmSuckActive[iSuckRow][iSuckCol] &&
           InArmSuck.Item[iSuckRow][iSuckCol]!=NULL_IC)
            return false;
    }
    return flag;
}
//------------------------------------------------------------------------------
TQPF_Timer DoAutoCleanPlaceToCleanKitDelay;
bool DoAutoCleanPlaceToCleanKit(bool Reset)
{
    if(Reset)
    {
        iAutoCleanPlaceToCleanKitTask=1;
        return false;
    }

    int &Task=iAutoCleanPlaceToCleanKitTask;
    bool flag=false;
    QueueTaskList[60].CheckTaskChange();                                        //Steven 20220218 : Auto clean記錄Task變化

    switch(Task)
    {
        case 1:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=10;
            }

            InOutArmSuckActiveSet();
            break;
        case 5:
            InitialSet();
            Task=10;
            break;
        case 10:
            if(CheckInArmSuckICFallDownToHasNullIC())
            {
                Task=5;
                break;
            }

            if(InArmSuck.HasIC()==false)
                return true;

            Task=11;
        case 11:
            flag=InArmSuck.ArmAll_HasICType(NULL_IC, HAS_NULL_CLEAN_IC);        //Steven 20220929 : 避免關Site時, In arm來回跑 //Steven 20250420 : fixed for auto clean
            if(flag || MoveInArmXYPickCleanKit(bAutoPlace, 0, euShuttle1))
            {
                if(InArmSuck.HasRealIC())
                {
                    MoveInOutArmZToKitPickPlace(bAutoPlace, true, 0, 0);        //ChungHung 20150303 add iSht for Hotplate AutoClean //ChungHung 20131120 AutoClean use Hotplate1
                    Task=100;
                }
                else
                {
                    Task=300;
                }
            }
            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=15;                                                        //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 15:
            CheckInArmSuckICFallDownToHasNullIC();                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=11;
            break;
        case 100:
            if(MoveInOutArmZToKitPickPlace(bAutoPlace, false, 0, 0))            //ChungHung 20150303 add iSht for Hotplate AutoClean //ChungHung 20131120 AutoClean use Hotplate1
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                    Sen[SnRKManualTStart].IsOn())
                {
                    bEnterOffset=false;
                    fMain->Pause("DoAutoCleanPlaceToCleanKit 100");
                    Task=200;
                    break;
                }
                else
                {
                    if(InOutArmPickerUseMotor==eptUseCyn)
                    {
                        DoAutoCleanPlaceToCleanKitDelay.Set0_1SecAndOn(100);
                        Task=110;
                        break;
                    }
                }
                Task=300;
            }
            if(CheckInArmSuckICFallDownToHasNullIC(false)==true)                //Steven 20110516 : 修改成整合式Alarm
            {
                Task=105;                                                       //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            }
            break;
        case 105:
            CheckInArmSuckICFallDownToHasNullIC();                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=100;
            break;
        case 110:
            if(DoAutoCleanPlaceToCleanKitDelay.Off())
            {
                Task=300;
            }
            break;
        case 200:
            if(bEnterOffset==false)
            {
                Task=300;
            }
            else
            {
                bEnterOffset=false;
                Task=220;
            }
            break;
        case 220:
            if(MoveInArmZToPlateSafe(Task))
                Task=1;
            break;
        case 300:
            flag=PlaceToCleanKit();
            if(flag)
            {
                PlaceToCleanList->DataForwardAndNextTeam();
                Task=400;
            }
            break;
        case 400:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=1;
                if(InArmSuck.HasIC()==false)
                {
                    return true;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int CheckShuttleSensor_Clean(eWhichShuttle iSht, bool alarmflag)
{
    int ret=0;
    switch(TestIF.iTestMode)
    {
        case SingleSite:                                                        //1x1
            ret=CheckShuttleSensor_9045_1x1(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;                                                              //Ifor 20180313 (Steven) : add 避免 1x1 蝦頭不會搖搖
        case DualSite:                                                          //1x2
        case QualSite2X2N:                                                      //Steven 20201014 : for 2x2 nn mode auto clean
            ret=CheckShuttleSensor_9045_1x2(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case QualSite1X4:                                                       //1x4
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
        case _8Site2X4N:                                                        //Wei 20231211 : 2X4NN Mode
            ret=CheckShuttleSensor_9045_1x4(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case QualSite2X2:                                                       //2x2
            ret=CheckShuttleSensor_9045_2x2(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case TriSite1X3:                                                        //Frank 20160323 1x3 AutoClean Add
        case _6Site2X3N:                                                        //Steven 20220425 : 2X3NN Mode
        case _6Site2X3:                                                         //ChungHung 20150119 add for 2x3 mode autoclean
            ret=CheckShuttleSensor_9045_2x3(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case _16Site4X4:                                                        //Sam 20190226 : 16Site4X4
        case _8Site2X4:                                                         //2x4
            ret=CheckShuttleSensor_9045_2x4(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case _10Site2X5:                                                        //wei 20190614 10 site
            ret=CheckShuttleSensor_9045_2x5(iSht, alarmflag, true);
            break;
        case _12Site2X6:
            ret=CheckShuttleSensor_9045_2x6(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        case _16Site2X8:                                                        //2x8
        case _32Site4X8N:                                                       //Steven 20140512: For HT-9047
            ret=CheckShuttleSensor_9045_2x8(iSht, alarmflag, true);             //JerryYang 20191113 新增alarm code區分in shuttle floating error/ clean pad floating error
            break;
        default:
            ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "CheckShuttleSensor_Clean");
    }
    return ret;
}
//==============================================================================
bool CheckAutoCleanCloseSite(int iSht)                                          //Steven 20220929 : 判斷開關Site
{
    bool bResult=false;
    if(bUseTwoArm32Site==true)
    {
        bResult=(InArmSideAllClose(iSht));
    }
    return bResult;
}
//------------------------------------------------------------------------------
void DoAutoCleanKit()                                                           //ChungHung 20130701 add 修改AutoClean 流程
{
    AnsiString Message;
    int ret=0;
    int &Task=iDoAutoCleanTask;
    int flag=0, iCleanPadCount=0;
    if((bLockPlaceToShuttleByAutoClean ||
        bLockPickFromShuttleByAutoClean) &&
       bPlaceToCleanKit==false &&
       bPickFromKitByAutoClean==false)                                          //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion    //Ifor 20191104 : fix 雙Arm Auto Clean InArm 丟料至Clean Kit Index2 Drop 要InArm 讓位互卡 Hangup
        return;

    if(fContact->fShow==true && iContactMode==CONTACT_DEVICE_MAP_CHECK)         //Steven 20220510 : For QTI SD Device Map Function
        return;

    static bool bShakeFlag, bVibration;                                         //Steven 20210616 : Auto Clean也要震動

    bool bIsSuckICFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL]={false}, bHasErr=false;
    AnsiString ErrPart="";
    bool bSupport2Arm;

    if(IniConfig.bF16CheckShuttleSensorBroken && bDoingF16)                     //Steven 20221213 : 確認shuttle 有沒有斷線
        return;

    int iSuckRow=0;                                                             //ChungHung 20141121 add for Use HotPlate AutoClean

    bool bDoAutoCleanAutoAlignment;
    if(USE_IN_Y_IS_AUTO_PITCH==true && CUSTOMER_CODE==CC_ASE_KaohSiung)         //JerryYang 20251218 : IN/OUT ARM支援不同模組      //KenHsieh 20220103 : 新增ASEKH判定
    {
        if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==true &&
           (LastSet.iRealDummy==HAS_TRAY || LastSet.iRealDummy==REALLY))        //KenHsieh 20211214 : AOA add AutoClean
        {
            if(lInArmAutoAlignmentCKTimingFlag || bRunInArmAutoAlignment)
            {
                if(iCleanOut==1 && HasICUnderMachine()==false)                  //process clean out ,don't supply new tray
                {
                    return;
                }
                else
                {
                    bDoAutoCleanAutoAlignment=CheckInArmAutoAlignmentCKModeBeUse(lInArmAutoAlignmentCKTimingFlag, true);
                    bRunInArmAutoAlignment=bDoAutoCleanAutoAlignment;
                    return;
                }
            }
        }
        else
        {
            bRunInArmAutoAlignment=false;
        }
    }

    bUse8Picker=false;                                                          //Steven 20201014 : 整合8吸嘴auto clean
    if(IniConfig.bE43AutoCleanUseHotplate)
    {
        bUse8Picker=true;
    }
    else if(MachineTypeChoice==Type_HT9046_LS ||
            MachineTypeChoice==Type_HT1032)
    {
        if(SubMachineType==Type_None)
            bUse8Picker=true;
        else if(USE_IN_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
            bUse8Picker=true;
    }

    if(IniConfig.bE43AutoCleanUseHotplate)
        iSuckRow=0;
    else
        iSuckRow=1;

    if(DoInArmPineRelease()==false)
        return;

    SocketAirCoolingStart();                                                    //jou 2016-04-28 Socket Air Cooling contact count trun on
    //bUseFix3CylinderActive=false;                                             //Sam 20240807 : Mark  //Ifor 20180329 : add 避免 Auto Clean 搖搖功能卡死
    switch(Task)
    {
        case 1:
            if(FIX3_FULL_PLACE==Fix3K_UseCylinder &&
               bUseFix3CylinderActive==true)                                    //Ifor 20210226 Fix: Auto Clean Hang up
            {
                InitialFix3CanFullTask();
                Task=2;
            }
            else
            {
                DoStructUnitConvert();
                DoInArm_9045_Type();                                            //Steven 20201014 : 將DoInArm_9045_Type獨立,避免DoInArm_9045還沒執行就被使用
                DoInArm_SuckerMap();                                            //JerryYang 20190805 add
                Cylinder[C_HotplateVibration].Off();                            //JerryYang 20191123 fix auto clean過程中馬達不停振動
                Cylinder[C_TrayVibration].Off();
                PlaceToCleanList->ClearGroupList();                             //JerryYang 20241219 : initial data避免資料沒清乾淨

                MOT[MInShuttle1].fCanMoveM=true;                                //Steven 20220506 : 避免shuttle被鎖住,導致Auto Clean死機
                MOT[MInShuttle2].fCanMoveM=true;
                ZeroMemory(CleanKitRecord , sizeof(CleanKitRecord));            //Sam 20230619 : 新增 Clean吸放時間 Log
                if(CosFunction.bFullTestBeforeAutoClean &&                      //JerryYang 20160331 SPIL中科要求Auto clean前做 full view check
                   REAL_TIME_CCD==true &&
                   !COM2->bCCDDummyRum)
                {
                    RecordProcess("Start Full View Check...");
                    fContact->InitDoFullViewCheck();
                    bFullViewCheckFinish=false;
                    Task=3;                                                     //full view check
                }
                else
                {
                    bFullViewCheckFinish=true;
                    Task=5;                                                     //auto clean
                }
            }
            break;
         case 2:
            if(UseFix3Cylinder(0)==true)
            {
                Task=1;
            }
            break;
        case 3:
            if(fContact->DoFullViewCheck()==true)
            {
                bFullViewCheckFinish=true;
                Task=5;
            }
            break;
        case 5:
            GetXPitchOfCleanKit();
            RecordProcess("Start Auto Cleaning...");                            //Steven 20130614 : Auto Clean加上紀錄
            bAutoCleaning=true;                                                 //JerryYang 20151109 add for 力成 AutoClean旗標
            SW[SwTesterAirCooling].Off();                                       //jou 2016-04-28 Socket Air Cooling contact count trun on
            iL03SocketAirCoolingCT=0;                                           //jou 2016-04-28 Socket Air Cooling contact count trun on
                                                                                //Ifor 20170425 (wei) Auto Clean Start 傳送次數與Auto Clean End 無法匹配 InitialAutoCleanTask 移至 DoAutoCleanKit 流程中上報
            if(IniConfig.bEnable_SECS_GEM==true)                                //Steven 20140528 : Secs Gem
                EventReport(SECS_EVENT.DoAutoClean);                            //34     Auto Clean Start
            #ifdef DEBUG_AUTO_CLEAN
                Message.sprintf("\\--------------- DoAutoCleanKit - Start Auto Clean");
                fMain->AddAutoCleanMessage(Message);
            #endif

//            if(TestIF.iAutoClean_Function==true         &&                      //Steven 20241111 : Mark for 1x4 run 3 col tray auto clean //Ifor 20180227 (Steven) : add 1*4 模式無法使用 3*x Tray from
//               fCleaning->rgAutoCleanOnOff->ItemIndex==1 &&
//               (TestIF.iTestMode==QualSite1X4 ||
//                TestIF.iTestMode==_8Site2X4N) &&                                //Wei 20231211 : 2X4NN Mode
//               iInArmType!=e9045_1x4_1_Ac &&                                    //Steven 2200720 : 1x4只開site Ac
//               fCleaning->rgCleanKitType->ItemIndex==1)
//            {
//                if(TrayForm.Loader.XDivision==3)                                //Steven 20220927 : <4 --> ==3
//                {
//                    bErrorAutoClean=true;
//                    Task=2000;
//                    break;
//                }
//            }
            iCleanPadCount=TrayHasCleanICCount();                               //wei 20150904
            if(iCleanPadCount>=TestIF_File.iAutoClean_DeveicePices)
            {
                Task=20;
            }
            else
            {
                if((TestIF_File.iTestMode==_6Site2X3 && iCleanPadCount>=6) ||   //Steven 20171220 (Wei) : Fixed for 2x3 Auto Clean Hang Up
                   (TestIF_File.iTestMode==_10Site2X5 && iCleanPadCount>=10))   //KEVIN 20220113 ADD 2x5 site
                {
                    Task=20;
                    break;
                }

                bErrorAutoClean=true;
                Task=2000;
            }
            break;
        case 10:                                                                //Steven 20130620 : 避免Clean Count到的時候，被關Arm導致Hang Up，回傳值從bool改為int
            InitialShuttleAutoCleanTask();
            InitialIndexAutoCleanTask();
            Task=20;
        case 20:
            if(MoveInArmZToPlateSafe(Task))                                     //in arm z移至安全位置
            {
                InitialSet();                                                   //kevin 20150720

                if(CosFunction.bAutoCleanAutoSelIndexArm==false)
                {
                    if(TestIF.iAutoClean_SelectArm!=1)                          //ChungHung 20131218 add for SCK request  //0:Arm1 1:Arm2 2:Arm1 & Arm2
                        MOT[MInShuttle1].fCanMoveL=true;
                    if(TestIF.iAutoClean_SelectArm!=0)
                        MOT[MInShuttle2].fCanMoveL=true;

                    if(TestIF.iAutoClean_SelectArm==1)
                    {
                        Task=2100;
                    }
                    else
                    {
                        Task=100;
                    }

                    if(TestIF_File.iAutoClean_SelectArm==2)                     //wei 20220801 2x3 NN Mode
                    {
                        if(CheckAutoCleanCloseSite(0))                          //Steven 20220929 : 判斷開關Site
                        {
                            Task=2100;
                        }
                        else
                        {
                            Task=100;
                        }
                    }
                }
                else
                {
                    MOT[MInShuttle1].fCanMoveL=true;
                    if(TestIF.bCleanIndexOtherArm==true)
                        MOT[MInShuttle2].fCanMoveL=true;

                    if(TestIF.bCleanIndexOtherArm==false &&
                       TestIF_File.iShuttleMode==1 &&
                       TestIF_File.iShuttle_Sel==1)
                    {
                        Task=2100;
                    }
                    else
                    {
                        Task=100;
                    }
                }
            }
            break;
        case 100:
            if(CheckInArmSuckInitial()==false)                                  //In Arm 吸嘴上如果有 Device 就 Alarm
                break;

            //AI(ht9045-v899) 20260408: purge phantom HAS_NULL_CLEAN_IC from InArm before dispatch
            if(bUseCKPP && InArmSuck.HasIC() && !InArmSuck.HasDefineIC(HAS_CLEAN_IC) && !InArmSuck.HasDefineIC(CLEAN_FINISH_IC))
            {
                RecordProcess("AC_PURGE_PHANTOM InArm has only HAS_NULL_CLEAN_IC, clearing");
                for(int pr=0; pr<InArmSuck.iMaxRow; pr++)
                    for(int pc=0; pc<InArmSuck.iMaxCol; pc++)
                        if(InArmSuck.Item[pr][pc]==HAS_NULL_CLEAN_IC)
                            InArmSuck.SetItemData(pr, pc, NULL_IC);
            }
            if(InArmSuck.HasIC()==false                &&                       //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
               b1ShuttleMoveToLeft==true               &&                       //ChungHung 20131120 AutoClean use Hotplate1
               (FTestSuck.HasDefineIC(HAS_CLEAN_IC)    ||
                FTestSuck.HasDefineIC(CLEAN_FINISH_IC) ||
                FLCarryKit.HasDefineIC(HAS_CLEAN_IC)   ||
                FLCarryKit.HasDefineIC(CLEAN_FINISH_IC)))
            {
                Task=540;
            }
            else
            {
                if(InArmSuck.HasDefineIC(CLEAN_FINISH_IC))
                {
                    Task=1100;
                }
                else if(InArmSuck.HasDefineIC(HAS_CLEAN_IC))
                {
                    InitPlaceToShuttleTask();
                    Task=400;
                }
                else
                {
                    Task=200;
                }
            }
            break;
        case 200:
            DoAutoCleanPickfromCleanKit(euShuttle1, true);                      //initial Task
            Task=300;
            break;
        case 300:
            bPickFromKitByAutoClean=true;                                       //Steven 20210603 : 新增吸放Clean Kit的Flag
            flag=DoAutoCleanPickfromCleanKit(euShuttle1, false);
            if(flag==1)
            {
                bPickFromKitByAutoClean=false;                                  //Steven 20210603 : 新增吸放Clean Kit的Flag
                if(InArmSuck.HasIC()==false)
                {
                    Task=200;                                                   //沒吸到重來
                    if(FLCarryKit.FindNoIC()==false)                            //kevin 20180425 SHUTTLE 有NULL IC
                        Task=530;
                }
                else
                {
                    InitPlaceToShuttleTask();                                   //吸到了放到shuttle上
                    Task=400;
                }
            }
            else if(flag==2)                                                    //Steven 20130620 : 避免Clean Count到的時候，被關Arm導致Hang Up，回傳值從bool改為int
            {
                bPickFromKitByAutoClean=false;                                  //Steven 20210603 : 新增吸放Clean Kit的Flag
                Task=10;
            }
            break;
        case 400:
            bPlaceToShuttleByAutoClean=true;                                    //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
            if(DoPlaceToShuttle(euShuttle1))
            {
                bPlaceToShuttleByAutoClean=false;                               //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                iShuttleRowKit=GetShuttleState(euShuttle1, bAutoPlace);

                if(USE_PICKER_COUNT==ep1Picker)
                {
                    if(IsFLCarrKitAllHasIC()==false)
                    {
                        Task=200;
                    }
                    else
                    {
                        Task=500;
                    }
                }
                else
                {
                    //AI(ht9045-v899) 20260417: revert to V898 logic - premature CLEAN_FINISH_IC recycling caused pick to restart from col 0 instead of continuing right half
                    if(iShuttleRowKit!=0)
                    {
                        Task=200;                                               //Shuttle 沒放滿要再去Clean Kit吸Device
                    }
                    else
                    {
                        Task=500;
                    }
                }
            }
            break;
        case 500:                                                               //Pick from shuttle ,place to Clean Kit
            if(MoveInArmZToPlateSafe(Task))
            {
                bShakeFlag=false;
                bVibration=false;                                               //Steven 20210616 : Auto Clean也要震動
                if(IniConfig.bF01ShakeShuttleWhenJam==false && IniConfig.bF23ShuttleVibration==false)
                    bShakeFlag=true;
                Task=510;
            }
            break;
        case 510:
            if(CosFunction.bAutoCleanShuttleDisable==true)                      //jou 2013-02-27 Auto Clean disable shuttle sensor detect
            {
                if(IniConfig.bAutoCleanShuttleDisable)                          //jou 2013-02-27 Auto Clean disable shuttle sensor detect
                {
                    Task=515;
                    break;
                }
            }
            else
            {
                if(iAutoCleanShuttle==0)
                {
                    Task=515;
                    break;
                }
            }

            ret=CheckShuttleSensor_Clean(euShuttle1, bShakeFlag);
            if(ret==1)
            {
                bShakeFlag=false;
                bVibration=false;                                               //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
            }
            else if(ret==3)
            {
                Task=511;                                                       //Steven 20220728 : 做個紀錄
                break;
            }
            else if(ret==2)
            {
                if(IniConfig.bF23ShuttleVibration &&                            //Steven 20210616 : Auto Clean也要震動
                   ((IniConfig.bF01ShakeShuttleWhenJam==true && bVibration==false) ||
                     IniConfig.bF01ShakeShuttleWhenJam==false))                 //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
                {
                    if(IniConfig.bF01ShakeShuttleWhenJam==true)
                    {
                        bShakeFlag=false;
                    }
                    else
                    {
                        bShakeFlag=true;
                    }
                    bVibration=true;
                    Task=522;
                    DoVibrateShuttle(euShuttle1, true);                         //JerryYang 20190123 shuttle震動馬達
                }
                else if(bShuttleKnock==false)                                   //Jou 2013-03-08 修改敲敲功能
                {
                    if(IniConfig.bF01ShakeShuttleWhenJam==false &&
                       IniConfig.bF23ShuttleVibration==false)                   //kevin 20190731 不使用搖搖
                    {
                        bShakeFlag=true;
                        break;
                    }
                    Task=520;
                    DoShakeShuttle(euShuttle1, true);                           //Steven 20120801 : 修改抖抖功能 (true為初始化)
                }
                else
                {
                    bShakeFlag=true;
                    DoKnockShuttle(euShuttle1, true);                           //jou 2013-07-17 Knock Shuttle(true為初始化)
                    Task=521;
                }
                break;
            }
            else
            {
                Task=515;
            }
            break;
        case 511:                                                               //Steven 20220728 : 做個紀錄
            Task=510;
            break;
        case 515:
            MOT[MInShuttle1].fCanMoveL=true;

            if(CosFunction.bAutoCleanAutoSelIndexArm==false)
            {
                if(TestIF.iAutoClean_SelectArm==0)
                {
                    Task=540;
                }
                else if(TestIF.iAutoClean_SelectArm==2)
                {
                    if(CheckAutoCleanCloseSite(1)==false)                       //Steven 20220929 : 判斷開關Site
                    {
                        if(MOT[MMAutoCleanKit].Tray.HasDataIC(HAS_CLEAN_IC))    //Steven 20210315 : 修正雙arm auto clean只有一半的clean pad
                            Task=2100;
                        else
                            Task=540;
                    }
                    else
                    {
                        Task=540;
                    }
                }
            }
            else
            {
                if(TestIF.bCleanIndexOtherArm==false)                           //使用兩支arm進行auto clean可選擇one arm or two arm進行
                {
                    Task=540;
                }
                else
                {
                    if(CheckAutoCleanCloseSite(1)==false &&                     //Steven 20220929 : 判斷開關Site
                       MOT[MMAutoCleanKit].Tray.HasDataIC(HAS_CLEAN_IC))        //Steven 20210315 : 修正雙arm auto clean只有一半的clean pad
                        Task=2100;
                    else
                        Task=540;
                }
            }
            break;
        case 520:
            if(DoShakeShuttle(euShuttle1))                                      //Steven 20120801 : 修改抖抖功能
            {
                Task=530;
            }
            break;
        case 521:                                                               //Steven 20210616 : Auto Clean也要震動
            if(DoKnockShuttle(euShuttle1))                                      //Jou 2013-03-08 修改敲敲功能
            {
                Task=530;
            }
            break;
        case 522:                                                               //Steven 20210616 : Auto Clean也要震動
            if(DoVibrateShuttle(euShuttle1))                                    //JerryYang 20190123 shuttle震動馬達
            {
                Task=530;
            }
            break;
        case 530:
            if(InSHT1InLF())
            {
                MOT[MInShuttle1].fCanMoveL=false;
                bShuttleShake=false;
                IndexZCanMove[0]=true;
                IndexZCanMove[1]=true;
                CleanSetSpeed(true);
                Task=510;
            }
            break;
        case 540:
            if(MoveInArmXYToShuttle_9045(euShuttle1, 0, ZAxisNotDown, false))
            {
                Task=600;
            }
            break;
        case 600:
            if(FLCarryKit.UseSiteNoIC() &&
               InSHT1InLF()              &&
               FTestSuck.UseSiteNoIC())
            {
                Task=1200;
            }
            else
            {
                if(InSHT1InLF() &&                                              //Steven 20190212 : 修正Hang up
                   bInedxCleanFinish[0])                                        //Steven 20200730 : 修正Hang up
                    iShuttleRowKit=GetShuttleState(euShuttle1, bAutoPick);
                else
                    iShuttleRowKit=0;

                if(iShuttleRowKit!=0 && InSHT1InLF() &&
                   (FLCarryKit.HasType(CLEAN_FINISH_IC) ||                      //JerryYang 20160123 修正當index arm掉料後,shuttle只剩下HAS_NULL_IC會hang up //JerryYang 20151124 Fix hang up,Shuttle上有 CLEAN_FINISH_IC才能吸取CleanPad
                    (bInedxCleanFinish[0] && FLCarryKit.HasType(HAS_NULL_CLEAN_IC))))//kevin 20170518 (wei) add 全部IC掉落Hang up
                {
                    MOT[MInShuttle1].fCanMoveL=false;
                    InitPickFromShuttleTask();
                    if(CUSTOMER_CODE==CC_KYEC_LEE ||
                       CosFunction.bHiSiliconFunction)
                    {
                        bShakeFlag=false;
                        if(IniConfig.bF01ShakeShuttleWhenJam==false &&
                           IniConfig.bF23ShuttleVibration==false)
                            bShakeFlag=true;
                        Task=610;
                    }
                    else
                    {
                        Task=800;
                    }
                }
            }
            break;
        case 610:
            if(CosFunction.bAutoCleanShuttleDisable==true)                      //jou 2013-02-27 Auto Clean disable shuttle sensor detect
            {
                if(IniConfig.bAutoCleanShuttleDisable)                          //jou 2013-02-27 Auto Clean disable shuttle sensor detect
                {
                    Task=800;
                    break;
                }
            }
            else
            {
                if(iAutoCleanShuttle==0)
                {
                    Task=800;
                    break;
                }
            }

            ret=CheckShuttleSensor_Clean(euShuttle1, bShakeFlag);
            if(ret==1)
            {
                bShakeFlag=false;
                if(IniConfig.bF01ShakeShuttleWhenJam==false &&
                   IniConfig.bF23ShuttleVibration==false)
                    bShakeFlag=true;
            }
            else if(ret==3)
            {
                break;
            }
            else if(ret==2)
            {
                Task=620;
                bShakeFlag=true;
                DoShakeShuttle(euShuttle1, true);                               //Steven 20120801 : 修改抖抖功能 (true為初始化)
                break;
            }
            else
            {
                Task=800;
            }
            break;
        case 620:
            if(DoShakeShuttle(euShuttle1))                                      //Steven 20120801 : 修改抖抖功能
            {
                Task=630;
            }
            break;
        case 630:
            if(InSHT1InLF())
            {
                MOT[MInShuttle1].fCanMoveL=false;
                bShuttleShake=false;
                IndexZCanMove[0]=true;
                IndexZCanMove[1]=true;
                CleanSetSpeed(true);
                Task=610;
            }
            break;
        case 800:
            bPickFromShuttleByAutoClean=true;                                   //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
            if(DoPickFromShuttle(euShuttle1, iShuttleRowKit))
            {
                if(bUse8Picker && iCloseSiteModeFor2x8==e2x8_STMMode ||         //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
                                  iCloseSiteModeFor2x8==e2x8_TW153Mode)
                {
                    if(FLCarryKit.LeftSideNoIC(4)==true && FLCarryKit.RightSideNoIC(4)==false)
                    {
                        iShuttleRowKit=GetShuttleState(euShuttle1, bAutoPick);
                        break;
                    }
                }

                bPickFromShuttleByAutoClean=false;                              //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                Task=1000;
            }
            break;
        case 1000:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=1050;
            }
            break;
        case 1050:
            if(LastSet.iRealDummy==REALLY)
            {
                ptrInSHT=&FLCarryKit;

                if(iShuttleRowKit==1 || iShuttleRowKit==2)                      //Kit 1
                    ret=0;
                else                                                            //kit 2
                    ret=4;

                for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                   //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Suck[i][j].Enable       &&
                           InArmSuck.Suck[i][j].SenUsing!="" &&
                           InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC)
                        {
                            if(InArmSuck.Suck[i][j].GetStatus()==false)
                            {
                                bIsSuckICFallDown[i][j]=true;
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                                bHasErr=true;
                            }
                        }
                    }
                }
            }

            if(bHasErr)
            {
                ret=ShowErrorMessage("JAM0128", K_SKIP, MInArmX, false, ErrPart); //Steven 20091123 : Device Drop Error //JerryYang 20160511 JAM0126->JAM0128,將IC及Clean pad相關的alarm code分開

                for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                   //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Suck[i][j].Enable       &&
                           InArmSuck.Suck[i][j].SenUsing!="" &&
                           InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC     &&
                           InArmSuck.Suck[i][j].GetStatus()==false)
                        {
                            InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);
                            bIsSuckICFallDown[i][j]=true;                       //ChungHung 20141121 add for Use HotPlate AutoClean
                        }
                    }
                }

                if(SoftStop)
                    fMain->Pause("DoAutoCleanKit 1050");

                Task=1100;                                                      //JerryYang 20160125 Device drop error only skip
            }
            else
            {
                Task=1100;
            }
            break;
        case 1100:
            if(InArmSuck.HasIC()==false)
            {
                Task=600;
                break;
            }
            MOT[MInShuttle1].fCanMoveL=true;                                    //ChungHung 20150410 add because case 2600 lock here need unlock
            DoAutoCleanPlaceToCleanKit(true);
            if(TestIF_File.iAutoClean_Tray==eCKPos_Fix3)                        //JerryYang 20161219 (Steven) 避免Auto clean hang up
            {
                if(iCatchTrayControlManual>=2)                                  // catch tray busy
                    break;
            }
            Task=1200;
            break;
        case 1200:
            bPlaceToCleanKit=true;                                              //Steven 20171204 (Wei) : 避免放到一半要讓位不回來原本位置放
            if(DoAutoCleanPlaceToCleanKit(false))
            {
                if(InArmSuck.HasIC()==false)
                {
                    if(FLCarryKit.UseSiteHasIC())                               //JerryYang 20160123 修正當index arm掉料後,shuttle只剩下HAS_NULL_IC會hang up
                    {
                        Task=600;
                    }
                    else
                    {
                        if(CosFunction.bAutoCleanAutoSelIndexArm==false)
                        {
                            if(TestIF.iAutoClean_SelectArm!=2)                  //ChungHung 20131218 add for SCK request  //0:Arm1 1:Arm2 2:Arm1 & Arm2
                            {
                                Task=2000;
                            }
                            else
                            {
                                if(CheckAutoCleanCloseSite(1))                  //wei 20220801 2x3 NN Mode   //Steven 20220929 : 判斷開關Site
                                {
                                    Task=2000;
                                }
                                else
                                {
                                    Task=1300;
                                }
                            }
                        }
                        else
                        {
                            if(TestIF.bCleanIndexOtherArm==false ||
                               CheckAutoCleanCloseSite(1)==true)                //Steven 20220929 : 判斷開關Site
                            {
                                Task=2000;
                            }
                            else
                            {
                                Task=1300;
                            }
                        }
                    }
                    bPlaceToCleanKit=false;                                     //Steven 20171204 (Wei) : 避免放到一半要讓位不回來原本位置放
                }
                else
                {
                    Task=1000;
                }

                if(Task==1300)                                                  //Steven 20220926 : Auto Clean做完Arm 1後多餘的動作
                {
                    if(BLCarryKit.UseSiteNoIC() &&
                       BTestSuck.UseSiteNoIC())
                    {
                        Task=2100;
                    }
                }
            }
            break;
        case 1300:
            if(MoveInArmXYToShuttle_9045(euShuttle2, 0, ZAxisNotDown, false))
            {
                bSupport2Arm=fCleaning->CleanPadCountCanSupport2Arm();          //Steven 20221006 : 修正雙arm auto clean只有一半的clean pad
                if(bSupport2Arm)
                    Task=2600;
                else
                    Task=2100;
            }
            break;
        case 2000:
            if(FLCarryKit.HasType(HAS_CLEAN_IC) || FLCarryKit.HasType(HAS_NULL_CLEAN_IC) ||    //JerryYang 20220216 : 新增保護，避免auto clean流程結束的時候 suhttle或index arm還有clean pad造成後續將clean pad送測試訊號
               BLCarryKit.HasType(HAS_CLEAN_IC) || BLCarryKit.HasType(HAS_NULL_CLEAN_IC) ||
               FTestSuck.HasType(HAS_CLEAN_IC)  || FTestSuck.HasType(HAS_NULL_CLEAN_IC) ||
               BTestSuck.HasType(HAS_CLEAN_IC)  || BTestSuck.HasType(HAS_NULL_CLEAN_IC))
            {
                fAllMotorHome=false;
                ShowMyMessage("Auto clean process error, need home!");
                break;
            }

            if(CosFunction.bAutoCleanAutoSelIndexArm==false)
            {
                if(TestIF.iAutoClean_SelectArm!=1)
                    MOT[MInShuttle1].fCanMoveL=true;
                if(TestIF.bCleanIndexOtherArm!=0)
                    MOT[MInShuttle2].fCanMoveL=true;
            }
            else
            {
                MOT[MInShuttle1].fCanMoveL=true;
                if(TestIF.bCleanIndexOtherArm==true)
                    MOT[MInShuttle2].fCanMoveL=true;
            }

            bRunAutoClean=false;
            if(HasICUnderMachine()==true || HasAnyICInMachine()==true)          //JerryYang 20250305 : fix Initial的auto clean之後沒有做index check
            {
            bAutoCleanFinishOnlyUseRTC=true;                                    //JerryYang 20161216 (Steven) 做完auto clean後只靠RTC檢查Socket,不做index check
            }

            //if(CosFunction.bSmartAutoClean && bRunACAdaptive)                 //Sam 20240726 : AI Clean Mark  //Sam 20230620 : 優化 Smart Auto Clean
            //{
                                                                                //不要清除
            //}
            //else
            //{
                iAutoClean_IndexContactCount=0;
            //}
            lAutoClean_TimeCount=0;                                             //jou 20250102 : auto clean triger time count
            if(iCleanOut==1)                                                    //Steven 20140725
                iCheckFinish_ByAutoClean=2;                                     //pig 2011.09.01 AutoClean

            CleanSetSpeed(false);
            iCloseSiteState=CloseSiteState();

            if(bErrorAutoClean)                                                 //JerryYang 20160520 修正auto clean發生error時 不能修改clean kit參數的問題
            {
                ShowErrorMessage("WAR16313", K_RETRY, MMSystem);                //kevin 20200527 add Autoclean count Alarm
                RecordProcess("Error Auto Clean Finish!");                      //Steven 20130614 : Auto Clean加上紀錄
            }
            else
            {
                RecordProcess("Auto Clean Finish!");                            //Steven 20130614 : Auto Clean加上紀錄
            }
            if(bBackupOneCycle_ByAutoClean)                                     //Sam 20230309 : 避免觸發 OneCycle 後，OneCycle 未完成時又觸發 AutoClean 動作，AutoClean 做完並不會提示 OneCycle Finish
            {
                bBackupOneCycle_ByAutoClean=false;
                NewRecordProcess("MES2115", "ONE CYCLE pressed", "Auto Clean Finish");
                fMain->BtnOneCycleClick(fMain);
            }
            ZeroMemory(iAutoCleanByBinCount, sizeof(iAutoCleanByBinCount));
            ZeroMemory(iAutoCleanBySiteCount, sizeof(iAutoCleanBySiteCount));
            bAutoCleaning=false;                                                //JerryYang 20151109 add for 力成 AutoClean旗標
            fContactCT->ACSmartClearData();                                     //Sam 20230111 : Smart Auto Clean
            //if(CosFunction.bSmartAutoClean)                                   //Sam 20240726 : AI Clean Mark  //Sam 20230620 : 優化 Smart Auto Clean
            //{
            //    if(bRunACAdaptive)
            //        iYeildCT[1]=RunInfo.iUnloadCount;
            //    bRunACAdaptive=false;
            //}
            ReadWriteAutoCleanCount(false);                                     //Steven 20180524 : Fixed for clean count
            SearchCleanNum();
            bIndexCheckState=true;                                              //wei 20150903
            bErrorAutoClean=false;                                              //wei 20150904
            if(IniConfig.bEnable_SECS_GEM==true)                                //Steven 20140528 : Secs Gem
                EventReport(SECS_EVENT.AutoCleanFinish);                        //50     Auto Clean Finish
            #ifdef DEBUG_AUTO_CLEAN
                Message.sprintf("DoAutoCleanKit, Auto Clean Finish");
                fMain->AddAutoCleanMessage(Message);
            #endif

            if(CUSTOMER_CODE==CC_SPIL_TAICHUNG_LOGIC &&
               IniConfig.bFTPJamCodeUpload && IniConfig.bEnableFTP)             //Steven 20140917 : 台中SPIL要求Auto Clean要傳送Alarm Code
            {
                fNote->aJamCodeFilePath=fFTPClient->SaveJamCodeFile(IniConfig.SocketHandlerID, Now(), "MES1608", "Autoclean Finish");
                fFTPClient->UploadFileToServer2(IniConfig.FtpUplaodPath, fNote->aJamCodeFilePath);    //Steven 20140513 : 移動到Form Show之前
            }

            fLtcSensor->ClearLtcSensor(1);                                      //Sam 20221101 : Latch 清除都要確認是否清清乾淨
            fLtcSensor->ClearLtcSensor(0);                                      //Sam 20221101 : Latch 清除都要確認是否清清乾淨
            fLtcSensor->SetLtcSensor(0);                                        //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2  //Steven 20140805 : 將Latch清空，確保沒有問題
            fLtcSensor->SetLtcSensor(1);                                        //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
            DoInArm_SuckerMap();                                                //kevin 20161007 Auto Clean開關site 吸嘴使用異常修正
            break;
        case 2100:                                                              //Clean Other Arm start
            if(CheckInArmSuckInitial()==false)
                break;

            if(MoveInArmZToPlateSafe(Task))                                     //in arm z移至安全位置
            {
                if(InArmSuck.HasIC()==false                &&                   //Steven 20130620 mark: Auto Clean到一半按歸零要繼續跑
                   b2ShuttleMoveToLeft==true               &&                   //ChungHung 20131120 AutoClean use Hotplate1
                   (BTestSuck.HasDefineIC(HAS_CLEAN_IC)    ||
                    BTestSuck.HasDefineIC(CLEAN_FINISH_IC) ||
                    BLCarryKit.HasDefineIC(HAS_CLEAN_IC)   ||
                    BLCarryKit.HasDefineIC(CLEAN_FINISH_IC)))
                {
                    Task=2540;
                }
                else
                {
                    if(InArmSuck.HasDefineIC(CLEAN_FINISH_IC))
                    {
                        Task=3100;
                    }
                    else if(InArmSuck.HasDefineIC(HAS_CLEAN_IC))
                    {
                        InitPlaceToShuttleTask();
                        Task=2400;
                    }
                    else
                    {
                        Task=2200;
                    }
                }
            }
            break;
        case 2200:
            DoAutoCleanPickfromCleanKit(euShuttle2, true);                      //initial Task
            Task=2300;
            break;
        case 2300:
            bPickFromKitByAutoClean=true;                                       //Steven 20210603 : 新增吸放Clean Kit的Flag
            flag=DoAutoCleanPickfromCleanKit(euShuttle2, false);
            if(flag==1)
            {
                bPickFromKitByAutoClean=false;                                  //Steven 20210603 : 新增吸放Clean Kit的Flag
                if(InArmSuck.HasIC()==false)
                {
                    Task=2200;
                    if(BLCarryKit.FindNoIC()==false)                            //kevin 20180425 SHUTTLE 有NULL IC
                        Task=2530;
                }
                else
                {
                    InitPlaceToShuttleTask();
                    Task=2400;
                }
            }
            else if(flag==2)                                                    //Steven 20130620 : 避免Clean Count到的時候，被關Arm導致Hang Up，回傳值從bool改為int
            {
                Task=10;
            }
            break;
        case 2400:
            bPlaceToShuttleByAutoClean=true;                                    //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
            if(DoPlaceToShuttle(euShuttle2))
            {
                bPlaceToShuttleByAutoClean=false;                               //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                if(USE_PICKER_COUNT==ep1Picker)
                {
                    if(IsBLCarrKitAllHasIC()==false)
                    {
                        Task=2200;
                    }
                    else
                    {
                        Task=2500;
                    }
                }
                else
                {
                    iShuttleRowKit=GetShuttleState(euShuttle2, bAutoPlace);
                    //AI(ht9045-v899) 20260417: revert to V898 logic - premature CLEAN_FINISH_IC recycling caused pick restart from col 0 (ARM2)
                    if(iShuttleRowKit!=0)
                    {
                        Task=2200;                                              //Shuttle 沒放滿要再去Clean Kit吸Device
                    }
                    else
                    {
                        Task=2500;
                    }
                }
            }
            break;
        case 2500:                                                              //Pick from shuttle ,place to Clean Kit
            if(MoveInArmZToPlateSafe(Task))
            {
                bShakeFlag=false;
                bVibration=false;                                               //Steven 20210616 : Auto Clean也要震動
                if(IniConfig.bF01ShakeShuttleWhenJam==false && IniConfig.bF23ShuttleVibration==false)
                    bShakeFlag=true;
                Task=2510;
            }
            break;
        case 2510:
            if(CosFunction.bAutoCleanShuttleDisable==true)                      //jou 2013-02-27 Auto Clean disable shuttle sensor detect
            {
                if(IniConfig.bAutoCleanShuttleDisable)                          //jou 2013-02-27 Auto Clean disable shuttle sensor detect
                {
                    Task=2515;
                    break;
                }
            }
            else
            {
                if(iAutoCleanShuttle==0)                                        //JerryYang 20170327 (Jou) 修正Auto Clean中shuttle 2 Shuttle偵測不能disable的問題
                {
                    Task=2515;
                    break;
                }
            }

            ret=CheckShuttleSensor_Clean(euShuttle2, bShakeFlag);
            if(ret==1)
            {
                bShakeFlag=false;
                bVibration=false;                                               //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
            }
            else if(ret==3)
            {
                break;
            }
            else if(ret==2)
            {
                if(IniConfig.bF23ShuttleVibration &&                            //Steven 20210616 : Auto Clean也要震動
                   ((IniConfig.bF01ShakeShuttleWhenJam==true && bVibration==false) ||
                     IniConfig.bF01ShakeShuttleWhenJam==false))                 //JerryYang 20180711 (wei) Shuttle震動後還是置偏要接著搖
                {
                    if(IniConfig.bF01ShakeShuttleWhenJam==true)
                    {
                        bShakeFlag=false;
                    }
                    else
                    {
                        bShakeFlag=true;
                    }
                    bVibration=true;
                    Task=2522;
                    DoVibrateShuttle(euShuttle2, true);                         //JerryYang 20190123 shuttle震動馬達
                }
                else if(bShuttleKnock==false)                                   //Jou 2013-03-08 修改敲敲功能
                {
                    if(IniConfig.bF01ShakeShuttleWhenJam==false &&
                       IniConfig.bF23ShuttleVibration==false)                   //kevin 20190731 不使用搖搖
                    {
                        bShakeFlag=true;
                        break;
                    }
                    Task=2520;
                    DoShakeShuttle(euShuttle2, true);                           //Steven 20120801 : 修改抖抖功能 (true為初始化)
                }
                else
                {
                    bShakeFlag=true;
                    DoKnockShuttle(euShuttle2, true);                           //jou 2013-07-17 Knock Shuttle(true為初始化)
                    Task=2521;
                }
                break;
            }
            else
            {
                Task=2515;
            }
            break;
        case 2515:
            MOT[MInShuttle2].fCanMoveL=true;

            if(TestIF.iAutoClean_SelectArm==2 ||                                //jou 2016-05-11 修正Auto Clean POP function異常
               TestIF_File.bCleanIndexOtherArm)                                 //JerryYang 20160307 使用雙Arm Input arm要到shuttle1等待
            {
                bSupport2Arm=fCleaning->CleanPadCountCanSupport2Arm();          //Steven 20221006 : 修正雙arm auto clean只有一半的clean pad
                if(bSupport2Arm)
                    Task=2518;
                else
                    Task=2540;
            }
            else
            {
                Task=2540;
            }

            if(Task==2540)                                                      //Steven 20220822 : 判斷Shuttle 1是不是還有Clean Pad, 避免Hang up
            {
                if(InArmSuck.HasIC()==false &&
                   (FLCarryKit.HasDefineIC(HAS_NULL_CLEAN_IC) ||
                    FLCarryKit.HasDefineIC(HAS_CLEAN_IC)      ||                //Jimmychiu 20221028 add HAS_CLEAN_IC
                    FLCarryKit.HasDefineIC(CLEAN_FINISH_IC)   ||
                    FTestSuck.HasDefineIC(HAS_CLEAN_IC)       ||
                    FTestSuck.HasDefineIC(CLEAN_FINISH_IC)))
                {
                    Task=2518;
                }
            }
            break;
        case 2518:
            if(InArmSuck.HasIC()==false && b1ShuttleMoveToLeft==true &&
               (FLCarryKit.HasDefineIC(HAS_NULL_CLEAN_IC)   ||
                FLCarryKit.HasDefineIC(HAS_CLEAN_IC)        ||                  //Jimmychiu 20221028 add HAS_CLEAN_IC
                FLCarryKit.HasDefineIC(CLEAN_FINISH_IC)))
            {
                Task=540;
            }
            else if(InArmSuck.HasIC()==false &&
                    (FLCarryKit.HasDefineIC(HAS_NULL_CLEAN_IC) ||
                     FLCarryKit.HasDefineIC(HAS_CLEAN_IC)      ||               //Jimmychiu 20221028 add HAS_CLEAN_IC
                     FLCarryKit.HasDefineIC(CLEAN_FINISH_IC)   ||
                     FTestSuck.HasDefineIC(HAS_CLEAN_IC)       ||
                     FTestSuck.HasDefineIC(CLEAN_FINISH_IC)))
            {

            }
            else                                                                //Steven 20220922 : Shuttle 1都沒料, 只處理Shuttle 2
            {
                Task=2540;                                                      //Steven 20220926 : 2515 --> 2540
            }
            break;
        case 2520:
            if(DoShakeShuttle(euShuttle2))                                      //Steven 20120801 : 修改抖抖功能
            {
                Task=2530;
            }
            break;
        case 2521:                                                              //Steven 20210616 : Auto Clean也要震動
            if(DoKnockShuttle(euShuttle2))                                      //Jou 2013-03-08 修改敲敲功能
            {
                Task=2530;
            }
            break;
        case 2522:                                                              //Steven 20210616 : Auto Clean也要震動
            if(DoVibrateShuttle(euShuttle2))                                    //JerryYang 20190123 shuttle震動馬達
            {
                Task=2530;
            }
            break;
        case 2530:
            if(InSHT2InLF())
            {
                MOT[MInShuttle2].fCanMoveL=false;
                bShuttleShake=false;
                IndexZCanMove[0]=true;
                IndexZCanMove[1]=true;
                InitialSet();                                                   //kevin 20150720
                Task=2510;
            }
            break;
        case 2540:
            if(MoveInArmXYToShuttle_9045(euShuttle2, 0, ZAxisNotDown, false))
            {
                Task=2600;
            }
            break;
        case 2600:
            if(BLCarryKit.UseSiteNoIC() &&
               InSHT2InLF()              &&
               BTestSuck.UseSiteNoIC())
            {
                Task=3200;
            }
            else
            {                                                                   //shuttle 2上有Clean Pad & shuttle 2移至左邊時
                if(InSHT2InLF() &&                                              //Steven 20190212 : 修正Hang up
                   bInedxCleanFinish[1])                                        //Steven 20200730 : 修正Hang up
                    iShuttleRowKit=GetShuttleState(euShuttle2, bAutoPick);
                else
                    iShuttleRowKit=0;

                if(iShuttleRowKit!=0 && InSHT2InLF() &&
                   (BLCarryKit.HasType(CLEAN_FINISH_IC) ||                      //JerryYang 20160123 修正當index arm掉料後,shuttle只剩下HAS_NULL_IC會hang up //JerryYang 20151124 Fix hang up,Shuttle上有 CLEAN_FINISH_IC才能吸取CleanPad
                   (bInedxCleanFinish[1] && BLCarryKit.HasType(HAS_NULL_CLEAN_IC))))//kevin 20170518 (wei) 全部IC掉落Hang up
                {
                    MOT[MInShuttle2].fCanMoveL=false;
                    InitPickFromShuttleTask();
                    if(CUSTOMER_CODE==CC_KYEC_LEE || CosFunction.bHiSiliconFunction)
                    {
                        bShakeFlag=false;
                        if(IniConfig.bF01ShakeShuttleWhenJam==false && IniConfig.bF23ShuttleVibration==false)
                            bShakeFlag=true;
                        Task=2610;
                    }
                    else
                    {
                        Task=2800;
                    }
                }
                else if(iShuttleRowKit!=0 && bInedxCleanFinish[1] &&
                        BLCarryKit.HasType(HAS_NULL_CLEAN_IC))                  //Frank 20171213 (Steven) modify
                {
                    Task=3200;
                }
            }
            break;
        case 2610:
            if(CosFunction.bAutoCleanShuttleDisable==true)                      //jou 2013-02-27 Auto Clean disable shuttle sensor detect
            {
                if(IniConfig.bAutoCleanShuttleDisable)                          //jou 2013-02-27 Auto Clean disable shuttle sensor detect
                {
                    Task=2800;
                    break;
                }
            }
            else
            {
                if(iAutoCleanShuttle==0)                                        //JerryYang 20170327 (Jou) 修正Auto Clean中shuttle 2 Shuttle偵測不能disable的問題
                {
                    Task=2800;
                    break;
                }
            }

            ret=CheckShuttleSensor_Clean(euShuttle2, bShakeFlag);
            if(ret==1)
            {
                bShakeFlag=false;
                if(IniConfig.bF01ShakeShuttleWhenJam==false && IniConfig.bF23ShuttleVibration==false)
                    bShakeFlag=true;
            }
            else if(ret==3)
            {
                break;
            }
            else if(ret==2)
            {
                Task=2620;
                bShakeFlag=true;
                DoShakeShuttle(euShuttle2, true);                               //Steven 20120801 : 修改抖抖功能 (true為初始化)
                break;
            }
            else
            {
                Task=2800;
            }
            break;
        case 2620:
            if(DoShakeShuttle(euShuttle2))                                      //Steven 20120801 : 修改抖抖功能
            {
                Task=2630;
            }
            break;
        case 2630:
            if(InSHT2InLF())
            {
                MOT[MInShuttle2].fCanMoveL=false;
                bShuttleShake=false;
                IndexZCanMove[0]=true;
                IndexZCanMove[1]=true;
                InitialSet();
                Task=2610;
            }
            break;
        case 2800:
            bPickFromShuttleByAutoClean=true;                                   //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
            if(DoPickFromShuttle(euShuttle2, iShuttleRowKit))
            {
                if(bUse8Picker)
                {
                    if(iCloseSiteModeFor2x8==e2x8_STMMode ||                    //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
                       iCloseSiteModeFor2x8==e2x8_TW153Mode)
                    {
                        if(BLCarryKit.LeftSideNoIC(4)==true && BLCarryKit.RightSideNoIC(4)==false)
                        {
                            iShuttleRowKit=GetShuttleState(euShuttle2, bAutoPick);
                            break;
                        }
                    }
                }

                bPickFromShuttleByAutoClean=false;                              //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                Task=3000;
            }
            break;
        case 3000:
            if(MoveInArmZToPlateSafe(Task))
            {
                Task=3050;
                if(CheckInArmSuckICFallDownToHasNullIC(false)==true)            //Steven 20110516 : 修改成整合式Alarm
                {
                    Task=3010;                                                  //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
                }
            }
            break;
        case 3010:
            CheckInArmSuckICFallDownToHasNullIC();                              //JerryYang 20200422 修正掉料可能檢查不出來的問題, 檢查到有掉料要重新再確認全部吸嘴
            Task=3000;
            break;
        case 3050:
            if(LastSet.iRealDummy==REALLY)
            {
                ptrInSHT=&BLCarryKit;

                if(iShuttleRowKit==1 || iShuttleRowKit==2)
                    ret=0;
                else
                    ret=4;

                for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                   //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Suck[i][j].Enable       &&
                           InArmSuck.Suck[i][j].SenUsing!="" &&
                           InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC)
                        {
                            if(InArmSuck.Suck[i][j].GetStatus()==false)
                            {
                                bIsSuckICFallDown[i][j]=true;
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                                bHasErr=true;
                            }
                        }
                    }
                }
            }

            if(bHasErr)
            {
                ret=ShowErrorMessage("JAM0128", K_SKIP, MInArmX, false, ErrPart); //Steven 20091123 : Device Drop Error  //JerryYang 20160511 JAM0126->JAM0128,將IC及Clean pad相關的alarm code分開

                for(int i=iSuckRow; i<InArmSuck.iMaxRow; i++)                   //ChungHung 20141121 add for Use HotPlate AutoClean 1--->i
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Suck[i][j].Enable       &&
                           InArmSuck.Suck[i][j].SenUsing!="" &&
                           InArmSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC     &&
                           InArmSuck.Suck[i][j].GetStatus()==false)
                        {
                            InArmSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);
                            bIsSuckICFallDown[i][j]=true;                       //ChungHung 20141121 add for Use HotPlate AutoClean
                        }
                    }
                }

                if(SoftStop)
                    fMain->Pause("DoAutoCleanKit 3050");

                Task=3100;                                                      //JerryYang 20160125 Device drop error only skip
            }
            else
            {
                Task=3100;
            }
            break;
        case 3100:
            if(InArmSuck.HasIC()==false)
            {
                Task=2600;
                break;
            }
            MOT[MInShuttle2].fCanMoveL=true;                                    //ChungHung 20150410 add because case 2600 lock here need unlock
            DoAutoCleanPlaceToCleanKit(true);
            if(TestIF_File.iAutoClean_Tray==eCKPos_Fix3)                        //JerryYang 20161219 (Steven) 避免Auto clean hang up
            {
                if(iCatchTrayControlManual>=2)                                  // catch tray busy
                    break;
            }
            Task=3200;
            break;
        case 3200:
            if(DoAutoCleanPlaceToCleanKit(false))
            {
                if(InArmSuck.HasIC()==false)
                {
                    if(BLCarryKit.UseSiteHasIC())                               //JerryYang 20160123 修正當index arm掉料後,shuttle只剩下HAS_NULL_IC會hang up
                    {
                        Task=2600;
                    }
                    else
                    {
                        Task=2000;
                    }
                }
                else
                {
                    Task=3000;
                }
            }
            break;
    }

    #ifdef DEBUG_AUTO_CLEAN
    static int oldTask=-1;
    if(oldTask!=Task)
    {
        Message.sprintf("DoAutoCleanKit, %d, Go Task, %d", oldTask, Task);
        fMain->AddAutoCleanMessage(Message);
        oldT2ask=Task;
    }
    #endif
}
//------------------------------------------------------------------------------
// INARM 馬達因 PICK UP　ALARM 馬達釋放可以移動　重新start馬達還會回到原位　20120307
//------------------------------------------------------------------------------
TQPF_Timer tCleanInArmServoOnDelay;
bool DoInArmPineRelease()
{
    static bool bHaveServoOn=false;
    static bool bXMoveFlag=false;
    static bool bYMoveFlag=false;
    static bool bNeedDelay=false;

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20110802
    {
        if(fNote->bMyServoOffInArm)
        {
            if(bHaveServoOn==false)
            {
                MOT[MInArmX].ServoOnOff(true);
                MOT[MInArmY].ServoOnOff(true);
                tCleanInArmServoOnDelay.SetMSAndOn(500);
                bHaveServoOn=true;
                bXMoveFlag=false;
                bYMoveFlag=false;
            }

            if(InArmZSafe(DETECT_ALL_FLAG)==-1)                                 //如果Z軸在上才可以動
            {
                if(tCleanInArmServoOnDelay.Off())
                {
                    if(bXMoveFlag==false)
                    {
                        bXMoveFlag=MOT[MInArmX].MotorMove(fNote->iMyServoOffInArmPosX);
                    }

                    if(bYMoveFlag==false)
                    {
                        bYMoveFlag=MOT[MInArmY].MotorMove(fNote->iMyServoOffInArmPosY);
                    }

                    if(bXMoveFlag==true && bYMoveFlag==true)
                    {
                        tCleanInArmServoOnDelay.SetMSAndOn(500);
                        fNote->bMyServoOffInArm=false;
                        bHaveServoOn=false;
                        bNeedDelay=true;
                        return true;
                    }
                }
            }
            else
            {
                fNote->bMyServoOffInArm=false;
                fAllMotorHome=false;

                if(IniConfig.bSPILFunction==true ||                             //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
                   CUSTOMER_CODE==CC_SIGURD_PeiXing)                            //JerryYang 20160328 for 矽格北興,觸發回home的地方補上log
                    ShowMyMessage("In Arm PICK UP ALARM,Need home");            //Ifor 20151208 :矽品新增馬達異常 Show Alarm Message
                StopAllMotor();
                bNeedDelay=false;
            }
        }
        else
        {
            bHaveServoOn=false;
            bNeedDelay=false;
            return true;
        }
    }
    else
    {
        return true;
    }

    if(bNeedDelay==true &&                                                      //Steven 20110804 End: Servo Off後要回到原位
       tCleanInArmServoOnDelay.Off()==false)                                    //Steven 20110809 : ServoOn後要Delay一下
    {
        bNeedDelay=false;
    }
    return false;
}
//------------------------------------------------------------------------------
void ResetAutoClean()
{
    bool bKitNeedClear=false;

    if(InArmSuck.HasDefineIC(HAS_NULL_CLEAN_IC) ||                              //JerryYang 20160824 回Home時in arm上有HAS_NULL_CLEAN_IC也要清除
       InArmSuck.HasDefineIC(HAS_CLEAN_IC) ||
       InArmSuck.HasDefineIC(CLEAN_FINISH_IC))
    {
        bKitNeedClear=true;
    }

    if(FLCarryKit.HasDefineIC(HAS_NULL_CLEAN_IC) ||                             //2014-04-01 Dell   只有在Autoclean 回home 都要清除clean pad
       FLCarryKit.HasDefineIC(HAS_CLEAN_IC) ||
       FLCarryKit.HasDefineIC(CLEAN_FINISH_IC))
    {
        bKitNeedClear=true;
    }

    if(BLCarryKit.HasDefineIC(HAS_NULL_CLEAN_IC) ||                             //2014-04-01 Dell   只有在Autoclean 回home 都要清除clean pad
       BLCarryKit.HasDefineIC(HAS_CLEAN_IC) ||
       BLCarryKit.HasDefineIC(CLEAN_FINISH_IC))
    {
        bKitNeedClear=true;
    }

    if(FTestSuck.HasDefineIC(HAS_NULL_CLEAN_IC) ||                              //JerryYang 20160825 回Home時Index arm上有Clean pad要清除
       FTestSuck.HasDefineIC(HAS_CLEAN_IC) ||
       FTestSuck.HasDefineIC(CLEAN_FINISH_IC))
    {
        bKitNeedClear=true;
    }
    if(BTestSuck.HasDefineIC(HAS_NULL_CLEAN_IC) ||                              //JerryYang 20160825 回Home時Index arm上有Clean pad要清除
       BTestSuck.HasDefineIC(HAS_CLEAN_IC) ||
       BTestSuck.HasDefineIC(CLEAN_FINISH_IC))
    {
        bKitNeedClear=true;
    }

    if(bKitNeedClear)
    {
        ShowMyMessage("Please remove all the clean pad on Shuttle, In Arm and Index arm!", "And put it back to the Clean Kit");

        RestoreCleanKitData();                                                  //ChungHung 20130628 add 回復已被InArm吸走的CleanKit上的IC計數  請勿亂用
        InArmSuck.ClearAll();
        FLCarryKit.ClearAll();
        BLCarryKit.ClearAll();
        FTestSuck.ClearAll();                                                   //JerryYang 20160825 回Home時Index arm上有Clean pad要清除
        BTestSuck.ClearAll();                                                   //JerryYang 20160825 回Home時Index arm上有Clean pad要清除
        fAllMotorHome=false;                                                    //JerryYang 20160825 修正auto clean中按home沒有把回home流程做完
    }

    bRunAutoClean=false;                                                        //Steven 20221219 : 歸零後要把Auto Clean動作重置
}
//------------------------------------------------------------------------------
void DoShuttle1AutoClean_Arm1PickArm2Test()                                     //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
{
    int &Task=iDoShuttle1AutoCleanTask;
    int iPos=0, iIndex1ZPos=0;
    bool bShtHasCleanIC=false, bIndexHasCleanIC=false;
    bool bShtFullIC=false;
    AnsiString Message="";
    bool bAllICDrop=false;                                                      //kevin 20180308 add 所有ic 掉落

    switch(Task)
    {
        case 1:                                                                 //Shuttle 1是否可移動
            if(MOT[MInShuttle1].IsCanMove())
            {
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                //Clean Kit
                {
                    Task=200;
                }
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
                            bAutoCleanShuttle1MoveToLeft==true &&
                            bAutoCleanShuttle1HasPickErr==true)
                {
                    SetMotorScaleSpeed(MInShuttle1, 10);
                    Task=3000;
                }
            }
            break;
        case 200:
            bIndexHasCleanIC=(FTestSuck.HasDefineIC(HAS_CLEAN_IC) ||
                              FTestSuck.HasDefineIC(CLEAN_FINISH_IC));
            if(bIndexHasCleanIC)
            {
                Task=2000;                                                      //SHT_RIGHT
            }
            else if(FLCarryKit.UseSiteNoIC())
            {
                Task=1000;                                                      //SHT_LEFT
            }
            else
            {
                bShtFullIC=FLCarryKit.UseSiteFullIC();                          //Ifor 20161215 add 帶入使用site數量
                if(bShtFullIC==false)                                           //Steven 20151015 : When Index drop and only have HAS_NULL_IC, will hang up
                {
                    bShtHasCleanIC=FLCarryKit.HasType(HAS_CLEAN_IC);
                    bAllICDrop=FLCarryKit.ShtAll_HasICType(HAS_NULL_IC, HAS_NULL_CLEAN_IC);        //kevin 20180308 所有ic 掉落

                    if((bShtHasCleanIC || bAllICDrop) &&
                       bInedxCleanFinish[0]==false)                             //kevin 20180430  Autoclean index Arm 2做完 no claen pad
                        Task=2000;                                              //SHT_RIGHT
                    else
                        Task=1000;                                              //SHT_LEFT
                }
                else
                {
                    bShtHasCleanIC=FLCarryKit.ShtAll_HasICType(HAS_CLEAN_IC, HAS_NULL_CLEAN_IC);   //Steven 20130620 : 改成Function
                    if(bShtHasCleanIC==false)
                        Task=1000;                                              //SHT_LEFT
                    else
                        Task=2000;                                              //SHT_RIGHT
                }
            }
            break;
        case 1000:
            iIndex1ZPos=MOT[MTestZ1].Gali_ReadPos();
            if(iIndex1ZPos>=Prod.TestZ1_Safe)
            {
                Task=1100;
            }
            break;
        case 1100:
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
                iPos=Prod.InSHT[0].iLeft;                                       //CLEANPAD HOTPLATE 在左邊等放IC
            if(MOT[MInShuttle1].MotorMove(iPos))
            {
                Task=1;
                b1ShuttleMoveToLeft=true;                                       //確實移動到左邊
            }
            else
            {
                b1ShuttleMoveToLeft=false;
            }
            break;
        case 2000:                                                              //進去INDEX
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
                iPos=Prod.InSHT[0].iRight;
            if(MOT[MInShuttle1].MotorMove(iPos))
            {
                Task=1;
                b1ShuttleMoveToRight=true;                                      //確實移動到右邊
            }
            else
            {
                b1ShuttleMoveToRight=false;
            }
            break;
        case 3000:                                                              //Richard 20230418 : pickupError shutter move to left position
            if(bAutoCleanShuttle1MoveToLeft==false)
            {
                Task=1;
                break;
            }
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
            {
                MOT[MInShuttle1].fCanMoveM=false;
                SetMotorScaleSpeed(MInShuttle1, SHSpeed.iSH1Sp);
            }
            break;
    }
}
//------------------------------------------------------------------------------
void DoShuttle1AutoClean()
{
    if(bShuttleShake)
    {
        return ;
    }

    if(IniConfig.bF16CheckShuttleSensorBroken && bDoingF16)                     //Steven 20221213 : 確認shuttle 有沒有斷線
        return;

    if(RunAutoCleanByArmPickArm2Test())                                         //獨立流程  //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
    {
        DoShuttle1AutoClean_Arm1PickArm2Test();
        return;
    }

    int &Task=iDoShuttle1AutoCleanTask;
    int iPos, iIndex1ZPos;
    bool bShtHasCleanIC, bIndexHasCleanIC;
    bool bShtFullIC;
    AnsiString Message;
    bool bAllICDrop=false;                                                      //kevin 20180308 add 所有ic 掉落

    #ifdef DEBUG_AUTO_CLEAN
    static int iOldTask=0;
    if(iOldTask!=Task)
    {
        Message.sprintf("DoShuttle1AutoClean %d, %d, Go to Task, %d", iSht, iOldTask, Task);
        fMain->AddAutoCleanMessage(Message);
        iOldTask=Task;
    }
    #endif

    switch(Task)
    {
        case 1:                                                                 //Shuttle 1是否可移動
            if(MOT[MInShuttle1].IsCanMove())
            {
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                //Clean Kit
                {
                    Task=200;
                }
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
                            bAutoCleanShuttle1MoveToLeft==true &&
                            bAutoCleanShuttle1HasPickErr==true)
                {
                    SetMotorScaleSpeed(MInShuttle1, 10);
                    Task=3000;
                }
            }
            break;
        case 200:
            if(iDoIndexAutoCleanTask>=400 && iDoIndexAutoCleanTask<1900)        //kevin 20120217 AUTOCLEAN還在INDEX
            {
                Task=2000;                                                      //SHT_Right
                break;
            }

            bIndexHasCleanIC=(FTestSuck.HasDefineIC(HAS_CLEAN_IC) ||
                              FTestSuck.HasDefineIC(CLEAN_FINISH_IC));
            if(bIndexHasCleanIC)
            {
                Task=2000;                                                      //SHT_RIGHT
            }
            else if(FLCarryKit.UseSiteNoIC())
            {
                Task=1000;                                                      //SHT_LEFT
            }
            else
            {
                bShtFullIC=FLCarryKit.UseSiteFullIC();                          //Ifor 20161215 add 帶入使用site數量
                if(bShtFullIC==false)                                           //Steven 20151015 : When Index drop and only have HAS_NULL_IC, will hang up
                {
                    bShtHasCleanIC=FLCarryKit.HasType(HAS_CLEAN_IC);
                    bAllICDrop=FLCarryKit.ShtAll_HasICType(HAS_NULL_IC, HAS_NULL_CLEAN_IC);     //kevin 20180308 所有ic 掉落

                    if((bShtHasCleanIC || bAllICDrop) &&
                       bInedxCleanFinish[0]==false)                             //kevin 20180430  Autoclean index Arm 2做完 no claen pad
                        Task=2000;                                              //SHT_RIGHT
                    else
                        Task=1000;                                              //SHT_LEFT
                }
                else
                {
                    bShtHasCleanIC=FLCarryKit.ShtAll_HasICType(HAS_CLEAN_IC, HAS_NULL_CLEAN_IC);   //Steven 20130620 : 改成Function

                    bAllICDrop=FLCarryKit.ShtAll_HasICType(HAS_NULL_IC, HAS_NULL_CLEAN_IC);        //JerryYang 20250505 : fix all site drop hang up
                    if(bShtHasCleanIC==false || bAllICDrop==true)
                        Task=1000;                                              //SHT_LEFT
                    else
                        Task=2000;                                              //SHT_RIGHT
                }
            }
            break;
        case 1000:
            iIndex1ZPos=MOT[MTestZ1].Gali_ReadPos();
            if(iIndex1ZPos>=Prod.TestZ1_Safe)
            {
                Task=1100;
            }
            break;
        case 1100:
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
                iPos=Prod.InSHT[0].iLeft;                                       //CLEANPAD HOTPLATE 在左邊等放IC

            if(MOT[MInShuttle1].MotorMove(iPos))
            {
                Task=1;
                if(iDoIndexAutoCleanTask>=400 && iDoIndexAutoCleanTask<1900)    //kevin 20120217 AUTOCLEAN還在INDEX
                    MOT[MInShuttle1].fCanMoveM=false;
                b1ShuttleMoveToLeft=true;                                       //確實移動到左邊
            }
            else
            {
                b1ShuttleMoveToLeft=false;
            }
            break;
        case 2000:                                                              //進去INDEX
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
                iPos=Prod.InSHT[0].iRight;

            if(MOT[MInShuttle1].MotorMove(iPos))
            {
                Task=1;
                if(iDoIndexAutoCleanTask>=400 && iDoIndexAutoCleanTask<1900)    //kevin 20120217 AUTOCLEAN還在INDEX
                    MOT[MInShuttle1].fCanMoveM=false;
                b1ShuttleMoveToRight=true;                                      //確實移動到右邊
            }
            else
            {
                b1ShuttleMoveToRight=false;
            }
            break;
        case 3000:                                                              //Richard 20230418 : pickupError shutter move to left position
            if(bAutoCleanShuttle1MoveToLeft==false)
            {
                Task=1;
                break;
            }
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
            {
                MOT[MInShuttle1].fCanMoveM=false;
                SetMotorScaleSpeed(MInShuttle1, SHSpeed.iSH1Sp);
            }
            break;
    }
}
//------------------------------------------------------------------------------
void DoShuttle2AutoClean()
{
    if(CosFunction.bAutoCleanAutoSelIndexArm==false)
    {
        if(TestIF.iAutoClean_SelectArm==0)
        {
            return;
        }
    }
    else
    {
        if(TestIF.bCleanIndexOtherArm==false && TestIF_File.iShuttleMode==1 &&
           TestIF_File.iShuttle_Sel==0)
        {
            return;
        }

        if(TestIF.bCleanIndexOtherArm==false &&
           TestIF_File.iShuttleMode==0)                                         //0:Arm1 1:Arm2 2:Arm1 & Arm2
        {
            return;
        }
    }

    if(bShuttleShake)
    {
        return ;
    }

    if(IniConfig.bF16CheckShuttleSensorBroken && bDoingF16)                     //Steven 20221213 : 確認shuttle 有沒有斷線
        return;

    if(RunAutoCleanByArmPickArm2Test())                                         //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
    {
        return;
    }

    int &Task=iDoShuttle2AutoCleanTask;
    int iPos,iIndex2ZPos;
    bool bShtHasCleanIC, bIndexHasCleanIC;
    bool bShtFullIC=false;
    bool bAllICDrop=false;                                                      //kevin 20170215 (wei) 所有ic 掉落

    #ifdef DEBUG_AUTO_CLEAN
    static int iOldTask=0;
    if(iOldTask!=Task)
    {
        Message.sprintf("DoShuttle2AutoClean %d, %d, Go to Task, %d", iSht, iOldTask, Task);
        fMain->AddAutoCleanMessage(Message);
        iOldTask=Task;
    }
    #endif

    switch(Task)
    {
        case 1:                                                                 //Shuttle 2是否可移動
            if(MOT[MInShuttle2].IsCanMove())
            {
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                //Clean Kit
                {
                    Task=200;
                }
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
                            bAutoCleanShuttle2MoveToLeft==true &&
                            bAutoCleanShuttle2HasPickErr==true)
                {
                    SetMotorScaleSpeed(MInShuttle2, 10);
                    Task=3000;
                }
            }
            break;
        case 200:
            if(iDoIndexAutoCleanTask>=2400 && iDoIndexAutoCleanTask<3900)       //kevin 20120217 AUTOCLEAN還在INDEX
            {
                Task=2000;                                                      //SHT_Right
                break;
            }

            bIndexHasCleanIC=(BTestSuck.HasDefineIC(HAS_CLEAN_IC) ||
                              BTestSuck.HasDefineIC(CLEAN_FINISH_IC));

            if(bIndexHasCleanIC)
            {
                Task=2000;                                                      //SHT_RIGHT
            }
            else if(BLCarryKit.UseSiteNoIC())
            {
                Task=1000;                                                      //SHT_LEFT
            }
            else
            {
                bShtFullIC=BLCarryKit.UseSiteFullIC();                          //Ifor 20161215 add 帶入使用site數量
                if(bShtFullIC==false)                                           //Steven 20151015 : When Index drop and only have HAS_NULL_IC, will hang up
                {
                    bShtHasCleanIC=BLCarryKit.HasType(HAS_CLEAN_IC);
                    bAllICDrop=BLCarryKit.ShtAll_HasICType(HAS_NULL_IC, HAS_NULL_CLEAN_IC);    //kevin 20170215 (wei) 所有ic 掉落

                    if((bShtHasCleanIC || bAllICDrop) &&
                       bInedxCleanFinish[1]==false)                             //kevin 20180525 20180430  Autoclean index Arm 2做完 no claen pad
                        Task=2000;                                              //SHT_RIGHT
                    else
                        Task=1000;                                              //SHT_LEFT
                }
                else
                {
                    bShtHasCleanIC=BLCarryKit.ShtAll_HasICType(HAS_CLEAN_IC, HAS_NULL_CLEAN_IC);   //Steven 20130620 : 改成Function
                    bAllICDrop=BLCarryKit.ShtAll_HasICType(HAS_NULL_CLEAN_IC, HAS_NULL_CLEAN_IC);  //kevin 20170215 (wei) 所有ic 掉落
                    if(bShtHasCleanIC==false || bAllICDrop || iDoIndexAutoCleanTask==3900)      //Steven 20151015 : When Index drop and only have HAS_NULL_IC, will hang up
                        Task=1000;                                              //SHT_LEFT
                    else
                        Task=2000;                                              //SHT_RIGHT
                }
            }
            break;
        case 1000:
            iIndex2ZPos=MOT[MTestZ2].Gali_ReadPos();
            if(iIndex2ZPos>=Prod.TestZ2_Safe)
                Task=1100;
            break;
        case 1100:
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
                iPos=Prod.InSHT[1].iLeft;                                       //CLEANPAD HOTPLATE 在左邊等放IC

            if(IniConfig.bF21InOutArmZMotorPrivate)
            {
                if(DoInOutARM_SHT_MoveSafe(1))                                  //kevin 20161005 SHUTTLE 1 移動安全保護
                    return;
            }
            if(MOT[MInShuttle2].MotorMove(iPos))
            {
                Task=1;
                if(iDoIndexAutoCleanTask>=2400 && iDoIndexAutoCleanTask<3900)   //kevin 20120217 AUTOCLEAN還在INDEX
                    MOT[MInShuttle2].fCanMoveM=false;
                b2ShuttleMoveToLeft=true;                                       //確實移動到左邊
            }
            else
            {
                b2ShuttleMoveToLeft=false;                                      //確實移動到左邊
            }
            break;
        case 2000:                                                              //進去INDEX
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
                iPos=Prod.InSHT[1].iRight;

            if(IniConfig.bF21InOutArmZMotorPrivate)
            {
                if(DoInOutARM_SHT_MoveSafe(1))                                  //kevin 20161005 SHUTTLE 1 移動安全保護
                    return;
            }
            if(MOT[MInShuttle2].MotorMove(iPos))
            {
                Task=1;
                if(iDoIndexAutoCleanTask>=2400 && iDoIndexAutoCleanTask<3900)   //kevin 20120217 AUTOCLEAN還在INDEX
                    MOT[MInShuttle2].fCanMoveM=false;
                b2ShuttleMoveToRight=true;                                      //確實移動到右邊
            }
            else
            {
                b2ShuttleMoveToRight=false;                                     //確實移動到右邊
            }
            break;
        case 3000:                                                              //Richard 20230418 : pickupError shutter move to left position
            if(bAutoCleanShuttle2MoveToLeft==false)
            {
                Task=1;
                break;
            }
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                MOT[MInShuttle2].fCanMoveM=false;
                SetMotorScaleSpeed(MInShuttle2, SHSpeed.iSH2Sp);
            }
            break;
    }
}
//------------------------------------------------------------------------------
bool DoInArmMoveToWaitPosByAutoClean()                                          //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
{
    if(CosFunction.bIndexJamInArmMoveSafePostionByAutoClaen)
    {
        if(bPlaceToShuttleByAutoClean || bPickFromShuttleByAutoClean)
        {
            bLockPlaceToShuttleByAutoClean=false;
            bLockPickFromShuttleByAutoClean=false;
            return false;
        }
        else
        {
            bLockPlaceToShuttleByAutoClean=true;
            bLockPickFromShuttleByAutoClean=true;

            if(bPlaceToCleanKit)                                                //Steven 20171204 (Wei) : 避免放到一半要讓位不回來原本位置放
                return false;

            if(bPickFromKitByAutoClean==true)                                   //Steven 20210603 : 新增吸放Clean Kit的Flag
                return false;

            if(MoveInArm2XYToWait())                                            //如果要移開,而且還沒移到定位
            {
                bLockPlaceToShuttleByAutoClean=false;
                bLockPickFromShuttleByAutoClean=false;
                return true;                                                    //先離開等In Arm
            }
        }
    }
    else
    {
        bLockPlaceToShuttleByAutoClean=false;
        bLockPickFromShuttleByAutoClean=false;
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void RecDebug(TMyKitSuck &tray, int iTask)
{
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            MNetLog(AnsiString().sprintf("Task=%d Row=%d Col=%d Item=%d", iTask, i, j, tray.Item[i][j]));
        }
    }
}
//------------------------------------------------------------------------------
bool DoSocketSensorAlarm(AnsiString sFunc,int Task)
{
    if(DoInArmMoveToWaitPosByAutoClean()==true)                                 //JerryYang 20241118 : fix auto clean hang up   //Steven 20130613 : Index異常時, In Arm要先讓位功能
    {
        RecordProcess(AnsiString().sprintf("%s__%d: iShowSocketSensor=%d",sFunc,Task, iShowSocketSensor));

        if(TestIF_File.bEnSocketSensor && iShowSocketSensor>0)                  //kevin 20130504 socket sensor
        {
            if(iShowSocketSensor==2)                                            //JerryYang 20161024 Socket sensor改成偵測置偏
            {
                ShowErrorMessage("WAR0323", K_RETRY, MTestZ1, false, sSocketSensorErr);  //Socket detect device floting error
            }
            else if(iShowSocketSensor==1)
            {
                ShowErrorMessage("WAR0322", K_RETRY, MTestZ1, false, sSocketSensorErr);
            }
            iShowSocketSensor=0;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
AnsiString GetMotFunc(AnsiString asFunc,int iTask)
{
    return AnsiString().sprintf("%s %d", asFunc, iTask);
}
//------------------------------------------------------------------------------
extern void SetTechDataToProd_AutoClean();
//------------------------------------------------------------------------------
TQPF_Timer DoIndexAutoCleanDelay;
void DoIndexAutoClean_Arm1PickArm2Test(int iSpeed, int iSpeedSY, int iSpeedSZ, int &iContactCount,
                                       bool bDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       bool bSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       bool bIsSuckICFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       bool bTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL],
                                       AnsiString ErrPart)                      //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
{
    static int iCT=0;

    int ret=0, iPos;
    int &Task=iDoIndexAutoCleanTask;
    bool bFlagY1=false, bFlagS2=false, flag1=false;
    bool bHasErr=false, bHasDuplicateErr=false;                                 //kevin 20180716
    AnsiString Str;

    switch(Task)
    {
        case 1:
            fHome->InitDoTestZHome();
            bInedxCleanFinish[0]=false;                                         //kevin 20170520 (wei) Autoclean index Arm 1做完
            bInedxCleanFinish[1]=false;                                         //kevin 20170520 (wei) Autoclean index Arm 2做完
            Task=100;
            break;
        case 100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeed, GetMotFunc(__FUNC__,Task)))
            {
                if(TestSocket.HasDefineIC(HAS_CLEAN_IC))                        //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                {
                    Task=3090;
                }
                else if(TestSocket.HasDefineIC(CLEAN_FINISH_IC))                //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                {
                    Task=3150;
                }
                else if(FTestSuck.HasDefineIC(HAS_CLEAN_IC))                    //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                {
                    Task=200;
                }
                else if(FTestSuck.HasDefineIC(CLEAN_FINISH_IC))                 //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                {
                    Task=3750;
                }
                else
                {
                    Task=200;
                }
            }
            break;
        case 200:
            //重置index狀態
            fHome->InitDoTestZHome();
            iContactCount=0;
            Task=300;
            break;
        case 300:
            if(EP_Install)
            {
                ADAM_WriteVoltage(TestIF.fAutoClean_AireForce);
            }
            bFlagY1=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedSY, GetMotFunc(__FUNC__,Task));//kevin 20120517 iSpeed);    //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            bFlagS2=false;

            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
            {
                if(FLCarryKit.UseSiteHasIC() && InSHT1InRT())                   //Sam 20210923 : 修正 AutoClean Index Arm 偷跑壓空氣導致  Hang up    //Steven 20220211 : HasRealIC --> HasIC
                {
                    MOT[MInShuttle1].fCanMoveM=false;                           //kevin 20170119 index cleanpad尚未吸取 shuttle不能移動
                    bFlagS2=true;
                }
            }
            else if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
            {
                bFlagS2=true;
            }

            if(bFlagY1 && bFlagS2)
            {
                MOT[MInShuttle1].fCanMoveM=false;                               //index與shuttle都到等待位置上，shuttle 1有clean pad
                Task=400;
            }
            break;
        case 400:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Pick+TestIF_File.iAutoClean_IndexPickOffset, iSpeed, "DoIndexAutoClean_Arm1PickArm2Test_400"))        //wei 20150318 Auto clean Index Pick Offset
            {
                DoIndexAutoCleanDelay.SetMSAndOn(500);
                Task=500;
            }
            break;
        case 500:
            if(DoIndexAutoCleanDelay.Off())
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        bSuckFinish[i][j]=false;                                //Steven 20110301 : 初始化，都當作還沒做完
                        bTestSuckUse[i][j]=false;
                    }
                }
                FTestSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=600;
            }
            break;
        case 600:
            flag1=true;
            for(int i=0; i<MAX_Index_Row; i++)                                  //交換IC狀態
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FLCarryKit.Item[i][j])
                    {
                        if(FLCarryKit.Item[i][j]==HAS_NULL_CLEAN_IC ||
                           bSuckFinish[i][j]==true)                             //Steven 20110301
                        {
                            if(FTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                               FTestSuck.Item[i][j]==NULL_IC)                   //Steven 20111202 : Retry會掉料
                                FTestSuck.Suck[i][j].Normal();                  //Steven 20111201 : 預防負壓壓降
                            bSuckFinish[i][j]=true;                             //Steven 20110301
                        }
                        else
                        {
                            if(fMain->bAutoCleanTest==true && i==1 && j==2)
                            {
                                FTestSuck.Suck[i][j].Error=true;
                                fMain->bAutoCleanTest=false;
                                bSuckFinish[i][j]=true;
                                return;
                            }
                            #ifdef SOFT_SIMULTE
                            int x=atoi(fMain->edHPY->Text.c_str());
                            int y=atoi(fMain->edHPX->Text.c_str());
                            if(fMain->cbIndexDrop->Checked==true && i==x && j==y)
                            {
                                FTestSuck.Suck[i][j].Error=true;
                                fMain->bAutoCleanTest=false;
                                bSuckFinish[i][j]=true;
                                continue;
                            }
                            #endif
                            if(FTestSuck.Suck[i][j].Suck())
                            {
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);
                                bDuplicateErr[i][j]=false;                      //Steven 20100105
                                bSuckFinish[i][j]=true;                         //Steven 20110301 : 吸取完成的不做
                            }
                            else if(FTestSuck.Suck[i][j].Error)                 //Steven 20110301 : 有錯誤的不做
                            {
                                bSuckFinish[i][j]=true;
                            }
                            else
                            {
                                flag1=false;                                    //jou 2011-08-16 只要有未完成的就繼續等
                            }
                        }
                    }
                    else
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                           FTestSuck.Item[i][j]==NULL_IC)                       //Steven 20111202 : Retry會掉料
                            FTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                        bSuckFinish[i][j]=true;                                 //Steven 20110301 : 沒有東西的地方要跳過
                    }
                }
            }

            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bSuckFinish[i][j]==false)                                //只要有未完成的就繼續等
                        flag1=false;
                }
            }

            if(flag1==true)                                                     //Steven 20110301 : 所有吸嘴都做完
            {
                for(int i=0; i<MAX_Index_Row; i++)                              //若有吸取錯誤
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            Task=650;
                            return;
                        }
                    }
                }

                if(FLCarryKit.HasRealIC())
                    break;

                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FLCarryKit.Item[i][j])
                        {
                            if(FLCarryKit.Item[i][j]==HAS_NULL_CLEAN_IC)
                            {
                                FTestSuck.Suck[i][j].Normal();
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);
                            }
                        }
                        bDuplicateErr[i][j]=false;                              //Steven 20100105
                    }
                }

                if(FLCarryKit.UseSiteHasIC())
                    break;

                Task=700;
            }
            break;
        case 650:                                                               //移至安全位置
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean_Arm1PickArm2Test_650"))
            {
                Task=651;
            }
            break;
        case 651:                                                               //Richard 20230418 : pickupError shutter move to left position
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            iPos=IsNNMode();
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;
                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iPos][j];
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(IniConfig.bD43IndexDropErrorCanRetryandSkip==true && bHasErr==true)
            {
                MOT[MInShuttle1].fCanMoveM=true;
                bAutoCleanShuttle1MoveToLeft=true;
                bAutoCleanShuttle1HasPickErr=true;
                Task=652;
            }
            else
            {
                Task=660;
            }
            break;
        case 652:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
               bAutoCleanShuttle1MoveToLeft)                                    //ChungHung 20131015 fix hangup
            {
                MOT[MInShuttle1].fCanMoveM=true;
                if(InSHT1InLF()!=true)
                {
                    break;
                }
            }
            MOT[MInShuttle1].fCanMoveM=false;
            Task=660;
        case 660:
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            iPos=IsNNMode();
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iPos][j];
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(bHasErr)                                                         //Shuttle 上方 確認是否掉料??
            {
                bHasErr=false;                                                  //Ifor 20160322 移動至上方
                if(DoInArmMoveToWaitPosByAutoClean()==false)                    //JerryYang 20241118 : fix auto clean hang up    //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return ;
                }
                bIsTestSitICFallDown=true;
                if(IniConfig.bIndexPickErrOnlySKIP==true ||                     //kevin 201701103 (Steven) index pick up error only skip
                   IniConfig.bD64IndexPickErrOnlySKIP)                          //JerryYang 20160301 index pick-up error only skip
                    ret=ShowErrorMessage("JAM0312", K_SKIP, MTestZ1, bHasDuplicateErr, ErrPart);            //Device Pick-Up Error  //JerryYang 20160511 JAM0301->JAM0312,將IC及Clean pad相關的alarm code分開
                else
                    ret=ShowErrorMessage("JAM0312", K_SKIP|K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);    //Devicr Pick-Up Error  //JerryYang 20160511 JAM0301->JAM0312,將IC及Clean pad相關的alarm code分開

                if(ret==K_SKIP)
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.Suck[i][j].Error)
                            {
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);       //JerryYang 20160223 MARK
                                FTestSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC); //Steven 20260126 : 為了記得座標,但是只能填入 HAS_NULL_CLEAN_IC
                                FTestSuck.Suck[i][j].Normal();
                                bTestSuckUse[i][j]=true;
                            }
                            bDuplicateErr[i][j]=false;
                        }
                    }
                }
                else
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                            if(FTestSuck.Suck[i][j].Error)
                                bDuplicateErr[i][j]=true;
                }
                FTestSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                bAutoCleanShuttle1MoveToLeft=false;
                MOT[MInShuttle1].fCanMoveM=true;
            }
            bHasErr=false;
            bAutoCleanShuttle1HasPickErr=false;
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)
                {
                    Task=670;
                }
                else
                {
                    if(FLCarryKit.HasRealIC())
                        Task=400;
                    else
                        Task=700;
                }
            }
            break;
        case 670:
            if(InSHT1InRT())
            {
                MOT[MInShuttle1].fCanMoveM=false;
                if(FLCarryKit.HasRealIC())                                      //有IC->Retry
                    Task=400;
                else
                    Task=700;                                                   //無IC->Skip
            }
            break;
        case 700:                                                               //Steven 20160323 : 吸好之後要停一下
            DoIndexAutoCleanDelay.SetMSAndOn(500);
            Task=710;
        case 710:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=720;
            }
            break;
        case 720:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean_Arm1PickArm2Test_720"))
            {
                Task=800;
            }
            break;
        case 800:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FTestSuck.Suck[i][j].GetStatus()==false)
                        FTestSuck.Suck[i][j].Normal();
                }
            }
            Task=900;
            break;
        case 900:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedSY, GetMotFunc(__FUNC__,Task)))  //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            {
                Task=1000;
            }
            break;
        case 1000:
            fHome->InitDoTestZHome();
            Task=1150;
            break;
        case 1150:
            if(MOT[MTestZ1].Gali_MotMove(Prod.iAutoCleanZ_Contact[0], iSpeedSZ, "DoIndexAutoClean_Arm1PickArm2Test_1150"))//kevin 20181016 add drop 過壓
            {
                flag1=true;
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Item[i][j]>0)
                        {
                            if(FTestSuck.Item[i][j]!=HAS_CLEAN_IC ||
                               FTestSuck.Suck[i][j].Destroy())
                            {
                                TestSocket.MoveSuckData(FTestSuck, i, j);
                            }
                            else if(FTestSuck.Suck[i][j].Error==false)
                            {
                                flag1=false;
                            }
                        }
                    }
                }

                if(flag1==true)
                {
                    bIndexCheckNoStopVaccum=false;
                    bHasICinSocket=true;

                    DoIndexAutoCleanDelay.SetSecAndOn(1.5);
                    Task=1160;
                }
                break;
                }
                break;
        case 1160:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=1400;
            }
            break;
        case 1400:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeed, GetMotFunc(__FUNC__,Task)))
            {
                if(CheckSocketSensor(0, "DoIndexAutoClean_Arm1PickArm2Test_1400", true, false))  //Jimmychiu 20230821 : Arm1PickArm2Test add Socket Sensor Function
                {
                    Task=1450;
                }
                else
                {
                    bHasICinSocket=false;
                    Task=1500;
                }
            }
            break;
        case 1450:
            if(DoSocketSensorAlarm(__FUNC__, Task)==true)
            {
                Task=1400;                                                      //Steven 20201014 : 修正掉料異常時, index arm要分開
            }
            break;
        case 1500:
            if(MOT[MTestY1].Gali_MotMove(Prod.TestY1_Front, iSpeedSY, "DoIndexAutoClean_Arm1PickArm2Test_1500"))          //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            {
                Task=2100;
            }
            break;
        case 2100:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeed, GetMotFunc(__FUNC__,Task)))
            {
                Task=2200;
            }
            break;
        case 2200:
            fHome->InitDoTestZHome();
            iContactCount=0;
            Task=2300;
            break;
        case 2300:
            if(EP_Install)
            {
                ADAM_WriteVoltage(TestIF.fAutoClean_AireForce);
            }
            bFlagY1=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedSY, GetMotFunc(__FUNC__, Task));//kevin 20120517 iSpeed);   //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            if(bFlagY1)
            {
                DoIndexAutoCleanDelay.SetSecAndOn(1.5);
                MOT[MInShuttle2].fCanMoveM=false;
                Task=2310;
            }
            break;
        case 2310:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=2900;
            }
            break;
        case 2900:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedSY, GetMotFunc(__FUNC__, Task)))//kevin 20120517 iSpeed))  //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            {
                Task=3000;
            }
            break;
        case 3000:
            fHome->InitDoTestZHome();
            if(Prod.iAutoCleanZ_Contact[1]>-1000)                               //Steven 20260130 : Add protection for Auto clean
            {
                Str.sprintf("iAutoCleanZ_Contact[0, 1]=[%d, %d] !!!", Prod.iAutoCleanZ_Contact[0], Prod.iAutoCleanZ_Contact[1]);
                RecordProcess(Str);
                SetTechDataToProd_AutoClean();
            }
            Task=3090;
            break;
        case 3090:
            if(MOT[MTestZ2].Gali_MotMove(Prod.iAutoCleanZ_Contact[1], iSpeedSZ, "DoIndexAutoClean_Arm1PickArm2Test_3090"))
            {
                iContactCount++;
                DoIndexAutoCleanDelay.Set0_1SecAndOn(Prod.iAutoClean_ContactTime);  //Sam 20230111 : Smart Auto Clean
                Task=3100;
            }
            break;
        case 3100:
            if(DoIndexAutoCleanDelay.Off())
            {
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
                    SW[SwSocketClean].Off();
                if(iContactCount<Prod.iAutoClean_ContactCount)                  //Sam 20230111 : Smart Auto Clean
                {
                    Task=3130;
                }
                else
                {
                    if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    {
                        Task=3150;
                    }
                    else
                    {
                        Task=3200;
                    }
                }
            }
            break;
        case 3130:
            if(MOT[MTestZ2].Gali_MotMove(Prod.iAutoCleanZ_Shift[1], iSpeedSZ, "DoIndexAutoClean_Arm1PickArm2Test_3130"))
            {
                DoIndexAutoCleanDelay.SetSecAndOn(1.5);
                iCT=0;
                Task=3135;
            }
            break;
        case 3135:
            if(DoIndexAutoCleanDelay.Off())
            {
                bHasICinSocket=true;
                if(CheckSocketSensor(1, "DoIndexAutoClean_Arm1PickArm2Test_3135", true, true))  //Jimmychiu 20230821 : Arm1PickArm2Test add Socket Sensor Function
                {
                    iCT++;
                    if(iCT>=3)
                    {
                        iCT=0;
                        Task=3140;
                    }
                    break;
                }

                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
                    SW[SwSocketClean].On();
                bHasICinSocket=false;
                Task=3090;
            }
            break;
        case 3140:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeed, GetMotFunc(__FUNC__,Task)))
            {
                Task=3142;
            }
            break;
        case 3142:
            if(DoSocketSensorAlarm(__FUNC__, Task)==true)
            {
                Task=3130;
            }
            break;
        case 3150:
            if(MOT[MTestZ2].Gali_MotMove(Prod.iAutoCleanZ_Shift[1], iSpeedSZ, "DoIndexAutoClean_Arm1PickArm2Test_3150"))
            {
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
                    SW[SwSocketClean].On();
                DoIndexAutoCleanDelay.SetSecAndOn(1.5);
                Task=3160;
            }
            break;
        case 3160:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=3400;
            }
            break;
        case 3400:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeed, GetMotFunc(__FUNC__, Task)))
            {
                Task=3600;
            }
            break;
        case 3600:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedSY, GetMotFunc(__FUNC__, Task)))//kevin 20120517 iSpeed))    //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            {
                bHasICinSocket=true;
                if(CheckSocketSensor(0, "DoIndexAutoClean_Arm1PickArm2Test_3600", true, false))  //Jimmychiu 20230821 : Arm1PickArm2Test add Socket Sensor Function
                {
                    Task=3610;
                }
                else
                {
                    bHasICinSocket=false;
                    Task=3650;
                }
            }
            break;
        case 3610:
            if(DoSocketSensorAlarm(__FUNC__, Task)==true)
            {
                Task=3600;
            }
            break;
        case 3650:
            if(MOT[MTestZ1].Gali_MotMove(Prod.iAutoCleanZ_Contact[0], iSpeedSZ))//kevin 20180717 drop high
            {
                Task=3700;
            }
            break;
        case 3700:
//            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
//                return;                                                       //Steven 20250422 : Mark for [D58]

//            if(CosFunction.bJAM0303NeedOpenChamberDoor)                         //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
//                bIsTestSitICFallDown=true;

/*            if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX || CUSTOMER_CODE==CC_KYEC_CHEN)
                iRet=ShowErrorMessage("JAM0314", K_RETRY, MTestZ1, false, ErrPart); //Steven 20100129 : Device Drop Error   //JerryYang 20160511 JAM0303->JAM0314,將IC及Clean pad相關的alarm code分開
            else
                iRet=ShowErrorMessage("JAM0314", K_SKIP, MTestZ1, false, ErrPart);  //Steven 20100129 : Device Drop Error    //JerryYang 20160511 JAM0303->JAM0314,將IC及Clean pad相關的alarm code分開

            if(iRet==K_SKIP)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])                             //kevin 20150624
                            FTestSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);     // 掉料忽視
                    }
                }
            }  */
//            Task=1500;
            flag1=true;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(TestSocket.Item[i][j]>0)
                    {
                        if(TestSocket.Item[i][j]==HAS_NULL_CLEAN_IC)            //Steven 20260126 : fixed for [D58] auto clean
                        {
                            FTestSuck.MoveSuckData(TestSocket, i, j);
                        }
                        else if(FTestSuck.Suck[i][j].Suck())
                        {
                            FTestSuck.MoveSuckData(TestSocket, i, j);
                            FTestSuck.SetItemData(i, j, CLEAN_FINISH_IC);
                        }
                        else
                        {
                            flag1=false;                                        //jou 2011-08-16 只要有未完成的就繼續等
                        }
                    }
                }
            }

            if(flag1==true)
            {
                RecDebug(FTestSuck, Task);
                Task=3750;
            }
            break;
        case 3750:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeed, GetMotFunc(__FUNC__,Task)))
            {
                Task=3800;
                flag1=false;
                if(LastSet.iRealDummy==REALLY)
                {
                    iPos=IsNNMode();
                    ErrPart=" ";                                                //Steven 20160323 : Auto Clean掉料顯示
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.Suck[i][j].Enable       &&
                               FTestSuck.Suck[i][j].SenUsing!="" &&
                               FTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                               FTestSuck.Item[i][j]!=NULL_IC     &&
                               bTestSuckUse[i][j]==false)
                            {
                                if(FTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    FTestSuck.Suck[i][j].Normal();              //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                    flag1=true;
                                    ErrPart+=IndexSuckName[i+iPos][j];
                                    bIsSuckICFallDown[i][j]=true;               //kevin 20150624
                                }
                            }
                        }
                    }
                }

                if(flag1)
                {
                    MOT[MTestZ1].Gali_Command("ST", GetMotFunc(__FUNC__,Task));
                    Task=3760;
                }
            }
            break;
        case 3760:                                                              //Sam 20221220 : 補 Index Arm1
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeed, GetMotFunc(__FUNC__,Task)))
            {
                Task=3770;
            }
            break;
        case 3770:
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;
            if(CosFunction.bJAM0303NeedOpenChamberDoor)                         //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                bIsTestSitICFallDown=true;                                      //kevin 20130706
            if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX || CUSTOMER_CODE==CC_KYEC_CHEN)
                ret=ShowErrorMessage("JAM0314", K_RETRY, MTestZ1, false, ErrPart);  //Steven 20100129 : Device Drop Error   //JerryYang 20160511 JAM0303->JAM0314,將IC及Clean pad相關的alarm code分開
            else
                ret=ShowErrorMessage("JAM0314", K_SKIP, MTestZ1, false, ErrPart);   //Steven 20100129 : Device Drop Error    //JerryYang 20160511 JAM0303->JAM0314,將IC及Clean pad相關的alarm code分開

            if(ret==K_SKIP)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])                             //kevin 20150624
                            FTestSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);     // 掉料忽視
                    }
                }
            }
            Task=3800;
            break;
        case 3800:                                                              //step go to end
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedSY, GetMotFunc(__FUNC__,Task)))    //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            {
                if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    Task=3810;
                else
                    Task=3840;
            }
            break;
        case 3810:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Place+TestIF_File.iAutoClean_IndexReleaseOffset, iSpeed, "DoIndexAutoClean_Arm1PickArm2Test_3810"))  //Jou 2015-08-22 Auto clean Index Release Offset
            {
                Task=3820;
            }
            break;
        case 3820:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FTestSuck.Item[i][j]>0)
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                           FTestSuck.Suck[i][j].Destroy())
                        {
                            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)    //Clean Kit
                                FLCarryKit.MoveSuckData(FTestSuck, i, j);
                        }
                    }
                }
            }

            flag1=false;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FTestSuck.Item[i][j] && FTestSuck.Suck[i][j].Error==false)
                    {
                        flag1=true;
                        break;
                    }
                }
            }

            if(flag1==false)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            Task=3830;
                            return;
                        }
                    }
                }
            }

            if(FTestSuck.UseSiteHasIC())
                break;

            for(int i=0; i<MAX_Index_Row; i++)                                  //Steven 20100105
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                    bDuplicateErr[i][j]=false;
            Task=3840;
            break;
        case 3830:                                                              //破壞異常處理
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                        FTestSuck.Suck[i][j].Error=false;                       //Steven 20101229 : 換位置
                        bDuplicateErr[i][j]=true;                               //Steven 20101229 : 換位置
                    }
                    else
                    {
                        bDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);   //Vacuum sensor OFF error

            bHasErr=false;
            Task=3820;
            break;
        case 3840:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean 3840"))
            {
                if((iInArmType==e9045_1x3_4 ||                                  //Steven 20220922 : Fixed for 1x3 auto clean
                    iInArmType==e9045_1x3_2_14) &&
//                   IniConfig.bE43AutoCleanUseHotplate==false &&
                   TestIF_File.iAutoClean_DeveicePices%3==0)
                {
                    FLCarryKit.SetItemData(0, 3, NULL_IC);
                }
                Task=3850;
            }
            break;
        case 3850:
            MOT[MInShuttle1].fCanMoveM=true;
            bInedxCleanFinish[0]=true;                                          //kevin 20170520 (wei) Autoclean index Arm 1做完
            Task=3900;
        case 3900:
            MOT[MInShuttle2].fCanMoveM=true;
            bInedxCleanFinish[1]=true;                                          //kevin 20170520 (wei) Autoclean index Arm 2做完
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
            {
                bRunAutoClean=false;                                            //結束Auto Clean動作
                bAutoCleanFinishOnlyUseRTC=true;                                //JerryYang 20161216 (Steven) 做完auto clean後只靠RTC檢查Socket,不做index check
                iAutoClean_IndexContactCount=0;
                CleanSetSpeed(false);
            }
            break;
    }
}
//------------------------------------------------------------------------------
void DoIndexAutoClean()
{
    int &Task=iDoIndexAutoCleanTask;
    bool bFlagY1=false, bFlagS2=false, flag=false, flag1=false;
    int iSpeed, iSpeedSZ, iIndexSpeed, iSpeedSY, iPos;                          //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
    static int iContactCount;
    AnsiString Message;

    if(IniConfig.bF16CheckShuttleSensorBroken && bDoingF16)                     //Steven 20221213 : 確認shuttle 有沒有斷線
        return;

    if(TestIF_File.iAutoClean_MotorSpeed[2]>=80)
        iIndexSpeed=TestIF_File.iAutoClean_MotorSpeed[2]-20;                    //kevin 20120919
    else
        iIndexSpeed=TestIF_File.iAutoClean_MotorSpeed[2];

    iSpeed=MOT[MTestY1].Motor->PJogHighSpeed*iIndexSpeed/100;
    if(iSpeed>=300000)                                                          //kevin 20120919 GAIL SPEED > 3000000 RC= -2
       iSpeed=300000;
    iSpeedSZ=iSpeed*2;
    if(iSpeedSZ>=300000)
       iSpeedSZ=300000;

    int ret;
    int iRet=0;
    static AnsiString ErrPart="";                                               //Steven 20160323 : Auto Clean掉料顯示, 加上static
    bool bHasDuplicateErr=false;
    bool bHasErr=false, bCheckDestroy=false, bCheckSuck=false;                  //kevin 20180716
    static bool bDuplicateErr[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};

    static bool bSuckFinish[MAX_SOCKET_ROW][MAX_SOCKET_COL]  ={{false, false, false, false, false, false, false, false},    //Steven 20110301 : 確認吸取完成
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false},
                                                               {false, false, false, false, false, false, false, false}};

    static bool bTestSuckUse[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false,false,false,false,false,false,false,false},
                                                              {false,false,false,false,false,false,false,false},
                                                              {false,false,false,false,false,false,false,false},
                                                              {false,false,false,false,false,false,false,false}};

    static bool bIsSuckICFallDown[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{false,false,false,false,false,false,false,false},       //kevin 20150624 掉料使用
                                                                   {false,false,false,false,false,false,false,false},
                                                                   {false,false,false,false,false,false,false,false},
                                                                   {false,false,false,false,false,false,false,false}};

    if(bFullViewCheckFinish==false)                                             //JerryYang 20160331 做完FulL view check流程才進來,避免同時對index arm馬達下指令
        return;

    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
    {
        iSpeedSZ=iSpeed;
        iSpeedSY=iSpeed;
    }
    else
    {
        iSpeedSY=50000;
    }

    if(RunAutoCleanByArmPickArm2Test())                                         //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
    {
        DoIndexAutoClean_Arm1PickArm2Test(iSpeed, iSpeedSY, iSpeedSZ, iContactCount,
                                          bDuplicateErr, bSuckFinish,
                                          bIsSuckICFallDown,
                                          bTestSuckUse, ErrPart);
        return;
    }

    if(Task>=400 && Task<=1900 && MOT[MInShuttle1].IsCanMove())
        return;

    if(Task>=2400 && Task<=3900 && MOT[MInShuttle2].IsCanMove())
        return;

    switch(Task)
    {
        case 1:
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
            {
                InitialSet();
            }
            fHome->InitDoTestZHome();
            bInedxCleanFinish[0]=false;                                         //kevin 20170520 (wei) Autoclean index Arm 1做完
            bInedxCleanFinish[1]=false;                                         //kevin 20170520 (wei) Autoclean index Arm 2做完
            Task=100;
            break;
        case 100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean 100"))
            {
                if(CheckAutoCleanCloseSite(0))                                  //Steven 20220929 : 判斷開關Site
                {
                    if(BTestSuck.HasDefineIC(HAS_CLEAN_IC))                     //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                    {
                        Task=2650;
                    }
                    else if(BTestSuck.HasDefineIC(CLEAN_FINISH_IC))             //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                    {
                        Task=3500;
                    }
                    else
                    {
                        Task=2100;
                    }
                }
                else
                {
                    if(CheckAutoCleanCloseSite(0)==false)                       //Steven 20220929 : 判斷開關Site
                    {
                        if(FTestSuck.HasDefineIC(HAS_CLEAN_IC))                 //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                        {
                            Task=650;
                        }
                        else if(FTestSuck.HasDefineIC(CLEAN_FINISH_IC))         //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                        {
                            Task=1500;
                        }
                        else
                        {
                            Task=200;
                        }
                    }

                    if((CosFunction.bAutoCleanAutoSelIndexArm==false && TestIF.iAutoClean_SelectArm==1) ||
                       (CosFunction.bAutoCleanAutoSelIndexArm==true && TestIF.bCleanIndexOtherArm==false &&
                        TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1))
                    {
                        if(BTestSuck.HasDefineIC(HAS_CLEAN_IC))                 //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                        {
                            Task=2650;
                        }
                        else if(BTestSuck.HasDefineIC(CLEAN_FINISH_IC))         //Steven 20130620 : Auto Clean到一半按歸零要繼續跑
                        {
                            Task=3500;
                        }
                        else
                        {
                            Task=2100;
                        }
                    }
                }
            }
            break;
        case 200:
            //重置index狀態
            fHome->InitDoTestZHome();
            iContactCount=0;
            Task=300;
            break;
        case 300:
            if(EP_Install)
            {
                ADAM_WriteVoltage(TestIF.fAutoClean_AireForce);
            }
            bFlagY1=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedSY, "DoIndexAutoClean 300");//kevin 20120517 iSpeed);    //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            bFlagS2=false;

            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
            {
                if(FLCarryKit.UseSiteHasIC() && InSHT1InRT())                   //Sam 20210923 : 修正 AutoClean Index Arm 偷跑壓空氣導致  Hang up    //Steven 20220211 : HasRealIC --> HasIC
                {
                    MOT[MInShuttle1].fCanMoveM=false;                           //kevin 20170119 index cleanpad尚未吸取 shuttle不能移動
                    bFlagS2=true;
                }
            }
            else if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
            {
                bFlagS2=true;
            }

            if(bFlagY1 && bFlagS2)
            {
                MOT[MInShuttle1].fCanMoveM=false;

                if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    Task=400;
                else
                    Task=650;
            }
            break;
        case 400:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Pick+TestIF_File.iAutoClean_IndexPickOffset, iSpeed, "DoIndexAutoClean_Arm1PickArm2Test_400"))        //wei 20150318 Auto clean Index Pick Offset
            {
                DoIndexAutoCleanDelay.SetMSAndOn(500);
                Task=500;
            }
            break;
        case 500:
            if(DoIndexAutoCleanDelay.Off())
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        bSuckFinish[i][j]=false;                                //Steven 20110301 : 初始化，都當作還沒做完
                        bTestSuckUse[i][j]=false;
                    }
                }
                FTestSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=600;
            }
            break;
        case 600:
            flag1=true;
            for(int i=0; i<MAX_Index_Row; i++)                                  //交換IC狀態
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FLCarryKit.Item[i][j])
                    {
                        if(FLCarryKit.Item[i][j]==HAS_NULL_CLEAN_IC)
                        {
                            FTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                            FTestSuck.MoveSuckData(FLCarryKit, i, j);
                            bSuckFinish[i][j]=true;                             //Steven 20110301
                        }
                        else if(bSuckFinish[i][j]==true)
                        {
                            if(FTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                               FTestSuck.Item[i][j]==NULL_IC)                   //Steven 20111202 : Retry會掉料
                            {
                                FTestSuck.Suck[i][j].Normal();                  //Steven 20111201 : 預防負壓壓降
                            }
                        }
                        else
                        {
                            if(fMain->bAutoCleanTest==true && i==1 && j==2)
                            {
                                FTestSuck.Suck[i][j].Error=true;
                                fMain->bAutoCleanTest=false;
                                bSuckFinish[i][j]=true;
                                return;
                            }
                            #ifdef SOFT_SIMULTE
                            int x=atoi(fMain->edHPY->Text.c_str());
                            int y=atoi(fMain->edHPX->Text.c_str());
                            if(fMain->cbIndexDrop->Checked==true && i==x && j==y)
                            {
                                FTestSuck.Suck[i][j].Error=true;
                                fMain->bAutoCleanTest=false;
                                bSuckFinish[i][j]=true;
                                continue;
                            }
                            #endif

                            if(FTestSuck.Suck[i][j].Suck())
                            {
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);
                                bDuplicateErr[i][j]=false;                      //Steven 20100105
                                bSuckFinish[i][j]=true;                         //Steven 20110301 : 吸取完成的不做
                            }
                            else if(FTestSuck.Suck[i][j].Error)                 //Steven 20110301 : 有錯誤的不做
                            {
                                bSuckFinish[i][j]=true;
                            }
                            else
                            {
                                flag1=false;                                    //jou 2011-08-16 只要有未完成的就繼續等
                            }
                        }
                    }
                    else
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                           FTestSuck.Item[i][j]==NULL_IC)                       //Steven 20111202 : Retry會掉料
                        {
                            FTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                        }
                        bSuckFinish[i][j]=true;                                 //Steven 20110301 : 沒有東西的地方要跳過
                    }
                }
            }

            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bSuckFinish[i][j]==false)                                //只要有未完成的就繼續等
                        flag1=false;
                }
            }

            if(flag1==true)                                                     //Steven 20110301 : 所有吸嘴都做完
            {
                for(int i=0; i<MAX_Index_Row; i++)                              //若有吸取錯誤
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            Task=650;
                            return;
                        }
                    }
                }

                if(FLCarryKit.HasRealIC())
                    break;

                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC)
                        {
                            FTestSuck.Suck[i][j].Normal();
                        }
                        bDuplicateErr[i][j]=false;
                    }
                }

                if(FLCarryKit.UseSiteHasIC())
                    break;

                Task=700;
            }
            break;
        case 650:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean_650"))
            {
                if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    Task=651;
                else
                    Task=900;
            }
            break;
        case 651:                                                               //Richard 20230418 : pickupError shutter move to left position
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            iPos=IsNNMode();

            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iPos][j];
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(IniConfig.bD43IndexDropErrorCanRetryandSkip==true &&
               bHasErr==true)
            {
                MOT[MInShuttle1].fCanMoveM=true;
                bAutoCleanShuttle1MoveToLeft=true;
                bAutoCleanShuttle1HasPickErr=true;
                Task=652;
            }
            else
            {
                Task=660;
            }
            break;
        case 652:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
               bAutoCleanShuttle1MoveToLeft)                                    //ChungHung 20131015 fix hangup
            {
                MOT[MInShuttle1].fCanMoveM=true;
                if(InSHT1InLF()!=true)
                {
                    break;
                }
            }
            MOT[MInShuttle1].fCanMoveM=false;
            Task=660;
        case 660:
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            iPos=IsNNMode();
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iPos][j];
                    }
                    else
                    {
                        FTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(bHasErr)
            {
                bHasErr=false;                                                  //Ifor 20160322 移動至上方
                if(DoInArmMoveToWaitPosByAutoClean()==false)                    //JerryYang 20241118 : fix auto clean hang up    //Steven 20130613 : Index異常時, In Arm要先讓位功能
                {
                    return ;
                }
                bIsTestSitICFallDown=true;

                if(IniConfig.bIndexPickErrOnlySKIP==true ||                     //kevin 201701103 (Steven) index pick up error only skip
                   IniConfig.bD64IndexPickErrOnlySKIP)                          //JerryYang 20160301 index pick-up error only skip
                {
                    ret=ShowErrorMessage("JAM0312", K_SKIP, MTestZ1, bHasDuplicateErr, ErrPart); //Device Pick-Up Error             //JerryYang 20160511 JAM0301->JAM0312,將IC及Clean pad相關的alarm code分開
                }
                else
                {
                    ret=ShowErrorMessage("JAM0312", K_SKIP|K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart); //Devicr Pick-Up Error     //JerryYang 20160511 JAM0301->JAM0312,將IC及Clean pad相關的alarm code分開
                }

                if(ret==K_SKIP)
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.Suck[i][j].Error)
                            {
                                FTestSuck.MoveSuckData(FLCarryKit, i, j);
                                FTestSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC); //Steven 20250326 : fixed for auto clean
                                FTestSuck.Suck[i][j].Normal();
                                bTestSuckUse[i][j]=true;
                            }
                            bDuplicateErr[i][j]=false;
                        }
                    }
                }
                else
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.Suck[i][j].Error)
                            {
                                bDuplicateErr[i][j]=true;
                            }
                        }
                    }
                }
                FTestSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                bAutoCleanShuttle1MoveToLeft=false;
                MOT[MInShuttle1].fCanMoveM=true;
            }
            bHasErr=false;
            bAutoCleanShuttle1HasPickErr=false;
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)
                {
                    Task=670;
                }
                else
                {
                    if(FLCarryKit.HasRealIC())
                        Task=400;
                    else
                        Task=700;
                }
            }
            break;
        case 670:
            if(InSHT1InRT())
            {
                MOT[MInShuttle1].fCanMoveM=false;
                if(FLCarryKit.HasRealIC())
                    Task=400;
                else
                    Task=700;
            }
            break;
        case 700:                                                               //Steven 20160323 : 吸好之後要停一下
            DoIndexAutoCleanDelay.SetMSAndOn(500);
            Task=710;
        case 710:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=720;
            }
            break;
        case 720:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean_720"))
            {
                Task=800;
            }
            break;
        case 800:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FTestSuck.Suck[i][j].GetStatus()==false)
                        FTestSuck.Suck[i][j].Normal();
                }
            }
            Task=900;
            break;
        case 900:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedSY, "DoIndexAutoClean 900"))//kevin 20120517 iSpeed))    //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            {
                Task=1000;
            }
            break;
        case 1000:
            fHome->InitDoTestZHome();
            Task=1050;
            break;
        case 1050:                                                              //kevin 20180716 drop contract 移至下拋高度
            if(Prod.iAutoClean_ContactMode==0)                                  //Sam 20230111 : Smart Auto Clean //kevin 20180716 direction
            {
                Task=1150;
            }
            else
            {
                if(MOT[MTestZ1].Gali_MotMove(Prod.iAutoCleanZ_Drop[0], iSpeedSZ, "DoIndexAutoClean_1050")) //kevin 20180717 drop high
                    Task=1055;
            }
            break;
        case 1055:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FTestSuck.Item[i][j]==HAS_CLEAN_IC)
                    {
                        FTestSuck.Suck[i][j].Off();
                    }
                }
            }
            DoTestYRearDelayAC.SetSecAndOn(Prod.TestZ_Drop_Wait);               //Steven 20140909 : 換到迴圈外面
            Task=1060;
        case 1060:
            bCheckDestroy=true;
            if(bCheckDestroy==true && DoTestYRearDelayAC.Off())
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_CLEAN_IC)
                            FTestSuck.Suck[i][j].Normal();
                    }
                }
                bIndexCheckNoStopVaccum=false;
                Task=1150;
            }
            break;
        case 1150:
            if(MOT[MTestZ1].Gali_MotMove(Prod.iAutoCleanZ_Contact[0], iSpeedSZ, "DoIndexAutoClean_1150"))//kevin 20181016 add drop 過壓
            {
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
                    SW[SwSocketClean].Off();

                if(Prod.iAutoClean_ContactMode==0)                              //Sam 20230111 : Smart Auto Clean //kevin 20180716 direction
                {
                    Task=1200;
                    DoIndexAutoCleanDelay.Set0_1SecAndOn(Prod.iAutoClean_ContactTime);  //Sam 20230111 : Smart Auto Clean
                    iContactCount++;
                    TestIF_File.iIndexArmAutoCleanCnt++;                        //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
                }
                else
                {
                    Task=1160;                                                  //KEVIN 20180724 ADD
                }
            }
            break;
        case 1160:
            if(DeviceForm.VacuumMode==VacuumONMode)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Item[i][j]==HAS_CLEAN_IC)
                        {
                            FTestSuck.Suck[i][j].On();
                            if(INDEX_SUCKER_TYPE==1)
                            {
                                fiosetview->bIndexSuck[0][i][j]=true;
                            }
                        }
                    }
                }
            }

            DoTestYRearDelayAC.SetMSAndOn(100);
            Task=1170;
            break;
        case 1170:
            if(INDEX_SUCKER_TYPE==1)
            {
                bCheckSuck=fiosetview->ProcessIndexSuckDestroy1();
            }
            else
            {
                bCheckSuck=true;
            }

            if(bCheckSuck==true && DoTestYRearDelayAC.Off())
            {
                bFTestSuckDrop=false;
                DoIndexAutoCleanDelay.Set0_1SecAndOn(Prod.iAutoClean_ContactTime);  //Sam 20230111 : Smart Auto Clean
                iContactCount++;
                TestIF_File.iIndexArmAutoCleanCnt++;                            //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
                Task=1200;
            }
            break;
        case 1200:                                                              //在清潔位置等待
            if(DoIndexAutoCleanDelay.Off())
            {
                if(iContactCount<Prod.iAutoClean_ContactCount)                  //Sam 20230111 : Smart Auto Clean
                {
                    Task=1300;
                }
                else
                {
                    if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    {
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(FTestSuck.Item[i][j])
                                {
                                    if(FTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC)
                                        FTestSuck.SetItemData(i, j, CLEAN_FINISH_IC);         //Steven 20130701
                                }
                            }
                        }
                    }
                    Task=1350;
                }
            }
            break;
        case 1300:
            if(MOT[MTestZ1].Gali_MotMove(Prod.iAutoCleanZ_Shift[0], iSpeedSZ, "DoIndexAutoClean_1300"))  //kevin 20181016 add drop 過壓
            {
                flag=false;
                if(LastSet.iRealDummy==REALLY)
                {
                    iPos=IsNNMode();
                    ErrPart=" ";                                                //Steven 20160323 : Auto Clean掉料顯示
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            bIsSuckICFallDown[i][j]=false;                      //kevin 20150624
                            if(FTestSuck.Suck[i][j].Enable       &&
                               FTestSuck.Suck[i][j].SenUsing!="" &&
                               FTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                               FTestSuck.Item[i][j]!=NULL_IC     &&
                               bTestSuckUse[i][j]==false)
                            {
                                if(FTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    FTestSuck.Suck[i][j].Normal();              //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                    flag=true;
                                    ErrPart+=IndexSuckName[i+iPos][j];
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case 1300"));

                    if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    {
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(FTestSuck.Item[i][j])
                                {
                                    if(FTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC)
                                        FTestSuck.SetItemData(i, j, CLEAN_FINISH_IC);
                                }
                            }
                        }
                    }
                    DoIndexAutoCleanDelay.SetMSAndOn(100);
                    Task=1360;
                    break;
                }

                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
                    SW[SwSocketClean].On();

                DoIndexAutoCleanDelay.SetMSAndOn(100);
                Task=1310;
            }
            break;
        case 1310:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=1050;
            }
            break;
        case 1350:
            if(MOT[MTestZ1].Gali_MotMove(Prod.iAutoCleanZ_Shift[0],iSpeedSZ, "DoIndexAutoClean_1350"))   //JerryYang 20181119 (Steven) : fix auto clean 高度異常
            {
                for(int i=0; i<MAX_Index_Row; i++)                              //ChungHung 20150407 add for SCK drop issue
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        bDuplicateErr[i][j]=false;
                        bIsSuckICFallDown[i][j]=false;                          //kevin 20150624
                    }
                }
                Task=1360;
                DoIndexAutoCleanDelay.SetMSAndOn(100);
            }
            break;
        case 1360:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=1400;
            }
            break;
        case 1400:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean 1400"))
            {
                Task=1500;

                flag=false;
                if(LastSet.iRealDummy==REALLY)
                {
                    iPos=IsNNMode();
                    ErrPart=" ";                                                //Steven 20160323 : Auto Clean掉料顯示
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FTestSuck.Suck[i][j].Enable       &&
                               FTestSuck.Suck[i][j].SenUsing!="" &&
                               FTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                               FTestSuck.Item[i][j]!=NULL_IC     &&
                               bTestSuckUse[i][j]==false)
                            {
                                if(FTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    FTestSuck.Suck[i][j].Normal();              //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                    flag=true;
                                    ErrPart+=IndexSuckName[i+iPos][j];
                                    bIsSuckICFallDown[i][j]=true;               //kevin 20150624
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    MOT[MTestZ1].Gali_Command("ST", __FUNC__+AnsiString(",case 1400"));
                    Task=1410;
                }
            }
            break;
        case 1410:                                                              //Sam 20221220 : 補 Index Arm1
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front,Prod.TestY2_Rear, iSpeed, __FUNC__+AnsiString("case 1410")))
                Task=1420;
            break;
        case 1420:
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            if(CosFunction.bJAM0303NeedOpenChamberDoor)                         //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                bIsTestSitICFallDown=true;                                      //kevin 20130706

            if(CUSTOMER_CODE==CC_KYEC_LEE ||
               CUSTOMER_CODE==CC_KYEC_XILINX ||
               CUSTOMER_CODE==CC_KYEC_CHEN)
                iRet=ShowErrorMessage("JAM0314", K_RETRY, MTestZ1, false, ErrPart); //Steven 20100129 : Device Drop Error   //JerryYang 20160511 JAM0303->JAM0314,將IC及Clean pad相關的alarm code分開
            else
                iRet=ShowErrorMessage("JAM0314", K_SKIP, MTestZ1, false, ErrPart);  //Steven 20100129 : Device Drop Error    //JerryYang 20160511 JAM0303->JAM0314,將IC及Clean pad相關的alarm code分開

            if(iRet==K_SKIP)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])                             //kevin 20150624
                            FTestSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);     // 掉料忽視
                    }
                }
            }
            Task=1500;
            break;
        case 1500:
            if(MOT[MTestY1].Gali_MotMove(Prod.TestY1_Front, iSpeedSY, "DoIndexAutoClean_1500"))          //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            {
                if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    Task=1600;
                else
                    Task=1800;
            }
            break;
        case 1600:
            if(MOT[MTestZ1].Gali_MotMove(Prod.TestZ1_Place+TestIF_File.iAutoClean_IndexReleaseOffset, iSpeed, "DoIndexAutoClean_1600"))  //Jou 2015-08-22 Auto clean Index Release Offset
            {
                Task=1700;
            }
            break;
        case 1700:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FTestSuck.Item[i][j])
                    {
                        if(FTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                           FTestSuck.Suck[i][j].Destroy())
                        {
                            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)    //Clean Kit
                                FLCarryKit.MoveSuckData(FTestSuck, i, j);
                        }
                    }
                }
            }

            flag=false;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FTestSuck.Item[i][j] && FTestSuck.Suck[i][j].Error==false)
                    {
                        flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Suck[i][j].Error)
                        {
                            Task=1750;
                            return;
                        }
                    }
                }
            }

            if(FTestSuck.UseSiteHasIC())
                break;

            for(int i=0; i<MAX_Index_Row; i++)
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                    bDuplicateErr[i][j]=false;
            Task=1800;
            break;
        case 1750:
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            iPos=IsNNMode();
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(FTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i+iPos][j];
                        FTestSuck.Suck[i][j].Error=false;                       //Steven 20101229 : 換位置
                        bDuplicateErr[i][j]=true;                               //Steven 20101229 : 換位置
                    }
                    else
                    {
                        bDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ1, bHasDuplicateErr, ErrPart);   //Vacuum sensor OFF error

            bHasErr=false;
            Task=1700;
            break;
        case 1800:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeed, "DoIndexAutoClean 1800"))
            {
                if((iInArmType==e9045_1x3_4 ||                                  //Steven 20220922 : Fixed for 1x3 auto clean
                    iInArmType==e9045_1x3_2_14) &&
//                   IniConfig.bE43AutoCleanUseHotplate==false &&
                   TestIF_File.iAutoClean_DeveicePices%3==0)
                {
                    FLCarryKit.SetItemData(0, 3, NULL_IC);
                }
                Task=1900;
            }
            break;
        case 1900:
            MOT[MInShuttle1].fCanMoveM=true;
            bInedxCleanFinish[0]=true;                                          //kevin 20170520 (wei) Autoclean index Arm 1做完

            if((CosFunction.bAutoCleanAutoSelIndexArm==false && TestIF.iAutoClean_SelectArm==2) ||
               (CosFunction.bAutoCleanAutoSelIndexArm==true  && TestIF.bCleanIndexOtherArm==true))
            {
                if(CheckAutoCleanCloseSite(1)==false)                           //Steven 20220929 : 判斷開關Site
                {
                    Task=2200;
                    break;
                }
            }

            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
            {
                bRunAutoClean=false;
                bAutoCleanFinishOnlyUseRTC=true;                                //JerryYang 20161216 (Steven) 做完auto clean後只靠RTC檢查Socket,不做index check
                iAutoClean_IndexContactCount=0;
                CleanSetSpeed(false);
            }
            break;
        case 2100:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeed, "DoIndexAutoClean 2100"))
            {
                Task=2200;
            }
            break;
        case 2200:
            fHome->InitDoTestZHome();
            iContactCount=0;
            Task=2300;
            break;
        case 2300:
            if(EP_Install)
            {
                ADAM_WriteVoltage(TestIF.fAutoClean_AireForce);
            }

            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                             //JimmyChiu 20251101 : add Index Arm Axis
            {
                bFlagY1=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedSY, "DoIndexAutoClean 2300");//kevin 20120517 iSpeed);   //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            }
            else
            {
                bFlagY1=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, iSpeedSY, "DoIndexAutoClean 2300");//kevin 20120517 iSpeed);   //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            }
            bFlagS2=false;

            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
            {
                if(BLCarryKit.UseSiteHasIC() && InSHT2InRT())
                {
                    MOT[MInShuttle2].fCanMoveM=false;                           //kevin 20170119 index cleanpad尚未吸取 shuttle不能移動
                    bFlagS2=true;
                }
            }
            else if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
            {
                bFlagS2=true;
            }

            if(bFlagY1 && bFlagS2)
            {
                MOT[MInShuttle2].fCanMoveM=false;

                if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    Task=2400;
                else
                    Task=2650;
            }
            break;
        case 2400:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Pick+TestIF_File.iAutoClean_IndexPickOffset, iSpeed, "DoIndexAutoClean_2400"))         //wei 20150318 Auto clean Index Pick Offset
            {
                DoIndexAutoCleanDelay.SetMSAndOn(500);
                Task=2500;
            }
            break;
        case 2500:
            if(DoIndexAutoCleanDelay.Off())
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        bSuckFinish[i][j]=false;                                //Steven 20110301 : 初始化，都當作還沒做完
                        bTestSuckUse[i][j]=false;
                    }
                }
                BTestSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                Task=2600;
            }
            break;
        case 2600:
            flag1=true;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(BLCarryKit.Item[i][j])
                    {
                        if(BLCarryKit.Item[i][j]==HAS_NULL_CLEAN_IC)
                        {
                            BTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                            BTestSuck.MoveSuckData(BLCarryKit, i, j);
                            bSuckFinish[i][j]=true;                             //Steven 20110301
                        }
                        else if(bSuckFinish[i][j]==true)
                        {
                            if(BTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                               BTestSuck.Item[i][j]==NULL_IC)                   //Steven 20111202 : Retry會掉料
                            {
                                BTestSuck.Suck[i][j].Normal();                  //Steven 20111201 : 預防負壓壓降
                            }
                        }
                        else
                        {
                            #ifdef SOFT_SIMULTE
                            int x=atoi(fMain->edHPY->Text.c_str());
                            int y=atoi(fMain->edHPX->Text.c_str());
                            if(fMain->cbIndexDrop->Checked==true && i==x && j==y)
                            {
                                BTestSuck.Suck[i][j].Error=true;
                                fMain->bAutoCleanTest=false;
                                bSuckFinish[i][j]=true;
                                continue;
                            }
                            #endif

                            if(BTestSuck.Suck[i][j].Suck())
                            {
                                BTestSuck.MoveSuckData(BLCarryKit, i, j);
                                bDuplicateErr[i][j]=false;                      //Steven 20100105
                                bSuckFinish[i][j]=true;                         //Steven 20110301 : 吸取完成的不做
                            }
                            else if(BTestSuck.Suck[i][j].Error)                 //Steven 20110301 : 有錯誤的不做
                            {
                                bSuckFinish[i][j]=true;
                            }
                            else
                            {
                                flag1=false;                                    //jou 2011-08-16 只要有未完成的就繼續等
                            }
                        }
                    }
                    else
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                           BTestSuck.Item[i][j]==NULL_IC)                       //Steven 20111202 : Retry會掉料
                        {
                            BTestSuck.Suck[i][j].Normal();                      //Steven 20111201 : 預防負壓壓降
                        }
                        bSuckFinish[i][j]=true;                                 //Steven 20110301 : 沒有東西的地方要跳過
                    }
                }
            }

            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bSuckFinish[i][j]==false)                                //只要有未完成的就繼續等
                        flag1=false;
                }
            }

            if(flag1==true)                                                     //Steven 20110301 : 所有吸嘴都做完
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            Task=2650;
                            return;
                        }
                    }
                }

                if(BLCarryKit.HasRealIC())
                    break;

                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC)
                        {
                            BTestSuck.Suck[i][j].Normal();
                        }
                        bDuplicateErr[i][j]=false;
                    }
                }

                if(BLCarryKit.UseSiteHasIC())
                    break;

                Task=2700;
            }
            break;
        case 2650:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, iSpeed, "DoIndexAutoClean_2650"))
            {
                if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    Task=2651;
                else
                    Task=2900;
            }
            break;
        case 2651:                                                              //Richard 20230418 : add Index Pick Error Can Retry and Start
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;

            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                    }
                    else
                    {
                        BTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(IniConfig.bD43IndexDropErrorCanRetryandSkip==true && bHasErr==true)
            {
                MOT[MInShuttle2].fCanMoveM=true;
                bAutoCleanShuttle2MoveToLeft=true;
                bAutoCleanShuttle2HasPickErr=true;
                Task=2652;
            }
            else
            {
                Task=2660;
            }
            break;
        case 2652:
            if(IniConfig.bD43IndexDropErrorCanRetryandSkip &&
               bAutoCleanShuttle2MoveToLeft)
            {
                MOT[MInShuttle2].fCanMoveM=true;
                if(InSHT2InLF()!=true)
                {
                    break;
                }
            }
            MOT[MInShuttle2].fCanMoveM=false;
            Task=2660;
        case 2660:
            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;

            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                    }
                    else
                    {
                        BTestSuck.Suck[i][j].Error=false;
                    }
                }
            }

            if(bHasErr)
            {
                bHasErr=false;

                if(CosFunction.bJAM0301NeedOpenChamberDoor)                     //wei : JAM0301 & JAM0302需要開啟Chamber門10秒
                {
                    if(DoInArmMoveToWaitPosByAutoClean()==false)                //JerryYang 20241118 : fix auto clean hang up    //Steven 20130613 : Index異常時, In Arm要先讓位功能
                    {
                        return ;
                    }
                    bIsTestSitICFallDown=true;
                }

                if(IniConfig.bIndexPickErrOnlySKIP==true ||                     //kevin 201701103 (Steven) index pick up error only skip
                   IniConfig.bD64IndexPickErrOnlySKIP)                          //JerryYang 20160301 index pick-up error only skip
                    ret=ShowErrorMessage("JAM0313", K_SKIP, MTestZ2, bHasDuplicateErr, ErrPart);            //Devicr Pick-Up Error                      //JerryYang 20160511 JAM0302->JAM0313,將IC及Clean pad相關的alarm code分開
                else
                    ret=ShowErrorMessage("JAM0313", K_SKIP|K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);    //Steven 20091123 : Devicr Pick-Up Error    //JerryYang 20160511 JAM0302->JAM0313,將IC及Clean pad相關的alarm code分開

                if(ret==K_SKIP)
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BTestSuck.Suck[i][j].Error)
                            {
                                BTestSuck.MoveSuckData(BLCarryKit, i, j);
                                BTestSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC); //Steven 20250326 : fixed for auto clean
                                BTestSuck.Suck[i][j].Normal();
                                bTestSuckUse[i][j]=true;
                            }
                            bDuplicateErr[i][j]=false;
                        }
                    }
                }
                else
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BTestSuck.Suck[i][j].Error)
                            {
                                bDuplicateErr[i][j]=true;
                            }
                        }
                    }
                }
                BTestSuck.ResetAll();                                           //Steven 20160323 : 避免未開啟真空
                bAutoCleanShuttle2MoveToLeft=false;
                MOT[MInShuttle2].fCanMoveM=true;
            }

            bHasErr=false;
            bAutoCleanShuttle1HasPickErr=false;
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
            {
                if(IniConfig.bD43IndexDropErrorCanRetryandSkip)
                {
                    Task=2670;
                }
                else
                {
                    if(BLCarryKit.HasRealIC())
                        Task=2400;
                    else
                        Task=2700;
                }
            }
            break;
        case 2670:
            if(InSHT2InRT())
            {
                MOT[MInShuttle2].fCanMoveM=false;
                if(BLCarryKit.HasRealIC())                                      //有IC->Retry
                    Task=2400;
                else
                    Task=2700;                                                  //無IC->Skip
            }
            break;
        case 2700:                                                              //Steven 20160323 : 吸好之後要停一下
            DoIndexAutoCleanDelay.SetMSAndOn(500);
            Task=2710;
        case 2710:
            if(DoIndexAutoCleanDelay.Off())
            {
                Task=2720;
            }
            break;
        case 2720:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Safe, iSpeed, "DoIndexAutoClean_2720"))
            {
                Task=2800;
            }
            break;
        case 2800:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(BTestSuck.Suck[i][j].GetStatus()==false)
                        BTestSuck.Suck[i][j].Normal();
                }
            }
            Task=2900;
            break;
        case 2900:
            bFlagY1=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedSY, "DoIndexAutoClean 2900");  //kevin 20120517 iSpeed))  //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣

            if(bFlagY1)
            {
                Task=3000;
            }
            break;
        case 3000:
            fHome->InitDoTestZHome();
            Task=3050;
            break;
        case 3050:                                                              //kevin 20180716 drop contract 移至下拋高度
            if(Prod.iAutoClean_ContactMode==0)                                  //kevin 20180716 direction  //Sam 20230111 : Smart Auto Clean
            {
                 Task=3090;
            }
            else
            {
                if(MOT[MTestZ2].Gali_MotMove(Prod.iAutoCleanZ_Drop[1], iSpeedSZ, "DoIndexAutoClean_3050")) //kevin 20180717 drop high //Steven 20180920 : #P180916-ATK-H9-04 , Auto Clean-Socket Position Offset does not use for Arm2
                    Task=3055;
            }
            break;
        case 3055:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(BTestSuck.Item[i][j]==HAS_CLEAN_IC)
                    {
                        BTestSuck.Suck[i][j].Off();
                    }
                }
            }
            DoTestYRearDelayAC.SetSecAndOn(Prod.TestZ_Drop_Wait);               // delay 0.3 sec for ic down        //Steven 20140909 : 換到迴圈外面
            Task=3060;
            break;
        case 3060:
            bCheckDestroy=true;
            if(bCheckDestroy==true && DoTestYRearDelayAC.Off())
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(BTestSuck.Item[i][j]==HAS_CLEAN_IC)
                            BTestSuck.Suck[i][j].Normal();
                    }
                }
                bIndexCheckNoStopVaccum=false;
                Task=3090;
            }
            break;
        case 3090:                                                              //socket pos kevin 20180630 change
            if(MOT[MTestZ2].Gali_MotMove(Prod.iAutoCleanZ_Contact[1], iSpeedSZ, "DoIndexAutoClean_3090"))
            {                                                                   //kevin 20180630 add clean pad - device thickness
                if(Prod.iAutoClean_ContactMode==0)                              //Sam 20230111 : Smart Auto Clean //kevin 20180716 direction
                {
                    iContactCount++;
                    TestIF_File.iIndexArmAutoCleanCnt++;                        //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
                    DoIndexAutoCleanDelay.Set0_1SecAndOn(Prod.iAutoClean_ContactTime);  //Sam 20230111 : Smart Auto Clean
                    Task=3100;
                }
                else
                {
                    Task=3095;                                                  //drop mode
                }
            }
            break;
        case 3095:
            if(DeviceForm.VacuumMode==VacuumONMode)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(BTestSuck.Item[i][j]==HAS_CLEAN_IC)
                        {
                            BTestSuck.Suck[i][j].On();
                            if(INDEX_SUCKER_TYPE==1)                            //Steven 20111202
                            {
                                fiosetview->bIndexSuck[1][i][j]=true;
                            }
                        }
                    }
                }
            }

            DoTestYRearDelayAC.SetMSAndOn(100);                                 // delay 0.3 sec for ic down
            Task=3096;
            break;
        case 3096:
            if(INDEX_SUCKER_TYPE==1)
            {
                bCheckSuck=fiosetview->ProcessIndexSuckDestroy2();
            }
            else
            {
                bCheckSuck=true;
            }

            if(bCheckSuck==true && DoTestYRearDelayAC.Off())
            {
                bFTestSuckDrop=false;
                iContactCount++;
                TestIF_File.iIndexArmAutoCleanCnt++;                            //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
                DoIndexAutoCleanDelay.Set0_1SecAndOn(Prod.iAutoClean_ContactTime);  //Sam 20230111 : Smart Auto Clean
                Task=3100;
            }
            break;
        case 3100:
            if(DoIndexAutoCleanDelay.Off())
            {
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
                    SW[SwSocketClean].Off();
                if(iContactCount<Prod.iAutoClean_ContactCount)                  //Sam 20230111 : Smart Auto Clean
                {
                    Task=3200;
                }
                else
                {
                    if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    {
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(BTestSuck.Item[i][j])
                                {
                                    if(BTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC)
                                        BTestSuck.SetItemData(i, j, CLEAN_FINISH_IC);         //Steven 20130701
                                }
                            }
                        }
                    }
                    Task=3350;
                }
            }
            break;
        case 3200:                                                              //kevin 20180716 add drop contract
            if(MOT[MTestZ2].Gali_MotMove(Prod.iAutoCleanZ_Shift[1], iSpeedSZ, "DoIndexAutoClean_3200"))
            {
                flag=false;
                if(LastSet.iRealDummy==REALLY)
                {
                    ErrPart=" ";                                                //Steven 20160323 : Auto Clean掉料顯示
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BTestSuck.Suck[i][j].Enable       &&
                               BTestSuck.Suck[i][j].SenUsing!="" &&
                               BTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC &&
                               bTestSuckUse[i][j]==false)
                            {
                                if(BTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    BTestSuck.Suck[i][j].Normal();              //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                    flag=true;
                                    ErrPart+=IndexSuckName[i][j];
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    MOT[MTestZ2].Gali_Command("ST", __FUNC__+AnsiString(",case 3200"));

                    if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    {
                        for(int i=0; i<MAX_Index_Row; i++)
                        {
                            for(int j=0; j<NEW_MAX_Index_Col; j++)
                            {
                                if(BTestSuck.Item[i][j])
                                {
                                    if(BTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC)
                                        BTestSuck.SetItemData(i, j, CLEAN_FINISH_IC);         //Steven 20130701
                                }
                            }
                        }
                    }
                    DoTestYRearDelayAC.SetMSAndOn(100);
                    Task=3360;
                    break;
                }
                if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
                    SW[SwSocketClean].On();
                DoTestYRearDelayAC.SetMSAndOn(100);
                Task=3210;
            }
            break;
        case 3210:
            if(DoTestYRearDelayAC.Off())
            {
                Task=3050;
            }
            break;
        case 3350:
            if(MOT[MTestZ2].Gali_MotMove(Prod.iAutoCleanZ_Shift[1], iSpeedSZ, "DoIndexAutoClean_3350"))  //JerryYang 20181119 (Steven) : fix auto clean 高度異常     //kevin 20181016 add drop 過壓
            {
                for(int i=0; i<MAX_Index_Row; i++)                              //ChungHung 20150407 add for SCK drop issue
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        bDuplicateErr[i][j]=false;
                        bIsSuckICFallDown[i][j]=false;                          //kevin 20150624
                    }
                }
                Task=3360;
                DoTestYRearDelayAC.SetMSAndOn(100);
            }
            break;
        case 3360:
            if(DoTestYRearDelayAC.Off())
            {
                Task=3400;
            }
            break;
        case 3400:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeed, "DoIndexAutoClean 3400"))
            {
                Task=3500;

                flag=false;
                if(LastSet.iRealDummy==REALLY)
                {
                    ErrPart=" ";                                                //Steven 20160323 : Auto Clean掉料顯示
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BTestSuck.Suck[i][j].Enable       &&
                               BTestSuck.Suck[i][j].SenUsing!="" &&
                               BTestSuck.Item[i][j]!=HAS_NULL_CLEAN_IC &&
                               BTestSuck.Item[i][j]!=NULL_IC &&
                               bTestSuckUse[i][j]==false)
                            {
                                if(BTestSuck.Suck[i][j].GetStatus()==false)
                                {
                                    BTestSuck.Suck[i][j].Normal();              //jou 2012-01-17 直接關掉，避免掉到shuttle去，也避免要掉不掉Hang up
                                    flag=true;
                                    ErrPart+=IndexSuckName[i][j];
                                    bIsSuckICFallDown[i][j]=true;               //kevin 20150624
                                }
                            }
                        }
                    }
                }

                if(flag)
                {
                    MOT[MTestZ2].Gali_Command("ST", __FUNC__+AnsiString(",case 3400"));
                    Task=3410;
                }
            }
            break;
        case 3410:                                                              //Jimmychiu 20211022 : #R210901-ATK-H9-03 , The index Arm move on the shuttle when drop error during auto clean.
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front,Prod.TestY2_Rear, iSpeed, __FUNC__+AnsiString("case 3410")))
                Task=3420;
            break;
        case 3420:
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            if(CosFunction.bJAM0303NeedOpenChamberDoor)                         //Steven : JAM0303 & JAM0403需要開啟Chamber門10秒
                bIsTestSitICFallDown=true;                                      //kevin 20130706

            iRet=ShowErrorMessage("JAM0315", K_SKIP, MTestZ2, false, ErrPart);  //Steven 20100129 : Device Drop Error    //JerryYang 20160511 JAM0304->JAM0315,將IC及Clean pad相關的alarm code分開

            if(iRet==K_SKIP)                                                    //kevin 20150624 add
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(bIsSuckICFallDown[i][j])                             //kevin 20150624
                            BTestSuck.SetItemData(i, j, HAS_NULL_CLEAN_IC);     // 掉料忽視
                    }
                }
            }
            Task=3500;
            break;
        case 3500:
            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                             //JimmyChiu 20220708 : add Index Arm Axis
            {
                flag=MOT[MTestY1].Gali_MotMove(Prod.TestY1_Middle, iSpeedSY, "DoIndexAutoClean_3500");   //kevin 20120517 iSpeed))
            }
            else
            {
                flag=MOT[MTestY2].Gali_MotMove(Prod.TestY2_Rear, iSpeedSY, "DoIndexAutoClean_3500");     //kevin 20120517 iSpeed))  //Isaac 20200203 : iSpeedSY,客戶要求AutoCleanIndex速度要一樣
            }

            if(flag)
            {
                if(TestIF_File.iAutoClean_Tray!=eCKPos_CleanAir)
                    Task=3600;
                else
                    Task=3800;
            }
            break;
        case 3600:
            if(MOT[MTestZ2].Gali_MotMove(Prod.TestZ2_Place+TestIF_File.iAutoClean_IndexReleaseOffset, iSpeed, "DoIndexAutoClean_3600"))  //Jou 2015-08-22 Auto clean Index Release Offset
            {
                Task=3700;
            }
            break;
        case 3700:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(BTestSuck.Item[i][j])
                    {
                        if(BTestSuck.Item[i][j]==HAS_NULL_CLEAN_IC ||
                           BTestSuck.Suck[i][j].Destroy())
                        {
                            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)    //Clean Kit
                                BLCarryKit.MoveSuckData(BTestSuck, i, j);
                        }
                    }
                }
            }
            flag=false;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(BTestSuck.Item[i][j] &&
                       BTestSuck.Suck[i][j].Error==false)
                    {
                        flag=true;
                        break;
                    }
                }
            }

            if(flag==false)
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(BTestSuck.Suck[i][j].Error)
                        {
                            Task=3750;
                            return;
                        }
                    }
                }
            }

            if(BTestSuck.UseSiteHasIC())
                break;

            for(int i=0; i<MAX_Index_Row; i++)
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                    bDuplicateErr[i][j]=false;
            Task=3800;
            break;
        case 3750:                                                              //破壞異常處理
            if(DoInArmMoveToWaitPosByAutoClean()==false)                        //ChungHung 20150129 add when Index Jam SCK want to Inarm move to safe postion
                return;

            ErrPart=" ";
            bHasErr=false;
            bHasDuplicateErr=false;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(bDuplicateErr[i][j])
                        bHasDuplicateErr=true;

                    if(BTestSuck.Suck[i][j].Error)
                    {
                        bHasErr=true;
                        ErrPart+=IndexSuckName[i][j];
                        BTestSuck.Suck[i][j].Error=false;                       //Steven 20101229 : 換位置
                        bDuplicateErr[i][j]=true;                               //Steven 20101229 : 換位置
                    }
                    else
                    {
                        bDuplicateErr[i][j]=false;
                    }
                }
            }

            if(bHasErr)
                ShowErrorMessage("JAM0327", K_RETRY, MTestZ2, bHasDuplicateErr, ErrPart);   //Vacuum sensor OFF error

            bHasErr=false;
            Task=3700;
            break;
        case 3800:
            if(MOT[MTestZ2].Gali_Two_ZAxis_Move(Prod.TestZ2_Safe, iSpeed, "DoIndexAutoClean 3800"))
            {
                if((iInArmType==e9045_1x3_4 ||                                  //Steven 20220922 : Fixed for 1x3 auto clean
                    iInArmType==e9045_1x3_2_14) &&
                   TestIF_File.iAutoClean_DeveicePices%3==0)
                {
                    BLCarryKit.SetItemData(0, 3, NULL_IC);
                }
                Task=3900;
            }
            break;
        case 3900:
            MOT[MInShuttle2].fCanMoveM=true;
            bInedxCleanFinish[1]=true;                                          //kevin 20170520 (wei) Autoclean index Arm 2做完
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanAir)
            {
                bRunAutoClean=false;
                bAutoCleanFinishOnlyUseRTC=true;                                //JerryYang 20161216 (Steven) 做完auto clean後只靠RTC檢查Socket,不做index check
                iAutoClean_IndexContactCount=0;
                CleanSetSpeed(false);
            }
            break;
    }

    #ifdef DEBUG_AUTO_CLEAN
    static int iOldTask=-1;
    if(iOldTask!=Task)
    {
        Message.sprintf("DoIndexAutoClean, %d, Go to Task, %d", iOldTask, Task);
        fMain->AddAutoCleanMessage(Message);
        iOldTask=Task;
    }
    #endif
}
//------------------------------------------------------------------------------
bool Special_2X6_Tray_XItem7()                                                  //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
{                                                                               //使用 Auto Clean Tray 模式，Tray 的 XItem 為 7，最右邊第 8 格的部分不能放 IC，只能塞用假資料
    if(iInArmType==e9045_2x6_8                  &&
       TestIF_File.bAutoClean_UseTray           &&
       TrayForm.Loader.XDivision==7             &&
       TestIF_File.iAutoClean_XDivision_Tray==8 &&
       TestIF_File.iAutoClean_YDivision_Tray==2 &&
       LastSet.bUseTestSocket[0][0][5]==true    &&
       LastSet.bUseTestSocket[0][1][5]==true)
    {
        return true;

    }
    return false;
}
//------------------------------------------------------------------------------
void GetInarmSuckRow(int iShtRowKit,int &isuckRow,int &ikitStep)
{
    if(iShtRowKit==1 || iShtRowKit==2)                                          //JerryYang 20160411 當X-Pitch > 40mm,要分成4次放到shuttle
    {
        ikitStep=0;
        isuckRow=iShtRowKit-1;
    }
    else
    {
        ikitStep=4;
        isuckRow=iShtRowKit-3;
    }
    if(isuckRow<0)
        isuckRow=0;
}
//------------------------------------------------------------------------------

