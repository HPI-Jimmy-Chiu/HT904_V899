#include "MachineDefine.h"
#pragma hdrstop

#include "cSortCT.h"

#include "cprod.h"
#include "cmydef.h"
#include "MyMotor.h"
#include "uTrayEditForm.h"
#include "cShowBinSelect.h"
#include "main.h"
#include "csystem.h"
#include "cCounterClear.h"
//#include "MachineType.h"
#include "cMyDB.h"
#include "cSocket.h"
#include "cContactCT.h"
#include "common.h"
#include "cObserver.h"
#include "note.h"
#include "uHGemEquipment.h"
#include "uHGemHT9045.h"
#include "atester_ProcessCount.h"
#include "cSecurity.h"
#include "SCK_ART.h"
#include "uLotInfo.h"
#include "uYieldMonitoring.h"
#include "ProductionInfo.h"                                                     //Sam 20170809 (Steven) 移植超豐 OEE 功能 form HT-7045
#include "mymessbox.h"                                                          //Sam 20171213 (Steven) : 超豐良率監控
#include "BarCode.h"
#include "cinitial.h"
#include "database.h"                                                           //Sam 20240604 : 新增 BinDisplay TFT
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSortCT *fSortCT;
//---------------------------------------------------------------------------
void _MyCountPanel::SetObject(TPanel *_pnlCount,    TPanel *_pnlYield,    TLabel *_lblName,
                              TPanel *_pnlCountART, TPanel *_pnlYieldART, TLabel *_lblNameART,
                              int _iTag)
{
    pnlCount        =_pnlCount;
    pnlYield        =_pnlYield;
    lblName         =_lblName;
    pnlCountART     =_pnlCountART;
    pnlYieldART     =_pnlYieldART;
    lblNameART      =_lblNameART;

    iTag            =_iTag;
    pnlCount->Tag   =iTag;
    pnlYield->Tag   =iTag;
    lblName ->Tag   =iTag;
    pnlCountART->Tag=iTag;
    pnlYieldART->Tag=iTag;
    lblNameART->Tag =iTag;
    SetVisible(true);
}
//---------------------------------------------------------------------------
void _MyCountPanel::SetTop(int _iTop)
{
    iTop            =_iTop;
    pnlCount->Top   =iTop+2;
    pnlYield->Top   =iTop+2;
    lblName->Top    =iTop+6;
    pnlCountART->Top=iTop+2;
    pnlYieldART->Top=iTop+2;
    lblNameART->Top =iTop+6;

    lblName->Left    =7;
    pnlYield->Left   =60;
    pnlCount->Left   =116;
    lblNameART->Left =7;
    pnlYieldART->Left=60;
    pnlCountART->Left=116;
}
//---------------------------------------------------------------------------
void _MyCountPanel::SetVisible(bool _bVisible)
{
    bVisible            =_bVisible;
    pnlCount->Visible   =bVisible;
    pnlYield->Visible   =bVisible;
    lblName->Visible    =bVisible;
    pnlCountART->Visible=bVisible;
    pnlYieldART->Visible=bVisible;
    lblNameART->Visible =bVisible;
}
//---------------------------------------------------------------------------
__fastcall TfSortCT::TfSortCT(TComponent* Owner)
    : TForm(Owner)
{
    myCountPanel[eAuto1     ].SetObject(pnlAuto1,  pnlAuto1Yield,  lblAuto1,  pnARTAuto1,  pnARTAuto1Yield,  lblARTAuto1,  eAuto1);
    myCountPanel[eAuto2     ].SetObject(pnlAuto2,  pnlAuto2Yield,  lblAuto2,  pnARTAuto2,  pnARTAuto2Yield,  lblARTAuto2,  eAuto2);
    myCountPanel[eAuto3     ].SetObject(pnlAuto3,  pnlAuto3Yield,  lblAuto3,  pnARTAuto3,  pnARTAuto3Yield,  lblARTAuto3,  eAuto3);
    myCountPanel[eAuto4     ].SetObject(pnlAuto4,  pnlAuto4Yield,  lblAuto4,  pnARTAuto4,  pnARTAuto4Yield,  lblARTAuto4,  eAuto4);
    myCountPanel[eAuto5     ].SetObject(pnlAuto5,  pnlAuto5Yield,  lblAuto5,  pnARTAuto5,  pnARTAuto5Yield,  lblARTAuto5,  eAuto5);
    myCountPanel[eAuto6     ].SetObject(pnlAuto6,  pnlAuto6Yield,  lblAuto6,  pnARTAuto6,  pnARTAuto6Yield,  lblARTAuto6,  eAuto6);
    myCountPanel[eFix1      ].SetObject(pnlFix1,   pnlFix1Yield,   lblFix1,   pnARTFix1,   pnARTFix1Yield,   lblARTFix1,   eFix1);
    myCountPanel[eFix2      ].SetObject(pnlFix2,   pnlFix2Yield,   lblFix2,   pnARTFix2,   pnARTFix2Yield,   lblARTFix2,   eFix2);
    myCountPanel[eFix3      ].SetObject(pnlFix3,   pnlFix3Yield,   lblFix3,   pnARTFix3,   pnARTFix3Yield,   lblARTFix3,   eFix3);
    myCountPanel[eFix4      ].SetObject(pnlFix4,   pnlFix4Yield,   lblFix4,   pnARTFix4,   pnARTFix4Yield,   lblARTFix4,   eFix4);
    myCountPanel[eFix5      ].SetObject(pnlFix5,   pnlFix5Yield,   lblFix5,   pnARTFix5,   pnARTFix5Yield,   lblARTFix5,   eFix5);
    myCountPanel[eFix6      ].SetObject(pnlFix6,   pnlFix6Yield,   lblFix6,   pnARTFix6,   pnARTFix6Yield,   lblARTFix6,   eFix6);
    myCountPanel[eFix7      ].SetObject(pnlFix7,   pnlFix7Yield,   lblFix7,   pnARTFix7,   pnARTFix7Yield,   lblARTFix7,   eFix7);
    myCountPanel[eFix8      ].SetObject(pnlFix8,   pnlFix8Yield,   lblFix8,   pnARTFix8,   pnARTFix8Yield,   lblARTFix8,   eFix8);
    myCountPanel[eFix9      ].SetObject(pnlFix9,   pnlFix9Yield,   lblFix9,   pnARTFix9,   pnARTFix9Yield,   lblARTFix9,   eFix9);
    myCountPanel[eFix10     ].SetObject(pnlFix10,  pnlFix10Yield,  lblFix10,  pnARTFix10,  pnARTFix10Yield,  lblARTFix10,  eFix10);
    myCountPanel[eFix11     ].SetObject(pnlFix11,  pnlFix11Yield,  lblFix11,  pnARTFix11,  pnARTFix11Yield,  lblARTFix11,  eFix11);
    myCountPanel[eFix12     ].SetObject(pnlFix12,  pnlFix11Yield,  lblFix12,  pnARTFix12,  pnARTFix12Yield,  lblARTFix12,  eFix12);
    myCountPanel[eBulkBox   ].SetObject(pnlBinBox, pnlBinBoxYield, lblBinBox, pnARTBinBox, pnARTBinBoxYield, lblARTBinBox, eBulkBox);
    myCountPanel[eMag1      ].SetObject(pnlMag1,   pnlMag1Yield,   lblMag1,   pnARTMag1,   pnARTMag1Yield,   lblARTMag1,   eMag1);
    myCountPanel[eMag2      ].SetObject(pnlMag2,   pnlMag2Yield,   lblMag2,   pnARTMag2,   pnARTMag2Yield,   lblARTMag2,   eMag2);
    myCountPanel[eMag3      ].SetObject(pnlMag3,   pnlMag3Yield,   lblMag3,   pnARTMag3,   pnARTMag3Yield,   lblARTMag3,   eMag3);
    myCountPanel[eMag4      ].SetObject(pnlMag4,   pnlMag4Yield,   lblMag4,   pnARTMag4,   pnARTMag4Yield,   lblARTMag4,   eMag4);
    myCountPanel[eMag5      ].SetObject(pnlMag5,   pnlMag5Yield,   lblMag5,   pnARTMag5,   pnARTMag5Yield,   lblARTMag5,   eMag5);
    myCountPanel[eMag6      ].SetObject(pnlMag6,   pnlMag6Yield,   lblMag6,   pnARTMag6,   pnARTMag6Yield,   lblARTMag6,   eMag6);
    myCountPanel[eMag7      ].SetObject(pnlMag7,   pnlMag7Yield,   lblMag7,   pnARTMag7,   pnARTMag7Yield,   lblARTMag7,   eMag7);
    myCountPanel[eMag8      ].SetObject(pnlMag8,   pnlMag8Yield,   lblMag8,   pnARTMag8,   pnARTMag8Yield,   lblARTMag8,   eMag8);
    myCountPanel[eMag9      ].SetObject(pnlMag9,   pnlMag9Yield,   lblMag9,   pnARTMag9,   pnARTMag9Yield,   lblARTMag9,   eMag9);
    myCountPanel[eMag10     ].SetObject(pnlMag10,  pnlMag10Yield,  lblMag10,  pnARTMag10,  pnARTMag10Yield,  lblARTMag10,  eMag10);
    myCountPanel[eMag11     ].SetObject(pnlMag11,  pnlMag11Yield,  lblMag11,  pnARTMag11,  pnARTMag11Yield,  lblARTMag11,  eMag11);
    myCountPanel[eMag12     ].SetObject(pnlMag12,  pnlMag12Yield,  lblMag12,  pnARTMag12,  pnARTMag12Yield,  lblARTMag12,  eMag12);
    myCountPanel[eMag13     ].SetObject(pnlMag13,  pnlMag13Yield,  lblMag13,  pnARTMag13,  pnARTMag13Yield,  lblARTMag13,  eMag13);
    myCountPanel[eMag14     ].SetObject(pnlMag14,  pnlMag14Yield,  lblMag14,  pnARTMag14,  pnARTMag14Yield,  lblARTMag14,  eMag14);

    for(int i=0; i<eTrayCount; i++)
    {
        if(i!=eBulkBox)
        {
            myCountPanel[i].pnlYield->OnMouseDown=pnlAuto1YieldMouseDown;
            myCountPanel[i].pnlYield->OnDblClick =pnlAuto1DblClick;             //Steven 20240223 : 可以手動清除Count
            myCountPanel[i].pnlCount->OnMouseDown=pnlAuto1YieldMouseDown;       //Steven 20240223 : 補上滑鼠事件
            myCountPanel[i].pnlCount->OnDblClick =pnlAuto1DblClick;
        }
    }
    bShow=false;

    pnlTrayCnt[eAuto1]=pnlAuto1TrayCt;
    pnlTrayCnt[eAuto2]=pnlAuto2TrayCt;
    pnlTrayCnt[eAuto3]=pnlAuto3TrayCt;
    pnlTrayCnt[eAuto4]=pnlAuto4TrayCt;
    pnlTrayCnt[eAuto5]=pnlAuto5TrayCt;
    pnlTrayCnt[eAuto6]=pnlAuto6TrayCt;

    pnlTrayID[eAuto1]=pnlAuto1CID;
    pnlTrayID[eAuto2]=pnlAuto2CID;
    pnlTrayID[eAuto3]=pnlAuto3CID;
    pnlTrayID[eAuto4]=pnlAuto4CID;
    pnlTrayID[eAuto5]=pnlAuto5CID;
    pnlTrayID[eAuto6]=pnlAuto6CID;

    if(USE_COVER_TRAYID==tCID_NFC)
    {
        fSortCT->Width=500;
    }
    else
    {
        fSortCT->Width=272;
        pnlTrayCount->Visible=false;
        pnlLoadTrayCt->Visible=false;
        for(int i=0; i<MAX_AUTO_TRAY; i++)
        {
            pnlTrayCnt[i]->Visible=false;
            pnlTrayID[i]->Visible=false;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::FormShow(TObject *Sender)
{
    bShow=true;

    edLotID->Text=IniConfig.sLotID;                                             //Steven 20140814
    if(CUSTOMER_CODE==CC_ASE_M)                                                 //Steven 20140814 : For ASE-M
    {
        gbLotID->Visible=true;
        Timer1->Enabled=true;
    }

    ShowLoadingIC();
    ShowSortIC();

    if((USE_AUTO_RETEST==eartInstall &&
       (bAutoReTest_ART ||
        IniConfig.bA10_AutoReTest)) ||                                          //kevin 20150615
       CosFunction.bUseARTSortCount)                                            //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
    {
        ShowLoadingIC_ART();
    }
    UpForm();
    ARTSortCount->TabVisible=(CosFunction.bUseSCKART==false &&                  //Steven 20161201 : For SCK 93K ART
                              USE_AUTO_RETEST==eartInstall &&
                              (bAutoReTest_ART ||                               //wei 20160614 alex open
                               IniConfig.bA10_AutoReTest));                     //wei 20150331 打開功能就顯示
    if(CosFunction.bUseARTSortCount)                                            //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
    {
        ARTSortCount->TabVisible=true;
        ARTSortCount->Caption="Auto Sort Count";
    }

    PageControl1->ActivePageIndex=0;
    tsICCount->TabVisible=CosFunction.bShowHPICCount;                           //Steven 20221228 : 計算加熱盤IC數量
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::ShowLoadingIC()
{
    bool bContinuousLoaderAlarm=false;
    pnlLoader->Caption=LastSet.SendCT[0];

    if(iRunStartMode==FT)                                                       //jou 2010-11-17 start :Piggy-Back Functions
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //Steven 20110506 : Auto Site Mapping時不啟動
        {
            LastSet.SendCT[2]=0;
            if((USE_AUTO_RETEST==eartInstall &&
                (bAutoReTest_ART ||
                 IniConfig.bA10_AutoReTest)) ||
                 CosFunction.bUseARTSortCount)                                  //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
            {
                LastSet.SendCT_ART[2]=0;
            }
        }
        else
        {
            if(TestIF.bContinuousLoader==true)
            {
                if(LastSet.SendCT[2]>=int(TestIF.iContinuousLoaderCount))
                {
                    LastSet.SendCT[2]=0;                                        //20141001 wei add
                    if(Prod.iCountAlarmAction==0)                               //20141001 wei add
                    {
                        bContinuousLoaderAlarm=true;
                    }
                    else
                    {
                        iWhoTriggerPiggyBack=pbtContinualLoader;                //Steven 20111207 : 誰觸發了Piggy Back
                        ProcessPiggyBackFunction();                             //Steven 20110725 : 整合成function
                    }
                }
            }
        }
    }
    else if(iRunStartMode==RT)
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //Steven 20110506 : Auto Site Mapping時不啟動
        {
            LastSet.SendCT[2]=0;
        }
        else
        {
            if(TestIF.bContinuousLoader_RT==true)
            {
                if(LastSet.SendCT[2]>=int(TestIF.iContinuousLoaderCount_RT))
                {
                    LastSet.SendCT[2]=0;                                        //20141001 wei add
                    if(Prod.iCountAlarmAction==0)                               //20141001 wei add
                    {
                        bContinuousLoaderAlarm=true;
                    }
                    else
                    {
                        iWhoTriggerPiggyBack=pbtContinualLoader;                //Steven 20111207 : 誰觸發了Piggy Back
                        ProcessPiggyBackFunction();                             //Steven 20110725 : 整合成function
                    }
                }
            }
        }
    }

    if(bContinuousLoaderAlarm)                                                  //20141001 wei add
    {
        ShowErrorMessage("WAR07324", 0, MMInterface, false);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSortCT::ShowLoadingIC_ART()
{
    bool bContinuousLoaderAlarm=false;
    pnlLoadingART->Caption=LastSet.SendCT_ART[0];

    if(iRunStartMode==FT)                                                       //jou 2010-11-17 start :Piggy-Back Functions
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //Steven 20110506 : Auto Site Mapping時不啟動
        {
           LastSet.SendCT_ART[2]=0;
        }
        else
        {
            if(TestIF.bContinuousLoader==true)
            {
                if(LastSet.SendCT_ART[2]>=int(TestIF.iContinuousLoaderCount))
                {
                    LastSet.SendCT_ART[2]=0;                                    //20141001 wei add
                    if(Prod.iCountAlarmAction==0)                               //20141001 wei add
                    {
                        bContinuousLoaderAlarm=true;
                    }
                    else
                    {
                        iWhoTriggerPiggyBack=pbtContinualLoader;                //Steven 20111207 : 誰觸發了Piggy Back
                        ProcessPiggyBackFunction();                             //Steven 20110725 : 整合成function
                    }
                }
            }
        }
    }
    else if(iRunStartMode==RT)
    {
        if(LastSet.iRunStartMode==rsmAutoSiteMap)                               //Steven 20110506 : Auto Site Mapping時不啟動
        {
            LastSet.SendCT_ART[2]=0;
        }
        else
        {
            if(TestIF.bContinuousLoader_RT==true)
            {
                if(LastSet.SendCT_ART[2]>=int(TestIF.iContinuousLoaderCount_RT))
                {
                    LastSet.SendCT_ART[2]=0;                                    //20141001 wei add
                    if(Prod.iCountAlarmAction==0)                               //20141001 wei add
                    {
                        bContinuousLoaderAlarm=true;
                    }
                    else
                    {
                        iWhoTriggerPiggyBack=pbtContinualLoader;                //Steven 20111207 : 誰觸發了Piggy Back
                        ProcessPiggyBackFunction();                             //Steven 20110725 : 整合成function
                    }
                }
            }
        }
    }

    if(bContinuousLoaderAlarm)                                                  //20141001 wei add
    {
        ShowErrorMessage("WAR07324", 0, MMInterface, false);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::ShowSortIC()
{
    unsigned int Sum=0, Sum_ART=0;
    double Sum2=0.0;
    int ipass=0, iFail=0;                                                       //wei 20160923 Secs Gem 回傳Pass/Fail顆數
    int ipass_ATR=0, iFail_ATR=0;                                               //wei 20170119 (Steven) TSMC Secs Gem 回傳Pass/Fail顆數

    for(int i=0; i<eTrayCount; i++)                                             //JerryYang 20220909 : 10->eTrayCount  //kevin 20160819  //kevin 20110901使用FIX分2TRAY
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            TrayCTPanel[i]->Caption=LastSet.BinCT[0][iTo3Unload[i]];
            Sum+=LastSet.BinCT[0][iTo3Unload[i]];
            if((USE_AUTO_RETEST==eartInstall &&
               (bAutoReTest_ART || IniConfig.bA10_AutoReTest)) ||
                CosFunction.bUseARTSortCount)                                   //Ifor 20170315 (wei) add 新增使用ART Sort Count 計數功能
            {
                TrayCTART[i]->Caption=LastSet.BinCT_ART[0][iTo3Unload[i]];
                Sum_ART  +=LastSet.BinCT_ART[0][iTo3Unload[i]];
            }

            if(Prod.iIsPassT6[i]==1)                                            //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6     //wei 20160923 Secs Gem 回傳Pass/Fail顆數
            {
                ipass    +=LastSet.BinCT    [0][iTo3Unload[i]];
                ipass_ATR+=LastSet.BinCT_ART[0][iTo3Unload[i]];                 //wei 20170119 (Steven) TSMC Secs Gem 回傳Pass/Fail顆數
            }
            else
            {
                iFail    +=LastSet.BinCT    [0][iTo3Unload[i]];
                iFail_ATR+=LastSet.BinCT_ART[0][iTo3Unload[i]];                 //wei 20170119 (Steven) TSMC Secs Gem 回傳Pass/Fail顆數
            }
        }
    }
    RunInfo.iUnloadCount=Sum;
    RunInfo.iUnloadCount_ART=Sum_ART;                                           //kevin 20150615 ART
    iSECSGEMPass=ipass;                                                         //wei 20160923 Secs Gem 回傳Pass/Fail顆數
    iSECSGEMFail=iFail;                                                         //wei 20160923 Secs Gem 回傳Pass/Fail顆數

    iATRPassCount[iATRFtRtMode]=ipass_ATR;                                      //wei 20170119 (Steven) TSMC Secs Gem 回傳Pass/Fail顆數
    iATRFailCount[iATRFtRtMode]=iFail_ATR;
    iATRTotalCount[iATRFtRtMode]=Sum_ART;
    fLotInfo->sgATRCount->Cells[iATRFtRtMode+1][1]=iATRPassCount[iATRFtRtMode];
    fLotInfo->sgATRCount->Cells[iATRFtRtMode+1][2]=iATRFailCount[iATRFtRtMode];
    fLotInfo->sgATRCount->Cells[iATRFtRtMode+1][3]=iATRTotalCount[iATRFtRtMode];

    AnsiString AYield="";                                                       //kevin 20170816 (Steven) add 傳送YIELD 給ASE
    ASE_Yield[0]="";

    for(int i=0; i<eTrayCount; i++)                                             //JerryYang 20220909 : 10->eTrayCount  //kevin 20110901使用FIX分2TRAY
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            RunInfo.sT6AutoYield[i]             =ChangeToPercentage(LastSet.BinCT[0][iTo3Unload[i]], Sum);
            myCountPanel[i].pnlYield->Caption   =RunInfo.sT6AutoYield[i];
            myCountPanel[i].pnlCount->Caption   =LastSet.BinCT[0][iTo3Unload[i]];                                       //Steven 20230108 : fixed display
            fTrayYield[i]                       =ChangeToFloat(LastSet.BinCT[0][iTo3Unload[i]], Sum);
            RunInfo.sT6AutoYield_ART[i]         =ChangeToPercentage(LastSet.BinCT_ART[0][iTo3Unload[i]], Sum_ART);
            myCountPanel[i].pnlYieldART->Caption=RunInfo.sT6AutoYield_ART[i];                                           //kevin 20150615 ART
            myCountPanel[i].pnlCountART->Caption=LastSet.BinCT_ART[0][iTo3Unload[i]];                                   //Steven 20230108 : fixed display

            AYield.sprintf("%s=%s,", s6TrayName[i], myCountPanel[i].pnlYield->Caption);                                 //kevin 20170816 (Steven) add 傳送YIELD 給ASE
            ASE_Yield[0]+=AYield;                                               //kevin 20170816 (Steven) add 傳送YIELD 給ASE
            if(NUMBER_PANEL_TYPE==4)                                            //Sam 20240604 : 新增 BinDisplay TFT
                HSys.BinDisCtrl->WriteTargetCount(iTo3Unload[i]+3, LastSet.BinCT[0][iTo3Unload[i]]);
        }
    }

    pnlTotal->Caption   =RunInfo.iUnloadCount;
    pnlTotalART->Caption=RunInfo.iUnloadCount_ART;

    fShowBinSelect->ShowCategoryBin();

    fShowBinSelect->IndexInput->Caption     =LastSet.iIndexInputOutPut[0];      //kevin 20130125
    fShowBinSelect->IndexOut->Caption       =LastSet.iIndexInputOutPut[1];      //kevin 20130125
    fShowBinSelect->OutArm_input->Caption   =LastSet.iIndexInputOutPut[2];      //kevin 20130125
    fShowBinSelect->labInArm_input->Caption =LastSet.iIndexInputOutPut[3];      //jou 2015-08-21 add InArm Pick count

    if(TestIF_File.bLowYieldAlarmByBin)
    {
        Sum2 =ArmData[0]->GetByBinLowYieldPCA();
        Sum2+=ArmData[1]->GetByBinLowYieldPCA();

        pnlYield->Caption   =FormatFloat("0.00%", Sum2/2.0);                    //Steven 20141125
        pnlYieldART->Caption=FormatFloat("0.00%", Sum2/2.0);                    //kevin 20150615
    }

    if(CosFunction.bLowYieldUseContactCounts)                                   //Sam 20221020 : LowYield 改使用 ContactCounts 的資料來計算
        fLotInfo->labLowYieldICCount->Caption=IntToStr(ArmData[2]->GetTotalCT());
    fLotInfo->ShowAMRCategoryBin();                                             //Sam 20240304 : 新增 AMR 功能
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlLoaderMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    #ifndef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_PTI)                                                   //Sam 20220822 : PTI 其明要求 Loader 禁止編輯
        return;
    #endif

    if(Button==mbRight && MOT[MMTrayY].fHasTray)
    {
        if(IniConfig.bI27_ManualSortMode && bRunManualSortMode==true)           //Steven 20150915 : For TSMC 手動整盤功能
        {
            EditTray(MMTrayY, 2);
        }
        else
        {
            if(CosFunction.bUseEditLDTrayNeedManualRemoveTray==true)            //Ifor 20200331 : add Edit Loader Tray Need Manual Remove Tray
            {
                iLDTrayNeedManualRemoveTray=1;                                  //0:Normal 1:Edit Loader Tray 2:Data Change
            }
            EditTray(MMTrayY);
        }
    }
    else if(Button==mbRight && MOT[MMOCR].fHasTray)
    {
        EditTray(MMOCR);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlAuto1YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *Ptr;
    Ptr=(TPanel *)Sender;

    int iTag=Ptr->Tag;

    if(SystemStart)
        return;

    if(IniConfig.bP41UnloadTrayDisableEdit==true)                               //jou 20240403 : Unload Tray Disable Edit
        return;

    if(LastSet.iTester==ON_LINE &&
       USE_TRAY_MAPPING==1 &&
       USE_KEYENCE_EMPTY==3 &&
       TestIF_File.bEnableTrayID2==true)                                        //JerryYang 20250120 : add
    {
        return;
    }

    if(CUSTOMER_CODE==CC_PTI)                                                   //Sam 20231129 : PTI 其明要求 Unloader 禁止編輯
        return;

    if(Button==mbRight && MOT[iMMAuto[iTag]].fHasTray)
        EditTray(iMMAuto[iTag]);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlAuto2YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(IniConfig.bP41UnloadTrayDisableEdit==true)                               //jou 20240403 : Unload Tray Disable Edit
        return;

    if(Button==mbRight && MOT[MMAuto2].fHasTray)
        EditTray(MMAuto2);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlAuto3YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(IniConfig.bP41UnloadTrayDisableEdit==true)                               //jou 20240403 : Unload Tray Disable Edit
        return;

    if(Button==mbRight && MOT[MMAuto3].fHasTray)
        EditTray(MMAuto3);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlFix1YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(IniConfig.bP41UnloadTrayDisableEdit==true)                               //jou 20240403 : Unload Tray Disable Edit
        return;

    if(TestIF_File.iMagFixTrayType==1)                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
    {
        return;
    }

    if(Button==mbRight && MOT[MManualTray1].fHasTray)
        EditTray(MManualTray1);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlFix2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(IniConfig.bP41UnloadTrayDisableEdit==true)                               //jou 20240403 : Unload Tray Disable Edit
        return;

    if(TestIF_File.iMagFixTrayType==1)                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
    {
        return;
    }

    if(Button==mbRight && MOT[MManualTray2].fHasTray)
        EditTray(MManualTray2);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlFix3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(IniConfig.bP41UnloadTrayDisableEdit==true)                               //jou 20240403 : Unload Tray Disable Edit
        return;

    if(TestIF_File.iMagFixTrayType==1)                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
    {
        return;
    }

    if(Button==mbRight && MOT[MManualTray3].fHasTray)
        EditTray(MManualTray3);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::btnClearCountClick(TObject *Sender)
{
    if(SystemStart)
        return;

    //AI(ht9045-v899) 20260605: Sender==btnClearCount 為人工按鈕觸發,程式呼叫(Lot End/SECS/MES)傳入其他物件,後續清除提示僅人工觸發才跳
    bool bManualClear=(Sender==btnClearCount);

    if(CUSTOMER_CODE!=CC_Greatek)                                               //Sam 201700915 (Steven) : 超豐清除不用權限
    {
        if(fSecurity->Insufficient(108)==false)                                 //wei 20151022 Bin Clean Count權限設定
            return;
    }

    if(CUSTOMER_CODE==CC_TSMC_TAINAN && HasICUnderMachine()==true)              //wei 20160923 機台內有IC不能清除 Count
        return;

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20250120 : add
    {
        if(InArmSuck.HasIC()          || OutArmSuck.HasIC()         ||
           ShuttleHasIC()             || IndexHasIC()               ||
           MOT[MInRotateKit].HasIC()  || MOT[MOutRotateKit].HasIC())
        {
            return;
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20191008 : add KYEC 清除 Count 需刷Barcode & 權限
    {
        ReEnterBarcode[0]=false;
        fMain->cbUserSelect->ItemIndex=0;
        AccessLevel=0;
        fMain->ChangeLevelAttr();
        fMain->cbUserSelect->Text="Operator";
        fMain->spbUserName->Caption="Operator";
        fMain->btLogin->Caption="Login";
        if(FileExists(pwPath))
        {
            fMain->cbUserSelectChange(NULL);
        }
        else
        {
            fMain->stOperatorClick(fMain);
        }

        if(AccessLevel<iDefSupervisorLevel)
        {
            return;
        }
    }
    else if(bManualClear && CUSTOMER_CODE!=CC_Greatek)                          //AI(ht9045-v899) 20260605: 僅人工按鈕觸發才提示,程式呼叫不跳詢問避免誤清
    {
        if(ShowMyMessageBox_YES_NO("Clear Sort Count?", "確定要清空計數？")!=1)  //AI(ht9045-v899) 20260605: 改用 ShowMyMessageBox_YES_NO 留下操作紀錄
        {
            return;
        }
        else
        {
            if(SystemStart)                                                     //JerryYang 20180510 (Steven) 加上保護避免Start時清除count
                return;
        }
    }

    if(IniConfig.bEnable_SECS_GEM==true)                                        //Steven 20140528 : Secs Gem
        EventReport(SECS_EVENT.DoClearCount);                                   // 5     按下 Clear Count

    MyDBIProductionData("Clear Sorting Count");                                 //Steven 20140816 : Production Data

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {
        if(IniConfig.bN14_1_EnableOEEFunction==true &&
           IniConfig.iN14_1_OEERecordCycleTime>0)
        {
            fProductionInfo->EachCycleSecondDo_SaveAndUpdateOEEFiles(true);
        }
    }

    fMain->Clarn_Data(8, "btnClearCountClick");
    fContactCT->sgYield->Refresh();                                             //JerryYang 20170224 (wei) 清除後要更新畫面
    if(fSCKART->bShow)                                                          //Steven 20161201 (wei) : For SCK 93K ART
        fSCKART->UpdateCount();

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {
        fProductionInfo->ClearOEECount();
    }
    iallSitCount=0;                                                             //kevin 20180720 (wei) add all site fail count

    if(CosFunction.bBarcodeTrayRecFile==true)                                   //jou 20190930 : Barcode Tray record file
    {
        fBarCode->InitBarcodeRecFile();
    }
}
//---------------------------------------------------------------------------
// fix bin 分割tray更新
//---------------------------------------------------------------------------
void __fastcall TfSortCT::UpForm()
{
    int iTop=2;
    fSortCT->Height=1000;
    pnlUnloadBG->Height=800;

    for(int i=0; i<eTrayCount; i++)
    {
        myCountPanel[i].SetVisible(true);
    }

    myCountPanel[eBulkBox ].SetVisible(iHWFix_BinBox==1);
    myCountPanel[eAuto4   ].SetVisible(AUTO_EMPTY_COLOR>=3);
    myCountPanel[eAuto5   ].SetVisible(AUTO_EMPTY_COLOR>=3);
    myCountPanel[eAuto6   ].SetVisible(AUTO_EMPTY_COLOR>=4);

    for(int i=eMag1; i<=eMag14; i++)                                            //JerryYang 20220909 : add magazine
    {
        myCountPanel[i].SetVisible(AUTO3_IS_MAGAZINE>0);
    }

    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20230907 : For HT-9011UC
    {
        if(CosFunction.bUseTrayUpDownSet)                                       //wei 20160224 TSMC FIX UPDOWN
        {
            myCountPanel[eFix7 ].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix1]);
            myCountPanel[eFix8 ].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix2]);
            myCountPanel[eFix9 ].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix3]);
            myCountPanel[eFix10].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix4]);
            myCountPanel[eFix11].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix5]);
            myCountPanel[eFix12].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix6]);
        }
        else
        {
            myCountPanel[eFix7 ].SetVisible(TrayForm.iFixTrayMode);
            myCountPanel[eFix8 ].SetVisible(TrayForm.iFixTrayMode);
            myCountPanel[eFix9 ].SetVisible(TrayForm.iFixTrayMode);
            myCountPanel[eFix10].SetVisible(TrayForm.iFixTrayMode);
            myCountPanel[eFix11].SetVisible(TrayForm.iFixTrayMode);
            myCountPanel[eFix12].SetVisible(TrayForm.iFixTrayMode);
        }
    }
    else
    {
        for(int i=eFix7; i<=eFix12; i++)
        {
            myCountPanel[i     ].SetVisible(false);
        }

        if(CosFunction.bUseTrayUpDownSet)                                       //wei 20160224 TSMC FIX UPDOWN
        {
            myCountPanel[eFix4 ].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix1]);
            myCountPanel[eFix5 ].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix2]);
            myCountPanel[eFix6 ].SetVisible(TrayForm.iFixTrayMode && TrayForm.bTrayUpDownSet[eFix3]);
        }
        else
        {
            myCountPanel[eFix4 ].SetVisible(TrayForm.iFixTrayMode);
            myCountPanel[eFix5 ].SetVisible(TrayForm.iFixTrayMode);
            myCountPanel[eFix6 ].SetVisible(TrayForm.iFixTrayMode);
        }
    }

    iTop=2;
    for(int i=0; i<eTrayCount; i++)                                             //JerryYang 20220909 : add magazine
    {
        if(myCountPanel[i].bVisible==true)
        {
            myCountPanel[i].SetTop(iTop);
            iTop=iTop+24;
        }
    }
    iVisibleHeight=iTop;
    btnClearCount->Top=iTop;
    pnlUnloadBG->Height=btnClearCount->Top+btnClearCount->Height+10;

    if(PageControl1->ActivePage==SortCount)
    {
        if(gbLotID->Visible==false)
        {
            fSortCT->Height=pnlUnloadBG->Top+pnlUnloadBG->Height+40+btnClearCount->Height;
        }
        else
        {
            fSortCT->Height=gbLotID->Top+gbLotID->Height+40;
        }
    }
    else if(PageControl1->ActivePage==tsICCount)
    {
        fSortCT->Height=120;
    }
    else
    {
        pnlUnloadingARTBG->Height=iVisibleHeight+4;
        fSortCT->Height=pnlUnloadingARTBG->Top+pnlUnloadingARTBG->Height+40;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::spbClearAllCountClick(TObject *Sender)
{
    AnsiString str, Buffer1="";                                                 //Steven 20140814 : Add for ASE_M

    if(SystemStart)
        return;

    if(Application->MessageBox("Clear Sort Count? (確定要清空計數？)", "Confirm", MB_YESNO | MB_TOPMOST)!=IDYES)
        return;

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse &&
           Prod.iTrayType[i]!=tTrayBox)
        {
            if(Buffer1=="")
                str.sprintf("%s, %s", s6TrayName[i], RunInfo.sT6AutoYield[i]);
            else
                str.sprintf(", %s, %s", s6TrayName[i], RunInfo.sT6AutoYield[i]);
            Buffer1=Buffer1+str;
        }
    }

    str.sprintf("Unloading, %d, items, Jam, %d, times, UPH, %d, Yield, %s,", RunInfo.iUnloadCount, LastSet.iJamCount[0], RunInfo.iAvgUPH, Buffer1);
    RecordProcess(str);

    MyDBIProductionData("Clear All Count");

    if(IniConfig.bEnable_SECS_GEM==true)                                        //Steven 20140528 : Secs Gem
        EventReport(SECS_EVENT.DoClearCount);                                   // 5     按下 Clear Count

    fProductionInfo->CalculateNowArmSiteBinQty(true);

    fCounterClear->ClearCount(ctLoadingCounts);
    fCounterClear->ClearCount(ctTraySortCount);
    fCounterClear->ClearCount(ctTesterCategory);                                //jou 2011-07-26 cObserver會出現 over 100% value
    fCounterClear->ClearCount(ctIndexCount);                                    //kevin 20130125 記錄 index吸取ic數量 index放置shuttle 數量

    ShowLoadingIC();
    ShowSortIC();

    for(int k=0; k<3; k++)
    {
        ArmData[k]->ClearALLCT();                                               //2012-01-03    Dell fix 當按下Count Clear,在Tester Category的I/F Error數值錯誤
    }
    fProductionInfo->UpdateControlBinCount(true);                               //Sam 20200525 : Control Bin

    for(int i=0; i<32; i++)
        fYieldMonitoring->bShowSiteYield[i]=false;
    fContactCT->sgYield->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::Timer1Timer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    if(SystemStart==true)                                                       //Steven 20140814
    {
        AnsiString sPath=AuthPath+"config.ini", str="";
        if(edLotID->Text!=IniConfig.sLotID)
        {
            RecordProcess("Lot ID : "+edLotID->Text);
            IniConfig.sLotID=edLotID->Text;
            WriteIniData(sPath, "Count", "Lot ID", IniConfig.sLotID);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::Timer2Timer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    static bool bChange=false;
    bool bChange1;

    bChange1=(iHWFix_BinBox==1 || bCancelErrorBin)?true:false ;                 //kevin 20160824

    if(bChange1!=bChange)
    {
        bChange=bChange1;
        fSortCT->UpForm();                                                      //kevin 20110901 分割fix tray
    }
}
//---------------------------------------------------------------------------
void TfSortCT::IntervalTotalYieldDifference()                                   //20150604 Mylin Interval Total Yield Difference {
{
    if(SystemStart==false)                                                      // 2010.07.05 , Joye
    {
        return;
    }

    if(fAllMotorHome==false || iOneCycle!=0 || iCleanOut!=0)
    {
        return;
    }

    int sum=0;
    int ipass=0;
    int iTotal;
    int iTotalUseSite=0, iYieldDif=0;
    double fYield=0.0;

    if(Prod.bAlarm4EnableIntervalYield==false ||
       Prod.iAlarm4IntervalYieldContinueCount<2)
    {
        return;
    }

    if(bIntervalYield_IsShowAlarm==true)
    {
        if(CUSTOMER_CODE==CC_Greatek)
        {
            ShowErrorMessage("WAR0706", 0, MMInterface);
            ClearIntervalTotalYieldDifferenceCount();                           //20150604 Mylin Interval Total Yield Difference
            bIntervalYield_IsShowAlarm=false;
        }
    }

    for(int i=0; i<eTrayCount; i++)
    {
        sum+=LastSet.BinCT[0][iTo3Unload[i]];
        if(Prod.iIsPassT6[i]==1)                                                //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
            ipass+=LastSet.BinCT[0][iTo3Unload[i]];
    }

    iTotal=sum;

    if(iTotal<iIntervalYield_StartCount)
    {
        iIntervalYield_StartCount=iTotal;
    }

    if(ipass<iIntervalYield_StartPassCount)
    {
        iIntervalYield_StartPassCount=ipass;
    }

    if(iIntervalYield_StartPassCount>iTotal)
    {
        iIntervalYield_StartPassCount=ipass;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j]==true)
            {
                iTotalUseSite++;
            }
        }
    }

    if(iIntervalYield_TotalIntervalCount==0)
    {
        iIntervalYield_StartPassCount=ipass;
        iIntervalYield_StartCount=iTotal;
        iIntervalYield_TotalIntervalCount=iTotalUseSite*Prod.iAlarm4IntervalYieldIntervalCount;
    }
    else
    {
        if(iIntervalYield_TotalIntervalCount!=iTotalUseSite*Prod.iAlarm4IntervalYieldIntervalCount)
        {
            iIntervalYield_TotalIntervalCount=iTotalUseSite*Prod.iAlarm4IntervalYieldIntervalCount;
        }
    }

    iIntervalYield_IntervalCount=iTotal-iIntervalYield_StartCount;

    //數量到達，計算Yield
    bool bRefresh=false;
    if(iTotal>=(iIntervalYield_StartCount+iIntervalYield_TotalIntervalCount) &&
       (iIntervalYield_StartCount+iIntervalYield_TotalIntervalCount)!=0)
    {
        sum=iTotal-iIntervalYield_StartCount;
        if(sum>0)
        {
            fYield=(double)(ipass-iIntervalYield_StartPassCount)*100/sum;

            if(fYield>100)
            {
                fYield=100;
            }

            if(fYield<=0 && (iTotal-ipass)>0)
            {
                fYield=0.01;
            }

            for(int i=Prod.iAlarm4IntervalYieldContinueCount-1-1; i>=0; i--)
            {
                fIntervalYield_YieldHistory[i+1]=fIntervalYield_YieldHistory[i];
            }
            fIntervalYield_YieldHistory[0]=fYield;
            iIntervalYield_IntervalCount=0;
            bRefresh=true;
        }
    }

    if(bRefresh==true)
    {
        bool bIsAlarm=false;
        {
            for(int i=0; i<Prod.iAlarm4IntervalYieldContinueCount-1; i++)
            {
                iYieldDif=0;
                iYieldDif=fIntervalYield_YieldHistory[i+1]-fIntervalYield_YieldHistory[i];

                if(fIntervalYield_YieldHistory[i]!=0 && fIntervalYield_YieldHistory[i+1]!=0)
                {
                    if(Prod.iAlarm4IntervalYieldYield<iYieldDif)
                    {
                        bIsAlarm=true;
                    }
                    else
                    {
                        bIsAlarm=false;
                        break;
                    }
                }
                else
                {
                    bIsAlarm=false;
                    break;
                }
            }
        }

        if(bIsAlarm==true)
        {
            bIntervalYield_IsShowAlarm=true;
        }

        iTotalUseSite=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(LastSet.bUseTestSocket[0][i][j]==true)
                {
                    iTotalUseSite++;
                }
            }
        }

        iIntervalYield_StartPassCount=ipass;
        iIntervalYield_StartCount=iTotal;
        iIntervalYield_TotalIntervalCount=iTotalUseSite*Prod.iAlarm4IntervalYieldIntervalCount;
    }
}
//---------------------------------------------------------------------------
void TfSortCT::ClearIntervalTotalYieldDifferenceCount()                         //20150604 Mylin Interval Total Yield Difference }
{
    iIntervalYield_TotalIntervalCount=0;
    iIntervalYield_IntervalCount=0;
    iIntervalYield_StartCount=0;
    iIntervalYield_StartPassCount=0;
    for(int i=0; i<5; i++)
    {
        fIntervalYield_YieldHistory[i]=0;
    }
    bIntervalYield_IsShowAlarm=false;
}

