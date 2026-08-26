#include "MachineDefine.h"
#pragma hdrstop

#include "HandlerSys.h"
#include "cmydef.h"
#include "main.h"
#include "mysensor.h"
#include "common.h"
//#include "Fingerprint.h"
#include "OCR.h"
#include "myQwertyKeyBoard.h"
#include "database.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THandlerSystem *HandlerSystem;

#if EN_HEATER_SHEET                                                             //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定
#include <map>
#define WIDTH_OF_LB_HEATER              100
#define HEIGHT_OF_LB_HEATER              28
#define WIDTH_OF_CB_HEATER              170
#define HEIGHT_OF_CB_HEATER              28
#define TOP_OF_LB                        12
#define TOP_OF_CB                        12
#define LEFT_OF_1ST                      12
#define GAP_OF_HOR_IN_SET                10
#define GAP_OF_HOR_OUT_SET               30
#define GAP_OF_VER                       10
#define COUNT_OF_ITEM_IN_COL             10

std::vector<int>   g_vecHeaterTypeIdxForShow;
std::map<int, int> g_mapHeaterTypeIdxToHeaterShowIdx;
std::map<int, int> g_mapHeaterShowIdxToHeaterTypeIdx;

static bool s_bSuppressHeaterTypeEvent = false;                                 //AI(ht9045-heater-control) 20260619 (RogerYang) : 載入期程式設定rgHeaterType時，禁止事件覆寫逐站值

