#include "MachineDefine.h"
#pragma hdrstop

#include "uhome.h"

#include "cmydef.h"
#include "csystem.h"
#include "cprod.h"
#include "MyMotor.h"
#include "atester.h"
#include "MyKitSuck.h"
#include "mycylin.h"
#include "main.h"
#include "myswitch.h"
#include "mymessbox.h"
#include "ckernel.h"
#include "note.h"
#include "rs232.h"
#include "adam6024.h"
#include "cMyDB.h"
#include "cpublic.h"
#include "uLotInfo.h"
#include "LtcSensor.h"
#include "AutoClean.h"
#include "OCR.h"
#include "fRotate.h"
#include "aRotateKIT.h"
#include "aRotateKIT_In.h"
#include "aRotateKIT_Out.h"
#include "MessageDef.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20160509 :add New ATC Interface
#include "mysensor.h"
#include "BarCode.h"
#include "acarry.h"
#include "asendic.h"
#include "myMN200motor.h"
#include "common.h"
#include "TrayStepMotor.h"
#include "cinitial.h"
#include "fAOI.h"
#include "Magazine.h"
#include "iosetview.h"
#include "aArmHeader.h"
#include "cTrayMapping.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern bool IndexZCanMove[2];
TfHome *fHome;
int iFramWidth;
int iFramHeight;
int iFramTop;
int iFramLeft;
//int iIndexTask=1;
bool DoIndexHome();
extern bool SetNoiseDelay;
TQPF_Timer SingleHomeDelay;
extern TMyHotPlatePlaceLog HPPlaceLog;                                          //Steven 20211110 : 記錄放料到加熱盤的位置
//------------------------------------------------------------------------------
THomeClass::THomeClass(int MotNo, int Order, bool bView) : TComponent(Owner)    //Stven 20120823 : Home元件改用Vector新增
{
    HomeOrder   =Order;
    index       =MotNo;
    Visible     =bView;
    THomeOrder  =HomeOrder;
    THomeFlag   =1;
    AnsiString str;

    if(Visible==true)
    {
        labName                     =new TLabel(this);
        labName     ->Parent        =(bView)?fHome->Panel1:fHome->palUnUse;
        labName     ->Visible       =bView;
        str.sprintf("labName%02d", Order);
        labName     ->Name          =str;
        labName     ->Color         =TColor(0x00C2B8A6);
        labName     ->Height        =20;
        labName     ->Font->Color   =clNavy;
        labName     ->Font->Size    =12;
        labName     ->Font->Name    ="MS Sans Serif";
        labName     ->Caption       =MOT[MotNo].NumberAlias;                    //Steven 20240509 : 改用馬達Class

        edPos               =new TEdit(this);
        edPos   ->Parent    =(bView)?fHome->Panel1:fHome->palUnUse;
        str.sprintf("edPos%02d", Order);
        edPos   ->Name      =str;
        edPos   ->Height    =20;
        edPos   ->Width     =50;
        edPos   ->Text      =0;
        edPos   ->ReadOnly  =true;

        ledHome             =new TALed(this);
        ledHome ->Parent    =(bView)?fHome->Panel1:fHome->palUnUse;
        str.sprintf("ledHome%02d", Order);
        ledHome ->Name      =str;
        ledHome ->LEDStyle  =LEDSqLarge;
        ledHome ->FalseColor=clSilver;
        ledHome ->TrueColor =clLime;
        ledHome ->Height    =20;
        ledHome ->Width     =20;
        ledHome ->Blink     =false;
    }
}
//---------------------------------------------------------------------------
__fastcall TfHome::TfHome(TComponent* Owner)
     : TForm(Owner)
{
    iHomeStep       =1;
    fShow           =false;
    fAbort          =false;
    ledHome->Visible=false;
}
//------------------------------------------------------------------------------
void __fastcall TfHome::InitialHomeClass()
{
    static bool bInitial=false;

    if(bInitial==true)
        return ;

    bInitial=true;

    AnsiString Str;

    HomeClass.push_back(new THomeClass(MInArmX       , SECOND_HOME));           //Stven 20120823 Start : Home元件改用Vector新增
    HomeClass.push_back(new THomeClass(MInArmY       , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MInArmPitch   , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MInArmZA      , FIRST_HOME));
    HomeClass.push_back(new THomeClass(MInArmZB      , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZC      , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZD      , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZE      , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZF      , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZG      , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZH      , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInShuttle1   , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MInShuttle2   , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MTestY1       , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MTestZ1       , FIRST_HOME));
    HomeClass.push_back(new THomeClass(MTestZ2       , FIRST_HOME));
    HomeClass.push_back(new THomeClass(MTestY2       , SECOND_HOME, (USE_INDEX_ARM_AXES==IndexArm_4_Axis)));            //Jimmychiu 20221123 : Add for HT9016C

    HomeClass.push_back(new THomeClass(MOutShuttle1  , SECOND_HOME, false));
    HomeClass.push_back(new THomeClass(MOutShuttle2  , SECOND_HOME, false));
    HomeClass.push_back(new THomeClass(MOutArmX      , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MOutArmY      , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MOutArmPitch  , SECOND_HOME));
    HomeClass.push_back(new THomeClass(MOutArmZA     , FIRST_HOME));
    HomeClass.push_back(new THomeClass(MOutArmZB     , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZC     , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZD     , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZE     , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZF     , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZG     , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZH     , FIRST_HOME, (InOutArmPickerUseMotor==eptUseMot)));
    if(TRAY_ARM_MODE==eUnderCoveyor)
        HomeClass.push_back(new THomeClass(MTrayX, SECOND_HOME));
    else
        HomeClass.push_back(new THomeClass(MTrayX, THREE_HOME));

    HomeClass.push_back(new THomeClass(MInArmPitchY  , SECOND_HOME, (USE_IN_Y_IS_AUTO_PITCH==true)));                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
    HomeClass.push_back(new THomeClass(MInArmPitchX2 , SECOND_HOME, (USE_IN_Y_IS_AUTO_PITCH==true)));
    HomeClass.push_back(new THomeClass(MOutArmPitchY , SECOND_HOME, (USE_OUT_Y_IS_AUTO_PITCH==true)));
    HomeClass.push_back(new THomeClass(MOutArmPitchX2, SECOND_HOME, (USE_OUT_Y_IS_AUTO_PITCH==true)));
    HomeClass.push_back(new THomeClass(MLoaderZ      , SECOND_HOME, LOAD_Z_USE_MOTOR[0]));
    HomeClass.push_back(new THomeClass(MEmptyZ       , SECOND_HOME, LOAD_Z_USE_MOTOR[1]));
    HomeClass.push_back(new THomeClass(MColorZ       , SECOND_HOME, LOAD_Z_USE_MOTOR[2]));
    HomeClass.push_back(new THomeClass(MAuto1Z       , SECOND_HOME, (LOAD_Z_USE_MOTOR[3] || USE_LdUldCassetteMode)));   //Ifor 20251216 add:Boat Carrier
    HomeClass.push_back(new THomeClass(MAuto2Z       , SECOND_HOME, (LOAD_Z_USE_MOTOR[4] || USE_LdUldCassetteMode)));   //Ifor 20251216 add:Boat Carrier
    HomeClass.push_back(new THomeClass(MAuto3Z       , SECOND_HOME, LOAD_Z_USE_MOTOR[5]));
    HomeClass.push_back(new THomeClass(MInRotateKit  , SECOND_HOME, (USE_ROTATE_KIT && iRotate_Type!=eCynRotate)));
    HomeClass.push_back(new THomeClass(MOutRotateKit , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e1MotRotate || iRotate_Type==e8MotRotate || iRotate_Type==e1MotRotate1Dut || iRotate_Type==eInOutArm1Motor))));  //add One sucker with rotate //Steven 20170705 (wei) : 修正Rotate回Home頁面顯示問題
    HomeClass.push_back(new THomeClass(MAOIKit       , SECOND_HOME, USE_AOI_Inspection));
    if(USE_LdUldCassetteMode==1)                                                //Ifor 20251216 add:Boat Carrier
        HomeClass.push_back(new THomeClass(MLoaderY  , FIRST_HOME, USE_LdUldCassetteMode));
    else
        HomeClass.push_back(new THomeClass(MLoaderY      , SECOND_HOME, (INSTALL_OCR_YMot==eocrYMotInstal)));           //RogerYang 20250909 : 只要是Motor就要回home
    HomeClass.push_back(new THomeClass(MEmptyY       , SECOND_HOME, false));
    HomeClass.push_back(new THomeClass(MColorY       , SECOND_HOME, false));
    if(USE_LdUldCassetteMode==1)                                                //Ifor 20251216 add:Boat Carrier
    {
        HomeClass.push_back(new THomeClass(MAuto1Y   , FIRST_HOME, USE_LdUldCassetteMode));
        HomeClass.push_back(new THomeClass(MAuto2Y   , FIRST_HOME, USE_LdUldCassetteMode));
    }
    else
    {
        HomeClass.push_back(new THomeClass(MAuto1Y       , SECOND_HOME, false));
        HomeClass.push_back(new THomeClass(MAuto2Y       , SECOND_HOME, false));
    }

    HomeClass.push_back(new THomeClass(MAuto3Y       , SECOND_HOME, false));
    HomeClass.push_back(new THomeClass(MInArmZAe     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));                      //Steven 20230323 : For HT1032
    HomeClass.push_back(new THomeClass(MInArmPitchX3 , SECOND_HOME, (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)));
    HomeClass.push_back(new THomeClass(MInArmPitchX4 , SECOND_HOME, (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)));
    HomeClass.push_back(new THomeClass(MInArmZAf     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmPitchX3, SECOND_HOME, (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)));
    HomeClass.push_back(new THomeClass(MOutArmPitchX4, SECOND_HOME, (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)));
    if(USE_LdUldCassetteMode==1)
        HomeClass.push_back(new THomeClass(MTrayZ        , SECOND_HOME,  (USE_LdUldCassetteMode)));                     //Ifor 20251216 add:Boat Carrier
    else
        HomeClass.push_back(new THomeClass(MTrayZ        , FIRST_HOME,  (TRAY_ARM_MODE==eUnderCoveyor)));               //Ifor 20251216 add:Boat Carrier
    HomeClass.push_back(new THomeClass(MOutSortAa    , FIRST_HOME, (USE_OUT_SORT_ARM==iOutSortX40mm)));                 //Steven 20240822 : For HT-9046AU
    HomeClass.push_back(new THomeClass(MOutSortAb    , FIRST_HOME, (USE_OUT_SORT_ARM==iOutSortX40mm)));
    HomeClass.push_back(new THomeClass(MInArmXScale  , THREE_HOME, false));
    HomeClass.push_back(new THomeClass(MInArmYScale  , THREE_HOME, false));
    HomeClass.push_back(new THomeClass(MOutArmXScale , THREE_HOME, false));
    HomeClass.push_back(new THomeClass(MOutArmYScale , THREE_HOME, false));
    HomeClass.push_back(new THomeClass(MShuttle1Pitch, SECOND_HOME, AUTO_SENSOR_INSTALL));                              //wei 20160914 Auto Shuttle Sensor
    HomeClass.push_back(new THomeClass(MShuttle2Pitch, SECOND_HOME, AUTO_SENSOR_INSTALL));                              //wei 20160914 Auto Shuttle Sensor
    //Steven 20170329 (Wei) : Add individual rotate motor
    //==>
    HomeClass.push_back(new THomeClass(MInRotateB    , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));             //JerryYang 20170605 (wei) 修正回Home頁面顯示問題  //Steven 20260316 : Fix operator precedence
    HomeClass.push_back(new THomeClass(MInRotateC    , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate))));
    HomeClass.push_back(new THomeClass(MInRotateD    , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate))));
    HomeClass.push_back(new THomeClass(MInRotateE    , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));  //Steven 20260316 : Fix operator precedence
    HomeClass.push_back(new THomeClass(MInRotateF    , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));             //Steven 20260316 : Fix operator precedence
    HomeClass.push_back(new THomeClass(MInRotateG    , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate))));
    HomeClass.push_back(new THomeClass(MInRotateH    , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate))));
    HomeClass.push_back(new THomeClass(MOutRotateB   , SECOND_HOME, (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    HomeClass.push_back(new THomeClass(MOutRotateC   , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    HomeClass.push_back(new THomeClass(MOutRotateD   , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    HomeClass.push_back(new THomeClass(MOutRotateE   , SECOND_HOME, (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    HomeClass.push_back(new THomeClass(MOutRotateF   , SECOND_HOME, (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    HomeClass.push_back(new THomeClass(MOutRotateG   , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    HomeClass.push_back(new THomeClass(MOutRotateH   , SECOND_HOME, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    //<==
    //Steven 20170329 (Wei) : Add individual rotate motor

    HomeClass.push_back(new THomeClass(MLightScale   , SECOND_HOME, USE_TRAY_ROBOT));                                   //Steven 20170330 (Wei) : For HT-9046LM
    HomeClass.push_back(new THomeClass(MInArmZAg     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZAh     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MArmAlignment , SECOND_HOME, false));
    HomeClass.push_back(new THomeClass(MLoadHingeR   , SECOND_HOME, USE_LOADER_HINGE));                                 //Steven 20170330 (Wei) : For TSMC
    HomeClass.push_back(new THomeClass(MLoadHingeZ   , SECOND_HOME, USE_LOADER_HINGE));                                 //Steven 20170330 (Wei) : For TSMC
    HomeClass.push_back(new THomeClass(MPreciser     , SECOND_HOME, USE_PRECISER));                                     //Frank 20180410 (Steven) : InArm Preciser Station
    //JerryYang 20180213 add motor
    //==>
    HomeClass.push_back(new THomeClass(MInArmZBe     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZBf     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZBg     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MInArmZBh     , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZAe    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZAf    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZAg    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZAh    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZBe    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZBf    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZBg    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    HomeClass.push_back(new THomeClass(MOutArmZBh    , FIRST_HOME,  (USE_PICKER_COUNT==ep16Picker && InOutArmPickerUseMotor==eptUseMot)));
    //<==
    //JerryYang 20180213 add motor

    HomeClass.push_back(new THomeClass(MOutSortX     , SECOND_HOME,  (USE_OUT_SORT_ARM==iOutSortX40mm)));               //Steven 20240822 : For HT-9046AU
    HomeClass.push_back(new THomeClass(MCaselevatorZ , FIRST_HOME,  false));    //Sam 20190112 LM

    HomeClass.push_back(new THomeClass(MCasArmX      , SECOND_HOME, USE_MR_SYSTEM));
    HomeClass.push_back(new THomeClass(MCasArmZ      , FIRST_HOME,  USE_MR_SYSTEM));
    HomeClass.push_back(new THomeClass(MTrayBracketZ , FIRST_HOME,  USE_MR_SYSTEM));
    HomeClass.push_back(new THomeClass(MStackedTrayX , SECOND_HOME, USE_MR_SYSTEM));
    HomeClass.push_back(new THomeClass(MStackedTrayZ , FIRST_HOME,  USE_MR_SYSTEM));

    HomeClass.push_back(new THomeClass(MUnloadRobotZ , THREE_HOME,  (USE_MR_SYSTEM==2)));
    HomeClass.push_back(new THomeClass(MOutSortY     , SECOND_HOME, (USE_OUT_SORT_ARM==iOutSortX40mm)));                //Steven 20240822 : For HT-9046AU

    HomeClass.push_back(new THomeClass(MCCDX         , THREE_HOME,  USE_FINE_PITCH));
    HomeClass.push_back(new THomeClass(MCCDY         , THREE_HOME,  USE_FINE_PITCH));
    HomeClass.push_back(new THomeClass(MCCDZ         , SECOND_HOME, USE_FINE_PITCH));

    //Frank 20210612 : Flipper Function
    //==>
    HomeClass.push_back(new THomeClass(MInFlipper1   , THREE_HOME,   USE_DEVICE_FLIPPER));
    HomeClass.push_back(new THomeClass(MInFlipper2   , THREE_HOME,   USE_DEVICE_FLIPPER));
    HomeClass.push_back(new THomeClass(MInFlipper3   , THREE_HOME,   USE_DEVICE_FLIPPER));
    HomeClass.push_back(new THomeClass(MOutFlipper1  , THREE_HOME,   USE_DEVICE_FLIPPER));
    HomeClass.push_back(new THomeClass(MOutFlipper2  , THREE_HOME,   USE_DEVICE_FLIPPER));
    HomeClass.push_back(new THomeClass(MOutFlipper3  , THREE_HOME,   USE_DEVICE_FLIPPER));
    //<==
    //Frank 20210612 : Flipper Function

    HomeClass.push_back(new THomeClass(MLdCarRotArm           , FIRST_HOME, USE_LD_Rot_Arm));
    if(USE_LdUldCassetteMode==1)                                                //Ifor 20251216 add:Boat Carrier
    {
        HomeClass.push_back(new THomeClass(MLoaderY_CCW       , FIRST_HOME,   USE_LdUldCassetteMode));
        HomeClass.push_back(new THomeClass(MAuto1Y_CCW        , FIRST_HOME,   USE_LdUldCassetteMode));
        HomeClass.push_back(new THomeClass(MAuto2Y_CCW        , FIRST_HOME,   USE_LdUldCassetteMode));
    }
    else
    {
        HomeClass.push_back(new THomeClass(MLoaderY_CCW           , SECOND_HOME,  false));
        HomeClass.push_back(new THomeClass(MAuto1Y_CCW            , SECOND_HOME,  false));
        HomeClass.push_back(new THomeClass(MAuto2Y_CCW            , SECOND_HOME,  false));
    }

    HomeClass.push_back(new THomeClass(MAuto3Y_CCW            , SECOND_HOME,  false));
    HomeClass.push_back(new THomeClass(MAuto4Y_CCW            , SECOND_HOME,  false));
    HomeClass.push_back(new THomeClass(MAuto5Y_CCW            , SECOND_HOME,  false));
    HomeClass.push_back(new THomeClass(MAuto6Y_CCW            , SECOND_HOME,  false));
    HomeClass.push_back(new THomeClass(M1_3R         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_4X         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_4Y         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_4R         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));

    HomeClass.push_back(new THomeClass(M1_5X         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_5Y         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_5R         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_6X         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_6Y         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_6R         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_7X         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_7Y         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_7R         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_8X         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_8Y         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(M1_8R         , SECOND_HOME,  (MachineTypeChoice==Type_HT502)));
    HomeClass.push_back(new THomeClass(MMagazine     , THREE_HOME,    AUTO3_IS_MAGAZINE));                              //JerryYang 20220909 : add magazine
    HomeClass.push_back(new THomeClass(MCatchMgzTray , SECOND_HOME,   AUTO3_IS_MAGAZINE));                              //JerryYang 20220909 : add magazine
    HomeClass.push_back(new THomeClass(MMagYTrayOut  , SECOND_HOME,   false));  //JerryYang 20220909 : add magazine
    HomeClass.push_back(new THomeClass(MFix3Full     , SECOND_HOME,  (FIX3_FULL_PLACE==Fix3K_UseStepperMotor)));        //JimmyChiu 20220927 : Stepper Motor Control in Fix3
    HomeClass.push_back(new THomeClass(MAuto4Z       , SECOND_HOME,  LOAD_Z_USE_MOTOR[6]));                             //Steven 20230907 : For HT-9011UC
    HomeClass.push_back(new THomeClass(MAuto5Z       , SECOND_HOME,  LOAD_Z_USE_MOTOR[7]));
    HomeClass.push_back(new THomeClass(MAuto6Z       , SECOND_HOME,  LOAD_Z_USE_MOTOR[8]));
    HomeClass.push_back(new THomeClass(MAuto4Y       , SECOND_HOME,  false));
    HomeClass.push_back(new THomeClass(MAuto5Y       , SECOND_HOME,  false));
    HomeClass.push_back(new THomeClass(MAuto6Y       , SECOND_HOME,  false));

    HomeClass.push_back(new THomeClass(MTopAOIArmX, FIRST_HOME,   false)); //USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall));  //Ian 20230823 Top AOI Function
    HomeClass.push_back(new THomeClass(MTopAOIArmY, FIRST_HOME,   false)); //USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall));  //Ian 20230823 Top AOI Function
    HomeClass.push_back(new THomeClass(MTopAOIArmR, SECOND_HOME,  false)); // USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall));  //Ian 20230823 Top AOI Function
    HomeClass.push_back(new THomeClass(MTopAOICCDZ, FIRST_HOME,    USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall));  //Ian 20230823 Top AOI Function
    HomeClass.push_back(new THomeClass(MTopAOIElevZ1 , SECOND_HOME,  false));
    HomeClass.push_back(new THomeClass(MTopAOIElevZ2 , SECOND_HOME,  false));

    HomeClass.push_back(new THomeClass(MOutSortPitchX, SECOND_HOME,  (USE_OUT_SORT_ARM==iOutSortX40mm)));               //Steven 20240822 : For HT-9046AU
    HomeClass.push_back(new THomeClass(MOutSortSht   , SECOND_HOME,  (USE_OUT_SORT_ARM==iOutSortX40mm)));
    HomeClass.push_back(new THomeClass(MLoad2Z       , SECOND_HOME,  (USE_2nd_LOADER==eartInstall)));                   //RogerYang 20250331 : For HT-9046AU
    HomeClass.push_back(new THomeClass(MLoad2Y      , SECOND_HOME,  false));

    HomeClass.push_back(new THomeClass(MInSh1LtcSenZ1, SECOND_HOME, In_Shuttle_Auto_Latch));                            //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    HomeClass.push_back(new THomeClass(MInSh1LtcSenZ2, SECOND_HOME, In_Shuttle_Auto_Latch));                            //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    HomeClass.push_back(new THomeClass(MInSh2LtcSenZ1, SECOND_HOME, In_Shuttle_Auto_Latch));                            //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    HomeClass.push_back(new THomeClass(MInSh2LtcSenZ2, SECOND_HOME, In_Shuttle_Auto_Latch));                            //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料

    for(int i=HomeClass.size(); i<TOTAL_MOTOR; i++)                             //Steven 20221115 : 修正歸零例外
    {
        Str.sprintf("[M%d]", i);
        HomeClass.push_back(new THomeClass(i, THREE_HOME,  false));
    }

    int iLPitch =270,   iTPitch =30;
    int iLabelL =5,     iLabelT =8;
    int iLedL   =170,   iLedT   =8;
    int iEditL  =195,   iEditT  =8;
    int iMaxRowItem=15;
    int ColItem=0, RowItem=0;

    for(unsigned int i=0; i<HomeClass.size(); i++)
    {
        if(HomeClass[i]->Visible)
        {
            HomeClass[i]->labName->Top  =iLabelT+iTPitch*RowItem;
            HomeClass[i]->labName->Left =iLabelL+iLPitch*ColItem;
            HomeClass[i]->ledHome->Top  =iLedT  +iTPitch*RowItem;
            HomeClass[i]->ledHome->Left =iLedL  +iLPitch*ColItem;
            HomeClass[i]->edPos->Top    =iEditT +iTPitch*RowItem;
            HomeClass[i]->edPos->Left   =iEditL +iLPitch*ColItem;
            RowItem++;

            if(RowItem>=iMaxRowItem)
            {
                RowItem=0;
                ColItem++;
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfHome::FormDestroy(TObject *Sender)
{
    try
    {
        for(vector<THomeClass *>::iterator iter=HomeClass.begin(); iter!=HomeClass.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(HomeClass);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfHome::FormDestroy");
    }
    LogSoftwareOffTime("TfHome, FormDestroy");                                  //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
int iSingleMotorHomeTask[TOTAL_MOTOR];
TQPF_Timer SMotorHomeDeleyTime[TOTAL_MOTOR];
void InitProcessSingleMotorTask(int Index)
{
    iSingleMotorHomeTask[Index]=1;
    fHome->iHomeStep=1;
}
//------------------------------------------------------------------------------
//void SetShuttleAxisCounter(int Index)
//{
//    if(Index==MInShuttle1)
//    {
//        _Hon_m4_set_axis_counter(0, 3, 0, 0, 1-MOT[MInShuttle1].Motor->Direction, 0);
//    }
//    else if(Index==MInShuttle2)
//    {
//        _Hon_m4_set_axis_counter(0, 3, 1, 0, 1-MOT[MInShuttle2].Motor->Direction, 0);
//    }
//}
//------------------------------------------------------------------------------
bool ProcessSingleMotorHome(int Index)
{
    int ret, iRef;
    int &iTask=iSingleMotorHomeTask[Index];
    switch(iTask)
    {
        case 1:                                                                 //檢查傳入的index是否在範圍內
            if(Index<0 || Index>=TOTAL_MOTOR)
            {
                ShowMyMessage("Error motor index!", "錯誤的馬達索引表", "ProcessSingleMotorHome");
                return true;
            }
            iTask=200;
            break;
        case 200:                                                               //檢查該馬達是否有被Enable
            MOT[Index].MotorInitial();
            if(MOT[Index].Motor->Enable==false)
            {
                MOT[Index].HomeFlag=1;
                MOT[Index].Position=0;                                          //Sam 20211009 : 修正軟體模擬 Home 失敗問題
                iTask=1;
                return true;
            }
            iTask=300;
            break;
        case 300:
            ret=MOT[Index].MotorHome(0);
            if(MOT[Index].HomeFlag==0 && ret==2)
            {
                MOT[Index].PCIL132_StopMotor();
                iTask=1;
                iRef=MOT[Index].GetErrorIndex();
                if(iRef==9) iRef=7;
                JamCode=MotorIndexToJamCode(Index);
                ShowMotorErrorMessage(JamCode, iRef+1);
                iTask=1;
                return false;
            }
            else if(ret==3)
            {
                iRef=MOT[Index].GetErrorIndex();
                if(iRef==9) iRef=6;
                JamCode=MotorIndexToJamCode(Index);
                if(fNote->fShow==false)                                         //Jimmychiu : 20221122 avoid double Alarm to stop
                    ShowMotorErrorMessage(JamCode, iRef+1);
                iTask=1;
                return false;
            }
            else if(ret==1 && MOT[Index].HomeFlag)
            {
                if(Index==MInArmZA ||
                   Index==MInArmZB ||
                   Index==MInArmZC ||
                   Index==MInArmZD ||
                   Index==MInArmZE ||
                   Index==MInArmZF ||
                   Index==MInArmZG ||
                   Index==MInArmZH ||
                   Index==MOutArmZA ||
                   Index==MOutArmZB ||
                   Index==MOutArmZC ||
                   Index==MOutArmZD ||
                   Index==MOutArmZE ||
                   Index==MOutArmZF ||
                   Index==MOutArmZG ||
                   Index==MOutArmZH ||
                   Index==MInArmZAe ||
                   Index==MInArmZAf ||
                   Index==MInArmZAg ||
                   Index==MInArmZAh ||
                   Index==MInArmZBe ||
                   Index==MInArmZBf ||
                   Index==MInArmZBg ||
                   Index==MInArmZBh ||
                   Index==MOutArmZAe ||
                   Index==MOutArmZAf ||
                   Index==MOutArmZAg ||
                   Index==MOutArmZAh ||
                   Index==MOutArmZBe ||
                   Index==MOutArmZBf ||
                   Index==MOutArmZBg ||
                   Index==MOutArmZBh ||
                   Index==MOutSortAa ||                                         //RogerYang 20250402 :for HT9046AU Add
                   Index==MOutSortAb)                                           //RogerYang 20250402 :for HT9046AU Add
                {
                    MOT[Index].HomeFlag=false;
                    iTask=500;
                    return false;
                }

                if(USE_MAGNETIC_SCALE)
                {
                    iTask=600;
                }
                else
                {
                    iTask=1;
                    return true;
                }
            }
            else if(ret==2)
            {
                MOT[Index].MotorInitial();
                iTask=400;
                break;
            }
            break;
        case 400:
            ret=MOT[Index].MotorHome(0);
            if(MOT[Index].HomeFlag==2 && ret==2)
            {
                MOT[Index].PCIL132_StopMotor();
                iTask=1;
                iRef=MOT[Index].GetErrorIndex();
                if(iRef==9) iRef=7;
                JamCode=MotorIndexToJamCode(Index);
                ShowMotorErrorMessage(JamCode, iRef+1);
                iTask=1;
                return false;
            }
            else if(ret==3)
            {
                iRef=MOT[Index].GetErrorIndex();
                if(iRef==9) iRef=6;
                JamCode=MotorIndexToJamCode(Index);
                ShowMotorErrorMessage(JamCode,iRef+1);
                iTask=1;
                return false;
            }
            else if(ret==1 && MOT[Index].HomeFlag)
            {
                if(Index==MInArmZA ||
                   Index==MInArmZB ||
                   Index==MInArmZC ||
                   Index==MInArmZD ||
                   Index==MInArmZE ||
                   Index==MInArmZF ||
                   Index==MInArmZG ||
                   Index==MInArmZH ||
                   Index==MOutArmZA ||
                   Index==MOutArmZB ||
                   Index==MOutArmZC ||
                   Index==MOutArmZD ||
                   Index==MOutArmZE ||
                   Index==MOutArmZF ||
                   Index==MOutArmZG ||
                   Index==MOutArmZH ||
                   Index==MInArmZAe ||                                          //Ztex 2023.12.25 For HT-1032AT
                   Index==MInArmZAf ||
                   Index==MInArmZAg ||
                   Index==MInArmZAh ||
                   Index==MInArmZBe ||
                   Index==MInArmZBf ||
                   Index==MInArmZBg ||
                   Index==MInArmZBh ||
                   Index==MOutArmZAe ||
                   Index==MOutArmZAf ||
                   Index==MOutArmZAg ||
                   Index==MOutArmZAh ||
                   Index==MOutArmZBe ||
                   Index==MOutArmZBf ||
                   Index==MOutArmZBg ||
                   Index==MOutArmZBh ||                                         //Ztex 2023.12.25 For HT-1032AT
                   Index==MOutSortAa ||                                         //RogerYang 20250402 :for HT9046AU Add
                   Index==MOutSortAb)                                           //RogerYang 20250402 :for HT9046AU Add
                {
                    MOT[Index].HomeFlag=false;
                    iTask=500;
                    return false;
                }

                if(USE_MAGNETIC_SCALE)
                {
                    iTask=600;
                }
                else
                {
                    iTask=1;
                    return true;
                }
            }
            break;
        case 500:                                                               //只有In / Out Arm吸嘴
            if(MOT[Index].MotorMove(ZSafePos))
            {
                iTask=1;
                MOT[Index].HomeFlag=true;
                return true;
            }
            break;
        case 600:                                                               //Steven 20160426 : 磁性尺
            if(Index==MInArmX || Index==MInArmY || Index==MOutArmX || Index==MOutArmY)
                SingleHomeDelay.SetSecAndOn(0.5);
            else
                SingleHomeDelay.SetSecAndOn(0.01);
            iTask=610;
            break;
        case 610:
            if(SingleHomeDelay.Off())
            {
                if(Index==MInArmX)
                {
                    MOT[MInArmXScale].Motor->ResetPos(MOT[MInArmX].ReadEncoderPos());
                    MOT[MLightScale].Motor->ResetPos(MOT[MInArmX].ReadEncoderPos());
                }

                if(Index==MInArmY)
                {
                    MOT[MInArmYScale].Motor->ResetPos(MOT[MInArmY].ReadEncoderPos());
                    MOT[MLightScale].Motor->ResetPos(MOT[MInArmY].ReadEncoderPos());
                }

                if(Index==MOutArmX)
                {
                    MOT[MOutArmXScale].Motor->ResetPos(MOT[MOutArmX].ReadEncoderPos());
                    MOT[MLightScale].Motor->ResetPos(MOT[MOutArmX].ReadEncoderPos());
                }

                if(Index==MOutArmY)
                {
                    MOT[MOutArmYScale].Motor->ResetPos(MOT[MOutArmY].ReadEncoderPos());
                    MOT[MLightScale].Motor->ResetPos(MOT[MOutArmY].ReadEncoderPos());
                }
                iTask=1;
                return true;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfHome::InitGali_HomeTask()
{
    IndexZCanMove[0]=true;
    IndexZCanMove[1]=true;
    MOT[MTestY1].MovFlag=false;
    MOT[MTestY2].MovFlag=false;
    MOT[MTestZ1].MovFlag=false;
    MOT[MTestZ2].MovFlag=false;
    MOT[MTestY1].bScanFlag=false;
    MOT[MTestY2].bScanFlag=false;
    MOT[MTestZ1].bScanFlag=false;
    MOT[MTestZ2].bScanFlag=false;
    MOT[MTestY1].GaliSofDelayCount=0;
    MOT[MTestY2].GaliSofDelayCount=0;
    MOT[MTestZ1].GaliSofDelayCount=0;
    MOT[MTestZ2].GaliSofDelayCount=0;
}
//---------------------------------------------------------------------------
void TfHome::ShowLed(int index, int attr)
{
    if(HomeClass[index]->Visible==true)
    {
        if(attr)
        {
            if(     attr==1) HomeClass[index]->ledHome->TrueColor=clLime;
            else if(attr==2) HomeClass[index]->ledHome->TrueColor=clRed;
            else if(attr==3) HomeClass[index]->ledHome->TrueColor=clYellow;
            HomeClass[index]->ledHome->Value=true;
        }
        else
        {
            HomeClass[index]->ledHome->Value=false;
        }
    }
}
//---------------------------------------------------------------------------
void TfHome::ResetAllMotorLed()
{
    for(int i=0; i<TOTAL_MOTOR; i++)
        ShowLed(i, 0);
}
//---------------------------------------------------------------------------
bool TfHome::ShowMotorHomePos(int index)
{
    AnsiString S1, S2;
    if(HomeClass[index]->Visible==true)
    {
        if(index==MTestY1 || index==MTestZ1 || index==MTestZ2 || index==MTestY2)
            HomeClass[index]->edPos->Text=MOT[index].Gali_ReadPos();
        else
            HomeClass[index]->edPos->Text=MOT[index].ReadPos();

        if((index>=MInArmZA && index<=MInArmZH) ||
           (index>=MOutArmZA && index<=MOutArmZH))                              //Steven 20091120 Start : Check Suck Z Home Position
        {
            if(atoi(HomeClass[index]->edPos->Text.c_str())>5000)
            {
                StopAllMotor();
                fHome->ShowLed(index, 2);
                S1.sprintf("M%2d position error, please check the home sensor!!", index);
                S2.sprintf("M%2d 位置錯誤，請檢查歸零感應器!!", index);
                ShowMyMessage(S1, S2, "ShowMotorHomePos");
                MOT[index].PCIL132_SetPos(0);
                fHome->fAbort=false;
                SystemStart=false;
                fHome->iHomeStep=1;
                SoftStop=true;
                return false;
            }
        }
    }
    return true;
}
//==============================================================================
bool  CylinderInitState[CynForHome];                                            //true : Push(), false : Pop()
bool  CylinderInitOKFlag[CynForHome];                                           //true : 不需要復歸, false : 需要復歸
bool  CylinderInitSwitchType[CynForHome];                                       //RogerYang 20250909 : true: Push/Pop  false On/Off
//------------------------------------------------------------------------------
void SetCylinderResetStateHomeBegin()
{
    for(int i=0; i<CynForHome; i++)
    {
        CylinderInitSwitchType[i]=true;                                         //RogerYang 20250909 : true: Push/Pop  false On/Off

        if(LOAD_Z_USE_MOTOR[0]==true &&
           (CynNeedHome[i]==C_Load_Up ||
            CynNeedHome[i]==C_Load_Middle))                                     //JerryYang 20191017 修正loader Z馬達版回home問題
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[1]==true &&
                (CynNeedHome[i]==C_Empty_Up ||
                 CynNeedHome[i]==C_Empty_Middle))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[2]==true &&
                (CynNeedHome[i]==C_Color_Up ||
                 CynNeedHome[i]==C_Color_Middle))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[3]==true &&
                (CynNeedHome[i]==C_Auto1_Up ||
                 CynNeedHome[i]==C_Auto1_Selector))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[4]==true &&
                (CynNeedHome[i]==C_Auto2_Up ||
                 CynNeedHome[i]==C_Auto2_Selector))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[5]==true &&
                (CynNeedHome[i]==C_Auto3_Up ||
                 CynNeedHome[i]==C_Auto3_Selector))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[6]==true &&
                (CynNeedHome[i]==C_Auto4_Up ||
                 CynNeedHome[i]==C_Auto4_Selector))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[7]==true &&
                (CynNeedHome[i]==C_Auto5_Up ||
                 CynNeedHome[i]==C_Auto5_Selector))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(LOAD_Z_USE_MOTOR[8]==true &&
                (CynNeedHome[i]==C_Auto6_Up ||
                 CynNeedHome[i]==C_Auto6_Selector))
        {
            CylinderInitOKFlag[i]=true;
        }
        else if(AUTO3_IS_MAGAZINE==1 &&                                         //JerryYang 20220909 : add magazine
               (CynNeedHome[i]==C_Auto3_Selector  ||
                CynNeedHome[i]==C_Auto3Side_Fixer ||
                CynNeedHome[i]==C_Auto3_Up        ||
                CynNeedHome[i]==C_Auto3LoaderZ_Select))
        {
            CylinderInitOKFlag[i]=true;
        }
        else
        {
            if(Cylinder[CynNeedHome[i]].Enable==true)                           //Steven 20240123 : 改用enable確認氣缸是否要復歸
            {
                CylinderInitOKFlag[i]=false;
            }
            else
            {
                CylinderInitOKFlag[i]=true;
            }
        }

        if(CynNeedHome[i]==C_Auto1Separate  ||
           CynNeedHome[i]==C_Auto2Separate  ||                                  //Steven 20240131 : 歸零的時候要常態Push
           CynNeedHome[i]==C_Auto3Separate  ||                                  //ChungHung 20140701 add Auto Retest
           CynNeedHome[i]==C_LoadCarRFIDRotArmU ||                              //RogerYang 20250828 add for Loader Rotate Arm
           CynNeedHome[i]==C_LoadTrayDetU       ||                              //RogerYang 20250828 add for 殘料檢氣缸
           CynNeedHome[i]==C_LoadTrayDetB       ||                              //RogerYang 20250828 add for 殘料檢氣缸
           (Cylinder[C_TrayXFloodgate1].Enable==true && CynNeedHome[i]==C_TrayXFloodgate1) ||                           //Ztex 2024.04.03 Add Ready Home
           (Cylinder[C_TrayXFloodgate2].Enable==true && CynNeedHome[i]==C_TrayXFloodgate2) ||
           (Cylinder[C_TrayXFloodgate3].Enable==true && CynNeedHome[i]==C_TrayXFloodgate3) ||
           (Cylinder[C_TrayXFloodgate4].Enable==true && CynNeedHome[i]==C_TrayXFloodgate4))
        {
            CylinderInitState[i]=true;                                          //true : Push()
        }
        else
        {
            if(CynNeedHome[i]==C_LoadCarRFIDRotArmD  ||                         //RogerYang 20250909 : true: Push/Pop  false On/Off
               CynNeedHome[i]==C_LoadTrayDetD        ||
               CynNeedHome[i]==C_LoadTrayDetF)
            {
                CylinderInitSwitchType[i]=false;
            }
            CylinderInitState[i]=false;                                         //false : Pop()
        }

        if(MachineTypeChoice==Type_HT1032)                                      //RogerYang 20250523 會影響到9045的AutoZ氣缸(push會往上升)
        {
            if(IsSafePLCIOInstall()==1 &&                                       //Ztex 2024.05.07 Add PLC Safe Door
              (CynNeedHome[i]==C_SafeDoor1Lock ||
               CynNeedHome[i]==C_SafeDoor2Lock ||
               CynNeedHome[i]==C_SafeDoor3Lock ||
               CynNeedHome[i]==C_SafeDoor4Lock ||
               CynNeedHome[i]==C_SafeDoor6Lock ||
               CynNeedHome[i]==C_SafeDoor7Lock ||
               CynNeedHome[i]==C_SafeDoor8Lock
              ))
            {
                CylinderInitState[i]=false;                                     //false : Pop()
            }
            else
            {
                CylinderInitState[i]=true;                                      //true : Push()
            }
        }

        if(CynNeedHome[i]==C_TrayZ_Selector      ||
           CynNeedHome[i]==C_EmptyLoaderZ_Select ||
           CynNeedHome[i]==C_ColorLoaderZ_Select ||
           CynNeedHome[i]==C_LoaderSeparate      ||
           CynNeedHome[i]==C_EmptySeparate       ||
           CynNeedHome[i]==C_ColorSeparate       ||
           CynNeedHome[i]==C_Auto1Separate       ||
           CynNeedHome[i]==C_Auto2Separate       ||
           CynNeedHome[i]==C_Auto3Separate       ||
           CynNeedHome[i]==C_LoaderEdgePush      ||
           CynNeedHome[i]==C_EmptyEdgePush       ||
           CynNeedHome[i]==C_ColorEdgePush       ||
           CynNeedHome[i]==C_Auto1EdgePush       ||
           CynNeedHome[i]==C_Auto2EdgePush       ||
           CynNeedHome[i]==C_Auto3EdgePush       ||
           CynNeedHome[i]==C_Auto1_Selector      ||
           CynNeedHome[i]==C_Auto2_Selector      ||
           CynNeedHome[i]==C_Auto3_Selector
          )
        {
            CylinderInitOKFlag[i]=true;
        }
    }
}
//---------------------------------------------------------------------------
//void SetCylinderResetStateHomeAfter()                                         //Steven 20240123 : 完全不會執行, mark
//{
//    for(int i=0; i<CynForHome; i++)
//        CylinderInitOKFlag[i]=true;
//}
//------------------------------------------------------------------------------
TQPF_Timer ResetOKDeleyTime;
//void WritePanasonicParameter(int Index, unsigned char Command, unsigned Data);
//void ReadPanasonicParameter(int index, unsigned char Command);
void SetHomeSerial(int i)
{
    char str[256];
    fHome->ShowLed(i, 3);
    sprintf(str, "M%2d homeing ....", i+1);
    fHome->ListBox1->Items->Insert(0, str);
}
//------------------------------------------------------------------------------
TQPF_Timer CynPushDeleyTime[MAX_TRACK];
int PrePushLifterCylinderTask[MAX_TRACK]={1};
bool bLoaderUpStatus[MAX_TRACK]={false};
//------------------------------------------------------------------------------
bool PrePushLifterCylinder(int index)                                           //Steven 20240109 : 預先打兩下汽缸
{
    bool ret=false;
    int &Task=PrePushLifterCylinderTask[index];

    if(Prod.iTrayType[index]==tNotUse)
        return true;

    switch(Task)
    {
        case 1:
            bLoaderUpStatus[index]=(Cylinder[iC_Up[index]].OnSensor() ||
                                    Cylinder[iC_Middle[index]].OnSensor());
            Task=100;
        case 100:
            if(bLoaderUpStatus[index]==false)
            {
                Cylinder[iC_Up[index]].On();
            }

            Cylinder[iC_EdgePush[index]].On();
            CynPushDeleyTime[index].SetMSAndOn(1000);
            Task=200;
            break;
        case 200:
            if(CynPushDeleyTime[index].Off())
            {
                if(bLoaderUpStatus[index]==false)
                {
                    Cylinder[iC_Up[index]].Off();
                }

                Cylinder[iC_EdgePush[index]].Off();
                CynPushDeleyTime[index].SetMSAndOn(1000);
                Task=300;
            }
            break;
        case 300:
            if(CynPushDeleyTime[index].Off())
            {
                if(bLoaderUpStatus[index]==false)
                {
                    Cylinder[iC_Up[index]].On();
                }

                Cylinder[iC_EdgePush[index]].On();
                CynPushDeleyTime[index].SetMSAndOn(1000);
                Task=400;
            }
            break;
        case 400:
            if(CynPushDeleyTime[index].Off())
            {
                if(bLoaderUpStatus[index]==false)
                {
                    Cylinder[iC_Up[index]].Off();
                }

                Cylinder[iC_EdgePush[index]].Off();
                CynPushDeleyTime[index].SetMSAndOn(1000);
                Task=500;
            }
            break;
        case 500:
            if(CynPushDeleyTime[index].Off())
            {
                ret=true;
            }
            break;
    }
    return ret;
}
//------------------------------------------------------------------------------
bool PrePushLoaderCylinder(bool bReset)                                         //Steven 20150429 : 預先打兩下Loader汽缸
{
    bool ret=true;

    if(bReset)
    {
        for(int i=0; i<MAX_TRACK; i++)
        {
            PrePushLifterCylinderTask[i]=1;
            bLoaderUpStatus[i]=false;
        }
        return ret;
    }

    if(IniConfig.bP05_LoaderCylinderPreOn==false)
    {
        return ret;
    }

    for(int i=0; i<MAX_TRACK; i++)
    {
        if(AUTO_EMPTY_COLOR==0)                                                 //使用四軌
        {
            if(i==etEmpty || i==etColor)
                continue;
        }

        if(PrePushLifterCylinder(i)==false)
            ret=false;
    }

    return ret;
}
//------------------------------------------------------------------------------
int iTopBtmHome=0;
TQPF_Timer tTopBtmDelay;
//------------------------------------------------------------------------------
bool DoTopBtmHome(bool bFirst)                                                  //Jimmychiu 20240322 : Top & Bottom Inspect
{
    if(bFirst==true)
    {
        iTopBtmHome=1;
        return true;
    }
    else
    {
        int &Task=iTopBtmHome;
        switch(Task)
        {
            case 1:  //check safe status
                FrmAOI->ttbInsp->DoLockRotate(true);
                SW[SwCCDZBreaker].On();  //Jimmychiu 20240322 : Top & Bottom Inspect
                if(FrmAOI->GetRotateIs0()==false)
                {
                    ShowMyMessage("Please manually rotate the platform to the origin.");
                    return false;
                }
//                iSingleMotorHomeTask[MTopAOIArmX]=1;
//                iSingleMotorHomeTask[MTopAOIArmY]=1;
//                iSingleMotorHomeTask[MTopAOIArmR]=1;
                iSingleMotorHomeTask[MTopAOICCDZ]=1;
                Task=100;
                break;
            case 100://XYZ home
                {
                    bool bZMove=ProcessSingleMotorHome(MTopAOICCDZ);
                    if(bZMove)
                    {
                        bZMove=false;
                        if(MOT[MTopAOICCDZ].Led[iHomeLed])
                        {
//                            fHome->HomeClass[MTopAOIArmX]->THomeFlag=0;
//                            fHome->HomeClass[MTopAOIArmY]->THomeFlag=0;
//                            fHome->HomeClass[MTopAOIArmR]->THomeFlag=0;
                            fHome->HomeClass[MTopAOICCDZ]->THomeFlag=0;
                            fHome->ShowLed(MTopAOICCDZ, 1);
                            return true;
                            //Task=200;
                        }
                        else
                        {
                            iSingleMotorHomeTask[MTopAOICCDZ]=1;
                            Task=100;
                        }
                    }
                }
                break;
            case 200:     //move Y
                {
                    bool bZMove=ProcessSingleMotorHome(MTopAOIArmY);
                    if(bZMove)
                    {
                        bZMove=false;
                        if(MOT[MTopAOIArmY].Led[iHomeLed])
                        {
                            Task=300;
                        }
                        else
                        {
                            iSingleMotorHomeTask[MTopAOIArmY]=1;
                            Task=200;
                        }
                    }
                }
                break;
            case 300:
                {
                    bool bZMove=ProcessSingleMotorHome(MTopAOIArmX);
                    if(bZMove)
                    {
                        bZMove=false;
                        if(MOT[MTopAOIArmX].Led[iHomeLed])
                        {
                            Task=400;
                        }
                        else
                        {
                            iSingleMotorHomeTask[MTopAOIArmX]=1;
                            Task=300;
                        }
                    }
                }
                break;
            case 400://move XY
                if(FrmAOI->ttbInsp->DoMoveFixedSeatXY2RotatePos())
                {
                    FrmAOI->ttbInsp->DoLockRotate(false);
                    iSingleMotorHomeTask[MTopAOIArmR]=1;
                    Task=500;
                }
                break;
            case 500://rotate home
                {
                    bool bZMove=ProcessSingleMotorHome(MTopAOIArmR);
                    if(bZMove)
                    {
                        bZMove=false;
                        if(MOT[MTopAOIArmR].Motor->Enable==true)
                        {
                            if(MOT[MTopAOIArmR].Led[iHomeLed])
                            {
                                FrmAOI->ttbInsp->DoRotate(0, true);
                                Task=600;
                            }
                            else
                            {
                                ShowMyMessage("AOI rotate not at Home");
                            }
                        }
                        else
                        {
                            Task=900;
                        }
                    }
                }
                break;
            case 600:  //rotate 0
                if(FrmAOI->ttbInsp->DoRotate(0))
                {
                    Task=900;
                    FrmAOI->ttbInsp->DoLockRotate(true);
                }
                break;
            case 900:  //finish
                fHome->HomeClass[MTopAOIArmX]->THomeFlag=0;
                fHome->HomeClass[MTopAOIArmY]->THomeFlag=0;
                fHome->HomeClass[MTopAOIArmR]->THomeFlag=0;
                fHome->HomeClass[MTopAOICCDZ]->THomeFlag=0;
                SW[SwLightOrg].OnOff(true);
                tTopBtmDelay.SetSecAndOn(20);                                   //Eastsun 20260302 : 新增Time out
                Task=1000;
                break;
            case 1000:
                if(USE_Scanner_AOI_Inspection_FixLight_Z_Axis==1)               //Eastsun 20260410 : AOI Fix Light Z Axis bypass
                    return true;
                #ifdef SOFT_SIMULTE
                return true;
                #endif                                                    //無電動缸直接Pass

                if(Sen[SnLightZINP].IsOff())
                {
                    SW[SwLightOrg].OnOff(false);
                    tTopBtmDelay.SetSecAndOn(20);                               //Eastsun 20260302 : 新增Time out
                    Task=1100;
                }

                if(tTopBtmDelay.Off())                                          //Eastsun 20260302 : 新增Time out
                {
                    ShowMyMessage("Sensor SnLightZINP Off TimeOut");
                }
                break;
            case 1100:
                if(Sen[SnLightZORG].IsOff()==false)
                {
                    SW[SwLightOrg].OnOff(true);
                    return true;
                }

                 if(tTopBtmDelay.Off())                                          //Eastsun 20260302 : 新增Time out
                {
                    ShowMyMessage("Sensor SnLightZORG Off TimeOut");
                }
                break;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
extern bool bPlaceToShuttleFirst;
extern bool bPickFromHotplate;
extern bool MoveOutArmXY_ToFix_Tray_Full(bool bMoveY);
TQPF_Timer hRealCCDTimeOut;
TQPF_Timer hHomeDelay;
bool ProcessMotorHome(bool Flag2)
{
#ifndef SOFT_SIMULTE
    int iTrayArmPos;
#endif
    int ibuffer=0;
    int ret, iRef, iCT;
    int iPos = 33736;
    int iPitch_Move[4]={0,0,0,0};                                               //Ztex 2023.12.15 Add Pitch X Home Twice
    int Ang45=0, Ang90=0, buffer=0;                                             //kevin 20131003 讀取馬達轉90度pluse
    char str[256];
    bool bSensor=false;
    bool bATCSiteTest[32];                                                      //Ifor 20160510 add ATC Test Site
    AnsiString sBuffer;
    AnsiString strxxx="", Str;

    static int iInRotateCount=1;
    static int iOutRotateCount=1;
    static int Index_Z1_FindZPosition=0;                                        //Isaac 20201110 : Index Y find motor phase
    static int Index_Z2_FindZPosition=0;
    static int Index_Y1_FindZPosition=0;
    static int Index_Y2_FindZPosition=0;
    static bool flag1, flag2, flag3, flag4, flag5, flag6, flag[MAX_ARM_Row][MAX_ARM_Col], flag11;
    static bool flag7 ,flag8;                                                   //2013-04-12    Dell :旋轉站;馬達版
    static bool flag9, flag10;                                                  //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    static bool flag12, flag13;                                                 //Isaac 20201110 : Index Y find motor phase
    static bool flag14[4]={false, false, false, false};                         //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    static bool bCyflag[4];
    static bool bCheckInArmX=false;
    static bool bNeedPutBack=false;
    static bool IndexY1=false, IndexZ1=false, IndexY2=false, IndexZ2=false;     //kevin 20161214 (Steven) Increase Hone Speed
    static bool bflag[MAX_AUTO_TRAY]={false, false, false, false, false, false};                                        //kevin 20180726 Auto 12 3 up
    static bool bFindMotorPhaseEveryGoHomeProcess=false;
    static bool bPitchHome_Twice=false;                                         //Ztex 2023.12.15 Add Pitch X Home Twice
    static bool bPitchHome[2]={false,false};                                    //Ztex 2023.12.15 Add Pitch X Home Twice
    static bool bOCRHomeActionPass=false;                                       //Frank 20250214 add
    static bool bIsYCarMoving=false;                                            //RogerYang 20251029 : 修正Loader Y motor判斷有無tray可能被機構誤觸發
    static bool flag18[3]={false, false, false};                                //Ifor 20251220 add:Boat Z
    if(fHome->fAbort)
    {
        fHome->fAbort=false;
        SoftStop=true;
        fHome->iHomeStep=1;
        HomeLog("fHome->fAbort");                                               //Kevin  20110525
        return true;
    }
    try
    {
        sBuffer.sprintf("%d", fHome->iHomeStep);
        HomeLog(sBuffer);                                                       //Kevin  20110525
    }
    catch(...)
    {
        MyDBIProcess("Exception", "ProcessMotorHome");
        fMain->MemoHome->Lines->Add("fHome->iHomeStep != int");
    }
    #ifdef DEBUG_HOME                                                           //wei (Steven) 20170317 快速回Home開關

    if(fHome->iHomeStep>=650 &&                                                 //kevin 20161214 (Steven) Increase Hone Speed
       fHome->iHomeStep<=1250 &&
       (IndexY1==false || IndexZ1==false || IndexY2==false || IndexZ2==false))
    {
        if(IndexY1==false)
        {
            if(MOT[MTestY1].Gali_SingalHome())
            {
                IndexY1=true;
                fHome->ListBox1->Items->Insert(0, "Index Y1 home finish.");
            }
        }

        if(IndexY2==false)
        {
            if(MOT[MTestY2].Gali_SingalHome())
            {
                IndexY2=true;
                fHome->ListBox1->Items->Insert(0, "Index Y2 home finish.");
            }
        }

        if(IndexZ1==false)
        {
            if(MOT[MTestZ1].Gali_SingalHome())
            {
                IndexZ1=true;
                fHome->ListBox1->Items->Insert(0, "Index Z1 home finish.");
            }
        }

        if(IndexZ2==false)
        {
            if(MOT[MTestZ2].Gali_SingalHome())
            {
                IndexZ2=true;
                fHome->ListBox1->Items->Insert(0, "Index Z2 home finish.");
            }
        }
    }
    #endif

    if(IniConfig.bP35TrayArm && bPushHomeDetect)                                //kevin 20171006 (wei) tray arm home 需遮住sensor
    {
        if(Sen[SnTrayArmSafePos].IsOff())
        {
            ShowMyMessage("Please move tray arm on safe sensor");
            return false;
        }
    }
    bPushHomeDetect=false;

    switch(fHome->iHomeStep)
    {
        case 1:
            iReadCIDAction=ePortTotal;
            iMoveToShuttle=0;
            bIsYCarMoving=false;                                                //RogerYang 20251029 : 修正Loader Y motor判斷有無tray可能被機構誤觸發

            for(int i=0; i<TOTAL_MOTOR; i++)                                    //JerryYang 20250429 : 進入HOME流程強制清除home旗標, 避免沒做完home又按start會接著繼續跑
            {
                MOT[i].MotorInitial();
                MOT[i].HomeFlag=0;
                fHome->HomeClass[i]->THomeFlag=1;
                if(MOT[i].Motor->Enable==false)
                {
                    fHome->HomeClass[i]->THomeFlag=0;
                    MOT[i].HomeFlag=1;
                }
            }

            #ifndef SOFT_SIMULTE
            if(CosFunction.bUSEJCETSiteMapMode &&
               IniConfig.bI21EnableASM==true &&
               InArmSiteMapData.iP!=-1)                                         //Steven 20211209 : 紀錄Site map資料
            {
                Str.sprintf("%d, %d, %d, %d, %s, ASM_Home", InArmSiteMapData.iP, InArmSiteMapData.iPlateR, InArmSiteMapData.iPlateC, InArmSiteMapData.iSuckR, InArmSuck.Suck[InArmSiteMapData.iSuckR][InArmSiteMapData.iSuckC].sName);
                fMain->slAutoSiteMapLog->AddTextWithDateTime(Str);              //Steven 20211209 : 紀錄Auto Site Map動作
                RecordProcess(Str);
                fMain->DoStateRecord(0, false);                                 //KenHsieh 20230116 : 區分手動或自動(sbclick -> Function)
            }
            #endif

            if(SW[SwAirOff].Enable==true)                                       //Steven 20221215 : Power saving for vacuum pump
                SW[SwAirOff].On();

            HPPlaceLog.InitialPosition();                                       //Steven 20211110 : 記錄放料到加熱盤的位置
            bUnloading=false;                                                   //JerryYang 20250227 : fix
            if(CheckNozzleEventFinish()==false)                                 //Steven 20210825 : 吹氣完成才可以歸零
            {
                return false;
            }

            bSuckingFlagZ1=true;                                                //Steven 20240916 : index下降到shuttle吸放料
            bSuckingFlagZ2=true;
            InitDoInArmAdditionalFunction();
            InitDoOutArmAdditionalFunction();
            dmTrayMotor->StartSetSpeed();                                       //Steven 20210318 : Loader入Tray改步進, 換位置寫入速度, 避免動作異常
            bCatchTrayFinishAction=false;                                       //JerryYang 20241118 : fix
            InitialIndexSocketCheckTask();
            bNeedPutBack=false;
            bDoingF16=false;                                                    //Steven 20221213 : 確認shuttle 有沒有斷線

            if(asMotorDatabaseErr!="")                                          //jou 20180814 (Steven) : 增加Motor database 異常警示
            {
                ShowMyMessage(asMotorDatabaseErr);
                return false;
            }

            if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)                      //Ifor 20181127 : add RTC TIME SYNC
            {
                COM2->sRealTimeCom_Send[COM2->rtTIMESYNC].sprintf("@TIMESYNC:,%s",FormatDateTime("MM/DD/YYYY HH:NN:SS AM/PM", Now())+ "+");                     // RTC僅收@到+的資料，"+"必需放在資料最後面
                COM2->SendCommToVision(COM2->rtTIMESYNC, true);
            }

            Cylinder[C_TrayZ_Selector     ].Off();                              //Steven 20240123 : 歸零前先把分離氣缸關起來
            Cylinder[C_EmptyLoaderZ_Select].Off();
            Cylinder[C_ColorLoaderZ_Select].Off();
            for(int i=0; i<MAX_AUTO_TRAY; i++)
            {
                Cylinder[C_AutoZ_Select[i]].Off();
            }

            for(int i=0; i<4; i++)
                Temperature.iATCCurrentFailCount[i]=0;                          //Steven 20151123 : Continue Fail Temp Offset for ATC

            if(CosFunction.bOutShtLoseICInArmAway)                              //JerryYang 20200519 out shuttle lose IC, in arm要讓位
            {
                if(bSht1LoseICErr)
                {
                    bSht1LoseICErr=false;
                    ShowOutputShuttleICStatus(0);
                    StartDetectMotorSensor(0);                                  //清除
                    b1ShuttleMoveToLeft=false;
                    return false;
                }

                if(bSht2LoseICErr)
                {
                    bSht2LoseICErr=false;
                    ShowOutputShuttleICStatus(1);
                    StartDetectMotorSensor(1);                                  //清除
                    b2ShuttleMoveToLeft=false;
                    return false;
                }
            }

            MoveInRotateToDegreeAtSameTime(0, true);                            //Steven 20170425 (wei) : Add rotate motor
            MoveOutRotateToDegreeAtSameTime(0, true);
            iInRotateCount=1;
            iOutRotateCount=1;

            PrePushLoaderCylinder(true);                                        //Steven 20150429 : 預先打兩下Loader汽缸
            bZ1PickShuttle=false;                                               //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            bZ2PickShuttle=false;                                               //Steven 20150407 : 修正[D45] Out Arm等Index Z功能, 避免Auto Homing
            bZ1Isdownflag=false;                                                //Isaac 20180307 index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖，回home後重置 //Steven 20180522 : true --> false, 解除Hang up
            bZ2Isdownflag=false;                                                //Isaac 20180307 index arm在shuttle放置位子時，shuttle抖抖須等到arm起來才能抖，回home後重置
            fMain->SendMSG_CMD(MSG_CMD_HandlerHomeStart);                       //Steven 20150304 : Add GPIB LOG
            fNote->bMyServoOffInArm=false;                                      //Steven 20110804 : 歸零後要重置狀態
            fNote->bMyServoOffOutArm=false;                                     //Steven 20110804 : 歸零後要重置狀態
            NeedWaitTrayArm        =false;                                      //wei 20151002
            bIndexPickErrShtStayRight1=false;                                   //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
            bIndexPickErrShtStayRight2=false;                                   //JerryYang 20181206 (Steven) : fix 啟用D43功能時,index arm pick up error後按retry可能發生hang up
            bIndexPickUpErrorWaitRetry=false;

            fNote->bMyServoOffOutShuttle1=false;                                //ChungHung 20140522 add OutShuttle lose devices can servo off
            fNote->bMyServoOffOutShuttle2=false;                                //ChungHung 20140522 add OutShuttle lose devices can servo off
            bDoOverDrive           =false;                                      //Steven 20151207 : OverDrive for TSMC
            bDoReContact           =false;                                      //Steven 20151207 : Recontact for TSMC
            bAutoCleaning=false;                                                //Isaac 20171006 (wei) ： AMKOR_Philippines machine status Bit16 是否在做autoclean
            bNeedWaitRTCAutoVerify =false;                                      //jou 2014-06-24 RTC 自動進行Model驗證
            bRTCAutoVerifyControlEP=false;                                      //jou 2014-06-24 RTC 自動進行Model驗證
            bWaitOutArmCheckCylin=false;                                        //jou 20240131 : 修正out arm 與 auto tray互卡衝突hang up
            bIsPlacingToBuffer=false;                                           //JerryYang 20250828 : fix color誤退TRAY
            bIsCatchingFromBuffer=false;                                        //RogerYang 20260225 : JSCC防止夾tray的時候Color/Empty誤退，驗證中

            if(bShuttle1Pause)                                                  //Sam 20250520 : 修正 D42 功能觸發時按 Home 死機
            {
                sBuffer.printf("D42 function is stopped by HOME action shuttle1");
                MyDBIProcess("Message", sBuffer);
                bShuttle1Pause=false;
            }

            if(bShuttle2Pause)
            {
                sBuffer.printf("D42 function is stopped by HOME action shuttle2");
                MyDBIProcess("Message", sBuffer);
                bShuttle2Pause=false;
            }

            bArm1NeedSuck=false;                                                //JerryYang 20190123 新增保護避免真空持續on會造成all site掉料
            bArm2NeedSuck=false;
            bD44Arm1CheckVacOn=false;
            bD44Arm2CheckVacOn=false;
            iIndexTakeDeviceChk1=0;                                             //JerryYang 20190325 fix回home後hang up
            iIndexTakeDeviceChk2=0;

            MyDBIProcessNew("Motion", "WAR2207", "Do process motor home start.");                                       //jou 2010-11-23
            bMoveInArm2XYToWaitOk=true;                                         //Steven 20131025 : 要先動到安全位置才可以再到別的地方
            bIndexDropVacuumError=false;                                        //kevin 20190418 避免 inarm 來回跑
            bArm1Delay=false;                                                   //JerryYang 20200903 : 修正ARM 1預熱ARM2測試功能流程
            bArm2Delay=false;
            bHalfViewVerifyNeedAboveSocket=false;
            DoSetupSystemToProd();                                              //JerryYang 20220215 : RTC Auto Verify half view check

            bTJControlMode=false;                                               //Ifor 20230531 add:回Home 清除TJ Mode旗標
            bChangeTest_TempAlarm=true;                                         //Ifor 20230505 add: flag=true & Offset=0 重設溫度
            bChangeTest_TempOffset=0;                                           //Ifor 20230505 add:清除資料
            bTriggerRTC_AutoSTD=false;                                          //JerryYang 20240829 : RTC Auto STD
            fLotInfo->cbRTCASTD->Enabled=true;

            bInArmHasHotIC=false;                                               //Sam 20211012 : Debug 用
            bLoadNewEmptyTrayToCarStart=false;                                  //Sam 20211119 : 放 Tray 增加保護
            bUnLoadNewEmptyToStackStart=false;                                  //Sam 20211119 : 放 Tray 增加保護
            bLoadNewColorTrayToCarStart=false;                                  //Sam 20211119 : 放 Tray 增加保護
            bUnLoadNewColorToStackStart=false;                                  //Sam 20211119 : 放 Tray 增加保護

            bAutoCleanShuttle1MoveToLeft=false;                                 //Sam 20250728 : 避免 AutoClean Drop Alarm 未完成時 Home Hange up
            bAutoCleanShuttle1HasPickErr=false;
            bAutoCleanShuttle2MoveToLeft=false;
            bAutoCleanShuttle2HasPickErr=false;

            if(LastSet.iRealDummy==REALLY)                                      //JerryYang 20170815 (Steven) In arm A吸嘴上IC大於36mm時X軸回homeIC會撞到基座,要求強制將IC取下才能回home
            {
                if(DeviceForm.XDimension>=3600 || UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1)
                {
                    if(USE_PICKER_COUNT==ep1Picker)                             //Ifor 20251106 add:單吸嘴模組Out Arm 回home IC會撞到強制將IC取下才能回home
                    {
                        if(InArmSuck.Suck[0][0].GetStatus()==true)
                        {
                            ShowMyMessage("Please take off the device of in arm Vacuum A");
                            if(CUSTOMER_CODE==CC_ASE_SG)
                                InArmSuck.SetItemData(0, 0, NULL_IC);
                            return false;
                        }
                    }
                    else
                    {
                        if(InArmSuck.Suck[0][0].GetStatus()==true)
                        {
                            ShowMyMessage("Please take off the device of in arm Vacuum A");
                            return false;
                        }

                        if(InArmSuck.Suck[1][0].GetStatus()==true)
                        {
                            ShowMyMessage("Please take off the device of in arm Vacuum B");
                            return false;
                        }

                        if(CUSTOMER_CODE==CC_ASE_SG)                            //Ifor 20251120 add:ASE SG 要求InArm第二支吸嘴需要取走
                        {
                            InArmSuck.SetItemData(0, 0, NULL_IC);               //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                            InArmSuck.SetItemData(1, 0, NULL_IC);               //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE

                            if(InArmSuck.Suck[0][1].GetStatus()==true)
                            {
                                ShowMyMessage("Please take off the device of in arm Vacuum C");

                                return false;
                            }

                            if(InArmSuck.Suck[1][1].GetStatus()==true)
                            {
                                ShowMyMessage("Please take off the device of in arm Vacuum D");

                                return false;
                            }
                            InArmSuck.SetItemData(0, 1, NULL_IC);               //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                            InArmSuck.SetItemData(1, 1, NULL_IC);               //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE

                            if(bUseAxExPicker()==true)
                            {
                                if(InArmSuck.Suck[0][2].GetStatus()==true)
                                {
                                    ShowMyMessage("Please take off the device of in arm Vacuum E");
                                    return false;
                                }

                                if(InArmSuck.Suck[1][2].GetStatus()==true)
                                {
                                    ShowMyMessage("Please take off the device of in arm Vacuum F");

                                    return false;
                                }
                                InArmSuck.SetItemData(0, 2, NULL_IC);           //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                                InArmSuck.SetItemData(1, 2, NULL_IC);           //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                            }
                            else if(bUseAxxGPicker()==true)
                            {
                                if(InArmSuck.Suck[0][3].GetStatus()==true)
                                {
                                    ShowMyMessage("Please take off the device of in arm Vacuum G");
                                    return false;
                                }

                                if(InArmSuck.Suck[1][3].GetStatus()==true)
                                {
                                    ShowMyMessage("Please take off the device of in arm Vacuum H");
                                    return false;
                                }
                                InArmSuck.SetItemData(0, 3, NULL_IC);           //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                                InArmSuck.SetItemData(1, 3, NULL_IC);           //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                            }
                        }
                    }
                }

                if(DeviceForm.XDimension>=4000 || UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1)
                {
                    if(USE_PICKER_COUNT==ep1Picker)                             //Ifor 20251106 add:單吸嘴模組Out Arm 回home IC會撞到強制將IC取下才能回home
                    {
                        if(OutArmSuck.Suck[0][0].GetStatus()==true)
                        {
                            ShowMyMessage("Please take off the device of out arm Vacuum A");
                            if(CUSTOMER_CODE==CC_ASE_SG)
                                OutArmSuck.SetItemData(0, 0, NULL_IC);
                            return false;
                        }
                    }
                    else
                    {
                        if(CUSTOMER_CODE==CC_ASE_SG)                            //Ifor 20251120 add:ASE SG 要求OutArm第三支吸嘴需要取走
                        {
                            if(TestIF_File.iTestMode==SingleSite)
                            {
                                if(OutArmSuck.Suck[0][0].GetStatus()==true)
                                {
                                    if(TestIF_File.bSingleUseOtherSuck==true)
                                        ShowMyMessage("Please take off the device of out arm Vacuum E");                //Ifor 20200108 Fix: In Arm => Out Arm
                                    else
                                        ShowMyMessage("Please take off the device of out arm Vacuum G");                //Ifor 20200108 Fix: In Arm => Out Arm
                                    return false;
                                }

                                if(OutArmSuck.Suck[1][0].GetStatus()==true)
                                {
                                    ShowMyMessage("Please take off the device of out arm Vacuum H");
                                    return false;
                                }
                                OutArmSuck.SetItemData(0, 0, NULL_IC);          //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                                OutArmSuck.SetItemData(1, 0, NULL_IC);          //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                            }
                            else if(TestIF_File.iTestMode==DualSite)
                            {
                                if(OutArmSuck.Suck[0][0].GetStatus()==true)
                                {
                                    ShowMyMessage("Please take off the device of out arm Vacuum A");                    //Ifor 20200108 Fix: In Arm => Out Arm
                                    return false;
                                }

                                if(OutArmSuck.Suck[1][0].GetStatus()==true)
                                {
                                    ShowMyMessage("Please take off the device of out arm Vacuum B");
                                    return false;
                                }
                                OutArmSuck.SetItemData(0, 0, NULL_IC);          //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                                OutArmSuck.SetItemData(1, 0, NULL_IC);          //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE

                                if(bUseAxExPicker()==true)
                                {
                                    if(OutArmSuck.Suck[0][1].GetStatus()==true)
                                    {
                                        ShowMyMessage("Please take off the device of out arm Vacuum E");                //Ifor 20200108 Fix: In Arm => Out Arm
                                        return false;
                                    }

                                    if(OutArmSuck.Suck[1][1].GetStatus()==true)
                                    {
                                        ShowMyMessage("Please take off the device of out arm Vacuum F");
                                        return false;
                                    }
                                }
                                else if(bUseAxxGPicker()==true)
                                {
                                   if(OutArmSuck.Suck[0][1].GetStatus()==true)
                                    {
                                        ShowMyMessage("Please take off the device of out arm Vacuum G");                //Ifor 20200108 Fix: In Arm => Out Arm
                                        return false;
                                    }

                                    if(OutArmSuck.Suck[1][1].GetStatus()==true)
                                    {
                                        ShowMyMessage("Please take off the device of out arm Vacuum H");
                                        return false;
                                    }
                                }
                                OutArmSuck.SetItemData(0, 1, NULL_IC);          //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                                OutArmSuck.SetItemData(1, 1, NULL_IC);          //Ifor 20250523 add:避免IC拔掉後報DROP的JAM CODE
                            }
                        }
                        else
                        {
                            if(OutArmSuck.Suck[0][3].GetStatus()==true)
                            {
                                ShowMyMessage("Please take off the device of out arm Vacuum G");                        //Ifor 20200108 Fix: In Arm => Out Arm
                                return false;
                            }

                            if(OutArmSuck.Suck[1][3].GetStatus()==true)
                            {
                                ShowMyMessage("Please take off the device of out arm Vacuum H");
                                return false;
                            }
                        }
                    }
                }
            }

            if(USE_LdUldCassetteMode==1)
            {
                if(Sen[SnLoaderBoatActDetect].IsOn())
                {
                    ShowErrorMessage("MES0921", 0, MMTrayZ);
                    return false;
                }

                if(Sen[SnAuto1BoatActDetect].IsOn())
                {
                    ShowErrorMessage("WAR1116", 0, MAuto1Z);
                    return false;
                }

                if(Sen[SnAuto2BoatActDetect].IsOn())
                {
                    ShowErrorMessage("WAR1216", 0, MAuto2Z);
                    return false;
                }
            }

            if(LastSet.iRealDummy!=DUMMY)
            {
                if(IniConfig.bP37bAutoCylinderUP)                               //kevin 20200504 add for Auto 123 氣缸常態在上
                {
                    for(int i=eAuto1; i<=iAutoRight; i++)
                    {
                        int iAuto=iAutoIndex[i];
                        if(LastSet.iRealDummy!=DUMMY && MOT[iMMAuto_Car[i]].fHasTray)
                        {
                            if(IniConfig.bP37bAutoCylinderUP)                   //kevin 20180726 (wei) Auto 123 氣缸常態在上
                            {
                                Cylinder[C_Auto_Up[iAuto]].Off();
                                Cylinder[C_Auto_Selector[iAuto]].Off();         //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
                                ShowMyMessage("Please take out all trays from Auto-unloader #1,#2,#3 ");
                            }

                            if(Sen[SnAutoTrayCar[iAuto]].IsOff())
                                MOT[iMMAuto_Car[i]].fHasTray=false;
                        }
                    }
                }
            }

            if(TestIF_File.bUseSLKClamp==true)                                  //JerryYang 20160526 Socket clamp跟SKL Clamp都夾著時要把socket clamp放開
            {
                if(Sen[SnSocketClampPull1].IsOff() || Sen[SnSocketClampPull2].IsOff() ||
                   Sen[SnSocketClampPush1].IsOn()  || Sen[SnSocketClampPush2].IsOn()  )
                {
                    ShowMyMessage("Socket clamp must unclamp !!", "Socket clamp必須打開才能回Home");
                    fHome->fAbort=false;
                    SystemStart=false;
                    fHome->iHomeStep=1;
                    SoftStop=true;
                    return true;
                }
            }

            if(AUTO3_IS_MAGAZINE==1)                                            //JerryYang 20220909 : add magazine
            {
                if(bChaneMagTrayflag || bMagGetNewTrayflag)
                {
                    if(Sen[SnAuto3TrayDetect].IsOn() ||
                       Sen[SnMagazineTrackDetect].IsOn() ||
                       Sen[SnMagazineTrackDetect2].IsOn())                      //Sam 20221116 : Magazine TrayArm 自動補 Tray)
                    {
                        ShowMyMessage("Please remove the tray from AUTO3, check SnAuto3TrayDetect/SnMagazineTrackDetect/SnMagazineTrackDetect2");
                        return false;
                    }

                    if(iMagChangeStep==1)                                       //表示正在從AUTO3夾TRAY移動到Magazine中
                    {
                        if(MOT[MMAuto3_Car].fHasTray && MOT[MMAuto3].fHasTray==true)                                    //正在AUTO3夾到Magazine
                        {
                            iMagChangeStep=0;
                            Str.sprintf("Please remove the tray on Magazine slot%d and AUTO3", iAuto3MagazineIndex+1);
                            ShowMyMessage(Str);
                            iAuto3MagazineIndex=-1;
                            iWhichMag=-1;
                            MOT[MMAuto3].ClearTray(__FUNC__);
                            MOT[MMAuto3_Car].fHasTray=false;
                        }
                    }
                    else if(iMagChangeStep==2)                                  //表示正在從Magazine夾TRAY移動到AUTO3中
                    {
                        if(MOT[MMAuto3].fHasTray==false && MOT[MMAuto3_Car].fHasTray==true && MOT[iMMgzTray[iWhichMag]].fHasTray==true)
                        {
                            iMagChangeStep=0;
                            MOT[iMMgzTray[iWhichMag]].ClearTray(__FUNC__);
                            Str.sprintf("Please remove the tray on Magazine slot%d and AUTO3", iWhichMag+1);
                            ShowMyMessage(Str);
                            iWhichMag=-1;
                            iAuto3MagazineIndex=-1;
                        }
                    }
                    bChaneMagTrayflag=false;
                    iWhichMag=-1;
                    iAuto3MagazineIndex=-1;
                }
            }

            if(INSTALL_OCR_YMot==eocrYMotInstal)                                //Frank 20250214 add
            {
                if(CUSTOMER_CODE!=CC_ARDENTEC &&                                //AI(ht9045-v899) 20260504: ARDENTEC OCR YMot Home Check bypass, no mechanical interference
                   MOT[MMTrayY].fHasTray && Sen[SnLoaderSureTray].IsOn() && Cylinder[C_Load_Middle].OffStatus()==false)
                {
                    ShowMyMessage("Loader cylinder up , can not do homing!");
                }
            }

            if(AUTO3_IS_MAGAZINE==1)                                            //pig 2016.06.29 MgzTrayCatchCynHome end
            {                                                                   // 2011.01.21 , Joye , Auto 3 Magazine {
                Cylinder[C_Auto3EdgePush].Off();                                //kevin 20140603
                Cylinder[C_CatchMagazineTray].Off();                            //kevin 20140603
                if(Sen[SnMagazineTrackDetect].Enable &&
                   Sen[SnMagazineTrackDetect].IsOn()==true)                     //Sam 20221116 : Magazine TrayArm 自動補 Tray
                {                                                               // Alarm : Track has Tray , Magazine Can not Home
                    ShowMyMessage("SnMagazineTrackDetect is on, can not do homing!");

                    fHome->iHomeStep    = 1;
                    fHome->fAbort       = false;
                    SystemStart         = false;
                    SoftStop            = true;
                    return true;
                }

                if(Sen[SnMagazineTrackDetect2].Enable && Sen[SnMagazineTrackDetect2].IsOn()==true)
                {                                                               // Alarm : Track has Tray , Magazine Can not Home
                    ShowMyMessage("SnMagazineTrackDetect2 is on, can not do homing!");

                    fHome->iHomeStep    = 1;
                    fHome->fAbort       = false;
                    SystemStart         = false;
                    SoftStop            = true;
                    return true;
                }
            }

            bATC_SITE_2ND_CHECK[0]=false;                                       //Ifor 20160509 add ATC 測試時開啟第二點溫度監控
            bATC_SITE_2ND_CHECK[1]=false;
            if(ATC_SYSTEM==eNewATCSystem)
            {
                for(int i=0; i<iATC_Use_Heat_Count; i++)                        //Ifor 20160516 add Index 測試前開啟 ATC 第二點溫度偵測
                    bATCSiteTest[i]=false;                                      //Ifor 20160516 修改ATC Heat 設定數
                ATC_InterfaceForm->SiteTesting(iATC_Use_Heat_Count, bATCSiteTest);
            }
            else if(ATC_SYSTEM==eATCHonPrecType)
            {
                ATCInterfaceForm->SendTestEnd(0);
                ATCInterfaceForm->SendTestEnd(1);
            }

            IsTest=false;                                                       //JerryYang 20160318 增加防護 回Home時要重置GPIB
            fMain->RunTestProgram(false);
            Close_ADAM_6024();

            if(Open_ADAM_6024()==false &&                                       //Ifor 20150709 :開啟CKD FCM
               LastSet.iRealDummy==REALLY)                                      //Jimmychiu 20230804 : 整合全部連線檢查
            {
                ShowMyMessage("Adam Connect Error and Stop Home");
                return false;
            }

            if(MACHINE_HAS_AUTO_ALIGNMENT_CCD)                                  //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
            {
                fAutoAlignment->TcpipOpen();
                if(CUSTOMER_CODE!=CC_ASE_KaohSiung)
                {
                    fAutoAlignment->ChangeFileName(fMain->cbSetupFileName->Text);
                }
                else
                {
                    fAutoAlignment->ChangeFileName(TestIF_File.AutoAlignmentFileName);                                  //Kenhsieh 20211122 : 修改無法讀取fileName問題
                    fAutoAlignment->InArmSendCommand(LOAD_FILE);                //Kenhsieh 20211006 : 回Home時更新Alignment工作檔
                }
            }

            if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)                      //----- by dell ccd realtime-------------
            {
                fLotInfo->RTCChangeFile(false);                                 //Steven 20110826 : Real Time CCD - 初始化工作檔
                COM2->InitRealTimeCCDPara();
                hRealCCDTimeOut.SetSecAndOn(10);                                //JerryYang 20250602 : 2sec to 10sec  //ChungHung 20140514 fix in homeing and contact make time out
            }

            if(INSTALL_OCR!=eocrUninstal && TestIF.bOcrFunction)                //Steven 20120716 : OCR
            {
                fOCR->SetOcrFileName(fMain->cbSetupFileName->Text);
                fOCR->InitialOCRPara();                                         //ChungHung 20120830 add OCR Function add

                bOCRHomeActionPass=true;                                        //Frank 20250214 add
            }

            if(IniConfig.bHaveRotateShuttle==true && TestIF.bRotateShuttle)     //kevin 20110531 旋轉SHUTTLE 鎖最高速度
            {
                MOT[MInShuttle1].Motor->PJogHighSpeed   =IniConfig.iPJogHighSpeed;
                MOT[MInShuttle1].Motor->InitSpeed       =IniConfig.iInitSpeed;
                MOT[MInShuttle2].Motor->PJogHighSpeed   =IniConfig.iPJogHighSpeed;
                MOT[MInShuttle2].Motor->InitSpeed       =IniConfig.iInitSpeed;
            }
            else
            {
                MOT[MInShuttle1].Motor->InitSpeed       =iInitSpeedSh1;         //kevin 20110531 旋轉SHUTTLE 鎖最高速度
                MOT[MInShuttle1].Motor->PJogHighSpeed   =iPJogHighSpeedSh1;
                MOT[MInShuttle2].Motor->InitSpeed       =iInitSpeedSh2;
                MOT[MInShuttle2].Motor->PJogHighSpeed   =iPJogHighSpeedSh2;
            }

            if(TestIF_File.iAutoClean_Tray!=eCKPos_HP2 &&                       //Steven 20130620 Start : Auto Clean到一半按歸零要繼續跑
               CUSTOMER_CODE!=CC_ASE_KaohSiung)                                 //kevin 20150806
            {
                if(bRunAutoClean==true)
                {
                    ResetAutoClean();
                    hAutoCleanHangUp.SetSecAndOn(Prod.iHangupMaxTime);
                }

                InitialAutoCleanTask();
                InitialShuttleAutoCleanTask();
                InitialIndexAutoCleanTask();
                InitPickFromShuttleTask();
            }

            if(fContact->IsRun2DCheck()==true)                                  //JerryYang 20250220 : 2DID硬體順序檢查功能
            {
                if(fContact->RestoreData_2D_Check()==true)                      //JerryYang 20250428 : fix 2DID map
                {
                    return false;
                }
                fContact->Do2DIDMapCheck(true);
            }

            if(CUSTOMER_CODE==CC_SIGURD_HUKOU ||                                //jou 2012-03-07 矽格希望歸完零，只有當下在測試的IC丟到error bin。
               CUSTOMER_CODE==CC_SIGURD_ChungXing ||                            //KaiChen 20201112 ：矽格-中興，強制執行
               IniConfig.bI02HomeSetSocketICToErrBin)                           //JerryYang 20151026 歸零時把當下在測試的IC當ErrorBin
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE &&                                //Ifor 20190218 : add KYEC 要求海思專版需做Self Test時不丟至Err Bin
                   CosFunction.bHiSiliconFunction==true &&
                   (bNeedSendATCRunSelfTest==true ||
                    bNeedWaitATCRunSelfTestFinish==true))
                {
                }
                else
                {
                    if(bUseTwoArm32Site==true)
                    {
                        iCT=FTestSuck.CountRealIC();
                        sBuffer.printf("Do Home Index Arm1 Place to Error bin : Device=%d;", iCT);
                        MyDBIProcess("Message", sBuffer);
                        FTestSuck.SetAllRealIC2InterfaceBin();
                        iCT=BTestSuck.CountRealIC();
                        sBuffer.printf("Do Home Index Arm2 Place to Error bin : Device=%d;", iCT);
                        MyDBIProcess("Message", sBuffer);
                        BTestSuck.SetAllRealIC2InterfaceBin();
                    }
                    else
                    {
                        if(fFrontNeedTest==true)
                        {
                            iCT=FTestSuck.CountRealIC();
                            sBuffer.printf("Do Home Index Arm1 Place to Error bin : Device=%d;", iCT);
                            MyDBIProcess("Message", sBuffer);
                            FTestSuck.SetAllRealIC2InterfaceBin();
                        }
                        else if(fRearNeedTest==true)
                        {
                            iCT=BTestSuck.CountRealIC();
                            sBuffer.printf("Do Home Index Arm2 Place to Error bin : Device=%d;",iCT);
                            MyDBIProcess("Message", sBuffer);
                            BTestSuck.SetAllRealIC2InterfaceBin();
                        }
                    }
                }
            }
            else if(IniConfig.bA03UseAfterHomeCarryAndSuckIcToRBin)
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE &&
                   CosFunction.bHiSiliconFunction==true &&                      //Ifor 20190218 : add KYEC 要求海思專版需做Self Test時不丟至Err Bin
                   (bNeedSendATCRunSelfTest==true ||
                    bNeedWaitATCRunSelfTestFinish==true))
                {
                }
                else
                {
                    if(CUSTOMER_CODE==CC_SCK)
                    {
                        FTestSuck.SetAllHASIC2ErrorBin();
                        BTestSuck.SetAllHASIC2ErrorBin();
                        TestSocket.SetAllHASIC2ErrorBin();
                    }
                    else
                    {
                        iCT=FRCarryKit.CountRealIC();
                        if(iCT>0)
                        {
                            sBuffer.printf("Do Home Out Shuttle1 Place to Error bin : Device=%d;",iCT);
                            MyDBIProcess("Message", sBuffer);
                        }
                        FRCarryKit.SetAllRealIC2InterfaceBin();
                        iCT=BRCarryKit.CountRealIC();
                        if(iCT>0)
                        {
                            sBuffer.printf("Do Home Out Shuttle2 Place to Error bin : Device=%d;",iCT);
                            MyDBIProcess("Message", sBuffer);
                        }
                        BRCarryKit.SetAllRealIC2InterfaceBin();
                        iCT=FTestSuck.CountRealIC();
                        if(iCT>0)
                        {
                            sBuffer.printf("Do Home Index Arm1 Place to Error bin : Device=%d;",iCT);
                            MyDBIProcess("Message", sBuffer);
                        }
                        FTestSuck.SetAllRealIC2InterfaceBin();
                        iCT=BTestSuck.CountRealIC();
                        if(iCT>0)
                        {
                            sBuffer.printf("Do Home Index Arm2 Place to Error bin : Device=%d;",iCT);
                            MyDBIProcess("Message", sBuffer);
                        }
                        BTestSuck.SetAllRealIC2InterfaceBin();
                        TestSocket.SetAllRealIC2InterfaceBin();
                        iCT=OutArmSuck.CountRealIC();
                        if(iCT>0)
                        {
                            sBuffer.printf("Do Home Output Arm Place to Error bin : Device=%d;",iCT);
                            MyDBIProcess("Message", sBuffer);
                        }

                        for(int i=0; i<OutArmSuck.iPickRow; i++)
                        {
                            for(int j=0; j<OutArmSuck.iPickCol; j++)
                            {
                                if(OutArmSuck.Item[i][j]!=NULL_IC && OutArmSuck.Item[i][j]!=HAS_NULL_IC)
                                {
                                    OutArmSuck.iWhichAuto[i][j]=Prod.iIfErrorT6;
                                    OutArmSuck.Item[i][j]=TEST_PASS+iTestBinCount;
                                    OutArmSuck.iBinData[i][j]=iTestBinCount;
                                    OutArmSuck.bPass[i][j]=false;
                                    OutArmSuck.bNeedReTest[i][j]=false;
                                }
                            }
                        }

                        if(CUSTOMER_CODE==CC_ASE_SG)                            //Ifor 20251120 add:ASE 要求A03 InArm InShuttle 也要丟Error Bin
                        {
                            iCT=FLCarryKit.CountRealIC();
                            if(iCT>0)
                            {
                                sBuffer.printf("Do Home In Shuttle1 Place to Error bin : Device=%d;",iCT);
                                MyDBIProcess("Message", sBuffer);
                            }
                            FLCarryKit.SetAllRealIC2InterfaceBin();
                            iCT=BLCarryKit.CountRealIC();
                            if(iCT>0)
                            {
                                sBuffer.printf("Do Home In Shuttle2 Place to Error bin : Device=%d;",iCT);
                                MyDBIProcess("Message", sBuffer);
                            }
                            BLCarryKit.SetAllRealIC2InterfaceBin();

                            iCT=InArmSuck.CountRealIC();
                            if(iCT>0)
                            {
                                sBuffer.printf("Do Home Input Arm Place to Error bin : Device=%d;",iCT);
                                MyDBIProcess("Message", sBuffer);
                            }
                            for(int i=0; i<MAX_ARM_Row; i++)
                            {
                                for(int j=0; j<MAX_ARM_Col; j++)
                                {
                                    if(InArmSuck.Item[i][j]!=NULL_IC && InArmSuck.Item[i][j]!=HAS_NULL_IC)
                                    {
                                        InArmSuck.iWhichAuto[i][j]=Prod.iIfErrorT6;
                                        InArmSuck.Item[i][j]=TEST_PASS+iTestBinCount;
                                        InArmSuck.iBinData[i][j]=iTestBinCount;
                                        InArmSuck.bPass[i][j]=false;
                                        InArmSuck.bNeedReTest[i][j]=false;
                                    }
                                }
                            }
                        }
                    }
                    bPlaceToShuttleFirst=true;
                }
            }

            fMain->ReStartAutoSiteMapping(true);                                //jou 2011-03-24 start : Auto Site Mapping

            bNowDoInterFaceErrorStep=false;
            bContactTimeOverStep=false;
            iDoInterFaceErrorStepTask=1;

            bCheckInArmX=false;
            for(int i=0; i<int(fHome->HomeClass.size()); i++)
            {
                fHome->HomeClass[i]->THomeOrder=fHome->HomeClass[i]->HomeOrder;
            }
            IndexZCanMove[0]=true;
            IndexZCanMove[1]=true;
            IndexMotorBreakerOFF();
            MagazineBreakerOFF();                                               //JerryYang 20220909 : add magazine
            InOutArmZBreakerOFF();                                              //add One sucker with rotate
            LDCarRotArmZBreakerOFF();                                           //RogerYang 20250828 add for Loader Rotate Arm
            CassetteBreakerOFF();                                               //Ifor 20251216 add:Boat Carrier
            ResetOKDeleyTime.Set0_1SecAndOn(2);
            //AI(ht9045-v899) 20260505: 預防中途 Home (生產過程 hangup 後按 Home All) 時，
            //  C_LoaderUpPress 仍 ON 導致台車移動撞壓桿。僅對裝有 MLoaderY 滑軌台車的機型 (eocrYMotInstal) 動作，
            //  避免影響其他機型既有行為。C_LoaderUpPress 固定鎖在機台，台車要移動必須先退開。
            if(INSTALL_OCR_YMot==eocrYMotInstal)
                Cylinder[C_LoaderUpPress].Off();
            if(MOT[MMTrayY].fHasTray==false &&
               LastSet.iRunStartMode!=rsmAutoRetest)                            //ChungHung 20140625 AutoRetest 不能鎖
            {
                if(TRAY_ARM_MODE==eUnderCoveyor)
                {
                    if(USE_OUT_SORT_ARM!=eartUninstall)                         //rogerYang 20250722 add for 9046AU
                    {
                         Cylinder[C_TrayY_Fixer].On();
                    }
                    else
                    {
                        Cylinder[C_LoaderPushBack_Back].On();
                        Cylinder[C_LoaderPushBack_Push].Off();
                    }
                }
                else
                {
                    Cylinder[C_TrayY_Fixer].On();
                }
                Cylinder[C_LoaderEdgePush].On();
            }

            if(TRAY_ARM_MODE==eUnderCoveyor &&
                USE_OUT_SORT_ARM==eartUninstall)                                //rogerYang 20250722 add for 9046AU
            {
                for(int i=0; i<3; i++)
                    TrayCylinMoveIn(3+i);
            }

            SocketAirCoolingStart();                                            //jou 2016-04-28 Socket Air Cooling contact count trun on

            if(bPickFromHotplate ||                                             //jou 2010-01-19 start : 如果資料未轉移過來,需把IC放回去
               bPickFromLoader)                                                 //Steven 20210903 : 修正Loader吸到一半歸零要放回去
            {
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        bSensor=InArmSuck.Suck[i][j].Sensor();
                        if(InArmSuck.Item[i][j]==NULL_IC && bSensor==true)
                        {
                            flag[i][j]=false;
                        }
                        else
                        {
                            flag[i][j]=true;
                        }
                    }
                }

                if(bPickFromHotplate)                                           //Steven 20210903 : 修正Loader吸到一半歸零要放回去
                {                                                               //Steven 20190815 : 修正加熱盤吸取到一半按下歸零,會導致記憶體破壞, Hang up
                    if(iHotWhichShuttle[iPickPlate[0]][iPickPlateX[0]][iPickPlateY[0]]==-1)
                    {
                        if(InArmSuck.Item[0][0]!=NULL_IC)
                        {
                            iHotWhichShuttle[iPickPlate[0]][iPickPlateX[0]][iPickPlateY[0]]=InArmSuck.iWhichShtPickFor32;
                        }
                    }

                    if(iHotWhichKit[iPickPlate[0]][iPickPlateX[0]][iPickPlateY[0]]==-1)
                    {
                        if(InArmSuck.Item[0][0]!=NULL_IC)
                        {
                            iHotWhichKit[iPickPlate[0]][iPickPlateX[0]][iPickPlateY[0]]=InArmSuck.iWhichKitPickFor32;
                        }
                    }
                }
            }

            iSpeedY=(50000*2);                                                  //Ifor 20150703 加快Y移動速度
            if(INDEX_PRESS_TYPE==e240KG ||                                      //jou 2011-12-23 從TfMain::TfMain改放到做Home就要重新設定一次
               INDEX_PRESS_TYPE==e400KG ||
               INDEX_PRESS_TYPE==e260KG ||
               INDEX_PRESS_TYPE==e360KG ||
               INDEX_PRESS_TYPE==e500KG)                                        //kevin 20160725 Add 500 KG
            {
                iSpeedFast=50000*4;
                iSpeedSlow=30000*4;
            }
            else
            {
                iSpeedFast=50000;
                iSpeedSlow=30000;
            }

            for(int i=0; i<2; i++)                                              //Steven 20170425 (wei) : Add rotate motor
            {
                for(int j=0; j<4; j++)
                {
                    MOT[MInRotateKit].Tray.iCurrRotAng[i][j]=0;
                    MOT[MOutRotateKit].Tray.iCurrRotAng[i][j]=0;
                }
            }

            if(MOT[MMPlate1].HasIC()==false && MOT[MMPlate2].HasIC()==false)    //Steven 20220117 : 歸零且加熱盤上面沒有東西的話, 變數要重置
            {
                iPickPlate[0]=0;
                iPickPlateX[0]=0;
                iPickPlateY[0]=0;
                iPickPlate[1]=0;
                iPickPlateX[1]=0;
                iPickPlateY[1]=0;
            }

            if(IniConfig.iI22TestTimeOutOption==3 && bI22_NeedHomeDelay)
            {
                hHomeDelay.SetSecAndOn(IniConfig.fI22HomeDelay);
            }

            TestIntervalsBoostTime.LatchCycleTime(true);                        //JerryYang 20181122 (Steven) :  (Steven) : 將不同function計時器分開
            fHome->iHomeStep=2;
            if(SHUTTLE_FLOODGATE==1)                                            //Ztex 2023.06.02 Add Check_SHUTTLE_FLOODGATE_Staste
            {
                #ifndef SOFT_SIMULTE
                Cylinder[C_Shuttle1Floodgate].Off();
                Cylinder[C_Shuttle2Floodgate].Off();
                Cylinder[C_OutShuttle1Floodgate].Off();                         //Ifor 20240620 add:Out Shuttle Floodgate
                Cylinder[C_OutShuttle2Floodgate].Off();                         //Ifor 20240620 add:Out Shuttle Floodgate
                if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==true ||
                   Cylinder[C_Shuttle2Floodgate].GetOutBit()==true      ||
                   Cylinder[C_OutShuttle1Floodgate].GetOutBit()==true   ||      //Ifor 20240620 add:Out Shuttle Floodgate
                   Cylinder[C_OutShuttle2Floodgate].GetOutBit()==true   )       //Ifor 20240620 add:Out Shuttle Floodgate
                    return false;
                #endif
            }
            break;
        case 2:                                                                 //ChungHung 20140514 fix in homeing and contact make time out
            if(CUSTOMER_CODE==CC_ASE_SG)
            {
                iCheckShuttleSensor=1;
            }

            if(IniConfig.iI22TestTimeOutOption==3 && bI22_NeedHomeDelay)
            {
                if(hHomeDelay.Off()==true)
                {
                    bI22_NeedHomeDelay=false;
                }
                else
                {
                    break;
                }
            }

            if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)                      //wait RTC change file finish
            {
                if(fLotInfo->bRTCChangeFileFinish==false)
                {
                    if(hRealCCDTimeOut.Off())
                        ShowMyMessage("RTC Change File TimeOut");
                    break;
                }
            }
            SW[SwMotorRelay].Off();
            if(SubMachineType==Type_HT9046AU)
                ResetOKDeleyTime.SetSecAndOn(6);                                //RogerYang 20250411 Add for 9046AU
            else
                ResetOKDeleyTime.SetSecAndOn(3);

            if(bPickFromHotplate ||                                             //jou 2010-01-19 start : 如果資料未轉移過來,需把IC放回去
               bPickFromLoader)                                                 //Steven 20210903 : 修正Loader吸到一半歸零要放回去
            {
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        if(flag[i][j]==false)
                        {
                            flag[i][j]=InArmSuck.Suck[i][j].Destroy();
                        }
                    }
                }

                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        if(flag[i][j]==false)
                        {
                            break;
                        }
                    }
                }
            }
            bPickFromLoader=false;                                              //Steven 20210903 : 修正Loader吸到一半歸零要放回去
            bWaitRotateFinish=false;                                            //KevinCheng 20260318 : Rotate流程未完，不可關閉。

            if(bArm1SuckComplete==false)                                        //Steven 20210824 : index歸零時要把IC放回去
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(FTestSuck.Item[i][j]!=NULL_IC)                       //直接把IC資料放回去
                        {
                            FLCarryKit.MoveSuckData(FTestSuck, i, j);
                        }

                        if(FLCarryKit.Item[i][j]!=NULL_IC &&
                           FLCarryKit.Item[i][j]!=HAS_NULL_IC)                  //有料的地方都要吹
                        {
                            FTestSuck.Suck[i][j].Off();
                        }
                    }
                }
            }

            if(bArm2SuckComplete==false)                                        //Steven 20210824 : index歸零時要把IC放回去
            {
                for(int i=0; i<MAX_Index_Row; i++)
                {
                    for(int j=0; j<NEW_MAX_Index_Col; j++)
                    {
                        if(BTestSuck.Item[i][j]!=NULL_IC)                       //直接把IC資料放回去
                        {
                            BLCarryKit.MoveSuckData(BTestSuck, i, j);
                        }

                        if(BLCarryKit.Item[i][j]!=NULL_IC &&
                           BLCarryKit.Item[i][j]!=HAS_NULL_IC)                  //有料的地方都要吹
                        {
                            BTestSuck.Suck[i][j].Off();
                        }
                    }
                }
            }

            fHome->iHomeStep=3;
            break;
        case 3:
            if(ResetOKDeleyTime.Off())
            {
                if(MACHINE_HAS_AUTO_ALIGNMENT_CCD)                              //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
                {
                    fAutoAlignment->InArmSendCommand(LOAD_FILE);
                }

                if(bArm1SuckComplete==false)                                    //Steven 20210824 : index歸零時要把IC放回去
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(FLCarryKit.Item[i][j]!=NULL_IC &&
                               FLCarryKit.Item[i][j]!=HAS_NULL_IC)              //有料的地方都要吹
                            {
                                FTestSuck.Suck[i][j].Normal();
                            }
                        }
                    }
                }
                bArm1SuckComplete=true;

                if(bArm2SuckComplete==false)                                    //Steven 20210824 : index歸零時要把IC放回去
                {
                    for(int i=0; i<MAX_Index_Row; i++)
                    {
                        for(int j=0; j<NEW_MAX_Index_Col; j++)
                        {
                            if(BLCarryKit.Item[i][j]!=NULL_IC &&
                               BLCarryKit.Item[i][j]!=HAS_NULL_IC)              //有料的地方都要吹
                            {
                                BTestSuck.Suck[i][j].Normal();
                            }
                        }
                    }
                }
                bArm2SuckComplete=true;

                if(Cylinder[C_Shuttle_Knocker_1].Enable)                        //Steven 20130108 : 避免蝦頭敲敲汽缸打開撞斷
                    Cylinder[C_Shuttle_Knocker_1].Off();
                if(Cylinder[C_Shuttle_Knocker_2].Enable)
                    Cylinder[C_Shuttle_Knocker_2].Off();
                if(USE_AUTO_RETEST==eartInstall)                                //ChungHung 20140814 add
                    Cylinder[C_TurnTrayArmLock].Off();

                DoMotorPowerOn();
                if(SubMachineType==Type_HT9046AU)                               //RogerYang 20250411 Add for 9046AU
                    ResetOKDeleyTime.SetSecAndOn(4);
                else
                    ResetOKDeleyTime.SetSecAndOn(2);                            //JerryYang 20230522 : 0.3s -> 2s, Power on過幾秒再解magazin煞車
                fHome->iHomeStep=4;
                if(MOT[MMTrayY].fHasTray==false)
                {
                    Cylinder[C_TrayY_Fixer].Off();
                    Cylinder[C_LoaderEdgePush].Off();
                }
            }
            break;
        case 4:
            if(ResetOKDeleyTime.Off())
            {
                fHome->iHomeStep=5;
            }
            break;
        case 5:
            if(ResetOKDeleyTime.Off()==false)
                break;

            fHome->ListBox1->Clear();
            MOT[MTestY1].Gali_Command("SH", __FUNC__);                          //servo On Gail Motor
            ResetOKDeleyTime.SetSecAndOn(0.3);
            SetNoiseDelay=false;
            fHome->iHomeStep=10;
            break;
        case 10:
            if(ResetOKDeleyTime.Off())
                fHome->iHomeStep=20;
            break;
        case 20:
            MOT[MTestZ1].Gali_Command("ST", __FUNC__);                          // claer all motor command
            StopAllDestroy();
            for(int i=0; i<TOTAL_MOTOR; i++)
                fHome->ShowLed(i, 0);
            fHome->Show();
            IndexMotorBreakerON();
            MagazineBreakerON();                                                //JerryYang 20220909 : add magazine
            InOutArmZBreakerON();                                               //add One sucker with rotate
            LDCarRotArmZBreakerOn();                                            //RogerYang 20250828 add for Loader Rotate Arm
            CassetteBreakerON();                                                //Ifor 20251216 add:Boat Carrier
            for(int i=eAuto1; i<=iAutoRight; i++)
            {
                SW[SwAutoCCW[i]].Off();
                SW[SwAutoCW[i]].Off();
            }

            fHome->iHomeStep=100;
            #ifdef SOFT_SIMULTE
                Cylinder[C_TrayX_UpDown].Off();
                fHome->iHomeStep=300;
            #endif
            break;
        case 100:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }
            ResetOKDeleyTime.SetSecAndOn(0.2);
            fHome->iHomeStep=200;
            break;
        case 200:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }

            if(ResetOKDeleyTime.Off())
            {
                fHome->iHomeStep=250;
                fHome->ListBox1->Items->Insert(0, "Tray catcher cylinder up ....");
            }
            break;
        case 250:
            if(Cylinder[C_TrayX_UpDown].Pop())
                fHome->iHomeStep=270;
            break;
        case 270:
            ResetOKDeleyTime.SetSecAndOn(0.1);
            fHome->iHomeStep=280;
            break;
        case 280:
            if(ResetOKDeleyTime.Off())
                fHome->iHomeStep=300;
            break;
        case 300:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }
            SetCylinderResetStateHomeBegin();
            fHome->ResetAllMotorLed();
            for(int i=0; i<TOTAL_MOTOR; i++)
            {
                MOT[i].MotorInitial();
                MOT[i].HomeFlag=0;
                fHome->HomeClass[i]->THomeFlag=1;
                if(MOT[i].Motor->Enable==false)
                {
                    fHome->HomeClass[i]->THomeFlag=0;
                    MOT[i].Position=0;
                    MOT[i].fCanMoveL=true;
                    MOT[i].fCanMoveR=true;
                    MOT[i].fCanMoveM=true;
                    MOT[i].fCanMove=true;
                    MOT[i].MotorMove(MOT[i].Position);
                    MOT[i].HomeFlag=1;
                    fHome->ShowLed(i, 1);
                }
            }
            MOT[MTrayX].TrayArmInitial();

            fHome->HomeClass[MInArmXScale]->THomeFlag=0;                        //Steven 20160426 : 磁性尺
            fHome->HomeClass[MInArmYScale]->THomeFlag=0;
            fHome->HomeClass[MOutArmXScale]->THomeFlag=0;
            fHome->HomeClass[MOutArmYScale]->THomeFlag=0;

            fHome->InitGali_HomeTask();

            //Isaac 20201012 : index Y超過範圍，做一次Tmode
            //=>
            bOverRangeDoTMode=false;
            bOverRange4Indexhome=false;
            bTriger4Indexhome=false;
            iIndexOverRangeCount=0;
            //<=
            //Isaac 20201012 : index Y超過範圍，做一次Tmode
            if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)
            {
                DoTopBtmHome(true);
                FrmAOI->ttbInsp->DoCCDLightDown(false, true);
                fHome->iHomeStep=305;
            }
            else
            {
                fHome->iHomeStep=310;
            }
            MotorTask=FIRST_HOME;                                               //第一階段回Home
            for(int i=0; i<TOTAL_MOTOR; i++)
            {
                if(fHome->HomeClass[i]->THomeFlag &&
                   fHome->HomeClass[i]->THomeOrder==MotorTask)                  //Z軸先回Home
                {
                    fHome->ShowLed(i, 3);
                    sprintf(str, "M%2d homeing ....", i+1);
                    fHome->ListBox1->Items->Insert(0, str);
                }
            }
            break;
        case 305:                                                               //Jimmychiu 20240307 : Loader Tray改用步進馬達
            if(DoTopBtmHome(false))
            {
                fHome->iHomeStep=310;
            }
            break;
        case 310:                                                               //是否需要讀取扭力值
            if(SW[SwReadTorue].Enable)
            {
                InitWriteAndCheckMotorTorqueTask();
                fHome->iHomeStep=320;
                fHome->ListBox1->Items->Insert(0, "MTestZ1 set Torque 80...");
            }
            else
            {
                fHome->iHomeStep=350;
            }
            break;
        case 320:                                                               //Arm 1 扭力讀取是否正常
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
            }

            ret=COM2->iWriteAndCheckMotorTorque(0, 300);

            if(ret==1)
            {
                fHome->ListBox1->Items->Insert(0, "MTestZ1 set Torque 300...");                                         //kevin 20130425
                InitWriteAndCheckMotorTorqueTask();
                if(bContaceTorque==false &&
                   IniConfig.bKoreaFunction==false &&
                   CUSTOMER_CODE!=CC_TSMC_TAINAN &&
                   IniConfig.bSPILFunction==false)                              //JerryYang 20230204 : Add SPIL support回傳GPIB force指令
                {
                    asArmForce1="";                                             //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND
                }
                fHome->iHomeStep=322;
            }
            else if(ret==2)
            {
                ShowMyMessage("Motor MTestZ1 torque set error!!", "MTestZ1馬達壓力設定錯誤", "ProcessMotorHome 320");
                fHome->fAbort=false;
                SystemStart=false;
                fHome->iHomeStep=1;
                SoftStop=true;
                return true;
            }
            break;
        case 322:                                                               //Arm 2 扭力讀取是否正常
            if(USE_IO_CHANGE_TOQUE==true)                                       //jou 2012-06-21 Enable index I/O Change Toque
            {
                SW[SwIndexChangeToque1].Off();
                SW[SwIndexChangeToque2].Off();
                ret=1;
            }

            ret=COM2->iWriteAndCheckMotorTorque(1, 300);

            if(ret==1)
            {
                InitWriteAndCheckMotorTorqueTask();
                fHome->ListBox1->Items->Insert(0, "MTestZ2 set Torque 300...");                                         //kevin 20130425

                if(bContaceTorque==false &&
                   IniConfig.bKoreaFunction==false &&
                   CUSTOMER_CODE!=CC_TSMC_TAINAN &&
                   IniConfig.bSPILFunction==false)                              //JerryYang 20230204 : Add SPIL support回傳GPIB force指令
                {
                    asArmForce2="";                                             //kevin 20130425 2013.01.11 Q_Q TSMC GPIB COMMAND
                }
                fHome->iHomeStep=350;
            }
            else if(ret==2)
            {
                ShowMyMessage("Motor MTestZ2 torque set error!!", "MTestZ2馬達壓力設定錯誤", "ProcessMotorHome 322");
                fHome->fAbort=false;
                SystemStart=false;
                fHome->iHomeStep=1;
                SoftStop=true;
                return true;
            }
            break;
        case 350:
            fHome->iHomeStep=375;
            MOT[MTestY1].iGali_SingalHomeTask=1;
            MOT[MTestY2].iGali_SingalHomeTask=1;
            MOT[MTestZ1].iGali_SingalHomeTask=1;
            MOT[MTestZ2].iGali_SingalHomeTask=1;
            MOT[MTestZ1].iGali_FindZPhaseTask[1]=1;                             //Ifor 20170817 (wei) add Z Phase Task
            MOT[MTestZ2].iGali_FindZPhaseTask[2]=1;                             //Ifor 20170817 (wei) add Z Phase Task
            MOT[MTestY1].iGali_FindZPhaseTask[0]=1;                             //Isaac 20201110 : Index Y find motor phase
            MOT[MTestY2].iGali_FindZPhaseTask[3]=1;                             //Isaac 20201110 : Index Y find motor phase

            if(IniConfig.bP37bAutoCylinderUP)                                   //kevin 20180726 Auto 123 氣缸常態在上
            {                                                                   //kevin 20180726  Auto 123 氣缸常態在上
                fHome->iHomeStep=360;
                for(int i=eAuto1; i<=iAutoRight; i++)
                {
                    bflag[i]=false;                                             //升到接盤位置
                    AutoCylinderLower(i, C_Auto_Up[i], C_Auto_Selector[i], true);                                       //Steven 20140409 : AutoCylinderXX Add Reset
                    AutoCylinderMiddle(i, C_Auto_Up[i], C_Auto_Selector[i],true);                                       //升到接盤位置
                    AutoCylinderUp(i, C_Auto_Up[i], C_Auto_Selector[i],true);   //升到接盤位置
                }
            }
            break;
        case 360:                                                               //kevin 20180726  Auto 123 氣缸常態在上
            for(int i=eAuto1; i<=iAutoRight; i++)
            {
                if(Sen[SnAutoTrackDetect[i]].IsOn() ||
                   Sen[SnAutoTrayCar[i]].IsOn())                                //軌道上有tray
                {
                    bflag[i]=true;                                              //kevin 20180726 Auto 12 3 up
                    flag1=true;
                }
            }

            if(flag1)                                                           //kevin 20180720  Auto 123 氣缸常態在上
                fHome->iHomeStep=361;
            else
                fHome->iHomeStep=362;
            break;
        case 361:
            for(int i=eAuto1; i<=iAutoRight; i++)
            {
                if(bflag[i])                                                    //kevin 20180726 Auto 12 3 up
                {
                    ibuffer++;
                    if(AutoCylinderMiddle(i, C_Auto_Up[i], C_Auto_Selector[i]))                                         //升到接盤位置
                    {
                       Cylinder[C_AutoZ_Select[i]].On();
                       bflag[i]=false;
                    }
                }
            }

            if(ibuffer==0)
                fHome->iHomeStep=362;
            break;
        case 362:
            for(int i=eAuto1; i<=iAutoRight; i++)
            {
                if(AutoCylinderUp(i, C_Auto_Up[i], C_Auto_Selector[i]))         //升到接盤位置
                {
                    Cylinder[C_AutoZ_Select[i]].Off();
                    bflag[i]=true;
                }
            }

            for(int i=eAuto1; i<=iAutoRight; i++)
            {
                if(bflag[i])                                                    //升到接盤位置
                {
                   ibuffer++;
                }
            }

            if(ibuffer==3)
                fHome->iHomeStep=375;
            break;
        case 375:                                                               //所有Z軸回Home中
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }
            flag1=true;
            for(int i=0; i<TOTAL_MOTOR; i++)
            {
                ret=0;
                if(fHome->HomeClass[i]->THomeFlag &&
                   fHome->HomeClass[i]->THomeOrder==MotorTask)
                {
                    if(INDEX_MOTION_CARD==0 && (i==MTestZ1 || i==MTestZ2))      //Index的Z軸         //Steven 20210623 : Index使用Galil
                    {
                        if(MOT[i].Motor->Enable)
                        {
                            if(MOT[i].Led[iCcwLed] || MOT[i].Led[iCwLed])
                            {
                                iRef=MOT[i].GetErrorIndex();
                                if(iRef==9) iRef=7;
                                JamCode=MotorIndexToJamCode(i);
                                ShowMotorErrorMessage(JamCode, iRef+1);
                                fHome->fAbort=false;
                                SoftStop=true;
                                return true;
                            }

                            if(MOT[i].Gali_SingalHome(true)==true)              //RogerYang 20161116 Index Z 第一次回Home, flag=true
                            {
                                if(MOT[i].Led[iServoOn]==false)
                                {
                                    StopAllMotor();
                                    JamCode=MotorIndexToJamCode(i);
                                    ShowMotorErrorMessage(JamCode, 0);
                                    fHome->fAbort=false;
                                    SoftStop=true;
                                    return true;
                                }

                                fHome->HomeClass[i]->THomeFlag=0;
                                fHome->ShowLed(i, 1);
                                sprintf(str, "M%02d home finish.", i+1);
                                fHome->ListBox1->Items->Insert(0, str);
                            }
                        }
                        else
                        {
                            fHome->HomeClass[i]->THomeFlag=0;
                            fHome->ShowLed(i, 1);
                            sprintf(str, "M%02d home finish.", i+1);
                            fHome->ListBox1->Items->Insert(0, str);
                        }
                    }
                    else
                    {
                        if(i==MInArmY)
                        {
                            if(MOT[MInArmPitch].HomeFlag==1)
                            {
                                if(bCheckInArmX==true)
                                {
                                    ret=0;
                                    MOT[MInArmX].ScanMotorStatus();
                                    if(MOT[MInArmX].Motor->HomeFlag())
                                    {
                                        MOT[MInArmX].PCIL132_StopMotor();
                                        bCheckInArmX=false;
                                    }
                                }
                                else
                                {
                                    ret=MOT[i].MotorHome(flag1);
                                }
                            }
                            else
                            {
                                ret=0;
                            }
                        }
                        else
                        {
                            ret=MOT[i].MotorHome(flag1);
                        }
                    }

                    if(MOT[i].HomeFlag==0 && ret==2)
                    {
                        StopAllMotor();
                        fHome->ShowLed(i, 2);
                        fHome->iHomeStep=1;
                        SystemStart=false;
                        iRef=MOT[i].GetErrorIndex();
                        if(iRef==9) iRef=7;
                        JamCode=MotorIndexToJamCode(i);
                        ShowMotorErrorMessage(JamCode, iRef+1);
                        fHome->fAbort=false;
                        SoftStop=true;
                        return true;
                    }

                    if(ret==4)
                    {
                        StopAllMotor();
                        fHome->ShowLed(i, 2);
                        JamCode=MotorIndexToJamCode(i);
                        ShowMotorErrorMessage(JamCode, 0);
                        fHome->fAbort=false;
                        SystemStart=false;
                        fHome->iHomeStep=1;
                        SoftStop=true;
                        return true;
                    }
                    else if(ret==3)
                    {
                        StopAllMotor();
                        fHome->ShowLed(i, 2);
                        iRef=MOT[i].GetErrorIndex();
                        if(iRef==9) iRef=6;
                        sprintf(str, "Mot%04d", 5000+i*10+iRef);
                        JamCode=MotorIndexToJamCode(i);
                        ShowMotorErrorMessage(JamCode, iRef+1);
                        fHome->fAbort=false;
                        SystemStart=false;
                        fHome->iHomeStep=1;
                        SoftStop=true;
                        return true;
                    }
                    else if(ret==1 && MOT[i].HomeFlag==1)
                    {
                        MOT[i].HomeFlag=1;
                        fHome->HomeClass[i]->THomeFlag=0;
                        fHome->ShowLed(i, 1);
                        sprintf(str, "M%02d home finish", i+1);
                        fHome->ListBox1->Items->Insert(0, str);
                        MOT[i].Position=0;
                    }
                    else
                    {
                        flag1=false;
                    }
                }

                if(!fHome->ShowMotorHomePos(i))
                {
                    return true;
                }
            }

            if(flag1)
            {
                if(MotorTask==FIRST_HOME)
                {
                    Cylinder[C_FixTray_FullPlace].Off();                        //ChungHung 20140313 add Fix3 can Full Tray
                    if(CheckInArmSuckICFallDownToHasNullIC()==true)             //JerryYang 20200424 回home時新增掉料偵測
                    {
                        fHome->fAbort=false;
                        SystemStart=false;
                        fHome->iHomeStep=1;
                        SoftStop=true;
                        return true;
                    }

                    if(CheckOutArmSuckICFallDown()==true)                       //JerryYang 20200424 回home時新增掉料偵測
                    {
                        fHome->fAbort=false;
                        SystemStart=false;
                        fHome->iHomeStep=1;
                        SoftStop=true;
                        return true;
                    }

                    for(int i=0; i<4; i++)
                    {
                        if(Cylinder[C_TrayXFloodgate1+i].Enable)
                        {
                            Cylinder[C_TrayXFloodgate1+i].On();
                            bCyflag[i]=false;
                        }
                        else
                        {
                            bCyflag[i]=true;
                        }
                    }

                    MotorTask=SECOND_HOME;
                    fHome->iHomeStep=400;
                }
            }
            break;
        case 400:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }

            for(int i=0; i<CynForHome; i++)                                     //RogerYang 20250909 : 雙動氣缸 pop側先關
            {
                if(CylinderInitSwitchType[i]==false)
                {
                    Cylinder[CynNeedHome[i]].Off();
                    CylinderInitOKFlag[i]=true;
                }
            }

            flag1=true;
            for(int i=0; i<CynForHome; i++)
            {
                if(CylinderInitOKFlag[i]==false)
                {
                    if(CylinderInitState[i])
                    {
                        if(Cylinder[CynNeedHome[i]].Push())
                            CylinderInitOKFlag[i]=true;
                        else
                            flag1=false;
                    }
                    else
                    {
                        if(Cylinder[CynNeedHome[i]].Pop())
                            CylinderInitOKFlag[i]=true;
                        else
                            flag1=false;
                    }
                }
            }

//            if(bCyflag[0]==false)
//                bCyflag[0]=(Cylinder[C_TrayXFloodgate1].Enable==false || Cylinder[C_TrayXFloodgate1].Push());
//            if(bCyflag[1]==false)
//                bCyflag[1]=(Cylinder[C_TrayXFloodgate2].Enable==false || Cylinder[C_TrayXFloodgate2].Push());
//            if(bCyflag[2]==false)
//                bCyflag[2]=(Cylinder[C_TrayXFloodgate3].Enable==false || Cylinder[C_TrayXFloodgate3].Push());
//            if(bCyflag[3]==false)
//                bCyflag[3]=(Cylinder[C_TrayXFloodgate4].Enable==false || Cylinder[C_TrayXFloodgate4].Push());
//
//            if(!(bCyflag[0] && bCyflag[1] && bCyflag[2] && bCyflag[3]))
//                flag1=false;

            if(MOT[MTrayX].fHasTray ||                                          //jou 2012-10-26 修正歸零catch tray 會把tray丟在中途
               (LastSet.iRunStartMode==rsmAutoRetest &&
                Cylinder[C_CatchTray_FixOn].OnStatus()) ||
               (USE_AUTO_RETEST==eartInstall &&                                 //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
                Cylinder[C_CatchTray_FixOn].OnStatus()))                        //kevin 20160920
            {
                if(USE_AUTO_RETEST==eartInstall)                                //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
                {
                    if(Cylinder[C_CatchTray_FixOn].OnSensor()==false)           //kevin 20160920 沒有tray
                    {
                        MOT[MTrayX].fHasTray=false;
                        Cylinder[C_CatchTray_FixOn].Off();
                        Cylinder[C_CatchTray_FixOff].On();
                    }
                    else
                    {
                        if(MOT[MTrayX].fHasTray==false)                         //JerryYang 20251014 : tray arm沒資料但是偵測到有tray盤的時候跳一下alarm
                        {
                            Str.sprintf("Tray arm detect a tray on it. Please check if there is a tray!(C_CatchTray_FixOn_On)");
                            ShowMyMessage(Str);
                        }
                        MOT[MTrayX].fHasTray=true;                              //ChungHung 20141208 add for AutoRetest mode 中途關程式 catch Tray 要搬到loader
                        Cylinder[C_CatchTray_FixOn].On();
                        Cylinder[C_CatchTray_FixOff].Off();
                    }
                }
                else
                {
                    Cylinder[C_CatchTray_Fix].On();
                }
            }
            else
            {
                if(USE_AUTO_RETEST==eartInstall)                                //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
                {
                    Cylinder[C_CatchTray_FixOn].Off();
                    Cylinder[C_CatchTray_FixOff].On();
                }
                else
                {
                    Cylinder[C_CatchTray_Fix].Off();
                }
            }

            if(flag1)
            {
                if(USE_AUTO_RETEST==eartInstall)                                //ChungHung 20140814 add
                {
                    if(Cylinder[C_TurnTrayArmLock].Push())
                        fHome->iHomeStep=500;
                }
                else                                                            //Steven 20140827
                {
                    fHome->iHomeStep=500;
                }
            }
            break;
        case 500:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }
            fHome->iHomeStep=600;
            MotorTask=SECOND_HOME;
            for(int i=0; i<TOTAL_MOTOR; i++)
            {
                if(fHome->HomeClass[i]->THomeFlag &&
                   fHome->HomeClass[i]->THomeOrder==MotorTask)
                {
                    fHome->ShowLed(i, 3);
                    sprintf(str, "M%2d homeing ....", i+1);
                    fHome->ListBox1->Items->Insert(0, str);
                }
            }

            if(USE_PICKER_COUNT==ep16Picker &&
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.15 Add Pitch X Home Twice
            {
                bPitchHome_Twice=true;
                bPitchHome[0]=false;
                bPitchHome[1]=false;
                fHome->iHomeStep=510;
            }

            #ifdef DEBUG_HOME                                                   //wei (Steven) 20170317 快速回Home開關

            IndexY1=false;                                                      //kevin 20161214 (Steven) Increase Hone Speed
            IndexY2=false;
            IndexZ1=false;
            IndexZ2=false;
            #endif
            break;
        case 510:                                                               //Ztex 2023.12.15 Add Pitch X Home Twice
            for(int i=0;i<4;i++)
                iPitch_Move[i]=0;
            if(bPitchHome[0]==false)
            {
                bPitchHome[0]=fHome->MoveInArmPitch_X(iPitch_Move, true);
            }

            if(bPitchHome[1]==false)
            {
                bPitchHome[1]=fHome->MoveOutArmPitch_X(iPitch_Move, true);
            }

            if(bPitchHome[0]==true && bPitchHome[1]==true)
            {
                bPitchHome[0]=false;
                bPitchHome[1]=false;
                fHome->iHomeStep=520;
            }

            break;
        case 520:                                                               //Ztex 2023.12.15 Add Pitch X Home Twice
            for(int i=0;i<4;i++)
                iPitch_Move[i]=-200;
            if(bPitchHome[0]==false)
            {
                bPitchHome[0]=fHome->MoveInArmPitch_X(iPitch_Move, true);
            }

            if(bPitchHome[1]==false)
            {
                bPitchHome[1]=fHome->MoveOutArmPitch_X(iPitch_Move, true);
            }

            if(bPitchHome[0]==true && bPitchHome[1]==true)
            {
                bPitchHome[0]=false;
                bPitchHome[1]=false;
                fHome->iHomeStep=600;
            }
            break;
        case 600:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }
            flag1=true;
            for(int i=0; i<TOTAL_MOTOR; i++)
            {
                if(fHome->HomeClass[i]->THomeFlag &&
                   fHome->HomeClass[i]->THomeOrder==MotorTask)
                {
                    ret=0;
                    if(INDEX_MOTION_CARD==0 && (i==MTestY1 || i==MTestY2))      //Steven 20210623 : Index使用Galil
                    {
                        if(MOT[i].Motor->Enable)
                        {
                            #ifdef DEBUG_HOME                                   //wei (Steven) 20170317 快速回Home開關
                            if(MOT[i].Led[iCcwLed] ||
                               MOT[i].Led[iCwLed] &&
                               MOT[i].Led[iHomeLed])                            //kevin 20170209 add
                            {
                                iRef=MOT[i].GetErrorIndex();
                                if(iRef==9) iRef=7;
                                JamCode=MotorIndexToJamCode(i);
                                ShowMotorErrorMessage(JamCode, iRef+1);
                                fHome->fAbort=false;
                                SoftStop=true;
                                return true;
                            }
                            #else

                            #endif
                            if(MOT[i].Gali_SingalHome())
                            {
                                if(MOT[i].Led[iServoOn]==false)
                                {
                                    StopAllMotor();
                                    JamCode=MotorIndexToJamCode(i);
                                    ShowMotorErrorMessage(JamCode, 8);
                                    fHome->fAbort=false;
                                    SoftStop=true;
                                    return true;
                                }

                                fHome->HomeClass[i]->THomeFlag=0;
                                fHome->ShowLed(i, 1);
                                sprintf(str, "M%02d home finish.", i+1);
                                fHome->ListBox1->Items->Insert(0, str);
                                MOT[i].Position=0;
                            }
                            else if(MOT[i].Led[iAlarmLed])                      //Steven 20230708 : Galil歸零要可以Alarm
                            {
                                iRef=MOT[i].GetErrorIndex();
                                if(iRef==9)
                                    iRef=7;
                                JamCode=MotorIndexToJamCode(i);
                                ShowMotorErrorMessage(JamCode, iRef+1);
                                fHome->fAbort=false;
                                SoftStop=true;
                                return true;
                            }
                        }
                        else
                        {
                            MOT[i].Position=0;
                            MOT[i].HomeFlag=1;
                            fHome->HomeClass[i]->THomeFlag=0;
                            fHome->ShowLed(i, 1);
                            sprintf(str, "M%02d home finish.", i+1);
                            fHome->ListBox1->Items->Insert(0, str);
                        }
                    }
                    else
                    {
                        ret=MOT[i].MotorHome(flag1);
                    }

                    if(i==MLoaderY)                                             //RogerYang 20251105 : 修正撞機問題
                    {
                        if(INSTALL_OCR_YMot==eocrYMotUninstal)                  //RogerYang 20250909 : 只要是Motor就要回home//Frank 20250214 add
                        {
                            MOT[i].HomeFlag=1;
                            ret=1;
                            MOT[i].PCIL132_SetPos(0);
                        }

                        if(INSTALL_OCR!=eocrUninstal &&
                           CosFunction.bTrayOCR)                                //wei 20150925
                        {
                            if(bOCRHomeActionPass)
                            {
                                Cylinder[C_OCRLight_Up].Off();
                                if(IniConfig.OCRLightChange)                    //wei 20181225 光源auto change
                                {
                                    fOCR->ChangeLightValue(1, 0);
                                    fOCR->ChangeLightValue(2, 0);
                                }
                                bOCRHomeActionPass=false;
                            }
                        }
                    }

                    if(MOT[i].HomeFlag==0 && ret==2)
                    {
                        StopAllMotor();
                        fHome->ShowLed(i, 2);
                        fHome->iHomeStep=1;
                        SystemStart=false;
                        iRef=MOT[i].GetErrorIndex();
                        if(iRef==9) iRef=7;
                        JamCode=MotorIndexToJamCode(i);
                        ShowMotorErrorMessage(JamCode, iRef+1);
                        fHome->fAbort=false;
                        SoftStop=true;
                        return true;
                    }

                    if(ret==4)
                    {
                        StopAllMotor();
                        fHome->ShowLed(i, 2);
                        JamCode=MotorIndexToJamCode(i);
                        ShowMotorErrorMessage(JamCode, 8);
                        fHome->fAbort=false;
                        SystemStart=false;
                        fHome->iHomeStep=1;
                        SoftStop=true;
                        return true;
                    }
                    else if(ret==3)
                    {
                        StopAllMotor();
                        fHome->ShowLed(i, 2);
                        iRef=MOT[i].GetErrorIndex();
                        if(iRef==9) iRef=6;
                        JamCode=MotorIndexToJamCode(i);                         //alarm message problem
                        ShowMotorErrorMessage(JamCode, iRef+1);
                        fHome->fAbort=false;
                        SystemStart=false;
                        fHome->iHomeStep=1;
                        SoftStop=true;
                        return true;
                    }
                    else if(ret==1 && MOT[i].HomeFlag==1)
                    {
                        MOT[i].HomeFlag=1;
                        fHome->HomeClass[i]->THomeFlag=0;
                        fHome->ShowLed(i, 1);
                        sprintf(str, "M%02d home finish.", i+1);
                        fHome->ListBox1->Items->Insert(0, str);
                        MOT[i].Position=0;
                    }
                    else
                    {
                        flag1=false;
                    }
                }

                if(!fHome->ShowMotorHomePos(i))
                    return true;
            }

            if(Sen[SnMagazineTrackDetect].IsOn()==true)                         //JerryYang 20220909 : add magazine
            {
                // Alarm : Track has tray can not up/down
                ShowMyMessage("SnMagazineTrackDetect is on, Magazine can not move");
                fHome->fAbort=false;
                SystemStart=false;
                fHome->iHomeStep=1;
                SoftStop=true;
                return false;
            }

            if(Sen[SnMagazineTrackDetect2].IsOn()==true)                        //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {                                                                   // Alarm : Track has tray can not up/down
                ShowMyMessage("SnMagazineTrackDetect2 is on, Magazine can not move");
                fHome->fAbort=false;
                SystemStart=false;
                fHome->iHomeStep=1;
                SoftStop=true;
                return false;
            }

            if(Cylinder[C_OCRLight_Up].OnSensor()==true)
                flag11=true;

            if(flag1 && flag11)
            {
                if(MotorTask==FIRST_HOME)
                {
                    for(int i=0; i<TOTAL_MOTOR; i++)
                        if(fHome->HomeClass[i]->THomeFlag &&
                           fHome->HomeClass[i]->THomeOrder==MotorTask)
                            fHome->ShowLed(i, 3);
                    MotorTask=SECOND_HOME;
                }
                else if(MotorTask==SECOND_HOME)
                {
                    if(USE_PICKER_COUNT==ep16Picker &&                          //Ztex 2023.12.15 Add Pitch X Home Twice  //Ztex 2024.01.14 Modify Pitch X Move Pos Error Issue
                       USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be &&
                       bPitchHome_Twice==true)
                    {
                        for(int i=0;i<4;i++)
                            iPitch_Move[i]=-500;

                        if(bPitchHome[0]==false)
                        {
                            bPitchHome[0]=fHome->MoveInArmPitch_X(iPitch_Move, false);
                        }

                        if(bPitchHome[1]==false)
                        {
                            bPitchHome[1]=fHome->MoveOutArmPitch_X(iPitch_Move, false);
                        }

                        if(bPitchHome[0]==true && bPitchHome[1]==true)
                        {
                            bPitchHome_Twice=false;
                            for(int i=0;i<TOTAL_MOTOR;i++)
                            {
                                if(i==MInArmPitch  || i==MInArmPitchX2  || i==MInArmPitchX3  || i==MInArmPitchX4 ||
                                   i==MOutArmPitch || i==MOutArmPitchX2 || i==MOutArmPitchX3 || i==MOutArmPitchX4)
                                {
                                    fHome->HomeClass[i]->THomeFlag=1;
                                }
                            }
                            fHome->iHomeStep=600;
                            break;
                        }
                    }
                    else
                    {
                        MotorTask=THREE_HOME;
                        fHome->iHomeStep=650;
                        //----- by dell ccd realtime-------------
                        if(REAL_TIME_CCD==true)                                 //不管有沒有開RTC都要歸零
                        {
                            COM2->SendCommToVision(COM2->rtHome, true);
                            hRealCCDTimeOut.SetSecAndOn(2);
                        }
                        //---------------------------------------
                        #ifdef DEBUG_HOME                                       //wei (Steven) 20170317 快速回Home開關
                        MOT[MTestY1].iGali_SingalHomeTask=1;                    //kevin 20161214 (Steven) Increase Hone Speed
                        MOT[MTestY2].iGali_SingalHomeTask=1;
                        MOT[MTestZ1].iGali_SingalHomeTask=1;
                        MOT[MTestZ2].iGali_SingalHomeTask=1;
                        MOT[MTestZ1].iGali_FindZPhaseTask[1]=1;                 //Ifor 20170817 (wei) add Z Phase Task
                        MOT[MTestZ2].iGali_FindZPhaseTask[2]=1;                 //Ifor 20170817 (wei) add Z Phase Task
                        MOT[MTestY1].iGali_FindZPhaseTask[0]=1;                 //Isaac 20201110 : Index Y find motor phase
                        MOT[MTestY2].iGali_FindZPhaseTask[3]=1;                 //Isaac 20201110 : Index Y find motor phase
                        #endif
                    }
                }
                else if(MotorTask==THREE_HOME)
                {
                    fHome->iHomeStep=700;

                    for(int i=0; i<4; i++)
                    {
                        if(Cylinder[C_TrayXFloodgate1+i].Enable)
                        {
                            bCyflag[i]=false;
                        }
                        else
                        {
                            bCyflag[i]=true;
                        }
                    }
                }
            }
            break;
        case 650:                                                               //Tray Arm歸零
            //----- by dell ccd realtime-------------
            if(REAL_TIME_CCD==true && !COM2->bCCDDummyRum)
            {
                if(COM2->bRealTimeCom_ReceiveOK[COM2->rtHome]==false)
                {
                    if(hRealCCDTimeOut.Off())
                    {
                        if(COM2->OpenRTCComPortAgain())                         //ChungHung 20121005 add
                            ShowErrorMessage("WAR0338", 0, MMIndex, 0, __FUNC__);                                       //RTC Home Error!
                        COM2->SendCommToVision(COM2->rtHome, true);
                        hRealCCDTimeOut.SetSecAndOn(2);
                    }
                    break;
                }
            }
            //---------------------------------------
            if(CheckInArmSuckICFallDownToHasNullIC()==true)                     //JerryYang 20200424 回home時新增掉料偵測
            {
                fHome->fAbort=false;
                SystemStart=false;
                fHome->iHomeStep=1;
                SoftStop=true;
                return true;
            }

            if(CheckOutArmSuckICFallDown()==true)                               //JerryYang 20200424 回home時新增掉料偵測
            {
                fHome->fAbort=false;
                SystemStart=false;
                fHome->iHomeStep=1;
                SoftStop=true;
                return true;
            }
            MotorTask=THREE_HOME;
            for(int i=0; i<TOTAL_MOTOR; i++)
            {
                if(fHome->HomeClass[i]->THomeFlag &&
                   fHome->HomeClass[i]->THomeOrder==MotorTask)
                {
                    fHome->ShowLed(i, 3);
                }
            }
            fHome->iHomeStep=600;
            break;
        case 700:
            #ifdef DEBUG_HOME                                                   //wei (Steven) 20170317 快速回Home開關
            fHome->iHomeStep=710;
            #else
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "ProcessMotorHome 700"))                       //Steven 20140828 : 歸零後檢查Index位置    //JerryYang 20160905 10 --> Prod.TestZ1_Safe
                fHome->iHomeStep=710;
            #endif
            break;
        case 710:
            #ifdef DEBUG_HOME                                                   //wei (Steven) 20170317 快速回Home開關
            fHome->iHomeStep=750;                                               //kevin 20161214 (Steven) Increase Hone Speed
            #else
            if(MOT[MTestY1].GalilTwoY_Move(-10, 10, 60000, "ProcessMotorHome 710"))                                     //Steven 20140828 : 歸零後檢查Index位置
            {
                fHome->iHomeStep=750;
            }
            #endif
            break;
        case 750:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }
