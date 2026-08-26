#include "MachineDefine.h"
#pragma hdrstop

#include "cTrayAssignment.h"

#include "common.h"
#include "cprod.h"
#include "cmydef.h"
#include "mymessbox.h"
#include "cSecurity.h"
#include "BarcodeReader.h"
#include "cMyDB.h"
#include "cBuilder.h"
#include "MyMotor.h"
#include "main.h"
#include "uHGemHT9045.h"
#include "csystem.h"
#include "myQwertyKeyBoard.h"
#include "cAuthority.h"
#include "cShowBinSelect.h"
#include "uLotInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTrayAssignment *fTrayAssignment;
//---------------------------------------------------------------------------
__fastcall TfTrayAssignment::TfTrayAssignment(TComponent* Owner)
    : TForm(Owner)
{
    for(int i=0; i<eTrayCount; i++)
        iTrayDirect[i]=0;

    if(bUseAuto2Empty)
    {
        RGLoader->Items->Add("[3]AUTO1");                                       //kevin 20120718新增auto2 氣缸 kyec mft
        RGAuto2->Items->Add("[3]AUTO2");
        rgLoad_RT->Items->Add("[3]AUTO1");                                      //kevin 20120718新增auto2 氣缸 kyec mft
        rgAuto2_RT->Items->Add("[3]AUTO2");
    }

    if(CosFunction.bLoaderTrayToAuto1)
    {
        RGLoader->Items->Add("[3]AUTO1");
        rgLoad_RT->Items->Add("[3]AUTO1");
    }

    chkICSort[0]   =chkICSort1;     chkICSort[1]   =chkICSort2;      chkICSort[2]   =chkICSort3;
    chkICSort[3]   =chkICSort4;     chkICSort[4]   =chkICSort5;      chkICSort[5]   =chkICSort6;
    edtICSort[0][0]=edtICSort1_1;   edtICSort[0][1]=edtICSort1_2;    edtICSort[0][2]=edtICSort1_3;
    edtICSort[0][3]=edtICSort1_4;   edtICSort[0][4]=edtICSort1_5;    edtICSort[0][5]=edtICSort1_6;
    edtICSort[1][0]=edtICSort2_1;   edtICSort[1][1]=edtICSort2_2;    edtICSort[1][2]=edtICSort2_3;
    edtICSort[1][3]=edtICSort2_4;   edtICSort[1][4]=edtICSort2_5;    edtICSort[1][5]=edtICSort2_6;
    edtICSort[2][0]=edtICSort3_1;   edtICSort[2][1]=edtICSort3_2;    edtICSort[2][2]=edtICSort3_3;
    edtICSort[2][3]=edtICSort3_4;   edtICSort[2][4]=edtICSort3_5;    edtICSort[2][5]=edtICSort3_6;

    edtICSort[3][0]=edtICSort4_1;   edtICSort[3][1]=edtICSort4_2;    edtICSort[3][2]=edtICSort4_3;
    edtICSort[3][3]=edtICSort4_4;   edtICSort[3][4]=edtICSort4_5;    edtICSort[3][5]=edtICSort4_6;
    edtICSort[4][0]=edtICSort5_1;   edtICSort[4][1]=edtICSort5_2;    edtICSort[4][2]=edtICSort5_3;
    edtICSort[4][3]=edtICSort5_4;   edtICSort[4][4]=edtICSort5_5;    edtICSort[4][5]=edtICSort5_6;
    edtICSort[5][0]=edtICSort6_1;   edtICSort[5][1]=edtICSort6_2;    edtICSort[5][2]=edtICSort6_3;
    edtICSort[5][3]=edtICSort6_4;   edtICSort[5][4]=edtICSort6_5;    edtICSort[5][5]=edtICSort6_6;
}
//---------------------------------------------------------------------------
void TfTrayAssignment::FixCanUse()
{
    if(IniConfig.bUseFix3==true)
    {
        Prod.iTrayType[eFix3]=tTrayFix;
    }
    else if(AUTO_EMPTY_COLOR>=3)
    {
        Prod.iTrayType[eFix3]=tTrayFix;
    }
    else
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        {
            if(DeviceForm_File.XDimension>15)                                   //kevin 20110916
            {
                Prod.iTrayType[eFix3]=tNotUse;                                  //不使用FIX3TRAY
            }
            else
            {
                Prod.iTrayType[eFix3]=tTrayFix;                                 //使用   //kevin 20110916
            }
        }
        else
        {
            Prod.iTrayType[eFix3]=tNotUse;                                      //不使用FIX3TRAY
        }
    }

    if(CosFunction.bLoaderTrayToAuto1)
    {
        if(TrayForm.LoaderToEmptyColor[iRunStartMode]==2)
            Prod.iTrayType[eAuto1]=tNotUse;
        else
            Prod.iTrayType[eAuto1]=tTrayAuto;
    }

    if(USE_ROTATE==eCynRot)                                                     //kevin 20130812  氣缸版
    {
        if(iRotate_Out_Tray6>=0)
            Prod.iTrayType[iRotate_Out_Tray6]=tNotUse;
    }

    if(MachineTypeChoice==Type_HT9045 &&
       FIX3_FULL_PLACE==Fix3K_UseCylinder &&
       USE_ROTATE==eMotRot)                                                     //JerryYang 20230608 : HT-9045機型使用汽缸FIX板+Rotate時鎖定不得使用FIX3
    {
        Prod.iTrayType[iFixRight    ]=tNotUse;
        Prod.iTrayType[iFixRightHalf]=tNotUse;
    }
    fShowBinSelect->SetAutoVisible();

    if(iHWFix_BinBox==1)
    {
        Prod.iTrayType[eBulkBox     ]=tTrayBox;
    }
    else if(IniConfig.bI33ErrorBinBox)
    {
        Prod.iTrayType[eBulkBox     ]=tTrayBox;
    }
    else
    {
        Prod.iTrayType[eBulkBox     ]=tNotUse;
    }

    //==> Ifor 20260702 : Top&Bottom AOI installed occupies Fix2/Fix3 tray slots, force disable at Tray.Data read time
    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)
    {
        Prod.iTrayType[eFix2]=tNotUse;
        Prod.iTrayType[eFix3]=tNotUse;
        if(TrayForm.iFixTrayMode==1)                                            //Fix Up/Down mode : also disable Fix2/Fix3 lower split slots
        {
            TrayForm.bTrayUpDownSet[eFix2]=false;
            TrayForm.bTrayUpDownSet[eFix3]=false;
            if(AUTO_EMPTY_COLOR>=3)
            {
                Prod.iTrayType[eFix8]=tNotUse;
                Prod.iTrayType[eFix9]=tNotUse;
            }
            else
            {
                Prod.iTrayType[eFix5]=tNotUse;
                Prod.iTrayType[eFix6]=tNotUse;
            }
        }
    }
    //<== Ifor 20260702
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::ReadFile()
{
    AnsiString S="", Str1, Str2;
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\Tray.Data", DataPath, S);
    AnsiString szDir2="D:\\HT9045\\config\\config.ini";

    if(IniConfig.bP46_LoadTrayModeByHandler)                                    //Steven 20221117 : Loader Tray Mode設定跟著機台變
    {
        TrayForm.iManualRemoveLoader   =ReadIniData(szDir2, "Flag",    "Skip Manual Remove Tray",  0);
    }
    else
    {
        TrayForm.iManualRemoveLoader   =ReadIniData(szDir, "Flag",     "Skip Manual Remove Tray",  0);                  //Steven 20101019       //Steven 20130819 : 加入開門檢查的功能
    }

    TrayForm.iRotateKIT_InputType      =CheckRange(ReadIniData(szDir, "Rotation", "Input",  0), 0, 2);
    TrayForm.iRotateKIT_OutputType     =CheckRange(ReadIniData(szDir, "Rotation", "Output", 0), 0, 2);

    if(iRotate_Type!=eCynRotate)                                                //2013-04-12    Dell 太多地方卡這個參數,所以在馬達版先強用一下
        TrayForm.iRotateKIT_InputType  =1;

    TrayForm.LodareType                =ReadIniData(szDir, "Flag",     "Loader Type",   0);

    if(USE_TRAY_MAPPING==1 &&
       TestIF_File.bEnableTrayID2==true &&
       USE_KEYENCE_EMPTY==3)                                                    //JerryYang 20250113 : Color上方TrayID
    {
        TrayForm.LodareType=1;
    }

    if(CosFunction.bUseTrayUpDownSet==false)
    {
        TrayForm.iFixTrayMode=0;
    }
    else
    {
        if(TrayForm.LodareType==0)                                              //jou 2012-09-04 Loader type 選擇 same , Fix tray mode 強制設定為 Full tray
            TrayForm.iFixTrayMode=0;
        else
            TrayForm.iFixTrayMode      =ReadIniData(szDir, "Flag",     "Fix Tray Mode", 0);
    }

    if(TrayForm.iFixTrayMode==1)                                                //wei 20160224 TSMC FIX UPDOWN
    {
        TrayForm.bTrayUpDownSet[eFix1]=ReadIniData(szDir, "Flag",     "Use Fix1 Tray", 0);
        TrayForm.bTrayUpDownSet[eFix2]=ReadIniData(szDir, "Flag",     "Use Fix2 Tray", 0);
        if(Prod.iTrayType[eFix3]==tNotUse)
            TrayForm.bTrayUpDownSet[eFix3]=false;
        else
            TrayForm.bTrayUpDownSet[eFix3]=ReadIniData(szDir, "Flag", "Use Fix3 Tray", 0);

        if(AUTO_EMPTY_COLOR>=3)
        {
            TrayForm.bTrayUpDownSet[eFix4 ]=ReadIniData(szDir, "Flag", "Use Fix4 Tray", 0);
            TrayForm.bTrayUpDownSet[eFix5 ]=ReadIniData(szDir, "Flag", "Use Fix5 Tray", 0);
            TrayForm.bTrayUpDownSet[eFix6 ]=ReadIniData(szDir, "Flag", "Use Fix6 Tray", 0);

            TrayForm.bTrayUpDownSet[eFix7 ]=TrayForm.bTrayUpDownSet[eFix1];
            TrayForm.bTrayUpDownSet[eFix8 ]=TrayForm.bTrayUpDownSet[eFix2];
            TrayForm.bTrayUpDownSet[eFix9 ]=TrayForm.bTrayUpDownSet[eFix3];
            TrayForm.bTrayUpDownSet[eFix10]=TrayForm.bTrayUpDownSet[eFix4];
            TrayForm.bTrayUpDownSet[eFix11]=TrayForm.bTrayUpDownSet[eFix5];
            TrayForm.bTrayUpDownSet[eFix12]=TrayForm.bTrayUpDownSet[eFix6];
        }
        else
        {
            TrayForm.bTrayUpDownSet[eFix4 ]=TrayForm.bTrayUpDownSet[eFix1];
            TrayForm.bTrayUpDownSet[eFix5 ]=TrayForm.bTrayUpDownSet[eFix2];
            TrayForm.bTrayUpDownSet[eFix6 ]=TrayForm.bTrayUpDownSet[eFix3];
        }
    }
    else
    {
        TrayForm.bTrayUpDownSet[eFix1 ]=false;
        TrayForm.bTrayUpDownSet[eFix2 ]=false;
        TrayForm.bTrayUpDownSet[eFix3 ]=false;
        TrayForm.bTrayUpDownSet[eFix4 ]=false;
        TrayForm.bTrayUpDownSet[eFix5 ]=false;
        TrayForm.bTrayUpDownSet[eFix6 ]=false;
        TrayForm.bTrayUpDownSet[eFix7 ]=false;
        TrayForm.bTrayUpDownSet[eFix8 ]=false;
        TrayForm.bTrayUpDownSet[eFix9 ]=false;
        TrayForm.bTrayUpDownSet[eFix10]=false;
        TrayForm.bTrayUpDownSet[eFix11]=false;
        TrayForm.bTrayUpDownSet[eFix12]=false;
    }

    if(AUTO_EMPTY_COLOR>=3)
    {
        Prod.iTrayType[eFix7 ]=(TrayForm.bTrayUpDownSet[eFix1])?tTrayFix:tNotUse;
        Prod.iTrayType[eFix8 ]=(TrayForm.bTrayUpDownSet[eFix2])?tTrayFix:tNotUse;
        Prod.iTrayType[eFix9 ]=(TrayForm.bTrayUpDownSet[eFix3])?tTrayFix:tNotUse;
        Prod.iTrayType[eFix10]=(TrayForm.bTrayUpDownSet[eFix4])?tTrayFix:tNotUse;
        Prod.iTrayType[eFix11]=(TrayForm.bTrayUpDownSet[eFix5])?tTrayFix:tNotUse;
        Prod.iTrayType[eFix12]=(TrayForm.bTrayUpDownSet[eFix6])?tTrayFix:tNotUse;
    }
    else
    {
        Prod.iTrayType[eFix4]=(TrayForm.bTrayUpDownSet[eFix1])?tTrayFix:tNotUse;
        Prod.iTrayType[eFix5]=(TrayForm.bTrayUpDownSet[eFix2])?tTrayFix:tNotUse;
        Prod.iTrayType[eFix6]=(TrayForm.bTrayUpDownSet[eFix3])?tTrayFix:tNotUse;
    }

    if(CosFunction.bUnloadTrayModeByRecipe==false)                              //Steven 20220710 : 甬矽要求Unload Tray Mode by機台設置
    {
        TrayForm.bAutoFeed             =ReadIniData(szDir, "Flag", "Auto Feed",                false);
        TrayForm.bFailAutoTrayManual_FT=ReadIniData(szDir, "Flag", "bFailAutoTrayManual_FT",   false);                  //Steven 20150116 : 手動移除Auto Fail Bin Tray
        TrayForm.bFailAutoTrayManual_RT=ReadIniData(szDir, "Flag", "bFailAutoTrayManual_RT",   false);                  //Steven 20150116 : 手動移除Auto Fail Bin Tray
    }

    if(fMain->hanaART->IsHanaArtAvailable())                                    //Steven 20251007 : for Hana ART
        TrayForm.bAutoFeed          =true;

    //AI(ht9045-v899) 20260817: CC_CYUEAN 固定關閉 Auto Tray Feed, 不受工作檔 Tray.Data 與 hanaART 影響
    if(CosFunction.bDisableAutoTrayFeed)
        TrayForm.bAutoFeed          =false;

    TrayForm.bColorTray             =ReadIniData(szDir, "Flag", "Color Tray Sensor",    false);                         //20140903 wei colcr Tray
    TrayForm.bChkLoadDirection      =ReadIniData(szDir, "Flag", "Check loader tray direction", false);                  //Steven 20190815 : JCET不重測Tray偵測
    TrayForm.bMoveAfterTrayGoOut    =ReadIniData(szDir, "Flag", "MoveAfterTrayGoOut",    false);                        //JerryYang 20241002 : SPIL要求功能 Trray arm等AUTO TRAY退到外面才能移動

    TrayForm.bSpecTrayCnt           =ReadIniData(szDir, "Flag",     "bSpecTrayCnt",    false);                          //JerryYang 20250220 : AUTO IN OUT

    TrayForm.bVTestNoRTBin          =ReadIniData(szDir, "VTest",    "NoRTBin",         false);                          //RogerYang 20250814 : 偉測不可複測bin功能
    TrayForm.asNoRTBinFix[0]        =ReadIniData(szDir, "VTest",    "NoRTBinFix1",     AnsiString(""));
    TrayForm.asNoRTBinFix[1]        =ReadIniData(szDir, "VTest",    "NoRTBinFix2",     AnsiString(""));
    TrayForm.asNoRTBinFix[2]        =ReadIniData(szDir, "VTest",    "NoRTBinFix3",     AnsiString(""));

    TrayForm.iFullTrayCount         =ReadIniData(szDir, "AMR",      "Full Tray Count",    12);
    TrayForm.iInputTrayCount        =ReadIniData(szDir, "AMR",      "Input Tray Count",   12);
    //==> Eastsun 20260512 F009 整合: 補入 AMR section 缺失 wiring (D1.1.1)
    TrayForm.bEnableAMRLoader       =ReadIniData(szDir, "AMR",      "Enahle AMR Loader",  false);
    if(TrayForm.bEnableAMRLoader==true &&
       (fMain->cbRunStartMode->Text=="Re-Test Continuous" || fMain->cbRunStartMode->Text=="Re-Test Initial Start"))
        TrayForm.bEnableAMR=false;                                              //Eastsun 20260512 F011 Phase 6 (a-side L239-L243 Re-Test override)
    else
        TrayForm.bEnableAMR         =ReadIniData(szDir, "AMR",      "Enahle AMR",         false);
    TrayForm.iReaderPos             =ReadIniData(szDir, "AMR",      "Reader Pos",         0);
    TrayForm.iTrayOrder[0]          =ReadIniData(szDir, "AMR",      "1st Tray Type",      0);
    TrayForm.iTrayOrder[1]          =ReadIniData(szDir, "AMR",      "2nd Tray Type",      0);
    TrayForm.iTrayOrder[2]          =ReadIniData(szDir, "AMR",      "3rd Tray Type",      0);
    TrayForm.bIDTrayOrder[0]        =ReadIniData(szDir, "AMR",      "1st ID Use",         false);
    TrayForm.bIDTrayOrder[1]        =ReadIniData(szDir, "AMR",      "2nd ID Use",         false);
    TrayForm.bIDTrayOrder[2]        =ReadIniData(szDir, "AMR",      "3rd ID Use",         false);
    if(TrayForm.iTrayOrder[1]==eNoUse)                                          //Eastsun 20260512 F011 Phase 6 (a-side L249-L254 iAMRCoverTray)
        iAMRCoverTray=1;
    else if(TrayForm.iTrayOrder[2]==eNoUse)
        iAMRCoverTray=2;
    else
        iAMRCoverTray=3;
    //<== Eastsun 20260512 F009 整合

    if(TrayForm.bSpecTrayCnt)                                                   //JerryYang 20250220 : AUTO IN OUT
    {
        iBundleIn=TrayForm.iFullTrayCount;
        iBundleOut=TrayForm.iInputTrayCount;
    }

    if(CUSTOMER_CODE==CC_SCS)                                                   //2013-10-08    Dell Fix
    {
        TrayForm.iManualRemoveLoader=2;
    }
    else if(IniConfig.bSPILFunction==true)                                      //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction  //wei 20150317 新竹矽品 auto skip 強制手動拿Tray
    {
        if(IniConfig.bP44LockLoaderTrayToNone)                                  //JerryYang 20220331 : 矽品中山要求AUTO SKIP後可由TRAY ARM搬TRAY
        {
            TrayForm.iManualRemoveLoader=0;
        }
        else
        {
            TrayForm.iManualRemoveLoader=1;
        }
    }
    else if(CUSTOMER_CODE==CC_SCS || CUSTOMER_CODE==CC_KYEC_XILINX)             //jou 20170413 (Steven) : KYEC Xilinx 要求"When Loader had skip device, need open the door to check the tray."寫為固定選項
    {
        TrayForm.iManualRemoveLoader=2;
        rgLoaderTrayMode->Controls[0]->Enabled=false;
        rgLoaderTrayMode->Controls[1]->Enabled=false;
        rgLoaderTrayMode->Controls[2]->Enabled=false;
    }
    else if(bEnable_KLT_Function==true)
    {
        if(ArmSpeed_File[InArm].bAutoSKIP)                                      //Ifor 20180802 : add KLT Auto SKIP 功能需求
        {
            TrayForm.iManualRemoveLoader=1;
            rgLoaderTrayMode->Controls[0]->Enabled=false;
            rgLoaderTrayMode->Controls[1]->Enabled=false;
            rgLoaderTrayMode->Controls[2]->Enabled=false;
            if(iHasReadeLastData==1)
                iHasReadeLastData=2;
        }
        else
        {
            rgLoaderTrayMode->Controls[0]->Enabled=true;
            rgLoaderTrayMode->Controls[1]->Enabled=true;
            rgLoaderTrayMode->Controls[2]->Enabled=true;
        }
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE)                                         //Ifor 20190703 : add KYEC 強制手動取TRAY
    {
        //==> Eastsun 20260512 F011 Phase 6 (a-side cTrayAssignment.cpp L182-L196 KYEC AMR iManualRemoveLoader + bAutoFeed)
        if(TrayForm.bEnableAMR)                                                 //Eastsun 20260423 (a-side L182)
        {
            TrayForm.iManualRemoveLoader=0;
        }
        else
        {
            TrayForm.iManualRemoveLoader=1;
        }
        rgLoaderTrayMode->Controls[0]->Enabled=false;
        rgLoaderTrayMode->Controls[1]->Enabled=false;
        rgLoaderTrayMode->Controls[2]->Enabled=false;
        TrayForm.bAutoFeed=true;                                                //Eastsun 20260512 F011 Phase 6 (a-side L194 KYEC AMR Auto Feed)
        chkAutoTrayFeed->Enabled=false;
        //<== Eastsun 20260512 F011 Phase 6
    }

    TrayForm.AutoCoverInitial          =ReadIniData(szDir, "Flag",     "Auto Cover Initial", false);
    TrayForm.AutoCoverRetest           =ReadIniData(szDir, "Flag",     "Auto Cover Retest",  false);

    TrayForm.Loader.iTrayType          =ReadIniData(szDir, "Loader",   "Tray Type",     0);
    TrayForm.Loader.Direction          =ReadIniData(szDir, "Loader",   "Direction",     0);
    TrayForm.LoaderToEmptyColor[FT]    =ReadIniData(szDir, "Loader",   "ToBuffer",      0);
    TrayForm.LoaderToEmptyColor[RT]    =ReadIniData(szDir, "Loader",   "ToBuffer_RT",   0);

    if(USE_TRAY_MAPPING==1 &&
       TestIF_File.bEnableTrayID2==true &&
       USE_KEYENCE_EMPTY==3)                                                    //JerryYang 20250113 : Color上方TrayID
    {
        TrayForm.LoaderToEmptyColor[FT]    =0;
        TrayForm.LoaderToEmptyColor[RT]    =0;
    }

    TrayForm.Empty.iTrayType           =ReadIniData(szDir, "Empty",    "Tray Type",     0);
    TrayForm.Color.iTrayType           =ReadIniData(szDir, "Color",    "Tray Type",     0);

    FixCanUse();
    if(USE_TRAY_MAPPING==1 &&
       TestIF_File.bEnableTrayID2==true &&
       USE_KEYENCE_EMPTY==3)                                                    //JerryYang 20250113 : Color上方TrayID
    {
        TrayForm.AutoFromEmptyColor[FT][0] =1;
        TrayForm.AutoFromEmptyColor[RT][0] =1;

        TrayForm.AutoFromEmptyColor[FT][1] =1;
        TrayForm.AutoFromEmptyColor[RT][1] =1;

        TrayForm.AutoFromEmptyColor[FT][2] =1;
        TrayForm.AutoFromEmptyColor[RT][2] =1;
    }

    for(int i=0; i<=iFixRight; i++)
    {
        if(Prod.iTrayType[i]==tTrayBox)
        {
            TrayForm.Auto[i].iTrayType =0;
            TrayForm.Auto[i].Direction =0;
        }
        else if(Prod.iTrayType[i]!=tNotUse)
        {
            TrayForm.Auto[i].iTrayType =ReadIniData(szDir, s6TrayName[i],  "Tray Type",        0);
            TrayForm.Auto[i].Direction =ReadIniData(szDir, s6TrayName[i],  "Direction",        0);
        }
        else if(i<=eAuto6)                                                      //避免沒資料
        {
            TrayForm.Auto[i].iTrayType =TrayForm.Auto[eAuto1].iTrayType;
            TrayForm.Auto[i].Direction =TrayForm.Auto[eAuto1].iTrayType;
        }
        else if(i<=eFix12)
        {
            TrayForm.Auto[i].iTrayType =TrayForm.Auto[eFix1].iTrayType;
            TrayForm.Auto[i].Direction =TrayForm.Auto[eFix1].iTrayType;
        }
        else if(i<=eMag14)
        {
            TrayForm.Auto[i].iTrayType =TrayForm.Auto[iMagAtAuto].iTrayType;
            TrayForm.Auto[i].Direction =TrayForm.Auto[iMagAtAuto].iTrayType;
        }

        if(Prod.iTrayType[i]==tTrayAuto)
        {
            TrayForm.AutoFromEmptyColor[FT][eAuto1+i] =ReadIniData(szDir, s6TrayName[i], "FromBuffer",       0);        //JerryYang 20231001
            TrayForm.AutoFromEmptyColor[RT][eAuto1+i] =ReadIniData(szDir, s6TrayName[i], "FromBuffer_RT",    0);
            if(IniConfig.bP09TrayEndCanSelectTray)                              //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
                TrayForm.Auto[eAuto1+i].TrayEndRecv   =ReadIniData(szDir, s6TrayName[i], "TrayEndRecv",      false);
        }
    }

    if(TrayForm.iFixTrayMode==1)                                               //AI(ht9045-v899) 20260611(CASE-20260611-001): 上面迴圈只讀到iFixRight為止,Fix下半盤(eFix4~6 / eFix7~12)的Direction/iTrayType從未設定,預設停在0導致下半盤(如Fix5)擺放方向與上半盤不一致;此處仿bTrayUpDownSet同步,使下半盤方向/盤型跟隨對應上半盤
    {
        if(AUTO_EMPTY_COLOR>=3)
        {
            TrayForm.Auto[eFix7 ].Direction=TrayForm.Auto[eFix1].Direction;
            TrayForm.Auto[eFix8 ].Direction=TrayForm.Auto[eFix2].Direction;
            TrayForm.Auto[eFix9 ].Direction=TrayForm.Auto[eFix3].Direction;
            TrayForm.Auto[eFix10].Direction=TrayForm.Auto[eFix4].Direction;
            TrayForm.Auto[eFix11].Direction=TrayForm.Auto[eFix5].Direction;
            TrayForm.Auto[eFix12].Direction=TrayForm.Auto[eFix6].Direction;

            TrayForm.Auto[eFix7 ].iTrayType=TrayForm.Auto[eFix1].iTrayType;
            TrayForm.Auto[eFix8 ].iTrayType=TrayForm.Auto[eFix2].iTrayType;
            TrayForm.Auto[eFix9 ].iTrayType=TrayForm.Auto[eFix3].iTrayType;
            TrayForm.Auto[eFix10].iTrayType=TrayForm.Auto[eFix4].iTrayType;
            TrayForm.Auto[eFix11].iTrayType=TrayForm.Auto[eFix5].iTrayType;
            TrayForm.Auto[eFix12].iTrayType=TrayForm.Auto[eFix6].iTrayType;
        }
        else
        {
            TrayForm.Auto[eFix4 ].Direction=TrayForm.Auto[eFix1].Direction;
            TrayForm.Auto[eFix5 ].Direction=TrayForm.Auto[eFix2].Direction;
            TrayForm.Auto[eFix6 ].Direction=TrayForm.Auto[eFix3].Direction;

            TrayForm.Auto[eFix4 ].iTrayType=TrayForm.Auto[eFix1].iTrayType;
            TrayForm.Auto[eFix5 ].iTrayType=TrayForm.Auto[eFix2].iTrayType;
            TrayForm.Auto[eFix6 ].iTrayType=TrayForm.Auto[eFix3].iTrayType;
        }
    }

    if(bUseAuto2Empty==true)                                                    //kevin 20120726 Auto2 送空tray auto3 強至用 auto2空tray
    {
        if(TrayForm.AutoFromEmptyColor[FT][eAuto2]==2)
            TrayForm.AutoFromEmptyColor[FT][eAuto3]=2;

        if(TrayForm.AutoFromEmptyColor[RT][eAuto2]==2)
            TrayForm.AutoFromEmptyColor[RT][eAuto3]=2;
    }
    else                                                                        //jou 2012-11-28 當沒有安裝 Auto 2 Empty 時，必須強制把 TrayForm.AutoFromEmptyColor[RT][2]=2 -> 1
    {
        if(TrayForm.AutoFromEmptyColor[FT][eAuto3]>=2)
            TrayForm.AutoFromEmptyColor[FT][eAuto3]=1;
        if(TrayForm.AutoFromEmptyColor[RT][eAuto3]>=2)
            TrayForm.AutoFromEmptyColor[RT][eAuto3]=1;
    }

    if(IniConfig.bE71_10TrayPitchLockTrayAssign==true)                          //Ifor 20221212 add:KYEC 要求 Tray Form X或Y 小於 10mm 強制開啟
    {
        if((UserDefForm_File[TrayForm.Loader.iTrayType].XPitch*100<=1000 &&     //Eastsun 20260515 F023: E19 skip zero pitch
            UserDefForm_File[TrayForm.Loader.iTrayType].XPitch>0) ||
           (UserDefForm_File[TrayForm.Loader.iTrayType].YPitch*100<=1000 &&
            UserDefForm_File[TrayForm.Loader.iTrayType].YPitch>0) )
        {
            for(int i=0; i<=iAutoRight; i++)
            {
                if(Prod.iTrayType[i]==tTrayAuto)
                {
                    TrayForm.AutoFromEmptyColor[FT][i]=1;
                    TrayForm.AutoFromEmptyColor[RT][i]=1;
                }
            }
            TrayForm.LoaderToEmptyColor[FT]=0;
            TrayForm.LoaderToEmptyColor[RT]=0;
            TrayForm.LodareType=1;                                              //Ifor 20221220 add:需變更為Different才能顯示
        }
    }

    iBackupLoadDir=TrayForm.Loader.Direction;                                   //JerryYang 20201111 iTrayType->Direction  //JerryYang 20200422 Auto skip次數到達後, 自動再去最後一排吸吸看

    if(CosFunction.bTraySortCntFunc)                                            //Steven 20210608 : Tray指定數量退盤功能
    {
        for(int i=0; i<=iAutoRight; i++)
        {
            if(Prod.iTrayType[i]==tTrayAuto)
            {
                TrayForm.bTraySortCntFunc[i]=ReadIniData(szDir, s6TrayName[i],  "bTraySortCntFunc",  false);

                for(int j=0; j<6; j++)
                {
                    Str2.sprintf("TraySortCnt%d_%d", i+1, j+1);
                    TrayForm.iTraySortCntFunc[i][j]=ReadIniData(szDir, s6TrayName[i], Str2, 0);
                }
            }
        }
    }
    if(fLotInfo!=NULL) //Eastsun 20260710 Merge
    {
        if(TrayForm.bEnableAMR)
            fLotInfo->tsKYEC_AMR->TabVisible=true;
        else
            fLotInfo->tsKYEC_AMR->TabVisible=false;
    }

}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::DoIniDataToForm()
{
    cbInputRotateKIT->ItemIndex         =TrayForm.iRotateKIT_InputType;
    cbOutputRotateKIT->ItemIndex        =TrayForm.iRotateKIT_OutputType;

    chkAutoTrayFeed->Checked            =TrayForm.bAutoFeed;
    chkColorTray->Checked               =TrayForm.bColorTray;                   //20140903 wei colcr Tray

    cbMoveAfterTrayGoOut->Checked       =TrayForm.bMoveAfterTrayGoOut;          //JerryYang 20241002 : SPIL要求功能 Trray arm等AUTO TRAY退到外面才能移動

    chkCheckLoadDirection->Checked      =TrayForm.bChkLoadDirection;            //Steven 20190815 : JCET不重測Tray偵測

    cbSpecTrayCnt->Checked              =TrayForm.bSpecTrayCnt;                 //JerryYang 20250220 : AUTO IN OUT

    cbVTestNoRTBin->Checked             =TrayForm.bVTestNoRTBin;                //RogerYang 20250814 : 偉測不可複測bin功能
    edNoRTBinFix1->Text                 =TrayForm.asNoRTBinFix[0];
    edNoRTBinFix2->Text                 =TrayForm.asNoRTBinFix[1];
    edNoRTBinFix3->Text                 =TrayForm.asNoRTBinFix[2];

    rgLoaderTrayMode->ItemIndex         =TrayForm.iManualRemoveLoader;          //Steven 20101019 : Loader有Skip的話，就要手動拿走Loader Tray           //Steven 20130819 : 加入開門檢查的功能
    cbManuTakeAutoFailTray_FT->Checked  =TrayForm.bFailAutoTrayManual_FT;       //Steven 20150116 : 手動移除Auto Fail Bin Tray
    cbManuTakeAutoFailTray_RT->Checked  =TrayForm.bFailAutoTrayManual_RT;       //Steven 20150116 : 手動移除Auto Fail Bin Tray
    rgLoaderType->ItemIndex             =TrayForm.LodareType;
    rgFixTrayMode->ItemIndex            =TrayForm.iFixTrayMode;

    cbLoader->ItemIndex                 =TrayForm.Loader.iTrayType;
    iTrayDirect[0]                      =TrayForm.Loader.Direction;
    RGLoader->ItemIndex                 =TrayForm.LoaderToEmptyColor[FT];
    rgLoad_RT->ItemIndex                =TrayForm.LoaderToEmptyColor[RT];

    cbEmpty->ItemIndex                  =TrayForm.Empty.iTrayType;
    cbColor->ItemIndex                  =TrayForm.Color.iTrayType;

    edAMRFullTrayCount->Text            =TrayForm.iFullTrayCount;
    edAMRInputTrayCount->Text           =TrayForm.iInputTrayCount;              //JerryYang 20250220 : AUTO IN OUT
    //==> Eastsun 20260512 F009 整合: AMR DFM UI binding (D1.1.4)
    cbEnableAMR_KYEC->Checked           =TrayForm.bEnableAMR;
    cbEnableAMRLoader_KYEC->Checked     =TrayForm.bEnableAMRLoader;
    rgAMRReaderPos_KYEC->ItemIndex      =TrayForm.iReaderPos;
    cbAMRTray1st_KYEC->ItemIndex        =TrayForm.iTrayOrder[0];
    cbAMRTray2nd_KYEC->ItemIndex        =TrayForm.iTrayOrder[1];
    cbAMRTray3rd_KYEC->ItemIndex        =TrayForm.iTrayOrder[2];
    cbAMRTrayID1st_KYEC->ItemIndex      =TrayForm.bIDTrayOrder[0];
    cbAMRTrayID2nd_KYEC->ItemIndex      =TrayForm.bIDTrayOrder[1];
    cbAMRTrayID3rd_KYEC->ItemIndex      =TrayForm.bIDTrayOrder[2];
    //<== Eastsun 20260512 F009 整合

    RGAuto1->ItemIndex                  =TrayForm.AutoFromEmptyColor[FT][eAuto1];
    rgAuto1_RT->ItemIndex               =TrayForm.AutoFromEmptyColor[RT][eAuto1];
    iTrayDirect[eAuto1+1]               =TrayForm.Auto[0].Direction;
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        chkTrayEndRecvAuto1->Checked    =TrayForm.Auto[eAuto1].TrayEndRecv;

    RGAuto2->ItemIndex                  =TrayForm.AutoFromEmptyColor[FT][eAuto2];
    rgAuto2_RT->ItemIndex               =TrayForm.AutoFromEmptyColor[RT][eAuto2];
    iTrayDirect[eAuto2+1]               =TrayForm.Auto[eAuto2].Direction;
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        chkTrayEndRecvAuto2->Checked    =TrayForm.Auto[eAuto2].TrayEndRecv;

    RGAuto3->ItemIndex                  =TrayForm.AutoFromEmptyColor[FT][eAuto3];
    rgAuto3_RT->ItemIndex               =TrayForm.AutoFromEmptyColor[RT][eAuto3];
    iTrayDirect[eAuto3+1]               =TrayForm.Auto[eAuto3].Direction;
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        chkTrayEndRecvAuto3->Checked    =TrayForm.Auto[eAuto3].TrayEndRecv;

    RGAuto4->ItemIndex                  =TrayForm.AutoFromEmptyColor[FT][eAuto4];
    rgAuto4_RT->ItemIndex               =TrayForm.AutoFromEmptyColor[RT][eAuto4];
    iTrayDirect[eAuto4+1]               =TrayForm.Auto[eAuto4].Direction;
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        chkTrayEndRecvAuto4->Checked    =TrayForm.Auto[eAuto4].TrayEndRecv;

    RGAuto5->ItemIndex                  =TrayForm.AutoFromEmptyColor[FT][eAuto5];
    rgAuto5_RT->ItemIndex               =TrayForm.AutoFromEmptyColor[RT][eAuto5];
    iTrayDirect[eAuto5+1]               =TrayForm.Auto[eAuto5].Direction;
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        chkTrayEndRecvAuto5->Checked    =TrayForm.Auto[eAuto5].TrayEndRecv;

    RGAuto6->ItemIndex                  =TrayForm.AutoFromEmptyColor[FT][eAuto6];
    rgAuto6_RT->ItemIndex               =TrayForm.AutoFromEmptyColor[RT][eAuto6];
    iTrayDirect[eAuto6+1]               =TrayForm.Auto[eAuto6].Direction;
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        chkTrayEndRecvAuto6->Checked    =TrayForm.Auto[eAuto6].TrayEndRecv;

    if(IniConfig.bBinBox)                                                       //jou 2012-12-11 support Bin Box start
    {
        if(AUTO_EMPTY_COLOR>=3)                                                 //Steven 20230907 : For HT-9011UC
        {
            if(cbFix6->Items->Count<=iBinBoxType)
            {
                cbFix6->Items->Add("Bin Box");
            }
        }
        else
        {
            if(cbFix3->Items->Count<=iBinBoxType)
            {
                cbFix3->Items->Add("Bin Box");
            }
        }
    }

    cbFix1->ItemIndex                   =TrayForm.Auto[eFix1].iTrayType;
    iTrayDirect[eFix1+1]                =TrayForm.Auto[eFix1].Direction;

    cbFix2->ItemIndex                   =TrayForm.Auto[eFix2].iTrayType;
    iTrayDirect[eFix2+1]                =TrayForm.Auto[eFix2].Direction;

    cbFix3->ItemIndex                   =TrayForm.Auto[eFix3].iTrayType;
    iTrayDirect[eFix3+1]                =TrayForm.Auto[eFix3].Direction;

    cbFix4->ItemIndex                   =TrayForm.Auto[eFix4].iTrayType;
    iTrayDirect[eFix4+1]                =TrayForm.Auto[eFix4].Direction;

    cbFix5->ItemIndex                   =TrayForm.Auto[eFix5].iTrayType;
    iTrayDirect[eFix5+1]                =TrayForm.Auto[eFix5].Direction;

    cbFix6->ItemIndex                   =TrayForm.Auto[eFix6].iTrayType;
    iTrayDirect[eFix6+1]                =TrayForm.Auto[eFix6].Direction;

    if(IniConfig.bTrayAssignUseGraphic &&                                       //Steven 20111121 : 使用圖片去顯示Tray Assign
       AUTO_EMPTY_COLOR<3)                                                      //Steven 20230915 : HT9016UC暫時不支援圖像模式
    {
        RadioToGraphic();
        RadioToGraphic_RT();
    }

    if(CosFunction.bUseTrayUpDownSet)                                           //wei 20160224 TSMC FIX UPDOWN
    {
        ckUseFix1->Checked  =TrayForm.bTrayUpDownSet[eFix1];
        ckUseFix2->Checked  =TrayForm.bTrayUpDownSet[eFix2];
        ckUseFix3->Checked  =TrayForm.bTrayUpDownSet[eFix3];
        ckUseFix4->Checked  =TrayForm.bTrayUpDownSet[eFix4];
        ckUseFix5->Checked  =TrayForm.bTrayUpDownSet[eFix5];
        ckUseFix6->Checked  =TrayForm.bTrayUpDownSet[eFix6];
    }

    if(CosFunction.bTraySortCntFunc)                                            //Steven 20210608 : Tray指定數量退盤功能
    {
        if(HasICUnderMachine() || HasAnyICInMachine())
        {
            grpICSort1->Enabled=false;
            grpICSort2->Enabled=false;
            grpICSort3->Enabled=false;
            grpICSort4->Enabled=false;
            grpICSort5->Enabled=false;
            grpICSort6->Enabled=false;
        }
        else
        {
            grpICSort1->Enabled=true;
            grpICSort2->Enabled=true;
            grpICSort3->Enabled=true;
            grpICSort4->Enabled=true;
            grpICSort5->Enabled=true;
            grpICSort6->Enabled=true;
        }

        grpICSortFunc->Visible=true;
        for(int i=0; i<=iAutoRight; i++)
        {
            if(Prod.iTrayType[i]==tTrayAuto)
            {
                chkICSort[i]->Checked=TrayForm.bTraySortCntFunc[i];

                for(int j=0; j<6; j++)
                    edtICSort[i][j]->Text=TrayForm.iTraySortCntFunc[i][j];
            }
        }
    }
    else
    {
        grpICSortFunc->Visible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::LoadImage()
{
    imgLoader->Picture->LoadFromFile(BmpPath+"type0.bmp");

    imgAuto1->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgAuto2->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgAuto3->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgAuto4->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgAuto5->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgAuto6->Picture->LoadFromFile(BmpPath+"type0.bmp");

    imgFix1->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgFix2->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgFix3->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgFix4->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgFix5->Picture->LoadFromFile(BmpPath+"type0.bmp");
    imgFix6->Picture->LoadFromFile(BmpPath+"type0.bmp");
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::FormShow(TObject *Sender)
{
    AnsiString S;
    LoadImage();
    ReadFile();
    DoIniDataToForm();
    ShowCompnet();
    S.sprintf("Tray Assignment  ''%s''  ",GetLastOpenFN());
    Caption=S;
    ShowTrayDirectIMG();

    if(USE_ROTATE_KIT==1)
        gbRotateKit->Visible=(iRotate_Type==eCynRotate);

    Left=75;                                                                    //Steven 20091103
    Top=10;                                                                     //Steven 20091103
    rgFixTrayMode->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex);
    gbColorTrayMode->Visible=(USE_COLOR_TRAY_SENSOR==1);                        //20140903 wei colcr Tray

    if(CosFunction.bSpecialP24)                                                 //Sam 20230320 : 整合 P24 北興版為獨立功能碼
    {
        RGLoader->ItemIndex=0;
        RGLoader->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
        lblHint->Visible=IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
    }
    else
    {
        lblHint->Visible=false;
    }

    if(IniConfig.bP18FailAutoTrayManual==true)                                  //ChungHung 20120329 FailBin AutoTray Manual
    {
        if(Prod.iIsFailT6[eAuto1]==1)                                           //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
            RGAuto1->Enabled=false;
        else
            RGAuto1->Enabled=true;

        if(Prod.iIsFailT6[eAuto2]==1)                                           //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
            RGAuto2->Enabled=false;
        else
            RGAuto2->Enabled=true;

        if(Prod.iIsFailT6[eAuto3]==1)                                           //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
            RGAuto3->Enabled=false;
        else
            RGAuto3->Enabled=true;
    }
    else
    {
        RGAuto1->Enabled=true;
        RGAuto2->Enabled=true;
        RGAuto3->Enabled=true;
    }

    if((bUseAuto2Empty || CosFunction.bLoaderTrayToAuto1) && RGLoader->ItemIndex==2)                                    //kevin 20120718 判別 auto1 是否有設 bin別
        RGAuto1->Enabled=false;
    else
        RGAuto1->Enabled=true;

    if(bUseAuto2Empty && RGAuto2->ItemIndex==2)                                 //kevin 20120718 判別 auto1 是否有設 bin別
        RGAuto3->Enabled=false;
    else
        RGAuto3->Enabled=true;

    if((bUseAuto2Empty || CosFunction.bLoaderTrayToAuto1) && rgLoad_RT->ItemIndex==2)                                   //kevin 20120718 判別 auto1 是否有設 bin別
        rgAuto1_RT->Enabled=false;
    else
        rgAuto1_RT->Enabled=true;

    if(bUseAuto2Empty && rgAuto2_RT->ItemIndex==2)                              //kevin 20120718 判別 auto1 是否有設 bin別
        rgAuto3_RT->Enabled=false;
    else
        rgAuto3_RT->Enabled=true;

    if(USE_ROTATE_KIT==1)
        gbRotateKit->Visible=(iRotate_Type==eCynRotate);

    if(CosFunction.bSpecialP24)                                                 //Sam 20230320 : 整合 P24 北興版為獨立功能碼
    {
        RGLoader->ItemIndex=0;
        RGLoader->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
        RGAuto1->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;   //Sam 20231129 : 力成也要矽格北興P24功能
        RGAuto2->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
        RGAuto3->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
        lblHint->Visible=IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;

        rgLoad_RT->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;                                         //Sam 20231129 : 力成也要矽格北興P24功能
        rgAuto1_RT->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
        rgAuto2_RT->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
        rgAuto3_RT->Enabled=!IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
        lblHint2->Visible=IniConfig.bP24SkipEventNeedRemoveEmptyAndColorTray;
    }
    else
    {
        lblHint->Visible=false;
        lblHint2->Visible=false;                                                //Sam 20231129 : 力成也要矽格北興P24功能
    }

    if(USE_TRAY_MAPPING==1 &&
       TestIF_File.bEnableTrayID2==true &&
       USE_KEYENCE_EMPTY==3)                                                    //JerryYang 20250113 : Color上方TrayID
    {
            rgLoaderType->Enabled=false;
            RGLoader->Enabled=false;
            rgLoad_RT->Enabled=false;

            RGAuto1->Enabled=false;
            RGAuto2->Enabled=false;
            RGAuto3->Enabled=false;

            rgAuto1_RT->Enabled=false;
            rgAuto2_RT->Enabled=false;
            rgAuto3_RT->Enabled=false;
    }

    if(fMain->hanaART->IsHanaArtAvailable())                                    //Steven 20251007 : for Hana ART
        chkAutoTrayFeed->Enabled        =false;

    //AI(ht9045-v899) 20260817: CC_CYUEAN 固定關閉 Auto Tray Feed, 取消勾選並鎖定(只鎖這一顆, 同群組其他兩項不動)
    if(CosFunction.bDisableAutoTrayFeed)
    {
        chkAutoTrayFeed->Checked        =false;
        chkAutoTrayFeed->Enabled        =false;
    }

    if(CUSTOMER_CODE==CC_SCC ||                                                 //Steven 20101221
       CUSTOMER_CODE==CC_SCK)                                                   //ChungHung 20130621 add SCK RMS
    {
        if(IniConfig.bEnableRms && AccessLevel<=iDefEngineerLevel)              //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel
        {
            palTrayAssign->Enabled      =false;
            rgLoaderTrayMode->Enabled   =false;                                 //Steven 20130819 : 加入開門檢查的功能
            gbUnloadMode->Enabled       =false;
        }
        else if(AccessLevel<LevelSet.AccessLevel[16])
        {
            palTrayAssign->Enabled      =false;
            rgLoaderTrayMode->Enabled   =false;
            gbUnloadMode->Enabled       =false;
        }
        else
        {
            palTrayAssign->Enabled      =true;
            rgLoaderTrayMode->Enabled   =true;                                  //Steven 20130819 : 加入開門檢查的功能
            gbUnloadMode->Enabled       =true;
        }
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE ||                                       //20140320 wei   KYEC 低於權限顯示不能修改
            CUSTOMER_CODE==CC_KYEC_XILINX)
    {
        if(AccessLevel<LevelSet.AccessLevel[16])
        {
            palTrayAssign->Enabled      =false;
            rgLoaderTrayMode->Enabled   =false;
            gbUnloadMode->Enabled       =false;
        }
        else
        {
            palTrayAssign->Enabled      =true;
            rgLoaderTrayMode->Enabled   =true;
            gbUnloadMode->Enabled       =true;
        }
    }
    else if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)         //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        if(bAuthCriticalPara[20])
        {
            rgLoaderTrayMode->Enabled   =false;
            palTrayAssign->Enabled      =false;
            gbUnloadMode->Enabled       =false;
        }
        else
        {
            if(AccessLevel<LevelSet.AccessLevel[16])
                rgLoaderTrayMode->Enabled=false;
            else
                rgLoaderTrayMode->Enabled=fSecurity->Insufficient(102, false);
        }
    }
    else
    {
        if(CUSTOMER_CODE==CC_SCS)                                               //Steven 20130819 : Chew要求要直接設定,不能改  //2013-09-28 Dell modify for CC_AMKOR_Korea
        {
            rgLoaderTrayMode->Enabled=false;
        }
        else                                                                    //Steven 20150416 : 新的權限設定
        {
            if(AccessLevel<LevelSet.AccessLevel[16])
                rgLoaderTrayMode->Enabled=false;
            else
                rgLoaderTrayMode->Enabled=fSecurity->Insufficient(102, false);
        }

        palTrayAssign->Enabled=fSecurity->Insufficient(16, false);
        gbUnloadMode->Enabled =fSecurity->Insufficient(16, false);
    }

    if(CosFunction.bUnloadTrayModeByRecipe==true)                               //Steven 20220710 : 甬矽要求Unload Tray Mode by機台設置
    {
        gbUnloadMode->Enabled=false;
    }

    chkTrayEndRecvAuto1->Visible=IniConfig.bP09TrayEndCanSelectTray;            //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
    chkTrayEndRecvAuto2->Visible=IniConfig.bP09TrayEndCanSelectTray;            //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
    chkTrayEndRecvAuto3->Visible=IniConfig.bP09TrayEndCanSelectTray;            //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
    chkTrayEndRecvAuto4->Visible=IniConfig.bP09TrayEndCanSelectTray;
    chkTrayEndRecvAuto5->Visible=IniConfig.bP09TrayEndCanSelectTray;
    chkTrayEndRecvAuto6->Visible=IniConfig.bP09TrayEndCanSelectTray;

    rbTemp->SetFocus();

    labFix1->Visible=(MOT[MManualTray1].TrayFeedHasIC());                       //wei 20160420 Fix盤要沒有IC才能上下盤切換
    labFix2->Visible=(MOT[MManualTray2].TrayFeedHasIC());                       //wei 20160420 Fix盤要沒有IC才能上下盤切換
    labFix3->Visible=(MOT[MManualTray3].TrayFeedHasIC());                       //wei 20160420 Fix盤要沒有IC才能上下盤切換
    labFix4->Visible=(MOT[MManualTray4].TrayFeedHasIC());
    labFix5->Visible=(MOT[MManualTray5].TrayFeedHasIC());
    labFix6->Visible=(MOT[MManualTray6].TrayFeedHasIC());
    ckUseFix1->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray1].TrayFeedHasIC()==false);  //wei 20160420 Fix盤要沒有IC才能上下盤切換
    ckUseFix2->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray2].TrayFeedHasIC()==false);  //wei 20160420 Fix盤要沒有IC才能上下盤切換
    ckUseFix3->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray3].TrayFeedHasIC()==false);  //wei 20160420 Fix盤要沒有IC才能上下盤切換
    ckUseFix4->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray4].TrayFeedHasIC()==false);
    ckUseFix5->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray5].TrayFeedHasIC()==false);
    ckUseFix6->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray6].TrayFeedHasIC()==false);

    fShow=true;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::imgLoaderClick(TObject *Sender)
{
    TImage *Ptr;
    char Dir[256];
    Ptr=(TImage *)Sender;
    int dir=iTrayDirect[Ptr->Tag];
    dir++;
    if(dir>=8)
        dir=0;
    iTrayDirect[Ptr->Tag]=dir;
    sprintf(Dir, "%stype%d.bmp", BmpPath, dir);
    try
    {
        Ptr->Picture->LoadFromFile(Dir);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "imgLoaderClick");
        ShowMyMessage("Path :D:\\HT9045\\IMG\\BMP\\ be delete");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::ShowCompnet()
{
    gbEmptyTray->Visible=rgLoaderType->ItemIndex;
    gbColorTray->Visible=rgLoaderType->ItemIndex;

    if(IniConfig.bP09TrayEndCanSelectTray==false)                               //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
    {
        gbAuto1->Visible=rgLoaderType->ItemIndex;
        gbAuto2->Visible=rgLoaderType->ItemIndex;
        gbAuto3->Visible=rgLoaderType->ItemIndex;
        gbAuto4->Visible=rgLoaderType->ItemIndex;
        gbAuto5->Visible=rgLoaderType->ItemIndex;
        gbAuto6->Visible=rgLoaderType->ItemIndex;
    }
    else
    {
        gbAuto1->Visible=true;
        gbAuto2->Visible=true;
        gbAuto3->Visible=true;
        gbAuto4->Visible=true;
        gbAuto5->Visible=true;
        gbAuto6->Visible=true;
    }

    if(CosFunction.bUseTrayUpDownSet==true)
        rgFixTrayMode->Visible=rgLoaderType->ItemIndex;

    grpFix1->Visible=rgLoaderType->ItemIndex;
    grpFix2->Visible=rgLoaderType->ItemIndex;
    grpFix3->Visible=rgLoaderType->ItemIndex;
    grpFix4->Visible=rgLoaderType->ItemIndex;
    grpFix5->Visible=rgLoaderType->ItemIndex;
    grpFix6->Visible=rgLoaderType->ItemIndex;

    pnlFTTrayFrom->Visible=rgLoaderType->ItemIndex;
    pnlRTTrayFrom->Visible=rgLoaderType->ItemIndex;

    if(IniConfig.bUseFix3==false)                                               //2013-07-16    Dell    9046LS第一台沒Fix3;第二台以上Fix3標配
        grpFix3->Visible=false;                                                 //2013-01-15    Dell    9046LS 硬體沒有Fix3

    if(pgRunMode->ActivePageIndex==0 ||                                         //TabSheet1
       pgRunMode->ActivePageIndex==2)
    {
        edAuto1Type->Text=(RGAuto1->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto2Type->Text=(RGAuto2->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto3Type->Text=(RGAuto3->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto4Type->Text=(RGAuto4->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto5Type->Text=(RGAuto5->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto6Type->Text=(RGAuto6->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
    }
    else
    {
        edAuto1Type->Text=(rgAuto1_RT->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto2Type->Text=(rgAuto2_RT->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto3Type->Text=(rgAuto3_RT->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto4Type->Text=(rgAuto4_RT->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto5Type->Text=(rgAuto5_RT->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
        edAuto6Type->Text=(rgAuto6_RT->ItemIndex==0)?cbEmpty->Text:cbColor->Text;
    }

    if(IniConfig.bTrayAssignUseGraphic &&                                       //Steven 20111121 : 使用圖片去顯示Tray Assign
       AUTO_EMPTY_COLOR<3)                                                      //Steven 20230915 : HT9016UC暫時不支援圖像模式
    {
        if(rgLoaderType->ItemIndex==0)
        {
            tsNormalTestGroup->TabVisible   =true;
            tsReTestGroup->TabVisible       =true;
            tsNormalTestGraph->TabVisible   =false;
            tsReTestGraph->TabVisible       =false;
        }
        else
        {
            tsNormalTestGroup->TabVisible   =false;
            tsReTestGroup->TabVisible       =false;
            tsNormalTestGraph->TabVisible   =true;
            tsReTestGraph->TabVisible       =true;
        }
    }
    else
    {
        tsNormalTestGroup->TabVisible       =true;
        tsReTestGroup->TabVisible           =true;
        tsNormalTestGraph->TabVisible       =false;
        tsReTestGraph->TabVisible           =false;
    }

    //在最下面調整畫面--------------------------
    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20230907 : For HT-9011UC
    {
        if(AUTO_EMPTY_COLOR==3)
        {
            grpICSort6->Visible=false;
            gbAuto6   ->Visible=false;
            RGAuto6   ->Visible=false;
            rgAuto6_RT->Visible=false;
        }
        Width=1280;                                                             //RogerYang 20250826 : 修改顯示位置
        if(IniConfig.bA65_BundleIDList)                                         //JerryYang 20250220 : AUTO IN OUT
        {
            Width+=250;
            palAMR->Left=1250;                                                  //RogerYang 20250826 : 修改顯示位置
        }
        else
        {
            palAMR->Left=1262;                                                  //RogerYang 20250826 : 修改顯示位置
        }
    }
    else
    {
        grpICSort4->Visible=false;
        grpICSort5->Visible=false;
        grpICSort6->Visible=false;
        gbAuto4->Visible   =false;
        gbAuto5->Visible   =false;
        gbAuto6->Visible   =false;
        RGAuto4->Visible   =false;
        RGAuto5->Visible   =false;
        RGAuto6->Visible   =false;
        grpFix4->Visible   =false;
        grpFix5->Visible   =false;
        grpFix6->Visible   =false;
        rgAuto4_RT->Visible=false;
        rgAuto5_RT->Visible=false;
        rgAuto6_RT->Visible=false;
        Width=861;
        if(IniConfig.bA65_BundleIDList)                                         //JerryYang 20250220 : AUTO IN OUT
        {
            Width+=250;
            palAMR->Left=842;                                                   //RogerYang 20250826 : 修改顯示位置
        }
        else if(IniConfig.bVTESTFunction==true)                                 //RogerYang 20250604 偉測不可複測bin功能
        {
//            if(bVTestNoRTBin==true)                                             //Rogeryang 20250723 改成label
//            {
//                lblVtestNoRTBin->Caption="MES Enable No Retest Bin";
//                lblVtestNoRTBin->Font->Color=clBlue;
//            }
//            else
//            {
//                lblVtestNoRTBin->Caption="MES Disable No Retest Bin";
//                lblVtestNoRTBin->Font->Color=clRed;
//            }
            edNoRTBinFix1->Color=(bNoRTBinFixFlag[0]==true)?clYellow:clWindow;  //Rogeryang 20250723 add 顯示顏色
            edNoRTBinFix2->Color=(bNoRTBinFixFlag[1]==true)?clYellow:clWindow;
            edNoRTBinFix3->Color=(bNoRTBinFixFlag[2]==true)?clYellow:clWindow;

            cbSpecTrayCnt->Visible=false;
            Label47->Visible=false;
            edAMRInputTrayCount->Visible=false;
            Label49->Visible=false;
            edAMRFullTrayCount->Visible=false;

            grpVTestNoTBin->Top=5;
            grpVTestNoTBin->Visible=true;
            Width+=250;
            palAMR->Left=842;                                                   //RogerYang 20250826 : 修改顯示位置
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::rgLoaderTypeClick(TObject *Sender)
{
    if(rgLoaderType->ItemIndex!=TrayForm.LodareType &&
       Barcode_Reader(bcTrayAssign)==0)                                         // 20140103 wei KYEC Barcode Reader
    {
        rgLoaderType->ItemIndex=TrayForm.LodareType;
        return;
    }

    cbLoaderChange(this);
    ShowCompnet();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::rgFixTrayModeClick(TObject *Sender)
{
    labFix1->Visible=(MOT[MManualTray1].TrayFeedHasIC());                       //wei 20160420 Fix盤要沒有IC才能上下盤切換
    labFix2->Visible=(MOT[MManualTray2].TrayFeedHasIC());                       //wei 20160420 Fix盤要沒有IC才能上下盤切換
    labFix3->Visible=(MOT[MManualTray3].TrayFeedHasIC());                       //wei 20160420 Fix盤要沒有IC才能上下盤切換
    labFix4->Visible=(MOT[MManualTray4].TrayFeedHasIC());
    labFix5->Visible=(MOT[MManualTray5].TrayFeedHasIC());
    labFix6->Visible=(MOT[MManualTray6].TrayFeedHasIC());

    ckUseFix1->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray1].TrayFeedHasIC()==false);  //wei 20160420 Fix盤要沒有IC才能上下盤切換
    ckUseFix2->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray2].TrayFeedHasIC()==false);  //wei 20160420 Fix盤要沒有IC才能上下盤切換
    ckUseFix3->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray3].TrayFeedHasIC()==false);  //wei 20160420 Fix盤要沒有IC才能上下盤切換
    ckUseFix4->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray4].TrayFeedHasIC()==false);
    ckUseFix5->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray5].TrayFeedHasIC()==false);
    ckUseFix6->Visible=(CosFunction.bUseTrayUpDownSet && rgLoaderType->ItemIndex==1 && rgFixTrayMode->ItemIndex==1 && MOT[MManualTray6].TrayFeedHasIC()==false);

    if(MOT[MManualTray1].TrayFeedHasIC()==false &&                              //wei 20160420 Fix盤要沒有IC才能上下盤切換
       MOT[MManualTray2].TrayFeedHasIC()==false &&
       MOT[MManualTray3].TrayFeedHasIC()==false &&
       MOT[MManualTray4].TrayFeedHasIC()==false &&
       MOT[MManualTray5].TrayFeedHasIC()==false &&
       MOT[MManualTray6].TrayFeedHasIC()==false)
    {
        ShowCompnet();
    }
    else
    {
        rgFixTrayMode->ItemIndex=TrayForm.iFixTrayMode;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::cbEmptyChange(TObject *Sender)
{
    if(bUseAuto2Empty&&rgAuto2_RT->ItemIndex==2 && BinTrayDetect(1, 2)==1)      //kevin 20120718 判別 auto2 是否有設 bin別
    {
        rgAuto3_RT->Enabled=false;
    }
    else
    {
        rgAuto3_RT->Enabled=true;
    }
    ShowCompnet();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::ShowTrayDirectIMG()
{
    TImage *MyImage[]={imgLoader, imgAuto1, imgAuto2, imgAuto3, imgAuto4, imgAuto5, imgAuto6,
                                  imgFix1,  imgFix2,  imgFix3,  imgFix4,  imgFix5,  imgFix6};

    AnsiString Dir;
    for(int i=0; i<sizeof(MyImage)/4; i++)
    {
        if(iTrayDirect[i]<0 || iTrayDirect[i]>8)
            iTrayDirect[i]=0;
        Dir.sprintf("%stype%d.bmp", BmpPath, iTrayDirect[i]);
        try
        {
            MyImage[i]->Picture->LoadFromFile(Dir);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "ShowTrayDirectIMG");
            ShowMyMessage("''Path :D:\\HT9045\\IMG\\BMP\\'' be delete");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::RGLoaderClick(TObject *Sender)
{
    if(pgRunMode->ActivePageIndex==0 ||                                         //TabSheet1
       pgRunMode->ActivePageIndex==2)
    {
        cbEmpty->Enabled=true;
        cbColor->Enabled=true;
        if(RGLoader->ItemIndex==0)
        {
            cbEmpty->Enabled=false;
            cbEmpty->ItemIndex=cbLoader->ItemIndex;
        }
        else
        {
            cbColor->Enabled=false;
            cbColor->ItemIndex=cbLoader->ItemIndex;
        }

        if(IniConfig.bTrayAssignUseGraphic &&                                   //Steven 20111121 : 使用圖片去顯示Tray Assign
           AUTO_EMPTY_COLOR<3)                                                  //Steven 20230915 : HT9016UC暫時不支援圖像模式
        {
            if(rgLoaderType->ItemIndex==0)
            {
                if(RGLoader->ItemIndex==0)
                {
                    sbNormalTest->Position=0;
                }
                else
                {
                    sbNormalTest->Position=15;
                }
            }
        }
    }

    if(bUseAuto2Empty && RGLoader->ItemIndex==2 && BinTrayDetect(0, 1)==1)      //kevin 20120718 判別 auto1 是否有設 bin別
        RGAuto1->Enabled=false;
    else
        RGAuto1->Enabled=true;
    ShowCompnet();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::rgLoad_RTClick(TObject *Sender)
{
    if(pgRunMode->ActivePageIndex==1 ||                                         //TabSheet2
       pgRunMode->ActivePageIndex==3)
    {
        cbEmpty->Enabled=true;
        cbColor->Enabled=true;
        if(rgLoad_RT->ItemIndex==0)
        {
            cbEmpty->Enabled=false;
            cbEmpty->ItemIndex=cbLoader->ItemIndex;
        }
        else
        {
            cbColor->Enabled=false;
            cbColor->ItemIndex=cbLoader->ItemIndex;
        }

        if(IniConfig.bTrayAssignUseGraphic &&                                   //Steven 20111121 : 使用圖片去顯示Tray Assign
           AUTO_EMPTY_COLOR<3)                                                  //Steven 20230915 : HT9016UC暫時不支援圖像模式
        {
            if(rgLoaderType->ItemIndex==0)
            {
                if(rgLoad_RT->ItemIndex==0)
                {
                    sbNormalTest_RT->Position=0;
                }
                else
                {
                    sbNormalTest_RT->Position=15;
                }
            }
        }
    }

    if(bUseAuto2Empty && rgLoad_RT->ItemIndex==2 && BinTrayDetect(0, 1)==1)     //kevin 20120718 判別 auto1 是否有設 bin別
        rgAuto1_RT->Enabled=false;
    else
        rgAuto1_RT->Enabled=true;
    ShowCompnet();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::cbLoaderChange(TObject *Sender)
{
    if(rgLoaderType->ItemIndex==0)
        return;
    rgLoad_RTClick(this);
    RGLoaderClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    rgLoaderTrayMode->Enabled=true;
    ReadFile();
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    fShow=false;

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S="";
    //TComboBox *PtrCombBox[]={cbEmpty, cbColor};

    S=GetLastOpenFN();
    AnsiString szDir="";

    if(IniConfig.bFTBin2RTBin==true)                                            //Steven 20120627 : 當FT存檔時,把RT設定跟FT一樣
    {
        if(pgRunMode->ActivePage==tsNormalTestGroup ||
           pgRunMode->ActivePage==tsNormalTestGraph)
        {
            if(IniConfig.bTrayAssignUseGraphic &&                               //Steven 20111121 : 使用圖片去顯示Tray Assign
               AUTO_EMPTY_COLOR<3)                                              //Steven 20230915 : HT9016UC暫時不支援圖像模式
            {
                sbNormalTest_RT->Position=sbNormalTest->Position;
            }
            else
            {
                rgLoad_RT->ItemIndex =RGLoader->ItemIndex;
                rgAuto1_RT->ItemIndex=RGAuto1->ItemIndex;
                rgAuto2_RT->ItemIndex=RGAuto2->ItemIndex;
                rgAuto3_RT->ItemIndex=RGAuto3->ItemIndex;
                rgAuto4_RT->ItemIndex=RGAuto4->ItemIndex;
                rgAuto5_RT->ItemIndex=RGAuto5->ItemIndex;
                rgAuto6_RT->ItemIndex=RGAuto6->ItemIndex;
            }
        }
    }

    if(IniConfig.bTrayAssignUseGraphic &&                                       //Steven 20111121 : 使用圖片去顯示Tray Assign
       AUTO_EMPTY_COLOR<3)                                                      //Steven 20230915 : HT9016UC暫時不支援圖像模式
    {
        GraphicToRadio();
        GraphicToRadio_RT();
    }

    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SaveRecipe);
    }

    if(CosFunction.bOLPFunction)                                                //Sam 20230921 : Bin 設定錯誤不能啟動
    {
        LastSet.OLPSetBinErr[2]=0;
    }

    szDir.sprintf("%s%s", DataPath, S);
    SaveSetupFile(szDir, S);
    #ifdef ASE_KaohSiung
        fBuilder->bSaveAsJobFile(S, "JOBFILE");                                 //kevin 20150105  Start 另存 jobfile
    #endif
    if(TrayForm.bEnableAMR) //Eastsun 20260710 Merge
        fLotInfo->tsKYEC_AMR->TabVisible=true;
    else
        fLotInfo->tsKYEC_AMR->TabVisible=false;

    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//------------------------------------------------------------
void __fastcall TfTrayAssignment::SaveSetupFile(AnsiString szDir, AnsiString S)
{
    int iLinkSum=0;
    int ibuffer=0;
    AnsiString Str1, Str2;
    TComboBox *PtrCombBox[]={cbEmpty, cbColor};
    MyForceDirectories(szDir);
    szDir+="\\Tray.Data";
    AnsiString szDir2="D:\\HT9045\\config\\config.ini";

    if(IniConfig.bP46_LoadTrayModeByHandler)                                    //Steven 20221117 : Loader Tray Mode設定跟著機台變
    {
        WriteIniData(szDir2, "Flag", "Skip Manual Remove Tray",  rgLoaderTrayMode->ItemIndex);
    }
    else
    {
        WriteIniData(szDir, "Flag", "Skip Manual Remove Tray",  rgLoaderTrayMode->ItemIndex);                           //Steven 20101019          //Steven 20130819 : 加入開門檢查的功能
    }

    WriteIniData(szDir, "Rotation", "Input",   cbInputRotateKIT->ItemIndex);
    WriteIniData(szDir, "Rotation", "Output",  cbOutputRotateKIT->ItemIndex);

    if(CosFunction.bUnloadTrayModeByRecipe==false)                              //Steven 20220710 : 甬矽要求Unload Tray Mode by機台設置
    {
        WriteIniData(szDir, "Flag", "Auto Feed",                chkAutoTrayFeed->Checked);
        WriteIniData(szDir, "Flag", "bFailAutoTrayManual_FT",   cbManuTakeAutoFailTray_FT->Checked);                    //Steven 20150116 : 手動移除Auto Fail Bin Tray
        WriteIniData(szDir, "Flag", "bFailAutoTrayManual_RT",   cbManuTakeAutoFailTray_RT->Checked);                    //Steven 20150116 : 手動移除Auto Fail Bin Tray
    }
    WriteIniData(szDir, "Flag", "Color Tray Sensor",        chkColorTray->Checked);                                     //20140903 wei colcr Tray
    WriteIniData(szDir, "Flag", "Check loader tray direction", chkCheckLoadDirection->Checked);                         //Steven 20190815 : JCET不重測Tray偵測
    WriteIniData(szDir, "Flag", "Loader Type",              rgLoaderType->ItemIndex);
    WriteIniData(szDir, "Flag", "MoveAfterTrayGoOut",        cbMoveAfterTrayGoOut->Checked);                            //JerryYang 20241002 : SPIL要求功能 Trray arm等AUTO TRAY退到外面才能移動

    WriteIniData(szDir, "Flag", "bSpecTrayCnt",        cbSpecTrayCnt->Checked);                                         //JerryYang 20250212 : add

    WriteIniData(szDir, "VTest", "NoRTBin",       cbVTestNoRTBin->Checked);     //RogerYang 20250814 : 偉測不可複測bin功能
    WriteIniData(szDir, "VTest", "NoRTBinFix1",   edNoRTBinFix1->Text);
    WriteIniData(szDir, "VTest", "NoRTBinFix2",   edNoRTBinFix2->Text);
    WriteIniData(szDir, "VTest", "NoRTBinFix3",   edNoRTBinFix3->Text);

    if(CosFunction.bUseTrayUpDownSet==false)
    {
        WriteIniData(szDir, "Flag", "Fix Tray Mode",    0);
    }
    else
    {
        WriteIniData(szDir, "Flag", "Fix Tray Mode",    rgFixTrayMode->ItemIndex);
    }

    if(CosFunction.bUseTrayUpDownSet)                                           //wei 20160224 TSMC FIX UPDOWN
    {
        WriteIniData(szDir, "Flag", "Use Fix1 Tray",    ckUseFix1->Checked);
        WriteIniData(szDir, "Flag", "Use Fix2 Tray",    ckUseFix2->Checked);
        WriteIniData(szDir, "Flag", "Use Fix3 Tray",    ckUseFix3->Checked);
        if(AUTO_EMPTY_COLOR>=3)
        {
            WriteIniData(szDir, "Flag", "Use Fix4 Tray", ckUseFix4->Checked);
            WriteIniData(szDir, "Flag", "Use Fix5 Tray", ckUseFix5->Checked);
            WriteIniData(szDir, "Flag", "Use Fix6 Tray", ckUseFix6->Checked);
        }
    }
    WriteIniData(szDir, "Flag", "Fix Tray Link",    iLinkSum);

    WriteIniData(szDir, "Loader", "Tray Type",      cbLoader->ItemIndex);
    WriteIniData(szDir, "Loader", "Direction",      iTrayDirect[0]);
    WriteIniData(szDir, "Loader", "ToBuffer",       RGLoader->ItemIndex);
    WriteIniData(szDir, "Loader", "ToBuffer_RT",    rgLoad_RT->ItemIndex);

    WriteIniData(szDir, "Empty", "Tray Type",       cbEmpty->ItemIndex);
    WriteIniData(szDir, "Color", "Tray Type",       cbColor->ItemIndex);

    WriteIniData(szDir, "AMR", "Full Tray Count",         edAMRFullTrayCount->Text);                                    //JerryYang 20250220 : AUTO IN OUT
    WriteIniData(szDir, "AMR", "Input Tray Count",        edAMRInputTrayCount->Text);
    //==> Eastsun 20260512 F009 整合: AMR WriteIni (D1.1.2)
    WriteIniData(szDir, "AMR", "Enahle AMR",              cbEnableAMR_KYEC->Checked);
    WriteIniData(szDir, "AMR", "Enahle AMR Loader",       cbEnableAMRLoader_KYEC->Checked);
    WriteIniData(szDir, "AMR", "Reader Pos",              rgAMRReaderPos_KYEC->ItemIndex);
    WriteIniData(szDir, "AMR", "1st Tray Type",           cbAMRTray1st_KYEC->ItemIndex);
    WriteIniData(szDir, "AMR", "2nd Tray Type",           cbAMRTray2nd_KYEC->ItemIndex);
    WriteIniData(szDir, "AMR", "3rd Tray Type",           cbAMRTray3rd_KYEC->ItemIndex);
    WriteIniData(szDir, "AMR", "1st ID Use",              cbAMRTrayID1st_KYEC->ItemIndex);
    WriteIniData(szDir, "AMR", "2nd ID Use",              cbAMRTrayID2nd_KYEC->ItemIndex);
    WriteIniData(szDir, "AMR", "3rd ID Use",              cbAMRTrayID3rd_KYEC->ItemIndex);
    //<== Eastsun 20260512 F009 整合

    if(IniConfig.bP36BufferTrayNoSame &&
       (RGLoader->ItemIndex==0 ||                                               //empty tray
        rgLoad_RT->ItemIndex==0))                                               //kevin 20171117 (wei) Load && Unload 強制不能使用相同軌道
    {
        if(RGLoader->ItemIndex==0)                                              //color tray
        {
            RGAuto1->ItemIndex=1;
            RGAuto2->ItemIndex=1;
            RGAuto3->ItemIndex=1;
            RGAuto4->ItemIndex=1;
            RGAuto5->ItemIndex=1;
            RGAuto6->ItemIndex=1;
        }

        if(rgLoad_RT->ItemIndex==0)
        {
            rgAuto1_RT->ItemIndex=1;
            rgAuto2_RT->ItemIndex=1;
            rgAuto3_RT->ItemIndex=1;
            rgAuto4_RT->ItemIndex=1;
            rgAuto5_RT->ItemIndex=1;
            rgAuto6_RT->ItemIndex=1;
        }
    }

    if(IniConfig.bP36BufferTrayNoSame &&
       (RGLoader->ItemIndex==1 ||                                               //color tray
        rgLoad_RT->ItemIndex==1))                                               //kevin 20171117 (wei) Load && Unload 強制不能使用相同軌道
    {
        if(RGLoader->ItemIndex==1)
        {
            RGAuto1->ItemIndex=0;                                               //empty tray
            RGAuto2->ItemIndex=0;
            RGAuto3->ItemIndex=0;
            RGAuto4->ItemIndex=0;
            RGAuto5->ItemIndex=0;
            RGAuto6->ItemIndex=0;
        }

        if(rgLoad_RT->ItemIndex==1)
        {
            rgAuto1_RT->ItemIndex=0;
            rgAuto2_RT->ItemIndex=0;
            rgAuto3_RT->ItemIndex=0;
            rgAuto4_RT->ItemIndex=0;
            rgAuto5_RT->ItemIndex=0;
            rgAuto6_RT->ItemIndex=0;
        }
    }

    WriteIniData(szDir, s6TrayName[eAuto1], "Tray Type",       PtrCombBox[RGAuto1->ItemIndex]->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto1], "FromBuffer",      RGAuto1->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto1], "FromBuffer_RT",   rgAuto1_RT->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto1], "Direction",       iTrayDirect[eAuto1+1]);
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        WriteIniData(szDir, s6TrayName[eAuto1], "TrayEndRecv", chkTrayEndRecvAuto1->Checked);

    if(RGAuto2->ItemIndex>=2)                                                   //kevin 20120912
        ibuffer=RGAuto3->ItemIndex;
    else
        ibuffer=RGAuto2->ItemIndex;
    WriteIniData(szDir, s6TrayName[eAuto2], "Tray Type",       PtrCombBox[ibuffer]->ItemIndex);                         //kevin 20120912
    WriteIniData(szDir, s6TrayName[eAuto2], "FromBuffer",      RGAuto2->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto2], "FromBuffer_RT",   rgAuto2_RT->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto2], "Direction",       iTrayDirect[eAuto2+1]);
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        WriteIniData(szDir, s6TrayName[eAuto2], "TrayEndRecv", chkTrayEndRecvAuto2->Checked);

    WriteIniData(szDir, s6TrayName[eAuto3], "Tray Type",       PtrCombBox[RGAuto3->ItemIndex]->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto3], "FromBuffer",      RGAuto3->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto3], "FromBuffer_RT",   rgAuto3_RT->ItemIndex);
    WriteIniData(szDir, s6TrayName[eAuto3], "Direction",       iTrayDirect[eAuto3+1]);
    if(IniConfig.bP09TrayEndCanSelectTray)                                      //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
        WriteIniData(szDir, s6TrayName[eAuto3], "TrayEndRecv", chkTrayEndRecvAuto3->Checked);

    WriteIniData(szDir, s6TrayName[eFix1], "Tray Type",        cbFix1->ItemIndex);
    WriteIniData(szDir, s6TrayName[eFix1], "Direction",        iTrayDirect[eFix1+1]);

    WriteIniData(szDir, s6TrayName[eFix2], "Tray Type",        cbFix2->ItemIndex);
    WriteIniData(szDir, s6TrayName[eFix2], "Direction",        iTrayDirect[eFix2+1]);

    WriteIniData(szDir, s6TrayName[eFix3], "Tray Type",        cbFix3->ItemIndex);
    WriteIniData(szDir, s6TrayName[eFix3], "Direction",        iTrayDirect[eFix3+1]);

    if(AUTO_EMPTY_COLOR>=3)
    {
        WriteIniData(szDir, s6TrayName[eAuto4], "Tray Type",       PtrCombBox[RGAuto4->ItemIndex]->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto4], "FromBuffer",      RGAuto4->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto4], "FromBuffer_RT",   rgAuto4_RT->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto4], "Direction",       iTrayDirect[eAuto4+1]);
        if(IniConfig.bP09TrayEndCanSelectTray)                                  //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
            WriteIniData(szDir, eAuto4, "TrayEndRecv", chkTrayEndRecvAuto4->Checked);

        WriteIniData(szDir, s6TrayName[eAuto5], "Tray Type",       PtrCombBox[RGAuto5->ItemIndex]->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto5], "FromBuffer",      RGAuto5->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto5], "FromBuffer_RT",   rgAuto5_RT->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto5], "Direction",       iTrayDirect[eAuto5+1]);
        if(IniConfig.bP09TrayEndCanSelectTray)                                  //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
            WriteIniData(szDir, eAuto5, "TrayEndRecv", chkTrayEndRecvAuto5->Checked);

        WriteIniData(szDir, s6TrayName[eAuto6], "Tray Type",       PtrCombBox[RGAuto6->ItemIndex]->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto6], "FromBuffer",      RGAuto6->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto6], "FromBuffer_RT",   rgAuto6_RT->ItemIndex);
        WriteIniData(szDir, s6TrayName[eAuto6], "Direction",       iTrayDirect[eAuto6+1]);
        if(IniConfig.bP09TrayEndCanSelectTray)                                  //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
            WriteIniData(szDir, eAuto6, "TrayEndRecv", chkTrayEndRecvAuto6->Checked);

        WriteIniData(szDir, s6TrayName[eFix4], "Tray Type",        cbFix4->ItemIndex);
        WriteIniData(szDir, s6TrayName[eFix4], "Direction",        iTrayDirect[eFix4+1]);

        WriteIniData(szDir, s6TrayName[eFix5], "Tray Type",        cbFix5->ItemIndex);
        WriteIniData(szDir, s6TrayName[eFix5], "Direction",        iTrayDirect[eFix5+1]);

        WriteIniData(szDir, s6TrayName[eFix6], "Tray Type",        cbFix6->ItemIndex);
        WriteIniData(szDir, s6TrayName[eFix6], "Direction",        iTrayDirect[eFix6+1]);
    }

    if(CosFunction.bTraySortCntFunc)                                            //Steven 20210608 : Tray指定數量退盤功能
    {
        for(int i=0; i<=iFixRight; i++)
        {
            if(Prod.iTrayType[i]==tTrayAuto)
            {
                WriteIniData(szDir, s6TrayName[i],  "bTraySortCntFunc",  chkICSort[i]->Checked);

                for(int j=0; j<6; j++)
                {
                    Str2.sprintf("TraySortCnt%d_%d", i+1, j+1);
                    WriteIniData(szDir, s6TrayName[i], Str2, edtICSort[i][j]->Text);
                }
            }
        }
    }

    AddSpace(szDir);
    ReadFile();
    spbSave->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::sbNormalTestChange(TObject *Sender)
{
    GraphicToRadio();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::GraphicToRadio()
{
    AnsiString str;
    str.sprintf("%sTray%02d.bmp", BmpPath, sbNormalTest->Position);
    ImgNormalTest->Picture->LoadFromFile(str);

    RGAuto3->ItemIndex =sbNormalTest->Position&0x01;
    RGAuto2->ItemIndex =sbNormalTest->Position&0x02;
    RGAuto1->ItemIndex =sbNormalTest->Position&0x04;
    RGLoader->ItemIndex=sbNormalTest->Position&0x08;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::RadioToGraphic()
{
    int index[4]={0, 0, 0, 0}, i, j=0;                                          //QQQ

    index[0]=RGAuto3->ItemIndex;
    index[1]=RGAuto2->ItemIndex;
    index[2]=RGAuto1->ItemIndex;
    index[3]=RGLoader->ItemIndex;

    for(i=0; i<4; i++)
        j+=index[i]<<i;

    sbNormalTest->Position=j;
    AnsiString str;
    str.sprintf("%sTray%02d.bmp", BmpPath, sbNormalTest->Position);
    ImgNormalTest->Picture->LoadFromFile(str);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::sbNormalTest_RTChange(TObject *Sender)
{
    GraphicToRadio_RT();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::GraphicToRadio_RT()
{
    AnsiString str;
    str.sprintf("%sTray%02d.bmp", BmpPath, sbNormalTest_RT->Position);
    ImgReTest->Picture->LoadFromFile(str);

    if(IniConfig.bFTTrayAss2RTTrayAss==true)                                    //jou 2012-05-02 當FT Tray Assignment存檔時,把RT Tray Assignment設定跟FT一樣
    {
        if(pgRunMode->ActivePage==tsNormalTestGraph)
        {
            sbNormalTest_RT->Position=sbNormalTest->Position;
        }
    }

    rgAuto3_RT->ItemIndex=sbNormalTest_RT->Position&0x01;
    rgAuto2_RT->ItemIndex=sbNormalTest_RT->Position&0x02;
    rgAuto1_RT->ItemIndex=sbNormalTest_RT->Position&0x04;
    rgLoad_RT->ItemIndex =sbNormalTest_RT->Position&0x08;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::RadioToGraphic_RT()
{
    int index[4]={0, 0, 0, 0}, i, j=0;

    index[0]=rgAuto3_RT->ItemIndex;
    index[1]=rgAuto2_RT->ItemIndex;
    index[2]=rgAuto1_RT->ItemIndex;
    index[3]=rgLoad_RT->ItemIndex;

    for(i=0; i<4; i++)
        j+=index[i]<<i;

    sbNormalTest_RT->Position=j;
    AnsiString str;
    str.sprintf("%sTray%02d.bmp", BmpPath, sbNormalTest_RT->Position);
    ImgReTest->Picture->LoadFromFile(str);
}
//---------------------------------------------------------------------------
// kevin 20120726 Bin別判斷 是否要被使用 auto 1 2 3 fix 1 2 3 是否被設定 bin
//----------------------------------------------------------------------------
int TfTrayAssignment::BinTrayDetect(int iBinTray, int bAuto12)                  //Steven 20120210 Start: Fix3有Bin不能做Auto Clean
{
    bool bFix3HasBin=false;
    AnsiString BinTray;
    BinTray.sprintf("%s already has bin!!", s6TrayName[bAuto12]);
    if((RGLoader->ItemIndex==2   && iBinTray==0) ||
       (RGAuto2->ItemIndex==2    && iBinTray==1) ||
       (rgLoad_RT->ItemIndex==2  && iBinTray==0) ||
       (rgAuto2_RT->ItemIndex==2 && iBinTray==1))                               //Auto 1  Auto 2
    {
        if(RGLoader->ItemIndex==2 && iBinTray==0 ||
           RGAuto2->ItemIndex==2 && iBinTray==1)
        {
            BinTray.sprintf("%s already has bin!!", s6TrayName[bAuto12]);
        }
        else if((rgLoad_RT->ItemIndex==2 && iBinTray==0) ||
                (rgAuto2_RT->ItemIndex==2 && iBinTray==1))                      //Auto 1  Auto 2     //QQQ
        {
            BinTray.sprintf("RT - %s already has bin!!", s6TrayName[bAuto12]);
        }

        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
        {
            if(Prod.iT6CatData[i]==iBinTray ||
               Prod.iT6CatData[i]==iFixRight)                                   //放在FIX 3
            {
                bFix3HasBin=true;
            }
        }

        if(Prod.iIfErrorT6==iBinTray ||
           Prod.iIfErrorT6==iFixRight)                                          //放在FIX 3
            bFix3HasBin=true;

        if(bFix3HasBin==true && bAuto12!=0)
        {
            if(RGLoader->ItemIndex==2)
                RGLoader->ItemIndex=0;
            else if(RGAuto2->ItemIndex==2)
                RGAuto2->ItemIndex=0;
            else if(rgLoad_RT->ItemIndex==2)
                rgLoad_RT->ItemIndex=0;
            else if(rgAuto2_RT->ItemIndex==2)
                rgAuto2_RT->ItemIndex=0;

            ShowMyMessage(BinTray , "Cannot Put tray .");
            return 0;
        }
        return 1;
    }
    return 2;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::RGAuto2Click(TObject *Sender)
{
    if(bUseAuto2Empty && RGAuto2->ItemIndex==2 && BinTrayDetect(1, 2)==1)       //kevin 20120718 判別 auto2 是否有設 bin別
        RGAuto3->Enabled=false;
    else
        RGAuto3->Enabled=true;
    ShowCompnet();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::cbLoaderDropDown(TObject *Sender)
{
    if(Barcode_Reader(bcTrayAssign)==0)                                         // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::edtICSort1_1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 1000);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayAssignment::FormShortCut(TWMKey &Msg, bool &Handled)
{
   if(Msg.CharCode==VK_F5)                                                      //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=75;
        Top=10;
    }
}
//---------------------------------------------------------------------------
//==> Eastsun 20260512 F009 整合: cbEnableAMR_KYEC OnClick (D1.1.6)
void __fastcall TfTrayAssignment::cbEnableAMR_KYECClick(TObject *Sender)
{
    if(cbEnableAMR_KYEC->Checked)                                                    //Eastsun 20260512 : AMR 模式下 Loader Tray Mode 強制設成 None
    {
        rgLoaderTrayMode->ItemIndex=0;
    }
    else
    {
        rgLoaderTrayMode->ItemIndex=1;
    }
}
//----------------------------------------------------------------------------
//<== Eastsun 20260512 F009 整合