int TypeIdxToShowIdx(int iTypeIdx)
{
    std::map<int, int>::iterator it = g_mapHeaterTypeIdxToHeaterShowIdx.find(iTypeIdx);
    if(it == g_mapHeaterTypeIdxToHeaterShowIdx.end()) return INVALID_INT_VAL_NEG;
    return it->second;
}
int ShowIdxToTypeIdx(int iShowIdx)
{
    std::map<int, int>::iterator it = g_mapHeaterShowIdxToHeaterTypeIdx.find(iShowIdx);
    if(it == g_mapHeaterShowIdxToHeaterTypeIdx.end()) return INVALID_INT_VAL_NEG;
    return it->second;
}
void HeaterInsOpt_Read()
{
    int iHeaterInsOpt_Old = CheckAndReadIniDataGeneral("TempCtrl","HEATER_CTRL_TYPE",KT4H);
    for(int iTypeIdx=0; iTypeIdx<eHeaterType_Count; ++iTypeIdx)
    {
        g_tHeaterInsInfo[iTypeIdx].m_iHeaterInsOpt =
            CheckAndReadIniDataGeneral("TempCtrl", g_tHeaterInsInfo[iTypeIdx].m_asSaveName, INVALID_INT_VAL_NEG);
        if(INVALID_INT_VAL_NEG==g_tHeaterInsInfo[iTypeIdx].GetHeaterInsOpt())
            g_tHeaterInsInfo[iTypeIdx].m_iHeaterInsOpt = iHeaterInsOpt_Old;     //回退 HEATER_CTRL_TYPE(=rgHeaterType 設定值)，非固定 KT4H；其他機型進版沿用原廠牌
    }
}
#endif
//---------------------------------------------------------------------------
__fastcall THandlerSystem::THandlerSystem(TComponent* Owner)
    : TForm(Owner)
{
    slCustomerCode=new TStringList();
    for(int i=0; i<rgCustomerList->Items->Count; i++)
        slCustomerCode->Add(rgCustomerList->Items->Strings[i]);

#if EN_HEATER_SHEET
    g_vecHeaterTypeIdxForShow.clear();
    g_mapHeaterTypeIdxToHeaterShowIdx.clear();
    g_mapHeaterShowIdxToHeaterTypeIdx.clear();
    {
        int iShowCount = 0;
        for(int i = 0; i<eHeaterType_Count; ++i)
        {
            if(g_tHeaterInsInfo[i].GetOccupy())
            {
                g_mapHeaterTypeIdxToHeaterShowIdx[i] = iShowCount;
                g_mapHeaterShowIdxToHeaterTypeIdx[iShowCount] = i;
                g_vecHeaterTypeIdxForShow.push_back(i);
                iShowCount++;
            }
            else
                g_mapHeaterTypeIdxToHeaterShowIdx[i] = INVALID_INT_VAL_NEG;
        }
        for(int i = 0; i<(int)g_vecHeaterTypeIdxForShow.size(); ++i)
        {
            int iTypeIdx = g_vecHeaterTypeIdxForShow[i];
            int iHorIdx  = i / COUNT_OF_ITEM_IN_COL;
            int iVerIdx  = i % COUNT_OF_ITEM_IN_COL;
            TLabel    *pLb = new TLabel(this);
            TComboBox *pCb = new TComboBox(this);
            pLb->Parent = grpHeater;
            pCb->Parent = grpHeater;
            pLb->Width = WIDTH_OF_LB_HEATER; pLb->Height = HEIGHT_OF_LB_HEATER;
            pCb->Width = WIDTH_OF_CB_HEATER; pCb->Height = HEIGHT_OF_CB_HEATER;
            pLb->Top  = TOP_OF_LB + iVerIdx*(HEIGHT_OF_LB_HEATER + GAP_OF_VER);
            pCb->Top  = TOP_OF_CB + iVerIdx*(HEIGHT_OF_CB_HEATER + GAP_OF_VER);
            pLb->Left = LEFT_OF_1ST + iHorIdx*(WIDTH_OF_LB_HEATER + GAP_OF_HOR_IN_SET + WIDTH_OF_CB_HEATER + GAP_OF_HOR_OUT_SET);
            pCb->Left = pLb->Left + (WIDTH_OF_LB_HEATER + GAP_OF_HOR_IN_SET);
            pLb->Caption = g_tHeaterInsInfo[iTypeIdx].GetShowName();
            for(int j = 0; j<eHeaterInsOpt_Count; ++j)
                pCb->Items->Add(g_HeaterInsOptStr[j]);
            pCb->ItemIndex = g_tHeaterInsInfo[iTypeIdx].GetHeaterInsOpt();   //fix: ref used i, must be iTypeIdx
            pCb->OnClick = cbHeaterInsOptChange_Base;
            pCb->Tag = i;
            g_tHeaterInsInfo[iTypeIdx].SetCtrlItemProp(GetCtrlItemVisProp(iTypeIdx), pLb, pCb);
        }
    }
#endif
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::FormDestroy(TObject *Sender)
{
    slCustomerCode->Clear();
    delete slCustomerCode;
    TempComp.clear();
    LogSoftwareOffTime("THandlerSystem, FormDestroy");                          //Steven 20210526 : 紀錄軟體執行時間
}

#if EN_HEATER_SHEET
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::cbHeaterInsOptChange_Base(TObject *Sender)      //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定
{
    TComboBox* pCb = dynamic_cast<TComboBox*>(Sender);
    if(NULL==pCb) return;
}
#endif
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::FormShow(TObject *Sender)
{
    SortItemToMap();
    LoaderSystemSet();
    LoaderSafeDoorSet();
//    if(CUSTOMER_CODE!=0)                                                      //Steven 20131001 : Lee要求不准有關安全門的功能
//    {
        GroupBox1->Visible=false;
        GroupBox2->Visible=false;
//    }
//    else
//    {
//        GroupBox1->Visible=true;
//        GroupBox2->Visible=true;
//    }

    rgRotateKitIn->Enabled  =(rgRotateKit_Type->ItemIndex == 0);
    rgRotateKitOut->Enabled =(rgRotateKit_Type->ItemIndex == 0);
    if(CosFunction.bUserDefineIndexZSafePos==true)                              //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
        edtUserDefineIndexZSafePos->Enabled=true;
    else
        edtUserDefineIndexZSafePos->Enabled=false;
    pcSetting->ActivePageIndex=0;
    tsCustomerCode->TabVisible=false;

    AnsiString Str="D:\\GPIB9045\\system\\general.ini";                         //Steven 20140708 : 左左右加入機種的選擇
    AnsiString Str2=CheckAndReadIniData(Str, "Version", "Model", AnsiString("HT-9045W"));

    if(Str2=="9045GPIB")             cbHandlerModel->ItemIndex=0;
    else if(Str2=="9046GPIB")        cbHandlerModel->ItemIndex=1;
    else if(Str2=="9045GPIB_12Site") cbHandlerModel->ItemIndex=2;
    else if(Str2=="9046_32GPIB")     cbHandlerModel->ItemIndex=3;
    else if(Str2=="502GPIB")         cbHandlerModel->ItemIndex=4;
    else if(Str2=="1032GPIB")        cbHandlerModel->ItemIndex=5;
    else if(Str2=="7080GPIB")        cbHandlerModel->ItemIndex=6;

    if(cbHandlerModel->ItemIndex==0)                                            //Steven 20190307 : Add for HT-9046LA
        rgModel->ItemIndex=Type_None;
    else
        rgModel->ItemIndex=CheckAndReadIniDataGeneral("Version", "SubModel", Type_None);

    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::LoaderSystemSet()
{
//    SUPPORT_2_EMPTY_EMPTY  =CheckAndReadIniDataGeneral("System", "SUPPORT_2_EMPTY_EMPTY", 0); //RogerYang 20250823:搬去ReadGeneralIni()  //Steven 20230907 : For HT-9011UC

    MachineTrack->ItemIndex=CheckAndReadIniDataGeneral("System", "AUTO_EMPTY_COLOR", MachineTrack->ItemIndex);
    if(SUPPORT_2_EMPTY_EMPTY)
    {
        MachineTrack->ItemIndex=2;                                              //7 Track (Empty Unloader)
    }

    ElectronPressure->ItemIndex=EP_Install;

    chkLoader->Checked=LOAD_Z_USE_MOTOR[0];                                     //Steven 20190813 : 入Tray改用步進馬達
    chkEmpty->Checked =LOAD_Z_USE_MOTOR[1];
    chkColor->Checked =LOAD_Z_USE_MOTOR[2];
    chkAuto1->Checked =LOAD_Z_USE_MOTOR[3];
    chkAuto2->Checked =LOAD_Z_USE_MOTOR[4];
    chkAuto3->Checked =LOAD_Z_USE_MOTOR[5];
    chkAuto4->Checked =LOAD_Z_USE_MOTOR[6];                                     //Steven 20230907 : For HT-9011UC
    chkAuto5->Checked =LOAD_Z_USE_MOTOR[7];
    chkAuto6->Checked =LOAD_Z_USE_MOTOR[8];
    chkLoaderY->Checked=LOAD_Y_USE_MOTOR[0];                                    //Jimmychiu 20240307 : Loader Tray改用步進馬達
    chkEmptyY->Checked =LOAD_Y_USE_MOTOR[1];                                    //AI(ht9045-v899) 20260423: load empty y stepper checkbox
    chkColorY->Checked =LOAD_Y_USE_MOTOR[2];                                    //AI(ht9045-v899) 20260423: load color y stepper checkbox
    chkAuto1Y->Checked =LOAD_Y_USE_MOTOR[3];                                    //AI(ht9045-v899) 20260423: load auto1 y stepper checkbox
    chkAuto2Y->Checked =LOAD_Y_USE_MOTOR[4];                                    //AI(ht9045-v899) 20260423: load auto2 y stepper checkbox
    chkAuto3Y->Checked =LOAD_Y_USE_MOTOR[5];                                    //AI(ht9045-v899) 20260423: load auto3 y stepper checkbox

    cbLoaderCassette->Checked=LOADUNLOAD_USE_CASSETTE[0];
    cbEmptyCassette->Checked =LOADUNLOAD_USE_CASSETTE[1];
    cbColorCassette->Checked =LOADUNLOAD_USE_CASSETTE[2];
    cbAuto1Cassette->Checked =LOADUNLOAD_USE_CASSETTE[3];
    cbAuto2Cassette->Checked =LOADUNLOAD_USE_CASSETTE[4];
    cbAuto3Cassette->Checked =LOADUNLOAD_USE_CASSETTE[5];
    cbAuto4Cassette->Checked =LOADUNLOAD_USE_CASSETTE[6];                       //Steven 20230907 : For HT-9011UC
    cbAuto5Cassette->Checked =LOADUNLOAD_USE_CASSETTE[7];
    cbAuto6Cassette->Checked =LOADUNLOAD_USE_CASSETTE[8];

    ArmZAtoZH->ItemIndex=InOutArmPickerUseMotor;

    rgNumberPanelType->ItemIndex=NUMBER_PANEL_TYPE;
    rgIonFanType->ItemIndex=ION_FAN_TYPE;
    rgShuttleSensor->ItemIndex=SHUTTLE_SENSOR_TYPE;
    rgNUECType->ItemIndex=NUEC_TYPE;                                            //Sam 20230707 : EtherCAT Shuttle sensor

    cbEnableOutShtSensor->Checked=ENABLE_OUT_SHUTTLE_SENEOR;                    //Steven 20120531 : by pass out shuttle sensor
    chkOutShtYSensorByLatch->Checked=ENABLE_OUT_SHUTTLEY_LATCH;                 //Steven 20170518 (jou) : Out shuttle Y sensor 使用 LATCH
    chk2x3modeUseAxisYSensor->Checked=Use_AxisY_Sensor_2x3mode;                 //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
    chkBiasModeUseYSensor->Checked=Bias_Mode_Use_Y_Sensor;                      //Steven 20240805 : 1x2 Bias mode 用Y sensor 偵測Outshuttle殘料
    rgSafeDoor->ItemIndex=SAFE_DOOR_AMOUNT;
    rgWeightCali->ItemIndex=WEIGHT_CALIBRATION;                                 //Steven 20111202 : 要初始化
    edtCustomerCode->Text=CheckAndReadIniDataGeneral("System", "CUSTOMER_CODE", 0);
    edtSeriaNo->Text=CheckAndReadIniDataGeneral("Version", "Serial No", AnsiString("29818"));
    Auto2SelectCy->ItemIndex=bUseAuto2Empty;                                    //kevin 20120718 AUTO2有分離氣缸裝置
    rgInstallAutoRestest->ItemIndex=USE_AUTO_RETEST;                            //ChungHung 20140317 add Auto Retest
    rgAuto1ART->ItemIndex=UNLOADER_ART[eAuto1];                                 //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    rgAuto2ART->ItemIndex=UNLOADER_ART[eAuto2];                                 //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    rgAuto3ART->ItemIndex=UNLOADER_ART[eAuto3];                                 //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    rgAuto4ART->ItemIndex=UNLOADER_ART[eAuto4];                                 //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    rgAuto5ART->ItemIndex=UNLOADER_ART[eAuto5];                                 //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    rgAuto6ART->ItemIndex=UNLOADER_ART[eAuto6];                                 //Steven 20161221 (jou) : for SCK only Auto 2 has ART

//----------------------------------
    rgIndexSuckerType->ItemIndex=CheckAndReadIniDataGeneral("System", "INDEX_SUCKER_TYPE",  0);             //jou 2010-05-19 start : 負壓
    rgIndexPressType->ItemIndex =CheckAndReadIniDataGeneral("System", "INDEX_PRESS_TYPE",   e85KG);         //Steven 20110310 : 240KG
    chkUserDefMaxContactHeight->Checked =CheckAndReadIniDataGeneral("Index", "EnableUserDefMaxContactHeight",   false);                 //Steven 20180412 : For new HT-9045高度較高
    edtUserDefMaxContactHeight->Text    =CheckAndReadIniDataGeneral("Index", "UserDefMaxContactHeight",   AnsiString("-135.0"));        //Steven 20180412 : For new HT-9045高度較高

    chkUser_Define_IndexZ_SafePos->Checked =CheckAndReadIniDataGeneral("Index", "EnableUser_Define_IndexZ_SafePos",   false);           //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
    edtUserDefineIndexZSafePos->Text       =CheckAndReadIniDataGeneral("Index", "UserDefineIndexZSafePos",   AnsiString("200"));        //Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)

    rgTrayVibration->ItemIndex  =CheckAndReadIniDataGeneral("System", "TRAY_VIBRATION",     0);             //Steven 20110627 : Tray Vibration
    rgTrayArmMode->ItemIndex    =CheckAndReadIniDataGeneral("System", "TRAY_ARM_MODE",      0);             //Frank 20230419
    rg2ndLoader->ItemIndex      =CheckAndReadIniDataGeneral("System", "USE_2nd_LOADER",     0);             //Steven 20240822 : For HT-9046AU
    rgTrayRobot->ItemIndex      =CheckAndReadIniDataGeneral("System", "USE_TRAY_ROBOT",     0);             //Steven 20170330 (Wei) : For HT-9046LM
    rgLoaderHinge->ItemIndex    =CheckAndReadIniDataGeneral("System", "USE_LOADER_HINGE",   0);             //Steven 20170330 (Wei) : For TSMC
    rgDieClean->ItemIndex       =CheckAndReadIniDataGeneral("System", "USE_DIE_CLEAN",      0);             //wei 20170418
    rgRealTimeCCD->ItemIndex    =CheckAndReadIniDataGeneral("System", "REAL_TIME_CCD",      0);             //Steven 20110705 : Real Time CCD
    rgRealTimeCCDTempNum->ItemIndex  =CheckAndReadIniDataGeneral("System", "RTC_TemperNumber",      0);     //Isaac 20201217 : RTC CCD增加第二組感溫
    rgCCDTemp->ItemIndex        =CheckAndReadIniDataGeneral("System", "CCD2_TEMPER",        0);             //Steven 20110705 : Real Time CCD
    rgLBTemp->ItemIndex         =CheckAndReadIniDataGeneral("System", "LB_TEMP",            0);             //Steven 20181023 : LB溫度
    rgLBTemp2->ItemIndex        =CheckAndReadIniDataGeneral("System", "LB_TEMP_UpDown",     0);             //Frank 20241231 : add
    rgESDTemp->ItemIndex        =CheckAndReadIniDataGeneral("System", "Index_ESDAir",       0);             //kevin 20200207 add index ESD temp
    rgHeater->ItemIndex         =CheckAndReadIniDataGeneral("System", "USE_16_HEATER",      eht4Heater);    //Steven 20111011 : 使用16個Heater

#if !EN_HEATER_SHEET                                                            //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定
    rgHeaterType->ItemIndex     =CheckAndReadIniDataGeneral("TempCtrl", "HEATER_CTRL_TYPE", KT4H);
#else
    HeaterInsOpt_Read();
    s_bSuppressHeaterTypeEvent=true;                                            //AI(ht9045-heater-control) 20260619 (RogerYang) : 回填rgHeaterType前先擋事件，避免洗掉混廠牌設定
    { int iFirst=-1;
      if(IsAllSame_HeaterInsOpt(true,&iFirst) && 0<=iFirst && iFirst<eHeaterInsOpt_Count)
          rgHeaterType->ItemIndex=iFirst;
      else
          rgHeaterType->ItemIndex=CheckAndReadIniDataGeneral("TempCtrl","HEATER_CTRL_TYPE",KT4H);
      for(int s=0; s<(int)g_vecHeaterTypeIdxForShow.size(); ++s)
      { int ti=g_vecHeaterTypeIdxForShow[s];
        if(ti<0||ti>=eHeaterType_Count) continue;
        TComboBox *pCb=g_tHeaterInsInfo[ti].GetCtrlItem_Cb();
        if(NULL!=pCb) pCb->ItemIndex=g_tHeaterInsInfo[ti].GetHeaterInsOpt(); } }
    s_bSuppressHeaterTypeEvent=false;                                           //AI(ht9045-heater-control) 20260619 (RogerYang) : 回填結束恢復事件
#endif          //Steven 20141030 : 新增OMRON E5DC溫控器
    rgColorSensor->ItemIndex    =CheckAndReadIniDataGeneral("System",  "USE_COLOR_TRAY_SENSOR", 0);         //20140903 wei colcr Tray
    rgSocketSen->ItemIndex      =CheckAndReadIniDataGeneral("System",  "USE_SOCKET_SENSOR", 0);             //JerryYang 20200327 Socket sensor改用硬體選項判斷

    rgCanBusMethod->ItemIndex    =CheckAndReadIniDataGeneral("System",  "Canbus_Method", 0);                //Sam 20210518 : 新增 CanBus 軟體配置

//    rgIndexCCD->ItemIndex       =CheckAndReadIniDataGeneral("System", "INDEX_CCD",      0);               //Ifor 20150720 : Index CCD //Ifor 20150728 Mark整合於RTC選項
//OCR-------------------------------
    rgOCR->ItemIndex            =CheckAndReadIniDataGeneral("System", "INSTALL_OCR",        eocrUninstal);  //Steven 20120716 : OCR
    cbOCR->Text                 =CheckAndReadIniDataGeneral("OCR",    "OCR_COM_PORT",       AnsiString("COM18"));
    cbOCRwithTester->Text       =CheckAndReadIniDataGeneral("OCR",    "OCRwithTester_COM_PORT",       AnsiString("COM19"));

    rgOCRYStepMot->ItemIndex    =CheckAndReadIniDataGeneral("System", "INSTALL_OCR_YMot",   eocrUninstal);  //Frank 20250214 add
//Barcode_2D------------------------
    rg2DBarcode->ItemIndex      =CheckAndReadIniDataGeneral("2D_BarCode", "BAR_CODE_INSTALL",  ebctUninstall);  //Steven 20120921 : Barcode_2D
    rgBottom2DID->ItemIndex     =CheckAndReadIniDataGeneral("2D_BarCode", "BOTTOM_2DID",       ebctUninstall);  //Steven 20190308 : Bottom 2D
    rgShuttleFloating->ItemIndex=CheckAndReadIniDataGeneral("2D_BarCode", "SHT_FLOATING_CHK",  ebctUninstall);  //Steven 20160920 : IC置偏檢查
    edCognexSystemCCD->Text     =CheckRange(CheckAndReadIniDataGeneral("2D_BarCode", "BAR_CODE_USECOUNT", 4), 2, 4);  //Ifor 20181206 :add COGNEX SYSTEM CCD Count
    rgBottom2DID_CCD->ItemIndex =CheckAndReadIniDataGeneral("2D_BarCode", "BOTTOM_2DID_CCD",   ebctUninstall);  //KaiChen 20200513 : Bottom 2D 8CCD
//----------------------------------
    rgInShtLastSensor->ItemIndex=CheckAndReadIniDataGeneral("System", "IN_SHT_LAST_SENSOR",  ebctUninstall);    //Steven 20181203 : In Shuttle最後一個Sensor定義
    rgSafeDoorLock->ItemIndex   =CheckAndReadIniDataGeneral("System", "SAFE_DOOR_LOCK",     0);             //20111130 Dell
    rgHeatGun->ItemIndex        =CheckAndReadIniDataGeneral("System", "INSTALL_HEAT_GUN",   0);             //kevin 20120523 : 選擇熱風槍機構模式
    rgATCHeatGun->ItemIndex     =CheckAndReadIniDataGeneral("System", "INSTALL_ATC_HEAT_GUN",   0);         //JerryYang 20220408 : add for ATC3.5
    rgMagBinDispType->ItemIndex =CheckAndReadIniDataGeneral("System", "MAGAZINE_BIN_DISP_TYPE", 0);         //JerryYang 20220909 : add magazine

    rgMotionCard->ItemIndex     =CheckAndReadIniDataGeneral("System", "MOTION_CARD_TYPE",   0);             //Brian 20121015 : 選擇Motion Card 模式
    rgMNetSpeed->ItemIndex      =CheckAndReadIniDataGeneral("System", "MOTIONNET_SPEED",    COMMSPEED_20M); //Steven 20181122 : MNet速度要可以修改
    rgIOCard->ItemIndex         =CheckAndReadIniDataGeneral("System", "IO_CARD_TYPE",       0);             //Brian 20121015 : 選擇I/O Card 模式
    rgTTLCard->ItemIndex        =CheckAndReadIniDataGeneral("System", "TTL_CARD_TYPE",      0);             //Steven 20121122 : 選擇TTL Card 模式
    rgTTLUseAddress->ItemIndex  =CheckAndReadIniDataGeneral("System", "TTL_CARD_USE_ADDRESS", 0);           //Isaac 20210922 : 選擇TTL板子是否帶站別
    rgHotPlatePos->ItemIndex    =CheckAndReadIniDataGeneral("System", "HOT_PLATE_POSITION", 0);             //Steven 20140222 : Hot Plate Pin的位置
    rgHotPlateLimit->ItemIndex  =CheckAndReadIniDataGeneral("System", "HOT_PLATE_LIMITATION", 0);           //Steven 20151117 : Hot Plate Pin的極限位置, 可以跑2x2 8吸嘴
    rgLaserDistance->ItemIndex  =CheckAndReadIniDataGeneral("System", "USE_LASER_DISTANCE", 0);             //Steven 20140228 : 雷射測距功能
    rgDeviceFlipper->ItemIndex  =CheckAndReadIniDataGeneral("System", "USE_DEVICE_FLIPPER", 0);             //Frank 20210612 : Flipper Function
//----------------------------------
    rgCCDAutoAlignmentMode->ItemIndex   = CheckAndReadIniDataGeneral("System", "MACHINE_HAS_AUTO_ALIGNMENT_CCD", 0);    //ChungHung 20210113 add for Alignment CCD   //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
//----------------------------------
//    rgFingerprintReaders->ItemIndex=tFingerInterface.bUseFingerprint==true?1:0;
//    rgFingerprintReaders->ItemIndex=CheckAndReadIniDataGeneral("System",  "USE_FINGER_PRINT", 0);//Steven 20190503 : 指紋辨識權限
//Comm Port-------------------------                                            //Steven 20120217 : Com Port改成可定義
    MyForceDirectories("D:\\RS232Standard\\System");
    AnsiString Str="D:\\RS232Standard\\System\\Setup.ini";

    cbComIndex->Text            =CheckAndReadIniDataGeneral("IndexDriver",     "COM_PORT",         AnsiString("COM1"));
    cbComTemp->Text             =CheckAndReadIniDataGeneral("TempCtrl",        "COM_PORT",         AnsiString("COM2"));
    cbComTempOmron->Text        =CheckAndReadIniDataGeneral("TempCtrl",        "COM_PORT_OMRON",   AnsiString("COM7"));
    cbComDyTemp->Text           =CheckAndReadIniDataGeneral("TempCtrl",        "COM_PORT_DYNAMIC", AnsiString("COM6"));
    cbComBinDisp->Text          =CheckAndReadIniDataGeneral("NUMBER_PANEL",    "COM_PORT",         AnsiString("COM4"));
    cbbComBinDisp2->Text        =CheckAndReadIniDataGeneral("NUMBER_PANEL2",    "COM_PORT",      AnsiString("COM4"));

    cbComRTC->Text              =CheckAndReadIniDataGeneral("RealTimeCCD",     "Port",             AnsiString("COM3"));
    cbComTester->Text           =CheckAndReadIniData(Str,   "COMPort",         "CommName",         AnsiString("COM5"));
    cbComTTLRS232->Text         =CheckAndReadIniData(Str,   "COMPort_TTL",     "CommName",         AnsiString("COM3"));
    cbComTTLRS232_2->Text       =CheckAndReadIniData(Str,   "COMPort_TTL_2",   "CommName",         AnsiString("COM8"));     //Isaac 20210309 :TTL RS232兩塊板子

    //ATC
    rgATC->ItemIndex            =CheckAndReadIniDataGeneral("ATC", "USE_ATC_MODE"   ,  eATCUninstall);  //jou 2012-03-08 Enable ATC mode
    cbbATC1->Text               =CheckAndReadIniDataGeneral("ATC", "ATC1_COM_PORT"  , AnsiString("COM15"));
    cbbATC2->Text               =CheckAndReadIniDataGeneral("ATC", "ATC2_COM_PORT"  , AnsiString("COM16"));
    cbbATC3->Text               =CheckAndReadIniDataGeneral("ATC", "ATC3_COM_PORT"  , AnsiString("COM17"));
    cbbATC4->Text               =CheckAndReadIniDataGeneral("ATC", "ATC4_COM_PORT"  , AnsiString("COM18"));

    rgATCMixMode->ItemIndex     =CheckAndReadIniDataGeneral("ATC", "ATC_Mix_Mode"   ,  eATCNonMix);  //jou 2012-03-08 Enable ATC mode

    edATCSystemIP->Text         =CheckAndReadIniDataGeneral("ATC", "ATC_SYSTEM_IP"  , AnsiString("172.16.8.90"));   //Ifor 20151230 :add New ATC Interface IP

    if(ATC_SYSTEM==eNewATCSystem)                                               //Ifor 20170620 (wei) add 使用New ATCSystem 強制設定Port為1234
        edATCSystemPort->Text   =1234;
    else
        edATCSystemPort->Text   =CheckAndReadIniDataGeneral("ATC", "ATC_SYSTEM_PORT", 1234);            //Ifor 20151230 :add New ATC Interface Port

    edATCSystemUseHeat->Text    =CheckAndReadIniDataGeneral("ATC", "ATC_SYSTEM_USEHEAT", 4);            //Ifor 20160506 :add New ATC Interface Use Heat Count

    rgInOutArmYPitch->ItemIndex =CheckAndReadIniDataGeneral("System", "USE_IN_OUT_ARM_Y_PITCH", iXPitch60);   //jou 2012-05-15 : 選擇 Y Pitch 機構模式

    rgOutArmYPitch->ItemIndex   =CheckAndReadIniDataGeneral("System", "USE_OUT_ARM_Y_PITCH", USE_IN_OUT_ARM_Y_PITCH);      //JerryYang 20251218 : IN/OUT ARM支援不同模組

    edtMinYPitch->Text          =CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_Y_PITCH_MIN", AnsiString("1500"));
    edtMaxYPitch->Text          =CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_Y_PITCH_MAX", AnsiString("7500"));
    rgInOutArmXPitch->ItemIndex =CheckAndReadIniDataGeneral("System", "USE_IN_OUT_ARM_X_PITCH", iXPitch40mm);//Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
    edtMinXPitch->Text          =CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MIN", AnsiString("4000"));    //JimmyChiu 20220708 : add Auto X pitch
    edtMaxXPitch->Text          =CheckAndReadIniDataGeneral("System", "IN_OUT_ARM_X_PITCH_MAX", AnsiString("12000"));   //JimmyChiu 20220708 : add Auto X pitch

    rgOutSortArm->ItemIndex     =CheckAndReadIniDataGeneral("OutSortArm", "USE_OUT_SORT_ARM", eartUninstall);           //Steven 20240822 : For HT-9046AU
    edtOutSortXPitchMin->Text   =CheckAndReadIniDataGeneral("OutSortArm", "USE_OUT_SORT_X_PITCH_MIN", AnsiString("1333")); //RogerYang 20250609 Add for 9046AU
    edtOutSortXPitchMax->Text   =CheckAndReadIniDataGeneral("OutSortArm", "USE_OUT_SORT_X_PITCH_MAX", AnsiString("4000"));

    edtHPLimit->Text            =CheckAndReadIniDataGeneral("System", "BASE_X_TO_HP",           AnsiString("6800"));  //Steven 20230826 : 基準軸X到加熱盤邊緣的距離
    rgHotplateType->ItemIndex   =CheckAndReadIniDataGeneral("System", "USE_HOTPLATE_TYPE", 0);          //jou 2012-05-15 : 選擇 Hotplate Type
    rgFix3FullPlace->ItemIndex  =CheckAndReadIniDataGeneral("System", "FIX3_FULL_PLACE",  Fix3K_Uninstall);           //Steven 20130126 : Fix3滿盤功能
    rgMagneticScale->ItemIndex  =CheckAndReadIniDataGeneral("System", "USE_MAGNETIC_SCALE", 0);         //Steven 20160426 : 磁性尺
    rgPickerCount->ItemIndex    =CheckAndReadIniDataGeneral("System", "USE_PICKER_COUNT", 1);           //Steven 20161117 : for HT-9045S
    rgPreciser->ItemIndex       =CheckAndReadIniDataGeneral("System", "USE_PRECISER", 1);               //Frank 20180410 (Steven) : InArm Preciser Station
//    cbPreciserInShuttleArea->Checked=PreciserInstallShuttleArea;                                        //Frank 20180410 (Steven) : InArm Preciser Station;
    rgPreciserPos->ItemIndex   =CheckAndReadIniDataGeneral("System", "iPreciserInstallArea", 0);//Ifor 20191008 : add Preciser Install Area

    rgRotateKit->ItemIndex      =CheckAndReadIniDataGeneral("ROTATE_KIT", "USE_ROTATE_KIT",    0);      //Steven 20121001 : 旋轉Kit
    rgRotateKitIn->ItemIndex    =CheckAndReadIniDataGeneral("ROTATE_KIT", "iRotate_In_Index",  0);      //Steven 20121001 : 旋轉Kit
    rgRotateKitOut->ItemIndex   =CheckAndReadIniDataGeneral("ROTATE_KIT", "iRotate_Out_Index", 2);      //Steven 20121001 : 旋轉Kit
    rgRotateKit_Type->ItemIndex =CheckAndReadIniDataGeneral("ROTATE_KIT", "RotateKit_Type", 0);         //2013-04-12    Dell :旋轉站;馬達版

    rgIOChangeToque->ItemIndex  =CheckAndReadIniDataGeneral("IndexDriver", "USE_IO_CHANGE_TOQUE", 0);   //jou 2012-06-21 Enable index I/O Change Toque
    rgIndexMotorAxis->ItemIndex =CheckAndReadIniDataGeneral("IndexDriver", "USE_INDEX_ARM_AXES",  0);   //JimmyChiu 20220708 : add Index Arm Axis
    rgIndexMotorType->ItemIndex =CheckAndReadIniDataGeneral("IndexDriver", "INDEX_DRIVER_TYPE",   0);
    chkUseHPComCard->Checked    =CheckAndReadIniDataGeneral("IndexDriver", "USE_HP_COM_CARD", false);   //Steven 20210204 : 使用鴻勁自製的通訊卡
    rgCleanAir->ItemIndex       =CheckAndReadIniDataGeneral("IndexDriver", "CLEAN_AIR", 0);             //ChungHung 20120821 : Clean Air

    rgInstallFix3->ItemIndex        =CheckAndReadIniDataGeneral("System", "FIX3_INSTALL",  0);          //2013-07-16     Dell    9046LS第一台沒Fix3;第二台以上Fix3標配
    rgShuttleCrossSensor->ItemIndex =CheckAndReadIniDataGeneral("System", "CROSS_SENSOR_INSTALL",  0);  //2013-07-16    Dell    Shuttle cross sensor
    rgShuttleZType->ItemIndex       =CheckAndReadIniDataGeneral("System", "SHUTTLE_Z_TYPE", 0);         //jou 2013-07-26 : 選擇 Shuttle Z Sensor Type
    rgAutoShuttleSensor->ItemIndex  =CheckAndReadIniDataGeneral("System", "AUTO_SENSOR_INSTALL",  0);   //wei 20160914 Auto Shuttle Sensor
    rgTrayMapping->ItemIndex        =CheckAndReadIniDataGeneral("System", "USE_TRAY_MAPPING",     0);   //wei 20161219 Tray Mapping
    rgShuttleVibration->ItemIndex   =CheckAndReadIniDataGeneral("System", "ShuttleVibration",     0);   //JerryYang 20171006 (wei) Shuttle 震動馬達
    rgFixAICCD->ItemIndex           =CheckAndReadIniDataGeneral("System", "Fix_AI_CCD",           0);   //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    rgMRSystem->ItemIndex           =CheckAndReadIniDataGeneral("System", "USE_MR_SYSTEM",        0);   //wei 20180702 MR
    rgRFIDSystem->ItemIndex         =CheckAndReadIniDataGeneral("System", "USE_RFID_SYSTEM",      0);   //wei 20180702 MR
    rgRFIDReader->ItemIndex         =CheckAndReadIniDataGeneral("System", "USE_RFID_READER",      0);   //Steven 20220713 : RFID Reader for SJSEMI
    cbbRFIDReader->Text             =CheckAndReadIniDataGeneral("RFID",   "RFIDReader_PORT", AnsiString("COM15"));
    rgAutoAlignment->ItemIndex      =CheckAndReadIniDataGeneral("System", "USE_AUTO_ALIGNMENT",    0);  //Sam 20181201 : AutoAlignment
    rgCOLORSENSOR_MUN->ItemIndex    =CheckAndReadIniDataGeneral("System", "USE_COLORSENSOR_MUN",  eCSMUN_Uninstall);   //Jimmychiu 20230630 : add color sensor MU-N in Loader

    rg3M_EM_AWARE_Monitor->ItemIndex=CheckAndReadIniDataGeneral("System", "USE_ESD_Monior",  0);        //2013-07-16     Dell    使用ESD monitor
    rgNovx3360->ItemIndex           =CheckAndReadIniDataGeneral("System", "USE_NOVX3360",  0);          //Steven 20131127 : 使用Simco ION風扇
    rgUsePulseType->ItemIndex       =CheckAndReadIniDataGeneral("System", "USE_PULSE_TYPE",  0);        //Ifor 20180316 : add Simco Use Pulse Type
//    if(CUSTOMER_CODE==CC_SJ_Semiconductor)
        edIONPulseCount->Text       =CheckRange(CheckAndReadIniDataGeneral("System", "ION_PULSE_COUNT", 3000), 3000, 100000);       //Steven 20220107 : 張寧要求ESD要快點Alarm     //Steven 20230322 : ION_PULSE_COUNT 最小值改成3000
//    else
//        edIONPulseCount->Text       =CheckRange(CheckAndReadIniDataGeneral("System", "ION_PULSE_COUNT", 5000), 5000, 20000);      //Ifor 20180316 : add Simco Use Pulse Type  //JerryYang 20210413 : 新電腦太快會誤alarm,最小值改為5000
    rgKasuga->ItemIndex             =CheckAndReadIniDataGeneral("System", "USE_KASUGA",  0);            //Ifor 20150731 : 使用Kasuga ION風扇
    rgChamberUsePulseType->ItemIndex=CheckAndReadIniDataGeneral("System", "CHAMBER_USE_PULSE_TYPE",  0);//Ifor 20190422 : add Chamber Use Pulse Type
    rgAuto3Magazine->ItemIndex      =CheckAndReadIniDataGeneral("System", "AUTO3_IS_MAGAZINE",       0);//JerryYang 20220909 : add magazine

    if(rgSocketSen->ItemIndex==0)                                               //JerryYang 20200327 重新整理第三個通訊模組
    {
        cbSocketSenAmpCnt->ItemIndex=0;
    }
    else
    {
        cbSocketSenAmpCnt->ItemIndex=CheckAndReadIniDataGeneral("System", "SocketSenAmpQty",  4);
    }

    cbSocketSenAmpCnt2nd->ItemIndex=CheckAndReadIniDataGeneral("System", "SocketSenAmpQty2nd",  0);

    cbSocketSenAmpCnt3rd->ItemIndex=CheckAndReadIniDataGeneral("System", "SocketSenAmpQty3rd",  0);     //JerryYang 20260205 : add第三組Socket sensor(8+16+8)

    rgAutoCleanIonFan->ItemIndex    =CheckAndReadIniDataGeneral("System", "USE_AutoCleanIonFan",  0);   //Isaac 20210609 : IO觸發IonFan清針

    if(rgRotateKit->ItemIndex==0)
    {
        cbRotateSenAmpCnt->ItemIndex=0;
    }
    else
    {
        cbRotateSenAmpCnt->ItemIndex=CheckAndReadIniDataGeneral("System", "RotateSenAmpQty",  4);
    }

    cbColorSenAmpCnt->ItemIndex=CheckAndReadIniDataGeneral("System", "ColorSenAmpQty",  0);

    cbVibrationCardQty->ItemIndex=CheckAndReadIniDataGeneral("System", "VibrationCardQty",  2);    //JerryYang 20230814 : add震動馬達通訊調速版本

    //Sam 20210518 : 新增 CanBus 軟體配置
    //==>
    coCanBusNudn1->ItemIndex=CheckAndReadIniDataGeneral("System", "CanBusNudn1Qty",  0);
    coNudn1Macid11->ItemIndex=CheckAndReadIniDataGeneral("System", "Nudn1Macid11Qty",  0);
    coNudn1Macid12->ItemIndex=CheckAndReadIniDataGeneral("System", "Nudn1Macid12Qty",  0);
    coNudn1Macid13->ItemIndex=CheckAndReadIniDataGeneral("System", "Nudn1Macid13Qty",  0);
    coNudn1Macid14->ItemIndex=CheckAndReadIniDataGeneral("System", "Nudn1Macid14Qty",  0);
    //<==
    //Sam 20210518 : 新增 CanBus 軟體配置

    rgKasuga_Fan->ItemIndex         =CheckAndReadIniDataGeneral("System", "USE_KASUGA_Fan",  0);        //KaiChen 20191225 ：KASUGA Fan 通訊
    rgHTIonBar->ItemIndex           =CheckAndReadIniDataGeneral("System", "HTIonBarFunction",  0);      //RogerYang 20250825 : Unloader新增3支IonBar，取代4 5 8 ion fan
    ed24VMonitorPulseCount->Text     =CheckAndReadIniDataGeneral("System", "I24V_PULSE_COUNT", 200);    //JerryYang 20190813 count最小值保護避免誤發alarm //Ifor 20190311 : add 24V Monitor Use Pulse Type
    rgOTDInstall->ItemIndex         =CheckAndReadIniDataGeneral("System", "USE_OTD",  0);               //Steven 20140312 : OTD

    rgUse4DUT->ItemIndex            =CheckAndReadIniDataGeneral("System", "SocketBasedAdd4Temp", 0);    //Steven 20131008 : for HT9046AH
    rgUseSucker_9046_DB->ItemIndex  =CheckAndReadIniDataGeneral("System", "USE_46_SUCKER_DB", 0);       //Steven 20131008 : HT9045使用46配氣
    rgUseSensor_9046_DB->ItemIndex  =CheckAndReadIniDataGeneral("System", "USE_46_SENSOR_DB", 0);       //Steven 20131008 : HT9045使用46配電
    rgIndexMotionCard->ItemIndex    =CheckAndReadIniDataGeneral("System", "INDEX_MOTION_CARD", 0);      //Steven 20210623 : Index使用Galil
    rg_ControlPanelMode->ItemIndex  =CheckAndReadIniDataGeneral("System", "ControlPanelMode", 0);       //Ken 20210702 AddPadInterface
    rgVacuUnitType->ItemIndex       =CheckAndReadIniDataGeneral("System", "VacuUnitType",     0);       //Sam 20230210 : 新增 VacuumUnit 通訊模組
    rgFinePitch->ItemIndex          =CheckAndReadIniDataGeneral("System",  "USE_FINE_PITCH", 0);        //Steven 20210624 : Fine Pitch
    rgUseOutSht->ItemIndex          =CheckAndReadIniDataGeneral("System",  "USE_OUT_SHT_MOT", 0);       //Steven 20210624 : Out shuttle獨立馬達

    rgAirConditioner->ItemIndex     =CheckAndReadIniDataGeneral("AIR_CON", "USE_AIR_CONDITIONER", 0);   //Steven 20131011 : 冷氣機
    cbAirCon->Text                  =CheckAndReadIniDataGeneral("AIR_CON", "AIR_CON_PORT",  AnsiString("COM15"));   //Steven 20131011 : 冷氣機

    cbEMAwarePort1->Text            =CheckAndReadIniDataGeneral("EM Aware", "EM_AWARE_PORT1",  AnsiString("COM15"));//Steven 20131114 : ESD Monitor
    cbEMAwarePort2->Text            =CheckAndReadIniDataGeneral("EM Aware", "EM_AWARE_PORT2",  AnsiString("COM16"));//Steven 20131114 : ESD Monitor
    cbEMAwarePort3->Text            =CheckAndReadIniDataGeneral("EM Aware", "EM_AWARE_PORT3",  AnsiString("COM17"));//Steven 20131114 : ESD Monitor
    cbEMAwarePort4->Text            =CheckAndReadIniDataGeneral("EM Aware", "EM_AWARE_PORT4",  AnsiString("COM18"));//Steven 20131114 : ESD Monitor
    cbESDUse4COM->Checked           =CheckAndReadIniDataGeneral("EM Aware", "EM_AWARE_USE_4_COM",  true);//Steven 20131114 : ESD Monitor
    cbNovx3360->Text                =CheckAndReadIniDataGeneral("EM Aware", "NOVX_3360_PORT",  AnsiString("COM18"));//Steven 20131127 : 使用Simco ION風扇
    cbKASUGA_Fan->Text              =CheckAndReadIniDataGeneral("System",   "KASUGA_Fan_PORT", AnsiString("COM18"));    //KaiChen 20191225 ：KASUGA Fan 通訊
    cbComLaser1->Text               =CheckAndReadIniDataGeneral("Laser",    "COM_Laser_1",     AnsiString("COM16"));//Steven 20140228 : 雷射測距功能
    cbComLaser2->Text               =CheckAndReadIniDataGeneral("Laser",    "COM_Laser_2",     AnsiString("COM17"));//Steven 20140228 : 雷射測距功能
    cbComLaserInArm->Text           =CheckAndReadIniDataGeneral("Laser",    "COM_Laser_InArm", AnsiString("COM18"));//Steven 20140228 : 雷射測距功能
    cb2DReader1->Text               =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode1_COM_PORT", AnsiString("COM15"));
    cb2DReader2->Text               =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode2_COM_PORT", AnsiString("COM16"));
    cb2DReader3->Text               =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode3_COM_PORT", AnsiString("COM17"));
    cb2DReader4->Text               =CheckAndReadIniDataGeneral("2D_BarCode",   "BarCode4_COM_PORT", AnsiString("COM18"));

    edMaxKpa->Text                  =CheckAndReadIniDataGeneral("System" , "EP_MAXKPA" , 499.0);
    edMaxMpaFB->Text                =CheckAndReadIniDataGeneral("System" , "EP_MAXA"   , 5.013);
    edMinMpa->Text                  =CheckAndReadIniDataGeneral("System" , "EP_MINMPA"   , 0.001);          //JerryYang 20171023 (wei) add PA Min
    edtMinMpaFB->Text               =CheckAndReadIniDataGeneral("System" , "EP_MINA_FeedBack"   , 0.908);

    rgAOI->ItemIndex                =CheckAndReadIniDataGeneral("System", "AOI",  0);                           //2014-04-07    Dell    for SPIL AOI
    rgVibrationCommuncation->ItemIndex=CheckAndReadIniDataGeneral("Vibration", "VibrationCommunication",  0);   //JerryYang 20230814 : add震動馬達通訊調速版本

    int iTemp175                    =CheckAndReadIniDataGeneral("System", "HighTemperatureSet175", 0);          //Frank 20160705 add 改機最高溫175度
    int iTemp155                    =CheckAndReadIniDataGeneral("System", "HighTemperatureSet155", 0);          //Sam 20240118 新增 155度 模式
    int iTemp150                    =CheckAndReadIniDataGeneral("System", "HighTemperatureSet150", 0);          //wei 20150617 改機最高溫150度
    if(iTemp175!=0)                                                             //Steven 20250701 : 整合機台溫度設定
    {
        rgHighTempLimit->ItemIndex  =tTemp175;
    }
    else if(iTemp155!=0)
    {
        rgHighTempLimit->ItemIndex  =tTemp155;
    }
    else if(iTemp150!=0)
    {
        rgHighTempLimit->ItemIndex  =tTemp150;
    }
    else
    {
        rgHighTempLimit->ItemIndex=CheckAndReadIniDataGeneral("System", "HighTempLimit", tTemp130);            //Steven 20250701 : 整合機台溫度設定
    }

    rgDewpointHW->ItemIndex         =CheckAndReadIniDataGeneral("System",  "DewPoint_Hardware_Install", 0); //Steven 20191017 : 露點計
    rgCatchTrayModel->ItemIndex     =CheckAndReadIniDataGeneral("System" , "USE_CATCH_TRAY_MODEL"   , 0);
    rgCatchTrayBlock->ItemIndex     =CheckAndReadIniDataGeneral("System" , "UseCatchTrayBlock"   , 0);      ;    //kevin 20200512 夾tray遮版削短
//    if(USE_AUTO_RETEST==eartInstall)
//        rgCatchTrayModel->ItemIndex     =2;                                   //wei 20150810

    rgCKDFCM->ItemIndex             =CheckAndReadIniDataGeneral("System" , "USE_CKD_FCM_CleanAir",   0);
    rgSocketClamp->ItemIndex        =CheckAndReadIniDataGeneral("System" , "INSTALL_SOCKET_CLAMP",   0);            //JerryYang 20160523 是否改機為SLK、SOCKET夾持汽缸
    rgDoubleEPControl->ItemIndex    =CheckAndReadIniDataGeneral("System" , "INSTALL_DOUBLE_EP",   0);               //Ifor 20190104 : Add Double EP Control

    rgGroundMan->ItemIndex              =CheckAndReadIniDataGeneral("Ground_Man", "USE_GROUND_MAN",  0);            //Steven 20190828 : 通訊式GroundMan
    cbbGroundMan->Text                  =CheckAndReadIniDataGeneral("Ground_Man", "Ground_Man_COM_PORT", AnsiString("COM18"));
    rgGroundMan_ScanPoint->ItemIndex    =CheckAndReadIniDataGeneral("Ground_Man", "Ground_Man_ScanPoint",  0);      //KaiChen 20191005 ：通訊式GroundMan
    edGroundMan_AlarmOhm->Text          =CheckAndReadIniDataGeneral("Ground_Man", "Ground_Man_AlarmOhm" , 3);       //KaiChen 20191005 ：通訊式GroundMan
    rgHotGunFlow->ItemIndex             =CheckAndReadIniDataGeneral("System", "HotGunFlowEnable", 0);               //KaiChen 20190729 ：Hot Gun Flow
    edHotGunFlow_LineNo->Text           =CheckAndReadIniDataGeneral("System", "HotGunFlow_LineNo" , 0);             //KaiChen 20190729 ：Hot Gun Flow
    edHotGunFlow_DevNo->Text            =CheckAndReadIniDataGeneral("System", "HotGunFlow_DevNo" , 0);              //KaiChen 20190729 ：Hot Gun Flow
    edHotGunFlow_Gun1_ChannelNo->Text   =CheckAndReadIniDataGeneral("System", "HotGunFlow_Gun1_ChannelNo" , 0);     //KaiChen 20190729 ：Hot Gun Flow
    edHotGunFlow_Gun2_ChannelNo->Text   =CheckAndReadIniDataGeneral("System", "HotGunFlow_Gun2_ChannelNo" , 0);     //KaiChen 20190729 ：Hot Gun Flow
    rgScanner_AOI->ItemIndex            =CheckAndReadIniDataGeneral("System", "Scanner_AOI",  0);                   //Ifor 20190725 : add Scanner AOI
    rgTopScanner_AOI->ItemIndex         =CheckAndReadIniDataGeneral("System", "Top_Scanner_AOI",  0);               //Ifor 20200902 add: TFAMD Top AOI
    rgLdUldUseStepMotor->ItemIndex      =CheckAndReadIniDataGeneral("TrayY", "LoaderUnload_StepMotor", 0);          //Steven 20200529 : Loader入Tray改步進
    cbbTrayStepMotor->Text              =CheckAndReadIniDataGeneral("TrayY", "COM PORT",  AnsiString("COM18"));
    if(iIndEPCnt==16)
    {
        cbIndEPCnt->ItemIndex=0;
    }
    else if(iIndEPCnt==8)                                                       //RogerYang 20260603 : Add 8EP
    {
        cbIndEPCnt->ItemIndex=2;
    }
    else
    {
        cbIndEPCnt->ItemIndex=1;                                                //JerryYang 20210413 : Add 4組獨立EP版本
    }

    rgOHTSystem->ItemIndex                  =CheckAndReadIniDataGeneral("System", "USE_OHT_SYSTEM",        0);          //KaiChen 20200716 ：OHT
    rgMultileEmpty->ItemIndex               =CheckAndReadIniDataGeneral("System", "USE_Multile_Empty",     0);          //KaiChen 20200716 ：OHT
    rgLoaderKeyence->ItemIndex              =CheckAndReadIniDataGeneral("System", "USE_KEYENCE_LOADER",     0);         //wei 20161219 Tray Mapping
    rgEmptyKeyence->ItemIndex               =CheckAndReadIniDataGeneral("System", "USE_KEYENCE_EMPTY",      0);         //wei 20161219 Tray Mapping
    rgMultileEmptyTrayIDKeyence->ItemIndex  =CheckAndReadIniDataGeneral("System", "USE_MultileEmptyTrayID_Keyence",      0);
    rgTrayMappingGrabImage->ItemIndex       =CheckAndReadIniDataGeneral("System", "TRAY_MAPPING_GRAB",      0);         //KenHsieh 20220923 : add Tray Map Throw IC Function
    rgBaseHeaterCount->ItemIndex            =CheckAndReadIniDataGeneral("System", "BASE_HEATER_COUNT",  0);             //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    rgShuttleFloodgate->ItemIndex           =CheckAndReadIniDataGeneral("System", "SHUTTLE_FLOODGATE",  0);             //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    rgTriTempMachine->ItemIndex             =CheckAndReadIniDataGeneral("System", "Tri_Temp_Machine",  0);              //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    rgAirStreamSelect->ItemIndex            =CheckAndReadIniDataGeneral("System", "AirStream_Select",  0);              //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    edtTriTemperature_MaxDegree->Text       =CheckAndReadIniDataGeneral("System", "Tri_Temperature_MaxDegree",  0);     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    edtTriTemperature_MinDegree->Text       =CheckAndReadIniDataGeneral("System", "Tri_Temperature_MinDegree",  0);     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    edtTriTempTotalCh->Text                 =CheckAndReadIniDataGeneral("System", "TriTemperature_TotalChannel",  0);   //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    edtOutShtMaxTemp->Text                  =CheckAndReadIniDataGeneral("System", "SetHeaterTemp_MaxOutSht",  20);      //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    edtIndexMaxTemp->Text                   =CheckAndReadIniDataGeneral("System", "SetHeaterTemp_MaxIndex",  20);       //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    edtBaseMaxTemp->Text                    =CheckAndReadIniDataGeneral("System", "SetHeaterTemp_MaxBase",  20);        //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    edt_Total_Compressor->Text              =CheckAndReadIniDataGeneral("System", "Total_Compressor",  3);              //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    rgSafePlcIO->ItemIndex                  =CheckAndReadIniDataGeneral("System", "SafePlcIO", eSafePLCIOType_Uninstall);//Jason 20230619 增加PLC_IO模組及安全相關IO.
    rg_IndexDoorHeater->ItemIndex           =CheckAndReadIniDataGeneral("System", "IndexDoorHeater",  0);               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    rgDoubleBeltMode->ItemIndex             =CheckAndReadIniDataGeneral("System", "DOUBLE_BELT_MODE",       0);         //Ztex 2023.12.13 Add Double Belt Mode

    rgCoverTrayID->ItemIndex                =CheckAndReadIniDataGeneral("System", "USE_COVER_TRAYID",        0);        //JerryYang 20240318 : add
    rgInShtAutoLatch->ItemIndex             =CheckAndReadIniDataGeneral("System", "In_Shuttle_Auto_Latch", 0);          //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    rgLDCarRotAtm->ItemIndex                =CheckAndReadIniDataGeneral("System", "USE_LD_Rot_Arm",        0);          //RogerYang 20250828 add for Loader Rotate Arm

    rgE84Sensor->ItemIndex                  =CheckAndReadIniDataGeneral("System", "AGVModal",  0);                //Frank 20251204 add E84
    rgLdUldCassetteMode->ItemIndex          =CheckAndReadIniDataGeneral("System", "USE_LdUldCassetteMode",  0);         //Frank 20251217 add
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::SaveSystemSet()
{
    if(Application->MessageBox("Do you want to store the setting?", NULL, MB_YESNO | MB_TOPMOST)!=IDYES) return;
//----------------------------------
    WriteIniDataGeneral("System", "AUTO_EMPTY_COLOR", MachineTrack->ItemIndex);
    AUTO_EMPTY_COLOR=MachineTrack->ItemIndex;
    if(MachineTrack->ItemIndex==2)                                              //7 Track (Empty Unloader)
    {
        WriteIniDataGeneral("System", "SUPPORT_2_EMPTY_EMPTY", 1);
    }
    else
    {
        WriteIniDataGeneral("System", "SUPPORT_2_EMPTY_EMPTY", 0);
    }

//----------------------------------
    WriteIniDataGeneral("System", "EP_Install", ElectronPressure->ItemIndex);
//    EP_Install=ElectronPressure->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("System", "INOUT_ARM_PICKER_USE_MOTOR", ArmZAtoZH->ItemIndex);
    InOutArmPickerUseMotor=ArmZAtoZH->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("TrayZ", "LOAD_Z_USE_MOTOR",  chkLoader->Checked);      //Steven 20190813 : 入Tray改用步進馬達
    WriteIniDataGeneral("TrayZ", "EMPTY_Z_USE_MOTOR", chkEmpty->Checked);
    WriteIniDataGeneral("TrayZ", "COLOR_Z_USE_MOTOR", chkColor->Checked);
    WriteIniDataGeneral("TrayZ", "AUTO1_Z_USE_MOTOR", chkAuto1->Checked);
    WriteIniDataGeneral("TrayZ", "AUTO2_Z_USE_MOTOR", chkAuto2->Checked);
    WriteIniDataGeneral("TrayZ", "AUTO3_Z_USE_MOTOR", chkAuto3->Checked);
    WriteIniDataGeneral("TrayZ", "AUTO4_Z_USE_MOTOR", chkAuto4->Checked);       //Steven 20230907 : For HT-9011UC
    WriteIniDataGeneral("TrayZ", "AUTO5_Z_USE_MOTOR", chkAuto5->Checked);
    WriteIniDataGeneral("TrayZ", "AUTO6_Z_USE_MOTOR", chkAuto6->Checked);

    WriteIniDataGeneral("TrayCassette", "LOAD_USE_Cassette",   cbLoaderCassette->Checked);      //Steven 20190813 : 入Tray改用步進馬達

    //AI(ht9045-v899) 20260423: cassette mode (Boat Carrier) already owns MAuto1Y/MAuto2Y, force-disable LOAD_Y_USE_MOTOR[3]/[4] to avoid motor contention
    if(rgLdUldCassetteMode->ItemIndex==1 && (chkAuto1Y->Checked || chkAuto2Y->Checked))
    {
        Application->MessageBox("Cassette mode (Boat Carrier) already uses MAuto1Y/MAuto2Y. AUTO1_Y_USE_MOTOR / AUTO2_Y_USE_MOTOR will be force-disabled.", "Conflict", MB_OK | MB_ICONWARNING);
        chkAuto1Y->Checked=false;
        chkAuto2Y->Checked=false;
    }

    WriteIniDataGeneral("TrayY", "LOAD_Y_USE_MOTOR", chkLoaderY->Checked);      //Jimmychiu 20240307 : Loader Tray改用步進馬達
    WriteIniDataGeneral("TrayY", "EMPTY_Y_USE_MOTOR", chkEmptyY->Checked);      //AI(ht9045-v899) 20260423: persist empty y stepper
    WriteIniDataGeneral("TrayY", "COLOR_Y_USE_MOTOR", chkColorY->Checked);      //AI(ht9045-v899) 20260423: persist color y stepper
    WriteIniDataGeneral("TrayY", "AUTO1_Y_USE_MOTOR", chkAuto1Y->Checked);      //AI(ht9045-v899) 20260423: persist auto1 y stepper
    WriteIniDataGeneral("TrayY", "AUTO2_Y_USE_MOTOR", chkAuto2Y->Checked);      //AI(ht9045-v899) 20260423: persist auto2 y stepper
    WriteIniDataGeneral("TrayY", "AUTO3_Y_USE_MOTOR", chkAuto3Y->Checked);      //AI(ht9045-v899) 20260423: persist auto3 y stepper
    WriteIniDataGeneral("TrayCassette", "EMPTY_USE_Cassette",  cbEmptyCassette->Checked);
    WriteIniDataGeneral("TrayCassette", "COLOR_USE_Cassette",  cbColorCassette->Checked);
    WriteIniDataGeneral("TrayCassette", "AUTO1_USE_Cassette",  cbAuto1Cassette->Checked);
    WriteIniDataGeneral("TrayCassette", "AUTO2_USE_Cassette",  cbAuto2Cassette->Checked);
    WriteIniDataGeneral("TrayCassette", "AUTO3_USE_Cassette",  cbAuto3Cassette->Checked);
    WriteIniDataGeneral("TrayCassette", "AUTO4_USE_Cassette",  cbAuto4Cassette->Checked);
    WriteIniDataGeneral("TrayCassette", "AUTO5_USE_Cassette",  cbAuto5Cassette->Checked);
    WriteIniDataGeneral("TrayCassette", "AUTO6_USE_Cassette",  cbAuto6Cassette->Checked);       //Steven 20230907 : For HT-9011UC

    LOAD_Z_USE_MOTOR[0]=chkLoader->Checked;
    LOAD_Z_USE_MOTOR[1]=chkEmpty->Checked;
    LOAD_Z_USE_MOTOR[2]=chkColor->Checked;
    LOAD_Z_USE_MOTOR[3]=chkAuto1->Checked;
    LOAD_Z_USE_MOTOR[4]=chkAuto2->Checked;
    LOAD_Z_USE_MOTOR[5]=chkAuto3->Checked;
    LOAD_Z_USE_MOTOR[6]=chkAuto4->Checked;                                      //Steven 20230907 : For HT-9011UC
    LOAD_Z_USE_MOTOR[7]=chkAuto5->Checked;
    LOAD_Z_USE_MOTOR[8]=chkAuto6->Checked;
    LOAD_Y_USE_MOTOR[0]=chkLoaderY->Checked;                                     //Jimmychiu 20240307 : Loader Tray改用步進馬達
    LOAD_Y_USE_MOTOR[1]=chkEmptyY->Checked;                                      //AI(ht9045-v899) 20260423: apply empty y stepper
    LOAD_Y_USE_MOTOR[2]=chkColorY->Checked;                                      //AI(ht9045-v899) 20260423: apply color y stepper
    LOAD_Y_USE_MOTOR[3]=chkAuto1Y->Checked;                                      //AI(ht9045-v899) 20260423: apply auto1 y stepper
    LOAD_Y_USE_MOTOR[4]=chkAuto2Y->Checked;                                      //AI(ht9045-v899) 20260423: apply auto2 y stepper
    LOAD_Y_USE_MOTOR[5]=chkAuto3Y->Checked;                                      //AI(ht9045-v899) 20260423: apply auto3 y stepper

    LOADUNLOAD_USE_CASSETTE[0]=cbLoaderCassette->Checked;
    LOADUNLOAD_USE_CASSETTE[1]=cbEmptyCassette->Checked;
    LOADUNLOAD_USE_CASSETTE[2]=cbColorCassette->Checked;
    LOADUNLOAD_USE_CASSETTE[3]=cbAuto1Cassette->Checked;
    LOADUNLOAD_USE_CASSETTE[4]=cbAuto2Cassette->Checked;
    LOADUNLOAD_USE_CASSETTE[5]=cbAuto3Cassette->Checked;
    LOADUNLOAD_USE_CASSETTE[6]=cbAuto4Cassette->Checked;                        //Steven 20230907 : For HT-9011UC
    LOADUNLOAD_USE_CASSETTE[7]=cbAuto5Cassette->Checked;
    LOADUNLOAD_USE_CASSETTE[8]=cbAuto6Cassette->Checked;

    WriteIniDataGeneral("TrayY", "LoaderUnload_StepMotor", rgLdUldUseStepMotor->ItemIndex); //Steven 20200529 : Loader入Tray改步進
    WriteIniDataGeneral("TrayY", "COM PORT",  cbbTrayStepMotor->Text);

//---------------------------------- kevin 20120718 Auto2分離氣缸裝置--------------------------
    WriteIniDataGeneral("System", "bUseAuto2Empty", Auto2SelectCy->ItemIndex);
    bUseAuto2Empty=Auto2SelectCy->ItemIndex;
//------------------------------------------------------------------------------
    USE_AUTO_RETEST=rgInstallAutoRestest->ItemIndex;
    if(USE_AUTO_RETEST)
    {
        UNLOADER_ART[eAuto1]=rgAuto1ART->ItemIndex;                             //Steven 20161221 (jou) : for SCK only Auto 2 has ART
        UNLOADER_ART[eAuto2]=rgAuto2ART->ItemIndex;                             //Steven 20161221 (jou) : for SCK only Auto 2 has ART
        UNLOADER_ART[eAuto3]=rgAuto3ART->ItemIndex;                             //Steven 20161221 (jou) : for SCK only Auto 2 has ART
        UNLOADER_ART[eAuto4]=rgAuto4ART->ItemIndex;                             //RogerYang 20250823 : Fix index
        UNLOADER_ART[eAuto5]=rgAuto5ART->ItemIndex;
        UNLOADER_ART[eAuto6]=rgAuto6ART->ItemIndex;
    }
    WriteIniDataGeneral("System", "USE_AUTO_RETEST",    USE_AUTO_RETEST);       //ChungHung 20140317 add Auto Retest
    WriteIniDataGeneral("System", "UNLOADER_AUTO1_ART", UNLOADER_ART[eAuto1]);  //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    WriteIniDataGeneral("System", "UNLOADER_AUTO2_ART", UNLOADER_ART[eAuto2]);  //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    WriteIniDataGeneral("System", "UNLOADER_AUTO3_ART", UNLOADER_ART[eAuto3]);  //Steven 20161221 (jou) : for SCK only Auto 2 has ART
    WriteIniDataGeneral("System", "UNLOADER_AUTO4_ART", UNLOADER_ART[eAuto4]);
    WriteIniDataGeneral("System", "UNLOADER_AUTO5_ART", UNLOADER_ART[eAuto5]);
    WriteIniDataGeneral("System", "UNLOADER_AUTO6_ART", UNLOADER_ART[eAuto6]);
    WriteIniDataGeneral("System", "bAutoTrackCanGoRear", rgAutoTrackCanGoRear->ItemIndex);  //Isaac 20180109 (Steven) : auto123可前進後退
    WriteIniDataGeneral("System", "bAutoZNoUseART", rgInstallARTtwocylinder->ItemIndex);    //kevin 20181023 add ART Auto 1 2 3  使用一段氣缸 k21

//----------------------------------
    CUSTOMER_CODE=atoi(edtCustomerCode->Text.c_str());
    WriteIniDataGeneral("System", "CUSTOMER_CODE", CUSTOMER_CODE);
    fMain->InitialSuperVisorPassword(CUSTOMER_CODE);
//----------------------------------
    WriteIniDataGeneral("System", "NUMBER_PANEL_TYPE", rgNumberPanelType->ItemIndex);
//    NUMBER_PANEL_TYPE=rgNumberPanelType->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("System",    "WEIGHT_CALIBRATION", rgWeightCali->ItemIndex);    //Steven 20111108
    WEIGHT_CALIBRATION=rgWeightCali->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("System", "ION_FAN_TYPE", rgIonFanType->ItemIndex);
    ION_FAN_TYPE=rgIonFanType->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("System", "SHUTTLE_SENSOR_TYPE", rgShuttleSensor->ItemIndex);
    SHUTTLE_SENSOR_TYPE=rgShuttleSensor->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("System", "NUEC_TYPE", rgNUECType->ItemIndex);          //Sam 20230707 : EtherCAT Shuttle sensor
    NUEC_TYPE=rgNUECType->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("System", "ENABLE_OUT_SHUTTLE_SENEOR", cbEnableOutShtSensor->Checked);      //Steven 20120531 : by pass out shuttle sensor
    ENABLE_OUT_SHUTTLE_SENEOR=cbEnableOutShtSensor->Checked;                                        //Steven 20120531 : by pass out shuttle sensor
    WriteIniDataGeneral("System", "ENABLE_OUT_SHUTTLEY_LATCH", chkOutShtYSensorByLatch->Checked);   //Steven 20170518 (jou) : Out shuttle Y sensor 使用 LATCH
    ENABLE_OUT_SHUTTLEY_LATCH=chkOutShtYSensorByLatch->Checked;
    WriteIniDataGeneral("System", "Use_AxisY_Sensor_2x3mode", chk2x3modeUseAxisYSensor->Checked);   //Isaac 20180402 (Steven) 2x3 mode 用Y sensor 偵測Outshuttle殘料
    Use_AxisY_Sensor_2x3mode=chk2x3modeUseAxisYSensor->Checked;

    WriteIniDataGeneral("System", "Bias_Mode_Use_Y_Sensor", chkBiasModeUseYSensor->Checked);
    Bias_Mode_Use_Y_Sensor=chkBiasModeUseYSensor->Checked;                      //Steven 20240805 : 1x2 Bias mode 用Y sensor 偵測Outshuttle殘料
//----------------------------------
    WriteIniDataGeneral("System", "SAFE_DOOR_AMOUNT", rgSafeDoor->ItemIndex);
    SAFE_DOOR_AMOUNT=rgSafeDoor->ItemIndex;
//----------------------------------
    WriteIniDataGeneral("Version", "Serial No", edtSeriaNo->Text);
//----------------------------------
    //jou 2010-05-19 start : 負壓
    WriteIniDataGeneral("System", "INDEX_SUCKER_TYPE", rgIndexSuckerType->ItemIndex);
    INDEX_SUCKER_TYPE=rgIndexSuckerType->ItemIndex;
    //jou 2010-05-19 end
//Steven 20110310 240KG----------------------------------
    WriteIniDataGeneral("System", "INDEX_PRESS_TYPE", rgIndexPressType->ItemIndex);
    INDEX_PRESS_TYPE=(eIndexPressType)rgIndexPressType->ItemIndex;
//Steven 20180412 : For new HT-9045高度較高--------------
    WriteIniDataGeneral("Index", "EnableUserDefMaxContactHeight",   chkUserDefMaxContactHeight->Checked);
    WriteIniDataGeneral("Index", "UserDefMaxContactHeight",         edtUserDefMaxContactHeight->Text);
//Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
//=>
    WriteIniDataGeneral("Index", "EnableUser_Define_IndexZ_SafePos",   chkUser_Define_IndexZ_SafePos->Checked);
    WriteIniDataGeneral("Index", "UserDefineIndexZSafePos",         edtUserDefineIndexZSafePos->Text);
//<=
//Richard 20230107 : SPIL客戶自定義機台安裝高度(200~800間)
//Steven 20110627 Tray Vibration-------------------------
    WriteIniDataGeneral("System", "TRAY_VIBRATION", rgTrayVibration->ItemIndex);
    TRAY_VIBRATION=rgTrayVibration->ItemIndex;

    WriteIniDataGeneral("System", "USE_2nd_LOADER", rg2ndLoader->ItemIndex);    //Steven 20240822 : For HT-9046AU
    USE_2nd_LOADER=rg2ndLoader->ItemIndex;

    WriteIniDataGeneral("System", "TRAY_ARM_MODE", rgTrayArmMode->ItemIndex);   //Frank 20230419
    TRAY_ARM_MODE=rgTrayArmMode->ItemIndex;

    WriteIniDataGeneral("System", "LOADER_VIBRATION", rgLoaderVibration->ItemIndex);  //JerryYang 20191001 loader震動馬達
    USE_LOADER_VIBRATION=rgLoaderVibration->ItemIndex;
//Steven 20170330 (Wei) : For HT-9046LM-------------------------
    WriteIniDataGeneral("System", "USE_TRAY_ROBOT", rgTrayRobot->ItemIndex);
    USE_TRAY_ROBOT=rgTrayRobot->ItemIndex;

//Steven 20170330 (Wei) : For TSMC------------------------------
    WriteIniDataGeneral("System", "USE_LOADER_HINGE", rgLoaderHinge->ItemIndex);
    USE_LOADER_HINGE=rgLoaderHinge->ItemIndex;

//wei 20170418
    WriteIniDataGeneral("System", "USE_DIE_CLEAN", rgDieClean->ItemIndex);
    USE_DIE_CLEAN=rgDieClean->ItemIndex;

//Ifor 20150708 CKD FCM Clean Air-------------------------
    WriteIniDataGeneral("System", "USE_CKD_FCM_CleanAir", rgCKDFCM->ItemIndex);
    USE_CKD_FCM_CleanAir=rgCKDFCM->ItemIndex;

//wei 20150807 Catch Tray Model
    WriteIniDataGeneral("System", "USE_CATCH_TRAY_MODEL", rgCatchTrayModel->ItemIndex);
    USE_CATCH_TRAY_MODEL=rgCatchTrayModel->ItemIndex;

    //kevin 20200512  夾tray 遮版削短 避免沒有夾好 TRAY ，讓TRAY移動過程 掉下去炸到 Pass TRAY
    WriteIniDataGeneral("System", "UseCatchTrayBlock", rgCatchTrayBlock->ItemIndex);
    bNewCatchTrayblock=rgCatchTrayBlock->ItemIndex;

//Steven 20111011 : 使用16個Heater
    WriteIniDataGeneral("System", "USE_16_HEATER",  rgHeater->ItemIndex);

#if !EN_HEATER_SHEET                                                            //AI(ht9045-heater-control) 20260618 (RogerYang) : 移植創發2type溫控模組設定
    WriteIniDataGeneral("TempCtrl", "HEATER_CTRL_TYPE",  rgHeaterType->ItemIndex);
#else
    { for(int ti=0; ti<eHeaterType_Count; ++ti)
      { int sIdx=TypeIdxToShowIdx(ti); int iInsOpt=INVALID_INT_VAL_NEG;
        if(INVALID_INT_VAL_NEG!=sIdx)
        { TComboBox *pCb=g_tHeaterInsInfo[ti].GetCtrlItem_Cb();
          if(NULL!=pCb) iInsOpt=pCb->ItemIndex; }
        g_tHeaterInsInfo[ti].m_iHeaterInsOpt=iInsOpt;
        WriteIniDataGeneral("TempCtrl", g_tHeaterInsInfo[ti].m_asSaveName, iInsOpt); }
      int iFirst=-1;
      if(IsAllSame_HeaterInsOpt(true,&iFirst) && 0<=iFirst && iFirst<eHeaterInsOpt_Count)
          WriteIniDataGeneral("TempCtrl","HEATER_CTRL_TYPE",iFirst);
      else
          WriteIniDataGeneral("TempCtrl","HEATER_CTRL_TYPE",rgHeaterType->ItemIndex); }
#endif      //Steven 20141030 : 新增OMRON E5DC溫控器

//Steven 20110704 Real Time CCD--------------------------
    WriteIniDataGeneral("System", "REAL_TIME_CCD",  rgRealTimeCCD->ItemIndex);
    //Isaac 20201217 : RTC CCD增加第二組感溫
    //=>
    if(rgRealTimeCCD->ItemIndex==0)
    {
        WriteIniDataGeneral("System", "RTC_TemperNumber",  0);
    }
    else
    {
        WriteIniDataGeneral("System", "RTC_TemperNumber",  rgRealTimeCCDTempNum->ItemIndex);
    }
    //<=
    //Isaac 20201217 : RTC CCD增加第二組感溫
    WriteIniDataGeneral("System", "CCD2_TEMPER",    rgCCDTemp->ItemIndex);
    WriteIniDataGeneral("System", "LB_TEMP",        rgLBTemp->ItemIndex);       //Steven 20181023 : LB溫度
    WriteIniDataGeneral("System", "LB_TEMP_UpDown", rgLBTemp2->ItemIndex);      //Frank 20241231 : add
    WriteIniDataGeneral("System", "Index_ESDAir",   rgESDTemp->ItemIndex);      //kevin 20200207 add index ESD temp

//Steven 20120716 : OCR----------------------------------
    WriteIniDataGeneral("System", "INSTALL_OCR",    rgOCR->ItemIndex);
    WriteIniDataGeneral("OCR",    "OCR_COM_PORT",   cbOCR->Text);
    WriteIniDataGeneral("OCR",    "OCRwithTester_COM_PORT",   cbOCRwithTester->Text);

    WriteIniDataGeneral("System", "INSTALL_OCR_YMot",rgOCRYStepMot->ItemIndex);             //Frank 20250214 add
//Barcode_2D------------------------
    WriteIniDataGeneral("2D_BarCode", "BOTTOM_2DID",       rgBottom2DID->ItemIndex);        //Steven 20190308 : Bottom 2D
    if(rgBottom2DID->ItemIndex==1)
        WriteIniDataGeneral("2D_BarCode", "BAR_CODE_INSTALL",  ebctUseCCDMode);             //Steven 20120921 : Barcode_2D
    else
        WriteIniDataGeneral("2D_BarCode", "BAR_CODE_INSTALL",  rg2DBarcode->ItemIndex);     //Steven 20120921 : Barcode_2D

    WriteIniDataGeneral("2D_BarCode", "SHT_FLOATING_CHK",  rgShuttleFloating->ItemIndex);   //Steven 20160920 : IC置偏檢查
    WriteIniDataGeneral("2D_BarCode", "BAR_CODE_USECOUNT",   edCognexSystemCCD->Text);      //Ifor 20160506 :add New ATC Interface Use Heat Count
    WriteIniDataGeneral("2D_BarCode", "BOTTOM_2DID_CCD",   rgBottom2DID_CCD->ItemIndex);    //KaiChen 20200513 : Bottom 2D 8CCD
//----------------------------------
    WriteIniDataGeneral("System", "IN_SHT_LAST_SENSOR",  rgInShtLastSensor->ItemIndex);     //Steven 20181203 : In Shuttle最後一個Sensor定義
    WriteIniDataGeneral("System", "SAFE_DOOR_LOCK", rgSafeDoorLock->ItemIndex);             //20111130  Dell
    WriteIniDataGeneral("System", "HOT_PLATE_POSITION", rgHotPlatePos->ItemIndex);          //Steven 20140222 : Hot Plate Pin的位置
    WriteIniDataGeneral("System", "HOT_PLATE_LIMITATION", rgHotPlateLimit->ItemIndex);      //Steven 20151117 : Hot Plate Pin的極限位置, 可以跑2x2 8吸嘴
    WriteIniDataGeneral("System", "USE_LASER_DISTANCE", rgLaserDistance->ItemIndex);        //Steven 20140228 : 雷射測距功能
    WriteIniDataGeneral("System", "USE_DEVICE_FLIPPER", rgDeviceFlipper->ItemIndex);        //Frank 20210612 : Flipper Function
    WriteIniDataGeneral("System", "MAGAZINE_BIN_DISP_TYPE", rgMagBinDispType->ItemIndex);   //JerryYang 20220909 : add magazine
//----------------------------------
    WriteIniDataGeneral("System", "MACHINE_HAS_AUTO_ALIGNMENT_CCD", rgCCDAutoAlignmentMode->ItemIndex);     //ChungHung 20210113 add for Alignment CCD  //KenHsieh 20210813 : add CCD AUTO ALIGNMENT

//Comm Port-------------------------                                            //Steven 20120217 : Com Port改成可定義
    AnsiString Str;
    Str="D:\\RS232Standard\\System";
    MyForceDirectories(Str, "THandlerSystem::SaveSystemSet");                   //檢查資料夾是否存在，不存在就建立一個新的
    Str+="\\Setup.ini";

    WriteIniDataGeneral("IndexDriver",   "COM_PORT",            cbComIndex->Text);
    WriteIniDataGeneral("TempCtrl",      "COM_PORT",            cbComTemp->Text);
    WriteIniDataGeneral("TempCtrl",      "COM_PORT_OMRON",      cbComTempOmron->Text);
    WriteIniDataGeneral("TempCtrl",      "COM_PORT_DYNAMIC",    cbComDyTemp->Text);
    WriteIniDataGeneral("NUMBER_PANEL",  "COM_PORT",            cbComBinDisp->Text);
    WriteIniDataGeneral("NUMBER_PANEL2", "COM_PORT",            cbbComBinDisp2->Text);

    WriteIniDataGeneral("RealTimeCCD",   "Port",                cbComRTC->Text);
    WriteIniData(Str,   "COMPort",       "CommName",            cbComTester->Text);
    WriteIniData(Str,   "COMPort_TTL",   "CommName",            cbComTTLRS232->Text);
    WriteIniData(Str,   "COMPort_TTL_2", "CommName",            cbComTTLRS232_2->Text);         //Isaac 20210309 :TTL RS232兩塊板子

    WriteIniDataGeneral("EM Aware",     "EM_AWARE_PORT1",       cbEMAwarePort1->Text);          //Steven 20131114 : ESD Monitor
    WriteIniDataGeneral("EM Aware",     "EM_AWARE_PORT2",       cbEMAwarePort2->Text);          //Steven 20131114 : ESD Monitor
    WriteIniDataGeneral("EM Aware",     "EM_AWARE_PORT3",       cbEMAwarePort3->Text);          //Steven 20131114 : ESD Monitor
    WriteIniDataGeneral("EM Aware",     "EM_AWARE_PORT4",       cbEMAwarePort4->Text);          //Steven 20131114 : ESD Monitor
    WriteIniDataGeneral("EM Aware",     "EM_AWARE_USE_4_COM",   cbESDUse4COM->Checked);         //Steven 20131114 : ESD Monitor
    WriteIniDataGeneral("EM Aware",     "NOVX_3360_PORT",       cbNovx3360->Text);              //Steven 20131127 : 使用Simco ION風扇

    WriteIniDataGeneral("System",       "KASUGA_Fan_PORT",      cbKASUGA_Fan->Text);            //KaiChen 20191225 ：KASUGA Fan 通訊

    //ATC
    WriteIniDataGeneral("ATC",          "USE_ATC_MODE",         rgATC->ItemIndex);              //jou 2012-03-08 Enable ATC mode
    WriteIniDataGeneral("ATC",          "ATC1_COM_PORT",        cbbATC1->Text);
    WriteIniDataGeneral("ATC",          "ATC2_COM_PORT",        cbbATC2->Text);
    WriteIniDataGeneral("ATC",          "ATC3_COM_PORT",        cbbATC3->Text);
    WriteIniDataGeneral("ATC",          "ATC4_COM_PORT",        cbbATC4->Text);

    WriteIniDataGeneral("ATC",          "ATC_Mix_Mode",         rgATCMixMode->ItemIndex);              //jou 2012-03-08 Enable ATC mode

    WriteIniDataGeneral("ATC",          "ATC_SYSTEM_IP",        edATCSystemIP->Text);
    if(ATC_SYSTEM==eNewATCSystem)                                                               //Ifor 20170620 (wei) add 使用New ATCSystem 強制設定Port為1234
    {
        WriteIniDataGeneral("ATC",      "ATC_SYSTEM_PORT",      1234);
        edATCSystemPort->Text=1234;
    }
    else
    {
        WriteIniDataGeneral("ATC",      "ATC_SYSTEM_PORT",      edATCSystemPort->Text);
    }
    WriteIniDataGeneral("ATC",          "ATC_SYSTEM_USEHEAT",   edATCSystemUseHeat->Text);      //Ifor 20160506 :add New ATC Interface Use Heat Count

    WriteIniDataGeneral("System",       "USE_IN_OUT_ARM_Y_PITCH", rgInOutArmYPitch->ItemIndex); //jou 2012-05-15 : 選擇 Y Pitch 機構模式

    WriteIniDataGeneral("System",       "USE_OUT_ARM_Y_PITCH", rgOutArmYPitch->ItemIndex);      //JerryYang 20251218 : IN/OUT ARM支援不同模組

    WriteIniDataGeneral("System",       "IN_OUT_ARM_Y_PITCH_MIN", edtMinYPitch->Text);
    WriteIniDataGeneral("System",       "IN_OUT_ARM_Y_PITCH_MAX", edtMaxYPitch->Text);
    WriteIniDataGeneral("System",       "USE_IN_OUT_ARM_X_PITCH", rgInOutArmXPitch->ItemIndex); //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
    WriteIniDataGeneral("System",       "IN_OUT_ARM_X_PITCH_MIN", edtMinXPitch->Text);          //JimmyChiu 20220708 : add Auto X pitch
    WriteIniDataGeneral("System",       "IN_OUT_ARM_X_PITCH_MAX", edtMaxXPitch->Text);          //JimmyChiu 20220708 : add Auto X pitch
    WriteIniDataGeneral("System",       "BASE_X_TO_HP",          edtHPLimit->Text);             //Steven 20230826 : 基準軸X到加熱盤邊緣的距離
    WriteIniDataGeneral("System",       "INSTALL_HEAT_GUN",     rgHeatGun->ItemIndex);          //kevin 20120523 : 選擇熱風槍機構模式
    WriteIniDataGeneral("System",       "INSTALL_ATC_HEAT_GUN", rgATCHeatGun->ItemIndex);       //JerryYang 20220408 : add for ATC3.5
    WriteIniDataGeneral("System",       "FIX3_FULL_PLACE",      rgFix3FullPlace->ItemIndex);    //Steven 20130126 : Fix3滿盤功能
    WriteIniDataGeneral("System",       "USE_MAGNETIC_SCALE",   rgMagneticScale->ItemIndex);    //Steven 20160426 : 磁性尺
    WriteIniDataGeneral("System",       "USE_PICKER_COUNT",     rgPickerCount->ItemIndex);      //Steven 20161117 : for HT-9045S
    WriteIniDataGeneral("System",       "USE_COLOR_TRAY_SENSOR", rgColorSensor->ItemIndex);     //20140903 wei colcr Tray
    WriteIniDataGeneral("System",       "USE_SOCKET_SENSOR",    rgSocketSen->ItemIndex);        //JerryYang 20200327 Socket sensor改用硬體選項判斷

    WriteIniDataGeneral("OutSortArm",   "USE_OUT_SORT_ARM",     rgOutSortArm->ItemIndex);       //Steven 20240822 : For HT-9046AU
    WriteIniDataGeneral("OutSortArm",   "USE_OUT_SORT_X_PITCH_MIN", edtOutSortXPitchMin->Text);
    WriteIniDataGeneral("OutSortArm",   "USE_OUT_SORT_X_PITCH_MAX", edtOutSortXPitchMax->Text);

    WriteIniDataGeneral("System",       "Canbus_Method",        rgCanBusMethod->ItemIndex);     //Sam 20210518 : 新增 CanBus 軟體配置

    WriteIniDataGeneral("System",       "USE_PRECISER",         rgPreciser->ItemIndex);         //Frank 20180410 (Steven) : InArm Preciser Station
    WriteIniDataGeneral("System",       "iPreciserInstallArea", rgPreciserPos->ItemIndex);      //Ifor 20191008 : add Preciser Install Area

    WriteIniDataGeneral("ROTATE_KIT",   "USE_ROTATE_KIT",       rgRotateKit->ItemIndex);        //Steven 20121001 : 旋轉Kit
    WriteIniDataGeneral("ROTATE_KIT",   "RotateKit_Type",       rgRotateKit_Type->ItemIndex);   //2013-04-12    Dell :旋轉站;馬達版

    if(rgRotateKit_Type->ItemIndex)
    {
        rgRotateKitIn->ItemIndex =-1;
        rgRotateKitOut->ItemIndex =-1;
    }
    WriteIniDataGeneral("ROTATE_KIT",   "iRotate_In_Index",     rgRotateKitIn->ItemIndex);      //Steven 20121001 : 旋轉Kit
    WriteIniDataGeneral("ROTATE_KIT",   "iRotate_Out_Index",    rgRotateKitOut->ItemIndex);     //Steven 20121001 : 旋轉Kit
    WriteIniDataGeneral("IndexDriver",  "USE_IO_CHANGE_TOQUE",  rgIOChangeToque->ItemIndex);    //jou 2012-06-21 Enable index I/O Change Toque
    WriteIniDataGeneral("IndexDriver",  "INDEX_DRIVER_TYPE",    rgIndexMotorType->ItemIndex);
    WriteIniDataGeneral("IndexDriver",  "USE_HP_COM_CARD",      chkUseHPComCard->Checked);      //Steven 20210204 : 使用鴻勁自製的通訊卡
    WriteIniDataGeneral("IndexDriver",  "CLEAN_AIR",            rgCleanAir->ItemIndex);         //ChungHung 20120821 : Clean Air
    WriteIniDataGeneral("IndexDriver",  "USE_INDEX_ARM_AXES",   rgIndexMotorAxis->ItemIndex);   //JimmyChiu 20220708 : add Index Arm Axis
    WriteIniDataGeneral("System",       "USE_HOTPLATE_TYPE",    rgHotplateType->ItemIndex);     //jou 2012-05-15 : 選擇 Hotplate Type
    WriteIniDataGeneral("System",       "MOTION_CARD_TYPE",     rgMotionCard->ItemIndex);       //Brian 20121015 : 選擇Motion Card 模式
    WriteIniDataGeneral("System",       "MOTIONNET_SPEED",      rgMNetSpeed->ItemIndex);        //Steven 20181122 : MNet速度要可以修改
    WriteIniDataGeneral("System",       "IO_CARD_TYPE",         rgIOCard->ItemIndex);           //Brian 20121015 : 選擇I/O Card 模式
    WriteIniDataGeneral("System",       "TTL_CARD_TYPE",        rgTTLCard->ItemIndex);          //Steven 20121122 : 選擇TTL Card 模式

    if(rgTTLCard->ItemIndex==3)                                                 //Isaac 20210922 : 選擇TTL板子是否帶站別，兩塊板子必帶站別
        WriteIniDataGeneral("System",       "TTL_CARD_USE_ADDRESS", 1);         //兩塊板子一定要有站別
    else
        WriteIniDataGeneral("System",       "TTL_CARD_USE_ADDRESS", rgTTLUseAddress->ItemIndex);

    WriteIniDataGeneral("System",       "SHUTTLE_Z_TYPE",       rgShuttleZType->ItemIndex);     //jou 2013-07-26 : 選擇 Shuttle Z Sensor Type
    WriteIniDataGeneral("System", "FIX3_INSTALL",          rgInstallFix3->ItemIndex);           //2013-07-16    Dell    9046LS第一台沒Fix3;第二台以上Fix3標配
    WriteIniDataGeneral("System", "CROSS_SENSOR_INSTALL",  rgShuttleCrossSensor->ItemIndex);    //2013-07-16    Dell    Shuttle cross sensor
    WriteIniDataGeneral("System", "AUTO_SENSOR_INSTALL",   rgAutoShuttleSensor->ItemIndex);     //wei 20160914 Auto Shuttle Sensor
    WriteIniDataGeneral("System", "ShuttleVibration",      rgShuttleVibration->ItemIndex);      //JerryYang 20171006 (wei) Shuttle 震動馬達
    WriteIniDataGeneral("System", "USE_TRAY_MAPPING",      rgTrayMapping->ItemIndex);           //wei 20161219 Tray Mapping
    WriteIniDataGeneral("System", "Fix_AI_CCD",            rgFixAICCD->ItemIndex);              //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    WriteIniDataGeneral("System", "USE_AUTO_ALIGNMENT",    rgAutoAlignment->ItemIndex);         //Sam 20181201 : AutoAlignment
    WriteIniDataGeneral("System", "USE_COLORSENSOR_MUN",   rgCOLORSENSOR_MUN->ItemIndex);       //Jimmychiu 20230630 : add color sensor MU-N in Loader
    WriteIniDataGeneral("System", "USE_ESD_Monior",        rg3M_EM_AWARE_Monitor->ItemIndex);   //2013-07-16     Dell    使用ESD monitor
    WriteIniDataGeneral("System", "USE_OTD",               rgOTDInstall->ItemIndex);            //Steven 20140312 : OTD
    WriteIniDataGeneral("System", "USE_NOVX3360",          rgNovx3360->ItemIndex);              //Steven 20131127 : 使用Simco ION風扇
    WriteIniDataGeneral("System", "USE_AutoCleanIonFan",   rgAutoCleanIonFan->ItemIndex);       //Isaac 20210609 : IO觸發IonFan清針
    WriteIniDataGeneral("System", "USE_PULSE_TYPE",        rgUsePulseType->ItemIndex);          //Ifor 20180316 : add Simco Use Pulse Type
    WriteIniDataGeneral("System", "ION_PULSE_COUNT",       edIONPulseCount->Text);              //Ifor 20180316 : add Simco Use Pulse Type
    WriteIniDataGeneral("System", "USE_KASUGA",            rgKasuga->ItemIndex);                //Ifor 20150731 : 使用Kasuga ION風扇
    WriteIniDataGeneral("System", "USE_KASUGA_Fan",        rgKasuga_Fan->ItemIndex);            //KaiChen 20191225 ：KASUGA Fan 通訊
    WriteIniDataGeneral("System", "HTIonBarFunction",      rgHTIonBar->ItemIndex);              //RogerYang 20250825 : Unloader新增3支IonBar，取代4 5 8 ion fan
    WriteIniDataGeneral("System", "SocketBasedAdd4Temp",   rgUse4DUT->ItemIndex);               //Steven 20131008 : for HT9046AH
    WriteIniDataGeneral("System", "USE_46_SUCKER_DB",      rgUseSucker_9046_DB->ItemIndex);     //Steven 20131008 : HT9045使用46配氣
    WriteIniDataGeneral("System", "USE_46_SENSOR_DB",      rgUseSensor_9046_DB->ItemIndex);     //Steven 20131008 : HT9045使用46配電
    WriteIniDataGeneral("System", "INDEX_MOTION_CARD",     rgIndexMotionCard->ItemIndex);       //Steven 20210623 : Index使用Galil
    WriteIniDataGeneral("System", "ControlPanelMode",      rg_ControlPanelMode->ItemIndex);     //Ken 20210702 AddPadInterface
    WriteIniDataGeneral("System", "VacuUnitType",          rgVacuUnitType->ItemIndex);          //Sam 20230210 : 新增 VacuumUnit 通訊模組
    WriteIniDataGeneral("System",  "USE_FINE_PITCH",       rgFinePitch->ItemIndex);             //Steven 20210624 : Fine Pitch
    WriteIniDataGeneral("System", "USE_OUT_SHT_MOT",       rgUseOutSht->ItemIndex);             //Steven 20210624 : Out shuttle獨立馬達
    WriteIniDataGeneral("AIR_CON", "USE_AIR_CONDITIONER",  rgAirConditioner->ItemIndex);        //Steven 20131011 : 冷氣機
    WriteIniDataGeneral("AIR_CON", "AIR_CON_PORT",         cbAirCon->Text);                     //Steven 20131011 : 冷氣機
    WriteIniDataGeneral("Laser",   "COM_Laser_1",          cbComLaser1->Text);                  //Steven 20140228 : 雷射測距功能
    WriteIniDataGeneral("Laser",   "COM_Laser_2",          cbComLaser2->Text);                  //Steven 20140228 : 雷射測距功能
    WriteIniDataGeneral("Laser",   "COM_Laser_InArm",      cbComLaserInArm->Text);              //Steven 20140228 : 雷射測距功能
    WriteIniDataGeneral("System", "AOI",  rgAOI->ItemIndex);                                    //2014-04-07    Dell    //2014-04-07    Dell    for SPIL AOI
    WriteIniDataGeneral("Vibration", "VibrationCommunication",  rgVibrationCommuncation->ItemIndex);       //JerryYang 20230814 : add震動馬達通訊調速版本
    WriteIniDataGeneral("System", "EP_MAXKPA", edMaxKpa->Text);
    WriteIniDataGeneral("System", "EP_MAXA"  , edMaxMpaFB->Text);
    WriteIniDataGeneral("System", "EP_MINMPA"  , edMinMpa->Text);                               //JerryYang 20171023 (wei) add PA Min
    WriteIniDataGeneral("System", "EP_MINA_FeedBack", edtMinMpaFB->Text);
    WriteIniDataGeneral("System", "USE_MR_SYSTEM",rgMRSystem->ItemIndex);                       //wei 20180822 MR
    WriteIniDataGeneral("System", "I24V_PULSE_COUNT",         ed24VMonitorPulseCount->Text);    //Ifor 20190311 : add 24V Monitor Use Pulse Type
    WriteIniDataGeneral("System", "USE_RFID_SYSTEM",        rgRFIDSystem->ItemIndex);           //wei 20180702 MR
    WriteIniDataGeneral("System", "USE_RFID_READER",        rgRFIDReader->ItemIndex);           //Steven 20220713 : RFID Reader for SJSEMI
    WriteIniDataGeneral("RFID",   "RFIDReader_PORT", cbbRFIDReader->Text);
    WriteIniDataGeneral("System", "CHAMBER_USE_PULSE_TYPE", rgChamberUsePulseType->ItemIndex);  //Ifor 20190422 : add Chamber Use Pulse Type
    WriteIniDataGeneral("System", "SocketSenAmpQty",        cbSocketSenAmpCnt->ItemIndex);      //JerryYang 20200327 socket/ rotate/ color的ampifier數量可分別設定
    WriteIniDataGeneral("System", "RotateSenAmpQty",        cbRotateSenAmpCnt->ItemIndex);
    WriteIniDataGeneral("System", "ColorSenAmpQty",         cbColorSenAmpCnt->ItemIndex);

    WriteIniDataGeneral("System", "SocketSenAmpQty2nd",     cbSocketSenAmpCnt2nd->ItemIndex);      //JerryYang 20200327 socket/ rotate/ color的ampifier數量可分別設定
    WriteIniDataGeneral("System", "SocketSenAmpQty3rd",     cbSocketSenAmpCnt3rd->ItemIndex);      //JerryYang 20260205 : add第三組Socket sensor(8+16+8)

    WriteIniDataGeneral("System", "VibrationCardQty",       cbVibrationCardQty->ItemIndex); //JerryYang 20230814 : add震動馬達通訊調速版本

    //Sam 20210518 : 新增 CanBus 軟體配置
    //<==
    WriteIniDataGeneral("System", "CanBusNudn1Qty",         coCanBusNudn1->ItemIndex);
    WriteIniDataGeneral("System", "Nudn1Macid11Qty",        coNudn1Macid11->ItemIndex);
    WriteIniDataGeneral("System", "Nudn1Macid12Qty",        coNudn1Macid12->ItemIndex);
    WriteIniDataGeneral("System", "Nudn1Macid13Qty",        coNudn1Macid13->ItemIndex);
    WriteIniDataGeneral("System", "Nudn1Macid14Qty",        coNudn1Macid14->ItemIndex);

    //<==
    //Sam 20210518 : 新增 CanBus 軟體配置

    WriteIniDataGeneral("System", "Scanner_AOI",  rgScanner_AOI->ItemIndex);        //Ifor 20190725 : add Scanner AOI
    WriteIniDataGeneral("System", "Top_Scanner_AOI",  rgTopScanner_AOI->ItemIndex); //Ifor 20200902 add: TFAMD Top AOI
    if(EP_Install!=0)                                                           //Steven 20140524 : 配合統一由外面讀取
    {
        EP_MAXKPA=CheckAndReadIniDataGeneral("System" , "EP_MAXKPA" , 499.0);
        EP_MAXAFB=CheckAndReadIniDataGeneral("System" , "EP_MAXA"   , 5.013);
        EP_MINMPA=CheckAndReadIniDataGeneral("System" , "EP_MINMPA" , 0.001);   //JerryYang 20171023 (wei) add PA Min
        EP_MinAFB=CheckAndReadIniDataGeneral("System" , "EP_MINA_FeedBack", 0.908);
    }

    WriteIniDataGeneral("2D_BarCode", "BarCode1_COM_PORT", cb2DReader1->Text);
    WriteIniDataGeneral("2D_BarCode", "BarCode2_COM_PORT", cb2DReader2->Text);
    WriteIniDataGeneral("2D_BarCode", "BarCode3_COM_PORT", cb2DReader3->Text);
    WriteIniDataGeneral("2D_BarCode", "BarCode4_COM_PORT", cb2DReader4->Text);

    //Steven 20140708 : 左左右加入機種的選擇
    Str="D:\\GPIB9045\\system\\general.ini";
    AnsiString Str2="9045GPIB";
    switch(cbHandlerModel->ItemIndex)
    {
        case 0: Str2="9045GPIB";        break;
        case 1: Str2="9046GPIB";        break;
        case 2: Str2="9045GPIB_12Site"; break;
        case 3: Str2="9046_32GPIB";     break;
        case 4: Str2="502GPIB";         break;
        case 5: Str2="1032GPIB";        break;
        case 6: Str2="7080GPIB";        break;
    }
    WriteIniData(Str, "Version", "Model", Str2);
    if(cbHandlerModel->ItemIndex==0)                                            //Steven 20190307 : Add for HT-9046LA
        WriteIniDataGeneral("Version", "SubModel", Type_None);
    else
        WriteIniDataGeneral("Version", "SubModel", rgModel->ItemIndex);

    WriteIniDataGeneral("Version", "Ver",                   asHandlerVersion);                      //Ifor 20161109 Handler Version Modify

    if(rgHighTempLimit->ItemIndex==tTemp150)                                    //Steven 20250701 : 整合機台溫度設定
    {
        WriteIniDataGeneral("System",  "HighTemperatureSet150", 1);             //wei 20150617 改機最高溫150度
        WriteIniDataGeneral("System",  "HighTemperatureSet155", 0);             //Sam 20240118 新增 155度 模式
        WriteIniDataGeneral("System",  "HighTemperatureSet175", 0);             //Frank 20160705 add 改機最高溫150度
    }
    else if(rgHighTempLimit->ItemIndex==tTemp155)
    {
        WriteIniDataGeneral("System",  "HighTemperatureSet150", 0);             //wei 20150617 改機最高溫150度
        WriteIniDataGeneral("System",  "HighTemperatureSet155", 1);             //Sam 20240118 新增 155度 模式
        WriteIniDataGeneral("System",  "HighTemperatureSet175", 0);             //Frank 20160705 add 改機最高溫150度
    }
    else if(rgHighTempLimit->ItemIndex==tTemp175)
    {
        WriteIniDataGeneral("System",  "HighTemperatureSet150", 0);             //wei 20150617 改機最高溫150度
        WriteIniDataGeneral("System",  "HighTemperatureSet155", 0);             //Sam 20240118 新增 155度 模式
        WriteIniDataGeneral("System",  "HighTemperatureSet175", 1);             //Frank 20160705 add 改機最高溫150度
    }
    else
    {
        WriteIniDataGeneral("System",  "HighTemperatureSet150", 0);             //wei 20150617 改機最高溫150度
        WriteIniDataGeneral("System",  "HighTemperatureSet155", 0);             //Sam 20240118 新增 155度 模式
        WriteIniDataGeneral("System",  "HighTemperatureSet175", 0);             //Frank 20160705 add 改機最高溫150度
    }
    WriteIniDataGeneral("System", "HighTempLimit", rgHighTempLimit->ItemIndex); //Steven 20250701 : 整合機台溫度設定

    WriteIniDataGeneral("System",  "DewPoint_Hardware_Install", rgDewpointHW->ItemIndex);           //Steven 20191017 : 露點計
    WriteIniDataGeneral("System",  "INSTALL_SOCKET_CLAMP",  rgSocketClamp->ItemIndex);              //JerryYang 20160523 是否改機為SLK、SOCKET夾持汽缸
    WriteIniDataGeneral("System",  "INSTALL_DOUBLE_EP",     rgDoubleEPControl->ItemIndex);          //Ifor 20190104 : Add Double EP Control

    WriteIniDataGeneral("Ground_Man", "USE_GROUND_MAN",  rgGroundMan->ItemIndex);                   //Steven 20190828 : 通訊式GroundMan
    WriteIniDataGeneral("Ground_Man", "Ground_Man_COM_PORT", cbbGroundMan->Text);
    WriteIniDataGeneral("Ground_Man", "Ground_Man_ScanPoint",   rgGroundMan_ScanPoint->ItemIndex);      //KaiChen 20191005 ：通訊式GroundMan
    WriteIniDataGeneral("Ground_Man", "Ground_Man_AlarmOhm",    edGroundMan_AlarmOhm->Text);            //KaiChen 20191005 ：通訊式GroundMan

    WriteIniDataGeneral("System",  "HotGunFlowEnable",              rgHotGunFlow->ItemIndex);               //KaiChen 20190729 ：Hot Gun Flow
    WriteIniDataGeneral("System",  "HotGunFlow_LineNo",             edHotGunFlow_LineNo->Text);             //KaiChen 20190729 ：Hot Gun Flow
    WriteIniDataGeneral("System",  "HotGunFlow_DevNo",              edHotGunFlow_DevNo->Text);              //KaiChen 20190729 ：Hot Gun Flow
    WriteIniDataGeneral("System",  "HotGunFlow_Gun1_ChannelNo",     edHotGunFlow_Gun1_ChannelNo->Text);     //KaiChen 20190729 ：Hot Gun Flow
    WriteIniDataGeneral("System",  "HotGunFlow_Gun2_ChannelNo",     edHotGunFlow_Gun2_ChannelNo->Text);     //KaiChen 20190729 ：Hot Gun Flow

    switch(cbIndEPCnt->ItemIndex)                                               //JerryYang 20210413 : Add 4組獨立EP版本
    {
        case 0: Str2="16";      break;
        case 1: Str2="4";       break;
        case 2: Str2="8";       break;                                          //RogerYang 20260603 : Add 8EP
    }
    WriteIniDataGeneral("System", "Individual_EP_COUNT", Str2);
    WriteIniDataGeneral("System", "AUTO3_IS_MAGAZINE",      rgAuto3Magazine->ItemIndex);                    //JerryYang 20210610 : add Auto3 Magazine

    WriteIniDataGeneral("System", "USE_OHT_SYSTEM",                 rgOHTSystem->ItemIndex);                //KaiChen 20200716 ：OHT
    WriteIniDataGeneral("System", "USE_Multile_Empty",              rgMultileEmpty->ItemIndex);             //KaiChen 20200716 ：OHT
    WriteIniDataGeneral("System", "USE_KEYENCE_LOADER",             rgLoaderKeyence->ItemIndex);            //wei 20161219 Tray Mapping
    WriteIniDataGeneral("System", "USE_KEYENCE_EMPTY",              rgEmptyKeyence->ItemIndex);             //wei 20161219 Tray Mapping
    WriteIniDataGeneral("System", "USE_MultileEmptyTrayID_Keyence", rgMultileEmptyTrayIDKeyence->ItemIndex);
    WriteIniDataGeneral("System", "TRAY_MAPPING_GRAB",              rgTrayMappingGrabImage->ItemIndex);     //KenHsieh 20220923 : add Tray Map Throw IC Function
    WriteIniDataGeneral("System", "BASE_HEATER_COUNT",              rgBaseHeaterCount->ItemIndex);          //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "SHUTTLE_FLOODGATE",              rgShuttleFloodgate->ItemIndex);         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "Tri_Temp_Machine",               rgTriTempMachine->ItemIndex);           //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "AirStream_Select",               rgAirStreamSelect->ItemIndex);          //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "Tri_Temperature_MaxDegree",      edtTriTemperature_MaxDegree->Text);     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "Tri_Temperature_MinDegree",      edtTriTemperature_MinDegree->Text);     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "TriTemperature_TotalChannel",    edtTriTempTotalCh->Text);               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "SetHeaterTemp_MaxOutSht",        edtOutShtMaxTemp->Text);                //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "SetHeaterTemp_MaxIndex",         edtIndexMaxTemp->Text);                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "SetHeaterTemp_MaxBase",          edtBaseMaxTemp->Text);                  //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "Total_Compressor",               edt_Total_Compressor->Text);            //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "IndexDoorHeater",                rg_IndexDoorHeater->ItemIndex);         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    WriteIniDataGeneral("System", "SafePlcIO",                      rgSafePlcIO->ItemIndex);                //Jason 20230619 增加PLC_IO模組及安全相關IO.
    WriteIniDataGeneral("System", "DOUBLE_BELT_MODE",               rgDoubleBeltMode->ItemIndex);           //Ztex 2023.12.13 Add Double Belt Mode

    WriteIniDataGeneral("System", "USE_COVER_TRAYID",               rgCoverTrayID->ItemIndex);
    WriteIniDataGeneral("MachineDefine", "Tri_Temp_Machine",        rgTriTempMachine->ItemIndex);           //Ztex 2024.03.05 Add MachineDefine Tri_Temp_Machine
    WriteIniDataGeneral("System", "In_Shuttle_Auto_Latch",          rgInShtAutoLatch->ItemIndex);           //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    WriteIniDataGeneral("System", "USE_LD_Rot_Arm",                 rgLDCarRotAtm->ItemIndex);              //RogerYang 20250828 add for Loader Rotate Arm
    WriteIniDataGeneral("System", "AGVModal",  rgE84Sensor->ItemIndex);                                     //Frank 20251204 add E84
    WriteIniDataGeneral("System", "USE_LdUldCassetteMode",          rgLdUldCassetteMode->ItemIndex);        //Ifor 20251216 add:Boat Carrier
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::SaveBtnClick(TObject *Sender)
{
    SaveSystemSet();
    Application->MessageBox("Please restart the program to active new parameters.", NULL, MB_OK|MB_TOPMOST);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::LoadBtnClick(TObject *Sender)
{
    LoaderSystemSet();
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::LoaderSafeDoorSet()
{
    SafeDoor1->Checked=Sen[SnSafeDoor1].Enable;
    SafeDoor2->Checked=Sen[SnSafeDoor2].Enable;
    SafeDoor3->Checked=Sen[SnSafeDoor3].Enable;
    SafeDoor4->Checked=Sen[SnSafeDoor4].Enable;
    SafeDoor5->Checked=Sen[SnSafeDoor5].Enable;
    SafeDoor6->Checked=Sen[SnSafeDoor6].Enable;
    SafeDoor7->Checked=Sen[SnSafeDoor7].Enable;
    SafeDoor8->Checked=Sen[SnSafeDoor8].Enable;
    SafeDoor9->Checked=Sen[SnSafeDoor9].Enable;
    SafeDoor10->Checked=Sen[SnSafeDoor10].Enable;
    HeaterDoor1->Checked=Sen[SnHeaterDoor].Enable;
    HeaterDoor2->Checked=Sen[SnHeaterDoor2].Enable;
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::SaveSafeDoorSet()
{
    Sen[SnSafeDoor1].Enable=SafeDoor1->Checked;
    Sen[SnSafeDoor2].Enable=SafeDoor2->Checked;
    Sen[SnSafeDoor3].Enable=SafeDoor3->Checked;
    Sen[SnSafeDoor4].Enable=SafeDoor4->Checked;
    Sen[SnSafeDoor5].Enable=SafeDoor5->Checked;
    Sen[SnSafeDoor6].Enable=SafeDoor6->Checked;
    Sen[SnSafeDoor7].Enable=SafeDoor7->Checked;
    Sen[SnSafeDoor8].Enable=SafeDoor8->Checked;
    Sen[SnSafeDoor9].Enable=SafeDoor9->Checked;
    Sen[SnSafeDoor10].Enable=SafeDoor10->Checked;
    Sen[SnHeaterDoor].Enable=HeaterDoor1->Checked;
    Sen[SnHeaterDoor2].Enable=HeaterDoor2->Checked;
    if(SAFE_DOOR_AMOUNT==0)                                                     //7扇門
    {
        Sen[SnSafeDoor4].Enable=false;
        Sen[SnSafeDoor5].Enable=false;
        Sen[SnSafeDoor10].Enable=false;
    }
    else if(SAFE_DOOR_AMOUNT==1)                                                //8扇門
    {
        Sen[SnSafeDoor4].Enable=false;
        Sen[SnSafeDoor5].Enable=false;
    }
    else if(SAFE_DOOR_AMOUNT==2)                                                //9扇門
    {
        Sen[SnSafeDoor10].Enable=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::ExitBtnClick(TObject *Sender)
{
    tsCustomerCode->TabVisible=false;
    HSys.ReadGeneralIni();                                                      //Steven 20190320 : Handler Sys讀檔獨立成Function
    SaveSafeDoorSet();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::BtnEnableAllClick(TObject *Sender)
{
    TCheckBox *Ptr[]={SafeDoor1, SafeDoor2, SafeDoor3, SafeDoor4, SafeDoor5,
                      SafeDoor6, SafeDoor7, SafeDoor8, SafeDoor9, SafeDoor10};

    for(int i=0; i<sizeof(Ptr)/4; i++)
        Ptr[i]->Checked=false;
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::rgCustomerListClick(TObject *Sender)
{
    AnsiString str=rgCustomerList->Items->Strings[rgCustomerList->ItemIndex];
    edtCustomerCode->Text=str.SubString(1, 3);
    if(edtCustomerCode->Text=="000")
        edtCustomerCode->Text="0";
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::ExitBtnMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
    {
        tsCustomerCode->TabVisible=true;
        pcSetting->ActivePageIndex=7;
        edtSearchCode->Visible=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tsCustomerCode->TabVisible=false;
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::btnSetATCComClick(TObject *Sender)
{
    cbComIndex->Text    ="COM11";
    cbComTemp->Text     ="COM12";
    cbComTempOmron->Text="COM13";
    cbComDyTemp->Text   ="COM4";
    cbComTester->Text   ="COM2";
    cbComBinDisp->Text  ="COM14";
    cbComRTC->Text      ="COM1";
    cbbATC1->Text       ="COM15";
    cbbATC2->Text       ="COM16";
    cbbATC3->Text       ="COM17";
    cbbATC4->Text       ="COM18";
    cbOCR->Text         ="COM3";
    cbAirCon->Text      ="COM15";
    cbComTTLRS232->Text ="COM3";
    cbComTTLRS232_2->Text="COM8";                                               //Isaac 20210309 :TTL RS232兩塊板子
}
//---------------------------------------------------------------------------
AnsiString THandlerSystem::GetCustomerName()                                    //Steven 20120524 : 改用迴圈取出客戶名稱
{
    AnsiString Str="HonPrec", tmp;
    int iCustomerCode;
    for(int i=0; i<rgCustomerList->Items->Count; i++)
    {
        iCustomerCode=atoi(rgCustomerList->Items->Strings[i].SubString(1, 3).c_str());     //取出前三碼的客戶代碼
        if(iCustomerCode==CUSTOMER_CODE)
        {
            if(CUSTOMER_CODE==CC_SPIL_CHINA_SUZHOU && SPIL_FOR_QLE==1)          //Steven 20230110 : For渠梁
            {
                Str="QLE";
            }
            else if(CUSTOMER_CODE==CC_IFXTH_Thai)                               //Ifor 20251113 add:客戶要求顯示Infineon
            {
                Str="Infineon";
            }
            else if(CUSTOMER_CODE==CC_Carsem_Thai)                              //Ifor 20260331 add:客戶要求顯示CARSEM
            {
                Str="CARSEM";
            }
            else
            {
                tmp=rgCustomerList->Items->Strings[i].SubString(5, rgCustomerList->Items->Strings[i].Length());   //去除前四碼為客戶代碼+空格
                Str=tmp.SubString(1, tmp.AnsiPos(" ")-1);                       //取得從第一個字元到第一個空格的字串
            }
            return Str;
        }
    }
    return Str;
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::rgRotateKit_TypeClick(TObject *Sender)
{
    rgRotateKitIn->Enabled =(rgRotateKit_Type->ItemIndex==0);
    rgRotateKitOut->Enabled=(rgRotateKit_Type->ItemIndex==0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtUserDefMaxContactHeightClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, -100.00, -160.00);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::pcSettingChange(TObject *Sender)
{
    edtSearchCode->Visible=(pcSetting->ActivePage==tsCustomerCode);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtSearchCodeChange(TObject *Sender)
{
    AnsiString Text;
    if(edtSearchCode->Text=="")
    {
        rgCustomerList->Items->Clear();
        for(int i=0; i<slCustomerCode->Count; i++)
            rgCustomerList->Items->Add(slCustomerCode->Strings[i]);
    }
    else
    {
        rgCustomerList->Items->Clear();
        for(int i=0; i<slCustomerCode->Count; i++)
        {
            Text=slCustomerCode->Strings[i].UpperCase();
            if(Text.AnsiPos(edtSearchCode->Text.UpperCase())!=0)
                rgCustomerList->Items->Add(slCustomerCode->Strings[i]);
        }
    }
}
//---------------------------------------------------------------------------
void THandlerSystem::SortItemToMap()
{
    InitItemToMap(pnlHandler1);
    InitItemToMap(pnlHandler2);
    InitItemToMap(pnlHandler3);

    InitItemToMap(pnlLoader1);
    InitItemToMap(pnlLoader2);
    InitItemToMap(pnlLoader3);
    InitItemToMap(pnlLoader4);
    InitItemToMap(pnlLoader5);

    InitItemToMap(pnlInArm1);
    InitItemToMap(pnlInArm2);
    InitItemToMap(pnlInArm3);
    TempComp.push_back(new TTempComponents(gbRotateKit));
    TempComp.push_back(new TTempComponents(grpOther));

    InitItemToMap(pnlESD1);
    InitItemToMap(pnlESD2);
    InitItemToMap(pnlOther1);
    InitItemToMap(pnlSht1);
    InitItemToMap(pnlSht2);

    InitItemToMap(pnlIndex1);
    InitItemToMap(pnlIndex2);
    InitItemToMap(pnlIndex3);
    InitItemToMap(pnlIndex4);
    InitItemToMap(pnlIndex5);
}
//---------------------------------------------------------------------------
void THandlerSystem::InitItemToMap(TWinControl *PCtrl)
{
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];

        TRadioGroup   *RadioGroup = dynamic_cast <TRadioGroup *>(P);
        TGroupBox     *GroupBox   = dynamic_cast <TGroupBox   *>(P);
        if(RadioGroup!=NULL)
        {
            TempComp.push_back(new TTempComponents(RadioGroup));
        }
        else if(GroupBox!=NULL)
        {
            TempComp.push_back(new TTempComponents(GroupBox));
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtSearchFunctionChange(TObject *Sender)
{
    if(edtSearchFunction->Text=="")
    {
        for(int i=0; i<(int)TempComp.size(); i++)
        {
            TempComp[i]->Obj->Parent=TempComp[i]->Obj->Parent;
        }
    }
    else if(edtSearchFunction->Text.Length()>2)
    {
        for(int i=0; i<(int)TempComp.size(); i++)
        {
            if(TempComp[i]->Caption.AnsiPos(edtSearchFunction->Text.UpperCase())!=0)
            {
                TempComp[i]->Obj->Parent=scrlbxSearchFunc;
            }
            else
            {
                if(TempComp[i]->Obj->Parent!=TempComp[i]->Par)
                    TempComp[i]->Obj->Parent=TempComp[i]->Par;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::rgTTLCardClick(TObject *Sender)                 //Isaac 20210922 : 選擇TTL板子是否帶站別，兩塊板子必帶站別
{
    if(rgTTLCard->ItemIndex==3)
    {
        rgTTLUseAddress->ItemIndex=1;
        rgTTLUseAddress->Enabled=false;
    }
    else
        rgTTLUseAddress->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtUserDefineIndexZSafePosClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 200, 800);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtCustomerCodeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 999, 0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edMaxMpaFBClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.0, 6.0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edMaxKpaClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 400, 950);   //JerryYang 20171211 (Steven) EP Max KPA 900 -> 900
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edMinMpaClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, -1.0, 10.0);  //JerryYang 20171211 (Steven) EP Min KPA 0 -> -1
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edATCSystemPortClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edATCSystemUseHeatClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 32, 4);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edHotGunFlow_Gun1_ChannelNoClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 3, 0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edHotGunFlow_DevNoClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 64, 0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtMinYPitchClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 6000, 1500);  //kevin 20170922 (wei) 小鍵盤上下限
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtMaxYPitchClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 80000, 4000);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edCognexSystemCCDClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 4, 2);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::ed24VMonitorPulseCountClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 5000, 1);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edGroundMan_AlarmOhmClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 1);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edIONPulseCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100000, 3000); //Steven 20230322 : ION_PULSE_COUNT 最小值改成3000
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtTriTempTotalChClick(TObject *Sender)         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 40, 4);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtOutShtMaxTempClick(TObject *Sender)          //Ztex 2023.04.19 Add HT-1032 TriTemp Function
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 120.0, 20.0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtTriTemperature_MaxDegreeClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 175.0, 35.0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtTriTemperature_MinDegreeClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 35.0, -55.0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtIndexMaxTempClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 120.0, 20.0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edt_Total_CompressorChange(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 8, 0);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtMinXPitchClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1100, 4000);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::edtMaxXPitchClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 3000, 16000);
}
//---------------------------------------------------------------------------
void __fastcall THandlerSystem::rgHeaterTypeClick(TObject *Sender)
{
#if EN_HEATER_SHEET
    if(s_bSuppressHeaterTypeEvent)                                              //AI(ht9045-heater-control) 20260619 (RogerYang) : 僅處理使用者實際點選，程式設定值時略過
        return;

    int iOpt=rgHeaterType->ItemIndex;
    if(iOpt<0 || iOpt>=eHeaterInsOpt_Count)                                     //AI(ht9045-heater-control) 20260619 (RogerYang) : 範圍保護
        return;

    for(int ti=0; ti<eHeaterType_Count; ++ti)                                   //AI(ht9045-heater-control) 20260619 (RogerYang) : rgHeaterType當下廠牌設為各站預設並即時回填ini(含原本ini沒有的站)
    {
        int iInsOpt=(INVALID_INT_VAL_NEG!=TypeIdxToShowIdx(ti))?iOpt:INVALID_INT_VAL_NEG;
        g_tHeaterInsInfo[ti].m_iHeaterInsOpt=iInsOpt;
        TComboBox *pCb=g_tHeaterInsInfo[ti].GetCtrlItem_Cb();
        if(NULL!=pCb)
            pCb->ItemIndex=iOpt;
        WriteIniDataGeneral("TempCtrl", g_tHeaterInsInfo[ti].m_asSaveName, iInsOpt);
    }
    WriteIniDataGeneral("TempCtrl", "HEATER_CTRL_TYPE", iOpt);                  //AI(ht9045-heater-control) 20260619 (RogerYang) : 全機統一為該廠牌
#endif
}
//---------------------------------------------------------------------------

