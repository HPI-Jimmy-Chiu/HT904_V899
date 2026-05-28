#include "MachineDefine.h"
#pragma hdrstop

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
#include "main.h"
#include "ckernel.h"
#include "cOffSet.h"
#include "aArmHeader.h"
#include "mysensor.h"
#include "handlerlog.h"
#include "INPUT.h"                                                              //KaiHuang 20201203 Add
#include "Password.h"                                                           //KaiHuang 20201203 Add
#include "cinitial.h"
#include "cpublic.h"
#include "FTPClient.h"                                                          //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma link "HTray"
#pragma resource "*.dfm"
TfBarCode *fBarCode;

int iConntectionOkTask;
int iBottomConntectionOkTask1;                                                  //KaiChen 20200513 : Bottom 2D 8CCD
int iBottomConntectionOkTask2;                                                  //KaiChen 20200513 : Bottom 2D 8CCD

int i2DIDCheckSH1Task;
int i2DIDCheckSH2Task;
TQPF_Timer tChangeSetupFile;
TComboBox   *Multi2DSiteCH[2][2];                                               //Steven 20200810 : 一個IC使用多個2DID
bool bChangeSetupFile=false;
HTimer OCRChangeFileTimeOut;                                                    //Ifor 20210407 add: OCR Change Setup File Timer
bool bWhitelistAlarm=false;                                                     //JerryYang 20241104 : 支援2DID白名單功能
bool bNeedCheckWhitleList=false;
//------------------------------------------------------------------------------
void _MyMulti2DStruct::Set2D(int i, int j, AnsiString Str)                      //Steven 20240612 : for 對角2D
{
    if(TestIF_File.iMulti2DMap[i][j]<=0)
    {
        Str="";
    }
    else if(TestIF_File.iMulti2DMap[i][j]>0)
    {
        if(Str=="" || Str=="0")
            Str=asBarCodeErrorSend;
        sMulti2D[TestIF_File.iMulti2DMap[0][0]-1]=Str;
    }
}
//------------------------------------------------------------------------------
void _MyMulti2DStruct::SetToErrCode()
{
    Clear2D();
    sMulti2D[0]=asBarCodeErrorSend;
}
//------------------------------------------------------------------------------
AnsiString _MyMulti2DStruct::Get2DText()                                        //Steven 20240612 : for 對角2D
{
    AnsiString Str="";
    if(sSeparator=="")
        sSeparator=" ";

    for(int i=0; i<4; i++)
    {
        if(sMulti2D[i]!="")
        {
            if(Str=="")
                Str=sMulti2D[i];
            else
                Str=Str+sSeparator+sMulti2D[i];
        }
    }

    return Str;
}
//------------------------------------------------------------------------------
AnsiString _MyMulti2DStruct::Get2DCommaText()
{
    AnsiString Str="";

    for(int i=0; i<4; i++)
    {
        if(sMulti2D[i]!="")
        {
            if(Str=="")
                Str=sMulti2D[i];
            else
                Str=Str+AnsiString(",")+sMulti2D[i];
        }
    }

    return Str;
}
//------------------------------------------------------------------------------
AnsiString _MyMulti2DStruct::Get2DTextByMap(int i)                              //Steven 20240612 : for 對角2D
{
    AnsiString Str;
    Str="";
    if(i>=0 && i<4)
        Str=sMulti2D[i];

    return Str;
}
//------------------------------------------------------------------------------
void _MyMulti2DStruct::Clear2D()                                                //Steven 20240612 : for 對角2D
{
    sMulti2D[0]="";
    sMulti2D[1]="";
    sMulti2D[2]="";
    sMulti2D[3]="";
}
//------------------------------------------------------------------------------
__fastcall TfBarCode::TfBarCode(TComponent* Owner)
    : TForm(Owner)
{
    BarcodeCOM[0]=Barcode_1;
    BarcodeCOM[1]=Barcode_2;
    BarcodeCOM[2]=Barcode_3;
    BarcodeCOM[3]=Barcode_4;
    iConntectionOkTask=0;

    iBottomConntectionOkTask1=0;                                                //KaiChen 20200513 : Bottom 2D 8CCD
    iBottomConntectionOkTask2=0;                                                //KaiChen 20200513 : Bottom 2D 8CCD

    bGetLF=false;
    bGetST=false;
    bGetGF=false;

    for(int i=0; i<BAR_CODE_COUNT; i++)
    {
        bBarcodeStartDelay      [i]     =false;
        bBarcodeError           [i]     =false;
        bBarcodeDataSaveReady   [i]     =false;
        iGetSE2                 [i]     =-1;                                    //Steven 20160120 : Auto Teach 2D Matrix
        iGetSE3                 [i]     =-1;                                    //Steven 20120120 : Auto Learn 2D ROI
        iGetSE4                 [i]     =-1;                                    //Steven 20120126 : 只拍一張, 只確認有無讀取
        bGetSE9                 [i]     =false;
        iCurrentStep            [i]     =-1;                                    //Steven 20160503 : 確認移動位置與拍照位置相同
        bStepError              [i]     =false;                                 //Steven 20160503 : 確認移動位置與拍照位置相同

        bHasMultiLineBarcode    [i]     =false;                                 //Ifor 20210407 add: 自製OCR

        Socket2DReceiveList[i]=new TStringList;
        Socket2DReceiveList[i]->Clear();

        map2DShuttleList[i].clear();
        BarCode_SaveImage[i]=new TMemoryStream();
    }
    map2DList.clear();
    list2DByLot=new TStringList;
    list2DByLot->Clear();

    list2DSorting=new TStringList;                                              //Frank 20221122 : 2DID sorting for ATK
    list2DSorting->Clear();

    map2DSortingList.clear();                                                   //Steven 20240515 : modified for 2D sort

    s2DSorting=new TStringList;
    s2DSorting->Clear();

    listError2DID=new TStringList;                                              //JerryYang 20231218 : 2DID黑名單功能
    listError2DID->Clear();

    list2DWhitle=new TStringList;                                               //JerryYang 20241104 : 支援2DID白名單功能
    list2DWhitle->Clear();

    list2DWhitleResult=new TStringList;
    list2DWhitleResult->Clear();

    list2DWhitleTemp=new TStringList;
    list2DWhitleTemp->Clear();

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
            iBarCodeNo[i][j]=0;
        }
        bEnableCCDChannelConnect[i]=true;
    }

    if(BOTTOM_2DID==1 && BOTTOM_2DID_CCD==1)                                    //KaiChen 20200513 : Bottom 2D 8CCD
    {
        for(int i=0; i<BAR_CODE_COUNT; i++)
        {
            bEnableCCDChannelConnect[i]=true;
        }
    }
    iBottom_1=0;
    iBottom_2=1;
    iBottom_3=2;
    iBottom_4=3;
    iBottom_5=4;
    iBottom_6=5;
    iBottom_7=6;
    iBottom_8=7;

    iBarCode1_1=0;
    iBarCode1_2=1;
    iBarCode2_1=2;
    iBarCode2_2=3;
    bShow=false;

    myOutShuttleLabel->SetColorMap(0, TColor(0x00DFD9CC));
    myOutShuttleLabel->SetColorMap(1, clGreen);
    myOutShuttleLabel->SetColorMap(2, clRed);
    myOutShuttleLabel->SetColorMap(3, clBtnFace);

    mtBarcodeOutSh->SetColorMap(0, TColor(0x00DFD9CC));
    mtBarcodeOutSh->SetColorMap(1, clGreen);
    mtBarcodeOutSh->SetColorMap(2, clRed);
    mtBarcodeOutSh->SetColorMap(3, clBtnFace);

    myInShuttleLabel->SetColorMap(0, TColor(0x00DFD9CC));
    myInShuttleLabel->SetColorMap(1, clGreen);
    myInShuttleLabel->SetColorMap(2, clRed);
    myInShuttleLabel->SetColorMap(3, clBtnFace);

    mtBarcodeInSh->SetColorMap(0, TColor(0x00DFD9CC));
    mtBarcodeInSh->SetColorMap(1, clGreen);
    mtBarcodeInSh->SetColorMap(2, clRed);
    mtBarcodeInSh->SetColorMap(3, clBtnFace);

    mtSFC_Label1->SetColorMap(0, TColor(0x00DFD9CC));
    mtSFC_Label1->SetColorMap(1, clGreen);
    mtSFC_Label1->SetColorMap(2, clRed);
    mtSFC_Label1->SetColorMap(3, clBtnFace);

    mtSFC_Shuttle->SetColorMap(0, TColor(0x00DFD9CC));
    mtSFC_Shuttle->SetColorMap(1, clGreen);
    mtSFC_Shuttle->SetColorMap(2, clRed);
    mtSFC_Shuttle->SetColorMap(3, clBtnFace);

    bGPIBTestBarCodeError=false;
    bGPIBTestBarCodeFormatError=false;

    for(int i=0; i<32; i++)
        iBinReturnMess[i]="";

    tmr1->Enabled=false;
    mtBarcodeSetDefaultView();

    edCCDAddress[iBarCode1_1]=edShuttle_1A_Address;
    edCCDAddress[iBarCode1_2]=edShuttle_1B_Address;
    edCCDAddress[iBarCode2_1]=edShuttle_2A_Address;
    edCCDAddress[iBarCode2_2]=edShuttle_2B_Address;
    edCCDPort[iBarCode1_1]=edShuttle_1A_Port;
    edCCDPort[iBarCode1_2]=edShuttle_1B_Port;
    edCCDPort[iBarCode2_1]=edShuttle_2A_Port;
    edCCDPort[iBarCode2_2]=edShuttle_2B_Port;
    TimerProcess2DData->Enabled=true;
    iRunningMode=LastSet.iRunStartMode;                                         //Steven 20160728 : Log by running mode
    asBarcodeLogName=ReadIniData(asBarCodeLogPath+"\\LastBarcodeLog.ini", "Bar Code", "Last Barcode Log Name", AnsiString(""));
    #ifdef SOFT_SIMULTE
        dVisionVer=2;
    #else
        dVisionVer=1;
    #endif

    //jou 20190930 : Barcode Tray record file
    TTMyTray *temp_ShowBinMappingUnloadPtr[eTrayCount]=
                {   trShowBinMappingAuto1, trShowBinMappingAuto2, trShowBinMappingAuto3, trShowBinMappingAuto4, trShowBinMappingAuto5, trShowBinMappingAuto6,
                    trShowBinMappingFix1,  trShowBinMappingFix2,  trShowBinMappingFix3,  trShowBinMappingFix4,  trShowBinMappingFix5,  trShowBinMappingFix6,
                    trShowBinMappingFix1,  trShowBinMappingFix2,  trShowBinMappingFix3,  trShowBinMappingFix4,  trShowBinMappingFix5,  trShowBinMappingFix6,  trShowBinMappingBinBox,
                    trShowBinMappingMag01, trShowBinMappingMag02, trShowBinMappingMag03, trShowBinMappingMag04, trShowBinMappingMag05, trShowBinMappingMag06, trShowBinMappingMag07,
                    trShowBinMappingMag08, trShowBinMappingMag09, trShowBinMappingMag10, trShowBinMappingMag11, trShowBinMappingMag12, trShowBinMappingMag13, trShowBinMappingMag14
                };

    TComboBox *temp_ShowBinMappingAuto_Index_Ptr[eTrayCount]=
                {   cobMain_BinMapping_Auto1, cobMain_BinMapping_Auto2, cobMain_BinMapping_Auto3, cobMain_BinMapping_Auto4, cobMain_BinMapping_Auto5, cobMain_BinMapping_Auto6,
                    cobMain_BinMapping_Fix1,  cobMain_BinMapping_Fix2,  cobMain_BinMapping_Fix3,  cobMain_BinMapping_Fix4,  cobMain_BinMapping_Fix5,  cobMain_BinMapping_Fix6,
                    cobMain_BinMapping_Fix1,  cobMain_BinMapping_Fix2,  cobMain_BinMapping_Fix3,  cobMain_BinMapping_Fix4,  cobMain_BinMapping_Fix5,  cobMain_BinMapping_Fix6,  cobMain_BinMapping_BinBox,
                    cobMain_BinMapping_Mag01, cobMain_BinMapping_Mag02, cobMain_BinMapping_Mag03, cobMain_BinMapping_Mag04, cobMain_BinMapping_Mag05, cobMain_BinMapping_Mag06, cobMain_BinMapping_Mag07,
                    cobMain_BinMapping_Mag08, cobMain_BinMapping_Mag09, cobMain_BinMapping_Mag10, cobMain_BinMapping_Mag11, cobMain_BinMapping_Mag12, cobMain_BinMapping_Mag13, cobMain_BinMapping_Mag14
                };

    for(int i=0; i<eTrayCount; i++)
    {
        ShowBinMappingUnloadPtr[i]      =temp_ShowBinMappingUnloadPtr[i];
        ShowBinMappingAuto_Index_Ptr[i] =temp_ShowBinMappingAuto_Index_Ptr[i];
    }

    for(int i=0; i<eTrayCount; i++)
    {
        ShowBinMappingAuto_Index_Ptr[i]->OnChange   =cobMain_BinMapping_Auto1Change;
        ShowBinMappingAuto_Index_Ptr[i]->Tag        =i;
    }

    Multi2DSiteCH[0][0]=cbAa;                                                   //Steven 20200810 : 一個IC使用多個2DID
    Multi2DSiteCH[0][1]=cbAb;
    Multi2DSiteCH[1][0]=cbBa;
    Multi2DSiteCH[1][1]=cbBb;
    if(CosFunction.bReadClipCodeFromUnloader==true)
    {
        elUnloaderClip    =new HTEditList;
        InitUnloaderClipEdtList();
        tsCCD_Unloader->TabVisible=true;
    }
    else
    {
        tsCCD_Unloader->TabVisible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::FormShow(TObject *Sender)
{
    Left=10;                                                                    //Steven 20220407 : 避免跑到畫面外
    Top =0;
    bShow=true;
    #ifdef SOFT_SIMULTE                                                         //Steven 20190604 : 非軟體模擬不顯示
    gbManualTest->Visible=(AccessLevel>=iDefHonPrecLevel);                      //jou 2014-06-19 Security Have 5 Level 3->iDefHonPrecLevel
    #else
    gbManualTest->Visible=false;
    #endif

    pgc2DMap->ActivePageIndex   =0;
    pgc2DMap->ActivePage        =tsBarcodeAuto;
    pgcFunction->ActivePageIndex=0;

    DoIniDataToForm();
    bStartTest[3]=false;
    btnGPIBReader4->Enabled=(HasICUnderMachine()==false);
    mtBarcodeSetDefaultView();
    pgc2DID->ActivePageIndex=0;
    gbSelect2DIDUse->Visible=false;

    if(CUSTOMER_CODE==CC_KYEC_XILINX)
    {
        rgNoCodeDeviceToErr->Enabled=false;
        cbNoCodeDeviceAutoSkip->Enabled=false;
    }
    else if(CUSTOMER_CODE==CC_SCK)                                              //Steven 20200225 : SCK想要獨立控管部分2DID功能
    {
        cbNoCodeDeviceAutoSkip->Enabled=(AccessLevel>=LevelSet.AccessLevel[161]);
    }
    else if(CUSTOMER_CODE==CC_ASE_SG)                                           //Ifor 20260112 add:ASE_SG 要求不開啟
    {
        cbNoCodeDeviceAutoSkip->Checked=false;
        cbNoCodeDeviceAutoSkip->Enabled=false;
    }

    if(TestIF_File.i2DIDFormat==eAMD)                                           //JerryYang 20200422 2DID format選項改用下拉選單
    {
        gbManualTest->Visible=false;
        rgStringFormat->Visible=false;
    }
    else                                                                        //Steven 20210304 : 修正Visible=false之後就不顯示的問題
    {
        rgStringFormat->Visible=true;
    }

    if(AccessLevel>=iDefEngineerLevel)                                          //Frank 20171011 add Shuttle Check 2DID Pos
        palCheck2DID->Visible=true;
    else
        palCheck2DID->Visible=false;

    if((TestIF_File.iTestMode<=QualSite1X4  ||
       IsNNMode()==NN_1Row))//   &&
       //BAR_CODE_INSTALL!=ebcUseOCR          )                                 //Ifor 20220322 add 自制OCR不使用 //Steven 20250808 : Mark for OCR
        gbSelect2DIDUse->Visible=true;
    else
        gbSelect2DIDUse->Visible=false;

    if(CosFunction.bBarcodeErrNoTestAndShowH)
        rgNoCodeDeviceToErr->Enabled=false;

    edConsecutiveFailure->Visible=(BAR_CODE_INSTALL==ebctInShtIntel ||          //JerryYang 20200616 2DID In-house版本支援累加fail alarm功能
                                   BAR_CODE_INSTALL==ebctEtherNetCCD ||         //Ifor 20190129 : add Cognex EtherNet 通訊
                                   BAR_CODE_INSTALL==ebctUseCCDMode ||
                                   BAR_CODE_INSTALL==ebcUseOCR);                //Ifor 20210407 add: 自製OCR

    cbCheckLotHaveCode->Visible  =((CUSTOMER_CODE==CC_KYEC_XILINX ||
                                    CosFunction.b2DCodeCheckByCoustomerLot) &&
                                   (BAR_CODE_INSTALL==ebctInShtIntel ||
                                    BAR_CODE_INSTALL==ebctUseCCDMode));         //Sam 20220223 : 2D Code Check by Coustomer Lot

    cbConsecutiveFailure->Visible=(BAR_CODE_INSTALL==ebctInShtIntel ||          //JerryYang 20200616 2DID In-house版本支援累加fail alarm功能
                                   BAR_CODE_INSTALL==ebctEtherNetCCD ||         //Ifor 20190129 : add Cognex EtherNet 通訊
                                   BAR_CODE_INSTALL==ebctUseCCDMode ||
                                   BAR_CODE_INSTALL==ebcUseOCR);                //Ifor 20210407 add: 自製OCR

    tsLog->TabVisible=(BAR_CODE_INSTALL==ebctInShtIntel ||
                       BAR_CODE_INSTALL==ebctEtherNetCCD ||
                       BAR_CODE_INSTALL==ebcUseOCR);                            //Ifor 20210407 add: 自製OCR

    //Steven 20170707 (wei) : Fixed for 2DID Display
    //==>
    cbSearch2DIDByLot->Visible      =(CUSTOMER_CODE==CC_KYEC_XILINX || CosFunction.bCanUseSearch2DIDByLot);
    cbb2DIDListErrorBin->Visible    =(CUSTOMER_CODE==CC_KYEC_XILINX || CosFunction.bCanUseSearch2DIDByLot);    //Steven 20190604 : 2DID不在List內的另外分bin
    grp2DIDSorting->Visible         =CosFunction.bSortingBy2DList;              //Frank 20221122 : 2DID sorting for ATK
    cbRetryOffsetMove->Visible      =(BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR);  //Ifor 20190129 : add Cognex EtherNet 通訊    //Ifor 20210407 add: 自製OCR
    edRetryOffsetMove->Visible      =(BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR);  //Ifor 20190129 : add Cognex EtherNet 通訊    //Ifor 20210407 add: 自製OCR
    cbRetryShiftOffsetMove->Visible =(BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR);  //Ifor 20190129 : add Cognex EtherNet 通訊    //Ifor 20210407 add: 自製OCR
    edRetryShiftOffsetMove->Visible =(BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD || BAR_CODE_INSTALL==ebcUseOCR);  //Ifor 20190129 : add Cognex EtherNet 通訊    //Ifor 20210407 add: 自製OCR
    chk2DIDYield->Visible           =(BAR_CODE_INSTALL==ebctUseCCDMode);
    edt2DIDYield->Visible           =(BAR_CODE_INSTALL==ebctUseCCDMode);
    ed_2D_YieldIgnoreCnt->Visible   =(BAR_CODE_INSTALL==ebctUseCCDMode);        //JerryYang 20241104 : Ignore count變更為可以修改
    cb2DUseSubJob->Visible          =CosFunction.b2DUseSubJobFunction;          //Ifor 20200807 add:In House 2D Use Sub Job Function
    cb2DUseAnyChar->Visible         =CosFunction.b2DUseAnyCharFunction;         //Ifor 20210723 add:2D Use Any Char 收到2D資料不判斷
    gbCheckCodeByServer2DID->Visible=CosFunction.bRead2DIDFromServer;           //Jimmychiu 20230925 : read 2did in json file
    grpMakeWhite2DIDList->Visible   =CosFunction.bMakeWhite2DIDList;            //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    chkSearch2DIDToError->Visible   =(CUSTOMER_CODE!=CC_JCET);                  //RogerYang 20251214 : JCET目前只有白名單，暫不開放
    //<==
    //Steven 20170707 (wei) : Fixed for 2DID Display

    tsXML->TabVisible=(CUSTOMER_CODE==CC_Murata);                               //Steven 20200409 : Murata 2DID比對功能
    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //Steven 20170426 : 修正項目顯示
    {
        if(AccessLevel==iDefHonPrecLevel)                                       //Alick 20170223 (wei) add Xilinx 振偉需求只有最高權限hontech時，才能修改
        {
            edtBarcodeMinLength->Enabled=true;
            edtBarcodeMaxLength->Enabled=true;
        }
        else
        {
            edtBarcodeMinLength->Enabled=false;
            edtBarcodeMaxLength->Enabled=false;
        }

        if(AccessLevel>=iDefEngineerLevel)                                      //Frank 20171011 (Steven) add Shuttle Check 2DID Pos
            palCheck2DID->Visible=true;
        else
            palCheck2DID->Visible=false;
    }
    else
    {
        edtBarcodeMinLength->Enabled=true;
        edtBarcodeMaxLength->Enabled=true;
    }

    if(BAR_CODE_INSTALL==ebctEtherNetCCD)                                       //Ifor 20190225 :add Bar Code Use HandShake Communication
    {
        cbUseHandShakeCommunication->Visible=(CosFunction.bUseHandShakeCommunication);
        edHandShakeTimeOut->Visible         =(CosFunction.bUseHandShakeCommunication);
    }
    else
    {
        cbUseHandShakeCommunication->Visible=false;
        edHandShakeTimeOut->Visible         =false;
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20210817 2D File Neme
    {
        Label2D->Visible=true;
        edt2DFileName->Visible=true;
    }
    SetVisible();

    if(BAR_CODE_INSTALL==ebctUninstall && SHT_FLOATING_CHK!=0)                  //Ifor 20190225 :add Bar Code Use HandShake Communication
    {
        pgc2DID->ActivePageIndex=1;
        fBarCode->Caption="Shuttle floating detection";
    }
    else
    {
        pgc2DID->ActivePageIndex=0;
    }

    chkMulti2DID->Visible  =CosFunction.bEnableMulti2D;
    tsMulti2DID->TabVisible=TestIF_File.bEnableMulti2D;                         //Steven 20200810 : 一個IC使用多個2DID
    tsOCR->TabVisible=(BAR_CODE_INSTALL==ebcUseOCR);                            //Ifor 20210407 add: 自製OCR

    if(CUSTOMER_CODE!=CC_ASE_CL)                                                //KaiHuang 20200911 Add
    {
        cbCheckSum->Visible=false;
        lblCheckSum->Visible=false;
        edtCheckSumLength->Visible=false;
    }

    ts4CCD->TabVisible=false;
    ts8CCD->TabVisible=false;
    if(BOTTOM_2DID && BOTTOM_2DID_CCD)                                          //Bottom 8CCD
        pgcCCDSetting->ActivePageIndex=1;
    else                                                                        //Top + Bottom 4CCD
        pgcCCDSetting->ActivePageIndex=0;

    if(CosFunction.bReadClipCodeFromUnloader==true)
    {
//        pgc2DID->ActivePage=tsCCD;
        pgcCCDSetting->ActivePage=tsCCD_Unloader;
        ReadUnloaderClipIni();
    }
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
    tmr1->Enabled=false;

    if(bStartTest[3]==true)
    {
        SetNoiseDelay=false;
        TestISTimeOut=false;
        TestSocket.ClearAll();
        bStartTest[3]=false;
        tmr1->Enabled=false;
    }
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::FormDestroy(TObject *Sender)
{
    try
    {
        TimerProcess2DData->Enabled=false;
        TimerCCDInitial->Enabled=false;
        TimerDownCCDConnect->Enabled=false;
        ClientSocket_Shuttle1_A->Active=false;
        ClientSocket_Shuttle1_B->Active=false;
        ClientSocket_Shuttle2_A->Active=false;
        ClientSocket_Shuttle2_B->Active=false;
        ClientSocket_Shuttle1_A->Close();
        ClientSocket_Shuttle1_B->Close();
        ClientSocket_Shuttle2_A->Close();
        ClientSocket_Shuttle2_B->Close();

        //KaiChen 20200513 : Bottom 2D 8CCD
        //==>
        TimerBotton8CCDConnect->Enabled=false;                                  //KaiHuang 20200924 Add
        TimerBottom8CCDInitial->Enabled=false;                                  //KaiHuang 20200924 Add
        ClientSocket_Bottom_1->Active=false;
        ClientSocket_Bottom_2->Active=false;
        ClientSocket_Bottom_3->Active=false;
        ClientSocket_Bottom_4->Active=false;
        ClientSocket_Bottom_5->Active=false;
        ClientSocket_Bottom_6->Active=false;
        ClientSocket_Bottom_7->Active=false;
        ClientSocket_Bottom_8->Active=false;
        ClientSocket_Bottom_1->Close();
        ClientSocket_Bottom_2->Close();
        ClientSocket_Bottom_3->Close();
        ClientSocket_Bottom_4->Close();
        ClientSocket_Bottom_5->Close();
        ClientSocket_Bottom_6->Close();
        ClientSocket_Bottom_7->Close();
        ClientSocket_Bottom_8->Close();
        //<==
        //KaiChen 20200513 : Bottom 2D 8CCD

        for(int i=0; i<BAR_CODE_COUNT; i++)
        {
            if(Socket2DReceiveList[i]!=NULL)                                    //Steven 20161220 (jou) : 修正delete方式
                delete Socket2DReceiveList[i];
            map2DShuttleList[i].clear();
        }
        map2DList.clear();
        map2DSortingList.clear();                                               //Steven 20240515 : modified for 2D sort
        delete list2DByLot;
        delete list2DSorting;                                                   //Frank 20221122 : 2DID sorting for ATK
        delete s2DSorting;
        delete listError2DID;                                                   //JerryYang 20231218 : 2DID黑名單功能
        delete list2DWhitle;                                                    //JerryYang 20241104 : 支援2DID白名單功能
        delete list2DWhitleResult;
        delete list2DWhitleTemp;
        if(CosFunction.bReadClipCodeFromUnloader==true)
        {
            delete elUnloaderClip;
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfBarCode::FormDestroy");
    }
    LogSoftwareOffTime("TfBarCode, FormDestroy");                               //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TfBarCode::InitialBarcodeScanOutShuttle1()
{
    iInitialBarcodeOutShuttle1Task=1;
    iShtDupilcateFailCnt[0]=0;
}
//---------------------------------------------------------------------------
void TfBarCode::InitialBarcodeScanOutShuttle2()
{
    iInitialBarcodeOutShuttle2Task=1;
    iShtDupilcateFailCnt[1]=0;
}
//---------------------------------------------------------------------------
void TfBarCode::SetVisible()
{
    tsBarcodeMapping->TabVisible =CosFunction.bBarcodeTrayRecFile;              //jou 20190930 : Barcode Tray record file

    tsBarcodeAuto4->TabVisible  =(AUTO_EMPTY_COLOR>=3);                         //Steven 20240704 : Add for HT-9011UC
    tsBarcodeFix4->TabVisible   =(AUTO_EMPTY_COLOR>=3);
    tsBarcodeBinBox->TabVisible =false;
    tsMagazine1->TabVisible     =(AUTO3_IS_MAGAZINE==1);
    tsMagazine2->TabVisible     =(AUTO3_IS_MAGAZINE==1);
    grpLotVerify->Visible       =CosFunction.bLotIDVerify;                      //Steven 20240704 : Lot Verification function for ATK

    palInSht->Visible           =(BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctUseCCDMode || BAR_CODE_INSTALL==ebctEtherNetCCD);   //Ifor 20190129 : add Cognex EtherNet 通訊
    palOutSht->Visible          =(BAR_CODE_INSTALL==ebctOutShtAMD);
    gb2DTriggerCmd->Visible     =(BAR_CODE_INSTALL!=ebctUseCCDMode);
    cbTriggerMode->Visible      =false;
//    cbExposureTimeOut->Visible  =(BAR_CODE_INSTALL==ebctUseCCDMode);          //Steven 20170425 : 沒用到Mark

    ts2DID->TabVisible          =(BAR_CODE_INSTALL!=ebctUninstall);
    tsCognexSetting->TabVisible =(BAR_CODE_INSTALL==ebctInShtIntel);
    tsCCD->TabVisible           =(BAR_CODE_INSTALL==ebctUseCCDMode || BAR_CODE_INSTALL==ebctEtherNetCCD || SHT_FLOATING_CHK!=0 || BAR_CODE_INSTALL==ebcUseOCR || CosFunction.bReadClipCodeFromUnloader); //Ifor 20190129 : add Cognex EtherNet 通訊  //Ifor 20210407 add: 自製OCR
    tsFloating->TabVisible      =(SHT_FLOATING_CHK!=0);
    cbCheckCodeByShuttle->Visible=(BAR_CODE_INSTALL==ebctUseCCDMode);
    chkBottom2DID->Visible      =BOTTOM_2DID;                                   //Steven 20190308 : Bottom 2D
    grpBottom2D->Visible        =BOTTOM_2DID;
    tsCCD_Unloader->Visible     =CosFunction.bReadClipCodeFromUnloader;
    //**************
    //這部分會影響分Bin,如果要讓客戶可以選,要發信通知
    //**************
    if(IniConfig.bKoreaFunction)                                                //Steven 20171128 (Wei) : modify
    {
        cbCheckCodeByShuttle->Enabled=true;                                     //Alick 20170203 (wei) add for XILINX 使用CCD時該選項反
        edShtDuplicateRetryCnt->Visible=(BAR_CODE_INSTALL==ebctUseCCDMode);
    }
    else
    {
        cbCheckCodeByShuttle->Enabled=!(BAR_CODE_INSTALL==ebctUseCCDMode);      //Alick 20170203 (wei) add for XILINX 使用CCD時該選項反
        edShtDuplicateRetryCnt->Visible=(BAR_CODE_INSTALL==ebctUseCCDMode);
    }
    //**************
    //這部分會影響分Bin,如果要讓客戶可以選,要發信通知
    //**************
}
//---------------------------------------------------------------------------
//V3.27B.520 Steven 20170425 : 簡化2DID存檔
//---------------------------------------------------------------------------
void __fastcall TfBarCode::ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString sGroup="";

    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //Alick 20170207 (wei) add for Xilinx Barcode Length save by machine
    {
        szDir="D:\\HT9045\\system\\Barcode.ini";
        sGroup="Configuration_Barcode(XILINX)";
    }
    else
    {
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        sGroup="Configuration";
    }

    if(BAR_CODE_INSTALL==ebctUninstall)
        TestIF_File.bEnableBarCode      =false;
    else
        TestIF_File.bEnableBarCode      =ReadIniData(szDir, sGroup, "Bar Code", false);

    if(BOTTOM_2DID)                                                             //Steven 20190308 : Bottom 2D
        TestIF_File.bEnableBottom2D     =ReadIniData(szDir, sGroup, "Bottom 2D", false);
    else
        TestIF_File.bEnableBottom2D     =false;

    if(CosFunction.bEnableMulti2D)                                              //Steven 20200810 : 一個IC使用多個2DID
    {
        TestIF_File.bEnableMulti2D      =ReadIniData(szDir, sGroup, "Multi 2D", false);
        TestIF_File.dMulti2DXPitch      =ReadIniData(szDir, sGroup, "Multi 2D X Pitch", 0.0);
        TestIF_File.iMulti2DType        =ReadIniData(szDir, sGroup, "Multi 2D Typeh", 0);

        if(TestIF_File.iMulti2DType==e1x2In1CCD ||
           TestIF_File.iMulti2DType==e2x1In1CCD ||
           TestIF_File.iMulti2DType==e2x2In2CCD)
        {
            TestIF_File.iMulti2DCount=2;
        }
        else //if(TestIF_File.iMulti2DType==e2x1In2CCD)
        {
            TestIF_File.iMulti2DCount=1;
        }
//        else
//        {
//            TestIF_File.iMulti2DCount=4;
//        }

        if(TestIF_File.iMulti2DType==e1x2In1CCD)
        {
            TestIF_File.iMulti2DXItem   =2;
            TestIF_File.iMulti2DYItem   =1;
        }
        else if(TestIF_File.iMulti2DType==e2x1In2CCD)
        {
            TestIF_File.iMulti2DXItem   =1;
            TestIF_File.iMulti2DYItem   =1;
        }
        else if(TestIF_File.iMulti2DType==e2x1In1CCD)
        {
            TestIF_File.iMulti2DXItem   =1;
            TestIF_File.iMulti2DYItem   =2;
        }
        else if(TestIF_File.iMulti2DType==e2x2In2CCD)
        {
            TestIF_File.iMulti2DXItem   =2;
            TestIF_File.iMulti2DYItem   =1;
        }

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<2; j++)
            {
                S.sprintf("Multi 2D Map %d-%d", i+1, j+1);
                if(TestIF_File.iMulti2DType==e1x2In1CCD && i==1)
                    TestIF_File.iMulti2DMap[i][j]=0;
                else if((TestIF_File.iMulti2DType==e2x1In1CCD || TestIF_File.iMulti2DType==e2x1In2CCD) && j==1)
                    TestIF_File.iMulti2DMap[i][j]=0;
                else
                    TestIF_File.iMulti2DMap[i][j]=ReadIniData(szDir, sGroup, S, 0);
            }
        }
    }
    else
    {
        TestIF_File.iMulti2DCount       =1;
        TestIF_File.iMulti2DXItem       =1;
        TestIF_File.iMulti2DYItem       =1;
        TestIF_File.bEnableMulti2D      =false;
    }

    TestIF_File.dBottom2DOffsetX        =ReadIniData(szDir, sGroup, "Bottom 2D Offset X",  0.0);
    TestIF_File.dBottom2DOffsetY        =ReadIniData(szDir, sGroup, "Bottom 2D Offset Y",  0.0);

    if(TestIF_File.bEnableBottom2D)
        TestIF_File.bEnableBarCode      =true;

    TestIF_File.iBarCodeDelay           =ReadIniData(szDir, sGroup, "Bar Code Delay Time", 10000);
    TestIF_File.iBarCodePosDelay        =ReadIniData(szDir, sGroup, "Bar Code Pos Delay Time", 50);                 //wei 20151126
    TestIF_File.iBarCodePos1Delay       =ReadIniData(szDir, sGroup, "Bar Code Pos1 Delay Time", 50);                //wei 20151126

    TestIF_File.iBarCodeMinLength       =ReadIniData(szDir, sGroup, "Bar Code Min Length", 5);                      //wei 20151127 字元數比對
    TestIF_File.iBarCodeMaxLength       =ReadIniData(szDir, sGroup, "Bar Code Max Length", 30);

    TestIF_File.iCheckSumLength         =ReadIniData(szDir, sGroup, "Bar Code Check Sum Length" , 17);              //KaiChen 20191121 ：中壢日月光 2D Check Sum
    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        TestIF_File.bCheckSum            =ReadIniData(szDir, sGroup, "Bar Code Check Sum"        , false);          //KaiChen 20191121 ：中壢日月光 2D Check Sum
    }
    else
    {
        TestIF_File.bCheckSum           =false;
        cbCheckSum->Visible             =false;
    }

    TestIF_File.b2DIDAllowList          =ReadIniData(szDir, sGroup, "Check 2DID Allow List Function"        , false);           //JerryYang 20241104 : 支援2DID白名單功能

    TestIF_File.iConsecutiveFailure     =ReadIniData(szDir, sGroup, "Consecutive Failure", 3);                                  //wei 20160823 Consecutive Failure

    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                                                                           //Steven 20200909 : 將沒有2DID的IC的後續做法變成選項
        TestIF_File.iNoCodeDeviceToErr  =ReadIniData(szDir, sGroup, "Bar Code NoCodeDeviceToErr",  0);                          //Steven 20151221 : 將讀取異常的IC放到Error Bin
    else if(CUSTOMER_CODE==CC_ASE_CL || CosFunction.bBarcodeErrNoTestAndShowH)
        TestIF_File.iNoCodeDeviceToErr  =ReadIniData(szDir, sGroup, "Bar Code NoCodeDeviceToErr",  2);
    else
        TestIF_File.iNoCodeDeviceToErr  =ReadIniData(szDir, sGroup, "Bar Code NoCodeDeviceToErr",  1);

    if(CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20260112 add:ASE_SG 要求不開啟
        TestIF_File.bNoCodeDeviceAutoSkip=false;
    else
        TestIF_File.bNoCodeDeviceAutoSkip   =ReadIniData(szDir, sGroup, "Bar Code NoCodeDeviceAutoSkip", true);                     //Steven 20151221 : 讀不到Code時,自動Skip跳下一顆

    if(CUSTOMER_CODE==CC_KYEC_XILINX)
        TestIF_File.iBarcodeRetryCount      =CheckRange(ReadIniData(szDir, sGroup, "Bar Code Auto Retry", 1), 1, 10);           //Steven 20151221 : 讀不到Code時,自動Retry的次數
    else
        TestIF_File.iBarcodeRetryCount      =CheckRange(ReadIniData(szDir, sGroup, "Bar Code Auto Retry", 0), 0, 10);           //Frank 20170508 (Steven) : 賽靈思最少要做一次

    TestIF_File.b2DUseUndefinedCMD      =ReadIniData(szDir, sGroup, "Bar Code Use Undefined CMD Mode", false);                  //Ifor 20151226 :改用 自行定義 Trigger Command
    TestIF_File.str2DTriggerONCMD       =ReadIniData(szDir, sGroup, "Bar Code Undefined CMD ON", AnsiString("LON"));            //Ifor 20151226 :Trigger ON Command
    TestIF_File.str2DTriggerOFFCMD      =ReadIniData(szDir, sGroup, "Bar Code Undefined CMD OFF", AnsiString("LOFF"));          //Ifor 20151226 :Trigger OFF Command

    TestIF_File.b2DTriggerMode          =ReadIniData(szDir, sGroup, "Bar Code Enable Trigger Mode", false);                     //Steven 20151225 : 改用拍完就跑的方式
    TestIF_File.i2DTriggerTime          =CheckRange(ReadIniData(szDir, sGroup, "Bar Code Trigger Time", 1000), 1000, 10000);    //Steven 20151225 : 拍照的等待時間 //Steven 20200513 : Bar Code Trigger Time最小值1000

    //**************
    //這部分會影響分Bin,如果要讓客戶可以選,要發信通知
    //**************
    if(IniConfig.bKoreaFunction)                                                //Steven 20171128 (Wei) : modify
        TestIF_File.bCheckCodeByShuttle =ReadIniData(szDir, sGroup, "Check duplicate code by shuttle", true);                   //Steven 20160428 : 檢查2D重複碼
    else
        TestIF_File.bCheckCodeByShuttle =true;
    //**************
    //這部分會影響分Bin,如果要讓客戶可以選,要發信通知
    //**************

    TestIF_File.iShtDuplicateRetryCnt   =ReadIniData(szDir, sGroup, "ShuttleDuplicateRertyCount", 0);                           //Steven 20160823 : 蝦頭重複碼要可以自動Retry
    TestIF_File.bCheckCodeByLot         =ReadIniData(szDir, sGroup, "Check duplicate code by lot", false);                      //Steven 20160428 : 檢查2D重複碼

    if(fLotInfo!=NULL)                                                          //JimmyChiu 20211014 :   Show Duplication function "Enable" or "Disable" on the Lot Info.
    {
        if(TestIF_File.bCheckCodeByLot)
            fLotInfo->SetCheckCodeByLot(true);
        else
            fLotInfo->SetCheckCodeByLot(false);
    }

    if(CosFunction.bLotIDVerify==true)                                          //Steven 20240704 : Lot Verification function for ATK
    {
        TestIF_File.bLotIDVerify    =ReadIniData(szDir, "Lot Verification", "Enable", false);
        TestIF_File.sLotIDVerify    =ReadIniData(szDir, "Lot Verification", "Lot ID Name",      AnsiString(""));
        TestIF_File.sLotIDSubstr    =ReadIniData(szDir, "Lot Verification", "Lot ID Substr",    AnsiString(""));
        TestIF_File.iLotIDVerifyS   =ReadIniData(szDir, "Lot Verification", "Lot ID Start",     0);
        TestIF_File.iLotIDVerifyE   =ReadIniData(szDir, "Lot Verification", "Lot ID End",       0);
        TestIF_File.i2DIDStrStart   =ReadIniData(szDir, "Lot Verification", "2DID Start",       0);
        TestIF_File.i2DIDStrEnd     =ReadIniData(szDir, "Lot Verification", "2DID End",         0);
        TestIF_File.iLotIDLength    =TestIF_File.iLotIDVerifyE-TestIF_File.iLotIDVerifyS;
    }
    else
    {
        TestIF_File.bLotIDVerify=false;
    }

    if(fLotInfo!=NULL)
    {
        fLotInfo->edtLotVerify->Visible =TestIF_File.bLotIDVerify;
        fLotInfo->edtLotVerify->Text    =TestIF_File.sLotIDSubstr;
    }

    if(CUSTOMER_CODE==CC_KYEC_XILINX ||                                         //Steven 20170707 (wei) : Fixed 2DID for Korea
       CosFunction.b2DCodeCheckByCoustomerLot)                                  //Sam 20220223 : 2D Code Check by Coustomer Lot
    {
        TestIF_File.bCheckLotHaveCode   =ReadIniData(szDir, sGroup, "Check Lot have code", false);                  //wei 20160505 Barcode 比對Lot
    }
    else
    {
        TestIF_File.bCheckLotHaveCode   =false;
    }
    TestIF_File.bEnableConsecutiveFailure=ReadIniData(szDir, sGroup, "Enable Consecutive Failure", false);          //wei 20160823  Consecutive Failure
    ed_BarCodeNo->Text                  =ReadIniData(szDir, sGroup, "Bar Code Text", AnsiString("ABCDEFGHIJKLMNOP"));

    TestIF_File.bRetryOffsetMove        =ReadIniData(szDir, sGroup, "Enable Retry Offset Move", false);             //wei 20161116 Retry時先退出再進去讀取
    TestIF_File.dRetryOffsetMove        =ReadIniData(szDir, sGroup, "Retry Offset Move mm", 0);                     //wei 20161116 Retry時先退出再進去讀取

    TestIF_File.bRetryShiftOffsetMove   =ReadIniData(szDir, sGroup, "Enable Retry Shift Offset Move", false);       //wei 20161116 Retry時先退出再進去讀取(前中後)
    TestIF_File.dRetryShiftOffsetMove   =ReadIniData(szDir, sGroup, "Retry Shift Offset Move mm", 0);               //wei 20161116 Retry時先退出再進去讀取(前中後)

    TestIF_File.b2DIDYield              =ReadIniData(szDir, sGroup, "Enable Check 2DID Yield",  false);             //Steven 20171222 (Wei) : Yield Alarm of 2DID
    TestIF_File.d2DIDYield              =ReadIniData(szDir, sGroup, "2DID Controlled Yield",      99.0);            //Steven 20171222 (Wei) : Yield Alarm of 2DID

    TestIF_File.i2DYieldIgnoreCnt       =ReadIniData(szDir, sGroup, "i2DYieldIgnoreCnt",      100);                 //JerryYang 20241104 : Ignore count變更為可以修改
    TestIF_File.bSetCloseSite2DIDtoEmpty=ReadIniData(szDir, sGroup, "bSetCloseSite2DIDtoEmpty",   false);           //Steven 20190313 : Close site 2DID set to empty

    if(CosFunction.bUse2DIDAllSiteFailSetToErrBin)                              //Steven 20200702 : All site 2DID fail改成可以開關
        TestIF_File.iEnableAllSite2DIDErr=ReadIniData(szDir, sGroup, "iEnableAllSite2DIDErr",   2);
    else if(IniConfig.bKoreaFunction==true)
        TestIF_File.iEnableAllSite2DIDErr=ReadIniData(szDir, sGroup, "iEnableAllSite2DIDErr",   1);
    else
        TestIF_File.iEnableAllSite2DIDErr=ReadIniData(szDir, sGroup, "iEnableAllSite2DIDErr",   0);

    if(CosFunction.b2DUseSubJobFunction==true)                                  //Ifor 20200807 add:In House 2D Use Sub Job Function
        TestIF_File.b2DUseSubJob=ReadIniData(szDir, sGroup, "b2DUseSubJob",   true);
    else
        TestIF_File.b2DUseSubJob=false;

    if(CosFunction.b2DUseAnyCharFunction==true)                                 //Ifor 20210723 add:2D Use Any Char 收到2D資料不判斷
        TestIF_File.b2DUseAnyChar=ReadIniData(szDir, sGroup, "b2DUseAnyChar",   true);
    else
        TestIF_File.b2DUseAnyChar=false;

    if(SHT_FLOATING_CHK==0)                                                     //Steven 20160920 : IC置偏檢查
        TestIF_File.bEnableShtFloatChk  =false;
    else
        TestIF_File.bEnableShtFloatChk  =ReadIniData(szDir, sGroup, "Shuttle Float Check Enable",              false);
    TestIF_File.iSFCStartDelay          =ReadIniData(szDir, sGroup, "Shuttle Float Check Start Delay",         100);
    TestIF_File.iSFCExposureTimeOut     =ReadIniData(szDir, sGroup, "Shuttle Float Check Exposure Time Out",   100);
    TestIF_File.iSFCGetResultTimeOut    =ReadIniData(szDir, sGroup, "Shuttle Float Check Get Result Time Out", 100);
    TestIF_File.iSFCAutoRetry           =ReadIniData(szDir, sGroup, "Shuttle Float Check Auto Retry",          1);
    TestIF_File.bSFCUse2Photo           =ReadIniData(szDir, sGroup, "Shuttle Float Check Use 2 Photo",         false);
    TestIF_File.iSFCUse2PhotoOffset     =ReadIniData(szDir, sGroup, "Shuttle Float Check Use 2 Photo Offset",  10);

    TestIF_File.bSearch2DIDByLot        =ReadIniData(szDir, sGroup, "Search 2DID By Lot",                      false);  //Frank 20170316 (wei) add Search 2DID By Lot
    TestIF_File.b2DIDListErrorBin       =ReadIniData(szDir, sGroup, "Search 2DID By Lot Error Bin",            2);      //Steven 20190604 : 2DID不在List內的另外分bin
    if(CosFunction.bSortingBy2DList==true)                                      //Frank 20221122 : 2DID sorting for ATK
    {
        TestIF_File.bSortingBy2DIDList  =ReadIniData(szDir, sGroup, "Sorting By 2DID List",                    false);  //JerryYang 20190313
        TestIF_File.iActionOf2DNotInList=ReadIniData(szDir, sGroup, "iActionOf2DNotInList", 0);                         //Steven 20250707 : Action Of 2D Not In List
    }
    else
    {
        TestIF_File.bSortingBy2DIDList  =false;
        TestIF_File.iActionOf2DNotInList=0;
    }

    #ifdef SOFT_SIMULTE
    fMain->pnlSaveSummary->Visible=true;
    #else
    fMain->pnlSaveSummary->Visible=(IniConfig.bSPILFunction && TestIF_File.bSortingBy2DIDList);           //Steven 20240604 : 2D Sort加上手動存Summary功能
        #ifdef BETA_VERSION
            if(IniConfig.bSPILFunction==false)
            {
                fMain->pnlSaveSummary->Visible=(CUSTOMER_CODE==CC_AMKOR_Korea);
            }
        #endif

    #endif

    if(CosFunction.bRead2DIDFromServer==true)                                   //Jimmychiu 20230925 : read 2did in json file
    {
        TestIF_File.bCheckCodeByServer2DID=ReadIniData(szDir, sGroup, "Check Code By Server 2DID",       false);
        TestIF_File.asMes2DID_URL         =ReadIniData(szDir, sGroup, "asMes2DID_URL",                   AnsiString(""));
        lbfinalpathShow->Caption          =GetBarcodeByServerData();
    }
    else
    {
        TestIF_File.bCheckCodeByServer2DID=false;
        lbfinalpathShow->Caption="None";
    }

    if(CosFunction.bMakeWhite2DIDList==true)                                    //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        TestIF_File.bChkMakeWhite2DIDList   =ReadIniData(szDir, sGroup, "Make White 2DID List",       false);
    }
    else
    {
        TestIF_File.bChkMakeWhite2DIDList   =false;
    }

    TestIF_File.b2DIDNotExist2Error     =ReadIniData(szDir, sGroup, "Search2DIDToErrorBin",                     false);     //JerryYang 20231218 : 2DID黑名單功能
    TestIF_File.bSaveFailImage          =ReadIniData(szDir, sGroup, "Save Fail Image",                          false);     //Frank 20170408 (Steven) add Save Fail Image
    TestIF_File.s2DFileName             =ReadIniData(szDir, sGroup, "2DFileName",       fMain->cbSetupFileName->Text);      //kevin 20210817 2D FILENAME

    TestIF_File.iSelectUseCCDSh1        =ReadIniData(szDir, sGroup, "Select Use CCD Sh1",                      0);          //kevin 20210814 add Frank 20171011 add Shuttle Check 2DID Pos
    TestIF_File.iSelectUseCCDSh2        =ReadIniData(szDir, sGroup, "Select Use CCD Sh2",                      1);          //kevin 20210814 add Frank 20171011 add Shuttle Check 2DID Pos

    TestIF_File.b2DIDStringFormat       =ReadIniData(szDir, sGroup, "String Format",                           0);          //RogerYang 20181222 新增String format選項

    TestIF_File.bUseHandShakeCommunication  =ReadIniData(szDir, sGroup, "Bar Code Use HandShake Communication",    false);  //Ifor 20190225 :add Bar Code Use HandShake Communication
    TestIF_File.i2DHandShakeTimeOut         =ReadIniData(szDir, sGroup, "Bar Code HandShake Time Out", 2000);               //Ifor 20190225 :add Bar Code Use HandShake Communication

    //Ifor 20210407 add: 自製OCR
    //==>
    TestIF_File.i2DReadMultiLine         =ReadIniData(szDir, sGroup, "Bar Code Read Multi Line", 0);
    TestIF_File.i2D1stLineLength         =ReadIniData(szDir, sGroup, "Bar Code 1st Line Length", 8);
    TestIF_File.i2D2ndLineLength         =ReadIniData(szDir, sGroup, "Bar Code 2nd Line Length", 4);
    TestIF_File.as2DInsertString         =ReadIniData(szDir, sGroup, "Bar Code Insert String", AnsiString(""));
    //<==
    //Ifor 20210407 add: 自製OCR

    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //Steven 20170425 : 簡化2DID存檔
    {

        TestIF_File.iNoCodeDeviceToErr      =0;                                 //Steven 20151221 : 將讀取異常的IC放到Error Bin
        TestIF_File.bNoCodeDeviceAutoSkip   =false;                             //wei 20160331 XILINX Barcode NoCodeDeviceToErr and NoCodeDeviceAutoSkip 強制關閉

        if(BAR_CODE_INSTALL==ebctUseCCDMode)                                    //Alick 20170203 add XILINX CCD模式下強制打勾
            TestIF_File.bCheckCodeByShuttle =true;

        if(LastSet.iRealDummy!=DUMMY || LastSet.iTester!=OFF_LINE)              //wei 20161118 強制打開確認重複碼功能  //Alick 20170120 add 修正no tray nodevice時無法關閉重複碼
        {
             TestIF_File.bCheckCodeByLot=true;
             TestIF_File.bCheckLotHaveCode=true;
        }
    }

    SetVisible();

    if(BAR_CODE_INSTALL!=ebctUninstall)                                         //jou 20211029 : 需判斷是否有安裝2DID
    {
        if(TestIF_File.bEnableBarCode)                                          //Steven 20150713 : GPIB update to V2.01 for 2D Code   //wei 20151120
            fMain->SendMSG_CMD(MSG_CMD_EnableBarCode);
        else
            fMain->SendMSG_CMD(MSG_CMD_DisableBarCode);

        fMain->ShowOCRState(0);
    }

    Change2DSetupFile();
    SetSFCCheckStepCount();
    mtBarcodeSetDefaultView();
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::DoIniDataToForm()
{
    cbEnableBarCode->Checked        =TestIF_File.bEnableBarCode;
    edBarcodeScanDelayTime->Text    =AnsiString(TestIF_File.iBarCodeDelay);
    edBarcodePosDelayTime->Text     =AnsiString(TestIF_File.iBarCodePosDelay);  //wei 20151126 BarCode Pos Delay
    edFirstDelay->Text              =AnsiString(TestIF_File.iBarCodePos1Delay);
    edtBarcodeMinLength->Text       =AnsiString(TestIF_File.iBarCodeMinLength); //wei 20151127 字元數比對
    edtBarcodeMaxLength->Text       =AnsiString(TestIF_File.iBarCodeMaxLength);
    edtCheckSumLength->Text         =AnsiString(TestIF_File.iCheckSumLength);   //KaiChen 20191121 ：中壢日月光 2D Check Sum
    cbCheckSum->Checked             =TestIF_File.bCheckSum;                     //KaiChen 20191121 ：中壢日月光 2D Check Sum
    rgNoCodeDeviceToErr->ItemIndex  =TestIF_File.iNoCodeDeviceToErr;            //Steven 20151221 : 將讀取異常的IC放到Error Bin
    cbNoCodeDeviceAutoSkip->Checked =TestIF_File.bNoCodeDeviceAutoSkip;         //Steven 20151221 : 讀不到Code時,自動Skip跳下一顆
    edBarcodeRetryCount->Text       =AnsiString(TestIF_File.iBarcodeRetryCount);//Steven 20151221 : 讀不到Code時,自動Retry的次數
    cbTriggerMode->Checked          =TestIF_File.b2DTriggerMode;                //Steven 20151225 : 改用拍完就跑的方式
//    cbExposureTimeOut->Checked      =TestIF_File.bExposureTimeOut;
    cbCheckCodeByShuttle->Checked   =TestIF_File.bCheckCodeByShuttle;           //Steven 20160428 : 檢查2D重複碼
    chkBottom2DID->Checked          =TestIF_File.bEnableBottom2D;               //Steven 20190308 : Bottom 2D
    chkMulti2DID->Checked           =TestIF_File.bEnableMulti2D;                //Steven 20200810 : 一個IC使用多個2DID
    edtXPitch->Text                 =TestIF_File.dMulti2DXPitch;
    rgMulti2DType->ItemIndex        =TestIF_File.iMulti2DType;

    if(CosFunction.bLotIDVerify==true)                                          //Steven 20240704 : Lot Verification function for ATK
    {
        chkEnableLotVerify->Checked =TestIF_File.bLotIDVerify;
        edtLotIDStart->Text         =AnsiString(TestIF_File.iLotIDVerifyS);
        edtLotIDEnd->Text           =AnsiString(TestIF_File.iLotIDVerifyE);
        edt2DStart->Text            =AnsiString(TestIF_File.i2DIDStrStart);
        edt2DEnd->Text              =AnsiString(TestIF_File.i2DIDStrEnd);
        edtLotIDVerify->Text        =TestIF_File.sLotIDVerify;
        edtLotIDSubstr->Text        =TestIF_File.sLotIDSubstr;
    }

    SetMulti2DMap(TestIF_File.iMulti2DType);
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            if(TestIF_File.iMulti2DType==e1x2In1CCD && i==1)
                Multi2DSiteCH[i][j]->ItemIndex=0;
            else if((TestIF_File.iMulti2DType==e2x1In1CCD || TestIF_File.iMulti2DType==e2x1In2CCD) && j==1)
                Multi2DSiteCH[i][j]->ItemIndex=0;
            else
                Multi2DSiteCH[i][j]->ItemIndex=TestIF_File.iMulti2DMap[i][j];
        }
    }

    edtOffsetX->Text                =TestIF_File.dBottom2DOffsetX;
    edtOffsetY->Text                =TestIF_File.dBottom2DOffsetY;
    edShtDuplicateRetryCnt->Text    =TestIF_File.iShtDuplicateRetryCnt;         //Steven 20160823 : 蝦頭重複碼要可以自動Retry
    cbCheckCodeByLot->Checked       =TestIF_File.bCheckCodeByLot;               //Steven 20160428 : 檢查2D重複碼
    cbCheckLotHaveCode->Checked     =TestIF_File.bCheckLotHaveCode;             //wei 20160505 Barcode 比對Lot
    cbConsecutiveFailure->Checked   =TestIF_File.bEnableConsecutiveFailure;     //wei 20160823  Consecutive Failure
    edTriggerTime->Text             =AnsiString(TestIF_File.i2DTriggerTime);    //Steven 20151225 : 拍照的等待時間
    cb2DUseUndefinedCMD->Checked    =TestIF_File.b2DUseUndefinedCMD;            //Ifor 20151226 :改用 自行定義 Trigger Command
    edtBarcodeONCMD->Text           =TestIF_File.str2DTriggerONCMD;             //Ifor 20151226 :自行定義 Trigger ON Command
    edtBarcodeOFFCMD->Text          =TestIF_File.str2DTriggerOFFCMD;            //Ifor 20151226 :自行定義 Trigger ON Command

    cbRetryOffsetMove->Checked      =TestIF_File.bRetryOffsetMove;              //wei 20161116 Retry時先退出再進去讀取
    edRetryOffsetMove->Text         =TestIF_File.dRetryOffsetMove;              //wei 20161116 Retry時先退出再進去讀取

    cbRetryShiftOffsetMove->Checked =TestIF_File.bRetryShiftOffsetMove;         //wei 20161116 Retry時先退出再進去讀取(前中後)
    edRetryShiftOffsetMove->Text    =TestIF_File.dRetryShiftOffsetMove;         //wei 20161116 Retry時先退出再進去讀取(前中後)

    chk2DIDYield->Checked           =TestIF_File.b2DIDYield;                    //Steven 20171222 (Wei) : Yield Alarm of 2DID
    edt2DIDYield->Text              =TestIF_File.d2DIDYield;                    //Steven 20171222 (Wei) : Yield Alarm of 2DID
    ed_2D_YieldIgnoreCnt->Text      =TestIF_File.i2DYieldIgnoreCnt;             //JerryYang 20241104 : Ignore count變更為可以修改
    rgAllSiteFail->ItemIndex        =TestIF_File.iEnableAllSite2DIDErr;         //Steven 20200702 : All site 2DID fail改成可以開關
    chkCloseSite2DIDIsEmpty->Checked=TestIF_File.bSetCloseSite2DIDtoEmpty;      //Steven 20190313 : Close site 2DID set to empty
    cbSearch2DIDByLot->Checked      =TestIF_File.bSearch2DIDByLot;              //Frank 20170316 (wei) add Search 2DID By Lot
    cbb2DIDListErrorBin->ItemIndex  =TestIF_File.b2DIDListErrorBin;             //Steven 20190604 : 2DID不在List內的另外分bin
    cbSortingBy2DIDList->Checked    =TestIF_File.bSortingBy2DIDList;            //JerryYang 20190313
    rgActionOf2DSort->ItemIndex     =TestIF_File.iActionOf2DNotInList;          //Steven 20250707 : Action Of 2D Not In List

    chkSearch2DIDToError->Checked   =TestIF_File.b2DIDNotExist2Error;           //JerryYang 20231218 : 2DID黑名單功能

    cbSaveFailImage->Checked        =TestIF_File.bSaveFailImage;                //Frank 20170408 (Steven) add Save Fail Image
    edt2DFileName->Text             =TestIF_File.s2DFileName;                   //kevin 20210817 2D FILENAME
    rgSelectSh1->ItemIndex          =TestIF_File.iSelectUseCCDSh1;              //kevin 20210814 add Frank 20171011 add Shuttle Check 2DID Pos
    rgSelectSh2->ItemIndex          =TestIF_File.iSelectUseCCDSh2;              //kevin 20210814 add Frank 20171011 add Shuttle Check 2DID Pos
    cb2DUseSubJob->Checked          =TestIF_File.b2DUseSubJob;                  //Ifor 20200807 add:In House 2D Use Sub Job Function
    cb2DUseAnyChar->Checked         =TestIF_File.b2DUseAnyChar;                 //Ifor 20210723 add:2D Use Any Char 收到2D資料不判斷
    coCommReader1->Text=HSys.asBarCodeComPort[0];                               //wei 20150912 2D
    coCommReader2->Text=HSys.asBarCodeComPort[1];                               //wei 20150912 2D
    coCommReader3->Text=HSys.asBarCodeComPort[2];                               //wei 20150912 2D
    coCommReader4->Text=HSys.asBarCodeComPort[3];                               //wei 20150912 2D

    //Ifor 20151224 add
    edShuttle_1A_Address->Text=HSys.asCCDBarCodeIP[0];                          //Ifor 20151224 CCD 2D
    edShuttle_1B_Address->Text=HSys.asCCDBarCodeIP[1];                          //Ifor 20151224 CCD 2D
    edShuttle_2A_Address->Text=HSys.asCCDBarCodeIP[2];                          //Ifor 20151224 CCD 2D
    edShuttle_2B_Address->Text=HSys.asCCDBarCodeIP[3];                          //Ifor 20151224 CCD 2D

    edShuttle_1A_Port->Text=HSys.asCCDBarCodePort[0];                           //Ifor 20151224 CCD 2D
    edShuttle_1B_Port->Text=HSys.asCCDBarCodePort[1];                           //Ifor 20151224 CCD 2D
    edShuttle_2A_Port->Text=HSys.asCCDBarCodePort[2];                           //Ifor 20151224 CCD 2D
    edShuttle_2B_Port->Text=HSys.asCCDBarCodePort[3];                           //Ifor 20151224 CCD 2D

    edConsecutiveFailure->Text     =TestIF_File.iConsecutiveFailure;            //wei 20160823  Consecutive Failure

    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //wei 20160318 Barcode Error依客戶設定
    {
        asBarCodeErrorSend="BARCODEREJECT";
        if(LastSet.iRealDummy==DUMMY && LastSet.iTester==OFF_LINE)
        {
            cbCheckCodeByLot->Enabled=true;                                     //Alick 20170222 (wei) add NOTRAY NODEVICE時可關閉確認重複碼功能
            cbCheckLotHaveCode->Enabled=true;                                   //Alick 20170222 (wei) add NOTRAY NODEVICE時可關閉確認重複碼功能
        }
        else
        {
            cbCheckCodeByLot->Enabled=false;                                    //wei 20161118 強制打開確認重複碼功能
            cbCheckLotHaveCode->Enabled=false;                                  //Alick 20170222 (wei) 強制打開確認重複碼功能
        }
    }
    else
    {
        if(CosFunction.bBarcodeErrNoTestAndShowH==true)                         //jou 20191007 : Barcode Error No Test & Show "H"
        {
            asBarCodeErrorSend="H";
        }

        if(TestIF_File.i2DIDFormat==eAMD)                                       //JerryYang 20200422 2DID format選項改用下拉選單
        {
            asBarCodeErrorSend="DECODEFAIL";
        }
        else
        {
            asBarCodeErrorSend="ERROR";
        }
        if(CosFunction.b2DCodeCheckByCoustomerLot)                              //Sam 20220223 : 2D Code Check by Coustomer Lot
        {

        }
        else
        {
            cbCheckLotHaveCode->Checked=false;                                  //Steven 20170707 (wei) : Fixed 2DID for Korea
        }
    }

    cbEnableShtFloatChk->Checked    =TestIF_File.bEnableShtFloatChk;            //Steven 20160920 : IC置偏檢查
    edSFCStartDelay->Text           =TestIF_File.iSFCStartDelay;
    edSFCExposureTimeOut->Text      =TestIF_File.iSFCExposureTimeOut;
    edSFCGetResultTimeOut->Text     =TestIF_File.iSFCGetResultTimeOut;
    edSFCAutoRetry->Text            =TestIF_File.iSFCAutoRetry;
    cbSFCUse2Photo->Checked         =TestIF_File.bSFCUse2Photo;
    edSFCUse2PhotoOffset->Text      =TestIF_File.iSFCUse2PhotoOffset;

    rgStringFormat->ItemIndex       =TestIF_File.b2DIDStringFormat;             //RogerYang 20181222 新增String format選項
    cbUseHandShakeCommunication->Checked    =TestIF_File.bUseHandShakeCommunication;    //Ifor 20190225 :add Bar Code Use HandShake Communication
    edHandShakeTimeOut->Text        =AnsiString(TestIF_File.i2DHandShakeTimeOut);       //Ifor 20190225 :add Bar Code Use HandShake Communication

    cbReadMultiLine_2DBarcode->ItemIndex       =TestIF_File.i2DReadMultiLine;   //Ifor 20210407 add: 自製OCR
    edt1stLineLength_2DBarCode->Text           =TestIF_File.i2D1stLineLength;
    edt2ndLineLength_2DBarCode->Text           =TestIF_File.i2D2ndLineLength;
    edtInsertString1stAnd2nd_2DBarCode->Text   =TestIF_File.as2DInsertString;

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //KaiHuang 20200911 : Add
    {
        edtCheckSumLength->Text         =AnsiString(TestIF_File.iCheckSumLength);   //KaiChen 20191121 ：中壢日月光 2D Check Sum
        cbCheckSum->Checked             =TestIF_File.bCheckSum;                     //KaiChen 20191121 ：中壢日月光 2D Check Sum
    }
    else
    {
        edtCheckSumLength->Text         =AnsiString(TestIF_File.iCheckSumLength);
        cbCheckSum->Checked             =false;
    }

    if(CosFunction.bRead2DIDFromServer==true)                                   //Jimmychiu 20230925 : read 2did in json file
    {
        cbCheckCodeByServer2DID->Checked=TestIF_File.bCheckCodeByServer2DID;
        le2DID_ServerURL->Text=TestIF_File.asMes2DID_URL;
        lbfinalpathShow->Caption=GetBarcodeByServerData();
    }

    if(CosFunction.bMakeWhite2DIDList==true)                                    //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        chkMakeWhite2DIDList->Checked=TestIF_File.bChkMakeWhite2DIDList;
    }

    cb2DIDAllowList->Checked       =TestIF_File.b2DIDAllowList;                 //JerryYang 20241104 : 支援2DID白名單功能
    if(CosFunction.bReadClipCodeFromUnloader==true)
    {
        ReadUnloaderClipIni();
    }
}
//---------------------------------------------------------------------------
//V3.27B.520 Steven 20170425 : 簡化2DID讀檔
//---------------------------------------------------------------------------
void __fastcall TfBarCode::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString sGroup;
    AnsiString szDir_Xilinx="D:\\HT9045\\system\\Barcode.ini";                  //Alick 20170207 (wei) add for Xilinx Barcode Length save by machine
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //Alick 20170207 (wei) add for Xilinx Barcode Length save by machine
    {
        sGroup="Configuration_Barcode(XILINX)";
        szDir ="D:\\HT9045\\system\\Barcode.ini";

    }
    else
    {
        sGroup="Configuration";
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    }

    if(CosFunction.bEnableMulti2D)
    {
        if(chkMulti2DID->Checked)
            if(CheckMulti2DMap())
                return;
    }

    if(BOTTOM_2DID)                                                             //Steven 20190308 : Bottom 2D
        WriteIniData(szDir, sGroup, "Bottom 2D", (chkBottom2DID->Checked)?1:0);
    else
        WriteIniData(szDir, sGroup, "Bottom 2D", 0);

    if(CosFunction.bEnableMulti2D)                                              //Steven 20200810 : 一個IC使用多個2DID
    {
        if((rgMulti2DType->ItemIndex==e2x1In2CCD || rgMulti2DType->ItemIndex==e2x2In2CCD) &&
           TestIF_File.iTestMode>=DualSite2x1  &&
           IsNNMode()!=NN_1Row)
        {
            ShowMyMessage("Test mode with 2 mode, can not select multi 2D in 2CCD!");
            rgMulti2DType->ItemIndex=0;
            return;
        }

        WriteIniData(szDir, sGroup, "Multi 2D", (chkMulti2DID->Checked)?1:0);
        WriteIniData(szDir, sGroup, "Multi 2D X Pitch", edtXPitch->Text);
        WriteIniData(szDir, sGroup, "Multi 2D Typeh", rgMulti2DType->ItemIndex);

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<2; j++)
            {
                S.sprintf("Multi 2D Map %d-%d", i+1, j+1);
                if((rgMulti2DType->ItemIndex==e1x2In1CCD && i==1) ||
                   ((rgMulti2DType->ItemIndex==e2x1In1CCD || rgMulti2DType->ItemIndex==e2x1In2CCD) && j==1))
                    ;
                else
                    WriteIniData(szDir, sGroup, S, Multi2DSiteCH[i][j]->ItemIndex);
            }
        }
    }

    if(CosFunction.bLotIDVerify)                                                //Steven 20240704 : Lot Verification function for ATK
    {
        WriteIniData(szDir, "Lot Verification", "Enable", (chkEnableLotVerify->Checked)?1:0);
        WriteIniData(szDir, "Lot Verification", "Lot ID Name",      edtLotIDVerify->Text);
        WriteIniData(szDir, "Lot Verification", "Lot ID Substr",    edtLotIDSubstr->Text);
        WriteIniData(szDir, "Lot Verification", "Lot ID Start",     edtLotIDStart->Text);
        WriteIniData(szDir, "Lot Verification", "Lot ID End",       edtLotIDEnd->Text);
        WriteIniData(szDir, "Lot Verification", "2DID Start",       edt2DStart->Text);
        WriteIniData(szDir, "Lot Verification", "2DID End",         edt2DEnd->Text);
    }

    WriteIniData(szDir, sGroup, "Bottom 2D Offset X",             atof(edtOffsetX->Text.c_str()));
    WriteIniData(szDir, sGroup, "Bottom 2D Offset Y",             atof(edtOffsetY->Text.c_str()));

    if(BAR_CODE_INSTALL==ebctUninstall)
    {
        WriteIniData(szDir, sGroup, "Bar Code",                    false);
    }
    else
    {
        if(BOTTOM_2DID && chkBottom2DID->Checked)
            WriteIniData(szDir, sGroup, "Bar Code",                1);
        else
            WriteIniData(szDir, sGroup, "Bar Code",                (cbEnableBarCode->Checked)?1:0);
    }

    WriteIniData(szDir, sGroup, "Bar Code Delay Time",             atoi(edBarcodeScanDelayTime->Text.c_str()));
    WriteIniData(szDir, sGroup, "Bar Code Pos Delay Time",         atoi(edBarcodePosDelayTime->Text.c_str()));     //wei 20151126 BarCode Pos Delay
    WriteIniData(szDir, sGroup, "Bar Code Pos1 Delay Time",        atoi(edFirstDelay->Text.c_str()));
    WriteIniData(szDir, sGroup, "Bar Code Min Length",             atoi(edtBarcodeMinLength->Text.c_str()));       //wei 20151127 字元數比對
    WriteIniData(szDir, sGroup, "Bar Code Max Length",             atoi(edtBarcodeMaxLength->Text.c_str()));
    WriteIniData(szDir, sGroup, "Bar Code Check Sum Length",       atoi(edtCheckSumLength->Text.c_str()));         //KaiChen 20191121 ：中壢日月光 2D Check Sum
    WriteIniData(szDir, sGroup, "Bar Code Check Sum",              (cbCheckSum->Checked)?1:0);                     //KaiChen 20191121 ：中壢日月光 2D Check Sum
    WriteIniData(szDir, sGroup, "Check 2DID Allow List Function",   (cb2DIDAllowList->Checked)?1:0);               //JerryYang 20241104 : 支援2DID白名單功能
    WriteIniData(szDir, sGroup, "Bar Code NoCodeDeviceToErr",      rgNoCodeDeviceToErr->ItemIndex);                //Steven 20151221 : 將讀取異常的IC放到Error Bin
    WriteIniData(szDir, sGroup, "Bar Code NoCodeDeviceAutoSkip",   (cbNoCodeDeviceAutoSkip->Checked)?1:0);         //Steven 20151221 : 讀不到Code時,自動Skip跳下一顆
    WriteIniData(szDir, sGroup, "Bar Code Auto Retry",             edBarcodeRetryCount->Text);                     //Steven 20151221 : 讀不到Code時,自動Retry的次數
    WriteIniData(szDir, sGroup, "Bar Code Enable Trigger Mode",    (cbTriggerMode->Checked)?1:0);                  //Steven 20151225 : 改用拍完就跑的方式
    WriteIniData(szDir, sGroup, "Check duplicate code by shuttle", (cbCheckCodeByShuttle->Checked)?1:0);           //Steven 20160428 : 檢查2D重複碼
    WriteIniData(szDir, sGroup, "ShuttleDuplicateRertyCount",      (edShtDuplicateRetryCnt->Text));                //Steven 20160823 : 蝦頭重複碼要可以自動Retry
    WriteIniData(szDir, sGroup, "Check duplicate code by lot",     (cbCheckCodeByLot->Checked)?1:0);               //Steven 20160428 : 檢查2D重複碼
    WriteIniData(szDir, sGroup, "Bar Code Trigger Time",           edTriggerTime->Text);                           //Steven 20151225 : 拍照的等待時間
    WriteIniData(szDir, sGroup, "Bar Code tEXT",                   ed_BarCodeNo->Text);

    WriteIniData(szDir, sGroup, "Bar Code Use Undefined CMD Mode", (cb2DUseUndefinedCMD->Checked)?1:0);            //Ifor 20151226 :改用 自行定義 Trigger Command
    WriteIniData(szDir, sGroup, "Bar Code Undefined CMD ON",       edtBarcodeONCMD->Text);                         //Ifor 20151226 :Trigger ON Command
    WriteIniData(szDir, sGroup, "Bar Code Undefined CMD OFF",      edtBarcodeOFFCMD->Text);                        //Ifor 20151226 :Trigger OFF Command
    WriteIniData(szDir, sGroup, "Check Lot have code",             (cbCheckLotHaveCode->Checked)?1:0);             //wei 20160505 Barcode 比對Lot
    WriteIniData(szDir, sGroup, "Enable Consecutive Failure",      (cbConsecutiveFailure->Checked)?1:0);           //wei 20160823 Consecutive Failure
    WriteIniData(szDir, sGroup, "Consecutive Failure",             edConsecutiveFailure->Text);                    //wei 20160823 Consecutive Failure

    WriteIniData(szDir, sGroup, "Enable Retry Offset Move",        (cbRetryOffsetMove->Checked)?1:0);              //wei 20161116 Retry時先退出再進去讀取
    WriteIniData(szDir, sGroup, "Retry Offset Move mm",            edRetryOffsetMove->Text);                       //wei 20161116 Retry時先退出再進去讀取

    WriteIniData(szDir, sGroup, "Enable Retry Shift Offset Move",  (cbRetryShiftOffsetMove->Checked)?1:0);         //wei 20161116 Retry時先退出再進去讀取(前中後)
    WriteIniData(szDir, sGroup, "Retry Shift Offset Move mm",      edRetryShiftOffsetMove->Text);                  //wei 20161116 Retry時先退出再進去讀取(前中後)

    WriteIniData(szDir, sGroup, "Enable Check 2DID Yield",         (chk2DIDYield->Checked)?1:0);                   //Steven 20171222 (Wei) : Yield Alarm of 2DID
    WriteIniData(szDir, sGroup, "2DID Controlled Yield",           edt2DIDYield->Text);                            //Steven 20171222 (Wei) : Yield Alarm of 2DID
    WriteIniData(szDir, sGroup, "i2DYieldIgnoreCnt",               ed_2D_YieldIgnoreCnt->Text);                    //JerryYang 20241104 : Ignore count變更為可以修改

    WriteIniData(szDir, sGroup, "bSetCloseSite2DIDtoEmpty",        (chkCloseSite2DIDIsEmpty->Checked)?1:0);        //Steven 20190313 : Close site 2DID set to empty
    WriteIniData(szDir, sGroup, "iEnableAllSite2DIDErr",           rgAllSiteFail->ItemIndex);                      //Steven 20200702 : All site 2DID fail改成可以開關

    WriteIniData(szDir, sGroup, "b2DUseSubJob",                     (cb2DUseSubJob->Checked)?1:0);                 //Ifor 20200807 add:In House 2D Use Sub Job Function
    WriteIniData(szDir, sGroup, "b2DUseAnyChar",                    (cb2DUseAnyChar->Checked)?1:0);                 //Ifor 20210723 add:2D Use Any Char 收到2D資料不判斷
    //Steven 20160920 : IC置偏檢查
    WriteIniData(szDir, sGroup, "Shuttle Float Check Enable",              (cbEnableShtFloatChk->Checked)?1:0);
    WriteIniData(szDir, sGroup, "Shuttle Float Check Start Delay",         edSFCStartDelay->Text);
    WriteIniData(szDir, sGroup, "Shuttle Float Check Exposure Time Out",   edSFCExposureTimeOut->Text);
    WriteIniData(szDir, sGroup, "Shuttle Float Check Get Result Time Out", edSFCGetResultTimeOut->Text);
    WriteIniData(szDir, sGroup, "Shuttle Float Check Auto Retry",          edSFCAutoRetry->Text);
    WriteIniData(szDir, sGroup, "Shuttle Float Check Use 2 Photo",         (cbSFCUse2Photo->Checked)?1:0);
    WriteIniData(szDir, sGroup, "Shuttle Float Check Use 2 Photo Offset",  edSFCUse2PhotoOffset->Text);

    WriteIniData(szDir, sGroup, "Search 2DID By Lot",              (cbSearch2DIDByLot->Checked)?1:0);       //Frank 20170316 (wei) add Search 2DID By Lot
    WriteIniData(szDir, sGroup, "Search 2DID By Lot Error Bin",     cbb2DIDListErrorBin->ItemIndex);        //Steven 20190604 : 2DID不在List內的另外分bin
    WriteIniData(szDir, sGroup, "Sorting By 2DID List",            (cbSortingBy2DIDList->Checked)?1:0);     //JerryYang 20190313
    WriteIniData(szDir, sGroup, "iActionOf2DNotInList",             rgActionOf2DSort->ItemIndex);           //Steven 20250707 : Action Of 2D Not In List

    WriteIniData(szDir, sGroup, "Search2DIDToErrorBin",            (chkSearch2DIDToError->Checked)?1:0);    //JerryYang 20231218 : 2DID黑名單功能

    WriteIniData(szDir, sGroup, "Save Fail Image",                 (cbSaveFailImage->Checked)?1:0);         //Frank 20170408 (Steven) add Save Fail Image

    WriteIniData(szDir, sGroup, "Select Use CCD Sh1",              (rgSelectSh1->ItemIndex)?1:0);
    WriteIniData(szDir, sGroup, "Select Use CCD Sh2",              (rgSelectSh2->ItemIndex)?1:0);           //kevin 20210814 add Frank 20171011 add Shuttle Check 2DID Pos
    WriteIniData(szDir, sGroup, "2DFileName",                       edt2DFileName->Text);                   //kevin 20210817 2D FILENAME
    WriteIniData(szDir, sGroup, "String Format",                    rgStringFormat->ItemIndex);             //RogerYang 20181222 新增String format選項

    WriteIniData(szDir, sGroup, "Bar Code Use HandShake Communication",(cbUseHandShakeCommunication->Checked)?1:0);  //Ifor 20190225 :add Bar Code Use HandShake Communication
    WriteIniData(szDir, sGroup, "Bar Code HandShake Time Out",           edHandShakeTimeOut->Text);                  //Ifor 20190225 :add Bar Code Use HandShake Communication

    WriteIniData(szDir, sGroup, "Bar Code Read Multi Line", cbReadMultiLine_2DBarcode->ItemIndex);          //Ifor 20210407 add: 自製OCR
    WriteIniData(szDir, sGroup, "Bar Code 1st Line Length", edt1stLineLength_2DBarCode->Text);
    WriteIniData(szDir, sGroup, "Bar Code 2nd Line Length", edt2ndLineLength_2DBarCode->Text);
    WriteIniData(szDir, sGroup, "Bar Code Insert String",   edtInsertString1stAnd2nd_2DBarCode->Text);

    if(CosFunction.bRead2DIDFromServer==true)                                   //Jimmychiu 20230925 : read 2did in json file
    {
        WriteIniData(szDir, sGroup, "Check Code By Server 2DID", (cbCheckCodeByServer2DID->Checked)?1:0);
        WriteIniData(szDir, sGroup, "asMes2DID_URL            ", le2DID_ServerURL->Text);
    }

    if(CosFunction.bMakeWhite2DIDList==true)                                    //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        WriteIniData(szDir, sGroup, "Make White 2DID List",     (chkMakeWhite2DIDList->Checked)?1:0);
    }

    ReadFile();
    if(cbBarCodeSimulate->Checked==false)
        spbStartCom->Click();
    spbSave->Down=false;
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    if(CosFunction.bReadClipCodeFromUnloader==true)
    {
        WriteUnloaderClipIni();
        ReadUnloaderClipIni();
    }

    if(CUSTOMER_CODE==CC_JCET)                                              //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能  刷新uLotInfo選項
    {
        fLotInfo->JCETWhite2DIDShow(JCETUseMakeWhite2DIDList());
    }
}
//---------------------------------------------------------------------------
void TfBarCode::mtBarcodeSetDefaultView()
{
    if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)
    {
        fLotInfo->sgBarcode->Cells[ 0][ 0]="In Arm";
        fLotInfo->sgBarcode->Cells[ 1][ 0]="1_A/C";
        fLotInfo->sgBarcode->Cells[ 2][ 0]="1_E/G";
        fLotInfo->sgBarcode->Cells[ 3][ 0]="2_B/D";
        fLotInfo->sgBarcode->Cells[ 4][ 0]="2_F/H";
        fLotInfo->sgBarcode->Cells[ 5][ 0]="Total";
        fLotInfo->sgBarcode->Cells[ 0][ 1]="Load";
        fLotInfo->sgBarcode->Cells[ 0][ 2]="Pass";
        fLotInfo->sgBarcode->Cells[ 0][ 3]="Fail";
        fLotInfo->sgBarcode->Cells[ 0][ 4]="Rate(%)";
        fLotInfo->sgBarcode->Cells[ 0][ 5]="Retry";
        fLotInfo->sgBarcode->Cells[ 0][ 6]="Duplicate";

        mtBarcodeInSh->YItem=3;
        mtBarcodeInSh->XItem=4;
        mtBarcodeInSh->SetCellNumber(0, 0, "1");
        mtBarcodeInSh->SetCellColorIndex(0, 0, 3);
        mtBarcodeInSh->SetCellNumber(1, 0, "2");
        mtBarcodeInSh->SetCellColorIndex(1, 0, 3);
        mtBarcodeInSh->SetCellNumber(2, 0, "3");
        mtBarcodeInSh->SetCellColorIndex(2, 0, 3);
        mtBarcodeInSh->SetCellNumber(3, 0, "4");
        mtBarcodeInSh->SetCellColorIndex(3, 0, 3);

        myInShuttleLabel->YItem=3;
        myInShuttleLabel->XItem=1;
        myInShuttleLabel->SetCellNumber(0, 0, "In Arm");
        myInShuttleLabel->SetCellColorIndex(0, 0, 3);
        myInShuttleLabel->SetCellNumber(0, 1, "Row A");
        myInShuttleLabel->SetCellColorIndex(0, 1, 3);
        myInShuttleLabel->SetCellNumber(0, 2, "Row B");
        myInShuttleLabel->SetCellColorIndex(0, 2, 3);
    }
    else
    {
        fLotInfo->sgBarcode->Cells[ 0][ 0]="Shuttle";
        fLotInfo->sgBarcode->Cells[ 1][ 0]="1_A";
        fLotInfo->sgBarcode->Cells[ 2][ 0]="1_B";
        fLotInfo->sgBarcode->Cells[ 3][ 0]="2_A";
        fLotInfo->sgBarcode->Cells[ 4][ 0]="2_B";
        fLotInfo->sgBarcode->Cells[ 5][ 0]="Total";
        fLotInfo->sgBarcode->Cells[ 0][ 1]="Load";
        fLotInfo->sgBarcode->Cells[ 0][ 2]="Pass";
        fLotInfo->sgBarcode->Cells[ 0][ 3]="Fail";
        fLotInfo->sgBarcode->Cells[ 0][ 4]="Rate(%)";
        fLotInfo->sgBarcode->Cells[ 0][ 5]="Retry";
        fLotInfo->sgBarcode->Cells[ 0][ 6]="Duplicate";

        mtBarcodeInSh->YItem=9;
        mtBarcodeInSh->XItem=4;
        mtBarcodeInSh->SetCellNumber(0, 0, "A1");
        mtBarcodeInSh->SetCellColorIndex(0, 0, 3);
        mtBarcodeInSh->SetCellNumber(1, 0, "B1");
        mtBarcodeInSh->SetCellColorIndex(1, 0, 3);
        mtBarcodeInSh->SetCellNumber(2, 0, "A2");
        mtBarcodeInSh->SetCellColorIndex(2, 0, 3);
        mtBarcodeInSh->SetCellNumber(3, 0, "B2");
        mtBarcodeInSh->SetCellColorIndex(3, 0, 3);

        myInShuttleLabel->YItem=9;
        myInShuttleLabel->XItem=1;
        myInShuttleLabel->SetCellNumber(0, 0, "In Shuttle");
        myInShuttleLabel->SetCellColorIndex(0, 0, 3);
        myInShuttleLabel->SetCellNumber(0, 1, "a");
        myInShuttleLabel->SetCellColorIndex(0, 1, 3);
        myInShuttleLabel->SetCellNumber(0, 2, "b");
        myInShuttleLabel->SetCellColorIndex(0, 2, 3);
        myInShuttleLabel->SetCellNumber(0, 3, "c");
        myInShuttleLabel->SetCellColorIndex(0, 3, 3);
        myInShuttleLabel->SetCellNumber(0, 4, "d");
        myInShuttleLabel->SetCellColorIndex(0, 4, 3);
        myInShuttleLabel->SetCellNumber(0, 5, "e");
        myInShuttleLabel->SetCellColorIndex(0, 5, 3);
        myInShuttleLabel->SetCellNumber(0, 6, "f");
        myInShuttleLabel->SetCellColorIndex(0, 6, 3);
        myInShuttleLabel->SetCellNumber(0, 7, "g");
        myInShuttleLabel->SetCellColorIndex(0, 7, 3);
        myInShuttleLabel->SetCellNumber(0, 8, "h");
        myInShuttleLabel->SetCellColorIndex(0, 8, 3);
    }
    mtBarcodeOutSh->SetCellNumber(0, 0, "A1");
    mtBarcodeOutSh->SetCellColorIndex(0, 0, 3);
    mtBarcodeOutSh->SetCellNumber(1, 0, "B1");
    mtBarcodeOutSh->SetCellColorIndex(1, 0, 3);
    mtBarcodeOutSh->SetCellNumber(2, 0, "A2");
    mtBarcodeOutSh->SetCellColorIndex(2, 0, 3);
    mtBarcodeOutSh->SetCellNumber(3, 0, "B2");
    mtBarcodeOutSh->SetCellColorIndex(3, 0, 3);

    myOutShuttleLabel->SetCellNumber(0, 0, "Out Shuttle");
    myOutShuttleLabel->SetCellColorIndex(0, 0, 3);
    myOutShuttleLabel->SetCellNumber(0, 1, "a");
    myOutShuttleLabel->SetCellColorIndex(0, 1, 3);
    myOutShuttleLabel->SetCellNumber(0, 2, "b");
    myOutShuttleLabel->SetCellColorIndex(0, 2, 3);
    myOutShuttleLabel->SetCellNumber(0, 3, "c");
    myOutShuttleLabel->SetCellColorIndex(0, 3, 3);
    myOutShuttleLabel->SetCellNumber(0, 4, "d");
    myOutShuttleLabel->SetCellColorIndex(0, 4, 3);
    myOutShuttleLabel->SetCellNumber(0, 5, "e");
    myOutShuttleLabel->SetCellColorIndex(0, 5, 3);
    myOutShuttleLabel->SetCellNumber(0, 6, "f");
    myOutShuttleLabel->SetCellColorIndex(0, 6, 3);
    myOutShuttleLabel->SetCellNumber(0, 7, "g");
    myOutShuttleLabel->SetCellColorIndex(0, 7, 3);
    myOutShuttleLabel->SetCellNumber(0, 8, "h");
    myOutShuttleLabel->SetCellColorIndex(0, 8, 3);

    mtBarcodeInSh->SetCellNumber(0, 0, "A1");
    mtBarcodeInSh->SetCellColorIndex(0, 0, 3);
    mtBarcodeInSh->SetCellNumber(1, 0, "B1");
    mtBarcodeInSh->SetCellColorIndex(1, 0, 3);
    mtBarcodeInSh->SetCellNumber(2, 0, "A2");
    mtBarcodeInSh->SetCellColorIndex(2, 0, 3);
    mtBarcodeInSh->SetCellNumber(3, 0, "B2");
    mtBarcodeInSh->SetCellColorIndex(3, 0, 3);

     //kevin 20191004 myInShuttleLabel->SetCellNumber(0, 0, "In Shuttle");
    myInShuttleLabel->SetCellColorIndex(0, 0, 3);
    myInShuttleLabel->SetCellNumber(0, 1, "a");
    myInShuttleLabel->SetCellColorIndex(0, 1, 3);
    myInShuttleLabel->SetCellNumber(0, 2, "b");
    myInShuttleLabel->SetCellColorIndex(0, 2, 3);
    myInShuttleLabel->SetCellNumber(0, 3, "c");
    myInShuttleLabel->SetCellColorIndex(0, 3, 3);
    myInShuttleLabel->SetCellNumber(0, 4, "d");
    myInShuttleLabel->SetCellColorIndex(0, 4, 3);
    myInShuttleLabel->SetCellNumber(0, 5, "e");
    myInShuttleLabel->SetCellColorIndex(0, 5, 3);
    myInShuttleLabel->SetCellNumber(0, 6, "f");
    myInShuttleLabel->SetCellColorIndex(0, 6, 3);
    myInShuttleLabel->SetCellNumber(0, 7, "g");
    myInShuttleLabel->SetCellColorIndex(0, 7, 3);
    myInShuttleLabel->SetCellNumber(0, 8, "h");
    myInShuttleLabel->SetCellColorIndex(0, 8, 3);

    mtSFC_Shuttle->SetCellNumber(0, 0, "A1");
    mtSFC_Shuttle->SetCellColorIndex(0, 0, 3);
    mtSFC_Shuttle->SetCellNumber(1, 0, "B1");
    mtSFC_Shuttle->SetCellColorIndex(1, 0, 3);
    mtSFC_Shuttle->SetCellNumber(2, 0, "A2");
    mtSFC_Shuttle->SetCellColorIndex(2, 0, 3);
    mtSFC_Shuttle->SetCellNumber(3, 0, "B2");
    mtSFC_Shuttle->SetCellColorIndex(3, 0, 3);

    mtSFC_Label1->SetCellNumber(0, 0, "In Shuttle");
    mtSFC_Label1->SetCellColorIndex(0, 0, 3);
    if(TestIF_File.bSFCUse2Photo)
    {
        mtSFC_Label1->YItem=17;
        mtSFC_Label1->SetCellNumber(0, 1, "a1");
        mtSFC_Label1->SetCellColorIndex(0, 1, 3);
        mtSFC_Label1->SetCellNumber(0, 2, "a2");
        mtSFC_Label1->SetCellColorIndex(0, 2, 3);
        mtSFC_Label1->SetCellNumber(0, 3, "b1");
        mtSFC_Label1->SetCellColorIndex(0, 3, 3);
        mtSFC_Label1->SetCellNumber(0, 4, "b2");
        mtSFC_Label1->SetCellColorIndex(0, 4, 3);
        mtSFC_Label1->SetCellNumber(0, 5, "c1");
        mtSFC_Label1->SetCellColorIndex(0, 5, 3);
        mtSFC_Label1->SetCellNumber(0, 6, "c2");
        mtSFC_Label1->SetCellColorIndex(0, 6, 3);
        mtSFC_Label1->SetCellNumber(0, 7, "d1");
        mtSFC_Label1->SetCellColorIndex(0, 7, 3);
        mtSFC_Label1->SetCellNumber(0, 8, "d2");
        mtSFC_Label1->SetCellColorIndex(0, 8, 3);

        mtSFC_Label1->SetCellNumber(0, 9, "e1");
        mtSFC_Label1->SetCellColorIndex(0, 9, 3);
        mtSFC_Label1->SetCellNumber(0, 10, "e2");
        mtSFC_Label1->SetCellColorIndex(0, 10, 3);
        mtSFC_Label1->SetCellNumber(0, 11, "f1");
        mtSFC_Label1->SetCellColorIndex(0, 11, 3);
        mtSFC_Label1->SetCellNumber(0, 12, "f2");
        mtSFC_Label1->SetCellColorIndex(0, 12, 3);
        mtSFC_Label1->SetCellNumber(0, 13, "g1");
        mtSFC_Label1->SetCellColorIndex(0, 13, 3);
        mtSFC_Label1->SetCellNumber(0, 14, "g2");
        mtSFC_Label1->SetCellColorIndex(0, 14, 3);
        mtSFC_Label1->SetCellNumber(0, 15, "h1");
        mtSFC_Label1->SetCellColorIndex(0, 15, 3);
        mtSFC_Label1->SetCellNumber(0, 16, "h2");
        mtSFC_Label1->SetCellColorIndex(0, 16, 3);
    }
    else
    {
        mtSFC_Label1->YItem=9;
        mtSFC_Label1->SetCellNumber(0, 1, "a");
        mtSFC_Label1->SetCellColorIndex(0, 1, 3);
        mtSFC_Label1->SetCellNumber(0, 2, "b");
        mtSFC_Label1->SetCellColorIndex(0, 2, 3);
        mtSFC_Label1->SetCellNumber(0, 3, "c");
        mtSFC_Label1->SetCellColorIndex(0, 3, 3);
        mtSFC_Label1->SetCellNumber(0, 4, "d");
        mtSFC_Label1->SetCellColorIndex(0, 4, 3);
        mtSFC_Label1->SetCellNumber(0, 5, "e");
        mtSFC_Label1->SetCellColorIndex(0, 5, 3);
        mtSFC_Label1->SetCellNumber(0, 6, "f");
        mtSFC_Label1->SetCellColorIndex(0, 6, 3);
        mtSFC_Label1->SetCellNumber(0, 7, "g");
        mtSFC_Label1->SetCellColorIndex(0, 7, 3);
        mtSFC_Label1->SetCellNumber(0, 8, "h");
        mtSFC_Label1->SetCellColorIndex(0, 8, 3);
    }
}
//---------------------------------------------------------------------------
AnsiString TfBarCode::GetSimuBarCodeName(int iRow, int iCol)
{
    AnsiString asReturnString="";
    AnsiString asNo="";
    AnsiString asTail="";

    asNo.sprintf("%05d", iBarCodeNo[iRow][iCol]);

    ed_BarCodeNo->Text=ed_BarCodeNo->Text.SubString(1, 16);

    if(TestIF_File.b2DIDStringFormat==0)
        asReturnString.sprintf("%s%02d%02d%s", ed_BarCodeNo->Text.c_str(), iRow, iCol, asNo.c_str());
    else if(TestIF_File.b2DIDStringFormat==1)
        asReturnString.sprintf("%s-%02d%02d%s", ed_BarCodeNo->Text.c_str(), iRow, iCol, asNo.c_str());
    else if(TestIF_File.b2DIDStringFormat==2)                                   //Steven 20210303 : Add "."
        asReturnString.sprintf("%s.%02d%02d%s", ed_BarCodeNo->Text.c_str(), iRow, iCol, asNo.c_str());
    else if(TestIF_File.b2DIDStringFormat==3)                                   //Steven 20210303 : Add "."
        asReturnString.sprintf("%s.%02d%02d-%s", ed_BarCodeNo->Text.c_str(), iRow, iCol, asNo.c_str());
    else if(TestIF_File.b2DIDStringFormat==4)                                   //Steven 20210303 : Add "."
        asReturnString.sprintf("%s-%02d%02d.%s", ed_BarCodeNo->Text.c_str(), iRow, iCol, asNo.c_str());
    else if(TestIF_File.b2DIDStringFormat==5)                                   //Steven 20210518 : 2DID改為找非法字元方式
        asReturnString.sprintf("%s-%02d %02d.%s", ed_BarCodeNo->Text.c_str(), iRow, iCol, asNo.c_str());
    iBarCodeNo[iRow][iCol]++;

    if(iBarCodeNo[iRow][iCol]>99999)
        iBarCodeNo[iRow][iCol]=0;

    return asReturnString;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::InitBarCodeRS232(int iStep)
{
    #ifndef SOFT_SIMULTE
    bool flag[BAR_CODE_COUNT];
    AnsiString str;

    for(int i=0; i<BAR_CODE_COUNT; i++)
    {
        if(BAR_CODE_USECOUNT<=2)                                                //Ifor 20190409 : add Shuttle 僅用兩顆CCD
        {
            if(i==1 || i==3)
            {
                continue;
            }
        }
        flag[i]=GetCOMPortStatus(HSys.asBarCodeComPort[i]);
        if(flag[i]==false)
        {
            str.sprintf("Bar Code %d: %s port status error!", i+1, HSys.asBarCodeComPort[i]);
            ShowMyMessage(str);
            continue;
        }

        BarcodeCOM[i]->BaudRate = HSys.InBarcodeBaudRate;
        if(HSys.BarcodeByteSize==5)
            BarcodeCOM[i]->ByteSize = TByteSize(_5);
        else if(HSys.BarcodeByteSize==6)
            BarcodeCOM[i]->ByteSize = TByteSize(_6);
        else if(HSys.BarcodeByteSize==7)
            BarcodeCOM[i]->ByteSize = TByteSize(_7);
        else if(HSys.BarcodeByteSize==8)
            BarcodeCOM[i]->ByteSize = TByteSize(_8);

        if(HSys.BarcodeStopBit==1)
            BarcodeCOM[i]->StopBits = TStopBits(_1);
        else if(HSys.BarcodeStopBit==1.5)
            BarcodeCOM[i]->StopBits = TStopBits(_1_5);
        else if(HSys.BarcodeStopBit==2)
            BarcodeCOM[i]->StopBits = TStopBits(_2);

        if(HSys.BarcodeParity=="None")
            BarcodeCOM[i]->Parity   = None;
        else if(HSys.BarcodeParity=="Odd")
            BarcodeCOM[i]->Parity   = Odd;
        else if(HSys.BarcodeParity=="Even")
            BarcodeCOM[i]->Parity   = Even;
        else if(HSys.BarcodeParity=="Space")
            BarcodeCOM[i]->Parity   = Space;
        else if(HSys.BarcodeParity=="Mark")
            BarcodeCOM[i]->Parity   = TParity(3);

        BarcodeCOM[i]->CommName="\\\\.\\"+HSys.asBarCodeComPort[i];
        try
        {
            BarcodeCOM[i]->StopComm();
            BarcodeCOM[i]->StartComm();                                         //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfBarCode::InitBarCodeRS232");
            str.sprintf("Bar Code %d: COM%d port start error!", i+1, HSys.asBarCodeComPort[i]);
            ShowMyMessage(str);
        }
    }
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::Barcode_1ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString DataStr, Path, Data;
    char *data;
    data=(char*)Buffer;
    int len=0;
    AnsiString cStr;
    char cStr1[1024]={0};

    if(BufferLength>1024)                                                       //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }

    if(bImageSaveflag[iBarCode1_1]==true)                                       //Frank 20170408 (Steven) add Save Fail Image
    {
        BarCode_SaveImage[0]->Write(Buffer, BufferLength);
        SaveFailImage(0);
        return;
    }

    strncpy(cStr1, data, BufferLength);

    Data.sprintf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate);

    MyForceDirectories(Path, "Barcode_1ReceiveData");
    cStr=DoBarcodeAnalysis(cStr1);                                              //wei 20161223 Barcode (Steven) 品質顯示

    len=cStr.Length();

    if(len>TestIF_File.iBarCodeMaxLength)
    {
        DataStr=asBarCodeErrorSend;                                             //wei 20160318 Barcode Error依客戶設定
    }
    else if(len>=TestIF_File.iBarCodeMinLength &&
            len<=TestIF_File.iBarCodeMaxLength)                                 //wei 20151127 字元數比對
    {
        fNote->t2DCode->SetCellNumber(0, iSH1_1BarcodePosition, cStr);
        fNote->t2DCode->SetCellColorIndex(0, iSH1_1BarcodePosition, 0);

        if(bBarcodeAutoSkip[iBarCode1_1]==true)                                 //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
        {
            bBarcodeAutoSkipError[iBarCode1_1]=true;
            bBarcodeAutoSkip[iBarCode1_1]=false;
        }
        else if(DoBarcodeCheck(iBarCode1_1, cStr, iSH1_1BarcodePosition)==false)//wei 20160505 Barcode 比對Lot
        {
            bBarcodeDataSaveReady[iBarCode1_1]=false;
            fNote->t2DCode->SetCellColorIndex(0, iSH1_1BarcodePosition, 3);
        }
        else
        {
            DataStr.sprintf("%s InShuttle1-1 %d Site%d : %s", Data.c_str(), iSH1_1BarcodePosition, TestIF_File.iSiteMap[0][iSH1_1BarcodePosition], cStr1);   //wei 20160325 Barcode 紀錄位置
            lb_2DBarcodeData->Items->Add(Data);
            lb_2DBarcodeData->Items->Add("InShuttle1-1 : " + AnsiString(cStr1));
            Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
            WriteDataToFile(Path.c_str() , DataStr.c_str());
            if(lb_2DBarcodeData->Items->Count>100)
                lb_2DBarcodeData->Items->Clear();
            mtBarcodeInSh->SetCellNumber(0, iSH1_1BarcodePosition+1, cStr);     //wei 20150912 2D

            if(TestIF_File.i2DIDFormat==eAMD)                                   //JerryYang 20200422 2DID format選項改用下拉選單
            {
                fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(0, iSH1_1BarcodePosition+1, cStr);
            }
            bBarcodeDataSaveReady[iBarCode1_1]=true;
        }
    }
    Memo1->Lines->Add(cStr);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::Barcode_2ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString DataStr, Path, Data;
    char *data;
    data=(char*)Buffer;
    int len=0;
    AnsiString cStr;
    char cStr1[1024]={0};

    if(BufferLength>1024)                                                       //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }

    if(bImageSaveflag[iBarCode1_2]==true)                                       //Frank 20170408 (Steven) add Save Fail Image
    {
        BarCode_SaveImage[1]->Write(Buffer, BufferLength);
        SaveFailImage(1);
        return;
    }

    strncpy(cStr1, data, BufferLength);

    Data.sprintf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate);

    MyForceDirectories(Path, "Barcode_2ReceiveData");
    cStr=DoBarcodeAnalysis(cStr1);                                              //wei 20161223 (Steven) Barcode 品質顯示

    len=cStr.Length();

    if(len>TestIF_File.iBarCodeMaxLength)
    {
        DataStr=asBarCodeErrorSend;                                             //wei 20160318 Barcode Error依客戶設定
    }
    else if(len>=TestIF_File.iBarCodeMinLength &&
            len<=TestIF_File.iBarCodeMaxLength)                                 //wei 20151127 字元數比對
    {
        fNote->t2DCode->SetCellNumber(1, iSH1_2BarcodePosition, cStr);
        fNote->t2DCode->SetCellColorIndex(1, iSH1_2BarcodePosition, 0);

        if(bBarcodeAutoSkip[iBarCode1_2]==true)                                 //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
        {
            bBarcodeAutoSkipError[iBarCode1_2]=true;
            bBarcodeAutoSkip[iBarCode1_2]=false;
        }
        else if(DoBarcodeCheck(iBarCode1_2, cStr, iSH1_2BarcodePosition)==false)//wei 20160505 Barcode 比對Lot
        {
            bBarcodeDataSaveReady[iBarCode1_2]=false;
            fNote->t2DCode->SetCellColorIndex(1, iSH1_2BarcodePosition, 3);
        }
        else
        {
            DataStr.sprintf("%s InShuttle1-2 %d Site%d : %s", Data.c_str(), iSH1_2BarcodePosition, TestIF_File.iSiteMap[1][iSH1_2BarcodePosition], cStr1);   //wei 20160325 Barcode 紀錄位置
            lb_2DBarcodeData->Items->Add(Data);
            lb_2DBarcodeData->Items->Add("InShuttle1-2 : " + AnsiString(cStr1));
            Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
            WriteDataToFile(Path.c_str() , DataStr.c_str());
            if(lb_2DBarcodeData->Items->Count>100)
                lb_2DBarcodeData->Items->Clear();
            mtBarcodeInSh->SetCellNumber(1, iSH1_2BarcodePosition+1, cStr);     //wei 20150912 2D
            bBarcodeDataSaveReady[iBarCode1_2]=true;
        }
    }
    Memo1->Lines->Add(cStr);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::Barcode_3ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString DataStr, Path, Data;
    char *data;
    data=(char*)Buffer;
    int len=0;
    AnsiString cStr;
    char cStr1[1024]={0};

    if(BufferLength>1024)                                                       //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }

    if(bImageSaveflag[iBarCode2_1]==true)                                       //Frank 20170408 (Steven) add Save Fail Image
    {
        BarCode_SaveImage[2]->Write(Buffer, BufferLength);
        SaveFailImage(2);
        return;
    }

    strncpy(cStr1, data, BufferLength);

    Data.sprintf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path, "Barcode_3ReceiveData");

    cStr=DoBarcodeAnalysis(cStr1);                                              //wei 20161223 Barcode 品質顯示

    len=cStr.Length();

    if(len>TestIF_File.iBarCodeMaxLength)
    {
        DataStr=asBarCodeErrorSend;                                             //wei 20160318 Barcode Error依客戶設定
    }
    else if(len>=TestIF_File.iBarCodeMinLength &&
            len<=TestIF_File.iBarCodeMaxLength)                                 //wei 20151127 字元數比對
    {
        fNote->t2DCode->SetCellNumber(0, iSH2_1BarcodePosition, cStr);
        fNote->t2DCode->SetCellColorIndex(0, iSH2_1BarcodePosition, 0);

        if(bBarcodeAutoSkip[iBarCode2_1]==true)                                 //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
        {
            bBarcodeAutoSkipError[iBarCode2_1]=true;
            bBarcodeAutoSkip[iBarCode2_1]=false;
        }
        else if(DoBarcodeCheck(iBarCode2_1, cStr, iSH2_1BarcodePosition)==false)//wei 20160505 Barcode 比對Lot
        {
            bBarcodeDataSaveReady[iBarCode2_1]=false;
            fNote->t2DCode->SetCellColorIndex(0, iSH2_1BarcodePosition, 3);
        }
        else
        {
            DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], cStr1);   //wei 20160325 Barcode 紀錄位置
            lb_2DBarcodeData->Items->Add(Data);
            lb_2DBarcodeData->Items->Add("InShuttle2-1 : " + AnsiString(cStr1));
            Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
            WriteDataToFile(Path.c_str() , DataStr.c_str());
            if(lb_2DBarcodeData->Items->Count>100)
                lb_2DBarcodeData->Items->Clear();
            mtBarcodeInSh->SetCellNumber(2, iSH2_1BarcodePosition+1, cStr);     //wei 20150912 2D

            if(TestIF_File.i2DIDFormat==eAMD)                                   //JerryYang 20200422 2DID format選項改用下拉選單
            {
                fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iSH2_1BarcodePosition+1, cStr);
            }
            bBarcodeDataSaveReady[iBarCode2_1]=true;
        }
    }
    Memo2->Lines->Add(cStr);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::Barcode_4ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString DataStr, Path, Data;
    char *data;
    data=(char*)Buffer;
    int len=0;
    AnsiString cStr;
    char cStr1[1024]={0};

    if(BufferLength>1024)                                                       //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        fMain->AddTorqueLog(LogMsg);
        fMain->slTorqueLog->AddTextWithDateTime(LogMsg);
        return;
    }

    if(bImageSaveflag[iBarCode2_2]==true)                                       //Frank 20170408 (Steven) add Save Fail Image
    {
        BarCode_SaveImage[3]->Write(Buffer, BufferLength);
        SaveFailImage(3);
        return;
    }

    strncpy(cStr1, data, BufferLength);

    Data.sprintf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate);

    MyForceDirectories(Path, "Barcode_4ReceiveData");

    cStr=DoBarcodeAnalysis(cStr1);                                              //wei 20161223 (Steven) Barcode 品質顯示

    len=cStr.Length();

    if(len>TestIF_File.iBarCodeMaxLength)
    {
        DataStr=asBarCodeErrorSend;                                             //wei 20160318 Barcode Error依客戶設定
    }
    else if(len>=TestIF_File.iBarCodeMinLength &&
            len<=TestIF_File.iBarCodeMaxLength)                                 //wei 20151127 字元數比對
    {
        fNote->t2DCode->SetCellNumber(1, iSH2_2BarcodePosition, cStr);
        fNote->t2DCode->SetCellColorIndex(1, iSH2_2BarcodePosition, 0);

        if(bBarcodeAutoSkip[iBarCode2_2]==true)                                 //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
        {
            bBarcodeAutoSkipError[iBarCode2_2]=true;
            bBarcodeAutoSkip[iBarCode2_2]=false;
        }
        else if(DoBarcodeCheck(iBarCode2_2, cStr, iSH2_2BarcodePosition)==false)//wei 20160505 Barcode 比對Lot
        {
            bBarcodeDataSaveReady[iBarCode2_2]=false;
            fNote->t2DCode->SetCellColorIndex(1, iSH2_2BarcodePosition, 3);
        }
        else
        {
            DataStr.sprintf("%s InShuttle2-2 %d Site%d : %s", Data.c_str(), iSH2_2BarcodePosition, TestIF_File.iSiteMap[1][iSH2_2BarcodePosition], cStr1);   //wei 20160325 Barcode 紀錄位置
            lb_2DBarcodeData->Items->Add(Data);
            lb_2DBarcodeData->Items->Add("InShuttle2-2 : " + AnsiString(cStr1));
            Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
            WriteDataToFile(Path.c_str() , DataStr.c_str());
            if(lb_2DBarcodeData->Items->Count>100)
                lb_2DBarcodeData->Items->Clear();
            mtBarcodeInSh->SetCellNumber(3, iSH2_2BarcodePosition+1, cStr);     //wei 20150912 2D
            bBarcodeDataSaveReady[iBarCode2_2]=true;
        }
    }
    Memo2->Lines->Add(cStr);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnClearClick(TObject *Sender)
{
    lb_2DBarcodeData->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnClearMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btnClear->BevelOuter=bvLowered;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnClearMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    btnClear->BevelOuter=bvRaised;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btn_2DBarcodeDataSendClick(TObject *Sender)
{
    int lenstr;
    char edstr[256];
    AnsiString str;

    strncpy(edstr, (ed_2DBarcode->Text+"\r").c_str(), sizeof(edstr));
    lenstr=strlen(edstr);
    if(cb_2DBarcodeTestMode->Checked==true)
    {
        BarcodeCOM[rg_2DBarcodeWhichNum->ItemIndex]->WriteCommData(edstr, lenstr);
    }

    str.sprintf("%04d.%02d.%02d_%02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    lb_2DBarcodeData->Items->Add(str);
    if(lb_2DBarcodeData->Items->Count>100)
        lb_2DBarcodeData->Items->Clear();
}
//---------------------------------------------------------------------------
bool TfBarCode::Barcode_StartScan_In(int BarCodeIndex, int iXpos, AnsiString &cDeviceInf, AnsiString cLastDeviceInf)
{
    int lenOnstr;
    char Onstr[128];
    AnsiString Re="";

    AnsiString strTrigCMD="";

    if(BAR_CODE_INSTALL==ebctEtherNetCCD &&
       CosFunction.bUseHandShakeCommunication &&                                //Ifor 20190225 :add Bar Code Use HandShake Communication
       TestIF_File.bUseHandShakeCommunication==true)
    {
        if(bCCDBarcodeWaitReply[BarCodeIndex]==true)
        {
            return false;
        }
    }
    else
    {
        bCCDBarcodeWaitReply[BarCodeIndex]=false;
    }

    if(TestIF_File.b2DUseUndefinedCMD==true)
    {
        strTrigCMD=TestIF_File.str2DTriggerONCMD;
    }
    else
    {
        strTrigCMD="LON";
    }

    if(BAR_CODE_INSTALL==ebcUseOCR &&                                           //Ifor 20210407 add: 自製OCR
       TestIF_File.i2DReadMultiLine==1)
    {
        strTrigCMD=strTrigCMD+"2";
        bHasMultiLineBarcode[BarCodeIndex]=true;
    }
    else
    {
        bHasMultiLineBarcode[BarCodeIndex]=false;
    }

    strncpy(Onstr, (strTrigCMD+"\r").c_str(), sizeof(Onstr));                   //Ifor 20151226 :修改新增 2d 命令變數
    lenOnstr=strlen(Onstr);

    if(bBarcodeError[BarCodeIndex]==true)
    {
        bBarcodeDataSaveReady[BarCodeIndex]=false;
        bBarcodeError[BarCodeIndex]=false;
    }

    if(bBarcodeStartDelay[BarCodeIndex]==true)
    {
        if(BAR_CODE_INSTALL==ebctEtherNetCCD ||                                 //Ifor 20190129 : add Cognex EtherNet
           BAR_CODE_INSTALL==ebctUseCCDMode ||
           BAR_CODE_INSTALL==ebcUseOCR)                                         //Ifor 20210407 add: 自製OCR
        {
            switch(BarCodeIndex)
            {
                case 0:
                  if(ClientSocket_Shuttle1_A->Active)
                  {
                      ClientSocket_Shuttle1_A->Socket->SendText(strTrigCMD+"\r");
                      AddCCDCommunicationLog(BarCodeIndex, strTrigCMD);
                  }
                  break;
                case 1:
                  if(ClientSocket_Shuttle1_B->Active)
                  {
                      ClientSocket_Shuttle1_B->Socket->SendText(strTrigCMD+"\r");
                      AddCCDCommunicationLog(BarCodeIndex, strTrigCMD);
                  }
                  break;
                case 2:
                  if(ClientSocket_Shuttle2_A->Active)
                  {
                      ClientSocket_Shuttle2_A->Socket->SendText(strTrigCMD+"\r");
                      AddCCDCommunicationLog(BarCodeIndex, strTrigCMD);
                  }
                  break;
                case 3:
                  if(ClientSocket_Shuttle2_B->Active)
                  {
                      ClientSocket_Shuttle2_B->Socket->SendText(strTrigCMD+"\r");
                      AddCCDCommunicationLog(BarCodeIndex, strTrigCMD);
                  }
                  break;
            }
        }
        else
        {
            BarcodeCOM[BarCodeIndex]->WriteCommData(Onstr, lenOnstr);           //傳送開啟指令

            if(BarCodeIndex==0 || BarCodeIndex==1)
                Memo1->Lines->Add("Barcode_"+AnsiString(BarCodeIndex+1)+strTrigCMD);
            else
                Memo2->Lines->Add("Barcode_"+AnsiString(BarCodeIndex+1)+strTrigCMD);
        }
        bBarcodeStartDelay[BarCodeIndex]=false;
    }

    #ifdef SOFT_SIMULTE
    bool bSimulateBarCode=true;
    if(bBarcodeDataSaveReady[BarCodeIndex]==false)
    {
        bSimulateBarCode=true;
        if(BAR_CODE_INSTALL==ebctEtherNetCCD ||
           BAR_CODE_INSTALL==ebcUseOCR)                                         //Ifor 20210407 add: 自製OCR
        {
            switch(BarCodeIndex)
            {
                case 0:
                  if(ClientSocket_Shuttle1_A->Active)
                  {
                    bSimulateBarCode=false;
                  }
                  break;
                case 1:
                  if(ClientSocket_Shuttle1_B->Active)
                  {
                    bSimulateBarCode=false;
                  }
                  break;
                case 2:
                  if(ClientSocket_Shuttle2_A->Active)
                  {
                    bSimulateBarCode=false;
                  }
                  break;
                case 3:
                  if(ClientSocket_Shuttle2_B->Active)
                  {
                    bSimulateBarCode=false;
                  }
                  break;
            }
        }
        if(bSimulateBarCode)
        {
            Re.sprintf("Y%04dM%02dD%02dH%02dM%02dS%02dSiteNo%d%d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, BarCodeIndex, iXpos+1);
            mtBarcodeInSh->SetCellNumber(BarCodeIndex, iXpos+1, Re.c_str());
            if(TestIF_File.i2DIDFormat==eAMD)                                   //JerryYang 20200422 2DID format選項改用下拉選單
            {
                if(BarCodeIndex==0)
                {
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(0, iXpos+1, Re.c_str());
                }
                else if(BarCodeIndex==2)
                {
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iXpos+1, Re.c_str());
                }
            }
            bBarcodeDataSaveReady[BarCodeIndex]=true;
        }
    }
    #endif

    if(bBarcodeDataSaveReady[BarCodeIndex]==true || cbBarCodeSimulate->Checked)
    {
        if(cbBarCodeSimulate->Checked)
        {
            Re=GetSimuBarCodeName(BarCodeIndex, iXpos);
            cDeviceInf="";
//            if(Re.Length()<iSize)                                             //wei 20151120
            {
                cDeviceInf=Re;
                mtBarcodeInSh->SetCellNumber(BarCodeIndex, iXpos+1, Re.c_str());
                if(TestIF_File.i2DIDFormat==eAMD)                               //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    if(BarCodeIndex==2)
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iXpos+1, Re.c_str());
                    }
                    else
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(BarCodeIndex, iXpos+1, Re.c_str());
                    }
                }
            }
        }
        else
        {
            iBarcodePassCount[BarCodeIndex]++;                                  //wei 20160325 Barcode Rate計算
            Re=mtBarcodeInSh->GetCellText(BarCodeIndex, iXpos+1);               //wei 20160218 add inshuttle
            if(TestIF_File.i2DIDFormat==eAMD)                                   //JerryYang 20200422 2DID format選項改用下拉選單
            {
                if(BAR_CODE_INSTALL==ebctUseCCDMode)
                {
                    if(BarCodeIndex==3)
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iXpos+1, Re.c_str());
                    }
                    else if(BarCodeIndex==1)
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(BarCodeIndex, iXpos+1, Re.c_str());
                    }
                }
                else
                {
                    if(BarCodeIndex==2)
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iXpos+1, Re.c_str());
                    }
                    else
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(BarCodeIndex, iXpos+1, Re.c_str());
                    }
                }
            }

            if(Re=="" ||
               (Re==cLastDeviceInf &&
                Re!=asBarCodeErrorSend &&                                       //wei 20160318 Barcode Error依客戶設定
                Re!=asBarCodeErrorCheckSum))                                    //KaiHuang 20200826 : Add CheckSum
            {
                bBarcodeDataSaveReady[BarCodeIndex]=false;
                return false;
            }
            cDeviceInf=StringReplace(Re, "\r\n", "\0\0", TReplaceFlags()<<rfReplaceAll);     //Frank 20160829 add
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void TfBarCode::TurnOffInspection(int BarCodeIndex)
{
    int lenOffstr;
    char Offstr[128];
    AnsiString Re="";
    AnsiString strTrigOFFCMD="";

    if(TestIF_File.b2DUseUndefinedCMD==true)
    {
        strTrigOFFCMD=TestIF_File.str2DTriggerOFFCMD;
    }
    else
    {
        strTrigOFFCMD="LOFF";
    }

    strncpy(Offstr, strTrigOFFCMD.c_str(), sizeof(Offstr));                     //Ifor 20151226 :修改新增 2d 命令變數
    lenOffstr=strlen(Offstr);

    BarcodeCOM[BarCodeIndex]->WriteCommData(Offstr, lenOffstr);                 //傳送開啟指令
    bBarcodeStartDelay[BarCodeIndex]=false;

    if(BarCodeIndex==0 || BarCodeIndex==1)
        Memo1->Lines->Add("Barcode_"+AnsiString(BarCodeIndex+1)+strTrigOFFCMD); //Ifor 20151226 :修改新增 2d 命令變數
    else
        Memo2->Lines->Add("Barcode_"+AnsiString(BarCodeIndex+1)+strTrigOFFCMD); //Ifor 20151226 :修改新增 2d 命令變數
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
    bShow=false;
    i2DIDCheckSH1Task=1;
    i2DIDCheckSH2Task=1;
    btStart2DIDCheckSh1->Enabled=true;
    btStart2DIDCheckSh2->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::edBarcodeScanDelayTimeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 0, 100000);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btResetBarCodeCountClick(TObject *Sender)
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
            iBarCodeNo[i][j]=0;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::spbResetComClick(TObject *Sender)
{
    Barcode_1->StopComm();
    Barcode_3->StopComm();
    if(BAR_CODE_USECOUNT>=4)                                                    //Ifor 20190409 : add Shuttle 僅用兩顆CCD
    {
        Barcode_2->StopComm();
        Barcode_4->StopComm();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btReader1OnClick(TObject *Sender)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;

    if(BAR_CODE_USECOUNT<=2)                                                    //Ifor 20190409 : add Shuttle 僅用兩顆CCD
    {
        if(Ptr->Tag==1 || Ptr->Tag==3)
        {
           return;
        }
    }
    AnsiString CMD, Str;

    if(TestIF_File.b2DUseUndefinedCMD==true)                                    //Ifor 20151226 :自行定義 Trigger Command
        CMD=TestIF_File.str2DTriggerONCMD+"\r";
    else
        CMD="LON\r";
    Str.sprintf("Barcode_%d %s", Ptr->Tag+1, CMD);

    if(Ptr->Tag==0)
        Barcode_1->WriteCommData(CMD.c_str(), CMD.Length());
    else if(Ptr->Tag==1)
        Barcode_2->WriteCommData(CMD.c_str(), CMD.Length());
    else if(Ptr->Tag==2)
        Barcode_3->WriteCommData(CMD.c_str(), CMD.Length());
    else
        Barcode_4->WriteCommData(CMD.c_str(), CMD.Length());

    Memo1->Lines->Add(Str);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnGPIBReader4Click(TObject *Sender)
{
    int iNowCheckStep=0;
    TestSocket.Item[0][iNowCheckStep]=HAS_IC;
    bBarcodeStartDelay[iBarCode2_2]=true;
    bBarcodeDataSaveReady[iBarCode2_2]=false;
    InitTestTask();
    IsTest=true;
    bStartTest[3]=true;
    tmr1->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::tmr1Timer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    int iRet=0;
    int iNowCheckStep=0;

    if(bStartTest[3]==true)
    {
        if(Barcode_4StartScan_In(iNowCheckStep, sizeof (TestSocket.cDeviceInf[0][iNowCheckStep]) , TestSocket.cDeviceInf[0][iNowCheckStep], "QQ"))
        {
            iRet=ProcessTestResult(1);
            if(iRet==1)
            {
                SetNoiseDelay=false;
                TestISTimeOut=false;
                TestSocket.ClearAll();
                bStartTest[3]=false;
                tmr1->Enabled=false;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::edBarcodePosDelayTimeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 0, 5000);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::spbStartComClick(TObject *Sender)
{
    if(TestIF_File.bEnableBarCode)
    {
        InitSht2DCodeComPort(2);
    }
}
//---------------------------------------------------------------------------
void TfBarCode::InitSht2DCodeComPort(int iStep)                                 //Steven 20151221 : 重置Com Port, 避免異常
{
    if(BAR_CODE_INSTALL==ebctUninstall ||                                       //Steven 20160213 : For 2D code
       BAR_CODE_INSTALL==ebctUseCCDMode ||                                      //Ifor 20190129 : add Cognex EtherNet 通訊
       BAR_CODE_INSTALL==ebctEtherNetCCD ||
       BAR_CODE_INSTALL==ebcUseOCR)                                             //Ifor 20210407 add: 自製OCR
        return;

    #ifndef SOFT_SIMULTE
    if(iStep==0)
    {
        Barcode_1->StopComm();
        Barcode_1->StartComm();
        if(BAR_CODE_USECOUNT>=4)                                                //Ifor 20190409 : add Shuttle 僅用兩顆CCD
        {
            Barcode_2->StopComm();
            Barcode_2->StartComm();
        }
    }
    else if(iStep==1)
    {
        Barcode_3->StopComm();
        Barcode_3->StartComm();
        if(BAR_CODE_USECOUNT>=4)                                                //Ifor 20190409 : add Shuttle 僅用兩顆CCD
        {
            Barcode_4->StopComm();
            Barcode_4->StartComm();
        }
    }
    else
    {
        Barcode_1->StopComm();
        Barcode_3->StopComm();
        if(BAR_CODE_USECOUNT>=4)                                                //Ifor 20190409 : add Shuttle 僅用兩顆CCD
        {
            Barcode_2->StopComm();
            Barcode_4->StopComm();
        }
        InitBarCodeRS232(2);                                                    //wei 20150912 2D
    }
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::pgc2DIDChange(TObject *Sender)
{
    mtBarcodeSetDefaultView();

    if(CosFunction.bBarcodeTrayRecFile==true)                                   //jou 20190930 : Barcode Tray record file
    {
        for(int i=0; i<6; i++)
            DoShow_BinMapping_Info(i);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::edBarcodeRetryCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 100);
}
//---------------------------------------------------------------------------
bool TfBarCode::Barcode_1StartScan(int BarCodeIndex, int Sht)
{
    int lenOnstr, lenOffstr, ret;
    char Onstr[128], Offstr[128];
    AnsiString strTrigONCMD="";
    AnsiString strTrigOFFCMD="";

    if(TestIF_File.b2DUseUndefinedCMD==true)
    {
        strTrigONCMD=TestIF_File.str2DTriggerONCMD+"\r";
        strTrigOFFCMD=TestIF_File.str2DTriggerONCMD+"\r";
    }
    else
    {
        strTrigONCMD="LON\r";
        strTrigOFFCMD="LOFF\r";
    }

    strncpy(Onstr, strTrigONCMD.c_str(), sizeof(Offstr));                       //Ifor 20151226 :修改新增 2d 命令變數
    lenOnstr=strlen(Onstr);

    strncpy(Offstr, strTrigOFFCMD.c_str(), sizeof(Offstr));                     //Ifor 20151226 :修改新增 2d 命令變數
    lenOffstr=strlen(Offstr);

    if(bBarcodeError[BarCodeIndex]==true)
    {
        bBarcodeDataSaveReady[BarCodeIndex]=false;
        bBarcodeError[BarCodeIndex]=false;
    }

    if(bBarcodeStartDelay[BarCodeIndex]==true)
    {
        BarcodeCOM[BarCodeIndex]->WriteCommData(Onstr, lenOnstr);               //傳送開啟指令
        BarcodeDelay[BarCodeIndex].SetMSAndOn(TestIF_File.iBarCodeDelay);
        bBarcodeStartDelay[BarCodeIndex]=false;
    }

    if(BarcodeDelay[BarCodeIndex].Off())
    {
        if(bBarcodeDataSaveReady[BarCodeIndex]==true)
        {
            return true;
        }
        else
        {
            #ifdef SOFT_SIMULTE
            ret=K_SKIP;
            #else
            if(LastSet.iRealDummy==DUMMY)
                ret=K_SKIP;
            else
                ret=ShowErrorMessage("JAM0580", K_RETRY|K_SKIP, Sht);           //Out Shuttle1 Barcode Error
            #endif
            if(ret==K_RETRY)
            {
                bBarcodeStartDelay[BarCodeIndex]=true;
                return false;
            }
            else
            {
                BarcodeCOM[BarCodeIndex]->WriteCommData(Offstr, lenOffstr);     //傳送關閉指令
                bBarcodeError[BarCodeIndex]=true;
                return true;
            }
        }
    }
    else
    {
        if(bBarcodeDataSaveReady[BarCodeIndex]==true)
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfBarCode::Barcode_4StartScan_In(int iXpos, int iSize, AnsiString &cDeviceInf, AnsiString cLastDeviceInf)
{
    int BarCodeIndex=3;
    int lenOnstr, lenOffstr, ret;
    char Onstr[128], Offstr[128];
    AnsiString Re="";
    AnsiString strTrigONCMD="";
    AnsiString strTrigOFFCMD="";

    if(TestIF_File.b2DUseUndefinedCMD==true)
    {
        strTrigONCMD=TestIF_File.str2DTriggerONCMD+"\r";
        strTrigOFFCMD=TestIF_File.str2DTriggerONCMD+"\r";
    }
    else
    {
        strTrigONCMD="LON\r";
        strTrigOFFCMD="LOFF\r";
    }

    strncpy(Onstr, strTrigONCMD.c_str(), sizeof(Offstr));                       //Ifor 20151226 :修改新增 2d 命令變數
    lenOnstr=strlen(Onstr);

    strncpy(Offstr, strTrigOFFCMD.c_str(), sizeof(Offstr));                     //Ifor 20151226 :修改新增 2d 命令變數
    lenOffstr=strlen(Offstr);

    if(bBarcodeError[BarCodeIndex]==true)
    {
        bBarcodeDataSaveReady[BarCodeIndex]=false;
        bBarcodeError[BarCodeIndex]=false;
    }

    if(bBarcodeStartDelay[BarCodeIndex]==true)
    {
        BarcodeCOM[BarCodeIndex]->WriteCommData(Onstr, lenOnstr);               //傳送開啟指令
        BarcodeDelay[BarCodeIndex].SetMSAndOn(TestIF_File.iBarCodeDelay);
        bBarcodeStartDelay[BarCodeIndex]=false;
        Memo2->Lines->Add("Barcode_4 LON");
    }

    if(BarcodeDelay[BarCodeIndex].Off())
    {
        if(bBarcodeDataSaveReady[BarCodeIndex]==true)
        {
            Re=mtBarcodeInSh->GetCellText(3, iXpos+1);
            cDeviceInf="";
            cDeviceInf=Re;

            return true;
        }
        else
        {
            #ifdef SOFT_SIMULTE
            ret=K_SKIP;
            #else
            if(TestIF_File.bNoCodeDeviceAutoSkip==true &&
               fContact->IsRun2DCheck()==false)                                 //JerryYang 20250220 : 2DID硬體順序檢查功能
                ret=ShowErrorMessage("JAM0461", K_RETRY, MInShuttle2);
            else
                ret=ShowErrorMessage("JAM0461", K_RETRY|K_SKIP, MInShuttle2);   //In Shuttle2 Barcode Error
            #endif
            if(ret==K_RETRY)
            {
                bBarcodeStartDelay[BarCodeIndex]=true;
                return false;
            }
            else
            {
                BarcodeCOM[BarCodeIndex]->WriteCommData(Offstr, lenOffstr);     //傳送關閉指令
                bBarcodeError[BarCodeIndex]=true;
                return true;
            }
        }
    }
    else
    {
        if(bBarcodeDataSaveReady[BarCodeIndex]==true || cbBarCodeSimulate->Checked)
        {
            if(cbBarCodeSimulate->Checked)
            {
                Re=GetSimuBarCodeName(BarCodeIndex, iXpos);
                cDeviceInf="";
    //            if(Re.Length()<iSize)                                         //wei 20151120
                {
                    cDeviceInf=Re;
                    mtBarcodeInSh->SetCellNumber(3, iXpos+1, Re.c_str());
                }
            }
            else
            {
                Re=mtBarcodeInSh->GetCellText(3, iXpos+1);
                if(Re=="")
                {
                    bBarcodeDataSaveReady[BarCodeIndex]=false;
                    return false;
                }
                cDeviceInf="";
                cDeviceInf=Re;
            }

            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::BtShuttle_1A_ConnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    if(BOTTOM_2DID==1 && BOTTOM_2DID_CCD==1)                                    //Bottom 8CCD  KaiHuang 20200910 : Add
        TimerBotton8CCDConnect->Enabled=true;
    else
        TimerDownCCDConnect->Enabled=true;
    bEnableCCDChannelConnect[tempBtn->Tag]=true;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::BtShuttle_1A_DisconnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    bEnableCCDChannelConnect[tempBtn->Tag]=false;
    try
    {
        if(BOTTOM_2DID==1 && BOTTOM_2DID_CCD==1)                                //Bottom 8CCD  KaiHuang 20200910 : Add
        {
            switch(tempBtn->Tag)
            {
                case 0:
                    ClientSocket_Bottom_1->Close();
                    break;
                case 1:
                    ClientSocket_Bottom_2->Close();
                    break;
                case 2:
                    ClientSocket_Bottom_3->Close();
                    break;
                case 3:
                    ClientSocket_Bottom_4->Close();
                    break;
                case 4:
                    ClientSocket_Bottom_5->Close();
                    break;
                case 5:
                    ClientSocket_Bottom_6->Close();
                    break;
                case 6:
                    ClientSocket_Bottom_7->Close();
                    break;
                case 7:
                    ClientSocket_Bottom_8->Close();
                    break;
            }
        }
        else                                                                    //Top CCD + Bottom 4CCD
        {
            switch(tempBtn->Tag)
            {
                case 0:
                    ClientSocket_Shuttle1_A->Close();
                    break;
                case 1:
                    ClientSocket_Shuttle1_B->Close();
                    break;
                case 2:
                    ClientSocket_Shuttle2_A->Close();
                    break;
                case 3:
                    ClientSocket_Shuttle2_B->Close();
                    break;
            }
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfBarCode::BtShuttle_1A_DisconnectClick");
    }
}
//---------------------------------------------------------------------------
//Ifor 20151224 CCD 2D Code Send CMD
void __fastcall TfBarCode::btShuttle_1A_TriggerClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    AnsiString strSendCMD="";
    tempBtn = (TSpeedButton *)Sender;

    if(TestIF_File.i2DReadMultiLine==1)
    {
        bHasMultiLineBarcode[tempBtn->Tag]=true;
    }
    if(BOTTOM_2DID==1 && BOTTOM_2DID_CCD==1)                                    //Bottom 8CCD  KaiHuang 20200910 : Add
    {
        switch(tempBtn->Tag)
        {
            case 0:
                strSendCMD=edBottom_1_CMD->Text;
                SendCCDCommand(iBottom_1, " ", strSendCMD);
                break;
            case 1:
                strSendCMD=edBottom_2_CMD->Text;
                SendCCDCommand(iBottom_2, " ", strSendCMD);
                break;
            case 2:
                strSendCMD=edBottom_3_CMD->Text;
                SendCCDCommand(iBottom_3, " ", strSendCMD);
                break;
            case 3:
                strSendCMD=edBottom_4_CMD->Text;
                SendCCDCommand(iBottom_4, " ", strSendCMD);
                break;
            case 4:
                strSendCMD=edBottom_5_CMD->Text;
                SendCCDCommand(iBottom_5, " ", strSendCMD);
                break;
            case 5:
                strSendCMD=edBottom_6_CMD->Text;
                SendCCDCommand(iBottom_6, " ", strSendCMD);
                break;
            case 6:
                strSendCMD=edBottom_7_CMD->Text;
                SendCCDCommand(iBottom_7, " ", strSendCMD);
                break;
            case 7:
                strSendCMD=edBottom_8_CMD->Text;
                SendCCDCommand(iBottom_8, " ", strSendCMD);
                break;
        }
    }
    else
    {
        switch(tempBtn->Tag)
        {
            case 0:
                strSendCMD=edShuttle_1A_CMD->Text;
                SendCCDCommand(iBarCode1_1, " ", strSendCMD);
                break;
            case 1:
                strSendCMD=edShuttle_1B_CMD->Text;
                SendCCDCommand(iBarCode1_2, " ", strSendCMD);
                break;
            case 2:
                strSendCMD=edShuttle_2A_CMD->Text;
                SendCCDCommand(iBarCode2_1, " ", strSendCMD);
                break;
            case 3:
                strSendCMD=edShuttle_2B_CMD->Text;
                SendCCDCommand(iBarCode2_2, " ", strSendCMD);
                break;
            case 5:
                strSendCMD="LOFF";
                SendCCDCommand(iBarCode1_1, " ", strSendCMD);
                break;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::TimerDownCCDConnectTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;

    if(TestIF_File.bEnableBarCode==false && TestIF_File.bEnableShtFloatChk==false)
    {
        bTimerRun=false;
        return;
    }

    static int Count1=90;
    static int Count2=90;
    static int Count3=90;
    static int Count4=90;
    bool bNeedConnect=true;

    if(BAR_CODE_USECOUNT==2 &&                                                  //Steven 20190916 : 修正2DID通訊異常
       (BAR_CODE_INSTALL==ebctEtherNetCCD ||
        BAR_CODE_INSTALL==ebcUseOCR))                                           //Ifor 20210407 add: 自製OCR
        bNeedConnect=false;
    else
        bNeedConnect=true;

//    if(TestIF_File.iShuttleMode==0 || (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0))   //Ifor 20190222 : 2D Reader (Ethernet) 關ARM 時 關閉相對應的Reader
    {
        if(ClientSocket_Shuttle1_A->Active                              &&      //相同的IP跟Port就不要重新連線
           ClientSocket_Shuttle1_A->Address==edShuttle_1A_Address->Text &&
           ClientSocket_Shuttle1_A->Port==edShuttle_1A_Port->Text)
        {
        }
        else
        {
            if(ClientSocket_Shuttle1_A->Active)
            {
                ClientSocket_Shuttle1_A->Close();
                SendCCDCommand(iBarCode1_1, "ClientSocket Close", "");
                Count1=0;
            }
        }

        if(ClientSocket_Shuttle1_A->Active==false && bEnableCCDChannelConnect[iBarCode1_1]==true)
        {
            Count1++;
            if(Count1>20)                                                       //kevin 20191004 change
            {
                ClientSocket_Shuttle1_A->Address=edShuttle_1A_Address->Text;
                ClientSocket_Shuttle1_A->Port=atoi(edShuttle_1A_Port->Text.c_str());
                SendCCDCommand(iBarCode1_1, "ClientSocket connecting..", "");
                try
                {
                    ClientSocket_Shuttle1_A->Open();
                }
                catch(...)
                {
                    LogClientSocketExceptionError(ClientSocket_Shuttle1_A, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
                }
                SendCCDCommand(iBarCode1_1, "ClientSocket Open", "");
                Count1=0;
            }
        }
        if(bNeedConnect==true)
        {
            if(ClientSocket_Shuttle1_B->Active                              &&  //相同的IP跟Port就不要重新連線
               ClientSocket_Shuttle1_B->Address==edShuttle_1B_Address->Text &&
               ClientSocket_Shuttle1_B->Port==edShuttle_1B_Port->Text)
            {
            }
            else
            {
                if(ClientSocket_Shuttle1_B->Active)
                {
                    ClientSocket_Shuttle1_B->Close();
                    SendCCDCommand(iBarCode1_2, "ClientSocket Close", "");
                    Count2=0;
                }
            }

            if(ClientSocket_Shuttle1_B->Active==false && bEnableCCDChannelConnect[iBarCode1_2]==true)
            {
                Count2++;
                if(Count2>100)
                {
                    ClientSocket_Shuttle1_B->Address=edShuttle_1B_Address->Text;
                    ClientSocket_Shuttle1_B->Port=atoi(edShuttle_1B_Port->Text.c_str());
                    SendCCDCommand(iBarCode1_2, "ClientSocket connecting..", "");
                    try
                    {
                        ClientSocket_Shuttle1_B->Open();
                    }
                    catch(...)
                    {
                        LogClientSocketExceptionError(ClientSocket_Shuttle1_B, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
                    }
                    SendCCDCommand(iBarCode1_2, "ClientSocket Open", "");
                    Count2=0;
                }
            }
        }
    }
//    else
//    {
//        if(ClientSocket_Shuttle1_A->Active)
//        {
//            ClientSocket_Shuttle1_A->Close();
//            SendCCDCommand(iBarCode1_1, "ClientSocket Close", "");
//            Count1=0;
//        }
//
//        if(bNeedConnect==true)
//        {
//            if(ClientSocket_Shuttle1_B->Active)
//            {
//                ClientSocket_Shuttle1_B->Close();
//                SendCCDCommand(iBarCode1_2, "ClientSocket Close", "");
//                Count2=0;
//            }
//        }
//    }

//    if(TestIF_File.iShuttleMode==0 || (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1))   //Ifor 20190222 : 2D Reader (Ethernet) 關ARM 時 關閉相對應的Reader
    {
        if(ClientSocket_Shuttle2_A->Active                              &&      //相同的IP跟Port就不要重新連線
           ClientSocket_Shuttle2_A->Address==edShuttle_2A_Address->Text &&
           ClientSocket_Shuttle2_A->Port==edShuttle_2A_Port->Text)
        {
        }
        else
        {
            if(ClientSocket_Shuttle2_A->Active)
            {
                ClientSocket_Shuttle2_A->Close();
                SendCCDCommand(iBarCode2_1, "ClientSocket Close", "");
                Count3=0;
            }
        }

        if(ClientSocket_Shuttle2_A->Active==false && bEnableCCDChannelConnect[iBarCode2_1]==true)
        {
            Count3++;
            if(Count3>100)
            {
                ClientSocket_Shuttle2_A->Address=edShuttle_2A_Address->Text;
                ClientSocket_Shuttle2_A->Port=atoi(edShuttle_2A_Port->Text.c_str());
                SendCCDCommand(iBarCode2_1, "ClientSocket connecting..", "");
                try
                {
                    ClientSocket_Shuttle2_A->Open();
                }
                catch(...)
                {
                    LogClientSocketExceptionError(ClientSocket_Shuttle2_A, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
                }
                SendCCDCommand(iBarCode2_1, "ClientSocket Open", "");
                Count3=0;
            }
        }
        if(bNeedConnect==true)                                                  //Ifor 20180104 :add 使用4顆 CCD 才執行連線動作
        {
            if(ClientSocket_Shuttle2_B->Active                              &&  //相同的IP跟Port就不要重新連線
               ClientSocket_Shuttle2_B->Address==edShuttle_2B_Address->Text &&
               ClientSocket_Shuttle2_B->Port==edShuttle_2B_Port->Text)
            {
            }
            else
            {
                if(ClientSocket_Shuttle2_B->Active)
                {
                    ClientSocket_Shuttle2_B->Close();
                    SendCCDCommand(iBarCode2_2, "ClientSocket Close", "");
                    Count4=0;
                }
            }

            if(ClientSocket_Shuttle2_B->Active==false && bEnableCCDChannelConnect[iBarCode2_2]==true)
            {
                Count4++;
                if(Count4>100)
                {
                    ClientSocket_Shuttle2_B->Address=edShuttle_2B_Address->Text;
                    ClientSocket_Shuttle2_B->Port=atoi(edShuttle_2B_Port->Text.c_str());
                    SendCCDCommand(iBarCode2_2, "ClientSocket connecting..", "");
                    try
                    {
                        ClientSocket_Shuttle2_B->Open();
                    }
                    catch(...)
                    {
                        LogClientSocketExceptionError(ClientSocket_Shuttle2_B, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
                    }
                    SendCCDCommand(iBarCode2_2, "ClientSocket Open", "");
                    Count4=0;
                }
            }
        }
    }
//    else
//    {
//        if(ClientSocket_Shuttle2_A->Active)
//        {
//            ClientSocket_Shuttle2_A->Close();
//            SendCCDCommand(iBarCode2_1, "ClientSocket Close", "");
//            Count3=0;
//        }
//
//        if(bNeedConnect==true)
//        {
//            if(ClientSocket_Shuttle2_B->Active)
//            {
//                ClientSocket_Shuttle2_B->Close();
//                SendCCDCommand(iBarCode2_2, "ClientSocket Close", "");
//                Count4=0;
//            }
//        }
//    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
void TfBarCode::SetSFCCheckStepCount()
{
    if(TestIF_File.bSFCUse2Photo)                                               //遞減方式檢查
        iSFCTotalMoveStep=InArmSuck.iShtCol*2;
    else
        iSFCTotalMoveStep=InArmSuck.iShtCol;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::ClientSocket_Shuttle1_AConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;

    if(InitialOK==false)
    {
        Ptr->Active=false;
        return;
    }

    if(BAR_CODE_INSTALL==ebctEtherNetCCD ||                                     //Ifor 20190129 : add COGNEX
       BAR_CODE_INSTALL==ebcUseOCR)                                             //Ifor 20210407 add: 自製OCR
        return;

    AnsiString Str;
    Str.sprintf("IP: %s, Port:%d [Connect OK]", Ptr->Address, Ptr->Port);
    SendCCDCommand(Ptr->Tag, Str, "");

    SetSFCCheckStepCount();
    if(Ptr->Tag==iBarCode1_1)
    {
        Change2DSetupFile();                                                    //Steven 20160425 : 重置2DSys時間
    }
    else if(dVisionVer>=0)
    {
        if(TestIF_File.bEnableShtFloatChk)                                      //Steven 20160920 : IC置偏檢查
        {
            Str.sprintf("E9,1,%d", iSFCTotalMoveStep);
            SendCCDCommand(Ptr->Tag, "Clear buffer", Str);                      //Reset CCD Shuttle暫存器
        }
        if(TestIF_File.bEnableBarCode)
        {
            ClearBuffer(Ptr->Tag, "Clear Buffer when socket connect");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::ClientSocket_Shuttle1_ADisconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    SendCCDCommand(Ptr->Tag, "DisConnect", "");
    Ptr->Active=false;                                                          //KaiHuang 20200910 : Add
    try                                                                         //KaiHuang 20200910 : Add
    {
        Ptr->Close();
        SendCCDCommand(Ptr->Tag, "ClientSocket Close Success", "");
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "TfBarCode::Disconnect");         //Steven 20231113 : 紀錄網路連線例外
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::ClientSocket_Shuttle1_AError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    if(InitialOK==false)
    {
        return;
    }

    try
    {
        TClientSocket *Ptr;
        Ptr=(TClientSocket *)Sender;
        int Tag=Ptr->Tag;
        SendCCDCommand(Tag, GetErrorMsg(Sender, ErrorCode), "");
        ErrorEvent=TErrorEvent(NULL);
        ErrorCode =0;

        try
        {
            Ptr->Close();
        }
        catch(...)
        {
            LogClientSocketExceptionError(Sender, "Barcode ClientSocket Error");
        }

        Abort();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "TfBarCode::Error");              //Steven 20231113 : 紀錄網路連線例外
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::ClientSocket_Shuttle1_ARead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    AnsiString Log;
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    int Tag=Ptr->Tag;
    int iSH_Position=0;                                                         //Ifor 20190129 : add Cognex EtherNet
    int iSH_BarCode=0;                                                          //Ifor 20190129 : add Cognex EtherNet
    AnsiString strCodePosition;                                                 //Ifor 20190129 : add Cognex EtherNet

    AnsiString EthernetBuffer;
    int iBufferLenght=0;
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0)                                                         //13.09.30.01   klutter
    {
        if(BAR_CODE_INSTALL==ebctEtherNetCCD ||                                 //Ifor 20190129 : add Cognex EtherNet
           BAR_CODE_INSTALL==ebcUseOCR ||                                       //Ifor 20210407 add: 自製OCR
           (BAR_CODE_INSTALL==ebctUseCCDMode &&
            TestIF_File.b2DUseSubJob==true))
        {
            bCCDBarcodeWaitReply[Tag]=false;                                    //Ifor 20190225 :add Bar Code Use HandShake Communication
            switch(Tag)
            {
                case 0:
                    iSH_Position    =iSH1_1BarcodePosition;
                    iSH_BarCode     =iBarCode1_1;
                    strCodePosition ="Shuttle1-A";
                    break;
                case 1:
                    iSH_Position    =iSH1_2BarcodePosition;
                    iSH_BarCode     =iBarCode1_2;
                    strCodePosition ="Shuttle1-B";
                    break;
                case 2:
                    iSH_Position    =iSH2_1BarcodePosition;
                    iSH_BarCode     =iBarCode2_1;
                    strCodePosition ="Shuttle2-A";
                    break;
                case 3:
                    iSH_Position    =iSH2_2BarcodePosition;
                    iSH_BarCode     =iBarCode2_2;
                    strCodePosition ="Shuttle2-B";
                    break;
            }

            AnsiString DataStr, Path, Data;
            int len=0;
            AnsiString cStr;
            AnsiString cStrMultiLine;

            cStr=Socket->ReceiveText();
            Log.sprintf("Recv, Row Data, %s", cStr);                             //Steven 20250808 : for OCR
            AddCCDCommunicationLog(Tag, Log);

            if(BAR_CODE_INSTALL==ebcUseOCR)
            {
                if(bChangeSetupFile==true)
                {
                    if(cStr.Pos("LF")!=0 && (cStr.Pos("OK")!=0 || cStr.Pos("NG")!=0))
                        bChangeSetupFile=false;
                }
                else
                {
                    cStrMultiLine=cStr;
                    cStrMultiLine=cStrMultiLine.Trim();
                    if(TestIF_File.i2DReadMultiLine==1 && bHasMultiLineBarcode[Tag]==true)
                    {
                        bHasMultiLineBarcode[Tag] = false;
                        asMultiLine_Ocr[1] = cStrMultiLine.SubString(cStrMultiLine.Pos("_")+1,cStrMultiLine.Length());  //2019WW03
                        asMultiLine_Ocr[0] = cStrMultiLine.SubString(1,cStrMultiLine.Pos("_")-1);                       //0001
                        if(asMultiLine_Ocr[1].Length() != TestIF_File.i2D1stLineLength)
                        {
                            asMultiLine_Ocr[1] ="";
                            asMultiLine_Ocr[0] ="";
                            cStr = "ERROR_Read";
                        }
                        else if(asMultiLine_Ocr[0].Length() != TestIF_File.i2D2ndLineLength)
                        {
                            asMultiLine_Ocr[1] ="";
                            asMultiLine_Ocr[0] ="";
                            cStr = "ERROR_Read";
                        }
                        else
                        {
                            cStr = asMultiLine_Ocr[1]+ TestIF_File.as2DInsertString + asMultiLine_Ocr[0]; //2019ww03_0001
                        }
                    }
                    else if(TestIF_File.i2DReadMultiLine==0)
                    {
                        if(cStrMultiLine.Length() != TestIF_File.i2D1stLineLength)
                        {
                            asMultiLine_Ocr[1] ="";
                            asMultiLine_Ocr[0] ="";
                            cStr = "ERROR_Read";
                        }
                        else
                        {
                            cStr = cStrMultiLine;
                        }
                    }
                }
            }

            Data.sprintf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
            Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate);

            MyForceDirectories(Path, "ClientSocket_Shuttle1_ARead");

            cStr=DoBarcodeAnalysis(cStr);                                       //wei 20161223 Barcode (Steven) 品質顯示

            len=cStr.Length();
            if(cStr.Pos("VR,")!=0 || cStr.Pos("LF,")!=0 || cStr.Pos("GF,")!=0 ||cStr.Pos("SE9,")!=0)
            {

            }
            else
            {
                if(len>TestIF_File.iBarCodeMaxLength)
                {
                    DataStr=asBarCodeErrorSend;                                 //wei 20160318 Barcode Error依客戶設定
                }
                else if(len>=TestIF_File.iBarCodeMinLength &&
                        len<=TestIF_File.iBarCodeMaxLength)                     //wei 20151127 字元數比對
                {
                    fNote->t2DCode->SetCellNumber(Tag, iSH_Position, cStr);
                    fNote->t2DCode->SetCellColorIndex(Tag, iSH_Position, 0);

                    if(bBarcodeAutoSkip[iSH_BarCode]==true)                     //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
                    {
                        bBarcodeAutoSkipError[iSH_BarCode]=true;
                        bBarcodeAutoSkip[iSH_BarCode]=false;
                    }
                    else if(DoBarcodeCheck(iSH_BarCode, cStr, iSH_Position)==false)    //wei 20160505 Barcode 比對Lot
                    {
                        bBarcodeDataSaveReady[iSH_BarCode]=false;
                        fNote->t2DCode->SetCellColorIndex(Tag, iSH_Position, 3);
                    }
                    else
                    {
                        DataStr.sprintf("%s %s %d Site%d : %s", Data.c_str(), strCodePosition, iSH_Position, TestIF_File.iSiteMap[Tag][iSH_Position], cStr);  //RogerYang 20190211 修正字串給錯 //wei 20160325 Barcode 紀錄位置
                        lb_2DBarcodeData->Items->Add(Data);
                        lb_2DBarcodeData->Items->Add(strCodePosition +" : " + AnsiString(cStr));      //RogerYang 20190211 修正字串給錯
                        Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
                        WriteDataToFile(Path.c_str() , DataStr.c_str());
                        if(lb_2DBarcodeData->Items->Count>100)
                            lb_2DBarcodeData->Items->Clear();
                        if(cStr=="NG")
                        {
                            bBarcodeDataSaveReady[iSH_BarCode]=false;
                        }
                        else if(cStr=="ERROR_Read")
                        {
                            bBarcodeDataSaveReady[iSH_BarCode]=false;
                            cStr="ERROR";
                        }
                        else
                        {
                            mtBarcodeInSh->SetCellNumber(Tag, iSH_Position+1, cStr);                 //wei 20150912 2D
                            if(TestIF_File.i2DIDFormat==eAMD)                   //JerryYang 20200422 2DID format選項改用下拉選單
                            {
                                if(cStr.Pos("VR,")!=0 || cStr.Pos("LF,")!=0 || cStr.Pos("GF,")!=0 ||cStr.Pos("SE9,")!=0)
                                {

                                }
                                else
                                {
                                    if(Tag==2)
                                    {
                                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, iSH_Position+1, cStr);
                                    }
                                    else
                                    {
                                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(Tag, iSH_Position+1, cStr);
                                    }
                                }
                            }
                            bBarcodeDataSaveReady[iSH_BarCode]=true;
                        }
                    }
                }
                else
                {
                    DataStr.sprintf("%s %s %d Site%d : %s", Data.c_str(), strCodePosition, iSH_Position, TestIF_File.iSiteMap[Tag][iSH_Position], cStr);  //RogerYang 20190211 修正字串給錯 //wei 20160325 Barcode 紀錄位置
                    lb_2DBarcodeData->Items->Add(Data);
                    lb_2DBarcodeData->Items->Add(strCodePosition +" : " + AnsiString(cStr));      //RogerYang 20190211 修正字串給錯
                    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
                    WriteDataToFile(Path.c_str() , DataStr.c_str());
                    if(lb_2DBarcodeData->Items->Count>100)
                        lb_2DBarcodeData->Items->Clear();
                    bBarcodeDataSaveReady[iSH_BarCode]=false;
                }
            }
            Log.sprintf("Recv, After Check, %s", cStr);
            AddCCDCommunicationLog(Tag, Log);
        }
        else
        {
            EthernetBuffer=Socket->ReceiveText();
            EthernetBuffer=StringReplace(EthernetBuffer, "\r", ",", TReplaceFlags()<<rfReplaceAll); //Steven 20160512 : 2D log排版
            EthernetBuffer=StringReplace(EthernetBuffer, "\n", "", TReplaceFlags()<<rfReplaceAll);
            Socket2DReceiveList[Tag]->Add(EthernetBuffer);

            Log.sprintf("Recv, , %s", EthernetBuffer);
            AddCCDCommunicationLog(Tag, Log);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::TimerProcess2DDataTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;

    AnsiString sPrefix, sReply="";
    AnsiString Str2, Str;
    int iPosition;
    int iFunction=0;
    int iResult=-1;
    bool bResult=false;

    for(int Tag=0; Tag<BAR_CODE_COUNT; Tag++)
    {
        if(Socket2DReceiveList[Tag]->Count>0)
        {
            sDownCCDRecevieData[Tag]=Socket2DReceiveList[Tag]->Strings[0];
            Str2=StringReplace(sDownCCDRecevieData[Tag], ",", "\n", TReplaceFlags()<<rfReplaceAll);     //Steven 20160401 : 解決Code有空格的問題
            Socket2DReceiveList[Tag]->Delete(0);
            if(Socket2DReceiveList[Tag]->Count==0)
                Socket2DReceiveList[Tag]->Clear();

            TStringList *sList = new TStringList();
            sList->SetText(Str2.c_str());                                       //Steven 20160401 : 解決Code有空格的問題
            sPrefix=sList->Strings[0];

            if(sPrefix=="VR")                                                   //軟體版本
            {
                bSFCGetVR=true;
                if(sList->Count>=2)
                    dVisionVer=atof(sList->Strings[1].c_str());
                else
                    dVisionVer=1;

                sReply.sprintf("VROK : %s", sDownCCDRecevieData[Tag]);
            }
            else if(sPrefix=="CA")                                              //關閉Vision軟體
            {

            }
            else if(sPrefix=="AP")                                              //取得路徑
            {

            }
            else if(sPrefix=="ST")                                              //設定時間
            {
                if(sList->Count>=2)
                    iResult=atoi(sList->Strings[1].c_str());
                bGetST=true;
                sReply.sprintf("%s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
            }
            else if(dVisionVer>=2)                                              //新版Command     //wei 20170119 (Steven) 版本號大於2
            {
                if(sList->Count>=2)
                    iFunction=atoi(sList->Strings[1].c_str());

                if(sList->Count>=3)
                    iResult=atoi(sList->Strings[2].c_str());

                //----------------------------
                // Shuttle Floating Check
                //----------------------------
                if(iFunction==1)
                {
                    if(sPrefix=="GV")
                    {
                        if(sList->Count>=4)
                        {
                            if(bSFCGetResultOK[Tag]==false)
                            {
                                bResult=CCDSFC_Decoding(Tag);
                                if(bResult)
                                {
                                    bSFCGetResultOK[Tag]=true;
                                    sReply.sprintf("SFC GVOK : %s", sDownCCDRecevieData[Tag]);
                                }
                                else
                                {
                                    sReply.sprintf("SFC GVNG : %s", sDownCCDRecevieData[Tag]);
                                }
                            }
                            else
                            {
                                sReply.sprintf("SFC GVTO : %s", sDownCCDRecevieData[Tag]);
                            }
                        }
                        else
                        {
                            sReply.sprintf("SFC GVNG : %s", sDownCCDRecevieData[Tag]);
                        }
                    }
                    else
                    {
                        sReply.sprintf("SFC %s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
                        /*--------------------------------------------------------------
                        //  回傳格式: E1,1,1,5,0
                        //  內文說明: E1,[功能],[成功與否],[位置],[有無料]
                        //------------------------------------------------------------*/
                        if(sPrefix=="E0")
                        {

                        }
                        else if(sPrefix=="E1" ||
                                sPrefix=="E2" ||                                //Auto Tune no ic
                                sPrefix=="E3" ||                                //Auto Tune has ic
                                sPrefix=="E4" ||                                //只拍一張, 只確認有無讀取
                                sPrefix=="E8")                                  //Reset單筆資料
                        {
                            if(sList->Count>=4)                                 //Steven 20160503 : 確認移動位置與拍照位置相同
                            {
                                iPosition=atoi(sList->Strings[3].c_str());
                                if(iPosition!=iSFCCurrentStep[Tag])
                                {
                                    Str.sprintf("SFC Alarm, %s Current step %d compare with exposure position %d is different!", sPrefix, iSFCCurrentStep[Tag], iPosition);    //Steven 20160512 : 2D log排版
                                    AddCCDCommunicationLog(Tag, Str);
                                    bSFCStepError[Tag]=true;
                                }
                            }

                            if(bSFCStepError[Tag]==false)
                            {
                                if(iResult!=1)
                                {
                                    Str.sprintf("SFC Alarm, %s result error!", sPrefix);
                                    AddCCDCommunicationLog(Tag, Str);
                                    bSFCError[Tag]=true;
                                }
                                else
                                {
                                    if(bSFCExposureOK[Tag]==false)
                                    {
                                        bSFCExposureOK[Tag]=true;
                                    }
                                }
                            }
                        }
                        else if(sPrefix=="E5")                                  //調整光源
                            ;
                        else if(sPrefix=="E9")                                  //Reset全部資料
                            bSFCGetSE9[Tag]=true;
                        else if(sPrefix=="GS")                                  //取得狀態
                            bGetGS[Tag]=true;
                        else if(sPrefix=="LF")                                  //設定工作檔
                            bSFCGetLF=true;
                        else if(sPrefix=="FL")                                  //取得檔案列表
                            ;
                        else if(sPrefix=="GF")                                  //取得工作檔名
                        {
                            if(sList->Count>=4)
                                sSFCCurrentGF=sList->Strings[3];
                            else
                                sSFCCurrentGF="";
                            bSFCGetGF=true;
                        }
                        else if(sPrefix=="SF")                                  //存檔
                            ;
                    }
                }
                //----------------------------
                // 2DID
                //----------------------------
                else //if(iFunction==0)
                {
                    if(sPrefix=="GV")
                    {
                        if(sList->Count>=4)
                        {
                            if(bCCDBarcodeGetResultOK[Tag]==false)
                            {
                                if(TestIF_File.bEnableMulti2D)                  //Steven 20200810 : 一個IC使用多個2DID
                                    CCDBarcode_DecodingForMulti2D(Tag);
                                else
                                    CCDBarcode_Decoding(Tag);
                                bCCDBarcodeGetResultOK[Tag]=true;
                                sReply.sprintf("2DID GVOK : %s", sDownCCDRecevieData[Tag]);
                            }
                            else
                            {
                                sReply.sprintf("2DID GVTO : %s", sDownCCDRecevieData[Tag]);
                            }
                        }
                        else
                        {
                            sReply.sprintf("2DID GVNG   : %s", sDownCCDRecevieData[Tag]);
                        }
                    }
                    else
                    {
                        sReply.sprintf("2DID %s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
                        /*--------------------------------------------------------------
                        //  回傳格式: E0,1,1
                        //  內文說明: E0,[功能],[成功與否],[位置]
                        //------------------------------------------------------------*/
                        if(sPrefix=="E0" || sPrefix=="E1")
                        {
                            if(sList->Count>=3)                                 //Steven 20160503 : 確認移動位置與拍照位置相同
                            {
                                iPosition=atoi(sList->Strings[3].c_str());      //Sam 20220209 : 修正 2D Position Error
                                if(iPosition!=iCurrentStep[Tag])
                                {
                                    Str.sprintf("Alarm, Current step %d compare with exposure position %d is different!", iCurrentStep[Tag], iPosition);    //Steven 20160512 : 2D log排版
                                    AddCCDCommunicationLog(Tag, Str);
                                    bStepError[Tag]=true;
                                }
                            }

                            if(bCCDBarcodeExposureOK[Tag]==false)
                            {
                                bCCDBarcodeExposureOK[Tag]=true;
                            }
                        }
                        else if(sPrefix=="E2")                                  //Auto Teach 2D
                        {
                            iGetSE2[Tag]=iResult;
                        }
                        else if(sPrefix=="E3")                                  //Auto Learn ROI
                        {
                            iGetSE3[Tag]=iResult;
                        }
                        else if(sPrefix=="E4")                                  //只拍一張, 只確認有無讀取
                        {
                            iGetSE4[Tag]=iResult;
                        }
                        else if(sPrefix=="E5")                                  //調整光源
                        {
                            ;
                        }
                        else if(sPrefix=="E8")                                  //Reset單筆資料
                        {
                            ;
                        }
                        else if(sPrefix=="E9")                                  //Reset全部資料
                        {
                            bGetSE9[Tag]=true;
                        }
                        else if(sPrefix=="GS")                                  //取得狀態
                        {
                            bGetGS[Tag]=true;
                        }
                        else if(sPrefix=="LF")                                  //設定工作檔
                        {
                            bGetLF=true;
                            bChangeBarcodeFileOK=true;
                        }
                        else if(sPrefix=="FL")                                  //取得檔案列表
                        {
                            ;
                        }
                        else if(sPrefix=="GF")                                  //取得工作檔名
                        {
                            if(sList->Count>=4)
                                sCurrentGF=sList->Strings[3];
                            else
                                sCurrentGF="";
                            bGetGF=true;
                        }
                        else if(sPrefix=="SF")                                  //存檔
                        {
                            ;
                        }
                    }
                }
            }
            else //if(dVisionVer==1)
            {
                if(sList->Count>1)
                    iResult=atoi(sList->Strings[1].c_str());
                else
                    iResult=-1;

                if(sPrefix=="GV")
                {
                    if(sList->Count>1)
                    {
                        if(bCCDBarcodeGetResultOK[Tag]==false)
                        {
                            if(TestIF_File.bEnableMulti2D)                      //Steven 20200810 : 一個IC使用多個2DID
                                CCDBarcode_DecodingForMulti2D(Tag);
                            else
                                CCDBarcode_Decoding(Tag);
                            bCCDBarcodeGetResultOK[Tag]=true;
                            sReply.sprintf("2DID GVOK : %s", sDownCCDRecevieData[Tag]);
                        }
                        else
                        {
                            sReply.sprintf("2DID GVTO : %s", sDownCCDRecevieData[Tag]);
                        }
                    }
                    else
                    {
                        sReply.sprintf("2DID GVNG   : %s", sDownCCDRecevieData[Tag]);
                    }
                }
                else
                {
                    sReply.sprintf("2DID %s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
                    /*--------------------------------------------------------------
                    //  回傳格式: SE0,1,1
                    //  內文說明: SE[事件],[成功與否],[位置]
                    //------------------------------------------------------------*/
                    if(sPrefix=="SE0" || sPrefix=="SE1")
                    {
                        if(BOTTOM_2DID && TestIF_File.bEnableBottom2D) //Steven 20190308 : Bottom 2D
                        {

                        }
                        else
                        {
                            if(sList->Count>=3)                                         //Steven 20160503 : 確認移動位置與拍照位置相同
                            {
                                iPosition=atoi(sList->Strings[2].c_str());
                                if(iPosition!=iCurrentStep[Tag])
                                {
                                    Str.sprintf("Alarm, Current step %d compare with exposure position %d is different!", iCurrentStep[Tag], iPosition);    //Steven 20160512 : 2D log排版
                                    AddCCDCommunicationLog(Tag, Str);
                                    bStepError[Tag]=true;
                                }
                            }
                        }

                        if(bStepError[Tag]==false && bCCDBarcodeExposureOK[Tag]==false)
                        {
                            bCCDBarcodeExposureOK[Tag]=true;
                        }
                    }
                    else if(sPrefix=="SE2")                                     //Auto Teach 2D
                    {
                        iGetSE2[Tag]=iResult;
                    }
                    else if(sPrefix=="SE3")                                     //Auto Learn ROI
                    {
                        iGetSE3[Tag]=iResult;
                    }
                    else if(sPrefix=="SE4")                                     //Steven 20120126 : 只拍一張, 只確認有無讀取
                    {
                        iGetSE4[Tag]=iResult;
                    }
                    else if(sPrefix=="GF")                                      //取得工作檔名
                    {
                        if(sList->Count>=3)
                            sCurrentGF=sList->Strings[2];
                        else
                            sCurrentGF="";
                        bGetGF=true;
                    }
                    else if(sPrefix=="LF")                                      //設定工作檔
                    {
                        bGetLF=true;
                        bChangeBarcodeFileOK=true;
                    }
                    else if(sPrefix=="FL")                                      //取得檔案列表
                    {
                        ;
                    }
                    else if(sPrefix=="SE9")                                     //Reset資料
                    {
                        bGetSE9[Tag]=true;
                    }
                    else if(sPrefix=="ST")                                      //設定時間
                    {
                        bGetST=true;
                    }
                    else if(sPrefix=="GS")                                      //取得狀態
                    {
                        bGetGS[Tag]=true;
                    }
                }
            }
            SendCCDCommand(Tag, sReply);
            sList->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete sList;
        }
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
void TfBarCode::CCDBarcode_Decoding(int Channel)
{
    int iResult=-1;
    int iCol, len, iListCount, iPos, iTotalCount;
    AnsiString sPrefix;
    AnsiString dataSplit, Path;

    if(BOTTOM_2DID_CCD==1 &&                                                    //RogerYang 20190319 add bottom 2did
       BOTTOM_2DID==1 &&
       TestIF_File.bEnableBottom2D)                                             //KaiChen 20200513 : Bottom 2D 8CCD
    {
        iCol=1;
    }
    else if(BOTTOM_2DID &&
            TestIF_File.bEnableBottom2D)                                        //Steven 20190308 : Bottom 2D
    {
        iCol=2;                                                                 //RogerYang 20190319 四吸嘴 1支ccd會有兩筆資料
    }
    else
    {
        iCol=TestSocket.iShtCol;
    }

    TStringList *sList  =new TStringList();
    TRegExpr * regex    =new TRegExpr;                                          //Steven 20160602 : Add regular expression to check alphanumeric character

    if(TestIF_File.b2DIDStringFormat==0)                                        //RogerYang 20181222 新增String format選項 /*--- https://poychang.github.io/note-regular-expression/ ---*/
    {
        regex->Expression   ="(\\w+)";                                          // w = an alphanumeric character (including "_")
    }
    else if(TestIF_File.b2DIDStringFormat==1)
    {
        regex->Expression   ="(\\w+-\\w+)";                                     // w = an alphanumeric character (including "_") //Steven 20181001 : Add "-"
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==2)
    {
        regex->Expression   ="(\\w+[.]+\\w+)";                                  // w = an alphanumeric character (including "_") //Steven 20210303 : Add "."
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==3)
    {
        regex->Expression   ="(\\w+[.]+\\w+-\\w+)";                             // w = an alphanumeric character (including "_") //Steven 20210303 : Add "."
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==4)
    {
        regex->Expression   ="(\\w+-\\w+[.]+\\w+)";                             // w = an alphanumeric character (including "_") //Steven 20210303 : Add "."
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==5)                                   //Steven 20210518 : 2DID改為找非法字元方式
    {
        regex->Expression   ="[^a-zA-Z0-9-: _.]";                               //不等於裡面指定的文字
    }

    sList->CommaText=sDownCCDRecevieData[Channel];
    WriteBarCodeLog(Channel, sDownCCDRecevieData[Channel]);
    iListCount=sList->Count;
    sPrefix=sList->Strings[0];

    if(dVisionVer>=2)                                                           //新版Command  //wei 20170119 (Steven) 版本號大於2
    {
        //--------------------
        //回傳格式
        //GV,0,1,11111111,Code1,Code2,Code3,Code4,Code5,Code6,Code7,Code8
        //--------------------
        if(iListCount>1)
            iResult=atoi(sList->Strings[2].c_str());

        iTotalCount=4+iCol;
    }
    else
    {
        //--------------------
        //回傳格式
        //GV,1,11111111,Code1,Code2,Code3,Code4,Code5,Code6,Code7,Code8
        //--------------------
        if(iListCount>1)
            iResult=atoi(sList->Strings[1].c_str());

        iTotalCount=3+iCol;
    }

    if(iListCount<iTotalCount  ||
       iResult==999)
    {
        for(int i=0; i<iCol; i++)
        {
            dataSplit="Decode_Error";
            mtBarcodeInSh->SetCellNumber(Channel, i+1,  dataSplit.c_str());     //Ifor 20151225 Add CCD BarCode 資料顯示
            dataSplit=asBarCodeErrorSend;                                       //wei 20160318 Barcode Error依客戶設定

            if(BOTTOM_2DID_CCD==1 && BOTTOM_2DID==1 && TestIF_File.bEnableBottom2D)//KaiChen 20200513 : Bottom 2D 8CCD
            {
                if(Channel==iBottom_1)
                {
                    InArmSuck.cDeviceInf[0][0]=dataSplit;
                }
                else if(Channel==iBottom_2)
                {
                    InArmSuck.cDeviceInf[1][0]=dataSplit;
                }
                else if(Channel==iBottom_3)
                {
                    InArmSuck.cDeviceInf[0][1]=dataSplit;
                }
                else if(Channel==iBottom_4)
                {
                    InArmSuck.cDeviceInf[1][1]=dataSplit;
                }
                else if(Channel==iBottom_5)
                {
                    InArmSuck.cDeviceInf[0][2]=dataSplit;
                }
                else if(Channel==iBottom_6)
                {
                    InArmSuck.cDeviceInf[1][2]=dataSplit;
                }
                else if(Channel==iBottom_7)
                {
                    InArmSuck.cDeviceInf[0][3]=dataSplit;
                }
                else if(Channel==iBottom_8)
                {
                    InArmSuck.cDeviceInf[1][3]=dataSplit;
                }
            }
            else if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)                 //Steven 20190308 : Bottom 2D
            {
                if(Channel==iBarCode1_1)
                {
                    InArmSuck.cDeviceInf[0][i]=dataSplit;
                }
                else if(Channel==iBarCode1_2)
                {
                    InArmSuck.cDeviceInf[0][i+2]=dataSplit;
                }
                else if(Channel==iBarCode2_1)
                {
                    InArmSuck.cDeviceInf[1][i]=dataSplit;
                }
                else
                {
                    InArmSuck.cDeviceInf[1][i+2]=dataSplit;
                }
            }
            else
            {
                if(Channel==iBarCode1_1)
                {
                    FLCarryKit.cDeviceInf[0][i]=dataSplit;
                }
                else if(Channel==iBarCode1_2)
                {
                    if(InArmSuck.iShtRow==2)                                    //Ifor 20220411 : Fixed for 單排使用CCD2
                        FLCarryKit.cDeviceInf[1][i]=dataSplit;
                    else
                        FLCarryKit.cDeviceInf[0][i]=dataSplit;
                }
                else if(Channel==iBarCode2_1)
                {
                    BLCarryKit.cDeviceInf[0][i]=dataSplit;
                }
                else
                {
                    if(InArmSuck.iShtRow==2)
                        BLCarryKit.cDeviceInf[1][i]=dataSplit;
                    else                                                        //IC在0的位置, 使用CCD4
                        BLCarryKit.cDeviceInf[0][i]=dataSplit;
                }
            }
        }
    }
    else
    {
        for(int i=0; i<iCol; i++)
        {
            if(sList->Count-1>=i)
            {
                if(dVisionVer>=2)                                               //新版Command      //wei 20170119 (Steven) 版本號大於2
                    iPos=i+4;
                else
                    iPos=i+3;
                dataSplit=sList->Strings[iPos];
                if(CosFunction.b2DUseAnyCharFunction==true &&
                   TestIF_File.b2DUseAnyChar==true)                             //Ifor 20210724 add: AMD 要求Barcode不卡控字元
                {
                    len=dataSplit.Length();
                }
                else
                {
                    regex->InputString=dataSplit;                               //Steven 20160602 : Add regular expression to check alphanumeric character
                    if(TestIF_File.b2DIDStringFormat==5)                        //Steven 20210518 : 2DID改為找非法字元方式
                    {
                        if(regex->Exec())                                       //如果是true就是裡面有非法字元
                        {
                            len=0;
                            dataSplit=asBarCodeErrorSend;                       //JerryYang 20200714 不符合正則表達式要分ERROR

                        }
                        else
                        {
                            len=dataSplit.Length();
                        }
                    }
                    else
                    {
                        if(regex->Exec())
                        {
                            len=regex->MatchLen[0];                             //取出第一個找到符合Expression的字串長度
                            if(len!=dataSplit.Length())                         //JerryYang 20200714 不符合正則表達式要分ERROR
                            {
                                dataSplit=asBarCodeErrorSend;
                            }
                        }
                        else
                        {
                            len=0;
                            dataSplit=asBarCodeErrorSend;                       //JerryYang 20200714 不符合正則表達式要分ERROR
                        }
                    }
                }

                if(dataSplit=="ERROR" &&
                   CosFunction.bBarcodeErrNoTestAndShowH==true)                 //jou 20191007 : Barcode Error No Test & Show "H"
                {
                    fNote->t2DCode->Visible=true;
                    dataSplit=asBarCodeErrorSend;                               //wei 20160318 Barcode Error依客戶設定
                    if(Channel==iBarCode1_1 || Channel==iBarCode2_1)            //Steven 20160607 : 在畫面上顯示異常的字串
                    {
                        fNote->t2DCode->SetCellNumber(0, i, dataSplit);
                        fNote->t2DCode->SetCellColorIndex(0, i, 3);
                    }

                    if(Channel==iBarCode1_2 || Channel==iBarCode2_2)
                    {
                        fNote->t2DCode->SetCellNumber(1, i, dataSplit);
                        fNote->t2DCode->SetCellColorIndex(1, i, 3);
                    }
                }
                else if(len<TestIF_File.iBarCodeMinLength ||
                        len>TestIF_File.iBarCodeMaxLength ||
                        (dataSplit=="ERROR" && CUSTOMER_CODE==CC_KYEC_XILINX))  //Steven 20160602 : && --> ||  //
                {
                    fNote->t2DCode->Visible=true;
                    if(Channel==iBarCode1_1 || Channel==iBarCode2_1)            //Steven 20160607 : 在畫面上顯示異常的字串
                    {
                        fNote->t2DCode->SetCellNumber(0, i, dataSplit);
                        fNote->t2DCode->SetCellColorIndex(0, i, 3);
                    }

                    if(Channel==iBarCode1_2 || Channel==iBarCode2_2)
                    {
                        fNote->t2DCode->SetCellNumber(1, i, dataSplit);
                        fNote->t2DCode->SetCellColorIndex(1, i, 3);
                    }
                    dataSplit=asBarCodeErrorSend;                               //wei 20160318 Barcode Error依客戶設定
                }
                else if(TestIF_File.bCheckSum &&
                        CheckDigit(dataSplit)==false &&
                        CUSTOMER_CODE==CC_ASE_CL)                               //KaiHuang 20200826 : Fix KaiChen 20191121 ：中壢日月光 2D Check Sum
                {
                    fNote->t2DCode->Visible=true;

                    if(BOTTOM_2DID==1 &&
                       BOTTOM_2DID_CCD==1 &&
                       TestIF_File.bEnableBottom2D)                             //Bottom 8CCD
                    {
                        if(Channel==iBottom_1)
                        {
                            fNote->t2DCode->SetCellNumber(0, 1, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(0, 1, 3);
                        }
                        else if(Channel==iBottom_2)
                        {
                            fNote->t2DCode->SetCellNumber(1, 1, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(1, 1, 3);
                        }
                        else if(Channel==iBottom_3)
                        {
                            fNote->t2DCode->SetCellNumber(0, 2, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(1, 2, 3);
                        }
                        else if(Channel==iBottom_4)
                        {
                            fNote->t2DCode->SetCellNumber(1, 2, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(1, 2, 3);
                        }
                        else if(Channel==iBottom_5)
                        {
                            fNote->t2DCode->SetCellNumber(0, 3, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(0, 3, 3);
                        }
                        else if(Channel==iBottom_6)
                        {
                            fNote->t2DCode->SetCellNumber(1, 3, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(1, 3, 3);
                        }
                        else if(Channel==iBottom_7)
                        {
                            fNote->t2DCode->SetCellNumber(0, 4, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(0, 4, 3);
                        }
                        else
                        {
                            fNote->t2DCode->SetCellNumber(1, 4, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(1, 4, 3);
                        }
                    }
                    else  //Top CCD + Bottom 4CCD
                    {
                        if(Channel==iBarCode1_1 || Channel==iBarCode2_1)        //Steven 20160607 : 在畫面上顯示異常的字串
                        {
                            fNote->t2DCode->SetCellNumber(0, i, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(0, i, 3);
                        }

                        if(Channel==iBarCode1_2 || Channel==iBarCode2_2)
                        {
                            fNote->t2DCode->SetCellNumber(1, i, dataSplit);
                            fNote->t2DCode->SetCellColorIndex(1, i, 3);
                        }
                    }
                    dataSplit=asBarCodeErrorCheckSum;
                }
                else if(sList->Strings[iPos]=="" || sList->Strings[iPos]=="0")
                {
                    if(Channel==iBarCode1_1)
                    {
                        if(FLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                           FLCarryKit.Item[0][i]!=NULL_IC)
                        {
                            dataSplit=asBarCodeErrorSend;                       //wei 20160318 Barcode Error依客戶設定
                        }
                        else
                        {
                            dataSplit="";
                        }
                    }
                    else if(Channel==iBarCode1_2)
                    {
                        if(InArmSuck.iShtRow==2)                                //Ifor 20220411 : Fixed for 單排使用CCD2
                        {
                            if(FLCarryKit.Item[1][i]!=HAS_NULL_IC &&
                               FLCarryKit.Item[1][i]!=NULL_IC)
                            {
                                dataSplit=asBarCodeErrorSend;                   //wei 20160318 Barcode Error依客戶設定
                            }
                            else
                            {
                                dataSplit="";
                            }
                        }
                        else
                        {
                            if(FLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                               FLCarryKit.Item[0][i]!=NULL_IC)
                            {
                                dataSplit=asBarCodeErrorSend;                   //wei 20160318 Barcode Error依客戶設定
                            }
                            else
                            {
                                dataSplit="";
                            }
                        }
                    }
                    else if(Channel==iBarCode2_1)
                    {
                        if(BLCarryKit.Item[0][i]!=HAS_NULL_IC &&
                           BLCarryKit.Item[0][i]!=NULL_IC)
                        {
                            dataSplit=asBarCodeErrorSend;                       //wei 20160318 Barcode Error依客戶設定
                        }
                        else
                        {
                            dataSplit="";
                        }
                    }
                    else
                    {
                        if(InArmSuck.iShtRow==2)
                        {
                            if(BLCarryKit.Item[1][i]!=HAS_NULL_IC && BLCarryKit.Item[1][i]!=NULL_IC)
                            {
                                dataSplit=asBarCodeErrorSend;                   //wei 20160318 Barcode Error依客戶設定
                            }
                            else
                            {
                                dataSplit="";
                            }
                        }
                        else                                                    //IC在0的位置, 使用CCD4
                        {
                            if(BLCarryKit.Item[0][i]!=HAS_NULL_IC && BLCarryKit.Item[0][i]!=NULL_IC)
                            {
                                dataSplit=asBarCodeErrorSend;                   //wei 20160318 Barcode Error依客戶設定
                            }
                            else
                            {
                                dataSplit="";
                            }
                        }
                    }
                }
            }
            else
            {
                dataSplit=asBarCodeErrorSend;
            }

            if(BOTTOM_2DID_CCD==1 && BOTTOM_2DID==1 &&                          //RogerYang 20190319 add bottom 2did
               TestIF_File.bEnableBottom2D)                                     //KaiChen 20200513 : Bottom 2D 8CCD
            {
                if(Channel==iBottom_1)
                {
                    InArmSuck.cDeviceInf[0][0]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(0, 1,  dataSplit.c_str());
                }
                else if(Channel==iBottom_2)
                {
                    InArmSuck.cDeviceInf[1][0]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(0, 2,  dataSplit.c_str());
                }
                else if(Channel==iBottom_3)
                {
                    InArmSuck.cDeviceInf[0][1]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(1, 1,  dataSplit.c_str());
                }
                else if(Channel==iBottom_4)
                {
                    InArmSuck.cDeviceInf[1][1]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(1, 2,  dataSplit.c_str());
                }
                else if(Channel==iBottom_5)
                {
                    InArmSuck.cDeviceInf[0][2]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(2, 1,  dataSplit.c_str());
                }
                else if(Channel==iBottom_6)
                {
                    InArmSuck.cDeviceInf[1][2]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(2, 2,  dataSplit.c_str());
                }
                else if(Channel==iBottom_7)
                {
                    InArmSuck.cDeviceInf[0][3]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(3, 1,  dataSplit.c_str());
                }
                else
                {
                    InArmSuck.cDeviceInf[1][3]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(3, 2,  dataSplit.c_str());
                }
            }
            else if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)                 //Steven 20190308 : Bottom 2D
            {
                if(Channel==iBarCode1_1)
                {
                    InArmSuck.cDeviceInf[0][i]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(i, 1,  dataSplit.c_str());
                }
                else if(Channel==iBarCode1_2)
                {
                    InArmSuck.cDeviceInf[0][i+2]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(i+2, 1,  dataSplit.c_str());
                }
                else if(Channel==iBarCode2_1)
                {
                    InArmSuck.cDeviceInf[1][i]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(i, 2,  dataSplit.c_str());
                }
                else
                {
                    InArmSuck.cDeviceInf[1][i+2]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(i+2, 2,  dataSplit.c_str());
                }
            }
            else
            {
                mtBarcodeInSh->SetCellNumber(Channel, i+1,  dataSplit.c_str());

                if(Channel==iBarCode1_1)
                {
                    FLCarryKit.cDeviceInf[0][i]=dataSplit;
                    if(TestIF_File.i2DIDFormat==eAMD)                           //JerryYang 20200422 2DID format選項改用下拉選單
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(0, i+1, dataSplit.c_str());
                    }
                }
                else if(Channel==iBarCode1_2)
                {
                    if(InArmSuck.iShtRow==2)                                    //Ifor 20220411 : Fixed for 單排使用CCD2
                        FLCarryKit.cDeviceInf[1][i]=dataSplit;
                    else
                        FLCarryKit.cDeviceInf[0][i]=dataSplit;
                }
                else if(Channel==iBarCode2_1)
                {
                    BLCarryKit.cDeviceInf[0][i]=dataSplit;
                }
                else
                {
                    if(InArmSuck.iShtRow==2)
                        BLCarryKit.cDeviceInf[1][i]=dataSplit;
                    else                                                        //IC在0的位置, 使用CCD4
                        BLCarryKit.cDeviceInf[0][i]=dataSplit;
                    if(TestIF_File.i2DIDFormat==eAMD)                           //JerryYang 20200422 2DID format選項改用下拉選單
                    {
                        fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, i+1, dataSplit.c_str());
                    }
                }
            }
        }
    }
    sList->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete regex;
    delete sList;
}
//---------------------------------------------------------------------------
void TfBarCode::CCDBarcode_DecodingForMulti2D(int Channel)                      //Steven 20200810 : 一個IC使用多個2DID
{
    AnsiString dataSplit, Path;
    int iCol, len, iListCount, iPos, iTotalCount;
    AnsiString sPrefix;
    int iResult=-1;

    if(BOTTOM_2DID &&                                                           //RogerYang 20190319 add bottom 2did
       TestIF_File.bEnableBottom2D)                                             //Steven 20190308 : Bottom 2D
    {
        iCol=2;                                                                 //RogerYang 20190319 四吸嘴 1支ccd會有兩筆資料
    }
    else
    {
        iCol=InArmSuck.iShtCol;
    }
    iCol=iCol*TestIF_File.iMulti2DCount;

    TStringList *sList  =new TStringList();
    TRegExpr * regex    =new TRegExpr;                                          //Steven 20160602 : Add regular expression to check alphanumeric character

    if(TestIF_File.b2DIDStringFormat==0)                                        //RogerYang 20181222 新增String format選項 /*--- https://poychang.github.io/note-regular-expression/ ---*/
    {
        regex->Expression   ="(\\w+)";                                          // w = an alphanumeric character (including "_")
    }
    else if(TestIF_File.b2DIDStringFormat==1)
    {
        regex->Expression   ="(\\w+-\\w+)";                                     // w = an alphanumeric character (including "_") //Steven 20181001 : Add "-"
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==2)
    {
        regex->Expression   ="(\\w+[.]+\\w+)";                                  // w = an alphanumeric character (including "_") //Steven 20210303 : Add "."
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==3)
    {
        regex->Expression   ="(\\w+[.]+\\w+-\\w+)";                             // w = an alphanumeric character (including "_") //Steven 20210303 : Add "."
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==4)
    {
        regex->Expression   ="(\\w+-\\w+[.]+\\w+)";                             // w = an alphanumeric character (including "_") //Steven 20210303 : Add "."
                                                                                // + = one or more ("greedy")
    }
    else if(TestIF_File.b2DIDStringFormat==5)                                   //Steven 20210518 : 2DID改為找非法字元方式
    {
        regex->Expression   ="[^a-zA-Z0-9-: _.]";                               //不等於裡面指定的文字
    }

    sList->CommaText=sDownCCDRecevieData[Channel];
    WriteBarCodeLog(Channel, sDownCCDRecevieData[Channel]);
    iListCount=sList->Count;
    sPrefix=sList->Strings[0];

    if(dVisionVer>=2)                                                           //新版Command  //wei 20170119 (Steven) 版本號大於2
    {
        //--------------------
        //回傳格式
        //GV,0,1,11111111,Code1,Code2,Code3,Code4,Code5,Code6,Code7,Code8
        //--------------------
        if(iListCount>1)
            iResult=atoi(sList->Strings[2].c_str());

        iTotalCount=4+iCol;
    }
    else
    {
        //--------------------
        //回傳格式
        //GV,1,11111111,Code1,Code2,Code3,Code4,Code5,Code6,Code7,Code8
        //--------------------
        if(iListCount>1)
            iResult=atoi(sList->Strings[1].c_str());

        iTotalCount=3+iCol;
    }

    if(iListCount<iTotalCount  ||
       iResult==999)
    {
        for(int i=0; i<iCol; i++)
        {
            cMilti2DBuffer[Channel][i]=asBarCodeErrorSend;                      //wei 20160318 Barcode Error依客戶設定
        }
    }
    else
    {
        for(int i=0; i<iCol; i++)
        {
            if(sList->Count-1>=i)
            {
                if(dVisionVer>=2)                                               //新版Command      //wei 20170119 (Steven) 版本號大於2
                    iPos=i+4;
                else
                    iPos=i+3;
                dataSplit=sList->Strings[iPos];
                regex->InputString=dataSplit;                                   //Steven 20160602 : Add regular expression to check alphanumeric character
                if(TestIF_File.b2DIDStringFormat==5)                            //Steven 20210518 : 2DID改為找非法字元方式
                {
                    if(regex->Exec())                                           //如果是true就是裡面有非法字元
                    {
                        len=0;
                        dataSplit=asBarCodeErrorSend;                           //JerryYang 20200714 不符合正則表達式要分ERROR

                    }
                    else
                    {
                        len=dataSplit.Length();
                    }
                }
                else
                {
                    if(regex->Exec())
                    {
                        len=regex->MatchLen[0];                                 //取出第一個找到符合Expression的字串長度
                        if(len!=dataSplit.Length())                             //JerryYang 20200714 不符合正則表達式要分ERROR
                        {
                            dataSplit=asBarCodeErrorSend;
                        }
                    }
                    else
                    {
                        len=0;
                        dataSplit=asBarCodeErrorSend;                           //JerryYang 20200714 不符合正則表達式要分ERROR
                    }
                }

                if(dataSplit=="ERROR" &&
                   CosFunction.bBarcodeErrNoTestAndShowH==true)                 //jou 20191007 : Barcode Error No Test & Show "H"
                {
                    dataSplit=asBarCodeErrorSend;                               //wei 20160318 Barcode Error依客戶設定
                }
                else if(len<TestIF_File.iBarCodeMinLength ||
                        len>TestIF_File.iBarCodeMaxLength ||                    //Steven 20160602 : && --> ||  //
                        (dataSplit=="ERROR" && CUSTOMER_CODE==CC_KYEC_XILINX))
                {
                    dataSplit=asBarCodeErrorSend;                               //wei 20160318 Barcode Error依客戶設定
                }
                else if(TestIF_File.bCheckSum &&
                        CheckDigit(dataSplit)==false)                           //KaiChen 20191121 ：中壢日月光 2D Check Sum
                {
                    dataSplit=asBarCodeErrorCheckSum;
                }
            }
            else
            {
                dataSplit=asBarCodeErrorSend;
            }

            if(BOTTOM_2DID &&                                                   //RogerYang 20190319 add bottom 2did
               TestIF_File.bEnableBottom2D)                                     //Steven 20190308 : Bottom 2D
            {
                cMilti2DBuffer[Channel][i]=dataSplit;
            }
            else
            {
                cMilti2DBuffer[Channel][i]=dataSplit;
            }
        }
    }

    sList->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete regex;
    delete sList;
}
//------------------------------------------------------------------------------
void TfBarCode::CCDBarcode_SortingForMulti2D(int iSht)                          //Steven 20200810 : 一個IC使用多個2DID
{
    if(TestIF_File.bEnableMulti2D==false)
        return;

    std::map<int, AnsiString>mapMulti2D;                                        //Steven 20240612 : for 對角2D
    std::map<int, AnsiString>::iterator mapMulti2DIter;
    AnsiString dataSplit;//, Path;
    mapMulti2D.clear();
    int i2DRow, i2DCol;
    int iCol;//, len, iListCount, iPos, iTotalCount;

    if(iSht==0)
        ptrMulti2DIDSHT=&FLCarryKit;
    else
        ptrMulti2DIDSHT=&BLCarryKit;

    //RogerYang 20190319 add bottom 2did
    //==>
    if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)                              //Steven 20190308 : Bottom 2D
    {
        iCol=2;                                                                 //RogerYang 20190319 四吸嘴 1支ccd會有兩筆資料
    }
    else
    {
        iCol=InArmSuck.iShtCol;
    }
//    iCol=iCol*TestIF_File.iMulti2DXItem;

    if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)
    {
//        cMilti2DBuffer[Channel][i]=   //待處理
    }
    else
    {
        if(TestIF_File.iMulti2DType==e2x2In1CCD)
        {
            if(iSht==0 || (iSht==1 && InArmSuck.iShtRow==2))
            {
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    i2DRow=i+iSht*2;
                    for(int j=0; j<iCol; j++)
                    {
                        i2DCol=j*4;
                        dataSplit="";
                        mapMulti2D.clear();                                     //Steven 20240612 : for 對角2D
                        if(ptrMulti2DIDSHT->Item[i][j]!=HAS_NULL_IC &&
                           ptrMulti2DIDSHT->Item[i][j]!=NULL_IC)
                        {
                            if((TestIF_File.iMulti2DMap[0][0]>0 && (cMilti2DBuffer[i2DRow][i2DCol+0]=="" || cMilti2DBuffer[i2DRow][i2DCol+0]=="0")) ||
                               (TestIF_File.iMulti2DMap[1][0]>0 && (cMilti2DBuffer[i2DRow][i2DCol+1]=="" || cMilti2DBuffer[i2DRow][i2DCol+1]=="0")) ||
                               (TestIF_File.iMulti2DMap[0][1]>0 && (cMilti2DBuffer[i2DRow][i2DCol+2]=="" || cMilti2DBuffer[i2DRow][i2DCol+2]=="0")) ||
                               (TestIF_File.iMulti2DMap[1][1]>0 && (cMilti2DBuffer[i2DRow][i2DCol+3]=="" || cMilti2DBuffer[i2DRow][i2DCol+3]=="0")))
                            {
                                dataSplit=asBarCodeErrorSend;
                            }
                            else
                            {
                                if(TestIF_File.iMulti2DMap[0][0]>0)
                                {
                                    mapMulti2D[TestIF_File.iMulti2DMap[0][0]-1]=cMilti2DBuffer[i2DRow][i2DCol+0];
                                }
                                if(TestIF_File.iMulti2DMap[1][0]>0)
                                {
                                    mapMulti2D[TestIF_File.iMulti2DMap[1][0]-1]=cMilti2DBuffer[i2DRow][i2DCol+1];
                                }
                                if(TestIF_File.iMulti2DMap[0][1]>0)
                                {
                                    mapMulti2D[TestIF_File.iMulti2DMap[0][1]-1]=cMilti2DBuffer[i2DRow][i2DCol+2];
                                }
                                if(TestIF_File.iMulti2DMap[1][1]>0)
                                {
                                    mapMulti2D[TestIF_File.iMulti2DMap[1][1]-1]=cMilti2DBuffer[i2DRow][i2DCol+3];
                                }

                                for(mapMulti2DIter=mapMulti2D.begin(); mapMulti2DIter!=mapMulti2D.end(); mapMulti2DIter++)  //Steven 20240612 : for 對角2D
                                {
                                    dataSplit=dataSplit+mapMulti2DIter->second+AnsiString(" ");
                                }
                            }
                        }
                        dataSplit.Trim();
                        ptrMulti2DIDSHT->cDeviceInf[i][j]=dataSplit;
                        mtBarcodeInSh->SetCellNumber(i2DRow, j+1,  dataSplit.c_str());
                    }
                }
            }
            else
            {
                for(int j=0; j<iCol; j++)
                {
                    i2DRow=3;
                    i2DCol=j*4;
                    dataSplit="";
                    mapMulti2D.clear();                                         //Steven 20240612 : for 對角2D
                    if(ptrMulti2DIDSHT->Item[0][j]!=HAS_NULL_IC &&
                       ptrMulti2DIDSHT->Item[0][j]!=NULL_IC)
                    {
                        if((TestIF_File.iMulti2DMap[0][0]>0 && (cMilti2DBuffer[i2DRow][i2DCol+0]=="" || cMilti2DBuffer[i2DRow][i2DCol+0]=="0")) ||
                           (TestIF_File.iMulti2DMap[1][0]>0 && (cMilti2DBuffer[i2DRow][i2DCol+1]=="" || cMilti2DBuffer[i2DRow][i2DCol+1]=="0")) ||
                           (TestIF_File.iMulti2DMap[0][1]>0 && (cMilti2DBuffer[i2DRow][i2DCol+2]=="" || cMilti2DBuffer[i2DRow][i2DCol+2]=="0")) ||
                           (TestIF_File.iMulti2DMap[1][1]>0 && (cMilti2DBuffer[i2DRow][i2DCol+3]=="" || cMilti2DBuffer[i2DRow][i2DCol+3]=="0")))
                        {
                            dataSplit=asBarCodeErrorSend;
                        }
                        else
                        {
                            if(TestIF_File.iMulti2DMap[0][0]>0)
                            {
                                mapMulti2D[TestIF_File.iMulti2DMap[0][0]-1]=cMilti2DBuffer[i2DRow][i2DCol+0];
                            }
                            if(TestIF_File.iMulti2DMap[1][0]>0)
                            {
                                mapMulti2D[TestIF_File.iMulti2DMap[1][0]-1]=cMilti2DBuffer[i2DRow][i2DCol+1];
                            }
                            if(TestIF_File.iMulti2DMap[0][1]>0)
                            {
                                mapMulti2D[TestIF_File.iMulti2DMap[0][1]-1]=cMilti2DBuffer[i2DRow][i2DCol+2];
                            }
                            if(TestIF_File.iMulti2DMap[1][1]>0)
                            {
                                mapMulti2D[TestIF_File.iMulti2DMap[1][1]-1]=cMilti2DBuffer[i2DRow][i2DCol+3];
                            }

                            for(mapMulti2DIter=mapMulti2D.begin(); mapMulti2DIter!=mapMulti2D.end(); mapMulti2DIter++)  //Steven 20240612 : for 對角2D
                            {
                                dataSplit=dataSplit+mapMulti2DIter->second+AnsiString(" ");
                            }
                        }
                    }
                    dataSplit.Trim();
                    ptrMulti2DIDSHT->cDeviceInf[0][j]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(2, j+1,  dataSplit.c_str());
                }
            }
        }
        else if(TestIF_File.iMulti2DType==e2x1In2CCD)
        {
            i2DRow=iSht*2;
            for(int j=0; j<iCol; j++)
            {
                i2DCol=j*TestIF_File.iMulti2DXItem;
                dataSplit="";
                if(ptrMulti2DIDSHT->Item[0][j]!=HAS_NULL_IC && ptrMulti2DIDSHT->Item[0][j]!=NULL_IC)
                {
                    if(cMilti2DBuffer[i2DRow+0][i2DCol]=="" || cMilti2DBuffer[i2DRow+0][i2DCol]=="0" ||
                       cMilti2DBuffer[i2DRow+1][i2DCol]=="" || cMilti2DBuffer[i2DRow+1][i2DCol]=="0")
                    {
                        dataSplit=asBarCodeErrorSend;
                    }
                    else
                    {
                        if(TestIF_File.iMulti2DMap[0][0]==1)
                            dataSplit.sprintf("%s %s", asBarCodeErrorSend=cMilti2DBuffer[i2DRow+0][i2DCol], cMilti2DBuffer[i2DRow+1][i2DCol]);
                        else
                            dataSplit.sprintf("%s %s", asBarCodeErrorSend=cMilti2DBuffer[i2DRow+1][i2DCol], cMilti2DBuffer[i2DRow+0][i2DCol]);

                    }
                }
                dataSplit.Trim();
                ptrMulti2DIDSHT->cDeviceInf[0][j]=dataSplit;
                mtBarcodeInSh->SetCellNumber(i2DRow, j+1,  dataSplit.c_str());
                mtBarcodeInSh->SetCellNumber(i2DRow+1, j+1,  "");
            }
        }
        else if(TestIF_File.iMulti2DType==e2x2In2CCD)
        {
            i2DRow=iSht*2;
            for(int j=0; j<iCol; j++)
            {
                i2DCol=j*TestIF_File.iMulti2DXItem;
                dataSplit="";
                mapMulti2D.clear();                                             //Steven 20240612 : for 對角2D
                if(ptrMulti2DIDSHT->Item[0][j]!=HAS_NULL_IC && ptrMulti2DIDSHT->Item[0][j]!=NULL_IC)
                {
                          //AI(ht9045-v899) 20260519: match e2x2In2CCD missing barcode check with 2x2 buffer mapping.
                          if((TestIF_File.iMulti2DMap[0][0]>0 && (cMilti2DBuffer[i2DRow+0][i2DCol+0]=="" || cMilti2DBuffer[i2DRow+0][i2DCol+0]=="0")) ||
                              (TestIF_File.iMulti2DMap[1][0]>0 && (cMilti2DBuffer[i2DRow+1][i2DCol+0]=="" || cMilti2DBuffer[i2DRow+1][i2DCol+0]=="0")) ||
                              (TestIF_File.iMulti2DMap[0][1]>0 && (cMilti2DBuffer[i2DRow+0][i2DCol+1]=="" || cMilti2DBuffer[i2DRow+0][i2DCol+1]=="0")) ||
                              (TestIF_File.iMulti2DMap[1][1]>0 && (cMilti2DBuffer[i2DRow+1][i2DCol+1]=="" || cMilti2DBuffer[i2DRow+1][i2DCol+1]=="0")))
                    {
                        dataSplit=asBarCodeErrorSend;
                    }
                    else
                    {
                        if(TestIF_File.iMulti2DMap[0][0]>0)
                        {
                            mapMulti2D[TestIF_File.iMulti2DMap[0][0]-1]=cMilti2DBuffer[i2DRow+0][i2DCol+0];
                        }
                        if(TestIF_File.iMulti2DMap[1][0]>0)
                        {
                            mapMulti2D[TestIF_File.iMulti2DMap[1][0]-1]=cMilti2DBuffer[i2DRow+1][i2DCol+0];
                        }
                        if(TestIF_File.iMulti2DMap[0][1]>0)
                        {
                            mapMulti2D[TestIF_File.iMulti2DMap[0][1]-1]=cMilti2DBuffer[i2DRow+0][i2DCol+1];
                        }
                        if(TestIF_File.iMulti2DMap[1][1]>0)
                        {
                            mapMulti2D[TestIF_File.iMulti2DMap[1][1]-1]=cMilti2DBuffer[i2DRow+1][i2DCol+1];
                        }

                        for(mapMulti2DIter=mapMulti2D.begin(); mapMulti2DIter!=mapMulti2D.end(); mapMulti2DIter++)  //Steven 20240612 : for 對角2D
                        {
                            dataSplit=dataSplit+mapMulti2DIter->second+AnsiString(" ");
                        }
                    }
                }
                dataSplit.Trim();
                ptrMulti2DIDSHT->cDeviceInf[0][j]=dataSplit;
                mtBarcodeInSh->SetCellNumber(i2DRow, j+1,  dataSplit.c_str());
                mtBarcodeInSh->SetCellNumber(i2DRow+1, j+1, "");
            }
        }
        else// if(TestIF_File.iMulti2DType==e1x2In1CCD ||
            //    TestIF_File.iMulti2DType==e2x1In1CCD)
        {
            if(iSht==0 || (iSht==1 && InArmSuck.iShtRow==2))
            {
                for(int i=0; i<InArmSuck.iShtRow; i++)
                {
                    i2DRow=i+iSht*2;
                    for(int j=0; j<iCol; j++)
                    {
                        i2DCol=j*2;
                        dataSplit="";
                        if(ptrMulti2DIDSHT->Item[i][j]!=HAS_NULL_IC && ptrMulti2DIDSHT->Item[i][j]!=NULL_IC)
                        {
                            if(cMilti2DBuffer[i2DRow][i2DCol+0]=="" || cMilti2DBuffer[i2DRow][i2DCol+0]=="0" ||
                               cMilti2DBuffer[i2DRow][i2DCol+1]=="" || cMilti2DBuffer[i2DRow][i2DCol+1]=="0")
                            {
                                dataSplit=asBarCodeErrorSend;
                            }
                            else
                            {
                                if(TestIF_File.iMulti2DMap[0][0]==1)
                                    dataSplit.sprintf("%s %s", asBarCodeErrorSend=cMilti2DBuffer[i2DRow][i2DCol+0], cMilti2DBuffer[i2DRow][i2DCol+1]);
                                else
                                    dataSplit.sprintf("%s %s", asBarCodeErrorSend=cMilti2DBuffer[i2DRow][i2DCol+1], cMilti2DBuffer[i2DRow][i2DCol+0]);
                            }
                        }
                        dataSplit.Trim();
                        ptrMulti2DIDSHT->cDeviceInf[i][j]=dataSplit;
                        mtBarcodeInSh->SetCellNumber(i2DRow, j+1,  dataSplit.c_str());
                    }
                }
            }
            else
            {
                for(int j=0; j<iCol; j++)
                {
                    i2DRow=3;
                    i2DCol=j*2;
                    dataSplit="";
                    if(ptrMulti2DIDSHT->Item[0][j]!=HAS_NULL_IC && ptrMulti2DIDSHT->Item[0][j]!=NULL_IC)
                    {
                        if(cMilti2DBuffer[i2DRow][i2DCol+0]=="" || cMilti2DBuffer[i2DRow][i2DCol+0]=="0" ||
                           cMilti2DBuffer[i2DRow][i2DCol+1]=="" || cMilti2DBuffer[i2DRow][i2DCol+1]=="0")
                        {
                            dataSplit=asBarCodeErrorSend;
                        }
                        else
                        {
                            if(TestIF_File.iMulti2DMap[0][0]==1)
                                dataSplit.sprintf("%s %s", asBarCodeErrorSend=cMilti2DBuffer[i2DRow][i2DCol+0], cMilti2DBuffer[i2DRow][i2DCol+1]);
                            else
                                dataSplit.sprintf("%s %s", asBarCodeErrorSend=cMilti2DBuffer[i2DRow][i2DCol+1], cMilti2DBuffer[i2DRow][i2DCol+0]);

                        }
                    }
                    dataSplit.Trim();
                    ptrMulti2DIDSHT->cDeviceInf[0][j]=dataSplit;
                    mtBarcodeInSh->SetCellNumber(2, j+1,  dataSplit.c_str());
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
enum e2dAutoLearnTask
{
    e2dALInit           =1,
    e2dALChkSh1HasIC    =2,
    e2dALMoveSh1        =3,
    e2dALChkSh2HasIC    =4,
    e2dALMoveSh2        =5,
    e2dALMoveDelayOn    =6,
    e2dALMoveDelay      =7,
    e2dALSetSE2         =8,
    e2dALGetSE2         =9,
    e2dALSetSE4         =10,                                                    //Steven 20120126 : 只拍一張, 只確認有無讀取
    e2dALGetSE4         =11,
    e2dALSetSE3         =12,
    e2dALGetSE3         =13,
    e2dALFinish

};
e2dAutoLearnTask i2dAutoLearnTask;
TQPF_Timer t2dAutoLearnDelay;
TQPF_Timer ChangeFileDelay;
//---------------------------------------------------------------------------
bool TfBarCode::DoBarcodeCCDAutoTeach(bool bReset)                              //Steven 20160118 : 自動學習2D code Matrix
{
    e2dAutoLearnTask &Task=i2dAutoLearnTask;
    static int iNowMoveStep1=0, iNowMoveStep2=0;
    static int iTeachPosition=iBarCode1_1;
    bool bResult=false;
    int pos=0;

    if(bReset)
    {
        Task=e2dALInit;
        return bResult;
    }

    switch(Task)
    {
        case e2dALInit:
            iNowMoveStep1=InArmSuck.iShtCol;                                    //遞減方式檢查
            iNowMoveStep2=InArmSuck.iShtCol;                                    //遞減方式檢查
            for(int i=0; i<BAR_CODE_COUNT; i++)
            {
                iGetSE2[i]=-1;                                                  //Steven 20160120 : Auto Teach 2D Matrix
                iGetSE3[i]=-1;                                                  //Steven 20120120 : Auto Learn 2D ROI
            }
            Task=e2dALChkSh1HasIC;
            fContact->Memo1->Lines->Add("Shuttle 1 moving");
            break;
        case e2dALChkSh1HasIC:
            iNowMoveStep1--;

            if(iNowMoveStep1>=0 && iNowMoveStep1<InArmSuck.iShtCol)
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)  //只做蝦頭二
                {
                    fContact->Memo1->Lines->Add("Shuttle 2 moving");
                    Task=e2dALChkSh2HasIC;
                }
                else if(InArmSuck.iShtRow==2)
                {
                    if((FLCarryKit.Item[0][iNowMoveStep1]==HAS_NULL_IC || FLCarryKit.Item[0][iNowMoveStep1]==NULL_IC) &&
                       (FLCarryKit.Item[1][iNowMoveStep1]==HAS_NULL_IC || FLCarryKit.Item[1][iNowMoveStep1]==NULL_IC))
                    {
                        Task=e2dALChkSh1HasIC;
                    }
                    else
                    {
                        Task=e2dALMoveSh1;
                    }
                }
                else
                {
                    if(FLCarryKit.Item[0][iNowMoveStep1]==HAS_NULL_IC || FLCarryKit.Item[0][iNowMoveStep1]==NULL_IC)
                    {
                        Task=e2dALChkSh1HasIC;
                        break;
                    }
                    else
                    {
                        Task=e2dALMoveSh1;
                    }
                }
            }
            else                                                                //都沒料所以跳蝦頭二
            {
                fContact->Memo1->Lines->Add("Shuttle 2 moving");
                Task=e2dALChkSh2HasIC;
            }
            break;
        case e2dALMoveSh1:
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSHBarCodeDetectPos1x1[0];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSHBarCodeDetectPos1x2[0][iNowMoveStep1];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSHBarCodeDetectPos2x3[0][iNowMoveStep1];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSHBarCodeDetectPos1x4[0][iNowMoveStep1];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221027 : Add for 2x5
            {
                pos=Prod.iInSHBarCodeDetectPos2x5[0][iNowMoveStep1];
            }
            else if(InArmSuck.iShtCol==6)                                       //Steven 20170111 : fixed for 2x6
            {
                pos=Prod.iInSHBarCodeDetectPos2x6[0][iNowMoveStep1];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSHBarCodeDetectPos2x8[0][iNowMoveStep1];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeCCDAutoTeach");
            }

            pos+=Offset.iSHLeft2D[0];

            if(MOT[MInShuttle1].MotorMove(pos))
            {
                Task=e2dALChkSh2HasIC;
            }
            break;
        case e2dALChkSh2HasIC:
            iNowMoveStep2--;

            if(iNowMoveStep2>=0 && iNowMoveStep2<InArmSuck.iShtCol)
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)  //只做蝦頭一
                {
                    Task=e2dALMoveDelayOn;
                }
                else if(InArmSuck.iShtRow==2)
                {
                    if((FLCarryKit.Item[0][iNowMoveStep2]==HAS_NULL_IC || FLCarryKit.Item[0][iNowMoveStep2]==NULL_IC) &&
                       (FLCarryKit.Item[1][iNowMoveStep2]==HAS_NULL_IC || FLCarryKit.Item[1][iNowMoveStep2]==NULL_IC))
                    {
                        Task=e2dALChkSh2HasIC;
                    }
                    else
                    {
                        Task=e2dALMoveSh2;
                    }
                }
                else
                {
                    if(FLCarryKit.Item[0][iNowMoveStep2]==HAS_NULL_IC || FLCarryKit.Item[0][iNowMoveStep2]==NULL_IC)
                    {
                        Task=e2dALChkSh2HasIC;
                        break;
                    }
                    else
                    {
                        Task=e2dALMoveSh2;
                    }
                }
            }
            else                                                                //都沒料所以跳過
            {
                Task=e2dALMoveDelayOn;
            }
            break;
        case e2dALMoveSh2:
            if(InArmSuck.iShtCol==1)
            {
                pos=Prod.iInSHBarCodeDetectPos1x1[1];
            }
            else if(InArmSuck.iShtCol==2)
            {
                pos=Prod.iInSHBarCodeDetectPos1x2[1][iNowMoveStep2];
            }
            else if(InArmSuck.iShtCol==3)                                       //ChungHung 20140115 add for 2x3_6
            {
                pos=Prod.iInSHBarCodeDetectPos2x3[1][iNowMoveStep2];
            }
            else if(InArmSuck.iShtCol==4)
            {
                pos=Prod.iInSHBarCodeDetectPos1x4[1][iNowMoveStep2];
            }
            else if(InArmSuck.iShtCol==5)                                       //Steven 20221027 : Add for 2x5
            {
                pos=Prod.iInSHBarCodeDetectPos2x5[1][iNowMoveStep2];
            }
            else if(InArmSuck.iShtCol==6)                                       //Steven 20170111 : fixed for 2x6
            {
                pos=Prod.iInSHBarCodeDetectPos2x6[1][iNowMoveStep2];
            }
            else if(InArmSuck.iShtCol==8)
            {
                pos=Prod.iInSHBarCodeDetectPos2x8[1][iNowMoveStep2];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeCCDAutoTeach");
            }

            pos+=Offset.iSHLeft2D[1];

            if(MOT[MInShuttle2].MotorMove(pos))
            {
                Task=e2dALMoveDelayOn;
            }
            break;
        case e2dALMoveDelayOn:
            t2dAutoLearnDelay.SetMSAndOn(100);
            break;
        case e2dALMoveDelay:
            if(t2dAutoLearnDelay.Off())
                Task=e2dALSetSE2;
            break;
        case e2dALSetSE2:
            if(TestIF_File.iShuttleMode==0)
            {
                if(InArmSuck.iShtRow==2)
                {
                    iTeachPosition=iBarCode2_1;
                }
                else
                {
                    iTeachPosition=iBarCode2_2;
                }
            }
            else if(TestIF_File.iShuttle_Sel==1)
            {
                iTeachPosition=iBarCode1_1;
            }
            t2dAutoLearnDelay.SetSecAndOn(10);
            fContact->Memo1->Lines->Add("Auto train code by CCD"+AnsiString(iTeachPosition+1));
            SendCCDCommand(iTeachPosition, "Auto Teach", "SE2");
            Task=e2dALGetSE2;
            break;
        case e2dALGetSE2:
            if(iGetSE2[iTeachPosition]==1)
            {
                fContact->Memo1->Lines->Add("Auto train code by CCD"+AnsiString(iTeachPosition+1)+" OK");
                Task=e2dALSetSE3;
            }
            else if(iGetSE2[iTeachPosition]==0)
            {
                fContact->Memo1->Lines->Add("Auto train code by CCD"+AnsiString(iTeachPosition+1)+" Fail");
                fContact->Memo1->Lines->Add("Please train code manually by CCD"+AnsiString(iTeachPosition+1)+" Fail");
                ShowMyMessage("Train 2D Code Fail!", "Please train code manually");
                Task=e2dALSetSE4;
            }
            else if(t2dAutoLearnDelay.Off())                                    //2D Matrix Teach Fail
            {
                fContact->Memo1->Lines->Add("Auto train code by CCD"+AnsiString(iTeachPosition+1)+" Time Out");
                Task=e2dALSetSE2;
            }
            break;
        case e2dALSetSE4:
            t2dAutoLearnDelay.SetSecAndOn(10);
            fContact->Memo1->Lines->Add("Check code by CCD"+AnsiString(iTeachPosition+1));
            iGetSE4[iTeachPosition]=-1;
            SendCCDCommand(iTeachPosition, "Auto Teach check code", "SE4");
            Task=e2dALGetSE4;
            break;
        case e2dALGetSE4:
            if(iGetSE4[iTeachPosition]==1)
            {
                Task=e2dALSetSE3;
            }
            else if(iGetSE4[iTeachPosition]>1)
            {
                iGetSE2[iTeachPosition]=0;
                Task=e2dALGetSE2;
            }
            else if(t2dAutoLearnDelay.Off())
            {
                ShowMyMessage("Clear 2D Code buffer time out!", "Please check the connection.");
                Task=e2dALSetSE4;
            }
            break;
        case e2dALSetSE3:
            fContact->Memo1->Lines->Add("Auto learn ROI");

            for(int i=0; i<BAR_CODE_COUNT; i++)
                iGetSE3[i]=true;

            if(TestIF_File.iShuttleMode==0)
            {
                iGetSE3[iBarCode1_1]=false;
                iGetSE3[iBarCode2_2]=false;
                SendCCDCommand(iBarCode1_1, "Auto Learn", "SE3");
                SendCCDCommand(iBarCode2_2, "Auto Learn", "SE3");

                if(InArmSuck.iShtRow==2)
                {
                    iGetSE3[iBarCode1_2]=false;
                    iGetSE3[iBarCode2_1]=false;
                    SendCCDCommand(iBarCode1_2, "Auto Learn", "SE3");
                    SendCCDCommand(iBarCode2_1, "Auto Learn", "SE3");
                }
            }
            else
            {
                if(TestIF_File.iShuttle_Sel==0)
                {
                    iGetSE3[iBarCode1_1]=false;
                    SendCCDCommand(iBarCode1_1, "Auto Learn", "SE3");
                    if(InArmSuck.iShtRow==2)
                    {
                        iGetSE3[iBarCode1_2]=false;
                        SendCCDCommand(iBarCode1_2, "Auto Learn", "SE3");
                    }
                }
                else
                {
                    if(InArmSuck.iShtRow==2)
                    {
                        iGetSE3[iBarCode2_1]=false;
                        SendCCDCommand(iBarCode2_1, "Auto Learn", "SE3");
                    }
                    iGetSE3[iBarCode2_2]=false;
                    SendCCDCommand(iBarCode2_2, "Auto Learn", "SE3");
                }
            }
            t2dAutoLearnDelay.SetSecAndOn(10);
            break;
        case e2dALGetSE3:
            if(iGetSE3[iBarCode1_1] && iGetSE3[iBarCode1_2] && iGetSE3[iBarCode2_1] && iGetSE3[iBarCode2_2])
            {
                Task=e2dALFinish;
                fContact->Memo1->Lines->Add("Auto learn ROI ok.");
            }
            else if(t2dAutoLearnDelay.Off())
            {
                Task=e2dALSetSE3;
                fContact->Memo1->Lines->Add("Auto learn ROI time out!!");
            }
            break;
        case e2dALFinish:
            bResult=true;
            break;
    }

    return bResult;
}
//---------------------------------------------------------------------------
void TfBarCode::ClearLogName()                                                  //Steven 20160728 : Log by running mode
{
    asBarcodeLogName="";
    WriteIniData(asBarCodeLogPath+"\\LastBarcodeLog.ini", "Bar Code", "Last Barcode Log Name", asBarcodeLogName);
}
//---------------------------------------------------------------------------
void TfBarCode::WriteBarCodeLog(int index, AnsiString Message)                  //Steven 20160728 : Log by running mode
{
    AnsiString DataStr, Path, str;
    DataStr.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %s, Shuttle%d-%d, %s",
                    SystemYear, SystemMonth, SystemDate,
                    SystemHour, SystemMin, SystemSec, SystemMSec,
                    StartModeName[LastSet.iRunStartMode],                       //Steven 20160727 : 2DID log format
                    index/2+1, index%2+1,
                    Message);

    Path.sprintf("%s\\%04d_%02d_%02d\\", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path, "WriteBarCodeLog");

    if(asBarcodeLogName=="" || LastSet.iRunStartMode!=iRunningMode)             //Steven 20160728 : Log by running mode
    {
        if(LastSet.iRunStartMode==rsmContinuStart || LastSet.iRunStartMode==rsmInitialStart)
            str="FT";
        else if(LastSet.iRunStartMode==rsmContinuRetest || LastSet.iRunStartMode==rsmCInitialRetest)
            str="RT";
        else if(LastSet.iRunStartMode==rsmAutoSiteMap)
            str="ASM";
        else if(LastSet.iRunStartMode==rsmQAMode)
            str="QA";
        else if(LastSet.iRunStartMode==rsmContinuEQC || LastSet.iRunStartMode==rsmInitialEQC)
            str="EQC";
        else if(LastSet.iRunStartMode==rsmInitial_ART || LastSet.iRunStartMode==rsmContinuStart_ART ||
                LastSet.iRunStartMode==rsmContinuRetest_ART || LastSet.iRunStartMode==rsmAutoRetest)
            str="ART";
        else if(LastSet.iRunStartMode==rsmFIFOMode)
            str="FIFO";

        Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d%02d%02d_%s.csv", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour, SystemMin, SystemSec, str);
        iRunningMode=LastSet.iRunStartMode;
        asBarcodeLogName=Path;
        WriteIniData(asBarCodeLogPath+"\\LastBarcodeLog.ini", "Bar Code", "Last Barcode Log Name", asBarcodeLogName);
    }
    WriteDataToFile(asBarcodeLogName.c_str() , DataStr.c_str());

    lb_2DBarcodeData->Items->Add(DataStr);
    if(lb_2DBarcodeData->Items->Count>100)
        lb_2DBarcodeData->Items->Clear();
}
//---------------------------------------------------------------------------
void TfBarCode::SendCCDCommand(int index, AnsiString Msg, AnsiString Msg2)
{
    if(memoCCDCommLog==NULL || InitialOK==false)
        return;

    AnsiString Log;
    GetTimeInfo();

    if(Msg2.Pos("LF")!=0)
    {
        OCRChangeFileTimeOut.SetSecAndOn(5);
        bChangeSetupFile=true;
    }

    if(Msg2=="")
        Log.sprintf("Action, %s", Msg);
    else
        Log.sprintf("Send, %s, %s", Msg, Msg2);

    AddCCDCommunicationLog(index, Log);

    if(BOTTOM_2DID_CCD==1 && BOTTOM_2DID==1 &&
       TestIF_File.bEnableBottom2D)                                             //KaiChen 20200513 : Bottom 2D 8CCD
    {
        if(index==iBottom_1)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_1->Active)
                {
                    ClientSocket_Bottom_1->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
        if(index==iBottom_2)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_2->Active)
                {
                    ClientSocket_Bottom_2->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
        if(index==iBottom_3)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_3->Active)
                {
                    ClientSocket_Bottom_3->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
        if(index==iBottom_4)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_4->Active)
                {
                    ClientSocket_Bottom_4->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
        if(index==iBottom_5)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_5->Active)
                {
                    ClientSocket_Bottom_5->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
        if(index==iBottom_6)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_6->Active)
                {
                    ClientSocket_Bottom_6->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
        if(index==iBottom_7)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_7->Active)
                {
                    ClientSocket_Bottom_7->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
        if(index==iBottom_8)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Bottom_8->Active)
                {
                    ClientSocket_Bottom_8->Socket->SendText(Msg2+"\r\n");
                }
            }
        }
    }
    else
    {
        if(index==iBarCode1_1)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Shuttle1_A->Active)
                {
                    if(BAR_CODE_INSTALL==ebcUseOCR)
                    {
                        ClientSocket_Shuttle1_A->Socket->SendText(Msg2);
                    }
                    else
                    {
                        ClientSocket_Shuttle1_A->Socket->SendText(Msg2+"\r\n");
                    }
                }
            }
        }
        else if(index==iBarCode1_2)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Shuttle1_B->Active)
                {
                    if(BAR_CODE_INSTALL==ebcUseOCR)
                    {
                        ClientSocket_Shuttle1_B->Socket->SendText(Msg2);
                    }
                    else
                    {
                        ClientSocket_Shuttle1_B->Socket->SendText(Msg2+"\r\n");
                    }
                }
            }
        }
        else if(index==iBarCode2_1)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Shuttle2_A->Active)
                {
                    if(BAR_CODE_INSTALL==ebcUseOCR)
                    {
                        ClientSocket_Shuttle2_A->Socket->SendText(Msg2);
                    }
                    else
                    {
                        ClientSocket_Shuttle2_A->Socket->SendText(Msg2+"\r\n");
                    }
                }
            }
        }
        else if(index==iBarCode2_2)
        {
            if(Msg2=="")
            {
            }
            else
            {
                if(ClientSocket_Shuttle2_B->Active)
                {
                    if(BAR_CODE_INSTALL==ebcUseOCR)
                    {
                        ClientSocket_Shuttle2_B->Socket->SendText(Msg2);
                    }
                    else
                    {
                        ClientSocket_Shuttle2_B->Socket->SendText(Msg2+"\r\n");
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfBarCode::Change2DSetupFile()
{
    if(BOTTOM_2DID_CCD==1 && BOTTOM_2DID==1 &&
       TestIF_File.bEnableBottom2D)                                             //KaiChen 20200513 : Bottom 2D 8CCD
    {
        iBottomConntectionOkTask1=1;
        iBottomConntectionOkTask2=1;
    }
    else
    {
        iConntectionOkTask=1;
    }
}
//---------------------------------------------------------------------------
//void TfBarCode::Sync2DDateTime()    //Steven 20160425 : 重置2DSys時間
//{
//    iConntectionOkTask=dVisionVer;
//}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::TimerCCDInitialTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;

    int &Task=iConntectionOkTask;
    AnsiString Str,sBuffer;

    if(ClientSocket_Shuttle1_A->Active && ClientSocket_Shuttle1_B->Active &&    //相同的IP跟Port就不要重新連線
       ClientSocket_Shuttle2_A->Active && ClientSocket_Shuttle2_B->Active )     //ifor 20230116 change 避免重新連線斷線
    {
        switch(Task)
        {
            case 1:
                SetSFCCheckStepCount();
                dVisionVer=1;
                Str.sprintf("VR");
                SendCCDCommand(iBarCode1_1, "Get Version", Str);                //Reset CCD Shuttle暫存器
                bSFCGetVR=false;
                tChangeSetupFile.SetSecAndOn(30);
                Task=100;

            case 100:
                if(bSFCGetVR==true)
                {
                    if(TestIF_File.bEnableBarCode)
                        Task=1000;
                    else if(TestIF_File.bEnableShtFloatChk)
                        Task=2000;
                    else
                        Task=3000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=1;
                }
                break;
            case 1000:                                                          //Reset CCD Shuttle暫存器
                bGetSE9[iBarCode1_1]=false;
                ClearBuffer(iBarCode1_1, "Clear Buffer when CCD initial");
                ClearBuffer(iBarCode1_2, "Clear Buffer when CCD initial");
                ClearBuffer(iBarCode2_1, "Clear Buffer when CCD initial");
                ClearBuffer(iBarCode2_2, "Clear Buffer when CCD initial");
                tChangeSetupFile.SetSecAndOn(10);
                Task=1050;
                //break;
            case 1050:
                if(bGetSE9[iBarCode1_1])
                    Task=1100;
                else if(tChangeSetupFile.Off())
                    Task=1000;
                break;
            case 1100:                                                          //取得工作檔名稱
                bGetGF=false;
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    SendCCDCommand(iBarCode1_1, "Get File Name", "GF,0");
                else
                    SendCCDCommand(iBarCode1_1, "Get File Name", "GF");
                tChangeSetupFile.SetSecAndOn(10);
                Task=1150;
                //break;
            case 1150:
                if(bGetGF)
                {
                    //Steven 20191007 : Mark掉避免沒有切換Top跟Bottom
//                    if(fMain->cbSetupFileName->Text.UpperCase().AnsiCompare(sCurrentGF)==0)
//                    {
//                        Task=2000;
//                        bGetLF=true;
//                    }
//                    else
                    {
                        Task=1200;
                    }
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=1100;
                }
                break;
            case 1200:
                bGetLF=false;

                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 20210818
                {
                    if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)              //Steven 20190524 : Add bottom 2DID
                    {
                        if(dVisionVer>=2)                                       //wei 20170119 (Steven) 版本號大於2
                            Str.sprintf("LF,1,%s", TestIF_File.s2DFileName);
                        else if(TestIF_File.bEnableMulti2D)                     //Steven 20200810 : 一個IC使用多個2DID
                            Str.sprintf("LF%s,1,%d", TestIF_File.s2DFileName, TestIF_File.iMulti2DCount);
                        else
                            Str.sprintf("LF%s,1", TestIF_File.s2DFileName);
                    }
                    else
                    {
                        if(dVisionVer>=2)                                       //wei 20170119 (Steven) 版本號大於2
                            Str.sprintf("LF,0,%s,0", TestIF_File.s2DFileName);
                        else if(TestIF_File.bEnableMulti2D)                     //Steven 20200810 : 一個IC使用多個2DID
                            Str.sprintf("LF%s,0,%d", TestIF_File.s2DFileName, TestIF_File.iMulti2DCount);
                        else
                            Str.sprintf("LF%s,0", TestIF_File.s2DFileName);
                    }
                }
                else
                {
                    if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)              //Steven 20190524 : Add bottom 2DID
                    {
                        if(dVisionVer>=2)                                       //wei 20170119 (Steven) 版本號大於2
                            Str.sprintf("LF,0,%s,1", fMain->cbSetupFileName->Text);
                        else if(TestIF_File.bEnableMulti2D)                     //Steven 20200810 : 一個IC使用多個2DID
                            Str.sprintf("LF%s,1,%d", fMain->cbSetupFileName->Text, TestIF_File.iMulti2DCount);
                        else
                            Str.sprintf("LF%s,1", fMain->cbSetupFileName->Text);
                    }
                    else
                    {
                        if(dVisionVer>=2)                                       //wei 20170119 (Steven) 版本號大於2
                            Str.sprintf("LF,0,%s,0", fMain->cbSetupFileName->Text);
                        else if(TestIF_File.bEnableMulti2D)                     //Steven 20200810 : 一個IC使用多個2DID
                            Str.sprintf("LF%s,0,%d", fMain->cbSetupFileName->Text, TestIF_File.iMulti2DCount);
                        else
                            Str.sprintf("LF%s,0", fMain->cbSetupFileName->Text);
                    }
                }
                SendCCDCommand(iBarCode1_1, "Change File", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task=1250;
            case 1250:
                if(bGetLF)
                {
                    Task=2000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=1300;
                }
                break;
            case 1300:
                bGetSE9[iBarCode1_1]=false;
                ClearBuffer(iBarCode1_1, "Clear Buffer when CCD initial");
                ClearBuffer(iBarCode1_2, "Clear Buffer when CCD initial");
                ClearBuffer(iBarCode2_1, "Clear Buffer when CCD initial");
                ClearBuffer(iBarCode2_2, "Clear Buffer when CCD initial");
                Task=2100;
            case 2000:
                if(TestIF_File.bEnableShtFloatChk)
                    Task=2100;
                else
                    Task=3000;
                break;
            case 2100:
                bSFCGetSE9[iBarCode1_1]=false;
                Str.sprintf("E9,1,%d", iSFCTotalMoveStep);

                SendCCDCommand(iBarCode1_1, "Clear buffer", Str);
                SendCCDCommand(iBarCode1_2, "Clear buffer", Str);
                SendCCDCommand(iBarCode2_1, "Clear buffer", Str);
                SendCCDCommand(iBarCode2_2, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task=2150;
                //break;
            case 2150:
                if(bSFCGetSE9[iBarCode1_1])
                    Task=2200;
                else if(tChangeSetupFile.Off())
                    Task=2100;
                break;
            case 2200:                                                          //取得工作檔名稱
                bSFCGetGF=false;
                SendCCDCommand(iBarCode1_1, "Get File Name", "GF,1");
                tChangeSetupFile.SetSecAndOn(10);
                Task=2250;
                //break;
            case 2250:
                if(bSFCGetGF)
                {
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 20210818 add 2D FILENAME  setup
                    {
                        sBuffer=TestIF_File.s2DFileName;
                    }
                    else
                    {
                        sBuffer=fMain->cbSetupFileName->Text;
                    }

                    if(sBuffer.UpperCase().AnsiCompare(sSFCCurrentGF)==0)
                    {
                        Task=3000;
                        bSFCGetLF=true;
                    }
                    else
                    {
                        Task=2300;
                    }
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=2200;
                }
                break;
            case 2300:
                bSFCGetLF=false;
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 20210818 add 2D FILENAME  setup
                {
                    Str.sprintf("LF,1,%s", TestIF_File.s2DFileName);
                }
                else
                {
                    Str.sprintf("LF,1,%s", fMain->cbSetupFileName->Text);
                }

                SendCCDCommand(iBarCode1_1, "SFC Change File",Str);             //kevin 20210818 add 2D FILENAME
                tChangeSetupFile.SetSecAndOn(10);
                Task=2350;
            case 2350:
                if(bSFCGetLF)
                {
                    Task=3000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=2300;
                }
                break;
           /* case 3000:
                tChangeSetupFile.SetSecAndOn(10);                               //Steven 20200324 : 加入心跳功能
                Task=3100;
                break;
            case 3100:
                if(tChangeSetupFile.Off())
                {
                    if(SystemStart==false)
                    {
                        SendCCDCommand(iBarCode1_1, "Heart Beat", "HB");
                        SendCCDCommand(iBarCode1_2, "Heart Beat", "HB");
                        SendCCDCommand(iBarCode2_1, "Heart Beat", "HB");
                        SendCCDCommand(iBarCode1_2, "Heart Beat", "HB");
                    }
                    Task=3000;
                }
                break;*/
        }
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
bool TfBarCode::DoBarcodeCount()                                                //int iBarCode, int Count, int PassCount, int FailCount, int AutoRetry
{
    bool bNeedAlarm=false;
    int  Count1=0;
    int  PassCount1=0;
    int  FailCount1=0;
    double  rate1=0.0, rate=0.0;
    AnsiString str="";
    int  AutoRetry1=0;
    int  Duplicate=0;

    for(int i=0; i<4; i++)
    {
        fLotInfo->sgBarcode->Cells[1+i][1]=iNeedBarcodeCount[i];
        fLotInfo->sgBarcode->Cells[1+i][2]=iBarcodePassCount[i];
        fLotInfo->sgBarcode->Cells[1+i][3]=iBarcodeErrorCount[i];
        fLotInfo->sgBarcode->Cells[1+i][5]=iBarcodeAutoRetry[i];
        fLotInfo->sgBarcode->Cells[1+i][6]=iBarcodeDuplicate[i];
        Count1+=iNeedBarcodeCount[i];
        PassCount1+=iBarcodePassCount[i];
        FailCount1+=iBarcodeErrorCount[i];
        AutoRetry1+=iBarcodeAutoRetry[i];
        Duplicate+=iBarcodeDuplicate[i];

        if(iNeedBarcodeCount[i]!=0)
        {
            rate=double(ChangeToFloatNonPcnt((double)(iBarcodePassCount[i]*100.0), (double)(iNeedBarcodeCount[i])));
        }
        else
        {
            rate=0;
        }

        str.sprintf("%2.2f", rate);
        fLotInfo->sgBarcode->Cells[1+i][4]=str.c_str();

        if(Count1!=0)
        {
            rate1=double(PassCount1*100.0/Count1);
        }
        else
        {
            rate1=0;
        }
        s2DIDYield.sprintf("%2.2f", rate1);

        fLotInfo->sgBarcode->Cells[5][1]=Count1;
        fLotInfo->sgBarcode->Cells[5][2]=PassCount1;
        fLotInfo->sgBarcode->Cells[5][3]=FailCount1;
        fLotInfo->sgBarcode->Cells[5][4]=s2DIDYield.c_str();
        fLotInfo->sgBarcode->Cells[5][5]=AutoRetry1;
        fLotInfo->sgBarcode->Cells[5][6]=Duplicate;
    }

    if(TestIF_File.b2DIDYield)                                                  //Steven 20171222 (Wei) : Yield Alarm of 2DID
    {
        if(Count1>TestIF_File.i2DYieldIgnoreCnt && rate1<TestIF_File.d2DIDYield)//JerryYang 20241104 : Ignore count變更為可以修改
            bNeedAlarm=true;
    }
    return bNeedAlarm;
}
//---------------------------------------------------------------------------
void TfBarCode::AddCCDCommunicationLog(int index, AnsiString Str)               //Steven 20160421 : Add 2D CCD Log
{
    AnsiString Path, Name, Log;

    if(BOTTOM_2DID_CCD==1 && BOTTOM_2DID==1 &&
       TestIF_File.bEnableBottom2D)                                             //KaiChen 20200513 : Bottom 2D 8CCD
    {
        if(index==iBottom_1)
            Name="Bottom_1";
        else if(index==iBottom_2)
            Name="Bottom_2";
        else if(index==iBottom_3)
            Name="Bottom_3";
        else if(index==iBottom_4)
            Name="Bottom_4";
        else if(index==iBottom_5)
            Name="Bottom_5";
        else if(index==iBottom_6)
            Name="Bottom_6";
        else if(index==iBottom_7)
            Name="Bottom_7";
        else
            Name="Bottom_8";
    }
    else if(BOTTOM_2DID &&
            TestIF_File.bEnableBottom2D)                                        //Steven 20190308 : Bottom 2D
    {
        if(index==iBarCode1_1)
            Name="CCD-Aa";
        else if(index==iBarCode1_2)
            Name="CCD-Ab";
        else if(index==iBarCode2_1)
            Name="CCD-Ba";
        else
            Name="CCD-Bb";
    }
    else
    {
        if(index==iBarCode1_1)
            Name="Shuttle1-A";
        else if(index==iBarCode1_2)
            Name="Shuttle1-B";
        else if(index==iBarCode2_1)
            Name="Shuttle2-A";
        else
            Name="Shuttle2-B";
    }
    //Ifor 20211130 add:OCR Map
    if(BAR_CODE_INSTALL==ebcUseOCR &&
      (Str=="LON" || Str==TestIF_File.str2DTriggerONCMD))
    {
        if(index==iBarCode1_1)
            Name="Shuttle1-B";
        else if(index==iBarCode1_2)
            Name="Shuttle1-A";
        else if(index==iBarCode2_1)
            Name="Shuttle2-B";
        else
            Name="Shuttle2-A";
    }

    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d.%03d, %s, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Name, Str);
    if(memoCCDCommLog->Lines->Count>1000)
        memoCCDCommLog->Clear();
    memoCCDCommLog->Lines->Add(Log);

    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeCommLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path, "AddCCDCommunicationLog");

    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeCommLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str() , Log.c_str());
}
//---------------------------------------------------------------------------
bool TfBarCode::DoBarcodeCheck(int iBarCode, AnsiString &Str, int iXItem)       //wei 20160505 Barcode 比對Lot
{
    int iIndex=0,iPos=0;
    AnsiString Str2, Str1, Str3;
    if(TestIF_File.bCheckCodeByLot)
    {
        if(FileExists(asBarCodeLot))                                            //Steven 20160505 : 加上保護, 不然開程式會跳Error
        {
            fBarCode->list2DByLot->Clear();
            fBarCode->list2DByLot->LoadFromFile(asBarCodeLot);
        }

        fBarCode->list2DByLot->Sort();
        iPos=Str.Pos("\r\n");
        if(iPos>0)
            Str1=Str.SubString(1, iPos-1);
        else
            Str1=Str;
        if(fBarCode->list2DByLot->Find(Str1, iIndex))
        {
            bCheckCodeError[iBarCode]=true;
            return false;
        }
    }

    if(TestIF_File.bCheckLotHaveCode)
    {
        if(CosFunction.b2DCodeCheckByCoustomerLot)                              //Sam 20220223 : 2D Code Check by Coustomer Lot
        {
            Str1=fLotInfo->edCustomerLotId->Text;
            iPos=Str1.Pos("\n");
            if(iPos>0)
                Str3=Str1.SubString(1, iPos-1);
            else
                Str3=Str1;

            if(Str3.Length()>8)                                                 //CustomerLotId 最多只取8碼來比對
            {
                Str2=Str.SubString(1, 8);
                Str3=Str3.SubString(1, 8);
            }
            else
            {
                Str2=Str.SubString(1, Str3.Length());
            }

            if(Str2!=Str3)
            {
                bCheckLotError[iBarCode]=true;
                return false;
            }
        }
        else
        {
            FILE *P=NULL;
            char t[256];
            if(FileExists(asBarCodeDownLot)!=false)                             //Alick 20170331 (wei) LotCheckData判斷路徑是否存在
            {
                if(LastSet.iTester==OFF_LINE)                                   //Frank 20170611 (Steven) add Alex 多種Dummy IC 需要同時辨識
                {
                    fBarCode->list2DByLot->Clear();
                    fBarCode->list2DByLot->LoadFromFile(asBarCodeDownLot);
                    fBarCode->list2DByLot->Sort();
                    iPos=Str.Pos("\r\n");
                    if(iPos>0)
                        Str1=Str.SubString(1, iPos-1);
                    else
                        Str1=Str;

                    Str2=Str1.SubString(1, 9);

                    if(!fBarCode->list2DByLot->Find(Str2, iIndex))
                    {
                        bCheckLotError[iBarCode]=true;
                        return false;
                    }
                }
                else
                {
                    P=fopen(asBarCodeDownLot.c_str(),"r");
                    if(P!=NULL)
                    {
                        fgets(t, 30, P);
                        Str1=t;
                        asBarcodeLotNumber="     Lot ID : "+Str1;               //wei Barcode Lot Alarm時，顯示出Lot
                        fclose(P);
                    }

                    iPos=Str1.Pos("\n");                                        //Frank 20170426 (Steven) add 消除"\n"為一個字元的寬度
                    if(iPos>0)
                        Str3=Str1.SubString(1, iPos-1);
                    else
                        Str3=Str1;

                    Str2=Str.SubString(1, Str3.Length());

                    if(Str2!=Str3)
                    {
                        bCheckLotError[iBarCode]=true;
                        return false;
                    }
                }
            }
            else
            {
                ShowErrorMessage("WAR0482", K_RETRY, MMSystem);
                return false;
            }
        }
    }

    if(TestIF_File.bSearch2DIDByLot && LastSet.iTester==OFF_LINE)               //Frank 20170316 (wei) add Search 2DID By Lot
    {
        if(FileExists(asSearch2DIDByLot))
        {
            fBarCode->list2DByLot->Clear();
            fBarCode->list2DByLot->LoadFromFile(asSearch2DIDByLot);
        }
        else
        {
            //待Frank新增
        }

        fBarCode->list2DByLot->Sort();

        iPos=Str.Pos("\r\n");
        if(iPos>0)
            Str1=Str.SubString(1, iPos-1);
        else
            Str1=Str;
        if(fBarCode->list2DByLot->Find(Str1, iIndex)==false)
        {
            Str=asBarCodeErrorSend;
        }
    }

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btBarcodeChangeFileConnectClick(TObject *Sender)
{
    if(ClientSocket_BarcodeChangeFile->Active==false)
    {
        ClientSocket_BarcodeChangeFile->Address=IPAddress1->Text;
        ClientSocket_BarcodeChangeFile->Port=atoi(IPPort1->Text.c_str());
        ClientSocket_BarcodeChangeFile->Open();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btBarcodeChangeFileDisConnectClick(TObject *Sender)
{
    try
    {
        ClientSocket_BarcodeChangeFile->Close();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfBarCode::btBarcodeChangeFileDisConnectClick");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btBarcodeChangeFileSendClick(TObject *Sender)
{
    AnsiString str, str1;

    fBarCode->spbResetCom->Click();
    str.sprintf("%s\r\n", edtCognexSendCmd->Text);
    ClientSocket_BarcodeChangeFile->Socket->SendBuf(str.c_str(),str.Length());
    moBarcodeChangeFile->Lines ->Add(str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::ClientSocket_BarcodeChangeFileConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    moBarcodeChangeFile->Lines ->Add("Connect Port 5000 OK");
    bBarcodeConnect=true;
    fBarCode->InitialBarcodeScanChangeFile();
    TimerBarcodeChangeFile->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::ClientSocket_BarcodeChangeFileDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    moBarcodeChangeFile->Lines ->Add("DisConnect Port 5000 OK");
    bBarcodeConnect=false;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::ClientSocket_BarcodeChangeFileRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int length=Socket->ReceiveLength();
    AnsiString s;
    char temp[1024]={'\0'};

    BYTE* Buff;
    Buff=new BYTE[length];
    Socket->ReceiveBuf(Buff, length);

    memcpy(temp, Buff, length);
    s=temp;

    if(s=="OK\r\n")
    {
        bChangeBarcodeFileOK=true;
    }
//    else
//    {
//        bChangeBarcodeFileOK=false;
//    }

    moBarcodeChangeFile->Lines ->Add(s.c_str());
    delete[] Buff;                                                              //Ifor 20170603 (wei) 刪除Buffer
    Buff=NULL;                                                                  //kevin 20180517
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::TimerBarcodeChangeFileTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    AnsiString str, str1;

    if(bBarcodeConnect)
    {
        if(BAR_CODE_INSTALL!=ebcUseOCR)                                         //Ifor 20210407 add: 自製OCR
            fBarCode->spbResetCom->Click();
        if(DoBarcodeChangeFile())
        {
            bBarcodeConnect=false;
            TimerBarcodeChangeFile->Enabled=false;
        }
    }
}
//------------------------------------------------------------------------------
bool TfBarCode::DoBarcodeChangeFile()                                           //wei 20160728 Barcode File切換
{
    int &Task=iInitialChangeFileTask;
    AnsiString str, str1, str2;
    static AnsiString asErrorPart="",asError="";
    unsigned char str_ch[256]={0};

    switch(Task)
    {
        case 1:
            asErrorPart="";
            asError="";
            Task=100;
            break;
        case 100:
            if(BAR_CODE_INSTALL==ebctEtherNetCCD)
            {
                if(ClientSocket_Shuttle1_A->Active)
                {
                    str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asCCDBarCodeIP[0], fMain->cbSetupFileName->Text);
                }
                else
                {
                    Task=200;
                    break;
                }
                if(FileExists(str1))
                {
                      TMemoryStream *FileList = new TMemoryStream();
                      FileList->LoadFromFile(str1);
                      str2=FileList->Size;
                      str="||>Config.Load " + str2 + "\r\n";

                      strcpy((unsigned char*)str_ch,str.c_str());
                      ClientSocket_Shuttle1_A->Socket->SendBuf(str_ch,str.Length());
                      str="Shuttle1_A Send: "+str;
                      memoCCDCommLog->Lines ->Add(str.c_str());

                      ClientSocket_Shuttle1_A->Socket->SendStream(FileList);
                      str1="Shuttle1_A Send: "+str1;
                      memoCCDCommLog->Lines ->Add(str1.c_str());
                }
                else
                {
                    asErrorPart+="Shuttle1_A ,";
                    Task=200;
                    break;
                }
            }
            else if(BAR_CODE_INSTALL==ebctUseCCDMode)
            {
                str1.sprintf("LF%s,0", fMain->cbSetupFileName->Text);
                memoCCDCommLog->Lines ->Add(str1.c_str());
                SendCCDCommand(iBarCode1_1, "Change File", str1);
            }
            else if(BAR_CODE_INSTALL==ebcUseOCR)                                //Ifor 20210407 add: 自製OCR
            {
                if(ClientSocket_Shuttle1_A->Active)
                {
                    str1.printf("LF%s", fMain->cbSetupFileName->Text);
                    SendCCDCommand(iBarCode1_1, " ", str1);
                    Task=480;
                    break;
                }
                else
                {
                    Task=200;
                    break;
                }
            }
            else
            {
                str.sprintf("%s, %s%s\\%s.cfg\r\n",HSys.asBarCodeComPort[0], sBarcodeFile, HSys.asBarCodeComPort[0], fMain->cbSetupFileName->Text);
                str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asBarCodeComPort[0], fMain->cbSetupFileName->Text);
                if(FileExists(str1))
                {
                    ClientSocket_BarcodeChangeFile->Socket->SendBuf(str.c_str(),str.Length());
                    moBarcodeChangeFile->Lines ->Add(str.c_str());
                }
                else
                {
                    asErrorPart+="COM15 ,";
                    Task=200;
                    break;
                }
            }
            ChangeFileDelay.SetMSAndOn(3000);
            Task=150;
            break;
        case 150:
            if(ChangeFileDelay.Off())
            {
                Task=200;
            }

            if(bChangeBarcodeFileOK)
            {
                bChangeBarcodeFileOK=false;
                Task=200;
            }
            break;
        case 200:
            if(BAR_CODE_INSTALL==ebctEtherNetCCD)
            {
                if(ClientSocket_Shuttle1_B->Active)
                {
                    str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asCCDBarCodeIP[1], fMain->cbSetupFileName->Text);
                }
                else
                {
                    Task=300;
                    break;
                }
                if(FileExists(str1))
                {
                      TMemoryStream *FileList = new TMemoryStream();
                      FileList->LoadFromFile(str1);
                      str2=FileList->Size;
                      str="||>Config.Load " + str2 + "\r\n";

                      strcpy((unsigned char*)str_ch,str.c_str());
                      ClientSocket_Shuttle1_B->Socket->SendBuf(str_ch,str.Length());
                      str="Shuttle1_B Send: "+str;
                      memoCCDCommLog->Lines ->Add(str.c_str());

                      ClientSocket_Shuttle1_B->Socket->SendStream(FileList);
                      str1="Shuttle1_B Send: "+str1;
                      memoCCDCommLog->Lines ->Add(str1.c_str());
                }
                else
                {
                    asErrorPart+="Shuttle1_B ,";
                    Task=300;
                    break;
                }
            }
            else if(BAR_CODE_INSTALL==ebctUseCCDMode)
            {
                str1.sprintf("LF%s,0", fMain->cbSetupFileName->Text);
                memoCCDCommLog->Lines ->Add(str1.c_str());
                SendCCDCommand(iBarCode1_2, "Change File", str1);
            }
            else if(BAR_CODE_INSTALL==ebcUseOCR)                                //Ifor 20210407 add: 自製OCR
            {
                if(ClientSocket_Shuttle1_B->Active)
                {
                    str1.printf("LF%s", fMain->cbSetupFileName->Text);
                    SendCCDCommand(iBarCode1_2, " ", str1);
                    Task=480;
                    break;
                }
                else
                {
                    Task=300;
                    break;
                }
            }
            else
            {
                str.sprintf("%s, %s%s\\%s.cfg\r\n",HSys.asBarCodeComPort[1], sBarcodeFile, HSys.asBarCodeComPort[1], fMain->cbSetupFileName->Text);
                str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asBarCodeComPort[1], fMain->cbSetupFileName->Text);
                if(FileExists(str1))
                {
                    ClientSocket_BarcodeChangeFile->Socket->SendBuf(str.c_str(),str.Length());
                    moBarcodeChangeFile->Lines ->Add(str.c_str());
                }
                else
                {
                    asErrorPart+="COM16 ,";
                    Task=300;
                    break;
                }
            }
            ChangeFileDelay.SetMSAndOn(3000);
            Task=250;
            break;
        case 250:
            if(ChangeFileDelay.Off())
            {
                Task=300;
            }

            if(bChangeBarcodeFileOK)
            {
                bChangeBarcodeFileOK=false;
                Task=300;
            }
            break;
        case 300:
            if(BAR_CODE_INSTALL==ebctEtherNetCCD)
            {
                if(ClientSocket_Shuttle2_A->Active)
                {
                    str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asCCDBarCodeIP[2], fMain->cbSetupFileName->Text);
                }
                else
                {
                    Task=400;
                    break;
                }
                if(FileExists(str1))
                {
                      TMemoryStream *FileList = new TMemoryStream();
                      FileList->LoadFromFile(str1);
                      str2=FileList->Size;
                      str="||>Config.Load " + str2 + "\r\n";

                      strcpy((unsigned char*)str_ch,str.c_str());
                      ClientSocket_Shuttle2_A->Socket->SendBuf(str_ch,str.Length());
                      str="Shuttle2_A Send: "+str;
                      memoCCDCommLog->Lines ->Add(str.c_str());

                      ClientSocket_Shuttle2_A->Socket->SendStream(FileList);
                      str1="Shuttle2_A Send: "+str1;
                      memoCCDCommLog->Lines ->Add(str1.c_str());
                }
                else
                {
                    asErrorPart+="Shuttle2_A ,";
                    Task=400;
                    break;
                }
            }
            else if(BAR_CODE_INSTALL==ebctUseCCDMode)
            {
                str1.sprintf("LF%s,0", fMain->cbSetupFileName->Text);
                memoCCDCommLog->Lines ->Add(str1.c_str());
                SendCCDCommand(iBarCode2_1, "Change File", str1);
            }
            else if(BAR_CODE_INSTALL==ebcUseOCR)                                //Ifor 20210407 add: 自製OCR
            {
                if(ClientSocket_Shuttle2_A->Active)
                {
                    str1.printf("LF%s", fMain->cbSetupFileName->Text);
                    SendCCDCommand(iBarCode2_1, " ", str1);
                    Task=480;
                    break;
                }
                else
                {
                    Task=400;
                    break;
                }
            }
            else
            {
                str.sprintf("%s, %s%s\\%s.cfg\r\n",HSys.asBarCodeComPort[2], sBarcodeFile, HSys.asBarCodeComPort[2], fMain->cbSetupFileName->Text);
                str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asBarCodeComPort[2], fMain->cbSetupFileName->Text);
                if(FileExists(str1))
                {
                    ClientSocket_BarcodeChangeFile->Socket->SendBuf(str.c_str(),str.Length());
                    moBarcodeChangeFile->Lines ->Add(str.c_str());
                }
                else
                {
                    asErrorPart+="COM17 ,";
                    Task=400;
                    break;
                }
            }
            ChangeFileDelay.SetMSAndOn(3000);
            Task=350;
            break;
        case 350:
            if(ChangeFileDelay.Off())
            {
                Task=400;
            }

            if(bChangeBarcodeFileOK)
            {
                bChangeBarcodeFileOK=false;
                Task=400;
            }
            break;
        case 400:
            if(BAR_CODE_INSTALL==ebctEtherNetCCD)
            {
                if(ClientSocket_Shuttle2_B->Active)
                {
                    str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asCCDBarCodeIP[3], fMain->cbSetupFileName->Text);
                }
                else
                {
                    Task=500;
                    break;
                }
                if(FileExists(str1))
                {
                      TMemoryStream *FileList = new TMemoryStream();
                      FileList->LoadFromFile(str1);
                      str2=FileList->Size;
                      str="||>Config.Load " + str2 + "\r\n";

                      strcpy((unsigned char*)str_ch,str.c_str());
                      ClientSocket_Shuttle2_B->Socket->SendBuf(str_ch,str.Length());
                      str="Shuttle2_B Send: "+str;
                      memoCCDCommLog->Lines ->Add(str.c_str());

                      ClientSocket_Shuttle2_B->Socket->SendStream(FileList);
                      str1="Shuttle2_B Send: "+str1;
                      memoCCDCommLog->Lines ->Add(str1.c_str());
                }
                else
                {
                    asErrorPart+="Shuttle2_B ,";
                    Task=500;
                    break;
                }
            }
            else if(BAR_CODE_INSTALL==ebctUseCCDMode)
            {
                str1.sprintf("LF%s,0", fMain->cbSetupFileName->Text);
                memoCCDCommLog->Lines ->Add(str1.c_str());
                SendCCDCommand(iBarCode2_2, "Change File", str1);
            }
            else if(BAR_CODE_INSTALL==ebcUseOCR)                                //Ifor 20210407 add: 自製OCR
            {
                if(ClientSocket_Shuttle2_B->Active)
                {
                    str1.printf("LF%s", fMain->cbSetupFileName->Text);
                    SendCCDCommand(iBarCode2_2, " ", str1);
                    Task=480;
                    break;
                }
                else
                {
                    Task=100;
                    break;
                }
            }
            else
            {
                str.sprintf("%s, %s%s\\%s.cfg\r\n",HSys.asBarCodeComPort[3], sBarcodeFile, HSys.asBarCodeComPort[3], fMain->cbSetupFileName->Text);
                str1.printf("%s%s\\%s.cfg", sBarcodeFile, HSys.asBarCodeComPort[3], fMain->cbSetupFileName->Text);
                if(FileExists(str1))
                {
                    ClientSocket_BarcodeChangeFile->Socket->SendBuf(str.c_str(),str.Length());
                    moBarcodeChangeFile->Lines ->Add(str.c_str());
                }
                else
                {
                    asErrorPart+="COM18 ,";
                    Task=500;
                    break;
                }
            }
            ChangeFileDelay.SetMSAndOn(3000);
            Task=450;
            break;
        case 450:
            if(ChangeFileDelay.Off())
            {
                Task=500;
            }

            if(bChangeBarcodeFileOK)
            {
                bChangeBarcodeFileOK=false;
                Task=500;
            }
            break;
        case 480:
            if(bChangeSetupFile==true)
            {
                if(OCRChangeFileTimeOut.Off())
                {
                    ShowMyMessage("OCR SetupFile Change Fail!", "OCR 工作黨切換失敗!", "DoBarcodeChangeFile");
                    return true;
                }
            }
            else
            {
                return true;
            }
            break;
        case 500:
            if(asErrorPart!="")
                asError.sprintf("Barcode Change File OK \r\n But %s No Setup File", asErrorPart);
            else
                asError.sprintf("Barcode Change File OK");
            ShowMyMessage(asError.c_str());
            fBarCode->btBarcodeChangeFileDisConnect->Click();                   //wei 20160728
            HWND InterfaceProgram_DMCC=FindWindow( NULL , "DMCC Load Config");  //Alick 20160721   主程式關閉後關閉小程式
            if(InterfaceProgram_DMCC!=0)
                PostMessage(InterfaceProgram_DMCC, WM_CLOSE, 0, 0);
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void TfBarCode::CleanBarcodeError(int iStep)                                    //wei Barcode Lot Alarm時，顯示出Lot
{
    if(iStep==1)
    {
        bCheckLotError[iBarCode1_1]=false;
        bCheckLotError[iBarCode1_2]=false;
        bCheckCodeError[iBarCode1_1]=false;
        bCheckCodeError[iBarCode1_2]=false;
    }
    else
    {
        bCheckLotError[iBarCode2_1]=false;
        bCheckLotError[iBarCode2_2]=false;
        bCheckCodeError[iBarCode2_1]=false;
        bCheckCodeError[iBarCode2_2]=false;
    }
}
//------------------------------------------------------------------------------
bool TfBarCode::CCDSFC_Decoding(int Channel)
{
    bool bResult=true;

    AnsiString Data;
    int iResult=-1;

    TStringList *sList=new TStringList();
    sList->CommaText=sDownCCDRecevieData[Channel];

    if(sList->Count>=4)
    {
        iResult=atoi(sList->Strings[2].c_str());
        if(iResult==1)
        {
            Data=sList->Strings[3];
            if(Data.Length()==iSFCTotalMoveStep)
            {
                for(int i=0; i<iSFCTotalMoveStep; i++)
                    iSFCCheckResult[Channel][i]=atoi(Data.SubString(i+1, 1).c_str());
            }
            else
            {
                bResult=false;
            }
        }
        else
        {
            bResult=false;
        }
    }
    else
    {
        bResult=false;
    }
    sList->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sList;
    return bResult;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::edRetryOffsetMoveClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -10, 10);
}
//------------------------------------------------------------------------------
AnsiString TfBarCode::DoBarcodeAnalysis(AnsiString str)                         //wei 20161223 (Steven) Barcode 品質顯示
{
    int iCheckCount=0;

    iCheckCount=str.Pos("(");
    if(iCheckCount==0)
        return str;
    else
        return str.SubString(1, iCheckCount-1);
}
//------------------------------------------------------------------------------
bool CheckAndSaveImage(TMemoryStream *imgbuf0, AnsiString asSaveFailImagePath)
{
    bool result=false;

    TMemoryStream *imgbuf1=new TMemoryStream;
    imgbuf1->Clear();

    unsigned long imgStartPos;
    if(imgbuf0->Size>0)
    {
        imgbuf0->Position = 0;
        int xptr=0;
        unsigned char xbyte[4];
        while(xptr<imgbuf0->Size)
        {
            memset(xbyte,'\0',4);
            imgbuf0->Read(&xbyte,4);
            if(xbyte[0]==0xff && xbyte[1]==0xd8 && xbyte[2]==0xff && xbyte[3]==0xe0)
            {
                imgStartPos= imgbuf0->Position-4;

                // copy image
                imgbuf0->Position=imgStartPos;
                imgbuf1->Position=0;
                imgbuf1->CopyFrom(imgbuf0,imgbuf0->Size-imgStartPos);

                //check image
                if(imgbuf1->Size>0)
                {
                    imgbuf1->SaveToFile(asSaveFailImagePath);
                    result=true;
                }
                break;
            }

            xptr++;
            imgbuf0->Position=xptr;
        }
    }

    return result;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::SaveFailImage(int Pos)
{
    AnsiString Data;
    AnsiString Path;

    Data.sprintf("COM%d_%02d_%02d_%02d.jpg", 15+Pos, SystemHour, SystemMin, SystemSec);
    AnsiString asSaveFailImage=Path.sprintf("%s\\%04d_%02d_%02d\\%s", asBarCodeLogPath, SystemYear, SystemMonth, SystemDate, Data);
    if(bImageSaveflag[Pos]==true)
    {
        if(BarCode_SaveImage[Pos]->Size>0 && CheckAndSaveImage(BarCode_SaveImage[Pos], asSaveFailImage)==true)
        {
            if(FileExists(asSaveFailImage)==true)
            {
                ShellExecute(NULL, NULL, asSaveFailImage.c_str(), NULL, NULL, SW_SHOW);
            }
        }
        else
        {
            BarCode_SaveImage[Pos]->SaveToFile(ChangeFileExt(asSaveFailImage, ".dump"));
        }

        BarCode_SaveImage[Pos]->Clear();
        bImageSaveflag[Pos]=false;
    }
    else
    {
        //show dump data to memo
        char xbyte;
        unsigned long xptr=0;
        BarCode_SaveImage[Pos]->Position=0;

        while(xptr<BarCode_SaveImage[Pos]->Size)
        {
            BarCode_SaveImage[Pos]->Read(&xbyte, 1);
            xptr++;
            BarCode_SaveImage[Pos]->Position=xptr;
        }

        BarCode_SaveImage[Pos]->Clear();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btSaveImageClick(TObject *Sender)
{
    for(int i=0; i<4; i++)
    {
        if(bImageSaveflag[i]==true)
        {
            AnsiString cmd="||>image.send\r\n";
            BarcodeCOM[i]->WriteCommData(cmd.c_str(),cmd.Length());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::BitBtn1Click(TObject *Sender)
{
    AnsiString cmd="||>trigger on\r\n";
    BarcodeCOM[1]->WriteCommData(cmd.c_str(),cmd.Length());
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::bt2DIDOffsetClick(TObject *Sender)
{
    fOffSet->Show();
    fOffSet->btnToIndexOffset->Click();
    fOffSet->IndexOffSetBT1->Click();
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btStart2DIDCheckSh1Click(TObject *Sender)
{
    i2DIDCheckTask=100;
    btStart2DIDCheckSh1->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btStart2DIDCheckSh2Click(TObject *Sender)
{
    i2DIDCheckTask=200;
    btStart2DIDCheckSh2->Enabled=false;
}
//------------------------------------------------------------------------------
void TfBarCode::Do2DIDCheck()
{
    int &Task=i2DIDCheckTask;
    switch(Task)
    {
        case 100:
            iInitialBarcodeInShuttle1Task=1;
            if(CUSTOMER_CODE==CC_ASE_SG)                                        //Ifor 20251105 add:ASE SG 要求使用舊版本2DIDCheck流程
                Task=160;
            else
                Task=110;
            break;
        case 110:
            if(DoBarcodeCCDInShuttle_1(true))
            {
                ShowMyMessage("已完成shuttle 1 測試");
                btStart2DIDCheckSh1->Enabled=true;
                btStart2DIDCheckSh1->Down=false;
            }
            break;
        case 160:
            if(Do2DIDCheckSh1())
            {
                ShowMyMessage("已完成shuttle 1 測試");
                btStart2DIDCheckSh1->Enabled=true;
                btStart2DIDCheckSh1->Down=false;
            }
            break;
        case 200:
            iInitialBarcodeInShuttle2Task=1;
            if(CUSTOMER_CODE==CC_ASE_SG)                                        //Ifor 20251105 add:ASE SG 要求使用舊版本2DIDCheck流程
                Task=260;
            else
                Task=220;
            break;
        case 220:
            if(DoBarcodeCCDInShuttle_2(true))
            {
                ShowMyMessage("已完成shuttle 2 測試");
                btStart2DIDCheckSh2->Enabled=true;
                btStart2DIDCheckSh2->Down=false;
            }
            break;
        case 260:
            if(Do2DIDCheckSh2())
            {
                ShowMyMessage("已完成shuttle 2 測試");
                btStart2DIDCheckSh2->Enabled=true;
                btStart2DIDCheckSh2->Down=false;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::edt2DIDYieldClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 100.0);
}
//---------------------------------------------------------------------------
bool TfBarCode::DoCheckSHT2DIDStatus(int SHT)                                   //Isacc 20180329 (Steven) 2DID All site fail recheck twice and alarm,check Shuttle IC
{
    bool bResult=true;

    if(TestIF_File.iEnableAllSite2DIDErr==0)                                    //Steven 20200909 : 將2DID all site fail變成選項
    {
        bResult=false;
    }
    else if(SHT==0)
    {
        if(FLCarryKit.HasRealIC())
        {
            #ifdef SOFT_SIMULTE                                                 //Isaac 20200206 : 模擬2DID all fail
            if(cb2DIDAllfailSimulate->Checked==true)
            {
                return true;
            }
            #endif
            for(int i=0; i<FLCarryKit.iShtRow; i++)
            {
                for(int j=0; j<FLCarryKit.iShtCol; j++)
                {
                    if(FLCarryKit.Item[i][j]==HAS_IC ||
                       FLCarryKit.Item[i][j]==HAS_HOT_IC)
                    {
                        if(FLCarryKit.cDeviceInf[i][j]==""  ||
                           FLCarryKit.cDeviceInf[i][j]=="0" ||
                           FLCarryKit.cDeviceInf[i][j]==asBarCodeErrorSend ||
                           FLCarryKit.cDeviceInf[i][j]=="ERROR" ||
                           FLCarryKit.cDeviceInf[i][j]==asBarCodeErrorCheckSum)
                        {
                        }
                        else
                        {
                            bResult=false;
                        }
                    }
                }
            }
        }
        else
        {
            bResult=false;
        }
    }
    else if(SHT==1)
    {
        if(BLCarryKit.HasRealIC())
        {
            #ifdef SOFT_SIMULTE                                                 //Isaac 20200206 : 模擬2DID all fail
            if(cb2DIDAllfailSimulate->Checked==true)
            {
                return true;
            }
            #endif
            for(int i=0; i<BLCarryKit.iShtRow; i++)
            {
                for(int j=0; j<BLCarryKit.iShtCol; j++)
                {
                    if(BLCarryKit.Item[i][j]==HAS_IC || BLCarryKit.Item[i][j]==HAS_HOT_IC)
                    {
                        if(BLCarryKit.cDeviceInf[i][j]==""  ||
                           BLCarryKit.cDeviceInf[i][j]=="0" ||
                           BLCarryKit.cDeviceInf[i][j]==asBarCodeErrorSend ||
                           BLCarryKit.cDeviceInf[i][j]=="ERROR" ||
                           BLCarryKit.cDeviceInf[i][j]==asBarCodeErrorCheckSum)
                        {
                        }
                        else
                        {
                            bResult=false;
                        }
                    }
                }
            }
        }
        else
        {
            bResult=false;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
void TfBarCode::InitBottom2DIDScan(bool bClear2DID)                             //Steven 20190308 : Bottom 2D
{
    iBottom2DIDTask=1;
    iBottom2DID_8CCDTask=1;                                                     //KaiChen 20200513 : Bottom 2D 8CCD
    AnsiString Log;
    iShtDupilcateFailCnt[0]=false;
    ZeroMemory(bNeedScanBottom2D, sizeof(bNeedScanBottom2D));

    if(bClear2DID)
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(InArmSuck.Item[i][j]!=NULL_IC &&
                   InArmSuck.Item[i][j]!=HAS_NULL_IC &&
                   InArmSuck.cDeviceInf[i][j]!="")
                {
                    map2DIter=map2DList.find(InArmSuck.cDeviceInf[i][j]);
                    if(map2DList.size()!=0 && map2DIter!=map2DList.end())
                    {
                        Log.sprintf("Delete, In Arm %d_%d, Code, %s", i+1, j+1, InArmSuck.cDeviceInf[i][j]);
                        if(i==0)
                        {
                            if(j%2==0)
                                AddCCDCommunicationLog(iBarCode1_1, Log);
                            else
                                AddCCDCommunicationLog(iBarCode1_2, Log);
                        }
                        else
                        {
                            if(j%2==0)
                                AddCCDCommunicationLog(iBarCode2_1, Log);
                            else
                                AddCCDCommunicationLog(iBarCode2_2, Log);
                        }

                        map2DList.erase(map2DIter);
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
bool TfBarCode::MoveInArmXY_ToBottom2DID()
{
    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20111226 : In Arm 咖啡杯解除
    {
        if(fNote->bMyServoOffInArm)
        {
            return false;
        }
    }

    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =12000;                                                     //使用iXpitchMaxX3會在 HT-9132LS遇到問題
    int iMovePitchY =(TestIF_File.dSiteYPitch==6350)?6350:TestIF.iARM_Y_PITCH;
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    if(iBottomKit==0)                                                           // Z1 & Z3
    {
        iXPos   =Tech.iBottom2DIDInX+TestIF_File.dBottom2DOffsetX*100;
        iYPos   =Tech.iBottom2DIDInY+TestIF_File.dBottom2DOffsetY*100;
    }
    else // iKit = 1                                                            // Z2 & Z4
    {
        iXPos   =Tech.iBottom2DIDInX+TestIF_File.dBottom2DOffsetX*100-4000;
        iYPos   =Tech.iBottom2DIDInY+TestIF_File.dBottom2DOffsetY*100;
    }

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)                         //jou 2010-12-20 Pitch & Z 縮減為一個
    {
        iOffsetPos=InOfsLoader;
    }
    else
    {
        /*
        if(iRotate_In_Index==1)
            iOffsetPos=InOfsHP2;
        else
            iOffsetPos=InOfsHP1;
        */
        iOffsetPos=InOfsBottom2DID;                                             //Sam 20240808 : 修正 Pitch Offset
    }

    iYVariable=GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    for(int i=0; i<InArmSuck.iMaxRow; i++)                                      //KaiChen 20200428 ：新增Bottom2DID Z軸 OffSet 移動
    {
        for(int j=0; j<InArmSuck.iMaxCol; j++)
        {
            iZPos[i][j]=10+InArmOffSet[InOfsBottom2DID]->GetPlace()+InArmOffSet[InOfsBottom2DID]->GetPlace(i, j);
            if(InArmSuck.Item[i][j]!=HAS_NULL_IC && InArmSuck.Item[i][j]!=NULL_IC)
            {
                if(iZPos[i][j]!=10)
                {
                    bZFlag[i][j]=true;
                }
            }
            else
            {
                bZFlag[i][j]=false;
            }
        }
    }

    if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, ZAxisDown, false))
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfBarCode::CheckWhichKitBottom2DID()
{
    if(BOTTOM_2DID_CCD==1 && BOTTOM_2DID==1 && TestIF_File.bEnableBottom2D)     //KaiChen 20200513 : Bottom 2D 8CCD
    {
        bCCDBarcodeExposureOK[0]=!(InArmSuck.Item[0][0]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC && InArmSuck.cDeviceInf[0][0]=="" && bNeedScanBottom2D[0][0]==true);
        bCCDBarcodeExposureOK[1]=!(InArmSuck.Item[1][0]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC && InArmSuck.cDeviceInf[1][0]=="" && bNeedScanBottom2D[1][0]==true);
        bCCDBarcodeExposureOK[2]=!(InArmSuck.Item[0][1]!=HAS_NULL_IC && InArmSuck.Item[0][1]!=NULL_IC && InArmSuck.cDeviceInf[0][1]=="" && bNeedScanBottom2D[0][1]==true);
        bCCDBarcodeExposureOK[3]=!(InArmSuck.Item[1][1]!=HAS_NULL_IC && InArmSuck.Item[1][1]!=NULL_IC && InArmSuck.cDeviceInf[1][1]=="" && bNeedScanBottom2D[1][1]==true);
        bCCDBarcodeExposureOK[4]=!(InArmSuck.Item[0][2]!=HAS_NULL_IC && InArmSuck.Item[0][2]!=NULL_IC && InArmSuck.cDeviceInf[0][2]=="" && bNeedScanBottom2D[0][2]==true);
        bCCDBarcodeExposureOK[5]=!(InArmSuck.Item[1][2]!=HAS_NULL_IC && InArmSuck.Item[1][2]!=NULL_IC && InArmSuck.cDeviceInf[1][2]=="" && bNeedScanBottom2D[1][2]==true);
        bCCDBarcodeExposureOK[6]=!(InArmSuck.Item[0][3]!=HAS_NULL_IC && InArmSuck.Item[0][3]!=NULL_IC && InArmSuck.cDeviceInf[0][3]=="" && bNeedScanBottom2D[0][3]==true);
        bCCDBarcodeExposureOK[7]=!(InArmSuck.Item[1][3]!=HAS_NULL_IC && InArmSuck.Item[1][3]!=NULL_IC && InArmSuck.cDeviceInf[1][3]=="" && bNeedScanBottom2D[1][3]==true);

        bBottom2DNeedMoveInArm=(bCCDBarcodeExposureOK[0]==false || bCCDBarcodeExposureOK[1]==false || bCCDBarcodeExposureOK[2]==false || bCCDBarcodeExposureOK[3]==false ||
                                bCCDBarcodeExposureOK[4]==false || bCCDBarcodeExposureOK[5]==false || bCCDBarcodeExposureOK[6]==false || bCCDBarcodeExposureOK[7]==false);
    }
    else
    {
        if(iBottomKit==0)
        {
            bCCDBarcodeExposureOK[0]=!(InArmSuck.Item[0][0]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC && InArmSuck.cDeviceInf[0][0]=="" && bNeedScanBottom2D[0][0]==true);
            bCCDBarcodeExposureOK[1]=!(InArmSuck.Item[0][2]!=HAS_NULL_IC && InArmSuck.Item[0][2]!=NULL_IC && InArmSuck.cDeviceInf[0][2]=="" && bNeedScanBottom2D[0][2]==true);
            bCCDBarcodeExposureOK[2]=!(InArmSuck.Item[1][0]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC && InArmSuck.cDeviceInf[1][0]=="" && bNeedScanBottom2D[1][0]==true);
            bCCDBarcodeExposureOK[3]=!(InArmSuck.Item[1][2]!=HAS_NULL_IC && InArmSuck.Item[1][2]!=NULL_IC && InArmSuck.cDeviceInf[1][2]=="" && bNeedScanBottom2D[1][2]==true);
        }
        else if(iBottomKit==1)
        {
            bCCDBarcodeExposureOK[0]=!(InArmSuck.Item[0][1]!=HAS_NULL_IC && InArmSuck.Item[0][1]!=NULL_IC && InArmSuck.cDeviceInf[0][1]=="" && bNeedScanBottom2D[0][1]==true);
            bCCDBarcodeExposureOK[1]=!(InArmSuck.Item[0][3]!=HAS_NULL_IC && InArmSuck.Item[0][3]!=NULL_IC && InArmSuck.cDeviceInf[0][3]=="" && bNeedScanBottom2D[0][3]==true);
            bCCDBarcodeExposureOK[2]=!(InArmSuck.Item[1][1]!=HAS_NULL_IC && InArmSuck.Item[1][1]!=NULL_IC && InArmSuck.cDeviceInf[1][1]=="" && bNeedScanBottom2D[1][1]==true);
            bCCDBarcodeExposureOK[3]=!(InArmSuck.Item[1][3]!=HAS_NULL_IC && InArmSuck.Item[1][3]!=NULL_IC && InArmSuck.cDeviceInf[1][3]=="" && bNeedScanBottom2D[1][3]==true);
        }
        else
        {
            bCCDBarcodeExposureOK[0]=true;
            bCCDBarcodeExposureOK[1]=true;
            bCCDBarcodeExposureOK[2]=true;
            bCCDBarcodeExposureOK[3]=true;
        }
        bBottom2DNeedMoveInArm=(bCCDBarcodeExposureOK[0]==false || bCCDBarcodeExposureOK[1]==false || bCCDBarcodeExposureOK[2]==false || bCCDBarcodeExposureOK[3]==false);
    }
}
//---------------------------------------------------------------------------
bool TfBarCode::DoBottom2DIDScan()
{
    int &Task=iBottom2DIDTask;
    int ret, iPos=0, l, m;
    int iIndex;
    static bool bHasErr[2][4];
    static int iRetryCount=0, iStepCount;
    static int iRetryCountExpTimo=0;                                            //Steven 20160425 : 修改拍照Time Out
    static bool bHasError=false;
    static bool bLotCheckError=false;                                           //Alick 20170313 (wei) add
    static bool bHasDupError=false;
    static bool bLotHasDupError=false;
    static int iClearBuffRetry=0;                                               //JerryYang 20250506 : bottom 2DID 增加Retry
    static AnsiString ErrPart;
    static bool bNeesScanCode[2][4];
    static AnsiString sErrorPart="", sSimuCode[4];
    AnsiString cLastString, sCCDCommand;
    AnsiString Log;
    AnsiString Str, Str1, Str2, Str3;

    switch(Task)
    {
        case 1:                                                                 //jou 20170509 (Steven) : 2D alway retry
            iRetryCount=0;
            iStepCount=0;
            iBottomKit=-1;
            iClearBuffRetry=0;
            bBottom2DNeedMoveInArm=false;
            ZeroMemory(bNeesScanCode, sizeof(bNeesScanCode));
            if(InArmSuck.HasRealIC()==false)
                return true;

            AddCCDCommunicationLog(iBarCode1_1, "<<Cycle, Cycle start.------------------");
            Task=100;
        case 100:
            iBottomKit=0;
            Log="case 100: bNeedScanBottom2D ";
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Item[i][j]!=HAS_NULL_IC && InArmSuck.Item[i][j]!=NULL_IC)
                        bNeedScanBottom2D[i][j]=true;
                    else
                        bNeedScanBottom2D[i][j]=false;

                    mtBarcodeInSh->SetCellNumber(j, i+1, "");
                    InArmSuck.cDeviceInf[i][j]="";

                    Str.sprintf("[%d][%d]=%s,", i, j, bNeedScanBottom2D[i][j]?AnsiString("True"):AnsiString("False"));
                    Log+=Str;
                }
            }
            AddCCDCommunicationLog(iBarCode1_1, Log);
            Task=200;
        case 200:
            CheckWhichKitBottom2DID();

            Log.sprintf("case 200: bCCDBarcodeExposureOK [0]:%s, [1]:%s, [2]:%s, [3]:%s, iBottomKit:%d",
                            bCCDBarcodeExposureOK[0]?AnsiString("True"):AnsiString("False"),
                            bCCDBarcodeExposureOK[1]?AnsiString("True"):AnsiString("False"),
                            bCCDBarcodeExposureOK[2]?AnsiString("True"):AnsiString("False"),
                            bCCDBarcodeExposureOK[3]?AnsiString("True"):AnsiString("False"),
                            iBottomKit);
            AddCCDCommunicationLog(iBarCode1_1, Log);
            if(iBottomKit==-1)
                return true;

            for(int i=0; i<BAR_CODE_COUNT; i++)
            {
                bStepError[i]=false;                                            //Steven 20160503 : 確認移動位置與拍照位置相同
//                bCCDBarcodeExposureOK[i]=true;                                //RogerYang 20190319 Mark
                bCCDBarcodeGetResultOK[i]=true;
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                {
                    sSimuCode[i]="GV,0,1,1,";
                }
                else
                {
                    sSimuCode[i]="GV,1,1,";
                }

                bGetSE9[i]=false;

                ClearBuffer(i, "Clear Buffer when bottom 2D scan");
            }

            if(bBottom2DNeedMoveInArm)
                MoveInArmXY_ToBottom2DID();
            Bottom2DPosDelay.SetMSAndOn(5000);
            Task=500;
        case 500:
            if(bGetSE9[iBarCode1_1] && bGetSE9[iBarCode1_2] && bGetSE9[iBarCode2_1] && bGetSE9[iBarCode2_2])
            {
                Task=1000;
                iClearBuffRetry=0;
            }
            #ifdef SOFT_SIMULTE
            else if(cbBarCodeSimulate->Checked)
            {
                Task=1000;
                iClearBuffRetry=0;
            }
            #endif
            else if(Bottom2DPosDelay.Off())
            {
                if(iClearBuffRetry>=3)                                          //JerryYang 20250506 : bottom 2DID 增加Retry
                {
                    ShowErrorMessage("WAR0462", K_RETRY, MInArmX, false, "Clear buffer");
                    Task=1;
                }
                else                                                            //Steven 20240821 : Retry一次
                {
                    bGetSE9[iBarCode1_1]=false;
                    bGetSE9[iBarCode1_2]=false;
                    bGetSE9[iBarCode2_1]=false;
                    bGetSE9[iBarCode2_2]=false;
                    iClearBuffRetry++;
                    for(int i=0; i<BAR_CODE_COUNT; i++)
                    {
                        ClearBuffer(i, "Clear Buffer when bottom 2D scan");
                    }
                    Bottom2DPosDelay.SetMSAndOn(5000);
                }
            }
            break;
        case 1000:
            iCurrentStep[iBarCode1_1]=iBottomKit;                               //Richard 20230424 : 確認iPosition跟bottom 2D位置一致
            iCurrentStep[iBarCode1_2]=iBottomKit;
            iCurrentStep[iBarCode2_1]=iBottomKit;
            iCurrentStep[iBarCode2_2]=iBottomKit;
            if(bBottom2DNeedMoveInArm)
            {
                ret=MoveInArmXY_ToBottom2DID();
            }
            else
            {
                ret=true;
            }

            if(ret)
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   Sen[SnRKManualTStart].IsOn())
                {
                    Task=1050;
                    fMain->Pause("DoBottom2DIDScan 1000");
                    break;
                }

                Log.sprintf("Action, In arm move to kit %d", iBottomKit);
                AddCCDCommunicationLog(iBarCode1_1, Log);

                Bottom2DPosDelay.SetMSAndOn(TestIF_File.iBarCodePosDelay);
                ZeroMemory(bStepError, sizeof(bStepError));
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out
                Task=1120;
            }
            break;
        case 1050:
            Task=1000;
            break;
        //RogerYang 20190319
        //==>
        case 1120:
            if(Bottom2DPosDelay.Off())                                          //到位Delay
            {
                Log.sprintf("case 1120: bCCDBarcodeExposureOK [0]:%s, [1]:%s, [2]:%s, [3]:%s, iBottomKit:%d",
                                bCCDBarcodeExposureOK[0]?AnsiString("True"):AnsiString("False"),
                                bCCDBarcodeExposureOK[1]?AnsiString("True"):AnsiString("False"),
                                bCCDBarcodeExposureOK[2]?AnsiString("True"):AnsiString("False"),
                                bCCDBarcodeExposureOK[3]?AnsiString("True"):AnsiString("False"),
                                iBottomKit);
                AddCCDCommunicationLog(iBarCode1_1, Log);
//                CheckWhichKitBottom2DID();
//                Log.sprintf("case 1120: bCCDBarcodeExposureOK [0]:%s, [1]:%s, [2]:%s, [3]:%s, iBottomKit:%d",
//                                bCCDBarcodeExposureOK[0]?AnsiString("True"):AnsiString("False"),
//                                bCCDBarcodeExposureOK[1]?AnsiString("True"):AnsiString("False"),
//                                bCCDBarcodeExposureOK[2]?AnsiString("True"):AnsiString("False"),
//                                bCCDBarcodeExposureOK[3]?AnsiString("True"):AnsiString("False"),
//                                iBottomKit);
//                AddCCDCommunicationLog(iBarCode1_1, Log);

                if(bCCDBarcodeExposureOK[0]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                    mtBarcodeInSh->SetCellNumber(0+iBottomKit, 1, "Exposure...");
                }
                else
                {
                    bCCDBarcodeExposureOK[0]=false;                             //Steven 20200619 : 修正, Kit=0全關時, 會出現秒差會導致Kit=1的不送訊號
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", iBottomKit);
                }
                SendCCDCommand(iBarCode1_1, "exposure 1100: ", sCCDCommand);

                if(bCCDBarcodeExposureOK[1]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                    mtBarcodeInSh->SetCellNumber(2+iBottomKit, 1, "Exposure...");
                }
                else
                {
                    bCCDBarcodeExposureOK[1]=false;                             //Steven 20200619 : 修正, Kit=0全關時, 會出現秒差會導致Kit=1的不送訊號
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", iBottomKit);
                }
                SendCCDCommand(iBarCode1_2, "exposure 1100: ", sCCDCommand);

                if(bCCDBarcodeExposureOK[2]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                    mtBarcodeInSh->SetCellNumber(0+iBottomKit, 2, "Exposure...");
                }
                else
                {
                    bCCDBarcodeExposureOK[2]=false;                             //Steven 20200619 : 修正, Kit=0全關時, 會出現秒差會導致Kit=1的不送訊號
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", iBottomKit);
                }
                SendCCDCommand(iBarCode2_1, "exposure 1100: ", sCCDCommand);

                if(bCCDBarcodeExposureOK[3]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                    mtBarcodeInSh->SetCellNumber(2+iBottomKit, 2, "Exposure...");
                }
                else
                {
                    bCCDBarcodeExposureOK[3]=false;                             //Steven 20200619 : 修正, Kit=0全關時, 會出現秒差會導致Kit=1的不送訊號
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", iBottomKit);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", iBottomKit);
                }
                SendCCDCommand(iBarCode2_2, "exposure 1100: ", sCCDCommand);

                Task=1150;
                Bottom2DExposureDelay.SetMSAndOn(TestIF_File.i2DTriggerTime);
            }
            //<==
            //RogerYang 20190319
            break;
        case 1150:
            if(bStepError[iBarCode1_1]==true || bStepError[iBarCode1_2]==true ||
               bStepError[iBarCode2_1]==true || bStepError[iBarCode2_2]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart="";
                if(bStepError[iBarCode1_1])
                    ErrPart+=OutArmSuck.Suck[0][0+iBottomKit].sName;

                if(bStepError[iBarCode1_2])
                    ErrPart+=OutArmSuck.Suck[0][2+iBottomKit].sName;

                if(bStepError[iBarCode2_1])
                    ErrPart+=OutArmSuck.Suck[1][0+iBottomKit].sName;

                if(bStepError[iBarCode2_2])
                    ErrPart+=OutArmSuck.Suck[1][2+iBottomKit].sName;
                Task=1180;
            }
            else if(bCCDBarcodeExposureOK[iBarCode1_1]==true && bCCDBarcodeExposureOK[iBarCode1_2]==true &&     //2091319 RogerYang iBarCode1_2沒寫到
                    bCCDBarcodeExposureOK[iBarCode2_1]==true && bCCDBarcodeExposureOK[iBarCode2_2]==true)
            {
                Task=1200;
                bNeedScanBottom2D[0][0+iBottomKit]=!bCCDBarcodeExposureOK[iBarCode1_1];
                bNeedScanBottom2D[0][2+iBottomKit]=!bCCDBarcodeExposureOK[iBarCode1_2];
                bNeedScanBottom2D[1][0+iBottomKit]=!bCCDBarcodeExposureOK[iBarCode2_1];
                bNeedScanBottom2D[1][2+iBottomKit]=!bCCDBarcodeExposureOK[iBarCode2_2];

                if(InArmSuck.Item[0][0+iBottomKit]!=HAS_NULL_IC && InArmSuck.Item[0][0+iBottomKit]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(0+iBottomKit, 1, "Waiting result..");
                if(InArmSuck.Item[0][2+iBottomKit]!=HAS_NULL_IC && InArmSuck.Item[0][2+iBottomKit]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(2+iBottomKit, 1, "Waiting result..");
                if(InArmSuck.Item[1][0+iBottomKit]!=HAS_NULL_IC && InArmSuck.Item[1][0+iBottomKit]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(0+iBottomKit, 2, "Waiting result..");
                if(InArmSuck.Item[1][2+iBottomKit]!=HAS_NULL_IC && InArmSuck.Item[1][2+iBottomKit]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(2+iBottomKit, 2, "Waiting result..");
            }
            #ifdef SOFT_SIMULTE
            else if(cbBarCodeSimulate->Checked)
            {
                sSimuCode[iBarCode1_1]+=GetSimuBarCodeName(iBarCode1_1, iBottomKit)+",";
                sSimuCode[iBarCode1_2]+=GetSimuBarCodeName(iBarCode1_2, iBottomKit)+",";
                sSimuCode[iBarCode2_1]+=GetSimuBarCodeName(iBarCode2_1, iBottomKit)+",";
                sSimuCode[iBarCode2_2]+=GetSimuBarCodeName(iBarCode2_2, iBottomKit)+",";
                Task=1200;
            }
            #endif
            else if(Bottom2DExposureDelay.Off())                                //拍照time out
            {
                ErrPart="";
                if(bCCDBarcodeExposureOK[iBarCode1_1]==false)
                {
                    mtBarcodeInSh->SetCellNumber(0+iBottomKit, 1, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBarCode1_1, Log);
                    ErrPart+=OutArmSuck.Suck[0][0+iBottomKit].sName;
                }

                if(bCCDBarcodeExposureOK[iBarCode1_2]==false)
                {
                    mtBarcodeInSh->SetCellNumber(2+iBottomKit, 1, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBarCode1_2, Log);
                    ErrPart+=OutArmSuck.Suck[0][2+iBottomKit].sName;
                }

                if(bCCDBarcodeExposureOK[iBarCode2_1]==false)
                {
                    mtBarcodeInSh->SetCellNumber(0+iBottomKit, 2, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBarCode2_1, Log);
                    ErrPart+=OutArmSuck.Suck[1][0+iBottomKit].sName;
                }

                if(bCCDBarcodeExposureOK[iBarCode2_2]==false)
                {
                    mtBarcodeInSh->SetCellNumber(2+iBottomKit, 2, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBarCode2_2, Log);
                    ErrPart+=OutArmSuck.Suck[1][2+iBottomKit].sName;
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }
            break;
        case 1160:
            if(iRetryCountExpTimo<3)
            {
                Task=1170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MInArmX, false, ErrPart);  //2D function CCD 1 or 2 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    Task=1120;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            if(bCCDBarcodeExposureOK[0]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                else
                    sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                mtBarcodeInSh->SetCellNumber(0+iBottomKit, 1, "Exposure...");
                SendCCDCommand(iBarCode1_1, "exposure 1100: ", sCCDCommand);
            }

            if(bCCDBarcodeExposureOK[1]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                else
                    sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                mtBarcodeInSh->SetCellNumber(2+iBottomKit, 1, "Exposure...");
                SendCCDCommand(iBarCode1_2, "exposure 1100: ", sCCDCommand);
            }

            if(bCCDBarcodeExposureOK[2]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                else
                    sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                mtBarcodeInSh->SetCellNumber(0+iBottomKit, 2, "Exposure...");
                SendCCDCommand(iBarCode2_1, "exposure 1100: ", sCCDCommand);
            }

            if(bCCDBarcodeExposureOK[3]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", iBottomKit);
                else
                    sCCDCommand.sprintf("SE0,1,%d", iBottomKit);
                mtBarcodeInSh->SetCellNumber(2+iBottomKit, 2, "Exposure...");
                SendCCDCommand(iBarCode2_2, "exposure 1100: ", sCCDCommand);
            }

            Task=1150;
            Bottom2DExposureDelay.SetMSAndOn(TestIF_File.i2DTriggerTime);
            break;
        case 1180:
            Log.sprintf("Alarm, In arm exposure position error!");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            ShowErrorMessage("WAR0469", K_RETRY, MInArmX, false, ErrPart);      //In shuttle 1 exposure position error!
            Task=1;
            break;
        case 1200:
            iBottomKit++;
            CheckWhichKitBottom2DID();

            Log.sprintf("case 1200: bCCDBarcodeExposureOK [0]:%s, [1]:%s, [2]:%s, [3]:%s, iBottomKit:%d",
                            bCCDBarcodeExposureOK[0]?AnsiString("True"):AnsiString("False"),
                            bCCDBarcodeExposureOK[1]?AnsiString("True"):AnsiString("False"),
                            bCCDBarcodeExposureOK[2]?AnsiString("True"):AnsiString("False"),
                            bCCDBarcodeExposureOK[3]?AnsiString("True"):AnsiString("False"),
                            iBottomKit);
            AddCCDCommunicationLog(iBarCode1_1, Log);

            if(iBottomKit==0 || iBottomKit==1)
            {
                Task=1000;
            }
            else
            {
                Task=1300;
            }
            break;
        case 1300:
            if(dVisionVer>=2)                                                   //wei 20170119 (Steven) 版本號大於2
                sCCDCommand="GV,0";
            else
                sCCDCommand="GV0";                                              //Ifor 20151228 取得 CCD 讀取資料
            ZeroMemory(bCCDBarcodeGetResultOK, sizeof(bCCDBarcodeGetResultOK));
            if((InArmSuck.Item[0][0]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC) ||
               (InArmSuck.Item[0][1]!=HAS_NULL_IC && InArmSuck.Item[0][1]!=NULL_IC))
            {
                SendCCDCommand(iBarCode1_1, "get result 1130: ", sCCDCommand);
                bCCDBarcodeGetResultOK[iBarCode1_1]=false;
                if(TestIF_File.i2DIDFormat==eAMD)                               //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(0, 1, "");
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(0, 2, "");
                }
            }
            else
            {
                bCCDBarcodeGetResultOK[iBarCode1_1]=true;
            }

            if((InArmSuck.Item[0][2]!=HAS_NULL_IC && InArmSuck.Item[0][2]!=NULL_IC) ||
               (InArmSuck.Item[0][3]!=HAS_NULL_IC && InArmSuck.Item[0][3]!=NULL_IC))
            {
                SendCCDCommand(iBarCode1_2, "get result 1130: ", sCCDCommand);
                bCCDBarcodeGetResultOK[iBarCode1_2]=false;
            }
            else
            {
                bCCDBarcodeGetResultOK[iBarCode1_2]=true;
            }

            if((InArmSuck.Item[1][0]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC) ||
               (InArmSuck.Item[1][1]!=HAS_NULL_IC && InArmSuck.Item[1][1]!=NULL_IC))
            {
                SendCCDCommand(iBarCode2_1, "get result 1130: ", sCCDCommand);
                bCCDBarcodeGetResultOK[iBarCode2_1]=false;
                if(TestIF_File.i2DIDFormat==eAMD)                               //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, 1, "");
                    fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(1, 2, "");
                }
            }
            else
            {
                bCCDBarcodeGetResultOK[iBarCode2_1]=true;
            }

            if((InArmSuck.Item[1][2]!=HAS_NULL_IC && InArmSuck.Item[1][2]!=NULL_IC) ||
               (InArmSuck.Item[1][3]!=HAS_NULL_IC && InArmSuck.Item[1][3]!=NULL_IC))
            {
                SendCCDCommand(iBarCode2_2, "get result 1130: ", sCCDCommand);
                bCCDBarcodeGetResultOK[iBarCode2_2]=false;
            }
            else
            {
                bCCDBarcodeGetResultOK[iBarCode2_2]=true;
            }

            #ifdef SOFT_SIMULTE
            if(cbBarCodeSimulate->Checked)
            {
                Socket2DReceiveList[iBarCode1_1]->Add(sSimuCode[iBarCode1_1]);
                Socket2DReceiveList[iBarCode1_2]->Add(sSimuCode[iBarCode1_2]);
                Socket2DReceiveList[iBarCode2_1]->Add(sSimuCode[iBarCode2_1]);
                Socket2DReceiveList[iBarCode2_2]->Add(sSimuCode[iBarCode2_2]);
            }
            #endif

            bHasError=false;
            Task=1350;
            Bottom2DDelay.SetMSAndOn(TestIF_File.iBarCodeDelay);                //這個是讀取Time Out用的
        case 1350:
            if(bCCDBarcodeGetResultOK[iBarCode1_1]==true && bCCDBarcodeGetResultOK[iBarCode1_2]==true &&
               bCCDBarcodeGetResultOK[iBarCode2_1]==true && bCCDBarcodeGetResultOK[iBarCode2_2]==true) //讀取完成
            {
                sErrorPart="";
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Item[i][j]!=HAS_NULL_IC && InArmSuck.Item[i][j]!=NULL_IC)    //wei 20160318 Barcode Error依客戶設定
                        {
                            //KaiHuang 20200826 : Add CheckSum
                            //==>
                            if(InArmSuck.cDeviceInf[i][j]==asBarCodeErrorSend || InArmSuck.cDeviceInf[i][j]=="")
                            {
                                bHasError=true;
                                sErrorPart+=InArmSuck.Suck[i][j].sName;
                                if(CUSTOMER_CODE==CC_ASE_CL)
                                {
                                    sErrorPart+="(Err)";
                                }
                            }
                            else if(InArmSuck.cDeviceInf[i][j]==asBarCodeErrorCheckSum)
                            {
                                bHasError=true;
                                sErrorPart+=InArmSuck.Suck[i][j].sName;
                                if(CUSTOMER_CODE==CC_ASE_CL)
                                {
                                    sErrorPart+="(Sum)";
                                }
                            }
                            //<==
                        }
                    }
                }

                Task=2000;
            }
            else if(Bottom2DDelay.Off())  //讀取超時
            {
                sErrorPart="";
                for(int i=0; i<4; i++)
                {
                    if(bCCDBarcodeGetResultOK[iBarCode1_1+i]==false)
                    {
                        bHasError=true;
                        bCCDBarcodeGetResultOK[iBarCode1_1+i]=true;
                        AddCCDCommunicationLog(iBarCode1_1+i, "Action 1350, Decode time out.");
                    }
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iBarcodeRetryCount)                  //Steven 20160421 : 修正判斷方式
                {
                    Task=100;
                    iRetryCount++;                                              //Steven 20190327 : 修正Bottom 2D讀取Retry
                }
                else
                {
                    if(TestIF_File.bNoCodeDeviceAutoSkip &&
                       fContact->IsRun2DCheck()==false)                         //JerryYang 20250220 : 2DID硬體順序檢查功能
                    {
                        AddCCDCommunicationLog(iBarCode1_1, ">>Cycle 2000, Decode end with auto skip.---------------\r\n");
                        if(TestIF_File.bCheckCodeByShuttle==false && TestIF_File.bCheckCodeByLot==false)
                        {
                            Task=5000;
                        }
                        else
                        {
                            if(TestIF_File.bCheckLotHaveCode)                   //Steven 20170707 (wei) : Fixed 2DID for Korea
                                Task=3500;                                      //Alick 20170313 (wei) modify 4000==>3500
                            else
                                Task=4000;
                        }
                    }
                    else
                    {
                        Task=2500;
                    }
                }
                bHasError=false;
            }
            else
            {
                AddCCDCommunicationLog(iBarCode1_1, ">>Cycle 2000, Decode end.---------------\r\n");
                if(TestIF_File.bCheckCodeByShuttle==false && TestIF_File.bCheckCodeByLot==false)
                {
                    Task=5000;
                }
                else
                {
                    if(TestIF_File.bCheckLotHaveCode)                           //Steven 20170707 (wei) : Fixed 2DID for Korea
                        Task=3500;                                              //Alick 20170313 (wei) modify 4000==>3500
                    else
                        Task=4000;
                }
            }
            break;
        case 2500:
            Task=2600;
//            break;
        case 2600:
            Log.sprintf("Alarm 2600, In arm Barcode Error");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            if(TestIF_File.bCheckCodeByLot && iRetryCount<=TestIF_File.iBarcodeRetryCount)    //Alick 20170124 add Retry時bRetryOffsetMove=true先退出去  //Alick 20170315 (wei) modify bRetryOffsetMove=>bCheckCodeByLot
            {
                bHasError=false;
                iRetryCount++;
                Task=100;                                                       //jou 20170509 (Steven) : 2D alway retry
            }
            else
            {
                if(fContact->IsRun2DCheck()==true)                              //JerryYang 20250220 : 2DID硬體順序檢查功能
                {
                    ret=ShowErrorMessage("JAM0497", K_RETRY, MInArmX, false, sErrorPart);
                }
                else
                {
                    ret=ShowErrorMessage("JAM0460", K_RETRY|K_SKIP, MInArmX, false, sErrorPart);  //In Shuttle1 Barcode Error
                }
                iRetryCount=0;
                bHasError=false;
                if(ret==K_RETRY)
                {
                    Task=100;
                }
                else
                {
                    AddCCDCommunicationLog(iBarCode1_1, ">>Cycle 2600, Decode end with barcode error.---------------\r\n");
                    if(TestIF_File.bCheckCodeByShuttle==false && TestIF_File.bCheckCodeByLot==false)
                    {
                        Task=5000;
                    }
                    else
                    {
                        if(TestIF_File.bCheckLotHaveCode)                       //Steven 20170707 (wei) : Fixed 2DID for Korea
                            Task=3500;                                          //Alick 20170313 (wei) modify 4000==>3500
                        else
                            Task=4000;
                    }
                }
            }
            break;
        case 3500:
            if(TestIF_File.bCheckLotHaveCode)
            {
                FILE *P=NULL;
                char t[256];
                ZeroMemory(bHasErr, sizeof(bHasErr));
                bLotCheckError=false;
                if(FileExists(asBarCodeDownLot)!=false)
                {
                    if(LastSet.iTester==OFF_LINE)                               //Frank 20170611 (Steven) add Alex 多種Dummy IC 需要同時辨識
                    {
                        fBarCode->list2DByLot->Clear();
                        fBarCode->list2DByLot->LoadFromFile(asBarCodeDownLot);
                        fBarCode->list2DByLot->Sort();
                    }
                    else
                    {
                        P=fopen(asBarCodeDownLot.c_str(), "r");
                        if(P!=NULL)
                        {
                            fgets(t, 30, P);
                            Str1=t;
                            asBarcodeLotNumber="     Lot ID : "+Str1;           //wei Barcode Lot Alarm時，顯示出Lot
                            fclose(P);
                        }

                        iPos=Str1.Pos("\n");                                    //Frank 20170426 (Steven) add 消除"\n"為一個字元的寬度
                        if(iPos>0)
                            Str3=Str1.SubString(1, iPos-1);
                        else
                            Str3=Str1;
                    }

                    for(int i=0; i<InArmSuck.iMaxRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            if(InArmSuck.Item[i][j]!=HAS_NULL_IC && InArmSuck.Item[i][j]!=NULL_IC &&
                               InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorSend && InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorCheckSum &&  //KaiHuang 20200826 : Add CheckSum
                               InArmSuck.cDeviceInf[i][j]!="")
                            {
                                Str=InArmSuck.cDeviceInf[i][j];
                                Str2=Str.SubString(1, 9);

                                if(!fBarCode->list2DByLot->Find(Str2, iIndex) && LastSet.iTester==OFF_LINE)     //Frank 20170611 (Steven) add Alex 多種Dummy IC 需要同時辨識
                                {
                                    bLotCheckError=true;
                                    bHasErr[i][j]=true;
                                }
                                else if(Str2!=Str3)
                                {
                                    bLotCheckError=true;
                                    bHasErr[i][j]=true;
                                }
                            }
                        }
                    }

                    if(bLotCheckError)
                    {
                        Task=3600;
                    }
                    else
                    {
                        Task=4000;
                    }
                }
                else
                {
                    ShowErrorMessage("WAR0482", K_RETRY, MMSystem);
                }
            }
            else
            {
                Task=4000;
            }
            break;
        case 3600:
            fNote->t2DCode->Visible=true;
            fNote->t2DCode->XItem=InArmSuck.iMaxRow;
            fNote->t2DCode->YItem=InArmSuck.iMaxCol;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, InArmSuck.cDeviceInf[i][j].c_str());
                    if(bHasErr[i][j]==true)
                        fNote->t2DCode->SetCellColorIndex(i, j, 3);
                    else
                        fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }

            Log.sprintf("WAR0471, In arm check have Lot barcode error!\r\n");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            if(bLotCheckError==true)
            {
                ret=ShowErrorMessage("WAR0471", K_RETRY, MInArmX, false);       //In Shuttle1 Barcode Error  //Alick 20170202 remove skip
            }

            if(ret==K_RETRY)
            {
                Task=1;
            }
            break;
        case 4000:                                                              //Steven 20160428 : 檢查蝦頭的2D重複碼
            if(TestIF_File.bCheckCodeByShuttle || TestIF_File.bCheckCodeByLot)  //Steven 20171128 (Wei) : check lot之前一定要先確認蝦頭內沒有重複的
            {
                for(int i=0; i<BAR_CODE_COUNT; i++)
                {
                    map2DShuttleList[i].clear();
                }
                ZeroMemory(bHasErr, sizeof(bHasErr));
                bHasDupError=false;
                ErrPart="";

                for(int i=0; i<2; i++)                                          //JerryYang 20201112 修正Bottom 2DID 檢查到重複碼後位置記錄錯誤
                {
                    for(int j=0; j<2; j++)
                    {
                        for(int k=0; k<2; k++)
                        {
                            l=k*2+j;
                            m=i*2+k;
                            if(InArmSuck.Item[i][l]!=HAS_NULL_IC &&
                               InArmSuck.Item[i][l]!=NULL_IC &&
                               InArmSuck.cDeviceInf[i][l]!=asBarCodeErrorSend &&
                               InArmSuck.cDeviceInf[i][l]!=""&&
                               InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorCheckSum)  //KaiHuang 20200826 : Add CheckSum
                            {
                                mapShtIter[m]=map2DShuttleList[m].find(InArmSuck.cDeviceInf[i][l]);
                                if(mapShtIter[m]==map2DShuttleList[m].end())
                                {
                                    map2DShuttleList[m][InArmSuck.cDeviceInf[i][l]]=m;
                                }
                                else                                            //有重複的就不用加進去了
                                {
                                    iBarcodeDuplicate[m]++;
                                    bHasDupError=true;
                                    ErrPart+=OutArmSuck.Suck[i][l].sName;
                                    bHasErr[i][l]=true;
                                }
                            }
                        }
                    }
                }

                if(bHasDupError)
                {
                    Task=4050;
                }
                else
                {
                    Task=4100;
                }
            }
            else
            {
                Task=4100;
            }
            break;
        case 4050:
            fNote->t2DCode->Visible=true;
            fNote->t2DCode->XItem=InArmSuck.iMaxRow;
            fNote->t2DCode->YItem=InArmSuck.iMaxCol;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, InArmSuck.cDeviceInf[i][j].c_str());
                    if(bHasErr[i][j]==true)
                        fNote->t2DCode->SetCellColorIndex(i, j, 3);
                    else
                        fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }

            Log.sprintf("Alarm 4050, In arm check have duplicate barcode error!\r\n");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            if(TestIF_File.iShtDuplicateRetryCnt<=0 ||                          //Steven 20190520 : 修正等於0時會變成無窮迴圈
               TestIF_File.iShtDuplicateRetryCnt>=iShtDupilcateFailCnt[0])      //Steven 20160823 : 蝦頭重複碼要可以自動Retry
            {
                iShtDupilcateFailCnt[0]=0;
                ret=ShowErrorMessage("WAR0465", K_RETRY|K_SKIP, MInArmX, false, ErrPart);  //In shuttle 1 check have duplicate barcode error!
            }
            else
            {
                iShtDupilcateFailCnt[0]++;
                ret=K_RETRY;
            }

            if(ret==K_SKIP)
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(bHasErr[i][j]==true)                                 //Steven 20160503 : 重複的Code要設定為Error
                        {
                            InArmSuck.cDeviceInf[i][j]=asBarCodeErrorSend;
                        }
                    }
                }
                Task=4100;
            }
            else
            {
                Task=1;
            }
            break;
        case 4100:
            iShtDupilcateFailCnt[0]=0;
            if(TestIF_File.bCheckCodeByLot &&
               fContact->IsRun2DCheck()==false)                                 //JerryYang 20250220 : 2DID硬體順序檢查功能
            {
                bLotHasDupError=false;
                ZeroMemory(bHasErr, sizeof(bHasErr));
                ErrPart="";

                for(int i=0; i<2; i++)                                          //JerryYang 20201112 修正Bottom 2DID 檢查到重複碼後位置記錄錯誤
                {
                    for(int j=0; j<2; j++)
                    {
                        for(int k=0; k<2; k++)
                        {
                            l=k*2+j;
                            m=i*2+k;
                            if(InArmSuck.Item[i][l]!=HAS_NULL_IC &&
                               InArmSuck.Item[i][l]!=NULL_IC &&
                               InArmSuck.cDeviceInf[i][l]!=asBarCodeErrorSend &&
                               InArmSuck.cDeviceInf[i][l]!=""&&
                               InArmSuck.cDeviceInf[i][l]!=asBarCodeErrorCheckSum)  //KaiHuang 20200826 : Add CheckSum
                            {
                                map2DIter=map2DList.find(InArmSuck.cDeviceInf[i][l]);
                                if(map2DList.size()!=0 && map2DIter!=map2DList.end())
                                {
                                    iBarcodeDuplicate[m]++;
                                    bLotHasDupError=true;
                                    ErrPart+=ErrPart+=OutArmSuck.Suck[i][l].sName;
                                    bHasErr[i][l]=true;
                                }
                            }
                        }
                    }
                }

                if(bLotHasDupError)
                {
                    Task=4150;
                }
                else
                {
                    Task=4200;
                }
            }
            else
            {
                Task=4200;
            }
            break;
        case 4150:
            fNote->t2DCode->Visible=true;
            fNote->t2DCode->XItem=InArmSuck.iMaxRow;
            fNote->t2DCode->YItem=InArmSuck.iMaxCol;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, InArmSuck.cDeviceInf[i][j].c_str());
                    if(bHasErr[i][j]==true)
                        fNote->t2DCode->SetCellColorIndex(i, j, 3);
                    else
                        fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }

            Log.sprintf("Alarm 4150, In Arm check have duplicate barcode in lot error!\r\n");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            ret=ShowErrorMessage("WAR0467", K_RETRY|K_SKIP, MInArmX, false, ErrPart);  //In shuttle 1 check have duplicate barcode in lot error!
            if(ret==K_SKIP)
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(bHasErr[i][j]==true)                                 //Steven 20160503 : 重複的Code要設定為Error
                        {
                            InArmSuck.cDeviceInf[i][j]=asBarCodeErrorSend;
                        }
                    }
                }
                Task=4200;
            }
            else
            {
                Task=1;
            }
            break;
        case 4200:                                                              //jou 20191008 : Barcode duplicate file by out arm
            if(TestIF_File.bCheckCodeByLot &&
               CosFunction.bBarcodeDuplicateFileByOutArm==false &&              //把新的值加到List裡面
               fContact->IsRun2DCheck()==false)                                 //JerryYang 20250220 : 2DID硬體順序檢查功能
            {
                list2DByLot->Clear();
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Item[i][j]!=HAS_NULL_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorSend &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorCheckSum &&
                           InArmSuck.cDeviceInf[i][j]!="")
                        {
                            map2DList[InArmSuck.cDeviceInf[i][j]]=i;
                            list2DByLot->Add(InArmSuck.cDeviceInf[i][j]);
                        }
                    }
                }

                WriteDataToFile(asBarCodeLot, list2DByLot->Text);               //Steven 20160429 : 開程式要把2D List讀回來
            }

            if(TestIF_File.bSearch2DIDByLot && LastSet.iTester==OFF_LINE)// && CUSTOMER_CODE==CC_KYEC_XILINX)   //Frank 20170316 (wei) add Search 2DID By Lot
                Task=4500;
            else
                Task=5000;
            break;
        case 4500:                                                              //Frank 20170316 (wei) add Search 2DID By Lot
            if(FileExists(asSearch2DIDByLot))
            {
                list2DByLot->Clear();
                list2DByLot->LoadFromFile(asSearch2DIDByLot);
            }
            else
            {
                //待Frank新增
            }

            list2DByLot->Sort();

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    Str=InArmSuck.cDeviceInf[i][j];
                    iPos=Str.Pos("\r\n");
                    if(iPos>0)
                        Str1=Str.SubString(1, iPos-1);
                    else
                        Str1=Str;
                    if(fBarCode->list2DByLot->Find(Str1, iIndex)==true)
                    {
                        InArmSuck.cDeviceInf[i][j]=asBarCodeErrorSend;
                    }
                }
            }
            Task=5000;
            break;
        case 5000:
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<2; j++)
                {
                    for(int k=0; k<2; k++)
                    {
                        l=k*2+j;
                        m=i*2+k;
                        if(InArmSuck.Item[i][l]!=HAS_NULL_IC && InArmSuck.Item[i][l]!=NULL_IC)
                        {
                            iNeedBarcodeCount[m]++;
                            if(InArmSuck.cDeviceInf[i][l]==asBarCodeErrorSend ||
                               InArmSuck.cDeviceInf[i][l]==asBarCodeErrorCheckSum ||
                               InArmSuck.cDeviceInf[i][l]=="")
                            {
                                iBarcodeErrorCount[m]++;
                                iBarcodeReject++;
                            }
                            else
                            {
                                iBarcodePassCount[m]++;
                            }
                            InArmSuck.PordRec[i][l].Add2DIDRecord(InArmSuck.cDeviceInf[i][l]);
                        }
                    }
                }
            }

            if(TestIF_File.bEnableConsecutiveFailure && iBarcodeReject>=TestIF_File.iConsecutiveFailure)   //JerryYang 20210414 : Bottom 2D 支援累加fail alarm功能
            {
                ShowErrorMessage("WAR0475", K_SKIP, MInShuttle1, false);
                iBarcodeReject=0;
            }

            if(DoBarcodeCount())                                                //Steven 20171222 (Wei) : Yield Alarm of 2DID
            {
                Task=5200;
                break;
            }
            else
            {
                Task=5100;
            }
        case 5100:
            Task=1;
            return true;
        case 5200:
            ShowErrorMessage("WAR0483", 0, MMSystem, false, s2DIDYield);
            Task=5100;
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::edtOffsetXClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 30.0, -30.0);
}
//------------------------------------------------------------------------------
void TfBarCode::Read2DIDList()                                                  //Steven 20190604 : 2DID不在List內的另外分bin
{
    list2DByLot->Clear();
    listError2DID->Clear();

    if(LastSet.iTester!=OFF_LINE ||
       TestIF_File.bEnableBarCode==false)
    {
        return;
    }

    if(TestIF_File.bSearch2DIDByLot)                                            //Frank 20170316 (wei) add Search 2DID By Lot
    {
        if(FileExists(asSearch2DIDByLot))
        {
            list2DByLot->LoadFromFile(asSearch2DIDByLot);
        }
        else
        {
            //待Frank新增
        }
        list2DByLot->Sort();
    }
    else if(TestIF_File.b2DIDNotExist2Error)                                    //JerryYang 20231218 : 2DID黑名單功能
    {
        if(FileExists(as2DIDSortToError))
        {
            listError2DID->LoadFromFile(as2DIDSortToError);
            listError2DID->Sort();
        }
        else
        {
            AnsiString s="";
            s.sprintf("Can not found the 2DID file in path %s", as2DIDSortToError);
            ShowMyMessage(s);
        }
    }
}
//------------------------------------------------------------------------------
int TfBarCode::b2DIDIsInsideList(int i, int j)                                  //Steven 20190604 : 2DID不在List內的另外分bin
{
    int ret=0, iIndex;
    AnsiString Str, Str1;
    int iPos;
    Str.sprintf("%s", TestSocket.cDeviceInf[i][j]);
    iPos=Str.Pos("\r\n");
    if(iPos>0)
        Str1=Str.SubString(1, iPos-1);
    else
        Str1=Str;

    if(Str==asBarCodeErrorSend || Str=="" || Str==asBarCodeErrorCheckSum)       //KaiHuang 20200826 : Add CheckSum
    {
        ret=2;
    }
    else if(list2DByLot->Find(Str1, iIndex)==true)
    {
        ret=1;
    }
    return ret;
}
//------------------------------------------------------------------------------
int TfBarCode::bSHT_2DIDIsInsideList(int iSHT, int i, int j, int *iIndex)       //JerryYang 20241104 : 支援2DID白名單功能
{
    int ret=0, iX=0;
    AnsiString Str, Str1;
    int iPos;

    if(iSHT==0)
    {
        Str.sprintf("%s", FLCarryKit.cDeviceInf[i][j]);
    }
    else
    {
        Str.sprintf("%s", BLCarryKit.cDeviceInf[i][j]);
    }

    iPos=Str.Pos("\r\n");
    if(iPos>0)
        Str1=Str.SubString(1, iPos-1);
    else
        Str1=Str;

    if(Str==asBarCodeErrorSend || Str=="" || Str==asBarCodeErrorCheckSum)       //KaiHuang 20200826 : Add CheckSum
    {
        ret=2;
    }
    else if(TestIF_File.b2DIDAllowList && fLotInfo->cbRunMode->Text!="CORR")
    {
        if(list2DWhitle->Find(Str1, iX)==true)
        {
           ret=1;
        }
    }
    else if(list2DByLot->Find(Str1, iX)==true)
    {
        ret=1;
    }
    *iIndex=iX;
    return ret;
}
//---------------------------------------------------------------------------
int TfBarCode::b2DIDIsInsideToErrorBin(int i, int j)                            //JerryYang 20231218 : 2DID黑名單功能
{
    if(TestIF_File.b2DIDNotExist2Error==false)
        return 0;

    int ret=0, iIndex;
    AnsiString Str, Str1;
    int iPos;
    Str.sprintf("%s", TestSocket.cDeviceInf[i][j]);
    iPos=Str.Pos("\r\n");
    if(iPos>0)
        Str1=Str.SubString(1, iPos-1);
    else
        Str1=Str;

    if(Str==asBarCodeErrorSend || Str=="" || Str==asBarCodeErrorCheckSum)
    {
        ret=2;
    }
    else if(listError2DID->Find(Str1, iIndex)==true)
    {
        ret=1;
    }
    return ret;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::cobMain_BinMapping_Auto1Change(TObject *Sender)      //jou 20190930 : Barcode Tray record file
{
    TComboBox *temp_Ptr;
    temp_Ptr=(TComboBox *)Sender;
    int iTag, iTrayNumber;
    iTag=temp_Ptr->Tag;

    iTrayNumber=ShowBinMappingAuto_Index_Ptr[iTag]->ItemIndex;

    if(iTrayNumber==LastSet.iDevice_Info_By_Tray[iTag])
    {
        DoShow_BinMapping_Info(iTag);
    }
    else
    {
        DoShow_BinMapping_Info_Old_Data(iTag, iTrayNumber);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::DoShow_BinMapping_Info(int iAuto)                    //jou 20190930 : Barcode Tray record file
{
    int iTrayIndex, x, y;                                                       //13.08.06.01   klutter

    iTrayIndex=LastSet.iDevice_Info_By_Tray[iAuto];

    while(ShowBinMappingAuto_Index_Ptr[iAuto]->Items->Count<iTrayIndex+1)
    {
        ShowBinMappingAuto_Index_Ptr[iAuto]->Items->Add(ShowBinMappingAuto_Index_Ptr[iAuto]->Items->Count+1);
    }
    ShowBinMappingAuto_Index_Ptr[iAuto]->ItemIndex=iTrayIndex;

    for(x=0; x<MOT[iMMAuto[iAuto]].Tray.XItem; x++)
    {
        for(y=0; y<MOT[iMMAuto[iAuto]].Tray.YItem; y++)
        {
            ShowBinMappingUnloadPtr[iAuto]->SetCellNumber(x, y, MOT[iMMAuto[iAuto]].Tray.cDeviceInf[x][y]);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::DoShow_BinMapping_Info_Old_Data(int iAuto, int iTrayNumber)  //jou 20190930 : Barcode Tray record file
{
    int iTrayIndex, x, y;                                                       //13.08.15.01   klutter

    iTrayIndex=iTrayNumber;
    while(ShowBinMappingAuto_Index_Ptr[iAuto]->Items->Count<iTrayIndex)
    {
        ShowBinMappingAuto_Index_Ptr[iAuto]->Items->Add(ShowBinMappingAuto_Index_Ptr[iAuto]->Items->Count+1);
    }

    Read_Device_Info_By_Tray(iAuto, iTrayIndex);
    for(x=0; x<_MAX_COL_ITEM; x++)
    {
        for(y=0; y<_MAX_ROW_ITEM; y++)
        {
            ShowBinMappingUnloadPtr[iAuto]->SetCellNumber(x, y, mtShowBarcodeTray[iAuto][x][y]);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::WriteTrayInfo(AnsiString asData, int iAuto, int iTrayNo) //jou 20190930 : Barcode Tray record file
{
    if(LastSet.iRealDummy==REALLY           &&                                  //13.12.19.01s   klutter
       TestIF_File.bEnableBarCode==true     &&
       CosFunction.bBarcodeTrayRecFile==true)
    {
        if(asData.IsEmpty()==true)
        {
            return;
        }

        AnsiString asPath, asLogString="", asFileName, asSchedule, asTestStatus;
        asSchedule      = fLotInfo->edtSysLotID->Text;
        asTestStatus    = fMain->cbRunStartMode->Text;
        FILE *Fp;

        asPath.printf("D:\\HT9045_Log\\TrayInfo");
        if(DirectoryExists(asPath)==false)
        {
            if(ForceDirectories(asPath)==false)
            {
                asPath.cat_printf(" create fail");
                throw Exception(asPath);
            }
        }

        asPath.cat_printf("\\%04d_%02d", SystemYear, SystemMonth);              //13.10.23.01s  klutter
        if(DirectoryExists(asPath)==false)
        {
            if(ForceDirectories(asPath)==false)
            {
                asPath.cat_printf(" create fail");
                throw Exception(asPath);
            }
        }

        if(asSchedule.IsEmpty())                                                //13.12.24.01s  klutter
        {
            asSchedule="NA";
        }

        asPath.cat_printf("\\%s", asSchedule);
        if(DirectoryExists(asPath)==false)
        {
            if(ForceDirectories(asPath)==false)
            {
                asPath.cat_printf(" create fail");
                throw Exception(asPath);
            }
        }

        if(asTestStatus.IsEmpty())                                              //13.12.24.01s  klutter
        {
            asTestStatus="FT";
        }

        if(iRunStartMode==FT)
        {
            asTestStatus = "FT";
        }
        else if(iRunStartMode==RT)
        {
             asTestStatus="RT"+AnsiString(LastSet.iBarcodeTrayRecFileRetestCT+1);
        }

        asPath.cat_printf("\\%s", asTestStatus);
        if(DirectoryExists(asPath)==false)
        {
            if(CreateDir(asPath)==false)
            {
                asPath.cat_printf(" create fail");
                throw Exception(asPath);
            }
        }

        if(iAuto==eFix7  || iAuto==eFix8  || iAuto==eFix9 ||
           iAuto==eFix10 || iAuto==eFix11 || iAuto==eFix12 || iAuto==eBulkBox)
        {

        }
        else if(iAuto>=eAuto1 && iAuto<eTrayCount)
        {
            asFileName.printf("%s_%s_%02d", asSchedule, s06TrayName[iAuto], iTrayNo);
            asPath.cat_printf("\\%s.txt", asFileName);

            Fp=fopen(asPath.c_str(), "a+");
            if(Fp!=NULL)
            {
                asLogString.printf("%s\n", asData);
                fwrite(asLogString.c_str(), asLogString.Length(), 1, Fp);
                fclose(Fp);
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::BarWriteTrayMap(AnsiString asData, int iAuto, int iTrayNo) //jou 20190930 : Barcode Tray record file
{
    if(LastSet.iRealDummy==REALLY           &&                                  //13.12.19.01s   klutter
       TestIF_File.bEnableBarCode==true     &&
       CosFunction.bBarcodeTrayRecFile==true || CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(asData.IsEmpty()==true)
        {
            return;
        }

        AnsiString asPath, asLogString="", asFileName, asSchedule, asTestStatus;
        asSchedule      = sOSATLotID;
        asTestStatus    = fMain->cbRunStartMode->Text;
        FILE *Fp;

        asPath.printf("D:\\HT9045_Log\\TrayInfo");
        if(DirectoryExists(asPath)==false)
        {
            if(CreateDir(asPath)==false)
            {
                asPath.cat_printf(" create fail");
                throw Exception(asPath);
            }
        }

        asPath.cat_printf("\\%04d_%02d", SystemYear, SystemMonth);              //13.10.23.01s  klutter
        if(DirectoryExists(asPath)==false)
        {
            if(CreateDir(asPath)==false)
            {
                asPath.cat_printf(" create fail");
                throw Exception(asPath);
            }
        }                                                                       //13.10.23.01e  klutter

        if(asSchedule.IsEmpty())                                                //13.12.24.01s  klutter
        {
            asSchedule="NA";
        }                                                                       //13.12.24.01e  klutter

        asPath.cat_printf("\\%s", asSchedule);
        if(DirectoryExists(asPath)==false)
        {
            if(CreateDir(asPath)==false)
            {
                asPath.cat_printf(" create fail");
                throw Exception(asPath);
            }
        }

        if(TrayID[3+iAuto][1]=="")
        {
            TrayID[3+iAuto][1]="NA";
        }

        asFileName.printf("%s_%s", asSchedule, TrayID[3+iAuto][1]);
        asPath.cat_printf("\\%s.txt", asFileName);

        Fp=fopen(asPath.c_str(), "a+");
        if(Fp!=NULL)
        {
            asLogString.printf("%s\n", asData);
            fwrite(asLogString.c_str(), asLogString.Length(), 1, Fp);
            fclose(Fp);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::Write_Device_Info_By_Tray(int iAuto, int iTray_Number)  //jou 20190930 : Barcode Tray record file
{
    int iMot=iMMAuto[iAuto], x, y, iBin=0;                                      //14.11.25.01   klutter
    AnsiString asSafeTemp_Patch, asData, SBin="";                               //13.10.03.01   klutter
    asSafeTemp_Patch="D:\\HT9045_Log\\Temp";                                    //14.11.25.01   klutter
    if(DirectoryExists(asSafeTemp_Patch)==false)
    {
        if(!CreateDir(asSafeTemp_Patch))
        {
            asSafeTemp_Patch.cat_printf(" create fail");
            throw Exception(asSafeTemp_Patch);
        }
    }

    if(iAuto==eFix7  || iAuto==eFix8  || iAuto==eFix9 ||
       iAuto==eFix10 || iAuto==eFix11 || iAuto==eFix12 || iAuto==eBulkBox)
        return;
    else if(iAuto>=eAuto1 && iAuto<eTrayCount)
        asSafeTemp_Patch.cat_printf("\\Device_info_%s_%02d.dat", s06TrayName[iAuto], iTray_Number+1);
    else
        return;

    FILE *Fp=fopen(asSafeTemp_Patch.c_str(), "wb");
    if(Fp!=NULL)
    {
        for(x=0; x<MOT[iMot].Tray.XItem; x++)
        {
            for(y=0; y<MOT[iMot].Tray.YItem; y++)
            {
                strcpy(mtShowBarcodeTray[iMot][x][y], MOT[iMot].Tray.cDeviceInf[x][y].c_str());
            }
        }

        fwrite((int *)&mtShowBarcodeTray[iMot][0][0][0], 1, sizeof(mtShowBarcodeTray[0]), Fp);
        fclose(Fp);

        for(y=0; y<MOT[iMot].Tray.YItem; y++)
        {
            for(x=0; x<MOT[iMot].Tray.XItem; x++)
            {
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                {
                    iBin=(MOT[iMot].Tray.iBinCode[x][y]-TEST_PASS);
                    if(iBin==-TEST_PASS) //沒DATA
                        SBin="NA";
                    else
                        SBin=IntToStr(iBin);
                    if(x ==(MOT[iMot].Tray.XItem-1))
                        asData.cat_printf("[%02d:%02d]%s;\n", x, y,SBin);       //kevin 20220325 add bin code
                    else
                        asData.cat_printf("[%02d:%02d]%s,", x, y,SBin);         //kevin 20220325 add bin code
                }
                else
                {
                    asData.cat_printf("[%02d,%02d]=%20s\t\t", x, y,
                                   MOT[iMot].Tray.cDeviceInf[x][y]);            //13.10.03.01   klutter
                }
                MOT[iMot].Tray.cDeviceInf[x][y]="";
                ShowBinMappingUnloadPtr[iMot]->SetCellNumber(x, y, " ");
            }

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                BarWriteTrayMap(asData, iAuto, iTray_Number+1);                 //kevin 20220326 Aut 1 2 3 tray map save file
            else
                WriteTrayInfo(asData, iAuto, iTray_Number+1);                   //13.10.03.01   klutter
            asData="";
        }
    }
    else
    {
        asSafeTemp_Patch.cat_printf(" file save error");
        ShowMyMessage(asSafeTemp_Patch.c_str());
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::Read_Device_Info_By_Tray(int iAuto, int iTray_Number)//jou 20190930 : Barcode Tray record file
{
    AnsiString asSafeTemp_Patch;
    asSafeTemp_Patch="D:\\HT9045_Log\\Temp";                                    //14.11.25.01   klutter
    if(DirectoryExists(asSafeTemp_Patch)==false)
    {
        if(!CreateDir( asSafeTemp_Patch))
        {
            asSafeTemp_Patch.cat_printf(" create fail");
            throw Exception(asSafeTemp_Patch);
        }
    }

    if(iAuto==eFix7  || iAuto==eFix8  || iAuto==eFix9 ||
       iAuto==eFix10 || iAuto==eFix11 || iAuto==eFix12 || iAuto==eBulkBox)
        return;
    else if(iAuto>=eAuto1 && iAuto<eTrayCount)
        asSafeTemp_Patch.cat_printf("\\Device_info_%s_%02d.dat", s06TrayName[iAuto], iTray_Number+1);
    else
        return;

    FILE *Fp=fopen(asSafeTemp_Patch.c_str(), "rb");
    if(Fp!=NULL)
    {
        fread((int *)&mtShowBarcodeTray[iAuto][0][0][0], 1, sizeof(mtShowBarcodeTray[0]), Fp);
        fclose(Fp);
    }
    else
    {
        asSafeTemp_Patch.cat_printf(" file read error");
        ShowMyMessage(asSafeTemp_Patch.c_str());
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::InitBarcodeRecFile()                                 //jou 20190930 : Barcode Tray record file
{
    LastSet.iBarcodeTrayRecFileRetestCT=0;
    for(int i=0; i<10 ; i++)
        LastSet.iDevice_Info_By_Tray[i]=0;

    for(int i=eAuto1; i<=iAutoRight; i++)
    {
        ShowBinMappingAuto_Index_Ptr[i]->Clear();
        ShowBinMappingAuto_Index_Ptr[i]->Text="";
        ShowBinMappingUnloadPtr[i]->XItem=AutoForm[i]->XDivision;
        ShowBinMappingUnloadPtr[i]->YItem=AutoForm[i]->YDivision;
        ShowBinMappingUnloadPtr[i]->ClearCell();
    }
}
//------------------------------------------------------------------------------
int TfBarCode::ChangeCharacter(AnsiString aNum)                                 //KaiChen 20191121 ：中壢日月光 2D Check Sum
{
    if( aNum=="0" )           return 0;
    else if( aNum=="1" )      return 1;
    else if( aNum=="2" )      return 2;
    else if( aNum=="3" )      return 3;
    else if( aNum=="4" )      return 4;
    else if( aNum=="5" )      return 5;
    else if( aNum=="6" )      return 6;
    else if( aNum=="7" )      return 7;
    else if( aNum=="8" )      return 8;
    else if( aNum=="9" )      return 9;
    else if( aNum=="A" )      return 10;
    else if( aNum=="B" )      return 11;
    else if( aNum=="C" )      return 12;
    else if( aNum=="D" )      return 13;
    else if( aNum=="E" )      return 14;
    else if( aNum=="F" )      return 15;
    else if( aNum=="G" )      return 16;
    else if( aNum=="H" )      return 17;
    else if( aNum=="J" )      return 18;
    else if( aNum=="K" )      return 19;
    else if( aNum=="L" )      return 20;
    else if( aNum=="M" )      return 21;
    else if( aNum=="N" )      return 22;
    else if( aNum=="P" )      return 23;
    else if( aNum=="Q" )      return 24;
    else if( aNum=="R" )      return 25;
    else if( aNum=="S" )      return 26;
    else if( aNum=="T" )      return 27;
    else if( aNum=="U" )      return 28;
    else if( aNum=="V" )      return 29;
    else if( aNum=="W" )      return 30;
    else if( aNum=="X" )      return 31;
    else if( aNum=="Y" )      return 32;
    else if( aNum=="Z" )      return 33;
    else                      return 99;
}
//------------------------------------------------------------------------------
bool TfBarCode::ReChangeCharacter(int iNum, AnsiString aCom)                    //KaiChen 20191121 ：中壢日月光 2D Check Sum
{
    AnsiString aData;

    switch(iNum+1)
    {
        case 1:     aData="0";      break;
        case 2:     aData="1";      break;
        case 3:     aData="2";      break;
        case 4:     aData="3";      break;
        case 5:     aData="4";      break;
        case 6:     aData="5";      break;
        case 7:     aData="6";      break;
        case 8:     aData="7";      break;
        case 9:     aData="8";      break;
        case 10:    aData="9";      break;
        case 11:    aData="A";      break;
        case 12:    aData="B";      break;
        case 13:    aData="C";      break;
        case 14:    aData="D";      break;
        case 15:    aData="E";      break;
        case 16:    aData="F";      break;
        case 17:    aData="G";      break;
        case 18:    aData="H";      break;
        case 19:    aData="J";      break;
        case 20:    aData="K";      break;
        case 21:    aData="L";      break;
        case 22:    aData="M";      break;
        case 23:    aData="N";      break;
        case 24:    aData="P";      break;
        case 25:    aData="Q";      break;
        case 26:    aData="R";      break;
        case 27:    aData="S";      break;
        case 28:    aData="T";      break;
        case 29:    aData="U";      break;
        case 30:    aData="V";      break;
        case 31:    aData="W";      break;
        case 32:    aData="X";      break;
        case 33:    aData="Y";      break;
        case 34:    aData="Z";      break;
        case 35:    aData="0";      break;                                      //20160204 , Nickliu , 2D Check sum

    }

    if(aCom==aData)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
bool TfBarCode::CheckDigit(AnsiString sSource)                                  //KaiChen 20191121 ：中壢日月光 2D Check Sum
{
    int iOdd=0, iEven=0, iDivide=0, iSum=0;
    bool bResult=false;
    String sTestResultToTxtFile;                                                //nickliu

    for(int i=0; i<TestIF_File.iCheckSumLength-1; i++)                          //Nickliu 20190401 ASECheckSumLength
    {
        if(i%2==0)
        {
            iEven+=ChangeCharacter(sSource.SubString(i+1, 1));
        }
        else
        {
            iOdd+=ChangeCharacter(sSource.SubString(i+1, 1));
        }
    }

    iSum=iEven+iOdd*3;
    iDivide=iSum%34;

    bResult=ReChangeCharacter(34-iDivide,sSource.SubString(17, 1));
    sTestResultToTxtFile=AnsiString(34-iDivide);
    sTestResultToTxtFile+=",";
    sTestResultToTxtFile+=sSource.SubString(17, 1);
    sTestResultToTxtFile+=",";
    return bResult;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::chkMulti2DIDClick(TObject *Sender)
{
    tsMulti2DID->TabVisible=chkMulti2DID->Checked;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::SetMulti2DMap(int iType)                             //Steven 20200810 : 一個IC使用多個2DID
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            Multi2DSiteCH[i][j]->Visible=true;
            Multi2DSiteCH[i][j]->Clear();
            Multi2DSiteCH[i][j]->Items->Add("---");
            Multi2DSiteCH[i][j]->Items->Add("1");
            Multi2DSiteCH[i][j]->Items->Add("2");
            if(iType>=3)
            {
                Multi2DSiteCH[i][j]->Items->Add("3");
                Multi2DSiteCH[i][j]->Items->Add("4");
            }
            Multi2DSiteCH[i][j]->ItemIndex=0;

            if(i==1 && iType==e1x2In1CCD)
            {
                Multi2DSiteCH[i][j]->Visible=false;
            }

            if(j==1 && (iType==e2x1In1CCD || iType==e2x1In2CCD))
            {
                Multi2DSiteCH[i][j]->Visible=false;
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::rgMulti2DTypeClick(TObject *Sender)
{
    SetMulti2DMap(rgMulti2DType->ItemIndex);
}
//------------------------------------------------------------------------------
bool __fastcall TfBarCode::CheckMulti2DMap()
{
//    for(int i=0; i<2; i++)                                                    //Steven 20240612 : Mark for 對角2D
//    {
//        for(int j=i+1; j<2; j++)
//        {
//            if(Multi2DSiteCH[i][j]->ItemIndex<=0 && Multi2DSiteCH[i][j]->Visible)
//            {
//                ShowMyMessage("2DID map not select!");
//                return true;
//            }
//        }
//    }

    if(rgMulti2DType->ItemIndex==e1x2In1CCD)
    {
        if(Multi2DSiteCH[0][0]->ItemIndex==Multi2DSiteCH[0][1]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }
    }
    else if(rgMulti2DType->ItemIndex==e2x1In1CCD ||
            rgMulti2DType->ItemIndex==e2x1In2CCD)
    {
        if(Multi2DSiteCH[0][0]->ItemIndex==Multi2DSiteCH[1][0]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }
    }
    else if(rgMulti2DType->ItemIndex==e2x2In1CCD ||
            rgMulti2DType->ItemIndex==e2x2In2CCD)
    {
        if(Multi2DSiteCH[0][0]->ItemIndex!=0 &&                                 //Steven 20240612 : Mark for 對角2D
           Multi2DSiteCH[0][1]->ItemIndex!=0 &&
           Multi2DSiteCH[0][0]->ItemIndex==Multi2DSiteCH[0][1]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }

        if(Multi2DSiteCH[0][0]->ItemIndex!=0 &&
           Multi2DSiteCH[1][0]->ItemIndex!=0 &&
           Multi2DSiteCH[0][0]->ItemIndex==Multi2DSiteCH[1][0]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }

        if(Multi2DSiteCH[0][0]->ItemIndex!=0 &&
           Multi2DSiteCH[1][1]->ItemIndex!=0 &&
           Multi2DSiteCH[0][0]->ItemIndex==Multi2DSiteCH[1][1]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }

        if(Multi2DSiteCH[0][1]->ItemIndex!=0 &&
           Multi2DSiteCH[1][0]->ItemIndex!=0 &&
           Multi2DSiteCH[0][1]->ItemIndex==Multi2DSiteCH[1][0]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }

        if(Multi2DSiteCH[0][1]->ItemIndex!=0 &&
           Multi2DSiteCH[1][1]->ItemIndex!=0 &&
           Multi2DSiteCH[0][1]->ItemIndex==Multi2DSiteCH[1][1]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }

        if(Multi2DSiteCH[1][0]->ItemIndex!=0 &&
           Multi2DSiteCH[1][1]->ItemIndex!=0 &&
           Multi2DSiteCH[1][0]->ItemIndex==Multi2DSiteCH[1][1]->ItemIndex)
        {
            ShowMyMessage("2DID map set same CH!");
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void TfBarCode::ClearBuffer(int iShuttle, AnsiString Log)                       //Steven 20200908 : 整合2DID Clear Buffer
{
    AnsiString Str;
    //RogerYang 20190319 add bottom 2did
    //==>
    if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)                              //Steven 20190308 : Bottom 2D
    {
        if(dVisionVer>=2)                                                       //wei 20170119 (Steven) 版本號大於2
        {
            if(TestIF_File.bEnableMulti2D)
                Str.sprintf("E9,0,%d,%d", 2, TestIF_File.iMulti2DCount);
            else
                Str.sprintf("E9,0,%d", 2);
        }
        else
        {
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
                Str.sprintf("SE0,9,%d,%d", 2, TestIF_File.iMulti2DCount);
            else
                Str.sprintf("SE0,9,%d", 2);
        }
    }
    else
    {
        if(dVisionVer>=2)                                                       //wei 20170119 (Steven) 版本號大於2
        {
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
                Str.sprintf("E9,0,%d,%d", InArmSuck.iShtCol, TestIF_File.iMulti2DCount);
            else
                Str.sprintf("E9,0,%d", InArmSuck.iShtCol);
        }
        else
        {
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
                Str.sprintf("SE0,9,%d,%d", InArmSuck.iShtCol, TestIF_File.iMulti2DCount);
            else
                Str.sprintf("SE0,9,%d", InArmSuck.iShtCol);
        }
    }
    SendCCDCommand(iShuttle, Log, Str);                                         //Reset CCD Shuttle暫存器
    //<==
    //RogerYang 20190319 add bottom 2did
}
//------------------------------------------------------------------------------
void TfBarCode::SendSE0(int iShuttle, int iHasIC, int iNowMoveStep, int iiNowMoveStepMulti2D, AnsiString Log)   //Steven 20200908 : 整合2DID SE0
{
    AnsiString Str;
    if(iHasIC==false)
    {
        if(dVisionVer>=2)                                                       //wei 20170119 (Steven) 版本號大於2
        {
            if(TestIF_File.bEnableMulti2D)
                Str.sprintf("E0,0,0,%d,%d", iNowMoveStep, iiNowMoveStepMulti2D);
            else
                Str.sprintf("E0,0,0,%d", iNowMoveStep);
        }
        else
        {
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
                Str.sprintf("SE0,0,%d,%d", iNowMoveStep, iiNowMoveStepMulti2D);
            else
                Str.sprintf("SE0,0,%d", iNowMoveStep);
        }
        mtBarcodeInSh->SetCellNumber(iShuttle, iNowMoveStep+1, "");
    }
    else
    {
        if(dVisionVer>=2)                                                       //wei 20170119 (Steven) 版本號大於2
        {
            if(TestIF_File.bEnableMulti2D)
                Str.sprintf("E0,0,1,%d,%d", iNowMoveStep, iiNowMoveStepMulti2D);
            else
                Str.sprintf("E0,0,1,%d", iNowMoveStep);
        }
        else
        {
            if(TestIF_File.bEnableMulti2D)                                      //Steven 20200810 : 一個IC使用多個2DID
                Str.sprintf("SE0,1,%d,%d", iNowMoveStep, iiNowMoveStepMulti2D);
            else
                Str.sprintf("SE0,1,%d", iNowMoveStep);
        }
        mtBarcodeInSh->SetCellNumber(iShuttle, iNowMoveStep+1, "Exposure...");
    }

    SendCCDCommand(iShuttle, Log, Str);
}
//------------------------------------------------------------------------------
int TfBarCode::GetMovePos(int iSht, int iNowMoveStep, int iNowMoveStepMulti2DX, bool bAlarm)
{
    int pos=0;
    if(TestIF_File.bEnableMulti2D &&
       (TestIF_File.iMulti2DType==e1x2In1CCD ||
        TestIF_File.iMulti2DType==e2x2In1CCD ||
        TestIF_File.iMulti2DType==e2x2In2CCD))                                  //Steven 20240612 : for 對角2D
    {
        if(InArmSuck.iShtCol==1)
        {
            pos=Prod.iInSHBarMulti2DDetectPos1x1[iSht][iNowMoveStepMulti2DX];
        }
        else if(InArmSuck.iShtCol==2)
        {
            pos=Prod.iInSHBarMulti2DDetectPos1x2[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        }
        else if(InArmSuck.iShtCol==3)                                           //ChungHung 20140115 add for 2x3_6
        {
            pos=Prod.iInSHBarMulti2DDetectPos2x3[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        }
        else if(InArmSuck.iShtCol==4)
        {
            pos=Prod.iInSHBarMulti2DDetectPos1x4[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        }
        else if(InArmSuck.iShtCol==5)                                           //Steven 20221027 : Add for 2x5
        {
            pos=Prod.iInSHBarMulti2DDetectPos2x5[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        }
        else if(InArmSuck.iShtCol==6)                                           //Steven 20170111 : fixed for 2x6
        {
            pos=Prod.iInSHBarMulti2DDetectPos2x6[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        }
        else if(InArmSuck.iShtCol==8)
        {
            pos=Prod.iInSHBarMulti2DDetectPos2x8[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        }
        else
        {
            if(bAlarm)
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "TfBarCode::GetMovePos");
        }
    }
    else
    {
        if(InArmSuck.iShtCol==1)
        {
            pos=Prod.iInSHBarCodeDetectPos1x1[iSht];
        }
        else if(InArmSuck.iShtCol==2)
        {
            pos=Prod.iInSHBarCodeDetectPos1x2[iSht][iNowMoveStep];
        }
        else if(InArmSuck.iShtCol==3)                                           //ChungHung 20140115 add for 2x3_6
        {
            pos=Prod.iInSHBarCodeDetectPos2x3[iSht][iNowMoveStep];
        }
        else if(InArmSuck.iShtCol==4)
        {
            pos=Prod.iInSHBarCodeDetectPos1x4[iSht][iNowMoveStep];
        }
        else if(InArmSuck.iShtCol==5)                                           //Steven 20221027 : Add for 2x5
        {
            pos=Prod.iInSHBarCodeDetectPos2x5[iSht][iNowMoveStep];
        }
        else if(InArmSuck.iShtCol==6)                                           //Steven 20170111 : fixed for 2x6
        {
            pos=Prod.iInSHBarCodeDetectPos2x6[iSht][iNowMoveStep];
        }
        else if(InArmSuck.iShtCol==8)
        {
            pos=Prod.iInSHBarCodeDetectPos2x8[iSht][iNowMoveStep];
        }
        else
        {
            if(bAlarm)
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "TfBarCode::GetMovePos");
        }
    }
    return pos;
}
//------------------------------------------------------------------------------
bool TfBarCode::IsSHT2DIDScanFinish(int SHT)                                    //JerryYang 20200916 : 判斷是否掃完2D
{
    bool bResult=true;

    if(SHT==0)
    {
        if(FLCarryKit.HasRealIC())
        {
            for(int i=0; i<FLCarryKit.iShtRow; i++)
            {
                for(int j=0; j<FLCarryKit.iShtCol; j++)
                {
                    if(FLCarryKit.Item[i][j]==HAS_IC ||
                       FLCarryKit.Item[i][j]==HAS_HOT_IC)
                    {
                        if(FLCarryKit.cDeviceInf[i][j]==""  ||
                           FLCarryKit.cDeviceInf[i][j]=="0")
                        {
                            bResult=false;
                        }
                    }
                }
            }
        }
        else
        {
            bResult=true;                                                       //JerryYang 20230620 : 沒有real ic就不用掃2D
        }
    }
    else if(SHT==1)
    {
        if(BLCarryKit.HasRealIC())
        {
            for(int i=0; i<BLCarryKit.iShtRow; i++)
            {
                for(int j=0; j<BLCarryKit.iShtCol; j++)
                {
                    if(BLCarryKit.Item[i][j]==HAS_IC ||
                       BLCarryKit.Item[i][j]==HAS_HOT_IC)
                    {
                        if(BLCarryKit.cDeviceInf[i][j]==""  ||
                           BLCarryKit.cDeviceInf[i][j]=="0")
                        {
                            bResult=false;
                        }
                    }
                }
            }
        }
        else
        {
            bResult=true;                                                       //JerryYang 20230620 : 沒有real ic就不用掃2D
        }
    }
    return bResult;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=75;
        Top=10;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::edtXPitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, 150.0, 0.0);
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::IPPort1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::edtBarcodeMinLengthClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, atoi(edtBarcodeMaxLength->Text.c_str()));
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::edtBarcodeMaxLengthClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, atoi(edtBarcodeMinLength->Text.c_str()));
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::edt1stLineLength_2DBarCodeMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, 0);
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::TimerBotton8CCDConnectTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;

    if(TestIF_File.bEnableBarCode==false &&
       TestIF_File.bEnableShtFloatChk==false)
    {
        bTimerRun=false;
        return;
    }

    static int Count1=90;
    static int Count2=90;
    static int Count3=90;
    static int Count4=90;
    static int Count5=90;
    static int Count6=90;
    static int Count7=90;
    static int Count8=90;

    //==========================================================================
    // iBottom_1
    //==========================================================================
    if(ClientSocket_Bottom_1->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_1->Address=="172.16.8.200" &&
       ClientSocket_Bottom_1->Port==5011)
    {
    }
    else
    {
        if(ClientSocket_Bottom_1->Active)
        {
            ClientSocket_Bottom_1->Close();
            SendCCDCommand(iBottom_1, "ClientSocket Close", "");
            Count1=0;
        }
    }

    if(ClientSocket_Bottom_1->Active==false &&
       bEnableCCDChannelConnect[iBottom_1]==true)
    {
        Count1++;
        if(Count1>20)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_1->Address="172.16.8.200";
            ClientSocket_Bottom_1->Port=5011;
            SendCCDCommand(iBottom_1, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_1->Open();
                SendCCDCommand(iBottom_1, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_1, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_1, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count1=0;
        }
    }
    //==========================================================================
    // iBottom_2
    //==========================================================================
    if(ClientSocket_Bottom_2->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_2->Address=="172.16.8.200" &&
       ClientSocket_Bottom_2->Port==5012)
    {
    }
    else
    {
        if(ClientSocket_Bottom_2->Active)
        {
            ClientSocket_Bottom_2->Close();
            SendCCDCommand(iBottom_2, "ClientSocket Close", "");
            Count2=0;
        }
    }

    if(ClientSocket_Bottom_2->Active==false &&
       bEnableCCDChannelConnect[iBottom_2]==true)
    {
        Count2++;
        if(Count2>30)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_2->Address="172.16.8.200";
            ClientSocket_Bottom_2->Port=5012;
            SendCCDCommand(iBottom_2, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_2->Open();
                SendCCDCommand(iBottom_2, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_2, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_2, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count1=0;
        }
    }
    //==========================================================================
    // iBottom_3
    //==========================================================================
    if(ClientSocket_Bottom_3->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_3->Address=="172.16.8.200" &&
       ClientSocket_Bottom_3->Port==5013)
    {
    }
    else
    {
        if(ClientSocket_Bottom_3->Active)
        {
            ClientSocket_Bottom_3->Close();
            SendCCDCommand(iBottom_3, "ClientSocket Close", "");
            Count3=0;
        }
    }

    if(ClientSocket_Bottom_3->Active==false &&
       bEnableCCDChannelConnect[iBottom_3]==true)
    {
        Count3++;
        if(Count3>30)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_3->Address="172.16.8.200";
            ClientSocket_Bottom_3->Port=5013;
            SendCCDCommand(iBottom_3, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_3->Open();
                SendCCDCommand(iBottom_3, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_3, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_3, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count3=0;
        }
    }
    //==========================================================================
    // iBottom_4
    //==========================================================================
    if(ClientSocket_Bottom_4->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_4->Address=="172.16.8.200" &&
       ClientSocket_Bottom_4->Port==5014)
    {
    }
    else
    {
        if(ClientSocket_Bottom_4->Active)
        {
            ClientSocket_Bottom_4->Close();
            SendCCDCommand(iBottom_4, "ClientSocket Close", "");
            Count4=0;
        }
    }

    if(ClientSocket_Bottom_4->Active==false &&
       bEnableCCDChannelConnect[iBottom_4]==true)
    {
        Count4++;
        if(Count4>30)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_4->Address="172.16.8.200";
            ClientSocket_Bottom_4->Port=5014;
            SendCCDCommand(iBottom_4, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_4->Open();
                SendCCDCommand(iBottom_4, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_4, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_4, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count4=0;
        }
    }
    //==========================================================================
    // iBottom_5
    //==========================================================================
    if(ClientSocket_Bottom_5->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_5->Address=="172.16.8.200" &&
       ClientSocket_Bottom_5->Port==5015)
    {
    }
    else
    {
        if(ClientSocket_Bottom_5->Active)
        {
            ClientSocket_Bottom_5->Close();
            SendCCDCommand(iBottom_5, "ClientSocket Close", "");
            Count5=0;
        }
    }

    if(ClientSocket_Bottom_5->Active==false &&
       bEnableCCDChannelConnect[iBottom_5]==true)
    {
        Count5++;
        if(Count5>30)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_5->Address="172.16.8.200";
            ClientSocket_Bottom_5->Port=5015;
            SendCCDCommand(iBottom_5, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_5->Open();
                SendCCDCommand(iBottom_5, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_5, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_5, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count5=0;
        }
    }
    //==========================================================================
    // iBottom_6
    //==========================================================================
    if(ClientSocket_Bottom_6->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_6->Address=="172.16.8.200" &&
       ClientSocket_Bottom_6->Port==5016)
    {
    }
    else
    {
        if(ClientSocket_Bottom_6->Active)
        {
            ClientSocket_Bottom_6->Close();
            SendCCDCommand(iBottom_6, "ClientSocket Close", "");
            Count6=0;
        }
    }

    if(ClientSocket_Bottom_6->Active==false && bEnableCCDChannelConnect[iBottom_6]==true)
    {
        Count6++;
        if(Count6>30)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_6->Address="172.16.8.200";
            ClientSocket_Bottom_6->Port=5016;
            SendCCDCommand(iBottom_6, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_6->Open();
                SendCCDCommand(iBottom_6, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_6, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_6, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count6=0;
        }
    }
    //==========================================================================
    // iBottom_7
    //==========================================================================
    if(ClientSocket_Bottom_7->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_7->Address=="172.16.8.200" &&
       ClientSocket_Bottom_7->Port==5017)
    {
    }
    else
    {
        if(ClientSocket_Bottom_7->Active)
        {
            ClientSocket_Bottom_7->Close();
            SendCCDCommand(iBottom_7, "ClientSocket Close", "");
            Count7=0;
        }
    }

    if(ClientSocket_Bottom_7->Active==false && bEnableCCDChannelConnect[iBottom_7]==true)
    {
        Count7++;
        if(Count7>30)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_7->Address="172.16.8.200";
            ClientSocket_Bottom_7->Port=5017;
            SendCCDCommand(iBottom_7, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_7->Open();
                SendCCDCommand(iBottom_7, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_7, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_7, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count7=0;
        }
    }
    //==========================================================================
    // iBottom_8
    //==========================================================================
    if(ClientSocket_Bottom_8->Active                  &&                        //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_8->Address=="172.16.8.200" &&
       ClientSocket_Bottom_8->Port==5018)
    {
    }
    else
    {
        if(ClientSocket_Bottom_8->Active)
        {
            ClientSocket_Bottom_8->Close();
            SendCCDCommand(iBottom_8, "ClientSocket Close", "");
            Count8=0;
        }
    }

    if(ClientSocket_Bottom_8->Active==false && bEnableCCDChannelConnect[iBottom_8]==true)
    {
        Count8++;
        if(Count8>30)                                                           //kevin 20191004 change
        {
            ClientSocket_Bottom_8->Address="172.16.8.200";
            ClientSocket_Bottom_8->Port=5018;
            SendCCDCommand(iBottom_8, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_8->Open();
                SendCCDCommand(iBottom_8, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch(...)
            {
                SendCCDCommand(iBottom_8, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                LogClientSocketExceptionError(ClientSocket_Bottom_8, "TfBarCode::TimerDownCCDConnect");            //Steven 20231113 : 紀錄網路連線例外
            }
            Count8=0;
        }
    }
    //==========================================================================

    bTimerRun=false;                                                            //最下面
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::TimerBottom8CCDInitialTimer(TObject *Sender)         //KaiChen 20200513 : Bottom 2D 8CCD
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;

    int &Task=iBottomConntectionOkTask1;
    int &Task2=iBottomConntectionOkTask2;
    AnsiString Str;

    if(ClientSocket_Bottom_1->Active &&                                         //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_1->Address=="172.16.8.200" &&
       ClientSocket_Bottom_1->Port==5011 &&
       ClientSocket_Bottom_2->Active &&                                         //KaiHuang 20200910 : Add 2~3
       ClientSocket_Bottom_2->Address=="172.16.8.200" &&
       ClientSocket_Bottom_2->Port==5012 &&
       ClientSocket_Bottom_3->Active &&
       ClientSocket_Bottom_3->Address=="172.16.8.200" &&
       ClientSocket_Bottom_3->Port==5013 &&
       ClientSocket_Bottom_4->Active &&
       ClientSocket_Bottom_4->Address=="172.16.8.200" &&
       ClientSocket_Bottom_4->Port==5014)
    {
        switch(Task)
        {
            case 1:
                SetSFCCheckStepCount();
                dVisionVer=1;
                Str.sprintf("VR");
                SendCCDCommand(iBottom_1, "Get Version", Str);                  //Reset CCD Shuttle暫存器
                bSFCGetVR=false;
                tChangeSetupFile.SetSecAndOn(30);
                Task=100;

            case 100:
                if(bSFCGetVR==true)
                {
                    if(TestIF_File.bEnableBarCode)
                        Task=1000;
                    else if(TestIF_File.bEnableShtFloatChk)
                        Task=2000;
                    else
                        Task=3000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=1;
                }
                break;
            case 1000:                                                          //Reset CCD Shuttle暫存器
                bGetSE9[iBottom_1]=false;
                bGetSE9[iBottom_2]=false;                                       //KaiHuang 20200910 : Add
                bGetSE9[iBottom_3]=false;                                       //KaiHuang 20200910 : Add
                bGetSE9[iBottom_4]=false;                                       //KaiHuang 20200910 : Add

                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    Str.sprintf("E9,0,%d", InArmSuck.iShtCol);
                else
                    Str.sprintf("SE0,9,%d", InArmSuck.iShtCol);

                SendCCDCommand(iBottom_1, "Clear buffer", Str);
                SendCCDCommand(iBottom_2, "Clear buffer", Str);
                SendCCDCommand(iBottom_3, "Clear buffer", Str);
                SendCCDCommand(iBottom_4, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(1000);
                Task=1050;

            case 1050:
                if(bGetSE9[iBottom_1] && bGetSE9[iBottom_2] && bGetSE9[iBottom_3] && bGetSE9[iBottom_4])    //KaiHuang 20200910 : Add iBottom_2~4
                    Task=1100;
                else if(tChangeSetupFile.Off())
                    Task=1000;
                break;
            case 1100:                                                          //取得工作檔名稱
                bGetGF=false;
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    SendCCDCommand(iBottom_1, "Get File Name", "GF,0");
                else
                    SendCCDCommand(iBottom_1, "Get File Name", "GF");
                tChangeSetupFile.SetSecAndOn(10);
                Task=1150;

            case 1150:
                if(bGetGF)
                {
                    Task=1200;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=1100;
                }
                break;
            case 1200:
                bGetLF=false;
                if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)                  //Steven 20190524 : Add bottom 2DID
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,1", fMain->cbSetupFileName->Text);
                    else
                        Str.sprintf("LF%s,1", fMain->cbSetupFileName->Text);
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,0", fMain->cbSetupFileName->Text);
                    else
                        Str.sprintf("LF%s,0", fMain->cbSetupFileName->Text);
                }
                SendCCDCommand(iBottom_1, "Change File", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task=1250;
            case 1250:
                if(bGetLF)
                {
                    Task=2000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=1200;
                }
                break;
            case 2000:
                if(TestIF_File.bEnableShtFloatChk)
                    Task=2100;
                else
                    Task=3000;
                break;
            case 2100:
                bSFCGetSE9[iBottom_1]=false;
                bSFCGetSE9[iBottom_2]=false;                                    //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_3]=false;                                    //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_4]=false;                                    //KaiHuang 20200910 : Add
                Str.sprintf("E9,1,%d", iSFCTotalMoveStep);

                SendCCDCommand(iBottom_1, "Clear buffer", Str);
                SendCCDCommand(iBottom_2, "Clear buffer", Str);
                SendCCDCommand(iBottom_3, "Clear buffer", Str);
                SendCCDCommand(iBottom_4, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(1000);
                Task=2150;
                //break;
            case 2150:
                if(bSFCGetSE9[iBottom_1] && bSFCGetSE9[iBottom_2] && bSFCGetSE9[iBottom_3] && bSFCGetSE9[iBottom_4])
                    Task=2200;
                else if(tChangeSetupFile.Off())
                    Task=2100;
                break;
            case 2200:                                                          //取得工作檔名稱
                bSFCGetGF=false;
                SendCCDCommand(iBottom_1, "Get File Name", "GF,1");
                tChangeSetupFile.SetSecAndOn(10);
                Task=2250;
                //break;
            case 2250:
                if(bSFCGetGF)
                {
                    if(fMain->cbSetupFileName->Text.UpperCase().AnsiCompare(sSFCCurrentGF)==0)
                    {
                        Task=3000;
                        bSFCGetLF=true;
                    }
                    else
                    {
                        Task=2300;
                    }
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=2200;
                }
                break;
            case 2300:
                bSFCGetLF=false;
                SendCCDCommand(iBottom_1, "SFC Change File", "LF,1,"+fMain->cbSetupFileName->Text);
                tChangeSetupFile.SetSecAndOn(10);
                Task=2350;
            case 2350:
                if(bSFCGetLF)
                {
                    Task=3000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task=2300;
                }
                break;
            case 3000:
                break;
        }
    }

    if(Task!=3000)
    {
        bTimerRun=false;
        return;
    }

    if(ClientSocket_Bottom_5->Active &&                                         //相同的IP跟Port就不要重新連線
       ClientSocket_Bottom_5->Address=="172.16.8.200" &&
       ClientSocket_Bottom_5->Port==5015 &&
       ClientSocket_Bottom_6->Active &&
       ClientSocket_Bottom_6->Address=="172.16.8.200" &&                        //KaiHuang 20200910 : Add 6~8
       ClientSocket_Bottom_6->Port==5016 &&
       ClientSocket_Bottom_7->Active &&
       ClientSocket_Bottom_7->Address=="172.16.8.200" &&
       ClientSocket_Bottom_7->Port==5017 &&
       ClientSocket_Bottom_8->Active &&
       ClientSocket_Bottom_8->Address=="172.16.8.200" &&
       ClientSocket_Bottom_8->Port==5018)
    {
        switch(Task2)
        {
            case 1:
                SetSFCCheckStepCount();
                #ifdef SOFT_SIMULTE
                    dVisionVer=2;
                #else
                    dVisionVer=1;
                #endif
                Str.sprintf("VR");
                SendCCDCommand(iBottom_5, "Get Version", Str);                  //Reset CCD Shuttle暫存器
                bSFCGetVR=false;
                tChangeSetupFile.SetSecAndOn(30);
                Task2=100;

            case 100:
                if(bSFCGetVR==true)
                {
                    if(TestIF_File.bEnableBarCode)
                        Task2=1000;
                    else if(TestIF_File.bEnableShtFloatChk)
                        Task2=2000;
                    else
                        Task2=3000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task2=1;
                }
                break;
            case 1000:                                                          //Reset CCD Shuttle暫存器
                bGetSE9[iBottom_5]=false;
                bGetSE9[iBottom_6]=false;                                       //KaiHuang 20200910 : Add
                bGetSE9[iBottom_7]=false;                                       //KaiHuang 20200910 : Add
                bGetSE9[iBottom_8]=false;                                       //KaiHuang 20200910 : Add

                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    Str.sprintf("E9,0,%d", InArmSuck.iShtCol);
                else
                    Str.sprintf("SE0,9,%d", InArmSuck.iShtCol);

                SendCCDCommand(iBottom_5, "Clear buffer", Str);
                SendCCDCommand(iBottom_6, "Clear buffer", Str);
                SendCCDCommand(iBottom_7, "Clear buffer", Str);
                SendCCDCommand(iBottom_8, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(1000);
                Task2=1050;

            case 1050:
                if(bGetSE9[iBottom_5] && bGetSE9[iBottom_6] && bGetSE9[iBottom_7] && bGetSE9[iBottom_8])    //KaiHuang 20200910 : Add 6~8
                    Task2=1100;
                else if(tChangeSetupFile.Off())
                    Task2=1000;
                break;
            case 1100:                                                          //取得工作檔名稱
                bGetGF=false;
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    SendCCDCommand(iBottom_5, "Get File Name", "GF,0");
                else
                    SendCCDCommand(iBottom_5, "Get File Name", "GF");
                tChangeSetupFile.SetSecAndOn(10);
                Task2=1150;
                //break;
            case 1150:
                if(bGetGF)
                {
                    Task2=1200;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task2=1100;
                }
                break;
            case 1200:
                bGetLF=false;
                if(BOTTOM_2DID && TestIF_File.bEnableBottom2D)                  //Steven 20190524 : Add bottom 2DID
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,1", fMain->cbSetupFileName->Text);
                    else
                        Str.sprintf("LF%s,1", fMain->cbSetupFileName->Text);
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,0", fMain->cbSetupFileName->Text);
                    else
                        Str.sprintf("LF%s,0", fMain->cbSetupFileName->Text);
                }
                SendCCDCommand(iBottom_5, "Change File", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task2=1250;
            case 1250:
                if(bGetLF)
                {
                    Task2=2000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task2=1200;
                }
                break;
            case 2000:
                if(TestIF_File.bEnableShtFloatChk)
                    Task2=2100;
                else
                    Task2=3000;
                break;
            case 2100:
                bSFCGetSE9[iBottom_5]=false;
                bSFCGetSE9[iBottom_6]=false;                                    //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_7]=false;                                    //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_8]=false;                                    //KaiHuang 20200910 : Add
                Str.sprintf("E9,1,%d", iSFCTotalMoveStep);

                SendCCDCommand(iBottom_5, "Clear buffer", Str);
                SendCCDCommand(iBottom_6, "Clear buffer", Str);
                SendCCDCommand(iBottom_7, "Clear buffer", Str);
                SendCCDCommand(iBottom_8, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task2=2150;

            case 2150:
                if(bSFCGetSE9[iBottom_5] && bSFCGetSE9[iBottom_6] && bSFCGetSE9[iBottom_7] && bSFCGetSE9[iBottom_8])    //KaiHuang 20200910 : Add 6~8
                    Task2=2200;
                else if(tChangeSetupFile.Off())
                    Task2=2100;
                break;
            case 2200:                                                          //取得工作檔名稱
                bSFCGetGF=false;
                SendCCDCommand(iBottom_5, "Get File Name", "GF,1");
                tChangeSetupFile.SetSecAndOn(10);
                Task2=2250;

            case 2250:
                if(bSFCGetGF)
                {
                    if(fMain->cbSetupFileName->Text.UpperCase().AnsiCompare(sSFCCurrentGF)==0)
                    {
                        Task2=3000;
                        bSFCGetLF=true;
                    }
                    else
                    {
                        Task2=2300;
                    }
                }
                else if(tChangeSetupFile.Off())
                {
                    Task2=2200;
                }
                break;
            case 2300:
                bSFCGetLF=false;
                SendCCDCommand(iBottom_5, "SFC Change File", "LF,1,"+fMain->cbSetupFileName->Text);
                tChangeSetupFile.SetSecAndOn(10);
                Task2=2350;
            case 2350:
                if(bSFCGetLF)
                {
                    Task2=3000;
                }
                else if(tChangeSetupFile.Off())
                {
                    Task2=2300;
                }
                break;
            case 3000:
                break;
        }
    }

    bTimerRun=false;                                                            //保持在最下面
}
//---------------------------------------------------------------------------
bool TfBarCode::DoBottom2DID_8CCD_Scan()                                        //KaiChen 20200513 : Bottom 2D 8CCD
{
    int &Task=iBottom2DID_8CCDTask;
    int ret, iPos=0, l, m;
    int iIndex;
    static bool bHasErr[2][4];
    static int iRetryCount=0, iStepCount;
    static int iRetryCountExpTimo=0;                                            //Steven 20160425 : 修改拍照Time Out
    static bool bHasError=false;
    static bool bLotCheckError=false;                                           //Alick 20170313 (wei) add
    static bool bHasDupError=false;
    static bool bLotHasDupError=false;
    static AnsiString ErrPart;
    static bool bNeesScanCode[2][4];
    static AnsiString sErrorPart="", sSimuCode[8];
    AnsiString cLastString, sCCDCommand;
    AnsiString Log;
    AnsiString Str, Str1, Str2, Str3;

    //KaiHuang Add 20200910 : 斷線要重跑流程
    //==>
    if(ClientSocket_Bottom_1->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_1, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 1 , Please Wait !!!");
        return false;
    }
    if(ClientSocket_Bottom_2->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_2, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 2 , Please Wait !!!");
        return false;
    }
    if(ClientSocket_Bottom_3->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_3, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 3 , Please Wait !!!");
        return false;
    }
    if(ClientSocket_Bottom_4->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_4, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 4 , Please Wait !!!");
        return false;
    }
    if(ClientSocket_Bottom_5->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_5, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 5 , Please Wait !!!");
        return false;
    }
    if(ClientSocket_Bottom_6->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_6, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 6 , Please Wait !!!");
        return false;
    }
    if(ClientSocket_Bottom_7->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_7, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 7 , Please Wait !!!");
        return false;
    }
    if(ClientSocket_Bottom_8->Active==false)
    {
        Task=1;
        AddCCDCommunicationLog(iBottom_8, "<<Disconnect, Reset Task");
        ShowMyMessage("Bottom 2D Disconnect - 8 , Please Wait !!!");
        return false;
    }
    //<==

    switch(Task)
    {
        case 1:                                                                 //jou 20170509 (Steven) : 2D alway retry
            iRetryCount=0;
            iStepCount=0;
            bBottom2DNeedMoveInArm=false;
            ZeroMemory(bNeesScanCode, sizeof(bNeesScanCode));
            if(InArmSuck.HasRealIC()==false)
                return true;
            AddCCDCommunicationLog(iBottom_1, " ");
            AddCCDCommunicationLog(iBottom_1, "<<Cycle, Cycle start.------------------");
            Task=100;
        case 100:
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(InArmSuck.Item[i][j]!=HAS_NULL_IC && InArmSuck.Item[i][j]!=NULL_IC)
                        bNeedScanBottom2D[i][j]=true;
                    else
                        bNeedScanBottom2D[i][j]=false;

                    mtBarcodeInSh->SetCellNumber(j, i+1, "");
                    InArmSuck.cDeviceInf[i][j]="";
                }
            }
            Task=200;
        case 200:
            CheckWhichKitBottom2DID();

            if(dVisionVer>=2)                                                   //wei 20170119 (Steven) 版本號大於2
                sCCDCommand.sprintf("E9,0,%d", 1);                              //RogerYang 20190319 四吸嘴 1支ccd會有兩筆資料
            else
                sCCDCommand.sprintf("SE0,9,%d", 1);                             //清空暫存器     //RogerYang 20190319 四吸嘴 1支ccd在GV前會有兩筆資料

            for(int i=0; i<BAR_CODE_COUNT; i++)
            {
                iCurrentStep[i]=0;                                              //Steven 20231018 : fixed for bottom 2D scan
                bStepError[i]=false;                                            //Steven 20160503 : 確認移動位置與拍照位置相同
                bCCDBarcodeGetResultOK[i]=true;
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                {
                    sSimuCode[i]="GV,0,1,1,";
                }
                else
                {
                    sSimuCode[i]="GV,1,1,";
                }
                bGetSE9[i]=false;
                SendCCDCommand(i, "Clear buffer", sCCDCommand);                 //Reset CCD Shuttle暫存器
            }

            if(bBottom2DNeedMoveInArm)
                MoveInArmXY_ToBottom2DID();
            Bottom2DPosDelay.SetMSAndOn(5000);
            Task=500;
        case 500:
            if(bGetSE9[iBottom_1] && bGetSE9[iBottom_2] && bGetSE9[iBottom_3] && bGetSE9[iBottom_4] &&
               bGetSE9[iBottom_5] && bGetSE9[iBottom_6] && bGetSE9[iBottom_7] && bGetSE9[iBottom_8])
            {
                Task=1000;
            }
            #ifdef SOFT_SIMULTE
            else if(cbBarCodeSimulate->Checked)
            {
                Task=1000;
            }
            #endif
            else if(Bottom2DPosDelay.Off())
            {
                ShowErrorMessage("WAR0462", K_RETRY, MInArmX, false, "Clear buffer");
                Task=1;
            }
            break;
        case 1000:
            if(bBottom2DNeedMoveInArm)
            {
                ret=MoveInArmXY_ToBottom2DID();
            }
            else
            {
                ret=true;
            }

            if(ret)
            {
                if((AccessLevel>=1 || CosFunction.bOPCanPressStepAndTStart) &&  //JerryYang 20170417 (wei) OP權限也能按Step跟T.Start
                   Sen[SnRKManualTStart].IsOn())
                {
                    Task=1050;
                    fMain->Pause("DoBottom2DID_8CCD_Scan 1000");
                    break;
                }

                Log.sprintf("Action, In arm move to kit 8 CCD");
                AddCCDCommunicationLog(iBottom_1, Log);

                Bottom2DPosDelay.SetMSAndOn(TestIF_File.iBarCodePosDelay);
                ZeroMemory(bStepError, sizeof(bStepError));
                iRetryCountExpTimo=0;                                           //Steven 20160425 : 修改拍照Time Out

                for(int i=0; i<BAR_CODE_COUNT; i++)                             //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成 Retry 送錯 cmd 卡死
                    bCCDBarcodeExposureError[i]=false;

                Task=1120;
            }
            break;
        case 1050:
            Task=1000;
            break;

        case 1120:
            if(Bottom2DPosDelay.Off())                                          //到位Delay
            {
                //==================================================================
                if(bCCDBarcodeExposureOK[0]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_1, "exposure 1100: ", sCCDCommand);
                //==================================================================
                if(bCCDBarcodeExposureOK[1]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_2, "exposure 1100: ", sCCDCommand);
                //==================================================================
                if(bCCDBarcodeExposureOK[2]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_3, "exposure 1100: ", sCCDCommand);
                //==================================================================
                if(bCCDBarcodeExposureOK[3]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_4, "exposure 1100: ", sCCDCommand);
                //==================================================================
                if(bCCDBarcodeExposureOK[4]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_5, "exposure 1100: ", sCCDCommand);
                //==================================================================
                if(bCCDBarcodeExposureOK[5]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_6, "exposure 1100: ", sCCDCommand);
                //==================================================================
                if(bCCDBarcodeExposureOK[6]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_7, "exposure 1100: ", sCCDCommand);
                //==================================================================
                if(bCCDBarcodeExposureOK[7]==false)
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,1,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,1,%d", 0);
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
                }
                else
                {
                    if(dVisionVer>=2)                                           //wei 20170119 (Steven) 版本號大於2
                        sCCDCommand.sprintf("E0,0,0,%d", 0);
                    else
                        sCCDCommand.sprintf("SE0,0,%d", 0);
                }
                SendCCDCommand(iBottom_8, "exposure 1120: ", sCCDCommand);      //KaiHuang 20200818 : exposure 1100 →1120
                //==================================================================

                Task=1150;
                Bottom2DExposureDelay.SetMSAndOn(TestIF_File.i2DTriggerTime);
            }
            break;
        case 1150:
            if(bStepError[iBottom_1]==true || bStepError[iBottom_2]==true ||
               bStepError[iBottom_3]==true || bStepError[iBottom_4]==true ||
               bStepError[iBottom_5]==true || bStepError[iBottom_6]==true ||
               bStepError[iBottom_7]==true || bStepError[iBottom_8]==true)
            {
                ErrPart="";
                if(bStepError[iBottom_1])
                    ErrPart+=InArmSuck.Suck[0][0].sName;
                if(bStepError[iBottom_2])
                    ErrPart+=InArmSuck.Suck[1][0].sName;
                if(bStepError[iBottom_3])
                    ErrPart+=InArmSuck.Suck[0][1].sName;
                if(bStepError[iBottom_4])
                    ErrPart+=InArmSuck.Suck[1][1].sName;
                if(bStepError[iBottom_5])
                    ErrPart+=InArmSuck.Suck[0][2].sName;
                if(bStepError[iBottom_6])
                    ErrPart+=InArmSuck.Suck[1][2].sName;
                if(bStepError[iBottom_7])
                    ErrPart+=InArmSuck.Suck[0][3].sName;
                if(bStepError[iBottom_8])
                    ErrPart+=InArmSuck.Suck[1][3].sName;

                Task=1180;
            }
            else if(bCCDBarcodeExposureOK[iBottom_1]==true && bCCDBarcodeExposureOK[iBottom_2]==true &&
                    bCCDBarcodeExposureOK[iBottom_3]==true && bCCDBarcodeExposureOK[iBottom_4]==true &&
                    bCCDBarcodeExposureOK[iBottom_5]==true && bCCDBarcodeExposureOK[iBottom_6]==true &&
                    bCCDBarcodeExposureOK[iBottom_7]==true && bCCDBarcodeExposureOK[iBottom_8]==true)
            {
                Task=1200;
                bNeedScanBottom2D[0][0]=!bCCDBarcodeExposureOK[iBottom_1];
                bNeedScanBottom2D[1][0]=!bCCDBarcodeExposureOK[iBottom_2];
                bNeedScanBottom2D[0][1]=!bCCDBarcodeExposureOK[iBottom_3];
                bNeedScanBottom2D[1][1]=!bCCDBarcodeExposureOK[iBottom_4];
                bNeedScanBottom2D[0][2]=!bCCDBarcodeExposureOK[iBottom_5];
                bNeedScanBottom2D[1][2]=!bCCDBarcodeExposureOK[iBottom_6];
                bNeedScanBottom2D[0][3]=!bCCDBarcodeExposureOK[iBottom_7];
                bNeedScanBottom2D[1][3]=!bCCDBarcodeExposureOK[iBottom_8];

                if(InArmSuck.Item[0][0]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(0, 1, "Waiting result..");
                if(InArmSuck.Item[1][0]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(0, 2, "Waiting result..");
                if(InArmSuck.Item[0][1]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(1, 1, "Waiting result..");
                if(InArmSuck.Item[1][1]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(1, 2, "Waiting result..");
                if(InArmSuck.Item[0][2]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(2, 1, "Waiting result..");
                if(InArmSuck.Item[1][2]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(2, 2, "Waiting result..");
                if(InArmSuck.Item[0][3]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(3, 1, "Waiting result..");
                if(InArmSuck.Item[1][3]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC)
                    mtBarcodeInSh->SetCellNumber(3, 2, "Waiting result..");
            }
            #ifdef SOFT_SIMULTE
            else if(cbBarCodeSimulate->Checked)
            {
                sSimuCode[iBottom_1]+=GetSimuBarCodeName(iBottom_1, iBottomKit)+",";
                sSimuCode[iBottom_2]+=GetSimuBarCodeName(iBottom_2, iBottomKit)+",";
                sSimuCode[iBottom_3]+=GetSimuBarCodeName(iBottom_3, iBottomKit)+",";
                sSimuCode[iBottom_4]+=GetSimuBarCodeName(iBottom_4, iBottomKit)+",";
                sSimuCode[iBottom_5]+=GetSimuBarCodeName(iBottom_5, iBottomKit)+",";
                sSimuCode[iBottom_6]+=GetSimuBarCodeName(iBottom_6, iBottomKit)+",";
                sSimuCode[iBottom_7]+=GetSimuBarCodeName(iBottom_7, iBottomKit)+",";
                sSimuCode[iBottom_8]+=GetSimuBarCodeName(iBottom_8, iBottomKit)+",";
                Task=1200;
            }
            #endif
            else if(Bottom2DExposureDelay.Off())                                //拍照time out
            {
                ErrPart="";
                if(bCCDBarcodeExposureOK[iBottom_1]==false)
                {
                    mtBarcodeInSh->SetCellNumber(0, 1, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_1, Log);
                    ErrPart+=InArmSuck.Suck[0][0].sName;
                    bCCDBarcodeExposureError[iBottom_1]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }

                if(bCCDBarcodeExposureOK[iBottom_2]==false)
                {
                    mtBarcodeInSh->SetCellNumber(0, 2, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_2, Log);
                    ErrPart+=InArmSuck.Suck[1][0].sName;
                    bCCDBarcodeExposureError[iBottom_2]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }

                if(bCCDBarcodeExposureOK[iBottom_3]==false)
                {
                    mtBarcodeInSh->SetCellNumber(1, 1, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_3, Log);
                    ErrPart+=InArmSuck.Suck[0][1].sName;
                    bCCDBarcodeExposureError[iBottom_3]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }

                if(bCCDBarcodeExposureOK[iBottom_4]==false)
                {
                    mtBarcodeInSh->SetCellNumber(1, 2, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_4, Log);
                    ErrPart+=InArmSuck.Suck[1][1].sName;
                    bCCDBarcodeExposureError[iBottom_4]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }

                if(bCCDBarcodeExposureOK[iBottom_5]==false)
                {
                    mtBarcodeInSh->SetCellNumber(2, 1, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_5, Log);
                    ErrPart+=InArmSuck.Suck[0][2].sName;
                    bCCDBarcodeExposureError[iBottom_5]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }

                if(bCCDBarcodeExposureOK[iBottom_6]==false)
                {
                    mtBarcodeInSh->SetCellNumber(2, 2, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_6, Log);
                    ErrPart+=InArmSuck.Suck[1][2].sName;
                    bCCDBarcodeExposureError[iBottom_6]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }

                if(bCCDBarcodeExposureOK[iBottom_7]==false)
                {
                    mtBarcodeInSh->SetCellNumber(3, 1, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_7, Log);
                    ErrPart+=InArmSuck.Suck[0][3].sName;
                    bCCDBarcodeExposureError[iBottom_7]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }

                if(bCCDBarcodeExposureOK[iBottom_8]==false)
                {
                    mtBarcodeInSh->SetCellNumber(3, 2, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDCommunicationLog(iBottom_8, Log);
                    ErrPart+=InArmSuck.Suck[1][3].sName;
                    bCCDBarcodeExposureError[iBottom_8]=true;                   //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成送錯 cmd 卡死
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }
            break;
        case 1160:
            if(iRetryCountExpTimo<3)
            {
                for(int i=0; i<BAR_CODE_COUNT; i++)                             //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成 Retry 送錯 cmd 卡死
                {
                    if(bCCDBarcodeExposureOK[i]==true && bCCDBarcodeExposureError[i]==true)
                    {
                        bCCDBarcodeExposureOK[i]=false;
                        bCCDBarcodeExposureError[i]=false;
                    }
                }

                Task=1170;
            }
            else
            {
                ret=ShowErrorMessage("WAR0464", K_RETRY|K_SKIP, MInArmX, false, ErrPart);  //2D function CCD 1 or 2 Exposure time out!
                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    for(int i=0; i<BAR_CODE_COUNT; i++)                         //KaiHuang 20200824 : 避免 Exposure Time Out 時又收到 2D 回應,造成 Retry 送錯 cmd 卡死
                    {
                        if(bCCDBarcodeExposureOK[i]==true && bCCDBarcodeExposureError[i]==true)
                        {
                            bCCDBarcodeExposureOK[i]=false;
                            bCCDBarcodeExposureError[i]=false;
                        }
                    }
                    Task=1120;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            //==================================================================
            if(bCCDBarcodeExposureOK[0]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_1, "exposure 1100: ", sCCDCommand);
            //==================================================================
            if(bCCDBarcodeExposureOK[1]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_2, "exposure 1100: ", sCCDCommand);
            //==================================================================
            if(bCCDBarcodeExposureOK[2]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_3, "exposure 1100: ", sCCDCommand);
            //==================================================================
            if(bCCDBarcodeExposureOK[3]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_4, "exposure 1100: ", sCCDCommand);
            //==================================================================
            if(bCCDBarcodeExposureOK[4]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_5, "exposure 1100: ", sCCDCommand);
            //==================================================================
            if(bCCDBarcodeExposureOK[5]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_6, "exposure 1100: ", sCCDCommand);
            //==================================================================
            if(bCCDBarcodeExposureOK[6]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_7, "exposure 1100: ", sCCDCommand);
            //==================================================================
            if(bCCDBarcodeExposureOK[7]==false)
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,1,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,1,%d", 0);
                mtBarcodeInSh->SetCellNumber(0, 1, "Exposure...");
            }
            else
            {
                if(dVisionVer>=2)                                               //wei 20170119 (Steven) 版本號大於2
                    sCCDCommand.sprintf("E0,0,0,%d", 0);
                else
                    sCCDCommand.sprintf("SE0,0,%d", 0);
            }
            SendCCDCommand(iBottom_8, "exposure 1100: ", sCCDCommand);
            //==================================================================

            Task=1150;
            Bottom2DExposureDelay.SetMSAndOn(TestIF_File.i2DTriggerTime);
            break;
        case 1180:
            Log.sprintf("Alarm, In arm exposure position error!");
            AddCCDCommunicationLog(iBottom_1, Log);
            ShowErrorMessage("WAR0469", K_RETRY, MInArmX, false, ErrPart);         //In shuttle 1 exposure position error!
            Task=1;
            break;
        case 1200:
            Task=1300;
            break;
        case 1300:
            if(dVisionVer>=2)                                                   //wei 20170119 (Steven) 版本號大於2
                sCCDCommand="GV,0";
            else
                sCCDCommand="GV0";                                              //Ifor 20151228 取得 CCD 讀取資料
            ZeroMemory(bCCDBarcodeGetResultOK, sizeof(bCCDBarcodeGetResultOK));
            //==========================================================================
            // iBottom_1
            //======================================================================
            if(InArmSuck.Item[0][0]!=HAS_NULL_IC && InArmSuck.Item[0][0]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_1]=false;
                SendCCDCommand(iBottom_1, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_1]=true;
            }
            //==========================================================================
            // iBottom_2
            //======================================================================
            if(InArmSuck.Item[1][0]!=HAS_NULL_IC && InArmSuck.Item[1][0]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_2]=false;
                SendCCDCommand(iBottom_2, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_2]=true;
            }
            //==========================================================================
            // iBottom_3
            //======================================================================
            if(InArmSuck.Item[0][1]!=HAS_NULL_IC && InArmSuck.Item[0][1]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_3]=false;
                SendCCDCommand(iBottom_3, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_3]=true;
            }
            //==========================================================================
            // iBottom_4
            //======================================================================
            if(InArmSuck.Item[1][1]!=HAS_NULL_IC && InArmSuck.Item[1][1]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_4]=false;
                SendCCDCommand(iBottom_4, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_4]=true;
            }
            //==========================================================================
            // iBottom_5
            //======================================================================
            if(InArmSuck.Item[0][2]!=HAS_NULL_IC && InArmSuck.Item[0][2]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_5]=false;
                SendCCDCommand(iBottom_5, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_5]=true;
            }
            //==========================================================================
            // iBottom_6
            //======================================================================
            if(InArmSuck.Item[1][2]!=HAS_NULL_IC && InArmSuck.Item[1][2]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_6]=false;
                SendCCDCommand(iBottom_6, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_6]=true;
            }
            //==========================================================================
            // iBottom_7
            //======================================================================
            if(InArmSuck.Item[0][3]!=HAS_NULL_IC && InArmSuck.Item[0][3]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_7]=false;
                SendCCDCommand(iBottom_7, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_7]=true;
            }
            //==========================================================================
            // iBottom_8
            //======================================================================
            if(InArmSuck.Item[1][3]!=HAS_NULL_IC && InArmSuck.Item[1][3]!=NULL_IC)
            {
                bCCDBarcodeGetResultOK[iBottom_8]=false;
                SendCCDCommand(iBottom_8, "get result 1130: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBottom_8]=true;
            }
            //======================================================================

            #ifdef SOFT_SIMULTE
            if(cbBarCodeSimulate->Checked)
            {
                Socket2DReceiveList[iBottom_1]->Add(sSimuCode[iBottom_1]);
                Socket2DReceiveList[iBottom_2]->Add(sSimuCode[iBottom_2]);
                Socket2DReceiveList[iBottom_3]->Add(sSimuCode[iBottom_3]);
                Socket2DReceiveList[iBottom_4]->Add(sSimuCode[iBottom_4]);
                Socket2DReceiveList[iBottom_5]->Add(sSimuCode[iBottom_5]);
                Socket2DReceiveList[iBottom_6]->Add(sSimuCode[iBottom_6]);
                Socket2DReceiveList[iBottom_7]->Add(sSimuCode[iBottom_7]);
                Socket2DReceiveList[iBottom_8]->Add(sSimuCode[iBottom_8]);
            }
            #endif

            bHasError=false;
            Task=1350;
            Bottom2DDelay.SetMSAndOn(TestIF_File.iBarCodeDelay);                //這個是讀取Time Out用的
        case 1350:
            if(bCCDBarcodeGetResultOK[iBottom_1]==true && bCCDBarcodeGetResultOK[iBottom_2]==true &&
               bCCDBarcodeGetResultOK[iBottom_3]==true && bCCDBarcodeGetResultOK[iBottom_4]==true &&
               bCCDBarcodeGetResultOK[iBottom_5]==true && bCCDBarcodeGetResultOK[iBottom_6]==true &&
               bCCDBarcodeGetResultOK[iBottom_7]==true && bCCDBarcodeGetResultOK[iBottom_8]==true)      //讀取完成
            {
                sErrorPart="";
                for(int i=0; i<InArmSuck.iMaxRow; i++)                          //清空畫面
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Item[i][j]!=HAS_NULL_IC && InArmSuck.Item[i][j]!=NULL_IC)
                        {
                            //KaiHuang 20200826 : Fix
                            //==>
                            if(InArmSuck.cDeviceInf[i][j]==asBarCodeErrorSend || InArmSuck.cDeviceInf[i][j]=="")    //wei 20160318 Barcode Error依客戶設定
                            {
                                bHasError=true;
                                sErrorPart+=InArmSuck.Suck[i][j].sName;
                                if(CUSTOMER_CODE==CC_ASE_CL)
                                {
                                    sErrorPart+="(Err)";
                                }
                            }
                            else if(InArmSuck.cDeviceInf[i][j]==asBarCodeErrorCheckSum)
                            {
                                bHasError=true;
                                sErrorPart+=InArmSuck.Suck[i][j].sName;
                                if(CUSTOMER_CODE==CC_ASE_CL)
                                {
                                    sErrorPart+="(Sum)";
                                }
                            }
                            //<==
                        }
                    }
                }

                Task=2000;
            }
            else if(Bottom2DDelay.Off())                                        //讀取超時
            {
                sErrorPart="";
                for(int i=0; i<8; i++)
                {
                    if(bCCDBarcodeGetResultOK[iBottom_1+i]==false)
                    {
                        bHasError=true;
                        bCCDBarcodeGetResultOK[iBottom_1+i]=true;
                        AddCCDCommunicationLog(iBottom_1+i, "Action 1350, Decode time out.");
                    }
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iBarcodeRetryCount)                  //Steven 20160421 : 修正判斷方式
                {
                    Task=100;
                    iRetryCount++;                                              //Steven 20190327 : 修正Bottom 2D讀取Retry
                }
                else
                {
                    if(TestIF_File.bNoCodeDeviceAutoSkip &&
                       fContact->IsRun2DCheck()==false)                         //JerryYang 20250220 : 2DID硬體順序檢查功能
                    {
                        AddCCDCommunicationLog(iBarCode1_1, ">>Cycle 2000, Decode end with auto skip.---------------\r\n");
                        if(TestIF_File.bCheckCodeByShuttle==false && TestIF_File.bCheckCodeByLot==false)
                        {
                            Task=5000;
                        }
                        else
                        {
                            if(TestIF_File.bCheckLotHaveCode)                   //Steven 20170707 (wei) : Fixed 2DID for Korea
                                Task=3500;                                      //Alick 20170313 (wei) modify 4000==>3500
                            else
                                Task=4000;
                        }
                    }
                    else
                    {
                        Task=2500;
                    }
                }
                bHasError=false;
            }
            else
            {
                AddCCDCommunicationLog(iBottom_1, ">>Cycle 2000, Decode end.---------------\r\n");
                if(TestIF_File.bCheckCodeByShuttle==false && TestIF_File.bCheckCodeByLot==false)
                {
                    Task=5000;
                }
                else
                {
                    if(TestIF_File.bCheckLotHaveCode)                           //Steven 20170707 (wei) : Fixed 2DID for Korea
                        Task=3500;                                              //Alick 20170313 (wei) modify 4000==>3500
                    else
                        Task=4000;
                }
            }
            break;
        case 2500:
            Task=2600;
//            break;
        case 2600:
            Log.sprintf("Alarm 2600, In arm Barcode Error");
            AddCCDCommunicationLog(iBottom_1, Log);
            if(TestIF_File.bCheckCodeByLot &&
               iRetryCount<=TestIF_File.iBarcodeRetryCount)                     //Alick 20170124 add Retry時bRetryOffsetMove=true先退出去  //Alick 20170315 (wei) modify bRetryOffsetMove=>bCheckCodeByLot
            {
                bHasError=false;
                iRetryCount++;
                Task=100;                                                       //jou 20170509 (Steven) : 2D alway retry
            }
            else
            {
                if(fContact->IsRun2DCheck()==true)                              //JerryYang 20250220 : 2DID硬體順序檢查功能
                {
                    ret=ShowErrorMessage("JAM0497", K_RETRY, MInArmX, false, sErrorPart);      //In Shuttle1 Barcode Error
                }
                else
                {
                    ret=ShowErrorMessage("JAM0460", K_RETRY|K_SKIP, MInArmX, false, sErrorPart);      //In Shuttle1 Barcode Error
                }
                iRetryCount=0;
                bHasError=false;
                if(ret==K_RETRY)
                {
                    Task=100;
                }
                else
                {
                    AddCCDCommunicationLog(iBottom_1, ">>Cycle 2600, Decode end with barcode error.---------------\r\n");
                    if(TestIF_File.bCheckCodeByShuttle==false && TestIF_File.bCheckCodeByLot==false)
                    {
                        Task=5000;
                    }
                    else
                    {
                        if(TestIF_File.bCheckLotHaveCode)                       //Steven 20170707 (wei) : Fixed 2DID for Korea
                            Task=3500;                                          //Alick 20170313 (wei) modify 4000==>3500
                        else
                            Task=4000;
                    }
                }
            }
            break;
        case 3500:
            if(TestIF_File.bCheckLotHaveCode)
            {
                FILE *P=NULL;
                char t[256];
                ZeroMemory(bHasErr, sizeof(bHasErr));
                bLotCheckError=false;
                if(FileExists(asBarCodeDownLot)!=false)
                {
                    if(LastSet.iTester==OFF_LINE)                               //Frank 20170611 (Steven) add Alex 多種Dummy IC 需要同時辨識
                    {
                        fBarCode->list2DByLot->Clear();
                        fBarCode->list2DByLot->LoadFromFile(asBarCodeDownLot);
                        fBarCode->list2DByLot->Sort();
                    }
                    else
                    {
                        P=fopen(asBarCodeDownLot.c_str(), "r");
                        if(P!=NULL)
                        {
                            fgets(t, 30, P);
                            Str1=t;
                            asBarcodeLotNumber="     Lot ID : "+Str1;           //wei Barcode Lot Alarm時，顯示出Lot
                            fclose(P);
                        }

                        iPos=Str1.Pos("\n");                                    //Frank 20170426 (Steven) add 消除"\n"為一個字元的寬度
                        if(iPos>0)
                            Str3=Str1.SubString(1, iPos-1);
                        else
                            Str3=Str1;
                    }

                    for(int i=0; i<InArmSuck.iMaxRow; i++)
                    {
                        for(int j=0; j<InArmSuck.iMaxCol; j++)
                        {
                            if(InArmSuck.Item[i][j]!=HAS_NULL_IC && InArmSuck.Item[i][j]!=NULL_IC &&
                               InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorSend && InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorCheckSum &&  //KaiHuang 20200826 : Add CheckSum
                               InArmSuck.cDeviceInf[i][j]!="")
                            {
                                Str=InArmSuck.cDeviceInf[i][j];
                                Str2=Str.SubString(1, 9);

                                if(!fBarCode->list2DByLot->Find(Str2, iIndex) && LastSet.iTester==OFF_LINE)     //Frank 20170611 (Steven) add Alex 多種Dummy IC 需要同時辨識
                                {
                                    bLotCheckError=true;
                                    bHasErr[i][j]=true;
                                }
                                else if(Str2!=Str3)
                                {
                                    bLotCheckError=true;
                                    bHasErr[i][j]=true;
                                }
                            }
                        }
                    }

                    if(bLotCheckError)
                    {
                        Task=3600;
                    }
                    else
                    {
                        Task=4000;
                    }
                }
                else
                {
                    ShowErrorMessage("WAR0482", K_RETRY, MMSystem);
                }
            }
            else
            {
                Task=4000;
            }
            break;
        case 3600:
            fNote->t2DCode->Visible=true;
            fNote->t2DCode->XItem=2;
            fNote->t2DCode->YItem=4;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, InArmSuck.cDeviceInf[i][j].c_str());
                    if(bHasErr[i][j]==true)
                        fNote->t2DCode->SetCellColorIndex(i, j, 3);
                    else
                        fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }

            Log.sprintf("WAR0471, In arm check have Lot barcode error!\r\n");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            if(bLotCheckError==true)
            {
                ret=ShowErrorMessage("WAR0471", K_RETRY, MInArmX, false);       //In Shuttle1 Barcode Error  //Alick 20170202 remove skip
            }

            if(ret==K_RETRY)
            {
                Task=1;
            }
            break;
        case 4000:                                                              //Steven 20160428 : 檢查蝦頭的2D重複碼
            if(TestIF_File.bCheckCodeByShuttle || TestIF_File.bCheckCodeByLot)  //Steven 20171128 (Wei) : check lot之前一定要先確認蝦頭內沒有重複的
            {
                map2DShuttleList[iBarCode1_1].clear();
                ZeroMemory(bHasErr, sizeof(bHasErr));
                bHasDupError=false;
                ErrPart="";
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Item[i][j]!=HAS_NULL_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorSend  &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorCheckSum &&
                           InArmSuck.cDeviceInf[i][j]!="")
                        {
                            mapShtIter[iBarCode1_1]=map2DShuttleList[iBarCode1_1].find(InArmSuck.cDeviceInf[i][j]);
                            if(mapShtIter[iBarCode1_1]==map2DShuttleList[iBarCode1_1].end())
                            {
                                map2DShuttleList[iBarCode1_1][InArmSuck.cDeviceInf[i][j]]=i*4+j;
                            }
                            else                                                //有重複的就不用加進去了
                            {
                                iBarcodeDuplicate[iBarCode1_1]++;
                                bHasDupError=true;
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                                bHasErr[i][j]=true;
                                ret=atoi(map2DShuttleList[iBarCode1_1][InArmSuck.cDeviceInf[i][j]].c_str());   //Steven 20160503 : 重複的Code要設定為Error
                                ErrPart+=InArmSuck.Suck[ret/4][ret%4].sName;
                                bHasErr[ret/4][ret%4]=true;
                            }
                        }
                    }
                }

                if(bHasDupError)
                {
                    Task=4050;
                }
                else
                {
                    Task=4100;
                }
            }
            else
            {
                Task=4100;
            }
            break;
        case 4050:
            fNote->t2DCode->Visible=true;
            fNote->t2DCode->XItem=2;
            fNote->t2DCode->YItem=4;

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, InArmSuck.cDeviceInf[i][j].c_str());
                    if(bHasErr[i][j]==true)
                        fNote->t2DCode->SetCellColorIndex(i, j, 3);
                    else
                        fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }

            Log.sprintf("Alarm 4050, In arm check have duplicate barcode error!\r\n");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            if(TestIF_File.iShtDuplicateRetryCnt<=0 ||                          //Steven 20190520 : 修正等於0時會變成無窮迴圈
               TestIF_File.iShtDuplicateRetryCnt>=iShtDupilcateFailCnt[0])      //Steven 20160823 : 蝦頭重複碼要可以自動Retry
            {
                iShtDupilcateFailCnt[0]=0;
                ret=ShowErrorMessage("WAR0465", K_RETRY|K_SKIP, MInArmX, false, ErrPart);  //In shuttle 1 check have duplicate barcode error!
            }
            else
            {
                iShtDupilcateFailCnt[0]++;
                ret=K_RETRY;
            }

            if(ret==K_SKIP)
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(bHasErr[i][j]==true)                                 //Steven 20160503 : 重複的Code要設定為Error
                        {
                            InArmSuck.cDeviceInf[i][j]=asBarCodeErrorSend;
                        }
                    }
                }
                Task=4100;
            }
            else
            {
                Task=1;
            }
            break;
        case 4100:
            iShtDupilcateFailCnt[0]=0;
            if(TestIF_File.bCheckCodeByLot)
            {
                bLotHasDupError=false;
                ZeroMemory(bHasErr, sizeof(bHasErr));
                ErrPart="";
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Item[i][j]!=HAS_NULL_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorSend &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorCheckSum &&//KaiHuang 20200826 : Add CheckSum
                           InArmSuck.cDeviceInf[i][j]!="")
                        {
                            map2DIter=map2DList.find(InArmSuck.cDeviceInf[i][j]);
                            if(map2DList.size()!=0 && map2DIter!=map2DList.end())
                            {
                                iBarcodeDuplicate[iBarCode1_1]++;
                                bLotHasDupError=true;
                                ErrPart+=InArmSuck.Suck[i][j].sName;
                                bHasErr[0][i]=true;
                            }
                        }
                    }
                }

                if(bLotHasDupError)
                {
                    Task=4150;
                }
                else
                {
                    Task=4200;
                }
            }
            else
            {
                Task=4200;
            }
            break;
        case 4150:
            fNote->t2DCode->Visible=true;
            fNote->t2DCode->XItem=InArmSuck.iMaxRow;
            fNote->t2DCode->YItem=InArmSuck.iMaxCol;
            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    fNote->t2DCode->SetCellNumber(i, j, InArmSuck.cDeviceInf[i][j].c_str());
                    if(bHasErr[i][j]==true)
                        fNote->t2DCode->SetCellColorIndex(i, j, 3);
                    else
                        fNote->t2DCode->SetCellColorIndex(i, j, 0);
                }
            }

            Log.sprintf("Alarm 4150, In Arm check have duplicate barcode in lot error!\r\n");
            AddCCDCommunicationLog(iBarCode1_1, Log);
            ret=ShowErrorMessage("WAR0467", K_RETRY|K_SKIP, MInArmX, false, ErrPart);  //In shuttle 1 check have duplicate barcode in lot error!
            if(ret==K_SKIP)
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(bHasErr[i][j]==true)                                 //Steven 20160503 : 重複的Code要設定為Error
                        {
                            InArmSuck.cDeviceInf[i][j]=asBarCodeErrorSend;
                        }
                    }
                }
                Task=4200;
            }
            else
            {
                Task=1;
            }
            break;
        case 4200:
            if(TestIF_File.bCheckCodeByLot &&                                   //把新的值加到List裡面
               CosFunction.bBarcodeDuplicateFileByOutArm==false)                //jou 20191008 : Barcode duplicate file by out arm
            {
                list2DByLot->Clear();
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Item[i][j]!=HAS_NULL_IC &&
                           InArmSuck.Item[i][j]!=NULL_IC &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorSend &&
                           InArmSuck.cDeviceInf[i][j]!=asBarCodeErrorCheckSum &&//KaiHuang 20200826 : Add CheckSum
                           InArmSuck.cDeviceInf[i][j]!="")
                        {
                            map2DList[InArmSuck.cDeviceInf[i][j]]=i;
                            list2DByLot->Add(InArmSuck.cDeviceInf[i][j]);
                        }
                    }
                }

                WriteDataToFile(asBarCodeLot, list2DByLot->Text);               //Steven 20160429 : 開程式要把2D List讀回來
            }

            if(TestIF_File.bSearch2DIDByLot && LastSet.iTester==OFF_LINE)// && CUSTOMER_CODE==CC_KYEC_XILINX)   //Frank 20170316 (wei) add Search 2DID By Lot
                Task=4500;
            else
                Task=5000;
            break;
        case 4500:                                                              //Frank 20170316 (wei) add Search 2DID By Lot
            if(FileExists(asSearch2DIDByLot))
            {
                list2DByLot->Clear();
                list2DByLot->LoadFromFile(asSearch2DIDByLot);
            }
            else
            {
                //待Frank新增
            }

            list2DByLot->Sort();

            for(int i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(int j=0; j<InArmSuck.iMaxCol; j++)
                {
                    Str=InArmSuck.cDeviceInf[i][j];
                    iPos=Str.Pos("\r\n");
                    if(iPos>0)
                        Str1=Str.SubString(1, iPos-1);
                    else
                        Str1=Str;
                    if(fBarCode->list2DByLot->Find(Str1, iIndex)==true)
                    {
                        InArmSuck.cDeviceInf[i][j]=asBarCodeErrorSend;
                    }
                }
            }
            Task=5000;
            break;
        case 5000:
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<2; j++)
                {
                    for(int k=0; k<2; k++)
                    {
                        l=k*2+j;
                        m=i*2+k;
                        if(InArmSuck.Item[i][l]!=HAS_NULL_IC && InArmSuck.Item[i][l]!=NULL_IC)
                        {
                            iNeedBarcodeCount[m]++;
                            if(InArmSuck.cDeviceInf[i][l]==asBarCodeErrorSend ||
                               InArmSuck.cDeviceInf[i][l]==asBarCodeErrorCheckSum ||  //KaiHuang 20200826 : Add CheckSum
                               InArmSuck.cDeviceInf[i][l]=="")
                                iBarcodeErrorCount[m]++;
                            else
                                iBarcodePassCount[m]++;
                            InArmSuck.PordRec[i][l].Add2DIDRecord(InArmSuck.cDeviceInf[i][l]);
                        }
                    }
                }
            }

            if(DoBarcodeCount())                                                //Steven 20171222 (Wei) : Yield Alarm of 2DID
            {
                Task=5200;
                break;
            }
            else
            {
                Task=5100;
            }
        case 5100:
            Task=1;
            return true;
        case 5200:
            ShowErrorMessage("WAR0483", 0, MMSystem, false, s2DIDYield);
            Task=5100;
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TfBarCode::DoPassword()                                         //JerryYang 20151028 add for Amkor
{
    bool bFlag=true;
    bool bTechComExist=FileExists(pwPath);                                      //2012-01-03    Dell modify

    if(fInput->fShow==false)
    {
        if(bTechComExist)
            fMain->cbUserSelectChange(NULL);
        else
            fMain->stOperatorClick(fMain);

        if(AccessLevel<2)                                                       //Supervise才有權限
        {
            bFlag=false;
            if(bTechComExist)
            {
                fPassword->Label3->Visible=true;
                fPassword->Label4->Visible=true;
            }
        }
        else
        {
            if(bTechComExist)
            {
                fPassword->Label3->Visible=false;
                fPassword->Label4->Visible=false;
            }
        }
    }
    return bFlag;
}
//---------------------------------------------------------------------------
bool TfBarCode::RunCheckBarcodeByServerData()                                   //Jimmychiu 20230925 : read 2did in json file
{
    return ((CosFunction.bRead2DIDFromServer==true) && (TestIF_File.bCheckCodeByServer2DID==true));
}
//---------------------------------------------------------------------------
bool TfBarCode::JCETUseMakeWhite2DIDList()                                      //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
{
    return (TestIF_File.bEnableBarCode==true &&
            CosFunction.bMakeWhite2DIDList==true &&
            TestIF_File.bChkMakeWhite2DIDList==true &&
            TestIF_File.b2DIDAllowList==true);
}
//---------------------------------------------------------------------------
AnsiString TfBarCode::GetBarcodeByServerData()                                  //Jimmychiu 20230925 : read 2did in json file
{
    //target path = FormSetPath+lotid
    return AnsiString().sprintf("%s%s",TestIF_File.asMes2DID_URL,fLotInfo->edtSysLotID->Text);
}
//----------------------------------------------------------------------
bool TfBarCode::GetWhite2DIDList(AnsiString LotID, AnsiString sRunMode)         //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
{
    //檢查是否有相同檔名的list, 如果有就是續測. 下載後接續存檔
    AnsiString sPath, str1, str2, sLocPath;
    TSearchRec sr;
    AnsiString cmd, str;
    AnsiString sCust=LotID.SubString(0,3);                                      //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
    AnsiString sTar=IniConfig.cN23FtpDownloadPath;
    if(sTar.SubString(sTar.Length(),1)!="/")
    {
        sTar=sTar+AnsiString("/");
    }
    sPath.sprintf("%s%s/%s/", sTar, sCust, LotID);

    str1.sprintf("%s_%s.csv", LotID, sRunMode);
    fFTPClient->Download_2DSortingList(sPath, str1, 5000);
    str2.sprintf("%s\\%s_%s.csv", "D:\\RMS", LotID, sRunMode);
    sLocPath.sprintf("%s%s\\%s\\", sWhite2DIDListLoc, sCust, LotID);            //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv

    MyForceDirectories(sLocPath);

    if(FileExists(str2))   //從下載下來的D:\RMS位置搬到指定資料夾
    {
//        檢查本地暫存用的資料夾(清空不屬於此LOT的東西) 20251214 因為改成不只有FT1的資料，所以不清空
//        if(MyForceDirectories(sWhite2DIDListLoc)==1)
//        {
//            if (FindFirst(sWhite2DIDListLoc + "*.*", faAnyFile, sr)==0)
//            {
//                do
//                {
//                     排除子目錄 "." 和 ".."
//                    if ((sr.Attr & faDirectory)==0)
//                    {
//                        AnsiString fileToDelete=sWhite2DIDListLoc+sr.Name;
//                        DeleteFile(fileToDelete);
//                    }
//                } while (FindNext(sr)==0);
//                FindClose(sr);
//            }
//        }

        AnsiString cmd;
        // 使用 COPY 直接複製下載下來的檔案(D:\RMS)到暫存資料夾(sWhite2DIDListLoc)
        cmd.sprintf(
            "cmd.exe /c COPY /Y \"%s\" \"%s\"",
            str2.c_str(),
            sLocPath.c_str()
        );
        WinExec(cmd.c_str(), SW_HIDE);
        MySleep(500); //睡一下等檔案複製下來
        if(FileExists(sLocPath+str1))
        {
            DeleteFile(str2);
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
bool TfBarCode::UpdateWhite2DIDList(AnsiString sLotID, AnsiString sRunMode)     //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
{
    if(fLotInfo->cbRunMode->Text.Pos("FT")>0)   //改成都要上傳
    {
        AnsiString sLocPath, sMsg;
        AnsiString sCust=sLotID.SubString(0,3);                                 //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
        sLocPath.sprintf("%s%s\\%s\\", sWhite2DIDListLoc, sCust, sLotID);

        // 本地暫存資料夾
        if (!DirectoryExists(sLocPath))
        {
            sMsg.sprintf("本地2DID白名單資料夾 %s 路徑不存在，無法上傳至FTP", sLocPath);
            ShowMyMessage(sMsg);
            return false;
        }

        if(sLotID.Length()<3)                                                   //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
        {
            sMsg.sprintf("批號 %s 異常，無法取得客戶代碼(前3碼)", sLotID);
            ShowMyMessage(sMsg);
            return false;
        }

        AnsiString sPath, sSrcFile, str;
        AnsiString sFileName;

        // 組出檔名
        sFileName.sprintf("%s_%s.csv", sLotID, sRunMode);
        // 本地檔案完整路徑
        sSrcFile=sLocPath+sFileName;

        // 本地是否有此檔
        if (FileExists(sSrcFile))
        {
            fFTPClient->UploadFileToServer2(IniConfig.cN23FtpDownloadPath, sFileName);
            if(fFTPClient->bError==true)
            {
                sMsg.sprintf("上傳2DID白名單至FTP失敗 %s ", sLocPath);
                ShowMyMessage(sMsg);
            }
            return !fFTPClient->bError;
        }
        else
        {
            sMsg.sprintf("本地2DID白名單無資料 %s ", sSrcFile);
            ShowMyMessage(sMsg);
            return false;
        }
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
void TfBarCode::TransformListToMap()                                            //Steven 20240515 : modified for 2D sort
{
    AnsiString Str, StrKey, StrValue;
    map2DSortingList.clear();

//    list2DSorting->Add("AAA=1");
//    list2DSorting->Add("BBB=2");

    for(int i=0; i<list2DSorting->Count; i++)
    {
        Str=list2DSorting->Strings[i];
        StrKey=Str.SubString(1, Str.AnsiPos("=")-1);
        StrValue=Str.SubString(Str.AnsiPos("=")+1, Str.Length());
        map2DSortingList[StrKey]=atoi(StrValue.c_str());
    }

//    int iValue1=GotBinFrom2DSortList("AAA");
//    int iValue2=GotBinFrom2DSortList("BBB");
//    int iValue3=GotBinFrom2DSortList("CCC");
}
//------------------------------------------------------------------------------
int TfBarCode::GotBinFrom2DSortList(AnsiString s2DID)                           //Steven 20240515 : modified for 2D sort
{
    int iTempBin=iTestBinCount;
    map2DSortIter=map2DSortingList.find(s2DID);
    if(map2DSortIter==map2DSortingList.end())
    {
        iTempBin=iTestBinCount;
    }
    else
    {
        iTempBin=map2DSortIter->second;
    }
    return iTempBin;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::btnGetLotIDClick(TObject *Sender)
{
    int iStart      =atoi(edtLotIDStart->Text.c_str())+1;
    int iEnd        =atoi(edtLotIDEnd->Text.c_str())+1;
    int iLength     =iEnd-iStart+1;
    edtLotIDSubstr->Text=edtLotIDVerify->Text.SubString(iStart, iLength);
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::btnTestLotIDClick(TObject *Sender)
{
//    int iStart      =atoi(edtLotIDStart->Text.c_str());
//    int iEnd        =atoi(edtLotIDEnd->Text.c_str());
    int iLength     =edtLotIDSubstr->Text.Length();
    int iPos        =edtSampleString->Text.AnsiPos(edtLotIDSubstr->Text)-1;

    edtSampleSubstr->Text=edtLotIDSubstr->Text;
    edt2DStart->Text=iPos;
    edt2DEnd->Text  =iPos+iLength-1;
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::ed_2D_YieldIgnoreCntClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 1, 100);     //JerryYang 20241104 : Ignore count變更為可以修改
}
//------------------------------------------------------------------------------
void TfBarCode::InitUnloaderClipEdtList()
{
    bool bShow=true;
    bool bEnable=true;
    bool bReadFromFile=true;
    elUnloaderClip->Add(edClip_Address_Auto1,    &ccdUnloader[eUnloaderClipAuto1].sIP,      ECText,     "UnloaderClip", "UnloaderClip_IP_Auto1",    bShow, bEnable, bReadFromFile, AnsiString("172.16.8.202"));
    elUnloaderClip->Add(edClip_Port_Auto1,       &ccdUnloader[eUnloaderClipAuto1].sPort,    ECText,     "UnloaderClip", "UnloaderClip_Port_Auto1",  bShow, bEnable, bReadFromFile, AnsiString("6000"));
    elUnloaderClip->Add(edClip_Address_Auto2,    &ccdUnloader[eUnloaderClipAuto2].sIP,      ECText,     "UnloaderClip", "UnloaderClip_IP_Auto2",    bShow, bEnable, bReadFromFile, AnsiString("172.16.8.203"));
    elUnloaderClip->Add(edClip_Port_Auto2,       &ccdUnloader[eUnloaderClipAuto2].sPort,    ECText,     "UnloaderClip", "UnloaderClip_Port_Auto2",  bShow, bEnable, bReadFromFile, AnsiString("6000"));
    elUnloaderClip->Add(edClip_Address_Auto3,    &ccdUnloader[eUnloaderClipAuto3].sIP,      ECText,     "UnloaderClip", "UnloaderClip_IP_Auto3",    bShow, bEnable, bReadFromFile, AnsiString("172.16.8.204"));
    elUnloaderClip->Add(edClip_Port_Auto3,       &ccdUnloader[eUnloaderClipAuto3].sPort,    ECText,     "UnloaderClip", "UnloaderClip_Port_Auto3",  bShow, bEnable, bReadFromFile, AnsiString("6000"));
    elUnloaderClip->Add(edClip_Address_Fix1,     &ccdUnloader[eUnloaderClipFix1].sIP,       ECText,     "UnloaderClip", "UnloaderClip_IP_Fix1",     bShow, bEnable, bReadFromFile, AnsiString("172.16.8.205"));
    elUnloaderClip->Add(edClip_Port_Fix1,        &ccdUnloader[eUnloaderClipFix1].sPort,     ECText,     "UnloaderClip", "UnloaderClip_Port_Fix1",   bShow, bEnable, bReadFromFile, AnsiString("6000"));
    elUnloaderClip->Add(edClip_Address_Fix2,     &ccdUnloader[eUnloaderClipFix2].sIP,       ECText,     "UnloaderClip", "UnloaderClip_IP_Fix2",     bShow, bEnable, bReadFromFile, AnsiString("172.16.8.206"));
    elUnloaderClip->Add(edClip_Port_Fix2,        &ccdUnloader[eUnloaderClipFix2].sPort,     ECText,     "UnloaderClip", "UnloaderClip_Port_Fix2",   bShow, bEnable, bReadFromFile, AnsiString("6000"));
    elUnloaderClip->Add(edClip_Address_Fix3,     &ccdUnloader[eUnloaderClipFix3].sIP,       ECText,     "UnloaderClip", "UnloaderClip_IP_Fix3",     bShow, bEnable, bReadFromFile, AnsiString("172.16.8.207"));
    elUnloaderClip->Add(edClip_Port_Fix3,        &ccdUnloader[eUnloaderClipFix3].sPort,     ECText,     "UnloaderClip", "UnloaderClip_Port_Fix3",   bShow, bEnable, bReadFromFile, AnsiString("6000"));
    ReadUnloaderClipIni();
    for(int i=eUnloaderClipAuto1;i<eUnloaderClipTotal;i++)
    {
        ccdUnloader[i].RecMsgCCDUnloaderClip=RecMsgCCDClip;
    }
    InitialAllCCDUnloaderClip();
}
//------------------------------------------------------------------------------
void TfBarCode::ReadUnloaderClipIni()
{
    if(elUnloaderClip!=NULL)
    {
        elUnloaderClip->ReadEditTextFromFile(AuthPath, GetUnloaderClipFileName());
        elUnloaderClip->InitialDataToEdit();
        for(int i=eUnloaderClipAuto1;i<eUnloaderClipTotal;i++)
        {
            ccdUnloader[i].SetCommParameter(ccdUnloader[i].sIP,ccdUnloader[i].sPort);
        }
    }
}
//------------------------------------------------------------------------------
void TfBarCode::WriteUnloaderClipIni()
{
    if(elUnloaderClip!=NULL)
    {
        elUnloaderClip->SaveEditTextToFile(AuthPath, GetUnloaderClipFileName());
    }
}
//------------------------------------------------------------------------------
void TfBarCode::RecMsgCCDClip(AnsiString asMsg)
{
    if(memoCCDCommLog->Lines->Count>50)
        memoCCDCommLog->Lines->Clear();
    memoCCDCommLog->Lines->Add(asMsg);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
AnsiString TfHIKROBOTcmd::GetStartCmd()
{
    AnsiString sret="start";
    return sret;
}
//------------------------------------------------------------------------------
AnsiString TfHIKROBOTcmd::GetAnalysisResult(AnsiString sreceive)
{
    AnsiString result = "";
    int lfPos = sreceive.Pos("LF");
    if (lfPos > 0)
    {
         AnsiString temp = sreceive.SubString(lfPos + 2, sreceive.Length());
        int atPos = temp.Pos("@");
        if (atPos > 0)
        {
            result = temp.SubString(1, atPos - 1);
        }
    }
    return result;
}
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
uCCDUnloaderClip::uCCDUnloaderClip()
{
    InitialDatas();
    socketClient=new uSocketClient();
    socketClient->SetReceiveFunc(ReceiveData);
}
//------------------------------------------------------------------------------
uCCDUnloaderClip::~uCCDUnloaderClip()
{
    delete socketClient;
}
//------------------------------------------------------------------------------
void uCCDUnloaderClip::ReceiveData(char* cGet, int iLen)
{
    asReceiveMsg=AnsiString(cGet);
    if(asReceiveMsg.Length()>iLen)
        asReceiveMsg=asReceiveMsg.SubString(0,iLen);
    asReceiveMsg=TfHIKROBOTcmd().GetAnalysisResult(asReceiveMsg);
    if(RecMsgCCDUnloaderClip)
    {
        RecMsgCCDUnloaderClip("[Receive]"+asReceiveMsg);
    }
}
//------------------------------------------------------------------------------
void uCCDUnloaderClip::SendCommandSocket(AnsiString asCommand)
{
    if(socketClient->IsConnected()==false)
    {
        DoCommuncation();
    }
    asReceiveMsg="";
    bflag=socketClient->SendCommand(asCommand.c_str(),asCommand.WideCharBufSize());
    if(RecMsgCCDUnloaderClip)
    {
        if(bflag)
            RecMsgCCDUnloaderClip("[Send]"+asCommand);
        else
        {
            RecMsgCCDUnloaderClip("[Send Error]"+asCommand+"[IP]"+socketClient->GetSocketAddress()+"[Port]"+socketClient->GetSocketPort());
        }
    }
    Sleep(5);
}
//------------------------------------------------------------------------------
void uCCDUnloaderClip::SendCommandPhoto()
{
    if(bSimulationCommand)
        return;
    SendCommandSocket(TfHIKROBOTcmd().GetStartCmd());
}
//------------------------------------------------------------------------------
void uCCDUnloaderClip::SendCommandStr(AnsiString sMsg)
{
    if(bSimulationCommand)
        return
    SendCommandSocket(sMsg);
}
//------------------------------------------------------------------------------
bool uCCDUnloaderClip::SetCommParameter(AnsiString asAddress, AnsiString asPort)
{
    return socketClient->SetCommParameter(asAddress,asPort);
}
//------------------------------------------------------------------------------
bool uCCDUnloaderClip::DoCommuncation()
{
    if(bSimulationCommand)
        return true;
    return socketClient->DoOpenCommuncation();
}
//------------------------------------------------------------------------------
void uCCDUnloaderClip::DoSocketClose()
{
    if(bSimulationCommand)
        return;
    socketClient->Close();
}
//------------------------------------------------------------------------------
const int iMaxRetryConn=10;
AnsiString uCCDUnloaderClip::DoGetPhotoCmd(bool bReset,AnsiString &sErr)
{
    if(bReset)
    {
        iCommandTask=1;
        sErr="";
        iRetryConn=0;
        bGetResult=false;
        return "";
    }
    switch(iCommandTask)
    {
        case 1:
            //send command
            if(DoCommuncation())
            {
                SendCommandPhoto();
                iCommandTask=100;
            }
            else
            {
                sErr=AnsiString().sprintf("Socket connection failed.IP:%s Port:%s",sIP,sPort);
            }
            break;
        case 100:
            if(bSimulationCommand)
            {
                asReceiveMsg=AnsiString("SimulateCode");
            }

            if(asReceiveMsg=="")
            {
                if(iRetryConn>iMaxRetryConn)
                {
                    iRetryConn=0;
                    sErr="Socket data reception failed.";
                }
                else
                {
                    iRetryConn++;
                }
            }
            else
            {
                bGetResult=true;
                return asReceiveMsg;
            }
            break;
    }
    return "";
}
//------------------------------------------------------------------------------
void __fastcall TfBarCode::btnClip_Connect_Auto1Click(TObject *Sender)
{
    TBitBtn *tempBtn;
    tempBtn = (TBitBtn *)Sender;
    int itag=tempBtn->Tag;
    ccdUnloader[itag].DoCommuncation();
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnClip_Disconnect_Auto1Click(TObject *Sender)
{
    TBitBtn *tempBtn;
    tempBtn = (TBitBtn *)Sender;
    int itag=tempBtn->Tag;
    ccdUnloader[itag].DoSocketClose();
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnClip_SendCmd_Auto1Click(TObject *Sender)
{
    TBitBtn *tempBtn;
    tempBtn = (TBitBtn *)Sender;
    int itag=tempBtn->Tag;
    AnsiString sMsg="";
    if(itag==eUnloaderClipAuto1)
    {
        sMsg=edClip_CMD_Auto1->Text;
    }
    else if(itag==eUnloaderClipAuto2)
    {
        sMsg=edClip_CMD_Auto2->Text;
    }
    else if(itag==eUnloaderClipAuto3)
    {
        sMsg=edClip_CMD_Auto3->Text;
    }
    else if(itag==eUnloaderClipFix1)
    {
        sMsg=edClip_CMD_Fix1->Text;
    }
    else if(itag==eUnloaderClipFix2)
    {
        sMsg=edClip_CMD_Fix2->Text;
    }
    else
    {
        sMsg=edClip_CMD_Fix3->Text;
    }
    ccdUnloader[itag].SendCommandStr(sMsg);
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnGetBarcodeClick(TObject *Sender)
{
    TBitBtn *tempBtn;
    tempBtn = (TBitBtn *)Sender;
    int itag=tempBtn->Tag;
    AnsiString sGet="";
    AnsiString sErr="";
    sGet=ccdUnloader[itag].DoGetPhotoCmd(false,sErr);
    if(sGet.IsEmpty()==false)
    {
        RecMsgCCDClip(sGet);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBarCode::btnInitialGetBarcodeClick(TObject *Sender)
{
    TBitBtn *tempBtn;
    tempBtn = (TBitBtn *)Sender;
    int itag=tempBtn->Tag;
    AnsiString sErr="";
    ccdUnloader[itag].DoGetPhotoCmd(true,sErr);
}
//---------------------------------------------------------------------------
void TfBarCode::InitialAllCCDUnloaderClip()
{
    AnsiString sErr="";
    for(int i=eUnloaderClipAuto1;i<eUnloaderClipTotal;i++)
    {
        ccdUnloader[i].DoGetPhotoCmd(true,sErr);
    }
}
//---------------------------------------------------------------------------
