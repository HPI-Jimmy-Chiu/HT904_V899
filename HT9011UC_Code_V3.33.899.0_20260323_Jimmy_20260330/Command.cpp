#include "MachineDefine.h"
#pragma hdrstop

#include "Command.h"

#include "BarCode.h"
#include "rs232.h"
#include "database.h"
#include "mymessbox.h"
#include "note.h"
#include "cMyDef.h"
#include "common.h"
#include "cprod.h"
#include "MyMotor.h"
#include "MyKitSuck.h"
#include "myQwertyKeyBoard.h"
#include "atester.h"
#include "Main.h"
#include "csystem.h"
#include "uLotInfo.h"
#include "MessageDef.h"
#include "cpublic.h"
#include "cContact.h"
#include "LtcSensor.h"
#include "bthermo.h"
#include "cinitial.h"
#include "adam6024.h"
#include "cSetUp.h"
#include "cContactCT.h"
#include "cConfiguration.h"
#include "cOffSet.h"
#include "cTestCategory.h"
#include "cBinSel.h"
#include "cUnitConvert.h"
#include "uYieldMonitoring.h"
#include "cShowBinSelect.h"
#include "mysensor.h"
#include "cSecurity.h"
#include "ProductionInfo.h"
#include "cTowerLight.h"
#include "DIOInterFaceCFG.h"
#include "cSpeed.h"
#include "iosetview.h"
#include "cTrayForm.h"
#include "cHotPlate.h"
#include "cTrayAssignment.h"
#include "uTemp_Set.h"
#include "cTesterIF.h"
#include "cBinSel.h"
#include "cLd_ULd.h"
#include "uYieldMonitoring.h"
#include "MyCCLinkSensor.h"
#include "uCleaning.h"
#include "QAMode.h"
#include "BarCode.h"
#include "fRotate.h"
#include "uteach.h"
#include "uMotorTest.h"
#include "cBuilder.h"
#include "cStartCondition.h"
#include "cCounterSel.h"
#include "cCounterClear.h"
#include "cSecurity.h"
#include "OmronEJ1N.h"
#include "ShuttleMove.h"
#include "uhome.h"
#include "cBinSel.h"
#include "cSetUp.h"
#include "FTPClient.h"
#include "cObserver.h"
#include "uLotInfo.h"
#include "ATC_Handler_Side.h"   //Ifor 20210831 :add New ATC Interface
#include "cSocket.h"
#include "SCK_ART.h"
#include "cSortCT.h"
#include "cAuthority.h"
#include "FixAICCD.h"
#include "main.h"
#include "AMR.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

AnsiString asTempArmOrder[2][16];
//---------------------------------------------------------------------------
int __fastcall TfMain::RefreshTempData(bool bTransfer, int iArm, int iSite)     //JerryYang 20190812 新增sitemap to 溫控器轉換, 輸入site,輸出Heater位置
{
    AnsiString asNULL = "NULL";
    int iTempKit0_Arm1=0, iTempKit1_Arm1=0, iTempKit0_Arm2=0, iTempKit1_Arm2=0;
    ZeroMemory(asTempArmOrder, sizeof(asTempArmOrder));
    int j2, iHead=-1;
    bool bSendTmp=(IniConfig.bL17HeadHeaterOnWhenCloseSite &                    //RogerYand 20251105 : 偉測要求L17開啟，關site也要上拋溫度給測試機(先用在2X4測試)
                    IniConfig.bVTESTFunction);

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<16; j++)
        {
            asTempArmOrder[i][j]=asNULL;
        }
    }

    if(Temperature.iIndexHeatMode==ChamberOnly ||
       Temperature.iIndexHeatMode==SocketChamber)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)                                 //wei 20160526 temp for sitemap
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)                                //Steven 20190604 : Fixed for記憶體破壞
                {
                    if(bTestSiteUse[0][i][j]==true)
                    {
                        asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[tcChamber];
                    }
                    else
                    {
                        asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                    }

                    if(bTestSiteUse[1][i][j]==true)
                    {
                        asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[tcChamber];
                    }
                    else
                    {
                        asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                    }
                }
            }
        }
        iHead=-1;
    }
    else
    {
        if(TestIF_File.iTestMode==QualSite2X2N)                                 //Steven 20231018 : Fixed for 2x2 NN mode
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcAa2]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcAb2]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcAa1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcAb1]:asNULL;
            }
            else
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)                                
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcHead2]:asNULL;
            }

            if(bTransfer==true)
            {
                if(iArm==1)
                {
                    iHead=iTempKit0_Arm1;
                }
                else if(iArm==2)
                {
                    iHead=iTempKit0_Arm2;
                }
            }
        }
        else if(TestIF_File.iTestMode==_6Site2X3N)                              //Steven 20220425 : 2X3NN Mode
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcAa2]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcAb2]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcAc2]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcAa1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcAb1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcAc1]:asNULL;
            }
            else
            {
                if(TestIF_File.iSiteMap[0][0]>0)                                //Steven 20250203 : Fixed for記憶體破壞
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcHead2]:asNULL;
            }

            if(bTransfer==true)
            {
                if(iArm==1)
                {
                    iHead=iTempKit0_Arm1;
                }
                else if(iArm==2)
                {
                    iHead=iTempKit0_Arm2;
                }
            }
        }
        else if(TestIF_File.iTestMode==_8Site2X4N)                              //Wei 20231211 : 2X4NN Mode
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                if(TestIF_File.iSiteMap[0][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcAa2]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcAb2]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcAc2]:asNULL;
                if(TestIF_File.iSiteMap[0][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][3]-1]=(bTestSiteUse[1][0][3]==true)?asGPIBTempShow[tcAd2]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcAa1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcAb1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcAc1]:asNULL;
                if(TestIF_File.iSiteMap[1][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][3]-1]=(bTestSiteUse[0][0][3]==true)?asGPIBTempShow[tcAd1]:asNULL;
            }
            else
            {
                if(TestIF_File.iSiteMap[0][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][0]-1]=(bTestSiteUse[1][0][0]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][1]-1]=(bTestSiteUse[1][0][1]==true)?asGPIBTempShow[tcHead3]:asNULL;
                if(TestIF_File.iSiteMap[0][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][2]-1]=(bTestSiteUse[1][0][2]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[0][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[0][3]-1]=(bTestSiteUse[1][0][3]==true)?asGPIBTempShow[tcHead4]:asNULL;
                if(TestIF_File.iSiteMap[1][0]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][0]-1]=(bTestSiteUse[0][0][0]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][1]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][1]-1]=(bTestSiteUse[0][0][1]==true)?asGPIBTempShow[tcHead1]:asNULL;
                if(TestIF_File.iSiteMap[1][2]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][2]-1]=(bTestSiteUse[0][0][2]==true)?asGPIBTempShow[tcHead2]:asNULL;
                if(TestIF_File.iSiteMap[1][3]>0)
                    asTempArmOrder[0][TestIF_File.iSiteMap[1][3]-1]=(bTestSiteUse[0][0][3]==true)?asGPIBTempShow[tcHead2]:asNULL;
            }

            if(bTransfer==true)
            {
                if(iArm==1)
                {
                    iHead=iTempKit0_Arm1;
                }
                else if(iArm==2)
                {
                    iHead=iTempKit0_Arm2;
                }
            }
        }
        else
        {
            for(int i=0; i<MAX_Index_Row; i++)                                  //wei 20160526 temp for sitemap
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(TestIF_File.iSiteMap[i][j]>0)
                    {
                        iTempKit0_Arm1=tcAa1+(i*4+j);
                        iTempKit0_Arm2=tcAa2+(i*4+j);
                        if(TestIF_File.iTestMode==QualSite1X4)
                        {
                            if(USE_16_HEATER==eht16Heater       ||              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                               USE_16_HEATER==eht16HeaterEJ1N   ||
                               USE_16_HEATER==eht32HeaterEJ1N   ||              //Steven 20140923 : Index使用EJ1N版32組加熱器
                               USE_16_HEATER==eht32HeaterKT4H   ||              //Steven 20150211 : Index使用KT4H版32組加熱器
                               USE_16_HEATER==eht16HeaterDTME08 ||              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                               USE_16_HEATER==eht32HeaterDTME08 )               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                            {
                                if(TestIF_File.b2CableLayoutKit ||
                                   (ATC_SYSTEM==eNewATCSystem &&
                                    Temperature.bATCActiveCooling==true))
                                {

                                }
                                else if(TestIF_File.b1CableLayoutKit)
                                {
                                    if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                    {
                                        iTempKit0_Arm1=tcAa1;
                                    }
                                    else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                    {
                                        iTempKit0_Arm1=tcBa1;
                                    }

                                    if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                    {
                                        iTempKit0_Arm2=tcAa2;
                                    }
                                    else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                    {
                                        iTempKit0_Arm2=tcBa2;
                                    }
                                }
                                else  //三條線的版本
                                {
                                    if(iTempKit0_Arm1==tcAb1)
                                        iTempKit0_Arm1=tcBa1;
                                    else if(iTempKit0_Arm1==tcAc1)
                                        iTempKit0_Arm1=tcAb1;
                                    else if(iTempKit0_Arm1==tcAd1)
                                        iTempKit0_Arm1=tcBb1;

                                    if(iTempKit0_Arm2==tcAb2)
                                        iTempKit0_Arm2=tcBa2;
                                    else if(iTempKit0_Arm2==tcAc2)
                                        iTempKit0_Arm2=tcAb2;
                                    else if(iTempKit0_Arm2==tcAd2)
                                        iTempKit0_Arm2=tcBb2;
                                }
                            }
                            else    //4組溫控器
                            {
                                if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                {
                                    iTempKit0_Arm1=tcHead1;
                                }
                                else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                {
                                    iTempKit0_Arm1=tcHead2;
                                }

                                if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                {
                                    iTempKit0_Arm2=tcHead3;
                                }
                                else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                {
                                    iTempKit0_Arm2=tcHead4;
                                }
                            }

                            if(bTestSiteUse[0][i][j]==true)
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                            }
                            else
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTestSiteUse[1][i][j]==true)
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                            }
                            else
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTransfer==true)
                            {
                                if(TestIF_File.iSiteMap[i][j]==iSite)
                                {
                                    if(iArm==1)
                                    {
                                        iHead=iTempKit0_Arm1;
                                    }
                                    else if(iArm==2)
                                    {
                                        iHead=iTempKit0_Arm2;
                                    }
                                }
                            }
                        }
                        //QQ 20230214 : 2x1
                        else if(TestIF_File.iTestMode==QualSite2X2)
                        {
                            if(USE_16_HEATER==eht16Heater       ||              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                               USE_16_HEATER==eht16HeaterEJ1N   ||
                               USE_16_HEATER==eht32HeaterEJ1N   ||              //Steven 20140923 : Index使用EJ1N版32組加熱器
                               USE_16_HEATER==eht32HeaterKT4H   ||              //Steven 20150211 : Index使用KT4H版32組加熱器
                               USE_16_HEATER==eht16HeaterDTME08 ||              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                               USE_16_HEATER==eht32HeaterDTME08 )               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                            {
                                if(TestIF_File.bSquare_OctalKit)                //JerryYang 20181003 2x2 用8 site kit要轉換
                                {
                                    if(iTempKit0_Arm1==tcAa1)
                                        iTempKit0_Arm1=tcAb1;
                                    else if(iTempKit0_Arm1==tcAb1)
                                        iTempKit0_Arm1=tcAc1;
                                    else if(iTempKit0_Arm1==tcBa1)
                                        iTempKit0_Arm1=tcBb1;
                                    else if(iTempKit0_Arm1==tcBb1)
                                        iTempKit0_Arm1=tcBc1;

                                    if(iTempKit0_Arm2==tcAa2)
                                        iTempKit0_Arm2=tcAb2;
                                    else if(iTempKit0_Arm2==tcAb2)
                                        iTempKit0_Arm2=tcAc2;
                                    else if(iTempKit0_Arm2==tcBa2)
                                        iTempKit0_Arm2=tcBb2;
                                    else if(iTempKit0_Arm2==tcBb2)
                                        iTempKit0_Arm2=tcBc2;
                                }
                            }
                            else
                            {
                                if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcBa1)
                                {
                                    iTempKit0_Arm1=tcHead1;
                                }
                                else if(iTempKit0_Arm1==tcAb1 || iTempKit0_Arm1==tcBb1)
                                {
                                    iTempKit0_Arm1=tcHead2;
                                }

                                if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcBa2)
                                {
                                    iTempKit0_Arm2=tcHead3;
                                }
                                else if(iTempKit0_Arm2==tcAb2 || iTempKit0_Arm2==tcBb2)
                                {
                                    iTempKit0_Arm2=tcHead4;
                                }
                            }

                            if(bTestSiteUse[0][i][j]==true)
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                            }
                            else
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTestSiteUse[1][i][j]==true)
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                            }
                            else
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }
                            if(bTransfer==true)
                            {
                                if(TestIF_File.iSiteMap[i][j]==iSite)
                                {
                                    if(iArm==1)
                                    {
                                        iHead=iTempKit0_Arm1;
                                    }
                                    else if(iArm==2)
                                    {
                                        iHead=iTempKit0_Arm2;
                                    }
                                }
                            }
                        }
                        else if(TestIF_File.iTestMode==_8Site2X4 ||
                                TestIF_File.iTestMode==_6Site2X3 ||
                                TestIF_File.iTestMode==TriSite1X3 ||
                                TestIF_File.iTestMode==DualSite ||
                                TestIF.iTestMode==SingleSite)
                        {
                            if(USE_16_HEATER==eht16Heater       ||              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                               USE_16_HEATER==eht16HeaterEJ1N   ||
                               USE_16_HEATER==eht32HeaterEJ1N   ||              //Steven 20140923 : Index使用EJ1N版32組加熱器
                               USE_16_HEATER==eht32HeaterKT4H   ||              //Steven 20150211 : Index使用KT4H版32組加熱器
                               USE_16_HEATER==eht16HeaterDTME08 ||              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                               USE_16_HEATER==eht32HeaterDTME08 )               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                            {
                                if(TestIF_File.iTestMode==_8Site2X4)
                                {
                                    if(TestIF_File.bNS8000CS==true)
                                    {
                                        if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1 || iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                        {
                                            iTempKit0_Arm1=tcAa1;
                                        }
                                        else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1 || iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                        {
                                            iTempKit0_Arm1=tcAb1;
                                        }

                                        if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2 || iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                        {
                                            iTempKit0_Arm2=tcAa2;
                                        }
                                        else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2 || iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                        {
                                            iTempKit0_Arm2=tcAb2;
                                        }
                                    }
                                    else if(TestIF_File.dSiteXPitch<=30)
                                    {
                                        //     a  b  c  d     一組加熱棒對2 site
                                        //   **************
                                        //  A* Aa  *  Ab  *
                                        //   **************
                                        //  B* Ba  *  Bb  *
                                        //   **************
                                        if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                        {
                                            iTempKit0_Arm1=tcAa1;
                                        }
                                        else if(iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                        {
                                            iTempKit0_Arm1=tcBa1;
                                        }
                                        else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                        {
                                            iTempKit0_Arm1=tcAb1;
                                        }
                                        else if(iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                        {
                                            iTempKit0_Arm1=tcBb1;
                                        }

                                        if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                        {
                                            iTempKit0_Arm2=tcAa2;
                                        }
                                        else if(iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                        {
                                            iTempKit0_Arm2=tcBa2;
                                        }
                                        else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                        {
                                            iTempKit0_Arm2=tcAb2;
                                        }
                                        else if(iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                        {
                                            iTempKit0_Arm2=tcBb2;
                                        }
                                    }
                                    else if(TestIF_File.bOctal_12Kit &&
                                            (USE_16_HEATER==eht32HeaterEJ1N ||
                                             USE_16_HEATER==eht32HeaterKT4H ||
                                             USE_16_HEATER==eht32HeaterDTME08) &&
                                            IniConfig.bSPILFunction==true)
                                    {
                                        if(iTempKit0_Arm1==tcAa1)
                                            iTempKit0_Arm1=tcAb1;
                                        else if(iTempKit0_Arm1==tcAb1)
                                            iTempKit0_Arm1=tcAc1;
                                        else if(iTempKit0_Arm1==tcAc1)
                                            iTempKit0_Arm1=tcAd1;
                                        else if(iTempKit0_Arm1==tcAd1)
                                            iTempKit0_Arm1=tcAe1;
                                        else if(iTempKit0_Arm1==tcBa1)
                                            iTempKit0_Arm1=tcBb1;
                                        else if(iTempKit0_Arm1==tcBb1)
                                            iTempKit0_Arm1=tcBc1;
                                        else if(iTempKit0_Arm1==tcBc1)
                                            iTempKit0_Arm1=tcBd1;
                                        else if(iTempKit0_Arm1==tcBd1)
                                            iTempKit0_Arm1=tcBe1;

                                        if(iTempKit0_Arm2==tcAa2)
                                            iTempKit0_Arm2=tcAb2;
                                        else if(iTempKit0_Arm2==tcAb2)
                                            iTempKit0_Arm2=tcAc2;
                                        else if(iTempKit0_Arm2==tcAc2)
                                            iTempKit0_Arm2=tcAd2;
                                        else if(iTempKit0_Arm2==tcAd2)
                                            iTempKit0_Arm2=tcAe2;
                                        else if(iTempKit0_Arm2==tcBa2)
                                            iTempKit0_Arm2=tcBb2;
                                        else if(iTempKit0_Arm2==tcBb2)
                                            iTempKit0_Arm2=tcBc2;
                                        else if(iTempKit0_Arm2==tcBc2)
                                            iTempKit0_Arm2=tcBd2;
                                        else if(iTempKit0_Arm2==tcBd2)
                                            iTempKit0_Arm2=tcBe2;
                                    }
                                    else if(TestIF_File.bOctal_16Kit &&
                                            (USE_16_HEATER==eht32HeaterEJ1N ||
                                             USE_16_HEATER==eht32HeaterKT4H ||
                                             USE_16_HEATER==eht32HeaterDTME08)) //JerryYang 20230814 : fix 2X4 mode使用2X8 SLK, 收到Remote temperature溫度offset補錯位置的問題
                                    {
                                        if(iTempKit0_Arm1==tcAa1)
                                            iTempKit0_Arm1=tcAc1;
                                        else if(iTempKit0_Arm1==tcAb1)
                                            iTempKit0_Arm1=tcAd1;
                                        else if(iTempKit0_Arm1==tcAc1)
                                            iTempKit0_Arm1=tcAe1;
                                        else if(iTempKit0_Arm1==tcAd1)
                                            iTempKit0_Arm1=tcAf1;
                                        else if(iTempKit0_Arm1==tcBa1)
                                            iTempKit0_Arm1=tcBc1;
                                        else if(iTempKit0_Arm1==tcBb1)
                                            iTempKit0_Arm1=tcBd1;
                                        else if(iTempKit0_Arm1==tcBc1)
                                            iTempKit0_Arm1=tcBe1;
                                        else if(iTempKit0_Arm1==tcBd1)
                                            iTempKit0_Arm1=tcBf1;

                                        if(iTempKit0_Arm2==tcAa2)
                                            iTempKit0_Arm2=tcAc2;
                                        else if(iTempKit0_Arm2==tcAb2)
                                            iTempKit0_Arm2=tcAd2;
                                        else if(iTempKit0_Arm2==tcAc2)
                                            iTempKit0_Arm2=tcAe2;
                                        else if(iTempKit0_Arm2==tcAd2)
                                            iTempKit0_Arm2=tcAf2;
                                        else if(iTempKit0_Arm2==tcBa2)
                                            iTempKit0_Arm2=tcBc2;
                                        else if(iTempKit0_Arm2==tcBb2)
                                            iTempKit0_Arm2=tcBd2;
                                        else if(iTempKit0_Arm2==tcBc2)
                                            iTempKit0_Arm2=tcBe2;
                                        else if(iTempKit0_Arm2==tcBd2)
                                            iTempKit0_Arm2=tcBf2;
                                    }
                                }
                            }
                            else
                            {
                                if(TestIF_File.iTestMode==_8Site2X4)
                                {
                                    if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1 || iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                    {
                                        iTempKit0_Arm1=tcHead1;
                                    }
                                    else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1 || iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                    {
                                        iTempKit0_Arm1=tcHead2;
                                    }

                                    if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2 || iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                    {
                                        iTempKit0_Arm2=tcHead3;
                                    }
                                    else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2 || iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                    {
                                        iTempKit0_Arm2=tcHead4;
                                    }
                                }
                                else if(TestIF_File.iTestMode==DualSite || TestIF_File.iTestMode==SingleSite)
                                {
                                    if(iTempKit0_Arm1==tcAa1)
                                    {
                                        iTempKit0_Arm1=tcHead1;
                                    }
                                    else if(iTempKit0_Arm1==tcAb1)
                                    {
                                        iTempKit0_Arm1=tcHead2;
                                    }

                                    if(iTempKit0_Arm2==tcAa2)
                                    {
                                        iTempKit0_Arm2=tcHead3;
                                    }
                                    else if(iTempKit0_Arm2==tcAb2)
                                    {
                                        iTempKit0_Arm2=tcHead3;
                                    }
                                }
                            }

                            if(bTestSiteUse[0][i][j]==true || bSendTmp)         //RogerYand 20251105 : 偉測要求L17開啟，關site也要上拋溫度給測試機(先用在2X4測試)
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                            }
                            else
                            {
                                asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTestSiteUse[1][i][j]==true|| bSendTmp)          //RogerYand 20251105 : 偉測要求L17開啟，關site也要上拋溫度給測試機(先用在2X4測試)
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                            }
                            else
                            {
                                asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                            }

                            if(bTransfer==true)
                            {
                                if(TestIF_File.iSiteMap[i][j]==iSite)
                                {
                                    if(iArm==1)
                                    {
                                        iHead=iTempKit0_Arm1;
                                    }
                                    else if(iArm==2)
                                    {
                                        iHead=iTempKit0_Arm2;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(TestIF_File.iTestMode==_10Site2X5 ||             //Steven 20220104 : fixed for 2x5mode
                               TestIF_File.iTestMode==_12Site2X6 ||
                               TestIF_File.iTestMode==_16Site2X8)
                            {
                                if(USE_16_HEATER==eht16Heater       ||          //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                                   USE_16_HEATER==eht16HeaterEJ1N   ||
                                   USE_16_HEATER==eht32HeaterEJ1N   ||          //Steven 20140923 : Index使用EJ1N版32組加熱器
                                   USE_16_HEATER==eht32HeaterKT4H   ||          //Steven 20150211 : Index使用KT4H版32組加熱器
                                   USE_16_HEATER==eht16HeaterDTME08 ||          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                                   USE_16_HEATER==eht32HeaterDTME08 )           //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                                {
//                                  tcAa1=11, tcAb1=12, tcAc1=13, tcAd1=14, tcBa1=15, tcBb1=16, tcBc1=17, tcBd1=18,
//                                  tcAe1=33, tcAf1=34, tcAg1=35, tcAh1=36, tcBe1=37, tcBf1=38, tcBg1=39, tcBh1=40,

//                                  tcAa2=19, tcAb2=20, tcAc2=21, tcAd2=22, tcBa2=23, tcBb2=24, tcBc2=25, tcBd2=26,
//                                  tcAe2=41, tcAf2=42, tcAg2=43, tcAh2=44, tcBe2=45, tcBf2=46, tcBg2=47, tcBh2=48,

                                    if(j<4)
                                    {
                                        if((USE_16_HEATER==eht16Heater ||
                                            USE_16_HEATER==eht16HeaterEJ1N ||
                                            USE_16_HEATER==eht16HeaterDTME08) || //JerryYang 20181002 1組加熱棒對應2個site
                                           ((USE_16_HEATER==eht32HeaterEJ1N ||
                                             USE_16_HEATER==eht32HeaterKT4H ||
                                             USE_16_HEATER==eht32HeaterDTME08) &&   //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                                             TestIF_File.bUse32Heater==false) ||
                                           (ATC_SYSTEM==eNewATCSystem && Temperature.bATCActiveCooling==true && TestIF_File.bUse32Heater==false))
                                        {
                                            if(iTempKit0_Arm1==tcAa1 || iTempKit0_Arm1==tcAb1)
                                            {
                                                iTempKit0_Arm1=tcAa1;
                                            }
                                            else if(iTempKit0_Arm1==tcAc1 || iTempKit0_Arm1==tcAd1)
                                            {
                                                iTempKit0_Arm1=tcAb1;
                                            }
                                            else if(iTempKit0_Arm1==tcBa1 || iTempKit0_Arm1==tcBb1)
                                            {
                                                iTempKit0_Arm1=tcBa1;
                                            }
                                            else if(iTempKit0_Arm1==tcBc1 || iTempKit0_Arm1==tcBd1)
                                            {
                                                iTempKit0_Arm1=tcBb1;
                                            }

                                            if(iTempKit0_Arm2==tcAa2 || iTempKit0_Arm2==tcAb2)
                                            {
                                                iTempKit0_Arm2=tcAa2;
                                            }
                                            else if(iTempKit0_Arm2==tcAc2 || iTempKit0_Arm2==tcAd2)
                                            {
                                                iTempKit0_Arm2=tcAb2;
                                            }
                                            else if(iTempKit0_Arm2==tcBa2 || iTempKit0_Arm2==tcBb2)
                                            {
                                                iTempKit0_Arm2=tcBa2;
                                            }
                                            else if(iTempKit0_Arm2==tcBc2 || iTempKit0_Arm2==tcBd2)
                                            {
                                                iTempKit0_Arm2=tcBb2;
                                            }
                                        }

                                        if(bTestSiteUse[0][i][j]==true)
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm1];
                                        }
                                        else
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTestSiteUse[1][i][j]==true)
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit0_Arm2];
                                        }
                                        else
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTransfer==true)
                                        {
                                            if(TestIF_File.iSiteMap[i][j]==iSite)
                                            {
                                                if(iArm==1)
                                                {
                                                    iHead=iTempKit0_Arm1;
                                                }
                                                else if(iArm==2)
                                                {
                                                    iHead=iTempKit0_Arm2;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        j2=j-4;
                                        iTempKit1_Arm1=tcAe1+(i*4+j2);
                                        iTempKit1_Arm2=tcAe2+(i*4+j2);

                                        if((USE_16_HEATER==eht16Heater ||
                                            USE_16_HEATER==eht16HeaterEJ1N ||
                                            USE_16_HEATER==eht16HeaterDTME08) ||       //JerryYang 20181002 1組加熱棒對應2個site
                                          ((USE_16_HEATER==eht32HeaterEJ1N ||
                                            USE_16_HEATER==eht32HeaterKT4H ||
                                            USE_16_HEATER==eht32HeaterDTME08) &&
                                            TestIF_File.bUse32Heater==false) ||
                                          (ATC_SYSTEM==eNewATCSystem &&
                                           Temperature.bATCActiveCooling==true &&
                                           TestIF_File.bUse32Heater==false))
                                        {
                                            if(iTempKit1_Arm1==tcAe1 || iTempKit1_Arm1==tcAf1)
                                            {
                                                iTempKit1_Arm1=tcAc1;
                                            }
                                            else if(iTempKit1_Arm1==tcAg1 || iTempKit1_Arm1==tcAh1)
                                            {
                                                iTempKit1_Arm1=tcAd1;
                                            }
                                            else if(iTempKit1_Arm1==tcBe1 || iTempKit1_Arm1==tcBf1)
                                            {
                                                iTempKit1_Arm1=tcBc1;
                                            }
                                            else if(iTempKit1_Arm1==tcBg1 || iTempKit1_Arm1==tcBh1)
                                            {
                                                iTempKit1_Arm1=tcBd1;
                                            }

                                            if(iTempKit1_Arm2==tcAe2 || iTempKit1_Arm2==tcAf2)
                                            {
                                                iTempKit1_Arm2=tcAc2;
                                            }
                                            else if(iTempKit1_Arm2==tcAg2 || iTempKit1_Arm2==tcAh2)
                                            {
                                                iTempKit1_Arm2=tcAd2;
                                            }
                                            else if(iTempKit1_Arm2==tcBe2 || iTempKit1_Arm2==tcBf2)
                                            {
                                                iTempKit1_Arm2=tcBc2;
                                            }
                                            else if(iTempKit1_Arm2==tcBg2 || iTempKit1_Arm2==tcBh2)
                                            {
                                                iTempKit1_Arm2=tcBd2;
                                            }
                                        }

                                        if(bTestSiteUse[0][i][j]==true)
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit1_Arm1];
                                        }
                                        else
                                        {
                                            asTempArmOrder[0][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTestSiteUse[1][i][j]==true)
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asGPIBTempShow[iTempKit1_Arm2];
                                        }
                                        else
                                        {
                                            asTempArmOrder[1][TestIF_File.iSiteMap[i][j]-1]=asNULL;
                                        }

                                        if(bTransfer==true)
                                        {
                                            if(TestIF_File.iSiteMap[i][j]==iSite)
                                            {
                                                if(iArm==1)
                                                {
                                                    iHead=iTempKit1_Arm1;
                                                }
                                                else if(iArm==2)
                                                {
                                                    iHead=iTempKit1_Arm2;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //QQ 20230214 : 4x4
                        //QQ 20230214 : 4x8
                    }
                }
            }
        }
    }
    return iHead;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WritePERSITETemperature()                               //JerryYang 20181003  (Steven) : 回傳各Site溫度改為依照Site Map順序回覆
{
    AnsiString sRet="";
    sRet=PERSITETemperatureStrings();
    if(TestIF.iTestType==TCP_IP_MODE)                                           //wei 20211027 open short TCP/IP
        asTCPIPTemperature.sprintf("%s\r", sRet);                               //Sam 20231205 : 修正 NOVATEK 指令
    else
        SendMSG_CMD(MSG_CMD_TempArm, sRet+"\r");                                //Sam 20231205 : 修正 NOVATEK 指令
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfMain::PERSITETemperatureStrings()                       //Sam 20220408 : Novatek 新增 SET_ALL?
{
    //Head1 //Head2 //Head5 //Head6
    AnsiString t="";
    AnsiString asSite = "NULL";                                                 //Ifor 20160130 Add 回覆字串改變數處理
    int iArm=-1;
    AnsiString Str="";

    RefreshTempData();

    //2013.01.24 Q_Q TSMC GPIB COMMAND Part 2. {
    if(fContact->fShow)                                                         //Ifor 20240612 add:避免Contact Mode 資料回覆錯誤
    {
        if(iWhichArmDown==1)
        {
            iArm=0;
            Str="iWhichArmDown==1";
        }
        else if(iWhichArmDown==2)
        {
            iArm=1;
            Str="iWhichArmDown==2";
        }
        else
        {
            iArm=-1;
            Str="iArm=-1(fContact)";
        }
    }
    else
    {
        if(IndexStatus==Z1Down_Z2Up || (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            iArm=0;
            Str="Z1Down_Z2Up";
        }
        else if(IndexStatus==Z1Up_Z2Down || (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            iArm=1;
            Str="Z1Up_Z2Down";
        }
        else if(IndexStatus==Z1_Z2_Down || (iContactMode!=CONTACT_NORMAL && iIndexArm==0))  //JerryYang 20201101 : 修正GPIB傳輸溫度命令異常
        {
            iArm=0;
            Str="Z1_Z2_Down";
        }
        else
        {
            iArm=-1;
            Str="iArm=-1";
        }
    }

    if(TestIF_File.iTestMode==SingleSite)                                       //Ifor 20160130 add 海思 SingleSite GPIB溫度上報格式
    {
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="SINGLESITE";                                                //Ifor 20160310 後面無site需加上"_"+溫度
        }
        else
        {
            asSite="SINGLESITE_";
        }

        if(iArm==-1)
        {
            t.sprintf("%sNULL_", asSite);                                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s%s_", asSite, asTempArmOrder[iArm][0]);                //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==DualSite)                                    // Dual Site 1x2
    {                                                                           //Ifor 20160130 add 海思GPIB Dual Site 1x2 資料格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="DUAL1X2";
        }
        else
        {
            asSite="DUAL_1X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_", asSite);                                 //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_", asSite,
                                   asTempArmOrder[iArm][0],
                                   asTempArmOrder[iArm][1]);                    //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {                                                                           //wei 20160512 add TSMC 1X3 TempArm
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="TRI1X3";
        }
        else
        {
            asSite="TRI_1X3";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_", asSite);                            //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_", asSite,
                                        asTempArmOrder[iArm][0],
                                        asTempArmOrder[iArm][1],
                                        asTempArmOrder[iArm][2]);               //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==QualSite1X4)                                 // Qual Site 1x4
    {                                                                           //Ifor 20160130 add Qual Site 1x4 海思GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="QUAD1X4";
        }
        else
        {
            asSite="QUAD_1X4";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_", asSite);                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_", asSite,
                                           asTempArmOrder[iArm][0],
                                           asTempArmOrder[iArm][1],
                                           asTempArmOrder[iArm][2],
                                           asTempArmOrder[iArm][3]);            //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==DualSite2x1)                                 //wei 20170417 (Steven) : Fixed for Dual Site 2x1
    {                                                                           //Ifor 20160130 add 海思GPIB Dual Site 1x2 資料格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="DUAL2X1";
        }
        else
        {
            asSite="DUAL_2X1";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_", asSite);                                 //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_", asSite,
                                   asTempArmOrder[iArm][0],
                                   asTempArmOrder[iArm][1]);                    //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==QualSite2X2)                                 // Qual Site 2x2
    {                                                                           //Ifor 20160130 add 海思 Qual Site 2x2 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="QUAD2X2";
        }
        else
        {
            asSite="QUAD_2X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_", asSite);                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_", asSite,
                                           asTempArmOrder[iArm][0],
                                           asTempArmOrder[iArm][1],
                                           asTempArmOrder[iArm][2],
                                           asTempArmOrder[iArm][3]);            //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)
    {                                                                           //Ifor 20160130 add 海思 Qual Site 2x2 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="QUAD2X2";
            else
                asSite="QUAD2X2N";
        }
        else
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="QUAD_2X2";
            else
                asSite="QUAD_2X2N";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_", asSite);                       //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_\r", asSite,
                                           asTempArmOrder[0][0],
                                           asTempArmOrder[0][1],
                                           asTempArmOrder[0][2],
                                           asTempArmOrder[0][3]);               //Steven 20231018 : Fixed for 2x2 NN mode
        }
    }
    else if(TestIF_File.iTestMode==_6Site2X3)
    {                                                                           //Ifor 20160130 add 海思 6 Site 2X3 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="6SITE2X3";
        }
        else
        {
            asSite="6SITE_2X3";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);             //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_", asSite,
                                                 asTempArmOrder[iArm][0],
                                                 asTempArmOrder[iArm][1],
                                                 asTempArmOrder[iArm][2],
                                                 asTempArmOrder[iArm][3],
                                                 asTempArmOrder[iArm][4],
                                                 asTempArmOrder[iArm][5]);      //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="6SITE2X3";
            else
                asSite="6SITE2X3N";
        }
        else
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="6SITE_2X3";
            else
                asSite="6SITE_2X3N";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);             //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_", asSite,
                                                 asTempArmOrder[0][0],
                                                 asTempArmOrder[0][1],
                                                 asTempArmOrder[0][2],
                                                 asTempArmOrder[0][3],
                                                 asTempArmOrder[0][4],
                                                 asTempArmOrder[0][5]);         //Sam 20231205 : 修正 NOVATEK 指令 //Steven 20231018 : Fixed for 2x2 NN mode
        }
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="8SITE2X4";
            else
                asSite="8SITE2X4N";
        }
        else
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20250926 : JSCK要求修改命令
                asSite="8SITE_2X4";
            else
                asSite="8SITE_2X4N";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_", asSite,
                                                     asTempArmOrder[0][0],
                                                     asTempArmOrder[0][1],
                                                     asTempArmOrder[0][2],
                                                     asTempArmOrder[0][3],
                                                     asTempArmOrder[0][4],
                                                     asTempArmOrder[0][5],
                                                     asTempArmOrder[0][6],
                                                     asTempArmOrder[0][7]);     //Sam 20231205 : 修正 NOVATEK 指令 //Steven 20231018 : Fixed for 2x2 NN mode
        }
    }
    else if(TestIF_File.iTestMode==_8Site2X4)                                   // 8 Site 2x4
    {                                                                           //Ifor 20160130 add 海思 8 Site 2X4 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="8SITE2X4";
        }
        else
        {
            asSite="8SITE_2X4";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
            NewRecordProcess("", "GetNULLTemp!", Str);                          //RogerYang 20250924 : 紀錄Log
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7]);                               //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
    {                                                                           //Ifor 20160130 add 海思 12 Site 2X6 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="10SITE5X2";
        }
        else
        {
            asSite="10SITE_5X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite); //Sam 20231205 : 修正 NOVATEK 指令    //Steven 20220104 : fixed for 2x5mode
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite ,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7],
                        asTempArmOrder[iArm][8],
                        asTempArmOrder[iArm][9]);                               //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //12 Site //wei 20150702
    {                                                                           //Ifor 20160130 add 海思 12 Site 2X6 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="12SITE6X2";
        }
        else
        {
            asSite="12SITE_6X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite ,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7],
                        asTempArmOrder[iArm][8],
                        asTempArmOrder[iArm][9],
                        asTempArmOrder[iArm][10],
                        asTempArmOrder[iArm][11]);                              //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16 Site
    {                                                                           //Ifor 20160130 add 海思 16 Site 2X8 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="16SITE8X2";
        }
        else
        {
            asSite="16SITE_8X2";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[iArm][0],
                        asTempArmOrder[iArm][1],
                        asTempArmOrder[iArm][2],
                        asTempArmOrder[iArm][3],
                        asTempArmOrder[iArm][4],
                        asTempArmOrder[iArm][5],
                        asTempArmOrder[iArm][6],
                        asTempArmOrder[iArm][7],
                        asTempArmOrder[iArm][8],
                        asTempArmOrder[iArm][9],
                        asTempArmOrder[iArm][10],
                        asTempArmOrder[iArm][11],
                        asTempArmOrder[iArm][12],
                        asTempArmOrder[iArm][13],
                        asTempArmOrder[iArm][14],
                        asTempArmOrder[iArm][15]);                              //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_16Site4X4)                                  //16 Site
    {                                                                           //Ifor 20160130 add 海思 16 Site 2X8 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="16SITE4X4";
        }
        else
        {
            asSite="16SITE_4X4";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[1][0],
                        asTempArmOrder[1][1],
                        asTempArmOrder[0][0],
                        asTempArmOrder[0][1],
                        asTempArmOrder[1][2],
                        asTempArmOrder[1][3],
                        asTempArmOrder[0][2],
                        asTempArmOrder[0][3],
                        asTempArmOrder[1][4],
                        asTempArmOrder[1][5],
                        asTempArmOrder[0][4],
                        asTempArmOrder[0][5],
                        asTempArmOrder[1][6],
                        asTempArmOrder[1][7],
                        asTempArmOrder[0][6],
                        asTempArmOrder[0][7]);                                  //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {                                                                           //Ifor 20160130 add 海思 16 Site 2X8 GPIB溫度上報格式
        if(IniConfig.iI25UseGPIBFormat==1)                                      //Steven 20160301 : 改為海思格式
        {
            asSite="32SITE4X8";
        }
        else
        {
            asSite="32SITE_4X8";
        }

        if(iArm==-1)
        {
            t.sprintf("%s_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_NULL_", asSite);   //Sam 20231205 : 修正 NOVATEK 指令
        }
        else
        {
            t.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_%s_",
                        asSite,
                        asTempArmOrder[1][0],
                        asTempArmOrder[1][1],
                        asTempArmOrder[0][0],
                        asTempArmOrder[0][1],
                        asTempArmOrder[1][2],
                        asTempArmOrder[1][3],
                        asTempArmOrder[0][2],
                        asTempArmOrder[0][3],
                        asTempArmOrder[1][4],
                        asTempArmOrder[1][5],
                        asTempArmOrder[0][4],
                        asTempArmOrder[0][5],
                        asTempArmOrder[1][6],
                        asTempArmOrder[1][7],
                        asTempArmOrder[0][6],
                        asTempArmOrder[0][7],
                        asTempArmOrder[1][8],
                        asTempArmOrder[1][9],
                        asTempArmOrder[0][8],
                        asTempArmOrder[0][9],
                        asTempArmOrder[1][10],
                        asTempArmOrder[1][11],
                        asTempArmOrder[0][10],
                        asTempArmOrder[0][11],
                        asTempArmOrder[1][12],
                        asTempArmOrder[1][13],
                        asTempArmOrder[0][12],
                        asTempArmOrder[0][13],
                        asTempArmOrder[1][14],
                        asTempArmOrder[1][15],
                        asTempArmOrder[0][14],
                        asTempArmOrder[0][15]);                                 //Sam 20231205 : 修正 NOVATEK 指令
        }
    }
    else
    {
        t.sprintf("UNKNOWN\r");
    }

    return t;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteHandlerID()                                        //kevin 20130425
{
    SendMSG_CMD(MSG_CMD_HandlerID, IniConfig.sGPIBMachineID);
}
//------------------kevin 20130425-------------------------------------------
void __fastcall TfMain::WriteArmStatus()
{
    AnsiString sRet="";
    sRet=ArmStatusStrings();
    sRet+="\r";                                                                 //Sam 20231205 : 修正 NOVATEK 指令
    SendMSG_CMD(MSG_CMD_TestArm, sRet);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfMain::ArmStatusStrings()
{
    AnsiString t;
    if(IndexStatus==Z1_Z2_Down ||                                               //Steven 20230214 : Add GPIB for NN mode
       IndexStatus==Z1Down_Z2Up || iContactMode==CONTACT_TEST && iIndexArm==0)  //kevin 20310605 add contract mode
        t=(IniConfig.bA10_6_HANA_ART_TestMode_Enable)?"F":"1";                  //Sam 20231205 : 修正 NOVATEK 指令
    else if(IndexStatus==Z1Up_Z2Down || iContactMode==CONTACT_TEST && iIndexArm==1) //kevin 20310605 add contract mode
        t=(IniConfig.bA10_6_HANA_ART_TestMode_Enable)?"B":"2";                  //Sam 20231205 : 修正 NOVATEK 指令
    else
        t=(IniConfig.bA10_6_HANA_ART_TestMode_Enable)?"F":"0";                  //Sam 20231205 : 修正 NOVATEK 指令 //Steven 20250414 : HANA ART Function
    return t;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteArmForce()
{
    AnsiString t;

    if(IniConfig.bKoreaFunction==true)
    {
        t.sprintf("%s\r", asArmForce1.c_str());       //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else if(CUSTOMER_CODE==CC_TSMC_TAINAN)             //wei 20151117 傳送ContactForce設定值
    {
        t.sprintf("%sT\r", asArmForce1.c_str());                                //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else
    {
        if(IndexStatus==Z1_Z2_Down ||                                               //Steven 20230214 : Add GPIB for NN mode
           IndexStatus==Z1Down_Z2Up || iContactMode==CONTACT_TEST && iIndexArm==0)  //kevin 20310605 add contract mode
        {
            t.sprintf("%s\r", asArmForce1.c_str());                             //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else if(IndexStatus==Z1Up_Z2Down || iContactMode==CONTACT_TEST && iIndexArm==1)    //kevin 20310605 add contract mode
        {
            t.sprintf("%s\r", asArmForce2.c_str());                             //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else
        {
            //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
            t.sprintf("%d\r", 0);
        }
    }

    SendMSG_CMD(MSG_CMD_Force, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteTempData()                                         //Steven 20110613 : Corn GPIB add from 7040
{
    AnsiString t;
    t=TempDataStrings();
    t+=" \r\n";                                                                 //Sam 20231205 : 修正 NOVATEK 指令
    SendMSG_CMD(MSG_CMD_HandlerTemperature, t);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfMain::TempDataStrings()                                 //Sam 20220408 : Novatek 新增 SET_ALL?
{
    AnsiString t;
    if(IniConfig.iI38SETTEMPRespondSetTemp==1)                                  //kevin 20180308 Settemp? 回傳需要 Settemp +25.0.
    {
        if(LastSet.iTemperature==Tempture_Ambient)                              //kevin 20180320 add 避免資料轉換異常
            t.sprintf("Settemp +%0.1f", Temperature.fAbitTemp);                 //Sam 20231205 : 修正 NOVATEK 指令
        else
            t.sprintf("Settemp +%s.0", edWorkTemperBase->Text.c_str());         //Sam 20231205 : 修正 NOVATEK 指令
    }
    else if(IniConfig.iI38SETTEMPRespondSetTemp==2)                             //Steven 20250701 : for DOOSAN TESNA
    {
        if(LastSet.iTemperature==Tempture_Ambient)                              //kevin 20180320 add 避免資料轉換異常
            t.sprintf("%d", int(Temperature.fAbitTemp));                        //Sam 20231205 : 修正 NOVATEK 指令
        else
            t.sprintf("%d", atoi(edWorkTemperBase->Text.c_str()));              //Sam 20231205 : 修正 NOVATEK 指令
    }
    else
    {
        if(LastSet.iTemperature==Tempture_Ambient)
            t.sprintf("+25.0");                                                 //Sam 20231205 : 修正 NOVATEK 指令  //kevin 20180305 strncpy(t, "+25.0", sizeof(t));
        else
            t.sprintf("+%s.0", edWorkTemperBase->Text.c_str());                 //Sam 20231205 : 修正 NOVATEK 指令  //kevin 20180305 sprintf(t, "+%s.0", edWorkTemperBase->Text.c_str(), t);
    }
    return t;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSetTempStatus()                                    //Steven 20110613 : Corn GPIB add from 7040
{
    int ret=-1, i, TempMode;
    AnsiString t;

    try                                                                         //kevin 20180320 add 避免資料轉換異常
    {
        i=atoi(HGpib2Handler->cReturn);
    }
    catch(...)
    {
        i=25;
        MyDBIProcess("Exception", "TfMain::WriteSetTempStatus");
    }

    if(IniConfig.iI38SETTEMPRespondSetTemp==1)                                  //kevin 20180308 Settemp? 回傳需要 Settemp +25.0.
    {
        if(LastSet.iTemperature==Tempture_Ambient)                              //kevin 20180320 add 避免資料轉換異常
        {
            if(i>50)
            {
                TempMode=1;
                Temperature.fAbitTemp=30;
            }
            else
            {
                TempMode=0;
            }
            ret=SetTemp(false, Temperature.fAbitTemp, atof(edSoakTime->Text.c_str()));
        }
        else
        {
            ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));
        }

        if(ret==0)
            ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);
    }
    else
    {
        if(i>25)
        {
            TempMode=1;
            edWorkTemperBase->Text=i;
        }
        else
        {
            TempMode=0;
        }
        ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));
    }

    if(ret==0)
        ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);

    if(ret==0)
        t.sprintf("SETTINGOK");
    else
        t.sprintf("SETTINGNG");

    SendMSG_CMD(MSG_CMD_SetTemp, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSetTestTempStatus()                                //Ifor 20210623 : //Ifor 20210623 add: Test Temp Change
{
    int iSetTemp;
    AnsiString t;

    try                                                                         //kevin 20180320 add 避免資料轉換異常
    {
        iSetTemp=atoi(HGpib2Handler->cReturn);
    }
    catch(...)
    {
        iSetTemp=0;
    }

    double dbSetATCTemp=0;
    int iNowDownArm=0;

    if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
        dbSetATCTemp=Temperature.fWorkTemperBase;
    else
        dbSetATCTemp=IniConfig.dATCAmbientTemperature;

    if(fContact->fShow)                                                         //Ifor 20240612 add:避免Contact Mode 資料回覆錯誤
    {
        if(iWhichArmDown==1)
        {
            iNowDownArm=Z1Down_Z2Up;
        }
        else if(iWhichArmDown==2)
        {
            iNowDownArm=Z1Up_Z2Down;
        }
    }
    else
    {
        iNowDownArm=IndexStatus;
    }

    if(iSetTemp<25)
    {
        t.sprintf("SETTINGNG");
    }
    else
    {
        bChangeTest_TempOffset=iSetTemp-dbSetATCTemp;

        if(bChangeTest_TempOffset!=0)
        {
            bChangeTest_TempAlarm=true;
            if(iNowDownArm==Z1Down_Z2Up)    //ARM1 在下
            {
                if(iATC_Use_Heat_Count<=4)
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp);
                }
                else
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp+bChangeTest_TempOffset);

                    ATC_InterfaceForm->SetSingleTemp(4,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(5,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(6,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(7,dbSetATCTemp);
                }
                t.sprintf("SETTINGOK");
            }
            else if(iNowDownArm==Z1Up_Z2Down)
            {
                if(iATC_Use_Heat_Count<=4)
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp+bChangeTest_TempOffset);
                }
                else
                {
                    ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp);
                    ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp);

                    ATC_InterfaceForm->SetSingleTemp(4,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(5,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(6,dbSetATCTemp+bChangeTest_TempOffset);
                    ATC_InterfaceForm->SetSingleTemp(7,dbSetATCTemp+bChangeTest_TempOffset);
                }
                t.sprintf("SETTINGOK");
            }
            else
            {
                ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
                bChangeTest_TempOffset=0;                                       //Ifor 20230505 add:清除資料
                bChangeTest_TempAlarm=false;                                    //Ifor 20230505 add:清除資料
                t.sprintf("SETTINGNG");
            }
        }
        else
        {
            ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
            bChangeTest_TempOffset=0;                                           //Ifor 20230505 add:清除資料
            bChangeTest_TempAlarm=false;                                        //Ifor 20230505 add:清除資料
            t.sprintf("SETTINGOK");
        }
    }
    fLotInfo->SetATCOffset(true);                                               //WriteSetTestTempStatus
    SendMSG_CMD(MSG_CMD_SetTestTemp, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSoakTimeData()                                     //Steven 20110613 : Corn GPIB add from 7040
{
    AnsiString t;
    if(LastSet.iTemperature==Tempture_Ambient)
        t.sprintf("NONE\r\n");
    else
        t.sprintf("%s\r\n", edSoakTime->Text.c_str());

    SendMSG_CMD(MSG_CMD_HandlerSoakTime, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSetSoakTimeStatus()    //Steven 20110613 : Corn GPIB add from 7040
{
    int i, ret;
    AnsiString t;
    i=atoi(HGpib2Handler->cReturn);
    if(i>=0)
    {
        edSoakTime->Text=i;
        ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));     //Steven 20120730
    }

    if(ret==0)
        t.sprintf("SETTINGOK");
    else
        t.sprintf("SETTINGNG");

    SendMSG_CMD(MSG_CMD_SetSoakTime, t);
}
//---------------------------------------------------------------------------
//void __fastcall TfMain::WriteSiteMapData()          //Steven 20110613 : Corn GPIB add from 7040
AnsiString __fastcall TfMain::WriteSiteMapData(bool bGPIB)    //Sam 20170801 (Steven) 移植超豐 OEE 功能 form HT-7045       //Steven 20110613 : Corn GPIB add from 7040
{
    AnsiString t;
    AnsiString tt;

    if(TestIF_File.iTestMode==SingleSite)       // Single Site 1x1
    {
        t.sprintf("SINGLE1X1-%d_\r\n",
                     TestIF_File.iSiteMap[0][0]);
    }
    else if(TestIF_File.iTestMode==DualSite)       // Dual Site 1x2
    {
        t.sprintf("DUAL1X2-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[0][1]);
    }
    else if(TestIF_File.iTestMode==TriSite1X3)      //Frank 20160329 add for 1x3_4
    {
        t.sprintf("TRI1X3-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[0][2]);
    }
    else if(TestIF_File.iTestMode==QualSite1X4)     // Qual Site 1x4
    {
        t.sprintf("QUAD1X4-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1)  // Dual Site 2x1
    {
        t.sprintf("DUAL2X1-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2)  // Qual Site 2x2
    {
        t.sprintf("QUAD2X2-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)  // Qual Site 2x2 OK
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            tt="QUAD2X2";
        else
            tt="QUAD2X2N";

        t.sprintf("%s-%d-%d-%d-%d_\r\n",
                     tt,
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3)                                   //ChungHung 20140115 add for 2x3_6
    {
        t.sprintf("6SITE2X3-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            tt="6SITE2X3";
        else
            tt="6SITE2X3N";

        t.sprintf("%s-%d-%d-%d-%d-%d-%d_\r\n",
                     tt,
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            tt="8SITE2X4";
        else
            tt="8SITE2X4N";

        t.sprintf("%s-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     tt,
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4)      // 8 Site 2x4
    {
        t.sprintf("8SITE2X4-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
    {
        t.sprintf("10SITE2X5-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",               //wei 20150702 移除4個Site
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        t.sprintf("12SITE2X6-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",         //wei 20150702 移除4個Site
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16Site 2x8
    {
        t.sprintf("16SITE2X8-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        t.sprintf("16SITE4X4-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3]);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        t.sprintf("32SITE4X8-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d_\r\n",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[2][4],
                     TestIF_File.iSiteMap[3][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[2][5],
                     TestIF_File.iSiteMap[3][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[2][6],
                     TestIF_File.iSiteMap[3][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7],
                     TestIF_File.iSiteMap[2][7],
                     TestIF_File.iSiteMap[3][7]);
    }
    else
    {
        t.sprintf("UNKNOWN\r\n", sizeof(t));
    }

    if(bGPIB)
        SendMSG_CMD(MSG_CMD_HandlerSiteMap, t);

    t=StringReplace(t, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
    return t;                                                                   //Sam 20170801 (Steven) 移植超豐 OEE 功能 form HT-7045
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteStartMode_NS()
{
    // 01: Initital Start
    // 02: Continuous start
    // 03: Restest Initial start
    // 04: Retest Continuous Restest

    AnsiString t;
    int mode=0;

    if(LastSet.iRunStartMode==rsmNull)
        mode=0;
    else if(LastSet.iRunStartMode==rsmInitialStart)
        mode=1;
    else if(LastSet.iRunStartMode==rsmContinuStart)
        mode=2;
    else if(LastSet.iRunStartMode==rsmCInitialRetest)
        mode=3;
    else if(LastSet.iRunStartMode==rsmContinuRetest)
        mode=4;
    else if(LastSet.iRunStartMode==rsmAutoSiteMap)
        mode=5;
    else if(LastSet.iRunStartMode==rsmQAMode)
        mode=6;
    else
        mode=-1;

    t.sprintf("%02d\r", mode);

    SendMSG_CMD(MSG_CMD_StartMode, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteAssign_NS()
{
    AnsiString t;

    if(TestIF.iTestMode==SingleSite)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,0000000%d\r\n",
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==DualSite)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,000000%d%d\r\n",
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,00000%d%d%d\r\n",
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==QualSite1X4)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,0000%d%d%d%d\r\n",
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==DualSite2x1)                                      // Dual Site 2x1
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,000000%d%d_\r\n",
                     TestIF.iSiteMap[1][0],
                     TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==QualSite2X2 ||
            TestIF.iTestMode==QualSite2X2N)
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,0000%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_6Site2X3 ||                                      //ChungHung 20140115 add for 2x3_6
            TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,00%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_8Site2X4 ||
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        t.sprintf("ASSIGN:00000000,00000000,00000000,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_10Site2X5)    //wei 20190614 10 site
    {
        t.sprintf("ASSIGN:00000000,00000000,000000%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_12Site2X6)
    {
        t.sprintf("ASSIGN:00000000,00000000,0000%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][5],
                    TestIF.iSiteMap[0][5],
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_16Site2X8)
    {
        t.sprintf("ASSIGN:00000000,00000000,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[1][7],
                    TestIF.iSiteMap[0][7],
                    TestIF.iSiteMap[1][6],
                    TestIF.iSiteMap[0][6],
                    TestIF.iSiteMap[1][5],
                    TestIF.iSiteMap[0][5],
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_16Site4X4)
    {
        t.sprintf("ASSIGN:00000000,00000000,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[3][3],
                    TestIF.iSiteMap[2][3],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[3][2],
                    TestIF.iSiteMap[2][2],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[3][1],
                    TestIF.iSiteMap[2][1],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[3][0],
                    TestIF.iSiteMap[2][0],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else if(TestIF.iTestMode==_32Site4X8N)
    {
        t.sprintf("ASSIGN:%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d\r\n" ,
                    TestIF.iSiteMap[3][7],
                    TestIF.iSiteMap[2][7],
                    TestIF.iSiteMap[1][7],
                    TestIF.iSiteMap[0][7],
                    TestIF.iSiteMap[3][6],
                    TestIF.iSiteMap[2][6],
                    TestIF.iSiteMap[1][6],
                    TestIF.iSiteMap[0][6],
                    TestIF.iSiteMap[3][5],
                    TestIF.iSiteMap[2][5],
                    TestIF.iSiteMap[1][5],
                    TestIF.iSiteMap[0][5],
                    TestIF.iSiteMap[3][4],
                    TestIF.iSiteMap[2][4],
                    TestIF.iSiteMap[1][4],
                    TestIF.iSiteMap[0][4],
                    TestIF.iSiteMap[3][3],
                    TestIF.iSiteMap[2][3],
                    TestIF.iSiteMap[1][3],
                    TestIF.iSiteMap[0][3],
                    TestIF.iSiteMap[3][2],
                    TestIF.iSiteMap[2][2],
                    TestIF.iSiteMap[1][2],
                    TestIF.iSiteMap[0][2],
                    TestIF.iSiteMap[3][1],
                    TestIF.iSiteMap[2][1],
                    TestIF.iSiteMap[1][1],
                    TestIF.iSiteMap[0][1],
                    TestIF.iSiteMap[3][0],
                    TestIF.iSiteMap[2][0],
                    TestIF.iSiteMap[1][0],
                    TestIF.iSiteMap[0][0]);
    }
    else
    {
        t.sprintf("UNKNOWN\r\n");
    }
    SendMSG_CMD(MSG_CMD_Assign, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteTemp_NS()
{
    AnsiString t, asNULL = "NULL";

    if(Temperature.iIndexHeatMode==ChamberOnly)
    {
        for(int i=tcHead1; i<=tcBd2; i++)
        {
            if(i!=tcChamber && i!=tcSocket && i!=tcCCD)
                asGPIBTempShow[i]=asGPIBTempShow[tcChamber];
        }
    }

    //2013.01.24 Q_Q TSMC GPIB COMMAND Part 2. {
    if(TestIF.iTestMode==SingleSite)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f\r\n", (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0);
            }
        }
        else
        {
            t.sprintf("1,%+06.1f\r\n", 0);
        }
    }
    else if(TestIF.iTestMode==DualSite)                                         // Dual Site 1x2
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n",
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f\r\n",
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==QualSite1X4)      // Qual Site 1x4
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==DualSite2x1)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f\r\n" ,
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2)                                      // Qual Site 2x2
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            0,
                            0,
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2N)                                     // Qual Site 2x2
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f\r\n" ,
                            0,
                            0,
                            0,
                            0);
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3)                                        //ChungHung 20140115 add for 2x3_6
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                             0,
                             0,
                             0,

                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4)                                        // 8 Site 2x4
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_10Site2X5)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater || USE_16_HEATER==eht16HeaterEJ1N || USE_16_HEATER==eht16HeaterDTME08)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_12Site2X6)
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcAf1]!="ERR" && asGPIBTempShow[tcAf1]!="NULL")?atof(asGPIBTempShow[tcAf1].c_str()):0,
                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0,
                            (asGPIBTempShow[tcBf1]!="ERR" && asGPIBTempShow[tcBf1]!="NULL")?atof(asGPIBTempShow[tcBf1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcAf2]!="ERR" && asGPIBTempShow[tcAf2]!="NULL")?atof(asGPIBTempShow[tcAf2].c_str()):0,
                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0,
                            (asGPIBTempShow[tcBf2]!="ERR" && asGPIBTempShow[tcBf2]!="NULL")?atof(asGPIBTempShow[tcBf2].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater || USE_16_HEATER==eht16HeaterEJ1N || USE_16_HEATER==eht16HeaterDTME08)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_16Site2X8)                                       //16 Site
    {
        if(IndexStatus==Z1Down_Z2Up)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcAf1]!="ERR" && asGPIBTempShow[tcAf1]!="NULL")?atof(asGPIBTempShow[tcAf1].c_str()):0,
                            (asGPIBTempShow[tcAg1]!="ERR" && asGPIBTempShow[tcAg1]!="NULL")?atof(asGPIBTempShow[tcAg1].c_str()):0,
                            (asGPIBTempShow[tcAh1]!="ERR" && asGPIBTempShow[tcAh1]!="NULL")?atof(asGPIBTempShow[tcAh1].c_str()):0,

                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0,
                            (asGPIBTempShow[tcBf1]!="ERR" && asGPIBTempShow[tcBf1]!="NULL")?atof(asGPIBTempShow[tcBf1].c_str()):0,
                            (asGPIBTempShow[tcBg1]!="ERR" && asGPIBTempShow[tcBg1]!="NULL")?atof(asGPIBTempShow[tcBg1].c_str()):0,
                            (asGPIBTempShow[tcBh1]!="ERR" && asGPIBTempShow[tcBh1]!="NULL")?atof(asGPIBTempShow[tcBh1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcAf2]!="ERR" && asGPIBTempShow[tcAf2]!="NULL")?atof(asGPIBTempShow[tcAf2].c_str()):0,
                            (asGPIBTempShow[tcAg2]!="ERR" && asGPIBTempShow[tcAg2]!="NULL")?atof(asGPIBTempShow[tcAg2].c_str()):0,
                            (asGPIBTempShow[tcAh2]!="ERR" && asGPIBTempShow[tcAh2]!="NULL")?atof(asGPIBTempShow[tcAh2].c_str()):0,

                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0,
                            (asGPIBTempShow[tcBf2]!="ERR" && asGPIBTempShow[tcBf2]!="NULL")?atof(asGPIBTempShow[tcBf2].c_str()):0,
                            (asGPIBTempShow[tcBg2]!="ERR" && asGPIBTempShow[tcBg2]!="NULL")?atof(asGPIBTempShow[tcBg2].c_str()):0,
                            (asGPIBTempShow[tcBh2]!="ERR" && asGPIBTempShow[tcBh2]!="NULL")?atof(asGPIBTempShow[tcBh2].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater || USE_16_HEATER==eht16HeaterEJ1N || USE_16_HEATER==eht16HeaterDTME08)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_16Site4X4)
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0);
            }
            else
            {
                 t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f\r\n",
                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0,

                             0,
                             0,
                             0,
                             0);
            }
        }
    }
    else if(TestIF.iTestMode==_32Site4X8N)
    {
        if(IndexStatus==Z1_Z2_Down)
        {
            if((USE_16_HEATER==eht32HeaterEJ1N ||                               //Steven 20140923 : Index使用EJ1N版32組加熱器
                USE_16_HEATER==eht32HeaterKT4H ||                               //Steven 20150211 : Index使用KT4H版32組加熱器
                USE_16_HEATER==eht32HeaterDTME08) &&                            //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                TestIF_File.bUse32Heater)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,

                            (asGPIBTempShow[tcAe2]!="ERR" && asGPIBTempShow[tcAe2]!="NULL")?atof(asGPIBTempShow[tcAe2].c_str()):0,
                            (asGPIBTempShow[tcAf2]!="ERR" && asGPIBTempShow[tcAf2]!="NULL")?atof(asGPIBTempShow[tcAf2].c_str()):0,
                            (asGPIBTempShow[tcAg2]!="ERR" && asGPIBTempShow[tcAg2]!="NULL")?atof(asGPIBTempShow[tcAg2].c_str()):0,
                            (asGPIBTempShow[tcAh2]!="ERR" && asGPIBTempShow[tcAh2]!="NULL")?atof(asGPIBTempShow[tcAh2].c_str()):0,

                            (asGPIBTempShow[tcBe2]!="ERR" && asGPIBTempShow[tcBe2]!="NULL")?atof(asGPIBTempShow[tcBe2].c_str()):0,
                            (asGPIBTempShow[tcBf2]!="ERR" && asGPIBTempShow[tcBf2]!="NULL")?atof(asGPIBTempShow[tcBf2].c_str()):0,
                            (asGPIBTempShow[tcBg2]!="ERR" && asGPIBTempShow[tcBg2]!="NULL")?atof(asGPIBTempShow[tcBg2].c_str()):0,
                            (asGPIBTempShow[tcBh2]!="ERR" && asGPIBTempShow[tcBh2]!="NULL")?atof(asGPIBTempShow[tcBh2].c_str()):0,

                            (asGPIBTempShow[tcAe1]!="ERR" && asGPIBTempShow[tcAe1]!="NULL")?atof(asGPIBTempShow[tcAe1].c_str()):0,
                            (asGPIBTempShow[tcAf1]!="ERR" && asGPIBTempShow[tcAf1]!="NULL")?atof(asGPIBTempShow[tcAf1].c_str()):0,
                            (asGPIBTempShow[tcAg1]!="ERR" && asGPIBTempShow[tcAg1]!="NULL")?atof(asGPIBTempShow[tcAg1].c_str()):0,
                            (asGPIBTempShow[tcAh1]!="ERR" && asGPIBTempShow[tcAh1]!="NULL")?atof(asGPIBTempShow[tcAh1].c_str()):0,

                            (asGPIBTempShow[tcBe1]!="ERR" && asGPIBTempShow[tcBe1]!="NULL")?atof(asGPIBTempShow[tcBe1].c_str()):0,
                            (asGPIBTempShow[tcBf1]!="ERR" && asGPIBTempShow[tcBf1]!="NULL")?atof(asGPIBTempShow[tcBf1].c_str()):0,
                            (asGPIBTempShow[tcBg1]!="ERR" && asGPIBTempShow[tcBg1]!="NULL")?atof(asGPIBTempShow[tcBg1].c_str()):0,
                            (asGPIBTempShow[tcBh1]!="ERR" && asGPIBTempShow[tcBh1]!="NULL")?atof(asGPIBTempShow[tcBh1].c_str()):0);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08)                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAa2]!="ERR" && asGPIBTempShow[tcAa2]!="NULL")?atof(asGPIBTempShow[tcAa2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,
                            (asGPIBTempShow[tcAb2]!="ERR" && asGPIBTempShow[tcAb2]!="NULL")?atof(asGPIBTempShow[tcAb2].c_str()):0,

                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBa2]!="ERR" && asGPIBTempShow[tcBa2]!="NULL")?atof(asGPIBTempShow[tcBa2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,
                            (asGPIBTempShow[tcBb2]!="ERR" && asGPIBTempShow[tcBb2]!="NULL")?atof(asGPIBTempShow[tcBb2].c_str()):0,

                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAa1]!="ERR" && asGPIBTempShow[tcAa1]!="NULL")?atof(asGPIBTempShow[tcAa1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,
                            (asGPIBTempShow[tcAb1]!="ERR" && asGPIBTempShow[tcAb1]!="NULL")?atof(asGPIBTempShow[tcAb1].c_str()):0,

                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBa1]!="ERR" && asGPIBTempShow[tcBa1]!="NULL")?atof(asGPIBTempShow[tcBa1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,
                            (asGPIBTempShow[tcBb1]!="ERR" && asGPIBTempShow[tcBb1]!="NULL")?atof(asGPIBTempShow[tcBb1].c_str()):0,

                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAc2]!="ERR" && asGPIBTempShow[tcAc2]!="NULL")?atof(asGPIBTempShow[tcAc2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,
                            (asGPIBTempShow[tcAd2]!="ERR" && asGPIBTempShow[tcAd2]!="NULL")?atof(asGPIBTempShow[tcAd2].c_str()):0,

                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBc2]!="ERR" && asGPIBTempShow[tcBc2]!="NULL")?atof(asGPIBTempShow[tcBc2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,
                            (asGPIBTempShow[tcBd2]!="ERR" && asGPIBTempShow[tcBd2]!="NULL")?atof(asGPIBTempShow[tcBd2].c_str()):0,

                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAc1]!="ERR" && asGPIBTempShow[tcAc1]!="NULL")?atof(asGPIBTempShow[tcAc1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,
                            (asGPIBTempShow[tcAd1]!="ERR" && asGPIBTempShow[tcAd1]!="NULL")?atof(asGPIBTempShow[tcAd1].c_str()):0,

                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBc1]!="ERR" && asGPIBTempShow[tcBc1]!="NULL")?atof(asGPIBTempShow[tcBc1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0,
                            (asGPIBTempShow[tcBd1]!="ERR" && asGPIBTempShow[tcBd1]!="NULL")?atof(asGPIBTempShow[tcBd1].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly)
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,
                            (asGPIBTempShow[tcHead3]!="ERR" && asGPIBTempShow[tcHead3]!="NULL")?atof(asGPIBTempShow[tcHead3].c_str()):0,

                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,
                            (asGPIBTempShow[tcHead1]!="ERR" && asGPIBTempShow[tcHead1]!="NULL")?atof(asGPIBTempShow[tcHead1].c_str()):0,

                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,
                            (asGPIBTempShow[tcHead4]!="ERR" && asGPIBTempShow[tcHead4]!="NULL")?atof(asGPIBTempShow[tcHead4].c_str()):0,

                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0,
                            (asGPIBTempShow[tcHead2]!="ERR" && asGPIBTempShow[tcHead2]!="NULL")?atof(asGPIBTempShow[tcHead2].c_str()):0);
            }
            else
            {
                t.sprintf("1,%+06.1f2,%+06.1f3,%+06.1f4,%+06.1f5,%+06.1f6,%+06.1f7,%+06.1f8,%+06.1f9,%+06.1f10,%+06.1f11,%+06.1f12,%+06.1f13,%+06.1f14,%+06.1f15,%+06.1f16,%+06.1f17,%+06.1f18,%+06.1f19,%+06.1f20,%+06.1f21,%+06.1f22,%+06.1f23,%+06.1f24,%+06.1f25,%+06.1f26,%+06.1f27,%+06.1f28,%+06.1f29,%+06.1f30,%+06.1f31,%+06.1f32,%+06.1f\r\n" ,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    else
    {
        t.sprintf("UNKNOWN\r", sizeof(t));
    }

    SendMSG_CMD(MSG_CMD_ActualTemp, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteForce_NS()
{
    AnsiString t;

    if(IniConfig.bKoreaFunction==true)
    {
        t.sprintf("%s\r", asArmForce1);                                         //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else if(CUSTOMER_CODE==CC_TSMC_TAINAN || IniConfig.bSPILFunction==true || CUSTOMER_CODE==CC_ASE_KaohSiung)              //wei 20151117 傳送ContactForce設定值  //JerryYang 20230204 : Add SPIL support回傳GPIB force指令
    {
        t.sprintf("%sT\r", asArmForce1);                                        //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
    }
    else
    {
        if(IndexStatus==Z1Down_Z2Up ||
           IndexStatus==Z1_Z2_Down)                                             //Steven 20230214 : Add GPIB for NN mode
        {
            t.sprintf("%s\r\n", asArmForce1);                                   //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else if(IndexStatus==Z1Up_Z2Down)
        {
            t.sprintf("%s\r\n", asArmForce2);                                   //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
        }
        else
        {
            //2013.01.26 Q_Q TSMC GPIB COMMAND Part 3.
            t.sprintf("%d\r", 0);
        }
    }
    SendMSG_CMD(MSG_CMD_ContactForce, t);
}
//ChungHung alter 20130510 End
//---------------------------------------------------------------------------
AnsiString __fastcall TfMain::WriteBinMap(bool bGPIB)                           //ChungHung 20150217 add for SCK request
{
    int iBinSelCT;
    AnsiString Data="";
    bool bHaveBinData=false;
    int temp;
    AnsiString aUnloader[]={"Auto1-", "Auto2-", "Auto3-", "Fix1-", "Fix2-", "Fix3-", "Fix4-", "Fix5-", "Fix6-"};
    if(TrayForm.iFixTrayMode)
    {
        iBinSelCT=9;    //use up down
    }
    else
    {
        iBinSelCT=6;
    }

    for(int i=0; i<iBinSelCT; i++)
    {
        //temp="";
        bHaveBinData=false;
        for(int j=0; j<iTestBinCount; j++)
        {
            temp=BinSelect[iTestRunMode].iCatDataT3Pos[j];
            if(temp<=0)
                continue;

            if(i==temp-1)
            {
                //Auto1-1,2_Auto2-3,4_Auto3-5,6_Fix1-15,16_Fix2-5_Fix3-7,8,9,error.
                bHaveBinData=true;
                aUnloader[i]+=AnsiString(j)+",";
            }
        }

        if(BinSelect[iTestRunMode].IfErrorT3==i)
        {
            aUnloader[i]+="error";
            bHaveBinData=true;
        }
        else
        {
            if(bHaveBinData)
                aUnloader[i].Delete(aUnloader[i].Length(), 1);
            else
                aUnloader[i]+="NA";
        }
        Data+=aUnloader[i]+"_";

    }
    Data.Delete(Data.Length(), 1);
    Data+=".";

    if(bGPIB)
        SendMSG_CMD(MSG_CMD_BinMap, Data);
    return Data;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSetBinMap(AnsiString BinData)                      //Steven 20230210 : Set Bin Map.
{
    int iMaxBin=16, iPos1, iPos2, iBin, iTray;
    AnsiString Data, Data1=BinData;
    AnsiString Data2;
    AnsiString aUnloader[10]={"BINMAP_", "Auto1-", "Auto2-", "Auto3-", "Fix1-", "Fix2-", "Fix3-", "Fix4-", "Fix5-", "Fix6-"};

    if(HasICUnderMachine()==false)
    {
        TStringList *SL2=new TStringList();
        fBinSel->sBinTraySetT3Pos[eBinFT]->Clear();
        for(int i=0; i<16; i++)
        {
            fBinSel->sBinTraySetT3Pos[eBinFT]->Add("0");
        }

        for(int i=0; i<10; i++)
        {
            iTray=i;
            if(i==0)
                continue;
            iPos1=Data1.AnsiPos(aUnloader[i]);
            if(iPos1!=0)
            {
                SL2->Clear();
                Data2=Data1.SubString(iPos1, Data1.Length());
                iPos1=Data2.AnsiPos("-")+1;
                iPos2=Data2.AnsiPos("_");
                if(iPos2!=0)
                    Data2=Data2.SubString(iPos1, iPos2-iPos1);
                else
                    Data2=Data2.SubString(iPos1, Data2.Length());
                SL2->CommaText=Data2;

                for(int j=0; j<SL2->Count; j++)
                {
                    if(SL2->Strings[j].AnsiPos("error")!=0)
                    {
                        BinSelect[eBinFT].IfErrorT3=iTray;
                    }
                    else
                    {
                        iBin=atoi(SL2->Strings[j].c_str());
                        if(iBin>=iMaxBin)
                        {
                            iMaxBin=iBin+1;
                            do
                            {
                                fBinSel->sBinTraySetT3Pos[eBinFT]->Add("0");
                            }while(fBinSel->sBinTraySetT3Pos[eBinFT]->Count<iMaxBin);
                        }
                        if(iBin!=0 || SL2->Strings[j]=="0")
                        {
                            fBinSel->sBinTraySetT3Pos[eBinFT]->Strings[iBin]=iTray;
                        }
                    }
                }
            }
        }

        fBinSel->Save(3617, eBinFT);
        fBinSel->Save(3616, eBinFT);
        SL2->Clear();
        delete SL2;
        Data="SETTINGOK";
    }
    else
    {
        Data="SETTINGNG";
    }

    SendMSG_CMD(MSG_CMD_SetBinMap, Data);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteTestMode()                                         //ChungHung 20150217 add for SCK request
{
    AnsiString t;

    if(iTestRunMode==FT)
        t.sprintf("Normal\r\n");
    else if(iTestRunMode==RT)
        t.sprintf("Retest\r\n");
    else if(iTestRunMode==FT_ART)
        t.sprintf("Normal_ART\r\n");
    else if(iTestRunMode==RT_ART)
        t.sprintf("Retest_ART\r\n");
    else
        t.sprintf("Off-line\r\n");                                              //ChungHung 20150408 add for SCK

    SendMSG_CMD(MSG_CMD_TestMode, t);
}
//---------------------------------------------------------------------------
void TfMain::WriteNowAllTempData()                                              //Frank 20150729 add Send Now All Temp Value To Test
{
    AnsiString sTemp="";
    double fTemp = 999.9;
    AnsiString t="";
    AnsiString asNULL="NULL";
    bool bReturnNull=false;

//    Handler Respond
//    Format : TEMP_Plate1_Plate2_Shuttle1_Shuttle2_Head1_Head2_Head5_Head6_Socket_Chamber
//    Ex1 , TEMP_+90.4_+90.2_+89.7_+90.3_+90.1_+90.2_+89.9_+90.0
//    Ex2, TEMP_NULL_+90.2_+89.7_+90.3_+90.1_+90.2_+89.9_+90.0
//    NULL代表沒有使用, 以Ex2為例, 代表Plate1未使用.

    AnsiString sCMDStr="TEMP_";
    AnsiString asStr;

    if(USE_16_HEATER==eht4Heater)
    {
        for(int i=tcHead1; i<=tcChamber; i++)                                   //Ifor 20151006 : Add GPIB Temp Report
        {
            fTemp=999.9;
            bReturnNull=false;                                                  //jou 2015-10-30 修正GPIB 溫度常溫Chamber only,chamber 溫度異常
            if(bUT150Install[i] && bGetHeaterUsed(i))
            {
                if(Temperature.iIndexHeatMode==ChamberOnly ||
                   Temperature.iIndexHeatMode==SocketChamber)
                    fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                else
                    fTemp=atof(asGPIBTempShow[i].c_str());
            }
            else
            {                                                                   //JerryYang 20160126 回傳溫度要考慮是否關ARM或關SITE
                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    if(((i>=tcHead1 && i<=tcHead2) || i==tcChamber) && bGetHeaterUsed(i))   //JerryYang 20160126 關ARM或關SITE就不回傳溫度
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        bReturnNull=true;
                    }
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    if(((i>=tcHead3 && i<=tcHead4) || i==tcChamber) && bGetHeaterUsed(i))   //JerryYang 20160126 關ARM或關SITE就不回傳溫度
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        bReturnNull = true;
                    }
                }
                else
                {
                    if(bGetHeaterUsed(i) && ((i>=tcHead1 && i<=tcHead4) || i==tcChamber)) //JerryYang 20160126 要考慮關SITE
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        bReturnNull=true;
                    }
                }
            }

            if(bReturnNull==true)
            {
                sTemp="NULL";
            }
            else
            {
                if(fTemp>=0)
                    sTemp.sprintf("+%1.1f", fTemp);
                else
                    sTemp.sprintf("-%1.1f", fTemp);
            }

            t+=sTemp;
            if(i<tcChamber)
            {
                t+="_";
            }
        }
    }
    //JerryYang 20160518 矽格湖口 柏均 要求16組加熱器的機型也要能支援4組加熱器的指令
    else if(USE_16_HEATER==eht16Heater       ||                                 //Steven 20120606 : 16溫控器 8Site使用Hontech頭
            USE_16_HEATER==eht16HeaterEJ1N   ||
            USE_16_HEATER==eht32HeaterEJ1N   ||                                 //Steven 20140923 : Index使用EJ1N版32組加熱器
            USE_16_HEATER==eht32HeaterKT4H   ||                                 //Steven 20150211 : Index使用KT4H版32組加熱器
            USE_16_HEATER==eht16HeaterDTME08 ||                                 //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            USE_16_HEATER==eht32HeaterDTME08 )                                  //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
    {
        bool bUsed[2][2]={{false, false}, {false, false}};                      //JerryYang 20160518 對應4組加熱器 Head1、Head2、Head3、Head4
        if(TestIF.iTestMode==SingleSite)
        {
            for(int i=0; i<4; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int iSinglesiteHead[4]={tcAa1, tcBa1, tcAa2, tcBa2};
                if(bUT150Install[iSinglesiteHead[i]] && bGet16HeaterUsedTo4Heater(iSinglesiteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[iSinglesiteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                if(bReturnNull==true)
                {
                    sTemp="NULL";
                }
                else
                {
                    if(fTemp>=0)
                        sTemp.sprintf("+%1.1f", fTemp);
                    else
                        sTemp.sprintf("-%1.1f", fTemp);
                }
                t+=sTemp;
                t+="_";
            }
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f", fTemp);
                    sTemp2.sprintf("+%1.1f", fTemp);
                    sTemp3.sprintf("+%1.1f", fTemp);
                    sTemp4.sprintf("+%1.1f", fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f", fTemp);
                    sTemp2.sprintf("-%1.1f", fTemp);
                    sTemp3.sprintf("-%1.1f", fTemp);
                    sTemp4.sprintf("-%1.1f", fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        else if(TestIF.iTestMode==DualSite)
        {
            for(int i=0; i<4; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int iDualsiteHead[4]={tcAa1, tcAb1, tcAa2, tcAb2};
                if((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                   (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit))   //Sam 20220429 : 修正 1Cabcle layout 溫度回傳問題
                {
                    iDualsiteHead[0]=tcAa1;
                    iDualsiteHead[1]=tcBa1;
                    iDualsiteHead[2]=tcAa2;
                    iDualsiteHead[3]=tcBa2;
                }
                if(bUT150Install[iDualsiteHead[i]] &&
                   bGet16HeaterUsedTo4Heater(iDualsiteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[iDualsiteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                if(bReturnNull==true)
                {
                    sTemp="NULL";
                }
                else
                {
                    if(fTemp>=0)
                        sTemp.sprintf("+%1.1f",fTemp);
                    else
                        sTemp.sprintf("-%1.1f",fTemp);
                }
                t+=sTemp;
                t+="_";
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f", fTemp);
                    sTemp2.sprintf("+%1.1f", fTemp);
                    sTemp3.sprintf("+%1.1f", fTemp);
                    sTemp4.sprintf("+%1.1f", fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f", fTemp);
                    sTemp2.sprintf("-%1.1f", fTemp);
                    sTemp3.sprintf("-%1.1f", fTemp);
                    sTemp4.sprintf("-%1.1f", fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false)                                //左邊全關
                {
                    sTemp1.sprintf("NULL");
                }
                if(bTestSiteUse[0][0][1]==false)
                {
                    sTemp2.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][0]==false)
                {
                    sTemp3.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][1]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 1x3
        else if(TestIF.iTestMode==QualSite1X4)
        {
            //Richard 20220809 SG湖口
            //==>
            int iOneCable[8]={tcAa1, tcBa1, tcAb1, tcBb1, tcAa2, tcBa2, tcAb2, tcBb2};
            int iOtherCable[8]={tcAa1, tcAb1, tcAc1, tcAd1, tcAa2, tcAb2, tcAc2, tcAd2};
            int i1X4siteHead[8];
            for(int j=0; j<8; j++)
            {
                if(USE_16_HEATER==eht4Heater ||
                   ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                    (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit)))
                {
                    i1X4siteHead[j]=iOneCable[j];
                }
                else
                {
                    i1X4siteHead[j]=iOtherCable[j];
                }
            }

            for(int i=0; i<8; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;

                if(bUT150Install[i1X4siteHead[i]] &&
                   bGet16HeaterUsedTo4Heater(i1X4siteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[i1X4siteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                bool bwrite=true;
                if(USE_16_HEATER==eht4Heater ||
                   ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                    (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit)))   //jou 2015-10-15  : 16溫控器 1條線版本)
                {
                    if(i1X4siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && (bTestSiteUse[0][0][0] || bTestSiteUse[0][0][1]))
                    {
//                        bUsed[0][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcBa1 && bUsed[0][0]==false && bReturnNull==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][1]==false && (bTestSiteUse[0][0][2]||bTestSiteUse[0][0][3]))
                    {
//                        bUsed[0][1]=true;
                    }
                    else if(i1X4siteHead[i]==tcBb1 && bUsed[0][1]==false && bReturnNull==false )
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i1X4siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && (bTestSiteUse[1][0][0]||bTestSiteUse[1][0][1]))
                    {
//                        bUsed[1][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcBa2 && bUsed[1][0]==false && bReturnNull==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i1X4siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][1]==false && (bTestSiteUse[1][0][2]||bTestSiteUse[1][0][3]))
                    {
//                        bUsed[1][1]=true;
                    }
                    else if(i1X4siteHead[i]==tcBb2 && bUsed[1][1]==false && bReturnNull==false)
                    {
                        bUsed[1][1]=true;
                    }
                    else
                    {
                        bwrite=false;
                    }
                }
            //<==
                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }

            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f",fTemp);
                    sTemp2.sprintf("+%1.1f",fTemp);
                    sTemp3.sprintf("+%1.1f",fTemp);
                    sTemp4.sprintf("+%1.1f",fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f",fTemp);
                    sTemp2.sprintf("-%1.1f",fTemp);
                    sTemp3.sprintf("-%1.1f",fTemp);
                    sTemp4.sprintf("-%1.1f",fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][0][1]==false)    //左邊全關
                {
                    sTemp1.sprintf("NULL");
                }
                if(bTestSiteUse[0][0][2]==false && bTestSiteUse[0][0][3]==false)
                {
                    sTemp2.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][0][1]==false)
                {
                    sTemp3.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][2]==false && bTestSiteUse[1][0][3]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 2x1
        else if(TestIF.iTestMode==QualSite2X2)
        {
            for(int i=0; i<8; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int i2x2siteHead[8]={tcAa1, tcBa1, tcAb1, tcBb1, tcAa2, tcBa2, tcAb2, tcBb2};
                if(bUT150Install[i2x2siteHead[i]] && bGet16HeaterUsedTo4Heater(i2x2siteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[i2x2siteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                bool bwrite=true;
                if(i2x2siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][0])
                {
                    bUsed[0][0]=true;
                }
                else if(i2x2siteHead[i]==tcBa1 && bUsed[0][0]==false)
                {
                    bUsed[0][0]=true;
                }
                else if(i2x2siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][0][1])
                {
                    bUsed[0][1]=true;
                }
                else if(i2x2siteHead[i]==tcBb1 && bUsed[0][1]==false)
                {
                    bUsed[0][1]=true;
                }
                else if(i2x2siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][0])
                {
                    bUsed[1][0]=true;
                }
                else if(i2x2siteHead[i]==tcBa2 && bUsed[1][0]==false)
                {
                    bUsed[1][0]=true;
                }
                else if(i2x2siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][0][1])
                {
                    bUsed[1][1]=true;
                }
                else if(i2x2siteHead[i]==tcBb2 && bUsed[1][1]==false)
                {
                    bUsed[1][1]=true;
                }
                else
                {
                    bwrite=false;
                }
                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f", fTemp);
                    sTemp2.sprintf("+%1.1f", fTemp);
                    sTemp3.sprintf("+%1.1f", fTemp);
                    sTemp4.sprintf("+%1.1f", fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f", fTemp);
                    sTemp2.sprintf("-%1.1f", fTemp);
                    sTemp3.sprintf("-%1.1f", fTemp);
                    sTemp4.sprintf("-%1.1f", fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][1][0]==false)    //左邊全關
                {
                    sTemp1.sprintf("NULL");
                }
                if(bTestSiteUse[0][0][1]==false && bTestSiteUse[0][1][1]==false)
                {
                    sTemp2.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][1][0]==false)
                {
                    sTemp3.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][1]==false && bTestSiteUse[1][1][1]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 2x2N
        else if(TestIF.iTestMode==_6Site2X3)
        {
            for(int i=0; i<12; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int i6siteHead[12]={tcAa1, tcAb1, tcAc1, tcBa1, tcBb1, tcBc1, tcAa2, tcAb2, tcAc2, tcBa2, tcBb2, tcBc2};
                if(bUT150Install[i6siteHead[i]] && bGet16HeaterUsedTo4Heater(i6siteHead[i]))
                {
                    if(Temperature.iIndexHeatMode==ChamberOnly ||
                       Temperature.iIndexHeatMode==SocketChamber)
                    {
                        fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                    }
                    else
                    {
                        fTemp=atof(asGPIBTempShow[i6siteHead[i]].c_str());
                    }
                }
                else
                {
                    bReturnNull=true;
                }
                bool bwrite=true;
                if(i6siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][0])
                {
                    bUsed[0][0]=true;
                }
                else if(i6siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][1])
                {
                    bUsed[0][0]=true;
                }
                else if(i6siteHead[i]==tcAc1 && bUsed[0][0]==false)
                {
                    bUsed[0][0]=true;
                }
                else if(i6siteHead[i]==tcBa1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][1][0])
                {
                    bUsed[0][1]=true;
                }
                else if(i6siteHead[i]==tcBb1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][1][1])
                {
                    bUsed[0][1]=true;
                }
                else if(i6siteHead[i]==tcBc1 && bUsed[0][1]==false)
                {
                    bUsed[0][1]=true;
                }
                else if(i6siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][0])
                {
                    bUsed[1][0]=true;
                }
                else if(i6siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][1])
                {
                    bUsed[1][0]=true;
                }
                else if(i6siteHead[i]==tcAc2 && bUsed[1][0]==false)
                {
                    bUsed[1][0]=true;
                }
                else if(i6siteHead[i]==tcBa2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][1][0])
                {
                    bUsed[1][1]=true;
                }
                else if(i6siteHead[i]==tcBb2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][1][1])
                {
                    bUsed[1][1]=true;
                }
                else if(i6siteHead[i]==tcBc2 && bUsed[1][1]==false)
                {
                    bUsed[1][1]=true;
                }
                else
                {
                    bwrite=false;
                }
                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1, sTemp2, sTemp3, sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f",fTemp);
                    sTemp2.sprintf("+%1.1f",fTemp);
                    sTemp3.sprintf("+%1.1f",fTemp);
                    sTemp4.sprintf("+%1.1f",fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f",fTemp);
                    sTemp2.sprintf("-%1.1f",fTemp);
                    sTemp3.sprintf("-%1.1f",fTemp);
                    sTemp4.sprintf("-%1.1f",fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][0][1]==false && bTestSiteUse[0][0][2]==false)
                {
                    sTemp1.sprintf("NULL");
                }
                if(bTestSiteUse[0][1][0]==false && bTestSiteUse[0][1][1]==false && bTestSiteUse[0][1][2]==false)
                {
                    sTemp2.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][0][1]==false && bTestSiteUse[1][0][2]==false)
                {
                    sTemp3.sprintf("NULL");
                }
                if(bTestSiteUse[1][1][0]==false && bTestSiteUse[1][1][1]==false && bTestSiteUse[1][1][2]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
        //QQ 20230214 : 2x3N
        else if(TestIF.iTestMode==_8Site2X4)
        {
            int iHeadNum;                                                       //JerryYang 20160613 使用NS8000也要支援GPIB溫度指令
            if((IniConfig.bSIGURDFunction &&
               ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit))) ||      //Sam 20210524 2x4 新增一條線版本
                TestIF_File.bNS8000CS)
                iHeadNum=4;
            else
                iHeadNum=16;

            for(int i=0; i<iHeadNum; i++)
            {
                double fTemp=999.9;
                bool bReturnNull=false;
                int i8siteHead[16]={tcAa1, tcAb1, tcBa1, tcBb1, tcAc1, tcAd1, tcBc1, tcBd1, tcAa2, tcAb2, tcBa2, tcBb2, tcAc2, tcAd2, tcBc2, tcBd2};
                int iNS8siteHead[4]={tcAa1, tcAb1, tcAa2, tcAb2};
                int ib1CablesiteHead[4]={tcAa1, tcBa1, tcAa2, tcBa2};           //Sam 20210524 2x4 新增一條線版本
                bool bwrite=true;
                if(IniConfig.bSIGURDFunction &&
                   ((IniConfig.bL30Use1CableLayoutKitByConfig==false && TestIF_File.b1CableLayoutKit) ||
                    (IniConfig.bL30Use1CableLayoutKitByConfig==true && IniConfig.bL30Use1CableLayoutKit)))  //Sam 20210524 2x4 新增一條線版本
                {
                    if(bUT150Install[ib1CablesiteHead[i]] &&
                       bGet16HeaterUsedTo4Heater(ib1CablesiteHead[i]))
                    {
                        if(Temperature.iIndexHeatMode==ChamberOnly ||
                           Temperature.iIndexHeatMode==SocketChamber)
                        {
                            fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                        }
                        else
                        {
                            fTemp=atof(asGPIBTempShow[ib1CablesiteHead[i]].c_str());
                        }
                    }
                    else
                    {
                        bReturnNull=true;
                    }
                    if(ib1CablesiteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && (bTestSiteUse[0][0][0] || bTestSiteUse[0][0][1] || bTestSiteUse[0][1][0] || bTestSiteUse[0][1][1]))
                    {
                        bUsed[0][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcAa1 && bUsed[0][0]==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa1 && bReturnNull==false && bUsed[0][1]==false && (bTestSiteUse[0][0][2] || bTestSiteUse[0][0][3] || bTestSiteUse[0][1][2] || bTestSiteUse[0][1][3]))
                    {
                        bUsed[0][1]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa1 && bUsed[0][1]==false)
                    {
                        bUsed[0][1]=true;
                    }  //-----------------------------------------------------------
                    else if(ib1CablesiteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && (bTestSiteUse[1][0][0] || bTestSiteUse[1][0][1] || bTestSiteUse[1][1][0] || bTestSiteUse[1][1][1]))
                    {
                        bUsed[1][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcAa2 && bUsed[1][0]==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa2 && bReturnNull==false && bUsed[1][1]==false && (bTestSiteUse[1][0][2] || bTestSiteUse[1][0][3] || bTestSiteUse[1][1][2] || bTestSiteUse[1][1][3]))
                    {
                        bUsed[1][1]=true;
                    }
                    else if(ib1CablesiteHead[i]==tcBa2 && bUsed[1][1]==false)
                    {
                        bUsed[1][1]=true;
                    }
                    else
                    {
                        bwrite=false;
                    }
                }
                else if(TestIF_File.bNS8000CS==true)
                {
                    if(bUT150Install[iNS8siteHead[i]] &&
                       bGet16HeaterUsedTo4Heater(iNS8siteHead[i]))
                    {
                        if(Temperature.iIndexHeatMode==ChamberOnly ||
                           Temperature.iIndexHeatMode==SocketChamber)
                        {
                            fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                        }
                        else
                        {
                            fTemp=atof(asGPIBTempShow[iNS8siteHead[i]].c_str());
                        }
                    }
                    else
                    {
                        bReturnNull=true;
                    }

                    if(iNS8siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && (bTestSiteUse[0][0][0] || bTestSiteUse[0][0][1] || bTestSiteUse[0][1][0] || bTestSiteUse[0][1][1]))
                    {
                        bUsed[0][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAa1 && bUsed[0][0]==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][1]==false && (bTestSiteUse[0][0][2] || bTestSiteUse[0][0][3] || bTestSiteUse[0][1][2] || bTestSiteUse[0][1][3]))
                    {
                        bUsed[0][1]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb1 && bUsed[0][1]==false)
                    {
                        bUsed[0][1]=true;
                    }  //-----------------------------------------------------------
                    else if(iNS8siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && (bTestSiteUse[1][0][0] || bTestSiteUse[1][0][1] || bTestSiteUse[1][1][0] || bTestSiteUse[1][1][1]))
                    {
                        bUsed[1][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAa2 && bUsed[1][0]==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][1]==false && (bTestSiteUse[1][0][2] || bTestSiteUse[1][0][3] || bTestSiteUse[1][1][2] || bTestSiteUse[1][1][3]))
                    {
                        bUsed[1][1]=true;
                    }
                    else if(iNS8siteHead[i]==tcAb2 && bUsed[1][1]==false)
                    {
                        bUsed[1][1]=true;
                    }  //
                    else
                    {
                        bwrite=false;
                    }
                }
                else
                {
                    if(bUT150Install[i8siteHead[i]] &&
                       bGet16HeaterUsedTo4Heater(i8siteHead[i]))
                    {
                        if(Temperature.iIndexHeatMode==ChamberOnly ||
                           Temperature.iIndexHeatMode==SocketChamber)
                        {
                            fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                        }
                        else
                        {
                            fTemp=atof(asGPIBTempShow[i8siteHead[i]].c_str());
                        }
                    }
                    else
                    {
                        bReturnNull=true;
                    }
                    if(i8siteHead[i]==tcAa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][0])
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcAb1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][0][1])
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcBa1 && bReturnNull==false && bUsed[0][0]==false && bTestSiteUse[0][1][0])
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcBb1 && bUsed[0][0]==false)
                    {
                        bUsed[0][0]=true;
                    }
                    else if(i8siteHead[i]==tcAc1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][0][2])
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i8siteHead[i]==tcAd1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][0][3])
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i8siteHead[i]==tcBc1 && bReturnNull==false && bUsed[0][1]==false && bTestSiteUse[0][1][2])
                    {
                        bUsed[0][1]=true;
                    }
                    else if(i8siteHead[i]==tcBd1 && bUsed[0][1]==false)
                    {
                        bUsed[0][1]=true;
                    }  //-----------------------------------------------------------
                    else if(i8siteHead[i]==tcAa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][0])
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcAb2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][0][1])
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcBa2 && bReturnNull==false && bUsed[1][0]==false && bTestSiteUse[1][1][0])
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcBb2 && bUsed[1][0]==false)
                    {
                        bUsed[1][0]=true;
                    }
                    else if(i8siteHead[i]==tcAc2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][0][2])
                    {
                        bUsed[1][1]=true;
                    }
                    else if(i8siteHead[i]==tcAd2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][0][3])
                    {
                        bUsed[1][1]=true;
                    }
                    else if(i8siteHead[i]==tcBc2 && bReturnNull==false && bUsed[1][1]==false && bTestSiteUse[1][1][2])
                    {
                        bUsed[1][1]=true;
                    }
                    else if(i8siteHead[i]==tcBd2 && bUsed[1][1]==false)
                    {
                        bUsed[1][1]=true;
                    }
                    else
                    {
                        bwrite=false;
                    }
                }
                if(bwrite)
                {
                    if(bReturnNull==true)
                    {
                        sTemp="NULL";
                    }
                    else
                    {
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                    }
                    t+=sTemp;
                    t+="_";
                }
            }
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                AnsiString sTemp1,sTemp2,sTemp3,sTemp4;
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());

                if(fTemp>=0)
                {
                    sTemp1.sprintf("+%1.1f",fTemp);
                    sTemp2.sprintf("+%1.1f",fTemp);
                    sTemp3.sprintf("+%1.1f",fTemp);
                    sTemp4.sprintf("+%1.1f",fTemp);
                }
                else
                {
                    sTemp1.sprintf("-%1.1f",fTemp);
                    sTemp2.sprintf("-%1.1f",fTemp);
                    sTemp3.sprintf("-%1.1f",fTemp);
                    sTemp4.sprintf("-%1.1f",fTemp);
                }

                if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==0)            //JerryYang 20160126 只使用ARM1
                {
                    sTemp3.sprintf("NULL");
                    sTemp4.sprintf("NULL");
                }
                else if(TestIF.iShuttleMode==1 && TestIF.iShuttle_Sel==1)       //JerryYang 20160126 只使用ARM2
                {
                    sTemp1.sprintf("NULL");
                    sTemp2.sprintf("NULL");
                }

                if(bTestSiteUse[0][0][0]==false && bTestSiteUse[0][0][1]==false &&      //左邊全關
                   bTestSiteUse[0][1][0]==false && bTestSiteUse[0][1][1]==false)
                {
                    sTemp1.sprintf("NULL");
                }
                if(bTestSiteUse[0][0][2]==false && bTestSiteUse[0][0][3]==false &&
                   bTestSiteUse[0][1][2]==false && bTestSiteUse[0][1][3]==false)
                {
                    sTemp2.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][0]==false && bTestSiteUse[1][0][1]==false &&
                   bTestSiteUse[1][1][0]==false && bTestSiteUse[1][1][1]==false)
                {
                    sTemp3.sprintf("NULL");
                }
                if(bTestSiteUse[1][0][2]==false && bTestSiteUse[1][0][3]==false &&
                   bTestSiteUse[1][1][2]==false && bTestSiteUse[1][1][3]==false)
                {
                    sTemp4.sprintf("NULL");
                }
                t.sprintf("%s_%s_%s_%s_", sTemp1, sTemp2, sTemp3, sTemp4);
            }
        }
       else if(TestIF.iTestMode==_16Site2X8)                                   //Sam 20241128 : 新增 2X8 溫度回傳
        {
            int i2X8_HeaterOneByOne[]={tcAa1, tcBa1, tcAb1, tcBb1, tcAc1, tcBc1, tcAd1, tcBd1,
                                       tcAe1, tcBe1, tcAf1, tcBf1, tcAg1, tcBg1, tcAh1, tcBh1,
                                       tcAa2, tcBa2, tcAb2, tcBb2, tcAc2, tcBc2, tcAd2, tcBd2,
                                       tcAe2, tcBe2, tcAf2, tcBf2, tcAg2, tcBg2, tcAh2, tcBh2};

            int i2X8_HeaterOneByTwo[]={tcAa1, tcBa1, tcAb1, tcBb1, tcAc1, tcBc1, tcAd1, tcBd1,
                                       tcAa2, tcBa2, tcAb2, tcBb2, tcAc2, tcBc2, tcAd2, tcBd2};
            int iHeater=0;
            int iAdrr=0;

            if(Temperature.iIndexHeatMode==ChamberOnly || Temperature.iIndexHeatMode==SocketChamber)
            {
                fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                if(fTemp>=0)
                    sTemp.sprintf("+%1.1f",fTemp);
                else
                    sTemp.sprintf("-%1.1f",fTemp);

                for(int iCol=0; iCol<8; iCol++)
                {
                    for(int iRow=0; iRow<2; iRow++)
                    {
                        if(bTestSiteUse[0][iRow][iCol])
                            t+=sTemp+"_";
                        else
                            t+="NULL_";
                    }
                }
                for(int iCol=0; iCol<8; iCol++)
                {
                    for(int iRow=0; iRow<2; iRow++)
                    {
                        if(bTestSiteUse[1][iRow][iCol])
                            t+=sTemp+"_";
                        else
                            t+="NULL_";
                    }
                }
            }
            else
            {
                if(TestIF_File.bUse32Heater)
                    iHeater=sizeof(i2X8_HeaterOneByOne)/sizeof(int);
                else
                    iHeater=sizeof(i2X8_HeaterOneByTwo)/sizeof(int);

                for(int i=0; i<iHeater; i++)
                {
                    if(TestIF_File.bUse32Heater)
                        iAdrr=i2X8_HeaterOneByOne[i];
                    else
                        iAdrr=i2X8_HeaterOneByTwo[i];
                    if(bUT150Install[iAdrr] &&bGetHeaterUsed(iAdrr))
                    {
                        fTemp=atof(asGPIBTempShow[iAdrr].c_str());
                        if(fTemp>=0)
                            sTemp.sprintf("+%1.1f",fTemp);
                        else
                            sTemp.sprintf("-%1.1f",fTemp);
                        t+=sTemp+"_";
                    }
                    else
                    {
                         t+="NULL_";
                    }
                }
            }
        }

        for(int i=tcSocket; i<=tcChamber; i++)
        {
            fTemp=999.9;
            bReturnNull=false;
            if(bUT150Install[i] && bGetHeaterUsed(i))
            {
                if(Temperature.iIndexHeatMode==ChamberOnly ||
                   Temperature.iIndexHeatMode==SocketChamber)
                    fTemp=atof(asGPIBTempShow[tcChamber].c_str());
                else
                    fTemp=atof(asGPIBTempShow[i].c_str());
            }
            else
            {
//                if(bGetHeaterUsed(i) && i==tcChamber && ATC_SYSTEM==eNonChamber)
//                {
//                    fTemp=atof(asGPIBTempShow[tcChamber].c_str());
//                }
//                else
                {
                    bReturnNull=true;
                }
            }

            if(bReturnNull==true)
            {
                sTemp="NULL";
            }
            else
            {
                if(fTemp>=0)
                    sTemp.sprintf("+%1.1f",fTemp);
                else
                    sTemp.sprintf("-%1.1f",fTemp);
            }

            t+=sTemp;
            if(i<tcChamber)
            {
                t+="_";
            }
        }
    }

    for(int i=tcHotPlate1; i<=tcShuttle2; i++)
    {
        fTemp = 999.9;
        if(bUT150Install[i] && bGetHeaterUsed(i))
        {
            fTemp=atof(asGPIBTempShow[i].c_str());
            if(fTemp >= 0)
            {
                sTemp.sprintf("+%1.1f",fTemp);
            }
            else
            {
                sTemp.sprintf("-%1.1f",fTemp);
            }
        }
        else
        {
            sTemp="NULL";
        }

        sCMDStr+=sTemp;
        if(i<tcShuttle2)
        {
            sCMDStr+="_";
        }
    }

    asStr=sCMDStr+"_"+t+"\r\n";                                                 //Sam 20210310 : SetupFile 矽格卓邵要求增加換行字元
    SendMSG_CMD(MSG_CMD_GetNowAllTemp, asStr);
}
//---------------------------------------------------------------------------
void TfMain::WriteChkSetup()                                                    // wei 20151125 Add CHKSETUP? Command
{
//Handler Respond
//Setup_1|P|90_2|F|0.05_3|F|1_4|F|2_5|F|3_6|F|4
//上行代表好品bin1卡關90%(小於90%需alarm)，不良品bin2~6分別卡關0.05%與1~4% (大於等於需alarm)
//Setup_1|P|97_2|P|97_6|F|1
//上行代表有複數個好品bin1和bin2，當有複數個好品bin，他們的卡關yield會是一樣的，以此例子請卡bin1+bin2需大於等於97%(若小於需alarm)
//原先是：Setup_bin|status|yield(%)_bin|status|yield(%)_......
//變更為：Setup_SiteDiff_bin|status|yield(%)_bin|status|yield(%)_......
//SiteDiff：site different的yield，單位是百分比，若不需要設定會代NULL
//e.g.1 Setup_5_1|P|90_2|F|0.05_3|F|1_4|F|2_5|F|3_6|F|4
//e.g.2 Setup_NULL_1|P|90_2|F|0.05_3|F|1_4|F|2_5|F|3_6|F|4

    AnsiString sTemp="", sTemp1;
    AnsiString sAlarmData = "";
    int i=0;
    SetWorkParameter();                                                         //Steven 20120130 : 存檔後要重新load參數
    double dVal =0.0;

    if(IniConfig.bSIGURDFunction)
    {
        if(Prod.bFailAlarmSiteYieldCmp)
        {
            sTemp1.sprintf("%f", Prod.dFailAlarmSiteYieldCmp);
            dVal = atof(sTemp1.c_str());
            sTemp1=FloatToStr(dVal);
            sTemp="SETUP_"+sTemp1;                                              //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        else
        {
            sTemp="SETUP_NULL";
        }
    }
    else
    {
        if(Prod.bFailAlarmSiteYieldDifferent)
        {
            sTemp1.sprintf("%f", Prod.dFailAlarmSiteYield);                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
            dVal = atof(sTemp1.c_str());
            sTemp1=FloatToStr(dVal);
            sTemp="SETUP_"+sTemp1;                                              //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        else
        {
            sTemp="SETUP_NULL";
        }
    }

    sAlarmData = sAlarmData + sTemp;

    //Bin Alarm Yield
    for(i=0; i<16; i++)                                                         //JerryYang 20151226
    {
        AnsiString sBinName = "";
        AnsiString sPassOrFail = "";
        AnsiString sYield = "";

        if(BinSelect[iTestRunMode].iCatDataT3Pos[i]>0)                          //JerryYang 20151210    //Steven 20230929 : !=0 --> >0
        {
            sBinName = IntToStr(i);
            if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                  //KaiChen 20200108 ：矽格-湖口，修改 CHKSETUP?，設定 SiteYieldCmp 和 LowYieldByTotal
            {
                if(Prod.bIsPassBin[i] && Prod.bFailAlarmLowYieldByTotal)
                {
                    sPassOrFail = "P";
                    sYield.sprintf("%f", Prod.dLowYieldLimitByTotal);
                }
                else
                {
                    if(BinSelect[iTestRunMode].bFailure[i]==true)
                    {
                        sPassOrFail = "F";
                        sYield.sprintf("%f",Prod.dFailureLimit[i]);
                    }
                }
            }
            else
            {
                if(Prod.bIsPassBin[i] && TestIF_File.bFailAlarmLowYield)        //JerryYang 20160202 判斷是否開啟Low yield enable
                {
                    sPassOrFail = "P";
                    sYield.sprintf("%f", Prod.dLowYieldLimit);                  //JerryYang 20160530 LowYieldLimit要能設定到小數點
                }
                else
                {
                    if(BinSelect[iTestRunMode].bFailure[i]==true)
                    {
                        sPassOrFail = "F";
                        sYield.sprintf("%f",Prod.dFailureLimit[i]);
                    }
                }
            }
            //sYield.sprintf("%1.0f",Prod.fBinAlarmLimit[i]);
            dVal = atof(sYield.c_str());
            sYield = FloatToStr(dVal);

            if(sYield!=0) //JerryYang 20151210 濾掉Yield=0
            {
                sTemp.sprintf("_%s|%s|%s",sBinName,sPassOrFail,sYield);
                sAlarmData = sAlarmData + sTemp;
            }
        }
    }

    SendMSG_CMD(MSG_CMD_ChkSetup, sAlarmData);
}
//---------------------------------------------------------------------------
void TfMain::WriteHandlerTestArmEncoder()                                       //wei 20160122
{
    AnsiString t="";
    long iArm1_Y=0, iArm1_Z=0, iArm2_Y=0, iArm2_Z=0;

    iArm1_Y=MOT[MTestY1].Gali_ReadPos();
    iArm1_Z=MOT[MTestZ1].Gali_ReadPos();
    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
        iArm2_Y=MOT[MTestY2].Gali_ReadPos();
    iArm2_Z=MOT[MTestZ2].Gali_ReadPos();

    t.sprintf("ARM1_Y_%d_Z_%d_ARM2_Y_%d_Z_%d\r",iArm1_Y,iArm1_Z,iArm2_Y,iArm2_Z );

    SendMSG_CMD(MSG_CMD_GetTestArmPos, t);
}
//---------------------------------------------------------------------------
void TfMain::WriteHandlerTestArmEP()                                            //wei 20160122
{
    AnsiString t;
    double dValue=0.0;
    t.sprintf("%d PA\r", ADAM_ReadPA(&dValue));                                 //wei 20220309 Add EP Return Voltage
    SendMSG_CMD(MSG_CMD_GetTestArmEP, t);
}
//---------------------------------------------------------------------------
void TfMain::SetSiteMapData()                                                   //wei 20151127 GPIB Change Site Map
{
    bool ret=true;
    AnsiString t;
    char str[256];

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));

    ret=HasICUnderMachine();
    if(ret==false)
    {
        for(int i=0; i<4; i++)                                                  //JerryYang 20160519 備份開關site
        {
            for(int j=0; j<8; j++)
            {
               iBackupDutOnOff[i][j]=bTestSiteUse[0][i][j];
            }
        }
        iBackupTestMode=TestIF_File.iTestMode;
        ret=ChangeToSiteMap(str);
    }
    else
    {
        ret=false;
    }

    if(ret)
        t.sprintf("SETTINGOK");
    else
        t.sprintf("SETTINGNG");

    SendMSG_CMD(MSG_CMD_SetSiteMapData, t);
}
//---------------------------------------------------------------------------
void TfMain::SetAlarmSetup()                                                    // wei 20151125 Add CHKSETUP? Command
{
    bool ret=false;
    AnsiString t;
    char str[256];
    AnsiString asLog;
    GetTimeInfo();

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));

    iRecordSigurdGPIBFlag=1;                                                    //KaiChen 20200330 ：紀錄，矽格 GPIB Commend Flag

    if(SystemStart==false &&
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        iRecordSigurdGPIBFlag=10;                                               //KaiChen 20200330 ：紀錄，矽格 GPIB Commend Flag
        if(IniConfig.bSIGURDFunction)
            ret=ChangeToAlarmSetup_SG(str);
        else
            ret=ChangeToAlarmSetup(str);
    }
    else
    {
        iRecordSigurdGPIBFlag=11;                                               //KaiChen 20200330 ：紀錄，矽格 GPIB Commend Flag
        ret=false;
    }

    //KaiChen 20200330 ：紀錄，矽格 GPIB Commend Flag
    //==>
    AnsiString sPathName;
    sPathName.sprintf("D:\\HT9045_Log\\Sigurd_GPIB");
    MyForceDirectories(sPathName);
    sPathName.sprintf("D:\\HT9045_Log\\Sigurd_GPIB\\GPIBLog_%04d%02d%02d.txt", SystemYear, SystemMonth, SystemDate);
    asLog.sprintf("GPIBFlag=%d, %s", iRecordSigurdGPIBFlag, AnsiString(str));
    WriteDataToFile(sPathName,asLog);
    //WriteIniData(sPathName, "GPIBLog",    "SETUP",       iRecordSigurdGPIBFlag);
    //<==
    //KaiChen 20200330 ：紀錄，矽格 GPIB Commend Flag

    if(ret)
        t.sprintf("SETTINGOK");
    else
        t.sprintf("SETTINGNG");

    SendMSG_CMD(MSG_CMD_SetAlarmSetup, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetCZtesterBin()                                        //JerryYang 20160308 回傳各Bin數量
{
    int iBinCount[1024];
    AnsiString sTestBinCount="";

    ZeroMemory(iBinCount, sizeof(iBinCount));
    for(int i=0; i<iTestBinCount; i++)
    {
        if(BinSelect[iTestRunMode].iCatDataT3Pos[i]!=0 || LastSet.iBinData32[0][i]!=0)    //回傳有設定Tray盤及數量不為0的bin數量
        {
            iBinCount[i]=LastSet.iBinData32[0][i];
        }
        else
        {
            iBinCount[i]=-1;
        }
    }
    for(int i=0; i<iTestBinCount; i++)
    {
        if(iBinCount[i]>=0)
        {
            sTestBinCount=(sTestBinCount+IntToStr(i)+"-"+IntToStr(iBinCount[i])+",");
        }
    }
    sTestBinCount=sTestBinCount+"U-0";

    SendMSG_CMD(MSG_CMD_TesterBin, sTestBinCount);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetCZSoakTime()                                         //JerryYang 20160315 回傳soak time
{
    AnsiString t;
    if(LastSet.iTemperature==Tempture_Ambient)
        t="NONE";
    else
        t=edSoakTime->Text;
    SendMSG_CMD(MSG_CMD_SoakTime, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetCZDoubleContactCount()                               //Isaac 20210706 : add MSG_CMD_DoubleContactCount指令，詢問handler doublecontact次數
{
    AnsiString t;
    if(Prod.bD22SupportMultiDoubleContact==false)
       //IniConfig.bD22VerifyMode==false)                                       //Sam 20231117 : 整合到 QA 模式//Sam 20221012 : 新增 VerifyMode 功能
        t="0";
    else
        t=IntToStr(Prod.iD22DoubleContactCount+1);                              //Sam 20231117 : 整合到 QA 模式
    SendMSG_CMD(MSG_CMD_DoubleContactCount, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetCDHandlerID()                                        //JerryYang 20190627 回傳handler ID
{
    AnsiString t;
    t=IniConfig.SocketHandlerID;
    SendMSG_CMD(MSG_CMD_HanderIDRS232, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetCZJamCode()                                          //JerryYang 20160315 回傳Jam Code
{
    AnsiString t, sTemp;
    if(fNote->fShow)                                                            //Isaac 20170825(jou) 回傳JAM增加支援WAR,MES
    {
        sTemp=fNote->edErrorCode->Text;
        sTemp.Delete(1, 3);                                                     //刪除JAM字元 只回傳Jam code的數字部分
        t=sTemp;
    }
    else
    {
        t="0";                                                                  //沒有發生Jam的時候回傳Jam code為"0"
    }
    SendMSG_CMD(MSG_CMD_JamCode, t);                                            //JerryYang 20200324 fix回傳jam code異常
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetCZSiteMap(bool bSendGPIB)                            //JerryYang 20160324 回傳SiteMap
{
    AnsiString sTemp, t="";

    if(TestIF_File.iTestMode==SingleSite)                                       // Single Site 1x1
    {
        t.sprintf("%d", TestIF_File.iSiteMap[0][0]);
    }
    else if(TestIF_File.iTestMode==DualSite)                                    // Dual Site 1x2
    {
        t.sprintf("%d,%d", TestIF_File.iSiteMap[0][0],
                           TestIF_File.iSiteMap[0][1]);
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {
        t.sprintf("%d,%d,%d", TestIF_File.iSiteMap[0][0],
                              TestIF_File.iSiteMap[0][1],
                              TestIF_File.iSiteMap[0][2]);
    }
    else if(TestIF_File.iTestMode==QualSite1X4)
    {
        t.sprintf("%d,%d,%d,%d", TestIF_File.iSiteMap[0][0],
                                 TestIF_File.iSiteMap[0][1],
                                 TestIF_File.iSiteMap[0][2],
                                 TestIF_File.iSiteMap[0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1)                                 // Dual Site 2x1
    {
        t.sprintf("%d,%d", TestIF_File.iSiteMap[0][0],
                           TestIF_File.iSiteMap[1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2 ||                               // Qual Site 2x2
            TestIF_File.iTestMode==QualSite2X2N)
    {
        t.sprintf("%d,%d,%d,%d", TestIF_File.iSiteMap[0][0],
                                 TestIF_File.iSiteMap[1][0],
                                 TestIF_File.iSiteMap[0][1],
                                 TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||                                 //ChungHung 20140115 add for 2x3_6
            TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
    {
        t.sprintf("%d,%d,%d,%d,%d,%d", TestIF_File.iSiteMap[0][0],
                                       TestIF_File.iSiteMap[1][0],
                                       TestIF_File.iSiteMap[0][1],
                                       TestIF_File.iSiteMap[1][1],
                                       TestIF_File.iSiteMap[0][2],
                                       TestIF_File.iSiteMap[1][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||                                 // 8 Site 2x4
            TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d",
                    TestIF_File.iSiteMap[0][0],
                    TestIF_File.iSiteMap[1][0],
                    TestIF_File.iSiteMap[0][1],
                    TestIF_File.iSiteMap[1][1],
                    TestIF_File.iSiteMap[0][2],
                    TestIF_File.iSiteMap[1][2],
                    TestIF_File.iSiteMap[0][3],
                    TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",                        //wei 20150702 移除4個Site
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16Site 2x8
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3]);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        t.sprintf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     TestIF_File.iSiteMap[0][0],
                     TestIF_File.iSiteMap[1][0],
                     TestIF_File.iSiteMap[2][0],
                     TestIF_File.iSiteMap[3][0],
                     TestIF_File.iSiteMap[0][1],
                     TestIF_File.iSiteMap[1][1],
                     TestIF_File.iSiteMap[2][1],
                     TestIF_File.iSiteMap[3][1],
                     TestIF_File.iSiteMap[0][2],
                     TestIF_File.iSiteMap[1][2],
                     TestIF_File.iSiteMap[2][2],
                     TestIF_File.iSiteMap[3][2],
                     TestIF_File.iSiteMap[0][3],
                     TestIF_File.iSiteMap[1][3],
                     TestIF_File.iSiteMap[2][3],
                     TestIF_File.iSiteMap[3][3],
                     TestIF_File.iSiteMap[0][4],
                     TestIF_File.iSiteMap[1][4],
                     TestIF_File.iSiteMap[2][4],
                     TestIF_File.iSiteMap[3][4],
                     TestIF_File.iSiteMap[0][5],
                     TestIF_File.iSiteMap[1][5],
                     TestIF_File.iSiteMap[2][5],
                     TestIF_File.iSiteMap[3][5],
                     TestIF_File.iSiteMap[0][6],
                     TestIF_File.iSiteMap[1][6],
                     TestIF_File.iSiteMap[2][6],
                     TestIF_File.iSiteMap[3][6],
                     TestIF_File.iSiteMap[0][7],
                     TestIF_File.iSiteMap[1][7],
                     TestIF_File.iSiteMap[2][7],
                     TestIF_File.iSiteMap[3][7]);
    }
    else
    {
        t.sprintf("UNKNOWN", sizeof(t));
    }
    aSendSiteMapping=t;
    if(bSendGPIB==true)
        SendMSG_CMD(MSG_CMD_SiteMap, t);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetCZAllMassTemp()                                      //JerryYang 20160324 回傳all mass temp
{
    AnsiString asNULL="NULL", t="";

    if(TestIF.iTestMode==SingleSite)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s", asGPIBTempShow[tcHead1]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s", asGPIBTempShow[tcAa1]);
            }
            else
            {
                t.sprintf("%s", asGPIBTempShow[tcHead1]);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s", asGPIBTempShow[tcHead3]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s", asGPIBTempShow[tcAa2]);
            }
            else
            {
                t.sprintf("%s", asGPIBTempShow[tcHead3]);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s", asGPIBTempShow[tcChamber]);
            }
            else
            {
                t.sprintf("NULL");
            }
        }
    }
    else if(TestIF.iTestMode==DualSite)                                         // Dual Site 1x2
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber], asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL ,
                                   LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                   LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber], asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                   LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                   LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead3]:asNULL,
                                   (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead4]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s", asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                      LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s", asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber],
                                      asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                      LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                      LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                      LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                      LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead3]:asNULL,
                                      (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcHead3]:asNULL,
                                      (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcHead4]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==QualSite1X4)                                      // Qual Site 1x4
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL,
                                         LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAd2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead4]:asNULL,
                                         LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcHead3]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcHead3]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcHead4]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcHead4]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==DualSite2x1)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber],
                                   asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                   LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                   LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", asGPIBTempShow[tcChamber],
                                   asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                   LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                   LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                                   (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2)                                      // Qual Site 2x2
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                         LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                                         LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                         LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                                         LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==1))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber],
                                         asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                         LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                                         LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBb2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                         LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                                         LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                                         (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                                         (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                                         (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==QualSite2X2N)                                     // Qual Site 2x2
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead1]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s" ,
                            LastSet.bUseTestSocket[0][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][1] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[1][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[1][0][1] ? asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3)
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130820 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAc1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBc1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130820 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAc2]:asNULL,
                                               LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                                               LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                                               LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                                               LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                                               LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                                               LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130820 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                                               (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber],
                                               asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAc2]:asNULL,
                                               LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAc1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                               LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                               LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)
            {
                t.sprintf("%s %s %s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAc2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcAd2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAc1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_8Site2X4)                                        // 8 Site 2x4
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                                                     LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down || (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber],
                                                     asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBd2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s", LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                                                     LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s", (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                                                     (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_10Site2X5)                                       //wei 20190614 10 site
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe2]:asNULL);

            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][4] || LastSet.bUseTestSocket[1][0][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3] ? asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][4] || LastSet.bUseTestSocket[1][1][4] ? asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_12Site2X6)                                       //12 Site   //wei 20150702
    {
        if(IndexStatus==Z1Down_Z2Up ||
          (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf2]:asNULL);

            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s",
                            (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][4] || LastSet.bUseTestSocket[1][0][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][5] || LastSet.bUseTestSocket[1][0][5])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][4] || LastSet.bUseTestSocket[1][1][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][5] || LastSet.bUseTestSocket[1][1][5])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_16Site2X8)                                       //16 Site
    {
        if(IndexStatus==Z1Down_Z2Up ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf1]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAg1]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAh1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf1]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBg1]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBh1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else if(IndexStatus==Z1Up_Z2Down ||
                (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcAf2]:asNULL,
                            LastSet.bUseTestSocket[1][0][6]?asGPIBTempShow[tcAg2]:asNULL,
                            LastSet.bUseTestSocket[1][0][7]?asGPIBTempShow[tcAh2]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcBe2]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcBf2]:asNULL,
                            LastSet.bUseTestSocket[1][1][6]?asGPIBTempShow[tcBg2]:asNULL,
                            LastSet.bUseTestSocket[1][1][7]?asGPIBTempShow[tcBh2]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[1][0][6]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[1][0][7]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[1][1][6]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[1][1][7]?asGPIBTempShow[tcBd2]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[1][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][4]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][5]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][6]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][0][7]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[1][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][3]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][5]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][6]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[1][1][7]?asGPIBTempShow[tcHead4]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            (LastSet.bUseTestSocket[0][0][0] || LastSet.bUseTestSocket[1][0][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][1] || LastSet.bUseTestSocket[1][0][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][2] || LastSet.bUseTestSocket[1][0][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][3] || LastSet.bUseTestSocket[1][0][3])?asGPIBTempShow[tcChamber]:asNULL,

                            (LastSet.bUseTestSocket[0][0][4] || LastSet.bUseTestSocket[1][0][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][5] || LastSet.bUseTestSocket[1][0][5])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][6] || LastSet.bUseTestSocket[1][0][6])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][0][7] || LastSet.bUseTestSocket[1][0][7])?asGPIBTempShow[tcChamber]:asNULL,

                            (LastSet.bUseTestSocket[0][1][0] || LastSet.bUseTestSocket[1][1][0])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][1] || LastSet.bUseTestSocket[1][1][1])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][2] || LastSet.bUseTestSocket[1][1][2])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][3] || LastSet.bUseTestSocket[1][1][3])?asGPIBTempShow[tcChamber]:asNULL,

                            (LastSet.bUseTestSocket[0][1][4] || LastSet.bUseTestSocket[1][1][4])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][5] || LastSet.bUseTestSocket[1][1][5])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][6] || LastSet.bUseTestSocket[1][1][6])?asGPIBTempShow[tcChamber]:asNULL,
                            (LastSet.bUseTestSocket[0][1][7] || LastSet.bUseTestSocket[1][1][7])?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_16Site4X4)
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if(USE_16_HEATER==eht16Heater       ||                         //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                    USE_16_HEATER==eht16HeaterEJ1N   ||
                    USE_16_HEATER==eht32HeaterEJ1N   ||                         //Steven 20140923 : Index使用EJ1N版32組加熱器
                    USE_16_HEATER==eht32HeaterKT4H   ||                         //Steven 20150211 : Index使用KT4H版32組加熱器
                    USE_16_HEATER==eht16HeaterDTME08 ||                         //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                    USE_16_HEATER==eht32HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                          LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                          LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                          LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                          LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                          LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                          LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                          LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                          LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,

                          LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcAa1]:asNULL,
                          LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcAb1]:asNULL,
                          LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcAc1]:asNULL,
                          LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcAd1]:asNULL,
                          LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcBa1]:asNULL,
                          LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcBb1]:asNULL,
                          LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcBc1]:asNULL,
                          LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                          LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                          LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead4]:asNULL,
                          LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead4]:asNULL,
                          LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead4]:asNULL,
                          LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead4]:asNULL,

                          LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcHead1]:asNULL,
                          LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcHead2]:asNULL,
                          LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcHead2]:asNULL,
                          LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcHead2]:asNULL,
                          LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                          LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcChamber]:asNULL,

                          LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcChamber]:asNULL,
                          LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else if(TestIF.iTestMode==_32Site4X8N)
    {
        if(IndexStatus==Z1_Z2_Down ||
           (iContactMode!=CONTACT_NORMAL && iIndexArm==0))
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber],
                            asGPIBTempShow[tcChamber]);
            }
            else if((USE_16_HEATER==eht32HeaterEJ1N ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                     USE_16_HEATER==eht32HeaterKT4H ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                     USE_16_HEATER==eht32HeaterDTME08) &&                       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                     TestIF_File.bUse32Heater)
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAe2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAf2]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAg2]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAh2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBe2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBf2]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBg2]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBh2]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcAe1]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcAf1]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcAg1]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcAh1]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcBe1]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcBf1]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcBg1]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcBh1]:asNULL);
            }
            else if(USE_16_HEATER==eht16Heater      ||
                    USE_16_HEATER==eht16HeaterEJ1N  ||
                    USE_16_HEATER==eht16HeaterDTME08 )                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcAa2]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcAb2]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcAc2]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcAd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcBa2]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcBb2]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcBc2]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcBd2]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcAa1]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcAb1]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcAc1]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcAd1]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcBa1]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcBb1]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcBc1]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcBd1]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcBd1]:asNULL);
            }
            else
            {
                 t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcHead3]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcHead4]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcHead1]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcHead2]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcHead2]:asNULL);
            }
        }
        else
        {
            if(Temperature.iIndexHeatMode==ChamberOnly ||
               Temperature.iIndexHeatMode==SocketChamber)                       //kevin 20130705 高雄日月光
            {
                t.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                            LastSet.bUseTestSocket[0][0][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][0][7]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][1][7]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][2][7]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][0]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][1]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][2]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][3]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][4]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][5]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][6]?asGPIBTempShow[tcChamber]:asNULL,
                            LastSet.bUseTestSocket[0][3][7]?asGPIBTempShow[tcChamber]:asNULL);
            }
            else
            {
                t.sprintf("NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL NULL");
            }
        }
    }
    else
    {
        t.sprintf("UNKNOWN", sizeof(t));
    }

    SendMSG_CMD(MSG_CMD_AllMassTemp, t);
}
//---------------------------------------------------------------------------
extern int iTestHeadMotorTask;
extern int iTestYTask;
extern int LoadTask;
void __fastcall TfMain::MachineStatus() //JerryYang 20151109 回覆tester機台狀態
{
    int i;
    bool bEMG=false;//JerryYang 20160125 判斷EMG是否被按下
    AnsiString ErrorCode=fNote->edErrorCode->Text;

    HHandler2Gpib.iStatus[Bit0_HandlerReBoot]=0;

    //------------判斷是否無空盤可放至Auto
    if(fNote->fShow &&
       (ErrorCode=="MES1021" || ErrorCode=="MES1421" ||
        ErrorCode=="MES1120" || ErrorCode=="MES1220" || ErrorCode=="MES1320" ||
        ErrorCode=="MES2520" || ErrorCode=="MES2620" || ErrorCode=="MES2720" || //Steven 20230907 : For HT-9011UC
        ErrorCode=="MES1720" || ErrorCode=="MES1820" || ErrorCode=="MES1920" ||
        ErrorCode=="MES2820" || ErrorCode=="MES2920" || ErrorCode=="MES3020") )
    {
        HHandler2Gpib.iStatus[Bit1_HandlerOutputFull]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit1_HandlerOutputFull]=0;
    }

    //----------判斷Loader 是否有Tray
    if(MOT[MMTrayY].HasIC()==false)
    {
        HHandler2Gpib.iStatus[Bit2_HandlerInputEmpty]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit2_HandlerInputEmpty]=0;
    }

    //----------判斷是否在做AutoClean
    if(bAutoCleaning==true)
    {
        HHandler2Gpib.iStatus[Bit3_ContactorCleaning]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit3_ContactorCleaning]=0;
    }

    //----------判斷是否在設定參數
    if((fTeach->fShow || fMotorTest->fShow || fShuttleMove->fShow || fHome->fShow) ||
       (fContact->fShow && fContact->rbModeNormal->Checked==false) ||
       (SystemStart==false &&
        (fSetup->fShow      || fOffSet->fShow       || fConfiguration->fShow    ||
         fSpeed->fShow      || fDIOFrom->fShow      || fYieldMonitoring->fShow  ||
         fTrayForm->fShow   || fHotPlate->fShow     || fTrayAssignment->fShow   ||
         fTemp_Set->fShow   || FTestIF->fShow       || fCounterClear->fShow     ||
         fLd_ULd->fShow     || fCCLink->bShow       || fTowerLight->fShow       ||
         fCleaning->fShow   || fQAMode->fShow       || fCounterSel->fShow       ||
         FrmRotate->fShow   || fBuilder->fShow      || fStartCondition->fShow   ||
         fBarCode->bShow    || fSecurity->fShow     || fBinSel->bShow           ||
         fLtcSensor->bShow  || fOmron->bShow        || fContact->fShow          ||
         fiosetview->fShow)))
    {
        HHandler2Gpib.iStatus[Bit4_HandlerDiagnostics]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit4_HandlerDiagnostics]=0;
    }

    if(SystemStart==false || (iTestHeadMotorTask==600 && iTestYTask>50))  //Steven 20171016 (wei) : 做Index Check的時候
    {
        HHandler2Gpib.iStatus[Bit5_IndexCheck]=0;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit5_IndexCheck]=1;
    }

    HHandler2Gpib.iStatus[Bit6_Reversed]=0;

    //------------判斷機台是否等待加熱
    if(fHeaterOK==false && LastSet.iTemperature!=Tempture_Ambient)              //加熱中
    {
        HHandler2Gpib.iStatus[Bit7_HandlerGuardband]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit7_HandlerGuardband]=0;
    }

    //----------判斷機台是否JAM
    if(fNote->fShow && fNote->edErrorCode->Text!="" && fSecurity->GetBit8(fNote->Edit3->Text+" "+fNote->edUnitName->Text, fNote->edErrorCode->Text)==1)                //Isaac 20170825(jou) 回傳JAM增加支援WAR,MES
    {
        HHandler2Gpib.iStatus[Bit8_HandlerJam]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit8_HandlerJam]=0;
    }

    //----------判斷機台是否STOP
    AnsiString asStrTemp=fMain->palMainStatus->Caption.UpperCase();             //JerryYang 20160127 機台停止中
    if(SystemStart==false ||
       Sen[SnFrontLeftEMG].IsOff() || Sen[SnFrontRightEMG].IsOff() ||           //緊停被按下時
       Sen[SnRearLeftEMG ].IsOff() || Sen[SnRearRightEMG ].IsOff() ||
       Sen[SnMotorPower].IsOff()   || Sen[SnSystemPower].IsOff()   ||
       (Enable_PLCSafety_IO && Sen[SnAllEMG].IsOff()))                          //KenHsieh 20250212 : 新增PLC 斷線可瞬間判斷EMG及安全門
    {
        bEMG=true;
    }
    if(asStrTemp=="PAUSE" || asStrTemp=="Alarm" || asStrTemp=="LOCK" || bEMG)
    {
        HHandler2Gpib.iStatus[Bit9_HandlerStop]=1;
    }
    else                                                                        //JerryYang 20170704 (Steven) 修正handler stop狀態錯誤
    {
        HHandler2Gpib.iStatus[Bit9_HandlerStop]=0;
    }

    //-----------判斷是否在預熱中
    if((fHeaterOK==true && LastSet.iTemperature!=Tempture_Ambient) &&           //JerryYang 20160126 Device預熱中
       (iHeaterWaitTime>0 || iInitialSoakTimer>0 || iSoakTimer>0))
    {
        HHandler2Gpib.iStatus[Bit10_HandlerSoak]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit10_HandlerSoak]=0;
    }
    //-----------判斷安全門
    for(i=0; i<MAX_SAFE_DOOR_CNT; i++)                                          //JerryYang 20230704 : 整合安全門15->MAX_SAFE_DOOR_CNT
    {
        if(Sen[iSafeDoor[i]].IsOff()==true)
        {
            HHandler2Gpib.iStatus[Bit11_HandlerDoorOpen]=1;
            break;
        }
        else
        {
            HHandler2Gpib.iStatus[Bit11_HandlerDoorOpen]=0;
        }
    }

    //----------判斷Handler內有無IC
    if(HasICUnderMachine()==true || MOT[MMTrayY].HasIC()==true)                 //JerryYang 20160126 loader沒有IC
    {
        HHandler2Gpib.iStatus[Bit12_HandlerEmpty]=0;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit12_HandlerEmpty]=1;
    }

    HHandler2Gpib.iStatus[Bit13_HandlerOk]=0;

    //------------判斷是否正在Unloading
    if(bUnloading)
    {
        HHandler2Gpib.iStatus[Bit14_Unloading]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit14_Unloading]=0;
    }

    //------------判斷是否正在Loading
    if(LoadTask==1000 || LoadTask==900)                                         //Steven 20171016 (wei) : 用來判斷機台是否正在loading
    {
        HHandler2Gpib.iStatus[Bit15_Loading]=1;
    }
    else
    {
        HHandler2Gpib.iStatus[Bit15_Loading]=0;
    }

    //----------判斷IC是否都在Output
    HHandler2Gpib.iStatus[Bit16_Reversed]=0;
    if(CUSTOMER_CODE==CC_AMKOR_Philippines)
    {
    }
    else
    {
        if(InArmSuck.HasIC()==false &&  OutArmSuck.HasIC()==false && ShuttleHasIC()==false &&
           IndexHasIC()==false && MOT[MMPlate1].HasIC()==false && MOT[MMPlate2].HasIC()==false)
        {
            for(int i=0; i<eTrayCount; i++)
            {
                if(Prod.iTrayType[i]!=tNotUse)
                {
                    if(MOT[iMMAuto[i]].HasIC()==true)
                    {
                        HHandler2Gpib.iStatus[Bit16_Reversed]=1;
                        break;
                    }
                }
            }
        }
    }

    HHandler2Gpib.iSendCommand=MSG_CMD_MachineState;                            //JerryYang 20151109 回覆機台狀態
    memset(HHandler2Gpib.Message,'\0', sizeof(HHandler2Gpib.Message));          //清空陣列
    COPYDATASTRUCT *pcp=new COPYDATASTRUCT;
    pcp->dwData=0;
    pcp->cbData=sizeof(HHandler2Gpib);
    pcp->lpData=(unsigned char *)&HHandler2Gpib.iSendCommand;

    SendMessage(fMain->HVisionWnd, WM_COPYDATA, (WPARAM) NULL, (LPARAM)pcp);
    delete pcp;
}
//---------------------------------------------------------------------------
bool TfMain::ChangeToSiteMap(char *str)                                         //wei 20151127 GPIB Change Site Map
{
    if(SystemStart==true)
    {
        return false;
    }

    AnsiString sSiteMap=AnsiString(str);
    int iSiteMap[32];                                                           //Jimmychiu 20230307 修正陣列大小16->32
    ZeroMemory(iSiteMap, 32);
    AnsiString sSiteMode="";

    int iDashPos=sSiteMap.Pos("-");
    int iSiteNum;//JerryYang 20160113

    sSiteMode =sSiteMap.SubString(1, iDashPos-1);
    sSiteMap  =sSiteMap.SubString(iDashPos, sSiteMap.Length());

    for(int i=0; i<16; i++)
    {
        AnsiString sBin="";
        int iBin=-1;

        if(i<9)
        {
            sBin=sSiteMap.SubString(2, 1);
            sSiteMap=sSiteMap.SubString(3, sSiteMap.Length());
        }
        else
        {
            sBin=sSiteMap.SubString(2, 2);
            sSiteMap=sSiteMap.SubString(4, sSiteMap.Length());
        }

        iBin=atoi(sBin.c_str());

        if(iBin>=1 && iBin<=16)
        {

            iSiteMap[i]=iBin;
        }
        else
        {
            return false;
        }

        if(sSiteMap.Pos("_")==1)
        {
            break;
        }
    }

    if(sSiteMode=="SINGLE1X1")                                                  //JerryYang 20160113 Sitemap防護
    {
        iSiteNum=1;
    }
    else if(sSiteMode=="DUAL1X2" || sSiteMode=="DUAL2X1")
    {
        iSiteNum=2;
    }
    else if(sSiteMode=="TRI1X3")
    {
        iSiteNum=3;
    }
    else if(sSiteMode=="QUAD1X4" || sSiteMode=="QUAD2X2" || sSiteMode=="QAD2X2N")
    {
        iSiteNum=4;
    }
    else if(sSiteMode=="6SITE2X3" || sSiteMode=="6SITE2X3N")
    {
        iSiteNum=6;
    }
    else if(sSiteMode=="8SITE2X4")
    {
        iSiteNum=8;
    }
    else if(sSiteMode=="10SITE2X5")
    {
        iSiteNum=10;
    }
    else if(sSiteMode=="12SITE2X6")
    {
        iSiteNum=12;
    }
    else if(sSiteMode=="16SITE2X8" || sSiteMode=="16SITE4X4")
    {
        iSiteNum=16;
    }
    else if(sSiteMode=="32SITE4X8")
    {
        iSiteNum=32;
    }
    else
    {
        return false;
    }

    for(int i=0; i<iSiteNum; i++)                                               //JerryYang 20160113 Sitemap防護
    {
        if(iSiteMap[i]>iSiteNum || iSiteMap[i]<1)
        {
            return false;
        }
        for(int j=i+1; j<iSiteNum; j++)
        {
            if(iSiteMap[i]==iSiteMap[j])
            {
                return false;
            }
        }
    }
    if(sSiteMode=="SINGLE1X1")//*   SingleSite
    {
        TestIF_File.iTestMode=SingleSite;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
    }
    else if(sSiteMode=="DUAL1X2")//*   DualSite
    {
        TestIF_File.iTestMode=DualSite;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];
    }
    else if(sSiteMode=="TRI1X3")//
    {
        TestIF_File.iTestMode=TriSite1X3;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];
        TestIF_File.iSiteMap[0][2]=iSiteMap[2];
    }
    else if(sSiteMode=="QUAD1X4")//*     QualSite1X4    _8Site1X4
    {
        TestIF_File.iTestMode=QualSite1X4;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];
        TestIF_File.iSiteMap[0][2]=iSiteMap[2];
        TestIF_File.iSiteMap[0][3]=iSiteMap[3];
    }
    else if(sSiteMode=="DUAL2X1")//*    DualSite2x1
    {
        TestIF_File.iTestMode=DualSite2x1;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
    }
    else if(sSiteMode=="QUAD2X2")//*    QualSite2X2
    {
        TestIF_File.iTestMode=QualSite2X2;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
    }
    else if(sSiteMode=="QAD2X2N")                                               //Steven 20230214 : 補上GPIB set site map
    {
        TestIF_File.iTestMode=QualSite2X2N;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
    }
    else if(sSiteMode=="6SITE2X3")//*      _6Site2X3
    {
        TestIF_File.iTestMode=_6Site2X3;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
    }
    else if(sSiteMode=="6Site2X3N")                                             //Steven 20230214 : 補上GPIB set site map
    {
        TestIF_File.iTestMode=_6Site2X3N;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
    }
    else if(sSiteMode=="8Site2X4N")                                             //Wei 20231211 : 2X4NN Mode
    {
        TestIF_File.iTestMode=_8Site2X4;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
    }
    else if(sSiteMode=="8SITE2X4")//*    _8Site2X4
    {
        TestIF_File.iTestMode=_8Site2X4;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
    }
    else if(sSiteMode=="10SITE2X5")//*    _10Site2X5      //wei 20190614 10 site
    {
        TestIF_File.iTestMode=_10Site2X5;
        TestIF_File.iSiteMap[0][0] = iSiteMap[0];
        TestIF_File.iSiteMap[1][0] = iSiteMap[1];
        TestIF_File.iSiteMap[0][1] = iSiteMap[2];
        TestIF_File.iSiteMap[1][1] = iSiteMap[3];
        TestIF_File.iSiteMap[0][2] = iSiteMap[4];
        TestIF_File.iSiteMap[1][2] = iSiteMap[5];
        TestIF_File.iSiteMap[0][3] = iSiteMap[6];
        TestIF_File.iSiteMap[1][3] = iSiteMap[7];
        TestIF_File.iSiteMap[0][4] = iSiteMap[8];
        TestIF_File.iSiteMap[1][4] = iSiteMap[9];
    }
    else if(sSiteMode=="12SITE2X6")//*    _12Site2X6
    {
        TestIF_File.iTestMode=_12Site2X6;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];
        TestIF_File.iSiteMap[0][5]=iSiteMap[10];
        TestIF_File.iSiteMap[1][5]=iSiteMap[11];
    }
    else if(sSiteMode=="16SITE2X8") //*     _16Site2X8
    {
        TestIF_File.iTestMode=_16Site2X8;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];
        TestIF_File.iSiteMap[0][5]=iSiteMap[10];
        TestIF_File.iSiteMap[1][5]=iSiteMap[11];
        TestIF_File.iSiteMap[0][6]=iSiteMap[12];
        TestIF_File.iSiteMap[1][6]=iSiteMap[13];
        TestIF_File.iSiteMap[0][7]=iSiteMap[14];
        TestIF_File.iSiteMap[1][7]=iSiteMap[15];
    }
    else if(sSiteMode=="16Site4X4")                                             //Steven 20230214 : 補上GPIB set site map
    {
        TestIF_File.iTestMode=_16Site4X4;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[2][0]=iSiteMap[2];
        TestIF_File.iSiteMap[3][0]=iSiteMap[3];
        TestIF_File.iSiteMap[0][1]=iSiteMap[4];
        TestIF_File.iSiteMap[1][1]=iSiteMap[5];
        TestIF_File.iSiteMap[2][1]=iSiteMap[6];
        TestIF_File.iSiteMap[3][1]=iSiteMap[7];
        TestIF_File.iSiteMap[0][2]=iSiteMap[8];
        TestIF_File.iSiteMap[1][2]=iSiteMap[9];
        TestIF_File.iSiteMap[2][2]=iSiteMap[10];
        TestIF_File.iSiteMap[3][2]=iSiteMap[11];
        TestIF_File.iSiteMap[0][3]=iSiteMap[12];
        TestIF_File.iSiteMap[1][3]=iSiteMap[13];
        TestIF_File.iSiteMap[2][3]=iSiteMap[14];
        TestIF_File.iSiteMap[3][3]=iSiteMap[15];
    }
    else if(sSiteMode=="32Site4X8N")                                            //Steven 20230214 : 補上GPIB set site map
    {
        TestIF_File.iTestMode=_32Site4X8N;
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[2][0]=iSiteMap[2];
        TestIF_File.iSiteMap[3][0]=iSiteMap[3];
        TestIF_File.iSiteMap[0][1]=iSiteMap[4];
        TestIF_File.iSiteMap[1][1]=iSiteMap[5];
        TestIF_File.iSiteMap[2][1]=iSiteMap[6];
        TestIF_File.iSiteMap[3][1]=iSiteMap[7];
        TestIF_File.iSiteMap[0][2]=iSiteMap[8];
        TestIF_File.iSiteMap[1][2]=iSiteMap[9];
        TestIF_File.iSiteMap[2][2]=iSiteMap[10];
        TestIF_File.iSiteMap[3][2]=iSiteMap[11];
        TestIF_File.iSiteMap[0][3]=iSiteMap[12];
        TestIF_File.iSiteMap[1][3]=iSiteMap[13];
        TestIF_File.iSiteMap[2][3]=iSiteMap[14];
        TestIF_File.iSiteMap[3][3]=iSiteMap[15];
        TestIF_File.iSiteMap[0][4]=iSiteMap[16];
        TestIF_File.iSiteMap[1][4]=iSiteMap[17];
        TestIF_File.iSiteMap[2][4]=iSiteMap[18];
        TestIF_File.iSiteMap[3][4]=iSiteMap[19];
        TestIF_File.iSiteMap[0][5]=iSiteMap[20];
        TestIF_File.iSiteMap[1][5]=iSiteMap[21];
        TestIF_File.iSiteMap[2][5]=iSiteMap[22];
        TestIF_File.iSiteMap[3][5]=iSiteMap[23];
        TestIF_File.iSiteMap[0][6]=iSiteMap[24];
        TestIF_File.iSiteMap[1][6]=iSiteMap[25];
        TestIF_File.iSiteMap[2][6]=iSiteMap[26];
        TestIF_File.iSiteMap[3][6]=iSiteMap[27];
        TestIF_File.iSiteMap[0][7]=iSiteMap[28];
        TestIF_File.iSiteMap[1][7]=iSiteMap[29];
        TestIF_File.iSiteMap[2][7]=iSiteMap[30];
        TestIF_File.iSiteMap[3][7]=iSiteMap[31];
    }
    else
    {
        return false;
    }

    bGPIBChangeSiteMap =true;
    fSetup->ScrollBar1Change(this);
    fSetup->DoIniDataToForm();
    fSetup->sbUpdateClick(this);
    fMain->ShowTestHeadComp(true);
    if(iBackupTestMode==TestIF_File.iTestMode)//JerryYang 20160519沒有切換Test mode就照原本的開關site
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<8; j++)
            {
               bTestSiteUse[0][i][j]=iBackupDutOnOff[i][j];
               LastSet.bUseTestSocket[0][i][j]=iBackupDutOnOff[i][j];
            }
        }
    }
    fMain->ShowTestHeadComp(false);
    //JerryYang 20160113 START:切換sitemap後要重新載入
    DoStructUnitConvert();
    fTestCategory->AdjFormData();
    fContactCT->ShowFormComp();
    SetWorkParameter();
    //JerryYang 20160113 END
    return true;
}
//20150901 Mylin GPIB Setup Site Map }
//---------------------------------------------------------------------------
bool TfMain::ChangeToAlarmSetup(char *str)          //wei 20151127 GPIB Change Site Map
{
    if(SystemStart==true)
    {
        return false;
    }

    AnsiString sAlarmData=AnsiString(str);
    AnsiString sTemp="";

    int iPosStart=0;
    int iPosEnd=0, iCate;
    double dValue=0;      //JerryYang 20160530 LowYieldLimit要能設定到小數點
    bool bFlag[eTrayCount]={false};
    bool bFlag1[eTrayCount]={false};
    AnsiString sBinName[TEST_MAX_BIN];
    AnsiString sPassOrFail[TEST_MAX_BIN];
    double fYield[TEST_MAX_BIN];

    //SiteDiff
    iPosStart=1;
    iPosEnd=sAlarmData.Pos("_");
    if(iPosEnd==0)
        iPosEnd=sAlarmData.Pos("\r\n");
    if(iPosEnd==0)
        return false;
    sTemp=sAlarmData.SubString(iPosStart, iPosEnd-iPosStart);
    dValue=atof(sTemp.c_str());
    sAlarmData=sAlarmData.SubString(iPosEnd+1, sAlarmData.Length());

    if(dValue>0)
    {
        Prod.bFailAlarmSiteYieldDifferent=true;
        TestIF_File.bFailAlarmSiteYieldDifferent=true;
        Prod.dFailAlarmSiteYield=dValue;        //JerryYang 20160530 LowYieldLimit要能設定到小數點
        TestIF_File.dFailAlarmSiteYield=dValue; //JerryYang 20160530 LowYieldLimit要能設定到小數點
    }
    else
    {
        Prod.bFailAlarmSiteYieldDifferent=false;
        TestIF_File.bFailAlarmSiteYieldDifferent=false;
//        TestIF_File.iLowYieldLimit=0;    //JerryYang 20151207 MARK,不清除Yield
        TestIF_File.bFailAlarmLowYield=false;//JerryYang 20151207
    }

    if(sAlarmData=="\n")
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            iCate=Prod.iT6CatData[i];
            if(iCate>=0)
                BinSelect[iTestRunMode].iStackDefFailCate[iCate]=1;     //QQQ
        }
        for(int i=0; i<eTrayCount; i++)
            BinSelect[iTestRunMode].iStackDefFailCate[i]=1;
    }

    //Bin Alarm Yield
    for(int i=0; i<15; i++)
    {
        sBinName[i]="";
        sPassOrFail[i]="";
        fYield[i]=0;
        Prod.bIsPassBin[i]=true;
        Prod.bFailure[i]=false;
    }
    sAlarmData=sAlarmData+"_";
    for(int i=0; i<iTestBinCount; i++)
    {
        iPosStart=1;
        iPosEnd=sAlarmData.Pos("_");
        sTemp=sAlarmData.SubString(iPosStart, iPosEnd-iPosStart);
        sAlarmData=sAlarmData.SubString(iPosEnd+1, sAlarmData.Length());

        //sTemp is 1|P|90
        //BinName
        iPosStart=1;
        iPosEnd=sTemp.Pos("|");
        sBinName[i]=sTemp.SubString(iPosStart, iPosEnd-iPosStart);
        int iBinNum=atoi(sBinName[i].c_str())-1;
        sTemp=sTemp.SubString(iPosEnd+1, sTemp.Length());

        //sTemp is P|90
        //PassOrFail
        iPosStart=1;
        iPosEnd=sTemp.Pos("|");
        sPassOrFail[iBinNum]=sTemp.SubString(iPosStart,iPosEnd-iPosStart);
        sTemp=sTemp.SubString(iPosEnd+1, sTemp.Length());

        //sTemp is 90
        fYield[iBinNum]=atof(sTemp.c_str());

        if(sAlarmData.Length()<=0)
        {
            break;
        }
    }

    for(int i=0; i<iTestBinCount; i++)
    {
        iCate=Prod.iT6CatData[i+1];
        if(iCate<0)
            continue;

        if(sPassOrFail[i]=="P")
        {
            Prod.bIsPassBin[i]=true;
            if(Prod.iT6CatData[i+1]==Prod.iIfErrorT6)//JerryYang 20151211 Tray被設為Error時,不能改成Pass
            {
                ShowMyMessage("Setup Error", "有Bin被設為Error,不能改為PASS");
                return false;
            }
//            MyBinPanel[tag]->iError=Y;
        }
        else if(sPassOrFail[i]=="F")
        {
            Prod.bIsPassBin[i]=false;
            Prod.bFailure[i]=true;
        }

        if(fYield[i]>0 && Prod.bIsPassBin[i]==true)
        {
            BinSelect[iTestRunMode].bFailure[i+1]=true;
        }
        else
        {
            BinSelect[iTestRunMode].bFailure[i+1]=false;
        }
        if(Prod.bIsPassBin[i]==true && Prod.bFailure[i]==true) //20150827
        {
            if(fYield[i]==0) //JerryYang 20151207 Bin設F 但yield設0,要把Enable改成False
            {
                Prod.dFailureLimit[i+1]=fYield[i];
                BinSelect[iTestRunMode].bFailure[i+1]=false;
            }
            else if(fYield[i]>0) //JerryYang 20151207 Bin設F 但yield設0,要把Enable改成False
            {
                Prod.dFailureLimit[i+1]=fYield[i];
                BinSelect[iTestRunMode].bFailure[i+1]=true;
            }
            else
            {
                BinSelect[iTestRunMode].bFailure[i+1]=false;
            }
        }
        else if(Prod.bIsPassBin[i]==false)
        {
            Prod.bFailAlarmLowYield=true;
            TestIF_File.bFailAlarmLowYield=true;
            Prod.dLowYieldLimit=fYield[i];        //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dLowYieldLimit=fYield[i]; //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        if(iCate!=0) //JerryYang 20160202 此Bin有對應的Tray盤
        {
            if(Prod.bIsPassBin[i]==true && Prod.bFailure[i]==true)
            {
                if(bFlag1[iCate]==true)
                {
                    ShowMyMessage("Setup Error", "設定錯誤,同一個Tray盤不能被設為PASS又FAIL");
                    return false;
                }
                BinSelect[iTestRunMode].iStackDefFailCate[iCate]=1;
                bFlag[iCate]=true;//JerryYang 20151211 用來判斷同一個Tray是否被設為PASS後又設Fail
            }
            else if(Prod.bIsPassBin[i]==false)
            {
                if(bFlag[iCate]==true)
                {
                    ShowMyMessage("Setup Error", "設定錯誤,同一個Tray盤不能同時設定PASS和FAIL");
                    return false;
                }
                BinSelect[iTestRunMode].iStackDefFailCate[iCate]=0;
                bFlag1[iCate]=true;;//JerryYang 20151211 用來判斷同一個Tray是否被設為Fail後又設PASS
            }
        }
    }
    if(TrayForm.iFixTrayMode==1 &&
       ((BinSelect[iTestRunMode].iStackDefFailCate[e3Fix1]!=BinSelect[iTestRunMode].iStackDefFailCate[e3Fix4]) || //JerryYang 20160202 選擇上下盤才判斷Fix盤的PASS FAIL是否設定相同
        (BinSelect[iTestRunMode].iStackDefFailCate[e3Fix2]!=BinSelect[iTestRunMode].iStackDefFailCate[e3Fix5]) ||
        (BinSelect[iTestRunMode].iStackDefFailCate[e3Fix3]!=BinSelect[iTestRunMode].iStackDefFailCate[e3Fix6])))
    {
        ShowMyMessage("Setup Error", "設定錯誤,Fix盤上下必須設定相同為PASS或FAIL");
        return false;
    }
    bGPIBChangeAlarm=true;
    fYieldMonitoring->btnApplyClick(this);
    fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
    fBinSel->spbSaveClick(this);
    fShowBinSelect->ShowBinSel();
    bGPIBChangeAlarm=false;
    return true;
}
//---------------------------------------------------------------------------
bool TfMain::ChangeToAlarmSetup_SG(char *str)          //wei 20151127 GPIB Change Site Map
{
    AnsiString asCmd="",str1="",str2="",asCondition="";
    TStringList *tSetup=new TStringList();
    TStringList *tCondition=new TStringList();
    double dSiteYieldCmp=0.0,dLowYield=0.0;
    int i=0,iSetBinNum=0,iBin=0,iCateg=0;
    bool bCondition=true,bYieldInitail=false;
    AnsiString sCategPassFail[eTrayCount];
    AnsiString sBinPassFail[TEST_MAX_BIN];
    int iBinNum[TEST_MAX_BIN];
    double fYield[TEST_MAX_BIN];
    bool bBinOverlapping[TEST_MAX_BIN];

    for(i=0;i<TEST_MAX_BIN;i++)
    {
        sBinPassFail[i]="";
        iBinNum[i]=0;
        fYield[i]=0.0;
        bBinOverlapping[i]=false;
    }

    for(i=0;i<eTrayCount;i++)
    {
        sCategPassFail[i]="";
    }

    asCmd=AnsiString(str).Trim();  //asCmd="5_1|P|95_4|F|0.1"
                                   //      "NULL_1|P|0_4|F|0_6|F|0";

                                   //NULL_1|P|0_2|P|0_3|P|0_4|F|0_5|F|0_6|F|0
                                   //NULL_1|P|90_2|P|90_3|P|90_4|F|0.3

    str1=StringReplace(asCmd,"_", ",", TReplaceFlags()<<rfReplaceAll);
    tSetup->CommaText=str1;
    iSetBinNum=tSetup->Count; //幾個 Bin 被設定

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)
    {
        //矽格湖口廠不要用 bYieldInitail
    }
    else
    {
        if(asCmd.Pos("NULL")!=0)
            bYieldInitail=true; //將 Yiled 設定清空
    }
    if(iSetBinNum>1)
    {
        for(i=0;i<iSetBinNum;i++)
        {
            if(tSetup->Strings[i]!="")
            {
                if(i==0)
                {
                    dSiteYieldCmp=atof(tSetup->Strings[i].c_str()); //取得 dSiteYieldCmp 設定值
                }
                else
                {
                    asCondition=tSetup->Strings[i].c_str();//1|P|95
                    str2=StringReplace(asCondition,"|", ",", TReplaceFlags()<<rfReplaceAll);
                    tCondition->CommaText=str2;
                    if(tCondition->Count==3)   //1|P|95
                    {
                        iBin=atoi(tCondition->Strings[0].c_str());          //1         4
                        sBinPassFail[iBin]=tCondition->Strings[1];          //P         F
                        fYield[iBin]=atof(tCondition->Strings[2].c_str());  //95        0.1

                        //檢查所有 Pass Bin LowYiled 數值有沒一樣。
                        if(sBinPassFail[iBin]=="P")
                        {
                            if(dLowYield==0.0)
                            {
                                dLowYield=fYield[iBin];  //取得 LowYield 值
                            }
                            else
                            {
                                if(dLowYield!=fYield[iBin])
                                {
                                    iRecordSigurdGPIBFlag=3;
                                    bCondition=false;
                                }
                            }
                        }

                        //檢查 Bin 是否有重複設定
                        if(bBinOverlapping[iBin]==false)
                        {
                            bBinOverlapping[iBin]=true;
                        }
                        else
                        {
                            iRecordSigurdGPIBFlag=4;
                            bCondition=false;
                        }

                        //檢查 Bin Pass/Fail 設定 Tray 有沒重疊
                        iCateg=Prod.iT6CatData[iBin];
                        if(iCateg>=0)
                        {
                            if(sCategPassFail[iCateg]=="")
                            {
                                sCategPassFail[iCateg]=sBinPassFail[iBin];
                            }
                            else
                            {
                                if(sCategPassFail[iCateg]!=sBinPassFail[iBin])
                                {
                                    iRecordSigurdGPIBFlag=5;
                                    bCondition=false;
                                }
                            }
                        }

                        //檢查 Error Tray 有沒被設定到 Pass Bin
                        if(sBinPassFail[iBin]=="P")
                        {
                            if(iCateg==Prod.iIfErrorT6)
                            {
                                iRecordSigurdGPIBFlag=6;
                                bCondition=false;
                            }
                        }
                    }
                    else
                    {
                        iRecordSigurdGPIBFlag=2;
                        bCondition=false;
                    }
                }
            }
        }
    }
    else
    {
        if(iSetBinNum==0 && bYieldInitail==false)
        {
            iRecordSigurdGPIBFlag=1;
            return false;
        }
    }

    if(bCondition==false)
    {
        return false;
    }

    //設定 Low Yields%(By Total) /  By Site Compare Yield% (1min)
    if(iTestRunMode==FT)
    {
        if(bYieldInitail || dSiteYieldCmp<=0.0)
        {
            TestIF_File.bFailAlarmSiteYieldCmp=false;
            TestIF_File.dFailAlarmSiteYieldCmp=0.0;
        }
        else
        {
            TestIF_File.bFailAlarmSiteYieldCmp=true;
            TestIF_File.dFailAlarmSiteYieldCmp=dSiteYieldCmp;
        }

        if(bYieldInitail || dLowYield<=0.0 )
        {
            TestIF_File.bFailAlarmLowYieldByTotal=false;
            TestIF_File.dLowYieldLimitByTotal=0.0;
        }
        else
        {
            TestIF_File.bFailAlarmLowYieldByTotal=true;
            TestIF_File.dLowYieldLimitByTotal=dLowYield;
        }
    }
    else
    {
        if(bYieldInitail || dSiteYieldCmp<=0.0)
        {
            TestIF_File.bFailAlarmSiteYieldCmp_RT=false;
            TestIF_File.dFailAlarmSiteYieldCmp_RT=0.0;
        }
        else
        {
            TestIF_File.bFailAlarmSiteYieldCmp_RT=true;
            TestIF_File.dFailAlarmSiteYieldCmp_RT=dSiteYieldCmp;
        }

        if(bYieldInitail || dLowYield<=0.0 )
        {
            TestIF_File.bFailAlarmLowYieldByTotal_RT=false;
            TestIF_File.dLowYieldLimitByTotal_RT=dLowYield=0.0;
        }
        else
        {
            TestIF_File.bFailAlarmLowYieldByTotal_RT=true;
            TestIF_File.dLowYieldLimitByTotal_RT=dLowYield;
        }
    }

    //設定 Failure / dFailureLimit / bConsFail
    for(i=0; i<TEST_MAX_BIN; i++)
    {
        iCateg=Prod.iT6CatData[i];
        if(iCateg<0)
            continue;

        if(sBinPassFail[i]=="P")
        {
            BinSelect[iTestRunMode].iStackDefFailCate[iTo3Unload[iCateg]]=0;    //Sam 20250303 : 修正 RMS SetAlarmSetup Category Pass/Fail 異常問題。
            BinSelect[iTestRunMode].bConsFail[i]=false;
            BinSelect[iTestRunMode].bFailure[i]=false;
            BinSelect[iTestRunMode].dFailureLimit[i]=0;
        }
        else if(sBinPassFail[i]=="F")
        {
            BinSelect[iTestRunMode].iStackDefFailCate[iTo3Unload[iCateg]]=1;    //Sam 20250303 : 修正 RMS SetAlarmSetup Category Pass/Fail 異常問題。
            BinSelect[iTestRunMode].bConsFail[i]=true;
             BinSelect[iTestRunMode].dFailureLimit[i]=fYield[i];
            if(fYield[i]<=0.0)
                BinSelect[iTestRunMode].bFailure[i]=false;
            else
                BinSelect[iTestRunMode].bFailure[i]=true;
        }
        else
        {
            if(iCateg!=0 && BinSelect[iTestRunMode].iStackDefFailCate[iTo3Unload[iCateg]]==1)   //Sam 20250303 : 修正 RMS SetAlarmSetup Category Pass/Fail 異常問題。
            {
                if(bYieldInitail)
                {
                    BinSelect[iTestRunMode].bConsFail[i]=false;     //將其他 Fail Bin bConsFail 預設為關閉
                    BinSelect[iTestRunMode].bFailure[i]=false;
                    BinSelect[iTestRunMode].dFailureLimit[i]=0.0;
                }
                else
                {
                    BinSelect[iTestRunMode].bConsFail[i]=true;      //將其他 Fail Bin bConsFail 預設為開啟
                }
            }
        }
    }

    fYieldMonitoring->btnApplyClick(this);
    /*
    if(iTestRunMode==FT)
        fBinSel->PageControl1->ActivePage=fBinSel->tsNormal;
    else
        fBinSel->PageControl1->ActivePage=fBinSel->tsRetest;
    */
    /*   //Sam 20230711 : 修正 OLP SetCategroy 異常 Mark
    if(iTestRunMode==FT)
        fBinSel->PageControl1->ActivePageIndex=0;
    else
        fBinSel->PageControl1->ActivePageIndex=1;
    */
    bGPIBChangeAlarm=true;
    fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
    fBinSel->spbSaveClick(this);
    fShowBinSelect->ShowBinSel();
    bGPIBChangeAlarm=false;

    return true;
}
//JerryYang 20181126 (Steven) : support Epson DLL function
//==>
//---------------------------------------------------------------------------
bool TfMain::CreateAndOpenMap()
{
    HANDLE hFileMap=OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "HandlerMemory");            //Open Map
    if(hFileMap!=NULL)
    {
        CmdData=(INFO *)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(INFO));    //映射到CmdData結構
        memset(CmdData, 0x00, sizeof(CmdData));
        for(int i=0; i<eCommandTotal; i++)
        {
            CmdData->bCommandList[i]=false;
        }
        sprintf(CmdData->cVersion,"%s" , ATPDLLVersion);    //JerryYang 20230721 : dll版本卡控
        return true;
    }
    else
    {
        hFileMapping=CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CmdData), "HandlerMemory");

        if(hFileMapping!=NULL)
        {
            if (GetLastError()==ERROR_ALREADY_EXISTS)
            {
                ShowMyMessage("Mapping file already created!");
                CloseHandle(hFileMapping);
            }

            hFileMap=OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "HandlerMemory");            //Open Map
            if (hFileMap!=NULL)
            {
                CmdData=(INFO *)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(INFO));    //映射到CmdData結構
                memset(CmdData, 0x00, sizeof(CmdData));
                for(int i=0; i<eCommandTotal; i++)
                {
                    CmdData->bCommandList[i]=false;
                }
                sprintf(CmdData->cVersion,"%s" , ATPDLLVersion);    //JerryYang 20230721 : dll版本卡控
                return true;
            }
            else
            {
                memset(CmdData, 0x00, sizeof(CmdData));
                ShowMyMessage("Open File Mapping Fail!");
            }
        }
    }

    return false;
}
//---------------------------------------------------------------------------
int TfMain::SetTrayBinByDLL(int iTrayNum, LPSTR asCategories, int iFail)
{
    char cBinTemp[3];
    int i, j, iTrayBin[256],iBin;
    char cTemp[256];
    memset(iTrayBin,'\0', sizeof(iTrayBin));
    memset(cTemp,'\0', sizeof(cTemp));

    strncpy(cTemp, asCategories, sizeof(cTemp));
    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }
    if(SystemStart==true)   //保護
    {
        return -4;  //Operation not Allowed
    }
    if(SettingsIsWindowOpened()==true)
    {
        return -6;  //Settings Window is Opened
    }
    if(HasICUnderMachine() || HasAnyICInMachine() || LastSet.iTester==OFF_LINE)
    {
        return -4;  //Operation not Allowed
    }

    if(iTrayNum<0 || iTrayNum>6)
        return -3;   //Parameter Error

    if(iFail>0 || iFail<-1)
        return -3;   //Parameter Error

    if(IniConfig.bFTBin2RTBin==false)
        return -4;

    for(i=0; i<iTestBinCount; i++)
    {
        if(cTemp[0]!='\0')
        {
            SplitStrByDotSpaceOnly(cTemp, cBinTemp, 4);

            iBin=StrToIntDef(cBinTemp, -1);
            if(iBin>=iTestBinCount || iBin<0)
            {
                fBinSel->ReadFile(false, false, "");
                return -3;   //Parameter Error
            }

            if(cBinTemp[0]=='0' && iFail==0)
            {
                fBinSel->ReadFile(false, false, "");
                return -3;
            }

            if(cBinTemp[0]=='0' && iFail==-1)  //比照EPSON的指令, Bin0當作是Error bin
            {
                BinSelect[iTestRunMode].IfErrorT3=iTrayNum-1;
            }
            else
            {
                if(iFail==0 && BinSelect[iTestRunMode].IfErrorT3==iTrayNum-1)
                {
                    BinSelect[iTestRunMode].IfErrorT3=5;
                }
                iTrayBin[i]=atoi(cBinTemp);
            }

            strcpy(cBinTemp,"");
        }
    }

    for(i=0; i<eTrayCount; i++)
    {

        if(i==iTrayNum-1)
        {
            if(iFail==0)    //Pass Tray
            {
                BinSelect[iTestRunMode].iStackDefFailCate[i]=0;
                bSetByDLL=true;
            }
            else if(iFail==-1)   //Fail Tray
            {
                BinSelect[iTestRunMode].iStackDefFailCate[i]=1;
                bSetByDLL=true;
            }
            else
            {
                bSetByDLL=false;
                fBinSel->ReadFile(false, false, "");
                return -3;      //Parameter Error
            }
        }

    }

    for(i=0; i<fBinSel->sBinTraySetT3Pos[iTestRunMode]->Count; i++)
    {
        if(fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]==iTrayNum)
        {
            if(iTestRunMode==RT)
            {
                if((IniConfig.bA02BinModelPrime && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||
                (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
                {
                    fBinSel->sBinTraySetT3Pos[FT]->Strings[i]=AnsiString(0);
                }
                else
                {
                    fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(0);
                }
            }
            else
            {
                fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(0);
            }
        }
    }

    for(i=0; i<iTestBinCount; i++)
    {
        for(j=0; j<iTestBinCount; j++)
        {
            if(i<fBinSel->sBinTraySetT3Pos[iTestRunMode]->Count)
            {
                if(i==iTrayBin[j] && iTrayBin[j]!=0)
                {
                    if(iTestRunMode==RT)
                    {
                        if((IniConfig.bA02BinModelPrime && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||
                        (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
                        {
                            fBinSel->sBinTraySetT3Pos[FT]->Strings[i]=AnsiString(iTrayNum);
                        }
                        else
                        {
                            fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(iTrayNum);
                        }
                    }
                    else
                    {
                        fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=AnsiString(iTrayNum);
                    }
                }
            }
        }
    }

    fBinSel->spbSaveClick(this);
    DoStructUnitConvert();
    fShowBinSelect->ShowBinSel();
    SetWorkParameter();                                                         //Steven 20120130 : 存檔後要重新load參數
    bSetByDLL=false;
    return 0;
}
//------------------------------------------------------------------------------
int TfMain::GetTrayBinByDLL(int iTrayNum)
{
    AnsiString asBinSetting="";
    asBinSetting=AnsiString(iTrayNum)+":";
    char *cTemp="A";
    int i;

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    if(iTrayNum<1 || iTrayNum>6)    //tray is not defined
        return -1;

    for(i=1; i<iTestBinCount; i++)  //從Bin1開始, 比照Epson所定義的格式
    {
        if(Prod.iT6PosCate[i]==iTrayNum)
        {
            cTemp=asBinSetting.AnsiLastChar();
            if(*cTemp==':')
            {
                asBinSetting=asBinSetting+AnsiString(i);
            }
            else
            {
                asBinSetting=asBinSetting+","+AnsiString(i);
            }

        }
    }
    if(Prod.iIfErrorT6==iTrayNum) //比照EPSON的指令, Bin0當作是Error bin
    {
        asBinSetting=asBinSetting+",0";
    }
    asBinSetting+=";";

    if(BinSelect[iTestRunMode].iStackDefFailCate[iTrayNum-1]==0)                //QQQ
    {
        asBinSetting+="0";
    }
    else
    {
        asBinSetting+="-1";
    }
    strcpy(CmdData->cGetBinCategories_Cmd3, asBinSetting.c_str());
    return 0;
}
//---------------------------------------------------------------------------
int TfMain::SetSiteMapByDLL(LPSTR cSiteMap, int iNoOfSites)
{
    int i, j;
    int iMode=TestIF_File.iTestMode;
    int iSiteCnt=SiteData[iMode].Cnt;
//    char *szSiteMap="[01,01,01][02,03,00][03,02,01][04,--,00]";
    AnsiString strTemp[32]; //最大32site
    bool bTempDutOnOff[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    bool bSiteOnOff[32];
    char *cSiteMapSplit[32];
    int iCount=0;
    char cSiteTemp[2];
    int iSiteMap[32];
    int iTemp=0;

    char cTemp[256];
    memset(cTemp, '\0', sizeof(cTemp));
    strncpy(cTemp, cSiteMap, sizeof(cTemp));

    if(SystemStart==true)   //保護
    {
        return -4;  //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    if(SettingsIsWindowOpened()==true)
    {
        return -6;  //Settings Window is Opened
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
    {
        return -4;  //Operation not Allowed
    }

    if(iNoOfSites!=iSiteCnt)
    {
        return -3;   //Parameter Error
    }

    char *temp=strtok(cTemp, " []");
    while(temp!=NULL)
    {
        cSiteMapSplit[iCount]=temp;

        temp = strtok(NULL, " []");

        if(iCount>=iNoOfSites)
        {
            //資料異常
            return -3;   //Parameter Error
        }
        iCount++;
    }

    for(i=0; i<iNoOfSites; i++)
    {
        for(j=0; j<3; j++)
        {
            SplitStrByDotSpaceOnly(cSiteMapSplit[i], cSiteTemp, 3);
            if(j==0)        //表示site的位置
            {
                iTemp=atoi(cSiteTemp);
                if(iTemp!=i+1)
                {
                    return -3;
                }
                if(iTemp>iSiteCnt || iTemp<0)
                {
                    return -3;   //Parameter Error
                }
            }
            else if(j==1)   //表示Site map的設定
            {
                if(cSiteTemp[0]=='-' && cSiteTemp[1]=='-')
                {
                    iSiteMap[i]=0;
                }
                else
                {
                    iSiteMap[i]=atoi(cSiteTemp);
                }
            }
            else if(j==2)   //表示開關site
            {
                iTemp=StrToIntDef(cSiteTemp, -1);
                if(iTemp<0 || iTemp>1)
                    return -3;   //Parameter Error

                bSiteOnOff[i]=atoi(cSiteTemp);
                if(iSiteMap[i]==0 && bSiteOnOff[i]==true)
                {
                    return -3;   //Parameter Error
                }
            }
        }
    }

    for(int i=0; i<iSiteCnt; i++)   //JerryYang 20160113 Sitemap防護
    {
        if(iSiteMap[i]>iSiteCnt || iSiteMap[i]<0)
        {
            return -3;  //Parameter Error
        }
        for(int j=i+1; j<iSiteCnt; j++)
        {
            if(iSiteMap[i]==iSiteMap[j] && iSiteMap[i]!=0)
            {
                return -3;  //Parameter Error
            }
        }
    }

    if(TestIF.iTestMode==SingleSite)//*   SingleSite
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
    }
    else if(TestIF.iTestMode==DualSite)//*   DualSite
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[0][1]=bSiteOnOff[1];
    }
    else if(TestIF_File.iTestMode==TriSite1X3)//
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];
        TestIF_File.iSiteMap[0][2]=iSiteMap[2];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[0][1]=bSiteOnOff[1];
        bTempDutOnOff[0][2]=bSiteOnOff[2];
    }
    else if(TestIF.iTestMode==QualSite1X4)//*     QualSite1X4    _8Site1X4
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[0][1]=iSiteMap[1];
        TestIF_File.iSiteMap[0][2]=iSiteMap[2];
        TestIF_File.iSiteMap[0][3]=iSiteMap[3];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[0][1]=bSiteOnOff[1];
        bTempDutOnOff[0][2]=bSiteOnOff[2];
        bTempDutOnOff[0][3]=bSiteOnOff[3];
    }
    else if(TestIF_File.iTestMode==DualSite2x1) //*    DualSite2x1
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
    }
    else if(TestIF.iTestMode==QualSite2X2 || //*    QualSite2X2
            TestIF.iTestMode==QualSite2X2N)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
    }
    else if(TestIF_File.iTestMode==_6Site2X3 || //*      _6Site2X3
            TestIF_File.iTestMode==_6Site2X3N)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
    }
    else if(TestIF_File.iTestMode==_8Site2X4 ||//*    _8Site2X4
            TestIF_File.iTestMode==_8Site2X4N) //Wei 20231211 : 2X4NN Mode
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
        bTempDutOnOff[0][4]=bSiteOnOff[8];
        bTempDutOnOff[1][4]=bSiteOnOff[9];
    }
    else if(TestIF_File.iTestMode==_12Site2X6)//*    _12Site2X6
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];
        TestIF_File.iSiteMap[0][5]=iSiteMap[10];
        TestIF_File.iSiteMap[1][5]=iSiteMap[11];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
        bTempDutOnOff[0][4]=bSiteOnOff[8];
        bTempDutOnOff[1][4]=bSiteOnOff[9];
        bTempDutOnOff[0][5]=bSiteOnOff[10];
        bTempDutOnOff[1][5]=bSiteOnOff[11];
    }
    else if(TestIF_File.iTestMode==_16Site2X8) //*     _16Site2X8
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[0][1]=iSiteMap[2];
        TestIF_File.iSiteMap[1][1]=iSiteMap[3];
        TestIF_File.iSiteMap[0][2]=iSiteMap[4];
        TestIF_File.iSiteMap[1][2]=iSiteMap[5];
        TestIF_File.iSiteMap[0][3]=iSiteMap[6];
        TestIF_File.iSiteMap[1][3]=iSiteMap[7];
        TestIF_File.iSiteMap[0][4]=iSiteMap[8];
        TestIF_File.iSiteMap[1][4]=iSiteMap[9];
        TestIF_File.iSiteMap[0][5]=iSiteMap[10];
        TestIF_File.iSiteMap[1][5]=iSiteMap[11];
        TestIF_File.iSiteMap[0][6]=iSiteMap[12];
        TestIF_File.iSiteMap[1][6]=iSiteMap[13];
        TestIF_File.iSiteMap[0][7]=iSiteMap[14];
        TestIF_File.iSiteMap[1][7]=iSiteMap[15];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[0][1]=bSiteOnOff[2];
        bTempDutOnOff[1][1]=bSiteOnOff[3];
        bTempDutOnOff[0][2]=bSiteOnOff[4];
        bTempDutOnOff[1][2]=bSiteOnOff[5];
        bTempDutOnOff[0][3]=bSiteOnOff[6];
        bTempDutOnOff[1][3]=bSiteOnOff[7];
        bTempDutOnOff[0][4]=bSiteOnOff[8];
        bTempDutOnOff[1][4]=bSiteOnOff[9];
        bTempDutOnOff[0][5]=bSiteOnOff[10];
        bTempDutOnOff[1][5]=bSiteOnOff[11];
        bTempDutOnOff[1][6]=bSiteOnOff[12];
        bTempDutOnOff[0][6]=bSiteOnOff[13];
        bTempDutOnOff[1][7]=bSiteOnOff[14];
        bTempDutOnOff[0][7]=bSiteOnOff[15];
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[2][0]=iSiteMap[2];
        TestIF_File.iSiteMap[3][0]=iSiteMap[3];
        TestIF_File.iSiteMap[0][1]=iSiteMap[4];
        TestIF_File.iSiteMap[1][1]=iSiteMap[5];
        TestIF_File.iSiteMap[2][1]=iSiteMap[6];
        TestIF_File.iSiteMap[3][1]=iSiteMap[7];
        TestIF_File.iSiteMap[0][2]=iSiteMap[8];
        TestIF_File.iSiteMap[1][2]=iSiteMap[9];
        TestIF_File.iSiteMap[2][2]=iSiteMap[10];
        TestIF_File.iSiteMap[3][2]=iSiteMap[11];
        TestIF_File.iSiteMap[0][3]=iSiteMap[12];
        TestIF_File.iSiteMap[1][3]=iSiteMap[13];
        TestIF_File.iSiteMap[2][3]=iSiteMap[14];
        TestIF_File.iSiteMap[3][3]=iSiteMap[15];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[2][0]=bSiteOnOff[2];
        bTempDutOnOff[3][0]=bSiteOnOff[3];
        bTempDutOnOff[0][1]=bSiteOnOff[4];
        bTempDutOnOff[1][1]=bSiteOnOff[5];
        bTempDutOnOff[2][1]=bSiteOnOff[6];
        bTempDutOnOff[3][1]=bSiteOnOff[7];
        bTempDutOnOff[0][2]=bSiteOnOff[8];
        bTempDutOnOff[1][2]=bSiteOnOff[9];
        bTempDutOnOff[2][2]=bSiteOnOff[10];
        bTempDutOnOff[3][2]=bSiteOnOff[11];
        bTempDutOnOff[0][3]=bSiteOnOff[12];
        bTempDutOnOff[1][3]=bSiteOnOff[13];
        bTempDutOnOff[2][3]=bSiteOnOff[14];
        bTempDutOnOff[3][3]=bSiteOnOff[15];
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        TestIF_File.iSiteMap[0][0]=iSiteMap[0];
        TestIF_File.iSiteMap[1][0]=iSiteMap[1];
        TestIF_File.iSiteMap[2][0]=iSiteMap[2];
        TestIF_File.iSiteMap[3][0]=iSiteMap[3];
        TestIF_File.iSiteMap[0][1]=iSiteMap[4];
        TestIF_File.iSiteMap[1][1]=iSiteMap[5];
        TestIF_File.iSiteMap[2][1]=iSiteMap[6];
        TestIF_File.iSiteMap[3][1]=iSiteMap[7];
        TestIF_File.iSiteMap[0][2]=iSiteMap[8];
        TestIF_File.iSiteMap[1][2]=iSiteMap[9];
        TestIF_File.iSiteMap[2][2]=iSiteMap[10];
        TestIF_File.iSiteMap[3][2]=iSiteMap[11];
        TestIF_File.iSiteMap[0][3]=iSiteMap[12];
        TestIF_File.iSiteMap[1][3]=iSiteMap[13];
        TestIF_File.iSiteMap[2][3]=iSiteMap[14];
        TestIF_File.iSiteMap[3][3]=iSiteMap[15];
        TestIF_File.iSiteMap[0][4]=iSiteMap[16];
        TestIF_File.iSiteMap[1][4]=iSiteMap[17];
        TestIF_File.iSiteMap[2][4]=iSiteMap[18];
        TestIF_File.iSiteMap[3][4]=iSiteMap[19];
        TestIF_File.iSiteMap[0][5]=iSiteMap[20];
        TestIF_File.iSiteMap[1][5]=iSiteMap[21];
        TestIF_File.iSiteMap[2][5]=iSiteMap[22];
        TestIF_File.iSiteMap[3][5]=iSiteMap[23];
        TestIF_File.iSiteMap[0][6]=iSiteMap[24];
        TestIF_File.iSiteMap[1][6]=iSiteMap[25];
        TestIF_File.iSiteMap[2][6]=iSiteMap[26];
        TestIF_File.iSiteMap[3][6]=iSiteMap[27];
        TestIF_File.iSiteMap[0][7]=iSiteMap[28];
        TestIF_File.iSiteMap[1][7]=iSiteMap[29];
        TestIF_File.iSiteMap[2][7]=iSiteMap[30];
        TestIF_File.iSiteMap[3][7]=iSiteMap[31];

        bTempDutOnOff[0][0]=bSiteOnOff[0];
        bTempDutOnOff[1][0]=bSiteOnOff[1];
        bTempDutOnOff[2][0]=bSiteOnOff[2];
        bTempDutOnOff[3][0]=bSiteOnOff[3];
        bTempDutOnOff[0][1]=bSiteOnOff[4];
        bTempDutOnOff[1][1]=bSiteOnOff[5];
        bTempDutOnOff[2][1]=bSiteOnOff[6];
        bTempDutOnOff[3][1]=bSiteOnOff[7];
        bTempDutOnOff[0][2]=bSiteOnOff[8];
        bTempDutOnOff[1][2]=bSiteOnOff[9];
        bTempDutOnOff[2][2]=bSiteOnOff[10];
        bTempDutOnOff[3][2]=bSiteOnOff[11];
        bTempDutOnOff[0][3]=bSiteOnOff[12];
        bTempDutOnOff[1][3]=bSiteOnOff[13];
        bTempDutOnOff[2][3]=bSiteOnOff[14];
        bTempDutOnOff[3][3]=bSiteOnOff[15];
        bTempDutOnOff[0][4]=bSiteOnOff[16];
        bTempDutOnOff[1][4]=bSiteOnOff[17];
        bTempDutOnOff[2][4]=bSiteOnOff[18];
        bTempDutOnOff[3][4]=bSiteOnOff[19];
        bTempDutOnOff[0][5]=bSiteOnOff[20];
        bTempDutOnOff[1][5]=bSiteOnOff[21];
        bTempDutOnOff[2][5]=bSiteOnOff[22];
        bTempDutOnOff[3][5]=bSiteOnOff[23];
        bTempDutOnOff[0][6]=bSiteOnOff[24];
        bTempDutOnOff[1][6]=bSiteOnOff[25];
        bTempDutOnOff[2][6]=bSiteOnOff[26];
        bTempDutOnOff[3][6]=bSiteOnOff[27];
        bTempDutOnOff[0][7]=bSiteOnOff[28];
        bTempDutOnOff[1][7]=bSiteOnOff[29];
        bTempDutOnOff[2][7]=bSiteOnOff[30];
        bTempDutOnOff[3][7]=bSiteOnOff[31];
    }
    else
    {
        return -3;   //Parameter Error
    }

    fSetup->ScrollBar1Change(this);
    fSetup->DoIniDataToForm();
    fSetup->sbUpdateClick(this);
    fMain->ShowTestHeadComp(true);

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
           bTestSiteUse[0][i][j]=bTempDutOnOff[i][j];
           LastSet.bUseTestSocket[0][i][j]=bTempDutOnOff[i][j];
           bTestSiteUse[1][i][j]=bTempDutOnOff[i][j];
           LastSet.bUseTestSocket[1][i][j]=bTempDutOnOff[i][j];
        }
    }

    fMain->ShowTestHeadComp(false);
    DoStructUnitConvert();
    fTestCategory->AdjFormData();
    fContactCT->ShowFormComp();
    SetWorkParameter();
    return 0;   //Success
}
//---------------------------------------------------------------------------
int TfMain::GetSiteMappingByDLL(LPSTR cSiteMap)
{
    int i, j;
    char cSiteInfo[32][10];
    AnsiString asSiteTemp[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    int iMode=TestIF_File.iTestMode;
    int iSiteCnt=SiteData[iMode].Cnt;

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    for(i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(j=0; j<MAX_SOCKET_COL; j++)
        {
            if(TestIF_File.iSiteMap[i][j]<=0)
            {
                asSiteTemp[i][j].sprintf("--");
            }
            else
            {
                asSiteTemp[i][j].sprintf("%02d", TestIF_File.iSiteMap[i][j]);
            }
        }
    }

    if(TestIF_File.iTestMode==SingleSite)       // Single Site 1x1
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
    }
    else if(TestIF_File.iTestMode==DualSite)       // Dual Site 1x2
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
    }
    else if(TestIF_File.iTestMode==TriSite1X3)
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[0][0][2]);
    }
    else if(TestIF_File.iTestMode==QualSite1X4)  // Qual Site 1x4
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[0][0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1)  // Dual Site 2x1
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2)  // Qual Site 2x2
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][1][1]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][0][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3)      //ChungHung 20140115 add for 2x3_6
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[0][1][2]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[0][0][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)  //Wei 20231211 : 2X4NN Mode
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[6], "[07,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[1][0][3]);
        sprintf(cSiteInfo[7], "[08,%s,%02d]", asSiteTemp[1][3], bTestSiteUse[0][0][3]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4)      // 8 Site 2x4
    {
        sprintf(cSiteInfo[0], "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1], "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2], "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3], "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4], "[05,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5], "[06,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6], "[07,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7], "[08,%s,%02d]", asSiteTemp[1][3], bTestSiteUse[0][1][3]);
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        sprintf(cSiteInfo[0],  "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1],  "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2],  "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3],  "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4],  "[05,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5],  "[06,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6],  "[07,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7],  "[08,%s,%02d]", asSiteTemp[1][3], bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[8],  "[09,%s,%02d]", asSiteTemp[0][4], bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[9],  "[10,%s,%02d]", asSiteTemp[1][4], bTestSiteUse[0][1][4]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        sprintf(cSiteInfo[0],  "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1],  "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2],  "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3],  "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4],  "[05,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5],  "[06,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6],  "[07,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7],  "[08,%s,%02d]", asSiteTemp[1][3], bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[8],  "[09,%s,%02d]", asSiteTemp[0][4], bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[9],  "[10,%s,%02d]", asSiteTemp[1][4], bTestSiteUse[0][1][4]);
        sprintf(cSiteInfo[10], "[11,%s,%02d]", asSiteTemp[0][5], bTestSiteUse[0][0][5]);
        sprintf(cSiteInfo[11], "[12,%s,%02d]", asSiteTemp[1][5], bTestSiteUse[0][1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site2X8)     //16Site 2x8
    {
        sprintf(cSiteInfo[0],   "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[1],   "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[2],   "[03,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[3],   "[04,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[4],   "[05,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[5],   "[06,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[6],   "[07,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[7],   "[08,%s,%02d]", asSiteTemp[1][3], bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[8],   "[09,%s,%02d]", asSiteTemp[0][4], bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[9],   "[10,%s,%02d]", asSiteTemp[1][4], bTestSiteUse[0][1][4]);
        sprintf(cSiteInfo[10],  "[11,%s,%02d]", asSiteTemp[0][5], bTestSiteUse[0][0][5]);
        sprintf(cSiteInfo[11],  "[12,%s,%02d]", asSiteTemp[1][5], bTestSiteUse[0][1][5]);
        sprintf(cSiteInfo[12],  "[13,%s,%02d]", asSiteTemp[0][6], bTestSiteUse[0][0][6]);
        sprintf(cSiteInfo[13],  "[14,%s,%02d]", asSiteTemp[1][6], bTestSiteUse[0][1][6]);
        sprintf(cSiteInfo[14],  "[15,%s,%02d]", asSiteTemp[0][7], bTestSiteUse[0][0][7]);
        sprintf(cSiteInfo[15],  "[16,%s,%02d]", asSiteTemp[1][7], bTestSiteUse[0][1][7]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        sprintf(cSiteInfo[0],   "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1],   "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[1][1][0]);
        sprintf(cSiteInfo[2],   "[03,%s,%02d]", asSiteTemp[2][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[3],   "[04,%s,%02d]", asSiteTemp[3][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[4],   "[05,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[5],   "[06,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[1][1][1]);
        sprintf(cSiteInfo[6],   "[07,%s,%02d]", asSiteTemp[2][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[7],   "[08,%s,%02d]", asSiteTemp[3][1], bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[8],   "[09,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[9],   "[10,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[1][1][2]);
        sprintf(cSiteInfo[10],  "[11,%s,%02d]", asSiteTemp[2][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[11],  "[12,%s,%02d]", asSiteTemp[3][2], bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[12],  "[13,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[1][0][3]);
        sprintf(cSiteInfo[13],  "[14,%s,%02d]", asSiteTemp[1][3], bTestSiteUse[1][1][3]);
        sprintf(cSiteInfo[14],  "[15,%s,%02d]", asSiteTemp[2][3], bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[15],  "[16,%s,%02d]", asSiteTemp[3][3], bTestSiteUse[0][1][3]);
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)
    {
        sprintf(cSiteInfo[0],   "[01,%s,%02d]", asSiteTemp[0][0], bTestSiteUse[1][0][0]);
        sprintf(cSiteInfo[1],   "[02,%s,%02d]", asSiteTemp[1][0], bTestSiteUse[1][1][0]);
        sprintf(cSiteInfo[2],   "[03,%s,%02d]", asSiteTemp[2][0], bTestSiteUse[0][0][0]);
        sprintf(cSiteInfo[3],   "[04,%s,%02d]", asSiteTemp[3][0], bTestSiteUse[0][1][0]);
        sprintf(cSiteInfo[4],   "[05,%s,%02d]", asSiteTemp[0][1], bTestSiteUse[1][0][1]);
        sprintf(cSiteInfo[5],   "[06,%s,%02d]", asSiteTemp[1][1], bTestSiteUse[1][1][1]);
        sprintf(cSiteInfo[6],   "[07,%s,%02d]", asSiteTemp[2][1], bTestSiteUse[0][0][1]);
        sprintf(cSiteInfo[7],   "[08,%s,%02d]", asSiteTemp[3][1], bTestSiteUse[0][1][1]);
        sprintf(cSiteInfo[8],   "[09,%s,%02d]", asSiteTemp[0][2], bTestSiteUse[1][0][2]);
        sprintf(cSiteInfo[9],   "[10,%s,%02d]", asSiteTemp[1][2], bTestSiteUse[1][1][2]);
        sprintf(cSiteInfo[10],  "[11,%s,%02d]", asSiteTemp[2][2], bTestSiteUse[0][0][2]);
        sprintf(cSiteInfo[11],  "[12,%s,%02d]", asSiteTemp[3][2], bTestSiteUse[0][1][2]);
        sprintf(cSiteInfo[12],  "[13,%s,%02d]", asSiteTemp[0][3], bTestSiteUse[1][0][3]);
        sprintf(cSiteInfo[13],  "[14,%s,%02d]", asSiteTemp[1][3], bTestSiteUse[1][1][3]);
        sprintf(cSiteInfo[14],  "[15,%s,%02d]", asSiteTemp[2][3], bTestSiteUse[0][0][3]);
        sprintf(cSiteInfo[15],  "[16,%s,%02d]", asSiteTemp[3][3], bTestSiteUse[0][1][3]);
        sprintf(cSiteInfo[16],  "[17,%s,%02d]", asSiteTemp[0][4], bTestSiteUse[1][0][4]);
        sprintf(cSiteInfo[17],  "[18,%s,%02d]", asSiteTemp[1][4], bTestSiteUse[1][1][4]);
        sprintf(cSiteInfo[18],  "[19,%s,%02d]", asSiteTemp[2][4], bTestSiteUse[0][0][4]);
        sprintf(cSiteInfo[19],  "[20,%s,%02d]", asSiteTemp[3][4], bTestSiteUse[0][1][4]);
        sprintf(cSiteInfo[20],  "[21,%s,%02d]", asSiteTemp[0][5], bTestSiteUse[1][0][5]);
        sprintf(cSiteInfo[21],  "[22,%s,%02d]", asSiteTemp[1][5], bTestSiteUse[1][1][5]);
        sprintf(cSiteInfo[22],  "[23,%s,%02d]", asSiteTemp[2][5], bTestSiteUse[0][0][5]);
        sprintf(cSiteInfo[23],  "[24,%s,%02d]", asSiteTemp[3][5], bTestSiteUse[0][1][5]);
        sprintf(cSiteInfo[24],  "[25,%s,%02d]", asSiteTemp[0][6], bTestSiteUse[1][0][6]);
        sprintf(cSiteInfo[25],  "[26,%s,%02d]", asSiteTemp[1][6], bTestSiteUse[1][1][6]);
        sprintf(cSiteInfo[26],  "[27,%s,%02d]", asSiteTemp[2][6], bTestSiteUse[0][0][6]);
        sprintf(cSiteInfo[27],  "[28,%s,%02d]", asSiteTemp[3][6], bTestSiteUse[0][1][6]);
        sprintf(cSiteInfo[28],  "[29,%s,%02d]", asSiteTemp[0][7], bTestSiteUse[1][0][7]);
        sprintf(cSiteInfo[29],  "[30,%s,%02d]", asSiteTemp[1][7], bTestSiteUse[1][1][7]);
        sprintf(cSiteInfo[30],  "[31,%s,%02d]", asSiteTemp[2][7], bTestSiteUse[0][0][7]);
        sprintf(cSiteInfo[31],  "[32,%s,%02d]", asSiteTemp[3][7], bTestSiteUse[0][1][7]);
    }

    strcpy(cSiteMap, cSiteInfo[0]);
    return iSiteCnt;
}
//---------------------------------------------------------------------------
void TfMain::GetSiteMappingForSIGURD(LPSTR cSiteMap)                            //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
{
    if(InitialOK==false)
    {
         return;
    }
    bool bSites[MAX_SOCKET_ROW*MAX_SOCKET_COL];
    ZeroMemory(bSites , sizeof(bSites));
    unsigned long ulvalue=0;
    int iTolCh=0;

    iTolCh=TestSocket.iShtRow*TestSocket.iShtCol;
    for(int i=0;i<iTolCh;i++)
    {
        if(GetSiteOnOffByChannel((i+1),TestSocket.iShtRow,TestSocket.iShtCol))
        {
            ulvalue|=(1<<i);
        }
    }
    sprintf(cSiteMap, "%08X", ulvalue);
}
//---------------------------------------------------------------------------
bool TfMain::GetSiteOnOffByChannel(int iCh,int iTolRow,int iTolCol)             //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
{
    int iFlag=GetShtModeFlag();
    for(int icol=0;icol<iTolCol;icol++)
    {
        for(int irow=0;irow<iTolRow;irow++)
        {
            if(TestIF_File.iSiteMap[irow][icol]==iCh)
            {
                if(iFlag==2)
                {
                    return bTestSiteUse[0][irow][icol];
                }
                else
                 return bTestSiteUse[iFlag][irow][icol];
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfMain::SetSiteOnOffByChannel(int iCh, bool bSwitch)
{
    int iFlag=GetShtModeFlag();
    for(int icol=0;icol<TestSocket.iShtCol;icol++)
    {
        for(int irow=0;irow<TestSocket.iShtRow;irow++)
        {
            if(TestIF_File.iSiteMap[irow][icol]==iCh)
            {
                if(iFlag==2)
                {
                    bLowYieldCloseSite[0][irow][icol]=bSwitch;
                    bLowYieldCloseSite[1][irow][icol]=bSwitch;
                    return true;
                }
                else
                {
                    bLowYieldCloseSite[iFlag][irow][icol]=bSwitch;
                    return true;
                }

            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void TfMain::SetSiteOnOff(AnsiString hexStr)
{
    if(InitialOK==false)
    {
         return;
    }
    bool bChSwitch[MAX_SOCKET_ROW*MAX_SOCKET_COL];
    ZeroMemory(bChSwitch, sizeof(bChSwitch));
    if(ParseHexToBoolArray(hexStr, bChSwitch)==true)
    {
        int iTolCh=TestSocket.iShtRow*TestSocket.iShtCol;
        for(int i=0; i<iTolCh; i++)
        {
            SetSiteOnOffByChannel((i+1), bChSwitch[i]);
        }
        ShowTestHeadComp(false);
    }
    fYieldMonitoring->bGetGPIBAutoSiteOff=true;
}
//---------------------------------------------------------------------------
bool TfMain::ParseHexToBoolArray(AnsiString hexStr, bool* bArr)
{
    hexStr=hexStr.Trim();
    if(hexStr.Length()!=8)
    {
        return false;                                                           //字串長度必須為 8
    }
    int iTotalChNum=MAX_SOCKET_ROW*MAX_SOCKET_COL;
    unsigned int hexValue = StrToIntDef("$" + hexStr, 0);
    for (int i=0; i<iTotalChNum; i++)
    {
        if ((hexValue & (1 << i)) != 0)
        {
            bArr[i] = true;
        }
        else
        {
            bArr[i] = false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void TfMain::HexCharToBits(char hexChar, bool* bArr, int startIndex)
{
    int value;
    if(hexChar>='0' && hexChar<='9')
        value=hexChar-'0';                                                      // 0-9
    else if(hexChar>='A' && hexChar<='F')
        value=hexChar-'A'+10;                                                   // A-F
    else if(hexChar>='a' && hexChar<='f')
        value=hexChar-'a'+10;                                                   // a-f
    else
        return;
    bArr[startIndex]  =(value&0x8)!=0;                                          // 最高位
    bArr[startIndex+1]=(value&0x4)!=0;
    bArr[startIndex+2]=(value&0x2)!=0;
    bArr[startIndex+3]=(value&0x1)!=0;                                          // 最低位
}
//---------------------------------------------------------------------------
int TfMain::SetTempByDLL(int iTempModeEPSON, double dTempVal)
{
    int ret, iTempMode;

    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(SettingsIsWindowOpened()==true)
    {
        return -6;                                                              //Settings Window is Opened
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
    {
        return -4;                                                              //Operation not Allowed
    }

    if(iTempModeEPSON>2 || iTempModeEPSON<1)
    {
        return -3;                                                              //Parameter Error
    }

    if(iTempModeEPSON==2 && (dTempVal<dTempMin || dTempVal>dTempMax))
    {
        return -3;                                                              //Parameter Error
    }

    if(iTempModeEPSON==2)                                                       //高溫iTempMode=2, EPSON定義的指令
    {
        iTempMode=Tempture_Hot;
        Temperature.iMachineTempMode=0;
        LastSet.iTemperature=Tempture_Hot;
    }
    else
    {
        iTempMode=Tempture_Ambient;
        Temperature.iMachineTempMode=1;
        LastSet.iTemperature=Tempture_Ambient;
        dTempVal=25;
    }

    edWorkTemperBase->Text=CheckRange(dTempVal, dTempMax, dTempMin);

    ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));

    if(ret==0)
    {
        if(iTempModeEPSON==2)                                                   //高溫iTempMode=2, EPSON定義的指令
        {
            iTempMode=Tempture_Hot;
            Temperature.iMachineTempMode=0;
        }
        else
        {
            iTempMode=Tempture_Ambient;
            Temperature.iMachineTempMode=1;
        }

        ret=ChangeTempMode(iTempMode, false, bRefreshFunction, true, true);

        if(ret==0)
        {
            return 0;
        }
        else
        {
            return -1;                                                          //General Error
        }
    }
    else
    {
        return -1;
    }
}
//---------------------------------------------------------------------------
int TfMain::GetTempSettingByDLL()
{
    AnsiString asTemp;
    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(LastSet.iTemperature==Tempture_Hot)                                      //配合EPSON的定義,  Temperature Mode (1 = Ambient; 2 = High Temp)
    {
        asTemp="2";
    }
    else
    {
        asTemp="1";
    }

    AnsiString asWorkTemperBase;
    asWorkTemperBase.sprintf("%.1f", Temperature.fWorkTemperBase);
    if(Temperature.fWorkTemperBase>=0)
    {
        asTemp=asTemp+",+"+asWorkTemperBase;
    }
    else
    {
        asTemp=asTemp+",-"+asWorkTemperBase;
    }
    strcpy(CmdData->cGetTempSettings_Cmd5, asTemp.c_str());
    return 0;
}
//----------------------------------------------------------------------------
int TfMain::FTPDownloadByDLL(LPSTR cRecipeName)
{
    AnsiString Msg;
    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(SettingsIsWindowOpened()==true)
    {
        return -6;                                                              //Settings Window is Opened
    }

    if(HasICUnderMachine() || HasAnyICInMachine() || LastSet.iTester==OFF_LINE)
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    fFTPClient->bControlBySECSGEM=true;
    fFTPClient->aSetUpNameBySECSGEM=cRecipeName;
    fFTPClient->ShowFTPModal(0);
    if(fFTPClient->iErrorBySECSGEM!=0)
    {
        fFTPClient->iErrorBySECSGEM=-1;
    }
    else                                                                        //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        fLotInfo->SetLotStart(__FUNC__, false);
        GetCriticalParaAuth();
    }
    return fFTPClient->iErrorBySECSGEM;
}
//---------------------------------------------------------------------------
int TfMain::GetBinCountByDLL(int iCategNum)
{
    int ret=-1;

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(iCategNum>=iTestBinCount || iCategNum<0)
    {
        return -3;                                                              //Parameter Error
    }

    TastCategory.UpdataCount(true);                                             //Steven 20250514 : 統一計算數量

    if(iCategNum>0)
    {
        if(Prod.iT6CatData[iCategNum]<0)                                        //QQQ
        {
            ret=-1;
        }
        else
        {
            ret=TastCategory.iTotalCategory[iCategNum];
        }
    }

    return ret;
}
//---------------------------------------------------------------------------
int TfMain::ClearBinCountByDLL()
{
    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }
    fMain->Clarn_Data(1, "ClearBinCountByDLL");

    return 0;
}
//---------------------------------------------------------------------------
int TfMain::GetSortCountByDLL(int nTrayNum)
{
    int ret=0;
    if(nTrayNum<0 || nTrayNum>6)
    {
        return -1;                                                              //the tray is not defined
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(nTrayNum==0)                                                             //0: total count for all trays
    {
        ret=RunInfo.iUnloadCount;
    }
    else
    {
        ret=LastSet.BinCT[0][iTo3Unload[nTrayNum-1]];
    }

    return ret;
}
//---------------------------------------------------------------------------
int TfMain::ClearSortCountByDLL()
{
    if(SystemStart==true)                                                       //保護
    {
        return -4;                                                              //Operation not Allowed
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }
    fCounterClear->ClearCount(ctTraySortCount);

    return 0;
}
//---------------------------------------------------------------------------
int TfMain::GetHandlerStatusByDll()
{
    int ret=6;
//INIT=0        Breaker is ON but Power is OFF
//IDLE=1        Power is ON but handler display status is HALT
//RUNNING=2     Operator or remote control has started the handler
//PAUSE=3       Operator or remote control has paused the handler
//SYSERROR=4    Temporarily paused by an ALARM
//MANUAL=5      Operator has opened a setting window
//UNDEFINED=6  (Undefined)

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(SystemStart)
    {
        ret=2;
    }
    else
    {
        if(((MyMessageBox->fShow==true && iUnLoaderCount==0) || fNote->fShow==true) && bAlarmReset==false)
        {
            ret=4;
        }
        //----------判斷是否在設定參數
        else if(SettingsIsWindowOpened()==true)
        {
            ret=5;
        }
        else if(Sen[SnMotorPower].IsOff())
        {
            ret=0;
        }
        else
        {
            if(fMain->CheckCanChangeRealDummy()==false)
            {
                ret=3;
            }
            else if(fMain->palMainStatus->Caption=="HALT")
            {
                ret=1;
            }
            else
            {
                ret=6;
            }
        }
    }
    return ret;
}
//---------------------------------------------------------------------------
double TfMain::GetAlarmStatusByDll()
{
    //JAMCode Format:
    //xx.yyy
    //xx = Unit Code
    //yyy = Alarm Code
    AnsiString asJamCode, asAlarmCode,asUnit, asResult;
    asJamCode=fNote->edErrorCode->Text;
    double ret=00.000;

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(fNote->fShow==true)
    {
        if(asJamCode.Pos("JAM")>0)
        {
            asAlarmCode=asJamCode.SubString(4, 2);
            asUnit=asJamCode.SubString(6, 3);
            asResult.sprintf("%s.%03d", asAlarmCode, asUnit.ToInt());
            ret=atof(asResult.c_str());
        }
        else
        {
            ret=00.000;
        }
    }
    else
    {
        ret=00.000;
    }
    return ret;
}
//---------------------------------------------------------------------------
int TfMain::RemoteControl(int iMode)
{
    int ret=0;

    if(InitialOK==false)
    {
         return -1;                                                             //Operation not Allowed
    }

    if(SettingsIsWindowOpened() || IsIndexMotorOutOfPower() || MyMessageBox->fShow==true ||
       fNote->fShow==true || fAllMotorHome==false || iHome==1)
    {
        return -1;                                                              //Operation not Allowed
    }

    if(iMode==2)                                                                //Start
    {
        if(SystemStart)
        {
            ret=0;
        }
        else
        {
            if(CheckSafeDoorIsClosed()==false)
            {
                return -1;
            }
            fMain->Start("RemoteControl Start");
            ret=0;
        }
    }
    else if(iMode==0)                                                           //Reset
    {
        fMain->Pause("RemoteControl Reset");
        ret=0;
//        fMain->BtnResetClick(fMain);
    }
    else                                                                        //Pause
    {
        fMain->Pause("RemoteControl Pause");
        ret=0;
    }
    return ret;
}
//---------------------------------------------------------------------------
int TfMain::GetBinCountPerSiteByDLL(int iCategNum, int iSiteNum)
{
    int iSocketCT=0;
    int iMode=TestIF_File.iTestMode;
    int iSiteCnt=SiteData[iMode].Cnt;
    int iRowCnt, iColCnt;
    int ret=-1;

//        a  b  c  d  e  f  g  h     **        a  b  c  d
//     A  1  3  5  7  9 11 13 15     **     A  1  2  3  4
//     B  2  4  6  8 10 12 14 16     **

    if(iCategNum>15 || iCategNum<0)
    {
        ret=-3;
        return ret;
    }

    if(iSiteNum>iSiteCnt || iSiteNum<0)
    {
        ret=-3;
        return ret;
    }

    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }
    TastCategory.UpdataCount(true);                                             //Steven 20250514 : 統一計算數量

    if(TestSocket.iShtRow==1)
    {
        iRowCnt=0;
        iColCnt=iSiteNum-1;
    }
    else if(TestSocket.iShtRow==2)
    {
        if(iSiteNum%2==0)     //Row B
        {
            iRowCnt=1;
            iColCnt=(iSiteNum/2)-1;
            if(iColCnt<0)
                iColCnt=0;
        }
        else                  //Row A
        {
            iRowCnt=0;
            iColCnt=iSiteNum/2;
        }
    }
    else
    {
        ret=-3;
        return ret;
    }

    iSocketCT=TastCategory.iCountCategory[0][iRowCnt][iColCnt][iCategNum]+TastCategory.iCountCategory[1][iRowCnt][iColCnt][iCategNum];
    return iSocketCT;
}
//---------------------------------------------------------------------------
int TfMain::GetTempActualByDLL()
{
    AnsiString t;
    AnsiString asSite="NULL";

    if(InitialOK==false)
    {
         return -4;  //Operation not Allowed
    }

    RefreshTempData();

    if(TestIF.iTestMode==SingleSite)
    {
            t.sprintf("1,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0);
    }
    else if(TestIF.iTestMode==DualSite || TestIF.iTestMode==DualSite2x1)       // Dual Site 1x2
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0);
    }
    else if(TestIF.iTestMode==TriSite1X3)
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0);
    }
    else if(TestIF.iTestMode==QualSite1X4 || TestIF.iTestMode==QualSite2X2) // Qual Site 1x4
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR" && asTempArmOrder[0][3]!="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR" && asTempArmOrder[1][3]!="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0);
    }
    //QQ 20230214 : 2x2N
    else if(TestIF.iTestMode==_6Site2X3)
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR" && asTempArmOrder[0][3]!="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR" && asTempArmOrder[0][4]!="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR" && asTempArmOrder[0][5]!="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR" && asTempArmOrder[1][3]!="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR" && asTempArmOrder[1][4]!="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR" && asTempArmOrder[1][5]!="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0);
    }
    //QQ 20230214 : 2x3N
    else if(TestIF.iTestMode==_8Site2X4)      // 8 Site 2x4
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f,13,%+03.1f,14,%+03.1f,15,%+03.1f,16,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR" && asTempArmOrder[0][0]!="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR" && asTempArmOrder[0][1]!="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR" && asTempArmOrder[0][2]!="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR" && asTempArmOrder[0][3]!="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR" && asTempArmOrder[0][4]!="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR" && asTempArmOrder[0][5]!="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[0][6]!="ERR" && asTempArmOrder[0][6]!="NULL")?atof(asTempArmOrder[0][6].c_str()):0.0,
                         (asTempArmOrder[0][7]!="ERR" && asTempArmOrder[0][7]!="NULL")?atof(asTempArmOrder[0][7].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR" && asTempArmOrder[1][0]!="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR" && asTempArmOrder[1][1]!="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR" && asTempArmOrder[1][2]!="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR" && asTempArmOrder[1][3]!="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR" && asTempArmOrder[1][4]!="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR" && asTempArmOrder[1][5]!="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0,
                         (asTempArmOrder[1][6]!="ERR" && asTempArmOrder[1][6]!="NULL")?atof(asTempArmOrder[1][6].c_str()):0.0,
                         (asTempArmOrder[1][7]!="ERR" && asTempArmOrder[1][7]!="NULL")?atof(asTempArmOrder[1][7].c_str()):0.0);
    }
    //QQ 20230214 : 2x5
    else if(TestIF.iTestMode==_12Site2X6)   //12 Site                       //wei 20150702
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f,13,%+03.1f,14,%+03.1f,15,%+03.1f,16,%+03.1f,17,%+03.1f,18,%+03.1f,19,%+03.1f,20,%+03.1f,21,%+03.1f,22,%+03.1f,23,%+03.1f,24,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR"  && asTempArmOrder[0][0] !="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR"  && asTempArmOrder[0][1] !="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR"  && asTempArmOrder[0][2] !="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR"  && asTempArmOrder[0][3] !="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR"  && asTempArmOrder[0][4] !="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR"  && asTempArmOrder[0][5] !="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[0][6]!="ERR"  && asTempArmOrder[0][6] !="NULL")?atof(asTempArmOrder[0][6].c_str()):0.0,
                         (asTempArmOrder[0][7]!="ERR"  && asTempArmOrder[0][7] !="NULL")?atof(asTempArmOrder[0][7].c_str()):0.0,
                         (asTempArmOrder[0][8]!="ERR"  && asTempArmOrder[0][8] !="NULL")?atof(asTempArmOrder[0][8].c_str()):0.0,
                         (asTempArmOrder[0][9]!="ERR"  && asTempArmOrder[0][9] !="NULL")?atof(asTempArmOrder[0][9].c_str()):0.0,
                         (asTempArmOrder[0][10]!="ERR" && asTempArmOrder[0][10]!="NULL")?atof(asTempArmOrder[0][10].c_str()):0.0,
                         (asTempArmOrder[0][11]!="ERR" && asTempArmOrder[0][11]!="NULL")?atof(asTempArmOrder[0][11].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR"  && asTempArmOrder[1][0] !="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR"  && asTempArmOrder[1][1] !="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR"  && asTempArmOrder[1][2] !="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR"  && asTempArmOrder[1][3] !="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR"  && asTempArmOrder[1][4] !="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR"  && asTempArmOrder[1][5] !="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0,
                         (asTempArmOrder[1][6]!="ERR"  && asTempArmOrder[1][6] !="NULL")?atof(asTempArmOrder[1][6].c_str()):0.0,
                         (asTempArmOrder[1][7]!="ERR"  && asTempArmOrder[1][7] !="NULL")?atof(asTempArmOrder[1][7].c_str()):0.0,
                         (asTempArmOrder[1][8]!="ERR"  && asTempArmOrder[1][8] !="NULL")?atof(asTempArmOrder[1][8].c_str()):0.0,
                         (asTempArmOrder[1][9]!="ERR"  && asTempArmOrder[1][9] !="NULL")?atof(asTempArmOrder[1][9].c_str()):0.0,
                         (asTempArmOrder[1][10]!="ERR" && asTempArmOrder[1][10]!="NULL")?atof(asTempArmOrder[1][10].c_str()):0.0,
                         (asTempArmOrder[1][11]!="ERR" && asTempArmOrder[1][11]!="NULL")?atof(asTempArmOrder[1][11].c_str()):0.0);
    }
    else if(TestIF.iTestMode==_16Site2X8)   //16 Site
    {
            t.sprintf("1,%+03.1f,2,%+03.1f,3,%+03.1f,4,%+03.1f,5,%+03.1f,6,%+03.1f,7,%+03.1f,8,%+03.1f,9,%+03.1f,10,%+03.1f,11,%+03.1f,12,%+03.1f,13,%+03.1f,14,%+03.1f,15,%+03.1f,16,%+03.1f,17,%+03.1f,18,%+03.1f,19,%+03.1f,20,%+03.1f,21,%+03.1f,22,%+03.1f,23,%+03.1f,24,%+03.1f,25,%+03.1f,26,%+03.1f,27,%+03.1f,28,%+03.1f,29,%+03.1f,30,%+03.1f,31,%+03.1f,32,%+03.1f",
                         (asTempArmOrder[0][0]!="ERR"  && asTempArmOrder[0][0] !="NULL")?atof(asTempArmOrder[0][0].c_str()):0.0,
                         (asTempArmOrder[0][1]!="ERR"  && asTempArmOrder[0][1] !="NULL")?atof(asTempArmOrder[0][1].c_str()):0.0,
                         (asTempArmOrder[0][2]!="ERR"  && asTempArmOrder[0][2] !="NULL")?atof(asTempArmOrder[0][2].c_str()):0.0,
                         (asTempArmOrder[0][3]!="ERR"  && asTempArmOrder[0][3] !="NULL")?atof(asTempArmOrder[0][3].c_str()):0.0,
                         (asTempArmOrder[0][4]!="ERR"  && asTempArmOrder[0][4] !="NULL")?atof(asTempArmOrder[0][4].c_str()):0.0,
                         (asTempArmOrder[0][5]!="ERR"  && asTempArmOrder[0][5] !="NULL")?atof(asTempArmOrder[0][5].c_str()):0.0,
                         (asTempArmOrder[0][6]!="ERR"  && asTempArmOrder[0][6] !="NULL")?atof(asTempArmOrder[0][6].c_str()):0.0,
                         (asTempArmOrder[0][7]!="ERR"  && asTempArmOrder[0][7] !="NULL")?atof(asTempArmOrder[0][7].c_str()):0.0,
                         (asTempArmOrder[0][8]!="ERR"  && asTempArmOrder[0][8] !="NULL")?atof(asTempArmOrder[0][8].c_str()):0.0,
                         (asTempArmOrder[0][9]!="ERR"  && asTempArmOrder[0][9] !="NULL")?atof(asTempArmOrder[0][9].c_str()):0.0,
                         (asTempArmOrder[0][10]!="ERR" && asTempArmOrder[0][10]!="NULL")?atof(asTempArmOrder[0][10].c_str()):0.0,
                         (asTempArmOrder[0][11]!="ERR" && asTempArmOrder[0][11]!="NULL")?atof(asTempArmOrder[0][11].c_str()):0.0,
                         (asTempArmOrder[0][12]!="ERR" && asTempArmOrder[0][12]!="NULL")?atof(asTempArmOrder[0][12].c_str()):0.0,
                         (asTempArmOrder[0][13]!="ERR" && asTempArmOrder[0][13]!="NULL")?atof(asTempArmOrder[0][13].c_str()):0.0,
                         (asTempArmOrder[0][14]!="ERR" && asTempArmOrder[0][14]!="NULL")?atof(asTempArmOrder[0][14].c_str()):0.0,
                         (asTempArmOrder[0][15]!="ERR" && asTempArmOrder[0][15]!="NULL")?atof(asTempArmOrder[0][15].c_str()):0.0,
                         (asTempArmOrder[1][0]!="ERR"  && asTempArmOrder[1][0] !="NULL")?atof(asTempArmOrder[1][0].c_str()):0.0,
                         (asTempArmOrder[1][1]!="ERR"  && asTempArmOrder[1][1] !="NULL")?atof(asTempArmOrder[1][1].c_str()):0.0,
                         (asTempArmOrder[1][2]!="ERR"  && asTempArmOrder[1][2] !="NULL")?atof(asTempArmOrder[1][2].c_str()):0.0,
                         (asTempArmOrder[1][3]!="ERR"  && asTempArmOrder[1][3] !="NULL")?atof(asTempArmOrder[1][3].c_str()):0.0,
                         (asTempArmOrder[1][4]!="ERR"  && asTempArmOrder[1][4] !="NULL")?atof(asTempArmOrder[1][4].c_str()):0.0,
                         (asTempArmOrder[1][5]!="ERR"  && asTempArmOrder[1][5] !="NULL")?atof(asTempArmOrder[1][5].c_str()):0.0,
                         (asTempArmOrder[1][6]!="ERR"  && asTempArmOrder[1][6] !="NULL")?atof(asTempArmOrder[1][6].c_str()):0.0,
                         (asTempArmOrder[1][7]!="ERR"  && asTempArmOrder[1][7] !="NULL")?atof(asTempArmOrder[1][7].c_str()):0.0,
                         (asTempArmOrder[1][8]!="ERR"  && asTempArmOrder[1][8] !="NULL")?atof(asTempArmOrder[1][8].c_str()):0.0,
                         (asTempArmOrder[1][9]!="ERR"  && asTempArmOrder[1][9] !="NULL")?atof(asTempArmOrder[1][9].c_str()):0.0,
                         (asTempArmOrder[1][10]!="ERR" && asTempArmOrder[1][10]!="NULL")?atof(asTempArmOrder[1][10].c_str()):0.0,
                         (asTempArmOrder[1][11]!="ERR" && asTempArmOrder[1][11]!="NULL")?atof(asTempArmOrder[1][11].c_str()):0.0,
                         (asTempArmOrder[1][12]!="ERR" && asTempArmOrder[1][12]!="NULL")?atof(asTempArmOrder[1][12].c_str()):0.0,
                         (asTempArmOrder[1][13]!="ERR" && asTempArmOrder[1][13]!="NULL")?atof(asTempArmOrder[1][13].c_str()):0.0,
                         (asTempArmOrder[1][14]!="ERR" && asTempArmOrder[1][14]!="NULL")?atof(asTempArmOrder[1][14].c_str()):0.0,
                         (asTempArmOrder[1][15]!="ERR" && asTempArmOrder[1][15]!="NULL")?atof(asTempArmOrder[1][15].c_str()):0.0);
    }
    //QQ 20230214 : 4x4
    //QQ 20230214 : 4x8
    else
    {
        return -3;
    }
    strncpy(CmdData->cGetTempActual_Cmd6, t.c_str(), sizeof(CmdData->cGetTempActual_Cmd6));
    return 0;
}
//---------------------------------------------------------------------------
bool TfMain::SettingsIsWindowOpened()
{
    if(fTeach->fShow     || fMotorTest->fShow    || fShuttleMove->fShow      ||
      fHome->fShow       || fLtcSensor->bShow    || fOmron->bShow            ||
      fContact->fShow    || fiosetview->fShow    || fContact->fShow          ||
      fSetup->fShow      || fOffSet->fShow       || fConfiguration->fShow    ||
      fSpeed->fShow      || fDIOFrom->fShow      || fYieldMonitoring->fShow  ||
      fTrayForm->fShow   || fHotPlate->fShow     || fTrayAssignment->fShow   ||
      fTemp_Set->fShow   || FTestIF->fShow       || fCounterClear->fShow     ||
      fLd_ULd->fShow     || fCCLink->bShow       || fTowerLight->fShow       ||
      fCleaning->fShow   || fQAMode->fShow       || fCounterSel->fShow       ||
      FrmRotate->fShow   || fBuilder->fShow      || fStartCondition->fShow   ||
      fBarCode->bShow    || fSecurity->fShow     || fBinSel->bShow           ||
      fFTPClient->bShow)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
//<==
//JerryYang 20181126 (Steven) : support Epson DLL function
//---------------------------------------------------------------------------
void TfMain::WriteSiteOnOff()                                                   //JerryYang 20190627 回傳開關site狀態
{
    char cSiteOnOff[256];
    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                        //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
    {
        GetSiteMappingForSIGURD(cSiteOnOff);
    }
    else
    {
        GetSiteMappingByDLL(cSiteOnOff);
    }
    SendMSG_CMD(MSG_CMD_GetSiteOnOff, cSiteOnOff);
}
//---------------------------------------------------------------------------
void TfMain::AutoSiteOnOff(AnsiString buffer)                                   //JimmyChiu 20250715 : Auto site on/off by GPIB
{
    SetSiteOnOff(buffer);
    AnsiString sRet="OK";
    SendMSG_CMD(MSG_CMD_GetSiteOnOff, sRet);
}
//---------------------------------------------------------------------------
void TfMain::WriteNumOfSites()                                                  //JerryYang 20190627 回傳site count
{
    AnsiString sNumOfSite="";
    int iSiteCnt=0;
    iSiteCnt=GetSiteCount();
    sNumOfSite.sprintf("%d", iSiteCnt);
    SendMSG_CMD(MSG_CMD_GetNumOfSites, sNumOfSite);
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetSamSungTmp(bool bSend)                                    //Steven 20191112 : 三星格式
{
    //Tester Send: TMP?
    //Handler Reply: Chamber,HP1,HP2,SH1,SH2,
    //If heater off, show NA
    //If no chamber, use the average of index heater.
    AnsiString Msg="", Str1, Str;
    int iCount=0;
    double dTemp=0.0, dResult=0.0, dTemp1=0.0;
    double dBase=25.0;
    if(LastSet.iTemperature!=Tempture_Ambient)
        dBase=Temperature.fWorkTemperBase;

    if(Temperature.iIndexHeatMode==ChamberOnly)
    {
        dTemp1=atof(asGPIBTempShow[tcChamber].c_str());

        if(dTemp1>dBase+10.0 ||
           dTemp1<dBase-10.0)
        {
            Str.sprintf("dTemp:%0.2f, Chamber", dTemp1);
            NewRecordProcess("", "GetSamSungTmp error!", Str);
            dTemp1=dBase;
        }
        Str1.sprintf("%0.1f,", dTemp1);
    }
    else
    {
        for(int i=tcHead1; i<=tcHead4; i++)
        {
            if(bUT150Install[i] && asGPIBTempShow[i]!="NULL" && asGPIBTempShow[i]!="ERR" &&
               bGetHeaterUsed(i) && asGPIBTempShow[i]!="..." && asGPIBTempShow[i]!="---")
            {
                dTemp1=atof(asGPIBTempShow[i].c_str());

                if(dTemp1>dBase+10.0 ||
                   dTemp1<dBase-10.0)
                {
                    Str.sprintf("dTemp:%0.2f, Pos:%d", dTemp1, i);
                    NewRecordProcess("", "GetSamSungTmp error!", Str);
                    dTemp1=dBase;
                }

                dTemp+=dTemp1;
                iCount++;
            }
        }

        for(int i=tcAa1; i<=tcBd2; i++)
        {
            if(bUT150Install[i] && asGPIBTempShow[i]!="NULL" && asGPIBTempShow[i]!="ERR" &&
               bGetHeaterUsed(i) && asGPIBTempShow[i]!="..." && asGPIBTempShow[i]!="---")
            {
                dTemp1=atof(asGPIBTempShow[i].c_str());

                if(dTemp1>dBase+10.0 ||
                   dTemp1<dBase-10.0)
                {
                    Str.sprintf("dTemp:%0.2f, Pos:%d", dTemp1, i);
                    NewRecordProcess("", "GetSamSungTmp error!", Str);
                    dTemp1=dBase;
                }

                dTemp+=dTemp1;
                iCount++;
            }
        }

        for(int i=tcAe1; i<=tcBh2; i++)
        {
            if(bUT150Install[i] && asGPIBTempShow[i]!="NULL" && asGPIBTempShow[i]!="ERR" &&
               bGetHeaterUsed(i) && asGPIBTempShow[i]!="..." && asGPIBTempShow[i]!="---")
            {
                dTemp1=atof(asGPIBTempShow[i].c_str());

                if(dTemp1>dBase+10.0 ||
                   dTemp1<dBase-10.0)
                {
                    Str.sprintf("dTemp:%0.2f, Pos:%d", dTemp1, i);
                    NewRecordProcess("", "GetSamSungTmp error!", Str);
                    dTemp1=dBase;
                }

                dTemp+=dTemp1;
                iCount++;
            }
        }

        if(dTemp==0.0 || iCount==0)
        {
            Str1.sprintf("%0.1f,", dBase);
            Str.sprintf("dTemp:%0.2f, iCount:%d", dTemp, iCount);
            NewRecordProcess("", "GetSamSungTmp error!", Str);
        }
        else
        {
            dResult=(dTemp/(double)iCount);
            if(dResult>dBase+10.0 ||
               dResult<dBase-10.0)
            {
                dResult=dBase;
                Str.sprintf("dTemp:%0.2f, iCount:%d", dTemp, iCount);
                NewRecordProcess("", "GetSamSungTmp error!", Str);
            }
            Str1.sprintf("%0.1f,", dResult);
        }
    }
    Msg=Str1;

    if(bUT150Install[tcHotPlate1])
        Str1=asGPIBTempShow[tcHotPlate1]+",";
    else if(bUT150Install[tcHotPlate2])                                         //Steven 20210108 : Hana 要求關掉的加熱盤也要有溫度
        Str1=asGPIBTempShow[tcHotPlate2]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bUT150Install[tcHotPlate2])
        Str1=asGPIBTempShow[tcHotPlate2]+",";
    else if(bUT150Install[tcHotPlate1])                                         //Steven 20210108 : Hana 要求關掉的加熱盤也要有溫度
        Str1=asGPIBTempShow[tcHotPlate1]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bUT150Install[tcShuttle1])
        Str1=asGPIBTempShow[tcShuttle1]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bUT150Install[tcShuttle2])
        Str1=asGPIBTempShow[tcShuttle2]+",";
    else
        Str1="NA,";
    Msg+=Str1;

    if(bSend)
        SendMSG_CMD(MSG_CMD_SamSung_Tmp, Msg);
    return Msg;
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetSamSungMap(bool bSend)                                    //Steven 20191112 : 三星格式
{
    AnsiString Msg="";

    if(TestIF_File.iTestMode==SingleSite  ||
       TestIF_File.iTestMode==DualSite    ||
       TestIF_File.iTestMode==TriSite1X3  ||
       TestIF_File.iTestMode==QualSite1X4)
    {
        Msg.sprintf("%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[0][3]);
    }
    else if(TestIF_File.iTestMode==DualSite2x1 ||
            TestIF_File.iTestMode==QualSite2X2 ||
            TestIF_File.iTestMode==QualSite2X2N)     //Frank 20200520 2X2NN Mode
    {
        Msg.sprintf("%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3  ||
            TestIF_File.iTestMode==_6Site2X3N ||   //Steven 20220425 : 2X3NN Mode
            TestIF_File.iTestMode==_8Site2X4N ||   //Wei 20231211 : 2X4NN Mode
            TestIF_File.iTestMode==_8Site2X4)
    {
        Msg.sprintf("%d%d%d%d%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[1][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[0][3],
                        TestIF_File.iSiteMap[1][2],
                        TestIF_File.iSiteMap[1][3]);
    }
    else if(TestIF_File.iTestMode==_12Site2X6)
    {
        Msg.sprintf("0%d0%d%d%d%d%d%d%d%d%d%d0%d0",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[1][1],
                        TestIF_File.iSiteMap[1][2],
                        TestIF_File.iSiteMap[0][3],
                        TestIF_File.iSiteMap[0][4],
                        TestIF_File.iSiteMap[1][3],
                        TestIF_File.iSiteMap[1][4],
                        TestIF_File.iSiteMap[0][5],
                        TestIF_File.iSiteMap[1][5]);
    }
    else if(TestIF_File.iTestMode==_16Site4X4)
    {
        Msg.sprintf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
                        TestIF_File.iSiteMap[0][0],
                        TestIF_File.iSiteMap[0][1],
                        TestIF_File.iSiteMap[2][0],
                        TestIF_File.iSiteMap[2][1],
                        TestIF_File.iSiteMap[0][2],
                        TestIF_File.iSiteMap[0][3],
                        TestIF_File.iSiteMap[2][2],
                        TestIF_File.iSiteMap[2][3],
                        TestIF_File.iSiteMap[1][0],
                        TestIF_File.iSiteMap[1][1],
                        TestIF_File.iSiteMap[3][0],
                        TestIF_File.iSiteMap[3][1],
                        TestIF_File.iSiteMap[1][2],
                        TestIF_File.iSiteMap[1][3],
                        TestIF_File.iSiteMap[3][2],
                        TestIF_File.iSiteMap[3][3]);
    }
    //QQ 20230214 : 4x8
    else if(TestIF_File.iTestMode==_32Site4X8N ||
            TestIF_File.iTestMode==_32Site4X8M)
    {

    }
    else //if(TestIF_File.iTestMode==_16Site2X8)
    {
        if(TestIF_File.iSiteMap[0][0]==0 &&
           TestIF_File.iSiteMap[1][0]==0 &&
           TestIF_File.iSiteMap[0][1]==0 &&
           TestIF_File.iSiteMap[1][1]==0 &&
           TestIF_File.iSiteMap[0][3]==0 &&
           TestIF_File.iSiteMap[1][3]==0 &&
           TestIF_File.iSiteMap[0][5]==0 &&
           TestIF_File.iSiteMap[1][5]==0 &&
           TestIF_File.iSiteMap[0][6]==0 &&
           TestIF_File.iSiteMap[1][6]==0 &&
           TestIF_File.iSiteMap[0][7]==0 &&
           TestIF_File.iSiteMap[1][7]==0)
        {
            Msg.sprintf("%d%d%d%d",
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[1][4]);
        }
        else if(TestIF_File.iSiteMap[0][1]==0 &&
                TestIF_File.iSiteMap[1][1]==0 &&
                TestIF_File.iSiteMap[0][3]==0 &&
                TestIF_File.iSiteMap[1][3]==0 &&
                TestIF_File.iSiteMap[0][5]==0 &&
                TestIF_File.iSiteMap[1][5]==0 &&
                TestIF_File.iSiteMap[0][7]==0 &&
                TestIF_File.iSiteMap[1][7]==0)
        {
            Msg.sprintf("%d%d%d%d%d%d%d%d",
                            TestIF_File.iSiteMap[0][0],
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[1][0],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[0][6],
                            TestIF_File.iSiteMap[1][4],
                            TestIF_File.iSiteMap[1][6]);
        }
        else if(TestIF_File.iSiteMap[0][0]==0 &&
                TestIF_File.iSiteMap[1][0]==0 &&
                TestIF_File.iSiteMap[0][1]==0 &&
                TestIF_File.iSiteMap[1][1]==0 &&
                TestIF_File.iSiteMap[0][6]==0 &&
                TestIF_File.iSiteMap[1][6]==0 &&
                TestIF_File.iSiteMap[0][7]==0 &&
                TestIF_File.iSiteMap[1][7]==0)
        {
            Msg.sprintf("%d%d%d%d%d%d%d%d",
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[0][3],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[1][3],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[0][5],
                            TestIF_File.iSiteMap[1][4],
                            TestIF_File.iSiteMap[1][5]);
        }
        else
        {
            Msg.sprintf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
                            TestIF_File.iSiteMap[0][0],
                            TestIF_File.iSiteMap[0][1],
                            TestIF_File.iSiteMap[1][0],
                            TestIF_File.iSiteMap[1][1],
                            TestIF_File.iSiteMap[0][2],
                            TestIF_File.iSiteMap[0][3],
                            TestIF_File.iSiteMap[1][2],
                            TestIF_File.iSiteMap[1][3],
                            TestIF_File.iSiteMap[0][4],
                            TestIF_File.iSiteMap[0][5],
                            TestIF_File.iSiteMap[1][4],
                            TestIF_File.iSiteMap[1][5],
                            TestIF_File.iSiteMap[0][6],
                            TestIF_File.iSiteMap[0][7],
                            TestIF_File.iSiteMap[1][6],
                            TestIF_File.iSiteMap[1][7]);
        }
    }

    if(bSend)
        SendMSG_CMD(MSG_CMD_SamSung_Map, Msg);

    return Msg;
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetSamSungSoakTime(bool bSend)
{
    AnsiString t;
    if(LastSet.iTemperature==Tempture_Ambient ||
       LastSet.iTemperature==Tempture_AmbientHot)
    {
        t.sprintf("0");
    }
    else
    {
        t.sprintf("%s", edSoakTime->Text.c_str());
    }

    if(bSend)
        SendMSG_CMD(MSG_CMD_SamSung_Soak, t);

    return t;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::GetTTLState()                                           //Isaac 20200903 :TTL RS232通訊
{
    TStringList *asBuffer;
    asBuffer=new TStringList();
    AnsiString Str;
    HHandler2Gpib.iSendCommand=MSG_CMD_State_TTL;
    memset(HHandler2Gpib.Message,'\0', sizeof(HHandler2Gpib.Message));//清空陣列

    HHandler2Gpib.bGpibMode=true;   //Isaac 20210922 : 選擇TTL板子是否帶站別
    HHandler2Gpib.bSupport32Bin=false;      //true : use two TTL board  //Isaac 20210309 :TTL RS232兩塊板子(先不使用)
    HHandler2Gpib.HandlerHwnd=this->Handle; //Isaac 20211115 : 修正因檢查handler和RS232視窗是否開啟而自動關閉RS232視窗
    HHandler2Gpib.GpibHwnd=HVisionWnd;      //Isaac 20211115 : 修正因檢查handler和RS232視窗是否開啟而自動關閉RS232視窗
    if(TTL_CARD_TYPE==3)
    {
        HHandler2Gpib.bGpibMode=true;   //true:要帶站號
        HHandler2Gpib.bSupport32Bin=true;//true : use two TTL board //Isaac 20210309 :TTL RS232兩塊板子
        if(Prod.DIOCfg.iCateBitLength==_3Bit || Prod.DIOCfg.iCateBitLength==_4Bit   ||
           Prod.DIOCfg.iCateBitLength==_5Bit || Prod.DIOCfg.iCateBitLength==_5BitPE ||
           Prod.DIOCfg.iCateBitLength==_5BitPO)
        {
            if(Prod.DIOCfg.iCateDataType==CHOneByOne)
            {
                //5BitBit
                if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
            else    //CHBinary
            {
                //5BitBinary
                if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
        }
        else
        {
            if(Prod.DIOCfg.iCateDataType==CHOneByOne)
            {
                //10BitBit
                if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
            else    //CHBinary
            {
                //10BitBinary
                if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
                {
                    HHandler2Gpib.bSupport32Bin=false;//true : use two TTL board
                }
            }
        }
    }
    else
    {
        HHandler2Gpib.bGpibMode=TTL_CARD_USE_ADDRESS;   //true:要帶站號     //Isaac 20210922 : 選擇TTL板子是否帶站別
        HHandler2Gpib.bSupport32Bin=false;  //false : use one TTL board
    }
//******************************************
//以下順序不能對調!!
//******************************************
    //Data[0] TS+5V
    asBuffer->Add("0");

    //Data[1] Bin Mode
    if(TestIF_File.bTTLUseASEJPMode)    //Frank 20220408 Add TTL ASE_JP Mode
    {
        asBuffer->Add("4");
    }
    else if(Prod.DIOCfg.iCateBitLength==_3Bit || Prod.DIOCfg.iCateBitLength==_4Bit   ||
            Prod.DIOCfg.iCateBitLength==_5Bit || Prod.DIOCfg.iCateBitLength==_5BitPE ||
            Prod.DIOCfg.iCateBitLength==_5BitPO)
    {
        if(Prod.DIOCfg.iCateDataType==CHOneByOne)
        {
            asBuffer->Add("0");     //5BitBit

            if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
        else    //CHBinary
        {
            asBuffer->Add("2");     //5BitBinary

            if(TestIF.iTestMode<=_8Site2X4)     //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
    }
    else
    {
        if(Prod.DIOCfg.iCateDataType==CHOneByOne)
        {
            asBuffer->Add("1");     //10BitBit

            if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
        else    //CHBinary
        {
            asBuffer->Add("3");     //10BitBinary

            if(TestIF.iTestMode<QualSite2X2)        //Isaac 20210309 :TTL RS232兩塊板子
            {
                HHandler2Gpib.bSupport32Bin=false;//false : use one TTL board
            }
        }
    }
    if(TestIF_File.bTTLUseASEJPMode)    //Frank 20220408 Add TTL ASE_JP Mode
    {
        asBuffer->Add("00000000");
        asBuffer->Add("00000000");
        asBuffer->Add("00000000");
        asBuffer->Add("00000000");
    }
    else
    {
        //Data[2-9] SOT Active Logic
        if(Prod.DIOCfg.iSTLogicMode==0) //Positive
            asBuffer->Add("11111111");
        else                            //Negative
            asBuffer->Add("00000000");

        //Data[10-17] Data Active Logic
        if(Prod.DIOCfg.iCateLogicMode==0)     //Positive
            asBuffer->Add("11111111");
        else                            //Negative
            asBuffer->Add("00000000");

        //Data[18-25] EOT Active Logic
        asBuffer->Add("00000000");

        //Data[26-33] DUT Active Logic
        if(Prod.DIOCfg.iDutType==0 ||   //None
           Prod.DIOCfg.iDutType==1 ||   //Positive Level
           Prod.DIOCfg.iDutType==2)     //Positive Pulse
            asBuffer->Add("11111111");
        else                            //Negative Level || Negative Pulse
            asBuffer->Add("00000000");
    }
    //Data[34-37] SOT Width (MS, 最大1000)
    Str.sprintf("%04d", Prod.DIOCfg.iSTPluseWidth);
    asBuffer->Add(Str);

    //Data[38-41] Dut Width (MS, 最大1000)
    Str.sprintf("%04d", 200);
    asBuffer->Add(Str);

    //Data[42-47] BIN Time Out (MS, 最大500000)
    Str.sprintf("%06d", 0);
    asBuffer->Add(Str);
//******************************************
//以上順序不能對調!!
//******************************************

    Str=StringReplace(asBuffer->Text, "\r\n", "", TReplaceFlags()<<rfReplaceAll);

    strncpy(HHandler2Gpib.Message, Str.c_str(), Str.Length());
    COPYDATASTRUCT *pcp=new COPYDATASTRUCT ;
    pcp->dwData=0;
    pcp->cbData=sizeof(HHandler2Gpib);
    pcp->lpData=(unsigned char *)&HHandler2Gpib.iSendCommand;

    SendMessage(fMain->HVisionWnd, WM_COPYDATA,(WPARAM) NULL, (LPARAM)pcp);
    delete pcp;
    delete asBuffer;
}
//---------------------------------------------------------------------------
void __fastcall TfMain::Send_Command_TTL(AnsiString asStr)
{
    const int iLength=asStr.Length();
    if(TestIF_File.iTestType==TTL_MODE && (TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))       //Isaac 20210309 :TTL RS232兩塊板子
    {
        HHandler2Gpib.iLotStatus=iLength;
        sprintf(HHandler2Gpib.Message, "%s", asStr);
        fMain->SendMSG_CMD(MSG_CMD_Command_TTL);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSetTempStatus_SIGURD()                             //KaiChen 20181129 ：Add GPIB SETTEMP_
{
    int ret=0, TempMode;
    double d=0.0;
    AnsiString t;
    bool bStatusOK=false;
    bool bValueOK=false;

    if(SystemStart==false &&
       (LastSet.iRunStartMode==rsmInitialStart   ||                             //Sam 20210512 : GIPB 設定要卡 Initail
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    d=atof(HGpib2Handler->cReturn);
    dTempMax=fTemp_Set->MaxTempSetting();
    dTempMin=fTemp_Set->MinTempSetting();
    if(d<=dTempMax)                                                             // 檢查是否超出最大值
    {
        bValueOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetTemp, t);
    }
    else if(bValueOK==false)
    {
        t.sprintf("VALUE NG");
        SendMSG_CMD(MSG_CMD_SetTemp, t);
    }
    else
    {
        if(Temperature.bATCActiveCooling)                                       //Sam 20250214 : RMS 新增 ATC 判斷
        {
            if(d>25.0)
            {
                TempMode=1;
            }
            else
            {
                TempMode=0;
            }
            edATCAmbientTemper->Text=d;
        }
        else
        {
            if(d>25)
            {
                TempMode=1;
                edWorkTemperBase->Text=d;
            }
            else
            {
                TempMode=0;
            }
        }
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut
        //==>
        t.sprintf("SETTINGOK>SETTEMP_%d>%d", (int)d, (int)d);
        SendMSG_CMD(MSG_CMD_SetTemp, t);
        //<==
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut
        if(Temperature.bATCActiveCooling)                                       //Sam 20250214 : RMS 新增 ATC 判斷
            ret=SetTemp(false, atof(edATCAmbientTemper->Text.c_str()), atof(edSoakTime->Text.c_str()));
        else
            ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));
        if(ret==0)
        {
            ret=ChangeTempMode(TempMode, false, bRefreshFunction, true);
        }

        if(ret!=0)
        {
            t.sprintf("SETTINGNG>SETTEMP_%d>%s Error", (int)d, edWorkTemperBase->Text);
            ShowMyMessage(t, "");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSetSoakTimeStatus_SIGURD()                         //KaiChen 20181129 ：Add GPIB SETSOAK_
{
    int i, ret=0;
    AnsiString t,S="",szDir="";
    bool bStatusOK=false;
    bool bValueOK=false;

    if(SystemStart==false &&
       (LastSet.iRunStartMode==rsmInitialStart   ||                             //Sam 20210512 : GIPB 設定要卡 Initail
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    i=atoi(HGpib2Handler->cReturn);
    if(i<=1000 && i>=0) //檢查是否超出最大值
    {
        bValueOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetSoakTime, t);
    }
    else if(bValueOK==false)
    {
        t.sprintf("VALUE NG");
        SendMSG_CMD(MSG_CMD_SetSoakTime, t);
    }
    else
    {
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut
        //==>
        edSoakTime->Text=i;
        t.sprintf("SETTINGOK>SETSOAK_%d>%s", i, edSoakTime->Text);
        SendMSG_CMD(MSG_CMD_SetSoakTime, t);
        //<==
        //Sam 20210510 : 先回傳 OK，再來寫入，不然寫入太慢對方會 TimeOut

        ret=SetTemp(false, atof(edWorkTemperBase->Text.c_str()), atof(edSoakTime->Text.c_str()));     //Steven 20120730
        if(ret==0)
        {
            S=GetLastOpenFN();
            szDir.sprintf("%s%s", DataPath, S);
            szDir+="\\Temperature.Data";
            if(i==0)  //常溫
            {
                if(CUSTOMER_CODE==CC_UTAC_TW)   //Sa
                {
                    //Sam 20240606 : 聯測 SoakTime = 0 不要變更溫度模式
                }
                else
                {
                    fMain->ChangeTempMode(0, false, true);
                    WriteIniData(szDir, "Mode", "Mode", 1);
                }
            }
            else      //高溫
            {
                fMain->ChangeTempMode(1, false, true);
                WriteIniData(szDir, "Mode", "Mode", 0);
            }
        }

        if(ret!=0)
        {
            t.sprintf("SETTINGNG>SETSOAK_%d>%s", i, edSoakTime->Text);
            ShowMyMessage(t, "");
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::SetSiteMapData_SIGURD()                                            //KaiChen 20181129 ：Add GPIB SETSITEMAP_
{
    bool ret=true;
    char str[256];
    char str_buf[256];
    AnsiString t;
    bool bStatusOK=false;

    if(SystemStart==false &&
       (LastSet.iRunStartMode==rsmInitialStart   ||                             //Sam 20210512 : GIPB 設定要卡 Initail
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode) &&                                    //Sam 20250214 : RMS 新增 QA 模式
       HasICUnderMachine()==false)
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetSiteMapData, t);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        for(int i=0; i<sizeof(str); i++)
        {
            if(str[i]=='\r' || str[i]=='\n')
            {
                str_buf[i]='\0';
            }
            else
            {
                str_buf[i]=str[i];
            }
        }

        for(int i=0; i<4; i++)  //JerryYang 20160519 備份開關site
        {
            for(int j=0; j<8; j++)
            {
                iBackupDutOnOff[i][j]=bTestSiteUse[0][i][j];
            }
        }
        iBackupTestMode=TestIF_File.iTestMode;
        ret=ChangeToSiteMap(str);

        char *asTestIFSiteMap=GetTestIFSiteMap().c_str();
        if(ret)
        {
            t.sprintf("SETTINGOK>SETSITEMAP_%s>%s\n", str_buf, asTestIFSiteMap);
        }
        else
        {
            t.sprintf("SETTINGNG>SETSITEMAP_%s>%s\n", str_buf, asTestIFSiteMap);
        }
        SendMSG_CMD(MSG_CMD_SetSiteMapData, t);
    }
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetTestIFSiteMap()   //KaiChen 20181129 ：Add GPIB SETSITEMAP_
{
    AnsiString asReturn;
    int iSiteMap[32]={-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2};
    int i=0;
    AnsiString sSiteMode;

    if(TestIF_File.iTestMode==SingleSite)//*   SingleSite
    {
        asReturn="SINGLE1X1";                           //Steven 20230214 : 補上GPIB set site map
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
    }
    else if(TestIF_File.iTestMode==DualSite)//*   DualSite
    {
        asReturn="DUAL1X2";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[0][1];
    }
    else if(TestIF_File.iTestMode==TriSite1X3)//
    {
        asReturn="TRI1X3";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[0][1];
        iSiteMap[2]=TestIF_File.iSiteMap[0][2];
    }
    else if(TestIF_File.iTestMode==QualSite1X4)//*     QualSite1X4    _8Site1X4
    {
        asReturn="QUAD1X4";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[0][1];
        iSiteMap[2]=TestIF_File.iSiteMap[0][2];
        iSiteMap[3]=TestIF_File.iSiteMap[0][3];
    }
    else if(TestIF_File.iTestMode==DualSite2x1)//*    DualSite2x1
    {
        asReturn="DUAL2X1";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
    }
    else if(TestIF_File.iTestMode==QualSite2X2)//*    QualSite2X2
    {
        asReturn="QUAD2X2";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
    }
    else if(TestIF_File.iTestMode==QualSite2X2N)                                //Steven 20230214 : 補上GPIB set site map
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            asReturn="QUAD2X2";
        else
            asReturn="QUAD2X2N";

        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
    }
    else if(TestIF_File.iTestMode==_6Site2X3)//*      _6Site2X3
    {
        asReturn="6SITE2X3";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
    }
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20230214 : 補上GPIB set site map
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            asReturn="6SITE2X3";
        else
            asReturn="6SITE2X3N";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
    }
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(CUSTOMER_CODE==CC_SCK)                                               //Steven 20251024 : JSCK要求修改命令
            asReturn="8SITE2X4";
        else
            asReturn="8SITE2X4N";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
    }
    else if(TestIF_File.iTestMode==_8Site2X4)//*    _8Site2X4
    {
        asReturn="8SITE2X4";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
    }
    else if(TestIF_File.iTestMode==_10Site2X5)
    {
        asReturn="10SITE2X5";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
        iSiteMap[8]=TestIF_File.iSiteMap[0][4];
        iSiteMap[9]=TestIF_File.iSiteMap[1][4];
    }
    else if(TestIF_File.iTestMode==_12Site2X6)//*    _12Site2X6
    {
        asReturn="12SITE2X6";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
        iSiteMap[8]=TestIF_File.iSiteMap[0][4];
        iSiteMap[9]=TestIF_File.iSiteMap[1][4];
        iSiteMap[10]=TestIF_File.iSiteMap[0][5];
        iSiteMap[11]=TestIF_File.iSiteMap[1][5];
    }
    else if(TestIF_File.iTestMode==_16Site2X8) //*     _16Site2X8
    {
        asReturn="16SITE2X8";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[0][1];
        iSiteMap[3]=TestIF_File.iSiteMap[1][1];
        iSiteMap[4]=TestIF_File.iSiteMap[0][2];
        iSiteMap[5]=TestIF_File.iSiteMap[1][2];
        iSiteMap[6]=TestIF_File.iSiteMap[0][3];
        iSiteMap[7]=TestIF_File.iSiteMap[1][3];
        iSiteMap[8]=TestIF_File.iSiteMap[0][4];
        iSiteMap[9]=TestIF_File.iSiteMap[1][4];
        iSiteMap[10]=TestIF_File.iSiteMap[0][5];
        iSiteMap[11]=TestIF_File.iSiteMap[1][5];
        iSiteMap[12]=TestIF_File.iSiteMap[0][6];
        iSiteMap[13]=TestIF_File.iSiteMap[1][6];
        iSiteMap[14]=TestIF_File.iSiteMap[0][7];
        iSiteMap[15]=TestIF_File.iSiteMap[1][7];
    }
    else if(TestIF_File.iTestMode==_16Site4X4)          //Steven 20230214 : 補上GPIB set site map
    {
        asReturn="16Site4X4";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[2][0];
        iSiteMap[3]=TestIF_File.iSiteMap[3][0];
        iSiteMap[4]=TestIF_File.iSiteMap[0][1];
        iSiteMap[5]=TestIF_File.iSiteMap[1][1];
        iSiteMap[6]=TestIF_File.iSiteMap[2][1];
        iSiteMap[7]=TestIF_File.iSiteMap[3][1];
        iSiteMap[8]=TestIF_File.iSiteMap[0][2];
        iSiteMap[9]=TestIF_File.iSiteMap[1][2];
        iSiteMap[10]=TestIF_File.iSiteMap[2][2];
        iSiteMap[11]=TestIF_File.iSiteMap[3][2];
        iSiteMap[12]=TestIF_File.iSiteMap[0][3];
        iSiteMap[13]=TestIF_File.iSiteMap[1][3];
        iSiteMap[14]=TestIF_File.iSiteMap[2][3];
        iSiteMap[15]=TestIF_File.iSiteMap[3][3];
    }
    else if(TestIF_File.iTestMode==_32Site4X8N)                                 //Steven 20230214 : 補上GPIB set site map
    {
        asReturn="32Site4X8";
        iSiteMap[0]=TestIF_File.iSiteMap[0][0];
        iSiteMap[1]=TestIF_File.iSiteMap[1][0];
        iSiteMap[2]=TestIF_File.iSiteMap[2][0];
        iSiteMap[3]=TestIF_File.iSiteMap[3][0];
        iSiteMap[4]=TestIF_File.iSiteMap[0][1];
        iSiteMap[5]=TestIF_File.iSiteMap[1][1];
        iSiteMap[6]=TestIF_File.iSiteMap[2][1];
        iSiteMap[7]=TestIF_File.iSiteMap[3][1];
        iSiteMap[8]=TestIF_File.iSiteMap[0][2];
        iSiteMap[9]=TestIF_File.iSiteMap[1][2];
        iSiteMap[10]=TestIF_File.iSiteMap[2][2];
        iSiteMap[11]=TestIF_File.iSiteMap[3][2];
        iSiteMap[12]=TestIF_File.iSiteMap[0][3];
        iSiteMap[13]=TestIF_File.iSiteMap[1][3];
        iSiteMap[14]=TestIF_File.iSiteMap[2][3];
        iSiteMap[15]=TestIF_File.iSiteMap[3][3];
        iSiteMap[16]=TestIF_File.iSiteMap[0][4];
        iSiteMap[17]=TestIF_File.iSiteMap[1][4];
        iSiteMap[18]=TestIF_File.iSiteMap[2][4];
        iSiteMap[19]=TestIF_File.iSiteMap[3][4];
        iSiteMap[20]=TestIF_File.iSiteMap[0][5];
        iSiteMap[21]=TestIF_File.iSiteMap[1][5];
        iSiteMap[22]=TestIF_File.iSiteMap[2][5];
        iSiteMap[23]=TestIF_File.iSiteMap[3][5];
        iSiteMap[24]=TestIF_File.iSiteMap[0][6];
        iSiteMap[25]=TestIF_File.iSiteMap[1][6];
        iSiteMap[26]=TestIF_File.iSiteMap[2][6];
        iSiteMap[27]=TestIF_File.iSiteMap[3][6];
        iSiteMap[28]=TestIF_File.iSiteMap[0][7];
        iSiteMap[29]=TestIF_File.iSiteMap[1][7];
        iSiteMap[30]=TestIF_File.iSiteMap[2][7];
        iSiteMap[31]=TestIF_File.iSiteMap[3][7];
    }

    for(i=0; i<32; i++)
    {
        if(iSiteMap[i]>-2)
        {
            asReturn=asReturn+"-"+iSiteMap[i];
        }
    }
    return  asReturn;
}
//---------------------------------------------------------------------------
void TfMain::ChkStatus()                                                        //KaiChen 20180910 ：Add GPIB CHKSTATUS?
{
    char runStartMode[10];
    char MainStatus[10];
    AnsiString t;

    if(fMain->palMainStatus->Caption=="HALT")
    {
        sprintf(MainStatus, "HALT");
    }
    else if(fMain->palMainStatus->Caption=="PAUSE")
    {
        sprintf(MainStatus, "PAUSE");
    }
    else if(fMain->palMainStatus->Caption=="Running")
    {
        sprintf(MainStatus, "RUNNING");
    }

    if(LastSet.iRunStartMode==rsmContinuStart ||                                //確認機台狀態
       LastSet.iRunStartMode==rsmContinuStart_MRT)                              //Sam 20240625 : 新增 MRT Status
    {
        sprintf(runStartMode, "FT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmInitialStart ||
            LastSet.iRunStartMode==rsmInitial_MRT)                              //Sam 20240625 : 新增 MRT Status
    {
        sprintf(runStartMode, "FT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmContinuRetest)
    {                                                     
        sprintf(runStartMode, "RT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmCInitialRetest)
    {
        sprintf(runStartMode, "RT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmRetest_MRT)                               //Sam 20240625 : 新增 MRT Status
    {
        sprintf(runStartMode, "RT");
    }
    else if(LastSet.iRunStartMode==rsmQAMode)                                   //Sam 20240815 : 新增 QA Mode
    {
        sprintf(runStartMode, "QA_INIT");
    }

    if(LastSet.iRunStartMode==rsmContinuStart_MRT ||                            //Sam 20240625 : 新增 MRT Status
       LastSet.iRunStartMode==rsmInitial_MRT      ||
       LastSet.iRunStartMode==rsmRetest_MRT)
    {
        t.sprintf("MRT_%s_%s", MainStatus, runStartMode);
    }
    else
    {
        t.sprintf("%s_%s", MainStatus, runStartMode);
    }
    SendMSG_CMD(MSG_CMD_SIGURD_CHKSTATUS, t);

}
//---------------------------------------------------------------------------
void TfMain::GetBinCategory()   //KaiChen 20180913 ：Add GPIB GETBINCATEGORY?
{
    AnsiString asCategData;
    for(int i=0; i<15; i++)
    {
        asCategData=asCategData+BinSelect[iTestRunMode].iCatDataT3Pos[15-i];   //QQQ
    }
    asCategData=asCategData+(BinSelect[iTestRunMode].IfErrorT3+1);              //Sam 20250221 : 修正矽格 RMS 回傳資料
    SendMSG_CMD(MSG_CMD_GETBINCATEGORY, asCategData);
}
//---------------------------------------------------------------------------
void TfMain::GetSetUpFileName()     //KaiChen 20181022 ：Add GPIB GETSETUPFILENAME?
{
    AnsiString asFileName;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)
    {
        asFileName=fMain->cbSetupFileName->Text+"\r\n";//Sam 20210310 : SetupFile 矽格卓邵要求增加換行字元
    }
    else
    {
        asFileName.sprintf("%s", fMain->cbSetupFileName->Text);
    }
    SendMSG_CMD(MSG_CMD_SETUPFILENAME, asFileName);
}
//---------------------------------------------------------------------------
void TfMain::GetHandlerID_Sigurd()                                              //KaiChen 20200507 ：Add GPIB HANDLERID?
{
    AnsiString asHandlerID;

    asHandlerID=IniConfig.asA32_1_HandlerID;

    if(IniConfig.bA32_2ReturnHandlerID2OI==true)                                //KaiChen 20200618 ：矽格，可以選擇是否回傳 HandlerID 給 OI
    {
        SendMSG_CMD(MSG_CMD_SIGURD_HANDLERID, asHandlerID);
    }
}
//---------------------------------------------------------------------------
void TfMain::SetSetupFileName()                                                 //KaiChen 20190613 ：Add GPIB SGSETUP_
{
    bool ret=false;
    bool bStatusOK=false;
    char str[256];
    AnsiString t;

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));

    AnsiString asChangeSetupFileName=AnsiString(str).Trim();

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SGSETUP, t);
    }
    else
    {
        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)    //KaiChen 20191128 ：矽格-北興，SetupFileName 不等於空白，就回傳 OK
        {
            if(asChangeSetupFileName!="")
                ret=true;
        }
        else
        {
            if(fFTPClient->CheckSetupFileNameFromServer(asChangeSetupFileName)==true)   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
                ret=true;
        }
        if(ret==true)
            t.sprintf("SETTINGOK");
        else
            t.sprintf("SETTINGNG");

        SendMSG_CMD(MSG_CMD_SGSETUP, t);

        if(ret==true)
        {
            ChangeSetupFileName(str);
        }
    }
}
//---------------------------------------------------------------------------
bool TfMain::ChangeSetupFileName(char *str)
{
    AnsiString asChangeSetupFileName=AnsiString(str).Trim();
    AnsiString asNowSetupFileName=fMain->cbSetupFileName->Text;

    if(SystemStart==true)
    {
        return false;
    }

    if(asChangeSetupFileName==asNowSetupFileName)   // 檢查是否與當前 Recipe 一樣
    {
        return false;
    }

    bSigurdDownload_Recipe=true;
    fFTPClient->bControlByGPIB=true;        //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
    fFTPClient->iErrorByGPIB=0;
    fFTPClient->asSetUpNameByGPIB=asChangeSetupFileName;
    fFTPClient->ShowFTPModal(0);

    bSigurdDownload_Recipe=false;
    fFTPClient->bControlByGPIB=false;
    if(fFTPClient->iErrorByGPIB==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
//Richard 20220929 :Add for UTAC 讀檔
//==>
void TfMain::PPSELECTAskFile()
{
    AnsiString asFileName="";
    asFileName.sprintf("%s", fMain->cbSetupFileName->Text);
    SendMSG_CMD(MSG_CMD_ASKPPSELECT, asFileName);
}
void TfMain::PPSELECTLoadFile()
{
    char str[256];

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    AnsiString atemp="";
    AnsiString afiletemp="";
    atemp=str;                                                       //example The Function File Name is "0809_PROD"
//    int itemp=0;                                                   //"PPSELECT 0809_PROD"
//    itemp=str.Pos(PPSELECT);
    afiletemp=(atemp.Trim()).SubString(atemp.Pos("T")+2,atemp.Length());

    bool bHasFile=false;

    if(HasICUnderMachine()==true)
    {
        bHasFile=false;
        ShowMessage("Has IC Under Machine");
    }
    else
    {
        for(int i=0; i<fMain->cbSetupFileName->Items->Count; i++)
        {
            if( (fMain->cbSetupFileName->Items->Strings[i])==afiletemp )
            {
                if(fMain->cbSetupFileName->Text!=fMain->cbSetupFileName->Items->Strings[i])
                {
                    fMain->cbSetupFileName->Text=fMain->cbSetupFileName->Items->Strings[i];
                    fMain->cbSetupFileNameChange(fMain);
                }
                bHasFile=true;
            }
        }
    }

    if(bHasFile==true) //如果沒有檔案 報錯
    {
        SendMSG_CMD(MSG_CMD_PPSELECT, "ECHOCODEOK");
        iFileOkPPSELECT=1;
    }
    else
    {
        SendMSG_CMD(MSG_CMD_PPSELECT, "ECHOCODENG");
        iFileOkPPSELECT=2;
    }

}
//<==
//Richard 20220929 :Add for UTAC 讀檔
//---------------------------------------------------------------------------
void TfMain::SetStartMode()     //KaiChen 20180910 ：Add GPIB SetStartMode_
{
    bool ret=true;
    char str[256];
    AnsiString t;
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));

    ret=ChangeHandlerStartMode(str);

    AnsiString sMultiContactCount=AnsiString(str).Trim();   //用來判別是 FT 還是 RT
    char runStartMode[10];

    if(LastSet.iRunStartMode==rsmContinuStart)  //確認機台狀態
    {
        sprintf(runStartMode, "FT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmInitialStart)
    {
        sprintf(runStartMode, "FT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmContinuRetest)
    {
        sprintf(runStartMode, "RT_CONT");
    }
    else if(LastSet.iRunStartMode==rsmCInitialRetest)
    {
        sprintf(runStartMode, "RT_INIT");
    }
    else if(LastSet.iRunStartMode==rsmQAMode) //Sam 20240301 : 新增 QA 模式
    {
        sprintf(runStartMode, "QA");
    }
    else
    {
        ShowMyMessage("SetStartMode command error1","SetStartMode 指令錯誤1");
    }

    if(ret==true)
    {
        if(sMultiContactCount.SubString(1,2)=="FT")
        {
            t.sprintf("SETTINGOK>SetStartMode_FT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="RT")
        {
            t.sprintf("SETTINGOK>SetStartMode_RT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="QA")    //Sam 20240301 : 新增 QA 模式
        {
            t.sprintf("SETTINGOK>SetStartMode_QA>%s", runStartMode);
        }
        else
        {
            ShowMyMessage("SetStartMode command error2","SetStartMode 指令錯誤2");
        }
    }
    else
    {
        if(sMultiContactCount.SubString(1,2)=="FT")
        {
            t.sprintf("SETTINGNG>SetStartMode_FT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="RT")
        {
            t.sprintf("SETTINGNG>SetStartMode_RT>%s", runStartMode);
        }
        else if(sMultiContactCount.SubString(1,2)=="QA")    //Sam 20240301 : 新增 QA 模式
        {
            t.sprintf("SETTINGNG>SetStartMode_QA>%s", runStartMode);
        }
        else
        {
            ShowMyMessage("SetStartMode command error3","SetStartMode 指令錯誤3");
        }
    }
    SendMSG_CMD(MSG_CMD_SETSTARTMODE, t);
}
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");
//---------------------------------------------------------------------------
bool TfMain::ChangeHandlerStartMode(char *str)                                  //KaiChen 20180910 ：Add GPIB SetStartMode_
{
    static bool bBackupState=false;
    if(SystemStart)
    {
        return false;
    }
    if(LastSet.iRealDummy!=REALLY)
    {
        ShowMyMessage("Please Check Real/Dummy Mode!!!");
        return false;
    }
    if(LastSet.iTester==OFF_LINE)
    {
        ShowMyMessage("Please Check Tester Mode!!!");
        return false;
    }

    AnsiString sMultiContactCount=AnsiString(str).Trim();

    if(sMultiContactCount.SubString(1,2)=="FT")
    {
        if(LastSet.iRunStartMode==rsmContinuRetest)
        {
            return false;
        }
        //fMain->palFTClick(fMain);
        bBackupState=cbRunStartMode->Enabled;
        cbRunStartMode->Enabled=true;                                           //Sam 20210715 : 切換模式不要卡權限
        fMain->palFTClick(fMain);
        cbRunStartMode->Enabled=bBackupState;
    }
    else if(sMultiContactCount.SubString(1, 2)=="RT")
    {
        if(LastSet.iRunStartMode==rsmContinuStart)
        {
            return false;
        }
        //fMain->palRTClick(fMain);
        bBackupState=cbRunStartMode->Enabled;
        cbRunStartMode->Enabled=true;                                           //Sam 20210715 : 切換模式不要卡權限
        fMain->palRTClick(fMain);
        cbRunStartMode->Enabled=bBackupState;
    }
    else if(sMultiContactCount.SubString(1, 2)=="QA")                           //Sam 20240301 : 新增 QA 模式
    {
        if(LastSet.iRunStartMode==rsmContinuStart || LastSet.iRunStartMode==rsmContinuRetest)
        {
            return false;
        }
        SetRunStartMode(rsmQAMode);
    }
    else
    {
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void TfMain::CheckList()     //KaiChen 20190613 ：Add GPIB CHECKLIST?
{
    AnsiString ret="";
    AnsiString t;
    AnsiString sCheckListName;

    sCheckListName.sprintf("%s-OI%04d%02d%02d", fMain->cbSetupFileName->Text, SystemYear, SystemMonth, SystemDate);
    fLotInfo->GenerateCheckList(false, sCheckListName, true);

    ret=fLotInfo->GenerateCheckList(false, fMain->cbSetupFileName->Text, false);

    if(ret=="OK")
    {
        t.sprintf("SETTINGOK");
    }
    else
    {
        t.sprintf("SETTINGNG");
    }

    fLotInfo->RefreshYieldMonitor();

    SendMSG_CMD(MSG_CMD_CHECKLIST, t);
}
//---------------------------------------------------------------------------
void TfMain::SetBinPosChange()                                                  //KaiChen 20190706 ：Add GPIB BINPOS_
{
    AnsiString asRet;
    char str[256];
    AnsiString t;
    bool bStatusOK=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        t.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_BINPOS, t);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asRet=BinPosChange(str);

        if(asRet=="")
        {
            t.sprintf("OK");
        }
        else if(asRet=="NG")
        {
            t.sprintf("NG");
        }
        else
        {
            t.sprintf("NG%s", asRet);
        }
        SendMSG_CMD(MSG_CMD_BINPOS, t);
    }
}
//---------------------------------------------------------------------------
AnsiString TfMain::BinPosChange(char *str) //KaiChen 20190706 ：Add GPIB BINPOS_
{
    AnsiString asBinPos=AnsiString(str).Trim();
    AnsiString asReturn="";
    AnsiString asBuf[16][2];
    int iBinData[16], iTrayData[16];
    int iPosEnd1=0, iPosEnd2=0;
    AnsiString asTemp1="";
    int iFailCurrent, iFailSet, iWhichTrayCurrent;

    RecordProcess(asBinPos);

    for(int i=0; i<16; i++)
    {
        iBinData[i]=0;
        iTrayData[i]=0;
        asBuf[i][0]="";
        asBuf[i][1]="";
    }

    for(int i=0; i<16; i++)
    {
        iPosEnd1=asBinPos.Pos(",");
        asTemp1=asBinPos.SubString(1, iPosEnd1-1);

        iPosEnd2=asTemp1.Pos("_");
        asBuf[i][0]=asTemp1.SubString(1, iPosEnd2-1);
        asBuf[i][1]=asTemp1.SubString(iPosEnd2+1, asTemp1.Length());

        iBinData[i]=atoi(asBuf[i][0].c_str());
//        iTrayData[i]=atoi(asBuf[i][1].c_str());
        if(asBuf[i][1]=="N")    //KaiChen 20191227 ：修改可以將 Bin 設定到 NoUse
        {
            iTrayData[i]=0;
        }
        else
        {
            iTrayData[i]=atoi(asBuf[i][1].c_str());
        }

        asBinPos=asBinPos.SubString(iPosEnd1+1, asBinPos.Length());

        if(iPosEnd1==0 || iPosEnd2==0)
        {
            return "NG";
        }

        if(asBinPos.Length()<=0)
        {
            break;
        }
    }

    for(int i=0; i<16; i++)
    {
        if(iBinData[i]<0 || iBinData[i]>15)
        {
            return "NG";
        }
        //if(iTrayData[i]<0 || iTrayData[i]>6)
        if(iTrayData[i]<0)  //KaiChen 20191227 ：增加回傳異常碼
        {
            return "NG";
        }
    }

    for(int i=0; i<16; i++)
    {
        if(iBinData[i]>0)
        {
            iWhichTrayCurrent   =BinSelect[iTestRunMode].iCatDataT3Pos[iBinData[i]];       //QQQ
            iFailCurrent        =BinSelect[iTestRunMode].iStackDefFailCate[iWhichTrayCurrent-1];
            iFailSet            =BinSelect[iTestRunMode].iStackDefFailCate[iTrayData[i]-1];

            if(iWhichTrayCurrent>0)
            {

                if(iFailCurrent==0 && iFailSet==0)
                {

                }
                else if(iFailCurrent==1 && iFailSet==1)
                {

                }
                else if(iFailCurrent==0 && iFailSet==1 && iTrayData[i]-1!=BinSelect[iTestRunMode].IfErrorT3)  //Good bin -> Fail bin
                {
                    asReturn=asReturn+"_A1";
                }
                else if(iFailCurrent==0 && iFailSet==1 && iTrayData[i]-1==BinSelect[iTestRunMode].IfErrorT3)
                {
                    asReturn=asReturn+"_A2";
                }
                else if(iFailCurrent==1 && iFailSet==0 && iWhichTrayCurrent-1!=BinSelect[iTestRunMode].IfErrorT3)
                {
                    asReturn=asReturn+"_B1";
                }
                else if(iFailCurrent==1 && iFailSet==0 && iWhichTrayCurrent-1==BinSelect[iTestRunMode].IfErrorT3)
                {
                    asReturn=asReturn+"_B2";
                }
                else
                {
                    return "NG";
                }
            }
            else
            {
                if(iTrayData[i]>6)  //KaiChen 20191227 ：增加回傳異常碼
                {
                    asReturn=asReturn+"_D1";
                }
                else
                {
                    asReturn=asReturn+"_C1";
                }
            }
        }
        else
        {
            break;
        }
    }

    if(asReturn=="")
    {
        for(int i=0; i<16; i++)
        {
            if(iBinData[i]>0)
            {
                BinSelect[iTestRunMode].iCatDataT3Pos[iBinData[i]]=iTrayData[i];   //QQQ
            }
            else
            {
                break;
            }
        }

        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            if(i<fBinSel->sBinTraySetT3Pos[iTestRunMode]->Count)
            {
                fBinSel->sBinTraySetT3Pos[iTestRunMode]->Strings[i]=BinSelect[iTestRunMode].iCatDataT3Pos[i];
            }
        }
        fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
        fBinSel->spbSaveClick(this);
        fShowBinSelect->ShowBinSel();
    }

    return asReturn;
}
//---------------------------------------------------------------------------
void TfMain::GetSGFTPSTATUS()                   //Sam 20210329 : Add GPIB SGFTP_STATUS
{
    AnsiString asRet;
    if(IniConfig.bA32EnableFTPAutomation==true)
        asRet.sprintf("ON");
    else
        asRet.sprintf("OFF");
    SendMSG_CMD(MSG_CMD_GetSGFTP_STATUS, asRet);
}
//---------------------------------------------------------------------------
void TfMain::SetSGFTP()                                                         //Sam 20210329 : Add GPIB SGFTP_ SGFTP_ON/SGFTP_OFF
{
    AnsiString asRet,asCmd;
    char str[256];
    AnsiString sPath;
    bool bStatusOK=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        asRet.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetSGFTP, asRet);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();
        sPath=AuthPath+"config.ini";

        if(asCmd.Pos("ON")==1)
        {
            WriteIniData(sPath, "Function", "bA32EnableFTPAutomation", 1);
            fConfiguration->cbA32->Checked=true;
            IniConfig.bA32EnableFTPAutomation=true;
            asRet.sprintf("OK");
        }
        else if(asCmd.Pos("OFF")==1)
        {
            WriteIniData(sPath, "Function", "bA32EnableFTPAutomation", 0);
            IniConfig.bA32EnableFTPAutomation=false;
            fConfiguration->cbA32->Checked=false;
            asRet.sprintf("OK");
        }
        else
        {
            asRet.sprintf("NG");
        }

        fLotInfo->RefreshYieldMonitor();

        SendMSG_CMD(MSG_CMD_SetSGFTP, asRet);
    }
}
//---------------------------------------------------------------------------
void TfMain::SetNONDOUBLEBIN()                                                  //Sam 20210329 : Add GPIB NONDOUBLEBIN_
{
    AnsiString asCmd,str1,asRet,asBin;
    char str[256];
    AnsiString sPath,sCheckListFilePath,asLastFileName;
    TStringList *tNonDoubleBin=new TStringList();
    bool bflag[256]={false};
    int i,iBin;
    bool bStatusOK=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        asRet.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetNONDOUBLEBIN, asRet);
    }
    else
    {
        for(i=0;i<256;i++)
        {
            bflag[i]=false;
        }

        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();

        str1=StringReplace(asCmd,"_", ",", TReplaceFlags()<<rfReplaceAll);
        tNonDoubleBin->CommaText=str1;

        asRet="OK";

        if(SystemStart==true)   //系統在 Run 的時候不給改
        {
            asRet.sprintf("NG");
        }
        else if(tNonDoubleBin->Count==1)
        {
            asBin=tNonDoubleBin->Strings[0];
            asCmd="NULL";
        }
        else if(tNonDoubleBin->Count>1)
        {
            for(i=0;i<tNonDoubleBin->Count;i++)
            {
                if(tNonDoubleBin->Strings[i]!="")
                {
                    iBin=atoi(tNonDoubleBin->Strings[i].c_str());
                    //檢查 Bin 是否有重複設定
                    if(bflag[iBin]==false)
                        bflag[iBin]=true;
                    else
                        asRet="NG";
                }
                else
                {
                    asRet="NG";
                }
            }
        }
        else
        {
            asRet="NG";
        }

        if(asRet=="OK")
            WriteIniData(sCheckListFilePath, "Tester_Control", "NONDOUBLEBIN", asCmd);
        else
            WriteIniData(sCheckListFilePath, "Tester_Control", "NONDOUBLEBIN", "");//有問題要把資料清空

        SendMSG_CMD(MSG_CMD_SetNONDOUBLEBIN, asRet);
    }
    tNonDoubleBin->Clear();
    delete tNonDoubleBin;
}
//---------------------------------------------------------------------------
void TfMain::SetSBinData()                     //Steven 20220421 : Amlogic需要收SBIN
{
    AnsiString asCmd, str2, asSBin;
    int iSBin;
    char str[544];
    strncpy(str, HGpib2Handler->cReturn, HGpib2Handler->GPIBBin);
    asCmd=AnsiString(str).Trim();  //asCmd="30_1|P|20_2|F|3";
    asCmd=asCmd.SubString(0, asCmd.AnsiPos(";")-1);
    asCmd=asCmd.SubString(9, asCmd.Length());
    TStringList *tSBin=new TStringList();
    tSBin->CommaText=asCmd;
    if(tSBin->Count==32)
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=MAX_SOCKET_TOTAL)
                {
                    asSBin=tSBin->Strings[32-TestIF.iSiteMap[i][j]]; //要反序,所以32-
                    iSBin=HexStrToInt(asSBin);
                    TestSocket.cSBin[i][j]=iSBin;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::SetBINCOUNT()                                                      //Sam 20210329 : Add GPIB BINCOUNT_
{
    AnsiString asRet,asCmd,str1,asCondition,str2,asCategory,asBin,asPassFail;
    char str[256];
    AnsiString sPath,sCheckListFilePath,sLastFilePath,asLastFileName;
    TStringList *tBinCount=new TStringList();
    TStringList *tCondition=new TStringList();

    bool bflag[TEST_MAX_BIN]={false};
    int i,j,iBin,iCount,iFailCnt;
    bool bCondition;
    bool bStatusOK=false;

    for(i=0; i<TEST_MAX_BIN; i++)
        bflag[i]=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        asRet.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetBINCOUNT, asRet);
    }
    else
    {
        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);     //CheckList檔案
        sLastFilePath=DataPath+asLastFileName+"\\Tester.Data";              //SetupFile

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();  //asCmd="30_1|P|20_2|F|3";

        str1=StringReplace(asCmd,"_", ",", TReplaceFlags()<<rfReplaceAll);
        tBinCount->CommaText=str1;

        asRet="OK";

        if(asCmd=="NULL")
        {
            for(j=0; j<15; j++)
            {
                asBin.sprintf("IsPassBin%d", j+1);
                WriteIniData(sCheckListFilePath, "Tester_Control", asBin, AnsiString("NULL")); //將所有 Bin Pass/Fail 設為 NULL
                Prod.iPersentIgnore[j]=0;
            }
        }
        else if(tBinCount->Count>1)
        {
            for(j=0; j<15; j++)
            {
                asBin.sprintf("IsPassBin%d", j+1);
                WriteIniData(sCheckListFilePath, "Tester_Control", asBin, AnsiString("NULL")); //將所有 Bin Pass/Fail 設為 NULL
            }
            for(i=0;i<tBinCount->Count;i++)
            {
                if(tBinCount->Strings[i]!="")
                {
                    if(i==0)
                    {
                        iCount=atoi(tBinCount->Strings[i].c_str());//LowYield(By Total) after contact count & By Site Compare after contact count
                        if(iCount>1)
                        {
                            WriteIniData(sCheckListFilePath, "FT_Yield", "Preset", iCount);                    //寫入 Check List
                            sLastFilePath=DataPath+asLastFileName+"\\Tester.Data";
                            if(iTestRunMode==FT)
                            {
                                WriteIniData(sLastFilePath, "Site Yield Alarm", "Site Yield Compare Count",iCount);     //寫入 FT SetupFile
                                TestIF_File.iLowYieldCountByTotal=iCount;
                                WriteIniData(sLastFilePath, "Low Yield Alarm", "By Total Count",iCount);                //寫入 FT SetupFile
                                TestIF_File.iFailAlarmSiteYieldCmpCount=iCount;
                            }
                            else if(iTestRunMode==RT)
                            {
                                WriteIniData(sLastFilePath, "Site Yield Alarm", "Site Yield Compare Count RT",iCount);  //寫入 RT SetupFile
                                TestIF.iLowYieldCountByTotal_RT=iCount;
                                WriteIniData(sLastFilePath, "Low Yield Alarm", "By Total Count RT",iCount);             //寫入 RT SetupFile
                                TestIF_File.iFailAlarmSiteYieldCmpCount_RT=iCount;
                            }
                        }
                        else
                        {
                            asRet="NG";
                        }
                    }
                    else
                    {
                        asCondition=tBinCount->Strings[i].c_str();//1|P|Y Bin1 Pass, Yield Ignore Cnt 數量為 Y
                        str2=StringReplace(asCondition,"|", ",", TReplaceFlags()<<rfReplaceAll);
                        tCondition->CommaText=str2;
                        if(tCondition->Count==3)
                        {
                            bCondition=true;
                            iBin=atoi(tCondition->Strings[0].c_str());
                            asPassFail=tCondition->Strings[1];
                            iFailCnt=atoi(tCondition->Strings[2].c_str());

                            //檢查 Bin 是否有重複設定
                            if(bflag[iBin]==false)
                                bflag[iBin]=true;
                            else
                                bCondition=false;

                            if(bCondition==true) //Condition 資料都正確才寫入
                            {
                                asCategory.sprintf("Category%d",iBin);
                                WriteIniData(sCheckListFilePath, asCategory, "Fail Percent Ignore", iFailCnt);                    //寫入 Check List

                                asBin.sprintf("IsPassBin%d",iBin);                                                                //寫入 Check List Pass/Fail
                                WriteIniData(sCheckListFilePath, "Tester_Control", asBin, asPassFail);   //P:Pass , F:Fail

                                Prod.iPersentIgnore[iBin]=iFailCnt;//先放到 Prod 等等再一起寫入 速度筆調快
                            }
                            else
                            {
                                asRet="NG";
                            }

                        }
                        else
                        {
                            asRet="NG";
                        }
                    }
                }
                else
                {
                    asRet="NG";
                }
            }
        }
        else
        {
            asRet="NG";
        }

        if(asRet=="OK")
        {
            for(i=0; i<TEST_MAX_BIN; i++)
            {
                BinSelect[iTestRunMode].iPersentIgnore[i]=Prod.iPersentIgnore[i];
            }
            bSetBINCOUNT=true;
            fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常 Mark
            fBinSel->spbSaveClick(this);                                                 //寫入 FT SetupFile
            bSetBINCOUNT=false;

            WriteIniData(sCheckListFilePath, "Tester_Control", "BINCOUNT", asCmd);
        }
        else
        {
            WriteIniData(sCheckListFilePath, "Tester_Control", "BINCOUNT", "");     //有問題要把資料清空
        }
        SendMSG_CMD(MSG_CMD_SetBINCOUNT, asRet);
    }
    tBinCount->Clear();
    tCondition->Clear();
    delete tBinCount;
    delete tCondition;
}
//---------------------------------------------------------------------------
void TfMain::SetSGOSBIN()                                                       //Sam 20210406 : Add GPIB SGOSBIN_
{
    AnsiString asCmd,asRet;
    char str[256];
    AnsiString sPath,sCheckListFilePath,asLastFileName;
    int iBin;
    bool bStatusOK=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        asRet.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetSGOSBIN, asRet);
    }
    else
    {
        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();
        iBin=atoi(asCmd.c_str());

        asRet="OK";
        if(asCmd=="NULL")
        {

        }
        else if(iBin<=0)
        {
             asRet="NG";
        }

        if(asRet=="OK")
        {
            WriteIniData(sCheckListFilePath, "Tester_Control", "SGOSBIN", asCmd);
        }
        else
        {
            WriteIniData(sCheckListFilePath, "Tester_Control", "SGOSBIN", "");//有問題要把資料清空
        }

        fLotInfo->RefreshYieldMonitor();

        SendMSG_CMD(MSG_CMD_SetSGOSBIN, asRet);
    }
}
//---------------------------------------------------------------------------
void TfMain::SetSGCONTFAIL()                                                    //Sam 20210422 : Add GPIB SGCONTFAIL_
{
    AnsiString asCmd,asRet;
    char str[256];
    AnsiString sPath,sCheckListFilePath,asLastFileName;
    int iFailCount;
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    int iOn=0;

    bool bStatusOK=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        asRet.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SetSGCONTFAIL, asRet);
    }
    else
    {
        szDir.sprintf("%s%s\\Tester.Data", DataPath, S);
        sPath.sprintf("D:\\HT9045_Log\\CheckingList");
        MyForceDirectories(sPath);
        asLastFileName=GetLastOpenFN();
        sCheckListFilePath.sprintf("%s\\%s.txt",sPath, asLastFileName);

        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();

        iFailCount=atoi(asCmd.c_str());

        asRet="OK";
        if(asCmd=="NULL")
        {

        }
        else if(iFailCount<=0)
        {
            asRet="NG";
        }

        if(asRet=="OK")
        {
            if(iTestRunMode==FT)
            {
                if(iFailCount>0 || asCmd!="NULL")
                {
                    iOn=1;
                    TestIF_File.bContsFailBySocket=true;
                    TestIF_File.bContsFailByHead=true;
                    TestIF_File.iContsFailHeadAlarmCT=iFailCount;
                    TestIF_File.iContsFailSocketAlarmCT=iFailCount;
                    WriteIniData(szDir, "Alarm", "HeadCT",   iFailCount);
                    WriteIniData(szDir, "Alarm", "SocketCT", iFailCount);
                    fYieldMonitoring->edContsFailSocketAlarmCT_FT->Text=iFailCount;
                    fYieldMonitoring->edContsFailHeadAlarmCT_FT->Text=iFailCount;
                }
                else
                {
                    iOn=0;
                    TestIF_File.bContsFailBySocket=false;
                    TestIF_File.bContsFailByHead=false;
                }
                WriteIniData(szDir, "Alarm", "SocketEnable", iOn);
                WriteIniData(szDir, "Alarm", "HeadEnable",   iOn);
                fYieldMonitoring->rbContsFailBySocket_FTOn->Checked=TestIF_File.bContsFailBySocket;
                fYieldMonitoring->rbContsFailBySocket_FTOff->Checked=!TestIF_File.bContsFailBySocket;
                fYieldMonitoring->rbContsFailByHead_FTOn->Checked=TestIF_File.bContsFailByHead;
                fYieldMonitoring->rbContsFailByHead_FTOff->Checked=TestIF_File.bContsFailByHead;
            }
            else
            {
                if(iFailCount>0 || asCmd!="NULL")
                {
                    iOn=1;
                    TestIF_File.bContsFailBySocket_RT=true;
                    TestIF_File.bContsFailByHead_RT=true;
                    TestIF_File.iContsFailHeadAlarmCT_RT=iFailCount;
                    TestIF_File.iContsFailSocketAlarmCT_RT=iFailCount;
                    WriteIniData(szDir, "Alarm", "HeadCT RT",   iFailCount);
                    WriteIniData(szDir, "Alarm", "SocketCT RT", iFailCount);
                    fYieldMonitoring->edContsFailSocketAlarmCT_RT->Text=iFailCount;
                    fYieldMonitoring->edContsFailHeadAlarmCT_RT->Text=iFailCount;
                }
                else
                {
                    iOn=0;
                    TestIF_File.bContsFailBySocket_RT=false;
                    TestIF_File.bContsFailByHead_RT=false;
                }
                WriteIniData(szDir, "Alarm", "SocketEnable RT", iOn);
                WriteIniData(szDir, "Alarm", "HeadEnable RT",   iOn);
                fYieldMonitoring->rbContsFailBySocket_RTOn->Checked=TestIF_File.bContsFailBySocket_RT;
                fYieldMonitoring->rbContsFailBySocket_RTOff->Checked=!TestIF_File.bContsFailBySocket_RT;
                fYieldMonitoring->rbContsFailByHead_RTOn->Checked=TestIF_File.bContsFailByHead_RT;
                fYieldMonitoring->rbContsFailByHead_RTOff->Checked=TestIF_File.bContsFailByHead_RT;
            }
            WriteIniData(sCheckListFilePath, "Alarm", "Socket", iFailCount);
            WriteIniData(sCheckListFilePath, "Alarm", "Head",   iFailCount);

            fLotInfo->RefreshYieldMonitor();
        }

        SendMSG_CMD(MSG_CMD_SetSGCONTFAIL, asRet);
    }
}
//---------------------------------------------------------------------------
void TfMain::SetTesterID()                                                      //Sam 20210617 : Add GPIB SETTESTERID
{
    AnsiString asRet,asCmd;
    char str[256];
    AnsiString sPath;
    bool bStatusOK=false;

    if(SystemStart==false &&                                                    //Sam 20210512 : GIPB 設定要卡 Initail
       (LastSet.iRunStartMode==rsmInitialStart   ||
        LastSet.iRunStartMode==rsmCInitialRetest ||
        LastSet.iRunStartMode==rsmQAMode))                                      //Sam 20250214 : RMS 新增 QA 模式
    {
        bStatusOK=true;
    }

    if(bStatusOK==false)
    {
        asRet.sprintf("STATUS NG");
        SendMSG_CMD(MSG_CMD_SETTESTERID, asRet);
    }
    else
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();
        sPath=AuthPath+"config.ini";

        if(asCmd!="")
        {
            WriteIniData(sPath, "bUseAlarmLogXml", "sN27_TesterID", asCmd);
            fConfiguration->edN27_6->Text=asCmd;
            IniConfig.sN27_TesterID=asCmd;
            asRet.sprintf("OK");
        }
        else
        {
            asRet.sprintf("NG");
        }
        SendMSG_CMD(MSG_CMD_SETTESTERID, asRet);
    }
}
//---------------------------------------------------------------------------
void TfMain::GetTesterID()                    //Sam 20210617 : Add GPIB SETTESTERID
{
    AnsiString asRet;
    asRet=IniConfig.sN27_TesterID+"\r\n";//Sam 20210310 : SetupFile 矽格卓邵要求增加換行字元
    SendMSG_CMD(MSG_CMD_GETTESTERID, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetAutoClean()                                                     //Sam 20220408 : Novatek 新增 AUTOCLEAN?
{
    AnsiString asRet="";
    asRet=AutoCleanStrings();
    SendMSG_CMD(MSG_CMD_GetAutClean , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::AutoCleanStrings()                                           //Sam 20220408 : Novatek 新增 AUTOCLEAN?
{
    AnsiString asRet="";
    asRet.sprintf("%d,%d,%d,%2.1f,%d",
                    TestIF.iAutoClean_Function,
                    TestIF.iAutoClean_IntervalContact,
                    TestIF_File.iAutoClean_ContactMode,
                    (double)TestIF.iAutoClean_ContactTime/10.0,
                    TestIF.iAutoClean_ContactCount);
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetForcePerPinN()                                                  //Sam 20220408 : Novatek 新增 DEVICEFORCEPERPIN?
{
    AnsiString asRet="";
    asRet=ForcePerPinNStrings();
    SendMSG_CMD(MSG_CMD_ForcePerPinN , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::ForcePerPinNStrings()                                        //Sam 20220408 : Novatek 新增 DEVICEFORCEPERPIN?
{
    AnsiString asRet="";
    asRet.sprintf("%0.4f", DeviceForm.ForcePerPinN);                            //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetContactHeight()                                                 //Sam 20220408 : Novatek 新增 ARMCONTACTHIGHVALUE?
{
    AnsiString asRet="";
    asRet=ContactHeightStrings();
    SendMSG_CMD(MSG_CMD_ContactHeight , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::ContactHeightStrings()                                       //Sam 20220408 : Novatek 新增 ARMCONTACTHIGHVALUE?
{
    AnsiString asRet="";
    asRet.sprintf("%0.2f,%0.2f", DeviceForm.IndexContact[0]/100.0, DeviceForm.IndexContact[1]/100.0);
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetYieldContinusFail()                                             //Sam 20220408 : Novatek 新增 YIELDCONTINUESFAIL?
{
    AnsiString asRet="";
    asRet=YieldContinusFailStrings();
    SendMSG_CMD(MSG_CMD_YieldContinusFail , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::YieldContinusFailStrings()                                   //Sam 20220408 : Novatek 新增 YIELDCONTINUESFAIL?
{
    AnsiString asRet="";
    //int iOnOff=0;
    //iOnOff=(Prod.bContsFailBySocket)?1:0;
    //asRet.sprintf("%d,%d",iOnOff, Prod.iContsFailSocketAlarmCT);
    asRet.sprintf("%d", Prod.iContsFailSocketAlarmCT);                          //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetYieldSiteCompare()                                              //Sam 20220408 : Novatek 新增 YIELDSITEUNBALANCE?
{
    AnsiString asRet="";
    asRet=YieldSiteCompareStrings();
    SendMSG_CMD(MSG_CMD_YieldSiteCompare , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::YieldSiteCompareStrings()                                    //Sam 20220408 : Novatek 新增 YIELDSITEUNBALANCE?
{
    AnsiString asRet="";
    //int iOnOff=0;
    //iOnOff=(Prod.bFailAlarmSiteYieldCmp)?1:0;
    //asRet.sprintf("%d,%2.1f,%d",iOnOff, Prod.dFailAlarmSiteYieldCmp, Prod.iFailAlarmSiteYieldCmpCount );
    asRet.sprintf("%2.1f", Prod.dFailAlarmSiteYieldCmp );                       //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetDUTStaus()                                                      //Sam 20220408 : Novatek 新增 DUTSTATUS?
{
    AnsiString asRet="";
    asRet=DUTStausStrings();
    SendMSG_CMD(MSG_CMD_DUTStatus, asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::DUTStausStrings()                                            //Sam 20220408 : Novatek 新增 DUTSTATUS?
{
    AnsiString asRet="";
    int iDut[32];

    for(int i=0; i<32; i++)
        iDut[i]=0;

    //Sam 20231205 : 修正 NOVATEK 指令
    //==>
    if(TestIF_File.iTestMode==SingleSite)                                       // Single Site 1x1
        asRet="SINGLE1X1";
    else if(TestIF_File.iTestMode==DualSite)                                    // Dual Site 1x2
        asRet="DUAL1X2";
    else if(TestIF_File.iTestMode==TriSite1X3)                                  //Frank 20160329 add for 1x3_4
        asRet="TRI1X3";
    else if(TestIF_File.iTestMode==QualSite1X4)                                 // Qual Site 1x4
        asRet="QUAD1X4";
    else if(TestIF_File.iTestMode==DualSite2x1)                                 // Dual Site 2x1
        asRet="DUAL2X1";
    else if(TestIF_File.iTestMode==QualSite2X2)                                 // Qual Site 2x2
        asRet="QUAD2X2";
    else if(TestIF_File.iTestMode==QualSite2X2N)                                // Qual Site 2x2 OK
        asRet=(CUSTOMER_CODE==CC_SCK)?"QUAD2X2":"QUAD2X2N";
    else if(TestIF_File.iTestMode==_6Site2X3)                                   //ChungHung 20140115 add for 2x3_6
        asRet="6SITE2X3";
    else if(TestIF_File.iTestMode==_6Site2X3N)                                  //Steven 20220425 : 2X3NN Mode
        asRet=(CUSTOMER_CODE==CC_SCK)?"6SITE2X3":"6SITE2X3N";
    else if(TestIF_File.iTestMode==_8Site2X4)                                   // 8 Site 2x4
        asRet="8SITE2X4";
    else if(TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
        asRet=(CUSTOMER_CODE==CC_SCK)?"8SITE2X4":"8SITE2X4N";
    else if(TestIF_File.iTestMode==_10Site2X5)                                  //wei 20190614 10 site
        asRet="10SITE2X5";
    else if(TestIF_File.iTestMode==_12Site2X6)
        asRet="12SITE2X6";
    else if(TestIF_File.iTestMode==_16Site2X8)                                  //16Site 2x8
        asRet="16SITE2X8";
    else if(TestIF_File.iTestMode==_16Site4X4)
        asRet="16SITE4X4";
    else if(TestIF_File.iTestMode==_32Site4X8N)
        asRet="32SITE4X8";
    else
        asRet="UNKNOWN";
    asRet+="_";
    //<==
    //Sam 20231205 : 修正 NOVATEK 指令
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF.iSiteMap[i][j]>0 && TestIF.iSiteMap[i][j]<=32)            //SiteMap：Use=1, not use=0。
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
                    iDut[TestIF.iSiteMap[i][j]-1]=(bTestSiteUse[1][i][j])?1:0;
                else
                    iDut[TestIF.iSiteMap[i][j]-1]=(bTestSiteUse[0][i][j])?1:0;
            }
        }
    }

    for(int i=0; i<TestSocket.iShtCnt; i++)
        asRet+=IntToStr(iDut[i])+",";

    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetUPH()                                                           //Sam 20220408 : Novatek 新增 UPH?
{
    AnsiString asRet="";
    asRet=UPHStrings();
    SendMSG_CMD(MSG_CMD_UPH , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::UPHStrings()                                                 //Sam 20220408 : Novatek 新增 UPH?
{
    AnsiString asRet="";
    if(fShowBinSelect->UPH_StringGrid->Cells[3][1]=="")                         //Sam 20231205 : 修正 NOVATEK 指令
    {
        return "0";
    }
    else
    {
        asRet.sprintf("%s",fShowBinSelect->UPH_StringGrid->Cells[3][1]);
        return asRet;
    }
}
//---------------------------------------------------------------------------
void TfMain::GetIndexCycleTime()                                                //Sam 20220408 : Novatek 新增 INDEXCYCLETIME?
{
    AnsiString asRet="";
    asRet=IndexCycleTimeStrings();
    SendMSG_CMD(MSG_CMD_IndexCycleTime , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::IndexCycleTimeStrings()                                      //Sam 20220408 : Novatek 新增 INDEXCYCLETIME?
{
    AnsiString asRet="";
    if(fObserver->TimeInfoGrid->Cells[4][11]=="")                               //Sam 20231205 : 修正 NOVATEK 指令
    {
        return "0";
    }
    else
    {
        asRet.sprintf("%s",fObserver->TimeInfoGrid->Cells[4][11]);
        return asRet;
    }
}
//---------------------------------------------------------------------------
void TfMain::GetTempOfs()                                                       //Sam 20220408 : Novatek 新增 GETTEMPOFFSET?
{
    AnsiString asRet="";
    asRet=TempOfsStrings();
    SendMSG_CMD(MSG_CMD_TempOfs , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::TempOfsStrings()                                             //Sam 20220408 : Novatek 新增 GETTEMPOFFSET?
{
    AnsiString asRet="", asTempOfs="";

    for(int i=0; i<tcTotalCount; i++)
    {
        if((i>=tcAa1 && i<=tcBd2) ||
           (i>=tcAe1 && i<=tcBh2))
        {
            if(bUT150Install[i])
            {
                asTempOfs.sprintf("%2.1f", Temperature.fTempOffSet[UserOffSet][i]);
                asRet+=asTempOfs+",";
            }
        }
    }
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetTempRange()                                                     //Sam 20220408 : Novatek 新增 GETTEMPERATURETOLERANCE?
{
    AnsiString asRet="";
    asRet=TempRangeStrings();
    SendMSG_CMD(MSG_CMD_TempRange , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::TempRangeStrings()                                           //Sam 20220408 : Novatek 新增 GETTEMPERATURETOLERANCE?
{
    AnsiString asRet="";
    //asRet.sprintf("%d,%d",IniConfig.iL04TemptureRange, IniConfig.iL05ChamberTemptureRange);
    asRet.sprintf("%d",IniConfig.iL04TemptureRange);                            //Sam 20231205 : 修正 NOVATEK 指令
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetVacuumAir()                                                     //Sam 20220408 : Novatek 新增 VACUUMAIR?
{
    AnsiString asRet="";
    asRet=VacuumAirStrings();
    SendMSG_CMD(MSG_CMD_VACUUMAIR , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::VacuumAirStrings()                                           //Sam 20220408 : Novatek 新增 VACUUMAIR?
{
    AnsiString asRet="";
    int iVacuumAir=0;
    iVacuumAir=(Sen[SnAirIsEnough].IsOn())?1:0;
    asRet.sprintf("%d",iVacuumAir);
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::GetAll()                                                           //Sam 20220408 : Novatek 新增 SET_ALL?
{
    AnsiString asRet="";
    asRet.sprintf("%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s",
    AutoCleanStrings(),
    ForcePerPinNStrings(),
    ContactHeightStrings(),
    YieldContinusFailStrings(),
    YieldSiteCompareStrings(),
    DUTStausStrings(),
    UPHStrings(),
    IndexCycleTimeStrings(),
    TempDataStrings(),
    TempOfsStrings(),
    TempRangeStrings(),
    ArmStatusStrings(),
    PERSITETemperatureStrings(),
    VacuumAirStrings(),
    HandlerVersionStrings());
    SendMSG_CMD(MSG_CMD_Get_All , asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetHandlerVersion()                                                //Sam 20220408 : Novatek 新增 HANDLERVERSION?
{
    AnsiString asRet="";
    asRet=HandlerVersionStrings();
    SendMSG_CMD(MSG_CMD_HandlerVersion , asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::HandlerVersionStrings()                                      //Sam 20220408 : Novatek 新增 HANDLERVERSION?
{
    AnsiString asRet="";
    asRet.sprintf("%s",asHandlerVersion);
    return asRet;
}
//---------------------------------------------------------------------------
void TfMain::UploadProdLog()                                                    //JerryYang 20220923 : [N17] upload lot summary
{
    GetYesterdayInfo();
    AnsiString  asPath, strPath, asFileName;
    bool bResult=false;

    asPath.sprintf("%s\\%04d%02d\\", asTravelingLogPath, SystemYearYesterday, SystemMonthYesterday);
    if(IniConfig.bSPILFunction==true)                                           //Steven 20250812 : Change file name for QLE
    {
        if(CUSTOMER_CODE==CC_SPIL_CHINA_SUZHOU && SPIL_FOR_QLE==1)
        {
            asFileName.sprintf("%s_%s_%04d%02d%02d_ProductionLog.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);     //Steven 20170123 (Jou) : 修改檔案命名格式
        }
        else
        {
            asFileName.sprintf("%s_%04d%02d%02d.csv", PC_NAME, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);     //Steven 20170123 (Jou) : 修改檔案命名格式
        }
    }
    else
    {
        asFileName.sprintf("%s_%s_%04d%02d%02d_ProductionLog.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);     //Steven 20170123 (Jou) : 修改檔案命名格式
    }
    asPath+=asFileName;
    if(IniConfig.bN17UploadProdLog)
    {
        if(DirectoryExists(IniConfig.asN17ProductionLogPath))
        {
            if(FileExists(asPath))
            {
                strPath.sprintf("%s\\%s", IniConfig.asN17ProductionLogPath, asFileName);
                bResult=CopyFile(asPath.c_str(), strPath.c_str(), true);
                if(bResult==false)
                {
                    ShowMyMessage("Uploaded Production log error\r\nPlease check the path of N-17.","上傳Production log失敗\r\n請檢查N-17路徑是否存在");
                }
                else
                {
                    RecordProcess("Uploaded Production log successfully.");   //Steven 20190722 : add TSV log
                }
            }
        }
        else
        {
            ShowMyMessage("Uploaded Production log error\r\nPlease check the path of N-17.","上傳Production log失敗\r\n請檢查N-17路徑是否存在");
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::GetShuttleMode()                                                   //Sam 20230130 : Add GPIB GETSHUTTLEMODE?
{
    AnsiString asRet="";
    if(TestIF_File.iShuttleMode==0)
    {
        asRet="0";                                                              //Normal
    }
    else
    {
        if(TestIF_File.iShuttle_Sel==0)
            asRet="1";                                                          //UseShuttle1
        else
            asRet="2";                                                          //UseShuttle2
    }
    SendMSG_CMD(MSG_CMD_GETSHUTTLEMODE, asRet);
}
//---------------------------------------------------------------------------
void TfMain::SetMaxTest()                                                       //Sam 20230201 : Add GPIB SETMAXTEST_
{
    AnsiString asRet="", asCmd="", szDir="";
    char str[256];
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
    TestIF_File.iMaxTime=atof(asCmd.c_str());
    FTestIF->edMaxTestTime->Text=TestIF_File.iMaxTime;
    WriteIniData(szDir, "Time", "MAX Time", TestIF_File.iMaxTime);
    asRet="OK";
    SendMSG_CMD(MSG_CMD_SETMAXTEST, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetMaxTest()                                                       //Sam 20230201 : Add GPIB GETMAXTEST
{
    AnsiString asRet="";
    asRet.sprintf("%0.2f", TestIF_File.iMaxTime);
    SendMSG_CMD(MSG_CMD_GETMAXTEST, asRet);
}
//---------------------------------------------------------------------------
void TfMain::SetMaxInitialTest()                                                //Sam 20230201 : Add GPIB SETINITIALMAXTEST_
{
    AnsiString asRet="", asCmd="", szDir="";
    char str[256];
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
    TestIF_File.iInitialMaxTime=atof(asCmd.c_str());
    FTestIF->edInitialMaxTest->Text=TestIF_File.iInitialMaxTime;
    WriteIniData(szDir, "Time", "Initial MAX Time", TestIF_File.iInitialMaxTime);
    asRet="OK";
    SendMSG_CMD(MSG_CMD_SETINITIALMAXTEST, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetMaxInitialTest()                                                //Sam 20230201 : Add GPIB GETINITIALMAXTEST
{
    AnsiString asRet="";
    asRet.sprintf("%0.2f", TestIF_File.iInitialMaxTime);
    SendMSG_CMD(MSG_CMD_GETINITIALMAXTEST, asRet);
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetSiteState()                                               //Sam 20230511 : 機台資料變更後須上傳 FTP
{
    AnsiString sMessage="";

    int iSiteState[32];
    for(int i=0; i<32; i++)
        iSiteState[i]=-1;

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF_File.iSiteMap[i][j]>0 && TestIF_File.iSiteMap[i][j]<=32)
            {
                if(bTestSiteUse[0][i][j] || bTestSiteUse[1][i][j])              //Sam 20231227 : 修正關 Arm 的時候資料錯誤
                    iSiteState[TestIF_File.iSiteMap[i][j]-1]=1;
                else
                    iSiteState[TestIF_File.iSiteMap[i][j]-1]=0;
            }
        }
    }

    for(int i=0; i<TestSocket.iShtCnt; i++)
    {
        sMessage+=IntToStr(iSiteState[i])+",";
    }

    return sMessage;
}
//---------------------------------------------------------------------------
//-------------------------------TCP/IP Command -----------------------------
//---------------------------------------------------------------------------
void __fastcall TfMain::TCPCommandServerClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    bHandlerResultConnect=true;
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Connect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::TCPCommandServerClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Disconnect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::TeraTCPResultServerClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    bHandlerResultConnect=true;
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Connect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::TeraTCPResultServerClientDisconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    bHandlerResultConnect=false;
    //Sam 20230417 : Log 新增 SocketHandle 資料
    AnsiString s="";
    s.sprintf("[%4d][%4d] Client Disconnect",Socket->SocketHandle, Socket->LocalPort);
    TCPIPCommunicationLog(s);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::TeraTCPResultServerClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    bHandlerResultConnect=false;
    if(ErrorCode==10053)                                                        // 網路線拔除
    {
        ErrorCode=0;
        TCPIPCommunicationLog("Connect fail : 網路線拔除!");
    }
    else if(ErrorCode==10061)                                                   //連線失敗
    {
        ErrorCode=0;
        TCPIPCommunicationLog("Connect fail : 連線失敗!");
    }
    else
    {
        TCPIPCommunicationLog("Connect fail :" + AnsiString(ErrorCode));
        ErrorCode=0;
    }
    //TCPCommandServer->Close();

    try
    {
        TeraTCPResultServer->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "TeraTCPResultServerClient Error");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::TCPIPCommunicationLog(AnsiString Str)
{
    AnsiString Path, s="";
    GetTimeInfo();
    s.sprintf("%s %02d:%02d:%02d.%03d %s",Now().FormatString("yyyy/mm/dd"), SystemHour, SystemMin, SystemSec, SystemMSec,Str);   //Sam 20230420 : 時間紀錄到小數點
    fMain->Memo2->Lines->Add(s);

    Path.sprintf("%s\\%04d_%02d_%02d", asTCPIPPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);
    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asTCPIPPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str(), s);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::HanderTcpIp()
{
    if(TCPCommandServer->Active==true)
        TCPCommandServer->Active==false;

    AnsiString s="";
    iHandlerCommandServerPort=7016;
    iHandlerResultServerPort=7017;
    try
    {
        if(CosFunction.bEnableHandlerResultServer==true)
        {
            fMain->TCPCommandServer->Close();
            fMain->TCPCommandServer->Port=iHandlerCommandServerPort;
            fMain->TCPCommandServer->Open();
            //Sam 20230417 : Log 新增 SocketHandle 資料
            s.sprintf("[%4d] Server Listen",iHandlerCommandServerPort);
            TCPIPCommunicationLog(s);
            fMain->TeraTCPResultServer->Close();
            fMain->TeraTCPResultServer->Port=iHandlerResultServerPort;
            fMain->TeraTCPResultServer->Open();
            //Sam 20230417 : Log 新增 SocketHandle 資料
            s.sprintf("[%4d] Server Listen",iHandlerResultServerPort);
            TCPIPCommunicationLog(s);
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfMain::HanderTcpIp");
        ShowMyMessage("Socket Server Open Error!!");
        TCPIPCommunicationLog("Socket Server Open Error!!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::HandlerTCPIPResultSendProcess(String sMessage)
{
    int iHandle=0,iErr=0,iHandleSendDone[100];
    int iPort=fMain->TCPCommandServer->Socket->LocalPort;
    AnsiString s="";
    char cBuffer[500];          //Sam 20201021 : for contact count 資料需要再加長 100//Sam 20191120 Fix Issue
    AnsiString Path;
    strcpy(cBuffer, sMessage.c_str());
    bool bNeedSend=false;
    ZeroMemory(iHandleSendDone, sizeof(iHandleSendDone));

    ReSendData:
    int iCount=fMain->TCPCommandServer->Socket->ActiveConnections;
    if(iCount>=1)   //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        for(int i=0; i<iCount; i++)
        {
            try
            {
                iHandle=fMain->TCPCommandServer->Socket->Connections[i]->SocketHandle;  //Sam 20230417 : Log 新增 SocketHandle 資料

                bNeedSend=true;      //Sam 20230509 : 送指令時發生例外事件需要重新送指令
                for(int j=0;j<100;j++)
                {
                    if(iHandleSendDone[j]==iHandle)
                    {
                        bNeedSend=false;//有傳送過的 Handle 就不要再送了
                        break;
                    }
                }

                if(bNeedSend)
                {
                    s.sprintf("[%4d][%4d][ #Send#     ] %s",iHandle, iPort, sMessage);
                    TCPIPCommunicationLog(s);
                    fMain->TCPCommandServer->Socket->Connections[i]->SendBuf(cBuffer, sMessage.Length());

                    for(int j=0;j<100;j++)   //Sam 20230509 : 送指令時發生例外事件需要重新送指令
                    {
                        if(iHandleSendDone[j]==0)
                        {
                            iHandleSendDone[j]=iHandle;//紀錄傳送過的 Handle
                            break;
                        }
                    }

                }
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfMain::HandlerTCPIPResultSendProcess");
                s.sprintf("[%4d][%4d][ #Exception#] %s",iHandle, iPort, sMessage);
                TCPIPCommunicationLog(s);

                iErr++;     //Sam 20230509 : 送指令時發生例外事件需要重新送指令
                if(iErr<3)
                    goto ReSendData;
                else
                    ShowMyMessage("Handler Result Server Socket Error!!");
            }
        }
    }
    else    //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        s.sprintf("[%04d][%4d][ #SendFail# ] %s",iHandle, iPort, sMessage);
        TCPIPCommunicationLog(s);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::HandlerTeraTResultSendProcess(String sMessage)
{
    int iHandle=0;
    int iPort=fMain->TeraTCPResultServer->Socket->LocalPort;
    AnsiString s="";
    char cBuffer[500];          //Sam 20201021 : for contact count 資料需要再加長 100//Sam 20191120 Fix Issue
    AnsiString Path;
    strcpy(cBuffer, sMessage.c_str());

    int iCount=fMain->TeraTCPResultServer->Socket->ActiveConnections;
    if(iCount>=1)   //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        for(int i=0; i<iCount; i++)
        {
            try
            {
                iHandle=fMain->TeraTCPResultServer->Socket->Connections[i]->SocketHandle;  //Sam 20230417 : Log 新增 SocketHandle 資料
                s.sprintf("[%4d][%4d][ #Send#     ] %s",iHandle, iPort, sMessage);
                TCPIPCommunicationLog(s);
                fMain->TeraTCPResultServer->Socket->Connections[i]->SendBuf(cBuffer, sMessage.Length());
            }
            catch(...)
            {
                MyDBIProcess("Exception", "TfMain::HandlerTeraTResultSendProcess");
                ShowMyMessage("Handler Result Server Socket Error!!");
                s.sprintf("[%4d][%4d][ #Exception#] %s",iHandle, iPort, sMessage);
                TCPIPCommunicationLog(s);
            }
        }
    }
    else    //Sam 20230417 : Log 新增 SocketHandle 資料
    {
        s.sprintf("[%04d][%4d][ #SendFail# ] %s",iHandle, iPort, sMessage);
        TCPIPCommunicationLog(s);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::TCPCommandServerClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int iPoint=0;
    int iData1=0, iData2=0;
    int iBufferLenght=Socket->ReceiveLength();
    char EthernetBuffer[100];
    double dData1=0.0;
    AnsiString sTemp="", sData[4], sSendMes="", sMemoMes="";
    AnsiString sData1="", sData2="", sData3="", sData4="", szDir="";;

    for(int i=0; i<100; i++)
        EthernetBuffer[i]=NULL;

    if(iBufferLenght>0)
    {
        Socket->ReceiveBuf(EthernetBuffer, iBufferLenght);
        sTemp=AnsiString(EthernetBuffer);
        sMemoMes.sprintf("[%4d][%4d][ #Receive#  ] %s", Socket->SocketHandle, Socket->LocalPort, sTemp);    //Sam 20230417 : Log 新增 SocketHandle 資料
        TCPIPCommunicationLog(sMemoMes);
        iPoint=sTemp.Pos(",");
        if(iPoint>0)
        {
            sData[0]=sTemp.SubString(1, iPoint-1);
            sTemp=sTemp.SubString(iPoint+1, sTemp.Length());
            iPoint=sTemp.Pos(",");
            if(iPoint>0)
            {
                sData[1]=sTemp.SubString(1, iPoint-1);
                sTemp=sTemp.SubString(iPoint+1, sTemp.Length());
                iPoint=sTemp.Pos(",");
                if(iPoint>0)
                {
                    sData[2]=sTemp.SubString(1,iPoint-1);
                    sTemp=sTemp.SubString(iPoint+1, sTemp.Length());
                    iPoint=sTemp.Pos(",");
                    if(iPoint>0)
                    {
                        sData[3]=sTemp.SubString(1,iPoint-1);
                    }
                }
            }
        }
    }
    if(sData[0]=="HTGR" && sData[1]=="101")                                     //Input Loader Qty
    {
        sSendMes.sprintf("HTSR,101,%d,",LastSet.SendCT[0]);
    }//ex. HTSR,101,13546,
    else if(sData[0]=="HTGR" && sData[1]=="102")                                //Pass Qty
    {
        sSendMes.sprintf("HTSR,102,%d,",iSECSGEMPass);
    }//ex. HTSR,102,10849,
    else if(sData[0]=="HTGR" && sData[1]=="103")                                //Fail Qty
    {
        sSendMes.sprintf("HTSR,103,%d,",iSECSGEMFail);
    }//ex. HTSR,103,0,
    else if(sData[0]=="HTGR" && sData[1]=="104" )                               //Open Bin Qty
    {
        iData1=BinSelect[iTestRunMode].IfErrorT3;
        if(iData1>=0)
            iData2=LastSet.BinCT[0][iTo3Unload[iData1]];
        sSendMes.sprintf("HTSR,104,%d,",iData2);
    }//ex. HTSR,104,0,
    else if(sData[0]=="HTGR" && sData[1]=="105" )                               //Short Qty
    {
        iData1=BinSelect[iTestRunMode].IfErrorT3;
        if(iData1>=0)
            iData2=LastSet.BinCT[0][iTo3Unload[iData1]];
        sSendMes.sprintf("HTSR,105,%d,",iData2);
    }//ex. HTSR,105,0,
    else if(sData[0]=="HTGR" && sData[1]=="106")                                //Open Site Count
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
            for(int j=0; j< MAX_SOCKET_COL; j++)
                if(bTestSiteUse[0][i][j]==true)
                    iData1++;
        sSendMes.sprintf("HTSR,106,%d,",iData1);
    }//ex. HTSR,106,29,
    else if(sData[0]=="HTGR" && sData[1]=="107")                                //Average UPH
    {
        sSendMes.sprintf("HTSR,107,%d,",atoi(RunInfo.iAvgUPH.c_str()));
    }//ex. HTSR,107,9131,
    else if(sData[0]=="HTGR" && sData[1]=="108")                                //Auto1-3 Fix1-3 Qty & Auto1 Yield
    {
        for(int i=0; i<6; i++)
        {
            iData1+=LastSet.BinCT[0][iTo3Unload[i]];
            sData1+=IntToStr(LastSet.BinCT[0][iTo3Unload[i]])+",";
        }

        if(iData1>0)
            dData1=ChangeToFloat((double)(LastSet.BinCT[0][iTo3Unload[0]]), (double)iData1);    //Steven 20250820 : 針對除以0加上保護

        sSendMes.sprintf("HTSR,108,%s%0.1f,", sData1, dData1);
    }//ex. HTSR,108,10849,0,0,0,0,0,100.0,
    else if(sData[0]=="HTGR" && sData[1]=="109")                                //Tester Online/Offline
    {
        if(LastSet.iTester==OFF_LINE)
            sData1="Off Line";
        else
            sData1="On Line";
         sSendMes.sprintf("HTSR,109,%s,", sData1);
    }//ex. HTSR,109,Off Line,
    else if(sData[0]=="HTGR" && sData[1]=="201")                                //Lot ID
    {
        sSendMes.sprintf("HTSR,201,%s,", fLotInfo->edtSysLotID->Text.c_str());
    }//ex. HTSR,201,,
    else if(sData[0]=="HTGR" && sData[1]=="202")                                 //Operator ID
    {
        sSendMes.sprintf("HTSR,202,%s,", fLotInfo->edtSysOperatorID->Text.c_str());
    }//ex. HTSR,202,,
    else if(sData[0]=="HTGR" && sData[1]=="203")                                //Machine ID
    {
        sSendMes.sprintf("HTSR,203,%s,", IniConfig.sGPIBMachineID);
    }//ex. HTSR,203,PJLY1027,
    else if(sData[0]=="HTGR" && sData[1]=="204")                                //Initail Start 開始時間 & Initail Stat 開始後目前所生產的時間
    {
        TDateTime sj1;
        if(InitialStartTime!="")
        {
            sData1=InitialStartTime;
            sj1=StrToDateTime(InitialStartTime)-Now();
            sData2=sj1.FormatString("hh:nn:ss");
        }
        else
        {
            sData1=Now().FormatString("yyyy/mm/dd hh:nn:ss");
            sData2="00:00:00";
        }
        sSendMes.sprintf("HTSR,204,%s,%s,",sData1,sData2);
    }//ex. HTSR,204,2023/04/26 13:17:09,00:01:12,
    else if(sData[0]=="HTSET" && sData[1]=="205" )                              //Handler status (Running , halt ,Alarm ,Pause, Lock, etc..)
    {
        sSendMes.sprintf("HTSR,205,%s,",fMain->palMainStatus->Caption.c_str());
    }//ex. HTSR,205,HALT,
    else if(sData[0]=="HTGR" && sData[1]=="206")       //Site State             //Site State  SiteMap：Use=1, close=0, not use=-1。
    {
        int iCount=0, iMaxSite=0;

        if(MachineTypeChoice==Type_HT9045) //9045
            iMaxSite=8;
        else if(MachineTypeChoice==Type_HT9046_LS)
            iMaxSite=32;
        else if(MachineTypeChoice==Type_HT9045_12Site)
            iMaxSite=12;
        else
            iMaxSite=16;

        iCount=iMaxSite-TestSocket.iShtCnt;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)
                {
                    if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
                    {
                        if(bTestSiteUse[1][i][j])
                            sData1+="1,";
                        else
                            sData1+="0,";
                    }
                    else
                    {
                        if(bTestSiteUse[0][i][j])
                            sData1+="1,";
                        else
                            sData1+="0,";
                    }
                }
                else
                {
                    sData1+="-1,";
                }
            }
        }
        while(iCount>0)
        {
            sData1+="-1,";
            iCount--;
        }
        sSendMes.sprintf("HTSR,206,%s",sData1);
    }//ex. HTSR,206,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,
    else if(sData[0]=="HTGR" && sData[1]=="207")                                //Site Map
    {
        int iCount=0,iMaxSite=0;
        if(MachineTypeChoice==Type_HT9045) //9045
            iMaxSite=8;
        else if(MachineTypeChoice==Type_HT9046_LS)
            iMaxSite=32;
        else if(MachineTypeChoice==Type_HT9045_12Site)
            iMaxSite=12;
        else
            iMaxSite=16;

        iCount=iMaxSite-TestSocket.iShtCnt;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)
                    sData1+=IntToStr(int(TestIF_File.iSiteMap[i][j]))+",";
                else
                    sData1+="-1,";
            }
        }
        while(iCount>0)
        {
            sData1+="-1,";
            iCount--;
        }
        sSendMes.sprintf("HTSR,207,%s",sData1);
    }//ex. HTSR,207,1,5,9,13,17,21,25,29,2,6,10,14,18,22,26,30,3,7,11,15,19,23,27,31,4,8,12,16,20,24,28,32,
    else if(sData[0]=="HTGR" && sData[1]=="208")                                //Test Time
    {
        sSendMes.sprintf("HTSR,208,TestTime,%0.1f,",atof(fObserver->TimeInfoGrid->Cells[3][14].c_str()));

    }//ex. HTSR,208,TestTime,0.0,
    else if(sData[0]=="HTGR" && sData[1]=="209")                                //Index Time
    {
        sSendMes.sprintf("HTSR,209,IndexTime,%0.1f,",atof(fObserver->TimeInfoGrid->Cells[5][11].c_str()));
    }//ex. HTSR,209,IndexTime,0.0,
    else if(sData[0]=="HTGR" && sData[1]=="210")                                //Reply Normal or RT
    {
        if(eRunStartMode(LastSet.iRunStartMode)==0 || eRunStartMode(LastSet.iRunStartMode)==1 || eRunStartMode(LastSet.iRunStartMode)==8 || eRunStartMode(LastSet.iRunStartMode)==9) //FT->RT
            sData1="Normal";
        else if(eRunStartMode(LastSet.iRunStartMode)==2 || eRunStartMode(LastSet.iRunStartMode)==3 || eRunStartMode(LastSet.iRunStartMode)==10) //RT->FT
            sData1="RT";
        sSendMes.sprintf("HTSR,210,%s,",sData1);
    }//ex. HTSR,210,Normal,
    else if(sData[0]=="HTGR" && sData[1]=="211")                                //AirForce
    {
        sSendMes.sprintf("HTSR,211,%0.2f,",atof(fContact->edAirForce->Text.c_str()));
    }//ex. HTSR,211,163.84,
    else if(sData[0]=="HTGR" && sData[1]=="212")                                //Contact KG
    {
        sSendMes.sprintf("HTSR,212,%0.2f,",atof(fContact->edForcePerDeviceKG->Text.c_str()));
    }//ex. HTSR,212,5.12,
    else if(sData[0]=="HTGR" && sData[1]=="213")                                //Socket Time
    {
        if(LastSet.iTemperature==Tempture_Ambient)
            sData1="OFF";
        else
            sData1=edSoakTime->Text;
        sSendMes.sprintf("HTSR,213,%s,",sData1);
    }//ex. HTSR,213,OFF, or HTSR,213,5,
    else if(sData[0]=="HTGR" && sData[1]=="215")                                //Handler State
    {
        if(bIsAutoOneCycle)
        {
            sData1="WaitOneCycle";
        }
        else if(fMain->palMainStatus->Caption=="HALT" && SystemStart==false)
        {
            sData1="HALT";
        }
        else
        {
            if(fMain->palMainStatus->Caption=="HALT")
                sData1="PAUSE";
            else
                sData1=fMain->palMainStatus->Caption;
        }
        sSendMes.sprintf("HTSR,215,%s,",sData1);
    }//ex. HTSR,215,HALT,
    else if(sData[0]=="HTGR" && sData[1]=="250")                                 //Contact heigh
    {
        if(TestIF_File.iShuttleMode==0) //Double Suttle
        {
            sData1=AnsiString(DeviceForm_File.IndexContact[0]);
            sData2=AnsiString(DeviceForm_File.IndexContact[1]);
        }
        else    //Single Suttle
        {
            if(TestIF_File.iShuttle_Sel==0) //Only Suttle1
                sData1=AnsiString(DeviceForm_File.IndexContact[0]);
            else //Only Suttle2
                sData2=AnsiString(DeviceForm_File.IndexContact[1]);
        }
        sSendMes.sprintf("HTSR,250,%s,%s,",sData1,sData2);
    }//ex. HTSR,250,-117.88,-118.18,
    else if(sData[0]=="HTGR" && sData[1]=="251")                                //Qty by Socket
    {
        int iSocketQty[32];
        for(int i=0; i<32; i++)
            iSocketQty[i]=-1;

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)
                {
                    if(TestIF.iTestMode==_32Site4X8N)
                    {
                        iSocketQty[TestIF_File.iSiteMap[i][j]-1]=ArmData[2]->ArmSKET[i][j]->GetTotal();
                    }
                    else
                    {
                        iSocketQty[TestIF_File.iSiteMap[i][j]-1]=ArmData[0]->ArmSKET[i][j]->GetTotal()+ArmData[1]->ArmSKET[i][j]->GetTotal();
                    }
                }
            }
        }

        for(int i=0; i<TestSocket.iShtCnt; i++)
        {
            if(iSocketQty[i]!=-1)
            {
                sData1+="S"+AnsiString(i+1)+"="+IntToStr(iSocketQty[i])+",";
            }
        }
        sSendMes.sprintf("HTSR,251,%s",sData1);
    }//ex. HTSR,251,S1=3,S2=3,S3=3,S4=3,S5=3,S6=3,S7=3,S8=3,S9=3,S10=3,S11=3,S12=3,S13=3,S14=3,S15=3,S16=3,S17=3,S18=3,S19=0,S20=3,S21=3,S22=3,S23=3,S24=3,S25=3,S26=0,S27=3,S28=3,S29=3,S30=3,S31=3,S32=0,
    else if(sData[0]=="HTGR" && sData[1]=="252")                                //SetupFile
    {
        sSendMes.sprintf("HTSR,252,%s,",GetLastOpenFN());
    }//ex. HTSR,252,013-HTQFP64-10X10-000_125-8-E8-AA,
    else if(sData[0]=="HTGR" && sData[1]=="253")                                //Yield by Socket
    {
        double dSocketYield[32];
        for(int i=0; i<32; i++)
            dSocketYield[i]=-1.0;

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)
                    dSocketYield[TestIF_File.iSiteMap[i][j]-1]=ArmData[2]->ArmSKET[i][j]->GetPCA();
            }
        }
        
        for(int i=0; i<TestSocket.iShtCnt; i++)
        {
            if(dSocketYield[i]!=-1)
                sData1+="S"+AnsiString(i+1)+"="+AnsiString(FormatFloat("0.0",dSocketYield[i]))+",";
        }
        sSendMes.sprintf("HTSR,253,%s", sData1);
    }//ex. HTSR,253,S1=0.0,S2=0.0,S3=0.0,S4=0.0,S5=0.0,S6=0.0,S7=0.0,S8=0.0,S9=0.0,S10=0.0,S11=0.0,S12=0.0,S13=0.0,S14=0.0,S15=0.0,S16=0.0,S17=0.0,S18=0.0,S19=0.0,S20=0.0,S21=0.0,S22=0.0,S23=0.0,S24=0.0,S25=0.0,S26=0.0,S27=0.0,S28=0.0,S29=0.0,S30=0.0,S31=0.0,S32=0.0,
    else if(sData[0]=="HTGR" && sData[1]=="254")                                //ForcePerPinG
    {
        sSendMes.sprintf("HTSR,254,%0.4f,", DeviceForm_File.ForcePerPinG);
    }//ex. HTSR,254,40.0000,
    else if(sData[0]=="HTGR" && sData[1]=="255")                                //Bin Qty
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            if(BinSelect[iTestRunMode].iCatDataT3Pos[i]!=0)
            {
                if(BinSelect[iTestRunMode].bConsFail[i])
                    sData1="F";
                else
                    sData1="P";
                sData2.sprintf("%s-Bin%d=%d,",sData1,i,LastSet.iBinData32[2][i]); //Sam 20230913 : 0 > 2 修正回傳異常
                sData3+=sData2;
            }
        }
        sSendMes.sprintf("HTSR,255,%s",sData3);
    }//ex. HTSR,255,P-Bin1=144,F-Bin2=0,F-Bin3=0,F-Bin4=0,F-Bin5=0,
    else if(sData[0]=="HTGR" && sData[1]=="256")                                //Bin Yield
    {
        for(int i=eAuto1; i<=iAutoRight; i++)
            dData1+=LastSet.BinCT[0][iTo3Unload[i]];

        for(int i=0; i<iTestBinCount; i++)
        {
            if(BinSelect[iTestRunMode].iCatDataT3Pos[i]!=0)
            {
                if(BinSelect[iTestRunMode].bConsFail[i])
                    sData1="F";
                else
                    sData1="P";

                if(dData1>0)
                    sData2.sprintf("%0.2f",LastSet.iBinData32[2][i]*100.0/dData1);  //Sam 20230913 : 0 > 2 修正回傳異常
                else
                    sData2="0.00";

                sData3.sprintf("%s-Bin%d=%s,",sData1,i,sData2);
                sData4+=sData3;
            }
        }
        sSendMes.sprintf("HTSR,256,%s",sData4);

    }//ex. HTSR,256,P-Bin1=100.00,F-Bin2=0.00,F-Bin3=0.00,F-Bin4=0.00,F-Bin5=0.00,
    else if(sData[0]=="HTSET" && sData[1]=="301")                               //Change StupFile
    {
        if(SystemStart==false)
        {
            fFTPClient->bControlBySECSGEM=true;
            fFTPClient->aSetUpNameBySECSGEM=sData[2];
            fFTPClient->ShowFTPModal(0);
            bSecsGemDownloadFTP=true;
            sData1="OK";
        }
        else
        {
            sData1="BUSY";
        }
         sSendMes.sprintf("HTSR,301,%s,",sData1);
    }//ex. QQ
    else if(sData[0]=="HTSET" && sData[1]=="302")                               //Fail Bin setting DoubleContact On
    {
        /*  //Sam 20230711 : 修正 OLP SetCategroy 異常 Mark
        if(eRunStartMode(LastSet.iRunStartMode)==0 || eRunStartMode(LastSet.iRunStartMode)==1)          //FT
            fBinSel->PageControl1->ActivePageIndex=0;
        else if(eRunStartMode(LastSet.iRunStartMode)==2 || eRunStartMode(LastSet.iRunStartMode)==3)     //RT
            fBinSel->PageControl1->ActivePageIndex=1;
        */
        for(int i=0; i<16; i++)                    //Initial
            BinSelect[iTestRunMode].iDBContact[i]=0;

        for(int j=0; j<6 ;j++)
        {
            if(BinSelect[iTestRunMode].iStackDefFailCate[j]==1)
            {
                for(int i=0; i<16; i++)
                {
                    if(BinSelect[iTestRunMode].iCatDataT3Pos[i]==(j+1))
                        BinSelect[iTestRunMode].iDBContact[i]=1;
                }
            }
        }
        bTCPIPChangeAlarm=true;
        fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
        fBinSel->spbSaveClick(this);
        bTCPIPChangeAlarm=false;
        sSendMes.sprintf("HTSR,302,DoubleContact_On,");
    }//ex. HTSR,302,DoubleContact_On,
    else if(sData[0]=="HTSET" && sData[1]=="303")                               //All Bin setting DoubleContact Off
    {
        bDoubleContact=false;
        /*  //Sam 20230711 : 修正 OLP SetCategroy 異常 Mark
        if(eRunStartMode(LastSet.iRunStartMode)==0 || eRunStartMode(LastSet.iRunStartMode)==1)      //FT
            fBinSel->PageControl1->ActivePageIndex=0;
        else if(eRunStartMode(LastSet.iRunStartMode)==2 || eRunStartMode(LastSet.iRunStartMode)==3) //RT
            fBinSel->PageControl1->ActivePageIndex=1;
        */
        for(int i=0; i<16; i++)
            BinSelect[iTestRunMode].iDBContact[i]=0;
        bTCPIPChangeAlarm=true;
        fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
        fBinSel->spbSaveClick(this);
        bTCPIPChangeAlarm=false;
        sSendMes.sprintf("HTSR,303,DoubleContact_Off,");
    }//ex. HTSR,303,DoubleContact_Off,
    else if(sData[0]=="HTSET" && sData[1]=="306")                               //Continue fail turn on
    {
        fYieldMonitoring->rbContsFailBySocket_FTOn->Checked=true;
        fYieldMonitoring->rbContsFailByHead_FTOn->Checked=true;
        TestIF_File.bContsFailBySocket=true;
        TestIF_File.bContsFailByHead=true;
        fYieldMonitoring->btnApplyClick(fYieldMonitoring);
        sSendMes.sprintf("HTSR,306,ContinuefailON,");
    }//ex. HTSR,306,ContinuefailON,

    else if(sData[0]=="HTSET" && sData[1]=="307")                               //Continue fail  off
    {
        fYieldMonitoring->rbContsFailBySocket_FTOff->Checked=true;
        fYieldMonitoring->rbContsFailByHead_FTOff->Checked=true;
        TestIF_File.bContsFailBySocket=false;
        TestIF_File.bContsFailByHead=false;
        fYieldMonitoring->btnApplyClick(fYieldMonitoring);
        sSendMes.sprintf("HTSR,307,ContinuefailOFF,");
    }//ex. HTSR,307,ContinuefailOFF,
    else if(sData[0]=="HTSET" && sData[1]=="308")                                //Set Loader to Empty
    {
        fTrayAssignment->RGLoader->ItemIndex=0;
        TrayForm.LoaderToEmptyColor[FT]=fTrayAssignment->RGLoader->ItemIndex;
        fTrayAssignment->spbSaveClick(fTrayAssignment);
        sSendMes.sprintf("HTSR,308,Empty,");

    }//ex. HTSR,308,Empty,
    else if(sData[0]=="HTSET" && sData[1]=="309")                                //Ser Loader to Color
    {
        fTrayAssignment->rgLoad_RT->ItemIndex=1;
        TrayForm.LoaderToEmptyColor[RT]=fTrayAssignment->rgLoad_RT->ItemIndex;
        fTrayAssignment->spbSaveClick(fTrayAssignment);
        sSendMes.sprintf("HTSR,309,Color,");
    }//ex. HTSR,308,Empty,
    else if(sData[0]=="HTSET" && sData[1]=="310")                               //Level to Supervisor
    {
        fLotInfo->btUpload->Visible=false;  //Eliot 2011_0815  // 2011.10.20 Q_Q Transplant form Eliot
        AccessLevel=2;
        ChangeLevelAttr();
        cbUserSelect->Text="Supervisor";
        spbUserName->Caption="Supervisor";
        sSendMes.sprintf("HTSR,310,Supervisor,");

    }//ex. HTSR,310,Supervisor,
    else if(sData[0]=="HTSET" && sData[1]=="311")                               //Level to Operator
    {
        AccessLevel=0;
        ChangeLevelAttr();
        cbUserSelect->Text="Operator";
        spbUserName->Caption="Operator";
        sSendMes.sprintf("HTSR,311,Operator,");

    }//ex. HTSR,311,Operator,
    else if(sData[0]=="HTSET" && sData[1]=="312")                               //Low Yield Open
    {
        fYieldMonitoring->cbLowYield_FT->Checked=true;
        fYieldMonitoring->btnApplyClick(fYieldMonitoring);
        sSendMes.sprintf("HTSR,312,LowYieldOpen,");
    }//ex. HTSR,312,LowYieldOpen,
    else if(sData[0]=="HTSET" && sData[1]=="313")                               //Low Yield Close
    {
        fYieldMonitoring->cbLowYield_FT->Checked=false;
        fYieldMonitoring->btnApplyClick(fYieldMonitoring);
        sSendMes.sprintf("HTSR,313,LowYieldClose,");
    }//ex. HTSR,313,LowYieldClose,
    else if(sData[0]=="HTSET" && sData[1]=="314")                               //Config D22 On
    {
        szDir.sprintf("%sconfig.ini",AuthPath);
        fConfiguration->cbD22_1->Checked=true;
        IniConfig.bD22SupportMultiDoubleContact=true;
        IniConfig.bD22SupportMultiDoubleContact=ReadWriteIni(szDir, "Index", "bD22SupportMultiDoubleContact",IniConfig.bD22SupportMultiDoubleContact,false,false);
        sSendMes.sprintf("HTSR,314,DoubleContactD22Ture,");
    }//ex. HTSR,314,DoubleContactD22Ture,
    else if(sData[0]=="HTSET" && sData[1]=="315")                               //Config D22 Off
    {
        szDir.sprintf("%sconfig.ini",AuthPath);
        fConfiguration->cbD22_1->Checked=false;
        IniConfig.bD22SupportMultiDoubleContact=false;
        IniConfig.bD22SupportMultiDoubleContact=ReadWriteIni(szDir, "Index", "bD22SupportMultiDoubleContact",IniConfig.bD22SupportMultiDoubleContact,false,false);
        sSendMes.sprintf("HTSR,315,DoubleContactD22False,");
    }//ex. HTSR,315,DoubleContactD22False,
    else if(sData[0]=="HTSET" && sData[1]=="316")                               //Config D22 count change
    {
        szDir.sprintf("%sconfig.ini",AuthPath);
        fConfiguration->coD22->ItemIndex=atoi(sData[2].c_str())-2;
        IniConfig.iD22DoubleContactCount=atoi(sData[2].c_str())-2;
        IniConfig.iD22DoubleContactCount=ReadWriteIni(szDir, "Index", "iD22DoubleContactCount",IniConfig.iD22DoubleContactCount,0,false);
        sSendMes.sprintf("HTSR,316,SetOK,");

    }//ex. HTSR,316,SetOK,
    else if(sData[0]=="HTSET" && sData[1]=="317")                               //FT <> RT
    {
        if(eRunStartMode(LastSet.iRunStartMode)==0 || eRunStartMode(LastSet.iRunStartMode)==1)      //FT->RT
        {
            palRTClick(fMain);
            sData1="RT";
        }
        else if(eRunStartMode(LastSet.iRunStartMode)==2 || eRunStartMode(LastSet.iRunStartMode)==3) //RT->FT
        {
            palFTClick(fMain);
            sData1="Normal";
        }
        sSendMes.sprintf("HTSET,317,%s,",sData1);
    }//ex. HTSET,317,Normal,
    else if(sData[0]=="HTSET" && sData[1]=="318")                               //FT <> RT 時強制 Initail
    {
        if(eRunStartMode(LastSet.iRunStartMode)==0 || eRunStartMode(LastSet.iRunStartMode)==1) //FT->RT
        {
            SetRunStartMode(rsmCInitialRetest);
            fBinSel->PageControl1->ActivePageIndex=1;
            sData1="RT";
        }
        else if(eRunStartMode(LastSet.iRunStartMode)==2 || eRunStartMode(LastSet.iRunStartMode)==3) //RT->FT
        {
            SetRunStartMode(rsmInitialStart);
            fBinSel->PageControl1->ActivePageIndex=0;
            sData1="Normal";
        }
        else
        {
            sData1="NG";
        }
        sSendMes.sprintf("HTSET,318,%s,",sData1);
    }//ex. HTSET,318,Normal,
    else if(sData[0]=="HTSET" && sData[1]=="322")                               //指定 Bin Double Contact On
    {
        iData1=atoi(sData[2].c_str());
        /*  //Sam 20230711 : 修正 OLP SetCategroy 異常 Mark
        if(eRunStartMode(LastSet.iRunStartMode)==0 || eRunStartMode(LastSet.iRunStartMode)==1)      //FT(normal)
            fBinSel->PageControl1->ActivePageIndex=0;
        else if(eRunStartMode(LastSet.iRunStartMode)==2 || eRunStartMode(LastSet.iRunStartMode)==3)   //RT
            fBinSel->PageControl1->ActivePageIndex=1;
        */
        BinSelect[iTestRunMode].iDBContact[iData1]=1;
        bTCPIPChangeAlarm=true;
        fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
        fBinSel->spbSaveClick(this);
        bTCPIPChangeAlarm=false;
        sSendMes.sprintf("HTSR,322,DoubleContact_On,%d,",iData1);

    }//ex. HTSR,322,DoubleContact_On,2,
    else if(sData[0]=="HTSET" && sData[1]=="323")                               //指定 Bin Double Contact Off
    {
        iData1=atoi(sData[2].c_str());
        if(eRunStartMode(LastSet.iRunStartMode)==0 || eRunStartMode(LastSet.iRunStartMode)==1)  //FT(normal)
            fBinSel->PageControl1->ActivePageIndex=0;
        else if(eRunStartMode(LastSet.iRunStartMode)==2 || eRunStartMode(LastSet.iRunStartMode)==3)   //RT
            fBinSel->PageControl1->ActivePageIndex=1;
        BinSelect[iTestRunMode].iDBContact[iData1]=0;
        bTCPIPChangeAlarm=true;
        fBinSel->spbSaveClick(this);
        bTCPIPChangeAlarm=false;
        sSendMes.sprintf("HTSR,323,DoubleContact_Off,%d,",iData1);

    }//ex. HTSR,323,DoubleContact_Off,2,
    else if(sData[0]=="HTSET" && sData[1]=="331")                                //Config I31 On
    {
        szDir.sprintf("%sconfig.ini",AuthPath);
        IniConfig.bI31_1GPIBLotEnd=true;
        IniConfig.bI31_1GPIBLotEnd=ReadWriteIni(szDir, "Specific", "I31_GPIBLotEnd",IniConfig.bI31_1GPIBLotEnd,false,false);
        sSendMes.sprintf("HTSR,331,GpibLotEndI31_True,");
    }//ex. HTSR,331,GpibLotEndI31_True,
    else if(sData[0]=="HTSET" && sData[1]=="332")                               //Config I31 Off
    {
        szDir.sprintf("%sconfig.ini",AuthPath);
        IniConfig.bI31_1GPIBLotEnd=false;
        IniConfig.bI31_1GPIBLotEnd=ReadWriteIni(szDir, "Specific", "I31_GPIBLotEnd",IniConfig.bI31_1GPIBLotEnd,false,false);
        sSendMes.sprintf("HTSR,332,GpibLotEndI31_Off,");
    }//ex. HTSR,332,GpibLotEndI31_Off,
    //Sam 20181003 : TCP Command 自動啟動
    //==>
    //AI(ht9045-v899) 20260331: 擴展支援 bRemoteLotStart (Greatek/OEE)
    else if(sData[0]=="HTSET" && sData[1]=="333")                               //TCP Start
    {
        //Format: HTSET,333
        //Respond: HTSR,333,OK or HTSR,333,NG
        if(CUSTOMER_CODE==CC_TERAPOWER || CosFunction.bRemoteLotStart)
        {
            if(fMain->palMainStatus->Caption=="HALT" && SystemStart==false)
            {
                fMain->Start("TCP Command Start!!");
                RecordProcess("TCP Remote Start");
                sData1="OK";
            }
            else
            {
                sData1="NG";
            }
        }
        else
        {
            sData1="NG";
        }
        sSendMes.sprintf("HTSR,333,%s,",sData1);
    }//ex. HTSR,333,OK,
    else if(sData[0]=="HTSET" && sData[1]=="334")                               //TCP Pause
    {
        if(SystemStart==true && CUSTOMER_CODE==CC_TERAPOWER)                    //Sam 20181003 : 卡只有晶兆成可以接收停止通訊
        {
            fMain->Pause("TCP Command Pause");
            sData1="OK";
        }
        else
        {
            sData1="NG";
        }
        sSendMes.sprintf("HTSR,334,%s,",sData1);
    }//ex. HTSR,334,OK,
    //<==
    //Sam 20181003 : TCP Command 自動啟測
    else if(sData[0]=="HTSET" && sData[1]=="350")                               //Setting Contact Heigh
    {
        szDir.sprintf("%s%s\\Contact.Data",DataPath,GetLastOpenFN());
        if(SystemStart==false)
        {
            WriteIniData(szDir, "Test Arm1", "Contact",  sData[2]);
            WriteIniData(szDir, "Test Arm2", "Contact",  sData[3]);
            DeviceForm_File.IndexContact[0]  =ReadWriteIni(szDir, "Test Arm1", "Contact", 1.0, 1.0, true, true, 1.0, -148.0);
            DeviceForm_File.IndexContact[1]  =ReadWriteIni(szDir, "Test Arm2", "Contact", 1.0, 1.0, true, true, 1.0, -148.0);
            sData1="OK";
        }
        else
        {
            sData1="Fail";
        }
        sSendMes.sprintf("HTSR,350,%s,",sData1);

    }//ex. HTSR,350,OK,
    else if(sData[0]=="HTSET" && sData[1]=="354")                               //Setting ForcePerPinN
    {
        szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
        if(SystemStart==false)
        {
            fContact->edForcePerPinG->Text=sData[2];
            if(CosFunction.bFixNameOfForcePerPinG)                              //Steven 20240821 : 凌中心說要修正存檔名稱
            {
                WriteIniData(szDir, "Torque Control", "Force Per Pin N",    fContact->edForcePerPinN->Text);
                WriteIniData(szDir, "Torque Control", "Force Per Pin G",    fContact->edForcePerPinG->Text);
                DeviceForm_File.ForcePerPinN=ReadIniData(szDir, "Torque Control", "Force Per Pin N", 0.0);
                DeviceForm_File.ForcePerPinG=ReadIniData(szDir, "Torque Control", "Force Per Pin G", DeviceForm_File.ForcePerPinN*1000.0/9.8);
            }
            else
            {
                WriteIniData(szDir, "Torque Control", "Force Per Pin",      fContact->edForcePerPinN->Text);
                WriteIniData(szDir, "Torque Control", "Force Per Pin Kg",   fContact->edForcePerPinG->Text);
                DeviceForm_File.ForcePerPinN=ReadIniData(szDir, "Torque Control", "Force Per Pin", 0.0);
                DeviceForm_File.ForcePerPinG=ReadIniData(szDir, "Torque Control", "Force Per Pin Kg", DeviceForm_File.ForcePerPinN*1000.0/9.8);
            }
            sData1="OK";
        }
        else
        {
            sData1="Fail";
        }
        sSendMes.sprintf("HTSR,354,%s,",sData1);
    }//ex. HTSR,354,OK,
    else if(sData[0]=="HTGR" && sData[1]=="401")                                 //Tempture
    {
        if(LastSet.iTemperature==Tempture_Ambient)
            sData1="OFF";
        else
            sData1=edWorkTemperBase->Text;
        sSendMes.sprintf("HTSR,401,%s,",sData1);

    }//ex. HTSR,401,125,
    else if(sData[0]=="HTSET" && sData[1]=="403")                               //Show message
    {
        if(sData[2]!=NULL && (sData[2].Length()<1023 && sData[2].Length()!=0))
            sData1=sData[2]+",OK";
        else
            sData1="Fail";
         sSendMes.sprintf("HTSR,403,%s,",sData1);
         ShowMyMessage(sData[2]);
    }//ex. HTSR,403,124,OK,
    else if(sData[0]=="HTSET" && sData[1]=="404")                               //Close Site
    {
        bool bFlag=false;
        int iArm=0, ct1=0, iSiteStatus=0;
        if(SystemStart)
        {
            sData1="NG,Running,";
        }
        else if(CanChangeSite(false)==false)
        {
            if(LastSet.iTemperature==Tempture_Hot)
                sData1="NG,NeedCleanOut,";
            else
                sData1="NG,NeedOneCycle,";
        }
        else
        {
            //Sam 20210219 : TCP 開關 Site 最後一個不能關
            //==>
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)  //Sam 20210720 :  關 Amr 關 Site 異常
                iArm=1;   //只開 Arm2
            else
                iArm=0;

            for(int i=0; i<TestSocket.iMaxRow; i++)
            {
                for(int j=0; j<TestSocket.iMaxCol; j++)
                {
                    if(LastSet.bUseTestSocket[iArm][i][j])
                        ct1++;
                }
            }
            //<==
            //Sam 20210219 : TCP 開關 Site 最後一個不能關

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(atoi(sData[2].c_str())==int(TestIF_File.iSiteMap[i][j]))
                    {
                        if(LastSet.bUseTestSocket[iArm][i][j] && ct1==1)        //Sam 20200219 : TCP 開關 Site 最後一個不能關
                        {
                            iSiteStatus=1;
                            sData1+="NG,";
                        }
                        else
                        {
                            bFlag=!LastSet.bUseTestSocket[iArm][i][j];          //Sam 20210720 :  關 Amr 關 Site 異常
                            LastSet.bUseTestSocket[0][i][j]=bFlag;
                            LastSet.bUseTestSocket[1][i][j]=bFlag;
                            bTestSiteUse[0][i][j]=bFlag;
                            bTestSiteUse[1][i][j]=bFlag;
                            iSiteStatus=bFlag;
                            fMain->ShowTestHeadComp(false);
                            sData1+="OK,";
                        }
                        break;
                    }
                }
            }
            sData1+=sData[2]+",";
            sData1+=IntToStr(iSiteStatus)+",";
        }
        sSendMes.sprintf("HTSR,404,%s", sData1);

    }//ex. HTSR,404,OK,2,0,
    ////Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    //Sam 20230613 : 通知系統 Handler 已經密碼鎖定，改為被詢問。 Mark
    /*
    else if(sData[0]=="HTSET" && sData[1]=="502")                               //TCP Unlock Alarm Password
    {
        if(fNote->bNeedPassWord)
        {
            MyDBIProcess("Message","TCP Command Unlock Password!!");
            fNote->bNeedPassWord=false;
        }
        return;
    }
    */
    else if(sData[0]=="HTGR" && sData[1]=="411")    //Sam 20230802 : Add new TCP command
    {
        if(TestIF.iAutoClean_Function)
        {
            sData1="On";
        }
        else
        {
            fCleaning->rgAutoCleanOnOff->ItemIndex=0;
            sData1="Off";
        }
        sSendMes.sprintf("HTSR,411,%s,",sData1);
    }
    else if(sData[0]=="HTGR" && sData[1]=="413")    //Sam 20230802 : Add new TCP command
    {
        sSendMes.sprintf("HTSR,413,%d,",atoi(fCleaning->edCleaningCount->Text.c_str()));    //Sam 20240726 : AI Clean
    }
    else if(sData[0]=="HTGR" && sData[1]=="415")                                //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
    {
         sSendMes.sprintf("HTSR,415,%d,",TestIF_File.iIndexArmAutoCleanCnt);
    }
    else if(sData[0]=="HTSET" && sData[1]=="461")    //Sam 20230802 : Add new TCP command
    {
        if(bRunAutoClean)
        {
            sData1="NG";
        }
        else
        {
            AnsiString szDir="", S="";
            S=GetLastOpenFN();
            szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
            if(sData[2]=="On")
            {
                fCleaning->rgAutoCleanOnOff->ItemIndex=1;
                TestIF.iAutoClean_Function=1;
                TestIF_File.iAutoClean_Function=1;
            }
            else
            {
                fCleaning->rgAutoCleanOnOff->ItemIndex=0;
                TestIF.iAutoClean_Function=0;
                TestIF_File.iAutoClean_Function=0;
            }
            WriteIniData(szDir, "Configuration", "iAutoClean_Function", TestIF.iAutoClean_Function);
            sData1="OK";
        }
        sSendMes.sprintf("HTSR,461,%s,",sData1);
    }
    else if(sData[0]=="HTSET" && sData[1]=="463")    //Sam 20230802 : Add new TCP command
    {
        AnsiString szDir="", S="";
        S=GetLastOpenFN();
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        TestIF.iAutoClean_ContactTime=atof(sData[2].c_str())*10.0;
        TestIF_File.iAutoClean_ContactTime=atof(sData[2].c_str())*10.0;
        fCleaning->edContactTime->Text=sData[2];
        WriteIniData(szDir, "Configuration", "iAutoClean_ContactTime",TestIF.iAutoClean_ContactTime);
        sSendMes.sprintf("HTSR,463,OK,");
    }
    else if(sData[0]=="HTSET" && sData[1]=="464")    //Sam 20230802 : Add new TCP command
    {
        AnsiString szDir="", S="";
        S=GetLastOpenFN();
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        TestIF.iAutoClean_ContactCount=atoi(sData[2].c_str());
        TestIF_File.iAutoClean_ContactCount=atoi(sData[2].c_str());
        fCleaning->edACContactCount->Text=sData[2];
        WriteIniData(szDir, "Configuration", "iAutoClean_ContactCount",TestIF.iAutoClean_ContactCount);
        sSendMes.sprintf("HTSR,464,OK,");
    }
    else if(sData[0]=="HTSET" && sData[1]=="466")    //Sam 20230802 : Add new TCP command
    {
        AnsiString szDir="", S="";
        S=GetLastOpenFN();
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        TestIF.iAutoClean_IntervalContact=atoi(sData[2].c_str());
        TestIF_File.iAutoClean_IntervalContact=atoi(sData[2].c_str());
        fCleaning->edIntervalContact->Text=sData[2];
        WriteIniData(szDir, "Configuration", "iAutoClean_IntervalContact",TestIF.iAutoClean_IntervalContact);
        sSendMes.sprintf("HTSR,466,OK,");
    }
    else if(sData[0]=="HTSET" && sData[1]=="468")    //Sam 20230802 : Add new TCP command
    {
        if(bRunAutoClean)
        {
            sData1="NG";
        }
        else
        {
            AnsiString szDir="", S="";
            S=GetLastOpenFN();
            szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
            TestIF.iAutoClean_AlarmCount=atoi(sData[2].c_str());
            TestIF_File.iAutoClean_AlarmCount=atoi(sData[2].c_str());
            fCleaning->edAlarmCount->Text=sData[2];
            WriteIniData(szDir, "Configuration", "iAutoClean_AlarmCount",TestIF.iAutoClean_AlarmCount);
            sData1="OK";
        }
        sSendMes.sprintf("HTSR,468,%s,",sData1);
    }
    else if(sData[0]=="HTSET" && sData[1]=="469")    //Sam 20230802 : Add new TCP command
    {
        if(bRunAutoClean)
        {
            sData1="NG";
        }
        else
        {
            fCleaning->btnResetCleanCountClick(fCleaning);
            sData1="OK";
        }
        sSendMes.sprintf("HTSR,469,%s,",sData1);
    }
    else if(sData[0]=="HTSET" && sData[1]=="470")    //Sam 20230802 : Add new TCP command
    {
        AnsiString szDir="", S="";
        S=GetLastOpenFN();
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        TestIF.fAutoClean_DevicePinForceGf=atof(sData[2].c_str());
        TestIF_File.fAutoClean_DevicePinForceGf=atof(sData[2].c_str());
        WriteIniData(szDir, "Configuration", "fAutoClean_DevicePinForceGf",TestIF.fAutoClean_DevicePinForceGf);
        sSendMes.sprintf("HTSR,470,OK,");
    }
    else if(sData[0]=="HTSET" && sData[1]=="519")   //Sam 20230613 : 通知系統 Handler 已經密碼鎖定，改為被詢問
    {
        if(fNote->fShow && fNote->bNeedTCPAlarm)
        {
            MyDBIProcess("Message","TCP Command Unlock Password!!");
            fNote->bNeedTCPAlarm=false;
            fNote->bNeedPassWord=false;
            sSendMes.sprintf("HTSR,519,OK,");
        }
        else
        {
            sSendMes.sprintf("HTSR,519,NG,");
        }
    }
    else if(sData[0]=="HTSET" && sData[1]=="520")   //Sam 20230613 : 通知系統 Handler 已經密碼鎖定，改為被詢問
    {
        if(fNote->fShow && fNote->bNeedTCPAlarm)
        {
            fNote->bNeedTCPAlarm=false;
            fNote->bCloseShowMsg=true;
            sSendMes.sprintf("HTSR,520,OK,");
            HandlerTCPIPResultSendProcess(sSendMes);
            fNote->BtnSkipClick(fNote->BtnRetry);
            return;
        }
        else
        {
            sSendMes.sprintf("HTSR,520,NG,");
        }
    }
    else if(sData[0]=="HTSET" && sData[1]=="700")                               //TCP ART LOTCLEAR?
    {
        fSCKART->iWaitGPIBLotR=0;
        ZeroMemory(iAutoTrayCount, sizeof(iAutoTrayCount));
        if(HasICUnderMachine()==false)
        {
            LastSet.iTCPModeLotState=0;
            LastSet.bBreakSCKART=false;
            Clarn_Data(1, "ART_LOTCLEARED");
            fSCKART->ClearLotInfo();                                            //LOTCLEARED
            RecordProcess("ART LOTCLEARED.");
            fLotInfo->btClearBarcodeList->Click();
        }
         sSendMes.sprintf("HTSR,700,OK,");

    }//ex. HTSR,700,OK,
    else if(sData[0]=="HTSET" && sData[1]=="701")                               //TCP ART LOTRETESTCLEAR?
    {
        if(fSCKART->iTesterType==1 || fSCKART->iCurrentStatus!=fSCKART->iLOTSTATUS_A)
        {
            Clarn_Data(2, "ART_LOTRETESTCLEARED");
            if(LastSet.iTester==OFF_LINE)
            {
                if(BinSelect[OffT].bAutoRetest[0])
                    LastSet.BinCT[0][0]=0;
                if(BinSelect[OffT].bAutoRetest[1])
                    LastSet.BinCT[0][1]=0;
                if(BinSelect[OffT].bAutoRetest[2])
                    LastSet.BinCT[0][2]=0;
            }
            else
            {
                if(BinSelect[FT].bAutoRetest[0])
                    LastSet.BinCT[0][0]=0;
                if(BinSelect[FT].bAutoRetest[1])
                    LastSet.BinCT[0][1]=0;
                if(BinSelect[FT].bAutoRetest[2])
                    LastSet.BinCT[0][2]=0;
            }

            for(int i=0; i<10; i++)
            {
                LastSet.lSCKARTBinCT[i]=0;
            }
            AMR.ARTReset();                                                     //Sam 20240304 : 新增 AMR 功能
            LastSet.iSCKARTInputCT=0;
            LastSet.lShuttleCount=0;
            LotSummary.ClearRTData();

            WriteLastDataFile(false);//kevin 20141030
            fSortCT->ShowLoadingIC();
            fSortCT->ShowSortIC();

            if(fSCKART->iTesterType==0)
            {
                fSCKART->iInputCount=LastSet.iSCKART_RTUnitCount;
                LastSet.iSCKART_RTUnitCount=0;
            }
            fSCKART->iInputJamCnt    =0;
            fSCKART->iOutputJamCnt   =0;

            if(fSCKART->iCurrentStatus==fSCKART->iLOTSTATUS_R)
            {
                fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
                fSCKART->iWaitGPIBLotR=3;
            }
            fSCKART->AccessFile(false, -1);
            RecordProcess("ART LOTRETESTCLEARED.");
            fLotInfo->btClearBarcodeList->Click();
            sData1="OK";                                                        //LOTRETESTCLEAR
        }
        else
        {
            fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
            fSCKART->iCurrentFlexARTStep=10;
            sData1="NG";                                                        //SETTINGNG
        }
         sSendMes.sprintf("HTSR,701,%s,",sData1);
    }//ex. HTSR,701,OK,
    else if(sData[0]=="HTSET" && sData[1]=="702")                               //TCP ART INPUTQTY
    {
        bool bChangeLotID=false;
        if(HasICUnderMachine()==false && HasAnyICInMachine()==false) //Sam 20191124 : 也要檢查 Auto tray
        {
            bChangeLotID=true;
        }
        else
        {
            sData2.sprintf("%s", sData[3]);
            if(sData2==fSCKART->sLotID && (sData2!="" && sData2!=" "))
                bChangeLotID=false;
            else
                bChangeLotID=true;
        }

        if(bChangeLotID==true)
        {
            fSCKART->iCurrent93KARTStep=1;
            fSCKART->iCurrentFlexARTStep=4;
            Clarn_Data(1, "ART_INPUTQTY");
            fSCKART->ClearLotInfo();
            RecordProcess("ART INPUTQTY.");
            fLotInfo->btClearBarcodeList->Click();
            fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
            fSCKART->sLotID.sprintf("%s", sData[3]);
            fSCKART->sLotStartTime=Now().FormatString("yyyymmdd_hhnnss");
            if(bQAModeFlag==true)
            {
                fSCKART->iLotCount  =TestIF_File.iQAModeCount;
                fSCKART->iInputCount=TestIF_File.iQAModeCount;
                fSCKART->AccessFile(false, 1);                                  //必須在轉換模式前先存檔一次, 不然轉模式的裡面會讀取到舊的資料
                SetRunStartMode(rsmInitial_ART);
                bQAModeFlag=false;
            }
            else
            {
                fSCKART->iLotCount  =StrToInt(sData[2]);
                fSCKART->iInputCount=StrToInt(sData[2]);
                fSCKART->AccessFile(false, 1);
            }
            sData3.sprintf("Lot start: %s, %d", fSCKART->sLotID, fSCKART->iLotCount);
            RecordProcess(sData3);
            sData1="OK";                                                        //SETTINGOK
        }
        else
        {
            sData1="NG";                                                        //SETTINGNG
        }
        sSendMes.sprintf("HTSR,702,%s,",sData1);
        HandlerTCPIPResultSendProcess(sSendMes);
        if(fSCKART->iTesterType==1)// && CosFunction.bAutoRetestGPIBmode==true)
        {
            fSCKART->iNeedRT=0;
            fMain->SetLotState(2);                                              //TCP ART Lot Start
            HandlerTCPIPResultSendProcess(sSendMes);
            LastSet.bEndLotAutoRetestGPIB=false;
            LastSet.bWaitStartLotAutoRetestGPIB=false;
            LastSet.bFirstTestAutoRetestGPIB=true;
        }
        return;
    }//ex. HTSR,702,OK,
    else if(sData[0]=="HTSET" && sData[1]=="703")                               //TCP ART
    {
        if(fSCKART->iFTRTCount==0)
            fSCKART->iCurrent93KARTStep=3;
        else
            fSCKART->iCurrent93KARTStep=9;
        LastSet.bWaitStartLotAutoRetestGPIB=true;                               //TCP ART RT Start
        sSendMes.sprintf("HTSR,703,OK,");
    }//ex. HTSR,703,OK,
    else if(sData[0]=="HTSET" && sData[1]=="704")                               //TCP ART
    {
        if(fSCKART->iCurrent93KARTStep>=10)
            fSCKART->iCurrent93KARTStep=12;
        LastSet.bWaitEndLotAutoRetestGPIB=true;
        sSendMes.sprintf("HTSR,704,OK,");
    }//ex. HTSR,704,OK,
    //TCP ART  SRQKIND?
    else if(sData[0]=="HTSET" && sData[1]=="706")
    {
        if(fNote->fShow)
        {
            if(fNote->bNeedTCPAlarm && fNote->bNeedPassWord)                    //Sam 20230613 : 通知系統 Handler 已經密碼鎖定，改為被詢問
            {
                iData1=99;                                                      //當有 bNeedTCPAlarm 時 IT詢問要回傳 99
            }
            else if(fNote->edErrorCode->Text=="WAR16104" ||                     //Sam 20250820 : 706 在 TrayFeed 與 Fix 滿盤時要回覆101
                    fNote->edErrorCode->Text=="MES1720"  ||
                    fNote->edErrorCode->Text=="MES1820"  ||
                    fNote->edErrorCode->Text=="MES1920")
            {
                iData1=101;
            }
            else
            {
                iData1=LastSet.iTCPModeLotState;
            }
        }
        else
        {
            iData1=LastSet.iTCPModeLotState;
        }
        sSendMes.sprintf("HTSR,706,%d,",iData1);                                //SRQKIND
    }//ex. HTSR,706,2,
    else if(sData[0]=="HTSET" && sData[1]=="709")                               //TCP ART Break 強制中斷 ART
    {
        LastSet.iSCKART_RTUnitCount=0;
        fSCKART->CheckNeedRT();                                                 //For TCP ART
        fMain->SetLotState(10);                                                 //TCP ART Final Lot End
        sSendMes.sprintf("HTSR,709,OK,");
    }//ex. HTSR,709,OK,
    else if(sData[0]=="HTSET" && sData[1]=="710")                               //TCP ART RT Count Setting
    {
        szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
        MyForceDirectories(szDir);
        szDir+="\\Tester.Data";
        TestIF_File.iSCKART_TryCnt=atoi(sData[2].c_str());
        TestIF_File.iSCKART_TryCnt=ReadWriteIni(szDir,"AutoRetest","Try Count",TestIF_File.iSCKART_TryCnt,3,false,true,1,20);
        sSendMes.sprintf("HTSR,710,OK,");
    }//ex. HTSR,710,OK,
    else if(sData[0]=="HTSET" && sData[1]=="711")                               //TCP ART Initail ART
    {
        if(HasICUnderMachine()==false)
        {
            fSCKART->AccessFile(false, 1);                                      //必須在轉換模式前先存檔一次, 不然轉模式的裡面會讀取到舊的資料
            SetRunStartMode(rsmInitial_ART);
            sData1="OK";
        }
        else
        {
            sData1="NG";
        }
        sSendMes.sprintf("HTSR,711,%s,",sData1);
    }//ex. HTSR,711,OK,
    else if(sData[0]=="HTSET" && sData[1]=="712")                               //TCP ART 設定開啟或關閉
    {
        szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
        MyForceDirectories(szDir);
        szDir+="\\Tester.Data";
        TestIF_File.bSCKART_EnableART=atoi(sData[2].c_str());  //0 Close; 1 Open
        TestIF_File.bSCKART_EnableART=ReadWriteIni(szDir,"AutoRetest","Enable ART",TestIF_File.bSCKART_EnableART,IniConfig.bA10_AutoReTest,false);
        fSCKART->Show();
        fSCKART->btnExit->Click();
        fMain->SetStartModeData();
        sSendMes.sprintf("HTSR,712,OK,");

    }//ex. HTSR,712,OK,
    else if(sData[0]=="HTSET" && sData[1]=="713")                               //TCP ART 設定開啟或關閉 Auto Socket Off Function
    {
        szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
        MyForceDirectories(szDir);
        szDir+="\\Tester.Data";
        TestIF_File.bSCKART_AutoSocketOff=atoi(sData[2].c_str());  //0 Close; 1 Open
        TestIF_File.bSCKART_AutoSocketOff=ReadWriteIni(szDir, "AutoRetest","Auto Socket Off",TestIF_File.bSCKART_AutoSocketOff,IniConfig.bA10_AutoReTest,false);
        sSendMes.sprintf("HTSR,713,OK,");
    }//ex. HTSR,713,OK,
    //AI(ht9045-v899) 20260331: Greatek TCP Remote Start Lot
    else if(sData[0]=="HTSET" && sData[1]=="720")                               //TCP Remote Start Lot (Greatek/OEE)
    {
        //Format: HTSET,720,<LotID>,<OPID>
        //Respond: HTSR,720,OK,<LotID> or HTSR,720,NG,<ErrorCode>
        if(CosFunction.bRemoteLotStart==false)
        {
            sSendMes.sprintf("HTSR,720,NG,NOT_SUPPORTED");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        if(RunInfo.bLotStart==true)
        {
            sSendMes.sprintf("HTSR,720,NG,ALREADY_STARTED");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        if(HasICUnderMachine()==true || HasAnyICInMachine()==true)
        {
            sSendMes.sprintf("HTSR,720,NG,IC_IN_MACHINE");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        if(sData[2]=="" || sData[2]==" ")
        {
            sSendMes.sprintf("HTSR,720,NG,EMPTY_LOTID");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        //Set LotID
        fLotInfo->edtSysLotID->Text = sData[2];
        //Set OPID (optional)
        if(sData[3]!="" && sData[3]!=" ")
        {
            fLotInfo->edtSysOperatorID->Text = sData[3];
        }
        //Execute Lot Start
        fLotInfo->SetLotID(sData[2], false);
        fLotInfo->SetLotStart(__FUNC__);
        RecordProcess("TCP Remote Lot Start: " + sData[2]);
        sSendMes.sprintf("HTSR,720,OK,%s", sData[2]);
        HandlerTCPIPResultSendProcess(sSendMes);
        return;
    }//ex. HTSR,720,OK,LOT001
    //AI(ht9045-v899) 20260331: Greatek TCP Remote End Lot
    else if(sData[0]=="HTSET" && sData[1]=="721")                               //TCP Remote End Lot (Greatek/OEE)
    {
        //Format: HTSET,721
        //Respond: HTSR,721,OK,<LotID> or HTSR,721,NG,<ErrorCode>
        if(CosFunction.bRemoteLotStart==false)
        {
            sSendMes.sprintf("HTSR,721,NG,NOT_SUPPORTED");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        if(RunInfo.bLotStart==false)
        {
            sSendMes.sprintf("HTSR,721,NG,NOT_STARTED");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        if(SystemStart==true)
        {
            sSendMes.sprintf("HTSR,721,NG,SYSTEM_RUNNING");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        if(HasICUnderMachine()==true || HasAnyICInMachine()==true)
        {
            sSendMes.sprintf("HTSR,721,NG,IC_IN_MACHINE");
            HandlerTCPIPResultSendProcess(sSendMes);
            return;
        }
        //Execute Lot End
        AnsiString sLotID = fLotInfo->edtSysLotID->Text;
        fLotInfo->SetLotEnd(__FUNC__);
        RecordProcess("TCP Remote Lot End: " + sLotID);
        sSendMes.sprintf("HTSR,721,OK,%s", sLotID);
        HandlerTCPIPResultSendProcess(sSendMes);
        return;
    }//ex. HTSR,721,OK,LOT001
    else if(sData[0]=="HTGR" && sData[1]=="732")                                //TCP ART 詢問啟動狀態
    {
        if(TestIF_File.bSCKART_EnableART)
            sData1="Open";
        else
            sData1="Close";
        sSendMes.sprintf("HTSR,732,%s,",sData1);
    }//ex. HTSR,732,Open,
    else if(sData[0]=="HTGR" && sData[1]=="733")                                //詢問是否有設定 ART 位置
    {
        if(Prod.bART6Tray[0] ||
           Prod.bART6Tray[1] ||
           Prod.bART6Tray[2] )
        {
            sData1="OK";
        }
        else
        {
            sData1="NG";
        }
        sSendMes.sprintf("HTSR,733,%s,",sData1);
    }//ex. HTSR,733,OK,
    //Sam 20240304 : 新增 AMR 功能
    //==>                                                             
    else if(sData[0]=="HTGR" && sData[1]=="801")                                //詢問是否有 Alarm 停機
    {
        if(fNote->fShow)
        {
            //if(fNote->edErrorCode->Text=="WAR16104" || fNote->edErrorCode->Text=="MES1640")
            if(fNote->edErrorCode->Text=="MES1640")
                sData1="Normal";
            else if(fNote->edErrorCode->Text=="WAR16104" ||
                    fNote->edErrorCode->Text=="MES1720"  ||                     //Sam 202500701 : AMR Fix Tray Feed
                    fNote->edErrorCode->Text=="MES1820"  ||
                    fNote->edErrorCode->Text=="MES1920")
                sData1="Fix full";
            else
                sData1="Down";
        }
        else if(MyMessageBox->fShow)
        {
            sData1="Down";
        }
        else
        {
            sData1="Normal";
        }
        sSendMes.sprintf("HTSR,801,%s,",sData1);
    }//ex. HTSR,801,Down
    else if(sData[0]=="HTGR" && sData[1]=="802")                                //詢問是否在 Tray Feed
    {
        AnsiString sLoader="Loader",sEmpty="NA",sColor="NA";
        if(LastSet.bAMRTrayFeedWait)
            sData1="true";
        else
            sData1="false";
//        if(Sen[SnLoaderTrayHasTray].IsOn())   //Loader 強制都要收料
//            sLoader="Loader";
        if(Sen[SenEmptySelectHasTray].IsOn())
            sEmpty="Empty";
        if(Sen[SenColorSelectHasTray].IsOn())
            sColor="Color";

        sData2.sprintf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
                                                    sLoader,
                                                     sEmpty,
                                                     sColor,
                                     AMR.GetTrackBinData(0),
                                     AMR.GetTrackBinData(1),
                                     AMR.GetTrackBinData(2),
                                     AMR.GetTrackBinData(3),
                                     AMR.GetTrackBinData(4),
                                     AMR.GetTrackBinData(5),
                                     AMR.GetTrackBinData(6),
                                     AMR.GetTrackBinData(7),
                                     AMR.GetTrackBinData(8));
        sSendMes.sprintf("HTSR,802,%s,%s,",sData1,sData2);
    }//ex. HTSR,801,Down
    else if(sData[0]=="HTSET" && sData[1]=="803")                               //AMR Tray Feed 完成
    {
        AMR.Initial();
        sSendMes="HTSR,803,OK,";
    }//ex. HTSR,803,OK,
    else if(sData[0]=="HTSET" && sData[1]=="804")                               //設定 AMR 開啟
    {
        szDir.sprintf("%sconfig.ini",AuthPath);
        int iSet=atoi(sData[2].c_str());
        bool bOnOff=(iSet==1)?true:false;
        if(iSet==0 || iSet==1)
        {
            fConfiguration->cbA60_1->Checked=bOnOff;
            IniConfig.bA60EnableAMR=bOnOff;
            WriteIniData(szDir, "Function", "bA60EnableAMR",bOnOff);
            sData1="OK";
        }
        else
        {
            sData1="NG";
        }
        fLotInfo->RefreshAMR();
        sSendMes.sprintf("HTSR,804,%s,",sData1);

    }//ex. HTSR,804,OK,
    else if(sData[0]=="HTGR" && sData[1]=="805")                                //詢問 AMR 開關狀態
    {
        sData1=(IniConfig.bA60EnableAMR)?1:0;
        sSendMes.sprintf("HTSR,805,%s,",sData1);
    }//ex. HTSR,805,0,  HTSR,805,1,
    else if(sData[0]=="HTGR" && sData[1]=="810")                                //詢問 Loader 進盤量是否達到設定值
    {
        fLotInfo->RefreshAMR();
        if(AMR.CheckLoaderCount())
            sData1="true";
        else
            sData1="false";
        if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)          //Sam 20250423 : AMR + ART Retest 修改滿 Tray 也要收盤
        {
            if(fSCKART->iFTRTCount==0)                                          //AMR 模式補盤需要提示 FT or RT
                sSendMes.sprintf("HTSR,810,%s,FT,",sData1);
            else
                sSendMes.sprintf("HTSR,810,%s,RT,",sData1);
        }
        else
        {
            sSendMes.sprintf("HTSR,810,%s,",sData1);
        }
    }//ex. HTSR,810,true, (達到) HTSR,810,false (未達到)
    else if(sData[0]=="HTSET" && sData[1]=="811")                               //AMR Loader 已經補盤完成累加 Loader 計數
    {
        if(sData[2]=="Last")
            LastSet.bAMRLoaderLast=true;
        LastSet.bAMRRequestSupplyTray=false;//Sam 20240827 : 新增 AMR 功能
        AMR.LoaderCarryIn();
        sSendMes="HTSR,811,OK,";
    }//ex. "HTSR,811,OK,
    else if(sData[0]=="HTSET" && sData[1]=="812")                               //AMR 詢問 Loader 汽缸狀態是否可以補盤
    {
        if(bLoadingNewICTray || LastSet.bAMRLoaderLast)     //有進盤中或者收到最後補盤指令後都要回 NO
        {
            sData1="NO";
        }
        else
        {
            sData1="OK";
            LastSet.bAMRRequestSupplyTray=true; //Loader 進盤流程會被鎖定，直到收到 AMR 補完盤，收到 HTSET,811, 命令後才會解除
            fLotInfo->RefreshAMR();
        }
        sSendMes.sprintf("HTSR,812,%s,",sData1);
    }
    else if(sData[0]=="HTGR" && sData[1]=="840")                                //詢問 Auto1 出盤量是否達到設定值
    {
        fLotInfo->RefreshAMR();
        if(AMR.CheckUnloaderCount(0))
            sData1="true";
        else
            sData1="false";
        if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)          //Sam 20250423 : AMR + ART Retest 修改滿 Tray 也要收盤
        {
            if(AMR.IsAutoReTestTray(0))
            {
                if(fSCKART->iNeedRT==2)
                    sData2=AMR.GetTrackBinData(0);                              //最後測試就丟正常的 Bin
                else
                    sData2=AMR.GetTrackBinDataCateR(0);                         //還需要 RT 就丟有設定 CateR & Retest 的 Category 的 Bin count
            }
            else if(BinSelect[iTestRunMode].bCateR[0])
            {
                if(fSCKART->iNeedRT==2)
                    sData2=AMR.GetTrackBinData(0);                              //最後測試就丟正常的 Bin
                else
                    sData2="CateR";
            }
            else
            {
                sData2=AMR.GetTrackBinData(0);
            }
        }
        else
        {
            sData2=AMR.GetTrackBinData(0);
        }
        sSendMes.sprintf("HTSR,840,%s,%s,",sData1,sData2);
    }//ex. HTSR,840,true, (達到) HTSR,840,false (未達到)
    else if(sData[0]=="HTSET" && sData[1]=="841")                               //AMR Auto1 已經收盤完成清除 Auto 計數
    {
        AMR.UnloaderCarryOut(0);
        sSendMes="HTSR,841,OK,";
    }//ex. "HTSR,841,OK,
    else if(sData[0]=="HTGR" && sData[1]=="850")                                //詢問 Auto2 出盤量是否達到設定值
    {
        fLotInfo->RefreshAMR();
        if(AMR.CheckUnloaderCount(1))
            sData1="true";
        else
            sData1="false";
        if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)          //Sam 20250423 : AMR + ART Retest 修改滿 Tray 也要收盤
        {
            if(AMR.IsAutoReTestTray(1))
            {
                if(fSCKART->iNeedRT==2)
                    sData2=AMR.GetTrackBinData(1);                              //最後測試就丟正常的 Bin
                else
                    sData2=AMR.GetTrackBinDataCateR(1);                         //還需要 RT 就丟有設定 CateR & Retest 的 Category 的 Bin count
            }
            else if(BinSelect[iTestRunMode].bCateR[1])
            {
                if(fSCKART->iNeedRT==2)
                    sData2=AMR.GetTrackBinData(1);                              //最後測試就丟正常的 Bin
                else
                    sData2="CateR";
            }
            else
            {
                sData2=AMR.GetTrackBinData(1);
            }
        }
        else
        {
            sData2=AMR.GetTrackBinData(1);
        }

        sSendMes.sprintf("HTSR,850,%s,%s,",sData1,sData2);
    }//ex. HTSR,850,true, (達到) HTSR,850,false (未達到)
    else if(sData[0]=="HTSET" && sData[1]=="851")                               //AMR Auto2 已經收盤完成清除 Auto 計數
    {
        AMR.UnloaderCarryOut(1);
        //AMR.ClearTrackBinData(1);
        sSendMes="HTSR,851,OK,";
    }//ex. "HTSR,851,OK,
    else if(sData[0]=="HTGR" && sData[1]=="860")                                //詢問 Auto3 出盤量是否達到設定值
    {
        fLotInfo->RefreshAMR();
        if(AMR.CheckUnloaderCount(2))
            sData1="true";
        else
            sData1="false";

        if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)          //Sam 20250423 : AMR + ART Retest 修改滿 Tray 也要收盤
        {
            if(AMR.IsAutoReTestTray(2))
            {
                if(fSCKART->iNeedRT==2)
                    sData2=AMR.GetTrackBinData(2);                              //最後測試就丟正常的 Bin
                else
                    sData2=AMR.GetTrackBinDataCateR(2);                         //還需要 RT 就丟有設定 CateR & Retest 的 Category 的 Bin count
            }
            else if(BinSelect[iTestRunMode].bCateR[2])
            {
                if(fSCKART->iNeedRT==2)
                    sData2=AMR.GetTrackBinData(2);                              //最後測試就丟正常的 Bin
                else
                    sData2="CateR";
            }
            else
            {
                sData2=AMR.GetTrackBinData(2);
            }
        }
        else
        {
            sData2=AMR.GetTrackBinData(2);
        }

        sSendMes.sprintf("HTSR,860,%s,%s,",sData1,sData2);
    }//ex. HTSR,860,true, (達到) HTSR,860,false (未達到)
    else if(sData[0]=="HTSET" && sData[1]=="861")                               //AMR Auto3 已經收盤完成清除 Auto 計數
    {
        AMR.UnloaderCarryOut(2);
        //AMR.ClearTrackBinData(2);
        sSendMes="HTSR,861,OK,";
    }//ex. "HTSR,861,OK,    
    //<==
    //Sam 20240304 : 新增 AMR 功能
    HandlerTCPIPResultSendProcess(sSendMes);
}
//---------------------------------------------------------------------------
int TfMain::GetProdModeByDll()                                                  //JerryYang 20220311 : ATP鎖定Critical parameter
{
    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(RunInfo.bLotStart)                                                       //Lot start=Production mode
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//---------------------------------------------------------------------------
int TfMain::SetProdModeByDll(int iProdMode)                                     //bProdMode=true  : enable production(block edit mode)  //JerryYang 20220311 : ATP鎖定Critical parameter
{                                                                               //bProdMode=false : disable production (allow edit)
    AnsiString Msg="";
    if(InitialOK==false)
    {
         return -4;                                                             //Operation not Allowed
    }

    if(iProdMode==0)                                                            //Allow edit
    {
        if(RunInfo.bLotStart==false)
        {
            return 0;                                                           //Sucess
        }
        else
        {
            if(fMain->CheckCanChangeRealDummy()==false || HasICUnderMachine())
            {
                return -1;                                                      //fail
            }
            fLotInfo->SetLotEnd(__FUNC__);
            return 0;                                                           //Sucess
        }
    }
    else                                                                        //Block edit
    {
        if(RunInfo.bLotStart)
        {
            return 0;                                                           //Sucess
        }
        else
        {
            fLotInfo->SetLotStart(__FUNC__);
            return 0;                                                           //Sucess
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::WriteREADYNEXTSHOT()                                               //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    SendMSG_CMD(MSG_CMD_READYNEXTSHOT, GetREADYNEXTSHOT());
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetREADYNEXTSHOT()                                           //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    AnsiString asDeviceInf="";
    AnsiString asSendData="";
    int iHasCode=0;

    if(IndexStatus==Z1Up_Z2Down)                                                //sht1 barcode scan
    {
        iHasCode=-1;
    }
    else if(IndexStatus==Z1Down_Z2Up)
    {
        iHasCode=-2;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)                                     //Steven 20231024 : not support NN mode
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(IndexStatus==Z1Up_Z2Down)                                        //sht1 barcode scan
            {
                if(FTestSuck.HasNotTestYet() ||
                   (FLCarryKit.Item[i][j]!=NULL_IC &&
                    FLCarryKit.Item[i][j]!=HAS_NULL_IC &&
                    FLCarryKit.cDeviceInf[i][j]!=""))
                {
                    iHasCode=1;
                }
            }
            else if(IndexStatus==Z1Down_Z2Up)
            {
                if(BTestSuck.HasNotTestYet() ||
                   (BLCarryKit.Item[i][j]!=NULL_IC &&
                    BLCarryKit.Item[i][j]!=HAS_NULL_IC &&
                    BLCarryKit.cDeviceInf[i][j]!=""))
                {
                    iHasCode=1;
                }
            }
        }
    }

    if(iHasCode==1)
    {
        asSendData="1";                                                         //1 =>  ready
    }
    else
    {
        if(MOT[MMTrayY].HasIC() ||
           MOT[MMPlate1].HasIC() ||
           MOT[MMPlate2].HasIC() ||
           (iHasCode==-1 && (FTestSuck.HasNotTestYet() || FLCarryKit.HasRealIC())) ||
           (iHasCode==-2 && (BTestSuck.HasNotTestYet() || BLCarryKit.HasRealIC())))
        {
            asSendData="2";                                                     //2 => Ongoing to read next 2DID
        }
        else
        {
            asSendData="0";
        }
    }

    asSendData="READYNEXTSHOT:"+asSendData;
    return asSendData;
}
//---------------------------------------------------------------------------
void TfMain::WriteNEXT2DID()                                                    //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    SendMSG_CMD(MSG_CMD_NEXT2DID, GetNEXT2DID());
}
//---------------------------------------------------------------------------
AnsiString TfMain::GetNEXT2DID()                                                //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    TStringList *tNEXTBarCodeList=new TStringList;
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        tNEXTBarCodeList->Add("0");
    }
    if(IndexStatus==Z1Up_Z2Down)                                                //sht1 barcode scan
    {
        if(FTestSuck.HasNotTestYet())
            Get2DID_OrderBySites(&FTestSuck, tNEXTBarCodeList);
        else
            Get2DID_OrderBySites(&FLCarryKit, tNEXTBarCodeList);
    }
    else                                                                        //sht2 barcode scan
    {
        if(BTestSuck.HasNotTestYet())
            Get2DID_OrderBySites(&BTestSuck, tNEXTBarCodeList);
        else
            Get2DID_OrderBySites(&BLCarryKit, tNEXTBarCodeList);
    }
    AnsiString t="NEXT2DID:"+tNEXTBarCodeList->CommaText;
    delete tNEXTBarCodeList;
    return t;
}
//---------------------------------------------------------------------------
void TfMain::Get2DID_OrderBySites(TMyKitSuck *kit, TStringList *sSourceList)    //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
{
    if((BAR_CODE_INSTALL!=ebctUninstall && TestIF.bEnableBarCode) ||
       (INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction))                      //Ifor 20210407 add: 自製OCR
    {
    }
    else
    {
        return;
    }
    int iSiteNo=0;
    for(int i=0; i<TestSocket.iShtRow; i++)                                     //Steven 20231024 : need to add for NN mode
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iSiteNo=TestIF.iSiteMap[i][j]-1;
            if(iSiteNo>=0 &&
               kit->Item[i][j]!=NULL_IC &&
               kit->Item[i][j]!=HAS_NULL_IC)
            {
                sSourceList->Strings[MAX_SOCKET_TOTAL-1-iSiteNo]=kit->cDeviceInf[i][j];
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfMain::SetAICCD()                                                         //Sam 20231108 : Add GPIB SETAICCD_
{
    TStringList *tFixAOICmd=new TStringList();
    AnsiString asRet="OK", asCmd="", szDir="",str1="";
    char str[256];

    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, GetLastOpenFN());
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
    str1=StringReplace(asCmd,"_", ",", TReplaceFlags()<<rfReplaceAll);

    tFixAOICmd->CommaText=str1;

    if(tFixAOICmd->Count==8)                                                    //Sam 20240826 : FixAOI 指令修改
    {
        //1_1_1000_15000_0.5_150 共六組參數
        //bEnableFix2BGAAICCD
        TestIF_File.bEnableFix2BGAAICCD             =(tFixAOICmd->Strings[0]=="1")?true:false;
        TestIF_File.iResultShowType                 =atoi(tFixAOICmd->Strings[1].c_str());      //Sam 20240325 : 新增 DamageTrayMapping 功能
        TestIF_File.iFix2BGAAICCDStartDelay         =atoi(tFixAOICmd->Strings[2].c_str());
        TestIF_File.iFix2BGAAICCDExposureTimeOut    =atoi(tFixAOICmd->Strings[3].c_str());
        TestIF_File.iFix2BGAAICCDAutoRetry          =atoi(tFixAOICmd->Strings[4].c_str());      //Sam 20240826 : FixAOI 指令修改
        TestIF_File.dInspectResultThres             =atof(tFixAOICmd->Strings[5].c_str());
        TestIF_File.iFix2BGAAICCDGetResultTimeOut   =atoi(tFixAOICmd->Strings[6].c_str());
        TestIF_File.iFix2BGAAICCDOutArmCycleInsp    =atoi(tFixAOICmd->Strings[7].c_str());

        fFixAICCD->chkEnableFix2AICCD->Checked          =TestIF_File.bEnableFix2BGAAICCD;
        fFixAICCD->rgResultShowType->ItemIndex          =TestIF_File.iResultShowType;   //Sam 20240325 : 新增 DamageTrayMapping 功能
        fFixAICCD->edtlFix2AICCDStartDelay->Text        =IntToStr(TestIF_File.iFix2BGAAICCDStartDelay);
        fFixAICCD->edtlFix2AICCDExposureTimeOut->Text   =IntToStr(TestIF_File.iFix2BGAAICCDExposureTimeOut);
        fFixAICCD->edtInspectResultThres->Text          =FloatToStr(TestIF_File.dInspectResultThres);
        fFixAICCD->edtOutArmCycleInsp->Text             =IntToStr(TestIF_File.iFix2BGAAICCDOutArmCycleInsp);

        WriteIniData(szDir, "Configuration", "Fix2 AI CCD Enable",                  TestIF_File.bEnableFix2BGAAICCD);
        WriteIniData(szDir, "Configuration", "Fix2 AI iResultShowType",             TestIF_File.iResultShowType);   //Sam 20240325 : 新增 DamageTrayMapping 功能
        WriteIniData(szDir, "Configuration", "Fix2 AI CCD Start Delay",             TestIF_File.iFix2BGAAICCDStartDelay);
        WriteIniData(szDir, "Configuration", "Fix2 AI CCD Exposure Time Out",       TestIF_File.iFix2BGAAICCDExposureTimeOut);
        WriteIniData(szDir, "Configuration", "Fix2 AI InspectResultThres",          TestIF_File.dInspectResultThres);
        WriteIniData(szDir, "Configuration", "Fix2 AI OutArmCycleInsp",             TestIF_File.iFix2BGAAICCDOutArmCycleInsp);
    }
    else
    {
        asRet="NG";
    }
    SendMSG_CMD(MSG_CMD_SETAICCD, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetAICCD()                                                         //Sam 20240826 : Add GPIB GETAICCD?
{
    AnsiString asRet="";
    asRet.sprintf("AICCD_%s_%d_%d_%d_%d_%2.2f_%d_%d",
                  (TestIF_File.bEnableFix2BGAAICCD)?"1":"0",
                   TestIF_File.iResultShowType,
                   TestIF_File.iFix2BGAAICCDStartDelay,
                   TestIF_File.iFix2BGAAICCDExposureTimeOut,
                   TestIF_File.iFix2BGAAICCDAutoRetry,
                   TestIF_File.dInspectResultThres,
                   TestIF_File.iFix2BGAAICCDGetResultTimeOut,
                   TestIF_File.iFix2BGAAICCDOutArmCycleInsp);
    SendMSG_CMD(MSG_CMD_GETAICCD, asRet);
}
//---------------------------------------------------------------------------
bool TfMain::IsStackHasLess16Bin(int iStack)                                    //JerryYang 20231218 : 檢查該Unloader是否有設定bin別  //JerryYang 20250505 : 只檢查16 bin以下
{
    int iBinCntMax=17; 
    for(int i=1; i<iBinCntMax; i++)                                             //從Bin1開始, 比照Epson所定義的格式
    {
        if(BinSelect[iTestRunMode].iCatDataT3Pos[i]==iStack+1)
        {
            return true;
        }
    }

    if(BinSelect[iTestRunMode].IfErrorT3==iStack)                               //比照EPSON的指令, Bin0當作是Error bin
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfMain::TransformTcGPIBData(AnsiString asStr)
{
    double dbATC_Offset[ATC_HEAD_COUNT];
    int ret=0;
    ZeroMemory(dbATC_Offset, sizeof(dbATC_Offset));
    AnsiString str;
    TStringList *IsLine=new TStringList();
    int Num=asStr.Pos("_");
    asStr.Delete(1, Num);
    str=StringReplace(asStr,"_", ",", TReplaceFlags()<<rfReplaceAll);
    IsLine->CommaText=str;

    bool bUse1by2Hear=false;
    int iRow=TestSocket.iShtRow;
    int iCol=TestSocket.iShtCol;
    
    if((TestIF_File.iTestMode==_12Site2X6   ||
        TestIF_File.iTestMode==_16Site2X8   ||
        TestIF_File.iTestMode==_32Site4X8N) &&
        TestIF_File.bUse32Heater==false)
    {
        iCol=TestSocket.iShtCol/2;
        bUse1by2Hear=true;
    }
    else if(TestIF_File.iTestMode==QualSite2X2N ||
            TestIF_File.iTestMode==_6Site2X3N   ||
            TestIF_File.iTestMode==_8Site2X4N)
    {
        iRow=TestSocket.iShtRow/2;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<iCol; j++)
        {
            //          Arm1                    Arm2
            //00 01 02 03 04 05 06 07   16 17 18 19 20 21 22 23
            //08 09 10 11 12 13 14 15   24 25 26 27 28 29 30 31
            if(iRow==1)
            {                                                                   //20160523 irow = 1 Site Mapping 方向  由左至右資料讀取
                dbATC_Offset[j]=Temperature.dATCTempOffset[j];                                          //Ifor 20160523 Arm1 陣列起始位置00
                dbATC_Offset[j+(iATC_Use_Heat_Count/2)]=Temperature.dATCTempOffset[j+16];               //Ifor 20160523 Arm2 陣列起始位置16
            }
            else
            {
                if(bUse1by2Hear==true)
                {
                    dbATC_Offset[j*2+i]=Temperature.dATCTempOffset[j*2+(i*8)];                                //Ifor 20160523 Arm1 陣列起始位置00 +下排起始位置間距8  (00+8=08)
                    dbATC_Offset[j*2+i+(iATC_Use_Heat_Count/2)]=Temperature.dATCTempOffset[(j*2+16)+(i*8)];   //Ifor 20160523 Arm2 陣列起始位置16 +下排起始位置間距8  (16+8=24)
                }
                else
                {                                                               //Ifor 20160523 irow = 2 Site Mapping 方向 由上至下 由下往上 位置之資料讀取
                    dbATC_Offset[j*2+i]=Temperature.dATCTempOffset[j+(i*8)];                                //Ifor 20160523 Arm1 陣列起始位置00 +下排起始位置間距8  (00+8=08)
                    dbATC_Offset[j*2+i+(iATC_Use_Heat_Count/2)]=Temperature.dATCTempOffset[(j+16)+(i*8)];   //Ifor 20160523 Arm2 陣列起始位置16 +下排起始位置間距8  (16+8=24)
                }
            }
        }
    }

    for(int i=0; i<(iATC_Use_Heat_Count/2); i++)
    {
        if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
        {
            dGPIBATCOffset[i]=atof(IsLine->Strings[i].c_str());
            dbATC_Offset[i]+=atof(IsLine->Strings[i].c_str());
            if(Temperature.fWorkTemperBase+dbATC_Offset[i]>136 ||
               Temperature.fWorkTemperBase+dbATC_Offset[i]<20)
                ret++;
        }
        else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
        {
            dGPIBATCOffset[i+iATC_Use_Heat_Count/2]=atof(IsLine->Strings[i].c_str());
            dbATC_Offset[i+iATC_Use_Heat_Count/2]+=atof(IsLine->Strings[i].c_str());
            if(Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]>136 ||
               Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]<20)
                ret++;
        }
        else if(IndexStatus==Z1_Z2_Down || iGPIBIndexStatus==Z1_Z2_Down)
        {
            if(i<iRow*iCol)
            {
                dGPIBATCOffset[i]=atof(IsLine->Strings[i].c_str());
                dbATC_Offset[i]+=atof(IsLine->Strings[i].c_str());
                if(Temperature.fWorkTemperBase+dbATC_Offset[i]>136 ||
                   Temperature.fWorkTemperBase+dbATC_Offset[i]<20)
                    ret++;

                dGPIBATCOffset[i+iATC_Use_Heat_Count/2]=atof(IsLine->Strings[i].c_str());
                dbATC_Offset[i+iATC_Use_Heat_Count/2]+=atof(IsLine->Strings[i+(iRow*iCol)].c_str());
                if(Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]>136 ||
                   Temperature.fWorkTemperBase+dbATC_Offset[i+iATC_Use_Heat_Count/2]<20)
                    ret++;
            }
        }
    }

    if(ret==0)
    {
        bGPIBOffsetCommand=true;
        ATC_InterfaceForm->SetOffset(iATC_Use_Heat_Count, dbATC_Offset);        //TransformTcGPIBData, not using
    }
    delete IsLine;                                                              //Steven 20160912 : Add delete for save memory
}
//---------------------------------------------------------------------------
void TfMain::SetOSBIN()                                                         //Sam 20250115 : Add GPIB SETOSBIN_
{
    AnsiString asRet="", asCmd="", szDir="";
    char str[256];
    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();

    if(fBinSel->SetOSBin(atoi(asCmd.c_str())))
        asRet="OK";
    else
        asRet="NG";
    SendMSG_CMD(MSG_CMD_SETOSBIN, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetOSBIN()                                                         //Sam 20250115 : Add GPIB GETOSBIN?
{
    AnsiString asRet="";
    asRet.sprintf("OSBIN_%d", TestIF_File.iOpenBin);
    SendMSG_CMD(MSG_CMD_GETOSBIN, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetDUTCHK()                                                        //Steven 20250701 : for DOOSAN TESNA
{
    AnsiString asRet="";
    asRet+=AnsiString((TestIF_File.iSiteMap[0][0]<10)?TestIF_File.iSiteMap[0][0]:TestIF_File.iSiteMap[0][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][0]<10)?TestIF_File.iSiteMap[1][0]:TestIF_File.iSiteMap[1][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][1]<10)?TestIF_File.iSiteMap[0][1]:TestIF_File.iSiteMap[0][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][1]<10)?TestIF_File.iSiteMap[1][1]:TestIF_File.iSiteMap[1][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][2]<10)?TestIF_File.iSiteMap[0][2]:TestIF_File.iSiteMap[0][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][2]<10)?TestIF_File.iSiteMap[1][2]:TestIF_File.iSiteMap[1][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][3]<10)?TestIF_File.iSiteMap[0][3]:TestIF_File.iSiteMap[0][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][3]<10)?TestIF_File.iSiteMap[1][3]:TestIF_File.iSiteMap[1][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][4]<10)?TestIF_File.iSiteMap[0][4]:TestIF_File.iSiteMap[0][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][4]<10)?TestIF_File.iSiteMap[1][4]:TestIF_File.iSiteMap[1][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][5]<10)?TestIF_File.iSiteMap[0][5]:TestIF_File.iSiteMap[0][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][5]<10)?TestIF_File.iSiteMap[1][5]:TestIF_File.iSiteMap[1][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][6]<10)?TestIF_File.iSiteMap[0][6]:TestIF_File.iSiteMap[0][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][6]<10)?TestIF_File.iSiteMap[1][6]:TestIF_File.iSiteMap[1][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[0][7]<10)?TestIF_File.iSiteMap[0][7]:TestIF_File.iSiteMap[0][7]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[1][7]<10)?TestIF_File.iSiteMap[1][7]:TestIF_File.iSiteMap[1][7]+'A');

    asRet+=AnsiString((TestIF_File.iSiteMap[2][0]<10)?TestIF_File.iSiteMap[2][0]:TestIF_File.iSiteMap[2][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][0]<10)?TestIF_File.iSiteMap[3][0]:TestIF_File.iSiteMap[3][0]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][1]<10)?TestIF_File.iSiteMap[2][1]:TestIF_File.iSiteMap[2][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][1]<10)?TestIF_File.iSiteMap[3][1]:TestIF_File.iSiteMap[3][1]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][2]<10)?TestIF_File.iSiteMap[2][2]:TestIF_File.iSiteMap[2][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][2]<10)?TestIF_File.iSiteMap[3][2]:TestIF_File.iSiteMap[3][2]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][3]<10)?TestIF_File.iSiteMap[2][3]:TestIF_File.iSiteMap[2][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][3]<10)?TestIF_File.iSiteMap[3][3]:TestIF_File.iSiteMap[3][3]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][4]<10)?TestIF_File.iSiteMap[2][4]:TestIF_File.iSiteMap[2][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][4]<10)?TestIF_File.iSiteMap[3][4]:TestIF_File.iSiteMap[3][4]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][5]<10)?TestIF_File.iSiteMap[2][5]:TestIF_File.iSiteMap[2][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][5]<10)?TestIF_File.iSiteMap[3][5]:TestIF_File.iSiteMap[3][5]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][6]<10)?TestIF_File.iSiteMap[2][6]:TestIF_File.iSiteMap[2][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][6]<10)?TestIF_File.iSiteMap[3][6]:TestIF_File.iSiteMap[3][6]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[2][7]<10)?TestIF_File.iSiteMap[2][7]:TestIF_File.iSiteMap[2][7]+'A');
    asRet+=AnsiString((TestIF_File.iSiteMap[3][7]<10)?TestIF_File.iSiteMap[3][7]:TestIF_File.iSiteMap[3][7]+'A');

    SendMSG_CMD(MSG_CMD_DUTCHK, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetFFC()                                                           //Steven 20250701 : for Ampere
{
    AnsiString asRet="";
    if(Temperature.bATC_FFCEnable==true)
        asRet.sprintf("GetFFC 1");
    else
        asRet.sprintf("GetFFC 0");
    SendMSG_CMD(MSG_CMD_GetFFC, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetTJFunction()
{                                   
    AnsiString asRet="";
    if(Temperature.bEnableTJFunction==true)
        asRet.sprintf("GetTJFunction 1");
    else
        asRet.sprintf("GetTJFunction 0");    
    SendMSG_CMD(MSG_CMD_GetTJFunction, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetPowerFollowing()
{
    AnsiString asRet="";
    if(Temperature.bPowerFollower_Enable==true)
        asRet.sprintf("GetPowerFollowing 1");
    else
        asRet.sprintf("GetPowerFollowing 0");
    SendMSG_CMD(MSG_CMD_GetPowerFollowing, asRet);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteHeadContactCount()                                 //Ifor 20240510 add:Report Head Contact Count
{
    AnsiString t="";
    AnsiString asCmd="";
    int iSelect=-1;

    char str[256];

    try
    {
        strncpy(str, HGpib2Handler->cReturn, sizeof(str));
        asCmd=AnsiString(str).Trim();

        iSelect=atoi(asCmd.c_str())-1;
    }
    catch(...)
    {
        asChangeSetupFileName="";
        iSelect=-1;
    }

    if(iSelect<0 || iSelect>=3)
    {
        t.sprintf("Out_Of_Scope\r");
    }
    else
    {
        if(TestIF_File.iTestMode==DualSite)
        {
            t.sprintf("Arm1_%d_%d_Arm2_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][2]);
        }
        else if(TestIF_File.iTestMode==DualSite2x1)
        {
            t.sprintf("Arm1_%d_%d_Arm2_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1]);
        }
        else if(TestIF_File.iTestMode==QualSite1X4 || TestIF_File.iTestMode==_8Site1X4)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][6]);

        }
        else if(TestIF_File.iTestMode==SingleSite)
        {
            t.sprintf("Arm1_%d_Arm2_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][1][0]);
        }
        else if(TestIF_File.iTestMode==QualSite2X2 || TestIF_File.iTestMode==QualSite2X2N)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3]);
        }
        else if(TestIF_File.iTestMode==_6Site2X3)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5]);
        }
        else if(TestIF_File.iTestMode==TriSite1X3)
        {
            t.sprintf("Arm1_%d_%d_%d_Arm2_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][4]);
        }
        else if(TestIF_File.iTestMode==_8Site2X4)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7]);
        }
        else if(TestIF_File.iTestMode==_16Site2X8)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][0][8],
                                        IniConfig.HeadContactCount[iSelect][0][9],
                                        IniConfig.HeadContactCount[iSelect][0][10],
                                        IniConfig.HeadContactCount[iSelect][0][11],
                                        IniConfig.HeadContactCount[iSelect][0][12],
                                        IniConfig.HeadContactCount[iSelect][0][13],
                                        IniConfig.HeadContactCount[iSelect][0][14],
                                        IniConfig.HeadContactCount[iSelect][0][15],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7],
                                        IniConfig.HeadContactCount[iSelect][1][8],
                                        IniConfig.HeadContactCount[iSelect][1][9],
                                        IniConfig.HeadContactCount[iSelect][1][10],
                                        IniConfig.HeadContactCount[iSelect][1][11],
                                        IniConfig.HeadContactCount[iSelect][1][12],
                                        IniConfig.HeadContactCount[iSelect][1][13],
                                        IniConfig.HeadContactCount[iSelect][1][14],
                                        IniConfig.HeadContactCount[iSelect][1][15]);
        }
        else if(TestIF_File.iTestMode==_12Site2X6)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][0][8],
                                        IniConfig.HeadContactCount[iSelect][0][9],
                                        IniConfig.HeadContactCount[iSelect][0][10],
                                        IniConfig.HeadContactCount[iSelect][0][11],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7],
                                        IniConfig.HeadContactCount[iSelect][1][8],
                                        IniConfig.HeadContactCount[iSelect][1][9],
                                        IniConfig.HeadContactCount[iSelect][1][10],
                                        IniConfig.HeadContactCount[iSelect][1][11]);
        }
        else if(TestIF_File.iTestMode==_10Site2X5)
        {
            t.sprintf("Arm1_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_Arm2_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_\r",
                                        IniConfig.HeadContactCount[iSelect][0][0],
                                        IniConfig.HeadContactCount[iSelect][0][1],
                                        IniConfig.HeadContactCount[iSelect][0][2],
                                        IniConfig.HeadContactCount[iSelect][0][3],
                                        IniConfig.HeadContactCount[iSelect][0][4],
                                        IniConfig.HeadContactCount[iSelect][0][5],
                                        IniConfig.HeadContactCount[iSelect][0][6],
                                        IniConfig.HeadContactCount[iSelect][0][7],
                                        IniConfig.HeadContactCount[iSelect][0][8],
                                        IniConfig.HeadContactCount[iSelect][0][9],
                                        IniConfig.HeadContactCount[iSelect][1][0],
                                        IniConfig.HeadContactCount[iSelect][1][1],
                                        IniConfig.HeadContactCount[iSelect][1][2],
                                        IniConfig.HeadContactCount[iSelect][1][3],
                                        IniConfig.HeadContactCount[iSelect][1][4],
                                        IniConfig.HeadContactCount[iSelect][1][5],
                                        IniConfig.HeadContactCount[iSelect][1][6],
                                        IniConfig.HeadContactCount[iSelect][1][7],
                                        IniConfig.HeadContactCount[iSelect][1][8],
                                        IniConfig.HeadContactCount[iSelect][1][9]);
        }
        else
        {
            t.sprintf("UNKNOWN\r");
        }
    }

    SendMSG_CMD(MSG_CMD_GetContactCount, t);
}

//---------------------------------------------------------------------------
void __fastcall TfMain::WriteSetSetupFile()         //Ifor 20231101 add:FTP Function
{
    AnsiString t="";
    AnsiString asRawSetupFileName="";
    bool bMachineHasIC=false;
    bool bDownloadFail=false;

    try         //kevin 20180320 add 避免資料轉換異常
    {
        asChangeSetupFileName=HGpib2Handler->cReturn;
    }
    catch(...)
    {
        asChangeSetupFileName="";
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
        bMachineHasIC=true;

    if(IniConfig.bEnableFTP==true || bMachineHasIC==true)
    {
        t.sprintf("SETNG_%s", asChangeSetupFileName);
    }
    else
    {
        if(asChangeSetupFileName=="")
        {
            t.sprintf("SETNG_%s", asChangeSetupFileName);
        }
        else
        {
            fMain->iHasChangeFile=9;
            asRawSetupFileName=fMain->cbSetupFileName->Text;
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\r", "", TReplaceFlags()<<rfReplaceAll); //Steven 20160512 : 2D log排版
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\n", "", TReplaceFlags()<<rfReplaceAll);
            fMain->cbSetupFileName->Text=asChangeSetupFileName;
            fMain->cbSetupFileNameChange(fMain);

            if(fMain->iHasChangeFile==1)
            {
                t.sprintf("SETOK_%s", fMain->cbSetupFileName->Text);
            }
            else
            {
                t.sprintf("SETNG_%s", asRawSetupFileName);
                bDownloadFail=true;
            }
        }
    }
    fMain->iHasChangeFile=0;
    SendMSG_CMD(MSG_CMD_SetupFileChange, t);
    if(bDownloadFail)
    {
        iChangeFileHasErr=2;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteFTPDownSetupFile()         //Ifor 20231101 add:FTP Function
{
    AnsiString t="";
    AnsiString asRawSetupFileName="";
    bool bMachineHasIC=false;
    bool bDownloadFail=false;

    try         //kevin 20180320 add 避免資料轉換異常
    {
        asChangeSetupFileName=HGpib2Handler->cReturn;
    }
    catch(...)
    {
        asChangeSetupFileName="";
    }

    if(HasICUnderMachine() || HasAnyICInMachine())
        bMachineHasIC=true;

    if(IniConfig.bEnableFTP==false || bMachineHasIC==true)
    {
        t.sprintf("SETNG_%s", asChangeSetupFileName);
    }
    else
    {
        if(asChangeSetupFileName == "")
        {
            t.sprintf("SETNG_%s", asChangeSetupFileName);
        }
        else
        {
            asRawSetupFileName=fMain->cbSetupFileName->Text;
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\r", "", TReplaceFlags()<<rfReplaceAll); //Steven 20160512 : 2D log排版
            asChangeSetupFileName=StringReplace(asChangeSetupFileName, "\n", "", TReplaceFlags()<<rfReplaceAll);

            fFTPClient->bControlByGPIB=true;
            fFTPClient->asSetUpNameByGPIB=asChangeSetupFileName;
            fFTPClient->ShowFTPModal(0);
            fFTPClient->bControlByGPIB=false;

            if(fMain->iHasChangeFile==1)
            {
                t.sprintf("SETOK_%s", fMain->cbSetupFileName->Text);
            }
            else
            {
                t.sprintf("SETNG_%s", asRawSetupFileName);
                bDownloadFail=true;
            }
        }
    }
    fMain->iHasChangeFile=0;
    SendMSG_CMD(MSG_CMD_FTPDownLoad, t);

    if(bDownloadFail)
    {
        iChangeFileHasErr=1;
//        if(fNote->fShow)      //Ifor 20240314 Mark GPIB 通訊會卡住 報警拉到外面處理
//        {
//            iChangeFileHasErr=1;
//        }
//        else
//        {
//            iChangeFileHasErr=0;
//            ShowErrorMessage("WAR1684", K_SKIP, MMSystem, 0, asChangeSetupFileName);     //下載 %s.zip 失敗
//        }
    }
}
//---------------------------------------------------------------------------
void TfMain::GetSocketCounter()
{
    AnsiString asRet,asCmd;

    if(TestIF_File.iTestMode==DualSite2x1)
    {
        asRet.sprintf("%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[1][0]);
    }
    else if(TestIF_File.iTestMode==QualSite2X2 || TestIF_File.iTestMode==QualSite2X2N)
    {
        asRet.sprintf("%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[1][0],LastSet.iSocketContactCount[1][1]);
    }
    else if(TestIF_File.iTestMode==_6Site2X3)
    {
        asRet.sprintf("%d,%d,%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2],LastSet.iSocketContactCount[1][0],LastSet.iSocketContactCount[1][1],LastSet.iSocketContactCount[1][2]);
    }
    else if(TestIF_File.iTestMode==_8Site2X4)
    {
        asRet.sprintf("%d,%d,%d,%d,%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2],LastSet.iSocketContactCount[0][3],LastSet.iSocketContactCount[1][0],LastSet.iSocketContactCount[1][1],LastSet.iSocketContactCount[1][2],LastSet.iSocketContactCount[1][3]);
    }
    else
    {
        if(TestIF_File.iTestMode==SingleSite)
        {
            asRet.sprintf("%d",LastSet.iSocketContactCount[0][0]);
        }
        else if(TestIF_File.iTestMode==DualSite)
        {
            asRet.sprintf("%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1]);
        }
        else if(TestIF_File.iTestMode==TriSite1X3)
        {
            asRet.sprintf("%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2]);
        }
        else if(TestIF_File.iTestMode==QualSite1X4)
        {
            asRet.sprintf("%d,%d,%d,%d",LastSet.iSocketContactCount[0][0],LastSet.iSocketContactCount[0][1],LastSet.iSocketContactCount[0][2],LastSet.iSocketContactCount[0][3]);
        }
        else
        {
            asRet.sprintf("NG");
        }
    }

    SendMSG_CMD(MSG_CMD_GetSocketCounter, asRet);
}
//---------------------------------------------------------------------------
void TfMain::GetTIMCounter()
{
    AnsiString asRet,asCmd;
    int iCh=0;
    char str[256];

    strncpy(str, HGpib2Handler->cReturn, sizeof(str));
    asCmd=AnsiString(str).Trim();
//    iCh=atoi(asCmd);
    iCh=asCmd.ToIntDef(-1)-1;

    if(asCmd!="" && iCh!=-1)
    {
        asRet.sprintf("OK");
        if(TestIF_File.iTestMode==DualSite2x1)
        {
            asRet.sprintf("%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][1]);
        }
        else if(TestIF_File.iTestMode==QualSite2X2)
        {
            asRet.sprintf("%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][1],IniConfig.HeadContactCount[0][iCh][3]);
        }
        else if(TestIF_File.iTestMode==QualSite2X2N)
        {
            asRet.sprintf("%d,%d,%d,%d",IniConfig.HeadContactCount[0][0][0],IniConfig.HeadContactCount[0][0][2],IniConfig.HeadContactCount[0][1][0],IniConfig.HeadContactCount[0][1][2]);   //待驗證
        }
        else if(TestIF_File.iTestMode==_6Site2X3)
        {
            asRet.sprintf("%d,%d,%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4],IniConfig.HeadContactCount[0][iCh][1],IniConfig.HeadContactCount[0][iCh][3],IniConfig.HeadContactCount[0][iCh][5]);
        }
        else if(TestIF_File.iTestMode==_8Site2X4)
        {
            asRet.sprintf("%d,%d,%d,%d,%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4],IniConfig.HeadContactCount[0][iCh][6],IniConfig.HeadContactCount[0][iCh][1],IniConfig.HeadContactCount[0][iCh][3],IniConfig.HeadContactCount[0][iCh][5],IniConfig.HeadContactCount[0][iCh][7]);
        }
        else
        {
            if(TestIF_File.iTestMode==SingleSite)
            {
                asRet.sprintf("%d",IniConfig.HeadContactCount[0][iCh][0]);
            }
            else if(TestIF_File.iTestMode==DualSite)
            {
                asRet.sprintf("%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2]);
            }
            else if(TestIF_File.iTestMode==TriSite1X3)
            {
                asRet.sprintf("%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4]);
            }
            else if(TestIF_File.iTestMode==QualSite1X4)
            {
                asRet.sprintf("%d,%d,%d,%d",IniConfig.HeadContactCount[0][iCh][0],IniConfig.HeadContactCount[0][iCh][2],IniConfig.HeadContactCount[0][iCh][4],IniConfig.HeadContactCount[0][iCh][6]);
            }
            else
            {
                asRet.sprintf("NG");
            }
        }
    }

    SendMSG_CMD(MSG_CMD_GetTIMCounter, asRet);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteMultiZoneTemp()
{
    AnsiString asMultiZone="";

    if(IndexStatus==Z1Down_Z2Up || iGPIBIndexStatus==Z1Down_Z2Up)
    {
        asMultiZone.sprintf("%s_%s_%s_%s;\r\n",
                            asGPIBTempShow[tcAa1],
                            asGPIBTempShow[tcAb1],
                            asGPIBTempShow[tcAc1],
                            asGPIBTempShow[tcAd1]);
    }
    else if(IndexStatus==Z1Up_Z2Down || iGPIBIndexStatus==Z1Up_Z2Down)
    {
        asMultiZone.sprintf("%s_%s_%s_%s;\r\n",
                            asGPIBTempShow[tcAa2],
                            asGPIBTempShow[tcAb2],
                            asGPIBTempShow[tcAc2],
                            asGPIBTempShow[tcAd2]);
    }

    SendMSG_CMD(MSG_CMD_MultiZoneTemp, asMultiZone.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfMain::WriteMultiZoneEnable()
{
    AnsiString asMultiZoneEnable="";

    asMultiZoneEnable.sprintf("%d,%d,%d,%d;\r\n",
                              Temperature.bZoneTempEnable[0],
                              Temperature.bZoneTempEnable[1],
                              Temperature.bZoneTempEnable[2],
                              Temperature.bZoneTempEnable[3]);

    SendMSG_CMD(MSG_CMD_MultiZoneEnable, asMultiZoneEnable.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfMain::ReadWaterValve()
{
    AnsiString asStr="";
    ATC_InterfaceForm->ReadTCWaterValue();
    for(int i=0; i<iATC_Use_Heat_Count; i++)
    {
        asStr+=asReadTCWater[i];
        asStr+=",";
    }
    SendMSG_CMD(MSG_CMD_READ_WATER_VALVE, asStr);
}
//---------------------------------------------------------------------------
void __fastcall TfMain::ReadDynamicPID()
{
    AnsiString asStr="";
    for(int i=0; i<iATC_Use_Heat_Count; i++)
    {
        asStr+=asReadDynamicPID[i];
        asStr+=",";
    }
    SendMSG_CMD(MSG_CMD_READ_DYNAMIC_PID, asStr);
}
//---------------------------------------------------------------------------