//            SetCylinderResetStateHomeAfter();                                 //Steven 20240123 : 完全不會執行, mark
            fHome->iHomeStep=800;
            break;
        case 800:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }

            flag1=true;
            if(flag1)
            {
                fHome->iHomeStep=900;
            }
            break;
        case 900:
            if(fHome->fShow==false)
            {
                flag1=false;
                SoftStop=true;
                break;
            }
            fHome->iHomeStep=1000;
            fHome->ListBox1->Items->Insert(0, "Cylinder Auto 1 down...");
            fHome->ListBox1->Items->Insert(0, "Cylinder Auto 2 down...");
            fHome->ListBox1->Items->Insert(0, "Cylinder Auto 3 down...");
            if(AUTO_EMPTY_COLOR>=3)
            {
                fHome->ListBox1->Items->Insert(0, "Cylinder Auto 4 down...");
                fHome->ListBox1->Items->Insert(0, "Cylinder Auto 5 down...");
            }

            if(AUTO_EMPTY_COLOR>=4)
            {
                fHome->ListBox1->Items->Insert(0, "Cylinder Auto 6 down...");
            }
            flag1=false;
            flag2=false;
            flag3=false;
            flag4=false;
            flag5=false;
            flag6=false;
            break;
        case 1000:
            if(fHome->fShow==false)
            {
                SoftStop=true;
                break;
            }

            if(IniConfig.bP37bAutoCylinderUP)                                   //kevin 20180726 Auto 123 氣缸常態在上
            {                                                                   //kevin 20180726 (wei) Auto 123 Z氣缸常態在上
                flag1=true;
                flag2=true;
                flag3=true;
                flag4=true;
                flag5=true;
                flag6=true;
            }
            else
            {
                if(flag1==false)
                {
                    if(TRAY_ARM_MODE==eUnderCoveyor)
                    {
                        if(USE_OUT_SORT_ARM!=eartUninstall)                     //rogerYang 20250722 add for 9046AU
                        {
                             Cylinder[C_TrayY_Fixer].Off();
                        }
                        else
                        {
                            Cylinder[C_LoaderPushBack_Back].Off();
                            Cylinder[C_LoaderPushBack_Push].On();
                        }
                    }
                    else
                    {
                        Cylinder[C_TrayY_Fixer].Off();
                    }
                    Cylinder[C_LoaderEdgePush].Off();
                    Cylinder[C_Auto1_Up].Off();                                 //ChungHung 20140526 add
                    if(Cylinder[C_Auto1_Selector].Pop())
                        flag1=true;
                }

                if(flag2==false)
                {
                    Cylinder[C_Auto2_Up].Off();
                    if(Cylinder[C_Auto2_Selector].Pop())
                        flag2=true;
                }

                if(flag3==false)
                {
                    if(AUTO3_IS_MAGAZINE==1)                                    //JerryYang 20220909 : add magazine
                    {
                        flag3=true;
                    }
                    else
                    {
                        Cylinder[C_Auto3_Up].Off();                             //ChungHung 20140526 add
                        if(Cylinder[C_Auto3_Selector].Pop())
                            flag3=true;
                    }
                }

                if(AUTO_EMPTY_COLOR>=3)
                {
                    if(flag4==false)
                    {
                        Cylinder[C_Auto4_Up].Off();
                        if(Cylinder[C_Auto4_Selector].Pop())
                            flag4=true;
                    }

                    if(flag5==false)
                    {
                        Cylinder[C_Auto5_Up].Off();
                        if(Cylinder[C_Auto5_Selector].Pop())
                            flag5=true;
                    }

                    if(AUTO_EMPTY_COLOR>=4)
                    {
                        if(flag6==false)
                        {
                            Cylinder[C_Auto6_Up].Off();
                            if(Cylinder[C_Auto6_Selector].Pop())
                                flag6=true;
                        }
                    }
                    else
                    {
                        flag6=true;
                    }
                }
                else
                {
                    flag4=true;
                    flag5=true;
                    flag6=true;
                }
            }

            if(flag1 && flag2 && flag3)
                fHome->iHomeStep=1100;
            break;
        case 1100:
            if(fHome->fShow==false)
            {
                fHome->iHomeStep=1300;
                break;
            }

            fHome->Panel2->Visible=true;
            fHome->iHomeStep=1200;
            ResetOKDeleyTime.SetSecAndOn(0.1);
            fLtcSensor->ClearLtcSensor(1);                                      //Sam 20221101 : Latch 清除都要確認是否清清乾淨
            fLtcSensor->ClearLtcSensor(0);                                      //Sam 20221101 : Latch 清除都要確認是否清清乾淨
            iOutShuttle1HasICErrRetryCnt=0;
            iOutShuttle2HasICErrRetryCnt=0;

            bIn_ICRotationCompleteOnKit=false;                                  //Sam 20210623 : 修正 IC 轉完後被 Home，導致 IC 轉向異常。
            bOut_ICRotationCompleteOnKit=false;
            bIn_XYMoveFinishOnRotationKit=false;                                //Sam 20240726 : 修正移動完 XY 準備放 IC 到 RotationKit 時 Home
            bOut_XYMoveFinishOnRotationKit=false;

            if(CosFunction.bUseAfterHomeShtChkLoseICNeedSlow)                   //Sam 20201020 : 回 Home 第一次的 Shuttle Check IC lose 需要變慢。
            {
                bAfterHomeShtChkLoseICNeedSlow[0]=true;
                bAfterHomeShtChkLoseICNeedSlow[1]=true;
            }
            break;
        case 1200:
            if(fHome->fShow==false)
            {
                fHome->iHomeStep=1300;
                break;
            }