//---------------------------------------------------------------------------
void TfSortCT::Alarm4Yield()
{
    int sum=0, iTotalUseSite=0, ret=0;
    int iNN=IsNNMode();
    bool bHeadToHeadFlag=false;
    bool bSiteToSiteFlag=false;
    bool bSiteOverFlag=false;
    bool bUseSite[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double fYieldMax=0.0;
    double fYieldMin=100.0;
    double fHeadYieldMax=0.0;
    double fHeadYieldMin=100.0;
    double IndexZ1Yield[MAX_SOCKET_ROW][MAX_SOCKET_COL], IndexZ2Yield[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double fSiteYield[MAX_SOCKET_ROW][MAX_SOCKET_COL], IndexNNYield[MAX_SOCKET_ROW][MAX_SOCKET_COL];                    //Sam 20230306 : 修正 NN mode
    AnsiString YieldMaxPart="", YieldMinPart="", HeadYieldMaxPart1="", HeadYieldMinPart1="", HeadYieldMaxPart2="", HeadYieldMinPart2="", YieldOverPart="", str="";

    if(SystemStart==false)
    {
        return;
    }

    if(fAllMotorHome==false || iOneCycle!=0 || iCleanOut!=0)
    {
        return;
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)                                         //Sam 20220614 : 修正 Alarm4 關 Arm 失效
    {
         for(int j=0; j<MAX_SOCKET_COL; j++)
         {
            IndexZ1Yield[i][j]=0.0;
            IndexZ2Yield[i][j]=0.0;
            fSiteYield[i][j]=0.0;
            IndexNNYield[i][j]=0.0;                                             //Sam 20230306 : 修正 NN mode
            bUseSite[i][j]=false;
         }
    }

    if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(LastSet.bUseTestSocket[0][i][j])
                {
                    IndexZ1Yield[i][j]=ArmData[0]->ArmSKET[i][j]->GetBySitePCA();
                    fSiteYield[i][j]=IndexZ1Yield[i][j];
                    sum+=ArmData[0]->ArmSKET[i][j]->GetTotal();
                    bUseSite[i][j]=true;
                }
            }
        }
    }
    else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(LastSet.bUseTestSocket[1][i][j])
                {
                    IndexZ2Yield[i][j]=ArmData[1]->ArmSKET[i][j]->GetBySitePCA();
                    fSiteYield[i][j]=IndexZ2Yield[i][j];
                    sum+=ArmData[1]->ArmSKET[i][j]->GetTotal();
                    bUseSite[i][j]=true;
                }
            }
        }
    }
    else
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(LastSet.bUseTestSocket[0][i][j] && LastSet.bUseTestSocket[1][i][j])
                {
                    IndexZ1Yield[i][j]=ArmData[0]->ArmSKET[i][j]->GetBySitePCA();
                    IndexZ2Yield[i][j]=ArmData[1]->ArmSKET[i][j]->GetBySitePCA();
                    IndexNNYield[i][j]=ArmData[2]->ArmSKET[i][j]->GetBySitePCA();                                       //Sam 20230306 : 修正 NN mode
                    if(bUseTwoArm32Site==true)
                    {
                        fSiteYield[i][j]=IndexNNYield[i][j];
                    }
                    else
                    {
                        fSiteYield[i][j]=(IndexZ1Yield[i][j]+IndexZ2Yield[i][j])/2.0;
                    }
                    sum+=ArmData[0]->ArmSKET[i][j]->GetTotal()+ArmData[1]->ArmSKET[i][j]->GetTotal();
                    bUseSite[i][j]=true;
                }
            }
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(bUseSite[i][j])
            {
                if(fYieldMax<fSiteYield[i][j])
                {
                    fYieldMax=fSiteYield[i][j];
                }

                if(fYieldMin>fSiteYield[i][j])
                {
                    fYieldMin=fSiteYield[i][j];
                }

                if(TestIF_File.iShuttleMode==0 || (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0))
                {
                    if(fHeadYieldMax<IndexZ1Yield[i][j])
                        fHeadYieldMax=IndexZ1Yield[i][j];

                    if(fHeadYieldMin>IndexZ1Yield[i][j])
                        fHeadYieldMin=IndexZ1Yield[i][j];
                }

                if(TestIF_File.iShuttleMode==0 || (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1))
                {
                    if(fHeadYieldMax<IndexZ2Yield[i][j])
                        fHeadYieldMax=IndexZ2Yield[i][j];

                    if(fHeadYieldMin>IndexZ2Yield[i][j])
                        fHeadYieldMin=IndexZ2Yield[i][j];
                }
            }
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(bUseSite[i][j])
            {
                if(fYieldMax==fSiteYield[i][j])
                {
                    YieldMaxPart+=IndexSuckName[i][j];
                }

                if(fYieldMin==fSiteYield[i][j])
                {
                    YieldMinPart+=IndexSuckName[i][j];
                }

                if(TestIF_File.iShuttleMode==0 || (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0))
                {
                    if(fHeadYieldMax==IndexZ1Yield[i][j])
                    {
                        if(HeadYieldMaxPart1=="")
                            HeadYieldMaxPart1="Arm1";
                        HeadYieldMaxPart1+=IndexSuckName[i+iNN][j];
                    }

                    if(fHeadYieldMin==IndexZ1Yield[i][j])
                    {
                        if(HeadYieldMinPart1=="")
                            HeadYieldMinPart1="Arm1";
                        HeadYieldMinPart1+=IndexSuckName[i+iNN][j];
                    }
                }

                if(TestIF_File.iShuttleMode==0 || (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1))
                {
                    if(fHeadYieldMax==IndexZ2Yield[i][j])
                    {
                        if(HeadYieldMaxPart2=="")
                            HeadYieldMaxPart2="Arm2";
                        HeadYieldMaxPart2+=IndexSuckName[i][j];
                    }

                    if(fHeadYieldMin==IndexZ2Yield[i][j])
                    {
                        if(HeadYieldMinPart2=="")
                            HeadYieldMinPart2="Arm2";
                        HeadYieldMinPart2+=IndexSuckName[i][j];
                    }
                }
            }
        }
    }

    if(sum>=Prod.iSiteToSiteYieldCmpCount && Prod.iSiteToSiteYieldCmpCount!=0)
    {
        if(Prod.bSiteToSiteYieldCmp && Prod.iSiteToSiteYieldCmp!=0)
        {
            if((fYieldMax-fYieldMin)>=Prod.iSiteToSiteYieldCmp)
            {
                bSiteToSiteFlag=true;
            }
        }
    }

    if(bUseTwoArm32Site==true)
    {
    }
    else if(sum>=Prod.iHeadToHeadYieldCmpCount && Prod.iHeadToHeadYieldCmpCount!=0)
    {
        if(Prod.bHeadToHeadYieldCmp && Prod.iHeadToHeadYieldCmp!=0)
        {
            if((fHeadYieldMax-fHeadYieldMin)>=Prod.iHeadToHeadYieldCmp)
            {
                bHeadToHeadFlag=true;
            }
        }
    }

    if(sum>=Prod.iSiteYieldOverAlertCount && Prod.iSiteYieldOverAlertCount!=0)
    {
        if(Prod.bSiteYieldOverAlert==true && Prod.iSiteYieldOverAlert!=0)
        {
            if(fYieldMax>=Prod.iSiteYieldOverAlert)
            {
               bSiteOverFlag=true;
            }

            if(fHeadYieldMax>=Prod.iSiteYieldOverAlert)
            {
                bSiteOverFlag=true;
            }
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j]==true)
            {
                iTotalUseSite++;
            }
        }
    }

    bool bCheckIntervalYield=false;
    if(Prod.bAlarm4ContinueType_Enable && Prod.iAlarm4ContinueType_IntervalCount>0)
    {
        if(sum>=Prod.iAlarm4ContinueType_IntervalCount*iTotalUseSite)
        {
            bCheckIntervalYield=true;

            fProductionInfo->CalculateNowArmSiteBinQty(true);

            for(int k=0; k<3; k++)
            {
                ArmData[k]->ClearALLCT();
            }
            fProductionInfo->UpdateControlBinCount(true);                       //Sam 20200525 : Control Bin
            fContactCT->sgYield->Refresh();
        }
        else
        {
            bSiteToSiteFlag=false;
            bHeadToHeadFlag=false;
            bSiteOverFlag  =false;
        }
    }

    if(bSiteToSiteFlag==true || bHeadToHeadFlag==true || bSiteOverFlag==true)   // 2014.10.29 , Joye , Alarm 4     SystemStart && iOneCycle==0 && iCleanOut==0 && fAllMotorHome==true &&
    {
        if(Prod.bAlarm4ContinueType_Enable==true)
        {
            iAlarm4ContinueType_ContinueCount_Last++;

            if(iAlarm4ContinueType_ContinueCount_Last>=Prod.iAlarm4ContinueType_ContinueCount)
            {
                if(CUSTOMER_CODE==CC_Greatek)
                {
                    ShowErrorMessage("WAR0713", K_RETRY, MMInterface, false);   //JerryYang 20160719 for 超豐,補上yield alarm
                    iAlarm4ContinueType_ContinueCount_Last=0;
                    fProductionInfo->CalculateNowArmSiteBinQty(true);
                    for(int k=0; k<3; k++)
                    {
                        ArmData[k]->ClearALLCT();
                    }
                    fContactCT->sgYield->Refresh();
                }
                iAlarm4ContinueType_ContinueCount_Last=0;                       //JerryYang 20160719
            }
        }
        else
        {
            fNote->RichEdit1->Visible=true;
            fNote->RichEdit1->Clear();
            if(bSiteToSiteFlag)
            {
                str.sprintf("Max %s : %2.2f %s ; Min %s : %2.2f %s ", YieldMaxPart, fYieldMax, "%", YieldMinPart, fYieldMin, "%");
                fNote->RichEdit1->Lines->Add(str.c_str());
                DoLowYieldAlarm("WAR0707", "");                                 //Steven 20180627 (wei) : 整合Low Yield Alarm
            }
            else if(bHeadToHeadFlag)
            {
                str.sprintf("Max %s : %2.2f %s ; Min %s : %2.2f %s", HeadYieldMaxPart1+HeadYieldMaxPart2, fHeadYieldMax, "%", HeadYieldMinPart1+HeadYieldMinPart2, fHeadYieldMin, "%");
                fNote->RichEdit1->Lines->Add(str.c_str());
                DoLowYieldAlarm("WAR0708", "");                                 //Steven 20180627 (wei) : 整合Low Yield Alarm
            }
            else if(bSiteOverFlag)
            {
                str.sprintf("Max %s : %2.2f %s", YieldMaxPart, fYieldMax, "%");
                fNote->RichEdit1->Lines->Add(str.c_str());
                DoLowYieldAlarm("WAR0709", "");                                 //Steven 20180627 (wei) : 整合Low Yield Alarm
            }

            if(ret==K_ONECYCLE)
            {
                fMain->BtnOneCycleClick(fMain);
            }
            else if(ret==K_SKIP)
            {
                fProductionInfo->CalculateNowArmSiteBinQty(true);
                for(int k=0; k<3; k++)
                {
                    ArmData[k]->ClearALLCT();
                }
                fProductionInfo->UpdateControlBinCount(true);                   //Sam 20200525 : Control Bin
                fContactCT->sgYield->Refresh();
            }
            fNote->RichEdit1->Visible=false;
        }
    }
    else if(bCheckIntervalYield==true)
    {
        iAlarm4ContinueType_ContinueCount_Last=0;
    }
}
//---------------------------------------------------------------------------
void TfSortCT::Alarm5Yield()                                                    //Sam 20171213 (Steven) : 超豐良率監控 //Sam 20180423 (wei) : Alarm5Yield Add PreYieldCmp
{
    static int iLowYieldCnt     [MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static int iYieldCmpOverCnt [MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static int iOSAlarmYieldCnt [MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static int iPreYieldCmpCnt[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static int iOldArmDataPass  [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static int iOldArmDataFail  [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    static int iOldArmDataBinCT [2][MAX_SOCKET_ROW][MAX_SOCKET_COL][256];
    static bool bStartRecord=true;
    static bool bfirst=true;
    static double dOldSiteYield [MAX_SOCKET_ROW][MAX_SOCKET_COL];

    int iIndexZOSAlarmCnt       [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    int iIndexZCnt              [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    int iLowYieldCntMax=0, iYieldCmpOverCntMax=0, iOSAlarmYieldCntMax=0, iPreYieldCmpCntMax=0;
    int iIndexZCntMax=0, ret=0;
    int iNN=IsNNMode();
    bool bLowYield=false;
    bool bYieldCmpOver=false;
    bool bOSAlarmYield=false;
    bool bPreYieldCmp=false;
    bool bTestSocket[MAX_SOCKET_ROW][MAX_SOCKET_COL];                           //Sam 20240423 : 修正 NN Mode Alarm5Yield
    double dYieldMax=0.0;
    double dIndexZYield         [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dSiteYield           [MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dIndexZOSAlarmYield  [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dOSAlarmYield        [MAX_SOCKET_ROW][MAX_SOCKET_COL];

    AnsiString s="";
    AnsiString sLowYieldErrPart="", sYieldCmpOverErrPart="", sOSAlarmYieldErrPart="", sPreYieldCmpErrPart="";

    if(SystemStart==false)
    {
        return;
    }

    if(fAllMotorHome==false || iOneCycle!=0 || iCleanOut!=0)
    {
        return;
    }

    if(TestIF_File.bAlarm5_BySiteLowYieldEnable==false &&
       TestIF_File.bAlarm5_BySiteCmpYieldEnable==false &&
       TestIF_File.bAlarm5_BySiteAlarmYieldEnable==false &&
       TestIF_File.bAlarm5_BySitePreCmpYieldEnable==false)                      //Sam 20180423 (wei) : MOFile of Yeild Download
    {
        return;
    }

    ZeroMemory(bTestSocket, sizeof(bTestSocket));

    if(IsNNMode()==NN_2Row)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                bTestSocket[i][j]=LastSet.bUseTestSocket[0][i][j];
            }
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            bTestSocket[0][j]=LastSet.bUseTestSocket[1][0][j];
            bTestSocket[1][j]=LastSet.bUseTestSocket[0][0][j];
        }
    }
    else
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                bTestSocket[i][j]=(LastSet.bUseTestSocket[0][i][j] &&
                                   LastSet.bUseTestSocket[1][i][j])?true:false;
            }
        }
    }

    if(bStartRecord)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(bTestSocket[i][j])                                           //Sam 20240423 : 修正 NN Mode Alarm5Yield
                {
                    for(int index=0; index<2; index++)
                    {
                        iOldArmDataPass[index][i][j]=ArmData[index]->ArmSKET[i][j]->Pass;
                        iOldArmDataFail[index][i][j]=ArmData[index]->ArmSKET[i][j]->Fail;
                        for(int k=0; k<iTestBinCount; k++)
                        {
                            iOldArmDataBinCT[index][i][j][k]=ArmData[index]->ArmSKET[i][j]->GetSelBinCT(k);
                        }
                    }
                }
            }
        }
        bStartRecord=false;
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(bTestSocket[i][j])                                               //Sam 20240423 : 修正 NN Mode Alarm5Yield
            {
                for(int index=0; index<2; index++)
                {
                    iIndexZCnt[index][i][j]=ArmData[index]->ArmSKET[i][j]->GetTotal()-iOldArmDataPass[index][i][j]-iOldArmDataFail[index][i][j];
                    if(iIndexZCnt[index][i][j]>=iIndexZCntMax)                  //Sam 20171213 : 取得最大 Site Contact
                        iIndexZCntMax=iIndexZCnt[index][i][j];
                    if(iIndexZCnt[index][i][j]<0)
                        bStartRecord=true;
                }
            }
        }
    }

    if(TestIF.iTestMode!=_32Site4X8N)
    {
        iIndexZCntMax=iIndexZCntMax*2;
    }

    if(iIndexZCntMax>=TestIF_File.iAlarm5_BySiteIntervalContactCnt &&           //Sam 20171213 : 達到 Site 設定 Contact 數量才檢查
       TestIF_File.iAlarm5_BySiteIntervalContactCnt!=0)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(bTestSocket[i][j])                                           //Sam 20240423 : 修正 NN Mode Alarm5Yield
                {
                    for(int index=0; index<2; index++)                          //Sam 20171213 : 取得 Pass Yield
                    {
                        dIndexZYield[index][i][j]=ChangeToFloat((double)(ArmData[index]->ArmSKET[i][j]->Pass-iOldArmDataPass[index][i][j]), (double)(iIndexZCnt[index][i][j]));  //Steven 20250820 : 針對除以0加上保護
                    }

                    if(TestIF_File.iAlarm5_OSBin!=0)                            //Sam 20171213 : 取得 O/S Alarm Yield
                    {
                        for(int index=0; index<2; index++)
                        {
                            iIndexZOSAlarmCnt[index][i][j]=0;
                            for(int k=0; k<=iTestBinCount; k++)                 //Steven 20230929 : 16 --> iTestBinCount
                            {
                                if(TestIF_File.iAlarm5_OSBin==Prod.iT6PosCate[k])
                                {
                                    iIndexZOSAlarmCnt[index][i][j]+=ArmData[index]->ArmSKET[i][j]->GetSelBinCT(k)-iOldArmDataBinCT[index][i][j][k];
                                }
                            }
                            dIndexZOSAlarmYield[index][i][j]=ChangeToFloat(double(iIndexZOSAlarmCnt[index][i][j]), double(iIndexZCnt[index][i][j]));
                        }
                    }

                    if(bUseTwoArm32Site==true)                                  //Steven 20220418 : NN mode Yield alarm
                    {
                        dSiteYield[i][j]   =dIndexZYield[0][i][j];
                        dOSAlarmYield[i][j]=dIndexZOSAlarmYield[0][i][j];
                    }
                    else
                    {
                        dSiteYield[i][j]   =(dIndexZYield[0][i][j]+dIndexZYield[1][i][j])/2.0;
                        dOSAlarmYield[i][j]=(dIndexZOSAlarmYield[0][i][j]+dIndexZOSAlarmYield[1][i][j])/2.0;
                    }
                }
            }
        }

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(bTestSocket[i][j])                                           //Sam 20240423 : 修正 NN Mode Alarm5Yield
                {
                    if(dYieldMax<=dSiteYield[i][j])
                        dYieldMax=dSiteYield[i][j];
                }
            }
        }

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(bTestSocket[i][j])                                           //Sam 20240423 : 修正 NN Mode Alarm5Yield
                {
                    if(TestIF_File.bAlarm5_BySiteLowYieldEnable &&              //Sam 20171213 : Low Yield 檢查 By Site
                       TestIF_File.dAlarm5_BySiteLowYieldRej!=0 &&
                       TestIF_File.dAlarm5_BySiteLowYield!=0)
                    {
                        if(dSiteYield[i][j]<TestIF_File.dAlarm5_BySiteLowYieldRej)                                      //Low Yeild 過小直接報警
                        {
                            bLowYield=true;
                            sLowYieldErrPart+=IndexSuckName[i+iNN][j]+":"+dSiteYield[i][j]+"%";
                            iLowYieldCnt[i][j]=3;
                        }
                        else if(dSiteYield[i][j]<TestIF_File.dAlarm5_BySiteLowYield)                                    //Low Yeild 異常提示不停機，連續三次才停機
                        {
                            bLowYield=true;
                            sLowYieldErrPart+=IndexSuckName[i+iNN][j]+":"+dSiteYield[i][j]+"%";
                            iLowYieldCnt[i][j]++;
                        }
                        else
                        {
                            iLowYieldCnt[i][j]=0;
                        }
                    }

                    if(TestIF_File.bAlarm5_BySiteCmpYieldEnable &&              //Sam 20171213 : Yield Compare 檢查 By Site
                       TestIF_File.dAlarm5_BySiteCmpYieldRej!=0 &&
                       TestIF_File.dAlarm5_BySiteCmpYield!=0)
                    {
                        if((dYieldMax-dSiteYield[i][j])>TestIF_File.dAlarm5_BySiteCmpYieldRej)                          //Site Yiled Compare 過大直接報警
                        {
                            bYieldCmpOver=true;
                            sYieldCmpOverErrPart+=IndexSuckName[i+iNN][j]+":"+dSiteYield[i][j]+"%";
                            iYieldCmpOverCnt[i][j]=3;
                        }
                        else if((dYieldMax-dSiteYield[i][j])>TestIF_File.dAlarm5_BySiteCmpYield)                        //Site Yiled Compare 異常提示不停機，連續三次才停機
                        {
                            bYieldCmpOver=true;
                            sYieldCmpOverErrPart+=IndexSuckName[i+iNN][j]+":"+dSiteYield[i][j]+"%";
                            iYieldCmpOverCnt[i][j]++;
                        }
                        else
                        {
                            iYieldCmpOverCnt[i][j]=0;
                        }
                    }

                    if(TestIF_File.bAlarm5_BySiteAlarmYieldEnable &&            //Sam 20171213 : O/S Alarm Yield 檢查 By Site
                       TestIF_File.dAlarm5_BySiteAlarmYieldRej!=0 &&
                       TestIF_File.dAlarm5_BySiteAlarmYield!=0)
                    {
                        if(dOSAlarmYield[i][j]>TestIF_File.dAlarm5_BySiteAlarmYieldRej)                                 //OS Alarm Yiled 過大直接報警
                        {
                            bOSAlarmYield=true;
                            sOSAlarmYieldErrPart+=IndexSuckName[i+iNN][j]+":"+dOSAlarmYield[i][j]+"%";
                            iOSAlarmYieldCnt[i][j]=3;
                        }
                        else if(dOSAlarmYield[i][j]>TestIF_File.dAlarm5_BySiteAlarmYield)                               //OS Alarm Yiled 異常提示不停機，連續三次才停機
                        {
                            bOSAlarmYield=true;
                            sOSAlarmYieldErrPart+=IndexSuckName[i+iNN][j]+":"+dOSAlarmYield[i][j]+"%";
                            iOSAlarmYieldCnt[i][j]++;
                        }
                        else
                        {
                            iOSAlarmYieldCnt[i][j]=0;
                        }
                    }

                    if(bfirst==false &&
                       TestIF_File.bAlarm5_BySitePreCmpYieldEnable &&           //Sam 20180423 : 比較前一次的 Yeild 差異。(設定值若是5%，第一次 80，第二次70(需報警)90(不需要))
                       TestIF_File.dAlarm5_BySitePreCmpYieldRej!=0 &&
                       TestIF_File.dAlarm5_BySitePreCmpYield!=0)
                    {
                        if((dOldSiteYield[i][j]-dSiteYield[i][j])>TestIF_File.dAlarm5_BySitePreCmpYieldRej)             //比較前一次 Yiled 變差過大直接報警
                        {
                            bPreYieldCmp=true;
                            sPreYieldCmpErrPart+=IndexSuckName[i+iNN][j]+":"+dSiteYield[i][j]+"%";
                            iPreYieldCmpCnt[i][j]=3;
                        }
                        else if((dOldSiteYield[i][j]-dSiteYield[i][j])>TestIF_File.dAlarm5_BySitePreCmpYield)           //比較前一次 Yiled 異常提示不停機，連續三次才停機
                        {
                            bPreYieldCmp=true;
                            sPreYieldCmpErrPart+=IndexSuckName[i+iNN][j]+":"+dSiteYield[i][j]+"%";
                            iPreYieldCmpCnt[i][j]++;
                        }
                        else
                        {
                            iPreYieldCmpCnt[i][j]=0;
                        }
                    }
                    dOldSiteYield[i][j]=dSiteYield[i][j];
                }
            }
        }

        bfirst=false;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(bTestSocket[i][j])                                           //Sam 20240423 : 修正 NN Mode Alarm5Yield
                {
                    if(iLowYieldCnt[i][j]>=iLowYieldCntMax)
                    {
                        iLowYieldCntMax=iLowYieldCnt[i][j];
                        if(iLowYieldCnt[i][j]>=3)
                        {
                            iLowYieldCnt[i][j]=0;
                        }
                    }

                    if(iYieldCmpOverCnt[i][j]>=iYieldCmpOverCntMax)
                    {
                        iYieldCmpOverCntMax=iYieldCmpOverCnt[i][j];
                        if(iYieldCmpOverCnt[i][j]>=3)
                        {
                            iYieldCmpOverCnt[i][j]=0;
                        }
                    }

                    if(iOSAlarmYieldCnt[i][j]>=iOSAlarmYieldCntMax)
                    {
                        iOSAlarmYieldCntMax=iOSAlarmYieldCnt[i][j];
                        if(iOSAlarmYieldCnt[i][j]>=3)
                        {
                            iOSAlarmYieldCnt[i][j]=0;
                        }
                    }

                    if(iPreYieldCmpCnt[i][j]>=iPreYieldCmpCntMax)
                    {
                        iPreYieldCmpCntMax=iPreYieldCmpCnt[i][j];
                        if(iPreYieldCmpCnt[i][j]>=3)
                        {
                            iPreYieldCmpCnt[i][j]=0;
                        }
                    }
                }
            }
        }

        if(bLowYield || bYieldCmpOver || bOSAlarmYield||bPreYieldCmp)
        {
            if(bLowYield)
            {
                if(iLowYieldCntMax>=3)
                {
                    ret=ShowErrorMessage("WAR0719", K_ONECYCLE, MMInterface, false, sLowYieldErrPart);
                }
                else
                {
                    if(MyMessageBox->Visible==true)
                        MyMessageBox->Close();
                    iUnLoaderCount=8;
                    s.printf("Low Yield Alarm!：%s", sLowYieldErrPart);
                    ShowUnloaderTrayMessage("WAR0701", s);
                    ret=K_SKIP;
                }
            }

            if(bYieldCmpOver)
            {
                if(iYieldCmpOverCntMax>=3)
                {
                    ret=ShowErrorMessage("WAR0718", K_ONECYCLE, MMInterface, false, sYieldCmpOverErrPart);
                }
                else
                {
                    if(MyMessageBox->Visible==true)
                        MyMessageBox->Close();
                    iUnLoaderCount=8;
                    s.printf("Site Yield Different Over Setting!：%s", sYieldCmpOverErrPart);
                    ShowUnloaderTrayMessage("WAR0702", s);
                    ret=K_SKIP;
                }
            }

            if(bOSAlarmYield)
            {
                if(iOSAlarmYieldCntMax>=3)
                {
                    ret=ShowErrorMessage("WAR0717", K_ONECYCLE, MMInterface, false, sOSAlarmYieldErrPart);
                }
                else
                {
                    if(MyMessageBox->Visible==true)
                        MyMessageBox->Close();
                    iUnLoaderCount=8;
                    s.printf("O/S Alarm Yield Warning!：%s", sOSAlarmYieldErrPart);
                    ShowUnloaderTrayMessage("WAR0720", s);
                    ret=K_SKIP;
                }
            }

            if(bPreYieldCmp)
            {
                 if(iPreYieldCmpCntMax>=3)
                {
                    ret=ShowErrorMessage("WAR0722", K_ONECYCLE, MMInterface, false, sOSAlarmYieldErrPart);
                }
                else
                {
                    if(MyMessageBox->Visible==true)
                        MyMessageBox->Close();
                    iUnLoaderCount=8;
                    s.printf("Previous Yield difference Warning!：%s", sOSAlarmYieldErrPart);
                    ShowUnloaderTrayMessage("WAR0721", s);
                    ret=K_SKIP;
                }
            }

            if(ret==K_ONECYCLE)
            {
                fMain->BtnOneCycleClick(fMain);
            }
        }
        bStartRecord=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlHP1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight && MOT[MMPlate1].fHasTray)                               //Steven 20230301 : 右鍵可以看加熱盤IC分布
    {
        EditTray(MMPlate1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlHP2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight && MOT[MMPlate2].fHasTray)                               //Steven 20230301 : 右鍵可以看加熱盤IC分布
    {
        EditTray(MMPlate2);
    }
}
//---------------------------------------------------------------------------
void TfSortCT::CheckTheYieldAfterPlaceAuto()                                    //Sam 20230306 : 修正 Initial Start 誤報警問題
{
    if(CosFunction.bYieldAlarm4)                                                //Sam 20181025 : 移到外面 晶兆成也要 Alarm Head to Head Yield
    {
        if(CUSTOMER_CODE==CC_TERAPOWER && iRunStartMode==RT)                    //Sam 20181114 : 晶兆成 Retest 不要觸發 Yeild Alarm4
        {
        }
        else
        {
            IntervalTotalYieldDifference();                                     //20150604 Mylin Interval Total Yield Difference
            Alarm4Yield();                                                      //20150604 Mylin Interval Total Yield Difference
        }
    }

    if(CosFunction.bYieldAlarm5)
        Alarm5Yield();                                                          //Sam 20171213 (Steven) : 超豐良率監控
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlMag1YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *Ptr;                                                                //JerryYang 20220909 : add magazine
    Ptr=(TPanel *)Sender;
    if(SystemStart)
        return;

    if(Button==mbRight && MOT[iMMgzTray[Ptr->Tag]].fHasTray)
        EditTray(iMMgzTray[Ptr->Tag]);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlMag1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *Ptr;                                                                //JerryYang 20220909 : add magazine
    Ptr=(TPanel *)Sender;

    if(SystemStart)
        return;

    if(Button==mbRight && MOT[iMMgzTray[Ptr->Tag]].fHasTray)
        EditTray(iMMgzTray[Ptr->Tag]);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::lblTotalClick(TObject *Sender)
{
    UpForm();
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlFix4YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(TestIF_File.iMagFixTrayType==1)                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
    {
        return;
    }

    if(Button==mbRight && MOT[MManualTray4].fHasTray)
        EditTray(MManualTray4);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlFix5YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(TestIF_File.iMagFixTrayType==1)                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
    {
        return;
    }

    if(Button==mbRight && MOT[MManualTray5].fHasTray)
        EditTray(MManualTray5);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlFix6YieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    if(TestIF_File.iMagFixTrayType==1)                                          //JerryYang 20221215 : Magazine把fix區當buffer區功能
    {
        return;
    }

    if(Button==mbRight && MOT[MManualTray6].fHasTray)
        EditTray(MManualTray6);
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlAuto1DblClick(TObject *Sender)
{
    TPanel *Ptr;
    Ptr=(TPanel *)Sender;

    if(SystemStart ||
       IniConfig.bO22_ClearSortCntByDoubleClick==false)                         //Steven 20241206 : 點兩下可以清除數量
    {
        return;
    }

    int iTag=Ptr->Tag;
    LastSet.BinCT[0][iTo3Unload[iTag]]=0;                                       //Steven 20240223 : 可以手動清除Count
    WriteLastDataFile();

    myCountPanel[iTag].pnlYield->Caption="0.00%";
    myCountPanel[iTag].pnlCount->Caption=0;
}
//---------------------------------------------------------------------------
void __fastcall TfSortCT::pnlAuto1CIDDblClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TPanel *)Sender, N_NO_SPACE);
}
//---------------------------------------------------------------------------
