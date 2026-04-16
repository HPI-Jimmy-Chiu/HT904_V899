#include "MachineDefine.h"
#pragma hdrstop

#include "uMotorTest.h"

#include "cmydef.h"
#include "cprod.h"
#include "mymotor.h"
#include "uhome.h"
//#include "input.h"
#include "cpublic.h"
#include "cinitial.h"
#include "mymessbox.h"
#include "note.h"
#include "myswitch.h"
#include "mysensor.h"
#include "uteach.h"
#include "csystem.h"
#include "common.h"
#include "OCR.h"
//#include "CMNET.h"
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"
TfMotorTest *fMotorTest;
int iRotaterOldPos;
int iRotaterNewPos,TargetPos;
bool bRotaterPosReady=false;
//------------------------------------------------------------------------------
TMotorTestClass::TMotorTestClass(int MotNo, bool bView) : TComponent(Owner)     //Steven 20120831 : MotorTest元件改用Vector新增
{
    AnsiString MotName;
    index           =MotNo;
    Visible         =bView;
    AllMotMoveFlag  =true;

    MotName.sprintf("M%02d", MotNo);

    if(Visible==false)                                                          //Steven 20240822 : 沒用到的馬達不new
        return;

    labName =new TLabel(fMotorTest->pnlMotor);
    edPos1  =new TEdit(fMotorTest->pnlMotor);
    edPos2  =new TEdit(fMotorTest->pnlMotor);
    cbUsing =new TCheckBox(fMotorTest->pnlMotor);

    labName ->Parent        =fMotorTest->pnlMotor;
    labName ->Name          ="labName"+AnsiString(MotNo);
    labName ->Color         =TColor(0x00C2B8A6);
    labName ->Height        =18;
    labName ->Width         =31;
    labName ->Font->Color   =clBlack;
    labName ->Font->Size    =10;
    labName ->Font->Name    ="Arial";
    labName ->Caption       =MotName;
    labName ->OnClick       =fMotorTest->lM00Click;
    labName ->Visible       =Visible;
    labName ->Tag           =index;

    edPos1  ->Parent    =fMotorTest->pnlMotor;
    edPos1  ->Name      ="edPos1_"+AnsiString(MotNo);
    edPos1  ->Height    =24;
    edPos1  ->Width     =65;
    edPos1  ->Text      =0;
    edPos1  ->OnClick   =fMotorTest->lpA00Click;
    edPos1  ->Visible   =Visible;
    edPos1  ->Tag       =index;

    edPos2  ->Parent    =fMotorTest->pnlMotor;
    edPos2  ->Name      ="edPos2_"+AnsiString(MotNo);
    edPos2  ->Height    =24;
    edPos2  ->Width     =65;
    edPos2  ->Text      =0;
    edPos2  ->OnClick   =fMotorTest->lpA00Click;
    edPos2  ->Visible   =Visible;
    edPos2  ->Tag       =index;

    cbUsing ->Parent    =fMotorTest->pnlMotor;
    cbUsing ->Name      ="cbUsing"+AnsiString(MotNo);
    cbUsing ->Height    =17;
    cbUsing ->Width     =15;
    cbUsing ->Caption   ="";
    cbUsing ->Visible   =Visible;
    cbUsing ->Tag       =index;
    cbUsing ->OnClick   =fMotorTest->ck00Click;

    MotorPanel =new TPanel(fMotorTest->pnlMotor);
    MotorPanel->Parent        =fMotorTest->pnlMotor;
    MotorPanel->Name          ="MotorPanel"+AnsiString(MotNo);
    MotorPanel->Color         =TColor(0x00C2B8A6);
    MotorPanel->Height        =19;
    MotorPanel->Width         =469;
    MotorPanel->BevelOuter    =bvNone;
    MotorPanel->Caption       ="";
    MotorPanel->Visible       =Visible;
    MotorPanel->Tag           =index;

    int iYTop=3;
    MotorLabel=new TLabel(MotorPanel);
    MotorLabel->Parent      =MotorPanel;
    MotorLabel->Top         =iYTop;
    MotorLabel->Left        =4;
    MotorLabel->Font->Size  =9;
    MotorLabel->Font->Name  ="MS Scan Serief";
    MotorLabel->Caption     =MotName;

    MotorHomeLed            =new TALed(MotorPanel);
    MotorHomeLed->Parent    =MotorPanel;
    MotorHomeLed->Top       =iYTop;
    MotorHomeLed->Left      =46;
    MotorHomeLed->LEDStyle  =LEDHorizontal;
    MotorHomeLed->Blink     =false;
    MotorHomeLed->Value     =false;
    MotorHomeLed->TrueColor =clGreen;
    for(int i=0; i<10; i++)
    {
        MotorLed[i]=new TALed(MotorPanel);
        MotorLed[i]->Parent     =MotorPanel;
        MotorLed[i]->Top        =iYTop;
        MotorLed[i]->Left       =46+39+i*39;
        MotorLed[i]->LEDStyle   =LEDHorizontal;
        MotorLed[i]->Blink      =false;
        MotorLed[i]->Value      =false;
        MotorLed[i]->TrueColor  =clRed;
    }
}
//---------------------------------------------------------------------------
void TMotorTestClass::SetPosition(int Top, int Left)
{
    if(Visible==false)                                                          //Steven 20240822 : 沒用到的馬達不new
        return;

    cbUsing ->Top    =Top;
    cbUsing ->Left   =Left;
    labName ->Top    =Top;
    labName ->Left   =Left+16;
    edPos1  ->Top    =Top-3;
    edPos1  ->Left   =Left+50;
    edPos2  ->Top    =Top-3;
    edPos2  ->Left   =Left+117;
}
//---------------------------------------------------------------------------
__fastcall TfMotorTest::TfMotorTest(TComponent* Owner)
    : TForm(Owner)
{
    AnsiString str="";
    ActiveIndex=-1;

    MotorTestClass.push_back(new TMotorTestClass(MInArmX));
    MotorTestClass.push_back(new TMotorTestClass(MInArmY));
    MotorTestClass.push_back(new TMotorTestClass(MInArmPitch));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZA));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZB, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZC, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZD, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZE, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZF, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZG, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZH, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MInShuttle1));
    MotorTestClass.push_back(new TMotorTestClass(MInShuttle2));
    MotorTestClass.push_back(new TMotorTestClass(MTestY1));
    MotorTestClass.push_back(new TMotorTestClass(MTestZ1));
    MotorTestClass.push_back(new TMotorTestClass(MTestZ2));
    MotorTestClass.push_back(new TMotorTestClass(MTestY2));
//  #ifdef Carry4
//    MotorTestClass.push_back(new TMotorTestClass(MOutShuttle1));
//    MotorTestClass.push_back(new TMotorTestClass(MOutShuttle2));
//  #else
    MotorTestClass.push_back(new TMotorTestClass(MOutShuttle1,   false));
    MotorTestClass.push_back(new TMotorTestClass(MOutShuttle2,   false));