#ifdef SOFT_SIMULTE
            MOT[MTrayX].SetSpeed(100);
#else
            MOT[MTrayX].SetSpeed(15);
#endif
            if(ResetOKDeleyTime.Off() || fHome->fShow==false)
            {
                flag1 =false;
                flag2 =false;
                flag3 =false;                                                   //Steven 20110503
                flag4 =false;                                                   //Steven 20110503
                flag5 =false;
                flag6 =false;
                flag7 =false;                                                   //2013-04-12    Dell :旋轉站;馬達版
                flag8 =false;                                                   //2013-04-12    Dell :旋轉站;馬達版
                flag9 =false;                                                   //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
                flag10=false;
                PrePushLoaderCylinder(true);                                    //Steven 20150429 : 預先打兩下Loader汽缸
                fHome->iHomeStep=1250;

                MOT[MTestY1].iGali_SingalHomeTask=1;
                MOT[MTestY2].iGali_SingalHomeTask=1;
                MOT[MTestZ1].iGali_SingalHomeTask=1;
                MOT[MTestZ2].iGali_SingalHomeTask=1;
                MOT[MTestZ1].iGali_FindZPhaseTask[1]=1;                         //Ifor 20170817 (wei) add Z Phase Task
                MOT[MTestZ2].iGali_FindZPhaseTask[2]=1;                         //Ifor 20170817 (wei) add Z Phase Task
                MOT[MTestY1].iGali_FindZPhaseTask[0]=1;                         //Isaac 20201110 : Index Y find motor phase
                MOT[MTestY2].iGali_FindZPhaseTask[3]=1;                         //Isaac 20201110 : Index Y find motor phase

                if(USE_MAGNETIC_SCALE)
                {
                    MOT[MInArmXScale].Motor->ResetPos(0);                       //Steven 20160426 : 磁性尺
                    MOT[MInArmYScale].Motor->ResetPos(0);
                    MOT[MOutArmXScale].Motor->ResetPos(0);
                    MOT[MOutArmYScale].Motor->ResetPos(0);
                    MOT[MInArmXScale].HomeFlag=1;
                    MOT[MInArmYScale].HomeFlag=1;
                    MOT[MOutArmXScale].HomeFlag=1;
                    MOT[MOutArmYScale].HomeFlag=1;
                }
            }
            break;
        case 1250:
            #ifdef DEBUG_HOME                                                   //wei (Steven) 20170317 快速回Home開關
            for(int i=0; i<TOTAL_MOTOR; i++)                                    //kevin 20170414 (wei) add show motor pos
                fHome->ShowMotorHomePos(i);

            if(IndexY1==true && IndexZ1==true && IndexY2==true && IndexZ2==true)                                        //kevin 20161214 (Steven) Increase Hone Speed
            {
                fHome->iHomeStep=1260;
            }
            #else
            if(flag1==false)
            {
                if(MOT[MTestY1].Gali_SingalHome())
                {
                    flag1=true;
                    fHome->ListBox1->Items->Insert(0, "Index Y1 home finish.");
                }
            }

            if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                             //Jimmychiu 20230117 TestY2 not homing in 3 axes
            {
                if(flag2==false)
                {
                    if(MOT[MTestY2].Gali_SingalHome())
                    {
                        flag2=true;
                        fHome->ListBox1->Items->Insert(0, "Index Y2 home finish.");
                    }
                }
            }
            else
            {
                flag2=true;
            }

            if(flag3==false)
            {
                if(MOT[MTestZ1].Gali_SingalHome())
                {
                    flag3=true;
                    fHome->ListBox1->Items->Insert(0, "Index Z1 home finish.");
                }
            }

            if(flag4==false)
            {
                if(MOT[MTestZ2].Gali_SingalHome())
                {
                    flag4=true;
                    fHome->ListBox1->Items->Insert(0, "Index Z2 home finish.");
                }
            }

            if(flag1==true && flag2==true && flag3==true && flag4==true)
            {
                flag1=false;
                flag2=false;
                flag3=false;
                flag4=false;

                if(IniConfig.bD13CheckIndexHomeSensor)                          //Steven 20140828 : 歸零後檢查Index位置
                {
                    fHome->iHomeStep=2000;
                }
                else if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true)                                       //Isaac 20201110 : Index Y find motor phase
                {
                    if(IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess==true)
                    {
                        if(bZ1ModifyDistanceRef==true || bZ2ModifyDistanceRef==true)
                        {
                            fHome->iHomeStep=1300;
                        }
                        else
                        {
                            fHome->iHomeStep=3000;
                        }
                    }
                    else if(IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess==false &&
                            bFindMotorPhaseEveryGoHomeProcess==false)
                    {
                        if(bZ1ModifyDistanceRef==true || bZ2ModifyDistanceRef==true)
                        {
                            fHome->iHomeStep=1300;
                        }
                        else
                        {
                            fHome->iHomeStep=3000;
                        }
                    }
                    else
                    {
                        fHome->iHomeStep=1300;
                    }
                }
                else
                {
                    fHome->iHomeStep=1300;
                }

                ResetOKDeleyTime.SetMSAndOn(30000);                             //Kevin  20110525
            }
            #endif
            break;
        case 1260:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "ProcessMotorHome 1260"))                      //Steven 20140828 : 歸零後檢查Index位置    //JerryYang 20160905 10 --> Prod.TestZ1_Safe
            {
                fHome->iHomeStep=1270;
            }
            break;
        case 1270:
            if(MOT[MTestY1].GalilTwoY_Move(-10, 10, 60000, "ProcessMotorHome 1270"))                                    //Steven 20140828 : 歸零後檢查Index位置
            {
                if(IniConfig.bD13CheckIndexHomeSensor)                          //Steven 20140828 : 歸零後檢查Index位置
                {
                    fHome->iHomeStep=2000;
                }
                else if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true)                                       //Isaac 20201110 : Index Y find motor phase
                {
                    if(IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess==true)
                    {
                        if(bZ1ModifyDistanceRef==true || bZ2ModifyDistanceRef==true)
                        {
                            fHome->iHomeStep=1300;
                        }
                        else
                        {
                            fHome->iHomeStep=3000;
                        }
                    }
                    else if(IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess==false &&
                            bFindMotorPhaseEveryGoHomeProcess==false)
                    {
                        if(bZ1ModifyDistanceRef==true ||
                           bZ2ModifyDistanceRef==true)
                        {
                            fHome->iHomeStep=1300;
                        }
                        else
                        {
                            fHome->iHomeStep=3000;
                        }
                    }
                    else
                    {
                        fHome->iHomeStep=1300;
                    }
                }
                else
                {
                    fHome->iHomeStep=1300;
                }
                IndexY1=false;
                IndexY2=false;
                IndexZ1=false;
                IndexZ2=false;

                ResetOKDeleyTime.SetMSAndOn(30000);                             //Kevin 20110525
            }
            break;
        case 1300:                                                              //Steven 20200206 : 增加歸零的保護機制
            flag1 =false;
            flag2 =false;
            flag3 =false;                                                       //Steven 20110503
            flag4 =false;                                                       //Steven 20110503
            flag5 =false;
            flag6 =false;
            flag7 =false;                                                       //2013-04-12    Dell :旋轉站;馬達版
            flag8 =false;                                                       //2013-04-12    Dell :旋轉站;馬達版
            flag9 =false;                                                       //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
            flag10=false;
            for(int j=0; j<4; j++)                                              //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
                flag14[j]=false;
            for(int j=0; j<3; j++)                                              //Ifor 20251220 add:Boat Z
                flag18[j]=false;
            fHome->iHomeStep=1310;
        case 1310:                                                              //歸零完成，移動到等待位置
