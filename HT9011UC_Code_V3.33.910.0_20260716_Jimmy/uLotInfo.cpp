#include "MachineDefine.h"
#pragma hdrstop

#include "uLotInfo.h"

#include "MachineType.h"
#include "cpublic.h"
#include "cmydef.h"
#include "main.h"
#include "mymessbox.h"
#include "common.h"
#include "cMyDB.h"
#include "cSecurity.h"
#include "note.h"
#include "csystem.h"
#include "FTPClient.h"
#include "cBuilder.h"
#include "rs232.h"
#include "cprod.h"
#include "cSetUp.h"
#include "cCounterClear.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20151230 :add New ATC Interface
#include "bthermo.h"
#include "OCR.h"
#include "cSetUp.h"
#include "Socket_ASE_KR.h"
#include "cContactCT.h"
#include "uHGemHT9045.h"
#include "cContact.h"
#include "myQwertyKeyBoard.h"
#include "INPUT.h"                                                              //JerryYang 20151103
#include "Password.h"                                                           //JerryYang 20151103
#include "mycylin.h"                                                        //Eastsun 20260515 : AMR Cylinder
#include "cprod.h"
#include "BarCode.h"
#include "SgdToXLS.h"
#include "atester.h"                                                            //Ifor 20160516 add Site Mapping判斷溫度顯示
#include "database.h"
#include "MessageDef.h"
#include "HS_Function.h"
#include "cObserver.h"
#include "BarcodeReader.h"
#include "ARMS.h"                                                               //Ifor 20170621 (wei) add ARMS Function
#include "ProductionInfo.h"                                                     //Sam 20170719 (Steven) 移植超豐 OEE 功能 form HT-7045
#include "cSortCT.h"                                                            //Sam 20170719 (Steven) 移植超豐 OEE 功能 form HT-7045
#include "MonitorInterface.h"                                                   //Sam 20170925 (Steven) 攝影機連線檢查
#include "mysensor.h"
#include "uTemp_Set.h"
#include "MyKitSuck.h"
#include "myMN200motor.h"
#include "cSocket.h"
#include "MyMotor.h"
#include "QAMode.h"
#include "SCK_ART.h"
#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
#include "ExternFunction.h"
#include "SortingBinTray.h"
#include "TesterTCP.h"
#include "cConfiguration.h"
#include "WinWaySetting.h"                                                      //Jimmychiu 20210906
#include "cShowBinSelect.h"
#include "uTemp_Set.h"                                                          //Ztex 2023.04.18 Add HT-1032 ATC TJ Function
#include "bthermo.h"                                                            //Ztex 2023.04.18 Add HT-1032 ATC TJ Function
#include "cAuthority.h"
#include "cinitial.h"
#include "TextProcess.h"
#include "uCleaning.h"
#include "InterfaceSYS.h"
#include "FileInfo.h"
#include "GroundMan.h"
#include "TfFTP.h"
#include "cStartCondition.h"
#include "acatchtray.h"
#include "AMR.h"
#include "AGV.h"
#include "fAOI.h"
//#include "BarcodeXML.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "HTray"
#pragma link "SPComm"
#pragma link "BtnPanelLane"
#pragma link "butPa1"
#pragma resource "*.dfm"
TfLotInfo *fLotInfo;
TPanel    *SocketSiteCH_Display[MAX_SOCKET_ROW][MAX_SOCKET_COL];                //JerryYang 20190702 ASE-CL顯示SocketID
TLabel    *SocketLabCol_Display[MAX_SOCKET_COL];
TLabel    *SocketLabRow_Display[MAX_SOCKET_ROW];
bool bLotID_OK=false;
bool bOPID_OK=false;
bool bTemp_OK=false;
bool bDeviceName_OK=false;
bool bLotFirstKeyIn=false;                                                      //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗

//AI(ht9045-v899) 20260525: trace PTI Lot Start context for tester TCP diagnostics
static void WritePTILotStartTrace(TfLotInfo *pLotInfo, AnsiString Stage, AnsiString Detail)
{
    if(CUSTOMER_CODE!=CC_PTI || pLotInfo==NULL)
        return;

    GetTimeInfo();

    AnsiString Path, FileName, Log, ProcLog;
    Path.sprintf("D:\\HT9045_Log\\PTI_LotStartTrace\\%04d_%02d", SystemYear, SystemMonth);
    MyForceDirectories(Path);
    FileName.sprintf("%s\\%04d_%02d_%02d.txt", Path, SystemYear, SystemMonth, SystemDate);

    Log.sprintf("%04d-%02d-%02d,%02d:%02d:%02d.%03d,Stage=%s,AccessLevel=%d,StartMode=%d,RunMode=%s,LotID=%s,OPID=%s,Tester=%d,TestType=%d,%s",
                SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec,
                Stage, AccessLevel, LastSet.iRunStartMode, pLotInfo->cbRunMode->Text,
                pLotInfo->edtSysLotID->Text, pLotInfo->edtSysOperatorID->Text,
                LastSet.iTester, TestIF.iTestType, Detail);
    WriteDataToFile(FileName.c_str(), Log.c_str());

    ProcLog.sprintf("PTI LotStart Trace: %s, RunMode=%s, %s", Stage, pLotInfo->cbRunMode->Text, Detail);
    RecordProcess(ProcLog);
}

//AI(ht9045-v899) 20260525: restore PTI RunMode before local Lot Start gate
static void NormalizePTIRunModeBeforeLotStart(TfLotInfo *pLotInfo)
{
    if(CUSTOMER_CODE!=CC_PTI || pLotInfo==NULL || pLotInfo->cbRunMode->Text!="")
        return;

    AnsiString OldRunMode=pLotInfo->cbRunMode->Text;
    AnsiString NewRunMode="Normal";
    AnsiString Detail;

    if(IniConfig.bB03_TesterReport)
    {
        NewRunMode="1'st";
    }
    else if(LastSet.iRunStartMode==rsmContinuRetest ||
            LastSet.iRunStartMode==rsmCInitialRetest)
    {
        NewRunMode="Re-Test";
    }

    pLotInfo->cbRunMode->Text=NewRunMode;
    pLotInfo->cbRunMode->ItemIndex=-1;
    for(int i=0; i<pLotInfo->cbRunMode->Items->Count; i++)
    {
        if(pLotInfo->cbRunMode->Items->Strings[i]==NewRunMode)
        {
            pLotInfo->cbRunMode->ItemIndex=i;
            break;
        }
    }

    Detail.sprintf("OldRunMode=%s,NewRunMode=%s", OldRunMode, NewRunMode);
    WritePTILotStartTrace(pLotInfo, "NormalizeRunMode", Detail);
}

//---------------------------------------------------------------------------
__fastcall TfLotInfo::TfLotInfo(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
    lblOCR_LotID->Caption="";
    iLotRead  =0;
    iLotStart =1;
    iLotEnd   =2;

    bStartChamberBoost=false;
    iXMLOnLineStatus=0;                                                         //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
    bRTCChangeFileFinish=false;                                                 //ChungHung 20140514 fix in homeing and contact make time out
    bStartCount_SCK=false;                                                      //ChungHung 20140714 add for SCK
    sVTestInternalLot="";                                                       //RogerYang 20250809 偉測Summary文件修改

    ESD_DataPtr[ 0]=pl_ESDProx1_1; ESD_DataPtr[ 1]=pl_ESDProx1_2; ESD_DataPtr[ 2]=pl_ESDProx1_3;
    ESD_DataPtr[ 3]=pl_ESDProx2_1; ESD_DataPtr[ 4]=pl_ESDProx2_2; ESD_DataPtr[ 5]=pl_ESDProx2_3;
    ESD_DataPtr[ 6]=pl_ESDProx3_1; ESD_DataPtr[ 7]=pl_ESDProx3_2; ESD_DataPtr[ 8]=pl_ESDProx3_3;
    ESD_DataPtr[ 9]=pl_ESDProx4_1; ESD_DataPtr[10]=pl_ESDProx4_2; ESD_DataPtr[11]=pl_ESDProx4_3;
    ESD_DataPtr[12]=pl_ESDProx5_1; ESD_DataPtr[13]=pl_ESDProx5_2; ESD_DataPtr[14]=pl_ESDProx5_3;
    ESD_DataPtr[15]=pl_ESDProx6_1; ESD_DataPtr[16]=pl_ESDProx6_2; ESD_DataPtr[17]=pl_ESDProx6_3;

    ESD_DECAY_DATA_Ptr[ 0]=pl_PositiveDecay1_1; ESD_DECAY_DATA_Ptr[ 1]=pl_PositiveDecay1_2; ESD_DECAY_DATA_Ptr[ 2]=pl_PositiveDecay1_3;
    ESD_DECAY_DATA_Ptr[ 3]=pl_PositiveDecay2_1; ESD_DECAY_DATA_Ptr[ 4]=pl_PositiveDecay2_2; ESD_DECAY_DATA_Ptr[ 5]=pl_PositiveDecay2_3;
    ESD_DECAY_DATA_Ptr[ 6]=pl_PositiveDecay3_1; ESD_DECAY_DATA_Ptr[ 7]=pl_PositiveDecay3_2; ESD_DECAY_DATA_Ptr[ 8]=pl_PositiveDecay3_3;
    ESD_DECAY_DATA_Ptr[ 9]=pl_PositiveDecay4_1; ESD_DECAY_DATA_Ptr[10]=pl_PositiveDecay4_2; ESD_DECAY_DATA_Ptr[11]=pl_PositiveDecay4_3;
    ESD_DECAY_DATA_Ptr[12]=pl_PositiveDecay5_1; ESD_DECAY_DATA_Ptr[13]=pl_PositiveDecay5_2; ESD_DECAY_DATA_Ptr[14]=pl_PositiveDecay5_3;
    ESD_DECAY_DATA_Ptr[15]=pl_PositiveDecay6_1; ESD_DECAY_DATA_Ptr[16]=pl_PositiveDecay6_2; ESD_DECAY_DATA_Ptr[17]=pl_PositiveDecay6_3;

    ESD_DECAY_DATA_Ptr[18]=pl_NegativeDecay1_1; ESD_DECAY_DATA_Ptr[19]=pl_NegativeDecay1_2; ESD_DECAY_DATA_Ptr[20]=pl_NegativeDecay1_3;
    ESD_DECAY_DATA_Ptr[21]=pl_NegativeDecay2_1; ESD_DECAY_DATA_Ptr[22]=pl_NegativeDecay2_2; ESD_DECAY_DATA_Ptr[23]=pl_NegativeDecay2_3;
    ESD_DECAY_DATA_Ptr[24]=pl_NegativeDecay3_1; ESD_DECAY_DATA_Ptr[25]=pl_NegativeDecay3_2; ESD_DECAY_DATA_Ptr[26]=pl_NegativeDecay3_3;
    ESD_DECAY_DATA_Ptr[27]=pl_NegativeDecay4_1; ESD_DECAY_DATA_Ptr[28]=pl_NegativeDecay4_2; ESD_DECAY_DATA_Ptr[29]=pl_NegativeDecay4_3;
    ESD_DECAY_DATA_Ptr[30]=pl_NegativeDecay5_1; ESD_DECAY_DATA_Ptr[31]=pl_NegativeDecay5_2; ESD_DECAY_DATA_Ptr[32]=pl_NegativeDecay5_3;
    ESD_DECAY_DATA_Ptr[33]=pl_NegativeDecay6_1; ESD_DECAY_DATA_Ptr[34]=pl_NegativeDecay6_2; ESD_DECAY_DATA_Ptr[35]=pl_NegativeDecay6_3;

    ATCChPal[ 0]=pan_ATCTempHead01; ATCChPal[ 1]=pan_ATCTempHead02; ATCChPal[ 2]=pan_ATCTempHead03; ATCChPal[ 3]=pan_ATCTempHead04;
    ATCChPal[ 4]=pan_ATCTempHead05; ATCChPal[ 5]=pan_ATCTempHead06; ATCChPal[ 6]=pan_ATCTempHead07; ATCChPal[ 7]=pan_ATCTempHead08;
    ATCChPal[ 8]=pan_ATCTempHead09; ATCChPal[ 9]=pan_ATCTempHead10; ATCChPal[10]=pan_ATCTempHead11; ATCChPal[11]=pan_ATCTempHead12;
    ATCChPal[12]=pan_ATCTempHead13; ATCChPal[13]=pan_ATCTempHead14; ATCChPal[14]=pan_ATCTempHead15; ATCChPal[15]=pan_ATCTempHead16;
    ATCChPal[16]=pan_ATCTempHead17; ATCChPal[17]=pan_ATCTempHead18; ATCChPal[18]=pan_ATCTempHead19; ATCChPal[19]=pan_ATCTempHead20;
    ATCChPal[20]=pan_ATCTempHead21; ATCChPal[21]=pan_ATCTempHead22; ATCChPal[22]=pan_ATCTempHead23; ATCChPal[23]=pan_ATCTempHead24;
    ATCChPal[24]=pan_ATCTempHead25; ATCChPal[25]=pan_ATCTempHead26; ATCChPal[26]=pan_ATCTempHead27; ATCChPal[27]=pan_ATCTempHead28;
    ATCChPal[28]=pan_ATCTempHead29; ATCChPal[29]=pan_ATCTempHead30; ATCChPal[30]=pan_ATCTempHead31; ATCChPal[31]=pan_ATCTempHead32;

    ATCReferPtr[ 0]=pl_ATCRefHead01; ATCReferPtr[ 1]=pl_ATCRefHead02; ATCReferPtr[ 2]=pl_ATCRefHead03; ATCReferPtr[ 3]=pl_ATCRefHead04;
    ATCReferPtr[ 4]=pl_ATCRefHead05; ATCReferPtr[ 5]=pl_ATCRefHead06; ATCReferPtr[ 6]=pl_ATCRefHead07; ATCReferPtr[ 7]=pl_ATCRefHead08;
    ATCReferPtr[ 8]=pl_ATCRefHead09; ATCReferPtr[ 9]=pl_ATCRefHead10; ATCReferPtr[10]=pl_ATCRefHead11; ATCReferPtr[11]=pl_ATCRefHead12;
    ATCReferPtr[12]=pl_ATCRefHead13; ATCReferPtr[13]=pl_ATCRefHead14; ATCReferPtr[14]=pl_ATCRefHead15; ATCReferPtr[15]=pl_ATCRefHead16;
    ATCReferPtr[16]=pl_ATCRefHead17; ATCReferPtr[17]=pl_ATCRefHead18; ATCReferPtr[18]=pl_ATCRefHead19; ATCReferPtr[19]=pl_ATCRefHead20;
    ATCReferPtr[20]=pl_ATCRefHead21; ATCReferPtr[21]=pl_ATCRefHead22; ATCReferPtr[22]=pl_ATCRefHead23; ATCReferPtr[23]=pl_ATCRefHead24;
    ATCReferPtr[24]=pl_ATCRefHead25; ATCReferPtr[25]=pl_ATCRefHead26; ATCReferPtr[26]=pl_ATCRefHead27; ATCReferPtr[27]=pl_ATCRefHead28;
    ATCReferPtr[28]=pl_ATCRefHead29; ATCReferPtr[29]=pl_ATCRefHead30; ATCReferPtr[30]=pl_ATCRefHead31; ATCReferPtr[31]=pl_ATCRefHead32;

    ATCPtr[ 0]=pl_ATCTempHead01; ATCPtr[ 1]=pl_ATCTempHead02; ATCPtr[ 2]=pl_ATCTempHead03; ATCPtr[ 3]=pl_ATCTempHead04;
    ATCPtr[ 4]=pl_ATCTempHead05; ATCPtr[ 5]=pl_ATCTempHead06; ATCPtr[ 6]=pl_ATCTempHead07; ATCPtr[ 7]=pl_ATCTempHead08;
    ATCPtr[ 8]=pl_ATCTempHead09; ATCPtr[ 9]=pl_ATCTempHead10; ATCPtr[10]=pl_ATCTempHead11; ATCPtr[11]=pl_ATCTempHead12;
    ATCPtr[12]=pl_ATCTempHead13; ATCPtr[13]=pl_ATCTempHead14; ATCPtr[14]=pl_ATCTempHead15; ATCPtr[15]=pl_ATCTempHead16;
    ATCPtr[16]=pl_ATCTempHead17; ATCPtr[17]=pl_ATCTempHead18; ATCPtr[18]=pl_ATCTempHead19; ATCPtr[19]=pl_ATCTempHead20;
    ATCPtr[20]=pl_ATCTempHead21; ATCPtr[21]=pl_ATCTempHead22; ATCPtr[22]=pl_ATCTempHead23; ATCPtr[23]=pl_ATCTempHead24;
    ATCPtr[24]=pl_ATCTempHead25; ATCPtr[25]=pl_ATCTempHead26; ATCPtr[26]=pl_ATCTempHead27; ATCPtr[27]=pl_ATCTempHead28;
    ATCPtr[28]=pl_ATCTempHead29; ATCPtr[29]=pl_ATCTempHead30; ATCPtr[30]=pl_ATCTempHead31; ATCPtr[31]=pl_ATCTempHead32;

    //Steven 20181224 : For ASE-CL
    //==>
    edSocket[0][0]=edtSocketAa; edSocket[0][1]=edtSocketAb; edSocket[0][2]=edtSocketAc; edSocket[0][3]=edtSocketAd;
    edSocket[0][4]=edtSocketAe; edSocket[0][5]=edtSocketAf; edSocket[0][6]=edtSocketAg; edSocket[0][7]=edtSocketAh;
    edSocket[1][0]=edtSocketBa; edSocket[1][1]=edtSocketBb; edSocket[1][2]=edtSocketBc; edSocket[1][3]=edtSocketBd;
    edSocket[1][4]=edtSocketBe; edSocket[1][5]=edtSocketBf; edSocket[1][6]=edtSocketBg; edSocket[1][7]=edtSocketBh;
    edSocket[2][0]=edtSocketCa; edSocket[2][1]=edtSocketCb; edSocket[2][2]=edtSocketCc; edSocket[2][3]=edtSocketCd;
    edSocket[2][4]=edtSocketCe; edSocket[2][5]=edtSocketCf; edSocket[2][6]=edtSocketCg; edSocket[2][7]=edtSocketCh;
    edSocket[3][0]=edtSocketDa; edSocket[3][1]=edtSocketDb; edSocket[3][2]=edtSocketDc; edSocket[3][3]=edtSocketDd;
    edSocket[3][4]=edtSocketDe; edSocket[3][5]=edtSocketDf; edSocket[3][6]=edtSocketDg; edSocket[3][7]=edtSocketDh;
    slASECLTestInfor = new TStringList;
    //<==
    //Steven 20181224 : For ASE-CL

    //JerryYang 20190702 ASE-CL顯示SocketID
    //==>
    TPanel *tempTestSiteCBox_Dis[MAX_SOCKET_ROW][MAX_SOCKET_COL]=
    {
        {palAa, palAb, palAc, palAd, palAe, palAf, palAg, palAh},
        {palBa, palBb, palBc, palBd, palBe, palBf, palBg, palBh},
        {palCa, palCb, palCc, palCd, palCe, palCf, palCg, palCh},
        {palDa, palDb, palDc, palDd, palDe, palDf, palDg, palDh}
    };
    TLabel *tempTestLabCol_Dis[MAX_SOCKET_COL]=
    {
        lbSocketIDColA, lbSocketIDColB, lbSocketIDColC, lbSocketIDColD, lbSocketIDColE, lbSocketIDColF, lbSocketIDColG, lbSocketIDColH
    };
    TLabel *tempTestLabRow_Dis[MAX_SOCKET_ROW]={lbSocketIDRowA, lbSocketIDRowB, lbSocketIDRowC, lbSocketIDRowD};

    int iCol, iRow;
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        iRow=i/MAX_SOCKET_COL;
        iCol=i%MAX_SOCKET_COL;

        if(i<MAX_SOCKET_ROW)
        {
            SocketLabRow_Display[i]=tempTestLabRow_Dis[i];
            SocketLabRow_Display[i]->Visible=false;
        }

        if(i<MAX_SOCKET_COL)
        {
            SocketLabCol_Display[i]=tempTestLabCol_Dis[i];
            SocketLabCol_Display[i]->Visible=false;
        }
        SocketSiteCH_Display[iRow][iCol]=tempTestSiteCBox_Dis[iRow][iCol];
        SocketSiteCH_Display[iRow][iCol]->Visible=false;
    }
    //<==
    //JerryYang 20190702 ASE-CL顯示SocketID
    TimerERMS->Enabled=true;
    tsChamberBoost->TabVisible=false;
    btnCancelTestPause->Enabled=false;
    btnESCFunction->Enabled=(IniConfig.bI41_6_Manual);
    if(IniConfig.bSIGURDFunction)                                               //Sam 20210401 : 俊堯要求只要顯示 FTP
        ts_FTPAutomation->Caption="FTP";
    //jimmychiu 20210906
    //<==
    if(ATC_SYSTEM==eWinWay && Temperature.bATCActiveCooling==true)ATC_WinWay->TabVisible=true;
    else ATC_WinWay->TabVisible=false;
    ATCPtrWinWay[0]=pnlWinwayPVCH1;ATCPtrWinWay[1]=pnlWinwayPVCH2;ATCPtrWinWay[2]=pnlWinwayPVCH3;ATCPtrWinWay[3]=pnlWinwayPVCH4;
    //<==
    //Jimmychiu 20230307 initialized in the constructor
    //<==
    bNeedToDeleteFile=false;
    iWaitRtcDeleteTask=0;
    bEventLogAlarm=false;
    ZeroMemory(fTempUserOffset,sizeof(fTempUserOffset));
    ZeroMemory(fContactHeight, sizeof(fContactHeight));
    ZeroMemory(fTempATCOffset, sizeof(fTempATCOffset));
    ZeroMemory(iShuttleMode, sizeof(iShuttleMode));
//    ZeroMemory(iAutoClean, sizeof(iAutoClean));
    ZeroMemory(bART, sizeof(bART));
    iART=0;
    iIndexHeatingMode=0;
    //<==
    //Jimmychiu 20230307 initialized in the constructor
    iProduceTimeCT=0;                                                           //jou 20221125 : 機台添加三小時送檢報警，從lot start時間開始計算
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::FormDestroy(TObject *Sender)
{
    try
    {
        TimerERMS->Enabled=false;
        slASECLTestInfor->Clear();
        delete slASECLTestInfor;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfLotInfo::FormDestroy");
    }
    LogSoftwareOffTime("TfLotInfo, FormDestroy");                               //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::FormShow(TObject *Sender)
{
    bShow=true;

    AnsiString sConfigPath=AuthPath+"Security_new.def";                         //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋

    GetTimeInfo();
    ts_FTPAutomation->TabVisible=false;                                         //KaiChen 20190530 ：Sigurd FTP Automation
    tsSigurd_CX->TabVisible =(CUSTOMER_CODE==CC_SIGURD_ChungXing);              //Sam 20220223 : 矽格中興廠新增 Lot 資料
    tsMurata->TabVisible    =(CUSTOMER_CODE==CC_Murata);
    tsSPIL_SZ->TabVisible   =(CUSTOMER_CODE==CC_SPIL_CHINA_SUZHOU);
    tsOEE->TabVisible       =(CosFunction.bOEEFunction);                        //Steven 20221225 : 整理LotInfo畫面
    SettsChipAdvVisible();                                                      //Steven 20221225 : add for CyuEan
    ts_ATC6_1->TabVisible   =(Tri_Temp_Machine==1 || (ATC_SYSTEM==eNewATCSystem && ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61));
    tsPATSetUp->TabVisible  =(CUSTOMER_CODE==CC_PANTHER);
    tsBundle->TabVisible    =(USE_COVER_TRAYID!=tCIDNotUse);
    tsSetupFileCheck->TabVisible=(IniConfig.bEnableRmsCheckSetupFile==true);    //Ifor 20230516 add: TFAMD 要求工作檔驗證

    lblPage->Visible=CosFunction.bUseSCKART;
    edPage->Visible=CosFunction.bUseSCKART;

    labCusLotID->Visible=false;                                                 //JerryYang 20230322 : 2D sort lot info UI修改
    labCusDevGrp->Visible=false;
    labDeviceName->Visible=false;
    edtCusLotID->Visible=false;
    edtCusDevGrp->Visible=false;
    edtDevice->Visible=false;
    labCusStep->Visible=false;                                                  //JerryYang 20260201 : add
    edtCusStep->Visible=false;

    pgLotinfo->ActivePage=tsLotID;

    if(CUSTOMER_CODE==CC_CYUEAN ||                                              //Steven 20240122 : CyuEan要FT/RT
       CUSTOMER_CODE==CC_JSI_HAOXING)                                           //Steven 20230302 : Add for 紹興長電
    {
        lbLotRunMode->Visible   =true;
        cbRunMode->Visible      =true;
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT");
        cbRunMode->Items->Add("RT");
        cbRunMode->Text="FT";
    }
    else if(CUSTOMER_CODE==CC_NEXPERIA_Guangdong)                               //Steven 20230301 : Add for 安世
    {
        lbLotRunMode->Visible   =false;
        cbRunMode->Visible      =false;
    }

    if(CUSTOMER_CODE==CC_MTI)
    {
        tsDeviceInfo->TabVisible=false;
        tsFTP->TabVisible=false;
        palSecsGem->Visible=false;                                              //Steven 20140701
    }
    else if(IniConfig.bShowLotInfo)
    {
        tsDeviceInfo->TabVisible=(IniConfig.bEnableRms==true);
        ResetLotInfo();                                                         //Steven 20240925 : 重開軟體時, 要讀回lot info
        if(tsDeviceInfo->TabVisible==true)
        {
            pgLotinfo->ActivePage=tsDeviceInfo;
        }
    }
    SetSelectionVisible();                                                      //Steven 20250519 : 統一Selection的顯示設定

    if(CUSTOMER_CODE==CC_TSI)                                                   //frank 20200814 : 每10盤記錄一次summary log
    {
        lblOPID->Caption        ="Tester ID";
        lbLotRunMode->Visible   =false;
        cbRunMode->Visible      =false;
        btnSaveData->Visible    =true;
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        tsLotID->TabVisible=IniConfig.bB12UsePATSetup;
        cbPATMode->ItemIndex=0;
        cbPATModeChange(cbPATMode);
        #ifdef SOFT_SIMULTE
        btnRealTime->Visible=true;
        btnpatHourly->Visible=true;
        btnpatEndLot->Visible=true;
        #endif
    }
    ShowXMLOnLine();                                                            //Steven 20200706 : 移到外面

    if(CUSTOMER_CODE==CC_JCET)                                                  //Steven 20170605 (wei) : For長電
        tsFTP->TabVisible=IniConfig.bEnableFTP;
    else
        tsFTP->TabVisible=(CosFunction.bFTPFunction);

    if(IniConfig.bEnable_SECS_GEM ||                                            //Steven 20140701
       CosFunction.bLotStartLockCriticalPara)                                   //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        palSecsGem->Visible=true;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE ||
       (CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true))       //wei 20150826 Lot 強制顯示
    {
        palCurrFailRate->Visible=false;
        Panel27->Visible=false;                                                 //Ifor 20190517 KYEC 不顯示

        edtSysLotID->PopupMenu=FormBarcodeReader->pmBarcode;
        edtSysOperatorID->PopupMenu=FormBarcodeReader->pmBarcode;
        #ifdef SOFT_SIMULTE
            LotKeyInTime->Enabled = false;
        #else
            LotKeyInTime->Enabled = true;                                       //Ifor 20190919 add Lot Info Sacn Time
        #endif
    }

    fLotInfo->ResetLotInfo();                                                   //Steven 20240925 : 重開軟體時, 要讀回lot info

    grpRFID->Visible=(USE_RFID_READER);                                         //Steven 20220713 : RFID Reader for SJSEMI

    if(CUSTOMER_CODE==CC_PTI ||                                                 //RogerYang 20170329 (Steven) 力成 LotID卡關
       CUSTOMER_CODE==CC_TFME_CHINA)
    {
        tsFTP->TabVisible=false;                                                //不知道幹嘛用，先藏起來
    }

    if(CUSTOMER_CODE==CC_SCK)                                                   //ChungHung 20131225 add
    {
        ts_AutoCleanMonitor->TabVisible=true;
    }
    else
    {
        ts_AutoCleanMonitor->TabVisible=false;
    }

    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //Steven 20150923
    {
        Label5->Visible=false;
        edTemp->Visible=false;
    }
    else if(CUSTOMER_CODE==CC_SCC ||
            CUSTOMER_CODE==CC_SCK)                                              //ChungHung 20130621 add SCK RMS
    {
        Label5->Visible=false;
        edTemp->Visible=false;
//        tsSelection->TabVisible=false;
        btDownload->Caption="Download from Handler";
    }
    else if(IniConfig.bSPILFunction==true)                                      //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
    {
        chkAutoCleanContactHeight->Checked=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean Contact Height",      false);                                //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋
        btnFtpTester->Visible       =false;
        Label153->Visible           =false;
        edDeviceName->Visible       =false;
        Label5->Visible             =false;
        edTemp->Visible             =false;
        FileListBox1->Visible       =false;
        tsDeviceInfo->TabVisible    =true;
        FileListBox1->Directory     =DataPath;
        chkTempOffset->Checked      =false;                                     //JerryYang 20180212 (Steven) SPIL要求鎖死不得修改
        chkContactHigh->Checked     =false;
        chkContactForce->Checked    =true;
        chkContactMode->Checked     =true;
        checkbAutoClean->Checked    =false;                                     //JerryYang 20191003 矽品只還原auto clean offset
        chkHotPlate->Checked        =true;
        chkLoadUnload->Checked      =true;
        chkSpeedSetting->Checked    =true;
        chkShuttleMode->Checked     =true;
        chkTestMode->Checked        =true;
        cbBottom2DOffset->Checked   =false;                                     //JerryYang 20201122 Bottom 2D offset不覆蓋
        chkTestMode->Visible        =false;                                     //JerryYang 20170214 (Steven) 此三項無作用,先不顯示
        chkBinasgn->Visible         =false;
        chkBinasgnOff->Visible      =false;
        groupbDownloadItem->Enabled =false;
        cbRTCASTD->Visible          =true;
        btnSaveClick(this);                                                     //JerryYang 20180212 (Steven) SPIL要求鎖死不得修改
    }
    else if(CUSTOMER_CODE==CC_TERAPOWER)
    {
        btnFtpTester->Visible=false;
    }
    else if(CUSTOMER_CODE==CC_AMD_M)
    {
        btnFtpTester->Visible=false;                                            //不顯示
    }
    else if(CUSTOMER_CODE==CC_AMKOR_China ||                                    //jou 2016-06-13 修正 Amkor china download recipe 參數會參照本機
            CUSTOMER_CODE==CC_QUALCOMM)                                         //JerryYang 20170412 (Steven) add QUALCOMM
    {
        chkTempOffset->Checked  =false;
        chkContactHigh->Checked =false;
        chkContactForce->Checked=true;
        chkContactMode->Checked =true;
        chkBinasgn->Visible     =false;                                         //jou 2014-08-26 安靠要求 Auto download Binasgn不顯示並且強制開啟
        chkBinasgnOff->Visible  =false;
        chkBinasgn->Checked     =true;
        chkBinasgnOff->Checked  =true;

        chkContactHigh->Enabled =false;
        chkTempOffset->Enabled  =false;
        chkContactForce->Enabled=false;
        chkContactMode->Enabled =false;

        btnSaveClick(this);
    }
    else if(CUSTOMER_CODE==CC_ChipMos_ZHUBEI)                                   //Jimmychiu 20250430 : ChipMos 關閉Device info 溫度欄位
    {
        edTemp->Visible=false;
    }
    else
    {
        ART_Panel->Visible=false;
    }
    tsRTCFullViewImg->TabVisible=REAL_TIME_CCD;                                 //Steven 20110825 : Real time CCD - 顯示Full View Image

    if(!TrayForm.bEnableAMR) //Eastsun 20260515 F020
        tsKYEC_AMR->TabVisible=false;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150706 ART
    {
        GroupBox3->Visible=false;
        ART_Panel->Visible=true;
        ART_Panel->Left=3;
        ART_Panel->Top=5;
    }
    else
    {
        ART_Panel->Visible=false;
        GroupBox3->Visible=true;
        GroupBox3->Left=3;
        GroupBox3->Top=5;
    }

    SetATCFormVisible();                                                        //Steven 20160217 : For ATC7.0  //Ifor 20160516 往下移動，避免ATC頁面關閉後又被打開

    tsBarCode->TabVisible=(BAR_CODE_INSTALL==ebctUseCCDMode || BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD);                          //Ifor 20190129 : add Cognex EtherNet 通訊
    btChangeFile->Visible=(BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD || (BAR_CODE_INSTALL==ebctUseCCDMode && CosFunction.b2DUseSubJobFunction==true));  //wei 20160728 Barcode File切換 //add Sub Job

    tsESDMonitor->TabVisible=false;                                             //Ifor 20160308 ESD 畫面顯示

    ts_OCRInterface->TabVisible=(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR==false);
    ts_SocketInterface->TabVisible=IniConfig.bSocketCommunication;              //ChungHung 20130112 add for ASE_KR Socket Tester

    if(MachineTypeChoice==Type_HT9046_LS)                                       //2013-01-15    Dell
    {
        if(REAL_TIME_CCD==true && COM2->bCCDDummyRum == false)
        {
            imgRTCFullView1->Width = imgRTCFullView1->Width/2;
            imgRTCFullView2->Width = imgRTCFullView2->Width/2;
            imgRTCFullView3->Width = imgRTCFullView3->Width/2;
            imgRTCFullView4->Width = imgRTCFullView4->Width/2;
        }
    }

    tsBarCode->TabVisible=(BAR_CODE_INSTALL==ebctUseCCDMode || BAR_CODE_INSTALL==ebctInShtIntel || BAR_CODE_INSTALL==ebctEtherNetCCD);                          //Ifor 20190129 : add Cognex EtherNet 通訊

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //ChungHung 20150413 add for TSMC
    {
        IniConfig.bShowLotInfo=true;
        btnDataFTPSaveToData->Visible=true;
        edtSysOperatorID->Visible=false;
        lblOPID->Visible=false;
        spSECSLotCheck->Visible=true;
        sbSECSLotStart->Enabled=false;
        Panel6->Visible=true;
        Panel6->Top=144;
        Panel6->Left=3;
    }
    else
    {
        btnDataFTPSaveToData->Visible=false;
        edtSysOperatorID->Visible=true;
        lblOPID->Visible=true;
        spSECSLotCheck->Visible=false;
        sbSECSLotStart->Enabled=true;
        sbSECSLotEnd->Down=true;
        sbSECSLotStart->Down=false;
        Panel6->Visible=false;
        Panel6->Top=320;
        Panel6->Left=3;
    }

    if(IniConfig.bSIGURDFunction || CosFunction.bShowYieldMonitor)              //Sam 20210916 : 新增 Yiled Monitor 到畫面上 //Sam 20210324 : 新增 Yield Monitor
    {
        tsYieldMonitior->TabVisible=true;
        tsTPW->TabVisible   =(CUSTOMER_CODE==CC_TERAPOWER || CUSTOMER_CODE==CC_PTI);
        tsSigurd->TabVisible=(CUSTOMER_CODE==CC_SIGURD_PeiXing);
        RefreshYieldMonitor();
    }
    else
    {
        tsYieldMonitior->TabVisible=false;
    }

    fBarCode->mtBarcodeSetDefaultView();
    chkTestMode->Visible=CosFunction.bLastSetInSetUpFile;                       //jou 2014-08-25 修正未開啟 CosFunction.bLastSetInSetUpFile LotInfo 顯示錯誤

    labLevelMode->Visible=CosFunction.bDownloadRecipeLevelMode;                 //jou 2016-01-06 download recipe 增加權限模式選擇
    coLevelMode->Visible=CosFunction.bDownloadRecipeLevelMode;                  //jou 2016-01-06 download recipe 增加權限模式選擇

    sgBarcode->Cells[ 0][ 0]="Shuttle";
    sgBarcode->Cells[ 1][ 0]="1_A";
    sgBarcode->Cells[ 2][ 0]="1_B";
    sgBarcode->Cells[ 3][ 0]="2_A";
    sgBarcode->Cells[ 4][ 0]="2_B";
    sgBarcode->Cells[ 5][ 0]="Total";
    sgBarcode->Cells[ 0][ 1]="Load";
    sgBarcode->Cells[ 0][ 2]="Pass";
    sgBarcode->Cells[ 0][ 3]="Fail";
    sgBarcode->Cells[ 0][ 4]="Rate(%)";
    sgBarcode->Cells[ 0][ 5]="Retry";
    sgBarcode->Cells[ 0][ 6]="Duplicate";

    //wei 20161125 OCR顆數統計顯示修改
    sgOCR->Cells[ 0][ 0]="OCR";
    sgOCR->Cells[ 1][ 0]="Tray";
    sgOCR->Cells[ 2][ 0]="Lot";
    sgOCR->Cells[ 0][ 1]="Pass";
    sgOCR->Cells[ 0][ 2]="Key In";
    sgOCR->Cells[ 0][ 3]="No IC";
    sgOCR->Cells[ 0][ 4]="Total";

    //wei 20170119 (jou) ATR FT/RT count
    //==>
    sgATRCount->Cells[ 0][ 0]="ATR";
    sgATRCount->Cells[ 1][ 0]="FT";
    sgATRCount->Cells[ 2][ 0]="RT";
    sgATRCount->Cells[ 0][ 1]="Pass";
    sgATRCount->Cells[ 0][ 2]="Fail";
    sgATRCount->Cells[ 0][ 3]="Total";
    //<==
    //wei 20170119 (jou) ATR FT/RT count

    ShowATCTempPanel();                                                         //Steven 20241112 : 調整ATC溫度顯示
    if(Temperature.bATCActiveCooling==false || CUSTOMER_CODE==CC_KYEC_LEE)      //Ifor 20160902 add 未啟動ATC功能 顯示ATC off line //Ifor 20170609 (wei) add KYEC ATC 不開啟自動連線由人員啟動
    {
        bStartATCRun=false;
        fLotInfo->pl_ATC_Online->Color=clRed;                                   //Ifor 20170609 (wei) add KYEC ATC 不開啟自動連線由人員啟動   //Color 改紅色
        fLotInfo->pl_ATC_Online->Caption="ATC Off Line";                        //Ifor 20170609 (wei) add KYEC ATC 不開啟自動連線由人員啟動   //顯示 ATC Offline
    }

    if(CUSTOMER_CODE==CC_PTI &&                                                 //RogerYang 20170417 LotInfo更新至FT Mode
       IniConfig.bB03_TesterReport==false)                                      //Sam 20240809 : PTI ART 模式
    {
        cbRunMode->Enabled=false;
    }

    if(CUSTOMER_CODE==CC_ONSEMI_M)
    {
        labDeviceName->Caption="Device ID";
        labDeviceName->Visible=true;
        edtDevice->Visible=true;
    }

    if(CUSTOMER_CODE==CC_JSCC_OS)                                               //RogerYang 20260127 : Add For JSCC_OS download by Device list
    {
        int iTopTmp=edtSysLotID->Top;
        edtSysLotID->Top=Panel28->Top+2;
        labLotID->Top=edtSysLotID->Top+4;
        edtDevice->Top=iTopTmp;
        labDeviceName->Top=edtDevice->Top+4;
        btnFTPDownLoadbyDeviceID->Top=edtDevice->Top-2;
        Panel28->Top=edtSysLotID->Top+20;
        labDeviceName->Caption="Device ID";
        labDeviceName->Visible=true;
        edtDevice->Visible=true;
        btnFTPDownLoadbyDeviceID->Visible=true;
        rgUnloader->ItemIndex=fTesterTCP->rgUnloader->ItemIndex;
    }

    if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                      //Ifor 20170505 (wei) add 矽格中興使用LotID不顯示Run Mode
    {
        cbRunMode->Visible      =false;
        lbLotRunMode->Visible   =false;
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {
        labLotID->Visible=false;                                                //JerryYang 20220923 : add
        edtSysLotID->Visible=false;
        lblOPID->Visible=false;
        edtSysOperatorID->Visible=false;
        lbLotRunMode->Visible=false;
        cbRunMode->Visible=false;

        sbSECSLotStart->Caption="Start Lot";                                    //Sam 20170925 : 顯示名稱修改
        sbSECSLotEnd->Caption="End Lot";
        btnFtpTester->Visible=false;                                            //Sam 20171006 (wei) : 超豐用不到隱藏起來
        btnFtpServer->Caption ="Download To Handler";                           //Sam 20171006 (wei) : 超豐要求顯示名稱修改
        btnFtpHD->Caption="Upload To Server";                                   //Sam 20171006 (wei) : 超豐要求顯示名稱修改
        //Sam 20170916 (Steven) 移植超豐外部呼叫執行檔功能 form HT-7045
        //==>
        sb_RunExecutFile->Visible=true;
        if(IniConfig.asA25RunExecutButtonName!="")
        {
            sb_RunExecutFile->Caption=IniConfig.asA25RunExecutButtonName;
        }
        else
        {
            sb_RunExecutFile->Caption="Run Execut";
        }
        //<==
        //Sam 20170916 (Steven) 移植超豐外部呼叫執行檔功能 form HT-7045
    }

    tsMurata->TabVisible    =(CUSTOMER_CODE==CC_Murata);
    edtLine->Visible        =(CUSTOMER_CODE==CC_Murata);

    edtProcessName->Visible =(CUSTOMER_CODE==CC_Murata || IniConfig.bVTESTFunction==true);
    edtProduct->Visible     =(CUSTOMER_CODE==CC_Murata || IniConfig.bVTESTFunction==true);
    tsVTest->TabVisible     =(IniConfig.bVTESTFunction==true);

    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                     //jou 20191008 : (Steven) add SCC使用Lot ID
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT");
        cbRunMode->Items->Add("RT1");
        cbRunMode->Items->Add("RT2");
        cbRunMode->Text="FT";
    }
    else if(IniConfig.bVTESTFunction==true)
    {
        pgcLotInfo->ActivePage=tsVTest;
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT0");                                           //RogerYang 20260420 : 何航航要求增加站點(第二次修改)
        cbRunMode->Items->Add("FT1");
        cbRunMode->Items->Add("FT2");
        cbRunMode->Items->Add("FT3");
        cbRunMode->Items->Add("FT4");
        cbRunMode->Items->Add("FT5");
        cbRunMode->Items->Add("FT6");
        cbRunMode->Items->Add("FT7");
        cbRunMode->Items->Add("FT8");
        cbRunMode->Items->Add("FT9");
        cbRunMode->Items->Add("FT10");
        cbRunMode->Items->Add("FT11");
        cbRunMode->Items->Add("FT12");
        cbRunMode->Items->Add("FT13");
        cbRunMode->Items->Add("FT14");
        cbRunMode->Items->Add("FT15");
        cbRunMode->Items->Add("RT0");                                           //RogerYang 20260420 : 何航航要求增加站點
        cbRunMode->Items->Add("RT1");
        cbRunMode->Items->Add("RT2");
        cbRunMode->Items->Add("RT3");
        cbRunMode->Items->Add("RT4");
        cbRunMode->Items->Add("RT5");
        cbRunMode->Items->Add("EQC0");                                          //RogerYang 20260420 : 何航航要求增加站點
        cbRunMode->Items->Add("EQC1");
        cbRunMode->Items->Add("EQC2");
        cbRunMode->Items->Add("EQC3");
        cbRunMode->Items->Add("EQC4");
        cbRunMode->Items->Add("EQC5");
        cbRunMode->Text="FT1";

        cbTestTimes->Items->Clear();                                            //RogerYang 20250809 偉測Summary文件修改
        cbTestTimes->Items->Add("RP0");
        cbTestTimes->Items->Add("RP1");
        cbTestTimes->Items->Add("RP2");
        cbTestTimes->Items->Add("RP3");
        cbTestTimes->Items->Add("RP4");
        cbTestTimes->Items->Add("RP5");
        cbTestTimes->Items->Add("RP6");
        cbTestTimes->Items->Add("RP7");
        cbTestTimes->Items->Add("RP8");
        cbTestTimes->Items->Add("RP9");
        cbTestTimes->Items->Add("RP10");
        cbTestTimes->Items->Add("RP11");
        cbTestTimes->Items->Add("RP12");
        cbTestTimes->Items->Add("RP13");
        cbTestTimes->Items->Add("RP14");
        cbTestTimes->Items->Add("RP15");
        cbTestTimes->Text="RP0";

        lbProcess->Visible=true;
        cbProcess->Visible=true;

        grpMesCheck->Visible=true;

        edtProcessName->Parent=tsVTest;
        edtProcessName->EditLabel->Caption="CustPart";
        edtProcessName->Left=70;
        edtProcessName->Top=5;
        edtProcessName->Enabled=true;

        edtProduct->Parent=tsVTest;
        edtProduct->EditLabel->Caption="CustLotNum";
        edtProduct->Left=70;
        edtProduct->Top=edtProcessName->Top+27;
        edtProduct->Enabled=true;

        sbSECSLotStart->Top=lbLotRunMode->Top+lbLotRunMode->Height+55;
        sbSECSLotEnd->Top=sbSECSLotStart->Top+35;

        btnFtpTester->Visible=false;
//        tsSelection->TabVisible=((tsDeviceInfo->TabVisible || IniConfig.bEnableErms==true || IniConfig.bEnableFTP==true));

        labConfigL04->Visible=true;

        //jou 20231101 : 客戶要求 selection Temperature Offset 預設不勾，反灰處理
        chkTempOffset->Enabled=false;
        chkTempOffset->Checked=false;
        WriteIniData(AuthPath+"Security_new.def", "Network", "Temp Offset",     false);
        //jou 20240118 : 客戶要求 selection Auto Clean 預設不勾，反灰處理
        checkbAutoClean->Enabled=false;
        checkbAutoClean->Checked=false;
        WriteIniData(AuthPath+"Security_new.def", "Network", "Auto Clean",     false);

        grpOEEState->Visible=true;
        grpOEEState->Left=0;
        sgOEEState->Cells[0][0]="OEE";
        sgOEEState->Cells[0][1]="TimeOEE";
        sgOEEState->Cells[0][2]="Jam";
        sgOEEState->Cells[0][3]="Retest";
        sgOEEState->Cells[0][4]="Down";
        sgOEEState->Cells[0][5]="Setup";
        sgOEEState->Cells[0][6]="Idle";
        sgOEEState->Cells[0][7]="ENG";
        sgOEEState->Cells[0][8]="PM";
        sgOEEState->Cells[0][9]="Stop";
    }
    else if(CUSTOMER_CODE==CC_Murata)
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("FT");
        cbRunMode->Items->Add("RT");
    }
    else if(IniConfig.bSPILFunction==true && LastSet.iTester==_2D_SORT)         //JerryYang 20230322 : 2D sort lot info UI修改
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("VS");
        cbRunMode->Items->Add("RC1");
        cbRunMode->Items->Add("RC2");
        labCusLotID->Visible=true;
        labCusDevGrp->Visible=true;
        labDeviceName->Visible=true;
        edtCusLotID->Visible=true;
        edtCusDevGrp->Visible=true;
        edtDevice->Visible=true;
        edtCusLotID->Enabled=false;                                             //JerryYang 20230322 : 2D sort lot info UI修改
        edtCusDevGrp->Enabled=false;
//        edtDevice->Enabled=false;
        labCusStep->Visible=true;                                               //JerryYang 20260201 : add
        edtCusStep->Visible=true;
        edtCusStep->Enabled=true;
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("Normal Test");
        cbRunMode->Items->Add("Pre-Test");
        cbRunMode->Items->Add("Re-Test");
        cbRunMode->Items->Add("GD");
        cbRunMode->Items->Add("EQC");
        labLotID->Visible=false;
        edtSysLotID->Visible=false;
        lblOPID->Visible=false;
        edtSysOperatorID->Visible=false;
        lbLotRunMode->Visible=false;
        cbRunMode->Visible=false;
        pgLotinfo->ActivePage=tsLotID;
    }
    else if(TestIF_File.b2DIDAllowList &&                                       //JerryYang 20241104 : 支援2DID白名單功能
            IniConfig.iN23DownloadMethod!=2)                                    //JerryYang 20250320 : 2DID白名單功能
    {
        if(CUSTOMER_CODE==CC_JCET)                                              //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
        {
            bool bFlag=false;
            bFlag=fBarCode->JCETUseMakeWhite2DIDList();
            if(bFlag==true && cbRunMode->Enabled==true)
            {
                cbRunMode->Items->Clear();
                cbRunMode->Items->Add("FT1");
                cbRunMode->Items->Add("FT2");
                cbRunMode->Items->Add("FT3");
                cbRunMode->Items->Add("FT4");
                cbRunMode->Items->Add("FT5");
                cbRunMode->Items->Add("FT6");
                cbRunMode->Items->Add("FT7");
                cbRunMode->Items->Add("FT8");
                cbRunMode->Items->Add("FT9");
                cbRunMode->Items->Add("QA1");                                   //RogerYang 20260615 : 凌中心需要QA mode
                cbRunMode->Items->Add("QA2");
                cbRunMode->Items->Add("QA3");
                cbRunMode->Items->Add("QA4");
                cbRunMode->Items->Add("QA5");
            }
            edtSysOperatorID->Visible=true;
            lblOPID->Visible=true;
            bFlag=fBarCode->JCETUseMakeWhite2DIDList();
            labCusLotID->Visible=bFlag;
            edtCusLotID->Visible=bFlag;
        }
        else
        {
            edtSysOperatorID->Visible=false;
            lblOPID->Visible=false;
            labCusLotID->Visible=false;                                         //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
            edtCusLotID->Visible=false;                                         //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
        }
        labDeviceName->Visible=false;
        edtDevice->Visible=false;
        lbProcess->Visible=false;
        cbProcess->Visible=false;
//        lbLotRunMode->Visible=false;
//        cbRunMode->Visible=false;
    }
    else if(CUSTOMER_CODE==CC_SCC ||                                            //Steven 20250314 : add for JSCC
            CUSTOMER_CODE==CC_SJ_Semiconductor)
    {
        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("P1");
        cbRunMode->Items->Add("P2");
        cbRunMode->Items->Add("P3");
        cbRunMode->Items->Add("P4");
        cbRunMode->Items->Add("P5");
        cbRunMode->Items->Add("P6");
        cbRunMode->Items->Add("P7");
        cbRunMode->Items->Add("P8");
        cbRunMode->Items->Add("P9");
        cbRunMode->Items->Add("P10");
        cbRunMode->Items->Add("P11");
        cbRunMode->Items->Add("P12");
        cbRunMode->Items->Add("P13");
        cbRunMode->Items->Add("P14");
        cbRunMode->Items->Add("P15");
        cbRunMode->Items->Add("RT1");
        cbRunMode->Items->Add("RT2");
        cbRunMode->Items->Add("RT3");
        cbRunMode->Items->Add("RT4");
        cbRunMode->Items->Add("RT5");
        cbRunMode->Items->Add("RT6");
        cbRunMode->Items->Add("RT7");
        cbRunMode->Items->Add("RT8");
        cbRunMode->Items->Add("RT9");
        cbRunMode->Items->Add("RT10");
        cbRunMode->Items->Add("RT11");
        cbRunMode->Items->Add("RT12");
        cbRunMode->Items->Add("RT13");
        cbRunMode->Items->Add("RT14");
        cbRunMode->Items->Add("RT15");
        cbRunMode->Text="P1";
    }
    else if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport)               //Sam 20240809 : PTI ART 模式
    {
        lbProcess->Visible=true;
        cbProcess->Visible=true;
        cbProcess->Items->Clear();
        cbProcess->Items->Add("Sample");
        cbProcess->Items->Add("100%");
        cbProcess->Text="100%";

        cbRunMode->Items->Clear();
        cbRunMode->Items->Add("1'st");
        cbRunMode->Items->Add("2'nd");
        cbRunMode->Items->Add("3'th");
        cbRunMode->Items->Add("4'th");
        cbRunMode->Items->Add("5'th");
        cbRunMode->Items->Add("6'th");
        cbRunMode->Items->Add("7'th");
        cbRunMode->Items->Add("8'th");
        cbRunMode->Items->Add("9'th");
        cbRunMode->Items->Add("10'th");
        cbRunMode->Items->Add("11'th");
        cbRunMode->Items->Add("12'th");
        cbRunMode->Items->Add("13'th");
        cbRunMode->Items->Add("14'th");
        cbRunMode->Items->Add("15'th");
        cbRunMode->Items->Add("16'th");
        cbRunMode->Items->Add("17'th");
        cbRunMode->Items->Add("18'th");
        cbRunMode->Items->Add("19'th");
        cbRunMode->Items->Add("20'th");
        cbRunMode->Text="1'st";
    }

    if(CUSTOMER_CODE==CC_GIGAS)
    {
        btnFtpTester->Visible=false;                                            //沒用到
        btnFTPTryConnect->Visible=true;                                         //測試FTP是否有連線(按鈕)
        lbFTPStatus->Visible=true;                                              //測試FTP是否有連線(狀態顯示)
        btnFtpServer->Caption ="Barcode Download";                              //Server->Barcode Download
        btnFtpHD->Caption="HD Upload";                                          //HD->HD Upload
        BtnPause->Visible=false;
        if(IniConfig.bEnableFTP==true)
        {
            TCheckBox *tempTCheckBox[17]=
            {
                chkContactHigh,chkContactForce,chkContactMode,checkbAutoClean,chkART,
                chkART_RTCount,chkCleanCount,chkHotPlate,chkLoadUnload,chkSpeedSetting,
                chkShuttleMode,chkTestMode,chkBinasgn,chkBinasgnOff,chkIndexHeatingMode,
                cbBottom2DOffset,chkAutoCleanContactHeight
            };
            for(int i=0;i<17;i++)
            {
                tempTCheckBox[i]->Visible=false;
                tempTCheckBox[i]->Checked=true;
            }
            grpMesCheck->Visible=false;
        }
        tsLotID->TabVisible=false;
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE)                                         //Ifor 20251028 add: KLT 要求不顯示畫面
    {
        BtnPause->Visible=false;
    }
    else
    {
        BtnPause->Visible=(CUSTOMER_CODE==CC_FOREHOPE_NINGBO);                  //Steven 20251125 : change
    }
    tsASEMARMS->TabVisible=CosFunction.bUseARMSFunction;                        //Ifor 20170621 (wei) add ARMS Function
    tsASECLEventLog->TabVisible=(CUSTOMER_CODE==CC_ASE_CL ||
                                 CUSTOMER_CODE==CC_HANA_MICRON ||               //JimmyChiu 20211008 R211005-Hana-H9-01
                                 CUSTOMER_CODE==CC_SJ_Semiconductor ||          //Steven 20221128 : Add Socket Id for SJSM
                                 CosFunction.bUseSocketContactCount);

    pnlLotInfo_ASECL->Visible=(CUSTOMER_CODE==CC_ASE_CL);                       //Steven 20221214 : fixed for SJSM
    pnlLotStart_ASECL->Visible=(CUSTOMER_CODE==CC_ASE_CL);

    if(CosFunction.bEnableHandlerResultServer)                                  // && IniConfig.bA60EnableAMR)    //Sam 20240304 : 新增 AMR 功能
    {
        tsAMR->TabVisible=true;
        RefreshAMR();
        ShowAMRCategoryBin(true);
    }
    else
    {
        tsAMR->TabVisible=false;
    }

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20250120 : add
    {
         for(int i=0; i<4; i++)
        {
            for(int j=0; j<8; j++)
            {
                edSocket[i][j]->Text=AnsiString(LastSet.strSocketID[i][j]);
            }
        }
    }

    if(IniConfig.bEnableFTP)
    {
        if((CUSTOMER_CODE==CC_JSCC_OS))                                         //RogerYang 20260127 : Add For JSCC_OS 田揚志說LOTInfo擺第一頁
        {
            pgLotinfo->ActivePage=tsLotID;
        }
        else
        {
            pgLotinfo->ActivePage=tsFTP;
        }
    }
    else if(IniConfig.bEnableRms==true)
        pgLotinfo->ActivePage=tsDeviceInfo;
    else if(CosFunction.bOEEFunction)
        pgLotinfo->ActivePage=tsLotID;
    else if(REAL_TIME_CCD)
        pgLotinfo->ActivePage=tsRTCFullViewImg;
    else if(ATC_SYSTEM==eWinWay && Temperature.bATCActiveCooling==true)         //jimmychiu 20210906
        pgLotinfo->ActivePage=ATC_WinWay;
    else if(ATC_SYSTEM!=eATCUninstall && ATC_SYSTEM!=eNonChamber)
        pgLotinfo->ActivePage=tsATC;
    else if(Tri_Temp_Machine==1)                                                //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        pgLotinfo->ActivePage=ts_ATC6_1;

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20190701 SPIL要求主畫面可切換EQC mode
    {
        lblPage->Visible=false;
        edPage->Visible=false;
        if(LastSet.iTester!=_2D_SORT && TestIF_File.b2DIDAllowList==false)      //JerryYang 20241104 : 支援2DID白名單功能  //JerryYang 20230322 : 2D sort lot info UI修改
        {
            palQAMode->Visible=true;
            labLotID->Caption="SPIL LOT ID-STAGE-STEP";
            edtSysLotID->Width=150;
            edtSysLotID->Left=150;
            lbLotRunMode->Visible=false;
            cbRunMode->Visible=false;
        }
        else
        {
            if(TestIF_File.b2DIDAllowList==true)                                //JerryYang 20241104 : 支援2DID白名單功能
            {
                lbLotRunMode->Visible=true;
                cbRunMode->Visible=true;
                cbRunMode->Items->Clear();
                cbRunMode->Items->Add("Normal");
                cbRunMode->Items->Add("RT");
                cbRunMode->Items->Add("EQC");
                cbRunMode->Items->Add("CORR");
            }
            else
            {
                lbLotRunMode->Visible=true;
                cbRunMode->Visible=true;
            }

            labJobSeq->Visible=false;
            edtJobSeq->Visible=false;
        }
    }
    //==> Eastsun 20260526 #026-1.80 Ifor 20201112 add:KYEC 要求QA Mode 改成EQC Mode
    else if(CUSTOMER_CODE==CC_KYEC_LEE && IniConfig.bQAMode==true)
    {
        labQACount->Caption="EQC Mode Device Counts:";
        labQACount->Font->Size=8;
        edQAMode->Left=136;
        btnQAmodeSave->Left=210;
        palQAMode->Visible=true;
    }
    //<== Eastsun 20260526 #026-1.80
    else if(CUSTOMER_CODE==CC_JCET)                                             //RogerYang 20260625 : 葉宏瑋要拉長
    {
        edtSysLotID->Width=180;
    }
    else
    {
        palQAMode->Visible=false;
        labLotID->Caption="Lot ID : ";                                          //JerryYang 20220923 : add
    }

    ShowSocketID();                                                             //JerryYang 20190702 ASE-CL顯示SocketID

    pan_DewPoint->Visible=(DewPoint_Hardware_Install>0);                        //Steven 20191017 : 露點計
    pl_DewPoint->Visible=(DewPoint_Hardware_Install>0);                         //Steven 20191017 : 露點計

    myInShuttleLotInfo->SetColorMap(0, TColor(0x00DFD9CC));
    myInShuttleLotInfo->SetColorMap(1, clGreen);
    myInShuttleLotInfo->SetColorMap(2, clRed);
    myInShuttleLotInfo->SetColorMap(3, clBtnFace);

    mtBarcodeInShLotInfo->SetColorMap(0, TColor(0x00DFD9CC));
    mtBarcodeInShLotInfo->SetColorMap(1, clGreen);
    mtBarcodeInShLotInfo->SetColorMap(2, clRed);
    mtBarcodeInShLotInfo->SetColorMap(3, clBtnFace);

    myInShuttleLotInfo->SetCellNumber(0, 0, "In Shuttle");
    myInShuttleLotInfo->SetCellColorIndex(0, 0, 3);
    myInShuttleLotInfo->SetCellNumber(0, 1, "a");
    myInShuttleLotInfo->SetCellColorIndex(0, 1, 3);
    myInShuttleLotInfo->SetCellNumber(0, 2, "b");
    myInShuttleLotInfo->SetCellColorIndex(0, 2, 3);
    myInShuttleLotInfo->SetCellNumber(0, 3, "c");

    mtBarcodeInShLotInfo->SetCellNumber(0, 0, "Shuttle1_1");
    mtBarcodeInShLotInfo->SetCellColorIndex(0, 0, 3);
    mtBarcodeInShLotInfo->SetCellNumber(1, 0, "Shuttle1_2");
    mtBarcodeInShLotInfo->SetCellColorIndex(1, 0, 3);

    mtBarcodeInShLotInfo->SetCellNumber(2, 0, "Shuttle2_1");
    mtBarcodeInShLotInfo->SetCellColorIndex(2, 0, 3);
    mtBarcodeInShLotInfo->SetCellNumber(3, 0, "Shuttle2_2");
    mtBarcodeInShLotInfo->SetCellColorIndex(3, 0, 3);

    if(TestIF_File.i2DIDFormat==eAMD)                                           //JerryYang 20200422 2DID format選項改用下拉選單
    {
        grpBarcodeDisplayLotInfo->Visible=true;
    }
    else
    {
        grpBarcodeDisplayLotInfo->Visible=false;
    }

    tmrChamberBoost->Enabled=(CosFunction.bUseChamberBoostMode);                //Steven 20191128 : Chamber Boost Function
    AnsiString sPath=AuthPath+"config.ini";
    SetLotStart("fLotInfo::FormShow", true);                                    //Steven 20250515 : 整合Open Short測試報表

    edtJobSeq->Text=ReadIniData(sPath, "Lot Info", "Job Sequence", AnsiString(""));                                     //JerryYang 20220923 : add

//    if(((CUSTOMER_CODE==CC_SCC ||                                               //Steven 20200306 : SCC要求按下Lot End的時候, 資料要清空
//         CUSTOMER_CODE==CC_Murata ||
//         CUSTOMER_CODE==CC_SJ_Semiconductor ||
//         CUSTOMER_CODE==CC_CYUEAN) &&                                           //Steven 20240122 : add for CyuEan
    if(edtSysLotID->Text!="" ||
       (CosFunction.bLotStartLockCriticalPara &&                                //JerryYang 20220311 : ATP鎖定Critical parameter
        LastSet.iTester==ON_LINE &&
        (fMain->CheckCanChangeRealDummy()==false ||
         HasICUnderMachine())))
    {
        fMain->cbRunStartMode->Enabled=false;
        edtBarcodeRecipe->Enabled=false;                                        //Ifor 20241108 add:Barcode Multi Recipe   //Eastsun 20260527 整合 #027-2.MR.U1 edtBarcodeRecipe disable :KYEC

        if(CosFunction.bUseLogUploadToFTPFunction==true)                        //Steven 20240925 : Fixed for log uoload
        {
            bSysLotStart      =true;                                            //Ifor 20160302 add for KYEC_HS LotStart
            bEPLogStart_KYEC  =true;                                            //Ifor 20160302 add for KYEC_HS EPLogStart
            bTempLogStart_KYEC=true;                                            //Ifor 20160302 add for KYEC_HS TempLogStart
            if(USE_NOVX3360==true)
            {
                bESDLogStart_KYEC=true;                                         //Ifor 20160302 add for KYEC_HS ESD
            }
            asATCEvenLotID=fLotInfo->edtSysLotID->Text;                         //Ifor 20170124 (Steven) :add LotID By ATC Even Log
            bArmTestInfoEvenLogStart_KYEC=true;                                 //Ifor 20190912 :add 海思 V02.30 版 Record Torque
        }
    }

    if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)              //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        edQAMode->Enabled=!bAuthCriticalPara[14];
    }

    if(CUSTOMER_CODE==CC_ASE_SG)
    {
        btnClearTemperature->Visible=true;
    }
    else
    {
        btnClearTemperature->Visible=false;
    }

    if(CUSTOMER_CODE==CC_LEADYO)                                                //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    {
        lblPage->Visible=false;
        edPage->Visible =false;
        Panel28->Visible=false;
    }

    if(CosFunction.bFirstTrayCheckOnUnloader)                                   //Jimmychiu 20251205 : First Tray Check On Unloader
    {
        tsOtherTool->TabVisible=true;
        cbFirstTrayCheckOnUnloader->Visible=true;
    }
    else
    {
        tsOtherTool->TabVisible=false;
        cbFirstTrayCheckOnUnloader->Visible=false;
    }
    lbShowDevName->Visible=CosFunction.bScanBarcodeAndDownloadFileInRMS;
    lbShowDevName->Caption="";

    if(iAQLBin==0)                                                              //Eastsun 20260520 整合
        iAQLBin=-1;                                                             //Eastsun 20260520 整合
    fLotInfo->lbLotAQLSetCount->Caption=IntToStr(iAQLCount);                    //Eastsun 20260520 整合
    fLotInfo->lbLotAQLSetBin->Caption=IntToStr(iAQLBin);                        //Eastsun 20260520 整合

    ReadWriteFTPAutomationData(true);                                           //KaiChen 20190530 ：Sigurd FTP Automation
    AdjtsYieldMonitiorSize();                                                   //Steven 20221225 : 統一Lot Info尺寸調整
    InitialRefrigerantSystem();                                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall && IniConfig.bA74AOIFailCountLinkLotRunMode)
        FrmAOI->AOIFailCountRefresh();                 //Eastsun 20260408 :
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void TfLotInfo::SettsChipAdvVisible()                                           //Steven 20250515 : 整合Open Short測試報表
{
    if(CUSTOMER_CODE==CC_CYUEAN)
    {
        tsChipAdv->TabVisible=true;
    }
    else if(CUSTOMER_CODE==CC_PANTHER ||
            CUSTOMER_CODE==CC_Greatek)
    {
        tsChipAdv->TabVisible=false;
    }
    else                                                                        //if(TestIF_File.iTestType==TCP_IP_MODE)
    {
        tsChipAdv->TabVisible=true;
        tsChipAdv->Caption="Lot Info";
    }
//    else
//    {
//        tsChipAdv->TabVisible=false;
//    }
}
//---------------------------------------------------------------------------
void TfLotInfo::SetSelectionVisible()                                           //Steven 20250519 : 統一Selection的顯示設定
{
//        tsSelection->TabVisible=((tsDeviceInfo->TabVisible ||
//                                  IniConfig.bEnableErms==true) &&               //Steven 20170621 (wei) : Add for R160624-ATK-H9-01 ATK Auto Recipe Downloading Function.
//                                 CUSTOMER_CODE!=CC_SCC &&
//                                 CUSTOMER_CODE!=CC_SCK);
    if(IniConfig.bVTESTFunction==true)                                          //jou 20221209 : VTEST開啟FTP下載工作檔需顯示tsSelection
    {
        if((IniConfig.bEnableRms==true || IniConfig.bEnableFTP==true) &&
            AccessLevel>=iDefEngineerLevel)
        {
            if(AccessLevel>=iDefSupervisorLevel)
            {
                groupbDownloadItem->Visible=true;
                grpMesCheck->Visible=true;
            }
            else
            {
                groupbDownloadItem->Visible=false;
                grpMesCheck->Visible=false;
            }
            tsSelection->TabVisible=true;
        }
        else
        {
            tsSelection->TabVisible=false;
        }
//        #ifdef BETA_VTestSummaryFile                                            //RogerYang 20250809 偉測Summary文件修改
            Panel28->Width=285;                                                 //RogerYang 20250822 避免cbTestTimes被遮住
            cbTestTimes->Visible=True;
            lblTestTimes->Visible=True;
//        #endif
    }
    else
    {
        /*if(tsDeviceInfo->TabVisible==true &&
           AccessLevel>=iDefSupervisorLevel)                                    //jou 2014-06-19 Security Have 5 Level 2->iDefSupervisorLevel
        {
            if(CUSTOMER_CODE==CC_SCC)
                tsSelection->TabVisible=false;
            else
                tsSelection->TabVisible=true;
        }  */
//        if(CUSTOMER_CODE==CC_SCC ||
//           CUSTOMER_CODE==CC_SCK)
//        {
//            tsSelection->TabVisible=false;
//        }
//        else
//        {
        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //Ifor 20251028 add: KLT 要求不顯示畫面
        {
            tsSelection->TabVisible=false;
        }
        else
        {
            if(IniConfig.bEnableRms==true ||
               IniConfig.bEnableErms==true ||                                   //Steven 20170621 (wei) : Add for R160624-ATK-H9-01 ATK Auto Recipe Downloading Function.
               IniConfig.bEnableFTP==true ||
               IniConfig.bSPILFunction)
                tsSelection->TabVisible=true;
            else
                tsSelection->TabVisible=false;
        }
//        }
    }
    if(IniConfig.bA75DownloadItemByAccessLevel==true)                           //AI(safety-critical-change) 20260611 (RogerYang) : 開關啟用時依權限控管下載項目
    {
        bool bDenyByOP=(AccessLevel==0);                                        //OP權限禁止操作
        for(int i=0; i<groupbDownloadItem->ControlCount; i++)                   //逐一控制子項,跳過警告Label避免變灰
        {
            if(groupbDownloadItem->Controls[i]!=lblDownloadAccessWarning)
                groupbDownloadItem->Controls[i]->Enabled=!bDenyByOP;
        }
        lblDownloadAccessWarning->Visible=bDenyByOP;                            //權限不足才顯示紅字
    }
    else
    {
        lblDownloadAccessWarning->Visible=false;                                //功能未啟用,永遠隱藏
    }
}
//---------------------------------------------------------------------------
extern int iTestHeadMotorTask;
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");//Steven 20120615 : 改成可用序號(Mode)或名稱(ModeText)進行模式改變
void __fastcall TfLotInfo::sbSECSLotEndClick(TObject *Sender)                   //ChungHung OLP 聚成專用      //wei 20150324 更改元件Button-->SpeedButton
{
    AnsiString Msg;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(SystemStart && Ptr->Tag==0)                                              //Sam 20200226 : OLP 增加 LotStar & LotEnd 控制
    {
        sbSECSLotStart->Down=true;
        return;
    }

    if(CosFunction.bAutoCleanShuttleDisable==false)                             //wei 20150326 CC_KYEC 隨時都可更改不需要clean out
    {
        if(fMain->CheckCanChangeRealDummy()==false)
        {
            sbSECSLotStart->Down=true;
            SetRunStartMode((eRunStartMode)LastSet.iRunStartMode);
            ShowErrorMessage("MES1646", 0, MMSystem, false, "SECSLotEndClick1");                                        //Must finish [Clean out]!!
            return;
        }

        if(iTestHeadMotorTask!=1 && fAllMotorHome==true)
        {
            sbSECSLotStart->Down=true;
            SetRunStartMode((eRunStartMode)LastSet.iRunStartMode);
            ShowErrorMessage("MES1646", 0, MMSystem, false, "SECSLotEndClick2");                                        //Must finish [Clean out]!!
            return;
        }
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {                                                                           //JimmyChiu 20220118 相似功能整合
        if(fProductionInfo->_bOEEStartLotSuccess==false)                        //Sam 20170810 (Steven) 移植超豐 OEE 功能 form HT-7045
        {
            ShowMyMessage("Please Lot Start!");
            return;
        }

        if(Application->MessageBox("Are You Sure End Lot?", "End Lot?", MB_YESNO)!=IDYES)
        {
            return;
        }
    }

    SetLotEnd(__FUNC__);                                                        //Steven 20250515 : 整合Open Short測試報表

    fMain->cbRunStartMode->Enabled=true;
    fMain->palFT->Enabled=true;
    fMain->palRT->Enabled=true;
    fMain->palOffLine->Enabled=true;
    fMain->palEQC->Enabled=true;
    bLotID_OK=false;
    bOPID_OK=false;
    bTemp_OK=false;
    bDeviceName_OK=false;
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::ReadWriteLotInfo(bool bRead)                         //Steven 20250515 : 整合Lot測試報表 Lot End
{
    AnsiString sPath=AuthPath+"config.ini";

    if(bRead)
    {
        lbledtStarTime      ->Text=ReadIniData(sPath, "Lot Info", "Start Time",     AnsiString(""));
        lbledtEndTime       ->Text=ReadIniData(sPath, "Lot Info", "End Time",       AnsiString(""));
        lbledtTesterOsVer   ->Text=ReadIniData(sPath, "Lot Info", "Tester OS Ver",  AnsiString(""));
        lbledtTesterID      ->Text=ReadIniData(sPath, "Lot Info", "Tester ID",      AnsiString(""));
        edtSysOperatorID    ->Text=ReadIniData(sPath, "Lot Info", "Operator",       AnsiString(""));
        lbledtCustomer      ->Text=ReadIniData(sPath, "Lot Info", "Customer",       AnsiString(""));
        edCustomerLotId     ->Text=ReadIniData(sPath, "Lot Info", "Customer Lot ID",AnsiString(""));
        lbledtTestProg      ->Text=ReadIniData(sPath, "Lot Info", "Test Program",   AnsiString(""));
        lbledtDeviceName    ->Text=ReadIniData(sPath, "Lot Info", "Device Name",    AnsiString(""));
        edtStage            ->Text=ReadIniData(sPath, "Lot Info", "Stage",          AnsiString(""));
        edtStep             ->Text=ReadIniData(sPath, "Lot Info", "Step",           AnsiString(""));
        edtSysLotID         ->Text=ReadIniData(sPath, "Lot Info", "Lot No",         AnsiString(""));
        lbledtSubLotNo      ->Text=ReadIniData(sPath, "Lot Info", "Sub Lot No",     AnsiString(""));
        lbledtModeCode      ->Text=ReadIniData(sPath, "Lot Info", "Mode Code",      AnsiString(""));
        lbledtTestCode      ->Text=ReadIniData(sPath, "Lot Info", "Test Code",      AnsiString(""));
        lbledtTestBinNo     ->Text=ReadIniData(sPath, "Lot Info", "Test Bin No",    AnsiString(""));
        if(fBarCode->JCETUseMakeWhite2DIDList()==true)                          //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
        {
            cbRunMode->Text=ReadIniData(sPath, "Lot Info", "Run Mode 2DID",       AnsiString(""));
            for(int i=0; i<cbRunMode->Items->Count; i++)
            {
                if(cbRunMode->Items->Strings[i]==cbRunMode->Text)
                {
                    cbRunMode->ItemIndex=i;
                    break;
                }
            }
        }
        else
        {
            cbRunMode       ->Text=ReadIniData(sPath, "Lot Info", "Run Mode",       AnsiString(""));
        }
        edtCusLotID         ->Text=ReadIniData(sPath, "Lot Info", "Customer Lot ID",AnsiString(""));                    //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能

        lbledtMachineID     ->Text=IniConfig.SocketHandlerID;
        coStation           ->Text=ReadIniData(sPath, "Lot Info", "Station",        AnsiString(""));
        edStationNum        ->Text=ReadIniData(sPath, "Lot Info", "Station Number", AnsiString(""));
        edtLotEventLogName  ->Text=ReadIniData(sPath, "Lot Info", "EventLogFile",   AnsiString(""));                    //Steven 20250809 : 修正event log上傳

        for(int i=0; i<3; i++)                                                  //Steven 20250603 : By Lot Summary
        {
            ArmDataLot[i]->ReadFile();
        }
    }
    else
    {
        WriteIniData(sPath, "Lot Info", "Start Time",       lbledtStarTime      ->Text);
        WriteIniData(sPath, "Lot Info", "End Time",         lbledtEndTime       ->Text);
        WriteIniData(sPath, "Lot Info", "Tester OS Ver",    lbledtTesterOsVer   ->Text);
        WriteIniData(sPath, "Lot Info", "Tester ID",        lbledtTesterID      ->Text);
        WriteIniData(sPath, "Lot Info", "Operator",         edtSysOperatorID    ->Text);
        WriteIniData(sPath, "Lot Info", "Customer",         lbledtCustomer      ->Text);
        WriteIniData(sPath, "Lot Info", "Customer Lot ID",  edCustomerLotId     ->Text);
        WriteIniData(sPath, "Lot Info", "Test Program",     lbledtTestProg      ->Text);
        WriteIniData(sPath, "Lot Info", "Device Name",      lbledtDeviceName    ->Text);
        WriteIniData(sPath, "Lot Info", "Lot No",           edtSysLotID         ->Text);
        WriteIniData(sPath, "Lot Info", "Sub Lot No",       lbledtSubLotNo      ->Text);
        WriteIniData(sPath, "Lot Info", "Mode Code",        lbledtModeCode      ->Text);
        WriteIniData(sPath, "Lot Info", "Test Code",        lbledtTestCode      ->Text);
        WriteIniData(sPath, "Lot Info", "Test Bin No",      lbledtTestBinNo     ->Text);
        if(fBarCode->JCETUseMakeWhite2DIDList()==true)                          //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
        {
            WriteIniData(sPath, "Lot Info", "Run Mode 2DID",    cbRunMode       ->Text);
        }
        else
        {
            WriteIniData(sPath, "Lot Info", "Run Mode",         cbRunMode       ->Text);
        }
        WriteIniData(sPath, "Lot Info", "Station",          coStation           ->Text);
        WriteIniData(sPath, "Lot Info", "Station Number",   edStationNum        ->Text);
        WriteIniData(sPath, "Lot Info", "Stage",            edtStage            ->Text);
        WriteIniData(sPath, "Lot Info", "Step",             edtStep             ->Text);
        WriteIniData(sPath, "Lot Info", "EventLogFile",     edtLotEventLogName  ->Text);                                //Steven 20250809 : 修正event log上傳
        WriteIniData(sPath, "Lot Info", "Customer Lot ID",  edtCusLotID         ->Text);                                //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能

        for(int i=0; i<3; i++)                                                  //Steven 20250603 : By Lot Summary
        {
            ArmDataLot[i]->WriteFile();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::SetLotID(AnsiString ID, bool bReadFromFile)          //Steven 20200416 : 整合Lot ID設置
{
    AnsiString sPath=AuthPath+"config.ini";
    if(bReadFromFile)
    {
        if(IniConfig.bN22Enable_EventLog)                                       //JerryYang 20220215 : ASE-CL lot info需求
        {
            edtASECL_LotID->Text=ReadIniData(sPath, "Lot Info", "Lot ID", AnsiString(""));

            edtASECL_OPID->Text=ReadIniData(sPath, "Lot Info", "OP_ID", AnsiString(""));
            edtASECL_TesterID->Text=ReadIniData(sPath, "Lot Info", "TESTER_ID", AnsiString(""));
            edtASECL_LoadBoard->Text=ReadIniData(sPath, "Lot Info", "LoadBoard_ID", AnsiString(""));
            cbRunModeASECL->Text=ReadIniData(sPath, "Lot Info", "RunMode", AnsiString(""));
            edInsertion->Text=ReadIniData(sPath, "Lot Info", "Insertion", AnsiString(""));
            edCustomerDevice->Text=ReadIniData(sPath, "Lot Info", "CusDevice", AnsiString(""));
            edFlowID->Text=ReadIniData(sPath, "Lot Info", "FlowID", AnsiString(""));

            bFTPDownlodFinish=ReadIniData(sPath, "Lot Info", "bFTPDownloadFinish", false);
        }

        edtSysLotID->Text=ReadIniData(sPath, "Lot Info", "Lot ID", AnsiString(""));
        edtASECL_LotID->Text=edtSysLotID->Text;

        if(fMain->hanaART->IsHanaArtAvailable())                                //Steven 20250414 : Hana ART
            RunInfo.LotStartTime=ReadIniData(sPath, "Lot Info", "LotStartTime", AnsiString("20240101000000"));
        else
            RunInfo.LotStartTime=ReadIniData(sPath, "Lot Info", "LotStartTime", AnsiString("2020-01-01 00:00:00"));

        if(edtSysLotID->Text!="")
        {
        }

        if(USE_RFID_READER)                                                     //Steven 20220713 : RFID Reader for SJSEMI
        {
            pnlLoader->Caption  =ReadIniData(sPath, "RFID", "Loader ID",   AnsiString(""));
//            pnlUnloader->Caption=ReadIniData(sPath, "RFID", "Unloader ID", AnsiString(""));
        }
    }
    else
    {
        edtASECL_LotID->Text=ID;
        edtSysLotID->Text=ID;

        if(IniConfig.bN22Enable_EventLog)                                       //JerryYang 20220215 : ASE-CL lot info需求
        {
            WriteIniData(sPath, "Lot Info", "bFTPDownloadFinish", bFTPDownlodFinish);
            WriteIniData(sPath, "Lot Info", "Lot ID", ID);
            WriteIniData(sPath, "Lot Info", "OP_ID", edtASECL_OPID->Text);
            WriteIniData(sPath, "Lot Info", "TESTER_ID", edtASECL_TesterID->Text);
            WriteIniData(sPath, "Lot Info", "LoadBoard_ID", edtASECL_LoadBoard->Text);
            WriteIniData(sPath, "Lot Info", "RunMode", cbRunModeASECL->Text);
            WriteIniData(sPath, "Lot Info", "Insertion", edInsertion->Text);
            WriteIniData(sPath, "Lot Info", "CusDevice", edCustomerDevice->Text);
            WriteIniData(sPath, "Lot Info", "FlowID", edFlowID->Text);
        }

        WriteIniData(sPath, "Lot Info", "Lot ID", ID);

        if(fMain->hanaART->IsHanaArtAvailable())                                //Steven 20250414 : Hana ART
            RunInfo.LotStartTime.sprintf("%04d%02d%02d%02d%02d%02d",
                                            SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        else
            RunInfo.LotStartTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",       //pig 2014.05.09 KYEC Issue
                                            SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

        WriteIniData(sPath, "Lot Info", "LotStartTime", RunInfo.LotStartTime);
        WriteIniData(sPath, "Lot Info", "Customer Lot ID", edtCusLotID->Text);  //Sam 20220223 : 矽格中興廠新增 Lot 資料
        if(USE_RFID_READER)                                                     //Steven 20220713 : RFID Reader for SJSEMI
        {
            WriteIniData(sPath, "RFID", "Loader ID",   pnlLoader->Caption);
        }
    }

    if(bReadFromFile && edtSysLotID->Text=="")
        RunInfo.LotNo.sprintf("[%04d%02d%02d.%02d%02d%02d]", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else
        RunInfo.LotNo=edtSysLotID->Text;
    tRecordOEE.SetSecAndOn(3600);
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::SetLotStart(AnsiString sFunc, bool bReadFromFile)    //Steven 20250515 : 整合Lot測試報表
{
    RecordProcess("Start Lot Press", sFunc);

    AnsiString sPath=AuthPath+"config.ini";
    AnsiString str, Msg;

    if(bReadFromFile)
    {
        SetLotID("", true);
        ReadWriteLotInfo(true);                                                 //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
        slEventLog->SetLotData(RunInfo.LotNo, lbledtStarTime->Text, edtLotEventLogName->Text);
    }
    else
    {
        SetLotID(edtSysLotID->Text);
        if(lbledtStarTime->Text=="")
        {
            str.sprintf("%04d%02d%02d_%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
            lbledtStarTime->Text=str;
        }

        for(int i=0; i<3; i++)                                                  //Steven 20250603 : By Lot Summary
        {
            ArmDataLot[i]->ClearALLCT();
        }

        slEventLog->SetLotData(RunInfo.LotNo, lbledtStarTime->Text);            //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
        //==> Eastsun 20260527 整合 #027-2.MR.U7 Lot Start log with Barcode Recipe :KYEC
        if(TestIF_File.bEnableBarCode==true &&
           BAR_CODE_INSTALL==ebctUseCCDMode &&
           TestIF_File.bBarCodeMultiRecipe==true)
        {
            Msg.sprintf("Lot Start, Lot ID:%s, OP ID:%s, Run Mode:%s, 2D Recipe:%s", edtSysLotID->Text, edtSysOperatorID->Text, cbRunMode->Text, edtBarcodeRecipe->Text);
        }
        else
        {
            Msg.sprintf("Lot Start, Lot ID:%s, OP ID:%s, Run Mode:%s", edtSysLotID->Text, edtSysOperatorID->Text, cbRunMode->Text);                                 //Steven 20200302 : Lot Start/end增加log
        }
        //<== Eastsun 20260527 #027-2.MR.U7
        RecordProcess(Msg, sFunc);
        ReadWriteLotInfo(false);
    }

    SetTesterStartTimeByB03();                                                  //Sam 20240809 : PTI ART 模式

    if(CosFunction.bRunModeFollowLotInfo)                                       //Steven 20250603 : 根據Lot Info的Run mode進行切換
    {
        if(cbRunMode->Text.AnsiPos("RT")==1)
        {
            fMain->DoFTRTClick(true, false);                                    //RogerYang 20260410 : 整合並區分手動按下還是程式按下
        }
        else if(cbRunMode->Text=="EQC")
        {
            fMain->palEQCClick(fMain);
        }
        else
        {
            //fMain->palFTClick(fMain);
            fMain->DoFTRTClick(false, false);                                   //RogerYang 20260410 : 整合並區分手動按下還是程式按下
        }
    }

    RunInfo.SetLotStartTime();                                                  //Sam 20240426 : Add BarCoder Inspection Report  Eastsun 20260527 整合#BCIR.P16 :KYEC

    if(IniConfig.bVTESTFunction==true)                                          //jou 20200409 : VTest Mes system
    {
        cbProcess->Enabled=false;
        edtProcessName->Enabled=false;
        edtProduct->Enabled=false;
        cbTestTimes->Enabled=false;                                             //RogerYang 20250809 偉測Summary文件修改
        if((IniConfig.bEnableRms || IniConfig.bEnableFTP) && IniConfig.bCheckFile)
        {
            if(fMesSystem->CheckVTENGmode(edtSysLotID->Text)==true)
            {
                fMesSystem->bDownloadLotInforFlag=true;
                bFTPDownloadSetupFile = true;
                LastSet.bHasDownloadFile=true;
                RecordProcess("ENG mode lot start");
            }

            LastSet.SendCT_ART[0]=0;
            LastSet.SendCT_ART[1]=0;
            LastSet.SendCT_ART[2]=0;
            LastSet.SendCT_ART[3]=0;

            for(int i=0; i<eTrayCount; i++)                                     //kevin 20110926  無法清fix6
            {
                LastSet.BinCT_ART[0][iTo3Unload[i]]=0;
                LastSet.BinCT_ART[2][iTo3Unload[i]]=0;
                LastSet.BinCT_ART[3][iTo3Unload[i]]=0;
            }

            for(int i=0; i<eTrayCount; i++)
            {
                LastSet.iBinData32_ART[0][i]=0;
                LastSet.iBinData32_ART[2][i]=0;
                LastSet.iBinData32_ART[3][i]=0;
            }

            ArmData[2]->ClearALLCT();

            if(CUSTOMER_CODE==CC_VTEST)
            {
                edtProcessName->Enabled=false;
                edtProduct->Enabled=false;
            }
        }

        fMesSystem->bFirstMaterialsQA=true;                                     //jou 20230207 : VTEST 首盤請執行送檢，通知QA確認
    }

    if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport)                    //Sam 20240809 : PTI ART 模式
        cbProcess->Enabled=false;

//    if((IniConfig.bSPILFunction==true ||                                      //Steven 20250806 : Mark
//        CUSTOMER_CODE==CC_SJ_Semiconductor) &&
    if(CosFunction.bSortingBy2DList==true &&
       LastSet.iTester==_2D_SORT &&
       TestIF_File.bSortingBy2DIDList==true &&
       RunInfo.bLotStart==false)
    {
        fMain->Clarn_Data(8, "LotStart_ClearData");
        fSCKART->ClearLotInfo();
        EventReport(SECS_EVENT.DoVisualSortLotStart);
    }
    else
    {
        if(CUSTOMER_CODE!=CC_TSMC_TAINAN &&
           IniConfig.bEnable_SECS_GEM==true &&                                  //Steven 20140528 : Secs Gem
           RunInfo.bLotStart==false)                                            //wei 20150821 不能連續送
            EventReport(SECS_EVENT.DoLotStart);
    }

    if(edtSysLotID->Text!="")
        SetLotComponents(false);
    else
        SetLotComponents(true);
    edtBarcodeRecipe->Enabled=false;   //==> Eastsun 20260527 整合#027-2.MR.M5 explicit LotStart disable (parity with a-side L7225) :KYEC
    //==> Eastsun 20260527 整合#027-2.MR.U6 Change2DSetupFile on LotStart :KYEC
    if(TestIF_File.bEnableBarCode==true &&
       BAR_CODE_INSTALL==ebctUseCCDMode &&
       TestIF_File.bBarCodeMultiRecipe==true)
    {
        fBarCode->Change2DSetupFile();
    }
    //<== Eastsun 20260527 整合#027-2.MR.U6

    if(TestIF.iTestType==TCP_IP_MODE)                                           //Steven 20230322 : for OS Tester
    {
        if((CUSTOMER_CODE==CC_SJ_Semiconductor_OS ||                            //Steven 20230213 : For SJSemi OS Tester
            CUSTOMER_CODE==CC_XINITECH) &&
           IniConfig.bEnable_SECS_GEM)
        {
                                                                                //透過SECS GEM傳送Lot ID
        }
        else
        {
            if(iRunStartMode==RT)
                str.sprintf("LOTNUMBER,%s,RT,", fLotInfo->edtSysLotID->Text);
            else
                str.sprintf("LOTNUMBER,%s,FT,", fLotInfo->edtSysLotID->Text);
            //AI(ht9045-v899) 20260525: mark PTI tester TCP command window for StateRecord analysis
            WritePTILotStartTrace(this, "BeforeTesterTCPSend", "Commands=LOTNUMBER/LOTSTART/OPERATORID/GETOSSETUP");
            fTesterTCP->SendTCPIPCommand(0, "LOTNUMBER", str);
            fTesterTCP->SendTCPIPCommand(0, "LOTSTART", "LOTSTART");
            str.sprintf("OPERATORID,%s,", fLotInfo->edtSysOperatorID->Text);
            fTesterTCP->SendTCPIPCommand(0, "OPERATORID", str);
            fTesterTCP->SendTCPIPCommand(0, "Get OS Setup", "GETOSSETUP");      //Steven 20230505 : 取得OS Tester資訊
            WritePTILotStartTrace(this, "AfterTesterTCPSend", "Commands queued to TesterTCP");
        }
    }

    if((CUSTOMER_CODE==CC_KYEC_LEE ||
        CUSTOMER_CODE==CC_KYEC_XILINX) &&                                       //jou 2015-10-02 Auto Retest GPIB mode
       IniConfig.bA10_AutoReTest)                                               //wei 20151230 art 紀錄資料
    {
        SaveLotOperatorID(fLotInfo->edtSysLotID->Text.c_str(), fLotInfo->edtSysOperatorID->Text.c_str(), false);
    }

    if(CosFunction.bUseLogUploadToFTPFunction==true)
    {
        bSysLotStart      =true;                                                //Ifor 20160302 add for KYEC_HS LotStart
        bEPLogStart_KYEC  =true;                                                //Ifor 20160302 add for KYEC_HS EPLogStart
        bTempLogStart_KYEC=true;                                                //Ifor 20160302 add for KYEC_HS TempLogStart
        if(USE_NOVX3360==true)
        {
            bESDLogStart_KYEC=true;                                             //Ifor 20160302 add for KYEC_HS ESD
        }
        asATCEvenLotID=fLotInfo->edtSysLotID->Text;                             //Ifor 20170124 (Steven) :add LotID By ATC Even Log
        bArmTestInfoEvenLogStart_KYEC=true;                                     //Ifor 20190912 :add 海思 V02.30 版 Record Torque
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {                                                                           //Sam 20170719 (Steven) 移植超豐 OEE 功能 form HT-7045
        lb_PIOEELotStatus->Caption="Production Start Lot.....";

        fSortCT->btnClearCountClick(fSortCT);
        if(IniConfig.bN14_4_OEEAutoLoadMOFile==false)                           //Mylin 20170524 Add Auto Load MO By Option
        {
            fProductionInfo->OEE_SetMO(fLotInfo->ed_PIOEEMO->Text);
        }
        fProductionInfo->OEE_SetHandlerID(IniConfig.SocketHandlerID);
        lb_PIOEELotStatus->Caption="Production Start Lot Success!";
        ASET_StartTimeNAME=Now().FormatString("yyyymmdd_hhnn");
        if(MOT[MMTrayY].Tray.HasIC()==false)
        {
            ASE_InTrayNum=0;
            iLoadTrayCount=0;
        }

        ClearAllTrayCount();                                                    //Steven 20251029 : outputtray 數量
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                LastSet.iSocketContactCount[i][j]=0;
            }
        }

        fObserver->iTestReceiveTimeCount=0;                                     //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        fObserver->sTestReceiveTime="";
        fObserver->bTestIndexZ=false;
        fObserver->iTestIndexZCount=0;
        fObserver->sTestIndexZTime="";
        fObserver->dOEEIndexCycleTime=0;                                        //Sam 20180802 (wei) : OEE 32Site 修正
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        ZeroMemory(iSLT_HeadContactCount, sizeof(iSLT_HeadContactCount));       //Ifor 20191218 : add KYEC 要求 同SLT輸出表格
        asSLT_LotStartTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");
    }

    if(CosFunction.bBarcodeTrayRecFile==true)                                   //jou 20190930 : Barcode Tray record file
    {
        fBarCode->InitBarcodeRecFile();
    }

    if(CUSTOMER_CODE==CC_SCC ||                                                 //Steven 20200306 : SCC要求按下Lot End的時候, 資料要清空
       CUSTOMER_CODE==CC_Murata ||
       CUSTOMER_CODE==CC_SJ_Semiconductor ||
       CUSTOMER_CODE==CC_CYUEAN)                                                //Steven 20240122 : add for CyuEan
    {
        fMain->cbRunStartMode->Enabled=false;
        fMain->palFT->Enabled=false;
        fMain->palRT->Enabled=false;
        fMain->palOffLine->Enabled=false;
        fMain->palEQC->Enabled=false;
    }

    if(CosFunction.bSpecailLowYeild)                                            //Sam 20211221 : Lot Start 才需要重新第一階段檢查
        fCounterClear->LowYieldSpecialInitail();

    WriteIniData(sPath, "Lot Info", "Job Sequence", edtJobSeq->Text);           //JerryYang 20220923 : add for SLT lot summary

    if(CosFunction.bSortingBy2DList==true &&
       LastSet.iTester==_2D_SORT &&
       TestIF_File.bSortingBy2DIDList==true)                                    //Frank 20221122 : 2DID sorting for ATK
    {
        fSCKART->sLotStartTime=Now().FormatString("yyyymmddhhnnss");
        fSCKART->AccessFile(false, 1);
    }

    if(IniConfig.bA38_SLT_Summary)                                              //JerryYang 20220923 : add for SLT lot summary
    {
        bool bChangeLotID=false;
        AnsiString strLotID=edtSysLotID->Text;
        if(HasICUnderMachine()==false && HasAnyICInMachine()==false)
        {
            bChangeLotID=true;
        }
        else
        {
            if(strLotID==fSCKART->sLotID && (strLotID!="" && strLotID!=" "))
            {
                bChangeLotID=false;
            }
            else
            {
                bChangeLotID=true;
            }
        }

        fObserver->memoLotSummary->Clear();
        if(bChangeLotID==true)
        {
            fSCKART->iCurrent93KARTStep=1;
            fSCKART->iCurrentFlexARTStep=4;
            fMain->Clarn_Data(1, "ART_INPUTQTY");
            fSCKART->ClearLotInfo();
            RecordProcess("ART INPUTQTY.");
            fLotInfo->btClearBarcodeList->Click();                              //Steven 20190214 : 統一清除2DID方式

            fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_W);
    //        AnsiString Str = "1,2,3,4,5,6,7,8,9";
            TStringList *ss=new TStringList;
            str=StringReplace(strLotID, "-", ",", TReplaceFlags()<<rfReplaceAll);

            ss->CommaText=str;
            for(int i=0; i<ss->Count; i++)
            {
                 ss->Strings[i];                                                //??就得到被分割的字符串
            }

            fSCKART->sInfo_TestBinNo="";
            AnsiString sTestBin="";
            if(ss->Strings[2]=="RC")
            {
                for(int i=0; i<iTestBinCount; i++)                              //從Bin1開始, 比照Epson所定義的格式
                {
                    int iBinTray=Prod.iT6CatData[i];
                    if(Prod.bCateRTo6Tray[iBinTray]==true)
                    {
                        sTestBin.sprintf("%02d", i);
                        if(fSCKART->sInfo_TestBinNo=="")
                        {
                            fSCKART->sInfo_TestBinNo=sTestBin;
                        }
                        else
                        {
                            fSCKART->sInfo_TestBinNo=fSCKART->sInfo_TestBinNo+"-"+sTestBin;
                        }
                    }
                }
            }
            else
            {
                fSCKART->sInfo_TestBinNo="All";
            }

            fSCKART->sLotStartTime=Now().FormatString("yyyymmddhhnnss");
            fSCKART->sInfo_Customer="";
            fSCKART->sLotID.sprintf("%s", ss->Strings[0]);                      //Lot ID
            fSCKART->sInfo_CustLotID="";
            fSCKART->sInfo_CustDevGup="";
            fSCKART->sInfo_DeviceName="";
            fSCKART->sInfo_Stage=ss->Strings[1];                                //Stage
            fSCKART->sInfo_Step=ss->Strings[2];                                 //Step
            fSCKART->sInfo_ReportCnt=edtJobSeq->Text;                           //Job Sequence
            fSCKART->sInfo_ProgramName="";
            fSCKART->sInfo_TesterID="";
            fSCKART->sInfo_HandlerID=IniConfig.SocketHandlerID;
            fSCKART->sInfo_Temperauture="";
            fSCKART->sInfo_CurrQty="";
            fSCKART->sInfo_OperatorID=edtSysOperatorID->Text;
            fSCKART->AccessFile(false, 1);
            str.sprintf("Lot start: %s", fSCKART->sLotID);
            RecordProcess(str);                                                 //Steven 20190722 : add TSV log

            if(fSCKART->iTesterType==1 || IniConfig.bA37LotStartLotEnd)         // && CosFunction.bAutoRetestGPIBmode==true) //Steven 20170309 (wei) : Fixed for ART
            {
                fSCKART->iNeedRT=0;
                if(LastSet.iRunStartMode==rsmInitial_ART || LastSet.iRunStartMode==rsmContinuStart_ART ||
                   LastSet.iRunStartMode==rsmContinuRetest_ART || LastSet.iRunStartMode==rsmAutoRetest)
                {
                    fMain->SetLotState(2);                                      //SECS ART FT Start
                }
                else
                {
                    if(fSCKART->sInfo_Step==FT)
                        fMain->SetLotState(2);                                  //SECS ART FT Start
                    else
                        fMain->SetLotState(4);                                  //SECS ART FT Start
                }
                LastSet.bEndLotAutoRetestGPIB=false;
                LastSet.bWaitStartLotAutoRetestGPIB=false;
                LastSet.bFirstTestAutoRetestGPIB=true;
            }
            delete ss;
        }
    }

    if(CosFunction.bIndexCheckCanTurnOff &&
       IniConfig.iD71IndexCheckOnOffMode==0)                                    //Isaac 20211019 : 可選擇做index check的時機，lot start後，要做index check
    {
        bLotStartEndNeedIndexCheck=true;
    }
    else
    {
        bLotStartEndNeedIndexCheck=false;
    }

    if(IniConfig.bUseAutoSiteMapping==true &&                                   //Jimmychiu 20230707 : Auto Site Mapping Trigger Function
       IniConfig.bI21EnableASM==true &&
       IniConfig.bI50_EnableAutoSiteMappingTrigger==true &&
       IniConfig.bI50_StartLot==true)
    {
        RecordProcess("Trigger Auto Site Map after Start Lot [I50]");
        SetRunStartMode(rsmAutoSiteMap);
    }

    if(IniConfig.bVTESTFunction==true)                                          //RogerYang 20250604 偉測不可複測bin功能
    {
        CheckNoRetestBinFlag();
    }

    LastSet.iAutoTempOfsTriggerCnt=0;                                           //Sam 20220406 : 溫度自動補償功能 By FTP
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::SetLotEnd(AnsiString sFunc)                          //Steven 20250515 : 整合Lot測試報表 Lot End
{
    AnsiString sPath=AuthPath, str, Msg;
    TStringList *SL=new TStringList();
    sPath=AuthPath+"config.ini";
    dtEndLot=Now();
    str.sprintf("%04d%02d%02d_%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    lbledtEndTime->Text=str;
    SL->Add(lbledtStarTime      ->Text);
    SL->Add(lbledtEndTime       ->Text);
    SL->Add(lbledtTesterOsVer   ->Text);
    SL->Add(lbledtTesterID      ->Text);
    SL->Add(edtSysOperatorID    ->Text);
    SL->Add(lbledtCustomer      ->Text);
    SL->Add(lbledtTestProg      ->Text);
    SL->Add(lbledtDeviceName    ->Text);
    SL->Add(edtSysLotID         ->Text);
    SL->Add(lbledtSubLotNo      ->Text);
    SL->Add(lbledtModeCode      ->Text);
    SL->Add(lbledtTestCode      ->Text);
    SL->Add(lbledtTestBinNo     ->Text);
    SL->Add(lbledtMachineID     ->Text);
    SL->Add(edtStage            ->Text);
    SL->Add(edtStep             ->Text);
    fMain->slLotInfolog->AddText(SL->CommaText);
    fMain->slLotInfolog->MySaveToFile();
    RunInfo.LotEndTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    Msg.sprintf("Lot End, Lot ID:%s, OP ID:%s, Run Mode:%s", edtSysLotID->Text, edtSysOperatorID->Text, cbRunMode->Text);                                       //Steven 20200302 : Lot Start/end增加log
    RecordProcess(Msg, sFunc);

    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        asSLT_LotEndTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");
        FormHS->RecordContact_SLTLog_HS();
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        fMain->patFunc->SetEndLotTime(dtEndLot);
        UpdateLotInfoPAT();
        fMain->patFunc->GenerateEndLotReport();
        fMain->patFunc->JamLogs.clear();
        fStartCondition->sbClearCountClick(fStartCondition);
        fMain->machineTime.EndLot();                                            //Jimmychiu 20250916 : 新增機台運作狀態紀錄
    }

    if(IniConfig.bEnable_SECS_GEM==true &&
        RunInfo.bLotStart==true)                                                //Steven 20140528 : Secs Gem  //wei 20150821 不能連續送
    {
        if(fAGV->bATK_AMR_DoLotEndSent==false)                                  //AI(general) 20260402 (RogerYang) : ATK AMR 已提前送過則跳過
            EventReport(SECS_EVENT.DoLotEnd);
        fAGV->bATK_AMR_DoLotEndSent=false;                                      //AI(general) 20260402 (RogerYang) : 不論有無送出都重設旗標
        fAGV->bATK_AMR_DoHostLotStart=false;                                    //put here
        fAGV->bATKAMR_GET_LOTORDER0_Ready=false;
    }

    if(TestIF.iTestType==TCP_IP_MODE)                                           //Steven 20230213 : For SJSemi OS Tester
    {
        fTesterTCP->SendTCPIPCommand(0, "LOTEND", "LOTEND");
    }

    if(CUSTOMER_CODE==CC_PTI &&                                                 //Sam 20170502 (wei) 力成按下 "Lot End" 後即 JamAlarmLogTxt LotId 資料夾所有資料上傳至 FTP
       IniConfig.bEnableFTP==true &&
       RunInfo.bLotStart==true)
    {
        TStringList *slInputQuantity;                                           //Sam 20171208 (wei) : PTI Lot End 上報 Input 數量
        slInputQuantity=new TStringList();
        slInputQuantity->Add(IntToStr(LastSet.SendCT[0]));
        slInputQuantity->Add(fLotInfo->edtSysLotID->Text);
        slInputQuantity->Add(fLotInfo->edtSysOperatorID->Text);
        slInputQuantity->Add(fMain->cbSetupFileName->Text);
        slInputQuantity->Add(fLotInfo->cbRunMode->Text);
        fMain->slInputDataLog->AddTextWithDateTime(slInputQuantity->CommaText);
        fMain->slInputDataLog->MySaveFileByFileNameAndType(IniConfig.asN06_FileName, fLotInfo->edtSysLotID->Text, "INPUT");
        slInputQuantity->Clear();
        delete slInputQuantity;
        fSortCT->btnClearCountClick(fSortCT);

        AnsiString asJamCodeFilePath, asJamCodeFileName;                        //RogerYang 20170508 (wei) 拿掉static
        asJamCodeFilePath.sprintf("D:\\HT9045_Log\\JamAlarmLogTxt\\%s", edtSysLotID->Text);
        TStringList *lstFiles = new TStringList;
        TSearchRec sr;
        if(FindFirst(asJamCodeFilePath + "\\*.*", faAnyFile, sr)==0)
        {
            do                                                                  //尋找資料夾內所有檔案
            {
                lstFiles->Add(sr.Name);
            }while(FindNext(sr)==0);
            FindClose(sr);

            for(int i=0; i<lstFiles->Count; i++)
            {
                 if(lstFiles->Strings[i]!="." && lstFiles->Strings[i]!="..")
                 {
                    MySleep(1000);                                              //Sam 20240308 : 上傳延遲一下
                    asJamCodeFileName.sprintf("%s\\%s", asJamCodeFilePath, lstFiles->Strings[i]);
                    fFTPClient->UploadFileToServer2(IniConfig.FtpUplaodPath, asJamCodeFileName);
                 }
            }
        }
        lstFiles->Clear();                                                      //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete lstFiles;
    }

    AnsiString asSocketIdProductDataPath, asSocketIdProductDataFileName;
    if(CUSTOMER_CODE==CC_PTI && RunInfo.bLotStart==true)                        //Sam 20170516 (wei) 自定義 SocketID Count 資料 csv 檔保存至 D:\\HT9045_Log\\SocketIdProductData\\yyyymmddhhnnss_LotID.csv
    {
        TStringList *SL;
        SL=new TStringList();
        asSocketIdProductDataPath.sprintf("D:\\HT9045_Log\\SocketIdProductData");
        asSocketIdProductDataFileName.sprintf("%s_%s.csv", Now().FormatString("yyyymmddhhnnss"), edtSysLotID->Text);
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                SL->Clear();
                SL->Add(edtSysLotID->Text);
                SL->Add(LastSet.strSocketID[j][i]);
                SL->Add(AnsiString(LastSet.iSocketContactCount[j][i]));
            }
            fMain->slSocketIdProductData->AddText(SL->CommaText);
            fMain->slSocketIdProductData->MySaveFileByFileName(asSocketIdProductDataPath, asSocketIdProductDataFileName);
        }
        SL->Clear();                                                            //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete SL;

        if(IniConfig.bN12_EnableSocketIdProductDataFTP==true)
        {
            MySleep(1000);                                                      //Sam 20240308 : 上傳延遲一下
            fFTPClient->UpSocketIdPoductDataToServerByFTP(asSocketIdProductDataPath, asSocketIdProductDataFileName);
        }
    }

    if(IniConfig.bB03_TesterReport)                                             //Sam 20231115 : PTI 新增 Tester report
    {
        if(bCanRunSCKART==false)                                                //Sam 20240809 : PTI ART 模式
            ProductTesterReport();
        else
            SetRunStartMode(rsmInitial_ART);
    }

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)
        sbSECSLotStart->Down=true;

    //==> Eastsun 20260527 整合#BCIR.P14 SaveInspReportEnd :KYEC
    fBarCode->SaveInspReportEnd(asATCEvenLotID, GetLastOpenFN());                              //Sam 20240426 : Add BarCoder Inspection Report
    //<== Eastsun 20260527 #BCIR.P14

    if(CosFunction.bUseLogUploadToFTPFunction==true)
    {
        bSysLotStart=false;                                                     //Ifor 20160302 add for KYEC_HS bSysLotStart
        bEPLogEnd_KYEC=true;                                                    //Ifor 20160302 add for KYEC_HS EPLogEnd
        bTempLogEnd_KYEC=true;                                                  //Ifor 20160302 add for KYEC_HS TempLogEnd
        FormHS->RecordLog_HS(true);

        if(USE_NOVX3360==true)
            bESDLogEnd_KYEC=true;                                               //Ifor 20160302 KYEC FTP UP Load ESD Log End

        if(CUSTOMER_CODE==CC_KYEC_LEE &&
           (ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem) &&
           Temperature.bATCActiveCooling==true)
        {
            bATCEvenLogEnd_KYEC=true;
            asATCEvenLotID="";
        }
        bArmTestInfoEvenLogEnd_KYEC=true;                                       //Ifor 20190912 :add 海思 V02.30 版 Record Torque
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {                                                                           //JimmyChiu 20220118 相似功能整合
        #ifndef SOFT_SIMULTE                                                    //JimmyChiu 20220119 : ADD
        fProductionInfo->DoIPSCProcess();
        #endif
        RecordProcess("End Lot Press");
        lb_PIOEELotStatus->Caption="Production End Lot.....";
        fProductionInfo->UploadBinQtyReport(true);
        #ifndef SOFT_SIMULTE                                                    //JimmyChiu 20220119 : ADD
        fProductionInfo->SocketLifeTimeUpload();
        #endif
        fProductionInfo->UploadTrayMappingLog();                                //Sam 20201209 : 增加資料上傳
        fProductionInfo->OEE_EndLot();
        fSortCT->btnClearCountClick(fSortCT);
        lb_PIOEELotStatus->Caption="Production End Lot Success!";
        ed_PIOEEMO->Text="";
        fObserver->sTestReceiveTime="";                                         //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        fObserver->sTestIndexZTime="";
        fObserver->dOEEIndexCycleTime=0;                                        //Sam 20180802 (wei) : OEE 32Site 修正
    }

    if(IniConfig.bN09_LotCountAutoFunc==false)                                  //JerryYang 20190928 SPIL lot count
    {
        fSCKART->SaveTestSummary(1);
    }

    if(IniConfig.bVTESTFunction==true)
    {
        cbProcess->Enabled=true;
        cbTestTimes->Enabled=true;                                              //RogerYang 20250809 偉測Summary文件修改
        iProduceTimeCT=0;                                                       //jou 20221125 : 機台添加三小時送檢報警，從lot start時間開始計算

        if(CUSTOMER_CODE==CC_VTEST_Shanghai && IniConfig.bCheckFile)            //jou 20210823 : lot start增加保護避免重複執行run mode
        {
            if(RunInfo.bLotStart==true &&
               fMesSystem->bDownloadLotInforFlag==true)
            {
                fMesSystem->RunModeRW(false, edtSysLotID->Text, cbRunMode->Text);

//                #ifdef BETA_VTestSummaryFile                                    //RogerYang 20250809 偉測Summary文件修改
                    fMesSystem->VTestSummaryReport();
//                #else
//                if(IniConfig.bN10_UploadSummaryToFTP==true)
//                {
//                    fMesSystem->VTestSummaryReport();
//                    fMesSystem->VTestUPHReport();                               //jou 20220525 : lot end 時輸出 UPH report
//                }
//                #endif
            }
        }
        else if(CUSTOMER_CODE==CC_VTEST)
        {
//            #ifdef BETA_VTestSummaryFile                                        //RogerYang 20250809 偉測Summary文件修改
                fMesSystem->VTestSummaryReport();
//            #else
//                if(IniConfig.bN10_UploadSummaryToFTP==true)
//                {
//                    fMesSystem->VTestSummaryReport();
//                }
//            #endif

            edtProcessName->Enabled=true;
            edtProcessName->Text="";
            edtProduct->Enabled=true;
            edtProduct->Text="";
        }

        if(fMesSystem->CheckVTENGmode(edtSysLotID->Text)==true)
        {
//            SetLotID("");
//            edtSysOperatorID->Text="";
        }

        fMesSystem->ClearMesData();
    }

    if(CUSTOMER_CODE==CC_UTAC)                                                  //Richard 20220929 :Add for UTAC Lot Start/End 0xC0
    {
        if(LastSet.iRunStartMode==rsmContinuStart)
        {
            fMain->SetLotState(8);                                              //UTAC ART FT Lot End
        }
        else if(LastSet.iRunStartMode==rsmContinuRetest)
        {
            fMain->SetLotState(10);                                             //UTAC ART Final Lot End
        }
    }

    SaveGroundESDByLot(RunInfo.LotNo, edCustomerLotId->Text, coStation->Text, edStationNum->Text);                      //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。

    if(CUSTOMER_CODE==CC_CYUEAN)
    {
        bFTPDownloadSetupFile=false;
        if(IniConfig.bN34_GenerateOEEAlarmRpt)                                  //Jimmychiu 20250324 : CC_CYUEAN OEE report
        {
            SendCommand_EventLog(EL_UPLOAD_CHIPADV_LOTEND, "1");
        }
    }

    if(CUSTOMER_CODE==CC_LEADYO && IniConfig.bN33_UpLoadOCRBinLogByNet)         //KenHsieh 20230502 : 利揚要求上傳OCR + BIN Log上傳至Host
        fOCR->UpLoadOCRAndBinLog();

    if(fBarCode->JCETUseMakeWhite2DIDList()==true)                              //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        fBarCode->UpdateWhite2DIDList(fLotInfo->edtSysLotID->Text, fLotInfo->cbRunMode->Text);
    }

    SetLotComponents(true);                                                     //Steven 20250515 : 整合Lot測試報表
    SetLotID("");
    lbledtStarTime      ->Text="";
    lbledtEndTime       ->Text="";
    lbledtTesterOsVer   ->Text="";
    lbledtTesterID      ->Text="";
    edtSysOperatorID    ->Text="";
    lbledtCustomer      ->Text="";
    lbledtTestProg      ->Text="";
    lbledtDeviceName    ->Text="";
    edtSysLotID         ->Text="";
    lbledtSubLotNo      ->Text="";
    lbledtModeCode      ->Text="";
    lbledtTestCode      ->Text="";
    lbledtTestBinNo     ->Text="";
    cbRunMode           ->Text="";
    edCustomerLotId     ->Text="";
    coStation           ->Text="";
    edStationNum        ->Text="";
    edtCusLotID         ->Text="";
    edtCusDevGrp        ->Text="";
    edtBarcodeRecipe    ->Text="";   //==> Eastsun 20260527 整合#027-2.MR.U3 clear edtBarcodeRecipe :KYEC
    edPage              ->Text="";
    edtStage            ->Text="";
    edtStep             ->Text="";
    slEventLog->SetLotData(RunInfo.LotNo, lbledtStarTime->Text);                //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID

    for(int i=0; i<3; i++)                                                      //Steven 20250603 : By Lot Summary
    {
        ArmDataLot[i]->ClearALLCT();
    }

    ReadWriteLotInfo(false);
    SL->Clear();
    delete SL;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::SetLotComponents(bool bLotEnd)                       //Steven 20250515 : 整合Lot測試報表
{
    RunInfo.bLotStart            =!bLotEnd;                                     //RogerYang 20250312(補code:jou 20230223 : 換位置，VTEST需判斷完才能false)
    edtSysLotID         ->Enabled=bLotEnd;
    sbSECSLotStart      ->Down   =!bLotEnd;
    sbSECSLotEnd        ->Down   =bLotEnd;
    pnlLoader           ->Caption="";
    edPage              ->Enabled=bLotEnd;                                      //JerryYang 20190928 SPIL lot count
    edtSysOperatorID    ->Enabled=bLotEnd;                                      //wei 20150326 Lot Start 反灰
    edCustomerLotId     ->Enabled=bLotEnd;                                      //Sam 20220223 : 矽格中興廠新增 Lot 資料
    coStation           ->Enabled=bLotEnd;                                      //Sam 20220223 : 矽格中興廠新增 Lot 資料
    edStationNum        ->Enabled=bLotEnd;                                      //Sam 20220223 : 矽格中興廠新增 Lot 資料
    edtJobSeq           ->Enabled=bLotEnd;                                      //JerryYang 20220923 : add
    edtBarcodeRecipe     ->Enabled=bLotEnd;                                      //Ifor 20241108 add:Barcode Multi Recipe   //==> Eastsun 20260527 整合#027-2.MR.U2 edtBarcodeRecipe enable/disable :KYEC

    if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport==false)             //Sam 20240809 : PTI ART 模式
        cbRunMode       ->Enabled=false;                                        //wei 20150326 Lot Start 反灰
    else
        cbRunMode       ->Enabled=bLotEnd;

    edtCusLotID         ->Enabled=bLotEnd;                                      //JerryYang 20230322 : 2D sort lot info UI修改
    edtCusDevGrp        ->Enabled=bLotEnd;
    edtDevice           ->Enabled=bLotEnd;

    lbledtStarTime      ->Enabled=bLotEnd;
    lbledtEndTime       ->Enabled=bLotEnd;
    lbledtTesterOsVer   ->Enabled=bLotEnd;
    lbledtTesterID      ->Enabled=bLotEnd;
    lbledtCustomer      ->Enabled=bLotEnd;
    lbledtTestProg      ->Enabled=bLotEnd;
    lbledtDeviceName    ->Enabled=bLotEnd;
    lbledtSubLotNo      ->Enabled=bLotEnd;
    lbledtModeCode      ->Enabled=bLotEnd;
    lbledtTestCode      ->Enabled=bLotEnd;
    lbledtTestBinNo     ->Enabled=bLotEnd;
    edtStage            ->Enabled=bLotEnd;
    edtStep             ->Enabled=bLotEnd;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btUploadClick(TObject *Sender)                       //Steven 20101108 : For Amkor
{
    int ret=ShowMyMessageBox_YES_NO("Are you sure to upload the file?", "確定上傳檔案嗎？");
    if(ret==2)
    {
        return;
    }
    DoUpload(fMain->cbSetupFileName->Text,edDeviceName->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::_DelTree(String dir, String sExcpet)                 //ChungHung 20150413 add for TSMC
{
    String d=dir;
    String FileName;
    TSearchRec SearchRec;

    if(!DirectoryExists(dir))
        return;                                                                 // 要 #include "FileCtrl.hpp" 才有這個函式

    if(d.SubString(d.Length(),1)!="\\") d=d+"\\";

    if(FindFirst(d+"*.*", faAnyFile, SearchRec) == 0)
    {
        do
        {
            if(SearchRec.Attr & faDirectory)                                    //ChungHung 20150420 modify
            {
                if((SearchRec.Name!=".") && (SearchRec.Name!="..") && (SearchRec.Name!=sExcpet))
                {
                    _DelTree(d+SearchRec.Name);                                 //遞迴呼叫
                    RemoveDir(d+SearchRec.Name);
                }
            }
            else
            {
                FileSetAttr(d+SearchRec.Name,faArchive);
                DeleteFile(d+SearchRec.Name);
            }
        } while (FindNext(SearchRec) == 0);
        FindClose(SearchRec);
    }
    RemoveDir(d);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btDownloadClick(TObject *Sender)                     //Steven 20101108
{
    if(SystemStart || HasICUnderMachine())                                      //Steven 20110503
    {
        return;
    }

    if(IniConfig.bVTESTFunction==true && HasAutoICInMachine())                  //jou 20211129 : 修正 VTEST Tray end 結束後重開程式會重置 Auto Tray盤造成疊料。
    {
        LastSet.bHasDownloadFile=true;
        return;
    }

    int iPos=0, iCode, ret=0;
    bool bFlag=false;
    AnsiString asStringC="", asStringE="", asBuffer="", sDLFileName="";
    TStringList *slErrorCode;
    AnsiString asSourcePath="", str1="", str2="", sDeviceName=edDeviceName->Text;

    if(CosFunction.bScanBarcodeAndDownloadFileInRMS==true)                      //Jimmychiu 20241226 : Scan Barcode And Download File In RMS
    {
        edDeviceName->Text="";
    }

    if(IniConfig.bEnableRms)
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        {
            bFlag=false;
            asBuffer.sprintf("%sSPIL.txt", DataPath);
            if(FileExists(asBuffer))
                DeleteFile(asBuffer);

            if(FileExists(asBuffer))
                bFlag=true;

            FileListBox1->Update();
            for(int i=0; i<FileListBox1->Items->Count; i++)
            {
                if(FileExists(FileListBox1->Items->Strings[i]))
                    DeleteFile(FileListBox1->Items->Strings[i]);

                if(FileExists(FileListBox1->Items->Strings[i]))
                    bFlag=true;
            }

            if(bFlag==true)
            {
                ShowMyMessage("Recipe folder data delete fail", "1030,所有於Recipe folder之檔案刪除失敗-Package recipe & .TXT");
                return;
            }

            asBuffer="D:\\HT9045\\IniData\\SPIL\\HandlerAutoDownload.bat";
            if(FileExists(asBuffer))
            {
                system(asBuffer.c_str());
            }
            else
            {
                ShowMyMessage("default error (程式啟動錯誤,無法啟動java檔案)","1001,default error, need to inform IT, please!!");
                return;
            }

            asBuffer.sprintf("%sSPIL.txt", DataPath);
            if(FileExists(asBuffer)==false)
            {
                ShowMyMessage("Error code.TXT file missing (Error code.TXT檔案遺失)","1035,Error code.TXT file missing, call IT to check, please!!");
                return;
            }
            else
            {
                bFlag=false;
                slErrorCode=new TStringList;
                slErrorCode->LoadFromFile(asBuffer);
                iPos=slErrorCode->Strings[0].Length();
                iCode=atoi(slErrorCode->Strings[0].SubString(1, 1).c_str());
                FileListBox1->Update();
                if(iPos==1 && iCode==0 && FileListBox1->Items->Count==1)
                {
                    sDLFileName=FileListBox1->Items->Strings[0].SubString(1, FileListBox1->Items->Strings[0].Length()-4);
                }
                else if(iPos>=4)
                {
                    if(iCode==1)
                    {
                        iCode=atoi(slErrorCode->Strings[0].SubString(2, 3).c_str());
                        if(iCode>0 && iCode<100)
                        {
                            if(iCode==1)
                            {
                                ShowMyMessage("default error (程式啟動錯誤,無法啟動java檔案)","1001,default error, need to inform IT, please!!");
                            }
                            else if(iCode==3)
                            {
                                ShowMyMessage("LOTID-Stage format error (Lot ID 格式錯誤)","1003,lotid-stage error, check typo and correct first!!");
                            }
                            else if(iCode==54)
                            {
                                ShowMyMessage("FTP download error (Recipe自FTP server下載recipe失敗)","1054,FTP download error, need to call IT dispose, please!!");
                                return;
                            }
                            else if(iCode==71)
                            {
                                ShowMyMessage("1071,temp & soak database connect error (temp & soak database 連線失敗)","temp & soak database connect error, need to check the connect first.");
                            }
                            else if(iCode==72)
                            {
                                ShowMyMessage("1072,L/B database connect error (L/B database 連線失敗)","L/B database connect error, need to check the connect first.");
                            }
                            else if(iCode==73)
                            {
                                ShowMyMessage("1073,temp & soak database select error (temp & soak database 搜尋失敗)","temp & soak database select error, need to confirm with PD whether it be build up, please!!(建檔)");
                            }
                            else if(iCode==74)
                            {
                                ShowMyMessage("1074,L/B database select error (L/B database 搜尋失敗)","L/B database select error, need to confirm with PMR whether it be build up, please!!(建檔)");
                            }
                            else
                            {
                                ShowMyMessage("Error Code : "+slErrorCode->Strings[0], "");
                            }
                        }
                        else
                        {
                            bFlag=true;
                        }
                    }
                    else
                    {
                        bFlag=true;
                    }
                }
                else
                {
                    bFlag=true;
                }
                slErrorCode->Clear();                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                delete slErrorCode;
                if(bFlag)
                {
                    ShowMyMessage("Error code.TXT format error (error code.TXT格式錯誤)", "1036,Error code.TXT format error, call IT to check, please!!");
                    return;
                }

                if(FileListBox1->Items->Count!=1)
                {
                    ShowMyMessage("Recipe write error (Recipe於local HD寫入失敗)", "1027,Recipe write error, call IT to check, please!!");
                    return;
                }
            }

            asBuffer.sprintf("%sSPIL.txt", DataPath);
            if(FileExists(asBuffer))
                DeleteFile(asBuffer);
            asBuffer=fMain->cbSetupFileName->Text;
        }
        else
        {
            ReadRmsPath();
            LastSet.bHasDownloadFile=false;
            if(CUSTOMER_CODE==CC_AMKOR_Korea)                                   //Steven 20150923
            {
                if(sDeviceName=="")
                {
                    ShowMyMessage("Device Name can not Empty!");
                    return;
                }
            }
            else
            {
                if(sDeviceName=="")
                {
                    ShowMyMessage("Device Name & Temperature can't Empty!", "Device Name與溫度不可以空白");
                    return;
                }

                CutTempToEdit(sDeviceName);

                if(IniConfig.bCheckFile && edTemp->Text=="")
                {
                    ShowMyMessage("Temperature can't Empty!", "溫度不可以空白");
                    return;
                }
            }

            if(IniConfig.iN05_UpDLMethod==eByFTP)
            {
                FileInfo().EnsureDirectoriesExist(asProductionLogPath);         //Jimmychiu 20250430 : 自動建立路徑資料夾並產生Barcode.ini檔案
                asSourcePath=asProductionLogPath+"\\Barcode.ini";
            }
            else
            {
                asSourcePath=IniConfig.sRmsPath+"\\Barcode.ini";
            }

            if(FileExists(asSourcePath.c_str()))
            {
                if(CheckIniDataMem(asSourcePath, "Barcode", sDeviceName))       //RogerYang 20260214 : 整個檔案載入記憶體，不受 32KB Section 限制
                {
                    sDLFileName=ReadIniDataMem(asSourcePath, "Barcode", sDeviceName, AnsiString(""));
                }
                else
                {
                    str1.sprintf("The %s Device Name not exsit!!", sDeviceName);
                    str2.sprintf("%s的Device Name不存在!!", sDeviceName);
                    ShowMyMessage(str1, str2);
                    return;
                }
            }
            else
            {
                ShowErrorMessage("WAR1681", 0, MMSystem, 0, asSourcePath);      //File Read error
                return;
            }
        }

        if(DownloadFromServer(sDLFileName, false))
        {
            //jou 2012-03-01 [D31] RTC Change Recipe Need reCreate RTC Model
            //jou 2012-03-28 如果download的工作檔不一樣的話,就需要重建model
//            if(IniConfig.bD31RTCChangeRecipeNeedreCreateModel==false)         //Steven 20190109 : 只要下載的工作檔相同就不刪除RTC ROI
            {
                if(fMain->cbSetupFileName->Text!=sDLFileName)
                {
                    fSetup->bFirstTime=false;
                }
                else
                {
                    fSetup->bFirstTime=true;
                }
            }
            #ifdef DEBUG
            RecordProcess("btDownloadClick 1");
            #endif
            if(CosFunction.bRecipeCheck)                                        //JerryYang 20151028 add for Amkor 如果download的工作檔不一樣,要求做Height Calibration後才能start
            {
                if(fMain->cbSetupFileName->Text!=sDLFileName)
                {
                    fMain->cbSetupFileName->Text=sDLFileName;                   //JerryYang 20151112 先載入Recipe才跳出提醒頁面
                    fMain->cbSetupFileNameChange(fMain);                        //JerryYang 20151112 先載入Recipe才跳出提醒頁面
                    ret=ShowMyMessageBox_YES_SKIP("Handler change another recipe, please do height calibration first","已載入新工單,請先調整測試高度");                       //JerryYang 20151112 加入中文說明
                    if (ret==1)                                                 //點YES跳出Contact頁面
                    {
                        if(DoPassword()==false)                                 //JerryYang 20151127 點YES要Show登入權限畫面
                        {
                            bHeightCalibrationFinish=false;                     //JerryYang 20151201
                        }
                        else
                        {
                            bHeightCalibrationFinish=false;                     //JerryYang 20151201
                            fContact->ShowModal();
                        }
                    }
                    else if (ret==2)                                            //權限夠才能SKIP
                    {
                        if(DoPassword()==false)
                        {
                            bHeightCalibrationFinish=false;
                        }
                        else
                        {
                            bHeightCalibrationFinish=true;
                        }
                    }
                }
                else
                {
                    fMain->cbSetupFileName->Text=sDLFileName;                   //JerryYang 20151228
                    fMain->cbSetupFileNameChange(fMain);                        //JerryYang 20151228
                }
            }
            else                                                                //JerryYang 20151112 不檢查download的工作當與當前是否相同,直接載入Recipe
            {
                fMain->cbSetupFileName->Text=sDLFileName;
                fMain->cbSetupFileNameChange(fMain);
            }
            #ifdef DEBUG
            RecordProcess("btDownloadClick 2");
            #endif
            fSetup->bFirstTime=false;
            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
            {
                if(asBuffer!=sDLFileName)
                    _DelTree(DataPath+asBuffer);
                fMain->LookForFile();
                fMain->cbSetupFileName->Text=GetLastOpenFN();
            }

            if(CosFunction.bRTCAutoModelVerify==true &&                         //jou 2014-06-24 RTC 自動進行Model驗證
               IniConfig.bD36EnableRTCAutoModelVerify==true &&
               bNeedWaitRTCAutoVerify==false)
            {
                bRTCAutoModelVerifyFirstTime=true;
            }
            fMain->ModifyTester(ON_LINE);                                       //Steven 20191218 : 整合修改LastSet.iTester

            if(IniConfig.bVTESTFunction==true)                                  //jou 20210823 : 依照run mode change start mode
            {
                if(fLotInfo->cbRunMode->Text.Pos("FT")>0 ||
                   fLotInfo->cbRunMode->Text.Pos("EQC")>0)
                    SetRunStartMode(rsmInitialStart);
                else
                    SetRunStartMode(rsmCInitialRetest);
            }
            else
            {
                if(CosFunction.bKeepOnly1SetupFile)
                {
                    ClearAllSetupFile(sDLFileName);                             //Steven 20200512 : 刪除全部工作檔, 只留下當下的   //Steven 20221202 : 換位置
                    if(RunInfo.bLotStart==false)
                        SetRunStartMode(rsmInitialStart);
                }
                else
                {
                    SetRunStartMode(rsmInitialStart);
                }
            }
            #ifdef DEBUG
            RecordProcess("btDownloadClick 3");
            #endif
        }
        else
        {
            edTemp->Text="";
            edDeviceName->Text="";
            cbbDeviceName->Text="";
            return;
        }

        if(CUSTOMER_CODE==CC_AMKOR_China ||                                     //jou 2015-06-13 Amkor China客戶要求下載完工作檔後要將Sites全部自動開啟
           CUSTOMER_CODE==CC_QUALCOMM)                                          //JerryYang 20170412 (Steven) add QUALCOMM
        {
            for(int k=0; k<2; k++)
            {
                for(int i=0; i<MAX_SOCKET_ROW; i++)
                {
                    for(int j=0; j<MAX_SOCKET_COL; j++)
                    {
                        LastSet.bUseTestSocket[k][i][j]=true;
                        bTestSiteUse[k][i][j]=LastSet.bUseTestSocket[k][i][j];
                    }
                }
            }
            fMain->ShowTestHeadComp(true);                                      //Steven 20161211 (wei) 有修改開關Site需要重繪畫面
        }
        fMain->ShowTestHeadComp(false);                                         //jou 2015-06-02 有修改開關Site需要重繪畫面
        #ifdef DEBUG
        RecordProcess("btDownloadClick 4");
        #endif

        fMain->ModifyTester(ON_LINE);                                           //Steven 20191218 : 整合修改LastSet.iTester
        NewRecordProcess("MES2157", "Change to On_Line", "by LotInfo Download Button");                                 //ChungHung 20140722 add add record
        fMain->UpdateMainOperateMode();
        fMain->CloseGpibProgram(__FUNC__);
        #ifdef DEBUG
        RecordProcess("btDownloadClick 5");
        #endif
        fMain->LoadTestModePicture();
        LastSet.iRealDummy=REALLY;
        fMain->LoadRunModePicture();
        fMain->UpdateMainOperateMode();
        #ifdef DEBUG
        RecordProcess("btDownloadClick 6");
        #endif

        MySleep(100);
        if(CUSTOMER_CODE==CC_SCC ||                                             //Ifor 20181029 add : SCC RMS 新增資料比對功能
           CUSTOMER_CODE==CC_AMD_M)                                             //Ifor 20200727 :add TF-AMD RMS Data Check
        {                                                                       // record data
            DownloadWorkFile_NET    = SYS_SetupFile;
            LastSetTemperature_NET  = LastSet.iTemperature;
            TrayForm_NET            = TrayForm;
            HotPlateForm_NET        = HotPlateForm_File;
            Temperature_NET         = Temperature;
            TestIF_NET              = TestIF_File;

            DeviceForm_NET          = DeviceForm_File;

            for(int i=0; i<8; i++)                                              //ChungHung 20141002 add for KYEC AutoRetest 3->5 //Ifor 20170418 add MRT Mode 5 -> 8
            {
                BinSelect_NET[i]    = BinSelect[i];
            }
        }

        if(IniConfig.bCheckFile)                                                //Steven 20101208 : 對工作檔和Tester資料進行比對
        {
            bool bFlag=false;
            AnsiString sTemp=edTemp->Text.SubString(1, 3);                      //只抓前三碼
            double fTemp;

            //---------------------------
            //如果是ROOM就是常溫
            //---------------------------
            if(sTemp.UpperCase()=="ROO")
            {
                if(LastSet.iTemperature==Tempture_Ambient)
                {
                    bFlag=false;
                }
                else
                {
                    bFlag=true;
                    asStringE="No Open Ambient mode, please key in again!!";
                    asStringC="未開啟常溫模式,請開啟後重新輸入一次!!";
                }
            }
            else
            {
                //---------------------------
                //可能是105C 或 105c；也有可能是25C 或 25c
                //---------------------------
                if(sTemp.AnsiPos("C")!=0 || sTemp.AnsiPos("c")!=0)
                    sTemp=sTemp.SubString(1, 2);

                fTemp=atof(sTemp.c_str());

                if(IniConfig.bVTESTFunction==true)                              //RogerYang 20250329 Vtest沒有設定IniConfig.fAmbientTemp，修改判斷方式
                {
                    if(LastSet.iTemperature==Tempture_Ambient)
                    {
                        bFlag=false;
                    }
                    else if(LastSet.iTemperature==Tempture_Hot ||               //高溫
                            LastSet.iTemperature==Tempture_AmbientHot)          //RogerYang 20250717 Add 恆溫控制
                    {
                        if(fTemp==Temperature.fWorkTemperBase)                  //高溫的輸入值與設定值相符
                        {
                            bFlag=false;
                        }
                        else
                        {
                            bFlag=true;
                            asStringE="Work Temperature set error, please check and key in again!!";
                            asStringC="工作溫度設定錯誤,請確認後重新輸入一次!!";
                        }
                    }
                    else
                    {
                        bFlag=true;
                        asStringE="Error Temperature Mode, please check Ambient/Hot mode and key in again!!";
                        asStringC="模式設定錯誤，請確認低溫/高溫模式後重新輸入一次!!";
                    }
                }
                else
                {
                    if(fTemp<=IniConfig.fAmbientTemp)                           //常溫
                    {
                        if(LastSet.iTemperature==Tempture_Ambient)
                        {
                            bFlag=false;
                        }
                        else
                        {
                            bFlag=true;
                            asStringE="No Open Ambient mode, please key in again!!";
                            asStringC="未開啟常溫模式,請開啟後重新輸入一次!!";
                        }
                    }
                    else
                    {
                        if(LastSet.iTemperature==Tempture_Hot)                  //高溫
                        {
                            if(fTemp==Temperature.fWorkTemperBase)              //高溫的輸入值與設定值相符
                            {
                                bFlag=false;
                            }
                            else
                            {
                                bFlag=true;
                                asStringE="Work Temperature set error, please key in again!!";
                                asStringC="工作溫度設定錯誤,請開啟後重新輸入一次!!";
                            }
                        }
                        else
                        {
                            bFlag=true;
                            asStringE="No Open Hot mode, please key in again!!";
                            asStringC="未開啟高溫模式,請開啟後重新輸入一次!!";
                        }
                    }
                }
            }

            if(bFlag)
            {
                ShowMyMessage(asStringE,asStringC);
                LastSet.bHasDownloadFile=false;
                edDeviceName->Text="";
                cbbDeviceName->Text="";
                edTemp->Text="";
            }
            else
            {
                LastSet.bHasDownloadFile=true;
                fMain->Clarn_Data(9, "DownloadFile_AutoClearCount");
            }
        }
        else
        {
            LastSet.bHasDownloadFile=true;
            fMain->Clarn_Data(9, "DownloadFile_AutoClearCount");
        }
        #ifdef DEBUG
        RecordProcess("btDownloadClick 1");
        #endif
    }
    else
    {
        LastSet.bHasDownloadFile=true;
        fMain->Clarn_Data(9, "DownloadFile_AutoClearCount");
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfLotInfo::DownloadFromServer_TSMC(AnsiString sOrgFileName , AnsiString sDLFileName)                    //ChungHung 20150413 add for TSMC
{
    bool bCheckHasAllFile=false;
    AnsiString FileName[8]={"ArmCondition.Data", "Binasgn.Data", "Contact.Data", "HandlerCondition.Data",
                            "HotPlate.Data", "Temperature.Data", "Tester.Data", "Tray.Data"};

    bool bNeedCover=false;
    SetCurrentDirectory(_T("D://"));
    //----------------------
    //檢查是不是有斷線
    //----------------------
    if(CosFunction.bFTPFunction==false)                                         //Steven 20110211 : 使用FTP的不需要檢查
    {
        if(DirectoryExists(IniConfig.sRmsPath)==false)
        {
            ShowErrorMessage("WAR1683", 0, MMSystem);                           //Connection Fail.
            return false;
        }
    }

    AnsiString ActiveDataPath = "D:\\HT9045\\IniData\\DataFTP";
    AnsiString Backup         = "D:\\HT9045\\IniData\\Data";
    AnsiString str;
    if(DirectoryExists(ActiveDataPath)==false)
    {
        SetCurrentDirectory(_T("D://"));
        MyForceDirectories(ActiveDataPath);
        MySleep(50);
    }

    if(DirectoryExists(ActiveDataPath+"\\"+sOrgFileName)==false)                //如果沒有工作檔就把原本的複製到FTP資料夾
    {
        if(DirectoryExists(Backup+"\\"+sOrgFileName)==true)
        {
            str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", Backup+"\\"+sOrgFileName, ActiveDataPath+"\\"+sOrgFileName);
            system(str.c_str());
            MySleep(50);
        }
    }

    AnsiString str1, str2, str3;
    AnsiString sConfigPath=AuthPath+"Security_new.def";
    AnsiString sNewFilePath;
    AnsiString SPath[2]={IncludeTrailingPathDelimiter(DataPath), IncludeTrailingPathDelimiter(OffsetPath)};

    int ret=0, ret2=0;
    static int iCount=0;
    bool bHasSetUpFile=false;
    HINSTANCE hInstance;

    str3=fOffSet->GetOffsetPath(sOrgFileName);
    str2.sprintf("%s\\Position Offset.data", str3);                             //wei 20160621 FTPDOWNLOAD前先備份Offset
    if(FileExists(str2))
    {
        str1.sprintf("%s\\Position Offset.bak", str3);
        CopyFile(str2.c_str(), str1.c_str(), false);
        MySleep(50);
    }

    str2.sprintf("%s\\Position Offset Hot.data",  str3);
    if(FileExists(str2))
    {
        str1.sprintf("%s\\Position Offset Hot.bak",  str3);
        CopyFile(str2.c_str(), str1.c_str(), false);
        MySleep(50);
    }

    //----------------------
    //首先判斷工作檔有沒有
    //----------------------
    if(!DirectoryExists(SPath[0]+sDLFileName))
    {
        MyForceDirectories(SPath[0]+sDLFileName);
        bHasSetUpFile=false;
    }
    else
    {
        bHasSetUpFile=true;
    }

    if(bHasSetUpFile==false)                                                    //沒有的話,就先建立新的資料夾
    {
        for(int i=1; i<2; i++)
        {
            MyForceDirectories(SPath[i]+sDLFileName);
        }
        fMain->cbSetupFileName->Items->Add(sDLFileName);
    }

    //jou 2012-12-22 下載 Offset 檔案 start
//    if(CosFunction.bFTPFunction && CUSTOMER_CODE!=CC_TSMC_TAINAN) //ChungHung 20150413 add for TSMC
//    {
//        str3=OffsetPath+sDLFileName+".Offset";
//        if(FileExists(str3))
//        {
//            str1="e \""+str3+"\" -o\""+OffsetPath+sDLFileName+"\\\" -y";
//            hInstance=ShellExecute(this,"open","d:\\HT9045\\7z.exe",str1.c_str(),NULL,SW_HIDE);
//            MySleep(200);
//            DeleteFile(str3.c_str());
//        }
//    }
    //jou 2012-12-22 下載 Offset 檔案 end

    if(!DirectoryExists(Backup+"\\"+sOrgFileName))
    {
        bHasSetUpFile=false;
        ShowMyMessage("Please check parameter for new setup file!","新的設定檔請確認參數");
    }
    else
    {
        bHasSetUpFile=true;
    }

    //----------------------
    //檢查Offset檔
    //----------------------
//    bool bHaveOffset[2]={true,true};          //wei 20160621 mask
//    str1.sprintf("%s%s\\Position Offset.Data", OffsetPath, sOrgFileName);
//    if(!FileExists(str1))    //如果沒有就從當下的工作檔複製過來
//    {
//        str2.sprintf("%s%s\\Position Offset.Data", OffsetPath, GetLastOpenFN());
//        CopyFile(str2.c_str(), str1.c_str(), false);
//        bHaveOffset[0]=false;
//    }
//
//    str1.sprintf("%s%s\\Position Offset Hot.Data", OffsetPath, sOrgFileName);
//    if(!FileExists(str1))
//    {
//        str2.sprintf("\"%s%s\\Position Offset Hot.Data",  OffsetPath, GetLastOpenFN());
//        CopyFile(str2.c_str(), str1.c_str(), false);
//        bHaveOffset[1]=false;
//    }

    if(CosFunction.bFTPFunction==false)                                         //Steven 20110211 : 使用FTP的不需要下載
    {
        //----------------------
        //開始下載檔案
        //----------------------
        do
        {
            if(FileExists("d:\\HT9045\\7z.exe")==false)
            {
                CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe", false);
            }

            if(IniConfig.bSPILFunction==false)                                  //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
            {
                str1.sprintf("%s%s.zip", DataPath, sDLFileName);

                if(CUSTOMER_CODE==CC_SCC ||
                   CUSTOMER_CODE==CC_SCK)                                       //ChungHung 20130621 add SCK RMS
                {
                    str2.sprintf("%s\\%s.zip", IniConfig.sRmsDownPath, sDLFileName);
                }
                else
                {
                    str2.sprintf("%s\\%s.zip", IniConfig.sRmsPath, sDLFileName);
                }

                ret=CopyFile(str2.c_str(), str1.c_str(), false);                //複製一份新的
                MySleep(100);

                if(ret==0)
                {
                    ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, sDLFileName);                         //下載 %s.zip 失敗，重試??
                    if(ret2==K_SKIP)
                        return false;
                }
                else
                {
                    ret2=0;
                }
            }
            else
            {
                ret2=0;
            }
        }
        while(ret2==1);
    }

    //----------------------
    //把要還原的資料先備份
    //----------------------
//    double fTempUserOffset[tcTotalCount], fContactHeight[25];   //ChungHung 20140804 add 修正 RMS 原本 只還原到前十個 uer Offset   //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    int iIndexHeatingMode;
    int iShuttleMode[2];
    int iTrayMapDate[12];
    if(bHasSetUpFile)
    {
        //----------------------
        //把溫度Offset資料備份
        //----------------------
        str1.sprintf("%s%s\\Temperature.Data", DataPath, sOrgFileName);
        //for(int i=0; i<10; i++)
        for(int i=0; i<tcTotalCount; i++)                                       //ChungHung 20140804 add 修正 RMS 原本 只還原到前十個 uer Offset
        {
            str2.printf("CH%d", i+1);
            fTempUserOffset[i]=ReadIniData(str1, "User OffSet", str2, 0.0);
            if(ATC_SYSTEM>eATC30 && ATC_SYSTEM!=eNonChamber)                    //Steven 20221209 : ATC offset要不要覆蓋
            {
                str2.sprintf("ATCTempOffset[%d]", i);
                fTempATCOffset[i]=ReadIniData(str1, "ATC", str2, 0.0);
            }
        }
        iIndexHeatingMode=CheckAndReadIniData(str1, "Index",   "Heating Mode",   0);                                    //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式
        MySleep(100);

        //----------------------
        //把HotPlate資料備份
        //----------------------
        str1.sprintf("%s%s\\HotPlate.Data", DataPath, sOrgFileName);            //wei 20160419 少一個\\
        str2.sprintf("%sHotPlate.Data", DataPath);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);

        //----------------------
        //把Contact高度備份
        //----------------------
        str1.sprintf("%s%s\\Contact.Data", DataPath, sOrgFileName);
        fContactHeight[0]=ReadIniData(str1, "Test Arm1", "Pick Up", 0.0);
        fContactHeight[1]=ReadIniData(str1, "Test Arm1", "Contact", 0.0);
        fContactHeight[2]=ReadIniData(str1, "Test Arm1", "Drop",    0.0);
        fContactHeight[3]=ReadIniData(str1, "Test Arm1", "Place",   0.0);
        fContactHeight[4]=ReadIniData(str1, "Test Arm2", "Pick Up", 0.0);
        fContactHeight[5]=ReadIniData(str1, "Test Arm2", "Contact", 0.0);
        fContactHeight[6]=ReadIniData(str1, "Test Arm2", "Drop",    0.0);
        fContactHeight[7]=ReadIniData(str1, "Test Arm2", "Place",   0.0);
        fContactHeight[8]=ReadIniData(str1, "Torque Control", "Pin Number",    0.0);
        fContactHeight[9]=ReadIniData(str1, "Torque Control", "Force Per Pin", 0.0);
        fContactHeight[19]=ReadIniData(str1,"Torque Control", "Torque",        0.0);
        fContactHeight[20]=ReadIniData(str1, "Test Arm1", "ContactBackUp",     0.0);                                    //Steven 20190822 : download cover file include ContactBackUp and ShuttlePickBackUp
        fContactHeight[21]=ReadIniData(str1, "Test Arm2", "ContactBackUp",     0.0);
        fContactHeight[22]=ReadIniData(str1, "Test Arm1", "ShuttlePickBackUp",     0.0);
        fContactHeight[23]=ReadIniData(str1, "Test Arm2", "ShuttlePickBackUp",     0.0);

        fContactHeight[10]=ReadIniData(str1, "Wait Time", "Drop Wait",   1.0);
        fContactHeight[11]=ReadIniData(str1, "Wait Time", "Drop Speed",  1.0);
        fContactHeight[12]=ReadIniData(str1, "Mode", "Contact",                          0.0);
        fContactHeight[13]=ReadIniData(str1, "Mode", "Vacuum",                           0.0);
        fContactHeight[14]=ReadIniData(str1, "Mode", "Dummy Contact",                    0.0);
        fContactHeight[15]=ReadIniData(str1, "Mode", "Head Device Mode",                 0.0);
        fContactHeight[16]=ReadIniData(str1, "Mode", "Kit Diameter",                     3.0);
        fContactHeight[17]=ReadIniData(str1, "Mode", "Suck Shuttle Device After Tested", 0.0);
        fContactHeight[18]=ReadIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber", 0.0);

        fContactHeight[24]=ReadIniData(str1, "Mode", "Die Force Kit Diameter",           2.0);                          //Ifor 20191003 : add Die Force 可以自定義Kit直徑
        //----------------------
        //把Ld/Uld速度備份
        //----------------------
        str1.sprintf("%s%s\\UdUld.Data", DataPath, sOrgFileName);
        str2.sprintf("%sUdUld.Data", DataPath);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);

        //----------------------
        //把速度設定備份
        //----------------------
        str1.sprintf("%s%s\\ArmCondition.Data", DataPath, sOrgFileName);
        str2.sprintf("%sArmCondition.Data", DataPath);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);

        //----------------------
        //備份開啟單一Shuttle選項
        //----------------------
        str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sOrgFileName);
        iShuttleMode[0]=ReadIniData(str1, "Configuration", "Shuttle Mode", 0);
        iShuttleMode[1]=ReadIniData(str1, "Configuration", "Shuttle1 Cancel", 0);

        //wei 20161219 Tray Mapping
        //==>
        iTrayMapDate[0]=ReadIniData(str1, "Configuration", "Tray Mapping Enable",  false);
        iTrayMapDate[1]=ReadIniData(str1, "Configuration", "Tray Mapping ID Enable", false);
        iTrayMapDate[2]=ReadIniData(str1, "Configuration", "Tray Mapping Start Delay",            100);
        iTrayMapDate[3]=ReadIniData(str1, "Configuration", "Tray Mapping Exposure Time Out",      100);
        iTrayMapDate[4]=ReadIniData(str1, "Configuration", "Tray Mapping Get Result Time Out",    100);
        iTrayMapDate[5]=ReadIniData(str1, "Configuration", "Tray Mapping Auto Retry",             1);

        iTrayMapDate[6]=ReadIniData(str1, "Configuration", "Tray Code Min Length",                5);
        iTrayMapDate[7]=ReadIniData(str1, "Configuration", "Tray Code Max Length",                30);
        iTrayMapDate[8]=ReadIniData(str1, "Configuration", "Tray Mapping Catch Count",            4);
        iTrayMapDate[9]=ReadIniData(str1, "Configuration", "Tray Mapping Catch Shift",            1000);

        iTrayMapDate[10]=ReadIniData(str1, "Configuration", "Disable Tray Mapping Suck",           false);
        iTrayMapDate[11]=ReadIniData(str1, "Configuration", "Enable Tray Suck Map Check",          false);
        //<==
        //wei 20161219 Tray Mapping
    }

    //----------------------
    //解壓縮檔案並覆蓋到原本的資料夾
    //----------------------
    if(FileExists(DataPath+sDLFileName+".zip"))                                 //Steven 20110603
    {
        sNewFilePath=DataPath+sDLFileName+"\\";
        FileListBox2->Directory=sNewFilePath;                                   //Steven 20140609

        hInstance=NULL;
        if(IniConfig.FtpUseSystemCallToUnZip)                                   //Steven 20140609
        {
            str1="d:\\HT9045\\7z.exe e \""+DataPath+sDLFileName+".zip\" -o\""+DataPath+sDLFileName+"\\\" -y";
            ret=system(str1.c_str());
        }
        else
        {
            //jou 2012-12-14 system改採用ShellExecute
            str1="e \""+DataPath+sDLFileName+".zip\" -o\""+DataPath+sDLFileName+"\\\" -y";
            hInstance=ShellExecute(this,"open","d:\\HT9045\\7z.exe",str1.c_str(),NULL,SW_HIDE);
        }
    }
    else
    {
        ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, sDLFileName);  //下載 %s.zip 失敗
        return false;
    }

    iCount=0;
    do
    {
        FileListBox2->Update();
        iCount++;
        MySleep(500);                                                           //Steven 20140609 : 1000 --> 100
        if(iCount>100)
        {
            ret=-1;
            break;
        }
    }while(FileListBox2->Items->Count<8);                                       //Steven 20140609 : 確認下載解壓縮後的檔案最少8個

    MySleep(500);                                                               //Steven 20140609 : 1000 --> 100
    bCheckHasAllFile=true;
    for(int i=0; i<8; i++)
    {
        str1.sprintf("%s%s\\%s", DataPath, sDLFileName, FileName[i]);
        if(FileExists(str1)==false)
        {
            bCheckHasAllFile=false;
        }
    }

    if(bCheckHasAllFile &&
       ((IniConfig.FtpUseSystemCallToUnZip && ret==0) || int(hInstance)>32))
    {
        //----------------------
        //把壓縮檔砍掉
        //----------------------
        str1.sprintf("%s%s.zip", DataPath, sDLFileName);                        //jou 2012-12-14 system改採用DeleteFile
        DeleteFile(str1.c_str());
        MySleep(100);

        if(bHasSetUpFile &&                                                     //如果不是新的工作檔就要進行資料還原
           (IniConfig.bEnableRms==true || IniConfig.bEnableErms==true))         //Sam 20181210 : 修正下載檔案被還原的問題
        {
            //----------------------
            //不覆蓋就是必須要還原
            // 0 : 不覆蓋
            // 1 : 要覆蓋
            //----------------------

            //----------------------
            //把溫度Offset資料還原
            //----------------------
            if(CosFunction.bDownloadRecipeLevelMode)                            //jou 2016-01-06 download recipe 增加權限模式選擇
            {
                if(fLotInfo->coLevelMode->Text!="Normal")
                    bNeedCover=true;
                else
                    bNeedCover=false;
            }
            else
            {
                bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Temp Offset", true);
            }

            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制要還原
                bNeedCover=false;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
                for(int i=0; i<tcTotalCount; i++)                               //ChungHung 20140804 add 修正 RMS 原本 只還原到前十個 uer Offset
                {
                    str2.printf("CH%d", i+1);
                    WriteIniData(str1, "User OffSet", str2, fTempUserOffset[i]);
                    if(ATC_SYSTEM>eATC30 && ATC_SYSTEM!=eNonChamber)            //Steven 20221209 : ATC offset要不要覆蓋
                    {
                        str2.sprintf("ATCTempOffset[%d]", i);
                        WriteIniData(str1, "ATC", str2, fTempATCOffset[i]);
                    }
                }
            }

            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Index Heat Mode", true);                            //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式
            if(bNeedCover==false)
            {
                str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Index",   "Heating Mode",   iIndexHeatingMode);
            }

            //----------------------
            //把Contact高度還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact High", false);                              //預設不覆蓋
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Test Arm1", "Pick Up", fContactHeight[0]);
                WriteIniData(str1, "Test Arm1", "Contact", fContactHeight[1]);
                WriteIniData(str1, "Test Arm1", "Drop",    fContactHeight[2]);
                WriteIniData(str1, "Test Arm1", "Place",   fContactHeight[3]);
                WriteIniData(str1, "Test Arm2", "Pick Up", fContactHeight[4]);
                WriteIniData(str1, "Test Arm2", "Contact", fContactHeight[5]);
                WriteIniData(str1, "Test Arm2", "Drop",    fContactHeight[6]);
                WriteIniData(str1, "Test Arm2", "Place",   fContactHeight[7]);
                WriteIniData(str1, "Test Arm1", "ContactBackUp",     fContactHeight[20]);                               //Steven 20190822 : download cover file include ContactBackUp and ShuttlePickBackUp
                WriteIniData(str1, "Test Arm2", "ContactBackUp",     fContactHeight[21]);
                WriteIniData(str1, "Test Arm1", "ShuttlePickBackUp", fContactHeight[22]);
                WriteIniData(str1, "Test Arm2", "ShuttlePickBackUp", fContactHeight[23]);
            }

            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact Force", true);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Torque Control", "Pin Number",    fContactHeight[8]);
                WriteIniData(str1, "Torque Control", "Force Per Pin", fContactHeight[9]);
                WriteIniData(str1, "Torque Control", "Torque"       , fContactHeight[19]);
            }

            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact Mode", true);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Wait Time", "Drop Wait",                    fContactHeight[10]);
                WriteIniData(str1, "Wait Time", "Drop Speed",                   fContactHeight[11]);
                WriteIniData(str1, "Mode", "Contact",                           fContactHeight[12]);
                WriteIniData(str1, "Mode", "Vacuum",                            fContactHeight[13]);
                WriteIniData(str1, "Mode", "Dummy Contact",                     fContactHeight[14]);
                WriteIniData(str1, "Mode", "Head Device Mode",                  fContactHeight[15]);
                WriteIniData(str1, "Mode", "Kit Diameter",                      fContactHeight[16]);
                WriteIniData(str1, "Mode", "Suck Shuttle Device After Tested",  fContactHeight[17]);
                WriteIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber",  fContactHeight[18]);

                WriteIniData(str1, "Mode", "Die Force Kit Diameter",            fContactHeight[24]);                    //Ifor 20191003 : add Die Force 可以自定義Kit直徑
            }

            //----------------------
            //把HotPlate資料還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "HotPlate", false);

            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%sHotPlate.Data", DataPath);
                str2.sprintf("%s%s\\HotPlate.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(100);
            }

//            str1.sprintf("del \"%sHotPlate.Data\"", DataPath);                //刪除備份
//            system(str1.c_str());
            //jou 2012-12-14 system改採用DeleteFile
            str1.sprintf("%sHotPlate.Data", DataPath);                          //刪除備份
            DeleteFile(str1.c_str());
            MySleep(100);

            //----------------------
            //把Ld/Uld速度還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Load Unload", false);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%sUdUld.Data", DataPath);
                str2.sprintf("%s%s\\UdUld.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(100);
            }

//            str1.sprintf("del \"%sUdUld.Data\"", DataPath);                   //刪除備份
//            system(str1.c_str());
            //jou 2012-12-14 system改採用DeleteFile
            str1.sprintf("%sUdUld.Data", DataPath);                             //刪除備份
            DeleteFile(str1.c_str());
            MySleep(100);

            //----------------------
            //把速度設定備份
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Speed Setting", true);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%sArmCondition.Data", DataPath);
                str2.sprintf("%s%s\\ArmCondition.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(100);
            }

//            str1.sprintf("del \"%sArmCondition.Data\"", DataPath);            //刪除備份
//            system(str1.c_str());
            //jou 2012-12-14 system改採用DeleteFile
            str1.sprintf("%sArmCondition.Data", DataPath);                      //刪除備份
            DeleteFile(str1.c_str());
            MySleep(100);

            //----------------------
            //備份開啟單一Shuttle選項
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Shuttle Mode", false);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Configuration", "Shuttle Mode", iShuttleMode[0]);
                WriteIniData(str1, "Configuration", "Shuttle1 Cancel", iShuttleMode[1]);
            }

            //wei 20161219 Tray Mapping
            //==>
            str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
            WriteIniData(str1, "Configuration", "Tray Mapping Enable",      iTrayMapDate[0]);
            WriteIniData(str1, "Configuration", "Tray Mapping ID Enable",   iTrayMapDate[1]);
            WriteIniData(str1, "Configuration", "Tray Mapping Start Delay", iTrayMapDate[2]);
            WriteIniData(str1, "Configuration", "Tray Mapping Exposure Time Out",iTrayMapDate[3]);
            WriteIniData(str1, "Configuration", "Tray Mapping Get Result Time Out", iTrayMapDate[4]);
            WriteIniData(str1, "Configuration", "Tray Mapping Auto Retry",  iTrayMapDate[5]);
            WriteIniData(str1, "Configuration", "Tray Code Min Length",     iTrayMapDate[6]);
            WriteIniData(str1, "Configuration", "Tray Code Max Length",     iTrayMapDate[7]);
            WriteIniData(str1, "Configuration", "Tray Mapping Catch Count", iTrayMapDate[8]);
            WriteIniData(str1, "Configuration", "Tray Mapping Catch Shift", iTrayMapDate[9]);
            WriteIniData(str1, "Configuration", "Disable Tray Mapping Suck",iTrayMapDate[10]);
            WriteIniData(str1, "Configuration", "Enable Tray Suck Map Check",iTrayMapDate[11]);
            //<==
            //wei 20161219 Tray Mapping

            //----------------------
            //把TestMode還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Test Mode", false);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%sTestMode.Data", DataPath);
                str2.sprintf("%s%s\\TestMode.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(100);
            }

            //jou 2012-12-14 system改採用DeleteFile
            str1.sprintf("%sTestMode.Data", DataPath);                          //刪除備份
            DeleteFile(str1.c_str());
            MySleep(100);

            //----------------------
            //把Binasgn還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Binasgn", false);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%sBinasgn.Data", DataPath);
                str2.sprintf("%s%s\\Binasgn.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(100);
            }

            //jou 2012-12-14 system改採用DeleteFile
            str1.sprintf("%sBinasgn.Data", DataPath);                           //刪除備份
            DeleteFile(str1.c_str());
            MySleep(100);

            //----------------------
            //把Binasgn還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "BinasgnOff", false);
            if(CUSTOMER_CODE==CC_Greatek)                                       //Sam 20171015 (wei) : 超豐強制覆蓋
                bNeedCover=true;
            if(bNeedCover==false)                                               //不覆蓋就要還原
            {
                str1.sprintf("%sBinasgnOff.Data", DataPath);
                str2.sprintf("%s%s\\BinasgnOff.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(100);
            }

            //jou 2012-12-14 system改採用DeleteFile
            str1.sprintf("%sBinasgnOff.Data", DataPath);                        //刪除備份
            DeleteFile(str1.c_str());
            MySleep(100);
        }

        MySleep(1000);
        if(fFTPClient->bControlBySECSGEM==false &&                              //ChungHung 20150515 add Control by SECSGEM
           fFTPClient->bControlByGPIB==false)                                   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        {
            if(CUSTOMER_CODE!=CC_Greatek)                                       //Sam 20170919 (Steven) : 超豐要求 Server Download 不要顯示訊息
                ShowErrorMessage("MES1687", 0, MMSystem, 0, sDLFileName);       //UnZip %s.zip OK.
        }
        return true;
    }
    else
    {
        if(fFTPClient->bControlBySECSGEM==false &&                              //ChungHung 20150515 add Control by SECSGEM
           fFTPClient->bControlByGPIB==false)                                   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        {
            ShowErrorMessage("WAR1686", 0, MMSystem, 0, sDLFileName);           //UnZip %s.zip Fail.
        }
        return false;
    }
}
//---------------------------------------------------------------------------
int TfLotInfo::DoBackupSetupFile(AnsiString DataPath, AnsiString sDLFileName1)  //Steven 20191101 : 整合下載工作檔的覆蓋方式
{
    AnsiString str, str1, str2, sLog="";;
    int ret=0;

    //Sam 20210803 : FTP SetFile Change Log
    //==>
    sLog.sprintf("Backup [%s] ", sDLFileName1);
    WriteFTPSetupFileChangeLog(sLog);
    //<==
    //Sam 20210803 : FTP SetFile Change Log

    //----------------------
    //把溫度Offset資料備份
    //----------------------
    str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName1);

    for(int i=0; i<tcTotalCount; i++)                                           //ChungHung 20140804 add 修正 RMS 原本 只還原到前十個 uer Offset
    {
        str2.printf("CH%d", i+1);
        fTempUserOffset[i]=ReadIniData(str1, "User OffSet", str2, 0.0);
        if(ATC_SYSTEM>eATC30 && ATC_SYSTEM!=eNonChamber)                        //Steven 20221209 : ATC offset要不要覆蓋
        {
            str2.sprintf("ATCTempOffset[%d]", i);
            fTempATCOffset[i]=ReadIniData(str1, "ATC", str2, 0.0);
        }
    }
    iIndexHeatingMode=CheckAndReadIniData(str1, "Index",   "Heating Mode",   0);                                        //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式
    MySleep(100);

    //Sam 20210803 : FTP SetFile Change Log
    //==>
    for(int i=0;i<tcTotalCount;i++)
    {
        sLog.sprintf("fTempUserOffset[%d]=%3.3f", i, fTempUserOffset[i]);
        WriteFTPSetupFileChangeLog(sLog);

        if(ATC_SYSTEM>eATC30 && ATC_SYSTEM!=eNonChamber)                        //Steven 20221209 : ATC offset要不要覆蓋
        {
            sLog.sprintf("fTempATCOffset[%d]=%3.3f", i, fTempATCOffset[i]);
            WriteFTPSetupFileChangeLog(sLog);
        }
    }
    //<==
    //Sam 20210803 : FTP SetFile Change Log

    //----------------------
    //把HotPlate資料備份
    //----------------------
    str1.sprintf("%s%s\\HotPlate.Data", DataPath, sDLFileName1);                //Ifor 20190520 :add HotPlate.Data路徑少\\
    str2.sprintf("%sHotPlate.Data", DataPath);
    ret=CopyFile(str1.c_str(), str2.c_str(), false);
    MySleep(100);

    //----------------------
    //把Contact高度備份
    //----------------------
    if(CosFunction.bContactHeightSaveToContactIni)                              //Steven 20200616 : JSCC要求把Contact Height放到別的檔案
    {
        str1="D:\\HT9045\\system\\Contact.ini";
        if(FileExists(str1)==false || CheckIniData(str1, sDLFileName1, "Test Arm1")==false)
        {
            fContactHeight[0]=0.0;
            fContactHeight[1]=0.0;
            fContactHeight[2]=0.0;
            fContactHeight[3]=0.0;
            fContactHeight[4]=0.0;
            fContactHeight[5]=0.0;
            fContactHeight[6]=0.0;
            fContactHeight[7]=0.0;
            fContactHeight[20]=0.0;
            fContactHeight[21]=0.0;
            fContactHeight[22]=0.0;
            fContactHeight[23]=0.0;
        }
        else
        {
            fContactHeight[0]=ReadIniData(str1, sDLFileName1, "Pick Up1", 0.0);
            fContactHeight[1]=ReadIniData(str1, sDLFileName1, "Test Arm1", 0.0);
            fContactHeight[2]=ReadIniData(str1, sDLFileName1, "Drop1",    0.0);
            fContactHeight[3]=ReadIniData(str1, sDLFileName1, "Place1",   0.0);
            fContactHeight[4]=ReadIniData(str1, sDLFileName1, "Pick Up2", 0.0);
            fContactHeight[5]=ReadIniData(str1, sDLFileName1, "Test Arm2", 0.0);
            fContactHeight[6]=ReadIniData(str1, sDLFileName1, "Drop2",    0.0);
            fContactHeight[7]=ReadIniData(str1, sDLFileName1, "Place2",   0.0);
            fContactHeight[20]=ReadIniData(str1, sDLFileName1, "ContactBackUp1",     0.0);                              //Steven 20190822 : download cover file include ContactBackUp and ShuttlePickBackUp
            fContactHeight[21]=ReadIniData(str1, sDLFileName1, "ContactBackUp2",     0.0);
            fContactHeight[22]=ReadIniData(str1, sDLFileName1, "ShuttlePickBackUp1",     0.0);
            fContactHeight[23]=ReadIniData(str1, sDLFileName1, "ShuttlePickBackUp2",     0.0);
        }
    }
    else
    {
        str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName1);
        fContactHeight[0]=ReadIniData(str1, "Test Arm1", "Pick Up", 0.0);
        fContactHeight[1]=ReadIniData(str1, "Test Arm1", "Contact", 0.0);
        fContactHeight[2]=ReadIniData(str1, "Test Arm1", "Drop",    0.0);
        fContactHeight[3]=ReadIniData(str1, "Test Arm1", "Place",   0.0);
        fContactHeight[4]=ReadIniData(str1, "Test Arm2", "Pick Up", 0.0);
        fContactHeight[5]=ReadIniData(str1, "Test Arm2", "Contact", 0.0);
        fContactHeight[6]=ReadIniData(str1, "Test Arm2", "Drop",    0.0);
        fContactHeight[7]=ReadIniData(str1, "Test Arm2", "Place",   0.0);
        fContactHeight[20]=ReadIniData(str1, "Test Arm1", "ContactBackUp",     0.0);                                    //Steven 20190822 : download cover file include ContactBackUp and ShuttlePickBackUp
        fContactHeight[21]=ReadIniData(str1, "Test Arm2", "ContactBackUp",     0.0);
        fContactHeight[22]=ReadIniData(str1, "Test Arm1", "ShuttlePickBackUp",     0.0);
        fContactHeight[23]=ReadIniData(str1, "Test Arm2", "ShuttlePickBackUp",     0.0);
    }

    str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName1);
    fContactHeight[8]=ReadIniData(str1, "Torque Control", "Pin Number",    0.0);
    fContactHeight[9]=ReadIniData(str1, "Torque Control", "Force Per Pin", 0.0);
    fContactHeight[19]=ReadIniData(str1,"Torque Control", "Torque",        0.0);

    fContactHeight[10]=ReadIniData(str1, "Wait Time", "Drop Wait",   1.0);
    fContactHeight[11]=ReadIniData(str1, "Wait Time", "Drop Speed",  1.0);
    fContactHeight[12]=ReadIniData(str1, "Mode", "Contact",                          0.0);
    fContactHeight[13]=ReadIniData(str1, "Mode", "Vacuum",                           0.0);
    fContactHeight[14]=ReadIniData(str1, "Mode", "Dummy Contact",                    0.0);
    fContactHeight[15]=ReadIniData(str1, "Mode", "Head Device Mode",                 0.0);
    fContactHeight[16]=ReadIniData(str1, "Mode", "Kit Diameter",                     3.0);
    fContactHeight[17]=ReadIniData(str1, "Mode", "Suck Shuttle Device After Tested", 0.0);
    fContactHeight[18]=ReadIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber", 0.0);

    fContactHeight[24]=ReadIniData(str1, "Mode", "Die Force Kit Diameter",           2.0);                              //Ifor 20191003 : add Die Force 可以自定義Kit直徑

   //Sam 20210803 : FTP SetFile Change Log
    //==>
    for(int i=0;i<25;i++)
    {
        sLog.sprintf("fContactHeight[%d]=%3.3f",i,fContactHeight[i]);
        WriteFTPSetupFileChangeLog(sLog);
    }
    //<==
    //Sam 20210803 : FTP SetFile Change Log

    //----------------------
    //把Ld/Uld速度備份
    //----------------------
    str1.sprintf("%s%s\\UdUld.Data", DataPath, sDLFileName1);
    str2.sprintf("%sUdUld.Data", DataPath);
    ret=CopyFile(str1.c_str(), str2.c_str(), false);
    MySleep(100);

    //----------------------
    //把速度設定備份
    //----------------------
    str1.sprintf("%s%s\\ArmCondition.Data", DataPath, sDLFileName1);
    str2.sprintf("%sArmCondition.Data", DataPath);                              //Ifor 20190520 :add ArmCondition.Data路徑多了\\
    ret=CopyFile(str1.c_str(), str2.c_str(), false);
    MySleep(100);

    //----------------------
    //備份開啟單一Shuttle選項
    //----------------------
    str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName1);
    iShuttleMode[0]=ReadIniData(str1, "Configuration", "Shuttle Mode", 0);
    iShuttleMode[1]=ReadIniData(str1, "Configuration", "Shuttle1 Cancel", 0);

   //Sam 20210803 : FTP SetFile Change Log
    //==>
    for(int i=0; i<2; i++)
    {
        sLog.sprintf("iShuttleMode[%d]=%d", i, iShuttleMode[i]);
        WriteFTPSetupFileChangeLog(sLog);
    }
    //<==
    //Sam 20210803 : FTP SetFile Change Log

    //----------------------
    //備份Auto Retest選項
    //----------------------                                                    //Steven 20190918 : ART設定下載不覆蓋
    if(CosFunction.bUseSCKART)
    {
        str1.sprintf("%s%s\\Tester.Data", DataPath, sDLFileName1);
        bART[0]=ReadIniData(str1, "AutoRetest", "Enable ART",              false);
        bART[1]=ReadIniData(str1, "AutoRetest", "Run ART Without Cmd",     false);
        bART[2]=ReadIniData(str1, "AutoRetest", "Auto Socket Off",         false);
        iART=ReadIniData(str1, "AutoRetest", "Try Count",               3);
    }

    //----------------------
    //備份Auto Clean選項
    //----------------------                                                    //Steven 20161116 : ATC說要加上Auto Clean
    str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName1);        //Steven 20220729 : Add

    iAutoClean[ 0]=ReadIniData(str1, "Configuration", "iAutoClean_ContactShiftHeight",  AnsiString("1000"));
    iAutoClean[ 1]=ReadIniData(str1, "Configuration", "iAutoClean_ContactCleanHeight",  AnsiString("0"));
    iAutoClean[ 2]=ReadIniData(str1, "Configuration", "iAutoClean_IndexPickOffset",     AnsiString("0"));
    iAutoClean[ 3]=ReadIniData(str1, "Configuration", "iAutoClean_IndexReleaseOffset",  AnsiString("0"));
    iAutoClean[ 4]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle1PickOffset",  AnsiString("-200"));
    iAutoClean[ 5]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle1PlaceOffset", AnsiString("0"));
    iAutoClean[ 6]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle1XOffset",     AnsiString("0"));
    iAutoClean[ 7]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle1YOffset",     AnsiString("0"));
    iAutoClean[ 8]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle2PickOffset",  AnsiString("-200"));
    iAutoClean[ 9]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle2PlaceOffset", AnsiString("0"));
    iAutoClean[10]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle2XOffset",     AnsiString("0"));
    iAutoClean[11]=ReadIniData(str1, "Configuration", "iAutoClean_Shuttle2YOffset",     AnsiString("0"));
    iAutoClean[12]=ReadIniData(str1, "Configuration", "HotplatlXOffset",                AnsiString("0.0"));
    iAutoClean[13]=ReadIniData(str1, "Configuration", "HotplatlYOffset",                AnsiString("0.0"));
    iAutoClean[14]=ReadIniData(str1, "Configuration", "HotplatlPickOffset",             AnsiString("0.0"));
    iAutoClean[15]=ReadIniData(str1, "Configuration", "HotplatlPlaceOffset",            AnsiString("0.0"));             //kevin 20150209 end add offset
    iAutoClean[16]=ReadIniData(str1, "Configuration", "HotplatlPitchOffset",            AnsiString("0.0"));             //kevin 20150526
    iAutoClean[17]=ReadIniData(str1, "Configuration", "ShuttlePitchOffset",             AnsiString("0.0"));             //kevin 20150526
    iAutoClean[18]=ReadIniData(str1, "Configuration", "iAutoClean_Function",            AnsiString("0"));
    iAutoClean[19]=ReadIniData(str1, "Configuration", "iAutoClean_Mode",                AnsiString("0"));
    iAutoClean[20]=ReadIniData(str1, "Configuration", "iAutoClean_Tray",                AnsiString("0"));
    iAutoClean[21]=ReadIniData(str1, "Configuration", "iAutoClean_AlarmCount",          AnsiString("10"));
    iAutoClean[22]=ReadIniData(str1, "Configuration", "iAutoClean_iPadThickness",       AnsiString("10"));              //kevin 20180630 add clean pad - device
    iAutoClean[23]=ReadIniData(str1, "Configuration", "iAutoClean_MotorSpeed[0]",       AnsiString("10"));
    iAutoClean[24]=ReadIniData(str1, "Configuration", "iAutoClean_MotorSpeed[1]",       AnsiString("10"));
    iAutoClean[25]=ReadIniData(str1, "Configuration", "iAutoClean_MotorSpeed[2]",       AnsiString("10"));
    iAutoClean[26]=ReadIniData(str1, "Configuration", "iAutoClean_MotorSpeed[3]",       AnsiString("10"));
    iAutoClean[27]=ReadIniData(str1, "Configuration", "iAutoClean_ContactMode",         AnsiString("0"));
    iAutoClean[28]=ReadIniData(str1, "Configuration", "iAutoClean_DropHigh",            AnsiString("0"));               //kevin 20180717 autoClean drop high
    iAutoClean[29]=ReadIniData(str1, "Configuration", "iAutoClean_ContactTime",         AnsiString("5"));
    iAutoClean[30]=ReadIniData(str1, "Configuration", "iAutoClean_ContactCount",        AnsiString("3"));
    iAutoClean[31]=ReadIniData(str1, "Configuration", "iAutoClean_DevicePinCount",      AnsiString("0"));
    iAutoClean[32]=ReadIniData(str1, "Configuration", "fAutoClean_ForcePerPin",         AnsiString("0.0"));             //kevin 20150826
    iAutoClean[33]=ReadIniData(str1, "Configuration", "fAutoClean_DevicePinForceGf",    AnsiString("0.0"));
    iAutoClean[34]=ReadIniData(str1, "Configuration", "fAutoClean_AireForce",           AnsiString("0.0"));
    iAutoClean[35]=ReadIniData(str1, "Configuration", "dAutoClean_XPitch_Kit",          AnsiString("2200.0"));
    iAutoClean[36]=ReadIniData(str1, "Configuration", "dAutoClean_YPitch_Kit",          AnsiString("2200.0"));
    iAutoClean[37]=ReadIniData(str1, "Configuration", "dAutoClean_XStart_Kit",          AnsiString("3300.0"));
    iAutoClean[38]=ReadIniData(str1, "Configuration", "dAutoClean_YStart_Kit",          AnsiString("1800.0"));
    iAutoClean[39]=ReadIniData(str1, "Configuration", "iAutoClean_XDivision_Kit",       AnsiString("8"));
    iAutoClean[40]=ReadIniData(str1, "Configuration", "iAutoClean_YDivision_Kit",       AnsiString("2"));
    iAutoClean[41]=ReadIniData(str1, "Configuration", "dAutoClean_XPitch_Tray",         AnsiString("2200.0"));
    iAutoClean[42]=ReadIniData(str1, "Configuration", "dAutoClean_YPitch_Tray",         AnsiString("2200.0"));
    iAutoClean[43]=ReadIniData(str1, "Configuration", "dAutoClean_XStart_Tray",         AnsiString("3300.0"));
    iAutoClean[44]=ReadIniData(str1, "Configuration", "dAutoClean_YStart_Tray",         AnsiString("1800.0"));
    iAutoClean[45]=ReadIniData(str1, "Configuration", "iAutoClean_XDivision_Tray",      AnsiString("8"));
    iAutoClean[46]=ReadIniData(str1, "Configuration", "iAutoClean_YDivision_Tray",      AnsiString("2"));
    iAutoClean[47]=ReadIniData(str1, "Configuration", "cAutoClean_PackageTray",         AnsiString("BGA"));
    iAutoClean[48]=ReadIniData(str1, "Configuration", "iAutoClean_IntervalContact",     AnsiString("20"));
    iAutoClean[49]=ReadIniData(str1, "Configuration", "iAutoClean_DeveicePices",        AnsiString("8"));
    iAutoClean[50]=ReadIniData(str1, "Configuration", "iCleanIndexOtherArm",            AnsiString("0"));
    iAutoClean[51]=ReadIniData(str1, "Configuration", "iAutoClean_SelectArm",           AnsiString("0"));               //0:Arm1 1:Arm2 2:Arm1 & Arm2
    iAutoClean[52]=ReadIniData(str1, "Configuration", "bAutoClean_FailAlarmLowYield",   AnsiString("0"));
    iAutoClean[53]=ReadIniData(str1, "Configuration", "iAutoClean_LowYieldLimit",       AnsiString("80"));
    iAutoClean[54]=ReadIniData(str1, "Configuration", "iAutoClean_LowYieldCount",       AnsiString("1000"));
    iAutoClean[55]=ReadIniData(str1, "Configuration", "bAutoClean_FailAlarmSiteYieldDifferent", AnsiString("0"));
    iAutoClean[56]=ReadIniData(str1, "Configuration", "iAutoClean_FailAlarmSiteYield", AnsiString("80"));
    iAutoClean[57]=ReadIniData(str1, "Configuration", "iAutoClean_FailAlarmSiteYieldDifferentCount", AnsiString("1000"));
    iAutoClean[58]=ReadIniData(str1, "Configuration", "bAutoClean_ConseFailureBySocket_Normal", AnsiString("0"));
    iAutoClean[59]=ReadIniData(str1, "Configuration", "iAutoClean_ConseFailureCountBySocket_Normal", AnsiString("10"));
    iAutoClean[60]=ReadIniData(str1, "Configuration", "bAutoClean_ConseFailureBySocket_Retest", AnsiString("0"));
    iAutoClean[61]=ReadIniData(str1, "Configuration", "iAutoClean_ConseFailureCountBySocket_Retest", AnsiString("10"));
    iAutoClean[62]=ReadIniData(str1, "Configuration", "bAutoClean_ConseFailureByHead_Normal", AnsiString("0"));
    iAutoClean[63]=ReadIniData(str1, "Configuration", "iAutoClean_ConseFailureCountByHead_Normal", AnsiString("10"));
    iAutoClean[64]=ReadIniData(str1, "Configuration", "bAutoClean_ConseFailureByHead_Retest", AnsiString("0"));
    iAutoClean[65]=ReadIniData(str1, "Configuration", "iAutoClean_ConseFailureCountByHead_Retest", AnsiString("10"));
    iAutoClean[66]=ReadIniData(str1, "Configuration", "bAutoClean_UseTray",     AnsiString("0"));
    iAutoClean[67]=ReadIniData(str1, "Configuration", "bAutoClean_UseNSKit",    AnsiString("0"));
    iAutoClean[68]=ReadIniData(str1, "Configuration", "iAutoCleanShuttle",      AnsiString("1"));                       //kevin 20120710

    //Sam 20210803 : FTP SetFile Change Log
    //==>
    for(int i=0; i<69; i++)                                                     //Steven 20240911 : 24 --> 69
    {
        sLog.sprintf("iAutoClean[%d]=%s", i, iAutoClean[i]);
        WriteFTPSetupFileChangeLog(sLog);
    }
    //<==
    //Sam 20210803 : FTP SetFile Change Log

    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");                //JerryYang 20190703 auto clean清潔次數備份
    for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
    {
        for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
        {
            str.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
            strCleanCnt[X][Y]=ReadIniData(szDir, "Configuration", str, AnsiString(0));
        }
    }

    return ret;
}
//---------------------------------------------------------------------------
int TfLotInfo::DoOverWriteSetupFile(AnsiString DataPath, AnsiString sDLFileName)
{
    bool bNeedCover;
    int ret=0;
    AnsiString str, str1, str2, sLog="";;
    AnsiString sConfigPath=AuthPath+"Security_new.def";

    //Sam 20210803 : FTP SetFile Change Log
    //==>
    sLog.sprintf("Restore [%s] ",sDLFileName);
    WriteFTPSetupFileChangeLog(sLog);
    //<==
    //Sam 20210803 : FTP SetFile Change Log
    //----------------------
    //不覆蓋就是必須要還原
    // 0 : 不覆蓋
    // 1 : 要覆蓋
    //----------------------

    //----------------------
    //把溫度Offset資料還原
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Temp Offset", true);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Temp Offset use local setting");            //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
        //for(int i=0; i<10; i++)
        for(int i=0; i<tcTotalCount; i++)                                       //ChungHung 20140804 add 修正 RMS 原本 只還原到前十個 uer Offset
        {
            str2.printf("CH%d", i+1);
            WriteIniData(str1, "User OffSet", str2, fTempUserOffset[i]);
            if(ATC_SYSTEM>eATC30 && ATC_SYSTEM!=eNonChamber)                    //Steven 20221209 : ATC offset要不要覆蓋
            {
                str2.sprintf("ATCTempOffset[%d]", i);
                WriteIniData(str1, "ATC", str2, fTempATCOffset[i]);
            }
        }
    }

    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Index Heat Mode", true);                                    //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式
    if(bNeedCover==false)
    {
        WriteFTPSetupFileChangeLog("Index Heat Mode use local setting");        //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
        WriteIniData(str1, "Index",   "Heating Mode",   iIndexHeatingMode);
    }

    //----------------------
    //把Contact高度還原
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact High", false);                                      //預設不覆蓋
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Contact High use local setting");           //Sam 20210803 : FTP SetFile Change Log
        if(CosFunction.bContactHeightSaveToContactIni)                          //Steven 20200616 : JSCC要求把Contact Height放到別的檔案
        {
            str1="D:\\HT9045\\system\\Contact.ini";
            WriteIniData(str1, sDLFileName, "Pick Up1",             fContactHeight[0]);
            WriteIniData(str1, sDLFileName, "Test Arm1",            fContactHeight[1]);
            WriteIniData(str1, sDLFileName, "Drop1",                fContactHeight[2]);
            WriteIniData(str1, sDLFileName, "Place1",               fContactHeight[3]);
            WriteIniData(str1, sDLFileName, "Pick Up2",             fContactHeight[4]);
            WriteIniData(str1, sDLFileName, "Test Arm2",            fContactHeight[5]);
            WriteIniData(str1, sDLFileName, "Drop2",                fContactHeight[6]);
            WriteIniData(str1, sDLFileName, "Place2",               fContactHeight[7]);
            WriteIniData(str1, sDLFileName, "ContactBackUp1",       fContactHeight[20]);                                //Steven 20190822 : download cover file include ContactBackUp and ShuttlePickBackUp
            WriteIniData(str1, sDLFileName, "ContactBackUp2",       fContactHeight[21]);
            WriteIniData(str1, sDLFileName, "ShuttlePickBackUp1",   fContactHeight[22]);
            WriteIniData(str1, sDLFileName, "ShuttlePickBackUp2",   fContactHeight[23]);
        }
        else
        {
            str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
            WriteIniData(str1, "Test Arm1", "Pick Up",              fContactHeight[0]);
            WriteIniData(str1, "Test Arm1", "Contact",              fContactHeight[1]);
            WriteIniData(str1, "Test Arm1", "Drop",                 fContactHeight[2]);
            WriteIniData(str1, "Test Arm1", "Place",                fContactHeight[3]);
            WriteIniData(str1, "Test Arm2", "Pick Up",              fContactHeight[4]);
            WriteIniData(str1, "Test Arm2", "Contact",              fContactHeight[5]);
            WriteIniData(str1, "Test Arm2", "Drop",                 fContactHeight[6]);
            WriteIniData(str1, "Test Arm2", "Place",                fContactHeight[7]);
            WriteIniData(str1, "Test Arm1", "ContactBackUp",        fContactHeight[20]);                                //Steven 20190822 : download cover file include ContactBackUp and ShuttlePickBackUp
            WriteIniData(str1, "Test Arm2", "ContactBackUp",        fContactHeight[21]);
            WriteIniData(str1, "Test Arm1", "ShuttlePickBackUp",    fContactHeight[22]);
            WriteIniData(str1, "Test Arm2", "ShuttlePickBackUp",    fContactHeight[23]);
        }
    }

    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact Force", true);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Contact High use local setting");           //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
        WriteIniData(str1, "Torque Control", "Pin Number",    fContactHeight[8]);
        WriteIniData(str1, "Torque Control", "Force Per Pin", fContactHeight[9]);
        WriteIniData(str1, "Torque Control", "Torque"       , fContactHeight[19]);
    }

    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact Mode", true);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Contact High use local setting");           //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
        WriteIniData(str1, "Wait Time", "Drop Wait",                    fContactHeight[10]);
        WriteIniData(str1, "Wait Time", "Drop Speed",                   fContactHeight[11]);
        WriteIniData(str1, "Mode", "Contact",                           fContactHeight[12]);
        WriteIniData(str1, "Mode", "Vacuum",                            fContactHeight[13]);
        WriteIniData(str1, "Mode", "Dummy Contact",                     fContactHeight[14]);
        WriteIniData(str1, "Mode", "Head Device Mode",                  fContactHeight[15]);
        WriteIniData(str1, "Mode", "Kit Diameter",                      fContactHeight[16]);
        WriteIniData(str1, "Mode", "Suck Shuttle Device After Tested",  fContactHeight[17]);
        WriteIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber",  fContactHeight[18]);

        WriteIniData(str1, "Mode", "Die Force Kit Diameter",            fContactHeight[24]);                            //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    }

    //----------------------
    //把HotPlate資料還原
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "HotPlate", false);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("HotPlate use local setting");               //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%sHotPlate.Data", DataPath);
        str2.sprintf("%s%s\\HotPlate.Data", DataPath, sDLFileName);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);
    }

    //jou 2012-12-14 system改採用DeleteFile
    str1.sprintf("%sHotPlate.Data", DataPath);                                  //刪除備份
    DeleteFile(str1.c_str());
    MySleep(100);

    //----------------------
    //把Ld/Uld速度還原
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Load Unload", false);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Load Unload use local setting");            //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%sUdUld.Data", DataPath);
        str2.sprintf("%s%s\\UdUld.Data", DataPath, sDLFileName);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);
    }

    //jou 2012-12-14 system改採用DeleteFile
    str1.sprintf("%sUdUld.Data", DataPath);                                     //刪除備份
    DeleteFile(str1.c_str());
    MySleep(100);

    //----------------------
    //把速度設定備份
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Speed Setting", true);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Load Unload use local setting");            //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%sArmCondition.Data", DataPath);
        str2.sprintf("%s%s\\ArmCondition.Data", DataPath, sDLFileName);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);
    }

    //jou 2012-12-14 system改採用DeleteFile
    str1.sprintf("%sArmCondition.Data", DataPath);                              //刪除備份
    DeleteFile(str1.c_str());
    MySleep(100);

    //----------------------
    //備份開啟單一Shuttle選項
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Shuttle Mode", false);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Shuttle Mode use local setting");           //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
        WriteIniData(str1, "Configuration", "Shuttle Mode", iShuttleMode[0]);
        WriteIniData(str1, "Configuration", "Shuttle1 Cancel", iShuttleMode[1]);
    }

    //----------------------
    //備份Auto Retest選項
    //----------------------
    if(CosFunction.bUseSCKART)                                                  //Steven 20190918 : ART設定下載不覆蓋
    {
        bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Auto Retest", true);
    }
    else
    {
        bNeedCover=true;
    }

    if(bNeedCover==false)
    {
        WriteFTPSetupFileChangeLog("Auto Retest use local setting");            //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\Tester.Data", DataPath, sDLFileName);
        WriteIniData(str1, "AutoRetest", "Enable ART",              bART[0]);
        WriteIniData(str1, "AutoRetest", "Run ART Without Cmd",     bART[1]);
    }

    if(CosFunction.bUseSCKART)                                                  //Steven 20191101 : ART RT count不覆蓋
    {
        bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "ART_RT_Count ", true);
    }
    else
    {
        bNeedCover=true;
    }

    if(bNeedCover==false)
    {
        WriteFTPSetupFileChangeLog("ART_RT_Count use local setting");           //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\Tester.Data", DataPath, sDLFileName);
        WriteIniData(str1, "AutoRetest", "Try Count",              iART);
        WriteIniData(str1, "AutoRetest", "Auto Socket Off",        bART[2]);
    }

    //----------------------
    //備份Auto Clean選項
    //----------------------                                                    //Steven 20161116 : ATC說要加上Auto Clean
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean", true);

    if(CUSTOMER_CODE==CC_SCC)                                                   //Steven 20190712 : 李國旗說要寫死不覆蓋
        bNeedCover=false;                                                       //JerryYang 20190919 : 曹沖說除了Height，其他的都需要正常download覆蓋本機參數

    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Auto Clean use local setting");             //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
        if(CUSTOMER_CODE==CC_SCC)                                               //JerryYang 20190919 : 曹沖說除了Height，其他的都需要正常download覆蓋本機參數
        {
            WriteIniData(str1, "Configuration", "iAutoClean_ContactShiftHeight",    iAutoClean[ 0]);
            WriteIniData(str1, "Configuration", "iAutoClean_ContactCleanHeight",    iAutoClean[ 1]);
            WriteIniData(str1, "Configuration", "iAutoClean_IndexPickOffset",       iAutoClean[ 2]);
            WriteIniData(str1, "Configuration", "iAutoClean_IndexReleaseOffset",    iAutoClean[ 3]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PickOffset",    iAutoClean[ 4]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PlaceOffset",   iAutoClean[ 5]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1XOffset",       iAutoClean[ 6]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1YOffset",       iAutoClean[ 7]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PickOffset",    iAutoClean[ 8]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PlaceOffset",   iAutoClean[ 9]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2XOffset",       iAutoClean[10]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2YOffset",       iAutoClean[11]);
            WriteIniData(str1, "Configuration", "HotplatlXOffset",                  iAutoClean[12]);
            WriteIniData(str1, "Configuration", "HotplatlYOffset",                  iAutoClean[13]);
            WriteIniData(str1, "Configuration", "HotplatlPickOffset",               iAutoClean[14]);
            WriteIniData(str1, "Configuration", "HotplatlPlaceOffset",              iAutoClean[15]);
            WriteIniData(str1, "Configuration", "HotplatlPitchOffset",              iAutoClean[16]);
            WriteIniData(str1, "Configuration", "ShuttlePitchOffset",               iAutoClean[17]);
        }
        else
        {
            WriteIniData(str1, "Configuration", "iAutoClean_ContactShiftHeight",                iAutoClean[ 0]);
            WriteIniData(str1, "Configuration", "iAutoClean_ContactCleanHeight",                iAutoClean[ 1]);
            WriteIniData(str1, "Configuration", "iAutoClean_IndexPickOffset",                   iAutoClean[ 2]);
            WriteIniData(str1, "Configuration", "iAutoClean_IndexReleaseOffset",                iAutoClean[ 3]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PickOffset",                iAutoClean[ 4]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1PlaceOffset",               iAutoClean[ 5]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1XOffset",                   iAutoClean[ 6]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle1YOffset",                   iAutoClean[ 7]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PickOffset",                iAutoClean[ 8]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2PlaceOffset",               iAutoClean[ 9]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2XOffset",                   iAutoClean[10]);
            WriteIniData(str1, "Configuration", "iAutoClean_Shuttle2YOffset",                   iAutoClean[11]);
            WriteIniData(str1, "Configuration", "HotplatlXOffset",                              iAutoClean[12]);
            WriteIniData(str1, "Configuration", "HotplatlYOffset",                              iAutoClean[13]);
            WriteIniData(str1, "Configuration", "HotplatlPickOffset",                           iAutoClean[14]);
            WriteIniData(str1, "Configuration", "HotplatlPlaceOffset",                          iAutoClean[15]);
            WriteIniData(str1, "Configuration", "HotplatlPitchOffset",                          iAutoClean[16]);
            WriteIniData(str1, "Configuration", "ShuttlePitchOffset",                           iAutoClean[17]);
            WriteIniData(str1, "Configuration", "iAutoClean_Function",                          iAutoClean[18]);
            WriteIniData(str1, "Configuration", "iAutoClean_Mode",                              iAutoClean[19]);
            WriteIniData(str1, "Configuration", "iAutoClean_Tray",                              iAutoClean[20]);
            WriteIniData(str1, "Configuration", "iAutoClean_AlarmCount",                        iAutoClean[21]);
            WriteIniData(str1, "Configuration", "iAutoClean_iPadThickness",                     iAutoClean[22]);
            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[0]",                     iAutoClean[23]);
            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[1]",                     iAutoClean[24]);
            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[2]",                     iAutoClean[25]);
            WriteIniData(str1, "Configuration", "iAutoClean_MotorSpeed[3]",                     iAutoClean[26]);
            WriteIniData(str1, "Configuration", "iAutoClean_ContactMode",                       iAutoClean[27]);
            WriteIniData(str1, "Configuration", "iAutoClean_DropHigh",                          iAutoClean[28]);
            WriteIniData(str1, "Configuration", "iAutoClean_ContactTime",                       iAutoClean[29]);
            WriteIniData(str1, "Configuration", "iAutoClean_ContactCount",                      iAutoClean[30]);
            WriteIniData(str1, "Configuration", "iAutoClean_DevicePinCount",                    iAutoClean[31]);
            WriteIniData(str1, "Configuration", "fAutoClean_ForcePerPin",                       iAutoClean[32]);
            WriteIniData(str1, "Configuration", "fAutoClean_DevicePinForceGf",                  iAutoClean[33]);
            WriteIniData(str1, "Configuration", "fAutoClean_AireForce",                         iAutoClean[34]);
            WriteIniData(str1, "Configuration", "dAutoClean_XPitch_Kit",                        iAutoClean[35]);
            WriteIniData(str1, "Configuration", "dAutoClean_YPitch_Kit",                        iAutoClean[36]);
            WriteIniData(str1, "Configuration", "dAutoClean_XStart_Kit",                        iAutoClean[37]);
            WriteIniData(str1, "Configuration", "dAutoClean_YStart_Kit",                        iAutoClean[38]);
            WriteIniData(str1, "Configuration", "iAutoClean_XDivision_Kit",                     iAutoClean[39]);
            WriteIniData(str1, "Configuration", "iAutoClean_YDivision_Kit",                     iAutoClean[40]);
            WriteIniData(str1, "Configuration", "dAutoClean_XPitch_Tray",                       iAutoClean[41]);
            WriteIniData(str1, "Configuration", "dAutoClean_YPitch_Tray",                       iAutoClean[42]);
            WriteIniData(str1, "Configuration", "dAutoClean_XStart_Tray",                       iAutoClean[43]);
            WriteIniData(str1, "Configuration", "dAutoClean_YStart_Tray",                       iAutoClean[44]);
            WriteIniData(str1, "Configuration", "iAutoClean_XDivision_Tray",                    iAutoClean[45]);
            WriteIniData(str1, "Configuration", "iAutoClean_YDivision_Tray",                    iAutoClean[46]);
            WriteIniData(str1, "Configuration", "cAutoClean_PackageTray",                       iAutoClean[47]);
            WriteIniData(str1, "Configuration", "iAutoClean_IntervalContact",                   iAutoClean[48]);
            WriteIniData(str1, "Configuration", "iAutoClean_DeveicePices",                      iAutoClean[49]);
            WriteIniData(str1, "Configuration", "iCleanIndexOtherArm",                          iAutoClean[50]);
            WriteIniData(str1, "Configuration", "iAutoClean_SelectArm",                         iAutoClean[51]);
            WriteIniData(str1, "Configuration", "bAutoClean_FailAlarmLowYield",                 iAutoClean[52]);
            WriteIniData(str1, "Configuration", "iAutoClean_LowYieldLimit",                     iAutoClean[53]);
            WriteIniData(str1, "Configuration", "iAutoClean_LowYieldCount",                     iAutoClean[54]);
            WriteIniData(str1, "Configuration", "bAutoClean_FailAlarmSiteYieldDifferent",       iAutoClean[55]);
            WriteIniData(str1, "Configuration", "iAutoClean_FailAlarmSiteYield",                iAutoClean[56]);
            WriteIniData(str1, "Configuration", "iAutoClean_FailAlarmSiteYieldDifferentCount",  iAutoClean[57]);
            WriteIniData(str1, "Configuration", "bAutoClean_ConseFailureBySocket_Normal",       iAutoClean[58]);
            WriteIniData(str1, "Configuration", "iAutoClean_ConseFailureCountBySocket_Normal",  iAutoClean[59]);
            WriteIniData(str1, "Configuration", "bAutoClean_ConseFailureBySocket_Retest",       iAutoClean[60]);
            WriteIniData(str1, "Configuration", "iAutoClean_ConseFailureCountBySocket_Retest",  iAutoClean[61]);
            WriteIniData(str1, "Configuration", "bAutoClean_ConseFailureByHead_Normal",         iAutoClean[62]);
            WriteIniData(str1, "Configuration", "iAutoClean_ConseFailureCountByHead_Normal",    iAutoClean[63]);
            WriteIniData(str1, "Configuration", "bAutoClean_ConseFailureByHead_Retest",         iAutoClean[64]);
            WriteIniData(str1, "Configuration", "iAutoClean_ConseFailureCountByHead_Retest",    iAutoClean[65]);
            WriteIniData(str1, "Configuration", "bAutoClean_UseTray",                           iAutoClean[66]);
            WriteIniData(str1, "Configuration", "bAutoClean_UseNSKit",                          iAutoClean[67]);
            WriteIniData(str1, "Configuration", "iAutoCleanShuttle",                            iAutoClean[68]);
        }
    }

    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Cleaning Count", bNeedCover);                               //KenHsieh 20230518 : Auto Clean count不覆蓋
    if(bNeedCover==false)                                                       //KenHsieh 20230518 : Auto Clean count不覆蓋
    {
        AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");            //JerryYang 20190703 auto clean清潔次數備份
        for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)
        {
            for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
            {
                str.sprintf("iAutoCleanPad_CountTime_%d_%d", Y, X);
                WriteIniData(szDir, "Configuration", str, strCleanCnt[X][Y]);
            }
        }
    }

    //----------------------
    //把TestMode還原
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Test Mode", false);
    if(CUSTOMER_CODE==CC_TERAPOWER)
        bNeedCover=false;                                                       //Sam 20230110 : 晶兆成建勳要強制

    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Auto Clean use local setting");             //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%sTestMode.Data", DataPath);
        str2.sprintf("%s%s\\TestMode.Data", DataPath, sDLFileName);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);
    }

    //jou 2012-12-14 system改採用DeleteFile
    str1.sprintf("%sTestMode.Data", DataPath);                                  //刪除備份
    DeleteFile(str1.c_str());
    MySleep(100);

    //----------------------
    //把Binasgn還原
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Binasgn", false);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("Binasgn use local setting");                //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%sBinasgn.Data", DataPath);
        str2.sprintf("%s%s\\Binasgn.Data", DataPath, sDLFileName);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);
    }

    //jou 2012-12-14 system改採用DeleteFile
    str1.sprintf("%sBinasgn.Data", DataPath);                                   //刪除備份
    DeleteFile(str1.c_str());
    MySleep(100);

    //----------------------
    //把Binasgn還原
    //----------------------
    bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "BinasgnOff", false);
    if(bNeedCover==false)                                                       //不覆蓋就要還原
    {
        WriteFTPSetupFileChangeLog("BinasgnOff use local setting");             //Sam 20210803 : FTP SetFile Change Log
        str1.sprintf("%sBinasgnOff.Data", DataPath);
        str2.sprintf("%s%s\\BinasgnOff.Data", DataPath, sDLFileName);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(100);
    }

    //jou 2012-12-14 system改採用DeleteFile
    str1.sprintf("%sBinasgnOff.Data", DataPath);                                //刪除備份
    DeleteFile(str1.c_str());
    MySleep(100);
    return ret;
}
//---------------------------------------------------------------------------
bool __fastcall TfLotInfo::DownloadFromServer(AnsiString sDLFileName, bool bFromFTP)
{
    int iCheckSumResult;
    bool bCheckHasAllFile=false;
    AnsiString sLog="";
    AnsiString FileName[8]={"ArmCondition.Data", "Binasgn.Data", "Contact.Data", "HandlerCondition.Data",
                            "HotPlate.Data", "Temperature.Data", "Tester.Data", "Tray.Data"};
    int iHeatingMode=0;                                                         //Sam 20210224 : 北興俊堯要求 NoChamber Head Only，Chamber Head Only;NoChamber Head+Socket，Chamber Head+Chamber

    SetCurrentDirectory(_T("D://"));
    //----------------------
    //檢查是不是有斷線
    //----------------------

    if(IniConfig.bEnableRms)                                                    // CosFunction.bFTPFunction==false)                 //Steven 20110211 : 使用FTP的不需要檢查
    {
        if(IniConfig.iN05_UpDLMethod==eByFTP)
        {
            //pass
        }
        else if(CUSTOMER_CODE==CC_SCC ||
                CUSTOMER_CODE==CC_SCK)                                          //ChungHung 20130621 add SCK RMS
        {
            if(DirectoryExists(IniConfig.sRmsDownPath)==false)
            {
                ShowErrorMessage("WAR1683", 0, MMSystem);                       //Connection Fail.
                return false;
            }
        }
        else if(IniConfig.bSPILFunction==false)                                 //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        {
            if(DirectoryExists(IniConfig.sRmsPath)==false)
            {
                ShowErrorMessage("WAR1683", 0, MMSystem);                       //Connection Fail.
                return false;
            }
        }
    }

    AnsiString str, str1, str2, str3;
    AnsiString sConfigPath=AuthPath+"Security_new.def";
    AnsiString sNewFilePath, sDLOffsetPath, sCurrOffsetPath;
    AnsiString SPath[2]={IncludeTrailingPathDelimiter(DataPath), IncludeTrailingPathDelimiter(OffsetPath)};

    int ret=0, ret2=0;
    static int iCount=0;
    bool bHasSetUpFile=false, bHasSetUpFile2=false;
    bool bHasATCFileTransfer=false;                                             //Eastsun 20260522 整合: ATC Recipe File Transfer (Dual-Socket) flag
    bool bFilecomplete=false;                                                   //Eastsun 20260522 整合
    if(CosFunction.bUseATCFileTransfer==true &&                                 //Eastsun 20260522 整合
       ATC_SYSTEM==eNewATCSystem )
    {
        bHasATCFileTransfer=true;
    }
    AnsiString sDLFileName1=sDLFileName.SubString(1, sDLFileName.Length()-4);   //找沒有_NET的
    HINSTANCE hInstance;

    sDLOffsetPath=fOffSet->GetOffsetPath(sDLFileName);
    sCurrOffsetPath=fOffSet->GetOffsetPath();

    if(sDLFileName.AnsiPos(AnsiString("_NET"))==0)
    {
        sDLFileName1=sDLFileName;
    }

    //----------------------
    //首先判斷工作檔有沒有
    //----------------------
    if(CosFunction.bFTPDownloadAlwaysCover)                                     //JerryYang 20190523 KYEC download工作檔時不要還原本機的參數
    {
        if(!DirectoryExists(SPath[0]+sDLFileName))
        {
            MyForceDirectories(SPath[0]+sDLFileName);
            bHasSetUpFile=false;
        }
        else
        {
            bHasSetUpFile=true;
        }
    }
    else
    {
        if(!DirectoryExists(SPath[0]+sDLFileName1))                             //Ifor 20190520 : Fix  sDLFileName1 -> sDLFileName 避免FTP DownLoad 資料被還原
        {                                                                       //Steven 20200514 : 要放在sDLFileName前面, 避免沒有 _NET的版本誤判
            MyForceDirectories(SPath[0]+sDLFileName1);                          //Ifor 20190520 : Fix  sDLFileName1 -> sDLFileName 避免FTP DownLoad 資料被還原
            bHasSetUpFile=false;
        }
        else
        {
            bHasSetUpFile=true;
        }

        MyForceDirectories(SPath[0]+sDLFileName);                               //Ifor 20190520 : Fix  sDLFileName -> sDLFileName1 避免FTP DownLoad 資料被還原
    }

    if(bHasSetUpFile==false)                                                    //沒有的話,就先建立新的資料夾
    {
        MyForceDirectories(sDLOffsetPath);
        MyForceDirectories(SPath[0]+sDLFileName);
        fMain->cbSetupFileName->Items->Add(sDLFileName);
    }

    if(IniConfig.bE45_AllSetupFileUseOneFile==false &&
       CosFunction.bUseLocalRecipeOffset==false)                                //Steven 20190109 : 修正統一Offset時,不下載Offset
    {
        if(CosFunction.bFTPFunction)                                            //jou 2012-12-22 下載 Offset 檔案 start
        {
            str3=OffsetPath+sDLFileName+".Offset";
            if(FileExists(str3))
            {
                str1="e \""+str3+"\" -o\""+OffsetPath+sDLFileName+"\\\" -y";
                hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str1.c_str(), NULL, SW_HIDE);
                MySleep(200);
                DeleteFile(str3.c_str());
            }
        }
    }
    else
    {
        str1.sprintf("%s%s", OffsetPath, sDLFileName);
        MyForceDirectories(str1);
    }

    //----------------------
    //檢查Offset檔
    //----------------------
    str1.sprintf("%s\\Position Offset.Data", sDLOffsetPath);
    if(!FileExists(str1))                                                       //如果沒有就從當下的工作檔複製過來
    {
        str2.sprintf("%s\\Position Offset.Data", sCurrOffsetPath);
        CopyFile(str2.c_str(), str1.c_str(), false);
    }

    str1.sprintf("%s\\Position Offset Hot.Data", sDLOffsetPath);
    if(!FileExists(str1))
    {
        str2.sprintf("%s\\Position Offset Hot.Data",  sCurrOffsetPath);
        CopyFile(str2.c_str(), str1.c_str(), false);
    }

    if(CosFunction.bFTPFunction==false ||                                       //Steven 20110211 : 使用FTP的不需要下載
       IniConfig.bEnableRms==true)                                              //Steven 20190109 : 修正FTP跟RMS同時打開的問題
    {
        //----------------------
        //開始下載檔案
        //----------------------
        AnsiString sSourcesFilePath="";
        AnsiString sTargetFilePath=DataPath;
        AnsiString sFileName=sDLFileName;
        do
        {
            if(FileExists("d:\\HT9045\\7z.exe")==false)
            {
                CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe", false);
            }

            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
            {
                ret2=0;
            }
            else
            {
                if(CUSTOMER_CODE==CC_SCC ||
                   CUSTOMER_CODE==CC_SCK )
                {
                    sSourcesFilePath=IniConfig.sRmsDownPath;
                }
                else
                {
                    sSourcesFilePath=IniConfig.sRmsPath;
                }

                if(IniConfig.iN05_UpDLMethod==eByNetwork)
                {
                    if(RMSDownloadByNetwork(sSourcesFilePath,sTargetFilePath,sDLFileName))
                    {
                        ret2=0;
                    }
                    else
                    {
                        ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, sDLFileName);                     //下載 %s.zip 失敗，重試??
                        if(ret2==K_SKIP)
                            return false;
                    }
                }
                else if(IniConfig.iN05_UpDLMethod==eByFTP)
                {
                    if(RMSDownloadByFTP(sSourcesFilePath,sTargetFilePath,sDLFileName))
                    {
                        ret2=0;
                    }
                    else
                    {
                        ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, sDLFileName);                     //下載 %s.zip 失敗，重試??
                        if(ret2==K_SKIP)
                            return false;
                    }
                }

                if(CosFunction.bUseFTPDownLoadATCRecipe==true   &&              //Ifor 20210406 add: RMS Up/Down Load ATC Recipe
                   ATC_SYSTEM==eNewATCSystem                    &&              //JerryYang 20190906 ATC工作檔經由handler上傳/下載
                   Temperature.bATCActiveCooling==true          &&              //Ifor 20191115 : add FTP DownLoad ATC Recipe
                   CosFunction.bHiSiliconFunction==false        )               //Ifor 20200716 add:Hisi版不下載ATC工作檔
                {
                    if(DirectoryExists(sATCPath))
                    {
                        sDLFileName=sDLFileName+"ATC_Recipe";
                        if(IniConfig.iN05_UpDLMethod==eByNetwork)
                        {
                            RMSDownloadByNetwork(sSourcesFilePath,sTargetFilePath,sDLFileName);
                        }
                        else if(IniConfig.iN05_UpDLMethod==eByFTP)
                        {
                            RMSDownloadByFTP(sSourcesFilePath,sTargetFilePath,sDLFileName);
                        }

                        if(FileExists(DataPath+sDLFileName+"ATC_Recipe"+".zip"))
                        {
                            str1.sprintf("d:\\HT9045\\7z.exe e \"%s%s.zip\" -o\"%s\\\" -y", DataPath, sDLFileName+"ATC_Recipe", sATCPath);
                            ret=system(str1.c_str());
                        }
                        else
                        {
                            ShowMyMessage("Download ATC recipe fail", "ATC recipe not exist in server");
                            return false;
                        }
                        MySleep(100);
                        str.sprintf("del \"%s%s.zip\"", DataPath, sDLFileName+"ATC_Recipe");                            //最後要把壓縮檔給刪除。
                        system(str.c_str());
                    }
                    else
                    {
                        ShowMyMessage("Download ATC recipe fail, check below path", sATCPath);
                        return false;
                    }
                }
            }
        }
        while(ret2==1);
    }

    //----------------------
    //把要還原的資料先備份
    //----------------------
    if(bHasSetUpFile)                                                           //前提是該資料夾已經有資料了
    {
        DoBackupSetupFile(DataPath, sDLFileName1);                              //Steven 20191101 : 整合下載工作檔的覆蓋方式
    }
    else                                                                        //Sam 20210803 : FTP SetFile Change Log
    {
        sLog.sprintf("No backup [%s] ", sDLFileName1);
        WriteFTPSetupFileChangeLog(sLog);
    }

    //----------------------
    //解壓縮檔案並覆蓋到原本的資料夾
    //----------------------
    if(FileExists(DataPath+sDLFileName+".zip"))                                 //Steven 20110603
    {
        sNewFilePath=DataPath+sDLFileName+"\\";
        FileListBox2->Directory=sNewFilePath;                                   //Steven 20140609

        hInstance=NULL;
        if(IniConfig.FtpUseSystemCallToUnZip)                                   //Steven 20140609
        {
            str1.sprintf("d:\\HT9045\\7z.exe e \"%s%s.zip\" -o\"%s%s\\\" -y", DataPath, sDLFileName, DataPath, sDLFileName);
            ret=system(str1.c_str());
            if(CosFunction.bUseFTPDownLoadATCRecipe==true   &&                  //JerryYang 20190906 ATC工作檔經由handler上傳/下載
               ATC_SYSTEM==eNewATCSystem                    &&                  //Ifor 20191115 : add FTP DownLoad ATC Recipe
               Temperature.bATCActiveCooling==true          &&
               CosFunction.bHiSiliconFunction==false        )                   //Ifor 20200716 add:Hisi版不下載ATC工作檔
            {
                if(DirectoryExists(sATCPath))
                {
                    if(FileExists(DataPath+sDLFileName+"ATC_Recipe"+".zip"))
                    {
                        str1.sprintf("d:\\HT9045\\7z.exe e \"%s%s.zip\" -o\"%s\\\" -y", DataPath, sDLFileName+"ATC_Recipe", sATCPath);
                        ret=system(str1.c_str());
                    }
                    else
                    {
                        ShowMyMessage("Download ATC recipe fail", "ATC recipe not exist in server");
                        return false;
                    }
                }
                else
                {
                    ShowMyMessage("Download ATC recipe fail, check below path", sATCPath);
                    return false;
                }
            }

            if(bHasATCFileTransfer==true)                                       //Eastsun 20260522 整合
            {
                if(DirectoryExists(asATCFileTransferPath))
                {
                    if(FileExists(DataPath+sDLFileName+"\\"+sDLFileName1+".dat"))
                    {
//                        str1.sprintf("d:\\HT9045\\7z.exe e \"%s%s.dat\" -o\"%s\\\" -y", DataPath, sDLFileName+"ATC_Recipe", asATCFileTransferPath);
//                        ret=system(str1.c_str());
                        str1=DataPath+sDLFileName+"\\"+sDLFileName1+".dat";
                        str2=asATCFileTransferPath+sDLFileName1+".dat";

                        CopyFile(str1.c_str(), str2.c_str() , FALSE);
                            bFilecomplete=true;
                    }
                    else
                    {
                        RecordProcess("Download ATC recipe fail", "ATC recipe not exist in server");
//                        ShowMyMessage("Download ATC recipe fail", "ATC recipe not exist in server");
                        return false;
                    }
                }
                else
                {
                    RecordProcess("Download ATC recipe fail, check below path", asATCFileTransferPath);
//                    ShowMyMessage("Download ATC recipe fail, check below path", asATCFileTransferPath);
                    return false;
                }
            }
        }
        else
        {
            str1="e \""+DataPath+sDLFileName+".zip\" -o\""+DataPath+sDLFileName+"\\\" -y";
            hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str1.c_str(), NULL, SW_HIDE);                    //jou 2012-12-14 system改採用ShellExecute
        }
    }
    else
    {
        ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, sDLFileName);  //下載 %s.zip 失敗
        return false;
    }

    str1.sprintf("%s%s\\%s", DataPath, sDLFileName, sDLFileName);
    if(DirectoryExists(str1))
        rmdir(str1.c_str());

    iCount=0;
    do
    {
        FileListBox2->Update();
        iCount++;
        MySleep(500);                                                           //Steven 20140609 : 1000 --> 100
        if(iCount>100)
        {
            ret=-1;
            break;
        }
    }while(FileListBox2->Items->Count<8);                                       //Steven 20140609 : 確認下載解壓縮後的檔案最少8個

    MySleep(500);                                                               //Steven 20140609 : 1000 --> 100

    iCheckSumResult=CompareMD5ByFolder(DataPath+sDLFileName);                   //Steven 20170927 (wei) : 比對工作檔的檢查碼是否正確
    if(IniConfig.bN20_CheckMD5 && iCheckSumResult==0)
    {
        ShowErrorMessage("WAR16118", 0, MMSystem, 0, sDLFileName);              //MD5 check fail!
        return false;
    }

    bCheckHasAllFile=true;
    for(int i=0; i<8; i++)
    {
        str1.sprintf("%s%s\\%s", DataPath, sDLFileName, FileName[i]);
        if(FileExists(str1)==false)
        {
            bCheckHasAllFile=false;
        }
    }

    if(bCheckHasAllFile &&
       ((IniConfig.FtpUseSystemCallToUnZip && ret==0) ||
        int(hInstance)>32))
    {
        //----------------------
        //把壓縮檔砍掉
        //----------------------
        str1.sprintf("%s%s.zip", DataPath, sDLFileName);
        DeleteFile(str1.c_str());                                               //jou 2012-12-14 system改採用DeleteFile
        MySleep(100);

        if(CUSTOMER_CODE==CC_TERAPOWER)                                         //Sam 20191210 : 修正下載檔案被還原的問題) //如果不是新的工作檔就要進行資料還原
        {
            bHasSetUpFile2=bHasSetUpFile;
            if(IniConfig.bEnableRms==true ||
               IniConfig.bEnableErms==true)
            {
            }
            else
            {
                bHasSetUpFile=false;
            }
        }

        if(bHasSetUpFile)                                                       //如果不是新的工作檔就要進行資料還原
        {
            DoOverWriteSetupFile(DataPath, sDLFileName);                        //Steven 20191101 : 整合下載工作檔的覆蓋方式
        }
        else
        {
            if(CosFunction.bContactHeightSaveToContactIni)                      //Steven 20200616 : JSCC要求把Contact Height放到別的檔案
            {
                str1="D:\\HT9045\\system\\Contact.ini";
                if(FileExists(str1)==false || CheckIniData(str1, sDLFileName, "Test Arm1")==false)
                {
                    WriteIniData(str1, sDLFileName, "Pick Up1",             0.0);
                    WriteIniData(str1, sDLFileName, "Test Arm1",            0.0);
                    WriteIniData(str1, sDLFileName, "Drop1",                0.0);
                    WriteIniData(str1, sDLFileName, "Place1",               0.0);
                    WriteIniData(str1, sDLFileName, "Pick Up2",             0.0);
                    WriteIniData(str1, sDLFileName, "Test Arm2",            0.0);
                    WriteIniData(str1, sDLFileName, "Drop2",                0.0);
                    WriteIniData(str1, sDLFileName, "Place2",               0.0);
                    WriteIniData(str1, sDLFileName, "ContactBackUp1",       0.0);
                    WriteIniData(str1, sDLFileName, "ContactBackUp2",       0.0);
                    WriteIniData(str1, sDLFileName, "ShuttlePickBackUp1",   0.0);
                    WriteIniData(str1, sDLFileName, "ShuttlePickBackUp2",   0.0);
                }
            }
        }

        if(CosFunction.bSmartAutoClean && TestIF.bACSmart)                      //Sam 20240726 : AI Clean
        {
            str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
            int iACInterval=ReadIniData(str1, "Configuration",  "iAutoClean_IntervalContact",     20);
            AnsiString sLog="";
            sLog.sprintf("AI clean load default interval : %d", iACInterval);
            NewRecordProcess("", sLog, "");
            if(bHasSetUpFile2)
            {
                sLog.sprintf("AI clean load previous interval : %d", iAutoClean[24]);
                NewRecordProcess("", sLog, "");
                WriteIniData(str1, "Configuration", "iAutoClean_IntervalContact",     iAutoClean[24]);
            }
        }

        if(CUSTOMER_CODE==CC_KYEC_LEE ||
           CUSTOMER_CODE==CC_KYEC_XILINX)                                       //Steven 20140826 : KYEC要Auto Clean強制ON
        {
            if(CosFunction.bAutoRetestGPIBmode==false)                          //jou 2015-10-02 Auto Retest GPIB mode
            {
                str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
                if(TestIF_File.iAutoClean_Mode==0)                              //Ifor 20180823 :Add Auto Clean 功能選項無開啟就不使用
                {
                    WriteIniData(str1, "Configuration", "iAutoClean_Function", 0);
                }
                else
                {
                    WriteIniData(str1, "Configuration", "iAutoClean_Function", 1);
                }
            }
        }

        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                    //Sam 20210224 : 北興俊堯要求 NoChamber Head Only，Chamber Head Only;NoChamber Head+Socket，Chamber Head+Chamber
        {
            str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
            iHeatingMode=ReadIniData(str1, "Index", "Heating Mode", 0);
            if(iHeatingMode!=0)
            {
                if(ATC_SYSTEM>eATC60)                                           //No Chamber
                {
                    if(iHeatingMode==1)
                        WriteIniData(str1, "Index", "Heating Mode", 1);
                    else
                        WriteIniData(str1, "Index", "Heating Mode", 4);
                }
                else                                                            //Chamber
                {
                    /*
                    3 Chamber+Socket
                    1 Chamber
                    2 Chamber+Head
                    0 Head
                    */
                    if(iHeatingMode==4)
                        WriteIniData(str1, "Index", "Heating Mode", 2);
                    //else
                        //WriteIniData(str1, "Index", "Heating Mode", 4);
                }
            }
            str1.sprintf("%s%s\\ArmCondition.Data", DataPath, sDLFileName);
            WriteIniData(str1, "Index Arm", "Counter Air ON Time", 0.1);        //Sam 20210324 : 北興俊堯要求直接改為0.1
        }

        if(fFTPClient->bControlBySECSGEM==false &&                              //ChungHung 20150515 add Control by SECSGEM
           fFTPClient->bControlByGPIB==false)                                   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        {
            if(CUSTOMER_CODE==CC_GIGAS)                                         //Isaac 20200723 : 全智要求不要顯示，有log就好
            {
                str1="UnZip "+sDLFileName+".zip OK.";
                RecordProcess(str1);                                            //UnZip %s.zip OK.
            }
            else
            {
                ShowErrorMessage("MES1687", 0, MMSystem, 0, sDLFileName);       //UnZip %s.zip OK.
            }
        }

        if(bHasATCFileTransfer==true && bFilecomplete==true)                    //Eastsun 20260522 整合
        {
            if(bStartATCRun && ATC_InterfaceForm->IsConnect())
            {
                RecordProcess("Download ATC PID FileTransfer Start!!");
                fFTPClient->memoFTP->Lines->Add("FTP Download ATC PID FileTransfer Start!!");
                iATC_RecipeFileTransfer=1;                                          //Ifor 20251229 add: ATC Rrcipe 傳送與接收 0:無需傳送 1:需要傳送 2: 傳送中 6:檔案接收中
            }
            else
            {
                RecordProcess("ATC System Connect Error, Download PID FileTransfer Fail!!");
                fFTPClient->memoFTP->Lines->Add("ATC System Connect Error, Download PID FileTransfer Fail!!");
            }
        }
        return true;
    }
    else
    {
        if(fFTPClient->bControlBySECSGEM==false &&                              //ChungHung 20150515 add Control by SECSGEM
           fFTPClient->bControlByGPIB==false)                                   //KaiChen 20181129 ：Add FTP Downlaod Setup File by GPIB Command
        {
            ShowErrorMessage("WAR1686", 0, MMSystem, 0, sDLFileName);           //UnZip %s.zip Fail.
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfLotInfo::DownloadFromERMS(AnsiString sDLFileName)             //Steven 20160711 : 使用進階版RMS
{                                                                               //Steven 20161104 : ERMS改用壓縮檔
    bool bCheckHasAllFile=false;
    AnsiString FileName[8]={"ArmCondition.Data", "Binasgn.Data", "Contact.Data", "HandlerCondition.Data",
                            "HotPlate.Data", "Temperature.Data", "Tester.Data", "Tray.Data"};

//    bool bNeedCover=false;
    SetCurrentDirectory(_T("D://"));
    //----------------------
    //檢查是不是有斷線
    //----------------------
    AnsiString str1, str2, str3;
    AnsiString sConfigPath=AuthPath+"Security_new.def";                         //IniConfig.sErmsPath+"\\SP.ini";    //Steven 20170621 (wei) : Add for R160624-ATK-H9-01 ATK Auto Recipe Downloading Function.
    AnsiString sNewFilePath;
    AnsiString SPath[2]={IncludeTrailingPathDelimiter(DataPath), IncludeTrailingPathDelimiter(OffsetPath)};

    int ret=0;
    static int iCount=0;
    bool bHasSetUpFile=false;
    HINSTANCE hInstance;

    //----------------------
    //首先判斷工作檔有沒有
    //----------------------
    if(!DirectoryExists(SPath[0]+sDLFileName))
    {
        MyForceDirectories(SPath[0]+sDLFileName);
        bHasSetUpFile=false;
    }
    else
    {
        bHasSetUpFile=true;
    }

    if(bHasSetUpFile==false)                                                    //沒有的話,就先建立新的資料夾
    {
        for(int i=1; i<2; i++)
        {
            MyForceDirectories(SPath[i]+sDLFileName);
        }
        fMain->cbSetupFileName->Items->Add(sDLFileName);
    }

    //----------------------
    //把要還原的資料先備份
    //----------------------
    if(bHasSetUpFile)                                                           //前提是該資料夾已經有資料了
    {
        DoBackupSetupFile(DataPath, sDLFileName);                               //Steven 20191101 : 整合下載工作檔的覆蓋方式
    }

    //----------------------
    //解壓縮檔案並覆蓋到原本的資料夾
    //----------------------
    if(FileExists(IniConfig.sErmsPath+"\\"+sDLFileName+".zip"))                 //Steven 20110603
    {
//        sNewFilePath=IniConfig.sErmsPath+"\\"+sDLFileName+"\\";
//        FileListBox2->Directory=sNewFilePath;       //Steven 20140609

        hInstance=NULL;
       /* if(IniConfig.FtpUseSystemCallToUnZip)                                 //Steven 20140609
        {
            str1="d:\\HT9045\\7z.exe e \""+IniConfig.sErmsPath+"\\"+sDLFileName+"\" -o\""+DataPath+sDLFileName+"\\\" -y";
            ret=system(str1.c_str());
        }
        else  */
        {
            //jou 2012-12-14 system改採用ShellExecute
            str1="e \""+IniConfig.sErmsPath+"\\"+sDLFileName+".zip"+"\" -o\""+DataPath+sDLFileName+"\\\" -y";
            hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe", str1.c_str(), NULL, SW_HIDE);
        }
    }
    else
    {
        ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, sDLFileName+".zip");                                   //下載 %s.zip 失敗
        return false;
    }

    iCount=0;
 /*  do
    {
        FileListBox2->Update();
        iCount++;
        MySleep(500);                                                           //Steven 20140609 : 1000 --> 100
        if(iCount>100)
        {
            ret=-1;
            break;
        }
    }while(FileListBox2->Items->Count<8);                                       //Steven 20140609 : 確認下載解壓縮後的檔案最少8個  */

    MySleep(500);                                                               //Steven 20140609 : 1000 --> 100
    bCheckHasAllFile=true;
    for(int i=0; i<8; i++)
    {
        str1.sprintf("%s%s\\%s", DataPath, sDLFileName, FileName[i]);
        if(FileExists(str1)==false)
        {
            bCheckHasAllFile=false;
        }
    }

    if(bCheckHasAllFile &&
       ((IniConfig.FtpUseSystemCallToUnZip && ret==0) || int(hInstance)>32))
    {
        //----------------------
        //把壓縮檔砍掉
        //----------------------
        str1.sprintf("%s\\%s.zip", IniConfig.sErmsPath, sDLFileName);
        DeleteFile(str1.c_str());
        MySleep(100);

        if(bHasSetUpFile)                                                       //如果不是新的工作檔就要進行資料還原
        {
            DoOverWriteSetupFile(DataPath, sDLFileName);                        //Steven 20191101 : 整合下載工作檔的覆蓋方式
        }
        else
        {
            if(CosFunction.bContactHeightSaveToContactIni)                      //Steven 20200616 : JSCC要求把Contact Height放到別的檔案
            {
                str1="D:\\HT9045\\system\\Contact.ini";
                if(FileExists(str1)==false || CheckIniData(str1, sDLFileName, "Test Arm1")==false)
                {
                    WriteIniData(str1, sDLFileName, "Pick Up1",             0.0);
                    WriteIniData(str1, sDLFileName, "Test Arm1",            0.0);
                    WriteIniData(str1, sDLFileName, "Drop1",                0.0);
                    WriteIniData(str1, sDLFileName, "Place1",               0.0);
                    WriteIniData(str1, sDLFileName, "Pick Up2",             0.0);
                    WriteIniData(str1, sDLFileName, "Test Arm2",            0.0);
                    WriteIniData(str1, sDLFileName, "Drop2",                0.0);
                    WriteIniData(str1, sDLFileName, "Place2",               0.0);
                    WriteIniData(str1, sDLFileName, "ContactBackUp1",       0.0);
                    WriteIniData(str1, sDLFileName, "ContactBackUp2",       0.0);
                    WriteIniData(str1, sDLFileName, "ShuttlePickBackUp1",   0.0);
                    WriteIniData(str1, sDLFileName, "ShuttlePickBackUp2",   0.0);
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::UploadToServer()
{
    AnsiString sULFileName=fMain->cbSetupFileName->Text;
    UploadToServer(sULFileName, true);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::UploadToServer(AnsiString sULFileName, bool bSuccessAlarm)
{
    SetCurrentDirectory(_T("D://"));
    AnsiString str1, str2;
    int ret=0, ret2=K_RETRY;
    HINSTANCE hInstance;
    AnsiString sSourcesFilePath=DataPath;
    AnsiString sTargetFilePath=IniConfig.sRmsPath;
    if(CUSTOMER_CODE==CC_ChipMos_ZHUBEI)
    {
        if(IniConfig.iN05_UpDLMethod==eByFTP)
        {
            sTargetFilePath=FileInfo().PathCombin(sTargetFilePath, IniConfig.SocketHandlerID);
        }
    }

    AnsiString sFileName=sULFileName;
    do
    {
        //先刪除舊的壓縮檔
        //jou 2012-12-14 system改採用DeleteFile
        str1.sprintf("%s%s.zip", DataPath, sULFileName);
        DeleteFile(str1.c_str());
        MySleep(50);

        //把7Zip準備好
        if(FileExists("d:\\HT9045\\7z.exe")==false)
        {
            CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe" , false);                                 //Ifor 20170321 FALSE => false
        }

        //把工作檔壓縮
        //jou 2012-12-14 system改採用ShellExecute
        str1.sprintf("a -tzip \"%s%s.zip\" \"%s%s\\*.*\"", DataPath, sULFileName, DataPath, sULFileName);
        hInstance=ShellExecute(this, "open", "d:\\HT9045\\7z.exe",str1.c_str(),NULL,SW_HIDE);
        MySleep(500);                                                           //jou 2016-01-06 100->500 Unload 上傳失敗修正
        sFileName=sULFileName+".zip";
        if(IniConfig.iN05_UpDLMethod==eByNetwork)
        {
            if(RMSUploadByNetwork(sSourcesFilePath,sTargetFilePath,sFileName))
            {
                ret=1;
            }
        }
        else if(IniConfig.iN05_UpDLMethod==eByFTP)
        {
            if(RMSUploadByFTP(sSourcesFilePath,sTargetFilePath,sFileName))
            {
                ret=1;
            }
        }
        //Ifor 20210406 add: RMS Up/Down Load ATC Recipe
        //==>
        if(CosFunction.bUseFTPDownLoadATCRecipe==true   &&
           ATC_SYSTEM==eNewATCSystem                    &&
           Temperature.bATCActiveCooling==true          &&
           CosFunction.bHiSiliconFunction==false        )                       //JerryYang 20190906 ATC工作檔經由handler上傳/下載  //Ifor 20191115 : add FTP DownLoad ATC Recipe    //Ifor 20200716 add:Hisi版不下載ATC工作檔
        {
            if(DirectoryExists(sATCPath))
            {
                str1.sprintf("a -tzip \"%s%s.zip\" \"%s\\*%s*\"", DataPath, sULFileName+"ATC_Recipe", sATCPath, sULFileName+"_");
                hInstance=ShellExecute(this,"open","d:\\HT9045\\7z.exe",str1.c_str(),NULL,SW_HIDE);
                MySleep(500);                                                   //jou 2016-01-06 100->500 Unload 上傳失敗修正

                sFileName=sULFileName+"ATC_Recipe.zip";
                if(IniConfig.iN05_UpDLMethod==eByNetwork)
                {
                    RMSUploadByNetwork(sSourcesFilePath,sTargetFilePath,sFileName);
                }
                else if(IniConfig.iN05_UpDLMethod==eByFTP)
                {
                    RMSUploadByFTP(sSourcesFilePath,sTargetFilePath,sFileName);
                }
                str1.sprintf("del \"%s%s.zip\"", DataPath, sULFileName);        //最後要把壓縮檔給刪除。
                system(str1.c_str());
            }
            else
            {
                ShowMyMessage("Upload ATC recipe fail, check below path", sATCPath);
            }
        }
        //<==
        //Ifor 20210406 add: RMS Up/Down Load ATC Recipe

        if(int(hInstance)<=32 || ret==0)
        {
            ret2=ShowErrorMessage("WAR1685", K_RETRY|K_SKIP, MMSystem, 0, sFileName);                                   //Upload %s.zip Fail, Retry??
        }
        else
        {
            if(bSuccessAlarm)
            {
                ret2=ShowErrorMessage("MES1689", K_SKIP, MMSystem, 0, sFileName);                                       //Upload %s.zip OK
            }
            else
            {
                ret2=K_SKIP;
            }
        }
    }
    while(ret2==K_RETRY);

    //jou 2012-12-14 system改採用DeleteFile
    str1.sprintf("%s%s.zip", DataPath, sULFileName);                            //最後要把壓縮檔給刪除。
    DeleteFile(str1.c_str());
    MySleep(50);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edTempKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    int iPos;                                                                   //因應安靠的BarCode項賽

    iPos=(edTemp->Text.UpperCase()).AnsiPos("/K");
    if(iPos!=0)
    {
        edTemp->Text=edTemp->Text.SubString(1, edTemp->Text.Length()-2);
        edTemp->Text=edTemp->Text+"+";
        edTemp->SelStart=edTemp->Text.Length();
    }

    iPos=(edTemp->Text.UpperCase()).AnsiPos("/O");
    if(iPos!=0)
    {
        edTemp->Text=edTemp->Text.SubString(1, edTemp->Text.Length()-2);
        edTemp->Text=edTemp->Text+"/";
        edTemp->SelStart=edTemp->Text.Length();
    }

    iPos=edTemp->Text.AnsiPos("/0");
    if(iPos!=0)
    {
        edTemp->Text=edTemp->Text.SubString(1, edTemp->Text.Length()-2);
        edTemp->Text=edTemp->Text+"/";
        edTemp->SelStart=edTemp->Text.Length();
    }

    if(bLotFirstKeyIn==false)                                                   //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bLotFirstKeyIn=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnFtpServerClick(TObject *Sender)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;

    if(CUSTOMER_CODE==CC_JSCC_OS)                                               //RogerYang 20260127 : Add For JSCC_OS download by Device list
    {
        if(edtDevice->Text=="")
        {
            ShowMyMessage("Please Input Device ID First!");
            return;
        }
        else
        {
            AnsiString sDeviceCorrPath=DataPath+"DeviceCorrespond.ini";
            AnsiString str1, str2;
            if(FileExists(sDeviceCorrPath))
            {
                sJSCCOSFileName=ReadIniData(sDeviceCorrPath, edtDevice->Text, "Program",   AnsiString(""));
                if(sJSCCOSFileName=="")
                {
                    str1.sprintf("Get Recipe name failed by %s! Please maintain the DeviceCorrespond.ini first!", edtDevice->Text);
                    str2.sprintf("Device ID %s 找不到對應的程序，請維護!", edtDevice->Text);
                    ShowMyMessage(str1, str2);
                    WriteIniData(sDeviceCorrPath, edtDevice->Text, "Program", AnsiString(""));
                    return;
                }
            }
            else
            {
                int ret=ShowMyMessageBox_YES_NO("DeviceCorrespond.ini file not exist! Making new empty file?",
                    "DeviceCorrespond.ini 檔案不存在，是否新增空資料?");
                if(ret==1)
                {
                    WriteIniData(sDeviceCorrPath, edtDevice->Text, "Program", AnsiString(""));
                }
                return;
            }
        }
    }

    if(IniConfig.bEnableFTP==false && CUSTOMER_CODE==CC_TSMC_TAINAN)            //ChungHung 20150413 add for TSMC   //ChungHung 20150415 add for TSMC
    {
        Ptr->Enabled=true;
        return;
    }
    AnsiString TargetPath,SourcePtah,str;

    Ptr->Enabled=false;
    if(Ptr->Tag==0)
    {
        if(IniConfig.iServerEnable>AccessLevel)
        {
            Ptr->Enabled=true;
            return;
        }
    }
    else if(Ptr->Tag==1)
    {
        if(IniConfig.iHDEnable>AccessLevel)
        {
            Ptr->Enabled=true;
            return;
        }
    }
    else if(Ptr->Tag==2)
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        {
            return;
        }
    }
    else if(Ptr->Tag==3)                                                        //ChungHung 20150413 add for TSMC
    {
        if(CUSTOMER_CODE==CC_TSMC_TAINAN && SystemStart==false)
        {
            TargetPath="D:\\HT9045\\IniData\\Data\\";
            SourcePtah="D:\\HT9045\\IniData\\DataFTP\\";

            if(DirectoryExists(TargetPath)==false)
            {
                SetCurrentDirectory(_T("D://"));
                MyForceDirectories(TargetPath);
                MySleep(50);
            }

            str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", SourcePtah+GetLastOpenFN(), TargetPath+GetLastOpenFN());
            system(str.c_str());
            MySleep(50);
        }
        Ptr->Enabled=true;
        return;
    }

//jou 2013-01-08 make code 不用鎖定,隨時發現機台對應錯誤就立即更正.立即產生對應檔.最低限度為按下pause就能更改,然後續run
//    if(fMain->CheckCanChangeRealDummy()==false)
//    {
//        Ptr->Enabled=true;
//        return;
//    }

    //jou 2013-01-08 不用鎖定,隨時發現機台對應錯誤就立即更正.立即產生對應檔.最低限度為按下pause就能更改,然後續run
    if(SystemStart==true)
    {
        Ptr->Enabled=true;
        return;
    }

    fFTPClient->ShowFTPModal(Ptr->Tag);
    fFTPClient->bShow=false;

    if(CUSTOMER_CODE==CC_TSMC_TAINAN && SystemStart==false)                     //ChungHung 20150413 add for TSMC
    {
        _DelTree("D:\\HT9045\\IniData\\DataFTP\\", GetLastOpenFN());
    }

    Ptr->Enabled=true;

    if(CUSTOMER_CODE==CC_TERAPOWER)
    {
        if(fMain->cbSetupFileName->Text.Pos("_NET")>0)
        {
            btSaveSetupFile->Click();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btSaveSetupFileClick(TObject *Sender)
{
    SetCurrentDirectory(_T("D://"));
//  char str[256]="";
    char cStr1[256]="", cStr2[256]="";                                          //20111026 jou
    AnsiString OrgPath="", NewPath="", str1, str2;
    AnsiString SPath[2]={IncludeTrailingPathDelimiter(DataPath), IncludeTrailingPathDelimiter(OffsetPath)};

    str2=fMain->cbSetupFileName->Text;
    str1=str2.SubString(1, str2.Length()-4);
    int SPath_length=sizeof(SPath)/sizeof(AnsiString);
    for(int i=0; i<SPath_length; i++)                                           //Jimmychiu 20230307 Replace constant with array length
    {
        if(IniConfig.bE45_AllSetupFileUseOneFile && i==1)                       //Steven 20190117 : 沒有要複製Offset
            continue;

        OrgPath=SPath[i];
        NewPath=OrgPath;
        OrgPath+=str2;
        NewPath+=str1;
        MyForceDirectories(NewPath);

        //------------------20111026    jou------------------
        OrgPath+="\\*.*";
        if(CUSTOMER_CODE!=CC_TSMC_TAINAN ||
           ((CUSTOMER_CODE==CC_TSMC_TAINAN || CUSTOMER_CODE==CC_AMD_M) && i!=1))                             //wei 20160726 TSMC offset不複製
        {
            strncpy(cStr1, OrgPath.c_str(), sizeof(cStr1));
            strncpy(cStr2, NewPath.c_str(), sizeof(cStr2));
            //複製工作檔
            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
            oFile.hwnd=Handle;
            oFile.wFunc=FO_COPY;
            oFile.pFrom=cStr1;
            oFile.pTo=cStr2;
            oFile.fFlags=FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI;
            SHFileOperation(&oFile);
            //---------------------------------------------------
        }

        if(i==2)                                                                //Steven 20101118 : 強制要加加
            i++;
    }
    MySleep(500);                                                               //landam

    ClearAllSetupFile(str1, str2);                                              //Steven 20200512 : 刪除全部工作檔, 只留下當下的

    if(bNoSendSiteOnOff==true)
        EventReport(SECS_EVENT.SwitchSetupFile);

    bNoSendSiteOnOff=false;                                                     //wei 20160511 Send Site On Off
    //Ifor 20161221 (Steven) add KYEC 要求 Close Open Site By Setup File FTP 下載後不重置
    //==>
    if(CosFunction.bFTPDownLoadSiteBySetupFile==true)                           //Ifor 20171123 (Steven) : add FTP DownLoad Site By SetupFile
    {
        if(bDownloadFTP && bFTPDownLoadHasTestMode)
        {
            fMain->ShowTestHeadComp(false);
        }
        else
        {
            fMain->ShowTestHeadComp(true);
        }
    }
    else
    {
        fMain->ShowTestHeadComp(true);                                          //20140218  WEI
    }
    //<==
    //Ifor 20161221 add KYEC 要求 Close Open Site By Setup File FTP 下載後不重置
    btSaveSetupFile->Visible=false;

    if(ATC_SYSTEM==eATCHonPrecType)                                             //Ifor 20160101 工作檔更換重新連線ATC
    {
        if(ATCInterfaceForm->IsOnLine()==false)
            ATCInterfaceForm->OnLine();
        if(ATCInterfaceForm->ATC_SYS.IsChillerRun()==false)
            ATCInterfaceForm->ATCChillerSwitch(true);
        if(ATCInterfaceForm->ATC_SYS_PAL[0]->bATCRunSetting==false)
            ATCInterfaceForm->SetRunATC(true);
        bRunATC=true;                                                           //ChungHung 20160118 add for Hisi V102
    }

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20160621 FTPDOWNLOAD後切換Online和Normal
    {
//        LastSet.iTester=ON_LINE;
        fMain->ModifyTester(ON_LINE);                                           //Steven 20191218 : 整合修改LastSet.iTester
        fMain->UpdateMainOperateMode();
        fMain->CloseGpibProgram(__FUNC__);
        fMain->LoadTestModePicture();

        LastSet.iRealDummy=REALLY;
        fMain->LoadRunModePicture();
        fMain->UpdateMainOperateMode();
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::CutTempToEdit(AnsiString asString)
{
    int iPos1=0,iPos2=0,iTemp=25;
    AnsiString asBuffer1,asBuffer2;
    if(CUSTOMER_CODE==CC_SCC ||
       CUSTOMER_CODE==CC_SCK)                                                   //ChungHung 20130621 add SCK RMS
    {
        iPos1=asString.Pos("_")+1;
        asBuffer1=asString.SubString(iPos1,asString.Length());
        iPos2=asBuffer1.Pos("_")+1;
        asBuffer2=asBuffer1.SubString(iPos2,asBuffer1.Length());

        iTemp=atoi(asBuffer2.c_str());
        if(iTemp>140)
            iTemp=140;
        if(iTemp<25)
            iTemp=25;
        edTemp->Text=iTemp;
    }
}
//---------------------------------------------------------------------------
enum eWaitRtcDeleteTask
{
    wrdInitial=1,
    wrdSendComm,
    wrdWaitReply,
    wrdEndTask
};
TQPF_Timer WaitRtcDeleteDelay;
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::Timer1Timer(TObject *Sender)
{
    if(InitialOK==false)                                                        //Steven 20160912 : Add InitialOK in Timer
        return;

    AnsiString str;
    AnsiString strxxx;
    int &Task=iWaitRtcDeleteTask;
    switch(Task)
    {
        case wrdInitial:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20211122 RTC FileName
                COM2->sRealTimeCom_Send[COM2->rtFileOK] = str.sprintf("@FILE%s+" , TestIF_File.sRtcFileName);
            else
                COM2->sRealTimeCom_Send[COM2->rtFileOK] = str.sprintf("@FILE%s+" , fMain->cbSetupFileName->Text);

            if(fSetup->fShow)
            {
                if(TestIF_File.iTestMode==DualSite    ||
                   TestIF_File.iTestMode==SingleSite ||
                   TestIF_File.iTestMode==QualSite1X4 ||
                   TestIF_File.iTestMode==_8Site1X4                             //ChungHung 20150528 add for 海思 _8Site1x4
                   )                                                            //Steven 20120316 : 只分一條跟兩條?l
                {
                    COM2->sRealTimeCom_Send[COM2->rtSite]   = strxxx.sprintf("@SITE2+");
                }
                else if(bUseTwoArm32Site==true)
                {
                    COM2->sRealTimeCom_Send[COM2->rtSite]   = strxxx.sprintf("@SITE10+");
                }
                else
                {
                    COM2->sRealTimeCom_Send[COM2->rtSite]   = strxxx.sprintf("@SITE5+");
                }
            }
            else
            {
                if(TestIF_File.iTestMode==DualSite    ||
                   TestIF_File.iTestMode==SingleSite ||
                   TestIF_File.iTestMode==QualSite1X4 ||
                   TestIF_File.iTestMode==_8Site1X4                             //ChungHung 20150528 add for 海思 _8Site1x4
                   )                                                            //Steven 20120316 : 只分一條跟兩條?l
                {
                    COM2->sRealTimeCom_Send[COM2->rtSite]   = strxxx.sprintf("@SITE2+");
                }
                else if(bUseTwoArm32Site==true)
                {
                    COM2->sRealTimeCom_Send[COM2->rtSite]   = strxxx.sprintf("@SITE10+");
                }
                else
                {
                    COM2->sRealTimeCom_Send[COM2->rtSite]   = strxxx.sprintf("@SITE5+");
                }
            }

            COM2->SendCommToVision(COM2->rtFileOK, true);
            COM2->SendCommToVision(COM2->rtSite,   true);
            if(bNeedToDeleteFile)
                Task=wrdSendComm;
            else
                Task=wrdEndTask;
            break;
        case wrdSendComm:                                                       //需要砍檔案的話就要進來
            COM2->SendCommToVision(COM2->rtDELETE, true);
            WaitRtcDeleteDelay.SetSecAndOn(10);
            Task=wrdWaitReply;
            break;
        case wrdWaitReply:
            if(WaitRtcDeleteDelay.Off())
            {
                Task=wrdEndTask;
            }

            if(COM2->bRealTimeCom_ReceiveOK[COM2->rtDELETE]==true)
            {
                Task=wrdEndTask;
            }
            break;
        case wrdEndTask:
            //Steven 20110825 : 取full view image
            COM2->SendCommToVision(COM2->rtMODEL, true);
            COM2->SendCommToVision(COM2->rtInspEnd, false);
            bSendRealCCDSendStart=true;                                         //Ifor 20240919 add:避免RTC 變手動模式
            bRTCChangeFileFinish=true;                                          //ChungHung 20140514 fix in homeing and contact make time out
            Timer1->Enabled=false;
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::RTCChangeFile(bool bNeedDelete)
{
    if(!COM2->bCCDDummyRum && InitialOK==true && bDoROILearning==false)
    {
        if(Timer1->Enabled==false)
        {
            iWaitRtcDeleteTask=1;
            bRTCChangeFileFinish=false;                                         //ChungHung 20140514 fix in homeing and contact make time out
            //jou 2012-03-01 [D31] RTC Change Recipe Need reCreate RTC Model
            if(IniConfig.bD31RTCChangeRecipeNeedreCreateModel==true)
            {
                bNeedToDeleteFile=bNeedDelete;
            }
            else
            {
                bNeedToDeleteFile=false;
            }
            Timer1->Enabled=true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::LoadRTCFullViewImg(bool bShowImage)                  //Steven 20110825 : Real Time CCD - 顯示Full View Image
{
    imgRTCFullView1->Visible=true;
    imgRTCFullView2->Visible=true;
    //ChungHung 20130906 add
    imgRTCFullView3->Visible=true;
    imgRTCFullView4->Visible=true;

    if(bShowImage)
    {
        if(MachineTypeChoice==Type_HT9046_LS)                                   //2013-01-15    Dell
        {
            if(FileExists(FULLVIEWIMAGEPATH1))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH1);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView1->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH4))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH4);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView2->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH2))
                imgRTCFullView3->Picture->LoadFromFile(FULLVIEWIMAGEPATH2);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView3->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView3->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH5))
                imgRTCFullView4->Picture->LoadFromFile(FULLVIEWIMAGEPATH5);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView4->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView4->Visible=false;
        }
        else
        {
            if(FileExists(FULLVIEWIMAGEPATH1))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH1);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView1->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH2))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH2);
            else if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView2->Visible=false;
        }
    }
    else
    {
        if(FileExists(FULLVIEWIMAGEPATH3))
            imgRTCFullView1->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
        else
            imgRTCFullView1->Visible=false;

        if(FileExists(FULLVIEWIMAGEPATH3))
            imgRTCFullView2->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
        else
            imgRTCFullView2->Visible=false;

        if(MachineTypeChoice==Type_HT9046_LS)                                   //2013-01-15    Dell
        {
            if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView3->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView3->Visible=false;

            if(FileExists(FULLVIEWIMAGEPATH3))
                imgRTCFullView4->Picture->LoadFromFile(FULLVIEWIMAGEPATH3);
            else
                imgRTCFullView4->Visible=false;
        }
    }
}
//------------------------------------------------------------------------------
HTimer ShowATCThermoOverDelay [ATC_HEAD_COUNT];
HTimer ShowATCThermoBelowDelay[ATC_HEAD_COUNT];
HTimer ShowATCThermoCheckDelay[ATC_HEAD_COUNT];                                 //pig 2012.02.12 KyecATC

HTimer ATCOutsideAlarmCheck[ATC_HEAD_COUNT];                                    //Ifor 20160804 add ATC [L11-6] 第一點溫度與設定溫度相差3度連續3秒Alarm
HTimer ATCCompareAlarmCheck[ATC_HEAD_COUNT];                                    //Ifor 20160804 add ATC [L11-8] 測中兩點溫度相差3度連續3秒Alarm
HTimer ATCTempSameAlarmCheck[ATC_HEAD_COUNT];                                   //Ifor 20160804 add ATC 第一點溫度相同Alarm 判斷
HTimer ATCRefTempSameAlarmCheck[ATC_HEAD_COUNT];                                //Ifor 20160804 add ATC 第二點溫度相同Alarm 判斷
HTimer ATCMaxAlarmCheck[ATC_HEAD_COUNT];                                        //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時

HTimer ATCOFSDelay;

//------------------------------------------------------------------------------
void __fastcall TfLotInfo::ShowATCThermo()                                      //Steven 20120410 : Hontech ATC
{
    static double OldSetATCTemp=-1;

    double dSetATCTemp=0.0;
    double dTempRange=0.0;

    RunInfo.ATCSoftwareVersion=sATCVerRead;                                     //Ifor 20160321 (wei) Add SECS GEM ATC Ver
    fObserver->pnlATCVersion->Caption=RunInfo.ATCSoftwareVersion;               //Ifor 20170317 (wei) add ATC 版本顯示於Motion View 位置

    if(IniConfig.bL11_6ATCUseTemperatureOutsideAlarm)                           //Ifor 20150911 : [L11-6] ATC突波於設定範圍內且持續發生超過設定時間後發出Alarm      //Steven 20160301 : For ATC
        dTempRange=IniConfig.iATCTemperatureOutside;
    else if(IniConfig.bL11_1ATCTemperatureOverAlarm)
        dTempRange=IniConfig.iATCTemperatureRange;
    else if(LastSet.iTemperature==Tempture_AmbientHot)                          //kevin 20140918 恆溫控制       //Steven 20160301 : For ATC
        dTempRange=Temperature.fAmbientHotGuartbent;
    else if(LastSet.iTemperature==Tempture_Ambient && Temperature.bAmbientGuardbandCheck && IniConfig.bL20AbientGuardBand)                                      //kevin 20180115 (Steven) add Amient Guard Band
        dTempRange=Temperature.iAmbGuardband;                                   //kevin 20180115 : 檢查範圍
    else
        dTempRange=IniConfig.iL04TemptureRange;

    if(LastSet.iTemperature==Tempture_Hot ||
       LastSet.iTemperature==Tempture_AmbientHot)                               //kevin 20181011  : add 恆溫控制
        dSetATCTemp=(double)Temperature.fWorkTemperBase;
    else
        dSetATCTemp=(double)IniConfig.dATCAmbientTemperature;                   //Steven 20130122 : [L11] ATC常溫的溫度

    if(OldSetATCTemp!=dSetATCTemp &&
       Temperature.bMultiZoneEnable==false)
    {
        OldSetATCTemp=dSetATCTemp;
        palATCWorkingTemp->Caption=dSetATCTemp;
    }

    if(ATC_SYSTEM==eATCHonPrecType &&
       Temperature.bATC70Active==true)                                          //Eliot 2015_0105 start
    {
        ShowATC70Thermo(dSetATCTemp, dTempRange);
    }                                                                           //Eliot 2015_0105 end
    else if(ATC_SYSTEM==eATCHonPrecType)
    {
        ShowATC20Thermo(dSetATCTemp, dTempRange);
    }
    else if(ATC_SYSTEM==eNewATCSystem)                                          //Ifor 20151230 :add New ATC Interface 溫度顯示
    {
        ShowNewATCThermo(dSetATCTemp, dTempRange, OldSetATCTemp);
    }

    if(ATC_SYSTEM==eNewATCSystem &&
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70 &&
       Temperature.bATCActiveCooling &&
       Sen[SnTJCurrent].Enable && Sen[SnTJCurrent].IsOn())                      //Steven 20180124 (Jou) : Check ATC7.0 TJ Current
    {
        ShowErrorMessage("WAR15243", 0, MMATC_Head);
    }
}
//------------------------------------------------------------------------------
void TfLotInfo::ShowATC70Thermo(double SetATCTemp, double iTempRange)
{
    static int iAlarmCount=0;                                                   //Eliot 2015_0105
    static int Count[ATC_HEAD_COUNT];
    static int iCount[ATC_HEAD_COUNT];                                          //pig 2012.02.12 KyecATC
//    static bool bATCOfsST=false;
    static bool bInitialfinish=false;
    static bool bCheckOverAlarm[ATC_HEAD_COUNT];                                //pig 2012.02.12 KyecATC
    static bool bCheckBelowAlarm[ATC_HEAD_COUNT];                               //pig 2012.02.12 KyecATC
    static bool bATCFirstIn[ATC_HEAD_COUNT];                                    //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCTempAlwaysSameAlarm[ATC_HEAD_COUNT][2];                     //Ifor 20160716 : add 發生溫度連續相同Alarm
    static bool bATCSameTempFirstIn[ATC_HEAD_COUNT];                            //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCHasAlarm[ATC_HEAD_COUNT];                                   //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
    static bool bATCMaxFirstIn[ATC_HEAD_COUNT];                                 //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
    static double OldATCTemp[ATC_HEAD_COUNT];                                   //Ifor 20160115 上一筆ATC第一點溫度

    AnsiString str;

    bool bUseMaxPeakTiming=false;                                               //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
    double ATCNowTemp[ATC_HEAD_COUNT];
    double ATCNowTSDTemp[ATC_HEAD_COUNT];                                       //Eliot 2015_0105
    double ATCTemp[ATC_HEAD_COUNT];                                             //Eliot 2015_0105
    double iMaxTime=ATCInterfaceForm->iCheckSameTempTime;                       //Ifor 20160215 add 最大相同秒數

    if(bInitialfinish==false)                                                   //Ifor 20160420 add
    {
        bInitialfinish=true;
        ZeroMemory(OldATCTemp, sizeof(OldATCTemp));
        ZeroMemory(Count, sizeof(Count));
        ZeroMemory(iCount, sizeof(iCount));
        ZeroMemory(bCheckOverAlarm, sizeof(bCheckOverAlarm));
        ZeroMemory(bCheckBelowAlarm, sizeof(bCheckBelowAlarm));
        ZeroMemory(bATCFirstIn, sizeof(bATCFirstIn));
        ZeroMemory(bATCTempAlwaysSameAlarm, sizeof(bATCTempAlwaysSameAlarm));
        ZeroMemory(bATCSameTempFirstIn, sizeof(bATCSameTempFirstIn));
        ZeroMemory(bATCMaxFirstIn, sizeof(bATCMaxFirstIn));                     //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
        ZeroMemory(bATCHasAlarm, sizeof(bATCHasAlarm));                         //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
        return;
    }
    ZeroMemory(ATCNowTemp, sizeof(ATCNowTemp));
    ZeroMemory(ATCNowTSDTemp, sizeof(ATCNowTSDTemp));
    ZeroMemory(ATCTemp, sizeof(ATCTemp));

    if(ATC_SYSTEM!=eATCHonPrecType ||
       Temperature.bATC70Active==false)

    if(fContact->fShow==true)
        return;

    if(LastSet.iTemperature==1 && aldATCPower->Value==false)
    {
        iAlarmCount++;
        if(iAlarmCount>=50)
        {
            iAlarmCount=0;
            if(LastSet.iTester==ON_LINE)                                        //JerryYang 20230322 : 新增2DID模式, != offline改為 ==online  //Ifor 20180222 (wei) :HT7045 Offline 不判斷 ATC是否連線
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20230719 add:KYEC要求改用Message
                {
                    if(bStartATCRun)
                        ShowMyMessage("ATC System Connect Error!!", "ATC 系統連線異常!!");
                    else
                        ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
                }
                else
                {
                    ShowErrorMessage("WAR15309", 0, MMATC_TCPIP, false, "Main--Timer5");                                //ATC software disconnect! Please check ATC software ==> ATC connect error, Please confirm whether to open? and connect ATC
                }
            }
        }
    }
    else
    {
        iAlarmCount=0;
    }
    iATC70_Count++;
    if(iATC70_Count>=100)                                                       //Ifor 20160223 100*100=10000ms 10s未收到溫度回覆訊息Alarm
    {
        iATC70_Count=0;
        if(LastSet.iTester==ON_LINE)                                            //JerryYang 20230322 : 新增2DID模式, != offline改為 ==online  //Ifor 20180222 (wei) :HT7045 Offline 不判斷 ATC是否連線
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //Ifor 20230719 add:KYEC要求改用Message
            {
                if(bStartATCRun)
                    ShowMyMessage("ATC System Connect Error!!", "ATC 系統連線異常!!");
                else
                    ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
            }
            else
            {
                ShowErrorMessage("WAR15309", 0, MMATC_TCPIP, false, "Main--Timer5");                                    //ATC software disconnect! Please check ATC software ==> ATC connect error, Please confirm whether to open? and connect ATC
            }
        }
        return;
    }

    for(unsigned int i=0; i<4; i++)
    {
        if(Temperature.bATC7ChannelEnabled[i])
        {
            ATCNowTemp[i]=ATCInterfaceForm->fATC7NowTemp[i];
            ATCNowTSDTemp[i]=ATCInterfaceForm->fATC7NowTSDTemp[i];
            if(Temperature.bATC7TSDFunction==false)
                ATCTemp[i] = ATCNowTemp[i];
            else
                ATCTemp[i] = ATCNowTSDTemp[i];

            if(ATCTemp[i]==0)                                                   //Ifor 20160716 ATC 7.0 溫度讀取到0度強制修改成999.9
                ATCTemp[i]=999.9;

            if(iMaxTime!=0 && SystemStart && iATCOnLine)                        //Ifor 20160804 更改溫度相同警示計時方式 Start
            {
                if(OldATCTemp[i]==ATCTemp[i])                                   //Ifor 20160816 add 溫度相同Alarm 於機台Run與ATC連線正常下才偵測
                {
                    if(bATCSameTempFirstIn[i]==false)
                    {
                        ATCTempSameAlarmCheck[i].SetSecAndOn(iMaxTime);         //Ifor 20160804 設定計時時間 & 開始計時
                        bATCSameTempFirstIn[i]=true;
                    }
                    else
                    {
                        if(ATCTempSameAlarmCheck[i].Off())
                        {
                            bATCTempAlwaysSameAlarm[i][0]=true;                 //Ifor 20160716 設定ATC第一點溫度相同Alarm 旗標
                            str.sprintf("CH%02d" , i+1);
                            ShowErrorMessage("WAR15300", 0, MMATC_Head, false, str);                                    //ATC temperature sensor always same
                            bATCSameTempFirstIn[i]=false;                       //Ifor 20160804 重新計時
                        }
                    }
                }
                else
                {
                    OldATCTemp[i]=ATCTemp[i];
                    ATCTempSameAlarmCheck[i].Pause();                           //Ifor 20160804 暫停計時
                    ATCTempSameAlarmCheck[i].Clear();                           //Ifor 20160804 清除計時值
                    bATCSameTempFirstIn[i]=false;                               //Ifor 20160804 重新計時
                    bATCTempAlwaysSameAlarm[i][0]=false;                        //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
                }
            }                                                                   //Ifor 20160804 更改溫度相同警示計時方式 End
            else                                                                //Ifor 20160921 重新計時溫度連續相同
            {
                bATCSameTempFirstIn[i]=false;                                   //Ifor 20160804 重新計時
                if(OldATCTemp[i]!=ATCTemp[i])
                {
                    bATCTempAlwaysSameAlarm[i][0]=false;                        //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
                }
            }

            if(Temperature.bATC7ChannelEnabled[i]==true)
            {
                if(iCount[i]==0)
                {
                    ShowATCThermoCheckDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                    iCount[i]++;
                }

                if(SystemStart &&                                               //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
                   bRunAutoClean==false &&                                      //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
                   bCheckATCTemp==false &&                                      //Ifor 20230503 add:Start 後的Temp Wait 不報警
                   bChangeTest_TempAlarm==false)                                //Ifor 20230504 add: 避免修改溫度後報警
                {
                    if(bATCTempAlwaysSameAlarm[i][0]==true &&
                       OldATCTemp[i]==ATCTemp[i])                               //Ifor 20160716 add 當發生ATC溫度連續相同Alarm，Alarm解除後需再次判斷溫度，若相同須再次Alarm不可跑貨
                    {
                        str.sprintf("CH%02d" , i+1);
                        ShowErrorMessage("WAR15300", 0, MMATC_Head, false, str);                                        //ATC temperature sensor always same
                    }

                    if(IniConfig.bL11_6ATCUseTemperatureOutsideAlarm==true &&   //Ifor 20150911 : ATC 2.0 量測溫度超出設定值  [L11_6]功能
                       bATCHasAlarm[0]==false)                                  //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    {                                                           //Ifor 20150911 :判斷"現在溫度 >= 設定溫度 +設定突波溫度" || "現在溫度 <= 設定溫度 - 設定突波溫度"
                        if((ATCTemp[i] >= (SetATCTemp + IniConfig.iATCTemperatureOutside)) ||
                           (ATCTemp[i] <= (SetATCTemp - IniConfig.iATCTemperatureOutside)) )
                        {
                            if(bATCFirstIn[i]==false)                           //Ifor 20150911 : 判斷是否為新事件發生
                            {
                                ATCOutsideAlarmCheck[i].SetSecAndOn(IniConfig.iATCTemperatureContinuous);               //Ifor 20160804 設定[L11_6]計時時間 & 開始計時
                                bATCFirstIn[i] = true;
                                fMain->LabHisi_Set->Caption = IntToStr(IniConfig.iATCTemperatureOutside)+"°";           //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                            }
                            else
                            {
                                if(ATCOutsideAlarmCheck[i].Off())
                                {
                                    bATCHasAlarm[0]=true;                       //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                    fMain->LabHisi_Set->Caption = IntToStr(IniConfig.iATCTemperatureOutside)+"°"+IntToStr(IniConfig.iATCTemperatureContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                    str.sprintf("CH%02d" , i+1);
                                    ShowErrorMessage("WAR15301", 0, MMATC_Head, false, str);                            //ATC Temperature alarm by [L11-6]
                                    bATCFirstIn[i] = false;
                                }
                            }
                        }
                        else
                        {
                            bATCFirstIn[i] = false;
                            ATCOutsideAlarmCheck[i].Pause();                    //Ifor 20160804 暫停計時
                            ATCOutsideAlarmCheck[i].Clear();                    //Ifor 20160804 清除計時值
                        }
                    }

                    if(IniConfig.bL11_7ATCUseMaxSurgeAlarm==true && bATCHasAlarm[1]==false)                             //Ifor 20150911 判斷是否超出最大突波允許值 [L11_7]功能    //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    {
                        if((ATCTemp[i]>=(SetATCTemp+IniConfig.iATCMaxSurgeAlarm)) ||
                           (ATCTemp[i]<=(SetATCTemp-IniConfig.iATCMaxSurgeAlarm)) )
                        {
                            if(bUseMaxPeakTiming==true)                         //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
                            {
                                if(bATCMaxFirstIn[i]==false)                    //Ifor 20150911 : 判斷是否為新事件發生
                                {
                                    ATCMaxAlarmCheck[i].SetSecAndOn(IniConfig.iATCMaxAlarmContinuous);                  //Ifor 20160804 設定[L11_6]計時時間 & 開始計時
                                    bATCMaxFirstIn[i] = true;
                                    fMain->LabHisi_Set->Caption = IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°";            //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                }
                                else
                                {
                                    if(ATCMaxAlarmCheck[i].Off())
                                    {
                                        bATCHasAlarm[1]=true;                   //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                        fMain->LabHisi_Set->Caption = IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°"+IntToStr(IniConfig.iATCMaxAlarmContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                        str.sprintf("CH%02d" , i+1);
                                        ShowErrorMessage("WAR15302", 0, MMATC_Head, false, str);                        //ATC Temperature alarm by [L11-6]
                                        bATCMaxFirstIn[i] = false;
                                    }
                                }
                            }
                            else
                            {
                                fMain->LabHisi_Set->Caption = IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°"+IntToStr(IniConfig.iATCMaxAlarmContinuous) +"s";        //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                str.sprintf("CH%02d" , i+1);
                                ShowErrorMessage("WAR15302", 0, MMATC_Head, false, str);                                //ATC Temperature alarm by [L11-7]
                            }
                        }
                        else
                        {
                            bATCMaxFirstIn[i] = false;
                            ATCMaxAlarmCheck[i].Pause();
                            ATCMaxAlarmCheck[i].Clear();
                        }
                    }
                }
                else
                {
                    bATCHasAlarm[0]=false;                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    bATCHasAlarm[1]=false;                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    bATCHasAlarm[2]=false;                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                }

                if(ATCTemp[i]>=(SetATCTemp-iTempRange) ||
                   ShowATCThermoCheckDelay[i].Off())
                {
                    if(ATCTemp[i]<200 && LastSet.iTemperature==1)
                        bCheckOverAlarm[i]=true;
                }

                if(ATCTemp[i]<=(SetATCTemp+iTempRange) || ShowATCThermoCheckDelay[i].Off())
                {
                    if(ATCTemp[i]>0 && LastSet.iTemperature==1)
                        bCheckBelowAlarm[i]=true;
                }

                if(ATCTemp[i]>(SetATCTemp+iTempRange))                          // OVER
                {
                    ATCPtr[i]->Color=(TColor)0x008000FF;
                    if(Count[i]==0)
                    {
                        ShowATCThermoCheckDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                        Count[i]++;
                    }

                    if(ShowATCThermoCheckDelay[i].Off())
                    {
                        ATCInterfaceForm->SetRunATC(false);                     //Steven 20120725 : 換位置
                        bRunATC=false;                                          //ChungHung 20160118 add for Hisi V102
                        str.sprintf("CH%02d" , i+1);
                        ShowErrorMessage("WAR15304", 0, MMATC_Head, false, str);                                        //ATC temperature over error
                        if(bCheckOverAlarm[i]==true)
                        {
                            bCheckOverAlarm[i]=false;
                            iCount[i]=0;
                            ShowMyMessage(str, "");
                        }
                        ShowATCThermoCheckDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                    }
                }
                else if(ATCTemp[i]<(SetATCTemp-iTempRange) && ATCTemp[i]!=0)    // Below
                {
                    ATCPtr[i]->Color=clYellow;
                    if(SystemStart &&                                           //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
                       bRunAutoClean==false &&                                  //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
                       bCheckATCTemp==false &&                                  //Ifor 20230503 add:Start 後的Temp Wait 不報警
                       bChangeTest_TempAlarm==false)                            //Ifor 20230504 add: 避免修改溫度後報警
                    {
                        if(Count[i]==0)
                        {
                            ShowATCThermoCheckDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                            Count[i]++;
                        }

                        if(ShowATCThermoCheckDelay[i].Off())
                        {
                            ATCInterfaceForm->SetRunATC(true);                  //Steven 20120725 : 換位置
                            bRunATC=true;                                       //ChungHung 20160118 add for Hisi V102
                            if(bCheckBelowAlarm[i]==true)
                            {
                                bCheckBelowAlarm[i]=false;
                                iCount[i]=0;
                                str.sprintf("CH%02d" , i+1);
                                ShowErrorMessage("WAR15305", 0, MMATC_Head, false, str);                                //ATC temperature below error
                            }
                            ShowATCThermoCheckDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                        }
                    }
                }
                else
                {
                    Count[i]=0;
                    ATCPtr[i]->Color=(TColor)0x00E9AC3A;
                    ShowATCThermoCheckDelay[i].Pause();
                    ShowATCThermoCheckDelay[i].Clear();
                }
                str.sprintf("%5.2f", ATCNowTemp[i]);
                ATCPtr[i]->Caption=str ;
                str.sprintf("%5.2f", ATCNowTSDTemp[i]);
                ATCReferPtr[i]->Caption=str ;
            }
            else
            {
                ATCPtr[i]->Color        =clWhite;
                ATCPtr[i]->Caption      ="NA";                                  //kevin 20191214 change  Off-Line->NA
                ATCReferPtr[i]->Color   =clWhite;
                ATCReferPtr[i]->Caption ="Off-Line";                            //kevin 20191214 change  Off-Line->NA
            }
        }
        else
        {
            ATCPtr[i]->Color   =clWhite;
            ATCPtr[i]->Caption ="NA";                                           //kevin 20191214 change  Off-Line->NA
        }
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ShowATC20Thermo(double SetATCTemp, double iTempRange)
{
    static int Count[ATC_HEAD_COUNT];
    static int iCount[ATC_HEAD_COUNT];                                          //pig 2012.02.12 KyecATC

    static bool bInitialfinish=false;
    static bool bCheckOverAlarm[ATC_HEAD_COUNT];                                //pig 2012.02.12 KyecATC
    static bool bCheckBelowAlarm[ATC_HEAD_COUNT];                               //pig 2012.02.12 KyecATC
    static bool bATCFirstIn[ATC_HEAD_COUNT];                                    //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCompareCFirstIn[ATC_HEAD_COUNT];                             //Ifor 20150925 : 兩組Sensor其中一組溫度第一次超出溫度預設
    static bool bATCTempAlwaysSameAlarm[ATC_HEAD_COUNT][2];                     //Ifor 20160716 : add 發生溫度連續相同Alarm
    static bool bATCSameTempFirstIn[ATC_HEAD_COUNT];                            //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCRefSameTempFirstIn[ATC_HEAD_COUNT];                         //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCMaxFirstIn[ATC_HEAD_COUNT];                                 //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
    static bool bATCHasAlarm[4];                                                //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次

    static double OldATCTemp[ATC_HEAD_COUNT];                                   //Ifor 20160115 上一筆ATC第一點溫度
    static double OldATCRefTemp[ATC_HEAD_COUNT];                                //Ifor 20160115 上一筆ATC第二點溫度

    bool bUseMaxPeakTiming=false;                                               //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時

    double ATCNowTemp[ATC_HEAD_COUNT];
    double ATCRefTemp[ATC_HEAD_COUNT];
    double iMaxTime=ATCInterfaceForm->iCheckSameTempTime;                       //Ifor 20160215 add 最大相同秒數

    AnsiString str;

    if(ATC_SYSTEM!=eATCHonPrecType)
        return ;

    if(bInitialfinish==false)                                                   //Ifor 20160420 add
    {
        bInitialfinish=true;
        ZeroMemory(OldATCRefTemp, sizeof(OldATCRefTemp));
        ZeroMemory(Count, sizeof(Count));
        ZeroMemory(iCount, sizeof(iCount));
        ZeroMemory(bCheckOverAlarm, sizeof(bCheckOverAlarm));
        ZeroMemory(bCheckBelowAlarm, sizeof(bCheckBelowAlarm));
        ZeroMemory(bATCFirstIn, sizeof(bATCFirstIn));
        ZeroMemory(bATCompareCFirstIn, sizeof(bATCompareCFirstIn));
        ZeroMemory(bATCTempAlwaysSameAlarm, sizeof(bATCTempAlwaysSameAlarm));
        ZeroMemory(bATCSameTempFirstIn, sizeof(bATCSameTempFirstIn));
        ZeroMemory(bATCRefSameTempFirstIn, sizeof(bATCRefSameTempFirstIn));
        ZeroMemory(bATCMaxFirstIn, sizeof(bATCMaxFirstIn));                     //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
        ZeroMemory(bATCHasAlarm, sizeof(bATCHasAlarm));                         //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
        return;
    }
    ZeroMemory(ATCNowTemp, sizeof(ATCNowTemp));
    ZeroMemory(ATCRefTemp, sizeof(ATCRefTemp));

    if(Temperature.bATCActiveCooling==false)
    {
        for(unsigned int i=0; i<ATCInterfaceForm->ATC_SYS_PAL.size(); i++)
        {
            ATCPtr[i]->Color            =clWhite;
            ATCPtr[i]->Caption          ="NA";                                  //kevin 20191214 change  Off-Line->NA
            ATCReferPtr[i]->Color       =clWhite;                               //Steven 20150108 : [L11-5] For海思使用兩組感溫
            ATCReferPtr[i]->Caption     ="NA";                                  //kevin 20191214 change  Off-Line->NA
        }
        aldATCPower->Value          =false;                                     //Ifor 20160621 ATC2.0 斷線&離線 關閉 顯示燈號
        aldATCChillerStatus->Value  =false;
        return;
    }

    if(ATCInterfaceForm->bATCConnectError==true)
    {
        ATCInterfaceForm->bATCConnectError=false;
        if(LastSet.iTester==ON_LINE)                                            //JerryYang 20230322 : 新增2DID模式, != offline改為 ==online  //Ifor 20180222 (wei) :HT7045 Offline 不判斷 ATC是否連線
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //Ifor 20230719 add:KYEC要求改用Message
            {
                if(bStartATCRun)
                    ShowMyMessage("ATC System Connect Error!!", "ATC 系統連線異常!!");
                else
                    ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
            }
            else
            {
                ShowErrorMessage("WAR15309", 0, MMATC_TCPIP);
            }
        }
        fMain->ChangeATCSiteUse();
    }

    for(unsigned int i=0; i<ATCInterfaceForm->ATC_SYS_PAL.size(); i++)
    {
        ATCNowTemp[i]=ATCInterfaceForm->GetATCSiteNowTemperature(i);
        ATCRefTemp[i]=ATCInterfaceForm->GetATCSiteNowTemperature_Ref(i);        //Steven 20150108 : [L11-5] For海思使用兩組感溫

        if(ATCNowTemp[i]==0)                                                    //Ifor 20160716 ATC 2.0 溫度讀取到0度強制修改成999.9
            ATCNowTemp[i]=999.9;
        if(ATCRefTemp[i]==0)
            ATCRefTemp[i]=999.9;

        if(iMaxTime!=0 && bATC_EnablesChannel[i]==true  &&
           SystemStart && iATCOnLine)                                           //Ifor 20160718 ATC2.0 溫度相同Alarm 未開Site 不偵測
        {
            if(OldATCTemp[i]==ATCNowTemp[i])                                    //Ifor 20160804 更改溫度相同警示計時方式 Start //Ifor 20160816 add 溫度相同Alarm 於機台Run與ATC連線正常下才偵測
            {
                if(bATCSameTempFirstIn[i]==false)
                {
                    ATCTempSameAlarmCheck[i].SetSecAndOn(iMaxTime);             //Ifor 20160804 設定溫度相同Alarm計時時間 & 開始計時
                    bATCSameTempFirstIn[i]=true;
                }
                else
                {
                    if(ATCTempSameAlarmCheck[i].Off())
                    {
                        bATCTempAlwaysSameAlarm[i][0]=true;                     //Ifor 20160716 設定ATC第一點溫度相同Alarm 旗標
                        str.sprintf("CH%02d" , i+1);
                        ShowErrorMessage("WAR15300", 0, MMATC_Head, false, str);                                        //ATC temperature sensor always same
                        bATCSameTempFirstIn[i]=false;                           //Ifor 20160804 重新計時
                    }
                }
            }
            else
            {
                OldATCTemp[i]=ATCNowTemp[i];
                ATCTempSameAlarmCheck[i].Pause();                               //Ifor 20160804 暫停計時
                ATCTempSameAlarmCheck[i].Clear();                               //Ifor 20160804 清除計時值
                bATCSameTempFirstIn[i]=false;                                   //Ifor 20160804 重新計時
                bATCTempAlwaysSameAlarm[i][0]=false;                            //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
            }                                                                   //Ifor 20160804 更改溫度相同警示計時方式 End

            if(Temperature.bUseReferTempSensor==true)
            {
                if(OldATCRefTemp[i]==ATCRefTemp[i])                             //Ifor 20160804 更改第二點溫度相同警示計時方式 Start //Ifor 20160816 add 溫度相同Alarm 於機台Run與ATC連線正常下才偵測
                {
                    if(bATCRefSameTempFirstIn[i]==false)
                    {
                        ATCRefTempSameAlarmCheck[i].SetSecAndOn(iMaxTime);      //Ifor 20160804 設定第二點溫度相同計時時間 & 開始計時
                        bATCRefSameTempFirstIn[i]=true;
                    }
                    else
                    {
                        if(ATCRefTempSameAlarmCheck[i].Off())
                        {
                            bATCTempAlwaysSameAlarm[i][1]=true;                 //Ifor 20160716 設定ATC第一點溫度相同Alarm 旗標
                            str.sprintf("CH%02d" , i+1);
                            ShowErrorMessage("WAR15306", 0, MMATC_NI, false, str);                                      //ATC temperature sensor always same
                            bATCRefSameTempFirstIn[i]=false;                    //Ifor 20160804 重新計時
                        }
                    }
                }
                else
                {
                    OldATCRefTemp[i]=ATCRefTemp[i];
                    ATCRefTempSameAlarmCheck[i].Pause();                        //Ifor 20160804 暫停計時
                    ATCRefTempSameAlarmCheck[i].Clear();                        //Ifor 20160804 清除計時值
                    bATCRefSameTempFirstIn[i]=false;                            //Ifor 20160804 重新計時
                    bATCTempAlwaysSameAlarm[i][1]=false;                        //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
                }                                                               //Ifor 20160804  更改第二點溫度相同警示計時方式 End
            }
        }
        else                                                                    //Ifor 20160921 重新計時
        {
            bATCSameTempFirstIn[i]=false;                                       //Ifor 20160804 重新計時
            bATCRefSameTempFirstIn[i]=false;                                    //Ifor 20160804 重新計時
            if(OldATCTemp[i]!=ATCNowTemp[i])
            {
                bATCTempAlwaysSameAlarm[i][0]=false;                            //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
            }

            if(OldATCRefTemp[i]!=ATCRefTemp[i])
            {
                bATCTempAlwaysSameAlarm[i][1]=false;                            //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
            }
        }

        if(ATCInterfaceForm->ATC_SYS_PAL[i]->LedATCConnect->Value  &&           //Ifor 20160120 Add 新增判斷開啟ATC才執行
           (TestIF.iTestMode==DualSite ||
            TestIF.iTestMode==SingleSite ||
            TestIF.iTestMode==QualSite2X2 ||
            TestIF.iTestMode==QualSite1X4))                                     //Ifor 20210608 add: ATC2.0 Qual Site Mode
        {
            aldATCPower->Value          =ATCInterfaceForm->ATCRun(true);        //Ifor 20160621 (wei) ATC2.0 狀態顯示往下移動
            aldATCChillerStatus->Value  =ATCInterfaceForm->ledChiller->Value;
            pl_ATCChillerSV->Caption    =ATCInterfaceForm->ATC_SYS_PAL[0]->fATCSetChillerValue;
            RunInfo.asChillerTemp       =pl_ATCChillerSV->Caption;
            if(ATCInterfaceForm->ATC_SYS_PAL[i]->bSiteOnOffSetting)
            {
                if(iCount[i]==0)
                {
                    ShowATCThermoCheckDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                    iCount[i]++;
                }

                if(ATCNowTemp[i]>=(SetATCTemp-iTempRange) ||
                   ShowATCThermoCheckDelay[i].Off())
                {
                    bCheckOverAlarm[i]=true;
                }

                if(ATCNowTemp[i]<=(SetATCTemp+iTempRange) ||
                   ShowATCThermoCheckDelay[i].Off())
                {
                    bCheckBelowAlarm[i]=true;
                }

                if(SystemStart &&                                               //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
                   bRunAutoClean==false &&                                      //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
                   bCheckATCTemp==false &&                                      //Ifor 20230503 add:Start 後的Temp Wait 不報警
                   bChangeTest_TempAlarm==false)                                //Ifor 20230504 add: 避免修改溫度後報警
                {
                    if(Temperature.bUseReferTempSensor==true)                   //Ifor 20160114 開啟[L11_5]第二點感溫Sensor
                    {
                        if(bATCTempAlwaysSameAlarm[i][1]==true &&
                            OldATCRefTemp[i]==ATCRefTemp[i])                    //Ifor 20160716 add 當發生ATC溫度連續相同Alarm，Alarm解除後需再次判斷溫度，若相同須再次Alarm不可跑貨
                        {
                            str.sprintf("CH%02d" , i+1);
                            ShowErrorMessage("WAR15306", 0, MMATC_NI, false, str);                                      //ATC temperature refer sensor always same
                        }

                        if(IniConfig.bL11_8ATCUseTemperatureCompare==true &&    //Ifor 20151029 : ATC 2.0 [L11_8]功能
                           bATCHasAlarm[2]==false)                              //Ifor 20160718 修改 ATC2.0 [L11_8]Alarm //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                        {                                                       //Ifor 20160804 更改 [L11-8] 兩點溫差三度三秒Alarm計時方式 Start
                            if((ATCNowTemp[i]-ATCRefTemp[i]>=IniConfig.iATCTemperatureOutside) ||
                               (ATCRefTemp[i]-ATCNowTemp[i]>=IniConfig.iATCTemperatureOutside) )
                            {
                                if(bATCompareCFirstIn[i]==false)                //Ifor 20150911 : 判斷是否為新事件發生
                                {
                                    ATCCompareAlarmCheck[i].SetSecAndOn(IniConfig.iATCTemperatureContinuous);           //Ifor 20160804 設定[L11_8]計時時間 & 開始計時
                                    bATCompareCFirstIn[i]=true;
                                    fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°";         //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                }
                                else
                                {
                                    if(ATCCompareAlarmCheck[i].Off())
                                    {
                                        if((i<2 && bATC_SITE_2ND_CHECK[0]==true) ||
                                           (i>=2 && bATC_SITE_2ND_CHECK[1]==true))
                                        {
                                            bATCHasAlarm[2]=true;               //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                            fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°"+IntToStr(IniConfig.iATCTemperatureContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                            str.sprintf("CH%02d" , i+1);
                                            ShowErrorMessage("WAR15303", 0, MMATC_Head, false, str);                    //ATC Temperature alarm by [L11-8]
                                            bATCompareCFirstIn[i] = false;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                ATCCompareAlarmCheck[i].Pause();                //Ifor 20160804 暫停計時
                                ATCCompareAlarmCheck[i].Clear();                //Ifor 20160804 清除計時值
                                bATCompareCFirstIn[i] = false;
                            }                                                   //Ifor 20160804 更改 [L11-8] 兩點溫差三度三秒Alarm計時方式 End
                        }
                    }

                    if(bATCTempAlwaysSameAlarm[i][0]==true &&
                       OldATCTemp[i]==ATCNowTemp[i])                            //Ifor 20160716 add 當發生ATC溫度連續相同Alarm，Alarm解除後需再次判斷溫度，若相同須再次Alarm不可跑貨
                    {
                        str.sprintf("CH%02d" , i+1);
                        ShowErrorMessage("WAR15300", 0, MMATC_Head, false, str);                                        //ATC temperature sensor always same
                    }

                    if(IniConfig.bL11_6ATCUseTemperatureOutsideAlarm==true &&   //Ifor 20150911 : ATC 2.0 量測溫度超出設定值  [L11_6]功能
                       bATCHasAlarm[0]==false)                                  //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    {                                                           //Ifor 20150911 :判斷"現在溫度 >= 設定溫度 +設定突波溫度" || "現在溫度 <= 設定溫度 - 設定突波溫度"
                        if((ATCNowTemp[i]>=(SetATCTemp+IniConfig.iATCTemperatureOutside)) ||
                           (ATCNowTemp[i]<=(SetATCTemp-IniConfig.iATCTemperatureOutside)) )
                        {
                            if(bATCFirstIn[i]==false)                           //Ifor 20150911 : 判斷是否為新事件發生
                            {
                                ATCOutsideAlarmCheck[i].SetSecAndOn(IniConfig.iATCTemperatureContinuous);               //Ifor 20160804 設定[L11_6]計時時間 & 開始計時
                                bATCFirstIn[i] = true;
                                fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°";             //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                            }
                            else
                            {
                                if(ATCOutsideAlarmCheck[i].Off())
                                {
                                    bATCHasAlarm[0]=true;                       //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                    fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°"+IntToStr(IniConfig.iATCTemperatureContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                    str.sprintf("CH%02d" , i+1);
                                    ShowErrorMessage("WAR15301", 0, MMATC_Head, false, str);                            //ATC Temperature alarm by [L11-6]
                                    bATCFirstIn[i] = false;
                                }
                            }
                        }
                        else
                        {
                            bATCFirstIn[i] = false;
                            ATCOutsideAlarmCheck[i].Pause();                    //Ifor 20160804 暫停計時
                            ATCOutsideAlarmCheck[i].Clear();                    //Ifor 20160804 清除計時值
                        }
                    }

                    if(IniConfig.bL11_7ATCUseMaxSurgeAlarm==true)               //Ifor 20160718 修改 ATC2.0 [L11_7]Alarm
                    {
                        if((ATCNowTemp[i]>=(SetATCTemp+IniConfig.iATCMaxSurgeAlarm)) ||
                           (ATCNowTemp[i]<=(SetATCTemp-IniConfig.iATCMaxSurgeAlarm)) )
                        {
                            str.sprintf("CH%02d" , i+1);
                            ShowErrorMessage("WAR15302", 0, MMATC_Head, false, str);                                    //ATC Temperature alarm by [L11-7]
                        }
                    }

                    if(IniConfig.bL11_7ATCUseMaxSurgeAlarm==true &&             //Ifor 20150911 判斷是否超出最大突波允許值 [L11_7]功能
                       bATCHasAlarm[1]==false)                                  //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    {
                        if((ATCNowTemp[i]>=(SetATCTemp+IniConfig.iATCMaxSurgeAlarm)) ||
                           (ATCNowTemp[i]<=(SetATCTemp-IniConfig.iATCMaxSurgeAlarm)) )
                        {
                            if(bUseMaxPeakTiming==true)                         //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
                            {
                                if(bATCMaxFirstIn[i]==false)                    //Ifor 20150911 : 判斷是否為新事件發生
                                {
                                    ATCMaxAlarmCheck[i].SetSecAndOn(IniConfig.iATCMaxAlarmContinuous);                  //Ifor 20160804 設定[L11_6]計時時間 & 開始計時
                                    bATCMaxFirstIn[i] = true;
                                    fMain->LabHisi_Set->Caption = IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°";            //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                }
                                else
                                {
                                    if(ATCMaxAlarmCheck[i].Off())
                                    {
                                        bATCHasAlarm[1]=true;
                                        fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°"+IntToStr(IniConfig.iATCMaxAlarmContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                        str.sprintf("CH%02d", i+1);
                                        ShowErrorMessage("WAR15302", 0, MMATC_Head, false, str);                        //ATC Temperature alarm by [L11-6]
                                        bATCMaxFirstIn[i] = false;
                                    }
                                }
                            }
                            else
                            {
                                bATCHasAlarm[1]=true;                           //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°"+IntToStr(IniConfig.iATCMaxAlarmContinuous) +"s";          //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                str.sprintf("CH%02d", i+1);
                                ShowErrorMessage("WAR15302", 0, MMATC_Head, false, str);                                //ATC Temperature alarm by [L11-7]
                            }
                        }
                        else
                        {
                            bATCMaxFirstIn[i] = false;
                            ATCMaxAlarmCheck[i].Pause();
                            ATCMaxAlarmCheck[i].Clear();
                        }
                    }
                }
                else
                {
                    bATCHasAlarm[0]=false;                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    bATCHasAlarm[1]=false;                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    bATCHasAlarm[2]=false;                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                }

                if(ATCNowTemp[i]<=0 ||                                          // Error
                   ATCNowTemp[i]>=999)                                          //Ifor 20151029 ATC2.0 新增 sensor 異常 or 損壞警告
                {
                    ATCPtr[i]->Color=clRed;
                    ATCPtr[i]->Caption="Error";
                }
                else
                {
                    if(ATCNowTemp[i]>(SetATCTemp+iTempRange))                   // OVER
                    {
                        ATCPtr[i]->Color=(TColor)0x008000FF;
                        if(SystemStart &&                                       //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
                           bRunAutoClean==false &&                              //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
                           bCheckATCTemp==false &&                              //Ifor 20230503 add:Start 後的Temp Wait 不報警
                           bChangeTest_TempAlarm==false)                        //Ifor 20230504 add: 避免修改溫度後報警
                        {
                            if(Count[i]==0)
                            {
                                ShowATCThermoOverDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                                Count[i]++;
                            }

                            if(ShowATCThermoOverDelay[i].Off())                 //Steven 20140916 : [L11-4] ATC的最高上限溫度
                            {
                                if(bCheckOverAlarm[i]==true)
                                {
                                    Count[i]++;
                                    if(Count[i]>2)
                                    {
                                        if(ATCNowTemp[i]-OldATCTemp[i]>iTempRange)
                                        {
                                        }
                                        else
                                        {
                                            Count[i]=1;
                                        }
                                    }

                                    OldATCTemp[i]=ATCNowTemp[i];
                                    fMain->OverBelowRangeTemp(i+1, ATCNowTemp[i], SetATCTemp, iTempRange);              //Steven 20140617 : for 海思
                                    if(ATCNowTemp[i]>IniConfig.iATCTemperatureOverLimit)                                //Steven 20140916 : [L11-4] ATC的最高上限溫度
                                    {
                                        str.sprintf("CH%02d" , i+1);
                                        ShowErrorMessage("WAR15304", 0, MMATC_Head, false, str);                        //ATC temperature over error
                                        bCheckOverAlarm[i]=false;
                                        iCount[i]=0;
                                    }
                                    else if(Count[i]>2)                         // 2012.10.22 , Joye , ATC Main Mode
                                    {
                                        str.sprintf("CH%02d" , i+1);
                                        ShowErrorMessage("WAR15304", 0, MMATC_Head, false, str);                        //ATC temperature over error
                                        bCheckOverAlarm[i]=false;
                                        iCount[i]=0;
                                    }
                                    ShowATCThermoOverDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);          //Ifor 20160804 移至 ShowErrorMessage 下面
                                }
                            }
                        }
                    }
                    else if(ATCNowTemp[i]<(SetATCTemp-iTempRange))              // Below
                    {
                        ATCPtr[i]->Color=clYellow;
                        if(SystemStart &&                                       //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
                           bRunAutoClean==false &&                              //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
                           bCheckATCTemp==false &&                              //Ifor 20230503 add:Start 後的Temp Wait 不報警
                           bChangeTest_TempAlarm==false)                        //Ifor 20230504 add: 避免修改溫度後報警
                        {
                            if(Count[i]==0)
                            {
                                ShowATCThermoBelowDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                                Count[i]++;
                            }

                            if(ShowATCThermoBelowDelay[i].Off())
                            {
                                fMain->OverBelowRangeTemp(i+1, ATCNowTemp[i], SetATCTemp, iTempRange);                  //Steven 20140617 : for 海思
                                if(bCheckBelowAlarm[i]==true)
                                {
                                    bCheckBelowAlarm[i]=false;
                                    iCount[i]=0;
                                    str.sprintf("CH%02d" , i+1);
                                    ShowErrorMessage("WAR15305", 0, MMATC_Head, false, str);                            //ATC temperature below error
                                }
                                ShowATCThermoBelowDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);             //Ifor 20160804 移至 ShowErrorMessage 下面
                            }
                        }
                    }
                    else
                    {
                        Count[i]=0;
                        ATCPtr[i]->Color=(TColor)0x0025AB12;
                    }
                    str.sprintf("%5.2f", ATCNowTemp[i]);
                    ATCPtr[i]->Caption=str ;
                }

                if(Temperature.bUseReferTempSensor==true)                       //Ifor 20151014 ATC 第二組溫度顏色顯示
                {
                    if(ATCRefTemp[i]<=0 ||                                      //Ifor 20151029 ATC2.0 新增 Refer sensor 異常 or 損壞顏色警告
                       ATCRefTemp[i]>=999)                                      // Error
                    {
                        ATCReferPtr[i]->Color=clRed;
                        ATCReferPtr[i]->Caption="Error";
                    }
                    else
                    {
                        if(ATCRefTemp[i]>(ATCNowTemp[i]+iTempRange))            // OVER     //Ifor 20160718  Refer Sensor Over range SetATCTemp ==> ATCNowTemp
                            ATCReferPtr[i]->Color=(TColor)0x008000FF;
                        else if(ATCRefTemp[i]<(ATCNowTemp[i]-iTempRange))       // Below    //Ifor 20160718  Refer Sensor Below range SetATCTemp ==> ATCNowTemp
                            ATCReferPtr[i]->Color=clYellow;
                        else
                            ATCReferPtr[i]->Color=(TColor)0x0025AB12;
                        str.sprintf("%5.2f", ATCRefTemp[i]);
                        ATCReferPtr[i]->Caption=str;                            //Steven 20150108 : [L11-5] For海思使用兩組感溫
                    }
                }
                else
                {
                    ATCReferPtr[i]->Color   =clWhite;
                    ATCReferPtr[i]->Caption ="NA";                              //kevin 20191214 change  Off-Line->NA
                }
            }
            else
            {
                ATCPtr[i]->Color        =clWhite;
                ATCPtr[i]->Caption      ="NA";                                  //kevin 20191214 change  Off-Line->NA
                ATCReferPtr[i]->Color   =clWhite;                               //Steven 20150108 : [L11-5] For海思使用兩組感溫
                ATCReferPtr[i]->Caption ="NA";                                  //kevin 20191214 change  Off-Line->NA
            }
        }
        else
        {
            ATCPtr[i]->Color            =clWhite;
            ATCPtr[i]->Caption          ="NA";                                  //kevin 20191214 change  Off-Line->NA
            ATCReferPtr[i]->Color       =clWhite;                               //Steven 20150108 : [L11-5] For海思使用兩組感溫
            ATCReferPtr[i]->Caption     ="NA";                                  //kevin 20191214 change  Off-Line->NA
            aldATCPower->Value          =false;                                 //Ifor 20160621 ATC2.0 斷線&離線 關閉 顯示燈號
            aldATCChillerStatus->Value  =false;
        }
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ShowNewATCThermo(double SetATCTemp, double iTempRange, double &OldSetATCTemp)
{
    static int Count[ATC_HEAD_COUNT];
    static int iCount[ATC_HEAD_COUNT];                                          //pig 2012.02.12 KyecATC

    static bool bInitialfinish=false;
    static bool bCheckOverAlarm[ATC_HEAD_COUNT];                                //pig 2012.02.12 KyecATC
    static bool bCheckBelowAlarm[ATC_HEAD_COUNT];                               //pig 2012.02.12 KyecATC
    static bool bATCFirstIn[ATC_HEAD_COUNT];                                    //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCompareCFirstIn[ATC_HEAD_COUNT];                             //Ifor 20150925 : 兩組Sensor其中一組溫度第一次超出溫度預設
    static bool bATCTempAlwaysSameAlarm[ATC_HEAD_COUNT][2];                     //Ifor 20160716 : add 發生溫度連續相同Alarm
    static bool bATCSameTempFirstIn[ATC_HEAD_COUNT];                            //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCRefSameTempFirstIn[ATC_HEAD_COUNT];                         //Ifor 20150911 : 溫度第一次超出突波設定旗標
    static bool bATCMaxFirstIn[ATC_HEAD_COUNT];                                 //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
    static bool bATCHasAlarm[4];                                                //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
    static bool bATCOfsST=false;

    static double OldATCTemp[ATC_HEAD_COUNT];                                   //Ifor 20160115 上一筆ATC第一點溫度
    static double OldATCRefTemp[ATC_HEAD_COUNT];                                //Ifor 20160115 上一筆ATC第二點溫度

    if(ATC_SYSTEM!=eNewATCSystem)
        return;

    bool bUseMaxPeakTiming=false;                                               //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
    bool bShowTJTemp=false;                                                     //Ifor 20190328 : add 顯示TJ溫度
    bool bCanUseLowTemp=false;

    double iMaxTime=ATCInterfaceForm->iCheckSameTempTime;                       //Ifor 20160215 add 最大相同秒數
    double ATCNowTemp[ATC_HEAD_COUNT];
    double ATCRefTemp[ATC_HEAD_COUNT];
    AnsiString str;

    if(bInitialfinish==false)                                                   //Ifor 20160420 add
    {
        bInitialfinish=true;
        ZeroMemory(OldATCRefTemp, sizeof(OldATCRefTemp));
        ZeroMemory(Count, sizeof(Count));
        ZeroMemory(iCount, sizeof(iCount));
        ZeroMemory(bCheckOverAlarm, sizeof(bCheckOverAlarm));
        ZeroMemory(bCheckBelowAlarm, sizeof(bCheckBelowAlarm));
        ZeroMemory(bATCFirstIn, sizeof(bATCFirstIn));
        ZeroMemory(bATCompareCFirstIn, sizeof(bATCompareCFirstIn));
        ZeroMemory(bATCTempAlwaysSameAlarm, sizeof(bATCTempAlwaysSameAlarm));
        ZeroMemory(bATCSameTempFirstIn, sizeof(bATCSameTempFirstIn));
        ZeroMemory(bATCRefSameTempFirstIn, sizeof(bATCRefSameTempFirstIn));
        ZeroMemory(bATCMaxFirstIn, sizeof(bATCMaxFirstIn));                     //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
        ZeroMemory(bATCHasAlarm, sizeof(bATCHasAlarm));                         //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
        return;
    }

    if(ATC_InterfaceForm->IsConnect()==false ||
       Temperature.bATCActiveCooling==false ||
       bStartATCRun==false)                                                     //Ifor 20160830 Mark 整合ATC 按鍵功能 cbActiveNewATC->Checked ==> bStartATCRun
    {
        for(int i=0; i<iATC_Use_Heat_Count; i++)
        {
            if(bATC_EnablesChannel[i]==false)
            {
                ATCPtr[i]->Color        =clWhite;
                ATCPtr[i]->Caption      ="NA";                                  //kevin 20191214 change  Off-Line->NA
                ATCReferPtr[i]->Color   =clWhite;
                ATCReferPtr[i]->Caption ="NA";                                  //kevin 20191214 change  Off-Line->NA
            }
            else
            {
            #ifdef DEBUG_ATC
                ATC_InterfaceForm->dTC[i]=i+0.1;
            #endif
            }
//            #ifdef DEBUG_ATC
//                ATC_InterfaceForm->dTC[i]=i+0.1;
//            #else
//                ATCPtr[i]->Color        =clWhite;
//                ATCPtr[i]->Caption      ="NA";                                  //kevin 20191214 change  Off-Line->NA
//                ATCReferPtr[i]->Color   =clWhite;
//                ATCReferPtr[i]->Caption ="NA";                                  //kevin 20191214 change  Off-Line->NA
//            #endif
        }

        if(Temperature.bATCActiveCooling==false)                                //Steven 20250409 : ATC沒開不要下去叫
            return;
    }

    ZeroMemory(ATCNowTemp, sizeof(ATCNowTemp));
    ZeroMemory(ATCRefTemp, sizeof(ATCRefTemp));

    if(Temperature.bATCActiveCooling==true &&
      (ATC_InterfaceForm->IS_ATC33() ||
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61))
    {
        bCanUseLowTemp=true;
    }
    else
    {
        bCanUseLowTemp=false;
    }

    if(CosFunction.bATC32UseTJMode==true &&
      (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_32 ||
       ATC_InterfaceForm->IS_ATC33() ||
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60 ||                        //JerryYang 20250813 : add
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_61) ||
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70)                          //Steven 20230213 : 修正ATC7.0讀不到第二點感溫
    {
        bShowTJTemp=true;
    }
    else
    {
        bShowTJTemp=false;
    }

    if(bChangeTest_TempOffset!=0)
    {
        if(bATCOfsST==false)
        {
            bATCOfsST=true;
            ATCOFSDelay.SetSecAndOn(Temperature.iATC_OFS_ST);                   //Ifor 20241118 : 測試中變溫
        }

        if(ATCOFSDelay.Off())
        {
            fLotInfo->SetATCOffset(true, true);                                 //Ifor 20241118 : 測試中變溫
            bATCOfsST=false;
        }
    }
    else
    {
        bATCOfsST=false;
    }

    for(int i=0; i<iATC_Use_Heat_Count; i++)
    {
        if(TestIF.iTestMode==SingleSite &&
           Temperature.bMultiZoneEnable &&
           OldSetATCTemp!=0)                                                    //wei 20240617 Multi Zone
        {
            SetATCTemp=Temperature.dZoneTempSetting[i%4];
            str.sprintf("%1.1f_%1.1f_%1.1f_%1.1f",
                        Temperature.dZoneTempSetting[0],
                        Temperature.dZoneTempSetting[1],
                        Temperature.dZoneTempSetting[2],
                        Temperature.dZoneTempSetting[3]);
            palATCWorkingTemp->Caption=str.c_str();
            OldSetATCTemp=0;
        }

        ATCNowTemp[i]=int(ATC_InterfaceForm->dTC[i]*10.0);
        if(Tri_Temp_Machine==1)                                                 //Ztex 2024.03.19 Add 100 -> 1000
            ATCNowTemp[i]=ATCNowTemp[i]/1000.0;
        else
            ATCNowTemp[i]=ATCNowTemp[i]/100.0;                                  //JerryYang 20191021 取小數點第二位
        if(bShowTJTemp==true)
        {
            ATCRefTemp[i]=ATC_InterfaceForm->dTJ[i]/10.0;
        }
        else
        {
            ATCRefTemp[i]=ATC_InterfaceForm->dTC2[i]/10.0;
            if(ATCRefTemp[i]==0)
                ATCRefTemp[i]=999.9;
        }

        if(ATCNowTemp[i]==0)                                                    //Ifor 20160716 New ATC 溫度讀取到0度強制修改成999.9
            ATCNowTemp[i]=999.9;
    }

    if(Temperature.bEnableTJFunction)
    {
        bHasTjTemp=false;
        if(bShowTJTemp==true)
        {
            for(int i=0; i<iATC_Use_Heat_Count; i++)
            {
                if((ATCRefTemp[i]>0 && ATCRefTemp[i]<200))
                {
                    bHasTjTemp=true;                                            //JerryYang 20251124 : Tj control吃不同的溫度range
                }
            }
        }
    }

    for(int i=0; i<iATC_Use_Heat_Count; i++)
    {
        if(bATC_EnablesChannel[i]==false)
        {
//            if(iATC_Use_Heat_Count<=8)                                          //Ifor 20160507 add ATC QualSite
//            {
                ATCPtr[i]->Color        =clWhite;
                ATCPtr[i]->Caption      ="NA";                                  //kevin 20191214 change
                ATCReferPtr[i]->Color   =clWhite;
                ATCReferPtr[i]->Caption ="NA";
//            }
        }
        else
        {
            if(Temperature.bATCActiveCooling==true)                             //Ifor 20160215 Add 溫度判斷 Start
            {
                if(iMaxTime!=0 && SystemStart && iATCOnLine)
                {
                    if(OldATCTemp[i]==ATCNowTemp[i])                            //Ifor 20160804 更改溫度相同警示計時方式 Start //Ifor 20160816 add 溫度相同Alarm 於機台Run與ATC連線正常下才偵測
                    {
                        if(bATCSameTempFirstIn[i]==false)
                        {
                            ATCTempSameAlarmCheck[i].SetSecAndOn(iMaxTime);     //Ifor 20160804 設定計時時間 & 開始計時
                            bATCSameTempFirstIn[i]=true;
                        }
                        else
                        {
                            if(ATCTempSameAlarmCheck[i].Off())
                            {
                                #ifndef SOFT_SIMULTE
                                bATCTempAlwaysSameAlarm[i][0]=true;             //Ifor 20160716 設定ATC第一點溫度相同Alarm 旗標
                                str.sprintf("CH%02d" , i+1);
                                ShowErrorMessage("WAR15300", 0, MMATC_Head, false, str);                                //ATC temperature sensor always same
                                bATCSameTempFirstIn[i]=false;                   //Ifor 20160804 重新計時
                                #endif
                            }
                        }
                    }
                    else
                    {
                        OldATCTemp[i]=ATCNowTemp[i];
                        ATCTempSameAlarmCheck[i].Pause();                       //Ifor 20160804 暫停計時
                        ATCTempSameAlarmCheck[i].Clear();                       //Ifor 20160804 清除計時值
                        bATCSameTempFirstIn[i]=false;                           //Ifor 20160804 重新計時
                        bATCTempAlwaysSameAlarm[i][0]=false;                    //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
                    }                                                           //Ifor 20160804 更改溫度相同警示計時方式 End

                    if(Temperature.bUseReferTempSensor==true &&
                       bShowTJTemp==false)
                    {
                        if(OldATCRefTemp[i]==ATCRefTemp[i])                     //Ifor 20160804 更改第二點溫度相同警示計時方式 Start //Ifor 20160816 add 溫度相同Alarm 於機台Run與ATC連線正常下才偵測
                        {
                            if(bATCRefSameTempFirstIn[i]==false)
                            {
                                ATCRefTempSameAlarmCheck[i].SetSecAndOn(iMaxTime);                                      //Ifor 20160804 設定計時時間 & 開始計時
                                bATCRefSameTempFirstIn[i]=true;
                            }
                            else
                            {
                                if(ATCRefTempSameAlarmCheck[i].Off())
                                {
                                    bATCTempAlwaysSameAlarm[i][1]=true;         //Ifor 20160716 設定ATC第一點溫度相同Alarm 旗標
                                    str.sprintf("CH%02d" , i+1);
                                    ShowErrorMessage("WAR15306", 0, MMATC_NI, false, str);                              //ATC temperature sensor always same
                                    bATCRefSameTempFirstIn[i]=false;            //Ifor 20160804 重新計時
                                }
                            }
                        }
                        else
                        {
                            OldATCRefTemp[i]=ATCRefTemp[i];
                            ATCRefTempSameAlarmCheck[i].Pause();                //Ifor 20160804 暫停計時
                            ATCRefTempSameAlarmCheck[i].Clear();                //Ifor 20160804 清除計時值
                            bATCRefSameTempFirstIn[i]=false;                    //Ifor 20160804 重新計時
                            bATCTempAlwaysSameAlarm[i][1]=false;                //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
                        }                                                       //Ifor 20160804  更改第二點溫度相同警示計時方式 End
                    }
                }
                else                                                            //Ifor 20160921 重新計時
                {
                    bATCSameTempFirstIn[i]=false;                               //Ifor 20160804 重新計時
                    bATCRefSameTempFirstIn[i]=false;                            //Ifor 20160804 重新計時
                    if(OldATCTemp[i]!=ATCNowTemp[i])
                    {
                        bATCTempAlwaysSameAlarm[i][0]=false;                    //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
                    }

                    if(OldATCRefTemp[i]!=ATCRefTemp[i])
                    {
                        bATCTempAlwaysSameAlarm[i][1]=false;                    //Ifor 20160716 清除ATC第一點溫度相同Alarm 旗標
                    }
                }
            }                                                                   //Ifor 20160215 Add 溫度判斷 End

            if(SystemStart &&                                                   //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
               bRunAutoClean==false &&                                          //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
               bCheckATCTemp==false &&                                          //Ifor 20230503 add:Start 後的Temp Wait 不報警
               bChangeTest_TempAlarm==false)                                    //Ifor 20230504 add: 避免修改溫度後報警
            {
                if(Temperature.bUseReferTempSensor==true &&
                   bShowTJTemp==false &&                                        //Ifor 20160114 開啟[L11_5]第二點感溫Sensor
                   Temperature.bATCActiveCooling==true)                         //Steven 20251003 : Add Offline Don't Alarm
                {
                    if(bATCTempAlwaysSameAlarm[i][1]==true &&
                       OldATCRefTemp[i]==ATCRefTemp[i])                         //Ifor 20160716 add 當發生ATC溫度連續相同Alarm，Alarm解除後需再次判斷溫度，若相同須再次Alarm不可跑貨
                    {
                        str.sprintf("CH%02d" , i+1);
                        ShowErrorMessage("WAR15306", 0, MMATC_NI, false, str);  //ATC temperature refer sensor always same
                    }

                    if(IniConfig.bL11_8ATCUseTemperatureCompare==true && bATCHasAlarm[2]==false)                        //Ifor 20151029 : ATC 2.0 [L11_8]功能    //Ifor 20160718 修改 ATC2.0 [L11_8]Alarm //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                    {
                                                                                                //Ifor 20160804 更改 [L11-8] 兩點溫差三度三秒Alarm計時方式 Start
                        if((ATCNowTemp[i]-ATCRefTemp[i]>=IniConfig.iATCTemperatureOutside) ||
                           (ATCRefTemp[i]-ATCNowTemp[i]>=IniConfig.iATCTemperatureOutside) )
                        {
                            if(bATCompareCFirstIn[i]==false)                    //Ifor 20150911 : 判斷是否為新事件發生
                            {
                                ATCCompareAlarmCheck[i].SetSecAndOn(IniConfig.iATCTemperatureContinuous);               //Ifor 20160804 設定[L11_8]計時時間 & 開始計時
                                bATCompareCFirstIn[i]=true;
                                fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°";             //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                            }
                            else
                            {
                                if(ATCCompareAlarmCheck[i].Off())
                                {
                                    if((i<2 && bATC_SITE_2ND_CHECK[0]==true) ||
                                       (i>=2 && bATC_SITE_2ND_CHECK[1]==true))
                                    {
                                        bATCHasAlarm[2]=true;                   //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                        fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°"+IntToStr(IniConfig.iATCTemperatureContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                        str.sprintf("CH%02d", i+1);
                                        ShowErrorMessage("WAR15303", 0, MMATC_Head, false, str);                        //ATC Temperature alarm by [L11-8]
                                        bATCompareCFirstIn[i] = false;
                                    }
                                }
                            }
                        }
                        else
                        {
                            ATCCompareAlarmCheck[i].Pause();                    //Ifor 20160804 暫停計時
                            ATCCompareAlarmCheck[i].Clear();                    //Ifor 20160804 清除計時值
                            bATCompareCFirstIn[i]=false;
                        }                                                       //Ifor 20160804 更改 [L11-8] 兩點溫差三度三秒Alarm計時方式 End
                    }
                }

                if(bATCTempAlwaysSameAlarm[i][0]==true &&
                   OldATCTemp[i]==ATCNowTemp[i] &&                              //Ifor 20160716 add 當發生ATC溫度連續相同Alarm，Alarm解除後需再次判斷溫度，若相同須再次Alarm不可跑貨
                   Temperature.bATCActiveCooling==true)                         //Ztex 2025.02.25 Add Offline Don't Alarm
                {
                    #ifndef SOFT_SIMULTE
                    str.sprintf("CH%02d" , i+1);
                    ShowErrorMessage("WAR15300", 0, MMATC_Head, false, str);    //ATC temperature sensor always same
                    #endif
                }

                if(IniConfig.bL11_6ATCUseTemperatureOutsideAlarm==true &&       //Ifor 20160718 修改 New ATC [L11_6]Alarm
                   bTJControlMode==false &&                                     //Ifor 20190423 : add TJ控溫模式下不Alarm
                   bATCHasAlarm[0]==false)                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                {                                                               //Ifor 20150911 :判斷"現在溫度 >= 設定溫度 +設定突波溫度" || "現在溫度 <= 設定溫度 - 設定突波溫度" //20160407 與海思討論後統一為3度
                    if((ATCNowTemp[i]>=(SetATCTemp+IniConfig.iATCTemperatureOutside)) ||
                       (ATCNowTemp[i]<=(SetATCTemp-IniConfig.iATCTemperatureOutside)))
                    {
                        if(bATCFirstIn[i]==false)                               //Ifor 20150911 : 判斷是否為新事件發生
                        {
                            ATCOutsideAlarmCheck[i].SetSecAndOn(IniConfig.iATCTemperatureContinuous);                   //Ifor 20160804 設定[L11_6]功能計時時間 & 開始計時
                            bATCFirstIn[i] = true;
                            fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°";                 //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                        }
                        else
                        {
                            if(ATCOutsideAlarmCheck[i].Off())
                            {
                                if(ATC_InterfaceForm->HasAlarmMsg()==false)
                                {
                                    bATCHasAlarm[0]=true;                       //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                    fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCTemperatureOutside)+"°"+IntToStr(IniConfig.iATCTemperatureContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                    str.sprintf("CH%02d" , i+1);
                                    ShowErrorMessage("WAR15301", 0, MMATC_Head, false, str);                            //ATC Temperature alarm by [L11-6]
                                    bATCFirstIn[i] = false;
                                }
                            }
                        }
                    }
                    else
                    {
                        bATCFirstIn[i]=false;
                        ATCOutsideAlarmCheck[i].Pause();                        //Ifor 20160804 暫停計時
                        ATCOutsideAlarmCheck[i].Clear();                        //Ifor 20160804 清除計時值
                    }
                }
                else
                {
                    bATCFirstIn[i]=false;
                    ATCOutsideAlarmCheck[i].Pause();                            //Ifor 20160804 暫停計時
                    ATCOutsideAlarmCheck[i].Clear();                            //Ifor 20160804 清除計時值
                }

                if(IniConfig.bL11_7ATCUseMaxSurgeAlarm==true &&                 //Ifor 20160718 修改 New ATC [L11_7]Alarm
                   bTJControlMode==false &&                                     //Ifor 20190423 : add TJ控溫模式下不Alarm
                   bATCHasAlarm[1]==false)                                      //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                {
                    if((ATCNowTemp[i]>=(SetATCTemp+IniConfig.iATCMaxSurgeAlarm)) ||
                       (ATCNowTemp[i]<=(SetATCTemp-IniConfig.iATCMaxSurgeAlarm)) )
                    {
                        if(ATC_InterfaceForm->HasAlarmMsg()==false)
                        {
                            if(bUseMaxPeakTiming==true)                         //Ifor 20200803 add:Hisi V2.4 最大峰值Alarm計時
                            {
                                if(bATCMaxFirstIn[i]==false)                    //Ifor 20150911 : 判斷是否為新事件發生
                                {
                                    ATCMaxAlarmCheck[i].SetSecAndOn(IniConfig.iATCMaxAlarmContinuous);                  //Ifor 20160804 設定[L11_6]計時時間 & 開始計時
                                    bATCMaxFirstIn[i]=true;
                                    fMain->LabHisi_Set->Caption = IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°";            //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                }
                                else
                                {
                                    if(ATCMaxAlarmCheck[i].Off())
                                    {
                                        bATCHasAlarm[1]=true;                   //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                                        fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°"+IntToStr(IniConfig.iATCMaxAlarmContinuous) +"s";  //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                        str.sprintf("CH%02d", i+1);
                                        ShowErrorMessage("WAR15302", 0, MMATC_Head, false, str);                        //ATC Temperature alarm by [L11-6]
                                        bATCMaxFirstIn[i]=false;
                                    }
                                }
                            }
                            else
                            {
                                bATCHasAlarm[1]=true;
                                fMain->LabHisi_Set->Caption=IntToStr(IniConfig.iATCMaxSurgeAlarm)+"°"+IntToStr(IniConfig.iATCMaxAlarmContinuous) +"s";          //Ifor 20200803 add:HisiV2.4 溫度Alarm狀態顯示
                                str.sprintf("CH%02d", i+1);
                                ShowErrorMessage("WAR15302", 0, MMATC_Head, false, str);                                //ATC Temperature alarm by [L11-7]
                            }
                        }
                    }
                    else
                    {
                        bATCMaxFirstIn[i] = false;
                        ATCMaxAlarmCheck[i].Pause();
                        ATCMaxAlarmCheck[i].Clear();
                    }
                }
            }
            else
            {
                bATCHasAlarm[0]=false;                                          //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                bATCHasAlarm[1]=false;                                          //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
                bATCHasAlarm[2]=false;                                          //Ifor 20200803 add:ATC 溫度異常僅Alarm 一次
            }

            if((ATCNowTemp[i]<=0 || ATCNowTemp[i]>=999) &&                      // Error
               bCanUseLowTemp==false)                                           //Ifor 20151029 ATC2.0 新增 sensor 異常 or 損壞警告
            {
//                if(iATC_Use_Heat_Count<=8)                                      //Ifor 20160507 add ATC QualSite
//                {
                    ATCPtr[i]->Color=clRed;
                    ATCPtr[i]->Caption="Error";
//                }
            }
            else
            {
                if(ATCNowTemp[i]>(SetATCTemp+iTempRange))                       // OVER
                {
//                    if(iATC_Use_Heat_Count<=8)                                  //Ifor 20160507 add ATC QualSite
                        ATCPtr[i]->Color=(TColor)0x008000FF;

                    if(SystemStart &&                                           //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
                       bRunAutoClean==false &&                                  //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
                       bCheckATCTemp==false &&                                  //Ifor 20230503 add:Start 後的Temp Wait 不報警
                       bChangeTest_TempAlarm==false)                            //Ifor 20230504 add: 避免修改溫度後報警
                    {
                        if(Count[i]==0)
                        {
                            ShowATCThermoOverDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                            Count[i]++;
                        }

                        if(ShowATCThermoOverDelay[i].Off())
                        {
                            if(bCheckOverAlarm[i]==true)
                            {
                                Count[i]++;

                                if(Count[i]>2)
                                {
                                    if(ATCNowTemp[i]-OldATCTemp[i]>iTempRange)
                                    {
                                        //ATCInterfaceForm->SetRunATC(false);
                                    }
                                    else
                                    {
                                        Count[i]=1;
                                    }
                                }

                                OldATCTemp[i]=ATCNowTemp[i];
                                fMain->OverBelowRangeTemp(i+1, ATCNowTemp[i], SetATCTemp, iTempRange);                  //Steven 20140617 : for 海思

                                if(ATCNowTemp[i]>IniConfig.iATCTemperatureOverLimit)                                    //Steven 20140916 : [L11-4] ATC的最高上限溫度
                                {
                                    str.sprintf("CH%02d" , i+1);
                                    ShowErrorMessage("WAR15304", 0, MMATC_Head, false, str);                            //ATC temperature over error
                                    bCheckOverAlarm[i]=false;
                                    iCount[i]=0;
                                }
                                else if(Count[i]>2)                             // 2012.10.22 , Joye , ATC Main Mode
                                {
                                    str.sprintf("CH%02d" , i+1);
                                    ShowErrorMessage("WAR15304", 0, MMATC_Head, false, str);                            //ATC temperature over error
                                    bCheckOverAlarm[i]=false;
                                    iCount[i]=0;
                                }
                                ShowATCThermoOverDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);              //Ifor 20160804 移至 ShowErrorMessage 下面
                            }
                        }
                    }
                }
                else if(ATCNowTemp[i]<(SetATCTemp-iTempRange))                  // Below
                {
//                    if(iATC_Use_Heat_Count<=8)                                  //Ifor 20160507 add ATC QualSite
                    ATCPtr[i]->Color=clYellow;

                    if(SystemStart &&                                           //Ifor 20150911 : 機台在跑的時候才檢查ATC溫度過低
                       bRunAutoClean==false &&                                  //Ifor 20160914 add 執行 Auto Clean 不偵測溫度Alarm
                       bCheckATCTemp==false &&                                  //Ifor 20230503 add:Start 後的Temp Wait 不報警
                       bChangeTest_TempAlarm==false)                            //Ifor 20230504 add: 避免修改溫度後報警
                    {
                        if(Count[i]==0)
                        {
                            ShowATCThermoBelowDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);
                            Count[i]++;
                        }

                        if(ShowATCThermoBelowDelay[i].Off())
                        {
                            fMain->OverBelowRangeTemp(i+1, ATCNowTemp[i], SetATCTemp, iTempRange);                      //Steven 20140617 : for 海思
                            if(bCheckBelowAlarm[i]==true)
                            {
                                bCheckBelowAlarm[i]=false;
                                iCount[i]=0;
                                str.sprintf("CH%02d" , i+1);
                                ShowErrorMessage("WAR15305", 0, MMATC_Head, false, str);                                //ATC temperature below error
                                ShowATCThermoBelowDelay[i].SetSecAndOn(IniConfig.dATCTemperatureCheckTime);             //Ifor 20160804 移至 ShowErrorMessage 下面
                            }
                        }
                    }
                }
                else
                {
                    Count[i]=0;
//                    if(iATC_Use_Heat_Count<=8)                                  //Ifor 20160507 add ATC QualSite
                    ATCPtr[i]->Color=(TColor)0x0025AB12;
                }

//                if(iATC_Use_Heat_Count<=8)
//                {
                    str.sprintf("%5.2f", ATCNowTemp[i]);
                    ATCPtr[i]->Caption=str ;
//                }
            }

            if(Temperature.bUseReferTempSensor==true &&                         //Ifor 20151014 ATC 第二組溫度顏色顯示
               Temperature.bATCActiveCooling==true)                             //Steven 20251003 : Add Offline Don't Alarm
            {                                                                   //Ifor 20151029 ATC2.0 新增 Refer sensor 異常 or 損壞顏色警告
                if((ATCRefTemp[i]<=0 || ATCRefTemp[i]>=999) && bShowTJTemp==false && bCanUseLowTemp==false)
                {
//                    if(iATC_Use_Heat_Count<=8)                                  //Ifor 20160507 add ATC QualSite
//                    {
                        ATCReferPtr[i]->Color  =clRed;
                        ATCReferPtr[i]->Caption="Error";
//                    }
                }
                else
                {
//                    if(iATC_Use_Heat_Count<=8)                                  //Ifor 20160507 add ATC QualSite
//                    {
                        if(bShowTJTemp==true)
                        {
                            ATCReferPtr[i]->Color=(TColor)0x0025AB12;
                        }
                        else
                        {
                            if(ATCRefTemp[i]>(ATCNowTemp[i]+iTempRange))        // OVER     //Ifor 20160718  Refer Sensor Over range SetATCTemp ==> ATCNowTemp
                                ATCReferPtr[i]->Color=(TColor)0x008000FF;
                            else if(ATCRefTemp[i]<(ATCNowTemp[i]-iTempRange))   // Below    //Ifor 20160718  Refer Sensor Below range SetATCTemp ==> ATCNowTemp
                                ATCReferPtr[i]->Color=clYellow;
                            else
                                ATCReferPtr[i]->Color=(TColor)0x0025AB12;
                        }
                        str.sprintf("%5.2f", ATCRefTemp[i]);
                        ATCReferPtr[i]->Caption=str;                            //Steven 20150108 : [L11-5] For海思使用兩組感溫
//                    }
                }
            }
            else
            {
//                if(iATC_Use_Heat_Count<=8)
//                {
                    ATCReferPtr[i]->Color   =clWhite;
                    ATCReferPtr[i]->Caption ="NA";
//                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::Timer2Timer(TObject *Sender)
{
    static bool bOldFTPAutomation=false;
    static int iTempSetTimer=0;                                                 //jimmychiu 2021
    if(InitialOK==false)                                                        //Steven 20160912 : Add InitialOK in Timer
        return;

    if(CUSTOMER_CODE==CC_TSMC_TAINAN && bSecsGemDownloadFTP==true)              //wei 20170119 (Steven) DownLoad 沒有馬上按掉會Time Out
    {
        if(IniConfig.bEnable_SECS_GEM==true)
            ShowMyMessage("DOWNLOAD_RECIPE_BY_EA Finish!!");
    }

    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //Frank 20171030 (Steven) add Clear Barcode List新增權限 Xilinx
    {
        btClearBarcodeList->Visible=(AccessLevel>=iDefHonPrecLevel)?true:false;
    }

    if(fAGV->IsSPIL_AMR())
    {
        labLoaderBundleID->Caption=asBundleTrayID[ePortLoader];
        lblLoaderCarBundleID->Caption=asBundleTrayID[ePortEmpty];
    }

    bSecsGemDownloadFTP=false;

    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction==true && bOCRConnectTest==true)                                 //wei 20160613 ocr連線測試
    {
        bOCRConnectTestCount++;

        if(bOCRConnectTestCount>3)                                              //wei 20161028 ocr連線測試
        {
            if(bOCRConnectOK)
            {
                ShowMyMessage("OCR Connect Test OK");
                bSignIn=true;
                bOCRConnect=true;
            }
            else
            {
                ShowMyMessage("OCR Connect Test NG");
                bSignIn=false;
                bOCRConnect=false;
            }
            bOCRConnectTest=false;
            bOCRConnectTestCount=0;
        }
        else
        {
            return;
        }
    }

    lbOCRUseFile->Caption="";
    lbOCRNowFile->Caption="";

    lblAutoCount->Caption=iUnloaderTrayCountCal[0];
    lblAutoCount2->Caption=iUnloaderTrayCountCal[1];
    lblAutoCount3->Caption=iUnloaderTrayCountCal[2];

    if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction==true)
    {
        if(bSignIn)
        {
            Label41->Caption="Log In";
        }
        else
        {
            Label41->Caption="Log Out";
        }
        lbOCRUseFile->Caption=fMain->cbSetupFileName->Text;
        if(asCheckFileName!="")
            lbOCRNowFile->Caption=asCheckFileName;
        else
            lbOCRNowFile->Caption="";
    }
    else
    {
        Label41->Caption="Log Out";
        lbOCRUseFile->Caption=fMain->cbSetupFileName->Text;
        lbOCRNowFile->Caption="";
    }
//OCR Function
    if(bGetLotIDFormTester)
    {
        fLotInfo->palHandlerwithTester->Caption="Connection";
        fLotInfo->palHandlerwithTester->Color=clLime;
        lblTester_LotID->Caption= fOCR->sTesterLotId;
    }
    else
    {
        fLotInfo->palHandlerwithTester->Caption="No Connection";
        fLotInfo->palHandlerwithTester->Color=clRed;
        lblTester_LotID->Caption=fSetup->edOcrText->Text;
    }
//Auto ReTest
    ts_AutoRetestMonitor->TabVisible=(CosFunction.bUseSCKART==false &&
                                      USE_AUTO_RETEST==eartInstall &&
                                      ((IniConfig.bA10_AutoReTest && (CosFunction.bAutoRetestGPIBmode==false || CUSTOMER_CODE==CC_KYEC_XILINX)) || bAutoReTest_ART));  //kevin 20150601);       //wei 20150331 打開功能就顯示        //Steven 20161201 : For SCK 93K ART  //Frank 20161212 (Jou) For Xilinx 打開功能顯示
    tsOCRBarCode        ->TabVisible=(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR);                               //wei 20150720  打開功能就顯示

    if(ATC_SYSTEM==eWinWay)                                                     //jimmychiu 2021
        ATC_WinWay->TabVisible=true;
    else
        ATC_WinWay->TabVisible=false;

    //jimmychiu 2021
    if(ATC_SYSTEM==eWinWay && Temperature.bATCActiveCooling==true)
    {
        //check winway connect
        fWinway->OpenCommPort();
        //set temp

        double dbGetAtcSetTemp=0.0;
        double dbSetATCTemp=0.0;
        bool bHasErr=false;

        if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
            dbSetATCTemp=Temperature.fWorkTemperBase;
        else
            dbSetATCTemp=IniConfig.dATCAmbientTemperature;
        iTempSetTimer++;
//        fWinway->SetST();
        //send get pv command
        for(int i=0;i<4;i++)
        {
            //Ifor 20220104 add:
            dbGetAtcSetTemp=fWinway->arrATC_Site[i]->GetST();
            if(dbSetATCTemp!=dbGetAtcSetTemp || iTempSetTimer>=3)
            {
                fWinway->SetTempratureAll(dbSetATCTemp);
                iTempSetTimer=0;
            }

            if(SystemStart==true && fWinway->arrATC_Site[i]->iWinWaySendCount >=6)
            {
                fWinway->arrATC_Site[i]->WinwayCOM->StopComm();
                fWinway->arrATC_Site[i]->bCommConnect=false;
                fWinway->arrATC_Site[i]->SetPT((double)9999);
                fWinway->arrATC_Site[i]->iWinWaySendCount=0;
                bHasErr=true;
            }

//          ATCPtrWinWay[i]->Caption=FormatFloat(L"0.0" ,fWinway->arrATC_Site[i]->GetPT()) ;
          ATCPtrWinWay[i]->Caption=FormatFloat(L"0.0" ,fWinway->arrATC_Site[i]->GetPT_NoCommand()) ;
        }

        if(bHasErr)
        {
            ShowMyMessage("WinWay ATC System Connect Error!");
        }
    }

    if(CUSTOMER_CODE==CC_GIGAS ||                                               //Isaac 20210129 : FTP頁面，關閉功能要不顯示畫面，但視窗不要關掉
       IniConfig.bVTESTFunction==true)
    {
    }
    else
    {
        tsFTP               ->TabVisible=IniConfig.bEnableFTP;                  //Ifor 20180822 : Add 關閉ftp功能關閉顯示頁面
    }

    if(IniConfig.bVTESTFunction==true)
    {
        if(IniConfig.bEnableRms)
        {
            fLotInfo->Panel29->Caption="Server";
        }

        if(RunInfo.bLotStart==true &&                                           //RogerYang 20260424 : 陳永恆說不再需要，等趙坤鵬同意  //jou 20230203 : 無錫偉測 機台添加三小時送檢報警，從lot start時間開始計算
           SystemStart==true && iHome==0 &&
           fNote->fShow==false && MyMessageBox->fShow==false )
        {
            iProduceTimeCT++;
            if(iProduceTimeCT>=(3600*3))
            {
                int ret=ShowErrorMessage("WAR16123", K_SKIP , MMSystem);        //RogerYang 20251030 : 袁林要把Retry拿掉
                if(ret==K_RETRY)
                    iProduceTimeCT=9000;
                else
                    iProduceTimeCT=0;
            }
        }

        labConfigL04->Caption="[L04] Temperature range : " + AnsiString(IniConfig.iL04TemptureRange);
    }
    else
    {
        tsDeviceInfo        ->TabVisible=IniConfig.bEnableRms;                  //Ifor 20181029 : Add 關閉RMS功能關閉顯示頁面
    }
    tsTesterLog         ->TabVisible=(TestIF_File.iTestType==TCP_IP_MODE);
    grpBarcodeDisplayLotInfo->Visible=(TestIF_File.i2DIDFormat==eAMD);          //JerryYang 20200422 2DID format選項改用下拉選單 //Ifor 20200422 :add 2D主畫面顯示
    //KaiChen 20190530 ：Sigurd FTP Automation
    //==>
    ts_FTPAutomation    ->TabVisible=IniConfig.bA32EnableFTPAutomation;
    if(bOldFTPAutomation!=ts_FTPAutomation->TabVisible)
    {
        bOldFTPAutomation=ts_FTPAutomation->TabVisible;
        if(ts_FTPAutomation->TabVisible)
        {
            pgLotinfo->ActivePage=ts_FTPAutomation;
            fLotInfo->ShowInformation(false);
            fLotInfo->Height=150;
            fLotInfo->Width=150;
            sbTest->Top=50;
        }
        else
        {
            pgLotinfo->ActivePage=tsYieldMonitior;
            RefreshYieldMonitor();
        }
    }
    else
    {
        AdjtsYieldMonitiorSize();                                               //Steven 20221225 : 統一Lot Info尺寸調整
    }
    //<==
    //KaiChen 20190530 ：Sigurd FTP Automation
    //==> Eastsun 20260527 整合#027-2.MR.U4 show/hide BarcodeRecipe widget :KYEC
    if(TestIF_File.bEnableBarCode==true &&
       BAR_CODE_INSTALL==ebctUseCCDMode &&
       TestIF_File.bBarCodeMultiRecipe==true)
    {
        labBarcodeRecipe->Visible=true;
        edtBarcodeRecipe->Visible=true;
    }
    else
    {
        labBarcodeRecipe->Visible=false;
        edtBarcodeRecipe->Visible=false;
    }
    //<== Eastsun 20260527 #027-2.MR.U4
    spOCRCleanList->Visible=(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR && IniConfig.bCompareOCRData);           //KenHsieh 20220825 : 新增OCR比對功能
    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20241104 : 支援2DID白名單功能
    {
        if(TestIF_File.b2DIDAllowList)
        {
            if(RunInfo.bLotStart==false)
            {
                if(AccessLevel>=iDefEngineerLevel ||
                   SPIL_FOR_QLE==1)                                             // KevinC 20250912 渠梁OP權限
                {
                    cbRunMode->Enabled=true;
                }
                else
                {
                    cbRunMode->Enabled=false;
                }
            }
        }
        else
        {
           cbRunMode->Enabled=true;
        }
    }
    ScanRefrigerantSystem();                                                    //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    fLotInfo->palAQLMode->Visible=IniConfig.bI52_bAQLSortMode;                  //Eastsun 20260520 整合//Ifor 20210713 add: AQL Sor tMode
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edDeviceNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
#ifndef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_AMKOR_China ||                                         //jou 2013-01-04 防止OP使用複製貼上的方式讀取工作檔
       CUSTOMER_CODE==CC_QUALCOMM)                                              //JerryYang 20170412 (Steven) add QUALCOMM
    {
        if(Button==mbRight || Button==mbLeft)
        {
            Clipboard()->Clear();
        }
    }
#endif
    if((CUSTOMER_CODE==CC_SCC && AccessLevel<iDefHonPrecLevel))                 // ||
//       CUSTOMER_CODE==CC_ETRENDTECH)                                          //Steven 20230302 : add for 逸昌
    {
        edDeviceName->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input Device Name:");
        edDeviceName->Text=sBarcodeID;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbbDeviceNameChange(TObject *Sender)
{
    edDeviceName->Text=cbbDeviceName->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::pgLotinfoChange(TObject *Sender)
{
    AnsiString sConfigPath=AuthPath+"Security_new.def";
    if(pgLotinfo->ActivePage==tsSelection)
    {
        chkTempOffset->Checked  =CheckAndReadIniData(sConfigPath, "Network", "Temp Offset",     true);
        chkContactHigh->Checked =CheckAndReadIniData(sConfigPath, "Network", "Contact High",    false);
        chkContactForce->Checked=CheckAndReadIniData(sConfigPath, "Network", "Contact Force",   true);
        chkContactMode->Checked =CheckAndReadIniData(sConfigPath, "Network", "Contact Mode",    true);
        chkHotPlate->Checked    =CheckAndReadIniData(sConfigPath, "Network", "HotPlate",        false);
        chkLoadUnload->Checked  =CheckAndReadIniData(sConfigPath, "Network", "Load Unload",     false);
        chkSpeedSetting->Checked=CheckAndReadIniData(sConfigPath, "Network", "Speed Setting",   true);
        chkShuttleMode->Checked =CheckAndReadIniData(sConfigPath, "Network", "Shuttle Mode",    false);

        chkTestMode->Checked    =CheckAndReadIniData(sConfigPath, "Network", "Test Mode",       true);
        chkBinasgn->Checked     =CheckAndReadIniData(sConfigPath, "Network", "Binasgn",         true);
        chkBinasgnOff->Checked  =CheckAndReadIniData(sConfigPath, "Network", "BinasgnOff",      true);

        checkbAutoClean->Checked=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean",      true);                  //jou 20161122 Auto Clean 參數可以選擇是否需要上傳下載

        chkAutoCleanContactHeight->Checked=CheckAndReadIniData(sConfigPath, "Network", "Auto Clean Contact Height",      false);                                //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋

        cbBottom2DOffset->Checked =CheckAndReadIniData(sConfigPath, "Network", "Bottom 2D Offset",    false);           //JerryYang 20201122 Bottom 2D offset不覆蓋

        chkART->Visible=(CosFunction.bUseSCKART);
        chkART_RTCount->Visible=(CosFunction.bUseSCKART);
        if(CosFunction.bUseSCKART)
        {
            chkART->Checked     =CheckAndReadIniData(sConfigPath, "Network", "Auto Retest",     true);                  //Steven 20190918 : ART設定下載不覆蓋
            chkART_RTCount->Checked=CheckAndReadIniData(sConfigPath, "Network", "ART_RT_Count ", true);                 //Steven 20191101 : ART RT count不覆蓋
        }
        chkIndexHeatingMode->Checked=CheckAndReadIniData(sConfigPath, "Network", "Index Heat Mode", true);              //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式

        if(IniConfig.bVTESTFunction==true)                                      //RogerYang 20250314 偉測張冬冬要求 Stop Yield和Consecutive Fail功能修改為灰色,不可更改,默認開啟
        {
            chkStopYield->Checked           =true;
            chkConsecutiveFailure->Checked  =true;
            chkStopYield->Enabled           =false;
            chkConsecutiveFailure->Enabled  =false;
        }

        chkCleanCount->Checked=CheckAndReadIniData(sConfigPath, "Network", "Cleaning Count", checkbAutoClean->Checked);                                         //KenHsieh 20230518 : Auto Clean count不覆蓋
    }

    AdjtsYieldMonitiorSize();                                                   //Steven 20221225 : 統一Lot Info尺寸調整
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnSaveClick(TObject *Sender)
{
    AnsiString sConfigPath=AuthPath+"Security_new.def";
    WriteIniData(sConfigPath, "Network", "Temp Offset",     chkTempOffset->Checked);
    WriteIniData(sConfigPath, "Network", "Contact High",    chkContactHigh->Checked);
    WriteIniData(sConfigPath, "Network", "Contact Force",   chkContactForce->Checked);
    WriteIniData(sConfigPath, "Network", "Contact Mode",    chkContactMode->Checked);
    WriteIniData(sConfigPath, "Network", "HotPlate",        chkHotPlate->Checked);
    WriteIniData(sConfigPath, "Network", "Load Unload",     chkLoadUnload->Checked);
    WriteIniData(sConfigPath, "Network", "Speed Setting",   chkSpeedSetting->Checked);
    WriteIniData(sConfigPath, "Network", "Shuttle Mode",    chkShuttleMode->Checked);
    WriteIniData(sConfigPath, "Network", "Test Mode",       chkTestMode->Checked);
    WriteIniData(sConfigPath, "Network", "Binasgn",         chkBinasgn->Checked);
    WriteIniData(sConfigPath, "Network", "BinasgnOff",      chkBinasgnOff->Checked);
    WriteIniData(sConfigPath, "Network", "Auto Clean",      checkbAutoClean->Checked);                                  //jou 20161122 Auto Clean 參數可以選擇是否需要上傳下載
    WriteIniData(sConfigPath, "Network", "Bottom 2D Offset",cbBottom2DOffset->Checked);                                 //JerryYang 20201122 Bottom 2D offset不覆蓋
    WriteIniData(sConfigPath, "Network", "Cleaning Count",  chkCleanCount->Checked);                                    //KenHsieh 20230518 : Auto Clean count不覆蓋
    WriteIniData(sConfigPath, "Network", "Auto Clean Contact Height", chkAutoCleanContactHeight->Checked);              //JerryYang 20241019 : 矽品二林 耀仁要求Auto clean高度可選擇不覆蓋
    if(CosFunction.bUseSCKART)
        WriteIniData(sConfigPath, "Network", "Auto Retest", chkART->Checked);   //Steven 20190918 : ART設定下載不覆蓋
    WriteIniData(sConfigPath, "Network", "Heat Mode",       chkIndexHeatingMode->Checked);                              //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式

    if(IniConfig.bEnableRms==true)
        pgLotinfo->ActivePage=tsDeviceInfo;

    if(IniConfig.bVTESTFunction==true)
    {
        WriteIniData(sConfigPath, "Network", "Stop Yield",      chkStopYield->Checked);
        WriteIniData(sConfigPath, "Network", "Consecutive Failure",      chkConsecutiveFailure->Checked);
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ConnectNetDeviceS()                                             //連結本機網路磁碟機(S:)                  //Steven 20140306 : 使用網路硬碟
{
    if(DirectoryExists(IniConfig.asAlarmLocalDirectory))
    {
        return;
    }

    int iTRV;
    AnsiString asTryStatus="NetOK";
    AnsiString asTempAdddr;
    NETRESOURCE nr;

    ZeroMemory(&nr, sizeof(NETRESOURCE));
    nr.dwScope=RESOURCE_GLOBALNET;
    nr.dwType=RESOURCETYPE_ANY;
    nr.lpLocalName=IniConfig.asAlarmLocalDirectory.c_str();
    asTempAdddr=IniConfig.asAlarmRemoteDirectory;
    nr.lpRemoteName=asTempAdddr.c_str();
    nr.lpComment=NULL;
    nr.lpProvider=NULL;
    iTRV=WNetAddConnection2(&nr,(LPSTR)NULL,(LPSTR)NULL,CONNECT_UPDATE_PROFILE);
    if(iTRV!=NO_ERROR)
    {
        switch (iTRV)
        {
            case ERROR_ACCESS_DENIED             : {    asTryStatus="NetERROR_ACCESS_DENIED            "; break;}
            case ERROR_ALREADY_ASSIGNED          : {    asTryStatus="NetERROR_ALREADY_ASSIGNED         "; break;}
            case ERROR_BAD_DEV_TYPE              : {    asTryStatus="NetERROR_BAD_DEV_TYPE             "; break;}
            case ERROR_BAD_DEVICE                : {    asTryStatus="NetERROR_BAD_DEVICE               "; break;}
            case ERROR_BAD_NET_NAME              : {    asTryStatus="NetERROR_BAD_NET_NAME             "; break;}
            case ERROR_BAD_PROFILE               : {    asTryStatus="NetERROR_BAD_PROFILE              "; break;}
            case ERROR_BAD_PROVIDER              : {    asTryStatus="NetERROR_BAD_PROVIDER             "; break;}
            case ERROR_BUSY                      : {    asTryStatus="NetERROR_BUSY                     "; break;}
            case ERROR_CANCELLED                 : {    asTryStatus="NetERROR_CANCELLED                "; break;}
            case ERROR_CANNOT_OPEN_PROFILE       : {    asTryStatus="NetERROR_CANNOT_OPEN_PROFILE      "; break;}
            case ERROR_DEVICE_ALREADY_REMEMBERED : {    asTryStatus="NetERROR_DEVICE_ALREADY_REMEMBERED"; break;}
            case ERROR_EXTENDED_ERROR            : {    asTryStatus="NetERROR_EXTENDED_ERROR           "; break;}
            case ERROR_INVALID_PASSWORD          : {    asTryStatus="NetERROR_INVALID_PASSWORD         "; break;}
            case ERROR_NO_NET_OR_BAD_PATH        : {    asTryStatus="NetERROR_NO_NET_OR_BAD_PATH       "; break;}
            case ERROR_NO_NETWORK                : {    asTryStatus="NetERROR_NO_NETWORK               "; break;}
            default                              : {    asTryStatus="NetERROR "+IntToStr(iTRV);           break;}
        }
        ShowMyMessage(asTryStatus, "連結網路硬碟失敗");
    }
}
//---------------------------------------------------------------------------
bool TfLotInfo::N23UseLotInfoFile()
{
    AnsiString strPath, str4;
    if(IniConfig.bN23UseLotInfoFile)                                            //Steven 20240829 : Lot info從檔案讀取
    {
        strPath=IncludeTrailingPathDelimiter(IniConfig.sN23LotInfoPath)+edtSysLotID->Text+AnsiString(".txt");
        if(FileExists(strPath))
        {
            mmo2DLotInfo->Lines->LoadFromFile(strPath);
            for(int i=0; i<mmo2DLotInfo->Lines->Count; i++)
            {
                str4=mmo2DLotInfo->Lines->Strings[i];
                if(str4.AnsiPos("CUST_LOT_ID:")!=0)
                {
                    edtCusLotID->Text=str4.SubString(str4.AnsiPos(":")+1, str4.Length());
                }
                else if(str4.AnsiPos("FAMILY:")!=0)
                {
                    edtCusDevGrp->Text=str4.SubString(str4.AnsiPos(":")+1, str4.Length());
                }

                if(str4.AnsiPos("CURR_DEVICE:")!=0)
                {
                    edtDevice->Text=str4.SubString(str4.AnsiPos(":")+1, str4.Length());
                }
            }
        }
        else
        {
            ShowMyMessage("The Lot info for 2DID sorting is missing", "找不到2DID sorting用的Lot info");
            return false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbSECSLotStartClick(TObject *Sender)
{
    if(IniConfig.bVTESTFunction==true &&                                        //RogerYang 20260327 : VTENGrecipe/VTEErecipe lot號開批需要OP以上權限
        fMesSystem->CheckVTENGmode(fLotInfo->edtSysLotID->Text)==true)
    {
        if(DoPassword()==false)
        {
            return;
        }
    }

    bReadLotInfoFromART=false;
    int ret=0, ret2=0, iBin, iIndex=0, iPos=0, temp;
    static int iCount;

    AnsiString str1="", str2="", str3="", str4="", str5="", strPath="", str="", strSort="", strLotID="",sID="", sFileName="";
    char cSort[1024];
    AnsiString sDeviceID="",sBin="";
    memset(cSort, '\0', sizeof(cSort));
    ZeroMemory(iByBinCnt, sizeof(iByBinCnt));
    ZeroMemory(iExceptAutoCnt, sizeof(iExceptAutoCnt));
    mmo2DLotInfo->Clear();
    dtStartLot=Now();

    //AI(ht9045-v899) 20260528: block PTI operator Lot Start while tester is offline
    if(CUSTOMER_CODE==CC_PTI &&
       OFFLINE_ALARM &&
       LastSet.iTester==OFF_LINE &&
       AccessLevel<iDefSupervisorLevel)
    {
        WritePTILotStartTrace(this, "OfflineOperatorBlocked", "Reason=AccessLevelBelowSupervisor");
        ShowMyMessage("Please check the tester mode is connected"); //AI(ht9045-v899) 20260529: clarify PTI offline tester mode prompt
        return;
    }

    //AI(ht9045-v899) 20260525: normalize PTI RunMode before local validation blocks TesterTCP Lot Start
    NormalizePTIRunModeBeforeLotStart(this);

    if(CosFunction.bSortingBy2DList==true &&
       LastSet.iTester==_2D_SORT &&
       TestIF_File.bSortingBy2DIDList==true)
    {
        str2.sprintf("%s\\SortBy2DID_%s.csv", "D:\\HT9045_Log\\2D_SortList", sTotalLotID);
        if(HasICUnderMachine() && HasAnyICInMachine())                          //JerryYang 20240111 : add
        {
        }
        else
        {
            if(FileExists(str2))                                                //Steven 20160505 : 加上保護, 不然開程式會跳Error
            {
                DeleteFile(str2);
            }
        }

        if(N23UseLotInfoFile()==false)
            return;

        if(IniConfig.iN23DownloadMethod==3)                                     //手動
        {
            if(rgSort2DID->ItemIndex==1)                                        //手動選檔案
                str2=edSort2DIDBinFile->Text;
        }
        else if(IniConfig.iN23DownloadMethod==2)                                //SECS/GEM
        {
        }
        else if(IniConfig.iN23DownloadMethod==1)                                //Net Drive
        {
            //----------------------
            //開始下載檔案
            //----------------------
            do
            {
                if(!DirectoryExists("D:\\HT9045_Log\\2D_SortList\\"))
                {
                    MyForceDirectories("D:\\HT9045_Log\\2D_SortList\\");
                }
                str1.sprintf("%sSortBy2DID_%s.csv", IncludeTrailingPathDelimiter(IniConfig.sN23DownloadDrivePath), edtSysLotID->Text);
                str2.sprintf("%s\\SortBy2DID_%s.csv", "D:\\HT9045_Log\\2D_SortList", edtSysLotID->Text);

                ret=CopyFile(str1.c_str(), str2.c_str(), false);                //複製一份新的
                ::Sleep(100);

                if(ret==0)
                {
                    ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, str1);                                //下載失敗，重試
                    if(ret2==K_SKIP)
                        break;
                }
                else
                {
                    ret2=0;
                }
            }
            while(ret2==1);
        }
        else                                                                    //FTP
        {
            strPath=IniConfig.cN23FtpDownloadPath;
            str1.sprintf("%s.csv", edtSysLotID->Text);
            fFTPClient->Download_2DSortingList(strPath, str1);
            str2.sprintf("%s\\%s.csv", "D:\\RMS", edtSysLotID->Text);
        }

        if(IniConfig.iN23DownloadMethod==2)                                     //SECS/GEM
        {
        }
        else if(FileExists(str2))                                               //Steven 20160505 : 加上保護, 不然開程式會跳Error
        {
            fSCKART->sLotID=edtSysLotID->Text;
            fBarCode->list2DSorting->Clear();
            fBarCode->list2DSorting->LoadFromFile(str2);
//            fBarCode->list2DSorting->Sort();

            iCount=0;
            sl2DMappingLog->Clear();                                            //JerryYang 20230322 : add 2D mapping result
            sl2DMappingLog->Add("2D Code, Binning, ECID, Device, Lot No., VS-shuttle, VS-unload, RC1-shuttle, RC1-unload, RC2-shuttle, RC2-unload");

            for(int i=0; i<fBarCode->list2DSorting->Count; i++)
            {
                if(CUSTOMER_CODE==CC_AMKOR_Korea)
                {
                    str3=fProductionInfo->GetCSVLineData(5, fBarCode->list2DSorting->Strings[i]);
                    str4=fProductionInfo->GetCSVLineData(11, fBarCode->list2DSorting->Strings[i]);

                    if(str3=="" || str3=="Unit_ID")
                        continue;

                    iBin=atoi(str4.c_str())-48;
                    str5.sprintf("%s=%d", str3, iBin);
                    fBarCode->list2DSorting->Strings[iCount]=str5;
                    iCount++;
                }
                else if(CUSTOMER_CODE==CC_JCET)                                 //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
                {
                }
                else
                {
                    str=fBarCode->list2DSorting->Strings[i];
                    str4=StringReplace(str, '"', "", TReplaceFlags()<<rfReplaceAll);

                    sDeviceID="";                                               //JerryYang 20230322 : add 2D mapping result
                    sBin="";
                    if(str4.Pos(",")>0)
                    {
                        sDeviceID=str4.SubString(1, str4.Pos(",")-1);
                        sBin=str4.SubString(str4.Pos(",")+1, str4.Length());
                    }

                    if(sDeviceID!="" && sBin!="")
                    {
                        sl2DMappingLog->Add(sDeviceID+","+sBin+",,"+edtDevice->Text+","+edtCusLotID->Text+",,,,,,");
                    }
                    str5=StringReplace(str4, " ", "_", TReplaceFlags()<<rfReplaceAll);

                    fBarCode->s2DSorting->CommaText=str5;
                    str1=StringReplace(str, ",", "=", TReplaceFlags()<<rfReplaceAll);
                    strSort=StringReplace(str1, '"', "", TReplaceFlags()<<rfReplaceAll);
                    fBarCode->list2DSorting->Strings[i]=strSort;

                    int iBin=StrToIntDef(fBarCode->s2DSorting->Strings[1], iTestBinCount);
                    if(iBin>=0 && iBin<iTestBinCount)
                    {
                        iByBinCnt[iBin]++;
                    }
                }
            }

            fBarCode->TransformListToMap();                                     //Steven 20240515 : modified for 2D sort
            for(int i=0; i<6; i++)                                              //JerryYang 20240927 : 2D SORT模式Output arm放料避免空洞
            {
                for(int j=0; j<iTestBinCount; j++)
                {
                    temp=Prod.iT6CatData[j];

                    if(temp<=0)
                        continue;

                    if(i==temp-1)
                    {
                        iExceptAutoCnt[i]+=iByBinCnt[j];
                    }
                }
            }

            if(!DirectoryExists("D:\\HT9045_Log\\2D_MappingResult\\"))          //JerryYang 20230322 : add 2D mapping result
            {
                MyForceDirectories("D:\\HT9045_Log\\2D_MappingResult\\");
            }

            str3.sprintf("D:\\HT9045_Log\\2D_MappingResult\\%s_%s_%s_VS_Result.csv", edtSysLotID->Text, edtCusLotID->Text, edtCusDevGrp->Text);                 //JerryYang 20230322 : add 2D mapping result
            if(FileExists(str3)==false)
                sl2DMappingLog->SaveToFile(str3);
            int sum=0, Sum_ART=0;
//            double f=0.0, f1=0.0;
            for(int i=0; i<eTrayCount; i++)
            {
                sum     +=LastSet.BinCT    [0][iTo3Unload[i]];
                Sum_ART +=LastSet.BinCT_ART[0][iTo3Unload[i]];                  //wei 20150923 add ART計數
            }
            int iTemp=0;

            for(int i=0; i<iTestBinCount+1; i++)                                //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
            {
                if(LastSet.iTester==_2D_SORT)
                {
                    fShowBinSelect->StrGrdCategory->Cells[0][1+i]="";
                    fShowBinSelect->StrGrdCategory->Cells[1][1+i]="";
                    fShowBinSelect->StrGrdCategory->Cells[2][1+i]="";
                    fShowBinSelect->StrGrdCategory->Cells[3][1+i]="";
                    if(iByBinCnt[i]>0 || i==iTestBinCount)
                    {
                        if(i==iTestBinCount)
                        {
                            fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="Error Bin";
                        }
                        else
                        {
                            fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="BIN "+AnsiString(i);
                        }
                        fShowBinSelect->StrGrdCategory->Cells[2][1+iTemp]=LastSet.iBinData32[0][i];
                        fShowBinSelect->StrGrdCategoryART->Cells[1][1+iTemp]=LastSet.iBinData32_ART[0][i];              //wei 20150923 add ART計數
                        fShowBinSelect->StrGrdCategory->Cells[1][1+iTemp]=iByBinCnt[i];
                        if(sum>0)
                        {
//                            f=ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);
                            fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]=ChangeToPercentage((double)LastSet.iBinData32[0][i], (double)sum);
                        }
                        else
                        {
                            fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]="0.00%";
                        }

                        if((USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest) || CosFunction.bUseARTSortCount)                                         //wei 20150923 add ART計數  //Ifor 20170316 (wei) add MRT Mode
                        {
                            if(Sum_ART>0)
                            {
//                                f1=ChangeToFloat((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                                fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]=ChangeToPercentage((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                            }
                            else
                            {
                                fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]="0.00%";
                            }
                        }
                        iTemp++;
                    }
                }
            }
        }
        else
        {
            ShowMyMessage("The 2DID sorting list is missing", "找不到2DID sorting list");
            return;
        }
    }
    else
    {
        if(TestIF_File.b2DIDAllowList)
        {
            if(N23UseLotInfoFile()==false)
                return;

            if(cbRunMode->Text=="" ||                                           //Steven 20250811 : Add run mode check
               cbRunMode->ItemIndex==-1)                                        //JerryYang 20230322 : add 2D mapping result
            {
                sbSECSLotEnd->Down=true;
                ShowMyMessage("Please select the run mode in lot info!!");
                return;
            }
            else if(cbRunMode->Text!="CORR")                                    //JerryYang 20241104 : 支援2DID白名單功能
            {
                str2.sprintf("D:\\HT9045_Log\\2D_SortList\\Search2DIDByLot.txt");
                if(FileExists(str2))                                            //Steven 20160505 : 加上保護, 不然開程式會跳Error
                {
                    DeleteFile(str2);
                }

                if(IniConfig.iN23DownloadMethod==0)
                {
                    if(fBarCode->JCETUseMakeWhite2DIDList()==true)              //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
                    {
                        if(edtSysLotID->Text.Length()<3)                        //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
                        {
                            AnsiString sMsg;
                            sMsg.sprintf("批號 %s 異常，無法取得客戶代碼(前3碼)", edtSysLotID->Text);
                            ShowMyMessage(sMsg);
                            return;
                        }
                        TStringList *list2DTemp=new TStringList();
                        AnsiString sLocTmp;
                        AnsiString sCust=edtSysLotID->Text.SubString(0,3);
                        //檢查是否有相同檔名的list, 如果有就是續測. 下載後接續存檔
                        bool rlt=fBarCode->GetWhite2DIDList(edtSysLotID->Text);
                        fBarCode->bNeedCheckWhitleList=
                            (cbRunMode->Text=="FT1" || cbRunMode->Text.Pos("QA")>0)?false:true;     //RogerYang 20260615 : 新增QA      //FT1不用執行
                        if(cbRunMode->Text!="FT1" &&
                            cbRunMode->Text.Pos("QA")==0 &&                     //RogerYang 20260615 : 新增QA
                            rlt==false)                                         //非FT1要確認有沒有FT1的白名單檔案
                        {
                            ShowMyMessage("The 2DID sorting list is missing", "找不到2DID sorting list");
                            return;
                        }
                        str2.sprintf("%s%s\\%s\\%s_FT1.csv", sWhite2DIDListLoc, sCust, edtSysLotID->Text, edtSysLotID->Text);                                   //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
                        if(cbRunMode->Text=="FT1")                              //取得iJCETWhitelistSN作為塞入序號(PartID)用
                        {
                            if(rlt==true)
                            {
                                list2DTemp->LoadFromFile(str2);
                            }
                        }
                        else                                                    //如果不是FT1 也要下載當下runmode的名單，僅作為紀錄，不比對
                        {
                            sLocTmp.sprintf("%s%s\\%s\\%s_%s.csv", sWhite2DIDListLoc, sCust, edtSysLotID->Text, edtSysLotID->Text, cbRunMode->Text);            //RogerYang 20260225 : 修改白名單路徑 \\客戶代碼(批次前3碼)\\批次_FTx_csv
                            rlt=fBarCode->GetWhite2DIDList(edtSysLotID->Text, cbRunMode->Text);
                            if(rlt==true)
                            {
                                list2DTemp->LoadFromFile(sLocTmp);
                            }
                        }

                        if(list2DTemp->Count>1)                                 //有資料，根據_FTx.csv取得最後一筆資料的partID
                        {
                            TStringList* sRowList=new TStringList();
                            sRowList->Delimiter=',';
                            sRowList->DelimitedText=list2DTemp->Strings[list2DTemp->Count-1];
                            fBarCode->iJCETWhitelistSN=sRowList->Strings[sRowList->Count-1].ToIntDef(0);                //在最後一個欄位
                            delete sRowList;
                        }
                        else
                        {
                            fBarCode->iJCETWhitelistSN=0;
                        }
                        delete list2DTemp;
                    }
                }
                else if(IniConfig.iN23DownloadMethod==1)                        //JerryYang 20250320 : 2DID白名單功能 Net Drive
                {
                    //----------------------
                    //開始下載檔案
                    //----------------------
                    do
                    {
                        MyForceDirectories(asBarCodeLogPath);
                        WIN32_FIND_DATA filedata1;                              // Structure for file data
                        HANDLE filehandle1;                                     // Handle for searching
                        AnsiString szFileName;
                        filehandle1=FindFirstFile((IniConfig.sN23_4_URL + "*.txt").c_str(), &filedata1);
                        TStringList *MyList=new TStringList();
                        MyList->Clear();
                        if(filehandle1!=INVALID_HANDLE_VALUE)
                        {
                            do
                            {
                                /* 不處理隱藏檔及 . 跟 .. */
                                if((filedata1.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                                    strcmp(filedata1.cFileName, ".")==0 ||
                                    strcmp(filedata1.cFileName, "..")==0)
                                    continue;

                                if(ExtractFileExt(filedata1.cFileName).LowerCase()==".txt")                             // 若找到的檔案的副檔名是 .txt
                                {
                                    szFileName=ChangeFileExt(ExtractFileName(filedata1.cFileName), "");                 // 取出檔名，其實就是把副檔名設成""
                                    MyList->Add(szFileName);
                                }
                            } while(FindNextFile(filehandle1, &filedata1));
                            FindClose(filehandle1);
                        }

                        fBarCode->bNeedCheckWhitleList=true;

                        if(MyList->Count>0)
                        {
                            iIndex=-1;
                            if(SPIL_FOR_QLE==1)     //JerryYang 20260409 : QLE 白名單檔名
                            {
                                iPos=edtSysLotID->Text.AnsiPos("-");
                                sWhiteListLotID=edtSysLotID->Text.SubString(1, iPos-1);
                                sFileName.sprintf("%s_%s", sWhiteListLotID, sWhiteListProcess);
                            }
                            else if(CUSTOMER_CODE==CC_SJ_Semiconductor)              //RogerYang 20260125 : Add for SJSM LotID format
                            {
                                sWhiteListLotID=edtSysLotID->Text;
                                sWhiteListProcess=cbRunMode->Text;
                                sFileName.sprintf("%s_%s", sWhiteListLotID, sWhiteListProcess);
                                sWhiteListProcess=edtSysLotID->Text.SubString(iPos+1, edtSysLotID->Text.Length());
                            }
                            else
                            {
                                sFileName=edtSysLotID->Text;
                                sWhiteListLotID=edtSysLotID->Text;
                                sWhiteListProcess=edtSysLotID->Text;
                            }

                            for(int i=0; i<MyList->Count; i++)
                            {
                                if(MyList->Strings[i].AnsiPos(sFileName)!=0)
                                {
                                    iIndex=i;
                                    as2DWhiteListLog="D:\\HT9045_Log\\2DBarCode\\CheckResult\\"+MyList->Strings[iIndex]+"_CheckResult_"+Now().FormatString("yyyymmddhhnnss")+".txt";
                                    as2DWhiteListLogName=MyList->Strings[iIndex]+"_CheckResult_"+Now().FormatString("yyyymmddhhnnss")+".txt";
                                    if(MyList->Strings[i].AnsiPos("FirstStage")!=0)
                                    {
                                        fBarCode->bNeedCheckWhitleList=false;
                                    }
                                }
                            }

                            if(iIndex>=0)
                            {
                                str1.sprintf("%s%s.txt", IniConfig.sN23_4_URL, MyList->Strings[iIndex]);
                                str2.sprintf("%s\\%s.txt", asBarCodeLogPath, MyList->Strings[iIndex]);   //RogerYang 20260527 : Fix
                                delete MyList;

                                if(FileExists(str2))
                                {
                                    DeleteFile(str2);
                                }

                                ret=CopyFile(str1.c_str(), str2.c_str(), false);                                        //複製一份新的
                                ::Sleep(100);

                                if(ret==0)
                                {
                                    ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, str1);                //下載失敗，重試
                                    if(ret2==K_SKIP)
                                        break;
                                }
                                else
                                {
                                    ret2=0;
                                }
                            }
                            else
                            {
                                str1.sprintf("%s Can not find file %s", IniConfig.sN23_4_URL, sFileName);
                                ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, str1);                    //下載失敗，重試
                                if(ret2==K_SKIP)
                                    break;
                            }
                        }
                        else
                        {
                            delete MyList;
                            str1.sprintf("%s Can not find any txt file", IniConfig.sN23_4_URL);
                            ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, str1);                        //下載失敗，重試
                            if(ret2==K_SKIP)
                                break;
                        }
                    }
                    while(ret2==1);
                }
                else if(IniConfig.iN23DownloadMethod==2)                        //SECS/GEM
                {
                }
                else if(IniConfig.iN23DownloadMethod==3)                        //手動
                {
                    if(rgSort2DID->ItemIndex==1)                                //手動選檔案
                        str2=edSort2DIDBinFile->Text;
                }

                if(FileExists(str2))                                            //Steven 20160505 : 加上保護, 不然開程式會跳Error
                {
                    fBarCode->list2DWhitle->Clear();

                    fBarCode->list2DWhitle->LoadFromFile(str2);
                    fBarCode->list2DWhitleResult->LoadFromFile(str2);

                    if(fBarCode->JCETUseMakeWhite2DIDList()==true)              //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
                    {
                        for(int i=1; i<fBarCode->list2DWhitle->Count; i++)      //避開title
                        {
                            iPos=fBarCode->list2DWhitle->Strings[i].AnsiPos(",");
                            if(iPos>1)
                            {
                                sID=fBarCode->list2DWhitle->Strings[i].SubString(1, iPos-1);
                                if(sID!="2DID")
                                {
                                    fBarCode->list2DWhitle->Strings[i]=sID;
                                }
                            }
                        }
                    }
                    else
                    {
                        for(int i=0; i<fBarCode->list2DWhitle->Count; i++)
                        {
                            iPos=fBarCode->list2DWhitle->Strings[i].AnsiPos("_");
                            if(iPos>1)
                            {
                                sID=fBarCode->list2DWhitle->Strings[i].SubString(1, iPos-1);
                                fBarCode->list2DWhitle->Strings[i]=sID;
                            }
                        }
                    }
                    fBarCode->list2DWhitle->Sort();
                }
                else
                {
                    if(CUSTOMER_CODE==CC_JCET)                                  //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
                    {
                        if(fBarCode->JCETUseMakeWhite2DIDList()==true &&
                            cbRunMode->Text!="FT1" &&
                            cbRunMode->Text.Pos("QA")==0)                       //RogerYang 20260615 : 新增QA
                        {
                            ShowMyMessage("The 2DID sorting list is missing", "找不到2DID sorting list");
                            return;
                        }
                    }
                    else
                    {
                        btnASECL_LotStart->Down=false;
                        return;
                    }
                }
            }
        }
    }

    AnsiString Msg, MsgE;
    int len=0;
    TRegExpr * regex    =new TRegExpr;                                          //Steven 20200818 : 增加正規表達式判斷, 避免存檔例外
    regex->Expression   ="([<>/!|:\"\*\?])";
    regex->InputString=edtSysLotID->Text;
    if(edtSysLotID->Text.AnsiPos("\\")!=0 || regex->Exec())
    {
        len=regex->MatchLen[0];                                                 //取出第一個找到符合Expression的字串長度
        if(len!=0 || edtSysLotID->Text.AnsiPos("\\")!=0)
        {
            ShowMyMessage("Lot ID can not use special charater \\/:*?\"<>|", edtSysLotID->Text);
            edtSysLotID->Text="";
        }
    }

    regex->InputString=edtSysOperatorID->Text;
    if(edtSysOperatorID->Text.AnsiPos("\\")!=0 || regex->Exec())
    {
        len=regex->MatchLen[0];                                                 //取出第一個找到符合Expression的字串長度
        if(len!=0 || edtSysOperatorID->Text.AnsiPos("\\")!=0)
        {
            ShowMyMessage("OP ID can not use special charater \\/:*?\"<>|", edtSysOperatorID->Text);
            edtSysOperatorID->Text="";
        }
    }

    regex->InputString=cbRunMode->Text;
    if(cbRunMode->Text.AnsiPos("\\")!=0 || regex->Exec())
    {
        len=regex->MatchLen[0];                                                 //取出第一個找到符合Expression的字串長度
        if(len!=0 || cbRunMode->Text.AnsiPos("\\")!=0)
        {
            ShowMyMessage("Run Mode can not use special charater \\/:*?\"<>|", cbRunMode->Text);
            cbRunMode->Text="";
        }
    }

    #ifndef SOFT_SIMULTE
    if(CUSTOMER_CODE==CC_SCC)                                                   //Steven 20200302 : SCC楊恩民說輸入字串5~30個字元
    {
        if(edtSysLotID->Text.Length()<5 || edtSysLotID->Text.Length()>30)
        {
            SetLotID("");
        }

        if(edtSysOperatorID->Text.Length()<5 || edtSysOperatorID->Text.Length()>30)
        {
            edtSysOperatorID->Text="";
        }
    }
    #endif

    if(IniConfig.bEnable_SECS_GEM==false && TestIF.iTestType==TCP_IP_MODE)      //Steven 20230302 : Add for OS Tester
    {
        if(edtSysLotID->Text=="" || edtSysOperatorID->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter LotID and Operator ID!!");
            return ;
        }
    }

    //==> Eastsun 20260527 整合#027-2.MR.U5 BarcodeRecipe required :KYEC
    if(TestIF_File.bEnableBarCode==true &&
       BAR_CODE_INSTALL==ebctUseCCDMode &&
       TestIF_File.bBarCodeMultiRecipe==true)
    {
        if(edtBarcodeRecipe->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter Barcode Recipe!!");
            return ;
        }
    }
    //<== Eastsun 20260527 #027-2.MR.U5

    if(CUSTOMER_CODE==CC_Murata)                                                //Steven 20200409 : Murata 2DID比對功能
    {
        regex->InputString=edPage->Text;                                        //Steven 20200818 : 增加正規表達式判斷, 避免存檔例外
        if(edPage->Text.AnsiPos("\\")!=0 || regex->Exec())
        {
            len=regex->MatchLen[0];                                             //取出第一個找到符合Expression的字串長度
            if(len!=0 || edPage->Text.AnsiPos("\\")!=0)
            {
                ShowMyMessage("Page can not use special charater \\/:*?\"<>|", edPage->Text);
                edPage->Text="";
            }
        }

        regex->InputString=edtLine->Text;
        if(edtLine->Text.AnsiPos("\\")!=0 || regex->Exec())
        {
            len=regex->MatchLen[0];                                             //取出第一個找到符合Expression的字串長度
            if(len!=0 || edtLine->Text.AnsiPos("\\")!=0)
            {
                ShowMyMessage("Line ID can not use special charater \\/:*?\"<>|", edtLine->Text);
                edtLine->Text="";
            }
        }

        regex->InputString=edtProcessName->Text;
        if(edtProcessName->Text.AnsiPos("\\")!=0 || regex->Exec())
        {
            len=regex->MatchLen[0];                                             //取出第一個找到符合Expression的字串長度
            if(len!=0 || edtProcessName->Text.AnsiPos("\\")!=0)
            {
                ShowMyMessage("Process Name can not use special charater \\/:*?\"<>|", edtProcessName->Text);
                edtProcessName->Text="";
            }
        }

        regex->InputString=edtProduct->Text;
        if(edtProduct->Text.AnsiPos("\\")!=0 || regex->Exec())
        {
            len=regex->MatchLen[0];                                             //取出第一個找到符合Expression的字串長度
            if(len!=0 || edtProduct->Text.AnsiPos("\\")!=0)
            {
                ShowMyMessage("Product Name can not use special charater \\/:*?\"<>|", edtProduct->Text);
                edtProduct->Text="";
            }
        }
        delete regex;

        if(edtLine->Text=="" || edtProcessName->Text=="" || edtProduct->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter Line and Process and Product Name!!");
            return ;
        }

        if(edtSysLotID->Text=="" ||
           edPage->Text=="" ||
           edtSysOperatorID->Text=="" ||
           cbRunMode->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter lot information!!");
            return ;
        }
    }

    if(CUSTOMER_CODE==CC_TSI)                                                   //frank 20200814 : 每10盤記錄一次summary log
    {
        if(edtSysLotID->Text=="" ||
           edtSysOperatorID->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter LotID and Operator ID!!");
            return ;
        }

        if(RunInfo.bLotStart==false)
        {
            for(int i=0; i<10; i++)
                LastSet.TrayCount[i]=0;
        }
    }
    else if(CUSTOMER_CODE==CC_Greatek)                                          //Sam 20171101 (wei) : 超豐不用檢查 OPID and LotID
    {
    }
    else if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                 //KaiChen 20200618 ：矽格中興，不檢查RunMode
    {
        if(edtSysLotID      ->Text==""  ||
           edtSysOperatorID ->Text==""  ||
           edCustomerLotId  ->Text==""  ||                                      //Sam 20220223 : 矽格中興廠新增 Lot 資料
           coStation        ->Text==""  ||                                      //Sam 20220223 : 矽格中興廠新增 Lot 資料
           edStationNum     ->Text=="")                                         //Sam 20220223 : 矽格中興廠新增 Lot 資料
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter lot information!!");
        }
    }
    else if(IniConfig.bVTESTFunction==true)
    {
        if(edtSysLotID->Text=="" ||
           edtSysOperatorID->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter LotID and Operator ID!!");
            return ;
        }

        if(RunInfo.bLotStart==true &&
           fMesSystem->bDownloadLotInforFlag==true)
        {
            return;
        }

        if(CUSTOMER_CODE==CC_VTEST_Shanghai)
        {
            if(edtSysOperatorID->Text.Length()<4)
            {
                sbSECSLotEnd->Down=true;
                ShowMyMessage("OP ID 輸入小於4個字","OP ID Length less than 4");
                return ;
            }

            if(fMesSystem->RunModeRW(true, edtSysLotID->Text, cbRunMode->Text)==1)                                      //jou 20210823 : lot start增加保護避免重複執行run mode
            {
                Msg.printf("Lot: %s run mode %s 重複執行", edtSysLotID->Text, cbRunMode->Text);
                MsgE.printf("Lot: %s run mode %s Repeat execution", edtSysLotID->Text, cbRunMode->Text);
                ShowMyMessage(Msg, MsgE);
                return ;
            }
        }
    }
    else if(CUSTOMER_CODE==CC_SJ_Semiconductor_OS ||                            //Steven 20230213 : For SJSemi OS Tester
            CUSTOMER_CODE==CC_XINITECH)
    {
        if(IniConfig.bEnable_SECS_GEM==false && TestIF.iTestType==TCP_IP_MODE)  //Steven 20230213 : For SJSemi OS Tester
        {
            if(edtSysLotID->Text=="" ||
               edtSysOperatorID->Text=="")
            {
                sbSECSLotEnd->Down=true;
                ShowMyMessage("Please Enter LotID and Operator ID!!");
                return ;
            }
        }

        if(USE_RFID_READER && pnlLoader->Caption=="")
        {
            ShowMyMessage("Please Enter Tray ID!!");
            return ;
        }
    }
    else if((IniConfig.bSPILFunction==true ||
             CUSTOMER_CODE==CC_SJ_Semiconductor) &&
            CosFunction.bSortingBy2DList==true &&
            LastSet.iTester==_2D_SORT &&
            TestIF_File.bSortingBy2DIDList==true)
    {
        if(edtSysLotID->Text=="" ||
           edtSysOperatorID->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter LotID and Operator ID!!");
            return ;
        }
        else if(cbRunMode->Text=="" ||
                cbRunMode->ItemIndex==-1)                                       //JerryYang 20230322 : add 2D mapping result
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please select the run mode in lot info!!");
            return;
        }
    }
    else if(CUSTOMER_CODE==CC_LEADYO)                                           //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    {
        if(IniConfig.bN33_1_NetChangeFileAndData)                               //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
        {
            TStringList *ProductionInfo=new TStringList;
            AnsiString asProdPath="D:\\HT9045_Log\\Production_Info";
            AnsiString S="", asPath="", asSetupFile="", asLotID="", asFileName="";

            MyForceDirectories(asProdPath);

            if(bCheckOnlyOneFileAndData()==false)
            {
                sbSECSLotEnd->Down=true;
                return;
            }

            asPath=asProdPath+"\\TestInfo.txt";
            ProductionInfo->Clear();
            ProductionInfo->LoadFromFile(asPath);

            if(LastSet.iRunStartMode==rsmInitialStart)
            {
                asSetupFile=ProductionInfo->Strings[21];
                asSetupFile=asSetupFile.SubString(asSetupFile.Pos(":")+1, asSetupFile.Length()-asSetupFile.Pos(":"));   //冒號後是檔名
                if(asSetupFile=="")
                {
                    sbSECSLotEnd->Down=true;
                    ShowMyMessage("Setup File name is NULL, Please check TestInfo.txt");
                    ProductionInfo->Clear();
                    delete ProductionInfo;
                    return;
                }

                S.sprintf("D:\\HT9045\\IniData\\Data\\%s", asSetupFile);        //檢查工作檔是否存在
                if(DirectoryExists(S)==false)
                {
                    sbSECSLotEnd->Down=true;
                    ShowMyMessage("Setup File is not exist, Please check TestInfo.txt");
                    ProductionInfo->Clear();
                    delete ProductionInfo;
                    return;
                }

                if(fMain->cbSetupFileName->Text!=asSetupFile)
                {
                    fMain->cbSetupFileName->Text=asSetupFile;
                    fMain->cbSetupFileNameChange(fMain);
                }
            }

            asLotID=ProductionInfo->Strings[0];
            asLotID=asLotID.SubString(asLotID.Pos(":")+1, asLotID.Length()-asLotID.Pos(":"));
            edtSysLotID->Text=asLotID;

            ProductionInfo->Clear();
            delete ProductionInfo;
        }

        if(edtSysLotID->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter LotID!!");
            return;
        }
    }
    else if(CUSTOMER_CODE==CC_JCET)                                             //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
    {
        if(fBarCode->JCETUseMakeWhite2DIDList() &&
            edtCusLotID->Text=="")
        {
            sbSECSLotEnd->Down=false;
            ShowMyMessage("Please Enter Cust. Lot ID!!");
            return ;
        }

        if(edtSysLotID->Text=="" ||
                edtSysOperatorID->Text=="")
        {
            sbSECSLotEnd->Down=false;
            ShowMyMessage("Please Enter LotID and Operator ID!!");
            return ;
        }
    }
    else
    {
        if(IniConfig.bSPILFunction==true && TestIF_File.b2DIDAllowList &&       //JerryYang 20250320 : 2DID白名單功能
           IniConfig.iN23DownloadMethod!=2)                                     //JerryYang 20241104 : 支援2DID白名單功能
        {
            btClearBarcodeList->Click();                                        //JerryYang 20250327 : add
            if(edtSysLotID->Text=="")
            {
                ShowMyMessage("Please Enter LotID!!");
                return ;
            }

            if(fLotInfo->cbRunMode->Text=="")                                   //JerryYang 20250521 : add
            {
                ShowMyMessage("Please Enter Run Mode!!");
                return ;
            }
        }
        else if(CUSTOMER_CODE==CC_XINYUN)                                       //RogerYang 20260610 : XINYUN SECS開批只卡LotID
        {
            if(edtSysLotID->Text=="")
            {
                sbSECSLotEnd->Down=true;
                ShowMyMessage("Please Enter LotID!!");
                return ;
            }
        }
        else if(edtSysLotID->Text=="" ||
                edtSysOperatorID->Text=="")
        {
            sbSECSLotEnd->Down=true;
            ShowMyMessage("Please Enter LotID and Operator ID!!");
            return ;
        }
        else if(cbRunMode->Text=="" && CUSTOMER_CODE!=CC_SIGURD_ChungXing && cbRunMode->Visible)                        //Sam 20210623 : 修正 Bug  //KaiChen 20200618 ：矽格中興，不檢查RunMode  //Steven 20200306 : SCC要求按下Lot End的時候, 資料要清空
        {
            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20220923 : 20220927 : 矽品不檢查run mode
            {
            }
            else
            {
                sbSECSLotEnd->Down=true;
                ShowMyMessage("Please select run mode!!");
                return ;
            }
        }
    }

    fBarCode->ResetBarcodeCSVForLotStart();                                     //Ifor 20260625 add: 每次 LOT START 換新 LOG 檔並重載比對快取
    if(IniConfig.bVTESTFunction==true)                                          //jou 20200409 : VTest Mes system
    {
        if((IniConfig.bEnableRms || IniConfig.bEnableFTP) && IniConfig.bCheckFile)
        {
            if(fMesSystem->CheckVTENGmode(edtSysLotID->Text)==false)
            {
                fMesSystem->LabeledEditLotNo->Text=edtSysLotID->Text;
                fMesSystem->LabeledEditOPID->Text=edtSysOperatorID->Text;
                fMesSystem->buttonDownloadLotInfor->Click();

//                #ifdef BETA_VTestSummaryFile                                  //RogerYang 20250809 偉測Summary文件修改
                TStringList *Strlist = new TStringList();
                Strlist->Delimiter = ',';
                if(edtProcessName->Text=="")                                    //RogerYang 20250809 Summary文件產出必須要有資料，否則不允許開批
                    Strlist->Add("產品型號(CustPart)");
                if(edtProduct->Text=="")
                    Strlist->Add("客戶批次(CustLotNum)");
                if(sVTestInternalLot=="")
                    Strlist->Add("廠內批次(LotNum)");
                if(cbProcess->Text=="")
                    Strlist->Add("測試站點(WipStep)");
                if(cbTestTimes->Text=="")
                    Strlist->Add("測試次數");

                if(Strlist->Count>0)
                {
                    str5=Strlist->CommaText+" 資料不可為空，請檢察MES(MesFileLog)下載資料是否正確!!";
                    ShowMyMessage(str5);
                    Strlist->Clear();
                    delete Strlist;
                    return ;
                }
                Strlist->Clear();
                delete Strlist;
//                #endif

                if(fMesSystem->bDownloadLotInforFlag==false)
                {
                    edtSysLotID->Enabled=true;
                    edtSysOperatorID->Enabled=true;
                    cbProcess->Enabled=true;
                    cbTestTimes->Enabled=true;                                  //RogerYang 20250809 偉測Summary文件修改
                    edtProcessName->Enabled=true;
                    edtProduct->Enabled=true;
                    return;
                }
            }
        }
        else
        {
            bNoRTBinFixFlag[0]=TrayForm.asNoRTBinFix[0]==""?false:true;         //RogerYang 20250814 偉測不可複測bin功能
            bNoRTBinFixFlag[1]=TrayForm.asNoRTBinFix[1]==""?false:true;         //如果開批不用下載，這裡直接看TrayForm設定決定要不要做
            bNoRTBinFixFlag[2]=TrayForm.asNoRTBinFix[2]==""?false:true;         //bNoRTBinFixFlag - 該Fix是否啟用不可複測Bin
        }
        fMesSystem->bNoRTBinFlag[0]=TrayForm.asNoRTBinFix[0]==""?true:false;    //RogerYang 20250604 偉測不可複測bin功能
        fMesSystem->bNoRTBinFlag[1]=TrayForm.asNoRTBinFix[1]==""?true:false;    //bNoRTBinFlag - 判斷當下是否已經掃過條碼，空值代表不需要掃碼輸入
        fMesSystem->bNoRTBinFlag[2]=TrayForm.asNoRTBinFix[2]==""?true:false;
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {                                                                           //Sam 20170719 (Steven) 移植超豐 OEE 功能 form HT-7045
        if(IniConfig.bC11UseMonitorView && fMonitor->MVCtrl->IsConnect()==false)                                        //Sam 20170925 : 檢查攝影機有沒有連線  //JimmyChiu 20220125 優先檢測功能是否開啟
        {
            ShowMyMessage("Please Check Monitor Connect!!!");
            return;
        }

        if(Application->MessageBox("Are You Sure Start Lot?", "Start Lot?", MB_YESNO)!=IDYES)
        {
            return;
        }

        if(fProductionInfo->OEE_StartLot()==false)                              //Steven 20250520 : QQQ
        {
            lb_PIOEELotStatus->Caption="Production Start Lot Fail!";
            AnsiString sMsg="Production Start Lot Fail!#Please Check MO or Machine ID";
            ShowMyMessage(sMsg);
            return;
        }
    }

    if(CUSTOMER_CODE==CC_Murata)                                                //Steven 20200624 : Lot ID讀到DayDate, 自動轉成YYMMDD
    {
        AnsiString Date, Year;
        Year.sprintf("%04d", SystemYear);
        Year=Year.SubString(3, 2);
        Date.sprintf("%s%02d%02d", Year, SystemMonth, SystemDate);
        if(edtSysLotID->Text.AnsiPos("DAYDATE")!=0)
        {
            edtSysLotID->Text=StringReplace(edtSysLotID->Text, "DAYDATE", Date, TReplaceFlags()<<rfReplaceAll);
        }
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        fMain->patFunc->SetStartLotTime(dtStartLot);
        UpdateLotInfoPAT();
        fMain->machineTime.StartLot();                                          //Jimmychiu 20250916 : 新增機台運作狀態紀錄
        for(int i=0; i<8; i++)
            LastSet.SystemAccSecond[0][i]=0;
    }
    SetLotID(edtSysLotID->Text, false);

    if(IniConfig.bVTESTFunction &&                                              //AI(ht9045-config) 20260507 (RogerYang) : VTEST開批詢問是否沿用同一套Change KIT
        CosFunction.bUseHeadContactCount &&
        IniConfig.bLifeTimeCount[0])
    {
        int iMaxCnt=0;
        for(int i=0; i<FTestSuck.iShtRow; i++)
            for(int j=0; j<FTestSuck.iShtCol; j++)
                if(IniConfig.HeadContactCount[0][i][j] > iMaxCnt)
                    iMaxCnt = IniConfig.HeadContactCount[0][i][j];

        AnsiString strMsg;
        strMsg.sprintf("本批次與上一批次是否使用同一套 Change KIT?\r\n(目前最大 Contact Count: %d)", iMaxCnt);
        int ret = ShowMyMessageBox_YES_NO(strMsg, "Change KIT Check");
        if(ret == 2)
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
                for(int j=0; j<FTestSuck.iShtCol; j++)
                    IniConfig.HeadContactCount[0][i][j] = 0;
            SaveLastSetIni();                                                   //AI(ht9045-config) 20260508 (RogerYang) : 內部已判定VTEST寫入Recipe
            RecordProcess("VTEST Indium count cleared - Kit changed at Lot Start");
        }
        else
        {
            RecordProcess("VTEST Indium count kept - Same Kit at Lot Start");
        }
    }

    SetLotStart(__FUNC__);                                                      //Steven 20250515 : 整合Open Short測試報表

    if(CUSTOMER_CODE==CC_TSMC_TAINAN && IniConfig.bEnable_SECS_GEM==true)       //wei 20160517 TSMC lot卡關
    {
        ShowMyMessage("Please pressed Start , RUN !!");
    }

    if(CosFunction.bFirstTrayCheckOnUnloader==true &&                           //Jimmychiu 20251205 : First Tray Check On Unloader
       IniConfig.bP62FirstTrayCheckOnUnloader==true &&
       IniConfig.bP62AlwaysEnabledAtLotStart==true)
    {
        SetFirstTrayCheckOnUnloader();
    }

    if(CUSTOMER_CODE==CC_AMD_M && edtSysLotID->Enabled && iAMD_Function==2)     //Ifor 20231220 add 避免點一次記錄一次
    {
        FormHS->RecordParameter_TFAMDLog();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btStartCountClick(TObject *Sender)
{
    bStartCount_SCK=true;                                                       //ChungHung 20140714 only for SCK add bStartCount_SCK
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::labLotIDMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE && AccessLevel==iDefHonPrecLevel)
        fQwertyKey->ShowQwertyKey(edtSysLotID, N_NO_SYMBOL|N_NO_SPACE);         //wei  20150825 : 改用QWERTY鍵盤
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::spOCRSaveLogClick(TObject *Sender)
{
    Save_BarCodeLog();
}
//---------------------------------------------------------------------------
void TfLotInfo::Save_BarCodeLog()                                               //wei 20150720 BarCode log
{
    TDateTime tt=Now();
    Word SystemYear, SystemMonth, SystemDate;
    DecodeDate(tt, SystemYear, SystemMonth, SystemDate);
    AnsiString str1, str2, sLastFile, sFolder;

    str1.sprintf("%04d%s%02d%s", SystemYear, "_", SystemMonth, "\\");
    fMain->ProcessTimeUpdate(true);

    AnsiString fp="", s_cFolder;
    sFolder.sprintf("%s", asBarCodeLogPath1);

    if(sFolder.SubString(sFolder.Length(), 1)!="\\")
        sFolder=sFolder+"\\";

    s_cFolder=sFolder+str1;
    fp=s_cFolder+str2;
    MyForceDirectories(s_cFolder);
    sLastFile=s_cFolder+sFileNameTime+".txt";                                   //Steven 20190801 : 修正檔名錯誤
    Memo3->Lines->SaveToFile(sLastFile);                                        //Alick 20170104 (Steven) modify MEMO2改成MEMO3紀錄LOG同時+上時間
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::spOCRChangeFileClick(TObject *Sender)
{
    if(lbOCRUseFile->Caption!="")
    {
        for(int i=0; i<10; i++)
        {
            bOCROK[i]=false;
        }
        bOCROK[0]=true;
        Timer3->Enabled=true;
    }
    else
    {
       ShowMyMessage("Please Enter Change OCR File Name");
    }
    spOCRChangeFile->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::Timer3Timer(TObject *Sender)
{
    if(InitialOK==false)                                                        //Steven 20160912 : Add InitialOK in Timer
        return;

    int ret;
    ret=fOCR->OCRChangeFile();
    if(ret==1)
        Timer3->Enabled=false;
}
//---------------------------------------------------------------------------
bool __fastcall TfLotInfo::DoPassword()                                         //JerryYang 20151028 add for Amkor
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
void __fastcall TfLotInfo::spOCRLogInClick(TObject *Sender)
{
    fOCR->DoDisConnect();
    fOCR->DoConnect();
    spOCRLogIn->Down=false;

    if(TestIF_File.bOcrFunction)
        OCRConnectTest();

    if(INSTALL_OCR!=eocrUninstal)
    {
        if(TestIF_File.bOcrFunction)
            fMain->SendMSG_CMD(MSG_CMD_EnableBarCode);
        else
            fMain->SendMSG_CMD(MSG_CMD_DisableBarCode);

        if(TestIF_File.b2DUsePinInspection)                                     //Ifor 20240528 add:Pin1 Function
            fMain->SendMSG_CMD(MSG_CMD_EnablePin1Function);
        else
            fMain->SendMSG_CMD(MSG_CMD_DisablePin1Function);
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::SaveLotOperatorID(AnsiString Lot, AnsiString Operator, bool bRead)                                      //wei 20151230 art 紀錄資料
{
    AnsiString sPath=AuthPath+"config.ini";

    IniConfig.LotID_ART         = ReadWriteIni(sPath, "ARTReset", "LotID_ART",      Lot,        "", bRead);
    IniConfig.OperatorID_ART    = ReadWriteIni(sPath, "ARTReset", "OperatorID_ART", Operator,   "", bRead);
}
//---------------------------------------------------------------------------
enum eATCAction
{
    eATCGetSW_Ver=0,

    eATCActionTotal
};
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::NetATCTimeTimer(TObject *Sender)
{
    static int  iTask=1;
    static int  iCount=0;
    static int  iRetryCount=0;
    static int  iDleayCount=0;                                                  //Ifor 20170328 (wei) add ATC Run 命令太頻繁加上延遲時間
    static int  iTimeOutCheck=0;
    static int  iTriggerCount[eATCActionTotal];
    static bool bFirstConnect=true;
    static bool bFirstSetOffset=true;
    static bool bFirstSetRecipe=true;
    static bool bTSDFunction=true;
    static bool bATCSiteSetting=true;
    static bool bFirstSetFFCOffset=true;                                        //Ifor 20190730 : add FFC Temperature Offset
    static bool bRunNetATCTimer=false;
    static bool bHasATCAlarm=false;                                             //Ifor 20180307 : add ATC Alarm 未解除時不對ATC下命令
    static bool bRecipeFileChange=false;
    static bool bSendPFCommand=false;                                           //KenHsieh 20240216 : add ATC Power Follow Function

    int iRecipeLenght=0;
    AnsiString Msg, Msg1, sAlarmCode, sErrorPart;
    AnsiString asATCFPVer="";                                                   //KenHsieh 20240216 : add ATC Power Follow Function
    bool bCleanMultiUse[4];
//    int iATCVerBuf=0;                                                           //KenHsieh 20240216 : add ATC Power Follow Function

    if(InitialOK==false || bRunNetATCTimer==true)
    {
        iTimeOutCheck++;
        if(iTimeOutCheck>=50)                                                   //Ifor 20160805 add Time Out Check 避免程式永遠無法進入導致ATC Interface 無法連線
        {
            iTimeOutCheck=0;
            bRunNetATCTimer=false;
        }
        return;
    }
    AnsiString asFileTransfer;                                                  //Eastsun 20260522 整合
    static int iTransferFileDleayCount=0;                                       //Eastsun 20260522 整合
    if(iATC_RecipeFileTransfer >1)                                              //Eastsun 20260522 整合: 傳輸中或檔案接收中，等候逾時
    {
        if(iATC_RecipeFileTransfer==2)                                          //Eastsun 20260522 add: only count when state==2 (sending)
            iTransferFileDleayCount++;
        if(iTransferFileDleayCount>=300)  //30秒逾時 (100ms*300)
        {
            iTransferFileDleayCount=0;
            iATC_RecipeFileTransfer=0;
            RecordProcess("ATC Recipe File Transfer Timeout, reset state");
        }
        return;
    }
    else
    {
        iTransferFileDleayCount=0;
    }
    bRunNetATCTimer=true;

    if(bHasATCAlarm==true)                                                      //Ifor 20180307 (Steven) : add ATC Alarm 未解除時不對ATC下命令
    {
        iDleayCount++;
        if(iDleayCount>=2)                                                      //Ifor 20180308 add 避免 ATC Alarm 在未解除Handler Alarm前被關閉 延遲400ms
        {
            if(fNote->fShow==true || MyMessageBox->Visible==true)
            {
                if(Tri_Temp_Machine==1)                                         //Ztex 2024.03.07 Add If Handler Alarm ,CMD Need Send To ATC
                {
                    ;
                }
                else
                {
                    bRunNetATCTimer=false;
                    return;
                }
            }
            else
            {
                iDleayCount=0;
                bHasATCAlarm=false;
            }
        }
        else
        {
            bRunNetATCTimer=false;
            return;
        }
    }

    if(bStartATCRun==false ||                                                   //Ifor 20160830 Mark 整合ATC 按鍵功能 cbActiveNewATC->Checked ==> bStartATCRun
       Temperature.bATCActiveCooling==false ||
       (IniConfig.bPowerSaveFunction==true &&
        IniConfig.bC05_PowerSaveTemp==true &&
        fMain->tPSM.HotModule->bCheckTempClose==true))                          //Steven 20220419 : Power Saving補上new ATC
    {
        aldATCPower->Value=false;                                               //Ifor 20160414 ATC關閉時設定連線燈號 Off
        bATCTempAdjustmentOffset=false;
        bSetTempChange=true;                                                    //Ifor 20160414 ATC關閉時設定相關參數重送給ATC
        if(ATC_InterfaceForm->IsConnect()==true)
        {
            if(Tri_Temp_Machine==1 && AirStream_Select==1)                      //Ztex 2023.04.19 Add HT-1032 TriTemp Function
            {
                ATC_InterfaceForm->bCloseAirMachine=true;
                ATC_InterfaceForm->SendAirMachineStatus(0, Temperature.fSetTempature2AirMachine*10, Temperature.dSetIndexAirstreamTemp*10);                     //ATC 斷線關閉AirMachine
            }

            if(CUSTOMER_CODE==CC_ASE_SG)
            {
                if(ATC_InterfaceForm->iATC_MODE_TYPE!=60    &&
                   ATC_InterfaceForm->iATC_MODE_TYPE!=61    )
                {
                    ATC_InterfaceForm->Stop();
                }
            }
            ATC_InterfaceForm->sAlarmMessageList->Clear();                      //Ifor 20160826 add 斷線清除 ATC Alarm List
            ATC_InterfaceForm->Disconnet();                                     //Ifor 20160414 關閉ATC連線
        }

        if(ATC_InterfaceForm->iATC_MODE_TYPE==61)                               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            palATC61Status->Caption="ATC OffLine";
            palATC61Status->Color=clRed;
        }
        bRunNetATCTimer=false;
        NetATCTime->Enabled=true;
        return;
    }

    if(ATC_InterfaceForm->IsConnect()==true)
    {
        aldATCPower->Value=true;

        if(ATC_InterfaceForm->iATC_MODE_TYPE==61)                               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            palATC61Status->Caption="ATC OnLine";
            palATC61Status->Color=clLime;
        }

        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            pl_ATC_Online->Caption="ATC On Line";
            pl_ATC_Online->Color=clLime;                                        //kevin 20201207
        }
    }
    else
    {
        aldATCPower->Value=false;
    }

    if(bSetTempChange==true)
    {
        bFirstConnect=true;
        bFirstSetOffset=true;
        bFirstSetRecipe=true;
        bTSDFunction=true;
        ATC_InterfaceForm->bGetNowRecipeFlag=false;
        ATC_InterfaceForm->bGetRecipeListFlag=false;                            //Ifor 20170615 (wei) add重新下載ATC Receipe File List
        bSetTempChange=false;
        bATCSiteSetting=true;
        ATC_InterfaceForm->ATC_Now_RecipeFile->Caption="";                      //Ifor 20190307 : add 重新設定ATC工作檔
        bFirstSetFFCOffset=true;                                                //Ifor 20190730 : add FFC Temperature Offset
        bRecipeFileChange=true;                                                 //Ifor 20200306 : fix
        bSendPFCommand=true;
        if(iTask!=20)
            iTask=1;
    }

    if(CosFunction.bHiSiliconFunction==true ||
       TestIF_File.i2DIDFormat==eAMD ||
       CosFunction.bATCUseHandlerRecipeName==true)                              //Ifor 20191126 : add ATC Use Handler Recipe Name  //JerryYang 20200422 2DID format選項改用下拉選單
    {
        if(IniConfig.bL25_1ATCFileNameWithTemp==true)
            sATCFileName=fMain->cbSetupFileName->Text+"_"+Temperature.fWorkTemperBase;
        else
            sATCFileName=fMain->cbSetupFileName->Text;                          //kevin 20180627 (wei) add ATC3.1 main form
    }
    else
    {
        sATCFileName=Temperature.ATCFileName;                                   //kevin 20180627 (wei) add ATC3.1 temperature file name
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        sATCFileName=Temperature.ATCFileName;                                   //kevin 20181126 add ATC3.1 temperature file name
        if(sATCFileName.Pos("JOBFILE"))
        {
            if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
                sATCFileName= fMain->cbSetupFileName->Text+"_"+Temperature.fWorkTemperBase;
            else
                sATCFileName= fMain->cbSetupFileName->Text+"_"+Temperature.fAbitTemp;                                   //kevin 20180627  常溫的溫度
        }
    }

    ReadAirMachineStatus();                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function      //讀取AirMachine狀態

    iCount++;
    switch(iTask)
    {
        case 1:                                                                 //Ifor 20160107 判斷是否連線 開啟連線
            if(ATC_InterfaceForm->IsConnect()==false)
            {
                if(bFirstConnect==true)
                {
                    ATC_InterfaceForm->Connect();
                    bFirstConnect=false;
                    for(int i=0; i<eATCActionTotal; i++)
                        iTriggerCount[i]=0;
                }
                else
                {
                    ATC_InterfaceForm->Disconnet();
                    bFirstConnect=true;
                }
                bATCTempAdjustmentOffset=false;
                bSetTempChange=true;                                            //Ifor 20160414 ATC 斷線後相關參數重送給ATC
                aldATCPower->Value=false;
                if(bFirstConnect==false)
                {
                    iCount=0;
                    iTask=20;
                }
                iATCForHSMode=0;                                                //Ifor 20160912 ATC 通訊異常需清除ATC 版本，避免人員更換版本跑貨
            }
            else
            {
                if((Tri_Temp_Machine==1 && bDefrostKeepATCTemp==true) ||        //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                   (Tri_Temp_Machine==1 && bManualDefrost_Start==true && fTemp_Set->fShow==true))
                {
                    iTask=12;
                }
                else
                {
                    iTask=2;
                }
            }
            break;
        case 2:                                                                 //Ifor 20160107 取得現在Receipe File Name
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(ATC_InterfaceForm->bGetNowRecipeFlag==false ||
                   bFirstSetRecipe==false)                                      //Ifor 20190307 : add需監控ATC工作檔名稱，避免ATC工作檔被人員更改導致異常發生 && ==> ||
                {
                    ATC_InterfaceForm->GetNowRecipeFile();
                }

                if(bATCSiteSetting==true)
                {
                    bATCSiteSetting=false;
                    fMain->ChangeATCSiteUse();
                    ATC_InterfaceForm->ClearATCMessage();                       //Ifor 20160726 連線後關閉ATC Alarm 視窗
                }
                iTask=3;
            }
            else
            {
                iTask=1;
            }
            break;
        case 3:                                                                 //Ifor 20160107 取得現在Receipe File List
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(ATC_InterfaceForm->bGetRecipeListFlag==false)
                {
                     ATC_InterfaceForm->GetRecipeFileList();
                }

                if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_31 ||            //New ATC System 僅ATC3.1使用Self Test
                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_32)              //Ifor 20180416 : add ATC3.2使用Self Test
                {
                    if(CosFunction.bHiSiliconFunction==true ||                  //Ifor 20170329 (wei) add 非海思版本與未開啟 Self Test 功能 不詢問Self Test 狀態
                       bUseATC_SelfTestFunction==true)
                        ATC_InterfaceForm->QuerySelfTestStatus();               //Ifor 20160826 add Self Test Status 詢問
                }
                iTask=4;
            }
            else
            {
                iTask=1;
            }
            break;
        case 4:                                                                 //Ifor 20160107 取得目前 ATC 系統名稱
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(ATC_InterfaceForm->iATC_MODE_TYPE!=ATC_TYPE_UNSET)
                {
                    AnsiString strATCType="";
                    strATCType.sprintf("ATC %1.1f Monitor" , ATC_InterfaceForm->iATC_MODE_TYPE/10.0);
                    palATC->Caption =strATCType;
                }

                if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33)      //JerryYang 20250729 : ATC 3.3+6.0
                {
                    SW[SwDryAirUseATCCar].On();
                    SW[SwDryAirUseHandler].Off();
                }
                else
                {
                    SW[SwDryAirUseHandler].On();
                    SW[SwDryAirUseATCCar].Off();
                }
                iTask=5;
            }
            else
            {
                iTask=1;
            }
            break;
        case 5:                                                                 //Ifor 20160107 取得目前 ATC 版本名稱
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                iTriggerCount[eATCGetSW_Ver]++;
                if(iATCForHSMode==0 ||
                   iTriggerCount[eATCGetSW_Ver]>=60)                            //60*200 = 1200 (2 Min)
                {
                    iTriggerCount[eATCGetSW_Ver]=0;
                    ATC_InterfaceForm->GetSW_Ver();
                }

                if(Temperature.bATC_MultiSensorEnable==true)                    //Ifor 20251203 add:
                {
                    ATC_InterfaceForm->EnablesMultiSensor(Temperature.bATC_MultiSensorUse);
                }
                else
                {
                    for(int i=0; i<4; i++)
                        bCleanMultiUse[i]=false;
                    ATC_InterfaceForm->EnablesMultiSensor(bCleanMultiUse);
                }
                iTask=6;
            }
            else
            {
                iTask=1;
            }
            break;
         case 6:                                                                //Ifor 20160111 設定 ATC 工作擋
            if(ATC_InterfaceForm->IsConnect()==true)                            //Ifor 20200226 : FIX AMD Recipe 無法切換問題
            {                                                                   //AMD 工作檔會自動建立三個溫度區間    *_25_50dC   *_51_90dC  *_91_105dC
                if(TestIF_File.i2DIDFormat==eAMD)                               //非AMD客戶不會建立此工作檔所以判斷
                {                                                               //JerryYang 20200422 2DID format選項改用下拉選單
                    iRecipeLenght=sATCFileName.Length()+8;
                    if(LastSet.iTemperature==Tempture_Hot ||
                       LastSet.iTemperature==Tempture_AmbientHot)
                    {
                        if(Temperature.fWorkTemperBase>90)
                        {
                            iRecipeLenght+=1;
                        }
                    }
                    else
                    {
                        if(IniConfig.dATCAmbientTemperature<25 || IniConfig.dATCAmbientTemperature>90) //Ifor 20230310 add: AMD新增負溫工作檔
                        {
                            iRecipeLenght+=1;
                        }
                    }

                    if(ATC_InterfaceForm->ATC_Now_RecipeFile->Caption.Pos(sATCFileName)==0 ||
                       iRecipeLenght!=ATC_InterfaceForm->ATC_Now_RecipeFile->Caption.Length())
                    {
                        bRecipeFileChange=true;
                    }
                }
                else
                {
                    if(ATC_InterfaceForm->ATC_Now_RecipeFile->Caption!=sATCFileName)
                    {
                        bRecipeFileChange=true;
                    }
                }

                if(ATC_InterfaceForm->asATC_SW_Ver.Pos("KL")>=1)                //Ifor 20160105 Add 判斷是否為海思ATC程式版本   //Ifor 20200914 Fix: HISI => KL
                    iATCForHSMode=1;
                else
                    iATCForHSMode=2;
                sATCVerRead=ATC_InterfaceForm->asATC_SW_Ver;

                if(bRecipeFileChange==true)
                {
                    bFirstSetRecipe=false;
                    bRecipeFileChange=false;
                    ATC_InterfaceForm->bGetNowRecipeFlag=false;
                    bFirstSetOffset=true;                                       //Ifor 20190308 : 工作檔改變需重新設定Offset不然ATCOffset 為0

                    if(iATC_RecipeFileTransfer==0)                              //Eastsun 20260522 整合: 沒在傳送 ATC Recipe 時才執行原本的 ChangeRecipe
                    {
                        if(sATCFileName.Pos("_NET")==0)                             //Ifor 20160509 add 避免FTP下載時送出帶有_NET工作檔
                        {
                            if(LastSet.iTemperature==Tempture_Hot ||
                               LastSet.iTemperature==Tempture_AmbientHot)           //kevin 20140918 恆溫控制
                            {
                                ATC_InterfaceForm->ChangeRecipe(sATCFileName, Temperature.fWorkTemperBase);                 //kevin 20180627 change
                                if(Tri_Temp_Machine==1)                             //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                                    SetCloseSiteTemp(Temperature.fWorkTemperBase);  //設定關閉Site的溫度
                            }
                            else
                            {
                                ATC_InterfaceForm->ChangeRecipe(sATCFileName, IniConfig.dATCAmbientTemperature);
                                if(Tri_Temp_Machine==1)                             //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                                    SetCloseSiteTemp(Temperature.fWorkTemperBase);  //設定關閉Site的溫度
                            }
                        }
                    }
                    else  //Eastsun 20260522 整合: ATC Recipe File Transfer
                    {
                        if(CosFunction.bUseATCFileTransfer==true)  //Eastsun 20260522 整合
                        {
                            if(iATC_RecipeFileTransfer==1)  //Eastsun 20260522 整合 0:無需傳送 1:需要傳送 2:傳送中 6:檔案接收中
                            {
                                AnsiString asFileTransfer;
                                asFileTransfer.sprintf("%s%s.dat", asATCFileTransferPath, sATCFileName.c_str());
                                ATC_InterfaceForm->ATC_Recipe_FileTransfer(asFileTransfer);
                                iATC_RecipeFileTransfer=2;  //Eastsun 20260522 整合
                            }
                        }
                        else
                        {
                            iATC_RecipeFileTransfer=0;  //Eastsun 20260522 整合
                        }
                    }
//                    ATC_InterfaceForm->GetSW_Ver();
                }
                iTask=7;
            }
            else
            {
                iTask=1;
            }
            break;
        case 7:                                                                 //Ifor 20160107 取得目前 ATC 溫度 若與HANDLER 端不符 重新設定溫度
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                bool bATCNeedResetSetTemp=false;                                //wei 20230202 判斷ATC所有控制器設定溫度，一個溫度不一樣就要重新設定溫度
                double dbSetATCTemp=0;

                if(LastSet.iTemperature==Tempture_Hot ||
                   LastSet.iTemperature==Tempture_AmbientHot)                   //kevin 20140918 恆溫控制
                    dbSetATCTemp=Temperature.fWorkTemperBase;                   //Ifor 20160111 : ATC高溫的設定溫度
                else
                    dbSetATCTemp=IniConfig.dATCAmbientTemperature;              //Ifor 20160111 : [L11] ATC常溫的設定溫度

                bATCNeedResetSetTemp=ATC_InterfaceForm->CheckATCSetTemp(dbSetATCTemp);                                  //wei 20230202 判斷ATC所有控制器設定溫度，一個溫度不一樣就要重新設定溫度

                if(bChangeTest_TempOffset==0)
                {
                    if(TestIF.iTestMode==SingleSite && Temperature.bMultiZoneEnable)                                    //wei 20240617 Multi Zone
                    {
                        bATCNeedResetSetTemp=false;
                        ATC_InterfaceForm->SetMultiZoneTemp(dbSetATCTemp);
                    }

                    if(bATCNeedResetSetTemp)                                    //wei 20230202 判斷ATC所有控制器設定溫度，一個溫度不一樣就要重新設定溫度
                    {
                        ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
                    }
                }

                if(CUSTOMER_CODE==CC_KYEC_LEE)
                {
                    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_31   ||      //Ifor 20161121 add ATC3.1 Lot Start/End 接收狀態判斷 & 重送機制
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_32)          //Ifor 20180419 : add 傳送 Lot ID 給 ATC 3.2
                    {
                        bool bHasReceive=false;
                        if(bSendATCLotStart==true)                              //Ifor 20161121 add 確認ATC3.1 是否有收到ATC Lot Start命令若未收到重送命令
                        {
                            bHasReceive=ATC_InterfaceForm->GetATCLotStartStatus();
                            if(bHasReceive==false ||                            //Ifor 20161121 add Get ATC3.1 Lot Start Status
                               bReSendATCLotEven==true)                         //Ifor 20161121 add 斷線需重新送出 ATC Lot Even
                            {
                                bReSendATCLotEven=false;
                                ATC_InterfaceForm->LotStart(asATCEvenLotID);    //Ifor 20161121 (Steven) add Lot ID 傳送至 ATC3.1 System
                            }
                        }

                        if(bSendATCLotEnd==true)                                //Ifor 20161121 add 確認ATC3.1 是否有收到ATC Lot End命令若未收到重送命令
                        {
                            bHasReceive=ATC_InterfaceForm->GetATCLotEndStatus();                                        //Ifor 20161121 add Get ATC3.1 Lot End Status
                            if(bHasReceive==false || bReSendATCLotEven==true)   //Ifor 20161121 add 斷線需重新送出 ATC Lot Even
                            {
                                bReSendATCLotEven=false;
                                ATC_InterfaceForm->LotEnd(asATCEvenLotID);      //Ifor 20161121 (Steven) add Lot ID 傳送至 ATC3.1 System
                            }
                        }
                    }
                }
                iTask=8;
            }
            else
            {
                iTask=1;
            }
            break;
        case 8:                                                                 //Ifor 20160111 設定 ATC OffSet
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(bChangeTest_TempOffset==0)                                   //Ifor 20241118 : 測試中變溫
                {
                    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70)          //Steven 20190520 : 修正ATC7.0
                    {
                        fLotInfo->SetATCOffset(true);                           //ATC7.0
                    }
                    else if(bFirstSetOffset || bNeedInitialTestDelay)
                    {
                        fLotInfo->SetATCOffset(bFirstSetOffset);                //NetATCTimeTimer 200ms
                        if(CosFunction.bATCUseTempAdjustment==true)             //Ifor 20190215 : add ATC 使用 三點校正功能
                        {
                            if(bATCTempAdjustmentOffset==true)
                            {
                                bFirstSetOffset=false;
                            }
                        }
                        else
                        {
                            bFirstSetOffset=false;
                        }
                    }
                }

                if(CosFunction.bUseFFCTempOffset==true &&                       //Ifor 20190730 : add FFC Temperature Offset
                   bFirstSetFFCOffset==true)
                {
                    fLotInfo->SetATCFFCOffset();
                    bFirstSetFFCOffset=false;
                }

//                if(Temperature.bBoostFuncttion &&                             //Steven 20180817 : Boost Function
//                   iTriggerBoostFunction!=-1 &&
//                   iBoostFuncStep==2)    //煞車功能,暫時不用
//                {
//                    iTask=80;
//                }
//                else
                {
                    iTask=9;
                }
            }
            else
            {
                iTask=1;
            }
            break;
        //Steven 20180817 : Boost Function
        //==>
//        case 80:
//            if(ATC_InterfaceForm->IsConnect()==true)                          //煞車功能, 所以只補一半
//            {   //
//                double dbSetATCTemp=0;
//
//                if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)//kevin 20140918 恆溫控制
//                    dbSetATCTemp=Temperature.fWorkTemperBase+abs(Temperature.dBoostOffset[iTriggerBoostFunction]/2.0);                           //Ifor 20160111 : ATC高溫的設定溫度
//                else
//                    dbSetATCTemp=IniConfig.dATCAmbientTemperature+abs(Temperature.dBoostOffset[iTriggerBoostFunction]/2.0);                      //Ifor 20160111 : [L11] ATC常溫的設定溫度
//
//                ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
//                iBoostFuncStep=3;
//                iTask=7;
//            }
//            else
//            {
//                iTask=1;
//            }
//            break;
        //<==
        //Steven 20180817 : Boost Function
        case 9:
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(ATC_InterfaceForm->HasAlarmMsg()==true)                      //Ifor 20160111 顯示 ATC 錯誤訊息
                {
                    bHasATCAlarm=true;                                          //Ifor 20180307 (Steven) : add ATC Alarm 未解除時不對ATC下命令
                    iDleayCount=0;
                    FormHS->ShowATCAlarmMessage();
                }
                else if((fNote->fShow && (fNote->edErrorCode->Text=="MES16113" || fNote->edErrorCode->Text=="MES16114" ||
                                          fNote->edErrorCode->Text=="MES16115" || fNote->edErrorCode->Text=="MES16119")||                                       //JerryYang 20220817 : 漏水檢測異常時不要RUN  //jou 20231019 : Water Leakage Chiller
                        (Sen[SnWaterLeakageUp].Enable && Sen[SnWaterLeakageUp].IsOff()==true) ||
                        (Sen[SnWaterLeakageDown].Enable && Sen[SnWaterLeakageDown].IsOff()==true) ||
                        (Sen[SnWaterLeakagePlate].Enable && Sen[SnWaterLeakagePlate].IsOff()==true) ||
                        (Sen[SnWaterLeakageChiller].Enable && Sen[SnWaterLeakageChiller].IsOff()==true)))               //jou 20231019 : Water Leakage Chiller
                {
                }
                else
                {
                    if(ATC_InterfaceForm->GetRunState()==false &&               //Ifor 20160107 判斷ATC是否有Run
                       AirStream_Select==0)                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function ATC 初始化需要一點時間  //ATC 初始化需要一點時間
                    {
                        ATC_InterfaceForm->Run();
                    }
                    else
                    {
                        iDleayCount++;                                          //Ifor 20170328 (wei) add ATC Run 命令太頻繁加上延遲時間
                        if(iDleayCount>=10)                                     //200 ms x 10 = 2000 ms
                        {
                            iDleayCount=0;
                            bFirstSetRecipe=false;                              //Ifor 20190307 : add需監控ATC工作檔名稱，避免ATC工作檔被人員更改導致異常發生
                            ATC_InterfaceForm->Run();
                        }
                    }
                }
                iTask=10;
            }
            else
            {
                iTask=1;
            }
            break;
        case 10:                                                                //Ifor 20160506 判斷ATC系統是否被關Site 若被關Site 需重新設定Site
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                 for(int i=0; i<32; i++)
                 {
                    if(bATC_EnablesChannel[i]!=ATC_InterfaceForm->bATC_ModuleState[i])
                    {
                        fMain->ChangeATCSiteUse();
                        break;
                    }
                 }
                 iTask=11;
            }
            else
            {
                iTask=1;
            }
            break;
        case 11:                                                                //Ifor 20160803 add 設定Chiller 溫度
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(CosFunction.bUseHandlerSetChillerTemp==true)                 //Ifor 20191218 : add ATC Chiller 溫度由Handler設定
                {
                    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_31   ||
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_32   ||      //Ifor 20180419 : add ATC 3.2 Chiller 溫度設定By Setup File
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33   ||
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35   ||      //JerryYang 20220408 : add for ATC3.5
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_36   )
                    {
                        ATC_InterfaceForm->SetChillerTemp((double)Temperature.iATCChillerTemp);
                    }
                }
                else                                                            //由Handler 設定Chiller 溫度 不需要讀取
                {
                    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_31   ||
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_32   ||
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33   ||
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35   ||      //JerryYang 20220408 : add for ATC3.5
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_36   )
                    {
                        ATC_InterfaceForm->GetChillerStatus();                  //Ifor 20230515 add:取得Chiller 狀態
                    }
                }

                if(Tri_Temp_Machine==1 && AirStream_Select==1)                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                {
                    ATC_InterfaceForm->bCloseAirMachine=false;
                }

                if(IniConfig.bL43EnableATCPowerFollow && bSendPFCommand)        //KenHsieh 20240216 : add ATC Power Follow Function
                    iTask=17;
                else
                    iTask=13;
            }
            else
            {
                iTask=1;
            }
            break;
        case 12:
            if(bDefrostKeepATCTemp==false || bManualDefrost_Start==false)       //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                iTask=1;
            break;
        case 13:
            if(ATC_InterfaceForm->IsConnect()==true)                            //JerryYang 20231016 : Add handler端設定TJ參數(Slope、Offset)
            {
//                if(CosFunction.bATC_SlopeSaveOnHandler==true &&
//                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60)
                if(CosFunction.bATC_SlopeSaveOnHandler==true)
                {
                    ATC_InterfaceForm->Send_ATCSETTJENABLED(Temperature.bEnableTJFunction);
                    iTask=14;
                    break;
                }
                else
                {
                    iTask=1;
                }
            }
            else
            {
                iTask=1;
            }
            break;
        case 14:
            if(ATC_InterfaceForm->IsConnect()==true)                            //JerryYang 20231016 : Add handler端設定TJ參數(Slope、Offset)
            {
//                if(CosFunction.bATC_SlopeSaveOnHandler==true &&
//                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60 &&
//                   Temperature.bEnableTJFunction)
                if(CosFunction.bATC_SlopeSaveOnHandler==true &&
                   Temperature.bEnableTJFunction)
                {
                    ATC_InterfaceForm->Send_ATCSETTJPARAMETER(FloatToStr(Temperature.fSetTJ_Slope),FloatToStr(Temperature.fSetTJ_Offset));
                }
                iTask=15;                                                       //JerryYang 20231128 : 傳送ATC site map
            }
            else
            {
                iTask=1;
            }
            break;
        case 15:
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(CosFunction.bATC_SlopeSaveOnHandler==true &&
                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60 &&
                   Temperature.bEnableTJFunction)
                {
                    fMain->ATC_TJ_SITEMAP();
                }
                iTask=16;
            }
            else
            {
                iTask=1;
            }
            break;
        case 16:
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(CosFunction.bATC_SlopeSaveOnHandler==true &&                 //JerryYang 20251201 : ATC連動watchdog新增continue欄位
                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60 &&
                   Temperature.bEnableTJFunction)
                {
                    ATC_InterfaceForm->Send_ATCWATCHDOG(Temperature.bTjWatchdog, Temperature.dDelayAfterSOT, Temperature.fTjInputVLow, Temperature.fTjInputVHigh, Temperature.dDelayAfterSOTContinue);
                }
                iTask=1;
            }
            else
            {
                iTask=1;
            }
            break;
        case 17:                                                                //KenHsieh 20240216 : add ATC Power Follow Function
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                if(ATC_InterfaceForm->iATC_MODE_TYPE!=ATC_TYPE_60 &&
                   ATC_InterfaceForm->iATC_MODE_TYPE!=ATC_TYPE_70)              //Steven 20260415 : ATC 6.0/7.0 unsupported (1104)
                {
                    ATC_InterfaceForm->Send_ATCSetPFParameter(Temperature.bPowerFollower_Enable,
                                                              Temperature.iPowerFollower_FullPower,
                                                              Temperature.dPowerFollower_PFSlope,
                                                              Temperature.dPowerFollower_WGain,
                                                              Temperature.iPowerFollower_Many2one);
                }
                bSendPFCommand=false;
            }

            iTask=1;
            break;
        case 20:                                                                //Ifor 20160107 等待網路連線
            if(ATC_InterfaceForm->IsConnect()==true)
            {
                iTask=2;
                iCount=0;
                iRetryCount=0;
                break;
            }

            if(iCount==5)
            {
                if(ATC_InterfaceForm->IsConnect()==true)
                    ATC_InterfaceForm->Disconnet();
            }
            else if(iCount==10)
            {
                if(ATC_InterfaceForm->IsConnect()==false)
                    ATC_InterfaceForm->Connect();
            }
            else if(iCount>=20)                                                 //Ifor 20160625 == 修改 >= 避免發生例外造成異常
            {
                iCount=0;
                iRetryCount++;
            }

            if(iRetryCount>=5)
            {
                if(LastSet.iTester==ON_LINE)                                    //Ifor 20180222 (Steven) :HT7045 Offline 不判斷 ATC是否連線
                {
                    ShowErrorMessage("WAR15309", 0, MMATC_TCPIP, false, "NetATCTime");                                  //ATC connect error
                }
                iRetryCount=0;
                iCount=0;
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)
                {
                    fLotInfo->pl_ATC_Online->Caption="ATC Off Line";            //kevin 20201207 add display
                    fLotInfo->pl_ATC_Online->Color=clRed;
                }
            }

            if(ATC_InterfaceForm->HasAlarmMsg()==true)
            {
                bHasATCAlarm=true;                                              //Ifor 20180307 (Steven) : add ATC Alarm 未解除時不對ATC下命令
                iDleayCount=0;
                if(bStartATCRun==true)                                          //Ifor 20160831 (Steven) add 避免 ATC Alarm 兩次 && 避免ATC 無Alarm
                    FormHS->ShowATCAlarmMessage();
                ATC_InterfaceForm->sAlarmMessageList->Clear();                  //Ifor 20160826 add 斷線清除 ATC Alarm List
            }
            break;
    }
    bRunNetATCTimer=false;
}
//---------------------------------------------------------------------------
double ConvertPackageOffset()                                                   //Ifor 20190306 : add Package Offset 三點校正
{
    int iLowBase =Temperature.dATCPackageTemp[0];
    int iHighBase=Temperature.dATCPackageTemp[1];
    double m, s, Temp[5]={0.0};                                                 //kevin 20141006 Temp[3]->Temp[5]
    int ct1, ct2;
    char str[256];

    double dbSetATCTemp=0;

    if(LastSet.iTemperature==Tempture_Hot ||
       LastSet.iTemperature==Tempture_AmbientHot)                               //kevin 20140918 恆溫控制
        dbSetATCTemp=Temperature.fWorkTemperBase;                               //Ifor 20160111 : ATC高溫的設定溫度
    else
        dbSetATCTemp=IniConfig.dATCAmbientTemperature;                          //Ifor 20160111 : [L11] ATC常溫的設定溫度

    if(dbSetATCTemp>=Temperature.dATCPackageTemp[1])
    {
        iLowBase=Temperature.dATCPackageTemp[1];
        iHighBase=Temperature.dATCPackageTemp[2];
        ct1=1;
        ct2=2;
    }
    else
    {
        iHighBase=Temperature.dATCPackageTemp[1];
        ct1=0;
        ct2=1;
    }

    if(dbSetATCTemp==Temperature.dATCPackageTemp[0])
        return (Temperature.dATCPackageOffset[0]+Temperature.dATCPackageTemp[0])-dbSetATCTemp;
    if(dbSetATCTemp==Temperature.dATCPackageTemp[1])
        return (Temperature.dATCPackageOffset[1]+Temperature.dATCPackageTemp[1])-dbSetATCTemp;
    if(dbSetATCTemp==Temperature.dATCPackageTemp[2])
        return (Temperature.dATCPackageOffset[2]+Temperature.dATCPackageTemp[2])-dbSetATCTemp;

    Temp[0]=Temperature.dATCPackageOffset[0]+Temperature.dATCPackageTemp[0];
    Temp[1]=Temperature.dATCPackageOffset[1]+Temperature.dATCPackageTemp[1];
    Temp[2]=Temperature.dATCPackageOffset[2]+Temperature.dATCPackageTemp[2];

    if((iHighBase-iLowBase)==0)
        m=0;
    else
        m=(double)(iHighBase-dbSetATCTemp)/(double)(iHighBase-iLowBase);

    if(m==0)
        return dbSetATCTemp-dbSetATCTemp;
    s=Temp[ct2]+(Temp[ct1]-Temp[ct2])*m;
    sprintf(str,"%4.1f",s);
    return atof(str)-dbSetATCTemp;
}
//---------------------------------------------------------------------------
void TfLotInfo::SetATCOffset(bool bFirstSetOffset, bool bOFSClose)              //Ifor 20241118 : 測試中變溫
{
    double dbATC_Offset[ATC_MAX_SITE];                                          //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    int j1, j2, j3, j4, iCol=0;
    double dbATC_InitTempOffset[ATC_HEAD_COUNT];
    double dbATCOffstBuffer[32];
    double dbATC_TC2Offset[ATC_HEAD_COUNT];
    int iSiteMapping_InitTempOffset[]   ={11,15,12,16,13,17,14,18,33,37,34,38,35,39,36,40,19,23,20,24,21,25,22,26,41,45,42,46,43,47,44,48};
    ZeroMemory(dbATC_Offset, sizeof(dbATC_Offset));
    ZeroMemory(dbATC_InitTempOffset, sizeof(dbATC_InitTempOffset));
    ZeroMemory(dbATCOffstBuffer, sizeof(dbATCOffstBuffer));
    ZeroMemory(dbATC_TC2Offset, sizeof(dbATC_TC2Offset));                       //KenHsieh 20240311 : add Tc2 Offset
    static bool bFirstChange=false;
    int iTotal_Channel;                                                         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    double dTemp=0;
    double dATC_MultiOfs[32];

    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_70)                          //Ifor 20160429 add New ATC System Offset Function from ATC 7.0
    {
        for(int iAtc=0; iAtc<4; iAtc++)
        {
            if(LastSet.iTemperature==Tempture_Hot ||
               LastSet.iTemperature==Tempture_AmbientHot)                       //kevin 20180811 (Steven) : add 恆溫控制
            {
                if(fContact->fShow && iContactMode!=CONTACT_NORMAL)
                {
                    dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc];              //Steven 20151123 : Contact Mode no need another offset
                }
                else if(Temperature.bEnableATCTestTimeOffset &&                 //Steven 20160216 : 測試時間太短也要Offset
                        dTestSec<Temperature.iATCTestTimeOffsetTime)
                {
                    dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc]+Temperature.dATCTestTimeOffset[iAtc];
                }
                else if(Temperature.bEnableTempOffsetForInitial &&              //Steven 20141117 : 起測時溫度要補Offset
                        iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)
                {
                    if(CUSTOMER_CODE==CC_ASE_M)                                 //Ifor 20170310 (wei) add ASEM ATC7.0 Initial Offset 溫度補償僅在Delay Time 補溫
                    {
                        if(Temperature.iCintactCntForTempOffsetAtInitial==1)
                        {
                            if(bNeedInitialTestDelay==true)
                                dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc]+Temperature.ATCInitialOffset[iAtc];
                            else
                                dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc];
                        }
                        else
                        {
                            dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc]+Temperature.ATCInitialOffset[iAtc];           //Steven 20151006 : Initial Temp Offset for ATC
                        }
                    }
                    else
                    {
                        dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc]+Temperature.ATCInitialOffset[iAtc];               //Steven 20151006 : Initial Temp Offset for ATC
                    }
                }
                else if(Temperature.bEnableATCConFailOffset &&                  //Steven 20151123 : Continue Fail Temp Offset for ATC
                        Temperature.iATCCurrentFailCount[iAtc]>=Temperature.iATCConFailOffsetCount)                     //Steven 20151209 : Modify for ATC 7.0
                {
                    dbATC_Offset[iAtc]=Temperature.dATCConFailOffset[iAtc];
                }
                else if(LastSet.iRunStartMode==rsmQAMode &&                     //Steven 20151125 : QA Mode Temp Offset for ATC
                        Temperature.bEnableATCQAModeOffset)
                {
                    dbATC_Offset[iAtc]=Temperature.dATCQAModeOffset[iAtc];
                }
                else
                {
                    dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc];              //Steven 20151112 : 修改ATC7.0 Offset
                }
            }
            else
            {
                dbATC_Offset[iAtc]=Temperature.dATCInPC[iAtc];
            }
        }
        ATC_InterfaceForm->SetOffset(iATC_Use_Heat_Count, dbATC_Offset);        //TfLotInfo::SetATCOffset for ATC_TYPE_70
    }
    else
    {
        if(bFirstSetOffset==true || bNeedInitialTestDelay)
        {
            if(Temperature.bEnableTempOffsetForInitial==true)
            {
                for(int i=0; i<32; i++)
                {
                    dbATC_InitTempOffset[i]=Temperature.fTempOffSet[InitTempOffset][iSiteMapping_InitTempOffset[i]];
                }
            }

            int iNowDownArm=0;
            if(fContact->fShow)                                                 //Ifor 20240612 add:避免Contact Mode 資料回覆錯誤
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

            if(CosFunction.bUseSecondATCTempOffset==true && bChangeTest_TempOffset!=0)
            {
                if(iNowDownArm==Z1Down_Z2Up)
                {
                    for(int i=0; i<16; i++)
                    {
                        if(bOFSClose==true)                                     //Arm 1
                            dbATCOffstBuffer[i]=0;
                        else
                            dbATCOffstBuffer[i]=Temperature.dATCSecondTempOffset[i];

                        dbATCOffstBuffer[i+16]=Temperature.dATCTempOffset[i+16];                                        //Arm 2
                    }
                }
                else if(iNowDownArm==Z1Up_Z2Down)
                {
                    for(int i=0; i<16; i++)
                    {
                        dbATCOffstBuffer[i]=Temperature.dATCTempOffset[i];      //Arm 1

                        if(bOFSClose==true)                                     //Arm 2
                            dbATCOffstBuffer[i+16]=0;
                        else
                            dbATCOffstBuffer[i+16]=Temperature.dATCSecondTempOffset[i+16];
                    }
                }
                else
                {
                    for(int i=0; i<32; i++)
                        dbATCOffstBuffer[i]=Temperature.dATCTempOffset[i];
                }
            }
            else
            {
                for(int i=0; i<32; i++)
                {
                    dbATCOffstBuffer[i]=Temperature.dATCTempOffset[i];
                }
            }

            if(CosFunction.bATCUseTempAdjustment==true)                         //Ifor 20190215 : add ATC 使用 三點校正功能
            {
                if(bATCTempAdjustmentOffset==true)                              //Ifor 20190306 : add Package Offset 三點校正
                {
                    if(bFirstChange==false)                                     //重新等待計算，避免Offset 尚未更新導致補錯溫度
                    {
                        bATCTempAdjustmentOffset=false;
                        bFirstChange=true;
                        return;
                    }
                    else
                    {
                        Temperature.dATCPackageOffsettemp=ConvertPackageOffset();
                        bFirstSetOffset=false;
                        bFirstChange=false;
                    }
                }
                else
                {
                    return;
                }
            }
            else
            {
                bFirstSetOffset=false;
            }

            if(Tri_Temp_Machine==1)                                             //Ztex 2023.04.19 Add HT-1032 TriTemp Function
            {
                iTotal_Channel=TriTemperature_TotalChannel;                     //設定 TotalChannel
                int iATC_Channel[8]={2,3,0,1,65,66,63,64};
                for(int i=0; i<8; i++)
                {
                    dTemp=GetFactSetTemp(iATC_Channel[i], Temperature.fWorkTemperBase);
                    dbATC_Offset[i+32]=dTemp-Temperature.fWorkTemperBase;
                }
            }
            else
            {
                iTotal_Channel=iATC_Use_Heat_Count;
            }

            if(CosFunction.bHiSiliconFunction==true)                            //Ifor 20151216 海思專用版本 Offset 為0
            {
                ATC_InterfaceForm->SetOffset(iTotal_Channel, dbATC_Offset);     //TfLotInfo::SetATCOffset for HiSilicon
            }
            else
            {
                bool bUse1by2Heat=false;                                        //Ifor 20180505 (Steven) : add 一對二加熱器判斷
                if((TestIF_File.iTestMode==_12Site2X6     ||
                    TestIF_File.iTestMode==_16Site2X8     ||
                    TestIF_File.iTestMode==_16Site4X4     ||                    //kevin 20190516 add _16Site4X4
                    TestIF_File.iTestMode==_32Site4X8N)   &&
                    TestIF_File.bUse32Heater==false         )
                {
                    iCol=FTestSuck.iShtCol/2;
                    bUse1by2Heat=true;
                }

                if(TestIF.iTestMode==SingleSite && Temperature.bMultiZoneEnable)                                        //wei 20240617 Multi Zone
                {
                    iCol=4;
                }
                else if((TestIF_File.iTestMode==_8Site2X4 ||
                    TestIF_File.iTestMode==_16Site4X4) &&                       //Steven 20240425 add _16Site4X4
                   TestIF_File.bOctal_16Kit)                                    //JerryYang 20230204 : 修正8 site mode使用16site SLK溫度offset異常
                {
                    iCol=8;
                }
                else if((TestIF_File.iTestMode==_8Site2X4 ||
                         TestIF_File.iTestMode==_16Site4X4) &&                  //Steven 20240425 add _16Site4X4
                        TestIF_File.bOctal_12Kit)
                {
                    iCol=6;
                }
                else                                                            //JerryYang 20250225 : fixed for ATC offset
                {
                    iCol=FTestSuck.iShtCol;
                }

                bool bIsSTMMode=false;
                if(TestIF_File.iTestMode==_16Site2X8 && TestIF_File.bUse32Heater==false &&
                   ((TestIF_File.iSiteMap[0][1]==0 && TestIF_File.iSiteMap[0][3]==0 && TestIF_File.iSiteMap[0][5]==0 && TestIF_File.iSiteMap[0][7]==0 &&
                   TestIF_File.iSiteMap[1][0]==0 && TestIF_File.iSiteMap[1][2]==0 && TestIF_File.iSiteMap[1][4]==0 && TestIF_File.iSiteMap[1][6]==0) ||
                   (TestIF_File.iSiteMap[0][0]==0 && TestIF_File.iSiteMap[0][2]==0 && TestIF_File.iSiteMap[0][4]==0 && TestIF_File.iSiteMap[0][6]==0 &&
                   TestIF_File.iSiteMap[1][1]==0 && TestIF_File.iSiteMap[1][3]==0 && TestIF_File.iSiteMap[1][5]==0 && TestIF_File.iSiteMap[1][7]==0)))
                {
                    bIsSTMMode=true;
                }

            /*    if(iATC_Use_Heat_Count==8 &&                                  //Steven 20231017 : Fixed ATC 3.x temp offset
                   IsNNMode()==NN_1Row)
                {
                    int iJ1Arm[2][4]={{0, 1, 2, 3},                             //2x2 and 2x3NN mode to 8ch ATC
                                      {4, 5, 6, 7}};

                    int iJ3Arm[2][8]={{ 0,  1,  2,  3,  4,  5,  6,  7},         //2x2 and 2x3NN mode offset to 8ch ATC
                                      {16, 17, 18, 19, 20, 21, 22, 23}};

                    for(int i=0; i<2; i++)
                    {
                        for(int j=0; j<iCol; j++)
                        {
                            if((Temperature.bBoostFuncttion || Temperature.bLBTempFunction) &&                          //Steven 20180817 : Boost Function
                               iTriggerBoostFunction!=-1 &&
                               iBoostFuncStep==0)
                            {
                                if(Temperature.iBoostFunctionMode==2)
                                {
                                    dbATC_Offset[iJ1Arm[i][j]]=Temperature.dATCTempOffset[iJ3Arm[i][j]]+Temperature.dIndexATCSecondTempOffset[iJ3Arm[i][j]]+Temperature.dBoostOffset[iTriggerBoostFunction];
                                }
                                else if((IndexStatus==Z1Up_Z2Down && BTestSuck.AlreadyTest()==false) ||
                                        FTestSuck.AlreadyTest()  ||
                                        (fContact->fShow && iContactMode!=CONTACT_NORMAL && fContact->CarlibrationTask==800))
                                {
                                    dbATC_Offset[iJ1Arm[i][j]]=Temperature.dATCTempOffset[iJ3Arm[i][j]]+Temperature.dIndexATCSecondTempOffset[iJ3Arm[i][j]];
                                }
                                else
                                {
                                    dbATC_Offset[iJ1Arm[i][j]]=Temperature.dATCTempOffset[iJ3Arm[i][j]]+Temperature.dIndexATCSecondTempOffset[iJ3Arm[i][j]]+Temperature.dBoostOffset[iTriggerBoostFunction];
                                }
                            }
                            else if(Temperature.bEnableTempOffsetForInitial &&  //Steven 20180815 : ATC起測時溫度要補Offset
                                   iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)
                            {
                                dbATC_Offset[iJ1Arm[i][j]]=Temperature.dATCTempOffset[iJ3Arm[i][j]]+Temperature.fTempOffSet[InitTempOffset][iJ3Arm[i][j]];      //Ifor 20160523 Arm1 陣列起始位置00
                            }
                            else
                            {
                                if(Temperature.bBoostFuncttion || Temperature.bLBTempFunction)
                                {
                                    dbATC_Offset[iJ1Arm[i][j]]=Temperature.dATCTempOffset[iJ3Arm[i][j]]+Temperature.dIndexATCSecondTempOffset[iJ3Arm[i][j]];    //Ifor 20160523 Arm1 陣列起始位置00
                                }
                                else
                                {
                                    dbATC_Offset[iJ1Arm[i][j]]=Temperature.dATCTempOffset[iJ3Arm[i][j]];                //Ifor 20160523 Arm1 陣列起始位置00
                                }
                            }

                            if(CosFunction.bATCUseTempAdjustment==true)
                            {
                                dbATC_Offset[iJ1Arm[i][j]]=dATCTempAdjustmentOffset[iJ3Arm[i][j]];
                            }

                            if(CosFunction.bATCUsePackageOffset==true)
                            {
                                dbATC_Offset[iJ1Arm[i][j]]=dbATC_Offset[iJ1Arm[i][j]]+Temperature.dATCPackageOffsettemp;
                            }
                        }
                    }
                }
                else       */
                {                                                               //Steven 20250716 : Rework for ATC temp offset
                    for(int i=0; i<FTestSuck.iShtRow; i++)                      //Ifor 20160523 ATC 4、8、32 Site OffSet 整合
                    {
                        for(int j=0; j<iCol; j++)                               //JerryYang 20250225 : fixed for ATC offset
                        {
                            j3=fTemp_Set->iSiteToOfs[0][i][j];
                            j4=fTemp_Set->iSiteToOfs[1][i][j];
                            //          Arm1                    Arm2
                            //00 01 02 03 04 05 06 07   16 17 18 19 20 21 22 23
                            //08 09 10 11 12 13 14 15   24 25 26 27 28 29 30 31
                            if(FTestSuck.iShtRow==1 &&
                               ATC_SYSTEM==eNewATCSystem &&
                               iATC_Use_Heat_Count==32 &&
                               TestIF.iTestMode==DualSite)
                            {
                                j1=j*2+i;
                                j2=j*2+i+(iATC_Use_Heat_Count/2);
                            }
                            else if(ATC_SYSTEM==eNewATCSystem && iATC_Use_Heat_Count==8 && TestIF.iTestMode==DualSite)
                            {
                                j1=j;
                                j2=j+(iATC_Use_Heat_Count/2);

                                j3=j*8;
                                j4=j*8+16;
                            }
                            else if(bUse1by2Heat==true && bIsSTMMode==false)    //JerryYang 20251028 : fix 交錯型 ATC offset錯誤//JimmyChiu 20211026 : #P211001-ATK-H9-02 , V3.21.701.1 , Different Temp Offset site on Temperature Page for x16 & x8 heater SLK on HT9046AT.
                            {
                                j1=j3=(j*2)+(i*8);
                                j2=j4=j1+(iATC_Use_Heat_Count/2);
                            }
                            else
                            {
                                j1=fTemp_Set->iSiteToATC[0][i][j];
                                j2=fTemp_Set->iSiteToATC[1][i][j];
                            }

                            if(TestIF.iTestMode==SingleSite &&
                               Temperature.bMultiZoneEnable)
                            {
                                dbATC_Offset[j1]=dbATCOffstBuffer[j1];
                                dbATC_Offset[j2]=dbATCOffstBuffer[j1+16];
                            }
                            else if(CosFunction.bATCUseTempAdjustment==true)
                            {
                                dbATC_Offset[j1]=dATCTempAdjustmentOffset[j3];
                                dbATC_Offset[j2]=dATCTempAdjustmentOffset[j4];
                            }
                            else if((Temperature.bBoostFuncttion ||
                                     Temperature.bLBTempFunction) &&            //Steven 20180817 : Boost Function
                                    iTriggerBoostFunction!=-1 &&
                                    iBoostFuncStep==0 &&
                                    bUnderTest==false)
                            {
                                if(Temperature.iBoostFunctionMode==2)
                                {
                                    dbATC_Offset[j1]=dbATCOffstBuffer[j3]+Temperature.dIndexATCSecondTempOffset[j3]+Temperature.dBoostOffset[iTriggerBoostFunction];
                                    dbATC_Offset[j2]=dbATCOffstBuffer[j4]+Temperature.dIndexATCSecondTempOffset[j4]+Temperature.dBoostOffset[iTriggerBoostFunction];
                                }
                                else if((IndexStatus==Z1Up_Z2Down &&
                                         BTestSuck.AlreadyTest()==false) ||
                                        FTestSuck.AlreadyTest()  ||
                                        (fContact->fShow && iContactMode!=CONTACT_NORMAL && fContact->CarlibrationTask==800))
                                {
                                    dbATC_Offset[j1]=dbATCOffstBuffer[j3]+Temperature.dIndexATCSecondTempOffset[j3];
                                    dbATC_Offset[j2]=dbATCOffstBuffer[j4]+Temperature.dIndexATCSecondTempOffset[j4]+Temperature.dBoostOffset[iTriggerBoostFunction];
                                }
                                else
                                {
                                    dbATC_Offset[j1]=dbATCOffstBuffer[j3]+Temperature.dIndexATCSecondTempOffset[j3]+Temperature.dBoostOffset[iTriggerBoostFunction];
                                    dbATC_Offset[j2]=dbATCOffstBuffer[j4]+Temperature.dIndexATCSecondTempOffset[j4];
                                }
                            }
                            else if(Temperature.bEnableTempOffsetForInitial &&  //Steven 20180815 : ATC起測時溫度要補Offset
                                    iInitContactCount<Temperature.iCintactCntForTempOffsetAtInitial)
                            {
                                dbATC_Offset[j1]=dbATCOffstBuffer[j3]+dbATC_InitTempOffset[j1];
                                dbATC_Offset[j2]=dbATCOffstBuffer[j4]+dbATC_InitTempOffset[j2];
                            }
                            else
                            {
                                if(Temperature.bBoostFuncttion || Temperature.bLBTempFunction)
                                {
                                    dbATC_Offset[j1]=dbATCOffstBuffer[j3]+Temperature.dIndexATCSecondTempOffset[j3];
                                    dbATC_Offset[j2]=dbATCOffstBuffer[j4]+Temperature.dIndexATCSecondTempOffset[j4];
                                }
                                else
                                {
                                    dbATC_Offset[j1]=dbATCOffstBuffer[j3];
                                    dbATC_Offset[j2]=dbATCOffstBuffer[j4];
                                }
                            }

                            if(CosFunction.bATCUsePackageOffset==true)
                            {
                                dbATC_Offset[j1]=dbATC_Offset[j1]+Temperature.dATCPackageOffsettemp;
                                dbATC_Offset[j2]=dbATC_Offset[j2]+Temperature.dATCPackageOffsettemp;
                            }

                            if(bGPIBOffsetCommand)
                            {
                                dbATC_Offset[j1]=dbATC_Offset[j1]+dGPIBATCOffset[j1];
                                dbATC_Offset[j2]=dbATC_Offset[j2]+dGPIBATCOffset[j2];
                            }

                            if(Temperature.bUseReferTempSensor &&
                               Temperature.bUseTC2Offset)                       //KenHsieh 20240311 : add Tc2 Offset
                            {
                                dbATC_TC2Offset[j1]=Temperature.dATCTempOffset[j3+8];
                                dbATC_TC2Offset[j2]=Temperature.dATCTempOffset[j4+8];
                            }
                        }
                    }
                }

                if(Temperature.bBoostFuncttion ||
                   Temperature.bLBTempFunction)                                 //Steven 20180817 : Boost Function
                {
                    if(iTriggerBoostFuncBack!=-1 &&
                       iBoostFuncStep>=10)                                      //Cool down offset is controlled by MainProc()
                    {
                    }
                    else
                    {
                        ATC_InterfaceForm->SetOffset(iTotal_Channel, dbATC_Offset);                                     //TfLotInfo::SetATCOffset with boost function
                    }
                }
                else                                                            //Ifor 20160523 送出ATC 對應 Heat Offset 資料
                {
                    ATC_InterfaceForm->SetOffset(iTotal_Channel, dbATC_Offset);                                         //Ztex 2023.04.19 Add HT-1032 TriTemp Function //ATC 送Offset命令 TfLotInfo::SetATCOffset
                }

                if(Temperature.bUseReferTempSensor &&
                   Temperature.bUseTC2Offset &&
                   ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33)              //Steven 20260415 : TC2 Offset Rogers(TYPE_33) only
                    ATC_InterfaceForm->SetTC2Offset(iATC_Use_Heat_Count, dbATC_TC2Offset);
                if(Temperature.bATC_MultiSensorEnable==true)
                {
                    for(int i=0; i<iATC_Use_Heat_Count; i++)
                    {
                        for(int j=0; j<4; j++)
                        {
                            dATC_MultiOfs[(i*4)+j]=Temperature.dATC_MultiSensorOfs[(i*4)+j]+dbATC_Offset[i];
                        }
                    }
                    ATC_InterfaceForm->SetMultiSensorOffset(iATC_Use_Heat_Count, dATC_MultiOfs);
                }
            }
        }

        if(Tri_Temp_Machine==1)                                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            if(Temperature.bUseReferTempSensor==true)
                ATC_InterfaceForm->Set2ndFunction(Temperature.bUseReferTempSensor);                                     //TriTemp 使用第二點溫度
        }
        else
        {
            if(CosFunction.bATC32UseTJMode==false)                              //Ifor ASEM & AMD ATC3.2 跑 TJ 不使用第二點Sensor
                ATC_InterfaceForm->Set2ndFunction(Temperature.bUseReferTempSensor);                                     //Ifor 20160506 add ATC 第二點溫度Sensor開關 //Ifor 20160818 搬移位置ATC 7.0無第二點溫度Sensor
        }

        if(Temperature.bLBTempFunction && iTriggerBoostFunction==-1)
        {
            if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60)
                fTemp_Set->ControlATC60AirFlow(0);
        }
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::SetATCFFCOffset()
{
    int idataCount=0;
    bool bHasSet=false;
    double dbATC_FFCOffset[ATC_HEAD_COUNT];

    if(Temperature.bATC_FFCEnable==true)
    {
        for(int i=0; i<2; i++)
        {
            bHasSet=false;
            idataCount=0;
            ZeroMemory(dbATC_FFCOffset, sizeof(dbATC_FFCOffset));
            for(int j=0; j<10; j++)
            {
                if(Temperature.bATC_FFCPointUse[i][j]==true)
                {
                    dbATC_FFCOffset[0]              =i+1;                       //Arm
                    dbATC_FFCOffset[1+(idataCount)] =Temperature.iATC_FFCTimeOn[i][j];                                  //On Time
                    dbATC_FFCOffset[2+(idataCount)] =Temperature.iATC_FFCTimeOff[i][j];                                 //Off Time
                    dbATC_FFCOffset[3+(idataCount)] =Temperature.dATC_FFCOffset[i][j];                                  //Offset
                    idataCount=idataCount+3;
                    bHasSet=true;
                }
            }

            if(bHasSet==True)
                ATC_InterfaceForm->SetFFCOffset(idataCount+1, dbATC_FFCOffset);
        }
    }

    ATC_InterfaceForm->FFCOffsetEnable(Temperature.bATC_FFCEnable);             //Ifor 20190730 : add FFC Temperature Offset
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::coLevelModeChange(TObject *Sender)
{
    AnsiString S="";                                                            //jou 2016-01-06 download recipe 增加權限模式選擇
    AnsiString szDir="";

    szDir=asSystemPath+"\\System.dat";

    S=CheckAndReadIniData(szDir, "System", "Level Mode", AnsiString("00000000"));

    fPassword->edPassword->Clear();
    fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
    if(fPassword->edPassword->Text!=S)
    {
        coLevelMode->ItemIndex=0;
        coLevelMode->Text="Normal";
        labLevelMode->Font->Color=clBlack;
        coLevelMode->Font->Color=clBlack;
        chkTempOffset->Checked=false;
        btnSaveClick(this);
    }
    else
    {
        coLevelMode->ItemIndex=1;
        coLevelMode->Text="Engineer";
        labLevelMode->Font->Color=clRed;
        coLevelMode->Font->Color=clRed;
        chkTempOffset->Checked=true;
        btnSaveClick(this);
        btDownload->SetFocus();
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::SetATCFormVisible()                                             //Steven 20160217 : For ATC7.0
{
    if(Tri_Temp_Machine==1)                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        tsATC->TabVisible=false;                                                //隱藏ATC Tabsheet
    }
    else
    {
        if(ATC_SYSTEM==eATCHonPrecType)
        {
            if(Temperature.bATC70Active==true)                                  //Steven 20160217 : For ATC7.0
            {
                palATC->Caption="ATC 7.0 Monitor";
                aldATC7Status->Visible=true;
                lblATC70->Visible=true;
                aldATCChillerStatus->Visible=false;
                lblChiller->Visible=false;
            }
            else
            {
                palATC->Caption="ATC 2.0 Monitor";
                aldATC7Status->Visible=false;
                lblATC70->Visible=false;
                aldATCChillerStatus->Visible=true;
                lblChiller->Visible=true;
            }
            tsATC->TabVisible=true;
        }
        else if(ATC_SYSTEM==eNewATCSystem)
        {
            palATC->Caption="ATC Monitor";
            tsATC->TabVisible=true;

            aldATC7Status->Visible=false;
            lblATC70->Visible=false;
            lblChiller->Visible=false;
            aldATCChillerStatus->Visible=false;
            pan_ATCChillerSV->Visible=false;
            pl_ATCChillerSV->Visible=false;
            lblATC_Now_RecipeFile->Visible=true;
            NetATCTime->Enabled=true;
        }
        else
        {
            tsATC->TabVisible=false;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btClearBarcodeCountClick(TObject *Sender)
{
    AnsiString FolderName;
    AnsiString FileName;

    FolderName.sprintf("D:\\HT9045_Log\\2DBarCode\\%04d_%02d_%02d\\", SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(FolderName);
    FileName.sprintf("%04d-%02d-%02d %02d_%02d_%02d.xls", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    SGDToXLS(sgBarcode, AnsiString(FolderName+FileName));

    ZeroMemory(iNeedBarcodeCount, sizeof(iNeedBarcodeCount));
    ZeroMemory(iBarcodeDuplicate, sizeof(iBarcodeDuplicate));
    ZeroMemory(iBarcodeErrorCount, sizeof(iBarcodeErrorCount));
    ZeroMemory(iBarcodePassCount, sizeof(iBarcodePassCount));
    ZeroMemory(iBarcodeAutoRetry, sizeof(iBarcodeAutoRetry));
    fBarCode->DoBarcodeCount();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btClearBarcodeListClick(TObject *Sender)
{
    iBarcodeReject=0;                                                           //JerryYang 20200520 清除2DID累加fail數量
    fBarCode->map2DList.clear();
    fBarCode->list2DByLot->Clear();                                             //Steven 20160429 : 清空2D List, 要存到檔案裡
    fBarCode->SendCCDCommand(fBarCode->iBarCode1_1, "2DID by lot list");        //Steven 20160926 : 補上清除資料的紀錄
    RecordProcess("Clear barcode list.");                                       //Alick 20170202(jou) add 紀錄按下claer button
    fBarCode->list2DByLot->SaveToFile(asBarCodeLot);
    btClearBarcodeCount->Click();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::spSECSLotCheckClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20160517 TSMC lot卡關
    {
        if(IniConfig.bEnable_SECS_GEM==true && iTrayFeed!=1)
        {
            AnsiString sLotID=fLotInfo->edtSysLotID->Text;
            if(sLotID=="")
            {
                ShowMyMessage("Please Enter LotID !!");
            }
            else
            {
                EventReport(SECS_EVENT.DoLotStart);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::TimerERMSTimer(TObject *Sender)                      //Steven 20160711 : 使用進階版RMS
{                                                                               //Steven 20161104 : ERMS改用壓縮檔
    static bool bRun=false;
    static bool bDelay=false;
    static int  iAccessLevel=-1;                                                //Steven 20170829 (wei) : 使用變數判斷, 減少浪費Run Time的CPU資源
    char str1[256]="";
    bool bDownloadOk;
    WIN32_FIND_DATA filedata;                                                   // Structure for file data
    HANDLE filehandle;                                                          // Handle for searching
    AnsiString SDataPath, sFileExt, FileName;
    TStringList *sFolderList;

    if(InitialOK==false || bRun || IniConfig.bEnableErms==false)
    {
        return;
    }
    bRun=true;

    if(iAccessLevel!=AccessLevel)                                               //Steven 20170829 (wei) : 使用變數判斷, 減少浪費Run Time的CPU資源
    {
        for(int i=0; i<groupbDownloadItem->ControlCount; i++)                   //Steven 20170829 (wei) : ERMS Selsection新增權限控管
            groupbDownloadItem->Controls[i]->Enabled=(AccessLevel>=LevelSet.AccessLevel[151]);
        iAccessLevel=AccessLevel;
    }

    MyForceDirectories(IniConfig.sErmsPath);
    sFolderList=new TStringList();
    SDataPath=IniConfig.sErmsPath+"\\*.*";
    filehandle=FindFirstFile((SDataPath+"*").c_str(), &filedata);               //Steven 20101118 Start : 不要顯示資料夾以外的檔案
    if(filehandle!=INVALID_HANDLE_VALUE)
    {
        do                                                                      /* 不處理隱藏檔及 . 跟 .. */
        {
            if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                strcmp(filedata.cFileName, ".")==0 ||
                strcmp(filedata.cFileName, "..")==0)
            {
                continue;
            }
            else if(filedata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)         //FILE_ATTRIBUTE_COMPRESSED)   //如果該檔案為資料夾
            {
                FileName=filedata.cFileName;
                sFileExt=ExtractFileExt(FileName).LowerCase();

                if(sFileExt==".zip")                                            //若找到的檔案的副檔名是.7z or .zip
                {
                    sFolderList->Add(FileName.Delete(FileName.Pos(sFileExt), ExtractFileExt(FileName).Length()));
                }
            }
        }while(FindNextFile(filehandle, &filedata));
    }
    FindClose(filehandle);

    MNetLog("sFolderList->Count="+AnsiString(sFolderList->Count));
    if(sFolderList->Count>1 ||
       (fMain->palMainStatus->Caption!="HALT" &&
        sFolderList->Count>0))
    {                                                                           //刪除多餘的檔案
        if(bDelay==false)
        {
            bDelay=true;
            MNetLog("Delay 5 Sec 1");
            sFolderList->Clear();                                               //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete sFolderList;
            bRun=false;
            return;
        }
        bDelay=false;
        MNetLog("Delay 5 Sec 1 ok");

        for(int i=0; i<sFolderList->Count; i++)
        {
            SDataPath=IniConfig.sErmsPath+"\\"+sFolderList->Strings[i]+".zip";
            if(FileExists(SDataPath))
            {
                DeleteFile(SDataPath);
                MNetLog("Delete "+SDataPath);
            }
        }
        SDataPath=IniConfig.sErmsPath+"\\OK.dat";
        if(FileExists(SDataPath))
        {
            DeleteFile(SDataPath);
            MNetLog("Delete OK.dat");
        }
        SDataPath=IniConfig.sErmsPath+"\\NG.dat";
        if(FileExists(SDataPath))
        {
            DeleteFile(SDataPath);
            MNetLog("Delete NG.dat");
        }
        sFolderList->SaveToFile(IniConfig.sErmsPath+"\\NG.dat");
        RecordProcess("ERMS load NG 1: "+sFolderList->CommaText);
    }
    else if(fMain->palMainStatus->Caption=="HALT" && sFolderList->Count==1)
    {
        if(bDelay==false)
        {
            bDelay=true;
            sFolderList->Clear();                                               //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete sFolderList;
            MNetLog("Delay 5 Sec 2");
            bRun=false;
            return;
        }
        bDelay=false;
        MNetLog("Delay 5 Sec 2 OK");

        RecordProcess("ERMS load START : "+sFolderList->Text);
        bDownloadOk=DownloadFromERMS(sFolderList->Strings[0]);

        if(bDownloadOk)
        {
            fMain->ChangeSetUpFile(sFolderList->Strings[0]);
        }

        MNetLog("bDownloadOk");

        SDataPath=IniConfig.sErmsPath+"\\"+sFolderList->Strings[0];
        if(DirectoryExists(SDataPath))
        {
            ZeroMemory(&oFile, sizeof(SHFILEOPSTRUCT));
            oFile.hwnd=Handle;
            oFile.wFunc=FO_DELETE;
            strncpy(str1, SDataPath.c_str(), sizeof(str1));
            oFile.pFrom=str1;
            oFile.fFlags=FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI;
            bool bDelOK=SHFileOperation(&oFile);
            if(bDelOK==true)
            {
                ShowMyMessage("Delete Setup File ERROR");
            }
            MNetLog("Delete Setup File");
        }

        if(FileExists(SDataPath+".zip"))
        {
            DeleteFile(SDataPath);
            MNetLog("Delete "+SDataPath);
        }

        SDataPath=IniConfig.sErmsPath+"\\OK.dat";
        if(FileExists(SDataPath))
        {
            DeleteFile(SDataPath);
            MNetLog("Delete OK.dat");
        }
        SDataPath=IniConfig.sErmsPath+"\\NG.dat";
        if(FileExists(SDataPath))
        {
            DeleteFile(SDataPath);
            MNetLog("Delete NG.dat");
        }
        SDataPath=IniConfig.sErmsPath+"\\SP.ini";
        if(FileExists(SDataPath))
        {
            DeleteFile(SDataPath);
            MNetLog("Delete SP, ini");
        }

        if(bDownloadOk)
        {
            sFolderList->SaveToFile(IniConfig.sErmsPath+"\\OK.dat");
            RecordProcess("ERMS load OK : "+sFolderList->Text);
            ShowErrorMessage("MES1687", 0, MMSystem, 0, sFolderList->Text);     //UnZip %s.zip OK.   //Steven 20170829 (wei) : ERMS download ok need have MSG.
        }
        else
        {
            sFolderList->SaveToFile(IniConfig.sErmsPath+"\\NG.dat");
            RecordProcess("ERMS load NG 2: "+sFolderList->Text);
            ShowErrorMessage("WAR1686", 0, MMSystem, 0, sFolderList->Strings[0]);                                       //UnZip %s.zip Fail.
        }
    }
    sFolderList->Clear();                                                       //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sFolderList;
    bRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btChangeFileClick(TObject *Sender)                   //wei 20160728 Barcode File切換
{
    if(TestIF_File.bEnableBarCode)
    {
        if(BAR_CODE_INSTALL==ebctInShtIntel)
        {
            fBarCode->btBarcodeChangeFileDisConnect->Click();                   //wei 20160728 Barcode File切換
            fBarCode->btBarcodeChangeFileConnect->Click();
        }
        else if(BAR_CODE_INSTALL==ebctEtherNetCCD)
        {
            bBarcodeConnect=true;
            fBarCode->InitialBarcodeScanChangeFile();
            fBarCode->TimerBarcodeChangeFile->Enabled=true;
        }
        else if(BAR_CODE_INSTALL==ebctUseCCDMode &&
                CosFunction.b2DUseSubJobFunction==true &&
                TestIF_File.b2DUseSubJob==true)
        {
            bBarcodeConnect=true;
            fBarCode->InitialBarcodeScanChangeFile();
            fBarCode->TimerBarcodeChangeFile->Enabled=true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::pl_ATC_OnlineClick(TObject *Sender)
{
    if(Temperature.bATCActiveCooling==false || SystemStart)                     //Ifor 20171011 : add 未開啟ATC或機台移動時不執行
        return;
    if(fSecurity->Insufficient(114)==false)
        return;

    if(bStartATCRun)
    {
        if(ATC_SYSTEM==eATCHonPrecType)
            ATCInterfaceForm->btOffLine->Click();

        pl_ATC_Online->Caption="ATC Off Line";
        pl_ATC_Online->Color=clRed;
        palATC61Status->Caption="ATC Off Line";
        palATC61Status->Color=clRed;
        bStartATCRun=false;

        if(IniConfig.bL39_1AutoRunWhenTempOk==true)                             //Hmy 20191224  Add Wait Temperature Stable Time
        {
            bReChkTempInRange       =true;
            bAllPosTempOkOverTime   =false;
            bAllPosTempInRange      =false;
        }
    }
    else
    {
        if(ATC_SYSTEM==eATCHonPrecType)
            ATCInterfaceForm->btOnLine->Click();

        pl_ATC_Online->Caption="ATC On Line";
        pl_ATC_Online->Color=clLime;
        palATC61Status->Caption="ATC On Line";
        palATC61Status->Color=clLime;
        bStartATCRun=true;
    }
    iATCForHSMode=0;                                                            //Ifor 20160912 ATC 通訊異常需清除ATC 版本，避免人員更換版本跑貨
}
//---------------------------------------------------------------------------
void TfLotInfo::OCRConnectTest()                                                //wei 20161028 ocr連線測試
{
    SW[SwOCRTigger].On();
    MySleep(100);
    SW[SwOCRTigger].Off();
    bOCRConnectTest=true;
    bOCRConnectOK=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysOperatorIDKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        return;
    }

    if(bLotFirstKeyIn==false)                                                   //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bLotFirstKeyIn=true;
    }

    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        edtSysOperatorID->Text=InputBarcodeNumber("Input OP ID:", "UserName");
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA ||                                     //Steven 20211112 : 通富微不可以用鍵盤輸入
            IniConfig.bVTESTFunction==true ||                                   //jou 20220912 : 增加VTEST不可以用鍵盤輸入
            (IniConfig.bSPILFunction &&
             TestIF_File.b2DIDAllowList &&                                      //JerryYang 20241104 : 支援2DID白名單功能
             AccessLevel==0))
    {
        edtSysOperatorID->Text="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysLotIDKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        return;
    }

    if(CUSTOMER_CODE==CC_PTI ||                                                 //RogerYang 20170327 (Steven) 力成使用條碼機 避免利用Tab切換游標直接輸入
       IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        SetLotID("");
        AnsiString sBarcodeID=InputBarcodeNumber("Input Lot ID:", "LotID");
        SetLotID(sBarcodeID);
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA ||                                     //Steven 20211112 : 通富微不可以用鍵盤輸入
            IniConfig.bVTESTFunction==true ||                                   //jou 20220912 : 增加VTEST不可以用鍵盤輸入
            (IniConfig.bSPILFunction &&
             TestIF_File.b2DIDAllowList &&                                      //JerryYang 20241104 : 支援2DID白名單功能
             AccessLevel==0))
    {
#ifndef SOFT_SIMULTE
        edtSysLotID->Text="";
#endif
    }

    if(bLotFirstKeyIn==false)                                                   //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bLotFirstKeyIn=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbARMSShowClick(TObject *Sender)
{
    fARMS->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::palSecsGemMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    static int iStep=0;

    if(SystemStart ||
       AccessLevel<iDefHonPrecLevel ||                                          //V3.27K.538 Ifor 20170905 (wei) : add 京元喬智要求自動填入Lot/Operator ID 功能
       CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20191017 : KYEC 要求不可有後門KEY IN 資料
        return;

    switch(iStep)
    {
        case 0:
            if(Button==mbLeft)
            {
                iStep=1;
            }
            else
            {
                iStep=0;
            }
            break;
        case 1:
            if(Button==mbLeft)
            {
                iStep=2;
            }
            else
            {
                iStep=0;
            }
            break;
        case 2:
            if(Button==mbRight)
            {
                iStep=3;
            }
            else
            {
                iStep=0;
            }
            break;
        case 3:
            if(Button==mbRight)
            {
                iStep=4;
            }
            else
            {
                iStep=0;
            }
            break;
        case 4:
            if(Button==mbLeft)
            {
                iStep=5;
            }
            else
            {
                iStep=0;
            }
            break;
        case 5:
            if(Button==mbLeft)
            {
                if(edtSysLotID->Text=="")
                    SetLotID("0123456789");

                if(edtSysOperatorID->Text=="")
                    edtSysOperatorID->Text="12345";
            }
            iStep=0;
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sb_RunExecutFileClick(TObject *Sender)
{
    if(IniConfig.asA25RunExecutFile.Trim()=="")                                 //Sam 20170916 (Steven) 移植超豐外部呼叫執行檔功能 form HT-7045
    {
        ShowMyMessage("Run Execut File Path Is Empty Error!");
        return;
    }

    SHELLEXECUTEINFO execinfo;
    memset(&execinfo,0,sizeof(execinfo));
    execinfo.cbSize=sizeof(execinfo);
    execinfo.lpVerb="open";

    execinfo.lpFile = IniConfig.asA25RunExecutFile.c_str();

    if(FileExists(execinfo.lpFile)==true)
    {
        execinfo.lpParameters="";
        execinfo.fMask=SEE_MASK_NOCLOSEPROCESS;
        execinfo.nShow=SW_SHOWMINIMIZED;

        ShellExecuteEx(&execinfo);
    }
    else
    {
        ShowMyMessage("Run Execut File Is Not Exists!");
    }
    CloseHandle(execinfo.hProcess);                                             //Jimmychiu 20220901 釋放記憶體
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edDeviceNameKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if((CUSTOMER_CODE==CC_SCC && AccessLevel<iDefHonPrecLevel))
    {
        edDeviceName->Text="";
        Clipboard()->Clear();
        AnsiString sBarcodeID=InputBarcodeNumber("Input Device Name:");
        edDeviceName->Text=sBarcodeID;
    }

    if(CUSTOMER_CODE==CC_AMD_M && bDeviceName_OK==true)                         //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bDeviceName_OK=false;
        edDeviceName->Text="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sb_Main_EvenLevelLoginClick(TObject *Sender)
{
    fPassword->edPasswordPassWord->Text="";

    fPassword->ShowEventLogLogin();
    if(fPassword->CheckLoginSuccess()==true)
    {
        cbbASECL_LoginMode->ItemIndex=fPassword->GetLoginLevel();
    }
    else
    {
        cbbASECL_LoginMode->ItemIndex=0;
    }
    sb_Main_EvenLevelLogin->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtASECL_LotIDClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL|N_NO_SPACE);
}
//---------------------------------------------------------------------------
extern int ASECL_LotStart(AnsiString Source);                                   //Steven 20260408 : ASECL_LotStart的XML換位置
void __fastcall TfLotInfo::btnASECL_LotStartClick(TObject *Sender)
{
    AnsiString aStr, str2, str1, asFileName;
    int ret2;//, ret2;

    if(HasICUnderMachine()==false && HasAnyICInMachine()==false)                //JerryYang 20240423
    {
        fSCKART->ClearLotInfo();
    }

    if(CUSTOMER_CODE==CC_ASE_CL && USE_TRAY_MAPPING==1 && TestIF_File.bEnableTrayID2)
    {
        asFileName.sprintf("%s\\%s@WAFERLOT@%sXX@99@SWBIN@%s.txt", asTravelingLogPath, fLotInfo->edtASECL_LotID->Text, fLotInfo->edInsertion->Text, fLotInfo->edFlowID->Text);  //Steven 20170123 (Jou) : 修改檔案命名格式
        if(FileExists(asFileName))
        {
            TStringList *SL;
            SL=new TStringList();
            SL->LoadFromFile(asFileName);
            for(int i=0; i<SL->Count; i++)
            {
                if(SL->Strings[i].AnsiPos("JHT_LOG_START")>0)                   //判斷是否有LOT END過
                {
                    ShowMyMessage("The flow ID already existed, please key in again.", "此FlowID已存在, 請重新輸入!");
                    btnASECL_LotStart->Down=false;
                    return;
                }
            }
        }
    }

    if(TestIF_File.b2DIDAllowList)
    {
        str2.sprintf("D:\\HT9045_Log\\2D_SortList\\Search2DIDByLot.txt");
        if(FileExists(str2))                                                    //Steven 20160505 : 加上保護, 不然開程式會跳Error
        {
            DeleteFile(str2);
        }
    }

    if(TestIF_File.b2DIDAllowList)
    {
        //----------------------
        //開始下載檔案
        //----------------------
        do
        {
            if(!DirectoryExists("D:\\HT9045_Log\\2DBarCode\\"))
            {
                ForceDirectories("D:\\HT9045_Log\\2DBarCode\\");
            }
            str1.sprintf("%s%s_ASSY.xml", IniConfig.sN23_4_URL, edtASECL_LotID->Text);

            if(cbRunModeASECL->Text=="CORR")
            {
                str1.sprintf("%sCORR.xml", IniConfig.sN23_4_URL);
                str2.sprintf("D:\\HT9045_Log\\2DBarCode\\CORR.xml");
            }
            else if(cbRunModeASECL->Text=="FT1" || cbRunModeASECL->Text=="RT1")
            {
                str1.sprintf("%s%s_ASSY.xml", IniConfig.sN23_4_URL, edtASECL_LotID->Text);
                str2.sprintf("D:\\HT9045_Log\\2DBarCode\\%s_ASSY.xml", edtASECL_LotID->Text);
            }
            else if(cbRunModeASECL->Text=="FT2" || cbRunModeASECL->Text=="QT1" || cbRunModeASECL->Text=="RT2")
            {
                str1.sprintf("%s%s_FT1.xml", IniConfig.sN23_4_URL, edtASECL_LotID->Text);
                str2.sprintf("D:\\HT9045_Log\\2DBarCode\\%s_FT1.xml", edtASECL_LotID->Text);
            }
            else if(cbRunModeASECL->Text=="FT3" || cbRunModeASECL->Text=="QT2" || cbRunModeASECL->Text=="RT3")
            {
                str1.sprintf("%s%s_FT2.xml", IniConfig.sN23_4_URL, edtASECL_LotID->Text);
                str2.sprintf("D:\\HT9045_Log\\2DBarCode\\%s_FT2.xml", edtASECL_LotID->Text);
            }
            else if(cbRunModeASECL->Text=="FT4" || cbRunModeASECL->Text=="QT3" || cbRunModeASECL->Text=="RT4")
            {
                str1.sprintf("%s%s_FT3.xml", IniConfig.sN23_4_URL, edtASECL_LotID->Text);
                str2.sprintf("D:\\HT9045_Log\\2DBarCode\\%s_FT3.xml", edtASECL_LotID->Text);
            }
            else if(cbRunModeASECL->Text=="FT5" || cbRunModeASECL->Text=="QT4" || cbRunModeASECL->Text=="RT5")
            {
                str1.sprintf("%s%s_FT4.xml", IniConfig.sN23_4_URL, edtASECL_LotID->Text);
                str2.sprintf("D:\\HT9045_Log\\2DBarCode\\%s_FT4.xml", edtASECL_LotID->Text);
            }
            else if(cbRunModeASECL->Text=="QT5")
            {
                str1.sprintf("%s%s_FT5.xml", IniConfig.sN23_4_URL, edtASECL_LotID->Text);
                str2.sprintf("D:\\HT9045_Log\\2DBarCode\\%s_FT5.xml", edtASECL_LotID->Text);
            }

            if(FileExists(str2))
            {
                DeleteFile(str2);
            }
//
//            ret=CopyFile(str1.c_str(), str2.c_str(), false);    //複製一份新的
//            ::Sleep(100);

            fFTPClient->Download_2DID_WhiteList(IniConfig.sN23_4ASE_CL_FTPDownlaodPath);

//            if(ret==0)
//            {
//                ret2=ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, str1);                                    //下載失敗，重試
//                if(ret2==K_SKIP)
//                    break;
//            }
//            else
            {
                ret2=0;
            }
        }
        while(ret2==1);

        if(FileExists(str2))                                                    //Steven 20160505 : 加上保護, 不然開程式會跳Error
        {
            ASECL_LotStart(str2);
        }
        else
        {
            ShowMyMessage("The 2DID sorting list is missing", "找不到2DID sorting list");
            btnASECL_LotStart->Down=false;
            return;
        }
    }

    aStr.sprintf("%s LOTIN", edtASECL_OPID->Text);
    RecordProcess(aStr);
    aStr.sprintf("LOGIN = UserName : %s - LOT NAME : %s", edtASECL_OPID->Text, edtASECL_LotID->Text);
    SaveEventLogInfo("200000000", aStr, 10, " ");                               //START

    for(int i=0; i<8; i++)
        LastSet.SystemAccSecond[0][i]=0;

    ArmData[0]->ClearALLCT();
    ArmData[1]->ClearALLCT();
    ArmData[2]->ClearALLCT();
    fProductionInfo->UpdateControlBinCount(true);                               //Sam 20200525 : Control Bin
    fMain->UpdateRecordScreen(true);
    SetLotID(edtASECL_LotID->Text);                                             //JerryYang 20220215 : ASE-CL lot info需求

    edtASECL_OPID->Enabled       = false;
    edtASECL_LotID->Enabled      = false;
    edtASECL_TesterID->Enabled   = false;
    cbbASECL_LoginMode->Enabled  = false;
    cbRunModeASECL->Enabled      = false;
    edInsertion->Enabled=false;
    edCustomerDevice->Enabled=false;
    edFlowID->Enabled=false;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<8; j++)
        {
            edSocket[i][j]->Text=AnsiString(LastSet.strSocketID[i][j]);
        }
    }

    if(fMain->CheckCanChangeRealDummy()==false || SystemStart || HasICUnderMachine()==true || HasAnyICInMachine()==true ||                                      //JerryYang 20220215 : 機台內有IC不能download
      (LastSet.iRealDummy==REALLY && ((MOT[MMTrayY].fHasTray && MOT[MMTrayY].Tray.HasIC()) || MOT[MMTrayY_Car].fHasTray)))
    {
        if(bFTPDownlodFinish)                                                   //已經download過就不用再次download
        {
        }
        else
        {
            ShowMyMessage("Handler Must Clean Out");
            return;
        }
    }
    else
    {
        fFTPClient->DownloadWorkFileSettingFormServer();
        SetLotID(edtASECL_LotID->Text);                                         //JerryYang 20220215 : ASE-CL lot info需求
        SetLotStart(__FUNC__);
    }
    fLotInfo->SaveOEELog(true, true);
}
//---------------------------------------------------------------------------
bool __fastcall TfLotInfo::CheckEventLogParameter()
{
    AnsiString S;

    if(CUSTOMER_CODE!=CC_ASE_CL ||
       IniConfig.bN22Enable_EventLog==false ||
       LastSet.iTester==OFF_LINE)
        return true;

    S=edtASECL_LotID->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Lot Name must Key in!!");
        return false;
    }

    S=edInsertion->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Insertion must Key in!!");
        return false;
        }

    S=edCustomerDevice->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Customer Device must Key in!!");
        return false;
    }

    S=edFlowID->Text;
    S=S.Trim();
    if(S.Length()<=0)
    {
        ShowMyMessage("Flow ID must Key in!!");
        return false;
    }

    if(btnASECL_LotStart->Down==false)
    {
        ShowMyMessage("Must Press Lot Start First!!");
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::SaveBackEventLogInfo(AnsiString aBackAlarm)
{
    AnsiString aDownTime, aJamReStartTime, Target;
    TStringList *sl;
    sl = new TStringList;
    int is, im, ih;
    AnsiString str, str1;
    FILE *Fp;

    if(aBackAlarm=="")
        return;

    aJamReStartTime = Now().FormatString("hh:nn:ss");
    is = tEventLogTimer.LatchCycleTimeSec();
    is = is%60;
    im = (is/60)%60;
    ih = is/3600;
    aDownTime.sprintf("%02d:%02d:%02d", ih, im, is);

    sl->Clear();
    sl->CommaText=aBackEventLogMessage;

    str.sprintf("HANDLER LOG_%s_%04d_%02d_%02d", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);
    FileInfo().EnsureDirectoriesExist(asSaveEventLogPath);                          //Steven 20260504 : Ensure folder exists
    Target=asSaveEventLogPath+"\\"+str+".csv";

    if(FileExists(Target.c_str())==false)
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
        str1.sprintf("SiteID,ProjectID,TesterID,DATE,TIME,LOT NAME,LOGIN MODE,STOP TIME,RESTART TIME,DOWN TIME,KEYWORD,STATUS,ERROR CODE,ERROR TYPE,ERROR MESSAGE,MESSAGE\n");  //pig 20180413 AddASECLTestLogInfor
        fputs(str1.c_str(), Fp);
    }
    else
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
    }

    fputs(sl->CommaText.c_str(),Fp);
    fclose(Fp);
    aBackEventLogMessage="";
    sl->Clear();
    delete sl;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::SaveBackEventTracker(AnsiString aBackAlarm)
{
//    return; //暫時不用
    AnsiString aDownTime, aJamReStartTime, Target, aTargetPath;
    TStringList *sl;
    sl = new TStringList;
    int is, im, ih;
    AnsiString str, str1, aPath;
    FILE *Fp;

//    if(IniConfig.bN22Enable_EventLog==false)  //JerryYang 20190702 Mark掉,功能沒開也要存log           //Steven 20181224 : For ASE-CL
//        return;

    if(aBackAlarm=="")
        return;

//    iEventLogEndTime = MyTickCount();
    aJamReStartTime = Now().FormatString("hh:nn:ss");
//    is = (iEventLogEndTime-iEventLogStartTime)/1000;
    is = tEventLogTimer.LatchCycleTimeSec();
    is = is%60;
    im = (is/60)%60;
    ih = is/3600;
    aDownTime.sprintf("%02d:%02d:%02d", ih, im, is);

    sl->Clear();
    sl->CommaText=asEventTrackerMsg;

    str.sprintf("%s@%04d_%02d_%02d_EventTracker", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);

    aPath=as9045LogPath+"\\ASE log";
    aTargetPath.sprintf("%s\\%04d\\%02d\\%02d\\", aPath, SystemYear, SystemMonth, SystemDate);
    if(!(DirectoryExists(aTargetPath)))
        ForceDirectories(aTargetPath);

    Target=aTargetPath+str+".csv";

    if(FileExists(Target.c_str())==false)
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
        str1.sprintf("SiteID,ProjectID,TesterID,DATE,TIME,LOT NAME,LOGIN MODE,STOP TIME,RESTART TIME,DOWN TIME,KEYWORD,STATUS,ERROR CODE,ERROR TYPE,ERROR MESSAGE,MESSAGE\n");  //pig 20180413 AddASECLTestLogInfor
        fputs(str1.c_str(),Fp);
    }
    else
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
    }

    fputs(sl->CommaText.c_str(),Fp);
    fclose(Fp);
    asEventTrackerMsg="";
    sl->Clear();
    delete sl;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::UploadEventLogFile(AnsiString aFileName)             //移動前一天的
{
    AnsiString aPath=as9045LogPath+"\\ASECL";
    AnsiString aSource, aTarget;
    MyForceDirectories(aPath);
    aSource=asSaveEventLogPath+"\\"+aFileName+".csv";
    aTarget=aPath+"\\"+aFileName+".csv";
    if(FileExists(aSource))
        CopyFile(aSource.c_str(), aTarget.c_str(), false);
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::SaveASECLTestLogInfo()
{
    AnsiString Target, S, Target1;
    FILE *Fp;
    AnsiString str, str1, str3;
    int iCT=0;
    AnsiString aTestCT="", aLotName, LotInName;
    AnsiString aTestDate, aTestTime, aRecipe, aHandler, aTesterID;
    static int iTotalOld=0, iPassOld=0, iFailOld=0;                             //KaiChen 20200316 :Add
    int iTotal=0;                                                               //KaiChen 20200316 :Add
    if(CUSTOMER_CODE!=CC_ASE_CL)                                                //JerryYang 20230204 : add
        return;
    AnsiString sPathName;

    sPathName.sprintf("D:\\HT9045\\system\\ASECL_SUMMARY.ini");
    if(FileExists(sPathName)==false)                                            //檢查檔案
    {
        WriteIniDataNoLog(sPathName, "SUMMARY",    "iTotalOld",      0);
        WriteIniDataNoLog(sPathName, "SUMMARY",    "iPassOld",       0);
        WriteIniDataNoLog(sPathName, "SUMMARY",    "iFailOld",       0);
    }

    iTotalOld   =ReadIniData(sPathName, "SUMMARY",    "iTotalOld",   iTotalOld);
    iPassOld    =ReadIniData(sPathName, "SUMMARY",    "iPassOld",    iPassOld);
    iFailOld    =ReadIniData(sPathName, "SUMMARY",    "iFailOld",    iFailOld);

    MyForceDirectories(asSaveEventLogPath);
    aLotName  = edtASECL_LotID->Text;
    LotInName = cbbASECL_LoginMode->Text;
    str.sprintf("TEST SUMMARY_%s_%04d_%02d_%02d", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);
    Target = asSaveEventLogPath+"\\"+str+".csv";

    if(FileExists(Target)==false)
    {
        Fp=fopen(Target.c_str(),"a+");
        if(Fp==NULL)
            return;
        str1.sprintf("SiteID,ProjectCode,TesterID,DATE,TIME,LOT NAME,LOGIN MODE,INPUT,PASS,FAIL,ERROR#,Socket Usage,Socket Usage#\n");
        fputs(str1.c_str(), Fp);
    }
    else
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
    }

    if(aBackTestSummaryFile!=AnsiString(str))
    {
        UploadEventLogFile(aBackTestSummaryFile);
        aBackTestSummaryFile=AnsiString(str);
    }

    slASECLTestInfor->Clear();
    TastCategory.UpdataCount(true);                                             //Steven 20250514 : 統一計算數量
    iTotal=TastCategory.iTotalSocket-iTotalOld;                                 //KaiChen 20200316 :Add

    aRecipe     = fMain->cbSetupFileName->Text;                                 //JerryYang 20190702 fix log
    aHandler    = IniConfig.SocketHandlerID;
    aTesterID   = edtASECL_TesterID->Text;
    aTestDate   = Now().FormatString("yyyy/mm/dd");
    aTestTime   = Now().FormatString("hh:nn:ss");

    slASECLTestInfor->Add(aHandler);                                            //SiteID
    slASECLTestInfor->Add(aRecipe);                                             //ProjectCode
    slASECLTestInfor->Add(aTesterID);                                           //TesterID
    slASECLTestInfor->Add(aTestDate);                                           //DATE
    slASECLTestInfor->Add(aTestTime);                                           //TIME
    slASECLTestInfor->Add(aLotName);                                            //LOT NAME
    slASECLTestInfor->Add(LotInName);                                           //LOGIN MODE
    slASECLTestInfor->Add(TastCategory.iTotalSocket);
    slASECLTestInfor->Add(TastCategory.iPassSocket);
    slASECLTestInfor->Add(TastCategory.iTotalSocket-TastCategory.iPassSocket);

    if(iTotal<0)
    {
        slASECLTestInfor->Add(TastCategory.iTotalSocket);
        slASECLTestInfor->Add(TastCategory.iPassSocket);
        slASECLTestInfor->Add(TastCategory.iTotalSocket-TastCategory.iPassSocket);
    }
    else                                                                        //KaiChen 20200316 :Add
    {
        slASECLTestInfor->Add(TastCategory.iTotalSocket-iTotalOld);
        slASECLTestInfor->Add(TastCategory.iPassSocket-iPassOld);
        slASECLTestInfor->Add(TastCategory.iTotalSocket-TastCategory.iPassSocket-iFailOld);
    }
    iTotalOld   =TastCategory.iTotalSocket;
    iPassOld    =TastCategory.iPassSocket;
    iFailOld    =TastCategory.iTotalSocket-TastCategory.iPassSocket;
    WriteIniDataNoLog(sPathName, "SUMMARY",    "iTotalOld",      iTotalOld);
    WriteIniDataNoLog(sPathName, "SUMMARY",    "iPassOld",       iPassOld);
    WriteIniDataNoLog(sPathName, "SUMMARY",    "iFailOld",       iFailOld);

    slASECLTestInfor->Add(iPerMinuteNumberofError);                             //Error

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j]==true)
            {
                if(i==0 && j==0)
                    aTestCT=aTestCT+"1";
                else
                    aTestCT=aTestCT+"1";                                        //JerryYang 20190625 fix log
                iCT++;
            }
            else
            {
                if(i==0 && j==0)
                    aTestCT=aTestCT+"0";
                else
                    aTestCT=aTestCT+"0";                                        //JerryYang 20190625 fix log
            }
        }
    }

    slASECLTestInfor->Add(iCT);                                                 //Socket CT
    slASECLTestInfor->Add(aTestCT);                                             //Socket

    fputs(slASECLTestInfor->CommaText.c_str(), Fp);
    fputs("\n", Fp);                                                            //JerryYang 20190625 fix log
    fclose(Fp);

//    TastCategory.iTotalSocket = 0;
//    TastCategory.iPassSocket  = 0;
    iPerMinuteNumberofError   = 0;
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::SaveASECLNewTestLogInfo(int iR, int iC, int iBinData)
{
//    return; //暫時不用
    AnsiString Target, S, Target1, aTargetPath="", aPath="";
    FILE *Fp;
    AnsiString str, str1, str3;
    int iCT=0;//, iRow, iCol;
    AnsiString aTestCT="", aLotName, LotInName, aSocketID=" ", sBarcode="";
    AnsiString aTestDate, aTestTime, aRecipe, aHandler, aTesterID, aTestResult;
//    static int iTotalOld=0, iPassOld=0, iFailOld=0;                             //KaiChen 20200316 :Add
//    int iTotal=0;                                                               //KaiChen 20200316 :Add

//    if(IniConfig.bN22Enable_EventLog==false)  //JerryYang 20190702 Mark掉,功能沒開也要存log           //Steven 20181224 : For ASE-CL
//        return;

    if(LastSet.iRealDummy!=REALLY)
        return;

    AnsiString sPathName;

    sPathName.sprintf("D:\\HT9045\\system\\ASECL_SUMMARY.ini");
    str.sprintf("%s@%04d_%02d_%02d_UnitTracker", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);

    aPath=as9045LogPath+"\\ASE log";
    aTargetPath.sprintf("%s\\%04d\\%02d\\%02d\\", aPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(aTargetPath, __FUNC__);

    sPathName=asSaveEventLogPath+"\\"+str+".csv";
    MyForceDirectories(asSaveEventLogPath, __FUNC__);
    aLotName  = edtASECL_LotID->Text;
    LotInName = cbbASECL_LoginMode->Text;
    Target = aTargetPath+str+".csv";

    if(FileExists(Target)==false)
    {
        Fp=fopen(Target.c_str(),"a+");
        if(Fp==NULL)
            return;
        str1.sprintf("SITEID,PROJECTID,MACHINEID,DATE,TIME,LOT NAME,LOGIN MODE,SERIALNUMBER#,SOCKETID,TEST RESULT,SOCKET USAGE#\n");
        fputs(str1.c_str(), Fp);
    }
    else
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
    }

    if(aBackTestSummaryFile!=AnsiString(str))
    {
        UploadEventLogFile(aBackTestSummaryFile);
        aBackTestSummaryFile=AnsiString(str);
    }

    slASECLTestInfor->Clear();
//    fObserver->UpdateBinCountPerSite();         //KaiChen 20200316 :Add
//    iTotal=TastCategory.iTotalSocket-iTotalOld; //KaiChen 20200316 :Add

    aRecipe     = fMain->cbSetupFileName->Text;                                 //JerryYang 20190702 fix log
    aHandler    = IniConfig.SocketHandlerID;
    aTesterID   = edtASECL_TesterID->Text;
    aTestDate   = Now().FormatString("yyyy/mm/dd");
    aTestTime   = Now().FormatString("hh:nn:ss");
    if(TestIF_File.iSiteMap[iR][iC]>0)
    {
        aSocketID=LastSet.strSocketID[iR][iC];
        sBarcode=TestSocket.cDeviceInf[iR][iC];
    }

    if(Prod.bIsPassBin[iBinData]==1)
    {
        aTestResult="PASS";
    }
    else
    {
        aTestResult="FAIL";
    }

    slASECLTestInfor->Add("ASE");                                               //SiteID
    slASECLTestInfor->Add(aRecipe);                                             //ProjectCode
    slASECLTestInfor->Add(aHandler);                                            //TesterID
    slASECLTestInfor->Add(aTestDate);                                           //DATE
    slASECLTestInfor->Add(aTestTime);                                           //TIME
    slASECLTestInfor->Add(aLotName);                                            //LOT NAME
    slASECLTestInfor->Add(LotInName);                                           //LOGIN MODE
    slASECLTestInfor->Add(sBarcode);                                            //SERIAL NUMBER    2D barcode
    slASECLTestInfor->Add(aSocketID);                                           //SOCKETID
    slASECLTestInfor->Add(aTestResult);                                         //TEST RESULT

//    slASECLTestInfor->Add(" ");        //Error

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j]==true)
            {
                if(i==0 && j==0)
                    aTestCT=aTestCT+"1";
                else
                    aTestCT=aTestCT+"1";                                        //JerryYang 20190625 fix log
                iCT++;
            }
            else
            {
                if(i==0 && j==0)
                    aTestCT=aTestCT+"0";
                else
                    aTestCT=aTestCT+"0";                                        //JerryYang 20190625 fix log
            }
        }
    }

//    slASECLTestInfor->Add(iCT);             //Socket CT
    slASECLTestInfor->Add(aTestCT);                                             //Socket

    fputs(slASECLTestInfor->CommaText.c_str(), Fp);
    fputs("\n", Fp);                                                            //JerryYang 20190625 fix log
    fclose(Fp);

//    TastCategory.iTotalSocket = 0;
//    TastCategory.iPassSocket  = 0;
//    iPerMinuteNumberofError   = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::SaveASECLTesterHardwareLog()
{
    AnsiString str, str1, str2;
    FILE *Fp;
    AnsiString aRecipe=" ", aTesterID=" ", aLotName=" ", aHandler=" ", aDate, aTime, aLoadBard=" ";
    AnsiString Target;
    AnsiString asSocketCnt[32];
    ZeroMemory(asSocketCnt, sizeof(asSocketCnt));
    MyForceDirectories(asSaveEventLogPath);
    aRecipe     = fMain->cbSetupFileName->Text;                                 //JerryYang 20190702 fix log
    aTesterID   = edtASECL_TesterID->Text;
    aLotName    = edtASECL_LotID->Text;
    aHandler    = IniConfig.SocketHandlerID;
    aLoadBard   = edtASECL_LoadBoard->Text;
    aDate.sprintf("%04d/%02d/%02d", SystemYear, SystemMonth, SystemDate);
    aTime.sprintf("%02d:%02d:%02d", SystemHour, SystemMin, SystemSec);

    str.sprintf("TESTER LOG_%s_%04d_%02d_%02d", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);
    aBackTestLogFile=str;
    Target = asSaveEventLogPath+"\\"+str+".csv";

    if(FileExists(Target)==false)
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;

        str.sprintf("SiteID,ProjectID,TesterID,DATE,TIME,LOT NAME,LOAD BOARD");

        for(int i=0; i<MAX_Index_Row; i++)                                      //Eliot 2009_12_27 2->MAX_Index_Row
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)                              //Eliot 2009_12_27 4->MAX_Index_Col
            {
                if(TestIF_File.iSiteMap[i][j]>0)                                //JerryYang 20190702 fix log
                {
                    str1.sprintf(",SOCKET%d", TestIF_File.iSiteMap[i][j]);
                    str2.sprintf(",SOCKET%d QTY", TestIF_File.iSiteMap[i][j]);
                    str=str+str1+str2;
                }
            }
        }
        str=str+"\n";
        fputs(str.c_str(), Fp);
    }
    else
    {
        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
    }

    str.sprintf("%s,%s,%s,%s,%s,%s,%s", aHandler, aRecipe, aTesterID, aDate, aTime, aLotName, aLoadBard);

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            if(TestIF_File.iSiteMap[i][j]>0)                                    //JerryYang 20190702 fix log
            {
                str1.sprintf(",%s,%d", edSocket[i][j]->Text, LastSet.iSocketContactCount[i][j]);                        //JerryYang 20190421 fix log問題
                str=str+str1;
            }
        }
    }
    str=str+"\n";

    fputs(str.c_str(), Fp);
    fclose(Fp);

    UploadEventLogFile(aBackTestLogFile);
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::SaveOEELog(bool bForce, bool bClear)
{
//    return; //暫時不用
    AnsiString Target, S, Target1, aPath, aTargetPath;
    FILE *Fp;
    AnsiString str, str1, str3;
//    int iCT=0, iRow, iCol, iNowTime;
    AnsiString aTestCT="", aLotName, LotInName, aSocketID=" ";
    AnsiString aTestDate, aNowTime, aRecipe, aHandler, aTesterID, aTestResult;
//    static int iTotalOld=0, iPassOld=0, iFailOld=0;                           //KaiChen 20200316 :Add
    int iTotalBarcodeFailCnt=0;//, iMTBF=0;                                        //KaiChen 20200316 :Add
//    static bool bFirstTime=true;//, bUpdataTimeData=false;
    static int iLastCnt[10]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static iLastHour=-1;
    int iIdleTime=0, iOPerateTime=0, iDownTime=0, iTempOPerateTime=0, iTempUnloadCnt=0, iUPH=0, iTemploadCnt=0;
//    int iTotal=0, iSaveTimePerHour=10000;

    if(bClear)
    {
        iLastHour=SystemHour;
        iLastCnt[0]=0;
        iLastCnt[1]=0;
        iLastCnt[2]=0;
        iLastCnt[3]=0;
        iLastCnt[4]=0;
        iLastCnt[5]=0;
        iLastCnt[6]=0;
        iLastCnt[7]=0;
        iLastCnt[8]=0;
        iLastCnt[9]=0;
        return;
    }

    if(LastSet.iRealDummy!=REALLY)
        return;

    if(btnASECL_LotStart->Down==false)
        return;

    AnsiString sPathName;

    str.sprintf("%s@Lot OEE_%04d-%02d-%02d", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);

    MyForceDirectories(asSaveEventLogPath, __FUNC__);

    aPath=as9045LogPath+"\\ASE log";
    aTargetPath.sprintf("%s\\%04d\\%02d\\%02d\\", aPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(aTargetPath, __FUNC__);

    aLotName  = edtASECL_LotID->Text;
    LotInName = edtASECL_OPID->Text;
//    str.sprintf("TEST SUMMARY_%s_%04d_%02d_%02d", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate);
    Target=aTargetPath+str+".csv";

    if(FileExists(Target)==false)
    {
        Fp=fopen(Target.c_str(),"a+");
        if(Fp==NULL)
            return;
        str1.sprintf("Lot ID,Operator ID,Start Date Time,UPH Hourly Time,System Up Time,Operation Time,Down Time,Idle Time,Maintainance Time,TC Up Time,TC Down Time,TC Idle Time,Total Barcode Fail,Total Vision Pass,Total Vision Fail ,Lot Size,Total Quantity In,Total Quantity Out,Overall Total Passed,Overall Total Failed,Overall Total Non-Test,Overall Total Special Bin,Production UPH,Sprint UPH,Soft Jam,Hard Jam,MTBF,MTBA\n");
        fputs(str1.c_str(), Fp);

        if(bForce==false)
        {
            fclose(Fp);
            return;
        }
    }
    else
    {
        if(bForce==true)
        {
        }
        else
        {
            if(SystemHour!=iLastHour)                                           //JerryYang 20180515 記錄Time data
            {
                iLastHour=SystemHour;
            }
            else
            {
                return;
            }
        }

        Fp=fopen(Target.c_str(), "a+");
        if(Fp==NULL)
            return;
    }

    slASECLTestInfor->Clear();
    aRecipe     = fMain->cbSetupFileName->Text;                                 //JerryYang 20190702 fix log
    aHandler    = IniConfig.SocketHandlerID;
    aTesterID   = edtASECL_TesterID->Text;
//    aNowTime   = Now().FormatString("yyyy/mm/dd hh:nn:ss");
    aNowTime.sprintf("%04d/%02d/%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

//    if(LastSet.iJamCount[1]==0)
//    {
//        iMTBF=((LastSet.SystemAccSecond[0][stPauseTime]+LastSet.SystemAccSecond[0][stProductTime]+LastSet.SystemAccSecond[0][stJamTime])/1000);
//    }
//    else
//    {
//        iMTBF=((LastSet.SystemAccSecond[0][stPauseTime]+LastSet.SystemAccSecond[0][stProductTime]+LastSet.SystemAccSecond[0][stJamTime])/1000)/LastSet.iJamCount[1];  //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位    //Isaac 20180417 (Steven) 修正MTBF公式(pause+production+jam)/jamcount
//    }

    for(int i=0; i<4; i++)
    {
        iTotalBarcodeFailCnt+=iBarcodeErrorCount[i];
    }

    slASECLTestInfor->Add(aLotName);                                            //LOT NAME
    slASECLTestInfor->Add(LotInName);                                           //LOGIN MODE
    slASECLTestInfor->Add(RunInfo.LotStartTime);                                //LOT START TIME
    slASECLTestInfor->Add(aNowTime);                                            //UPH Hourly Time
    slASECLTestInfor->Add(IntToStr(int(iSystemUpTime/1000)));                   //System Up Time

    iOPerateTime=int(LastSet.SystemAccSecond[0][stStartTime]/1000);
    if((iOPerateTime-iLastCnt[7])>0)
    {
        slASECLTestInfor->Add(IntToStr(iOPerateTime-iLastCnt[7]));              //Operation Time

        iTempOPerateTime=iOPerateTime-iLastCnt[7];
    }
    else
    {
        slASECLTestInfor->Add("0");                                             //Operation Time
    }
    iLastCnt[7]=iOPerateTime;

    iDownTime=int(LastSet.SystemAccSecond[0][stPauseTime]/1000);
     if((iDownTime-iLastCnt[8])>0)
    {
        slASECLTestInfor->Add(IntToStr(iDownTime-iLastCnt[8]));                 //Down Time
    }
    else
    {
        slASECLTestInfor->Add("0");                                             //Operation Time
    }
    iLastCnt[8]=iDownTime;

//    slASECLTestInfor->Add(IntToStr(int(LastSet.SystemAccSecond[0][stPauseTime]/1000)));       //Down Time

    iIdleTime=int(((LastSet.SystemAccSecond[0][stPowerOn]-LastSet.SystemAccSecond[0][stStartTime]-LastSet.SystemAccSecond[0][stPauseTime])/1000));
     if((iIdleTime-iLastCnt[9])>0)
    {
        slASECLTestInfor->Add(IntToStr(iIdleTime-iLastCnt[9]));                 //Down Time
    }
    else
    {
        slASECLTestInfor->Add("0");                                             //Operation Time
    }
    iLastCnt[9]=iIdleTime;

//    slASECLTestInfor->Add(IntToStr(int(((LastSet.SystemAccSecond[0][stPowerOn]-LastSet.SystemAccSecond[0][stStartTime]-LastSet.SystemAccSecond[0][stPauseTime])/1000)))); //Idle Time
    slASECLTestInfor->Add("0");                                                 //Maintainance Time
    slASECLTestInfor->Add("0");                                                 //System Up Time
    slASECLTestInfor->Add("0");                                                 //Down Time
    slASECLTestInfor->Add("0");                                                 //Idle Time

    if(iTotalBarcodeFailCnt>iLastCnt[0])
    {
        slASECLTestInfor->Add(IntToStr(iTotalBarcodeFailCnt-iLastCnt[0]));      //Total Barcode Fail
    }
    else
    {
        slASECLTestInfor->Add("0");                                             //Total Barcode Fail
    }
    iLastCnt[0]=iTotalBarcodeFailCnt;

    slASECLTestInfor->Add("0");                                                 //Total Vision Pass
    slASECLTestInfor->Add("0");                                                 //Total Vision Fail
    slASECLTestInfor->Add("0");                                                 //Lot size

    if(LastSet.SendCT[0]>iLastCnt[1])                                           //Total Quantity In
    {
        iTemploadCnt=LastSet.SendCT[0]-iLastCnt[1];
        slASECLTestInfor->Add(IntToStr(LastSet.SendCT[0]-iLastCnt[1]));
    }
    else
    {
        slASECLTestInfor->Add("0");                                             //Total Quantity In
    }
    iLastCnt[1]=LastSet.SendCT[0];

//    slASECLTestInfor->Add(IntToStr(LastSet.SendCT[0]));                          //Total Quantity In

    if(RunInfo.iUnloadCount>iLastCnt[2])                                        //Total Quantity Out
    {
        iTempUnloadCnt=RunInfo.iUnloadCount-iLastCnt[2];
        slASECLTestInfor->Add(IntToStr(iTempUnloadCnt));
    }
    else
    {
        slASECLTestInfor->Add("0");                                             //Total Quantity In
    }
    iLastCnt[2]=RunInfo.iUnloadCount;
//    slASECLTestInfor->Add(IntToStr(RunInfo.iUnloadCount));                       //Total Quantity Out

    if(iSECSGEMPass>iLastCnt[3])                                                //Overall Total Passed
    {
        slASECLTestInfor->Add(IntToStr(iSECSGEMPass-iLastCnt[3]));
    }
    else
    {
        slASECLTestInfor->Add("0");
    }
    iLastCnt[3]=iSECSGEMPass;
//    slASECLTestInfor->Add(IntToStr(iSECSGEMPass));                               //Overall Total Passed

    if(iSECSGEMFail>iLastCnt[4])                                                //Overall Total Failed
    {
        slASECLTestInfor->Add(IntToStr(iSECSGEMFail-iLastCnt[4]));
    }
    else
    {
        slASECLTestInfor->Add("0");
    }
    iLastCnt[4]=iSECSGEMFail;
//    slASECLTestInfor->Add(IntToStr(iSECSGEMFail));                             //Overall Total Failed
    slASECLTestInfor->Add("0");                                                 //Overall Total Non-Test
    slASECLTestInfor->Add("0");                                                 //Overall Total Special Bin

//    slASECLTestInfor->Add(IntToStr(RunInfo.iUPH));                               //Production UPH
    if(iTempOPerateTime>0 && iTempUnloadCnt>0)
    {
        iUPH=double(3600.0/iTempOPerateTime)*iTempUnloadCnt;
        slASECLTestInfor->Add(IntToStr(iUPH));                                  //Production UPH
    }
    else
    {
        slASECLTestInfor->Add("0");                                             //Production UPH
    }

    slASECLTestInfor->Add("0");                                                 //Sprint UPH

    if(LastSet.iJamCount[0]>iLastCnt[5])                                        //Soft Jam
    {
        slASECLTestInfor->Add(IntToStr(LastSet.iJamCount[0]-iLastCnt[5]));
    }
    else
    {
        slASECLTestInfor->Add("0");
    }

//    slASECLTestInfor->Add(IntToStr(LastSet.iJamCount[0]));                       //Soft Jam
    slASECLTestInfor->Add("0");                                                 //Hard Jam
    slASECLTestInfor->Add("0");

    if(iTempOPerateTime>0)
    {
        if(LastSet.iJamCount[0]>iLastCnt[5])
        {
            slASECLTestInfor->Add(IntToStr(iTempOPerateTime/(LastSet.iJamCount[0]-iLastCnt[5])));
        }
        else
        {
            slASECLTestInfor->Add(IntToStr(iTempOPerateTime));
        }
    }
    else
    {
        slASECLTestInfor->Add("0");
    }
    iLastCnt[5]=LastSet.iJamCount[0];
    iLastCnt[6]=LastSet.SystemAccSecond[0][stPowerOn];
    if(iTemploadCnt==0 && iTempUnloadCnt==0)
    {
    }
    else
    {
        fputs(slASECLTestInfor->CommaText.c_str(), Fp);
        fputs("\n", Fp);                                                        //JerryYang 20190625 fix log
    }
    fclose(Fp);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::ShowSocketID()                                       //JerryYang 20190702 ASE-CL顯示SocketID
{
    int iMode=TestIF_File.iTestMode;
    int iTestCHCT=SiteData[iMode].Cnt;

    if(IniConfig.bDualSiteSupply4CH==true)                                      //jou 2012-11-20 Dual Site supply 4's Channel
    {
        if(TestIF_File.iTestMode==DualSite)
        {
            iTestCHCT+=2;
        }
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction  //JerryYang 20230204 : SPIL中科要求改回來一樣顯示8 site
    {
        if((TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4) &&                               //Sam 20190226 : 16Site4X4
            CosFunction.bEnableOctal_12Kit==true)                               //jou 2015-05-14 Octal Site supply 12 sites channel
        {
            if(TestIF_File.bOctal_12Kit==true)
                iTestCHCT+=4;
            else
                iTestCHCT=SiteData[iMode].Cnt;
        }
    }

    if(CosFunction.bUse32ChanelSiteMap)                                         //Steven 20170530 (wei) : Use 32CH site map
    {
        iTestCHCT=32;

        if(IniConfig.bVTESTFunction)                                            //RogerYang 20260529 : 偉測趙坤鵬說開放到最多16site
        {
            iTestCHCT=16;
        }
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        SocketLabRow_Display[i]->Visible=false;
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            SocketSiteCH_Display[i][j]->Caption="";
            SocketSiteCH_Display[i][j]->Visible=false;
            SocketLabCol_Display[j]->Visible=false;
            edSocket[i][j]->Visible=false;
        }
    }

    for(int i=0; i<SiteData[iMode].XItem; i++)
    {
        SocketLabCol_Display[i]->Visible=true;
        for(int j=0; j<SiteData[iMode].YItem; j++)
        {
            SocketSiteCH_Display[j][i]->Visible=true;
            SocketLabRow_Display[j]->Visible=true;
            edSocket[j][i]->Visible=true;
        }
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            if(TestIF_File.iSiteMap[i][j]==0)
            {
                SocketSiteCH_Display[i][j]->Caption="";
            }
            else
            {
                SocketSiteCH_Display[i][j]->Caption=("CH "+AnsiString (TestIF_File.iSiteMap[i][j]));
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnQAmodeSaveClick(TObject *Sender)                  //JerryYang 20190701 SPIL要求主畫面可切換EQC mode
{
    AnsiString szDir=GetRecipePath();
    MyForceDirectories(szDir);
    szDir=GetRecipeFileName("Tester.Data");
    TestIF_File.iQAModeCount        =CheckRange(atoi(edQAMode->Text.c_str()), 5, 10000);
    WriteIniData(szDir, "QA Mode", "Count", int(TestIF_File.iQAModeCount));     //Steven 20111026
    fQAMode->ReadFile();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edQAModeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 5);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edPageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_Murata)
        return;
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 20);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::LotKeyInTimeTimer(TObject *Sender)
{
    static bool bRunLotKeyInTime=false;
    static int iCount=0;
    AnsiString sLastKeyin = "";
    if(InitialOK==false || bRunLotKeyInTime==true)
        return;
    if(bLotFirstKeyIn==true)                                                    //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bLotFirstKeyIn=false;
        iCount=0;
        return;
    }

    bRunLotKeyInTime=true;

    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        if(edtSysLotID->Text.Length()!=10)                                      //Ifor 20190919 : add BarCode Reader Lot ID
        {
            SetLotID("");
        }
        else
        {
            if(sLastKeyin=="")
            {
                sLastKeyin=edtSysLotID->Text;

                if(sLastKeyin.Length()!=10)
                {
                    SetLotID("");
                    sLastKeyin="";
                }
            }
            else
            {
                SetLotID(sLastKeyin);
            }
        }

        sLastKeyin="";
        if(TrayForm.bEnableAMR ||                                               //Eastsun 20260515 F010 AMR/AGV Operator ID
           (TrayForm.bEnableAMR==false && TrayForm.bEnableAMRLoader==true &&
           (fMain->cbRunStartMode->Text=="Re-Test Continuous" ||
           fMain->cbRunStartMode->Text=="Re-Test Initial Start")) && edtSysOperatorID->Text=="AGV")
        {
            sLastKeyin=edtSysOperatorID->Text;
            edtSysOperatorID->Text=sLastKeyin;
        }
        else if(edtSysOperatorID->Text.Length()<6 ||                            //Ifor 20190919 : add BarCode Reader Operator ID
           edtSysOperatorID->Text.Length()>7)                                   // 2013.12.17 , Joye , KYEC Barcdoe Reader
        {
            edtSysOperatorID->Text="";
            sLastKeyin="";
        }
        else
        {
            if(sLastKeyin=="")
            {
                sLastKeyin=edtSysOperatorID->Text;
                AnsiString sHeadNum;
                int iHeadNum=0;
                if(sLastKeyin.Length()==6)
                {
                    sHeadNum=sLastKeyin.SubString(1, 2);
                    iHeadNum=atoi(sHeadNum.c_str());
                }
                else if(sLastKeyin.Length()==7)
                {
                    sHeadNum=sLastKeyin.SubString(1,3);
                    iHeadNum=atoi(sHeadNum.c_str());
                }
                                                                                //Ifor 20180517 : add BarCode 編碼規則 0:民國 1: 西元
                if(bEnable_KLT_Function==true)                                  //Ifor 20180802 (Steven) : add KLT bBarCodeRules -> bEnable_KLT_Function 0:KYEC 1: KLT
                {
                    if(iHeadNum<3 || iHeadNum>31)                               // KYEC 3~31 (2003~2031)
                    {
                        edtSysOperatorID->Text="";
                        sLastKeyin="";
                    }
                }
                else
                {
                    if(iHeadNum<85 || iHeadNum>120)                             // KYEC 85~120 (85~120)
                    {
                        edtSysOperatorID->Text="";
                        sLastKeyin="";
                    }
                }
            }
            else
            {
                edtSysOperatorID->Text=sLastKeyin;
            }
        }
    }
    else if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        iCount++;
        if(iCount>=10)
        {
            iCount=0;
            if(bLotID_OK==false)
                edtSysLotID->Text="";

            if(bOPID_OK==false)
                edtSysOperatorID->Text="";

            if(bDeviceName_OK==false)
                edDeviceName->Text="";

            if(bTemp_OK==false)
                edTemp->Text="";
        }
    }

    bRunLotKeyInTime=false;
}
//---------------------------------------------------------------------------
TQPF_Timer tChamberBoostTime;
TQPF_Timer tChamberBoostTimer;
void __fastcall TfLotInfo::tmrChamberBoostTimer(TObject *Sender)
{
    //Steven 20191128 : Chamber Boost Function
    static bool bRun=false;
    static bool bFirstIn=true;
    AnsiString Str;
    int iTime;

    if(CosFunction.bUseChamberBoostMode==false || InitialOK==false || bRun)
        return;

    bRun=true;
    if(bStartChamberBoost)
    {
        if(bFirstIn)
        {
            tChamberBoostTime.SetSecAndOn(Temperature.iChamberBoostTime*60);
            tChamberBoostTimer.LatchCycleTimeSec(true);
            bFirstIn=false;
        }
        else
        {
            iTime=Temperature.iChamberBoostTime*60-tChamberBoostTimer.LatchCycleTimeSec();
            if(iTime<0)
                Str="";
            else if(iTime<60)
                Str.sprintf("%d Sec", iTime);
            else
                Str.sprintf("%d:%02d", iTime/60, iTime%60);
            pnlChamberBoost->Caption=Str;
            if(tChamberBoostTime.Off())
            {
                btnStopChamberBoost->Click();
                ShowErrorMessage("MES15401", K_SKIP, MMSystem, false);
            }
        }
    }
    else
    {
        bFirstIn=true;
        if(SystemStart)
        {
            btnStartChamberBoost->Enabled=false;
            btnStopChamberBoost->Enabled=false;
        }
        else if(InArmSuck.HasIC() || OutArmSuck.HasIC() || ShuttleHasIC() ||
                IndexHasIC() || AllArmZIsSafe()==false)
        {
            btnStartChamberBoost->Enabled=false;
            btnStopChamberBoost->Enabled=false;
        }
        else
        {
            btnStartChamberBoost->Enabled=true;
        }
        pnlChamberBoost->Caption="";
        if(CosFunction.bUseChamberBoostMode)
        {
            if(tsChamberBoost->TabVisible && bUT150Install[tcChamber]==false)
            {
                tsChamberBoost->TabVisible=false;
            }
            else if(tsChamberBoost->TabVisible==false && bUT150Install[tcChamber])
            {
                tsChamberBoost->TabVisible=true;
            }
        }
        else
        {
            tsChamberBoost->TabVisible=false;
        }
    }
    bRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnStartChamberBoostClick(TObject *Sender)
{
    bStartChamberBoost=true;                                                    //Steven 20191128 : Chamber Boost Function
    btnStartChamberBoost->Down=false;
    btnStartChamberBoost->Enabled=false;
    btnStopChamberBoost->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnStopChamberBoostClick(TObject *Sender)
{
    bStartChamberBoost=false;                                                   //Steven 20191128 : Chamber Boost Function
    btnStopChamberBoost->Down=false;
    btnStartChamberBoost->Enabled=true;
    btnStopChamberBoost->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysOperatorIDMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        return;
    }
    else if(IniConfig.bO23_InputLotIDByBarcode)                                 //Steven 20241224 : LotID只能用Barcode
    {
        AnsiString str, str2;
        TEdit *TempEdit=(TEdit *)Sender;
        if(TempEdit==edtCusLotID)                                               //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
        {
            str="Input Cus. lot ID:";
            str2="LotID";
        }
        else
        {
            str="Input OP ID:";
            str2="UserName";
        }
        TempEdit->Text=InputBarcodeNumber(str, str2);
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA ||                                     //Steven 20211112 : 通富微不可以用鍵盤輸入
            IniConfig.bVTESTFunction==true ||                                   //jou 20220912 : 增加VTEST不可以用鍵盤輸入
            (IniConfig.bSPILFunction &&
             TestIF_File.b2DIDAllowList &&                                      //JerryYang 20241104 : 支援2DID白名單功能
             AccessLevel==0))
    {
        edtSysOperatorID->Text=InputBarcodeNumber("Input OP ID:", "UserName");
    }
    else if(CUSTOMER_CODE==CC_SCC)                                              //Steven 20200302 : SCC楊恩民說輸入字串5~30個字元
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
        if(edtSysOperatorID->Text.Length()<5 || edtSysOperatorID->Text.Length()>30)
        {
            edtSysOperatorID->Text="";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysLotIDMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CUSTOMER_CODE==CC_Murata)
    {
    }
    else if(CUSTOMER_CODE==CC_PTI ||                                            //RogerYang 20170327 (Steven) 力成使用條碼機
            IniConfig.bO23_InputLotIDByBarcode)                                 //Steven 20241224 : LotID只能用Barcode
    {
        SetLotID("");
        AnsiString sBarcodeID=InputBarcodeNumber("Input Lot ID:", "LotID");
        SetLotID(sBarcodeID);
    }
    else if(CUSTOMER_CODE==CC_TFME_CHINA)
    {
        edtSysLotID->Text=InputBarcodeNumber("Input Lot ID:", "LotID");
    }
    else if(IniConfig.bSPILFunction &&
            TestIF_File.b2DIDAllowList && AccessLevel==0)                       //JerryYang 20241104 : 支援2DID白名單功能
    {
        edtSysLotID->Text=InputBarcodeNumber("Input Lot ID:", "LotID");
    }
    else if(CUSTOMER_CODE==CC_SCC)                                              //Steven 20200302 : SCC楊恩民說輸入字串5~30個字元
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
        if(edtSysLotID->Text.Length()<5 || edtSysLotID->Text.Length()>30)
        {
            SetLotID("");
        }
    }
    else if(IniConfig.bVTESTFunction==true)                                     //Steven 20211112 : 通富微不可以用鍵盤輸入  //jou 20220912 : 增加VTEST不可以用鍵盤輸入
    {
        edtSysLotID->Text=InputBarcodeNumber("Input Lot ID:", "LotID");
        edtSysOperatorID->Text=InputBarcodeNumber("Input OP ID:", "UserName");
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ClearAllSetupFile(AnsiString sSetupFile, AnsiString sSetupFile_Net)                                     //Steven 20200512 : 刪除全部工作檔, 只留下當下的
{
    WIN32_FIND_DATA filedata;                                                   // Structure for file data
    HANDLE filehandle;                                                          // Handle for searching
    AnsiString SPath[2]={IncludeTrailingPathDelimiter(DataPath), IncludeTrailingPathDelimiter(OffsetPath)};
    AnsiString SDataPath, sFileName;
    if(sSetupFile_Net!="")
        fBuilder->DeleteSetupFile(sSetupFile_Net);

    if(CosFunction.bKeepOnly1SetupFile)                                         //Steven 20200511 : 改成客戶功能 //wei 20131115 FTP下載後保留下載檔案，其餘Data刪除
    {
        for(int i=0; i<2; i++)
        {
            if(DirectoryExists(SPath[i]))
            {
                SDataPath=SPath[i]+"*.*";
                filehandle=FindFirstFile((SDataPath+"*").c_str(), &filedata);   //Steven 20101118 Start : 不要顯示資料夾以外的檔案
                if(filehandle!=INVALID_HANDLE_VALUE)
                {
                    do                                                          /* 不處理隱藏檔及 . 跟 .. */
                    {
                        if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                            strcmp(filedata.cFileName, ".")==0 ||
                            strcmp(filedata.cFileName, "..")==0)
                        {
                            continue;
                        }
                        else if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)                                   //如果該檔案為資料夾
                        {
                            sFileName=AnsiString(filedata.cFileName);
                            if(sFileName!=sSetupFile)
                            {
                                sFileName=SPath[i]+sFileName;
                                DeleteDirectory(sFileName);
                            }
                        }
                    } while(FindNextFile(filehandle, &filedata));
                    FindClose(filehandle);
                }
            }
        }
    }
    FindClose(filehandle);                                                      //Jimmychiu 20220901 釋放記憶體

    if(sSetupFile==fMain->cbSetupFileName->Text)                                //Steven 20220818 : Fixed for bD31RTCChangeRecipeNeedreCreateModel
        fSetup->bFirstTime=true;
    else
        fSetup->bFirstTime=false;

    RecordProcess("Clear All Setup File");
    fMain->cbSetupFileName->Clear();
    fMain->LookForFile();
    fMain->cbSetupFileName->Text=sSetupFile;
    WriteLastDataFN(sSetupFile);                                                //jou 2015-01-16 修正FTP download error
    fMain->ChangeSetUpFile(sSetupFile);                                         //Steven 20211013 : 修正刪除工作檔之後沒有切換
    if(CosFunction.bLastSetInSetUpFile)
        SetRunStartMode((eRunStartMode)LastSet.iRunStartMode);                  //Steven 20211028 : 要重新設定模式,避免OnLine/OffLine Bin沒切換
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysLotIDKeyPress(TObject *Sender, char &Key)
{
    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
        return;

    if(CUSTOMER_CODE==CC_Murata)
    {
        if(Key=='\r')
        {
            edPage->SetFocus();
        }
        else if(Key=='$')
        {
            edtSysLotID->Text=edtSysLotID->Text.SubString(1, edtSysLotID->Text.Length()-1);
            edPage->SetFocus();
        }
    }
    else if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        if(bLotID_OK==true)
            bLotID_OK=false;

        if(Key=='\r')
            bLotID_OK=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edPageKeyPress(TObject *Sender, char &Key)
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        if(Key=='\r')
        {
            edtSysOperatorID->SetFocus();
        }
        else if(Key=='$')
        {
            edPage->Text=edPage->Text.SubString(1, edPage->Text.Length()-1);
            edtSysOperatorID->SetFocus();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysOperatorIDKeyPress(TObject *Sender,
      char &Key)
{
    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
        return;

    if(CUSTOMER_CODE==CC_Murata)
    {
        if(Key=='\r')
        {
            cbRunMode->Text="";
            cbRunMode->SetFocus();
        }
        else if(Key=='$')
        {
            edtSysOperatorID->Text=edtSysOperatorID->Text.SubString(1, edtSysOperatorID->Text.Length()-1);
            cbRunMode->Text="";
            cbRunMode->SetFocus();
        }
    }
    else if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        if(bOPID_OK==true)
            bOPID_OK=false;

        if(Key=='\r')
            bOPID_OK=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbRunModeKeyPress(TObject *Sender, char &Key)
{
    if(CUSTOMER_CODE==CC_Murata)                                                //Steven 20200629 : Murata要求輸入barcode後, 自動跳下一個欄位
    {
        if(Key=='\r')
        {
            sbSECSLotStart->Click();
            sbSECSLotStart->Down=true;
            sbSECSLotEnd->Down=false;
        }
        else if(Key=='$')
        {
            cbRunMode->Text=cbRunMode->Text.SubString(1, cbRunMode->Text.Length()-1);
            sbSECSLotStart->Click();
            sbSECSLotStart->Down=true;
            sbSECSLotEnd->Down=false;
        }
    }

    if(fBarCode->JCETUseMakeWhite2DIDList()==true)                              //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        cbRunMode->Text="";                                                     //不允許手動輸入
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbRunModeKeyDown(TObject *Sender, WORD &Key,         //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
      TShiftState Shift)
{
    if(fBarCode->JCETUseMakeWhite2DIDList()==true)                              //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        cbRunMode->Text="";                                                     //不允許手動輸入
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbRunModeKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(fBarCode->JCETUseMakeWhite2DIDList()==true)                              //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能
    {
        cbRunMode->Text="";                                                     //不允許手動輸入
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edDeviceNameKeyPress(TObject *Sender, char &Key)
{
    if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        if(bDeviceName_OK==true)
            bDeviceName_OK=false;

        if(Key=='\r')
        {
            bDeviceName_OK=true;
        }
    }
    else if(CosFunction.bScanBarcodeAndDownloadFileInRMS==true)
    {
        bool bKey=(Key=='\r');
        if(bKey)
        {
            lbShowDevName->Caption=edDeviceName->Text;
            RecordProcess("ScanBarcodeAndDownloadFileInRMS Name:%s",(lbShowDevName->Caption).c_str());
            btDownload->Click();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edTempKeyPress(TObject *Sender, char &Key)
{
    if(CUSTOMER_CODE==CC_AMD_M && CosFunction.bHiSiliconFunction==true)
    {
        if(bTemp_OK==true)
            bTemp_OK=false;

        if(Key=='\r')
            bTemp_OK=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edDeviceNameKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(bLotFirstKeyIn==false)                                                   //Ifor 20190924 : add Barcode 輸入判斷避免讀碼失敗
    {
        bLotFirstKeyIn=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edTempKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(CUSTOMER_CODE==CC_AMD_M &&
       CosFunction.bHiSiliconFunction==true &&
       bTemp_OK==true)                                                          //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bTemp_OK=false;
        edTemp->Text="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysLotIDKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        return;
    }

    if(CUSTOMER_CODE==CC_AMD_M &&
       CosFunction.bHiSiliconFunction==true &&
       bLotID_OK==true)                                                         //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bLotID_OK=false;
        edtSysLotID->Text="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtSysOperatorIDKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(IniConfig.bO23_InputLotIDByBarcode)                                      //Steven 20241224 : LotID只能用Barcode
    {
        return;
    }

    if(CUSTOMER_CODE==CC_AMD_M &&
       CosFunction.bHiSiliconFunction==true &&
       bOPID_OK==true)                                                          //Ifor 20200827 add:避免刷兩次Barcode造成異常
    {
        bOPID_OK=false;
        edtSysOperatorID->Text="";
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ShowXMLOnLine()                                                 //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
{
    if(CUSTOMER_CODE==CC_Murata)
    {
        if(iXMLOnLineStatus==0)
        {
            if(IniConfig.bN10_9_UploadUnloadTrayToFTP ||
               IniConfig.bN23_1_Enable2DIDCompare ||
               IniConfig.bN23_3_UploadTestResult)
            {
                pnlXMLOnLine->Caption="Server On Line";
                pnlXMLOnLine->Color=clLime;
                RecordProcess("Server function on Line");
            }
            else
            {
                pnlXMLOnLine->Caption="Server Off Line";
                pnlXMLOnLine->Color=clRed;
                RecordProcess("Server function off Line");
            }
        }
        else if(iXMLOnLineStatus==1)
        {
            pnlXMLOnLine->Caption="Server On Line";
            pnlXMLOnLine->Color=clLime;
            RecordProcess("Server function on Line");
        }
        else
        {
            pnlXMLOnLine->Caption="Server Off Line";
            pnlXMLOnLine->Color=clRed;
            RecordProcess("Server function off Line");
        }
    }
    else
    {
        pnlXMLOnLine->Visible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::pnlXMLOnLineClick(TObject *Sender)                   //Steven 20200629 : Murata要求可以按按鈕後停止Server功能
{
    if(fSecurity->Insufficient(164)==false || HasICUnderMachine() || HasAnyICInMachine())
        return;

    //0: 由Congig控制
    //1: 連線模式
    //2: 離線模式
    if(IniConfig.bN10_9_UploadUnloadTrayToFTP ||
       IniConfig.bN23_1_Enable2DIDCompare ||
       IniConfig.bN23_3_UploadTestResult)
    {
        if(iXMLOnLineStatus==2)
        {
            iXMLOnLineStatus=1;
        }
        else
        {
            iXMLOnLineStatus=2;
        }
    }
    else
    {
        iXMLOnLineStatus=0;
    }
    ShowXMLOnLine();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::WhenTestRecordTemperatureLog_3Sigma(AnsiString aFileName, int iUseIndex)                     //kevin 20200521 add 3 SIGMA  Hmy 20200510 Add Enable 3 Sigma Temp Monitor//Hmy 20200510 Add Enable 3 Sigma Temp Monitor
{
    AnsiString TempNamePnATC[]={"Head1", "Head2", "Head3", "Head4", "Head5", "Head6", "Head7", "Head8"};
    AnsiString TempNamePnATC_forATC7[]={"Head1-1", "Head1-2", "Head1-3", "Head1-4", "Head1-5", "Head1-6", "Head1-7", "Head1-8",
                                        "Head2-1", "Head2-2", "Head2-3", "Head2-4", "Head2-5", "Head2-6", "Head2-7", "Head2-8"};

    TDateTime tdSaveTime=Now();
    AnsiString asLotIDPath=as3SigmaLogPath +"\\"+sOSATLotID;                    //edtSysLotID->Text;
    AnsiString asTxtName="";
    AnsiString asPath, asFN;
    AnsiString asHeadName_Temp;
    FILE *P;
    static TStringList *sList=new TStringList();
//    int iListCount;
    AnsiString asData1, asData2;
    int iListTemperatureData;
    double *dListTemperatureData;
    double daverage[3]={0.0, 0.0, 0.0};                                         //平均值
    double div[4]={0.0, 0.0, 0.0, 0.0};                                         //標準差

    MyForceDirectories(as3SigmaLogPath);
    MyForceDirectories(asLotIDPath);
    if(Temperature.i3SigmaTempMonitior_SetCoun<=0)
    {
        sList->Clear();
        return;
    }

    //if(i3SigmaTempMonitior_NowContactCount < Temperature.i3SigmaTempMonitior_SetCoun)
    {
        i3SigmaTempMonitior_NowContactCount++;

        if(iUseIndex==0)
        {
            for(int i=0; i<4; i++)
            {
                if(ATCPtr[i]->Caption!="---" &&
                   ATCPtr[i]->Caption!="--" &&
                   ATCPtr[i]->Caption!="0.0 'C" &&
                   ATCPtr[i]->Caption!="NA")                                    //kevin 20200629 add
                {
                    asHeadName_Temp = TempNamePnATC[i] +"_" + ATCPtr[i]->Caption;
                    sList->Add(asHeadName_Temp);
                }
            }
        }
        else if(iUseIndex==1)
        {
            for(int i=4; i<8; i++)
            {
                if(ATCPtr[i]->Caption!="---" &&
                   ATCPtr[i]->Caption!="--" &&
                   ATCPtr[i]->Caption!="0.0 'C" &&
                   ATCPtr[i]->Caption!="NA")                                    //kevin 20200629 add
                {
                    asHeadName_Temp=TempNamePnATC[i]+"_"+ATCPtr[i]->Caption;
                    sList->Add(asHeadName_Temp);
                }
            }
        }
    }

    if(i3SigmaTempMonitior_NowContactCount>=Temperature.i3SigmaTempMonitior_SetCoun)                                    //kevin 20200629 mark
    {
        i3SigmaTempMonitior_NowContactCount=0;
        iListTemperatureData=sList->Count;
        dListTemperatureData=(double*)malloc(sizeof(double) * iListTemperatureData);
        for(int i=0; i<iListTemperatureData; i++)
        {
            asData1=sList->Strings[i];
            asData2=asData1.SubString(asData1.Pos("_")+1, asData1.Length());
            daverage[0]=atof(asData2.c_str());
            dListTemperatureData[i]=daverage[0];
            daverage[1]+=daverage[0];
        }
        daverage[2]=ChangeToFloatNonPcnt((double)(daverage[1]), (double)(iListTemperatureData));                        //平均值
        for(int i=0; i<iListTemperatureData; i++)
        {
            div[0]=pow((dListTemperatureData[i]-daverage[2]), 2);
            div[1]+=div[0];
        }

        div[2]=ChangeToFloatNonPcnt((double)(div[1]), (double)(iListTemperatureData-1)); //Steven 20260505 : add zero-guard for (iListTemperatureData-1)
        div[3]=sqrt(div[2]);
        div[3]=div[3]*3;
        if(Temperature.d3SigmaTempMonitior_Set3xSigmaValue<=div[3])
        {
            ShowErrorMessage("WAR16314", K_RETRY, MMSystem);                    //kevin 20200630 change
        }

        asFN.sprintf("%s\\%s.txt", asLotIDPath.c_str(), tdSaveTime.FormatString("yyyymmdd_hhnnss").c_str());
        P=fopen(asFN.c_str(), "a+");
        if(P!=NULL)
        {
            for(int i=0; i<iListTemperatureData; i++)
            {
                fputs(sList->Strings[i].c_str(), P);
                fputs("\n", P);
            }
            AnsiString as1;
            as1="平均值"+FloatToStr(daverage[2]);
            fputs(as1.c_str(), P);
            fputs("\n", P);
            as1="標準差"+FloatToStr(div[3]);
            fputs(as1.c_str(), P);
            fputs("\n", P);
            fclose(P);
        }

        if(dListTemperatureData!=NULL)
        {
            free(dListTemperatureData);                                         //Steven 20260612 : Fix malloc/delete mismatch - use free() to match malloc()
            dListTemperatureData=NULL;                                          //Hmy 20180809 Modify
        }
        sList->Clear();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnSaveDataClick(TObject *Sender)
{
    if(RunInfo.bLotStart)
    {
        ShowMyMessage("save data error,need lot end!!");
        return;
    }

    if(SystemStart==false)                                                      //frank 20200814 : 每10盤記錄一次summary log
        SaveTrayRecord(10);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnCancelTestPauseClick(TObject *Sender)
{
    bTesterSendPause=false;
    bTesterPauseMusic=false;                                                    //Steven 20220616 : Can select "Alarm Reset" when show "Tester Pause" for QORVO.
    bPauseAlarmDelayActive=false;                                               //RogerYang 20260626 : 手動取消 Pause 一併清逾時計時
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnESCFunctionClick(TObject *Sender)
{
    if(EmptySocketCheckModeBeUse() && IniConfig.bI41_6_Manual)                  //Steven 20201022 : For RFMD Empty Socket Check Funstion.
    {
        fMain->ResetForESC("Start Empty Socket OneCycle by manual");
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::SetCheckCodeByLot(bool _enable)                                 //JimmyChiu 20211014 :   Show Duplication function "Enable" or "Disable" on the Lot Info.
{
    lbCheckCodeByLot->Caption="Check duplicate code by lot：";
    if(_enable)
    {
        lbCheckCodeByLot->Caption=lbCheckCodeByLot->Caption+"Enable";
        lbCheckCodeByLot->Color=clLime;
    }
    else
    {
        lbCheckCodeByLot->Caption=lbCheckCodeByLot->Caption+"Disable";
        lbCheckCodeByLot->Color=clRed;
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ShowInformation(bool bShow)                                     //KaiChen 20191111 ：權限卡關顯示
{
    if(bShow==true)
    {
        gbFTPAutomation_Download    ->Visible=true;
        gbFTPAutomation_Upload      ->Visible=true;
        sbRecipeUpload              ->Visible=true;
        sbRecipeDownload            ->Visible=true;
        sbFTPAutomationSave         ->Visible=true;

        if(IniConfig.bSIGURDFunction && pgLotinfo->ActivePageIndex==18)         //KaiChen 20191210 ：矽格-湖口，FTPAutomation登出時，頁面縮小
        {
            fLotInfo->Height=390;
            fLotInfo->Width=475;
            sbTest->Top=304;
            this->Top=205;
            this->Left=340;
        }
    }
    else
    {
        gbFTPAutomation_Download    ->Visible=false;
        gbFTPAutomation_Upload      ->Visible=false;
        sbRecipeUpload              ->Visible=false;
        sbRecipeDownload            ->Visible=false;
        sbFTPAutomationSave         ->Visible=false;

        if(IniConfig.bSIGURDFunction && pgLotinfo->ActivePageIndex==18)         //KaiChen 20191210 ：矽格-湖口，FTPAutomation登出時，頁面縮小
        {
            fLotInfo->Height=150;
            fLotInfo->Width=150;
            sbTest->Top=50;
            this->Top=592;
            this->Left=217;
        }
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ReadWriteFTPAutomationData(bool bRead)                          //KaiChen 20190530 ：Sigurd FTP Automation
{
    AnsiString sPath=AuthPath+"config.ini";
    if(bRead)
    {
        FTPAutomation_Down_ServerIP     =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_ServerIP",          FTPAutomation_Down_ServerIP,            "192.168.119.243",          bRead);  //Sam 20220831 : 矽格北興俊堯要求修改預設值
        FTPAutomation_Down_UserID       =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_UserID",            FTPAutomation_Down_UserID,              "autoload",                 bRead);
        FTPAutomation_Down_Password     =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_Password",          FTPAutomation_Down_Password,            "autoload",                 bRead);
        FTPAutomation_Down_ServerPath   =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_ServerPath",        FTPAutomation_Down_ServerPath,          "/download/HT-9046HA-8H/",  bRead);
        FTPAutomation_Up_ServerIP       =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_ServerIP",            FTPAutomation_Up_ServerIP,              "192.168.119.243",          bRead);
        FTPAutomation_Up_UserID         =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_UserID",              FTPAutomation_Up_UserID,                "autoload",                 bRead);
        FTPAutomation_Up_Password       =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_Password",            FTPAutomation_Up_Password,              "autoload",                 bRead);
        FTPAutomation_Up_ServerPath     =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_ServerPath",          FTPAutomation_Up_ServerPath,            "/upload/HT-9046HA-8H/",    bRead);
        FTPAutomation_Up_ErrorCodePath  =ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_ErrorCodePath",       FTPAutomation_Up_ErrorCodePath,         "/upload/error_code/",      bRead);

        edFTPAutomation_Down_ServerIP->Text         =FTPAutomation_Down_ServerIP;
        edFTPAutomation_Down_UserID->Text           =FTPAutomation_Down_UserID;
        edFTPAutomation_Down_Password->Text         =FTPAutomation_Down_Password;
        edFTPAutomation_Down_ServerPath->Text       =FTPAutomation_Down_ServerPath;
        edFTPAutomation_Up_ServerIP->Text           =FTPAutomation_Up_ServerIP;
        edFTPAutomation_Up_UserID->Text             =FTPAutomation_Up_UserID;
        edFTPAutomation_Up_Password->Text           =FTPAutomation_Up_Password;
        edFTPAutomation_Up_ServerPath->Text         =FTPAutomation_Up_ServerPath;
        edFTPAutomation_Up_ErrorCodePath->Text      =FTPAutomation_Up_ErrorCodePath;
    }
    else
    {
        FTPAutomation_Down_ServerIP         =edFTPAutomation_Down_ServerIP->Text.c_str();
        FTPAutomation_Down_UserID           =edFTPAutomation_Down_UserID->Text.c_str();
        FTPAutomation_Down_Password         =edFTPAutomation_Down_Password->Text.c_str();
        FTPAutomation_Down_ServerPath       =edFTPAutomation_Down_ServerPath->Text.c_str();
        FTPAutomation_Up_ServerIP           =edFTPAutomation_Up_ServerIP->Text.c_str();
        FTPAutomation_Up_UserID             =edFTPAutomation_Up_UserID->Text.c_str();
        FTPAutomation_Up_Password           =edFTPAutomation_Up_Password->Text.c_str();
        FTPAutomation_Up_ServerPath         =edFTPAutomation_Up_ServerPath->Text.c_str();
        FTPAutomation_Up_ErrorCodePath      =edFTPAutomation_Up_ErrorCodePath->Text.c_str();

        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_ServerIP",          FTPAutomation_Down_ServerIP,            "192.168.119.243",          bRead);  //Sam 20220831 : 矽格北興俊堯要求修改預設值
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_UserID",            FTPAutomation_Down_UserID,              "autoload",                 bRead);
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_Password",          FTPAutomation_Down_Password,            "autoload",                 bRead);
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Down_ServerPath",        FTPAutomation_Down_ServerPath,          "/download/HT-9046HA-8H/",  bRead);
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_ServerIP",            FTPAutomation_Up_ServerIP,              "192.168.119.243",          bRead);
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_UserID",              FTPAutomation_Up_UserID,                "autoload",                 bRead);
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_Password",            FTPAutomation_Up_Password,              "autoload",                 bRead);
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_ServerPath",          FTPAutomation_Up_ServerPath,            "/upload/HT-9046HA-8H/",    bRead);
        ReadWriteIni(sPath, "FTPAutomation",    "FTPAutomation_Up_ErrorCodePath",       FTPAutomation_Up_ErrorCodePath,         "/upload/error_code/",      bRead);
    }
}
//---------------------------------------------------------------------------
AnsiString TfLotInfo::CheckingCheckList(bool bInitialStart)                     //KaiChen 20190530 ：Sigurd FTP Automation
{
    AnsiString asResult="NG";

    asResult=GenerateCheckList(true, fMain->cbSetupFileName->Text, false, bInitialStart);

    NewRecordProcess("", "Checking CheckList:"+asResult);

    return asResult;
}
//---------------------------------------------------------------------------
AnsiString TfLotInfo::GenerateCheckList(bool bCheckFlag, AnsiString asCheckListName, bool bBack, bool bInitialStart)    //KaiChen 20191003 ：修改回傳 AnsiString   //KaiChen 20190530 ：Sigurd FTP Automation
{
    AnsiString sPathName, sCheckListName;
    FILE * pFile;
    AnsiString asSiteMapping;
    AnsiString asString;
    AnsiString asCheck;
    int iCheck;
    double dCheck;
    double dValue;
    bool bCheck;
    AnsiString asNonDoubleBin;
    AnsiString asPassFail,asBin;
    int iBin,iCate;
    TStringList *tNonDoubleBin=new TStringList();
    int iStackCateCnt[eTrayCount]={0};                                          //JerryYang 20230918
    int iStackDefFailCate;

    ZeroMemory(iStackCateCnt, sizeof(iStackCateCnt));                           //Sam 20240425 : [A32] 功能支援 9011

    if(bBack==true)
        sPathName.sprintf("D:\\HT9045_Log\\CheckingList\\back");
    else
        sPathName.sprintf("D:\\HT9045_Log\\CheckingList");

    sCheckListName.sprintf("%s\\%s.txt",sPathName, asCheckListName);
    MyForceDirectories(sPathName);

    if(bCheckFlag==true)                                                        // 做檢查
    {
        if(FileExists(sCheckListName)==false)
        {
            return "CheckList File Error";
        }

        if(IniConfig.bA32EnableCheckList[eCL_Temperature])
        {
            asCheck=ReadIniData(sCheckListName, "Temperature",    "Temperature",    (AnsiString)fMain->edWorkTemperBase->Text);
            if(LastSet.iTemperature==Tempture_Ambient)
            {
                if(asCheck!=25)
                {
                    CheckListValue_Check.printf("Temperature：%s", asCheck);
                    CheckListValue_Current.printf("Temperature：%s", "25");
                    return "[Temperature] Temperature";
                }
            }
            else
            {
                if(asCheck!=fMain->edWorkTemperBase->Text)
                {
                    CheckListValue_Check.printf("Temperature：%s", asCheck);
                    CheckListValue_Current.printf("Temperature：%s", (AnsiString)fMain->edWorkTemperBase->Text);
                    return "[Temperature] Temperature";
                }
            }
            asCheck=ReadIniData(sCheckListName, "Temperature",    "Soak",           (AnsiString)fMain->edSoakTime->Text);
            if(asCheck!=fMain->edSoakTime->Text)
            {
                CheckListValue_Check.printf("Soak Time：%s", asCheck);
                CheckListValue_Current.printf("Soak Time：%s", (AnsiString)fMain->edSoakTime->Text);
                return "[Temperature] SoakTime";
            }

            if(CUSTOMER_CODE==CC_SIGURD_PeiXing ||                              //Sam 20220221 : 北興廠新增 InitialDelay 檢查
                CUSTOMER_CODE==CC_UTAC_TW)                                      //Sam 20250303 : 聯測新增檢查
            {
                bCheck=ReadIniData(sCheckListName, "InitialMode",    "bEveryFirstDeviceUseInitialDelay",      TestIF_File.bEveryFirstDeviceUseInitialDelay);
                if(bCheck!=TestIF_File.bEveryFirstDeviceUseInitialDelay)
                {
                    CheckListValue_Check.printf("InitialDelay：%d", (int)bCheck);
                    CheckListValue_Current.printf("InitialDelay：%d (EveryFirstDevice : Enable)", (int)TestIF_File.bEveryFirstDeviceUseInitialDelay);
                    return "[Temperature] InitialMode";
                }

                if(IniConfig.bI13InitStartDelayHasFTandRT && iTestRunMode!=FT)
                {
                    dCheck=ReadIniData(sCheckListName, "InitialMode", "iInitialDelay", TestIF_File.dInitialDelay_1_RT);
                    if(dCheck!=TestIF_File.dInitialDelay_1_RT)
                    {
                        CheckListValue_Check.printf("InitialDelay：%0.2f", dCheck);
                        CheckListValue_Current.printf("InitialDelay：%0.2f", TestIF_File.dInitialDelay_1_RT);
                        return "[Temperature] InitialMode";
                    }
                }
                else
                {
                    dCheck=ReadIniData(sCheckListName, "InitialMode", "iInitialDelay", TestIF_File.iInitialDelay);
                    if(dCheck!=TestIF_File.iInitialDelay)
                    {
                        CheckListValue_Check.printf("InitialDelay：%0.2f", dCheck);
                        CheckListValue_Current.printf("InitialDelay：%0.2f", TestIF_File.iInitialDelay);
                        return "[Temperature] InitialMode";
                    }
                }
            }

            if(CUSTOMER_CODE==CC_UTAC_TW)                                       //Sam 20250303 : 聯測新增檢查
            {
                bCheck=ReadIniData(sCheckListName, "InitialMode",    "bAfterOpenHeatDoorUseInitialDelay",      TestIF_File.bAfterOpenHeatDoorUseInitialDelay);
                if(bCheck!=TestIF_File.bAfterOpenHeatDoorUseInitialDelay)
                {
                    CheckListValue_Check.printf("InitialDelay：%d", (int)bCheck);
                    CheckListValue_Current.printf("InitialDelay：%d (AfterOpenHeatDoor : Enable)", (int)TestIF_File.bAfterOpenHeatDoorUseInitialDelay);
                    return "[Temperature] InitialMode";
                }

                if(IniConfig.bI13InitStartDelayHasFTandRT && iTestRunMode!=FT)
                {
                    dCheck=ReadIniData(sCheckListName, "InitialMode", "iInitialDelay_5", TestIF_File.dInitialDelay_5_RT);
                    if(dCheck!=TestIF_File.dInitialDelay_5_RT)
                    {
                        CheckListValue_Check.printf("InitialDelay_5：%0.2f", dCheck);
                        CheckListValue_Current.printf("InitialDelay_5：%0.2f", TestIF_File.dInitialDelay_5_RT);
                        return "[Temperature] InitialMode";
                    }
                }
                else
                {
                    dCheck=ReadIniData(sCheckListName, "InitialMode", "iInitialDelay_5", TestIF_File.iInitialDelay_5);
                    if(dCheck!=TestIF_File.iInitialDelay_5)
                    {
                        CheckListValue_Check.printf("InitialDelay_5：%0.2f", dCheck);
                        CheckListValue_Current.printf("InitialDelay_5：%0.2f", TestIF_File.iInitialDelay_5);
                        return "[Temperature] InitialMode";
                    }
                }
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_Alarm])
        {
            dCheck=ReadIniData(sCheckListName, "Alarm",    "Start Wait",      TestIF_File.dStartDelayTime);
            if(CUSTOMER_CODE==CC_SIGURD_PeiXing ||
               CUSTOMER_CODE==CC_SIGURD_ChungXing)                              //KaiChen 20191120 ：矽格-北興/中興，檢查特別卡關
            {
                if(dCheck<TestIF_File.dStartDelayTime)
                {
                    CheckListValue_Check.printf("Start Wait：%0.4f", dCheck);
                    CheckListValue_Current.printf("Start Wait：%0.4f", TestIF_File.dStartDelayTime);
                    return "[Alarm] Start Wait";
                }
            }
            else if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                             //KaiChen 20191120 ：矽格-湖口，檢查特別卡關 0.5 Sec
            {
                if(0.5<TestIF_File.dStartDelayTime)
                {
                    CheckListValue_Check.printf("Start Wait：%s", "0.5");
                    CheckListValue_Current.printf("Start Wait：%0.4f", TestIF_File.dStartDelayTime);
                    return "[Alarm] Start Wait";
                }
            }
            else
            {
                if(dCheck!=TestIF_File.dStartDelayTime)
                {
                    CheckListValue_Check.printf("Start Wait：%0.4f", dCheck);
                    CheckListValue_Current.printf("Start Wait：%0.4f", TestIF_File.dStartDelayTime);
                    return "[Alarm] Start Wait";
                }
            }

            iCheck=!ReadIniData(sCheckListName, "Alarm",    "Tester",          LastSet.iTester);                        //KaiChen 20190918 ：矽格-游宗穎要求, 連線時 Tester=0, 斷線時 Tester=1
            if(iCheck!=LastSet.iTester)
            {
                CheckListValue_Check.printf("Tester：%d", iCheck);
                CheckListValue_Current.printf("Tester：%d", (int)LastSet.iTester);
                return "[Alarm] Tester";
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_FT_Yield])
        {
            if(iTestRunMode==FT)
            {
                bCheck=ReadIniData(sCheckListName, "FT_Yield",    "Yield Func",     TestIF_File.bFailAlarmLowYieldByTotal);
                if(bCheck!=TestIF_File.bFailAlarmLowYieldByTotal)
                {
                    CheckListValue_Check.printf("Yield Func：%d", (int)bCheck);
                    CheckListValue_Current.printf("Yield Func：%d (Low Yield : Enable)", (int)TestIF_File.bFailAlarmLowYieldByTotal);
                    return "[FT_Yield] Yield Func";
                }

                bCheck=ReadIniData(sCheckListName, "FT_Yield",    "ContsFailBySocket Func",     TestIF_File.bContsFailBySocket);
                if(bCheck!=TestIF_File.bContsFailBySocket)
                {
                    CheckListValue_Check.printf("ContsFailBySocket Func：%d", (int)bCheck);
                    CheckListValue_Current.printf("ContsFailBySocket Func：%d (ContsFailBySocket : Enable)", (int)TestIF_File.bContsFailBySocket);
                    return "[FT_Yield] ContsFailBySocket Func";
                }

                bCheck=ReadIniData(sCheckListName, "FT_Yield",    "ContsFailByHead Func",     TestIF_File.bContsFailByHead);
                if(bCheck!=TestIF_File.bContsFailByHead)
                {
                    CheckListValue_Check.printf("ContsFailByHead Func：%d", (int)bCheck);
                    CheckListValue_Current.printf("ContsFailByHead Func：%d (ContsFailByHead : Enable)", (int)TestIF_File.bContsFailByHead);
                    return "[FT_Yield] ContsFailByHead Func";
                }
            }

            if(iTestRunMode==FT && bInitialStart==false)                        //Sam 20210408 : 只有 FT & Initail Start 檢查
            {
                iCheck=ReadIniData(sCheckListName, "FT_Yield",    "Preset",          TestIF_File.iLowYieldCountByTotal);
                if(iCheck!=TestIF_File.iLowYieldCountByTotal)
                {
                    CheckListValue_Check.printf("Preset：%d", iCheck);
                    CheckListValue_Current.printf("Preset：%d (Low Yield : after contact count)", TestIF_File.iLowYieldCountByTotal);
                    return "[FT_Yield] Preset";
                }

                dCheck=ReadIniData(sCheckListName, "FT_Yield", "Low Yield",       TestIF_File.dLowYieldLimitByTotal);
                if(dCheck!=TestIF_File.dLowYieldLimitByTotal)
                {
                    CheckListValue_Check.printf("Low Yield：%0.4f", dCheck);
                    CheckListValue_Current.printf("Low Yield：%0.4f (Low Yield : Yield Limit%)", TestIF_File.dLowYieldLimitByTotal);
                    return "[FT_Yield] Low Yield";
                }
                dCheck=ReadIniData(sCheckListName, "FT_Yield", "Variance",        TestIF_File.dFailAlarmSiteYieldCmp);
                if(dCheck!=TestIF_File.dFailAlarmSiteYieldCmp)
                {
                    CheckListValue_Check.printf("Variance：%0.4f", dCheck);
                    CheckListValue_Current.printf("Variance：%0.4f (Yield Alarm : By Site Compare Yield%)", TestIF_File.dFailAlarmSiteYieldCmp);
                    return "[FT_Yield] Variance";
                }

                iCheck=ReadIniData(sCheckListName, "Alarm",    "Socket",          (int)TestIF_File.iContsFailSocketAlarmCT);
                if(iCheck!=(int)TestIF_File.iContsFailSocketAlarmCT)
                {
                    CheckListValue_Check.printf("Socket：%d", iCheck);
                    CheckListValue_Current.printf("Socket：%d", (int)TestIF_File.iContsFailSocketAlarmCT);
                    return "[Alarm] Socket";
                }

                iCheck=ReadIniData(sCheckListName, "Alarm",    "Head",            (int)TestIF_File.iContsFailHeadAlarmCT);
                if(iCheck!=(int)TestIF_File.iContsFailHeadAlarmCT)
                {
                    CheckListValue_Check.printf("Head：%d", iCheck);
                    CheckListValue_Current.printf("Head：%d", (int)TestIF_File.iContsFailHeadAlarmCT);
                    return "[Alarm] Head";
                }
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_SiteMapping])
        {
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    asSiteMapping.sprintf("Site %s", IndexSuckName[i][j]);

                    iCheck=ReadIniData(sCheckListName, "Site Mapping",    asSiteMapping,      TestIF_File.iSiteMap[i][j]);
                    if(iCheck!=TestIF_File.iSiteMap[i][j])
                    {
                        CheckListValue_Check.printf("Site：%d", iCheck);
                        CheckListValue_Current.printf("Site：%d", TestIF_File.iSiteMap[i][j]);
                        return "[Site Mapping]";
                    }
                }
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_Speed])
        {
            if(CUSTOMER_CODE==CC_SIGURD_PeiXing ||
               CUSTOMER_CODE==CC_SIGURD_ChungXing)                              //KaiChen 20191120 ：矽格-北興/中興，檢查特別卡關
            {
                iCheck=ReadIniData(sCheckListName,    "Speed",        "Index Arm",              ArmSpeed[IndexArm].iBodySP);
                if(iCheck<ArmSpeed[IndexArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[IndexArm].iBodySP);
                    return "[Speed] Index Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Input Arm",              ArmSpeed[InArm].iBodySP);
                if(iCheck<ArmSpeed[InArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[InArm].iBodySP);
                    return "[Speed] Input Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Output Arm",             ArmSpeed[OutArm].iBodySP);
                if(iCheck<ArmSpeed[OutArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[OutArm].iBodySP);
                    return "[Speed] Output Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Tray Arm",               ArmSpeed[TrayArm].iBodySP);
                if(iCheck<ArmSpeed[TrayArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[TrayArm].iBodySP);
                    return "[Speed] Tray Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Shuttle1",               SHSpeed.iSH1Sp);
                if(iCheck<SHSpeed.iSH1Sp)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", SHSpeed.iSH1Sp);
                    return "[Speed] Shuttle1";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Shuttle2",               SHSpeed.iSH2Sp);
                if(iCheck<SHSpeed.iSH2Sp)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", SHSpeed.iSH2Sp);
                    return "[Speed] Shuttle2";
                }
            }
            else
            {
                iCheck=ReadIniData(sCheckListName,    "Speed",        "Index Arm",              ArmSpeed[IndexArm].iBodySP);
                if(iCheck!=ArmSpeed[IndexArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[IndexArm].iBodySP);
                    return "[Speed] Index Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Input Arm",              ArmSpeed[InArm].iBodySP);
                if(iCheck!=ArmSpeed[InArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[InArm].iBodySP);
                    return "[Speed] Input Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Output Arm",             ArmSpeed[OutArm].iBodySP);
                if(iCheck!=ArmSpeed[OutArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[OutArm].iBodySP);
                    return "[Speed] Output Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Tray Arm",               ArmSpeed[TrayArm].iBodySP);
                if(iCheck!=ArmSpeed[TrayArm].iBodySP)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", ArmSpeed[TrayArm].iBodySP);
                    return "[Speed] Tray Arm";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Shuttle1",               SHSpeed.iSH1Sp);
                if(iCheck!=SHSpeed.iSH1Sp)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", SHSpeed.iSH1Sp);
                    return "[Speed] Shuttle1";
                }

                iCheck=ReadIniData(sCheckListName,    "Speed",        "Shuttle2",               SHSpeed.iSH2Sp);
                if(iCheck!=SHSpeed.iSH2Sp)
                {
                    CheckListValue_Check.printf("Index Arm：%d", iCheck);
                    CheckListValue_Current.printf("Index Arm：%d", SHSpeed.iSH2Sp);
                    return "[Speed] Shuttle2";
                }
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_Contact])
        {
            iCheck=ReadIniData(sCheckListName,    "Contact",        "Pin Count",              DeviceForm_File.iPinCT);
            if(iCheck!=DeviceForm_File.iPinCT)
            {
                CheckListValue_Check.printf("Pin Count：%d", iCheck);
                CheckListValue_Current.printf("Pin Count：%d",  DeviceForm_File.iPinCT);
                return "[Contact] Pin Count";
            }
            dCheck=ReadIniData(sCheckListName,    "Contact",        "Pin Force N",            DeviceForm_File.ForcePerPinN);
            if(dCheck!=DeviceForm_File.ForcePerPinN)
            {
                CheckListValue_Check.printf("Variance：%0.4f", dCheck);
                CheckListValue_Current.printf("Variance：%0.4f", DeviceForm_File.ForcePerPinN);
                return "[Contact] Pin Force N";
            }

            if(INSTALL_DOUBLE_EP==1)                                            //Sam 20250107 : A32 功能新增 Double Force
            {
                dCheck=ReadIniData(sCheckListName,    "Contact",        "Double Force",       DeviceForm_File.DoubleForce);
                if(dCheck!=DeviceForm_File.DoubleForce)
                {
                    CheckListValue_Check.printf("Variance：%0.4f", dCheck);
                    CheckListValue_Current.printf("Variance：%0.4f", DeviceForm_File.DoubleForce);
                    return "[Contact] Double Force";
                }
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_Category] )
        {
            if(iTestRunMode==FT && bInitialStart==false)                        //Sam 20210408 : 只有 FT & Initail Start 檢查
            {
                for(int i=0; i<iTestBinCount; i++)                              //Sam 20240425 : [A32] 功能支援255Bin
                {
                    asString.sprintf("Category%d", i);

                    bCheck=ReadIniData(sCheckListName,    asString,       "Fail C Bin",               BinSelect[FT].bFailCountEnable[i]);
                    if(bCheck!=BinSelect[FT].bFailCountEnable[i])
                    {
                        CheckListValue_Check.printf("Category%d, Fail C Bin：%d", i, (int)bCheck);
                        CheckListValue_Current.printf("Category%d, Fail C Bin：%d", i, (int)BinSelect[FT].bFailCountEnable[i]);
                        return asString+"Fail C Bin";
                    }

                    iCheck=ReadIniData(sCheckListName,    asString,       "Fail C Ignore",            BinSelect[FT].iFailCountIgnore[i]);
                    if(iCheck!=BinSelect[FT].iFailCountIgnore[i])
                    {
                        CheckListValue_Check    .printf("Category%d, Fail C Ignore：%d", i, iCheck);
                        CheckListValue_Current  .printf("Category%d, Fail C Ignore：%d", i, BinSelect[FT].iFailCountIgnore[i]);
                        return asString+"Fail C Ignore";
                    }

                    iCheck=ReadIniData(sCheckListName,    asString,       "Fail C Num",               BinSelect[FT].iFailCountLimit[i]);
                    if(iCheck!=BinSelect[FT].iFailCountLimit[i])
                    {
                        CheckListValue_Check    .printf("Category%d, Fail C Num：%d", i, iCheck);
                        CheckListValue_Current  .printf("Category%d, Fail C Num：%d", i, BinSelect[FT].iFailCountLimit[i]);
                        return asString+"Fail C Num";
                    }

                    bCheck=ReadIniData(sCheckListName,    asString,       "Fail Percent",             BinSelect[FT].bFailure[i]);
                    if(bCheck!=BinSelect[FT].bFailure[i])
                    {
                        CheckListValue_Check    .printf("Category%d, Fail Percent：%d", i, (int)bCheck);
                        CheckListValue_Current  .printf("Category%d, Fail Percent：%d", i, (int)BinSelect[FT].bFailure[i]);
                        return asString+"Fail Percent";
                    }

                    iCheck=ReadIniData(sCheckListName,    asString,       "Fail Percent Ignore",      BinSelect[FT].iPersentIgnore[i]);
                    if(iCheck!=BinSelect[FT].iPersentIgnore[i])
                    {
                        CheckListValue_Check    .printf("Category%d, Fail Percent Ignore：%d", i, iCheck);
                        CheckListValue_Current  .printf("Category%d, Fail Percent Ignore：%d", i, BinSelect[FT].iPersentIgnore[i]);
                        return asString+"Fail Percent Ignore";
                    }

                    dCheck=ReadIniData(sCheckListName,    asString,       "Fail Percent Num",         BinSelect[FT].dFailureLimit[i]);
                    if(dCheck!=BinSelect[FT].dFailureLimit[i])
                    {
                        CheckListValue_Check    .printf("Category%d, Fail Percent Ignore：%0.4f", i, dCheck);
                        CheckListValue_Current  .printf("Category%d, Fail Percent Ignore：%0.4f", i, BinSelect[FT].dFailureLimit[i]);
                        return asString+"Fail Percent Num";
                    }
                }
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_BinSetting])                       //KaiChen 20200527 ：矽興要求增加
        {
            if(CUSTOMER_CODE==CC_SIGURD_PeiXing ||                              //Sam 20210329 : 矽格北興要求只檢查 Pass/Fail Bin 數量
               (CUSTOMER_CODE==CC_SIGURD_ChungXing &&
                IniConfig.bA32_3For93KFunction==true))                          //Sam 20220620 : 中興廠新增 A32-3 功能 for 93K function
            {
                asCheck=ReadIniData(sCheckListName,"Tester_Control", "NONDOUBLEBIN", AnsiString(""));
                if(asCheck=="")
                {
                    //沒有資料，與客戶確認是否要報警
                }
                else if(asCheck=="NULL")
                {
                    //不檢查
                }
                else
                {
                    for(int i=0; i<iTestBinCount; i++)                          //Sam 20240425 : [A32] 功能支援255Bin
                    {
                        asBin.sprintf("IsPassBin%d", i);
                        asPassFail=ReadIniData(sCheckListName,"Tester_Control",asBin,AnsiString("NULL"));
                        if(asPassFail!="NULL")
                        {
                            iCate=BinSelect[iTestRunMode].iCatDataT3Pos[i];
                            iStackCateCnt[iCate]++;
                        }
                    }

                    asNonDoubleBin=StringReplace(asCheck,"_", ",", TReplaceFlags()<<rfReplaceAll);
                    tNonDoubleBin->CommaText=asNonDoubleBin;
                    for(int i=0; i<tNonDoubleBin->Count; i++)
                    {
                        iBin=atoi(tNonDoubleBin->Strings[i].c_str());
                        iCate=BinSelect[iTestRunMode].iCatDataT3Pos[iBin];
                        if(iStackCateCnt[iCate]>=2)
                        {
                            CheckListValue_Check    .printf("NONDOUBLEBIN=%s", asCheck);
                            CheckListValue_Current  .printf("Bin%d is double bin", iBin);
                            return "[Tester_Control] NONDOUBLEBIN";
                        }
                    }
                }

                for(int i=0; i<iTestBinCount; i++)                              //Sam 20240425 : [A32] 功能支援255Bin
                {
                    asBin.sprintf("IsPassBin%d", i);
                    asPassFail=ReadIniData(sCheckListName,"Tester_Control", asBin,AnsiString(""));
                    iCate=BinSelect[iTestRunMode].iCatDataT3Pos[i];

                    if(asPassFail=="")                                          //測試機 && Handler 都沒有設定 Bin
                    {
                    }
                    else if(asPassFail=="NULL" && iCate==0)                     //測試機 && Handler 都沒有設定 Bin
                    {
                    }
                    else if(asPassFail=="NULL" && iCate!=0)                     //測試機沒有設定 Bin , Handler 有設定 Bin
                    {
                        CheckListValue_Check    .printf("Bin%d is not use", i);
                        CheckListValue_Current  .printf("Bin%d is use", i);
                        return "[Tester_Control] BINCOUNT";
                    }
                    else if((asPassFail=="P" || asPassFail=="F") && iCate==0)   //測試機有設定 Bin , Handler 沒有設定 Bin
                    {
                        CheckListValue_Check    .printf("Bin%d is use", i);
                        CheckListValue_Current  .printf("Bin%d is not use", i);
                        return "[Tester_Control] BINCOUNT";
                    }
                    else if(iCate==0)                                           //測試機 && Handler 都沒有設定 Bin
                    {
                    }
                    else
                    {
                        iStackDefFailCate=BinSelect[iTestRunMode].iStackDefFailCate[iCate-1];
                        if(asPassFail=="P" && iStackDefFailCate==1)             //測試機設定 Bin Pass, Handler 設定 Bin Fail
                        {
                            CheckListValue_Check    .printf("Bin%d is Pass", i);
                            CheckListValue_Current  .printf("Bin%d is Fail", i);
                            return "[Tester_Control] BINCOUNT";
                        }
                        else if(asPassFail=="F" && iStackDefFailCate==0)        //測試機設定 Bin Fail, Handler 設定 Bin Pass
                        {
                            CheckListValue_Check    .printf("Bin%d is Fail", i);
                            CheckListValue_Current  .printf("Bin%d is Pass", i);
                            return "[Tester_Control] BINCOUNT";
                        }
                    }
                }
            }
            else
            {
                for(int i=0; i<iTestBinCount; i++)                              //Sam 20240425 : [A32] 功能支援255Bin
                {
                    asString.sprintf("Bin%d", i);
                    iCheck=ReadIniData(sCheckListName,    "BinSetting",        asString,              BinSelect[iTestRunMode].iCatDataT3Pos[i]);
                    if(iCheck!=BinSelect[iTestRunMode].iCatDataT3Pos[i])
                    {
                        CheckListValue_Check    .printf("Bin%d=%d", i, iCheck);
                        CheckListValue_Current  .printf("Bin%d=%d", i, BinSelect[iTestRunMode].iCatDataT3Pos[i]);
                        return "[BinSetting]"+asString;
                    }
                }
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_TrayForm])                         //KaiChen 20200527 ：矽興要求增加
        {
            dCheck=ReadIniData(sCheckListName, "TrayForm", "X_Start",        UserDefForm_File[0].XStart);
            if(dCheck!=UserDefForm_File[0].XStart)
            {
                CheckListValue_Check    .printf("X_Start：%0.4f", dCheck);
                CheckListValue_Current  .printf("X_Start：%0.4f", UserDefForm_File[0].XStart);
                return "[TrayForm] X_Start";
            }

            dCheck=ReadIniData(sCheckListName, "TrayForm", "Y_Start",        UserDefForm_File[0].YStart);
            if(dCheck!=UserDefForm_File[0].YStart)
            {
                CheckListValue_Check    .printf("Y_Start：%0.4f", dCheck);
                CheckListValue_Current  .printf("Y_Start：%0.4f", UserDefForm_File[0].YStart);
                return "[TrayForm] Y_Start";
            }

            dCheck=ReadIniData(sCheckListName, "TrayForm", "X_Pitch",        UserDefForm_File[0].XPitch);
            if(dCheck!=UserDefForm_File[0].XPitch)
            {
                CheckListValue_Check    .printf("X_Pitch：%0.4f", dCheck);
                CheckListValue_Current  .printf("X_Pitch：%0.4f", UserDefForm_File[0].XPitch);
                return "[TrayForm] X_Pitch";
            }

            dCheck=ReadIniData(sCheckListName, "TrayForm", "Y_Pitch",        UserDefForm_File[0].YPitch);
            if(dCheck!=UserDefForm_File[0].YPitch)
            {
                CheckListValue_Check    .printf("Y_Pitch：%0.4f", dCheck);
                CheckListValue_Current  .printf("Y_Pitch：%0.4f", UserDefForm_File[0].YPitch);
                return "[TrayForm] Y_Pitch";
            }

            dCheck=ReadIniData(sCheckListName, "TrayForm", "Thickness",        UserDefForm_File[0].ZDepth);
            if(dCheck!=UserDefForm_File[0].ZDepth)
            {
                CheckListValue_Check    .printf("Thickness：%0.4f", dCheck);
                CheckListValue_Current  .printf("Thickness：%0.4f", UserDefForm_File[0].ZDepth);
                return "[TrayForm] Thickness";
            }

            iCheck=ReadIniData(sCheckListName,  "TrayForm",        "X_Division",               UserDefForm_File[0].XDivision);
            if(iCheck!=UserDefForm_File[0].XDivision)
            {
                CheckListValue_Check    .printf("X_Division：%d", iCheck);
                CheckListValue_Current  .printf("X_Division：%d", UserDefForm_File[0].XDivision);
                return "[TrayForm] X_Division";
            }

            iCheck=ReadIniData(sCheckListName,  "TrayForm",        "Y_Division",               UserDefForm_File[0].YDivision);
            if(iCheck!=UserDefForm_File[0].YDivision)
            {
                CheckListValue_Check    .printf("Y_Division：%d", iCheck);
                CheckListValue_Current  .printf("Y_Division：%d", UserDefForm_File[0].YDivision);
                return "[TrayForm] Y_Division";
            }

            dCheck=ReadIniData(sCheckListName, "TrayForm", "PickUp",        UserDefForm_File[0].iPickUp);
            if(dCheck!=UserDefForm_File[0].iPickUp)
            {
                CheckListValue_Check    .printf("PickUp：%0.4f", dCheck);
                CheckListValue_Current  .printf("PickUp：%0.4f", UserDefForm_File[0].iPickUp);
                return "[TrayForm] PickUp";
            }
        }

        if(IniConfig.bA32EnableCheckList[eCL_HotPlate])                         //KaiChen 20200527 ：矽興要求增加
        {
            dCheck=ReadIniData(sCheckListName, "HotPlate", "X_Start",        HotPlateForm_File.XStart);
            if(dCheck!=HotPlateForm_File.XStart)
            {
                CheckListValue_Check    .printf("X_Start：%0.4f", dCheck);
                CheckListValue_Current  .printf("X_Start：%0.4f", HotPlateForm_File.XStart);
                return "[HotPlate] X_Start";
            }

            dCheck=ReadIniData(sCheckListName, "HotPlate", "Y_Start",        HotPlateForm_File.YStart);
            if(dCheck!=HotPlateForm_File.YStart)
            {
                CheckListValue_Check    .printf("Y_Start：%0.4f", dCheck);
                CheckListValue_Current  .printf("Y_Start：%0.4f", HotPlateForm_File.YStart);
                return "[HotPlate] Y_Start";
            }

            dCheck=ReadIniData(sCheckListName, "HotPlate", "X_Pitch",        HotPlateForm_File.XPitch);
            if(dCheck!=HotPlateForm_File.XPitch)
            {
                CheckListValue_Check    .printf("X_Pitch：%0.4f", dCheck);
                CheckListValue_Current  .printf("X_Pitch：%0.4f", HotPlateForm_File.XPitch);
                return "[HotPlate] X_Pitch";
            }

            dCheck=ReadIniData(sCheckListName, "HotPlate", "Y_Pitch",        HotPlateForm_File.YPitch);
            if(dCheck!=HotPlateForm_File.YPitch)
            {
                CheckListValue_Check    .printf("Y_Pitch：%0.4f", dCheck);
                CheckListValue_Current  .printf("Y_Pitch：%0.4f", HotPlateForm_File.YPitch);
                return "[HotPlate] Y_Pitch";
            }

            iCheck=ReadIniData(sCheckListName,  "HotPlate",        "X_Division",               HotPlateForm_File.XDivision);
            if(iCheck!=HotPlateForm_File.XDivision)
            {
                CheckListValue_Check    .printf("X_Division：%d", iCheck);
                CheckListValue_Current  .printf("X_Division：%d", HotPlateForm_File.XDivision);
                return "[HotPlate] X_Division";
            }

            iCheck=ReadIniData(sCheckListName,  "HotPlate",        "Y_Division",               HotPlateForm_File.YDivision);
            if(iCheck!=HotPlateForm_File.YDivision)
            {
                CheckListValue_Check    .printf("Y_Division：%d", iCheck);
                CheckListValue_Current  .printf("Y_Division：%d", HotPlateForm_File.YDivision);
                return "[HotPlate] Y_Division";
            }

            //Sam 20210428 : 中興廠杜鴻其要求新增
            //==>
            iCheck=ReadIniData(sCheckListName,  "Config",        "bD44CheckIndexICDestroy",    IniConfig.bD44CheckIndexICDestroy);
            if(iCheck!=IniConfig.bD44CheckIndexICDestroy)
            {
                CheckListValue_Check    .printf("CheckIndexICDestroy, Config Function D44：%d", iCheck);
                CheckListValue_Current  .printf("CheckIndexICDestroy, Config Function D44：%d", IniConfig.bD44CheckIndexICDestroy);
                return "[Config] bD44CheckIndexICDestroy";
            }

            if(IniConfig.bD44CheckIndexICDestroy)                               //Sam 20220517 : D44 有開啟時才檢查數值。
            {
                dCheck=ReadIniData(sCheckListName,  "Config",        "iD44TestHeadCheckVacuumTime",    IniConfig.iD44TestHeadCheckVacuumTime/1000.0);
                dValue=IniConfig.iD44TestHeadCheckVacuumTime/1000.0;

                if(abs(dCheck-dValue)>=0.02)                                    //Sam 20220517 : 修正資料轉換之間可能有小數點的產生導致數值無法符合的誤報警
                {
                    CheckListValue_Check    .printf("TestHeadCheckVacuumTime, Config Function D44：%1.2f", dCheck);
                    CheckListValue_Current  .printf("TestHeadCheckVacuumTime, Config Function D44：%1.2f", IniConfig.iD44TestHeadCheckVacuumTime/1000.0);
                    return "[Config] bD44CheckIndexICDestroy";
                }
            }
            //<==
            //Sam 20210428 : 中興廠杜鴻其要求新增
        }
    }
    else
    {
        if(FileExists(sCheckListName)==false)
        {
            pFile=fopen(sCheckListName.c_str(), "a");
            if(pFile!=NULL)
            {
                fclose(pFile);
            }
        }

        // Temperature
        if(LastSet.iTemperature==Tempture_Ambient)
        {
            WriteIniData(sCheckListName, "Temperature",    "Temperature",       (AnsiString)25);
        }
        else
        {
            WriteIniData(sCheckListName, "Temperature",    "Temperature",       (AnsiString)fMain->edWorkTemperBase->Text);
        }

        WriteIniData(sCheckListName, "Temperature",    "Soak",              (AnsiString)fMain->edSoakTime->Text);

        if(CUSTOMER_CODE==CC_SIGURD_PeiXing ||                                  //Sam 20220221 : 北興廠新增 InitialDelay 檢查
           CUSTOMER_CODE==CC_UTAC_TW)                                           //Sam 20250303 : 聯測新增檢查
        {
            WriteIniData(sCheckListName, "InitialMode",       "bEveryFirstDeviceUseInitialDelay",   TestIF_File.bEveryFirstDeviceUseInitialDelay);
            if(IniConfig.bI13InitStartDelayHasFTandRT && iTestRunMode!=FT)
                WriteIniData(sCheckListName, "InitialMode", "iInitialDelay", TestIF_File.dInitialDelay_1_RT);
            else
                WriteIniData(sCheckListName, "InitialMode", "iInitialDelay", TestIF_File.iInitialDelay);
        }

        if(CUSTOMER_CODE==CC_UTAC_TW)                                           //Sam 20250303 : 聯測新增檢查
        {
            WriteIniData(sCheckListName, "InitialMode",       "bAfterOpenHeatDoorUseInitialDelay",   TestIF_File.bAfterOpenHeatDoorUseInitialDelay);
            if(IniConfig.bI13InitStartDelayHasFTandRT && iTestRunMode!=FT)
                WriteIniData(sCheckListName, "InitialMode", "iInitialDelay_5", TestIF_File.dInitialDelay_5_RT);
            else
                WriteIniData(sCheckListName, "InitialMode", "iInitialDelay_5", TestIF_File.iInitialDelay_5);
        }

        // Alarm
        WriteIniData(sCheckListName, "Alarm",    "Start Wait",      TestIF_File.dStartDelayTime);
        if(iTestRunMode==FT)
        {
            WriteIniData(sCheckListName, "Alarm",    "Socket",          (int)TestIF_File.iContsFailSocketAlarmCT);
            WriteIniData(sCheckListName, "Alarm",    "Head",            (int)TestIF_File.iContsFailHeadAlarmCT);
        }
        else
        {
            WriteIniData(sCheckListName, "Alarm",    "Socket",          (int)TestIF_File.iContsFailSocketAlarmCT_RT);
            WriteIniData(sCheckListName, "Alarm",    "Head",            (int)TestIF_File.iContsFailHeadAlarmCT_RT);
        }
        WriteIniData(sCheckListName, "Alarm",    "Tester",          !LastSet.iTester);                                  //KaiChen 20190918 ：矽格-游宗穎要求, 連線時 Tester=0, 斷線時 Tester=1

        // FT_Yield
        if(iTestRunMode==FT)
        {
            WriteIniData(sCheckListName, "FT_Yield", "Yield Func",               TestIF_File.bFailAlarmLowYieldByTotal);
            WriteIniData(sCheckListName, "FT_Yield", "ContsFailBySocket Func",   TestIF_File.bContsFailBySocket);
            WriteIniData(sCheckListName, "FT_Yield", "ContsFailByHead Func",     TestIF_File.bContsFailByHead);

            WriteIniData(sCheckListName, "FT_Yield", "Preset",          TestIF_File.iLowYieldCountByTotal);
            WriteIniData(sCheckListName, "FT_Yield", "Low Yield",       TestIF_File.dLowYieldLimitByTotal);
            WriteIniData(sCheckListName, "FT_Yield", "Variance",        TestIF_File.dFailAlarmSiteYieldCmp);
        }
        else
        {
            WriteIniData(sCheckListName, "FT_Yield", "Yield Func",      TestIF_File.bFailAlarmLowYieldByTotal_RT);
            WriteIniData(sCheckListName, "FT_Yield", "ContsFailBySocket Func",   TestIF_File.bContsFailBySocket_RT);
            WriteIniData(sCheckListName, "FT_Yield", "ContsFailByHead Func",     TestIF_File.bContsFailByHead_RT);

            WriteIniData(sCheckListName, "FT_Yield", "Preset",          TestIF_File.iLowYieldCountByTotal_RT);
            WriteIniData(sCheckListName, "FT_Yield", "Low Yield",       TestIF_File.dLowYieldLimitByTotal_RT);
            WriteIniData(sCheckListName, "FT_Yield", "Variance",        TestIF_File.dFailAlarmSiteYieldCmp_RT);
        }

        //Site Mapping
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                asSiteMapping.sprintf("Site %s", IndexSuckName[i][j]);

                WriteIniData(sCheckListName, "Site Mapping",    asSiteMapping,      TestIF_File.iSiteMap[i][j]);
            }
        }

        // Speed
        WriteIniData(sCheckListName,    "Speed",        "Index Arm",            ArmSpeed[IndexArm].iBodySP);
        WriteIniData(sCheckListName,    "Speed",        "Input Arm",            ArmSpeed[InArm].iBodySP);
        WriteIniData(sCheckListName,    "Speed",        "Output Arm",           ArmSpeed[OutArm].iBodySP);
        WriteIniData(sCheckListName,    "Speed",        "Tray Arm",             ArmSpeed[TrayArm].iBodySP);
        WriteIniData(sCheckListName,    "Speed",        "Shuttle1",             SHSpeed.iSH1Sp);
        WriteIniData(sCheckListName,    "Speed",        "Shuttle2",             SHSpeed.iSH2Sp);

        // Contact
        WriteIniData(sCheckListName,    "Contact",        "Pin Count",          DeviceForm_File.iPinCT);
        WriteIniData(sCheckListName,    "Contact",        "Pin Force N",        DeviceForm_File.ForcePerPinN);
        if(INSTALL_DOUBLE_EP==1)                                                //Sam 20250107 : A32 功能新增 Double Force
        {
            WriteIniData(sCheckListName,    "Contact",        "Double Force",           DeviceForm_File.DoubleForce);
        }
        // Category1 ~ Category15
        for(int i=0; i<iTestBinCount; i++)                                      //Sam 20240425 : [A32] 支援255Bin
        {
            asString.sprintf("Category%d", i);
            WriteIniData(sCheckListName, asString, "Fail C Bin",                BinSelect[iTestRunMode].bFailCountEnable[i]);
            WriteIniData(sCheckListName, asString, "Fail C Ignore",             BinSelect[iTestRunMode].iFailCountIgnore[i]);
            WriteIniData(sCheckListName, asString, "Fail C Num",                BinSelect[iTestRunMode].iFailCountLimit[i]);
            WriteIniData(sCheckListName, asString, "Fail Percent",              BinSelect[iTestRunMode].bFailure[i]);
            WriteIniData(sCheckListName, asString, "Fail Percent Ignore",       BinSelect[iTestRunMode].iPersentIgnore[i]);
            WriteIniData(sCheckListName, asString, "Fail Percent Num",          BinSelect[iTestRunMode].dFailureLimit[i]);
        }

        //Bin
        for(int i=0; i<iTestBinCount; i++)                                      //Sam 20240425 : [A32] 功能支援255Bin
        {
            asString.sprintf("Bin%d", i);
            WriteIniData(sCheckListName, "BinSetting", asString,                BinSelect[iTestRunMode].iCatDataT3Pos[i]);
        }

        //  TrayForm
        WriteIniData(sCheckListName, "TrayForm", "X_Start",                     UserDefForm_File[0].XStart);
        WriteIniData(sCheckListName, "TrayForm", "Y_Start",                     UserDefForm_File[0].YStart);
        WriteIniData(sCheckListName, "TrayForm", "X_Pitch",                     UserDefForm_File[0].XPitch);
        WriteIniData(sCheckListName, "TrayForm", "Y_Pitch",                     UserDefForm_File[0].YPitch);
        WriteIniData(sCheckListName, "TrayForm", "Thickness",                   UserDefForm_File[0].ZDepth);
        WriteIniData(sCheckListName, "TrayForm", "X_Division",                  UserDefForm_File[0].XDivision);
        WriteIniData(sCheckListName, "TrayForm", "Y_Division",                  UserDefForm_File[0].YDivision);
        WriteIniData(sCheckListName, "TrayForm", "PickUp",                      UserDefForm_File[0].iPickUp);

        // HotPlate
        WriteIniData(sCheckListName, "HotPlate", "X_Start",                     HotPlateForm_File.XStart);
        WriteIniData(sCheckListName, "HotPlate", "Y_Start",                     HotPlateForm_File.YStart);
        WriteIniData(sCheckListName, "HotPlate", "X_Pitch",                     HotPlateForm_File.XPitch);
        WriteIniData(sCheckListName, "HotPlate", "Y_Pitch",                     HotPlateForm_File.YPitch);
        WriteIniData(sCheckListName, "HotPlate", "X_Division",                  HotPlateForm_File.XDivision);
        WriteIniData(sCheckListName, "HotPlate", "Y_Division",                  HotPlateForm_File.YDivision);

        //Sam 20210428 : 中興廠杜鴻其要求新增
        //==>
        WriteIniData(sCheckListName, "Config", "bD44CheckIndexICDestroy",       IniConfig.bD44CheckIndexICDestroy);
        WriteIniData(sCheckListName, "Config", "iD44TestHeadCheckVacuumTime",   IniConfig.iD44TestHeadCheckVacuumTime/1000.0);
        //<==
        //Sam 20210428 : 中興廠杜鴻其要求新增
    }
    tNonDoubleBin->Clear();
    delete tNonDoubleBin;
    return "OK";
}
//---------------------------------------------------------------------------
void TfLotInfo::AlarmCodeUpload(AnsiString JamCode)                             //KaiChen 20190530 ：Sigurd FTP Automation
{
    AnsiString sPathName;
    AnsiString asSource, asTarget;
    sPathName.sprintf("D:\\HT9045_Log\\ErrorBackup");
    MyForceDirectories(sPathName);
    asSource.sprintf("D:\\HT9045\\Error\\English\\%s.dat", JamCode);
    asTarget.sprintf("\\%s_%s_%04d%02d%02d%02d%02d%02d.dat", JamCode, IniConfig.asA32_1_HandlerID, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    TStringList *TestList=new TStringList;
    TestList->Add(asSource);
    TestList->SaveToFile(sPathName+asTarget);

    TestList->Clear();
    delete TestList;

    bSigurdUpload_Jamcode=true;
    fFTPClient->bError=false;
    fFTPClient->UploadFileToServer2(FTPAutomation_Up_ErrorCodePath, asTarget);
    bSigurdUpload_Jamcode=false;

    if(fFTPClient->bError==true)
    {
        NewRecordProcess("", "Upload file failed");
    }
    else
    {
        NewRecordProcess("", "Upload file successfully");
    }
}
//---------------------------------------------------------------------------
void  TfLotInfo::RefreshYieldMonitor()                                          //Sam 20210331 : 北興新增 Yield Monitor
{
    static bool bTimerRunning=false;                                            //Sam 20230220 : 增加保護
    if(IniConfig.bSIGURDFunction==false &&
       CosFunction.bShowYieldMonitor==false)                                    //Sam 20210916 : 新增 Yiled Monitor 到畫面上
        return;
    if(bTimerRunning)
        return;
     bTimerRunning=true;
    if(IniConfig.bSIGURDFunction)
        RefreshYieldMonitor_SIGURD();
    else
        RefreshYieldMonitor_TERAPOWER();
     AdjtsYieldMonitiorSize();
     bTimerRunning=false;
}
//---------------------------------------------------------------------------
void  TfLotInfo::RefreshYieldMonitor_SIGURD()
{
    AnsiString sPathName,sCheckListName,asLastOpenFN,asString;
    int iCheck,iBin;
    bool bCheck;
    double dCheck;
    bool bFailAlarmLowYieldByTotal,bContsFailBySocket,bContsFailByHead;
    int iLowYieldCountByTotal, iContsFailSocketAlarmCT, iContsFailHeadAlarmCT, iFailAlarmSiteYieldCmpCount;
    double dFailAlarmSiteYieldCmp,dLowYieldLimitByTotal;

    gbManualCheckList->Visible=(AccessLevel>=iDefHonPrecLevel)?true:false;
    btnManualStandard->Visible=(CUSTOMER_CODE==CC_UTAC_TW || CUSTOMER_CODE==CC_SIGURD_PeiXing);                         //Sam 20220622 : 聯測 & 北興新增手動觸發 Check list 產生。
    AdjtsYieldMonitiorSize();

    asLastOpenFN=GetLastOpenFN();
    sPathName.sprintf("D:\\HT9045_Log\\CheckingList");
    sCheckListName.sprintf("%s\\%s.txt", sPathName, asLastOpenFN);

    cbMonitor_FTPRMS->Checked=IniConfig.bA32EnableFTPAutomation;
    cbContactMode->ItemIndex=fContact->cbContactMode->ItemIndex;

    if(iTestRunMode==RT)
    {
        bFailAlarmLowYieldByTotal   =TestIF_File.bFailAlarmLowYieldByTotal_RT;
        bContsFailBySocket          =TestIF_File.bContsFailBySocket_RT;
        bContsFailByHead            =TestIF_File.bContsFailByHead_RT;
        iLowYieldCountByTotal       =TestIF_File.iLowYieldCountByTotal_RT;
        dFailAlarmSiteYieldCmp      =TestIF_File.dFailAlarmSiteYieldCmp_RT;
        dLowYieldLimitByTotal       =TestIF_File.dLowYieldLimitByTotal_RT;
        iContsFailSocketAlarmCT     =TestIF_File.iContsFailSocketAlarmCT_RT;
        iContsFailHeadAlarmCT       =TestIF_File.iContsFailHeadAlarmCT_RT;
        iFailAlarmSiteYieldCmpCount =TestIF_File.iFailAlarmSiteYieldCmpCount_RT;
    }
    else
    {
        bFailAlarmLowYieldByTotal   =TestIF_File.bFailAlarmLowYieldByTotal;
        bContsFailBySocket          =TestIF_File.bContsFailBySocket;
        bContsFailByHead            =TestIF_File.bContsFailByHead;
        iLowYieldCountByTotal       =TestIF_File.iLowYieldCountByTotal;
        dFailAlarmSiteYieldCmp      =TestIF_File.dFailAlarmSiteYieldCmp;
        dLowYieldLimitByTotal       =TestIF_File.dLowYieldLimitByTotal;
        iContsFailSocketAlarmCT     =TestIF_File.iContsFailSocketAlarmCT;
        iContsFailHeadAlarmCT       =TestIF_File.iContsFailHeadAlarmCT;
        iFailAlarmSiteYieldCmpCount =TestIF_File.iFailAlarmSiteYieldCmpCount;
    }

    //By Site Compare Yield% (1min) & Low Yields%(By Total)
    bCheck=ReadIniData(sCheckListName, "FT_Yield",    "Yield Func",     bFailAlarmLowYieldByTotal);

    cbSiteYieldCmp_FT->Checked=bCheck;
    cbLowYieldByTotal_FT->Checked=bCheck;

    if(bCheck!=bFailAlarmLowYieldByTotal && iTestRunMode==FT)
    {
        edSiteYieldCmpOnOff_Cur->Visible=true;
        edLowYieldByTotalOnOff_Cur->Visible=true;
        edSiteYieldCmpOnOff_Cur->Text=(bFailAlarmLowYieldByTotal)?"On":"OFF";
        edLowYieldByTotalOnOff_Cur->Text=(bFailAlarmLowYieldByTotal)?"On":"OFF";
    }
    else
    {
        edSiteYieldCmpOnOff_Cur->Visible=false;
        edLowYieldByTotalOnOff_Cur->Visible=false;
    }

    //By Site Compare Yield% (1min) & Low Yields%(By Total)
    iCheck=ReadIniData(sCheckListName, "FT_Yield",    "Preset",          iLowYieldCountByTotal);
    edSiteYieldCmpIg_FT->Text=iCheck;
    edLowYieldByTotalIg_FT->Text=iCheck;
    if((iCheck!=iLowYieldCountByTotal ||
        iCheck!=iFailAlarmSiteYieldCmpCount) &&
       iTestRunMode==FT)
    {
        edSiteYieldCmpIg_Cur->Visible=true;
        edLowYieldByTotalIg_Cur->Visible=true;
        edSiteYieldCmpIg_Cur->Text=iFailAlarmSiteYieldCmpCount;
        edLowYieldByTotalIg_Cur->Text=iLowYieldCountByTotal;
        lblSiteYieldCmpIg_Cur->Visible=true;
        lblLowYieldByTotalIg_Cur->Visible=true;
    }
    else
    {
        edSiteYieldCmpIg_Cur->Visible=false;
        edLowYieldByTotalIg_Cur->Visible=false;
        lblSiteYieldCmpIg_Cur->Visible=false;
        lblLowYieldByTotalIg_Cur->Visible=false;
    }

    //By Site Compare Yield% (1min)
    dCheck=ReadIniData(sCheckListName, "FT_Yield", "Variance",        dFailAlarmSiteYieldCmp);
    edSiteYieldCmp_FT->Text=dCheck;
    if(dCheck!=dFailAlarmSiteYieldCmp && iTestRunMode==FT)
    {
        edSiteYieldCmp_Cur->Visible=true;
        edSiteYieldCmp_Cur->Text=dFailAlarmSiteYieldCmp;
        lblSiteYieldCmp_Cur->Visible=true;
    }
    else
    {
        edSiteYieldCmp_Cur->Visible=false;
        lblSiteYieldCmp_Cur->Visible=false;
    }

    //Low Yields%(By Total)
    dCheck=ReadIniData(sCheckListName, "FT_Yield", "Low Yield",      dLowYieldLimitByTotal);
    edLowYieldByTotal_FT->Text=dCheck;
    if(dCheck!=dLowYieldLimitByTotal && iTestRunMode==FT)
    {
        edLowYieldByTotal_Cur->Visible=true;
        edLowYieldByTotal_Cur->Text=dLowYieldLimitByTotal;
        lblLowYieldByTotal_Cur->Visible=true;
    }
    else
    {
        edLowYieldByTotal_Cur->Visible=false;
        lblLowYieldByTotal_Cur->Visible=false;
    }

    //Consecutive Failure Alarm ( Socket )
    bCheck=ReadIniData(sCheckListName, "FT_Yield",    "ContsFailBySocket Func",     bContsFailBySocket);
    rbContsFailBySocket_FTOn ->Checked=bCheck;
    rbContsFailBySocket_FTOff->Checked=!bCheck;

    if(bCheck!=bContsFailBySocket && iTestRunMode==FT)
    {
        edtContsFailBySocket_Cur->Visible=true;
        edtContsFailBySocket_Cur->Text=(bContsFailBySocket)?"On":"OFF";
    }
    else
    {
        edtContsFailBySocket_Cur->Visible=false;
    }

    iCheck=ReadIniData(sCheckListName, "Alarm", "Socket",   (int)iContsFailSocketAlarmCT);
    edContsFailSocketAlarmCT_FT->Text=iCheck;
    if(iCheck!=(int)iContsFailSocketAlarmCT && iTestRunMode==FT)
    {
        edContsFailSocketAlarmCT_Cur->Visible=true;
        edContsFailSocketAlarmCT_Cur->Text=iContsFailSocketAlarmCT;
    }
    else
    {
        edContsFailSocketAlarmCT_Cur->Visible=false;
    }
    lblContsFailSocketAlarmCT_Cur->Visible=(edtContsFailBySocket_Cur->Visible || edContsFailSocketAlarmCT_Cur->Visible)?true:false;

    //Consecutive Failure Alarm ( Head)
    bCheck=ReadIniData(sCheckListName, "FT_Yield",    "ContsFailByHead Func",     bContsFailByHead);
    rbContsFailByHead_FTOn ->Checked=bCheck;
    rbContsFailByHead_FTOff->Checked=!bCheck;
    if(bCheck!=bContsFailByHead && iTestRunMode==FT)
    {
        edtContsFailByHead_Cur->Visible=true;
        edtContsFailByHead_Cur->Text=(bContsFailByHead)?"On":"OFF";
    }
    else
    {
        edtContsFailByHead_Cur->Visible=false;
    }

    iCheck=ReadIniData(sCheckListName, "Alarm", "Head",     (int)TestIF_File.iContsFailHeadAlarmCT);
    edContsFailHeadAlarmCT_FT->Text=iCheck;
    if(iCheck!=(int)iContsFailHeadAlarmCT && iTestRunMode==FT)
    {
        edContsFailHeadAlarmCT_Cur->Visible=true;
        edContsFailHeadAlarmCT_Cur->Text=TestIF_File.iContsFailHeadAlarmCT;
    }
    else
    {
        edContsFailHeadAlarmCT_Cur->Visible=false;
    }

    lblContsFailHeadAlarmCT_Cur->Visible=(edtContsFailByHead_Cur->Visible || edContsFailHeadAlarmCT_Cur->Visible)?true:false;

    //O/S Bin
    iBin=ReadIniData(sCheckListName, "Tester_Control", "SGOSBIN",   0);
    if(iBin>0)
    {
        edOSBin->Text=iBin;
        asString.sprintf("Category%d", iBin);
        iCheck=ReadIniData(sCheckListName, asString, "Fail Percent Ignore",   BinSelect[iTestRunMode].iPersentIgnore[iBin]);
        edOSBinCnt->Text=iCheck;
        if(iCheck!=BinSelect[iTestRunMode].iPersentIgnore[iBin])
        {
            edOSBinCnt_Cur->Text=BinSelect[iTestRunMode].iPersentIgnore[iBin];
            edOSBinCnt_Cur->Visible=true;
        }
        else
        {
            edOSBinCnt_Cur->Visible=false;
        }

        dCheck=ReadIniData(sCheckListName, asString, "Fail Percent Num",   BinSelect[iTestRunMode].dFailureLimit[iBin]);
        edOSBinPreset->Text=dCheck;
        if(dCheck!=BinSelect[iTestRunMode].dFailureLimit[iBin])
        {
            edOSBinPreset_Cur->Text=BinSelect[iTestRunMode].dFailureLimit[iBin];
            edOSBinPreset_Cur->Visible=true;
        }
        else
        {
            edOSBinPreset_Cur->Visible=false;
        }
        lblOSBin_Cur->Visible=(edOSBinCnt_Cur->Visible || edOSBinPreset_Cur->Visible)?true:false;
    }
    else
    {
        edOSBin->Text="Null";
        edOSBinCnt->Text="Null";
        edOSBinPreset->Text="Null";
    }

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //Sam 20210715 : 矽格湖口不要顯示
        grpOSBin->Visible=false;
    else
        grpOSBin->Visible=true;

    AdjtsYieldMonitiorSize();
}
//---------------------------------------------------------------------------
void  TfLotInfo::RefreshYieldMonitor_TERAPOWER()
{
    bool bLowYield=false, bContsFailBySocket=false, bContsFailByHead=false, bAllSiteFail=false, bSiteToSiteYieldEnable=false, bHeadToHeadYieldEnable=false;
    double dLowYield=0.0, dSiteToSiteYield=0.0, dHeadToHeadYield=0.0;
    int iLowYieldIg=0, iContsFailSocketAlarmCT=0, iContsFailHeadAlarmCT=0, iAllSiteFailCount=0, iSiteToSiteYieldCount=0, iHeadToHeadYieldCount=0;

    if(iRunStartMode==FT)
    {
        //Low Yield
        bLowYield               =TestIF_File.bFailAlarmLowYield;
        dLowYield               =TestIF_File.dLowYieldLimit;
        iLowYieldIg             =TestIF_File.iLowYieldCount;
        //Consecutive Failure Alarm ( Socket/Head )
        bContsFailBySocket      =TestIF_File.bContsFailBySocket;
        bContsFailByHead        =TestIF_File.bContsFailByHead;
        iContsFailSocketAlarmCT =TestIF_File.iContsFailSocketAlarmCT;
        iContsFailHeadAlarmCT   =TestIF_File.iContsFailHeadAlarmCT;
        //All Site Fial
        bAllSiteFail            =TestIF_File.bAllSiteFail;
        iAllSiteFailCount       =TestIF_File.iAllSiteFailCount;
    }
    else
    {
        //Low Yield
        bLowYield               =TestIF_File.bFailAlarmLowYield_RT;
        dLowYield               =TestIF_File.dLowYieldLimit_RT;
        iLowYieldIg             =TestIF_File.iLowYieldCount_RT;
        //Consecutive Failure Alarm ( Socket/Head )
        bContsFailBySocket      =TestIF_File.bContsFailBySocket_RT;
        bContsFailByHead        =TestIF_File.bContsFailByHead_RT;
        iContsFailSocketAlarmCT =TestIF_File.iContsFailSocketAlarmCT_RT;
        iContsFailHeadAlarmCT   =TestIF_File.iContsFailHeadAlarmCT_RT;
        //All Site Fial
        bAllSiteFail            =TestIF_File.bAllSiteFail_RT;
        iAllSiteFailCount       =TestIF_File.iAllSiteFailCountRT;
    }
    //Site To Site Yield %
    bSiteToSiteYieldEnable      =TestIF_File.bSiteToSiteYieldCmp;
    dSiteToSiteYield            =TestIF_File.iSiteToSiteYieldCmp;
    iSiteToSiteYieldCount       =TestIF_File.iSiteToSiteYieldCmpCount;
    //Head To Head Yield %
    bHeadToHeadYieldEnable      =TestIF_File.bHeadToHeadYieldCmp;
    dHeadToHeadYield            =TestIF_File.iHeadToHeadYieldCmp;
    iHeadToHeadYieldCount       =TestIF_File.iHeadToHeadYieldCmpCount;

    cbLowYield->Checked                 =bLowYield;
    edLowYield->Text                    =FloatToStr(dLowYield);
    edLowYieldIg->Text                  =IntToStr(iLowYieldIg);
    rbContsFailBySocket_On->Checked     =bContsFailBySocket;
    rbContsFailBySocket_Off->Checked    =!bContsFailBySocket;
    rbContsFailByHead_On->Checked       =bContsFailByHead;
    rbContsFailByHead_Off->Checked      =!bContsFailByHead;
    edContsFailSocketAlarmCT->Text      =IntToStr(iContsFailSocketAlarmCT);
    edContsFailHeadAlarmCT->Text        =IntToStr(iContsFailHeadAlarmCT);
    cbAllSiteFail->Checked              =bAllSiteFail;
    edAllSiteFailCount->Text            =IntToStr(iAllSiteFailCount);
    cb_SiteToSiteYieldEnable->Checked   =bSiteToSiteYieldEnable;
    ed_SiteToSiteYield->Text            =FloatToStr(dSiteToSiteYield);
    ed_SiteToSiteYieldCount->Text       =IntToStr(iSiteToSiteYieldCount);
    cb_HeadToHeadYieldEnable->Checked   =bHeadToHeadYieldEnable;
    ed_HeadToHeadYield->Text            =FloatToStr(dHeadToHeadYield);
    ed_HeadToHeadYieldCount ->Text      =IntToStr(iHeadToHeadYieldCount);

    chk_SmartAutoClean->Checked         =TestIF_File.bACSmart;                  //Sam 20230111 : Smart Auto Clean
    edt_SmartAutoClean->Text            =TestIF_File.iACSmart_Count;
    ed_SmartAutoCleanCTF->Text          =TestIF_File.iACSmart_Count_CTF;        //Sam 20240726 : AI Clean
    lbl_SmartAutoCleanCount->Caption    =IntToStr(iACSmartCount);
    lbl_SmartAutoCleanCount_CTF->Caption=IntToStr(iACSmartCount_CTF);           //Sam 20240726 : AI Clean

    if(iAdaptiveACInterval<=0)
    {
        fCleaning->ChangeACSmartInterval(2, "RefreshYieldMonitor");             //Sam 20240726 : AI Clean
    }
    AnsiString s="";
    s.sprintf("Adaptive Auto Clean Interval : %d/%d  Count", iAutoClean_IndexContactCount, iAdaptiveACInterval);
    lblAdaptiveIntervalCount->Caption=s;
}
//---------------------------------------------------------------------------
void TfLotInfo::WriteFTPSetupFileChangeLog(AnsiString msg)                      //Sam 20210803 : FTP SetFile Change Log
{
    AnsiString Path="",Log="";
    GetTimeInfo();
    Path.sprintf("%s\\%04d_%02d_%02d", sFTPSetupFileLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);
    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, msg);
    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", sFTPSetupFileLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str() , Log.c_str());
}
//---------------------------------------------------------------------------
void TfLotInfo::AdjtsYieldMonitiorSize()
{
    if(pgLotinfo->ActivePage==ts_ATC6_1)                                        //Ztex 2023.04.19 Add HT-1032 TriTemp Function //設定 ATC6_1 Tabsheet
    {
        fLotInfo->Height=610;
        fLotInfo->Width=440;
    }
    else if(pgLotinfo->ActivePage==tsATC)
    {
        fLotInfo->Height=610;
        fLotInfo->Width=580;
    }
    else if(pgLotinfo->ActivePage==tsASECLEventLog)                             //Steven 20190214 : 切換到ASE畫面才變大
    {
        fLotInfo->Width=671;
    }
    else if(CUSTOMER_CODE==CC_PANTHER)
    {
        fLotInfo->Width=671;
    }
//    else if(IniConfig.bSIGURDFunction &&                                      //Sam 20250428 : 修正 A32 上傳顯示問題 Mark
//            pgLotinfo->ActivePage==ts_FTPAutomation)                            //KaiChen 20200414 ：矽格-湖口，修改視窗大小問題
//    {
//        ShowInformation(false);
//    }
    else if(pgLotinfo->ActivePage==tsYieldMonitior &&
            IniConfig.bSIGURDFunction)
    {
        fLotInfo->Width=530;
        if(grpOSBin->Visible && gbManualCheckList->Visible)
            fLotInfo->Height=605;
        else if(grpOSBin->Visible && gbManualCheckList->Visible==false)
            fLotInfo->Height=550;
        else
            fLotInfo->Height=460;
        RefreshYieldMonitor();
    }
    else if(pgLotinfo->ActivePage==tsYieldMonitior &&
            CosFunction.bShowYieldMonitor)
    {
        fLotInfo->Width=530;
        fLotInfo->Height=601;
        RefreshYieldMonitor();
    }
    else if(CosFunction.bEnableHandlerResultServer && pgLotinfo->ActivePage==tsAMR)                                     //Sam 20240304 : 新增 AMR 功能
    {
        Width=420;
        RefreshAMR();
    }
    else if(pgLotinfo->ActivePage==tsFTP)
    {
        fLotInfo->Width=340;
    }
    else if(pgLotinfo->ActivePage==tsLotID && USE_RFID_READER)                  //Steven 20220713 : RFID Reader for SJSEMI
    {
        fLotInfo->Height=666;
    }
    else if(pgLotinfo->ActivePage==tsLotID && tsMurata->TabVisible==false  &&
            tsSigurd_CX->TabVisible==false && tsSPIL_SZ->TabVisible==false &&
            tsOEE->TabVisible==false       && ts2DSort->TabVisible==false  &&
            tsChipAdv->TabVisible==false   && tsVTest->TabVisible==false)       //JerryYang 20230322 : 2D sort lot info UI修改
    {
        pgcLotInfo->Visible=false;
        fLotInfo->Width=410;
    }
    else if(pgLotinfo->ActivePage==tsOCRBarCode)                                //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    {
        fLotInfo->Height=610;
        fLotInfo->Width=530;
    }
    else if(IniConfig.bVTESTFunction==true)
    {
        fLotInfo->Height=470;                                                   //jou 20220824 : tsSelection Height 390 -> 490
        fLotInfo->Width=620;                                                    //RogerYang 20250809 修正畫面被pgcLotInfo擋住
    }
    else
    {
        fLotInfo->Height=490;                                                   //jou 20220824 : tsSelection Height 390 -> 490
//        fLotInfo->Width=555;
        fLotInfo->Width=671;                                                    //Jimmychiu 20250827 : 調整lotinfo寬度
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbTestClick(TObject *Sender)
{
    static bool bFirstIN=true;

    AnsiString asPassword;

    if(gbFTPAutomation_Download->Visible==true)
    {
        ShowInformation(false);
        return;
    }

    asPassword=fSecurity->GetPasswoard();                                       //Sam 20220106 : 整合密碼
    fPassword->edPassword->Text="";
    if(bFirstIN)
    {
        bFirstIN=false;
        #ifdef SOFT_SIMULTE
            ShowInformation(true);
        #else
        fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
        if(asPassword==fPassword->edPassword->Text)
        {
            ShowInformation(true);
        }
        else
        {
            ShowInformation(false);
        }
        #endif
    }

    bFirstIN=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbRecipeUploadClick(TObject *Sender)
{
    static bool bFirstIN=true;
    AnsiString asPassword;
    asPassword=fSecurity->GetPasswoard();                                       //Sam 20220106 : 整合密碼
    fPassword->edPassword->Text="";
    if(bFirstIN)
    {
        bFirstIN=false;
        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                    //Sam 20240102 : 矽格北興俊堯說不要密碼。
            fPassword->edPassword->Text=asPassword;
        else
            fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
        if(asPassword==fPassword->edPassword->Text)
        {
            bSigurdUpload_Recipe=true;
            fFTPClient->ShowFTPModal(1);
            fFTPClient->bShow=false;
        }
    }
    bFirstIN=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbRecipeDownloadClick(TObject *Sender)
{
    bSigurdDownload_Recipe=true;
    fFTPClient->ShowFTPModal(0);
    fFTPClient->bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbFTPAutomationSaveClick(TObject *Sender)
{
    ReadWriteFTPAutomationData(false);
    sbFTPAutomationSave->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnManualCheckListClick(TObject *Sender)
{
    AnsiString asR="";
    bool bIniStart=false;
    if(CUSTOMER_CODE==CC_SIGURD_ChungXing &&
       IniConfig.bA32_3For93KFunction==false)                                   //Sam 20220620 : 中興廠新增 A32-3 功能 for 93K function
    {
        if(LastSet.iRunStartMode==rsmContinuStart)
        {
            asR=fLotInfo->CheckingCheckList();
            if(asR!="OK")
            {
                ShowErrorMessage("WAR16333", 0, MMSystem, false, asR);
            }
        }
    }
    else if(CUSTOMER_CODE==CC_SIGURD_PeiXing ||
            (CUSTOMER_CODE==CC_SIGURD_ChungXing &&                              //Sam 20220620 : 中興廠新增 A32-3 功能 for 93K function
             IniConfig.bA32_3For93KFunction==true))
    {
        if(LastSet.iRunStartMode==rsmContinuStart ||
            LastSet.iRunStartMode==rsmContinuRetest)
        {
           bIniStart=true;
        }
        asR=fLotInfo->CheckingCheckList(bIniStart);
        if(asR!="OK")
        {
            ShowErrorMessage("WAR16333", 0, MMSystem, false, asR);
        }
    }
    else                                                                        //Sam 20250107 : A32 功能新增 Double Force
    {
        if(LastSet.iRunStartMode==rsmContinuStart  ||
           LastSet.iRunStartMode==rsmContinuRetest ||
           LastSet.iRunStartMode==rsmInitialStart)
        {
            AnsiString asR=fLotInfo->CheckingCheckList();
            if(asR!="OK")
            {
                ShowErrorMessage("WAR16333", 0, MMSystem, false, asR);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnFTPTryConnectClick(TObject *Sender)
{
    fFTPClient->CheckFTPConnection();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btTesterTCPShowClick(TObject *Sender)
{
    fTesterTCP->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnClearTemperatureClick(TObject *Sender)
{
    bChangeTest_TempOffset=1;                                                   //Ifor 20230505 add: 需要有Offset溫度才會設定回生產溫度
    DoCheckHasTestTempChange();                                                 //Ifor 20230505 add: 確認是否測試中有切換溫度並切回原生產溫度
}
//---------------------------------------------------------------------------
void TfLotInfo::SaveGroundESDByLot(AnsiString sLot,AnsiString sCoustomerLot, AnsiString sStation, AnsiString sStationNum)
{
    if(CosFunction.bRecordGroundESDByTestIC==false)
        return;
    AnsiString s="";

    GetTimeInfo();
    AnsiString aSourceFilePath="", aCopyPath="", aCopyFilePath="", aFileName="",aStartMode="FT";
    aSourceFilePath.sprintf("%s\\%s.csv", sGroundESDLogPath, sLot);             //D:\\HT9045_Log\\GroundESDLog\\XXXX.csv
    aCopyPath.sprintf("%s\\%04d-%02d", sGroundESDLogPath, SystemYear, SystemMonth);                                     //D:\\HT9045_Log\\GroundESDLog\\yyyy-mm

    if(LastSet.iRunStartMode==rsmInitialStart || LastSet.iRunStartMode==rsmContinuStart)
        aStartMode="FT";
    else if(LastSet.iRunStartMode==rsmCInitialRetest || LastSet.iRunStartMode==rsmContinuRetest)
        aStartMode="RT";
    else if(LastSet.iRunStartMode==rsmQAMode)
        aStartMode="EQC";

    aFileName.sprintf("%s_%s_%s_%s-%04d%02d%02d_%02d%02d.csv",                  //LotId_CoustomerLotId_Station_StationNumber_yyyymmdd_hhmm.csv
                                                sLot,
                                                sCoustomerLot,
                                                sStation,
                                                sStationNum,
                                                SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);

    aCopyFilePath.sprintf("%s\\%s", aCopyPath, aFileName);                      //D:\\HT9045_Log\\GroundESDLog\\yyyy-mm\\LotId_CoustomerLotId_Station_StationNumber_yyyymmdd_hhmm.csv

    if(FileExists(aSourceFilePath)==false)
        return;

    MyForceDirectories(aCopyPath);
    CopyFile(aSourceFilePath.c_str(), aCopyFilePath.c_str(), false);
    DeleteFile(aSourceFilePath);

    if(IniConfig.bN30_UseGroundESDUpdataToFTP)
    {
        fFTPClient->UploadFileFTP(aCopyPath,
                                aFileName,
                                IniConfig.sN30_FTPUploadPath,
                                aFileName,
                                IniConfig.sN30_FTPUserName,
                                IniConfig.sN30_FTPPassword,
                                IniConfig.sN30_FTPHost,
                                __FUNC__);

        fFTPClient->UploadFileFTP(aCopyPath,                                    //Sam 20220816 : GroundESD 新增第二組上傳
                                aFileName,
                                IniConfig.sN30_FTPUploadPath2,
                                aFileName,
                                IniConfig.sN30_FTPUserName,
                                IniConfig.sN30_FTPPassword,
                                IniConfig.sN30_FTPHost,
                                __FUNC__);
        s.sprintf("Upload GroundESD %s", aFileName);
        RecordProcess(s);
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::AutoTempOfsByFTP()                                              //Sam 20220406 : 溫度自動補償功能 By FTP
{
    if(CosFunction.bAutoTmpeOfsByFTP==false ||
       IniConfig.iN31_UseAutoTempOfsByFTP==0 ||
       IniConfig.iN31_ContactCnt<1)
    {
        LastSet.iAutoTempOfsTriggerCnt=0;
        return;
    }
    int iArm=0;
    double dTempureOfs=0.0;
    AnsiString sAutoTempOfsFile="", sTempOfs="";                                //, asFileName="";
    AnsiString cmd="", msg="";
    static int iCount=0;
    bool bExcute=false;

    if(CUSTOMER_CODE==CC_UTAC_TW)                                               //Sam 20230815 : 聯測改為 Interval time
    {
        iCount++;
        if(iCount>IniConfig.iN31_ContactCnt)
        {
            iCount=0;
            bExcute=true;
        }
    }
    else
    {
        if(LastSet.iAutoTempOfsTriggerCnt>=IniConfig.iN31_ContactCnt)
            bExcute=true;
    }

    if(bExcute)
    {
        LastSet.iAutoTempOfsTriggerCnt=0;
        if(IniConfig.iN31_UseAutoTempOfsByFTP==2 ||
           CUSTOMER_CODE==CC_UTAC_TW)
            sAutoTempOfsFile=fFTPClient->N31_AtuoTempureOfsByLocal();
        else
            sAutoTempOfsFile=fFTPClient->N31_AtuoTempureOfsByFTP();

        sTempOfs=sAutoTempOfsFile.UpperCase();
        if(sTempOfs.Pos("SETTEMPOFS")>1)
        {
            RecordProcess("AutoTempOffsetFilePath", sAutoTempOfsFile);
            WriteIniData(sAutoTempOfsFile, "EXCUTE_RECORD", "Time", Now().FormatString("yyyy/mm/dd hh:nn:ss"));         //將有執行過的指令寫回檔案方便後續查詢
            TStringList *tsTempOfs= new TStringList();

            if(CheckKeyExist(sAutoTempOfsFile, "SetTempOfs", "TempOfs Arm 1"))
            {                                                                   //Steven 20250103 : auto temp offset加上兩個arm可以同時變更
                tsTempOfs->Clear();
                sTempOfs=ReadIniData(sAutoTempOfsFile, "SetTempOfs", "TempOfs Arm 1", AnsiString("0,0"));
                msg.sprintf("ARM 1 : %s", sTempOfs);
                RecordProcess("SetTempOfs", msg);
                tsTempOfs->CommaText=sTempOfs;

                for(int i=0; i<tsTempOfs->Count; i++)
                {
                    if(i>32)                                                    //最大吃到 32Site
                        break;

                    dTempureOfs=atof(tsTempOfs->Strings[i].c_str());
                    if(dTempureOfs!=0)                                          //Arm1
                        fTemp_Set->SaveRemoteTempOffset(1, i+1, dTempureOfs);
                }
            }

            if(CheckKeyExist(sAutoTempOfsFile, "SetTempOfs", "TempOfs Arm 2"))
            {                                                                   //Steven 20250103 : auto temp offset加上兩個arm可以同時變更
                tsTempOfs->Clear();
                sTempOfs=ReadIniData(sAutoTempOfsFile, "SetTempOfs", "TempOfs Arm 2", AnsiString("0,0"));
                msg.sprintf("ARM 2 : %s", sTempOfs);
                RecordProcess("SetTempOfs", msg);
                tsTempOfs->CommaText=sTempOfs;
                for(int i=0; i<tsTempOfs->Count; i++)
                {
                    if(i>32)                                                    //最大吃到 32Site
                        break;

                    dTempureOfs=atof(tsTempOfs->Strings[i].c_str());
                    if(dTempureOfs!=0)                                          //Arm2
                        fTemp_Set->SaveRemoteTempOffset(2, i+1, dTempureOfs);
                }
            }

            if(CheckKeyExist(sAutoTempOfsFile, "SetTempOfs", "Arm"))
            {
                iArm=ReadIniData(sAutoTempOfsFile, "SetTempOfs", "Arm", -1);
                sTempOfs=ReadIniData(sAutoTempOfsFile, "SetTempOfs", "TempOfs", AnsiString("0,0"));
                msg.sprintf("ARM %d : %s", iArm, sTempOfs);

                RecordProcess("SetTempOfs", msg);
                tsTempOfs->CommaText=sTempOfs;

                if(0<=iArm && iArm<=2)                                          //Sam 20230727 : 新增 Remote command
                {
                    for(int i=0; i<tsTempOfs->Count; i++)
                    {
                        if(i>32)                                                //最大吃到 32Site
                            break;
                        dTempureOfs=atof(tsTempOfs->Strings[i].c_str());
                        if((iArm==0 || iArm==1) && dTempureOfs!=0)              //Arm1
                            fTemp_Set->SaveRemoteTempOffset(1, i+1, dTempureOfs);

                        if((iArm==0 || iArm==2) && dTempureOfs!=0)              //Arm2
                            fTemp_Set->SaveRemoteTempOffset(2, i+1, dTempureOfs);
                    }
                }
            }

            fTemp_Set->ReadRemoteTempOffset();                                  //Sam 20220516 : 將新的 offset 讀回來。
            tsTempOfs->Clear();
            delete tsTempOfs;

            cmd=ReadIniData(sAutoTempOfsFile, "Remote", "Command", AnsiString(""));                                     //Sam 20230727 : 新增 Remote command
            if(cmd=="ONECYCLE")
            {
                msg=ReadIniData(sAutoTempOfsFile, "ONECYCLE", "Message", AnsiString(""));

                if((IniConfig.iN31_UseAutoTempOfsByFTP==2 ||
                    CUSTOMER_CODE==CC_UTAC_TW) &&
                   msg=="")
                {
                    msg="ONECYCLE by OI_FTP";
                }

                if(msg!="")
                {
                    iOneCycleFinishShowMsg=1;                                   //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
                    sOneCycleFinishShowMsg=StringReplace(msg, ",", "\n", TReplaceFlags()<<rfReplaceAll);
                    RecordProcess("Remote Command ONECYCLE "+msg);
                }
                else
                {
                    iOneCycleFinishShowMsg=2;                                   //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
                    sOneCycleFinishShowMsg="";
                    RecordProcess("Remote Command ONECYCLE");
                }
                WriteIniData(sAutoTempOfsFile, "EXCUTE_RECORD", "ONECYCLE",msg);                                        //將有執行過的指令寫回檔案方便後續查詢
                fMain->BtnOneCycleClick(fMain);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnManualStandardClick(TObject *Sender)
{
    fMain->CheckList();                                                         //Sam 20220622 : 聯測 & 北興新增手動觸發 Check list 產生。
    RefreshYieldMonitor();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edStationNumMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 99);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnGetLoaderClick(TObject *Sender)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;

    DoReadRFID(Ptr->Tag);
}
//---------------------------------------------------------------------------
void TfLotInfo::DoReadRFID(int Pos)
{
    iRFIDTag=Pos;                                                               //Steven 20220713 : RFID Reader for SJSEMI
    AnsiString CMD="~00 RMID*", Log, Log1;

    fLotInfo->pnlLoader->Caption="";

    RFID_Reader->StopComm();
    InitRFIDRS232();
    MySleep(10);
    RFID_Reader->WriteCommData(CMD.c_str(), CMD.Length());

    Log1.sprintf("Loader : %s", CMD.c_str());

    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d.%03d, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Log1);
    mmRFID->Lines->Add(Log);

    #ifdef SOFT_SIMULTE
        if(fLotInfo->pnlLoader->Caption=="")
            fLotInfo->pnlLoader->Caption="12345";
    #endif
}
//---------------------------------------------------------------------------
void TfLotInfo::InitRFIDRS232()
{
    bool flag;
    AnsiString str;

    flag=GetCOMPortStatus(HSys.asRFIDCom);
    if(flag==false)
    {
        str.sprintf("RFID %d: %s port status error!", 1, HSys.asRFIDCom);
        ShowMyMessage(str);
    }
    else
    {
        RFID_Reader->BaudRate=HSys.RFIDBaudRate;
        if(HSys.RFIDByteSize==5)
            RFID_Reader->ByteSize = TByteSize(_5);
        else if(HSys.RFIDByteSize==6)
            RFID_Reader->ByteSize = TByteSize(_6);
        else if(HSys.RFIDByteSize==7)
            RFID_Reader->ByteSize = TByteSize(_7);
        else if(HSys.RFIDByteSize==8)
            RFID_Reader->ByteSize = TByteSize(_8);

        if(HSys.RFIDStopBit==1)
            RFID_Reader->StopBits = TStopBits(_1);
        else if(HSys.RFIDStopBit==1.5)
            RFID_Reader->StopBits = TStopBits(_1_5);
        else if(HSys.RFIDStopBit==2)
            RFID_Reader->StopBits = TStopBits(_2);

        if(HSys.RFIDParity=="None")
            RFID_Reader->Parity   = None;
        else if(HSys.RFIDParity=="Odd")
            RFID_Reader->Parity   = Odd;
        else if(HSys.RFIDParity=="Even")
            RFID_Reader->Parity   = Even;
        else if(HSys.RFIDParity=="Space")
            RFID_Reader->Parity   = Space;
        else if(HSys.RFIDParity=="Mark")
            RFID_Reader->Parity   = TParity(3);

        RFID_Reader->CommName="\\\\.\\"+HSys.asRFIDCom;
        try
        {
            RFID_Reader->StopComm();
            RFID_Reader->StartComm();                                           //僅能啟動一次
        }
        catch(...)
        {
            str.sprintf("RFID %d: %s port start error!", 1, HSys.asRFIDCom);
            ShowMyMessage(str);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::RFID_ReaderReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    char *data;                                                                 //Steven 20220713 : RFID Reader for SJSEMI
    data=(char*)Buffer;
    AnsiString cStr="", Log="", Log1="";
    char cStr1[1024]={0};
    strncpy(cStr1, data, BufferLength);
    cStr=cStr1;
    cStr=cStr.Delete(1, 13);
    cStr=cStr.SubString(1, cStr.Length()-1);
    if(cStr=="")
    {
        cStr="Data error-Read Fail";
    }

    fLotInfo->pnlLoader->Caption=cStr;
    Log1.sprintf("Loader : %s", cStr.c_str());

    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d.%03d, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Log1);
    mmRFID->Lines->Add(Log);
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnLoadFileClick(TObject *Sender)
{
    AnsiString SortFileName;
    OpenDialog1->Title="Open 2DID Sort File";
    if(OpenDialog1->Execute())
    {
        SortFileName=OpenDialog1->FileName;
        edSort2DIDBinFile->Text=SortFileName;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::spOCRCleanListClick(TObject *Sender)
{
    fOCR->ListOCRByLot->Clear();
    fOCR->ListOCRByLot->SaveToFile(asOCRLotPath);
    spOCRCleanList->Down=false;
}
//---------------------------------------------------------------------------
bool TfLotInfo::bCheckOnlyOneFile(AnsiString asPath, AnsiString &asFileName)    //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
{
    TFileListBox *flbStr=new TFileListBox(this);
    int iCT=-1;
    bool bResult=true;

    flbStr->Parent=this;
    flbStr->Directory=asPath;
    flbStr->Mask="*.*";
    flbStr->Refresh();
    flbStr->Update();

    iCT=flbStr->Items->Count;
    if(iCT!=1)
        bResult=false;
    else
        asFileName=flbStr->Items->Strings[0];

    delete flbStr;
    return bResult;
}
//---------------------------------------------------------------------------
bool TfLotInfo::bCheckOnlyOneFileAndData()                                      //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
{
    bool bResult=true;
    AnsiString asProdPath="D:\\HT9045_Log\\Production_Info", asFileName="";
    AnsiString asCheck="", asPath="", asFileNameByFile="";

    MyForceDirectories(asProdPath);

    if(fLotInfo->bCheckOnlyOneFile(asProdPath, asFileName)==false)
    {
        ShowMyMessage("More than oone file or file not exist in folder, Please check TestInf.txt");
        bResult=false;
    }
    else
    {
        TStringList *ProductionInfo=new TStringList;
        asProdPath+="\\TestInfo.txt";
        ProductionInfo->LoadFromFile(asProdPath);

        asLotID_ByFile  =ProductionInfo->Strings[0];                            //擷取資料
        asType_ByFile   =ProductionInfo->Strings[3];
        asStation_ByFile=ProductionInfo->Strings[11];
        asState_ByFile  =ProductionInfo->Strings[16];

        if(asLotID_ByFile.Pos("FLOWCARDNO:")==0)
            asCheck="FLOWCARDNO item is not exist, Please check TestInfo.txt";
        else if(asType_ByFile.Pos("TYPE:")==0)
            asCheck="TYPE item is not exist, Please check TestInfo.txt";
        else if(asStation_ByFile.Pos("STATION:")==0)
            asCheck="STATION item is not exist, Please check TestInfo.txt";
        else if(asState_ByFile.Pos("STATE:")==0)
            asCheck="STATE item is not exist, Please check TestInfo.txt";

        if(asCheck!="")
        {
            ShowMyMessage(asCheck);
            ProductionInfo->Clear();
            delete ProductionInfo;
            return false;
        }

        asLotID_ByFile  =asLotID_ByFile.SubString(asLotID_ByFile.Pos(":")+1, asLotID_ByFile.Length()-asLotID_ByFile.Pos(":"));
        asType_ByFile   =asType_ByFile.SubString(asType_ByFile.Pos(":")+1, asType_ByFile.Length()-asType_ByFile.Pos(":"));
        asStation_ByFile=asStation_ByFile.SubString(asStation_ByFile.Pos(":")+1, asStation_ByFile.Length()-asStation_ByFile.Pos(":"));
        asState_ByFile  =asState_ByFile.SubString(asState_ByFile.Pos(":")+1, asState_ByFile.Length()-asState_ByFile.Pos(":"));

        if(asLotID_ByFile=="")
        {
            ShowMyMessage("FLOWCARDNO is NULL, Please check TestInfo.txt");
            bResult=false;
        }
        else if(asType_ByFile=="")
        {
            ShowMyMessage("TYPE is NULL, Please check TestInfo.txt");
            bResult=false;
        }
        else if(asStation_ByFile=="")
        {
            ShowMyMessage("STATION is NULL, Please check TestInfo.txt");
            bResult=false;
        }
        else if(asState_ByFile=="")
        {
            ShowMyMessage("STATE is NULL, Please check TestInfo.txt");
            bResult=false;
        }

        ProductionInfo->Clear();
        delete ProductionInfo;

        if(bResult)                                                             //處理路徑
        {
            asPath.sprintf("%s\\%s\\%s\\", AnsiString(asProductionByFilePath), asStation_ByFile, asType_ByFile);
            MyForceDirectories(asPath);

            asFileNameByFile.sprintf("%s_%s_%s.csv", asLotID_ByFile, asStation_ByFile, asState_ByFile);
            asPath+=asFileNameByFile;
            asProductionByFileNamePath=asPath;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
void TfLotInfo::SetTesterStartTimeByB03()                                       //Sam 20240809 : PTI ART 模式
{
    if(IniConfig.bB03_TesterReport)                                             //Sam 20231115 : PTI 新增 Tester report
    {
        GetTimeInfo();
        AnsiString sTemp="", sTemp1="";
        sTemp=IntToStr(SystemYear)+"/"+IntToStr(SystemMonth)+"/"+IntToStr(SystemDate);
        sB03RunData=sTemp;
        sTemp1=IntToStr(SystemHour)+":"+IntToStr(SystemMin)+":"+IntToStr(SystemSec);
        sB03StartTime=sTemp1;
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ProductTesterReport()                                           //Sam 20231115 : PTI 新增 Tester report  QQQ
{
    AnsiString Path="",sTemp="";
    AnsiString sCustomer="",sDeviceID="",sTestType="",sTestCycle="",sYield="", sEndTime="";
    AnsiString sTotalUnits="",sPassUnits="",sOpenUnits="",sShortUnits="";
    int iTotalUnits[8],iPassUnits[8],iOpenUnits[8],iShortUnits[8];
    ZeroMemory(iTotalUnits, sizeof(iTotalUnits));
    ZeroMemory(iPassUnits, sizeof(iPassUnits));
    ZeroMemory(iOpenUnits, sizeof(iOpenUnits));
    ZeroMemory(iShortUnits, sizeof(iShortUnits));

    int iSum=0,iPass=0, iTestCycle=0;
    MyForceDirectories(asTesterReportPath);

    sEndTime=Now().FormatString("hh:nn:ss");

    unsigned int BinCT[4][256];                                                 //Sam 20240809 : PTI ART 模式
    long SendCT[4];
    if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)
    {
         memcpy(&BinCT, &LastSet.BinCT_ART, sizeof(LastSet.BinCT_ART));
         memcpy(&SendCT, &LastSet.SendCT_ART, sizeof(LastSet.SendCT_ART));
    }
    else
    {
         memcpy(&BinCT, &LastSet.BinCT, sizeof(LastSet.BinCT));
         memcpy(&SendCT, &LastSet.SendCT, sizeof(LastSet.SendCT));
    }
    /*
    if(iRunStartMode==FT)
    {
        sTestCycle="1'st";
        iTestCycle=1;
    }
    else
    {
        sTestCycle="2'nd";
        iTestCycle=2;
    }
    */
    if(fLotInfo->cbRunMode->Text!="")
    {
        sTestCycle=fLotInfo->cbRunMode->Text;
        iTestCycle=atoi(sTestCycle.SubString(0, 1).c_str());
        if(iTestCycle==0)                                                       //Sam 20240930 : 修正報表檔名
            iTestCycle=1;
    }

    //Yield
    iSum=0;
    iPass=0;
    for(int i=0; i<6; i++)
    {
        iSum+=BinCT[0][i];
        if(BinSelect[iTestRunMode].iStackDefFailCate[i]==0)
            iPass+=BinCT[0][i];
    }

    sYield=ChangeToPercentage((double)iPass, (double)iSum);

    sCustomer=IniConfig.sB03_Customer;
    sDeviceID=IniConfig.sB03_DeviceID;
    sTestType=ChangeToPercentage((double)iSum, (double)SendCT[0]);

    Path.sprintf("%s\\%s_%s_%s_%d.csv", asTesterReportPath, sCustomer, fLotInfo->edtSysLotID->Text, fLotInfo->cbProcess->Text, iTestCycle);

    if(FileExists(Path))
    {
        DeleteFile(Path);
    }

    //Row1
    sTemp="";
    WriteDataToFile(Path, sTemp);
    //Row2
    sTemp=" , , , ,OS Tester Summary Report";
    WriteDataToFile(Path, sTemp);
    //Row3
    sTemp.sprintf("Customer:,%s,,,Lot No:,%s", sCustomer, fLotInfo->edtSysLotID->Text);
    WriteDataToFile(Path, sTemp);
    //Row4
    sTemp.sprintf("Device ID:,%s,,,Run Date:,%s", sDeviceID, sB03RunData);
    WriteDataToFile(Path, sTemp);
    //Row5
    sTemp.sprintf("Program Name:,%s,,,Start Time:,%s", GetLastOpenFN(), sB03StartTime);
    WriteDataToFile(Path, sTemp);
    //Row6
    sTemp.sprintf("OP ID:,%s,,,End Time:,%s", fLotInfo->edtSysOperatorID->Text, sEndTime);
    WriteDataToFile(Path, sTemp);
    //Row8
    sTemp.sprintf("Test Type:,%s,,,Test Cycle:,%s", sTestType, sTestCycle);
    WriteDataToFile(Path, sTemp);
    //Row9
    sTemp.sprintf("Input Q'ty:,%d,Bin 1 Q'ty,%d,Yield:,%s", iSum, BinCT[0][0], sYield);
    WriteDataToFile(Path, sTemp);
    //Row9
    sTemp.sprintf("Bin 2 Q'ty:,%d,Bin 3 Q'ty:,%d,Bin 4 Q'ty:,%d", BinCT[0][1], BinCT[0][2], BinCT[0][3]);
    WriteDataToFile(Path, sTemp);
    //Row10
    sTemp.sprintf("Bin 5 Q'ty:,%d,Bin 6 Q'ty:,%d", BinCT[0][4], BinCT[0][5]);
    WriteDataToFile(Path, sTemp);
    //Row10
    sTotalUnits="";
    sPassUnits="";
    sOpenUnits="";
    sShortUnits="";
    for(int iSiteNo=0; iSiteNo<8; iSiteNo++)
    {
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(TestIF_File.iSiteMap[i][j]==iSiteNo+1 && TestIF_File.iSiteMap[i][j]>0)
                {
                    iTotalUnits[iSiteNo]=ArmData[2]->ArmSKET[i][j]->GetTotal();
                    iPassUnits [iSiteNo]=ArmData[2]->ArmSKET[i][j]->GetPassCT();
                    iOpenUnits [iSiteNo]=ArmData[2]->ArmSKET[i][j]->GetFailCT();
                    iShortUnits[iSiteNo]=0;
                }
            }
        }

        if(iSiteNo!=7)
        {
            sTotalUnits+=IntToStr(iTotalUnits[iSiteNo])+"/";
            sPassUnits+=IntToStr(iPassUnits[iSiteNo])+"/";
            sOpenUnits+=IntToStr(iOpenUnits[iSiteNo])+"/";
            sShortUnits+=IntToStr(iShortUnits[iSiteNo])+"/";
        }
        else
        {
            sTotalUnits+=IntToStr(iTotalUnits[iSiteNo]);
            sPassUnits+=IntToStr(iPassUnits[iSiteNo]);
            sOpenUnits+=IntToStr(iOpenUnits[iSiteNo]);
            sShortUnits+=IntToStr(iShortUnits[iSiteNo]);
        }
    }

    sTemp.sprintf("Total Units (Site1/2/3/4/5/6/7/8): %s", sTotalUnits);
    WriteDataToFile(Path, sTemp);
    sTemp.sprintf("Pass  Units (Site1/2/3/4/5/6/7/8): %s", sPassUnits);
    WriteDataToFile(Path, sTemp);
    sTemp.sprintf("Open  Units (Site1/2/3/4/5/6/7/8): %s", sOpenUnits);
    WriteDataToFile(Path, sTemp);
    sTemp.sprintf("Short Units (Site1/2/3/4/5/6/7/8): %s", sShortUnits);
    WriteDataToFile(Path, sTemp);
    sTemp.sprintf("Loader Q'ty: %d,Unloader Q'ty: %d", SendCT[0],iSum);
    WriteDataToFile(Path, sTemp);
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::btnAirStreamOnOffClick(TObject *Sender)
{
   /*
    double dSetAirMachineTemp = Temperature.fSetTempature2AirMachine;
    double dSetAirMachineTemp2= Temperature.dSetIndexAirstreamTemp;
    if(AccessLevel<1)
    {
        ShowMyMessage("Change Air Cooling Machinr Status Fail # Please Log In for executing Engineer");
        return;
    }

    if(!SystemStart)
    {
        if(btnAirStreamOnOff->Caption=="Air Stream Stop")
        {
            ATC_InterfaceForm->bCloseAirMachine=true;
            btnAirStreamOnOff->Caption="Air Machine Run";                       //add Manual Air Cooling On/Off
            ATC_InterfaceForm->SendAirMachineStatus(0, dSetAirMachineTemp*10, dSetAirMachineTemp2*10);
        }
        else
        {
            ATC_InterfaceForm->bCloseAirMachine=false;
            btnAirStreamOnOff->Caption="Air Stream Stop";                       //add Manual Air Cooling On/Off
            ATC_InterfaceForm->SendAirMachineStatus(1, dSetAirMachineTemp*10, dSetAirMachineTemp2*10);
        }
    }
    */
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::rgOEEStateClick(TObject *Sender)
{
    if(InitialOK==false || fMesSystem->iOEEStateChangeCnt>0)                    //RogerYang 20250529 加入保護，曾發生狀態莫名其妙一直切換導致LOG暴增
    {
        return;
    }

    if(AccessLevel==0 && SystemStart==false)
    {
        if(rgOEEState->ItemIndex<1 || rgOEEState->ItemIndex>3)
        {
            fMesSystem->iOEEStateChangeCnt++;                                   //RogerYang 20250529 加入保護，曾發生狀態莫名其妙一直切換導致LOG暴增
            rgOEEState->ItemIndex=fMesSystem->iOEEState;
            if(fMesSystem->iOEEStateChangeCnt>0)                                //RogerYang 20250529 加入保護，曾發生狀態莫名其妙一直切換導致LOG暴增
                fMesSystem->iOEEStateChangeCnt--;
            return;
        }
    }

    if(rgOEEState->ItemIndex>0)
        fMesSystem->WriteOEEState("State : "+rgOEEState->Items->Strings[rgOEEState->ItemIndex]);
    fMesSystem->iOEEState=rgOEEState->ItemIndex;
    NewRecordProcess("", labOEEState->Caption, "cbbOEEStateClick");
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtLotVerifyMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    edtLotVerify->Text=TestIF_File.sLotIDSubstr;
    return;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edtLotVerifyKeyPress(TObject *Sender, char &Key)
{
    edtLotVerify->Text=TestIF_File.sLotIDSubstr;
    return;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::BtnPauseClick(TObject *Sender)
{
    fMain->Pause("fLotInfo");
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::BtnPauseMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    BtnPause->Down=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::BtnPauseMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    BtnPause->Down=false;
}
//---------------------------------------------------------------------------
void TfLotInfo::ResetLotInfo()                                                  //Steven 20240925 : 重開軟體時, 要讀回lot info
{
    if(IniConfig.bShowLotInfo)
    {
        if(HasICUnderMachine() ||                                               //Steven 20110527
           HasAutoICInMachine())                                                //jou 20211129 : 修正 VTEST Tray end 結束後重開程式會重置 Auto Tray盤造成疊料。
        {
            ReadRmsInfo();
            edDeviceName->Text  =IniConfig.sProductName;
            cbbDeviceName->Text =IniConfig.sProductName;
            edTemp->Text        =IniConfig.sProductTemp;
            LastSet.bHasDownloadFile=true;
        }
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ShowATCTempPanel()                                              //Steven 20241112 : 調整ATC溫度顯示
{
    if(iATC_Use_Heat_Count==8)                                                  //Ifor 20160507 add
    {
        Pan_ATC_Use_4Head->Left=4;
        Pan_ATC_Use_8Head->Top=216;
        Pan_ATC_Use_8Head->Left=4;
        Pan_ATC_Use_8Head->Visible=true;
        Pan_ATC_Use_32Head->Visible=false;
    }
    else if(iATC_Use_Heat_Count>8)
    {
        Pan_ATC_Use_4Head->Left=0;
        Pan_ATC_Use_4Head->Top=124;
        Pan_ATC_Use_8Head->Top=124;                                             //JerryYang 20250729 : ATC 3.3+6.0
        Pan_ATC_Use_8Head->Left=272;
        Pan_ATC_Use_8Head->Visible=true;
        Pan_ATC_Use_32Head->Visible=true;
    }
    else
    {
        Pan_ATC_Use_4Head->Left=4;
        Pan_ATC_Use_4Head->Top=124;
        Pan_ATC_Use_8Head->Visible=false;
        Pan_ATC_Use_32Head->Visible=false;
    }

    for(int i=0; i<ATC_HEAD_COUNT; i++)
    {
        ATCChPal[i]->Visible=(i<iATC_Use_Heat_Count);
        ATCPtr[i]->Visible=(i<iATC_Use_Heat_Count);
    }

    if(Temperature.bUseReferTempSensor==true)                                   //Ifor 20160711 修改第二點溫度顯示 開啟 & 隱藏
    {
        for(int i=0; i<ATC_HEAD_COUNT; i++)
        {
            ATCReferPtr[i]->Visible=(i<iATC_Use_Heat_Count);
        }
    }
    else
    {
        for(int i=0; i<ATC_HEAD_COUNT; i++)
            ATCReferPtr[i]->Visible=false;
    }
}
//---------------------------------------------------------------------------
bool TfLotInfo::CheckNoRetestBinFlag()                                          //RogerYang 20250604 偉測不可複測bin功能
{
    bool flag=true;
    for(int i=0; i<3; i++)
    {
        if(fMesSystem->NeedNoRTBinID(i)==true)
        {
            ShowErrorMessage(sMES1713[i], K_RETRY|K_SKIP, iMMAuto[eFix1+i], false);                                     //RogerYang 20250806 修正FixIndex
            flag=false;
        }
    }

    if(flag==false)
    {
        flag=true;
        for(int i=0; i<3; i++)                                                  //再檢查一次，如果user都輸入正確，則繼續執行
        {
            if(fMesSystem->NeedNoRTBinID(i)==true)
                flag=false;
        }
    }
    return flag;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnPATInstallClick(TObject *Sender)
{
    if(IniConfig.bB12UsePATSetup==false)
    {
        AnsiString sErrMsg=AnsiString().sprintf("Please enable the [B12] function in the config.");
        ShowMyMessage(sErrMsg);
        return;
    }
    PAT_Function* patFunc=fMain->patFunc;
    ClearLotInfoPAT();
    AnsiString sSetupFileName=patFunc->GetPATSetUpFile();
    if(FileInfo().IsFilePathExist(sSetupFileName))
    {
        if(patFunc->LoadSetupFile(sSetupFileName)==true)
        {
            if(leRunCardNumber->Text==patFunc->patSetup.lotNo)
            {
                UpdateLotInfoPAT();
                AnsiString sJobFullFileName=patFunc->GetPATJobFileDownload();
                AnsiString sRecipePath=DataPath+patFunc->patSetup.recipe;
                if(DirectoryExists(sRecipePath)==false)                         //copy now receipe and create new
                {
                    fProductionInfo->CopyFolder(DataPath+fProductionInfo->GetSetUpName()+"\\",sRecipePath+"\\");
                }
                fMain->ChangeSetUpFile(patFunc->patSetup.recipe);
                if(FileInfo().IsFilePathExist(sJobFullFileName))
                {
                    if(patFunc->LoadJobINI(sJobFullFileName))
                    {
                        //
                    }
                    else
                    {
                        AnsiString sErrMsg=AnsiString().sprintf("Load Job File failed. FileName=%s",sJobFullFileName);
                        ShowMyMessage(sErrMsg);
                    }
                }
                else
                {
                    AnsiString sErrMsg=AnsiString().sprintf("Job file download path does not exist.%s Path=%s",FileInfo().GetNewLine(),sJobFullFileName);
                    ShowMyMessage(sErrMsg);
                }
            }
            else
            {
                AnsiString sErrMsg=AnsiString().sprintf("RunCardNumber Error. %s KeyData=%s %s Server Data=%s",FileInfo().GetNewLine(),
                                                                                                               leRunCardNumber->Text,
                                                                                                               FileInfo().GetNewLine(),
                                                                                                               patFunc->patSetup.lotNo);
                ShowMyMessage(sErrMsg);
            }
        }
        else
        {
            AnsiString sErrMsg=AnsiString().sprintf("Load Set up File failed. FileName=%s",sSetupFileName);
            ShowMyMessage(sErrMsg);
        }
    }
    else
    {
        AnsiString sErrMsg=AnsiString().sprintf("File path does not exist.%s Path=%s",FileInfo().GetNewLine(),sSetupFileName);
        ShowMyMessage(sErrMsg);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbRTCASTDClick(TObject *Sender)
{
    if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum &&                            //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD
       bTriggerRTC_AutoSTD==false && cbRTCASTD->Checked==true)
    {
        iRTC_AutoSTDTask=1;
        bTriggerRTC_AutoSTD=true;
//        Timer4->Enabled=true;
        cbRTCASTD->Enabled=false;
        RecordProcess("Trigger RTC Auto STD Process.");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::ReadAirMachineStatus()                               //Add CheckAirMachine Status
{
    static TQPF_Timer Delay;                                                    //讀取AirMachine狀態
    static bool bTimerStart=true;
    double dSetAirMachineTemp =0.0;                                             //Ztex 2023.12.19 Add Air Stream Offset
    double dSetAirMachineTemp2=0.0;                                             //Ztex 2023.12.19 Add Air Stream Offset
    double dDefrostTemp       =IniConfig.iL32_8SetAirStreamTemp;

    if(bTimerStart==true)
    {
        bTimerStart=false;
        Delay.SetSecAndOn(1);                                                   //Isaac 20181225 : 一秒記錄一次
    }

    if(AirStream_Select==0)
        return;

    Temperature.dSetIndexAirstreamTemp  =TransformTemperature_AirStream(Temperature.dAirStreamIndex_Offset,0);          //Ztex 2023.12.19 Add Air Stream Offset
    Temperature.fSetTempature2AirMachine=TransformTemperature_AirStream(Temperature.dAirStreamSocket_Offset,1);         //Ztex 2023.12.19 Add Air Stream Offset

    dSetAirMachineTemp =Temperature.fSetTempature2AirMachine;                   //Ztex 2023.12.19 Add Air Stream Offset
    dSetAirMachineTemp2=Temperature.dSetIndexAirstreamTemp;                     //Ztex 2023.12.19 Add Air Stream Offset

    if(Delay.Off())
    {
        bTimerStart=true;
        bCheckUse_AirStream=(Temperature.bEnableArm_1_Air ||
                             Temperature.bEnableArm_2_Air ||
                             Temperature.bEnableSocket_Air);

        if(Tri_Temp_Machine==1 && ATC_InterfaceForm->IsConnect() && AirStream_Select==1)                                //Chunghung 20170712 modify for aircool show alarm
        {
            btnAirStreamOnOff->Visible=bCheckUse_AirStream;
            if(ATC_InterfaceForm->Send_Rec_AirMachineStatus.iA2H_RecStatus==0 &&
               bDefrostKeepATCTemp==false)
            {
                if(IniConfig.bL37UnDockTurnOffAir==true && USE_OTD==2)
                {
                    if(Sen[SenAutoDocking240KG].Status()==false &&  Sen[SenAutoDocking360KG].Status()==false)
                    {
                        ATC_InterfaceForm->SendAirMachineStatus(0, dSetAirMachineTemp*10, dSetAirMachineTemp2*10);
                    }
                    else
                    {
                        ATC_InterfaceForm->SendAirMachineStatus(1, dSetAirMachineTemp*10, dSetAirMachineTemp2*10);
                    }
                }
                else
                {
                    ATC_InterfaceForm->SendAirMachineStatus(1, dSetAirMachineTemp*10, dSetAirMachineTemp2*10);
                }
            }

            if(ATC_InterfaceForm->Send_Rec_AirMachineStatus.iA2H_RecStatus==0 &&
               bDefrostKeepATCTemp==true)
            {
                if(bAutoDefrost_OpenAirStream==true)
                {
                    ATC_InterfaceForm->SendAirMachineStatus(1, dDefrostTemp*10, dDefrostTemp*10);
                }
                else
                {
                    ATC_InterfaceForm->SendAirMachineStatus(0, dDefrostTemp*10, dDefrostTemp*10);
                }
            }

            if(ATC_InterfaceForm->Send_Rec_AirMachineStatus.iA2H_RecStatus==1)
            {
                CheckAirMachineStatus();
                ATC_InterfaceForm->Send_Rec_AirMachineStatus.iA2H_RecStatus=0;
            }
        }
        else
        {
            btnAirStreamOnOff                ->Visible = false;                 // add Manual Air Cooling On/Off
            palAirMachineStatus              ->Caption = "DisConnect";
            palAirMachineSetTemperature      ->Caption = "Off-Line";            // modify change  " " ->"Off-Line"
            palAirMachineAlarmStatus         ->Caption = "No";
            palAirDefrostSec                 ->Caption = "No";
            pal_Air_Machine_Temp_Ch1         ->Caption = "999";
            pal_Air_Machine_Temp_Ch2         ->Caption = "999";
            pal_AirStream_AirVolume          ->Caption = "999";

            palAirMachineStatus_Index        ->Caption = "DisConnect";
            palAirMachineSetTemperature_Index->Caption = "Off-Line";
            palAirMachineAlarmStatus_Index   ->Caption = "No";
            palAirDefrostSec_Index           ->Caption = "No";
            pnl_AirMachineTemp_Index_Ch_1    ->Caption = "999";
            pnl_AirMachineTemp_Index_Ch_2    ->Caption = "999";
            pal_AirStream_AirVolume_Index    ->Caption = "999";
            ATC_InterfaceForm->Send_Rec_AirMachineStatus.iA2H_RecStatus=0;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::CheckAirMachineStatus()                              //Add CheckAirMachine Status
{
    if(AirStream_Select==0)
        return;

    palAirMachineStatus              ->Caption = ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineStatus;
    palAirMachineSetTemperature      ->Caption = ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineSetTemp;
    palAirMachineAlarmStatus         ->Caption = ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineAlarm;
    palAirDefrostSec                 ->Caption = ATC_InterfaceForm->AirMachineInfo.iDefrostSec;                         //kevin 20161230
    pal_Air_Machine_Temp_Ch1         ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo.iATC_AirMachineSocketChTemp[0]);
    pal_Air_Machine_Temp_Ch2         ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo.iATC_AirMachineSocketChTemp[1]);
    pal_AirStream_AirVolume          ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo.iATC_AirVolume);

    if(ATC_InterfaceForm->iATC_MODE_TYPE==61)
    {
        palAirMachineStatus_Index        ->Caption = ATC_InterfaceForm->AirMachineInfo_Index.asATC_AirMachineStatus;
        palAirMachineSetTemperature_Index->Caption = ATC_InterfaceForm->AirMachineInfo_Index.asATC_AirMachineNowTemp;
        palAirMachineAlarmStatus_Index   ->Caption = ATC_InterfaceForm->AirMachineInfo_Index.asATC_AirMachineAlarm;
        palAirDefrostSec_Index           ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iDefrostSec);
        pnl_AirMachineTemp_Index_Ch_1    ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirMachineSocketChTemp[0]);
        pnl_AirMachineTemp_Index_Ch_2    ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirMachineSocketChTemp[1]);
        pal_AirStream_AirVolume_Index    ->Caption = AnsiString(ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirVolume);
    }

    if(bUT150Install[tcATCHotAir1]==true || bUT150Install[tcATCHotAir2]==true)
    {
        if(ATC_InterfaceForm->IsConnect())
        {
            UN150Read[tcATCHotAir1]=ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirMachineSocketChTemp[1];
            UN150Read[tcATCHotAir2]=ATC_InterfaceForm->AirMachineInfo.iATC_AirMachineSocketChTemp[1];
        }
        else
        {
            UN150Read[tcATCHotAir1]=999;
            UN150Read[tcATCHotAir2]=999;
        }
    }

    if(SystemStart)
    {
        if(Temperature.EnableAirMachineSocket)
        {
            if(palAirMachineStatus->Caption != "Run" && palAirMachineStatus->Caption != "NoUse")
            {
                if(LastSet.iLanguageCountry==1)
                    ShowMyMessage("冷風機正在執行自動除霜，請等待除霜完成再操作");
                else
                    ShowMyMessage("The air cooler is performing automatic defrosting, \n please wait until it is completed before using it");
            }
            else if(palAirMachineStatus->Caption != "Run" && palAirMachineStatus->Caption != "NoUse")
            {
                if(bManualAirCoolingOnOff ==false)
                    ShowErrorMessage("WAR1611", K_RETRY, MMSystem);
            }
        }
        else
        {
            if(palAirMachineStatus->Caption == "0")
            {
                ShowErrorMessage("WAR1611", K_RETRY, MMSystem);
            }
        }
    }
}
//Hmy 20161129 add air machine <-
//---------------------------------------------------------------------------
void TfLotInfo::SetCloseSiteTemp(double dTemp)                                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function
{
    for(int i=0; i<ATC_MAX_SITE; i++)                                           //設定關閉Site的溫度
        fTemp_Set->SetSingleWorkTemperature(i, dTemp);
}
//---------------------------------------------------------------------------
void TfLotInfo::InitialRefrigerantSystem()
{
    if(AirStream_Select==0)
        return;

    TPanel *pnlRefrigerantMachine[8]={pnlRefrigerantMachine1,pnlRefrigerantMachine2,pnlRefrigerantMachine3,pnlRefrigerantMachine4,
                                        pnlRefrigerantMachine5,pnlRefrigerantMachine6,pnlRefrigerantMachine7,pnlRefrigerantMachine8};

    TLabel *LabRefrigerantValue[8]={LabRefrigerantValue1,LabRefrigerantValue2,LabRefrigerantValue3,LabRefrigerantValue4,
                                        LabRefrigerantValue5,LabRefrigerantValue6,LabRefrigerantValue7,LabRefrigerantValue8};

    TPanel *pnlRefCopm1Status[8]={pnlRefCopm1Status_1,pnlRefCopm1Status_2,pnlRefCopm1Status_3,pnlRefCopm1Status_4,
                                        pnlRefCopm1Status_5,pnlRefCopm1Status_6,pnlRefCopm1Status_7,pnlRefCopm1Status_8};

    TPanel *pnlRefCopm2Status[8]={pnlRefCopm2Status_1,pnlRefCopm2Status_2,pnlRefCopm2Status_3,pnlRefCopm2Status_4,
                                        pnlRefCopm2Status_5,pnlRefCopm2Status_6,pnlRefCopm2Status_7,pnlRefCopm2Status_8};

    TLabel *LabRefCopm1HpValue[8]={labRefCopm1HpValue_1,labRefCopm1HpValue_2,labRefCopm1HpValue_3,labRefCopm1HpValue_4,
                                        labRefCopm1HpValue_5,labRefCopm1HpValue_6,labRefCopm1HpValue_7,labRefCopm1HpValue_8};

    TLabel *LabRefCopm2HpValue[8]={labRefCopm2HpValue_1,labRefCopm2HpValue_2,labRefCopm2HpValue_3,labRefCopm2HpValue_4,
                                        labRefCopm2HpValue_5,labRefCopm2HpValue_6,labRefCopm2HpValue_7,labRefCopm2HpValue_8};

    TLabel *LabRefCopm1LpValue[8]={labRefCopm1LpValue_1,labRefCopm1LpValue_2,labRefCopm1LpValue_3,labRefCopm1LpValue_4,
                                        labRefCopm1LpValue_5,labRefCopm1LpValue_6,labRefCopm1LpValue_7,labRefCopm1LpValue_8};

    TLabel *LabRefCopm2LpValue[8]={labRefCopm2LpValue_1,labRefCopm2LpValue_2,labRefCopm2LpValue_3,labRefCopm2LpValue_4,
                                        labRefCopm2LpValue_5,labRefCopm2LpValue_6,labRefCopm2LpValue_7,labRefCopm2LpValue_8};

    TLabel *LabRefrigerantAdjustValue[8]={LabRefrigerantAdjustValue1,LabRefrigerantAdjustValue2,LabRefrigerantAdjustValue3,LabRefrigerantAdjustValue4,
                                        LabRefrigerantAdjustValue5,LabRefrigerantAdjustValue6,LabRefrigerantAdjustValue7,LabRefrigerantAdjustValue8};
    for(int i=0; i<8; i++)
    {
        TripnlRefrigerantMachine[i] =pnlRefrigerantMachine[i];
        TriLabRefrigerantValue[i]   =LabRefrigerantValue[i];
        TripnlRefCopm1Status[i]     =pnlRefCopm1Status[i];
        TripnlRefCopm2Status[i]     =pnlRefCopm2Status[i];
        TriLabRefCopm1HpValue[i]    =LabRefCopm1HpValue[i];
        TriLabRefCopm2HpValue[i]    =LabRefCopm2HpValue[i];
        TriLabRefCopm1LpValue[i]    =LabRefCopm1LpValue[i];
        TriLabRefCopm2LpValue[i]    =LabRefCopm2LpValue[i];
        TriLabRefrigerantAdjustValue[i]    =LabRefrigerantAdjustValue[i];
    }
    bInitFormcomponent = false;
    OldRefrigerantCommand = false;
    ATC_OFFLINE_FormComInit();
    if(Total_Compressor>5)
        ts_RefrigerantStatus_Page_2->TabVisible=true;
    else
        ts_RefrigerantStatus_Page_2->TabVisible=false;

    for(int i=0; i<8; i++)
    {
        if(i<Total_Compressor)
        {
            pnlRefrigerantMachine[i]->Visible=true;
        }
        else
        {
            pnlRefrigerantMachine[i]->Visible=false;
        }
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ScanRefrigerantSystem()
{
    if(AirStream_Select==0)
        return;

    if(iATCOnLine==0 || ATC_InterfaceForm->IsConnect()==false)
    {
        if(bInitFormcomponent==false)
        {
            ATC_OFFLINE_FormComInit();
        }
        ATC_InterfaceForm->bReadRefrigerantMode_Send=false;
        ATC_InterfaceForm->bReadRefrigerantMode_Recv=false;
    }
    else
    {
        bInitFormcomponent=false;
        if(ATC_InterfaceForm->bReadRefrigerantMode_Recv==true)
        {
            ATC_InterfaceForm->bReadRefrigerantMode_Recv=false;
            RefreshRefrigerantAllStatus();
        }

        if(ATC_InterfaceForm->bReadRefrigerantMode_Send==false)
            ATC_InterfaceForm->Get_ATCRefrigeratorAllStatus(true);
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::ATC_OFFLINE_FormComInit()
{
    if(AirStream_Select==0)
        return;

    bInitFormcomponent = true;
    for(int i=0;i<8;i++)
    {
        TriLabRefrigerantValue[i]->Caption = "-999.0";
        TripnlRefCopm1Status[i]->Color = clGray;
        TripnlRefCopm2Status[i]->Color = clGray;
        TriLabRefCopm1HpValue[i]->Caption = "-999.0";
        TriLabRefCopm2HpValue[i]->Caption = "-999.0";
        TriLabRefCopm1LpValue[i]->Caption = "-999.0";
        TriLabRefCopm2LpValue[i]->Caption = "-999.0";
        TriLabRefrigerantAdjustValue[i]->Caption = "-999.0";
        TriLabRefrigerantAdjustValue[i]->Visible = false;
        TripnlRefrigerantMachine[i]->Color = clGray;
    }
    labRefrigerantMachineHighLimit->Caption     ="Comp#2 Hp Over High Limit : -999.0";
    labRefrigerantMachineLowLimit->Caption      ="Comp#2 Hp Over Low Limit : -999.0";

    OldRefrigerantCommand = false;
}
//---------------------------------------------------------------------------
void TfLotInfo::RefreshRefrigerantAllStatus()
{
    if(AirStream_Select==0)
        return;

    char strRefrigeratorAllStatu[8][40];
    if(ATC_InterfaceForm->bReadRefrigerantMode_AllStatus ==true)
    {
        for(int i=0; i<iATC_Refrigerator_Num; i++)
        {
            if(ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][0] ==1)
            {
                TripnlRefrigerantMachine[i]->Color = clGreen;
            }
            else
            {
                TripnlRefrigerantMachine[i]->Color = clGray;
            }
            sprintf(strRefrigeratorAllStatu[0], "%5.0f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][1]);
            sprintf(strRefrigeratorAllStatu[1], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][2]);
            sprintf(strRefrigeratorAllStatu[2], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][3]);
            sprintf(strRefrigeratorAllStatu[3], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][4]);
            sprintf(strRefrigeratorAllStatu[4], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][5]);
            sprintf(strRefrigeratorAllStatu[5], "%5.1f", ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][6]);
            TriLabRefrigerantValue[i]->Caption  = strRefrigeratorAllStatu[0];
            TriLabRefCopm1HpValue[i]->Caption   = strRefrigeratorAllStatu[1];
            TriLabRefCopm1LpValue[i]->Caption   = strRefrigeratorAllStatu[2];
            TriLabRefCopm2HpValue[i]->Caption   = strRefrigeratorAllStatu[3];
            TriLabRefCopm2LpValue[i]->Caption   = strRefrigeratorAllStatu[4];
            TriLabRefrigerantAdjustValue[i]->Caption    = strRefrigeratorAllStatu[5];
            if(ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][7] ==1)
            {
                TripnlRefCopm1Status[i]->Color = clGreen;
            }
            else
            {
                TripnlRefCopm1Status[i]->Color = clGray;
            }

            if(ATC_InterfaceForm->dATC_RefrigeratorAllStatus[i][8] ==1)
            {
                TripnlRefCopm2Status[i]->Color = clGreen;
            }
            else
            {
                TripnlRefCopm2Status[i]->Color = clGray;
            }
        }
        sprintf(strRefrigeratorAllStatu[6],"Comp#2 Hp Over High Limit : %5.1f",ATC_InterfaceForm->dATC_RefrigerantMachineHighLimit);
        sprintf(strRefrigeratorAllStatu[7],"Comp#2 Hp Over Low Limit : %5.1f",ATC_InterfaceForm->dATC_RefrigerantMachineLowLimit);
        labRefrigerantMachineHighLimit->Caption     =strRefrigeratorAllStatu[6];
        labRefrigerantMachineLowLimit->Caption      =strRefrigeratorAllStatu[7];
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            if(ATC_InterfaceForm->iATC_RefrigeratorUserMode[0][i]==1)
            {
                TripnlRefrigerantMachine[i]->Color = clGreen;
            }
            else
            {
                TripnlRefrigerantMachine[i]->Color = clGray;
            }
        }

        if(OldRefrigerantCommand ==false)
        {
            OldRefrigerantCommand = true;
            for(int i=1; i<8; i++)
            {
                TriLabRefrigerantValue[i]->Caption  = FloatToStr(-999.0);
                TriLabRefCopm1HpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefCopm1LpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefCopm2HpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefCopm2LpValue[i]->Caption   = FloatToStr(-999.0);
                TriLabRefrigerantAdjustValue[i]->Caption    = FloatToStr(-999.0);
                TripnlRefCopm1Status[i]->Color = clGray;
                TripnlRefCopm2Status[i]->Color = clGray;
                TripnlRefrigerantMachine[i]->Color = clGray;
            }
        }
    }
}                                                                               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
//---------------------------------------------------------------------------
double TfLotInfo::TransformTemperature_AirStream(double Offset,int iIndex)      //Ztex 2023.12.19 Add Air Stream Offset ==>
{
    if(AirStream_Select==0)
        return 0.0;

    TIniFile *tIni = new TIniFile("D:\\HT9045\\config\\AirStream.ini");

    AnsiString sTemp_Array[16]={"-60~-51","-50~-41","-40~-21","-20~-11","-10~0"  ,"1~15"   ,"16~25"  ,"26~50",
                                "51~80"  ,"81~90"  ,"91~105" ,"106~120","121~130","131~145","146~160","161~175"};

    AnsiString sLoadIni="";
    double dbSetTemp=atof(fMain->edWorkTemperBase->Text.c_str());
    double dRseult=dbSetTemp;

    int iTemp=-1;
    if(-60<=dbSetTemp && dbSetTemp<=-51)
        iTemp=0;
    else if(-50<=dbSetTemp && dbSetTemp<=-41)
        iTemp=1;
    else if(-40<=dbSetTemp && dbSetTemp<=-21)
        iTemp=2;
    else if(-20<=dbSetTemp && dbSetTemp<=-11)
        iTemp=3;
    else if(-10<=dbSetTemp && dbSetTemp<=0)
        iTemp=4;
    else if(1<=dbSetTemp && dbSetTemp<=15)
        iTemp=5;
    else if(16<=dbSetTemp && dbSetTemp<=25)
        iTemp=6;
    else if(26<=dbSetTemp && dbSetTemp<=50)
        iTemp=7;
    else if(51<=dbSetTemp && dbSetTemp<=80)
        iTemp=8;
    else if(81<=dbSetTemp && dbSetTemp<=90)
        iTemp=9;
    else if(91<=dbSetTemp && dbSetTemp<=105)
        iTemp=10;
    else if(106<=dbSetTemp && dbSetTemp<=120)
        iTemp=11;
    else if(121<=dbSetTemp && dbSetTemp<=130)
        iTemp=12;
    else if(131<=dbSetTemp && dbSetTemp<=145)
        iTemp=13;
    else if(146<=dbSetTemp && dbSetTemp<=160)
        iTemp=14;
    else if(161<=dbSetTemp && dbSetTemp<=175)
        iTemp=15;

    if(iTemp==-1)
    {
        delete tIni;
        return dRseult;
    }

    if(!tIni->ValueExists("Temp_Index", "-60~-51"))
    {
        tIni->WriteString("Temp_Index", "-60~-51", "-70");
        tIni->WriteString("Temp_Index", "-50~-41", "-70");
        tIni->WriteString("Temp_Index", "-40~-21", "-60");
        tIni->WriteString("Temp_Index", "-20~-11", "-60");
        tIni->WriteString("Temp_Index", "-10~0"  , "-50");
        tIni->WriteString("Temp_Index", "1~15"   , "-50");
        tIni->WriteString("Temp_Index", "16~25"  , "-50");
        tIni->WriteString("Temp_Index", "26~50"  , "25");
        tIni->WriteString("Temp_Index", "51~80"  , "30");
        tIni->WriteString("Temp_Index", "81~90"  , "30");
        tIni->WriteString("Temp_Index", "91~105" , "35");
        tIni->WriteString("Temp_Index", "106~120", "35");
        tIni->WriteString("Temp_Index", "121~130", "35");
        tIni->WriteString("Temp_Index", "131~145", "35");
        tIni->WriteString("Temp_Index", "146~160", "35");
        tIni->WriteString("Temp_Index", "161~175", "35");

        tIni->WriteString("Temp_Socket","-60~-51", "-70");
        tIni->WriteString("Temp_Socket","-50~-41", "-70");
        tIni->WriteString("Temp_Socket","-40~-21", "-50");
        tIni->WriteString("Temp_Socket","-20~-11", "-30");
        tIni->WriteString("Temp_Socket","-10~0"  , "-10");
        tIni->WriteString("Temp_Socket","1~15"   , "-10");
        tIni->WriteString("Temp_Socket","16~25"  , "10");
        tIni->WriteString("Temp_Socket","26~50"  , "25");
        tIni->WriteString("Temp_Socket","51~80"  , "90");
        tIni->WriteString("Temp_Socket","81~90"  , "100");
        tIni->WriteString("Temp_Socket","91~105" , "120");
        tIni->WriteString("Temp_Socket","106~120", "140");
        tIni->WriteString("Temp_Socket","121~130", "145");
        tIni->WriteString("Temp_Socket","131~145", "160");
        tIni->WriteString("Temp_Socket","146~160", "190");
        tIni->WriteString("Temp_Socket","161~175", "215");
    }

    dRseult=0;
    if(iIndex==0)
    {
        sLoadIni=tIni->ReadString("Temp_Index",sTemp_Array[iTemp],sLoadIni);
        dRseult= Offset+atof(sLoadIni.c_str());
        if(dRseult>35)
            dRseult=35;
    }
    else
    {
        sLoadIni=tIni->ReadString("Temp_Socket",sTemp_Array[iTemp],sLoadIni);
        dRseult = Offset+atof(sLoadIni.c_str());
    }

    if(dRseult<-70)
        dRseult=-70;

    if(dRseult>230)
        dRseult=230;

    delete tIni;
    return dRseult;
}                                                                               //Ztex 2023.12.19 Add Air Stream Offset <==
//---------------------------------------------------------------------------
FILE *WriteGroundRSDLOG;
//------------------------------------------------------------------------------
void TfLotInfo::SaveGroundESDData_Upolad()                                      //Sam 20250609 : Record Ground and ESD at intervals and upload
{
    //記錄當下的 Ground & ESD
    bool bHasFile=false;
    TStringList *SList;
    AnsiString aSaveFilePath="" ,aTitle="" , sDate="", sTime="", sDate1="", sTime1="", sRecord="", aFileName="", aFilePath="";
    AnsiString sGroundAll="", sGround="", sESD="";
    sDate=GetDateInfoByString();
    sTime=GetOnlyTimeInfoByString();
    sDate1=GetDateInfoByString("/");
    sTime1=GetOnlyTimeInfoByString(":");

    aTitle=aTitle                  +
           "Date, "                +
           "Time,"                 +
           "InArm,"                +
           "OutArm,"               +
           "Arm1,"                 +
           "Arm2,"                 +
           "HP1,"                  +
           "HP2,"                  +
           "SH1,"                  +
           "SH2,"                  +
           "InArm_A,"              +
           "InArm_B,"              +
           "InArm_C,"              +
           "InArm_D,"              +
           "InArm_E,"              +
           "InArm_F,"              +
           "InArm_G,"              +
           "InArm_H,"              +
           "OutArm_A,"             +
           "OutArm_B,"             +
           "OutArm_C,"             +
           "OutArm_D,"             +
           "OutArm_E,"             +
           "OutArm_F,"             +
           "OutArm_G,"             +
           "OutArm_H,"             +
           "Loader,"               +
           "Empty,"                +
           "Color,"                +
           "Auto1,"                +
           "Auto2,"                +
           "Auto3,"                +
           "Reserve1,"             +
           "Reserve1,"             +
           "EnableAlarm,"          +
           "AlarmContinuousTimes," +
           "AlarmOccurrence,"      +
           "ScanInterval,"         +

           "Station1_Status,"      +
           "Station1_1_Status,"    +
           "Station1_1_Setting,"   +
           "Station1_1_Value,"     +
           "Station1_2_Status,"    +
           "Station1_2_Setting,"   +
           "Station1_2_Valu,"      +
           "Station1_3_Status,"    +
           "Station1_3_Setting,"   +
           "Station1_3_Value,"     +

           "Station2_Status,"      +
           "Station2_1_Status,"    +
           "Station2_1_Setting,"   +
           "Station2_1_Value,"     +
           "Station2_2_Status,"    +
           "Station2_2_Setting,"   +
           "Station2_2_Valu,"      +
           "Station2_3_Status,"    +
           "Station2_3_Setting,"   +
           "Station2_3_Value,"     +

           "Station3_Status,"      +
           "Station3_1_Status,"    +
           "Station3_1_Setting,"   +
           "Station3_1_Value,"     +
           "Station3_2_Status,"    +
           "Station3_2_Setting,"   +
           "Station3_2_Valu,"      +
           "Station3_3_Status,"    +
           "Station3_3_Setting,"   +
           "Station3_3_Value,"     +

           "Station4_Status,"      +
           "Station4_1_Status,"    +
           "Station4_1_Setting,"   +
           "Station4_1_Value,"     +
           "Station4_2_Status,"    +
           "Station4_2_Setting,"   +
           "Station4_2_Valu,"      +
           "Station4_3_Status,"    +
           "Station4_3_Setting,"   +
           "Station4_3_Value,"     +

           "Station5_Status,"      +
           "Station5_1_Status,"    +
           "Station5_1_Setting,"   +
           "Station5_1_Value,"     +
           "Station5_2_Status,"    +
           "Station5_2_Setting,"   +
           "Station5_2_Valu,"      +
           "Station5_3_Status,"    +
           "Station5_3_Setting,"   +
           "Station5_3_Value,"     +

           "Station6_Status,"      +
           "Station6_1_Status,"    +
           "Station6_1_Setting,"   +
           "Station6_1_Value,"     +
           "Station6_2_Status,"    +
           "Station6_2_Setting,"   +
           "Station6_2_Valu,"      +
           "Station6_3_Status,"    +
           "Station6_3_Setting,"   +
           "Station6_3_Value";

    if(DirectoryExists(sGroundESDInterValPath)==false)
        ForceDirectories(sGroundESDInterValPath);
    aSaveFilePath.sprintf("%s\\%s_GroundESD_%s%s.csv", sGroundESDInterValPath, IniConfig.asA32_1_HandlerID, sDate, sTime);
    aFileName.sprintf("%s_GroundESD_%s%s.csv", IniConfig.asA32_1_HandlerID, sDate, sTime);
    bHasFile=FileExists(aSaveFilePath);
    if(bHasFile==false)
    {
        SList=new TStringList();
        SList->Add(aTitle);
        SList->SaveToFile(aSaveFilePath);
        SList->Clear();
        delete SList;
    }

    if(fGroundMan->asGroundVaule=="NA")
    {
        sGround="";
        for(int i=0;i<4;i++)
            for(int j=0;j<8;j++)
                sGround+=",";                                                   //Sam 20250609 : Record Ground and ESD at intervals and upload
        sGroundAll=sGround;
    }
    else
    {
        sGroundAll=fGroundMan->asGroundVaule;
    }

    if(sESDLogData=="")
    {
        sESDLogData="NA";
    }
    else
    {
        sESD=sESDLogData;
    }
    sRecord=sDate1                      +","+
            sTime1                      +","+
            sGroundAll   +
            sESDLogData;
    WriteGroundRSDLOG=fopen(aSaveFilePath.c_str(), "a+");
    if(WriteGroundRSDLOG!=NULL)
    {
        try
        {
            fprintf(WriteGroundRSDLOG, "%s \n", sRecord);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfLotInfo::SaveGroundESDData_Upolad");
        }
    }

    fclose(WriteGroundRSDLOG);

    fFTPClient->UploadFileFTP(sGroundESDInterValPath,
                              aFileName,
                              IniConfig.sN35_FTPUploadPath,
                              aFileName,
                              IniConfig.sN35_FTPUserName,
                              IniConfig.sN35_FTPPassword,
                              IniConfig.sN35_FTPHost,
                              __FUNC__);
}
//------------------------------------------------------------------------------
bool TfLotInfo::RMSDownloadByNetwork(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sSetupFileName)
{
    if(sSetupFileName.Pos(".zip")==0)                                           //RogerYang 20250813 修正檔案無法複製
        sSetupFileName+=".zip";
    AnsiString sSource=FileInfo().PathCombin(sSourcesFilePath, sSetupFileName);
    //sSource+=".zip";
    AnsiString sTarget=FileInfo().PathCombin(sTargetFilePath, sSetupFileName);
    //sTarget+=".zip";
    int ret=CopyFile(sSource.c_str(), sTarget.c_str(), false);
    MySleep(500);                                                               //jou 2016-01-06 100->500 Unload 上傳失敗修正
    return (ret==1);
}
//------------------------------------------------------------------------------
bool TfLotInfo::RMSDownloadByFTP(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sSetupFileName)
{
    AnsiString asUserID="",asPassword="",asHost="";
    GetFTP_SettingN06(asUserID,asPassword, asHost);
    TfFTP fFTP(asUserID,asPassword, asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        if(fFTP.DownloadFilterFile(sSourcesFilePath, sTargetFilePath, sSetupFileName, asError))
        {
            return true;
        }
        else
        {
            AnsiString sErr=AnsiString().sprintf("%s FTP Download Failed. FileName=%s", __FUNC__,sSetupFileName);
            ShowMyMessage(sErr);
        }
    }
    else
    {
        AnsiString sErr=AnsiString().sprintf("%s FTP Connect Failed", __FUNC__);
        ShowMyMessage(sErr);
    }
    return false;
}
//------------------------------------------------------------------------------
bool TfLotInfo::RMSUploadByNetwork(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sSetupFileName)
{
    return RMSDownloadByNetwork(sSourcesFilePath,sTargetFilePath,sSetupFileName);
}
//------------------------------------------------------------------------------
bool TfLotInfo::RMSUploadByFTP(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sSetupFileName)
{
    AnsiString asUserID="",asPassword="",asHost="";
    GetFTP_SettingN06(asUserID,asPassword, asHost);
    TfFTP fFTP(asUserID,asPassword, asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        if(fFTP.Upload(sSourcesFilePath, sTargetFilePath, sSetupFileName, asError))
        {
            return true;
        }
        else
        {
            AnsiString sErr=AnsiString().sprintf("%s FTP Upload Failed. FileName=%s", __FUNC__,sSetupFileName);
            ShowMyMessage(sErr);
        }
    }
    else
    {
        AnsiString sErr=AnsiString().sprintf("%s FTP Connect Failed", __FUNC__);
        ShowMyMessage(sErr);
    }
    return false;
}
//------------------------------------------------------------------------------
void TfLotInfo::GetFTP_SettingN06(AnsiString &asUserID, AnsiString &asPassword, AnsiString &asHost)
{
    #ifdef SOFT_SIMULTE
    asUserID="HONPREC";
    asPassword="27025312";
    asHost="127.0.0.1";
    #else
    asUserID=IniConfig.FtpUserName;
    asPassword=IniConfig.FtpPassword;
    asHost=IniConfig.FtpHost;
    #endif
}
//------------------------------------------------------------------------------
void TfLotInfo::DoUpload(AnsiString sSetupFileName, AnsiString sCustomName)
{
    int i=0,iPos=0;
    bool bFlag=false;
    AnsiString asStringC, asStringE, asBuffer;
    TStringList *slErrorCode;
    SetMD5ByFolder(DataPath+sSetupFileName);                                    //Steven 20170927 (wei) : 將工作檔加入檢查碼
    if(IniConfig.bEnableRms)
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        {
            bFlag=false;
            asBuffer.sprintf("%sSPIL.txt",DataPath);
            if(FileExists(asBuffer))
                DeleteFile(asBuffer);

            if(FileExists(asBuffer))
                bFlag=true;

            for(i=0; i<FileListBox1->Items->Count; i++)
            {
                if(FileExists(FileListBox1->Items->Strings[i]))
                  DeleteFile(FileListBox1->Items->Strings[i]);

                if(FileExists(FileListBox1->Items->Strings[i]))
                    bFlag=true;
            }

            if(bFlag==true)
            {
                ShowMyMessage("Recipe folder data delete fail","1030,所有於Recipe folder之檔案刪除失敗-Package recipe & .TXT");
                return;
            }

            asBuffer="D:\\HT9045\\IniData\\SPIL\\HandlerAutoUpload.bat";
            if(FileExists(asBuffer))
            {
                system(asBuffer.c_str());
            }
            else
            {
                ShowMyMessage("default error (程式啟動錯誤,無法啟動java檔案)", "1001,default error, need to inform IT, please!!");
                return;
            }

            asBuffer.sprintf("%sSPIL.txt",DataPath);
            if(FileExists(asBuffer)==false)
            {
                ShowMyMessage("Error code.TXT file missing (Error code.TXT檔案遺失)", "1035,Error code.TXT file missing, call IT to check, please!!");
                return;
            }
            else
            {
                bFlag=false;
                slErrorCode =new TStringList;
                slErrorCode->LoadFromFile(asBuffer);
                iPos=slErrorCode->Strings[0].Length();
                i=atoi(slErrorCode->Strings[0].SubString(1,1).c_str());
                if(iPos==1 && i==0)
                {
                    ShowMyMessage("0,Recipe upload success (Recipe上傳成功)","");
                }
                else if(iPos>=4)
                {
                    if(i==1)
                    {
                        i=atoi(slErrorCode->Strings[0].SubString(2,3).c_str());
                        if(i>0 && i<100)
                        {
                            if(i==1)
                            {
                                ShowMyMessage("default error (程式啟動錯誤,無法啟動java檔案)", "1001,default error, need to inform IT, please!!");
                            }
                            else if(i==3)
                            {
                                ShowMyMessage("LOTID-Stage format error (Lot ID 格式錯誤)", "1003,lotid-stage error, check typo and correct first!!");
                            }
                            else if(i==4)
                            {
                                ShowMyMessage("temp & soak & L/B error (temp & soak & L/B 找不到資訊或未建立)", "1004,temp,soak time or L/B error, need to check the data, please!!");
                            }
                            else if(i==12)
                            {
                                ShowMyMessage("Recipe not maintain (Recipe 尚未建立)", "1012,Setup file not be builded, need call supervisor to build up, please!!");
                            }
                            else if(i==31)
                            {
                                ShowMyMessage("Recipe loading error (Recipe載入失敗)", "1031,Recipe loading error, call IT to check , please!!");
                            }
                            else if(i==58)
                            {
                                ShowMyMessage("FTP make dir error (FTP server 建立A+B+C資料夾失敗)", "1058,FTP make dir error, need to call IT dispose, please!!");
                            }
                            else if(i==71)
                            {
                                ShowMyMessage("1071,temp & soak database connect error (temp & soak database 連線失敗)", "temp & soak database connect error, need to check the connect first.");
                            }
                            else if(i==72)
                            {
                                ShowMyMessage("1072,L/B database connect error (L/B database 連線失敗)", "L/B database connect error, need to check the connect first.");
                            }
                            else if(i==73)
                            {
                                ShowMyMessage("1073,temp & soak database select error (temp & soak database 搜尋失敗)", "temp & soak database select error, need to confirm with PD whether it be build up, please!!(建檔)");
                            }
                            else if(i==74)
                            {
                                ShowMyMessage("1074,L/B database select error (L/B database 搜尋失敗)", "L/B database select error, need to confirm with PMR whether it be build up, please!!(建檔)");
                            }
                            else
                            {
                                ShowMyMessage("Error Code : "+slErrorCode->Strings[0],"");
                            }
                        }
                        else
                        {
                            bFlag=true;
                        }
                    }
                    else
                    {
                        bFlag=true;
                    }
                }
                else
                {
                    bFlag=true;
                }
                slErrorCode->Clear();                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
                delete slErrorCode;
                if(bFlag)
                {
                    ShowMyMessage("Error code.TXT format error (error code.TXT格式錯誤)", "1036,Error code.TXT format error, call IT to check, please!!");
                    return;
                }
            }

            asBuffer.sprintf("%sSPIL.txt",DataPath);
            if(FileExists(asBuffer))
                DeleteFile(asBuffer);

            for(i=0; i<FileListBox1->Items->Count; i++)
            {
                if(FileExists(FileListBox1->Items->Strings[i]))
                  DeleteFile(FileListBox1->Items->Strings[i]);

                if(FileExists(FileListBox1->Items->Strings[i]))
                    bFlag=true;
            }

            if(bFlag==true)
            {
                ShowMyMessage("Recipe folder data delete fail","1030,所有於Recipe folder之檔案刪除失敗-Package recipe & .TXT");
                return;
            }
        }
        else
        {
            if(sCustomName=="")
            {
                ShowMyMessage("Device Name can't Empty!", "Device Name不可以空白");
                return;
            }

            if(CUSTOMER_CODE==CC_SCK)                                           //Steven 20180313 (Jou) : (Jou) : Add password for RMS upload
            {
                AnsiString sName    =CheckAndReadIniDataGeneral("RMS_Password", "Name", AnsiString("RMSUPLOAD"));
                AnsiString sPassword=CheckAndReadIniDataGeneral("RMS_Password", "Password", AnsiString("RMSUPLOAD"));
                fPassword->edUserName->Text="";                                 // name
                fPassword->edPassword->Text="";                                 // password
                fPassword->ShowModal();
                if(fPassword->edUserName->Text.UpperCase()!=sName.UpperCase() ||
                   fPassword->edPassword->Text.UpperCase()!=sPassword.UpperCase())
                {
                    return;
                }
            }

            CutTempToEdit(sCustomName);

            ReadRmsPath();
            AnsiString asSourcePath, str1, str2;
            if(IniConfig.iN05_UpDLMethod==eByFTP)
            {
                FileInfo().EnsureDirectoriesExist(asProductionLogPath);         //Jimmychiu 20250430 : 自動建立路徑資料夾並產生Barcode.ini檔案
                asSourcePath=asProductionLogPath+"\\Barcode.ini";
            }
            else
            {
                FileInfo().EnsureDirectoriesExist(IniConfig.sRmsPath);          //Jimmychiu 20250430 : 自動建立路徑資料夾並產生Barcode.ini檔案
                asSourcePath=IniConfig.sRmsPath+"\\Barcode.ini";
            }

            if(FileExists(asSourcePath)==false)
            {
                TFileStream *fileStream = new TFileStream(asSourcePath, fmCreate);
                delete fileStream;
            }

            if(FileExists(asSourcePath))
            {
                WriteIniData(asSourcePath, "Barcode", sCustomName, sSetupFileName);
            }
            else
            {
                ShowErrorMessage("WAR1681", 0, MMSystem, 0, asSourcePath);      //File Read error
                return;
            }
            UploadToServer(sSetupFileName, false);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfLotInfo::cbPATModeChange(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_PANTHER)
    {
        fMain->patFunc->sPATMode=cbPATMode->Text;
        fMain->patFunc->iPATModeInd=cbPATMode->ItemIndex;
        cbRunMode->Text=cbPATMode->Text;
        UpdatePATSubMode(cbPATMode->Text);
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::UpdateLotInfoPAT()
{
    PAT_Function* patFunc=fMain->patFunc;
    lnLotNumber->Text   =patFunc->patSetup.cLotNo;
    lnPartNumber->Text  =patFunc->patSetup.deviceNo;
    lnTestProgram->Text =patFunc->patSetup.programName;
    lnOpName->Text      =patFunc->patSetup.employeeNo;
    lnEntityNo->Text    =patFunc->patSetup.entityNo;
    lnLoadBoard->Text   =patFunc->patSetup.accessoryNo1;
    lnStartTime->Text   =patFunc->GetStartLotTimeStr();
    lnEndTime->Text     =patFunc->GetEndLotTimeStr(patFunc->REPORT_END_LOT);
    //synchronous
    if(cbPATMode->Text=="Re-Test" || cbPATMode->Text=="EQC")
    {
        patFunc->sPATMode   =cbPATSubMode->Text;
    }
    else
    {
        patFunc->sPATMode   =cbPATMode->Text;
    }
    edtSysLotID->Text   =lnLotNumber->Text;
    edtSysOperatorID->Text   =lnOpName->Text;
}
//---------------------------------------------------------------------------
void TfLotInfo::VisibleUploadBtnPAT(bool bVisible)
{
    sbUploadPAT->Down=!bVisible;
    sbUploadPAT->Visible=bVisible;
}
//---------------------------------------------------------------------------
void TfLotInfo::UpdatePATSubMode(const AnsiString& sModeName)
{
    cbPATSubMode->Clear();
    if(sModeName=="Re-Test")
    {
        cbPATSubMode->Visible=true;
        cbPATSubMode->Items->Add("Re-Test");
        for(int i=1; i<=10; i++)
        {
            cbPATSubMode->Items->Add(AnsiString().sprintf("RT%d",i));
        }
        cbPATSubMode->ItemIndex=0;
    }
    else if(sModeName=="EQC")
    {
        cbPATSubMode->Visible=true;
        cbPATSubMode->Items->Add("EQC");
        for(int i=1; i<=6; i++)
        {
            cbPATSubMode->Items->Add(AnsiString().sprintf("EQCRT%d",i));
        }
        cbPATSubMode->ItemIndex=0;
    }
    else
    {
        cbPATSubMode->Visible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbUploadPATClick(TObject *Sender)
{
    PAT_Function* patFunc=fMain->patFunc;
    AnsiString sFileName=patFunc->GetJobName();
    AnsiString sJobFullFileName=FileInfo().PathCombin(patFunc->GetPAT_Folder(),AnsiString("Job"));
    FileInfo().EnsureDirectoriesExist(sJobFullFileName);
    sJobFullFileName=FileInfo().PathCombin(sJobFullFileName,sFileName);
    if(patFunc->SaveJobINI(sJobFullFileName)==true)
    {
        AnsiString sJobUploadPath=FileInfo().PathCombin(IniConfig.sB13PATJobUploadPath,patFunc->GetJobName());
        if(FileInfo().DirectoryExist(IniConfig.sB13PATJobUploadPath)==true)
        {
            CopyFile(sJobFullFileName.c_str(), sJobUploadPath.c_str(), false);
            ShowMyMessage("Job Upload Success!");
        }
        else
        {
            AnsiString sErrMsg=AnsiString().sprintf("Job file upload path does not exist.%s Path=%s",FileInfo().GetNewLine(),sJobUploadPath);
            ShowMyMessage(sErrMsg);
            return;
        }
    }
    else
    {
        AnsiString sErrMsg=AnsiString().sprintf("Job file temp save path does not exist.%s Path=%s",FileInfo().GetNewLine(),sJobFullFileName);
        ShowMyMessage(sErrMsg);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::sbLotResetClick(TObject *Sender)
{
    ClearLotInfoPAT();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::leRunCardNumberMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    leRunCardNumber->Text="";
}
//---------------------------------------------------------------------------
void TfLotInfo::ClearLotInfoPAT()
{
    fMain->patFunc->DoIniDatas();
    UpdateLotInfoPAT();
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnRealTimeClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_PANTHER)
    {
        fMain->patFunc->GenerateRealTimeReport();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnpatHourlyClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_PANTHER)
    {
        fMain->patFunc->GenerateHourlyReport();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnpatEndLotClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_PANTHER)
    {
        fMain->patFunc->GenerateEndLotReport();
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::RefreshAMR()                                                    //Sam 20240304 : 新增 AMR 功能
{
    cbA60_1->Checked=IniConfig.bA60EnableAMR;

    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return;

    pnlWaitTXSetLoader->Caption=IntToStr(IniConfig.iA60NotifyQty[0]);
    pnlWaitRXSetAuto1->Caption=IntToStr(IniConfig.iA60NotifyQty[3]);
    pnlWaitRXSetAuto2->Caption=IntToStr(IniConfig.iA60NotifyQty[4]);
    pnlWaitRXSetAuto3->Caption=IntToStr(IniConfig.iA60NotifyQty[5]);

    pnlWaitTXTotalLoader->Caption=IntToStr(LastSet.iAMRTrayLoaderTotal);
    pnlWaitTXCntLoader->Caption=IntToStr(LastSet.iAMRTrayConut[0]);
    pnlWaitRXCntAuto1->Caption=IntToStr(LastSet.iAMRTrayConut[3]);
    pnlWaitRXCntAuto2->Caption=IntToStr(LastSet.iAMRTrayConut[4]);
    pnlWaitRXCntAuto3->Caption=IntToStr(LastSet.iAMRTrayConut[5]);

    aldWaitTXLoader->Value=AMR.CheckLoaderCount();
    //aldWaitTXLoader->Value=LastSet.bAMRRequestSupplyTray;

    aldWaitRXAuto1->Value=AMR.CheckUnloaderCount(0);
    aldWaitRXAuto2->Value=AMR.CheckUnloaderCount(1);
    aldWaitRXAuto3->Value=AMR.CheckUnloaderCount(2);

    aldWaitTrayFeed->Value=LastSet.bAMRTrayFeedWait;
    aldLoaderLast->Value=LastSet.bAMRLoaderLast;
    ShowAMRCategoryBin();
}
//---------------------------------------------------------------------------
void TfLotInfo::ShowAMRCategoryBin(bool bInitial)
{
    if(bInitial)
    {
        StrGrdCategory->Cells[1][0]="Count";
        StrGrdCategory->Cells[2][0]="Percent";
        StrGrdCategory->RowCount=iTestBinCount+2;
        for(int i=0; i<iTestBinCount; i++)
            StrGrdCategory->Cells[0][1+i]="Category "+AnsiString(i);

        StrGrdCategory->Cells[0][iTestBinCount+1]="Error Bin";
        return;
    }

    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return;

    int sum=0;
    double f=0.0;
    for(int i=0; i<iTestBinCount; i++)
        sum+=LastSet.iBinData32[2][i];

    for(int i=0; i<iTestBinCount+1; i++)
    {
        StrGrdCategory->Cells[1][1+i]=LastSet.iBinData32[2][i];
        if(sum>0)
        {
            f=(double)LastSet.iBinData32[2][i]*100.0/(double)sum;
            StrGrdCategory->Cells[2][1+i]=(AnsiString)GetFloatFormatString(f, 5, 2)+(AnsiString)("%");
        }
        else
        {
            StrGrdCategory->Cells[2][1+i]="0.00%";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnManualI49Click(TObject *Sender)
{
    if(SystemStart)
        return;

    int ret=0;
    bResetNotMsg=true;
    ret=ShowMyMessageBox_YES_NO("Do you want to enable off-line clean unit mode.", "你確定要執行離線清料作業。");
    if(ret==1)                                                                  //Sam 20240701 : Offline Clean out ic 新增權限設定
    {
        fMain->cbUserSelectChange(NULL);
        if(fSecurity->Insufficient(167)==false)
            ret=0;
    }

    if(ret==1)
    {
        if(DeviceForm.ContactMode==DirectContactMode || DeviceForm.ContactMode==DropContact)
        {
            TrayForm.bAutoFeed=true;
            if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)
            {
                LastSet.bBreakSCKART=true;
                TestIF_File.bSCKART_LotDeviceCheck=false;
            }

            bRestModeBackupParm=true;                                           //Sam 20250820 : [I49] 清料時 Contact Heigh 要拉高
            AnsiString S="";
            AnsiString szDir="";
            S=GetLastOpenFN();
            szDir=DataPath+S;
            szDir+="\\Contact.Data";
            iRestModeBackContactMode=ReadIniData(szDir, "Mode", "Contact", 0);
            dRestModeBackContactHeigh1=ReadIniData(szDir, "Test Arm1", "Contact", 0.0);
            dRestModeBackContactHeigh2=ReadIniData(szDir, "Test Arm2", "Contact", 0.0);
            WriteIniData(szDir, "Mode", "Contact",    DirectContactMode);
            WriteIniData(szDir, "Test Arm1", "Contact", dRestModeBackContactHeigh1+IniConfig.fI49_ChangeAboveSocket);
            WriteIniData(szDir, "Test Arm2", "Contact", dRestModeBackContactHeigh2+IniConfig.fI49_ChangeAboveSocket);
            fContact->ReadFile();
        }
        fMain->Reset("Reset by btnManualI49");
    }
    bResetNotMsg=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbRunModeDropDown(TObject *Sender)                   //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
{
    if(CUSTOMER_CODE==CC_JCET && fBarCode->JCETUseMakeWhite2DIDList()==true)
    {
        AnsiString sRunMode="";
        bool bFlag=false;
        sRunMode=InputBarcodeNumber("Input Run Mode", "RunMode");
        for(int i=0; i<cbRunMode->Items->Count; i++)                            //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
        {
            if(cbRunMode->Items->Strings[i]==sRunMode)
            {
                cbRunMode->Text=sRunMode;
                cbRunMode->ItemIndex=i;
                bFlag=true;
                break;
            }
        }

        if(bFlag==false)
        {
            cbRunMode->ItemIndex=-1;
        }
    }
}
//---------------------------------------------------------------------------
void TfLotInfo::JCETWhite2DIDShow(bool bUse)                                    //RogerYang 20251215 : JCET 2D FT1白名單/FT2比對功能
{
    AnsiString sPath=AuthPath+"config.ini";
    AnsiString sTmp;
    cbRunMode->Items->Clear();
    if(bUse==true)
    {
        sTmp=ReadIniData(sPath, "Lot Info", "Run Mode 2DID",       AnsiString(""));
        cbRunMode->Items->Add("FT1");
        cbRunMode->Items->Add("FT2");
        cbRunMode->Items->Add("FT3");
        cbRunMode->Items->Add("FT4");
        cbRunMode->Items->Add("FT5");
        cbRunMode->Items->Add("FT6");
        cbRunMode->Items->Add("FT7");
        cbRunMode->Items->Add("FT8");
        cbRunMode->Items->Add("FT9");
        cbRunMode->Text="";
    }
    else
    {
        sTmp=ReadIniData(sPath, "Lot Info", "Run Mode",       AnsiString(""));
        cbRunMode->Items->Add("Normal");
        cbRunMode->Items->Add("RT");
        cbRunMode->Items->Add("EQC");
        cbRunMode->Text="Normal";
        cbRunMode->ItemIndex=0;
    }
    for(int i=0; i<cbRunMode->Items->Count; i++)                                //RogerYang 20251208 : JCET 2D FT1白名單/FT2比對功能
    {
        if(cbRunMode->Items->Strings[i]==sTmp)
        {
            cbRunMode->Text=sTmp;
            cbRunMode->ItemIndex=i;
            break;
        }
    }
    labCusLotID->Visible=bUse;
    edtCusLotID->Visible=bUse;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbFirstTrayCheckOnUnloaderClick(TObject *Sender)
{
    if(bP60UserClicked==true)
    {
        bP60UserClicked=false;
        if(cbFirstTrayCheckOnUnloader->Checked==false)
        {
            if(AccessLevel<iDefEngineerLevel)
            {
                cbFirstTrayCheckOnUnloader->Checked=true;
            }
        }
        else
        {
            if(AccessLevel<iDefEngineerLevel)
            {
                cbFirstTrayCheckOnUnloader->Checked=false;
            }
            else if(IniConfig.bP62FirstTrayCheckOnUnloader==true)
            {
                IniConfig.bP62Auto1=(Prod.iIsFailT6[eAuto1]==0);
                IniConfig.bP62Auto2=(Prod.iIsFailT6[eAuto2]==0);
                IniConfig.bP62Auto3=(Prod.iIsFailT6[eAuto3]==0);
                cb1stCheck_Auto1->Checked=IniConfig.bP62Auto1;
                cb1stCheck_Auto2->Checked=IniConfig.bP62Auto2;
                cb1stCheck_Auto3->Checked=IniConfig.bP62Auto3;
            }
            else
            {
                cbFirstTrayCheckOnUnloader->Checked=false;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::cbFirstTrayCheckOnUnloaderMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    bP60UserClicked=true;
}
//---------------------------------------------------------------------------
void TfLotInfo::SetFirstTrayCheckOnUnloader()                                   //Jimmychiu 20251205 : First Tray Check On Unloader
{
    cbFirstTrayCheckOnUnloader->Checked=true;
    IniConfig.bP62Auto1=(Prod.iIsFailT6[eAuto1]==0);
    IniConfig.bP62Auto2=(Prod.iIsFailT6[eAuto2]==0);
    IniConfig.bP62Auto3=(Prod.iIsFailT6[eAuto3]==0);
    cb1stCheck_Auto1->Checked=IniConfig.bP62Auto1;
    cb1stCheck_Auto2->Checked=IniConfig.bP62Auto2;
    cb1stCheck_Auto3->Checked=IniConfig.bP62Auto3;
}
//---------------------------------------------------------------------------
void  TfLotInfo::RefreshOtherTool()                                             //Jimmychiu 20251219 : Refresh PTI funciton
{
    static bool bTimerRunning=false;
    if((iTestRunMode==FT && TestIF_File.bContinuousLoader==false) ||
       (iTestRunMode==RT && TestIF_File.bContinuousLoader_RT==false))
    {
        return;
    }

    if(bTimerRunning)
        return;
    bTimerRunning=true;
    //
    if(iTestRunMode==FT)
    {
        edLoaderCountNow->Text=IntToStr(LastSet.SendCT[2]);
        edLoaderCountAlarm->Text=IntToStr(TestIF_File.iContinuousLoaderCount);
    }
    else
    {
        edLoaderCountNow->Text=IntToStr(LastSet.SendCT[2]);
        edLoaderCountAlarm->Text=IntToStr(TestIF_File.iContinuousLoaderCount_RT);
    }
    //
    bTimerRunning=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnFTPDownLoadbyDeviceIDClick(TObject *Sender)       //RogerYang 20260127 : Add For JSCC_OS download by Device list
{
    btnFtpServer->Click();
}
//---------------------------------------------------------------------------

void __fastcall TfLotInfo::rgUnloaderClick(TObject *Sender)                     //RogerYang 20260128 : 田揚志畫面修改需求
{
    fTesterTCP->rgUnloader->ItemIndex=rgUnloader->ItemIndex;
}
//---------------------------------------------------------------------------
void TfLotInfo::ASECL_LotEnd(bool bManual)
{
    AnsiString aStr, S, sInput, sIdleTime, sProdTime, sDownTime;
    int iTotal=0, iPass, iFail, ret;
    AnsiString sYield;

    if(bManual==true)
    {
        ret=ShowMyMessageBox_YES_NO("Sure to lot end and upload summary?", "確定要Lot end上傳summary?");
        if(ret==2)
        {
            return;
        }
    }

    aStr.sprintf("LOT END WORKDONE");
    RecordProcess(aStr);
    SaveEventLogInfo("200000000", aStr, 11, " ");
    fObserver->WriteCategoryData();
    iTotal=TastCategory.iTotalSocket;
    iPass=TastCategory.iPassSocket;
    iFail=TastCategory.iTotalSocket-iPass;
    if(TastCategory.iTotalSocket!=0)
        sYield.sprintf("%5.2f%%", double(iPass)/double(iTotal)*100);
    else
        sYield="0.0";

    sInput.sprintf("INPUT:%d - PASS:%d - FAIL:%d - RATE:%s", iTotal, iPass, iFail, sYield);
    sIdleTime.sprintf("IDLE TIME : %s",         ConvertMSecToSPC(LastSet.SystemAccSecond[0][stPauseTime]));
    sProdTime.sprintf("PRODUCTION TIME : %s",   ConvertMSecToSPC(LastSet.SystemAccSecond[0][stProductTime]));
    sDownTime.sprintf("DOWN TIME : %s",         ConvertMSecToSPC(LastSet.SystemAccSecond[0][stJamTime]));

    SaveEventLogInfo("200000000", sInput, 12, AnsiString(iTotal));
    SaveEventLogInfo("200000000", sInput, 13, AnsiString(iPass));
    SaveEventLogInfo("200000000", sInput, 14, AnsiString(iFail));
    SaveEventLogInfo("200000000", sInput, 15, sYield);
    SaveEventLogInfo("200000000", sIdleTime, 16, ConvertMSecToSPC(LastSet.SystemAccSecond[0][stPauseTime]));
    SaveEventLogInfo("200000000", sProdTime, 17, ConvertMSecToSPC(LastSet.SystemAccSecond[0][stProductTime]));
    SaveEventLogInfo("200000000", sDownTime, 18, ConvertMSecToSPC(LastSet.SystemAccSecond[0][stJamTime]));
    SetLotID(edtASECL_LotID->Text);                                             //JerryYang 20220215 : ASE-CL lot info需求
    edtASECL_OPID->Enabled      =true;
    edtASECL_LotID->Enabled     =true;
    edtASECL_TesterID->Enabled  =true;
    cbbASECL_LoginMode->Enabled =true;
    edInsertion->Enabled=true;
    edCustomerDevice->Enabled=true;
    edFlowID->Enabled=true;
    cbRunModeASECL->Enabled =true;
    SaveASECLTesterHardwareLog();

    fSCKART->SaveTestSummary(true);                                             //JerryYang 20240423 : add
    if(IniConfig.bN23UploadJHT_Log && TestIF_File.bEnableTrayID2==true)
    {
        fFTPClient->N22_Upload_JHT_LOG();
    }

    if(IniConfig.bN35UploadJHT_Log && TestIF_File.bEnableTrayID2==true)
    {
        fFTPClient->N35_Upload_JHT_LOG();
    }

    edtASECL_LotID->Text="";
    edtASECL_OPID->Text="";
    edtASECL_TesterID->Text="";
    edtASECL_LoadBoard->Text="";
    edInsertion->Text="";
    edCustomerDevice->Text="";
    edFlowID->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnASECL_LotEndClick(TObject *Sender)
{
    ASECL_LotEnd(true);
}
//---------------------------------------------------------------------------
//==> Eastsun 20260515 F009 KYEC AMR tsKYEC_AMR handlers
void __fastcall TfLotInfo::btnAMRSupplementClick(TObject *Sender)
{
    asSupplementBin=edAMREventParam->Text;
    fLotInfo->labNowLoaderTrayID->Caption=edAMRLoaderTrayID->Text;
    fLotInfo->labNowAuto1TrayID_KYEC->Caption=edAMRAuto1TrayID->Text;
    fLotInfo->labNowAuto2TrayID_KYEC->Caption=edAMRAuto2TrayID->Text;
    fLotInfo->labNowAuto3TrayID_KYEC->Caption=edAMRAuto3TrayID->Text;
    EventReport(SECS_EVENT.AGVSupplement);
    asSupplementBin="";
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnAMRLDUnLDCheckClick(TObject *Sender)
{
    asLDUnLDCheckAGV=edAMREventParam->Text;
    EventReport(SECS_EVENT.AGVLDUnLDStatus);
    asLDUnLDCheckAGV="";
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnAMRLDUnLDFinishClick(TObject *Sender)
{
    asLDUnLDFinsihAGV=edAMREventParam->Text;
    EventReport(SECS_EVENT.AGVLDUnLDFinish);
    asLDUnLDFinsihAGV="";
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::labNowTrayCountClick(TObject *Sender)
{
    int i = 0;
    labNowTrayCount->Caption=i;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnAMRClearCountClick(TObject *Sender)
{
    int ret;
    ret=ShowMyMessageBox_YES_NO("Clear Tray Count?", "");
    if(ret==1)
    {
        LastSet.iLoaderTotalTray=0;
        LastSet.iLoaderTrayCount_ART=0;
        labNowTrayCount->Caption=0;
        labLotTrayCount_KYEC->Caption=0;
        LabDiffTrayCount->Caption=0;

        labLoaderTrayCount->Caption=0;
        labAuto1TrayCount_KYEC->Caption=0;
        labAuto2TrayCount_KYEC->Caption=0;
        labAuto3TrayCount_KYEC->Caption=0;

        TestIF_File.iAMRLDNowTrayCount=0;
        TestIF_File.iAMRLDSECSTrayCount=0;
        iLoaderTrayCountCal=0;
        iLoaderTrayCountAMRCal=0;

        for(int i=0; i<14; i++)
        {
            TestIF_File.iAMRMagzineDeviceCount[i]=0;
        }

        bAMRReceiveLoaderTotalTray=false;
        bAMRReceiveAGVStart=false;
        bAMRReceiveStart=false;
        iUnloaderTrayCountCal[0]=0;                                             //Eastsun 20260710整合
        iUnloaderTrayCountCal[1]=0;                                             //Eastsun 20260710整合
        iUnloaderTrayCountCal[2]=0;                                             //Eastsun 20260710整合
        bNeedCoverTray[0]=false;                                                //Eastsun 20260710整合
        bNeedCoverTray[1]=false;                                                //Eastsun 20260710整合
        bNeedCoverTray[2]=false;                                                //Eastsun 20260710整合
        bReflashTrayCount=false;                                                //Eastsun 20260710整合
        RecordProcess("AMR Button Clear ALL Par");
        fLotInfo->CheckActionFlag();
        LotSummary.CleariTotalCategory(false);                                   //Eastsun 20260710整合
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::btnAMRSetSECSClick(TObject *Sender)
{
    TestIF_File.iAMRTrayCount[3]=atoi(edAMRTrayCount->Text.c_str());
    TestIF_File.iAMRDeviceCount[3]=atoi(edAMRDeviceCount->Text.c_str());
    TestIF_File.asAMRBinSetting[0]=edAMRBinSetting->Text;
}
//---------------------------------------------------------------------------
void TfLotInfo::bClearAMRPar()                                                  //Eastsun 20260710整合
{
    LastSet.iLoaderTotalTray=0;
    LastSet.iLoaderTrayCount_ART=0;
    labNowTrayCount->Caption=0;
    labLotTrayCount_KYEC->Caption=0;
    LabDiffTrayCount->Caption=0;

    labLoaderTrayCount->Caption=0;
    labAuto1TrayCount_KYEC->Caption=0;
    labAuto2TrayCount_KYEC->Caption=0;
    labAuto3TrayCount_KYEC->Caption=0;

    TestIF_File.iAMRLDNowTrayCount=0;
    TestIF_File.iAMRLDSECSTrayCount=0;
    iLoaderTrayCountCal=0;
    iLoaderTrayCountAMRCal=0;

    for(int i=0; i<14; i++)
    {
        TestIF_File.iAMRMagzineDeviceCount[i]=0;
    }
    iUnloaderTrayCountCal[0]=0;
    iUnloaderTrayCountCal[1]=0;
    iUnloaderTrayCountCal[2]=0;
    bAMRReceiveLoaderTotalTray=false;
    bAMRReceiveAGVStart=false;
    bAMRReceiveStart=false;
    bNeedCoverTray[0]=false;
    bNeedCoverTray[1]=false;
    bNeedCoverTray[2]=false;
    bReflashTrayCount=false;
    RecordProcess("AMR Button Clear ALL Par");
    fLotInfo->CheckActionFlag();
    LotSummary.CleariTotalCategory(false);
}
//---------------------------------------------------------------------------
void TfLotInfo::CheckActionFlag()
{
    ledLoader->Value=bLoaderActionFlag[0];
    ledEmpty->Value=bLoaderActionFlag[1];
    ledColor->Value=bLoaderActionFlag[2];
    ledAuto1->Value=bUnLoaderActionFlag[0];
    ledAuto2->Value=bUnLoaderActionFlag[1];
    ledAuto3->Value=bUnLoaderActionFlag[2];
    ledStartAGV->Value=bAMRReceiveAGVStart;
    ledSTART->Value=bAMRReceiveStart;
    ledLoaderTotalTray->Value=bAMRReceiveLoaderTotalTray;
    ledLOT_START->Value=RunInfo.bLotStart;
}
//<== Eastsun 20260515 F009 KYEC AMR tsKYEC_AMR handlers

//==> Eastsun 20260515 F011 (AMR InitialUnLoaderTask impl)
void __fastcall TfLotInfo::InitialUnLoaderTask(int iPos)
{
    iUnloaderTask[iPos]=1;
}
//<== Eastsun 20260515 F011
//==> Eastsun 20260515 F020 (AMR LoaderAction/UnLoaderAction/CheckLDLevel/CheckAMRAction/ReflashInfo/Timer4Timer)
int iLoaderTask[3]={1,1,1};
int iloaderLevelTask[3]={1,1,1};
//--------------------------------------------------------------------------
void TfLotInfo::InitialLoaderTask(int iPos)
{
    iLoaderTask[iPos]=1;
}
//--------------------------------------------------------------------------
void TfLotInfo::InitialLDLevelTask(int iPos)
{
    iloaderLevelTask[iPos]=1;
}
//--------------------------------------------------------------------------
TQPF_Timer LoadActionDelay[3];
void TfLotInfo::LoaderAction(int iAuto)
{
    int &Task=iLoaderTask[iAuto];
    int C_MiddleIndex[3]={C_Load_Middle,C_Empty_Middle,C_Color_Middle};
    int SenIndex[3]={SnLoaderTrayHasTray_AGV,SnEmptyTrayHasTray_AGV,SnColorTrayHasTray_AGV};
    AnsiString str[3];
    int iTrayCount;  //iCoverTray removed
    AnsiString strs="";

    switch(Task)
    {
        case 1:
            if(bLoaderSECSActionFlag[iAuto]==true)
            {
                Task=100;
                RecordProcess("AMR LoaderAction");
            }
            break;
        case 100:
            if(Cylinder[C_MiddleIndex[iAuto]].OffStatus()==true)
            {
                bLoaderLockActionFlag[iAuto]=true;
                LoadActionDelay[iAuto].SetSecAndOn(300);
                str[0]="P1:1,P2:0,P3:0,P4:0,P5:0,P6:0";
                str[1]="P1:0,P2:1,P3:0,P4:0,P5:0,P6:0";
                str[2]="P1:0,P2:0,P3:1,P4:0,P5:0,P6:0";

                asLDUnLDCheckAGV=str[iAuto];
                EventReport(SECS_EVENT.AGVLDUnLDStatus);
                asLDUnLDCheckAGV="";
                Task=200;
            }
            break;
        case 200:
            if(Sen[SenIndex[iAuto]].IsOn()==true)
            {
                str[0]="P1:1,P2:0,P3:0,P4:0,P5:0,P6:0";
                str[1]="P1:0,P2:1,P3:0,P4:0,P5:0,P6:0";
                str[2]="P1:0,P2:0,P3:1,P4:0,P5:0,P6:0";

                asLDUnLDFinsihAGV=str[iAuto];
                EventReport(SECS_EVENT.AGVLDUnLDFinish);
                asLDUnLDFinsihAGV="";
                if(iAuto==0)
                Task=300;
                else
                    Task=400;
            }
            else if(LoadActionDelay[iAuto].Off())
            {
                //Error
            }
            break;
        case 300:
            if(atoi(fLotInfo->labLotTrayCount_KYEC->Caption.c_str())!=0)
            {
                TestIF_File.iAMRLDSECSTrayCount=iSECSSetTrayCount+iAMRCoverTray;
                fLotInfo->labNowTrayCount->Caption=TestIF_File.iAMRLDSECSTrayCount;

                iTrayCount=atoi(fLotInfo->LabDiffTrayCount->Caption.c_str());
                bReflashTrayCount=false;

                if(iTrayCount!=0)
                {
                    fLotInfo->LabDiffTrayCount->Caption=iTrayCount-TestIF_File.iAMRLDSECSTrayCount+iAMRCoverTray;
                    strs.sprintf("iSECSSetTrayCount = %d", iSECSSetTrayCount);
                    RecordProcess(strs,"AMR LoaderAction iTrayCount");
                }
                else
                {
                    bReflashTrayCount=true;
                    strs.sprintf("iSECSSetTrayCount = %d", iSECSSetTrayCount);
                    RecordProcess(strs,"AMR LoaderAction iTrayCount0");
                }
            }
            else
            {
                bReflashTrayCount=true;
                TestIF_File.iAMRLDSECSTrayCount=iSECSSetTrayCount+iAMRCoverTray;
                strs.sprintf("iSECSSetTrayCount = %d", iSECSSetTrayCount);
                RecordProcess(strs,"AMR First LoaderAction");
            }
            bAMRReceiveAGVStart=true;
            fLotInfo->CheckActionFlag();
            Task=400;
            break;
        case 400:
            bLoaderActionFlag[iAuto]=false;
            bLoaderSECSActionFlag[iAuto]=false;
            bLoaderLockActionFlag[iAuto]=false;
            Task=1;
            break;
    }
}
//--------------------------------------------------------------------------
TQPF_Timer UnLoadActionDelay[3];
void TfLotInfo::UnLoaderAction(int iAuto)
{
    int &Task=iUnloaderTask[iAuto];
    int SenIndex[3]={SnAuto1TrayHasTray,SnAuto2TrayHasTray,SnAuto3TrayHasTray};
    int C_MiddleIndex[3]={C_Auto1_Selector,C_Auto2_Selector,C_Auto3_Selector};
    AnsiString str[3];

    switch(Task)
    {
        case 1:
            if(bUnLoaderSECSActionFlag[iAuto]==true)
            {
                Task=100;
            }
            break;
        case 100:
            #ifndef SOFT_SIMULTE
            if(Cylinder[C_MiddleIndex[iAuto]].OffStatus()==true)
            #endif
            {
                bUnLoaderLockActionFlag[iAuto]=true;
                UnLoadActionDelay[iAuto].SetSecAndOn(300);

                str[0]="P1:0,P2:0,P3:0,P4:1,P5:0,P6:0";
                str[1]="P1:0,P2:0,P3:0,P4:0,P5:1,P6:0";
                str[2]="P1:0,P2:0,P3:0,P4:0,P5:0,P6:1";

                asLDUnLDCheckAGV=str[iAuto];
                EventReport(SECS_EVENT.AGVLDUnLDStatus);
                asLDUnLDCheckAGV="";
                Task=200;
            }
            break;
        case 200:
            #ifndef SOFT_SIMULTE
            if(Sen[SenIndex[iAuto]].IsOn()==false)
            #endif
            {
                str[0]="P1:0,P2:0,P3:0,P4:1,P5:0,P6:0";
                str[1]="P1:0,P2:0,P3:0,P4:0,P5:1,P6:0";
                str[2]="P1:0,P2:0,P3:0,P4:0,P5:0,P6:1";

                asLDUnLDFinsihAGV=str[iAuto];
                EventReport(SECS_EVENT.AGVLDUnLDFinish);
                asLDUnLDFinsihAGV="";
                Task=300;
            }
            #ifndef SOFT_SIMULTE
            else if(UnLoadActionDelay[iAuto].Off())
            {
                //Error
            }
            #endif
            break;
        case 300:
            bUnLoaderActionFlag[iAuto]=false;
            bUnLoaderSECSActionFlag[iAuto]=false;
            bUnLoaderLockActionFlag[iAuto]=false;
            Task=1;
            break;
    }
}
//--------------------------------------------------------------------------
void TfLotInfo::CheckLDLevel(int iAuto)
{
    int &Task=iloaderLevelTask[iAuto];
    int SenIndex[3]={SnLoaderTrayHasTray_AGV,SnEmptyTrayHasTray_AGV,SnColorTrayHasTray_AGV};
    int iTrayCount=atoi(fLotInfo->LabDiffTrayCount->Caption.c_str());
    AnsiString str[3];

    switch(Task)
    {
        case 1:
            if(Sen[SenIndex[iAuto]].IsOff()==true)
            {
                str[0]="P1:1,P2:0,P3:0,P4:0,P5:0,P6:0";
                str[1]="P1:0,P2:1,P3:0,P4:0,P5:0,P6:0";
                str[2]="P1:0,P2:0,P3:1,P4:0,P5:0,P6:0";
                if(iAuto==0 && iTrayCount==0)
                {
                    break;
                }
                asSupplementBin=str[iAuto];
                EventReport(SECS_EVENT.AGVSupplement);

                bLoaderActionFlag[iAuto]=true;
                asSupplementBin="";
                Task=100;
            }
            break;
        case 100:
            if(Sen[SenIndex[iAuto]].IsOn()==true)
            {
                Task=1;
            }
            break;
    }
}
//--------------------------------------------------------------------------
void TfLotInfo::CheckAMRAction()
{
//  int SenIndex[3]={SnLoaderTrayHasTray_AGV,SnEmptyTrayHasTray_AGV,SnColorTrayHasTray_AGV};  //W8004 assigned but never used

    for(int i=0; i<3; i++)
    {
        LoaderAction(i);
        UnLoaderAction(i);

        if(bLoaderActionFlag[i]==false)
            CheckLDLevel(i);
    }
}
//--------------------------------------------------------------------------
void TfLotInfo::ReflashInfo()
{
    fLotInfo->labLoaderTrayCount->Caption=iLoaderTrayCountCal;
    fLotInfo->labAuto1TrayCount_KYEC->Caption=iUnloaderTrayCountCal[0];
    fLotInfo->labAuto2TrayCount_KYEC->Caption=iUnloaderTrayCountCal[1];
    fLotInfo->labAuto3TrayCount_KYEC->Caption=iUnloaderTrayCountCal[2];
}
//--------------------------------------------------------------------------
void __fastcall TfLotInfo::Timer4Timer(TObject *Sender)
{
    CheckActionFlag();
    CheckAMRAction();

    ReflashInfo();
}
//<== Eastsun 20260515 F020 (AMR functions block)
//------------------------------------------------------------------------------
bool __fastcall TfLotInfo::SetAQLMode(bool bSet)                                //Eastsun 20260520 整合
{
    int tag=0;
    bool bFlag=false;

    if(IniConfig.bI52_bAQLSortMode==true)
    {
        if(bSet==true)
        {
            if(HasICUnderMachine()==false && bStartAQLSortMode==false) //Ifor 20210513 add: 機台內無IC 未設定過
            {
                if(iAQLCount==0 || iAQLBin==-1)
                {
                    iAQLCount=0;
                    iAQLBin=-1;
                    fLotInfo->lbLotAQLSetCount->Caption=IntToStr(iAQLCount);
                    fLotInfo->lbLotAQLSetBin->Caption=IntToStr(iAQLBin);
                    bSetAQLSortMode=false;
                    RecordProcess("AQL Setting Fail");
                    return false;
                }
                else
                {
                    if(iTestRunMode==FT)    //tsNormal
                    {
                        tag=eBinFT;
                    }
                    else if(iTestRunMode==RT)
                    {
                        tag=eBinRT;
                    }
                    else if(iTestRunMode==OffT) //ChungHung 20141002 add for KYEC AutoRetest  //ChungHung 20111110 add
                    {
                        tag=eBinOffLine;
                    }
                    else if(iTestRunMode==FT_ART) //ChungHung 20141002 add for KYEC AutoRetest
                    {
                        tag=eBinFT_ART;
                    }
                    else if(iTestRunMode==RT_ART) //ChungHung 20141002 add for KYEC AutoRetest
                    {
                        tag=eBinRT_ART;
                    }
                    else if(iTestRunMode==FT_MRT) //Ifor 20170316 (wei) add MRT Mode
                    {
                        tag=eBinFT_MRT;
                    }
                    else if(iTestRunMode==RT_MRT) //Ifor 20170316 (wei) add MRT Mode
                    {
                        tag=eBinRT_MRT;
                    }

                    if(BinSelect[tag].iCatDataT3Pos[iAQLBin]==0)
                    {
                        iAQLCount=0;
                        iAQLBin=-1;
                        fLotInfo->lbLotAQLSetCount->Caption=IntToStr(iAQLCount);
                        fLotInfo->lbLotAQLSetBin->Caption=IntToStr(iAQLBin);
                        bSetAQLSortMode=false;
                        RecordProcess("AQL Setting Fail");
                        return false;
                    }
                    else
                    {
                        AnsiString str1="";
                        str1.sprintf("AQL Setting Count: %d", iAQLCount);
                        RecordProcess(str1);

                        str1.sprintf("AQL Setting Bin: %d", iAQLBin);
                        RecordProcess(str1);

                        fLotInfo->lbLotAQLSetCount->Caption=IntToStr(iAQLCount);
                        fLotInfo->lbLotAQLSetBin->Caption=IntToStr(iAQLBin);

                        bSetAQLSortMode=true;
                        iAQLBin=BinSelect[tag].iCatDataT3Pos[iAQLBin]-1;
                        bFlag=true;
                    }
                }
            }
            else
            {
                iAQLCount=0;
                iAQLBin=-1;
                fLotInfo->lbLotAQLSetCount->Caption=IntToStr(iAQLCount);
                fLotInfo->lbLotAQLSetBin->Caption=IntToStr(iAQLBin);
                bSetAQLSortMode=false;
                RecordProcess("AQL Setting Fail");
                return false;
            }
        }
        else
        {
            if(HasICUnderMachine()==false && bSetAQLSortMode==true)
            {
                bStartAQLSortMode=true;
                bSetAQLSortMode=false;  //Ifor 20210517 AQL Strat 後需清除避免異常發生
            }
        }
    }
    return bFlag;
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::ATCTransferFileTimeTimer(TObject *Sender)
{
    if(CosFunction.bUseATCFileTransfer==true)
    {
        if(iATC_RecipeFileTransfer==0)
        {
            ATCTransferFileTime->Enabled=false;
            fFTPClient->UploadFileToServer2(IniConfig.FtpUplaodPath, fFTPClient->edtHDWaferName->Text, true);

            fFTPClient->plUnload->Enabled=true;
            fFTPClient->edtHDWaferName->Text="";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLotInfo::edDeviceIDMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(IniConfig.bEnableRmsCheckSetupFile==true)
    {
        AnsiString sBarcodeID=InputBarcodeNumber("Input Device ID:");
        edDeviceID->Text=sBarcodeID;
    }
}
//---------------------------------------------------------------------------