//  #endif
    MotorTestClass.push_back(new TMotorTestClass(MOutArmX));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmY));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmPitch));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZA));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZB, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZC, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZD, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZE, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZF, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZG, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZH, (InOutArmPickerUseMotor==eptUseMot)));
    MotorTestClass.push_back(new TMotorTestClass(MTrayX));
    MotorTestClass.push_back(new TMotorTestClass(MInArmPitchY,   (USE_IN_Y_IS_AUTO_PITCH==true))); //Steven 20131002 : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    MotorTestClass.push_back(new TMotorTestClass(MInArmPitchX2,  (USE_IN_Y_IS_AUTO_PITCH==true)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmPitchY,  (USE_OUT_Y_IS_AUTO_PITCH==true))); //Steven 20131002 : XY變距
    MotorTestClass.push_back(new TMotorTestClass(MOutArmPitchX2, (USE_OUT_Y_IS_AUTO_PITCH==true)));
    MotorTestClass.push_back(new TMotorTestClass(MLoaderZ,       LOAD_Z_USE_MOTOR[0]));
    MotorTestClass.push_back(new TMotorTestClass(MEmptyZ,        LOAD_Z_USE_MOTOR[1]));
    MotorTestClass.push_back(new TMotorTestClass(MColorZ,        LOAD_Z_USE_MOTOR[2]));
    MotorTestClass.push_back(new TMotorTestClass(MAuto1Z,       (LOAD_Z_USE_MOTOR[3] || USE_LdUldCassetteMode==1)));    //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MAuto2Z,       (LOAD_Z_USE_MOTOR[4] || USE_LdUldCassetteMode==1)));    //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MAuto3Z,        LOAD_Z_USE_MOTOR[5]));
    MotorTestClass.push_back(new TMotorTestClass(MInRotateKit,   (USE_ROTATE_KIT && iRotate_Type!=eCynRotate)));         //2013-04-12    Dell :旋轉站;馬達版
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateKit,  (USE_ROTATE_KIT && iRotate_Type!=eCynRotate && iRotate_Type!=e4MotRotate && iRotate_Type!=e2MotRotate2Dut)));         //2013-04-12    Dell :旋轉站;馬達版
    MotorTestClass.push_back(new TMotorTestClass(MAOIKit,        USE_AOI_Inspection));       //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    MotorTestClass.push_back(new TMotorTestClass(MLoaderY,       ((INSTALL_OCR_YMot==eocrYMotInstal && CosFunction.bTrayOCR) || //RogerYang 20250903 Fix For 殘料檢    //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
                                                                   USE_LdUldCassetteMode==1 ||                                       //Ifor 20251216 add:Boat Carrier
                                                                   LOAD_Y_USE_MOTOR[0]==true)));

    MotorTestClass.push_back(new TMotorTestClass(MEmptyY,        false));
    MotorTestClass.push_back(new TMotorTestClass(MColorY,        false));
    MotorTestClass.push_back(new TMotorTestClass(MAuto1Y,        USE_LdUldCassetteMode==1));    //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MAuto2Y,        USE_LdUldCassetteMode==1));    //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MAuto3Y,        false));

    MotorTestClass.push_back(new TMotorTestClass(MInArmZAe,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));       //Steven 20230323 : For HT1032  //Steven 20260316 : Fix operator precedence
    MotorTestClass.push_back(new TMotorTestClass(MInArmPitchX3,  (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmPitchX4,  (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZAf,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));  //Steven 20260316 : Fix operator precedence
    MotorTestClass.push_back(new TMotorTestClass(MOutArmPitchX3, (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmPitchX4, (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)));
    MotorTestClass.push_back(new TMotorTestClass(MTrayZ,         (TRAY_ARM_MODE==eUnderCoveyor || USE_LdUldCassetteMode==1)));  //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MOutSortAa,     (USE_OUT_SORT_ARM==iOutSortX40mm)));   //Steven 20240822 : For HT-9046AU
    MotorTestClass.push_back(new TMotorTestClass(MOutSortAb,     (USE_OUT_SORT_ARM==iOutSortX40mm)));
    MotorTestClass.push_back(new TMotorTestClass(MInArmXScale,   USE_MAGNETIC_SCALE));
    MotorTestClass.push_back(new TMotorTestClass(MInArmYScale,   USE_MAGNETIC_SCALE));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmXScale,  USE_MAGNETIC_SCALE));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmYScale,  USE_MAGNETIC_SCALE));
    MotorTestClass.push_back(new TMotorTestClass(MShuttle1Pitch, AUTO_SENSOR_INSTALL));   //wei 20160914 Auto Shuttle Sensor
    MotorTestClass.push_back(new TMotorTestClass(MShuttle2Pitch, AUTO_SENSOR_INSTALL));   //wei 20160914 Auto Shuttle Sensor
    //Steven 20170329 (Wei) : Add individual rotate motor
    //==>
    MotorTestClass.push_back(new TMotorTestClass(MInRotateB,  (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    MotorTestClass.push_back(new TMotorTestClass(MInRotateC,  (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    MotorTestClass.push_back(new TMotorTestClass(MInRotateD,  (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    MotorTestClass.push_back(new TMotorTestClass(MInRotateE,  (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    MotorTestClass.push_back(new TMotorTestClass(MInRotateF,  (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    MotorTestClass.push_back(new TMotorTestClass(MInRotateG,  (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    MotorTestClass.push_back(new TMotorTestClass(MInRotateH,  (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateB, (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateC, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateD, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateE, (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateF, (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateG, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    MotorTestClass.push_back(new TMotorTestClass(MOutRotateH, (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    //<==
    //Steven 20170329 (Wei) : Add individual rotate motor

    MotorTestClass.push_back(new TMotorTestClass(MLightScale,    USE_TRAY_ROBOT));
//    MotorTestClass.push_back(new TMotorTestClass(MTrayRobotX,    USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MTrayRobotY,    USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MTrayRobotZ,    USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
    MotorTestClass.push_back(new TMotorTestClass(MInArmZAg,       (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZAh,       (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));

    MotorTestClass.push_back(new TMotorTestClass(MArmAlignment,       false));
    MotorTestClass.push_back(new TMotorTestClass(MLoadHingeR,    USE_LOADER_HINGE)); //Steven 20170330 (Wei) : For TSMC
    MotorTestClass.push_back(new TMotorTestClass(MLoadHingeZ,    USE_LOADER_HINGE)); //Steven 20170330 (Wei) : For TSMC
    MotorTestClass.push_back(new TMotorTestClass(MPreciser,      USE_PRECISER));     //Frank 20180410 (Steven) : InArm Preciser Station

//    MotorTestClass.push_back(new TMotorTestClass(MSortX,         USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortY,         USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortPitch,     USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortPitch2,    USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortPitchY,    USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZA,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZB,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZC,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZD,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZE,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZF,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZG,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM
//    MotorTestClass.push_back(new TMotorTestClass(MSortZH,        USE_TRAY_ROBOT));   //Steven 20170330 (Wei) : For HT-9046LM

//   MotorTestClass.push_back(new TMotorTestClass("M99",  MCaselevatorZ,     USE_MR_SYSTEM));   //wei 20180702 MR
//    MotorTestClass.push_back(new TMotorTestClass(MLoadRobotZ,       USE_MR_SYSTEM));   //wei 20180702 MR
    MotorTestClass.push_back(new TMotorTestClass(MInArmZBe,       (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZBf,       (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZBg,       (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MInArmZBh,       (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZAe,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZAf,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZAg,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZAh,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZBe,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZBf,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZBg,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));
    MotorTestClass.push_back(new TMotorTestClass(MOutArmZBh,      (InOutArmPickerUseMotor==eptUseMot && (USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be))));

    MotorTestClass.push_back(new TMotorTestClass(MOutSortX,       (USE_OUT_SORT_ARM==iOutSortX40mm)));      //Steven 20240822 : For HT-9046AU
    MotorTestClass.push_back(new TMotorTestClass(MCaselevatorZ,   USE_MR_SYSTEM));   //Sam 20190112 LM

    MotorTestClass.push_back(new TMotorTestClass(MCasArmX,        USE_MR_SYSTEM));   //wei 20180702 MR
    MotorTestClass.push_back(new TMotorTestClass(MCasArmZ,        USE_MR_SYSTEM));   //wei 20180702 MR
    MotorTestClass.push_back(new TMotorTestClass(MTrayBracketZ,   USE_MR_SYSTEM));   //wei 20180702 MR
    MotorTestClass.push_back(new TMotorTestClass(MStackedTrayX,   USE_MR_SYSTEM));   //wei 20180702 MR
    MotorTestClass.push_back(new TMotorTestClass(MStackedTrayZ,   USE_MR_SYSTEM));   //wei 20180702 MR
    MotorTestClass.push_back(new TMotorTestClass(MUnloadRobotZ,   USE_MR_SYSTEM));   //wei 20180702 MR

    MotorTestClass.push_back(new TMotorTestClass(MOutSortY,       (USE_OUT_SORT_ARM==iOutSortX40mm)));      //Steven 20240822 : For HT-9046AU

    MotorTestClass.push_back(new TMotorTestClass(MCCDX, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(MCCDY, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(MCCDZ, USE_FINE_PITCH));

    MotorTestClass.push_back(new TMotorTestClass(MInFlipper1,  USE_DEVICE_FLIPPER));    //Frank 20210612 : Flipper Function
    MotorTestClass.push_back(new TMotorTestClass(MInFlipper2,  USE_DEVICE_FLIPPER));
    MotorTestClass.push_back(new TMotorTestClass(MInFlipper3,  USE_DEVICE_FLIPPER));
    MotorTestClass.push_back(new TMotorTestClass(MOutFlipper1, USE_DEVICE_FLIPPER));
    MotorTestClass.push_back(new TMotorTestClass(MOutFlipper2, USE_DEVICE_FLIPPER));
    MotorTestClass.push_back(new TMotorTestClass(MOutFlipper3, USE_DEVICE_FLIPPER));

    MotorTestClass.push_back(new TMotorTestClass(MLdCarRotArm, USE_LD_Rot_Arm));     //RogerYang 20250828 add for Loader Rotate Arm    //Steven 20210624 : Fine Pitch
    MotorTestClass.push_back(new TMotorTestClass(MLoaderY_CCW, USE_LdUldCassetteMode==1));  //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MAuto1Y_CCW , USE_LdUldCassetteMode==1));  //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MAuto2Y_CCW , USE_LdUldCassetteMode==1));  //Ifor 20251216 add:Boat Carrier
    MotorTestClass.push_back(new TMotorTestClass(MAuto3Y_CCW , false));
    MotorTestClass.push_back(new TMotorTestClass(MAuto4Y_CCW , false));
    MotorTestClass.push_back(new TMotorTestClass(MAuto5Y_CCW , false));
    MotorTestClass.push_back(new TMotorTestClass(MAuto6Y_CCW , false));
    MotorTestClass.push_back(new TMotorTestClass(M1_3R, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_4X, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_4Y, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_4R, USE_FINE_PITCH));

    MotorTestClass.push_back(new TMotorTestClass(M1_5X, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_5Y, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_5R, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_6X, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_6Y, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_6R, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_7X, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_7Y, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_7R, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_8X, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_8Y, USE_FINE_PITCH));
    MotorTestClass.push_back(new TMotorTestClass(M1_8R, USE_FINE_PITCH));

    MotorTestClass.push_back(new TMotorTestClass(MMagazine, (AUTO3_IS_MAGAZINE==1)));        //JerryYang 20220909 : add magazine
    MotorTestClass.push_back(new TMotorTestClass(MCatchMgzTray, (AUTO3_IS_MAGAZINE==1)));    //JerryYang 20220909 : add magazine
    MotorTestClass.push_back(new TMotorTestClass(MMagYTrayOut, false));
    MotorTestClass.push_back(new TMotorTestClass(MFix3Full, (FIX3_FULL_PLACE==Fix3K_UseStepperMotor))); //Steven 20221122 : Add Fix3 Motor

    MotorTestClass.push_back(new TMotorTestClass(MAuto4Z, LOAD_Z_USE_MOTOR[6]));//Steven 20230907 : For HT-9011UC
    MotorTestClass.push_back(new TMotorTestClass(MAuto5Z, LOAD_Z_USE_MOTOR[7]));
    MotorTestClass.push_back(new TMotorTestClass(MAuto6Z, LOAD_Z_USE_MOTOR[8]));
    MotorTestClass.push_back(new TMotorTestClass(MAuto4Y, false));
    MotorTestClass.push_back(new TMotorTestClass(MAuto5Y, false));
    MotorTestClass.push_back(new TMotorTestClass(MAuto6Y, false));

    MotorTestClass.push_back(new TMotorTestClass(MTopAOIArmX  , false));
    MotorTestClass.push_back(new TMotorTestClass(MTopAOIArmY  , false));
    MotorTestClass.push_back(new TMotorTestClass(MTopAOIArmR  , false));
    MotorTestClass.push_back(new TMotorTestClass(MTopAOICCDZ  , false));
    MotorTestClass.push_back(new TMotorTestClass(MTopAOIElevZ1, false));
    MotorTestClass.push_back(new TMotorTestClass(MTopAOIElevZ2, false));

    MotorTestClass.push_back(new TMotorTestClass(MOutSortPitchX, (USE_OUT_SORT_ARM==iOutSortX40mm)));       //Steven 20240822 : For HT-9046AU
    MotorTestClass.push_back(new TMotorTestClass(MOutSortSht, (USE_OUT_SORT_ARM==iOutSortX40mm)));
    MotorTestClass.push_back(new TMotorTestClass(MLoad2Z, USE_2nd_LOADER));                              //RogerYang 20250828 add
    MotorTestClass.push_back(new TMotorTestClass(MLoad2Y, false));                                       //RogerYang 20250828 add for Loader Rotate Arm

    MotorTestClass.push_back(new TMotorTestClass(MInSh1LtcSenZ1, In_Shuttle_Auto_Latch));   //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    MotorTestClass.push_back(new TMotorTestClass(MInSh1LtcSenZ2, In_Shuttle_Auto_Latch));   //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    MotorTestClass.push_back(new TMotorTestClass(MInSh2LtcSenZ1, In_Shuttle_Auto_Latch));   //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    MotorTestClass.push_back(new TMotorTestClass(MInSh2LtcSenZ2, In_Shuttle_Auto_Latch));   //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料

    int iLPitch =192,   iTPitch =28;                                            //調整畫面位置
    int iLeft   =4,     iTop    =4;
    int iMaxRowItem=12;
    int ColItem=0, RowItem=0;

    for(int i=0; i<TOTAL_MOTOR; i++)                                            //Steven 20240322 : 按照馬達編號顯示
    {
        for(unsigned int j=0; j<MotorTestClass.size(); j++)
        {
            if(MotorTestClass[j]->index==i &&                                   //Steven 20240322 : 按照馬達編號顯示
               MotorTestClass[j]->Visible)
            {
                MotorTestClass[j]->SetPosition(iTop+iTPitch*RowItem, iLeft+iLPitch*ColItem);
                RowItem++;

                if(RowItem>=iMaxRowItem)
                {
                    RowItem=0;
                    ColItem++;
                }
            }
        }
    }

    strngrdMotor->ColWidths[0] = strngrdMotor->DefaultColWidth+20;
    strngrdMotor->ColWidths[1] = strngrdMotor->DefaultColWidth-20;
    SelMotSpeed=0;
}
//---------------------------------------------------------------------------
TQPF_Timer  LoopWait;
void __fastcall TfMotorTest::DoLoopMove()
{
    int p, Wait;

    int &Task=LoopTask;
    bool AllMot_Flag;
    static bool flag=false;
    if(ActiveIndex==-1 ||                                                       //Steven 20230731 : Add protection
       btnHome->Down)                                                           //JerryYang 20230815 : 修正LoopMove異常
    {
        btnLoopMove->Down=false;
        return;
    }

    switch(Task)
    {
        case 1:
            if(select->ItemIndex==0)
            {
                p=atoi(MotorTestClass[ActiveIndex]->edPos1->Text.c_str());
                if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                            ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
                {
                    if(flag==false)
                    {
                        flag=true;
                        tLoopMoveTimer.LatchCycleTime(true);
                    }
                    if(MOT[ActiveIndex].Gali_MotMove(p, MOT[ActiveIndex].Motor->PJogHighSpeed*scrlbrMotorSpeed->Position/100))
                    {
                        flag=false;
                        lblJogPTime->Caption=tLoopMoveTimer.LatchCycleTime();

                        Wait=atoi(cbbLoopWaitTime->Text.c_str());
                        if(Wait>100 || Wait<=0)
                        {
                            Task=50;
                            break;
                        }
                        else
                        {
                            Task=10;
                            LoopWait.Set0_1SecAndOn(Wait);
                            break;
                        }
                    }
                }
                else
                {
                    if(flag==false)
                    {
                        flag=true;
                        tLoopMoveTimer.LatchCycleTime(true);
                    }
                    if(MOT[ActiveIndex].MotorMove(p))
                    {
                        flag=false;
                        lblJogPTime->Caption=tLoopMoveTimer.LatchCycleTime();
                        Wait=atoi(cbbLoopWaitTime->Text.c_str());
                        if(Wait>100 || Wait<=0)
                        {
                            Task=50;
                            break;
                        }
                        else
                        {
                            Task=10;
                            LoopWait.Set0_1SecAndOn(Wait);
                            break;
                        }
                    }
                }
            }
            else
            {
                AllMot_Flag=true;
                for(int i=0; i<int(MotorTestClass.size()); i++)
                {
                    if(MotorTestClass[i]->cbUsing->Checked)
                    {
                        p=atoi(MotorTestClass[i]->edPos1->Text.c_str());
                        MotorTestClass[i]->AllMotMoveFlag=MOT[i].MotorMove(p);

                    }
                    else
                    {
                        MotorTestClass[i]->AllMotMoveFlag=true;
                    }
                    if(MotorTestClass[i]->AllMotMoveFlag==false)
                        AllMot_Flag=false;
                }
                if(AllMot_Flag)
                {
                    Wait=atoi(cbbLoopWaitTime->Text.c_str());
                    if(Wait>100 || Wait<=0)
                    {
                        Task=50;
                        break;
                    }
                    else
                    {
                        Task=10;
                        LoopWait.Set0_1SecAndOn(Wait);
                        break;
                    }
                }

            }
            break;
        case 10:
            if(LoopWait.Off())
                Task=50;
            break;
        case 50:
            if(select->ItemIndex==0)
            {
                p=atoi(MotorTestClass[ActiveIndex]->edPos2->Text.c_str());
                if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                            ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
                {
                    if(flag==false)
                    {
                        flag=true;
                        tLoopMoveTimer.LatchCycleTime(true);
                    }
                    if(MOT[ActiveIndex].Gali_MotMove(p, MOT[ActiveIndex].Motor->PJogHighSpeed*scrlbrMotorSpeed->Position/100))
                    {
                        flag=false;
                        lblJogNTime->Caption=tLoopMoveTimer.LatchCycleTime();
                        Wait=atoi(cbbLoopWaitTime->Text.c_str());
                        dwLoopCount++;
                        lblLoopCount->Caption=dwLoopCount;
                        Average+=tLoopMoveTimer.LatchCycleTime();
                        lblAvgTime->Caption=ChangeToFloatNonPcnt((double)(Average), (double)(dwLoopCount));

                        if(Wait>100 || Wait<=0)
                        {
                            Task=1;
                            break;
                        }
                        else
                        {
                            Task=60;
                            LoopWait.Set0_1SecAndOn(Wait);
                            break;
                        }
                    }
                }
                else
                {
                    if(flag==false)
                    {
                        flag=true;
                        tLoopMoveTimer.LatchCycleTime(true);
                    }

                    if(MOT[ActiveIndex].MotorMove(p))
                    {
                        flag=false;
                        lblJogNTime->Caption=tLoopMoveTimer.LatchCycleTime();

                        Wait=atoi(cbbLoopWaitTime->Text.c_str());
                        dwLoopCount++;
                        lblLoopCount->Caption=dwLoopCount;
                        Average+=tLoopMoveTimer.LatchCycleTime();
                        lblAvgTime->Caption=ChangeToFloatNonPcnt((double)(Average), (double)(dwLoopCount));

                        if(Wait>100 || Wait<=0)
                        {
                            Task=1;
                            break;
                        }
                        else
                        {
                            Task=60;
                            LoopWait.Set0_1SecAndOn(Wait);
                            break;
                        }
                    }
                }
            }
            else
            {
                AllMot_Flag=true;
                for(int i=0; i<int(MotorTestClass.size()); i++)
                {
                    if(MotorTestClass[i]->Visible==true &&                  //Steven 20240822 : 沒用到的馬達不new
                       MotorTestClass[i]->cbUsing->Checked)
                    {
                        p=atoi(MotorTestClass[i]->edPos2->Text.c_str());
                        MotorTestClass[i]->AllMotMoveFlag=MOT[i].MotorMove(p);
                    }
                    else
                    {
                        MotorTestClass[i]->AllMotMoveFlag=true;
                    }
                    if(MotorTestClass[i]->AllMotMoveFlag==false)
                        AllMot_Flag=false;
                }

                if(AllMot_Flag)
                {
                    Wait=atoi(cbbLoopWaitTime->Text.c_str());
                    dwLoopCount++;
                    lblLoopCount->Caption=dwLoopCount;
                    Average+=tLoopMoveTimer.LatchCycleTime();
                    lblAvgTime->Caption=ChangeToFloatNonPcnt((double)(Average), (double)(dwLoopCount));

                    if(Wait>100 || Wait<=0)
                    {
                        Task=1;
                        break;
                    }
                    else
                    {
                        Task=60;
                        LoopWait.Set0_1SecAndOn(Wait);
                        break;
                    }
                }
            }
            break;
        case 60:
            if(LoopWait.Off())
                Task=1;
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::UpdateMotorLed()
{
    TALed *P[]={ALed1, ALed2, ALed3, ALed4, ALed5, ALed6, ALed7, ALed8, ALed9, ALed10};
    if(ActiveIndex==-1)
    {
        for(int i=0; i<9; i++)
            P[i]->Value=false;
        return;
    }
    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        MOT[ActiveIndex].Gali_ScanMotStatus();
    else
        MOT[ActiveIndex].ScanMotorStatus();
    for(int i=0; i<10; i++)
        P[i]->Value=MOT[ActiveIndex].Led[i];

    if(MOT[ActiveIndex].Led[iAlarmLed] && fNote->fShow)
    {
        MOT[ActiveIndex].PCIL132_StopMotor();
        MOT[ActiveIndex].HomeFlag=0;
        if(btnLoopMove->Down)
            btnLoopMove->Down=false;
        if(btnHome->Down)
            btnHome->Down=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::UpdateMotorParameter()
{
    if(ActiveIndex==-1)                                                         //Steven 20230731 : Add protection
        return;
    strngrdMotor->Cells[1][ 1]=MOT[ActiveIndex].Motor->ReadInitSpeed();
    strngrdMotor->Cells[1][ 2]=MOT[ActiveIndex].Motor->PJogHighSpeed;
    strngrdMotor->Cells[1][ 3]=MOT[ActiveIndex].Motor->PJogLowSpeed;
    strngrdMotor->Cells[1][ 4]=MOT[ActiveIndex].Motor->PHomeHighSpeed;
    strngrdMotor->Cells[1][ 5]=MOT[ActiveIndex].Motor->PHomeLowSpeed;
    strngrdMotor->Cells[1][ 6]=MOT[ActiveIndex].Motor->PSoftLimitP;
    strngrdMotor->Cells[1][ 7]=MOT[ActiveIndex].Motor->PSoftLimitN;
    strngrdMotor->Cells[1][ 8]=MOT[ActiveIndex].Motor->ReadAcc();
    strngrdMotor->Cells[1][ 9]=MOT[ActiveIndex].Motor->ReadDec();
    strngrdMotor->Cells[1][10]=MOT[ActiveIndex].Motor->ReadRange();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::ShowMotorSelect(int Index, int Attr)
{
    if(Index==-1)                                                               //Steven 20230731 : Add protection
        return;

    if(MotorTestClass[Index]->Visible)                                          //Steven 20240822 : 沒用到的馬達不new
    {
        pnlMotorAlias->Caption=MotorTestClass[Index]->labName->Caption+MOT[Index].Alias;

        if(Attr)
        {
            MotorTestClass[Index]->labName->Font->Color =clRed;
            MotorTestClass[Index]->edPos1->Font->Color  =clWhite;
            MotorTestClass[Index]->edPos2->Font->Color  =clWhite;
            MotorTestClass[Index]->edPos1->Color        =clRed;
            MotorTestClass[Index]->edPos2->Color        =clRed;
        }
        else
        {
            MotorTestClass[Index]->labName->Font->Color =clBlack;
            MotorTestClass[Index]->edPos1->Font->Color  =clBlack;
            MotorTestClass[Index]->edPos2->Font->Color  =clBlack;
            MotorTestClass[Index]->edPos1->Color        =clWindow;
            MotorTestClass[Index]->edPos2->Color        =clWindow;
        }
    }
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::UpdatePreasureScreen()
{
    AnsiString str;
    static int ct=0;
    ct++;
    if(ct>3)
    {
        ct=0;
        Edit14->Text        =MOT[MTestZ1].Gali_ReadPos();
        Edit24->Text        =MOT[MTestZ2].Gali_ReadPos();
        edtSpeedIndex1->Text=MOT[MTestZ1].Gali_ReadPos();
        edtSpeedIndex2->Text=MOT[MTestZ2].Gali_ReadPos();
    }
    static double T[2][256];
    static int p=0;
    double sum0, sum1;
    p++;
    if(p>=50)
        p=0;
    sum0=0;
    sum1=0;
    for(int i=0; i<50; i++)
    {
        sum0+=T[0][i];
        sum1+=T[1][i];
    }
    sum0/=50.0;
    sum1/=50.0;
    str.sprintf("%6.3f", sum0);
    Edit16->Text=str;
    str.sprintf("%6.3f", sum1);
    Edit102->Text=str;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::lM00Click(TObject *Sender)
{
    TLabel *Ptr;
    Ptr=(TLabel *)Sender;
    unsigned Speed;

#ifndef SOFT_SIMULTE
    if(MOT[Ptr->Tag].Motor->Enable==false)
        return;
#endif
    btnLoopMove->Down=false;
    btnHome->Down=false;
    if(ActiveIndex!=-1)
        ShowMotorSelect(ActiveIndex, 0);
    ActiveIndex=Ptr->Tag;
    ShowMotorSelect(ActiveIndex, 1);
    Speed=MOT[Ptr->Tag].GetSpeed();
    if(Speed>MOT[Ptr->Tag].Motor->PJogHighSpeed)
        Speed=MOT[Ptr->Tag].Motor->PJogHighSpeed;
    if(Speed<MOT[Ptr->Tag].Motor->PJogLowSpeed)
        Speed=MOT[Ptr->Tag].Motor->PJogLowSpeed;
    scrlbrMotorSpeed->Position=1;
    MOT[Ptr->Tag].SetSpeed(1);
    edtMotorRate->Text=MOT[Ptr->Tag].Motor->ReadRate();
    edtMotorRange->Text=MOT[Ptr->Tag].Motor->ReadRange();
    edtSpeed->Text=1;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::lpA00Click(TObject *Sender)
{
    TEdit *Ptr;
    Ptr=(TEdit *)Sender;
    unsigned int Speed;

#ifndef SOFT_SIMULTE
    if(MOT[Ptr->Tag].Motor->Enable==false)
        return;
#endif
    btnLoopMove->Down=false;
    btnHome->Down=false;
    if(ActiveIndex!=-1)
        ShowMotorSelect(ActiveIndex, 0);
    ActiveIndex=Ptr->Tag;
    ShowMotorSelect(ActiveIndex, 1);
    Speed=MOT[Ptr->Tag].Motor->ReadSpeed();
    if(Speed>MOT[Ptr->Tag].Motor->PJogHighSpeed)
        Speed=MOT[Ptr->Tag].Motor->PJogHighSpeed;
    if(Speed<MOT[Ptr->Tag].Motor->PJogLowSpeed)
        Speed=MOT[Ptr->Tag].Motor->PJogLowSpeed;
    scrlbrMotorSpeed->Position=1;
    MOT[Ptr->Tag].SetSpeed(1);
    edtMotorRate->Text=MOT[Ptr->Tag].Motor->ReadRate();
    edtMotorRange->Text=MOT[Ptr->Tag].Motor->ReadRange();
    edtSpeed->Text=1;
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[Ptr->Tag].Motor->PSoftLimitP, MOT[Ptr->Tag].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::scrlbrMotorSpeedScroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
    if(ActiveIndex==-1)
        return;
    if(bSingleHome==true)
        return;

    scrlbrMotorSpeed->Max=100;
    scrlbrMotorSpeed->SmallChange=1;
    MOT[ActiveIndex].SetSpeed(scrlbrMotorSpeed->Position);
    edtSpeed->Text=scrlbrMotorSpeed->Position;
    if(btnLoopMove->Down)
    {
        for(int i=0; i<int(MotorTestClass.size()); i++)
            if(MotorTestClass[i]->Visible==true &&                              //Steven 20240822 : 沒用到的馬達不new
               MotorTestClass[i]->cbUsing->Checked)
                MOT[i].SetSpeed(scrlbrMotorSpeed->Position);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbMotorTest_JogNMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;

    int NowPos=atoi(edtCommandPos->Text.c_str());
    if(abs(NowPos)>999999)                                                      //Steven 20100831 : 溢位保護
    {
        sbMotorTest_JogN->Down=false;                                           //Steven 20180507 : 避免卡住跳不出去
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(CheckSafeDoorIsClosed()==false)
    {
        sbMotorTest_JogN->Down=false;
        return;
    }

    SelMotSpeed=atoi(edtSpeed->Text.c_str());
    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        if(SelMotSpeed>500)
        {
            SelMotSpeed=500;
            edtSpeed->Text=SelMotSpeed;
            scrlbrMotorSpeed->Position=SelMotSpeed;
        }
        MOT[ActiveIndex].Gali_JogN(SelMotSpeed*100);
    }
    else
    {
        MOT[ActiveIndex].JogN(SelMotSpeed);                                     //Steven 20101227
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton26MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;
    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        MOT[ActiveIndex].Gali_Command("ST", __FUNC__);
    else
        MOT[ActiveIndex].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbMotorTest_JogPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;

    int NowPos=atoi(edtCommandPos->Text.c_str());
    if(abs(NowPos)>999999)                                                      //Steven 20100831 : 溢位保護
    {
        sbMotorTest_JogP->Down=false;                                           //Steven 20180507 : 避免卡住跳不出去
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(CheckSafeDoorIsClosed()==false)
    {
        sbMotorTest_JogP->Down=false;
        return;
    }

    SelMotSpeed=atoi(edtSpeed->Text.c_str());
    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        if(SelMotSpeed>500)
        {
            SelMotSpeed=500;
            edtSpeed->Text=SelMotSpeed;
            scrlbrMotorSpeed->Position=SelMotSpeed;
        }
        MOT[ActiveIndex].Gali_JogP(SelMotSpeed*100);
    }
    else
    {
        MOT[ActiveIndex].JogP(SelMotSpeed); //Steven 20101227
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbMotorTest_JogPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;
    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        MOT[ActiveIndex].Gali_Command("ST", __FUNC__);
    else
        MOT[ActiveIndex].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::Timer1Timer(TObject *Sender)
{
    if(fShow==false)
        return;

    static int ct=0;

    if(IsSafeLockCheck())
        Close();
    if(fNote->fShow || MyMessageBox->fShow)
    {
        btnLoopMove->Down=false;
        btnHome->Down=false;
        bSingleHome=false;
    }

    if(PageControl1->ActivePageIndex==1)
        UpdatePreasureScreen();

    if(ActiveIndex==-1)
        return;

    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        edtCommandPos->Text=MOT[ActiveIndex].Gali_ReadPos();
        pnlEncoderPos->Caption=MOT[ActiveIndex].Gali_ReadEncoderPos();
    }
    else
    {
        edtCommandPos->Text=MOT[ActiveIndex].ReadPos();
        pnlEncoderPos->Caption=MOT[ActiveIndex].ReadEncoderPos();
    }
    UpdateMotorLed();

    if(btnLoopMove->Down)
    {
        DoLoopMove();
    }
    else if(btnHome->Down)
    {
        if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                    ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        {
            if(DoGaliHome(ActiveIndex))
            {
                btnHome->Down=false;
                edtHomeOffset->Text=-(int)(MOT[ActiveIndex].Motor->LastHomePos*MOT[ActiveIndex].Motor->GearRatio);
            }
        }
        else if(bSingleHome==false)
        {
            btnHome->Down=false;
            edtHomeOffset->Text=MOT[ActiveIndex].Motor->LastHomePos;
            MOT[ActiveIndex].SetSpeed(scrlbrMotorSpeed->Position);
        }
    }
    ct++;
    if(ct>3)
    {
        ct=0;
        edtSpeedIndex1->Text=MOT[MTestZ1].GetSpeed();
        edtSpeedIndex2->Text=MOT[MTestZ2].GetSpeed();
    }

    if(btnHome->Down && (MyMessageBox->fShow || fNote->fShow))
    {
        btnHome->Down=false;
    }

    lblRealSpeed->Caption="Real Speed : "+AnsiString(MOT[ActiveIndex].GetSpeed());
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::FormShow(TObject *Sender)
{
    AnsiString Str;
    AnsiString sPath=asSystemPath+"MotorTest.ini";

    fShow=true;
    Left=5;
    Top =5;

    bSingleHome=false;
    for(int i=0; i<int(MotorTestClass.size()); i++)
    {
        if(MotorTestClass[i]->Visible==true)                                    //Steven 20240822 : 沒用到的馬達不new
        {
            MotorTestClass[i]->edPos1->Text=CheckAndReadIniData(sPath, MotorTestClass[i]->labName->Caption, "Position 1", 0);  //Steven 20120831 : 怕會記憶體破壞，改用ini
            MotorTestClass[i]->edPos2->Text=CheckAndReadIniData(sPath, MotorTestClass[i]->labName->Caption, "Position 2", 0);
        }
    }
    btnHome->Down=false;
    btnLoopMove->Down=false;

    if(LastSet.iLanguageCountry==0)
    {
        strngrdMotor->Cells[0][0]=" Item";
        strngrdMotor->Cells[1][0]=" Value";

        strngrdMotor->Cells[0][ 1]=" InitialSpeed";
        strngrdMotor->Cells[0][ 2]=" JogHighSpeed";
        strngrdMotor->Cells[0][ 3]=" JogLowSpeed";
        strngrdMotor->Cells[0][ 4]=" HomeHighSpeed";
        strngrdMotor->Cells[0][ 5]=" HomeLowSpeed";
        strngrdMotor->Cells[0][ 6]=" SoftLimitP";
        strngrdMotor->Cells[0][ 7]=" SoftLimitN";
        strngrdMotor->Cells[0][ 8]=" Acc";
        strngrdMotor->Cells[0][ 9]=" Dec";
        strngrdMotor->Cells[0][10]=" Range";
    }
    else if(LastSet.iLanguageCountry==1)
    {
        strngrdMotor->Cells[0][0]=" 項目";
        strngrdMotor->Cells[1][0]=" 設定值";

        strngrdMotor->Cells[0][ 1]=" 初始速度";
        strngrdMotor->Cells[0][ 2]=" 寸動最高速";
        strngrdMotor->Cells[0][ 3]=" 寸動最低速";
        strngrdMotor->Cells[0][ 4]=" 歸零最高速";
        strngrdMotor->Cells[0][ 5]=" 歸零最低速";
        strngrdMotor->Cells[0][ 6]=" 軟體最大點位";
        strngrdMotor->Cells[0][ 7]=" 軟體最小點位";
        strngrdMotor->Cells[0][ 8]=" 加速度";
        strngrdMotor->Cells[0][ 9]=" 減速度";
        strngrdMotor->Cells[0][10]=" 放大倍率";
    }
    bool f=SW[SwMotorRelay].OutValue;
    if(f)
    {
        DoMotorPowerOn();
        SW[SwServerON].On();
        btnMotorPower->Caption="Motor Power On";
        btnMotorPower->Down=true;
    }
    else
    {
        fHome->GaliMotorServoOff("TfMotorTest::FormShow");                      //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
        btnMotorPower->Caption="Motor Power Off";
        btnMotorPower->Down=false;
    }
    select->ItemIndex=0;
    ActiveIndex=-1;

    PageControl1->ActivePageIndex=0;

    if(XResolution>1300)                                                        //Steven 20230912 : 調整畫面為FULL HD
        Width=1300;
    else
        Width=1100;

    cbbMotorName->Clear();
    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        Str.sprintf("M%02d", i);
        cbbMotorName->Items->Add(Str);
    }

    tsMotorDatabase->TabVisible=(IO_CARD_TYPE==NewIO_MN200 ||
                                 IO_CARD_TYPE==PCI_P64C64);                     //Steven 20231218 HT7080B

    if(IO_CARD_TYPE==NewIO_MN200 ||
       IO_CARD_TYPE==PCI_P64C64)                                                //Steven 20231218 HT7080B
        sbtReload->Click();

    pnlStop->Color=(TColor)0x00DFD9CC;                                          //Steven 20230815 : Add for MotorTest
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSetPosPClick(TObject *Sender)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)                   //Steven 20230731 : Add protection
        return;

    if(MotorTestClass[ActiveIndex]->Visible==true)                              //Steven 20240822 : 沒用到的馬達不new
        MotorTestClass[ActiveIndex]->edPos1->Text=edtCommandPos->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSetPosNClick(TObject *Sender)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)                   //Steven 20230731 : Add protection
        return;

    if(MotorTestClass[ActiveIndex]->Visible==true)                              //Steven 20240822 : 沒用到的馬達不new
        MotorTestClass[ActiveIndex]->edPos2->Text=edtCommandPos->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnGoSoftPClick(TObject *Sender)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;

    MOT[ActiveIndex].MotorMove(MOT[ActiveIndex].Motor->PSoftLimitP);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnGoSoftNClick(TObject *Sender)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;

    MOT[ActiveIndex].MotorMove(MOT[ActiveIndex].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnHomeClick(TObject *Sender)
{
    btnLoopMove->Down=false;

    if(ActiveIndex==-1)
    {
        btnHome->Down=false;
        return;
    }

    if(ActiveIndex==MInArmXScale  || ActiveIndex==MInArmYScale ||               //Steven 20160426 : 磁性尺
       ActiveIndex==MOutArmXScale || ActiveIndex==MOutArmYScale)
    {
        MOT[ActiveIndex].Motor->ResetPos(0);
        MOT[ActiveIndex].HomeFlag=1;
        btnHome->Down=false;
        return;
    }

    if(btnHome->Down==true)
    {
        if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                    ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        {
            MOT[ActiveIndex].iGali_SingalHomeTask=1;
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
            fHome->InitGali_HomeTask();
        }
        else
        {
            fHome->iHomeStep=1;
            iSingleHomeIndex=ActiveIndex;
            InitProcessSingleMotorTask(iSingleHomeIndex);
            bSingleHome=true;
            MOT[ActiveIndex].HomeFlag=0;
        }
    }
    else
    {
        if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                    ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        {
            MOT[ActiveIndex].Gali_Command("ST", __FUNC__);
        }
        else
        {
            bSingleHome=false;
            MOT[ActiveIndex].PCIL132_StopMotor();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    if(ACol!=1 || ARow==0)
        return;

    if(ActiveIndex==-1)
        return;

    double ret;
    Edit2->Text=strngrdMotor->Cells[ACol][ARow];
    if(ARow==8 || ARow==9)
    {
        fQwertyKey->ShowQwertyKey(Edit2, N_DOUBLE, 0);
        ret=atof(Edit2->Text.c_str());
    }
    else
    {
        fQwertyKey->ShowQwertyKey(Edit2, N_INTEGER, 0);
        ret=atoi(Edit2->Text.c_str());
    }

    strngrdMotor->Cells[ACol][ARow]=ret;
    if(ARow==1)
        MOT[ActiveIndex].Motor->SetInitSpeed(ret);
    else if(ARow==2)
        MOT[ActiveIndex].Motor->PJogHighSpeed=ret;
    else if(ARow==3)
        MOT[ActiveIndex].Motor->PJogLowSpeed=ret;
    else if(ARow==4)
        MOT[ActiveIndex].Motor->PHomeHighSpeed=ret;
    else if(ARow==5)
        MOT[ActiveIndex].Motor->PHomeLowSpeed=ret;
    else if(ARow==6)
        MOT[ActiveIndex].Motor->PSoftLimitP=ret;
    else if(ARow==7)
        MOT[ActiveIndex].Motor->PSoftLimitN=ret;
    else if(ARow==8)
        MOT[ActiveIndex].Motor->SetAccDataBase(ret);
    else if(ARow==9)
        MOT[ActiveIndex].Motor->SetDecDataBase(ret);
    else if(ARow==10)
        MOT[ActiveIndex].Motor->SetRange(ret);
    btnLoopMove->Down=false;
    MOT[ActiveIndex].HomeFlag=0;
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbMotorTest_MoveNClick(TObject *Sender)
{
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;

    int Pos=atoi(cbbInterval->Text.c_str()), NowPos;
    NowPos=atoi(edtCommandPos->Text.c_str());

    if(abs(NowPos)>999999)                                                      //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        MOT[ActiveIndex].MovFlag=false;
        MOT[ActiveIndex].Gali_MotMove(NowPos-Pos, atoi(edtSpeed->Text.c_str())*100);
    }
    else
    {
        MOT[ActiveIndex].SetSpeed(scrlbrMotorSpeed->Position);
        MOT[ActiveIndex].InitMOTParameter();
        MOT[ActiveIndex].MotorMove(MOT[ActiveIndex].ReadPos()-Pos);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbMotorTest_MovePClick(TObject *Sender)
{
    int Pos=atoi(cbbInterval->Text.c_str()), NowPos;
    if(ActiveIndex==-1 || btnHome->Down || btnLoopMove->Down)
        return;

    NowPos=atoi(edtCommandPos->Text.c_str());

    if(abs(NowPos)>999999)                                                      //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        MOT[ActiveIndex].MovFlag=false;
        MOT[ActiveIndex].Gali_MotMove(NowPos+Pos, atoi(edtSpeed->Text.c_str())*100);
    }
    else
    {
        MOT[ActiveIndex].SetSpeed(scrlbrMotorSpeed->Position);
        MOT[ActiveIndex].InitMOTParameter();
        MOT[ActiveIndex].MotorMove(MOT[ActiveIndex].ReadPos()+Pos);
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfMotorTest::IsMotorCanRun(bool attr)
{
    bool flag=true;
    if(Sen[SnFrontLeftEMG].IsOff())
        flag=false;
    if(Sen[SnFrontRightEMG].IsOff())
        flag=false;
    if(Sen[SnRearLeftEMG].IsOff())
        flag=false;
    if(Sen[SnRearRightEMG].IsOff())
        flag=false;
    if(Enable_PLCSafety_IO && Sen[SnAllEMG].IsOff())                            //KenHsieh 20250212 : 新增PLC 斷線可瞬間判斷EMG及安全門
        flag=false;
    if(Sen[SnServo].Enable && Sen[SnServo].IsOff())                             //kevin 20140121  EMG沒有完全按下去
        flag=false;
    if(attr && flag==false)
        ShowMyMessage("EMG Stop", "");
    return flag;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnLoopMoveClick(TObject *Sender)
{
    int ret;
    if(ActiveIndex==-1)
    {
        btnLoopMove->Down=false;
        return;
    }

    btnHome->Down=false;
    if(btnLoopMove->Down)
    {
        LoopTask=1;
        btnLoopMove->Down=false;
        for(int i=0; i<TOTAL_MOTOR; i++)
            MOT[i].InitMOTParameter();
        if(MOT[ActiveIndex].HomeFlag==0)
        {
            ret=MessageDlg("Motor not home yet, sure to loop test? (馬達尚未歸零，確定要執行？)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
            if(ret==mrNo)
            {
                btnLoopMove->Down=false;
            }
            else
            {
                btnLoopMove->Down=true;
                dwLoopCount=0;
            }
        }
        else
        {
            btnLoopMove->Down=true;
            dwLoopCount=0;
            Average=0;                                                          //KenHsieh 20230103 : 重新Loop後歸零
            lblAvgTime->Caption=0;                                              //KenHsieh 20230103 : 重新Loop後歸零
        }
    }
    else
    {
        if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                    ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
            MOT[ActiveIndex].Gali_Command("ST", __FUNC__);
        else
            MOT[ActiveIndex].PCIL132_StopMotor();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    AnsiString sPath=asSystemPath+"MotorTest.ini";
    fShow=false;
    for(int i=0; i<int(MotorTestClass.size()); i++)
    {
        if(MotorTestClass[i]->Visible==true)                                    //Steven 20240822 : 沒用到的馬達不new
        {
            WriteIniData(sPath, MotorTestClass[i]->labName->Caption, "Position 1", MotorTestClass[i]->edPos1->Text);   //Steven 20120831 : 怕會記憶體破壞，改用ini
            WriteIniData(sPath, MotorTestClass[i]->labName->Caption, "Position 2", MotorTestClass[i]->edPos2->Text);
        }
    }

    PauseUT150Polling=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSetRateClick(TObject *Sender)
{
    btnLoopMove->Down=false;
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->SetRate(atoi(edtMotorRate->Text.c_str()));
    MOT[ActiveIndex].Motor->InitMotor(MOT[ActiveIndex].Motor->Address);
    MOT[ActiveIndex].HomeFlag=0;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSetRangeClick(TObject *Sender)
{
    btnLoopMove->Down=false;
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->SetRange(atoi(edtMotorRange->Text.c_str()));
    MOT[ActiveIndex].Motor->InitMotor(MOT[ActiveIndex].Motor->Address);
    MOT[ActiveIndex].HomeFlag=0;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::BitBtn1Click(TObject *Sender)
{
    char str[10];
    int high, low, Source;
    strncpy(str, cbbMotorName->Text.c_str(), sizeof(str));
    high=str[1]-'0';
    low =str[2]-'0';
    Source=high*10+low;
    if(ActiveIndex==-1 || Source>24 || Source<0)
        return;
    MOT[ActiveIndex].Motor->PJogHighSpeed  =MOT[Source].Motor->PJogHighSpeed;
    MOT[ActiveIndex].Motor->PJogLowSpeed   =MOT[Source].Motor->PJogLowSpeed;
    MOT[ActiveIndex].Motor->PHomeHighSpeed =MOT[Source].Motor->PHomeHighSpeed;
    MOT[ActiveIndex].Motor->PHomeLowSpeed  =MOT[Source].Motor->PHomeLowSpeed;
    MOT[ActiveIndex].Motor->PSoftLimitP    =MOT[Source].Motor->PSoftLimitP;
    MOT[ActiveIndex].Motor->PSoftLimitN    =MOT[Source].Motor->PSoftLimitN;
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnHighSpeedClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->PJogHighSpeed=MOT[ActiveIndex].Motor->ReadSpeed();
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnLowSpeedClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->PJogLowSpeed=MOT[ActiveIndex].Motor->ReadSpeed();
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnHomeHighClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->PHomeHighSpeed=MOT[ActiveIndex].Motor->ReadSpeed();
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnHomeLowClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->PHomeLowSpeed=MOT[ActiveIndex].Motor->ReadSpeed();
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSoftPPosClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->PSoftLimitP=MOT[ActiveIndex].ReadPos();
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSoftNPosClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    MOT[ActiveIndex].Motor->PSoftLimitN=MOT[ActiveIndex].ReadPos();
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnRangeClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnRateClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    UpdateMotorParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton4Click(TObject *Sender)
{
    Edit3->Text=Edit16->Text;
    Edit4->Text=Edit18->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton5Click(TObject *Sender)
{
    Edit5->Text=Edit16->Text;
    Edit6->Text=Edit18->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton8Click(TObject *Sender)
{
    Edit12->Text=Edit16->Text;
    Edit13->Text=Edit18->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton6Click(TObject *Sender)
{
    Edit8->Text=Edit102->Text;
    Edit10->Text=Edit103->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton7Click(TObject *Sender)
{
    Edit7->Text=Edit102->Text;
    Edit11->Text=Edit103->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton9Click(TObject *Sender)
{
    Edit15->Text=Edit102->Text;
    Edit17->Text=Edit103->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::ScrollBar2Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
    MOT[MTestZ1].SetSpeed(ScrollBar2->Position);
    edtSpeedIndex1->Text=MOT[MTestZ1].GetSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::ScrollBar3Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
    MOT[MTestZ2].SetSpeed(ScrollBar3->Position);
    edtSpeedIndex2->Text=MOT[MTestZ2].GetSpeed();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton44Click(TObject *Sender)
{
    int Pos=atoi(ComboBox4->Text.c_str());
    MOT[MTestZ1].MotorMove(MOT[MTestZ1].ReadPos()-Pos);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton45Click(TObject *Sender)
{
    int Pos=atoi(ComboBox4->Text.c_str());
    MOT[MTestZ1].MotorMove(MOT[MTestZ1].ReadPos()+Pos);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton50Click(TObject *Sender)
{
    int Pos=atoi(ComboBox5->Text.c_str());
    MOT[MTestZ2].MotorMove(MOT[MTestZ2].ReadPos()-Pos);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton51Click(TObject *Sender)
{
    int Pos=atoi(ComboBox5->Text.c_str());
    MOT[MTestZ2].MotorMove(MOT[MTestZ2].ReadPos()+Pos);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton46MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ1].Motor->JogN();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton46MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ1].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton47MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ1].Motor->JogP();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton47MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ1].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton52MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ2].Motor->JogN();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton52MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ2].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton53MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ2].Motor->JogP();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::SpeedButton53MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MOT[MTestZ2].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::edtSpeedChange(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;
    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        scrlbrMotorSpeed->Max=10000;
        scrlbrMotorSpeed->SmallChange=50;
        scrlbrMotorSpeed->Position=atoi(edtSpeed->Text.c_str());
    }
    else
    {
        scrlbrMotorSpeed->SmallChange=1;
        MOT[ActiveIndex].SetSpeed(atoi(edtSpeed->Text.c_str()));
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnGoClick(TObject *Sender)
{
    if(ActiveIndex==-1)
        return;

    if(IsMotorCanRun(true)==false)
        return;

    if(MOT[ActiveIndex].HomeFlag==0)
    {
        ShowMyMessage("Motor not home", "馬達尚未歸零", "MotorTest");
        return;
    }
    MOT[ActiveIndex].MotorMove(atoi(edtHomeOffset->Text.c_str()));
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnMotorPowerClick(TObject *Sender)
{
    bool bRelayOn=SW[SwMotorRelay].OutValue;
    AllBtnUp();                                                                 //Steven 20230815 : Add for MotorTest

    if(bRelayOn)
    {
        if(mrNo==MessageDlg("Sure to turn off motor power? (確定要關掉馬達電源？)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0))
            return;
    };
    bRelayOn=!bRelayOn;
    if(bRelayOn)
    {
        DoMotorPowerOn();
        SW[SwServerON].On();
        btnMotorPower->Caption="Motor Power On";
        btnMotorPower->Down=true;
    }
    else
    {
        fHome->GaliMotorServoOff("TfMotorTest::btnMotorPowerClick");            //Steven 20230712 : 修正SwServoOn.Off時, 要抓住Z煞車
        btnMotorPower->Caption="Motor Power Off";
        btnMotorPower->Down=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnStopClick(TObject *Sender)
{
    AllBtnUp();                                                                 //Steven 20230815 : Add for MotorTest
    StopAllMotor();                                                             //JerryYang 20230815 : add
    bSingleHome=false;                                                          //JerryYang 20230815 : add
    if(ActiveIndex==-1)
        return;
    if(INDEX_MOTION_CARD==0 && (ActiveIndex==MTestY1 || ActiveIndex==MTestZ1 ||
                                ActiveIndex==MTestZ2 || ActiveIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        MOT[ActiveIndex].Gali_Command("ST", __FUNC__);
    else
        MOT[ActiveIndex].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnServoOffClick(TObject *Sender)
{
    AllBtnUp();                                                                 //Steven 20230815 : Add for MotorTest
    if(ActiveIndex==-1)                                                         //Steven 20230731 : Add protection
        return;
    MOT[ActiveIndex].ScanMotorStatus();
    static bool bServoOn=true;

    bServoOn=!bServoOn;
    MOT[ActiveIndex].ServoOnOff(bServoOn);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::ck00Click(TObject *Sender)
{
    if(MotorTestClass[MTestY1]->cbUsing->Checked ||
       MotorTestClass[MTestZ1]->cbUsing->Checked ||
       MotorTestClass[MTestZ2]->cbUsing->Checked ||
       MotorTestClass[MTestY2]->cbUsing->Checked)
    {
        MotorTestClass[MTestY1]->cbUsing->Checked=false;
        MotorTestClass[MTestZ1]->cbUsing->Checked=false;
        MotorTestClass[MTestZ2]->cbUsing->Checked=false;
        MotorTestClass[MTestY2]->cbUsing->Checked=false;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfMotorTest::DoGaliHome(int Index)
{
    if(MOT[Index].Gali_SingalHome())
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnReloadMotorDataClick(TObject *Sender)
{
    InitialMotorParameter();
    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        if(INDEX_MOTION_CARD==0 && (MOT[i].Mot_Name==MTestY1 || MOT[i].Mot_Name==MTestZ1 ||
                                    MOT[i].Mot_Name==MTestZ2 || MOT[i].Mot_Name==MTestY2))      //Steven 20210623 : Index使用Galil
        {
            if(MOT[i].Mot_Name==MTestY1)
                MOT[MTestY1].Gali_Command("DP 0,0,0,0;DE 0,0,0,0", __FUNC__);
        }
        else
        {
            MOT[i].PCIL132_SetPos(0);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::edtSpeedClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 1, true, 100, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btResetMNetClick(TObject *Sender)
{
    int ret=ShowMyMessageBox_YES_NO("Do you want to Reset MNet?", "你確定要重置MNet)");
    if(ret==1 && SystemStart==false)
    {
        ResetMNet(0, "MNet斷電", "Power Off",false);                            //重置MNet系統
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::palExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::FormDestroy(TObject *Sender)
{
    try
    {
        for(vector<TMotorTestClass *>::iterator iter=MotorTestClass.begin(); iter!=MotorTestClass.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(MotorTestClass);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfMotorTest::FormDestroy");
    }
    LogSoftwareOffTime("TfMotorTest, FormDestroy");                             //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void SaveAsCSV(TMemo *Memo, int iTime)
{
    AnsiString str, tmp;
    str="D:\\LightScale";
    if(DirectoryExists(str)==false)
    {
        if(!MyForceDirectories(str))
        {
            return;
        }
    }

    if(fMotorTest->rgMoveType->ItemIndex==0)
    {
        if(fMotorTest->rgAxis->ItemIndex==1)
            tmp.sprintf("%s\\Motor1Positive", str);
        else if(fMotorTest->rgAxis->ItemIndex==2)
            tmp.sprintf("%s\\Motor3Positive", str);
        else if(fMotorTest->rgAxis->ItemIndex==3)
            tmp.sprintf("%s\\Motor5Positive", str);
        else if(fMotorTest->rgAxis->ItemIndex==4)
            tmp.sprintf("%s\\Motor7Positive", str);
    }
    else if(fMotorTest->rgMoveType->ItemIndex==1)
    {
        if(fMotorTest->rgAxis->ItemIndex==1)
            tmp.sprintf("%s\\Motor2Positive", str);
        else if(fMotorTest->rgAxis->ItemIndex==2)
            tmp.sprintf("%s\\Motor4Positive", str);
        else if(fMotorTest->rgAxis->ItemIndex==3)
            tmp.sprintf("%s\\Motor6Positive", str);
        else if(fMotorTest->rgAxis->ItemIndex==4)
            tmp.sprintf("%s\\Motor8Positive", str);
    }

    tmp=tmp+".csv";
    Memo->Lines->SaveToFile(tmp);
    Memo->Clear();
}
//---------------------------------------------------------------------------
TQPF_Timer DelayTime;
//const int MInArmX       =0;
//const int MInArmY       =1;
//const int MOutArmX      =19;
//const int MOutArmY      =20;
bool bStopflag=false;
void TfMotorTest::LightScale(bool Reset =false)
{
    static int Task=-1;
    AnsiString S1;
    static int iPitch=0;
    static int iDelayTime=0;
    static int iMoveItem=0;
    static int iMovePitch=0;
    static int iNeedMoveLim=0;
    static int iMaxLim=0;
    static int iMinLim=0;

    static int iNeedMovePos=0;
    static int iLightScalePos=0;
    static bool bFlag=false;
    static int iCount1=0;
    static int iCount2=0;
    static int iPosition=0;
    static int iUseAxisItem=-1;
    if(iUseAxis==-1 || rgAxis->ItemIndex==0)
        return;
    else
        iUseAxisItem=iUseAxis;

    if(Reset)
    {
        iPitch=atoi(edPitech->Text.c_str());
        iDelayTime=atoi(edDelayTime->Text.c_str());
        iMinLim=MOT[iUseAxis].Motor->PSoftLimitN;
        iMaxLim=MOT[iUseAxis].Motor->PSoftLimitP;
        edPitech->Enabled=false;
        edDelayTime->Enabled=false;
        iNeedMovePos=0;
        Task=0;
        bStopflag=false;
        return;
    }

    switch(Task)
    {
        case 0:
            fHome->iHomeStep=1;
            iSingleHomeIndex=iUseAxisItem;
            InitProcessSingleMotorTask(iSingleHomeIndex);
            bSingleHome=true;
            MOT[iUseAxisItem].HomeFlag=0;
            Task=1;
            break;
        case 1:
            if(bSingleHome==false)
            {
                Task=10;
            }
            break;
        case 10:
            iMoveItem=rgMoveType->ItemIndex;
            if(iMoveItem==0)
            {
                iNeedMovePos=MOT[iUseAxisItem].Motor->PSoftLimitP-100;
                iMovePitch=iPitch;
                iNeedMoveLim=MOT[iUseAxisItem].Motor->PSoftLimitN+100;
                Task=150;
            }
            else if(iMoveItem==1)
            {
                iNeedMovePos=MOT[iUseAxisItem].Motor->PSoftLimitN+100;
                iMovePitch=(-iPitch);
                iNeedMoveLim=MOT[iUseAxisItem].Motor->PSoftLimitP-100;
                Task=150;
            }
            else if(iMoveItem==2)
            {
                bFlag=true;
                Task=200;
            }
            break;
        case 100:
            iNeedMovePos=iMinLim;
            Task=150;
            break;
        case 150:
            iPosition=iNeedMovePos;

            if(MOT[iUseAxis].MotorMove(iPosition))
            {
                DelayTime.SetMSAndOn(iDelayTime);
                Task=160;
            }
            break;
        case 160:
            if(DelayTime.Off())
            {
                iLightScalePos=(MOT[MLightScale].ReadEncoderPos())*(-1);
                Task=170;
            }
            break;
        case 170:
            S1.sprintf("ArmPosition, %d, LightScalePos, %d ,[ %d ]",iNeedMovePos,iLightScalePos,iNeedMovePos-iLightScalePos);
            Memo1->Lines->Add(S1);
            iNeedMovePos-=iMovePitch;
            Task=180;
            break;
        case 180:
            if(iMoveItem==0 && iNeedMovePos>iNeedMoveLim)
            {
                Task=150;
            }
            else if(iMoveItem==1 && iNeedMoveLim>iNeedMovePos)
            {
                Task=150;
            }
            else
            {
                edPitech->Enabled=true;
                edDelayTime->Enabled=true;
                Timer2->Enabled=false;
                bStopflag=true;
            }
            break;
        case 200:
            if(bFlag)
            {
                iNeedMovePos=iMinLim;
                iMovePitch=iPitch;
                iNeedMoveLim=iMaxLim/2;
                iCount1++;
                iNeedMovePos-=iMovePitch*iCount1;
            }
            else
            {
                iNeedMovePos=iMaxLim;
                iMovePitch=(-iPitch);
                iNeedMoveLim=iMaxLim/2;
                iCount2++;
                iNeedMovePos-=iMovePitch*iCount2;
            }
            Task=210;
            break;
        case 210:
            if(bFlag==true && iNeedMovePos>iNeedMoveLim)
            {
                bFlag=false;
                Task=220;
            }
            else if(bFlag==false && iNeedMoveLim>iNeedMovePos)
            {
                bFlag=true;
                Task=220;
            }
            else
            {
                iCount1=0;
                iCount2=0;
                BitBtn3->Click();
                edPitech->Enabled=true;
                edDelayTime->Enabled=true;
                Timer2->Enabled=false;
                bStopflag=true;
            }
            break;
        case 220:
            iPosition=iNeedMovePos;

            if(MOT[iUseAxisItem].MotorMove(iPosition))
            {
                DelayTime.SetMSAndOn(iDelayTime);
                Task=230;
            }
            break;
        case 230:
            if(DelayTime.Off())
            {
                iLightScalePos=(MOT[MLightScale].ReadEncoderPos())*(-1);
                Task=240;
            }
            break;
        case 240:
            S1.sprintf("ArmPosition, %d, LightScalePos, %d ,[ %d ]", iNeedMovePos, iLightScalePos, iNeedMovePos-iLightScalePos);
            Memo1->Lines->Add(S1);
            iNeedMovePos-=iMovePitch;
            Task=200;
            break;
    }
}
//---------------------------------------------------------------------------
//KaiChen 20171228 ：Log Light Scale Data
//==>
void SaveAsCSV_Kaichen(TMemo *Memo, int iSelect, AnsiString sPath)
{
    AnsiString tmp, sDir;

    tmp=sPath+"\\";

    if(iSelect==1)
    {
        sDir="InArmX_Motor1Positive";
    }
    else if(iSelect==2)
    {
        sDir="InArmX_Motor2Positive";
    }
    else if(iSelect==3)
    {
        sDir="InArmY_Motor3Positive";
    }
    else if(iSelect==4)
    {
        sDir="InArmY_Motor4Positive";
    }
    else if(iSelect==5)
    {
        sDir="OutArmX_Motor5Positive";
    }
    else if(iSelect==6)
    {
        sDir="OutArmX_Motor6Positive";
    }
    else if(iSelect==7)
    {
        sDir="OutArmY_Motor7Positive";
    }
    else if(iSelect==8)
    {
        sDir="OutArmY_Motor8Positive";
    }

    tmp=tmp+sDir;

    tmp+=".csv";
    Memo->Lines->SaveToFile(tmp);

    Memo->Clear();
}
//<==
//KaiChen 20171228 ：Log Light Scale Data
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::Timer2Timer(TObject *Sender)
{
    Timer2->Enabled=false;
    fMotorTest->LightScale();
    if(bStopflag==false)
        Timer2->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::edDelayTimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1000, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::edPitechMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnSaveLogLightScaleDataClick(TObject *Sender)
{
    AnsiString strDate;

    strDate="D:\\LightScale";

    if(DirectoryExists(strDate)==false)
    {
        if(!MyForceDirectories(strDate))
        {
            return;
        }
    }

    strDate=FormatDateTime("yyyymmddhhmm", Now());
    strDate="D:\\LightScale\\LightScaleData_"+strDate;
    if(DirectoryExists(strDate)==false)
    {
        if(!MyForceDirectories(strDate))
        {
            return;
        }
    }

    SaveAsCSV_Kaichen(mmo1, 1, strDate);
    SaveAsCSV_Kaichen(mmo2, 2, strDate);
    SaveAsCSV_Kaichen(mmo3, 3, strDate);
    SaveAsCSV_Kaichen(mmo4, 4, strDate);
    SaveAsCSV_Kaichen(mmo5, 5, strDate);
    SaveAsCSV_Kaichen(mmo6, 6, strDate);
    SaveAsCSV_Kaichen(mmo7, 7, strDate);
    SaveAsCSV_Kaichen(mmo8, 8, strDate);

    iLogLightScaleCount_InArmX1     =0;
    iLogLightScaleCount_InArmX2     =0;
    iLogLightScaleCount_InArmY1     =0;
    iLogLightScaleCount_InArmY2     =0;
    iLogLightScaleCount_OutArmX1    =0;
    iLogLightScaleCount_OutArmX2    =0;
    iLogLightScaleCount_OutArmY1    =0;
    iLogLightScaleCount_OutArmY2    =0;

    ShowMessage("Save successfully!\nPath:"+strDate);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::BitBtn2Click(TObject *Sender)
{
    fMotorTest->LightScale(true);
    Timer2->Enabled=!Timer2->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::BitBtn3Click(TObject *Sender)
{
    if(fMotorTest->rgMoveType->ItemIndex==-1)
    {
        ShowMessage("往返動作選擇異常!");
        return;
    }
    else if(fMotorTest->rgAxis->ItemIndex==-1 || fMotorTest->rgAxis->ItemIndex==0)
    {
        ShowMessage("量測軸選擇異常!");
        return;
    }

    int iDelayTime;
    iDelayTime=atoi(edDelayTime->Text.c_str());
    SaveAsCSV(Memo1,iDelayTime);
    ShowMessage("Save successfully! Path:D:\\LightScale");
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::rgAxisClick(TObject *Sender)
{
    if(rgAxis->ItemIndex==1)
        iUseAxis=0;
    else if(rgAxis->ItemIndex==2)
        iUseAxis=1;
    else if(rgAxis->ItemIndex==3)
        iUseAxis=19;
    else if(rgAxis->ItemIndex==4)
        iUseAxis=20;
    else
        iUseAxis=-1;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbtReloadClick(TObject *Sender)
{
    TStringList *sList=new TStringList();
    TStringList *SL=new TStringList();

    int iCol=0, iRow=0;

    for(int i=0; i<strngrdMotorData->RowCount; i++)
    {
        for(int j=0; j<strngrdMotorData->ColCount; j++)
        {
            strngrdMotorData->Cells[j][i]="";
        }
    }

    if(FileExists(MotTablePath))
    {
        sList->LoadFromFile(MotTablePath);
        iRow=sList->Count;
        if(iRow>0)
        {
            strngrdMotorData->RowCount=iRow;
            for(int i=0; i<iRow; i++)
            {
                SL->CommaText=sList->Strings[i];

                if(SL->Count>iCol)
                {
                    iCol=SL->Count;
                    strngrdMotorData->ColCount=iCol;
                }

                for(int j=0; j<SL->Count; j++)
                {
                    strngrdMotorData->Cells[j][i]=SL->Strings[j];
                }
            }
        }
    }
    sList->Clear();
    SL->Clear();
    delete sList;
    delete SL;
    sbtReload->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    iSelectRow=ARow;
    iSelectCol=ACol;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnAddMotorClick(TObject *Sender)
{
    int iRow=strngrdMotorData->RowCount;
    strngrdMotorData->RowCount=iRow+1;
    for(int j=0; j<strngrdMotorData->ColCount; j++)
    {
        strngrdMotorData->Cells[j][iRow]="";
    }
    btnAddMotor->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnDeleteMotorClick(TObject *Sender)
{
    if(iSelectRow==0)
        return;
    for(int i=iSelectRow; i<strngrdMotorData->RowCount-1; i++)
    {
        for(int j=0; j<strngrdMotorData->ColCount; j++)
        {
            strngrdMotorData->Cells[j][i]=strngrdMotorData->Cells[j][i+1];
        }
    }
    strngrdMotorData->RowCount=strngrdMotorData->RowCount-1;
    btnDeleteMotor->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::btnModifyClick(TObject *Sender)
{
    if(iSelectRow>0)
    {
        Edit2->Text=strngrdMotorData->Cells[iSelectCol][iSelectRow];
        if(strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Motorname"))!=0 ||
           strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Alias"))!=0     ||
           strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("CardModel"))!=0)
        {
            fQwertyKey->ShowQwertyKey(Edit2, N_NO_SYMBOL|N_NO_SPACE);
        }
        else if(strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("GearRatio"))!=0 ||
                strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Acc"))!=0     ||
                strngrdMotorData->Cells[iSelectCol][0].AnsiPos(AnsiString("Dec"))!=0)
        {
            fQwertyKey->ShowQwertyKey(Edit2, N_DOUBLE);
        }
        else
        {
            fQwertyKey->ShowQwertyKey(Edit2, N_INTEGER);
        }
        strngrdMotorData->Cells[iSelectCol][iSelectRow]=Edit2->Text;
    }
    btnModify->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::sbUpdateClick(TObject *Sender)
{
    TStringList *sList=new TStringList();
    TStringList *SL=new TStringList();

    int iCol=0, iRow=0;

    iRow=strngrdMotorData->RowCount;
    iCol=strngrdMotorData->ColCount;

    for(int i=0; i<iRow; i++)
    {
        SL->Clear();

        for(int j=0; j<iCol; j++)
        {
            SL->Add(strngrdMotorData->Cells[j][i]);
        }
        sList->Add(SL->CommaText);
    }

    sList->SaveToFile(MotTablePath);
    sList->Clear();
    SL->Clear();
    delete sList;
    delete SL;

    sbUpdate->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataDblClick(TObject *Sender)
{
    btnModify->Click();
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    strngrdMotorData->MouseToCell(X, Y, iDragCol, iDragRow);

    if(iDragRow==0)
        strngrdMotorData->BeginDrag(false, 4);

    lblMotorName->Caption="Motor Name : "+ strngrdMotorData->Cells[1][iDragRow];
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    int CurrentCol, CurrentRow;
    strngrdMotorData->MouseToCell(X, Y, CurrentCol, CurrentRow);

    if(Sender==strngrdMotorData && CurrentRow>=0 && CurrentCol>=0)
    {
        Accept=true;
    }
    else
    {
        Accept=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::strngrdMotorDataDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
    TStringList *sList=new TStringList();
    int DestCol, DestRow;
    strngrdMotorData->MouseToCell(X, Y, DestCol, DestRow);
    if(iDragCol!=DestCol)
    {
        for(int i=0; i<strngrdMotorData->RowCount; i++)
        {
            sList->Add(strngrdMotorData->Cells[iDragCol][i]);
        }

        if(iDragCol>DestCol)
        {
            for(int j=iDragCol; j>DestCol; j--)
            {
                for(int i=0; i<strngrdMotorData->RowCount; i++)
                {
                    strngrdMotorData->Cells[j][i]=strngrdMotorData->Cells[j-1][i];
                }
            }
        }
        else if(iDragCol<DestCol)
        {
            for(int j=iDragCol; j<DestCol; j++)
            {
                for(int i=0; i<strngrdMotorData->RowCount; i++)
                {
                    strngrdMotorData->Cells[j][i]=strngrdMotorData->Cells[j+1][i];
                }
            }
        }

        for(int i=0; i<strngrdMotorData->RowCount; i++)
        {
            strngrdMotorData->Cells[DestCol][i]=sList->Strings[i];
        }
    }

    sList->Clear();
    delete sList;
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::edtMotorRateClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfMotorTest::edtMotorRangeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 1000);
}
//---------------------------------------------------------------------------
void TfMotorTest::LockAllButton(bool bLock)                                     //Steven 20230815 : Add for MotorTest
{
//    sbMotorTest_JogN->Enabled   =!bLock;                                      //不能鎖定Jog, 會導致功能失效
//    sbMotorTest_JogP->Enabled   =!bLock;
    sbMotorTest_MoveN->Enabled  =!bLock;
    sbMotorTest_MoveP->Enabled  =!bLock;
    btnLoopMove->Enabled        =!bLock;
    pnlStop->Color=(bLock)?clYellow:(TColor)0x00DFD9CC;                         //Steven 20230815 : Add for MotorTest
}
//---------------------------------------------------------------------------
void TfMotorTest::AllBtnUp()                                                    //Steven 20230815 : Add for MotorTest
{
    btnLoopMove->Down       =false;
    btnHome->Down           =false;
    btnGoSoftP->Down        =false;
    btnGoSoftN->Down        =false;
    sbMotorTest_MoveN->Down =false;
    sbMotorTest_MoveP->Down =false;
    btnGo->Down             =false;
}
//---------------------------------------------------------------------------