#ifdef SOFT_SIMULTE
            MOT[MInArmY].SetSpeed(100);
            MOT[MOutArmY].SetSpeed(100);
            MOT[MInShuttle1].SetSpeed(10);
            MOT[MInShuttle2].SetSpeed(10);
            MOT[MOutSortSht].SetSpeed(50);                                      //rogerYang 20250508 add for 9046AU
            MOT[MOutSortY].SetSpeed(100);
#else
            MOT[MInArmX].SetSpeed(10);
            MOT[MInArmY].SetSpeed(10);
            MOT[MOutArmX].SetSpeed(10);
            MOT[MOutArmY].SetSpeed(10);
            MOT[MInShuttle1].SetSpeed(10);
            MOT[MInShuttle2].SetSpeed(10);
            MOT[MTrayX].SetSpeed(15);
            FrmRotate->SetInRotateSpeed(70, 100);                               //Steven 20170425 (wei) : Add rotate motor
            FrmRotate->SetOutRotateSpeed(70, 100);
            MOT[MAOIKit].SetSpeed(20);                                          //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
            MOT[MMagazine].SetSpeed(100);                                       //JerryYang 20220909 : add magazine
            MOT[MCatchMgzTray].SetSpeed(100);                                   //JerryYang 20220909 : add magazine
            MOT[MOutSortSht].SetSpeed(10);                                      //rogerYang 20250508 add for 9046AU
            MOT[MOutSortX].SetSpeed(10);
            MOT[MOutSortY].SetSpeed(10);
            MOT[MInSh1LtcSenZ1].SetSpeed(10);                                   //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
            MOT[MInSh1LtcSenZ2].SetSpeed(10);
            MOT[MInSh2LtcSenZ1].SetSpeed(10);
            MOT[MInSh2LtcSenZ2].SetSpeed(10);

            if(USE_LdUldCassetteMode==1)
            {
                MOT[MTrayZ].SetSpeed(100);                                      //Ifor 20251220 add:Boat Z
                MOT[MAuto1Z].SetSpeed(100);
                MOT[MAuto2Z].SetSpeed(100);

                MOT[MLoaderY].SetSpeed(100);
                MOT[MAuto1Y].SetSpeed(100);
                MOT[MAuto2Y].SetSpeed(100);
                MOT[MLoaderY_CCW].SetSpeed(100);
                MOT[MAuto1Y_CCW].SetSpeed(100);
                MOT[MAuto2Y_CCW].SetSpeed(100);
            }

