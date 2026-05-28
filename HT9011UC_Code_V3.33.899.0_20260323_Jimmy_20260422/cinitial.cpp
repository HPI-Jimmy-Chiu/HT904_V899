#include "MachineDefine.h"
#pragma hdrstop

#include "cinitial.h"

#include "InOutArmZteach.h"
#include "MachineType.h"
#include "cprod.h"
#include "database.h"
#include "cmydef.h"
#include "aArmHeader.h"
#include "MyKitSuck.h"
#include "main.h"
#include "rs232.h"
#include "csystem.h"
//#include "math.h"
#include "cUnitConvert.h"
#include "MyMotor.h"
#include "mysensor.h"
#include "mycylin.h"
#include "myswitch.h"
#include "cSocket.h"
#include "cOffSet.h"
#include "mymessbox.h"
#include "cMyDB.h"
#include "cpublic.h"
#include "common.h"
#include "uShowMessage.h"
#include "LtcSensor.h"
#include "cContact.h"
//#include "AutoClean_ASEKH.h"
#include "mySYNTEKmotor.h"
#include "mySMCmotor.h"
#include "myMN200motor.h"
#include "myGALILmotor.h"                                                       //AI(general) 20260316 (RogerYang) : add Galil HTMotor class declaration for index-axis initialization.
#include "fRotate.h"
#include "AutoClean.h"
#include "myio.h"                                                               // 2015.01.15 , Joye , Safe door check
#include "iosetview.h"
#include "ShuttleMove.h"
#include "SCK_ART.h"
#include "TrayStepMotor.h"
//#include "HTMC88X1Motor.h"                                                    //Jimmychiu 20220926 : ADD MC88X1
#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
#include "cSortCT.h"
#include "acarry.h"
#include "aArmHeader.h"
#include "HTEditList.h"
#include "AutoAlignment.h"
#include "uteach.h"
#include "MyPLC_IO_Modbus.h"
#include "ModbusTCPClient.h"
//#include "SmartSetup.h"
#include "HS_Function.h"
#include "ProductionInfo.h"
#include "MyLaneIo.h"
#include "note.h"
#include "cSiteUseManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//==============================================================================
extern int iBackInArmHotCount;
extern int iHotCount;                                                           //   用以記錄同一次 Shuttle 的IC
extern int iHotPlateCount[2][50][50];                                           //   plate 上的 iHotCount 值
extern int iHotInArmOrder[2][50][50];                                           //   用以記錄 InArm 的前後組吸嘴
extern int iHotWhichKit[2][50][50];                                             //   plate 上的 IC 預備要放到那一個 shuttle 左,或右組 Kit
extern int iHotWhichShuttle[2][50][50];                                         //   plate 上的 IC 預備要放到那一個 shuttle
int iMotorRate[60];

bool bLoadMotorParameter=false;
bool CheckBinSet();
int iShuttleTempPos[2][2]={{0, 0}, {0, 0}};                                     //jou 980928 start : High tempture 130 deg Position shift // [0][0] = Shuttle 1 Left  ;  [0][1] = Shuttle 1 Right
//------------------------------------------------------------------------------
int GetSHCHKPos(int iSite, int iCenterBase)
{
    int Pos=0;
    int iSiteXPitch     = iUnitMultiply100(TestIF_File.dSiteXPitch);
    int iSiteShiftXPitch= iUnitMultiply100(TestIF_File.dSiteXCenterPitch);

    if(TestIF_File.iTestMode==SingleSite ||                                     //Steven 20160919 : for 1x1偏心要跑Z向Latch
       TestIF_File.iTestMode==DualSite2x1)                                      //jou 980827 start : use 14x14 ; Pitch X=40mm Y=60mm ; site 3 is tech point
    {
        Pos=iCenterBase;
    }
    else if(CUSTOMER_CODE==CC_TSMC_TAINAN && TestIF_File.bQualSite2X2Shift)     //wei 20160226 TSMC X Shift
    {
        iSiteShiftXPitch=iUnitMultiply100(TestIF_File.dShiftXPitch);
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch/2-iSiteShiftXPitch;                 //Line 1    //wei 20160922 +  --->  -
                break;
            case 1:
                Pos=iCenterBase-iSiteXPitch/2-iSiteShiftXPitch;                 //Line 2    //wei 20160922 +  --->  -
                break;
        }
    }
    else if(TestIF_File.iTestMode==DualSite     ||                              //1x2 & 2X2 & 2x1 BusyShuttle
            TestIF_File.iTestMode==QualSite2X2  ||
            TestIF_File.iTestMode==QualSite2X2N)                                //Frank 20200520 2X2NN Mode
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch/2;                                  //Line 1
                break;
            case 1:
                Pos=iCenterBase-iSiteXPitch/2;                                  //Line 2
                break;
        }
    }
    else if(TestIF_File.iTestMode==TriSite1X3 ||
            TestIF_File.iTestMode==_6Site2X3  ||                                //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch;
                break;
            case 1:
                Pos=iCenterBase;
                break;
            case 2:
                Pos=iCenterBase-iSiteXPitch;
                break;
        }
    }
    else if(TestIF_File.iTestMode==QualSite1X4  ||                              //1x4 & 2X4
            TestIF_File.iTestMode==_8Site1X4    ||                              //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF_File.iTestMode==_8Site2X4    ||
            TestIF_File.iTestMode==_8Site2X4N   ||                              //Wei 20231211 : 2X4NN Mode
            TestIF_File.iTestMode==_16Site4X4)                                  //Sam 20190226 : 16Site4X4
    {
        if(CosFunction.b2x4SupportCenterPitch &&                                //Richard 20220929 :Add for UTAC
           TestIF_File.bEnableUseXCenterPitch==true)
        {
            switch(iSite)
            {
                case 0:
                    Pos=iCenterBase+iSiteShiftXPitch/2+iSiteXPitch;             //Line 1
                    break;
                case 1:
                    Pos=iCenterBase+iSiteShiftXPitch/2;                         //Line 2
                    break;
                case 2:
                    Pos=iCenterBase-iSiteShiftXPitch/2;                         //Line 3
                    break;
                case 3:
                    Pos=iCenterBase-iSiteShiftXPitch/2-iSiteXPitch;             //Line 4
                    break;
            }
        }
        else
        {
            switch(iSite)
            {
                case 0:
                    Pos=iCenterBase+iSiteXPitch*1.5;                            //Line 1
                    break;
                case 1:
                    Pos=iCenterBase+iSiteXPitch/2;                              //Line 2
                    break;
                case 2:
                    Pos=iCenterBase-iSiteXPitch/2;                              //Line 3
                    break;
                case 3:
                    Pos=iCenterBase-iSiteXPitch*1.5;                            //Line 4
                    break;
            }
        }
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //2X5         //wei 20190614 10 site
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch*2;
                break;
            case 1:
                Pos=iCenterBase+iSiteXPitch;
                break;
            case 2:
                Pos=iCenterBase;
                break;
            case 3:
                Pos=iCenterBase-iSiteXPitch;
                break;
            case 4:
                Pos=iCenterBase-iSiteXPitch*2;
                break;
        }
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //2X6        //Steven 20130702 : _12Site2X6
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch*2.5;                                //Line 1
                break;
            case 1:
                Pos=iCenterBase+iSiteXPitch*1.5;                                //Line 2
                break;
            case 2:
                Pos=iCenterBase+iSiteXPitch/2;                                  //Line 3
                break;
            case 3:
                Pos=iCenterBase-iSiteXPitch/2;                                  //Line 4
                break;
            case 4:
                Pos=iCenterBase-iSiteXPitch*1.5;                                //Line 5
                break;
            case 5:
                Pos=iCenterBase-iSiteXPitch*2.5;                                //Line 6
                break;
        }
    }
    else if(TestIF_File.iTestMode==_16Site2X8   ||                              //2X8
            TestIF_File.iTestMode==_32Site4X8N  ||                              //Steven 20140813 : 32Site
            TestIF_File.iTestMode==_32Site4X8M)
    {
        switch(iSite)
        {
            case 0:
                Pos=iCenterBase+iSiteXPitch*3.5;                                //Line 1
                break;
            case 1:
                Pos=iCenterBase+iSiteXPitch*2.5;                                //Line 2
                break;
            case 2:
                Pos=iCenterBase+iSiteXPitch*1.5;                                //Line 3
                break;
            case 3:
                Pos=iCenterBase+iSiteXPitch/2;                                  //Line 4
                break;
            case 4:
                Pos=iCenterBase-iSiteXPitch/2;                                  //Line 5
                break;
            case 5:
                Pos=iCenterBase-iSiteXPitch*1.5;                                //Line 6
                break;
            case 6:
                Pos=iCenterBase-iSiteXPitch*2.5;                                //Line 7
                break;
            case 7:
                Pos=iCenterBase-iSiteXPitch*3.5;                                //Line 8
                break;
        }
    }

    return Pos;
}
//---------------------------------------------------------------------------
void InitialSuckerName()
{
    AnsiString str1, str2, strOn1, strOn2, strOff1, strOff2;
    for(int i=0; i<MAX_Index_Row; i++)                                          //Eliot 2009_12_27 2->MAX_Index_Row
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)                                  //Eliot 2009_12_27 4->MAX_Index_Col
        {
            str1.sprintf("FTestSuck%c%c", 'A'+i, 'A'+j);
            str2.sprintf("BTestSuck%c%c", 'A'+i, 'A'+j);

            FTestSuck.Suck[i][j].SuckerName         =str1;
            BTestSuck.Suck[i][j].SuckerName         =str2;
            FTestSuckBackup.Suck[i][j].SuckerName   =str1;
            BTestSuckBackup.Suck[i][j].SuckerName   =str2;
        }
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            str1.sprintf("TestSocket%c%c", 'A'+i, 'A'+j);
            TestSocket.Suck[i][j].SuckerName=str1;

            str1.sprintf("%c%c", 'A'+i, 'a'+j);
            TestSocket.Suck[i][j].sName=str1;
        }
    }

    for(int i=0; i<FLCarryKit.iMaxRow; i++)
    {
        for(int j=0; j<FLCarryKit.iMaxCol; j++)
        {
            str1.sprintf("FLCarryKit%c%c", 'A'+i, 'A'+j);
            FLCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("FRCarryKit%c%c", 'A'+i, 'A'+j);
            FRCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("BLCarryKit%c%c", 'A'+i, 'A'+j);
            BLCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("BRCarryKit%c%c", 'A'+i, 'A'+j);
            BRCarryKit.Suck[i][j].SuckerName=str1;
            str1.sprintf("OutSht3Kit%c%c", 'A'+i, 'A'+j);                       //Steven 20240822 : For HT-9046AU
            OutSht3Kit.Suck[i][j].SuckerName=str1;

            str1.sprintf("%c%c", 'A'+i, 'a'+j);
            FLCarryKit.Suck[i][j].sName=str1;
            FRCarryKit.Suck[i][j].sName=str1;
            BLCarryKit.Suck[i][j].sName=str1;
            BRCarryKit.Suck[i][j].sName=str1;
            OutSht3Kit.Suck[i][j].sName=str1;                                   //Steven 20240822 : For HT-9046AU
        }
    }

    if(USE_PICKER_COUNT!=ep16Picker)
    {
        for(int j=0; j<4; j++)
        {
            for(int i=0; i<2; i++)
            {
                InArmSuck.Suck[i][j].SuckerName         ="InArmSuck"+AnsiString(char ('A'+(i+2*j)));
                OutArmSuck.Suck[i][j].SuckerName        ="OutArmSuck"+AnsiString(char ('A'+(i+2*j)));
                InArmSuckBackup.Suck[i][j].SuckerName   =InArmSuck.Suck[i][j].SuckerName;
                OutArmSuckBackup.Suck[i][j].SuckerName  =OutArmSuck.Suck[i][j].SuckerName;
            }
        }
    }
    else if(USE_PICKER_COUNT==ep16Picker)
    {
        for(int i=0; i<2; i++)                                                  //Eliot 2009_12_27 2->MAX_Index_Row
        {
            for(int j=0; j<8; j++)                                              //Eliot 2009_12_27 4->MAX_Index_Col
            {
                str1.sprintf("InArmSuck%c%c", 'A'+i, 'A'+j);
                str2.sprintf("OutArmSuck%c%c", 'A'+i, 'A'+j);

                InArmSuck.Suck[i][j].SuckerName         =str1;
                OutArmSuck.Suck[i][j].SuckerName        =str2;
                InArmSuckBackup.Suck[i][j].SuckerName   =str1;
                OutArmSuckBackup.Suck[i][j].SuckerName  =str2;
            }
        }
    }

    for(int i=0; i<MAX_OutArm3_Row; i++)                                        //Steven 20240822 : For HT-9046AU
    {
        for(int j=0; j<MAX_OutArm3_Col; j++)
        {
            str1.sprintf("OutArm2Suck%c%c", 'A'+i, 'A'+j);
            str2.sprintf("%c%c", 'A'+i, 'a'+j);
            OutArm2Suck.Suck[i][j].SuckerName=str1;
            OutArm2Suck.Suck[i][j].sName=str2;
        }
    }

    CatchTraySuck.Suck[0][0].SuckerName="CatchSuck";
    CheckKitSuck.Suck[0][0].SuckerName ="CheckKitSuck_1";                       //20111130  Dell
    CheckKitSuck.Suck[0][1].SuckerName ="CheckKitSuck_2";                       //20111130  Dell
}
//------------------------------------------------------------------------------
void InitSucker()
{
    if(pSuck==NULL)
        pSuck=new TList;                                                        //Steven 20230701 : 吸嘴改用List方式, 避免例外

    int iSu=0, iSuOn=0, iSuOff=0;
    bool flag;
    bool bHasSuckSen, bHasSuckOn, bHasSuckOff;                                  //Stevem 20210625 : 修正新的CSV IO表讀檔方式
    AnsiString S, sDBDir;
    TTable *T;

    InitialSuckerName();

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            pSuck->Add(&InArmSuck.Suck[i][j]);
            pSuck->Add(&OutArmSuck.Suck[i][j]);
        }
    }

    for(int i=0; i<MAX_Index_Row; i++)                                          //Steven 20230808 : 修正Suck數量錯誤, 影響8site以上模式
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            pSuck->Add(&FTestSuck.Suck[i][j]);
            pSuck->Add(&BTestSuck.Suck[i][j]);
        }
    }

    for(int i=0; i<MAX_OutArm3_Row; i++)                                        //Steven 20240822 : For HT-9046AU
    {
        for(int j=0; j<MAX_OutArm3_Col; j++)
        {
            pSuck->Add(&OutArm2Suck.Suck[i][j]);
        }
    }

    pSuck->Add(&CatchTraySuck.Suck[0][0]);
    pSuck->Add(&CheckKitSuck.Suck[0][0]);
    pSuck->Add(&CheckKitSuck.Suck[0][1]);

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        for(int i=0; i<pSuck->Count; i++)                                       //Steven 20230701 : 吸嘴改用List方式, 避免例外
        {
            bHasSuckSen=false;
            bHasSuckOn =false;
            bHasSuckOff=false;
            pTempSuck=(TMySucker*)pSuck->Items[i];
            pTempSuck->SensorName   =pTempSuck->SuckerName;
            pTempSuck->OnPortName   =pTempSuck->SensorName+AnsiString("_On");
            pTempSuck->OffPortName  =pTempSuck->SensorName+AnsiString("_Off");

            pTempSuck->Enable=false;
            if(pTempSuck->SuckerName=="")
                continue;

            HSys.mapIOTableIter=HSys.mapIOTable.find(pTempSuck->SuckerName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSuckSen=true;
                iSu=atoi(HSys.mapIOTable[pTempSuck->SuckerName].c_str());
            }
            else
            {
                S.sprintf("Can not find sucker sensor %s", pTempSuck->SuckerName.c_str());
//                continue;
            }

            HSys.mapIOTableIter=HSys.mapIOTable.find(pTempSuck->OnPortName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSuckOn=true;
                iSuOn=atoi(HSys.mapIOTable[pTempSuck->OnPortName].c_str());
            }
            else
            {
                S.sprintf("Can not find sucker on %s", pTempSuck->OnPortName.c_str());
//                continue;
            }

            HSys.mapIOTableIter=HSys.mapIOTable.find(pTempSuck->OffPortName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSuckOff=true;
                iSuOff=atoi(HSys.mapIOTable[pTempSuck->OffPortName].c_str());
            }
            else
            {
                S.sprintf("Can not find sucker off %s", pTempSuck->OffPortName.c_str());
//                continue;
            }

            if(bHasSuckOn)
            {
                pTempSuck->OnUsing     =(HSys.IOTable[iSuOn]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSuOn]->iPort);
                pTempSuck->OnISABase   =HSys.IOTable[iSuOn]->iISABase;
                pTempSuck->OnRing      =HSys.IOTable[iSuOn]->iLane;
                pTempSuck->OnIP        =HSys.IOTable[iSuOn]->iIP;
                pTempSuck->OnPort      =HSys.IOTable[iSuOn]->iPort;
                pTempSuck->OnBit       =HSys.IOTable[iSuOn]->iBit;
                pTempSuck->OnType      =HSys.IOTable[iSuOn]->iInType;
                pTempSuck->ISABase     =pTempSuck->OnISABase;
                pTempSuck->OnEnable    =(pTempSuck->OnUsing!="");
            }

            if(bHasSuckOff)
            {
                pTempSuck->OffUsing    =(HSys.IOTable[iSuOff]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSuOff]->iPort);
                pTempSuck->OffISABase  =HSys.IOTable[iSuOff]->iISABase;
                pTempSuck->OffRing     =HSys.IOTable[iSuOff]->iLane;
                pTempSuck->OffIP       =HSys.IOTable[iSuOff]->iIP;
                pTempSuck->OffPort     =HSys.IOTable[iSuOff]->iPort;
                pTempSuck->OffBit      =HSys.IOTable[iSuOff]->iBit;
                pTempSuck->OffType     =HSys.IOTable[iSuOff]->iInType;
                pTempSuck->ISABase     =pTempSuck->OffISABase;
                pTempSuck->OffEnable   =(pTempSuck->OffUsing!="");
            }

            if(bHasSuckSen)
            {
                pTempSuck->SenUsing    =(HSys.IOTable[iSu]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSu]->iPort);
                pTempSuck->SenISABase  =HSys.IOTable[iSu]->iISABase;
                pTempSuck->SenRing     =HSys.IOTable[iSu]->iLane;
                pTempSuck->SenIP       =HSys.IOTable[iSu]->iIP;
                pTempSuck->SenPort     =HSys.IOTable[iSu]->iPort;
                pTempSuck->SenBit      =HSys.IOTable[iSu]->iBit;
                pTempSuck->SenType     =HSys.IOTable[iSu]->iInType;

                pTempSuck->OnAlarmTime =HSys.IOTable[iSu]->iOnAlarmTime*10;
                pTempSuck->OffAlarmTime=HSys.IOTable[iSu]->iOffAlarmTime*10;
                pTempSuck->OnDelayTime =HSys.IOTable[iSu]->iOnDelayTime*10;
                pTempSuck->OffDelayTime=HSys.IOTable[iSu]->iOffDelayTime*10;
                pTempSuck->ISABase     =pTempSuck->SenISABase;
            }

            #ifdef SOFT_SIMULTE
                pTempSuck->Enable       =false;
                pTempSuck->OnEnable     =false;
                pTempSuck->OffEnable    =false;
                pTempSuck->OnDelayTime =0;
                pTempSuck->OffDelayTime=0;
            #else
            if(pTempSuck->SenUsing=="")                                         //Steven 20230723 : 修正Suck Enable判斷方式
                pTempSuck->Enable=false;
            else if(bHasSuckSen && HSys.IOTable[iSu]->iEnable==1)
                pTempSuck->Enable=true;
            else
                pTempSuck->Enable=false;

            if(pTempSuck->Enable==false)
            {
                pTempSuck->OnEnable =false;
                pTempSuck->OffEnable=false;
            }
            #endif

            pTempSuck->fAlarm    =false;

            if(INDEX_SUCKER_TYPE==1)                                            //負壓
            {
                if(pTempSuck->SensorName.Pos("TestSuck")>0)
                {
                    pTempSuck->OnAlarmTime=pTempSuck->OnAlarmTime/10;           //Steven 20160224 : 因為避免機台INDEX吸不起IC
                    if(pTempSuck->OnAlarmTime>50)
                        pTempSuck->OnAlarmTime=50;
                }
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OnEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OnRing, pTempSuck->OnIP, pTempSuck->OnISABase, pTempSuck->SensorName);             //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OffEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OffRing, pTempSuck->OffIP, pTempSuck->OffISABase, pTempSuck->SensorName);          //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->Enable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->SenRing, pTempSuck->SenIP, pTempSuck->SenISABase, pTempSuck->SensorName, false);   //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }
        }
    }
    else
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true || USE_OUT_Y_IS_AUTO_PITCH)                                              //Steven 20170424 (wei) : new XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {                                                                       //Steven 20140828 : For XY-Pitch
            sDBDir=CurrentDir+"\\system\\sucker_YPitch.db";                     //JerryYang 20150903 檢查Database檔是否存在
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                 ShowMyMessage("sucker_YPitch.db does not exist");
                 return;
            }
        }
        else if(MachineTypeChoice==Type_HT9046 ||
                MachineTypeChoice==Type_HT9046_LS ||
                MachineTypeChoice==Type_HT1032)
        {
            sDBDir=CurrentDir+"\\system\\sucker_9046.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker_9046.db does not exist");
                return;
            }
        }
        else if(USE_46_SUCKER_DB==1)                                            //Steven 20131008 : HT9045使用46配氣
        {
            sDBDir=CurrentDir+"\\system\\sucker_9046.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker_9046.db does not exist");
                return;
            }
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            sDBDir=CurrentDir+"\\system\\sucker.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker.db does not exist");
                return;
            }
        }
        else
        {
            sDBDir=CurrentDir+"\\system\\sucker.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SuckerTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sucker.db does not exist");
                return;
            }
        }

        T=DataModule1->SuckerTable;
        T->Open();
        for(int i=0; i<pSuck->Count; i++)
        {
            flag=false;
            T->First();
            while(!T->Eof)
            {
                S=T->FieldByName("SuckerName")->AsString;
                pTempSuck=(TMySucker*)pSuck->Items[i];
                if(pTempSuck!=NULL && S==pTempSuck->SuckerName)
                {
                    flag=true;
                    break;
                }
                T->Next();
            }
            if(flag==false)
                continue;

            pTempSuck->SensorName   =T->FieldByName("SuckerName")->AsString;
            pTempSuck->OnPortName   =pTempSuck->SensorName+AnsiString("_On");
            pTempSuck->OffPortName  =pTempSuck->SensorName+AnsiString("_Off");
            if(T->FindField("ISABase")!=NULL)
                pTempSuck->ISABase  =atoi(T->FieldByName("ISABase")->AsString.c_str());         //Nickliu 20230310 add Suck ISABase Type
            else
                pTempSuck->ISABase  =eMotionNet;

            pTempSuck->OnISABase    =pTempSuck->ISABase;
            pTempSuck->OffISABase   =pTempSuck->ISABase;
            pTempSuck->SenISABase   =pTempSuck->ISABase;

            #ifdef PCI132
            pTempSuck->OnIP         =atoi(T->FieldByName("OnIP"         )->AsString.c_str());
            if(pTempSuck->OnISABase==eISABase ||                                //Nickliu 20230310 add Suck ISABase Type
               pTempSuck->OnISABase==ePCI1735U ||
               pTempSuck->OnISABase==ePLCbase)
            {
                pTempSuck->OnUsing  =T->FieldByName("OnPort")->AsString;
                pTempSuck->OnPort   =HexStrToInt(T->FieldByName("OnPort")->AsString.c_str());
            }
            else
            {
                pTempSuck->OnUsing  =T->FieldByName("OnLane")->AsString;
                if(pTempSuck->OnUsing!="")
                    pTempSuck->OnRing   =atoi(pTempSuck->OnUsing.c_str());
                pTempSuck->OnPort   =atoi(T->FieldByName("OnPort"       )->AsString.c_str());
            }

            pTempSuck->OnBit        =atoi(T->FieldByName("OnBit"        )->AsString.c_str());
            pTempSuck->OffIP        =atoi(T->FieldByName("OffIP"        )->AsString.c_str());
            if(pTempSuck->OffISABase==eISABase ||                               //Nickliu 20230310 add Suck ISABase Type
               pTempSuck->OffISABase==ePCI1735U ||
               pTempSuck->OffISABase==ePLCbase)
            {
                pTempSuck->OffUsing =T->FieldByName("OffPort"      )->AsString;
                pTempSuck->OffPort  =HexStrToInt(T->FieldByName("OffPort")->AsString.c_str());
            }
            else
            {
                pTempSuck->OffUsing     = T->FieldByName("OffLane"      )->AsString;
                if(pTempSuck->OffUsing!="")
                    pTempSuck->OffRing  =atoi(pTempSuck->OffUsing.c_str());
                pTempSuck->OffPort  =atoi(T->FieldByName("OffPort"      )->AsString.c_str());
            }

            pTempSuck->OffBit       =atoi(T->FieldByName("OffBit"       )->AsString.c_str());

            pTempSuck->SenIP        =atoi(T->FieldByName("SenIP"        )->AsString.c_str());
            if(pTempSuck->OffISABase==eISABase ||                               //Nickliu 20230310 add Suck ISABase Type
               pTempSuck->OffISABase==ePCI1735U ||
               pTempSuck->OffISABase==ePLCbase)
            {
                pTempSuck->SenUsing =T->FieldByName("SenPort"           )->AsString;
                pTempSuck->SenPort  =HexStrToInt(T->FieldByName("SenPort")->AsString.c_str());
            }
            else
            {
                pTempSuck->SenUsing =T->FieldByName("SenLane"           )->AsString;
                if(pTempSuck->SenUsing!="")
                    pTempSuck->SenRing  =atoi(T->FieldByName("SenLane"  )->AsString.c_str());
                pTempSuck->SenPort  =atoi(T->FieldByName("SenPort"      )->AsString.c_str());
            }
            pTempSuck->SenBit       =atoi(T->FieldByName("SenBit"       )->AsString.c_str());
            #else
            pTempSuck->OnIOPort     =HexStrToInt(T->FieldByName("OnIOPort"     )->AsString.c_str());
            pTempSuck->OnIOBit      =atoi(T->FieldByName("OnIOBit"      )->AsString.c_str());
            pTempSuck->OffIOPort    =HexStrToInt(T->FieldByName("OffIOPort"    )->AsString.c_str());
            pTempSuck->OffIOBit     =atoi(T->FieldByName("OffIOBit"     )->AsString.c_str());
            pTempSuck->SensorPort   =HexStrToInt(T->FieldByName("SensorPort"   )->AsString.c_str());
            pTempSuck->SensorBit    =atoi(T->FieldByName("SensorBit"    )->AsString.c_str());
            #endif
            pTempSuck->OnType       =atoi(T->FieldByName("OnType"      )->AsString.c_str());
            pTempSuck->OffType      =atoi(T->FieldByName("OffType"     )->AsString.c_str());
            pTempSuck->SenType      =atoi(T->FieldByName("SensorType"  )->AsString.c_str());
            pTempSuck->OnAlarmTime  =atoi(T->FieldByName("OnAlarmTime" )->AsString.c_str())*10;
            pTempSuck->OffAlarmTime =atoi(T->FieldByName("OffAlarmTime")->AsString.c_str())*10;
            pTempSuck->OnDelayTime  =atoi(T->FieldByName("OnDelayTime" )->AsString.c_str())*10;
            pTempSuck->OffDelayTime =atoi(T->FieldByName("OffDelayTime")->AsString.c_str())*10;
            pTempSuck->fAlarm       =false;

            #ifdef SOFT_SIMULTE
                pTempSuck->Enable       =false;
                pTempSuck->OnEnable     =false;
                pTempSuck->OffEnable    =false;
                pTempSuck->OnDelayTime  =0;
                pTempSuck->OffDelayTime =0;
            #else
            if(pTempSuck->SenUsing=="" ||                                       //Steven 20230723 : 修正Suck Enable判斷方式
               atoi(T->FieldByName("Enable")->AsString.c_str())==0)
                pTempSuck->Enable=false;
            else
                pTempSuck->Enable=true;

            pTempSuck->OnEnable  =pTempSuck->Enable;
            pTempSuck->OffEnable =pTempSuck->Enable;
            #endif

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OnEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OnRing, pTempSuck->OnIP, pTempSuck->OnISABase, pTempSuck->SensorName);             //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->OffEnable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->OffRing, pTempSuck->OffIP, pTempSuck->OffISABase, pTempSuck->SensorName);          //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(pTempSuck->Enable)
            #endif
            {
                MyLaneIO.SetUseIP(pTempSuck->SenRing, pTempSuck->SenIP, pTempSuck->SenISABase, pTempSuck->SensorName, false);   //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            //負壓
            if(INDEX_SUCKER_TYPE==1)
            {
                if(pTempSuck->SensorName.Pos("TestSuck")>0)
                {
                    pTempSuck->OnAlarmTime=pTempSuck->OnAlarmTime/10;           //Steven 20160224 : 因為避免機台INDEX吸不起IC
                    if(pTempSuck->OnAlarmTime>50)
                        pTempSuck->OnAlarmTime=50;
                }
            }
        }
        T->Close();
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            S.sprintf("%c%c", 'A'+i, 'a'+j);
            FTestSuck.Suck[i][j].sName=S;
            FTestSuck.Suck[i][j].iMyRow=i;
            FTestSuck.Suck[i][j].iMyCol=j;
            BTestSuck.Suck[i][j].sName=S;
            BTestSuck.Suck[i][j].iMyRow=i;
            BTestSuck.Suck[i][j].iMyCol=j;
        }
    }

    if(MachineTypeChoice==Type_HT9045)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=4; j<8; j++)
            {
                FTestSuck.Suck[i][j].Enable    =false;
                FTestSuck.Suck[i][j].OnEnable  =false;
                FTestSuck.Suck[i][j].OffEnable =false;
                BTestSuck.Suck[i][j].Enable    =false;
                BTestSuck.Suck[i][j].OnEnable  =false;
                BTestSuck.Suck[i][j].OffEnable =false;
            }
        }
    }
    else if(MachineTypeChoice==Type_HT9045_12Site)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=6; j<8; j++)
            {
                FTestSuck.Suck[i][j].Enable    =false;
                FTestSuck.Suck[i][j].OnEnable  =false;
                FTestSuck.Suck[i][j].OffEnable =false;
                BTestSuck.Suck[i][j].Enable    =false;
                BTestSuck.Suck[i][j].OnEnable  =false;
                BTestSuck.Suck[i][j].OffEnable =false;
            }
        }
    }

    CopyKitSuck(&FTestSuck, &FTestSuckBackup);
    CopyKitSuck(&BTestSuck, &BTestSuckBackup);
    InArmSuck.Suck[0][0].iMotNo=MInArmZA;
    InArmSuck.Suck[1][0].iMotNo=MInArmZB;
    InArmSuck.Suck[0][1].iMotNo=MInArmZC;
    InArmSuck.Suck[1][1].iMotNo=MInArmZD;
    InArmSuck.Suck[0][2].iMotNo=MInArmZE;
    InArmSuck.Suck[1][2].iMotNo=MInArmZF;
    InArmSuck.Suck[0][3].iMotNo=MInArmZG;
    InArmSuck.Suck[1][3].iMotNo=MInArmZH;

    OutArmSuck.Suck[0][0].iMotNo=MOutArmZA;
    OutArmSuck.Suck[1][0].iMotNo=MOutArmZB;
    OutArmSuck.Suck[0][1].iMotNo=MOutArmZC;
    OutArmSuck.Suck[1][1].iMotNo=MOutArmZD;
    OutArmSuck.Suck[0][2].iMotNo=MOutArmZE;
    OutArmSuck.Suck[1][2].iMotNo=MOutArmZF;
    OutArmSuck.Suck[0][3].iMotNo=MOutArmZG;
    OutArmSuck.Suck[1][3].iMotNo=MOutArmZH;

//    if(USE_PICKER_COUNT==ep16Picker &&
//       InOutArmPickerUseMotor==eptUseMotCyn)                                    //Steven for HT1032
//    {
//        InArmSuck.Suck[0][0].iMotNo=C_InArmAa;
//        InArmSuck.Suck[1][0].iMotNo=C_InArmBa;
//        InArmSuck.Suck[0][1].iMotNo=C_InArmAb;
//        InArmSuck.Suck[1][1].iMotNo=C_InArmBb;
//        InArmSuck.Suck[0][2].iMotNo=C_InArmAc;
//        InArmSuck.Suck[1][2].iMotNo=C_InArmBc;
//        InArmSuck.Suck[0][3].iMotNo=C_InArmAd;
//        InArmSuck.Suck[1][3].iMotNo=C_InArmBd;
//        InArmSuck.Suck[0][4].iMotNo=C_InArmAe;
//        InArmSuck.Suck[1][4].iMotNo=C_InArmBe;
//        InArmSuck.Suck[0][5].iMotNo=C_InArmAf;
//        InArmSuck.Suck[1][5].iMotNo=C_InArmBf;
//        InArmSuck.Suck[0][6].iMotNo=C_InArmAg;
//        InArmSuck.Suck[1][6].iMotNo=C_InArmBg;
//        InArmSuck.Suck[0][7].iMotNo=C_InArmAh;
//        InArmSuck.Suck[1][7].iMotNo=C_InArmBh;
//
//        OutArmSuck.Suck[0][0].iMotNo=C_OutArmAa;
//        OutArmSuck.Suck[1][0].iMotNo=C_OutArmBa;
//        OutArmSuck.Suck[0][1].iMotNo=C_OutArmAb;
//        OutArmSuck.Suck[1][1].iMotNo=C_OutArmBb;
//        OutArmSuck.Suck[0][2].iMotNo=C_OutArmAc;
//        OutArmSuck.Suck[1][2].iMotNo=C_OutArmBc;
//        OutArmSuck.Suck[0][3].iMotNo=C_OutArmAd;
//        OutArmSuck.Suck[1][3].iMotNo=C_OutArmBd;
//        OutArmSuck.Suck[0][4].iMotNo=C_OutArmAe;
//        OutArmSuck.Suck[1][4].iMotNo=C_OutArmBe;
//        OutArmSuck.Suck[0][5].iMotNo=C_OutArmAf;
//        OutArmSuck.Suck[1][5].iMotNo=C_OutArmBf;
//        OutArmSuck.Suck[0][6].iMotNo=C_OutArmAg;
//        OutArmSuck.Suck[1][6].iMotNo=C_OutArmBg;
//        OutArmSuck.Suck[0][7].iMotNo=C_OutArmAh;
//        OutArmSuck.Suck[1][7].iMotNo=C_OutArmBh;
//
//        for(int i=0; i<2; i++)
//        {
//            for(int j=0; j<8; j++)
//            {
//                S.sprintf("%c%c", 'A'+i, 'a'+j);
//                InArmSuck.Suck[i][j].sName=S;
//                InArmSuck.Suck[i][j].iMyRow=i;
//                InArmSuck.Suck[i][j].iMyCol=j;
//                OutArmSuck.Suck[i][j].sName=S;
//                OutArmSuck.Suck[i][j].iMyRow=i;
//                OutArmSuck.Suck[i][j].iMyCol=j;
//            }
//        }
//    }
//    else
    {
        InArmSuck.Suck[0][0].iMotNo=MInArmZA;
        InArmSuck.Suck[1][0].iMotNo=MInArmZB;
        InArmSuck.Suck[0][1].iMotNo=MInArmZC;
        InArmSuck.Suck[1][1].iMotNo=MInArmZD;
        InArmSuck.Suck[0][2].iMotNo=MInArmZE;
        InArmSuck.Suck[1][2].iMotNo=MInArmZF;
        InArmSuck.Suck[0][3].iMotNo=MInArmZG;
        InArmSuck.Suck[1][3].iMotNo=MInArmZH;

        OutArmSuck.Suck[0][0].iMotNo=MOutArmZA;
        OutArmSuck.Suck[1][0].iMotNo=MOutArmZB;
        OutArmSuck.Suck[0][1].iMotNo=MOutArmZC;
        OutArmSuck.Suck[1][1].iMotNo=MOutArmZD;
        OutArmSuck.Suck[0][2].iMotNo=MOutArmZE;
        OutArmSuck.Suck[1][2].iMotNo=MOutArmZF;
        OutArmSuck.Suck[0][3].iMotNo=MOutArmZG;
        OutArmSuck.Suck[1][3].iMotNo=MOutArmZH;

        if(USE_PICKER_COUNT==ep16Picker &&
           InOutArmPickerUseMotor==eptUseMot)                                   //Ztex 2023.04.19 Add HT-1032
        {
            InArmSuck.Suck[0][0].iMotNo=MInArmZA;
            InArmSuck.Suck[0][1].iMotNo=MInArmZC;
            InArmSuck.Suck[0][2].iMotNo=MInArmZE;
            InArmSuck.Suck[0][3].iMotNo=MInArmZG;
            InArmSuck.Suck[0][4].iMotNo=MInArmZAe;
            InArmSuck.Suck[0][5].iMotNo=MInArmZAf;
            InArmSuck.Suck[0][6].iMotNo=MInArmZAg;
            InArmSuck.Suck[0][7].iMotNo=MInArmZAh;

            InArmSuck.Suck[1][0].iMotNo=MInArmZB;
            InArmSuck.Suck[1][1].iMotNo=MInArmZD;
            InArmSuck.Suck[1][2].iMotNo=MInArmZF;
            InArmSuck.Suck[1][3].iMotNo=MInArmZH;
            InArmSuck.Suck[1][4].iMotNo=MInArmZBe;
            InArmSuck.Suck[1][5].iMotNo=MInArmZBf;
            InArmSuck.Suck[1][6].iMotNo=MInArmZBg;
            InArmSuck.Suck[1][7].iMotNo=MInArmZBh;

            OutArmSuck.Suck[0][0].iMotNo=MOutArmZA;
            OutArmSuck.Suck[0][1].iMotNo=MOutArmZC;
            OutArmSuck.Suck[0][2].iMotNo=MOutArmZE;
            OutArmSuck.Suck[0][3].iMotNo=MOutArmZG;
            OutArmSuck.Suck[0][4].iMotNo=MOutArmZAe;
            OutArmSuck.Suck[0][5].iMotNo=MOutArmZAf;
            OutArmSuck.Suck[0][6].iMotNo=MOutArmZAg;
            OutArmSuck.Suck[0][7].iMotNo=MOutArmZAh;

            OutArmSuck.Suck[1][0].iMotNo=MOutArmZB;
            OutArmSuck.Suck[1][1].iMotNo=MOutArmZD;
            OutArmSuck.Suck[1][2].iMotNo=MOutArmZF;
            OutArmSuck.Suck[1][3].iMotNo=MOutArmZH;
            OutArmSuck.Suck[1][4].iMotNo=MOutArmZBe;
            OutArmSuck.Suck[1][5].iMotNo=MOutArmZBf;
            OutArmSuck.Suck[1][6].iMotNo=MOutArmZBg;
            OutArmSuck.Suck[1][7].iMotNo=MOutArmZBh;

            for(int j=0; j<8; j++)
            {
                for(int i=0; i<2; i++)
                {
                    S.sprintf("%c%c", 'A'+i, 'a'+j);                            //Ztex 2024.07.11 Add Suck Name A --> Aa
                    InArmSuck.Suck[i][j].sName=S;
                    InArmSuck.Suck[i][j].iMyRow=i;
                    InArmSuck.Suck[i][j].iMyCol=j;
                    OutArmSuck.Suck[i][j].sName=S;
                    OutArmSuck.Suck[i][j].iMyRow=i;
                    OutArmSuck.Suck[i][j].iMyCol=j;
                }
            }
        }
        else
        {
            int k=0;
            for(int j=0; j<8; j++)
            {
                for(int i=0; i<2; i++)
                {
                    S.sprintf("%c", 'A'+k);
                    InArmSuck.Suck[i][j].sName=S;
                    InArmSuck.Suck[i][j].iMyRow=i;
                    InArmSuck.Suck[i][j].iMyCol=j;
                    OutArmSuck.Suck[i][j].sName=S;
                    OutArmSuck.Suck[i][j].iMyRow=i;
                    OutArmSuck.Suck[i][j].iMyCol=j;
                    k++;
                }
            }
        }
    }

    CopyKitSuck(&InArmSuck, &InArmSuckBackup);                                  //Frank HT1032 add
    CopyKitSuck(&OutArmSuck,&OutArmSuckBackup);
}
//==============================================================================
void InitialSwitchName()
{
    SW[SwFKPowerOff                 ].Name="SwFKPowerOff";
    SW[SwFKPowerOn                  ].Name="SwFKPowerOn";
    SW[SwFKReset                    ].Name="SwFKReset";
    SW[SwFKPause                    ].Name="SwFKPause";
    SW[SwFKHome                     ].Name="SwFKHome";
    SW[SwFKStart                    ].Name="SwFKStart";
    SW[SwFKOneCycle                 ].Name="SwFKOneCycle";
    SW[SwFKRetry                    ].Name="SwFKRetry";
    SW[SwFKSkip                     ].Name="SwFKSkip";
    SW[SwFKCleanOut                 ].Name="SwFKCleanOut";
    SW[SwFKTrayFeed                 ].Name="SwFKTrayFeed";
    SW[SwFKTrayEnd                  ].Name="SwFKTrayEnd";
    SW[SwFKAlarmReset               ].Name="SwFKAlarmReset";
    SW[SwFKCoverOpen                ].Name="SwFKCoverOpen";
    SW[SwRKPowerOff                 ].Name="SwRKPowerOff";
    SW[SwRKPowerOn                  ].Name="SwRKPowerOn";
    SW[SwRKReset                    ].Name="SwRKReset";
    SW[SwRKPause                    ].Name="SwRKPause";
    SW[SwRKHome                     ].Name="SwRKHome";
    SW[SwRKStart                    ].Name="SwRKStart";
    SW[SwRKOneCycle                 ].Name="SwRKOneCycle";
    SW[SwRKRetry                    ].Name="SwRKRetry";
    SW[SwRKSkip                     ].Name="SwRKSkip";
    SW[SwRKCleanOut                 ].Name="SwRKCleanOut";
    SW[SwRKTrayFeed                 ].Name="SwRKTrayFeed";
    SW[SwRKTrayEnd                  ].Name="SwRKTrayEnd";
    SW[SwRKAlarmReset               ].Name="SwRKAlarmReset";
    SW[SwRKCoverOpen                ].Name="SwRKCoverOpen";
    SW[SwRKManualStep               ].Name="SwRKManualStep";
    SW[SwRKManualTStart             ].Name="SwRKManualTStart";
    SW[SwTowerRed                   ].Name="SwTowerRed";
    SW[SwTowerGreen                 ].Name="SwTowerGreen";
    SW[SwTowerYellow                ].Name="SwTowerYellow";
    SW[SwFMotorBreaker              ].Name="SwFMotorBreaker";
    SW[SwBMotorBreaker              ].Name="SwBMotorBreaker";
    SW[SwMotorRelay                 ].Name="SwMotorRelay";
    SW[SwHeaterRelay                ].Name="SwHeaterRelay";
    SW[SwMusic1                     ].Name="SwMusic1";
    SW[SwMusic2                     ].Name="SwMusic2";
    SW[SwMusic3                     ].Name="SwMusic3";
    SW[SwMusic4                     ].Name="SwMusic4";
    SW[SwTestPassLed                ].Name="SwTestPassLed";
    SW[SwTestFailLed                ].Name="SwTestFailLed";
    SW[SwFrontActiveLed             ].Name="SwFrontActiveLed";
    SW[SwRearActiveLed              ].Name="SwRearActiveLed";
    SW[SwClear0                     ].Name="SwClear0";
    SW[SwClear1                     ].Name="SwClear1";
    SW[SwStart0                     ].Name="SwStart0";
    SW[SwStart1                     ].Name="SwStart1";
    SW[SwACTrayY                    ].Name="SwACTrayY";
    SW[SwACAuto1                    ].Name="SwACAuto1";
    SW[SwACAuto2                    ].Name="SwACAuto2";
    SW[SwACAuto3                    ].Name="SwACAuto3";
    SW[SwServerON                   ].Name="SwServerON";
    SW[SwManualZ1                   ].Name="SwManualZ1";
    SW[SwManualZ2                   ].Name="SwManualZ2";
    SW[SwACEmptyCW                  ].Name="SwACEmptyCW";
    SW[SwACEmptyCCW                 ].Name="SwACEmptyCCW";
    SW[SwACColorCW                  ].Name="SwACColorCW";
    SW[SwClear2                     ].Name="SwClear2";
    SW[SwClear3                     ].Name="SwClear3";
    SW[SwStart2                     ].Name="SwStart2";
    SW[SwStart3                     ].Name="SwStart3";
    SW[SwCCDAir                     ].Name="SwCCDAir";
    SW[SwACAutoCW                   ].Name="SwACAutoCW";
    SW[SwACAuto1Mode                ].Name="SwACAuto1Mode";
    SW[SwReadTorue                  ].Name="SwReadTorue";
    SW[SwBigFan                     ].Name="SwBigFan";
    SW[SwDut0                       ].Name="SwDut0";
    SW[SwDut1                       ].Name="SwDut1";
    SW[SwDut2                       ].Name="SwDut2";
    SW[SwDut3                       ].Name="SwDut3";
    SW[Sw10Bit                      ].Name="Sw10Bit";
    SW[SwAuto1SelectSlow            ].Name="SwAuto1SelectSlow";
    SW[SwCCDDestroy                 ].Name="SwCCDDestroy";
    SW[SwSafeLock                   ].Name="SwSafeLock";
    SW[SwRKSafeLock                 ].Name="SwRKSafeLock";                      //KenHsieh 20211228 : 區分實體IO與通訊面板
    SW[SwCCDLight                   ].Name="SwCCDLight";
    SW[SwHeaterFan                  ].Name="SwHeaterFan";
    SW[SwSocketClean                ].Name="SwSocketClean";
    SW[SwACColorCCW                 ].Name="SwACColorCCW";
    SW[SwShuttleFan                 ].Name="SwShuttleFan";
    SW[SwTesterDoubleContact        ].Name="SwTesterDoubleContact";
    SW[SwTesterPower                ].Name="SwTesterPower";
    SW[SwACEmpty1CW                 ].Name="SwACEmpty1CW";
    SW[SwACEmpty1CCW                ].Name="SwACEmpty1CCW";
    SW[SwDutHeaterCoolFan           ].Name="SwDutHeaterCoolFan";
    SW[SwLoaderBin                  ].Name="SwLoaderBin";
    SW[SwEmpty1Bin                  ].Name="SwEmpty1Bin";
    SW[SwEmpty2Bin                  ].Name="SwEmpty2Bin";
    SW[SwAuto1Bin                   ].Name="SwAuto1Bin";
    SW[SwAuto2Bin                   ].Name="SwAuto2Bin";
    SW[SwAuto3Bin                   ].Name="SwAuto3Bin";
    SW[SwFix1Bin                    ].Name="SwFix1Bin";
    SW[SwFix2Bin                    ].Name="SwFix2Bin";
    SW[SwFix3Bin                    ].Name="SwFix3Bin";
    SW[SwFix4Bin                    ].Name="SwFix4Bin";
    SW[SwFix5Bin                    ].Name="SwFix5Bin";
    SW[SwFix6Bin                    ].Name="SwFix6Bin";
    SW[SwZ1SuckMode0                ].Name="SwZ1SuckMode0";
    SW[SwZ1SuckMode1                ].Name="SwZ1SuckMode1";
    SW[SwZ2SuckMode0                ].Name="SwZ2SuckMode0";
    SW[SwZ2SuckMode1                ].Name="SwZ2SuckMode1";
    SW[SwShuttleCooling             ].Name="SwShuttleCooling";                  //jou 2010-06-09
    SW[SwCCDCooling                 ].Name="SwCCDCooling";                      //Steven 20110705
    SW[SwEpArm1                     ].Name="SwEpArm1";                          //Steven 20110708
    SW[SwEpArm2                     ].Name="SwEpArm2";                          //Steven 20110708
    SW[SwHeaterFanSpeed             ].Name="SwHeaterFanSpeed";                  //Steven 20110725
    SW[SwRotateCheckClear           ].Name="SwRotateCheckClear";                //ChungHung 20110922 : 轉轉蝦頭要檢查有沒有轉頭 Check Sensor
    SW[SwEP_D0                      ].Name="SwEP_D0";                           //Dell 20111111 Start : 加入Digital E/P
    SW[SwEP_D1                      ].Name="SwEP_D1";
    SW[SwEP_D2                      ].Name="SwEP_D2";
    SW[SwEP_D3                      ].Name="SwEP_D3";
    SW[SwEP_D4                      ].Name="SwEP_D4";
    SW[SwEP_D5                      ].Name="SwEP_D5";
    SW[SwEP_D6                      ].Name="SwEP_D6";
    SW[SwEP_D7                      ].Name="SwEP_D7";
    SW[SwEP_D8                      ].Name="SwEP_D8";
    SW[SwEP_D9                      ].Name="SwEP_D9";
    SW[SwCoolingFan_Blower          ].Name="SwCoolingFan_Blower";               //20111130  Dell
    SW[SwSafeDoorLock               ].Name="SwSafeDoorLock";                    //20111130  Dell
    SW[SwIndexIonFan                ].Name="SwIndexIonFan";                     //jou 2012-03-13 index離子槍出風開關控制
    SW[SwIndexChangeToque1          ].Name="SwIndexChangeToque1";               //jou 2012-06-21 Enable index I/O Change Toque
    SW[SwIndexChangeToque2          ].Name="SwIndexChangeToque2";               //jou 2012-06-21 Enable index I/O Change Toque
    SW[SwACLoaderCCW                ].Name="SwACLoaderCCW";                     //Loader退Tray
    SW[SwACAuto2CW                  ].Name="SwACAuto2CW";                       //Auto2進Tray
    SW[SwACAuto3CW                  ].Name="SwACAuto3CW";                       //Auto3進Tray
    SW[SwHeatGun                    ].Name="SwHeatGun";                         //ChungHung 20121107 add
    SW[SwCDAGun                     ].Name="SwCDAGun";                          //Steven 20181012 : 使用熱風槍吹冷風
    SW[SwVacuumPumpTogetherOn       ].Name="SwVacuumPumpTogetherOn";
    SW[SwAirConditioner             ].Name="SwAirConditioner";                  //Steven 20131011 : 冷氣機
    SW[SwHotplateCooling            ].Name="SwHotplateCooling";                 //jou 2013-11-07
    SW[SwCarRecord                  ].Name="SwCarRecord";                       //wei 2013-12-09
    SW[SwLoadCellA                  ].Name="SwLoadCellA";                       //kevin 20190306  add load Cell  read 1
    SW[SwLoadCellB                  ].Name="SwLoadCellB";                       //kevin 20190306  add load Cell  read 2

    SW[SwUnDock                     ].Name="SwUnDock";                          //Steven 20140310 : One Touch Docking
    SW[SwDockError                  ].Name="SwDockError";                       //Steven 20140310 : One Touch Docking

    SW[SwStartTest1                 ].Name="SwStartTest1";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    SW[SwStartTest2                 ].Name="SwStartTest2";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    SW[SwStartTest3                 ].Name="SwStartTest3";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    SW[SwOCRTigger                  ].Name="SwOCRTigger";                       //wei 20150720 OCR觸發

    SW[SwTesterAirCooling           ].Name="SwTesterAirCooling";                //jou 2016-01-07 Tester Air Cooling Switch
    SW[SwFanDirection               ].Name="SwFanDirection";                    //wei 20160215大風扇方向
    SW[SwDieClean                   ].Name="SwDieClean";
    SW[SwDieCleanSuck               ].Name="SwDieCleanSuck";

    SW[SwLightOff                   ].Name="SwLightOff";
    SW[SwAirOff                     ].Name="SwAirOff";

    SW[SwCaselevatorZ               ].Name="SwCaselevatorZ";
    SW[SwTrayBracketZ               ].Name="SwTrayBracketZ";

    SW[SwE84LREQ                    ].Name="SwE84LREQ";
    SW[SwE84UREQ                    ].Name="SwE84UREQ";
    SW[SwE84VA                      ].Name="SwE84VA";
    SW[SwE84READY                   ].Name="SwE84READY";
    SW[SwE84VS0                     ].Name="SwE84VS0";
    SW[SwE84VS1                     ].Name="SwE84VS1";
    SW[SwE84HOAVBL                  ].Name="SwE84HOAVBL";
    SW[SwE84ES                      ].Name="SwE84ES";
    SW[SwE84POWER                   ].Name="SwE84POWER";

    SW[SwCassette01                 ].Name="SwCassette01";                      //wei 20180702 MR
    SW[SwCassette02                 ].Name="SwCassette02";                      //wei 20180702 MR
    SW[SwCassette03                 ].Name="SwCassette03";                      //wei 20180702 MR
    SW[SwCassette04                 ].Name="SwCassette04";                      //wei 20180702 MR
    SW[SwCassette05                 ].Name="SwCassette05";                      //wei 20180702 MR
    SW[SwCassette06                 ].Name="SwCassette06";                      //wei 20180702 MR
    SW[SwCassette07                 ].Name="SwCassette07";                      //wei 20180702 MR
    SW[SwCassette08                 ].Name="SwCassette08";                      //wei 20180702 MR

    SW[SwCassette09                 ].Name="SwCassette09";                      //wei 20180702 MR
    SW[SwCassette10                 ].Name="SwCassette10";                      //wei 20180702 MR
    SW[SwMRStart                    ].Name="SwMRStart";                         //wei 20180702 MR
    SW[SwMRPause                    ].Name="SwMRPause";                         //wei 20180702 MR
    SW[SwMRUp                       ].Name="SwMRUp";                            //wei 20180702 MR
    SW[SwMRDown                     ].Name="SwMRDown";                          //wei 20180702 MR

    SW[SwMRError                    ].Name="SwMRError";                         //wei 20180702 MR
    SW[SwMRUnLoadReady              ].Name="SwMRUnLoadReady";                   //wei 20180702 MR
    SW[SwMRPresence                 ].Name="SwMRPresence";                      //wei 20180702 MR
    SW[SwMRPlacement                ].Name="SwMRPlacement";                     //wei 20180702 MR
    SW[SwMRLoadReady                ].Name="SwMRLoadReady";                     //wei 20180702 MR
    SW[SwMRManualMode               ].Name="SwMRManualMode";                    //wei 20180702 MR

    SW[SwMRLight                    ].Name="SwMRLight";                         //wei 20180702 MR
    SW[SwBufferArmZ                 ].Name="SwBufferArmZ";                      //wei 20200302 MR

    SW[SwTrayBracket2Z              ].Name="SwTrayBracket2Z";                   //KaiChen 20200716 ：OHT
    SW[SwCaselevator2Z              ].Name="SwCaselevator2Z";
    SW[SwMultileEmptyZ              ].Name="SwMultileEmptyZ";

    SW[SwOHT_UnloadReady_1          ].Name="SwOHT_UnloadReady_1";
    SW[SwOHT_CarrierPresence_1      ].Name="SwOHT_CarrierPresence_1";
    SW[SwOHT_CarrierPlacement_1     ].Name="SwOHT_CarrierPlacement_1";
    SW[SwOHT_LoadReady_1            ].Name="SwOHT_LoadReady_1";
    SW[SwOHT_ManualMode_1           ].Name="SwOHT_ManualMode_1";
    SW[SwOHT_AutoMode_1             ].Name="SwOHT_AutoMode_1";
    SW[SwOHT_Error_1                ].Name="SwOHT_Error_1";

    SW[SwOHT_UnloadReady_2          ].Name="SwOHT_UnloadReady_2";
    SW[SwOHT_CarrierPresence_2      ].Name="SwOHT_CarrierPresence_2";
    SW[SwOHT_CarrierPlacement_2     ].Name="SwOHT_CarrierPlacement_2";
    SW[SwOHT_LoadReady_2            ].Name="SwOHT_LoadReady_2";
    SW[SwOHT_ManualMode_2           ].Name="SwOHT_ManualMode_2";
    SW[SwOHT_AutoMode_2             ].Name="SwOHT_AutoMode_2";
    SW[SwOHT_Error_2                ].Name="SwOHT_Error_2";

    SW[SwMultileEmpty_ScanTray_Open ].Name="SwMultileEmpty_ScanTray_Open";
    SW[SwMultileEmpty_ScanTray_Close].Name="SwMultileEmpty_ScanTray_Close";
    SW[SwMultileEmpty_ScanTrayID    ].Name="SwMultileEmpty_ScanTrayID";

    SW[SwE84_2_LREQ                 ].Name="SwE84_2_LREQ";
    SW[SwE84_2_UREQ                 ].Name="SwE84_2_UREQ";
    SW[SwE84_2_VA                   ].Name="SwE84_2_VA";
    SW[SwE84_2_READY                ].Name="SwE84_2_READY";
    SW[SwE84_2_VS0                  ].Name="SwE84_2_VS0";
    SW[SwE84_2_VS1                  ].Name="SwE84_2_VS1";
    SW[SwE84_2_HOAVBL               ].Name="SwE84_2_HOAVBL";
    SW[SwE84_2_ES                   ].Name="SwE84_2_ES";
    SW[SwE84_2_POWER                ].Name="SwE84_2_POWER";

    SW[SwE84_1_LREQ                 ].Name="SwE84_1_LREQ";
    SW[SwE84_1_UREQ                 ].Name="SwE84_1_UREQ";
    SW[SwE84_1_VA                   ].Name="SwE84_1_VA";
    SW[SwE84_1_READY                ].Name="SwE84_1_READY";
    SW[SwE84_1_VS0                  ].Name="SwE84_1_VS0";
    SW[SwE84_1_VS1                  ].Name="SwE84_1_VS1";
    SW[SwE84_1_HOAVBL               ].Name="SwE84_1_HOAVBL";
    SW[SwE84_1_ES                   ].Name="SwE84_1_ES";
    SW[SwE84_1_POWER                ].Name="SwE84_1_POWER";

    SW[SwSafeDoorLock_LoadPort1     ].Name="SwSafeDoorLock_LoadPort1";
    SW[SwSafeDoorLock_LoadPort2     ].Name="SwSafeDoorLock_LoadPort2";

    SW[SwLoadDoorLock               ].Name="SwLoadDoorLock";                    //Sam 20190112 LM
    SW[SwLoadRobotZ                 ].Name="SwLoadRobotZ";
    SW[SwUnloadDoorLock             ].Name="SwUnloadDoorLock";
    SW[SwUnloadRobotZ               ].Name="SwUnloadRobotZ";

    SW[SwIonRelay                   ].Name="SwIonRelay";                        //Ifor 20190114 : add Chamber 開啟時 關閉 Ion 離子槍 吹氣
    //------------------------------------
    //Alick 20161011 (Steven) : TTL支援8Site
    //------------------------------------
    SW[SwClear4                     ].Name="SwClear4";                          //Clear All
    SW[SwClear5                     ].Name="SwClear5";                          //Anti-Start Signal
    SW[SwClear6                     ].Name="SwClear6";                          //Start Enable
    SW[SwClear7                     ].Name="SwClear7";                          //Reserve
    SW[SwStart4                     ].Name="SwStart4";
    SW[SwStart5                     ].Name="SwStart5";
    SW[SwStart6                     ].Name="SwStart6";
    SW[SwStart7                     ].Name="SwStart7";
    SW[SwDut4                       ].Name="SwDut4";
    SW[SwDut5                       ].Name="SwDut5";
    SW[SwDut6                       ].Name="SwDut6";
    SW[SwDut7                       ].Name="SwDut7";
    SW[Sw10Bit2                     ].Name="Sw10Bit2";

    SW[SwShuttleVibration1          ].Name="SwShuttleVibration1";               //JerryYang 20171006 (wei) Shuttle 震動馬達
    SW[SwShuttleVibration2          ].Name="SwShuttleVibration2";
    SW[SwPurgeAir                   ].Name="SwPurgeAir";                        //kevin 20180928 (Steven) : add blower load board
    SW[SwSocketClean2               ].Name="SwSocketClean2";                    //JerryYang 20190715 Clean air arm1 arm2分開控制
    SW[SwLoaderVibration            ].Name="SwLoaderVibration";                 //JerryYang 20191001 loader震動馬達
    SW[SwTesterDryAirSwitch         ].Name="SwTesterDryAirSwitch";              //Ifor 20200115 : add Tester Dry Air Control
    SW[SwIndEpArm1                  ].Name="SwIndEpArm1";                       //Steven 20110708
    SW[SwIndEpArm2                  ].Name="SwIndEpArm2";                       //Steven 20110708
    SW[SwAutoCoolDown               ].Name="SwAutoCoolDown";                    //kevin 20201223 AutoCool down
    SW[SwIonFanClean                ].Name="SwIonFanClean";                     //Isaac 20210609 : IO觸發IonFan清針

    SW[SwTjSignal01                 ].Name="SwTjSignal01";                      //Ifor 20210622 add: ATC Switch TJ
    SW[SwTjSignal02                 ].Name="SwTjSignal02";
    SW[SwTjSignal03                 ].Name="SwTjSignal03";
    SW[SwTjSignal04                 ].Name="SwTjSignal04";
    SW[SwTjSignal05                 ].Name="SwTjSignal05";
    SW[SwTjSignal06                 ].Name="SwTjSignal06";
    SW[SwTjSignal07                 ].Name="SwTjSignal07";
    SW[SwTjSignal08                 ].Name="SwTjSignal08";

    SW[SwATCHeatGun                 ].Name="SwATCHeatGun";                      //JerryYang 20220408 : add for ATC3.5
    SW[SwLBAir                      ].Name="SwLBAir";                           //JerryYang 20220923 : LB吹氣function

    SW[SwDryAirSwitch               ].Name="SwDryAirSwitch";                    //Ztex 2023.04.13 Add HT-1032 IO
    SW[SwColdAirSwitch              ].Name="SwColdAirSwitch";                   //Ztex 2023.04.13 Add HT-1032 IO
    SW[SwTriTempSafeDoor6Lock       ].Name="SwTriTempSafeDoor6Lock";            //Ztex 2023.04.13 Add HT-1032 IO

    SW[SwEnhaustAirFanPowerOn       ].Name="SwEnhaustAirFanPowerOn";            //Ztex 2023.04.26 Add HT-1032 IO Exhaust Air
    SW[SwMagazineMotorBreaker       ].Name="SwMagazineMotorBreaker";            //JerryYang 20221215 : add Magazine
    SW[SwMagazineSafeDoorLock       ].Name="SwMagazineSafeDoorLock";
    SW[SwMagazineSafeDoor2LockOn    ].Name="SwMagazineSafeDoor2LockOn";
    SW[SwMagazineSafeDoor2LockOff   ].Name="SwMagazineSafeDoor2LockOff";

    SW[SwESDAntennaRelay1           ].Name="SwESDAntennaRelay1";
    SW[SwESDAntennaRelay2           ].Name="SwESDAntennaRelay2";
    SW[SwESDAntennaRelay3           ].Name="SwESDAntennaRelay3";
    SW[SwESDAntennaRelay4           ].Name="SwESDAntennaRelay4";
    SW[SwESDAntennaRelay5           ].Name="SwESDAntennaRelay5";
    SW[SwESDAntennaRelay6           ].Name="SwESDAntennaRelay6";
    SW[SwESDAntennaRelay7           ].Name="SwESDAntennaRelay7";
    SW[SwESDAntennaRelay8           ].Name="SwESDAntennaRelay8";

    SW[SwLoaderAirClean             ].Name="SwLoaderAirClean";

    SW[SwACAuto4                    ].Name="SwACAuto4";                         //Steven 20240822 : For HT-9046AU
    SW[SwACAuto4CW                  ].Name="SwACAuto4CW";
    SW[SwACAuto5                    ].Name="SwACAuto5";
    SW[SwACAuto5CW                  ].Name="SwACAuto5CW";
    SW[SwACAuto6                    ].Name="SwACAuto6";
    SW[SwACAuto6CW                  ].Name="SwACAuto6CW";

    SW[SwLoad2Bin                   ].Name="SwLoad2Bin";
    SW[SwLoad2AirClean              ].Name="SwLoad2AirClean";
    SW[SwACLoad2CCW                 ].Name="SwACLoad2CCW";
    SW[SwACTray2Y                   ].Name="SwACTray2Y";
    SW[SwLoad2Vibration             ].Name="SwLoad2Vibration";

    SW[SwSafeDoor1HatchwayLock      ].Name="SwSafeDoor1HatchwayLock";           //ChungHung 20230718 add for Safe plc start
    SW[SwSafeDoor2HatchwayLock      ].Name="SwSafeDoor2HatchwayLock";
    SW[SwSafeDoor3HatchwayLock      ].Name="SwSafeDoor3HatchwayLock";
    SW[SwSafeDoor4HatchwayLock      ].Name="SwSafeDoor4HatchwayLock";
    SW[SwSafeDoor5HatchwayLock      ].Name="SwSafeDoor5HatchwayLock";
    SW[SwSafeDoor6HatchwayLock      ].Name="SwSafeDoor6HatchwayLock";
    SW[SwSafeDoor7HatchwayLock      ].Name="SwSafeDoor7HatchwayLock";
    SW[SwSafeDoor8HatchwayLock      ].Name="SwSafeDoor8HatchwayLock";

    SW[SwElectricControlBoxLock1    ].Name="SwElectricControlBoxLock1";
    SW[SwElectricControlBoxLock2    ].Name="SwElectricControlBoxLock2";
    SW[SwElectricControlBoxLock3    ].Name="SwElectricControlBoxLock3";
    SW[SwElectricControlBoxLock4    ].Name="SwElectricControlBoxLock4";
    SW[SwElectricControlBoxLock5    ].Name="SwElectricControlBoxLock5";
    SW[SwElectricControlBoxLock6    ].Name="SwElectricControlBoxLock6";
    SW[SwElectricControlBoxLock7    ].Name="SwElectricControlBoxLock7";         //ChungHung 20230718 add for Safe plc end

    SW[SwInArmZBreaker              ].Name="SwInArmZBreaker";                   //add One sucker with rotate
    SW[SwOutArmZBreaker             ].Name="SwOutArmZBreaker";                  //add One sucker with rotate

    SW[SwBottomBlower               ].Name="SwBottomBlower";
    SW[SwFixedSeatTLOn              ].Name="SwFixedSeatTLOn";
    SW[SwFixedSeatTLOff             ].Name="SwFixedSeatTLOff";
    SW[SwFixedSeatTROn              ].Name="SwFixedSeatTROn";
    SW[SwFixedSeatTROff             ].Name="SwFixedSeatTROff";
    SW[SwFixedSeatBLOn              ].Name="SwFixedSeatBLOn";
    SW[SwFixedSeatBLOff             ].Name="SwFixedSeatBLOff";
    SW[SwFixedSeatBROn              ].Name="SwFixedSeatBROn";
    SW[SwFixedSeatBROff             ].Name="SwFixedSeatBROff";

    SW[SwCCDZBreaker                ].Name="SwCCDZBreaker";
    SW[SwTopBtmRotateLockOn         ].Name="SwTopBtmRotateLockOn";
    SW[SwTopBtmRotateLockOff        ].Name="SwTopBtmRotateLockOff";
    SW[SwLightStart                 ].Name="SwLightStart";
    SW[SwPRGSEL0                    ].Name="SwPRGSEL0";
    SW[SwPRGSEL1                    ].Name="SwPRGSEL1";
    SW[SwPRGSEL2                    ].Name="SwPRGSEL2";
    SW[SwPRGSEL3                    ].Name="SwPRGSEL3";
    SW[SwPRGSEL4                    ].Name="SwPRGSEL4";
    SW[SwPRGSEL5                    ].Name="SwPRGSEL5";
    SW[SwPRGSEL6                    ].Name="SwPRGSEL6";
    SW[SwLightOrg                   ].Name="SwLightOrg";
    SW[SwMultiEp                    ].Name="SwMultiEp";                         //Ifor 20250618 add:Auto Switch Multi EP

    SW[SwLoadCarRFIDZBreaker        ].Name="SwLoadCarRFIDZBreaker";             //RogerYang 20250828 add for Loader Rotate Arm

    SW[SwCassetteLDMotBreaker     ].Name="SwCassetteLDMotBreaker";              //Ifor 20251216 add:Boat Carrier
    SW[SwCassetteAuto1MotBreaker  ].Name="SwCassetteAuto1MotBreaker";
    SW[SwCassetteAuto2MotBreaker  ].Name="SwCassetteAuto2MotBreaker";
}
//------------------------------------------------------------------------------
void InitialSwitch()
{
    TTable *T;
    int iSw;
    bool flag;
    AnsiString S, S1, sDBDir;
    bool bHasSwitch;                                                            //Stevem 20210625 : 修正新的CSV IO表讀檔方式
    AnsiString str;

    if(IO_CARD_TYPE==NewIO_MN200 ||                                             //Steven 20231218 HT7080B
       IO_CARD_TYPE==PCI_P64C64)
    {
        InitialSwitchName();
        for(int i=0; i<MAX_SWITCH_ITEM; i++)
        {
            bHasSwitch=false;
            SW[i].Enable=false;
            if(SW[i].Name=="")
                continue;

            HSys.mapIOTableIter=HSys.mapIOTable.find(SW[i].Name);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSwitch=true;
                iSw=atoi(HSys.mapIOTable[SW[i].Name].c_str());
            }
            else
            {
                str.sprintf("Can not find switch %s", SW[i].Name);
//                continue;
            }

            if(bHasSwitch)
            {
                if(HSys.IOTable[iSw]->iEnable==1)
                    SW[i].Enable    =true;
                SW[i].ISABase       =HSys.IOTable[iSw]->iISABase;
                SW[i].Using         =(HSys.IOTable[iSw]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSw]->iPort);
                SW[i].Ring          =HSys.IOTable[iSw]->iLane;
                SW[i].IP            =HSys.IOTable[iSw]->iIP;
                SW[i].Port          =HSys.IOTable[iSw]->iPort;
                SW[i].Bit           =HSys.IOTable[iSw]->iBit;
                SW[i].Type          =HSys.IOTable[iSw]->iInType;

                #ifndef SOFT_SIMULTE
                if(SW[i].Enable)
                #endif
                {
                    MyLaneIO.SetUseIP(SW[i].Ring, SW[i].IP, SW[i].ISABase, SW->Name);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                }
            }
            else
            {
                SW[i].Enable=false;
            }
            #ifdef SOFT_SIMULTE
            SW[i].Enable=false;
            #endif
        }
    }
    else
    {
        sDBDir=CurrentDir+"\\system\\switch.db";                                //JerryYang 20150903 檢查Database檔是否存在
        if(FileExists(sDBDir))
        {
            DataModule1->SwitchTable->TableName=sDBDir;
        }
        else
        {
            ShowMyMessage("switch.db does not exist");
            return;
        }

        T=DataModule1->SwitchTable;
        T->Open();
        InitialSwitchName();
        for(int i=0; i<MAX_SWITCH_ITEM; i++)
        {
            if(SW[i].Name=="")
                continue;
            T->First();
            flag=false;
            while(!T->Eof)
            {
                S=T->FieldByName("SwitchName")->AsString;
                if(S==SW[i].Name)
                {
                    flag=true;
                    break;
                }
                T->Next();
            }
            if(flag==false)
            {
                str.sprintf("Can not find switch name:%s", SW[i].Name);
            }
            else
            {
                SW[i].Enable            =atoi(T->FieldByName("Enable")->AsString.c_str());
                SW[i].ISABase           =atoi(T->FieldByName("ISABase")->AsString.c_str());
                if(SW[i].ISABase==eISABase ||
                   SW[i].ISABase==ePCI1735U ||                                  //Alick 20160809 add for PCI-1735U
                   SW[i].ISABase==ePLCbase)
                {
                    SW[i].Using         =T->FieldByName("Port")->AsString;
                    if(SW[i].Using=="")
                        SW[i].Enable    =false;
                    SW[i].Ring          =0;
                    SW[i].IP            =0;
                    SW[i].Port          =HexStrToInt(T->FieldByName("Port")->AsString.c_str());
                    SW[i].Bit           =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                else
                {
                    SW[i].Using         =T->FieldByName("Lane")->AsString;
                    if(SW[i].Using!="")
                        SW[i].Ring      =atoi(SW[i].Using.c_str());
                    else
                        SW[i].Enable=false;
                    SW[i].IP            =atoi(T->FieldByName("IP")->AsString.c_str());
                    SW[i].Port          =atoi(T->FieldByName("Port")->AsString.c_str());
                    SW[i].Bit           =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                SW[i].Type              =atoi(T->FieldByName("OutIOType")->AsString.c_str());
                #ifdef SOFT_SIMULTE
                SW[i].Enable=false;
                #endif

                #ifndef SOFT_SIMULTE
                if(SW[i].Enable)
                #endif
                {
                    MyLaneIO.SetUseIP(SW[i].Ring, SW[i].IP, SW[i].ISABase, SW->Name);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                }
            }
        }
        T->Close();
    }
    #ifdef SOFT_SIMULTE
        SW[SwReadTorue].Enable=false;                                           // for home and get height use
        SW[SwStart0].Enable=true;
        SW[SwStart1].Enable=true;
        SW[SwStart2].Enable=true;
        SW[SwStart3].Enable=true;
        SW[SwDut0].Enable=true;
        SW[SwDut1].Enable=true;
        SW[SwDut2].Enable=true;
        SW[SwDut3].Enable=true;

        SW[SwStart4].Enable=true;                                               //Alick 20161011 (Steven) : TTL支援8Site
        SW[SwStart5].Enable=true;
        SW[SwStart6].Enable=true;
        SW[SwStart7].Enable=true;
        SW[SwDut4].Enable=true;
        SW[SwDut5].Enable=true;
        SW[SwDut6].Enable=true;
        SW[SwDut7].Enable=true;
    #endif

    if(iControlPanelMode==1)                                                    //Steven 20250317 : 使用通訊式面板的要disable
    {
        SW[SwFKPowerOff                 ].Enable=false;
        SW[SwFKPowerOn                  ].Enable=false;
        SW[SwFKReset                    ].Enable=false;
        SW[SwFKPause                    ].Enable=false;
        SW[SwFKHome                     ].Enable=false;
        SW[SwFKStart                    ].Enable=false;
        SW[SwFKOneCycle                 ].Enable=false;
        SW[SwFKRetry                    ].Enable=false;
        SW[SwFKSkip                     ].Enable=false;
        SW[SwFKCleanOut                 ].Enable=false;
        SW[SwFKTrayFeed                 ].Enable=false;
        SW[SwFKTrayEnd                  ].Enable=false;
        SW[SwFKAlarmReset               ].Enable=false;
        SW[SwFKCoverOpen                ].Enable=false;
        SW[SwRKPowerOff                 ].Enable=false;
        SW[SwRKPowerOn                  ].Enable=false;
        SW[SwRKReset                    ].Enable=false;
        SW[SwRKPause                    ].Enable=false;
        SW[SwRKHome                     ].Enable=false;
        SW[SwRKStart                    ].Enable=false;
        SW[SwRKOneCycle                 ].Enable=false;
        SW[SwRKRetry                    ].Enable=false;
        SW[SwRKSkip                     ].Enable=false;
        SW[SwRKCleanOut                 ].Enable=false;
        SW[SwRKTrayFeed                 ].Enable=false;
        SW[SwRKTrayEnd                  ].Enable=false;
        SW[SwRKAlarmReset               ].Enable=false;
        SW[SwRKCoverOpen                ].Enable=false;
        SW[SwRKManualStep               ].Enable=false;
        SW[SwRKManualTStart             ].Enable=false;
        SW[SwFrontActiveLed             ].Enable=false;
    }

    CopySwitch(&SW[SwStart0], &SWBackup[BackSwStart0]);
    CopySwitch(&SW[SwStart1], &SWBackup[BackSwStart1]);
    CopySwitch(&SW[SwStart2], &SWBackup[BackSwStart2]);
    CopySwitch(&SW[SwStart3], &SWBackup[BackSwStart3]);

    CopySwitch(&SW[SwDut0], &SWBackup[BackSwDut0]);
    CopySwitch(&SW[SwDut1], &SWBackup[BackSwDut1]);
    CopySwitch(&SW[SwDut2], &SWBackup[BackSwDut2]);
    CopySwitch(&SW[SwDut3], &SWBackup[BackSwDut3]);

    CopySwitch(&SW[SwStart4], &SWBackup[BackSwStart4]);                         //Alick 20161011 (Steven) : TTL支援8Site
    CopySwitch(&SW[SwStart5], &SWBackup[BackSwStart5]);
    CopySwitch(&SW[SwStart6], &SWBackup[BackSwStart6]);
    CopySwitch(&SW[SwStart7], &SWBackup[BackSwStart7]);

    CopySwitch(&SW[SwDut4], &SWBackup[BackSwDut4]);
    CopySwitch(&SW[SwDut5], &SWBackup[BackSwDut5]);
    CopySwitch(&SW[SwDut6], &SWBackup[BackSwDut6]);
    CopySwitch(&SW[SwDut7], &SWBackup[BackSwDut7]);
}
//==============================================================================
void InitialSensorName()
{
    Sen[SnFKPowerOff                ].Name="SnFKPowerOff";
    Sen[SnFKPowerOn                 ].Name="SnFKPowerOn";
    Sen[SnFKReset                   ].Name="SnFKReset";
    Sen[SnFKPause                   ].Name="SnFKPause";
    Sen[SnFKHome                    ].Name="SnFKHome";
    Sen[SnFKStart                   ].Name="SnFKStart";
    Sen[SnFKOneCycle                ].Name="SnFKOneCycle";
    Sen[SnFKRetry                   ].Name="SnFKRetry";
    Sen[SnFKSkip                    ].Name="SnFKSkip";
    Sen[SnFKCleanOut                ].Name="SnFKCleanOut";
    Sen[SnFKTrayFeed                ].Name="SnFKTrayFeed";
    Sen[SnFKTrayEnd                 ].Name="SnFKTrayEnd";
    Sen[SnFKAlarmReset              ].Name="SnFKAlarmReset";
    Sen[SnFKCoverOpen               ].Name="SnFKCoverOpen";
    Sen[SnRKPowerOff                ].Name="SnRKPowerOff";
    Sen[SnRKPowerOn                 ].Name="SnRKPowerOn";
    Sen[SnRKReset                   ].Name="SnRKReset";
    Sen[SnRKPause                   ].Name="SnRKPause";
    Sen[SnRKHome                    ].Name="SnRKHome";
    Sen[SnRKStart                   ].Name="SnRKStart";
    Sen[SnRKOneCycle                ].Name="SnRKOneCycle";
    Sen[SnRKRetry                   ].Name="SnRKRetry";
    Sen[SnRKSkip                    ].Name="SnRKSkip";
    Sen[SnRKCleanOut                ].Name="SnRKCleanOut";
    Sen[SnRKTrayFeed                ].Name="SnRKTrayFeed";
    Sen[SnRKTrayEnd                 ].Name="SnRKTrayEnd";
    Sen[SnRKAlarmReset              ].Name="SnRKAlarmReset";
    Sen[SnRKCoverOpen               ].Name="SnRKCoverOpen";
    Sen[SnRKManualStep              ].Name="SnRKManualStep";
    Sen[SnRKManualTStart            ].Name="SnRKManualTStart";
    Sen[SnLoaderTrayHasTray         ].Name="SnLoaderTrayHasTray";
    Sen[SnLoaderCarHasTray          ].Name="SnLoaderCarHasTray";
    Sen[SnInPutSHT1S1               ].Name="SnInPutSHT1S1";
    Sen[SnInPutSHT1S2               ].Name="SnInPutSHT1S2";
    Sen[SnInPutSHT1S3               ].Name="SnInPutSHT1S3";
    Sen[SnInPutSHT1S4               ].Name="SnInPutSHT1S4";
    Sen[SnInPutSHT1S5               ].Name="SnInPutSHT1S5";
    Sen[SnInPutSHT1S6               ].Name="SnInPutSHT1S6";
    Sen[SnInPutSHT1S7               ].Name="SnInPutSHT1S7";
    Sen[SnInPutSHT1S8               ].Name="SnInPutSHT1S8";
    Sen[SnInPutSHT1S9               ].Name="SnInPutSHT1S9";
    Sen[SnInPutSHT2S1               ].Name="SnInPutSHT2S1";
    Sen[SnInPutSHT2S2               ].Name="SnInPutSHT2S2";
    Sen[SnInPutSHT2S3               ].Name="SnInPutSHT2S3";
    Sen[SnInPutSHT2S4               ].Name="SnInPutSHT2S4";
    Sen[SnInPutSHT2S5               ].Name="SnInPutSHT2S5";
    Sen[SnInPutSHT2S6               ].Name="SnInPutSHT2S6";
    Sen[SnInPutSHT2S7               ].Name="SnInPutSHT2S7";
    Sen[SnInPutSHT2S8               ].Name="SnInPutSHT2S8";
    Sen[SnInPutSHT2S9               ].Name="SnInPutSHT2S9";
    Sen[SnOutPutSHT1S1              ].Name="SnOutPutSHT1S1";
    Sen[SnOutPutSHT1S2              ].Name="SnOutPutSHT1S2";
    Sen[SnOutPutSHT1S3              ].Name="SnOutPutSHT1S3";
    Sen[SnOutPutSHT1S4              ].Name="SnOutPutSHT1S4";
    Sen[SnOutPutSHT1S5              ].Name="SnOutPutSHT1S5";
    Sen[SnOutPutSHT1S6              ].Name="SnOutPutSHT1S6";
    Sen[SnOutPutSHT1S7              ].Name="SnOutPutSHT1S7";
    Sen[SnOutPutSHT2S1              ].Name="SnOutPutSHT2S1";
    Sen[SnOutPutSHT2S2              ].Name="SnOutPutSHT2S2";
    Sen[SnOutPutSHT2S3              ].Name="SnOutPutSHT2S3";
    Sen[SnOutPutSHT2S4              ].Name="SnOutPutSHT2S4";
    Sen[SnOutPutSHT2S5              ].Name="SnOutPutSHT2S5";
    Sen[SnOutPutSHT2S6              ].Name="SnOutPutSHT2S6";
    Sen[SnOutPutSHT2S7              ].Name="SnOutPutSHT2S7";
    Sen[SnOutPutSHT1ZS1             ].Name="SnOutPutSHT1ZS1";
    Sen[SnOutPutSHT1ZS2             ].Name="SnOutPutSHT1ZS2";
    Sen[SnOutPutSHT2ZS1             ].Name="SnOutPutSHT2ZS1";
    Sen[SnOutPutSHT2ZS2             ].Name="SnOutPutSHT2ZS2";
    Sen[SnAuto1TrayDetect           ].Name="SnAuto1TrayDetect";
    Sen[SnAuto2TrayDetect           ].Name="SnAuto2TrayDetect";
    Sen[SnAuto3TrayDetect           ].Name="SnAuto3TrayDetect";
    Sen[SnAuto1IsFull               ].Name="SnAuto1IsFull";
    Sen[SnAuto2IsFull               ].Name="SnAuto2IsFull";
    Sen[SnAuto3IsFull               ].Name="SnAuto3IsFull";
    Sen[SnFixedTray1Detect          ].Name="SnFixedTray1Detect";
    Sen[SnFixedTray2Detect          ].Name="SnFixedTray2Detect";
    Sen[SnFixedTray3Detect          ].Name="SnFixedTray3Detect";
    Sen[SnFixedTray4Detect          ].Name="SnFixedTray4Detect";
    Sen[SnFixedTray5Detect          ].Name="SnFixedTray5Detect";
    Sen[SnFixedTray6Detect          ].Name="SnFixedTray6Detect";
    Sen[SnSafeDoor1                 ].Name="SnSafeDoor1";
    Sen[SnSafeDoor2                 ].Name="SnSafeDoor2";
    Sen[SnSafeDoor3                 ].Name="SnSafeDoor3";
    Sen[SnSafeDoor4                 ].Name="SnSafeDoor4";
    Sen[SnSafeDoor5                 ].Name="SnSafeDoor5";
    Sen[SnSafeDoor6                 ].Name="SnSafeDoor6";
    Sen[SnSafeDoor7                 ].Name="SnSafeDoor7";
    Sen[SnSafeDoor8                 ].Name="SnSafeDoor8";
    Sen[SnSafeDoor9                 ].Name="SnSafeDoor9";
    Sen[SnSafeDoor10                ].Name="SnSafeDoor10";
    Sen[SnAirIsEnough               ].Name="SnAirIsEnough";
    Sen[SnFrontRightEMG             ].Name="SnFrontRightEMG";
    Sen[SnRearLeftEMG               ].Name="SnRearLeftEMG";
    Sen[SnFMotorDown                ].Name="SnFMotorDown";
    Sen[SnBMotorDown                ].Name="SnBMotorDown";
    Sen[SnMotorPower                ].Name="SnMotorPower";
    Sen[SnSystemPower               ].Name="SnSystemPower";
    Sen[SnEmptyTrayHasTray1         ].Name="SnEmptyTrayHasTray1";
    Sen[SnEmptyTrayIsFull1          ].Name="SnEmptyTrayIsFull1";
    Sen[SnEmptyTrayIsLock1          ].Name="SnEmptyTrayIsLock1";
    Sen[SnEmptyTrayHasTray2         ].Name="SnEmptyTrayHasTray2";
    Sen[SnEmptyTrayIsFull2          ].Name="SnEmptyTrayIsFull2";
    Sen[SnRearPadActive             ].Name="SnRearPadActive";
    Sen[SnFrontLeftEMG              ].Name="SnFrontLeftEMG";
    Sen[SnRearRightEMG              ].Name="SnRearRightEMG";
    Sen[SnEPDieForce                ].Name="SnEPDieForce";
//    Sen[SnCatchTrayDown             ].Name="SnCatchTrayDown";
    Sen[SenBit0                     ].Name="SenBit0";
    Sen[SenBit1                     ].Name="SenBit1";
    Sen[SenBit2                     ].Name="SenBit2";
    Sen[SenBit3                     ].Name="SenBit3";
    Sen[SenBit4                     ].Name="SenBit4";
    Sen[SenBit5                     ].Name="SenBit5";
    Sen[SenBit6                     ].Name="SenBit6";
    Sen[SenBit7                     ].Name="SenBit7";
    Sen[SenBit8                     ].Name="SenBit8";
    Sen[SenBit9                     ].Name="SenBit9";
    Sen[SnLoaderPreDete             ].Name="SnLoaderPreDete";
    Sen[SnAuto1_Tray_Car            ].Name="SnAuto1_Tray_Car";
    Sen[SnAuto2_Tray_Car            ].Name="SnAuto2_Tray_Car";
    Sen[SnAuto3_Tray_Car            ].Name="SnAuto3_Tray_Car";
    Sen[SnAuto1PreDete              ].Name="SnAuto1PreDete";
    Sen[SnAuto2PreDete              ].Name="SnAuto2PreDete";
    Sen[SnAuto3PreDete              ].Name="SnAuto3PreDete";
    Sen[SnLoaderSureTray            ].Name="SnLoaderSureTray";
//    Sen[SnMotorYAlarm               ].Name="SnMotorYAlarm";
//    Sen[SnMotorZ1Alarm              ].Name="SnMotorZ1Alarm";
    Sen[SnAuto1FixCyPush            ].Name="SnAuto1FixCyPush";
    Sen[SnAuto2FixCyPush            ].Name="SnAuto2FixCyPush";
    Sen[SnAuto3FixCyPush            ].Name="SnAuto3FixCyPush";
    Sen[SnLoaderFixCyPush           ].Name="SnLoaderFixCyPush";
    Sen[SenBit10                    ].Name="SenBit10";
    Sen[SenBit11                    ].Name="SenBit11";
    Sen[SenBit12                    ].Name="SenBit12";
    Sen[SenBit13                    ].Name="SenBit13";
    Sen[SenBit14                    ].Name="SenBit14";
    Sen[SenBit15                    ].Name="SenBit15";
    Sen[SenBit16                    ].Name="SenBit16";
    Sen[SenBit17                    ].Name="SenBit17";
    Sen[SenBit18                    ].Name="SenBit18";
    Sen[SenBit19                    ].Name="SenBit19";
    Sen[SenEmptyHasTray             ].Name="SenEmptyHasTray";
    Sen[SenEmptyCWDete              ].Name="SenEmptyCWDete";
    Sen[SenEmptySelectHasTray       ].Name="SenEmptySelectHasTray";
    Sen[SenEmptyCCWDete             ].Name="SenEmptyCCWDete";
    Sen[SenEmptyCarHasTray          ].Name="SenEmptyCarHasTray";
    Sen[SenColorHasTray             ].Name="SenColorHasTray";
    Sen[SenColorCWDete              ].Name="SenColorCWDete";
    Sen[SenColorSelectHasTray       ].Name="SenColorSelectHasTray";
    Sen[SenColorCarHasTray          ].Name="SenColorCarHasTray";
    Sen[SenEmptyFixCyPush           ].Name="SenEmptyFixCyPush";
    Sen[SnAuto1TrayHasTray          ].Name="SnAuto1TrayHasTray";
    Sen[SnAuto1CWPreDetect          ].Name="SnAutoCWPreDetect";
    Sen[SnHeaterDoor                ].Name="SnHeaterDoor";
    Sen[SnCatchTrayFix1On           ].Name="SnCatchTrayFix1On";
    Sen[SnCatchTrayFix2On           ].Name="SnCatchTrayFix2On";
    Sen[SnSafeLock                  ].Name="SnSafeLock";
    Sen[SnRKSafeLock                ].Name="SnRKSafeLock";                      //KenHsieh 20211228 : 區分實體IO與通訊面板
//    Sen[SnIndexY1CHK                ].Name="SnIndexY1CHK";
//    Sen[SnIndexZ1CHK                ].Name="SnIndexZ1CHK";
//    Sen[SnIndexZ2CHK                ].Name="SnIndexZ2CHK";
//    Sen[SnIndexY2CHK                ].Name="SnIndexY2CHK";

    Sen[SnFPLevelOpe                ].Name="SnFPLevelOpe";
    Sen[SnFPLevelEng                ].Name="SnFPLevelEng";                      //Steven 20190503 : 指紋辨識權限
    Sen[SnFPLevelSup                ].Name="SnFPLevelSup";
    Sen[SnFPLevelHon                ].Name="SnFPLevelHon";

    Sen[SnIndexHeaterFan            ].Name="SnIndexHeaterFan";
//    Sen[SnCatchTrayOnDetect         ].Name="SnCatchTrayOnDetect";
    Sen[SnHeaterDoor2               ].Name="SnHeaterDoor2";
    Sen[SnHeaterDoor3               ].Name="SnHeaterDoor3";
    Sen[SnHeaterDoor4               ].Name="SnHeaterDoor4";                     //wei 20200616 : For ATC3.3 MR, 要第四個加熱門
    Sen[SnAuto1TrackDetect          ].Name="SnAuto1TrackDetect";
    Sen[SnAuto2TrackDetect          ].Name="SnAuto2TrackDetect";
    Sen[SnAuto3TrackDetect          ].Name="SnAuto3TrackDetect";
    Sen[SnEmptyIsFull               ].Name="SnEmptyIsFull";
    Sen[SnColorIsFull               ].Name="SnColorIsFull";
    Sen[SenColorCCWDete             ].Name="SenColorCCWDete";
    Sen[SenColorFixCyPush           ].Name="SenColorFixCyPush";
    Sen[SenEmpty1HasTray            ].Name="SenEmpty1HasTray";
    Sen[SenEmpty1CCWDete            ].Name="SenEmpty1CCWDete";
    Sen[SenEmpty1CarHasTray         ].Name="SenEmpty1CarHasTray";
    Sen[SnAutoUpSafedetect0         ].Name="SnAutoUpSafedetect0";
    Sen[SnAutoUpSafedetect1         ].Name="SnAutoUpSafedetect1";
    Sen[SnAutoUpSafedetect2         ].Name="SnAutoUpSafedetect2";
    Sen[SnLoaderUpSafedetect        ].Name="SnLoaderUpSafedetect";
    Sen[SnEmptyUpSafedetect         ].Name="SnEmptyUpSafedetect";
    Sen[SnColorUpSafedetect         ].Name="SnColorUpSafedetect";
    Sen[SnIonFanAlarm               ].Name="SnIonFanAlarm";
    Sen[SnIonFanLevelAlarm          ].Name="SnIonFanLevelAlarm";
    Sen[SnIonBarrierAlarm           ].Name="SnIonBarrierAlarm";
    Sen[SnIonBarrierLevelAlarm      ].Name="SnIonBarrierLevelAlarm";
    Sen[SnIonBarrierConditionAlarm  ].Name="SnIonBarrierConditionAlarm";
    Sen[SnAutoColorTrayDetect0      ].Name="SnAutoColorTrayDetect0";
    Sen[SnAutoColorTrayDetect1      ].Name="SnAutoColorTrayDetect1";
    Sen[SnAutoColorTrayDetect2      ].Name="SnAutoColorTrayDetect2";
    Sen[SnAutoColorTrayDetect4      ].Name="SnAutoColorTrayDetect4";            //RogerYang 20250825 : 新增Auto4~6
    Sen[SnAutoColorTrayDetect5      ].Name="SnAutoColorTrayDetect5";
    Sen[SnAutoColorTrayDetect6      ].Name="SnAutoColorTrayDetect6";
//    Sen[SnArm1OverPress1            ].Name="SnArm1OverPress1";
//    Sen[SnArm1OverPress2            ].Name="SnArm1OverPress2";
//    Sen[SnArm2OverPress1            ].Name="SnArm2OverPress1";
//    Sen[SnArm2OverPress2            ].Name="SnArm2OverPress2";
    Sen[SnEPAlarm                   ].Name="SnEPAlarm";
    Sen[SnCheckTrayDirection        ].Name="SnCheckTrayDirection";
    Sen[SnCheckLoadDirection        ].Name="SnCheckLoadDirection";
    Sen[SnIonFan6Alarm              ].Name="SnIonFan6Alarm";
    Sen[SnIonFan7Alarm              ].Name="SnIonFan7Alarm";
    Sen[SnIonFan8Alarm              ].Name="SnIonFan8Alarm";
    Sen[SnIonFan9Alarm              ].Name ="SnIonFan9Alarm";
    Sen[SnIonFan10Alarm             ].Name="SnIonFan10Alarm";
    Sen[SnIonFan11Alarm             ].Name="SnIonFan11Alarm";
    Sen[SnIonFan12Alarm             ].Name="SnIonFan12Alarm";
    Sen[SnNegativePressureAir       ].Name="SnNegativePressureAir";
    Sen[SnNegativePressureAir2      ].Name="SnNegativePressureAir2";            //Sam 20171110 (Steven) : 新增氣壓 Sensor
    Sen[SnLoaderEdgePush            ].Name="SnLoaderEdgePush";
    Sen[SnAuto1EdgePush             ].Name="SnAuto1EdgePush";
    Sen[SnAuto2EdgePush             ].Name="SnAuto2EdgePush";
    Sen[SnAuto3EdgePush             ].Name="SnAuto3EdgePush";
    //----- by dell ccd realtime-------------
    Sen[SnRealTimeCCDStop           ].Name="SnRealTimeCCDStop";
    Sen[SnRealTimeCCDIndexArm       ].Name="SnRealTimeCCDIndexArm";
    Sen[SnRTCCDTempCtrl             ].Name="SnRTCCDTempCtrl";
    //---------------------------------------
    Sen[SnUnLoaderFloating          ].Name="SnUnLoaderFloating";                //Steven 20110705 : Tray置偏
    Sen[SnRotateCheck               ].Name="SnRotateCheck";                     //ChungHung 20110922 : 轉轉蝦頭要檢查有沒有轉頭 Check Sensor
    Sen[SnCheckConnectIndexArm_1    ].Name="SnCheckConnectIndexArm_1";          //20111130  Dell
    Sen[SnCheckConnectIndexArm_2    ].Name="SnCheckConnectIndexArm_2";          //20111130  Dell
    Sen[SnFixFloating1              ].Name="SnFixFloating1";                    //Steven 20120131 : Fix Tray置偏偵測
    Sen[SnFixFloating2              ].Name="SnFixFloating2";
    Sen[SnFixFloating3              ].Name="SnFixFloating3";                    //Sam 20240129 : 新增第三組 Fix floating Sensor
    Sen[SnATCAlarm1                 ].Name="SnATCAlarm1";                       //jou 2012-03-13 ATC Alarm 1 Sensor
    Sen[SnATCAlarm2                 ].Name="SnATCAlarm2";                       //jou 2012-03-13 ATC Alarm 2 Sensor
    Sen[SnATCAlarm3                 ].Name="SnATCAlarm3";                       //jou 2012-03-13 ATC Alarm 3 Sensor
    Sen[SnATCAlarm4                 ].Name="SnATCAlarm4";                       //jou 2012-03-13 ATC Alarm 4 Sensor
    Sen[SenInArmYPitch60            ].Name="SenInArmYPitch60";                  //ChungHung 20120505 : HT9045 WS Only
    Sen[SenOutArmYPitch60           ].Name="SenOutArmYPitch60";                 //ChungHung 20120505 : HT9045 WS Only
    Sen[SnAutoDockingOff            ].Name="SnAutoDockingOff";                  //ChungHung 20120718 add UseAutoDocking Check Sensor
    Sen[SnAutoDockingOn             ].Name="SnAutoDockingOn";                   //ChungHung 20120718 add UseAutoDocking Check Sensor
    //---------Auto2 kevin 20120725---------------------------------------
    Sen[SnAuto2TrayHasTray          ].Name="SnAuto2TrayHasTray";
    Sen[SnAuto3TrayHasTray          ].Name="SnAuto3TrayHasTray";
    Sen[SnLoaderIsFull              ].Name="SnLoaderIsFull";
    Sen[SnAuto2CWPreDetect          ].Name="SnAuto2CWPreDetect";
    Sen[SnAuto3CWPreDetect          ].Name="SnAuto3CWPreDetect";
    Sen[SenLoaderCCWDete            ].Name="SenLoaderCCWDete";

    Sen[SnTesterDocking             ].Name="SnTesterDocking";                   //jou 2012-09-13 Tester Docking
    Sen[SnTrain                     ].Name="SnTrain";
    Sen[SnFix3FullPlace             ].Name="SnFix3FullPlace";                   //Steven 20121020 : Fix3滿盤
    Sen[SnGroundMan                 ].Name="SnGroundMan";                       //wei 20150424 add SnGroundMan偵測
    Sen[SnGroundMan2                ].Name="SnGroundMan2";
    Sen[SnGroundMan3                ].Name="SnGroundMan3";
    Sen[SnGroundMan4                ].Name="SnGroundMan4";

    Sen[SnTesterDryAir              ].Name="SnTesterDryAir";                    //Ifor 20200115 add: add Tester Dry Air Control

    Sen[SnIonFanPower01             ].Name="SnIonFanPower01";                   //Steven 20130201 : Kasuga離子風扇電源偵測
    Sen[SnIonFanPower02             ].Name="SnIonFanPower02";
    Sen[SnIonFanPower03             ].Name="SnIonFanPower03";
    Sen[SnIonFanPower04             ].Name="SnIonFanPower04";
    Sen[SnIonFanPower05             ].Name="SnIonFanPower05";
    Sen[SnIonFanPower06             ].Name="SnIonFanPower06";
    Sen[SnIonFanPower07             ].Name="SnIonFanPower07";
    Sen[SnIonFanPower08             ].Name="SnIonFanPower08";
    Sen[SnIonFanPower09             ].Name="SnIonFanPower09";
    Sen[SnIonFanPower10             ].Name="SnIonFanPower10";
    Sen[SnIonFanPower11             ].Name="SnIonFanPower11";
    Sen[SnIonFanPower12             ].Name="SnIonFanPower12";
    Sen[SnFix3FullPlace             ].Name="SnFix3FullPlace";                   //Steven 20121020 : Fix3滿盤

    Sen[SnRotateRowIn1              ].Name="SnRotateRowIn1";                    //kevin 20130524
    Sen[SnRotateRowIn2              ].Name="SnRotateRowIn2";
    Sen[SnRotateRowOut1             ].Name="SnRotateRowOut1";
    Sen[SnRotateRowOut2             ].Name="SnRotateRowOut2";

    Sen[SnSocket1                   ].Name="SnSocket1";                         //kevin 20130429   socket sensor
    Sen[SnSocket2                   ].Name="SnSocket2";                         //kevin 20130429   socket sensor
    Sen[SnSocket3                   ].Name="SnSocket3";                         //kevin 20130429   socket sensor
    Sen[SnSocket4                   ].Name="SnSocket4";                         //kevin 20130429   socket sensor
    Sen[SnSocket5                   ].Name="SnSocket5";                         //kevin 20130429   socket sensor
    Sen[SnSocket6                   ].Name="SnSocket6";                         //kevin 20130429   socket sensor
    Sen[SnSocket7                   ].Name="SnSocket7";                         //kevin 20130429   socket sensor
    Sen[SnSocket8                   ].Name="SnSocket8";                         //kevin 20130429   socket sensor

    Sen[SnSocket9                   ].Name="SnSocket9";                         //Steven 20200610 : Socket sensor 改成16顆
    Sen[SnSocket10                  ].Name="SnSocket10";
    Sen[SnSocket11                  ].Name="SnSocket11";
    Sen[SnSocket12                  ].Name="SnSocket12";
    Sen[SnSocket13                  ].Name="SnSocket13";
    Sen[SnSocket14                  ].Name="SnSocket14";
    Sen[SnSocket15                  ].Name="SnSocket15";
    Sen[SnSocket16                  ].Name="SnSocket16";

    Sen[SnSocket17                  ].Name="SnSocket17";
    Sen[SnSocket18                  ].Name="SnSocket18";
    Sen[SnSocket19                  ].Name="SnSocket19";
    Sen[SnSocket20                  ].Name="SnSocket20";
    Sen[SnSocket21                  ].Name="SnSocket21";
    Sen[SnSocket22                  ].Name="SnSocket22";
    Sen[SnSocket23                  ].Name="SnSocket23";
    Sen[SnSocket24                  ].Name="SnSocket24";

    Sen[SnCrossSHT1S1               ].Name="SnCrossSHT1S1";                     //2013-07-16    Dell    Shuttle cross sensor
    Sen[SnCrossSHT1S2               ].Name="SnCrossSHT1S2";                     //2013-07-16    Dell    Shuttle cross sensor
    Sen[SnCrossSHT2S1               ].Name="SnCrossSHT2S1";                     //2013-07-16    Dell    Shuttle cross sensor
    Sen[SnCrossSHT2S2               ].Name="SnCrossSHT2S2";                     //2013-07-16    Dell    Shuttle cross sensor

    Sen[SnServo                     ].Name="SnServo";                           //kevin 20140121 偵測servo on 訊號
    Sen[SnAutoTeach                 ].Name="SnAutoTeach";                       //kevin 201400512 AUTOTEACH IN/OUT ARM SENSOR

    Sen[SnEOF1                      ].Name="SnEOF1";                            //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnEOF2                      ].Name="SnEOF2";                            //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnEOF3                      ].Name="SnEOF3";                            //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnAOIResult1                ].Name="SnAOIResult1";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnAOIResult2                ].Name="SnAOIResult2";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    Sen[SnAOIResult3                ].Name="SnAOIResult3";                      //2014-03-04    Dell    for SPIL WLP Add 5S Inspection

    Sen[SnFix3Lock                  ].Name="SnFix3Lock";                        //Steven 20140512 : For HT-9046LA

    Sen[SenAutoDocking240KG         ].Name="SenAutoDocking240KG";               //ChungHung 20140709 add for hardware control OTD
    Sen[SenAutoDocking360KG         ].Name="SenAutoDocking360KG";               //ChungHung 20140709 add for hardware control OTD

    Sen[SnFixColorTrayDetect0       ].Name="SnFixColorTrayDetect0";             //20140903 wei colcr Tray
    Sen[SnFixColorTrayDetect1       ].Name="SnFixColorTrayDetect1";             //20140903 wei colcr Tray
    Sen[SnFixColorTrayDetect2       ].Name="SnFixColorTrayDetect2";             //20140903 wei colcr Tray

    Sen[SnFix4ColorTrayDetect       ].Name="SnFix4ColorTrayDetect";             //20140903 wei colcr Tray
    Sen[SnFix5ColorTrayDetect       ].Name="SnFix5ColorTrayDetect";             //20140903 wei colcr Tray
    Sen[SnFix6ColorTrayDetect       ].Name="SnFix6ColorTrayDetect";             //20140903 wei colcr Tray

    Sen[SnLoaderColorTrayDetect     ].Name="SnLoaderColorTrayDetect";           //20140903 wei colcr Tray
    Sen[SnEmptyColorTrayDetect      ].Name="SnEmptyColorTrayDetect";            //20140903 wei colcr Tray
    Sen[SnColorColorTrayDetect      ].Name="SnColorColorTrayDetect";            //20140903 wei colcr Tray
    Sen[SnOCRTigger                 ].Name="SnOCRTigger";                       //wei 20150720 OCR觸發
    Sen[SnLowSpeed                  ].Name="SnLowSpeed";                        //wei 20150720 OCR觸發
    Sen[SnOCRPosition               ].Name="SnOCRPosition";                     //wei 20150720 OCR觸發
    Sen[SnLoaderTrayHasTray_ART     ].Name="SnLoaderTrayHasTray_ART";           //wei 20151210
    Sen[SnSLK1UnclampButton         ].Name="SnSLK1UnclampButton";               //JerryYang 20160524
    Sen[SnSLK2UnclampButton         ].Name="SnSLK2UnclampButton";               //JerryYang 20160524
    Sen[SnSocketClampPush1          ].Name="SnSocketClampPush1";                //JerryYang 20160606
    Sen[SnSocketClampPush2          ].Name="SnSocketClampPush2";                //JerryYang 20160606
    Sen[SnSocketClampPull1          ].Name="SnSocketClampPull1";                //JerryYang 20160606
    Sen[SnSocketClampPull2          ].Name="SnSocketClampPull2";                //JerryYang 20160606
    Sen[SnSocketHasClamp1           ].Name="SnSocketHasClamp1";                 //JerryYang 20160606
    Sen[SnSocketHasClamp2           ].Name="SnSocketHasClamp2";                 //JerryYang 20160606

    Sen[SnHingeTopTray              ].Name="SnHingeTopTray";                    //wei 20170418 Hinge cassette
    Sen[SnSafeDoor11                ].Name="SnSafeDoor11";                      //wei 20180702 MR
    Sen[SnEmptyFull                 ].Name="SnEmptyFull";                       //wei 20180702 MR
    Sen[SnCassetteArmHave           ].Name="SnCassetteArmHave";                 //wei 20180702 MR
    Sen[SnBuffer1HaveCassette       ].Name="SnBuffer1HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer2HaveCassette       ].Name="SnBuffer2HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer3HaveCassette       ].Name="SnBuffer3HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer4HaveCassette       ].Name="SnBuffer4HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer5HaveCassette       ].Name="SnBuffer5HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer6HaveCassette       ].Name="SnBuffer6HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer7HaveCassette       ].Name="SnBuffer7HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer8HaveCassette       ].Name="SnBuffer8HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer9HaveCassette       ].Name="SnBuffer9HaveCassette";             //wei 20180702 MR
    Sen[SnBuffer10HaveCassette      ].Name="SnBuffer10HaveCassette";            //wei 20180702 MR
    Sen[SnLoadPortCatch             ].Name="SnLoadPortCatch";                   //wei 20180702 MR
    Sen[SnLoadPortPresent           ].Name="SnLoadPortPresent";                 //wei 20180702 MR
    Sen[SnLoadPortPlacement1        ].Name="SnLoadPortPlacement1";              //wei 20180702 MR
    Sen[SnLoadPortPlacement2        ].Name="SnLoadPortPlacement2";              //wei 20180702 MR
    Sen[SnTrayBracketHave           ].Name="SnTrayBracketHave";                 //wei 20180702 MR
    Sen[SnStackedTrayHave           ].Name="SnStackedTrayHave";                 //wei 20180702 MR
    Sen[SnBuffer6HaveTray           ].Name="SnBuffer6HaveTray";                 //wei 20180702 MR
    Sen[SnSafeDoor12                ].Name="SnSafeDoor12";                      //wei 20180702 MR
    Sen[SnSafeDoor13                ].Name="SnSafeDoor13";                      //wei 20180702 MR
    Sen[SnSafeDoor14                ].Name="SnSafeDoor14";                      //wei 20180702 MR
    Sen[SnSafeDoor15                ].Name="SnSafeDoor15";                      //wei 20180702 MR
    Sen[SnCassetteCatch             ].Name="SnCassetteCatch";                   //wei 20180702 MR

    Sen[SnE84VALID                  ].Name="SnE84VALID";                        //wei 20180702 E84
    Sen[SnE84CS0                    ].Name="SnE84CS0";                          //wei 20180702 E84
    Sen[SnE84CS1                    ].Name="SnE84CS1";                          //wei 20180702 E84
    Sen[SnE84AMAVBL                 ].Name="SnE84AMAVBL";                       //wei 20180702 E84
    Sen[SnE84TRREQ                  ].Name="SnE84TRREQ";                        //wei 20180702 E84
    Sen[SnE84BUSY                   ].Name="SnE84BUSY";                         //wei 20180702 E84
    Sen[SnE84COMPT                  ].Name="SnE84COMPT";                        //wei 20180702 E84
    Sen[SnE84CONT                   ].Name="SnE84CONT";                         //wei 20180702 E84
    Sen[SnE84GO                     ].Name="SnE84GO";                           //wei 20180702 E84

    Sen[SnCassette01                ].Name="SnCassette01";                      //wei 20180702 MR
    Sen[SnCassette02                ].Name="SnCassette02";                      //wei 20180702 MR
    Sen[SnCassette03                ].Name="SnCassette03";                      //wei 20180702 MR
    Sen[SnCassette04                ].Name="SnCassette04";                      //wei 20180702 MR
    Sen[SnCassette05                ].Name="SnCassette05";                      //wei 20180702 MR
    Sen[SnCassette06                ].Name="SnCassette06";                      //wei 20180702 MR
    Sen[SnCassette07                ].Name="SnCassette07";                      //wei 20180702 MR
    Sen[SnCassette08                ].Name="SnCassette08";                      //wei 20180702 MR

    Sen[SnCassette09                ].Name="SnCassette09";                      //wei 20180702 MR
    Sen[SnCassette10                ].Name="SnCassette10";                      //wei 20180702 MR
    Sen[SnMRStart                   ].Name="SnMRStart";                         //wei 20180702 MR
    Sen[SnMRPause                   ].Name="SnMRPause";                         //wei 20180702 MR
    Sen[SnMRUp                      ].Name="SnMRUp";                            //wei 20180702 MR
    Sen[SnMRDown                    ].Name="SnMRDown";                          //wei 20180702 MR

    Sen[SnCassetteArmCatch          ].Name="SnCassetteArmCatch";                //Sam 20190112 LM
    Sen[SnLoadPortHaveCassette1_1   ].Name="SnLoadPortHaveCassette1_1";
    Sen[SnLoadPortHaveCassette1_2   ].Name="SnLoadPortHaveCassette1_2";
    Sen[SnLoadPortHaveCassette2_1   ].Name="SnLoadPortHaveCassette2_1";
    Sen[SnLoadPortHaveCassette2_2   ].Name="SnLoadPortHaveCassette2_2";
    Sen[SnLoadPortHaveCassette3_1   ].Name="SnLoadPortHaveCassette3_1";
    Sen[SnLoadPortHaveCassette3_2   ].Name="SnLoadPortHaveCassette3_2";
    Sen[SnLoadPortHaveCassette4_1   ].Name="SnLoadPortHaveCassette4_1";
    Sen[SnLoadPortHaveCassette4_2   ].Name="SnLoadPortHaveCassette4_2";
    Sen[SnUnloadPortHaveCassette1_1 ].Name="SnUnloadPortHaveCassette1_1";
    Sen[SnUnloadPortHaveCassette1_2 ].Name="SnUnloadPortHaveCassette1_2";
    Sen[SnUnloadPortHaveCassette2_1 ].Name="SnUnloadPortHaveCassette2_1";
    Sen[SnUnloadPortHaveCassette2_2 ].Name="SnUnloadPortHaveCassette2_2";
    Sen[SnUnloadPortHaveCassette3_1 ].Name="SnUnloadPortHaveCassette3_1";
    Sen[SnUnloadPortHaveCassette3_2 ].Name="SnUnloadPortHaveCassette3_2";
    Sen[SnUnloadPortHaveCassette4_1 ].Name="SnUnloadPortHaveCassette4_1";
    Sen[SnUnloadPortHaveCassette4_2 ].Name="SnUnloadPortHaveCassette4_2";

    Sen[SnLoadRobotHaveCassette1    ].Name="SnLoadRobotHaveCassette1";
    Sen[SnLoadRobotHaveCassette2    ].Name="SnLoadRobotHaveCassette2";
    Sen[SnUnloadRobotHaveCassette1  ].Name="SnUnloadRobotHaveCassette1";
    Sen[SnUnloadRobotHaveCassette2  ].Name="SnUnloadRobotHaveCassette2";

    Sen[SnLoadRobotPosUp            ].Name="SnLoadRobotPosUp";
    Sen[SnLoadRobotPosDown          ].Name="SnLoadRobotPosDown";
    Sen[SnUnloadRobotPosUp          ].Name="SnUnloadRobotPosUp";
    Sen[SnUnloadRobotPosDown        ].Name="SnUnloadRobotPosDown";

    Sen[SnSafeDoor16                ].Name="SnSafeDoor16";
    Sen[SnSafeDoor17                ].Name="SnSafeDoor17";
    Sen[SnSafeDoor18                ].Name="SnSafeDoor18";
    Sen[SnSafeDoor19                ].Name="SnSafeDoor19";

    Sen[SnIndex1Connect1            ].Name="SnIndex1Connect1";                  //RogerYang 20161212 (Steven) 偵測Index1 SLK獨立加熱或共用加熱
    Sen[SnIndex1Connect2            ].Name="SnIndex1Connect2";                  //RogerYang 20161212 (Steven) 偵測Index1 SLK獨立加熱或共用加熱
    Sen[SnIndex2Connect1            ].Name="SnIndex2Connect1";                  //RogerYang 20161212 (Steven) 偵測Index2 SLK獨立加熱或共用加熱
    Sen[SnIndex2Connect2            ].Name="SnIndex2Connect2";                  //RogerYang 20161212 (Steven) 偵測Index2 SLK獨立加熱或共用加熱
    Sen[SnTrayCover                 ].Name="SnTrayCover";                       //Steven 20170623 (wei) : Add for catch tray with cover

    Sen[SnLoaderIsPreAlarm          ].Name="SnLoaderIsPreAlarm";                //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnEmptyIsPreAlarm           ].Name="SnEmptyIsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnColorIsPreAlarm           ].Name="SnColorIsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnAuto1IsPreAlarm           ].Name="SnAuto1IsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnAuto2IsPreAlarm           ].Name="SnAuto2IsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor
    Sen[SnAuto3IsPreAlarm           ].Name="SnAuto3IsPreAlarm";                 //wei 20170802 (Steven) Pre alarm sensor

    //------------------------------------
    //Alick 20161011 (Steven) : TTL支援8Site
    //------------------------------------
    Sen[SenBit20                    ].Name="SenBit20";
    Sen[SenBit21                    ].Name="SenBit21";
    Sen[SenBit22                    ].Name="SenBit22";
    Sen[SenBit23                    ].Name="SenBit23";
    Sen[SenBit24                    ].Name="SenBit24";
    Sen[SenBit25                    ].Name="SenBit25";
    Sen[SenBit26                    ].Name="SenBit26";
    Sen[SenBit27                    ].Name="SenBit27";
    Sen[SenBit28                    ].Name="SenBit28";
    Sen[SenBit29                    ].Name="SenBit29";
    Sen[SenBit30                    ].Name="SenBit30";
    Sen[SenBit31                    ].Name="SenBit31";
    Sen[SenBit32                    ].Name="SenBit32";
    Sen[SenBit33                    ].Name="SenBit33";
    Sen[SenBit34                    ].Name="SenBit34";
    Sen[SenBit35                    ].Name="SenBit35";
    Sen[SenBit36                    ].Name="SenBit36";
    Sen[SenBit37                    ].Name="SenBit37";
    Sen[SenBit38                    ].Name="SenBit38";
    Sen[SenBit39                    ].Name="SenBit39";
    Sen[SnTrayArmSafePos            ].Name="SnTrayArmSafePos";                  //kevin 20171006 (wei) Home tray arm must on
    Sen[SnTJCurrent                 ].Name="SnTJCurrent";                       //Steven 20180124 (Wei) : Check ATC7.0 TJ Current
    Sen[SnLoaderUpPress             ].Name="SnLoaderUpPress";                   //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
    Sen[SnPreciserDetect1           ].Name="SnPreciserDetect1";                 //Frank 20180410 (Steven) : InArm Preciser Station
    Sen[SnPreciserDetect2           ].Name="SnPreciserDetect2";
    Sen[SnLoadCell1                 ].Name="SnLoadCell1";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell2                 ].Name="SnLoadCell2";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell3                 ].Name="SnLoadCell3";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell4                 ].Name="SnLoadCell4";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell5                 ].Name="SnLoadCell5";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell6                 ].Name="SnLoadCell6";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell7                 ].Name="SnLoadCell7";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnLoadCell8                 ].Name="SnLoadCell8";                       //kevin 20190307  read load cell pass  or fail
    Sen[SnHotGun1                   ].Name="SnHotGun1";                         //kevin 20190621   Hot Gun 1 流量偵測
    Sen[SnHotGun2                   ].Name="SnHotGun2";                         //kevin 20130429   Hot Gun 2流量偵測

    Sen[SnAuto1UpPress              ].Name="SnAuto1UpPress";                     //JerryYang 20190423 新增unloader壓tray
    Sen[SnAuto2UpPress              ].Name="SnAuto2UpPress";
    Sen[SnAuto3UpPress              ].Name="SnAuto3UpPress";
    Sen[SnAuto4UpPress              ].Name="SnAuto4UpPress";                    //RogerYang 20250825 : 新增unloader壓tray(4~6)
    Sen[SnAuto5UpPress              ].Name="SnAuto5UpPress";
    Sen[SnAuto6UpPress              ].Name="SnAuto6UpPress";
    Sen[SnWaterLeakageUp            ].Name="SnWaterLeakageUp";                  //wei 20190617 漏水檢測
    Sen[SnWaterLeakageDown          ].Name="SnWaterLeakageDown";                //wei 20190617 漏水檢測
    Sen[SnWaterLeakagePlate         ].Name="SnWaterLeakagePlate";               //wei 20190617 漏水檢測

    Sen[SnLoaderTrackDetect         ].Name="SnLoaderTrackDetect";               //Sam 20200316 : Loader Detect Tray
    Sen[SnAuto1Z_Select1            ].Name="SnAuto1Z_Select1";                  //JerryYang 20200615 ART分離汽缸sensor
    Sen[SnAuto2Z_Select1            ].Name="SnAuto2Z_Select1";
    Sen[SnAuto3Z_Select1            ].Name="SnAuto3Z_Select1";
    Sen[SnAuto1Z_Select2            ].Name="SnAuto1Z_Select2";
    Sen[SnAuto2Z_Select2            ].Name="SnAuto2Z_Select2";
    Sen[SnAuto3Z_Select2            ].Name="SnAuto3Z_Select2";
    Sen[SnAuto1Z_Select3            ].Name="SnAuto1Z_Select3";
    Sen[SnAuto2Z_Select3            ].Name="SnAuto2Z_Select3";
    Sen[SnAuto3Z_Select3            ].Name="SnAuto3Z_Select3";
    Sen[SnAuto1Z_Select4            ].Name="SnAuto1Z_Select4";
    Sen[SnAuto2Z_Select4            ].Name="SnAuto2Z_Select4";
    Sen[SnAuto3Z_Select4            ].Name="SnAuto3Z_Select4";

    Sen[SnInFlipper1DeviceDetect    ].Name="SnInFlipper1DeviceDetect";          //Frank 20210612 : Flipper Function
    Sen[SnInFlipper2DeviceDetect    ].Name="SnInFlipper2DeviceDetect";
    Sen[SnInFlipper3DeviceDetect    ].Name="SnInFlipper3DeviceDetect";
    Sen[SnOutFlipper1DeviceDetect   ].Name="SnOutFlipper1DeviceDetect";
    Sen[SnOutFlipper2DeviceDetect   ].Name="SnOutFlipper2DeviceDetect";
    Sen[SnOutFlipper3DeviceDetect   ].Name="SnOutFlipper3DeviceDetect";

    Sen[SnLoaderCasstteDetect       ].Name="SnLoaderCasstteDetect";
    Sen[SnEmptyCasstteDetect        ].Name="SnEmptyCasstteDetect";
    Sen[SnColorCasstteDetect        ].Name="SnColorCasstteDetect";
    Sen[SnAuto1CasstteDetect        ].Name="SnAuto1CasstteDetect";
    Sen[SnAuto2CasstteDetect        ].Name="SnAuto2CasstteDetect";
    Sen[SnAuto3CasstteDetect        ].Name="SnAuto3CasstteDetect";
    Sen[SnAuto4CasstteDetect        ].Name="SnAuto4CasstteDetect";              //RogerYang 20250825 : 新增Auto4~6
    Sen[SnAuto5CasstteDetect        ].Name="SnAuto5CasstteDetect";
    Sen[SnAuto6CasstteDetect        ].Name="SnAuto6CasstteDetect";
    Sen[SnArm1SLK                   ].Name="SnArm1SLK";
    Sen[SnArm2SLK                   ].Name="SnArm2SLK";

    Sen[SnChamberHeatDetect         ].Name="SnChamberHeatDetect";               //JerryYang 20210107 : ChamberHeatDetect

    Sen[SnMRAuto21                  ].Name="SnMRAuto21";
    Sen[SnE84LREQ                   ].Name="SnE84LREQ";
    Sen[SnE84UREQ                   ].Name="SnE84UREQ";
    Sen[SnE84VA                     ].Name="SnE84VA";
    Sen[SnE84READY                  ].Name="SnE84READY";
    Sen[SnE84VS0                    ].Name="SnE84VS0";
    Sen[SnE84VS1                    ].Name="SnE84VS1";
    Sen[SnE84HOAVBL                 ].Name="SnE84HOAVBL";
    Sen[SnE84ES                     ].Name="SnE84ES";
    Sen[SnE84POWER                  ].Name="SnE84POWER";
    Sen[SnBufferTop                 ].Name="SnBufferTop";
    Sen[SnBufferCatch               ].Name="SnBufferCatch";
    Sen[SnCassetteOpen              ].Name="SnCassetteOpen";
    Sen[SnMultileEmpty_Door         ].Name="SnMultileEmpty_Door";
    Sen[SnMultileEmpty_MagazineTop  ].Name="SnMultileEmpty_MagazineTop";
    Sen[SnMultileEmpty_MagazineLow  ].Name="SnMultileEmpty_MagazineLow";
    Sen[SnMultileEmpty_MagazineCatch].Name="SnMultileEmpty_MagazineCatch";
    Sen[SnMultileEmpty_SelectHasTray].Name="SnMultileEmpty_SelectHasTray";
    Sen[SnMultileEmpty_HasTray      ].Name="SnMultileEmpty_HasTray";
    Sen[SnMultileEmpty_CornerPushCyPush].Name="SnMultileEmpty_CornerPushCyPush";
    Sen[SnMultileEmpty_CatchHasTray ].Name="SnMultileEmpty_CatchHasTray";
    Sen[SnSafeDoor20                ].Name="SnSafeDoor20";
    Sen[SnSafeDoor21                ].Name="SnSafeDoor21";
    Sen[SnLoadPort2Present          ].Name="SnLoadPort2Present";
    Sen[SnLoadPort2Placement1       ].Name="SnLoadPort2Placement1";
    Sen[SnLoadPort2Placement2       ].Name="SnLoadPort2Placement2";
    Sen[SnTrayBracket2Have          ].Name="SnTrayBracket2Have";
    Sen[SnCassetteArmIntoLoadPort   ].Name="SnCassetteArmIntoLoadPort";
    Sen[SnCassetteArmIntoLoadPort2  ].Name="SnCassetteArmIntoLoadPort2";
    Sen[SnBuffer7HaveTray           ].Name="SnBuffer7HaveTray";
    Sen[SnLoadPortAutoManual        ].Name="SnLoadPortAutoManual";
    Sen[SnLoadPort2AutoManual       ].Name="SnLoadPort2AutoManual";
    Sen[SnOHTIntoLoadPort           ].Name="SnOHTIntoLoadPort";
    Sen[SnOHTIntoLoadPort2          ].Name="SnOHTIntoLoadPort2";
    Sen[SnCassetteClose             ].Name="SnCassetteClose";
    Sen[SnCassetteOpen2             ].Name="SnCassetteOpen2";
    Sen[SnCassetteClose2            ].Name="SnCassetteClose2";
    Sen[SnCassetteSlotMap           ].Name="SnCassetteSlotMap";
    Sen[SnCassetteFloating          ].Name="SnCassetteFloating";
    Sen[SnMultileEmpty_ScanTrayID   ].Name="SnMultileEmpty_ScanTrayID";
    Sen[SnE84_2_VALID               ].Name="SnE84_2_VALID";
    Sen[SnE84_2_CS0                 ].Name="SnE84_2_CS0";
    Sen[SnE84_2_CS1                 ].Name="SnE84_2_CS1";
    Sen[SnE84_2_AMAVBL              ].Name="SnE84_2_AMAVBL";
    Sen[SnE84_2_TRREQ               ].Name="SnE84_2_TRREQ";
    Sen[SnE84_2_BUSY                ].Name="SnE84_2_BUSY";
    Sen[SnE84_2_COMPT               ].Name="SnE84_2_COMPT";
    Sen[SnE84_2_CONT                ].Name="SnE84_2_CONT";
    Sen[SnE84_2_GO                  ].Name="SnE84_2_GO";
    Sen[SnE84_1_VALID               ].Name="SnE84_1_VALID";
    Sen[SnE84_1_CS0                 ].Name="SnE84_1_CS0";
    Sen[SnE84_1_CS1                 ].Name="SnE84_1_CS1";
    Sen[SnE84_1_AMAVBL              ].Name="SnE84_1_AMAVBL";
    Sen[SnE84_1_TRREQ               ].Name="SnE84_1_TRREQ";
    Sen[SnE84_1_BUSY                ].Name="SnE84_1_BUSY";
    Sen[SnE84_1_COMPT               ].Name="SnE84_1_COMPT";
    Sen[SnE84_1_CONT                ].Name="SnE84_1_CONT";
    Sen[SnE84_1_GO                  ].Name="SnE84_1_GO";

    Sen[SnLoaderTrayHasTray_AGV     ].Name="SnLoaderTrayHasTray_AGV";           //kevin 20220520 add AGV load
    Sen[SnEmptyTrayHasTray_AGV      ].Name="SnEmptyTrayHasTray_AGV";            //kevin 20220520 add AGV load
    Sen[SnColorTrayHasTray_AGV      ].Name="SnColorTrayHasTray_AGV";            //kevin 20220520 add AGV load
    Sen[SnMultileEmptyCatch_HasTray ].Name="SnMultileEmptyCatch_HasTray";
    Sen[SnCSTHoldDownOff            ].Name="SnCSTHoldDownOff";
    Sen[SnCSTHoldDown2Off           ].Name="SnCSTHoldDown2Off";
    Sen[SnMultileEmpty_ScanTray2DID ].Name="SnMultileEmpty_ScanTray2DID";
    Sen[SnDieDetect_Sh1             ].Name="SnDieDetect_Sh1";
    Sen[SnDieDetect_Sh2             ].Name="SnDieDetect_Sh2";
    Sen[SnBuffer1_Placement1        ].Name="SnBuffer1_Placement1";
    Sen[SnBuffer2_Placement1        ].Name="SnBuffer2_Placement1";
    Sen[SnBuffer3_Placement1        ].Name="SnBuffer3_Placement1";
    Sen[SnBuffer4_Placement1        ].Name="SnBuffer4_Placement1";
    Sen[SnBuffer5_Placement1        ].Name="SnBuffer5_Placement1";
    Sen[SnBuffer6_Placement1        ].Name="SnBuffer6_Placement1";
    Sen[SnBuffer7_Placement1        ].Name="SnBuffer7_Placement1";
    Sen[SnBuffer8_Placement1        ].Name="SnBuffer8_Placement1";
    Sen[SnBuffer9_Placement1        ].Name="SnBuffer9_Placement1";
    Sen[SnBuffer10_Placement1       ].Name="SnBuffer10_Placement1";
    Sen[SnStackedTrayYDetect_Loader ].Name="SnStackedTrayYDetect_Loader";
    Sen[SnStackedTrayYDetect_Elevator2].Name="SnStackedTrayYDetect_Elevator2";

    Sen[SnInAreaAlignmentSenX       ].Name="SnInAreaAlignmentSenX";             //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    Sen[SnInAreaAlignmentSenY       ].Name="SnInAreaAlignmentSenY";
    Sen[SnOutAreaAlignmentSenX      ].Name="SnOutAreaAlignmentSenX";
    Sen[SnOutAreaAlignmentSenY      ].Name="SnOutAreaAlignmentSenY";
    Sen[SnInAreaAlignmentSenZ       ].Name="SnInAreaAlignmentSenZ";             //KenHsieh 20211110 ： AutoAlignment add Auto Z
    Sen[SnOutAreaAlignmentSenZ      ].Name="SnOutAreaAlignmentSenZ";            //KenHsieh 20211110 ： AutoAlignment add Auto Z

    Sen[SnSht1Left                  ].Name="SnSht1Left";                        //kevin 20220512 add SHUTTLE Left 位置偏移
    Sen[SnSht1Right                 ].Name="SnSht1Right";                       //kevin 20220512 add SHUTTLE Right 位置偏移
    Sen[SnSht2Left                  ].Name="SnSht2Left";                        //kevin 20220512 add SHUTTLE Left 位置偏移
    Sen[SnSht2Right                 ].Name="SnSht2Right";                       //kevin 20220512 add SHUTTLE Right 位置偏移

    Sen[SnAseTrayBufferLeft         ].Name="SnAseTrayBufferLeft";               //kevin 20220709 ASEKH 左邊放空TRAY 軟體不須判斷
    Sen[SnAseTrayBufferRight        ].Name="SnAseTrayBufferRight";              //kevin 20220709 ASEKH 右邊放空TRAY  軟體不須判斷

    Sen[SnATC1Ready                 ].Name="SnATC1Ready";                       //Jimmychiu 20210902 add: ATC Winway IO ready
    Sen[SnATC2Ready                 ].Name="SnATC2Ready";
    Sen[SnATC3Ready                 ].Name="SnATC3Ready";
    Sen[SnATC4Ready                 ].Name="SnATC4Ready";

    Sen[SnPlate1TempOverDetect      ].Name="SnPlate1TempOverDetect";            //Ztex 2023.04.13 Add HT-1032 IO
    Sen[SnPlate2TempOverDetect      ].Name="SnPlate2TempOverDetect";
    Sen[SnShuttle1TempOverDetect    ].Name="SnShuttle1TempOverDetect";
    Sen[SnShuttle2TempOverDetect    ].Name="SnShuttle2TempOverDetect";
    Sen[SnHead1TempOverDetect       ].Name="SnHead1TempOverDetect";
    Sen[SnHead2TempOverDetect       ].Name="SnHead2TempOverDetect";
    Sen[SnHead5TempOverDetect       ].Name="SnHead5TempOverDetect";
    Sen[SnHead6TempOverDetect       ].Name="SnHead6TempOverDetect";
    Sen[SnHumidityAnomaly1Detect    ].Name="SnHumidityAnomaly1Detect";
    Sen[SnHumidityAnomaly2Detect    ].Name="SnHumidityAnomaly2Detect";
    Sen[SnHumidityAnomaly3Detect    ].Name="SnHumidityAnomaly3Detect";
    Sen[SnDryAirIsEnough            ].Name="SnDryAirIsEnough";
    Sen[SnIonBarInAirIsEnough       ].Name="SnIonBarInAirIsEnough";
    Sen[SnIonBarOutAirIsEnough      ].Name="SnIonBarOutAirIsEnough";
    Sen[SnSafeDoor1Hatchway         ].Name="SnSafeDoor1Hatchway";
    Sen[SnSafeDoor2Hatchway         ].Name="SnSafeDoor2Hatchway";
    Sen[SnSafeDoor3Hatchway         ].Name="SnSafeDoor3Hatchway";
    Sen[SnSafeDoor4Hatchway         ].Name="SnSafeDoor4Hatchway";
    Sen[SnSafeDoor5Hatchway         ].Name="SnSafeDoor5Hatchway";
    Sen[SnSafeDoor6Hatchway         ].Name="SnSafeDoor6Hatchway";
    Sen[SnSafeDoor7Hatchway         ].Name="SnSafeDoor7Hatchway";
    Sen[SnSafeDoor8Hatchway         ].Name="SnSafeDoor8Hatchway";
    Sen[SnSafeDoor9Hatchway         ].Name="SnSafeDoor9Hatchway";
    Sen[SnSafeDoor10Hatchway        ].Name="SnSafeDoor10Hatchway";
    Sen[SnSafeDoor11Hatchway        ].Name="SnSafeDoor11Hatchway";
    Sen[SnSafeDoor6PosFixPickPlace  ].Name="SnSafeDoor6PosFixPickPlace";
    Sen[SnTriTempSafeDoor6Lock      ].Name="SnTriTempSafeDoor6Lock";
    Sen[SnTrayArmZSafePos           ].Name="SnTrayArmZSafePos";
    Sen[SnEnhaustAirFanAlarmDetect  ].Name="SnEnhaustAirFanAlarmDetect";        //Ztex 2023.04.26 Add HT-1032 IO Exhaust Air
    Sen[SnDockingAreaOpenCheck      ].Name="SnDockingAreaOpenCheck";            //Ztex 2023.05.02 Add HT-1032 IO Docking Area Open Check
    Sen[SnDewPointDetectIndexArm1   ].Name="SnDewPointDetectIndexArm1";         //Hmy 20170603  add By 三溫機 露點SENSOR Arm1偵測
    Sen[SnDewPointDetectIndexArm2   ].Name="SnDewPointDetectIndexArm2";         //Hmy 20170603  add By 三溫機 露點SENSOR Arm2偵測

    Sen[SnMagazineSafeDoor          ].Name="SnMagazineSafeDoor";                //JerryYang 20221215 : add Magazine
    Sen[SnMagazineDetect            ].Name="SnMagazineDetect";
    Sen[SnMagazineDetectTop         ].Name="SnMagazineDetectTop";
    Sen[SnMagazineTrackDetect       ].Name="SnMagazineTrackDetect";
    Sen[SnMagazineTrackDetect2      ].Name="SnMagazineTrackDetect2";            //Sam 20221116 : Magazine TrayArm 自動補 Tray
    Sen[SnMagazineTrackSelectDetect ].Name="SnMagazineTrackSelectDetect";
    Sen[SnMagazineSafeDoor2         ].Name="SnMagazineSafeDoor2";
    Sen[SnMagazineSafeDoor3         ].Name="SnMagazineSafeDoor3";
    Sen[SnMagazineHasTrayInside     ].Name="SnMagazineHasTrayInside";

    Sen[SnLoaderSeparateHasTray     ].Name="SnLoaderSeparateHasTray";
    Sen[SnEmptySeparateHasTray      ].Name="SnEmptySeparateHasTray";
    Sen[SnColorSeparateHasTray      ].Name="SnColorSeparateHasTray";
    Sen[SnAuto1SeparateHasTray      ].Name="SnAuto1SeparateHasTray";
    Sen[SnAuto2SeparateHasTray      ].Name="SnAuto2SeparateHasTray";
    Sen[SnAuto3SeparateHasTray      ].Name="SnAuto3SeparateHasTray";

    Sen[SenLoaderCCWDete_2          ].Name="SenLoaderCCWDete_2";
    Sen[SenEmptyCCWDete_2           ].Name="SenEmptyCCWDete_2";
    Sen[SenColorCCWDete_2           ].Name="SenColorCCWDete_2";
    Sen[SenAuto1CCWDete_2           ].Name="SenAuto1CCWDete_2";
    Sen[SenAuto2CCWDete_2           ].Name="SenAuto2CCWDete_2";
    Sen[SenAuto3CCWDete_2           ].Name="SenAuto3CCWDete_2";

    Sen[SnOpenDoorChangeKit1        ].Name="SnOpenDoorChangeKit1";
    Sen[SnOpenDoorChangeKit2        ].Name="SnOpenDoorChangeKit2";
    Sen[SnOpenDoorChangeKit3        ].Name="SnOpenDoorChangeKit3";

    Sen[SnIndexCylinderDetectHead1  ].Name="SnIndexCylinderDetectHead1";
    Sen[SnIndexCylinderDetectHead2  ].Name="SnIndexCylinderDetectHead2";
    Sen[SnIndexCylinderDetectHead5  ].Name="SnIndexCylinderDetectHead5";
    Sen[SnIndexCylinderDetectHead6  ].Name="SnIndexCylinderDetectHead6";

    Sen[SnIonBar1                   ].Name="SnIonBar1";
    Sen[SnIonBar2                   ].Name="SnIonBar2";
    Sen[SnIonBar3                   ].Name="SnIonBar3";
    Sen[SnIonBar4                   ].Name="SnIonBar4";
    Sen[SnIonBar5                   ].Name="SnIonBar5";
    Sen[SnIonBar6                   ].Name="SnIonBar6";
    Sen[SnIonBar7                   ].Name="SnIonBar7";
    Sen[SnIonBar8                   ].Name="SnIonBar8";
    Sen[SnIonBar9                   ].Name="SnIonBar9";

    Sen[SnTrayArmHasTray            ].Name="SnTrayArmHasTray";
    Sen[SnMCUSensor1                ].Name="SnMCUSensor1";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader
    Sen[SnMCUSensor2                ].Name="SnMCUSensor2";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader
    Sen[SnMCUSensor3                ].Name="SnMCUSensor3";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader
    Sen[SnMCUSensor4                ].Name="SnMCUSensor4";                      //Jimmychiu 20230630 : add color sensor MU-N in Loader

    Sen[SnAuto4_Tray_Car            ].Name="SnAuto4_Tray_Car";                  //Steven 20230907 : For HT-9011UC
    Sen[SnAuto4PreDete              ].Name="SnAuto4PreDete";
    Sen[SnAuto4TrackDetect          ].Name="SnAuto4TrackDetect";
    Sen[SnAuto4FixCyPush            ].Name="SnAuto4FixCyPush";
    Sen[SnAuto4EdgePush             ].Name="SnAuto4EdgePush";
    Sen[SnAuto4UpSafedetect         ].Name="SnAuto4UpSafedetect";
    Sen[SnAuto4SeparateHasTray      ].Name="SnAuto4SeparateHasTray";
    Sen[SnAuto4TrayHasTray          ].Name="SnAuto4TrayHasTray";
    Sen[SnAuto4CWPreDetect          ].Name="SnAuto4CWPreDetect";
    Sen[SenAuto4CCWDete_2           ].Name="SenAuto4CCWDete_2";
    Sen[SnAuto4Z_Select1            ].Name="SnAuto4Z_Select1";
    Sen[SnAuto4Z_Select2            ].Name="SnAuto4Z_Select2";
    Sen[SnAuto4Z_Select3            ].Name="SnAuto4Z_Select3";
    Sen[SnAuto4Z_Select4            ].Name="SnAuto4Z_Select4";
    Sen[SnAuto4IsPreAlarm           ].Name="SnAuto4IsPreAlarm";
    Sen[SnAuto5_Tray_Car            ].Name="SnAuto5_Tray_Car";
    Sen[SnAuto5PreDete              ].Name="SnAuto5PreDete";
    Sen[SnAuto5TrackDetect          ].Name="SnAuto5TrackDetect";
    Sen[SnAuto5FixCyPush            ].Name="SnAuto5FixCyPush";
    Sen[SnAuto5EdgePush             ].Name="SnAuto5EdgePush";
    Sen[SnAuto5UpSafedetect         ].Name="SnAuto5UpSafedetect";
    Sen[SnAuto5SeparateHasTray      ].Name="SnAuto5SeparateHasTray";
    Sen[SnAuto5TrayHasTray          ].Name="SnAuto5TrayHasTray";
    Sen[SnAuto5CWPreDetect          ].Name="SnAuto5CWPreDetect";
    Sen[SenAuto5CCWDete_2           ].Name="SenAuto5CCWDete_2";
    Sen[SnAuto5Z_Select1            ].Name="SnAuto5Z_Select1";
    Sen[SnAuto5Z_Select2            ].Name="SnAuto5Z_Select2";
    Sen[SnAuto5Z_Select3            ].Name="SnAuto5Z_Select3";
    Sen[SnAuto5Z_Select4            ].Name="SnAuto5Z_Select4";
    Sen[SnAuto5IsPreAlarm           ].Name="SnAuto5IsPreAlarm";
    Sen[SnAuto6_Tray_Car            ].Name="SnAuto6_Tray_Car";
    Sen[SnAuto6PreDete              ].Name="SnAuto6PreDete";
    Sen[SnAuto6TrackDetect          ].Name="SnAuto6TrackDetect";
    Sen[SnAuto6FixCyPush            ].Name="SnAuto6FixCyPush";
    Sen[SnAuto6EdgePush             ].Name="SnAuto6EdgePush";
    Sen[SnAuto6UpSafedetect         ].Name="SnAuto6UpSafedetect";
    Sen[SnAuto6SeparateHasTray      ].Name="SnAuto6SeparateHasTray";
    Sen[SnAuto6TrayHasTray          ].Name="SnAuto6TrayHasTray";
    Sen[SnAuto6CWPreDetect          ].Name="SnAuto6CWPreDetect";
    Sen[SenAuto6CCWDete_2           ].Name="SenAuto6CCWDete_2";
    Sen[SnAuto6Z_Select1            ].Name="SnAuto6Z_Select1";
    Sen[SnAuto6Z_Select2            ].Name="SnAuto6Z_Select2";
    Sen[SnAuto6Z_Select3            ].Name="SnAuto6Z_Select3";
    Sen[SnAuto6Z_Select4            ].Name="SnAuto6Z_Select4";
    Sen[SnAuto6IsPreAlarm           ].Name="SnAuto6IsPreAlarm";

    Sen[SnAuto4IsFull               ].Name="SnAuto4IsFull";
    Sen[SnAuto5IsFull               ].Name="SnAuto5IsFull";
    Sen[SnAuto6IsFull               ].Name="SnAuto6IsFull";

    Sen[SnAuto4TrayDetect           ].Name="SnAuto4TrayDetect";
    Sen[SnAuto5TrayDetect           ].Name="SnAuto5TrayDetect";
    Sen[SnAuto6TrayDetect           ].Name="SnAuto6TrayDetect";

    Sen[SnTrayArmTrayDetect1        ].Name="SnTrayArmTrayDetect1";
    Sen[SnTrayArmTrayDetect2        ].Name="SnTrayArmTrayDetect2";
    Sen[SnTrayArmTrayDetect3        ].Name="SnTrayArmTrayDetect3";
    Sen[SnTrayArmTrayDetect4        ].Name="SnTrayArmTrayDetect4";
    Sen[SnSafeMode                  ].Name="SnSafeMode";                        //jou 20231016 : CE PLC safe mode
    Sen[SnWaterLeakageChiller       ].Name="SnWaterLeakageChiller";             //jou 20231019 : Water Leakage Chiller
    Sen[SnSmokeDetect01             ].Name="SnSmokeDetect01";                   //Sam 20240112 : 新增煙霧偵測
    Sen[SnIonBar10                  ].Name="SnIonBar10";
    Sen[SnIonBar11                  ].Name="SnIonBar11";

    Sen[SnTesterAlarm               ].Name="SnTesterAlarm";
    Sen[SnDoubleLoadDetection       ].Name="SnDoubleLoadDetection";             //Steven 20240426 : 偵測loader疊盤

    Sen[SnLoader_Detect             ].Name="SnLoader_Detect";                   //Ifor 20211005 add Tray 載盤上升下降前判斷是否有異常
    Sen[SnEmpty_Detect              ].Name="SnEmpty_Detect";
    Sen[SnColor_Detect              ].Name="SnColor_Detect";
    Sen[SnAuto1_Detect              ].Name="SnAuto1_Detect";
    Sen[SnAuto2_Detect              ].Name="SnAuto2_Detect";
    Sen[SnAuto3_Detect              ].Name="SnAuto3_Detect";
    Sen[SnAuto4_Detect              ].Name="SnAuto4_Detect";
    Sen[SnAuto5_Detect              ].Name="SnAuto5_Detect";
    Sen[SnAuto6_Detect              ].Name="SnAuto6_Detect";
    Sen[SnIonFanCar                 ].Name="SnIonFanCar";                       //Ifor 20220310 add: Bin Car Ion Fan Check
    Sen[SnRKSafeLock                ].Name="SnRKSafeLock";                      //KenHsieh 20211228 : 區分實體IO與通訊面板
    Sen[SnIonFanCarPower            ].Name="SnIonFanCarPower";                  //Ifor 20220816 add: Bin Car Ion Fan Power Check
    Sen[SnEPFlowmeter               ].Name="SnEPFlowmeter";                     //Ifor 20240326 add: EP流量計監控
    Sen[SnChamberDryAir             ].Name="SnChamberDryAir";                   //Ifor 20240919 add: Chamber Dry Air

    Sen[SnLoad2IsFull               ].Name="SnLoad2IsFull";                     //Steven 20240822 : For HT-9046AU
    Sen[SnLoad2IsPreAlarm           ].Name="SnLoad2IsPreAlarm";
    Sen[SnLoad2TrayHasTray_AGV      ].Name="SnLoad2TrayHasTray_AGV";
    Sen[SnLoad2TrayHasTray_ART      ].Name="SnLoad2TrayHasTray_ART";
    Sen[SnLoad2TrayHasTray          ].Name="SnLoad2TrayHasTray";
    Sen[SnLoad2CarHasTray           ].Name="SnLoad2CarHasTray";
    Sen[SnCheckTray2Direction       ].Name="SnCheckTray2Direction";
    Sen[SenLoad2CCWDete             ].Name="SenLoad2CCWDete";
    Sen[SnLoad2UpSafedetect         ].Name="SnLoad2UpSafedetect";
    Sen[SenLoad2CCWDete_2           ].Name="SenLoad2CCWDete_2";
    Sen[SnLoad2TrackDetect          ].Name="SnLoad2TrackDetect";
    Sen[SnLoad2FixCyPush            ].Name="SnLoad2FixCyPush";
    Sen[SnLoad2EdgePush             ].Name="SnLoad2EdgePush";
    Sen[SnLoad2SeparateHasTray      ].Name="SnLoad2SeparateHasTray";
    Sen[SnLoad2SureTray             ].Name="SnLoad2SureTray";
    Sen[SnLoad2PreDete              ].Name="SnLoad2PreDete";
    Sen[SnCheckLoad2Direction       ].Name="SnCheckLoad2Direction";
    Sen[SnLoad2CasstteDetect        ].Name="SnLoad2CasstteDetect";
    Sen[SnDoubleLoad2Detection      ].Name="SnDoubleLoad2Detection";
    Sen[SnLoad2UpPress              ].Name="SnLoad2UpPress";
    Sen[SnAuto1HasCoverTray         ].Name="SnAuto1HasCoverTray";               //JerryYang 20241021 : Unloader增加第二組Sensor檢查是否有cover tray
    Sen[SnAuto2HasCoverTray         ].Name="SnAuto2HasCoverTray";
    Sen[SnAuto3HasCoverTray         ].Name="SnAuto3HasCoverTray";
    Sen[SnAuto4HasCoverTray         ].Name="SnAuto4HasCoverTray";
    Sen[SnAuto5HasCoverTray         ].Name="SnAuto5HasCoverTray";
    Sen[SnAuto6HasCoverTray         ].Name="SnAuto6HasCoverTray";

    Sen[SnElectricControlBox        ].Name="SnElectricControlBox";              //ChungHung 20230718 add for Safe plc
    Sen[SnAllSafeDoor               ].Name="SnAllSafeDoor";                     //ChungHung 20230718 add for Safe plc
    Sen[SnAllEMG                    ].Name="SnAllEMG";                          //KenHsieh 20250212 : 新增PLC 斷線可瞬間判斷EMG及安全門
    Sen[SnLoadIonGun                ].Name="SnLoadIonGun";                      //Ifor 20230427 add:Loader Ionizer Gun
    Sen[SnLoadLightGat              ].Name="SnLoadLightGat";                    //Ifor 20230427 add:LD/ULD light gat

    Sen[SnLightZORG                 ].Name="SnLightZORG";
    Sen[SnLightZINP                 ].Name="SnLightZINP";
    Sen[SnLightZREADY               ].Name="SnLightZREADY";
    Sen[SnLightZSERVO               ].Name="SnLightZSERVO";

    Sen[SnFixedSeatTLOn             ].Name="SnFixedSeatTLOn";
    Sen[SnFixedSeatTLOff            ].Name="SnFixedSeatTLOff";
    Sen[SnFixedSeatTROn             ].Name="SnFixedSeatTROn";
    Sen[SnFixedSeatTROff            ].Name="SnFixedSeatTROff";
    Sen[SnFixedSeatBLOn             ].Name="SnFixedSeatBLOn";
    Sen[SnFixedSeatBLOff            ].Name="SnFixedSeatBLOff";
    Sen[SnFixedSeatBROn             ].Name="SnFixedSeatBROn";
    Sen[SnFixedSeatBROff            ].Name="SnFixedSeatBROff";

    Sen[SnTopBtmRotateLockOn1       ].Name="SnTopBtmRotateLockOn1";
    Sen[SnTopBtmRotateLockOn2       ].Name="SnTopBtmRotateLockOn2";

    Sen[SnTopBtmAirMaxAlarm         ].Name="SnTopBtmAirMaxAlarm";
    Sen[SnTopBtmAirMinAlarm         ].Name="SnTopBtmAirMinAlarm";
    Sen[SnTopBtmAOIR180             ].Name="SnTopBtmAOIR180";
    Sen[SnChamberDryAir             ].Name="SnChamberDryAir";                   //Ifor 20240919 add: Chamber Dry Air

    Sen[SnLoadCarRFIDSW             ].Name="SnLoadCarRFIDSW";                   //RogerYang 20250828 : add for Loader Rotate Arm

    Sen[SnLoaderCarrier1             ].Name="SnLoaderCarrier1";                 //Ifor 20251216 add:Boat Carrier
    Sen[SnAuto1Carrier1              ].Name="SnAuto1Carrier1";
    Sen[SnAuto2Carrier1              ].Name="SnAuto2Carrier1";
    Sen[SnLoaderCarrier2             ].Name="SnLoaderCarrier2";
    Sen[SnAuto1Carrier2              ].Name="SnAuto1Carrier2";
    Sen[SnAuto2Carrier2              ].Name="SnAuto2Carrier2";
    Sen[SnLoaderBoatActDetect        ].Name="SnLoaderBoatActDetect";
    Sen[SnAuto1BoatActDetect         ].Name="SnAuto1BoatActDetect";
    Sen[SnAuto2BoatActDetect         ].Name="SnAuto2BoatActDetect";
}
//------------------------------------------------------------------------------
void InitialSensor()
{
    TTable *T;
    int iSn;
    bool flag;
    AnsiString S, S1, sDBDir;
    bool bHasSensor;                                                            //Stevem 20210625 : 修正新的CSV IO表讀檔方式
    AnsiString str;

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        InitialSensorName();
        for(int i=0; i<MAX_SENSOR_ITEM; i++)
        {
            bHasSensor=false;
            Sen[i].Enable=false;
            if(Sen[i].Name=="")
                continue;

            HSys.mapIOTableIter=HSys.mapIOTable.find(Sen[i].Name);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasSensor=true;
                iSn=atoi(HSys.mapIOTable[Sen[i].Name].c_str());
            }
            else
            {
                str.sprintf("Can not find sensor %s", Sen[i].Name);
//                continue;
            }

            if(bHasSensor)
            {
                Sen[i].ISABase      =HSys.IOTable[iSn]->iISABase;
                if(HSys.IOTable[iSn]->iEnable==1)
                    Sen[i].Enable   =true;
                Sen[i].Using        =(HSys.IOTable[iSn]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iSn]->iPort);
                Sen[i].Ring         =HSys.IOTable[iSn]->iLane;
                Sen[i].IP           =HSys.IOTable[iSn]->iIP;
                Sen[i].Port         =HSys.IOTable[iSn]->iPort;
                Sen[i].Bit          =HSys.IOTable[iSn]->iBit;
                Sen[i].Type         =HSys.IOTable[iSn]->iInType;
            }
            else
            {
                Sen[i].Enable=false;
            }
            #ifdef SOFT_SIMULTE
                #ifdef SOFT_SIMULTE_EtherCAT
                    if(Sen[i].ISABase==ePCI1203)                                //Sam 20230724 : 測試 SensorEtherCAT 用
                    {

                    }
                    else
                    {
                        Sen[i].Enable=false;
                    }
                #else
                    Sen[i].Enable=false;
                #endif
            #endif

            S=Sen[i].Name;
            if(Enable_PLCSafety_IO &&                                           //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
               Sen[i].ISABase==ePLCbase)                                        // 20230821 Joseph , Add Heater door //
            {
                bPLCIO[Sen[i].Port][Sen[i].Bit]=true;
                Sen[i].Type=1;
                Sen[i].Enable=1;
            }

            #ifndef SOFT_SIMULTE
            if(Sen[i].Enable)
            #endif
            {
                MyLaneIO.SetUseIP(Sen[i].Ring, Sen[i].IP, Sen[i].ISABase, Sen[i].Name, false);    //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }
        }
    }
    else
    {
        if(MachineTypeChoice==Type_HT9046)                                      //9046
        {
            sDBDir=CurrentDir+"\\system\\sensor_9046.db";                       //JerryYang 20150903 檢查Database檔是否存在
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor_9046.db does not exist");
                return;
            }
        }
        else if(MachineTypeChoice==Type_HT9046_LS ||
                MachineTypeChoice==Type_HT1032)
        {
            sDBDir=CurrentDir+"\\system\\Sensor_9046LS.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("Sensor_9046LS.db does not exist");
                return;
            }
        }
        else if(USE_46_SENSOR_DB)                                               //Steven 20131008 : HT9045使用46配電
        {
            sDBDir=CurrentDir+"\\system\\sensor_9046.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor_9046.db does not exist");
                return;
            }
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            sDBDir=CurrentDir+"\\system\\sensor.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor.db does not exist");
                return;
            }
        }
        else
        {
            sDBDir=CurrentDir+"\\system\\sensor.db";
            if(FileExists(sDBDir))
            {
                DataModule1->SensorTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("sensor.db does not exist");
                return;
            }
        }
        T=DataModule1->SensorTable;
        T->Open();
        InitialSensorName();
        for(int i=0; i<MAX_SENSOR_ITEM; i++)
        {
            if(Sen[i].Name=="")
                continue;
            T->First();
            flag=false;
            while(!T->Eof)
            {
                S=T->FieldByName("SensorName")->AsString;
                if(S==Sen[i].Name)
                {
                    flag=true;
                    break;
                }
                T->Next();
            }

            if(flag==false)
            {
                str.sprintf("Can not find sensor name:%s", Sen[i].Name);
            }
            else
            {
                Sen[i].ISABase      =atoi(T->FieldByName("ISABase")->AsString.c_str());
                Sen[i].Enable       =atoi(T->FieldByName("Enable")->AsString.c_str());
                if(Sen[i].ISABase==eISABase ||                                  //Alick 20160809 modify for PIC-1735U
                   Sen[i].ISABase==ePCI1735U ||
                   Sen[i].ISABase==ePLCbase)
                {
                    Sen[i].Using    =T->FieldByName("Port")->AsString;
                    if(Sen[i].Using=="")
                        Sen[i].Enable=false;
                    Sen[i].Ring     =0;
                    Sen[i].IP       =0;
                    Sen[i].Port     =HexStrToInt(T->FieldByName("Port")->AsString.c_str());
                    Sen[i].Bit      =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                else
                {
                    Sen[i].Using    =T->FieldByName("Lane")->AsString;
                    if(Sen[i].Using!="")
                        Sen[i].Ring =atoi(Sen[i].Using.c_str());
                    else
                        Sen[i].Enable=false;
                    Sen[i].IP       =atoi(T->FieldByName("IP")->AsString.c_str());
                    Sen[i].Port     =atoi(T->FieldByName("Port")->AsString.c_str());
                    Sen[i].Bit      =atoi(T->FieldByName("Bit")->AsString.c_str());
                }
                Sen[i].Type         =atoi(T->FieldByName("InType")->AsString.c_str());

                if(Enable_PLCSafety_IO &&                                       //Austin 20190531.01 增加PLC_IO模組及安全相關IO.
                   Sen[i].ISABase==ePLCbase)                                    // 20230821 Joseph , Add Heater door //
                {
                    bPLCIO[Sen[i].Port][Sen[i].Bit]=true;
                    Sen[i].Type=1;
                    Sen[i].Enable=1;
                }

                #ifdef SOFT_SIMULTE
                    Sen[i].Enable=false;
                #endif

                #ifndef SOFT_SIMULTE
                if(Sen[i].Enable)
                #endif
                {
                    MyLaneIO.SetUseIP(Sen[i].Ring, Sen[i].IP, Sen[i].ISABase, Sen[i].Name, false);    //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                }
            }
        }
        T->Close();
    }

    for(int i=0; i<10; i++)
    {
        Sen[SenBit0+i ].Enable=true;
        Sen[SenBit10+i].Enable=true;
    }

    for(int i=0; i<10; i++)
    {
        CopySensor(&Sen[SenBit0+i ], &SenBackUp[BackSenBit0+i]);
        CopySensor(&Sen[SenBit10+i], &SenBackUp[BackSenBit10+i]);
    }

    if(CosFunction.bTTLCanUse8Site)                                             //Alick 20161011 (Steven) : TTL支援8Site
    {
        for(int i=0; i<20; i++)
        {
            Sen[SenBit20+i].Enable=true;
            CopySensor(&Sen[SenBit20+i], &SenBackUp[BackSenBit20+i]);
        }
    }

    if(iControlPanelMode==1)                                                    //Steven 20250317 : 使用通訊式面板的要disable
    {
        Sen[SnFKPowerOff                ].Enable=false;
        Sen[SnFKPowerOn                 ].Enable=false;
        Sen[SnFKReset                   ].Enable=false;
        Sen[SnFKPause                   ].Enable=false;
        Sen[SnFKHome                    ].Enable=false;
        Sen[SnFKStart                   ].Enable=false;
        Sen[SnFKOneCycle                ].Enable=false;
        Sen[SnFKRetry                   ].Enable=false;
        Sen[SnFKSkip                    ].Enable=false;
        Sen[SnFKCleanOut                ].Enable=false;
        Sen[SnFKTrayFeed                ].Enable=false;
        Sen[SnFKTrayEnd                 ].Enable=false;
        Sen[SnFKAlarmReset              ].Enable=false;
        Sen[SnFKCoverOpen               ].Enable=false;
        Sen[SnRKPowerOff                ].Enable=false;
        Sen[SnRKPowerOn                 ].Enable=false;
        Sen[SnRKReset                   ].Enable=false;
        Sen[SnRKPause                   ].Enable=false;
        Sen[SnRKHome                    ].Enable=false;
        Sen[SnRKStart                   ].Enable=false;
        Sen[SnRKOneCycle                ].Enable=false;
        Sen[SnRKRetry                   ].Enable=false;
        Sen[SnRKSkip                    ].Enable=false;
        Sen[SnRKCleanOut                ].Enable=false;
        Sen[SnRKTrayFeed                ].Enable=false;
        Sen[SnRKTrayEnd                 ].Enable=false;
        Sen[SnRKAlarmReset              ].Enable=false;
//        Sen[SnRKCoverOpen               ].Enable=false;
        Sen[SnRKManualStep              ].Enable=false;
        Sen[SnRKManualTStart            ].Enable=false;
        Sen[SnRearPadActive             ].Enable=false;
    }

    SetIOTableByNUEC1();                                                        //Sam 20230725 : EtherCAT NUEC1 IO 取代線路 IO

    //******************************************************************************
    //  注意!! Handler 安全門相關, 不可任意修改!!
    //******************************************************************************
    InitialSafeDoor();
}
//******************************************************************************
//  注意!! Handler 安全門相關, 不可任意修改!!
//******************************************************************************
void InitialSafeDoor()
{
    Sen[SnSafeDoor1].Enable=true;
    Sen[SnSafeDoor2].Enable=true;
    Sen[SnSafeDoor3].Enable=true;
    Sen[SnSafeDoor6].Enable=true;
    Sen[SnSafeDoor7].Enable=true;
    Sen[SnSafeDoor8].Enable=true;
    Sen[SnSafeDoor9].Enable=true;

    if(MachineTypeChoice!=Type_HT7080)                                          //Nickliu 20230321 add 70XX safe door type
    {
        Sen[SnSafeDoor1].Type=1;                                                //JerryYang 20160720 補上安全門不能Disable的漏洞
        Sen[SnSafeDoor2].Type=1;
        Sen[SnSafeDoor3].Type=1;
        Sen[SnSafeDoor6].Type=1;
        Sen[SnSafeDoor7].Type=1;
        Sen[SnSafeDoor8].Type=1;
        Sen[SnSafeDoor9].Type=1;
    }
    else
    {
       Sen[SnSafeDoor1].Type=0;
       Sen[SnSafeDoor2].Type=0;
       Sen[SnSafeDoor3].Type=0;
       Sen[SnSafeDoor6].Type=0;
       Sen[SnSafeDoor7].Type=0;
       Sen[SnSafeDoor8].Type=0;
       Sen[SnSafeDoor9].Type=0;
    }

    if(SAFE_DOOR_AMOUNT==0)                                                     //7扇門
    {
        Sen[SnSafeDoor4 ].Enable=false;
        Sen[SnSafeDoor5 ].Enable=false;
        Sen[SnSafeDoor10].Enable=false;
    }
    else if(SAFE_DOOR_AMOUNT==1)                                                //8扇門
    {
        Sen[SnSafeDoor4 ].Enable=false;
        Sen[SnSafeDoor5 ].Enable=false;
        Sen[SnSafeDoor10].Enable=true;
        Sen[SnSafeDoor10].Type=1;
    }
    else if(SAFE_DOOR_AMOUNT==2)                                                //9扇門
    {
        Sen[SnSafeDoor4 ].Enable=true;
        Sen[SnSafeDoor5 ].Enable=true;
        Sen[SnSafeDoor10].Enable=false;

        Sen[SnSafeDoor4].Type=1;
        Sen[SnSafeDoor5].Type=1;
    }
    else if(SAFE_DOOR_AMOUNT==3)                                                //10扇門
    {
        Sen[SnSafeDoor4 ].Enable=true;
        Sen[SnSafeDoor5 ].Enable=true;
        Sen[SnSafeDoor10].Enable=true;

        Sen[SnSafeDoor4 ].Type=1;
        Sen[SnSafeDoor5 ].Type=1;
        Sen[SnSafeDoor10].Type=1;
    }
    else if(SAFE_DOOR_AMOUNT==4)                                                //Ztex 2023.04.13 Add HT-1032 IO
    {
        Sen[SnSafeDoor4].Enable=true;
        Sen[SnSafeDoor5].Enable=false;
        Sen[SnSafeDoor9].Enable=false;
    }

    if(USE_MR_SYSTEM)                                                           //wei 20180702 MR
    {
        if(USE_MR_SYSTEM==1)
        {
            #ifdef SOFT_SIMULTE
            Sen[SnSafeDoor11].Enable=false;
            #else
            Sen[SnSafeDoor11].Enable=true;
            #endif
        }
        else
        {
            Sen[SnSafeDoor16].Enable=true;
            Sen[SnSafeDoor17].Enable=true;
            Sen[SnSafeDoor18].Enable=true;
            Sen[SnSafeDoor19].Enable=true;
        }
        Sen[SnSafeDoor9].Enable=false;

        Sen[SnSafeDoor12].Enable=true;
        Sen[SnSafeDoor13].Enable=true;
        Sen[SnSafeDoor14].Enable=true;
        Sen[SnSafeDoor15].Enable=true;

        Sen[SnSafeDoor11].Type=1;
        Sen[SnSafeDoor12].Type=1;
        Sen[SnSafeDoor13].Type=1;
        Sen[SnSafeDoor14].Type=1;
        Sen[SnSafeDoor15].Type=1;
    }
    else if(USE_OHT_SYSTEM==1)                                                  //KaiChen 20200716 ：OHT
    {
        Sen[SnSafeDoor9].Enable=false;
        Sen[SnSafeDoor10].Enable=true;

        Sen[SnSafeDoor12].Enable=true;
        Sen[SnSafeDoor13].Enable=true;
        Sen[SnSafeDoor14].Enable=true;
        Sen[SnSafeDoor15].Enable=true;
        Sen[SnSafeDoor16].Enable=true;
        Sen[SnSafeDoor17].Enable=true;

        Sen[SnSafeDoor19].Enable=true;
        Sen[SnSafeDoor20].Enable=true;
        Sen[SnSafeDoor21].Enable=true;

        #ifdef SOFT_SIMULTE
        Sen[SnSafeDoor11].Enable=false;
        Sen[SnSafeDoor18].Enable=false;
        #else
        Sen[SnSafeDoor11].Enable=true;
        Sen[SnSafeDoor18].Enable=true;
        #endif

        Sen[SnSafeDoor10].Type=1;
        Sen[SnSafeDoor11].Type=1;
        Sen[SnSafeDoor12].Type=1;
        Sen[SnSafeDoor13].Type=1;
        Sen[SnSafeDoor14].Type=1;
        Sen[SnSafeDoor15].Type=1;
        Sen[SnSafeDoor16].Type=1;
        Sen[SnSafeDoor17].Type=1;
        Sen[SnSafeDoor18].Type=1;
        Sen[SnSafeDoor19].Type=1;
        Sen[SnSafeDoor20].Type=1;
        Sen[SnSafeDoor21].Type=1;

        Sen[SnOHTIntoLoadPort].Enable=true;
        Sen[SnOHTIntoLoadPort2].Enable=true;

        Sen[SnOHTIntoLoadPort].Type=1;
        Sen[SnOHTIntoLoadPort2].Type=1;
    }

    if(AUTO3_IS_MAGAZINE==1)                                                    //JerryYang 20221215 : add Magazine安全門
    {
        Sen[SnMagazineSafeDoor].Enable=true;
        Sen[SnMagazineSafeDoor].Type=1;
    }
    #ifdef SOFT_SIMULTE
    for(int i=0; i<MAX_SAFE_DOOR_CNT; i++)
        Sen[iSafeDoor[i]].Enable=false;
    #endif

    if(MachineTypeChoice==Type_HT1032)                                          //Ztex 2023.04.13 Add HT-1032 IO
    {
        for(int i=0; i<MAX_HATCH_DOOR_CNT; i++)
        {
            #ifdef SOFT_SIMULTE
            Sen[iSafeDoorHatchway[i]].Enable=false;
            #else
            Sen[iSafeDoorHatchway[i]].Enable=true;
            #endif
        }
        Sen[iSafeDoorHatchway[4]].Enable=false;
        Sen[iSafeDoorHatchway[9]].Enable=false;
        Sen[iSafeDoorHatchway[10]].Enable=false;
    }
    else
    {
        for(int i=0; i<MAX_HATCH_DOOR_CNT; i++)
        {
            Sen[iSafeDoorHatchway[i]].Enable=false;
        }
    }

    if(MachineTypeChoice!=Type_HT7080)                                          //Nickliu 20230313 add HT70XX No Heater Door
        InitialHeaterDoor();

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20230608 add:KYEC 要求[P35] Tray Arm home safe pos強制反灰由DB選擇開/關
    {
        IniConfig.bP35TrayArm=Sen[SnTrayArmSafePos].Enable;
    }
//#endif
}
//******************************************************************************
//
//  注意!! Handler 安全門相關, 不可任意修改!!
//
//******************************************************************************
void InitialHeaterDoor()
{
    #ifndef SOFT_SIMULTE                                                        //JerryYang 20230204 : 軟體模擬不用卡安全門
    if(ATC_SYSTEM==eATCSiliconType &&
       (TestIF_File.iTestMode==DualSite ||
        TestIF_File.iTestMode==SingleSite) &&
       Temperature.bATCActiveCooling==true)
    {
        Sen[SnHeaterDoor].Enable=false;
    }
    else
    {
        Sen[SnHeaterDoor].Enable=true;
    }

    Sen[SnHeaterDoor2].Enable=true;
    #endif
}
//------------------------------------------------------------------------------
//void InitialSensorLane()
//{
//    TTable *T;
//    int i;
//    bool flag;
//    AnsiString S, S1;
//    AnsiString sTemp;
//    char str[256];
//    DataModule1->SensorTable->TableName=CurrentDir+"\\system\\sensorLane.db";
//    T=DataModule1->SensorTable;
//    T->Open();
//    InitialSensorName();
//    for(i=0; i<MAX_SENSOR_ITEM; i++)
//    {
//        if(Sen[i].Name=="")
//            continue;
//        T->First();
//        flag=false;
//        while(!T->Eof)
//        {
//            S=T->FieldByName("SensorName")->AsString;
//            if(S==Sen[i].Name)
//            {
//                flag=true;
//                break;
//            }
//            T->Next();
//        }
//        if(flag==false)
//        {
//            sprintf(str,"Can not find sensor name:%s",Sen[i].Name.c_str());
//        }
//        else
//        {
//
//            Sen[i].Using         =T->FieldByName("Lane")->AsString;
//            if(Sen[i].Using!="")
//                Sen[i].Ring     =atoi(Sen[i].Using.c_str());
//            else
//                Sen[i].Enable=false;
//            Sen[i].IP           =atoi(T->FieldByName("IP")->AsString.c_str());
//            Sen[i].Port         =atoi(T->FieldByName("Port")->AsString.c_str());
//            Sen[i].Bit          =atoi(T->FieldByName("Bit")->AsString.c_str());
//            Sen[i].Type         =atoi(T->FieldByName("InType")->AsString.c_str());
//
//            #ifdef SOFT_SIMULTE
//                Sen[i].Enable=false;
//            #else
//                Sen[i].Enable=atoi(        T->FieldByName("Enable")->AsString.c_str());
//            #endif
//        }
//    }
//    T->Close();
//
//    //Alick 20161011 (Steven) : TTL支援8Site
//    //==>
//    for(i=0; i<10; i++)
//    {
//        Sen[SenBit0+i].Enable=true;
//        Sen[SenBit10+i].Enable=true;
//    }
//
//    for(i=0; i<10; i++)
//    {
//        CopySensor(&Sen[SenBit0 +i], &SenBackUp[BackSenBit0 +i]);
//        CopySensor(&Sen[SenBit10+i], &SenBackUp[BackSenBit10+i]);
//    }
//
//    if(CosFunction.bTTLCanUse8Site)
//    {
//        for(i=0; i<20; i++)
//        {
//            Sen[SenBit20+i].Enable=true;
//            CopySensor(&Sen[SenBit20+i], &SenBackUp[BackSenBit20+i]);
//        }
//    }
//    //<==
//    //Alick 20161011 (Steven) : TTL支援8Site
//}
//==============================================================================
AnsiString CheckMotorValue(TTable *PT, AnsiString asString, bool bIsNum)        //jou 20180814 (Steven) : 增加Motor database 異常警示
{
    AnsiString asAlias;
    char cCheckASCII[128]="";

    asAlias=PT->FieldByName("Alias")->AsString;

    if(atoi(PT->FieldByName("Enable")->AsString.c_str())==1)
    {
        if((asAlias=="MTestY1" || asAlias=="MTestZ1" ||
            asAlias=="MTestZ2" || asAlias=="MTestY2" ||
            asAlias=="MOutShuttle1" || asAlias=="MOutShuttle2") &&
           (asString=="BoardID" || asString=="Port" ||
            asString=="Acc"     || asString=="Dec"  ||
            asString=="ADc" || asString=="Lane" || asString=="IP"))
        {

        }
        else if(asString=="LimitLogic" || asString=="In1Logic")
        {
        }
        else if(PT->FieldByName(asString)->AsString.Length()<1)
        {
            asMotorDatabaseErr.sprintf("Motor database %s %s value = NULL error!!\nNeed open %s and fix data", asAlias, asString, cCheckASCII, PT->TableName);
            MessageDlg(asMotorDatabaseErr, mtConfirmation, TMsgDlgButtons()<<mbOK, 0);
            RecordProcess(asMotorDatabaseErr);
        }
        else if(bIsNum==true)
        {
            strncpy(cCheckASCII, PT->FieldByName(asString)->AsString.c_str(), sizeof(cCheckASCII));
            for(unsigned int z=0; z<strlen(cCheckASCII); z++)
            {
                if(cCheckASCII[z]==45 || cCheckASCII[z]==46)                    // - .
                {
                    continue;
                }
                if(cCheckASCII[z]<48 || cCheckASCII[z]>57)                      // 0~9
                {
                    asMotorDatabaseErr.printf("Motor database %s %s=%s type error\nNeed open %s and fix data", asAlias, asString, cCheckASCII, PT->TableName);
                    MessageDlg(asMotorDatabaseErr, mtConfirmation, TMsgDlgButtons()<<mbOK, 0);
                    RecordProcess(asMotorDatabaseErr);
                }
            }
        }
    }
    return PT->FieldByName(asString)->AsString;
}
//==============================================================================
void InitialMotorName()                                                         //Steven 20250520 : 變更Mot初始化方式
{
    MOT[MInArmX       ].SetAlias(  0, "MInArmX");
    MOT[MInArmY       ].SetAlias(  1, "MInArmY");
    MOT[MInArmPitch   ].SetAlias(  2, "MInArmPitch");
    MOT[MInArmZA      ].SetAlias(  3, "MInArmZA");
    MOT[MInArmZB      ].SetAlias(  4, "MInArmZB");
    MOT[MInArmZC      ].SetAlias(  5, "MInArmZC");
    MOT[MInArmZD      ].SetAlias(  6, "MInArmZD");
    MOT[MInArmZE      ].SetAlias(  7, "MInArmZE");
    MOT[MInArmZF      ].SetAlias(  8, "MInArmZF");
    MOT[MInArmZG      ].SetAlias(  9, "MInArmZG");
    MOT[MInArmZH      ].SetAlias( 10, "MInArmZH");
    MOT[MInShuttle1   ].SetAlias( 11, "MInShutte1");                            //不可以改成MInShuttle1, 會造成Teaching異常
    MOT[MInShuttle2   ].SetAlias( 12, "MInShutte2");
    MOT[MTestY1       ].SetAlias( 13, "MTestY1");
    MOT[MTestZ1       ].SetAlias( 14, "MTestZ1");
    MOT[MTestZ2       ].SetAlias( 15, "MTestZ2");
    MOT[MTestY2       ].SetAlias( 16, "MTestY2");
    MOT[MOutShuttle1  ].SetAlias( 17, "MOutShuttle1");
    MOT[MOutShuttle2  ].SetAlias( 18, "MOutShuttle2");
    MOT[MOutArmX      ].SetAlias( 19, "MOutArmX");
    MOT[MOutArmY      ].SetAlias( 20, "MOutArmY");
    MOT[MOutArmPitch  ].SetAlias( 21, "MOutArmPitch");
    MOT[MOutArmZA     ].SetAlias( 22, "MOutArmZA");
    MOT[MOutArmZB     ].SetAlias( 23, "MOutArmZB");
    MOT[MOutArmZC     ].SetAlias( 24, "MOutArmZC");
    MOT[MOutArmZD     ].SetAlias( 25, "MOutArmZD");
    MOT[MOutArmZE     ].SetAlias( 26, "MOutArmZE");
    MOT[MOutArmZF     ].SetAlias( 27, "MOutArmZF");
    MOT[MOutArmZG     ].SetAlias( 28, "MOutArmZG");
    MOT[MOutArmZH     ].SetAlias( 29, "MOutArmZH");
    MOT[MTrayX        ].SetAlias( 30, "MTrayX");
    MOT[MInArmPitchY  ].SetAlias( 31, "MInArmPitchY");
    MOT[MInArmPitchX2 ].SetAlias( 32, "MInArmPitchX2");
    MOT[MOutArmPitchY ].SetAlias( 33, "MOutArmPitchY");
    MOT[MOutArmPitchX2].SetAlias( 34, "MOutArmPitchX2");
    MOT[MLoaderZ      ].SetAlias( 35, "MLoaderZ");
    MOT[MEmptyZ       ].SetAlias( 36, "MEmptyZ");
    MOT[MColorZ       ].SetAlias( 37, "MColorZ");
    MOT[MAuto1Z       ].SetAlias( 38, "MAuto1Z");
    MOT[MAuto2Z       ].SetAlias( 39, "MAuto2Z");
    MOT[MAuto3Z       ].SetAlias( 40, "MAuto3Z");
    MOT[MInRotateKit  ].SetAlias( 41, "MInRotate");                             //不可以改成MInRotateKit, 會造成Teaching異常
    MOT[MOutRotateKit ].SetAlias( 42, "MOutRotate");
    MOT[MAOIKit       ].SetAlias( 43, "MAOIKit");
    MOT[MLoaderY      ].SetAlias( 44, "MLoaderY");
    MOT[MEmptyY       ].SetAlias( 45, "MEmptyY");
    MOT[MColorY       ].SetAlias( 46, "MColorY");
    MOT[MAuto1Y       ].SetAlias( 47, "MAuto1Y");
    MOT[MAuto2Y       ].SetAlias( 48, "MAuto2Y");
    MOT[MAuto3Y       ].SetAlias( 49, "MAuto3Y");
    MOT[MInArmZAe     ].SetAlias( 50, "MInArmZAe");                             //Steven 20230323 : For HT1032
    MOT[MInArmPitchX3 ].SetAlias( 51, "MInArmPitchX3");
    MOT[MInArmPitchX4 ].SetAlias( 52, "MInArmPitchX4");
    MOT[MInArmZAf     ].SetAlias( 53, "MInArmZAf");
    MOT[MOutArmPitchX3].SetAlias( 54, "MOutArmPitchX3");
    MOT[MOutArmPitchX4].SetAlias( 55, "MOutArmPitchX4");
    MOT[MTrayZ        ].SetAlias( 56, "MTrayZ");
    MOT[MOutSortAa    ].SetAlias( 57, "MOutSortAa");                            //Steven 20240822 : For HT-9046AU
    MOT[MOutSortAb    ].SetAlias( 58, "MOutSortAb");
    MOT[MInArmXScale  ].SetAlias( 59, "MInArmXScale");
    MOT[MInArmYScale  ].SetAlias( 60, "MInArmYScale");
    MOT[MOutArmXScale ].SetAlias( 61, "MOutArmXScale");
    MOT[MOutArmYScale ].SetAlias( 62, "MOutArmYScale");
    MOT[MShuttle1Pitch].SetAlias( 63, "MShuttle1Pitch");
    MOT[MShuttle2Pitch].SetAlias( 64, "MShuttle2Pitch");
    MOT[MInRotateB    ].SetAlias( 65, "MInRotateB");
    MOT[MInRotateC    ].SetAlias( 66, "MInRotateC");
    MOT[MInRotateD    ].SetAlias( 67, "MInRotateD");
    MOT[MInRotateE    ].SetAlias( 68, "MInRotateE");
    MOT[MInRotateF    ].SetAlias( 69, "MInRotateF");
    MOT[MInRotateG    ].SetAlias( 70, "MInRotateG");
    MOT[MInRotateH    ].SetAlias( 71, "MInRotateH");
    MOT[MOutRotateB   ].SetAlias( 72, "MOutRotateB");
    MOT[MOutRotateC   ].SetAlias( 73, "MOutRotateC");
    MOT[MOutRotateD   ].SetAlias( 74, "MOutRotateD");
    MOT[MOutRotateE   ].SetAlias( 75, "MOutRotateE");
    MOT[MOutRotateF   ].SetAlias( 76, "MOutRotateF");
    MOT[MOutRotateG   ].SetAlias( 77, "MOutRotateG");
    MOT[MOutRotateH   ].SetAlias( 78, "MOutRotateH");
    MOT[MLightScale   ].SetAlias( 79, "MLightScale");
    MOT[MInArmZAg     ].SetAlias( 80, "MInArmZAg");
    MOT[MInArmZAh     ].SetAlias( 81, "MInArmZAh");
    MOT[MArmAlignment ].SetAlias( 82, "MArmAlignment");                         //Steven 20240507 : 只是為了Teaching存檔方便;
    MOT[MLoadHingeR   ].SetAlias( 83, "MLoadHingeR");
    MOT[MLoadHingeZ   ].SetAlias( 84, "MLoadHingeZ");
    MOT[MPreciser     ].SetAlias( 85, "MPreciser");
    MOT[MInArmZBe     ].SetAlias( 86, "MInArmZBe");
    MOT[MInArmZBf     ].SetAlias( 87, "MInArmZBf");
    MOT[MInArmZBg     ].SetAlias( 88, "MInArmZBg");
    MOT[MInArmZBh     ].SetAlias( 89, "MInArmZBh");
    MOT[MOutArmZAe    ].SetAlias( 90, "MOutArmZAe");
    MOT[MOutArmZAf    ].SetAlias( 91, "MOutArmZAf");
    MOT[MOutArmZAg    ].SetAlias( 92, "MOutArmZAg");
    MOT[MOutArmZAh    ].SetAlias( 93, "MOutArmZAh");
    MOT[MOutArmZBe    ].SetAlias( 94, "MOutArmZBe");
    MOT[MOutArmZBf    ].SetAlias( 95, "MOutArmZBf");
    MOT[MOutArmZBg    ].SetAlias( 96, "MOutArmZBg");
    MOT[MOutArmZBh    ].SetAlias( 97, "MOutArmZBh");
    MOT[MOutSortX     ].SetAlias( 98, "MOutSortX");                             //Steven 20240822 : For HT-9046AU
    MOT[MCaselevatorZ ].SetAlias( 99, "MCaselevatorZ");
    MOT[MCasArmX      ].SetAlias(100, "MCasArmX");
    MOT[MCasArmZ      ].SetAlias(101, "MCasArmZ");
    MOT[MTrayBracketZ ].SetAlias(102, "MTrayBracketZ");
    MOT[MStackedTrayX ].SetAlias(103, "MStackedTrayX");
    MOT[MStackedTrayZ ].SetAlias(104, "MStackedTrayZ");
    MOT[MUnloadRobotZ ].SetAlias(105, "MUnloadRobotZ");
    MOT[MOutSortY     ].SetAlias(106, "MOutSortY");                             //Steven 20240822 : For HT-9046AU
    MOT[MCCDX         ].SetAlias(107, "MCCDX");
    MOT[MCCDY         ].SetAlias(108, "MCCDY");
    MOT[MCCDZ         ].SetAlias(109, "MCCDZ");
    MOT[MInFlipper1   ].SetAlias(110, "MInFlipper1");
    MOT[MInFlipper2   ].SetAlias(111, "MInFlipper2");
    MOT[MInFlipper3   ].SetAlias(112, "MInFlipper3");
    MOT[MOutFlipper1  ].SetAlias(113, "MOutFlipper1");
    MOT[MOutFlipper2  ].SetAlias(114, "MOutFlipper2");
    MOT[MOutFlipper3  ].SetAlias(115, "MOutFlipper3");
    MOT[MLdCarRotArm  ].SetAlias(116, "MLdCarRotArm");
    MOT[MLoaderY_CCW  ].SetAlias(117, "MLoaderY_CCW");
    MOT[MAuto1Y_CCW   ].SetAlias(118, "MAuto1Y_CCW");
    MOT[MAuto2Y_CCW   ].SetAlias(119, "MAuto2Y_CCW");
    MOT[MAuto3Y_CCW   ].SetAlias(120, "MAuto3Y_CCW");
    MOT[MAuto4Y_CCW   ].SetAlias(121, "MAuto4Y_CCW");
    MOT[MAuto5Y_CCW   ].SetAlias(122, "MAuto5Y_CCW");
    MOT[MAuto6Y_CCW   ].SetAlias(123, "MAuto6Y_CCW");
    MOT[M1_3R         ].SetAlias(124, "M1_3R");
    MOT[M1_4X         ].SetAlias(125, "M1_4X");
    MOT[M1_4Y         ].SetAlias(126, "M1_4Y");
    MOT[M1_4R         ].SetAlias(127, "M1_4R");
    MOT[M1_5X         ].SetAlias(128, "M1_5X");
    MOT[M1_5Y         ].SetAlias(129, "M1_5Y");
    MOT[M1_5R         ].SetAlias(130, "M1_5R");
    MOT[M1_6X         ].SetAlias(131, "M1_6X");
    MOT[M1_6Y         ].SetAlias(132, "M1_6Y");
    MOT[M1_6R         ].SetAlias(133, "M1_6R");
    MOT[M1_7X         ].SetAlias(134, "M1_7X");
    MOT[M1_7Y         ].SetAlias(135, "M1_7Y");
    MOT[M1_7R         ].SetAlias(136, "M1_7R");
    MOT[M1_8X         ].SetAlias(137, "M1_8X");
    MOT[M1_8Y         ].SetAlias(138, "M1_8Y");
    MOT[M1_8R         ].SetAlias(139, "M1_8R");
    MOT[MMagazine     ].SetAlias(140, "MMagazine");                             //JerryYang 20221215 : add Magazine
    MOT[MCatchMgzTray ].SetAlias(141, "MCatchMgzTray");
    MOT[MMagYTrayOut  ].SetAlias(142, "MMagYTrayOut");

    MOT[MFix3Full     ].SetAlias(143, "MFix3Full");
    MOT[MAuto4Z       ].SetAlias(144, "MAuto4Z");                               //Steven 20230907 : For HT-9011UC
    MOT[MAuto5Z       ].SetAlias(145, "MAuto5Z");
    MOT[MAuto6Z       ].SetAlias(146, "MAuto6Z");
    MOT[MAuto4Y       ].SetAlias(147, "MAuto4Y");
    MOT[MAuto5Y       ].SetAlias(148, "MAuto5Y");
    MOT[MAuto6Y       ].SetAlias(149, "MAuto6Y");

    MOT[MTopAOIArmX   ].SetAlias(150, "MTopAOIArmX");
    MOT[MTopAOIArmY   ].SetAlias(151, "MTopAOIArmY");
    MOT[MTopAOIArmR   ].SetAlias(152, "MTopAOIArmR");
    MOT[MTopAOICCDZ   ].SetAlias(153, "MTopAOICCDZ");
    MOT[MTopAOIElevZ1 ].SetAlias(154, "MTopAOIElevZ1");
    MOT[MTopAOIElevZ2 ].SetAlias(155, "MTopAOIElevZ2");

    MOT[MOutSortPitchX].SetAlias(156, "MOutSortPitchX");                        //Steven 20240822 : For HT-9046AU
    MOT[MOutSortSht   ].SetAlias(157, "MOutSortSht");
    MOT[MLoad2Z       ].SetAlias(158, "MLoad2Z");
    MOT[MLoad2Y       ].SetAlias(159, "MLoad2Y");

    MOT[MInSh1LtcSenZ1].SetAlias(160, "MInSh1LtcSenZ1");                        //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    MOT[MInSh1LtcSenZ2].SetAlias(161, "MInSh1LtcSenZ2");
    MOT[MInSh2LtcSenZ1].SetAlias(162, "MInSh2LtcSenZ1");
    MOT[MInSh2LtcSenZ2].SetAlias(163, "MInSh2LtcSenZ2");
    //虛擬Tray------------------------------------------------------------------
    MOT[MManualTray1            ].SetAlias(164, "MManualTray1");
    MOT[MManualTray2            ].SetAlias(165, "MManualTray2");
    MOT[MManualTray3            ].SetAlias(166, "MManualTray3");
    MOT[MMTrayY                 ].SetAlias(167, "MMTrayY");
    MOT[MMTrayY_Car             ].SetAlias(168, "MMTrayY_Car");
    MOT[MMPlate1                ].SetAlias(169, "MMPlate1");
    MOT[MMPlate2                ].SetAlias(170, "MMPlate2");
    MOT[MMAuto1                 ].SetAlias(171, "MMAuto1");
    MOT[MMAuto2                 ].SetAlias(172, "MMAuto2");
    MOT[MMAuto3                 ].SetAlias(173, "MMAuto3");
    MOT[MMAuto1_Car             ].SetAlias(174, "MMAuto1_Car");
    MOT[MMAuto2_Car             ].SetAlias(175, "MMAuto2_Car");
    MOT[MMAuto3_Car             ].SetAlias(176, "MMAuto3_Car");
    MOT[MMEmpty                 ].SetAlias(177, "MMEmpty");
    MOT[MMColor                 ].SetAlias(178, "MMColor");
    MOT[MMEmpty_Car             ].SetAlias(179, "MMEmpty_Car");
    MOT[MMColor_Car             ].SetAlias(180, "MMColor_Car");
    MOT[MMEmpty1                ].SetAlias(181, "MMEmpty1");
    MOT[MMEmpty1_Car            ].SetAlias(182, "MMEmpty1_Car");
    MOT[MMHot1RecBuf            ].SetAlias(183, "MMHot1RecBuf");
    MOT[MMHot2RecBuf            ].SetAlias(184, "MMHot2RecBuf");
    MOT[MMAutoCleanKit          ].SetAlias(185, "MMAutoCleanKit");
    MOT[MMOCR                   ].SetAlias(186, "MMOCR");
    MOT[MMBulkboxKit            ].SetAlias(187, "MMBulkboxKit");
    MOT[MMCABuffer1             ].SetAlias(188, "MMCABuffer1");
    MOT[MMCABuffer2             ].SetAlias(189, "MMCABuffer2");
    MOT[MMCABuffer3             ].SetAlias(190, "MMCABuffer3");
    MOT[MMCABuffer4             ].SetAlias(191, "MMCABuffer4");
    MOT[MMCABuffer5             ].SetAlias(192, "MMCABuffer5");
    MOT[MMCABuffer6             ].SetAlias(193, "MMCABuffer6");
    MOT[MMCABuffer7             ].SetAlias(194, "MMCABuffer7");
    MOT[MMCABuffer8             ].SetAlias(195, "MMCABuffer8");
    MOT[MMCABuffer9             ].SetAlias(196, "MMCABuffer9");
    MOT[MMCABuffer10            ].SetAlias(197, "MMCABuffer10");
    MOT[MMLoadPort              ].SetAlias(198, "MMLoadPort");
    MOT[MMTrayLoader            ].SetAlias(199, "MMTrayLoader");
    MOT[MMTrayEmpty             ].SetAlias(200, "MMTrayEmpty");
    MOT[MMTrayConversion        ].SetAlias(201, "MMTrayConversion");
    MOT[MMTrayAuto1             ].SetAlias(202, "MMTrayAuto1");
    MOT[MMTrayAuto2             ].SetAlias(203, "MMTrayAuto2");
    MOT[MMTrayAuto3             ].SetAlias(204, "MMTrayAuto3");
    MOT[MMFixTray1              ].SetAlias(205, "MMFixTray1");
    MOT[MMFixTray2              ].SetAlias(206, "MMFixTray2");
    MOT[MMFixTray3              ].SetAlias(207, "MMFixTray3");
    MOT[MMScanAOI               ].SetAlias(208, "MMScanAOI");
    MOT[MMInArmAOATray          ].SetAlias(209, "MMInArmAOATray");
    MOT[MMOutArmAOATray         ].SetAlias(210, "MMOutArmAOATray");
    MOT[MMAOASampleTray         ].SetAlias(211, "MMAOASampleTray");
    MOT[MMAOASamplePlate        ].SetAlias(212, "MMAOASamplePlate");
    MOT[MInPlacementX           ].SetAlias(213, "MInPlacementX");
    MOT[MInPlacementY           ].SetAlias(214, "MInPlacementY");
    MOT[MMMagazineTary1         ].SetAlias(215, "MMMagazineTary1");
    MOT[MMMagazineTary2         ].SetAlias(216, "MMMagazineTary2");
    MOT[MMMagazineTary3         ].SetAlias(217, "MMMagazineTary3");
    MOT[MMMagazineTary4         ].SetAlias(218, "MMMagazineTary4");
    MOT[MMMagazineTary5         ].SetAlias(219, "MMMagazineTary5");
    MOT[MMMagazineTary6         ].SetAlias(220, "MMMagazineTary6");
    MOT[MMMagazineTary7         ].SetAlias(221, "MMMagazineTary7");
    MOT[MMMagazineTary8         ].SetAlias(222, "MMMagazineTary8");
    MOT[MMMagazineTary9         ].SetAlias(223, "MMMagazineTary9");
    MOT[MMMagazineTary10        ].SetAlias(224, "MMMagazineTary10");
    MOT[MMMagazineTary11        ].SetAlias(225, "MMMagazineTary11");
    MOT[MMMagazineTary12        ].SetAlias(226, "MMMagazineTary12");
    MOT[MMMagazineTary13        ].SetAlias(227, "MMMagazineTary13");
    MOT[MMMagazineTary14        ].SetAlias(228, "MMMagazineTary14");
    MOT[MMMagazineTaryTop       ].SetAlias(229, "MMMagazineTaryTop");
    MOT[MMMagazineBuffer        ].SetAlias(230, "MMMagazineBuffer");
    MOT[MMBackupMagazineTary1   ].SetAlias(231, "MMBackupMagazineTary1");
    MOT[MMBackupMagazineTary2   ].SetAlias(232, "MMBackupMagazineTary2");
    MOT[MMBackupMagazineTary3   ].SetAlias(233, "MMBackupMagazineTary3");
    MOT[MMBackupMagazineTary4   ].SetAlias(234, "MMBackupMagazineTary4");
    MOT[MMBackupMagazineTary5   ].SetAlias(235, "MMBackupMagazineTary5");
    MOT[MMBackupMagazineTary6   ].SetAlias(236, "MMBackupMagazineTary6");
    MOT[MMBackupMagazineTary7   ].SetAlias(237, "MMBackupMagazineTary7");
    MOT[MMBackupMagazineTary8   ].SetAlias(238, "MMBackupMagazineTary8");
    MOT[MMBackupMagazineTary9   ].SetAlias(239, "MMBackupMagazineTary9");
    MOT[MMBackupMagazineTary10  ].SetAlias(240, "MMBackupMagazineTary10");
    MOT[MMBackupMagazineTary11  ].SetAlias(241, "MMBackupMagazineTary11");
    MOT[MMBackupMagazineTary12  ].SetAlias(242, "MMBackupMagazineTary12");
    MOT[MMBackupMagazineTary13  ].SetAlias(243, "MMBackupMagazineTary13");
    MOT[MMBackupMagazineTary14  ].SetAlias(244, "MMBackupMagazineTary14");
    MOT[MMTrayZ                 ].SetAlias(245, "MMTrayZ");
    MOT[MMEmptyZ                ].SetAlias(246, "MMEmptyZ");
    MOT[MMColorZ                ].SetAlias(247, "MMColorZ");
    MOT[MMAuto1Z                ].SetAlias(248, "MMAuto1Z");
    MOT[MMAuto2Z                ].SetAlias(249, "MMAuto2Z");
    MOT[MMAuto3Z                ].SetAlias(250, "MMAuto3Z");
    MOT[MMAuto4Z                ].SetAlias(251, "MMAuto4Z");
    MOT[MMAuto5Z                ].SetAlias(252, "MMAuto5Z");
    MOT[MMAuto6Z                ].SetAlias(253, "MMAuto6Z");
    MOT[MManualTray4            ].SetAlias(254, "MManualTray4");
    MOT[MManualTray5            ].SetAlias(255, "MManualTray5");
    MOT[MManualTray6            ].SetAlias(256, "MManualTray6");
    MOT[MMAuto4                 ].SetAlias(257, "MMAuto4");
    MOT[MMAuto5                 ].SetAlias(258, "MMAuto5");
    MOT[MMAuto6                 ].SetAlias(259, "MMAuto6");
    MOT[MMAuto4_Car             ].SetAlias(260, "MMAuto4_Car");
    MOT[MMAuto5_Car             ].SetAlias(261, "MMAuto5_Car");
    MOT[MMAuto6_Car             ].SetAlias(262, "MMAuto6_Car");
    MOT[MMFixTray4              ].SetAlias(263, "MMFixTray4");
    MOT[MMFixTray5              ].SetAlias(264, "MMFixTray5");
    MOT[MMFixTray6              ].SetAlias(265, "MMFixTray6");
    MOT[MMLoadPort1             ].SetAlias(266, "MMLoadPort1");
    MOT[MMLoadPort2             ].SetAlias(267, "MMLoadPort2");
    MOT[MMLoadPort3             ].SetAlias(268, "MMLoadPort3");
    MOT[MMLoadPort4             ].SetAlias(269, "MMLoadPort4");
    MOT[MMUnloadPort1           ].SetAlias(270, "MMUnloadPort1");
    MOT[MMUnloadPort2           ].SetAlias(271, "MMUnloadPort2");
    MOT[MMUnloadPort3           ].SetAlias(272, "MMUnloadPort3");
    MOT[MMUnloadPort4           ].SetAlias(273, "MMUnloadPort4");
}
//==============================================================================
void InitialMotorParameter()
{
    TTable *PT;
    AnsiString S, sDBDir;
    int iAdder, iMot;
    AnsiString Mot_Name;
    AnsiString sModel;
    int SoftLimit[2];
    int iIP;
    int iLane;
    int iPort;
    double dAcc, dDec;
    bool flag;
    bool bHasMotor=false;                                                       //Stevem 20210625 : 修正新的CSV IO表讀檔方式

    InitialMotorName();

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        HSys.LoadMotData();
        for(int i=0; i<TOTAL_MOTOR; i++)
        {
            iMot=-1;
            bHasMotor=false;
            Mot_Name.sprintf("M%02d", i);
            HSys.mapMotTableIter=HSys.mapMotTable.find(Mot_Name);
            if(HSys.mapMotTableIter!=HSys.mapMotTable.end())
            {
                iMot=atoi(HSys.mapMotTable[Mot_Name].c_str());
                if(iMot<0)                                                      //Steven 20250520 : 變更Mot初始化方式
                {
                    if(i==11)
                    {
                        iMot=atoi(HSys.mapMotTable["MInShuttle1"].c_str());
                    }
                    else if(i==12)
                    {
                        iMot=atoi(HSys.mapMotTable["MInShuttle2"].c_str());
                    }

                    if(iMot<0)                                                  //Steven 20250520 : 變更Mot初始化方式
                    {
                        S.sprintf("Can not find motor %s", Mot_Name);
                    }
                    else
                    {
                        bHasMotor=true;
                    }
                }
                else
                {
                    bHasMotor=true;
                }
            }
            else
            {
                S.sprintf("Can not find motor %s", Mot_Name);
            }

            if(bHasMotor)
            {
                sModel          =HSys.MotTable[iMot]->CardModel;
//                MOT[i].Alias    =HSys.MotTable[iMot]->Alias;
            }
            else
            {
                sModel          ="";
            }

//            MOT[i].NumberAlias  =AnsiString("[")+AnsiString(Mot_Name)+AnsiString("] ")+MOT[i].Alias;

            if(INDEX_MOTION_CARD==0 &&                                          //Steven 20210621 : for HT-502 II
               (i==MTestZ1 || i==MTestZ2 || i==MTestY1 || i==MTestY2))
            {
                int iGalilPort=0;                                               //AI(general) 20260316 (RogerYang) : Use TMyGALILMotor for index axes and map MTestY1/Z1/Z2/Y2 to X/Y/Z/W.
                if(i==MTestY1)
                    iGalilPort=0;
                else if(i==MTestZ1)
                    iGalilPort=1;
                else if(i==MTestZ2)
                    iGalilPort=2;
                else
                    iGalilPort=3;

                if(MOT[i].Motor!=NULL)
                {
                    delete MOT[i].Motor;
                    MOT[i].Motor=NULL;
                }
                MOT[i].Motor=new TMyGALILMotor(iGalilPort);
                #ifdef SOFT_SIMULTE
                    MOT[i].Motor->Enable=false;
                #else
                if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&                       //JimmyChiu 20220708 : add Index Arm Axis
                   (i==MTestY2))
                {
                    MOT[i].Motor->Enable=false;
                }
                else
                {
                    MOT[i].Motor->Enable=true;
                }
                #endif
            }
            else
            {
                if(MOT[i].Motor==NULL)
                {
                    if(sModel=="MN200")                                         //Steven 20150417 : ADD MN200軸控
                    {
                        if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1)
                            iAdder=HSys.MotTable[iMot]->iBoardID*100+HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=-1;

                        MOT[i].Motor=new TMyMN200Motor(iAdder);
                    }
                    else if(sModel=="MC88X1")                                   //Jimmychiu 20220926 : ADD MC88X1
                    {
                        if(HSys.MotTable[iMot]->iBoardID==-1)
                            iAdder=HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=HSys.MotTable[iMot]->iBoardID*0x10+HSys.MotTable[iMot]->iPort;
//                         MOT[i].Motor = new HTMC88X1Motor(iAdder);            //Steven 20231218 HT7080B
                    }
                    else if(sModel=="SYNTEK")
                    {
                        if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1 && HSys.MotTable[iMot]->iIP!=-1)
                            iAdder=HSys.MotTable[iMot]->iBoardID*1000+HSys.MotTable[iMot]->iIP*100+HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=-1;
                        MOT[i].Motor=new TMySYNTEKMotor(iAdder);
                    }
                    else
                    {
                        if(bHasMotor && HSys.MotTable[iMot]->iBoardID!=-1 && HSys.MotTable[iMot]->iPort!=-1)
                            iAdder=HSys.MotTable[iMot]->iBoardID*10+HSys.MotTable[iMot]->iPort;
                        else
                            iAdder=-1;

                        MOT[i].Motor=new TMySMCMotor(iAdder);

                        if(MOT[i].Mot_Name==MInShuttle1 ||                      //jou 2014-10-09 修正Device superfluous at Output Shuttle誤報錯誤
                           MOT[i].Mot_Name==MInShuttle2)
                            SYN_TEK_MOTION_MODULE=G9004_M204;
                    }
                    #ifdef SOFT_SIMULTE
                        MOT[i].Motor->Enable=false;
                    #else
                        if((sModel=="MC88X1" && iAdder<=0) ||                   //Steven 20240112 : Fixed for HT7080B
                           (sModel!="MC88X1" && iAdder<0)  ||
                           bHasMotor==false)
                        {
                            MOT[i].Motor->Enable=false;
                        }
                        else
                        {
                            MOT[i].Motor->Enable=HSys.MotTable[iMot]->iEnable;
                        }
                    #endif
                }
                else
                {
                    #ifdef SOFT_SIMULTE
                        MOT[i].Motor->Enable=false;
                    #else
                        if(bHasMotor)
                            MOT[i].Motor->Enable=HSys.MotTable[iMot]->iEnable;
                        else
                            MOT[i].Motor->Enable=false;
                    #endif
                }
            }

            if(bHasMotor)
            {
                MOT[i].Motor->GearRatio         = HSys.MotTable[iMot]->dGearRatio;
                MOT[i].Motor->Direction         =(HSys.MotTable[iMot]->iDirection==1)?true:false;
                MOT[i].Motor->HomeDirection     =(HSys.MotTable[iMot]->iHomeDirectior==1)?true:false;
                dAcc=HSys.MotTable[iMot]->dAcc;
                dDec=HSys.MotTable[iMot]->dDec;
                if(sModel=="MN200")                                             //Steven 20230616 : MN200的加減速單位是秒
                {
                    if(dAcc>1)
                        dAcc=HSys.MotTable[iMot]->dAcc/100.0;
                    if(dDec>1)
                        dDec=HSys.MotTable[iMot]->dDec/100.0;
                }
                else if(sModel=="MC88X1")                                       //Nickliu 20240108 add MCXX8 Set Rate
                {
                    dAcc=HSys.MotTable[iMot]->iRate;
                    dDec=HSys.MotTable[iMot]->iRate;
                    MOT[i].Motor->SetRate    (HSys.MotTable[iMot]->iRate);      //Nickliu 20230315 add MCXX8 Set Rate
                }

                MOT[i].Motor->SetAccDataBase     (dAcc);
                MOT[i].Motor->SetDecDataBase     (dDec);
                MOT[i].Motor->SetRange           (HSys.MotTable[iMot]->iRange);
                MOT[i].Motor->PHomeHighSpeed    = HSys.MotTable[iMot]->iHomeHighSpeed;                 //Steven 20231218 HT7080B
                MOT[i].Motor->PHomeLowSpeed     = HSys.MotTable[iMot]->iHomeLowSpeed;
                MOT[i].Motor->PJogHighSpeed     = HSys.MotTable[iMot]->iJogHighSpeed;
                MOT[i].Motor->PJogLowSpeed      = HSys.MotTable[iMot]->iJogLowSpeed;
                MOT[i].Motor->SetInitSpeed       (HSys.MotTable[iMot]->iInitSpeed);
                MOT[i].Motor->InitSpeed         = HSys.MotTable[iMot]->iInitSpeed;
                MOT[i].Motor->PServoAlarmOn     =(HSys.MotTable[iMot]->iServoAlarmOn==1)?true:false;
                MOT[i].Motor->MotorType         = HSys.MotTable[iMot]->i1P2P;
                MOT[i].Motor->bSensorType       = HSys.MotTable[iMot]->iSensorType;
                MOT[i].Motor->bLimitLogic       =(HSys.MotTable[iMot]->iLimitLogic==1)?true:false;
                MOT[i].Motor->bIn1Logic         =(HSys.MotTable[iMot]->iIn1Logic==1)?true:false;
                MOT[i].Motor->PSoftLimitN       = HSys.MotTable[iMot]->iSoftLimitN;
                MOT[i].Motor->PSoftLimitP       = HSys.MotTable[iMot]->iSoftLimitP;
                MOT[i].SimulateSpeed            = HSys.MotTable[iMot]->iSimulateSpeed;
            }
            MOT[i].HomeFlag                 = 0;
            SetMotorAccelSpeed(i, 100);

            if(MOT[i].Mot_Name==MInShuttle1)                                    //kevin 20110531 旋轉shuttle記錄速度
            {
                iInitSpeedSh1=MOT[i].Motor->InitSpeed;
                iPJogHighSpeedSh1=MOT[i].Motor->PJogHighSpeed;
            }

            if(MOT[i].Mot_Name==MInShuttle2)                                    //kevin 20110531 旋轉shuttle記錄速度
            {
                iInitSpeedSh2=MOT[i].Motor->InitSpeed;
                iPJogHighSpeedSh2=MOT[i].Motor->PJogHighSpeed;
            }

            if(bHasMotor && (i==MTestZ1 || i==MTestZ2))                         //Steven 20210623 : Index使用Galil
                MOT[i].IndexPickLimit=HSys.MotTable[iMot]->iPickLimit;

            if(INDEX_MOTION_CARD==0 &&                                          //Steven 20210623 : Index使用Galil
               (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                ;                                                               //Steven 20090922 需要修改
            else
                MOT[i].SetArmMaxSpeed();

            SoftLimit[0]=MOT[i].Motor->PSoftLimitN;
            SoftLimit[1]=MOT[i].Motor->PSoftLimitP;

            if(MOT[i].Motor->Enable)
                MOT[i].Motor->InitMotor(iAdder);
        }
    }
    else if(MOTION_CARD_TYPE==0)                                                //SYN-TEK Motion模組
    {
        sDBDir=AnsiString(CurrentDir)+"\\system\\motor.db";                     //JerryYang 20150903 檢查Database檔是否存在
        if(FileExists(sDBDir))
        {
            DataModule1->MotorTable->TableName=sDBDir;
        }
        else
        {
            ShowMyMessage("motor.db does not exist");
            return;
        }

        PT=DataModule1->MotorTable;
        PT->Open();

        for(int i=0; i<TOTAL_MOTOR; i++)
        {
            MOT[i].Mot_Name=i;
            PT->First();
            Mot_Name.sprintf("M%02d", i);

            flag=false;
            while(!PT->Eof)
            {
                if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
                {
                    flag=true;
                    break;
                }
                PT->Next();
            }

            if(flag==false)
            {
                if(MOT[i].Motor==NULL)
                    MOT[i].Motor=new TMySMCMotor(-1);
                 MOT[i].Motor->Enable=false;
                S.sprintf("Can not find motor %s", Mot_Name);
                continue;
            }

            if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
            {
                MOT[i].Alias=CheckMotorValue(PT, "Alias", false);
                MOT[i].NumberAlias=AnsiString("[")+Mot_Name+AnsiString("] ")+MOT[i].Alias;

                iLane   =(StrToIntDef(CheckMotorValue(PT, "Lane", true), -1));
                iIP     =(StrToIntDef(CheckMotorValue(PT, "IP"  , true), -1));
                iPort   =(StrToIntDef(CheckMotorValue(PT, "Port", true), -1));

                if(iIP==-1 || iLane==-1 || iPort==-1)
                {
                    if(MOT[i].Motor==NULL)
                        MOT[i].Motor=new TMySYNTEKMotor(-1);
                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                    {
                        int iGalilPort=0;                                       //AI(general) 20260316 (RogerYang) : Keep Galil axis-port mapping consistent in fallback branch.
                        if(i==MTestY1)
                            iGalilPort=0;
                        else if(i==MTestZ1)
                            iGalilPort=1;
                        else if(i==MTestZ2)
                            iGalilPort=2;
                        else
                            iGalilPort=3;

                        if(MOT[i].Motor!=NULL)
                        {
                            delete MOT[i].Motor;
                            MOT[i].Motor=NULL;
                        }
                        MOT[i].Motor=new TMyGALILMotor(iGalilPort);
                        #ifdef SOFT_SIMULTE
                            MOT[i].Motor->Enable=false;
                        #else
                            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&           //JimmyChiu 20220708 : add Index Arm Axis
                               (i==MTestY2))
                            {
                                MOT[i].Motor->Enable=false;
                            }
                            else
                            {
                                MOT[i].Motor->Enable=true;
                            }
                        #endif
                    }
                    else
                    {
                        MOT[i].Motor->Enable=false;
                    }
                }
                else
                {
                    if(MOT[i].Motor==NULL)
                        MOT[i].Motor = new TMySYNTEKMotor((iLane*1000+iIP*10+iPort));
                    #ifdef SOFT_SIMULTE
                        MOT[i].Motor->Enable=false;
                    #else
                        MOT[i].Motor->Enable=(atoi(CheckMotorValue(PT,"Enable"   ,true).c_str())==1)?true:false;
                    #endif
                }

                if(MOT[i].Motor->Enable)
                {
                    if(CheckMotorValue(PT,"GearRatio",true)!="" &&
                       CheckMotorValue(PT,"GearRatio",true)!="0")
                    {
                        MOT[i].Motor->GearRatio    = atof(CheckMotorValue(PT,"GearRatio"      ,true).c_str());
                    }
                    else
                    {
                        MOT[i].Motor->GearRatio=1.0;
                    }

                    MOT[i].Motor->Direction         =(atoi(CheckMotorValue(PT,"Direction"      ,true).c_str())==1)?true:false;
                    MOT[i].Motor->HomeDirection     =(atoi(CheckMotorValue(PT,"HomeDirectior"  ,true).c_str())==1)?true:false;
                    MOT[i].Motor->PHomeHighSpeed    = atoi(CheckMotorValue(PT,"HomeHighSpeed"  ,true).c_str());
                    MOT[i].Motor->PHomeLowSpeed     = atoi(CheckMotorValue(PT,"HomeLowSpeed"   ,true).c_str());
                    MOT[i].Motor->PJogHighSpeed     = atoi(CheckMotorValue(PT,"JogHighSpeed"   ,true).c_str());
//                    MOT[i].Motor->PJogLowSpeed      = atoi(CheckMotorValue(PT,"JogLowSpeed"    ,true).c_str());   //Steven 20200811 : 先達版本沒有JogLowSpeed
                    MOT[i].Motor->InitSpeed         = atoi(CheckMotorValue(PT,"InitSpeed"      ,true).c_str());
                    MOT[i].Motor->PServoAlarmOn     =(atoi(CheckMotorValue(PT,"ServoAlarmOn"   ,true).c_str())==1)?true:false;
                    MOT[i].Motor->MotorType         = atoi(CheckMotorValue(PT,"1P2P"           ,true).c_str());
                    MOT[i].Motor->bSensorType       = atoi(CheckMotorValue(PT,"HomeSenLogic"   ,true).c_str());
//                        MOT[i].SimulateSpeed            = atoi(CheckMotorValue(PT,"SimulateSpeed"  )->AsString.c_str());
                    MOT[i].HomeFlag                 = 0;

                    if(MOT[i].Mot_Name==MInShuttle1)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh1=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh1=MOT[i].Motor->PJogHighSpeed;
                    }

                    if(MOT[i].Mot_Name==MInShuttle2)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh2=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh2=MOT[i].Motor->PJogHighSpeed;
                    }

                    SoftLimit[0]=atoi(CheckMotorValue(PT,"SoftLimitN",true).c_str());
                    SoftLimit[1]=atoi(CheckMotorValue(PT,"SoftLimitP",true).c_str());

                    if(INDEX_MOTION_CARD==0 &&  (i==MTestZ1 || i==MTestZ2))     //Steven 20210623 : Index使用Galil
                        MOT[i].IndexPickLimit=atoi(CheckMotorValue(PT, "PickLimit", true).c_str());

                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                        ;                                                       //Steven 20090922 需要修改
                    else
                        MOT[i].SetArmMaxSpeed();
                }

                MOT[i].Motor->PSoftLimitN=atoi(CheckMotorValue(PT,"SoftLimitN",true).c_str());
                MOT[i].Motor->PSoftLimitP=atoi(CheckMotorValue(PT,"SoftLimitP",true).c_str());
                SoftLimit[0]=MOT[i].Motor->PSoftLimitN;
                SoftLimit[1]=MOT[i].Motor->PSoftLimitP;

                //--------------------------------------------------------------
                //PCI Board
                MOT[i].Mot_Name=i;
                dAcc=PT->FieldByName("ADc")->AsFloat;
                dDec=PT->FieldByName("ADc")->AsFloat;
                if(sModel=="MN200")                                             //Steven 20230616 : MN200的加減速單位是秒
                {
                    if(dAcc>1)
                        dAcc=dAcc/100.0;
                    if(dDec>1)
                        dDec=dDec/100.0;
                }
                MOT[i].Motor->SetAccDataBase(dAcc);
                MOT[i].Motor->SetDecDataBase(dDec);

                if(MOT[i].Motor->Enable)
                {
                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                        ;
                    else
                        MOT[i].Motor->SetSoftLimit(SoftLimit[0], SoftLimit[1]);
                }
            }
        }
    }
    else
    {
        if(USE_IN_Y_IS_AUTO_PITCH==true || USE_OUT_Y_IS_AUTO_PITCH==true)                                              //Steven 20170424 (wei) : new XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
            DataModule1->MotorTable->TableName=CurrentDir+"\\system\\motor_SMC_XYPitch.db";
        else
            DataModule1->MotorTable->TableName=CurrentDir+"\\system\\motor_SMC.db";

        PT=DataModule1->MotorTable;
        PT->Open();

        for(int i=0; i<TOTAL_MOTOR; i++)
        {
            PT->First();
            MOT[i].Mot_Name=i;
            Mot_Name.sprintf("M%02d", i);

            flag=false;
            while(!PT->Eof)
            {
                if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
                {
                    flag=true;
                    break;
                }
                PT->Next();
            }
            if(flag==false)
            {
                if(MOT[i].Motor==NULL)
                    MOT[i].Motor = new TMySMCMotor(-1);
                 MOT[i].Motor->Enable=false;
                S.sprintf("Can not find motor %s", Mot_Name);
                continue;
            }

            S=CheckMotorValue(PT, "Motorname", false);
            if(CheckMotorValue(PT, "Motorname", false)==Mot_Name)
            {
                S=CheckMotorValue(PT, "Port", true);
                if(PT->FindField("CardModel")!=NULL)
                {
                    sModel=CheckMotorValue(PT, "CardModel", false);
                }

                MOT[i].Alias        =CheckMotorValue(PT,"Alias"          , false);
                MOT[i].NumberAlias  =AnsiString("[")+Mot_Name+AnsiString("] ")+MOT[i].Alias;

                if(sModel=="SMC" || sModel=="MN200" || sModel=="MC88X1")        //Steven 20150417 : ADD MN200軸控  //Jimmychiu 20220926 : ADD MC88X1
                {
                    if(INDEX_MOTION_CARD==0 &&                                  //Steven 20210623 : Index使用Galil
                       (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
                    {
                        int iGalilPort=0;                                       //AI(general) 20260316 (RogerYang) : Instantiate TMyGALILMotor for index Galil axes in SMC/MN200/MC88X1 branch.
                        if(i==MTestY1)
                            iGalilPort=0;
                        else if(i==MTestZ1)
                            iGalilPort=1;
                        else if(i==MTestZ2)
                            iGalilPort=2;
                        else
                            iGalilPort=3;

                        if(MOT[i].Motor!=NULL)
                        {
                            delete MOT[i].Motor;
                            MOT[i].Motor=NULL;
                        }
                        MOT[i].Motor = new TMyGALILMotor(iGalilPort);
                        #ifdef SOFT_SIMULTE
                            MOT[i].Motor->Enable=false;
                        #else
                            if(USE_INDEX_ARM_AXES==IndexArm_3_Axis &&
                               (i==MTestY2))                                    //JimmyChiu 20220708 : add Index Arm Axis
                            {
                                MOT[i].Motor->Enable=false;
                            }
                            else
                            {
                                MOT[i].Motor->Enable=true;
                            }
                        #endif
                    }
                    else
                    {
                        if(MOT[i].Motor==NULL)
                        {
                            if(sModel=="MN200")                                 //Steven 20150417 : ADD MN200軸控
                            {
                                iAdder=(atoi(CheckMotorValue(PT, "BoardID", true).c_str()))*100;
                                iAdder+=atoi(CheckMotorValue(PT, "Port", true).c_str());
                                MOT[i].Motor = new TMyMN200Motor(iAdder);
                            }
/*                            else if(sModel=="MC88X1")                         //Jimmychiu 20220926 : ADD MC88X1        //Steven 20231218 HT7080B
                            {
                                iAdder=atoi(CheckMotorValue(PT, "Port", true).c_str());
                                MOT[i].Motor = new HTMC88X1Motor(iAdder);
                            }*/
                            else
                            {
                                iAdder=(atoi(CheckMotorValue(PT, "BoardID", true).c_str()))*10;
                                iAdder+=atoi(CheckMotorValue(PT, "Port", true).c_str());
                                MOT[i].Motor = new TMySMCMotor(iAdder);
                            }
                        }
                        #ifdef SOFT_SIMULTE
                            MOT[i].Motor->Enable=false;
                        #else
                            MOT[i].Motor->Enable=(atoi(CheckMotorValue(PT, "Enable", true).c_str())==1)?true:false;
                        #endif

                        if(MOT[i].Mot_Name==MInShuttle1 ||                      //jou 2014-10-09 修正Device superfluous at Output Shuttle誤報錯誤
                           MOT[i].Mot_Name==MInShuttle2)
                        {
                            SYN_TEK_MOTION_MODULE=G9004_M204;
                        }
                    }

                    if(CheckMotorValue(PT, "GearRatio", true)!="" &&
                       CheckMotorValue(PT, "GearRatio", true)!="0")
                    {
                        MOT[i].Motor->GearRatio    = atof(CheckMotorValue(PT, "GearRatio"      ,true).c_str());
                    }
                    else
                    {
                        MOT[i].Motor->GearRatio=1.0;
                    }

                    MOT[i].Motor->Direction         =(atoi(CheckMotorValue(PT,"Direction"      ,true).c_str())==1)?true:false;
                    MOT[i].Motor->HomeDirection     =(atoi(CheckMotorValue(PT,"HomeDirectior"  ,true).c_str())==1)?true:false;
                    MOT[i].Motor->PHomeHighSpeed    = atoi(CheckMotorValue(PT,"HomeHighSpeed"  ,true).c_str());
                    MOT[i].Motor->PHomeLowSpeed     = atoi(CheckMotorValue(PT,"HomeLowSpeed"   ,true).c_str());
                    MOT[i].Motor->PJogHighSpeed     = atoi(CheckMotorValue(PT,"JogHighSpeed"   ,true).c_str());
                    MOT[i].Motor->PJogLowSpeed      = atoi(CheckMotorValue(PT,"JogLowSpeed"    ,true).c_str());
                    MOT[i].Motor->InitSpeed         = atoi(CheckMotorValue(PT,"InitSpeed"      ,true).c_str());
                    MOT[i].Motor->PServoAlarmOn     =(atoi(CheckMotorValue(PT,"ServoAlarmOn"   ,true).c_str())==1)?true:false;
                    MOT[i].Motor->MotorType         = atoi(CheckMotorValue(PT,"1P2P"           ,true).c_str()); //
                    MOT[i].Motor->bSensorType       = atoi(CheckMotorValue(PT,"SensorType"     ,true).c_str()); //
                    dAcc=atof(CheckMotorValue(PT,"Acc"            ,true).c_str());
                    dDec=atof(CheckMotorValue(PT,"Dec"            ,true).c_str());
                    if(sModel=="MN200")                                         //Steven 20230616 : MN200的加減速單位是秒
                    {
                        if(dAcc>1)
                            dAcc=dAcc/100.0;
                        if(dDec>1)
                            dDec=dDec/100.0;
                    }
                    MOT[i].Motor->SetAccDataBase     (dAcc);
                    MOT[i].Motor->SetDecDataBase     (dDec);
                    if(sModel=="MC88X1")                                        //Nickliu 20230315 add MCXX8 Set Rate
                    {
                        MOT[i].Motor->SetRate        (atoi(CheckMotorValue(PT,"Rate"           ,true).c_str()));//Nickliu 20230315 add MCXX8 Set Rate
                    }
                    MOT[i].Motor->SetRange           (atoi(CheckMotorValue(PT,"Range"          ,true).c_str()));
                    MOT[i].Motor->bLimitLogic       =(atoi(CheckMotorValue(PT,"LimitLogic"     ,true).c_str())==1)?true:false;
                    MOT[i].Motor->bIn1Logic         =(atoi(CheckMotorValue(PT,"In1Logic"       ,true).c_str())==1)?true:false;
                    MOT[i].SimulateSpeed            = atoi(CheckMotorValue(PT,"SimulateSpeed"  ,true).c_str());
                    SetMotorAccelSpeed(i, 100);
                    MOT[i].HomeFlag                 = 0;

                    if(MOT[i].Mot_Name==MInShuttle1)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh1=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh1=MOT[i].Motor->PJogHighSpeed;
                    }

                    if(MOT[i].Mot_Name==MInShuttle2)                            //kevin 20110531 旋轉shuttle記錄速度
                    {
                        iInitSpeedSh2=MOT[i].Motor->InitSpeed;
                        iPJogHighSpeedSh2=MOT[i].Motor->PJogHighSpeed;
                    }

                    SoftLimit[0]=atoi(CheckMotorValue(PT, "SoftLimitN", true).c_str());
                    SoftLimit[1]=atoi(CheckMotorValue(PT, "SoftLimitP", true).c_str());

                    if(INDEX_MOTION_CARD==0 &&  (i==MTestZ1 || i==MTestZ2))     //Steven 20210623 : Index使用Galil
                        MOT[i].IndexPickLimit=atoi(CheckMotorValue(PT, "PickLimit", true).c_str());

                    if(INDEX_MOTION_CARD==0 && (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))     //Steven 20210623 : Index使用Galil
                        ;                                                       //Steven 20090922 需要修改
                    else
                        MOT[i].SetArmMaxSpeed();

                    MOT[i].Motor->PSoftLimitN=atoi(CheckMotorValue(PT,"SoftLimitN",true).c_str());
                    MOT[i].Motor->PSoftLimitP=atoi(CheckMotorValue(PT,"SoftLimitP",true).c_str());
                    SoftLimit[0]=MOT[i].Motor->PSoftLimitN;
                    SoftLimit[1]=MOT[i].Motor->PSoftLimitP;

                    if(MOT[i].Motor->Enable)
                        MOT[i].Motor->InitMotor(iAdder);
                }
            }
        }
        PT->Close();
    }

    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        if(MOT[i].Motor==NULL)
        {
            MOT[i].Motor=new TMySYNTEKMotor(-1);
            MOT[i].Motor->Enable=false;
            MOT[i].Motor->GearRatio=1.0;
        }

        if(MOT[i].Motor->Enable==true)
        {
            MOT[i].Motor->MotorIdleSafeDoorCheck=IdleCheckSafeDoor;             // 2015.01.15 , Joye , Safe door check
            if(INDEX_MOTION_CARD==0 &&                                          //Steven 20210623 : Index使用Galil
               (i==MTestY1 || i==MTestZ1 || i==MTestZ2 || i==MTestY2))
            {
                ;
            }
            else
            {
                MOT[i].PCIL132_StopMotor();
                //2013-03-27    Dell -----------------
                //InitMotor(0)絕對不可以跟"=new TMySYNTEKMotor((iLane*1000+iIP*10+iPort));"
                //放在同一個迴圈，會造成送出去的pulse count與command count對不起來．至於"為什麼""我在function抬頭解釋
                MOT[i].Motor->InitMotor(0);
                //2013-03-27    Dell -----------------
            }
        }
    }
//    delete PT;
    #ifdef DEBUG_AutoHomeLog
    if(fAllMotorHome==true)
    {
        NewRecordProcess("", "fAllMotorHome", "InitialMotorParameter");
    }
    #endif
    fAllMotorHome=false;

    for(int x=0; x<_MAX_COL_ITEM; x++)                                          //Steven 20221005 : Production Log減少記憶體使用量
    {
        for(int y=0; y<_MAX_ROW_ITEM; y++)
        {
            MOT[MMTrayY         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMPlate1        ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMPlate2        ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto1         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto2         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto3         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto4         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto5         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMAuto6         ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray1    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray2    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray3    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray4    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray5    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MManualTray6    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MOutRotateKit   ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MInRotateKit    ].Tray.PordRec[x][y]=new TMyProductionRecord();
            MOT[MMBulkboxKit    ].Tray.PordRec[x][y]=new TMyProductionRecord(); //kevin 20221005 add error bin box
        }
    }

    fHome->InitialHomeClass();                                                  //Steven 20240603 : 修正歸零馬達名稱消失問題
    BYTE bDevNo_In[4]= {MOT[MInArmPitch  ].Motor->iPortID,
                        MOT[MInArmPitchX2].Motor->iPortID,
                        MOT[MInArmPitchX3].Motor->iPortID,
                        MOT[MInArmPitchX4].Motor->iPortID};
    BYTE bDevNo_Put[4]={MOT[MOutArmPitch  ].Motor->iPortID,
                        MOT[MOutArmPitchX2].Motor->iPortID,
                        MOT[MOutArmPitchX3].Motor->iPortID,
                        MOT[MOutArmPitchX4].Motor->iPortID};

    MOT[MInArmPitch].Motor->SetGroup(1, 4, bDevNo_In);
    MOT[MOutArmPitch].Motor->SetGroup(2, 4, bDevNo_Put);
}
//==============================================================================
void InitialCylinderName()
{
    for(int i=0; i<MaxCylinderItem; i++)
        Cylinder[i].CylinderName="";

    Cylinder[C_TrayZ_Selector       ].CylinderName="C_TrayZ_Selector";
    Cylinder[C_TrayY_Fixer          ].CylinderName="C_TrayY_Fixer";
    Cylinder[C_Auto1Side_Fixer      ].CylinderName="C_Auto1Side_Fixer";
    Cylinder[C_Auto2Side_Fixer      ].CylinderName="C_Auto2Side_Fixer";
    Cylinder[C_Auto3Side_Fixer      ].CylinderName="C_Auto3Side_Fixer";
    Cylinder[C_Auto1_Selector       ].CylinderName="C_Auto1_Selector";
    Cylinder[C_Auto2_Selector       ].CylinderName="C_Auto2_Selector";
    Cylinder[C_Auto3_Selector       ].CylinderName="C_Auto3_Selector";
    Cylinder[C_TrayX_UpDown         ].CylinderName="C_TrayX_UpDown";
    Cylinder[C_EmptyLoaderZ_Select  ].CylinderName="C_EmptyLoaderZ_Select";
    Cylinder[C_ColorLoaderZ_Select  ].CylinderName="C_ColorLoaderZ_Select";
    Cylinder[C_Empty_Fix            ].CylinderName="C_Empty_Fix";
    Cylinder[C_TrayCover            ].CylinderName="C_TrayCover";               //Steven 20140409 : Auto Retest
    Cylinder[C_CatchTray_Fix        ].CylinderName="C_CatchTray_Fix";
    Cylinder[C_Color_Fix            ].CylinderName="C_Color_Fix";
    Cylinder[C_LoaderEdgePush       ].CylinderName="C_LoaderEdgePush";
    Cylinder[C_EmptyEdgePush        ].CylinderName="C_EmptyEdgePush";
    Cylinder[C_ColorEdgePush        ].CylinderName="C_ColorEdgePush";
    Cylinder[C_Auto1EdgePush        ].CylinderName="C_Auto1EdgePush";
    Cylinder[C_Auto2EdgePush        ].CylinderName="C_Auto2EdgePush";
    Cylinder[C_Auto3EdgePush        ].CylinderName="C_Auto3EdgePush";
    Cylinder[C_Load_Up              ].CylinderName="C_Load_Up";
    Cylinder[C_Load_Middle          ].CylinderName="C_Load_Middle";
    Cylinder[C_Color_Up             ].CylinderName="C_Color_Up";
    Cylinder[C_Color_Middle         ].CylinderName="C_Color_Middle";
    Cylinder[C_Empty_Up             ].CylinderName="C_Empty_Up";
    Cylinder[C_Empty_Middle         ].CylinderName="C_Empty_Middle";
    Cylinder[C_TrayVibration        ].CylinderName="C_TrayVibration";
    Cylinder[C_HotplateVibration    ].CylinderName="C_HotplateVibration";
    Cylinder[C_CoolingValve         ].CylinderName="C_CoolingValve";            //20111130  Dell
    Cylinder[C_Auto1_Up             ].CylinderName="C_Auto1_Up";                //Auto 3上頂汽缸
    Cylinder[C_Auto2_Up             ].CylinderName="C_Auto2_Up";                //Auto 2上頂汽缸
    Cylinder[C_Auto3_Up             ].CylinderName="C_Auto3_Up";                //Auto 3上頂汽缸
    Cylinder[C_Auto1LoaderZ_Select  ].CylinderName="C_Auto1LoaderZ_Select";     //Auto 1分離汽缸
    Cylinder[C_Auto2LoaderZ_Select  ].CylinderName="C_Auto2LoaderZ_Select";     //Auto 2分離汽缸
    Cylinder[C_Auto3LoaderZ_Select  ].CylinderName="C_Auto3LoaderZ_Select";     //Auto 3分離汽缸
    Cylinder[C_Fix1LoaderZ_Select   ].CylinderName="C_Fix1LoaderZ_Select";      //Fix 1分離汽缸
    Cylinder[C_Fix2LoaderZ_Select   ].CylinderName="C_Fix2LoaderZ_Select";      //Fix 2分離汽缸
    Cylinder[C_Fix3LoaderZ_Select   ].CylinderName="C_Fix3LoaderZ_Select";      //Fix 3分離汽缸
    Cylinder[C_Auto2_Middle         ].CylinderName="C_Auto2_Middle";
    Cylinder[C_Shuttle_Knocker_1    ].CylinderName="C_Shuttle_Knocker_1";       //Shuttle敲敲
    Cylinder[C_Shuttle_Knocker_2    ].CylinderName="C_Shuttle_Knocker_2";       //Shuttle敲敲
    Cylinder[C_InputRotateKIT       ].CylinderName="C_InputRotateKIT";          //jou 2013-03-01 Rotate kit
    Cylinder[C_OutputRotateKIT      ].CylinderName="C_OutputRotateKIT";         //jou 2013-03-01 Rotate kit
    Cylinder[C_FixTray_FullPlace    ].CylinderName="C_FixTray_FullPlace";       //Steven 20140310 : Fix3滿盤氣缸

    Cylinder[C_DockYAxisOn          ].CylinderName="C_DockYAxisOn";             //Steven 20140310 : One Touch Docking
    Cylinder[C_DockYAxisOff         ].CylinderName="C_DockYAxisOff";            //Steven 20140310 : One Touch Docking
    Cylinder[C_DockXAxisOn          ].CylinderName="C_DockXAxisOn";             //Steven 20140310 : One Touch Docking
    Cylinder[C_DockXAxisOff         ].CylinderName="C_DockXAxisOff";            //Steven 20140310 : One Touch Docking

    Cylinder[C_CatchTray_FixOn      ].CylinderName="C_CatchTray_FixOn";         //ChungHung 20140624 : AutoRetest
    Cylinder[C_CatchTray_FixOff     ].CylinderName="C_CatchTray_FixOff";        //ChungHung 20140624 : AutoRetest
    Cylinder[C_TurnTrayArm          ].CylinderName="C_TurnTrayArm";             //ChungHung 20140701 : AutoRetest
    Cylinder[C_TurnTrayArmLock      ].CylinderName="C_TurnTrayArmLock";         //ChungHung 20140814 : AutoRetest

    Cylinder[C_OCRLight_Up          ].CylinderName="C_OCRLight_Up";             //wei 20150720 OCR觸發
    Cylinder[C_OCRLight_Down        ].CylinderName="C_OCRLight_Down";           //wei 20150720 OCR觸發

    Cylinder[C_SLK1_Clamp           ].CylinderName="C_SLK1_Clamp";              //JerryYang 20160524
    Cylinder[C_SLK1_Unclamp         ].CylinderName="C_SLK1_Unclamp";            //JerryYang 20160524
    Cylinder[C_SLK2_Clamp           ].CylinderName="C_SLK2_Clamp";              //JerryYang 20160524
    Cylinder[C_SLK2_Unclamp         ].CylinderName="C_SLK2_Unclamp";            //JerryYang 20160524
    Cylinder[C_Socket_Clamp         ].CylinderName="C_Socket_Clamp";            //JerryYang 20160606
    Cylinder[C_Socket_Unclamp       ].CylinderName="C_Socket_Unclamp";          //JerryYang 20160606

    Cylinder[C_LoaderUpPress        ].CylinderName="C_LoaderUpPress";           //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
    Cylinder[C_Auto1UpPress         ].CylinderName="C_Auto1UpPress";            //JerryYang 20190423 新增unloader壓tray
    Cylinder[C_Auto2UpPress         ].CylinderName="C_Auto2UpPress";
    Cylinder[C_Auto3UpPress         ].CylinderName="C_Auto3UpPress";

    Cylinder[C_HingeLookOn          ].CylinderName="C_HingeLookOn";             //wei 20170418 Hinge cassette
    Cylinder[C_HingeLookOff         ].CylinderName="C_HingeLookOff";            //wei 20170418 Hinge cassette

    Cylinder[C_CassetteArmCatchOn   ].CylinderName="C_CassetteArmCatchOn";      //wei 20180702 MR
    Cylinder[C_CassetteArmCatchOff  ].CylinderName="C_CassetteArmCatchOff";     //wei 20180702 MR
    Cylinder[C_LoadPortYOn          ].CylinderName="C_LoadPortYOn";             //wei 20180702 MR
    Cylinder[C_LoadPortYOff         ].CylinderName="C_LoadPortYOff";            //wei 20180702 MR
    Cylinder[C_LoadPortCatchOn      ].CylinderName="C_LoadPortCatchOn";         //wei 20180702 MR
    Cylinder[C_LoadPortCatchOff     ].CylinderName="C_LoadPortCatchOff";        //wei 20180702 MR
    Cylinder[C_TrayBracketUpOn      ].CylinderName="C_TrayBracketUpOn";         //wei 20180702 MR
    Cylinder[C_TrayBracketUpOff     ].CylinderName="C_TrayBracketUpOff";        //wei 20180702 MR
    Cylinder[C_TrayBracketOpenOn    ].CylinderName="C_TrayBracketOpenOn";       //wei 20180702 MR
    Cylinder[C_TrayBracketOpenOff   ].CylinderName="C_TrayBracketOpenOff";      //wei 20180702 MR
    Cylinder[C_StackedTrayCatchOn   ].CylinderName="C_StackedTrayCatchOn";      //wei 20180702 MR
    Cylinder[C_StackedTrayCatchOff  ].CylinderName="C_StackedTrayCatchOff";     //wei 20180702 MR
    Cylinder[C_StackedTrayLockOn    ].CylinderName="C_StackedTrayLockOn";       //wei 20180702 MR
    Cylinder[C_StackedTrayLockOff   ].CylinderName="C_StackedTrayLockOff";      //wei 20180702 MR
    Cylinder[C_LoadRobotX           ].CylinderName="C_LoadRobotX";              //Sam 20190112 LM
    Cylinder[C_UnloadRobotX         ].CylinderName="C_UnloadRobotX";

    Cylinder[C_InFlipper1           ].CylinderName="C_InFlipper1";              //Frank 20210612 : Flipper Function
    Cylinder[C_InFlipper1Lock       ].CylinderName="C_InFlipper1Lock";
    Cylinder[C_InFlipper2           ].CylinderName="C_InFlipper2";
    Cylinder[C_InFlipper2Lock       ].CylinderName="C_InFlipper2Lock";
    Cylinder[C_InFlipper3           ].CylinderName="C_InFlipper3";
    Cylinder[C_InFlipper3Lock       ].CylinderName="C_InFlipper3Lock";
    Cylinder[C_OutFlipper1          ].CylinderName="C_OutFlipper1";
    Cylinder[C_OutFlipper1Lock      ].CylinderName="C_OutFlipper1Lock";
    Cylinder[C_OutFlipper2          ].CylinderName="C_OutFlipper2";
    Cylinder[C_OutFlipper2Lock      ].CylinderName="C_OutFlipper2Lock";
    Cylinder[C_OutFlipper3          ].CylinderName="C_OutFlipper3";
    Cylinder[C_OutFlipper3Lock      ].CylinderName="C_OutFlipper3Lock";

    Cylinder[C_LoaderCasstteLock    ].CylinderName="C_LoaderCasstteLock";
    Cylinder[C_EmptyCasstteLock     ].CylinderName="C_EmptyCasstteLock";
    Cylinder[C_ColorCasstteLock     ].CylinderName="C_ColorCasstteLock";
    Cylinder[C_Auto1CasstteLock     ].CylinderName="C_Auto1CasstteLock";
    Cylinder[C_Auto2CasstteLock     ].CylinderName="C_Auto2CasstteLock";
    Cylinder[C_Auto3CasstteLock     ].CylinderName="C_Auto3CasstteLock";

    Cylinder[C_PlacementArm         ].CylinderName="C_PlacementArm";            //JimmyChiu 20220908 add Pickup Error Placement
    Cylinder[C_InAreaAlignment      ].CylinderName="C_InAreaAlignment";         //ChungHung 20210113 add for Alignment CCD
    Cylinder[C_OutAreaAlignment     ].CylinderName="C_OutAreaAlignment";        //ChungHung 20210113 add for Alignment CCD
    Cylinder[C_TesterSidePush       ].CylinderName="C_TesterSidePush";          //Richard 20220321 : 渠梁Side Push

    Cylinder[C_LoadTrackFloodgate   ].CylinderName="C_LoadTrackFloodgate";      //Ztex 2023.04.13 Add HT-1032 IO
    Cylinder[C_EmptyTrackFloodgate  ].CylinderName="C_EmptyTrackFloodgate";
    Cylinder[C_ColorTrackFloodgate  ].CylinderName="C_ColorTrackFloodgate";
    Cylinder[C_Auto1TrackFloodgate  ].CylinderName="C_Auto1TrackFloodgate";
    Cylinder[C_Auto2TrackFloodgate  ].CylinderName="C_Auto2TrackFloodgate";
    Cylinder[C_Auto3TrackFloodgate  ].CylinderName="C_Auto3TrackFloodgate";
    Cylinder[C_SafeDoor1Lock        ].CylinderName="C_SafeDoor1Lock";
    Cylinder[C_SafeDoor2Lock        ].CylinderName="C_SafeDoor2Lock";
    Cylinder[C_SafeDoor3Lock        ].CylinderName="C_SafeDoor3Lock";
    Cylinder[C_SafeDoor4Lock        ].CylinderName="C_SafeDoor4Lock";
    Cylinder[C_SafeDoor5Lock        ].CylinderName="C_SafeDoor5Lock";
    Cylinder[C_SafeDoor6Lock        ].CylinderName="C_SafeDoor6Lock";
    Cylinder[C_SafeDoor7Lock        ].CylinderName="C_SafeDoor7Lock";
    Cylinder[C_SafeDoor8Lock        ].CylinderName="C_SafeDoor8Lock";
    Cylinder[C_Shuttle1Floodgate    ].CylinderName="C_Shuttle1Floodgate";
    Cylinder[C_Shuttle2Floodgate    ].CylinderName="C_Shuttle2Floodgate";
    Cylinder[C_OutShuttle1Floodgate ].CylinderName="C_OutShuttle1Floodgate";    //Ifor 20240620 add:Out Shuttle Floodgate
    Cylinder[C_OutShuttle2Floodgate ].CylinderName="C_OutShuttle2Floodgate";    //Ifor 20240620 add:Out Shuttle Floodgate

    Cylinder[C_LoaderPushBack_Push  ].CylinderName="C_LoaderPushBack_Push";
    Cylinder[C_EmptyPushBack_Push   ].CylinderName="C_EmptyPushBack_Push";
    Cylinder[C_ColorPushBack_Push   ].CylinderName="C_ColorPushBack_Push";
    Cylinder[C_Auto1PushBack_Push   ].CylinderName="C_Auto1PushBack_Push";
    Cylinder[C_Auto2PushBack_Push   ].CylinderName="C_Auto2PushBack_Push";
    Cylinder[C_Auto3PushBack_Push   ].CylinderName="C_Auto3PushBack_Push";
    Cylinder[C_LoaderSeparate       ].CylinderName="C_LoaderSeparate";
    Cylinder[C_EmptySeparate        ].CylinderName="C_EmptySeparate";
    Cylinder[C_ColorSeparate        ].CylinderName="C_ColorSeparate";
    Cylinder[C_Auto1Separate        ].CylinderName="C_Auto1Separate";
    Cylinder[C_Auto2Separate        ].CylinderName="C_Auto2Separate";
    Cylinder[C_Auto3Separate        ].CylinderName="C_Auto3Separate";

    Cylinder[C_UnderTrayArmYCatch   ].CylinderName="C_UnderTrayArmYCatch";

    Cylinder[C_EnhaustAirVentOpen   ].CylinderName="C_EnhaustAirVentOpen";      //Ztex 2023.04.26 Add HT-1032 IO Exhaust Air
    Cylinder[C_EnhaustAirVentClose  ].CylinderName="C_EnhaustAirVentClose";
    Cylinder[C_LUpEnhaustAirOpen    ].CylinderName="C_LUpEnhaustAirOpen";
    Cylinder[C_LUpEnhaustAirClose   ].CylinderName="C_LUpEnhaustAirClose";
    Cylinder[C_RUpEnhaustAirOpen    ].CylinderName="C_RUpEnhaustAirOpen";
    Cylinder[C_RUpEnhaustAirClose   ].CylinderName="C_RUpEnhaustAirClose";

    Cylinder[C_CatchMagazineTray    ].CylinderName="C_CatchMagazineTray";       //JerryYang 20221215 : add Magazine
    Cylinder[C_CatchMagazineTray1   ].CylinderName="C_CatchMagazineTray1";
    Cylinder[C_MagYTrayOut          ].CylinderName="C_MagYTrayOut";
    Cylinder[C_Auto3BlockZ          ].CylinderName="C_Auto3BlockZ";
    Cylinder[C_TrayXFloodgate1      ].CylinderName="C_TrayXFloodgate1";
    Cylinder[C_TrayXFloodgate2      ].CylinderName="C_TrayXFloodgate2";
    Cylinder[C_TrayXFloodgate3      ].CylinderName="C_TrayXFloodgate3";
    Cylinder[C_TrayXFloodgate4      ].CylinderName="C_TrayXFloodgate4";
    Cylinder[C_FixTray_UpDown       ].CylinderName="C_FixTray_UpDown";
    Cylinder[C_LoaderPushBack_Back  ].CylinderName="C_LoaderPushBack_Back";
    Cylinder[C_EmptyPushBack_Back   ].CylinderName="C_EmptyPushBack_Back";
    Cylinder[C_ColorPushBack_Back   ].CylinderName="C_ColorPushBack_Back";
    Cylinder[C_Auto1PushBack_Back   ].CylinderName="C_Auto1PushBack_Back";
    Cylinder[C_Auto2PushBack_Back   ].CylinderName="C_Auto2PushBack_Back";
    Cylinder[C_Auto3PushBack_Back   ].CylinderName="C_Auto3PushBack_Back";

    Cylinder[C_Auto4Side_Fixer      ].CylinderName="C_Auto4Side_Fixer";         //Steven 20230907 : For HT-9011UC
    Cylinder[C_Auto5Side_Fixer      ].CylinderName="C_Auto5Side_Fixer";
    Cylinder[C_Auto6Side_Fixer      ].CylinderName="C_Auto6Side_Fixer";
    Cylinder[C_Auto4_Selector       ].CylinderName="C_Auto4_Selector";
    Cylinder[C_Auto5_Selector       ].CylinderName="C_Auto5_Selector";
    Cylinder[C_Auto6_Selector       ].CylinderName="C_Auto6_Selector";
    Cylinder[C_Auto4EdgePush        ].CylinderName="C_Auto4EdgePush";
    Cylinder[C_Auto5EdgePush        ].CylinderName="C_Auto5EdgePush";
    Cylinder[C_Auto6EdgePush        ].CylinderName="C_Auto6EdgePush";
    Cylinder[C_Auto4_Up             ].CylinderName="C_Auto4_Up";
    Cylinder[C_Auto5_Up             ].CylinderName="C_Auto5_Up";
    Cylinder[C_Auto6_Up             ].CylinderName="C_Auto6_Up";
    Cylinder[C_Auto4LoaderZ_Select  ].CylinderName="C_Auto4LoaderZ_Select";
    Cylinder[C_Auto5LoaderZ_Select  ].CylinderName="C_Auto5LoaderZ_Select";
    Cylinder[C_Auto6LoaderZ_Select  ].CylinderName="C_Auto6LoaderZ_Select";
    Cylinder[C_Fix4LoaderZ_Select   ].CylinderName="C_Fix4LoaderZ_Select";
    Cylinder[C_Fix5LoaderZ_Select   ].CylinderName="C_Fix5LoaderZ_Select";
    Cylinder[C_Fix6LoaderZ_Select   ].CylinderName="C_Fix6LoaderZ_Select";
    Cylinder[C_Auto4UpPress         ].CylinderName="C_Auto4UpPress";
    Cylinder[C_Auto5UpPress         ].CylinderName="C_Auto5UpPress";
    Cylinder[C_Auto6UpPress         ].CylinderName="C_Auto6UpPress";
    Cylinder[C_Auto4CasstteLock     ].CylinderName="C_Auto4CasstteLock";
    Cylinder[C_Auto5CasstteLock     ].CylinderName="C_Auto5CasstteLock";
    Cylinder[C_Auto6CasstteLock     ].CylinderName="C_Auto6CasstteLock";
    Cylinder[C_Auto4TrackFloodgate  ].CylinderName="C_Auto4TrackFloodgate";
    Cylinder[C_Auto5TrackFloodgate  ].CylinderName="C_Auto5TrackFloodgate";
    Cylinder[C_Auto6TrackFloodgate  ].CylinderName="C_Auto6TrackFloodgate";
    Cylinder[C_Auto4PushBack_Push   ].CylinderName="C_Auto4PushBack_Push";
    Cylinder[C_Auto5PushBack_Push   ].CylinderName="C_Auto5PushBack_Push";
    Cylinder[C_Auto6PushBack_Push   ].CylinderName="C_Auto6PushBack_Push";
    Cylinder[C_Auto4Separate        ].CylinderName="C_Auto4Separate";
    Cylinder[C_Auto5Separate        ].CylinderName="C_Auto5Separate";
    Cylinder[C_Auto6Separate        ].CylinderName="C_Auto6Separate";
    Cylinder[C_Auto4PushBack_Back   ].CylinderName="C_Auto4PushBack_Back";
    Cylinder[C_Auto5PushBack_Back   ].CylinderName="C_Auto5PushBack_Back";
    Cylinder[C_Auto6PushBack_Back   ].CylinderName="C_Auto6PushBack_Back";

    Cylinder[C_LoaderSeparateRL   ].CylinderName="C_LoaderSeparateRL";
    Cylinder[C_LoaderSeparateRR   ].CylinderName="C_LoaderSeparateRR";
    Cylinder[C_LoaderSeparateFL   ].CylinderName="C_LoaderSeparateFL";
    Cylinder[C_LoaderSeparateFR   ].CylinderName="C_LoaderSeparateFR";
    Cylinder[C_EmptySeparateRL    ].CylinderName="C_EmptySeparateRL";
    Cylinder[C_EmptySeparateRR    ].CylinderName="C_EmptySeparateRR";
    Cylinder[C_EmptySeparateFL    ].CylinderName="C_EmptySeparateFL";
    Cylinder[C_EmptySeparateFR    ].CylinderName="C_EmptySeparateFR";
    Cylinder[C_ColorSeparateRL    ].CylinderName="C_ColorSeparateRL";
    Cylinder[C_ColorSeparateRR    ].CylinderName="C_ColorSeparateRR";
    Cylinder[C_ColorSeparateFL    ].CylinderName="C_ColorSeparateFL";
    Cylinder[C_ColorSeparateFR    ].CylinderName="C_ColorSeparateFR";
    Cylinder[C_Auto1SeparateRL    ].CylinderName="C_Auto1SeparateRL";
    Cylinder[C_Auto1SeparateRR    ].CylinderName="C_Auto1SeparateRR";
    Cylinder[C_Auto1SeparateFL    ].CylinderName="C_Auto1SeparateFL";
    Cylinder[C_Auto1SeparateFR    ].CylinderName="C_Auto1SeparateFR";
    Cylinder[C_Auto2SeparateRL    ].CylinderName="C_Auto2SeparateRL";
    Cylinder[C_Auto2SeparateRR    ].CylinderName="C_Auto2SeparateRR";
    Cylinder[C_Auto2SeparateFL    ].CylinderName="C_Auto2SeparateFL";
    Cylinder[C_Auto2SeparateFR    ].CylinderName="C_Auto2SeparateFR";
    Cylinder[C_Auto3SeparateRL    ].CylinderName="C_Auto3SeparateRL";
    Cylinder[C_Auto3SeparateRR    ].CylinderName="C_Auto3SeparateRR";
    Cylinder[C_Auto3SeparateFL    ].CylinderName="C_Auto3SeparateFL";
    Cylinder[C_Auto3SeparateFR    ].CylinderName="C_Auto3SeparateFR";
    Cylinder[C_Auto4SeparateRL    ].CylinderName="C_Auto4SeparateRL";
    Cylinder[C_Auto4SeparateRR    ].CylinderName="C_Auto4SeparateRR";
    Cylinder[C_Auto4SeparateFL    ].CylinderName="C_Auto4SeparateFL";
    Cylinder[C_Auto4SeparateFR    ].CylinderName="C_Auto4SeparateFR";
    Cylinder[C_Auto5SeparateRL    ].CylinderName="C_Auto5SeparateRL";
    Cylinder[C_Auto5SeparateRR    ].CylinderName="C_Auto5SeparateRR";
    Cylinder[C_Auto5SeparateFL    ].CylinderName="C_Auto5SeparateFL";
    Cylinder[C_Auto5SeparateFR    ].CylinderName="C_Auto5SeparateFR";
    Cylinder[C_Auto6SeparateRL    ].CylinderName="C_Auto6SeparateRL";
    Cylinder[C_Auto6SeparateRR    ].CylinderName="C_Auto6SeparateRR";
    Cylinder[C_Auto6SeparateFL    ].CylinderName="C_Auto6SeparateFL";
    Cylinder[C_Auto6SeparateFR    ].CylinderName="C_Auto6SeparateFR";

    Cylinder[C_Load2CasstteLock   ].CylinderName="C_Load2CasstteLock";          //Steven 20240822 : For HT-9046AU
    Cylinder[C_Load2_Middle       ].CylinderName="C_Load2_Middle";
    Cylinder[C_Load2_Up           ].CylinderName="C_Load2_Up";
    Cylinder[C_Tray2Z_Selector    ].CylinderName="C_Tray2Z_Selector";
    Cylinder[C_Load2UpPress       ].CylinderName="C_Load2UpPress";
    Cylinder[C_Tray2Y_Fixer       ].CylinderName="C_Tray2Y_Fixer";
    Cylinder[C_Load2EdgePush      ].CylinderName="C_Load2EdgePush";
    Cylinder[C_Load2TrackFloodgate].CylinderName="C_Load2TrackFloodgate";
    Cylinder[C_Load2PushBack_Push ].CylinderName="C_Load2PushBack_Push";
    Cylinder[C_Load2PushBack_Back ].CylinderName="C_Load2PushBack_Back";
    Cylinder[C_Load2Separate      ].CylinderName="C_Load2Separate";
    Cylinder[C_Load2SeparateRL    ].CylinderName="C_Load2SeparateRL";
    Cylinder[C_Load2SeparateRR    ].CylinderName="C_Load2SeparateRR";
    Cylinder[C_Load2SeparateFL    ].CylinderName="C_Load2SeparateFL";
    Cylinder[C_Load2SeparateFR    ].CylinderName="C_Load2SeparateFR";

    Cylinder[C_FixedSeatTL          ].CylinderName="C_FixedSeatTL";
    Cylinder[C_FixedSeatTR          ].CylinderName="C_FixedSeatTR";
    Cylinder[C_FixedSeatBL          ].CylinderName="C_FixedSeatBL";
    Cylinder[C_FixedSeatBR          ].CylinderName="C_FixedSeatBR";
    Cylinder[C_TopBtmRotateLock     ].CylinderName="C_TopBtmRotateLock";

    Cylinder[C_LoadCarRFIDRotArmD ].CylinderName="C_LoadCarRFIDRotArmD";        //RogerYang 20250828 add for Loader Rotate Arm
    Cylinder[C_LoadCarRFIDRotArmU ].CylinderName="C_LoadCarRFIDRotArmU";        //RogerYang 20250828 add for Loader Rotate Arm
    Cylinder[C_LoadTrayDetD       ].CylinderName="C_LoadTrayDetD";              //RogerYang 20250828 add for 殘料檢氣缸
    Cylinder[C_LoadTrayDetU       ].CylinderName="C_LoadTrayDetU";              //RogerYang 20250828 add for 殘料檢氣缸
    Cylinder[C_LoadTrayDetF       ].CylinderName="C_LoadTrayDetF";              //RogerYang 20250828 add for 殘料檢氣缸
    Cylinder[C_LoadTrayDetB       ].CylinderName="C_LoadTrayDetB";              //RogerYang 20250828 add for 殘料檢氣缸

    Cylinder[C_LoaderCarrier      ].CylinderName="C_LoaderCarrier";             //Ifor 20251216 add:Boat Carrier
    Cylinder[C_Auto1Carrier       ].CylinderName="C_Auto1Carrier";              //Ifor 20251216 add:Boat Carrier
    Cylinder[C_Auto2Carrier       ].CylinderName="C_Auto2Carrier";              //Ifor 20251216 add:Boat Carrier
}
//------------------------------------------------------------------------------
void InitCylinder()
{
    AnsiString S, sDBDir;
    bool flag;
    int iPort=0, iIP=0;
    int iCy=0, iCyOn=0, iCyOff=0;
    AnsiString str;
    bool bHasCyn, bHasOnSensor, bHasOffSensor;
    InitialCylinderName();

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
    {
        for(int i=0; i<MaxCylinderItem; i++)
        {
            bHasCyn=false;
            bHasOnSensor=false;
            bHasOffSensor=false;
            Cylinder[i].Enable=false;
            if(Cylinder[i].CylinderName=="")
                continue;

            Cylinder[i].OnSensorName=Cylinder[i].CylinderName+"_On";
            Cylinder[i].OffSensorName=Cylinder[i].CylinderName+"_Off";

            HSys.mapIOTableIter=HSys.mapIOTable.find(Cylinder[i].CylinderName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasCyn=true;
                iCy=atoi(HSys.mapIOTable[Cylinder[i].CylinderName].c_str());
            }
            else
            {
                bHasCyn=false;
                str.sprintf("Can not find cylinder %s", Cylinder[i].CylinderName);
            }

            HSys.mapIOTableIter=HSys.mapIOTable.find(Cylinder[i].OnSensorName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasOnSensor=true;
                iCyOn=atoi(HSys.mapIOTable[Cylinder[i].OnSensorName].c_str());
            }
            else
            {
                bHasOnSensor=false;
                str.sprintf("Can not find cylinder on %s", Cylinder[i].OnSensorName);
            }

            HSys.mapIOTableIter=HSys.mapIOTable.find(Cylinder[i].OffSensorName);
            if(HSys.mapIOTableIter!=HSys.mapIOTable.end())
            {
                bHasOffSensor=true;
                iCyOff=atoi(HSys.mapIOTable[Cylinder[i].OffSensorName].c_str());
            }
            else
            {
                bHasOffSensor=false;
                str.sprintf("Can not find cylinder off %s", Cylinder[i].OffSensorName);
            }

            if(bHasOnSensor)
            {
                Cylinder[i].OnSenRingUse    =(HSys.IOTable[iCyOn]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iCyOn]->iPort);
                Cylinder[i].OnSenRing       =HSys.IOTable[iCyOn]->iLane;
                Cylinder[i].OnSenIP         =HSys.IOTable[iCyOn]->iIP;
                Cylinder[i].OnSenPort       =HSys.IOTable[iCyOn]->iPort;
                Cylinder[i].OnSenBit        =HSys.IOTable[iCyOn]->iBit;
                Cylinder[i].OnSenType       =HSys.IOTable[iCyOn]->iInType;
                Cylinder[i].OnSenISABase    =HSys.IOTable[iCyOn]->iISABase;
                Cylinder[i].ISABase         =Cylinder[i].OnSenISABase;
                if((Cylinder[i].ISABase==eMotionNet ||
                    Cylinder[i].ISABase==ePCI1203) &&
                   (Cylinder[i].OnSenRing==0 && Cylinder[i].OnSenIP==0 && Cylinder[i].OnSenPort==0 && Cylinder[i].OnSenBit==0) ||    //Steven 20200901 : 修正氣缸Sensor的enable設定
                    Cylinder[i].OnSenRing<0 || Cylinder[i].OnSenIP<0 || Cylinder[i].OnSenPort<0 || Cylinder[i].OnSenBit<0)
                {
                    Cylinder[i].OnSenEnable =false;
                }
                else if((Cylinder[i].ISABase==eISABase ||
                         Cylinder[i].ISABase==ePCI1735U ||
                         Cylinder[i].ISABase==ePLCbase) &&
                        (Cylinder[i].OnSenPort==0 && Cylinder[i].OnSenBit==0) ||    //Steven 20200901 : 修正氣缸Sensor的enable設定
                         Cylinder[i].OnSenPort<0 || Cylinder[i].OnSenBit<0)
                {
                    Cylinder[i].OnSenEnable =false;
                }
                else
                {
                    Cylinder[i].OnSenEnable =(HSys.IOTable[iCyOn]->iEnable==1);
                }
            }
            else
            {
                Cylinder[i].OnSenEnable=false;
            }

            if(bHasOffSensor)
            {
                Cylinder[i].OffSenRingUse   =(HSys.IOTable[iCyOff]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iCyOff]->iPort);
                Cylinder[i].OffSenRing      =HSys.IOTable[iCyOff]->iLane;
                Cylinder[i].OffSenIP        =HSys.IOTable[iCyOff]->iIP;
                Cylinder[i].OffSenPort      =HSys.IOTable[iCyOff]->iPort;
                Cylinder[i].OffSenBit       =HSys.IOTable[iCyOff]->iBit;
                Cylinder[i].OffSenType      =HSys.IOTable[iCyOff]->iInType;
                Cylinder[i].OffSenISABase   =HSys.IOTable[iCyOff]->iISABase;
                Cylinder[i].ISABase         =Cylinder[i].OffSenISABase;
                if((Cylinder[i].ISABase==eMotionNet ||
                    Cylinder[i].ISABase==ePCI1203) &&
                   (Cylinder[i].OffSenRing==0 && Cylinder[i].OffSenIP==0 && Cylinder[i].OffSenPort==0 && Cylinder[i].OffSenBit==0) ||    //Steven 20200901 : 修正氣缸Sensor的enable設定
                    Cylinder[i].OffSenRing<0 || Cylinder[i].OffSenIP<0 || Cylinder[i].OffSenPort<0 || Cylinder[i].OffSenBit<0)
                {
                    Cylinder[i].OffSenEnable=false;
                }
                else if((Cylinder[i].ISABase==eISABase ||
                         Cylinder[i].ISABase==ePCI1735U ||
                         Cylinder[i].ISABase==ePLCbase) &&
                        (Cylinder[i].OffSenPort==0 && Cylinder[i].OffSenBit==0) ||    //Steven 20200901 : 修正氣缸Sensor的enable設定
                         Cylinder[i].OffSenPort<0 || Cylinder[i].OffSenBit<0)
                {
                    Cylinder[i].OffSenEnable=false;
                }
                else
                {
                    Cylinder[i].OffSenEnable=(HSys.IOTable[iCyOff]->iEnable==1);
                }
            }
            else
            {
                Cylinder[i].OffSenEnable=false;
            }

            if(bHasCyn)
            {
                Cylinder[i].OutRingUse      =(HSys.IOTable[iCy]->iPort==-1)?AnsiString(""):AnsiString(HSys.IOTable[iCy]->iPort);
                Cylinder[i].OutRing         =HSys.IOTable[iCy]->iLane;
                Cylinder[i].OutIP           =HSys.IOTable[iCy]->iIP;
                Cylinder[i].OutPort         =HSys.IOTable[iCy]->iPort;
                Cylinder[i].OutBit          =HSys.IOTable[iCy]->iBit;
                Cylinder[i].OutType         =HSys.IOTable[iCy]->iInType;
                Cylinder[i].OutISABase      =HSys.IOTable[iCy]->iISABase;
                Cylinder[i].ISABase         =Cylinder[i].OutISABase;
            }
            else
            {
                 Cylinder[i].Enable=false;
            }

            Cylinder[i].OnAlarmCode  =31000+i;                                  //Steven 20231127 : 氣缸Alarm改成自動生成, 分類31
            Cylinder[i].OffAlarmCode =31000+i;
            if(bHasCyn)
            {
                Cylinder[i].OnAlarmTime  =HSys.IOTable[iCy]->iOnAlarmTime;
                Cylinder[i].OffAlarmTime =HSys.IOTable[iCy]->iOffAlarmTime;
                Cylinder[i].OnDelayTime  =HSys.IOTable[iCy]->iOnDelayTime;
                Cylinder[i].OffDelayTime =HSys.IOTable[iCy]->iOffDelayTime;
            }

            if(Cylinder[i].OnAlarmTime<=0)      Cylinder[i].OnAlarmTime=10;
            if(Cylinder[i].OffAlarmTime<=0)     Cylinder[i].OffAlarmTime=10;
            if(Cylinder[i].OnDelayTime<=0)      Cylinder[i].OnDelayTime=1;
            if(Cylinder[i].OffDelayTime<=0)     Cylinder[i].OffDelayTime=1;

            #ifdef SOFT_SIMULTE
                Cylinder[i].Enable      =false;                                 //Steven 20230723 : 修正Cylinder Enable判斷方式
                Cylinder[i].OnSenEnable =false;
                Cylinder[i].OffSenEnable=false;
            #else
            if(bHasCyn)
            {
                if(HSys.IOTable[iCy]->iEnable==1 && Cylinder[i].OutRingUse!="")
                    Cylinder[i].Enable=true;

                #ifndef SOFT_SIMULTE
                if(Cylinder[i].Enable)
                #endif
                    MyLaneIO.SetUseIP(Cylinder[i].OutRing, Cylinder[i].OutIP, Cylinder[i].ISABase, Cylinder[i].CylinderName);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復

                #ifndef SOFT_SIMULTE
                if(Cylinder[i].OnSenEnable)
                #endif
                    MyLaneIO.SetUseIP(Cylinder[i].OnSenRing, Cylinder[i].OnSenIP, Cylinder[i].OnSenISABase, Cylinder[i].CylinderName, false);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復

                #ifndef SOFT_SIMULTE
                if(Cylinder[i].OffSenEnable)
                #endif
                    MyLaneIO.SetUseIP(Cylinder[i].OffSenRing, Cylinder[i].OffSenIP, Cylinder[i].OffSenISABase, Cylinder[i].CylinderName, false);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }
            #endif
        }
    }
    else
    {
        TTable *T;
        if(USE_AUTO_RETEST==eartInstall)                                        //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
        {
            sDBDir=CurrentDir+"\\system\\cylinder_AutoRetest.db";               //JerryYang 20150903 檢查Database檔是否存在
            if(FileExists(sDBDir))
            {
                DataModule1->CylinderTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("cylinder_AutoRetest.db does not exist");
                return;
            }
        }
        else
        {
            sDBDir=CurrentDir+"\\system\\cylinder.db";
            if(FileExists(sDBDir))
            {
                DataModule1->CylinderTable->TableName=sDBDir;
            }
            else
            {
                ShowMyMessage("cylinder.db does not exist");
                return;
            }
        }

        T=DataModule1->CylinderTable;
        T->Active=true;
        for(int i=0; i<MaxCylinderItem; i++)
        {
            Cylinder[i].Enable=false;
            if(Cylinder[i].CylinderName=="")
                continue;
            T->First();
            flag=false;
            while(!T->Eof)
            {
                if(Cylinder[i].CylinderName==T->FieldByName("CylinderName")->AsString)
                {
                    flag=true;
                    break;
                }
                T->Next();
            }
            if(flag==false)
            {
                str.sprintf("Can not find cylinder %s", Cylinder[i].CylinderName);
                continue;
            }
            if(T->FindField("ISABase")!=NULL)                                   //Nickliu 20230309 add Cylinder ISABase Type
                Cylinder[i].ISABase     =atoi(T->FieldByName("ISABase")->AsString.c_str());
            else
                Cylinder[i].ISABase     =eMotionNet;
            Cylinder[i].OutIP           =atoi(T->FieldByName("OutIP")->AsString.c_str());
            if(Cylinder[i].ISABase==eISABase ||                                 //Nickliu 20230309 add Cylinder ISABase Type
               Cylinder[i].ISABase==ePCI1735U ||                                //Alick 20160809 modify for PIC-1735U
               Cylinder[i].ISABase==ePLCbase)
            {
                Cylinder[i].OutRingUse  =T->FieldByName("OutIOPort")->AsString.c_str();
                Cylinder[i].OutPort     =HexStrToInt(T->FieldByName("OutIOPort")->AsString.c_str());
            }
            else
            {
                Cylinder[i].OutRingUse  =T->FieldByName("OutLane")->AsString.c_str();
                if(Cylinder[i].OutRingUse!="")
                    Cylinder[i].OutRing =atoi(Cylinder[i].OutRingUse.c_str());
                Cylinder[i].OutPort     =atoi(T->FieldByName("OutIOPort")->AsString.c_str());
            }

            Cylinder[i].OutBit          =atoi(T->FieldByName("OutIOBit")->AsString.c_str());
            Cylinder[i].OutType         =atoi(T->FieldByName("OutType")->AsString.c_str());

            Cylinder[i].OnSenIP         =atoi(T->FieldByName("OnSenIP")->AsString.c_str());
            if(Cylinder[i].ISABase==eISABase ||                                 //Nickliu 20230309 add Cylinder ISABase Type
               Cylinder[i].ISABase==ePCI1735U ||                                //Alick 20160809 modify for PIC-1735U
               Cylinder[i].ISABase==ePLCbase)
            {
                Cylinder[i].OnSenRingUse=T->FieldByName("OnSenPort")->AsString.c_str();
                Cylinder[i].OnSenPort   =HexStrToInt(T->FieldByName("OnSenPort")->AsString.c_str());
            }
            else
            {
                Cylinder[i].OnSenRingUse=T->FieldByName("OnSenLane")->AsString.c_str();
                if(Cylinder[i].OnSenRingUse!="")
                    Cylinder[i].OnSenRing=atoi(Cylinder[i].OnSenRingUse.c_str());
                Cylinder[i].OnSenPort   =atoi(T->FieldByName("OnSenPort")->AsString.c_str());
            }

            Cylinder[i].OnSenBit        =atoi(T->FieldByName("OnSenBit")->AsString.c_str());
            Cylinder[i].OnSenType       =atoi(T->FieldByName("OnSenType")->AsString.c_str());

            Cylinder[i].OffSenIP        =atoi(T->FieldByName("OffSenIP")->AsString.c_str());
            if(Cylinder[i].ISABase==eISABase ||                                 //Nickliu 20230309 add Cylinder ISABase Type
               Cylinder[i].ISABase==ePCI1735U ||                                //Alick 20160809 modify for PIC-1735U
               Cylinder[i].ISABase==ePLCbase)
            {
                Cylinder[i].OffSenRingUse=T->FieldByName("OffSenPort")->AsString.c_str();
                Cylinder[i].OffSenPort   =HexStrToInt(T->FieldByName("OffSenPort")->AsString.c_str());
            }
            else
            {
                Cylinder[i].OffSenRingUse=T->FieldByName("OffSenLane")->AsString.c_str();
                if(Cylinder[i].OffSenRingUse!="")
                    Cylinder[i].OffSenRing=atoi(Cylinder[i].OffSenRingUse.c_str());
                Cylinder[i].OffSenPort   =atoi(T->FieldByName("OffSenPort")->AsString.c_str());
            }

            Cylinder[i].OffSenBit        =atoi(T->FieldByName("OffSenBit")->AsString.c_str());
            Cylinder[i].OffSenType       =atoi(T->FieldByName("OffSenType")->AsString.c_str());

            S=T->FieldByName("OnAlarmCode")->AsString;
            S=S.SubString(4,4);
            Cylinder[i].OnAlarmCode  =31000+i;                                  //Steven 20231127 : 氣缸Alarm改成自動生成, 分類31
            Cylinder[i].OffAlarmCode =31000+i;
            Cylinder[i].OnAlarmTime  =atoi(T->FieldByName("OnAlarmTime")->AsString.c_str());
            Cylinder[i].OffAlarmTime =atoi(T->FieldByName("OffAlarmTime")->AsString.c_str());
            Cylinder[i].OnDelayTime  =atoi(T->FieldByName("OnDelayTime")->AsString.c_str());
            Cylinder[i].OffDelayTime =atoi(T->FieldByName("OffDelayTime")->AsString.c_str());

            #ifdef SOFT_SIMULTE
                Cylinder[i].Enable=false;
                Cylinder[i].OnSenEnable =false;                                 //Steven 20230723 : 修正Cylinder Enable判斷方式
                Cylinder[i].OffSenEnable=false;
            #else
            if(atoi(T->FieldByName("Enable")->AsString.c_str())!=0)
                Cylinder[i].Enable=true;

            if((Cylinder[i].OnSenRing==0 && Cylinder[i].OnSenIP==0 && Cylinder[i].OnSenPort==0 && Cylinder[i].OnSenBit==0) ||    //Steven 20200901 : 修正氣缸Sensor的enable設定
                Cylinder[i].OnSenRing<0 || Cylinder[i].OnSenIP<0 || Cylinder[i].OnSenPort<0 || Cylinder[i].OnSenBit<0)
                Cylinder[i].OnSenEnable =false;
            else if(Cylinder[i].OnSenRingUse=="")                               //Steven 20230723 : 修正Cylinder Enable判斷方式
                Cylinder[i].OnSenEnable =false;
            else
                Cylinder[i].OnSenEnable =Cylinder[i].Enable;

            if((Cylinder[i].OffSenRing==0 && Cylinder[i].OffSenIP==0 && Cylinder[i].OffSenPort==0 && Cylinder[i].OffSenBit==0) ||    //Steven 20200901 : 修正氣缸Sensor的enable設定
                Cylinder[i].OffSenRing<0 || Cylinder[i].OffSenIP<0 || Cylinder[i].OffSenPort<0 || Cylinder[i].OffSenBit<0)
                Cylinder[i].OffSenEnable=false;
            else if(Cylinder[i].OffSenRingUse=="")                              //Steven 20230723 : 修正Cylinder Enable判斷方式
                Cylinder[i].OffSenEnable=false;
            else
                Cylinder[i].OffSenEnable=Cylinder[i].Enable;
            #endif

            Cylinder[i].OnSensorName=Cylinder[i].CylinderName+"_On";
            Cylinder[i].OffSensorName=Cylinder[i].CylinderName+"_Off";

            #ifndef SOFT_SIMULTE
            if(Cylinder[i].Enable)
            #endif
            {
                MyLaneIO.SetUseIP(Cylinder[i].OutRing, Cylinder[i].OutIP, Cylinder[i].ISABase, Cylinder[i].CylinderName);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
            }

            #ifndef SOFT_SIMULTE
            if(Cylinder[i].OnSenEnable)
            #endif
                MyLaneIO.SetUseIP(Cylinder[i].OnSenRing, Cylinder[i].OnSenIP, Cylinder[i].OnSenISABase, Cylinder[i].CylinderName, false);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復

            #ifndef SOFT_SIMULTE
            if(Cylinder[i].OffSenEnable)
            #endif
                MyLaneIO.SetUseIP(Cylinder[i].OffSenRing, Cylinder[i].OffSenIP, Cylinder[i].OffSenISABase, Cylinder[i].CylinderName, false);     //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
        }
        T->Active=false;
    }

    if(AUTO_EMPTY_COLOR==0)                                                     //四軌  //Steven 20100709 Start: 不同軌道，強制啟動
    {
        Cylinder[C_EmptyLoaderZ_Select].Enable  =false;
        Cylinder[C_ColorLoaderZ_Select].Enable  =false;
        Cylinder[C_Empty_Fix].Enable            =false;
        Cylinder[C_Color_Fix].Enable            =false;
        Cylinder[C_Color_Up].Enable             =false;
        Cylinder[C_Color_Middle].Enable         =false;
        Cylinder[C_Empty_Up].Enable             =false;
        Cylinder[C_Empty_Middle].Enable         =false;
    }
    else if(AUTO_EMPTY_COLOR==1 && USE_MR_SYSTEM)
    {
        Cylinder[C_EmptyLoaderZ_Select].Enable  =true;
        Cylinder[C_ColorLoaderZ_Select].Enable  =false;
        Cylinder[C_Empty_Fix].Enable            =true;
        Cylinder[C_Color_Fix].Enable            =false;
        Cylinder[C_Color_Up].Enable             =false;
        Cylinder[C_Color_Middle].Enable         =false;
        Cylinder[C_Empty_Up].Enable             =true;
        Cylinder[C_Empty_Middle].Enable         =true;
    }
    else //if(AUTO_EMPTY_COLOR==1)                                              //六軌
    {
        Cylinder[C_EmptyLoaderZ_Select].Enable  =true;
        Cylinder[C_ColorLoaderZ_Select].Enable  =true;
        Cylinder[C_Empty_Fix].Enable            =true;
        Cylinder[C_Color_Fix].Enable            =true;
        Cylinder[C_Color_Up].Enable             =true;
        Cylinder[C_Color_Middle].Enable         =true;
        Cylinder[C_Empty_Up].Enable             =true;
        Cylinder[C_Empty_Middle].Enable         =true;
        if(LOAD_Z_USE_MOTOR[1]==true)                                           //Ztex 2024.08.13 Add Check Select Has Tray
            Cylinder[C_Empty_Middle].Enable     =false;
        if(LOAD_Z_USE_MOTOR[2]==true)                                           //Ztex 2024.08.13 Add Check Select Has Tray
            Cylinder[C_Color_Middle].Enable     =false;
    }
/*    else                                                                      //七軌//QQQ
    {

    }*/

    for(int i=0; i<MAX_AUTO_TRAY; i++)                                          //Steven 20200522 : 避免只安裝auto 2 ART的版本, 資料庫設定錯誤
    {
        if(USE_AUTO_RETEST==(int)eartUninstall ||
           UNLOADER_ART[i]==(int)eartUninstall)
        {
            Cylinder[C_Auto_Up[i]].Enable=false;
            Cylinder[C_AutoZ_Select[i]].Enable=false;
        }
    }

    //******************************************************************************
    //  注意!! bCheckSafeDoor為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    for(int i=0; i<MaxCylinderItem; i++)                                        //Steven 20230703 : Add Cylinder action check SafeDoor
    {
        Cylinder[i].bCheckSafeDoor=false;
        if(Cylinder[i].Enable==true)
        {
            if(Cylinder[i].CylinderName=="C_LoaderUpPress" ||                   //By Pass項目需要軟體硬體跟主管, 三司會審~
               Cylinder[i].CylinderName=="C_TrayY_Fixer" ||
               Cylinder[i].CylinderName=="C_LoaderEdgePush" ||
               Cylinder[i].CylinderName=="C_TrayZ_Selector" ||
               Cylinder[i].CylinderName=="C_Load_Middle" ||
               Cylinder[i].CylinderName=="C_Load_Up" ||
               Cylinder[i].CylinderName=="C_Empty_Fix" ||
               Cylinder[i].CylinderName=="C_Empty_Middle" ||
               Cylinder[i].CylinderName=="C_Empty_Up" ||
               Cylinder[i].CylinderName=="C_Color_Fix" ||
               Cylinder[i].CylinderName=="C_Color_Middle" ||
               Cylinder[i].CylinderName=="C_Color_Up" ||
               Cylinder[i].CylinderName=="C_Auto1UpPress" ||
               Cylinder[i].CylinderName=="C_Auto1Side_Fixer" ||
               Cylinder[i].CylinderName=="C_Auto1EdgePush" ||
               Cylinder[i].CylinderName=="C_Auto1LoaderZ_Select" ||
               Cylinder[i].CylinderName=="C_Auto1_Selector" ||
               Cylinder[i].CylinderName=="C_Auto1_Up" ||
               Cylinder[i].CylinderName=="C_Auto1TrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Auto2UpPress" ||
               Cylinder[i].CylinderName=="C_Auto2Side_Fixer" ||
               Cylinder[i].CylinderName=="C_Auto2EdgePush" ||
               Cylinder[i].CylinderName=="C_Auto2LoaderZ_Select" ||
               Cylinder[i].CylinderName=="C_Auto2_Selector" ||
               Cylinder[i].CylinderName=="C_Auto2_Up" ||
               Cylinder[i].CylinderName=="C_Auto2TrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Auto3UpPress" ||
               Cylinder[i].CylinderName=="C_Auto3Side_Fixer" ||
               Cylinder[i].CylinderName=="C_Auto3EdgePush" ||
               Cylinder[i].CylinderName=="C_Auto3LoaderZ_Select" ||
               Cylinder[i].CylinderName=="C_Auto3_Selector" ||
               Cylinder[i].CylinderName=="C_Auto3_Up" ||
               Cylinder[i].CylinderName=="C_Auto3TrackFloodgate" ||
               //Steven 20230907 : For HT-9011UC
               //==>
               Cylinder[i].CylinderName=="C_Auto4UpPress" ||
               Cylinder[i].CylinderName=="C_Auto4Side_Fixer" ||
               Cylinder[i].CylinderName=="C_Auto4EdgePush" ||
               Cylinder[i].CylinderName=="C_Auto4LoaderZ_Select" ||
               Cylinder[i].CylinderName=="C_Auto4_Selector" ||
               Cylinder[i].CylinderName=="C_Auto4_Up" ||
               Cylinder[i].CylinderName=="C_Auto4TrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Auto5UpPress" ||
               Cylinder[i].CylinderName=="C_Auto5Side_Fixer" ||
               Cylinder[i].CylinderName=="C_Auto5EdgePush" ||
               Cylinder[i].CylinderName=="C_Auto5LoaderZ_Select" ||
               Cylinder[i].CylinderName=="C_Auto5_Selector" ||
               Cylinder[i].CylinderName=="C_Auto5_Up" ||
               Cylinder[i].CylinderName=="C_Auto5TrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Auto6UpPress" ||
               Cylinder[i].CylinderName=="C_Auto6Side_Fixer" ||
               Cylinder[i].CylinderName=="C_Auto6EdgePush" ||
               Cylinder[i].CylinderName=="C_Auto6LoaderZ_Select" ||
               Cylinder[i].CylinderName=="C_Auto6_Selector" ||
               Cylinder[i].CylinderName=="C_Auto6_Up" ||
               Cylinder[i].CylinderName=="C_Auto6TrackFloodgate" ||
               //<==
               //Steven 20230907 : For HT-9011UC
               Cylinder[i].CylinderName=="C_SafeDoor1Lock" ||
               Cylinder[i].CylinderName=="C_SafeDoor2Lock" ||
               Cylinder[i].CylinderName=="C_SafeDoor3Lock" ||
               Cylinder[i].CylinderName=="C_SafeDoor4Lock" ||
               Cylinder[i].CylinderName=="C_SafeDoor5Lock" ||
               Cylinder[i].CylinderName=="C_SafeDoor6Lock" ||
               Cylinder[i].CylinderName=="C_SafeDoor7Lock" ||
               Cylinder[i].CylinderName=="C_SafeDoor8Lock" ||
               Cylinder[i].CylinderName=="C_LoadTrackFloodgate" ||
               Cylinder[i].CylinderName=="C_EmptyTrackFloodgate" ||
               Cylinder[i].CylinderName=="C_ColorTrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Auto1TrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Auto2TrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Auto3TrackFloodgate" ||
               Cylinder[i].CylinderName=="C_Shuttle1Floodgate" ||
               Cylinder[i].CylinderName=="C_Shuttle2Floodgate" ||
               Cylinder[i].CylinderName=="C_OutShuttle1Floodgate" ||            //Ifor 20240620 add:Out Shuttle Floodgate
               Cylinder[i].CylinderName=="C_OutShuttle2Floodgate" ||            //Ifor 20240620 add:Out Shuttle Floodgate
               Cylinder[i].CylinderName=="C_HotplateVibration" ||               //Steven 20230721 : Add C_HotplateVibration/C_TrayVibration/C_CoolingValve 作動不判斷安全門, 避免無窮迴圈
               Cylinder[i].CylinderName=="C_TrayVibration" ||
               Cylinder[i].CylinderName=="C_CoolingValve"  ||
               (Cylinder[i].CylinderName=="C_TrayX_UpDown"      && CUSTOMER_CODE==CC_SIGURD_PeiXing) ||                    //Sam 20250327 : 矽格北興要求TrayArm 不要卡控
               (Cylinder[i].CylinderName=="C_CatchTray_FixOn"   && CUSTOMER_CODE==CC_SIGURD_PeiXing) ||
               (Cylinder[i].CylinderName=="C_CatchTray_FixOff"  && CUSTOMER_CODE==CC_SIGURD_PeiXing) ||
               (Cylinder[i].CylinderName=="C_TrayCover"         && CUSTOMER_CODE==CC_SIGURD_PeiXing) ||
               (Cylinder[i].CylinderName=="C_CatchTray_Fix"     && CUSTOMER_CODE==CC_SIGURD_PeiXing) ||
               Cylinder[i].CylinderName=="C_LoadCarRFIDRotArmD"  ||             //RogerYang 20250828 add for Loader Rotate Arm
               Cylinder[i].CylinderName=="C_LoadCarRFIDRotArmU"  ||             //RogerYang 20250828 add for Loader Rotate Arm
               Cylinder[i].CylinderName=="C_LoadTrayDetD"  ||                   //RogerYang 20250828 add for 殘料檢氣缸
               Cylinder[i].CylinderName=="C_LoadTrayDetU"  ||                   //RogerYang 20250828 add for 殘料檢氣缸
               Cylinder[i].CylinderName=="C_LoadTrayDetF"  ||                   //RogerYang 20250828 add for 殘料檢氣缸
               Cylinder[i].CylinderName=="C_LoadTrayDetB")                      //RogerYang 20250828 add for 殘料檢氣缸
            {
                if(Cylinder[i].ISABase==eISABase ||
                   Cylinder[i].ISABase==ePCI1735U ||
                   Cylinder[i].ISABase==ePLCbase)
                {
                    Cylinder[i].bCheckSafeDoor=false;
                    iIP  =Cylinder[i].OutPort/32;                               //Steven 20240103 : 加入保護
                    iPort=Cylinder[i].OutPort%32;
                    bIdleNeedCheckSafeDoor[0][iIP][iPort][Cylinder[i].OutBit]=false;
                }
                else
                {                                                               //預設只有4個Ring, 超過要改陣列
                    bIdleNeedCheckSafeDoor[Cylinder[i].OutRing][Cylinder[i].OutIP][Cylinder[i].OutPort][Cylinder[i].OutBit]=false;
                }
            }
            else
            {
                if(Cylinder[i].ISABase==eISABase ||
                   Cylinder[i].ISABase==ePCI1735U ||
                   Cylinder[i].ISABase==ePLCbase)
                {
                    Cylinder[i].bCheckSafeDoor=true;
                    iIP  =Cylinder[i].OutPort/32;                               //Steven 20240103 : 加入保護
                    iPort=Cylinder[i].OutPort%32;
                    bIdleNeedCheckSafeDoor[0][iIP][iPort][Cylinder[i].OutBit]=true;
                }
                else
                {                                                               //預設只有4個Ring, 超過要改陣列
                    bIdleNeedCheckSafeDoor[Cylinder[i].OutRing][Cylinder[i].OutIP][Cylinder[i].OutPort][Cylinder[i].OutBit]=true;
                }
            }
        }
    }

    RunInfo.bHasLoaderPressTrayModule=Cylinder[C_LoaderUpPress].Enable;
    RunInfo.bHasHotplateVibrationModule=Cylinder[C_HotplateVibration].Enable;
}
//==============================================================================
void SetMotorScaleSpeed(int Index, int ScaleSpeed)
{
    if(ScaleSpeed>100)
        ScaleSpeed=100;

    if(IniConfig.bG14UseStartSoundAlarm && bStartMoveSpeed)                     //kevin 20201116  motor move speed 10 % 移鄧10 sec 系統暫停 恢復速度
        ScaleSpeed=10;

    if(ScaleSpeed<=1)
        ScaleSpeed=1;

    if(MOT[Index].Motor->Enable)
    {
        if(INDEX_MOTION_CARD==0 &&                                              //Steven 20210623 : Index使用Galil
           (Index==MTestY1 || Index==MTestZ1 || Index==MTestZ2 || Index==MTestY2))
        {
            if(ScaleSpeed>95)
                ScaleSpeed=95;
            MOT[Index].GailSpeed=MOT[Index].Motor->PJogHighSpeed*ScaleSpeed/100;
        }
        else
        {
            MOT[Index].SetSpeed(ScaleSpeed);
        }
    }
    else
    {
        #ifdef SOFT_SIMULTE
        if(INDEX_MOTION_CARD==0 &&                                              //Steven 20221121 : Index使用GalilSpeed
           (Index==MTestY1 || Index==MTestZ1 || Index==MTestZ2 || Index==MTestY2))
        {
            if(ScaleSpeed>95)
                ScaleSpeed=95;
            MOT[Index].GailSpeed=MOT[Index].Motor->PJogHighSpeed*ScaleSpeed/100;
        }
        #endif
        MOT[Index].SetSpeed(LastSet.SoftSpeed[Index]*ScaleSpeed/100);
    }
}
//------------------------------------------------------------------------------
void SetMotorAccelSpeed(int Index, int ADCSpeed)
{
    if(INDEX_MOTION_CARD==0 && (Index==MTestY1 || Index==MTestZ1 ||
                                Index==MTestZ2 || Index==MTestY2))              //Steven 20210623 : Index使用Galil
    {
        return;
    }

    if(ADCSpeed>100)
        ADCSpeed=100;

    if(IniConfig.bG14UseStartSoundAlarm && bStartMoveSpeed)                     //kevin 20201116  motor move speed 10 % 移鄧10 sec 系統暫停 恢復速度
        ADCSpeed=10;

    if(ADCSpeed<=1)
        ADCSpeed=1;
    if(MOT[Index].Motor->Enable)
    {
        MOT[Index].SetADCRate(ADCSpeed);
    }
}
//------------------------------------------------------------------------------
// 設定馬達比例速度,每一次 [START] 就會設定一次
//------------------------------------------------------------------------------
void SetGaliRate(int);
void SetMotorSpeed()
{
    int iSpeed[4]={0, 0, 0, 0};
    if(iHome==1 && bNeedArmZHome)
        return;

    if(bRunAutoClean && TestIF_File.iAutoClean_Function)                        //2013-10-19    Dell    fix 在做clean中按暫停開始速度會跑掉
    {
        CleanSetSpeed(true);                                                    //設定 "速度"
    }

    //Input Arm
    SetInArmSpeed(false);

    //Shuttle
    if(IniConfig.bHaveRotateShuttle==true && TestIF_File.bRotateShuttle)        //Steven 20100713 Start: 轉轉蝦頭
    {
        iSpeed[0]=IniConfig.iRotateADC;
        iSpeed[1]=IniConfig.iRotateADC;
    }
    else
    {
        iSpeed[0]=SHSpeed.iSH1ACDCSp;
        iSpeed[1]=SHSpeed.iSH2ACDCSp;
    }

    if(fContact->fShow==true ||                                                 //Steven 20110826 : Contact Test時，暫停再Start，Shuttle會爆衝
       fShuttleMove->fShow==true ||                                             //JerryYang 20160801 shuttle maintain時也要避免爆衝
       Zteach->fShow==true)                                                     //JerryYang 20170123 add Zteach
    {
        iSpeed[2]=10;
        iSpeed[3]=10;
    }
    else
    {
        iSpeed[2]=SHSpeed.iSH1Sp;                                               //kevin 20110520
        iSpeed[3]=SHSpeed.iSH2Sp;                                               //kevin 20110520
    }

    if(IniConfig.bF01ShakeShuttleWhenJam && bShuttleShake)                      //Sam 20200113 : 防止要搖搖時恢復速度
    {

    }
    else
    {
        SetMotorAccelSpeed(MInShuttle1, iSpeed[0]);
        SetMotorAccelSpeed(MInShuttle2, iSpeed[1]);
        SetMotorScaleSpeed(MInShuttle1, iSpeed[2]);
        SetMotorScaleSpeed(MInShuttle2, iSpeed[3]);
    }

//#ifdef Carry4
//    SetMotorAccelSpeed(MOutShuttle1, SHSpeed.iSH1ACDCSp);
//    SetMotorAccelSpeed(MOutShuttle2, SHSpeed.iSH2ACDCSp);
//    SetMotorScaleSpeed(MOutShuttle1, SHSpeed.iSH1Sp);
//    SetMotorScaleSpeed(MOutShuttle2, SHSpeed.iSH2Sp);
//#endif

    //Index
    SetMotorScaleSpeed(MTestY1, ArmSpeed[IndexArm].iBodySP);
    SetMotorScaleSpeed(MTestZ1, ArmSpeed[IndexArm].iBodySP);
    SetMotorScaleSpeed(MTestZ2, ArmSpeed[IndexArm].iBodySP);
    SetMotorScaleSpeed(MTestY2, ArmSpeed[IndexArm].iBodySP);
    SetGaliRate(ArmSpeed[IndexArm].iACDCBodySP);

    //Output Arm
    SetOutArmSpeed(false);

    //Tray Arm
    SetMotorAccelSpeed(MTrayX, ArmSpeed[TrayArm].iACDCBodySP);
    if(USE_AUTO_RETEST==eartInstall && CUSTOMER_CODE==CC_ASE_KaohSiung)         //kevin 20150606 固定最高速
    {
        iTrayArmAutoRetestBuffer=ArmSpeed[TrayArm].iBodySP;
        if(ArmSpeed[TrayArm].iBodySP>=50)
            iTrayArmAutoRetestBuffer=50;
        SetMotorScaleSpeed(MTrayX, iTrayArmAutoRetestBuffer);
    }
    else
    {
        if(LastSet.iRunStartMode==rsmAutoRetest)                                //Steven 20170118 : For ART Tray Arm Speed
            SetMotorScaleSpeed(MTrayX, IniConfig.iARTTrayArmSpeed);
        else
            SetMotorScaleSpeed(MTrayX, ArmSpeed[TrayArm].iBodySP);
    }

    SetMotorAccelSpeed(MTrayZ, ArmSpeed[TrayArm].iACDCBodySP);
    SetMotorScaleSpeed(MTrayZ, ArmSpeed[TrayArm].iBodySP);

    //Tray Z
    SetMotorScaleSpeed(MLoaderZ, TestIF_File.iTrayZStepSpeed[0]);               //Sam 20201221 : 修正 Tray Z馬達速度設定
    SetMotorScaleSpeed(MEmptyZ , TestIF_File.iTrayZStepSpeed[1]);
    SetMotorScaleSpeed(MColorZ , TestIF_File.iTrayZStepSpeed[2]);
    SetMotorScaleSpeed(MAuto1Z , TestIF_File.iTrayZStepSpeed[3]);
    SetMotorScaleSpeed(MAuto2Z , TestIF_File.iTrayZStepSpeed[4]);
    SetMotorScaleSpeed(MAuto3Z , TestIF_File.iTrayZStepSpeed[5]);               //JerryYang 20210721 Auto2Z->Auto3Z

    SetMotorScaleSpeed(MMagazine        , MGSpeed.iMZSp);                       //JerryYang 20221215 : add Magazine
    SetMotorScaleSpeed(MCatchMgzTray    , MGSpeed.iCYSp);
    SetMotorAccelSpeed(MMagazine        , MGSpeed.iMZACDCSp);
    SetMotorAccelSpeed(MCatchMgzTray    , MGSpeed.iCYACDCSp);

    //Rotate
    if(FrmRotate!=NULL)
    {
        FrmRotate->SetInRotateSpeed(ArmSpeed[InRotate].iACDCBodySP, ArmSpeed[InRotate].iBodySP);
        FrmRotate->SetOutRotateSpeed(ArmSpeed[OutRotate].iACDCBodySP, ArmSpeed[OutRotate].iBodySP);
    }

    if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor)                                  //Steven 20250908 : Fix 3 馬達版設定速度
    {
        SetMotorAccelSpeed(MFix3Full, 100);
        SetMotorScaleSpeed(MFix3Full, 100);
    }

    SetMotorAccelSpeed(MAOIKit                ,10);
    SetMotorScaleSpeed(MAOIKit                ,60);

    SetMotorScaleSpeed(MLdCarRotArm, ArmSpeed[TrayArm].iBodySP);                //RogerYang 20251028 : 暫用TrayArm的速度(但後來馬達改了，可能要再修)

    fShowMessage->sgdSpeedView->Repaint();
}
//------------------------------------------------------------------------------
void SetInArmSpeed(bool bShow, bool bReset)                                     //Steven 20140909 : Clean Out後要重置Speed
{
    int iMotNo;
    eSpeedPart Arm=InArm;
    if(bRunAutoClean && TestIF_File.iAutoClean_Function)                        //Steven 20160630 : set auto clean speed
    {
        CleanSetSpeed(true);
    }
    else if(ArmSpeed[Arm].bAutoSpeed==false)
    {
        SetMotorAccelSpeed(MInArmX, ArmSpeed[Arm].iACDCBodySP);
        SetMotorAccelSpeed(MInArmY, ArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MInArmX, ArmSpeed[Arm].iBodySP);
        SetMotorScaleSpeed(MInArmY, ArmSpeed[Arm].iBodySP);
        SetMotorAccelSpeed(MInArmPitch, ArmSpeed[Arm].iACDCVariSP);
        SetMotorScaleSpeed(MInArmPitch, ArmSpeed[Arm].iVariSP);

        if(USE_IN_Y_IS_AUTO_PITCH==true)                                              //ChungHung 20131231 alter AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            if(ArmSpeed[Arm].iACDCVariSP<80)                                    //Steven 20160630 : 限制Y-Pitch最低速度為80
                SetMotorAccelSpeed(MInArmPitchY, 80);
            else
                SetMotorAccelSpeed(MInArmPitchY, ArmSpeed[Arm].iACDCVariSP);

            if(ArmSpeed[Arm].iVariSP<80)
                SetMotorScaleSpeed(MInArmPitchY, 80);
            else
                SetMotorScaleSpeed(MInArmPitchY, ArmSpeed[Arm].iVariSP);
            SetMotorAccelSpeed(MInArmPitchX2, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MInArmPitchX2, ArmSpeed[Arm].iVariSP);
        }

        if(USE_PICKER_COUNT==ep16Picker)
        {
            SetMotorAccelSpeed(MInArmPitchX3, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MInArmPitchX3, ArmSpeed[Arm].iVariSP);
            SetMotorAccelSpeed(MInArmPitchX4, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MInArmPitchX4, ArmSpeed[Arm].iVariSP);
        }

        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                if(USE_PICKER_COUNT==ep16Picker &&
                   InOutArmPickerUseMotor==eptUseMotCyn)
                    iMotNo=MInArmZA;
                else
                    iMotNo=InArmSuck.Suck[i][j].iMotNo;
                if(IniConfig.bE51_EnableInArmZADC)                              //Steven 20141212 : 使用固定的ADC
                    SetMotorAccelSpeed(iMotNo, IniConfig.iE51_EnableInArmZADC);
                else
                    SetMotorAccelSpeed(iMotNo, ArmSpeed[Arm].iACDCZSP);
                SetMotorScaleSpeed(iMotNo, ArmSpeed[Arm].iZSP);
            }
        }
    }
    else
    {
        if(bReset)                                                              //Steven 20140909 : Clean Out後要重置Speed
        {
            AutoArmSpeed[Arm].iACDCBodySP=ArmSpeed[Arm].iACDCBodySP;
            AutoArmSpeed[Arm].iBodySP    =ArmSpeed[Arm].iBodySP;
        }

        if(AutoArmSpeed[Arm].iBodySP>ArmSpeed[Arm].iBodySP)
            AutoArmSpeed[Arm].iBodySP=ArmSpeed[Arm].iBodySP;

        if(AutoArmSpeed[Arm].iBodySP<25)
            AutoArmSpeed[Arm].iBodySP=25;

        if(AutoArmSpeed[Arm].iACDCBodySP>ArmSpeed[Arm].iACDCBodySP)
            AutoArmSpeed[Arm].iACDCBodySP=ArmSpeed[Arm].iACDCBodySP;

        if(AutoArmSpeed[Arm].iACDCBodySP<25)
            AutoArmSpeed[Arm].iACDCBodySP=25;

        if(AutoArmSpeed[Arm].iACDCBodySP<ArmSpeed_File[Arm].iAutoSpeedLow)      //kevin 20210913 AutoSpeed 最低速
            AutoArmSpeed[Arm].iACDCBodySP=ArmSpeed_File[Arm].iAutoSpeedLow;

        if(AutoArmSpeed[Arm].iBodySP<ArmSpeed_File[Arm].iAutoSpeedLow)          //kevin 20210913 AutoSpeed 最低速
            AutoArmSpeed[Arm].iBodySP=ArmSpeed_File[Arm].iAutoSpeedLow;

        SetMotorAccelSpeed(MInArmX, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorAccelSpeed(MInArmY, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MInArmX, AutoArmSpeed[Arm].iBodySP);
        SetMotorScaleSpeed(MInArmY, AutoArmSpeed[Arm].iBodySP);

        SetMotorAccelSpeed(MInArmPitch, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MInArmPitch, AutoArmSpeed[Arm].iBodySP);

        if(USE_IN_Y_IS_AUTO_PITCH==true)                                              //ChungHung 20131231 alter AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            if(AutoArmSpeed[Arm].iACDCBodySP<80)                                //Steven 20160630 : 限制Y-Pitch最低速度為80
                SetMotorAccelSpeed(MInArmPitchY, 80);
            else
                SetMotorAccelSpeed(MInArmPitchY, AutoArmSpeed[Arm].iACDCBodySP);

            if(AutoArmSpeed[Arm].iBodySP<80)
                SetMotorScaleSpeed(MInArmPitchY, 80);
            else
                SetMotorScaleSpeed(MInArmPitchY, AutoArmSpeed[Arm].iBodySP);
            SetMotorAccelSpeed(MInArmPitchX2, AutoArmSpeed[Arm].iACDCBodySP);   //Steven 20140930 : For XY-Pitch
            SetMotorScaleSpeed(MInArmPitchX2, AutoArmSpeed[Arm].iBodySP);       //Steven 20160123 : Fixed for Y-Pitch with auto speed
        }

        if(USE_PICKER_COUNT==ep16Picker)
        {
            SetMotorAccelSpeed(MInArmPitchX3, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MInArmPitchX3, ArmSpeed[Arm].iVariSP);
            SetMotorAccelSpeed(MInArmPitchX4, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MInArmPitchX4, ArmSpeed[Arm].iVariSP);
        }

        for(int i=0; i<InArmSuck.iMotRow; i++)
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                    iMotNo=MInArmZA;
                else
                    iMotNo=InArmSuck.Suck[i][j].iMotNo;
                if(IniConfig.bE51_EnableInArmZADC)                              //Steven 20141212 : 使用固定的ADC
                    SetMotorAccelSpeed(iMotNo, IniConfig.iE51_EnableInArmZADC);
                else
                    SetMotorAccelSpeed(iMotNo, AutoArmSpeed[Arm].iACDCBodySP);
                SetMotorScaleSpeed(iMotNo, AutoArmSpeed[Arm].iBodySP);
            }
        }

        if(bShow)
            fShowMessage->ShowSpeed(IniConfig.bG05ShowSpeedMessage);
    }
}
//------------------------------------------------------------------------------
void SetOutArmSpeed(bool bShow)                                                 //Steven 20140909 : Clean Out後要重置Speed
{
    int iMotNo;
    eSpeedPart Arm=OutArm;
    if(ArmSpeed[InArm].bAutoSpeed==false)                                       //jou 2013-01-24 Auto Speed 統一使用 In arm 當開啟設定值
    {
        SetMotorAccelSpeed(MOutArmX, ArmSpeed[Arm].iACDCBodySP);
        SetMotorAccelSpeed(MOutArmY, ArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MOutArmX, ArmSpeed[Arm].iBodySP);
        SetMotorScaleSpeed(MOutArmY, ArmSpeed[Arm].iBodySP);

        SetMotorAccelSpeed(MOutArmPitch, ArmSpeed[Arm].iACDCVariSP);
        SetMotorScaleSpeed(MOutArmPitch, ArmSpeed[Arm].iVariSP);

        if(USE_OUT_Y_IS_AUTO_PITCH==true)                                              //ChungHung 20131231 alter AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            if(ArmSpeed[Arm].iACDCVariSP<80)                                    //Steven 20160630 : 限制Y-Pitch最低速度為80
                SetMotorAccelSpeed(MOutArmPitchY, 80);
            else
                SetMotorAccelSpeed(MOutArmPitchY, ArmSpeed[Arm].iACDCVariSP);

            if(ArmSpeed[Arm].iVariSP<80)
                SetMotorScaleSpeed(MOutArmPitchY, 80);
            else
                SetMotorScaleSpeed(MOutArmPitchY, ArmSpeed[Arm].iVariSP);
            SetMotorAccelSpeed(MOutArmPitchX2, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MOutArmPitchX2, ArmSpeed[Arm].iVariSP);
        }

        if(USE_PICKER_COUNT==ep16Picker)
        {
            SetMotorAccelSpeed(MOutArmPitchX3, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MOutArmPitchX3, ArmSpeed[Arm].iVariSP);
            SetMotorAccelSpeed(MOutArmPitchX4, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MOutArmPitchX4, ArmSpeed[Arm].iVariSP);
        }

        for(int i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMotCol; j++)
            {
                if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                    iMotNo=MOutArmZA;
                else
                    iMotNo=OutArmSuck.Suck[i][j].iMotNo;
                if(IniConfig.bE51_EnableInArmZADC)                              //Steven 20141212 : 使用固定的ADC
                    SetMotorAccelSpeed(iMotNo, IniConfig.iE52_EnableOutArmZADC);
                else
                    SetMotorAccelSpeed(iMotNo, ArmSpeed[Arm].iACDCZSP);
                SetMotorScaleSpeed(iMotNo, ArmSpeed[Arm].iZSP);
            }
        }
    }
    else
    {
        if(AutoArmSpeed[Arm].iBodySP>ArmSpeed[Arm].iBodySP)
            AutoArmSpeed[Arm].iBodySP=ArmSpeed[Arm].iBodySP;

        if(AutoArmSpeed[Arm].iBodySP<25)
            AutoArmSpeed[Arm].iBodySP=25;

        if(AutoArmSpeed[Arm].iACDCBodySP>ArmSpeed[Arm].iACDCBodySP)
            AutoArmSpeed[Arm].iACDCBodySP=ArmSpeed[Arm].iACDCBodySP;

        if(AutoArmSpeed[Arm].iACDCBodySP<25)
            AutoArmSpeed[Arm].iACDCBodySP=25;

        if(AutoArmSpeed[Arm].iACDCBodySP<ArmSpeed_File[Arm].iAutoSpeedLow)      //kevin 20210913 AutoSpeed 最低速
            AutoArmSpeed[Arm].iACDCBodySP=ArmSpeed_File[Arm].iAutoSpeedLow;

        if(AutoArmSpeed[Arm].iBodySP<ArmSpeed_File[Arm].iAutoSpeedLow)          //kevin 20210913 AutoSpeed 最低速
            AutoArmSpeed[Arm].iBodySP=ArmSpeed_File[Arm].iAutoSpeedLow;

        SetMotorAccelSpeed(MOutArmX, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorAccelSpeed(MOutArmY, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MOutArmX, AutoArmSpeed[Arm].iBodySP);
        SetMotorScaleSpeed(MOutArmY, AutoArmSpeed[Arm].iBodySP);

        SetMotorAccelSpeed(MOutArmPitch, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MOutArmPitch, AutoArmSpeed[Arm].iBodySP);

        if(USE_IN_Y_IS_AUTO_PITCH==true)                                              //ChungHung 20131231 alter AutoYPitch  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        {
            if(AutoArmSpeed[Arm].iACDCBodySP<80)                                //Steven 20160630 : 限制Y-Pitch最低速度為80
                SetMotorAccelSpeed(MOutArmPitchY, 80);
            else
                SetMotorAccelSpeed(MOutArmPitchY, AutoArmSpeed[Arm].iACDCBodySP);

            if(AutoArmSpeed[Arm].iBodySP<80)
                SetMotorScaleSpeed(MOutArmPitchY, 80);
            else
                SetMotorScaleSpeed(MOutArmPitchY, AutoArmSpeed[Arm].iBodySP);
            SetMotorAccelSpeed(MOutArmPitchX2, AutoArmSpeed[Arm].iACDCBodySP);  //Steven 20140930 : For XY-Pitch
            SetMotorScaleSpeed(MOutArmPitchX2, AutoArmSpeed[Arm].iBodySP);      //Steven 20160123 : Fixed for Y-Pitch with auto speed
        }

        if(USE_PICKER_COUNT==ep16Picker)
        {
            SetMotorAccelSpeed(MOutArmPitchX3, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MOutArmPitchX3, ArmSpeed[Arm].iVariSP);
            SetMotorAccelSpeed(MOutArmPitchX4, ArmSpeed[Arm].iACDCVariSP);
            SetMotorScaleSpeed(MOutArmPitchX4, ArmSpeed[Arm].iVariSP);
        }

        for(int i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMotCol; j++)
            {
                if(USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMotCyn)
                    iMotNo=MOutArmZA;
                else
                    iMotNo=OutArmSuck.Suck[i][j].iMotNo;
                if(IniConfig.bE51_EnableInArmZADC)                              //Steven 20141212 : 使用固定的ADC
                    SetMotorAccelSpeed(iMotNo, IniConfig.iE52_EnableOutArmZADC);
                else
                    SetMotorAccelSpeed(iMotNo, AutoArmSpeed[Arm].iACDCBodySP);
                SetMotorScaleSpeed(iMotNo, AutoArmSpeed[Arm].iBodySP);
            }
        }

        if(bShow)
            fShowMessage->ShowSpeed(IniConfig.bG05ShowSpeedMessage);
    }
}
//------------------------------------------------------------------------------
void SetAllMotorSpeed(int speed)
{
    for(int i=0; i<TOTAL_MOTOR; i++)
        SetMotorScaleSpeed(i, speed);
}
//------------------------------------------------------------------------------
// 設定 Tray 是 SEND,RECEIVE ,吸嘴數量,MOT[]指標
//------------------------------------------------------------------------------
void SetMyKitSuckItemAmount()                                                   //Steven 20120906 : 改成依照機型選擇
{
    TestSocket.SetItemAmount(4, 8);                                             //Steven 20241101 : 指定成4x8

    if(MachineTypeChoice==Type_HT9046 ||
       MachineTypeChoice==Type_HT9046_LS ||
       MachineTypeChoice==Type_HT1032)
    {
        FLCarryKit.SetItemAmount(2, 8);
        FRCarryKit.SetItemAmount(2, 8);
        BLCarryKit.SetItemAmount(2, 8);
        BRCarryKit.SetItemAmount(2, 8);

        FTestSuck.SetItemAmount(2, 8);
        BTestSuck.SetItemAmount(2, 8);
        FTestSuckBackup.SetItemAmount(2, 8);
        BTestSuckBackup.SetItemAmount(2, 8);

        ptrInSHTBackup.SetItemAmount(2, 8);
//        if(MachineTypeChoice==Type_HT9046_LS)
//            TestSocket.SetItemAmount(4, 8);
//        else
//            TestSocket.SetItemAmount(2, 8);                                     //ChungHung 20140815 add 12 site hang up
    }
    else if(MachineTypeChoice==Type_HT9045_12Site)                              //ChungHung 20130507 add HT9045 updata for 12site 517 define Index and Shuttle 個數
    {
        FLCarryKit.SetItemAmount(2, 6);
        FRCarryKit.SetItemAmount(2, 6);
        BLCarryKit.SetItemAmount(2, 6);
        BRCarryKit.SetItemAmount(2, 6);

        FTestSuck.SetItemAmount(2, 6);
        BTestSuck.SetItemAmount(2, 6);
        FTestSuckBackup.SetItemAmount(2, 6);
        BTestSuckBackup.SetItemAmount(2, 6);

//        TestSocket.SetItemAmount(4, 8);

        ptrInSHTBackup.SetItemAmount(2, 6);                                     //ChungHung 20140815 add 12 site hang up
    }
    else
    {
        FLCarryKit.SetItemAmount(2, 4);
        FRCarryKit.SetItemAmount(2, 4);
        BLCarryKit.SetItemAmount(2, 4);
        BRCarryKit.SetItemAmount(2, 4);

        FTestSuck.SetItemAmount(2, 4);
        BTestSuck.SetItemAmount(2, 4);
        FTestSuckBackup.SetItemAmount(2, 4);
        BTestSuckBackup.SetItemAmount(2, 4);

//        TestSocket.SetItemAmount(4, 8);

        ptrInSHTBackup.SetItemAmount(2, 4);                                     //ChungHung 20140815 add 12 site hang up
    }

    if(USE_PICKER_COUNT==ep1Picker)                                             //Frank 20250812 : modify InOutArm1Motor
    {
        InArmSuckBackup.SetItemAmount(1, 1);
        OutArmSuckBackup.SetItemAmount(1, 1);
        InArmSuck.SetItemAmount(1, 1);
        OutArmSuck.SetItemAmount(1, 1);
        InArmSuckBackup.SetMotorCount(1, 1);
        OutArmSuckBackup.SetMotorCount(1, 1);
        InArmSuck.SetMotorCount(1, 1);
        OutArmSuck.SetMotorCount(1, 1);
    }
    else if(USE_PICKER_COUNT!=ep16Picker)                                       //Frank HT1032 add
    {
        InArmSuckBackup.SetItemAmount(2, 4);
        OutArmSuckBackup.SetItemAmount(2, 4);
        InArmSuck.SetItemAmount(2, 4);
        OutArmSuck.SetItemAmount(2, 4);
        InArmSuckBackup.SetMotorCount(2, 4);
        OutArmSuckBackup.SetMotorCount(2, 4);
        InArmSuck.SetMotorCount(2, 4);
        OutArmSuck.SetMotorCount(2, 4);
    }
    else
    {
        InArmSuck.SetItemAmount(2, 8);
        OutArmSuck.SetItemAmount(2, 8);
        InArmSuckBackup.SetItemAmount(2, 8);
        OutArmSuckBackup.SetItemAmount(2, 8);
        if(InOutArmPickerUseMotor==eptUseMot)
        {
            InArmSuck.SetMotorCount(2, 8);
            OutArmSuck.SetMotorCount(2, 8);
            InArmSuckBackup.SetMotorCount(2, 8);
            OutArmSuckBackup.SetMotorCount(2, 8);
        }
        else
        {
            InArmSuck.SetMotorCount(1, 1);
            OutArmSuck.SetMotorCount(1, 1);
            InArmSuckBackup.SetMotorCount(2, 1);
            OutArmSuckBackup.SetMotorCount(2, 1);
        }
    }
    OutArm2Suck.SetMotorCount(1, 2);                                            //Steven 20240822 : For HT-9046AU
    OutArm2Suck.SetItemAmount(1, 2);
    OutSht3Kit.SetItemAmount(2, 8);

    CatchTraySuck.SetItemAmount(1, 1);

    AOIKit.SetItemAmount(1, 1);                                                 //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    InArmPlaceSuck.SetItemAmount(1, 1);                                         //JimmyChiu 20220908 add Pickup Error Placement
}
//------------------------------------------------------------------------------
void InitialClass()
{
    for(int i=iFixMin; i<=iFixMax; i++)
    {
        MOT[iMMAuto[i]].ClearTray(__FUNC__);
    }
    SetMyKitSuckItemAmount();
}
//------------------------------------------------------------------------------
bool IsSuckerHasIC_NewIO_MN200()                                                //JimmyChiu 20220113 避免不正常關機導致掉料
{
    AnsiString sErrPart="", sData="";
    bool bInArmError=false, bOutArmError=false, bIndexArm1Error=false, bIndexArm2Error=false;
    int iC_DoorLock[]={C_SafeDoor1Lock, C_SafeDoor2Lock, C_SafeDoor3Lock, C_SafeDoor4Lock,
                       C_SafeDoor6Lock, C_SafeDoor7Lock, C_SafeDoor8Lock};      //Ztex 2024.12.19 Add Open Door Take Out Suck IC

    if(CosFunction.bInspectSuckICWhenSoftStart==false)                          //Jimmychiu 20240217 : 汶彥要求移除檢查
    {
        return false;
    }

    if((IO_CARD_TYPE==NewIO_MN200 || IO_CARD_TYPE==MotionnetIO_MN200) &&
        CheckAndReadIniDataGeneral("Record", "Program Close", 1)==0)
    {

    }
    else
    {
        return false;
    }

    BYTE NumLine=0x0;
    //connect to mn200
    #ifndef SOFT_SIMULTE
    if(mn_open_all(&NumLine)==0)
    {
        BYTE NumDev=0x0;
        for(int i=0; i<NumLine; i++)
            mn_start_line(i, &NumDev);
    }
    else
    {
        return true;
    }
    #endif

    while(1)
    {
        bInArmError=false;
        bOutArmError=false;
        bIndexArm1Error=false;
        bIndexArm2Error=false;

        sErrPart="";
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                if(InArmSuck.Suck[i][j].GetStatus())
                {
                    sErrPart+=InArmSuck.Suck[i][j].sName;
                    bInArmError=true;
                }
            }
        }
        if(bInArmError)
        {
            SW[SwMotorRelay].Off();
            SW[SwServerON].Off();

            if(SAFE_DOOR_LOCK)                                                  //20111130  Dell  //20111215 Dell
            {
                SW[SwSafeDoorLock].OnOff(false);
                SW[SwSafeDoorLock_LoadPort1].OnOff(false);
                SW[SwSafeDoorLock_LoadPort2].OnOff(false);
            }
            sData.sprintf("Please remove the IC at input arm %s.", sErrPart);
            ShowMyMessageBox_YES_NO(sData, "");
        }

        sErrPart="";
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                if(OutArmSuck.Suck[i][j].GetStatus())
                {
                    sErrPart+=OutArmSuck.Suck[i][j].sName;
                    bOutArmError=true;
                }
            }
        }
        if(bOutArmError)
        {
            SW[SwMotorRelay].Off();
            SW[SwServerON].Off();

            if(SAFE_DOOR_LOCK)                                                  //20111130  Dell  //20111215 Dell
            {
                SW[SwSafeDoorLock].OnOff(false);
                SW[SwSafeDoorLock_LoadPort1].OnOff(false);
                SW[SwSafeDoorLock_LoadPort2].OnOff(false);
            }
            sData.sprintf("Please remove the IC at output arm %s.", sErrPart);
            ShowMyMessageBox_YES_NO(sData, "");
        }

        for(int i=0; i<MAX_Index_Row; i++)  //交換IC狀態
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)
            {
                if(FTestSuck.Suck[i][j].GetStatus())
                {
                    sErrPart+=IndexSuckName[i][j];
                    bIndexArm1Error=true;
                }
            }
        }

        if(bIndexArm1Error)
        {
            SW[SwMotorRelay].Off();
            SW[SwServerON].Off();

            if(SAFE_DOOR_LOCK)                                                  //20111130  Dell  //20111215 Dell
            {
                SW[SwSafeDoorLock].OnOff(false);
                SW[SwSafeDoorLock_LoadPort1].OnOff(false);
                SW[SwSafeDoorLock_LoadPort2].OnOff(false);
            }
            sData.sprintf("Please remove the IC at index arm 1 %s.", sErrPart);
            ShowMyMessageBox_YES_NO(sData, "");

            if(MOT[MTestZ1].Led[iHomeLed]==false)                               //Steven 20240306 : 如果arm不在home, 就只檢查一次
                bIndexArm1Error=false;
        }
        sErrPart="";
        for(int i=0; i<MAX_Index_Row; i++)  //交換IC狀態
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)
            {
                if(BTestSuck.Suck[i][j].GetStatus())
                {
                    sErrPart+=IndexSuckName[i][j];
                    bIndexArm2Error=true;
                }
            }
        }

        if(bIndexArm2Error)
        {
            SW[SwMotorRelay].Off();
            SW[SwServerON].Off();

            if(SAFE_DOOR_LOCK)                                                  //20111130  Dell  //20111215 Dell
            {
                SW[SwSafeDoorLock].OnOff(false);
                SW[SwSafeDoorLock_LoadPort1].OnOff(false);
                SW[SwSafeDoorLock_LoadPort2].OnOff(false);
            }
            sData.sprintf("Please remove the IC at index arm 2 %s.", sErrPart);
            ShowMyMessageBox_YES_NO(sData, "");

            if(MOT[MTestZ2].Led[iHomeLed]==false)                               //Steven 20240306 : 如果arm不在home, 就只檢查一次
                bIndexArm2Error=false;
        }

        if(bIndexArm2Error==false && bIndexArm1Error==false && bInArmError==false && bOutArmError==false)
        {
            break;
        }
        else                                                                    //Ztex 2024.12.19 Add Open Door Take Out Suck IC
        {
            if(Tri_Temp_Machine==1)
            {
                for(int i=0; i<7; i++)
                {
                    if(Tri_Temp_Machine==1 && Enable_PLCSafety_IO==1)
                        Cylinder[iC_DoorLock[i]].Off();
                    else if(Tri_Temp_Machine==1)
                        Cylinder[iC_DoorLock[i]].On();
                }
            }
        }
    }

    if(mn_open_all(&NumLine)==0)
    {
        for(int i=0; i<NumLine; i++)
        {
            mn_reset(i);
            mn_stop_line(i);
        }
    }
    else
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void InitHontechHardware()
{
    InitSucker();
    InitialSwitch();
    IsSuckerHasIC_NewIO_MN200();                                                //JimmyChiu 20220113 避免不正常關機導致掉料
    InitialSensor();
    OpenPCI132Card(true);                                                       //ChungHung 20111101 add 24v壓降
    InitialMotorParameter();
    InitCylinder();

    #ifndef SOFT_SIMULTE                                                        //Maxwu 20230727 mark test
    if(Enable_PLCSafety_IO)                                                     //Austin 20190531.01 增加PLC_IO模組及安全相關IO. //Jason 20230619 增加安全PLC部分
    {
        bScanSlave[0]=true;
        InitPLCIO("172.16.8.120",502);                                          //固定
    }
    #endif                                                                      //Maxwu 20230727 mark test
    fTeach->InitialTeachEditList();                                             //JerryYang 20241119 : fix AOA
}
//------------------------------------------------------------------------------
void InitialHandler()
{
    InitialClass();
    InitNTPort();
    COM2->RS232Init();
    InitHontechHardware();
    LoadMachineRecord();
    ReadPassword();
    SetWorkParameter();
    SetMotorSpeed();                                                            //Steven 20160629 : Move down
    if(LoaderUnload_StepMotor ||                                                //Steven 20210304 : Tray Motor改模組化
       iControlPanelMode ||                                                     //KenHsieh 20211222 : Pad與步進馬達為同一Comport
       USE_VibrationCommunication)                                              //JerryYang 20230921 : add
    {
        dmTrayMotor->RS232Init(HSys.TrayStepMotor_ComPort);                     //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    }
};
//------------------------------------------------------------------------------
void SetSimuScreenPara()
{
    static bool flag=true;
    MOT[MInArmX].SetScreenScale(0, 109, Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase], Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]); //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
    #ifdef SOFT_SIMULTE
        MOT[MInArmY].SetScreenScale(208, 20, Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase], Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]);
    #else
        MOT[MInArmY].SetScreenScale(208, 8, Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase], Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]);
    #endif
    if(InOutArmPickerUseMotor==eptUseMot)
    {
        MOT[MInArmZA].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][0]);
        MOT[MInArmZC].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][1]);
        MOT[MInArmZE].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][2]);
        MOT[MInArmZG].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][3]);
        MOT[MInArmZB].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][0]);
        MOT[MInArmZD].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][1]);
        MOT[MInArmZF].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][2]);
        MOT[MInArmZH].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][3]);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                          //Steven for HT1032
           USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                             //Ztex 2023.12.06 Add HT-1032
        {
            MOT[MInArmZAe].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][4]);
            MOT[MInArmZAf].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][5]);
            MOT[MInArmZAg].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][6]);
            MOT[MInArmZAh].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][7]);
            MOT[MInArmZBe].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][4]);
            MOT[MInArmZBf].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][5]);
            MOT[MInArmZBg].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][6]);
            MOT[MInArmZBh].SetScreenScale(26, 38, ZSafePos, Prod.ZInArm_Tray_Pick[1][7]);
        }
    }
    else
    {
        MOT[MInArmZA].SetScreenScale( 2, 10, ZSafePos, Prod.ZInArm_Tray_Pick[0][0]);
    }

//#ifdef Carry4
//    MOT[MInShuttle1].SetScreenScale(136, 296, Prod.InSHT[0].iLeft, Prod.InSHT[0].iRight);
//    MOT[MInShuttle2].SetScreenScale(136, 296, Prod.InSHT[1].iLeft, Prod.InSHT[1].iRight);
//
//    MOT[MOutShuttle1].SetScreenScale(296, 433,Prod.OutSHT[0].iLeft, Prod.OutSHT[0].iRight);
//    MOT[MOutShuttle2].SetScreenScale(296, 433,Prod.OutSHT[1].iLeft, Prod.OutSHT[1].iRight);
//#else
    MOT[MInShuttle1].SetScreenScale(128, 284, Prod.InSHT[0].iLeft, Prod.InSHT[0].iRight);
    MOT[MInShuttle2].SetScreenScale(128, 284, Prod.InSHT[1].iLeft, Prod.InSHT[1].iRight);
//#endif
    if(bUseTwoArm32Site==true)
    {
        MOT[MTestY1].SetScreenScale(114, 97, Prod.TestY1_Front, Prod.TestY1_Middle);
        MOT[MTestY2].SetScreenScale(41, 60, Prod.TestY2_Rear, Prod.TestY2_Middle);
    }
    else
    {
        MOT[MTestY1].SetScreenScale(114, 80, Prod.TestY1_Front, Prod.TestY1_Middle);
        MOT[MTestY2].SetScreenScale(41, 80, Prod.TestY2_Rear, Prod.TestY2_Middle);
    }
    MOT[MTestZ1].SetScreenScale(320, 250, 0, Prod.TestZ1_Test);
    MOT[MTestZ2].SetScreenScale(320, 250, 0, Prod.TestZ2_Test);

    if(AUTO_EMPTY_COLOR>=3)
    {
        fMain->pnlOutArmY->Width=600;
        MOT[MOutArmX].SetScreenScale(0, 416, Prod.XOutArm_Auto_Place[0][iOutArmYBase][iOutArmXBase], 0);
    }
    else
    {
        fMain->pnlOutArmY->Width=330;
        MOT[MOutArmX].SetScreenScale(0, 224, Prod.XOutArm_Auto_Place[0][iOutArmYBase][iOutArmXBase], 0);
    }
    MOT[MOutArmY].SetScreenScale(8, 208, 0, Prod.YOutArm_Auto_Place[0][iOutArmYBase][iOutArmXBase]);

    if(InOutArmPickerUseMotor==eptUseMot)
    {
        MOT[MOutArmZA].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][0]);
        MOT[MOutArmZC].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][1]);
        MOT[MOutArmZE].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][2]);
        MOT[MOutArmZG].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][3]);

        MOT[MOutArmZB].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][0]);
        MOT[MOutArmZD].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][1]);
        MOT[MOutArmZF].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][2]);
        MOT[MOutArmZH].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][3]);

        if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                          //Steven for HT1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
           USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                             //Ztex 2023.12.06 Add HT-1032
        {
            MOT[MOutArmZAe].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][4]);
            MOT[MOutArmZAf].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][5]);
            MOT[MOutArmZAg].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][6]);
            MOT[MOutArmZAh].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][7]);
            MOT[MOutArmZBe].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][4]);
            MOT[MOutArmZBf].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][5]);
            MOT[MOutArmZBg].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][6]);
            MOT[MOutArmZBh].SetScreenScale(26, 38, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[1][7]);
        }
    }
    else
    {
        MOT[MOutArmZA].SetScreenScale( 2, 10, ZSafePos, Prod.ZOutArm_Shuttle1_Pick[0][0]);
    }

    MOT[MTrayX].SetScreenScale(130, 702, Prod.iXTrayLoad, Prod.iXTrayAuto[2]);
    MOT[MInArmPitch].SetScreenScale(0, 16, 0, 4100);
    MOT[MOutArmPitch].SetScreenScale(0, 16, 0, 4100);

    //ChungHung 20131231 alter AutoYPitch start
    MOT[MInArmPitchY].SetScreenScale(0, 16, 0, 4100);
    MOT[MOutArmPitchY].SetScreenScale(0, 16, 0, 4100);
    //ChungHung 20131231 alter AutoYPitch start

    MOT[MCasArmX].SetScreenScale(200, 800, Prod.iCassetteArmX[0], Prod.iCassetteArmX[9]);          //wei 20180702 MR
    MOT[MCaselevatorZ].SetScreenScale(304, 205, Prod.iLoadPortZ[0], Prod.iLoadPortZ[2]);           //wei 20180702 MR
    MOT[MTrayBracketZ].SetScreenScale(528, 420, Prod.iTrayBracketZ[0], Prod.iTrayBracketZ[1]);     //wei 20180702 MR
    MOT[MStackedTrayX].SetScreenScale(310, 740, Prod.iStackedTrayX[0], Prod.iStackedTrayX[5]);     //wei 20180702 MR
    //Sam 20190112 LM
    //==>
//    MOT[MLoadRobotZ].SetScreenScale(240, 600, Prod.iLoadRobotZ[0], Prod.iLoadRobotZ[4]);
    MOT[MUnloadRobotZ].SetScreenScale(240, 600, Prod.iUnloadRobotZ[0], Prod.iUnloadRobotZ[4]);
    //<==
    //Sam 20190112 LM
    if(flag)
    {
        if(InOutArmPickerUseMotor==eptUseMot)
        {
            MOT[MInArmZA].SetPanel((TWinControl *)fMain->ledInArmAa, true);
            MOT[MInArmZC].SetPanel((TWinControl *)fMain->ledInArmAb, true);
            MOT[MInArmZE].SetPanel((TWinControl *)fMain->ledInArmAc, true);
            MOT[MInArmZG].SetPanel((TWinControl *)fMain->ledInArmAd, true);
            MOT[MInArmZB].SetPanel((TWinControl *)fMain->ledInArmBa, true);
            MOT[MInArmZD].SetPanel((TWinControl *)fMain->ledInArmBb, true);
            MOT[MInArmZF].SetPanel((TWinControl *)fMain->ledInArmBc, true);
            MOT[MInArmZH].SetPanel((TWinControl *)fMain->ledInArmBd, true);

            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven for HT1032
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
            {
                MOT[MInArmZAe].SetPanel((TWinControl *)fMain->ledInArmAe, true);
                MOT[MInArmZAf].SetPanel((TWinControl *)fMain->ledInArmAf, true);
                MOT[MInArmZAg].SetPanel((TWinControl *)fMain->ledInArmAg, true);
                MOT[MInArmZAh].SetPanel((TWinControl *)fMain->ledInArmAh, true);
                MOT[MInArmZBe].SetPanel((TWinControl *)fMain->ledInArmBe, true);
                MOT[MInArmZBf].SetPanel((TWinControl *)fMain->ledInArmBf, true);
                MOT[MInArmZBg].SetPanel((TWinControl *)fMain->ledInArmBg, true);
                MOT[MInArmZBh].SetPanel((TWinControl *)fMain->ledInArmBh, true);
            }
        }
//        else //if(InOutArmPickerUseMotor==eptUseMotCyn)
//        {
//            MOT[MInArmZA].SetPanel((TWinControl *)fMain->pnlInArmX, true);
//            Cylinder[C_InArmAa].SetSimulateCompoment(fMain->ledInArmAa, akLeft,  8,  2);
//            Cylinder[C_InArmAb].SetSimulateCompoment(fMain->ledInArmAb, akLeft,  8,  2);
//            Cylinder[C_InArmAc].SetSimulateCompoment(fMain->ledInArmAc, akLeft,  8,  2);
//            Cylinder[C_InArmAd].SetSimulateCompoment(fMain->ledInArmAd, akLeft,  8,  2);
//            Cylinder[C_InArmBa].SetSimulateCompoment(fMain->ledInArmBa, akLeft, 28, 22);
//            Cylinder[C_InArmBb].SetSimulateCompoment(fMain->ledInArmBb, akLeft, 28, 22);
//            Cylinder[C_InArmBc].SetSimulateCompoment(fMain->ledInArmBc, akLeft, 28, 22);
//            Cylinder[C_InArmBd].SetSimulateCompoment(fMain->ledInArmBd, akLeft, 28, 22);
//
//            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven for HT1032
//               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
//            {
//                Cylinder[C_InArmAe].SetSimulateCompoment(fMain->ledInArmAe, akLeft,  8,  2);
//                Cylinder[C_InArmAf].SetSimulateCompoment(fMain->ledInArmAf, akLeft,  8,  2);
//                Cylinder[C_InArmAg].SetSimulateCompoment(fMain->ledInArmAg, akLeft,  8,  2);
//                Cylinder[C_InArmAh].SetSimulateCompoment(fMain->ledInArmAh, akLeft,  8,  2);
//                Cylinder[C_InArmBe].SetSimulateCompoment(fMain->ledInArmBe, akLeft, 28, 22);
//                Cylinder[C_InArmBf].SetSimulateCompoment(fMain->ledInArmBf, akLeft, 28, 22);
//                Cylinder[C_InArmBg].SetSimulateCompoment(fMain->ledInArmBg, akLeft, 28, 22);
//                Cylinder[C_InArmBh].SetSimulateCompoment(fMain->ledInArmBh, akLeft, 28, 22);
//            }
//        }

        MOT[MInArmY].SetPanel(fMain->Panel61, true);
        MOT[MInArmX].SetPanel((TWinControl *)fMain->pnlInArmX, false);          //Steven 20100827
//#ifdef Carry4
//        MOT[MInShuttle1].SetPanel(fMain->pnlInSht1);
//        MOT[MInShuttle2].SetPanel(fMain->pnlInSht2);
//
//        MOT[MOutShuttle1].SetPanel(fMain->pnlOutSht1);
//        MOT[MOutShuttle2].SetPanel(fMain->pnlOutSht2);
//        MOT[MOutShuttle1].SetUpDownMove(false);
//        MOT[MOutShuttle2].SetUpDownMove(false);
//
//        fMain->pnlInSht1->Visible=true;
//        fMain->pnlInSht2->Visible=true;
//        fMain->pnlOutSht1->Visible=true;
//        fMain->pnlOutSht2->Visible=true;
//        fMain->palShuttle1->Visible=false;
//        fMain->palShuttle2->Visible=false;
//#else
        MOT[MInShuttle1].SetPanel(fMain->palShuttle1, false);
        MOT[MInShuttle2].SetPanel(fMain->palShuttle2, false);

        fMain->pnlInSht1->Visible=false;
        fMain->pnlInSht2->Visible=false;
        fMain->pnlOutSht1->Visible=false;
        fMain->pnlOutSht2->Visible=false;
        fMain->palShuttle1->Visible=true;
        fMain->palShuttle2->Visible=true;
//#endif

        MOT[MOutArmX].SetPanel((TWinControl *)fMain->pnlOutArmX, false);        //Steven 20100827
        MOT[MOutArmY].SetPanel(fMain->pnlOutArmY, true);

        if(InOutArmPickerUseMotor==eptUseMot)
        {
            MOT[MOutArmZA].SetPanel((TWinControl *)fMain->ledOutArmAa, true);
            MOT[MOutArmZC].SetPanel((TWinControl *)fMain->ledOutArmAb, true);
            MOT[MOutArmZE].SetPanel((TWinControl *)fMain->ledOutArmAc, true);
            MOT[MOutArmZG].SetPanel((TWinControl *)fMain->ledOutArmAd, true);
            MOT[MOutArmZB].SetPanel((TWinControl *)fMain->ledOutArmBa, true);
            MOT[MOutArmZD].SetPanel((TWinControl *)fMain->ledOutArmBb, true);
            MOT[MOutArmZF].SetPanel((TWinControl *)fMain->ledOutArmBc, true);
            MOT[MOutArmZH].SetPanel((TWinControl *)fMain->ledOutArmBd, true);

            if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven for HT1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
               USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
            {
                MOT[MOutArmZAe].SetPanel((TWinControl *)fMain->ledOutArmAe, true);
                MOT[MOutArmZAf].SetPanel((TWinControl *)fMain->ledOutArmAf, true);
                MOT[MOutArmZAg].SetPanel((TWinControl *)fMain->ledOutArmAg, true);
                MOT[MOutArmZAh].SetPanel((TWinControl *)fMain->ledOutArmAh, true);
                MOT[MOutArmZBe].SetPanel((TWinControl *)fMain->ledOutArmBe, true);
                MOT[MOutArmZBf].SetPanel((TWinControl *)fMain->ledOutArmBf, true);
                MOT[MOutArmZBg].SetPanel((TWinControl *)fMain->ledOutArmBg, true);
                MOT[MOutArmZBh].SetPanel((TWinControl *)fMain->ledOutArmBh, true);
            }
        }
//        else //if(InOutArmPickerUseMotor==eptUseMotCyn)                         //Steven for HT1032
//        {
//            MOT[MOutArmZA].SetPanel((TWinControl *)fMain->pnlOutArmX, true);
//            Cylinder[C_OutArmAa].SetSimulateCompoment(fMain->ledOutArmAa, akLeft,  8,  2);
//            Cylinder[C_OutArmAb].SetSimulateCompoment(fMain->ledOutArmAb, akLeft,  8,  2);
//            Cylinder[C_OutArmAc].SetSimulateCompoment(fMain->ledOutArmAc, akLeft,  8,  2);
//            Cylinder[C_OutArmAd].SetSimulateCompoment(fMain->ledOutArmAd, akLeft,  8,  2);
//            Cylinder[C_OutArmBa].SetSimulateCompoment(fMain->ledOutArmBa, akLeft, 28, 22);
//            Cylinder[C_OutArmBb].SetSimulateCompoment(fMain->ledOutArmBb, akLeft, 28, 22);
//            Cylinder[C_OutArmBc].SetSimulateCompoment(fMain->ledOutArmBc, akLeft, 28, 22);
//            Cylinder[C_OutArmBd].SetSimulateCompoment(fMain->ledOutArmBd, akLeft, 28, 22);
//
//            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven for HT1032
//               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
//            {
//                Cylinder[C_OutArmAe].SetSimulateCompoment(fMain->ledOutArmAe, akLeft,  8,  2);
//                Cylinder[C_OutArmAf].SetSimulateCompoment(fMain->ledOutArmAf, akLeft,  8,  2);
//                Cylinder[C_OutArmAg].SetSimulateCompoment(fMain->ledOutArmAg, akLeft,  8,  2);
//                Cylinder[C_OutArmAh].SetSimulateCompoment(fMain->ledOutArmAh, akLeft,  8,  2);
//                Cylinder[C_OutArmBe].SetSimulateCompoment(fMain->ledOutArmBe, akLeft, 28, 22);
//                Cylinder[C_OutArmBf].SetSimulateCompoment(fMain->ledOutArmBf, akLeft, 28, 22);
//                Cylinder[C_OutArmBg].SetSimulateCompoment(fMain->ledOutArmBg, akLeft, 28, 22);
//                Cylinder[C_OutArmBh].SetSimulateCompoment(fMain->ledOutArmBh, akLeft, 28, 22);
//            }
//        }

        MOT[MTrayX].SetPanel((TWinControl *)fMain->ALed89, false);

        InArmSuckBackup.SetMyLed(0, 0,(TMyLed *)fMain->ledInArmAa);
        InArmSuckBackup.SetMyLed(0, 1,(TMyLed *)fMain->ledInArmAb);
        InArmSuckBackup.SetMyLed(0, 2,(TMyLed *)fMain->ledInArmAc);
        InArmSuckBackup.SetMyLed(0, 3,(TMyLed *)fMain->ledInArmAd);
        InArmSuckBackup.SetMyLed(1, 0,(TMyLed *)fMain->ledInArmBa);
        InArmSuckBackup.SetMyLed(1, 1,(TMyLed *)fMain->ledInArmBb);
        InArmSuckBackup.SetMyLed(1, 2,(TMyLed *)fMain->ledInArmBc);
        InArmSuckBackup.SetMyLed(1, 3,(TMyLed *)fMain->ledInArmBd);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                          //Steven for HT1032
           USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                             //Ztex 2023.12.06 Add HT-1032
        {
            InArmSuckBackup.SetMyLed(0, 4,(TMyLed *)fMain->ledInArmAe);
            InArmSuckBackup.SetMyLed(0, 5,(TMyLed *)fMain->ledInArmAf);
            InArmSuckBackup.SetMyLed(0, 6,(TMyLed *)fMain->ledInArmAg);
            InArmSuckBackup.SetMyLed(0, 7,(TMyLed *)fMain->ledInArmAh);
            InArmSuckBackup.SetMyLed(1, 4,(TMyLed *)fMain->ledInArmBe);
            InArmSuckBackup.SetMyLed(1, 5,(TMyLed *)fMain->ledInArmBf);
            InArmSuckBackup.SetMyLed(1, 6,(TMyLed *)fMain->ledInArmBg);
            InArmSuckBackup.SetMyLed(1, 7,(TMyLed *)fMain->ledInArmBh);
        }
//#ifdef Carry4
//        FLCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_FLCarryKitAa);
//        FLCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_FLCarryKitAb);
//        FLCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_FLCarryKitAc);
//        FLCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_FLCarryKitAd);
//        FLCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_FLCarryKitBa);
//        FLCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_FLCarryKitBb);
//        FLCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_FLCarryKitBc);
//        FLCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_FLCarryKitBd);
//
//        FRCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_FRCarryKitAa);
//        FRCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_FRCarryKitAb);
//        FRCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_FRCarryKitAc);
//        FRCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_FRCarryKitAd);
//        FRCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_FRCarryKitBa);
//        FRCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_FRCarryKitBb);
//        FRCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_FRCarryKitBc);
//        FRCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_FRCarryKitBd);
//
//        BLCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_BLCarryKitAa);
//        BLCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_BLCarryKitAb);
//        BLCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_BLCarryKitAc);
//        BLCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_BLCarryKitAd);
//        BLCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_BLCarryKitBa);
//        BLCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_BLCarryKitBb);
//        BLCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_BLCarryKitBc);
//        BLCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_BLCarryKitBd);
//
//        BRCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_BRCarryKitAa);
//        BRCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_BRCarryKitAb);
//        BRCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_BRCarryKitAc);
//        BRCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_BRCarryKitAd);
//        BRCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_BRCarryKitBa);
//        BRCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_BRCarryKitBb);
//        BRCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_BRCarryKitBc);
//        BRCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_BRCarryKitBd);
//#else
        FLCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_FLCarryKit_0);
        FLCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_FL2CarryKit_0);
        FLCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_FLCarryKit_1);
        FLCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_FL2CarryKit_1);
        FLCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_FLCarryKit_2);
        FLCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_FL2CarryKit_2);
        FLCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_FLCarryKit_3);
        FLCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_FL2CarryKit_3);

        if(MachineTypeChoice==Type_HT9046 ||                                    //Eliot 2009_12_25
           MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //ChungHung 20130507 add HT9045 updata for 12site 517 模擬畫面Shuttle
        {
            FLCarryKit.SetMyLed(0, 4, (TMyLed *)fMain->led_FL3CarryKit_0);
            FLCarryKit.SetMyLed(0, 5, (TMyLed *)fMain->led_FL4CarryKit_0);
            FLCarryKit.SetMyLed(0, 6, (TMyLed *)fMain->led_FL3CarryKit_1);
            FLCarryKit.SetMyLed(0, 7, (TMyLed *)fMain->led_FL4CarryKit_1);
            FLCarryKit.SetMyLed(1, 4, (TMyLed *)fMain->led_FL3CarryKit_2);
            FLCarryKit.SetMyLed(1, 5, (TMyLed *)fMain->led_FL4CarryKit_2);
            FLCarryKit.SetMyLed(1, 6, (TMyLed *)fMain->led_FL3CarryKit_3);
            FLCarryKit.SetMyLed(1, 7, (TMyLed *)fMain->led_FL4CarryKit_3);
        }

        FRCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_FRCarryKit_0);
        FRCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_FR2CarryKit_0);
        FRCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_FRCarryKit_1);
        FRCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_FR2CarryKit_1);
        FRCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_FRCarryKit_2);
        FRCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_FR2CarryKit_2);
        FRCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_FRCarryKit_3);
        FRCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_FR2CarryKit_3);

        if(MachineTypeChoice==Type_HT9046 ||                                    //Eliot 2009_12_25
           MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //ChungHung 20130507 add HT9045 updata for 12site 517 模擬畫面Shuttle
        {
            FRCarryKit.SetMyLed(0, 4, (TMyLed *)fMain->led_FR3CarryKit_0);
            FRCarryKit.SetMyLed(0, 5, (TMyLed *)fMain->led_FR4CarryKit_0);
            FRCarryKit.SetMyLed(0, 6, (TMyLed *)fMain->led_FR3CarryKit_1);
            FRCarryKit.SetMyLed(0, 7, (TMyLed *)fMain->led_FR4CarryKit_1);
            FRCarryKit.SetMyLed(1, 4, (TMyLed *)fMain->led_FR3CarryKit_2);
            FRCarryKit.SetMyLed(1, 5, (TMyLed *)fMain->led_FR4CarryKit_2);
            FRCarryKit.SetMyLed(1, 6, (TMyLed *)fMain->led_FR3CarryKit_3);
            FRCarryKit.SetMyLed(1, 7, (TMyLed *)fMain->led_FR4CarryKit_3);
        }

        BLCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_BLCarryKit_0);
        BLCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_BL2CarryKit_0);
        BLCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_BLCarryKit_1);
        BLCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_BL2CarryKit_1);
        BLCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_BLCarryKit_2);
        BLCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_BL2CarryKit_2);
        BLCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_BLCarryKit_3);
        BLCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_BL2CarryKit_3);

        if(MachineTypeChoice==Type_HT9046 ||                                    //Eliot 2009_12_25
           MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //ChungHung 20130507 add HT9045 updata for 12site 517 模擬畫面Shuttle
        {
            BLCarryKit.SetMyLed(0, 4, (TMyLed *)fMain->led_BL3CarryKit_0);
            BLCarryKit.SetMyLed(0, 5, (TMyLed *)fMain->led_BL4CarryKit_0);
            BLCarryKit.SetMyLed(0, 6, (TMyLed *)fMain->led_BL3CarryKit_1);
            BLCarryKit.SetMyLed(0, 7, (TMyLed *)fMain->led_BL4CarryKit_1);
            BLCarryKit.SetMyLed(1, 4, (TMyLed *)fMain->led_BL3CarryKit_2);
            BLCarryKit.SetMyLed(1, 5, (TMyLed *)fMain->led_BL4CarryKit_2);
            BLCarryKit.SetMyLed(1, 6, (TMyLed *)fMain->led_BL3CarryKit_3);
            BLCarryKit.SetMyLed(1, 7, (TMyLed *)fMain->led_BL4CarryKit_3);
        }

        BRCarryKit.SetMyLed(0, 0, (TMyLed *)fMain->led_BRCarryKit_0);
        BRCarryKit.SetMyLed(0, 1, (TMyLed *)fMain->led_BR2CarryKit_0);
        BRCarryKit.SetMyLed(0, 2, (TMyLed *)fMain->led_BRCarryKit_1);
        BRCarryKit.SetMyLed(0, 3, (TMyLed *)fMain->led_BR2CarryKit_1);
        BRCarryKit.SetMyLed(1, 0, (TMyLed *)fMain->led_BRCarryKit_2);
        BRCarryKit.SetMyLed(1, 1, (TMyLed *)fMain->led_BR2CarryKit_2);
        BRCarryKit.SetMyLed(1, 2, (TMyLed *)fMain->led_BRCarryKit_3);
        BRCarryKit.SetMyLed(1, 3, (TMyLed *)fMain->led_BR2CarryKit_3);

        if(MachineTypeChoice==Type_HT9046 ||                                    //Eliot 2009_12_25
           MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //ChungHung 20130507 add HT9045 updata for 12site 517 模擬畫面Shuttle
        {
            BRCarryKit.SetMyLed(0, 4, (TMyLed *)fMain->led_BR3CarryKit_0);
            BRCarryKit.SetMyLed(0, 5, (TMyLed *)fMain->led_BR4CarryKit_0);
            BRCarryKit.SetMyLed(0, 6, (TMyLed *)fMain->led_BR3CarryKit_1);
            BRCarryKit.SetMyLed(0, 7, (TMyLed *)fMain->led_BR4CarryKit_1);
            BRCarryKit.SetMyLed(1, 4, (TMyLed *)fMain->led_BR3CarryKit_2);
            BRCarryKit.SetMyLed(1, 5, (TMyLed *)fMain->led_BR4CarryKit_2);
            BRCarryKit.SetMyLed(1, 6, (TMyLed *)fMain->led_BR3CarryKit_3);
            BRCarryKit.SetMyLed(1, 7, (TMyLed *)fMain->led_BR4CarryKit_3);
        }
//#endif
        FTestSuck.SetMyLed(0, 0,  (TMyLed *)fMain->led_FTestSuckAa);
        FTestSuck.SetMyLed(0, 1,  (TMyLed *)fMain->led_FTestSuckAb);
        FTestSuck.SetMyLed(0, 2,  (TMyLed *)fMain->led_FTestSuckAc);
        FTestSuck.SetMyLed(0, 3,  (TMyLed *)fMain->led_FTestSuckAd);
        FTestSuck.SetMyLed(1, 0,  (TMyLed *)fMain->led_FTestSuckBa);
        FTestSuck.SetMyLed(1, 1,  (TMyLed *)fMain->led_FTestSuckBb);
        FTestSuck.SetMyLed(1, 2,  (TMyLed *)fMain->led_FTestSuckBc);
        FTestSuck.SetMyLed(1, 3,  (TMyLed *)fMain->led_FTestSuckBd);

        BTestSuck.SetMyLed(0, 0,  (TMyLed *)fMain->led_BTestSuckAa);
        BTestSuck.SetMyLed(0, 1,  (TMyLed *)fMain->led_BTestSuckAb);
        BTestSuck.SetMyLed(0, 2,  (TMyLed *)fMain->led_BTestSuckAc);
        BTestSuck.SetMyLed(0, 3,  (TMyLed *)fMain->led_BTestSuckAd);
        BTestSuck.SetMyLed(1, 0,  (TMyLed *)fMain->led_BTestSuckBa);
        BTestSuck.SetMyLed(1, 1,  (TMyLed *)fMain->led_BTestSuckBb);
        BTestSuck.SetMyLed(1, 2,  (TMyLed *)fMain->led_BTestSuckBc);
        BTestSuck.SetMyLed(1, 3,  (TMyLed *)fMain->led_BTestSuckBd);

        if(MachineTypeChoice==Type_HT9046 ||                                    //jou 981226 start for 9046
           MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //ChungHung 20130507 add HT9045 updata for 12site 517 模擬畫面Shuttle
        {
            FTestSuck.SetMyLed(0, 4,  (TMyLed *)fMain->led_FTestSuckAe);
            FTestSuck.SetMyLed(0, 5,  (TMyLed *)fMain->led_FTestSuckAf);
            FTestSuck.SetMyLed(0, 6,  (TMyLed *)fMain->led_FTestSuckAg);
            FTestSuck.SetMyLed(0, 7,  (TMyLed *)fMain->led_FTestSuckAh);
            FTestSuck.SetMyLed(1, 4,  (TMyLed *)fMain->led_FTestSuckBe);
            FTestSuck.SetMyLed(1, 5,  (TMyLed *)fMain->led_FTestSuckBf);
            FTestSuck.SetMyLed(1, 6,  (TMyLed *)fMain->led_FTestSuckBg);
            FTestSuck.SetMyLed(1, 7,  (TMyLed *)fMain->led_FTestSuckBh);

            BTestSuck.SetMyLed(0, 4,  (TMyLed *)fMain->led_BTestSuckAe);
            BTestSuck.SetMyLed(0, 5,  (TMyLed *)fMain->led_BTestSuckAf);
            BTestSuck.SetMyLed(0, 6,  (TMyLed *)fMain->led_BTestSuckAg);
            BTestSuck.SetMyLed(0, 7,  (TMyLed *)fMain->led_BTestSuckAh);
            BTestSuck.SetMyLed(1, 4,  (TMyLed *)fMain->led_BTestSuckBe);
            BTestSuck.SetMyLed(1, 5,  (TMyLed *)fMain->led_BTestSuckBf);
            BTestSuck.SetMyLed(1, 6,  (TMyLed *)fMain->led_BTestSuckBg);
            BTestSuck.SetMyLed(1, 7,  (TMyLed *)fMain->led_BTestSuckBh);
        }

        TestSocket.SetMyLed(0, 0, (TMyLed *)fMain->ledSocketAa);
        TestSocket.SetMyLed(0, 1, (TMyLed *)fMain->ledSocketAb);
        TestSocket.SetMyLed(0, 2, (TMyLed *)fMain->ledSocketAc);
        TestSocket.SetMyLed(0, 3, (TMyLed *)fMain->ledSocketAd);
        TestSocket.SetMyLed(1, 0, (TMyLed *)fMain->ledSocketBa);
        TestSocket.SetMyLed(1, 1, (TMyLed *)fMain->ledSocketBb);
        TestSocket.SetMyLed(1, 2, (TMyLed *)fMain->ledSocketBc);
        TestSocket.SetMyLed(1, 3, (TMyLed *)fMain->ledSocketBd);

        if(MachineTypeChoice==Type_HT9046 ||                                    //jou 981226 start for 9046
           MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //ChungHung 20130507 add HT9045 updata for 12site 517 模擬畫面Shuttle
        {
            TestSocket.SetMyLed(0, 4, (TMyLed *)fMain->ledSocketAe);
            TestSocket.SetMyLed(0, 5, (TMyLed *)fMain->ledSocketAf);
            TestSocket.SetMyLed(0, 6, (TMyLed *)fMain->ledSocketAg);
            TestSocket.SetMyLed(0, 7, (TMyLed *)fMain->ledSocketAh);
            TestSocket.SetMyLed(1, 4, (TMyLed *)fMain->ledSocketBe);
            TestSocket.SetMyLed(1, 5, (TMyLed *)fMain->ledSocketBf);
            TestSocket.SetMyLed(1, 6, (TMyLed *)fMain->ledSocketBg);
            TestSocket.SetMyLed(1, 7, (TMyLed *)fMain->ledSocketBh);
        }

        if(MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032)                                      //Steven 20240119 : fixed for 1032 LED display
        {
            TestSocket.SetMyLed(2, 0, (TMyLed *)fMain->ledSocketCa);
            TestSocket.SetMyLed(2, 1, (TMyLed *)fMain->ledSocketCb);
            TestSocket.SetMyLed(2, 2, (TMyLed *)fMain->ledSocketCc);
            TestSocket.SetMyLed(2, 3, (TMyLed *)fMain->ledSocketCd);
            TestSocket.SetMyLed(3, 0, (TMyLed *)fMain->ledSocketDa);
            TestSocket.SetMyLed(3, 1, (TMyLed *)fMain->ledSocketDb);
            TestSocket.SetMyLed(3, 2, (TMyLed *)fMain->ledSocketDc);
            TestSocket.SetMyLed(3, 3, (TMyLed *)fMain->ledSocketDd);
            TestSocket.SetMyLed(2, 4, (TMyLed *)fMain->ledSocketCe);
            TestSocket.SetMyLed(2, 5, (TMyLed *)fMain->ledSocketCf);
            TestSocket.SetMyLed(2, 6, (TMyLed *)fMain->ledSocketCg);
            TestSocket.SetMyLed(2, 7, (TMyLed *)fMain->ledSocketCh);
            TestSocket.SetMyLed(3, 4, (TMyLed *)fMain->ledSocketDe);
            TestSocket.SetMyLed(3, 5, (TMyLed *)fMain->ledSocketDf);
            TestSocket.SetMyLed(3, 6, (TMyLed *)fMain->ledSocketDg);
            TestSocket.SetMyLed(3, 7, (TMyLed *)fMain->ledSocketDh);
        }

        OutArmSuckBackup.SetMyLed(0, 0, (TMyLed *)fMain->ledOutArmAa);
        OutArmSuckBackup.SetMyLed(0, 1, (TMyLed *)fMain->ledOutArmAb);
        OutArmSuckBackup.SetMyLed(0, 2, (TMyLed *)fMain->ledOutArmAc);
        OutArmSuckBackup.SetMyLed(0, 3, (TMyLed *)fMain->ledOutArmAd);

        OutArmSuckBackup.SetMyLed(1, 0, (TMyLed *)fMain->ledOutArmBa);
        OutArmSuckBackup.SetMyLed(1, 1, (TMyLed *)fMain->ledOutArmBb);
        OutArmSuckBackup.SetMyLed(1, 2, (TMyLed *)fMain->ledOutArmBc);
        OutArmSuckBackup.SetMyLed(1, 3, (TMyLed *)fMain->ledOutArmBd);

        if(USE_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                          //Steven for HT1032  //JerryYang 20251218 : IN/OUT ARM支援不同模組
           USE_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                             //Ztex 2023.12.06 Add HT-1032
        {
            OutArmSuckBackup.SetMyLed(0, 4, (TMyLed *)fMain->ledOutArmAe);
            OutArmSuckBackup.SetMyLed(0, 5, (TMyLed *)fMain->ledOutArmAf);
            OutArmSuckBackup.SetMyLed(0, 6, (TMyLed *)fMain->ledOutArmAg);
            OutArmSuckBackup.SetMyLed(0, 7, (TMyLed *)fMain->ledOutArmAh);
            OutArmSuckBackup.SetMyLed(1, 4, (TMyLed *)fMain->ledOutArmBe);
            OutArmSuckBackup.SetMyLed(1, 5, (TMyLed *)fMain->ledOutArmBf);
            OutArmSuckBackup.SetMyLed(1, 6, (TMyLed *)fMain->ledOutArmBg);
            OutArmSuckBackup.SetMyLed(1, 7, (TMyLed *)fMain->ledOutArmBh);
        }

        CatchTraySuck.SetMyLed(0,0,(TMyLed *)fMain->ALed89);

        MOT[MMPlate1].SetHTrayPanel(fMain->mtPlate1);
        MOT[MMPlate2].SetHTrayPanel(fMain->mtPlate2);
        MOT[MMTrayY].SetHTrayPanel(fMain->mtLoader);
        MOT[MMTrayY_Car].SetHTrayPanel(fMain->mtLoaderBuffer);
        MOT[MMOCR].SetHTrayPanel(fMain->mtOCR);                                 //Steven 20120626 : OCR   //ChungHung 20120830 add OCR Function
        MOT[MManualTray1].SetHTrayPanel(fMain->mtFix1);
        MOT[MManualTray2].SetHTrayPanel(fMain->mtFix2);
        MOT[MManualTray3].SetHTrayPanel(fMain->mtFix3);
        MOT[MManualTray4].SetHTrayPanel(fMain->mtFix4);
        MOT[MManualTray5].SetHTrayPanel(fMain->mtFix5);
        MOT[MManualTray6].SetHTrayPanel(fMain->mtFix6);
        MOT[MMAuto1].SetHTrayPanel(fMain->mtAuto1);
        MOT[MMAuto2].SetHTrayPanel(fMain->mtAuto2);
        MOT[MMAuto3].SetHTrayPanel(fMain->mtAuto3);
        MOT[MMAuto4].SetHTrayPanel(fMain->mtAuto4);
        MOT[MMAuto5].SetHTrayPanel(fMain->mtAuto5);
        MOT[MMAuto6].SetHTrayPanel(fMain->mtAuto6);
        MOT[MMAuto1_Car].SetHTrayPanel(fMain->mtAuto1Car);
        MOT[MMAuto2_Car].SetHTrayPanel(fMain->mtAuto2Car);
        MOT[MMAuto3_Car].SetHTrayPanel(fMain->mtAuto3Car);
        MOT[MMAuto4_Car].SetHTrayPanel(fMain->mtAuto4Car);
        MOT[MMAuto5_Car].SetHTrayPanel(fMain->mtAuto5Car);
        MOT[MMAuto6_Car].SetHTrayPanel(fMain->mtAuto6Car);

        MOT[MMEmpty].SetHTrayPanel(fMain->mtEmpty);
        MOT[MMEmpty_Car].SetHTrayPanel(fMain->mtEmpty_Car);
        MOT[MMColor].SetHTrayPanel(fMain->mtColor);
        MOT[MMColor_Car].SetHTrayPanel(fMain->mtColor_Car);

        MOT[MMEmpty1].SetHTrayPanel(fMain->mtEmpty1);
        MOT[MMEmpty1_Car].SetHTrayPanel(fMain->mtEmpty1_Car);
        MOT[MMAutoCleanKit].SetHTrayPanel(fMain->tmyAutoClean);

        MOT[MInRotateKit  ].SetHTrayPanel(fMain->tmyInputRotateKit   );
        MOT[MOutRotateKit ].SetHTrayPanel(fMain->tmyOutputRotateKit  );

        //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
        //==>
        MOT[MMAOASampleTray].SetHTrayPanel(fAutoAlignment->myAlignmentTray);    //ChungHung 20210113 add for Alignment CCD
        MOT[MMAOASamplePlate].SetHTrayPanel(fAutoAlignment->myAlignmentPlate);  //ChungHung 20210113 add for Alignment CCD
        MOT[MMInArmAOATray].SetHTrayPanel(fMain->mtInArmAutoAlignmentTray);     //ChungHung 20210113 add for Alignment CCD
        MOT[MMOutArmAOATray].SetHTrayPanel(fMain->mtOutArmAutoAlignmentTray);   //ChungHung 20210113 add for Alignment CCD
        //<==
        //KenHsieh 20210813 : add CCD AUTO ALIGNMENT

        MOT[MMMagazineTary1].SetHTrayPanel(fMain->mtMagazineTray1);             //JerryYang 20221215 : add Magazine
        MOT[MMMagazineTary2].SetHTrayPanel(fMain->mtMagazineTray2);
        MOT[MMMagazineTary3].SetHTrayPanel(fMain->mtMagazineTray3);
        MOT[MMMagazineTary4].SetHTrayPanel(fMain->mtMagazineTray4);
        MOT[MMMagazineTary5].SetHTrayPanel(fMain->mtMagazineTray5);
        MOT[MMMagazineTary6].SetHTrayPanel(fMain->mtMagazineTray6);
        MOT[MMMagazineTary7].SetHTrayPanel(fMain->mtMagazineTray7);
        MOT[MMMagazineTary8].SetHTrayPanel(fMain->mtMagazineTray8);
        MOT[MMMagazineTary9].SetHTrayPanel(fMain->mtMagazineTray9);
        MOT[MMMagazineTary10].SetHTrayPanel(fMain->mtMagazineTray10);
        MOT[MMMagazineTary11].SetHTrayPanel(fMain->mtMagazineTray11);
        MOT[MMMagazineTary12].SetHTrayPanel(fMain->mtMagazineTray12);
        MOT[MMMagazineTary13].SetHTrayPanel(fMain->mtMagazineTray13);
        MOT[MMMagazineTary14].SetHTrayPanel(fMain->mtMagazineTray14);
        Cylinder[C_FixTray_FullPlace].SetSimulateCompoment(fMain->PageControl6, akTop, 460, 548);
    }
    flag=false;
}
//------------------------------------------------------------------------------
// 當換TRAY或更改點位時須呼叫
//------------------------------------------------------------------------------
void UpdateMyKitSuckDelayTimeToProd()                                           //Steven 20250319 : 針對OnDelayTime轉換包成function
{
    int max[8]={0, 0, 0, 0, 0, 0, 0, 0};
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(LastSet.iInArmVacuumDummyOnTime[i][j]<=500 && LastSet.iInArmVacuumDummyOnTime[i][j]>=max[0])
                max[0]=LastSet.iInArmVacuumDummyOnTime[i][j];
            if(LastSet.iOutArmVacuumDummyOnTime[i][j]<=500 && LastSet.iOutArmVacuumDummyOnTime[i][j]>=max[3])
                max[3]=LastSet.iOutArmVacuumDummyOnTime[i][j];

            if(LastSet.iInArmVacuumDummyOffTime[i][j]<=500 && LastSet.iInArmVacuumDummyOffTime[i][j]>=max[4])
                max[4]=LastSet.iInArmVacuumDummyOffTime[i][j];
            if(LastSet.iOutArmVacuumDummyOffTime[i][j]<=500 && LastSet.iOutArmVacuumDummyOffTime[i][j]>=max[7])
                max[7]=LastSet.iOutArmVacuumDummyOffTime[i][j];
        }
    }

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            if(LastSet.iFTestArmVacuumDummyOnTime[i][j]<=500 && LastSet.iFTestArmVacuumDummyOnTime[i][j]>=max[1])
                max[1]=LastSet.iFTestArmVacuumDummyOnTime[i][j];
            if(LastSet.iBTestArmVacuumDummyOnTime[i][j]<=500 && LastSet.iBTestArmVacuumDummyOnTime[i][j]>=max[2])
                max[2]=LastSet.iBTestArmVacuumDummyOnTime[i][j];
            if(LastSet.iFTestArmVacuumDummyOffTime[i][j]<=500 && LastSet.iFTestArmVacuumDummyOffTime[i][j]>=max[5])
                max[5]=LastSet.iFTestArmVacuumDummyOffTime[i][j];
            if(LastSet.iBTestArmVacuumDummyOffTime[i][j]<=500 && LastSet.iBTestArmVacuumDummyOffTime[i][j]>=max[6])
                max[6]=LastSet.iBTestArmVacuumDummyOffTime[i][j];
        }
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(LastSet.iInArmVacuumDummyOnTime[i][j]>=500 || LastSet.iInArmVacuumDummyOnTime[i][j]<=20)
                LastSet.iInArmVacuumDummyOnTime[i][j]=max[0];
            if(LastSet.iOutArmVacuumDummyOnTime[i][j]>=500 || LastSet.iOutArmVacuumDummyOnTime[i][j]<=20)
                LastSet.iOutArmVacuumDummyOnTime[i][j]=max[3];

            if(LastSet.iInArmVacuumDummyOffTime[i][j]>=500 || LastSet.iInArmVacuumDummyOffTime[i][j]<=20)
                LastSet.iInArmVacuumDummyOffTime[i][j]=max[4];
            if(LastSet.iOutArmVacuumDummyOffTime[i][j]>=500 || LastSet.iOutArmVacuumDummyOffTime[i][j]<=20)
                LastSet.iOutArmVacuumDummyOffTime[i][j]=max[7];
        }
    }

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            if(LastSet.iFTestArmVacuumDummyOnTime[i][j]>=500 || LastSet.iFTestArmVacuumDummyOnTime[i][j]<=20)
                LastSet.iFTestArmVacuumDummyOnTime[i][j]=max[1];
            if(LastSet.iBTestArmVacuumDummyOnTime[i][j]>=500 || LastSet.iBTestArmVacuumDummyOnTime[i][j]<=20)
                LastSet.iBTestArmVacuumDummyOnTime[i][j]=max[2];
            if(LastSet.iFTestArmVacuumDummyOffTime[i][j]>=500 || LastSet.iFTestArmVacuumDummyOffTime[i][j]<=20)
                LastSet.iFTestArmVacuumDummyOffTime[i][j]=max[5];
            if(LastSet.iBTestArmVacuumDummyOffTime[i][j]>=500 || LastSet.iBTestArmVacuumDummyOffTime[i][j]<=20)
                LastSet.iBTestArmVacuumDummyOffTime[i][j]=max[6];
        }
    }

    double dArmData[12];                                                        //kevin 20170906 (Steven) 0.03 會差 10ms
    if(bRunAutoClean==true)                                                     //Steven 20250319 : Auto Clean使用另外一組Delay Time
    {
        dArmData[0]=TestIF_File.dAutoClean_InArmVacuum*100;                     //Steven 20250410 : fixed for auto clean PnP delay
        dArmData[1]=TestIF_File.dAutoClean_InArmVacuum*100;
        dArmData[2]=TestIF_File.dAutoClean_InArmAirOn*100;
        dArmData[3]=TestIF_File.dAutoClean_InArmAirOn*100;
    }
    else
    {
        dArmData[0]=ArmSpeed[InArm].dVacuumTI*100;
        dArmData[1]=ArmSpeed[OutArm].dVacuumTI*100;
        dArmData[2]=ArmSpeed[InArm].dCTAirOn*100;
        dArmData[3]=ArmSpeed[OutArm].dCTAirOn*100;
    }
    dArmData[4]=ArmSpeed[InArm].iDestroyAgainCount;                             //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    dArmData[5]=ArmSpeed[OutArm].iDestroyAgainCount;                            //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    dArmData[6]=ArmSpeed[InArm].dDestroyAgainTime*100;                          //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
    dArmData[7]=ArmSpeed[OutArm].dDestroyAgainTime*100;                         //ChungHung 20130413 add 針對 阿凡達IC 無法Relase

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            InArmSuck.Suck[i][j].OnDelayTime        =dArmData[0];
            OutArmSuck.Suck[i][j].OnDelayTime       =dArmData[1];
            InArmSuck.Suck[i][j].OffDelayTime       =dArmData[2];
            OutArmSuck.Suck[i][j].OffDelayTime      =dArmData[3];

            InArmSuck.Suck[i][j].DestroyAgainCount  =dArmData[4];               //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
            OutArmSuck.Suck[i][j].DestroyAgainCount =dArmData[5];               //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
            InArmSuck.Suck[i][j].DestroyAgainTime   =dArmData[6];               //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
            OutArmSuck.Suck[i][j].DestroyAgainTime  =dArmData[7];               //ChungHung 20130413 add 針對 阿凡達IC 無法Relase

            InArmSuck.Suck[i][j].VacuumOnTime       =LastSet.iInArmVacuumDummyOnTime[i][j];
            OutArmSuck.Suck[i][j].VacuumOnTime      =LastSet.iOutArmVacuumDummyOnTime[i][j];
            InArmSuck.Suck[i][j].VacuumOffTime      =LastSet.iInArmVacuumDummyOffTime[i][j];
            OutArmSuck.Suck[i][j].VacuumOffTime     =LastSet.iOutArmVacuumDummyOffTime[i][j];
        }
    }

    if(bRunAutoClean==true)                                                     //Steven 20250319 : Auto Clean使用另外一組Delay Time
    {
        dArmData[8]=TestIF_File.dAutoClean_IndexVacuum*100;
        dArmData[9]=TestIF_File.dAutoClean_IndexAirOn*100;
    }
    else
    {
        dArmData[8]=ArmSpeed[IndexArm].dVacuumTI*100;
        if(TestIF_File.bEnableDelayTimeZero==true)                              //Jimmychiu 20230922 : R230824-ATK-H9-01 Request add "Air purge during place device on out-shuttle" function.
        {
            dArmData[9]=0;
        }
        else
        {
            dArmData[9]=ArmSpeed[IndexArm].dCTAirOn*100;
        }
    }
    dArmData[10]=ArmSpeed[IndexArm].iDestroyAgainCount;
    dArmData[11]=ArmSpeed[IndexArm].dDestroyAgainTime*100;

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            FTestSuck.Suck[i][j].OnDelayTime        =dArmData[8];
            BTestSuck.Suck[i][j].OnDelayTime        =dArmData[8];
            FTestSuck.Suck[i][j].OffDelayTime       =dArmData[9];
            BTestSuck.Suck[i][j].OffDelayTime       =dArmData[9];

            FTestSuck.Suck[i][j].DestroyAgainCount  =dArmData[10];              //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
            BTestSuck.Suck[i][j].DestroyAgainCount  =dArmData[10];              //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
            FTestSuck.Suck[i][j].DestroyAgainTime   =dArmData[11];              //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
            BTestSuck.Suck[i][j].DestroyAgainTime   =dArmData[11];              //ChungHung 20130413 add 針對 阿凡達IC 無法Relase

            FTestSuck.Suck[i][j].VacuumOnTime       =LastSet.iFTestArmVacuumDummyOnTime[i][j];
            BTestSuck.Suck[i][j].VacuumOnTime       =LastSet.iBTestArmVacuumDummyOnTime[i][j];
            FTestSuck.Suck[i][j].VacuumOffTime      =LastSet.iFTestArmVacuumDummyOffTime[i][j];
            BTestSuck.Suck[i][j].VacuumOffTime      =LastSet.iBTestArmVacuumDummyOffTime[i][j];
        }
    }

    CatchTraySuck.Suck[0][0].OnDelayTime            =ArmSpeed[TrayArm].dVacuumTI*100;
    CatchTraySuck.Suck[0][0].OffDelayTime           =ArmSpeed[TrayArm].dCTAirOn*100;
    CatchTraySuck.Suck[0][0].VacuumOnTime           =LastSet.iCatchArmVacuumDummyOnTime;
    CatchTraySuck.Suck[0][0].VacuumOffTime          =LastSet.iCatchArmVacuumDummyOffTime;
}
//------------------------------------------------------------------------------
void DoSetupSystemToProd()
{
    AnsiString  Str="";                                                         //kevin 20220813
    if(bUserDefMaxContactHeight==true &&
       CosFunction.bUserDefineIndexZSafePos==true)                              //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
    {
        iGalil_Z_SafePos=dUserDefineIndexZSafePos;
    }
    else
    {
        iGalil_Z_SafePos=200;
    }

    UpdateMyKitSuckDelayTimeToProd();                                           //Steven 20250319 : 針對OnDelayTime轉換包成function

    if(LastSet.iLoaderTraySimulateTime<0 || LastSet.iLoaderTraySimulateTime>20000)
        LastSet.iLoaderTraySimulateTime=3000;

    for(int i=0; i<3; i++)
    {
        if(LastSet.iUnLoaderTraySimulateTime[i]<0 || LastSet.iUnLoaderTraySimulateTime[i]>20000)
            LastSet.iUnLoaderTraySimulateTime[i]=3000;
    }
    bQualSiteQuickMode=false;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K12 && (DeviceForm.IndexDrop[0]!=0 || DeviceForm.IndexDrop[1]!=0))
    {
        Prod.TestZ1_Drop_Offset=DeviceForm.IndexDrop[0]+300;                    //kevin 20131115 上升 drop +3mm
        Prod.TestZ2_Drop_Offset=DeviceForm.IndexDrop[1]+300;                    //kevin 20131115 上升 drop +3mm
    }
    else
    {
        Prod.TestZ1_Drop_Offset=DeviceForm.IndexDrop[0];
        Prod.TestZ2_Drop_Offset=DeviceForm.IndexDrop[1];
    }

    if(DeviceForm.IndexDrop[0]==0)
    {
        if(bHPCleanout)                                                         //wei 20160624 Hotplate clean out
        {
            Prod.TestZ1_Test=-5000;                                             //ChungHung 20120725 Amkor_K 要可以記住AutoHeight的值，除非重新K高度 但選單Arm時只移動-50
        }
        else if(IniConfig.bRemeberAutoHeight==false)
        {
            if(CosFunction.bRTCHalfViewAutoVerify &&
               bHalfViewVerifyNeedAboveSocket==true &&
               REAL_TIME_CCD==true &&
               !COM2->bCCDDummyRum &&
               CosFunction.bRTCAutoModelVerify==true &&                         //JerryYang 20210128 Half View auto verify要拉高
               IniConfig.bD36EnableRTCAutoModelVerify==true)
            {
                Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0]+3000;
            }
            else
            {
                Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0];
            }
        }
        else
        {
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
            {
                Prod.TestZ1_Test=(-5000)+Offset.iIndexArmContact[0];
            }
            else
            {
                if(CosFunction.bRTCHalfViewAutoVerify &&
                   bHalfViewVerifyNeedAboveSocket==true &&
                   REAL_TIME_CCD==true &&
                   !COM2->bCCDDummyRum &&
                   CosFunction.bRTCAutoModelVerify==true &&                     //JerryYang 20210128 Half View auto verify要拉高
                   IniConfig.bD36EnableRTCAutoModelVerify==true)
                {
                    Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0]+3000;
                }
                else
                {
                    Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0];
                }
            }
        }
    }
    else
    {
        if(bHPCleanout)                                                         //wei 20160624 Hotplate clean out
        {                                                                       //ChungHung 20120725 Amkor_K 要可以記住AutoHeight的值，除非重新K高度 但選單Arm時只移動-50
            Prod.TestZ1_Test=-5000;
        }
        else if(IniConfig.bRemeberAutoHeight==false)
        {
            if(CosFunction.bRTCHalfViewAutoVerify &&
               bHalfViewVerifyNeedAboveSocket==true &&
               REAL_TIME_CCD==true &&
               !COM2->bCCDDummyRum &&
               CosFunction.bRTCAutoModelVerify==true &&                         //JerryYang 20210128 Half View auto verify要拉高
               IniConfig.bD36EnableRTCAutoModelVerify==true)
            {
                Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0]+3000;
            }
            else
            {
                Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0];
            }
        }
        else
        {
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
            {
                Prod.TestZ1_Test=(-5000)+Offset.iIndexArmContact[0];
            }
            else
            {
                if(CosFunction.bRTCHalfViewAutoVerify &&
                   bHalfViewVerifyNeedAboveSocket==true &&
                   REAL_TIME_CCD==true &&
                   !COM2->bCCDDummyRum &&
                   CosFunction.bRTCAutoModelVerify==true &&                     //JerryYang 20210128 Half View auto verify要拉高
                   IniConfig.bD36EnableRTCAutoModelVerify==true)
                {
                    Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0]+3000;
                }
                else
                {
                    Prod.TestZ1_Test=DeviceForm.IndexContact[0]+Offset.iIndexArmContact[0];
                }
            }
        }

        Prod.TestZ1_Test+=Prod.TestZ1_Drop_Offset;
    }

    if(DeviceForm.IndexDrop[1]==0)
    {
        if(bHPCleanout)                                                         //wei 20160624 Hotplate clean out
        {                                                                       //ChungHung 20120725 Amkor_K 要可以記住AutoHeight的值，除非重新K高度 但選單Arm時只移動-50
            Prod.TestZ2_Test=-5000;
        }
        else if(IniConfig.bRemeberAutoHeight==false)                            //Steven 20151116
        {
            if(CosFunction.bRTCHalfViewAutoVerify &&
               bHalfViewVerifyNeedAboveSocket==true &&
               REAL_TIME_CCD==true &&
               !COM2->bCCDDummyRum &&
               CosFunction.bRTCAutoModelVerify==true &&                         //JerryYang 20210128 Half View auto verify要拉高
               IniConfig.bD36EnableRTCAutoModelVerify==true)
            {
                Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1]+3000;
            }
            else
            {
                Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1];
            }
        }
        else
        {
            if(IniConfig.bIndexArm2SupplyLight==true ||                         //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
               TestIF_File.bForEgisTecTest==true     ||                         //Steven 20140922 : Arm2當作指紋測試
               (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                 //kevin 20150127 Arm1 下壓 arm2 測試
                TestIF_File.bArm1PickPlaceArm2Test==true))                      //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1];
            }
            else
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
                {
                    Prod.TestZ2_Test=(-5000)+Offset.iIndexArmContact[1];
                }
                else
                {
                    if(CosFunction.bRTCHalfViewAutoVerify &&
                       bHalfViewVerifyNeedAboveSocket==true &&
                       REAL_TIME_CCD==true &&
                       !COM2->bCCDDummyRum &&
                       CosFunction.bRTCAutoModelVerify==true &&                 //JerryYang 20210128 Half View auto verify要拉高
                       IniConfig.bD36EnableRTCAutoModelVerify==true)
                    {
                        Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1]+3000;
                    }
                    else
                    {
                        Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1];
                    }
                }
            }
        }
    }
    else
    {
        if(bHPCleanout)                                                         //wei 20160624 Hotplate clean out
        {                                                                       //ChungHung 20120725 Amkor_K 要可以記住AutoHeight的值，除非重新K高度 但選單Arm時只移動-50
            Prod.TestZ2_Test=-5000;
        }
        else if(IniConfig.bRemeberAutoHeight==false)
        {
            if(CosFunction.bRTCHalfViewAutoVerify &&
               bHalfViewVerifyNeedAboveSocket==true &&
               REAL_TIME_CCD==true &&
               !COM2->bCCDDummyRum &&
               CosFunction.bRTCAutoModelVerify==true &&                         //JerryYang 20210128 Half View auto verify要拉高
               IniConfig.bD36EnableRTCAutoModelVerify==true)
            {
                Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1]+3000;
            }
            else
            {
                Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1];
            }
        }
        else
        {
            if(IniConfig.bIndexArm2SupplyLight==true ||                         //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
               TestIF_File.bForEgisTecTest==true     ||                         //Steven 20140922 : Arm2當作指紋測試
               (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                 //kevin 20150127 Arm1 下壓 arm2 測試
                TestIF_File.bArm1PickPlaceArm2Test==true))                      //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
            {
                Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1];
            }
            else
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
                {
                    Prod.TestZ2_Test=(-5000)+Offset.iIndexArmContact[1];
                }
                else
                {
                    if(CosFunction.bRTCHalfViewAutoVerify &&
                       bHalfViewVerifyNeedAboveSocket==true &&
                       REAL_TIME_CCD==true &&
                       !COM2->bCCDDummyRum &&
                       CosFunction.bRTCAutoModelVerify==true &&                 //JerryYang 20210128 Half View auto verify要拉高
                       IniConfig.bD36EnableRTCAutoModelVerify==true)
                    {
                        Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1]+3000;
                    }
                    else
                    {
                        Prod.TestZ2_Test=DeviceForm.IndexContact[1]+Offset.iIndexArmContact[1];
                    }
                }
            }
        }

        Prod.TestZ2_Test+=Prod.TestZ2_Drop_Offset;
    }

    if(IniConfig.bI27_ManualSortMode &&
       LastSet.iTester==OFF_LINE &&
       bRunManualSortMode==true)                                                //Steven 20150915 : For TSMC 手動整盤功能
    {
        Prod.TestZ1_Test+=1000;
        Prod.TestZ2_Test+=1000;
    }

    if(fAutoTeach->IsRun())                                                     //JimmyChiu 20211020 : Auto alignment mode
    {
        Prod.TestZ1_Test+=fAutoTeach->GetSocketPickUpOffsetWhenAutoTeachOnly();
        Prod.TestZ2_Test+=fAutoTeach->GetSocketPickUpOffsetWhenAutoTeachOnly();
    }

    if(DeviceForm_File.dKitDiameter==8 ||                                       //Ifor 20200318 : add 鋼徑80mmn 因為機台Hold不住變形需補下壓高度
       DeviceForm_File.dKitDiameter==40.2)
    {
        double  iTotalOffset_1=0;
        double  iTotalOffset_2=0;
        int iLowLevel=0;
        int iHightLevel=0;

        if(DeviceForm_File.dPress>120)                                          //超過120才需補償
        {
            if(DeviceForm_File.dPress>dIndexZOffset[2][13])                     //760~800
            {
                iLowLevel   =13;
                iHightLevel =14;
            }
            if(DeviceForm_File.dPress>dIndexZOffset[2][12])                     //720~760
            {
                iLowLevel   =12;
                iHightLevel =13;
            }
            if(DeviceForm_File.dPress>dIndexZOffset[2][11])                     //680~720
            {
                iLowLevel   =11;
                iHightLevel =12;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][10])                //640~680
            {
                iLowLevel   =10;
                iHightLevel =11;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][9])                 //600~640
            {
                iLowLevel   =9;
                iHightLevel =10;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][8])                 //560~600
            {
                iLowLevel   =8;
                iHightLevel =9;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][7])                 //520~560
            {
                iLowLevel   =7;
                iHightLevel =8;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][6])                 //480~520
            {
                iLowLevel   =6;
                iHightLevel =7;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][5])                 //420~480
            {
                iLowLevel   =5;
                iHightLevel =6;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][4])                 //360~420
            {
                iLowLevel   =4;
                iHightLevel =5;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][3])                 //300~360
            {
                iLowLevel   =3;
                iHightLevel =4;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][2])                 //240~300
            {
                iLowLevel   =2;
                iHightLevel =3;
            }
            else if(DeviceForm_File.dPress>dIndexZOffset[2][1])                 //180~240
            {
                iLowLevel   =1;
                iHightLevel =2;
            }
            else                                                                //120~180
            {
                iLowLevel   =0;
                iHightLevel =1;
            }

            iTotalOffset_1=(dIndexZOffset[0][iLowLevel]+(((dIndexZOffset[0][iHightLevel]-dIndexZOffset[0][iLowLevel])/60)*(DeviceForm_File.dPress-dIndexZOffset[2][iLowLevel])))*100;
            iTotalOffset_2=(dIndexZOffset[1][iLowLevel]+(((dIndexZOffset[1][iHightLevel]-dIndexZOffset[1][iLowLevel])/60)*(DeviceForm_File.dPress-dIndexZOffset[2][iLowLevel])))*100;
        }
        Prod.TestZ1_Test-=iTotalOffset_1;
        Prod.TestZ2_Test-=iTotalOffset_2;
    }

    //DIO_CFG-------------------------------------------------------------------
    memcpy(&Prod.DIOCfg.cModeName[0], &TTLCfg.cModeName[0], sizeof(TTLCfg));

    for(int i=0; i<8; i++)                                                      //Alick 20161011 (Steven) : TTL支援8Site
    {
        for(int j=0; j<5; j++)
        {
            Sen[TTL_Sensor[i][j]].Type=Prod.DIOCfg.iCateLogicMode;
        }
    }

    for(int i=0; i<8; i++)                                                      //Steven 20161011 : TTL支援8Site 4 --> 8
    {
        SW[TTL_StartData[i]].Type=Prod.DIOCfg.iSTLogicMode;
    }
    //--------------------------------------------------------------------------
    Prod.TestZ_Drop_Wait=DeviceForm.DropWait;

    Prod.TestZ_Drop_Speed=DeviceForm.DropSpeed;                                 //Eliot 2011_0318
    Prod.bIndexUpSpeed=DeviceForm.bIndexUpSpeed;                                //kevin 20170524 (wei) add index up speed

    Prod.TestZ_Up_Wait=DeviceForm.UpWait;
    Prod.TestZ_Up_Speed=DeviceForm.UpSpeed;
    Prod.TestZ1_Up_Offset=DeviceForm.IndexUp[0];
    Prod.TestZ2_Up_Offset=DeviceForm.IndexUp[1];

    SetHangupMaxTime();                                                         //Wei 20230224 : 重置死機時間

    if(Prod.iHangupMaxTime<120)                                                 //jou 2011-02-23
        Prod.iHangupMaxTime=120;                                                //jou 2011-11-09 增加initial max time set

    Prod.iTesterDummyTime=TestIF.iDummyTime;
    Prod.dTesterStartDelayTime=TestIF.dStartDelayTime;

    //---------------------------------------
    //Initial delay function
    //ChungHung 20140425 add for TSMC Device
    //---------------------------------------
    Prod.bEveryFirstDeviceUseInitialDelay           =TestIF.bEveryFirstDeviceUseInitialDelay;
    Prod.bUseOtherArmToTestAfterInitialDelay        =TestIF.bUseOtherArmToTestAfterInitialDelay;    //Jerryyang 20180607 (wei) : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
    Prod.bAfterShowAlarmMessageUseInitialDelay      =TestIF.bAfterShowAlarmMessageUseInitialDelay;
    Prod.bWhenHappenTestedTimeBelowUseInitialDelay  =TestIF.bWhenHappenTestedTimeBelowUseInitialDelay;
    Prod.iEveryFirstDeviceUseInitialDelay           =TestIF.iEveryFirstDeviceUseInitialDelay;
    Prod.bAfterAutoCleanFunctionUseInitialDelay     =TestIF.bAfterAutoCleanFunctionUseInitialDelay; //ChungHung 20141017 add for SCK Add for TSMC Device
    Prod.bAfterOpenHeatDoorUseInitialDelay          =TestIF.bAfterOpenHeatDoorUseInitialDelay;      //ChungHung 20141210 add for SCK want to after Open HeatDoor have delay time
    Prod.dAfterTestedDelay                          =TestIF.dAfterTestedDelay;                      //ChungHung 20140730 add for ATK function after tested delay time
    Prod.bWhenPressStopOverUseInitialDelay          =TestIF_File.bWhenPressStopOverUseInitialDelay; //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    Prod.iWhenPressStopOver                         =TestIF_File.iWhenPressStopOver;                //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    Prod.bWhenNoFullSiteUseInitialDelay             =TestIF_File.bWhenNoFullSiteUseInitialDelay;    //wei 20151228 No FullSite delay
    Prod.bTestFinishToNextTestOver                  =TestIF_File.bTestFinishToNextTestOver;         //kevin 20160310 測試完成到下一次完成超過所設時間須啟動 delay
    Prod.bOTDUnlockDelay                            =TestIF_File.bOTDUnlockDelay;                   //Steven 20160818 : OTD打開Delay

    Prod.bTestStartToNextTestStart                  =TestIF_File.bTestStartToNextTestStart;         //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41
    Prod.dTeststartToNextTestStart                  =TestIF_File.dTeststartToNextTestStart;         //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41  over time
    Prod.dInitialDelay_10                           =TestIF_File.dInitialDelay_10;                  //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41 wait time

    Prod.bTTLUseASEJPMode                           =TestIF_File.bTTLUseASEJPMode;                  //Frank 20220408 Add TTL ASE_JP Mode
    //---------------------------------------

    if(TTLCfg.iCateBitLength==_8Bit || TTLCfg.iCateBitLength==_10Bit ||
       TTLCfg.iCateBitLength==_10BitPE || TTLCfg.iCateBitLength==_10BitPO)
    {
        SW[Sw10Bit].On();
        SW[Sw10Bit2].On();                                                      //Alick 20161011 (Steven) : TTL支援8Site
    }
    else
    {
        SW[Sw10Bit].Off();
        SW[Sw10Bit2].Off();                                                     //Alick 20161011 (Steven) : TTL支援8Site
    }

    LastSet.InitialWaitTime=Temperature.fInitialWaitTime;

    if(Temperature.fSoakTime>150)
    {
        Prod.iHotTime=Temperature.fSoakTime-5;                                  //jou 2012-03-07 Soak time > 150 Sec,UPH卡在Hotplate buffer不足，所以偷跑一秒
    }
    else
    {
        Prod.iHotTime=Temperature.fSoakTime;
    }

    iInArmPitch=3900;
    iOutArmPitch=3900;

    for(int i=0; i<2; i++)
    {
        Prod.HotPlateForm[i].iXItem =HotPlateForm.XDivision;
        Prod.HotPlateForm[i].iYItem =HotPlateForm.YDivision;
        Prod.HotPlateForm[i].iXPitch=HotPlateForm.XPitch;
        Prod.HotPlateForm[i].iYPitch=HotPlateForm.YPitch;
        Prod.HotPlateForm[i].iXEdge =HotPlateForm.XStart;
        Prod.HotPlateForm[i].iYEdge =HotPlateForm.YStart;
    }
    if(HotPlateForm.XDivision==5 || HotPlateForm.XDivision==7)
    {
        Str.sprintf("Hotplate X-Division wrong setting,加熱盤 X 數量 %d 設定錯誤", HotPlateForm.XDivision);    //kevin 20220813 show HP X alarm
        ShowMyMessage(Str);
    }

    //--------------------------------------------------------------------------
    //QA Mode
    //--------------------------------------------------------------------------
    Prod.iQAModeCount           =TestIF.iQAModeCount;
    Prod.iQAModeRunType         =TestIF.iQAModeRunType;                         //Steven 20120612 : QA做完後的動作
    Prod.bLowYieldAlarmByBin    =TestIF.bLowYieldAlarmByBin;                    //Steven 20140828 : By Bin Yield Monitor
    Prod.iQAModeBin             =TestIF.iQAModeBin;                             //Steven 20141023 : QA做完後的Bin
    Prod.bQAModeAfterTrayEnd    =TestIF.bQAModeAfterTrayEnd;                    //Steven 20151125 : QA做完後的TrayEnd要重做QA

    if(CUSTOMER_CODE==CC_AnalogDevice_Phil)
    {
        Prod.bQAModeAfterTrayEnd=false;
    }

    if(Prod.iQAModeRunType==3)                                                  //Sam 20240104 : 新增 QA 不計數模式
        Prod.iQAModeCount=99999;

    if(LastSet.iRunStartMode==rsmQAMode)                                        //Sam 20231117 : 整合到 QA 模式
    {
        Prod.bD22SupportMultiDoubleContact=TestIF.bQAD22DoubleContact;
        Prod.iD22DoubleContactCount=TestIF.iQAD22DoubleContactCount;            //Sam 20240104 : 新增 QA 不計數模式
    }
    else
    {
        Prod.bD22SupportMultiDoubleContact=IniConfig.bD22SupportMultiDoubleContact;
        Prod.iD22DoubleContactCount=IniConfig.iD22DoubleContactCount;           //Sam 20240104 : 新增 QA 不計數模式
    }

    Prod.iQASamplingT6Pos=iTo6Unload[TestIF_File.iQASamplingT3Pos];
    if(TestIF_File.iQASamplingT3Pos>0)
        Prod.iQASamplingT6=iTo6Unload[TestIF_File.iQASamplingT3Pos-1];
    else
        Prod.iQASamplingT6=eFix1;

    bool bRunRT=false;                                                          //Steven 20170711 (Wei) : 修正SCK_ART的Yield Alarm
    if(iRunStartMode==FT || iRunStartMode==FT_ART)                              //ChungHung 20141002 add for KYEC AutoRetest
    {
        bRunRT=false;

        if(CosFunction.bUseSCKART==true  &&
           (LastSet.iRunStartMode==rsmInitial_ART ||
            LastSet.iRunStartMode==rsmContinuStart_ART ||
            LastSet.iRunStartMode==rsmContinuRetest_ART ||                      //JerryYang 20210308 :add RT
            LastSet.iRunStartMode==rsmAutoRetest) &&                            //Sam 20240701 : 修正 ART 時在搬運時 LowYield 誤報警
           fSCKART->iFTRTCount!=0)
        {
            bRunRT=true;
        }
    }
    else
    {
        bRunRT=true;
    }

    Prod.bContsFailBySocket                 =TestIF.bContsFailBySocket;
    Prod.iContsFailSocketAlarmCT            =TestIF.iContsFailSocketAlarmCT;
    Prod.bCountSpcBinContinuously           =TestIF.bCountSpcBinContinuously_FT;    //Steven 20230529 : Spc Bin Couont改成連續錯誤

    Prod.bContsFailByHead                   =TestIF.bContsFailByHead;
    Prod.iContsFailHeadAlarmCT              =TestIF.iContsFailHeadAlarmCT;

    Prod.bFailAlarmSiteYieldDifferent       =TestIF.bFailAlarmSiteYieldDifferent;
    Prod.dFailAlarmSiteYield                =TestIF.dFailAlarmSiteYield;            //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
    Prod.iFailAlarmSiteYieldDifferentCount  =TestIF.iFailAlarmSiteYieldDifferentCount;

    Prod.bFailAlarmSiteYieldCmp             =TestIF.bFailAlarmSiteYieldCmp;
    Prod.dFailAlarmSiteYieldCmp             =TestIF.dFailAlarmSiteYieldCmp;         //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
    Prod.iFailAlarmSiteYieldCmpCount        =TestIF.iFailAlarmSiteYieldCmpCount;

    Prod.bFailAlarmLowYield                 =TestIF.bFailAlarmLowYield;
    Prod.dLowYieldLimit                     =TestIF.dLowYieldLimit;                 //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
    Prod.iLowYieldCount                     =TestIF.iLowYieldCount;

    Prod.bFailRateMode                      =TestIF.bFailRateMode;
    Prod.iIgnoreIC                          =TestIF.iIgnoreIC;

    Prod.bContinuousPass                    =TestIF.bContinuousPass;                //Eliot 20100708
    Prod.iContinuousPassBin                 =TestIF.iContinuousPassBin;             //Eliot 20100708
    Prod.iContinuousPassBinCount            =TestIF.iContinuousPassBinCount;        //Eliot 20100708
    Prod.iCountAlarmAction                  =TestIF.iCountAlarmAction;              //Steven 20101116

    Prod.bContinuousPassBySocket            =TestIF.bContinuousPassBySocket;        //Steven 20110915
    Prod.iContinuousPassBinCountBySocket    =TestIF.iContinuousPassBinCountBySocket;//Steven 20110915

    Prod.bFailAlarmLowYieldByTotal          =TestIF.bFailAlarmLowYieldByTotal;      //wei 20151116 Low Yield By Total
    Prod.dLowYieldLimitByTotal              =TestIF.dLowYieldLimitByTotal;          //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
    Prod.iLowYieldCountByTotal              =TestIF.iLowYieldCountByTotal;

    Prod.bLowYieldByPicker                  =TestIF.bLowYieldByPicker;              //Steven 20230223 : 根據Index吸嘴比較良率
    Prod.iLowYieldCountByPicker             =TestIF.iLowYieldCountByPicker;
    Prod.dLowYieldByPicker                  =TestIF.dLowYieldByPicker;

    if(CosFunction.bSpecailLowYeild && iRunStartMode==FT)                           //Sam 20210505 : PTI 要求的兩段 Low Yeild
    {
        Prod.bFailAlarmLowYieldSpecial      =TestIF.bFailAlarmLowYieldSpecial;
        Prod.dLowYieldLimitSpecial          =TestIF.dLowYieldLimitSpecial;
        Prod.iLowYieldCountSpecial1         =TestIF.iLowYieldCountSpecial1;
        Prod.iLowYieldCountSpecial2         =TestIF.iLowYieldCountSpecial2;
    }
    else
    {
        Prod.bFailAlarmLowYieldSpecial=false;
    }

    Prod.bErrOverLmt                        =TestIF.bErrOverLmt;
    Prod.dErrOverLmt                        =TestIF.dErrOverLmt;                    //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
    Prod.iErrOverLmtIg                      =TestIF.iErrOverLmtIg;

    Prod.bContsFailIgnore                   =TestIF.bContsFailIgnore;               //wei 20160115 銅鑼前幾顆不計算ContsFail
    Prod.iContsFailIgnore                   =TestIF.iContsFailIgnore;               //wei 20160115 銅鑼前幾顆不計算ContsFail
    Prod.bAllSiteFail                       =TestIF.bAllSiteFail;                   //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開
    Prod.iAllSiteFailCount                  =TestIF.iAllSiteFailCount;              //Steven 20230118 : All site fail RT

    Prod.bFailAlarmIntervalLowYieldBySite   =TestIF.bFailAlarmIntervalLowYieldBySite;//wei 20180606 Interval Low Yield By Site
    Prod.dIntervalLowYieldLimitBySite       =TestIF.dIntervalLowYieldLimitBySite;   //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
    Prod.iIntervalLowYieldCountBySite       =TestIF.iIntervalLowYieldCountBySite;

    Prod.bFailAlarmIntervalLowYieldByTotal  =TestIF.bFailAlarmIntervalLowYieldByTotal;//wei 20180718 Interval Low Yield By Total
    Prod.dIntervalLowYieldLimitByTotal      =TestIF.dIntervalLowYieldLimitByTotal;  //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
    Prod.iIntervalLowYieldCountByTotal      =TestIF.iIntervalLowYieldCountByTotal;

    //Sam 20200507 : TestIF_File > Prod
    //==>
    Prod.bAlarm4ContinueType_Enable         =TestIF.bAlarm4ContinueType_Enable;
    Prod.iAlarm4ContinueType_IntervalCount  =TestIF.iAlarm4ContinueType_IntervalCount;
    Prod.iAlarm4ContinueType_ContinueCount  =TestIF.iAlarm4ContinueType_ContinueCount;

    Prod.bAlarm4EnableIntervalYield         =TestIF.bAlarm4EnableIntervalYield;
    Prod.iAlarm4IntervalYieldIntervalCount  =TestIF.iAlarm4IntervalYieldIntervalCount;
    Prod.iAlarm4IntervalYieldContinueCount  =TestIF.iAlarm4IntervalYieldContinueCount;
    Prod.iAlarm4IntervalYieldYield          =TestIF.iAlarm4IntervalYieldYield;

    Prod.bSiteToSiteYieldCmp                =TestIF.bSiteToSiteYieldCmp;
    Prod.iSiteToSiteYieldCmp                =TestIF.iSiteToSiteYieldCmp;
    Prod.iSiteToSiteYieldCmpCount           =TestIF.iSiteToSiteYieldCmpCount;

    Prod.bHeadToHeadYieldCmp                =TestIF.bHeadToHeadYieldCmp;
    Prod.iHeadToHeadYieldCmp                =TestIF.iHeadToHeadYieldCmp;
    Prod.iHeadToHeadYieldCmpCount           =TestIF.iHeadToHeadYieldCmpCount;

    Prod.bSiteYieldOverAlert                =TestIF.bSiteYieldOverAlert;
    Prod.iSiteYieldOverAlert                =TestIF.iSiteYieldOverAlert;
    Prod.iSiteYieldOverAlertCount           =TestIF.iSiteYieldOverAlertCount;

    for(int i=0; i<iTestBinCount; i++)                                          //Sam 20250418 : 修正 By BinSetting LowYield
    {
        Prod.bLowYield[i]=BinSelect[iTestRunMode].bLowYield[i];
    }

    if(CosFunction.bAdaptiveYield)                                                          //Sam 20230914 : 自適應性良率監控
    {
        Prod.bAdaptiveLowYield                  =TestIF.bAdaptiveLowYield;
        Prod.iAdaptiveContsLowerAlarmNor        =TestIF.iAdaptiveContsLowerAlarmNor;        //Sam 20240726 : AI Clean
        Prod.iAdaptiveContsLowerAlarmMin        =TestIF.iAdaptiveContsLowerAlarmMin;
        Prod.iAdaptiveYieldMax                  =TestIF.iAdaptiveYieldMax;
        Prod.iAdaptiveYieldMin                  =TestIF.iAdaptiveYieldMin;
    }

    if(bRunRT)                                                                              //Steven 20170711 (Wei) : 修正SCK_ART的Yield Alarm
    {
        Prod.bContsFailBySocket                 =TestIF.bContsFailBySocket_RT;
        Prod.iContsFailSocketAlarmCT            =TestIF.iContsFailSocketAlarmCT_RT;
        Prod.bCountSpcBinContinuously           =TestIF_File.bCountSpcBinContinuously_RT;   //Steven 20230529 : Spc Bin Couont改成連續錯誤

        Prod.bContsFailByHead                   =TestIF.bContsFailByHead_RT;
        Prod.iContsFailHeadAlarmCT              =TestIF.iContsFailHeadAlarmCT_RT;

        Prod.bFailAlarmSiteYieldDifferent       =TestIF.bFailAlarmSiteYieldDifferent_RT;
        Prod.dFailAlarmSiteYield                =TestIF.dFailAlarmSiteYield_RT;             //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
        Prod.iFailAlarmSiteYieldDifferentCount  =TestIF.iFailAlarmSiteYieldDifferentCount_RT;

        Prod.bFailAlarmSiteYieldCmp             =TestIF.bFailAlarmSiteYieldCmp_RT;
        Prod.dFailAlarmSiteYieldCmp             =TestIF.dFailAlarmSiteYieldCmp_RT;          //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
        Prod.iFailAlarmSiteYieldCmpCount        =TestIF.iFailAlarmSiteYieldCmpCount_RT;

        Prod.bFailAlarmLowYield                 =TestIF.bFailAlarmLowYield_RT;
        Prod.dLowYieldLimit                     =TestIF.dLowYieldLimit_RT;                  //JerryYang 20160530 LowYieldLimit要能設定到小數點
        Prod.iLowYieldCount                     =TestIF.iLowYieldCount_RT;

        Prod.bFailRateMode                      =TestIF.bFailRateMode_RT;
        Prod.iIgnoreIC                          =TestIF.iIgnoreIC_RT;

        Prod.bContinuousPass                    =TestIF.bContinuousPass_RT;                 //Eliot 20100708
        Prod.iContinuousPassBin                 =TestIF.iContinuousPassBin_RT;              //Eliot 20100708
        Prod.iContinuousPassBinCount            =TestIF.iContinuousPassBinCount_RT;         //Eliot 20100708
        Prod.iCountAlarmAction                  =TestIF.iCountAlarmAction_RT;               //Steven 20101116

        Prod.bContinuousPassBySocket            =TestIF.bContinuousPassBySocket_RT;         //Steven 20110915
        Prod.iContinuousPassBinCountBySocket    =TestIF.iContinuousPassBinCountBySocket_RT; //Steven 20110915

        //wei 20151116 Low Yield By Total
        Prod.bFailAlarmLowYieldByTotal          =TestIF.bFailAlarmLowYieldByTotal_RT;
        Prod.dLowYieldLimitByTotal              =TestIF.dLowYieldLimitByTotal_RT;           //JerryYang 20160530 LowYieldLimit要能設定到小數點
        Prod.iLowYieldCountByTotal              =TestIF.iLowYieldCountByTotal_RT;

        //Steven 20230223 : 根據Index吸嘴比較良率
        Prod.bLowYieldByPicker                  =TestIF.bLowYieldByPicker_RT;
        Prod.iLowYieldCountByPicker             =TestIF.iLowYieldCountByPicker_RT;
        Prod.dLowYieldByPicker                  =TestIF.dLowYieldByPicker_RT;

        Prod.bContsFailIgnore                   =TestIF.bContsFailIgnore_RT;                //wei 20160115 銅鑼前幾顆不計算ContsFail
        Prod.iContsFailIgnore                   =TestIF.iContsFailIgnore_RT;                //wei 20160115 銅鑼前幾顆不計算ContsFail
        Prod.bAllSiteFail                       =TestIF.bAllSiteFail_RT;                    //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開
        Prod.iAllSiteFailCount                  =TestIF.iAllSiteFailCountRT;                //Steven 20230118 : All site fail RT

        //wei 20180606 Interval Low Yield By Site
        Prod.bFailAlarmIntervalLowYieldBySite   =TestIF.bFailAlarmIntervalLowYieldBySite_RT;
        Prod.dIntervalLowYieldLimitBySite       =TestIF.dIntervalLowYieldLimitBySite_RT;    //JerryYang 20160530 LowYieldLimit要能設定到小數點
        Prod.iIntervalLowYieldCountBySite       =TestIF.iIntervalLowYieldCountBySite_RT;

        //wei 20180718 Interval Low Yield By Total
        Prod.bFailAlarmIntervalLowYieldByTotal  =TestIF.bFailAlarmIntervalLowYieldByTotal_RT;
        Prod.dIntervalLowYieldLimitByTotal      =TestIF.dIntervalLowYieldLimitByTotal_RT;   //JerryYang 20160530 LowYieldLimit要能設定到小數點
        Prod.iIntervalLowYieldCountByTotal      =TestIF.iIntervalLowYieldCountByTotal_RT;

        if(CUSTOMER_CODE==CC_TERAPOWER)                                                     //Sam 20200507 : 晶兆成英傑要求 ART 時不要卡控 Alarm4 Yield
        {
            Prod.bAlarm4ContinueType_Enable =false;
            Prod.bAlarm4EnableIntervalYield =false;
            Prod.bSiteToSiteYieldCmp        =false;
            Prod.bHeadToHeadYieldCmp        =false;
            Prod.bSiteYieldOverAlert        =false;
        }

        if(CosFunction.bAdaptiveYield)                                                      //Sam 20230914 : 自適應性良率監控
        {
            Prod.bAdaptiveLowYield              =TestIF.bAdaptiveLowYield_RT;
            Prod.iAdaptiveContsLowerAlarmNor    =TestIF_File.iAdaptiveContsLowerAlarmNor_RT;//Sam 20240726 : AI Clean
            Prod.iAdaptiveContsLowerAlarmMin    =TestIF_File.iAdaptiveContsLowerAlarmMin_RT;
            Prod.iAdaptiveYieldMax              =TestIF_File.iAdaptiveYieldMax_RT;
            Prod.iAdaptiveYieldMin              =TestIF_File.iAdaptiveYieldMin_RT;
        }
    }

    if(TestIF_File.iShuttleMode==1)                                             //20111114  Dell for Disable Index Arm    Start
    {
        if(IniConfig.bShuttleMode50)                                            //20111212  Dell
        {
            //不做改變
        }
        else if(IniConfig.bIndexArm2SupplyLight==true ||                        //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
                TestIF_File.bForEgisTecTest==true     ||                        //Steven 20140922 : Arm2當作指紋測試
                (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                //kevin 20150127 Arm1 下壓 arm2 測試
                 TestIF_File.bArm1PickPlaceArm2Test==true))                     //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
        {
            //不做改變
        }
        else if(TestIF_File.iShuttle_Sel==0)
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //Ifor 20170509 (wei) 新增KYEC回Home後最小上升高度100
            {
                if(MachineTypeChoice==Type_HT9046_LS)                           //Ifor 20180822 (Steven) : Add KYEC LS 機型回Home後最小上升高度200
                {
                    Prod.TestZ2_Safe=iGalil_Z_SafePos;
                }
                else
                {
                    Prod.TestZ2_Safe=iGalil_Z_KYEC_SafePos;
                }
            }
            else
            {
                Prod.TestZ2_Safe    =iGalil_Z_SafePos;
            }
            Prod.TestY2_Middle  =0;
            Prod.TestZ2_Test    =0;
        }
        else
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //Ifor 20170509 (wei) 新增KYEC回Home後最小上升高度100
            {
                if(MachineTypeChoice==Type_HT9046_LS)                           //Ifor 20180822 (Steven) : Add KYEC LS 機型回Home後最小上升高度200
                {
                    Prod.TestZ1_Safe=iGalil_Z_SafePos;
                }
                else
                {
                    Prod.TestZ1_Safe=iGalil_Z_KYEC_SafePos;
                }
            }
            else
            {
                Prod.TestZ1_Safe    =iGalil_Z_SafePos;
            }

            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                             //JimmyChiu 20220708 : add Index Arm Axis
            {
                Prod.TestY1_Middle=0;
            }
            Prod.TestZ1_Test    =0;
        }
    }
    ////20111114  Dell for Disable Index Arm    End

    //wei 20141201 Low Yield Auto Clean(%) start
    Prod.bFailAlarmLowYield_AutoClean                 =TestIF.bFailAlarmLowYield_AutoClean;
    Prod.iLowYieldLimit_AutoClean                     =TestIF.iLowYieldLimit_AutoClean;
    Prod.iLowYieldCount_AutoClean                     =TestIF.iLowYieldCount_AutoClean;
    //wei 20141201 Low Yield Auto Clean(%) end
    if(IniConfig.bSPILFunction==true)                                           //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
        Prod.bF18InshuttleDetect=TestIF_File.bF18InshuttleDetect;
    else
        Prod.bF18InshuttleDetect=IniConfig.bF18InshuttleDetect;

    if((TestIF.iTestMode==SingleSite || iInArmType==e9045_1x4_1_Ac) &&
       (DeviceForm.XDimension+20>6000 ||
        TestIF_File.bSingleUseOtherSuck ||
        TestIF_File.bSingleInArmUseOtherSuck))                                  //wei 20220823 Single Site使用C吸嘴判斷
    {
        Prod.bSingleUseOtherSuck     =TestIF_File.bSingleUseOtherSuck;
        Prod.bSingleInArmUseOtherSuck=TestIF_File.bSingleInArmUseOtherSuck;     //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    }

    if(CosFunction.bAutoSiteMappingSetOpenBIN==true)                            //Steven 20230213 : [I21-9]的OS Bin跟著工作檔
    {
        Prod.iOpenBin=TestIF_File.iOpenBin;
        fMain->SetOpenBin();
    }

    //jou 20231122 : Rotate Use RT mode
    if(CosFunction.bRotateUseRTmode==true && tRotate.bRotateUseRTmode==true && bRunRT==true)    //RT
    {
        Prod.RotationTimeIn=tRotate.RotationTimeInRT;
        Prod.RotationTimeOut=tRotate.RotationTimeOutRT;

        for(int i=0; i<8; i++)
        {
            Prod.RotationCount[i]=tRotate.RotationCountRT[i];
            Prod.OutRotationCount[i]=tRotate.OutRotationCountRT[i];
        }

        if(USE_ROTATE_KIT==1 &&
           (iRotate_Type==e1MotRotate1Dut ||                                    //Steven 20250801 : for rotator
            iRotate_Type==e2MotRotate2Dut))
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.RotateDutDate[0][i][j]=0;
                    Prod.RotateDutDate[1][i][j]=0;
                }
            }
        }
        else                                                                    //RogerYang 20250815 : 修正RotateDutDate在其他模式下沒資料
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.RotateDutDate[0][i][j]=tRotate.RotateDutDate[2][i][j];
                    Prod.RotateDutDate[1][i][j]=tRotate.RotateDutDate[3][i][j];
                }
            }
        }
    }
    else    //FT
    {
        Prod.RotationTimeIn=tRotate.RotationTimeIn;
        Prod.RotationTimeOut=tRotate.RotationTimeOut;

        for(int i=0; i<8; i++)
        {
            Prod.RotationCount[i]=tRotate.RotationCount[i];
            Prod.OutRotationCount[i]=tRotate.OutRotationCount[i];
        }

        if(USE_ROTATE_KIT==1 &&
           (iRotate_Type==e1MotRotate1Dut ||                                    //Steven 20250801 : for rotator
            iRotate_Type==e2MotRotate2Dut))
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.RotateDutDate[0][i][j]=0;
                    Prod.RotateDutDate[1][i][j]=0;
                }
            }
        }
        else                                                                    //RogerYang 20250815 : 修正RotateDutDate在其他模式下沒資料
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<8; j++)
                {
                    Prod.RotateDutDate[0][i][j]=tRotate.RotateDutDate[0][i][j];
                    Prod.RotateDutDate[1][i][j]=tRotate.RotateDutDate[1][i][j];
                }
            }
        }
    }

    //Ifor 20210911 add :KLT 要求單排強制開啟F18功能
    //==>
    if(CUSTOMER_CODE==CC_KYEC_LEE && bEnable_KLT_Function==true)
    {
        if(TestIF_File.iTestMode==SingleSite    ||
           TestIF_File.iTestMode==DualSite      ||
           TestIF_File.iTestMode==TriSite1X3    ||
           TestIF_File.iTestMode==QualSite1X4    )
        {
            Prod.bF18InshuttleDetect=true;
        }
        //Ifor 20211104 add: KLT 要求ATC 需寫死參數
        //==>
        if(Temperature.bATCActiveCooling==true)
        {
            if(TestIF_File.iTestMode==SingleSite)
            {
                DeviceForm_File.iHeadDeviceCT=5;
                DeviceForm_File.dKitDiameter=6.0;
            }
            else if(TestIF_File.iTestMode==DualSite)
            {
                DeviceForm_File.iHeadDeviceCT=2;
                DeviceForm_File.dKitDiameter=6.0;
            }
            else if(TestIF_File.iTestMode==QualSite2X2)
            {
                DeviceForm_File.iHeadDeviceCT=2;
                DeviceForm_File.dKitDiameter=4.0;
            }
        }
        //<==
        //Ifor 20211104 add: KLT 要求ATC 需寫死參數
        //Ifor 20211117 add: KLT 要求Auto SKIP 功能By 產品大小開啟
        //==>
        if(UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1 || DeviceForm.XDimension>=3000)
        {
            ArmSpeed_File[InArm].bAutoSKIP=false;
        }
        else
        {
            ArmSpeed_File[InArm].bAutoSKIP=true;
        }
        //<==
        //Ifor 20211117 add: KLT 要求Auto SKIP 功能By 產品大小開啟
        if(FormHS!=NULL)
            FormHS->CheckEPRange();
    }
    //<==
    //Ifor 20210911 add :KLT 要求單排強制開啟F18功能

    Prod.bUseSocketHeating=TestIF_File.bUseSocketHeating;                       //Ztex 2024.09.07 Add Use Socket Heating
    Prod.iUseSocketHeating=TestIF_File.iUseSocketHeating;                       //Ztex 2024.09.07 Add Use Socket Heating

    fTrayAssignment->ReadFile();                                                //Ifor 20221221 add:E71功能開關需要讀回檔案
}
//------------------------------------------------------------------------------
void SetHangupMaxTime()                                                         //Wei 20230224 : 重置死機時間
{
    if(bInitialMaxTime==true)
    {
        Prod.iHangupMaxTime =TestIF.iInitialMaxTime*2+10;                       //jou 2011-04-01

        if(Prod.iHangupMaxTime>=300)                                            //jou 2013-07-08
            Prod.iHangupMaxTime=TestIF.iInitialMaxTime+60;                      //jou 2013-07-08
    }
    else
    {
        Prod.iHangupMaxTime =TestIF.iMaxTime*2+10;                              //jou 2011-04-01

        if(Prod.iHangupMaxTime>=300)                                            //jou 2013-07-08
            Prod.iHangupMaxTime=TestIF.iMaxTime+60;                             //jou 2013-07-08
    }

    if(Prod.iHangupMaxTime<120)                                                 //JerryYang 20240111 : 避免時間太短誤發hang up
        Prod.iHangupMaxTime=120;
}
//------------------------------------------------------------------------------
void InitialMachine()
{
    MOT[MMTrayY].fHasTray=false;
    MOT[MMOCR].fHasTray=false;                                                  //Steven 20120626 : OCR      //ChungHung 20120830 add OCR Function
    MOT[MMTrayY_Car].fHasTray=false;
    MOT[MMPlate1].fHasTray=true;
    MOT[MMPlate2].fHasTray=true;
    MOT[MMPlate1].InitEmptyTray(__FUNC__);
    MOT[MMPlate2].InitEmptyTray(__FUNC__);
    MOT[MMHot1RecBuf].fHasTray=true;
    MOT[MMHot2RecBuf].fHasTray=true;
    MOT[MMHot1RecBuf].InitEmptyTray(__FUNC__);
    MOT[MMHot2RecBuf].InitEmptyTray(__FUNC__);
    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]==tTrayAuto)
        {
            MOT[iMMAuto[i]].fHasTray=false;
            MOT[iMMAuto_Car[i]].fHasTray=false;
        }
        else if(Prod.iTrayType[i]==tTrayFix)
        {
            ClearFixTray(i, "InitialMachine");                                  //Steven 20160414 : 整合Fix盤設定
        }
    }
    FLCarryKit.ClearAll();
    FRCarryKit.ClearAll();
    BLCarryKit.ClearAll();
    BRCarryKit.ClearAll();
    FTestSuck.ClearAll();
    BTestSuck.ClearAll();
    InArmSuck.ClearAll();
    OutArmSuck.ClearAll();
    TestSocket.ClearAll();
    CatchTraySuck.ClearAll();
    CheckKitSuck.ClearAll();

    MOT[MMAutoCleanKit].fHasTray=true;                                          //jou 2012-05-21 Auto Clean
    MOT[MMAutoCleanKit].InitEmptyTray(__FUNC__);                                //jou 2012-05-21 Auto Clean

    MOT[MInRotateKit].fHasTray=true;                                            //jou 2013-03-01 Rotate kit
    MOT[MInRotateKit].InitEmptyTray(__FUNC__);                                  //jou 2013-03-01 Rotate kit
    MOT[MOutRotateKit].fHasTray=true;                                           //jou 2013-03-01 Rotate kit
    MOT[MOutRotateKit].InitEmptyTray(__FUNC__);                                 //jou 2013-03-01 Rotate kit

    if(CosFunction.bShowHPICCount)                                              //Steven 20221228 : 計算加熱盤IC數量
    {
        fSortCT->pnlLoad->Caption=MOT[MMTrayY].Tray.HowManyIC();
        fSortCT->pnlHP1->Caption =MOT[MMPlate1].Tray.HowManyIC();
        fSortCT->pnlHP2->Caption =MOT[MMPlate2].Tray.HowManyIC();
    }
}
//------------------------------------------------------------------------------
struct MachineRecord
{
    bool bInitialStart;
    int  Start;
    bool fHasTray[20];
    int  TrayData[20][MAX_X_ITEM][MAX_Y_ITEM];
    int  TrayXItem[20];
    int  TrayYItem[20];
    int  FLCarryKitItem[4][8];
    int  FRCarryKitItem[4][8];
    int  BLCarryKitItem[4][8];
    int  BRCarryKitItem[4][8];
    int  FTestSuckItem[4][8];
    int  BTestSuckItem[4][8];
    int  InArmSuckItem[4][8];
    int  OutArmSuckItem[4][8];
    int  TestSocketItem[4][8];
    int  CatchTrayItem;
    int  iWhichSht;
    int  iWhichKit;
    int  iInArmOrder;
    int  iPickPlate[2];
    int  iPlatePickX[2];
    int  iPlatePickY[2];
    int  iPlacePlate[2];
    int  iPlatePlaceX[2];
    int  iPlatePlaceY[2];
    int  iBackInArmHotCount;
    int  iHotCount;
    int  iHotPlateCount[2][50][50];
    int  iHotInArmOrder[2][50][50];
    int  iHotWhichKit[2][50][50];
    int  iHotWhichShuttle[2][50][50];
    int  iInRotateUnit;
    int  iOutRotateUnit;

    bool bPlaceToHotplate;
    bool bPickFromHotplate;

//jou 2011-03-24 start : Auto Site Mapping
    bool bSiteMappingCHKOK;
    int iKyecSiteMapStatus[32];
//jou 2011-03-24 end

    bool bBackupCleanOut;                                                       //jou 2011-07-06 : 改成任何狀況下,都要紀錄clean out模式,不然會hang up

    int iHotRecBuf[2][50][50];                                                  //jou 2011-12-26 加入記憶尚未完成吸取的位置

    int LoaderBuf[MAX_X_ITEM][MAX_Y_ITEM];
    int iRowOnHotPlate[2][50][50];                                              //JerryYang 20180718 (wei) : 放料至hot plate記錄吸嘴位置
    int  iWhichSite[20][MAX_X_ITEM][MAX_Y_ITEM];                                //Steven 20220602 : for Auto Site Map
    //JerryYang 20220909 : add magazine
    //==>
    bool fHasMagazineTray[14];
    int  MagazineTrayData[14][MAX_X_ITEM][MAX_Y_ITEM];
    int  MagazineTrayXItem[20];
    int  MagazineTrayYItem[20];
    int  iAuto3MagazineIndex;
    int  iBinData[3][MAX_X_ITEM][MAX_Y_ITEM];
    int  iWhichAuto[3][MAX_X_ITEM][MAX_Y_ITEM];
    //<==
    //JerryYang 20220909 : add magazine

    bool fHasTray_6[9];                                                         //Auto 4,5,6 Auto 4,5,6_CAR Fix 4,5,6
    int  TrayData_6[9][MAX_X_ITEM][MAX_Y_ITEM];
    int  TrayXItem_6[9];
    int  TrayYItem_6[9];
    int  iWhichSite_6[9][MAX_X_ITEM][MAX_Y_ITEM];
    int  iBinData_6[3][MAX_X_ITEM][MAX_Y_ITEM];                                 //Fix 4,5,6
    int  iWhichAuto_6[3][MAX_X_ITEM][MAX_Y_ITEM];

}MachRec;

extern int iPickPlate[2],  iPickPlateX[2],  iPickPlateY[2];
extern int iPlacePlate[2], iPlacePlateX[2], iPlacePlateY[2];
//------------------------------------------------------------------------------
AnsiString asUnloaderInfoFilePath[eTrayCount];                                  //ChungHung 20150205 add for ATK
void SetUnloaderInfoFile(int iUnloader)                                         //ChungHung 20150205 add for ATK
{
    AnsiString aDir, aPath="D:\\UnloaderInfo";
    MyForceDirectories(aPath, "SetUnloaderInfoFile_1");

    aDir.sprintf("%s\\%s", aPath, FormatDateTime("dd_mm_yyyy", Now()));
    MyForceDirectories(aPath, "SetUnloaderInfoFile_2");

    asUnloaderInfoFilePath[iUnloader].sprintf("%s\\%s_%s.dat", aDir, s6TrayName[iUnloader], FormatDateTime("_hh_mm_AM/PM", Now()));
    return;
}
//------------------------------------------------------------------------------
void LoadUnloaderInfo()                                                         //ChungHung 20150205 add for ATK
{
    AnsiString aFile, aPath="D:\\UnloaderInfo";
    aFile.sprintf("%s\\LastTrayInfo.txt", aPath);
    TStringList *ReportList = new TStringList;

    if(!FileExists(aFile))
    {
        for(int i=0; i<eTrayCount; i++)
        {
            if(Prod.iTrayType[i]!=tNotUse)
            {
                SetUnloaderInfoFile(i);
                ReportList->Add(asUnloaderInfoFilePath[i]);
            }
        }
        ReportList->SaveToFile(aFile);
    }

    ReportList->LoadFromFile(aFile);

    for(int i=0; i<ReportList->Count; i++)
    {
        aFile=ReportList->Strings[i];
        asUnloaderInfoFilePath[i]=aFile;
        MOT[iMMAuto[i]].Tray.ReadUnloaderInfo(aFile);
    }
    ReportList->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete ReportList;
    return;
}
//------------------------------------------------------------------------------
void SaveUnloaderInfo(int iWhichTray)                                           //ChungHung 20150205 add for ATK
{
    AnsiString aDir, aFile, aPath="D:\\UnloaderInfo";
    aDir.sprintf("%s\\%s", aPath, FormatDateTime("dd_mm_yyyy", Now()));
    MyForceDirectories(aPath, "SaveUnloaderInfo");

    TStringList *ReportList = new TStringList;
    aFile.sprintf("%s\\LastTrayInfo.txt", aPath);
    if(!FileExists(aFile))
    {
        for(int i=0; i<eTrayCount; i++)
        {
            if(Prod.iTrayType[i]!=tNotUse)
            {
                SetUnloaderInfoFile(i);
            }
        }
    }

    MOT[iMMAuto[iWhichTray]].Tray.SaveUnloaderInfo(asUnloaderInfoFilePath[iWhichTray]);

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            ReportList->Add(asUnloaderInfoFilePath[i]);
        }
    }

    aFile.sprintf("%s\\LastTrayInfo.txt", aPath);
    ReportList->SaveToFile(aFile);
    ReportList->Clear();                                                        //Ifor 20170603 TStringList 刪除前先 Clean
    delete ReportList;
    return;
}
//------------------------------------------------------------------------------
void SaveMachineRecord(bool bSpare)
{
    int tray=0, i=0, j=0, index=0, k=0;                                         //Jimmychiu 20220901 加入初始值
    MachRec.bInitialStart=false;
    if(LastSet.iRealDummy==REALLY &&
       (MOT[MManualTray1].HasIC() ||
        MOT[MManualTray2].HasIC() ||
        MOT[MManualTray3].HasIC() ||
        MOT[MManualTray4].HasIC() ||
        MOT[MManualTray5].HasIC() ||
        MOT[MManualTray6].HasIC() ||
        MOT[MMTrayY].HasIC() ||
        MOT[MMTrayY_Car].fHasTray ||
        MOT[MMPlate1].Tray.HasIC()||                                            //kevin 20120616   MOT[MMPlate1].HasIC() ||
        MOT[MMPlate2].HasIC() ||
        MOT[MMAuto1].fHasTray ||
        MOT[MMAuto2].fHasTray ||
        MOT[MMAuto3].fHasTray ||
        MOT[MMAuto4].fHasTray ||
        MOT[MMAuto5].fHasTray ||
        MOT[MMAuto6].fHasTray ||
        MOT[MMAuto1_Car].fHasTray ||
        MOT[MMAuto2_Car].fHasTray ||
        MOT[MMAuto3_Car].fHasTray ||
        MOT[MMAuto4_Car].fHasTray ||
        MOT[MMAuto5_Car].fHasTray ||
        MOT[MMAuto6_Car].fHasTray))
    {
        MachRec.bInitialStart=true;
    }

    for(tray=MManualTray1; tray<MMColor_Car; tray++)
    {
        index=tray-MManualTray1;
        MachRec.fHasTray[index]=MOT[tray].fHasTray;
        for(i=0; i<MAX_X_ITEM; i++)
        {
            for(j=0; j<MAX_Y_ITEM; j++)
            {
                MachRec.TrayData[index][i][j]  =MOT[tray].Tray.Data[i][j];
                MachRec.iWhichSite[index][i][j]=MOT[tray].Tray.iWhichSite[i][j];//Steven 20220602 : for Auto Site Map

                if(AUTO3_IS_MAGAZINE==1 &&
                   TestIF_File.iMagFixTrayType==1 &&
                   (tray>=MManualTray1 && tray<=MManualTray3))                  //JerryYang 20221215 : Magazine把fix區當buffer區功能
                {
                    MachRec.iBinData[index][i][j]  =MOT[tray].Tray.iBinData[i][j];
                    MachRec.iWhichAuto[index][i][j]=MOT[tray].Tray.iWhichAuto[i][j];
                }

                if((tray==MMPlate1 || tray==MMPlate2) &&
                   MachRec.iWhichSite[index][i][j]>0)
                {
                    bHotPlateHasSiteMap=true;
                }
            }
        }

        MachRec.TrayXItem[index]=MOT[tray].Tray.XItem;
        MachRec.TrayYItem[index]=MOT[tray].Tray.YItem;
    }

    if(AUTO_EMPTY_COLOR>=3)
    {
        for(tray=MManualTray4; tray<=MMAuto6_Car; tray++)
        {
            index=tray-MManualTray4;
            MachRec.fHasTray_6[index]=MOT[tray].fHasTray;
            for(i=0; i<MAX_X_ITEM; i++)
            {
                for(j=0; j<MAX_Y_ITEM; j++)
                {
                    MachRec.TrayData_6[index][i][j]  =MOT[tray].Tray.Data[i][j];
                    MachRec.iWhichSite_6[index][i][j]=MOT[tray].Tray.iWhichSite[i][j];//Steven 20220602 : for Auto Site Map

                    if(AUTO3_IS_MAGAZINE==1 &&
                       TestIF_File.iMagFixTrayType==1 &&
                       (tray>=MManualTray4 && tray<=MManualTray6))              //JerryYang 20221215 : Magazine把fix區當buffer區功能
                    {
                        MachRec.iBinData_6[index][i][j]  =MOT[tray].Tray.iBinData[i][j];
                        MachRec.iWhichAuto_6[index][i][j]=MOT[tray].Tray.iWhichAuto[i][j];
                    }
                }
            }

            MachRec.TrayXItem_6[index]=MOT[tray].Tray.XItem;
            MachRec.TrayYItem_6[index]=MOT[tray].Tray.YItem;
        }
    }

    for(i=0; i<MAX_X_ITEM; i++)
        for(j=0; j<MAX_Y_ITEM; j++)
            MachRec.LoaderBuf[i][j]=MOT[MMTrayY].Tray.Data[i][j];

    for(i=0; i<MAX_Index_Row; i++)
    {
        for(j=0; j<NEW_MAX_Index_Col; j++)
        {
            MachRec.FLCarryKitItem[i][j]=FLCarryKit.Item[i][j];
            MachRec.FRCarryKitItem[i][j]=FRCarryKit.Item[i][j];
            MachRec.BLCarryKitItem[i][j]=BLCarryKit.Item[i][j];
            MachRec.BRCarryKitItem[i][j]=BRCarryKit.Item[i][j];
            MachRec.FTestSuckItem[i][j] =FTestSuck.Item[i][j];
            MachRec.BTestSuckItem[i][j] =BTestSuck.Item[i][j];
            MachRec.TestSocketItem[i][j]=TestSocket.Item[i][j];
        }
    }
    MachRec.CatchTrayItem=CatchTraySuck.Item[0][0];                             //ChungHung 20150410 add
    for(i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(j=0; j<InArmSuck.iMaxCol; j++)
        {
            MachRec.InArmSuckItem[i][j] =InArmSuck.Item[i][j];
            MachRec.OutArmSuckItem[i][j]=OutArmSuck.Item[i][j];
        }
    }
    //Eliot 2009_12_24 end
    MachRec.iWhichSht  =InArmSuck.iWhichSht;
    MachRec.iWhichKit  =InArmSuck.iWhichKit;
    MachRec.iInArmOrder=iInArmOrder;
    for(i=0; i<2; i++)
    {
        MachRec.iPickPlate[i]   =iPickPlate[i];
        MachRec.iPlatePickX[i]  =iPickPlateX[i];
        MachRec.iPlatePickY[i]  =iPickPlateY[i];
        MachRec.iPlacePlate[i]  =iPlacePlate[i];
        MachRec.iPlatePlaceX[i] =iPlacePlateX[i];
        MachRec.iPlatePlaceY[i] =iPlacePlateY[i];
    }
    MachRec.iBackInArmHotCount=iBackInArmHotCount;
    MachRec.iHotCount=iHotCount;
    for(i=0; i<2; i++)
    {
        for(j=0; j<50; j++)
        {
            for(k=0; k<50; k++)
            {
                MachRec.iHotPlateCount[i][j][k]  =iHotPlateCount[i][j][k];
                MachRec.iHotInArmOrder[i][j][k]  =iHotInArmOrder[i][j][k];
                MachRec.iHotWhichKit[i][j][k]    =iHotWhichKit[i][j][k];
                MachRec.iHotWhichShuttle[i][j][k]=iHotWhichShuttle[i][j][k];
                MachRec.iRowOnHotPlate[i][j][k]  =iRowOnHotPlate[i][j][k];      //JerryYang 20180718 (wei) : 放料至hot plate記錄吸嘴位置
            }
        }
    }

    MachRec.bPlaceToHotplate =bPlaceToHotplate;
    MachRec.bPickFromHotplate=bPickFromHotplate;

    MachRec.bSiteMappingCHKOK=bSiteMappingCHKOK;                                //jou 2011-03-24 start : Auto Site Mapping

    MachRec.bBackupCleanOut=bBackupCleanOut;                                    //jou 2011-07-06 : 改成任何狀況下,都要紀錄clean out模式,不然會hang up

    if(AUTO3_IS_MAGAZINE==1)                                                    //JerryYang 20220909 : add magazine
    {
        for(int m=0; m<MAX_MGZ_TRAY; m++)
        {
            MachRec.fHasMagazineTray[m]=MOT[iMMgzTray[m]].fHasTray;

            for(i=0; i<MAX_X_ITEM; i++)
            {
                for(j=0; j<MAX_Y_ITEM; j++)
                {
                    MachRec.MagazineTrayData[m][i][j] = MOT[iMMgzTray[m]].Tray.Data[i][j];
                }
            }
            MachRec.MagazineTrayXItem[m]=MOT[iMMgzTray[m]].Tray.XItem;
            MachRec.MagazineTrayYItem[m]=MOT[iMMgzTray[m]].Tray.YItem;
        }

        MachRec.iAuto3MagazineIndex=iAuto3MagazineIndex;
    }

    if(bSpare)
    {
        WriteData("d:\\HT9045\\system\\machinerecordRealCCD.dat",
            (char *)&MachRec.bInitialStart, sizeof(struct MachineRecord));
    }
    else
    {
        WriteData("d:\\HT9045\\system\\machinerecord.dat",
            (char *)&MachRec.bInitialStart, sizeof(struct MachineRecord));
    }

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            SaveUnloaderInfo(i);
        }
    }
    PickFromHPList->SaveFile(sHPPickRec);                                       //Jimmychiu 20230417 : Record the position after placing the IC
    PlaceToCleanList->SaveFile(sCleanPlaceRec);                                 //Jimmychiu 20230417 : Record the position after placing the IC
}
//------------------------------------------------------------------------------
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");//Steven 20120615 : 改成可用序號(Mode)或名稱(ModeText)進行模式改變
extern bool bAutoEdgePush[MAX_AUTO_TRAY];
void LoadMachineRecord(bool bSpare)
{
    int ret, tray, index, iLoadData=0;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<50; j++)
        {
            for(int k=0; k<50; k++)
            {
                 iHotPlateCount[i][j][k]  =-1;
                 iHotInArmOrder[i][j][k]  =-1;
                 iHotWhichKit[i][j][k]    =-1;
                 iHotWhichShuttle[i][j][k]=-1;
            }
        }
    }
    bLoadMachineRecord=true;                                                    //JerryYang 20160614 用來判斷是否執行過LoadMachineRecord函式 避免還沒讀取到機台資料Lastset就被改掉
    for(int i=0; i<eTrayCount; i++)                                             //JerryYang 20240111 : add P53 function
    {
        sStackBinTemp[i]="";
    }
    InitialMachine();
    if(CosFunction.bRMSNoNeedToDownloadEveryTime==false)                        //Steven 20240926 : RMS不要每次下載包成功能
        LastSet.bHasDownloadFile=false;                                         //Steven 20110125
    bool bReadOK=false;//, bFixTray=false;

    LoadUnloaderInfo();                                                         //ChungHung 20150205 add for ATK

    if(bSpare)
    {
        bReadOK = ReadData("d:\\HT9045\\system\\machinerecordRealCCD.dat",
        (char *)&MachRec.bInitialStart, sizeof(struct MachineRecord));
    }
    else
    {
        bReadOK = ReadData("d:\\HT9045\\system\\machinerecord.dat",
        (char *)&MachRec.bInitialStart, sizeof(struct MachineRecord));
    }

    if(bReadOK)
    {
        bSiteMappingCHKOK=MachRec.bSiteMappingCHKOK;                            //jou 2011-03-24 start : Auto Site Mapping

        if(IniConfig.bI21EnableASM)                                             //Steven 20110502
        {
            if(bSiteMappingCHKOK==true)
            {
                bSiteMappingCHKOK=false;
                fMain->CheckSiteMapState(true);                                 //kevin 20150120 強致做autositemap
                RecordProcess("Trigger Auto Site Map after Load Machine Record");
                SetRunStartMode(rsmAutoSiteMap);
            }
        }
        else
        {
            bSiteMappingCHKOK=false;
        }

        if(MachRec.bInitialStart==false && bSpare==false)
        {
            SaveMachineRecord();
            bFTPDownloadSetupFile=false;                                        //Ifor 20170727 (wei) 修正Bin變更後重開可生產問題
            return;
        }
    }
    else
    {
        SaveMachineRecord();
        return;
    }

    if(bSpare==false)
    {
        int iState=0;
        iState=CheckAndReadIniDataGeneral("Record", "Program Close", 1);

        if(iState==0)                                                           //Ifor 20210621 add:Production Log Tray 連續
        {
            LastSet.iInTrayNum=0;
            ZeroMemory(LastSet.iOutTrayNum, sizeof(LastSet.iOutTrayNum));
        }

        #ifndef SOFT_SIMULTE
        if(iState==0)
        {
            if(IniConfig.bAbnormalStartCheck)                                   //Steven 20111216 : 不正常關程式偵測
            {
                if(MachineTypeChoice==Type_HT9045)                              //9045
                    ShowMyMessage("Because Abnormal Close HT9045 Porgram,Must Clear Machine !!", "因為不正常關閉HT9045程式，需手動清料");
                else if(MachineTypeChoice==Type_HT9045_12Site)                  //ChungHung 20130507 add HT9045 updata for 12site 517
                    ShowMyMessage("Because Abnormal Close HT9045_12Site Porgram,Must Clear Machine !!", "因為不正常關閉HT9045_12Site程式，需手動清料");
                else
                    ShowMyMessage("Because Abnormal Close HT9046 Porgram,Must Clear Machine !!", "因為不正常關閉HT9046程式，需手動清料");
                return;
            }
            else
            {
                RecordProcess("Abnormal Close Handler Porgram");
            }
            PickFromHPList->ResetFile(sHPPickRec);                              //Jimmychiu 20250220 : 避免重新開機後Hotplate rec有殘留資料
            PlaceToCleanList->ResetFile(sCleanPlaceRec);                        //Jimmychiu 20250220 : 避免重新開機後Hotplate rec有殘留資料
        }
        #else
        if(iState==0)
        {

        }
        #endif
        //jou 2010-10-21 end

        if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                      //KaiChen 20191111 ：矽格-湖口 鍾卓劭 不讀取 Last Data
        {
            bFTPDownloadSetupFile=false;
            RecordProcess("Program Start,No,load last data. SIGURD_HUKOU");
            return;
        }

        if(CUSTOMER_CODE==CC_AMKOR_Philippines)                                 //JerryYang 20200312 load data流程修改
        {
            while(iLoadData==0 || iLoadData==2)
            {
                ret=MessageDlg("Need Load last machine record? (需要讀取之前的設定嗎?)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
                if(ret==mrNo || ret==mrCancel)
                {
                    ret=MessageDlg("Are you Sure don't Load last machine record? (你確定不需要讀取之前的設定嗎?)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
                    if(ret==mrYes)
                    {
                        bFTPDownloadSetupFile=false;                            //Ifor 20170727 (wei) 修正Bin變更後重開可生產問題
                        RecordProcess("Program Start,No,give up last data.");
                        ShowMyMessage("Handler load initial data,\r\n Please remove all device in machince and put it to reject bin." );
                        return;
                    }
                    iLoadData==2;
                }
                else
                {
                    RecordProcess("Program Start,Yes,load last data.");
                    iLoadData=1;
                }
            }
        }
        else
        {
            ret=MessageDlg("Need Load last machine record? (需要讀取之前的設定嗎?)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
            if(ret==mrNo)
            {
                if(IniConfig.bKoreaFunction==false)
                {                                                               //Ifor 20160831 add 讀取last machine record多一道防護，避免按錯造成資料遺失
                    ret=MessageDlg("Are you Sure don't Load last machine record? (你確定不需要讀取之前的設定嗎?)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
                    if(ret==mrYes)
                    {
                        bFTPDownloadSetupFile=false;                            //Ifor 20170727 (wei) 修正Bin變更後重開可生產問題
                        RecordProcess("Program Start,No,give up last data.");   //JerryYang 20200316 fix load data log錯誤      //kevin 20180402 add

                        if(IniConfig.bSPILFunction==true)                       //JerryYang 20241021 : Unloader增加第二組Sensor檢查是否有cover tray
                        {
                            bNoitceFixTray=false;
                            for(tray=MManualTray1; tray<MManualTray3; tray++)
                            {
                                if(MachRec.fHasTray[tray-MManualTray1])
                                {
                                    bNoitceFixTray=true;
                                }
                            }
                        }

                        if(IniConfig.bVTESTFunction==true)
                        {
                            RecordProcess("Clear OEE data.");
                            fMesSystem->CleaOEEState12hList();
                        }
                        return;
                    }
                    RecordProcess("Program Start,Yes,load last data.");         //JerryYang 20200316 fix load data log錯誤
                }
                else
                {
                    bFTPDownloadSetupFile=false;                                //Ifor 20170727 (wei) 修正Bin變更後重開可生產問題
                    RecordProcess("Program Start,No,give up last data.");       //kevin 20180402 add
                    return;
                }
            }
            else
            {
                RecordProcess("Program Start,Yes,load last data.");             //kevin 20180402 add
            }
        }
    }

    LastSet.bHasDownloadFile=true;                                              //Steven 20110125
    if(bSpare)
    {
        bReadOK = ReadData("d:\\HT9045\\system\\machinerecordRealCCD.dat",
        (char *)&MachRec.bInitialStart, sizeof(struct MachineRecord));
    }
    else
    {
        bReadOK = ReadData("d:\\HT9045\\system\\machinerecord.dat",
        (char *)&MachRec.bInitialStart, sizeof(struct MachineRecord));
    }

    if(bReadOK)
    {
        for(tray=MManualTray1; tray<MMColor_Car; tray++)
        {
            index=tray-MManualTray1;
            MOT[tray].fHasTray=MachRec.fHasTray[index];
            if(MOT[tray].fHasTray)
            {
                for(int i=0; i<MAX_X_ITEM; i++)
                {
                    for(int j=0; j<MAX_Y_ITEM; j++)
                    {
                        MOT[tray].Tray.Data[i][j]=MachRec.TrayData[index][i][j];
                        MOT[tray].Tray.iWhichSite[i][j]=MachRec.iWhichSite[index][i][j];            //Steven 20220602 : for Auto Site Map

                        if(AUTO3_IS_MAGAZINE==1 &&
                           (tray>=MManualTray1 && tray<=MManualTray3))          //JerryYang 20221215 : Magazine把fix區當buffer區功能
                        {
                            MOT[tray].Tray.iBinData[i][j]=MachRec.iBinData[index][i][j];
                            MOT[tray].Tray.iWhichAuto[i][j]=MachRec.iWhichAuto[index][i][j];
                        }

                    }
                }
                MOT[tray].Tray.SetXYItem(MachRec.TrayXItem[index], MachRec.TrayYItem[index]);       //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[tray].Refresh();

                if(tray>=MMAuto1 && tray<=MMAuto3)
                    bAutoEdgePush[tray-MMAuto1]=true;
            }
        }

        if(AUTO_EMPTY_COLOR>=3)
        {
            for(tray=MManualTray4; tray<=MMAuto6_Car; tray++)
            {
                index=tray-MManualTray4;
                MOT[tray].fHasTray=MachRec.fHasTray_6[index];
                if(MOT[tray].fHasTray)
                {
                    for(int i=0; i<MAX_X_ITEM; i++)
                    {
                        for(int j=0; j<MAX_Y_ITEM; j++)
                        {
                            MOT[tray].Tray.Data[i][j]=MachRec.TrayData_6[index][i][j];
                            MOT[tray].Tray.iWhichSite[i][j]=MachRec.iWhichSite_6[index][i][j];            //Steven 20220602 : for Auto Site Map

                            if(AUTO3_IS_MAGAZINE==1 &&
                               (tray>=MManualTray1 && tray<=MManualTray3))      //JerryYang 20221215 : Magazine把fix區當buffer區功能
                            {
                                MOT[tray].Tray.iBinData[i][j]=MachRec.iBinData_6[index][i][j];
                                MOT[tray].Tray.iWhichAuto[i][j]=MachRec.iWhichAuto_6[index][i][j];
                            }
                        }
                    }
                    MOT[tray].Tray.SetXYItem(MachRec.TrayXItem_6[index], MachRec.TrayYItem_6[index]);       //Steven 20160614 : 設定Tray XY Item改用function加上保護
                    MOT[tray].Refresh();

                    if(tray>=MMAuto4 && tray<=MMAuto6)
                        bAutoEdgePush[eAuto4+(tray-MMAuto4)]=true;
                }
            }
        }

        if(MOT[MMTrayY].fHasTray)
        {
            for(int i=0; i<MAX_X_ITEM; i++)
                for(int j=0; j<MAX_Y_ITEM; j++)
                    MOT[MMTrayY].Tray.BufferData[i][j]=MachRec.LoaderBuf[i][j];
        }

        //Eliot 2009_12_24 start
        for(int i=0; i<FTestSuck.iMaxRow; i++)
        {
            for(int j=0; j<FTestSuck.iMaxCol; j++)
            {
                //ChungHung 20120425 modify
                FLCarryKit.SetItemData(i, j, MachRec.FLCarryKitItem[i][j]);
                FRCarryKit.SetItemData(i, j, MachRec.FRCarryKitItem[i][j]);
                BLCarryKit.SetItemData(i, j, MachRec.BLCarryKitItem[i][j]);
                BRCarryKit.SetItemData(i, j, MachRec.BRCarryKitItem[i][j]);
                FTestSuck.SetItemData (i, j, MachRec.FTestSuckItem[i][j]);
                BTestSuck.SetItemData (i, j, MachRec.BTestSuckItem[i][j]);
            }
        }

        for(int i=0; i<TestSocket.iMaxRow; i++)
        {
            for(int j=0; j<TestSocket.iMaxCol; j++)
            {
                TestSocket.SetItemData(i, j, MachRec.TestSocketItem[i][j]);
            }
        }

        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                //ChungHung 20120425 modify
                InArmSuck.SetItemData (i, j, MachRec.InArmSuckItem[i][j]);
                OutArmSuck.SetItemData(i, j, MachRec.OutArmSuckItem[i][j]);
            }
        }//Eliot 2009_12_24 end

        #ifndef SOFT_SIMULTE
        //Steven 2011-12-16
        if(IndexHasIC())
        {
            bIsTestSitICFallDownResetHT9045=true;
            FTestSuck.ClearAll();
            BTestSuck.ClearAll();
            TestSocket.ClearAll();
        }

        if(InArmSuck.HasIC() || OutArmSuck.HasIC())
        {
            InArmSuck.ClearAll();
            OutArmSuck.ClearAll();
        }
        #endif
        if(ShuttleHasIC())
        {
            //jou 980317 load MachRec Shuttle IC no need Clear
            for(int i=0; i<FLCarryKit.iMaxRow; i++)
            {
                for(int j=0; j<FLCarryKit.iMaxCol; j++)
                {
                    FLCarryKit.SetItemData(i, j, FLCarryKit.Item[i][j]);
                    FRCarryKit.SetItemData(i, j, FRCarryKit.Item[i][j]);
                    BLCarryKit.SetItemData(i, j, BLCarryKit.Item[i][j]);
                    BRCarryKit.SetItemData(i, j, BRCarryKit.Item[i][j]);
                }
            }

            //jou 2016-10-04 修正load machine record shuttle = null ic hang up
            if(FLCarryKit.HasRealIC())
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                        if(FLCarryKit.Item[i][j]==NULL_IC)
                            FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
            }

            if(BLCarryKit.HasRealIC())
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                        if(BLCarryKit.Item[i][j]==NULL_IC)
                            BLCarryKit.SetItemData(i, j, HAS_NULL_IC);
            }
        }

        InArmSuck.iWhichSht=MachRec.iWhichSht;
        InArmSuck.iWhichKit=MachRec.iWhichKit;
        iInArmOrder        =MachRec.iInArmOrder;

        for(int i=0; i<2; i++)
        {
            iPickPlate[i]   =MachRec.iPickPlate[i];
            iPickPlateX[i]  =MachRec.iPlatePickX[i];
            iPickPlateY[i]  =MachRec.iPlatePickY[i];
            iPlacePlate[i]  =MachRec.iPlacePlate[i];
            iPlacePlateX[i] =MachRec.iPlatePlaceX[i];
            iPlacePlateY[i] =MachRec.iPlatePlaceY[i];
        }
        iBackInArmHotCount  =MachRec.iBackInArmHotCount;
        //jou 980313 no load iHotCount
        iHotCount=MachRec.iHotCount;
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<50; j++)
            {
                for(int k=0; k<50; k++)
                {
                    iHotPlateCount[i][j][k]  =MachRec.iHotPlateCount[i][j][k];
                    iHotInArmOrder[i][j][k]  =MachRec.iHotInArmOrder[i][j][k];
                    iHotWhichKit[i][j][k]    =MachRec.iHotWhichKit[i][j][k];
                    iHotWhichShuttle[i][j][k]=MachRec.iHotWhichShuttle[i][j][k];
                    iRowOnHotPlate[i][j][k]  =MachRec.iRowOnHotPlate[i][j][k];  //JerryYang 20180718 (wei) : 放料至hot plate記錄吸嘴位置
                }
            }
        }

        bPlaceToHotplate =MachRec.bPlaceToHotplate;
        bPickFromHotplate=MachRec.bPickFromHotplate;
        bBackupCleanOut  =MachRec.bBackupCleanOut;                              //jou 2011-07-06 : 改成任何狀況下,都要紀錄clean out模式,不然會hang up

        if(AUTO3_IS_MAGAZINE==1)                                                //JerryYang 20220909 : add magazine
        {
            for(int m=0; m<MAX_MGZ_TRAY; m++)
            {
                MOT[iMMgzTray[m]].fHasTray=MachRec.fHasMagazineTray[m];

                if(MOT[iMMgzTray[m]].fHasTray)
                {
                    for(int i=0; i<MAX_X_ITEM; i++)
                    {
                        for(int j=0; j<MAX_Y_ITEM; j++)
                        {
                            MOT[iMMgzTray[m]].Tray.Data[i][j] = MachRec.MagazineTrayData[m][i][j];
                        }
                    }
                    MOT[iMMgzTray[m]].Tray.SetXYItem(MachRec.MagazineTrayXItem[m], MachRec.MagazineTrayYItem[m]);       //Steven 20160614 : 設定Tray XY Item改用function加上保護
                    MOT[iMMgzTray[m]].Refresh();
                }
            }
            iAuto3MagazineIndex=MachRec.iAuto3MagazineIndex;                    //JerryYang 20221207 : 開程式要把Magazine在哪一層讀回來
        }
        PickFromHPList->LoadFile(sHPPickRec);                                   //Jimmychiu 20230417 : Record the position after placing the IC
        PlaceToCleanList->LoadFile(sCleanPlaceRec);                             //Jimmychiu 20230417 : Record the position after placing the IC
    }
    else
    {
        PickFromHPList->ResetFile(sHPPickRec);                                  //Jimmychiu 20250220 : 避免重新開機後Hotplate rec有殘留資料
        PlaceToCleanList->ResetFile(sCleanPlaceRec);                            //Jimmychiu 20250220 : 避免重新開機後Hotplate rec有殘留資料
    }
    SaveMachineRecord();

    if(IniConfig.bUseAutoSiteMapping)                                           //jou 2011-03-24 start : Auto Site Mapping
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap && bSiteMappingCHKOK==false)
        {
            if(IndexHasIC() || ShuttleHasIC())
            {
                fMain->SetMainRunStartMode(rsmContinuStart);
            }
        }
    }

    bFTPDownloadSetupFile=true;
    if(bEnable_KLT_Function==true)                                              //Ifor 20180802 (Steven) : add KLT bBarCodeRules -> bEnable_KLT_Function 0:KYEC 1: KLT
    {
        iHasReadeLastData=1;                                                    //Ifor 20180802 (Steven) : add 重開程式後Loader有Tray 且開啟Auto SPIK 強制手動取Tray
    }

    if(CosFunction.bShowHPICCount)                                              //Steven 20221228 : 計算加熱盤IC數量
    {
        fSortCT->pnlLoad->Caption=MOT[MMTrayY].Tray.HowManyIC();
        fSortCT->pnlHP1->Caption=MOT[MMPlate1].Tray.HowManyIC();
        fSortCT->pnlHP2->Caption=MOT[MMPlate2].Tray.HowManyIC();
    }
}
//------------------------------------------------------------------------------
extern void SetAOATrayTeachPoint(int &iCol, int &iRow, TTrayMotor *myTray, bool bTraySkipLast=false);   //ChungHung 20210113 add for Alignment CCD
//------------------------------------------------------------------------------
void SetTechDataToProd_Tray()
{
    bool bTrayEndRecv[eTrayCount];
    int iTrayDir[eTrayCount+3], iTrayType[eTrayCount+3];
    int iTypeBuffer=0;                                                          //kevin 20110729 暫存TRAY PITCH資料

    if(IniConfig.bIndexArm2SupplyLight==true ||                                 //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
       TestIF_File.bForEgisTecTest==true     ||                                 //Steven 20140922 : Arm2當作指紋測試
       (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                         //kevin 20150127 Arm1 下壓 arm2 測試
        TestIF_File.bArm1PickPlaceArm2Test==true))                              //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
    {
        if(TestIF_File.bArm1PickPlaceArm2Test==true)
        {
            Prod.iTrayKitStartX=0;
            Prod.iTrayKitStartY=0;
            Prod.iTrayKitPitchX=0;
            Prod.iTrayKitPitchY=0;
        }
        else
        {
            Prod.iTrayKitStartX=4240;
            Prod.iTrayKitStartY=1940;
            Prod.iTrayKitPitchX=0;
            Prod.iTrayKitPitchY=7500;
        }
    }
    else
    {
        Prod.iTrayKitStartX=0;
        Prod.iTrayKitStartY=0;
        Prod.iTrayKitPitchX=0;
        Prod.iTrayKitPitchY=0;
    }

    LoadForm        =&TrayForm.Loader;
    EmptyForm       =&TrayForm.Empty;
    ColorForm       =&TrayForm.Color;

    for(int i=0; i<eTrayCount; i++)                                             //wei 20160519 secs陣列8-->10
    {
        AutoForm[i]=&TrayForm.Auto[i];
        bTrayEndRecv[i]=TrayForm.Auto[i].TrayEndRecv;                           //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
    }

    if(TrayForm.LodareType==0)
    {
        for(int i=0; i<eTrayCount+3; i++)                                         //wei 20160519 secs陣列8-->10
        {
            iTrayDir[i]=TrayForm.Loader.Direction;
            iTrayType[i]=TrayForm.Loader.iTrayType;
        }

        memcpy(&LoadForm->XPitch,           &UserDefForm[iTrayType[0]].XPitch, sizeof(TRAY_TYPE_PARA));
        memcpy(&EmptyForm->XPitch,          &UserDefForm[iTrayType[0]].XPitch, sizeof(TRAY_TYPE_PARA));
        memcpy(&ColorForm->XPitch,          &UserDefForm[iTrayType[0]].XPitch, sizeof(TRAY_TYPE_PARA));
        for(int i=0; i<eTrayCount; i++)
        {
            memcpy(&AutoForm[i]->XPitch,    &UserDefForm[iTrayType[0]].XPitch, sizeof(TRAY_TYPE_PARA));
        }
    }
    else
    {
        iTrayDir[etLoader]=TrayForm.Loader.Direction;
        iTrayDir[etEmpty ]=TrayForm.Empty.Direction;
        iTrayDir[etColor ]=TrayForm.Color.Direction;
        iTrayType[etLoader]=TrayForm.Loader.iTrayType;
        iTrayType[etEmpty ]=TrayForm.Empty.iTrayType;
        iTrayType[etColor ]=TrayForm.Color.iTrayType;

        for(int i=0; i<eTrayCount; i++)
        {
            iTrayDir[etAuto1+i] =TrayForm.Auto[i].Direction;
            iTrayType[etAuto1+i]=TrayForm.Auto[i].iTrayType;
        }

        //先存TYPE 才不會因MEMCPY被清為0 取錯資料   kevin 20110729
        iTypeBuffer =(TrayForm.Loader.iTrayType>3 || TrayForm.Loader.iTrayType<0)?0:(TrayForm.Loader.iTrayType);
        memcpy(&LoadForm->XPitch,    &UserDefForm[iTypeBuffer].XPitch, sizeof(TRAY_TYPE_PARA));
        TrayForm.Loader.iTrayType = iTypeBuffer;

        iTypeBuffer =(TrayForm.Empty.iTrayType>3 || TrayForm.Empty.iTrayType<0)?0:(TrayForm.Empty.iTrayType);
        memcpy(&EmptyForm->XPitch,   &UserDefForm[iTypeBuffer].XPitch, sizeof(TRAY_TYPE_PARA));
        TrayForm.Empty.iTrayType = iTypeBuffer;

        iTypeBuffer =(TrayForm.Color.iTrayType>3 || TrayForm.Color.iTrayType<0)?0:(TrayForm.Color.iTrayType);
        memcpy(&ColorForm->XPitch,   &UserDefForm[iTypeBuffer].XPitch, sizeof(TRAY_TYPE_PARA));
        TrayForm.Color.iTrayType = iTypeBuffer;

        for(int i=0; i<eTrayCount; i++)
        {
            iTypeBuffer=(TrayForm.Auto[i].iTrayType>3 || TrayForm.Auto[i].iTrayType<0)?0:(TrayForm.Auto[i].iTrayType);
            memcpy(&AutoForm[i]->XPitch, &UserDefForm[iTypeBuffer].XPitch, sizeof(TRAY_TYPE_PARA));
            TrayForm.Auto[i].iTrayType=iTypeBuffer;
        }
    }

    LoadForm->Direction  =iTrayDir[etLoader];
    EmptyForm->Direction =iTrayDir[etEmpty ];
    ColorForm->Direction =iTrayDir[etColor ];
    LoadForm->iTrayType  =iTrayType[etLoader];
    EmptyForm->iTrayType =iTrayType[etEmpty ];
    ColorForm->iTrayType =iTrayType[etColor ];

    for(int i=0; i<eTrayCount; i++)
    {
        TrayForm.Auto[i].TrayEndRecv=bTrayEndRecv[i];                           //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        AutoForm[i]->Direction=iTrayDir[etAuto1+i];
        AutoForm[i]->iTrayType=iTrayType[etAuto1+i];
    }
}
//------------------------------------------------------------------------------
void SetTechDataToProd_InArm()
{
    int iHPX1, iHPY1;                                                           //Steven 20140222 : Hot Plate Pin的位置
    int iHPX2, iHPY2;
    int iHPX1Laser, iHPY1Laser;                                                 //Steven 20140228 : 雷射測距功能
    int iHPX2Laser, iHPY2Laser;
    double dbTrayThick=0.0;                                                     //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
    int iTemp=0;                                                                //Sam 20240905 : 修正 AutoClean Offset
    int iAcOfs=0;
    if(HOT_PLATE_POSITION==0 &&                                                 //Steven 20140222 : Hot Plate Pin的位置
       MachineTypeChoice!=Type_HT1032)
    {
        iHPX1=Tech.iInArmPlate1X;
        iHPY1=Tech.iInArmPlate1Y;
        iHPX2=Tech.iInArmPlate2X;
        iHPY2=Tech.iInArmPlate2Y;

        iHPX1Laser=Tech.iHP1LaserX;
        iHPY1Laser=Tech.iHP1LaserY;
        iHPX2Laser=Tech.iHP2LaserX;
        iHPY2Laser=Tech.iHP2LaserY;
    }
    else
    {
        if(MachineTypeChoice==Type_HT1032 && USE_PICKER_COUNT==ep16Picker)//Ztex forHT-1132
        {
            iHPX1=Tech.iInArmPlate1X
                     +1000
                     -HotPlateForm.XStart*2
                     -(HotPlateForm.XDivision-1)*HotPlateForm.XPitch
                     +9000;
        }
        else
        {   //右邊的Teaching + 移到最右邊的9000 - X-Start*2 - (X-Item-1)*X-Pitch + 移到左邊Teach的9000位置
            iHPX1=Tech.iInArmPlate1X
                     +9000
                     -HotPlateForm.XStart*2
                     -(HotPlateForm.XDivision-1)*HotPlateForm.XPitch
                     +9000;
        }

        iHPX1Laser=Tech.iHP1LaserX
                 +10000
                 -HotPlateForm.XStart*2
                 -(HotPlateForm.XDivision-1)*HotPlateForm.XPitch
                 +10000;

        //右邊的Teaching + Y-Start + (Y-Item-1)*Y-Pitch
        iHPY1=Tech.iInArmPlate1Y-1000
                 +HotPlateForm.YStart*2
                 +(HotPlateForm.YDivision-1)*HotPlateForm.YPitch
                 -1000;

        iHPY1Laser=Tech.iHP1LaserY
                 +HotPlateForm.YStart
                 +(HotPlateForm.YDivision-1)*HotPlateForm.YPitch;

        if(MachineTypeChoice==Type_HT1032 && USE_PICKER_COUNT==ep16Picker)//Ztex forHT-1132
        {
            iHPX2=Tech.iInArmPlate2X
                     +1000
                     -HotPlateForm.XStart*2
                     -(HotPlateForm.XDivision-1)*HotPlateForm.XPitch
                     +9000;
        }
        else
        {
            iHPX2=Tech.iInArmPlate2X
                     +9000
                     -HotPlateForm.XStart*2
                     -(HotPlateForm.XDivision-1)*HotPlateForm.XPitch
                     +9000;
        }
        iHPX2Laser=Tech.iHP2LaserX
                 +10000
                 -HotPlateForm.XStart*2
                 -(HotPlateForm.XDivision-1)*HotPlateForm.XPitch
                 +10000;

        iHPY2=Tech.iInArmPlate2Y-1000
                 +HotPlateForm.YStart*2
                 +(HotPlateForm.YDivision-1)*HotPlateForm.YPitch
                 -1000;

        iHPY2Laser=Tech.iHP2LaserY
                 +HotPlateForm.YStart
                 +(HotPlateForm.YDivision-1)*HotPlateForm.YPitch;
    }

    //-------------------------------------------------------------------------------------------------
    //InArm  && Loader
    //-------------------------------------------------------------------------------------------------
    //Loader Tray [0][0] position for base Suck
    Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmLoadStageX+LoadForm->XStart+LoadForm->BlockXStart-KitPitchX+InArmOffSet[InOfsLoader]->GetX()+Prod.iTrayKitStartX;  //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
    Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmLoadStageY-LoadForm->YStart-LoadForm->BlockYStart+KitPitchY+InArmOffSet[InOfsLoader]->GetY()-Prod.iTrayKitStartY;
    Prod.ZInArm_Tray_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmLoadStagePickZ2+InArmOffSet[InOfsLoader]->GetPickUp();//+UserDefForm[iTrayType[0]].ZDepth-635;       //Steven 20200718 : 厚Tray要計算吸放的厚度
    Prod.ZInArm_Tray_Place[iInArmYBase][iInArmXBase]=Tech.iInArmLoadStagePickZ2+InArmOffSet[InOfsLoader]->GetPlace();       //Steven 20221114 :Add for device map function

    if(CosFunction.bUseTrayThickAdjustZHeight==true &&
       (IniConfig.bE70_UseTrayThickAdjustZHeight==true ||                       //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
        UserDefForm_File[0].bUseThickTray))                                     //JerryYang 20230620 : add厚tray選項
    {
        dbTrayThick=UserDefForm_File[TrayForm.Loader.iTrayType].ZDepth*100;
        if(dbTrayThick<600)
            dbTrayThick=600.0;

        Prod.ZInArm_Tray_Pick[iInArmYBase][iInArmXBase]+=dbTrayThick-635;
        Prod.ZInArm_Tray_Place[iInArmYBase][iInArmXBase]+=dbTrayThick-635;
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;

            if(j<4)
                iTemp=Tech.iInArmZHeightSub[i][j];
            else
                iTemp=Tech.iInArmZHeightSub_16[i][j-4];

            Prod.ZInArm_Tray_Pick[i][j]=Prod.ZInArm_Tray_Pick[iInArmYBase][iInArmXBase]+iTemp+   //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                                         InArmOffSet[InOfsLoader]->GetPickUp(i, j);
            Prod.ZInArm_Tray_Place[i][j]=Prod.ZInArm_Tray_Place[iInArmYBase][iInArmXBase]+iTemp+ //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                                         InArmOffSet[InOfsLoader]->GetPlace(i, j);               //Steven 20221114 :Add for device map function
            //Prod.XInArm_Tray_Pick[i][j]=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+InArmOffSet[InOfsLoader]->GetArmX(i, j);
            //Prod.YInArm_Tray_Pick[i][j]=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]+InArmOffSet[InOfsLoader]->GetArmY(i, j);
            //=>
            Prod.XInArm_Tray_Pick[i][j]=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]; //Eastsun 20251231 : 因後面計算吸嘴都是用基準偏移 所以把個別偏移參數拿到後面
            Prod.YInArm_Tray_Pick[i][j]=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase];
        }
    }

    //-------------------------------------------------------------------------------------------------
    //InArm  && HotPlate
    //-------------------------------------------------------------------------------------------------
    //Hotplate [0][0] position for base Suck
    Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]=iHPX1+HotPlateForm.XStart-9000+InArmOffSet[InOfsHP1]->GetX();        //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
    Prod.iHP1LaserX                                  =iHPX1Laser+HotPlateForm.XStart-10000+InArmOffSet[InOfsHP1]->GetX();  //Steven 20140228 : 雷射測距功能

    if(IniConfig.bHotPlateMove1CM && HotPlateForm.bUseWideHotplate==false)
    {
        if(HOT_PLATE_POSITION==0)
            Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]=iHPX1+HotPlateForm.XStart-9000+1000+InArmOffSet[InOfsHP1]->GetX(); //ChungHung Add 20101025 CC_ASE_CL HotPlate Offset 10mm
        else
            Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]=iHPX1+HotPlateForm.XStart-9000-1000+InArmOffSet[InOfsHP1]->GetX(); //Steven 20160302
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;
            //Prod.XInArm_Plate1_Pick[i][j]=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+InArmOffSet[InOfsHP1]->GetArmX(i, j);
            //=>
            Prod.XInArm_Plate1_Pick[i][j]=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]; //Eastsun 20251231 : 因後面計算吸嘴都是用基準偏移 所以把個別偏移參數拿到後面
        }
    }

    Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]=iHPY1-HotPlateForm.YStart+1000+InArmOffSet[InOfsHP1]->GetY();
    Prod.iHP1LaserY              =iHPY1Laser-1000+InArmOffSet[InOfsHP1]->GetY();   //Steven 20140228 : 雷射測距功能

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;
            Prod.YInArm_Plate1_Pick[i][j]=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+InArmOffSet[InOfsHP1]->GetArmY(i, j);
        }
    }

    //Hotplate [0][0] position for base Suck
    Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]=iHPX2+HotPlateForm.XStart-9000+InArmOffSet[InOfsHP2]->GetX();
    Prod.iHP2LaserX                                  =iHPX2Laser+HotPlateForm.XStart-10000+InArmOffSet[InOfsHP2]->GetX();  //Steven 20140228 : 雷射測距功能

    if(IniConfig.bHotPlateMove1CM && HotPlateForm.bUseWideHotplate==false)
    {
        if(HOT_PLATE_POSITION==0)
            Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]=iHPX2+HotPlateForm.XStart-9000+1000+InArmOffSet[InOfsHP2]->GetX(); //ChungHung Add 20101025 CC_ASE_CL HotPlate Offset 10mm
        else
            Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]=iHPX2+HotPlateForm.XStart-9000-1000+InArmOffSet[InOfsHP2]->GetX(); //Steven 20160302
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;
            //Prod.XInArm_Plate2_Pick[i][j]=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+InArmOffSet[InOfsHP2]->GetArmX(i, j);
            //==>
            Prod.XInArm_Plate2_Pick[i][j]=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase];//Eastsun 20251231 : 因後面計算吸嘴都是用基準偏移 所以把個別偏移參數拿到後面
        }
    }

    Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]   =iHPY2-HotPlateForm.YStart+1000+InArmOffSet[InOfsHP2]->GetY();
    Prod.iHP2LaserY                                     =iHPY2Laser-1000+InArmOffSet[InOfsHP2]->GetY();   //Steven 20140228 : 雷射測距功能

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;
            Prod.YInArm_Plate1_Pick[i][j]=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+InArmOffSet[InOfsHP2]->GetArmY(i, j);
        }
    }

    Prod.ZInArm_Plate1_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmPlatePickZ2+InArmOffSet[InOfsHP1]->GetPickUp();
    Prod.ZInArm_Plate2_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmPlatePickZ2+InArmOffSet[InOfsHP2]->GetPickUp();

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            Prod.ZInArmSafe[i][j]=ZSafePos;                                     //safe
            if(j<4)
                iTemp=Tech.iInArmZHeightSub[i][j];
            else
                iTemp=Tech.iInArmZHeightSub_16[i][j-4];

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)                      //Place
            {
                if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)             //jou 2010-12-20 Pitch & Z 縮減為一個
                {
                    Prod.ZInArm_Plate1_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsLoader]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                    Prod.ZInArm_Plate2_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsLoader]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                }
                else
                {
                    Prod.ZInArm_Plate1_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsLoader]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                    Prod.ZInArm_Plate2_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsLoader]->GetPlace(i, j)+InArmOffSet[InOfsHP2]->GetPlace();
                }
            }
            else
            {
                if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)             //jou 2010-12-20 Pitch & Z 縮減為一個
                {
                    Prod.ZInArm_Plate1_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsHP1]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                    Prod.ZInArm_Plate2_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsHP2]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                }
                else
                {
                    Prod.ZInArm_Plate1_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsHP1]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                    Prod.ZInArm_Plate2_Place[i][j]=Tech.iInArmPlatePickZ2+iTemp+200+                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsHP2]->GetPlace(i, j)+InArmOffSet[InOfsHP2]->GetPlace();
                }
            }

            if(i==iInArmYBase && j==iInArmXBase)
                continue;

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)                      //Pick
            {                                                                                       //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
                Prod.ZInArm_Plate1_Pick[i][j]=Prod.ZInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+
                    iTemp+ InArmOffSet[InOfsLoader]->GetPickUp(i, j);                               //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset

                Prod.ZInArm_Plate2_Pick[i][j]=Prod.ZInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+
                    iTemp+InArmOffSet[InOfsLoader]->GetPickUp(i, j);                                //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
            }
            else
            {
                Prod.ZInArm_Plate1_Pick[i][j]=Prod.ZInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+
                    iTemp+InArmOffSet[InOfsHP1]->GetPickUp(i, j);                                   //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset

                Prod.ZInArm_Plate2_Pick[i][j]=Prod.ZInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+
                    iTemp+InArmOffSet[InOfsHP2]->GetPickUp(i, j);                                   //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
            }
        }
    }

    if(IniConfig.bHotPlateMove1CM && HotPlateForm.bUseWideHotplate==false)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                Prod.ZInArm_Plate1_Pick[i][j]+=300;
                Prod.ZInArm_Plate2_Pick[i][j]+=300;
                Prod.ZInArm_Plate1_Place[i][j]+=300;
                Prod.ZInArm_Plate2_Place[i][j]+=300;
            }
        }
    }
    //-------------------------------------------------------------------------------------------------
    //InArm  && Auto Clean
    //-------------------------------------------------------------------------------------------------
    if(IniConfig.bE43AutoCleanUseHotplate)                                      //Sam 20240905 : 修正 AutoClean Offset
        iAcOfs=InOfsHP1;
    else
        iAcOfs=InOfsAutoClean;

    if(IniConfig.bE43AutoCleanUseHotplate)                                      //Steven 20160629 : for AutoClean use Hotplate1
    {
        Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase] + TestIF.dAutoClean_XStart - HotPlateForm.XStart+InArmOffSet[iAcOfs]->GetX();  //Steven 20210408 : add HP offset for auto clean
        Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase] + TestIF.dAutoClean_YStart - HotPlateForm.YStart+InArmOffSet[iAcOfs]->GetY();

//        if(USE_Y_AUTO_PITCH==true)                                            //Steven 20241010 : Fixed for auto clean with HP
//        {
//            Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-=6000;       //Steven 20191217 : 修正Auto Clean吸放料位置
//        }
    }
    else if(bUseNewCleanModeKit)                                                //kevin 20150701 Autoclean for kit in tray pin1
    {
        Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmAutoCleanX+TestIF.dAutoClean_XStart+InArmOffSet[iAcOfs]->GetX()+50;     //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
        Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmAutoCleanY-TestIF.dAutoClean_YStart+InArmOffSet[iAcOfs]->GetY()-90;     //kevin 20150827  -50 --> -90
        if(TestIF_File.bAutoClean_UseNSKit)                                     //jou 2013-08-08 SPIL use NS Clean kit
        {
            Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+1500;
        }
    }
    else
    {
        Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmAutoCleanX+TestIF.dAutoClean_XStart-18000+InArmOffSet[iAcOfs]->GetX();      //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
        Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmAutoCleanY-TestIF.dAutoClean_YStart- 2250+InArmOffSet[iAcOfs]->GetY();

        if(TestIF_File.bAutoClean_UseTray)                                      //ChungHung 20130402 alter 如果選擇Clean Kit換成Tray
        {
            Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+8100;
            Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-400;
        }
        else if(TestIF_File.bAutoClean_UseNSKit)                                //jou 2013-08-08 SPIL use NS Clean kit
        {
            Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+1500;
        }

        if(USE_PICKER_COUNT==ep16Picker && USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)//Ztex 2024.03.13 Add HT-1032AT Auto Clean
        {
            Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmAutoCleanX+TestIF.dAutoClean_XStart+1680+InArmOffSet[InOfsAutoClean]->GetX();
            Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]=Tech.iInArmAutoCleanY-TestIF.dAutoClean_YStart-6000+InArmOffSet[InOfsAutoClean]->GetY();
        }
    }

//    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable && bUseNewCleanModeKit==false)//JerryYang 20190722 Mark掉, 修正Y-Pitch機型 auto clean teach點位計算錯誤 //kevin 20180227 autoclean for tray kit
//    {
//        Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-=6000;
//    }

    if(IniConfig.bE43AutoCleanUseHotplate)
    {
        Prod.ZInArm_AutoClean_Pick[iInArmYBase][iInArmXBase] =Tech.iInArmPlatePickZ2+InArmOffSet[iAcOfs]->GetPickUp();            //Steven 20231113 : InOfsAutoClean --> InOfsHP1
        Prod.ZInArm_AutoClean_Place[iInArmYBase][iInArmXBase]=Tech.iInArmPlatePickZ2+200+InArmOffSet[iAcOfs]->GetPlace();
    }
    else
    {
        Prod.ZInArm_AutoClean_Pick[iInArmYBase][iInArmXBase] =Teach.iAutoCleanPick +InArmOffSet[iAcOfs]->GetPickUp();     //kevin 20190305 change autoclean pos
        Prod.ZInArm_AutoClean_Place[iInArmYBase][iInArmXBase]=Teach.iAutoCleanPick +200+InArmOffSet[iAcOfs]->GetPlace();  //kevin 20190305 change autoclean pos
    }

    if(IniConfig.bE33InOutArmZOffsetSameOne==true)                              //Sam 20240905 : 修正 AutoClean Offset
        iAcOfs=InOfsLoader;

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;

            if(j<4)
                iTemp=Tech.iInArmZHeightSub[i][j];
            else
                iTemp=Tech.iInArmZHeightSub_16[i][j-4];

            Prod.ZInArm_AutoClean_Pick[i][j]=iTemp+                             //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                                             Prod.ZInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+InArmOffSet[iAcOfs]->GetPickUp(i, j);//Jimmychiu 20231201 : Clean pad 吸放加入base z高度
        }
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;

            if(j<4)
                iTemp=Tech.iInArmZHeightSub[i][j];
            else
                iTemp=Tech.iInArmZHeightSub_16[i][j-4];

            Prod.ZInArm_AutoClean_Place[i][j]=iTemp+                            //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                                              Prod.ZInArm_AutoClean_Place[iInArmYBase][iInArmXBase]+InArmOffSet[iAcOfs]->GetPlace(i, j);//Jimmychiu 20231201 : Clean pad 吸放加入base z高度
        }
    }
    //-------------------------------------------------------------------------------------------------
    //InArm  && Shuttle
    //-------------------------------------------------------------------------------------------------
    Prod.ZInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]=Tech.iInArmShuttlePlaceZ+InArmOffSet[InOfsInSh1]->GetPlace();//In arm to shuttle Z          //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)                         //jou 2010-12-20 Pitch & Z 縮減為一個
    {
        Prod.ZInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]=Tech.iInArmShuttlePlaceZ+InArmOffSet[InOfsInSh1]->GetPlace();
    }
    else
    {
        Prod.ZInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]=Tech.iInArmShuttlePlaceZ+InArmOffSet[InOfsInSh2]->GetPlace();
    }

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;

            if(j<4)
                iTemp=Tech.iInArmZHeightSub[i][j];
            else
                iTemp=Tech.iInArmZHeightSub_16[i][j-4];

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)
            {
                Prod.ZInArm_Shuttle1_Place[i][j]=Prod.ZInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]+iTemp+InArmOffSet[InOfsLoader]->GetPlace(i, j);//Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                Prod.ZInArm_Shuttle2_Place[i][j]=Prod.ZInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]+iTemp+InArmOffSet[InOfsLoader]->GetPlace(i, j);//Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
            }
            else
            {
                Prod.ZInArm_Shuttle1_Place[i][j]=Prod.ZInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]+iTemp+InArmOffSet[InOfsInSh1]->GetPlace(i, j);//Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                Prod.ZInArm_Shuttle2_Place[i][j]=Prod.ZInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]+iTemp+InArmOffSet[InOfsInSh2]->GetPlace(i, j);//Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
            }
        }
    }

    Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]=Tech.iInArmShuttle1X-iShuttleTempPos[0][0];//shuttle 1 right //jou 2014-06-10 iShuttleTempPos[0][1] -> iShuttleTempPos[0][0] 修正溫度補償位置錯誤
    Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]=Tech.iInArmShuttle1Y;
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;
            Prod.XInArm_Shuttle1_Place[i][j]=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
            Prod.YInArm_Shuttle1_Place[i][j]=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
        }
    }

    Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]=Tech.iInArmShuttle2X-iShuttleTempPos[1][0];//shuttle 2 right //jou 2014-06-10 iShuttleTempPos[1][1] -> iShuttleTempPos[1][0] 修正溫度補償位置錯誤
    Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]=Tech.iInArmShuttle2Y;
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;
            Prod.XInArm_Shuttle2_Place[i][j]=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
            Prod.YInArm_Shuttle2_Place[i][j]=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
        }
    }

    //-------------------------------------------------------------------------------------------------
    //InArm && Safe Point   //Steven 20091008
    //-------------------------------------------------------------------------------------------------
    //Hotplate : 2x
    //In & out Arm Y pitch : 60 & 63.5 mm
    if(USE_HOTPLATE_TYPE==1)                                                    //Hotplate 2.0x
    {
        Prod.iInArmSafeX=iHPX1+16000;
    }
    else
    {
        if(CUSTOMER_CODE==CC_ASE_SG &&
           UserDefForm[TrayForm.Loader.iTrayType].XDivision<=2)                 //Ifor 20240320 add:ASE SG 要求多移動50mm避免IC撞到
        {
            Prod.iInArmSafeX=iHPX1+5000;
        }
        else
        {
            Prod.iInArmSafeX=iHPX1;
        }
    }

    Prod.iInArmSafeY=Tech.iInArmShuttle1Y-9000;                                 //ChungHung 20140625 add 改位置

    //-------------------------------------------------------------------------------------------------
    //InArm Preciser Station
    //-------------------------------------------------------------------------------------------------
    if(USE_PRECISER==2)                                                         //Steven 20191113 : 使用HP當Preciser
    {
        Prod.iInArmPreciserX        =Tech.iInArmPreciserX;
    }
    else if(iPreciserInstallArea==1)                                            //Ifor 20191008 : add Preciser Install Area
    {
        Prod.iInArmPreciserX        =Tech.iInArmPreciserX+(TestIF_File.dPreciserXPitch*100/2);
    }
    else                                                                        //Frank 20180410 (Steven) : InArm Preciser Station
    {
        Prod.iInArmPreciserX        =Tech.iInArmPreciserX-6400+(TestIF_File.dPreciserXPitch*100/2);
    }

    Prod.iInArmPreciserY        =Tech.iInArmPreciserY;
//    Prod.iInArmPreciserPlaceZ   =Tech.iInArmPreciserPlaceZ;
    Prod.iPreciserOpenPitch     =Tech.iPreciserOpenPitch;
    Prod.iPreciserClosePitch    =Tech.iPreciserClosePitch;
    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(j<4)
                iTemp=Tech.iInArmZHeightSub[i][j];
            else
                iTemp=Tech.iInArmZHeightSub_16[i][j-4];

            Prod.iInArmPreciserPlaceZ[i][j] =iTemp+                             //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                                             InArmOffSet[InOfsLoader]->GetPlace(i, j)   +Tech.iInArmPreciserPlaceZ  +InArmOffSet[InOfsPreciser]->GetPlace();
            Prod.iInArmPreciserPickUpZ[i][j]=iTemp+                             //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                                             InArmOffSet[InOfsLoader]->GetPickUp(i, j)  +Tech.iInArmPreciserPlaceZ  +InArmOffSet[InOfsPreciser]->GetPickUp();
        }
    }

    //-------------------------------------------------------------------------------------------------
    // Rotate KIT
    //-------------------------------------------------------------------------------------------------
    int iInArmRotateTech_X[2]={iHPX1, iHPX2};                                   // 2012.06.11 , Joye , Rotate KIT
    int iInArmRotateTech_Y[2]={iHPY1, iHPY2};

    if(iRotate_In_Index>-1)                                                     //2013-04-12    Dell
    {
        Prod.iInArmRotateToHotPlateX = iInArmRotateTech_X[iRotate_In_Index] -9000 + InArmOffSet[iRotate_In_Index+InOfsHP1]->GetX();    // HotPlate 1
        Prod.iInArmRotateToHotPlateY = iInArmRotateTech_Y[iRotate_In_Index] +1000 + InArmOffSet[iRotate_In_Index+InOfsHP1]->GetY();

        Prod.iInArmRotateToHotPlate_PickZ[iInArmYBase][iInArmXBase]=Tech.iInArmPlatePickZ2+InArmOffSet[iRotate_In_Index+InOfsHP1]->GetPickUp();      //Steven 20141110 : [0][2] --> [iInArmYBase][iInArmXBase]
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(j<4)
                    iTemp=Tech.iInArmZHeightSub[i][j];
                else
                    iTemp=Tech.iInArmZHeightSub_16[i][j-4];

                if(IniConfig.bE33InOutArmZOffsetSameOne==true)                  //place
                {

                    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)         //jou 2010-12-20 Pitch & Z 縮減為一個
                    {
                        Prod.iInArmRotateToHotPlate_PlaceZ[i][j]=Tech.iInArmPlatePickZ2+iTemp+200      //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                            +InArmOffSet[InOfsLoader]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                    }
                    else
                    {
                        Prod.iInArmRotateToHotPlate_PlaceZ[i][j]=Tech.iInArmPlatePickZ2+iTemp+200      //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                            +InArmOffSet[InOfsLoader]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                    }
                }
                else
                {

                    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)         //jou 2010-12-20 Pitch & Z 縮減為一個
                    {
                        Prod.iInArmRotateToHotPlate_PlaceZ[i][j]=Tech.iInArmPlatePickZ2+iTemp+200      //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                            +InArmOffSet[iRotate_In_Index+InOfsHP1]->GetPlace(i, j)+InArmOffSet[InOfsHP1]->GetPlace();
                    }
                    else
                    {
                        Prod.iInArmRotateToHotPlate_PlaceZ[i][j]=Tech.iInArmPlatePickZ2+iTemp+200      //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                            +InArmOffSet[iRotate_In_Index+InOfsHP1]->GetPlace(i, j)+InArmOffSet[iRotate_In_Index+InOfsHP1]->GetPlace();
                    }
                }

                if(i==iInArmYBase && j==iInArmXBase)
                    continue;

                if(IniConfig.bE33InOutArmZOffsetSameOne==true)                  //pick
                {
                    Prod.iInArmRotateToHotPlate_PickZ[i][j]=Prod.iInArmRotateToHotPlate_PickZ[iInArmYBase][iInArmXBase]+iTemp+   //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[InOfsLoader]->GetPickUp(i, j);
                }
                else
                {
                    Prod.iInArmRotateToHotPlate_PickZ[i][j]=Prod.iInArmRotateToHotPlate_PickZ[iInArmYBase][iInArmXBase]+iTemp+   //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                        InArmOffSet[iRotate_In_Index+1]->GetPickUp(i, j);
                }
            }
        }
    }

    //-------------------------------------------------------------------------------------------------
    // 旋轉站;馬達版
    //-------------------------------------------------------------------------------------------------
    Prod.iInArm_RotateX     = Tech.M_In_iRotateX  + InArmOffSet[InOfsRotate_In]->GetX();
    Prod.iInArm_RotateY     = Tech.M_In_iRotateY  + InArmOffSet[InOfsRotate_In]->GetY();
    Prod.iIn_iRotateA       = Tech.M_In_iRotateA;
    Prod.iIn_iRotateA_Backlash  = Tech.M_In_iRotateA_Backlash;                  //RogerYang 20260113 : Rotator新增背隙補償

    Prod.iInArm_RotatePick[iInArmYBase][iInArmXBase]    =Tech.M_In_iRotatePick   + InArmOffSet[InOfsRotate_In]->GetPickUp();
    Prod.iInArm_RotatePlace[iInArmYBase][iInArmXBase]   =Tech.M_In_iRotatePlace  + InArmOffSet[InOfsRotate_In]->GetPlace();

    for(int i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(i==iInArmYBase && j==iInArmXBase)
                continue;

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)
            {
                Prod.iInArm_RotatePick[i][j]  =Prod.iInArm_RotatePick[iInArmYBase][iInArmXBase]  + Tech.iInArmZHeightSub[i][j] + InArmOffSet[InOfsLoader]->GetPickUp(i, j);     //Sam 20250204 : 修正 Rotate 吸嘴平衡問題
                Prod.iInArm_RotatePlace[i][j] =Prod.iInArm_RotatePlace[iInArmYBase][iInArmXBase] + Tech.iInArmZHeightSub[i][j] + InArmOffSet[InOfsLoader]->GetPlace(i, j);
            }
            else
            {
                Prod.iInArm_RotatePick[i][j]  =Prod.iInArm_RotatePick[iInArmYBase][iInArmXBase]  + Tech.iInArmZHeightSub[i][j] + InArmOffSet[InOfsRotate_In]->GetPickUp(i, j);
                Prod.iInArm_RotatePlace[i][j] =Prod.iInArm_RotatePlace[iInArmYBase][iInArmXBase] + Tech.iInArmZHeightSub[i][j] + InArmOffSet[InOfsRotate_In]->GetPlace(i, j);
            }
        }
    }

    Prod.iINARM_Y_PITCH=TestIF.iARM_Y_PITCH+IniConfig.iInArm60mmOffset;
    Prod.iINARM_X_PITCH=0;

    //------------------------------------------------------------------------------
    //Steven 20131002 : XY變距
    //------------------------------------------------------------------------------
    Prod.iInArmX40Pitch     =Tech.iInArmX40Pitch;
    Prod.iInArmX120Pitch    =Tech.iInArmX120Pitch;
    Prod.iInArmX40Pitch2    =Tech.iInArmX40Pitch2;
    Prod.iInArmX120Pitch2   =Tech.iInArmX120Pitch2;
    Prod.iInArmX40Pitch3    =Tech.iInArmX40Pitch3;
    Prod.iInArmX120Pitch3   =Tech.iInArmX120Pitch3;
    Prod.iInArmX40Pitch4    =Tech.iInArmX40Pitch4;
    Prod.iInArmX120Pitch4   =Tech.iInArmX120Pitch4;
    Prod.iInArmY15Pitch     =Tech.iInArmY15Pitch;
    Prod.iInArmY60Pitch     =Tech.iInArmY60Pitch;

    //------------------------------------------------------------------------------
    //Steven 20120809 : 吸嘴自動校正
    //------------------------------------------------------------------------------
    Prod.iInArmPickX        =Tech.iInArmPickX;
    Prod.iInArmPickY        =Tech.iInArmPickY;

    //------------------------------------------------------------------------------
    //Ifor 20151209 InArm Decay Teach點位
    //------------------------------------------------------------------------------
    Prod.iInArmDecay_X      =Tech.iInArmDecay_X;
    Prod.iInArmDecay_Y      =Tech.iInArmDecay_Y;

    //------------------------------------------------------------------------------
    //JimmyChiu 20220908 add Pickup Error Placement
    //------------------------------------------------------------------------------
    Prod.iInPlacementX      = Tech.iInPlacementX;
    Prod.iInPlacementY      = Tech.iInPlacementY;

    Prod.iInSmartSetup_X   =Tech.iInSmartSetup_X;                               //Ifor 20240423 add: Smart Setup In Arm X Teach 點位
    Prod.iInSmartSetup_Y   =Tech.iInSmartSetup_Y;                               //Ifor 20240423 add: Smart Setup In Arm Y Teach 點位
    Prod.iInSmartSetup_Z   =Tech.iInSmartSetup_Z;                               //Ifor 20240423 add: Smart Setup In Arm Z Teach 點位
    Prod.iOutSmartSetup_X  =Tech.iOutSmartSetup_X;                              //Ifor 20240423 add: Smart Setup Out Arm X Teach 點位
    Prod.iOutSmartSetup_Y  =Tech.iOutSmartSetup_Y;                              //Ifor 20240423 add: Smart Setup Out Arm Y Teach 點位
    Prod.iOutSmartSetup_Z  =Tech.iOutSmartSetup_Z;                              //Ifor 20240423 add: Smart Setup Out Arm Z Teach 點位
}
//------------------------------------------------------------------------------
void SetTechDataToProd_Index()
{
    if(bUser_Define_IndexZ_SafePos==true &&
       CosFunction.bUserDefineIndexZSafePos==true)                              //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
    {
        iGalil_Z_SafePos=dUserDefineIndexZSafePos;
    }
    else
    {
        iGalil_Z_SafePos=200;
    }

    //-------------------------------------------------------------------------------------------------
    // Index
    //-------------------------------------------------------------------------------------------------
    //Prod.All_TestZ_Test_Safe=Tech.iTestZDown+(int)(DeviceForm.fKitDepthOffset*100.0);    //Steven 20091116 : Add Kit Depth Offset
    if(CUSTOMER_CODE==CC_ASE_KaohSiung && Tech.iTestZDown<-1000)                //kevin 20190930 add 保護
        Tech.iTestZDown=-1000;
    Prod.All_TestZ_Test_Safe=Tech.iTestZDown;                                   //Steven 20100203
    Prod.iTestDown_Z1   = Prod.All_TestZ_Test_Safe;
    Prod.iTestDown_Z2   = Prod.All_TestZ_Test_Safe;

    Prod.TestY1_Front   = Tech.iHT9040TestY1_Front;
    Prod.TestY1_Middle  = Tech.iHT9040TestY1_Middle;
    Prod.TestY2_Rear    = Tech.iHT9040TestY2_Rear;
    Prod.TestY2_Middle  = Tech.iHT9040TestY2_Middle;
    Prod.TestY1_Middle_Home = Tech.iHT9040TestY1_Middle;                        //kevin 20171023 (wei) Y1 home Z phase
    Prod.TestY2_Middle_Home = Tech.iHT9040TestY2_Middle;                        //kevin 20171023 (wei) Y2 home Z phase

    Prod.TestY_Pre_MovePos = 5000;                                              //2013-01-15    Dell TestY偷跑距離先固定

    if(bUseTwoArm32Site==true)
    {
        if(TestIF_File.iYPitchOffsetMode==1)                                    //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            Prod.TestY1_Front  = Tech.iHT9040TestY1_Front-1000;
            Prod.TestY2_Rear   = Tech.iHT9040TestY2_Rear+1000;
        }
        else
        {
            Prod.TestY1_Front  = Tech.iHT9040TestY1_Front;                      //Steven 20140512 : For HT-9047
            Prod.TestY2_Rear   = Tech.iHT9040TestY2_Rear;
        }
    }

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
    {
        Prod.TestY1_Front_EndWaitPos=Tech.iHT9040TestY1_Middle;
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE && Tri_Temp_Machine==1)                  //Wenqi 20240516 Modify TestY1 Home at Shuttle
    {
        Prod.TestY1_Front_EndWaitPos=Tech.iHT9040TestY1_Front;
    }
    else
    {
        Prod.TestY1_Front_EndWaitPos=Tech.iHT9040TestY1_Front+2000;
    }

    if(DeviceForm.ContactMode==DropPlaceShiftContact)                           //ChungHung 20150528 add for 海思 _8Site1x4
    {
        Prod.TestY1_Front   = Tech.iHT9040TestY1_Front;
        Prod.TestY1_Middle  = Tech.iHT9040TestY1_Middle+(TestIF.dSiteYPitch/2);
        Prod.TestY2_Rear    = Tech.iHT9040TestY2_Rear;
        Prod.TestY2_Middle  = Tech.iHT9040TestY2_Middle-(TestIF.dSiteYPitch/2);

        Prod.TestY1_Middle_Home = Tech.iHT9040TestY1_Middle+(TestIF.dSiteYPitch/2);     //kevin 20171023 (wei) Y1 home Z phase
        Prod.TestY2_Middle_Home = Tech.iHT9040TestY2_Middle-(TestIF.dSiteYPitch/2);     //kevin 20171023 (wei) Y2 home Z phase
    }
    else
    {
        //jou 981208 start : NS7000 bias kit
        if(TestIF.bNS7000kit)
        {
            if(CosFunction.bNonCenterModeCanUseShtOffset && IniConfig.bA50Enable1x4BiasYOffset && TestIF.iTestMode==QualSite1X4)      //Steven 20200715 : for Tinton
            {
                Prod.TestY1_Middle  = Tech.iHT9040TestY1_Middle-1500;
                Prod.TestY2_Middle  = Tech.iHT9040TestY2_Middle+1500;

                Prod.TestY1_Middle_Home = Tech.iHT9040TestY1_Middle-1500;       //kevin 20171023 (wei) Y1 home Z phase
                Prod.TestY2_Middle_Home = Tech.iHT9040TestY2_Middle+1500;       //kevin 20171023 (wei) Y2 home Z phase
            }
            else
            {
                Prod.TestY1_Front   = Tech.iHT9040TestY1_Front+1500;
                Prod.TestY1_Middle  = Tech.iHT9040TestY1_Middle-1500;
                Prod.TestY2_Rear    = Tech.iHT9040TestY2_Rear-1500;
                Prod.TestY2_Middle  = Tech.iHT9040TestY2_Middle+1500;

                Prod.TestY1_Middle_Home = Tech.iHT9040TestY1_Middle-1500;       //kevin 20171023 (wei) Y1 home Z phase
                Prod.TestY2_Middle_Home = Tech.iHT9040TestY2_Middle+1500;       //kevin 20171023 (wei) Y2 home Z phase
            }
        }
        else
        {
            if(bUseTwoArm32Site==true)
            {
                Prod.TestY1_Middle  = Tech.iHT9040TestY1_Middle-7000;
                Prod.TestY2_Middle  = Tech.iHT9040TestY2_Middle+7000;

                Prod.TestY1_Middle_Home = Tech.iHT9040TestY1_Middle-7000;       //kevin 20171023 Y1 home Z phase
                Prod.TestY2_Middle_Home = Tech.iHT9040TestY2_Middle+7000;       //kevin 20171023 Y2 home Z phase
            }
        }
        //jou 981208 end
    }
    Prod.TestZ1_Safe         =Tech.iTestZ1ShutleWait;                           //Steven 20100203
    Prod.TestZ2_Safe         =Tech.iTestZ1ShutleWait;                           //Steven 20100203

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20170509 (wei) 新增KYEC回Home後最小上升高度100
    {
        if(MachineTypeChoice==Type_HT9046_LS)                                   //Ifor 20180822 (Steven) : Add KYEC LS 機型回Home後最小上升高度200
        {
            if(Prod.TestZ1_Safe<=iGalil_Z_SafePos)
                Prod.TestZ1_Safe=iGalil_Z_SafePos;
            if(Prod.TestZ2_Safe<=iGalil_Z_SafePos)
                Prod.TestZ2_Safe=iGalil_Z_SafePos;
        }
        else
        {
            if(Prod.TestZ1_Safe<=iGalil_Z_KYEC_SafePos)
                Prod.TestZ1_Safe=iGalil_Z_KYEC_SafePos;
            if(Prod.TestZ2_Safe<=iGalil_Z_KYEC_SafePos)
                Prod.TestZ2_Safe=iGalil_Z_KYEC_SafePos;
        }
    }
    else
    {
        if(Prod.TestZ1_Safe<=iGalil_Z_SafePos)                                  //Ifor 20160311 避免Home sensor 不亮
            Prod.TestZ1_Safe=iGalil_Z_SafePos;
        if(Prod.TestZ2_Safe<=iGalil_Z_SafePos)
            Prod.TestZ2_Safe=iGalil_Z_SafePos;
    }

    Prod.TestZ1_Pick        = Tech.iTestZ1ShutlePick+DeviceForm.IndexArmPick[0]+Offset.iIndexArmPickUp[0];
    Prod.TestZ2_Pick        = Tech.iTestZ2ShutlePick+DeviceForm.IndexArmPick[1]+Offset.iIndexArmPickUp[1];
    Prod.TestZ1_Place       = Tech.iTestZ1ShutlePick+DeviceForm.IndexPlace[0]+Offset.iIndexArmPlace[0];
    Prod.TestZ2_Place       = Tech.iTestZ2ShutlePick+DeviceForm.IndexPlace[1]+Offset.iIndexArmPlace[1];
    if(fAutoTeach->IsRun())                                                     //JimmyChiu 20211020 : Auto alignment mode
    {
        Prod.TestZ1_Pick+=fAutoTeach->GetShuttlePickUpOffsetWhenAutoTeach();
        Prod.TestZ2_Pick+=fAutoTeach->GetShuttlePickUpOffsetWhenAutoTeach();
    }

    //-------------------------------------------------------------------------------------------------
    #ifndef SOFT_SIMULTE
    if(MOT[MTestZ1].Motor->Enable && Prod.TestZ1_Pick<MOT[MTestZ1].IndexPickLimit)
    {
        Prod.TestZ1_Pick=Tech.iTestZ1ShutlePick;
        ShowMyMessage("Index Z1 pick height over limit,try modify database(motor.db)!", "Z1吸取高度OffSet超過教導高度,修正 Motor.DB!");
    }
    if(MOT[MTestZ1].Motor->Enable && Prod.TestZ1_Place<MOT[MTestZ1].IndexPickLimit)
    {
        Prod.TestZ1_Place=Tech.iTestZ1ShutlePick;
        ShowMyMessage("Index Z1 place height over limit,try modify database(motor.db)!", "Z1放IC高度OffSet超過教導高度,修正 Motor.DB!");
    }
    if(MOT[MTestZ2].Motor->Enable && Prod.TestZ2_Pick<MOT[MTestZ2].IndexPickLimit)
    {
        Prod.TestZ2_Pick=Tech.iTestZ2ShutlePick;
        ShowMyMessage("Index Z2 pick height over limit,try modify database(motor.db)!", "Z2吸取高度OffSet超過教導高度,修正 Motor.DB!");
    }
    if(MOT[MTestZ2].Motor->Enable && Prod.TestZ2_Place<MOT[MTestZ2].IndexPickLimit)
    {
        Prod.TestZ2_Place=Tech.iTestZ2ShutlePick;
        ShowMyMessage("Index Z2 place height over limit,try modify database(motor.db)!", "Z2放IC高度OffSet超過教導高度,修正 Motor.DB!");
    }
    #endif

    //-----------------------------------------------------------------------------
    // INDEX && LOAD POS
    //-----------------------------------------------------------------------------
    Prod.iLoadCellY1 = Teach.iLoadCellY1;                                       //kevin 20190907 Arm1 在load cell 位置
    Prod.iLoadCellY2 = Teach.iLoadCellY2;                                       //kevin 20190907 Arm2 在load cell 位置

    Prod.dLoadCellZ1Down = iUnitMultiply100(DeviceForm_File.dLoadCellZ1Down);   //kevin 20190907 Arm1 在load cell 位置
    Prod.dLoadCellZ2Down = iUnitMultiply100(DeviceForm_File.dLoadCellZ2Down);   //kevin 20190907 Arm2 在load cell 位置

    if(TestIF_File.iShuttleMode==1)                                             //20111114  Dell for Disable Index Arm    Start
    {
        if(IniConfig.bShuttleMode50)                                            //20111212  Dell
        {
            if(TestIF_File.iShuttle_Sel==0)                                     //20130225  Dell    modify for ATK
            {
                Prod.InSHT[1].iRight = Prod.InSHT[1].iLeft+5;                   //Steven 20221206 : +2 --> +5               //因為目前有1個pulse的range，所以關arm會hang住，+2不夠，馬達不會動
            }
            else
            {
                Prod.InSHT[0].iRight = Prod.InSHT[0].iLeft+5;                   //Steven 20221206 : +2 --> +5
            }
        }
        else if(IniConfig.bIndexArm2SupplyLight==true ||                        //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
                TestIF_File.bForEgisTecTest==true     ||                        //Steven 20140922 : Arm2當作指紋測試
                (IniConfig.bD58UseArm1PickPlaceArm2Test==true &&                //kevin 20150127 Arm1 下壓 arm2 測試
                 TestIF_File.bArm1PickPlaceArm2Test==true))                     //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
        {
            Prod.InSHT[1].iRight = Prod.InSHT[1].iLeft+5;                       //Steven 20221206 : +2 --> +5
        }
        else if(TestIF_File.iShuttle_Sel==0)
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //Ifor 20170509 (wei) 新增KYEC回Home後最小上升高度100
            {
                if(MachineTypeChoice==Type_HT9046_LS)                           //Ifor 20180822 (Steven) : Add KYEC LS 機型回Home後最小上升高度200
                {
                    Prod.TestZ2_Safe=iGalil_Z_SafePos;
                }
                else
                {
                    Prod.TestZ2_Safe=iGalil_Z_KYEC_SafePos;
                }
            }
            else
            {
                Prod.TestZ2_Safe=iGalil_Z_SafePos;
            }
            Prod.TestY2_Middle   =0;
            Prod.TestZ2_Test     =0;
            Prod.iTestDown_Z2    =0;
            Prod.InSHT[1].iRight = Prod.InSHT[1].iLeft+5;                       //Steven 20221206 : +2 --> +5
        }
        else
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //Ifor 20170509 (wei) 新增KYEC回Home後最小上升高度100
            {
                if(MachineTypeChoice==Type_HT9046_LS)                           //Ifor 20180822 (Steven) : Add KYEC LS 機型回Home後最小上升高度200
                {
                    Prod.TestZ1_Safe=iGalil_Z_SafePos;
                }
                else
                {
                    Prod.TestZ1_Safe=iGalil_Z_KYEC_SafePos;
                }
            }
            else
            {
                Prod.TestZ1_Safe=iGalil_Z_SafePos;
            }

            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                             //JimmyChiu 20220708 : add Index Arm Axis
            {
                Prod.TestY1_Middle=0;
            }
            else
            {
            }
            Prod.TestZ1_Test   =0;
            Prod.iTestDown_Z1  =0;
            Prod.InSHT[0].iRight = Prod.InSHT[0].iLeft+5;                       //Steven 20221206 : +2 --> +5
        }
    }
}
//------------------------------------------------------------------------------
void SetTechDataToProd_OutArm()
{
    double dbTrayThick=0.0;
    int iFix=0, iAuto=0;
    int iTemp=0;                                                                //Ztex 2023.12.15 Add Pitch X Home Twice
    //--------------------------------------------------------------------------
    //OutArm && Shuttle && auto && Fix
    //--------------------------------------------------------------------------//Steven 20240309 : fixed for out arm to sht offset
    Prod.XOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase] =Tech.iOutArmShuttle1X+iShuttleTempPos[0][1];//shuttle 1 right   //JerryYang 20251003 : offset在in/out arm裡面加
    Prod.YOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase] =Tech.iOutArmShuttle1Y;
    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            if(i==iOutArmYBase && j==iOutArmXBase)
                continue;
            Prod.XOutArm_Shuttle1_Pick[i][j]=Prod.XOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase];
            Prod.YOutArm_Shuttle1_Pick[i][j]=Prod.YOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase];
        }
    }

    Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]=Tech.iOutArmShuttle2X+iShuttleTempPos[1][1];//shuttle 2 right    //JerryYang 20251003 : offset在in/out arm裡面加
    Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]=Tech.iOutArmShuttle2Y;
    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            if(i==iOutArmYBase && j==iOutArmXBase)
                continue;
            Prod.XOutArm_Shuttle2_Pick[i][j]=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
            Prod.YOutArm_Shuttle2_Pick[i][j]=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
        }
    }

    Prod.ZOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]=Tech.iOutArmShuttlePickZ2+OutArmOffSet[OutOfsOutSh1]->GetPickUp();
    Prod.ZOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]=Tech.iOutArmShuttlePickZ2+OutArmOffSet[OutOfsOutSh2]->GetPickUp();

    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            if(i==iOutArmYBase && j==iOutArmXBase)
                continue;

            if(j<4)
                iTemp=Tech.iOutArmZHeightSub[i][j];
            else
                iTemp=Tech.iOutArmZHeightSub_16[i][j-4];

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)                      //jou 2010-06-21
            {
                Prod.ZOutArm_Shuttle1_Pick[i][j]=Prod.ZOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]+iTemp+ //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                    OutArmOffSet[OutOfsAuto1]->GetPickUp(i, j);
                Prod.ZOutArm_Shuttle2_Pick[i][j]=Prod.ZOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]+iTemp+ //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                    OutArmOffSet[OutOfsAuto1]->GetPickUp(i, j);
            }
            else
            {
                Prod.ZOutArm_Shuttle1_Pick[i][j]=Prod.ZOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]+iTemp+ //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                    OutArmOffSet[OutOfsOutSh1]->GetPickUp(i, j);
                Prod.ZOutArm_Shuttle2_Pick[i][j]=Prod.ZOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]+iTemp+ //Ztex 2023.12.20 Add In\Out Arm 16 Z Offset
                    OutArmOffSet[OutOfsOutSh2]->GetPickUp(i, j);
            }
        }
    }
    //-------------------------------------------------------------------------------------------------
    //OutArm && auto && Fix
    //-------------------------------------------------------------------------------------------------
    Prod.XOutArm_Auto_Place[eAuto1][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto1X+AutoForm[eAuto1]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsAuto1]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Auto_Place[eAuto1][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto1Y-AutoForm[eAuto1]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsAuto1]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Auto_Place[eAuto2][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto2X+AutoForm[eAuto2]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsAuto2]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Auto_Place[eAuto2][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto2Y-AutoForm[eAuto2]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsAuto2]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Auto_Place[eAuto3][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto3X+AutoForm[eAuto3]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsAuto3]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Auto_Place[eAuto3][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto3Y-AutoForm[eAuto3]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsAuto3]->GetY()-Prod.iTrayKitStartY;

    Prod.XOutArm_Auto_Place[eAuto4][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto4X+AutoForm[eAuto4]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsAuto4]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Auto_Place[eAuto4][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto4Y-AutoForm[eAuto4]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsAuto4]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Auto_Place[eAuto5][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto5X+AutoForm[eAuto5]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsAuto5]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Auto_Place[eAuto5][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto5Y-AutoForm[eAuto5]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsAuto5]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Auto_Place[eAuto6][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto6X+AutoForm[eAuto6]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsAuto6]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Auto_Place[eAuto6][iOutArmYBase][iOutArmXBase]=Tech.iOutArmAuto6Y-AutoForm[eAuto6]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsAuto6]->GetY()-Prod.iTrayKitStartY;

    for(int k=0; k<=iAutoCnt; k++)
    {
        iAuto=k+eAuto1;
        if(Prod.iTrayType[iAuto]!=tTrayAuto)
        {
            continue;
        }

        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(i==iOutArmYBase && j==iOutArmXBase)
                    continue;
                //Prod.XOutArm_Auto_Place[k][i][j]=Prod.XOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase]+OutArmOffSet[OutOfsAuto1+k]->GetArmX(i, j);
                //Prod.YOutArm_Auto_Place[k][i][j]=Prod.YOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase]+OutArmOffSet[OutOfsAuto1+k]->GetArmY(i, j);
                //=>
                Prod.XOutArm_Auto_Place[k][i][j]=Prod.XOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase];    //Eastsun 20251231 : 因後面計算吸嘴都是用基準偏移 把個別偏移參數拿到後面
                Prod.YOutArm_Auto_Place[k][i][j]=Prod.YOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase];
            }
        }

        if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
            Prod.ZOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceZ2+OutArmOffSet[OutOfsAuto1]->GetPlace();
        else
            Prod.ZOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceZ2+OutArmOffSet[k+OutOfsAuto1]->GetPlace();

        if(CosFunction.bUseTrayThickAdjustZHeight==true &&
           (IniConfig.bE70_UseTrayThickAdjustZHeight==true ||                   //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
            UserDefForm_File[0].bUseThickTray))                                 //JerryYang 20230620 : add厚tray選項
        {
            dbTrayThick=UserDefForm_File[TrayForm.Auto[iAuto].iTrayType].ZDepth*100;
            if(dbTrayThick<600)
                dbTrayThick=600.0;

            Prod.ZOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase]+=dbTrayThick-635;
        }

        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                Prod.ZOutArmSafe[i][j]=ZSafePos;
                if(i==iOutArmYBase && j==iOutArmXBase)
                    continue;

                if(j<4)
                    iTemp=Tech.iOutArmZHeightSub[i][j];
                else
                    iTemp=Tech.iOutArmZHeightSub_16[i][j-4];

                if(IniConfig.bE33InOutArmZOffsetSameOne==true)                  //jou 2010-06-21
                {
                    Prod.ZOutArm_Auto_Place[k][i][j]=Prod.ZOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsAuto1]->GetPlace(i, j);  //Ztex 2023.12.15 Add Pitch X Home Twice
                }
                else
                {
                    Prod.ZOutArm_Auto_Place[k][i][j]=Prod.ZOutArm_Auto_Place[k][iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsAuto1+k]->GetPlace(i, j);//Ztex 2023.12.15 Add Pitch X Home Twice
                }
            }
        }
    }

    Prod.XOutArm_Fix_Place[0][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix1X+AutoForm[eFix1]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsFix1]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Fix_Place[0][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix1Y-AutoForm[eFix1]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsFix1]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Fix_Place[1][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix2X+AutoForm[eFix2]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsFix2]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Fix_Place[1][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix2Y-AutoForm[eFix2]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsFix2]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Fix_Place[2][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix3X+AutoForm[eFix3]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsFix3]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Fix_Place[2][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix3Y-AutoForm[eFix3]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsFix3]->GetY()-Prod.iTrayKitStartY;

    Prod.XOutArm_Fix_Place[3][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix4X+AutoForm[eFix4]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsFix4]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Fix_Place[3][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix4Y-AutoForm[eFix4]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsFix4]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Fix_Place[4][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix5X+AutoForm[eFix5]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsFix5]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Fix_Place[4][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix5Y-AutoForm[eFix5]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsFix5]->GetY()-Prod.iTrayKitStartY;
    Prod.XOutArm_Fix_Place[5][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix6X+AutoForm[eFix6]->XStart+LoadForm->BlockXStart-KitPitchX+OutArmOffSet[OutOfsFix6]->GetX()+Prod.iTrayKitStartX;
    Prod.YOutArm_Fix_Place[5][iOutArmYBase][iOutArmXBase]=Tech.iOutArmFix6Y-AutoForm[eFix6]->YStart-LoadForm->BlockYStart+KitPitchY+OutArmOffSet[OutOfsFix6]->GetY()-Prod.iTrayKitStartY;

    Prod.iOutArmBinBoxX=Tech.iOutArmBinBoxX+OutArmOffSet[OutOfsFix3]->GetX();   //jou 2012-12-11 start : Bin Box
    Prod.iOutArmBinBoxY=Tech.iOutArmBinBoxY+OutArmOffSet[OutOfsFix3]->GetY();

    for(int k=0; k<iFixCnt; k++)                                                //Sam 20240423 : 修正溢位
    {
        iFix=k+eFix1;
        if(Prod.iTrayType[iFix]!=tTrayFix)
        {
            continue;
        }

        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(Prod.iTrayType[iFix]==tTrayFix)
                {
                    if(i==iOutArmYBase && j==iOutArmXBase)
                        continue;
                    //Prod.XOutArm_Fix_Place[k][i][j]=Prod.XOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]+OutArmOffSet[OutOfsFix1+k]->GetArmX(i, j);
                    //Prod.YOutArm_Fix_Place[k][i][j]=Prod.YOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]+OutArmOffSet[OutOfsFix1+k]->GetArmY(i, j);
                    //==>
                    Prod.XOutArm_Fix_Place[k][i][j]=Prod.XOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]; //Eastsun 20251231 : 因後面計算吸嘴都是用基準偏移 所以把個別偏移參數拿到後面
                    Prod.YOutArm_Fix_Place[k][i][j]=Prod.YOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase];
                }
            }
        }

        //jou 2010-12-20 Pitch & Z 縮減為一個
        if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
        {
            if(FIX3_FULL_PLACE==Fix3K_UseCylinder46LA && iFix==eFix2)           //ChungHung 20140722 add for HT9046LA //JimmyChiu 20220927 : Stepper Motor Control in Fix3
            {
                Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1+OutArmOffSet[OutOfsFix1+k]->GetPlace();
                Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1-200+OutArmOffSet[OutOfsFix1+k]->GetPickUp();   //Ifor 20161222 (jou) add Fix Pickup Offset
            }
            else if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor && iFix==eFix3)      //Jimmychiu 20230628 : 修正Fix3 Z方向距離
            {
                Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1+OutArmOffSet[OutOfsFix1+k]->GetPlace();
                Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1-200+OutArmOffSet[OutOfsFix1+k]->GetPickUp();   //Ifor 20161222 (jou) add Fix Pickup Offset
            }
            else
            {
                Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFixZ1+OutArmOffSet[OutOfsFix1]->GetPlace();
                Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFixZ1-200+OutArmOffSet[OutOfsFix1]->GetPickUp();      //Ifor 20161222 (jou) add Fix Pickup Offset
            }
        }
        else
        {
            if(FIX3_FULL_PLACE==Fix3K_UseCylinder46LA && iFix==eFix2)           //ChungHung 20140722 add for HT9046LA //JimmyChiu 20220927 : Stepper Motor Control in Fix3
            {
                Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1+OutArmOffSet[OutOfsFix1+k]->GetPlace();
                Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1-200+OutArmOffSet[OutOfsFix1+k]->GetPickUp();   //Ifor 20161222 (jou) add Fix Pickup Offset
            }
            else if(FIX3_FULL_PLACE==Fix3K_UseStepperMotor && iFix==eFix3)      //Jimmychiu 20230628 : 修正Fix3 Z方向距離
            {
                Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1+OutArmOffSet[OutOfsFix1+k]->GetPlace();
                Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFix2Z1-200+OutArmOffSet[OutOfsFix1+k]->GetPickUp();   //Ifor 20161222 (jou) add Fix Pickup Offset
            }
            else
            {
                Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFixZ1+OutArmOffSet[OutOfsFix1+k]->GetPlace();
                Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceFixZ1-200+OutArmOffSet[OutOfsFix1+k]->GetPickUp();    //Ifor 20161222 (jou) add Fix Pickup Offset
            }
        }

        if(CosFunction.bUseTrayThickAdjustZHeight==true &&
           (IniConfig.bE70_UseTrayThickAdjustZHeight==true ||                   //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
            UserDefForm_File[0].bUseThickTray))                                 //JerryYang 20230620 : add厚tray選項
        {
            dbTrayThick=UserDefForm_File[TrayForm.Auto[eFix1+k].iTrayType].ZDepth*100;
            if(dbTrayThick<600)
                dbTrayThick=600.0;
            Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]+=dbTrayThick-635;
            Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]+=dbTrayThick-635;
        }

        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(i==iOutArmYBase && j==iOutArmXBase)
                    continue;

                if(j<4)
                    iTemp=Tech.iOutArmZHeightSub[i][j];
                else
                    iTemp=Tech.iOutArmZHeightSub_16[i][j-4];

                if(IniConfig.bE33InOutArmZOffsetSameOne==true)                  //jou 2010-06-21
                {
                    Prod.ZOutArm_Fix_Place[k][i][j]=Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsAuto1]->GetPlace(i, j); //Ztex 2023.12.15 Add Pitch X Home Twice
                    Prod.ZOutArm_Fix_Pick [k][i][j]=Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsAuto1]->GetPickUp(i, j);//Ztex 2023.12.15 Add Pitch X Home Twice
                }
                else
                {
                    Prod.ZOutArm_Fix_Place[k][i][j]=Prod.ZOutArm_Fix_Place[k][iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsFix1+k]->GetPlace(i, j); //Ztex 2023.12.15 Add Pitch X Home Twice
                    Prod.ZOutArm_Fix_Pick [k][i][j]=Prod.ZOutArm_Fix_Pick [k][iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsFix1+k]->GetPickUp(i, j);//Ztex 2023.12.15 Add Pitch X Home Twice
                }
            }
        }
    }

    //-------------------------------------------------------------------------------------------------
    //OutArm && Safe Point   //Steven 20091008
    //-------------------------------------------------------------------------------------------------
    Prod.iOutArmSafeX=Tech.iOutArmAuto3X;
    if(USE_OUT_Y_IS_AUTO_PITCH==true)                                                  //Steven 20160530 : Out arm Y-Pitch wait change tray position  //JerryYang 20251218 : IN/OUT ARM支援不同模組
        Prod.iOutArmSafeY=Tech.iOutArmShuttle1Y+iOutArmShtYCenterPos;
    else
        Prod.iOutArmSafeY=Tech.iOutArmShuttle1Y;

    //-------------------------------------------------------------------------------------------------
    //Rotate KIT
    //-------------------------------------------------------------------------------------------------
    int iOutArmRotateTech_X[3] = { Tech.iOutArmAuto1X , Tech.iOutArmAuto2X , Tech.iOutArmAuto3X};     // 2012.06.11 , Joye , Rotate KIT
    int iOutArmRotateTech_Y[3] = { Tech.iOutArmAuto1Y , Tech.iOutArmAuto2Y , Tech.iOutArmAuto3Y};

    if(iRotate_Out_Tray6>-1) //2013-04-12    Dell
    {
        Prod.iOutArmRotateToUnloaderX = iOutArmRotateTech_X[iRotate_Out_Tray6] - KitPitchX + OutArmOffSet[iRotate_Out_Tray6+OutOfsAuto1]->GetX();
        Prod.iOutArmRotateToUnloaderY = iOutArmRotateTech_Y[iRotate_Out_Tray6] + KitPitchY + OutArmOffSet[iRotate_Out_Tray6+OutOfsAuto1]->GetY();

        if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
        {
            Prod.iOutArmRotateToUnloader_PlaceZ[iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceZ2+OutArmOffSet[OutOfsAuto1]->GetPlace();
            Prod.iOutArmRotateToUnloader_PickZ [iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceZ2-200+OutArmOffSet[OutOfsAuto1]->GetPlace();
        }
        else
        {
            Prod.iOutArmRotateToUnloader_PlaceZ[iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceZ2+OutArmOffSet[iRotate_Out_Tray6+OutOfsAuto1]->GetPlace();
            Prod.iOutArmRotateToUnloader_PickZ [iOutArmYBase][iOutArmXBase]=Tech.iOutArmPlaceZ2-200+OutArmOffSet[iRotate_Out_Tray6+OutOfsAuto1]->GetPlace();
        }

        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(i==iOutArmYBase && j==iOutArmXBase)
                    continue;

                if(j<4)
                    iTemp=Tech.iOutArmZHeightSub[i][j];
                else
                    iTemp=Tech.iOutArmZHeightSub_16[i][j-4];

                if(IniConfig.bE33InOutArmZOffsetSameOne==true)//jou 2010-06-21
                {
                    Prod.iOutArmRotateToUnloader_PlaceZ[i][j]=
                        Prod.iOutArmRotateToUnloader_PlaceZ[iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsAuto1]->GetPlace(i, j); //Ztex 2023.12.15 Add Pitch X Home Twice
                    Prod.iOutArmRotateToUnloader_PickZ[i][j]=
                        Prod.iOutArmRotateToUnloader_PickZ[iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[OutOfsAuto1]->GetPlace(i, j);  //Ztex 2023.12.15 Add Pitch X Home Twice
                }
                else
                {
                    Prod.iOutArmRotateToUnloader_PlaceZ[i][j]=
                        Prod.iOutArmRotateToUnloader_PlaceZ[iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[iRotate_Out_Tray6+OutOfsAuto1]->GetPlace(i, j); //Ztex 2023.12.15 Add Pitch X Home Twice
                    Prod.iOutArmRotateToUnloader_PickZ[i][j]=
                        Prod.iOutArmRotateToUnloader_PickZ[iOutArmYBase][iOutArmXBase]+iTemp+OutArmOffSet[iRotate_Out_Tray6+OutOfsAuto1]->GetPlace(i, j);  //Ztex 2023.12.15 Add Pitch X Home Twice
                }
            }
        }
    }

    //-------------------------------------------------------------------------------------------------
    // 旋轉站;馬達版
    //-------------------------------------------------------------------------------------------------
    Prod.iOutArm_RotateX    = Tech.M_Out_iRotateX + OutArmOffSet[OutOfsRotate_Out]->GetX();
    Prod.iOutArm_RotateY    = Tech.M_Out_iRotateY + OutArmOffSet[OutOfsRotate_Out]->GetY();
    Prod.iOut_iRotateA      = Tech.M_Out_iRotateA;
    Prod.iOut_iRotateA_Backlash = Tech.M_Out_iRotateA_Backlash;                 //RogerYang 20260113 : Rotator新增背隙補償

    Prod.iOutArm_RotatePick[iOutArmYBase][iOutArmXBase] =Tech.M_Out_iRotatePick  + OutArmOffSet[OutOfsRotate_Out]->GetPickUp();//GetPickUp(); kevin 20130706 in out arm offset
    Prod.iOutArm_RotatePlace[iOutArmYBase][iOutArmXBase]=Tech.M_Out_iRotatePlace + OutArmOffSet[OutOfsRotate_Out]->GetPlace();//GetPlace(); kevin 20130706 in out arm offset

    for(int i=0; i<OutArmSuck.iMaxRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMaxCol; j++)
        {
            if(i==iOutArmYBase && j==iOutArmXBase)
                continue;

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)                      //jou 2010-06-21
            {
                Prod.iOutArm_RotatePick[i][j] =Prod.iOutArm_RotatePick[iOutArmYBase][iOutArmXBase] + Tech.iOutArmZHeightSub[i][j] + OutArmOffSet[OutOfsAuto1]->GetPickUp(i, j); //Sam 20250204 : 修正 Rotate 吸嘴平衡問題
                Prod.iOutArm_RotatePlace[i][j]=Prod.iOutArm_RotatePlace[iOutArmYBase][iOutArmXBase]+ Tech.iOutArmZHeightSub[i][j] + OutArmOffSet[OutOfsAuto1]->GetPlace(i, j);
            }
            else
            {
                Prod.iOutArm_RotatePick[i][j] =Prod.iOutArm_RotatePick[iOutArmYBase][iOutArmXBase] + Tech.iOutArmZHeightSub[i][j] + OutArmOffSet[OutOfsRotate_Out]->GetPickUp(i, j);//GetPickUp(); kevin 20130706 in out arm offset
                Prod.iOutArm_RotatePlace[i][j]=Prod.iOutArm_RotatePlace[iOutArmYBase][iOutArmXBase]+ Tech.iOutArmZHeightSub[i][j] + OutArmOffSet[OutOfsRotate_Out]->GetPlace(i, j);//GetPlace(); kevin 20130706 in out arm offset
            }
        }
    }

    Prod.iOutARM_Y_PITCH=TestIF.iARM_Y_PITCH+IniConfig.iOutArm60mmOffset;
    Prod.iOutARM_X_PITCH=0;
    Prod.iOutArmX40Pitch    =Tech.iOutArmX40Pitch;
    Prod.iOutArmX120Pitch   =Tech.iOutArmX120Pitch;
    Prod.iOutArmX40Pitch2   =Tech.iOutArmX40Pitch2;
    Prod.iOutArmX120Pitch2  =Tech.iOutArmX120Pitch2;
    Prod.iOutArmX40Pitch3   =Tech.iOutArmX40Pitch3;
    Prod.iOutArmX120Pitch3  =Tech.iOutArmX120Pitch3;
    Prod.iOutArmX40Pitch4   =Tech.iOutArmX40Pitch4;
    Prod.iOutArmX120Pitch4  =Tech.iOutArmX120Pitch4;
    Prod.iOutArmY15Pitch    =Tech.iOutArmY15Pitch;
    Prod.iOutArmY60Pitch    =Tech.iOutArmY60Pitch;

    Prod.iOutArmPickX       =Tech.iOutArmPickX;
    Prod.iOutArmPickY       =Tech.iOutArmPickY;
    Prod.iOutArmDecay_X     =Tech.iOutArmDecay_X;                               //Ifor 20151209 OutArm Decay X Teach點位
    Prod.iOutArmDecay_Y     =Tech.iOutArmDecay_Y;                               //Ifor 20151209OutnArm Decay Y Teach點位

    //以下得保持在最下面-------------
    for(int i=0; i<MAX_ARM_Row; i++)                                            //Steven 20141029 : 將XStart & YStart改為Prod
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            Prod.XStart[eAuto1][i][j]=Prod.XOutArm_Auto_Place[0][i][j];
            Prod.XStart[eAuto2][i][j]=Prod.XOutArm_Auto_Place[1][i][j];
            Prod.XStart[eAuto3][i][j]=Prod.XOutArm_Auto_Place[2][i][j];
            Prod.XStart[eAuto4][i][j]=Prod.XOutArm_Auto_Place[3][i][j];
            Prod.XStart[eAuto5][i][j]=Prod.XOutArm_Auto_Place[4][i][j];
            Prod.XStart[eAuto6][i][j]=Prod.XOutArm_Auto_Place[5][i][j];
            Prod.XStart[eFix1 ][i][j]=Prod.XOutArm_Fix_Place[0][i][j];
            Prod.XStart[eFix2 ][i][j]=Prod.XOutArm_Fix_Place[1][i][j];
            Prod.XStart[eFix3 ][i][j]=Prod.XOutArm_Fix_Place[2][i][j];
            if(iFixRight==eFix3)                                                //Steven 20240426 : 修正Fix分上下盤的點位問題
            {
                Prod.XStart[eFix4 ][i][j]=Prod.XOutArm_Fix_Place[0][i][j];
                Prod.XStart[eFix5 ][i][j]=Prod.XOutArm_Fix_Place[1][i][j];
                Prod.XStart[eFix6 ][i][j]=Prod.XOutArm_Fix_Place[2][i][j];
            }
            else
            {
                Prod.XStart[eFix4 ][i][j]=Prod.XOutArm_Fix_Place[3][i][j];
                Prod.XStart[eFix5 ][i][j]=Prod.XOutArm_Fix_Place[4][i][j];
                Prod.XStart[eFix6 ][i][j]=Prod.XOutArm_Fix_Place[5][i][j];
            }
            Prod.XStart[eFix7 ][i][j]=Prod.XOutArm_Fix_Place[0][i][j];
            Prod.XStart[eFix8 ][i][j]=Prod.XOutArm_Fix_Place[1][i][j];
            Prod.XStart[eFix9 ][i][j]=Prod.XOutArm_Fix_Place[2][i][j];
            Prod.XStart[eFix10][i][j]=Prod.XOutArm_Fix_Place[3][i][j];
            Prod.XStart[eFix11][i][j]=Prod.XOutArm_Fix_Place[4][i][j];
            Prod.XStart[eFix12][i][j]=Prod.XOutArm_Fix_Place[5][i][j];

            Prod.YStart[eAuto1][i][j]=Prod.YOutArm_Auto_Place[0][i][j];
            Prod.YStart[eAuto2][i][j]=Prod.YOutArm_Auto_Place[1][i][j];
            Prod.YStart[eAuto3][i][j]=Prod.YOutArm_Auto_Place[2][i][j];
            Prod.YStart[eAuto4][i][j]=Prod.YOutArm_Auto_Place[3][i][j];
            Prod.YStart[eAuto5][i][j]=Prod.YOutArm_Auto_Place[4][i][j];
            Prod.YStart[eAuto6][i][j]=Prod.YOutArm_Auto_Place[5][i][j];
            Prod.YStart[eFix1 ][i][j]=Prod.YOutArm_Fix_Place[0][i][j];
            Prod.YStart[eFix2 ][i][j]=Prod.YOutArm_Fix_Place[1][i][j];
            Prod.YStart[eFix3 ][i][j]=Prod.YOutArm_Fix_Place[2][i][j];
            if(iFixRight==eFix3)                                                //Steven 20240426 : 修正Fix分上下盤的點位問題
            {
                Prod.YStart[eFix4 ][i][j]=Prod.YOutArm_Fix_Place[0][i][j];
                Prod.YStart[eFix5 ][i][j]=Prod.YOutArm_Fix_Place[1][i][j];
                Prod.YStart[eFix6 ][i][j]=Prod.YOutArm_Fix_Place[2][i][j];
            }
            else
            {
                Prod.YStart[eFix4 ][i][j]=Prod.YOutArm_Fix_Place[3][i][j];
                Prod.YStart[eFix5 ][i][j]=Prod.YOutArm_Fix_Place[4][i][j];
                Prod.YStart[eFix6 ][i][j]=Prod.YOutArm_Fix_Place[5][i][j];
            }
            Prod.YStart[eFix7 ][i][j]=Prod.YOutArm_Fix_Place[0][i][j];
            Prod.YStart[eFix8 ][i][j]=Prod.YOutArm_Fix_Place[1][i][j];
            Prod.YStart[eFix9 ][i][j]=Prod.YOutArm_Fix_Place[2][i][j];
            Prod.YStart[eFix10][i][j]=Prod.YOutArm_Fix_Place[3][i][j];
            Prod.YStart[eFix11][i][j]=Prod.YOutArm_Fix_Place[4][i][j];
            Prod.YStart[eFix12][i][j]=Prod.YOutArm_Fix_Place[5][i][j];

            Prod.ZPlace[eAuto1][i][j]=Prod.ZOutArm_Auto_Place[0][i][j];
            Prod.ZPlace[eAuto2][i][j]=Prod.ZOutArm_Auto_Place[1][i][j];
            Prod.ZPlace[eAuto3][i][j]=Prod.ZOutArm_Auto_Place[2][i][j];
            Prod.ZPlace[eAuto4][i][j]=Prod.ZOutArm_Auto_Place[3][i][j];
            Prod.ZPlace[eAuto5][i][j]=Prod.ZOutArm_Auto_Place[4][i][j];
            Prod.ZPlace[eAuto6][i][j]=Prod.ZOutArm_Auto_Place[5][i][j];
            Prod.ZPlace[eFix1 ][i][j]=Prod.ZOutArm_Fix_Place[0][i][j];
            Prod.ZPlace[eFix2 ][i][j]=Prod.ZOutArm_Fix_Place[1][i][j];
            Prod.ZPlace[eFix3 ][i][j]=Prod.ZOutArm_Fix_Place[2][i][j];
            if(iFixRight==eFix3)                                                //Steven 20240426 : 修正Fix分上下盤的點位問題
            {
                Prod.ZPlace[eFix4 ][i][j]=Prod.ZOutArm_Fix_Place[0][i][j];
                Prod.ZPlace[eFix5 ][i][j]=Prod.ZOutArm_Fix_Place[1][i][j];
                Prod.ZPlace[eFix6 ][i][j]=Prod.ZOutArm_Fix_Place[2][i][j];
            }
            else
            {
                Prod.ZPlace[eFix4 ][i][j]=Prod.ZOutArm_Fix_Place[3][i][j];
                Prod.ZPlace[eFix5 ][i][j]=Prod.ZOutArm_Fix_Place[4][i][j];
                Prod.ZPlace[eFix6 ][i][j]=Prod.ZOutArm_Fix_Place[5][i][j];
            }
            Prod.ZPlace[eFix7 ][i][j]=Prod.ZOutArm_Fix_Place[0][i][j];
            Prod.ZPlace[eFix8 ][i][j]=Prod.ZOutArm_Fix_Place[1][i][j];
            Prod.ZPlace[eFix9 ][i][j]=Prod.ZOutArm_Fix_Place[2][i][j];
            Prod.ZPlace[eFix10][i][j]=Prod.ZOutArm_Fix_Place[3][i][j];
            Prod.ZPlace[eFix11][i][j]=Prod.ZOutArm_Fix_Place[4][i][j];
            Prod.ZPlace[eFix12][i][j]=Prod.ZOutArm_Fix_Place[5][i][j];

            Prod.ZPick[eAuto1][i][j]=Prod.ZOutArm_Auto_Place[0][i][j];
            Prod.ZPick[eAuto2][i][j]=Prod.ZOutArm_Auto_Place[1][i][j];
            Prod.ZPick[eAuto3][i][j]=Prod.ZOutArm_Auto_Place[2][i][j];
            Prod.ZPick[eAuto4][i][j]=Prod.ZOutArm_Auto_Place[3][i][j];
            Prod.ZPick[eAuto5][i][j]=Prod.ZOutArm_Auto_Place[4][i][j];
            Prod.ZPick[eAuto6][i][j]=Prod.ZOutArm_Auto_Place[5][i][j];

            Prod.ZPick[eFix1 ][i][j]=Prod.ZOutArm_Fix_Pick[0][i][j];
            Prod.ZPick[eFix2 ][i][j]=Prod.ZOutArm_Fix_Pick[1][i][j];
            Prod.ZPick[eFix3 ][i][j]=Prod.ZOutArm_Fix_Pick[2][i][j];
            if(iFixRight==eFix3)                                                //Steven 20240426 : 修正Fix分上下盤的點位問題
            {
                Prod.ZPick[eFix4 ][i][j]=Prod.ZOutArm_Fix_Pick[0][i][j];
                Prod.ZPick[eFix5 ][i][j]=Prod.ZOutArm_Fix_Pick[1][i][j];
                Prod.ZPick[eFix6 ][i][j]=Prod.ZOutArm_Fix_Pick[2][i][j];
            }
            else
            {
                Prod.ZPick[eFix4 ][i][j]=Prod.ZOutArm_Fix_Pick[3][i][j];
                Prod.ZPick[eFix5 ][i][j]=Prod.ZOutArm_Fix_Pick[4][i][j];
                Prod.ZPick[eFix6 ][i][j]=Prod.ZOutArm_Fix_Pick[5][i][j];
            }
            Prod.ZPick[eFix7 ][i][j]=Prod.ZOutArm_Fix_Pick[0][i][j];
            Prod.ZPick[eFix8 ][i][j]=Prod.ZOutArm_Fix_Pick[1][i][j];
            Prod.ZPick[eFix9 ][i][j]=Prod.ZOutArm_Fix_Pick[2][i][j];
            Prod.ZPick[eFix10][i][j]=Prod.ZOutArm_Fix_Pick[3][i][j];
            Prod.ZPick[eFix11][i][j]=Prod.ZOutArm_Fix_Pick[4][i][j];
            Prod.ZPick[eFix12][i][j]=Prod.ZOutArm_Fix_Pick[5][i][j];
        }
    }
}
//------------------------------------------------------------------------------
void SetTechDataToProd_TrayArm()
{
    //-------------------------------------------------------------------------------------------------
    //TrayArm and Track
    //-------------------------------------------------------------------------------------------------
    //jou 2010-08-03 offset tray arm 調整 offset,loader處隱藏,只保留Tray set.
    if(CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_ASE_KaohSiung_K12)  //Steven 20131101 : Add ASE-K12
        Prod.iXTrayLoad         =Tech.iTrayXLoader+LoadForm->iPickUp;
    else
        Prod.iXTrayLoad         =Tech.iTrayXLoader+LoadForm->iPickUp        +Offset.iTrayArmX[etLoader];

    Prod.iXTrayEmpty            =Tech.iTrayXEmpty+EmptyForm->iPickUp        +Offset.iTrayArmX[etEmpty];
    Prod.iXTraySafty            =Tech.iTrayXEmpty+EmptyForm->iPickUp        -2000;
    Prod.iXTrayColor            =Tech.iTrayXColor+EmptyForm->iPickUp        +Offset.iTrayArmX[etColor];
    Prod.iXTrayAuto[eAuto1]     =Tech.iTrayXAuto1+AutoForm[eAuto1]->iPickUp +Offset.iTrayArmX[etAuto1];
    Prod.iXTrayAuto[eAuto2]     =Tech.iTrayXAuto2+AutoForm[eAuto2]->iPickUp +Offset.iTrayArmX[etAuto2];
    Prod.iXTrayAuto[eAuto3]     =Tech.iTrayXAuto3+AutoForm[eAuto3]->iPickUp +Offset.iTrayArmX[etAuto3];
    Prod.iXTrayAuto[eAuto4]     =Tech.iTrayXAuto4+AutoForm[eAuto4]->iPickUp +Offset.iTrayArmX[etAuto4]; //Steven 20230907 : For HT-9011UC
    Prod.iXTrayAuto[eAuto5]     =Tech.iTrayXAuto5+AutoForm[eAuto5]->iPickUp +Offset.iTrayArmX[etAuto5];
    Prod.iXTrayAuto[eAuto6]     =Tech.iTrayXAuto6+AutoForm[eAuto6]->iPickUp +Offset.iTrayArmX[etAuto6];
    Prod.iXTrayClean            =Tech.iTrayXClean;                              //wei 20150826 拍拍Tray X軸 Teach
    Prod.iXTrayOCR              =Tech.iTrayXOCR;                                //wei 20151001 OCR拍拍Tray X軸 Teach

    Prod.iXTrayLoad_ART         =Tech.iTrayXLoader+LoadForm->iPickUp        +Offset.iTrayArmX_ART[etLoader];//kevin 20170831 (Steven) ART Auto->Load offset
    Prod.iXTrayEmpty_ART        =Tech.iTrayXEmpty+EmptyForm->iPickUp        +Offset.iTrayArmX_ART[etEmpty ];//kevin 20170831 (Steven) ART Auto->Load offset
    Prod.iXTrayColor_ART        =Tech.iTrayXColor+EmptyForm->iPickUp        +Offset.iTrayArmX_ART[etColor ];//kevin 20170831 (Steven) ART Auto->Load offset
    Prod.iXTrayAuto_ART[eAuto1] =Tech.iTrayXAuto1+AutoForm[eAuto1]->iPickUp +Offset.iTrayArmX_ART[etAuto1 ];//kevin 20170831 (Steven) ART Auto->Load offset
    Prod.iXTrayAuto_ART[eAuto2] =Tech.iTrayXAuto2+AutoForm[eAuto2]->iPickUp +Offset.iTrayArmX_ART[etAuto2 ];//kevin 20170831 (Steven) ART Auto->Load offset
    Prod.iXTrayAuto_ART[eAuto3] =Tech.iTrayXAuto3+AutoForm[eAuto3]->iPickUp +Offset.iTrayArmX_ART[etAuto3 ];//kevin 20170831 (Steven) ART Auto->Load offset
    Prod.iXTrayAuto_ART[eAuto4] =Tech.iTrayXAuto4+AutoForm[eAuto4]->iPickUp +Offset.iTrayArmX_ART[etAuto4 ];//Steven 20230907 : For HT-9011UC
    Prod.iXTrayAuto_ART[eAuto5] =Tech.iTrayXAuto5+AutoForm[eAuto5]->iPickUp +Offset.iTrayArmX_ART[etAuto5 ];
    Prod.iXTrayAuto_ART[eAuto6] =Tech.iTrayXAuto6+AutoForm[eAuto6]->iPickUp +Offset.iTrayArmX_ART[etAuto6 ];

    for(int i=0; i<6; i++) //為了不修改Tech, 所以保持是6                        //Steven 20190813 : 入Tray改用步進馬達
    {
        Prod.TrayZ_Up[i] =Tech.iTrayLoaderZ[i]+200;
        Prod.TrayZ_Mid[i]=Tech.iTrayLoaderZ[i]-UserDefForm[0].ZDepth+Offset.dTrayZseparate[i];  //JerryYang 20200805 fix tray z motor offset問題
    }

    Prod.TrayZ_Up [etAuto4]=Tech.iTrayAuto4Z[0]+200;                            //Steven 20230907 : For HT-9011UC
    Prod.TrayZ_Mid[etAuto4]=Tech.iTrayAuto4Z[0]-UserDefForm[0].ZDepth+Offset.dTrayZseparate[etAuto4];
    Prod.TrayZ_Up [etAuto5]=Tech.iTrayAuto4Z[1]+200;
    Prod.TrayZ_Mid[etAuto5]=Tech.iTrayAuto4Z[1]-UserDefForm[0].ZDepth+Offset.dTrayZseparate[etAuto5];
    Prod.TrayZ_Up [etAuto6]=Tech.iTrayAuto4Z[2]+200;
    Prod.TrayZ_Mid[etAuto6]=Tech.iTrayAuto4Z[2]-UserDefForm[0].ZDepth+Offset.dTrayZseparate[etAuto6];

    if(IniConfig.bP06_LoaderUseCarrierTray==false)                              //Frank 20161026 : Fixed for block tray  //JerryYang 20170830 (wei) 移到上面,避免造成置偏
    {
        LoadForm->BlockXItem=1;
        LoadForm->BlockYItem=1;
        LoadForm->BlockXStart=0.0;
        LoadForm->BlockYStart=0.0;
        LoadForm->BlockPitchX=0.0;
        LoadForm->BlockPitchY=0.0;
    }

    Prod.iTrayDeviceCnt[eAuto1] =Tech.iTrayXAuto1+(LoadForm->iPickUp);          //Sam 20190405 : Tray Decive Count
    Prod.iTrayDeviceCnt[eAuto2] =Tech.iTrayXAuto2+(LoadForm->iPickUp);          //Sam 20190405 : Tray Decive Count
    Prod.iTrayDeviceCnt[eAuto3] =Tech.iTrayXAuto3+(LoadForm->iPickUp);          //Sam 20190405 : Tray Decive Count
    Prod.iTrayDeviceCnt[eAuto4] =Tech.iTrayXAuto4+(LoadForm->iPickUp);          //Sam 20190405 : Tray Decive Count
    Prod.iTrayDeviceCnt[eAuto5] =Tech.iTrayXAuto5+(LoadForm->iPickUp);          //Sam 20190405 : Tray Decive Count
    Prod.iTrayDeviceCnt[eAuto6] =Tech.iTrayXAuto6+(LoadForm->iPickUp);          //Sam 20190405 : Tray Decive Count

    //-------------------------------------------------------------------------------------------------
    //wei 20161219 Tray Mapping
    //-------------------------------------------------------------------------------------------------
    Prod.iTrayMapping           =Tech.iTrayXLoader+(LoadForm->iPickUp*2);
    Prod.iTrayID                =Tech.iTrayXLoader+(LoadForm->iPickUp*2);       //wei 20170608

    //-------------------------------------------------------------------------------------------------
    //wei 20180702 MR
    //-------------------------------------------------------------------------------------------------
    for(int i=0; i<10; i++)
    {
        Prod.iCassetteArmX[i]=Tech.iCassetteArmX[i];
        Prod.iCassetteArmZ[i]=Tech.iCassetteArmZ[i];

        Prod.iTrayArmZPnP[i]=Tech.iTrayArmZPnP[i];
    }

    for(int i=0; i<3; i++)
    {
        Prod.iLoadPortZ[i]=Tech.iLoadPortZ[i];
    }

    for(int i=0; i<6; i++)
    {
        Prod.iStackedTrayX[i]=Tech.iStackedTrayX[i];
        Prod.iStackedTrayZ[i]=Tech.iStackedTrayZ[i];
    }

    Prod.iTrayBracketZ[0]=Tech.iTrayBracketZ[0];
    Prod.iTrayBracketZ[1]=Tech.iTrayBracketZ[1];

    //-------------------------------------------------------------------------------------------------
    //Sam 20190112 LM
    //-------------------------------------------------------------------------------------------------
    for(int i=0; i<5; i++)
    {
        Prod.iLoadRobotZ[i]=Tech.iLoadRobotZ[i];
        Prod.iUnloadRobotZ[i]=Tech.iUnloadRobotZ[i];
    }

    //-------------------------------------------------------------------------------------------------
    //JerryYang 20220909 : add magazine
    //-------------------------------------------------------------------------------------------------
    if(TestIF_File.iMagDisplayOrder==0)
    {
        Prod.iMagazineTrayPos[0]     = Tech.iMagazineTray1Pos-2800*0;           //Mag1
        Prod.iMagazineTrayPos[1]     = Tech.iMagazineTray1Pos-2800*1;
        Prod.iMagazineTrayPos[2]     = Tech.iMagazineTray1Pos-2800*2;
        Prod.iMagazineTrayPos[3]     = Tech.iMagazineTray1Pos-2800*3;
        Prod.iMagazineTrayPos[4]     = Tech.iMagazineTray1Pos-2800*4;
        Prod.iMagazineTrayPos[5]     = Tech.iMagazineTray1Pos-2800*5;
        Prod.iMagazineTrayPos[6]     = Tech.iMagazineTray1Pos-2800*6;
        Prod.iMagazineTrayPos[7]     = Tech.iMagazineTray1Pos-2800*7;
        Prod.iMagazineTrayPos[8]     = Tech.iMagazineTray1Pos-2800*8;
        Prod.iMagazineTrayPos[9]     = Tech.iMagazineTray1Pos-2800*9;
        Prod.iMagazineTrayPos[10]    = Tech.iMagazineTray1Pos-2800*10;
        Prod.iMagazineTrayPos[11]    = Tech.iMagazineTray1Pos-2800*11;
        Prod.iMagazineTrayPos[12]    = Tech.iMagazineTray1Pos-2800*12;
        Prod.iMagazineTrayPos[13]    = Tech.iMagazineTray1Pos-2800*13;              //Mag14
        Prod.iMagazineTrayPos[14]    = Tech.iMagazineStandbyPos;      //Standby Position
    }
    else
    {
        //Ifor 20240124 KYEC要求Magazine位置相反
        //==>
        Prod.iMagazineTrayPos[0]     = Tech.iMagazineTray1Pos-2800*13;           //Mag1
        Prod.iMagazineTrayPos[1]     = Tech.iMagazineTray1Pos-2800*12;
        Prod.iMagazineTrayPos[2]     = Tech.iMagazineTray1Pos-2800*11;
        Prod.iMagazineTrayPos[3]     = Tech.iMagazineTray1Pos-2800*10;
        Prod.iMagazineTrayPos[4]     = Tech.iMagazineTray1Pos-2800*9;
        Prod.iMagazineTrayPos[5]     = Tech.iMagazineTray1Pos-2800*8;
        Prod.iMagazineTrayPos[6]     = Tech.iMagazineTray1Pos-2800*7;
        Prod.iMagazineTrayPos[7]     = Tech.iMagazineTray1Pos-2800*6;
        Prod.iMagazineTrayPos[8]     = Tech.iMagazineTray1Pos-2800*5;
        Prod.iMagazineTrayPos[9]     = Tech.iMagazineTray1Pos-2800*4;
        Prod.iMagazineTrayPos[10]    = Tech.iMagazineTray1Pos-2800*3;
        Prod.iMagazineTrayPos[11]    = Tech.iMagazineTray1Pos-2800*2;
        Prod.iMagazineTrayPos[12]    = Tech.iMagazineTray1Pos-2800*1;
        Prod.iMagazineTrayPos[13]    = Tech.iMagazineTray1Pos-2800*0;          //Mag14
        Prod.iMagazineTrayPos[14]    = Tech.iMagazineStandbyPos;      //Standby Position
        //<==
        //Ifor 20240124 KYEC要求Magazine位置相反
    }

    Prod.iLDCassetteFront               =Tech.iLDCassetteFront;                 //Frank 20251217 add
    Prod.iLDCassetteFrontBack           =Tech.iLDCassetteFrontBack;
    Prod.iLDCassetteRear                =Tech.iLDCassetteRear;
    Prod.iLDCassetteRearBack            =Tech.iLDCassetteRearBack;
    Prod.iLDCassetteZStart              =Tech.iLDCassetteZStart;

    for(int i=0; i<3; i++)
    {
        Prod.iAutoCassetteFront[i]       =Tech.iAutoCassetteFront[i];
        Prod.iAutoCassetteFrontBack[i]   =Tech.iAutoCassetteFrontBack[i];
        Prod.iAutoCassetteRear[i]        =Tech.iAutoCassetteRear[i];
        Prod.iAutoCassetteRearBack[i]    =Tech.iAutoCassetteRearBack[i];
        Prod.iAutoCassetteZStart[i]      =Tech.iAutoCassetteZStart[i];
    }

    Prod.iCatchMazTray_Front=Tech.iCatchMazTray_Front;
    Prod.iCatchMazTray_Rear=Tech.iCatchMazTray_Rear;

}
//------------------------------------------------------------------------------
void SetTechDataToProd_Shuttle()
{
    double fTempRate=0.0;
    int iInPos=0, iTemp=0, iShift=0;                                            //Sam 20221205 : 修正 Shuttle 移動完成後又被修改 Offset 導致 Hang up
    if((LastSet.iTemperature==Tempture_Hot ||
        LastSet.iTemperature==Tempture_AmbientHot) &&                           //JerryYang 20230204 : 修正non hot plate mode沒有吃L09 shuttle shift offset
        Temperature.fWorkTemperBase>50.0 &&
        IniConfig.bL09HotTempShuttleNoAddPos==false)                            //kevin 20211116 no use shuttle offset
    {
        if(iTempLimitation==tTemp175)                                           //Isaac 20200916 : 修正開150度，Autoheight shuttle會扭力跳脫
        {
            fTempRate=(Temperature.fWorkTemperBase-50.0)/(175.0-50.0);
        }
        else if(iTempLimitation==tTemp155)                                      //Sam 20240118 新增 155度 模式
        {
            fTempRate=(Temperature.fWorkTemperBase-50.0)/(155.0-50.0);
        }
        else if(iTempLimitation==tTemp150)                                      //Isaac 20200916 : 修正開150度，Autoheight shuttle會扭力跳脫
        {
            fTempRate=(Temperature.fWorkTemperBase-50.0)/(150.0-50.0);
        }
        else if(iTempLimitation==tTemp200)
        {
            fTempRate=(Temperature.fWorkTemperBase-50.0)/(200.0-50.0);
        }
        else
        {
            fTempRate=(Temperature.fWorkTemperBase-50.0)/(130.0-50.0);
        }

        for(int i=0; i<2; i++)
            for(int j=0; j<2; j++)
                iShuttleTempPos[i][j]=LastSet.TempPosShift_Shuttle[i][j]*fTempRate;
    }
    else                                                                        //Steven 20240514 : 沒用到要歸零
    {
        if(Tri_Temp_Machine==1)                                                 //Ztex 2024.02.21 Add Change Temp When Shuttle Auto Shift Pos
        {
            if(Temperature.fWorkTemperBase>=165)
                iTemp=165;
            else if(Temperature.fWorkTemperBase<=-45)
                iTemp=-45;
            else
                iTemp=Temperature.fWorkTemperBase;

            if(Temperature.fWorkTemperBase>=25.0)
            {
                fTempRate=(iTemp-25.0)/(165.0-25.0);
                iShift=IniConfig.iL09_1HightTemp_Sht_Shift;
            }
            else
            {
                fTempRate=(fabs(iTemp)+25.0)/(25.0+45);
                iShift=IniConfig.iL09_2LowTemp_Sht_Shift;
            }
            for(int i=0; i<2; i++)
                for(int j=0; j<2; j++)
                    iShuttleTempPos[i][j]=iShift*fTempRate;
        }
        else
        {
           for(int i=0; i<2; i++)
               for(int j=0; j<2; j++)
                   iShuttleTempPos[i][j]=0;
        }
    }

    //-------------------------------------------------------------------------------------------------
    //shuttle Left Right
    //-------------------------------------------------------------------------------------------------
    Prod.InSHT[0].iLeft     = Tech.iInShuttle1Left +Offset.iSHLeftPod[0] +iShuttleTempPos[0][0];
    Prod.InSHT[0].iRight    = Tech.iInShuttle1Right+Offset.iSHRightPod[0]+iShuttleTempPos[0][1];
    Prod.InSHT[1].iLeft     = Tech.iInShuttle2Left +Offset.iSHLeftPod[1] +iShuttleTempPos[1][0];
    Prod.InSHT[1].iRight    = Tech.iInShuttle2Right+Offset.iSHRightPod[1]+iShuttleTempPos[1][1];

    if(b1ShuttleMoveToRight)                                                    //Sam 20221205 : 修正 Shuttle 移動完成後又被修改 Offset 導致 Hang up
    {
        iInPos=MOT[MInShuttle1].ReadPos();
        if(Prod.InSHT[0].iRight!=iInPos)
            bSHTOfsChangeRight[0]=true;
        else
            bSHTOfsChangeRight[0]=false;
    }
    else
    {
        bSHTOfsChangeRight[0]=false;
    }
    if(b2ShuttleMoveToRight)
    {
        iInPos=MOT[MInShuttle2].ReadPos();
        if(Prod.InSHT[1].iRight!=iInPos)
            bSHTOfsChangeRight[1]=true;
        else
            bSHTOfsChangeRight[1]=false;
    }
    else
    {
        bSHTOfsChangeRight[1]=false;
    }

    if(b1ShuttleMoveToLeft)                                                     //Sam 20230202 : 修正 Shuttle Left 移動完成後又被修改 Offset 導致 Hang up
    {
        iInPos=MOT[MInShuttle1].ReadPos();
        if(Prod.InSHT[0].iLeft!=iInPos)
            bSHTOfsChangeLeft[0]=true;
        else
            bSHTOfsChangeLeft[0]=false;
    }
    else
    {
        bSHTOfsChangeLeft[0]=false;
    }
    if(b2ShuttleMoveToLeft)
    {
        iInPos=MOT[MInShuttle2].ReadPos();
        if(Prod.InSHT[1].iLeft!=iInPos)
            bSHTOfsChangeLeft[1]=true;
        else
            bSHTOfsChangeLeft[1]=false;
    }
    else
    {
        bSHTOfsChangeLeft[1]=false;
    }

    Prod.OutSHT[0].iRight   = Tech.iOutShuttle1Right;
    Prod.OutSHT[1].iRight   = Tech.iOutShuttle2Right;
    Prod.OutSHT[0].iLeft    = Tech.iOutShuttle1Left;
    Prod.OutSHT[1].iLeft    = Tech.iOutShuttle2Left;

    if(Tech.iInSH1Sen7DetectPos==0 || Tech.iInSH2Sen7DetectPos==0)
    {
        Prod.iInSHSen7DetectPos1x1[0]=Prod.InSHT[0].iLeft;
        Prod.iInSHSen7DetectPos1x1[1]=Prod.InSHT[1].iLeft;

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<2; j++)
                Prod.iInSHSen7DetectPos1x2[i][j]=Prod.InSHT[i].iLeft;

            for(int j=0; j<3; j++)
                Prod.iInSHSen7DetectPos2x3[i][j]=Prod.InSHT[i].iLeft;

            for(int j=0; j<4; j++)
                Prod.iInSHSen7DetectPos1x4[i][j]=Prod.InSHT[i].iLeft;

            for(int j=0; j<5; j++)  //Steven 20221027 : Add for 2x5
                Prod.iInSHSen9DetectPos2x5[i][j]=Prod.InSHT[i].iLeft;

            for(int j=0; j<6; j++)
                Prod.iInSHSen9DetectPos2x6[i][j]=Prod.InSHT[i].iLeft;

            for(int j=0; j<8; j++)
                Prod.iInSHSen9DetectPos2x8[i][j]=Prod.InSHT[i].iLeft;
        }
    }
    else
    {
        //jou 981203 start : in shuttle sensor 7
        Prod.iInSHSen7DetectPos1x1[0]=Tech.iInSH1Sen7DetectPos+2000; //Shuttle 1
        Prod.iInSHSen7DetectPos1x1[1]=Tech.iInSH2Sen7DetectPos+2000; //Shuttle 2
        //shuttle 1
        Prod.iInSHSen7DetectPos1x2[0][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch/2; //site 1
        Prod.iInSHSen7DetectPos1x2[0][1]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch/2; //site 2
        //shuttle 2
        Prod.iInSHSen7DetectPos1x2[1][0]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch/2;
        Prod.iInSHSen7DetectPos1x2[1][1]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch/2;

        //Shuttle1  //ChungHung 20140115 add for 2x3_6
        Prod.iInSHSen7DetectPos2x3[0][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch;
        Prod.iInSHSen7DetectPos2x3[0][1]=Tech.iInSH1Sen7DetectPos+2000;
        Prod.iInSHSen7DetectPos2x3[0][2]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch;

        //Shuttle2  //ChungHung 20140115 add for 2x3_6
        Prod.iInSHSen7DetectPos2x3[1][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch;
        Prod.iInSHSen7DetectPos2x3[1][1]=Tech.iInSH1Sen7DetectPos+2000;
        Prod.iInSHSen7DetectPos2x3[1][2]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch;

        //shuttle 1
        Prod.iInSHSen7DetectPos1x4[0][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*3/2;
        Prod.iInSHSen7DetectPos1x4[0][1]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch/2;
        Prod.iInSHSen7DetectPos1x4[0][2]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch/2;
        Prod.iInSHSen7DetectPos1x4[0][3]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*3/2;
        //shuttle 2
        Prod.iInSHSen7DetectPos1x4[1][0]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*3/2;
        Prod.iInSHSen7DetectPos1x4[1][1]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch/2;
        Prod.iInSHSen7DetectPos1x4[1][2]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch/2;
        Prod.iInSHSen7DetectPos1x4[1][3]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*3/2;

        //Richard 20220929 fix Sen7 Position for 2X4CenterPitch
        //==>
        if( CosFunction.b2x4SupportCenterPitch && TestIF_File.bEnableUseXCenterPitch==true &&TestIF_File.iTestMode==_8Site2X4 )
        {
            //shuttle 1
            Prod.iInSHSen7DetectPos1x4[0][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF_File.dSiteXCenterPitch*100/2+TestIF.dSiteXPitch;
            Prod.iInSHSen7DetectPos1x4[0][1]=Tech.iInSH1Sen7DetectPos+2000+TestIF_File.dSiteXCenterPitch*100/2;
            Prod.iInSHSen7DetectPos1x4[0][2]=Tech.iInSH1Sen7DetectPos+2000-TestIF_File.dSiteXCenterPitch*100/2;
            Prod.iInSHSen7DetectPos1x4[0][3]=Tech.iInSH1Sen7DetectPos+2000-TestIF_File.dSiteXCenterPitch*100/2-TestIF.dSiteXPitch;
            //shuttle 2
            Prod.iInSHSen7DetectPos1x4[1][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF_File.dSiteXCenterPitch*100/2+TestIF.dSiteXPitch;
            Prod.iInSHSen7DetectPos1x4[1][1]=Tech.iInSH1Sen7DetectPos+2000+TestIF_File.dSiteXCenterPitch*100/2;
            Prod.iInSHSen7DetectPos1x4[1][2]=Tech.iInSH1Sen7DetectPos+2000-TestIF_File.dSiteXCenterPitch*100/2;
            Prod.iInSHSen7DetectPos1x4[1][3]=Tech.iInSH1Sen7DetectPos+2000-TestIF_File.dSiteXCenterPitch*100/2-TestIF.dSiteXPitch;
        }
        //<==
        //Richard 20220929 fix Sen7 Position for 2X4CenterPitch

        //Shuttle 1     //Steven 20221027 : Add for 2x5
        Prod.iInSHSen9DetectPos2x5[0][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*2;
        Prod.iInSHSen9DetectPos2x5[0][1]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*1;
        Prod.iInSHSen9DetectPos2x5[0][2]=Tech.iInSH1Sen7DetectPos+2000;
        Prod.iInSHSen9DetectPos2x5[0][3]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*1;
        Prod.iInSHSen9DetectPos2x5[0][4]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*2;
        //Shuttle 2
        Prod.iInSHSen9DetectPos2x5[1][0]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*2;
        Prod.iInSHSen9DetectPos2x5[1][1]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*1;
        Prod.iInSHSen9DetectPos2x5[1][2]=Tech.iInSH2Sen7DetectPos+2000;
        Prod.iInSHSen9DetectPos2x5[1][3]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*1;
        Prod.iInSHSen9DetectPos2x5[1][4]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*2;

        //Shuttle 1   //ChungHung 20130507 add HT9045 updata for 12site 517
        Prod.iInSHSen9DetectPos2x6[0][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*2.5;
        Prod.iInSHSen9DetectPos2x6[0][1]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x6[0][2]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x6[0][3]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x6[0][4]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x6[0][5]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*2.5;
        //Shuttle 2  //ChungHung 20130507 add HT9045 updata for 12site 517
        Prod.iInSHSen9DetectPos2x6[1][0]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*2.5;
        Prod.iInSHSen9DetectPos2x6[1][1]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x6[1][2]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x6[1][3]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x6[1][4]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x6[1][5]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*2.5;

        //shuttle 1
        Prod.iInSHSen9DetectPos2x8[0][0]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*3.5;
        Prod.iInSHSen9DetectPos2x8[0][1]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*2.5;
        Prod.iInSHSen9DetectPos2x8[0][2]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x8[0][3]=Tech.iInSH1Sen7DetectPos+2000+TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x8[0][4]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x8[0][5]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x8[0][6]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*2.5;
        Prod.iInSHSen9DetectPos2x8[0][7]=Tech.iInSH1Sen7DetectPos+2000-TestIF.dSiteXPitch*3.5;
        //shuttle 2
        Prod.iInSHSen9DetectPos2x8[1][0]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*3.5;
        Prod.iInSHSen9DetectPos2x8[1][1]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*2.5;
        Prod.iInSHSen9DetectPos2x8[1][2]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x8[1][3]=Tech.iInSH2Sen7DetectPos+2000+TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x8[1][4]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch/2;
        Prod.iInSHSen9DetectPos2x8[1][5]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*1.5;
        Prod.iInSHSen9DetectPos2x8[1][6]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*2.5;
        Prod.iInSHSen9DetectPos2x8[1][7]=Tech.iInSH2Sen7DetectPos+2000-TestIF.dSiteXPitch*3.5;
        //jou 981203 end
    }

    if(Tech.iInSH1BarCodePos==0)
    {
        Tech.iInSH1BarCodePos=Tech.iInSH1Sen7DetectPos;
    }

    if(Tech.iInSH2BarCodePos==0)
    {
        Tech.iInSH2BarCodePos=Tech.iInSH2Sen7DetectPos;
    }

    //----------------------------------
    //Steven 20160920 : IC置偏檢查
    //拍一張的
    //----------------------------------
    Prod.iInSFCPos1x1[0]=Tech.iInSH1BarCodePos+2000; //Shuttle 2
    Prod.iInSFCPos1x1[1]=Tech.iInSH2BarCodePos+2000; //Shuttle 1

    //shuttle 1
    Prod.iInSFCPos1x2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2; //site 1
    Prod.iInSFCPos1x2[0][1]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2; //site 2
    //shuttle 2
    Prod.iInSFCPos1x2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x2[1][1]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;

    //shuttle 1  //ChungHung 20140115 add for 2x3_6
    Prod.iInSFCPos1x3[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSFCPos1x3[0][1]=Tech.iInSH1BarCodePos+2000;
    Prod.iInSFCPos1x3[0][2]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch;
    //shuttle 2
    Prod.iInSFCPos1x3[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSFCPos1x3[1][1]=Tech.iInSH2BarCodePos+2000;
    Prod.iInSFCPos1x3[1][2]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch;

    //shuttle 1
    Prod.iInSFCPos1x4[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3/2;
    Prod.iInSFCPos1x4[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x4[0][2]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x4[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3/2;
    //shuttle 2
    Prod.iInSFCPos1x4[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3/2;
    Prod.iInSFCPos1x4[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x4[1][2]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x4[1][3]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3/2;

    //shuttle 1  //Steven 20221027 : Add for 2x5
    Prod.iInSFCPos1x5[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2;
    Prod.iInSFCPos1x5[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSFCPos1x5[0][2]=Tech.iInSH1BarCodePos+2000;
    Prod.iInSFCPos1x5[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch;
    Prod.iInSFCPos1x5[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2;
    //shuttle 2
    Prod.iInSFCPos1x5[0][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2;
    Prod.iInSFCPos1x5[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSFCPos1x5[1][2]=Tech.iInSH2BarCodePos+2000;
    Prod.iInSFCPos1x5[1][3]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch;
    Prod.iInSFCPos1x5[0][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2;

    //Shuttle 1
    Prod.iInSFCPos1x6[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSFCPos1x6[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x6[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x6[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x6[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x6[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    //Shuttle 2
    Prod.iInSFCPos1x6[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSFCPos1x6[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x6[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x6[1][3]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x6[1][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x6[1][5]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5;

    //shuttle 1
    Prod.iInSFCPos1x8[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3.5;
    Prod.iInSFCPos1x8[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSFCPos1x8[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x8[0][3]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x8[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x8[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x8[0][6]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    Prod.iInSFCPos1x8[0][7]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3.5;
    //shuttle 2
    Prod.iInSFCPos1x8[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3.5;
    Prod.iInSFCPos1x8[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSFCPos1x8[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x8[1][3]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x8[1][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSFCPos1x8[1][5]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSFCPos1x8[1][6]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    Prod.iInSFCPos1x8[1][7]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3.5;

    //----------------------------------
    //Steven 20160920 : IC置偏檢查
    //拍兩張的
    //----------------------------------
    int iOffset=TestIF_File.iSFCUse2PhotoOffset*100;
    Prod.iInSFCPos1x1_2[0][0]=Tech.iInSH1BarCodePos+2000+iOffset; //Shuttle 1
    Prod.iInSFCPos1x1_2[0][1]=Tech.iInSH2BarCodePos+2000-iOffset; //Shuttle 1
    Prod.iInSFCPos1x1_2[1][0]=Tech.iInSH1BarCodePos+2000+iOffset; //Shuttle 2
    Prod.iInSFCPos1x1_2[1][1]=Tech.iInSH2BarCodePos+2000-iOffset; //Shuttle 2

    //shuttle 1
    Prod.iInSFCPos1x2_2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset; //site 1
    Prod.iInSFCPos1x2_2[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset; //site 1
    Prod.iInSFCPos1x2_2[0][2]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset; //site 2
    Prod.iInSFCPos1x2_2[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset; //site 2
    //shuttle 2
    Prod.iInSFCPos1x2_2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x2_2[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x2_2[1][2]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x2_2[1][3]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset;

    //shuttle 1  //ChungHung 20140115 add for 2x3_6
    Prod.iInSFCPos1x3_2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x3_2[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch-iOffset;
    Prod.iInSFCPos1x3_2[0][2]=Tech.iInSH1BarCodePos+2000+iOffset;
    Prod.iInSFCPos1x3_2[0][3]=Tech.iInSH1BarCodePos+2000-iOffset;
    Prod.iInSFCPos1x3_2[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x3_2[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch-iOffset;
    //shuttle 2
    Prod.iInSFCPos1x3_2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x3_2[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch-iOffset;
    Prod.iInSFCPos1x3_2[1][2]=Tech.iInSH2BarCodePos+2000+iOffset;
    Prod.iInSFCPos1x3_2[1][3]=Tech.iInSH2BarCodePos+2000-iOffset;
    Prod.iInSFCPos1x3_2[1][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x3_2[1][5]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch-iOffset;

    //shuttle 1
    Prod.iInSFCPos1x4_2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3/2+iOffset;
    Prod.iInSFCPos1x4_2[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3/2-iOffset;
    Prod.iInSFCPos1x4_2[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x4_2[0][3]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x4_2[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x4_2[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x4_2[0][6]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3/2+iOffset;
    Prod.iInSFCPos1x4_2[0][7]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3/2-iOffset;
    //shuttle 2
    Prod.iInSFCPos1x4_2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3/2+iOffset;
    Prod.iInSFCPos1x4_2[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3/2-iOffset;
    Prod.iInSFCPos1x4_2[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x4_2[1][3]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x4_2[1][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x4_2[1][5]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x4_2[1][6]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3/2+iOffset;
    Prod.iInSFCPos1x4_2[1][7]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3/2-iOffset;

    //shuttle 1  //Steven 20221027 : Add for 2x5
    Prod.iInSFCPos1x5_2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch+iOffset*2;
    Prod.iInSFCPos1x5_2[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch-iOffset*2;
    Prod.iInSFCPos1x5_2[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x5_2[0][3]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch-iOffset;
    Prod.iInSFCPos1x5_2[0][4]=Tech.iInSH1BarCodePos+2000+iOffset;
    Prod.iInSFCPos1x5_2[0][5]=Tech.iInSH1BarCodePos+2000-iOffset;
    Prod.iInSFCPos1x5_2[0][6]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x5_2[0][7]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch-iOffset;
    Prod.iInSFCPos1x5_2[0][8]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch+iOffset*2;
    Prod.iInSFCPos1x5_2[0][9]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch-iOffset*2;
    //shuttle 2
    Prod.iInSFCPos1x5_2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch+iOffset*2;
    Prod.iInSFCPos1x5_2[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch-iOffset*2;
    Prod.iInSFCPos1x5_2[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x5_2[1][3]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch-iOffset;
    Prod.iInSFCPos1x5_2[1][4]=Tech.iInSH2BarCodePos+2000+iOffset;
    Prod.iInSFCPos1x5_2[1][5]=Tech.iInSH2BarCodePos+2000-iOffset;
    Prod.iInSFCPos1x5_2[1][6]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch+iOffset;
    Prod.iInSFCPos1x5_2[1][7]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch-iOffset;
    Prod.iInSFCPos1x5_2[1][8]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch+iOffset*2;
    Prod.iInSFCPos1x5_2[1][9]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch-iOffset*2;

    //Shuttle 1
    Prod.iInSFCPos1x6_2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x6_2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5-iOffset;
    Prod.iInSFCPos1x6_2[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x6_2[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x6_2[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x6_2[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x6_2[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x6_2[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x6_2[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x6_2[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x6_2[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x6_2[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5-iOffset;
    //Shuttle 2
    Prod.iInSFCPos1x6_2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x6_2[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5-iOffset;
    Prod.iInSFCPos1x6_2[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x6_2[1][3]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x6_2[1][4]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x6_2[1][5]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x6_2[1][6]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x6_2[1][7]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x6_2[1][8]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x6_2[1][9]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x6_2[1][10]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x6_2[1][11]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5-iOffset;

    //shuttle 1
    Prod.iInSFCPos1x8_2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3.5+iOffset;
    Prod.iInSFCPos1x8_2[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3.5-iOffset;
    Prod.iInSFCPos1x8_2[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x8_2[0][3]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5-iOffset;
    Prod.iInSFCPos1x8_2[0][4]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x8_2[0][5]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x8_2[0][6]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x8_2[0][7]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x8_2[0][8]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x8_2[0][9]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x8_2[0][10]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x8_2[0][11]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x8_2[0][12]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x8_2[0][13]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5-iOffset;
    Prod.iInSFCPos1x8_2[0][14]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3.5+iOffset;
    Prod.iInSFCPos1x8_2[0][15]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3.5-iOffset;
    //shuttle 2
    Prod.iInSFCPos1x8_2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3.5+iOffset;
    Prod.iInSFCPos1x8_2[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3.5-iOffset;
    Prod.iInSFCPos1x8_2[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x8_2[1][3]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5-iOffset;
    Prod.iInSFCPos1x8_2[1][4]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x8_2[1][5]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x8_2[1][6]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x8_2[1][7]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x8_2[1][8]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2+iOffset;
    Prod.iInSFCPos1x8_2[1][9]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2-iOffset;
    Prod.iInSFCPos1x8_2[1][10]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5+iOffset;
    Prod.iInSFCPos1x8_2[1][11]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5-iOffset;
    Prod.iInSFCPos1x8_2[1][12]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5+iOffset;
    Prod.iInSFCPos1x8_2[1][13]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5-iOffset;
    Prod.iInSFCPos1x8_2[1][14]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3.5+iOffset;
    Prod.iInSFCPos1x8_2[1][15]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3.5-iOffset;

    //----------------------------------
    //Steven 20120921 : BarCode_2D - In Shuttle
    //----------------------------------
    Prod.iInSHBarCodeDetectPos1x1[0]=Tech.iInSH1BarCodePos+2000;                //Shuttle 2
    Prod.iInSHBarCodeDetectPos1x1[1]=Tech.iInSH2BarCodePos+2000;                //Shuttle 1
    //shuttle 1
    Prod.iInSHBarCodeDetectPos1x2[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2; //site 1
    Prod.iInSHBarCodeDetectPos1x2[0][1]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2; //site 2
    //shuttle 2
    Prod.iInSHBarCodeDetectPos1x2[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos1x2[1][1]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;

    //shuttle 1  //ChungHung 20140115 add for 2x3_6
    Prod.iInSHBarCodeDetectPos2x3[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSHBarCodeDetectPos2x3[0][1]=Tech.iInSH1BarCodePos+2000;
    Prod.iInSHBarCodeDetectPos2x3[0][2]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch;
    //shuttle 2
    Prod.iInSHBarCodeDetectPos2x3[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSHBarCodeDetectPos2x3[1][1]=Tech.iInSH2BarCodePos+2000;
    Prod.iInSHBarCodeDetectPos2x3[1][2]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch;

    //shuttle 1
    Prod.iInSHBarCodeDetectPos1x4[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3/2;
    Prod.iInSHBarCodeDetectPos1x4[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos1x4[0][2]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos1x4[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3/2;
    //shuttle 2
    Prod.iInSHBarCodeDetectPos1x4[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3/2;
    Prod.iInSHBarCodeDetectPos1x4[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos1x4[1][2]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos1x4[1][3]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3/2;

    if(CosFunction.b2x4SupportCenterPitch &&                                    //Richard 20220929 fix barcode Position for 2X4CenterPitch
       TestIF_File.bEnableUseXCenterPitch==true &&
       TestIF_File.iTestMode==_8Site2X4)
    {
        //shuttle 1
        Prod.iInSHBarCodeDetectPos1x4[0][0]=Tech.iInSH1BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2+TestIF.dSiteXPitch;
        Prod.iInSHBarCodeDetectPos1x4[0][1]=Tech.iInSH1BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iInSHBarCodeDetectPos1x4[0][2]=Tech.iInSH1BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iInSHBarCodeDetectPos1x4[0][3]=Tech.iInSH1BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2-TestIF.dSiteXPitch;
        //shuttle 2
        Prod.iInSHBarCodeDetectPos1x4[1][0]=Tech.iInSH2BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2+TestIF.dSiteXPitch;
        Prod.iInSHBarCodeDetectPos1x4[1][1]=Tech.iInSH2BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iInSHBarCodeDetectPos1x4[1][2]=Tech.iInSH2BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iInSHBarCodeDetectPos1x4[1][3]=Tech.iInSH2BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2-TestIF.dSiteXPitch;
    }

    //shuttle 1  //Steven 20221027 : Add for 2x5
    Prod.iInSHBarCodeDetectPos2x5[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2;
    Prod.iInSHBarCodeDetectPos2x5[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSHBarCodeDetectPos2x5[0][2]=Tech.iInSH1BarCodePos+2000;
    Prod.iInSHBarCodeDetectPos2x5[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch;
    Prod.iInSHBarCodeDetectPos2x5[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2;
    //shuttle 2
    Prod.iInSHBarCodeDetectPos2x5[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2;
    Prod.iInSHBarCodeDetectPos2x5[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iInSHBarCodeDetectPos2x5[1][2]=Tech.iInSH2BarCodePos+2000;
    Prod.iInSHBarCodeDetectPos2x5[1][3]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch;
    Prod.iInSHBarCodeDetectPos2x5[1][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2;

    //Shuttle 1 //Steven 20170111 : fixed for 2x6
    Prod.iInSHBarCodeDetectPos2x6[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSHBarCodeDetectPos2x6[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x6[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x6[0][3]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x6[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x6[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    //Shuttle 2
    Prod.iInSHBarCodeDetectPos2x6[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSHBarCodeDetectPos2x6[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x6[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x6[1][3]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x6[1][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x6[1][5]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5;

    //shuttle 1
    Prod.iInSHBarCodeDetectPos2x8[0][0]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*3.5;
    Prod.iInSHBarCodeDetectPos2x8[0][1]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSHBarCodeDetectPos2x8[0][2]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x8[0][3]=Tech.iInSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x8[0][4]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x8[0][5]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x8[0][6]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    Prod.iInSHBarCodeDetectPos2x8[0][7]=Tech.iInSH1BarCodePos+2000-TestIF.dSiteXPitch*3.5;
    //shuttle 2
    Prod.iInSHBarCodeDetectPos2x8[1][0]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*3.5;
    Prod.iInSHBarCodeDetectPos2x8[1][1]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iInSHBarCodeDetectPos2x8[1][2]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x8[1][3]=Tech.iInSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x8[1][4]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iInSHBarCodeDetectPos2x8[1][5]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iInSHBarCodeDetectPos2x8[1][6]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    Prod.iInSHBarCodeDetectPos2x8[1][7]=Tech.iInSH2BarCodePos+2000-TestIF.dSiteXPitch*3.5;

    int dMilti2DPitch=TestIF.dMulti2DXPitch/2;
    Prod.iInSHBarMulti2DDetectPos1x1[0][0]=Prod.iInSHBarCodeDetectPos1x1[0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x1[0][1]=Prod.iInSHBarCodeDetectPos1x1[0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x1[1][0]=Prod.iInSHBarCodeDetectPos1x1[1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x1[1][1]=Prod.iInSHBarCodeDetectPos1x1[1]+dMilti2DPitch;

    Prod.iInSHBarMulti2DDetectPos1x2[0][0][0]=Prod.iInSHBarCodeDetectPos1x2[0][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x2[0][0][1]=Prod.iInSHBarCodeDetectPos1x2[0][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x2[0][1][0]=Prod.iInSHBarCodeDetectPos1x2[0][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x2[0][1][1]=Prod.iInSHBarCodeDetectPos1x2[0][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x2[1][0][0]=Prod.iInSHBarCodeDetectPos1x2[1][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x2[1][0][1]=Prod.iInSHBarCodeDetectPos1x2[1][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x2[1][1][0]=Prod.iInSHBarCodeDetectPos1x2[1][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x2[1][1][1]=Prod.iInSHBarCodeDetectPos1x2[1][1]+dMilti2DPitch;

    Prod.iInSHBarMulti2DDetectPos2x3[0][0][0]=Prod.iInSHBarCodeDetectPos2x3[0][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[0][0][1]=Prod.iInSHBarCodeDetectPos2x3[0][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[0][1][0]=Prod.iInSHBarCodeDetectPos2x3[0][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[0][1][1]=Prod.iInSHBarCodeDetectPos2x3[0][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[0][2][0]=Prod.iInSHBarCodeDetectPos2x3[0][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[0][2][1]=Prod.iInSHBarCodeDetectPos2x3[0][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[1][0][0]=Prod.iInSHBarCodeDetectPos2x3[1][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[1][0][1]=Prod.iInSHBarCodeDetectPos2x3[1][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[1][1][0]=Prod.iInSHBarCodeDetectPos2x3[1][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[1][1][1]=Prod.iInSHBarCodeDetectPos2x3[1][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[1][2][1]=Prod.iInSHBarCodeDetectPos2x3[1][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x3[1][2][0]=Prod.iInSHBarCodeDetectPos2x3[1][2]-dMilti2DPitch;

    Prod.iInSHBarMulti2DDetectPos1x4[0][0][0]=Prod.iInSHBarCodeDetectPos1x4[0][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[0][0][1]=Prod.iInSHBarCodeDetectPos1x4[0][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[0][1][0]=Prod.iInSHBarCodeDetectPos1x4[0][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[0][1][1]=Prod.iInSHBarCodeDetectPos1x4[0][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[0][2][0]=Prod.iInSHBarCodeDetectPos1x4[0][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[0][2][1]=Prod.iInSHBarCodeDetectPos1x4[0][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[0][3][0]=Prod.iInSHBarCodeDetectPos1x4[0][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[0][3][1]=Prod.iInSHBarCodeDetectPos1x4[0][3]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][0][0]=Prod.iInSHBarCodeDetectPos1x4[1][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][0][1]=Prod.iInSHBarCodeDetectPos1x4[1][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][1][0]=Prod.iInSHBarCodeDetectPos1x4[1][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][1][1]=Prod.iInSHBarCodeDetectPos1x4[1][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][2][1]=Prod.iInSHBarCodeDetectPos1x4[1][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][2][0]=Prod.iInSHBarCodeDetectPos1x4[1][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][3][0]=Prod.iInSHBarCodeDetectPos1x4[1][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos1x4[1][3][1]=Prod.iInSHBarCodeDetectPos1x4[1][3]+dMilti2DPitch;

    Prod.iInSHBarMulti2DDetectPos2x5[0][0][0]=Prod.iInSHBarCodeDetectPos2x5[0][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][0][1]=Prod.iInSHBarCodeDetectPos2x5[0][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][1][0]=Prod.iInSHBarCodeDetectPos2x5[0][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][1][1]=Prod.iInSHBarCodeDetectPos2x5[0][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][2][0]=Prod.iInSHBarCodeDetectPos2x5[0][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][2][1]=Prod.iInSHBarCodeDetectPos2x5[0][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][3][0]=Prod.iInSHBarCodeDetectPos2x5[0][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][3][1]=Prod.iInSHBarCodeDetectPos2x5[0][3]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][4][0]=Prod.iInSHBarCodeDetectPos2x5[0][4]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[0][4][1]=Prod.iInSHBarCodeDetectPos2x5[0][4]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][0][0]=Prod.iInSHBarCodeDetectPos2x5[1][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][0][1]=Prod.iInSHBarCodeDetectPos2x5[1][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][1][0]=Prod.iInSHBarCodeDetectPos2x5[1][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][1][1]=Prod.iInSHBarCodeDetectPos2x5[1][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][2][1]=Prod.iInSHBarCodeDetectPos2x5[1][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][2][0]=Prod.iInSHBarCodeDetectPos2x5[1][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][3][0]=Prod.iInSHBarCodeDetectPos2x5[1][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][3][1]=Prod.iInSHBarCodeDetectPos2x5[1][3]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][4][0]=Prod.iInSHBarCodeDetectPos2x5[1][4]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x5[1][4][1]=Prod.iInSHBarCodeDetectPos2x5[1][4]+dMilti2DPitch;

    Prod.iInSHBarMulti2DDetectPos2x6[0][0][0]=Prod.iInSHBarCodeDetectPos2x6[0][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][0][1]=Prod.iInSHBarCodeDetectPos2x6[0][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][1][0]=Prod.iInSHBarCodeDetectPos2x6[0][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][1][1]=Prod.iInSHBarCodeDetectPos2x6[0][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][2][0]=Prod.iInSHBarCodeDetectPos2x6[0][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][2][1]=Prod.iInSHBarCodeDetectPos2x6[0][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][3][0]=Prod.iInSHBarCodeDetectPos2x6[0][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][3][1]=Prod.iInSHBarCodeDetectPos2x6[0][3]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][4][0]=Prod.iInSHBarCodeDetectPos2x6[0][4]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][4][1]=Prod.iInSHBarCodeDetectPos2x6[0][4]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][5][0]=Prod.iInSHBarCodeDetectPos2x6[0][5]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[0][5][1]=Prod.iInSHBarCodeDetectPos2x6[0][5]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][0][0]=Prod.iInSHBarCodeDetectPos2x6[1][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][0][1]=Prod.iInSHBarCodeDetectPos2x6[1][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][1][0]=Prod.iInSHBarCodeDetectPos2x6[1][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][1][1]=Prod.iInSHBarCodeDetectPos2x6[1][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][2][1]=Prod.iInSHBarCodeDetectPos2x6[1][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][2][0]=Prod.iInSHBarCodeDetectPos2x6[1][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][3][0]=Prod.iInSHBarCodeDetectPos2x6[1][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][3][1]=Prod.iInSHBarCodeDetectPos2x6[1][3]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][4][0]=Prod.iInSHBarCodeDetectPos2x6[1][4]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][4][1]=Prod.iInSHBarCodeDetectPos2x6[1][4]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][5][0]=Prod.iInSHBarCodeDetectPos2x6[1][5]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x6[1][5][1]=Prod.iInSHBarCodeDetectPos2x6[1][5]+dMilti2DPitch;

    Prod.iInSHBarMulti2DDetectPos2x8[0][0][0]=Prod.iInSHBarCodeDetectPos2x8[0][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][0][1]=Prod.iInSHBarCodeDetectPos2x8[0][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][1][0]=Prod.iInSHBarCodeDetectPos2x8[0][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][1][1]=Prod.iInSHBarCodeDetectPos2x8[0][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][2][0]=Prod.iInSHBarCodeDetectPos2x8[0][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][2][1]=Prod.iInSHBarCodeDetectPos2x8[0][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][3][0]=Prod.iInSHBarCodeDetectPos2x8[0][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][3][1]=Prod.iInSHBarCodeDetectPos2x8[0][3]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][4][0]=Prod.iInSHBarCodeDetectPos2x8[0][4]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][4][1]=Prod.iInSHBarCodeDetectPos2x8[0][4]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][5][0]=Prod.iInSHBarCodeDetectPos2x8[0][5]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][5][1]=Prod.iInSHBarCodeDetectPos2x8[0][5]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][6][0]=Prod.iInSHBarCodeDetectPos2x8[0][6]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][6][1]=Prod.iInSHBarCodeDetectPos2x8[0][6]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][7][0]=Prod.iInSHBarCodeDetectPos2x8[0][7]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[0][7][1]=Prod.iInSHBarCodeDetectPos2x8[0][7]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][0][0]=Prod.iInSHBarCodeDetectPos2x8[1][0]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][0][1]=Prod.iInSHBarCodeDetectPos2x8[1][0]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][1][0]=Prod.iInSHBarCodeDetectPos2x8[1][1]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][1][1]=Prod.iInSHBarCodeDetectPos2x8[1][1]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][2][1]=Prod.iInSHBarCodeDetectPos2x8[1][2]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][2][0]=Prod.iInSHBarCodeDetectPos2x8[1][2]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][3][0]=Prod.iInSHBarCodeDetectPos2x8[1][3]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][3][1]=Prod.iInSHBarCodeDetectPos2x8[1][3]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][4][0]=Prod.iInSHBarCodeDetectPos2x8[1][4]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][4][1]=Prod.iInSHBarCodeDetectPos2x8[1][4]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][5][0]=Prod.iInSHBarCodeDetectPos2x8[1][5]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][5][1]=Prod.iInSHBarCodeDetectPos2x8[1][5]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][6][0]=Prod.iInSHBarCodeDetectPos2x8[1][6]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][6][1]=Prod.iInSHBarCodeDetectPos2x8[1][6]+dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][7][0]=Prod.iInSHBarCodeDetectPos2x8[1][7]-dMilti2DPitch;
    Prod.iInSHBarMulti2DDetectPos2x8[1][7][1]=Prod.iInSHBarCodeDetectPos2x8[1][7]+dMilti2DPitch;

    //----------------------------------
    //Steven 20120921 : BarCode_2D - Out Shuttle
    //----------------------------------
    if(Tech.iOutSH1BarCodePos==0)
    {
        Tech.iOutSH1BarCodePos=Tech.OutSH1ZOneRowDetectPos;
    }

    if(Tech.iOutSH2BarCodePos==0)
    {
        Tech.iOutSH2BarCodePos=Tech.OutSH2ZOneRowDetectPos;
    }

    Prod.iOutSHBarCodeDetectPos1x1[0]=Tech.iOutSH1BarCodePos+2000; //Shuttle 2
    Prod.iOutSHBarCodeDetectPos1x1[1]=Tech.iOutSH2BarCodePos+2000; //Shuttle 1
    //shuttle 1
    Prod.iOutSHBarCodeDetectPos1x2[0][0]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch/2; //site 1
    Prod.iOutSHBarCodeDetectPos1x2[0][1]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch/2; //site 2
    //shuttle 2
    Prod.iOutSHBarCodeDetectPos1x2[1][0]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos1x2[1][1]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch/2;

    //shuttle 1  //ChungHung 20140115 add for 2x3_6
    Prod.iOutSHBarCodeDetectPos2x3[0][0]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iOutSHBarCodeDetectPos2x3[0][1]=Tech.iOutSH1BarCodePos+2000;
    Prod.iOutSHBarCodeDetectPos2x3[0][2]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch;
    //shuttle 2
    Prod.iOutSHBarCodeDetectPos2x3[1][0]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iOutSHBarCodeDetectPos2x3[1][1]=Tech.iOutSH2BarCodePos+2000;
    Prod.iOutSHBarCodeDetectPos2x3[1][2]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch;

    //shuttle 1
    Prod.iOutSHBarCodeDetectPos1x4[0][0]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch*3/2;
    Prod.iOutSHBarCodeDetectPos1x4[0][1]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos1x4[0][2]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos1x4[0][3]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch*3/2;
    //shuttle 2
    Prod.iOutSHBarCodeDetectPos1x4[1][0]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch*3/2;
    Prod.iOutSHBarCodeDetectPos1x4[1][1]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos1x4[1][2]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos1x4[1][3]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch*3/2;

    if(CosFunction.b2x4SupportCenterPitch &&                                    //Richard 20220929 fix barcode Position for 2X4CenterPitch
       TestIF_File.bEnableUseXCenterPitch==true &&
       TestIF_File.iTestMode==_8Site2X4)
    {
        //shuttle 1
        Prod.iOutSHBarCodeDetectPos1x4[0][0]=Tech.iOutSH1BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2+TestIF.dSiteXPitch;
        Prod.iOutSHBarCodeDetectPos1x4[0][1]=Tech.iOutSH1BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iOutSHBarCodeDetectPos1x4[0][2]=Tech.iOutSH1BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iOutSHBarCodeDetectPos1x4[0][3]=Tech.iOutSH1BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2-TestIF.dSiteXPitch;
        //shuttle 2
        Prod.iOutSHBarCodeDetectPos1x4[1][0]=Tech.iOutSH2BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2+TestIF.dSiteXPitch;
        Prod.iOutSHBarCodeDetectPos1x4[1][1]=Tech.iOutSH2BarCodePos+2000+TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iOutSHBarCodeDetectPos1x4[1][2]=Tech.iOutSH2BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2;
        Prod.iOutSHBarCodeDetectPos1x4[1][3]=Tech.iOutSH2BarCodePos+2000-TestIF_File.dSiteXCenterPitch*100/2-TestIF.dSiteXPitch;
    }

    //shuttle 1  //Steven 20221027 : Add for 2x5
    Prod.iOutSHBarCodeDetectPos2x5[0][0]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch*2;
    Prod.iOutSHBarCodeDetectPos2x5[0][1]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iOutSHBarCodeDetectPos2x5[0][2]=Tech.iOutSH1BarCodePos+2000;
    Prod.iOutSHBarCodeDetectPos2x5[0][3]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch;
    Prod.iOutSHBarCodeDetectPos2x5[0][4]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch*2;
    //shuttle 2
    Prod.iOutSHBarCodeDetectPos2x5[1][0]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch*2;
    Prod.iOutSHBarCodeDetectPos2x5[1][1]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch;
    Prod.iOutSHBarCodeDetectPos2x5[1][2]=Tech.iOutSH2BarCodePos+2000;
    Prod.iOutSHBarCodeDetectPos2x5[1][3]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch;
    Prod.iOutSHBarCodeDetectPos2x5[1][4]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch*2;

    //Shuttle 1
    Prod.iOutSHBarCodeDetectPos2x6[0][0]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iOutSHBarCodeDetectPos2x6[0][1]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iOutSHBarCodeDetectPos2x6[0][2]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x6[0][3]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x6[0][4]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x6[0][5]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    //Shuttle 2
    Prod.iOutSHBarCodeDetectPos2x6[1][0]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iOutSHBarCodeDetectPos2x6[1][1]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iOutSHBarCodeDetectPos2x6[1][2]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x6[1][3]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x6[1][4]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x6[1][5]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch/2;

    //shuttle 1
    Prod.iOutSHBarCodeDetectPos2x8[0][0]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch*3.5;
    Prod.iOutSHBarCodeDetectPos2x8[0][1]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iOutSHBarCodeDetectPos2x8[0][2]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iOutSHBarCodeDetectPos2x8[0][3]=Tech.iOutSH1BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x8[0][4]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x8[0][5]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iOutSHBarCodeDetectPos2x8[0][6]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    Prod.iOutSHBarCodeDetectPos2x8[0][7]=Tech.iOutSH1BarCodePos+2000-TestIF.dSiteXPitch*3.5;
    //shuttle 2
    Prod.iOutSHBarCodeDetectPos2x8[1][0]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch*3.5;
    Prod.iOutSHBarCodeDetectPos2x8[1][1]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch*2.5;
    Prod.iOutSHBarCodeDetectPos2x8[1][2]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch*1.5;
    Prod.iOutSHBarCodeDetectPos2x8[1][3]=Tech.iOutSH2BarCodePos+2000+TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x8[1][4]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch/2;
    Prod.iOutSHBarCodeDetectPos2x8[1][5]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch*1.5;
    Prod.iOutSHBarCodeDetectPos2x8[1][6]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch*2.5;
    Prod.iOutSHBarCodeDetectPos2x8[1][7]=Tech.iOutSH2BarCodePos+2000-TestIF.dSiteXPitch*3.5;

    //------------------------------------------------------------------------------
    //Busy Shuttle
    //------------------------------------------------------------------------------
    Prod.iBS1Right_Half=Tech.iInShuttle1Right-8000+Tech.iShuttle1BusyHalfOffset+Offset.iSHHalft[0];
    Prod.iBS2Right_Half=Tech.iInShuttle2Right-8000+Tech.iShuttle2BusyHalfOffset+Offset.iSHHalft[0];

    //------------------------------------------------------------------------------
    //wei 20160914 Auto Shuttle Sensor
    //------------------------------------------------------------------------------
    Prod.iShuttle1120Pitch    =Tech.iShuttle1120Pitch;
    Prod.iShuttle1180Pitch    =Tech.iShuttle1180Pitch;        //wei 20160914 Auto Shuttle Sensor
    Prod.iShuttle2120Pitch    =Tech.iShuttle2120Pitch;        //wei 20160914 Auto Shuttle Sensor
    Prod.iShuttle2180Pitch    =Tech.iShuttle2180Pitch;        //wei 20160914 Auto Shuttle Sensor
}
//------------------------------------------------------------------------------
void SetTechDataToProd_AutoClean()
{
    int iACParamSel=1;                                                          //Sam 20230620 : 優化 Smart Auto Clean
    if(IniConfig.bEnableAutoCleanFunction && iRunACSmart>0)                     //Sam 20230111 : Smart Auto Clean
    {
        if(TestIF_File.bACSmart &&
           TestIF_File.iACSmart_Count!=0 &&
           iACUseParam==2)
        {
            iACParamSel=2;
        }
    }

    if(iACParamSel==2)                                                          //Sam 20230620 : 優化 Smart Auto Clean
    {
        Prod.iAutoClean_ContactMode =TestIF_File.iACSmart_ContactMode;
        Prod.iAutoCleanDropHigh     =TestIF_File.iACSmart_DropHigh;
        Prod.iAutoClean_ContactTime =TestIF_File.iACSmart_ContactTime;
        Prod.iAutoClean_ContactCount=TestIF_File.iACSmart_ContactCount;
    }
    else
    {
        Prod.iAutoClean_ContactMode =TestIF_File.iAutoClean_ContactMode;
        Prod.iAutoCleanDropHigh     =TestIF_File.iAutoCleanDropHigh;
        Prod.iAutoClean_ContactTime =TestIF_File.iAutoClean_ContactTime;
        Prod.iAutoClean_ContactCount=TestIF_File.iAutoClean_ContactCount;
    }

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220330 SPIL佶融要求把contact offset與auto clean offset切開
    {
        Prod.iAutoCleanZ_Contact[0] =Prod.TestZ1_Test-Offset.iIndexArmContact[0]-Prod.TestZ1_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness;     //JerryYang 20181119 (Steven) : (Steven) : fix auto clean 高度異常    //Steven 20181108 : 修正Auto Clean位置異常
        Prod.iAutoCleanZ_Drop[0]    =Prod.TestZ1_Test-Offset.iIndexArmContact[0]-Prod.TestZ1_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoCleanDropHigh;
        Prod.iAutoCleanZ_Shift[0]   =Prod.TestZ1_Test-Offset.iIndexArmContact[0]-Prod.TestZ1_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactShiftHeight;
        Prod.iAutoCleanZ_Contact[1] =Prod.TestZ2_Test-Offset.iIndexArmContact[1]-Prod.TestZ2_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness;
        Prod.iAutoCleanZ_Drop[1]    =Prod.TestZ2_Test-Offset.iIndexArmContact[1]-Prod.TestZ2_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoCleanDropHigh;
        Prod.iAutoCleanZ_Shift[1]   =Prod.TestZ2_Test-Offset.iIndexArmContact[1]-Prod.TestZ2_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactShiftHeight;
    }
    else
    {
        Prod.iAutoCleanZ_Contact[0] =Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness;     //JerryYang 20181119 (Steven) : (Steven) : fix auto clean 高度異常    //Steven 20181108 : 修正Auto Clean位置異常
        Prod.iAutoCleanZ_Drop[0]    =Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+Prod.iAutoCleanDropHigh;    //Sam 20230111 : Smart Auto Clean
        Prod.iAutoCleanZ_Shift[0]   =Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+TestIF.iAutoClean_ContactShiftHeight;
        Prod.iAutoCleanZ_Contact[1] =Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness;
        Prod.iAutoCleanZ_Drop[1]    =Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+Prod.iAutoCleanDropHigh;    //Sam 20230111 : Smart Auto Clean
        Prod.iAutoCleanZ_Shift[1]   =Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+TestIF.iAutoClean_ContactShiftHeight;
    }
}
//------------------------------------------------------------------------------
void SetTechDataToProd_AOI()
{
    //------------------------------------------------------------------------------
    //wei 20160617 Vitrox
    //------------------------------------------------------------------------------
    if(tAOISetup.bEnabledAOI)
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                Prod.OutArm_BGA_Place[i][j]=Prod.iBGAView_Z+OutArmOffSet[OutOfsBGA_Out]->GetPlace(i, j);
                Prod.OutArm_Pad_Place[i][j]=Prod.iPADView_Z+OutArmOffSet[OutOfsPADView_Out]->GetPlace(i, j);
            }
        }
    }
    //------------------------------------------------------------------------------
    //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    //------------------------------------------------------------------------------
    else if(USE_Fix_AI_CCD && TestIF_File.bEnableFix2BGAAICCD)
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                Prod.OutArm_BGA_Place[i][j]=Prod.iBGAView_Z+OutArmOffSet[OutOfsBGA_Out]->GetPlace(i, j);
            }
        }
    }

    //------------------------------------------------------------------------------
    //Ifor 20191211 : add Scanner AOI
    //------------------------------------------------------------------------------
    if(USE_Scanner_AOI_Inspection==true)
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                Prod.OutArm_ScannerAOI_Place[i][j]=Prod.iScannerAOI_Z+OutArmOffSet[OutOfsScannerAOI]->GetPlace(i, j);
            }
        }
    }

    //------------------------------------------------------------------------------
    //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    //------------------------------------------------------------------------------
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD==true)   //ChungHung 20210113 add for Alignment CCD
    {
        Prod.iInArmXCCDAlignmentPos     = Tech.iInArmXCCDAlignmentPos;
        Prod.iInArmYCCDAlignmentPos     = Tech.iInArmYCCDAlignmentPos;
        Prod.iInArmXBasePickerAlignmentPos  = Tech.iInArmXBasePickerAlignmentPos;
        Prod.iInArmYBasePickerAlignmentPos  = Tech.iInArmYBasePickerAlignmentPos;

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<8; j++)
            {
                Prod.iInArmZBasePickerAlignmentPos[i][j] =Tech.iInArmZBasePickerAlignmentPos[i][j];
                Prod.iOutArmZBasePickerAlignmentPos[i][j]=Tech.iOutArmZBasePickerAlignmentPos[i][j];
            }
        }

        Prod.dInArmCCDXResolution   = Tech.dInArmCCDXResolution;
        Prod.dInArmCCDYResolution   = Tech.dInArmCCDYResolution;
        Prod.dInArmCCDXRadian       = Tech.dInArmCCDXRadian;
        Prod.dInArmCCDYRadian       = Tech.dInArmCCDYRadian;

        Prod.iOutArmXCCDAlignmentPos    = Tech.iOutArmXCCDAlignmentPos;
        Prod.iOutArmYCCDAlignmentPos    = Tech.iOutArmYCCDAlignmentPos;
        Prod.iOutArmXBasePickerAlignmentPos = Tech.iOutArmXBasePickerAlignmentPos;
        Prod.iOutArmYBasePickerAlignmentPos = Tech.iOutArmYBasePickerAlignmentPos;

        Prod.dOutArmCCDXResolution  = Tech.dOutArmCCDXResolution;
        Prod.dOutArmCCDYResolution  = Tech.dOutArmCCDYResolution;
        Prod.dOutArmCCDXRadian      = Tech.dOutArmCCDXRadian;
        Prod.dOutArmCCDYRadian      = Tech.dOutArmCCDYRadian;

        for(int i=0; i<MAX_ARM_Row; i++)    //JerryYang 20240306 : fix AOA
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                Prod.iInArmCCD_Picker_OffsetX[i][j] =  Tech.iInArmCCD_Picker_PosX[i][j];
                Prod.iInArmCCD_Picker_OffsetY[i][j] =  Tech.iInArmCCD_Picker_PosY[i][j];

                Prod.iOutArmCCD_Picker_OffsetX[i][j] =  Tech.iOutArmCCD_Picker_PosX[i][j];
                Prod.iOutArmCCD_Picker_OffsetY[i][j] =  Tech.iOutArmCCD_Picker_PosY[i][j];
            }
        }
    }
    //<==
    //KenHsieh 20210813 : add CCD AUTO ALIGNMENT

    //------------------------------------------------------------------------------
    //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    //------------------------------------------------------------------------------
    Prod.iTopView_X         = Tech.M_iTopView_X     + OutArmOffSet[9]->GetX();
    Prod.iTopView_Y         = Tech.M_iTopView_Y     + OutArmOffSet[9]->GetY();
    Prod.iTopView_Pick      = Tech.M_iTopView_Pick  + OutArmOffSet[9]->GetPickUp();
    Prod.iTopView_Place     = Tech.M_iTopView_Place + OutArmOffSet[9]->GetPlace();
    Prod.iPADView_X         = Tech.M_iPADView_X     + OutArmOffSet[10]->GetX();        //wei 20160617 Vitrox
    Prod.iPADView_Y         = Tech.M_iPADView_Y     + OutArmOffSet[10]->GetY();        //wei 20160617 Vitrox
    Prod.iPADView_Z         = Tech.M_iPADView_Z     + OutArmOffSet[10]->GetPlace();    //wei 20160617 Vitrox
    Prod.iBGAView_X         = Tech.M_iBGAView_X     + OutArmOffSet[11]->GetX();        //wei 20160617 Vitrox
    Prod.iBGAView_Y         = Tech.M_iBGAView_Y     + OutArmOffSet[11]->GetY();        //wei 20160617 Vitrox
    Prod.iBGAView_Z         = Tech.M_iBGAView_Z     + OutArmOffSet[11]->GetPlace();    //wei 20160617 Vitrox
    Prod.iTopViewSafePos_X  = Tech.M_iTopViewSafePos_X;
    Prod.iTopViewSafePos_Y  = Tech.M_iTopViewSafePos_Y;
    Prod.iTopViewKit_Zup    = Tech.M_iTopViewKit_Zup;
    Prod.iTopViewKit_Z      = Tech.M_iTopView_KitZ;

    Prod.iScannerAOI_X      = Tech.M_ScannerAOI_X   + OutArmOffSet[OutOfsScannerAOI]->GetX();       //Ifor 20191211 : add Scanner AOI
    Prod.iScannerAOI_Y      = Tech.M_ScannerAOI_Y   + OutArmOffSet[OutOfsScannerAOI]->GetY();       //Ifor 20191211 : add Scanner AOI
    Prod.iScannerAOI_Z      = Tech.M_ScannerAOI_Z   + OutArmOffSet[OutOfsScannerAOI]->GetPlace();   //Ifor 20191211 : add Scanner AOI

    Prod.iMLoaderYCarPos      = Tech.iMLoaderYCarPos;                           //Frank 20250214 add
    Prod.iMLoaderYOCRPos      = Tech.iMLoaderYOCRPos;
    Prod.iMLoaderYSurePos     = Tech.iMLoaderYSurePos;
}
//------------------------------------------------------------------------------
void SetTechDataToProd_Yield()
{
    int iMode;
    bool bFix3HasBin=false;

    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
    {
        Prod.iT6CatData[i] =iTo6PosUnload[BinSelect[iTestRunMode].iCatDataT3Pos[i]]-1;   //Auto1 = 0
        Prod.iT6PosCate[i] =iTo6PosUnload[BinSelect[iTestRunMode].iCatDataT3Pos[i]];     //Auto1 = 1
        Prod.DBContact[i]  =BinSelect[iTestRunMode].iDBContact[i];
    }

    for(int i6=0; i6<eTrayCount; i6++)
    {
        Prod.iIsFailT6[i6]      =BinSelect[iTestRunMode].iStackDefFailCate [iTo3Unload[i6]];
        Prod.iIsPassT6[i6]      =(Prod.iIsFailT6[i6]==0)?1:0;
        Prod.bART6Tray[i6]      =BinSelect[iTestRunMode].bAutoRetest       [iTo3Unload[i6]];
        Prod.bLinkTo6Tray[i6]   =BinSelect[iTestRunMode].bAutoLink         [iTo3Unload[i6]];
        Prod.bCateRTo6Tray[i6]  =BinSelect[iTestRunMode].bCateR            [iTo3Unload[i6]];

        for(int j=0; j<8; j++)
        {
            Prod.iTo6StackDefFailCate[j][i6] =BinSelect[j].iStackDefFailCate [iTo3Unload[i6]];
            Prod.bTo6AutoRetest[j][i6]       =BinSelect[j].bAutoRetest       [iTo3Unload[i6]];
            Prod.bTo6AutoLink[j][i6]         =BinSelect[j].bAutoLink         [iTo3Unload[i6]];
            Prod.bTo6CateR[j][i6]            =BinSelect[j].bCateR            [iTo3Unload[i6]];
        }
    }

    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20240105 : 換位置
    {
        int iT3=BinSelect[iTestRunMode].iCatDataT3Pos[i]-1;
        if(iT3>=0 && BinSelect[iTestRunMode].iStackDefFailCate[iT3]>0)          //stack define fail
        {
            Prod.bIsPassBin[i]=false;  //fail
            Prod.iIsFailBin[i]=1;
        }
        else
        {
            Prod.bIsPassBin[i]=true;  //pass
            Prod.iIsFailBin[i]=0;
        }
    }

    Prod.iIfErrorT6=iTo6Unload[BinSelect[iTestRunMode].IfErrorT3];

    for(int j=0; j<8; j++)
    {
        Prod.iTo6IfError[j]=iTo6Unload[BinSelect[j].IfErrorT3];
        for(int i=0; i<iTestBinCount; i++)
        {
            Prod.iTo6CatData[j][i]=iTo6PosUnload[BinSelect[j].iCatDataT3Pos[i]]-1; //Auto1 = 0
            Prod.iTo6CatePos[j][i]=iTo6PosUnload[BinSelect[j].iCatDataT3Pos[i]];   //Auto1 = 1
        }
    }

    for(int k=0; k<iTestBinCount; k++)                                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)                                     //設定socket 那個bin是pass那個bin是fail   //Steven 20100113 : 2->MAX_Index_Row
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)                                 //Steven 20100113 : 4->MAX_Index_Col
            {
                //Steven 20140828 : By Bin Yield Monitor
                //Steven 20140828 : By Bin Arm Yield Monitor
                //Steven 20140828 : By Bin Site Yield Monitor
                ArmData             [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmData             [1]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmData             [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmDataLot          [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmDataLot          [1]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmDataLot          [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmHistory          [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmHistory          [1]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmHistory          [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmData_AutoClean   [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmData_AutoClean   [1]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                ArmData_AutoClean   [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
            }
        }

        if(bUseTwoArm32Site==true)
        {
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    ArmData             [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                    ArmDataLot          [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                    ArmHistory          [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                    ArmData_AutoClean   [0]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);

                    ArmData             [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                    ArmDataLot          [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                    ArmHistory          [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                    ArmData_AutoClean   [2]->SetArmBinCodeStatus(i, j, k, Prod.bIsPassBin[k], BinSelect[iTestRunMode].bLowYield[k], BinSelect[iTestRunMode].bArmYield[k], BinSelect[iTestRunMode].bSiteYield[k]);
                }
            }
        }
    }

    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
    {
        if(iTestRunMode<2)                                                      //Steven 20191231 : 修正記憶體破壞
            iMode=iTestRunMode;
        else
            iMode=FT;

        Prod.bScanner[i]     =BinSelect[iTestRunMode].bScanner[i];
        Prod.bConsFail[i]    =BinSelect[iTestRunMode].bConsFail[i];
        if(CosFunction.bDisableRTBinSet && iRunStartMode==RT)                   //wei 20151116 KYEC RT不偵測 Bin Yeild Alarm
        {
            Prod.bFailure[i]     =false;
        }
        else
        {
            if(CosFunction.bByBinAlarmFromYieldForm)                            //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
                Prod.bFailure[i]     =TestIF_File.bByBinFailureEnable[iMode][i];
            else
                Prod.bFailure[i]     =BinSelect[iTestRunMode].bFailure[i];
        }

        if(CosFunction.bByBinAlarmFromYieldForm)                                //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
            Prod.dFailureLimit[i] =TestIF_File.dByBinFailurePercent[iMode][i];
            Prod.iPersentIgnore[i]=TestIF_File.iByBinFailureIgnore[iMode];
        }
        else
        {
            Prod.dFailureLimit[i] =BinSelect[iTestRunMode].dFailureLimit[i];
            Prod.iPersentIgnore[i]=BinSelect[iTestRunMode].iPersentIgnore[i];
        }

        Prod.bFailCountEnable[i]=BinSelect[iTestRunMode].bFailCountEnable[i];                   //Steven 20140529 Start: Fail Persent & Count同時存在
        Prod.iFailCountIgnore[i]=BinSelect[iTestRunMode].iFailCountIgnore[i];
        Prod.iFailCountLimit [i]=BinSelect[iTestRunMode].iFailCountLimit[i];

        Prod.bSpecialBinByArm[i]        =BinSelect[iTestRunMode].bSpecialBinByArm[i];           //ChungHung 20140730 add ContinuousFailHaveOneCycle start
        Prod.iSpecialBinCountByArm[i]   =BinSelect[iTestRunMode].iSpecialBinCountByArm[i];
        Prod.bSpecialBinBySocket[i]     =BinSelect[iTestRunMode].bSpecialBinBySocket[i];
        Prod.iSpecialBinCountBySocket[i]=BinSelect[iTestRunMode].iSpecialBinCountBySocket[i];

        Prod.iAutoCleanByBin[i]         =BinSelect[iTestRunMode].iAutoCleanByBin[i];            //Steven 20160308 : By Bin count do auto clean
        Prod.iAutoCleanBySite[i]        =BinSelect[iTestRunMode].iAutoCleanBySite[i];           //Steven 20160308 : By Bin pre site count do auto clean

        if(CosFunction.bByBinAlarmFromYieldForm)                                //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
            Prod.bSpecBinBySiteCompareEnable[i]         =TestIF_File.bSpecBinBySiteCompareEnable[iTestRunMode][i];
            Prod.iSpecBinBySiteCompareIgnore[i]         =TestIF_File.iSpecBinBySiteCompareIgnore[iTestRunMode];
            Prod.dSpecBinBySiteComparePercent[i]        =TestIF_File.dSpecBinBySiteComparePercent[iTestRunMode][i];
            Prod.bSpecBinByArmPerSiteCompareEnable[i]   =TestIF_File.bSpecBinByArmPerSiteCompareEnable[iTestRunMode][i];
            Prod.iSpecBinByArmPerSiteCompareIgnore[i]   =TestIF_File.iSpecBinByArmPerSiteCompareIgnore[iTestRunMode];
            Prod.dSpecBinByArmPerSiteComparePercent[i]  =TestIF_File.dSpecBinByArmPerSiteComparePercent[iTestRunMode][i];
        }
        else
        {
            Prod.bSpecBinBySiteCompareEnable[i]         =BinSelect[iTestRunMode].bSpecBinBySiteCompareEnable[i];
            Prod.iSpecBinBySiteCompareIgnore[i]         =BinSelect[iTestRunMode].iSpecBinBySiteCompareIgnore[i];
            Prod.dSpecBinBySiteComparePercent[i]        =BinSelect[iTestRunMode].dSpecBinBySiteComparePercent[i];
            Prod.bSpecBinByArmPerSiteCompareEnable[i]   =BinSelect[iTestRunMode].bSpecBinByArmPerSiteCompareEnable[i];
            Prod.iSpecBinByArmPerSiteCompareIgnore[i]   =BinSelect[iTestRunMode].iSpecBinByArmPerSiteCompareIgnore[i];
            Prod.dSpecBinByArmPerSiteComparePercent[i]  =BinSelect[iTestRunMode].dSpecBinByArmPerSiteComparePercent[i];
        }

        Prod.bSCKART_EnableSPBinAlarm    =TestIF_File.bSCKART_EnableSPBinAlarm; //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
        Prod.iSCKART_SPBinSelect         =TestIF_File.iSCKART_SPBinSelect;
        Prod.dSCKART_SPBinAlarmYield     =TestIF_File.dSCKART_SPBinAlarmYield;
    }

    if(IniConfig.bEnableAutoCleanFunction &&                                    //Steven 20120210 Start: Fix3有Bin不能做Auto Clean
       TestIF.iAutoClean_Tray==eCKPos_Fix3 &&
       TestIF.iAutoClean_Function==true)
    {
        for(int i=0; i<iTestBinCount; i++)
        {
           if(Prod.iT6CatData[i]==iFixRight || Prod.iT6CatData[i]==iFixRightHalf)
                bFix3HasBin=true;
        }
        if(Prod.iIfErrorT6==iFixRight || Prod.iIfErrorT6==iFixRightHalf)
            bFix3HasBin=true;

        if(bFix3HasBin==true)
        {
            TestIF.iAutoClean_Function==false;
        }
    }
}
//------------------------------------------------------------------------------
void SetTechDataToProd()
{
    SetTechDataToProd_Tray();
                                                                                //Ifor 20180802 : 往下移避免Tray初始化無數據資料
    if(InitialOK==false)                                                        //JerryYang 20170913 (Steven) 避免還沒讀到motor.db就進來
    {
        return;
    }

    if(MOT[MMTrayY].fHasTray)       MOT[MMTrayY].Refresh();
    if(MOT[MMOCR].fHasTray)         MOT[MMOCR].Refresh();                       //ChungHung 20120830 add OCR Function
    if(MOT[MMEmpty].fHasTray)       MOT[MMEmpty].Refresh();
    if(MOT[MMColor].fHasTray)       MOT[MMColor].Refresh();

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse &&
           Prod.iTrayType[i]!=tTrayBox)
        {
            if(MOT[iMMAuto[i]].fHasTray)
                MOT[iMMAuto[i]].Refresh();
        }
    }

    SetTechDataToProd_TrayArm();
    SetTechDataToProd_Shuttle();
    SetTechDataToProd_InArm();
    SetTechDataToProd_Index();
    SetTechDataToProd_OutArm();
    SetTechDataToProd_AutoClean();
    SetTechDataToProd_AOI();
    SetTechDataToProd_Yield();

    if(CosFunction.bEnableHandlerResultServer)                                              //Sam 20240304 : 新增 AMR 功能
    {
        if(IniConfig.bA60EnableAMR)
        {
            ArmSpeed_File[InArm].bAutoSKIP=true;
            ArmSpeed_File[InArm].iAutoSkipCT=LoadForm->XDivision*LoadForm->YDivision;
            ArmSpeed[InArm].bAutoSKIP=true;
            ArmSpeed[InArm].iAutoSkipCT=LoadForm->XDivision*LoadForm->YDivision;
        }
        else
        {
            ArmSpeed_File[InArm].bAutoSKIP=false;
            ArmSpeed[InArm].bAutoSKIP=false;
        }
    }

    if(MOT[MMTrayY].Tray.XItem!=LoadForm->XDivision*LoadForm->BlockXItem ||
       MOT[MMTrayY].Tray.YItem!=LoadForm->YDivision*LoadForm->BlockYItem)
    {
        MOT[MMTrayY].Tray.SetBlockXYItem(LoadForm->BlockXItem, LoadForm->BlockYItem);       //Steven 20160614 : 設定Tray XY Item改用function加上保護
        MOT[MMTrayY].Tray.SetXYItem(LoadForm->XDivision, LoadForm->YDivision);              //Steven 20160614 : 設定Tray XY Item改用function加上保護
        MOT[MMTrayY].InitNewTray(NULL_IC, false, __FUNC__);
    }

    if(MOT[MMOCR].Tray.XItem!=LoadForm->XDivision*LoadForm->BlockXItem ||                   //ChungHung 20120830 add OCR Function
       MOT[MMOCR].Tray.YItem!=LoadForm->YDivision*LoadForm->BlockYItem)
    {
        MOT[MMOCR].Tray.SetBlockXYItem(LoadForm->BlockXItem, LoadForm->BlockYItem);         //Steven 20160614 : 設定Tray XY Item改用function加上保護
        MOT[MMOCR].Tray.SetXYItem(LoadForm->XDivision, LoadForm->YDivision);                //Steven 20160614 : 設定Tray XY Item改用function加上保護
        MOT[MMOCR].InitNewTray(NULL_IC, false, __FUNC__);
    }

    if(MOT[MMTrayY_Car].Tray.XItem!=LoadForm->XDivision*LoadForm->BlockXItem ||
       MOT[MMTrayY_Car].Tray.YItem!=LoadForm->YDivision*LoadForm->BlockYItem)
    {
        MOT[MMTrayY_Car].Tray.SetBlockXYItem(LoadForm->BlockXItem, LoadForm->BlockYItem);   //Steven 20160614 : 設定Tray XY Item改用function加上保護
        MOT[MMTrayY_Car].Tray.SetXYItem(LoadForm->XDivision, LoadForm->YDivision);          //Steven 20160614 : 設定Tray XY Item改用function加上保護
        MOT[MMTrayY_Car].InitNewTray(NULL_IC, false, __FUNC__);
    }

    MOT[MMEmpty     ].Tray.SetXYItem(EmptyForm->XDivision, EmptyForm->YDivision);           //Steven 20160614 : 設定Tray XY Item改用function加上保護
    MOT[MMEmpty_Car ].Tray.SetXYItem(EmptyForm->XDivision, EmptyForm->YDivision);
    MOT[MMColor     ].Tray.SetXYItem(ColorForm->XDivision, ColorForm->YDivision);
    MOT[MMColor_Car ].Tray.SetXYItem(ColorForm->XDivision, ColorForm->YDivision);

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&
       TestIF_File.bEnableAutoAlignment==true)                                              //KenHsieh 20211015 : ASE要求版號新增AOA
    {
        MOT[MMAOASampleTray].Tray.SetXYItem(LoadForm->XDivision, LoadForm->YDivision);      //ChungHung 20210113 add for Alignment CCD start
        MOT[MMAOASampleTray].InitNewTray(NULL_IC, false, __FUNC__);                         //Mylin 20210504 Modify add for Alignment CCD //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
        MOT[MMAOASampleTray].fHasTray = true;                                               //KenHsieh 20211013 : Modify Tray Scan
        MOT[MMAOASampleTray].InitNewTray(WAIT_ALIGN_IC, false, __FUNC__);                   //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    }
    SetAOATrayTeachPoint(TestIF.iAlignmentPointX, TestIF.iAlignmentPointY, &MOT[MMAOASampleTray], true);

    Prod.dInArmCCDXResolution    =Tech.dInArmCCDXResolution    ;
    Prod.dInArmCCDYResolution    =Tech.dInArmCCDYResolution    ;
    Prod.dInArmCCDXRadian        =Tech.dInArmCCDXRadian        ;
    Prod.dInArmCCDYRadian        =Tech.dInArmCCDYRadian        ;

    Prod.dOutArmCCDXResolution   =Tech.dOutArmCCDXResolution   ;
    Prod.dOutArmCCDYResolution   =Tech.dOutArmCCDYResolution   ;
    Prod.dOutArmCCDXRadian       =Tech.dOutArmCCDXRadian       ;
    Prod.dOutArmCCDYRadian       =Tech.dOutArmCCDYRadian       ;

    Prod.LoadForm.iXPitch=LoadForm->XPitch;
    Prod.LoadForm.iYPitch=LoadForm->YPitch;
    Prod.LoadForm.iXItem =LoadForm->XDivision;
    Prod.LoadForm.iYItem =LoadForm->YDivision;                                  //Steven 20240701 : 修正Fix模式下, X Pitch錯誤

    for(int i=0; i<ePosTrayCount; i++)
    {
        if(Prod.iTrayType[i]==tTrayAuto)
        {
            if(IniConfig.bP06_LoaderUseCarrierTray==false)                      //wei 20161123 未使用CarrierTray就初始化
            {
                AutoForm[i]->BlockYItem=1;
                AutoForm[i]->BlockXItem=1;
                AutoForm[i]->BlockXStart=0.0;
                AutoForm[i]->BlockYStart=0.0;
                AutoForm[i]->BlockPitchX=0.0;
                AutoForm[i]->BlockPitchY=0.0;
            }

            if(MOT[iMMAuto[i]].Tray.XItem!=AutoForm[i]->XDivision*LoadForm->BlockXItem ||
               MOT[iMMAuto[i]].Tray.YItem!=AutoForm[i]->YDivision*LoadForm->BlockYItem)
            {
                MOT[iMMAuto[i]].Tray.SetBlockXYItem(AutoForm[i]->BlockXItem, AutoForm[i]->BlockYItem);      //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[iMMAuto[i]].Tray.SetXYItem(AutoForm[i]->XDivision, AutoForm[i]->YDivision);             //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[iMMAuto[i]].InitNewTray(NULL_IC, false, __FUNC__);
            }

            if(MOT[iMMAuto_Car[i]].Tray.XItem!=AutoForm[i]->XDivision)
            {
                MOT[iMMAuto_Car[i]].Tray.SetBlockXYItem(AutoForm[i]->BlockXItem, AutoForm[i]->BlockYItem);  //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[iMMAuto_Car[i]].Tray.SetXYItem(AutoForm[i]->XDivision, AutoForm[i]->YDivision);         //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[iMMAuto_Car[i]].InitNewTray(NULL_IC, false, __FUNC__);
            }
        }
        else if(Prod.iTrayType[i]==tTrayFix && i<=iFixRight)
        {
            if(IniConfig.bP06_LoaderUseCarrierTray==false)                      //wei 20161123 未使用CarrierTray就初始化
            {
                AutoForm[i]->BlockYItem=1;
                AutoForm[i]->BlockXItem=1;
                AutoForm[i]->BlockXStart=0.0;
                AutoForm[i]->BlockYStart=0.0;
                AutoForm[i]->BlockPitchX=0.0;
                AutoForm[i]->BlockPitchY=0.0;
            }

            if(MOT[iMMAuto[i]].Tray.YItem!=AutoForm[i]->YDivision*LoadForm->BlockYItem ||
               MOT[iMMAuto[i]].Tray.XItem!=AutoForm[i]->XDivision*LoadForm->BlockXItem)
            {
                MOT[iMMAuto[i]].Tray.SetBlockXYItem(AutoForm[i]->BlockXItem, AutoForm[i]->BlockYItem);      //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[iMMAuto[i]].Tray.SetXYItem(AutoForm[i]->XDivision, AutoForm[i]->YDivision);             //Steven 20160614 : 設定Tray XY Item改用function加上保護 //Ifor 20160720 MManualTray 位置錯誤 AutoForm=>FixForm
                //InitNewFixTray(i, "SetTechDataToProd", 2);                                                  //Steven 20160414 : 整合Fix盤設定
            }
        }
        else if(Prod.iTrayType[i]==tTrayMag)
        {
            if(MOT[iMMAuto[i]].Tray.XItem!=AutoForm[iMagAtAuto]->XDivision*LoadForm->BlockXItem ||
               MOT[iMMAuto[i]].Tray.YItem!=AutoForm[iMagAtAuto]->YDivision*LoadForm->BlockYItem)
            {
                MOT[iMMAuto[i]].Tray.SetBlockXYItem(AutoForm[iMagAtAuto]->BlockXItem, AutoForm[iMagAtAuto]->BlockYItem);   //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[iMMAuto[i]].Tray.SetXYItem(AutoForm[iMagAtAuto]->XDivision, AutoForm[iMagAtAuto]->YDivision);          //Steven 20160614 : 設定Tray XY Item改用function加上保護
                MOT[iMMAuto[i]].InitNewTray(NULL_IC, false, __FUNC__);
            }
        }
    }

    for(int i=0; i<2; i++)
    {
        Prod.HotPlateForm[i].iXPitch=(HotPlateForm.XPitch);
        Prod.HotPlateForm[i].iYPitch=(HotPlateForm.YPitch);
        MOT[MMPlate1+i].Tray.SetXYItem(HotPlateForm.XDivision, HotPlateForm.YDivision);     //Steven 20160614 : 設定Tray XY Item改用function加上保護
    }
    MOT[MMPlate1].fHasTray=true;
    MOT[MMPlate2].fHasTray=true;

    fMain->mtPlate1->XItem=HotPlateForm.XDivision;
    fMain->mtPlate1->YItem=HotPlateForm.YDivision;
    fMain->mtPlate2->XItem=HotPlateForm.XDivision;
    fMain->mtPlate2->YItem=HotPlateForm.YDivision;

    //AI(ht9045-v899) 20260405: Phase4 init SiteUseMgr after HP setup complete
    SiteUseMgr.Init();

    MOT[MMAOASamplePlate].Alias = "MMAOASamplePlate";                           //Ztex 2024.07.03 Add HT-1032 AT AOA Sample Plate
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&                                        //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
       TestIF_File.bEnableAutoAlignment==true)                                  //KenHsieh 20211015 : ASE要求版號新增AOA
    {
        MOT[MMAOASamplePlate].Tray.SetXYItem(HotPlateForm.XDivision, HotPlateForm.YDivision);
        MOT[MMAOASamplePlate].InitNewTray(NULL_IC, false, __FUNC__);
        MOT[MMAOASamplePlate].fHasTray = true;                                  //Mylin 20210504 Modify add for Alignment CCD         //KenHsieh 20211013 : Modify Hotplate Scan
        MOT[MMAOASamplePlate].InitNewTray(WAIT_ALIGN_IC, false, __FUNC__);      //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT   //KenHsieh 20211013 : Modify Hotplate Scan
    }
    SetAOATrayTeachPoint(TestIF.iAlignmentPlatePointX, TestIF.iAlignmentPlatePointY, &MOT[MMAOASamplePlate]);

    if(USE_ROTATE_KIT==1 && tRotate.ActiveRotate)                               //kevin 20130629
    {                                                                           //jou 2013-03-01 Rotate kit
        MOT[MInRotateKit].fHasTray=true;
        if(iRotate_Type==eCynRotate)                                            //氣缸版 //kevin 20130629 add
        {
            if(iRotate_In_Index==0)                                             //Hotplate 1
                MOT[MMPlate1].fHasTray=false;                                   //Steven 20131029 : 解決Index Position Error
            else                                                                //Hotplate 2
                MOT[MMPlate2].fHasTray=false;

            if(iRotate_Out_Tray6==0)                                            //Auto 1
                MOT[MMAuto1].fHasTray=false;
            else if(iRotate_Out_Tray6==1)                                       //Auto 1//Auto 2
                MOT[MMAuto2].fHasTray=false;
            else if(iRotate_Out_Tray6==2)                                       //Auto 1//Auto 3   kevin 20130416
                MOT[MMAuto3].fHasTray=false;
        }
        MOT[MOutRotateKit].fHasTray=true;

        if(MOT[MInRotateKit].HasRealIC()==false)                                //Steven 20210610 : 修正歸零後, Rotator顯示異常的問題
        {
            if(iRotate_Type==e1MotRotate1Dut ||                                 //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
               iRotate_Type==eInOutArm1Motor)                                   //Frank 20250812 : modify InOutArm1Motor
            {
                MOT[MInRotateKit].Tray.SetXYItem(1, 1);
                MOT[MOutRotateKit].Tray.SetXYItem(1, 1);
                fMain->tmyInputRotateKit->XItem=1;
                fMain->tmyInputRotateKit->YItem=1;
                fMain->tmyOutputRotateKit->XItem=1;
                fMain->tmyOutputRotateKit->YItem=1;
            }
            else if(iRotate_Type==e2MotRotate2Dut)                              //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {
                MOT[MInRotateKit].Tray.SetXYItem(2, 1);
                MOT[MOutRotateKit].Tray.SetXYItem(2, 1);
                fMain->tmyInputRotateKit->XItem=2;
                fMain->tmyInputRotateKit->YItem=1;
                fMain->tmyOutputRotateKit->XItem=2;
                fMain->tmyOutputRotateKit->YItem=1;
            }
            else
            {
                if(tRotate.DutNum==tDutType_8)                                  //kevin 20130517 add 8 site
                {
                    MOT[MInRotateKit].Tray.SetXYItem(4, 2);                     //Steven 20160614 : 設定Tray XY Item改用function加上保護
                    fMain->tmyInputRotateKit->XItem=4;
                    fMain->tmyInputRotateKit->YItem=2;
                }
                else
                {
                    MOT[MInRotateKit].Tray.SetXYItem(2, 2);                     //Steven 20160614 : 設定Tray XY Item改用function加上保護
                    fMain->tmyInputRotateKit->XItem=2;
                    fMain->tmyInputRotateKit->YItem=2;
                }
            }
        }

        if(MOT[MOutRotateKit].HasRealIC()==false)                               //Steven 20210610 : 修正歸零後, Rotator顯示異常的問題
        {
            if(iRotate_Type==e1MotRotate1Dut ||                                 //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
               iRotate_Type==eInOutArm1Motor)                                   //Frank 20250812 : modify InOutArm1Motor
            {
                MOT[MOutRotateKit].Tray.SetXYItem(1, 1);
                fMain->tmyOutputRotateKit->XItem=1;
                fMain->tmyOutputRotateKit->YItem=1;
            }
            else if(iRotate_Type==e2MotRotate2Dut)                              //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
            {
                MOT[MOutRotateKit].Tray.SetXYItem(2, 1);
                fMain->tmyOutputRotateKit->XItem=2;
                fMain->tmyOutputRotateKit->YItem=1;
            }
            else
            {
                if(tRotate.DutNum==tDutType_8)                                  //kevin 20130517 add 8 site
                {
                    MOT[MOutRotateKit].Tray.SetXYItem(4, 2);
                    fMain->tmyOutputRotateKit->XItem=4;
                    fMain->tmyOutputRotateKit->YItem=2;
                }
                else
                {
                    MOT[MOutRotateKit].Tray.SetXYItem(2, 2);
                    fMain->tmyOutputRotateKit->XItem=2;
                    fMain->tmyOutputRotateKit->YItem=2;
                }
            }
        }
    }
    else
    {
        MOT[MInRotateKit].fHasTray=false;
        MOT[MOutRotateKit].fHasTray=false;
    }

    if(HotPlateForm.iPlateSelect==1)                                            //Hotplate 1
    {
        iPickPlate[0]=1;
        iPickPlate[1]=1;
        iPlacePlate[0]=1;
        iPlacePlate[1]=1;
        Prod.bPlateSelect[0]=false;                                             //Hotplate 2
        Prod.bPlateSelect[1]=true;                                              //Hotplate 1
        if(TestIF_File.iAutoClean_Function && TestIF_File.iAutoClean_Tray==eCKPos_HP2) //kevin 20141020  使用AUTOCLEAN要顯示HOTPLATE 2
            fMain->mtPlate1->Visible=true;
        else
            fMain->mtPlate1->Visible=false;
        fMain->mtPlate2->Visible=true;
    }
    else if(HotPlateForm.iPlateSelect==2)                                       //Hotplate 2
    {
        iPickPlate[0]=0;
        iPickPlate[1]=0;
        iPlacePlate[0]=0;
        iPlacePlate[1]=0;
        Prod.bPlateSelect[0]=true;                                              //Hotplate 2
        Prod.bPlateSelect[1]=false;                                             //Hotplate 1
        fMain->mtPlate2->Visible=false;
        fMain->mtPlate1->Visible=true;
    }
    else                                                                        //Hotplate 1 & 2
    {                                                                           //ChungHung 20120821 不能放這每次Start都會執行所以無法確定 iPickPlate 和 iPlacePlate 目前是 0或1
        if(MOT[MMPlate1].HasIC()==false && MOT[MMPlate2].HasIC()==false)
        {
            iPickPlate[0]=0;
            iPickPlate[1]=0;
            iPlacePlate[0]=0;
            iPlacePlate[1]=0;
        }
        Prod.bPlateSelect[0]=true;
        Prod.bPlateSelect[1]=true;
        fMain->mtPlate1->Visible=true;
        fMain->mtPlate2->Visible=true;
    }

    TestIF.dHeadXPitch=8000;
    iAuto1ZeroPos=-10;

    const double dMin=0.95, dMax=1.05;                                          //Steven 20090717 : Limitation of X/Y Scale
    for(int i=0; i<eTrayCount; i++)
    {
        if(IniConfig.dTrayXScale[i]<dMin || IniConfig.dTrayXScale[i]>dMax)    IniConfig.dTrayXScale[i]=1;
        if(IniConfig.dTrayYScale[i]<dMin || IniConfig.dTrayYScale[i]>dMax)    IniConfig.dTrayYScale[i]=1;

        if(IniConfig.dTrayXScale_Hot[i]<dMin  || IniConfig.dTrayXScale_Hot[i]>dMax)    IniConfig.dTrayXScale_Hot[i] =1;//Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
        if(IniConfig.dTrayYScale_Hot[i]<dMin  || IniConfig.dTrayYScale_Hot[i]>dMax)    IniConfig.dTrayYScale_Hot[i] =1;
        if(IniConfig.dTrayXScale_Cold[i]<dMin || IniConfig.dTrayXScale_Cold[i]>dMax)   IniConfig.dTrayXScale_Cold[i]=1;
        if(IniConfig.dTrayYScale_Cold[i]<dMin || IniConfig.dTrayYScale_Cold[i]>dMax)   IniConfig.dTrayYScale_Cold[i]=1;//Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
    }

    if(LastSet.fLoaderTrayXScale<dMin || LastSet.fLoaderTrayXScale>dMax)  LastSet.fLoaderTrayXScale=1;
    if(LastSet.fLoaderTrayYScale<dMin || LastSet.fLoaderTrayYScale>dMax)  LastSet.fLoaderTrayYScale=1;

    if(LastSet.fLoaderTrayXScale_Hot<dMin  || LastSet.fLoaderTrayXScale_Hot>dMax)   LastSet.fLoaderTrayXScale_Hot =1;//Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
    if(LastSet.fLoaderTrayYScale_Hot<dMin  || LastSet.fLoaderTrayYScale_Hot>dMax)   LastSet.fLoaderTrayYScale_Hot =1;
    if(LastSet.fLoaderTrayXScale_Cold<dMin || LastSet.fLoaderTrayXScale_Cold>dMax)  LastSet.fLoaderTrayXScale_Cold=1;
    if(LastSet.fLoaderTrayYScale_Cold<dMin || LastSet.fLoaderTrayYScale_Cold>dMax)  LastSet.fLoaderTrayYScale_Cold=1;//Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--

    for(int i=0; i<2; i++)
    {
        if(LastSet.fHotPlateXScale[i]  <dMin || LastSet.fHotPlateXScale[i]  >dMax) LastSet.fHotPlateXScale[i]  =1;
        if(LastSet.fHotPlateYScale[i]  <dMin || LastSet.fHotPlateYScale[i]  >dMax) LastSet.fHotPlateYScale[i]  =1;
        if(LastSet.fInShuttleXScale[i] <dMin || LastSet.fInShuttleXScale[i] >dMax) LastSet.fInShuttleXScale[i] =1;
        if(LastSet.fInShuttleYScale[i] <dMin || LastSet.fInShuttleYScale[i] >dMax) LastSet.fInShuttleYScale[i] =1;
        if(LastSet.fOutShuttleXScale[i]<dMin || LastSet.fOutShuttleXScale[i]>dMax) LastSet.fOutShuttleXScale[i]=1;
        if(LastSet.fOutShuttleYScale[i]<dMin || LastSet.fOutShuttleYScale[i]>dMax) LastSet.fOutShuttleYScale[i]=1;

        if(LastSet.fHotPlateXScale_Hot[i]  <dMin || LastSet.fHotPlateXScale_Hot[i]  >dMax) LastSet.fHotPlateXScale_Hot[i]  =1;   //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
        if(LastSet.fHotPlateYScale_Hot[i]  <dMin || LastSet.fHotPlateYScale_Hot[i]  >dMax) LastSet.fHotPlateYScale_Hot[i]  =1;
        if(LastSet.fInShuttleXScale_Hot[i] <dMin || LastSet.fInShuttleXScale_Hot[i] >dMax) LastSet.fInShuttleXScale_Hot[i] =1;
        if(LastSet.fInShuttleYScale_Hot[i] <dMin || LastSet.fInShuttleYScale_Hot[i] >dMax) LastSet.fInShuttleYScale_Hot[i] =1;
        if(LastSet.fOutShuttleXScale_Hot[i]<dMin || LastSet.fOutShuttleXScale_Hot[i]>dMax) LastSet.fOutShuttleXScale_Hot[i]=1;
        if(LastSet.fOutShuttleYScale_Hot[i]<dMin || LastSet.fOutShuttleYScale_Hot[i]>dMax) LastSet.fOutShuttleYScale_Hot[i]=1;

        if(LastSet.fHotPlateXScale_Cold[i]  <dMin || LastSet.fHotPlateXScale_Cold[i]  >dMax) LastSet.fHotPlateXScale_Cold[i]  =1;
        if(LastSet.fHotPlateYScale_Cold[i]  <dMin || LastSet.fHotPlateYScale_Cold[i]  >dMax) LastSet.fHotPlateYScale_Cold[i]  =1;
        if(LastSet.fInShuttleXScale_Cold[i] <dMin || LastSet.fInShuttleXScale_Cold[i] >dMax) LastSet.fInShuttleXScale_Cold[i] =1;
        if(LastSet.fInShuttleYScale_Cold[i] <dMin || LastSet.fInShuttleYScale_Cold[i] >dMax) LastSet.fInShuttleYScale_Cold[i] =1;
        if(LastSet.fOutShuttleXScale_Cold[i]<dMin || LastSet.fOutShuttleXScale_Cold[i]>dMax) LastSet.fOutShuttleXScale_Cold[i]=1;
        if(LastSet.fOutShuttleYScale_Cold[i]<dMin || LastSet.fOutShuttleYScale_Cold[i]>dMax) LastSet.fOutShuttleYScale_Cold[i]=1;//Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
    }

    Prod.iMaxPreasure=120;                                                      //Steven 20161024 : fixed for contact torque

    //以上得保持在最下面-------------
}
//------------------------------------------------------------------------------
bool CheckFix3FullPlaceTechData()                                               //Steven 20130126 : Fix3滿盤功能
{
    bool bResult=true;

    if(FIX3_FULL_PLACE!=Fix3K_ShortShuttle)                                     //Steven 20250814 : Fix3滿盤功能-馬達版+Sensor
        return bResult;

    if(Sen[SnFix3FullPlace].Enable && Sen[SnFix3FullPlace].IsOff())             //不滿盤
    {                                                       //原本機台的數字
        if(Tech.iInArmShuttle1X>34000)                      //32434
            bResult=false;
        if(Tech.iInArmShuttle2X>34000)                      //32410
            bResult=false;
        if(Tech.iOutArmShuttle1X<-50000)                    //-48871
            bResult=false;
        if(Tech.iOutArmShuttle2X<-50000)                    //-48909
            bResult=false;
        if(Tech.iOutArmFix1X<-37000)                        //-35692
             bResult=false;
        if(Tech.iOutArmFix2X<-23000)                        //-21404
            bResult=false;
        if(Tech.iOutArmFix3X<-9000)                         //-7113
            bResult=false;
        if(Tech.iInSH1Sen7DetectPos!=0 && Tech.iInSH1Sen7DetectPos>1000)            //116
            bResult=false;
        if(Tech.iInSH2Sen7DetectPos!=0 && Tech.iInSH2Sen7DetectPos>1000)            //116
            bResult=false;
    }
    else if(Sen[SnFix3FullPlace].Enable && Sen[SnFix3FullPlace].IsOn())     //滿盤
    {
        if(Tech.iInArmShuttle1X<34000)                      //32434
            bResult=false;
        if(Tech.iInArmShuttle2X<34000)                      //32410
            bResult=false;
        if(Tech.iOutArmShuttle1X>-50000)                    //-48871
            bResult=false;
        if(Tech.iOutArmShuttle2X>-50000)                    //-48909
            bResult=false;
        if(Tech.iOutArmFix1X>-37000)                        //-35692
             bResult=false;
        if(Tech.iOutArmFix2X>-23000)                        //-21404
            bResult=false;
        if(Tech.iOutArmFix3X>-9000)                         //-7113
            bResult=false;
        if(Tech.iInSH1Sen7DetectPos!=0 && Tech.iInSH1Sen7DetectPos<1000)            //116
            bResult=false;
        if(Tech.iInSH2Sen7DetectPos!=0 && Tech.iInSH2Sen7DetectPos<1000)            //116
            bResult=false;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void ChangeSite()
{
   if(IniConfig.bP27AutoSortingBinTrayByOutArmwhenCleanOut && bSortingSuckMode) //Sam 20250415 : 修正 P27 整盤功能
        return;
    int iCH=0;
    DoInArm_9045_Type();
    fTemp_Set->InitialAddrToATC();                                              //Steven 20241105 : 更新轉換矩陣
    //jou 980312
    CopyKitSuck(&FTestSuckBackup, &FTestSuck);
    CopyKitSuck(&BTestSuckBackup, &BTestSuck);

    SetInOutArmParameter();

    if(TestIF_File.iTestMode>=SingleSite && TestIF_File.iTestMode<=QualSite1X4)
    {
        //Alick 20161011 (Steven) : TTL支援8Site
        //==>
        if(CUSTOMER_CODE==CC_JCET &&
           (TTLCfg.iCateBitLength==_8Bit || TTLCfg.iCateBitLength==_10Bit ||
            TTLCfg.iCateBitLength==_10BitPE || TTLCfg.iCateBitLength==_10BitPO))
        {
            //Site1
            iCH=TestIF_File.iSiteMap[0][0]-1;
            if(iCH>=0 && iCH<16)
            {
                CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart0]);
                CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut0]);
                for(int k=0; k<10; k++)
                    CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit0+k]);
            }
            //Site2
            iCH=TestIF_File.iSiteMap[0][1]-1;
            if(iCH>=0 && iCH<16)
            {
                CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart1]);
                CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut1]);
                for(int k=0; k<10; k++)
                    CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit10+k]);
            }

            if(CosFunction.bTTLCanUse8Site)
            {
                //Site3
                iCH=TestIF_File.iSiteMap[0][2]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart2]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut2]);
                    for(int k=0; k<10; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit20+k]);
                }
                //Site4
                iCH=TestIF_File.iSiteMap[0][3]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart3]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut3]);
                    for(int k=0; k<10; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit30+k]);
                }
            }
        }
        else
        //<==
        //Alick 20161011 (Steven) : TTL支援8Site
        {
            //Site1
            iCH=TestIF_File.iSiteMap[0][0]-1;
            if(iCH>=0 && iCH<16)
            {
                CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart0]);
                CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut0]);
                for(int k=0; k<5; k++)
                    CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit0+k]);
            }
            //Site2
            iCH=TestIF_File.iSiteMap[0][1]-1;
            if(iCH>=0 && iCH<16)
            {
                CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart1]);
                CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut1]);
                for(int k=0; k<5; k++)
                    CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit5+k]);
            }
            //Site3
            iCH=TestIF_File.iSiteMap[0][2]-1;
            if(iCH>=0 && iCH<16)
            {
                CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart2]);
                CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut2]);
                for(int k=0; k<5; k++)
                    CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit10+k]);
            }
            //Site4
            iCH=TestIF_File.iSiteMap[0][3]-1;
            if(iCH>=0 && iCH<16)
            {
                CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart3]);
                CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut3]);
                for(int k=0; k<5; k++)
                    CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit15+k]);
            }
        }

        if(TestIF_File.iTestMode==SingleSite)                                   //2013-08-15 Dell   for TSMC 無ATC 也有SingleSite
        {
            if(MachineTypeChoice==Type_HT9046 ||
               MachineTypeChoice==Type_HT9046_LS ||
               MachineTypeChoice==Type_HT1032 ||
               (USE_46_SUCKER_DB==1 && MachineTypeChoice==Type_HT9045_12Site))  //Steven 20131008 : HT9045使用46配氣
            {
                if(TestIF_File.bNS7000kit==false)                               //kevin 20150416  add 偏心 Aa
                {
                    CopySuck(&FTestSuckBackup.Suck[0][5], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][5]);

                    CopySuck(&BTestSuckBackup.Suck[1][5], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[1][5]);
                }
            }
        }
        if(TestIF_File.iTestMode==DualSite)// || TestIF_File.iTestMode==DualSiteBS) //1x2 || 1x1 //Eliot 2009_03_14   //Frank 20200520 2X2NN Mode
        {                                                                       //Steven 20091028 Start : 1x2模式要使用第1和3的真空產生器
            if(USE_46_SUCKER_DB==1 && MachineTypeChoice==Type_HT9045_12Site)    //Steven 20140312 : For HT9045WA
            {
                if(TestIF_File.bNS7000kit==false)
                {
                    CopySuck(&FTestSuckBackup.Suck[0][5], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][1]);

                    CopySuck(&BTestSuckBackup.Suck[1][5], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][1]);

                    //Steven 20150116 : Fixed掉料問題
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[0][4]);
                    CopySuck(&FTestSuckBackup.Suck[0][4], &FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][6], &FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][7], &FTestSuck.Suck[0][7]);

                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[0][4], &BTestSuck.Suck[0][4]);
                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][6], &BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][7], &BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[1][5]);
                }
                else
                {
                    CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[1][0]);

                    CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[1][0]);
                }
            }
            else if(USE_46_SUCKER_DB==1 && MachineTypeChoice==Type_HT9045)      //Steven 20141027 : For HT9045 use 46DB
            {
                if(TestIF_File.bNS7000kit==false)
                {
                    //Isaac 20200311 : 1x2Site使用1x4Site Kit
                    //==>
                    if(TestIF_File.b1x2Use1x4SiteKit)                           //Isaac 20200311 : 1x2Site使用1x4Site Kit
                    {
                        CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);      //真空產生器1->Aa
                        CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[0][1]);      //真空產生器7->Ab

                        CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                        CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[0][1]);

                        //Fixed掉料問題
                        CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][2]);
                        CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][3]);
                        CopySuck(&FTestSuckBackup.Suck[0][4], &FTestSuck.Suck[0][4]);
                        CopySuck(&FTestSuckBackup.Suck[0][5], &FTestSuck.Suck[0][5]);
                        CopySuck(&FTestSuckBackup.Suck[0][6], &FTestSuck.Suck[0][6]);
                        CopySuck(&FTestSuckBackup.Suck[0][7], &FTestSuck.Suck[0][7]);

                        CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][2]);
                        CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][3]);
                        CopySuck(&BTestSuckBackup.Suck[0][4], &BTestSuck.Suck[0][4]);
                        CopySuck(&BTestSuckBackup.Suck[0][5], &BTestSuck.Suck[0][5]);
                        CopySuck(&BTestSuckBackup.Suck[0][6], &BTestSuck.Suck[0][6]);
                        CopySuck(&BTestSuckBackup.Suck[0][7], &BTestSuck.Suck[0][7]);
                    }
                    //<==
                    //Isaac 20200311 : 1x2Site使用1x4Site Kit
                }
                else
                {
                    CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[1][0]);

                    CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[1][0]);
                }
            }
            else if(MachineTypeChoice==Type_HT9045)                             //9045
            {
                if(TestIF_File.bNS7000kit==false)
                {
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][2]);

                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][2]);
                }
            }
            else if(MachineTypeChoice==Type_HT9045_12Site)                      //ChungHung 20130507 add HT9045 updata for 12site 517
            {
//ChungHung 20130705 chage mapping
                if(TestIF_File.bNS7000kit==false)
                {
                    CopySuck(&FTestSuckBackup.Suck[1][4], &FTestSuck.Suck[0][0]);         //wei 20150217  1x2位置錯誤 USE old 45 SUCKER.DB
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][1]);

                    CopySuck(&BTestSuckBackup.Suck[1][5], &BTestSuck.Suck[0][0]);         //wei 20150217  1x2位置錯誤 USE old 45 SUCKER.DB
                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][1]);

                    //Steven 20150116 : Fixed掉料問題    //wei 20150217  1x2位置錯誤 add
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[0][4], &FTestSuck.Suck[0][4]);
                    CopySuck(&FTestSuckBackup.Suck[0][5], &FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][6], &FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][7], &FTestSuck.Suck[0][7]);
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[1][4]);

                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[0][4], &BTestSuck.Suck[0][4]);
                    CopySuck(&BTestSuckBackup.Suck[0][5], &BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][6], &BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][7], &BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[1][5]);
                }
            }
            else
            {
                if(TestIF_File.bNS7000kit==false && TestIF_File.bUse1x3SiteKit) //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][1]);

                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][1]);
                }
                else if(TestIF_File.bNS7000kit==false)
                {
                    CopySuck(&FTestSuckBackup.Suck[0][5], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][6], &FTestSuck.Suck[0][1]);

                    CopySuck(&BTestSuckBackup.Suck[1][5], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][6], &BTestSuck.Suck[0][1]);

                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][6]);

                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[1][6]);
                }
                else
                {
                    if(TestIF.iTestMode==DualSite && TestIF_File.bQualSite2X2Shift && (TestIF_File.bNS7000kit || TestIF_File.bNS7000CS))//kevin 20170513 (wei) add
                    {
                        CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);
                        CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][2]);

                        CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                        CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][2]);
                    }
                    else
                    {
                        CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[0][1]);
                        CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[1][0]);

                        CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[0][1]);
                        CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[1][0]);
                    }
                }
            }
            //Steven 20091028 End
        }
        else if(TestIF_File.iTestMode==QualSite1X4 || TestIF_File.iTestMode==_8Site1X4)                                 //ChungHung 20150528 add for 海思 _8Site1x4   //1X4
        {
            if((USE_46_SUCKER_DB==1               && TestIF_File.bNS7000kit==false &&                                   //kevin 20220921 add //Steven 20131008 : HT9045使用46配氣
                                                     TestIF_File.bNS7000CS==false) ||                                   //jou 20230823 : 修正9045 1x4 NS SLK 真空開啟異常
               (MachineTypeChoice==Type_HT9046    && TestIF_File.bNS7000kit==false && TestIF_File.bNS7000CS==false) ||  //kevin 20140829 ASE使用偏心氣孔 //Steven 20140328 : For 9046 1x4使用HT KIT
               (MachineTypeChoice==Type_HT9046_LS && TestIF_File.bNS7000kit==false && TestIF_File.bNS7000CS==false))    //Steven 20140328 : For 9046 1x4使用HT KIT
            {
                //Aa --> 1
                //Ab --> 3
                //Ac --> 5
                //Ad --> 7
            }
            else if(MachineTypeChoice==Type_HT9045 && USE_46_SUCKER_DB==0)      //Steven 20211103 : 重整1x4的真空對應
            {
                if(TestIF_File.bNS7000kit==false &&
                   TestIF_File.bNS7000CS==false)                                //kevin 20140829
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2], &FTestSuck.Suck[0][3]);

                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][1], &FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][3], &FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2], &BTestSuck.Suck[0][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][1], &BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][3], &BTestSuck.Suck[1][3]);
                }
                else
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[0][3]);

                    CopySuck(&FTestSuckBackup.Suck[1][1], &FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][1], &FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][2], &FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][3], &FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[0][3]);

                    CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][1], &BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][2], &BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][3], &BTestSuck.Suck[1][3]);
                }
            }
            else if(TestIF_File.bNS7000kit==true || TestIF_File.bNS7000CS==true)
            {
                CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);   //Aa --> 1
                CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[0][1]);   //Ab --> 2
                CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][2]);   //Ac --> 3
                CopySuck(&FTestSuckBackup.Suck[1][1], &FTestSuck.Suck[0][3]);   //Ad --> 4

                CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[1][0]);
                CopySuck(&FTestSuckBackup.Suck[1][2], &FTestSuck.Suck[1][1]);
                CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[1][2]);
                CopySuck(&FTestSuckBackup.Suck[1][3], &FTestSuck.Suck[1][3]);

                CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[0][1]);
                CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][2]);
                CopySuck(&BTestSuckBackup.Suck[1][1], &BTestSuck.Suck[0][3]);

                CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[1][0]);
                CopySuck(&BTestSuckBackup.Suck[1][2], &BTestSuck.Suck[1][1]);
                CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[1][2]);
                CopySuck(&BTestSuckBackup.Suck[1][3], &BTestSuck.Suck[1][3]);
            }
            else
            {
                CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);   //Aa --> 1
                CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][1]);   //Ab --> 3
                CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][2]);   //Ac --> 5
                CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[0][3]);   //Ad --> 7

                CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[1][0]);
                CopySuck(&FTestSuckBackup.Suck[1][1], &FTestSuck.Suck[1][1]);
                CopySuck(&FTestSuckBackup.Suck[1][2], &FTestSuck.Suck[1][2]);
                CopySuck(&FTestSuckBackup.Suck[1][3], &FTestSuck.Suck[1][3]);

                CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][1]);
                CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][2]);
                CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[0][3]);

                CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[1][0]);
                CopySuck(&BTestSuckBackup.Suck[1][1], &BTestSuck.Suck[1][1]);
                CopySuck(&BTestSuckBackup.Suck[1][2], &BTestSuck.Suck[1][2]);
                CopySuck(&BTestSuckBackup.Suck[1][3], &BTestSuck.Suck[1][3]);
            }
        }
    }
    else
    {
        if(TestIF_File.iTestMode==QualSite2X2 ||
           TestIF_File.iTestMode==DualSite2x1 ||
           TestIF_File.iTestMode==QualSite2X2N)
        {
            //Alick 20161011 (Steven) : TTL支援8Site
            //==>
            if(CUSTOMER_CODE==CC_JCET &&
               (TTLCfg.iCateBitLength==_8Bit || TTLCfg.iCateBitLength==_10Bit ||
                TTLCfg.iCateBitLength==_10BitPE || TTLCfg.iCateBitLength==_10BitPO))
            {
                //Site1
                iCH=TestIF_File.iSiteMap[0][0]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart0]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut0]);
                    for(int k=0; k<10; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit0+k]);
                }
                //Site2
                iCH=TestIF_File.iSiteMap[0][1]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart1]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut1]);
                    for(int k=0; k<10; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit10+k]);
                }

                if(CosFunction.bTTLCanUse8Site)
                {
                    //Site3
                    iCH=TestIF_File.iSiteMap[1][0]-1;
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart2]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut2]);
                        for(int k=0; k<10; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit20+k]);
                    }
                    //Site4
                    iCH=TestIF_File.iSiteMap[1][1]-1;
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart3]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut3]);
                        for(int k=0; k<10; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*10+k], &Sen[SenBit30+k]);
                    }
                }
            }
            else
            //<==
            //Alick 20161011 (Steven) : TTL支援8Site
            {
                //Site1
                iCH=TestIF_File.iSiteMap[0][0]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart0]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut0]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit0+k]);
                }
                //Site2
                iCH=TestIF_File.iSiteMap[0][1]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart1]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut1]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit5+k]);
                }
                //Site3
                iCH=TestIF_File.iSiteMap[1][0]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart2]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut2]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit10+k]);
                }
                //Site4
                iCH=TestIF_File.iSiteMap[1][1]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart3]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut3]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit15+k]);
                }
            }
            //jou 980312

            if(TestIF_File.iTestMode==QualSite2X2N)
            {
                if(TestIF_File.bNS7000kit==false && TestIF_File.bUse1x3SiteKit) //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][1]);

                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][1]);
                }
                else if(TestIF_File.bNS7000kit==false)
                {
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][6],&FTestSuck.Suck[0][1]);

                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][6],&BTestSuck.Suck[0][1]);

                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][6]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][6]);
                }
                else
                {
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);

                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                }
            }
            else if(TestIF_File.iTestMode==QualSite2X2 &&
                    TestIF_File.bSquare_OctalKit)                               //Steven 20141224 : 2x2Site使用8Site Kit
            {
                CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][2]);    //1 --> Ac
                CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][0]);    //3 --> Aa
                CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);    //5 --> Ab
                CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][3]);    //7 --> Ad
                CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[1][2]);    //2 --> Bc
                CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][0]);    //4 --> Ba
                CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][1]);    //6 --> Bb
                CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);    //8 --> Bd

                CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][2]);
                CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][0]);
                CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][3]);
                CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[1][2]);
                CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][0]);
                CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][1]);
                CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
            }
            else if(TestIF_File.iTestMode==QualSite2X2 &&
                    TestIF_File.b2x2Use16SiteKit)                               //Steven 20191113 : 2x2Site使用16Site Kit
            {
                CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][2]);    //1 -->
                CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][3]);    //3 -->
                CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][0]);    //5 --> Aa
                CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][4]);    //7 -->
                CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][1]);    //1 --> Ab
                CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][5]);    //3 -->
                CopySuck(&FTestSuckBackup.Suck[0][6],&FTestSuck.Suck[0][6]);    //5 -->
                CopySuck(&FTestSuckBackup.Suck[0][7],&FTestSuck.Suck[0][7]);    //7 -->
                CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[1][2]);    //2 -->
                CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][3]);    //4 -->
                CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][0]);    //6 --> Ba
                CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][4]);    //8 -->
                CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][1]);    //2 --> Bb
                CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][5]);    //4 -->
                CopySuck(&FTestSuckBackup.Suck[1][6],&FTestSuck.Suck[1][6]);    //6 -->
                CopySuck(&FTestSuckBackup.Suck[1][7],&FTestSuck.Suck[1][7]);    //8 -->

                CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][2]);    //1 -->
                CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][3]);    //3 -->
                CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][0]);    //5 --> Aa
                CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][4]);    //7 -->
                CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][1]);    //1 --> Ab
                CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][5]);    //3 -->
                CopySuck(&BTestSuckBackup.Suck[0][6],&BTestSuck.Suck[0][6]);    //5 -->
                CopySuck(&BTestSuckBackup.Suck[0][7],&BTestSuck.Suck[0][7]);    //7 -->
                CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[1][2]);    //2 -->
                CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][3]);    //4 -->
                CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][0]);    //6 --> Ba
                CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][4]);    //8 -->
                CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][1]);    //2 --> Bb
                CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][5]);    //4 -->
                CopySuck(&BTestSuckBackup.Suck[1][6],&BTestSuck.Suck[1][6]);    //6 -->
                CopySuck(&BTestSuckBackup.Suck[1][7],&BTestSuck.Suck[1][7]);    //8 -->
            }
            else if(USE_46_SUCKER_DB==1)  //Steven 20131008 : HT9045使用46配氣      //Steven 20140312 : For HT9045WA
            {
                if((TestIF_File.iTestMode==QualSite2X2 ||
                    TestIF_File.iTestMode==DualSite2x1) &&
                   (TestIF_File.bNS7000CS ||                                    //Steven 20100119 : Use NS7000 Change Socket
                    TestIF_File.bHontechLayoutKit2x2))                          //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
                {

                }
                else
                {
                    //Steven 20120711 : 修正HT9046使用Hontech浮動頭生產2x2的真空對應位置
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);    //1 --> Aa
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);    //5 --> Ab
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][2]);    //3 --> Ba
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[0][3]);    //7 --> Bb
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
                }
            }
            else if(MachineTypeChoice==Type_HT9045_12Site)                      //ChungHung 20130507 add HT9045 updata for 12site 517
            {
                if((TestIF_File.iTestMode==QualSite2X2 &&
                    (TestIF_File.bNS7000CS ||                                   //kevin 20121009  20100119 : Use NS7000 Change Socket
                     TestIF_File.bHontechLayoutKit2x2)) ||                      //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
                   TestIF_File.iTestMode==DualSite2x1)
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);    // 1 --> Aa
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);    // 3 --> Ba
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);    // 5 --> Ab
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[1][1]);    // 7 --> Bb
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
                }
                else
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);    // 1 --> Aa
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][2]);    // 5 --> Ab
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][1]);    // 2 --> Ba
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);    // 6 --> Bb
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
                }
            }
            else if(MachineTypeChoice==Type_HT9045)                             //9045
            {
                if((TestIF_File.iTestMode==QualSite2X2 && (TestIF_File.bNS7000CS || TestIF.bHontechLayoutKit2x2)) || TestIF_File.iTestMode==DualSite2x1) //kevin 20121009  20100119 : Use NS7000 Change Socket  //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);    // 1 --> Aa
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);    // 2 --> Ab
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][2]);    // 3 --> Ba
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);    // 4 --> Bb
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
                }
                else
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);    // 1 --> Aa
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][2]);    // 5 --> Ab
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][1]);    // 2 --> Ba
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);    // 6 --> Bb
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
                }
            }
            else //9046
            {
                if((TestIF_File.iTestMode==QualSite2X2 || TestIF_File.iTestMode==DualSite2x1) && (TestIF_File.bNS7000CS || TestIF_File.bHontechLayoutKit2x2))    //Steven 20100119 : Use NS7000 Change Socket   //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
                {

                }
                else
                {
                    //Steven 20120711 : 修正HT9046使用Hontech浮動頭生產2x2的真空對應位置
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);    //1 --> Aa
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);    //3 --> Ab
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][2]);    //5 --> Ba
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[0][3]);    //7 --> Bb
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
                }
            }
        }
        else
        {
            if(TestIF_File.iTestMode==_6Site2X3)                                //ChungHung 20140115 add for 2x3_6
            {
                //Alick 20161011 (Steven) : TTL支援8Site
                //==>
                //Site1
                iCH=TestIF_File.iSiteMap[0][0]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart0]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut0]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit0+k]);
                }
                //Site2
                iCH=TestIF_File.iSiteMap[0][1]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart1]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut1]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit5+k]);
                }
                //Site3
                iCH=TestIF_File.iSiteMap[0][2]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart2]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut2]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit10+k]);
                }
                //Site4
                iCH=TestIF_File.iSiteMap[1][0]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart4]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut4]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit20+k]);
                }
                //Site5
                iCH=TestIF_File.iSiteMap[1][1]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart5]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut5]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit25+k]);
                }
                //Site6
                iCH=TestIF_File.iSiteMap[1][2]-1;
                if(iCH>=0 && iCH<16)
                {
                    CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart6]);
                    CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut6]);
                    for(int k=0; k<5; k++)
                        CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit30+k]);
                }
                //<==
                //Alick 20161011 (Steven) : TTL支援8Site

                if(USE_46_SUCKER_DB==1)                                         //Steven 20131008 : HT9045使用46配氣
                {

                }
                else if(MachineTypeChoice==Type_HT9045 ||
                        MachineTypeChoice==Type_HT9045_12Site)                  //9045
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);
                }
            }
            else if(TestIF_File.iTestMode==_8Site2X4 ||                         //2x4
                    TestIF_File.iTestMode==_16Site4X4)                          //Sam 20190226 : 16Site4X4
            {
                //Alick 20161011 (Steven) : TTL支援8Site
                //==>
                //Site1
                if(TestIF_File.iTestMode==_8Site2X4)
                {
                    iCH=TestIF_File.iSiteMap[0][0]-1;
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart0]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut0]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit0+k]);
                    }
                    //Site2
                    iCH=TestIF_File.iSiteMap[0][1]-1;
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart1]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut1]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit5+k]);
                    }
                    //Site3
                    iCH=TestIF_File.iSiteMap[0][2]-1;
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart2]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut2]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit10+k]);
                    }
                    //Site4
                    iCH=TestIF_File.iSiteMap[0][3]-1;
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart3]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut3]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit15+k]);
                    }
                    //Site5
                    iCH=TestIF_File.iSiteMap[1][0]-1;                           //Alick 20161206 modify [0][0]=>[1][0]
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart4]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut4]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit20+k]);
                    }
                    //Site6
                    iCH=TestIF_File.iSiteMap[1][1]-1;                           //Alick 20161206 modify [0][1]=>[1][1]
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart5]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut5]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit25+k]);
                    }
                    //Site7
                    iCH=TestIF_File.iSiteMap[1][2]-1;                           //Alick 20161206 modify [0][2]=>[1][2]
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart6]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut6]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit30+k]);
                    }
                    //Site8
                    iCH=TestIF_File.iSiteMap[1][3]-1;                           //Alick 20161206 modify [0][3]=>[1][3]
                    if(iCH>=0 && iCH<16)
                    {
                        CopySwitch(&SWBackup[BackSwStart0+iCH], &SW[SwStart7]);
                        CopySwitch(&SWBackup[BackSwDut0+iCH], &SW[SwDut7]);
                        for(int k=0; k<5; k++)
                            CopySensor(&SenBackUp[BackSenBit0+iCH*5+k], &Sen[SenBit35+k]);
                    }
                }
                //<==
                //Alick 20161011 (Steven) : TTL支援8Site
                if(TestIF_File.iTestMode==_16Site4X4 &&
                   TestIF_File.bOctal_16Kit==true)
                {
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][3]);

                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][4]);    //Steven 20150107 : 修正後四Site不會吸的問題
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][6],&FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][7],&FTestSuck.Suck[0][7]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[1][4]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][6],&FTestSuck.Suck[1][6]);
                    CopySuck(&FTestSuckBackup.Suck[1][7],&FTestSuck.Suck[1][7]);

                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][4]);    //Steven 20150107 : 修正後四Site不會吸的問題
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][6],&BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][7],&BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[1][4]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][6],&BTestSuck.Suck[1][6]);
                    CopySuck(&BTestSuckBackup.Suck[1][7],&BTestSuck.Suck[1][7]);
                }
                else if(TestIF_File.bOctal_12Kit &&                             //ChungHung 20140508 add for SCK
                        (MachineTypeChoice==Type_HT9046    ||
                         MachineTypeChoice==Type_HT9046_LS))
                {
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][4]);

                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][4]);

                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][4]);

                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][4]);
                }
                else if(TestIF_File.bOctal_16Kit &&
                        TestIF_File.dSiteXPitch<=40 &&                          //JerryYang 20220923 : 8 site模式支援16 site SLK
                       (MachineTypeChoice==Type_HT9046 ||
                        MachineTypeChoice==Type_HT9046_LS))
                {
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][3]);

                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][4]);//Steven 20150107 : 修正後四Site不會吸的問題
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][6],&FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][7],&FTestSuck.Suck[0][7]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[1][4]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][6],&FTestSuck.Suck[1][6]);
                    CopySuck(&FTestSuckBackup.Suck[1][7],&FTestSuck.Suck[1][7]);

                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][4]);    //Steven 20150107 : 修正後四Site不會吸的問題
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][6],&BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][7],&BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[1][4]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][6],&BTestSuck.Suck[1][6]);
                    CopySuck(&BTestSuckBackup.Suck[1][7],&BTestSuck.Suck[1][7]);
                }
                else if(TestIF_File.bOctal_16Kit &&
                       (MachineTypeChoice==Type_HT9046 ||
                        MachineTypeChoice==Type_HT9046_LS))                     //Steven 20140312 : 8Site使用16Site Kit
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][6],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][6],&FTestSuck.Suck[1][3]);

                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][4]);//Steven 20150107 : 修正後四Site不會吸的問題
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][7],&FTestSuck.Suck[0][7]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][4]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][6]);
                    CopySuck(&FTestSuckBackup.Suck[1][7],&FTestSuck.Suck[1][7]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][6],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][6],&BTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][4]);//Steven 20150107 : 修正後四Site不會吸的問題
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][7],&BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][4]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][6]);
                    CopySuck(&BTestSuckBackup.Suck[1][7],&BTestSuck.Suck[1][7]);
                }
                //ChungHung 20130705 chage mapping
                else if((MachineTypeChoice==Type_HT9046      ||
                         MachineTypeChoice==Type_HT9046_LS   ||                 //2013-03-05    Dell    for ATK pitch 80mm kit
                         USE_46_SUCKER_DB==1               ) &&                 //Steven 20131008 : HT9045使用46配氣
                         TestIF_File.bOctal_80Kit)
                {
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][7],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][7],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][7],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][7],&BTestSuck.Suck[1][3]);
                }
                else if(TestIF_File.bOctal_12Kit &&
                        MachineTypeChoice==Type_HT9045_12Site)                  //ChungHung 20140508 add for SCK
                {                                                               //no testting because no handler
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][3]);
                }
                else if(USE_46_SUCKER_DB==1)                                    //Steven 20131008 : HT9045使用46配氣
                {

                }
                else if(MachineTypeChoice==Type_HT9045 ||
                        MachineTypeChoice==Type_HT9045_12Site)                  //9045
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][1], &FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2], &FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][3], &FTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1], &BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2], &BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3], &BTestSuck.Suck[1][3]);
                }
            }
            else if(TestIF_File.iTestMode==_10Site2X5)                          //2x5     //wei 20190614 10 site
            {
                if(MachineTypeChoice==Type_HT9045_12Site &&                     //ChungHung 20130705 chage mapping
                   USE_46_SUCKER_DB!=1)                                         //Steven 20140311 : For HT9045WA
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[1][1]);

                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][4]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[1][4]);

                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][5]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[1][1]);

                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][4]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[1][4]);

                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][5]);
                }
                else
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)                  //ChungHung 20130507 add HT9045 updata for 12site 517 Index 吸嘴對應
                        {
                            CopySuck(&FTestSuckBackup.Suck[i][j],&FTestSuck.Suck[i][j]);
                            CopySuck(&BTestSuckBackup.Suck[i][j],&BTestSuck.Suck[i][j]);
                        }
                    }
                }
            }
            else if(TestIF_File.iTestMode==_12Site2X6)                          //2x6
            {
                if(MachineTypeChoice==Type_HT9045_12Site &&                     //ChungHung 20130705 chage mapping
                   USE_46_SUCKER_DB!=1)                                         //Steven 20140311 : For HT9045WA
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[1][1]);

                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][4]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[1][4]);

                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][3]);

                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][5]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[1][1]);

                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][4]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[1][4]);

                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][3]);

                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][5]);
                }
                else if(TestIF_File.b2x6Use2x8SitSLK)                           //Steven 20240807 : 12Site使用16Site Kit
                {
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][4]);
                    CopySuck(&FTestSuckBackup.Suck[0][6],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][7],&FTestSuck.Suck[0][7]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[1][6]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][4]);
                    CopySuck(&FTestSuckBackup.Suck[1][6],&FTestSuck.Suck[1][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][7],&FTestSuck.Suck[1][7]);

                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][4]);
                    CopySuck(&BTestSuckBackup.Suck[0][6],&BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][7],&BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[1][6]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][4]);
                    CopySuck(&BTestSuckBackup.Suck[1][6],&BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][7],&BTestSuck.Suck[1][7]);
                }
                else
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)                  //ChungHung 20130507 add HT9045 updata for 12site 517 Index 吸嘴對應
                        {
                            CopySuck(&FTestSuckBackup.Suck[i][j],&FTestSuck.Suck[i][j]);
                            CopySuck(&BTestSuckBackup.Suck[i][j],&BTestSuck.Suck[i][j]);
                        }
                    }
                }
            }
            else if(TestIF_File.iTestMode==_16Site2X8)                          //2x8 Eliot 2009_12_28
            {
                if(MachineTypeChoice==Type_HT1032 &&
                   TestIF_File.b6CableLayoutKit)                                //Steven 20230613 : ATC6.1 16Site SLK
                {
                    CopySuck(&FTestSuckBackup.Suck[0][4], &FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][0], &FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][5], &FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][1], &FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[0][2], &FTestSuck.Suck[0][4]);
                    CopySuck(&FTestSuckBackup.Suck[0][6], &FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][3], &FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][7], &FTestSuck.Suck[0][7]);
                    CopySuck(&FTestSuckBackup.Suck[1][4], &FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][0], &FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][5], &FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][1], &FTestSuck.Suck[1][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][2], &FTestSuck.Suck[1][4]);
                    CopySuck(&FTestSuckBackup.Suck[1][6], &FTestSuck.Suck[1][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][3], &FTestSuck.Suck[1][6]);
                    CopySuck(&FTestSuckBackup.Suck[1][7], &FTestSuck.Suck[1][7]);

                    CopySuck(&BTestSuckBackup.Suck[0][4], &BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][0], &BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][5], &BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][1], &BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[0][2], &BTestSuck.Suck[0][4]);
                    CopySuck(&BTestSuckBackup.Suck[0][6], &BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][3], &BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][7], &BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[1][4], &BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][0], &BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][5], &BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1], &BTestSuck.Suck[1][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][2], &BTestSuck.Suck[1][4]);
                    CopySuck(&BTestSuckBackup.Suck[1][6], &BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][3], &BTestSuck.Suck[1][6]);
                    CopySuck(&BTestSuckBackup.Suck[1][7], &BTestSuck.Suck[1][7]);
                }
                else
                {
                    for(int i=0; i<MAX_Index_Row; i++)                          //Steven 20120906 : 修正其他模式切換到16Site會Error
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            CopySuck(&FTestSuckBackup.Suck[i][j],&FTestSuck.Suck[i][j]);
                            CopySuck(&BTestSuckBackup.Suck[i][j],&BTestSuck.Suck[i][j]);
                        }
                    }
                }
            }
            else if(TestIF_File.iTestMode==_32Site4X8N)                         //2x8 Eliot 2009_12_28
            {
                if(MachineTypeChoice==Type_HT1032 &&
                   TestIF_File.b6CableLayoutKit)                                //Steven 20230613 : ATC6.1 16Site SLK
                {
                    CopySuck(&FTestSuckBackup.Suck[0][4],&FTestSuck.Suck[0][0]);
                    CopySuck(&FTestSuckBackup.Suck[0][0],&FTestSuck.Suck[0][1]);
                    CopySuck(&FTestSuckBackup.Suck[0][5],&FTestSuck.Suck[0][2]);
                    CopySuck(&FTestSuckBackup.Suck[0][1],&FTestSuck.Suck[0][3]);
                    CopySuck(&FTestSuckBackup.Suck[0][2],&FTestSuck.Suck[0][4]);
                    CopySuck(&FTestSuckBackup.Suck[0][6],&FTestSuck.Suck[0][5]);
                    CopySuck(&FTestSuckBackup.Suck[0][3],&FTestSuck.Suck[0][6]);
                    CopySuck(&FTestSuckBackup.Suck[0][7],&FTestSuck.Suck[0][7]);
                    CopySuck(&FTestSuckBackup.Suck[1][4],&FTestSuck.Suck[1][0]);
                    CopySuck(&FTestSuckBackup.Suck[1][0],&FTestSuck.Suck[1][1]);
                    CopySuck(&FTestSuckBackup.Suck[1][5],&FTestSuck.Suck[1][2]);
                    CopySuck(&FTestSuckBackup.Suck[1][1],&FTestSuck.Suck[1][3]);
                    CopySuck(&FTestSuckBackup.Suck[1][2],&FTestSuck.Suck[1][4]);
                    CopySuck(&FTestSuckBackup.Suck[1][6],&FTestSuck.Suck[1][5]);
                    CopySuck(&FTestSuckBackup.Suck[1][3],&FTestSuck.Suck[1][6]);
                    CopySuck(&FTestSuckBackup.Suck[1][7],&FTestSuck.Suck[1][7]);

                    CopySuck(&BTestSuckBackup.Suck[0][4],&BTestSuck.Suck[0][0]);
                    CopySuck(&BTestSuckBackup.Suck[0][0],&BTestSuck.Suck[0][1]);
                    CopySuck(&BTestSuckBackup.Suck[0][5],&BTestSuck.Suck[0][2]);
                    CopySuck(&BTestSuckBackup.Suck[0][1],&BTestSuck.Suck[0][3]);
                    CopySuck(&BTestSuckBackup.Suck[0][2],&BTestSuck.Suck[0][4]);
                    CopySuck(&BTestSuckBackup.Suck[0][6],&BTestSuck.Suck[0][5]);
                    CopySuck(&BTestSuckBackup.Suck[0][3],&BTestSuck.Suck[0][6]);
                    CopySuck(&BTestSuckBackup.Suck[0][7],&BTestSuck.Suck[0][7]);
                    CopySuck(&BTestSuckBackup.Suck[1][4],&BTestSuck.Suck[1][0]);
                    CopySuck(&BTestSuckBackup.Suck[1][0],&BTestSuck.Suck[1][1]);
                    CopySuck(&BTestSuckBackup.Suck[1][5],&BTestSuck.Suck[1][2]);
                    CopySuck(&BTestSuckBackup.Suck[1][1],&BTestSuck.Suck[1][3]);
                    CopySuck(&BTestSuckBackup.Suck[1][2],&BTestSuck.Suck[1][4]);
                    CopySuck(&BTestSuckBackup.Suck[1][6],&BTestSuck.Suck[1][5]);
                    CopySuck(&BTestSuckBackup.Suck[1][3],&BTestSuck.Suck[1][6]);
                    CopySuck(&BTestSuckBackup.Suck[1][7],&BTestSuck.Suck[1][7]);
                }
                else
                {
                    for(int i=0; i<MAX_Index_Row; i++)                          //Steven 20120906 : 修正其他模式切換到16Site會Error
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            CopySuck(&FTestSuckBackup.Suck[i][j],&FTestSuck.Suck[i][j]);
                            CopySuck(&BTestSuckBackup.Suck[i][j],&BTestSuck.Suck[i][j]);
                        }
                    }
                }
            }
        }
    }

    if(MachineTypeChoice==Type_HT9045        ||                                 //9045
       MachineTypeChoice==Type_HT9045_12Site ||                                 //ChungHung 20130705 chage mapping
       USE_46_SUCKER_DB==1)                                                     //Steven 20131008 : HT9045使用46配氣
    {                                                                           //jou 980916 start : add index suck mode change need Switch on/off  //Steven 20090916 Start
        if((TestIF_File.iTestMode==DualSite &&                                  //1x2
            TestIF_File.bNS7000kit==false) ||                                   //wei 20150519 TSMC 120
           TestIF_File.iTestMode==SingleSite)                                   //&& TestIF_File.dSiteXPitch==80 //wei 20150125
        {
            if(TestIF_File.b1x2Use1x4SiteKit==true ||                           //Isaac 20200311 : 1x2Site使用1x4Site Kit
              (CUSTOMER_CODE==CC_SCS &&                                         //jou 20240220 : JSCS single site ns bias SLK 真空異常
               TestIF_File.iTestMode==SingleSite &&
               TestIF_File.bNS7000kit==true))
            {
                SW[SwZ1SuckMode0].Off();
                SW[SwZ1SuckMode1].Off();
                SW[SwZ2SuckMode0].Off();
                SW[SwZ2SuckMode1].Off();
            }
            else
            {
                SW[SwZ1SuckMode0].On();
                SW[SwZ1SuckMode1].On();
                SW[SwZ2SuckMode0].On();
                SW[SwZ2SuckMode1].On();
            }
        }
        else
        {
            SW[SwZ1SuckMode0].Off();
            SW[SwZ1SuckMode1].Off();
            SW[SwZ2SuckMode0].Off();
            SW[SwZ2SuckMode1].Off();
        }
    }
}
//------------------------------------------------------------------------------
bool SetWorkParameter()
{
    ChangeSite();
    ReadTechData();
    DoStructUnitConvert();
    if(bDoBRTCGiveWayCheck==false)                                              //Ifor 20191121 RTC 讓位時不重新讀取位置避免發生異常
    {
        SetTechDataToProd();
        DoSetupSystemToProd();
    }
    SetSimuScreenPara();

    if(IniConfig.bHaveRotateShuttle==true && TestIF_File.bRotateShuttle)        //Steven 20110801 : 轉轉蝦頭要檢查有沒有轉頭
    {
        //In Shuttle向右移動時,用第1個位置檢查------------------------
        if(InArmSuck.iShtCol==1)
        {
            SThreadPara.iInShRotateCheck[0]=Prod.iInSHSen7DetectPos1x1[0];
            SThreadPara.iInShRotateCheck[1]=Prod.iInSHSen7DetectPos1x1[1];
        }
        else if(InArmSuck.iShtCol==2)
        {
            SThreadPara.iInShRotateCheck[0]=Prod.iInSHSen7DetectPos1x2[0][0];
            SThreadPara.iInShRotateCheck[1]=Prod.iInSHSen7DetectPos1x2[1][0];
        }
        else if(InArmSuck.iShtCol==3)                                           //ChungHung 20140115 add for 2x3_6
        {
            SThreadPara.iInShRotateCheck[0]=Prod.iInSHSen7DetectPos2x3[0][0];
            SThreadPara.iInShRotateCheck[1]=Prod.iInSHSen7DetectPos2x3[1][0];
        }
        else if(InArmSuck.iShtCol==4)
        {
            SThreadPara.iInShRotateCheck[0]=Prod.iInSHSen7DetectPos1x4[0][0];
            SThreadPara.iInShRotateCheck[1]=Prod.iInSHSen7DetectPos1x4[1][0];
        }
        else if(InArmSuck.iShtCol==5)                                           //Steven 20221107 : Add for 2x5
        {
            SThreadPara.iInShRotateCheck[0]=Prod.iInSHSen9DetectPos2x5[0][0];
            SThreadPara.iInShRotateCheck[1]=Prod.iInSHSen9DetectPos2x5[1][0];
        }
        else if(InArmSuck.iShtCol==6)                                           //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            SThreadPara.iInShRotateCheck[0]=Prod.iInSHSen9DetectPos2x6[0][0];
            SThreadPara.iInShRotateCheck[1]=Prod.iInSHSen9DetectPos2x6[1][0];
        }
        else if(InArmSuck.iShtCol==8)
        {
            SThreadPara.iInShRotateCheck[0]=Prod.iInSHSen9DetectPos2x8[0][0];
            SThreadPara.iInShRotateCheck[1]=Prod.iInSHSen9DetectPos2x8[1][0];
        }
        else
        {
            ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "SetWorkParameter");
        }

        //Out Shuttle向左移動時,檢查最後一個位置------------------------
        SThreadPara.iOShRotateToLeftCheck[0]=GetSHCHKPos(InArmSuck.iShtCol-1, Tech.OutSH1ZOneRowDetectPos+2000);
        SThreadPara.iOShRotateToLeftCheck[1]=GetSHCHKPos(InArmSuck.iShtCol-1, Tech.OutSH2ZOneRowDetectPos+2000);
        //Out Shuttle向右移動時,檢查第一個位置--------------------------
        SThreadPara.iOShRotateToRightCheck[0]=GetSHCHKPos(0, Tech.OutSH1ZOneRowDetectPos+2000);
        SThreadPara.iOShRotateToRightCheck[1]=GetSHCHKPos(0, Tech.OutSH2ZOneRowDetectPos+2000);
        //向右移動時，先檢查In Shuttle或Out
        SThreadPara.bChechInShFirst[0]=(SThreadPara.iInShRotateCheck[0]<SThreadPara.iOShRotateToRightCheck[0]);
        SThreadPara.bChechInShFirst[1]=(SThreadPara.iInShRotateCheck[1]<SThreadPara.iOShRotateToRightCheck[1]);
    }

    if(IN_SHT_LAST_SENSOR==1)                                                   //Steven 20181203 : In Shuttle最後一個Sensor定義
    {
        SThreadPara.iInShuttleSen7[0]=SnInPutSHT1S9;
        SThreadPara.iInShuttleSen7[1]=SnInPutSHT2S9;
    }
    else
    {
        //ChungHung 20130507 add HT9045 updata for 12site 517
        SThreadPara.iInShuttleSen7[0]=(MachineTypeChoice==Type_HT9045 || MachineTypeChoice==Type_HT9045_12Site)?SnInPutSHT1S7:SnInPutSHT1S9;
        SThreadPara.iInShuttleSen7[1]=(MachineTypeChoice==Type_HT9045 || MachineTypeChoice==Type_HT9045_12Site)?SnInPutSHT2S7:SnInPutSHT2S9;
    }

    if(fMain!=NULL)
        fMain->ShowFunctions();                                                 //Steven 20240123 : 顯示功能列表

    int mIndex[15]={MMTrayY, MMAuto1, MMAuto2, MMAuto3, MMAuto4, MMAuto5, MMAuto6,
                    MManualTray1, MManualTray2, MManualTray3,
                    MManualTray4, MManualTray5, MManualTray6, MMPlate1, MMPlate2};
    for(int i=0; i<15; i++)                                                     //Steven 20140516 : 移到下面,避免參數沒有Init
        if(MOT[mIndex[i]].Tray.XItem<=0 || MOT[mIndex[i]].Tray.YItem<=0)
            return false;

    return true;
}
//------------------------------------------------------------------------------
bool ReadTechData()
{
    bool ret=true;
//    ret=ReadData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));

    if(fTeach!=NULL)
        fTeach->ReadFile();

    //Steven 20120809 : 吸嘴自動校正
    Tech.iInArmPickX =(Tech.iInArmPickX ==0)?Tech.iInArmLoadStageX      :Tech.iInArmPickX;
    Tech.iInArmPickY =(Tech.iInArmPickY ==0)?Tech.iInArmLoadStageY-2000 :Tech.iInArmPickY;
    Tech.iOutArmPickX=(Tech.iOutArmPickX==0)?Tech.iOutArmAuto1X         :Tech.iOutArmPickX;
    Tech.iOutArmPickY=(Tech.iOutArmPickY==0)?Tech.iOutArmAuto1Y-2000    :Tech.iOutArmPickY;
    CHECK_RANGE=CheckAndReadIniDataGeneral("Shuttle",   "CHECK_RANGE"     , 500);   //Steven 20160108 : 改去Teaching調整

    if(CosFunction.bOutShuttleSensorCanNotDisable)                              //Steven 20151202 : Out Shuttle Sensor不能關閉檢查
    {
        if(Tech.OutSH1ZDetectPos==0)
            Tech.OutSH1ZDetectPos=20599;
        if(Tech.OutSH2ZDetectPos==0)
            Tech.OutSH2ZDetectPos=20469;
        if(Tech.OutSH1ZOneRowDetectPos==0)
            Tech.OutSH1ZOneRowDetectPos=20565;
        if(Tech.OutSH2ZOneRowDetectPos==0)
            Tech.OutSH2ZOneRowDetectPos=20453;
    }

    return ret;
}
//------------------------------------------------------------------------------
bool CompareTechData()                                                          //Steven 20220221 : 針對Teaching值做防呆
{
    AnsiString Str;
    if(Tech.iInArmPlate2Y-10000<Tech.iInArmPlate1Y)
    {
        Str.sprintf("HP1=%d, HP2=%d", Tech.iInArmPlate1Y, Tech.iInArmPlate2Y);
        ShowMyMessage("The teaching of hot plate is mistake, please verify!", "加熱盤的點位有誤, 請重新確認", Str);
        return false;
    }

    if(Tech.iInArmShuttle1Y+10000>Tech.iInArmShuttle2Y)
    {
        Str.sprintf("InSht1=%d, InSht2=%d", Tech.iInArmShuttle1Y, Tech.iInArmShuttle2Y);
        ShowMyMessage("The teaching of in shuttle is mistake, please verify!", "In Shuttle的點位有誤, 請重新確認", Str);
        return false;
    }

    if(Tech.iOutArmShuttle1Y+10000>Tech.iOutArmShuttle2Y)
    {
        Str.sprintf("OutSht1=%d, OutSht2=%d", Tech.iOutArmShuttle1Y, Tech.iOutArmShuttle2Y);
        ShowMyMessage("The teaching of out shuttle is mistake, please verify!", "Out Shuttle的點位有誤, 請重新確認", Str);
        return false;
    }

    if(Tech.iOutArmFix1X+10000>Tech.iOutArmFix2X)
    {
        Str.sprintf("Fix1=%d, Fix2=%d", Tech.iOutArmFix1X, Tech.iOutArmFix2X);
        ShowMyMessage("The teaching of fix tray is mistake, please verify!", "Fix盤的點位有誤, 請重新確認", Str);
        return false;
    }

    if(FIX3_FULL_PLACE==Fix3K_Uninstall && (Tech.iOutArmFix2X+10000>Tech.iOutArmFix3X)) //JerryYang 20230608 : Unloader FIX區為固定式時，按START需檢查FIX位置是否在合理範圍
    {
        Str.sprintf("Fix2=%d, Fix3=%d", Tech.iOutArmFix2X, Tech.iOutArmFix3X);
        ShowMyMessage("The teaching of fix tray is mistake, please verify!", "Fix盤的點位有誤, 請重新確認", Str);
        return false;
    }

    if(Tech.iOutArmAuto1X+10000>Tech.iOutArmAuto2X)
    {
        Str.sprintf("Auto1=%d, Auto2=%d", Tech.iOutArmAuto1X, Tech.iOutArmAuto2X);
        ShowMyMessage("The teaching of unloader tray is mistake, please verify!", "出料盤的點位有誤, 請重新確認", Str);
        return false;
    }

    if(Tech.iOutArmAuto2X+10000>Tech.iOutArmAuto3X)
    {
        Str.sprintf("Auto2=%d, Auto3=%d", Tech.iOutArmAuto2X, Tech.iOutArmAuto3X);
        ShowMyMessage("The teaching of unloader tray is mistake, please verify!", "出料盤的點位有誤, 請重新確認", Str);
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void SetSuckRetryCount()
{
    int i, j;
    for(i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(j=0; j<InArmSuck.iMaxCol; j++)
        {
            InArmSuck.Suck[i][j].SetRetryCount(ArmSpeed[InArm].iRetryCT);
            OutArmSuck.Suck[i][j].SetRetryCount(ArmSpeed[OutArm].iRetryCT);
        }
    }

    for(i=0; i<MAX_Index_Row; i++)
    {
        for(j=0; j<MAX_Index_Col; j++)
        {
            FTestSuck.Suck[i][j].SetRetryCount(0);  //Steven 20110317 : 確保是0
            BTestSuck.Suck[i][j].SetRetryCount(0);
//            FTestSuck.Suck[i][j].SetRetryCount(ArmSpeed[IndexArm].iRetryCT);
//            BTestSuck.Suck[i][j].SetRetryCount(ArmSpeed[IndexArm].iRetryCT);
        }
    }

    CatchTraySuck.Suck[0][0].SetRetryCount(ArmSpeed[TrayArm].iRetryCT);
}
//-----------------------------------------------------------------------------
//void GetEachAxisSpeed()     //Steven 20120822 : 沒用到,馬克掉
//{
//    double a,b,c;
//    double f;
//    a=XShiftF;
//    b=Z1Up;
//    c=sqrt((a*a)+(b*b));
//    f=b*MOT[MTestY1].GailSpeed;
//    iTestZAxisSpeed=f/c;
//}
//-----------------------------------------------------------------------------
void GetIndexParm()
{
    if(bGali_CardInstall)
    {
        Z2Down2Speed=0;                                                         //因為會計算齒輪比, 所以裡面的參數都必須要重置! 不然會導致Hang Up
        Z1Down2Speed=0;

        ////20111114  Dell for Disable Index Arm    Start
        if(TestIF_File.iShuttleMode==0              ||                          //Normmal
           IniConfig.bIndexArm2SupplyLight==true    ||
           TestIF_File.bForEgisTecTest==true        ||                          //Steven 20140922 : Arm2當作指紋測試
           (IniConfig.bD58UseArm1PickPlaceArm2Test==true && TestIF_File.bArm1PickPlaceArm2Test==true))   //kevin 20150127 Arm1 下壓 arm2 測試    //Ifor 20200811 Fix: Arm1 Pick Place Arm2Test 需卡兩個條件
        {
            Z1Safe      =Prod.All_TestZ_Test_Safe-MOT[MTestZ1].Gali_ReadPos();
            XShiftF     =Prod.TestY1_Front-Prod.TestY1_Middle;                  //Y1_Shuttle1 To Y1_Index Pitch
            Z1Up        =Prod.TestZ1_Safe-Prod.All_TestZ_Test_Safe;
            Z2DownSafe  =Prod.All_TestZ_Test_Safe-Prod.TestZ1_Safe;

            Z2Down2Speed=5000;
            #ifdef DEBUG_GALIL
            Z2Down      =Prod.TestZ2_Test-Prod.All_TestZ_Test_Safe+Z2Down2Speed;
            #else
            Z2Down      =Prod.TestZ2_Test-Prod.All_TestZ_Test_Safe;
            #endif

            Z2Safe      =Prod.All_TestZ_Test_Safe-MOT[MTestZ2].Gali_ReadPos();
            XShiftR     =Prod.TestY2_Middle-Prod.TestY2_Rear;                   //Y2_Index To Y2_Shuttle2 Pitch
            Z2Up        =Prod.TestZ1_Safe-Prod.All_TestZ_Test_Safe;
            Z1DownSafe  =Prod.All_TestZ_Test_Safe-Prod.TestZ1_Safe;
            Z1Down2Speed=5000;
            #ifdef DEBUG_GALIL
            Z1Down      =Prod.TestZ1_Test-Prod.All_TestZ_Test_Safe+Z1Down2Speed;
            #else
            Z1Down      =Prod.TestZ1_Test-Prod.All_TestZ_Test_Safe;
            #endif

            AxisY_Pre_MovePos = Prod.TestY_Pre_MovePos;
            Z1DownToShuttle=Prod.TestZ1_Place-Prod.TestZ1_Safe;                 //Steven 20181228 : Add Index Action    //ChungHung 20171116 modify for Index Action
            Z2DownToShuttle=Prod.TestZ2_Place-Prod.TestZ1_Safe;                 //Steven 20181228 : Add Index Action    //ChungHung 20171116 modify for Index Action
        }
        else if(TestIF_File.iShuttleMode==1)
        {
            if(IniConfig.bShuttleMode50==true)                                  //20111212  Dell
            {
                Z1Safe      =Prod.All_TestZ_Test_Safe-MOT[MTestZ1].Gali_ReadPos();
                XShiftF     =Prod.TestY1_Front-Prod.TestY1_Middle;              //Y1_Shuttle1 To Y1_Index Pitch
                Z1Up        =Prod.TestZ1_Safe-Prod.All_TestZ_Test_Safe;
                Z2DownSafe  =Prod.All_TestZ_Test_Safe-Prod.TestZ1_Safe;
                Z2Down      =Prod.TestZ2_Test-Prod.All_TestZ_Test_Safe;

                Z2Safe      =Prod.All_TestZ_Test_Safe-MOT[MTestZ2].Gali_ReadPos();
                XShiftR     =Prod.TestY2_Middle-Prod.TestY2_Rear;               //Y2_Index To Y2_Shuttle2 Pitch
                Z2Up        =Prod.TestZ1_Safe-Prod.All_TestZ_Test_Safe;
                Z1DownSafe  =Prod.All_TestZ_Test_Safe-Prod.TestZ1_Safe;
                Z1Down      =Prod.TestZ1_Test-Prod.All_TestZ_Test_Safe;

                AxisY_Pre_MovePos = Prod.TestY_Pre_MovePos;
            }
            else if(TestIF_File.iShuttle_Sel==0)
            {
                Z1Safe      =Prod.All_TestZ_Test_Safe - MOT[MTestZ1].Gali_ReadPos();
                XShiftF     =Prod.TestY1_Front - Prod.TestY1_Middle;            //Y1_Shuttle1 To Y1_Index Pitch
                Z1Up        =Prod.TestZ1_Safe - Prod.All_TestZ_Test_Safe;
                Z2DownSafe  =0;
                Z2Down      =0;

                Z2Safe      =0;
                XShiftR     =0;                                                 //Y2_Index To Y2_Shuttle2 Pitch
                Z2Up        =0;
                Z1DownSafe  =Prod.All_TestZ_Test_Safe - Prod.TestZ1_Safe;
                Z1Down      =Prod.TestZ1_Test - Prod.All_TestZ_Test_Safe;

                AxisY_Pre_MovePos = Prod.TestY_Pre_MovePos;
            }
            else
            {
                Z1Safe      =0;
//                if(USE_INDEX_ARM_AXES==IndexArm_3_Axis) //JimmyChiu 20220708 : add Index Arm Axis     //Steven 20230323 : Mark for NN mode關Arm
//                {
                    XShiftF     =Prod.TestY1_Front - Prod.TestY1_Middle;
//                }
//                else
//                {
//                    XShiftF     =Prod.TestY1_Front-MOT[MTestY1].Gali_ReadPos();  //Steven 20140624 : 關Arm跳脫修正
//                }
                Z1Up        =0;
                Z2DownSafe  =Prod.All_TestZ_Test_Safe - Prod.TestZ2_Safe;
                Z2Down      =Prod.TestZ2_Test - Prod.All_TestZ_Test_Safe;

                Z2Safe      =Prod.All_TestZ_Test_Safe - MOT[MTestZ2].Gali_ReadPos();
                XShiftR     =Prod.TestY2_Middle - Prod.TestY2_Rear;             //Y2_Index To Y2_Shuttle2 Pitch
                Z2Up        =Prod.TestZ2_Safe - Prod.All_TestZ_Test_Safe;
                Z1DownSafe  =0;
                Z1Down      =0;

                AxisY_Pre_MovePos = Prod.TestY_Pre_MovePos;
            }
        }
        ////20111114  Dell for Disable Index Arm    End
        //        GetEachAxisSpeed();
    }
}
//-----------------------------------------------------------------------------
void ShowMainScreenPresure(int index)
{
    AnsiString str;
    double position;

    int UseArm=0;//kevin 20130425  0

    if(TestIF_File.iShuttleMode==0 ||
       (TestIF_File.bArm1PickPlaceArm2Test && IniConfig.bD58UseArm1PickPlaceArm2Test==true))    //Steven 20200615 : add arm 2 torque for D58
    {
        UseArm=0;//kevin 20130425  使用兩隻ARM
    }
    else
    {
        if(TestIF_File.iShuttle_Sel==0)
            UseArm=1;                   //使用Arm 1
        else
            UseArm=2;                   //kevin 20130425  使用Arm 2
    }

    if(index==0)
    {
        if(UseArm==2)//kevin 20130425  使用ARM2
        {
            fMain->lbArm0Torque->Caption=AnsiString("1:NoUse");
            if(bContaceTorque==false &&
               IniConfig.bKoreaFunction==false &&
               CUSTOMER_CODE!=CC_TSMC_TAINAN &&
               IniConfig.bSPILFunction==false)
            {
                asArmForce1 = "NULL" ;    //2013.01.11 Q_Q TSMC GPIB COMMAND
            }
            return;
        }

        #ifdef SOFT_SIMULTE
            fMain->edTorue0->Text="10.0";
        #endif

        if(fMain->edTorue0->Text!="")
        {
            position=atof(ConvertTouMType(Prod.TestZ1_Test));

            if(IniConfig.bShowContactHeight==true)   //Steven 20100827
                str.sprintf("1: %s %%, Pos=%3.2f mm", fMain->edTorue0->Text, position);
            else
                str.sprintf("1: %s %%", fMain->edTorue0->Text);

            //MyDBITorqueData(1, fMain->edTorue0->Text, position);  //Steven 20100907 : 影響效能，關掉
            fMain->lbArm0Torque->Caption=str;
            //if(IniConfig.bKoreaFunction==false)
            if(bContaceTorque==false &&             //wei 20151117 傳送ContactForce設定值
               IniConfig.bKoreaFunction==false &&
               CUSTOMER_CODE!=CC_TSMC_TAINAN &&
               IniConfig.bSPILFunction==false)
            {
                asArmForce1 = FormatFloat ( "0.00" , atof(fMain->edTorue0->Text.c_str()))+"T";   //kevin 20130425  2013.01.11 Q_Q TSMC GPIB COMMAND
            }
        }
    }
    else
    {
        if(UseArm==1)//kevin 20130425  使用ARM1
        {
            fMain->lbArm1Torque->Caption=AnsiString("2:NoUse");
            //if(IniConfig.bKoreaFunction==false)
            if(bContaceTorque==false &&
               IniConfig.bKoreaFunction==false &&
               CUSTOMER_CODE!=CC_TSMC_TAINAN &&
               IniConfig.bSPILFunction==false)
            {
                asArmForce2 = "NULL" ;    //2013.01.11 Q_Q TSMC GPIB COMMAND
            }
            return;
        }

        #ifdef SOFT_SIMULTE
            fMain->edTorue1->Text="10.0";
        #endif

        if(fMain->edTorue1->Text!="")
        {
            position=atof(ConvertTouMType(Prod.TestZ2_Test));

            if(IniConfig.bShowContactHeight==true)   //Steven 20100827
                str.sprintf("2: %s %%, Pos=%3.2f mm", fMain->edTorue1->Text, position);
            else
                str.sprintf("2: %s %%", fMain->edTorue1->Text);

            fMain->lbArm1Torque->Caption=str;
            if(bContaceTorque==false &&
               IniConfig.bKoreaFunction==false &&
               CUSTOMER_CODE!=CC_TSMC_TAINAN &&
               IniConfig.bSPILFunction==false)
            {
                asArmForce2 = FormatFloat ( "0.00" , atof(fMain->edTorue1->Text.c_str()))+"T";              //kevin 20130425  //2013.01.11 Q_Q TSMC GPIB COMMAND
            }
        }
    }

/*
    char str[256];
    if(index==0)
        fMain->lbArm0Torque->Caption=AnsiString("1:")+fMain->edTorue0->Text+" %";
    else
        fMain->lbArm1Torque->Caption=AnsiString("2:")+fMain->edTorue1->Text+" %";
*/
}
//-----------------------------------------------------------------------------
void SetOffsetToTech()
{
/*
0    "Loader",
1    "Hot Plate1",
2    "Hot Plate2",
3    "Input Shuttle1",
4    "Input Shuttle2",

0    "Output Shuttle1",
1    "Output Shuttle2",
2    "Auto1",
3    "Auto2",
4    "Auto3",
5    "Fix1",
6    "Fix2",
7    "Fix3"
*/
    int i, j,ct,ret,iZ,iPitch;
    double InArmZ[MAX_ARM_Row][MAX_ARM_Col]={{0,0,0,0},{0,0,0,0}};
    double OutArmZ[MAX_ARM_Row][MAX_ARM_Col]={{0,0,0,0},{0,0,0,0}};

    if(fAllMotorHome==false)
    {
        ret=MessageDlg("Sure to Set Offset To Tech? (確定要將補償值存至教導?)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
        if(ret==mrNo)
        return;
    }
//------------------------------------------------------------------------------
//-----------
// In Arm X,Y
//-----------
    Tech.iInArmLoadStageX+=InArmOffSet[InOfsLoader]->GetX();
    Tech.iInArmLoadStageY+=InArmOffSet[InOfsLoader]->GetY();
    Tech.iInArmPlate1X+=InArmOffSet[InOfsHP1]->GetX();
    Tech.iInArmPlate1Y+=InArmOffSet[InOfsHP1]->GetY();
    Tech.iInArmPlate2X+=InArmOffSet[InOfsHP2]->GetX();
    Tech.iInArmPlate2Y+=InArmOffSet[InOfsHP2]->GetY();  //Wei 20141006 : GetX--> GetY
    Tech.iInArmShuttle1X+=InArmOffSet[InOfsInSh1]->GetX();
    Tech.iInArmShuttle1Y+=InArmOffSet[InOfsInSh1]->GetY();
    Tech.iInArmShuttle2X+=InArmOffSet[InOfsInSh2]->GetX();
    Tech.iInArmShuttle2Y+=InArmOffSet[InOfsInSh2]->GetY();
//-----------
// In Arm Z
//-----------
    iZ=InArmOffSet[InOfsLoader]->GetPickUp();
    Tech.iInArmLoadStagePickZ2+=iZ;
    iZ=(InArmOffSet[InOfsHP1]->GetPickUp()+InArmOffSet[InOfsHP2]->GetPickUp())/2;
    Tech.iInArmPlatePickZ2+=iZ;
    iZ=(InArmOffSet[InOfsInSh1]->GetPlace()+InArmOffSet[InOfsInSh2]->GetPlace())/2;
    Tech.iInArmShuttlePlaceZ+=iZ;

    for(i=0; i<InArmSuck.iMotRow; i++)
    {
        for(j=0; j<InArmSuck.iMotCol; j++)
        {
            InArmZ[i][j]+=InArmOffSet[InOfsLoader]->GetPickUp(i, j);
        }
    }

    for(ct=0;ct<2;ct++)
    {
        for(i=0; i<InArmSuck.iMotRow; i++)
        {
            for(j=0; j<InArmSuck.iMotCol; j++)
            {
                InArmZ[i][j]+=InArmOffSet[InOfsInSh1+ct]->GetPlace(i, j);
            }
        }
    }

    for(i=0; i<InArmSuck.iMotRow; i++)
    {
        for(j=0; j<InArmSuck.iMotCol; j++)
        {
            if(j<4)
                Tech.iInArmZHeightSub[i][j]+=InArmZ[i][j]/3;
            else
                Tech.iInArmZHeightSub_16[i][j-4]+=InArmZ[i][j]/3;
        }
    }
//-----------
// In Arm Pitch
//-----------
    iPitch=(InArmOffSet[InOfsInSh1]->GetVariable()+InArmOffSet[InOfsInSh2]->GetVariable())/2;
    Tech.iInArmX120Pitch+=iPitch;    //Steven 20131002 : XY變距
    iPitch=InArmOffSet[InOfsLoader]->GetVariable();
    Tech.iInArmX40Pitch+=iPitch;     //Steven 20131002 : XY變距

//------------------------------------------------------------------------------
//-----------
// Out Arm X,Y
//-----------
    Tech.iOutArmShuttle1X+=OutArmOffSet[OutOfsOutSh1]->GetX();
    Tech.iOutArmShuttle1Y+=OutArmOffSet[OutOfsOutSh1]->GetY();
    Tech.iOutArmShuttle2X+=OutArmOffSet[OutOfsOutSh2]->GetX();
    Tech.iOutArmShuttle2Y+=OutArmOffSet[OutOfsOutSh2]->GetY();
    Tech.iOutArmAuto1X+=OutArmOffSet[OutOfsAuto1]->GetX();
    Tech.iOutArmAuto1Y+=OutArmOffSet[OutOfsAuto1]->GetY();
    Tech.iOutArmAuto2X+=OutArmOffSet[OutOfsAuto2]->GetX();
    Tech.iOutArmAuto2Y+=OutArmOffSet[OutOfsAuto2]->GetY();
    Tech.iOutArmAuto3X+=OutArmOffSet[OutOfsAuto3]->GetX();
    Tech.iOutArmAuto3Y+=OutArmOffSet[OutOfsAuto3]->GetY();
    Tech.iOutArmFix1X+=OutArmOffSet[OutOfsFix1]->GetX();
    Tech.iOutArmFix1Y+=OutArmOffSet[OutOfsFix1]->GetY();
    Tech.iOutArmFix2X+=OutArmOffSet[OutOfsFix2]->GetX();
    Tech.iOutArmFix2Y+=OutArmOffSet[OutOfsFix2]->GetY();
    Tech.iOutArmFix3X+=OutArmOffSet[OutOfsFix3]->GetX();
    Tech.iOutArmFix3Y+=OutArmOffSet[OutOfsFix3]->GetY();
//-----------
// Out Arm Z
//-----------
    iZ=(OutArmOffSet[OutOfsOutSh1]->GetPickUp()+OutArmOffSet[OutOfsOutSh2]->GetPickUp())/2;
    Tech.iOutArmShuttlePickZ2+=iZ;
    iZ=(OutArmOffSet[OutOfsAuto1]->GetPlace()+OutArmOffSet[OutOfsAuto2]->GetPlace()+OutArmOffSet[OutOfsAuto3]->GetPlace())/3;
    Tech.iOutArmPlaceZ2+=iZ;
    if(FIX3_FULL_PLACE==Fix3K_UseCylinder46LA)//JimmyChiu 20220927 : Stepper Motor Control in Fix3
    {
        iZ=(OutArmOffSet[OutOfsFix1]->GetPlace()+OutArmOffSet[OutOfsFix3]->GetPlace())/2;
        Tech.iOutArmPlaceFixZ1+=iZ;

        iZ=OutArmOffSet[OutOfsFix2]->GetPlace();
        Tech.iOutArmPlaceFix2Z1+=iZ;

    }
    else
    {
        iZ=(OutArmOffSet[OutOfsFix1]->GetPlace()+OutArmOffSet[OutOfsFix2]->GetPlace())/2;
        Tech.iOutArmPlaceFixZ1+=iZ;
    }

    for(ct=0; ct<2; ct++)
    {
        for(i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(j=0; j<OutArmSuck.iMotCol; j++)
            {
                OutArmZ[i][j]+=OutArmOffSet[ct]->GetPickUp(i,j);
            }
        }
    }

    for(ct=0; ct<5; ct++)
    {
        for(i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(j=0; j<OutArmSuck.iMotCol; j++)
            {
                OutArmZ[i][j]+=OutArmOffSet[2+ct]->GetPlace(i,j);
            }
        }
    }

    for(i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(j=0; j<OutArmSuck.iMotCol; j++)
        {
            if(j<4)
                Tech.iOutArmZHeightSub[i][j]+=OutArmZ[i][j]/7;
            else
                Tech.iOutArmZHeightSub_16[i][j-4]+=OutArmZ[i][j]/7;
        }
    }
//-----------
// Out Arm Pitch
//-----------
    iPitch=(OutArmOffSet[OutOfsOutSh1]->GetVariable()+OutArmOffSet[OutOfsOutSh2]->GetVariable())/2;
    Tech.iOutArmX120Pitch+=iPitch;    //Steven 20131002 : XY變距
    iPitch=(OutArmOffSet[OutOfsAuto1]->GetVariable()+OutArmOffSet[OutOfsAuto2]->GetVariable()+
            OutArmOffSet[OutOfsAuto3]->GetVariable()+OutArmOffSet[OutOfsFix1]->GetVariable()+
            OutArmOffSet[OutOfsFix2]->GetVariable())/5;
    Tech.iOutArmX40Pitch+=iPitch;    //Steven 20131002 : XY變距
//------------------------------------------------------------------------------
//-----------
// In Arm X,Y
//-----------
    for(ct=0; ct<5; ct++)
    {
        InArmOffSet[ct]->SetX(0);
        InArmOffSet[ct]->SetY(0);
    }
//-----------
// In Arm Z
//-----------
    for(ct=0; ct<5; ct++)
    {
        InArmOffSet[ct]->SetPickUp(0);
        InArmOffSet[ct]->SetPlace(0);

        for(i=0; i<InArmSuck.iMotRow; i++)
        {
            for(j=0; j<InArmSuck.iMotCol; j++)
            {
                InArmOffSet[ct]->SetPickUp(i, j, 0);
                InArmOffSet[ct]->SetPlace(i, j, 0);
            }
        }
    }
//-----------
// In Arm Pitch
//-----------
    for(ct=0; ct<6; ct++)
    {
        InArmOffSet[ct]->SetVariable(0);
    }
//------------------------------------------------------------------------------
//-----------
// Out Arm X,Y
//-----------
    for(ct=0; ct<7; ct++)
    {
        OutArmOffSet[ct]->SetX(0);
        OutArmOffSet[ct]->SetY(0);
    }
//-----------
// Out Arm Z
//-----------
    for(ct=0; ct<7; ct++)
    {
        OutArmOffSet[ct]->SetPickUp(0);
        OutArmOffSet[ct]->SetPlace(0);

        for(i=0; i<OutArmSuck.iMotRow; i++)
        {
            for(j=0; j<OutArmSuck.iMotCol; j++)
            {
                OutArmOffSet[ct]->SetPickUp(i, j, 0);
                OutArmOffSet[ct]->SetPlace(i, j, 0);
            }
        }
    }
//-----------
// Out Arm Pitch
//-----------
    for(ct=0;ct<7;ct++)
    {
        OutArmOffSet[ct]->SetVariable(0);
    }
//------------------------------------------------------------------------------
    if(fTeach!=NULL)
        fTeach->SaveFile(true);
//    WriteData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));
    fOffSet->SaveFile(0,0,true);
    fOffSet->ReadFile();
    fOffSet->DoIniDataToForm(fOffSet->iNowOffsetSel,1);
    fAllMotorHome=false;
}
//------------------------------------------------------------------------------
void InitialHT9045SModule()                                                     //Steven 20161117 : for HT-9045S
{
    if(USE_PICKER_COUNT==0)
    {
        InArmSuck.Suck[0][1].Enable=false;
        InArmSuck.Suck[0][3].Enable=false;
        InArmSuck.Suck[1][1].Enable=false;
        InArmSuck.Suck[1][3].Enable=false;
        OutArmSuck.Suck[0][1].Enable=false;
        OutArmSuck.Suck[0][3].Enable=false;
        OutArmSuck.Suck[1][1].Enable=false;
        OutArmSuck.Suck[1][3].Enable=false;
        MOT[MInArmZC].Motor->Enable=false;
        MOT[MInArmZD].Motor->Enable=false;
        MOT[MInArmZG].Motor->Enable=false;
        MOT[MInArmZH].Motor->Enable=false;
        MOT[MOutArmZC].Motor->Enable=false;
        MOT[MOutArmZD].Motor->Enable=false;
        MOT[MOutArmZG].Motor->Enable=false;
        MOT[MOutArmZH].Motor->Enable=false;
    }
    else if(USE_PICKER_COUNT==ep1Picker)                                        //Frank 20250812 : modify InOutArm1Motor
    {
        InArmSuck.Suck[0][1].Enable=false;
        InArmSuck.Suck[0][2].Enable=false;
        InArmSuck.Suck[0][3].Enable=false;
        InArmSuck.Suck[1][0].Enable=false;
        InArmSuck.Suck[1][1].Enable=false;
        InArmSuck.Suck[1][2].Enable=false;
        InArmSuck.Suck[1][3].Enable=false;

        OutArmSuck.Suck[0][1].Enable=false;
        OutArmSuck.Suck[0][2].Enable=false;
        OutArmSuck.Suck[0][3].Enable=false;
        OutArmSuck.Suck[1][0].Enable=false;
        OutArmSuck.Suck[1][1].Enable=false;
        OutArmSuck.Suck[1][2].Enable=false;
        OutArmSuck.Suck[1][3].Enable=false;

        MOT[MInArmZB].Motor->Enable=false;
        MOT[MInArmZC].Motor->Enable=false;
        MOT[MInArmZD].Motor->Enable=false;
        MOT[MInArmZE].Motor->Enable=false;
        MOT[MInArmZF].Motor->Enable=false;
        MOT[MInArmZG].Motor->Enable=false;
        MOT[MInArmZH].Motor->Enable=false;

        MOT[MOutArmZB].Motor->Enable=false;
        MOT[MOutArmZC].Motor->Enable=false;
        MOT[MOutArmZD].Motor->Enable=false;
        MOT[MOutArmZE].Motor->Enable=false;
        MOT[MOutArmZF].Motor->Enable=false;
        MOT[MOutArmZG].Motor->Enable=false;
        MOT[MOutArmZH].Motor->Enable=false;
    }
}
//------------------------------------------------------------------------------
//Steven 20110407 : 設定Shuttle Thread的參數
//int iCHKStep=0;
//int base_pos[2]={0,0};
//int iScanSensor=2;
//bool bExeShuttleThread=false;
//------------------------------------------------------------------------------
//const int CHECK_RANGE=500;                                                    //2013-04-12    Dell 500->750 在KYEC舊機調不進去 //jou 2013-05-21 750 -> 500 出現Out Shuttle lose //Steven 20160108 : 改去Teaching調整
void InitShuttleThreadParameter()
{
    int iShuttleZPitch=5000;
    AnsiString Str1, Str2;

    if(InitialOK==false)
        return;

    int iSiteXPitch = iUnitMultiply100(TestIF_File.dSiteXPitch);
    int iSiteYPitch = iUnitMultiply100(TestIF_File.dSiteYPitch);
    int iSiteYOffset= iUnitMultiply100(TestIF_File.dSiteYOffset);               //jou 2015-01-08 修正32 site shuttle 偵測使用 Z Sensor

    for(int i=0; i<8; i++)
    {
       SThreadPara.bSh1HasIC[0][i]=false;
       SThreadPara.bSh1HasIC[1][i]=false;
       SThreadPara.bSh2HasIC[0][i]=false;
       SThreadPara.bSh2HasIC[1][i]=false;
    }

    SThreadPara.ErrPartSh1="";
    SThreadPara.ErrPartSh2="";

    //全域的X, Y 使用幾個Site-----------------
//    SThreadPara.iRowItem=InArmSuck.iShtRow;
//    SThreadPara.iColItem=InArmSuck.iShtCol;

    //共有幾個Line需要檢查----------------------
    if(TestIF_File.iTestMode==_16Site2X8   ||                                   //2x8
       TestIF_File.iTestMode==_32Site4X8N)                                      //Steven 20140512 : For HT-9047
    {
        SThreadPara.iCHKStep=8;
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //ChungHung 20130507 add HT9045 updata for 12site 517 定義Shuttle檢查幾個Line
    {
        SThreadPara.iCHKStep=6;
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
    {
        SThreadPara.iCHKStep=5;
    }
    else if(TestIF_File.iTestMode==_8Site2X4    ||                              //2x4 || 1x4
            TestIF_File.iTestMode==QualSite1X4  ||
            TestIF_File.iTestMode==_8Site1X4    ||                              //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF_File.iTestMode==_16Site4X4   ||                              //Sam 20190226 : 16Site4X4
            TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        SThreadPara.iCHKStep=4;
    }
    else if(TestIF_File.iTestMode==_6Site2X3    ||                              //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==TriSite1X3   ||                              //Frank 20160329 add for 1x3_4
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        SThreadPara.iCHKStep=3;
    }
    else if(TestIF_File.iTestMode==QualSite2X2  ||
            TestIF_File.iTestMode==DualSite     ||
            TestIF_File.iTestMode==QualSite2X2N)                                //Frank 20200520 2X2NN Mode
    {
        SThreadPara.iCHKStep=2;
    }
    else if(TestIF_File.iTestMode==SingleSite   ||
            TestIF_File.iTestMode==DualSite2x1)
    {
        SThreadPara.iCHKStep=1;
    }
    else
    {
        SThreadPara.iCHKStep=0;
    }
    //----------------------------------------

    //要使用幾顆Sensor偵測--------------------------------
    if(SHUTTLE_Z_TYPE==0)                                                       //jou 2013-07-26 : 選擇 Shuttle Z Sensor Type
        iShuttleZPitch=5000;
    else
        iShuttleZPitch=2300;

    if(InArmSuck.iShtRow==1 ||                                                  //Steven 20240227 : Shuttle使用一排的
       (InArmSuck.iShtRow==2 && iSiteYPitch<iShuttleZPitch) ||                  //2x2, 2x4 Y<5000
       (TestIF_File.iTestMode==_32Site4X8N &&                                   //jou 2015-01-08 修正32 site shuttle 偵測使用 Z Sensor
        iSiteYOffset>-1999 &&                                                   //Steven 20150917 : -3000 --> -1999
        SHUTTLE_Z_TYPE==0) ||                                                   //Steven 20140512 : For HT-9047
       (TestIF_File.iTestMode==_6Site2X3 && Use_AxisY_Sensor_2x3mode==true))    //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
    {
        SThreadPara.iScanSensor=1;                                              //1顆scan sensor
    }
    else
    {
        SThreadPara.iScanSensor=2;                                              //2顆scan sensor
    }
    //----------------------------------------

    if(IniConfig.bKoreaFunction && TestIF_File.iTestMode==_6Site2X3 && Use_AxisY_Sensor_2x3mode==true)
    {
        SThreadPara.bOutYUseLatch=(MOTION_CARD_TYPE==MotionCard_Contec);
    }
    else if(ENABLE_OUT_SHUTTLEY_LATCH==true &&                                  //Steven 20230313 : 只要選擇使用Y-Latch, 就要把線接起來用
            InArmSuck.iShtRow==1)                                               //Steven 20240227 : Shuttle使用一排的
    {
        if(Bias_Mode_Use_Y_Sensor==true)                                        //Steven 20240805 : 1x2 Bias mode 用Y sensor 偵測Outshuttle殘料
            SThreadPara.bOutYUseLatch=(MOTION_CARD_TYPE==MotionCard_Contec);
        else if(TestIF_File.bNS7000kit==false)                                  //Steven 20200227 : 1x4, 1x2要使用Z Sensor
            SThreadPara.bOutYUseLatch=(MOTION_CARD_TYPE==MotionCard_Contec);
        else
            SThreadPara.bOutYUseLatch=false;
    }
    else
    {
        SThreadPara.bOutYUseLatch=false;
    }

    SThreadPara.base_posY[0]=Tech.OutSH1ZOneRowDetectPos+2000;                  //shuttle 1 kit center point
    SThreadPara.base_posY[1]=Tech.OutSH2ZOneRowDetectPos+2000;                  //shuttle 2 kit center point

    SThreadPara.base_InposY[0]=Tech.iInSH1Sen7DetectPos+2000;                   //shuttle 1 kit center point  //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
    SThreadPara.base_InposY[1]=Tech.iInSH2Sen7DetectPos+2000;                   //shuttle 2 kit center point  //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC

    SThreadPara.base_pos[0]=Tech.OutSH1ZDetectPos+2000;                         //shuttle 1 kit center point
    SThreadPara.base_pos[1]=Tech.OutSH2ZDetectPos+2000;                         //shuttle 2 kit center point
    if(TestIF_File.bQualSite2X2Shift)                                           //kevin 20171117 (wei) X shuttle 偏移 out shuttle 偵測位置
    {
       SThreadPara.base_posY[0]=Tech.OutSH1ZOneRowDetectPos+2000-TestIF_File.dShiftXPitch;   //kevin 20171117 (wei) add superflow X shiftshuttle 1 kit center point
       SThreadPara.base_posY[1]=Tech.OutSH2ZOneRowDetectPos+2000-TestIF_File.dShiftXPitch;   //kevin 20171117 (wei) add superflow X shiftshuttle 2 kit center point
       SThreadPara.base_pos[0] =Tech.OutSH1ZDetectPos+2000-TestIF_File.dShiftXPitch;         //kevin 20171117 (wei) add superflow X shiftshuttle 1 kit center point
       SThreadPara.base_pos[1] =Tech.OutSH2ZDetectPos+2000-TestIF_File.dShiftXPitch;         //kevin 20171117 (wei) add superflow X shiftshuttle 2 kit center point
    }

    SThreadPara.iLaserBasePos[0]=Tech.iSH1Laser+2000;                           //Steven 20140228 : 雷射測距功能
    SThreadPara.iLaserBasePos[1]=Tech.iSH2Laser+2000;                           //Steven 20140228 : 雷射測距功能

    //要啟用哪一種偵測方式----------------------------------
    SThreadPara.bUseM204Mode=false;                                             //Steven 20110627只有一種狀況會使用M204的觸發
    if(TestIF_File.iTestMode==_6Site2X3 &&
       Use_AxisY_Sensor_2x3mode==true)                                          //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
    {
        if(SThreadPara.iCHKStep==0 ||
           Tech.OutSH1ZOneRowDetectPos==0 ||
           Tech.OutSH2ZOneRowDetectPos==0 ||
           SThreadPara.bOutYUseLatch==true)                                     //前後對照
        {
            SThreadPara.bUseM204Mode=true;                                      //Steven 20110627
            SThreadPara.bExeShuttleThread=false;
            fLtcSensor->SetLtcSensor(0);                                        //Steven 20110526  //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
            fLtcSensor->SetLtcSensor(1);                                        //Steven 20110526
        }
        else
        {
            SThreadPara.bExeShuttleThread=true;                                 //Thread
        }
    }
    else if(SThreadPara.iScanSensor==1)                                         //1x2跟1x4
    {
        if(MOTION_CARD_TYPE==MotionCard_Contec &&
           SThreadPara.bOutYUseLatch==true)                                     //Steven 20140512 : For HT-9047
        {
            SThreadPara.bUseM204Mode=true;                                      //Steven 20110627
            SThreadPara.bExeShuttleThread=false;
            fLtcSensor->SetLtcSensor(0);                                        //Steven 20110526  //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
            fLtcSensor->SetLtcSensor(1);                                        //Steven 20110526
        }
        else if(SYN_TEK_MOTION_MODULE==G9004_M204 &&
                InArmSuck.iShtRow==1 &&                                         //Steven 20240227 : Shuttle使用一排的
                TestIF_File.iTestMode!=SingleSite  &&                           //ChungHung 20140826 add single site 不使用上下Sensor
                TestIF_File.bNS7000kit==true)                                   //1x2跟1x4 偏心
        {
            if(SThreadPara.iCHKStep==0  ||
               Tech.OutSH1ZDetectPos==0 ||
               Tech.OutSH2ZDetectPos==0)                                        //如果Z Sensor的Teach沒有設定
            {
                if(SThreadPara.iCHKStep==0 ||
                   Tech.OutSH1ZOneRowDetectPos==0 ||
                   Tech.OutSH2ZOneRowDetectPos==0)                              //如果前後對照Sensor的Teach沒有設定
                {
                    SThreadPara.bExeShuttleThread=false;                        //Latch
                }
                else
                {
                    SThreadPara.bExeShuttleThread=true;                         //Thread
                }
            }
            else
            {
                SThreadPara.bUseM204Mode=true;                                  //Steven 20110627
                SThreadPara.bExeShuttleThread=false;
                fLtcSensor->SetLtcSensor(0);                                    //Steven 20110526  //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
                fLtcSensor->SetLtcSensor(1);                                    //Steven 20110526
            }
        }
        else
        {
            if(SThreadPara.iCHKStep==0 ||
               Tech.OutSH1ZOneRowDetectPos==0 ||
               Tech.OutSH2ZOneRowDetectPos==0)                                  //前後對照
            {
                SThreadPara.bExeShuttleThread=false;                            //Latch
            }
            else
            {
                SThreadPara.bExeShuttleThread=true;                             //Thread
            }
        }
    }
    else
    {
        //jou 2010-09-08 shuttle z 上下對照sensor,機構sensor無法到達
        //jou 2011-09-06 TestIF.dSiteYPitch=6000
        //               TestIF_File.dSiteYPitch=60.0;
        //jou 2013-07-26 : 選擇 Shuttle Z Sensor Type
        if(SHUTTLE_Z_TYPE==0)
            iShuttleZPitch=5000;
        else
            iShuttleZPitch=2300;

        if(SThreadPara.iCHKStep==0  ||
           Tech.OutSH1ZDetectPos==0 ||
           Tech.OutSH2ZDetectPos==0 ||
           iSiteYPitch<iShuttleZPitch)
        {
            SThreadPara.bExeShuttleThread=false;
        }
        else
        {
            if(SYN_TEK_MOTION_MODULE==G9004_M204 ||
               MOTION_CARD_TYPE==MotionCard_Contec)
            {
                SThreadPara.bUseM204Mode=true;                                  //Steven 20110627
                SThreadPara.bExeShuttleThread=false;
                fLtcSensor->SetLtcSensor(0);                                    //Steven 20110526  //JerryYang 20230223 : 清除latch函式拆成shuttle 1,2
                fLtcSensor->SetLtcSensor(1);                                    //Steven 20110526
            }
            else
            {
                SThreadPara.bExeShuttleThread=true;
            }
        }
    }
    //----------------------------------------

    //Steven 20110622設定要被檢查的Shuttle Sensor-----------
    int SnInSht1Index[8]={SnInPutSHT1S1, SnInPutSHT1S2, SnInPutSHT1S3, SnInPutSHT1S4, SnInPutSHT1S5, SnInPutSHT1S6, SnInPutSHT1S7, SnInPutSHT1S8};
    int SnInSht2Index[8]={SnInPutSHT2S1, SnInPutSHT2S2, SnInPutSHT2S3, SnInPutSHT2S4, SnInPutSHT2S5, SnInPutSHT2S6, SnInPutSHT2S7, SnInPutSHT2S8};

    int SnOutSht1Index[6]={SnOutPutSHT1S2, SnOutPutSHT1S3, SnOutPutSHT1S4, SnOutPutSHT1S5, SnOutPutSHT1S6, SnOutPutSHT1S7};
    int SnOutSht2Index[6]={SnOutPutSHT2S2, SnOutPutSHT1S3, SnOutPutSHT2S4, SnOutPutSHT2S5, SnOutPutSHT2S6, SnOutPutSHT2S7};

    for(int i=0; i<8; i++)                                                      //Steven 20250429 : 統一整理shuttle sensor顯示
    {
        SThreadPara.iUseInShtStep[i]=0;
        SThreadPara.iInShSenIndex[0][i]=0;
        SThreadPara.iInShSenIndex[1][i]=0;
        SThreadPara.bUseInShtSen[i]=false;                                      //Steven 20250429 : 有用到的in sht sensor位置

        SThreadPara.iOutShSenIndex[0][i]=0;
        SThreadPara.iOutShSenIndex[1][i]=0;
    }

    //In Shuttle Sensor ----
    if(SThreadPara.iCHKStep==1)                                                 // SingleSite
    {
        if(MachineTypeChoice==Type_HT9045)
        {
            SThreadPara.iUseInShtStep[0]=2;
            SThreadPara.iUseInShtStep[1]=2;
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            SThreadPara.iUseInShtStep[0]=2;
            SThreadPara.iUseInShtStep[1]=2;
        }
        else
        {
            if(AUTO_SENSOR_INSTALL)                                             //wei 20160914 Auto Shuttle Sensor  //使用 3
            {
                SThreadPara.iUseInShtStep[0]=2;
                SThreadPara.iUseInShtStep[1]=2;
            }
            else
            {
                SThreadPara.iUseInShtStep[0]=3;
                SThreadPara.iUseInShtStep[1]=3;
            }
        }
    }
    else if(SThreadPara.iCHKStep==2)
    {
        if(MachineTypeChoice==Type_HT9045)
        {
            if(TestIF_File.b1x2Use1x4SiteKit)                                   //Steven 20230725 : For HT9045 1x2mode use 1x4SLK sht sensor
            {
                SThreadPara.iUseInShtStep[0]=0;
                SThreadPara.iUseInShtStep[1]=5;
            }
            else if(iSiteXPitch<=3450)                                          //Frank 20170803 (wei) 2666==>3450
            {
                SThreadPara.iUseInShtStep[0]=2;
                SThreadPara.iUseInShtStep[1]=3;
            }
            else
            {
                SThreadPara.iUseInShtStep[0]=1;
                SThreadPara.iUseInShtStep[1]=4;
            }
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            if(iSiteXPitch<=3450)                                               //Frank 20170803 (wei) 2666==>3450
            {
                SThreadPara.iUseInShtStep[0]=2;
                SThreadPara.iUseInShtStep[1]=3;
            }
            else
            {
                SThreadPara.iUseInShtStep[0]=1;
                SThreadPara.iUseInShtStep[1]=4;
            }
        }
        else
        {
            if(AUTO_SENSOR_INSTALL)                                             //wei 20160914 Auto Shuttle Sensor
            {
                if(iSiteXPitch>=9000 && iSiteXPitch<=18000)                     //使用 1, 5
                {
                    SThreadPara.iUseInShtStep[0]=0;
                    SThreadPara.iUseInShtStep[1]=4;
                }
                else                                                            //使用 2, 4
                {
                    SThreadPara.iUseInShtStep[0]=1;
                    SThreadPara.iUseInShtStep[1]=3;
                }
            }
            else if(CUSTOMER_CODE==CC_TSMC_TAINAN && TestIF_File.bQualSite2X2Shift)  //wei 20160226 TSMC X Shift
            {
                SThreadPara.iUseInShtStep[0]=2;
                SThreadPara.iUseInShtStep[1]=4;
            }
            else if(iSiteXPitch<=4000)                                          //Frank 20170803 (wei) 2666==>3450
            {
                SThreadPara.iUseInShtStep[0]=3;
                SThreadPara.iUseInShtStep[1]=4;
            }
            else
            {
                SThreadPara.iUseInShtStep[0]=2;                                 //JerryYang 20250701 : Fixed for shuttle sensor
                SThreadPara.iUseInShtStep[1]=5;
            }
        }
    }
    else if(SThreadPara.iCHKStep==3)                                            //ChungHung 20140115 add for 2x3_6
    {
        if(MachineTypeChoice==Type_HT9045)
        {
            SThreadPara.iUseInShtStep[0]=0;                                     //使用 1, 3, 4
            SThreadPara.iUseInShtStep[1]=2;
            SThreadPara.iUseInShtStep[2]=3;
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            SThreadPara.iUseInShtStep[0]=0;                                     //使用 1, 3, 4
            SThreadPara.iUseInShtStep[1]=2;
            SThreadPara.iUseInShtStep[2]=3;
        }
        else
        {
            if(AUTO_SENSOR_INSTALL)                                             //wei 20160914 Auto Shuttle Sensor
            {
                if(iSiteXPitch*3*2>=12000 && iSiteXPitch*3*2<=24000)
                {
                    SThreadPara.iUseInShtStep[0]=1;                             //使用 2, 3, 4
                    SThreadPara.iUseInShtStep[1]=2;
                    SThreadPara.iUseInShtStep[2]=3;
                }
                else
                {
                    SThreadPara.iUseInShtStep[0]=0;                             //使用 1, 3, 5
                    SThreadPara.iUseInShtStep[1]=2;
                    SThreadPara.iUseInShtStep[2]=4;
                }
            }
            else
            {
                if(iSiteXPitch>=8000)                                           //使用 2, 4, 7     //Sam 20230828 : 多增加 = //Steven 20200313 : For 2x3 X-Pitch 120mm
                {
                    SThreadPara.iUseInShtStep[0]=1;
                    SThreadPara.iUseInShtStep[1]=3;
                    SThreadPara.iUseInShtStep[2]=6;
                }
                else
                {
                    SThreadPara.iUseInShtStep[0]=1;                             //使用 2, 4, 5
                    SThreadPara.iUseInShtStep[1]=3;
                    SThreadPara.iUseInShtStep[2]=4;
                }
            }
        }
    }
    else if(SThreadPara.iCHKStep==4)
    {
        if(MachineTypeChoice==Type_HT9045)
        {
            SThreadPara.iUseInShtStep[0]=0;                                     //使用 1, 3, 4, 6
            SThreadPara.iUseInShtStep[1]=2;
            SThreadPara.iUseInShtStep[2]=3;
            SThreadPara.iUseInShtStep[3]=5;
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            SThreadPara.iUseInShtStep[0]=0;                                     //使用 1, 3, 4, 6
            SThreadPara.iUseInShtStep[1]=2;
            SThreadPara.iUseInShtStep[2]=3;
            SThreadPara.iUseInShtStep[3]=5;
        }
        else
        {
            if(AUTO_SENSOR_INSTALL)                                             //wei 20160914 Auto Shuttle Sensor
            {
                SThreadPara.iUseInShtStep[0]=0;                                 //使用 1, 2, 4, 5
                SThreadPara.iUseInShtStep[1]=1;
                SThreadPara.iUseInShtStep[2]=3;
                SThreadPara.iUseInShtStep[3]=4;
            }
            else
            {
                if(IniConfig.bF30InShuttleSensorFollow16site)                   //JerryYang 20210426 : 2x4, 2x6 follow 2x8 shuttle sensor位置
                {
                    SThreadPara.iUseInShtStep[0]=2;                             //使用 3, 4, 5, 6
                    SThreadPara.iUseInShtStep[1]=3;
                    SThreadPara.iUseInShtStep[2]=4;
                    SThreadPara.iUseInShtStep[3]=5;
                }
                else
                {
                    SThreadPara.iUseInShtStep[0]=1;                             //使用 2, 4, 5, 7
                    SThreadPara.iUseInShtStep[1]=3;
                    SThreadPara.iUseInShtStep[2]=4;
                    SThreadPara.iUseInShtStep[3]=6;
                }
            }
        }
    }
    else if(SThreadPara.iCHKStep==5)                                            //_10Site2X5   kevin 20220122  add shuttle
    {
        SThreadPara.iUseInShtStep[0]=0;                                         //使用 1, 3, 4, 5, 7
        SThreadPara.iUseInShtStep[1]=2;
        SThreadPara.iUseInShtStep[2]=3;
        SThreadPara.iUseInShtStep[3]=4;
        SThreadPara.iUseInShtStep[4]=6;
    }
    else if(SThreadPara.iCHKStep==6)                                            //_12Site2X6
    {
        if(MachineTypeChoice==Type_HT9045_12Site)                               //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            SThreadPara.iUseInShtStep[0]=0;                                     //使用 1, 2, 3, 4, 5, 6
            SThreadPara.iUseInShtStep[1]=1;
            SThreadPara.iUseInShtStep[2]=2;
            SThreadPara.iUseInShtStep[3]=3;
            SThreadPara.iUseInShtStep[4]=4;
            SThreadPara.iUseInShtStep[5]=5;
        }
        else if(MachineTypeChoice==Type_HT9046 ||
                MachineTypeChoice==Type_HT9046_LS ||
                MachineTypeChoice==Type_HT1032)
        {
            if(IniConfig.bF30InShuttleSensorFollow16site)                       //JerryYang 20210426 : 2x4, 2x6 follow 2x8 shuttle sensor位置
            {
                if(iCloseSiteStep_2x6!=0)
                {
                    SThreadPara.iUseInShtStep[0]=2;
                    SThreadPara.iUseInShtStep[1]=3;
                    SThreadPara.iUseInShtStep[2]=4;
                    SThreadPara.iUseInShtStep[3]=5;
                }
                else
                {
                    SThreadPara.iUseInShtStep[0]=1;                             //使用 1, 2, 3, 4, 5, 6
                    SThreadPara.iUseInShtStep[1]=2;
                    SThreadPara.iUseInShtStep[2]=3;
                    SThreadPara.iUseInShtStep[3]=4;
                    SThreadPara.iUseInShtStep[4]=5;
                    SThreadPara.iUseInShtStep[5]=6;
                }
            }
            else
            {
                SThreadPara.iUseInShtStep[0]=0;                                 //使用 1, 2, 4, 5, 7, 8
                SThreadPara.iUseInShtStep[1]=1;
                SThreadPara.iUseInShtStep[2]=3;
                SThreadPara.iUseInShtStep[3]=4;
                SThreadPara.iUseInShtStep[4]=6;
                SThreadPara.iUseInShtStep[5]=7;
            }
        }
    }
    else if(SThreadPara.iCHKStep==8)
    {
        for(int i=0; i<8; i++)
        {
            SThreadPara.iUseInShtStep[i]=i;
        }
    }

    int id, iStep=SThreadPara.iCHKStep;

    if(SThreadPara.iCHKStep==6 &&
       IniConfig.bF30InShuttleSensorFollow16site &&                             //JerryYang 20210426 : 2x4, 2x6 follow 2x8 shuttle sensor位置
       iCloseSiteStep_2x6!=0)
    {
        iStep=4;
    }

    for(int i=0; i<iStep; i++)                                                  //Steven 20250429 : 統一整理shuttle sensor顯示
    {
        id=SThreadPara.iUseInShtStep[i];
        SThreadPara.bUseInShtSen[id]=true;
        SThreadPara.iInShSenIndex[0][i]=SnInSht1Index[id];
        SThreadPara.iInShSenIndex[1][i]=SnInSht2Index[id];

        if(id<6)                                                                //JerryYang 20250822 : i->id //JerryYang 20250725 : 增加保護
        {
            SThreadPara.iOutShSenIndex[0][i]=SnOutSht1Index[id];
            SThreadPara.iOutShSenIndex[1][i]=SnOutSht2Index[id];
        }
    }

    //2013-07-16    Dell    Shuttle cross sensor
    SThreadPara.iInShSenIndex[0][8] = SnCrossSHT1S1;
    SThreadPara.iInShSenIndex[0][9] = SnCrossSHT1S2;
    SThreadPara.iInShSenIndex[1][8] = SnCrossSHT2S1;
    SThreadPara.iInShSenIndex[1][9] = SnCrossSHT2S2;
    //----------------------------------------

    #ifdef DEBUG
        SThreadPara.bExeShuttleThread=false;
    #endif

    //計算要被比較的位置----------------------------------
    if(SThreadPara.iCHKStep==1)                                                 //SingleSite
    {
        if(SThreadPara.bUseM204Mode)
        {
            SThreadPara.iCheckPosMaxY[0][0]     =ChangeToFloatNonPcnt((double)(SThreadPara.base_posY[0]), (double)(MOT[MInShuttle1].Motor->GearRatio))+CHECK_RANGE;
            SThreadPara.iCheckPosMinY[0][0]     =ChangeToFloatNonPcnt((double)(SThreadPara.base_posY[0]), (double)(MOT[MInShuttle1].Motor->GearRatio))-CHECK_RANGE;
            SThreadPara.iCheckPosMaxY[1][0]     =ChangeToFloatNonPcnt((double)(SThreadPara.base_posY[1]), (double)(MOT[MInShuttle2].Motor->GearRatio))+CHECK_RANGE;
            SThreadPara.iCheckPosMinY[1][0]     =ChangeToFloatNonPcnt((double)(SThreadPara.base_posY[1]), (double)(MOT[MInShuttle2].Motor->GearRatio))-CHECK_RANGE;
            SThreadPara.iCheckInPosMaxY[0][0]   =ChangeToFloatNonPcnt((double)(SThreadPara.base_InposY[0]), (double)(MOT[MInShuttle1].Motor->GearRatio))+CHECK_RANGE;      //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
            SThreadPara.iCheckInPosMinY[0][0]   =ChangeToFloatNonPcnt((double)(SThreadPara.base_InposY[0]), (double)(MOT[MInShuttle1].Motor->GearRatio))-CHECK_RANGE;
            SThreadPara.iCheckInPosMaxY[1][0]   =ChangeToFloatNonPcnt((double)(SThreadPara.base_InposY[1]), (double)(MOT[MInShuttle2].Motor->GearRatio))+CHECK_RANGE;
            SThreadPara.iCheckInPosMinY[1][0]   =ChangeToFloatNonPcnt((double)(SThreadPara.base_InposY[1]), (double)(MOT[MInShuttle2].Motor->GearRatio))-CHECK_RANGE;
            SThreadPara.iLaserCheckPos[0][0]    =ChangeToFloatNonPcnt((double)(SThreadPara.iLaserBasePos[0]), (double)(MOT[MInShuttle1].Motor->GearRatio));                //Steven 20140228 : 雷射測距功能
            SThreadPara.iLaserCheckPos[1][0]    =ChangeToFloatNonPcnt((double)(SThreadPara.iLaserBasePos[1]), (double)(MOT[MInShuttle2].Motor->GearRatio));                //Steven 20140228 : 雷射測距功能
        }
        else
        {
            SThreadPara.iCheckPosMaxY[0][0]     =SThreadPara.base_posY[0]+CHECK_RANGE;
            SThreadPara.iCheckPosMinY[0][0]     =SThreadPara.base_posY[0]-CHECK_RANGE;
            SThreadPara.iCheckPosMaxY[1][0]     =SThreadPara.base_posY[1]+CHECK_RANGE;
            SThreadPara.iCheckPosMinY[1][0]     =SThreadPara.base_posY[1]-CHECK_RANGE;
            SThreadPara.iLaserCheckPos[0][0]    =SThreadPara.iLaserBasePos[0];  //Steven 20140228 : 雷射測距功能
            SThreadPara.iLaserCheckPos[1][0]    =SThreadPara.iLaserBasePos[1];  //Steven 20140228 : 雷射測距功能
        }
    }
    else
    {
        for(int iShuttleNo=0; iShuttleNo<2; iShuttleNo++)
        {
            for(int iLineNo=0; iLineNo<SThreadPara.iCHKStep; iLineNo++)
            {
                if(SThreadPara.bUseM204Mode)    //Steven 20110627
                {
                    SThreadPara.iCheckPosMax[iShuttleNo][iLineNo]=ChangeToFloatNonPcnt((double)(GetSHCHKPos(iLineNo, SThreadPara.base_pos[iShuttleNo])), (double)(MOT[MInShuttle1].Motor->GearRatio))+CHECK_RANGE;
                    SThreadPara.iCheckPosMin[iShuttleNo][iLineNo]=ChangeToFloatNonPcnt((double)(GetSHCHKPos(iLineNo, SThreadPara.base_pos[iShuttleNo])), (double)(MOT[MInShuttle1].Motor->GearRatio))-CHECK_RANGE*1.2;   //Steven 20110530 : 通常會抓到比較接近最小值的,所以要多減一點。

                    SThreadPara.iCheckPosMaxY[iShuttleNo][iLineNo]=ChangeToFloatNonPcnt((double)(GetSHCHKPos(iLineNo, SThreadPara.base_posY[iShuttleNo])), (double)(MOT[MInShuttle1].Motor->GearRatio))+CHECK_RANGE;
                    SThreadPara.iCheckPosMinY[iShuttleNo][iLineNo]=ChangeToFloatNonPcnt((double)(GetSHCHKPos(iLineNo, SThreadPara.base_posY[iShuttleNo])), (double)(MOT[MInShuttle1].Motor->GearRatio))-CHECK_RANGE*1.2;   //Steven 20110530 : 通常會抓到比較接近最小值的,所以要多減一點。

                    SThreadPara.iCheckInPosMaxY[iShuttleNo][iLineNo]=ChangeToFloatNonPcnt((double)(GetSHCHKPos(iLineNo, SThreadPara.base_InposY[iShuttleNo])), (double)(MOT[MInShuttle1].Motor->GearRatio))+CHECK_RANGE;       //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
                    SThreadPara.iCheckInPosMinY[iShuttleNo][iLineNo]=ChangeToFloatNonPcnt((double)(GetSHCHKPos(iLineNo, SThreadPara.base_InposY[iShuttleNo])), (double)(MOT[MInShuttle1].Motor->GearRatio))-CHECK_RANGE*1.2;   //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC

                }
                else
                {
                    SThreadPara.iCheckPosMax[iShuttleNo][iLineNo]=GetSHCHKPos(iLineNo, SThreadPara.base_pos[iShuttleNo])+CHECK_RANGE;
                    SThreadPara.iCheckPosMin[iShuttleNo][iLineNo]=GetSHCHKPos(iLineNo, SThreadPara.base_pos[iShuttleNo])-CHECK_RANGE;

                    SThreadPara.iCheckPosMaxY[iShuttleNo][iLineNo]=GetSHCHKPos(iLineNo, SThreadPara.base_posY[iShuttleNo])+CHECK_RANGE;
                    SThreadPara.iCheckPosMinY[iShuttleNo][iLineNo]=GetSHCHKPos(iLineNo, SThreadPara.base_posY[iShuttleNo])-CHECK_RANGE;
                }
            }
        }

        for(int iShuttleNo=0; iShuttleNo<2; iShuttleNo++)
        {
            for(int iLineNo=0; iLineNo<SThreadPara.iCHKStep; iLineNo++)
            {
                int j=SThreadPara.iCHKStep-1-iLineNo;     //從後面開始掃
                SThreadPara.iLaserCheckPos[iShuttleNo][iLineNo]=GetSHCHKPos(j, SThreadPara.iLaserBasePos[iShuttleNo]);                                    //Steven 20140228 : 雷射測距功能
            }
        }
    }

    //顯示Out Shuttle 相關參數與狀態------------------
    AnsiString  strMax1Z="Shuttle1 Z Max: ",
                strMax2Z="Shuttle2 Z Max: ",
                strMin1Z="Shuttle1 Z Min: ",
                strMin2Z="Shuttle2 Z Min: ",
                strMax1Y="Shuttle1 Y Max: ",
                strMax2Y="Shuttle2 Y Max: ",
                strMin1Y="Shuttle1 Y Min: ",
                strMin2Y="Shuttle2 Y Min: ",
                strMax1InY="Shuttle1 InY Max: ",
                strMax2InY="Shuttle2 InY Max: ",
                strMin1InY="Shuttle1 InY Min: ",
                strMin2InY="Shuttle2 InY Min: ";

    for(int iLineNo=0; iLineNo<SThreadPara.iCHKStep; iLineNo++)
    {
        strMax1Z+=AnsiString(SThreadPara.iCheckPosMax[0][iLineNo])+", ";
        strMin1Z+=AnsiString(SThreadPara.iCheckPosMin[0][iLineNo])+", ";
        strMax2Z+=AnsiString(SThreadPara.iCheckPosMax[1][iLineNo])+", ";
        strMin2Z+=AnsiString(SThreadPara.iCheckPosMin[1][iLineNo])+", ";

        strMax1Y+=AnsiString(SThreadPara.iCheckPosMaxY[0][iLineNo])+", ";
        strMin1Y+=AnsiString(SThreadPara.iCheckPosMinY[0][iLineNo])+", ";
        strMax2Y+=AnsiString(SThreadPara.iCheckPosMaxY[1][iLineNo])+", ";
        strMin2Y+=AnsiString(SThreadPara.iCheckPosMinY[1][iLineNo])+", ";

        strMax1InY+=AnsiString(SThreadPara.iCheckInPosMaxY[0][iLineNo])+", ";
        strMin1InY+=AnsiString(SThreadPara.iCheckInPosMinY[0][iLineNo])+", ";
        strMax2InY+=AnsiString(SThreadPara.iCheckInPosMaxY[1][iLineNo])+", ";
        strMin2InY+=AnsiString(SThreadPara.iCheckInPosMinY[1][iLineNo])+", ";
    }

    fMain->meShuttle1->Clear();
    fMain->meShuttle2->Clear();

    //Steven 20170925 (wei) : 修正Out Shuttle Latch的顯示方式
    if(SThreadPara.iScanSensor==1)
    {
        if(SThreadPara.bExeShuttleThread)
        {
            Str1="Use Y Sensor with Thread";
            Str2="Thread : 0.01mm/unit";
        }
        else
        {
            if(SThreadPara.bOutYUseLatch)
            {
                Str1="Use Y Sensor with Latch";
                Str2="Latch : pulse/unit";
            }
            else
            {
                Str1="Use Z Sensor with Latch";
                Str2="Latch : pulse/unit";
            }
        }
    }
    else    //2顆scan sensor
    {
        if(SThreadPara.bUseM204Mode)
        {
            Str1="Use Z Sensor with Latch";
            Str2="Latch : pulse/unit";
        }
        else
        {
            Str1="Use Z Sensor with Thread";
            Str2="Thread : 0.01mm/unit";
        }
    }
    fiosetview->labUseSensor1->Caption=Str1;
    fiosetview->labUseSensor2->Caption=Str1;

    fMain->AddShuttleMessage(0, Str1);
    fMain->AddShuttleMessage(1, Str1);
    fMain->AddShuttleMessage(0, Str2);
    fMain->AddShuttleMessage(1, Str2);

    if(ENABLE_OUT_SHUTTLEY_LATCH==true &&           //Steven 20180329 (Jou) : 使用Y-Latch偵測置偏
       IniConfig.iF07OutShuttleSensorMode==2 &&
       SThreadPara.iScanSensor==2)
    {
        Str1="Use Y Sensor with Latch for floating check.";
        Str2="Latch : pulse/unit";
        fMain->AddShuttleMessage(0, Str1);
        fMain->AddShuttleMessage(1, Str1);
    }

    fMain->AddShuttleMessage(0, strMax1Z);
    fMain->AddShuttleMessage(0, strMin1Z);
    fMain->AddShuttleMessage(0, strMax1Y);
    fMain->AddShuttleMessage(0, strMin1Y);
    fMain->AddShuttleMessage(0, strMax1InY);
    fMain->AddShuttleMessage(0, strMin1InY);

    fMain->AddShuttleMessage(1, strMax2Z);
    fMain->AddShuttleMessage(1, strMin2Z);
    fMain->AddShuttleMessage(1, strMax2Y);
    fMain->AddShuttleMessage(1, strMin2Y);
    fMain->AddShuttleMessage(1, strMax2InY);
    fMain->AddShuttleMessage(1, strMin2InY);

    if(TestIF_File.bEnSocketSensor)                                             //kevin 20130504 socket sensor
    {
        for(int i=0; i<iSnSocketCnt; i++)
            SThreadPara.iSocketSensor[i]=SnSocket1+i;
    }
    //----------------------------------------
}
//==============================================================================
int IsNNMode()                                                                  //Steven 20240916 : NN mode 判斷
{
    int iMode=None_NN;
    if(TestIF_File.iTestMode==QualSite2X2N ||                                   //Frank 20200520 2X2NN Mode
       TestIF_File.iTestMode==_6Site2X3N   ||                                   //Steven 20220425 : 2X3NN Mode
       TestIF_File.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        iMode=NN_1Row;
    }
    else if(TestIF_File.iTestMode==_32Site4X8N ||                               //Steven 20231225 : fixed for NN mode
            TestIF_File.iTestMode==_16Site4X4)
    {
        iMode=NN_2Row;
    }
    return iMode;
}
//==============================================================================
int GetInArmSuckBaseHeight(int iRow, int iCol)
{
    int iBack=Tech.iInArmZHeightSub[iRow][iCol];
    if(fProductionInfo->EnableInArmAutoCalSuckZ())
    {
        iBack+=iInArmZHeightDiff[iRow][iCol];
    }
    return iBack;
}
//------------------------------------------------------------------------------
int GetOutArmSuckBaseHeight(int iRow, int iCol)
{
    int iBack=Tech.iOutArmZHeightSub[iRow][iCol];
    if(fProductionInfo->EnableOutArmAutoCalSuckZ())
    {
        iBack+=iOutArmZHeightDiff[iRow][iCol];
    }
    return iBack;
}
//------------------------------------------------------------------------------
void TestTimeSetSpeed()                                                         //Ztex 2024.08.11 Add Test Time Set Speed
{
    int iMotNo;
    int iSetSp=0;
    eSpeedPart Arm=InArm;

    if(ArmSpeed_File[InArm].bTT_SetSpeed==false)
        return;
    if(LastSet.iTemperature==Tempture_Ambient || Tri_Temp_Machine==0)
        return;
    if(bTT_SetSpeed_Check==true)
        return;

    if(Prod.dTesterStartDelayTime<=7)
        iSetSp=90;
    else if(Prod.dTesterStartDelayTime>=17)
        iSetSp=40;
    else
        iSetSp=90-(Prod.dTesterStartDelayTime-7)*5;

    //In Arm
    SetMotorAccelSpeed(MInArmX, AutoArmSpeed[Arm].iACDCBodySP);
    SetMotorAccelSpeed(MInArmY, AutoArmSpeed[Arm].iACDCBodySP);
    SetMotorScaleSpeed(MInArmX, iSetSp);
    SetMotorScaleSpeed(MInArmY, iSetSp);

    SetMotorAccelSpeed(MInArmPitch, AutoArmSpeed[Arm].iACDCBodySP);
    SetMotorScaleSpeed(MInArmPitch, iSetSp);
    //In Arm

    //Out Arm
    SetMotorAccelSpeed(MOutArmX, AutoArmSpeed[Arm].iACDCBodySP);
    SetMotorAccelSpeed(MOutArmY, AutoArmSpeed[Arm].iACDCBodySP);
    SetMotorScaleSpeed(MOutArmX, iSetSp);
    SetMotorScaleSpeed(MOutArmY, iSetSp);

    SetMotorAccelSpeed(MOutArmPitch, AutoArmSpeed[Arm].iACDCBodySP);
    SetMotorScaleSpeed(MOutArmPitch, iSetSp);
    //Out Arm

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                    //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        //In Arm
        SetMotorAccelSpeed(MInArmPitchY, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MInArmPitchY, iSetSp);
        SetMotorAccelSpeed(MInArmPitchX2, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MInArmPitchX2, iSetSp);
        //In Arm
    }

    if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        //Out Arm
        SetMotorAccelSpeed(MOutArmPitchY, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MOutArmPitchY, iSetSp);
        SetMotorAccelSpeed(MOutArmPitchX2, AutoArmSpeed[Arm].iACDCBodySP);
        SetMotorScaleSpeed(MOutArmPitchX2, iSetSp);
        //Out Arm
    }

    if(USE_PICKER_COUNT==ep16Picker)
    {
        //In Arm
        SetMotorAccelSpeed(MInArmPitchX3, ArmSpeed[Arm].iACDCVariSP);
        SetMotorScaleSpeed(MInArmPitchX3, iSetSp);
        SetMotorAccelSpeed(MInArmPitchX4, ArmSpeed[Arm].iACDCVariSP);
        SetMotorScaleSpeed(MInArmPitchX4, iSetSp);
        //In Arm

        //Out Arm
        SetMotorAccelSpeed(MOutArmPitchX3, ArmSpeed[Arm].iACDCVariSP);
        SetMotorScaleSpeed(MOutArmPitchX3, iSetSp);
        SetMotorAccelSpeed(MOutArmPitchX4, ArmSpeed[Arm].iACDCVariSP);
        SetMotorScaleSpeed(MOutArmPitchX4, iSetSp);
        //Out Arm
    }

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            //In Arm
            iMotNo=InArmSuck.Suck[i][j].iMotNo;
            SetMotorAccelSpeed(iMotNo, AutoArmSpeed[Arm].iACDCBodySP);
            SetMotorScaleSpeed(iMotNo, iSetSp);
            //In Arm

            //Out Arm
            iMotNo=OutArmSuck.Suck[i][j].iMotNo;
            SetMotorAccelSpeed(iMotNo, AutoArmSpeed[Arm].iACDCBodySP);
            SetMotorScaleSpeed(iMotNo, iSetSp);
            //Out Arm
        }
    }
    bTT_SetSpeed_Check=true;
}
//------------------------------------------------------------------------------