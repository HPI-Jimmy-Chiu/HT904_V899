#include "MachineDefine.h"
#pragma hdrstop

#include "uHeaterThread.h"

#include "bthermo.h"
#include "csystem.h"
#include "cmydef.h"
#include "cpublic.h"
#include "cprod.h"
#include "uLotInfo.h"   //20090813 Steven
#include "mysensor.h"
#include "myswitch.h"
#include "database.h"
#include "note.h"
#include "iosetview.h"
#include "cMyDB.h"
#include "common.h"
#include "uTemp_Set.h"
#include "ATC_Handler_Side.h"
#include "InOutArmZteach.h"
#include "main.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

//******************************************************************************
//
//  注意!! THeaterThread為Handler 溫度控制相關, 修改時要小心!!
//
//******************************************************************************
extern bool InitialOK;
THeaterThread *HeaterThread;
bool bHeatOverTenErrorOK=false;                                                 //kevin 20130226 加熱ok後 溫度過高
bool bHeatOKOverError=false;                                                    //kevin 20130226 加熱ok後 溫度過高

//---------------------------------------------------------------------------
__fastcall THeaterThread::THeaterThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
    bEnd=false;                                                                 //Steven 20110718 : 為了要可以關閉執行緒
}
//---------------------------------------------------------------------------
void __fastcall THeaterThread::HeaterThreadProcess(void)
{
    if(InitialOK)
    {
        CheckATC6System();                                                      //ChungHung 20141024 add
        DoThermo();                                                             //jou 981209 any time need detect heater state
        HeaterDoorIsOpen();                                                     //jou 981013 : unify check heater doop sensor
        CheckHeater();                                                          //jou 981209 any time need detect heater state
        DoHeaterOn();                                                           //jou 981209 any time,也需偵測風扇是否轉動
    }
}
//---------------------------------------------------------------------------
void __fastcall THeaterThread::Execute()
{
    do
    {
        Synchronize( HeaterThreadProcess);
        MySleepEx(20,true);
    } while (!bEnd);                                                            //Steven 20110718 : 為了要可以關閉執行緒
//    } while (!Terminated);
}
//---------------------------------------------------------------------------
void StartHeaterThread()
{
    HeaterThread->Priority=tpNormal;
    HeaterThread->bEnd=false;                                                   //Steven 20110718 : 為了要可以關閉執行緒
    HeaterThread->Resume();
}
//------------------------------------------------------------------------------
void EndHeaterThread()
{
    HeaterThread->bEnd=true;                                                    //Steven 20110718 : 為了要可以關閉執行緒
    //HeaterThread->WaitFor();                                                  //Steven 20110718 : 為了要可以關閉執行緒
    HeaterThread->Terminate();                                                  //kevin 20170421 (wei) : 為了要可以關閉執行緒
//    HeaterThread->Suspend();
    MySleep(100);
}
//******************************************************************************
//
//  注意!! CheckHeater為Handler 溫度控制相關, 修改時要小心!!
// 此為系統檢查加熱情形
//
//******************************************************************************
TQPF_Timer CCDAlarmDelay;
TQPF_Timer tHeaterAlarmTimer;
TQPF_Timer tHeaterStableTimer;                                                  //JerryYang 20210122 : ASE-CL新增待溫功能
//extern AnsiString asTempCtrl[tcTotalCount];
//******************************************************************************
bool CheckHeaterOK()                                                            //Steven 20250116 : 確認HeaterOK
{
    bool bResult=true;
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20201015 add ASE off line no check Temp
    {
        if(LastSet.iTester==ON_LINE &&
           LastSet.iTemperature!=Tempture_Ambient &&
           fHeaterOK==false)                                                    //Steven 20111030 : 溫度異常Index就不可以動
        {
            bResult=false;
        }
    }
    else
    {
        if(LastSet.iTemperature!=Tempture_Ambient &&
           (fHeaterOK==false ||                                                 //Steven 20120314 : 加熱還沒好,不要亂動
            fHeaterStableOK==false))                                            //JerryYang 20210122 : ASE-CL新增待溫功能
        {
            bResult=false;
        }
    }
    return bResult;
}
//******************************************************************************
void CheckHeater()
{
    #ifdef SOFT_SIMULTE
        iATCOnLine=true;
        fHeaterStableOK =fMain->chkHeaterOk->Checked;
        fHeaterOK       =fMain->chkHeaterOk->Checked;
        iHeaterWait     =fMain->chkHeaterOk->Checked;
        return ;
    #endif
//    #else

    int  iChannel=0;
    int  pos=0, iTemptureRange=0;
    int  pos1=0;
    bool flag=false, flagOverOk=false, flagBellowOk=false, bLess30deg=true, flagAmbOver=false;
    bool flagOverTenCutHeat=false;                                              //kevin 20130226 超過溫度關加 熱
    bool bIsTJControl=false;                                                    //Ifor 20190328 : add TJ Temp Over Range
    bool bTempChange=false;
    bool bATC_HotGun=false;                                                     //Ifor 20220426 add ATC Hot Gun
    double dTemptureRangeOver=0.0, dTemptureRangeBelow=0.0;
    double dTempWorkBase=0.0;
    AnsiString S1="", S2="";
    AnsiString sInputErrorPart="", sEventErrorPart="";

    static int  iAlarmSecond=0;
    static int  iUT150ReadErrorCT[tcTotalCount]={0};
    static int  iESec=-1;
    static int  iCCDAlarmCT=0;                                                  //jou 2011-12-29 避免異常突波信號alarm
    static int  iAlarmOver165=0;                                                //kevin 20130222 設定溫度超過165 度就發alarm
    static int  iAlarmSecond1=0, iSec=-1, iAlarmSecondHeatOK=0;                 //kevin 20130226
    static bool bHeatOverStop=false;
    static bool bNeedSaveCCDErr=true;
    static bool bShuttleCooling=false;                                          //kevin 20130226
    static bool bChamboCooling=false;                                           //kevin 20130226
    static bool bFirstEnter=true;
    static bool bLastHeatOK=false;                                              //JerryYang 20210122 : ASE-CL新增待溫功能

    DoSwCoolingFan(false);                                                      //jou 2013-04-09

    {
        if(REAL_TIME_CCD)                                                       //----- by dell ccd realtime-------------
        {
            if(ATC_SYSTEM==eATCUninstall)                                       //JerryYang 20190531 ATC機型不偵測CCD溫度
            {
                if((Sen[SnRTCCDTempCtrl].Enable==true &&
                    Sen[SnRTCCDTempCtrl].IsOn()))
                {
                    iCCDAlarmCT++;                                              //jou 2011-12-29 避免異常突波信號alarm
                    if(iCCDAlarmCT>5 && CCDAlarmDelay.Off())
                    {
                        fLotInfo->ALed3->Value=true;                            //Steven 20110918
                        SW[SwHeaterRelay].Off();
                        HeaterLog("CheckHeater_1 SnRTCCDTempCtrl", false);      //Steven 20151123 : Log for Heater Relay

                        fHeaterOK=false;
                        if(CUSTOMER_CODE!=CC_ASE_KaohSiung)                     //kevin 20141015   會造成溫度過低不會發alarm
                            bHeatOKBellowError=false;                           //jou 2014-06-12 修正偶發性秀低溫異常
                        ShowErrorMessage("WAR15181", 0, MMSystem, "RTCCDTempCtrl");     //Steven 20140508 : Modify WAR15181
                        bNeedSaveCCDErr=true;
                        CCDAlarmDelay.SetSecAndOn(60);
                    }
                }
                else if(UN150Read[tcCCD]!=999 &&
                        UN150Read[tcCCD]>52.5)                                  //Steven 20110924 : 因為會誤判,所以拆開
                {
                    fLotInfo->ALed3->Value=true;                                //Steven 20110918
                    if(bNeedSaveCCDErr)                                         //只有第一次發生要紀錄
                    {
                        MyDBIProcessNew("Message", "WAR15110", "CCD Over Temperature", AnsiString(UN150Read[10]));  //Steven 20240930 : WAR1560 --> WAR15110
                        bNeedSaveCCDErr=false;
                    }
                    iCCDAlarmCT=0;
                    CCDAlarmDelay.SetSecAndOn(0);
                }
                else
                {
                    fLotInfo->ALed3->Value=false;                               //Steven 20110918
                    bNeedSaveCCDErr=true;                                       //Steven 20110924
                    iCCDAlarmCT=0;
                    CCDAlarmDelay.SetSecAndOn(0);
                }
            }
        }
        else
        {
            iCCDAlarmCT=0;
        }

        //---------------------------------------
        if(iESec!=SystemSec && SystemStart && iHome==0)
        {
            iESec=SystemSec;
            for(int i=0; i<tcTotalCount; i++)
            {
                if(bUT150Install[i]==false)
                    continue;
                if(bGetHeaterUsed(i)==false)
                    continue;

                if(LastSet.iTemperature==Tempture_Hot ||
                   LastSet.iTemperature==Tempture_AmbientHot)                   //kevin 20140918 恆溫控制
                {
                    if(UN150Read[i]==999)
                    {
                        iUT150ReadErrorCT[i]++;
                        if(iUT150ReadErrorCT[i]>=40)
                        {
                            iUT150ReadErrorCT[i]=0;
                            S1.sprintf("WAR15%02d", i+100);                     //Steven 20140422 : 重新整理    //JerryYang 20160926 修正通訊異常時Alarm顯示錯誤,i+1 --> i+50 //Steven 20190925 : i+50 --> i+100
                            S2.sprintf("CheckHeater 1 (%s)", asTempCtrl[i]);
                            ShowErrorMessage(S1, 0, MMSystem);
                            iTemperatureOk=1;                                   //kevin 20150914
                            NewRecordProcess("MES2130", "Temperature Wait", S2);//kevin 20150914 溫度尚未到達
                        }
                    }
                    else
                    {
                        iUT150ReadErrorCT[i]=0;
                    }
                }
                else
                {
                    iUT150ReadErrorCT[i]=0;
                }
            }
        }

        if(SW[SwHeaterRelay].Status()==false)
        {
            fHeaterOK=false;
            fHeaterOK_Backup=false;                                             //jou 2010-10-11
            if(CUSTOMER_CODE!=CC_ASE_KaohSiung)                                 //kevin 20141015   會造成溫度過低不會發alarm
                bHeatOKBellowError=false;                                       //jou 2014-06-12 修正偶發性秀低溫異常
        }

        if(bDoRTCLearning==true)                                                //Ifor 20260226 add: Contact Mode 執行RTC Learn 不開啟Hot Air
        {
            SW[SwHeatGun].OnOff(false);
        }
        else if(INSTALL_HEAT_GUN>0 && Temperature.bActiveHeatGun)                    //ChungHung 20121107 add
        {
            SW[SwHeatGun].OnOff(SW[SwHeaterRelay].Status());
        }
        else                                                                    //ChungHung fix 沒有將溫度關掉直接關ActiveHeatGun HeatGun 不會關
        {
            SW[SwHeatGun].OnOff(false);
        }

        if(INSTALL_ATC_HEAT_GUN>0 && Temperature.bATCActiveCooling)             //JerryYang 20220408 : add for ATC3.5
        {
            SW[SwATCHeatGun].OnOff(SW[SwHeaterRelay].Status());
        }
        else
        {
            SW[SwATCHeatGun].OnOff(false);
        }

        if(LastSet.iTemperature==Tempture_Hot ||
           LastSet.iTemperature==Tempture_AmbientHot ||                         //kevin 20180115 常溫控制
           (LastSet.iTemperature==Tempture_Ambient &&
            Temperature.bAmbientGuardbandCheck &&
            IniConfig.bL20AbientGuardBand))                                     //kevin 20180115 (Steven) add Amient Guard Band)
        {
            tHeaterAlarmTimer.LatchCycleTimeSec(true);
            flagOverOk=true;
            flagBellowOk=true;
            for(int i=0; i<tcTotalCount; i++)
            {
//                #ifdef SOFT_SIMULTE
//                    if(i!=tcLBUp)
//                        continue;
//                #endif
                bIsTJControl=false;                                             //Ifor 20190328 : add TJ Temp Over Range
                bATC_HotGun=false;
                if(ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true &&
                   (bTJControlMode==true ||
                   (Temperature.bEnableTJFunction && bHasTjTemp==true && (bArm1IsTest==true || bArm2IsTest==true))))  //JerryYang 20251124 : Tj control吃不同的溫度range
                {
                    if((i>=tcAa1 && i<=tcBd2) || (i>=tcAe1 && i<=tcBh2))
                    {
                        bIsTJControl=true;
                    }
                }

                if(ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true &&
                   Temperature.bActiveHeatGun &&
                  (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 ||
                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35 ||
                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61))
                {
                    if(i>=tcHeatGun1 && i<=tcHeatGun2)
                    {
                        bATC_HotGun=true;
                    }

                    if(i==tcATCHotAir1 || i==tcATCHotAir2)                      //Ifor 20250926 add:ATC Hot Air
                    {
                        bATC_HotGun=true;
                    }
                }

                //----- by dell ccd realtime-------------Steven 20110811
                if(i==tcCCD || i==tcCCD_2)                                      //Isaac 20201217 : RTC CCD增加第二組感溫
                    continue;

                if((i>=tcHead1 && i<=tcHead4) ||
                   (i>=tcAa1 && i<=tcBd2) ||
                   (i>=tcAe1 && i<=tcBh2))
                {
                    if(ATC_SYSTEM==eWinWay &&
                       Temperature.bATCActiveCooling==true)                     //20210901 Jimmychiu
                    {
                        continue;
                    }
                    else
                    {
                        if(bNeedWaitATCRunSelfTestFinish==true)                 //Ifor 20160817 add ATC Slef Test Heat過溫不Alarm
                            continue;

                        if((ATC_SYSTEM==eATCHonPrecType ||                      //20210901 Jimmychiu
                            ATC_SYSTEM==eNewATCSystem) &&
                           (Temperature.bATCActiveCooling==true ||
                            Temperature.bATC70Active==true ||
                            ATC_SYSTEM==eWinWay))                               //Ifor 20160817 add 判斷是否於ATC模式且開啟ATC功能
                        {
                            if(iATCOnLine==false)                               //Ifor 20160817 ATC 未連線 Heat過溫不Alarm
                                continue;
                            if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70)  //Ifor 20160929 馬來西亞 高溫125 異常問題 暫時卡掉
                                continue;
                        }
                    }
                }

                if((LastSet.iTemperature==Tempture_Ambient &&
                    Temperature.bAmbientGuardbandCheck &&
                    IniConfig.bL20AbientGuardBand) &&                           //JerryYang 20230307 : 丁曉冬要求常溫不報hot plate alarm
                    i<=tcHotPlate2 &&
                    IniConfig.bSPILFunction==true)
                {
                    continue;
                }

                if(Temperature.iIndexHeatMode==HeadOnly)                        //Head Only  //Steven 20111209 : Head Only要另外控制Chamber溫度
                {
                    if(IniConfig.bTemp25degControl==true &&                     //jou 2014-06-07 Temperature 25 deg. control
                       Temperature.fWorkTemperBase<=25.0)
                    {
                        if(bUT150Install[i] && bGetHeaterUsed(i))
                        {
                            if(Temperature.bUseAbitCHK &&
                               Temperature.iAbitCHKStatus==1)                   //Running check
                            {
                                if(i==tcShuttle1 || i==tcShuttle2)              //shuttle 1 & 2
                                {
                                    dTemptureRangeOver =Temperature.fAbitTemp-1;
                                    dTemptureRangeBelow=Temperature.fAbitTemp-3;

                                    if(bShuttleCooling==true)
                                    {
                                        if(UN150Read[tcShuttle1]<dTemptureRangeBelow &&
                                           UN150Read[tcShuttle2]<dTemptureRangeBelow) //常溫溫度降溫低於3 度才停止吹氣
                                            bShuttleCooling=false;
                                    }
                                    else
                                    {
                                        if(UN150Read[i]>dTemptureRangeOver)     //常溫溫度高 於1 度才開始吹氣
                                            bShuttleCooling=true;
                                    }
                                }
                                else if(i==tcChamber)
                                {
                                    if(CUSTOMER_CODE==CC_KYEC_LEE)              //Ifor 20180302 (wei) : KYEC 喬智要求Chambo Cooling開啟/關閉時機
                                    {
                                        dTemptureRangeOver =Temperature.fChamberCoolTemp+2;
                                        dTemptureRangeBelow=Temperature.fChamberCoolTemp+1;
                                    }
                                    else
                                    {
                                        dTemptureRangeOver =Temperature.fChamberCoolTemp-1;
                                        dTemptureRangeBelow=Temperature.fChamberCoolTemp-3;
                                    }

                                    if(bChamboCooling)
                                    {
                                        if(UN150Read[i]<dTemptureRangeBelow)
                                            bChamboCooling=false;
                                    }
                                    else
                                    {
                                        if(UN150Read[i]>dTemptureRangeOver)
                                            bChamboCooling=true;
                                    }
                                }
                            }
                            else
                            {
                                bShuttleCooling=false;
                                bChamboCooling=false;
                            }
                        }
                    }

                    if(i==tcChamber)
                        continue;
                }

                if(LastSet.iTemperature==Tempture_AmbientHot)                   //kevin 20140918 恆溫控制
                {
                    if(i==tcHotPlate1 || i==tcHotPlate2)                        //常溫溫度控制 不使用 HOTPLATE 加熱IC
                        continue;

                    if(Temperature.bShuttleNoHeatUp==true &&                    //Steven 20180815 : Amb Ctr mode, shuttle no heat up
                       (i==tcShuttle1 || i==tcShuttle2))
                    {
                        continue;
                    }

                    if(Temperature.bSLKNoHeatUp &&                              //Steven 20230221 : Amb Ctr mode, SLK no heat up
                       ((i>=tcHead1 && i<=tcHead4) ||
                        (i>=tcAa1 && i<=tcBd2) ||
                        (i>=tcAe1 && i<=tcBh2)))
                    {
                        continue;
                    }

                    if(i==tcShuttle1 || i==tcShuttle2)                          //shuttle 1 & 2 //Steven 20240606 : fixed for 渦流管
                    {
                        bShuttleCooling=true;
                        dTemptureRangeOver =Temperature.fWorkTemperBase-1;
                        dTemptureRangeBelow=Temperature.fWorkTemperBase-3;

                        if(bShuttleCooling==true)
                        {
                            if(UN150Read[tcShuttle1]<dTemptureRangeBelow &&
                               UN150Read[tcShuttle2]<dTemptureRangeBelow)       //常溫溫度降溫低於3 度才停止吹氣
                                bShuttleCooling=false;
                        }
                        else
                        {
                            if(UN150Read[i]>dTemptureRangeOver)                 //常溫溫度高 於1 度才開始吹氣
                                bShuttleCooling=true;
                        }
                    }

                    if(bUT150Install[i] &&
                       (bGetHeaterUsed(i) ||
                        i==tcChamber))                                          //Steven 20240606 : Add for chanmber mode
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                     //JerryYang 20190422 只有高雄渦流管要一直吹氣
                        {
                            bChamboCooling=true;
                        }
                        else
                        {
                            if(bChamboCooling)
                            {
                                if(UN150Read[i]<Temperature.fWorkTemperBase-3)  //Steven 20240606 : fixed for 渦流管
                                    bChamboCooling=false;
                            }
                            else
                            {
                                if(UN150Read[i]>Temperature.fWorkTemperBase-1)
                                    bChamboCooling=true;
                            }
                        }
                    }
                }

                if(bUT150Install[i] && bGetHeaterUsed(i))
                {
                    if(ATC_SYSTEM==eMikros)                                     //Ifor 20231027 add: Mikros ATC System不監控Index 溫度
                    {
                        if((i>=tcAa1 && i<=tcBd2) ||
                           (i>=tcAe1 && i<=tcBh2))
                        {
                            continue;
                        }
                    }
                    else if(CosFunction.bATCModeNoUseCheckHeaterFunction==true &&
                            Temperature.bATCActiveCooling==true)
                    {
                        if((i>=tcAa1 && i<=tcBd2) ||
                           (i>=tcAe1 && i<=tcBh2))
                        {
                            continue;
                        }
                    }
                    else if(CosFunction.bATCModeNoUseCheckHeaterFunction==true &&
                            Temperature.bATCActiveCooling==true)
                    {
                        if((i>=tcAa1 && i<=tcBd2) ||
                           (i>=tcAe1 && i<=tcBh2))
                        {
                            continue;
                        }
                    }

                    dTempWorkBase=(CosFunction.bUseIndividulTempSet && Temperature.bUseIndividualTemp)?Temperature.fIndividualTemp[i]:Temperature.fWorkTemperBase;        //Steven 20140924 : 各個加熱區獨立有自己的設定值

                    if(LastSet.iTemperature==Tempture_Ambient &&
                       Temperature.bAmbientGuardbandCheck &&
                       IniConfig.bL20AbientGuardBand)                           //kevin 20180115 (Steven) add Amient Guard Band
                    {
                         dTempWorkBase=Temperature.fAbitTemp;
                    }

                    if(TestIF.iTestMode==SingleSite && Temperature.bMultiZoneEnable)       //wei 20240617 Multi Zone
                    {
                        bTempChange=false;
                        if((i>=tcAa1 && i<=tcBd1) || (i>=tcAe1 && i<=tcBh1) ||
                           (i>=tcAa2 && i<=tcBd2) || (i>=tcAe2 && i<=tcBh2))
                        {
                            if(i==tcAa1 || i==tcAa2)
                                iChannel=0;
                            else if(i==tcAb1 || i==tcAb2)
                                iChannel=1;
                            else if(i==tcAc1 || i==tcAc2)
                                iChannel=2;
                            else if(i==tcAd1 || i==tcAd2)
                                iChannel=3;

                            bTempChange=true;
                        }

                        if(bTempChange)
                        {
                            dTempWorkBase=Temperature.dZoneTempSetting[iChannel];
                        }
                    }

                    if(bIsTJControl==true)
                    {
                        if(UN150Read[i]>(dTempWorkBase+Temperature.dTJTempRange_High))
                        {
                            flagOverOk=false;
                            pos1=i;
                            break;
                        }
                        else if(UN150Read[i]<(dTempWorkBase-Temperature.dTJTempRange_Low))
                        {
//                            flagBellowOk=false;                               //JerryYang 20190925 Tj mode時的Tc過低暫時不alarm
                            pos=i;
                            break;
                        }
                    }
                    else if(bChangeTest_TempOffset!=0 &&                        //Steven 20250414 : Fixed for heater not ok but handler run
                            bChangeTest_TempAlarm==true)                        //Ifor 20210623 add: Test Temp Change
                    {

                    }
                    else if(bATC_HotGun)
                    {

                    }
                    else if(bUseInitTempOffset                      &&          //Sam 20231214 : Temp offset use ready temp range
                            bEnable_KLT_Function==false             &&
                            IniConfig.bL28TempOfsUseReadyTempRange  &&
                            iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)
                    {
                        if(UN150Read[i]>(dTempWorkBase+1))
                        {
                            flagOverOk=false;
                        }
                        else if(UN150Read[i]<(dTempWorkBase-1))
                        {
                            flagBellowOk=false;
                        }
                        if(flagBellowOk==false || flagOverOk==false)
                        {
                            fHeaterOK=false;
                        }
                    }
                    else if(bUseInitTempOffset                      &&
                            bEnable_KLT_Function==false             &&
                            IniConfig.bL28TempOfsUseReadyTempRange  &&
                            Temperature.iTempReadyRange!=0          &&
                            iInitContactCount<(Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset))
                    {
                        if(iWhichIndexArm==1)
                        {
                            if((i>=tcAa1 && i<=tcBd1) || (i>=tcAe1 && i<=tcBh1))
                                continue;
                        }
                        else if(iWhichIndexArm==2)
                        {
                            if((i>=tcAa2 && i<=tcBd2) || (i>=tcAe2 && i<=tcBh2))
                                continue;
                        }

                        if(UN150Read[i]>(dTempWorkBase+Temperature.iTempReadyRange))
                        {
                            flagOverOk=false;
                        }
                        else if(UN150Read[i]<(dTempWorkBase-Temperature.iTempReadyRange))
                        {
                            flagBellowOk=false;
                        }

                        if(flagBellowOk==false || flagOverOk==false)
                        {
                            fHeaterOK=false;
                        }
                    }
                    else
                    {
                        if(IniConfig.bL07UseSingleTenmpertureLimit)             //Frank 20161212 (Jou) LastSet->IniConfig
                        {
                            if(i==tcLB)                                         //Steven 20181023 : LB溫度
                            {

                            }
                            else if(i==tcLBUp || i==tcLBDown)                   //Frank 20241231 : add
                            {
                                if(Temperature.bLBTempHighAlarm_Enable==true &&
                                   UN150Read[i]>Temperature.dLBTempHighSettingValue)
                                {
                                    flagOverOk=false;
                                    pos1=i;
                                }
                                else if(Temperature.bLBTempLowAlarm_Enable==true &&
                                        UN150Read[i]<Temperature.dLBTempLowSettingValue)
                                {
                                    flagBellowOk=false;
                                    pos1=i;
                                }
                            }
                            else if(i==tc2D)                                    //wei 20161021 Barcode 超過50度Alarm
                            {
                                iTemptureRange=50;
                                if(UN150Read[i]>iTemptureRange)
                                {
                                    flagOverOk=false;
                                    pos1=i;
                                }
                            }
                            else if(i==tcATCHotAir1 || i==tcATCHotAir2)         //JerryYang 20220408 : add for ATC3.5
                            {
                                iTemptureRange=IniConfig.dHeatGunTempATC+10;    //Ifor 20240920 add: ATC Hot Air Tempture Range
                                if(UN150Read[i]>iTemptureRange)
                                {
                                    flagOverOk=false;
                                    pos1=i;
                                }
                            }
                            else if(UN150Read[i]>(dTempWorkBase+IniConfig.dSingleTempLimit[i]))
                            {
                                flagOverOk=false;
                                pos1=i;
                                if(UN150Read[i]>(dTempWorkBase+IniConfig.dSingleTempLimit[i]+10)) //kevin 20130225      溫度超過設定10 度
                                    flagOverTenCutHeat=true;
                                break;
                            }
                            else if(UN150Read[i]<(dTempWorkBase-IniConfig.dSingleTempLimit[i]))
                            {
                                flagBellowOk=false;
                                pos=i;
                                break;
                            }
                        }
                        else
                        {
                            //Sam 20200804 : 給初始值防止誤報警
                            //==>
                            iTemptureRange      =IniConfig.iL04TemptureRange;
                            dTemptureRangeOver  =IniConfig.iL04TemptureRange;
                            dTemptureRangeBelow =IniConfig.iL04TemptureRange;
                            //<==
                            //Sam 20200804 : 給初始值防止誤報警
                            if(bUseInitTempOffset)                              //Steven 20180820 : 簡化init temp offset判斷式   //Steven 20141117 : 起測時溫度要補Offset
                            {
                                //jou 2016-05-11 修正Initial Offset不能補負值的問題
                                //jou 2016-05-27 修正開啟溫度Initial補償,Offset 填0異常 > -> >=
                                if(Temperature.fTempOffSet[InitTempOffset][i]>=0)
                                {
                                    if(iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)  //Steven 20141125 : <= --> <
                                        dTemptureRangeBelow=1;

                                    if(iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset)  //Steven 20141125 : <= --> <
                                    {
                                        if(bTestOverTimeTempOffsetF)
                                        {
                                            dTemptureRangeOver+=Temperature.fTempOffSet[TestOverTimeTempOffset][i];   //kevin 20160312 add
                                        }
                                        else
                                        {
                                            dTemptureRangeOver+=Temperature.fTempOffSet[InitTempOffset][i];
                                        }
                                    }
                                }
                                else
                                {
                                    if(IniConfig.bL28TempOfsUseReadyTempRange &&
                                       bEnable_KLT_Function==false)             //Sam 20231214 : Temp offset use ready temp range
                                    {
                                        if(iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)
                                            dTemptureRangeOver=1;

                                        if(iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset)
                                            dTemptureRangeBelow=1;
                                    }
                                    else
                                    {
                                        if(iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)  //Steven 20141125 : <= --> <
                                            dTemptureRangeBelow-=Temperature.fTempOffSet[InitTempOffset][i];

                                        if(iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset)  //Steven 20141125 : <= --> <
                                            dTemptureRangeOver=1;
                                    }
                                }
                            }
                            else if(CosFunction.bNonATCSupportLBFunction &&
                                    (Temperature.bBoostFuncttion ||
                                     Temperature.bLBTempFunction) &&            //JerryYang 20220126 : 2度range太小, DUT容易超出range
                                    i!=tcDUT1 &&
                                    i!=tcDUT2 &&
                                    i!=tcDUT3 &&
                                    i!=tcDUT4)
                            {
                                if(iTriggerBoostFunction!=-1 &&
                                   iBoostFuncStep<1)
                                {
                                    dTemptureRangeOver=2;
                                    dTemptureRangeBelow=2;
                                }
                            }
                            else if(CosFunction.bNonATCSupportLBFunction==false &&
                                    (Temperature.bBoostFuncttion ||
                                     Temperature.bLBTempFunction))              //Steven 20180817 : Boost Function
                            {
                                if(iTriggerBoostFunction!=-1 && iBoostFuncStep<1)
                                {
                                    dTemptureRangeOver=2;
                                    dTemptureRangeBelow=2;
                                }
                            }
                            else if(CUSTOMER_CODE==CC_TSMC_TAINAN &&
                                    LastSet.iTemperature==Tempture_Hot &&
                                    dTempWorkBase==25)                          //wei 20170202 (jou) 因TSMC常溫溫度高於25度，如果跑常溫溫度會Alarm，所以25度增加5度range
                            {
                                iTemptureRange      =IniConfig.iL04TemptureRange+5;
                                dTemptureRangeOver  =IniConfig.iL04TemptureRange+5;
                                dTemptureRangeBelow =IniConfig.iL04TemptureRange+5;
                            }
                            else if(LastSet.iTemperature==Tempture_AmbientHot)  //kevin 20140918 恆溫控制
                            {
                                iTemptureRange      =Temperature.fAmbientHotGuartbent;
                                dTemptureRangeOver  =Temperature.fAmbientHotGuartbent;
                                dTemptureRangeBelow =Temperature.fAmbientHotGuartbent;
                            }
                            else if(LastSet.iTemperature==Tempture_Ambient &&
                                    Temperature.bAmbientGuardbandCheck &&
                                    IniConfig.bL20AbientGuardBand)              //kevin 20180115 add Amient Guard Band)  //kevin (Steven) 20180115 常溫控制
                            {
                                iTemptureRange      =Temperature.iAmbGuardband;
                                dTemptureRangeOver  =Temperature.iAmbGuardband;
                                dTemptureRangeBelow =Temperature.iAmbGuardband;
                            }
                            else if(i==tcChamber)
                            {
                                if(LastSet.iTemperature==Tempture_AmbientHot)   //kevin 20141231 恆溫控制
                                {
                                    iTemptureRange      =Temperature.fAmbientHotGuartbent;
                                    dTemptureRangeOver  =Temperature.fAmbientHotGuartbent;
                                    dTemptureRangeBelow =Temperature.fAmbientHotGuartbent;
                                }
                                else if(LastSet.iTemperature==Tempture_Ambient &&
                                        Temperature.bAmbientGuardbandCheck &&
                                        IniConfig.bL20AbientGuardBand)          //kevin 20180115 (Steven) add Amient Guard Band)  //kevin 20180115 常溫控制
                                {
                                    iTemptureRange      =Temperature.iAmbGuardband;
                                    dTemptureRangeOver  =Temperature.iAmbGuardband;
                                    dTemptureRangeBelow =Temperature.iAmbGuardband;
                                }
                                else
                                {
                                    iTemptureRange      =IniConfig.iL05ChamberTemptureRange;
                                    dTemptureRangeOver  =IniConfig.iL05ChamberTemptureRange;
                                    dTemptureRangeBelow =IniConfig.iL05ChamberTemptureRange;
                                }
                            }
                            else if(i==tcSocket || (i>=tcDUT1 && i<=tcDUT4))    //2013-01-15    Dell DUT(Socket Base)增加為4顆
                            {
                                iTemptureRange      =IniConfig.iSocketTemptureRangeOver;    //ChungHung 20130408 add
                                dTemptureRangeOver  =IniConfig.iSocketTemptureRangeOver;
                                dTemptureRangeBelow =IniConfig.iSocketTemptureRangeUnder;   //Steven 20170705 (wei) : Over --> Under
                            }
                            else
                            {
                                if(ATC_SYSTEM>eATC30 &&                         //20141204 ChungHung add for ATC3.0 //2014-05-30    Dell    for ATC6.0   //Steven 20201202 : makr for ATC temp range
                                   Temperature.bATCActiveCooling==true &&
                                   ((i>=tcHead1 && i<=tcHead4) ||
                                    (i>=tcAa1 && i<=tcBd2) ||
                                    (i>=tcAe1 && i<=tcBh2)))
                                {
                                    if(LastSet.iTemperature==Tempture_AmbientHot)   //kevin 20140918 恆溫控制  //Steven 20201202 : Add for ATC temp range
                                    {
                                        iTemptureRange     =Temperature.fAmbientHotGuartbent;
                                        dTemptureRangeOver =Temperature.fAmbientHotGuartbent;
                                        dTemptureRangeBelow=Temperature.fAmbientHotGuartbent;
                                    }
                                    else
                                    {
                                        iTemptureRange      =IniConfig.iATCTemperatureRange;
                                        dTemptureRangeOver  =IniConfig.iATCTemperatureRange;
                                        dTemptureRangeBelow =IniConfig.iATCTemperatureRange;
                                    }
                                }
                                else
                                {
                                    iTemptureRange      =IniConfig.iL04TemptureRange;
                                    dTemptureRangeOver  =IniConfig.iL04TemptureRange;
                                    dTemptureRangeBelow =IniConfig.iL04TemptureRange;
                                }
                            }

                            if(i==tcHeatGun1 || i==tcHeatGun2)                  //kevin 20130322 hotgun offset
                            {
                                if(dTempWorkBase+iTemptureRange+10>=HeatGunLimit &&
                                   iSec!=SystemSec)                             //chamber 因offset可以調到30 度 所以總溫度超過165度強制斷加熱電源
                                {
                                    iSec=SystemSec;
                                    iAlarmOver165++;
                                    if(iAlarmOver165>40)
                                    {
                                        iAlarmOver165=0;
                                        if(IniConfig.bL12TempErrNoCloseHeater==false)//kevin 20140401 add IniConfig.bL12TempErrNoCloseHeater==false
                                        {
                                            SW[SwHeaterRelay].Off();            //溫度過高超過10 度
                                            S2.sprintf("CheckHeater 2 (%s)", asTempCtrl[i]);
                                            HeaterLog(S2, false);               //Steven 20151123 : Log for Heater Relay
                                        }

                                        S1.sprintf("WAR15%02d", i+100);         //Steven 20140422 : 重新整理
                                        S2.sprintf("CheckHeater 2 (%s)", asTempCtrl[i]);
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);//kevin 20130218
                                        iTemperatureOk=1;                       //kevin 20150914
                                        NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                    }
                                }
                            }
                            else if(i==tcATCHotAir1 || i==tcATCHotAir2)         //JerryYang 20220408 : add for ATC3.5
                            {
                                if(IniConfig.dHeatGunTempATC+iTemptureRange+10>=HeatGunLimit && //Ifor 20240920 dTempWorkBase => IniConfig.dHeatGunTempATC
                                   iSec!=SystemSec)                             //chamber 因offset可以調到30 度 所以總溫度超過165度強制斷加熱電源
                                {
                                    iSec=SystemSec;
                                    iAlarmOver165++;
                                    if(iAlarmOver165>40)
                                    {
                                        iAlarmOver165=0;
                                        if(IniConfig.bL12TempErrNoCloseHeater==false)//kevin 20140401 add IniConfig.bL12TempErrNoCloseHeater==false
                                        {
                                            SW[SwHeaterRelay].Off();            //溫度過高超過10 度
                                            S2.sprintf("CheckHeater 2 (%s)", asTempCtrl[i]);
                                            HeaterLog(S2, false);               //Steven 20151123 : Log for Heater Relay
                                        }

                                        S1.sprintf("WAR15%02d", i+100);         //Steven 20140422 : 重新整理
                                        S2.sprintf("CheckHeater 2 (%s)", asTempCtrl[i]);
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);//kevin 20130218
                                        iTemperatureOk=1;                       //kevin 20150914
                                        NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                    }
                                }
                            }
                            else if(i==tcChamber)                               //Ifor 20230328 add: Chamber Temp Limit 165
                            {
                                if(dTempWorkBase+iTemptureRange+10>=ChamberTempLimit &&
                                   iSec!=SystemSec)                             //chamber 因offset可以調到30 度 所以總溫度超過165度強制斷加熱電源
                                {
                                    iSec=SystemSec;
                                    iAlarmOver165++;
                                    if(iAlarmOver165>40)
                                    {
                                        iAlarmOver165=0;
                                        if(IniConfig.bL12TempErrNoCloseHeater==false)//kevin 20140401 add IniConfig.bL12TempErrNoCloseHeater==false
                                        {
                                            SW[SwHeaterRelay].Off();            //溫度過高超過10 度
                                            HeaterLog("CheckChamberHeater", false);        //Steven 20151123 : Log for Heater Relay
                                        }

                                        S1.sprintf("WAR15%02d", i+100);         //Steven 20140422 : 重新整理
                                        S2.sprintf("CheckHeater 2 (%s)", asTempCtrl[i]);
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);//kevin 20130218
                                        iTemperatureOk=1;                       //kevin 20150914
                                        NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                    }
                                }
                            }
                            else if(dTempWorkBase+iTemptureRange+10>=TempFuseLimitType &&
                                    iSec!=SystemSec)                            //chamber 因offset可以調到30 度 所以總溫度超過165度強制斷加熱電源
                            {
                                iSec=SystemSec;
                                iAlarmOver165++;
                                if(iAlarmOver165>40)
                                {
                                    iAlarmOver165=0;
                                    if(IniConfig.bL12TempErrNoCloseHeater==false)//kevin 20140401 add IniConfig.bL12TempErrNoCloseHeater==false
                                    {
                                        SW[SwHeaterRelay].Off();                //溫度過高超過10 度
                                        S2.sprintf("CheckHeater 3 (%s)", asTempCtrl[i]);
                                        HeaterLog(S2, false);                   //Steven 20151123 : Log for Heater Relay
                                    }

                                    S1.sprintf("WAR15%02d", i+100);             //Steven 20140422 : 重新整理
                                    S2.sprintf("CheckHeater 3 (%s, WorkBase=%f, TempRange=%d, FuseType=%d)", asTempCtrl[i], dTempWorkBase, iTemptureRange, TempFuseLimitType);
                                    ShowErrorMessage(S1, K_RETRY, MMSystem);    //kevin 20130218
                                    iTemperatureOk=1;                           //kevin 20150914
                                    NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                }
                            }

                            if(i==tcLBUp || i==tcLBDown)                        //Frank 20241231 : add
                            {
                                if(Temperature.bLBTempHighAlarm_Enable==true &&
                                   UN150Read[i]>Temperature.dLBTempHighSettingValue)
                                {
                                    flagOverOk=false;
                                    pos1=i;
                                }
                                else if(Temperature.bLBTempLowAlarm_Enable==true &&
                                        UN150Read[i]<Temperature.dLBTempLowSettingValue)
                                {
                                    flagBellowOk=false;
                                    pos1=i;
                                }
                            }
                            else if(i==tcLB)                                    //Steven 20181023 : LB溫度
                            {
                                if(UN150Read[i]>(dTempWorkBase+dTemptureRangeOver))
                                {
                                    flagOverOk=false;
                                    pos1=i;
                                }
                            }
                            else if(i==tcATCHotAir1 || i==tcATCHotAir2)         //JerryYang 20220408 : add for ATC3.5
                            {
                                if(UN150Read[i]>(IniConfig.dHeatGunTempATC+dTemptureRangeOver))
                                {
                                    flagOverOk=false;
                                    pos1=i;
                                    if(UN150Read[i]>(IniConfig.dHeatGunTempATC+iTemptureRange+80)) //kevin 20130321
                                        flagOverTenCutHeat=true;

                                }
                            }
                            else if(UN150Read[i]>(dTempWorkBase+dTemptureRangeOver))
                            {
                                flagOverOk=false;
                                pos1=i;
                                if(i==tcHeatGun1 || i==tcHeatGun2)              //kevin 20130322 hotgun offset
                                {
                                    if(UN150Read[i]>(dTempWorkBase+iTemptureRange+80)) //kevin 20130321
                                        flagOverTenCutHeat=true;
                                }
                                else if(UN150Read[i]>(dTempWorkBase+iTemptureRange+10)) //kevin 20130218
                                {
                                    flagOverTenCutHeat=true;
                                }
                            }
                            else if(i==tcSocket || (i>=tcDUT1 && i<=tcDUT4))    //Steven 20140308 : DUT溫度限制改成上下限分開
                            {
                                iTemptureRange=IniConfig.iSocketTemptureRangeUnder;
                                if(UN150Read[i]<(dTempWorkBase-iTemptureRange))
                                {
                                    flagBellowOk=false;
                                    pos=i;
                                }
                            }
                            else if(i==tc2D)                                    //wei 20161021 Barcode 超過50度Alarm
                            {
                                iTemptureRange=50;
                                if(UN150Read[i]>iTemptureRange)
                                {
                                    flagOverOk=false;
                                    pos1=i;
                                }
                            }
                            else if(UN150Read[i]<(dTempWorkBase-dTemptureRangeBelow))
                            {
                                flagBellowOk=false;
                                pos=i;
                            }
                        }
                    }
                }
            }

            if(flagOverTenCutHeat==false)
                bHeatOverTenErrorOK=true;                                       //可以啟動加熱電源  kevin   20130226
            else
                bHeatOverTenErrorOK=false;

            if(flagOverOk && flagBellowOk && fHeaterOK==false)                  //kevin 20130222 加熱剛完完成
            {                                                                   //ChungHung 20140519 add Chamber Mode Even Blow need Wait Initial Wait time in Temp_Set
                if(bHPCleanout)                                                 //wei 20160624 Hotplate clean out
                {
                    iHeaterWait=0;
                    iHeaterCooling=0;
                }
                else if(bFirstEnter==true ||
                        (IniConfig.bL15EnableChamberModeEvenBlowNeedWaitTime &&
                         Temperature.iIndexHeatMode!=HeadOnly &&
                         Temperature.iIndexHeatMode!=HeadSocket))               //ChungHung 20140523 alter just without no use chamber
                {
                    iHeaterWait=LastSet.InitialWaitTime;
                    iHeaterCooling=Temperature.fAbitColdTime;
                }
                else
                {
                    iHeaterWait=0;
                    iHeaterCooling=0;
                }
            }

            if(IniConfig.bL28TempOfsUseReadyTempRange &&
               bEnable_KLT_Function==false &&
               bNeedReCheckHeat==true)                                          //Sam 20231214 : Temp offset use ready temp range
            {
                bNeedReCheckHeat=false;
            }
            else
            {
                if((flagOverOk && flagBellowOk) ||                              // 加熱完成
                   bSendChangeTempDelayAlarm)                                   //kevin 20151023 Auto Temp offset
                {
                    fHeaterOK=true;
                    fHeaterOK_Backup=true;                                      //jou 2010-10-11
                    bHeatOKBellowError=true;                                    //kevin 20131230  溫度到達偵測低溫alarm

                    bFirstEnter=false;
                    if(fiosetview->fShow==false)                                //kevin 20141007
                    {
                        SW[SwShuttleCooling].Off();                             //jou 2010-06-09 start
                        SW[SwDutHeaterCoolFan].Off();                           //jou 2010-12-07 start
                    }
                    iAlarmSecondHeatOK=0;
                    iAlarmSecond=0;
                    iAlarmSecond1=0;
                    if(iTemperatureOkFirstTime==false || iTemperatureOk!=0)     //kevin 20150914
                    {
                        iTemperatureOkFirstTime=true;
                        iTemperatureOk=0;
                        NewRecordProcess("MES2131", "Temperature OK", "CheckHeater 4");   //kevin 20150914 溫度到達
                        RecordProcess("Working Temperature Arrived Done");      //kevin 20200829  加熱完成
                    }

                    if(CosFunction.bTempHeaterOkShowLight)                      //jou 20180529 : Temperature Heater Ok Show Light
                    {
                        if(bEnableTempHeaterOkShowLight==true)
                        {
                            bEnableTempHeaterOkShowLight=false;
                            bTempHeaterOkShowLightFlag=true;
                        }
                    }

                    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60 &&
                       Temperature.bBoostFuncttion==true)
                    {
                        if(bFlagBelowTurnOfValve1)
                        {
                            fTemp_Set->ControlATC60AirFlow(0);
                        }
                        bFlagBelowTurnOfValve1=false;
                    }
                    bTJtoTCWait=false;                                          //wei 20251002 : TJ轉回TC等待
                }
                else if(flagOverOk==false || flagBellowOk==false)               // 加熱完成,進入加熱完成等待秒數計數
                {
                    if(bUseInitTempOffset &&
                       IniConfig.bL28TempOfsUseReadyTempRange &&
                       bEnable_KLT_Function==false &&
                       iInitContactCount<(Temperature.iCintactCntForTempOffsetAtInitial+Temperature.iCintactDelayCntForInitTempOffset))  //Sam 20231214 : Temp offset use ready temp range
                    {

                    }
                    else
                    {
                        if(fHeaterOK && iSec!=SystemSec)
                        {
                            iSec=SystemSec;
                            iAlarmSecondHeatOK++;
                            bHeatOverStop=true;                                 //heat 過熱
                            iAlarmSecond=0;
                            iAlarmSecond1=0;

                            if(bTJtoTCWait)                                     //wei 20251002 : TJ轉回TC等待
                                iTempSec=60;
                            else
                                iTempSec=30;

                            if(iHome==0 && iAlarmSecondHeatOK>iTempSec ||
                               iHome!=0 && iAlarmSecondHeatOK>150)
                            {
                                iAlarmSecondHeatOK=0;
                                if(flagOverTenCutHeat &&
                                   IniConfig.bL12TempErrNoCloseHeater==false)   //kevin 20140401 add IniConfig.bL12TempErrNoCloseHeater==false//ChungHung 20120913 SCK  要求Temp Error 不要關閉加熱// if(flagOver==false)     //kevin 20130218
                                {
                                    SW[SwHeaterRelay].Off();                    //溫度過高超過10度
                                    S2.sprintf("CheckHeater 5 (%s)", asTempCtrl[pos1]);
                                    HeaterLog(S2, false);                       //Steven 20151123 : Log for Heater Relay
                                }
                                SW[SwDutHeaterCoolFan].OnOff(HasAreaOverAmbientTemp());

                                if(flagOverOk==false)                           //溫度過高
                                {
                                    fHeaterOK=false;
                                    S1.sprintf("WAR15%02d", pos1+100);          //Steven 20140422 : 重新整理
                                    S2.sprintf("CheckHeater 5 (%s)", asTempCtrl[pos1]);
                                    ShowErrorMessage(S1, K_RETRY, MMSystem);    //kevin 20130218
                                    iTemperatureOk=1;                           //kevin 20150914
                                    NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                }

                                if(flagBellowOk==false &&                       //Steven 20110317 : 溫度過低只叫一次
                                   SystemStart==true)
                                {
                                    fHeaterOK=false;
                                    if(bUseInitTempOffset &&                    //Steven 20180820 : 簡化init temp offset判斷式     //Steven 20141117 : 起測時溫度要補Offset
                                       iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)  //Steven 20141125 : <= --> <
                                    {
                                        ;
                                    }
                                    else if((Temperature.bBoostFuncttion ||
                                             Temperature.bLBTempFunction) &&
                                            iTriggerBoostFunction!=-1)          //Steven 20180817 : Boost Function
                                    {

                                    }
                                    else
                                    {
                                        S1.sprintf("WAR15%02d", pos);           //Steven 20140422 : 重新整理
                                        S2.sprintf("CheckHeater 6 (%s)", asTempCtrl[pos]);
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);//kevin 20130218
                                        iTemperatureOk=1;                       //kevin 20150914
                                        NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                    }
                                }
                            }

                            if(bSendChangeTempDelayAlarm==false)                //kevin 20151023 Auto Temp offset
                                fHeaterOK=false;                                //kevin 20131024
                        }
                        else
                        {
                            if(fHeaterOK==false && iSec!=SystemSec)             //kevin 20130220
                            {
                                iAlarmSecondHeatOK=0;
                                iSec=SystemSec;
                                if(flagOverOk==false)
                                    iAlarmSecond++;

                                if(flagBellowOk==false)
                                    iAlarmSecond1++;
                                else                                            //JerryYang 20181002 (Steven) : fix誤發alarm
                                    iAlarmSecond1=0;

                                if(flagOverTenCutHeat &&                        //ChungHung 20120913 SCK  要求Temp Error 不要關閉加熱// if(flagOver==false)     //kevin 20130218
                                   IniConfig.bL12TempErrNoCloseHeater==false)   //kevin 20140401 add IniConfig.bL12TempErrNoCloseHeater==false
                                {
                                    SW[SwHeaterRelay].Off();                    //溫度過高超過10 度 關閉加熱電源
                                    S2.sprintf("CheckHeater 7 (%s)", asTempCtrl[pos1]);
                                    HeaterLog(S2, false);                       //Steven 20151123 : Log for Heater Relay
                                }

                                if(bSendChangeTempDelayAlarm &&
                                   bSendChangeTempError==false)                 //kevin 20151023
                                    iTempSec=150;
                                else if(bTJtoTCWait)                            //wei 20251002 : TJ轉回TC等待
                                    iTempSec=60;
                                else
                                    iTempSec=30;

                                if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60 &&
                                   Temperature.bBoostFuncttion==true)
                                {
                                    if(flagOverOk==false && bFlagBelowTurnOfValve1==false)
                                    {
                                        bFlagBelowTurnOfValve1=true;
                                        fTemp_Set->ControlATC60AirFlow(1);
                                    }
                                    if(flagOverOk && flagBellowOk==false)
                                    {
                                        fTemp_Set->ControlATC60AirFlow(0);
                                    }
                                }

                                if((iHome==0 &&
                                    (flagOverOk==false && iAlarmSecond>iTempSec ||
                                    (flagOverTenCutHeat && iAlarmSecond>iTempSec) ||
                                    (flagOverOk==false && iAlarmSecond>iTempSec && bHeatOKOverError==true))) ||
                                   (iHome!=0 &&
                                    (flagOverOk==false && iAlarmSecond>150)))
                                {                                               //溫度過高    //溫度過高10 度+ offset溫度             剛剛所有溫度到達但有一溫控器過熱需等約20sec(30)
                                    if(iHome!=0 && flagOverOk==false && iAlarmSecond>150)
                                    {                                           //Steven 20241001 : 溫度過高增加紀錄判斷
                                        S2.sprintf("CheckHeater 7 (%s)", asTempCtrl[pos1]);
                                    }
                                    else if(flagOverOk==false && iAlarmSecond>iTempSec)
                                    {
                                        if(bHeatOKOverError==true)
                                            S2.sprintf("CheckHeater 9 (%s)", asTempCtrl[pos1]);
                                        else
                                            S2.sprintf("CheckHeater 10 (%s)", asTempCtrl[pos1]);
                                    }
                                    else if(flagOverTenCutHeat && iAlarmSecond>iTempSec)
                                    {
                                        S2.sprintf("CheckHeater 11 (%s)", asTempCtrl[pos1]);
                                    }

                                    flagOverTenCutHeat=false;                   //Jimmychiu 20230707 : 關閉一段時間後開啟加熱relay
                                    iAlarmSecond=0;
                                    bHeatOKOverError=false;

                                    if((Temperature.bBoostFuncttion ||
                                        Temperature.bLBTempFunction) &&         //Steven 20180817 : Boost Function
                                       (iTriggerBoostFunction!=-1 ||
                                        iTriggerBoostFuncBack!=-1 ||
                                        Zteach->fShow))                         //kevin 20210323 addZteach->fShow
                                    {

                                    }
                                    else
                                    {
                                        S1.sprintf("WAR15%02d", pos1+100);      //Steven 20140422 : 重新整理
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);//kevin 20130218
                                        SW[SwDutHeaterCoolFan].OnOff(HasAreaOverAmbientTemp());
                                        iTemperatureOk=1;                       //kevin 20150914
                                        NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                    }
                                }

                                if(SystemStart==false || iHome!=0)              //Pause 狀態不偵測低溫, Home不偵測低溫
                                    iAlarmSecond1=0;

                                if(bUseInitTempOffset   &&                      //Steven 20141117 : 起測時溫度要補Offset
                                   iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)  //Steven 20141125 : <= --> <
                                {
                                    iAlarmSecond1=0;                            //JerryYang 20181002 (Steven) : fix誤發alarm
                                }
                                else if(Temperature.bLBTempFunction &&          //Steven 20180817 : Boost Function
                                        iTriggerBoostFunction==Temperature.eBLBI)
                                {
                                    if(flagOverOk==true && flagBellowOk==false)
                                    {
                                        if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60 && bFlagBelowTurnOfValve)
                                        {
                                            bFlagBelowTurnOfValve=false;
                                            fTemp_Set->ControlATC60AirFlow(0);
                                        }
                                    }
                                }
                                else if((Temperature.bBoostFuncttion ||
                                         Temperature.bLBTempFunction) &&
                                        iTriggerBoostFunction!=-1)              //Steven 20180817 : Boost Function
                                {
                                    ;
                                }
                                else
                                {
                                    if(SystemStart &&
                                       flagBellowOk==false &&
                                       iAlarmSecond1>620 ||
                                      (bHeatOKBellowError==true &&
                                       flagBellowOk==false &&
                                       iAlarmSecond1>30))
                                    {                                           //機台run 時才偵測低溫 約10 分鐘發alarm剛剛所有溫度到達但有一溫控器溫度過低需等約20sec(30) 發alarm
                                        bHeatOKBellowError=false;               //kevin 20141006  需等加熱完成清除
                                        iAlarmSecond1=0;
                                        S1.sprintf("WAR15%02d", pos);           //Steven 20140422 : 重新整理
                                        S2.sprintf("CheckHeater 8 (%s)", asTempCtrl[pos]);
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);//kevin 20130218
                                        iTemperatureOk=1;                       //kevin 20150914
                                        NewRecordProcess("MES2130", "Temperature Wait", S2);   //kevin 20150914 溫度尚未到達
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if(IniConfig.bTemp25degControl==true ||                             //jou 2014-06-07 Temperature 25 deg. control
               LastSet.iTemperature==Tempture_AmbientHot)                       //kevin 20140918 恆溫控制
            {
                if((LastSet.iTemperature==Tempture_AmbientHot ||
                    (IniConfig.bTemp25degControl==true &&
                     Temperature.iIndexHeatMode==HeadOnly)) &&
                   Temperature.fWorkTemperBase<=25.0)
                {
                    if(fiosetview->fShow==false)
                    {
                        if(bShuttleCooling==true)
                            SW[SwShuttleCooling].On();
                        else
                            SW[SwShuttleCooling].Off();

                        if(bChamboCooling)                                      //kevin 20130109
                            SW[SwDutHeaterCoolFan].On();
                        else
                            SW[SwDutHeaterCoolFan].Off();
                    }
                }
                else
                {
                    SW[SwShuttleCooling].Off();                                 //jou 2010-06-09 start
                    SW[SwDutHeaterCoolFan].Off();                               //jou 2010-12-07 start
                }
            }
            else
            {
                SW[SwShuttleCooling].Off();                                     //jou 2010-06-09 start
                SW[SwDutHeaterCoolFan].Off();                                   //jou 2010-12-07 start
            }
        }
        else if(LastSet.iTemperature==Tempture_Ambient)
        {
            bFirstEnter=true;
            flag=false;
            flagAmbOver=false;
            bLess30deg=true;
            flagOverOk=true;
            flagBellowOk=true;

            for(int i=0; i<tcTotalCount; i++)
            {
                bIsTJControl=false;                                             //Ifor 20190328 : add TJ Temp Over Range
                if(ATC_SYSTEM==eNewATCSystem &&
                   Temperature.bATCActiveCooling==true &&
                   (bTJControlMode==true ||
                   (Temperature.bEnableTJFunction && bHasTjTemp==true && (bArm1IsTest==true || bArm2IsTest==true))))  //JerryYang 20251124 : Tj control吃不同的溫度range
                {
                    if((i>=tcAa1 && i<=tcBd2) ||
                       (i>=tcAe1 && i<=tcBh2))
                    {
                        bIsTJControl=true;
                    }
                }
                //----- by dell ccd realtime-------------Steven 20110811
                if(i==tcCCD || i==tcCCD_2)                                      //Isaac 20201217 : RTC CCD增加第二組感溫
                    continue;
                //---------------------------------------
                if(bUT150Install[i] && bGetHeaterUsed(i))
                {
                    if(ATC_SYSTEM==eMikros)                                     //Ifor 20231027 add: Mikros ATC System不監控Index 溫度
                    {
                        if((i>=tcAa1 && i<=tcBd2) ||
                           (i>=tcAe1 && i<=tcBh2))
                        {
                            continue;
                        }
                    }
                    else if(CosFunction.bATCModeNoUseCheckHeaterFunction==true &&
                            Temperature.bATCActiveCooling==true)
                    {
                        if((i>=tcAa1 && i<=tcBd2) ||
                           (i>=tcAe1 && i<=tcBh2))
                        {
                            continue;
                        }
                    }

                    if(CosFunction.bMonitorLBonAmbient)
                    {
                        if(i==tcLBUp || i==tcLBDown)                                //Frank 20241231 : add
                        {
                            if(Temperature.bLBTempHighAlarm_Enable==true &&
                               UN150Read[i]>Temperature.dLBTempHighSettingValue)
                            {
                                flagOverOk=false;
                                pos1=i;
                            }
                            else if(Temperature.bLBTempLowAlarm_Enable==true &&
                                    UN150Read[i]<Temperature.dLBTempLowSettingValue)
                            {
                                flagBellowOk=false;
                                pos1=i;
                            }
                        }
                    }
                    if(bIsTJControl==true)
                    {
                        if(UN150Read[i]>(dTempWorkBase+Temperature.dTJTempRange_High))
                        {
                            flagAmbOver=true;
                            pos1=i;
                            break;
                        }
                    }
                    else if(bChangeTest_TempOffset!=0 &&                        //Steven 20250414 : Fixed for heater not ok but handler run
                            bChangeTest_TempAlarm==true)                        //Ifor 20210623 add: Test Temp Change
                    {

                    }
                    else
                    {
                        if(Temperature.bUseAbitCHK &&
                           Temperature.iAbitCHKStatus==1)                       //Running check
                        {
                            if(i==tcShuttle1 || i==tcShuttle2)                  //shuttle 1 & 2
                            {
                                if(bShuttleCooling==true)
                                {
                                    if(UN150Read[tcShuttle1]<Temperature.fAbitTemp-3 &&
                                       UN150Read[tcShuttle2]<Temperature.fAbitTemp-3) //常溫溫度降溫低於3 度才停止吹氣
                                        bShuttleCooling=false;
                                }
                                else
                                {
                                    if(UN150Read[i]>Temperature.fAbitTemp-1)    //常溫溫度高 於1 度才開始吹氣
                                        bShuttleCooling=true;
                                }
                            }
                            else if(i==tcChamber)
                            {
                                if(bChamboCooling)
                                {
                                    if(CUSTOMER_CODE==CC_KYEC_LEE)              //Ifor 20180302 (wei) : KYEC 喬智要求Chambo Cooling開啟/關閉時機
                                    {
                                        if(UN150Read[i]<Temperature.fAbitTemp+1)
                                            bChamboCooling=false;
                                    }
                                    else
                                    {
                                        if(UN150Read[i]<Temperature.fAbitTemp-3)
                                            bChamboCooling=false;
                                    }
                                }
                                else
                                {
                                    if(CUSTOMER_CODE==CC_KYEC_LEE)              //Ifor 20180302 (wei) : KYEC 喬智要求Chambo Cooling開啟/關閉時機
                                    {
                                        if(UN150Read[i]>Temperature.fAbitTemp+2)
                                            bChamboCooling=true;
                                    }
                                    else
                                    {
                                        if(UN150Read[i]>Temperature.fAbitTemp-1)
                                            bChamboCooling=true;
                                    }
                                }
                            }
                            //----------------kevin 20130226

                            if(UN150Read[i]>(Temperature.fAbitTemp+IniConfig.iAmbTemperatureRange))
                            {
                                if((TestIF.iTestMode==_8Site2X4 ||
                                    TestIF.iTestMode==_16Site4X4) &&            //Sam 20190226 : 16Site4X4
                                   TestIF_File.bOctal_12Kit &&                  //kevin 20150624 12Site 使用8site 溫度不秀ALARM
                                   (i==tcAd1 || i==tcBd1 || i==tcAd2 || i==tcBd2))
                                    continue;

                                if(CosFunction.bTempLess30degShowLight)         //jou 2014-12-19 Temperature Less 30 deg. Show Light
                                {
                                    bLess30deg=false;
                                }

                                if(AMBIENT_TEMP_CHECK[i]==true)                 //Steven 20100831
                                {
                                    #ifndef SOFT_SIMULTE
                                    if(SystemStart && iHome==0)
                                    {
                                        if((ATC_SYSTEM==eATCHonPrecType ||
                                            ATC_SYSTEM==eNewATCSystem) &&
                                           (Temperature.bATCActiveCooling==true ||
                                            Temperature.bATC70Active==true))    //Ifor 20160817 add 判斷是否於ATC模式且開啟ATC功能
                                        {
                                            if((i>=tcAa1 && i<=tcBd2) ||
                                               (i>=tcAe1 && i<=tcBh2))
                                            {

                                            }
                                            else if(INSTALL_HEAT_GUN>0 && Temperature.bActiveHeatGun && (i==tcHeatGun1 || i==tcHeatGun2))
                                            {
                                                if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 ||
                                                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35 ||
                                                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61 )
                                                {
                                                     if(UN150Read[i]>Temperature.dATC_HotGunTemp+20)
                                                     {
                                                        flagAmbOver=true;
                                                     }
                                                }
                                            }
                                            else if(INSTALL_ATC_HEAT_GUN>0 && (i==tcATCHotAir1 || i==tcATCHotAir2))
                                            {
                                                if(UN150Read[i]>IniConfig.dHeatGunTempATC+20)
                                                {
                                                    flagAmbOver=true;
                                                }
                                            }
                                            else
                                            {
                                                flagAmbOver=true;
                                            }
                                        }
                                        else
                                        {
                                            flagAmbOver=true;
                                        }
                                    }
                                    #endif
                                }
                                else
                                {
                                    continue;
                                }
                                flag=true;
                            }
                        }
                        else
                        {
                            if(UN150Read[i]>(Temperature.fAbitTemp+IniConfig.iAmbTemperatureRange))
                            {
                                if(CosFunction.bTempLess30degShowLight)         //jou 2014-12-19 Temperature Less 30 deg. Show Light   //Steven 20150120
                                {
                                    bLess30deg=false;
                                }
                            }

                            bShuttleCooling=false;
                            bChamboCooling=false;                               //kevin 20130109
                        }
                    }
                }
            }

            if(CosFunction.bTempLess30degShowLight)                             //jou 2014-12-19 Temperature Less 30 deg. Show Light
            {
                if(bEnableTempLess30degShowLight==true)
                {
                    if(bLess30deg==true)
                    {
                        if(tHeaterAlarmTimer.LatchCycleTimeSec()>60);
                        {
                            bEnableTempLess30degShowLight=false;
                            bTempLess30degShowLightFlag=true;
                        }
                    }
                }
            }

            if(fiosetview->fShow==false)
            {
                if(bShuttleCooling==true)
                    SW[SwShuttleCooling].On();
                else
                    SW[SwShuttleCooling].Off();

                if(bChamboCooling)                                              //kevin 20130109
                    SW[SwDutHeaterCoolFan].On();
                else
                    SW[SwDutHeaterCoolFan].Off();
            }

            if(flagAmbOver==true ||                                             //kevin 20131011打開 常溫過高發alarm   kevin 20130430 ???
               (flagOverOk==false || flagBellowOk==false))                      //Jimmychiu 20251030 : Monitor the LB temperature under ambient temperature
            {
                if(iSec!=SystemSec)
                {
                    iSec=SystemSec;
                    iAlarmSecond++;
                    if(iAlarmSecond>IniConfig.iTempeAlarmSecond_Over)           //Steven 20111027
                    {
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
                           LastSet.iTester==0)                                  //kevin 20210730 Autohigh off line 不偵測溫度
                        {
                            iAlarmSecond=0;                                     //jou 20171024 (wei) : 修正長溫溫度alarm後沒有間隔時間,會持續alarm無法操作
                        }
                        else
                        {
                            if(Zteach->fShow==false)                            //kevin 20210310 add in out arm Teach mode
                            {
                                if(pos1==tcLBUp || pos1==tcLBDown)
                                {
                                    if(flagOverOk==false)
                                    {
                                        S1.sprintf("WAR15%02d", pos1+100);          //Steven 20140422 : 重新整理
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);    //kevin 20130218
                                    }
                                    else if(flagBellowOk==false)
                                    {
                                        S1.sprintf("WAR15%02d", pos1);           //Steven 20140422 : 重新整理
                                        ShowErrorMessage(S1, K_RETRY, MMSystem);//kevin 20130218
                                    }
                                }
                                else if(flagAmbOver)
                                {
                                    ShowErrorMessage("WAR15182", 0, MMSystem);
                                }
                            }
                        }
                        iAlarmSecond=0;                                         //jou 20171024 (wei) : 修正長溫溫度alarm後沒有間隔時間,會持續alarm無法操作
                    }
                }
            }
            else
            {
                iAlarmSecond=0;
            }
            fHeaterOK=flag;
        }
    }

    if(IniConfig.bL24HeaterStableTime)                                          //JerryYang 20210122 : ASE-CL新增待溫功能
    {
        if(bLastHeatOK!=fHeaterOK)                                              //狀態改變
        {
            if(fHeaterOK==true)
            {
                iStableTime=IniConfig.iL24HeaterStableTime;
                tHeaterStableTimer.LatchCycleTime(true);
            }
            fHeaterStableOK=false;
            bLastHeatOK=fHeaterOK;
        }
        else
        {
            if(iStableTime>0)
                iStableTime=IniConfig.iL24HeaterStableTime-tHeaterStableTimer.LatchCycleTimeSec();

            if(fHeaterOK==true)
            {
                int iTime=0;
                iTime=tHeaterStableTimer.LatchCycleTimeSec();
                if(iTime>=IniConfig.iL24HeaterStableTime)
                {
                    fHeaterStableOK=true;
                }
                else
                {
                    fHeaterStableOK=false;
                }
            }
            else
            {
                fHeaterStableOK=false;
            }
        }
    }
    else
    {
        fHeaterStableOK=true;
    }
//    #endif
}
//******************************************************************************
//
//  注意!! THeaterThread為Handler 溫度控制相關, 修改時要小心!!
//
//******************************************************************************