#endif
            if(INSTALL_OCR_YMot==eocrYMotInstal)                                //Frank 20250214 add
                MOT[MLoaderY].SetSpeed(100);

            if(MachineTypeChoice==Type_HT9046_LS)
                iPos+=5000;

            if(flag1==false)
                flag1=MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front_EndWaitPos, Prod.TestY2_Rear, 60000, "ProcessMotorHome 1310");                              //981118 jou Y1 +2000 easy change kit

            if(flag2==false)
            {
                if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
                    flag2=TrayArmMotorMove(Prod.iXTrayColor);
                else
                    flag2=TrayArmMotorMove(Prod.iXTrayEmpty);
            }

            if(flag3==false)
                flag3=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft+1000);     //Steven 20230109 : 修正Shuttle在正負一時可能會死雞
            if(flag4==false)
            {
                if(IniConfig.bVTESTFunction==true && fLotInfo->rgHomeStopPos->ItemIndex!=-1)
                {
                   if(fLotInfo->rgHomeStopPos->ItemIndex==1)
                      flag4=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight);
                   else
                      flag4=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft);
                }
                else
                {
                    flag4=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft+1000);
                }
            }
//#ifdef Carry4
//            if(flag5==false)
//                flag5=MOT[MOutShuttle1].MotorMove(Prod.OutSHT[0].iRight);
//            if(flag6==false)
//                flag6=MOT[MOutShuttle2].MotorMove(Prod.OutSHT[1].iRight);
//#else
//            flag5=true;                                                       //Frank 20250214 add
//            flag6=true;
//#endif
            if(USE_OUT_SORT_ARM!=eartUninstall)                                 //rogerYang 20250508 add for 9046AU
            {
                if(flag5==false)
                    flag5=MOT[MOutSortSht].MotorMove(Prod.SortSHT.iRight-1000);
            }
            else if(INSTALL_OCR_YMot==eocrYMotInstal)
            {
                if(MOT[MMTrayY].fHasTray)
                {
                    flag5=MOT[MLoaderY].MotorMove(Prod.iMLoaderYSurePos);
                }
                else
                {
                    #ifndef SOFT_SIMULTE
                    if(Sen[SnLoaderSureTray].IsOff()==false &&                  //RogerYang 20251029 : 修正Loader Y motor判斷有無tray可能被機構誤觸發
                       MOT[MLoaderY].Led[iHomeLed]==true &&
                       bIsYCarMoving==false)
                    {
                        ShowMyMessage("Loader carrier has tray , please remove that");
                    }
                    else
                    #endif
                    {
                        flag5=MOT[MLoaderY].MotorMove(Prod.iMLoaderYCarPos);
                        bIsYCarMoving=true;
                    }
                }
            }
            else
            {
                flag5=true;
            }
            flag6=true;

            if(USE_ROTATE_KIT==1 && (iRotate_Type==e1MotRotate ||
                                     iRotate_Type==e1MotRotate1Dut ||           //kevin 20130722  馬達版 ROTATE需歸零 //kevin 20130415
                                     iRotate_Type==eInOutArm1Motor))            //add One sucker with rotate
            {
                buffer =SetMotorResolution(Ang45, Ang90, true);                 //kevin 20131003
                flag7  =MOT[MInRotateKit].MotorMove(buffer);
                buffer =SetMotorResolution(Ang45, Ang90, false);                //kevin 20131003
                flag8  =MOT[MOutRotateKit].MotorMove(buffer);
            }
            else if(USE_ROTATE_KIT==1 && (iRotate_Type==e1MotRotate1Dut ||      //JerryYang 20230204 : fix rotate 1 motor 1 dut
                                          iRotate_Type==e4MotRotate ||
                                          iRotate_Type==e8MotRotate ||
                                          iRotate_Type==e2MotRotate2Dut))       //Steven 20170329 : Add individual rotate motor
            {
                flag7=MoveInRotateToDegreeAtSameTime(0);
                flag8=MoveOutRotateToDegreeAtSameTime(0);
            }
            else
            {
                flag7=true;                                                     //kevin 20130415
                flag8=true;
            }

            if(USE_AOI_Inspection)                                              //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
            {
                flag9=MOT[MAOIKit].MotorMove(Prod.iTopViewKit_Zup);             //20140918 wei  flag8修正為 flag9
            }
            else
            {
                flag9=true;                                                     //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
            }

            flag10=PrePushLoaderCylinder();                                     //Steven 20150429 : 預先打兩下Loader汽缸

            if(In_Shuttle_Auto_Latch==eInSHAutoLtc)                             //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
            {
                flag14[0]=MOT[MInSh1LtcSenZ1].MotorMove(Prod.iInSH1SenICDetectZ1+Prod.iInSH1SenICAddPos);
                flag14[1]=MOT[MInSh1LtcSenZ2].MotorMove(Prod.iInSH1SenICDetectZ2+Prod.iInSH1SenICAddPos);
                flag14[2]=MOT[MInSh2LtcSenZ1].MotorMove(Prod.iInSH2SenICDetectZ1+Prod.iInSH2SenICAddPos);
                flag14[3]=MOT[MInSh2LtcSenZ2].MotorMove(Prod.iInSH2SenICDetectZ2+Prod.iInSH2SenICAddPos);
            }
            else
            {
                for(int j=0; j<4; j++)
                    flag14[j]=true;
            }

            if(USE_LdUldCassetteMode==1)                                        //Ifor 20251220 add:Boat Z
            {
                flag18[0]=MOT[MTrayZ].MotorMove(9500);
                flag18[1]=MOT[MAuto1Z].MotorMove(9500);
                flag18[2]=MOT[MAuto2Z].MotorMove(9500);
            }
            else
            {
                for(int j=0; j<3; j++)
                    flag18[j]=true;
            }

            if(ResetOKDeleyTime.Off())                                          //Kevin  20110525 start
            {
                if(flag1==false)
                    sBuffer+=" MTestY1,";
                if(flag2==false)
                    sBuffer+=" MTrayX,";
                if(flag3==false)
                    sBuffer+=" MInShuttle1,";
                if(flag4==false)
                    sBuffer+=" MInShuttle2,";
                sBuffer+=" Home Time Out";
                HomeLog(sBuffer);
                fHome->iHomeStep=1;
                break;
            }

            if(flag1 && flag2 && flag3 && flag4 &&                              //kevin 20130415
               flag5 && flag6 && flag7 && flag8 &&
               flag9 &&                                                         //20140918  wei  增加  flag9
               flag10 &&                                                        //Steven 20150429 : 預先打兩下Loader汽缸
               flag14[0] && flag14[1] && flag14[2] && flag14[3] &&              //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
               flag18[0] && flag18[1] && flag18[2])                             //Ifor 20251220 add: Boat Z
            {
                bIsYCarMoving=false;                                            //RogerYang 20251029 : 修正Loader Y motor判斷有無tray可能被機構誤觸發
                #ifndef SOFT_SIMULTE
                if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
                    iTrayArmPos=Prod.iXTrayColor;
                else
                    iTrayArmPos=Prod.iXTrayEmpty;

                MOT[MTrayX].ScanMotorStatus();
                if(MOT[MTrayX].Motor->Enable)
                {
                    if(bCyflag[0]==false)
                        bCyflag[0]=(Cylinder[C_TrayXFloodgate1].Enable==false || Cylinder[C_TrayXFloodgate1].Pop());
                    if(bCyflag[1]==false)
                        bCyflag[1]=(Cylinder[C_TrayXFloodgate2].Enable==false || Cylinder[C_TrayXFloodgate2].Pop());
                    if(bCyflag[2]==false)
                        bCyflag[2]=(Cylinder[C_TrayXFloodgate3].Enable==false || Cylinder[C_TrayXFloodgate3].Pop());
                    if(bCyflag[3]==false)
                        bCyflag[3]=(Cylinder[C_TrayXFloodgate4].Enable==false || Cylinder[C_TrayXFloodgate4].Pop());

                    if(!(bCyflag[0] && bCyflag[1] && bCyflag[2] && bCyflag[3]))
                        break;

                    if(((iTrayArmPos-100>=MOT[MTrayX].ReadEncoderPos()) &&
                        (MOT[MTrayX].ReadEncoderPos()>=iTrayArmPos+100)) ||
                         MOT[MTrayX].Led[iHomeLed])
                    {
                        ShowMyMessage("Tray arm is not at safe position", "Tray Arm不在安全位置上");
                        fHome->fAbort=false;
                        SystemStart=false;
                        fHome->iHomeStep=1;
                        SoftStop=true;
                        break;
                    }
                }
                #endif
                flag1=false;
                flag2=false;
                flag3=false;
                flag4=false;                                                    //RogerYang 20250510 Add for 9046AU
                fHome->iHomeStep=1520;
            }
            break;
        case 1520:
            #ifdef DEBUG_HOME                                                   //wei (Steven) 20170317 快速回Home開關
                flag1=true;                                                     //Steven 20260612 : Fix == to = (was comparison, not assignment)
            #else
            if(flag1==false)                                                    //Steven 20230109 : 修正Shuttle在正負一時可能會死雞
                flag1=MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "ProcessMotorHome 1520");               //JerryYang 20160905 10 --> Prod.TestZ1_Safe
            #endif
            if(flag2==false)
                flag2=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft);
            if(flag3==false)
                flag3=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft);

            if(USE_OUT_SORT_ARM!=eartUninstall)                                 //rogerYang 20250508 add for 9046AU
            {
                if(flag4==false)
                    flag4=MOT[MOutSortSht].MotorMove(Prod.SortSHT.iRight);
            }
            else
                flag4=true;

            if(flag1 && flag2 && flag3 && flag4)                                //rogerYang 20250508 add for 9046AU
            {
                flag1=false;
                flag2=false;
                flag3=false;
                flag4=false;                                                    //wei 20161206 Auto Shuttle Sensor 回Home
                flag5=false;
                flag6=false;                                                    //RogerYang 20250510 Add for 9046AU
                bAutoShuttleHome=true;                                          //wei 20161206 Auto Shuttle Sensor 回Home
                fHome->iHomeStep=1530;
            }
            break;
        case 1530:                                                              //Steven 20200206 : 增加歸零的保護機制
            if(INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR)
            {
                if(MOT[MTrayX].ReadEncoderPos()<=Prod.iXTrayColor-100)
                {
                    fHome->iHomeStep=1300;
                    break;
                }
            }
            else
            {
                if(MOT[MTrayX].ReadEncoderPos()<=Prod.iXTrayEmpty-100)
                {
                    fHome->iHomeStep=1300;
                    break;
                }
            }
            InitialMagazineUpDown();                                            //JerryYang 20220909 : add magazine
            fHome->iHomeStep=1540;
        case 1540:
            if(flag1==false)
            {
                if(IniConfig.bVTESTFunction==true && fLotInfo->rgHomeStopPos->ItemIndex==0)
                    flag1=MoveInArm2XYToShuttle2Wait();                         //Steven 20110719 : In Arm不要呆在加熱盤上面拷
                else
                    flag1=MOT[MInArmX].MotorMove(Prod.XInArm_Tray_Pick[0][2]+6000);                                     //Steven 20110719 : In Arm不要呆在加熱盤上面拷
            }

            if(flag3==false)                                                    //Steven 20110719 : In Arm不要呆在加熱盤上面拷
            {
                if(IniConfig.bVTESTFunction==true && fLotInfo->rgHomeStopPos->ItemIndex==0)
                    flag3=true;
                else
                    flag3=MOT[MInArmY].MotorMove(Prod.YInArm_Tray_Pick[0][2]-10000);
            }

            if(flag2==false)
            {
                if(IniConfig.bVTESTFunction==true &&
                   fLotInfo->rgHomeStopPos->ItemIndex==1)
                    flag2=MoveOutArm2XYToShuttle2Wait();
                else
                    flag2=MoveOutArmXY_ToFix_Tray_Full();
            }

            if(AUTO_SENSOR_INSTALL)                                             //wei 20161206 Auto Shuttle Sensor 回Home
            {
                if(flag4==false)
                {
                    flag4=DoMoveShuttleSensor();
                }
            }
            else
            {
                flag4=true;
            }

            if(AUTO3_IS_MAGAZINE==1)                                            //JerryYang 20220909 : add magazine
            {
                flag5=DoMagazineUpDown(0, 3);
            }
            else
            {
                flag5=true;
            }

            if(USE_OUT_SORT_ARM!=eartUninstall)                                 //rogerYang 20250508 add for 9046AU
            {
                if(flag6==false)
                {
                    flag6=MoveSortArmXYToSortShtWait();
                }
            }
            else
                flag6=true;

            if(flag1 && flag2 &&                                                //JerryYang 20220909 : add magazine
               flag3 && flag4 && flag5 &&                                       //Steven 20110719 : In Arm不要呆在加熱盤上面拷
               flag6)                                                           //RogerYang 20250510 Add for 9046AU
            {
                flag1=false;
                flag2=false;
                bAutoShuttleHome=false;                                         //wei 20161206 Auto Shuttle Sensor 回Home
                fHome->iHomeStep=1545;
            }
            break;
        case 1545:
            if(AUTO3_IS_MAGAZINE==1)                                            //JerryYang 20220909 : add magazine
            {
                flag1=CatchMgzTrayMove(Prod.iCatchMazTray_Rear);
            }
            else
            {
                flag1=true;
            }

            if(flag1)
            {
                flag1=false;
                fHome->iHomeStep=1550;
            }
            break;
        case 1550:
            if(fHome->fShow)
                fHome->Close();
            fHome->iHomeStep=1600;
            break;
        case 1600:
            for(int i=0; i<TOTAL_MOTOR; i++)
            {
                MOT[i].fCanMove=true;
                MOT[i].fCanMoveL=true;
                MOT[i].fCanMoveM=true;
                MOT[i].fCanMoveR=true;
            }

            IndexStatus=0;
            fHome->iHomeStep=1;
            SoftStart=false;
            TestSocket.ClearAll()  ;
            if(FTestSuck.HasRealIC()==false)    FTestSuck.ClearAll();
            if(BTestSuck.HasRealIC()==false)    BTestSuck.ClearAll();
            if(FLCarryKit.HasRealIC()==false)   FLCarryKit.ClearAll();
            if(BLCarryKit.HasRealIC()==false)   BLCarryKit.ClearAll();
            if(FRCarryKit.HasRealIC()==false)   FRCarryKit.ClearAll();
            if(BRCarryKit.HasRealIC()==false)   BRCarryKit.ClearAll();
            if(ShuttleHasIC() || FTestSuck.UseSiteHasIC() || BTestSuck.UseSiteHasIC())
            {
                iOneCycle=1;
                fMain->DebugOneCycleHotPlate("ProcessMotorHome");               //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log
                _bHomeNeedOnecycle=true;
            }
            else
            {
                iOneCycle=0;
                _bHomeNeedOnecycle=false;
            }
            IndexStatus=Z1_Z2_Normal;
            SetInitialICCheck();

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20150507
            {
                if(MOT[MTrayX].fHasTray==false &&
                   USE_AUTO_RETEST==eartInstall &&
                   Cylinder[C_CatchTray_FixOn].OnStatus())                      //kevin 20150616
                {
                     ShowErrorMessage("WAR0614", K_RETRY, MTrayX);              //手動取下tray    //wei 20150805 WAR0612==>WAR0614
                     return false;
                }
            }

            if(LastSet.iRealDummy!=DUMMY &&
               LastSet.iRunStartMode!=rsmAutoRetest)                            //ChungHung 20140625 AutoRetest 不能鎖
            {
                if(MOT[MMTrayY].fHasTray)
                {
                    if(TRAY_ARM_MODE==eUnderCoveyor)
                    {
                        if(USE_OUT_SORT_ARM!=eartUninstall)                     //rogerYang 20250722 add for 9046AU
                        {
                            Cylinder[C_TrayY_Fixer].On();
                            Cylinder[C_LoaderEdgePush].On();
                        }
                        else
                        {
                            Cylinder[C_LoaderPushBack_Back].On();
                            Cylinder[C_LoaderPushBack_Push].Off();
                        }
                    }
                    else
                    {
                        Cylinder[C_TrayY_Fixer].On();
                        Cylinder[C_LoaderEdgePush].On();
                    }
                }

                if(MOT[MMAuto1].fHasTray)
                {
                    Cylinder[C_Auto1Side_Fixer].On();
                    Cylinder[C_Auto1EdgePush].On();
                    Cylinder[C_Auto1UpPress].On();                              //JerryYang 20190423 新增unloader壓tray
                    if(USE_OUT_SORT_ARM!=eartUninstall)                         //rogerYang 20250722 add for 9046AU
                    {
                        Cylinder[C_Auto1Separate].Pop();
                    }
                }

                if(MOT[MMAuto2].fHasTray)
                {
                    Cylinder[C_Auto2Side_Fixer].On();
                    Cylinder[C_Auto2EdgePush].On();
                    Cylinder[C_Auto2UpPress].On();
                    if(USE_OUT_SORT_ARM!=eartUninstall)                         //rogerYang 20250722 add for 9046AU
                    {
                        Cylinder[C_Auto2Separate].Pop();
                    }
                }

                if(MOT[MMAuto3].fHasTray)
                {
                    Cylinder[C_Auto3Side_Fixer].On();
                    Cylinder[C_Auto3EdgePush].On();
                    Cylinder[C_Auto3UpPress].On();
                    if(USE_OUT_SORT_ARM!=eartUninstall)                         //rogerYang 20250722 add for 9046AU
                    {
                        Cylinder[C_Auto3Separate].Pop();
                    }
                }

                if(MOT[MMAuto4].fHasTray)                                       //Steven 20230907 : For HT-9011UC
                {
                    Cylinder[C_Auto4Side_Fixer].On();
                    Cylinder[C_Auto4EdgePush].On();
                    Cylinder[C_Auto4UpPress].On();
                    if(USE_OUT_SORT_ARM!=eartUninstall)                         //rogerYang 20250722 add for 9046AU
                    {
                        Cylinder[C_Auto4Separate].Pop();
                    }
                }

                if(MOT[MMAuto5].fHasTray)
                {
                    Cylinder[C_Auto5Side_Fixer].On();
                    Cylinder[C_Auto5EdgePush].On();
                    Cylinder[C_Auto5UpPress].On();
                    if(USE_OUT_SORT_ARM!=eartUninstall)                         //rogerYang 20250722 add for 9046AU
                    {
                        Cylinder[C_Auto5Separate].Pop();
                    }
                }

                if(MOT[MMAuto6].fHasTray)
                {
                    Cylinder[C_Auto6Side_Fixer].On();
                    Cylinder[C_Auto6EdgePush].On();
                    Cylinder[C_Auto6UpPress].On();
                    if(USE_OUT_SORT_ARM!=eartUninstall)                         //rogerYang 20250722 add for 9046AU
                    {
                        Cylinder[C_Auto6Separate].Pop();
                    }
                }
            }
            PitchCylinderState[0]=0;
            PitchCylinderState[1]=0;
            PitchCylinderState[2]=0;
            StopAllMotor();                                                     //Steven 20220309 : 拿掉false

            Zteach->InitAutoPosTask();                                          //Frank 20171213 (Steven) : Hone 需Inital Task
            fHome->RotateCheckClear();
            InitialIndexAutoCleanTask();                                        //kevin 20120601 Autoclean
            MyDBIProcessNew("Motion", "WAR2208", "Do process motor home finish.");                                      //jou 2010-11-23
            HomeLog("Close");                                                   //Kevin  20110525
            if(iAseHome!=0)                                                     //kevin 20150925
            {
                iAseHome=0;
                RespondASECom("@e02104Done");                                   //kevin 20150415 回應 ase Home finish
            }

            bHomeUnlock=false;                                                  //kevin 20131218
            SetAutoSkipCount(0);                                                //Steven 20150217 : 顯示Auto Skip的數量, 1=++, 0=清空計數
            fMain->SendMSG_CMD(MSG_CMD_HandlerHomeFinish);                      //Steven 20150304 : Add GPIB LOG

            if(USE_Scanner_AOI_Inspection &&                                    //Richard 20220817:Add Scanner_AOI add check Big Size IC Function
               ScannerAOIIF.bEnabledPositionByAOI==false)                       //Ifor 20260515 add: Move Position Provided By AOI    GetGrabPos AOI      
            {
                FrmAOI->SendCommand("@GetGrabPos+", 0);                         //Expect Return@GetGrabPos,PitchXCnt,PitchYCnt,PitchX,PitchY +
            }

            if(TestIF_File.bEnableBarCode)                                      //Steven 20160823 : 歸零後也要清空蝦頭的2DID
            {
                fBarCode->InitialBarcodeScanInShuttle1();
                fBarCode->InitialBarcodeScanInShuttle2();
            }

            if(SHT_FLOATING_CHK==1 && TestIF_File.bEnableShtFloatChk)           //Steven 20160920 : IC置偏檢查
            {
                fBarCode->InitialShuttleFloatCheck1();
                fBarCode->InitialShuttleFloatCheck2();
                fBarCode->InitialSFCAutoTune1(false);
                fBarCode->InitialSFCAutoTune2(false);
                fBarCode->SetSFCCheckStepCount();
                Str.sprintf("E9,1,%d", fBarCode->iSFCTotalMoveStep);
                fBarCode->SendCCDCommand(fBarCode->iBarCode1_1, "Clear buffer", Str);
                fBarCode->SendCCDCommand(fBarCode->iBarCode1_2, "Clear buffer", Str);
                fBarCode->SendCCDCommand(fBarCode->iBarCode2_1, "Clear buffer", Str);
                fBarCode->SendCCDCommand(fBarCode->iBarCode2_2, "Clear buffer", Str);
            }

            //==> Eastsun 20260526 #026-4.PinN.P-N6 Pin1 mode switch :KYEC
            if(CosFunction.b2DUsePinInspection) //Ifor 20230531 add:2D/Pin1 模式切換
            {
                if(TestIF_File.b2DUsePinInspection==true)
                {
                    Str="@FNPN+";
                    fBarCode->SendCCDCommand(fBarCode->iBarCode1_1, "Switch To Pin1.", Str);
                }
                else
                {
                    Str="@FN2D+";
                    fBarCode->SendCCDCommand(fBarCode->iBarCode1_1, "Switch To 2D.", Str);
                }
            }
            //<== Eastsun 20260526 #026-4.PinN.P-N6

            if(INSTALL_OCR!=eocrUninstal || BAR_CODE_INSTALL!=ebctUninstall)    //kevin 20211101 home check 2D use
            {
                if(TestIF_File.bOcrFunction || TestIF_File.bEnableBarCode)      //Steven 20160912 : 避免沒開啟2D function
                    fMain->SendMSG_CMD(MSG_CMD_EnableBarCode);
                else
                    fMain->SendMSG_CMD(MSG_CMD_DisableBarCode);

                if(TestIF_File.b2DUsePinInspection)                             //Ifor 20240528 add:Pin1 Function
                    fMain->SendMSG_CMD(MSG_CMD_EnablePin1Function);
                else
                    fMain->SendMSG_CMD(MSG_CMD_DisablePin1Function);
            }

            if(iResetStopMachine==1)                                            //Frank 20260710 ADD//Eastsun 20260710整合
                iResetStopMachine=2;

            if(IniConfig.bVTESTFunction==true)
                fLotInfo->rgHomeStopPos->ItemIndex=-1;
            if(CosFunction.bFTPFunction)                                        //Ifor 20231101 add:FTP Function
            {
                if(IniConfig.bEnableFTP==true)
                {
                    fMain->SendMSG_CMD(MSG_CMD_EnableFTPFunction);
                }
                else
                {
                    fMain->SendMSG_CMD(MSG_CMD_DisableFTPFunction);
                }
            }
            return true;
        case 2000:                                                              //Steven 20140828 Start: 歸零後檢查Index位置
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "ProcessMotorHome 2000"))                      //JerryYang 20160905 10 --> Prod.TestZ1_Safe
                fHome->iHomeStep=2010;
            break;
        case 2010:
            if(MOT[MTestY1].GalilTwoY_Move(-10, 10, 60000, "ProcessMotorHome 2010"))
            {
                fHome->iHomeStep=2020;
                ResetOKDeleyTime.SetMSAndOn(200);                               //JerryYang 20160418 100->200,延長delay 時間,避免已經移動到home點卻沒偵測到
            }
            break;
        case 2020:
            if(ResetOKDeleyTime.Off())
            {
                MOT[MTestZ1].ScanMotorStatus();
                MOT[MTestZ2].ScanMotorStatus();
                MOT[MTestY1].ScanMotorStatus();
                MOT[MTestY2].ScanMotorStatus();
                fHome->iHomeStep=2050;
            }
            break;
        case 2050:
            flag1=true;

            fHome->ListBox1->Items->Insert(0, "Start check Index home sensor...");
        #ifndef SOFT_SIMULTE                                                    //JerryYang 20170202 (Steven) 避免軟體模擬時啟用D13造成無法回home
            if(MOT[MTestZ1].Led[iHomeLed]==false)
            {
                MNetLog("Index Z1 not at home! (2000)");
                flag1=false;
            }

            if(MOT[MTestZ2].Led[iHomeLed]==false)
            {
                MNetLog("Index Z2 not at home! (2000)");
                flag1=false;
            }

            if(MOT[MTestY1].Led[iHomeLed]==false)
            {
                MNetLog("Index Y1 not at home! (2000)");
                flag1=false;
            }

            if(MOT[MTestY2].Led[iHomeLed]==false)
            {
                MNetLog("Index Y2 not at home! (2000)");
                flag1=false;
            }
        #endif
            if(flag1==false)
            {
                RecordProcess("Index find home fail, try again! (2000)");
                fHome->iHomeStep=1;
            }
            else
            {
                fHome->iHomeStep=2060;                                          //kevin 20210908 index arm 沒有在 Shuttle Up
            }
            break;
        case 2060:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Rear, 60000, "ProcessMotorHome 1310"))        //981118 jou Y1 +2000 easy change kit
                fHome->iHomeStep=2100;                                          //kevin 20210908 index arm 沒有在 Shuttle Up
            break;
        case 2100:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(-10, 30000, "ProcessMotorHome 2100"))                                   //Steven 20140828
            {
                flag1=true;
                ResetOKDeleyTime.SetMSAndOn(100);
                fHome->iHomeStep=2150;
            }
            break;
        case 2150:
            if(ResetOKDeleyTime.Off())
            {
                fHome->ListBox1->Items->Insert(0, "Check Index Z must off");
                MOT[MTestZ1].ScanMotorStatus();
                MOT[MTestZ2].ScanMotorStatus();
                MOT[MTestY1].ScanMotorStatus();
                MOT[MTestY2].ScanMotorStatus();
            #ifndef SOFT_SIMULTE                                                //JerryYang 20170202 (Steven) 避免軟體模擬時啟用D13造成無法回home
                if(MOT[MTestZ1].Led[iHomeLed]==true)
                {
                    MNetLog("Index Z1 not away home! (2100)");
                    flag1=false;
                }

                if(MOT[MTestZ2].Led[iHomeLed]==true)
                {
                    MNetLog("Index Z2 not away home! (2100)");
                    flag1=false;
                }
            #endif
                if(flag1==false)
                {
                    RecordProcess("Index find home fail, try again! (2100)");
                    fHome->iHomeStep=1;
                }
                else
                {
                    ResetOKDeleyTime.SetMSAndOn(100);
                    fHome->iHomeStep=2200;
                }
            }
            break;
        case 2200:
            if(ResetOKDeleyTime.Off() && MOT[MTestZ1].Gali_Two_ZAxis_Move(-500, 30000, "ProcessMotorHome 2200"))
            {
                fHome->ListBox1->Items->Insert(0, "Check Index Z must on");
                ResetOKDeleyTime.SetMSAndOn(100);
                fHome->iHomeStep=2300;
            }
            break;
        case 2300:
            if(ResetOKDeleyTime.Off() && MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "ProcessMotorHome 2300"))
            {
                ResetOKDeleyTime.SetMSAndOn(200);
                fHome->iHomeStep=2400;
            }
            break;
        case 2400:
            if(ResetOKDeleyTime.Off())
            {
                flag1=true;
                MOT[MTestZ1].ScanMotorStatus();
                MOT[MTestZ2].ScanMotorStatus();
                MOT[MTestY1].ScanMotorStatus();
                MOT[MTestY2].ScanMotorStatus();
            #ifndef SOFT_SIMULTE                                                //JerryYang 20170202 (Steven) 避免軟體模擬時啟用D13造成無法回home
                if(MOT[MTestZ1].Led[iHomeLed]==false)
                {
                    MNetLog("Index Z1 not at home! (2400)");
                    flag1=false;
                }

                if(MOT[MTestZ2].Led[iHomeLed]==false)
                {
                    MNetLog("Index Z2 not at home! (2400)");
                    flag1=false;
                }
            #endif
                if(flag1==false)
                {
                    RecordProcess("Index find home fail, try again! (2400)");
                    fHome->iHomeStep=1;
                }
                else
                {
                    fHome->ListBox1->Items->Insert(0, "Check Index Z Ok");
                    fHome->ListBox1->Items->Insert(0, "Check Index Y must off");
                    fHome->iHomeStep=2500;
                }
            }
            break;
        case 2500:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "ProcessMotorHome 2500"))                      //JerryYang 20160905 10 --> Prod.TestZ1_Safe
            {
                fHome->iHomeStep=2600;
            }
            break;
        case 2600:
            if(MOT[MTestY1].GalilTwoY_Move(10, -10, 60000, "ProcessMotorHome 2600"))
            {
                flag1=true;
                ResetOKDeleyTime.SetMSAndOn(200);
                fHome->iHomeStep=2650;
            }
            break;
        case 2650:
            if(ResetOKDeleyTime.Off())
            {
                MOT[MTestZ1].ScanMotorStatus();
                MOT[MTestZ2].ScanMotorStatus();
                MOT[MTestY1].ScanMotorStatus();
                MOT[MTestY2].ScanMotorStatus();
            #ifndef SOFT_SIMULTE                                                //JerryYang 20170202 (Steven) 避免軟體模擬時啟用D13造成無法回home
                if(MOT[MTestY1].Led[iHomeLed]==true)
                {
                    MNetLog("Index Y1 not away home! (2600)");
                    flag1=false;
                }

                if(MOT[MTestY2].Led[iHomeLed]==true)
                {
                    MNetLog("Index Y2 not away home! (2600)");
                    flag1=false;
                }
            #endif
                if(flag1==false)
                {
                    RecordProcess("Index find home fail, try again! (2600)");
                    fHome->iHomeStep=1;
                }
                else
                {
                    ResetOKDeleyTime.SetMSAndOn(100);
                    fHome->iHomeStep=2700;
                }
            }
            break;
        case 2700:
            if(ResetOKDeleyTime.Off() && MOT[MTestY1].GalilTwoY_Move(500, -500, 60000, "ProcessMotorHome 2700"))
            {
                fHome->ListBox1->Items->Insert(0, "Check Index Y must on");
                ResetOKDeleyTime.SetMSAndOn(100);
                fHome->iHomeStep=2800;
            }
            break;
        case 2800:
            if(ResetOKDeleyTime.Off() && MOT[MTestY1].GalilTwoY_Move(-10, 10, 60000, "ProcessMotorHome 2800"))
            {
                ResetOKDeleyTime.SetMSAndOn(200);
                fHome->iHomeStep=2900;
            }
            break;
        case 2900:
            if(ResetOKDeleyTime.Off())
            {
                flag1=true;
                MOT[MTestZ1].ScanMotorStatus();
                MOT[MTestZ2].ScanMotorStatus();
                MOT[MTestY1].ScanMotorStatus();
                MOT[MTestY2].ScanMotorStatus();
            #ifndef SOFT_SIMULTE                                                //JerryYang 20170202 (Steven) 避免軟體模擬時啟用D13造成無法回home
                if(MOT[MTestY1].Led[iHomeLed]==false)
                {
                    MNetLog("Index Y1 not at home! (2900)");
                    flag1=false;
                }

                if(MOT[MTestY2].Led[iHomeLed]==false)
                {
                    MNetLog("Index Y2 not at home! (2900)");
                    flag1=false;
                }
            #endif
                if(flag1==false)
                {
                    RecordProcess("Index find home fail, try again! (2900)");
                    fHome->iHomeStep=1;
                }
                else
                {
                    fHome->ListBox1->Items->Insert(0, "Check Index Y OK");
                    if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true)                                        //Isaac 20201110 : Index Y find motor phase
                    {
                        if(IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess==true)
                        {
                            if(bZ1ModifyDistanceRef==true || bZ2ModifyDistanceRef==true)
                            {
                                fHome->iHomeStep=1300;
                            }
                            else
                            {
                                fHome->iHomeStep=3000;
                            }
                        }
                        else if(IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess==false &&
                                bFindMotorPhaseEveryGoHomeProcess==false)
                        {
                            if(bZ1ModifyDistanceRef==true || bZ2ModifyDistanceRef==true)
                            {
                                fHome->iHomeStep=1300;
                            }
                            else
                            {
                                fHome->iHomeStep=3000;
                            }
                        }
                        else
                        {
                            fHome->iHomeStep=1300;
                        }
                    }
                    else
                    {
                        fHome->iHomeStep=1300;
                    }
                    ResetOKDeleyTime.SetMSAndOn(30000);
                }
            }
            break;
        case 3000:                                                              //Ifor 20170817 (wei) add Find Index Z Phase Start
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, 30000, "ProcessMotorHome 3000"))                      //移動至安全位置
            {
                fHome->iHomeStep=3050;                                          //Isaac 20201110 : Index Y find motor phase
                Index_Z1_FindZPosition=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z1_Home_Position", 120);        //Ifor 讀取到find phase的預備位置
                Index_Z2_FindZPosition=ReadIniData(asGeneralPath, "IndexDriver", "Index_Z2_Home_Position", 120);

                if(Index_Z1_FindZPosition>0)
                    Index_Z1_FindZPosition=-(Index_Z1_FindZPosition-10);
                if(Index_Z2_FindZPosition>0)
                    Index_Z2_FindZPosition=-(Index_Z2_FindZPosition-10);

                if(Index_Z1_FindZPosition<-3000)
                {
                    ShowMyMessage("Index Z1 pos is too low!Plz set Index_Z1_Home_Position as 120 in Gerneral.ini file", "Index Z1 馬達尋相開始位置過低!請至Gerneral.ini 回復Index_Z1_Home_Position參數為120");
                    return false;
                }

                if(Index_Z2_FindZPosition<-3000)
                {
                    ShowMyMessage("Index Z2 pos is too low!Plz set Index_Z2_Home_Position as 120 in Gerneral.ini file", "Index Z2 馬達尋相開始位置過低!請至Gerneral.ini 回復Index_Z2_Home_Position參數為120");
                    return false;
                }
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            break;
        case 3050:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle_Home, Prod.TestY2_Rear, 400000, "ProcessMotorHome 3050"))                                         //kevin 20171012 (wei) add home    //Ifor 20170822 Middle 需使用Teach位置避免關ARM 數值為0   60000=>400000 //Isaac 20201110 : Index Y find motor phase
            {
                fHome->iHomeStep=3060;
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            break;
        case 3060:                                                              //Isaac 20201110 : Index Y find motor phase
            if(MOT[MTestZ1].Gali_MotMoveNoWait(Index_Z1_FindZPosition, 50000, 0))                                       //移動上一次紀錄位置-20 30000 =>50000
            {
                fHome->iHomeStep=3100;
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            break;
        case 3100:
            if(MOT[MTestZ1].Gali_FindZPhase())
            {
                fHome->ListBox1->Items->Insert(0, "Index Z1 Phase finish.");
                fHome->iHomeStep=3250;
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            if(fHome->HomeClass[MTestZ1]->Visible==true &&
               atoi(fHome->HomeClass[MTestZ1]->edPos->Text.c_str())>3000)       //Ifor 20170907 add Z Phase 保護避免過低壓到Socket
            {
                ShowErrorMessage("WAR0307", K_RETRY, MMSystem);
            }
            break;
        case 3250:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle_Home, 400000, "ProcessMotorHome 3250"))                                        //kevin 20171012 (wei) add home  //Ifor 20170822 Middle 需使用Teach位置避免關ARM 數值為0     //Isaac 20201110 : Index Y find motor phase，60000->400000
            {
                fHome->iHomeStep=3260;
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            break;
        case 3260:                                                              //Isaac 20201110 : Index Y find motor phase
            if(MOT[MTestZ2].Gali_MotMoveNoWait(Index_Z2_FindZPosition, 50000, 0))                                       //移動上一次紀錄位置-20 30000 =>50000
            {
                fHome->iHomeStep=3300;
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            break;
        case 3300:
            if(MOT[MTestZ2].Gali_FindZPhase())
            {
                fHome->ListBox1->Items->Insert(0, "Index Z2 Phase finish.");
                fHome->iHomeStep=3400;
                ResetOKDeleyTime.SetMSAndOn(30000);
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            if(fHome->HomeClass[MTestZ2]->Visible==true &&
               atoi(fHome->HomeClass[MTestZ2]->edPos->Text.c_str())>3000)       //Ifor 20170907 add Z Phase 保護避免過低壓到Socket
            {
                ShowErrorMessage("WAR0308", K_RETRY, MMSystem);
            }
            break;
        case 3400:
            if(MOT[MTestY1].GalilTwoY_Move(0, 0, 400000, "ProcessMotorHome 3400"))                                      //Isaac 20201110 : Index Y find motor phase
            {
                if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true)    //Isaac : Find Y phase
                {
                    Index_Y1_FindZPosition=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position", 20);
                    Index_Y2_FindZPosition=ReadIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position", 20);

                    if(Index_Y1_FindZPosition<0)
                    {
                        Index_Y1_FindZPosition=-Index_Y1_FindZPosition;
                    }

                    if(Index_Y2_FindZPosition>0)
                    {
                        Index_Y2_FindZPosition=-Index_Y2_FindZPosition;
                    }
                    Index_Y1_FindZPosition=(Index_Y1_FindZPosition-10);
                    Index_Y2_FindZPosition=(Index_Y2_FindZPosition+10);

                    if(Index_Y1_FindZPosition<=-20 || Index_Y1_FindZPosition>=3000)
                    {
                        ShowMyMessage("Index Y1 file is not correct!Plz set Index_Y1_Home_Position as 20 in Gerneral.ini file", "Index Y1 馬達尋相開始位置異常!請至Gerneral.ini 回復Index_Y1_Home_Position參數為20");
                        return false;
                    }

                    if(Index_Y2_FindZPosition>=20 || Index_Y2_FindZPosition<=-3000)
                    {
                        ShowMyMessage("Index Y2 file is not correct!Plz set Index_Y2_Home_Position as 20 in Gerneral.ini file", "Index Y2 馬達尋相開始位置異常!請至Gerneral.ini 回復Index_Y2_Home_Position參數為20");
                        return false;
                    }
                    fHome->iHomeStep=3600;
                    ResetOKDeleyTime.SetMSAndOn(200);
                }
                else
                {
                    fHome->iHomeStep=1300;
                    ResetOKDeleyTime.SetMSAndOn(30000);
                }
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            break;
        case 3600:                                                              //Isaac 20201110 : Index Y find motor phase
            if(ResetOKDeleyTime.Off())
            {
                if(MOT[MTestY1].GalilTwoY_Move(Index_Y1_FindZPosition, Index_Y2_FindZPosition, 60000, "ProcessMotorHome 3600"))
                {
                    fHome->iHomeStep=3610;
                    fHome->ListBox1->Items->Insert(0, "Index Y Start Find Phase.");
                    flag12=false;
                    flag13=false;
                }
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            break;
        case 3610:                                                              //Isaac 20201110 : Index Y find motor phase
            if(flag12==false)
                flag12=MOT[MTestY1].Gali_FindZPhase();
            if(flag13==false)
                flag13=MOT[MTestY2].Gali_FindZPhase();

            if(flag12==true && flag13==true)
            {
                fHome->ListBox1->Items->Insert(0, "Index Y1 and Y2 Phase Both Finish.");
                bFindMotorPhaseEveryGoHomeProcess=true;
                fHome->iHomeStep=1300;
                ResetOKDeleyTime.SetMSAndOn(30000);
                flag12=false;
                flag13=false;
            }
            fHome->ShowMotorHomePos(MTestZ1);
            fHome->ShowMotorHomePos(MTestZ2);
            fHome->ShowMotorHomePos(MTestY1);
            fHome->ShowMotorHomePos(MTestY2);
            if(fHome->HomeClass[MTestY1]->Visible==true &&
               atoi(fHome->HomeClass[MTestY1]->edPos->Text.c_str())>4000)       //Ifor 20170907 add Z Phase 保護避免過低壓到Socket
            {
                ShowErrorMessage("WAR03314", K_RETRY, MMSystem);
            }

            if(fHome->HomeClass[MTestY2]->Visible==true &&
               atoi(fHome->HomeClass[MTestY2]->edPos->Text.c_str())<-4000)      //Ifor 20170907 add Z Phase 保護避免過低壓到Socket
            {
                ShowErrorMessage("WAR03315", K_RETRY, MMSystem);
            }
            break;
    }
    return false;
}
//==============================================================================
void __fastcall TfHome::FormShow(TObject *Sender)
{
    Panel2->Visible=false;
    fShow=true;
    fAbort=false;
}
//------------------------------------------------------------------------------
void __fastcall TfHome::RotateCheckClear()
{
    //ChungHung 20110922 : 轉轉蝦頭要檢查有沒有轉頭 Check Sensor
    if(IniConfig.bHaveRotateShuttle==true && IniConfig.bRotateShNeedSensorCheck && TestIF.bRotateShuttle)
    {
        //清除SnRotateCheck
        SW[SwRotateCheckClear].On();
        MySleep(100);
        SW[SwRotateCheckClear].Off();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfHome::SpeedButton1Click(TObject *Sender)
{
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TfHome::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    Timer1->Enabled=false;
}
//------------------------------------------------------------------------------
void __fastcall TfHome::ScanKey()
{
    int Key;
    Key=ScanPannelKey();
    if(Key==SnFKPause)
    {
        sbAbortHomeClick(this);
        Close();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfHome::Timer1Timer(TObject *Sender)
{
    if(fShow==false) return;
    ScanKey();
}
//------------------------------------------------------------------------------
void  __fastcall TfHome::InitDoTestZHome()
{
    TestZTask=1;
    MOT[MTestY1].MovFlag=false;
    MOT[MTestY2].MovFlag=false;
    MOT[MTestZ1].MovFlag=false;
    MOT[MTestZ2].MovFlag=false;
    MOT[MTestY1].bScanFlag=false;
    MOT[MTestY2].bScanFlag=false;
    MOT[MTestZ1].bScanFlag=false;
    MOT[MTestZ2].bScanFlag=false;
    MOT[MTestY1].GaliSofDelayCount=0;
    MOT[MTestY2].GaliSofDelayCount=0;
    MOT[MTestZ1].GaliSofDelayCount=0;
    MOT[MTestZ2].GaliSofDelayCount=0;
}
//------------------------------------------------------------------------------
//long __fastcall TfHome::GetHomePitch(int GetMot)
//{
//    int &Task=TestZTask;
//    switch(Task)
//    {
//        case 1:
//            if(GetMot==MTestY1)
//                MOT[MTestY1].Gali_Command("DP0;DE0", __FUNC__);
//            else if(GetMot==MTestY2)
//                MOT[MTestY1].Gali_Command("DP,,,0;DE,,,0", __FUNC__);
//            else
//                MOT[MTestY1].Gali_Command("DP,0,0;DE,0,0", __FUNC__);
//            Task=50;
//            break;
//        case 50:
//            if(GetMot==MTestY1)
//            {
//                MOT[MTestY1].Gali_MotHome("X");
//                if(MOT[MTestY1].Gali_Command("MG_BGx", __FUNC__)==1)
//                    Task=100;
//            }
//            else if(GetMot==MTestY2)
//            {
//                MOT[MTestY1].Gali_MotHome("W");
//                if(MOT[MTestY1].Gali_Command("MG_BGw", __FUNC__)==1)
//                    Task=100;
//            }
//            else
//            {
//                MOT[MTestZ1].Gali_MotHome("YZ");   //Do_Z1_Z2Home
//                if(MOT[MTestZ1].Gali_Command("MG_BGy", __FUNC__)==1 || MOT[MTestZ1].Gali_Command("MG_BGz", __FUNC__)==1)
//                    Task=100;
//            }
//            break;
//        case 100:
//            if(GetMot==MTestY1)
//            {
//                if(MOT[MTestY1].Gali_Command("MG_BGx", __FUNC__)==0)
//                {
//                    MOT[MTestY1].MovFlag=false;
//                    return MOT[MTestY1].Gali_Command("TPX", __FUNC__);    //TPX or TDX
//                }
//            }
//            else if(GetMot==MTestY2)
//            {
//                if(MOT[MTestY1].Gali_Command("MG_BGw", __FUNC__)==0)
//                {
//                    MOT[MTestY2].MovFlag=false;
//                    return MOT[MTestY1].Gali_Command("TPW", __FUNC__);    //TPX or TDX
//                }
//            }
//            else
//            {
//                if(MOT[MTestY1].Gali_Command("MG_BGy", __FUNC__)==0 && MOT[MTestY1].Gali_Command("MG_BGz", __FUNC__)==0)
//                {
//                    if(GetMot==MTestZ1)
//                    {
//                        MOT[MTestZ1].MovFlag=false;
//                        return -MOT[MTestY1].Gali_Command("TPY", __FUNC__);    //TPY or TDY
//                    }
//                    else
//                    {
//                        MOT[MTestZ2].MovFlag=false;
//                        return -MOT[MTestY1].Gali_Command("TPZ", __FUNC__);    //TPZ or TDZ
//                    }
//                }
//            }
//            break;
//    }
//    return 0;
//}
//------------------------------------------------------------------------------
void __fastcall TfHome::sbAbortHomeClick(TObject *Sender)
{
    GaliMotorServoOff("sbAbortHomeClick");                                      //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
    fAbort=true;
    sbAbortHome->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void TfHome::GaliMotorServoOff(AnsiString sFunc)                                //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
{
    StopAllMotor();
    InitGali_HomeTask();
    MOT[MTestY1].Gali_Command("ST", __FUNC__);
    MOT[MTestY1].MovFlag=false;
    MOT[MTestY2].MovFlag=false;
    MOT[MTestZ1].MovFlag=false;
    MOT[MTestZ2].MovFlag=false;
    MOT[MTestY1].bScanFlag=false;
    MOT[MTestY2].bScanFlag=false;
    MOT[MTestZ1].bScanFlag=false;
    MOT[MTestZ2].bScanFlag=false;
    MOT[MTestY1].GaliSofDelayCount=0;
    MOT[MTestZ1].GaliSofDelayCount=0;
    MOT[MTestZ2].GaliSofDelayCount=0;
    MOT[MTestY2].GaliSofDelayCount=0;
    SystemStart=false;
    SW[SwMotorRelay].Off();
    SW[SwServerON].Off();
    bMotorPowerState=false;
    IndexMotorBreakerOFF();
    MagazineBreakerOFF();                                                       //JerryYang 20220909 : add magazine
    InOutArmZBreakerOFF();                                                      //add One sucker with rotate
    LDCarRotArmZBreakerOFF();                                                   //RogerYang 20250828 add for Loader Rotate Arm
    CassetteBreakerOFF();                                                       //Ifor 20251216 add:Boat Carrier
    fAllMotorHome=false;
    RecordProcess(AnsiString("GaliMotorServoOff - ")+sFunc);
}
//---------------------------------------------------------------------------
//Ztex 2023.12.15 Add Pitch X Home Twice ==>
bool TfHome::MoveInArmPitch_X(int *iSetPos, bool bCheckHome)
{
    #ifdef SOFT_SIMULTE
        return true;
    #endif

    bool bResult=false;
    bool bFalg[4]={false, false, false, false};
    static bool bFirstIn=true;                                                  //Ztex 2024.01.14 Modify Pitch X Move Pos Error Issue

    long lPosition[4]={MOT[MInArmPitch  ].Motor->ReadPos(),
                       MOT[MInArmPitchX2].Motor->ReadPos(),
                       MOT[MInArmPitchX3].Motor->ReadPos(),
                       MOT[MInArmPitchX4].Motor->ReadPos()};

//    long lTarget[4]={iSetPos,iSetPos,iSetPos,iSetPos};

    BYTE bDevNo[4]={MOT[MInArmPitch  ].Motor->iPortID,
                    MOT[MInArmPitchX2].Motor->iPortID,
                    MOT[MInArmPitchX3].Motor->iPortID,
                    MOT[MInArmPitchX4].Motor->iPortID};

    MOT[MInArmPitch].ScanMotorStatus();
    MOT[MInArmPitchX2].ScanMotorStatus();
    MOT[MInArmPitchX3].ScanMotorStatus();
    MOT[MInArmPitchX4].ScanMotorStatus();
    int iDevPos[4];

    if(bCheckHome==true)
    {
        if(MOT[MInArmPitch].Led[iHomeLed]==true || MOT[MInArmPitch].MotorMove(MOT[MInArmPitch].Motor->PSoftLimitP-10)==0)
        {
            MOT[MInArmPitch].Motor->Stop();
            bFalg[0]=true;
        }

        if(MOT[MInArmPitchX2].Led[iHomeLed]==true || MOT[MInArmPitchX2].MotorMove(MOT[MInArmPitchX2].Motor->PSoftLimitP-10)==0)
        {
            MOT[MInArmPitchX2].Motor->Stop();
            bFalg[1]=true;
        }

        if(MOT[MInArmPitchX3].Led[iHomeLed]==true || MOT[MInArmPitchX3].MotorMove(MOT[MInArmPitchX3].Motor->PSoftLimitP-10)==0)
        {
            MOT[MInArmPitchX3].Motor->Stop();
            bFalg[2]=true;
        }

        if(MOT[MInArmPitchX4].Led[iHomeLed]==true || MOT[MInArmPitchX4].MotorMove(MOT[MInArmPitchX4].Motor->PSoftLimitP-10)==0)
        {
            MOT[MInArmPitchX4].Motor->Stop();
            bFalg[3]=true;
        }

        if(bFalg[0]==true && bFalg[1]==true && bFalg[2]==true && bFalg[3]==true)
            bResult=true;
    }
    else
    {
        if(bFirstIn==true)                                                      //Ztex 2024.01.14 Modify Pitch X Move Pos Error Issue
        {
            bFirstIn=false;
            iDevPos[0]=iSetPos[0]-lPosition[0];
            iDevPos[1]=iSetPos[1]-lPosition[1];
            iDevPos[2]=iSetPos[2]-lPosition[2];
            iDevPos[3]=iSetPos[3]-lPosition[3];
            MOT[MInArmPitch  ].GetRealPos(&iDevPos[0]);
            MOT[MInArmPitchX2].GetRealPos(&iDevPos[1]);
            MOT[MInArmPitchX3].GetRealPos(&iDevPos[2]);
            MOT[MInArmPitchX4].GetRealPos(&iDevPos[3]);
            long lDevPos[4]={iDevPos[0], iDevPos[1], iDevPos[2], iDevPos[3]};

            MOT[MInArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);
        }

        if(MOT[MInArmPitch].Motor->MotionDone())
            bResult=true;
        else
            bResult=false;
    }

    if(bResult==true)
        bFirstIn=true;

    return bResult;
}
//---------------------------------------------------------------------------
bool TfHome::MoveOutArmPitch_X(int *iSetPos, bool bCheckHome)
{
    bool bResult=false;
    bool bFalg[4]={false, false, false, false};
    static bool bFirstIn=true;                                                  //Ztex 2024.01.14 Modify Pitch X Move Pos Error Issue
    #ifdef SOFT_SIMULTE
        return true;
    #endif
    long lPosition[4]={MOT[MOutArmPitch  ].Motor->ReadPos(),
                       MOT[MOutArmPitchX2].Motor->ReadPos(),
                       MOT[MOutArmPitchX3].Motor->ReadPos(),
                       MOT[MOutArmPitchX4].Motor->ReadPos()};

//    long lTarget[4]={iSetPos[0],iSetPos[1],iSetPos[2],iSetPos[3]};

    BYTE bDevNo[4]={MOT[MOutArmPitch  ].Motor->iPortID,
                    MOT[MOutArmPitchX2].Motor->iPortID,
                    MOT[MOutArmPitchX3].Motor->iPortID,
                    MOT[MOutArmPitchX4].Motor->iPortID};

    MOT[MOutArmPitch].ScanMotorStatus();
    MOT[MOutArmPitchX2].ScanMotorStatus();
    MOT[MOutArmPitchX3].ScanMotorStatus();
    MOT[MOutArmPitchX4].ScanMotorStatus();
    int iDevPos[4];

    if(bCheckHome==true)
    {
        if(MOT[MOutArmPitch].Led[iHomeLed]==true || MOT[MOutArmPitch].MotorMove(MOT[MOutArmPitch].Motor->PSoftLimitP-10)==0)
        {
            MOT[MOutArmPitch].Motor->Stop();
            bFalg[0]=true;
        }

        if(MOT[MOutArmPitchX2].Led[iHomeLed]==true || MOT[MOutArmPitchX2].MotorMove(MOT[MOutArmPitchX2].Motor->PSoftLimitP-10)==0)
        {
            MOT[MOutArmPitchX2].Motor->Stop();
            bFalg[1]=true;
        }

        if(MOT[MOutArmPitchX3].Led[iHomeLed]==true || MOT[MOutArmPitchX3].MotorMove(MOT[MOutArmPitchX3].Motor->PSoftLimitP-10)==0)
        {
            MOT[MOutArmPitchX3].Motor->Stop();
            bFalg[2]=true;
        }

        if(MOT[MOutArmPitchX4].Led[iHomeLed]==true || MOT[MOutArmPitchX4].MotorMove(MOT[MOutArmPitchX4].Motor->PSoftLimitP-10)==0)
        {
            MOT[MOutArmPitchX4].Motor->Stop();
            bFalg[3]=true;
        }

        if(bFalg[0]==true && bFalg[1]==true && bFalg[2]==true && bFalg[3]==true)
            bResult=true;
    }
    else
    {
        if(bFirstIn==true)                                                      //Ztex 2024.01.14 Modify Pitch X Move Pos Error Issue
        {
            bFirstIn=false;
            iDevPos[0]=iSetPos[0]-lPosition[0];
            iDevPos[1]=iSetPos[1]-lPosition[1];
            iDevPos[2]=iSetPos[2]-lPosition[2];
            iDevPos[3]=iSetPos[3]-lPosition[3];
            MOT[MOutArmPitch  ].GetRealPos(&iDevPos[0]);
            MOT[MOutArmPitchX2].GetRealPos(&iDevPos[1]);
            MOT[MOutArmPitchX3].GetRealPos(&iDevPos[2]);
            MOT[MOutArmPitchX4].GetRealPos(&iDevPos[3]);
            long lDevPos[4]={iDevPos[0], iDevPos[1], iDevPos[2], iDevPos[3]};

            MOT[MOutArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);
        }

        if(MOT[MOutArmPitch].Motor->MotionDone())
            bResult=true;
        else
            bResult=false;
    }

    if(bResult==true)
        bFirstIn=true;

    return bResult;
}
//Ztex 2023.12.15 Add Pitch X Home Twice <==
//---------------------------------------------------------------------------
