#include "MachineDefine.h"
#pragma hdrstop

#include "cTemperFrom.h"

#include "cMyDef.h"
#include "main.h"
#include "bthermo.h"
#include "HandlerSys.h"
#include "note.h"
#include "database.h"
#include "cMyDB.h"
#include "myQwertyKeyBoard.h"
//#include "INPUT.h"
#include "Password.h"
#include "mymessbox.h"
#include "mysensor.h"
#include "cprod.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20151230 :add New ATC Interface
#include "uLotInfo.h"
#include "iosetview.h"
#include "ATCInterface.h"                                                       //2014-05-30    Dell    for ATC6.0
#include "uTemp_Set.h"
#include "common.h"
#include "OCR.h"
#include "Congex.h"
#include "AutoTeach.h"
#include "database.h"                                                           //Ztex 2023.04.19 Add HT-1032 TriTemp Function
#include "TesterTCP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTemperFrom *fTemperFrom;
bool bGreen=false,bYellow=false;
TPanel *ShowTempComp[tcTotalCount];
TPanel *NameTempComp[tcTotalCount];

//---------------------------------------------------------------------------
__fastcall TfTemperFrom::TfTemperFrom(TComponent* Owner)
    : TForm(Owner)
{
    TPanel *tem1Ptr[]=                                                          //溫控器要一起改
    {
        hlNamePlate1, hlNamePlate2, hlNameShuttle1, hlNameShuttle2, hlNameHead12,
        hlNameHead34, hlNameHead56, hlNameHead78, hlNameDut, hlNameChamber, hlNameCCD,
        hlNameAa1, hlNameAb1, hlNameAc1, hlNameAd1,
        hlNameBa1, hlNameBb1, hlNameBc1, hlNameBd1,
        hlNameAa2, hlNameAb2, hlNameAc2, hlNameAd2,
        hlNameBa2, hlNameBb2, hlNameBc2, hlNameBd2,
        hlNameHeatGun1, hlNameHeatGun2,                                         //kevin 20120523 add
        hlNameDut_A1, hlNameDut_A2, hlNameDut_A3, hlNameDut_A4,                 //2013-01-15    Dell DUT增加為4顆
        hlNameAe1, hlNameAf1, hlNameAg1, hlNameAh1,                             //Steven 20140923 : Index使用EJ1N版32組加熱器
        hlNameBe1, hlNameBf1, hlNameBg1, hlNameBh1,
        hlNameAe2, hlNameAf2, hlNameAg2, hlNameAh2,
        hlNameBe2, hlNameBf2, hlNameBg2, hlNameBh2,
        hlName2D_2, hlNameLB, hlNameESD, hlNameCCD_2_2,
        hlNameATCHeatGun1_2, hlNameATCHeatGun2_2,                               //JerryYang 20220408 : add for ATC3.5
        hlNameOutShuttle1, hlNameOutShuttle2,                                   //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlNameBase1, hlNameBase2, hlNameBase3,                                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlNameBase4, hlNameBase5, hlNameBase6,                                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlNamePlate_3, hlNamePlate_4,                                           //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlNameShuttle_3, hlNameShuttle_4,                                       //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlNameDoor1, hlNameDoor2,                                               //Ztex 2023.10.23 Add Index Door Heater
        hlNameLBUp, hlNameLBDown                                                //Frank 20241231 : add
    };

    TPanel *temPtr[]=                                                           //溫控器要一起改
    {
        hlTempPlate1, hlTempPlate2, hlTempShuttle1, hlTempShuttle2, hlTempHead12,
        hlTempHead34, hlTempHead56, hlTempHead78, hlTempDut, hlTempChamber, hlTempCCD,
        hlTempAa1, hlTempAb1, hlTempAc1, hlTempAd1,
        hlTempBa1, hlTempBb1, hlTempBc1, hlTempBd1,
        hlTempAa2, hlTempAb2, hlTempAc2, hlTempAd2,
        hlTempBa2, hlTempBb2, hlTempBc2, hlTempBd2,
        hlTempHeatGun1, hlTempHeatGun2,                                         //kevin 20120523 add
        hlTempDut_A1 ,hlTempDut_A2 ,hlTempDut_A3 ,hlTempDut_A4,                 //2013-01-15    Dell DUT增加為4顆
        hlTempAe1, hlTempAf1, hlTempAg1, hlTempAh1,                             //Steven 20140923 : Index使用EJ1N版32組加熱器
        hlTempBe1, hlTempBf1, hlTempBg1, hlTempBh1,
        hlTempAe2, hlTempAf2, hlTempAg2, hlTempAh2,
        hlTempBe2, hlTempBf2, hlTempBg2, hlTempBh2,
        hlTemp2D_2, hlTempLB, hlTempESD, hlTempCCD_2_2,                         //Steven 20210625 : 多了一個hlNameCCD
        hlTempATCHeatGun1_2, hlTempATCHeatGun2_2,                               //JerryYang 20220408 : add for ATC3.5
        hlTempOutShuttle1, hlTempOutShuttle2,                                   //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlTempBase1, hlTempBase2, hlTempBase3,                                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlTempBase4, hlTempBase5, hlTempBase6,                                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlTempPlate_3, hlTempPlate_4,                                           //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlTempShuttle_3, hlTempShuttle_4,                                       //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        hlTempDoor1, hlTempDoor2,                                               //Ztex 2023.10.23 Add Index Door Heater
        hlTempLBUp, hlTempLBDown                                                //Frank 20241231 : add
    };

    for(int i=tcHotPlate1; i<tcTotalCount; i++)
    {
        ShowTempComp[i]=temPtr[i];
        NameTempComp[i]=tem1Ptr[i];
    }

    AnsiString tempYieldName[]=
    {
        "Error Recovery",    "Double Device", "Continue Good Bin", "Yield Monitoring", "Consecutive Alarm",
        "Index Start Error", "OCR",           "QA Mode",           "",                 ""
    };
    TPanel *tem2Ptr[]=
    {
        palErrorRecovery, palDoubleDevice, palCGoodBin, palYieldMonitor, palConsAlarm,
        palIndexStartErr, palOcrOn, pnlQAMode, palTest1, palTest2
    };

    for(int i=esytErrorRecovery; i<SHOW_YIELD_TOTAL; i++)
    {
        strShowYield[i].UsePanel            =tem2Ptr[i];
        strShowYield[i].asName              =tempYieldName[i];
        strShowYield[i].UsePanel->Caption   ="";
        strShowYield[i].UsePanel->Color     =clBtnFace;
        strShowYield[i].bShow=false;
    }

    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //Steven 20250423 : Add auto temp offset display
        strShowYield[esytOEE].asName="Auto Temp Offset";

    bShow=false;
    AutoSize=false;
    bStartRecord=false;
    iFormWidth=728;
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::FormShow(TObject *Sender)
{
    bShow=true;
//    int iLeft=4;

    pnl32Heater->Visible=false;                                                 //Steven 20250701 : 調整溫度顯示畫面
    if(CUSTOMER_CODE==CC_HONPREC_QC)
    {
        Button6->Visible=false;
        Button1->Visible=true;
        Button6->Top=4;
        Button1->Top=4;
        Memo1->Top=4;
        Memo1->Visible=true;
    }
    #ifdef SOFT_SIMULTE
        Button6->Top=4;
        Button1->Top=4;
        Memo1->Top=4;
        Button1->Visible=false;
        Button6->Visible=true;
        Memo1->Visible=false;
    #endif

    if(IniConfig.bShowFunctionWindow)
    {
        strShowYield[esytErrorRecovery].bShow   =true;                          //Error Recovery On
        strShowYield[esytErrorRecovery].OnOff   =true;
        strShowYield[esytDoubleDevice ].bShow   =true;                          //Double Device On
        strShowYield[esytCGoodBin     ].bShow   =true;                          //Continue Good Bin On
        strShowYield[esytYieldMonitor ].bShow   =true;                          //Yield Monitoring On
        strShowYield[esytConsAlarm    ].bShow   =true;                          //Consecutive Alarm On
        strShowYield[esytIndexStartErr].bShow   =true;                          //Index Start Error On
        strShowYield[esytIndexStartErr].OnOff   =true;
        strShowYield[esytOcrOn        ].bShow   =(INSTALL_OCR!=eocrUninstal);   //OCR On
        strShowYield[esytOcrOn        ].UsePanel->Caption=(INSTALL_OCR!=eocrUninstal)?"OCR":"";
        if(CUSTOMER_CODE==CC_GM_TEST)
            strShowYield[esytQAMode   ].bShow   =true;                          //Steven 20220523 : GM Test顯示site map check功能有沒有開
        else
            strShowYield[esytQAMode   ].bShow   =IniConfig.bQAMode;             //20141204 ChungHung add for ATK QA mode
        strShowYield[esytOEE          ].bShow  =(CUSTOMER_CODE==CC_SCK ||       //Steven 20220407 : JSCK OEE Function
                                                 CUSTOMER_CODE==CC_AMKOR_Korea);//Steven 20250423 : Add auto temp offset display
        strShowYield[esytTest2].bShow   =IniConfig.bFTContinueON;               //kevin 20121008 RT
        strShowYield[esytTest2].OnOff   =IniConfig.bFTContinueON;
        ShowYieldFuntion();
    }

    if(CUSTOMER_CODE==CC_GM_TEST)                                               //Steven 20220523 : GM Test顯示site map check功能有沒有開
    {
        strShowYield[esytQAMode].asName="Site Map Check";
        strShowYield[esytQAMode].UsePanel->Caption="Site Map Check";
    }

    if(REAL_TIME_CCD==true)                                                     //JimmyChiu 20250603 : 修正CCD溫度的畫面顯示
    {
        pnl4CCD->Visible        =false;
        grpCCD->Visible         =false;
        NameTempComp[tcCCD_2]   =hlNameCCD_2_2;
        ShowTempComp[tcCCD_2]   =hlTempCCD_2_2;

        if(RTC_TemperNumber<2)                                                  //Steven 20250701 : 調整溫度顯示畫面
        {
            NameTempComp[tcCCD_2]->Visible=false;
            ShowTempComp[tcCCD_2]->Visible=false;
        }

        if(USE_16_HEATER==eht4Heater)
        {
            if(iSocketBaseTempCount==eDut4ea ||
               iSocketBaseTempCount==eDut2ea ||
               RTC_TemperNumber==2)
            {
                NameTempComp[tcCCD]         =hlNameCCD_3;
                ShowTempComp[tcCCD]         =hlTempCCD_3;
                grpCCD->Visible             =true;
                grpCCD->Parent              =gbOldTempWindow;
                grpCCD->Left                =572;
            }
            else
            {
                NameTempComp[tcCCD]         =hlNameCCD;
                ShowTempComp[tcCCD]         =hlTempCCD;
                pnl4CCD->Visible            =true;
            }
        }
        else
        {
            NameTempComp[tcCCD]         =hlNameCCD_3;
            ShowTempComp[tcCCD]         =hlTempCCD_3;
            grpCCD->Visible             =true;
        }
    }
    else
    {
        pnl4CCD->Visible    =false;
        grpCCD->Visible     =false;
    }

    if(USE_16_HEATER==eht16Heater       ||                                      //Steven 20120606 : 16溫控器 8Site使用Hontech頭
       USE_16_HEATER==eht16HeaterEJ1N   ||
       USE_16_HEATER==eht32HeaterEJ1N   ||                                      //Steven 20140923 : Index使用EJ1N版32組加熱器
       USE_16_HEATER==eht32HeaterKT4H   ||                                      //Steven 20150211 : Index使用KT4H版32組加熱器
       USE_16_HEATER==eht16HeaterDTME08 ||                                      //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
       USE_16_HEATER==eht32HeaterDTME08 )                                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
    {
        ShowTempComp[tcHotPlate1]   =hlTempPlate1_2;
        ShowTempComp[tcHotPlate2]   =hlTempPlate2_2;
        ShowTempComp[tcShuttle1]    =hlTempShuttle1_2;
        ShowTempComp[tcShuttle2]    =hlTempShuttle2_2;
        ShowTempComp[tcChamber]     =hlTempChamber_2;
        ShowTempComp[tcSocket]      =hlTempDut_2;
        ShowTempComp[tcHeatGun1]    =hlTempHeatGun1_2;                          //Steven 20120712 : 修正
        ShowTempComp[tcHeatGun2]    =hlTempHeatGun2_2;                          //Steven 20120712 : 修正
        ShowTempComp[tcATCHotAir1]  =hlTempATCHeatGun1_2;                       //JerryYang 20220408 : add for ATC3.5
        ShowTempComp[tcATCHotAir2]  =hlTempATCHeatGun2_2;
        ShowTempComp[tcLB]          =hlTempLB;

        ShowTempComp[tcLBUp]        =hlTempLBUp;                                //Frank 20241231 : add
        ShowTempComp[tcLBDown]      =hlTempLBDown;

        if(CCD2_TEMPER==true)                                                   //Ifor 20190503 : CCD TEMP 改由設定值判斷開關
            ShowTempComp[tc2D]      =hlTemp2D_2;                                //wei 20160524 2D溫度
        if(iSocketBaseTempCount==eDut4ea || iSocketBaseTempCount==eDut2ea)      //Steven 20131226 : 修正畫面顯示
        {
            ShowTempComp[tcDUT1]    =hlTempDut_A1;
            ShowTempComp[tcDUT2]    =hlTempDut_A2;
            ShowTempComp[tcDUT3]    =hlTempDut_A3;                              //Steven 20120712 : 修正
            ShowTempComp[tcDUT4]    =hlTempDut_A4;                              //Steven 20120712 : 修正
            ShowTempComp[tcChamber] =hlTempChamber_3;
            grpIndex->Visible       =false;
            gb_Index->Visible       =true;
            grpChamber->Visible     =(ATC_SYSTEM==eATCUninstall);               //Steven 20240605 : 修正Chamber溫度顯示
            pnl32Heater->Visible    =true;                                      //Steven 20250701 : 調整溫度顯示畫面
        }
        else
        {
            grpIndex->Visible       =true;
            gb_Index->Visible       =false;
            grpChamber->Visible     =false;
            ShowTempComp[tcDUT1]    =hlTempDut;                                 //kevin 20200110 dut 1 ea sensor
            NameTempComp[tcDUT1]    =hlNameDut;
            if(MachineTypeChoice==Type_HT9046)
                NameTempComp[tcSocket]      =hlNameDut_2;                       //kevin 20200130 change HT9046 只有一個DUT顯示
        }
        NameTempComp[tcHotPlate1]   =hlNamePlate1_2;
        NameTempComp[tcHotPlate2]   =hlNamePlate2_2;
        NameTempComp[tcShuttle1]    =hlNameShuttle1_2;
        NameTempComp[tcShuttle2]    =hlNameShuttle2_2;
        NameTempComp[tcChamber]     =hlNameChamber_2;
        NameTempComp[tcSocket]      =hlNameDut_2;
        NameTempComp[tcHeatGun1]    =hlNameHeatGun1_2;
        NameTempComp[tcHeatGun2]    =hlNameHeatGun2_2;

        if(CCD2_TEMPER==true)                                                   //Ifor 20190503 : CCD TEMP 改由設定值判斷開關
            NameTempComp[tc2D]          =hlTemp2D_2;                            //wei 20160524 2D溫度

        if(iSocketBaseTempCount==eDut4ea || iSocketBaseTempCount==eDut2ea)      //Steven 20131226 : 修正畫面顯示
        {
            NameTempComp[tcDUT1]        =hlNameDut_A1;
            NameTempComp[tcDUT2]        =hlNameDut_A2;
            NameTempComp[tcDUT3]        =hlNameDut_A3;
            NameTempComp[tcDUT4]        =hlNameDut_A4;
            NameTempComp[tcChamber]     =hlNameChamber_3;

        }

        gbHeatGun->Visible  =(INSTALL_HEAT_GUN>0);                              //Steven 20120525 : 熱風槍
        gbATCHeatGun->Visible  =(INSTALL_ATC_HEAT_GUN>0);                       //JerryYang 20220408 : add for ATC3.5
    }
    else if(USE_16_HEATER==eht4Heater)
    {
        if(iSocketBaseTempCount==eDut4ea || iSocketBaseTempCount==eDut2ea)      //JerryYang 20160129 考慮使用4組加熱器,Dut2組以上的情況,要正常顯示
        {
            NameTempComp[tcDUT1]        =hlNameDut_A1;
            NameTempComp[tcDUT2]        =hlNameDut_A2;
            NameTempComp[tcDUT3]        =hlNameDut_A3;
            NameTempComp[tcDUT4]        =hlNameDut_A4;
            NameTempComp[tcChamber]     =hlNameChamber_3;
            NameTempComp[tcHeatGun1]    =hlNameHeatGun1_2;
            NameTempComp[tcHeatGun2]    =hlNameHeatGun2_2;
            ShowTempComp[tcDUT1]        =hlTempDut_A1;
            ShowTempComp[tcDUT2]        =hlTempDut_A2;
            ShowTempComp[tcDUT3]        =hlTempDut_A3;
            ShowTempComp[tcDUT4]        =hlTempDut_A4;
            ShowTempComp[tcChamber]     =hlTempChamber_3;
            ShowTempComp[tcHeatGun1]    =hlTempHeatGun1_2;
            ShowTempComp[tcHeatGun2]    =hlTempHeatGun2_2;

            palNewTempWindow->Visible=false;
            gbOldTempWindow->Height=69;

            if(ATC_SYSTEM==eATCUninstall)                                       //Steven 20240605 : 修正Chamber溫度顯示
            {
                grpChamber->Parent =gbOldTempWindow;
                grpChamber->Left   =pnl4Head7->Left+pnl4Head7->Width+1;
            }
            gb_Index->Parent=gbOldTempWindow;

            if(ATC_SYSTEM==eATCUninstall)
            {
                gb_Index->Left=grpChamber->Left+grpChamber->Width+1;
            }
            else
            {
                gb_Index->Left=pnl4Head7->Left+pnl4Head7->Width+1;
            }

            if(INSTALL_HEAT_GUN!=0)
            {
                gbHeatGun->Parent=gbOldTempWindow;
                gbHeatGun->Left=grpIndex->Left+grpIndex->Width;
            }

            if(INSTALL_ATC_HEAT_GUN!=0)                                         //JerryYang 20220408 : add for ATC3.5
            {
                gbATCHeatGun->Parent=gbOldTempWindow;
                gbATCHeatGun->Left=grpIndex->Left+grpIndex->Width+10;
            }
            gbPlate->Visible    =false;
            gbShuttle->Visible  =false;
            gbArm1->Visible     =false;
            gbArm2->Visible     =false;
            grpIndex->Visible   =false;
            gb_Index->Visible   =true;
            gbArm1_2->Visible   =false;
            gbArm2_2->Visible   =false;
            pnl4Dut->Visible    =false;
            pnl4Chamber->Visible=false;
            grpChamber->Visible =(ATC_SYSTEM==eATCUninstall);                   //Steven 20240605 : 修正Chamber溫度顯示
            pnl4HotGun1->Visible=false;
            pnl4HotGun2->Visible=false;
        }
        else
        {
            pnl4HotGun1->Visible        =(INSTALL_HEAT_GUN>0);                  //Steven 20120525 : 熱風槍
            pnl4HotGun2->Visible        =(INSTALL_HEAT_GUN>0);
            pnl4Chamber->Visible        =(ATC_SYSTEM==eATCUninstall);           //Steven 20240605 : 修正Chamber溫度顯示

            NameTempComp[tcHeatGun1]    =hlNameHeatGun1;
            NameTempComp[tcHeatGun2]    =hlNameHeatGun2;
            ShowTempComp[tcHeatGun1]    =hlTempHeatGun1;
            ShowTempComp[tcHeatGun2]    =hlTempHeatGun2;

            grpIndex->Visible  =true;
            gb_Index->Visible  =false;
        }
    }

    if(Tri_Temp_Machine==1)                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        gbFunctionWindow->Visible   =true;
        gbOldTempWindow ->Visible   =false;
        gbArm1->Visible             =true;
        gbArm2->Visible             =true;
        gbArm1_2->Visible           =true;
        gbArm2_2->Visible           =true;
        grpLB->Visible              =false;
        grp2DID->Visible            =false;
        grpIndex->Visible           =false;
        gbHeatGun->Visible          =false;
        gbATCHeatGun->Visible       =true;
        grpOutSht->Visible          =true;
        grpBase->Visible            =true;
        grp_HopPlate2->Visible      =true;
        grp_Shuttle2->Visible       =true;
        grpChamber->Visible         =false;
        grpCCD->Visible             =false;
        gbATCHeatGun->Visible       =true;
        gbATCHeatGun->Parent        =pnl32Heater;
        gbATCHeatGun->Caption       ="Air Stream";
        gbPlate->Caption            ="HP 1";
        gbShuttle->Caption          ="Shuttle 1";
    }
    else
    {
        grpOutSht->Visible           =false;
        grpBase->Visible             =false;
        gbFunctionWindow->Visible = IniConfig.bShowFunctionWindow;
        gbOldTempWindow ->Visible = (USE_16_HEATER==eht4Heater);
        palNewTempWindow->Visible =(USE_16_HEATER==eht16Heater       ||         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                                    USE_16_HEATER==eht16HeaterEJ1N   ||
                                    USE_16_HEATER==eht32HeaterEJ1N   ||         //Steven 20140923 : Index使用EJ1N版32組加熱器
                                    USE_16_HEATER==eht32HeaterKT4H   ||         //Steven 20150211 : Index使用KT4H版32組加熱器
                                    USE_16_HEATER==eht16HeaterDTME08 ||         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                                    USE_16_HEATER==eht32HeaterDTME08 );         //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
        gbArm1->Visible =(USE_16_HEATER==eht16Heater       ||                   //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                          USE_16_HEATER==eht16HeaterEJ1N   ||
                          USE_16_HEATER==eht32HeaterEJ1N   ||                   //Steven 20140923 : Index使用EJ1N版32組加熱器
                          USE_16_HEATER==eht32HeaterKT4H   ||                   //Steven 20150211 : Index使用KT4H版32組加熱器
                          USE_16_HEATER==eht16HeaterDTME08 ||                   //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                          USE_16_HEATER==eht32HeaterDTME08 );                   //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
        gbArm2->Visible =(USE_16_HEATER==eht16Heater       ||                   //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                          USE_16_HEATER==eht16HeaterEJ1N   ||
                          USE_16_HEATER==eht32HeaterEJ1N   ||                   //Steven 20140923 : Index使用EJ1N版32組加熱器
                          USE_16_HEATER==eht32HeaterKT4H   ||                   //Steven 20150211 : Index使用KT4H版32組加熱器
                          USE_16_HEATER==eht16HeaterDTME08 ||                   //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                          USE_16_HEATER==eht32HeaterDTME08 );                   //JimmyChiu 20210923 : Index使用DTME08版32組加熱器

        gbArm1_2->Visible=(USE_16_HEATER==eht32HeaterEJ1N ||                    //Steven 20140923 : Index使用EJ1N版32組加熱器
                           USE_16_HEATER==eht32HeaterKT4H ||                    //Steven 20150211 : Index使用KT4H版32組加熱器
                           USE_16_HEATER==eht32HeaterDTME08 ||                  //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                           (ATC_SYSTEM==eNewATCSystem &&
                            iATC_Use_Heat_Count>=16)                            //Ifor 20170814 (wei) Add HT9045AT 溫度顯示
                           );

        gbArm2_2->Visible=(USE_16_HEATER==eht32HeaterEJ1N ||                    //Steven 20140923 : Index使用EJ1N版32組加熱器
                           USE_16_HEATER==eht32HeaterKT4H ||                    //Steven 20150211 : Index使用KT4H版32組加熱器
                           USE_16_HEATER==eht32HeaterDTME08 ||                  //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                           (ATC_SYSTEM==eNewATCSystem &&
                            iATC_Use_Heat_Count>=16)                            //Ifor 20170814 (wei) Add HT9045AT 溫度顯示
                           );

        grpLB->Visible=LB_TEMP;                                                 //Steven 20181023 : LB溫度
        grpLB1->Visible=LB_TEMP_UpDown;                                         //Frank 20241231 : add
    }

    if((iSocketBaseTempCount==eDut4ea || iSocketBaseTempCount==eDut2ea) &&      //Steven 20131008 : For HT9046AH
       palNewTempWindow->Visible)
    {
        grpIndex->Visible=false;
        gb_Index->Visible=true;
    }

    if(CCD2_TEMPER==true)                                                       //Ifor 20190503 : CCD TEMP 改由設定值判斷開關
    {
        grp2DID->Visible=true;
        if(CCD2_TEMPER==true)
        {
            grp2DID->Caption="CCD2";
            hlName2D_2->Caption="CCD2";
            NameTempComp[tc2D]=hlName2D_2;
        }
    }
    else
    {
        grp2DID->Visible=false;
    }

    if(Index_ESDAir)                                                            //kevin 20200207 add index ESD temp
    {
        grpESD->Visible=true;
        hlNameESD->Caption="ESD";
        NameTempComp[tcIndexESD]=hlNameESD;
    }
    else
    {
        grpESD->Visible=false;
    }

    if(iSocketBaseTempCount==eDut2ea)                                           //Steven 20140116 : Socket數量,改用數字設定
    {
        hlNameDut_A3->Visible=false;
        hlNameDut_A4->Visible=false;
        hlTempDut_A3->Visible=false;
        hlTempDut_A4->Visible=false;
    }
    else if(MachineTypeChoice==Type_HT9046 && iSocketBaseTempCount==eDut1ea)    //kevin 20200130 add 1 dut display
    {
        grpIndex->Visible   =true;
        gb_Index->Visible   =false;
        hlTempDut_2->Visible=true;
        hlNameDut_2->Visible=true;
    }

    grpDoor->Visible=(INDEXDOORHEATER==1);                                      //Ztex 2023.10.23 Add Index Door Heater

    if(USE_16_HEATER==eht32HeaterEJ1N   ||                                      //Steven 20140923 : Index使用EJ1N版32組加熱器
       USE_16_HEATER==eht32HeaterKT4H   ||                                      //Steven 20150211 : Index使用KT4H版32組加熱器
       USE_16_HEATER==eht32HeaterDTME08 ||                                      //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
       grpChamber->Visible==true        ||
       gb_Index->Visible==true          ||
       grpBase->Visible==true           ||
       grp2DID->Visible==true           ||
       grpLB->Visible==true             ||
       grpLB1->Visible==true            ||
       grpESD->Visible==true)
    {                                                                           //Steven 20250701 : 調整溫度顯示畫面
        pnl32Heater->Visible=true;
    }

    ChangeFormSize();
    ArrangeFormWidth();                                                         //Steven 20231111 : 重新排列TempForm
    Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::ArrangeFormWidth()                                //Steven 20231111 : 重新排列TempForm
{
    TControl *ChildControl;
    int iWidth=0;
    int iFuncWidth=0;
    int i4HeatWidth=0;
    int i16HeatWidth=0;
    int i32HeatWidth=0;

    if(gbFunctionWindow->Visible)
    {
        iFuncWidth=740;
        iWidth=iFuncWidth;
    }

    if(gbOldTempWindow->Visible)
    {
        for(int i=0; i<gbOldTempWindow->ControlCount; i++)
        {
            ChildControl=gbOldTempWindow->Controls[i];
            if(ChildControl->Visible==true)
            {
                i4HeatWidth+=ChildControl->Width;
            }
        }
        i4HeatWidth+=10;

        if(i4HeatWidth>iWidth)
            iWidth=i4HeatWidth;
    }

    if(INDEXDOORHEATER==1)                                                      //Ztex 2023.10.23 Add Index Door Heater
        grpDoor->Visible         = true;
    else
        grpDoor->Visible         = false;

    if(palNewTempWindow->Visible)
    {
        for(int i=0; i<pnl16Heater->ControlCount; i++)
        {
            ChildControl=pnl16Heater->Controls[i];
            if(ChildControl->Visible==true)
            {
                i16HeatWidth+=ChildControl->Width;
            }
        }
        i16HeatWidth+=10;
        if(i16HeatWidth>iWidth)
            iWidth=i16HeatWidth;

        for(int i=0; i<pnl32Heater->ControlCount; i++)
        {
            ChildControl=pnl32Heater->Controls[i];
            if(ChildControl->Visible==true)
            {
                i32HeatWidth+=ChildControl->Width;
            }
        }
        i32HeatWidth+=10;
        if(i32HeatWidth>iWidth)
            iWidth=i32HeatWidth;
    }

    fTemperFrom->Width=iWidth+25;
    pnlLeft->Width=iWidth;
    palLed->Parent=fTemperFrom;
    palLed->Align=alRight;
    palLed->BringToFront();
}
//******************************************************************************
//
//  注意!! ShowThermo為Handler 溫度控制相關, 修改時要小心!!
//
//******************************************************************************
bool __fastcall TfTemperFrom::ShowThermo(int Addr)
{
    const double CCDTempLimit =45.0;

    static bool bTemperatureReady[tcTotalCount]={true};
    static bool iBelowAlmSec[tcTotalCount], iOverAlarmSec[tcTotalCount];
    static int  iSec=0;

    AnsiString S1="";
    AnsiString str;
    double dTemptureRangeOver,dTemptureRangeBelow;
    double SetTemp;
    bool   bNow_Is_ATC=false;                                                   //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    bool   bOverTemp=false;                                                     //Steven 20110705
    int    iChannel=0;
    int SITE_Ch[14]={tcOutSht1, tcOutSht2, tcDUT1, tcDUT2, tcDUT3, tcDUT4,
                     tcBase1, tcBase2, tcBase3, tcBase4, tcBase5, tcBase6,      //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                     tcDoor1,tcDoor2                                     };     //Ztex 2023.10.23 Add Index Door Heater

    bool bAirStream_Check=false;                                                //Ztex 2023.04.19 Add HT-1032 TriTemp Function

    if(Addr>=sizeof(ShowTempComp)/4)
        return false;

    if(Tri_Temp_Machine==1)                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        if(LastSet.iTemperature==Tempture_Ambient)
        {
            ShowTempComp[Addr]->Caption="---";
            ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
        }
        else
        {
            bAirStream_Check=false;
            if((Addr>=tcAa1 && Addr<=tcBd2) ||                                  //判斷ATC、Heater溫度區間及顯示元件顏色
               (Addr>=tcAe1 && Addr<=tcBh2) ||
               (Addr>=tcHotPlate1 && Addr<=tcShuttle2) ||
               (Addr>=tcHotPlate3 && Addr<=tcShuttle4))
            {
                SetTemp             =Temperature.fWorkTemperBase;
                dTemptureRangeOver  =IniConfig.iL36_1Tri_Temp_Rang_ATC;
                dTemptureRangeBelow =IniConfig.iL36_1Tri_Temp_Rang_ATC;
                bNow_Is_ATC=true;
            }
            else if(Addr==tcATCHotAir1 || Addr==tcATCHotAir2)
            {
                if(Addr==tcATCHotAir1)
                {
                    SetTemp             =Temperature.dSetIndexAirstreamTemp;
                    bAirStream_Check    =!Temperature.bEnableArm_1_Air && !Temperature.bEnableArm_2_Air;
                }
                else
                {
                    SetTemp             =Temperature.fSetTempature2AirMachine;
                    bAirStream_Check    =!Temperature.bEnableSocket_Air;
                }

                dTemptureRangeOver      =Temperature.SetAirstreamTemperatureRang_Index;
                dTemptureRangeBelow     =Temperature.SetAirstreamTemperatureRang_Index;
            }
            else
            {
                for(int i=0; i<14; i++)
                {
                    if(Addr==SITE_Ch[i])
                    {
                        SetTemp=Temperature.fIndividualTemp[SITE_Ch[i]];
                        break;
                    }
                }

                dTemptureRangeOver  =IniConfig.iL36_2Tri_Temp_Rang_Heater;
                dTemptureRangeBelow =IniConfig.iL36_2Tri_Temp_Rang_Heater;
            }

            if(bUT150Install[Addr]==true && bUT150HasUse[Addr]==true && bAirStream_Check==false)
            {
                #ifdef SOFT_SIMULTE
                AnsiString str1;
                str1.sprintf("%d", Addr);
                if(IniConfig.bL29AmbientNotShowTemp &&
                   LastSet.iTemperature==Tempture_Ambient)                      //Sam 20221101 : 常溫模式不顯示溫度
                    ShowTempComp[Addr]->Caption="---";
                else
                    ShowTempComp[Addr]->Caption=str1;
                asGPIBTempShow[Addr]=str1;                                      //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND
                if(bNow_Is_ATC==false)
                    UN150Read[Addr]=Addr;
                #else
    //            ShowTempComp[Addr]->Caption="...";
    //            asGPIBTempShow[Addr]="NULL";                                  //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND
                #endif

                str.sprintf("%5.1f", UN150Read[Addr]);

                if(IniConfig.bL29AmbientNotShowTemp &&
                   LastSet.iTemperature==Tempture_Ambient)                      //Sam 20221101 : 常溫模式不顯示溫度
                    ShowTempComp[Addr]->Caption="---";
                else
                    ShowTempComp[Addr]->Caption=str;

                if(Temperature.fWorkTemperBase<=0 && bNow_Is_ATC==true && UN150Read[Addr]<=0)
                {
                    if(UN150Read[Addr]-(SetTemp)>(dTemptureRangeOver))          // OVER
                    {
                        ShowTempComp[Addr]->Color=(TColor)0x008000FF;
                        bUT150State[Addr]=false;
                    }
                    else if(UN150Read[Addr]-(SetTemp)<((-1)*dTemptureRangeBelow))// Below
                    {
                        if(LastSet.iTemperature==Tempture_Hot ||
                           LastSet.iTemperature==Tempture_AmbientHot ||         //kevin 20140918 恆溫控制
                           (LastSet.iTemperature==Tempture_Ambient && Temperature.bAmbientGuardbandCheck && IniConfig.bL20AbientGuardBand))
                        {
                            ShowTempComp[Addr]->Color=clYellow;
                            bUT150State[Addr]=false;
                        }
                        else
                        {
                            ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                            bUT150State[Addr]=false;
                        }

                    }
                    else
                    {
                        ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                        bUT150State[Addr]=true;
                    }
                }
                else
                {
                    if(UN150Read[Addr]>(SetTemp+dTemptureRangeOver))            // OVER
                    {
                        ShowTempComp[Addr]->Color=(TColor)0x008000FF;
                        bUT150State[Addr]=false;
                    }
                    else if(UN150Read[Addr]<(SetTemp-dTemptureRangeBelow))      // Below
                    {
                        if(LastSet.iTemperature==Tempture_Hot ||
                           LastSet.iTemperature==Tempture_AmbientHot ||         //kevin 20140918 恆溫控制
                           (LastSet.iTemperature==Tempture_Ambient && Temperature.bAmbientGuardbandCheck && IniConfig.bL20AbientGuardBand))
                        {
                            ShowTempComp[Addr]->Color=clYellow;
                            bUT150State[Addr]=false;
                        }
                        else
                        {
                            ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                            bUT150State[Addr]=false;
                        }

                    }
                    else
                    {
                        ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                        bUT150State[Addr]=true;
                    }
                }
            }
            else
            {
                ShowTempComp[Addr]->Caption="---";
                ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                bUT150State[Addr]=true;
                UN150Read[Addr]=0;
            }
        }

    }
    else if(bUT150Install[Addr] && bGetHeaterUsed(Addr))
    {
        if(USE_16_HEATER==eht16Heater       ||                                  //Steven 20120606 : 16溫控器 8Site使用Hontech頭
           USE_16_HEATER==eht16HeaterEJ1N   ||
           USE_16_HEATER==eht32HeaterEJ1N   ||                                  //Steven 20140923 : Index使用EJ1N版32組加熱器
           USE_16_HEATER==eht32HeaterKT4H   ||                                  //Steven 20150211 : Index使用KT4H版32組加熱器
           USE_16_HEATER==eht16HeaterDTME08 ||                                  //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
           USE_16_HEATER==eht32HeaterDTME08 )                                   //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
        {
            if(Addr>=tcHead1 && Addr<tcSocket)
            {
                ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                ShowTempComp[Addr]->Caption="---";
                return false;
            }
        }

        if(ATC_SYSTEM==eNewATCSystem &&
           Temperature.bATCActiveCooling==true &&
           Temperature.bActiveHeatGun &&
          (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 ||
           ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35 ||
           ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61))
        {
            if(Addr>=tcHeatGun1 && Addr<=tcHeatGun2)
            {
                ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                str.sprintf("%5.1f", UN150Read[Addr]);
                ShowTempComp[Addr]->Caption=str;
                return false;
            }
        }

        //----- by dell ccd realtime-------------Steven 20110810
        if(Addr==tcCCD || (RTC_TemperNumber==2 && Addr==tcCCD_2))               //CCD  //Isaac 20201217 : RTC CCD增加第二組感溫
        {
            if(CUSTOMER_CODE==CC_SCC)                                           //jou 2012-03-30 SCC室溫就大約30度,所以就會一直吹氣
            {
                if(UN150Read[Addr]>=35.0)                                       //CCD超過30度就要吹
                {
                    bOverTemp=true;
                }
            }
            else
            {
                if(UN150Read[Addr]>=30.0)                                       //CCD超過30度就要吹
                {
                    bOverTemp=true;
                }
            }

            if(LastSet.iTemperature!=Tempture_AmbientHot)                       //kevin 20150731  恆溫控制
            {
                if(UN150Read[Addr]>=CCDTempLimit)                               //for HT9046LS
                {
                    bCCDOverTemp=true;
                }
                else if(UN150Read[Addr]<(CCDTempLimit-1))
                {
                    bCCDOverTemp=false;
                }
            }
            ShowTempComp[Addr]->Color=(UN150Read[Addr]>55.0)?(TColor)0x008000FF:(TColor)0x0025AB12;
        }
        else if(Addr==tc2D)                                                     //wei 20160614 2D溫控器讀取
        {
            ShowTempComp[Addr]->Color=(UN150Read[Addr]>50.0)?(TColor)0x008000FF:(TColor)0x0025AB12;
        }
        else if(Addr==tcLBUp || Addr==tcLBDown)                                 //Frank 20241231 : add
        {
            if(UN150Read[Addr]>=Temperature.dLBTempHighSettingValue)
                ShowTempComp[Addr]->Color=(TColor)0x008000FF;
            else
                ShowTempComp[Addr]->Color=(UN150Read[Addr]<Temperature.dLBTempLowSettingValue)?clYellow:(TColor)0x0025AB12;
        }
        else if(Addr==tcLB)                                                     //Steven 20181023 : LB溫度
        {
            if(UN150Read[Addr]>=200)
                ShowTempComp[Addr]->Color=(TColor)0x008000FF;
            else
                ShowTempComp[Addr]->Color=(UN150Read[Addr]<Temperature.dBoostIdleTime[Temperature.eBLBL])?clYellow:(TColor)0x0025AB12;
        }
        else
        //---------------------------------------
        {
            if(LastSet.iTemperature==Tempture_AmbientHot)                       //kevin 20150731  恆溫控制
            {
                if(Addr==tcChamber)                                             //for HT9046LS
                {
                    if(UN150Read[Addr]>=Temperature.fWorkTemperBase-Temperature.fAmbientHotGuartbent)//chambo     //kevin 20150731  恆溫控制
                    {
                        bCCDOverTemp=false;
                    }
                    else
                    {
                        bCCDOverTemp=true;                                      //降溫
                    }
                }
            }
            //Steven 20121101 Start : 修改常溫檢查範圍
            if(LastSet.iTemperature==Tempture_Ambient && Addr<=tcHotPlate2 &&   //jou 2012-12-03 修正高溫沒有顯示Hotplate
              (IniConfig.bSPILFunction==true ||                                 //Steven 20110725 : 常溫,矽品說不要顯示HotPlate的溫度  //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
               CUSTOMER_CODE==CC_SIGURD_PeiXing))                               //JerryYang 20160425 : 矽格建榮也要求常溫不顯示Hot plate的溫度
            {
                ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                ShowTempComp[Addr]->Caption="---";
                bTemperatureReady[Addr]=0;                                      //kevin 20210112 溫度過低過高  1 過低  :2  bTemperatureReady[Addr]=false//kevin 20130425
                return false;
            }

            if(CUSTOMER_CODE==CC_SCS &&
               ATC_SYSTEM==eATCHonPrecType &&
               (TestIF.iTestMode==DualSite || TestIF.iTestMode==SingleSite) &&
               LastSet.iTemperature==Tempture_Ambient &&
               Addr<=tcHotPlate2 &&
               Temperature.bATCActiveCooling==true)                             //Steven 20130122 : SCS常溫開啟ATC不要顯示HotPlate溫度
            {
                ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                ShowTempComp[Addr]->Caption="---";
                bTemperatureReady[Addr]=0;                                      //kevin 20210112  過高  1 過低  :2
                return false;
            }

            if(Addr==tcChamber && (Temperature.iIndexHeatMode==HeadOnly ||
                                   Temperature.iIndexHeatMode==HeadSocket))     //2013-09-02    Dell
            {
                if((CUSTOMER_CODE==CC_ASE_KaohSiung ||                          //kevin 20151119   head 模式不顯示CHAMBO溫度
                    CUSTOMER_CODE==CC_SIGURD_PeiXing) &&                        //Sam 20230328 : 矽格北興 Head 模式不顯示溫度
                   (LastSet.iTemperature==Tempture_Hot ||
                    LastSet.iTemperature==Tempture_AmbientHot))                 //kevin 20151119 沒使用chambo
                {
                    ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                    ShowTempComp[Addr]->Caption="---";
                    bTemperatureReady[Addr]=0;                                  //kevin 20210112  過高  1 過低  :2
                     return false;
                }
                else
                {
                    if(LastSet.iTemperature==Tempture_Hot)
                        SetTemp=Temperature.fChamberCoolTemp;
                    else
                        SetTemp=Temperature.fAbitTemp;
                }
            }
            else if(LastSet.iTemperature==Tempture_Hot &&
                    Temperature.bUseFixTemp==true &&                            //Steven 20131025 : 使4固定的DUT溫度
                    Addr>=tcDUT1                  &&
                    Addr<=tcDUT4)
            {
                SetTemp=Temperature.dFixedTemp;                                 //Steven 20131025 : 固定的DUT溫度值
            }
            else if((LastSet.iTemperature==Tempture_Hot||
                     LastSet.iTemperature==Tempture_AmbientHot) &&
                     Temperature.bATCActiveCooling==true &&                     //JerryYang 20220408 : add for ATC3.5
                    (Addr==tcATCHotAir1 || Addr==tcATCHotAir2))
            {
                SetTemp=IniConfig.dHeatGunTempATC;
            }
            else if(INSTALL_ATC_HEAT_GUN>0 && (Addr==tcATCHotAir1 || Addr==tcATCHotAir2))
            {
                 SetTemp=IniConfig.dHeatGunTempATC;
            }
            else
            {
                if(LastSet.iTemperature==Tempture_Hot ||
                   LastSet.iTemperature==Tempture_AmbientHot)                   //kevin 20140918 恆溫控制
                    SetTemp=(CosFunction.bUseIndividulTempSet && Temperature.bUseIndividualTemp)?Temperature.fIndividualTemp[Addr]:Temperature.fWorkTemperBase;        //Steven 20140924 : 各個加熱區獨立有自己的設定值
                else
                    SetTemp=Temperature.fAbitTemp;
                if(TestIF.iTestMode==SingleSite && Temperature.bMultiZoneEnable)    //wei 20240617 Multi Zone
                {
                    if(Addr==tcAa1 || Addr==tcAa2)
                        iChannel=0;
                    else if(Addr==tcAb1 || Addr==tcAb2)
                        iChannel=1;
                    else if(Addr==tcAc1 || Addr==tcAc2)
                        iChannel=2;
                    else if(Addr==tcAd1 || Addr==tcAd2)
                        iChannel=3;

                     SetTemp=Temperature.dZoneTempSetting[iChannel];
                }
            }

            if(IniConfig.bL07UseSingleTenmpertureLimit)
            {
                dTemptureRangeOver =IniConfig.dSingleTempLimit[Addr];
                dTemptureRangeBelow=IniConfig.dSingleTempLimit[Addr];
            }
            else
            {
                if(Addr==tcChamber)
                {
                    if(LastSet.iTemperature==Tempture_Ambient)
                    {
                        if(LastSet.iTemperature==Tempture_Ambient &&
                           Temperature.bAmbientGuardbandCheck &&
                           IniConfig.bL20AbientGuardBand)                       //kevin 20180115 (Steven) add Amient Guard Band
                        {
                            dTemptureRangeOver =Temperature.iAmbGuardband;
                            dTemptureRangeBelow=Temperature.iAmbGuardband;

                        }
                       else
                        {
                            dTemptureRangeOver =IniConfig.iAmbTemperatureRange; //jou 2013-04-11 Ambient Temperature Range
                            dTemptureRangeBelow=IniConfig.iAmbTemperatureRange; //jou 2013-04-11 Ambient Temperature Range
                        }
                    }
                    else
                    {
                        if(LastSet.iTemperature==Tempture_AmbientHot)           //kevin 20141231 恆溫控制
                        {
                            dTemptureRangeOver=Temperature.fAmbientHotGuartbent;
                            dTemptureRangeBelow=Temperature.fAmbientHotGuartbent;
                        }
                        else
                        {
                            if(LastSet.iTemperature==Tempture_Ambient &&
                               Temperature.bAmbientGuardbandCheck &&
                               IniConfig.bL20AbientGuardBand)                   //kevin 20180115 (Steven) add Amient Guard Band
                            {
                                dTemptureRangeOver =Temperature.iAmbGuardband;
                                dTemptureRangeBelow=Temperature.iAmbGuardband;

                            }
                            else
                            {
                                 dTemptureRangeOver =IniConfig.iL05ChamberTemptureRange;
                                 dTemptureRangeBelow=IniConfig.iL05ChamberTemptureRange;  //kevin 20190103 change alarm temp range different
                            }
                        }
                    }
                }
                else if(Addr==tcSocket || (Addr>=tcDUT1 && Addr<=tcDUT4))       //2013-01-15    Dell DUT(Socket Base)增加為4顆
                {
                    if(LastSet.iTemperature==Tempture_AmbientHot)               //kevin 20141231 恆溫控制
                    {
                        dTemptureRangeOver =Temperature.fAmbientHotGuartbent;
                        dTemptureRangeBelow=Temperature.fAmbientHotGuartbent;
                    }
                    else
                    {
                        dTemptureRangeOver =IniConfig.iSocketTemptureRangeOver; //ChungHung 20130408 add
                        dTemptureRangeBelow=IniConfig.iSocketTemptureRangeUnder;
                    }
                }
                else
                {
                    if(ATC_SYSTEM!=eNonChamber   &&                             //Steven 20140314 : For HT9045WA
                        ATC_SYSTEM>eATC30 &&                                    //20141204 ChungHung add for ATC3.0
                        Temperature.bATCActiveCooling==true &&                  //2014-05-30    Dell    for ATC6.0
                        ((Addr>=tcHead1 && Addr<=tcHead4) ||
                         (Addr>=tcAa1 && Addr<=tcBd2) ||
                         (Addr>=tcAe1 && Addr<=tcBh2)))
                    {
                        if(LastSet.iTemperature==Tempture_AmbientHot)           //kevin 20140918 恆溫控制
                        {
                            dTemptureRangeOver =Temperature.fAmbientHotGuartbent;
                            dTemptureRangeBelow=Temperature.fAmbientHotGuartbent;
                        }
                        else
                        {
                            dTemptureRangeOver =IniConfig.iATCTemperatureRange;
                            dTemptureRangeBelow=IniConfig.iATCTemperatureRange;
                        }
                    }
                    else
                    {
                        if(LastSet.iTemperature==Tempture_Ambient)
                        {
                            if(LastSet.iTemperature==Tempture_Ambient &&
                               Temperature.bAmbientGuardbandCheck &&
                               IniConfig.bL20AbientGuardBand)                   //kevin 20180115 (Steven) add Amient Guard Band
                            {
                                dTemptureRangeOver =Temperature.iAmbGuardband;
                                dTemptureRangeBelow=Temperature.iAmbGuardband;

                            }
                            else
                            {
                                dTemptureRangeOver =IniConfig.iAmbTemperatureRange; //jou 2013-04-11 Ambient Temperature Range
                                dTemptureRangeBelow=IniConfig.iAmbTemperatureRange; //jou 2013-04-11 Ambient Temperature Range
                            }
                        }
                        else
                        {
                            if(LastSet.iTemperature==Tempture_AmbientHot)       //kevin 20140918 恆溫控制
                            {
                                dTemptureRangeOver =Temperature.fAmbientHotGuartbent;
                                dTemptureRangeBelow=Temperature.fAmbientHotGuartbent;
                            }
                            else
                            {
                                dTemptureRangeOver =IniConfig.iL04TemptureRange;
                                dTemptureRangeBelow=IniConfig.iL04TemptureRange;
                            }
                        }
                    }
                }
            }

            if((bUseInitTempOffset   &&                                             //Steven 20141117 : 起測時溫度要補Offset
                iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial))   //Steven 20141125 : <= --> <
            {
                if(IniConfig.bL28TempOfsUseReadyTempRange && bEnable_KLT_Function==false)   //Sam 20231214 : Temp offset use ready temp range
                {
                    dTemptureRangeBelow=1;
                    dTemptureRangeOver=1;
                }
                else
                {
                    dTemptureRangeBelow=1;
                    dTemptureRangeOver+=Temperature.fTempOffSet[InitTempOffset][Addr];
                }
            }
            else if(bUseInitTempOffset && IniConfig.bL28TempOfsUseReadyTempRange && bEnable_KLT_Function==false && Temperature.iTempReadyRange!=0 &&    //Sam 20231214 : Temp offset use ready temp range
                    iInitContactCount<(Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset))
            {
                dTemptureRangeBelow=Temperature.iTempReadyRange;
                dTemptureRangeOver=Temperature.iTempReadyRange;
            }
            else if((Temperature.bBoostFuncttion ||
                     Temperature.bLBTempFunction) && iTriggerBoostFunction!=-1) //Steven 20180817 : Boost Function
            {
                if((Addr>=tcAa1 && Addr<=tcBd2) ||
                   (Addr>=tcAe1 && Addr<=tcBh2))
                {
                    dTemptureRangeBelow=1;
                    dTemptureRangeOver+=Temperature.dBoostOffset[iTriggerBoostFunction];
                }
            }

            if(INSTALL_ATC_HEAT_GUN>0 && (Addr==tcATCHotAir1 || Addr==tcATCHotAir2))
            {
                if(UN150Read[Addr]>(IniConfig.dHeatGunTempATC+10))              // OVER
                {
                    ShowTempComp[Addr]->Color=(TColor)0x008000FF;
                    iTempOverShowAlarmT[Addr]=1;
                }
                else if(UN150Read[Addr]<(IniConfig.dHeatGunTempATC-10))         // Below
                {
                    iTempOverShowAlarmT[Addr]=2;
                    if(LastSet.iTemperature==Tempture_Hot ||
                       LastSet.iTemperature==Tempture_AmbientHot ||
                       (LastSet.iTemperature==Tempture_Ambient && Temperature.bAmbientGuardbandCheck && IniConfig.bL20AbientGuardBand))
                        ShowTempComp[Addr]->Color=clYellow;
                    else
                        ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                }
                else
                {
                    iTempOverShowAlarmT[Addr]=0;
                    ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                }
            }
            else
            {
                if(UN150Read[Addr]>(SetTemp+dTemptureRangeOver))                // OVER
                {
                    ShowTempComp[Addr]->Color=(TColor)0x008000FF;
                    iTempOverShowAlarmT[Addr]=1;                                //kevin 20210112 溫度過高
                }
                else if(UN150Read[Addr]<(SetTemp-dTemptureRangeBelow))          // Below
                {
                    iTempOverShowAlarmT[Addr]=2;                                //kevin 20210112 溫度過低
                    if(LastSet.iTemperature==Tempture_Hot ||
                       LastSet.iTemperature==Tempture_AmbientHot ||             //kevin 20140918 恆溫控制
                       (LastSet.iTemperature==Tempture_Ambient && Temperature.bAmbientGuardbandCheck && IniConfig.bL20AbientGuardBand))
                        ShowTempComp[Addr]->Color=clYellow;
                    else
                        ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                }
                else
                {
                    iTempOverShowAlarmT[Addr]=0;                                //kevin 20210112 溫度過低過高
                    ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                }
            }

            //----- by dell ccd realtime-------------Steven 20110810
            if(Addr>=tcHead1 && UN150Read[Addr]>=30.0)                          //Chamber內任何位置超過30就吹
                bOverTemp=true;
            //---------------------------------------
        }

        bool bCanUseLowTemp=false;

        if(Temperature.bATCActiveCooling==true &&
          (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 ||
           ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35 ||
           ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61))
        {
            if((Addr>=tcAa1 && Addr<=tcBd2) ||
               (Addr>=tcAe1 && Addr<=tcBh2))
                bCanUseLowTemp=true;
            else
                bCanUseLowTemp=false;
        }
        else
        {
            bCanUseLowTemp=false;
        }

        if(UN150Read[Addr]<18 && bCanUseLowTemp==false)
        {
            #ifdef SOFT_SIMULTE
            //wei 20150703
            AnsiString str1;
            if(ATC_SYSTEM!=eNewATCSystem)
            {
                UN150Read[Addr]=Addr;
            }

            str1.sprintf("%0.2f", UN150Read[Addr]);

            if(IniConfig.bL29AmbientNotShowTemp &&
               LastSet.iTemperature==Tempture_Ambient)                          //Sam 20221101 : 常溫模式不顯示溫度
            {
                ShowTempComp[Addr]->Caption="---";
            }
            else
            {
                ShowTempComp[Addr]->Caption=str1;
            }

            asGPIBTempShow[Addr]=str1;                                          //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND

            #else
            ShowTempComp[Addr]->Caption="...";
            asGPIBTempShow[Addr]="NULL";                                        //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND
            #endif
            bTemperatureReady[Addr]=false;                                      //kevin 20130425

        }
        else if(UN150Read[Addr]==999)
        {
            if((TestIF.iTestMode==_8Site2X4 ||
                TestIF.iTestMode==_16Site4X4) &&                                //Sam 20190226 : 16Site4X4
               TestIF_File.bOctal_12Kit &&                                      //kevin 20150624 12Site 使用8site 溫度不秀ALARM
               (Addr==tcAd1 || Addr==tcBd1 || Addr==tcAd2 || Addr==tcBd2))
            {
                ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                ShowTempComp[Addr]->Caption="---";
                bOverTemp=false;
                asGPIBTempShow[Addr]="NULL" ;                                   //kevin 20130425  2013.01.11 Q_Q TSMC GPIB COMMAND
                bTemperatureReady[Addr]=true;                                   //kevin 20130425   沒有使用溫控器要當讀到
            }
            else
            {
                #ifdef SOFT_SIMULTE
                AnsiString str1;
                if(ATC_SYSTEM!=eNewATCSystem)
                {
                    UN150Read[Addr]=Addr;
                }
                str1.sprintf("%0.2f", UN150Read[Addr]);
                if(IniConfig.bL29AmbientNotShowTemp &&
                   LastSet.iTemperature==Tempture_Ambient)                      //Sam 20221101 : 常溫模式不顯示溫度
                    ShowTempComp[Addr]->Caption="---";
                else
                    ShowTempComp[Addr]->Caption=str1;
                asGPIBTempShow[Addr]=str1;                                      //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND
                UN150Read[Addr]=Addr;
                #else
                ShowTempComp[Addr]->Caption="ERR";
                asGPIBTempShow[Addr]="ERR";                                     //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND
                #endif
                bTemperatureReady[Addr]=false;                                  //kevin 20130425
            }
        }
        else
        {
            if((LastSet.iTemperature==Tempture_Hot ||
                LastSet.iTemperature==Tempture_AmbientHot) &&                   //kevin 20140918 恆溫控制
                Temperature.bUseFixTemp==true     &&                            //Steven 20131025 : 使4固定的DUT溫度
                Temperature.bShowFixedTemp==false &&                            //Steven 20131025 : True -> 畫面上要顯示固定的DUT溫度值, False -> 畫面上顯示fWorkTemperBase溫度值
                Addr>=tcDUT1                      &&
                Addr<=tcDUT4)
            {
                str.sprintf("%5.1f", UN150Read[Addr]+Temperature.fWorkTemperBase-Temperature.dFixedTemp);
            }
            else
            {
                str.sprintf("%5.1f", UN150Read[Addr]);
            }

            if(IniConfig.bL29AmbientNotShowTemp &&
               LastSet.iTemperature==Tempture_Ambient)                          //Sam 20221101 : 常溫模式不顯示溫度
                ShowTempComp[Addr]->Caption="---";
            else
                ShowTempComp[Addr]->Caption=str;

            if(Temperature.bUseReferTempSensor==true)                           //Ifor 20160629 add 開啟第二點溫度時傳送第二點溫度給測試機
                asGPIBTempShow[Addr].sprintf("%0.1f", UN150ReadSecondpoint[Addr]);
            else
                asGPIBTempShow[Addr].sprintf("%0.1f", UN150Read[Addr]);         //kevin 20130425  2013.01.11 Q_Q TSMC GPIB COMMAND

            bTemperatureReady[Addr]=true;                                       //kevin 20130425
        }

        if(LastSet.iTemperature==Tempture_Ambient &&
           Temperature.bAmbientGuardbandCheck &&
           IniConfig.bL20AbientGuardBand)                                       //JerryYang 20230307 : fix常溫模式Amient guard band check
        {
            if(SystemStart==false || iHome!=0)                                  //Pause 狀態不偵測低溫, Home不偵測低溫
            {
                iBelowAlmSec[Addr]=0;
                iOverAlarmSec[Addr]=0;
            }

            #ifndef SOFT_SIMULTE
            if(iTempOverShowAlarmT[Addr]==2)
            {
                if(SystemSec!=iSec)
                {
                    iSec=SystemSec;
                    iBelowAlmSec[Addr]++;
                    if(iBelowAlmSec[Addr]>=3)
                    {
                        S1.sprintf("WAR15%02d", Addr);
                        ShowErrorMessage(S1, 0, MMSystem);
                        iBelowAlmSec[Addr]=0;
                    }
                }
            }
            else if(iTempOverShowAlarmT[Addr]==1)
            {
                if(SystemSec!=iSec)
                {
                    iSec=SystemSec;
                    iOverAlarmSec[Addr]++;
                    if(iOverAlarmSec[Addr]>=3)
                    {
                        S1.sprintf("WAR15%02d", Addr+100);
                        ShowErrorMessage(S1, 0, MMSystem);
                        iOverAlarmSec[Addr]=0;
                    }
                }
            }
            #endif
        }
    }
    else
    {
        if(CUSTOMER_CODE==CC_ATEC)                                              //JerryYang 20191003 for 艾科 7000的2個感溫點SLK要依照site數顯示溫度數量
        {
            if(TestIF.iTestMode==QualSite1X4 && DeviceForm_File.iHeadDeviceCT==3 &&
               TestIF_File.b2CableLayoutKit && Temperature.bATCActiveCooling==false)
            {
                if(Addr==tcAc1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa1]->Caption;
                }
                else if(Addr==tcAd1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb1]->Caption;
                }
                else if(Addr==tcAc2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa2]->Caption;
                }
                else if(Addr==tcAd2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb2]->Caption;
                }
                else
                {
                    ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                    ShowTempComp[Addr]->Caption="---";
                }
            }
            else if(TestIF.iTestMode==QualSite2X2 && TestIF_File.b2CableLayoutKit && DeviceForm_File.iHeadDeviceCT==3)
            {
                if(Addr==tcBa1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa1]->Caption;
                }
                else if(Addr==tcBb1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb1]->Caption;
                }
                else if(Addr==tcBa2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa2]->Caption;
                }
                else if(Addr==tcBb2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb2]->Caption;
                }
                else
                {
                    ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                    ShowTempComp[Addr]->Caption="---";
                }
            }
            else if(TestIF.iTestMode==_8Site2X4 && TestIF_File.b2CableLayoutKit && Temperature.bATCActiveCooling==false && DeviceForm_File.iHeadDeviceCT==4)
            {
                if(Addr==tcAc1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb1]->Caption;
                }
                else if(Addr==tcAd1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb1]->Caption;
                }
                else if(Addr==tcBa1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa1]->Caption;
                }
                else if(Addr==tcBb1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa1]->Caption;
                }
                else if(Addr==tcBc1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb1]->Caption;
                }
                else if(Addr==tcBd1)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb1]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb1]->Caption;
                }
                else if(Addr==tcAc2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb2]->Caption;
                }
                else if(Addr==tcAd2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb2]->Caption;
                }
                else if(Addr==tcBa2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa2]->Caption;
                }
                else if(Addr==tcBb2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAa2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAa2]->Caption;
                }
                else if(Addr==tcBc2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb2]->Caption;
                }
                else if(Addr==tcBd2)
                {
                    ShowTempComp[Addr]->Color=ShowTempComp[tcAb2]->Color;
                    ShowTempComp[Addr]->Caption=ShowTempComp[tcAb2]->Caption;
                }
                else
                {
                    ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                    ShowTempComp[Addr]->Caption="---";
                }
            }
            else
            {
                ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
                ShowTempComp[Addr]->Caption="---";
            }
        }
        else
        {
            ShowTempComp[Addr]->Color=(TColor)0x0025AB12;
            ShowTempComp[Addr]->Caption="---";
        }
        bOverTemp=false;
        asGPIBTempShow[Addr]="NULL" ;                                           //kevin 20130425  2013.01.11 Q_Q TSMC GPIB COMMAND
        bTemperatureReady[Addr]=true;                                           //kevin 20130425   沒有使用溫控器要當讀到
    }
    return bOverTemp;
}
//---------------------------------------------------------------------------
//jou 2010-12-04 start
void TfTemperFrom::SetShowYield(eShowYieldType iItem, bool bOnOff)
{
    if(bOnOff==true)
    {
        strShowYield[iItem].OnOff=true;
    }
    else
    {
        strShowYield[iItem].OnOff=false;
    }
    ShowYieldFuntion();
}
//---------------------------------------------------------------------------
void TfTemperFrom::ShowYieldFuntion()
{
    AnsiString Str;
    for(int i=esytErrorRecovery; i<SHOW_YIELD_TOTAL; i++)
    {
        if(i==esytTest2)
        {
            if(strShowYield[i].bShow==true)                                     //kevin 20121008 start   add FT can't off set max value
            {
                if(LastSet.iRunStartMode==rsmContinuStart ||
                   LastSet.iRunStartMode==rsmInitialStart ||
                   LastSet.iRunStartMode==rsmAutoSiteMap ||
                   LastSet.iRunStartMode==rsmQAMode)
                    strShowYield[i].UsePanel->Caption="FT : Continues MAX_"+AnsiString(TestIF_File.iContsFailSocketAlarmCT); //kevin 20121009
                else
                    strShowYield[i].UsePanel->Caption="RT : Continues MAX_"+AnsiString(TestIF_File.iContsFailSocketAlarmCT_RT);//kevin 2012100

                strShowYield[i].UsePanel->Color=clLime;
                strShowYield[i].UsePanel->Refresh();
            }
            else
            {
                strShowYield[i].UsePanel->Caption="";
                strShowYield[i].UsePanel->Color=clBtnFace;
            }
        }
        else if(i==esytOEE)
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20220407 : JSCK OEE Function
            {
                strShowYield[esytOEE].OnOff=true;
                if(IniConfig.bN28_SCK_OEE)
                {
                    if(IniConfig.sN28_IP!="")
                        Str.sprintf("OEE ON(%s)", IniConfig.sN28_IP);
                    else
                        Str.sprintf("OEE ON(-)");
                    strShowYield[i].UsePanel->Caption=Str;
                    strShowYield[i].UsePanel->Color=clLime;
                }
                else
                {
                    Str.sprintf("OEE OFF");
                    strShowYield[i].UsePanel->Caption=Str;
                    strShowYield[i].UsePanel->Color=clRed;
                }
            }
            else
            {
                if(IniConfig.iN31_UseAutoTempOfsByFTP==0)                       //Steven 20250423 : Add auto temp offset display
                {
                    strShowYield[esytOEE].OnOff=false;
                    Str.sprintf("Auto Temp Offset OFF");
                    strShowYield[i].UsePanel->Caption=Str;
                    strShowYield[i].UsePanel->Color=clRed;
                }
                else
                {
                    strShowYield[esytOEE].OnOff=true;
                    if(IniConfig.iN31_UseAutoTempOfsByFTP==1)
                        Str.sprintf("Auto Temp Offset by FTP");
                    else
                        Str.sprintf("Auto Temp Offset by NetDrive");
                    strShowYield[i].UsePanel->Caption=Str;
                    strShowYield[i].UsePanel->Color=clLime;
                }
            }
        }
        else                                                                    //kevin 20121008 end
        {
            if(strShowYield[i].bShow==true)
            {
                if(strShowYield[i].OnOff==true)
                {
                    strShowYield[i].UsePanel->Caption=strShowYield[i].asName+" On";
                    strShowYield[i].UsePanel->Color=clLime;
                }
                else
                {
                    if(CUSTOMER_CODE==CC_ASE_M)                                 //Ifor 20170413 (wei) ASEM 要求無開啟功能不要顯示
                    {
                        strShowYield[i].UsePanel->Caption="";
                        strShowYield[i].UsePanel->Color=clBtnFace;
                    }
                    else
                    {
                        strShowYield[i].UsePanel->Caption=strShowYield[i].asName+" Off";
                        strShowYield[i].UsePanel->Color=clRed;
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfTemperFrom::ShowOffYieldFun()
{
    if(IniConfig.bShowFunctionWindow)
    {
        for(int i=esytErrorRecovery; i<SHOW_YIELD_TOTAL; i++)
        {
            if(strShowYield[i].bShow==true && strShowYield[i].OnOff==false)
            {
                if(IniConfig.bShowOffYieldBlink)                                //Steven 20120609 : 功能關畫面要不要閃爍
                {
                    strShowYield[i].iCount++;
                    if(strShowYield[i].iCount>5)
                    {
                        strShowYield[i].iCount=0;
                        strShowYield[i].bFlag=!strShowYield[i].bFlag;
                    }
                }
                else
                {
                    strShowYield[i].bFlag==true;
                }

                if(strShowYield[i].bFlag==true)
                {
                    strShowYield[i].UsePanel->Color=clRed;
                }
                else
                {
                    strShowYield[i].UsePanel->Color=clBtnFace;
                }
            }
        }
    }
}
//jou 2010-12-04 end
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::Timer1Timer(TObject *Sender)
{
    if(bShow==false)
        return;
    if(InitialOK==false)
        return;

    static bool bEnter=false;                                                   //ChungHung 20141107 fix out of memory
    if(bEnter)
        bEnter=true;

    static bool bCooling=true;
    bool bOverTemp=false;

    if(CUSTOMER_CODE==CC_GM_TEST)                                               //Steven 20220523 : GM Test顯示site map check功能有沒有開
        fTemperFrom->SetShowYield(fTemperFrom->esytQAMode,  IniConfig.bN29_ParameterCheckForGMTest);
    else if(IniConfig.bQAMode==true && LastSet.iRunStartMode==rsmQAMode)        //20141204 ChungHung add for ATK QA mode
        fTemperFrom->SetShowYield(fTemperFrom->esytQAMode,  true);
    else
        fTemperFrom->SetShowYield(fTemperFrom->esytQAMode,  false);

    for(int i=tcHotPlate1; i<tcTotalCount; i++)
    {
        bOverTemp|=ShowThermo(i);
        RunInfo.ShowTempComp[i]=ShowTempComp[i]->Caption;                       //Steven 20140510 : Secs Gem
    }

    //----- by dell ccd realtime-------------Steven 20110811
    if(bCooling!=bOverTemp)
        bCooling=true;

    if(REAL_TIME_CCD)
    {
        if((bOverTemp==true && bCooling==true) ||
           (Sen[SnRTCCDTempCtrl].Enable==true && Sen[SnRTCCDTempCtrl].IsOn()))  //Steven 20110725
        {
            if(SW[SwCCDCooling].Enable==true)
                SW[SwCCDCooling].On();
            bCooling=false;
        }
        else
        {
            if(fiosetview->fShow==false)
            {
                if(SW[SwCCDCooling].Enable==true)
                    SW[SwCCDCooling].Off();
            }
        }
    }
    else if(ATC_SYSTEM>eATC60 && ATC_SYSTEM!=eNonChamber)                       //2014-05-30    Dell    for ATC6.0
    {
        if(fiosetview->fShow==false)
            SW[SwCCDCooling].On();
    }
    //--------------------------------------

    ShowOffYieldFun();
    if(bStartRecord)                                                            //Steven 20120406 : 紀錄溫度
    {
        RecordTemp();
    }

    fLotInfo->ShowATCThermo();                                                  //Steven 20120410 : Hontech ATC

    bEnter=false;                                                               //ChungHung 20141107 fix out of memory
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
    Timer1->Enabled=false;
    MySleep(300);
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::Panel73MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE)
        return;
    if(Button==mbLeft)
    {
        if(bGreen==false)
            bYellow=true;
    }
    else
    {
        bGreen=false;
        bYellow=false;
    }
    fAutoTeach->DoAutoTeachProcess();                                           //JimmyChiu 20211020 : Auto alignment mode
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::Panel72MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE)
        return;
    if(Button==mbLeft)
    {
        if(bGreen==false && bYellow)
        {
            bGreen=true;
        }
        else
        {
            bGreen=false;
            bYellow=false;
        }
    }
    else
    {
        bGreen=false;
        bYellow=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::Panel71MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
       CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                     //Steven 20131101 : Add ASE-K12
        return;

    if(SystemStart || AccessLevel<iDefHonPrecLevel)                             //jou 2014-06-19 Security Have 5 Level 3->iDefHonPrecLevel
        return;

    if(bGreen==false || bYellow==false || Button==mbLeft)
    {
        bGreen=false;
        bYellow=false;
        return;
    }
    bGreen=false;
    bYellow=false;
    if(Application->MessageBox("Reset the hardware apparatus information?", NULL, MB_YESNO | MB_TOPMOST) != IDYES)
        return;

    #ifndef SOFT_SIMULTE
    if(CUSTOMER_CODE!=CC_HONPREC_QC)                                            //jou 2010-07-27 start : 隱藏畫面加上password防護
    {
        //fPassword->Edit1="";                                                  //Steven 20120912 : 避免密碼未清空
        fPassword->edPassword->Clear();                                         //kevin  20120914 : 避免密碼未清空
        fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);    //Steven 20120904 : 改用QWERTY鍵盤
        if(fPassword->edPassword->Text!="27025312")
        {
            return;
        }
    }
    #endif

    HandlerSystem->ShowModal();
}
//---------------------------------------------------------------------------
//extern AnsiString asTempCtrl[tcTotalCount];
void __fastcall TfTemperFrom::ShowHotName()
{
    for(int i=tcHotPlate1; i<=tcBd2; i++)
    {
        if((iSocketBaseTempCount==eDut4ea || iSocketBaseTempCount==eDut2ea) && i==8)    //kevin 20130321 : Omron EJ1N溫控器 SOCKET BASE 溫控器一個改4個
            continue;

        if(bUT150Install[i]==false)
        {
            ShowTempComp[i]->Caption="---";
            NameTempComp[i]->Caption="---";
        }
        else
        {
            NameTempComp[i]->Caption=asTempCtrl[i];
        }
    }

    if(Tri_Temp_Machine==1)
    {
        NameTempComp[tcShuttle1]->Caption   =asTempCtrl[tcShuttle1];
        NameTempComp[tcShuttle2]->Caption   =asTempCtrl[tcShuttle2];
        NameTempComp[tcShuttle3]->Caption   =asTempCtrl[tcShuttle3];
        NameTempComp[tcShuttle4]->Caption   =asTempCtrl[tcShuttle4];
        NameTempComp[tcATCHotAir1]->Caption =asTempCtrl[tcATCHotAir1];
        NameTempComp[tcATCHotAir2]->Caption =asTempCtrl[tcATCHotAir2];
        NameTempComp[tcHotPlate1]->Caption  =asTempCtrl[tcHotPlate1];
        NameTempComp[tcHotPlate2]->Caption  =asTempCtrl[tcHotPlate2];
        NameTempComp[tcHotPlate3]->Caption  =asTempCtrl[tcHotPlate3];
        NameTempComp[tcHotPlate4]->Caption  =asTempCtrl[tcHotPlate4];
        NameTempComp[tcDoor1]->Caption      =asTempCtrl[tcDoor1];               //Ztex 2023.10.23 Add Index Door Heater
        NameTempComp[tcDoor2]->Caption      =asTempCtrl[tcDoor2];               //Ztex 2023.10.23 Add Index Door Heater
    }

    for(int i=tcAe1; i<=tcBh2; i++)                                             //Steven 20150211 : Index使用KT4H版32組加熱器
    {
        if(bUT150Install[i]==false)
        {
            ShowTempComp[i]->Caption="---";
            NameTempComp[i]->Caption="---";
        }
        else
        {
            NameTempComp[i]->Caption=asTempCtrl[i];
        }
    }
}
//---------------------------------------------------------------------------
void TfTemperFrom::ChangeFormSize()
{
    int iHeight=0;
    if(IniConfig.bShowFunctionWindow)
    {
        if(IniConfig.bShowTemper==false)
        {
            gbOldTempWindow->Visible=false;
            palNewTempWindow->Visible=false;
        }
    }
    else
    {
        if(IniConfig.bShowTemper==false)
        {
            gbFunctionWindow->Visible=false;
            gbOldTempWindow->Visible =false;
            palNewTempWindow->Visible=false;
        }
    }

    if(gbFunctionWindow->Visible==true)
        iHeight=iHeight+gbFunctionWindow->Height;

    if(gbOldTempWindow->Visible==true)
        iHeight=iHeight+gbOldTempWindow->Height;

    if(palNewTempWindow->Visible==true)
    {
        iHeight=iHeight+pnl16Heater->Height;
    }

    if(pnl32Heater->Visible==true)                                              //Steven 20250701 : 調整溫度顯示畫面
    {
        iHeight=iHeight+pnl32Heater->Height;
    }
    fTemperFrom->Height=iHeight+5;
}
//---------------------------------------------------------------------------
void TfTemperFrom::SetIndex16HeaterPos()                                        //Steven 20111208 : 重新排列溫控器位置
{
    int iMyLeft[]={2, 61, 120, 178};
    int iMyTop[]={12, 40};

    if((TestIF_File.iTestMode==QualSite1X4 ||
        TestIF.iTestMode==_8Site1X4) &&                                         //ChungHung 20150528 add for 海思 _8Site1x4
       (TestIF_File.b2CableLayoutKit==false &&
        TestIF_File.b1CableLayoutKit==false))                                   //jou 2015-10-15  : 16溫控器 1條線版本
    {
        int iMapping[]={0, 4, 1, 5, 2, 6, 3, 7};

        for(int i=0; i<8; i++)
        {
            NameTempComp[tcAa1+iMapping[i]]->Top =iMyTop[i/4];
            NameTempComp[tcAa1+iMapping[i]]->Left=iMyLeft[i%4];
            ShowTempComp[tcAa1+iMapping[i]]->Top =iMyTop[i/4]+12;
            ShowTempComp[tcAa1+iMapping[i]]->Left=iMyLeft[i%4];
            NameTempComp[tcAa2+iMapping[i]]->Top =iMyTop[i/4];
            NameTempComp[tcAa2+iMapping[i]]->Left=iMyLeft[i%4];
            ShowTempComp[tcAa2+iMapping[i]]->Top =iMyTop[i/4]+12;
            ShowTempComp[tcAa2+iMapping[i]]->Left=iMyLeft[i%4];
//            iTop= ShowTempComp[tcAa2+iMapping[i]]->Top;//kevin 20120523
        }
    }
    else //16Site可能要修改
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                NameTempComp[tcAa1+i*4+j]->Top =iMyTop[i];
                NameTempComp[tcAa1+i*4+j]->Left=iMyLeft[j];
                ShowTempComp[tcAa1+i*4+j]->Top =iMyTop[i]+12;
                ShowTempComp[tcAa1+i*4+j]->Left=iMyLeft[j];
                NameTempComp[tcAa2+i*4+j]->Top =iMyTop[i];
                NameTempComp[tcAa2+i*4+j]->Left=iMyLeft[j];
                ShowTempComp[tcAa2+i*4+j]->Top =iMyTop[i]+12;
                ShowTempComp[tcAa2+i*4+j]->Left=iMyLeft[j];
//                iTop=ShowTempComp[tcAa2+i*4+j]->Top;//kevin 20120523
            }
        }
    }
}
//---------------------------------------------------------------------------
/*
bool __fastcall TfTemperFrom::TempOverCannotRunShowAlarm()
{
    for(int i=tcHotPlate1; i<tcTotalCount; i++)
    {
        if(bUT150Install[i] && bTempOverCannotRunShowAlarmT[i])
        {
            return true;
        }
    }
    return false;
} */
//---------------------------------------------------------------------------
bool __fastcall TfTemperFrom::TempRunShowAlarmHigh()
{
    for(int i=tcHotPlate1; i<tcTotalCount; i++)
    {
        if(bUT150Install[i] && iTempOverShowAlarmT[i]==1)                       //kevin 20210112 溫度過高
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TfTemperFrom::TempRunShowAlarmLow()
{
    for(int i=tcHotPlate1; i<tcTotalCount; i++)
    {
        if(bUT150Install[i] && iTempOverShowAlarmT[i]==2)                       //kevin 20210112 溫度過低
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
/*
void __fastcall TfTemperFrom::TempOverCannotRunShowAlarmClear()
{
    for(int i=tcHotPlate1; i<tcTotalCount; i++)
    {
        bTempOverCannotRunShowAlarmT[i]=false;
    }
} */
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::Button1Click(TObject *Sender)                     //Steven 20120406 : 紀錄溫度
{
    AnsiString sFileName;
    if(bStartRecord==false)
    {
        sFileName="Time";
        Button1->Caption="結束紀錄";
        bStartRecord=true;
        iStartMin=-1;
        for(int i=0; i<tcTotalCount; i++)
            sFileName+=", "+asTempCtrl[i];
        Memo1->Lines->Add(sFileName);
    }
    else
    {
        Button1->Caption="開始紀錄";
        bStartRecord=false;
        sFileName.sprintf("%s\\%04d%02d\\", asTempLogPath, SystemYear, SystemMonth);
        MyForceDirectories(sFileName);
        sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_Temp.csv", asTempLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        Memo1->Lines->SaveToFile(sFileName);
        Memo1->Clear();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::RecordTemp()
{
    AnsiString sFileName;
    if(iStartMin!=SystemMin)
    {
        iStartMin=SystemMin;
        sFileName.sprintf("%02d:%02d:%02d", SystemHour, SystemMin, SystemSec);

        for(int i=0; i<tcTotalCount; i++)
        {
            sFileName+=", "+ShowTempComp[i]->Caption;
        }

        Memo1->Lines->Add(sFileName);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTemperFrom::SetTempFormPos()                                  //Steven 20120525 : 換個地方調整TemperFrom位置
{
    fTemperFrom->Top=fMain->Top+fMain->Height+5;
    fTemperFrom->Left=fMain->Left;
}
//---------------------------------------------------------------------------
bool TfTemperFrom::Check_Tri_Temp_All_Temperature()                             //Ztex 2023.04.19 Add HT-1032 TriTemp Function)
{
    bool bResult=true;

    for(int i=0; i<tcTotalCount; i++)
    {
        bResult&=bUT150State[i];
    }

    if(bResult==false)
    {
        ShowMyMessage("Waiting Temperature in range");
    }

    return bResult;
}
//---------------------------------------------------------------------------
//#include "MyMotor.h"
void __fastcall TfTemperFrom::Button6Click(TObject *Sender)
{
    ShowErrorMessage("WAR07301", K_RETRY, MInShuttle1, false, "");
}
//---------------------------------------------------------------------------
