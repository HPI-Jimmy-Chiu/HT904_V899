//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "cTrayMapping.h"
#include "aoutarm.h"

#include "database.h"
#include "cpublic.h"
#include "common.h"
#include "cprod.h"
#include "main.h"
#include "handlerlog.h"
#include "cmydef.h"
#include "atester.h"
#include "note.h"
#include "MyMotor.h"
#include "cContact.h"
#include "mymessbox.h"
#include "ckernel.h"
#include "csystem.h"
#include "mysensor.h"
#include "SgdToXLS.h"
#include "myQwertyKeyBoard.h"
#include "InputForm.h"
#include "uLotInfo.h"
#include "ainarm2.h"
#include "OCR.h"
#include "uHGemHT9045.h"
#include "MachineType.h"
#include "acatchtray.h"
#include "cShowBinSelect.h"
#include "asendic.h"
#include "mycylin.h"
#include "TrayStepMotor.h"
#include "TfAOILaserScan.h"
#include "WinSocketErrorCode.h"
#include "cSortCT.h"
#include "AGV.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfTrayMapping *fTrayMapping;

int iCCDConntectionOkTask;
TQPF_Timer tCCDChangeSetupFile;
TQPF_Timer tNFTDelay;
const int DefaultLaserScanLoaderCWSpeed=33;
//---------------------------------------------------------------------------
__fastcall TfTrayMapping::TfTrayMapping(TComponent* Owner)
    : TForm(Owner)
{
    iNFCTask=1;
    bGetST=false;
    bGetLF=false;
    bGetGF=false;
    bGetCF=false;
    for(int i=0; i<iTotalFunction; i++)                                         //Sam 20190405 : Tray Decive Count
    {
        bEnableCCDTrayChannelConnect[i]=true;
        bStepError                  [i]=false;                                  //Steven 20160503 : 確認移動位置與拍照位置相同
        iCurrentStep                [i]=-1;                                     //Steven 20160503 : 確認移動位置與拍照位置相同
        SocketTrayReceiveList       [i]=new TStringList;
        SocketTrayReceiveList       [i]->Clear();
    }

    bEnableKeyenceChannelConnect[iLoaderID                    ]=true;
    bEnableKeyenceChannelConnect[iEmptyID                     ]=true;
    bEnableKeyenceChannelConnect[iMultileID                   ]=true;
    bEnableKeyenceChannelConnect[iAuto3ID                     ]=true;
    bEnableKeyenceChannelConnect[iKeyenceCoverTrayID_LoaderCar]=true;
    bEnableKeyenceChannelConnect[iKeyenceCoverTrayID_Loader   ]=true;
    bEnableKeyenceChannelConnect[iKeyenceCoverTrayID_AUTO1    ]=true;
    bEnableKeyenceChannelConnect[iKeyenceCoverTrayID_AUTO2    ]=true;
    bEnableKeyenceChannelConnect[iKeyenceCoverTrayID_AUTO3    ]=true;

    fShow=false;
    TimerProcessTrayData->Enabled=true;

    bUseTrayMap     =(CUSTOMER_CODE==CC_TSMC_TAINAN || USE_MR_SYSTEM==2);       //Load Tray Map
    bUseTrayID      =(CUSTOMER_CODE==CC_TSMC_TAINAN ||
                      CUSTOMER_CODE==CC_ASE_KaohSiung);                         //Load Tray Map
    bUseDeviceRemain=(USE_MR_SYSTEM==2 ||
                      USE_TRAY_MAPPING==etmDeviceRemain ||
                      CUSTOMER_CODE==CC_TERAPOWER ||
                      CUSTOMER_CODE==CC_SIGURD_HUKOU ||
                      CUSTOMER_CODE==CC_SIGURD_ChungXing ||
                      CUSTOMER_CODE==CC_HONPREC_QC ||
                      CUSTOMER_CODE==CC_Greatek);                               //Device Remain
    bUseDeviceCount =(USE_MR_SYSTEM==2);                                        //Sam 20200313 : UnloadTray Device Count 待整合

//    #ifdef SOFT_SIMULTE
//    bUseTrayMap=true;
//    bUseTrayID=true;
//    bUseDeviceRemain=true;
//    bUseDeviceCount=true;
//    #endif

    clntsckt_Tray[0]=ClientSocket_Tray_ID;
    clntsckt_Tray[1]=ClientSocket_Tray_Map;
    clntsckt_Tray[3]=ClientSocket_Tray_ID2;
    clntsckt_Tray[4]=ClientSocket_Tray_DeviceCnt1;
    clntsckt_Tray[5]=ClientSocket_Tray_DeviceCnt2;
    clntsckt_Tray[6]=ClientSocket_Tray_DeviceCnt3;

    edAddress[0]=edTray_ID_Address;
    edAddress[1]=edTray_Map_Address;
    edAddress[3]=edTray_ID2_Address;
    edAddress[4]=edTrayDeviceCnt1_Address;
    edAddress[5]=edTrayDeviceCnt2_Address;
    edAddress[6]=edTrayDeviceCnt3_Address;

    edPort[0]=edTray_ID_Port;
    edPort[1]=edTray_Map_Port;
    edPort[3]=edTray_ID2_Port;
    edPort[4]=edTrayDeviceCnt1_Port;
    edPort[5]=edTrayDeviceCnt2_Port;
    edPort[6]=edTrayDeviceCnt3_Port;

    clntsckt_Tray_Keyence[iLoaderID                     ]=ClientSocket_Keyence1;
    clntsckt_Tray_Keyence[iEmptyID                      ]=ClientSocket_Keyence2;
    clntsckt_Tray_Keyence[iMultileID                    ]=ClientSocket_Keyence3;
    clntsckt_Tray_Keyence[iAuto3ID                      ]=ClientSocket_Keyence4;
    clntsckt_Tray_Keyence[iKeyenceCoverTrayID_LoaderCar ]=ClientSocket_CoverTray_ID_LoaderCar;
    clntsckt_Tray_Keyence[iKeyenceCoverTrayID_Loader    ]=ClientSocket_CoverTray_ID_Loader;
    clntsckt_Tray_Keyence[iKeyenceCoverTrayID_AUTO1     ]=ClientSocket_CoverTray_ID_AUTO1Car;
    clntsckt_Tray_Keyence[iKeyenceCoverTrayID_AUTO2     ]=ClientSocket_CoverTray_ID_AUTO2Car;
    clntsckt_Tray_Keyence[iKeyenceCoverTrayID_AUTO3     ]=ClientSocket_CoverTray_ID_AUTO3Car;

    edAddress_Keyence[iLoaderID                         ]=edTray_ID_AddressKY;
    edAddress_Keyence[iEmptyID                          ]=edTray_ID2_AddressKY;
    edAddress_Keyence[iMultileID                        ]=edTray_ID3_AddressKY;
    edAddress_Keyence[iAuto3ID                          ]=edTray_ID4_AddressKY;
    edAddress_Keyence[iKeyenceCoverTrayID_LoaderCar     ]=edCoverTray_ID_AddressKY;
    edAddress_Keyence[iKeyenceCoverTrayID_Loader        ]=edCoverTray_ID2_AddressKY;
    edAddress_Keyence[iKeyenceCoverTrayID_AUTO1         ]=edCoverTray_ID3_AddressKY;
    edAddress_Keyence[iKeyenceCoverTrayID_AUTO2         ]=edCoverTray_ID4_AddressKY;
    edAddress_Keyence[iKeyenceCoverTrayID_AUTO3         ]=edCoverTray_ID5_AddressKY;

    edPort_Keyence[iLoaderID                            ]=edTray_ID_PortKY;
    edPort_Keyence[iEmptyID                             ]=edTray_ID2_PortKY;
    edPort_Keyence[iMultileID                           ]=edTray_ID3_PortKY;
    edPort_Keyence[iAuto3ID                             ]=edTray_ID4_PortKY;
    edPort_Keyence[iKeyenceCoverTrayID_LoaderCar        ]=edCoverTray_ID_PortKY;
    edPort_Keyence[iKeyenceCoverTrayID_Loader           ]=edCoverTray_ID2_PortKY;
    edPort_Keyence[iKeyenceCoverTrayID_AUTO1            ]=edCoverTray_ID3_PortKY;
    edPort_Keyence[iKeyenceCoverTrayID_AUTO2            ]=edCoverTray_ID4_PortKY;
    edPort_Keyence[iKeyenceCoverTrayID_AUTO3            ]=edCoverTray_ID5_PortKY;

    TrayMapSetDefaultView();

    iLoaderYCWSpeedPerc=33;
    if(USE_TRAY_MAPPING==etmDeviceRemain &&                                     //JimmyChiu 20220219 : 殘料檢查 From Loader
       (CosFunction.bTrayDeviceCheckFromLoader ||
        CosFunction.bTrayMapFromLoader))
    {
        tsLoaderDeviceCheck->TabVisible=true;
        InitialAOIGroup();
        InitialcbLaserScanMode();
    }
    else
    {
        tsLoaderDeviceCheck->TabVisible=false;
    }
    SetSimulationState(false);
    iLaserScanTask=0;
    iLoopCount=0;
    iLoopNow=0;
    bDoTrayDeviceCheckFromLoader=false;
    bSimulationAOICommand=false;
    iSettingCount=0;
    iAutoRetryCount=0;

    if(USE_LD_Rot_Arm==true)
        ldRFID=new TfRFID(commRFID, memoLdRFID);                                //RogerYang 20250830 : Add RFID

    lblNFC_CH1_UID->Caption="";
    lblNFC_CH2_UID->Caption="";
    lblNFC_CH1_CID->Caption="";
    lblNFC_CH1_CID->Caption="";
}
//---------------------------------------------------------------------------
__fastcall TfTrayMapping::~TfTrayMapping()                                      //RogerYang 20250830 : Add RFID
{
    if(USE_LD_Rot_Arm==true)
        delete ldRFID;   // 釋放
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::FormShow(TObject *Sender)
{
    fShow=true;

    Left=100;
    Top =50;

    DoIniDataToForm();

    if(bUseTrayMap)
    {
        PageControl1->ActivePageIndex=0;
        tsView->TabVisible=true;
    }
    else
    {
        PageControl1->ActivePageIndex=1;
        tsView->TabVisible=false;
    }
    palTrayMapGroup         ->Visible=(bUseTrayMap || bUseDeviceRemain);
    palTrayMapFunGroup      ->Visible=bUseTrayMap;
    palDeviceCountGroup     ->Visible=bUseDeviceCount;
    palTrayIDGroup          ->Visible=bUseTrayID;
    cbEnableTrayMap         ->Visible=bUseTrayMap;
    cbEnableTrayID          ->Visible=bUseTrayID;
    cbEnableTrayID2         ->Visible=bUseTrayID;
    cbEnableDeviceRemain    ->Visible=bUseDeviceRemain;
    cbEnableTrayDeviceCnt   ->Visible=bUseDeviceCount;
    if(USE_TRAY_MAPPING==etmDeviceRemain)
    {
        tsTrayCCD->TabVisible=false;
        if(CosFunction.bTrayDeviceCheckFromLoader)
            cbEnableDeviceRemainLaser->Visible=true;
        if(CosFunction.bTrayMapFromLoader)
            cbEnableTrayMapLaser->Visible=true;
    }

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //KaiChen 20200115 ：中壢日月光 TrayMapping
    {
        cbEnableTrayMap->Visible=true;

        cbEnableTrayID              ->Visible=false;
        cbEnableTrayDeviceCnt       ->Visible=false;
        cbEnableDeviceRemain        ->Visible=false;
        cbEnableTrayID2             ->Visible=false;
        palTrayIDGroup              ->Visible=false;
    }
    tsCommand->TabVisible=false;
    TrayMapSetDefaultView();
    SetYieldDatas(yieldRemainIC.GetYield());                                    //Jimmychiu 20231023 : 增加良率紀錄

    if(USE_LD_Rot_Arm==false)                                                   //RogerYang 20250830 : Add Loader旋轉臂+RFID
    {
        tsLoaderRFID->TabVisible=false;
    }

    if(USE_COVER_TRAYID==tCID_NFC)
    {
        if(tmrNFC->Enabled==false)
        {
            iNFCTask=1;
            tmrNFC->Enabled=true;
        }
    }

    //這一行請保持在最下面!!-----------------
    //myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void TfTrayMapping::TrayMapSetDefaultView()
{
    //JimmyChiu 20220219 : 整理程式碼
    //<=
    TrayMapSetDefault(sgTraymap);
    TrayMapSetDefault(sgSuckmap);
    TrayMapSetDefault(sgTrayDeviceCnt1);
    TrayMapSetDefault(sgTrayDeviceCnt2);
    TrayMapSetDefault(sgTrayDeviceCnt3);
    InitialTrayDeviceYield();
    //<=
    //JimmyChiu 20220219 : 整理程式碼
}
//---------------------------------------------------------------------------
void TfTrayMapping::TrayMapSetDefault(TStringGrid *tsGrid)
{
    AnsiString str="";
    tsGrid->ColCount=MOT[MMTrayY].Tray.XItem+1;
    tsGrid->RowCount=MOT[MMTrayY].Tray.YItem+1;
    tsGrid->Cells[0][0]="X/Y";
    for(int i=1; i<=MOT[MMTrayY].Tray.XItem; i++)
    {
        str.sprintf("%d", i);
        tsGrid->Cells[i][0]=str.c_str();
    }
    for(int i=1; i<=MOT[MMTrayY].Tray.YItem; i++)
    {
        str.sprintf("%d", i);
        tsGrid->Cells[0][i]=str.c_str();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    cbPassAOI->Checked=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    //這一行請保持在最下面!!-----------------
    //myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::FormDestroy(TObject *Sender)
{
    AnsiString sCCDCommand="LOFF";
    if(ClientSocket_Keyence1->Active)
    {
        ClientSocket_Keyence1->Socket->SendText(sCCDCommand+"\r\n");
    }

    if(ClientSocket_Keyence2->Active)
    {
        ClientSocket_Keyence2->Socket->SendText(sCCDCommand+"\r\n");
    }

    if(ClientSocket_Keyence3->Active)
    {
        ClientSocket_Keyence3->Socket->SendText(sCCDCommand+"\r\n");
    }

    if(ClientSocket_Keyence4->Active)
    {
        ClientSocket_Keyence4->Socket->SendText(sCCDCommand+"\r\n");
    }
    try
    {
        TimerProcessTrayData->Enabled   =false;
        TimerCCDTrayInitial->Enabled    =false;
        TimerDownCCDTrayConnect->Enabled=false;
        ClientSocket_Tray_ID->Active    =false;
        ClientSocket_Tray_Map->Active   =false;
        ClientSocket_Tray_ID2->Active    =false;
        //Sam 20190405 : Tray Decive Count
        //==>
        ClientSocket_Tray_DeviceCnt1->Active    =false;
        ClientSocket_Tray_DeviceCnt2->Active    =false;
        ClientSocket_Tray_DeviceCnt3->Active    =false;
        //<==
        //Sam 20190405 : Tray Decive Count
        ClientSocket_Keyence1->Active   =false;
        ClientSocket_Keyence2->Active   =false;
        ClientSocket_Keyence3->Active   =false;
        ClientSocket_Keyence4->Active   =false;
        ClientSocket_Tray_ID->Close();
        ClientSocket_Tray_Map->Close();
        ClientSocket_Tray_ID2->Close();
        //Sam 20190405 : Tray Decive Count
        //==>
        ClientSocket_Tray_DeviceCnt1->Close();
        ClientSocket_Tray_DeviceCnt2->Close();
        ClientSocket_Tray_DeviceCnt3->Close();
        //<==
        //Sam 20190405 : Tray Decive Count
        ClientSocket_Keyence1->Close();
        ClientSocket_Keyence2->Close();
        ClientSocket_Keyence3->Close();
        ClientSocket_Keyence4->Close();
        for(int i=0; i<iTotalFunction; i++)                                     //Sam 20190405 : Tray Decive Count
        {
            SocketTrayReceiveList[i]->Clear();                                  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete SocketTrayReceiveList[i];
        }
        ClearAllAOI();                                                          //JimmyChiu 20220219 : 殘料檢查 From Loader
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfTrayMapping::FormDestroy");
    }
    LogSoftwareOffTime("TfTrayMapping, FormDestroy");                           //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::DoIniDataToForm()
{
    edTray_ID_Address->Text         =HSys.asCCDTrayIP[iTray_ID              ];  //wei 20161219 Tray Mapping
    edTray_Map_Address->Text        =HSys.asCCDTrayIP[iTray_Map             ];  //wei 20161219 Tray Mapping

    edTray_ID2_Address->Text        =HSys.asCCDTrayIP[iTray_ID2             ];  //wei 20180809 Tray ID
    edTrayDeviceCnt1_Address->Text  =HSys.asCCDTrayIP[iTray_DeviceCnt1      ];  //Sam 20190405 : Tray Decive Count
    edTrayDeviceCnt2_Address->Text  =HSys.asCCDTrayIP[iTray_DeviceCnt2      ];
    edTrayDeviceCnt3_Address->Text  =HSys.asCCDTrayIP[iTray_DeviceCnt3      ];
    edCoverTray_ID_AddressKY->Text  =HSys.asCCDTrayIP[iCoverTrayID_LoaderCar];
    edCoverTray_ID2_AddressKY->Text =HSys.asCCDTrayIP[iCoverTrayID_Loader   ];
    edCoverTray_ID3_AddressKY->Text =HSys.asCCDTrayIP[iCoverTrayID_AUTO1    ];
    edCoverTray_ID4_AddressKY->Text =HSys.asCCDTrayIP[iCoverTrayID_AUTO2    ];
    edCoverTray_ID5_AddressKY->Text =HSys.asCCDTrayIP[iCoverTrayID_AUTO3    ];

    edTray_ID_Port->Text            =HSys.asCCDTrayPort[iTray_ID              ];//wei 20161219 Tray Mapping
    edTray_Map_Port->Text           =HSys.asCCDTrayPort[iTray_Map             ];//wei 20161219 Tray Mapping

    edTray_ID2_Port->Text           =HSys.asCCDTrayPort[iTray_ID2             ];//wei 20180809 Tray ID
    edTrayDeviceCnt1_Port->Text     =HSys.asCCDTrayPort[iTray_DeviceCnt1      ];//Sam 20190405 : Tray Decive Count
    edTrayDeviceCnt2_Port->Text     =HSys.asCCDTrayPort[iTray_DeviceCnt2      ];
    edTrayDeviceCnt3_Port->Text     =HSys.asCCDTrayPort[iTray_DeviceCnt3      ];
    edCoverTray_ID_PortKY->Text     =HSys.asCCDTrayPort[iCoverTrayID_LoaderCar];
    edCoverTray_ID2_PortKY->Text    =HSys.asCCDTrayPort[iCoverTrayID_Loader   ];
    edCoverTray_ID3_PortKY->Text    =HSys.asCCDTrayPort[iCoverTrayID_AUTO1    ];
    edCoverTray_ID4_PortKY->Text    =HSys.asCCDTrayPort[iCoverTrayID_AUTO2    ];
    edCoverTray_ID5_PortKY->Text    =HSys.asCCDTrayPort[iCoverTrayID_AUTO3    ];

    cbEnableTrayMap->Checked        =TestIF_File.bEnableTrayMap;                //wei 20161219 Tray Mapping
    cbEnableTrayID->Checked         =TestIF_File.bEnableTrayID;                 //wei 20161219 Tray Mapping
    cbEnableTrayID2->Checked        =TestIF_File.bEnableTrayID2;                //wei 20161219 Tray Mapping
    cbEnableTrayDeviceCnt->Checked  =TestIF_File.bEnableTrayDeviceCnt;          //Sam 20190405 : Tray Decive Count
    edTrayStartDelay->Text          =TestIF_File.iTrayStartDelay;               //wei 20161219 Tray Mapping
    edTrayExposureTimeOut->Text     =TestIF_File.iTrayExposureTimeOut;          //wei 20161219 Tray Mapping
    edTrayGetResultTimeOut->Text    =TestIF_File.iTrayGetResultTimeOut;         //wei 20161219 Tray Mapping
    edTrayAutoRetry->Text           =TestIF_File.iTrayAutoRetry;                //wei 20161219 Tray Mapping
    edtTraycodeMinLength->Text      =TestIF_File.iTrayCodeMinLength;            //wei 20161219 Tray Mapping
    edtTraycodeMaxLength->Text      =TestIF_File.iTrayCodeMaxLength;            //wei 20161219 Tray Mapping
    cbDisableMapSuck->Checked       =TestIF_File.bDisableMapSuck;               //wei 20161219 Tray Mapping
    cbEnableSuckMapCheck->Checked   =TestIF_File.bEnableSuckMapCheck;           //wei 20161219 Tray Mapping
    cbEnableDeviceRemain->Checked   =TestIF_File.bEnableDeviceRemain;           //wei 20170317 (steven) Device Remain 殘料檢測
    cbEnableTrayIDFTP->Checked      =TestIF_File.bEnableTrayIDDownFTP;          //wei 20170518 Tray Mapping
    cbEnableOCRTrayID->Checked      =TestIF_File.bEnableOCRTrayIDDown;          //wei 20170518 Tray Mapping
    edtTrayIDShift->Text            =TestIF_File.iTrayIDShift;                  //wei 20170606 Tray Mapping
    edtTrayID2Shift->Text           =TestIF_File.iTrayID2Shift;                 //wei 20170606 Tray Mapping
    edtTrayMapStart->Text           =TestIF_File.iTrayMapStart;                 //wei 20161219 Tray Mapping
    edtTrayDeciveCntStart->Text     =TestIF_File.iTrayDeciveCntStart;
    ReadFile();//Jimmychiu 20220915 Initial parameter
    dmTrayMotor->StartSetSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");

    WriteIniData(szDir, "Configuration", "Tray Mapping Enable",                 (cbEnableTrayMap->Checked)?1:0);
    if(cbEnableTrayID->Checked!=TestIF_File.bEnableTrayID)
    {
        if(INSTALL_OCR!=eocrUninstal && cbEnableTrayID->Checked==true)
        {
            fOCR->spInternetConnectClick(fOCR);
        }
    }
    WriteIniData(szDir, "Configuration", "Tray Mapping ID Enable",              (cbEnableTrayID->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "Tray Mapping ID 2 Enable",            (cbEnableTrayID2->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "Tray Mapping Start Delay",            edTrayStartDelay->Text);
    WriteIniData(szDir, "Configuration", "Tray Mapping Exposure Time Out",      edTrayExposureTimeOut->Text);
    WriteIniData(szDir, "Configuration", "Tray Mapping Get Result Time Out",    edTrayGetResultTimeOut->Text);
    WriteIniData(szDir, "Configuration", "Tray Mapping Auto Retry",             edTrayAutoRetry->Text);
    WriteIniData(szDir, "Configuration", "Tray Code Min Length",                edtTraycodeMinLength->Text);
    WriteIniData(szDir, "Configuration", "Tray Code Max Length",                edtTraycodeMaxLength->Text);
    WriteIniData(szDir, "Configuration", "Disable Tray Mapping Suck",           (cbDisableMapSuck->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "Enable Tray Suck Map Check",          (cbEnableSuckMapCheck->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "Enable Device Remain",                (cbEnableDeviceRemain->Checked)?1:0);   //wei 20170317 Device Remain 殘料檢測
    WriteIniData(szDir, "Configuration", "Enable Tray ID Down FTP",             (cbEnableTrayIDFTP->Checked)?1:0);

    if(INSTALL_OCR==eocrUninstal)
        cbEnableOCRTrayID->Checked=false;

    WriteIniData(szDir, "Configuration", "Enable OCR Tray ID",                  (cbEnableOCRTrayID->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "Tray ID Catch Shift",                 edtTrayIDShift->Text);
    WriteIniData(szDir, "Configuration", "Tray ID 2 Catch Shift",               edtTrayID2Shift->Text);
    WriteIniData(szDir, "Configuration", "Tray Mapping Catch Start",            edtTrayMapStart->Text);
    WriteIniData(szDir, "Configuration", "Tray Decive Count Catch Start",       edtTrayDeciveCntStart->Text);
    WriteIniData(szDir, "Configuration", "Tray Device Count Enable",            (cbEnableTrayDeviceCnt->Checked)?1:0);  //Sam 20190405 : Tray Decive Count
    //JimmyChiu 20220219 : 殘料檢查 From Loader
    //<==
    SaveFile_ScanLine();
    //<==
    //JimmyChiu 20220219 : 殘料檢查 From Loader

    ReadFile();
    spbSave->Down=false;
    dmTrayMotor->StartSetSpeed();                                               //Steven 20210318 : Loader入Tray改步進, 換位置寫入速度, 避免動作異常
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ReadFile()
{
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");

    cbbLdRFIDComPort->Text=HSys.asRFIDCom;

    if(USE_TRAY_MAPPING==etmUninstall)
    {
        TestIF_File.bEnableTrayMap      =false;
        TestIF_File.bEnableTrayID       =false;
        TestIF_File.bEnableDeviceRemain =false;                                 //wei 20170317 Device Remain 殘料檢測
        TestIF_File.bEnableTrayID2      =false;
        TestIF_File.bEnableTrayDeviceCnt=false;                                 //Sam 20190405 : Tray Decive Count
    }
    else
    {
        if(bUseTrayMap)
        {
            TestIF_File.bEnableTrayMap      =ReadIniData(szDir, "Configuration", "Tray Mapping Enable",                 false);
            TestIF_File.bEnableTrayID       =ReadIniData(szDir, "Configuration", "Tray Mapping ID Enable",              false);
            TestIF_File.bEnableTrayID2      =ReadIniData(szDir, "Configuration", "Tray Mapping ID 2 Enable",            false);
        }
        else
        {
            TestIF_File.bEnableTrayMap      =false;
            TestIF_File.bEnableTrayID       =false;
        }

        if(bUseDeviceRemain)
        {
            TestIF_File.bEnableDeviceRemain =ReadIniData(szDir, "Configuration", "Enable Device Remain",                false);     //wei 20170317 (steven) Device Remain 殘料檢測
        }
        else
        {
            TestIF_File.bEnableDeviceRemain=false;
        }

        if(bUseDeviceCount)
        {
            TestIF_File.bEnableTrayDeviceCnt=ReadIniData(szDir, "Configuration", "Tray Device Count Enable",            false);     //Sam 20190405 : Tray Decive Count
        }
        else
        {
            TestIF_File.bEnableTrayDeviceCnt=false;
        }
    }

    TestIF_File.iTrayStartDelay         =ReadIniData(szDir, "Configuration", "Tray Mapping Start Delay",            5900);     //Jimmychiu 20231110 : 和Roger討論後，Default 500 to 5900
    TestIF_File.iTrayExposureTimeOut    =ReadIniData(szDir, "Configuration", "Tray Mapping Exposure Time Out",      1500);
    TestIF_File.iTrayGetResultTimeOut   =ReadIniData(szDir, "Configuration", "Tray Mapping Get Result Time Out",    10000);    //Jimmychiu 20231110 : 和Roger討論後，Default 1500 to 10000
    TestIF_File.iTrayAutoRetry          =ReadIniData(szDir, "Configuration", "Tray Mapping Auto Retry",             1);

    TestIF_File.iTrayCodeMinLength      =ReadIniData(szDir, "Configuration", "Tray Code Min Length",                5);
    TestIF_File.iTrayCodeMaxLength      =ReadIniData(szDir, "Configuration", "Tray Code Max Length",                30);
    TestIF_File.iTrayMapCatch           =ReadIniData(szDir, "Configuration", "Tray Mapping Catch Count",            4);
    TestIF_File.iTrayMapShift           =ReadIniData(szDir, "Configuration", "Tray Mapping Catch Shift",            1000);

    TestIF_File.bDisableMapSuck         =ReadIniData(szDir, "Configuration", "Disable Tray Mapping Suck",           false);
    TestIF_File.bEnableSuckMapCheck     =ReadIniData(szDir, "Configuration", "Enable Tray Suck Map Check",          false);
    TestIF_File.bEnableTrayIDDownFTP    =ReadIniData(szDir, "Configuration", "Enable Tray ID Down FTP",             false);
    TestIF_File.bEnableOCRTrayIDDown    =ReadIniData(szDir, "Configuration", "Enable OCR Tray ID",                  false);

    TestIF_File.iTrayIDShift            =ReadIniData(szDir, "Configuration", "Tray ID Catch Shift",                 10000);
    TestIF_File.iTrayIDReadShift        =ReadIniData(szDir, "Configuration", "Tray ID Read Catch Shift",            -1000);
    TestIF_File.iTrayID2Shift           =ReadIniData(szDir, "Configuration", "Tray ID 2 Catch Shift",               10000);
    TestIF_File.iTrayMapStart           =ReadIniData(szDir, "Configuration", "Tray Mapping Catch Start",            1000);
    TestIF_File.iTrayDeciveCntStart     =ReadIniData(szDir, "Configuration", "Tray Decive Count Catch Start",       1000);
    //JimmyChiu 20220219 : 殘料檢查 From Loader
    //<==
    ReadFile_ScanLine();
    //<==
    //JimmyChiu 20220219 : 殘料檢查 From Loader
    if(USE_TRAY_MAPPING!=etmUninstall)
        fMain->ShowOCRState(0);

    ChangeTraySetupFile();

    TrayMapSetDefaultView();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btTray_ID_TriggerClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    AnsiString strSendCMD="";
    tempBtn = (TSpeedButton *)Sender;
    switch(tempBtn->Tag)
    {
        case 0:
            strSendCMD=edTray_ID_CMD->Text;
            SendCCDTrayCommand(iTray_ID, " ", strSendCMD);
            break;
        case 1:
            strSendCMD=edTray_Map_CMD->Text;
            SendCCDTrayCommand(iTray_Map, " ", strSendCMD);
            break;
        case 3:
            strSendCMD=edTray_ID2_CMD->Text;
            SendCCDTrayCommand(iTray_ID2, " ", strSendCMD);
            break;
        //Sam 20190405 : Tray Decive Count
        //==>
        case 4:
            strSendCMD=edTrayDeviceCnt1_CMD->Text;
            SendCCDTrayCommand(iTray_DeviceCnt1, " ", strSendCMD);
            break;
        case 5:
            strSendCMD=edTrayDeviceCnt2_CMD->Text;
            SendCCDTrayCommand(iTray_DeviceCnt2, " ", strSendCMD);
            break;
        case 6:
            strSendCMD=edTrayDeviceCnt3_CMD->Text;
            SendCCDTrayCommand(iTray_DeviceCnt3, " ", strSendCMD);
            break;
        //<==
        //Sam 20190405 : Tray Decive Count
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::SendCCDTrayCommand(int index, AnsiString Msg, AnsiString Msg2)
{
    if(memoTrayCCDCommLog==NULL || InitialOK==false)
        return;

    AnsiString Log;
    GetTimeInfo();
    if(Msg2=="")
        Log.sprintf("Action, %s", Msg);
    else
        Log.sprintf("Send, %s, %s", Msg, Msg2);

    AddCCDTrayCommunicationLog(index, Log);
    if(index==iTray_ID)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Tray_ID->Active)
            {
                ClientSocket_Tray_ID->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iTray_ID2)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Tray_ID2->Active)
            {
                ClientSocket_Tray_ID2->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iTray_Map || index==iDeviceRemain)                           //wei 20170317 (steven) Device Remain 殘料檢測
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Tray_Map->Active)
            {
                ClientSocket_Tray_Map->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    //Sam 20190405 : Tray Decive Count
    //==>
    else if(index==iTray_DeviceCnt1)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Tray_DeviceCnt1->Active)
            {
                ClientSocket_Tray_DeviceCnt1->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iTray_DeviceCnt2)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Tray_DeviceCnt2->Active)
            {
                ClientSocket_Tray_DeviceCnt2->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iTray_DeviceCnt3)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Tray_DeviceCnt3->Active)
            {
                ClientSocket_Tray_DeviceCnt3->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    //<==
    //Sam 20190405 : Tray Decive Count
}
//---------------------------------------------------------------------------
void TfTrayMapping::AddCCDTrayCommunicationLog(int index, AnsiString Str)       //Steven 20160421 : Add 2D CCD Log
{
    AnsiString Path, Name, Log;

    if(index==iTray_ID)
    {
        Name="Tray ID";
    }
    else if(index==iTray_Map)
    {
        if(TestIF_File.bEnableTrayMap==true)
            Name="Tray Map";
        else
            Name="IC Remain";
    }
    else if(index==iDeviceRemain)                                               //wei 20170317 (steven) Device Remain 殘料檢測
    {
        Name="IC Remain";
    }
    else if(index==iTray_ID2)
    {
        Name="Tray ID 2";
    }
    //Sam 20190405 : Tray Decive Count
    //==>
    else if(index==iTray_DeviceCnt1)
    {
        Name="Tray Device Count 1";
    }
    else if(index==iTray_DeviceCnt2)
    {
        Name="Tray Device Count 2";
    }
    else if(index==iTray_DeviceCnt3)
    {
        Name="Tray Device Count 3";
    }
    //<==
    //Sam 20190405 : Tray Decive Count
    else if(index==90)                                                          //JimmyChiu 20220219 : 殘料檢查 From Loader
    {
        Name="Laser From Loader";
    }
    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %s, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Name, Str);
    if(memoTrayCCDCommLog->Lines->Count>1000)
        memoTrayCCDCommLog->Clear();
    memoTrayCCDCommLog->Lines->Add(Log);

    Path.sprintf("%s\\%04d_%02d_%02d", asTrayMapCommLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path, "TfTrayMapping::AddCCDTrayCommunicationLog");
    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asTrayMapCommLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str() , Log.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::TimerDownCCDTrayConnectTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;
    static int iCount[iTotalFunction]={0};
    AnsiString sErrMsg="";

    //Sam 20200316 : 整合
    //==>
    for(int i=0; i<iTotalFunction; i++)
    {
        if(i==iTray_ID && TestIF_File.bEnableTrayID==false)
        {
            continue;
        }
        else if(i==iTray_Map && TestIF_File.bEnableTrayMap==false && TestIF_File.bEnableDeviceRemain==false)
        {
            continue;
        }
        else if(i==iDeviceRemain)
        {
            continue;
        }
        else if(i==iTray_ID2 && TestIF_File.bEnableTrayID2==false)
        {
            continue;
        }
        else if(i==iTray_DeviceCnt1 && TestIF_File.bEnableTrayDeviceCnt==false)
        {
            continue;
        }
        else if(i==iTray_DeviceCnt2 && TestIF_File.bEnableTrayDeviceCnt==false)
        {
            continue;
        }
        else if(i==iTray_DeviceCnt3 && TestIF_File.bEnableTrayDeviceCnt==false)
        {
            continue;
        }
        else if(i>=iCoverTrayID_LoaderCar && i<=iCoverTrayID_AUTO3)
        {
            continue;
        }

        if((i==iTray_ID && USE_KEYENCE_LOADER==1) ||
           (i==iTray_ID2 && USE_KEYENCE_EMPTY!=0))
        {
            if(clntsckt_Tray_Keyence[i]->Active                           &&    //相同的IP跟Port就不要重新連線
               clntsckt_Tray_Keyence[i]->Address==edAddress_Keyence[i]->Text &&
               clntsckt_Tray_Keyence[i]->Port   ==edPort_Keyence[i]->Text)
            {

            }
            else
            {
                if(clntsckt_Tray_Keyence[i]->Active)
                {
                    clntsckt_Tray_Keyence[i]->Close();
                    SendKeyenceCommand(i, "ClientSocket Close", "");
                    iCount[i]=0;
                }
            }

            if(clntsckt_Tray_Keyence[i]->Active==false &&
               bEnableKeyenceChannelConnect[i]==true)
            {
                iCount[i]++;
                if(iCount[i]>15)
                {
                    clntsckt_Tray_Keyence[i]->Address   =edAddress_Keyence[i]->Text;
                    clntsckt_Tray_Keyence[i]->Port      =atoi(edPort_Keyence[i]->Text.c_str());
                    SendKeyenceCommand(i, "ClientSocket connecting..", "");
                    try
                    {
                        clntsckt_Tray_Keyence[i]->Open();
                    }
                    catch(...)
                    {
                        sErrMsg.sprintf("TfTrayMapping::TimerDownCCDTrayConnectTimer Tag=%d",i);
                        MyDBIProcess("Exception", sErrMsg);
                    }
                    SendKeyenceCommand(i, "ClientSocket Open", "");
                    iCount[i]=0;
                }
            }
        }
        else
        {
            if(clntsckt_Tray[i]->Active                           &&            //相同的IP跟Port就不要重新連線
               clntsckt_Tray[i]->Address==edAddress[i]->Text &&
               clntsckt_Tray[i]->Port   ==edPort[i]->Text)
            {

            }
            else
            {
                if(clntsckt_Tray[i]->Active)
                {
                    clntsckt_Tray[i]->Close();
                    SendCCDTrayCommand(i, "ClientSocket Close", "");
                    iCount[i]=0;
                }
            }

            if(clntsckt_Tray[i]->Active==false && bEnableCCDTrayChannelConnect[i]==true)
            {
                iCount[i]++;
                if(iCount[i]>15)
                {
                    clntsckt_Tray[i]->Address   =edAddress[i]->Text;
                    clntsckt_Tray[i]->Port      =atoi(edPort[i]->Text.c_str());
                    SendCCDTrayCommand(i, "ClientSocket connecting..", "");
                    try
                    {
                        clntsckt_Tray[i]->Open();
                    }
                    catch(...)
                    {
                        sErrMsg.sprintf("TfTrayMapping::TimerDownCCDTrayConnectTimer Tag=%d",i);
                        MyDBIProcess("Exception", sErrMsg);
                    }
                    SendCCDTrayCommand(i, "ClientSocket Open", "");
                    iCount[i]=0;
                }
            }
        }
    }
    //<==
    //Sam 20200316 : 整合

    if(USE_KEYENCE_LOADER==1 ||
       USE_KEYENCE_EMPTY>0 ||                                                   //Jimmychiu 20250117 : Avoid executing Keyence inspection with only the Tray Mapping function enabled
       fAGV->IsSPIL_AMR())                                                      //JerryYang 20250521 : For AMR
    {
        for(int i=iKeyenceCoverTrayID_LoaderCar; i<=iKeyenceCoverTrayID_AUTO3; i++)
        {
            if(clntsckt_Tray_Keyence[i]->Active                           &&        //相同的IP跟Port就不要重新連線
               clntsckt_Tray_Keyence[i]->Address==edAddress_Keyence[i]->Text &&
               clntsckt_Tray_Keyence[i]->Port   ==edPort_Keyence[i]->Text)
            {

            }
            else
            {
                if(clntsckt_Tray_Keyence[i]->Active)
                {
                    clntsckt_Tray_Keyence[i]->Close();
                    SendKeyenceCommand(i, "ClientSocket Close", "");
                    iCount[i]=0;
                }
            }

            if(clntsckt_Tray_Keyence[i]->Active==false && bEnableKeyenceChannelConnect[i]==true)
            {
                iCount[i]++;
                if(iCount[i]>15)
                {
                    clntsckt_Tray_Keyence[i]->Address   =edAddress_Keyence[i]->Text;
                    clntsckt_Tray_Keyence[i]->Port      =atoi(edPort_Keyence[i]->Text.c_str());
                    SendKeyenceCommand(i, "ClientSocket connecting..", "");
                    try
                    {
                        clntsckt_Tray_Keyence[i]->Open();
                    }
                    catch(...)
                    {
                        sErrMsg.sprintf("TfTrayMapping::TimerDownCCDTrayConnectTimer Tag=%d",i);
                        MyDBIProcess("Exception", sErrMsg);
                    }
                    SendKeyenceCommand(i, "ClientSocket Open", "");
                    iCount[i]=0;
                }
            }
        }
    }

    if(USE_TRAY_MAPPING==etmDeviceRemain &&
      (CosFunction.bTrayDeviceCheckFromLoader ||
       CosFunction.bTrayMapFromLoader))
    {
        UpdateConnectionState(BtLoaderDC_Connect);                              //JimmyChiu 20220219 : 殘料檢查 From Loader
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
void TfTrayMapping::ChangeTraySetupFile()
{
    iCCDConntectionOkTask=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::TimerCCDTrayInitialTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;
    int isck;
    int &Task=iCCDConntectionOkTask;
    AnsiString Str;

    if(TestIF_File.bEnableTrayMap==true || TestIF_File.bEnableDeviceRemain==true)
    {
        isck=1;
    }
    else if(TestIF_File.bEnableTrayID)
    {
        isck=0;
    }
    else if(TestIF_File.bEnableTrayID2)
    {
        isck=3;
    }
    else if(TestIF_File.bEnableTrayDeviceCnt)
    {
        isck=4;
    }

    if(TestIF_File.bEnableTrayMap==true || TestIF_File.bEnableDeviceRemain==true)
    {
        if(clntsckt_Tray[isck]->Active                            &&  //相同的IP跟Port就不要重新連線
           clntsckt_Tray[isck]->Address==edAddress[isck]->Text &&
           clntsckt_Tray[isck]->Port==edPort[isck]->Text)
        {
            switch(Task)
            {
                case 1:
                    dVisionVer=1;
                    Str.sprintf("VR");
                    SendCCDTrayCommand(isck, "Get Version", Str);     //Reset CCD Shuttle暫存器
                    bTrayGetVR=false;
                    tCCDChangeSetupFile.SetSecAndOn(30);
                    Task=100;

                case 100:
                    if(bTrayGetVR==true)
                    {
                        Task=1000;
                    }
                    else if(tCCDChangeSetupFile.Off())
                        Task=1;
                    break;
                case 1000:  //Reset CCD Shuttle暫存器
                    bGetTraySE9[isck]=false;
                    if(isck==0 || isck==3)
                    {
                        Str.sprintf("E9,3");
                    }
                    else
                    {
                        Str.sprintf("E9,4");
                    }

                    SendCCDTrayCommand(isck, "Clear buffer", Str);

                    tCCDChangeSetupFile.SetSecAndOn(10);
                    Task=1050;
                    //break;
                case 1050:
                    if(bGetTraySE9[isck])
                        Task=1100;
                    else if(tCCDChangeSetupFile.Off())
                        Task=1000;
                    break;
                case 1100:  //取得工作檔名稱
                    bGetGF=false;
                    SendCCDTrayCommand(isck, "Get File Name", "GF,4");

                    tCCDChangeSetupFile.SetSecAndOn(10);
                    Task=1150;
                    //break;
                case 1150:
                    if(bGetGF)
                    {
                        if(fMain->cbSetupFileName->Text.UpperCase().AnsiCompare(sCurrentGF)==0)
                        {
                            Task=3000;
                            bGetLF=true;
                            bChangeBarcodeFileOK=true;
                        }
                        else
                        {
                            Task=1200;
                        }
                    }
                    else if(tCCDChangeSetupFile.Off())
                        Task=1100;
                    break;
                case 1200:
                    bGetLF=false;
                    SendCCDTrayCommand(isck, "Change File", "LF,4,"+fMain->cbSetupFileName->Text);

                    tCCDChangeSetupFile.SetSecAndOn(10);
                    Task=1250;
                case 1250:
                    if(bGetLF)
                    {
                        Task=3000;
                    }
                    else if(tCCDChangeSetupFile.Off())
                    {
                        Task=1200;
                    }
                    break;
                case 3000:
                    break;
            }
        }
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::BtTray_ID_ConnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    TimerDownCCDTrayConnect->Enabled=true;
    bEnableCCDTrayChannelConnect[tempBtn->Tag]=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::BtTray_ID_DisconnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    bEnableCCDTrayChannelConnect[tempBtn->Tag]=false;
    switch(tempBtn->Tag)
    {
        case 0:
            ClientSocket_Tray_ID->Close();
            break;

        case 1:
            ClientSocket_Tray_Map->Close();
            break;

        case 3:
            ClientSocket_Tray_ID2->Close();
            break;
        //Sam 20190405 : Tray Decive Count
        //==>
        case 4:
            ClientSocket_Tray_DeviceCnt1->Close();
            break;
        case 5:
            ClientSocket_Tray_DeviceCnt2->Close();
            break;
        case 6:
            ClientSocket_Tray_DeviceCnt3->Close();
            break;
        //<==
        //Sam 20190405 : Tray Decive Count
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::sbtExitClick(TObject *Sender)
{
    if(bDoTrayMapTune)
    {
        ShowMyMessage("Tray Map Auto Tune need Finish !!", "需要完成 Tray Map Auto Tune!!");
        return;
    }
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Tray_IDDisconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    SendCCDTrayCommand(Ptr->Tag, "DisConnect", "");
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Tray_IDError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    if(InitialOK==false)
    {
        return;
    }

    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    int Tag=Ptr->Tag;
    SendCCDTrayCommand(Tag, GetErrorMsg(Sender, ErrorCode), "");
    ErrorEvent=TErrorEvent(NULL);
    ErrorCode =0;

    try
    {
        Ptr->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "ClientSocket_Tray_ID Error");
    }

    Abort();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Tray_IDRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    int Tag=Ptr->Tag;

    AnsiString EthernetBuffer;
    int iBufferLenght=0;
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0)                                                         //13.09.30.01   klutter
    {
        EthernetBuffer=Socket->ReceiveText();
        EthernetBuffer=StringReplace(EthernetBuffer, "\r", ",", TReplaceFlags()<<rfReplaceAll); //Steven 20160512 : 2D log排版
        EthernetBuffer=StringReplace(EthernetBuffer, "\n", "", TReplaceFlags()<<rfReplaceAll);
        SocketTrayReceiveList[Tag]->Add(EthernetBuffer);

        AnsiString Log;
        Log.sprintf("Recv, , %s", EthernetBuffer);
        AddCCDTrayCommunicationLog(Tag, Log);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Tray_IDConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;

    if(InitialOK==false)
    {
        Ptr->Active=false;
        return;
    }

    AnsiString Str;
    Str.sprintf("IP: %s, Port:%d [Connect OK]", Ptr->Address, Ptr->Port);
    SendCCDTrayCommand(Ptr->Tag, Str, "");

    if(Ptr->Tag==iTray_Map)
    {
        ChangeTraySetupFile();                                                  //Steven 20160425 : 重置2DSys時間
    }
    else if(dVisionVer>=0)
    {
        if(TestIF_File.bEnableTrayID && USE_KEYENCE_LOADER==0)
        {
            Str.sprintf("E9,3,0");
            SendCCDTrayCommand(Ptr->Tag, "Clear buffer", Str);                  //Reset CCD Shuttle暫存器
        }

        if(TestIF_File.bEnableTrayID2 && USE_KEYENCE_EMPTY==0)
        {
            Str.sprintf("E9,3,1");
            SendCCDTrayCommand(Ptr->Tag, "Clear buffer", Str);                  //Reset CCD Shuttle暫存器
        }

        if(TestIF_File.bEnableTrayMap || TestIF_File.bEnableDeviceRemain)
        {
            Str.sprintf("E9,4");
            SendCCDTrayCommand(Ptr->Tag, "Clear buffer", Str);                  //Reset CCD Shuttle暫存器
        }

        if(TestIF_File.bEnableTrayDeviceCnt)                                    //Sam 20190405 : Tray Decive Count
        {
            Str.sprintf("E9,4");
            SendCCDTrayCommand(Ptr->Tag, "Clear buffer", Str);                  //Reset CCD Shuttle暫存器
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::TimerProcessTrayDataTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)                                     //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun=true;
    if(USE_TRAY_MAPPING==etmDeviceRemain &&
       GetSimulationState() &&
       DCCycleDelay.Off() &&
       SystemStart==false)
    {
        DCCycleDelay.SetSecAndOn(0.01);
        if(iLoopCount>iLoopNow)
        {
            if(cbLaserScanMode->Text==AnsiString("Check Device"))
            {
                if(DoLaserScanFromLoader(eAOIType_TrayDeviceCheckLaser,false,true))
                {
                    iLoopNow++;
                    if(fTrayMapping->asErrorLaserScan!="")
                    {
                        ShowCheckDeviceResult(fTrayMapping->asErrorLaserScan);
                    }
                    else if(fTrayMapping->GetTrayDeviceCheckIsRemainIC()==true)
                    {
                        ShowCheckDeviceResult("Fail");
                        SetRealRunInitialStart(eAOIType_TrayDeviceCheckLaser,true); //放置IC Tray必須檢出異常才能解除
                    }
                    else
                    {
                        ShowCheckDeviceResult("Pass");
                    }
                    DoLaserScanFromLoader(eAOIType_TrayDeviceCheckLaser,true,true);
                }
            }
            else    //Loader Tray Map
            {
                if(DoLaserScanFromLoader(eAOIType_TrayMapLaser,false,true))
                {
                    iLoopNow++;
                    DoLaserScanFromLoader(eAOIType_TrayMapLaser,true,true);
                }
            }
        }
        else
        {
            RecordMsgLaser("SimulateRemainIC Finish");
            cbAutoLearning->Checked=false;
            SetSimulationState(false);
        }
    }

    AnsiString TrayPos[7]={"Tray ID","Tray Map","IC Remain","Tray ID2","Tray Device Count1","Tray Device Count2","Tray Device Count3"};
    AnsiString sPrefix, sReply="";
    AnsiString Str2, Str;
    AnsiString Data;
    int iTrayIDDataCount=0;
    int iPosition;
    int iResult=-1;
    int ich=0;
    bool bResult=false;

    for(int Tag=0; Tag<iTotalFunction; Tag++)
    {
        if(SocketTrayReceiveList[Tag]->Count>0)
        {
            sDownCCDRecevieData[Tag]=SocketTrayReceiveList[Tag]->Strings[0];
            Str2=StringReplace(sDownCCDRecevieData[Tag], ",", "\n", TReplaceFlags()<<rfReplaceAll);     //Steven 20160401 : 解決Code有空格的問題
            SocketTrayReceiveList[Tag]->Delete(0);
            if(SocketTrayReceiveList[Tag]->Count==0)
                SocketTrayReceiveList[Tag]->Clear();

            TStringList *sList = new TStringList();
            sList->SetText(Str2.c_str());                                       //Steven 20160401 : 解決Code有空格的問題
            sPrefix=sList->Strings[0];

            if(sPrefix=="VR")                                                   //軟體版本
            {
                bTrayGetVR=true;
                if(sList->Count>=2)
                    dVisionVer=atof(sList->Strings[1].c_str());
                else
                    dVisionVer=1;

                sReply.sprintf("VROK : %s", sDownCCDRecevieData[Tag]);
            }
            else if(sPrefix=="CA")          //關閉Vision軟體
            {

            }
            else if(sPrefix=="AP")          //取得路徑
            {

            }
            else if(sPrefix=="ST")          //設定時間
            {
                if(sList->Count>=2)
                    iResult=atoi(sList->Strings[1].c_str());
                bGetST=true;
                sReply.sprintf("%s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
            }
            else if(dVisionVer>=2)          //新版Command
            {
                if(sList->Count>=2)
                    iFunction=atoi(sList->Strings[1].c_str());

                if(sList->Count>=3)
                    iResult=atoi(sList->Strings[2].c_str());

                //----------------------------
                // TRAY ID
                //----------------------------
                if(iFunction==3)
                {
                    if(USE_MR_SYSTEM)
                    {
                        if(sList->Count>=4)
                            ich=atoi(sList->Strings[2].c_str());

                        if(sList->Count>=4)
                            iResult=atoi(sList->Strings[3].c_str());
                    }
                    else
                    {
                        if(sList->Count>=3)
                            iResult=atoi(sList->Strings[3].c_str());
                    }

                    if(sPrefix=="GV")
                    {
                        if(USE_MR_SYSTEM)
                            iTrayIDDataCount=5;
                        else
                            iTrayIDDataCount=4;
                        if(sList->Count>=iTrayIDDataCount)
                        {
                            if(bTrayMapGetResultOK[Tag]==false)
                            {
                                bResult=CCDTrayID_Decoding(Tag);
                                if(bResult)
                                {
                                    bTrayMapGetResultOK[Tag]=true;
                                    sReply.sprintf("Tray ID GVOK : %s", sDownCCDRecevieData[Tag]);
                                }
                                else
                                {
                                    sReply.sprintf("Tray ID GVNG : %s", sDownCCDRecevieData[Tag]);
                                }
                            }
                            else
                            {
                                sReply.sprintf("Tray ID GVTO : %s", sDownCCDRecevieData[Tag]);  //wei 20161220  ????
                            }
                        }
                        else
                        {
                            sReply.sprintf("Tray ID GVNG : %s", sDownCCDRecevieData[Tag]);
                        }
                    }
                    else
                    {
                        if(USE_MR_SYSTEM)
                        {
                            if(ich==0)
                                sReply.sprintf("Tray ID %s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
                            else
                                sReply.sprintf("Tray ID2 %s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
                        }
                        else
                        {
                            sReply.sprintf("Tray ID %s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
                        }

                        if(sPrefix=="E1")
                        {
                            if(bTrayMapExposureOK[Tag]==false)
                            {
                                bTrayMapExposureOK[Tag]=true;
                            }
                        }
                        else if(sPrefix=="E9")          //Reset全部資料
                        {
                            bGetTraySE9[Tag]=true;
                        }
                        else if(sPrefix=="GS")          //取得狀態
                        {
                            ;
                        }
                        else if(sPrefix=="CF")          //產生工作檔資料
                        {
                            bGetCF=true;
                        }
                        else if(sPrefix=="LF")          //設定工作檔
                        {
                            bGetLF=true;
                            bChangeBarcodeFileOK=true;
                        }
                        else if(sPrefix=="FL")          //取得檔案列表
                        {
                            ;
                        }
                        else if(sPrefix=="GF")          //取得工作檔名
                        {
                            if(sList->Count>=4)
                                sSFCCurrentGF=sList->Strings[3];
                            else
                                sSFCCurrentGF="";
                            bGetGF=true;
                        }
                        else if(sPrefix=="SF")          //存檔
                        {
                            ;
                        }
                    }
                }
                //----------------------------
                // TRAY MAPPING    //  Tray Decive Count1~3
                //----------------------------
                else if(iFunction==4)
                {
                    if(sPrefix=="GV")
                    {
                        if(sList->Count>=4)
                        {
                            if(bTrayMapGetResultOK[Tag]==false)
                            {
                                bResult=CCDTrayMap_Decoding(Tag);
                                if(bResult)
                                {
                                    bTrayMapGetResultOK[Tag]=true;
                                    sReply.sprintf("%s GVOK : %s",TrayPos[Tag], sDownCCDRecevieData[Tag]);
                                }
                                else
                                {
                                    sReply.sprintf("%s GVNG : %s",TrayPos[Tag], sDownCCDRecevieData[Tag]);
                                }
                            }
                            else
                            {
                                sReply.sprintf("%s GVTO : %s",TrayPos[Tag], sDownCCDRecevieData[Tag]);
                            }
                        }
                        else
                        {
                            sReply.sprintf("%s GVNG : %s",TrayPos[Tag], sDownCCDRecevieData[Tag]);
                        }
                    }
                    else
                    {
                        sReply.sprintf("%s %s %s, %s",TrayPos[Tag],   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]); //Sam 20190405 : Tray Decive Count
                        /*--------------------------------------------------------------
                        //  回傳格式: E0,1,1
                        //  內文說明: E0,[功能],[成功與否],[位置]
                        //------------------------------------------------------------*/
                        if(sPrefix=="E0" || sPrefix=="E1" || sPrefix=="E2" || sPrefix=="E3")
                        {
                            if(sList->Count>=3)                                         //Steven 20160503 : 確認移動位置與拍照位置相同
                            {
                                iPosition=atoi(sList->Strings[3].c_str());
                                if(iPosition!=iCurrentStep[Tag])
                                {
                                    Str.sprintf("Alarm, Current step %d compare with exposure position %d is different!", iCurrentStep[Tag], iPosition);    //Steven 20160512 : 2D log排版
                                    AddCCDTrayCommunicationLog(Tag, Str);
                                    bStepError[Tag]=true;
                                }
                            }

                            if(bStepError[Tag]==false && bTrayMapExposureOK[Tag]==false)
                            {
                                bTrayMapExposureOK[Tag]=true;
                            }
                        }
                        else if(sPrefix=="E5")          //調整光源
                            ;
                        else if(sPrefix=="E8")          //Reset單筆資料
                            ;
                        else if(sPrefix=="E9")          //Reset全部資料
                            bGetTraySE9[Tag]=true;
                        else if(sPrefix=="GS")          //取得狀態
                            ;
                        else if(sPrefix=="LF")          //設定工作檔
                        {
                            bGetLF=true;
                            bChangeBarcodeFileOK=true;
                        }
                        else if(sPrefix=="SF")          //儲存檔案
                            bGetSF=true;
                        else if(sPrefix=="FL")          //取得檔案列表
                            ;
                        else if(sPrefix=="CF")          //產生工作檔資料
                            bGetCF=true;
                        else if(sPrefix=="GF")          //取得工作檔名
                        {
                            if(sList->Count>=4)
                                sCurrentGF=sList->Strings[3];
                            else
                                sCurrentGF="";
                            bGetGF=true;
                        }
                        else if(sPrefix=="SF")          //存檔
                            ;
                    }
                }
                else if(iFunction==5)       //wei 20170317 (steven) Device Remain 殘料檢測
                {
                    if(sPrefix=="GV")
                    {
                        if(sList->Count>=2)
                        {
                            if(sList->Count>=4)
                            {
                                sTrayMapFileName[Tag]=sList->Strings[4];    //Sam 20200211 : Vision Tray Map 通訊增加 圖檔名紀錄
                            }
                            if(bTrayMapGetResultOK[Tag]==false)
                            {
                                if(atoi(sList->Strings[2].c_str())==1)
                                {
                                    bTrayMapGetResultOK[Tag]=true;
                                    sReply.sprintf("%s GVOK : %s",TrayPos[Tag+1], sDownCCDRecevieData[Tag]);
                                }
                                else
                                {
                                    iDeviceRemainCount=0;
                                    if(sList->Count>=3) //Sam 20191115 : 殘留 IC 數量
                                    {
                                        iDeviceRemainCount=atoi(sList->Strings[3].c_str());
                                    }

                                    bTrayHaveDevice=true;
                                    sReply.sprintf("%s GVNG : %s",TrayPos[Tag+1], sDownCCDRecevieData[Tag]);
                                }
                            }
                            else
                            {
                                sReply.sprintf("%s GVTO : %s",TrayPos[Tag+1], sDownCCDRecevieData[Tag]);
                            }
                        }
                        else
                        {
                            sReply.sprintf("%s GVNG : %s",TrayPos[Tag+1], sDownCCDRecevieData[Tag]);
                        }
                    }
                    else
                    {
                        sReply.sprintf("IC Remain %s %s, %s",   sPrefix, (iResult==1)?"OK":"NG", sDownCCDRecevieData[Tag]);
                        /*--------------------------------------------------------------
                        //  回傳格式: E0,1,1
                        //  內文說明: E0,[功能],[成功與否],[位置]
                        //------------------------------------------------------------*/
                        if(sPrefix=="E0" || sPrefix=="E1" || sPrefix=="E2" || sPrefix=="E3")
                        {
                            if(sList->Count>=3)                                         //Steven 20160503 : 確認移動位置與拍照位置相同
                            {
                                iPosition=atoi(sList->Strings[3].c_str());
                                if(iPosition!=iCurrentStep[Tag])
                                {
                                    Str.sprintf("Alarm, Current step %d compare with exposure position %d is different!", iCurrentStep[Tag], iPosition);    //Steven 20160512 : 2D log排版
                                    AddCCDTrayCommunicationLog(Tag, Str);
                                    bStepError[Tag]=true;
                                }
                            }

                            if(bStepError[Tag]==false && bTrayMapExposureOK[Tag]==false)
                            {
                                bTrayMapExposureOK[Tag]=true;
                            }
                        }
                        else if(sPrefix=="E5")          //調整光源
                            ;
                        else if(sPrefix=="E8")          //Reset單筆資料
                            ;
                        else if(sPrefix=="E9")          //Reset全部資料
                            bGetTraySE9[Tag]=true;
                        else if(sPrefix=="GS")          //取得狀態
                            ;
                        else if(sPrefix=="LF")          //設定工作檔
                        {
                            bGetLF=true;
                            bChangeBarcodeFileOK=true;
                        }
                        else if(sPrefix=="SF")          //儲存檔案
                            bGetSF=true;
                        else if(sPrefix=="FL")          //取得檔案列表
                            ;
                        else if(sPrefix=="CF")          //產生工作檔資料
                            bGetCF=true;
                        else if(sPrefix=="GF")          //取得工作檔名
                        {
                            if(sList->Count>=4)
                                sCurrentGF=sList->Strings[3];
                            else
                                sCurrentGF="";
                            bGetGF=true;
                        }
                        else if(sPrefix=="SF")          //存檔
                            ;
                    }
                }
            }
            if(iFunction==5 && Tag==iTray_Map)
            {
                SendCCDTrayCommand(iDeviceRemain, sReply);
            }
            else
            {
                SendCCDTrayCommand(Tag, sReply);
            }
            sList->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete sList;
        }
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::CCDTrayID_Decoding(int Channel)
{
    bool bResult=true;
    AnsiString Data="", Data1="";
    int iResult=-1;
    int iTrayIDDataCount=0;

    TStringList *sList=new TStringList();
    if(USE_MR_SYSTEM)
        iTrayIDDataCount=4;
    else
        iTrayIDDataCount=3;

    if(cbBarCodeSimulate->Checked)
    {
        if(Channel==0)
        {
            sList->CommaText=Edit2->Text.c_str();
            WriteTrayMapLog(3, Edit2->Text);
            Data=sList->Strings[iTrayIDDataCount];
            asTrayIDData1=Data;
            labTrayID->Caption=Data;
        }
        else
        {
            Data1.sprintf("GV,3,1,1,%d",rand());
            Edit4->Text=Data1.c_str();
            sList->CommaText=Edit4->Text.c_str();
            WriteTrayMapLog(8, Edit4->Text);
            Data=sList->Strings[iTrayIDDataCount];
            asTrayIDData2=Data;
            labTrayID2->Caption=Data;
        }
    }
    else
    {
        if(Channel==0)
        {
            sList->CommaText=sDownCCDRecevieData[Channel];
            WriteTrayMapLog(3, sDownCCDRecevieData[Channel]);
        }
        else
        {
            sList->CommaText=sDownCCDRecevieData[Channel];
            WriteTrayMapLog(8, sDownCCDRecevieData[Channel]);
        }
//        sList->CommaText=Edit2->Text.c_str();;
//        WriteTrayMapLog(3, Edit2->Text);
    }
    if(sList->Count>=iTrayIDDataCount+1)
    {
        iResult=atoi(sList->Strings[iTrayIDDataCount-1].c_str());
        if(iResult==1)
        {
            Data=sList->Strings[iTrayIDDataCount];

            if(Data.Length()>TestIF_File.iTrayCodeMinLength && Data.Length()<=TestIF_File.iTrayCodeMaxLength)
            {
                if(Channel==0)
                {
                    asTrayIDData1="";
                    asTrayIDData1=sList->Strings[iTrayIDDataCount];
                    labTrayID->Caption=Data;
                }
                else
                {
                    asTrayIDData2="";
                    asTrayIDData2=sList->Strings[iTrayIDDataCount];
                    labTrayID2->Caption=Data;
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
    }
    else
        bResult=false;
    sList->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sList;
    return bResult;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::CCDTrayMap_Decoding(int Channel)
{
    AnsiString dataSplit;
    bool bResult=true;
    int iListCount;
    AnsiString asTraySize;
    int iTrayRow, iTrayCol;
    int ilen=0, ilen2=0, ilen3=0;
    int iResult=-1;
    AnsiString str="" , asTrayData;
    AnsiString sDeviceCnt[]={"AutoData1","AutoData2","AutoData3"};

    TStringList *sList  =new TStringList();
    ZeroMemory(iTrayMappingDate, sizeof(iTrayMappingDate));

    AnsiString Path;
    int iVisionTrayCnt=0;

    if(Channel==iTray_Map)  //Tray Map
    {
        Path.sprintf("%s\\%04d_%02d_%02d\\", asTrayMapDataPath, SystemYear, SystemMonth, SystemDate);
        MyForceDirectories(Path, "TfTrayMapping::CCDTrayMap_Decoding_1");

        if(asTrayIDData1!="")
            Path.sprintf("%s\\%04d_%02d_%02d\\%s.txt", asTrayMapDataPath, SystemYear, SystemMonth, SystemDate, asTrayIDData1);
        else
            Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d%02d%02d.txt", asTrayMapDataPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour, SystemMin, SystemSec);
    }
    else if(Channel==iTray_DeviceCnt1 || Channel==iTray_DeviceCnt2 || Channel==iTray_DeviceCnt3)
    {
        Path.sprintf("%s\\%s\\%04d_%02d_%02d\\", asTrayMapLogPath, sDeviceCnt[Channel-iTray_DeviceCnt1], SystemYear, SystemMonth, SystemDate);
        MyForceDirectories(Path, "TfTrayMapping::CCDTrayMap_Decoding_2");
        if(asTrayIDData1!="")
            Path.sprintf("%s\\%s\\%04d_%02d_%02d\\%s.txt", asTrayMapLogPath, sDeviceCnt[Channel-iTray_DeviceCnt1], SystemYear, SystemMonth, SystemDate, asTrayIDData1);
        else
            Path.sprintf("%s\\%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d%02d%02d.txt", asTrayMapLogPath, sDeviceCnt[Channel-iTray_DeviceCnt1], SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour, SystemMin, SystemSec);
    }
    if(cbBarCodeSimulate->Checked && Channel==iTray_Map )
    {
        str.sprintf("GV,4,1,%d_%d,",MOT[MMTrayY].Tray.XItem,MOT[MMTrayY].Tray.YItem);
        for(int i=0; i<MOT[MMTrayY].Tray.YItem*MOT[MMTrayY].Tray.XItem; i++)
        {
            if(rand()%2==0)
                str+=0;
            else
                str+=1;
        }

        sList->CommaText=str.c_str();
        WriteTrayMapLog(4, str.c_str());
    }
    else if(Channel==iTray_Map)
    {
        sList->CommaText=sDownCCDRecevieData[Channel];
        WriteTrayMapLog(4, sDownCCDRecevieData[Channel]);
    }
    else if(Channel==iTray_DeviceCnt1 || Channel==iTray_DeviceCnt2 || Channel==iTray_DeviceCnt3)
    {
        sList->CommaText=sDownCCDRecevieData[Channel];
        WriteTrayMapLog(Channel+5, sDownCCDRecevieData[Channel]);     //Auto1=9 ,Auto2=10, Auto3=11
    }
    iListCount=sList->Count;

    //--------------------
    //回傳格式
    //GV,0,1,11111111
    //--------------------
    if(iListCount>3)
        iResult=atoi(sList->Strings[2].c_str());

    if(iListCount>4)
        asTraySize=sList->Strings[3];

    ilen=sList->Strings[3].Pos("_");
    ilen2=sList->Strings[3].Length();
    iTrayCol=atoi(sList->Strings[3].SubString(1, ilen-1).c_str());
    iTrayRow=atoi(sList->Strings[3].SubString(ilen+1, ilen2).c_str());

    if(iResult!=1)                 //wei 20161220 error
    {
        bResult=false;
    }
    else
    {
        if(iListCount>=5)
        {
            if(iListCount>=6)
            {
                sTrayMapFileName[Channel]=sList->Strings[5].c_str();    //Sam 20200211 : Vision Tray Map 通訊增加 圖檔名紀錄
            }
            dataSplit=sList->Strings[4];
            ilen3=dataSplit.Length();

            if(ilen3==iTrayCol*iTrayRow && iTrayRow==MOT[MMTrayY].Tray.YItem && iTrayCol==MOT[MMTrayY].Tray.XItem)
            {
                if(Channel==iTray_Map)
                {
                    iVisionTrayCnt=0;
                    for(int j=0; j<iTrayRow; j++)
                    {
                        asTrayData="";
                        for(int i=0; i<iTrayCol; i++)
                        {
                            iTrayMappingDate[j][i]=dataSplit[1+i+iTrayCol*j];
                            sgTraymap->Cells[i+1][j+1]=MyDeCodeASCII(iTrayMappingDate[j][i]);
                            asTrayData+=MyDeCodeASCII(iTrayMappingDate[j][i]);
                            if(MyDeCodeASCII(iTrayMappingDate[j][i])=="1")
                            {
                                iVisionTrayCnt++;
                            }
                        }
                        WriteDataToFile(Path.c_str() , asTrayData.c_str());
                    }
                    //NewRecordProcess("","Tray Loader,Count Pass,Handler get load tray map form Vision System OK","IC Count:Vision-"+IntToStr(iVisionTrayCnt)+",Handler-"+IntToStr(iTrayRow*iTrayCol));
                    NewRecordProcess("","Tray Loader,Count Pass,Handler get load tray map form Vision System OK","IC Count:"+IntToStr(iVisionTrayCnt)+"/"+IntToStr(iTrayRow*iTrayCol)+",File:"+sTrayMapFileName[Channel]);
                }
                else    //Sam 20190405 : Tray Decive Count
                {
                    if(Channel==iTray_DeviceCnt1)
                    {
                        for(int j=0; j<iTrayRow; j++)
                        {
                            asTrayData="";
                            for(int i=0; i<iTrayCol; i++)
                            {
                                iTrayDeviceCountDate[0][j][i]=dataSplit[1+i+iTrayCol*j];
                                sgTrayDeviceCnt1->Cells[i+1][j+1]=MyDeCodeASCII(iTrayDeviceCountDate[0][j][i]);
                                asTrayData+=MyDeCodeASCII(iTrayDeviceCountDate[0][j][i]);
                            }
                            WriteDataToFile(Path.c_str() , asTrayData.c_str());
                        }
                    }
                    else if(Channel==iTray_DeviceCnt2)
                    {
                        for(int j=0; j<iTrayRow; j++)
                        {
                            asTrayData="";
                            for(int i=0; i<iTrayCol; i++)
                            {
                                iTrayDeviceCountDate[1][j][i]=dataSplit[1+i+iTrayCol*j];
                                sgTrayDeviceCnt2->Cells[i+1][j+1]=MyDeCodeASCII(iTrayDeviceCountDate[1][j][i]);
                                asTrayData+=MyDeCodeASCII(iTrayDeviceCountDate[1][j][i]);
                            }
                            WriteDataToFile(Path.c_str() , asTrayData.c_str());
                        }
                    }
                    else if(Channel==iTray_DeviceCnt3)
                    {
                        for(int j=0; j<iTrayRow; j++)
                        {
                            asTrayData="";
                            for(int i=0; i<iTrayCol; i++)
                            {
                                iTrayDeviceCountDate[2][j][i]=dataSplit[1+i+iTrayCol*j];
                                sgTrayDeviceCnt3->Cells[i+1][j+1]=MyDeCodeASCII(iTrayDeviceCountDate[2][j][i]);
                                asTrayData+=MyDeCodeASCII(iTrayDeviceCountDate[2][j][i]);
                            }
                            WriteDataToFile(Path.c_str() , asTrayData.c_str());
                        }
                    }
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
    }
    sList->Clear(); //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sList;
    return bResult;;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::Button1Click(TObject *Sender)
{
    if(TestIF_File.bEnableTrayMap==true)
        CCDTrayMap_Decoding(1);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::Button2Click(TObject *Sender)
{
    if(TestIF_File.bEnableTrayID==true && USE_KEYENCE_LOADER==0)
        CCDTrayID_Decoding(0);
}
//------------------------------------------------------------------------------
int  iInitialTrayIDTask=1;
//------------------------------------------------------------------------------
void InitialTrayIDTask()                                                        //wei 20161219 Tray Mapping
{
    iInitialTrayIDTask=1;
    bCCDTrayID[0]=true;
}
//------------------------------------------------------------------------------
bool TfTrayMapping::DoTrayIDCCD()
{
    int &Task=iInitialTrayIDTask;
    bool bResult=false;
    int ret, pos=0, iPosX=0, iPosY=0;
    int iTrayMap=iTray_ID;
    static int iRetryCount=0;
    static int iRetryCountExpTimo=0;
    static bool bHasError=false;
    AnsiString sCCDCommand;
    AnsiString Log;
    AnsiString DownName,DownName1,sKeyinTrayID;

    switch(Task)
    {
        case 1:
            AddCCDTrayCommunicationLog(iTrayMap, "<<Tray ID Cycle, Cycle start.------------------");

            bTrayMapExposureOK[iTrayMap]=true;
            bTrayMapGetResultOK[iTrayMap]=true;
            iRetryCountExpTimo=0;
            bFTPDownLoadFileError=false;

            bHasError=false;
            if(bCCDTrayID[0])                        //wei 20161102 IC置偏檢查
            {
                iRetryCount=0;
                bCCDTrayID[0]=false;
            }
            if(USE_MR_SYSTEM)
                sCCDCommand.sprintf("E9,3,0");   //清空暫存器
            else
                sCCDCommand.sprintf("E9,3");   //清空暫存器
            bGetTraySE9[iTrayMap]=false;
            SendCCDTrayCommand(iTrayMap, "Tray ID Clear buffer 1", sCCDCommand);     //Reset CCD Shuttle暫存器
            TMAStartDelay[iTrayMap].SetMSAndOn(5000);
            Task=500;
        case 500:       //確認Buffer清空
            if(bGetTraySE9[iTrayMap])
            {
                Task=900;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                Task=900;
            }
            else if(TMAStartDelay[iTrayMap].Off())
            {
                ShowErrorMessage("WAR0970", K_RETRY, MTrayX, false, "Tray ID Clear buffer");
                Task=1;
            }
            break;
        case 900:
            if(IsMoveInArm2XYToWait())
            {
                iPosX=MOT[MInArmX].ReadPos();
                iPosY=MOT[MInArmY].ReadPos();
                Log.sprintf("InArm X:%d,InArm Y:%d",iPosX, iPosY);   //InArm X:1527,InArm Y:-46666
                AddCCDTrayCommunicationLog(iTrayMap, Log);
                Task=1000;
            }
            else
            {
                ShowMyMessage("InArm X/Y no move Wait");
                MOT[MInArmX].fCanMove=true;
                MOT[MInArmY].fCanMove=true;
                Task=950;
            }
            break;
        case 950:
            if(MoveInArm2XYToWait())
            {
                MOT[MInArmX].fCanMove=false;
                MOT[MInArmY].fCanMove=false;
                Task=900;
            }
            break;
        case 1000:      //移動到拍照位置
            pos=Prod.iTrayID+TestIF_File.iTrayIDShift;  //wei 20170608

            #ifdef SOFT_SIMULTE
                Log.sprintf("Tray ID Action, Tray Arm move to step Tray");
                AddCCDTrayCommunicationLog(iTrayMap, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            #endif
            if(TrayArmMotorMove(pos, false))    //Sam 20241206 : Tray Arm 新增 Teach 保護
            {
                Log.sprintf("Tray ID Action, Tray Arm move to step Tray");
                AddCCDTrayCommunicationLog(iTrayMap, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            }
            break;
        case 1120:
            if(TMAStartDelay[iTrayMap].Off())    //到位Delay
            {
                bTrayMapExposureOK[iTrayMap]=false;
                bTrayError[iTrayMap]=false;
                //--------------------------------------
                //E1  , 3
                //拍照 Tray ID
                //--------------------------------------
                if(USE_MR_SYSTEM)
                    sCCDCommand.sprintf("E1,3,0");
                else
                    sCCDCommand.sprintf("E1,3");

                labTrayIDStatus->Caption="Tray ID Exposure...";
                SendCCDTrayCommand(iTrayMap, "Tray ID exposure 1120: ", sCCDCommand);

                Task=1150;
                iRetryCountExpTimo=0;        //Steven 20160425 : 修改拍照Time Out
                TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
            }
            break;
        case 1150:
            if(bTrayMapExposureOK[iTrayMap]==true)    //拍照完成
            {
                labTrayIDStatus->Caption="Waiting result..";
                Task=1200;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                Task=1200;
            }
            else if(TMAExposureDelay[iTrayMap].Off() || bTrayError[iTrayMap])   //拍照time out
            {
                if(bTrayError[iTrayMap])
                {
                    labTrayIDStatus->Caption="Exposure time out";
                    Log.sprintf("Tray ID Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDTrayCommunicationLog(iTrayMap, Log);
                }
                else
                {
                    labTrayIDStatus->Caption="Waiting result..";
                }

                iRetryCountExpTimo++;        //Steven 20160425 : 修改拍照Time Out
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
                ret=ShowErrorMessage("WAR0971", K_RETRY|K_SKIP, MTrayX);  //2D function CCD 3 or 4 Exposure time out!
                if(ret==K_SKIP)
                {
//                    Task=2000;
//                    bHasError=false;
                    if(TestIF_File.bEnableTrayIDDownFTP)        //wei 20170518
                    {
                        asTrayIDData1 = ShowMyInputTrayID("Tray ID" , "Please Keyin Tray ID : ");
                        sKeyinTrayID.sprintf("Tray ID exposure time out--SKIP Keyin %s",asTrayIDData1);
                        WriteTrayMapLog(3, sKeyinTrayID.c_str());
                        SearchTrayMapFile();
                        Task=3500;
                    }
                    else
                    {
                        Task=5000;
                    }
                }
                else
                {
                    sKeyinTrayID.sprintf("Tray ID exposure time out--RETRY reset");
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    Task=1;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            if(USE_MR_SYSTEM)
                sCCDCommand.sprintf("E1,3,0");
            else
                sCCDCommand.sprintf("E1,3");
            SendCCDTrayCommand(iTrayMap, "Tray ID exposure 1170: ", sCCDCommand);

            Task=1150;
            TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
            break;
        case 1200:
            if(USE_MR_SYSTEM)
                sCCDCommand="GV,3,0";       //Ifor 20151228 取得 CCD 讀取資料
            else
                sCCDCommand="GV,3";       //Ifor 20151228 取得 CCD 讀取資料

            bTrayMapGetResultOK[iTrayMap]=false;
            SendCCDTrayCommand(iTrayMap, "Tray ID get result 1200: ", sCCDCommand);

            bHasError=false;
            Task=1350;
            TMAResultDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            break;
        case 1350:
            if(bTrayMapGetResultOK[iTrayMap]==true) //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                CCDTrayID_Decoding(0);
                Task=2000;
            }
            else if(TMAResultDelay[iTrayMap].Off())  //讀取超時
            {
                if(bTrayMapGetResultOK[iTrayMap]==false)
                {
                    bTrayMapGetResultOK[iTrayMap]=true;
                    labTrayIDStatus->Caption="Inspection time Out";
                    AddCCDTrayCommunicationLog(iTrayMap, "Tray ID Action 1350, Inspection time out.");
                    bHasError=true;
                }

                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iTrayAutoRetry)                       //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    Task=2500;
                }
                bHasError=false;
            }
            else
            {
                labTrayIDStatus->Caption="Tray ID Inspection end !";
                AddCCDTrayCommunicationLog(iTrayMap, "Tray ID >>Cycle 2000, Inspection end.---------------\r\n");
                if(TestIF_File.bEnableTrayIDDownFTP)        //wei 20170518
                {
                    Task=3500;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 2500:
            labTrayIDStatus->Caption="Tray ID Inspection error !";
            Log.sprintf("Tray ID Alarm 2500, Tray ID read error");
            AddCCDTrayCommunicationLog(iTrayMap, Log);
            ret=ShowErrorMessage("WAR0973", K_RETRY|K_SKIP, MTrayX, false);
            iRetryCount=0;
            bHasError=false;
            if(ret==K_RETRY)
            {
                sKeyinTrayID.sprintf("Tray ID exposure Result error--RETRY reset");
                WriteTrayMapLog(3, sKeyinTrayID.c_str());
                Task=1;
            }
            else
            {
                if(TestIF_File.bEnableTrayIDDownFTP)        //wei 20170518
                {
                    asTrayIDData1 = ShowMyInputTrayID("Tray ID" , "Please Keyin Tray ID : ");
                    sKeyinTrayID.sprintf("Tray ID exposure Result error--SKIP Keyin %s",asTrayIDData1);
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    SearchTrayMapFile();
                    Task=3500;
                }
                else
                {
                    Task=5000;
                }
            }
            break;
        case 3500:
            if(DirectoryExists("Z:\\")==false)  //Ifor 20170202 (wei) add KYEC ATC 網路磁碟機未連線直接Alarm
            {
//                ShowMyMessage("Tray map Route Z: Disk Fail!!");  //Steven 20091004
                ret=ShowErrorMessage("WAR0976", K_RETRY|K_SKIP, MTrayX, false);
                if(ret==K_RETRY)
                {
                    sKeyinTrayID.sprintf("Tray map Route Z: Disk Fail--RETRY connect network");
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    fTrayMapping->AutoConnectNetwork();    //Ifor 20170202 自動連線ATC網路芳鄰
                }
                else
                {
                    bFTPDownLoadFileError=true;
                    sKeyinTrayID.sprintf("Tray map Route Z: Disk Fail--SKIP Full Tray");
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    Task=5000;
                }
            }
            else
            {
                TStringList *TestList=new TStringList;
                AnsiString str, str2;
                AnsiString asDataSource;

                asTrayIDFile=fLotInfo->edtSysLotID->Text;
                asTrayIDFile=asTrayIDFile.SubString(1,6);

                str2.sprintf("%s_%s.txt",asTrayIDFile,asTrayIDData1);
                WriteTrayMapLog(5, str2.c_str());
//                asDataPurpose="D:\\HT9045_Log\\TrayMapLog\\";
                //取得ATC來源資料名稱
                asDataSource.sprintf("Z:\\%s\\%s_%s.txt",asTrayIDFile,asTrayIDFile,asTrayIDData1);
                //複製來源檔案至目的位置
                str.sprintf("XCOPY /y/a/e/c/i/h/f/r %s %s\\", asDataSource, asTrayMapLogPath);
                TestList->Add(str);

                TestList->SaveToFile("D:\\HT9045\\EVENLOG.bat");                //Ifor 20160408 (Steven) 建立檔案與寫入檔案
                ExecZipCommand("D:\\HT9045\\EVENLOG.bat", " ");                 //Ifor 20160408 (Steven) 避免複製檔案時出現Dos執行畫面
                MySleep(1000);
                TestList->Clear();
                delete TestList;

                Task=4000;
            }
            break;
        case 4000:   //讀檔      //wei 20170518
            DownName1.sprintf("%s\\%s_%s.txt",asTrayMapLogPath,asTrayIDFile,asTrayIDData1);
            if(FileExists(DownName1.c_str())==false)
            {
//                ShowMyMessage("Data count no OK!", "檔案不存在!");
                ret=ShowErrorMessage("WAR0974", K_RETRY|K_SKIP, MTrayX, false);
                if(ret==K_RETRY)        //wei 20170518
                {
                    asTrayIDData1 = ShowMyInputTrayID("Tray ID" , "Please Keyin Tray ID : ");
                    sKeyinTrayID.sprintf("Tray ID Data no exist--RETRY Keyin %s",asTrayIDData1);
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    SearchTrayMapFile();
                    Task=3500;
                }
                else
                {
                    bFTPDownLoadFileError=true;
                    sKeyinTrayID.sprintf("Tray ID Data no exist--SKIP Full Tray");
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    Task=5000;
                }
            }
            else if(DoReadTrayData(DownName1.c_str()))
            {
                Task=5000;
            }
            else
            {
                ret=ShowErrorMessage("WAR0975", K_RETRY|K_SKIP, MTrayX, false);
                if(ret==K_RETRY)
                {
                    sKeyinTrayID.sprintf("Tray ID Data count no OK--RETRY reset");
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    Task=1;
                }
                else
                {
                    bFTPDownLoadFileError=true;
                    sKeyinTrayID.sprintf("Tray ID Data count no OK--SKIP Full Tray");
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    Task=5000;
                }
            }
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
int  iInitialTrayMapTask=1;
//------------------------------------------------------------------------------
void InitialTrayMapTask(int iTrayMap)     //Sam 20200323 : Modify Tray Function  //wei 20161219 Tray Mapping
{
    iInitialTrayMapTask=1;
    bClearRetryCnt[iTrayMap]=true;
    if(iTrayMap==iDeviceRemain)
    {
        bClearRetryCnt[iTray_Map]=true;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoTrayMapCCD(int iTrayType) //Sam 20200323 : Modify Tray Function
{
    int &Task=iInitialTrayMapTask;
    bool bResult=false;
    int ret, pos=0, iPosX=0, iPosY=0;
    int iFunction=0;
    int iAuto=0;
    int iTrayMap=0;
    static int iNowMoveStep=0;
    static int iRetryCount=0;
    static int iRetryCountExpTimo=0;
    static bool bHasError=false;

    AnsiString sCCDCommand;
    AnsiString Log;

    if(iTrayType==iTray_Map)                //Loader Tray Map
    {
        iFunction=4;
        iTrayMap=iTrayType;
    }
    else if(iTrayType==iDeviceRemain)       //Loader IC Remain
    {
        iFunction=5;
        iTrayMap=iTray_Map;
    }
    else if(iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)   //Unload Auto1~3 Device Count
    {
        iFunction=4;
        iTrayMap=iTrayType;
        iAuto=iTrayType-iTray_DeviceCnt1;
    }

    switch(Task)
    {
        case 1:
            AddCCDTrayCommunicationLog(iTrayType, "<<Tray Cycle, Cycle start.------------------");
            iNowMoveStep=0;
            iRetryCountExpTimo=0;
            iCurrentStep[iTrayMap]=iNowMoveStep;
            bTrayMapExposureOK[iTrayMap]=true;
            bTrayMapGetResultOK[iTrayMap]=true;
            bStepError[iTrayMap]=false;
            bHasError=false;
            bTrayHaveDevice=false;  //IC Remain 使用
            if(bClearRetryCnt[iTrayMap])
            {
                iRetryCount=0;
                bClearRetryCnt[iTrayMap]=false;
            }
            //--------------------------------------
            //Loader Tray Map   E9,4
            //Loader IC Remain  E9,5
            //Unload Auto1~3    E9,4
            //--------------------------------------
            sCCDCommand.sprintf("E9,%d",iFunction);   //清空暫存器
            bGetTraySE9[iTrayMap]=false;
            SendCCDTrayCommand(iTrayType, "Clear buffer 1", sCCDCommand);     //Reset CCD Shuttle暫存器
            TMAStartDelay[iTrayMap].SetMSAndOn(5000);
            Task=500;
        case 500:       //確認Buffer清空
            if(bGetTraySE9[iTrayMap] || cbBarCodeSimulate->Checked)
            {
                if(iTrayType==iTray_Map)
                {
                    Task=900;       //Loader Tray Map In Arm 讓位動作
                }
                else if(iTrayType==iDeviceRemain)
                {
                    Task=1000;
                }
                else if(iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)
                {
                     Task=950;      //Unload 點量 OutArm 讓位動作
                }
            }
            else if(TMAStartDelay[iTrayMap].Off())
            {
                ShowTrayFunErrMsg(0,iTrayType);
                Task=1;
            }
            break;
        case 900:    //Loader Tray Map In Arm 讓位動作
            if(IsMoveInArm2XYToWait())
            {
                iPosX=MOT[MInArmX].ReadPos();
                iPosY=MOT[MInArmY].ReadPos();
                Log.sprintf("InArm X:%d,InArm Y:%d",iPosX, iPosY);   //InArm X:1527,InArm Y:-46666
                AddCCDTrayCommunicationLog(iTrayType, Log);
                Task=1000;
            }
            else
            {
                ShowMyMessage("InArm X/Y no move Wait");
                MOT[MInArmX].fCanMove=true;
                MOT[MInArmY].fCanMove=true;
                Task=910;
            }
            break;
        case 910:
            if(MoveInArm2XYToWait())
            {
                MOT[MInArmX].fCanMove=false;
                MOT[MInArmY].fCanMove=false;
                Task=900;
            }
            break;
        case 950:       //Unload 點量 OutArm 讓位動作
            if(IsOutArmSafe())
            {
                if(TRAY_ARM_MODE==eAboveCoveyor)
                {
                    iPosX=MOT[MOutArmX].ReadPos();
                    iPosY=MOT[MOutArmY].ReadPos();
                    Log.sprintf("OutArm X:%d,OutArm Y:%d",iPosX, iPosY);   //InArm X:1527,InArm Y:-46666
                    AddCCDTrayCommunicationLog(iTrayType, Log);
                }
                Task=1000;
            }
            else
            {
                ShowMyMessage("OutArm X/Y no move Wait");
                MOT[MOutArmX].fCanMove=true;
                MOT[MOutArmY].fCanMove=true;
                Task=960;
            }
            break;
        case 960:
            if(MoveOutArmXY_ToFix_Tray_Full())
            {
                MOT[MOutArmX].fCanMove=false;
                MOT[MOutArmY].fCanMove=false;
                Task=950;
            }
            break;
        case 1000:      //移動到拍照位置
            if(iTrayType==iTray_Map || iTrayType==iDeviceRemain)  //Tray Map & IC Remain 跑一樣的位置
            {
                pos=Prod.iTrayMapping+TestIF_File.iTrayMapStart+(LoadForm->XPitch*iNowMoveStep);
            }
            else if(iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)
            {
                pos=Prod.iTrayDeviceCnt[iAuto]+TestIF_File.iTrayDeciveCntStart+(LoadForm->XPitch*iNowMoveStep);
            }
            #ifdef SOFT_SIMULTE
                Log.sprintf("Tray Map Action, Tray Arm move to step %d", iNowMoveStep);
                AddCCDTrayCommunicationLog(iTrayType, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            #endif
            if(TrayArmMotorMove(pos, false))    //Sam 20241206 : Tray Arm 新增 Teach 保護
            {
                Log.sprintf("Tray Map Action, Tray Arm move to step %d", iNowMoveStep);
                AddCCDTrayCommunicationLog(iTrayType, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            }
            break;
        case 1120:
            if(TMAStartDelay[iTrayMap].Off())    //到位Delay
            {
                bTrayMapExposureOK[iTrayMap]=false;
                bTrayError[iTrayMap]=false;
                //--------------------------------------
                //E1  , 4       , X
                //拍照 Tray Map 位置
                //Loader Tray Map  E1,4,X
                //Loader IC Remain E1,5,X
                //Unload Auto1~3   E1,4,X
                //--------------------------------------
                sCCDCommand.sprintf("E1,%d,%d", iFunction, iNowMoveStep);
                labTrayMapStatus->Caption="Tray Map Exposure...";
                SendCCDTrayCommand(iTrayType, "Tray Map exposure 1120: ", sCCDCommand);
                Task=1150;
                iRetryCountExpTimo=0;        //Steven 20160425 : 修改拍照Time Out
                TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
            }
            break;
        case 1150:
            if(bStepError[iTrayMap]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                Task=1180;
            }
            else if(bTrayMapExposureOK[iTrayMap]==true)    //拍照完成
            {
                labTrayMapStatus->Caption="Waiting result..";
                Task=1200;
            }
            else if(TMAExposureDelay[iTrayMap].Off() || bTrayError[iTrayMap])   //拍照time out
            {
                if(bTrayError[iTrayMap])
                {
                    labTrayMapStatus->Caption="Exposure time out";
                    Log.sprintf("Tray Map Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDTrayCommunicationLog(iTrayType, Log);
                }
                else
                {
                    labTrayMapStatus->Caption="Waiting result..";
                }
                iRetryCountExpTimo++;        //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }

            if(cbBarCodeSimulate->Checked)
            {
                Task=1200;
            }
            break;
        case 1160:
            if(iRetryCountExpTimo<3)
            {
                Task=1170;
            }
            else
            {
                ret=ShowTrayFunErrMsg(1,iTrayType);

                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    Task=1170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            //--------------------------------------
            //Loader Tray Map  E1,4,X
            //Loader IC Remain E1,5,X
            //Unload Auto1~3   E1,4,X
            //--------------------------------------
            sCCDCommand.sprintf("E1,%d,%d", iFunction, iNowMoveStep);
            SendCCDTrayCommand(iTrayType, "Tray Map exposure 1170: ", sCCDCommand);

            Task=1150;
            TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
            break;
        case 1180:
            Log.sprintf("Alarm, Tray Arm %d exposure position error!");
            AddCCDTrayCommunicationLog(iTrayType, Log);
            ShowTrayFunErrMsg(2,iTrayType);

            Task=1;
            break;
        case 1200:
            iNowMoveStep++;
            iCurrentStep[iTrayMap]=iNowMoveStep;
            if(iNowMoveStep<=MOT[MMTrayY].Tray.XItem-1)       //wei 20170217 修改for Tray XDivision
            {
                Task=1000;
            }
            else
            {
                Task=1300;
            }
            break;
        case 1300:
            //--------------------------------------
            //Loader Tray Map  GV,4
            //Loader IC Remain GV,5
            //Unload Auto1~3   GV,4
            //--------------------------------------
            sCCDCommand.sprintf("GV,%d", iFunction);
            bTrayMapGetResultOK[iTrayMap]=false;
            SendCCDTrayCommand(iTrayType, "Get result 1300: ", sCCDCommand);
            bHasError=false;
            Task=1350;
            TMAResultDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            break;
        case 1350:
            if(bTrayMapGetResultOK[iTrayMap]==true) //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {

                if(iTrayType==iTray_Map || iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)
                {
                    CCDTrayMap_Decoding(iTrayType);
                }
                else if(iTrayType==iDeviceRemain)
                {
                    if(CheckBox1->Checked)
                    {
                        bTrayHaveDevice=true;
                        bHasError=true;
                    }
                }
                Task=2000;
            }
            else if(TMAResultDelay[iTrayMap].Off() || bTrayHaveDevice)  //讀取超時
            {
                if(bTrayMapGetResultOK[iTrayMap]==false)
                {
                    bTrayMapGetResultOK[iTrayMap]=true;
                    labTrayMapStatus->Caption="Inspection time Out";
                    AddCCDTrayCommunicationLog(iTrayType, "Tray Map Action 1350, Inspection time out.");
                    bHasError=true;
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iTrayAutoRetry)      //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    if(iTrayType==iTray_Map || iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)
                    {
                        Task=2500;
                    }
                    else if(iTrayType==iDeviceRemain)
                    {
                        if(bTrayHaveDevice)
                        {
                            NewRecordProcess("","Tray Loader,Count Fail,Handler get device remain form Vision System NG","IC Count:"+IntToStr(iDeviceRemainCount)+",File:"+sTrayMapFileName[iTrayMap]); //Sam 20200211 : Vision Tray Map 通訊增加 圖檔名紀錄
                            iDeviceRemainCount=0;
                        }
                        else
                        {
                            NewRecordProcess("","Tray Loader,Count Fail,Handler get device remain form Vision System time out","");
                        }
                        Task=2400;
                    }
                }
                bHasError=false;
            }
            else
            {
                if(iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)//點量功能多比對 Handler 和 Vision 結果
                {
                    ret=iComparUnloadIC(iAuto);
                }
                if(ret==K_RETRY)
                {
                    Task=1;
                }
                else
                {
                    labTrayMapStatus->Caption="Inspection end !";
                    AddCCDTrayCommunicationLog(iTrayType, "Tray Map >>Cycle 2000, Inspection end.---------------\r\n");
                    Task=5000;
                }
            }
            break;
        case 2400:
            if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
            {
                if(TrayArmMotorMove(Prod.iXTrayColor))
                {
                    Task=2500;
                }
            }
            else
            {
                if(TrayArmMotorMove(Prod.iXTrayEmpty))
                {
                    Task=2500;
                }
            }
            break;
        case 2500:
            labTrayMapStatus->Caption="Inspection error";
            Log.sprintf("Tray Map Alarm 2500, Error");
            AddCCDTrayCommunicationLog(iTrayType, Log);
            iRetryCount=0;
            bHasError=false;
            Cylinder[C_TrayY_Fixer].Off();                                      //Sam 20240402 : Loader 殘料時Alarm時要解除汽缸Lock。
            Cylinder[C_LoaderEdgePush].Off();
            Cylinder[C_LoaderUpPress].Off();
            ret=ShowTrayFunErrMsg(3,iTrayType);    //Sam 20200323 : Modify Tray Function
            if(ret==K_RETRY)
            {
                Task=1;
            }
            else if(ret==K_SKIP)
            {
                Task=5000;
            }
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int iWhichTrayNeedDeviceCount() //Sam 20190405 : Tray Decive Count
{
    for(int i=0; i<3; i++)
    {
         //if(bCCDTrayDeviceCount[i]==true)
         if(bNeedCCDTrayDeviceCount[i])
         {
            return i;
         }
    }
    return -1;
}
//---------------------------------------------------------------------------
bool CheckNeedDeviceCount(int iAuto)                                            //Sam 20190405 : Tray Decive Count
{
    if(USE_TRAY_MAPPING!=etmUninstall && TestIF_File.bEnableTrayDeviceCnt)
    {
        return bNeedCCDTrayDeviceCount[iAuto];
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::PageControl1Change(TObject *Sender)
{
    TrayMapSetDefaultView();
}
//---------------------------------------------------------------------------
int iInitialTrayMapAutoTuneTask=1;
//---------------------------------------------------------------------------
bool TfTrayMapping::DoTrayMapAutoTuneCCD(int iTuneType, bool Reset)//Sam 20200323 : 整合 Tray Map & IC Reamin & Device Count
{
    //iTuneType=0,Loader Tray Map 無 IC 取像
    //iTuneType=1,Loader Tray Map 有 IC 取像
    //iTuneType=2,Loader IC Remain 取像
    //iTuneType=3,Unload Auto1 Tray Map 取像
    //iTuneType=4,Unload Auto2 Tray Map 取像
    //iTuneType=5,Unload Auto3 Tray Map 取像
    int &Task=iInitialTrayMapAutoTuneTask;
    bool bResult=false;
    int ret, pos=0;
    int iTrayMap=0;
    int iTrayType=0;
    int iFunction=0;
    int iAuto=0;
    int iE=0;
    static int iNowMoveStep=0;
    static int iRetryCount=0;
    static int iRetryCountExpTimo=0;
    static bool bHasError=false;
    static AnsiString ErrPart;
    static AnsiString sErrorPart="";
    AnsiString sCCDCommand;
    AnsiString Log;

    if(Reset)
    {
        Task=1;
        return bResult;
    }
    //--------------------------------------
    //sFunction  "4"=TrayMap ; "5"=IC Remain;
    //iE    "E,1"=Normal ; "E,2"=Tray No IC ; "E,3"=Tray Has IC
    //--------------------------------------
    if(iTuneType==0)            //Loader Tray Map training (No IC)
    {
        iTrayMap=iTray_Map;
        iTrayType=iTray_Map;
        iFunction=4;
        iE=2;
    }
    else if(iTuneType==1)       //Loader Tray Map training (Has IC)
    {
        iTrayMap=iTray_Map;
        iTrayType=iTray_Map;
        iFunction=4;
        iE=3;
    }
    else if(iTuneType==2)       //Loader IC Remain training (No IC)
    {
        iTrayMap=iTray_Map;
        iTrayType=iDeviceRemain;
        iFunction=5;
        iE=2;
    }
    else if(iTuneType==3)       //Unload Auto1 Tray Map training (No IC)
    {
        iTrayMap=iTray_DeviceCnt1;
        iTrayType=iTray_DeviceCnt1;
        iFunction=4;
        iE=2;
        iAuto=iTrayType-iTray_DeviceCnt1;
    }
    else if(iTuneType==4)       //Unload Auto2 Tray Map training (No IC)
    {
        iTrayMap=iTray_DeviceCnt2;
        iTrayType=iTray_DeviceCnt2;
        iFunction=4;
        iE=2;
        iAuto=iTrayType-iTray_DeviceCnt1;
    }
    else if(iTuneType==5)       //Unload Auto3  Tray Map training (No IC)
    {
        iTrayMap=iTray_DeviceCnt3;
        iTrayType=iTray_DeviceCnt3;
        iFunction=4;
        iAuto=iTrayType-iTray_DeviceCnt1;
        iE=2;
    }
    else
    {
        return bResult;
    }

    switch(Task)
    {
        case 1:
            AddCCDTrayCommunicationLog(iTrayType, "<<Tray Map Auto Tune Cycle, Cycle start.------------------");
            //--------------------------------------
            //Loader Tray Map   E9,4
            //Loader IC Remain  E9,5
            //Unload Auto1~3    E9,4
            //--------------------------------------
            sCCDCommand.sprintf("E9,%d",iFunction);   //清空暫存器
            iNowMoveStep=0;
            iRetryCountExpTimo=0;
            iCurrentStep[iTrayMap]=iNowMoveStep;
            bTrayMapExposureOK[iTrayMap]=true;
            bTrayMapGetResultOK[iTrayMap]=true;
            bStepError[iTrayMap]=false;
            bGetTraySE9[iTrayMap]=false;
            bHasError=false;
            SendCCDTrayCommand(iTrayType, "Tray Map Auto Tune Clear buffer 1", sCCDCommand);
            TMAStartDelay[iTrayMap].SetMSAndOn(5000);
            Task=500;
        case 500:       //確認Buffer清空
            if(bGetTraySE9[iTrayMap] || cbBarCodeSimulate->Checked)
            {
                if(iTuneType==1)
                    Task=1000;
                else
                    Task=600;
            }
            else if(TMAStartDelay[iTrayMap].Off())
            {
                ShowTrayFunErrMsg(0,iTrayType);
                //ShowErrorMessage("WAR0980", K_RETRY, MTrayX, false, "Tray Map Auto Tune Clear buffer");
                Task=1;
            }
            break;
        case 600:
            bGetCF=false;
            //--------------------------------------
            //Loader Tray Map  CF,4,3_5_30_30_25_25  (No IC)
            //Loader IC Remain CF,5,3_5_30_30_25_25
            //Unload Auto1~3   CF,4,3_5_30_30_25_25
            //--------------------------------------
            sCCDCommand.sprintf("CF,%d,%d_%d_%2.2f_%2.2f_%2.2f_%2.2f", iFunction, MOT[MMTrayY].Tray.XItem, MOT[MMTrayY].Tray.YItem, Prod.LoadForm.iXPitch/100.00, Prod.LoadForm.iYPitch/100.00, DeviceForm_File.XDimension, DeviceForm_File.YDimension);   //建立Tray資料XC、YC表示IC數量，XP、YP表是間距，W、H表示IC大小(單位:mm) X_Y_X Pitch_YPitch_IC W_IC H
            SendCCDTrayCommand(iTrayType, "Tray Map Auto Tune Tray Date 600", sCCDCommand);
            Task=700;
            break;
        case 700:
            if(bGetCF || cbBarCodeSimulate->Checked)
            {
                Log.sprintf("Tray Map Auto Tune Action, Tray Date OK!");
                AddCCDTrayCommunicationLog(iTrayType, Log);
                Task=1000;
            }
            break;
        case 1000:      //移動到拍照位置
            if(iTrayType==iTray_Map || iTrayType==iDeviceRemain)  //Tray Map & IC Remain 跑一樣的位置
            {
                pos=Prod.iTrayMapping+TestIF_File.iTrayMapStart+(LoadForm->XPitch*iNowMoveStep);
            }
            else if(iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)
            {
                pos=Prod.iTrayDeviceCnt[iAuto]+TestIF_File.iTrayDeciveCntStart+(LoadForm->XPitch*iNowMoveStep);
            }
            #ifdef SOFT_SIMULTE
                Log.sprintf("Tray Map Auto Tune Action, Tray Arm move to step %d", iNowMoveStep);
                AddCCDTrayCommunicationLog(iTrayType, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            #endif
            if(TrayArmMotorMove(pos, false))    //Sam 20241206 : Tray Arm 新增 Teach 保護
            {
                Log.sprintf("Tray Map Auto Tune Action, Tray Arm move to step %d", iNowMoveStep);
                AddCCDTrayCommunicationLog(iTrayType, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            }
            break;
        case 1120:
            if(TMAStartDelay[iTrayMap].Off())    //到位Delay
            {
                    bTrayMapExposureOK[iTrayMap]=false;
                    bTrayError[iTrayMap]=false;
                    //--------------------------------------
                    //E1  , 4       , X
                    //拍照 Tray Map 位置
                //Loader Tray Map  E2,4,X (No IC)
                //Loader Tray Map  E3,4,X (Has IC)
                //Loader IC Remain E2,5,X
                //Unload Auto1~3   E2,4,X
                    //--------------------------------------
                sCCDCommand.sprintf("E%d,%d,%d", iE, iFunction, iNowMoveStep);
                SendCCDTrayCommand(iTrayType, "Tray Map Auto Tune exposure 1120: ", sCCDCommand);
                    Task=1150;
                    iRetryCountExpTimo=0;        //Steven 20160425 : 修改拍照Time Out
                    TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
                }
            break;
        case 1150:
            if(bStepError[iTrayMap]==true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                Task=1180;
            }
            else if(bTrayMapExposureOK[iTrayMap]==true)    //拍照完成
            {
                Task=1200;
            }
            else if(TMAExposureDelay[iTrayMap].Off() || bTrayError[iTrayMap])   //拍照time out
            {
                if(bTrayError[iTrayMap])
                {
                    Log.sprintf("Tray Map Auto Tune Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDTrayCommunicationLog(iTrayType, Log);
                }
                else
                {

                }
                iRetryCountExpTimo++;        //Steven 20160425 : 修改拍照Time Out
                Task=1160;
            }

            if(cbBarCodeSimulate->Checked)
            {
                Task=1200;
            }
            break;
        case 1160:
            if(iRetryCountExpTimo<3)
            {
                Task=1170;
            }
            else
            {
                ret=ShowTrayFunErrMsg(1,iTrayType);
                if(ret==K_SKIP)
                {
                    Task=1200;
                }
                else
                {
                    Task=1170;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            sCCDCommand.sprintf("E%d,%d,%d", iE, iFunction, iNowMoveStep);
            SendCCDTrayCommand(iTrayType, "Tray Map Auto Tune exposure 1170: ", sCCDCommand);
            TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
            Task=1150;
            break;
        case 1180:
            Log.sprintf("Alarm, Tray Map Auto Tune Tray Arm %d exposure position error!");
            AddCCDTrayCommunicationLog(iTrayType, Log);
            ShowTrayFunErrMsg(2,iTrayType);
            Task=1;
            break;
        case 1200:
            iNowMoveStep++;
            iCurrentStep[iTrayMap]=iNowMoveStep;
            if(iNowMoveStep<=MOT[MMTrayY].Tray.XItem-1)       //wei 20170217 修改for Tray XDivision
            {
                Task=1000;
            }
            else
            {
                if(iTuneType==1)    //QQ
                    Task=1300;
                else
                    Task=2000;
            }
            break;
        case 1300:
            bGetSF=false;
            //--------------------------------------
            //Loader Tray Map  SF,4,SetupFileName (No IC)
            //Loader IC Remain SF,5,SetupFileName
            //Unload Auto1~3   SF,4,SetupFileName
            //--------------------------------------
            sCCDCommand.sprintf("SF,%d,%s", iFunction, fMain->cbSetupFileName->Text);
            SendCCDTrayCommand(iTrayType, "Save File", sCCDCommand);  //儲存檔案
            bHasError=false;
            TMAResultDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            Task=1350;

            break;
        case 1350:
            if(bGetSF==true) //儲存檔案完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                CCDTrayMap_Decoding(1);
                Task=2000;
            }
            else if(TMAResultDelay[iTrayMap].Off())  //讀取超時
            {
                sErrorPart="";
                if(bTrayMapGetResultOK[iTrayMap]==false)
                {
                    bTrayMapGetResultOK[iTrayMap]=true;
                    AddCCDTrayCommunicationLog(iTrayType, "Tray Map Auto Tune Action 1350, Inspection time out.");
                    bHasError=true;
                }

                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                Task=2500;
                bHasError=false;
            }
            else
            {
                AddCCDTrayCommunicationLog(iTrayType, "Tray Map Auto Tune >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 2500:
            Log.sprintf("Tray Map Auto Tune Alarm 2500, Tray Map Save File error");
            AddCCDTrayCommunicationLog(iTrayType, Log);
            iRetryCount=0;
            bHasError=false;
            Task=1;
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoReadTrayData(AnsiString FileName)
{
    AnsiString str, str2;
    bool bDataError=false;
    bool bResult=false;
    TStringList *MyList = new TStringList();

    str=FileName;
    MyList->LoadFromFile(str.c_str());

    MyList->Text=StringReplace(MyList->Text, " ", "", TReplaceFlags()<<rfReplaceAll);

    bDataError=!(MyList->Count==MOT[MMTrayY].Tray.YItem);

    if(bDataError==false)
    {
        for(int i=0; i<MyList->Count; i++)
        {
            if(MyList->Strings[i].Length()!=MOT[MMTrayY].Tray.XItem)
            {
                bDataError=true;
            }
        }
    }

    if(bDataError==false)
    {
        WriteTrayMapLog(5, MyList->CommaText);
        for(int i=0; i<MOT[MMTrayY].Tray.YItem; i++)
        {
            str2=MyList->Strings[i];
            for(int j=0; j<MOT[MMTrayY].Tray.XItem; j++)
            {
                iTrayMappingDate[i][j]=str2[1+j];
                sgTraymap->Cells[j+1][i+1]=MyDeCodeASCII(iTrayMappingDate[i][j]);
            }
        }
        bResult=true;
    }

    DeleteFile(str.c_str());
    return bResult;
}
//---------------------------------------------------------------------------
void TfTrayMapping::WriteTrayMapLog(int index, AnsiString Message)  //Steven 20160728 : Log by running mode
{
    AnsiString DataStr, Path, str;

    if(index==3)
    {
        str="Tray ID";
    }
    else if(index==4)
    {
        str="Tray Map";
    }
    else if(index==5)
    {
        str="Down Map";
    }
    else if(index==6)
    {
        str="Pick Map";
    }
    else if(index==7)
    {
        str="OCR ID";
    }
    else if(index==8)
    {
        str="Tray ID 2";
    }
    else if(index==9)
    {
        str="Tray Auto1";
    }
    else if(index==10)
    {
        str="Tray Auto2";
    }
    else if(index==11)
    {
        str="Tray Auto3";
    }

    DataStr.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %s, %s",
                    SystemYear, SystemMonth, SystemDate,
                    SystemHour, SystemMin, SystemSec, SystemMSec,
                    str,
                    Message);

    Path.sprintf("%s\\%04d_%02d_%02d\\", asTrayMapLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path, "TfTrayMapping::WriteTrayMapLog");
    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d.txt", asTrayMapLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate);
    WriteDataToFile(Path.c_str() , DataStr.c_str());

    MemoTrayMap->Lines->Add(DataStr);
    if(MemoTrayMap->Lines->Count>100)
        MemoTrayMap->Lines->Clear();
}
//---------------------------------------------------------------------------
void TfTrayMapping::WritePickMapLog()   //wei 20170119 (Steven) Tray Mapping紀錄是否有吸取IC
{
    AnsiString str;
    if(TestIF_File.bEnableTrayIDDownFTP==false)
    {
        str.sprintf("SV,4,1,%d_%d,",MOT[MMTrayY].Tray.XItem,MOT[MMTrayY].Tray.YItem);
    }

    for(int j=1; j<=MOT[MMTrayY].Tray.YItem; j++)
    {
        for(int i=1; i<=MOT[MMTrayY].Tray.XItem; i++)
        {
            if(sgSuckmap->Cells[i][j]=="1")
                str+="1";
            else
                str+="0";
            sgSuckmap->Cells[i][j]=0;
        }
        if(j!=MOT[MMTrayY].Tray.YItem)
            str+=",";
    }
    if(bFTPDownLoadFileError==true)
    {
        str+="--DownLoad Fail";
    }

    WriteTrayMapLog(6, str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edTrayStartDelayClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, 30000);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edTrayAutoRetryClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 30);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::Button4Click(TObject *Sender)
{
    AnsiString DownName="",DownName1="";
    if(DirectoryExists("Z:\\")==false)  //Ifor 20170202 (wei) add KYEC ATC 網路磁碟機未連線直接Alarm
    {
        ShowMyMessage("Tray map Route Z: Disk Fail!!");  //Steven 20091004
    }
    else
    {
        TStringList *TestList=new TStringList;
        AnsiString str,str1;
        AnsiString asDataSource,asDataPurpose;
        AnsiString asDataSourceFile;
        TStringList *sList=new TStringList();
        asDataSourceFile=fLotInfo->edtSysLotID->Text;
        asDataSourceFile=asDataSourceFile.SubString(1,6);
        asDataPurpose="D:\\HT9045_Log\\TrayMapLog\\";

        sList->CommaText=Edit2->Text.c_str();
        WriteTrayMapLog(3, Edit2->Text);
//        Data=sList->Strings[3];
        asTrayIDData1=sList->Strings[3];
        labTrayID->Caption=sList->Strings[3];

        SearchTrayMapFile();
        //取得ATC來源資料名稱
        asDataSource.sprintf("Z:\\%s\\%s_%s.txt",asDataSourceFile,asDataSourceFile,asTrayIDData1);
        //複製來源檔案至目的位置
        str.sprintf("XCOPY /y/a/e/c/i/h/f/r %s %s", asDataSource, asDataPurpose);
        TestList->Add(str);

        TestList->SaveToFile("D:\\HT9045\\EVENLOG.bat");                        //Ifor 20160408 (Steven) 建立檔案與寫入檔案
        ExecZipCommand("D:\\HT9045\\EVENLOG.bat", " ");                         //Ifor 20160408 (Steven) 避免複製檔案時出現Dos執行畫面
        MySleep(500);
        TestList->Clear();
        delete TestList;
        delete sList;

        DownName.sprintf("%s_%s",asDataSourceFile,asTrayIDData1);

        DownName1.sprintf("%s\\%s.txt",asTrayMapLogPath,DownName);
        if(FileExists(DownName1.c_str())==false)
        {
            ShowErrorMessage("WAR0974", K_RETRY|K_SKIP, MTrayX, false);
        }
        else
        {
            DoReadTrayData(DownName1.c_str());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edtTrayMapCatchClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 1, true, 30, MOT[MMTrayY].Tray.XItem);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edtTrayMapShiftClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1000, 30000);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::AutoConnectNetwork()
{
    if(CUSTOMER_CODE==CC_TSMC_TAINAN)
    {
        AnsiString str;
        TStringList *TestList=new TStringList;  //Ifor 20170210 (wei) New 物件移至判斷式內部，避免判斷式不成立時不會刪除

        str.sprintf("NET CONFIG SERVER /AUTODISCONNECT:-1");
        TestList->Add(str);
        str.sprintf("net use z: /delete");
        TestList->Add(str);
        str.sprintf("net use z: \\\\%s\\FT_tray_map\\ /user:p_fthandler FT2017Handle","10.97.16.81");
//        str.sprintf("net use z: \\\\%s\\FT_tray_map /user:wei 1234","172.16.8.101");
//        str.sprintf("net use z: \\\\%s\\20170622 ","172.16.8.101");
        TestList->Add(str);

        TestList->SaveToFile("D:\\HT9045\\NetworkConnect.bat");                 //Ifor 20170202 (Steven) 建立ATC連線檔案與寫入檔案
        ExecZipCommand("D:\\HT9045\\NetworkConnect.bat", " ");                  //Ifor 20170202 (Steven) 避免出現Dos執行畫面
        MySleep(1200);
        TestList->Clear();
        delete TestList;
        if(DirectoryExists("z:\\")==false)
        {
            ShowMyMessage("Tray map Route z: Disk Fail!!");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::SearchTrayMapFile()
{
    AnsiString str, asOCRSaveMane1, str1, str2;

    if(fLotInfo->edtSysLotID->Text!="")
    {
        asTrayIDFile=fLotInfo->edtSysLotID->Text;
        asTrayIDFile=asTrayIDFile.SubString(1,6);

        str.sprintf("z:\\%s",asTrayIDFile);
        asOCRSaveMane1=str.c_str();
        DirectoryListBox1->Directory=asOCRSaveMane1;
        DirectoryListBox1->Update();
        DirectoryListBox1->Refresh();
        FileListBox1->Directory=asOCRSaveMane1;
        FileListBox1->Update();
        FileListBox1->Refresh();

        for(int i=0; i<FileListBox1->Items->Count; i++)
        {
            str1=FileListBox1->Items->Strings[i];
            str2=str1.SubString(10,10);
            if(str2==asTrayIDData1)
            {
                str1=str1.SubString(8,12);
                asTrayIDData1=str1;
                continue;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edtTrayIDReadShiftClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -3000, 3000);
}
//---------------------------------------------------------------------------
void TfTrayMapping::WriteUnloadMapLog(int iWhichAuto, bool bTray)
{
    AnsiString str, Path, TempPath;
    AnsiString PathAuto[6]={"Auto1","Auto2","Auto3","Fix1","Fix2","Fix3"};
    int iAutoEA=0;
    Path.sprintf("%s\\Unloader\\%s\\%s", asTrayMapLogPath, fLotInfo->edtSysLotID->Text, PathAuto[iWhichAuto]);
    MyForceDirectories(Path, "TfTrayMapping::WriteUnloadMapLog");
    if(bTray==false)
    {
        for(int y=0; y<MOT[iMMAuto[iWhichAuto]].Tray.YItem; y++)
        {
            for(int x=0; x<MOT[iMMAuto[iWhichAuto]].Tray.XItem; x++)
            {
                if(iMRTrayCount[iWhichAuto+3]==0)
                {
                    str+="0";
                }
                else if(MOT[iMMAuto[iWhichAuto]].Tray.Data[x][y]!=NULL_IC)
                {
                    str+="1";
                    iAutoEA++;
                }
                else
                {
                    str+="0";
                }
            }
            if(y!=MOT[iMMAuto[iWhichAuto]].Tray.YItem)
                str+="\n";
        }

        Path.sprintf("%s\\Unloader\\%s\\%s\\%s.txt", asTrayMapLogPath, fLotInfo->edtSysLotID->Text, s6TrayName[iWhichAuto], MOT[iMMAuto[iWhichAuto]].Tray.cCassetteID);
        WriteDataToFile(Path.c_str() , str.c_str());
        iMRTrayCount[iWhichAuto+3]++;
        iAutoCount[iWhichAuto]=iAutoCount[iWhichAuto]+iAutoEA;
    }
}
//---------------------------------------------------------------------------
int  iInitialTrayID2Task=1;
//------------------------------------------------------------------------------
void InitialTrayID2Task()       //wei 20161219 Tray Mapping
{
    iInitialTrayID2Task=1;
    bCCDTrayID[1]=true;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoTrayID2CCD()
{
    int &Task=iInitialTrayID2Task;
    bool bResult=false;
    int ret, pos=0, iPosX=0, iPosY=0;
    int iTrayMap=iTray_ID2;
    static int iRetryCount=0;
    static int iRetryCountExpTimo=0;
    static bool bHasError=false;
    AnsiString sCCDCommand;
    AnsiString Log;
    AnsiString DownName,DownName1,sKeyinTrayID;

    switch(Task)
    {
        case 1:
            AddCCDTrayCommunicationLog(iTrayMap, "<<Tray ID 2 Cycle, Cycle start.------------------");

            bTrayMapExposureOK[iTrayMap]=true;
            bTrayMapGetResultOK[iTrayMap]=true;
            iRetryCountExpTimo=0;
            bFTPDownLoadFileError=false;

            bHasError=false;
            if(bCCDTrayID[1])                        //wei 20161102 IC置偏檢查
            {
                iRetryCount=0;
                bCCDTrayID[1]=false;
            }
            sCCDCommand.sprintf("E9,3,1");   //清空暫存器
            bGetTraySE9[iTrayMap]=false;
            SendCCDTrayCommand(iTrayMap, "Tray ID 2 Clear buffer 1", sCCDCommand);     //Reset CCD Shuttle暫存器
            TMAStartDelay[iTrayMap].SetMSAndOn(5000);
            Task=500;
        case 500:       //確認Buffer清空
            if(bGetTraySE9[iTrayMap])
            {
//                Task=900;     //empty 不用移動
                Task=1000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
//                Task=900;     //empty 不用移動
                Task=1000;
            }
            else if(TMAStartDelay[iTrayMap].Off())
            {
                ShowErrorMessage("WAR0970", K_RETRY, MTrayX, false, "Tray ID 2 Clear buffer");
                Task=1;
            }
            break;
        case 900:
            if(IsMoveInArm2XYToWait())
            {
                iPosX=MOT[MInArmX].ReadPos();
                iPosY=MOT[MInArmY].ReadPos();
                Log.sprintf("InArm X:%d,InArm Y:%d",iPosX, iPosY);   //InArm X:1527,InArm Y:-46666
                AddCCDTrayCommunicationLog(iTrayMap, Log);
                Task=1000;
            }
            else
            {
                ShowMyMessage("InArm X/Y no move Wait");
                MOT[MInArmX].fCanMove=true;
                MOT[MInArmY].fCanMove=true;
                Task=950;
            }
            break;
        case 950:
            if(MoveInArm2XYToWait())
            {
                MOT[MInArmX].fCanMove=false;
                MOT[MInArmY].fCanMove=false;
                Task=900;
            }
            break;
        case 1000:      //移動到拍照位置
            pos=Tech.iTrayXEmpty+(LoadForm->iPickUp*2)+TestIF_File.iTrayID2Shift;  //wei 20170608

            #ifdef SOFT_SIMULTE
                Log.sprintf("Tray ID 2 Action, Tray Arm move to step Tray");
                AddCCDTrayCommunicationLog(iTrayMap, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            #endif
            if(TrayArmMotorMove(pos, false))    //Sam 20241206 : Tray Arm 新增 Teach 保護
            {
                Log.sprintf("Tray ID 2 Action, Tray Arm move to step Tray");
                AddCCDTrayCommunicationLog(iTrayMap, Log);
                TMAStartDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayStartDelay);
                Task=1120;
            }
            break;
        case 1120:
            if(TMAStartDelay[iTrayMap].Off())    //到位Delay
            {
                bTrayMapExposureOK[iTrayMap]=false;
                bTrayError[iTrayMap]=false;
                //--------------------------------------
                //E1  , 3
                //拍照 Tray ID
                //--------------------------------------
                sCCDCommand.sprintf("E1,3,1");

                labTrayID2Status->Caption="Tray ID 2 Exposure...";
                SendCCDTrayCommand(iTrayMap, "Tray ID 2 exposure 1120: ", sCCDCommand);

                Task=1150;
                iRetryCountExpTimo=0;        //Steven 20160425 : 修改拍照Time Out
                TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
            }
            break;
        case 1150:
            if(bTrayMapExposureOK[iTrayMap]==true)    //拍照完成
            {
                labTrayID2Status->Caption="Waiting result..";
                Task=1200;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                Task=1200;
            }
            else if(TMAExposureDelay[iTrayMap].Off() || bTrayError[iTrayMap])   //拍照time out
            {
                if(bTrayError[iTrayMap])
                {
                    labTrayID2Status->Caption="Exposure time out";
                    Log.sprintf("Tray ID 2 Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    AddCCDTrayCommunicationLog(iTrayMap, Log);
                }
                else
                {
                    labTrayID2Status->Caption="Waiting result..";
                }

                iRetryCountExpTimo++;        //Steven 20160425 : 修改拍照Time Out
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
                ret=ShowErrorMessage("WAR0971", K_RETRY|K_SKIP, MTrayX);  //2D function CCD 3 or 4 Exposure time out!
                if(ret==K_SKIP)
                {
//                    Task=2000;
//                    bHasError=false;
                    if(TestIF_File.bEnableTrayIDDownFTP)        //wei 20170518
                    {
                        asTrayIDData2 = ShowMyInputTrayID("Tray ID 2" , "Please Keyin Tray ID : ");
                        sKeyinTrayID.sprintf("Tray ID 2 exposure time out--SKIP Keyin %s",asTrayIDData2);
                        WriteTrayMapLog(3, sKeyinTrayID.c_str());
                        SearchTrayMapFile();
                        Task=3500;
                    }
                    else
                    {
                        Task=5000;
                    }
                }
                else
                {
                    sKeyinTrayID.sprintf("Tray ID 2 exposure time out--RETRY reset");
                    WriteTrayMapLog(3, sKeyinTrayID.c_str());
                    Task=1;
                }
                iRetryCountExpTimo=0;
            }
            break;
        case 1170:
            sCCDCommand.sprintf("E1,3,1");
            SendCCDTrayCommand(iTrayMap, "Tray ID 2 exposure 1170: ", sCCDCommand);

            Task=1150;
            TMAExposureDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayExposureTimeOut);
            break;
        case 1200:
            sCCDCommand="GV,3,1";       //Ifor 20151228 取得 CCD 讀取資料

            bTrayMapGetResultOK[iTrayMap]=false;
            SendCCDTrayCommand(iTrayMap, "Tray ID 2 get result 1200: ", sCCDCommand);

            bHasError=false;
            Task=1350;
            TMAResultDelay[iTrayMap].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            break;
        case 1350:
            if(bTrayMapGetResultOK[iTrayMap]==true) //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                CCDTrayID_Decoding(3);
                Task=2000;
            }
            else if(TMAResultDelay[iTrayMap].Off())  //讀取超時
            {
                if(bTrayMapGetResultOK[iTrayMap]==false)
                {
                    bTrayMapGetResultOK[iTrayMap]=true;
                    labTrayID2Status->Caption="Inspection time Out";
                    AddCCDTrayCommunicationLog(iTrayMap, "Tray ID 2 Action 1350, Inspection time out.");
                    bHasError=true;
                }

                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iTrayAutoRetry)                       //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    Task=2500;
                }
                bHasError=false;
            }
            else
            {
                labTrayID2Status->Caption="Tray ID 2 Inspection end !";
                AddCCDTrayCommunicationLog(iTrayMap, "Tray ID 2 >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 2500:
            labTrayID2Status->Caption="Tray ID 2 Inspection error !";
            Log.sprintf("Tray ID 2 Alarm 2500, Tray ID read error");
            AddCCDTrayCommunicationLog(iTrayMap, Log);
            ret=ShowErrorMessage("WAR0973", K_RETRY|K_SKIP, MTrayX, false);
            iRetryCount=0;
            bHasError=false;
            if(ret==K_RETRY)
            {
                sKeyinTrayID.sprintf("Tray ID 2 exposure Result error--RETRY reset");
                WriteTrayMapLog(3, sKeyinTrayID.c_str());
                Task=1;
            }
            else
            {
                Task=5000;
            }
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edtTrayDeciveCntStartClick(TObject *Sender)
{
  fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, -10000, -50000);
}
//---------------------------------------------------------------------------
int TfTrayMapping::iComparUnloadIC(int iAuto)   //Sam 20200323 : Modify Tray Function
{
    int ret=0;
    int iL_IC=0,iV_IC=0,iH_IC=0;
    int iTrayMap=0;
    iTrayMap=iTray_DeviceCnt1+iAuto;
    AnsiString asCode="",astr1="",astr2="",astr3="";

    iL_IC=iVisonAndHandlerComparLessCnt(iAuto);
    iV_IC=iVisionICCnt(iAuto);
    iH_IC=MOT[iMMAuto[iAuto]].HowManyDevice();
    astr2.sprintf("IC Count:Vision-%d,Handler-%d,File:%s",iV_IC,iH_IC,sTrayMapFileName[iTrayMap]);
    if(iL_IC>0)
    {
        astr1.sprintf("Unload Tray %d,Count Fail,Handler get Device Count form Vision System NG",iAuto+1);
        astr3.sprintf("Count less=%d",iL_IC);
        asCode.sprintf("WAR%d84",11+iAuto);     //WAR1184
        ret=ShowErrorMessage(asCode, K_RETRY|K_SKIP, MTrayX, false,astr3);
        //NewRecordProcess("","Unload Tray "+IntToStr(iAuto+1)+",Count Fail,Handler get Device Count form Vision System NG","IC Count:Vision-"+IntToStr(iV_IC)+",Handler-"+IntToStr(iH_IC)+",File:"+sTrayMapFileName[iTrayMap]); //Sam 20200211 : Vision Tray Map 通訊增加 圖檔名紀錄
    }
    else
    {
        astr1.sprintf("Unload Tray %d,Count Pass,Handler get Device Count form Vision System OK",iAuto+1);
        //NewRecordProcess("","Unload Tray "+IntToStr(iAuto+1)+",Count Pass,Handler get Device Count form Vision System OK","IC Count:Vision-"+IntToStr(iV_IC)+",Handler-"+IntToStr(iH_IC)+",File:"+sTrayMapFileName[iTrayMap]);
    }
    NewRecordProcess("",astr1,astr2);
    return ret;
}
//---------------------------------------------------------------------------
int TfTrayMapping::iVisonAndHandlerComparLessCnt(int iAuto)     //Sam 20200323 : Modify Tray Function
{
    int iCount=0;
    for(int j=0; j<=MOT[iMMAuto[iAuto]].Tray.YItem-1; j++)
    {
        for(int i=0; i<=MOT[iMMAuto[iAuto]].Tray.XItem-1; i++)
        {
            if((MOT[iMMAuto[iAuto]].Tray.Data[i][j]==HAS_IC  && MyDeCodeASCII(fTrayMapping->iTrayDeviceCountDate[iAuto][j][i])!="1") ||
               (MOT[iMMAuto[iAuto]].Tray.Data[i][j]==NULL_IC && MyDeCodeASCII(fTrayMapping->iTrayDeviceCountDate[iAuto][j][i])!="0"))
            {
                iCount++;
            }
        }
    }
    return iCount;
}
//---------------------------------------------------------------------------
int TfTrayMapping::iVisionICCnt(int iAuto)      //Sam 20200323 : Modify Tray Function
{
    int iCount=0;
    for(int j=0; j<=MOT[iMMAuto[iAuto]].Tray.YItem-1; j++)
    {
        for(int i=0; i<=MOT[iMMAuto[iAuto]].Tray.XItem-1; i++)
        {
            if(MyDeCodeASCII(fTrayMapping->iTrayDeviceCountDate[iAuto][j][i])=="1")
            {
                iCount++;
            }
        }
    }
    return iCount;
}
//---------------------------------------------------------------------------
int TfTrayMapping::ShowTrayFunErrMsg(int iErr,int iTrayType)    //Sam 20200323 : Modify Tray Function
{
    int ret=0;
    int iAuto=0;
    AnsiString asCode="";
    if(iTrayType==iTray_Map)
    {
        asCode.sprintf("WAR098%d",iErr);     //WAR0980 WAR0981 WAR0982 WAR0983
        if(iErr==0)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX, false, "Tray Map Clear buffer");
        }
        else if(iErr==1)
        {
            ret=ShowErrorMessage(asCode, K_RETRY|K_SKIP, MTrayX);
        }
        else if(iErr==2)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX);
        }
        else if(iErr==3)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX, false);
        }

    }
    else if(iTrayType==iDeviceRemain)
    {
        asCode.sprintf("WAR099%d",iErr);     //WAR0990 WAR0991 WAR0992 WAR0993
        if(iErr==0)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX, false, "IC Remain Clear buffer");
        }
        else if(iErr==1)
        {
            ret=ShowErrorMessage(asCode, K_RETRY|K_SKIP, MTrayX);
        }
        else if(iErr==2)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX);
        }
        else if(iErr==3)
        {
            ret=ShowErrorMessage(asCode, K_RETRY|K_SKIP, MTrayX, false);
        }
    }
    else if(iTrayType==iTray_DeviceCnt1 || iTrayType==iTray_DeviceCnt2 || iTrayType==iTray_DeviceCnt3)
    {
        iAuto=iTrayType-iTray_DeviceCnt1;
        asCode.sprintf("WAR%d8%d",11+iAuto,iErr);    //WAR1180 WAR1181 WAR0992 WAR1183
        if(iErr==0)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX, false, "Tray Device Count Clear buffer");
        }
        else if(iErr==1)
        {
            ret=ShowErrorMessage(asCode, K_RETRY|K_SKIP, MTrayX);
        }
        else if(iErr==2)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX);
        }
        else if(iErr==3)
        {
            ret=ShowErrorMessage(asCode, K_RETRY, MTrayX, false);
        }
    }
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5) //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=75;
        Top=10;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Keyence1Connect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;

    if(InitialOK==false)
    {
        Ptr->Active=false;
        return;
    }

    AnsiString Str;
    Str.sprintf("IP: %s, Port:%d [Connect OK]", Ptr->Address, Ptr->Port);
    SendKeyenceCommand(Ptr->Tag, Str, "");
}
//---------------------------------------------------------------------------
void TfTrayMapping::SendKeyenceCommand(int index, AnsiString Msg, AnsiString Msg2)
{
    if(memoTrayCCDCommLog==NULL || InitialOK==false)
        return;

    AnsiString Log;
    GetTimeInfo();

    if(Msg2=="")
        Log.sprintf("Action, %s", Msg);
    else
        Log.sprintf("Send, %s, %s", Msg, Msg2);

    AddKeyenceCommunicationLog(index, Log);
    if(index==iLoaderID)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Keyence1->Active)
            {
                ClientSocket_Keyence1->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iTray_ID2 || index==iEmptyID)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_Keyence2->Active)
            {
                ClientSocket_Keyence2->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iKeyenceCoverTrayID_LoaderCar)        //JerryYang 20240318
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_CoverTray_ID_LoaderCar->Active)
            {
                ClientSocket_CoverTray_ID_LoaderCar->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iKeyenceCoverTrayID_Loader)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_CoverTray_ID_Loader->Active)
            {
                ClientSocket_CoverTray_ID_Loader->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iKeyenceCoverTrayID_AUTO1)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_CoverTray_ID_AUTO1Car->Active)
            {
                ClientSocket_CoverTray_ID_AUTO1Car->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iKeyenceCoverTrayID_AUTO2)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_CoverTray_ID_AUTO2Car->Active)
            {
                ClientSocket_CoverTray_ID_AUTO2Car->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
    else if(index==iKeyenceCoverTrayID_AUTO3)
    {
        if(Msg2=="")
        {
        }
        else
        {
            if(ClientSocket_CoverTray_ID_AUTO3Car->Active)
            {
                ClientSocket_CoverTray_ID_AUTO3Car->Socket->SendText(Msg2+"\r\n");
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::AddKeyenceCommunicationLog(int index, AnsiString Str)
{
    AnsiString Path, Name, Log;

    if(index==iLoaderID)
    {
        Name="Loader Tray ID";
    }
    else if(index==iTray_ID2 || index==iEmptyID)
    {
        Name="Empty Tray ID";
    }
//    else if(index==iMultileID)
//    {
//        Name="Multile Tray ID";
//    }
//    else if(index==iAuto3ID)
//    {
//        Name="Auto3 ID";
//    }

    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %s, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Name, Str);

    if(USE_COVER_TRAYID!=tCIDNotUse)                                            //JerryYang 20250429 : fix Auto In/Out
    {
        if(MemoKeyenceLogCover->Lines->Count>1000)
            MemoKeyenceLogCover->Clear();
        MemoKeyenceLogCover->Lines->Add(Log);
    }
    else
    {
        if(memoKeyenceCommLog->Lines->Count>1000)
            memoKeyenceCommLog->Clear();
        memoKeyenceCommLog->Lines->Add(Log);
    }

    Path.sprintf("%s\\%04d_%02d_%02d", asKeyenceCommLogPath, SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(Path);
    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asKeyenceCommLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);
    WriteDataToFile(Path.c_str() , Log.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Keyence1Disconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    SendKeyenceCommand(Ptr->Tag, "DisConnect", "");
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Keyence1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    if(InitialOK==false)
    {
        return;
    }

    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    int Tag=Ptr->Tag;
    SendKeyenceCommand(Tag, GetErrorMsg(Sender, ErrorCode), "");
    ErrorEvent=TErrorEvent(NULL);
    ErrorCode =0;

    try
    {
        Ptr->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "ClientSocket_Keyence1 Error");
    }

    Abort();
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_Keyence1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
    if(InitialOK==false)
    {
        return;
    }
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    int Tag=Ptr->Tag;

    AnsiString EthernetBuffer;
    int iBufferLenght=0;
    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0) //13.09.30.01   klutter
    {
        EthernetBuffer=Socket->ReceiveText();
        if(EthernetBuffer!="")
        {
            if(Tag==iLoaderID)
            {
                bKeyenceResultOK[0]=true;
                asTrayIDData1=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            }
            else if(Tag==iEmptyID)
            {
                bKeyenceResultOK[1]=true;
                asTrayIDData2=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            }
            else if(Tag==iAuto3ID)
            {
                bKeyenceResultOK[3]=true;
                asAuto3TrayIDData1=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            }
            else if(Tag==iMultileID)
            {
                bKeyenceResultOK[2]=true;
                asTrayIDData3=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            }
            if(Tag==iKeyenceCoverTrayID_LoaderCar)
            {
                bKeyenceResultOK[4]=true;
                asTrayIDDataCorverLoader=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            }
            else if(Tag==iKeyenceCoverTrayID_Loader)
            {
                bKeyenceResultOK[5]=true;
                asTrayIDDataLoader2=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
            }
            else if(Tag==iKeyenceCoverTrayID_AUTO1)
            {
                bKeyenceResultOK[6]=true;

                if(IniConfig.bA65_BundleIDList && IniConfig.bP53_ForcedScanBinCodeOfUnloader)  //JerryYang 20240821 : 自動讀取BIN LABEL
                {
                    TStringList *sCoverTrayList=new TStringList;
                    sCoverTrayList->Clear();
                    sCoverTrayList->CommaText=EthernetBuffer;

                    for(int i=0; i<sCoverTrayList->Count; i++)
                    {
                        if(i==0)
                        {
                            if(fNote->fShow &&  //JerryYang 20250429 : fix Auto In/Out
                               ((fNote->bAutoIsScanBinLabel[0]==true && (fNote->edErrorCode->Text=="MES1120" || fNote->edErrorCode->Text=="MES1124")) ||
                                (fNote->bAutoIsScanBinLabel[1]==true && (fNote->edErrorCode->Text=="MES1220" || fNote->edErrorCode->Text=="MES1224")) ||
                                (fNote->bAutoIsScanBinLabel[2]==true && (fNote->edErrorCode->Text=="MES1320" || fNote->edErrorCode->Text=="MES1324"))))
                            {

                            }
                            else
                            {
                                asAutoCoverTrayID1=sCoverTrayList->Strings[0];
                            }
                        }
                        else
                        {
                            if(fNote->fShow)
                            {
                                int iCnt=0;
                                for(int j=0; j<=iTestBinCount; j++)
                                {
                                    if(fNote->myBinCodeEdit[j]->Visible==true)
                                    {
                                        iCnt++;
                                    }
                                }
                                if((sCoverTrayList->Count-1)>iCnt && iCnt>0)    //JerryYang 20250429 : fix Auto In/Out
                                {
                                    ShowMyMessage("Auto scan bin label count is over bin setting count!");
                                }
                                else
                                {
                                    for(int j=0; j<=iTestBinCount; j++)
                                    {
                                        if(fNote->myBinCodeEdit[j]->Visible==true && fNote->myBinCodeEdit[j]->Text=="")
                                        {
                                            fNote->myBinCodeEdit[j]->Text=sCoverTrayList->Strings[i];
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    sCoverTrayList->Clear();
                    delete sCoverTrayList;

                }
                else
                {
                    asAutoCoverTrayID1=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
                }
            }
            else if(Tag==iKeyenceCoverTrayID_AUTO2)
            {
                bKeyenceResultOK[7]=true;

                if(IniConfig.bA65_BundleIDList && IniConfig.bP53_ForcedScanBinCodeOfUnloader)  //JerryYang 20240821 : 自動讀取BIN LABEL
                {
                    TStringList *sCoverTrayList=new TStringList;
                    sCoverTrayList->Clear();
                    sCoverTrayList->CommaText=EthernetBuffer;

                    for(int i=0; i<sCoverTrayList->Count; i++)
                    {
                        if(i==0)
                        {
                            if(fNote->fShow &&
                               ((fNote->bAutoIsScanBinLabel[0]==true && (fNote->edErrorCode->Text=="MES1120" || fNote->edErrorCode->Text=="MES1124")) ||
                                (fNote->bAutoIsScanBinLabel[1]==true && (fNote->edErrorCode->Text=="MES1220" || fNote->edErrorCode->Text=="MES1224")) ||
                                (fNote->bAutoIsScanBinLabel[2]==true && (fNote->edErrorCode->Text=="MES1320" || fNote->edErrorCode->Text=="MES1324"))))            //JerryYang 20250429 : fix Auto In/Out
                            {

                            }
                            else
                            {
                                asAutoCoverTrayID2=sCoverTrayList->Strings[0];
                            }
                        }
                        else
                        {
                            if(fNote->fShow)
                            {
                                for(int j=0; j<=iTestBinCount; j++)
                                {
                                    if(fNote->myBinCodeEdit[j]->Visible==true && fNote->myBinCodeEdit[j]->Text=="")
                                    {
                                        fNote->myBinCodeEdit[j]->Text=sCoverTrayList->Strings[i];
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    sCoverTrayList->Clear();
                    delete sCoverTrayList;

                }
                else
                {
                    asAutoCoverTrayID2=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
                }
            }
            else if(Tag==iKeyenceCoverTrayID_AUTO3)
            {
                bKeyenceResultOK[8]=true;

                if(IniConfig.bA65_BundleIDList && IniConfig.bP53_ForcedScanBinCodeOfUnloader)  //JerryYang 20240821 : 自動讀取BIN LABEL
                {
                    TStringList *sCoverTrayList=new TStringList;
                    sCoverTrayList->Clear();
                    sCoverTrayList->CommaText=EthernetBuffer;

                    for(int i=0; i<sCoverTrayList->Count; i++)
                    {
                        if(i==0)
                        {
                            if(fNote->fShow &&
                               ((fNote->bAutoIsScanBinLabel[0]==true && (fNote->edErrorCode->Text=="MES1120" || fNote->edErrorCode->Text=="MES1124")) ||
                                (fNote->bAutoIsScanBinLabel[1]==true && (fNote->edErrorCode->Text=="MES1220" || fNote->edErrorCode->Text=="MES1224")) ||
                                (fNote->bAutoIsScanBinLabel[2]==true && (fNote->edErrorCode->Text=="MES1320" || fNote->edErrorCode->Text=="MES1324"))))
                            {

                            }
                            else
                            {
                                asAutoCoverTrayID3=sCoverTrayList->Strings[0];
                            }
                        }
                        else
                        {
                            if(fNote->fShow)
                            {
                                for(int j=0; j<=iTestBinCount; j++)
                                {
                                    if(fNote->myBinCodeEdit[j]->Visible==true && fNote->myBinCodeEdit[j]->Text=="")
                                    {
                                        fNote->myBinCodeEdit[j]->Text=sCoverTrayList->Strings[i];
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    sCoverTrayList->Clear();
                    delete sCoverTrayList;

                }
                else
                {
                    asAutoCoverTrayID3=StringReplace(EthernetBuffer, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
                }
            }
        }
        AnsiString Log;
        Log.sprintf("Recv, , %s", EthernetBuffer);
        AddKeyenceCommunicationLog(Tag, Log);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::BtTray_ID_ConnectKYClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    TimerDownCCDTrayConnect->Enabled=true;
    bEnableKeyenceChannelConnect[tempBtn->Tag]=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::BtTray_ID_DisconnectKYClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    bEnableKeyenceChannelConnect[tempBtn->Tag]=false;
    switch(tempBtn->Tag)
    {
        case 0:
            ClientSocket_Keyence1->Close();
            break;
        case 1:
            ClientSocket_Keyence2->Close();
            break;
        case 2:
            ClientSocket_Keyence3->Close();
            break;
        case 3:
            ClientSocket_Keyence4->Close();
            break;
        case 4:
            ClientSocket_CoverTray_ID_LoaderCar->Close();
            break;
        case 5:
            ClientSocket_CoverTray_ID_Loader->Close();
            break;
        case 6:
            ClientSocket_CoverTray_ID_AUTO1Car->Close();
            break;
        case 7:
            ClientSocket_CoverTray_ID_AUTO2Car->Close();
            break;
        case 8:
            ClientSocket_CoverTray_ID_AUTO3Car->Close();
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btTray_ID_TriggerKYClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    AnsiString strSendCMD="";
    tempBtn = (TSpeedButton *)Sender;
    switch(tempBtn->Tag)
    {
        case 0:
            strSendCMD=edTray_ID_CMDKY->Text;
            SendKeyenceCommand(iLoaderID, " ", strSendCMD);
            break;
        case 1:
            strSendCMD=edTray_ID2_CMDKY->Text;
            SendKeyenceCommand(iEmptyID, " ", strSendCMD);
            break;
        case 2:
            strSendCMD=edTray_ID3_CMDKY->Text;         //20220627
            SendKeyenceCommand(iMultileID, " ", strSendCMD);
            break;
        case 3:
            strSendCMD=edTray_ID4_CMDKY->Text;
            SendKeyenceCommand(iAuto3ID, " ", strSendCMD);
            break;
        case 4:
            strSendCMD=edCoverTray_ID_LoaderCar_CMDKY->Text;
            SendKeyenceCommand(iKeyenceCoverTrayID_LoaderCar, " ", strSendCMD);
            break;
        case 5:
            strSendCMD=edCoverTray_ID_Loader_CMDKY->Text;
            SendKeyenceCommand(iKeyenceCoverTrayID_Loader, " ", strSendCMD);
            break;
        case 6:
            strSendCMD=edCoverTray_ID_AUTO1Car_CMDKY->Text;         //20220627
            SendKeyenceCommand(iKeyenceCoverTrayID_AUTO1, " ", strSendCMD);
            break;
        case 7:
            strSendCMD=edCoverTray_ID_AUTO2Car_CMDKY->Text;
            SendKeyenceCommand(iKeyenceCoverTrayID_AUTO2, " ", strSendCMD);
            break;
        case 8:
            strSendCMD=edCoverTray_ID_AUTO3Car_CMDKY->Text;
            SendKeyenceCommand(iKeyenceCoverTrayID_AUTO3, " ", strSendCMD);
            break;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoTrayIDKeyence()
{
    int &Task=iInitialTrayIDTask;
    bool bResult=false;
    int ret;
    static int iRetryCount=0;
//    static int iRetryCountExpTimo=0;
    static bool bHasError=false;
    AnsiString sCCDCommand, Log="";

    switch(Task)
    {
        case 1:
            AddKeyenceCommunicationLog(iLoaderID, "<<Tray ID Cycle, Cycle start.------------------");
            asTrayIDData1="";
            bKeyenceResultOK[iLoaderID]=false;
            sCCDCommand="LON";
            SendKeyenceCommand(iLoaderID, " ", sCCDCommand);
            TMAResultDelay[iLoaderID].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            Task=1000;
            break;
        case 1000:
            if(bKeyenceResultOK[iLoaderID]==true) //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                asTrayIDData1="TEST1";
                Task=2000;
            }
            else if(TMAResultDelay[iLoaderID].Off())  //讀取超時
            {
                if(bKeyenceResultOK[iLoaderID]==false)
                {
                    sCCDCommand="LOFF";
                    SendKeyenceCommand(iLoaderID, " ", sCCDCommand);
                    AddKeyenceCommunicationLog(iLoaderID, "Tray ID Action 1000, Inspection time out.");
                    bHasError=true;
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iTrayAutoRetry)                       //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    Task=3000;
                }
                bHasError=false;
            }
            else
            {
                labTrayIDStatus->Caption="Tray ID Inspection end !";
                AddKeyenceCommunicationLog(iLoaderID, "Tray ID >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 3000:
            Log.sprintf("Tray ID Alarm 3000, Tray ID read error");
            AddKeyenceCommunicationLog(iLoaderID, Log);
            ret=ShowErrorMessage("WAR0973", K_RETRY|K_SKIP, MTrayX, false);
            iRetryCount=0;
            bHasError=false;
            if(ret==K_RETRY)
            {
//                sKeyinTrayID.sprintf("Tray ID exposure Result error--RETRY reset");
//                WriteTrayMapLog(iLoaderID, sKeyinTrayID.c_str());
                Task=1;
            }
            else
            {
//                sKeyinTrayID.sprintf("Tray ID exposure Result error--SKIP reset");
                Task=5000;
            }
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
TQPF_Timer tCMDDelay;
bool TfTrayMapping::DoTrayIDKeyence2(int iFunction)
{
    int &Task=iInitialTrayIDTask;
    bool bResult=false;
    int ret, pos;
    static int iRetryCount=0;
    static bool bHasError=false;
    AnsiString sCCDCommand, Log="";

    switch(Task)
    {
        case 1:
            AddKeyenceCommunicationLog(iEmptyID, "<<Tray ID 2 Cycle, Cycle start.------------------");
            Task=500;
            break;
        case 500:
            if(iFunction==0)
                pos=Tech.iTrayXEmpty+(LoadForm->iPickUp*2)+TestIF_File.iTrayID2Shift;  //wei 20170608
            else
                pos=Prod.iXTrayAuto[1]+TestIF_File.iTrayID2Shift;  //wei 20170608
            if(TrayArmMotorMove(pos, false))    //Sam 20241206 : Tray Arm 新增 Teach 保護
            {
                Task=600;
            }
            break;
        case 600:
            sCCDCommand="LON";
            SendKeyenceCommand(iEmptyID, " ", sCCDCommand);
            TMAResultDelay[iEmptyID].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            tCMDDelay.SetMSAndOn(1000);
            asTrayIDData2="";
            bKeyenceResultOK[iEmptyID]=false;
            Task=700;
            break;
        case 700:
            if(tCMDDelay.Off())
            {
                tCMDDelay.SetMSAndOn(1000);
                Task=800;
            }
            break;
        case 800:
            if(tCMDDelay.Off())
            {
                Task=1000;
            }
            break;
        case 1000:
            if(bKeyenceResultOK[iEmptyID]==true) //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                asTrayIDData2.sprintf("%d",(rand()%2000)+1000);
                Task=2000;
            }
            else if(TMAResultDelay[iEmptyID].Off())  //讀取超時
            {
                if(bKeyenceResultOK[iEmptyID]==false)
                {
                    sCCDCommand="LOFF";
                    SendKeyenceCommand(iEmptyID, " ", sCCDCommand);
                    AddKeyenceCommunicationLog(iEmptyID, "Tray ID 2 Action 1000, Inspection time out.");
                    bHasError=true;
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iTrayAutoRetry)                       //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    Task=3000;
                }
                bHasError=false;
            }
            else
            {
                labTrayIDStatus->Caption="Tray ID 2 Inspection end !";
                AddKeyenceCommunicationLog(iEmptyID, "Tray ID 2 >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 3000:
            Log.sprintf("Tray ID 2 Alarm 3000, Tray ID read error");
            AddKeyenceCommunicationLog(iEmptyID, Log);
            ret=ShowErrorMessage("WAR0987", K_RETRY|K_SKIP, MTrayX, false);
            iRetryCount=0;
            bHasError=false;
            if(ret==K_RETRY)
            {
                Task=1;
            }
            else
            {
                asTrayIDData2="ERROR SKIP";
                Task=5000;
            }
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
int  iMultileTrayIDKeyence3Task=1;
//------------------------------------------------------------------------------
void InitialMultileTrayIDKeyence3Task()
{
    iMultileTrayIDKeyence3Task=1;
}
//------------------------------------------------------------------------------
bool TfTrayMapping::DoMultileTrayIDKeyence3()
{
    int &Task=iMultileTrayIDKeyence3Task;
    bool bResult=false;
//    int ret;
    static int iRetryCount=0;
    static bool bHasError=false;
    AnsiString sCCDCommand, Log="";

    switch(Task)
    {
        case 1:
            AddKeyenceCommunicationLog(iMultileID, "<<Multile Tray ID Cycle, Cycle start.------------------");
            asTrayIDData3="";
            bKeyenceResultOK[iMultileID]=false;
            iRetryCount=0;
            Task=100;
            break;
        case 100:
            sCCDCommand="LON";
            SendKeyenceCommand(iMultileID, " ", sCCDCommand);
            TMMultileTrayIDDelay.SetMSAndOn(10000);     //這個是讀取Time Out用的
            Task=1000;
            break;
        case 1000:
            if(bKeyenceResultOK[iMultileID]==true) //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                asTrayIDData3="TEST3";
                Task=2000;
            }
            else if(TMMultileTrayIDDelay.Off())  //讀取超時
            {
                if(bKeyenceResultOK[iMultileID]==false)
                {
                    sCCDCommand="LOFF";
                    SendKeyenceCommand(iMultileID, " ", sCCDCommand);
                    AddKeyenceCommunicationLog(iMultileID, "Multile Tray ID Action 1000, Inspection time out.");
                    bHasError=true;
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<1)
                {
                    iRetryCount++;
                    Task=100;
                }
                else
                {
                    Task=3000;
                }
                bHasError=false;
            }
            else
            {
                labTrayIDStatus->Caption="Multile Tray ID Inspection end !";
                AddKeyenceCommunicationLog(iMultileID, "Multile Tray ID >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 3000:
            Log.sprintf("Multile Tray ID Alarm 3000, Tray ID read error");
            AddKeyenceCommunicationLog(iMultileID, Log);
            iRetryCount=0;
            bHasError=false;
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoTrayIDKeyence4()
{
    int &Task=iInitialTrayIDTask;
    bool bResult=false;
    int ret;
    static int iRetryCount=0;
    static bool bHasError=false;
    AnsiString sCCDCommand, Log="";

    switch(Task)
    {
        case 1:
            AddKeyenceCommunicationLog(iAuto3ID, "<<Tray ID Cycle, Cycle start.------------------");
            sCCDCommand="LON";
            bKeyenceResultOK[iAuto3ID]=false;
            SendKeyenceCommand(iAuto3ID, " ", sCCDCommand);
            TMAResultDelay[iAuto3ID].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            asTrayIDData1="";
            asAuto3TrayIDData1="";
            Task=1000;
            break;
        case 1000:
            if(bKeyenceResultOK[iAuto3ID]==true) //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                asTrayIDData1="TEST1";
                Task=2000;
            }
            else if(TMAResultDelay[iAuto3ID].Off())  //讀取超時
            {
                if(bKeyenceResultOK[iAuto3ID]==false)
                {
                    sCCDCommand="LOFF";
                    SendKeyenceCommand(iAuto3ID, " ", sCCDCommand);
                    AddKeyenceCommunicationLog(iAuto3ID, "Tray ID Action 1000, Inspection time out.");
                    bHasError=true;
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true)
            {
                if(iRetryCount<TestIF_File.iTrayAutoRetry)                       //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    Task=3000;
                }
                bHasError=false;
            }
            else
            {
                labTrayIDStatus->Caption="Tray ID Inspection end !";
                AddKeyenceCommunicationLog(iAuto3ID, "Tray ID >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 3000:
            Log.sprintf("Tray ID Alarm 3000, Tray ID read error");
            AddKeyenceCommunicationLog(iAuto3ID, Log);
            ret=ShowErrorMessage("WAR0973", K_RETRY|K_SKIP, MTrayX, false);
            iRetryCount=0;
            bHasError=false;
            if(ret==K_RETRY)
            {
//                sKeyinTrayID.sprintf("Tray ID exposure Result error--RETRY reset");
//                WriteTrayMapLog(iLoaderID, sKeyinTrayID.c_str());
                Task=1;
            }
            else
            {
//                sKeyinTrayID.sprintf("Tray ID exposure Result error--SKIP reset");
                Task=5000;
            }
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edTray_ID_PortMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void TfTrayMapping::RecordMsgLaser(AnsiString asMsg)
{
    AddCCDTrayCommunicationLog(90, asMsg);
    asMsg=FormatDateTime("yyyy-mm-dd HH:MM:ss ", Now())+asMsg;
    ShowMSDCCommLog(asMsg);
}
//---------------------------------------------------------------------------
void TfTrayMapping::RecordErrorMsgLaser(AnsiString asMsg)
{
    asErrorLaserScan=asMsg;
    RecordMsgLaser(asMsg);
}
//---------------------------------------------------------------------------
void TfTrayMapping::ShowMSDCCommLog(AnsiString asStr)
{
    if(mLoaderDCCommLog->Lines->Count>50)
        mLoaderDCCommLog->Lines->Clear();
    mLoaderDCCommLog->Lines->Add(asStr);
}
//---------------------------------------------------------------------------
void TfTrayMapping::ClearAllAOI()
{
    for(int i=0;i<GetAOISize();i++)
    {
        delete GetAOI(i);
    }
}
//---------------------------------------------------------------------------
TfAOI* TfTrayMapping::GetAOI(int iIndex)
{
    iIndex=(iIndex>=GetAOISize() || iIndex<0)?0:iIndex;
    return mapAOI[iIndex];
}
//---------------------------------------------------------------------------
TfAOI* TfTrayMapping::GetAOI(AnsiString asName)
{
    for(int i=0;i<GetAOISize();i++)
    {
        if(GetAOI(i)->asName.Pos(asName)>0)
            return GetAOI(i);
    }
    return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::BtLoaderDC_ConnectClick(TObject *Sender)
{
    if(GetNowAOI()->socketClient->IsConnected())
        GetNowAOI()->socketClient->Close();
    else
        GetNowAOI()->DoCommuncation();
    UpdateConnectionState(BtLoaderDC_Connect);
}
//---------------------------------------------------------------------------
void TfTrayMapping::UpdateConnectionState(TBitBtn * btn)
{
    if(GetNowAOI()==NULL)return;
    if(GetNowAOI()->socketClient->IsConnected())
    {
        btn->Caption="Disconnect";
        btn->Font->Color=clRed;
    }
    else
    {
        btn->Caption="Connect";
        btn->Font->Color=clGreen;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btLoaderDC_TriggerClick(TObject *Sender)
{
    if(GetNowAOI()==NULL)return;
    GetNowAOI()->SendCommandSocket(edLoaderDC_CMD->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnSimulateRemainICClick(TObject *Sender)
{
    if(GetNowAOI()==NULL || SystemStart)
    {
        ShowMessage("設備運作中無法模擬");
        return;
    }
    else if(cbAutoLearning->Checked==true)
    {
        int ret=ShowMyMessageBox_YES_NO("Sure To Auto Learning", "確定要進行自動校正？");
        if(ret==2)
        {
            return;
        }
    }
    iLoopCount=0;
    iLoopNow=0;
    AnsiString asLoop=edLoopTimes->Text;
    if(IsNumber(asLoop))
        iLoopCount=atoi(asLoop.c_str());
    else
        return;
    if(iLoopCount>100)
    {
        iLoopCount=100;
        edLoopTimes->Text=IntToStr(iLoopCount);
    }
    iLoopCount=(iLoopCount>100)?100:iLoopCount;
    //
    if(cbLaserScanMode->Text==AnsiString("Check Device"))
    {
        DoLaserScanFromLoader(eAOIType_TrayDeviceCheckLaser,true);
        PageControl3->ActivePageIndex=3;
    }
    else    //Loader Tray Map
    {
        DoLaserScanFromLoader(eAOIType_TrayMapLaser,true);
        PageControl3->ActivePageIndex=2;
    }
    if(GetSimulationState())
    {
        SetSimulationState(false);
    }
    else
    {
        SetSimulationState(true);
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::SetSimulationState(bool bswitch)
{
    bSimulate=bswitch;
    if(bswitch)
    {
        btnSimulateRemainIC->Caption="Running...";
    }
    else
    {
        btnSimulateRemainIC->Caption="Run";
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapping::MoveTrayArmToSafeColor()
{
    return MOT[MTrayX].MotorMove(Prod.iXTrayColor);
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoLaserScanFromLoader(int iAOIType,bool bIsFirst,bool bIsRunMoveOut)
{
    if(bIsFirst)
    {
        asErrorLaserScan="";
        iLaserScanTask=1;
        iAutoRetryCount=0;
        return true;
    }
    else
    {
        lbTaskLDC->Caption=IntToStr(iLaserScanTask);
        if(asErrorLaserScan!="")
            return true;
        switch(iLaserScanTask)
        {
            case 1://Initial
                CloseAllSocket();
                if(INSTALL_OCR_YMot==eocrYMotInstal)                            //RogerYang 20250906 : Add for MLoaderY
                {
                    iLdYSpdBackup=GetAOI(iAOIType)->SetAndBackUpLoaderYSpeed(iLoaderYCWSpeedPerc);
                    if(bIsRunMoveOut)
                    {
                        iLaserScanTask=500;
                    }
                    else
                        iLaserScanTask=1000;
                }
                else
                {
                    if(bIsRunMoveOut)
                    {
                        ReleaseLoaderBreak(iAOIType);
                        iLaserScanTask=500;
                    }
                    else
                    {
                        iLaserScanTask=1000;
                    }
                }

                return false;
            case 500:
                if(IsRunMotor())     //moveout
                {
                    GetAOI(iAOIType)->DoMoveOut(true,asErrorLaserScan);
                    iLaserScanTask=600;
                }
                else
                {
                    iLaserScanTask=1000;
                }
                return false;
            case 600:
                if(IsRunMotor())     //moveout
                {
                    if(GetAOI(iAOIType)->DoMoveOut(false,asErrorLaserScan))
                    {
                        iLaserScanTask=1000;
                    }
                }
                else
                {
                    iLaserScanTask=1000;
                }
                return false;
            case 1000:
                if(IsCommAOI())
                {
                    if(cbAutoLearning->Checked)
                        GetAOI(iAOIType)->DoAutoTuning(true,asErrorLaserScan);
                    else
                        GetAOI(iAOIType)->DoSettingBeforeMotorMove(true,asErrorLaserScan);
                }
                iLaserScanTask=2000;
                return false;
            case 2000:
                if(IsCommAOI())
                {
                    if(cbAutoLearning->Checked)
                    {
                        if(GetAOI(iAOIType)->DoAutoTuning(false,asErrorLaserScan))
                        {
                            iLaserScanTask=2500;                                //RogerYang 20250830 : Add殘料檢(台車)
                        }
                    }
                    else
                    {
                        if(GetAOI(iAOIType)->DoSettingBeforeMotorMove(false,asErrorLaserScan))
                        {
                            iLaserScanTask=2500;                                //RogerYang 20250830 : Add殘料檢(台車)
                        }
                    }
                }
                else
                {
                    iLaserScanTask=2500;                                        //RogerYang 20250830 : Add殘料檢(台車)
                }
                return false;
            case 2500:                                                          //RogerYang 20250830 : Add殘料檢(台車)
                if(INSTALL_OCR_YMot==eocrYMotInstal)
                    iLaserScanTask=2600;
                else
                    iLaserScanTask=3000;
                break;
            case 2600:                                                          //RogerYang 20250830 : Add殘料檢(台車)
                 if(MOT[MLoaderY].MotorMove(Prod.iMLoaderYOCRPos))
                 {
                    Cylinder[C_LoaderEdgePush].On();                            //RogerYang 20251028 : Add
                    Cylinder[C_TrayY_Fixer].On();
                    iLaserScanTask=3000;
                }
                break;
            case 3000:
                if(IsRunMotor())     //movein
                {
                    GetAOI(iAOIType)->DoMoveIn(true,asErrorLaserScan);
                    iLaserScanTask=4000;
                }
                else
                {
                    iLaserScanTask=5000;
                }
                return false;
            case 4000:
                if(IsRunMotor())     //movein
                {
                    if(GetAOI(iAOIType)->DoMoveIn(false,asErrorLaserScan))
                    {
                        iLaserScanTask=5000;
                    }
                }
                else
                {
                    iLaserScanTask=5000;
                }
                return false;
            case 5000:
                if(IsCommAOI())
                {
                    if(cbAutoLearning->Checked)
                    {
                        iLaserScanTask=7000;
                    }
                    else
                    {
                        GetAOI(iAOIType)->DoGetValueAfterMotorArrival(true,asErrorLaserScan);
                    }
                }
                iLaserScanTask=6000;
                return false;
            case 6000:
                if(IsCommAOI())
                {
                    if(GetAOI(iAOIType)->DoGetValueAfterMotorArrival(false,asErrorLaserScan))
                    {
                        iLaserScanTask=7000;
                    }
                }
                else
                {
                    iLaserScanTask=7000;
                }
                return false;
            case 7000:
                if(IsRunMotor())     //moveout
                {
                    GetAOI(iAOIType)->DoMoveOut(true,asErrorLaserScan);
                    iLaserScanTask=7100;
                }
                else
                    iLaserScanTask=8000;
                return false;
            case 7100:
                if(DoRunAtInitialStart(eAOIType_TrayDeviceCheckLaser)==true &&
                   GetRealRunInitialStart(eAOIType_TrayDeviceCheckLaser)==false)
                {
                    //
                }
                else if(IsRunMotor())     //moveout
                {
                    if(GetAOI(iAOIType)->DoMoveOut(false,asErrorLaserScan))
                    {
                        iLaserScanTask=8000;
                    }
                }
                else
                {
                    iLaserScanTask=8000;
                }
                return false;
            case 8000:
                if(INSTALL_OCR_YMot==eocrYMotInstal)                            //RogerYang 20251028 : Add
                {
                    Cylinder[C_LoaderEdgePush].Off();
                    Cylinder[C_TrayY_Fixer].Off();
                }
                iLaserScanTask=9999;
                return false;
            case 9999:// finish
                if(INSTALL_OCR_YMot==eocrYMotInstal)                            //RogerYang 20250906 : Add for MLoaderY
                {
                    GetAOI(iAOIType)->SetAndBackUpLoaderYSpeed(iLdYSpdBackup);
                }
                return true;
            default:
                RecordErrorMsgLaser(AnsiString(__FUNC__)+" Wrong Step="+IntToStr(iLaserScanTask));
                return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::IsNumber(AnsiString& asValue)
{
    asValue=asValue.Trim();
    for(int i=0;i<asValue.Length();i++)
    {
        if (asValue.c_str()[i] >= '0' && asValue.c_str()[i] <= '9')
        {/*pass*/}
        else
            return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::edLoopTimesKeyPress(TObject *Sender,
      char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::cbLaserScanModeChange(TObject *Sender)
{
    ResetcbLaserScanMode();
}
//---------------------------------------------------------------------------
TfAOI* TfTrayMapping::GetNowAOI()
{
    if(USE_TRAY_MAPPING==etmDeviceRemain)
    {
        if(CosFunction.bTrayDeviceCheckFromLoader && cbLaserScanMode->Text==AnsiString("Check Device"))
        {
            return GetAOI(eAOIType_TrayDeviceCheckLaser);
        }
        else//Tray Map
        {
            return GetAOI(eAOIType_TrayMapLaser);
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------
void TfTrayMapping::CloseAllSocket()
{
    for(int i=0;i<GetAOISize();i++)
        GetAOI(i)->socketClient->Close();
}
//---------------------------------------------------------------------------
bool TfTrayMapping::IsRunMotor()
{
    if(fShow)
    {
        if(cbPassActive->Checked)
            return false;
    }
    else if(LastSet.iRealDummy==DUMMY)
        return false;
    return true;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::IsCommAOI()
{
    return (cbPassAOI->Checked && fShow)==false;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::IsRunTrayMapLaser()
{
    return (USE_TRAY_MAPPING==etmDeviceRemain &&
            CosFunction.bTrayMapFromLoader &&
            cbEnableTrayMapLaser->Checked &&
            (LastSet.iRealDummy!=DUMMY));
}
//---------------------------------------------------------------------------
bool TfTrayMapping::IsRunDeviceRemainLaser()
{
    return (USE_TRAY_MAPPING==etmDeviceRemain &&
            CosFunction.bTrayDeviceCheckFromLoader &&
            cbEnableDeviceRemainLaser->Checked  &&
            (LastSet.iRealDummy!=DUMMY));
}
//---------------------------------------------------------------------------
void TfTrayMapping::InitialcbLaserScanMode()
{
    cbLaserScanMode->Items->Clear();
    for(int i=0;i<GetAOISize();i++)
    {
        cbLaserScanMode->Items->Add(GetAOI(i)->asName);
    }
    cbLaserScanMode->Text=(GetAOISize()>0)?(GetAOI(0)->asName):AnsiString("");
}
//---------------------------------------------------------------------------
void TfTrayMapping::ResetcbLaserScanMode()
{

    TfAOI* aoi=GetAOI(cbLaserScanMode->Text);
    if(aoi==NULL)
        return;

    edLoaderDC_Address->Text=aoi->socketClient->GetSocketAddress();
    edLoaderDC_Port->Text=aoi->socketClient->GetSocketPort();
}
//---------------------------------------------------------------------------
void TfTrayMapping::InitialAOIGroup()
{
    if(CosFunction.bTrayDeviceCheckFromLoader)
    {
        TfTrayDeviceCheckFromLoader *_f=new TfTrayDeviceCheckFromLoader("Check Device");
        AddAOIMemeber(_f);
    }
    if(CosFunction.bTrayMapFromLoader)
    {
        TfTrayMapFromLoader *_f=new TfTrayMapFromLoader("Tray Map");
        _f->SetTrayIC(mtTrayMap);
        AddAOIMemeber(_f);
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::ShowLaserScanSheet(TTabSheet *tsShow)
{
    PageControl1->ActivePage=tsShow;
}
//---------------------------------------------------------------------------
void TfTrayMapping::AddAOIMemeber(TfAOI *aoi)
{
    aoi->RecordErrorMsgLaser=RecordErrorMsgLaser;
    aoi->RecordMsgLaser=RecordMsgLaser;
    mapAOI[GetAOISize()]=aoi;
}
//---------------------------------------------------------------------------
void TfTrayMapping::ReleaseLoaderBreak(int iAOIType,bool bswitch)
{
    int moveTrack=GetAOI(iAOIType)->GetMoveTrack();
    if(bswitch)
    {
        if(moveTrack==GetAOI(iAOIType)->eMT_Loader)
        {
            Cylinder[C_LoaderEdgePush].Off();
            Cylinder[C_LoaderUpPress].Off();  //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
            Cylinder[C_TrayY_Fixer].Off();
        }
        else if(moveTrack==GetAOI(iAOIType)->eMT_Color)
        {
            Cylinder[C_Color_Fix].Off();   //Open Fix Supply Try Fix Cylinder
        }
    }
    else
    {
        if(moveTrack==GetAOI(iAOIType)->eMT_Loader)
        {
            Cylinder[C_LoaderEdgePush].On();
            Cylinder[C_LoaderUpPress].On();  //JerryYang 20181120 (Steven) : (Steven) : 獨立控制loader壓tray
            Cylinder[C_TrayY_Fixer].On();
        }
        else if(moveTrack==GetAOI(iAOIType)->eMT_Color)
        {
            Cylinder[C_Color_Fix].On();   //Open Fix Supply Try Fix Cylinder
        }
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapping::GetTrayDeviceCheckIsRemainIC()
{
    return GetAOI(eAOIType_TrayDeviceCheckLaser)->IsRemainIC();
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoGetValueAfterMotorArrival(int iMode,bool bIsFirst,AnsiString &asError)
{
    return GetAOI(iMode)->DoGetValueAfterMotorArrival(bIsFirst,asError);
}
//---------------------------------------------------------------------------
bool TfTrayMapping::GetTrayMapHasNullIC()
{
    return GetAOI(eAOIType_TrayMapLaser)->HasNullIC(mtTrayMap);
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoSettingBeforeMotorMove(int iMode,bool bIsFirst,AnsiString &asError)
{
    return GetAOI(iMode)->DoSettingBeforeMotorMove(bIsFirst,asError);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnLoadFileClick(TObject *Sender)
{
    GetNowAOI()->DoAOI_LoadFile(true);
    GetNowAOI()->DoAOI_LoadFile(false);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnClearBufferClick(TObject *Sender)
{
    GetNowAOI()->DoAOI_ClearBuffer(true);
    GetNowAOI()->DoAOI_ClearBuffer(false);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnGrabClick(TObject *Sender)
{
    GetNowAOI()->DoAOI_Function(true);
    GetNowAOI()->DoAOI_Function(false);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnGetValueClick(TObject *Sender)
{
    GetNowAOI()->DoGetValue(true);
    GetNowAOI()->DoGetValue(false);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::cbSimulationAOICommandClick(TObject *Sender)
{
    bSimulationAOICommand=cbSimulationAOICommand->Checked;
    for(int i=0;i<GetAOISize();i++)
    {
        GetAOI(i)->bSimulationAOICommand=bSimulationAOICommand;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnSetLotClick(TObject *Sender)
{
    GetNowAOI()->DoSetLot(true);
    GetNowAOI()->DoSetLot(false);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnCreateFileClick(TObject *Sender)
{
    GetNowAOI()->DoCreateFile(true);
    GetNowAOI()->DoCreateFile(false);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnGoldenImageClick(TObject *Sender)
{
    GetNowAOI()->DoGoldenImage(true, 0);                                        //RogerYang 20250830 : 殘料檢變更格式
    GetNowAOI()->DoGoldenImage(false, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnMoveInClick(TObject *Sender)
{
    static bool _b=true;
    TrayMoveIn(_b, 0, Prod.iMLoaderYSurePos);
    _b=!_b;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnMoveOutClick(TObject *Sender)
{
    static bool _b=true;
    TrayMoveOut(_b, 0, Prod.iMLoaderYOCRPos);
    _b=!_b;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::Label18MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
        iSettingCount++;
    else if(Button==mbLeft)
    {
        if(iSettingCount==2)
        {
            tsCommand->TabVisible=!tsCommand->TabVisible;
            btnAddRandom->Visible=!btnAddRandom->Visible;
        }
        iSettingCount=999;
    }
    else
        iSettingCount=999;
    if(iSettingCount>2)
        iSettingCount=0;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnAddRandomClick(TObject *Sender)
{
    yieldRemainIC.AddYieldData(true);
}
//---------------------------------------------------------------------------
void TfTrayMapping::SetYieldDatas(AnsiString sData)
{
    int colPos=0;
    SetValueTrayDeviceYield(colPos++,0,yieldRemainIC.GetFileNameNoExt());
    SetValueTrayDeviceYield(colPos++,0,sData);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnRefreshYieldClick(TObject *Sender)
{
    SetYieldDatas(yieldRemainIC.GetYield());                                    //Jimmychiu 20231023 : 增加良率紀錄
}
//---------------------------------------------------------------------------
void TfTrayMapping::InitialTrayDeviceYield()
{
    sgTrayDeviceYield->ColCount=3;
    sgTrayDeviceYield->ColWidths[0]=50;   //Yield
    sgTrayDeviceYield->ColWidths[1]=150;  //MO
    sgTrayDeviceYield->ColWidths[2]=60;   //%
    sgTrayDeviceYield->RowCount=2;
    //title
    int colPos=0;
    SetTtileTrayDeviceYield(colPos++,0,"Yield");
    SetTtileTrayDeviceYield(colPos++,0,"MO");
    SetTtileTrayDeviceYield(colPos++,0,"%");
}
//---------------------------------------------------------------------------
void TfTrayMapping::SetTtileTrayDeviceYield(int icol,int irow,AnsiString sValue)
{
    int iscol=0,isrow=0;
    sgTrayDeviceYield->Cells[iscol+icol][isrow+irow]=sValue;
}
//---------------------------------------------------------------------------
void TfTrayMapping::SetValueTrayDeviceYield(int icol,int irow,AnsiString sValue)
{
    int iscol=1,isrow=1;
    sgTrayDeviceYield->Cells[iscol+icol][isrow+irow]=sValue;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::CheckRunAutoRetry(int iAOIType)
{
    if(IsRunDeviceRemainLaser() &&
       GetAOI(iAOIType)->bEnableRetryCount==True &&
       iAutoRetryCount<GetAOI(iAOIType)->iRemainICLaserRetryCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoMoveOut(int iAOIType,bool bIsFirst)
{
    return GetAOI(iAOIType)->DoMoveOut(bIsFirst,asErrorLaserScan);
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoMoveIn(int iAOIType,bool bIsFirst)
{
    return GetAOI(iAOIType)->DoMoveIn(bIsFirst,asErrorLaserScan);
}
//---------------------------------------------------------------------------
void TfTrayMapping::ReadFile_ScanLine()
{
    if(USE_TRAY_MAPPING==etmDeviceRemain)
    {
        AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
        AnsiString asGetAddress="", asGetPort="";
        bool bUpdateUI=false;
        iLoaderYCWSpeedPerc=CheckAndReadIniDataGeneral("System",   "LaserScan_LoaderYCWSpeed",DefaultLaserScanLoaderCWSpeed);
        edLoaderCWSpeed->Text=IntToStr(iLoaderYCWSpeedPerc);
        if(CosFunction.bTrayDeviceCheckFromLoader==true)
        {
            bUpdateUI=(cbLaserScanMode->Text==AnsiString("Check Device"));
            ReadFile_ScanLine(szDir,eAOIType_TrayDeviceCheckLaser,"Tray_Decive_Check",bUpdateUI);
            cbEnableDeviceRemainLaser->Checked  =ReadIniData(szDir, "Configuration", "Enable_Check_Device_Remain_Laser",      false);
        }

        if(CosFunction.bTrayMapFromLoader==true)
        {
            bUpdateUI=(cbLaserScanMode->Text==AnsiString("Tray Map"));
            ReadFile_ScanLine(szDir,eAOIType_TrayMapLaser,"Loader_Tray_Map",bUpdateUI);
            cbEnableTrayMapLaser->Checked  =ReadIniData(szDir, "Configuration", "Enable_Tray_Mapping_Laser",                  false);
        }
        ResetcbLaserScanMode();
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::ReadFile_ScanLine(AnsiString szDir,int iAoi,AnsiString sHead,bool bUpdateUI)
{
    AnsiString asGetAddress="",asGetPort="",asTemp="";
    //Address
    //Port
    asGetAddress  = CheckAndReadIniDataGeneral("System",   AnsiString().sprintf("%s_Address",sHead),  AnsiString("127.0.0.1"));
    asGetPort     = CheckAndReadIniDataGeneral("System",   AnsiString().sprintf("%s_Port",sHead),    AnsiString("5102"));
    GetAOI(iAoi)->socketClient->SetCommParameter(asGetAddress,asGetPort);
    //WaitAoiTime
    GetAOI(iAoi)->iActiveDelayTime=CheckAndReadIniDataGeneral("System",   AnsiString().sprintf("%s_WaitAoiTime",sHead),90);
    if(bUpdateUI)
    {
        edWaitAOITimes->Text=IntToStr(GetAOI(iAoi)->iActiveDelayTime);
    }
    //RunAtInitialStart
    GetAOI(iAoi)->bNeedCheckInitialStart      =ReadIniData(szDir, "Configuration", AnsiString().sprintf("%s_RunAtInitialStart",sHead), false);
    if(bUpdateUI)
    {
        cbRunAtInitialStart->Checked=GetAOI(iAoi)->bNeedCheckInitialStart;
    }
    //Enable_Retry_Count
    GetAOI(iAoi)->bEnableRetryCount           =ReadIniData(szDir, "Configuration", AnsiString().sprintf("%s_Enable_Retry_Count",sHead),      false);
    if(bUpdateUI)
    {
        cbEnableRetryCount->Checked=GetAOI(iAoi)->bEnableRetryCount;
    }
    //Auto_Retry
    GetAOI(iAoi)->iRemainICLaserRetryCount    =ReadIniData(szDir, "Configuration", AnsiString().sprintf("%s_Auto_Retry",sHead),        0);
    if(bUpdateUI)
    {
        edRemainIC_Laser_Retry->Text=IntToStr(GetAOI(iAoi)->iRemainICLaserRetryCount);
    }
    //AutoSkip
    GetAOI(iAoi)->bRemainICLaserAutoSkip      =ReadIniData(szDir, "Configuration", AnsiString().sprintf("%s_AutoSkip",sHead), false);
    if(bUpdateUI)
    {
        cbAutoSkip->Checked=GetAOI(iAoi)->bRemainICLaserAutoSkip;
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::SaveFile_ScanLine()
{
    if(USE_TRAY_MAPPING==etmDeviceRemain)
    {
        AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
        iLoaderYCWSpeedPerc=atoi(edLoaderCWSpeed->Text.c_str());
        WriteIniDataGeneral("System",       "LaserScan_LoaderYCWSpeed",    iLoaderYCWSpeedPerc);
        if(CosFunction.bTrayDeviceCheckFromLoader==true)
        {
            WriteIniData(szDir, "Configuration", "Enable_Check_Device_Remain_Laser",        cbEnableDeviceRemainLaser->Checked);
            if(cbLaserScanMode->Text==AnsiString("Check Device"))
            {
                SaveFile_ScanLine(szDir,eAOIType_TrayDeviceCheckLaser,"Tray_Decive_Check");
            }
        }

        if(CosFunction.bTrayMapFromLoader==true)
        {
            WriteIniData(szDir, "Configuration", "Enable_Tray_Mapping_Laser",        cbEnableTrayMapLaser->Checked);
            if(cbLaserScanMode->Text==AnsiString("Tray Map"))
            {
                SaveFile_ScanLine(szDir,eAOIType_TrayMapLaser,"Loader_Tray_Map");
            }
        }
        ReadFile_ScanLine();
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::SaveFile_ScanLine(AnsiString szDir,int iAoi,AnsiString sHead)
{
    AnsiString asTemp="";
    //Address
    //Port
    GetAOI(iAoi)->socketClient->SetCommParameter(edLoaderDC_Address->Text,edLoaderDC_Port->Text);
    WriteIniDataGeneral("System",       AnsiString().sprintf("%s_Address",sHead),           GetAOI(iAoi)->socketClient->GetSocketAddress());
    WriteIniDataGeneral("System",       AnsiString().sprintf("%s_Port",sHead),              GetAOI(iAoi)->socketClient->GetSocketPort());
    //WaitAoiTime
    asTemp=edWaitAOITimes->Text;
    GetAOI(iAoi)->iActiveDelayTime=atoi(asTemp.c_str());
    WriteIniDataGeneral("System",       AnsiString().sprintf("%s_WaitAoiTime",sHead),       GetAOI(iAoi)->iActiveDelayTime);
    //RunAtInitialStart
    GetAOI(iAoi)->bNeedCheckInitialStart=cbRunAtInitialStart->Checked;
    WriteIniData(szDir, "Configuration",AnsiString().sprintf("%s_RunAtInitialStart",sHead), GetAOI(iAoi)->bNeedCheckInitialStart);
    //Enable_Retry_Count
    GetAOI(iAoi)->bEnableRetryCount=cbEnableRetryCount->Checked;
    WriteIniData(szDir, "Configuration",AnsiString().sprintf("%s_Enable_Retry_Count",sHead),cbEnableRetryCount->Checked);
    //Auto_Retry
    asTemp=edRemainIC_Laser_Retry->Text;
    GetAOI(iAoi)->iRemainICLaserRetryCount=atoi(asTemp.c_str());
    WriteIniData(szDir, "Configuration",AnsiString().sprintf("%s_Auto_Retry",sHead),        GetAOI(iAoi)->iRemainICLaserRetryCount);
    //AutoSkip
    GetAOI(iAoi)->bRemainICLaserAutoSkip=cbAutoSkip->Checked;
    WriteIniData(szDir, "Configuration",AnsiString().sprintf("%s_AutoSkip",sHead),          GetAOI(iAoi)->bRemainICLaserAutoSkip);
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoAutoSkip(int iAOIType)
{
    return GetAOI(iAOIType)->bRemainICLaserAutoSkip;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoRunAtInitialStart(int iAOIType)
{
    if(GetAOI(iAOIType)==NULL)
    {
        return false;
    }
    else
    {
        return GetAOI(iAOIType)->bNeedCheckInitialStart;
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::SetRealRunInitialStart(int iAOIType,bool bSwitch)
{
    GetAOI(iAOIType)->bRealRunCheckInitialStart=bSwitch;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::GetRealRunInitialStart(int iAOIType)
{
    return GetAOI(iAOIType)->bRealRunCheckInitialStart;
}
//---------------------------------------------------------------------------
void TfTrayMapping::SearchPageCtrl(AnsiString sFilter)
{
    for(int i=0;i<PageControl3->PageCount;++i)
    {
        AnsiString sPageName=PageControl3->Pages[i]->Caption;
        if(sPageName.Pos(sFilter)>0)
        {
            PageControl3->ActivePage=PageControl3->Pages[i];
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TfTrayMapping::ShowCheckDevicePage()
{
    this->Show();
    SearchPageCtrl("Check Device Result");
    ShowMessage("Please Run Check Device When Initial Start");
}
//---------------------------------------------------------------------------
void TfTrayMapping::ShowCheckDeviceResult(AnsiString sValue)
{
    lbResult->Caption=sValue;
}
int  iReadCIDAction=ePortTotal;
int  iCoverTrayIDTask[iKeyenceTotalFunction]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
//------------------------------------------------------------------------------
void InitialCoverTrayIDTask(int iPos)                                           //wei 20161219 Tray Mapping
{
    iCoverTrayIDTask[iPos]=1;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoCoverTrayID(int iPos, bool bAlarm)
{
    bool bResult=false;
    if(USE_COVER_TRAYID==tCIDNotUse)
    {
        bResult=DoCoverTrayIDKeyence(iPos, bAlarm);
    }
    else if(USE_COVER_TRAYID==tCID_NFC)
    {
        if(tmrNFC->Enabled==false)
        {
            iNFCTask=1;
            tmrNFC->Enabled=true;
        }

        if(iPos==iKeyenceCoverTrayID_LoaderCar)
            bResult=DoLoadCarRotArmReadRFID(bAlarm);
        else
            bResult=DoCoverTrayID_NFC(iPos, bAlarm);
    }

    return bResult;
}
//---------------------------------------------------------------------------
bool TfTrayMapping::DoCoverTrayIDKeyence(int iPos, bool bAlarm)
{
    int &Task=iCoverTrayIDTask[iPos];
    bool bResult=false;
    int ret;
    static int iRetryCount=0;
    static bool bHasError=false;
    AnsiString sCCDCommand, Log="", asTempID="";

    switch(Task)
    {
        case 1:
            AddKeyenceCommunicationLog(iPos, "<<Cover Tray ID start.------------------");
            Task=600;
            break;
        case 600:
            bHasError=false;
            sCCDCommand="||>trigger on";
            SendKeyenceCommand(iPos, " ", sCCDCommand);
            TMAResultDelay[iPos].SetMSAndOn(TestIF_File.iTrayGetResultTimeOut);     //這個是讀取Time Out用的
            tCMDDelay.SetMSAndOn(1000);

            if(iPos==iKeyenceCoverTrayID_LoaderCar)
            {
                #ifdef SOFT_SIMULTE
                    if(asTrayIDDataCorverLoader=="ERROR")
                        asTrayIDDataCorverLoader="";
                #else
                    asTrayIDDataCorverLoader="";
                #endif
            }
            else if(iPos==iKeyenceCoverTrayID_Loader)
            {
                asTrayIDDataLoader2="";
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO1)
            {
                asAutoCoverTrayID1="";
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO2)
            {
                asAutoCoverTrayID2="";
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO3)
            {
                asAutoCoverTrayID3="";
            }

            bKeyenceResultOK[iPos]=false;
            Task=1000;
            break;
        case 1000:
            if(bKeyenceResultOK[iPos]==true)                                    //讀取完成
            {
                Task=2000;
            }
            else if(cbBarCodeSimulate->Checked)
            {
                asTempID.sprintf("%d", (rand()%2000)+1000);

                if(iPos==iKeyenceCoverTrayID_LoaderCar)
                {
                    if(asTrayIDDataCorverLoader=="")
                    {
                        asTrayIDDataCorverLoader="GE6TS477";
                    }
                    else if(asTrayIDDataCorverLoader=="GE6TS477")
                    {
                        asTrayIDDataCorverLoader="GE6TS478";
                    }
                    else if(asTrayIDDataCorverLoader=="GE6TS478")
                    {
                        asTrayIDDataCorverLoader="GE6TS479";
                    }
                    else if(asTrayIDDataCorverLoader=="GE6TS479")
                    {
                        asTrayIDDataCorverLoader="GE6TS477";
                    }
                }
                else if(iPos==iKeyenceCoverTrayID_Loader)
                {
                    asTrayIDDataLoader2="ERROR";
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO1)
                {
                    asAutoCoverTrayID1=asTempID;
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO2)
                {
                    asAutoCoverTrayID2=asTempID;
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO3)
                {
                    asAutoCoverTrayID3=asTempID;
                }
                Task=2000;
            }
            else if(TMAResultDelay[iPos].Off())                                 //讀取超時
            {
                if(bKeyenceResultOK[iPos]==false)
                {
                    AddKeyenceCommunicationLog(iPos, "Tray ID 2 Action 1000, Inspection time out.");
                    bHasError=true;
                    fNote->bScanBinLabel=false;                                 //JerryYang 20240821 : 自動讀取BIN LABEL
                    fNote->bAutoIsScanBinLabel[0]=false;                                  //JerryYang 20250429 : fix Auto In/Out
                    fNote->bAutoIsScanBinLabel[1]=false;
                    fNote->bAutoIsScanBinLabel[2]=false;
                }
                Task=2000;
            }
            break;
        case 2000:
            if(bHasError==true ||
              (iPos==iKeyenceCoverTrayID_LoaderCar && (asTrayIDDataCorverLoader=="ERROR" || asTrayIDDataCorverLoader=="NOREAD"|| asTrayIDDataCorverLoader.AnsiPos("Pass_")!=0 || asTrayIDDataCorverLoader.AnsiPos("Fail_")!=0)) ||
              (iPos==iKeyenceCoverTrayID_Loader    && (asTrayIDDataLoader2=="ERROR" || asTrayIDDataLoader2=="NOREAD"|| asTrayIDDataLoader2.AnsiPos("Pass_")!=0 || asTrayIDDataLoader2.AnsiPos("Fail_")!=0)) ||
              (iPos==iKeyenceCoverTrayID_AUTO1     && (asAutoCoverTrayID1=="ERROR" || asAutoCoverTrayID1=="NOREAD" || asAutoCoverTrayID1.AnsiPos("Pass_")!=0 || asAutoCoverTrayID1.AnsiPos("Fail_")!=0)) ||
              (iPos==iKeyenceCoverTrayID_AUTO2     && (asAutoCoverTrayID2=="ERROR" || asAutoCoverTrayID2=="NOREAD" || asAutoCoverTrayID2.AnsiPos("Pass_")!=0 || asAutoCoverTrayID2.AnsiPos("Fail_")!=0)) ||
              (iPos==iKeyenceCoverTrayID_AUTO3     && (asAutoCoverTrayID3=="ERROR" || asAutoCoverTrayID3=="NOREAD" || asAutoCoverTrayID3.AnsiPos("Pass_")!=0 || asAutoCoverTrayID3.AnsiPos("Fail_")!=0)))            //JerryYang 20250429 : fix Auto In/Out
            {
                if(iRetryCount<TestIF_File.iTrayAutoRetry)                      //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task=1;
                }
                else
                {
                    Task=3000;
                }
                bHasError=false;
                fNote->bScanBinLabel=false;                          //JerryYang 20250429 : fix Auto In/Out
            }
            else
            {
                labTrayIDStatus->Caption="Tray ID 2 Inspection end !";
                AddKeyenceCommunicationLog(iPos, "Tray ID 2 >>Cycle 2000, Inspection end.---------------\r\n");
                Task=5000;
            }
            break;
        case 3000:
            Log.sprintf("Tray ID 2 Alarm 3000, Tray ID read error");
            AddKeyenceCommunicationLog(iPos, Log);

            if(bAlarm==true)                                                    //JerryYang 202040503 :
            {
                ret=K_RETRY;
                if(iPos==iKeyenceCoverTrayID_AUTO1)
                {
                    ShowMyMessage("AUTO 1 TRAY ID READ ERROR!");
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO2)
                {
                    ShowMyMessage("AUTO 2 TRAY ID READ ERROR!");
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO3)
                {
                    ShowMyMessage("AUTO 3 TRAY ID READ ERROR!");
                }
                else
                {
                    ret=ShowErrorMessage("WAR16120", K_RETRY|K_SKIP, MTrayX, false);
                }
            }
            else
            {
                ret=K_SKIP;
            }

            iRetryCount=0;
            bHasError=false;
            if(ret==K_RETRY)
            {
                Task=1;
            }
            else
            {
                asTempID="ERROR";
                if(iPos==iKeyenceCoverTrayID_LoaderCar)
                {
                    asTrayIDDataCorverLoader=asTempID;
                }
                else if(iPos==iKeyenceCoverTrayID_Loader)
                {
                    asTrayIDDataLoader2=asTempID;
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO1)
                {
                    asAutoCoverTrayID1=asTempID;
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO2)
                {
                    asAutoCoverTrayID2=asTempID;
                }
                else if(iPos==iKeyenceCoverTrayID_AUTO3)
                {
                    asAutoCoverTrayID3=asTempID;
                }
                Task=5000;
            }
            break;
        case 5000:
            iRetryCount=0;
            Task=1;
            return true;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool DoCoverTrayID_NFC(int iPos, bool bAlarm)                                   //QQQ need to add
{
    int &Task=iCoverTrayIDTask[iPos];
    bool bResult=false;

    switch(Task)
    {
        case 1:
            if(iPos==iKeyenceCoverTrayID_Color)
            {
                sCoverTrayID=fTrayMapping->ldRFID->sBlockID2;
                fSortCT->pnlCoverTrayD->Caption=sCoverTrayID;
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO1)
            {
                asAutoCoverTrayID1=sCIDOnTrayArm;
                fSortCT->pnlAuto1CID->Caption=asAutoCoverTrayID1;
                asBundleTrayID[ePortAuto1]=asAutoCoverTrayID1;
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Auto1);
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO2)
            {
                asAutoCoverTrayID2=sCIDOnTrayArm;
                fSortCT->pnlAuto2CID->Caption=asAutoCoverTrayID2;
                asBundleTrayID[ePortAuto2]=asAutoCoverTrayID2;
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Auto2);
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO3)
            {
                asAutoCoverTrayID3=sCIDOnTrayArm;
                fSortCT->pnlAuto3CID->Caption=asAutoCoverTrayID3;
                asBundleTrayID[ePortAuto3]=asAutoCoverTrayID3;
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Auto3);
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO4)
            {
                asAutoCoverTrayID4=sCIDOnTrayArm;
                fSortCT->pnlAuto4CID->Caption=asAutoCoverTrayID4;
                asBundleTrayID[ePortAuto4]=asAutoCoverTrayID4;
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Auto4);
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO5)
            {
                asAutoCoverTrayID5=sCIDOnTrayArm;
                fSortCT->pnlAuto5CID->Caption=asAutoCoverTrayID5;
                asBundleTrayID[ePortAuto5]=asAutoCoverTrayID5;
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Auto5);
            }
            else if(iPos==iKeyenceCoverTrayID_AUTO6)
            {
                asAutoCoverTrayID6=sCIDOnTrayArm;
                fSortCT->pnlAuto6CID->Caption=asAutoCoverTrayID6;
                asBundleTrayID[ePortAuto6]=asAutoCoverTrayID6;
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Auto6);
            }
            bResult=true;
            break;
    }

    return bResult;
}
//------------------------------------------------------------------------------
void __fastcall TfTrayMapping::ClientSocket_CoverTray_ID_LoaderCarConnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;

    if(InitialOK==false)
    {
        Ptr->Active=false;
        return;
    }

    AnsiString Str;
    Str.sprintf("IP: %s, Port:%d [Connect OK]", Ptr->Address, Ptr->Port);
    SendKeyenceCommand(Ptr->Tag, Str, "");
}
//------------------------------------------------------------------------------
cDatabaseJson::cDatabaseJson()
{
    sTableName="";
    slSchemaList=new TStringList();
    table_root=cJSON_CreateObject();
}
//---------------------------------------------------------------------------
cDatabaseJson::~cDatabaseJson()
{
    slSchemaList->Clear();
    delete slSchemaList;
}
//---------------------------------------------------------------------------
void cDatabaseJson::AddColumnTitle(AnsiString titleListByDot)
{
    slSchemaList->Delimiter=',';
    slSchemaList->DelimitedText=titleListByDot;
}
//---------------------------------------------------------------------------
bool cDatabaseJson::AddRowData(AnsiString dataListByDot)
{
    bool bReturn=false;
    TStringList* rowList=new TStringList();
    rowList->Delimiter=',';
    rowList->DelimitedText=dataListByDot;
    if(slSchemaList->Count>rowList->Count)
    {
        bReturn=false;
    }
    else
    {
        cJSON* r_single=cJSON_CreateObject();
        AnsiString stitle="";
        AnsiString sdata="";
        for(int i=0;i<rowList->Count;i++)
        {
            stitle=slSchemaList->Strings[i];
            sdata=rowList->Strings[i];
            cJSON_AddStringToObject(r_single,stitle.c_str(),sdata.c_str());
        }
        int iIndex=cJSON_GetArraySize(table_root);
        cJSON_AddItemToObject(table_root, IntToStr(iIndex).c_str(), r_single);
        bReturn=true;
    }
    delete rowList;
    return bReturn;
}
//---------------------------------------------------------------------------
bool cDatabaseJson::UpdateRowData(int iPos,AnsiString dataListByDot)
{
    bool bReturn=false;
    TStringList* rowList=new TStringList();
    rowList->Delimiter=',';
    rowList->DelimitedText=dataListByDot;
    if(slSchemaList->Count>rowList->Count)
    {
        bReturn=false;
    }
    else
    {
        int iIndex=cJSON_GetArraySize(table_root);
        cJSON* r_single=cJSON_CreateObject();
        AnsiString stitle="";
        AnsiString sdata="";
        for(int i=0;i<rowList->Count;i++)
        {
            stitle=slSchemaList->Strings[i];
            sdata=rowList->Strings[i];
            cJSON_AddStringToObject(r_single,stitle.c_str(),sdata.c_str());
        }
        if(iIndex>iPos)
        {
            cJSON_ReplaceItemInArray(table_root,iPos,r_single);
        }
        else
        {
            cJSON_AddItemToObject(table_root,"TotalNum",r_single);
        }
        bReturn=true;
    }
    delete rowList;
    return bReturn;
}
//---------------------------------------------------------------------------
void cDatabaseJson::SaveFile(AnsiString FileName)
{
    WriteDataToFile(FileName.c_str(), cJSON_Print(table_root), true);
//    cJSON_Delete(table_root);
}
//---------------------------------------------------------------------------
bool cDatabaseJson::LoadFile(AnsiString FileName)
{
    char* file_buf=ReadDataFromFile(FileName);
    cJSON* load_root=cJSON_Parse(file_buf);
    if(load_root)
    {
        table_root=load_root;
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
cLineScanRemainICYieldRecord::cLineScanRemainICYieldRecord()
{
    sTemp="";
    InitialParameter();
}
//---------------------------------------------------------------------------
cLineScanRemainICYieldRecord::~cLineScanRemainICYieldRecord()
{
//
}
//---------------------------------------------------------------------------
AnsiString cLineScanRemainICYieldRecord::GetNowYearMonth()
{
    return AnsiString().sprintf("%s", Now().FormatString("yyyymm"));
}
//---------------------------------------------------------------------------
AnsiString cLineScanRemainICYieldRecord::GetFileName()
{
    AnsiString sTemp=AnsiString().sprintf("%s.json", Now().FormatString("yyyymmdd"));
    return sTemp;
}
//---------------------------------------------------------------------------
AnsiString cLineScanRemainICYieldRecord::GetFileNameNoExt()
{
    sTemp=fProductionInfo->_sOEE_DirectoryName;
    if(sTemp=="")
    {
        sTemp=AnsiString().sprintf("%s","Default");
    }
    return sTemp;
}
//---------------------------------------------------------------------------
AnsiString cLineScanRemainICYieldRecord::GetFileNameWithDir()
{
    AnsiString sTemp=GetDirector();
    MyForceDirectories(sTemp);
    //
    sTemp+="\\"+GetFolderName();
    MyForceDirectories(sTemp);
    //
    sTemp+="\\"+GetNowYearMonth();
    MyForceDirectories(sTemp);
    //
    sTemp+="\\"+GetFileName();
    return sTemp;
}
//---------------------------------------------------------------------------
AnsiString cLineScanRemainICYieldRecord::GetTotalNumDir()
{
    AnsiString sTemp=GetDirector();
    MyForceDirectories(sTemp);
    //
    sTemp+="\\"+GetFolderName();
    MyForceDirectories(sTemp);
    //
    sTemp+="\\"+GetTotalNumFileName();
    return sTemp;
}
//---------------------------------------------------------------------------
void cLineScanRemainICYieldRecord::InitialParameter()
{
    AnsiString sTitle="time,mo,result";
    dbJ_History.AddColumnTitle(sTitle);
    sTitle="latestTime,mo,count,sum,yield";
    dbJ_TotalNum.AddColumnTitle(sTitle);
}
//---------------------------------------------------------------------------
AnsiString cLineScanRemainICYieldRecord::GetYield()
{
    AnsiString sBack="0";
    dbJ_TotalNum.LoadFile(GetTotalNumDir());
    cJSON* r_single=cJSON_GetArrayItem(dbJ_TotalNum.table_root, 0);
    sBack=dbJ_TotalNum.GetStringFromJSON(r_single, "yield");
    return sBack;
}
//---------------------------------------------------------------------------
void cLineScanRemainICYieldRecord::AddYieldData(bool bYield)
{
    AnsiString sDatetime=Now().FormatString("yyyymmdd_hhnnss");
    AnsiString sResult=(bYield==true)?"1":"0";
    //now record
    RefreshTotalNum(sDatetime,bYield);
    //history record
    dbJ_History.LoadFile(GetFileNameWithDir());
    dbJ_History.AddRowData(AnsiString().sprintf("%s,%s,%s",
                            sDatetime,
                            GetFileNameNoExt(),
                            "1"
                            ));
    dbJ_History.SaveFile(GetFileNameWithDir());
}
//---------------------------------------------------------------------------
void cLineScanRemainICYieldRecord::RefreshTotalNum(AnsiString sDatetime,bool bYield)
{
    dbJ_TotalNum.LoadFile(GetTotalNumDir());
    int iSum=(bYield==true)?1:0;
    int iCount=0;
    AnsiString sTemp="";
    cJSON* r_single=cJSON_GetArrayItem(dbJ_TotalNum.table_root, 0);
    //sum
    sTemp=dbJ_TotalNum.GetStringFromJSON(r_single, "sum");
    iSum+=atoi(sTemp.c_str());
    //count
    sTemp=dbJ_TotalNum.GetStringFromJSON(r_single, "count");
    iCount=atoi(sTemp.c_str())+1;
    //yield
    double dAVG=ChangeToFloatNonPcnt((double)((double)iSum), (double)(iCount));
    dAVG*=100;
    AnsiString sYield=FormatFloat("#0.00", dAVG);
    //
    dbJ_TotalNum.UpdateRowData(0,AnsiString().sprintf("%s,%s,%d,%d,%s",
                            sDatetime,
                            GetFileNameNoExt(),
                            iCount,
                            iSum,
                            sYield
                            ));
    //
    dbJ_TotalNum.SaveFile(GetTotalNumDir());
}
//---------------------------------------------------------------------------
TfRFID::TfRFID(TComm *commRFID, TMemo *memoLdRFID)                              //RogerYang 20250830 : Add RFID
{
//    bRecv=true;
    COMRFID=commRFID;
    MemoStrLog=memoLdRFID;
//    bAutoRead=false;
    iAutoReadTimeout=0;
    bCommConnect=false;

    bClearCmd1  =false;
    bReadUID1   =false;
    bReadBlock1 =false;
    bClearCmd2  =false;
    bReadUID2   =false;
    bReadBlock2 =false;

    bShowRequestLog=false;
    sUID1="";
    sBlockID1="";
    sUID2="";
    sBlockID2="";
    for(int i=0; i<eRFIDCmdTotal; i++)
        iNeedTDoFlag[i]=0;
}
//---------------------------------------------------------------------------
TfRFID::~TfRFID()                                                               //RogerYang 20250830 : Add RFID
{
    CloseCommPort();
    COMRFID = NULL;
    MemoStrLog = NULL;
}
//---------------------------------------------------------------------------
bool TfRFID::OpenCommPort()
{
    if(bCommConnect==true)
    {
        CloseCommPort();
    }

    MySleepEx(10, false);

    try
    {
        HANDLE h=CreateFile(COMRFID->CommName.c_str(),
                                 GENERIC_READ|GENERIC_WRITE,
                                 0,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
                                 0);

        if(h==INVALID_HANDLE_VALUE)
        {
            ShowCommData("[Connect]", " INVALID_HANDLE_VALUE");
            bCommConnect=false;
            MessageBox(0, "Comm Port Error!! Please Chose Other Comm Port. ", "Alarm", MB_OK);
            return false;
        }

        CloseHandle(h);
        COMRFID->StartComm();
        bCommConnect=true;
        ShowCommData("[Connect]", " OK");
    }
    catch(...)
    {
        ShowCommData("[Connect]", " FAIL");
        bCommConnect=false;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool TfRFID::CloseCommPort()
{
    try
    {
        COMRFID->StopComm();
        bCommConnect=false;
        mSearchStage = 0;
//        bRecv = true;
        ShowCommData("[Disconnect]", " OK");
    }
    catch(...)
    {
        ShowCommData("[Disconnect]", " FAIL");
//        bRecv=true;
        return false;
    }
//    bRecv=true;
    return true;
}
//---------------------------------------------------------------------------
unsigned int Crc_16_create(unsigned char *string, unsigned char length)
{
    unsigned char Bitloop;
    unsigned int Crc_tmp=0xffff;

    while(length--)
    {
        Crc_tmp^=*string++;
        for(Bitloop=0; Bitloop<8; Bitloop++)
        {
            if(Crc_tmp&0x01)
            {
                Crc_tmp=(Crc_tmp>>1)^0xa001;
            }
            else
            {
                Crc_tmp=Crc_tmp>>1;
            }
        }
    }
    return(Crc_tmp);
}
//---------------------------------------------------------------------------
AnsiString BlockToString(const BYTE* data, int length)                          //RogerYang 20251209 Add
{
    AnsiString result = "";
    for(int i=0; i<length; i++)
    {
        if(data[i]==0x00) break;
        result+=(char)data[i];
    }
    return result;
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCReadHardwareVersion(int iCh)
{
    BYTE request[8] = {iCh, 0x03, 0x00, 0x0F, 0x00, 0x0E};
    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        iFunction=eNFCReadHardwareVersion1;
        iNeedTDoFlag[eNFCReadHardwareVersion1]=2;
        ShowCommData("[NFC]", "Request CH1 Hardware Version");
    }
    else
    {
        iFunction=eNFCReadHardwareVersion2;
        iNeedTDoFlag[eNFCReadHardwareVersion2]=2;
        ShowCommData("[NFC]", "Request CH2 Hardware Version");
    }

    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCReadFirmwareVersion(int iCh)
{
    BYTE request[8] = {iCh, 0x03, 0x00, 0x0E, 0x00, 0x01};
    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        ShowCommData("[NFC]", "Request CH1 Firmware Version");
        iFunction=eNFCReadFirmwareVersion1;
        iNeedTDoFlag[eNFCReadFirmwareVersion1]=2;
    }
    else
    {
        ShowCommData("[NFC]", "Request CH2 Firmware Version");
        iFunction=eNFCReadFirmwareVersion2;
        iNeedTDoFlag[eNFCReadFirmwareVersion2]=2;

    }

    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCReaderGetID(int iCh)
{
    BYTE request[8] = {iCh, 0x03, 0x00, 0x0D, 0x00, 0x01};
    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        ShowCommData("[NFC]", "Request Get CH1 ID");
        iFunction=eNFCReaderGetID1;
        iNeedTDoFlag[eNFCReaderGetID1]=2;
    }
    else
    {
        ShowCommData("[NFC]", "Request Get CH2 ID");
        iFunction=eNFCReaderGetID2;
        iNeedTDoFlag[eNFCReaderGetID2]=2;
    }

    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCClearFlag(int iCh)
{
    BYTE request[8] = {iCh, 0x06, 0x00, 0x01, 0x00, 0x00};
    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        ShowCommData("[NFC]", "Request Clear CH1 Flag");
        iFunction=eNFCClearFlag1;
        iNeedTDoFlag[eNFCClearFlag1]=2;
        bClearCmd1=false;
    }
    else
    {
        ShowCommData("[NFC]", "Request Clear CH2 Flag");
        iFunction=eNFCClearFlag2;
        iNeedTDoFlag[eNFCClearFlag2]=2;
        bClearCmd2=false;
    }

    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCReadUID(int iCh)
{
    BYTE request[8] = {iCh, 0x03, 0x00, 0x03, 0x00, 0x0A};
    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        ShowCommData("[NFC]", "Request UID 1");
        iFunction=eNFCReadUID1;
        iNeedTDoFlag[eNFCReadUID1]=2;
        sUID1="";
        bReadUID1=false;
    }
    else
    {
        ShowCommData("[NFC]", "Request UID 2");
        iFunction=eNFCReadUID2;
        iNeedTDoFlag[eNFCReadUID2]=2;
        sUID2="";
        bReadUID2=false;
    }
    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCReadMemData(int iCh)
{
    BYTE request[8] = {iCh, 0x06, 0x00, 0x22, 0x00, 0x04};
    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        iFunction=eNFCReadMemData1;
        iNeedTDoFlag[eNFCReadMemData1]=2;
        ShowCommData("[NFC]", "Request CH1 Mem Data");
    }
    else
    {
        iFunction=eNFCReadMemData2;
        iNeedTDoFlag[eNFCReadMemData2]=2;
        ShowCommData("[NFC]", "Request CH2 Mem Data");
    }

    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCReadBlock(int iCh)
{
    BYTE request[8];
    request[0] = iCh;
    request[1] = 0x03;
    request[2] = 0x10;
    request[3] = 0x00;
    request[4] = 0x00;
    request[5] = 0x08;

    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        sBlockID1="";
        bReadBlock1=false;
        iFunction=eNFCReadBlock1;
        iNeedTDoFlag[eNFCReadBlock1]=2;
        ShowCommData("[NFC]", "Request CH1 ReadBlock");
    }
    else
    {
        sBlockID2="";
        bReadBlock2=false;
        iFunction=eNFCReadBlock2;
        iNeedTDoFlag[eNFCReadBlock2]=2;
        ShowCommData("[NFC]", "Request CH2 ReadBlock");
    }

    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCReadSearchBlock(int startBlock, int blockCount)
{
//    startBlock  = StrToIntDef(EditStartBlock->Text, 0);
//    blockCount  = StrToIntDef(EditBlockCount->Text, 1);
//
//    BYTE cmd1[8];
//    cmd1[0] = 0x01;
//    cmd1[1] = 0x06;
//    cmd1[2] = 0x10;
//    cmd1[3] = 0x08;
//    cmd1[4] = (BYTE)startBlock;
//    cmd1[5] = (BYTE)blockCount;
//    unsigned int crc1 = Crc_16_create(cmd1, 6);
//    cmd1[6] = crc1 & 0xFF;
//    cmd1[7] = (crc1 >> 8) & 0xFF;
//
//    COMRFID->WriteCommData(cmd1, 8);
//
//    gStartBlock = startBlock;
//    gBlockCount = blockCount;
//    iFunction   = eNFCReadSearchBlock;
//    bRecv = false;
//    ShowCommData("[NFC]", "Request Search Block");
//    mSearchStage = 1;
}
//---------------------------------------------------------------------------
void TfRFID::SendNFCRequestToSlaveNumberState(int iCh)
{
    BYTE request[8] = {iCh, 0x03, 0x00, 0x03, 0x00, 0x02};
    unsigned int crc = Crc_16_create(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;

    if(iCh==1)
    {
        iFunction=eNFCReqToSlaveNoState1;
        iNeedTDoFlag[eNFCReqToSlaveNoState1]=2;
        ShowCommData("[NFC]", "Request CH1 Slave Number State");
    }
    else
    {
        iFunction=eNFCReqToSlaveNoState2;
        iNeedTDoFlag[eNFCReqToSlaveNoState1]=2;
        ShowCommData("[NFC]", "Request CH2 Slave Number State");
    }

    COMRFID->WriteCommData(request, 8);
}
//---------------------------------------------------------------------------W
void TfRFID::COMRFIDReceiveData(TObject *Sender, Pointer Buffer, WORD BufferLength)
{
    BYTE* data = (BYTE*)Buffer;

    if(BufferLength<5)
    {
        MemoStrLog->Lines->Add("DBG: too short frame");
        mSearchStage=0;
        return;
    }

    unsigned int receivedCRC=data[BufferLength-2]|(data[BufferLength-1]<<8);
    unsigned int calcCRC=Crc_16_create(data, BufferLength-2);

    /*if((iFunction==eNFCReadSearchBlock1 ||
        iFunction==eNFCReadSearchBlock2) &&
       mSearchStage==1 &&
       data[1]==0x06)
    {
        if(receivedCRC!=calcCRC)
        {
            MemoStrLog->Lines->Add("CRC Error!");
            mSearchStage = 0;
            return;
        }

        BYTE cmd2[8];
        cmd2[0]=data[0];
        cmd2[1]=0x03;
        cmd2[2]=0x10;
        cmd2[3]=0x09;
        cmd2[4]=0x00;
        cmd2[5]=(BYTE)gBlockCount*2;
        unsigned int crc2=Crc_16_create(cmd2, 6);
        cmd2[6]=crc2&0xFF;
        cmd2[7]=(crc2 >> 8)&0xFF;
        iNeedTDoFlag[iFunction]=0;

        COMRFID->WriteCommData(cmd2, 8);
        bRecv=true;
        mSearchStage=2;
        return;
    }*/

    if(receivedCRC!=calcCRC)
    {
        MemoStrLog->Lines->Add("CRC Error!");
        return;
    }

    if(!(data[0]==0x01 || data[0]==0x02) &&
       data[1]!=0x03)
    {
        MemoStrLog->Lines->Add("Invalid SlaveID or Function Code!");
        return;
    }

    if(iFunction==eNFCClearFlag1)
    {
        sUID1="";                                                               //RogerYang Add
        sBlockID1="";
        bClearCmd1=true;
        iNeedTDoFlag[iFunction]=0;
        bRecv=true;
        MemoStrLog->Lines->Add("Clear Ch1 Flag OK!");
        return;
    }
    else if(iFunction==eNFCClearFlag2)
    {
        sUID2="";
        sBlockID2="";
        bClearCmd2=true;
        iNeedTDoFlag[iFunction]=0;
        bRecv=true;
        MemoStrLog->Lines->Add("Clear Ch2 Flag OK!");
        return;
    }

    BYTE byteCount=data[2];
    if(byteCount+5!=BufferLength)
    {
        return;
    }

    AnsiString cardID;
    WORD wChar;

    if(iFunction==eNFCReadHardwareVersion1 ||
       iFunction==eNFCReadHardwareVersion2)
    {
        for(int i=0; i<byteCount; i+=2)
        {
            wChar=(data[3+i]<<8)|data[3+i+1];
            cardID+=(char)wChar;
        }
    }
    else if(iFunction==eNFCClearFlag1 ||
            iFunction==eNFCClearFlag2)
    {

    }
    else //if(iFunction==eNFCReadFirmwareVersion ||
//            iFunction==eNFCReaderGetID         ||
//            iFunction==eNFCReadUID             ||
//            iFunction==eNFCReadMemData         ||
//            iFunction==eNFCReadBlock           ||
//            iFunction==eNFCReqToSlaveNoState   ||
//            iFunction==eNFCReadSearchBlock    )
    {
        for(int i=0; i<byteCount; i++)
        {
            cardID+=AnsiString().sprintf("%02X", data[3+i]);
        }
    }

    if(iFunction==eNFCReadHardwareVersion1)
    {
        MemoStrLog->Lines->Add("CH1 Hardware Version: "+cardID);
    }
    else if(iFunction==eNFCReadHardwareVersion2)
    {
        MemoStrLog->Lines->Add("CH2 Hardware Version: "+cardID);
    }
    else if(iFunction==eNFCReadFirmwareVersion1)
    {
        MemoStrLog->Lines->Add("CH1 Firmware Version: "+cardID);
    }
    else if(iFunction==eNFCReadFirmwareVersion2)
    {
        MemoStrLog->Lines->Add("CH2 Firmware Version: "+cardID);
    }
    else if(iFunction==eNFCReaderGetID1)
    {
        fTrayMapping->lblNFC_CH1_CID->Caption=cardID;
        MemoStrLog->Lines->Add("CH1 Reader ID: "+cardID);
    }
    else if(iFunction==eNFCReaderGetID2)
    {
        fTrayMapping->lblNFC_CH2_CID->Caption=cardID;
        MemoStrLog->Lines->Add("CH2 Reader ID: "+cardID);
    }
    else if(iFunction==eNFCReadUID1 ||
            iFunction==eNFCReadUID2)
    {
        AnsiString shortUID=cardID.SubString(1, 16);                            //RogerYang 20251209 : 變數先改回來
        if(shortUID!="0000000000000000")
        {
            if(iFunction==eNFCReadUID1)
            {
                fTrayMapping->lblNFC_CH1_UID->Caption=cardID;
                MemoStrLog->Lines->Add("UID 1: "+shortUID);
                sUID1=shortUID;
                bReadUID1=true;
            }
            else
            {
                fTrayMapping->lblNFC_CH2_UID->Caption=cardID;
                MemoStrLog->Lines->Add("UID 2: "+shortUID);
                sUID2=shortUID;
                bReadUID2=true;
            }
        }
    }
    else if(iFunction==eNFCReadMemData1)
    {
        MemoStrLog->Lines->Add("CH1 Mem Data: " + cardID);
    }
    else if(iFunction==eNFCReadMemData2)
    {
        MemoStrLog->Lines->Add("CH2 Mem Data: " + cardID);
    }
    else if(iFunction==eNFCReadBlock1)
    {
        MemoStrLog->Lines->Add("CH1 Block[00]-[03]: " + cardID);
        sBlockID1 = BlockToString(&data[3], byteCount);                         //RogerYang 20251209 : add
        if(sBlockID1!="")
        {
            fTrayMapping->lblNFC_CH1_CID->Caption=sBlockID1;
            MemoStrLog->Lines->Add("CH1 Name: " + sBlockID1);
        }

        bReadBlock1=true;
    }
    else if(iFunction==eNFCReadBlock2)
    {
        MemoStrLog->Lines->Add("CH2 Block[00]-[03]: " + cardID);
        sBlockID2 = BlockToString(&data[3], byteCount);                         //RogerYang 20251209 : add
        if(sBlockID2!="")
        {
            fTrayMapping->lblNFC_CH2_CID->Caption=sBlockID2;
            MemoStrLog->Lines->Add("CH2 Name: " + sBlockID2);
        }

        bReadBlock2=true;
    }
    else if(iFunction==eNFCReqToSlaveNoState1)
    {
        MemoStrLog->Lines->Add("CH1 Slave Number State: " + cardID);
    }
    else if(iFunction==eNFCReqToSlaveNoState2)
    {
        MemoStrLog->Lines->Add("CH2 Slave Number State: " + cardID);
    }
    /*else if(iFunction==eNFCReadSearchBlock1 ||
            iFunction==eNFCReadSearchBlock2)
    {
        if(data[1]!=0x03)
            return;

        BYTE byteCount=data[2];
        if(byteCount+5!=BufferLength)
            return;

        if(byteCount!=(BYTE)(gBlockCount * 4))
            return;

        AnsiString spacedHex;
        for (int i=0;i<byteCount;++i)
        {
            spacedHex+=IntToHex(data[3+i], 2);
            if (i!=byteCount-1) spacedHex+=" ";
        }

        if(iFunction==eNFCReadSearchBlock1)
        {
            MemoStrLog->Lines->Add(AnsiString().sprintf("CH1 Block[%d]-[%d]: %s", gStartBlock, gBlockCount, spacedHex.c_str()));
            MemoStrLog->Lines->Add("[NFC] CH1 Search Block Done");
        }
        else
        {
            MemoStrLog->Lines->Add(AnsiString().sprintf("CH2 Block[%d]-[%d]: %s", gStartBlock, gBlockCount, spacedHex.c_str()));
            MemoStrLog->Lines->Add("[NFC] CH2 Search Block Done");
        }

        mSearchStage=0;
    }*/

    if(iFunction>=0 && iFunction<eRFIDCmdTotal)
    {
        iNeedTDoFlag[iFunction]=0;
        bRecv=true;
    }
}
//---------------------------------------------------------------------------
void TfRFID::ShowCommData(AnsiString sType, AnsiString sData)
{
    AnsiString sLog=sType+sData;

    if(MemoStrLog->Lines->Count>300)
    {
        MemoStrLog->Clear();
    }

    MemoStrLog->Lines->Append(sLog);

    AnsiString sHexLog=sType+"";
}
//---------------------------------------------------------------------------
//void TfRFID::UIDReadTimer()
//{
//    // 自動讀取模式，且上一筆已回覆
//    if(bAutoRead && bRecv)
//    {
//        SendNFCReadUID(1);    // 發送讀 UID 指令
//        iAutoReadTimeout++;  // 計數超時次數
//
//        // 超過 40 次 (~12 秒) 沒讀到卡片就自動結束
//        if(iAutoReadTimeout>40)
//        {
//            bAutoRead=false;
//            //tmrUIDRead->Enabled=false;
//            MemoStrLog->Lines->Add("[AutoRead] Timeout，自動讀取結束");
//        }
//    }
//}
//---------------------------------------------------------------------------
void TfRFID::SetClearFlag(int iCh)
{
//    if(!bCommConnect)
//    {
//        ShowCommData("[Error]", "COM port not connected!");
//        return;
//    }

//    if(bRecv==true)
        SendNFCClearFlag(iCh);
}
//---------------------------------------------------------------------------
//void TfRFID::ReadUID()
//{
//    if(!bCommConnect)
//    {
//        ShowCommData("[Error]", "COM port not connected!");
//        return;
//    }
//
//    if(bRecv==true)
//        SendNFCReadUID();
//}
//---------------------------------------------------------------------------
bool TfRFID::GetRecvStat()
{
    return bRecv;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnLdRFIDConnClick(TObject *Sender)              //KevinCheng 20250830 : Add RFID
{
    commRFID->StopComm();
    commRFID->CommName="\\\\.\\"+cbbLdRFIDComPort->Text;

    //commRFID->BaudRate=115200;
    if(ldRFID->OpenCommPort()==false)
    {
        pnlLdRFIDState->Color=clRed;
        pnlLdRFIDState->Caption="Disconnect";
    }
    else
    {
        pnlLdRFIDState->Color=clGreen;
        pnlLdRFIDState->Caption="Connect";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::commRFIDReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    if(ldRFID)
        ldRFID->COMRFIDReceiveData(Sender, Buffer, BufferLength);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnLdRFIDDisConnClick(TObject *Sender)
{
    if(ldRFID->CloseCommPort()==true)
    {
        pnlLdRFIDState->Color=clRed;
        pnlLdRFIDState->Caption="Disconnect";
    }
    else
    {
        pnlLdRFIDState->Color=clGreen;
        pnlLdRFIDState->Caption="Connect";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnLdRFIDClear1Click(TObject *Sender)
{
    ldRFID->iNeedTDoFlag[ldRFID->eNFCClearFlag1]=1;
    lblNFC_CH1_CID->Caption="";
    lblNFC_CH1_UID->Caption="";
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnReadID1Click(TObject *Sender)
{
    lblNFC_CH1_CID->Caption="";
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReaderGetID1]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnReadUID1Click(TObject *Sender)
{
    lblNFC_CH1_UID->Caption="";
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReadUID1]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnReadMemData1Click(TObject *Sender)
{
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReadMemData1]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnLdRFIDReadBlock1Click(TObject *Sender)
{
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReadBlock1]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnLdRFIDClear2Click(TObject *Sender)
{
    ldRFID->iNeedTDoFlag[ldRFID->eNFCClearFlag2]=1;
    lblNFC_CH2_CID->Caption="";
    lblNFC_CH2_UID->Caption="";
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnReadUID2Click(TObject *Sender)
{
    lblNFC_CH2_UID->Caption="";
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReadUID2]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnLdRFIDReadBlock2Click(TObject *Sender)
{
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReadBlock2]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnReadMemData2Click(TObject *Sender)
{
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReadMemData2]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::btnReadID2Click(TObject *Sender)
{
    lblNFC_CH1_CID->Caption="";
    ldRFID->iNeedTDoFlag[ldRFID->eNFCReaderGetID2]=1;
}
//---------------------------------------------------------------------------
void __fastcall TfTrayMapping::tmrNFCTimer(TObject *Sender)
{
    static bool bRunTimer=false;
    if(bRunTimer)
        return;

    bRunTimer=true;

    switch(iNFCTask)
    {
        case 1:
            if(fAGV->IsATK_AMR() &&
               fTrayMapping->ldRFID->bCommConnect==false)
            {
                btnLdRFIDConn->Click();
            }
            iNFCTask=100;
            break;
        case 100:
            ldRFID->bRecv=false;
            tNFTDelay.SetSecAndOn(1);
            if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadHardwareVersion1]==1)
            {
                ldRFID->SendNFCReadHardwareVersion(1);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadFirmwareVersion1]==1)
            {
                ldRFID->SendNFCReadFirmwareVersion(1);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReaderGetID1]==1)
            {
                ldRFID->SendNFCReaderGetID(1);                                  //RogerYang 20251209 : add
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadUID1]==1)
            {
                ldRFID->SendNFCReadUID(1);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadMemData1]==1)
            {
                ldRFID->SendNFCReadMemData(1);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadBlock1]==1)
            {
                ldRFID->SendNFCReadBlock(1);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReqToSlaveNoState1]==1)
            {
                ldRFID->SendNFCRequestToSlaveNumberState(1);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadSearchBlock1]==1)
            {
                ldRFID->iNeedTDoFlag[ldRFID->eNFCReadSearchBlock1]=0;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCClearFlag1]==1)
            {
                ldRFID->SetClearFlag(1);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadHardwareVersion2]==1)
            {
                ldRFID->SendNFCReadHardwareVersion(2);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadFirmwareVersion2]==1)
            {
                ldRFID->SendNFCReadFirmwareVersion(2);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReaderGetID2]==1)
            {
                ldRFID->SendNFCReaderGetID(2);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadUID2]==1)
            {
                ldRFID->SendNFCReadUID(2);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadMemData2]==1)
            {
                ldRFID->SendNFCReadMemData(2);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadBlock2]==1)
            {
                ldRFID->SendNFCReadBlock(2);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReqToSlaveNoState2]==1)
            {
                ldRFID->SendNFCRequestToSlaveNumberState(2);
                iNFCTask=1000;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCReadSearchBlock2]==1)
            {
                ldRFID->iNeedTDoFlag[ldRFID->eNFCReadSearchBlock2]=0;
            }
            else if(ldRFID->iNeedTDoFlag[ldRFID->eNFCClearFlag2]==1)
            {
                ldRFID->SetClearFlag(2);
                iNFCTask=1000;
            }
            break;
        case 1000:
            if(ldRFID->bRecv)
            {
                iNFCTask=100;
            }
            else if(tNFTDelay.Off())
            {
                for(int i=0; i<ldRFID->eRFIDCmdTotal; i++)
                {
                    if(ldRFID->iNeedTDoFlag[i]==2)
                        ldRFID->iNeedTDoFlag[i]=1;
                }
                iNFCTask=1;
            }
            break;
    }

    bRunTimer=false;
}
//---------------------------------------------------------------------------
