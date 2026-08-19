// =============================================================================
//  HandlerSys.cpp  --  Handler System Setup dialog: current-value read +
//                       widget fill (batch-5 wave, display-only bucket --
//                       see forms/fHandlerSys.h banner FIRST, it documents
//                       the full 44-method classification, GATE register,
//                       MAJOR RISK note, and DEFERRAL note; not repeated in
//                       full here).
//
//  Faithful translation of golden HandlerSys.cpp (1,376 lines, BCB6, cp950).
//  Translator: AI(W906-FW3-HandlerSys-WA) 20260819.
//  Translation wave: FW-3 HandlerSys Wave A (batch 5, display-only mandate).
//  Facade: forms/fHandlerSys.h.
//
//  WAVE SCOPE -- 3 of 44 golden methods (bucket (a), display/read-only; see
//  forms/fHandlerSys.h banner for the other 41's classification):
//    FormShow          golden :34-77   (GATE (H1) on SortItemToMap())
//    LoaderSystemSet   golden :79-457  (378 lines; see MAJOR RISK in header)
//    LoaderSafeDoorSet golden :990-1004
//
//  NOT translated this wave: the ctor (golden :18-24, `slCustomerCode=new
//  TStringList(); for(...) slCustomerCode->Add(rgCustomerList->Items->
//  Strings[i]);` -- entirely about the DEFERRED rgCustomerList/
//  slCustomerCode machinery, see forms/fHandlerSys.h's DEFERRAL note; this
//  facade's ctor is implicit/default, all widgets already NSDMI-constructed).
// =============================================================================
#include "MachineDefine.h"     // de-VCL'd include hub: vclcompat umbrella + portable STL
#pragma hdrstop

#include "forms/fHandlerSys.h"

#include "cmydef.h"       // the ~150 golden hardware-config globals LoaderSystemSet reads
                          //   (SUPPORT_2_EMPTY_EMPTY/LOAD_Z_USE_MOTOR[]/LOAD_Y_USE_MOTOR[]/
                          //   LOADUNLOAD_USE_CASSETTE[]/InOutArmPickerUseMotor/... -- see
                          //   forms/fHandlerSys.h's DEPENDENCY AUDIT for the full list)
                          //   + SnSafeDoor1..10/SnHeaterDoor/SnHeaterDoor2, SAFE_DOOR_AMOUNT
#include "MachineType.h"  // eAuto1..eAuto6/eNewATCSystem/iXPitch60/iXPitch40mm/tTemp*/
                          //   e85KG/eht4Heater/KT4H/eocrUninstal/ebctUninstall/
                          //   eATCUninstall/eATCNonMix/Fix3K_Uninstall/eCSMUN_Uninstall/
                          //   eartUninstall/COMMSPEED_20M/Type_None
#include "common.h"       // CheckAndReadIniData/CheckAndReadIniDataGeneral/CheckRange/
                          //   MyForceDirectories
#include "mysensor.h"     // Sen[]/TMySensor::Enable

//---------------------------------------------------------------------------
//  FormShow -- golden :34-77
//  GATE (H1): golden :36 `SortItemToMap();` -- SortItemToMap/InitItemToMap
//  need a TWinControl-shaped ControlCount/Controls[] walk that vclcompat's
//  TPanel/TGroupBox/TRadioGroup don't support. See forms/fHandlerSys.h
//  banner. `#if 0`-gated; NOT called.
//---------------------------------------------------------------------------
void THandlerSystem::FormShow()
{
#if 0
    SortItemToMap();   // GATE (H1)
#endif
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

    // golden :76 `myLog.Do_Log(Sender, asUser, asLogPath);` -- needs the
    // golden TObject *Sender this facade's FormShow intentionally drops (see
    // forms/fHandlerSys.h DEVIATION). myLog/Do_Log themselves exist
    // (handlerlog.h) -- not a missing-dependency gate, a dropped-parameter
    // consequence. Not translated.
}

//---------------------------------------------------------------------------
//  LoaderSystemSet -- golden :79-457
//---------------------------------------------------------------------------
void THandlerSystem::LoaderSystemSet()
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
    rgHeaterType->ItemIndex     =CheckAndReadIniDataGeneral("TempCtrl", "HEATER_CTRL_TYPE", KT4H);          //Steven 20141030 : 新增OMRON E5DC溫控器
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
    // GATE: `COMMSPEED_20M` (golden Motor/mn200.h #define) has NO ported
    // header anywhere in this tree -- SAME already-documented gate as
    // database.cpp:1082's `TODO(GA1-B6)` for this exact identifier (not a
    // new finding this wave). `#if 0`-gated; rgMNetSpeed->ItemIndex is left
    // untouched, matching that precedent's "value stays whatever it already
    // is" convention rather than substituting a guessed literal.
#if 0
    rgMNetSpeed->ItemIndex      =CheckAndReadIniDataGeneral("System", "MOTIONNET_SPEED",    COMMSPEED_20M); //Steven 20181122 : MNet速度要可以修改
#endif
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
    rgSafePlcIO->ItemIndex                  =CheckAndReadIniDataGeneral("System", "SafePlcIO", 0);                      //Jason 20230619 增加PLC_IO模組及安全相關IO.
    rg_IndexDoorHeater->ItemIndex           =CheckAndReadIniDataGeneral("System", "IndexDoorHeater",  0);               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    rgDoubleBeltMode->ItemIndex             =CheckAndReadIniDataGeneral("System", "DOUBLE_BELT_MODE",       0);         //Ztex 2023.12.13 Add Double Belt Mode

    rgCoverTrayID->ItemIndex                =CheckAndReadIniDataGeneral("System", "USE_COVER_TRAYID",        0);        //JerryYang 20240318 : add
    rgInShtAutoLatch->ItemIndex             =CheckAndReadIniDataGeneral("System", "In_Shuttle_Auto_Latch", 0);          //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    rgLDCarRotAtm->ItemIndex                =CheckAndReadIniDataGeneral("System", "USE_LD_Rot_Arm",        0);          //RogerYang 20250828 add for Loader Rotate Arm

    rgE84Sensor->ItemIndex                  =CheckAndReadIniDataGeneral("System", "AGVModal",  0);                //Frank 20251204 add E84
    rgLdUldCassetteMode->ItemIndex          =CheckAndReadIniDataGeneral("System", "USE_LdUldCassetteMode",  0);         //Frank 20251217 add
}

//---------------------------------------------------------------------------
//  LoaderSafeDoorSet -- golden :990-1004
//---------------------------------------------------------------------------
void THandlerSystem::LoaderSafeDoorSet()
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
