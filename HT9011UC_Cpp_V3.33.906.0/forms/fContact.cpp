// =============================================================================
//  forms/fContact.cpp  --  the READ-ONLY half of golden's TfContact
//  (cContact.cpp).  44 of golden's 134 method definitions (794 of its 22,213
//  method-body lines -- quote both, see the header banner).
//
//  AI(W906-FW-CONTACT-W26) 20260826: new file, translation wave FW-CONTACT-W26.
//  Golden source of truth: HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp
//  (22,762 lines, cp950, 0 U+FFFD, decoded this wave).  The full banner --
//  SAFETY POSTURE, denominator + method, batch criterion, WAVE SCOPE, the 90
//  gates, GATE REGISTER, GOLDEN NOTES, DEVIATIONS, and the symbol/CMake
//  analysis -- lives in forms/fContact.h.  This file carries per-statement
//  pointer-back comments only.
//
//  ==========================================================================
//  *** HOW THE 90 GATES ARE ENFORCED IN THIS FILE -- READ BEFORE EDITING ***
//  ==========================================================================
//  Every gated method is DECLARED in forms/fContact.h and DELIBERATELY NOT
//  DEFINED here.  The LINKER is the interlock: calling one fails the build with
//  `undefined reference to TfContact::<method>`, naming the exact entry point.
//  An empty stub would link and read as "already translated"; an `#if 0` body
//  is one keystroke from pressing an IC into a socket.  TfContact has no
//  virtual member, so an undefined member costs nothing until it is called.
//
//  The three gates that sit INSIDE an otherwise-active body -- (C-1), (C-2),
//  (C-3) -- are marked with a `// GATE (C-n)` comment at the exact statement,
//  and each states what golden did and what the port does instead.
//  ==========================================================================
// =============================================================================
#include "forms/fContact.h"

#include "cContact.h"           // ComputeIsRun2DCheck / ComputeIndexYSpeed / ComputeIndexZSpeed
                                // + CONTACT_NORMAL (:81) / CONTACT_DEVICE_MAP_CHECK (:83)
#include "MachineType.h"        // CC_* customer codes, eTestMode / eContactMode / eVacummMode,
                                // Type_HT904x, CheckRange<T> (:1525), MAX_SOCKET_TOTAL (:393)
#include "cmydef.h"             // CUSTOMER_CODE, iContactMode, iTotalGf, bKitOutDiameter,
                                // sKitOutDiameter[], bRTCAutoTuning, bRun2DCheck, EP_Install,
                                // INSTALL_DOUBLE_EP + DOUBLE_EP_* , BAR_CODE_INSTALL,
                                // MachineTypeChoice, N_DOUBLE/N_INTEGER, MTestY1/MTestZ1
#include "cprod.h"              // DeviceForm / DeviceForm_File / TestIF_File / InputLimit / Teach
                                // + bDoRTCLearning (:1360)
#include "cpublic.h"            // Get0_01MMType (:19) / ConvertTouMType (:20)
#include "cUnitConvert.h"       // iUnitMultiply100 (:51)
#include "Config.h"             // IniConfig (:1499)
#include "CosFunction.h"        // CosFunction (:488)
#include "LastSet.h"            // LastSet (:587) -- LastSet.bUseTestSocket in DutCount
#include "common.h"             // ReadIniData / GetRecipeFileName / OnlyNumberInPut /
                                // OnlyNumberAndDotInPut / BmpPath
#include "vclcompat/SysUtils.h" // FormatFloat (:41) / FileExists (:90)
#include "canary_support.h"     // ShowMyMessage (:80)
#include "aHotPlateSubstrate.h" // InArmSuck / OutArmSuck (:624/:627) -- THE 177-TU TMyKitSuck,
                                // NOT mykitsuck.h's (docs/KNOWLEDGE.md: two TMyKitSuck, different
                                // layout, 14 colliding globals; picking the wrong one links clean
                                // and reads the wrong offsets).  Also iCloseSiteModeFor2x8 (:868).
#include "csystem.h"            // IndexHasIC (:140) / ShuttleHasIC (:135) -- see the DIET RISK note
                                // in forms/fContact.h's "SYMBOLS THIS TU NEEDS"
#include "Motor/mymotor.h"      // MOT[] (:385) -- only MOT[..].GailSpeed (:196), a plain int READ
#include "BarcodeReader.h"      // Barcode_Reader (:111)
#include "EJ1N/TextProcess.h"   // MyFormatFloat (:135)
#include "forms/fQwertyKey.h"   // fQwertyKey->ShowQwertyKey (:370)

#include <cstdlib>              // atoi / atof

// ---------------------------------------------------------------------------
//  TfContact::TfContact -- PORT CTOR.  FIELDS ONLY (DEVIATION D-2).
//
//  Golden's ctor body (cContact.cpp:107-276) is NOT here; it is Init().
//  Why that split is load-bearing for THIS form specifically: golden's body
//  reads CUSTOMER_CODE / CosFunction / IniConfig / MachineTypeChoice /
//  INSTALL_DOUBLE_EP, calls FileExists() on an absolute path, calls
//  InitContactEdtList() and InitRTCAutoTuning(), and `new`s a TStringList --
//  all cross-TU work with unspecified construction order.  That exact shape
//  cost this tree 88 SEGFAULTs of 134 ctests once already (fLaserSensor's ctor
//  calling InitLaserEdtList() with elLaser still NULL, docs/KNOWLEDGE.md).
//
//  What IS here:
//    (a) the BCB6 TObject zero-fill equivalents.  Golden never initialises
//        fShow/bSetupStart/.../iDMCTrayX[] in its ctor for the members it does
//        not name, and relies on TObject::NewInstance zero-filling the whole
//        instance.  Plain C++ has no such fill, so this reproduces golden's
//        OBSERVABLE state rather than changing it.  (The five golden DOES
//        assign -- fShow/bSetupStart/bSetupStep/dDutCount + the speed block --
//        are assigned again in Init(), verbatim, exactly where golden has them.)
//    (b) the `.dfm`-sourced design-time values that translated code READS --
//        DEVIATION (D-6), and (D-7) for the two Tag=1 that are the difference
//        between faithful and merely compiling.
// ---------------------------------------------------------------------------
TfContact::TfContact()
    : bAutoHighFinish(false),
      iSpeedZ(0),
      iSpeedZSlow(0),
      iSpeed(0),
      TorqueData(0),
      MotorStatus(false),
      bContinueContact(false),
      RELEASE_UP_BIG(0),
      RELEASE_UP_SMALL(0),
      bOldRTCAutoTuning(false),
      itemp(0),
      brecordmsgLock(false),
      iSH1AutoLtcErr(0),
      iSH2AutoLtcErr(0),
      ErrPart_InSH1Ltc(""),
      ErrPart_InSH2Ltc(""),
      iDoFullViewCheck(0),
      Z_Height_Task(0),
      Z_PickFromSocketTask(0),
      bSetupStart(false),
      fShow(false),
      bSetupStep(false),
      bOneCycleFinish(false),
      dDutCount(0.0),
      CarlibrationTask(0),
      iChkInSHLatchTask(0),
      ROILearningTimes(0),
      bSetHasIC(false),
      iDeviceMapCheckTask(0),
      i2DIDCheckTask(0),
      i2DIDMapCheckLoadTask(0),
      i2DIDMapCheckUnloadTask(0),
      iDMCArm(0),
      bWaitLoadXY(false),
      bTrayFeed(false),
      bPlaceLoad(false),
      iTrayX(0),
      iTrayY(0),
      ROILearningTask(0),
      AutoContactTestTask(0),
      StepContactTestLoadTask(0),
      StepContactTestUnloadTask(0),
      sOutDiameter(0),
      iDoArm1PlaceToShuttleTask(0),
      iDoArm2PlaceToShuttleTask(0),
      dMinForce(0.0),
      iRTCAutoTuningTask(0),
      iStepContactTestKitTask(0),
      iStepContactTestUnKitTask(0),
      bUseTwoArm(false),
      iRunWhichArm(0),
      iTaskCaliAboveHeight(0),
      iAutoTeachInSHLtcStatus(0)
{
    // (a) BCB6 zero-fill equivalents for the arrays.
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 8; ++j)
        {
            iFTestBackItem[i][j] = 0;
            iBTestBackItem[i][j] = 0;
        }
    for (int i = 0; i < MAX_SOCKET_TOTAL; ++i)
    {
        iDMCTrayX[i]   = 0;
        iDMCTrayY[i]   = 0;
        iDMCChannel[i] = 0;
    }
    bUniversalkitflag[0] = false;
    bUniversalkitflag[1] = false;

    // -----------------------------------------------------------------------
    // (b) DEVIATION (D-6) -- `.dfm` design-time values that translated code
    //     READS.  Each cited; only the ones that are load-bearing are set (the
    //     values whose vclcompat default already equals the .dfm value are
    //     deliberately NOT restated -- see D-6).
    // -----------------------------------------------------------------------
    scrbSLK->Max      = 5;      // cContact.dfm:16129
    scrbSLK->Min      = 2;      // cContact.dfm:16130
    scrbSLK->Position = 2;      // cContact.dfm:16132 -- READ by DutCount (:2063-2068)
                                //   and CheckContactSettingChange (:17074)

    // DEVIATION (D-7) -- THE Tag PROVENANCE TRAP.  edForcePerPinNMouseDown
    // (:18547) and edForcePerPinGMouseDown (:19034) branch on Tag==1 to decide
    // whether the field is a BARCODE-SCAN target; vclcompat::TControl::Tag has
    // no .dfm loader (Controls.h:238-254), so an uninitialised 0 would SKIP the
    // barcode gate entirely -- a wrong branch, not a no-op.
    edForcePerPinN->Tag = 1;    // cContact.dfm:16202
    edForcePerPinG->Tag = 1;    // cContact.dfm:16020
    edPinCount->Tag     = 1;    // cContact.dfm:16181 (read by Edit16MouseDown, GATE (X-24))

    edOutDiameter->Text = "40,55,75,108";   // cContact.dfm:350

    // READ by CheckContactSettingChange (:17067 / :17069).  vclcompat::
    // TRadioGroup defaults ItemIndex to 0, so leaving these would invert the
    // Run-check for these two settings.
    rgSuckShuttleDeviceAfterTested->ItemIndex   = 1;   // cContact.dfm:17693
    rgSuckShuttleDeviceWaitOnShuttle->ItemIndex = 1;   // cContact.dfm:17638

    // DEVIATION (D-9) -- the Items lists delivered code can READ.  golden's
    // ctor only replaces these under specific customer/config branches; outside
    // them the .dfm lists stand, and rgDieForceKitDiameterClick (:19505) does
    // `atof(Items->Strings[ItemIndex])/10.0`.  An empty list would silently
    // yield dDieForceKitDiameter = 0.0.
    rgKitDiameter->Items->Add("30");             // cContact.dfm:16174
    rgKitDiameter->Items->Add("40");             // cContact.dfm:16175
    rgKitDiameter->Items->Add("60");             // cContact.dfm:16176
    rgKitDiameter->Caption = "Kit Diameter (mm)";// cContact.dfm:16170

    rgOutKitDiameter->Items->Add("40mm");        // cContact.dfm:16267
    rgOutKitDiameter->Items->Add("55mm");        // cContact.dfm:16268
    rgOutKitDiameter->Items->Add("75mm");        // cContact.dfm:16269
    rgOutKitDiameter->Caption = "Kit Out  Diameter";  // cContact.dfm:16263

    rgDieForceKitDiameter->Items->Add("30");     // cContact.dfm:16498
    rgDieForceKitDiameter->Items->Add("40");     // cContact.dfm:16499
    rgDieForceKitDiameter->Items->Add("60");     // cContact.dfm:16500
    rgDieForceKitDiameter->Caption = "Die Force Kit Diameter (mm)";  // cContact.dfm:16494

    coD41->Items->Add("Inside socket");          // cContact.dfm:18593
    coD41->Items->Add("Above Socket");           // cContact.dfm:18594

    // cbContactMode's .dfm list is deliberately NOT seeded: golden's own Init()
    // Clear()s and rebuilds it on BOTH arms of its branch (:205-235), so the
    // .dfm value is unreachable the moment Init() runs.  See DEVIATION (D-9).
}

// ---------------------------------------------------------------------------
//  TfContact::~TfContact -- golden :279-281.
//  Golden's body is EMPTY and stays empty.  The widget frees below are
//  PORT-ONLY (DEVIATION D-3): golden's TForm owner frees them, and this facade
//  has no owner.
// ---------------------------------------------------------------------------
TfContact::~TfContact()
{
    // -- golden body :280 (empty) --

    // -- PORT-ONLY (D-3), in golden's member-declaration order --
    delete pnlBottom;            delete sbtExit;              delete spbSave;
    delete btnStart;             delete btnPause;             delete pnlSLKSetting;
    delete gbContactForce;       delete imgSLK;               delete lblContactForce;
    delete lblForceN;            delete lblMaxForce;          delete lblForcePerDevice;
    delete lblForcePerDeviceKg;  delete lblForcePerDeviceN;   delete lblYDimension;
    delete lblXDimension;        delete lblReadEP;            delete edAirForce;
    delete edAirForceN;          delete edForcePerDeviceKG;   delete edForcePerDeviceN;
    delete edYDimension;         delete edXDimension;         delete scrbSLK;
    delete btnTStart;            delete btnTStep;             delete SetMPaBtn;
    delete rgKitDiameter;        delete btContactForce;       delete palOTD_4;
    delete pnlTop;               delete gbContactHeight;      delete imgIndex;
    delete lblReleaseHeight;     delete lblPickHeight;        delete lblDropOffset;
    delete lblContactHeight;     delete lblReleaseHeightMM;   delete lblPickHeightMM;
    delete lblDropOfsMM;         delete lblContactHighMM;     delete edReleaseHeight1;
    delete edPickUp1;            delete edDropOffset1;        delete edtSht1;
    delete edtSht2;              delete edtArm1;              delete edContactHeight1;
    delete edReleaseHeight2;     delete edPickUp2;            delete edDropOffset2;
    delete edContactHeight2;     delete edtArm2;              delete gbContactParameter;
    delete lblContactMode;       delete lblVacMode;           delete lblDummyContact;
    delete lblDropWaitTime;      delete lblSuckShuttleDeviceAfterTested;
    delete lblShuttleWaitingOutSiteChamber;                   delete lblDropSpeed;
    delete cbContactMode;        delete cbVacuumMode;         delete pnlDummyContact;
    delete rbNormal;             delete rbDummyMode;          delete edDropWaitTime;
    delete rgSuckShuttleDeviceAfterTested;                    delete rgShuttleWaitingOutSiteChamber;
    delete edDropSpeed;          delete gbDynaTemp;           delete btAt;
    delete btShowDynaTemp;       delete btTempOffset;         delete edSpeed;
    delete edSpeedZ;             delete lblIndexSpeedY;       delete lblIndexSpeedZ;
    delete rgPSDT;               delete lblPSDT;              delete lblOrgHeight;
    delete edContactBackUp1;     delete edContactBackUp2;     delete pnlTorqueControl;
    delete lblTorqueMax;         delete lblTorqueCompare;     delete edtTorqueMax;
    delete edtTorqueCmp;         delete labContactOffset;     delete edContactOffsetArm1;
    delete edContactOffsetArm2;  delete palOTD_6;             delete pnlSensorAdj;
    delete ledOTD;               delete OTDTimer;             delete lblLayoutkit;
    delete lblSLKWarning;        delete lblOrgPick;           delete edOrgPick1;
    delete edOrgPick2;           delete palHeightCalibration; delete pnlFuncSelect;
    delete chkShuttle;           delete cbRTCModel;           delete lblContactOfsMM;
    delete lblOrgHeightMM;       delete panHeightMode;        delete rgHandlerMode;
    delete lbAutoContactTest;    delete rbModeNormal;         delete rbAutoHeight;
    delete rbManualHeight;       delete rbContactTest;        delete edAutoContactTest;
    delete gbContactStatus;      delete Memo1;                delete gbHeightStatus;
    delete lblContact;           delete lblTorque;            delete lblArm1Sts;
    delete lblArm2Sts;           delete lblContactMM;         delete lblTorque1;
    delete palArm1Height;        delete palArm2Height;        delete PnlTorue0;
    delete PnlTorue1;            delete palTestSocket;        delete labD41;
    delete coD41;                delete edD41;                delete lblICCheck;
    delete pnlPurge;             delete lbAirPurgeBeforePickFromShuttle;
    delete lblPurgeTime;         delete lblPurgeInterval;     delete lblPurgeOffset;
    delete lblPurgeTimeS;        delete lblPurgeIntervalT;    delete lblPurgeOffsetMM;
    delete rgPurgeBeforePickShuttle;                          delete edtPurgeBeforePickShuttleTime;
    delete edtPurgeBeforePickShuttleInterval;                 delete edtPurgeBdforePickShuttleOffSet;
    delete lblPinCount;          delete edPinCount;           delete edForcePerPinN;
    delete edForcePerPinG;       delete lblPerGf;             delete lblPerPinN;
    delete rbAutoContactTest;    delete rbStepContactTest;    delete btEditTray;
    delete labSetKg;             delete edSetKg;              delete labTransfer;
    delete edTransfer;           delete spbOneCycle;          delete ledOneCycle;
    delete labShtOffset;         delete edShtPickOffset1;     delete edShtPickOffset2;
    delete lblOrgPickMM;         delete lblShtPickOfsMM;      delete btOffset;
    delete cb2DMatrix;           delete lblEPValueKpa;        delete lblEPValueKg;
    delete palSoakTimeWating;    delete lblWarning1;          delete palSoakTime;
    delete lbHisiVer;            delete btBarcode;            delete lblAirForce;
    delete edAirKPA;             delete labMPa;               delete TimerEP;
    delete cbSFCAutoTune;        delete palSFCInformation;    delete plIndexArmJogMove;
    delete lblJogMove;           delete btnIndexArmJogMove_Up;
    delete btnIndexArmJogMove_Down;                           delete cbIndexArmJogMove_mm;
    delete lblJogMoveMM;         delete lblSuckDevicesDuringTest;
    delete rgSuckDevicesDuringTest;                           delete lblIndexUpSpeed;
    delete rgIndexUpSpeed;       delete rgOutKitDiameter;     delete chkUseAddWeight;
    delete palTorqueArm1;        delete palTorqueArm2;        delete cbEnableUK;
    delete chkTeachInOutArmZ;    delete rgSuckShuttleDeviceWaitOnShuttle;
    delete lblSuckShuttleDeviceWaitOnShuttle;                 delete pnlHandlerSatus;
    delete labDelayStatus;       delete lblCountDown;         delete btnTempSet;
    delete lblMinForce;          delete rbDeviceMapping;      delete gbDoubleEPSet;
    delete lblDieForceEP;        delete lblLoadCellHeight;    delete edLoadCellHeight1;
    delete edLoadCellHeight2;    delete lblLoadCellHighMM;    delete rbLoadCellAutoHigh;
    delete rgDieForceKitDiameter;                             delete lblDieForce;
    delete edDoubleForce;        delete lblDieForceKg;        delete lblNoOfBall;
    delete edtPinOfDie;          delete edAutoKSHTReleaseOfs; delete lblReleaseOfs;
    delete btnTrayMap;           delete lblMaxForcePerIC;     delete chk2DID;
    delete AutoZTeachButton;     delete edOutDiameter;        delete lblUpWaitTime;
    delete edUpWaitTime;         delete lblUpSpeed;           delete edUpSpeed;
    delete lblUpOffset;          delete lblUpOfsMM;           delete edUpOffset2;
    delete edUpOffset1;          delete btTCPIP;              delete edDropByPassDetect;
    delete labbUserDefMaxContactHeight;                       delete pnlSidePush;
    delete lbTesterSidePush;     delete rgTesterSidePush;     delete edSidePushWaitTime;
    delete lblSidePushWaitTime;  delete rgSidePushMode;       delete lblSitePushMode;
    delete rbTemp;               delete labEPValue;           delete labEPValue_1032;
    delete lblReadEP2;           delete cbRTCAutoTuning;      delete lblDropByPassDetect;
    delete chk_K_Temperature;    delete rbKTempIndexMove;     delete rbDeviceLoopTest;
    delete edDeviceLoopTest;     delete cbOneTouchAutoContactHight;
    delete timerContact;         delete edDieForcePerPinN;    delete edDieForcePerPinG;
    delete Label1;               delete Label2;               delete cbTestContactMode;
    delete edTestContactCount;   delete lblNowCount;          delete lblCount;
    delete lblTsetSec;           delete edTestSec;            delete chkDailyCorrelation;
    delete labContactRelative;   delete edContactRelativeZ1;  delete edContactRelativeZ2;
    delete cbCalibrateAboveHeight;                            delete cbTeachInSHSen;
    delete lblATCTempWait;
}

// ---------------------------------------------------------------------------
//  TfContact::Init -- golden ctor BODY, cContact.cpp:107-276 (DEVIATION D-2).
//  Translated verbatim apart from the two marked gates.
// ---------------------------------------------------------------------------
void TfContact::Init()
{
    if(CUSTOMER_CODE==CC_JCET)                                                  // golden :108  //RogerYang 20260129 : 凌中心不想要人員在右上角關閉視窗時掉料 -> 把X收起來
    {
        // GATE (C-2) -- golden :110 `this->BorderIcons=TBorderIcons();`.
        // `TBorderIcons` has zero port and this facade has no window chrome, so
        // the statement has no offline consumer.  Pure decoration; gating it
        // changes no translated value.
    }

    brecordmsgLock=false;                                                       // golden :113
    AnsiString FileName="D:\\HT9045\\system\\ContactInfo.ini";                  // golden :114
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         // golden :115  //kevin 20130724 高雄日月光 contact修改成NS顯示
    {
        if(rgKitDiameter->Items->Text!="")                                      // golden :117
            rgKitDiameter->Items->Clear();                                      // golden :118
        rgKitDiameter->Caption= "Kit Diameter (Square)";                        // golden :119
        rgKitDiameter->Items->Add("40 mm");                                     // golden :120
        rgKitDiameter->Items->Add("55 mm");                                     // golden :121
        rgKitDiameter->Items->Add("75 mm");                                     // golden :122
        rgKitDiameter->Items->Add("20 mm");                                     // golden :123
        bKitOutDiameter =true;                                                  // golden :124
        rgOutKitDiameter->Items->Add("40 mm");                                  // golden :125  //kevin 20170628 (Steven) add for ASE-KH
        rgOutKitDiameter->Items->Add("55 mm");                                  // golden :126
        rgOutKitDiameter->Items->Add("75 mm");                                  // golden :127
        rgOutKitDiameter->Items->Add("20 mm");                                  // golden :128
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              // golden :131  //wei 20151005 add 56mm
    {
        if(rgKitDiameter->Items->Text!="")                                      // golden :133
            rgKitDiameter->Items->Clear();                                      // golden :134
        rgKitDiameter->Caption= "Kit Diameter";                                 // golden :135
        rgKitDiameter->Items->Add("28 mm");                                     // golden :136
        rgKitDiameter->Items->Add("40 mm");                                     // golden :137
        rgKitDiameter->Items->Add("58 mm");                                     // golden :138
        rgKitDiameter->Items->Add("56 mm");                                     // golden :139
    }

    if(CosFunction.bUseDynamicKitDiameter)                                      // golden :142  //Steven 20170605 (wei) : 可以自定義Kit直徑
    {
        rgKitDiameter->Items->Clear();                                          // golden :144

        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          // golden :146
        {
            // GATE (C-1) -- golden :148-151 is
            //     if(FileExists(FileName)) Items->CommaText=CheckAndReadIniData(FileName,"SLK Type","Type","28,40,58,56");
            //     else                     Items->CommaText="28,40,58,56";
            // CheckAndReadIniData WRITES the default back when the key is
            // absent (common.cpp:602-603 WriteInteger), so the `if` arm is a
            // write to D:\HT9045\system\ContactInfo.ini.  The port keeps
            // golden's own `else` arm verbatim -- i.e. it behaves exactly as
            // golden does before that .ini has been created.  That is a real
            // golden state, not an invented one.
            rgKitDiameter->Items->CommaText=AnsiString("28,40,58,56");          // golden :151 (else arm)
        }
        else                                                                    // golden :153
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 // golden :155  //kevin 20210813 高雄日月光 contact修改顯示
            {
                rgOutKitDiameter->Items->Clear();                               // golden :157  //kevin 20210814 add
                // GATE (C-1) -- golden :158-163 (three CheckAndReadIniData);
                // else arm kept, golden :165-169.
                rgKitDiameter->Items->CommaText=AnsiString("30,40,60");         // golden :166
                rgOutKitDiameter->Items->CommaText=AnsiString("40,55,75");      // golden :167  //kevin 20210814 add  OutDiameter
                edOutDiameter->Text=("40,55,75,20");                            // golden :168
                sOutDiameter = new TStringList();                               // golden :170
                sOutDiameter->Clear();                                          // golden :171
                sOutDiameter->CommaText=edOutDiameter->Text;                    // golden :172

                for(int i=0; i<sOutDiameter->Count; i++)                        // golden :174
                {
                    sKitOutDiameter[i]="";                                      // golden :176  //Ztex 2024.08.30 Add Clear sKitOutDiameter Array
                    if(sOutDiameter->Strings[i]!="" && atof(sOutDiameter->Strings[i].c_str())>15.0)   // golden :177
                       sKitOutDiameter[i]=sOutDiameter->Strings[i];             // golden :178
                }
                sOutDiameter->Clear();                                          // golden :180
                delete sOutDiameter;                                            // golden :181
                // GOLDEN NOTE, faithful: golden deletes the list but does NOT
                // null the member, leaving sOutDiameter dangling for the rest of
                // the object's life.  No translated body reads it, so the port
                // reproduces the dangle rather than "fixing" it; recorded so a
                // future wave that starts reading sOutDiameter knows.
            }
            else                                                               // golden :183
            {
                // GATE (C-1) -- golden :185-188; else arm kept.
                rgKitDiameter->Items->CommaText=AnsiString("30,40,60,56");      // golden :188
            }
        }
    }
    InitContactEdtList();                                                       // golden :192

    if(INSTALL_DOUBLE_EP==DOUBLE_EP_NORMAL || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)   // golden :194  //Ifor 20191003 //AI(ht9045-v899) 20260430: extend Die Force kit selection to Multi EP half.
    {
        rgDieForceKitDiameter->Items->Clear();                                  // golden :196
        // GATE (C-1) -- golden :197-200; else arm kept.
        rgDieForceKitDiameter->Items->CommaText=AnsiString("20,30,40,50");      // golden :200
    }

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           // golden :203
    {
        cbContactMode->Items->Clear();                                          // golden :205
        cbContactMode->Items->Add("Direct Contact");                            // golden :206
        cbContactMode->Items->Add("Drop Contact");                              // golden :207
        cbContactMode->Items->Add("Direct & Soft Contact");                     // golden :208
        cbContactMode->Items->Add("TMOVE Contact");                             // golden :209
        cbContactMode->Items->Add("TMOVE Drop Contact");                        // golden :210
        cbContactMode->Items->Add("Direct & Soft EP Contact");                  // golden :211
        cbContactMode->Items->Add("Drop & Soft EP Contact");                    // golden :212
        cbContactMode->Items->Add("Shift Contact for 8Site 1x4");               // golden :213
        cbContactMode->Items->Add("Drop & Slow Contact");                       // golden :214
        cbContactMode->Items->Add("TMOVE Slow Contact");                        // golden :215
        cbContactMode->Items->Add("TMOVE Drop & Slow Contact");                 // golden :216
    }
    else                                                                        // golden :218
    {
        cbContactMode->Items->Clear();                                          // golden :220
        cbContactMode->Items->Add("Direct Contact");                            // golden :221
        cbContactMode->Items->Add("Drop Contact");                              // golden :222
        if(CUSTOMER_CODE==CC_SPIL_CS)                                           // golden :223  //JerryYang 20260415 : 矽品中山要求Follow台積電顯示名稱
            cbContactMode->Items->Add("Direct & Soft Contact");                 // golden :224
        else
            cbContactMode->Items->Add("Direct & Slow Contact");                 // golden :226
        cbContactMode->Items->Add("TMOVE Contact");                             // golden :227
        cbContactMode->Items->Add("TMOVE Drop Contact");                        // golden :228
        cbContactMode->Items->Add("Direct & Soft EP Contact");                  // golden :229
        cbContactMode->Items->Add("Drop & Soft EP Contact");                    // golden :230
        cbContactMode->Items->Add("Shift Contact for 8Site 1x4");               // golden :231
        cbContactMode->Items->Add("Drop & Slow Contact");                       // golden :232
        cbContactMode->Items->Add("TMOVE Slow Contact");                        // golden :233
        cbContactMode->Items->Add("TMOVE Drop & Slow Contact");                 // golden :234
    }

    fShow=false;                                                                // golden :237
    bSetupStart=false;                                                          // golden :238
    bSetupStep=false;                                                           // golden :239
    dDutCount=2;                                                                // golden :240

    iSpeed=30;                                                                  // golden :242
    iSpeedZ=30;                                                                 // golden :243  //Steven 20110503 : 240KG 加快速度
    TorqueData=0;                                                               // golden :244
    MotorStatus=false;                                                          // golden :245
    bContinueContact=false;                                                     // golden :246
    bOldRTCAutoTuning=false;                                                    // golden :247  //Sam 20240711 : RTC Auto Tuning
    InitRTCAutoTuning();                                                        // golden :248  //Sam 20230419
    if(MachineTypeChoice==Type_HT9046)                                          // golden :249  //9046
        scrbSLK->Max=6;                                                         // golden :250
    else if(MachineTypeChoice==Type_HT9045_12Site)                              // golden :251  //ChungHung 20130507 add HT9045 updata for 12site 517
        scrbSLK->Max=6;                                                         // golden :252
    else
        scrbSLK->Max=5;                                                         // golden :254

    if(MachineTypeChoice==Type_HT9046_LS)                                       // golden :256  //Steven 20140516 : for 1024x768解析度
    {
        if(IniConfig.bSIGURDFunction)                                           // golden :258  //Sam 20211230 : 矽格先卡這個高度
            gbContactParameter->Height=230;                                     // golden :259
        else
            gbContactParameter->Height=265;                                     // golden :261  //kevin 20190913 //ChungHung 20150519 modify 169--->233  //wei 20170526 (jou) 233->265
    }
    else
    {
        gbContactParameter->Height=230;                                         // golden :265  //ChungHung 20150519 modify 126--->177  //wei 20170526 (jou) 177->230
    }

    ROILearningTimes=20;                                                        // golden :268  //Steven 20120222 : ROI的學習次數
    //OTDTimer->Enabled=(USE_OTD);                                              // golden :269 (commented out in golden)  //Steven 20140312 : OTD
    iDoArm1PlaceToShuttleTask=1;                                                // golden :270  //JimmyChiu 20220708 : add Index Arm Axis
    iDoArm2PlaceToShuttleTask=1;                                                // golden :271

    iAutoTeachInSHLtcStatus=eTeachSHLtcNomal;                                   // golden :273  //KenHsieh 20250909
    #ifndef BOOST_FUNCTION_TEST                                                 // golden :274 (MachineType.h:34 leaves it undefined -> this arm is LIVE)
    btnTempSet->Visible=false;                                                  // golden :275
    #endif                                                                      // golden :276
}

// ---------------------------------------------------------------------------
//  InitContactCbList / InitContactEdtList -- golden :352-354 / :356-361.
//  GOLDEN NOTE (G-EmptyBodies): both are genuinely empty in golden.  Kept with
//  golden's own comment text so a later reader does not "discover" them as
//  unported.  InitContactEdtList IS reachable -- Init() calls it (golden :192).
// ---------------------------------------------------------------------------
void TfContact::InitContactCbList()
{
}

void TfContact::InitContactEdtList()                                            //Steven 20190614 : 重新整理Edit to 變數的方式
{
//    bool bShow=true, bNoShow=false;                                           // golden :358
//    bool bEnable=true, bDisable=false;                                        // golden :359
//    bool bReadFromFile=true, bFixedValue=false;                               // golden :360
}

// ---------------------------------------------------------------------------
//  edDropWaitTimeKeyPressInt -- golden :1809-1813
// ---------------------------------------------------------------------------
void TfContact::edDropWaitTimeKeyPressInt(TObject *Sender, char &Key)
{
    (void)Sender;
    if(OnlyNumberAndDotInPut(Key)==false)                                       // golden :1811
        Key=NULL;                                                               // golden :1812
}

// ---------------------------------------------------------------------------
//  DutCount -- golden :1959-2072.
//  GOLDEN NOTE (G-DutFallthrough): the switch has NO `default:`; an iTestMode
//  outside the 17 listed leaves dDutCount at its previous value.  Verbatim.
// ---------------------------------------------------------------------------
void TfContact::DutCount()
{
    AnsiString asStr;                                                           // golden :1961
    switch(TestIF_File.iTestMode)                                               // golden :1962  //jou 2014-09-06 修正開啟程式的時候EP異常
    {
        case SingleSite:                                                        // golden :1964
            dDutCount=1;
            break;
        case DualSite:                                                          // golden :1967  //1x2
        case QualSite2X2N:                                                      // golden :1968  //Frank 20200520 2X2NN Mode
//        case DualSiteBS: //1x1 busyshuttle                                    // golden :1969
            if(IniConfig.bD27UseSingleSite85kg==true)                           // golden :1970
            {
                if((LastSet.bUseTestSocket[0][0][1]==false && LastSet.bUseTestSocket[1][0][1]==false)  ||   // golden :1972  //ChungHung 20130910 alter for SCK can close site by Index
                   (LastSet.bUseTestSocket[0][0][0]==false && LastSet.bUseTestSocket[1][0][0]==false))     // golden :1973  //Steven 20110915 : 1x2關Site,單Dut要可以壓到85KG
                    dDutCount=1;                                                // golden :1974
                else
                    dDutCount=2;                                                // golden :1976
            }
            else
            {
                dDutCount=2;                                                    // golden :1980
            }
            break;
        case DualSite2x1:                                                       // golden :1983
            if(IniConfig.bD27UseSingleSite85kg==true)                           // golden :1984
            {
                if((LastSet.bUseTestSocket[0][1][0]==false && LastSet.bUseTestSocket[1][1][0]==false) ||    // golden :1986
                   (LastSet.bUseTestSocket[0][0][0]==false && LastSet.bUseTestSocket[1][0][0]==false))      // golden :1987
                    dDutCount=1;                                                // golden :1988
                else
                    dDutCount=2;                                                // golden :1990
            }
            else
            {
                dDutCount=2;                                                    // golden :1994
            }
            break;
        case _6Site2X3N:                                                        // golden :1997  //Steven 20220425 : 2X3NN Mode
        case TriSite1X3:                                                        // golden :1998
            dDutCount=3;                                                        // golden :1999
            break;
        case QualSite1X4:                                                       // golden :2001  //1x4
        case QualSite2X2:                                                       // golden :2002  //2x2
        case _8Site1X4:                                                         // golden :2003  //ChungHung 20150528 add for 海思 _8Site1x4
        case _8Site2X4N:                                                        // golden :2004  //Wei 20231211 : 2X4NN Mode
//        case QualSite2X2BS: //2x2 busyshuttle                                 // golden :2005
            dDutCount=4;                                                        // golden :2006
            break;
        case _6Site2X3:                                                         // golden :2008  //ChungHung 20140115 add for 2x3_6
            dDutCount=6;                                                        // golden :2009
            break;
        case _8Site2X4:                                                         // golden :2011  //2x4
            if(TestIF_File.bOctal_12Kit)                                        // golden :2012  //ChungHung 20140508 add for SCK
                dDutCount=12;                                                   // golden :2013
            else
                dDutCount=8;                                                    // golden :2015
            break;
        case _10Site2X5:                                                        // golden :2017  //wei 20190614 10 site
            dDutCount=10;                                                       // golden :2018
            break;
        case _12Site2X6:                                                        // golden :2020
            dDutCount=12;                                                       // golden :2021
            break;
        case _16Site2X8:                                                        // golden :2023  //2x8
            if(TestIF_File.iSiteMap[0][0]<=0 && TestIF_File.iSiteMap[0][2]<=0 && TestIF_File.iSiteMap[0][4]<=0 && TestIF_File.iSiteMap[0][6]<=0 &&
               TestIF_File.iSiteMap[1][0]<=0 && TestIF_File.iSiteMap[1][2]<=0 && TestIF_File.iSiteMap[1][4]<=0 && TestIF_File.iSiteMap[1][6]<=0)   // golden :2024-2025  //Steven 20221122 : 16 site關site壓力顯示修正
            {
                dDutCount=8;                                                    // golden :2027
            }
            else if(TestIF_File.iSiteMap[0][1]<=0 && TestIF_File.iSiteMap[0][3]<=0 && TestIF_File.iSiteMap[0][5]<=0 && TestIF_File.iSiteMap[0][7]<=0 &&
                    TestIF_File.iSiteMap[1][1]<=0 && TestIF_File.iSiteMap[1][3]<=0 && TestIF_File.iSiteMap[1][5]<=0 && TestIF_File.iSiteMap[1][7]<=0)  // golden :2029-2030
            {
                dDutCount=8;                                                    // golden :2032
            }
            else if(TestIF_File.iSiteMap[0][0]<=0 && TestIF_File.iSiteMap[0][2]<=0 && TestIF_File.iSiteMap[0][4]<=0 && TestIF_File.iSiteMap[0][6]<=0 &&
                    TestIF_File.iSiteMap[1][1]<=0 && TestIF_File.iSiteMap[1][3]<=0 && TestIF_File.iSiteMap[1][5]<=0 && TestIF_File.iSiteMap[1][7]<=0)  // golden :2034-2035
            {
                dDutCount=8;                                                    // golden :2037
            }
            else if(TestIF_File.iSiteMap[0][1]<=0 && TestIF_File.iSiteMap[0][3]<=0 && TestIF_File.iSiteMap[0][5]<=0 && TestIF_File.iSiteMap[0][7]<=0 &&
                    TestIF_File.iSiteMap[1][0]<=0 && TestIF_File.iSiteMap[1][2]<=0 && TestIF_File.iSiteMap[1][4]<=0 && TestIF_File.iSiteMap[1][6]<=0)  // golden :2039-2040
            {
                dDutCount=8;                                                    // golden :2042
            }
            else if(iCloseSiteModeFor2x8>e2x8Standard)                          // golden :2044  //Steven 20260420 : != --> >
            {
                dDutCount=8;                                                    // golden :2046
            }
            else
            {
                dDutCount=16;                                                   // golden :2050
            }
            break;
        case _16Site4X4:                                                        // golden :2053  //Sam 20190226 : 16Site4X4
            dDutCount=8;                                                        // golden :2054  //KenHsieh 20230313 : NN Mode Dutcount新增
            break;
        case _32Site4X8N:                                                       // golden :2056
            dDutCount=16;                                                       // golden :2057  //KenHsieh 20230313 : NN Mode Dutcount 32 -> 16
            break;
    }

    if(TestIF_File.iTestMode==SingleSite)                                       // golden :2061  //JerryYang 20171215 (Steven) Single site只支援浮動頭1對1或是2對1
    {
        if(scrbSLK->Position==3 || scrbSLK->Position==6)                         // golden :2063
            scrbSLK->Position=5;                                                // golden :2064
        else if(scrbSLK->Position==4)                                           // golden :2065
            scrbSLK->Position=2;                                                // golden :2066
    }
    DeviceForm_File.iHeadDeviceCT=scrbSLK->Position;                            // golden :2068  //jou 2014-04-24 修正選擇head數時,force值不會馬上改變
    asStr.sprintf("%sContact%d.bmp", BmpPath, scrbSLK->Position);               // golden :2069
    // GATE (C-3) -- golden :2070 `imgSLK->Picture->LoadFromFile(asStr);`.
    // TImage / TPicture have zero port in this tree (forms/fTrayAssignment.h
    // :168 T1).  The path construction above IS kept, so the intent stays
    // observable; only the load is gated.  Same treatment as
    // forms/fTrayAssignment.cpp:705.
    (void)asStr;
    //ShowArmAndDeviceForce();                                                  // golden :2071 (commented out in golden)  //JimmyChiu 20220121 mark 改變階層
}

// ---------------------------------------------------------------------------
//  edPinCountKeyPress -- golden :2083-2087
// ---------------------------------------------------------------------------
void TfContact::edPinCountKeyPress(TObject *Sender, char &Key)
{
    (void)Sender;
    if(OnlyNumberInPut(Key)==false)                                             // golden :2085
        Key=NULL;                                                               // golden :2086
}

// ---------------------------------------------------------------------------
//  btnTStartClick / btnTStepClick -- golden :2251-2254 / :2256-2259.
//  These two set the click-latches that golden's ckernel.cpp manual-key trio
//  polls (WaitManualStartKey :116 / WaitManualStepKey :74).
// ---------------------------------------------------------------------------
void TfContact::btnTStartClick(TObject *Sender)
{
    (void)Sender;
    bSetupStart=true;                                                           // golden :2253
}

void TfContact::btnTStepClick(TObject *Sender)
{
    (void)Sender;
    bSetupStep=true;                                                            // golden :2258
}

// ---------------------------------------------------------------------------
//  Inital_ZTask -- golden :2275-2278
// ---------------------------------------------------------------------------
void TfContact::Inital_ZTask()
{
    Z_Height_Task=1;                                                            // golden :2277
}

// ---------------------------------------------------------------------------
//  OneCycleProcess -- golden :11741-11746 -- GATE (X-32), NOT DEFINED HERE.
//  Caught by the -fsyntax-only gate on the first pass of this wave, not by
//  reading: its two-line body needs `CONTACT_TEST` (golden cContact.cpp:77,
//  value 3), one of the SEVEN mode constants that golden keeps at cContact.cpp
//  file scope and that the port's cContact.h has not yet ported (it has three
//  of ten -- see GATE (X-10) in forms/fContact.h for the full list).
//  Re-declaring it here would fork the one list that decides which mode the
//  contact SM is in, so it is gated instead.  Same one-line unlock as (X-10)/
//  (X-11): add the seven to cContact.h:81-83.
//
//  spbOneCycleClick (golden :16861-16865) is gated TRANSITIVELY for the same
//  reason -- its whole body is `OneCycleProcess(); ledOneCycle->Value=
//  bContinueContact;`.  It is otherwise entirely clean.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  InitCarlibrationTask -- golden :11748-11751
// ---------------------------------------------------------------------------
void TfContact::InitCarlibrationTask()
{
    CarlibrationTask=1;                                                         // golden :11750
}

// ---------------------------------------------------------------------------
//  cbContactModeChange -- golden :13977-14062.
//  Both ini reads here are the PURE `ReadIniData` overload (common.h:235-238),
//  not CheckAndReadIniData -- no write-back.  GetRecipeFileName is real
//  (common.cpp:2410).
// ---------------------------------------------------------------------------
void TfContact::cbContactModeChange(TObject *Sender)
{
    (void)Sender;
    AnsiString szDir="";                                                        // golden :13979

    if(CosFunction.bFixedDropSpeed)                                             // golden :13981  //Steven 20131101 : 使用固定的Drop速度
    {
        edDropOffset1->Enabled=false;                                           // golden :13983
        edDropOffset2->Enabled=false;                                           // golden :13984
    }
    else
    {
        edDropOffset1->Enabled=true;                                            // golden :13988  //2012-01-03    Dell
        edDropOffset2->Enabled=true;                                            // golden :13989
    }

    if(cbContactMode->ItemIndex==DropPlaceShiftContact)                         // golden :13992  //Steven 20220607 : DropPlaceShiftContact模式已不使用, 自動跳下一個
    {
        cbContactMode->ItemIndex=DropContactModeDiffentSpeed;                   // golden :13994
    }

    if(cbContactMode->ItemIndex==DirectContactMode ||                           // golden :13997  //Steven 20100406 丟測模式會自動把Offset改成2mm
       cbContactMode->ItemIndex==TMove)                                         // golden :13998
    {
        edDropOffset1->Text=0;                                                  // golden :14000
        edDropOffset2->Text=0;                                                  // golden :14001
        edDropOffset1->Enabled=false;                                           // golden :14002
        edDropOffset2->Enabled=false;                                           // golden :14003
    }
    else if(cbContactMode->ItemIndex==DropContact ||                            // golden :14005
            cbContactMode->ItemIndex==DropContactModeDiffentSpeed ||            // golden :14006
            cbContactMode->ItemIndex==TMoveDrop ||                              // golden :14007
            cbContactMode->ItemIndex==TMoveDropSlowContact ||                   // golden :14008
            cbContactMode->ItemIndex==DropPlaceShiftContact)                    // golden :14009  //ChungHung 20150528 add for 海思 _8Site1x4
    {
        if(CUSTOMER_CODE==CC_KYEC_CHEN && IniConfig.bA16ContactTestDropContact) // golden :14011  //wei 20150831
        {
            edDropOffset1->Text=0;                                              // golden :14013
            edDropOffset2->Text=0;                                              // golden :14014
        }
        else
        {
            if(cbContactMode->ItemIndex==DropContact &&                         // golden :14018
               CUSTOMER_CODE==CC_PTI)                                           // golden :14019  //Sam 20190225 : 力成 DirectContact & DropContact 切換後，DropContact 的 Drop 不要變成預設值
            {
                szDir=GetRecipeFileName("Contact.Data");                        // golden :14021
                edDropOffset1->Text=FormatFloat("0.00", ReadIniData(szDir, "Test Arm1", "Drop_DropContactMode",    2.0));   // golden :14022
                edDropOffset2->Text=FormatFloat("0.00", ReadIniData(szDir, "Test Arm2", "Drop_DropContactMode",    2.0));   // golden :14023
            }
            else
            {
                edDropOffset1->Text=2;                                          // golden :14027
                edDropOffset2->Text=2;                                          // golden :14028
            }
        }
    }
    else if(cbContactMode->ItemIndex==DirectContactModeDiffentSpeed ||          // golden :14032
            cbContactMode->ItemIndex==TMoveSlowContact ||                       // golden :14033  //Steven 20160130 : TMove Soft contact
            cbContactMode->ItemIndex==DropContactSoftEP)                        // golden :14034  //kevin 20220621 add drop soft mode
    {
        if(atoi(edDropOffset1->Text.c_str())<2)                                 // golden :14036
        {
            edDropOffset1->Text="2";                                            // golden :14038
        }

        if(atoi(edDropOffset2->Text.c_str())<2)                                 // golden :14041
        {
            edDropOffset2->Text="2";                                            // golden :14043
        }
    }
    else                                                                        // golden :14046  //kevin 20220621 add other mode 清為 0
    {
        edDropOffset1->Text=0;                                                  // golden :14048
        edDropOffset2->Text=0;                                                  // golden :14049
    }

    if((cbContactMode->ItemIndex==DropContact ||                                // golden :14052
        cbContactMode->ItemIndex==TMoveDrop) &&                                 // golden :14053  //JerryYang 20240111 : add
        CosFunction.bTesterSidePushFunction==true)                              // golden :14054  //Richard 20220321 : 渠梁Side Push
    {
        pnlSidePush->Visible=true;                                              // golden :14056
    }
    else
    {
        pnlSidePush->Visible=false;                                             // golden :14060
    }
}

// ---------------------------------------------------------------------------
//  sbtExitClick -- golden :14478-14487
// ---------------------------------------------------------------------------
void TfContact::sbtExitClick(TObject *Sender)
{
    (void)Sender;
    if(CarlibrationTask!=1)                                                     // golden :14480
    {
        ShowMyMessage("\"Auto Height\"/\"Contect Test\" Mode need Finish !!", "需要完成\"Auto Height\"/\"Contect Test\"!!");   // golden :14482
        // NOTE: golden spells these with BCB6's doubled-quote escape
        // (`"""Auto Height"""`); the port uses C's backslash escape.  Same
        // resulting characters, and golden's own misspelling "Contect" is kept.
        return;                                                                 // golden :14483
    }
    sbtExit->Down=false;                                                        // golden :14485
    Close();                                                                    // golden :14486 -- DEVIATION (D-5), offline no-op
}

// ---------------------------------------------------------------------------
//  InitROILearningTask -- golden :14490-14494.
//  golden :14489 also declares a file-scope `TQPF_Timer ROILearningDelay;`
//  immediately above this function; it is read ONLY by Do_ROILearning
//  (GATE S-46), so it is not defined here -- a file-scope object with no
//  reader would just be dead storage.
// ---------------------------------------------------------------------------
void TfContact::InitROILearningTask()
{
    bDoRTCLearning=false;                                                       // golden :14492  //Ifor 20260226 add: Contact Mode 執行RTC Learn 不開啟Hot Air
    ROILearningTask=rlcLightOn;                                                 // golden :14493  //jou 2012-03-29 RTC啟動時,自動將燈箱打開
}

// ---------------------------------------------------------------------------
//  coD41Change -- golden :15049-15061
// ---------------------------------------------------------------------------
void TfContact::coD41Change(TObject *Sender)
{
    (void)Sender;
    if(coD41->ItemIndex==0)                                                     // golden :15051
    {
        edD41->Visible=false;                                                   // golden :15053
        labD41->Visible=false;                                                  // golden :15054
    }
    else
    {
        edD41->Visible=true;                                                    // golden :15058
        labD41->Visible=true;                                                   // golden :15059
    }
}

// ---------------------------------------------------------------------------
//  InitDoFullViewCheck -- golden :15064-15067.
//  golden :15063 declares a file-scope `TQPF_Timer HFullViewCheckDelay;` above
//  it; its only reader is DoFullViewCheck (GATE S-14), so it is not defined
//  here.
// ---------------------------------------------------------------------------
void TfContact::InitDoFullViewCheck()                                           //ChungHung 20130122 add for SCK
{
    iDoFullViewCheck=1;                                                         // golden :15066
}

// ---------------------------------------------------------------------------
//  edContactOffsetArm1MouseDown -- golden :15164-15168
// ---------------------------------------------------------------------------
void TfContact::edContactOffsetArm1MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, InputLimit.dContactHigh, InputLimit.dContactLow);   // golden :15167  //Steven 20140123 : Contact Height的Offset限制
}

// ---------------------------------------------------------------------------
//  edD41MouseDown -- golden :15321-15328
// ---------------------------------------------------------------------------
void TfContact::edD41MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              // golden :15324  //wei 20160309 Check Position Offset 最大值5
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 5.00, 0.00);   // golden :15325
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 20.0, 0.00);   // golden :15327  //Ifor 20160418 100 => 20 避免設定過高造成撞機
}

// ---------------------------------------------------------------------------
//  ChangeContactMode -- golden :15436-15454
// ---------------------------------------------------------------------------
void TfContact::ChangeContactMode(bool ret)                                     //Steven 20150224 : Auto Contact Test
{
    if(ret==true)                                                               // golden :15438
    {
        cbContactMode->ItemIndex=DropContact;                                   // golden :15440
        cbVacuumMode->ItemIndex=VacuumOFFMode;                                  // golden :15441
        edDropWaitTime->Text="0.5";                                             // golden :15442
        edDropOffset1->Enabled=true;                                            // golden :15443  //2012-01-03    Dell
        edDropOffset2->Enabled=true;                                            // golden :15444
    }
    else
    {
        cbContactMode->ItemIndex=DirectContactMode;                             // golden :15448
        cbVacuumMode->ItemIndex=VacuumOFFMode;                                  // golden :15449
        edDropWaitTime->Text="0.5";                                             // golden :15450
        edDropOffset1->Enabled=false;                                           // golden :15451  //2012-01-03    Dell
        edDropOffset2->Enabled=false;                                           // golden :15452
    }
}

// ---------------------------------------------------------------------------
//  spbOneCycleClick -- golden :16861-16865 -- GATE (X-32), NOT DEFINED HERE.
//  Transitive: its body calls OneCycleProcess(), which is gated on the seven
//  unported mode constants.  See the OneCycleProcess note above.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  edShtPickOffset1MouseDown -- golden :16867-16874
// ---------------------------------------------------------------------------
void TfContact::edShtPickOffset1MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    if(IniConfig.bChangeKitNoHardStop)                                          // golden :16870  //jou 2015-12-08 Xilinx 驗證用
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, InputLimit.dShuttleHigh, InputLimit.dShuttleLow);   // golden :16871
    else
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, InputLimit.iOffsetZHigh, InputLimit.iOffsetZLow);   // golden :16873  //Steven 20150811
    // GOLDEN NOTE, faithful: the `else` arm passes two INTs
    // (InputLimit.iOffsetZHigh/iOffsetZLow, cprod.h:3006-3007) into a
    // double-typed min/max pair, while the `if` arm passes doubles.  Golden
    // does the same implicit widening; kept verbatim.
}

// ---------------------------------------------------------------------------
//  labMPaDblClick -- golden :16943-16948
// ---------------------------------------------------------------------------
void TfContact::labMPaDblClick(TObject *Sender)
{
    (void)Sender;
    labEPValue->Visible=true;                                                   // golden :16945
    if(EP_Install==5)                                                           // golden :16946
        labEPValue_1032->Visible=true;                                          // golden :16947
}

// ---------------------------------------------------------------------------
//  CheckContactSettingChange -- golden :17054-17094.
//  GOLDEN NOTE (G-DimCmp), see forms/fContact.h: the last two comparisons
//  (golden :17088 / :17091) compare an INT against an ANSISTRING and are
//  therefore almost always true.  Translated verbatim -- the port's AnsiString
//  has the same non-explicit int ctor (vclcompat/AnsiString.h:67) and the same
//  free operator!= (:224), so the behaviour, bug included, is reproduced.
// ---------------------------------------------------------------------------
bool TfContact::CheckContactSettingChange()                                     //JerryYang 20170213 (Steven) Run check觸發條件判斷
{
    //============Contact parameter==========                                   // golden :17056
    if(cbContactMode->ItemIndex!=DeviceForm.ContactMode)                        // golden :17057
        return true;
    else if(cbVacuumMode->ItemIndex!=DeviceForm.VacuumMode)                     // golden :17059
        return true;
    else if(edDropWaitTime->Text!=FormatFloat("0.00", DeviceForm.DropWait))     // golden :17061
        return true;
    else if(edDropSpeed->Text!=FormatFloat("0.00", DeviceForm.DropSpeed))       // golden :17063
        return true;
    else if(rgShuttleWaitingOutSiteChamber->ItemIndex!=DeviceForm.bShuttleWaitingOutSiteChamber)   // golden :17065
        return true;
    else if(rgSuckShuttleDeviceAfterTested->ItemIndex!=DeviceForm.bSuckShuttleDeviceAfterTested)   // golden :17067
        return true;
    else if(rgSuckShuttleDeviceWaitOnShuttle->ItemIndex!=DeviceForm.bSuckShuttleDeviceWaitOnShuttle) // golden :17069  //Ifor 20180606 (wei)
        return true;
    //============Contact Force===============                                  // golden :17071
    else if(atof(edAirForce->Text.c_str())!=DeviceForm.dPress)                  // golden :17072
        return true;
    else if(scrbSLK->Position!=DeviceForm.iHeadDeviceCT)                        // golden :17074
        return true;
    else if(DeviceForm.dKitDiameter==6.0 && rgKitDiameter->ItemIndex!=2)        // golden :17076
        return true;
    else if(DeviceForm.dKitDiameter==4.0 && rgKitDiameter->ItemIndex!=1)        // golden :17078
        return true;
    else if(DeviceForm.dKitDiameter==5.6 && rgKitDiameter->ItemIndex!=3)        // golden :17080
        return true;
    else if(DeviceForm.dKitDiameter==3.0 && rgKitDiameter->ItemIndex!=0)        // golden :17082
        return true;
    else if(atoi(edPinCount->Text.c_str())!=DeviceForm.iPinCT)                  // golden :17084
        return true;
    else if(edForcePerPinN->Text!=FormatFloat("0.0000", DeviceForm.ForcePerPinN))   // golden :17086
        return true;
    else if(iUnitMultiply100(atoi(edXDimension->Text.c_str()))!=FormatFloat("0.00", DeviceForm.XDimension))   // golden :17088 -- see (G-DimCmp)
        return true;
    else if(iUnitMultiply100(atoi(edYDimension->Text.c_str()))!=FormatFloat("0.00", DeviceForm.YDimension))   // golden :17090 -- see (G-DimCmp)
        return true;
    else
        return false;                                                           // golden :17093
}

// ---------------------------------------------------------------------------
//  cbEnableUKClick -- golden :17139-17182.
//  Writes IniConfig.bRemeberAutoHeight / bChangeKitNoHardStop -- IN-MEMORY
//  config only; nothing here persists them.
// ---------------------------------------------------------------------------
void TfContact::cbEnableUKClick(TObject *Sender)
{
    (void)Sender;
    if(CosFunction.bUniversalKit)                                               // golden :17141  //JerryYang 20171102 (wei) UniversalKit改成客戶功能
    {
        if(cbEnableUK->Checked==true)                                           // golden :17143
        {
            palTorqueArm1->Visible          =true;                              // golden :17145
            palTorqueArm2->Visible          =true;                              // golden :17146
            labContactOffset->Visible       =true;                              // golden :17147
            edContactOffsetArm1->Visible    =true;                              // golden :17148
            edContactOffsetArm2->Visible    =true;                              // golden :17149
            labShtOffset->Visible           =true;                              // golden :17150
            edShtPickOffset1->Visible       =true;                              // golden :17151
            edShtPickOffset2->Visible       =true;                              // golden :17152
            lblShtPickOfsMM->Visible        =true;                              // golden :17153
            lblContactOfsMM->Visible        =true;                              // golden :17154
            edContactHeight1->Enabled       =false;                             // golden :17155
            edContactHeight2->Enabled       =false;                             // golden :17156
            edPickUp1->Enabled              =false;                             // golden :17157
            edPickUp2->Enabled              =false;                             // golden :17158
            IniConfig.bRemeberAutoHeight    =true;                              // golden :17159  //ChungHung 20120725 Amkor_K 要可以記住AutoHeight的值
            IniConfig.bChangeKitNoHardStop  =true;                              // golden :17160  //jou 2015-12-08 Xilinx 驗證用
        }
        else
        {
            palTorqueArm1->Visible          =false;                             // golden :17164
            palTorqueArm2->Visible          =false;                             // golden :17165
            labContactOffset->Visible       =false;                             // golden :17166
            edContactOffsetArm1->Visible    =false;                             // golden :17167
            edContactOffsetArm2->Visible    =false;                             // golden :17168
            labShtOffset->Visible           =false;                             // golden :17169
            edShtPickOffset1->Visible       =false;                             // golden :17170
            edShtPickOffset2->Visible       =false;                             // golden :17171
            lblShtPickOfsMM->Visible        =false;                             // golden :17172
            lblContactOfsMM->Visible        =false;                             // golden :17173
            edContactHeight1->Enabled       =true;                              // golden :17174
            edContactHeight2->Enabled       =true;                              // golden :17175
            edPickUp1->Enabled              =true;                              // golden :17176
            edPickUp2->Enabled              =true;                              // golden :17177
            IniConfig.bRemeberAutoHeight    =false;                             // golden :17178
            IniConfig.bChangeKitNoHardStop  =false;                             // golden :17179
        }
    }
}

// ---------------------------------------------------------------------------
//  edDropByPassDetectMouseDown -- golden :17189-17193
// ---------------------------------------------------------------------------
void TfContact::edDropByPassDetectMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 20.0, 0.00);  // golden :17192
}

// ---------------------------------------------------------------------------
//  ResetDMCTrayData -- golden :18339-18348
// ---------------------------------------------------------------------------
void TfContact::ResetDMCTrayData()
{
    iDMCArm=-1;                                                                 // golden :18341
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)                                       // golden :18342
    {
        iDMCTrayX[i]=-1;                                                        // golden :18344
        iDMCTrayY[i]=-1;                                                        // golden :18345
        iDMCChannel[i]=-1;                                                      // golden :18346
    }
}

// ---------------------------------------------------------------------------
//  edtPinOfDieMouseDown -- golden :18350-18355
// ---------------------------------------------------------------------------
void TfContact::edtPinOfDieMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 10000);   // golden :18353
    // GOLDEN NOTE, faithful: golden passes min=10, max=10000 into a
    // (…, double min, double max) pair whose declared order is
    // ShowQwertyKey(Ptr, iFunction, iDP, bCheckRange, min, max)
    // (forms/fQwertyKey.h:370) and which forwards to
    // CheckRange(Value, Maximum, Minimum) (MachineType.h:1525) -- i.e. golden's
    // two bounds arrive as Maximum=10, Minimum=10000, INVERTED.  CheckRange
    // handles the inversion explicitly (its `if(Maximum<Minimum)` arm,
    // MachineType.h:1527-1535), so the effective clamp is still [10, 10000].
    // Verbatim; do not "fix" the argument order.
    CountDieForceKg(true);                                                      // golden :18354
}

// ---------------------------------------------------------------------------
//  edDoubleForceMouseDown -- golden :18357-18369
// ---------------------------------------------------------------------------
void TfContact::edDoubleForceMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    double dKitDiameter =DeviceForm_File.dDieForceKitDiameter*10.0;             // golden :18360  //Steven 20241014 : Dual Force改用最大氣壓500kpa / 最小氣壓40Kpa換算
    double dDualForceMax=(((dKitDiameter*dKitDiameter*3.14)/4.0)*5.0*0.0101972); // golden :18361
    double dDualForceMin=(((dKitDiameter*dKitDiameter*3.14)/4.0)*0.4*0.0101972); // golden :18362

    dDualForceMax=MyFormatFloat(dDualForceMax, 2);                              // golden :18364  //Steven 20241014 : 指定小數點位數
    dDualForceMin=MyFormatFloat(dDualForceMin, 2);                              // golden :18365

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, dDualForceMin, dDualForceMax);   // golden :18367
    CountDieForceKg(false);                                                     // golden :18368
}

// ---------------------------------------------------------------------------
//  CountDieForceKg -- golden :18371-18394
// ---------------------------------------------------------------------------
void TfContact::CountDieForceKg(bool bByPinCount)                               //Steven 20191106: Pin of Die
{
    double dForce, dPinCount, dPinForce;                                        // golden :18373
    int iPinCount;                                                              // golden :18374
    AnsiString Str;                                                             // golden :18375
    if(bByPinCount)                                                             // golden :18376
    {
        dPinCount=atof(edtPinOfDie->Text.c_str());                              // golden :18378
        dPinForce=atof(edDieForcePerPinG->Text.c_str())/1000.0;                 // golden :18379
        dForce=dPinCount*dPinForce;                                             // golden :18380
        Str.sprintf("%0.2f", dForce);                                           // golden :18381
        edDoubleForce->Text=Str;                                                // golden :18382
    }
    else
    {
        dForce=atof(edDoubleForce->Text.c_str())*1000;                          // golden :18386
        dPinForce=atof(edForcePerPinG->Text.c_str());                           // golden :18387
        if(dPinForce!=0)                                                        // golden :18388
            iPinCount=dForce/dPinForce;                                         // golden :18389
        else
            iPinCount=dForce/1.0;                                               // golden :18391
        edtPinOfDie->Text=iPinCount;                                            // golden :18392
    }
}

// ---------------------------------------------------------------------------
//  edAutoKSHTReleaseOfsMouseDown -- golden :18396-18400
// ---------------------------------------------------------------------------
void TfContact::edAutoKSHTReleaseOfsMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, 10, 2);        // golden :18399  //Sam 20200217 : K高後 Shuttle Release Height offset By SetupFile
}

// ---------------------------------------------------------------------------
//  edForcePerDeviceKGClick -- golden :18408-18423
// ---------------------------------------------------------------------------
void TfContact::edForcePerDeviceKGClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 1.0, 120.0);  // golden :18410
    double dForce, dPinCount, dPinForce;                                        // golden :18411

    AnsiString Str;                                                             // golden :18413
    dPinCount=atoi(edPinCount->Text.c_str());                                   // golden :18414

    dForce=atof(edForcePerDeviceKG->Text.c_str())*1000.0;                       // golden :18416
    if(dPinCount!=0)                                                            // golden :18417
        dPinForce=dForce/dPinCount;                                             // golden :18418
    else
        dPinForce=dForce/1;                                                     // golden :18420

    edForcePerPinG->Text=dPinForce;                                             // golden :18422
    // GOLDEN NOTE, faithful: `Str` (golden :18413) is declared and never used.
    (void)Str;
}

// ---------------------------------------------------------------------------
//  edtPinOfDieKeyPress -- golden :18425-18428.  golden's whole body is `return;`
//  -- i.e. the key filter is deliberately disabled for this field.
// ---------------------------------------------------------------------------
void TfContact::edtPinOfDieKeyPress(TObject *Sender, char &Key)
{
    (void)Sender; (void)Key;
    return;                                                                     // golden :18427
}

// ---------------------------------------------------------------------------
//  edForcePerPinNMouseDown -- golden :18542-18563.
//  DEVIATION (D-7): `Buffer->Tag==1` is real here only because the port ctor
//  seeds Tag from cContact.dfm:16202.  With an unseeded Tag this would read 0
//  and SKIP the barcode gate -- a wrong branch, not a no-op.
// ---------------------------------------------------------------------------
void TfContact::edForcePerPinNMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    TEdit *Buffer;                                                              // golden :18545
    Buffer=(TEdit *)Sender;                                                     // golden :18546
    if(Buffer->Tag==1)                                                          // golden :18547
    {
        if(Barcode_Reader(bcContact)==0)                                        // golden :18549  // 20140103 wei KYEC Barcode Reader
        {
            return;                                                             // golden :18551
        }
    }

    if(CUSTOMER_CODE==CC_JCET || CUSTOMER_CODE==CC_AMKOR_China)                 // golden :18555  //Steven 20220216 : Pinforce加上上下限
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 4, true, InputLimit.dForcePerpinNHigh, InputLimit.dForcePerpinNLow);      // golden :18557  //Isaac 20210517
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 4, true, InputLimit.dForcePerpinNHigh+10, InputLimit.dForcePerpinNLow);   // golden :18561
    }
}

// ---------------------------------------------------------------------------
//  GotIndexYSpeed / GotIndexZSpeed -- golden :18663-18667 / :18669-18673.
//  The calculation itself is already ported as the free functions
//  ComputeIndexYSpeed / ComputeIndexZSpeed (cContact.h:340-341).  Delegating
//  rather than duplicating keeps ONE definition of the rule; golden's
//  side effect (writing the shared scratch member `itemp`) is reproduced
//  because golden's own bodies do it and `itemp` is observable.
// ---------------------------------------------------------------------------
int TfContact::GotIndexYSpeed(int _fixValue)                                    //JimmyChiu 20211028 : All speed can set by speed setting.
{
    itemp=ComputeIndexYSpeed(_fixValue, iContactMode, MOT[MTestY1].GailSpeed);  // golden :18665
    return itemp;                                                               // golden :18666
}

int TfContact::GotIndexZSpeed(int _fixValue)                                    //JimmyChiu 20211028 : All speed can set by speed setting.
{
    itemp=ComputeIndexZSpeed(_fixValue, iContactMode, MOT[MTestZ1].GailSpeed);  // golden :18671
    return itemp;                                                               // golden :18672
}

// ---------------------------------------------------------------------------
//  edForcePerPinGMouseDown -- golden :19029-19050.  See (D-7) as above.
// ---------------------------------------------------------------------------
void TfContact::edForcePerPinGMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    TEdit *Buffer;                                                              // golden :19032
    Buffer=(TEdit *)Sender;                                                     // golden :19033
    if(Buffer->Tag==1)                                                          // golden :19034
    {
        if(Barcode_Reader(bcContact)==0)                                        // golden :19036  // 20140103 wei KYEC Barcode Reader
        {
            return;                                                             // golden :19038
        }
    }

    if(CUSTOMER_CODE==CC_JCET || CUSTOMER_CODE==CC_AMKOR_China)                 // golden :19042
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 4, true, InputLimit.dForcePerpinHigh, InputLimit.dForcePerpinLow);        // golden :19044  //Steven 20220216
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 4, true, InputLimit.dForcePerpinHigh+10, InputLimit.dForcePerpinLow);     // golden :19048
    }
}

// ---------------------------------------------------------------------------
//  rgDieForceKitDiameterClick -- golden :19503-19506.
//  DEVIATION (D-9): this is the reader that makes the .dfm Items seeding
//  load-bearing -- on an empty Items list Strings[ItemIndex] would be "" and
//  dDieForceKitDiameter would silently become 0.0.
// ---------------------------------------------------------------------------
void TfContact::rgDieForceKitDiameterClick(TObject *Sender)
{
    (void)Sender;
    DeviceForm_File.dDieForceKitDiameter=atof(rgDieForceKitDiameter->Items->Strings[rgDieForceKitDiameter->ItemIndex].c_str())/10.0;   // golden :19505
}

// ---------------------------------------------------------------------------
//  InitRTCAutoTuning -- golden :19853-19856
// ---------------------------------------------------------------------------
void TfContact::InitRTCAutoTuning()                                             //Sam 20230419 : 新增 RTC Auto Tuning 功能
{
    iRTCAutoTuningTask=1;                                                       // golden :19855
}

// ---------------------------------------------------------------------------
//  cbRTCAutoTuningClick -- golden :20365-20378.
//  Writes the runtime global bRTCAutoTuning (cmydef.h:5704).  No persistence,
//  no machine command -- it selects whether the RTC auto-tune SM (GATE S-36)
//  is allowed to run, and that SM is itself gated.
// ---------------------------------------------------------------------------
void TfContact::cbRTCAutoTuningClick(TObject *Sender)
{
    (void)Sender;
    if(cbRTCAutoTuning->Checked)                                                // golden :20367  //Sam 20230419
    {
        cbRTCModel->Checked=true;                                               // golden :20369
        cbRTCModel->Enabled=false;                                              // golden :20370
        bRTCAutoTuning=true;                                                    // golden :20371  //Sam 20240711
    }
    else
    {
        cbRTCModel->Enabled=true;                                               // golden :20375
        bRTCAutoTuning=false;                                                   // golden :20376
    }
    // GOLDEN NOTE, faithful: the `else` arm re-enables cbRTCModel but does NOT
    // clear cbRTCModel->Checked, so unticking auto-tune leaves RTC-model ticked.
}

// ---------------------------------------------------------------------------
//  IsRun2DCheck -- golden :21203-21219.
//  The predicate is already ported as the free function ComputeIsRun2DCheck
//  (cContact.h:197-202) -- atester_shims.cpp:26 records that it is "the real
//  TfContact::IsRun2DCheck body".  Delegating keeps ONE definition.
// ---------------------------------------------------------------------------
bool TfContact::IsRun2DCheck()                                                  //JerryYang 20250220 : 2DID硬體順序檢查功能
{
    bool ret=false;                                                             // golden :21205
    ret=ComputeIsRun2DCheck(IniConfig.bF33_Check2DHardware,                      // golden :21206
                            BAR_CODE_INSTALL,                                    // golden :21207-21209
                            TestIF_File.bEnableBarCode,                          // golden :21210
                            InArmSuck.iShtRow,                                   // golden :21211
                            iContactMode,                                        // golden :21212  //JerryYang 20250428 : fix contact mode跳錯alarm
//                          TestIF_File.bEnableBottom2D==false &&                // golden :21213 (commented out in golden)
                            bRun2DCheck);                                        // golden :21214
    return ret;                                                                 // golden :21218
}

// ---------------------------------------------------------------------------
//  Inital_Z1PickFromSocketTask -- golden :21221-21224
// ---------------------------------------------------------------------------
void TfContact::Inital_Z1PickFromSocketTask()
{
    Z_PickFromSocketTask=1;                                                     // golden :21223
}

// ---------------------------------------------------------------------------
//  timerContactTimer -- golden :21355-21366.
//  GOLDEN NOTE (G-NoElse): there is NO else -- the two controls are disabled
//  when the machine holds an IC and this timer never re-enables them.
//  ⚠ This is the one delivered body that reaches the ht9045_sm layer
//  (IndexHasIC / ShuttleHasIC, csystem_predicates.cpp).  See the DIET RISK note
//  in forms/fContact.h: if the target's link diet forbids that edge, GATE this
//  method rather than widening the diet.
// ---------------------------------------------------------------------------
void TfContact::timerContactTimer(TObject *Sender)
{
    (void)Sender;
    if(IndexHasIC() ||                                                          // golden :21357  //JerryYang 20251118 : 機台有IC不能切換模式
       InArmSuck.HasIC() ||                                                     // golden :21358
       OutArmSuck.HasIC() ||                                                    // golden :21359
       ShuttleHasIC())                                                          // golden :21360
    {
        rgHandlerMode->Enabled=false;                                           // golden :21362
        rbModeNormal->Enabled=false;                                            // golden :21363
        return;                                                                 // golden :21364
    }
}

// ---------------------------------------------------------------------------
//  ShowMSG -- golden :21368-21378
// ---------------------------------------------------------------------------
void TfContact::ShowMSG(AnsiString asStr)
{
    if(brecordmsgLock==false)                                                   // golden :21370
    {
        brecordmsgLock=true;                                                    // golden :21372
        if(Memo1->Lines->Count>500)                                             // golden :21373
            Memo1->Lines->Clear();                                              // golden :21374
        Memo1->Lines->Add(asStr);                                               // golden :21375
        brecordmsgLock=false;                                                   // golden :21376
    }
}

// ---------------------------------------------------------------------------
//  UpdateContactRelative -- golden :22315-22323.
//  DEVIATION (D-8): `Get0_01MMType(char*)` (cpublic.h:19) takes a NON-const
//  char* because BCB6's AnsiString::c_str() returned char*.  The const_cast is
//  safe and machine-checked, not assumed: Get0_01MMType's entire body is
//  `double i=atof(str); ...` (cpublic.cpp:411-418) -- it never writes through
//  the pointer.  Precedent: tests/test_cpublic_foundation.cpp:359.
// ---------------------------------------------------------------------------
void TfContact::UpdateContactRelative()                                         //JimmyChiu 20220121 add
{
    int Pos=0;                                                                  // golden :22317
    Pos=Get0_01MMType(const_cast<char*>(edContactHeight1->Text.c_str()));       // golden :22318
    edContactRelativeZ1->Text=ConvertTouMType(Teach.iContactZ1Relative-Pos);    // golden :22319

    Pos=Get0_01MMType(const_cast<char*>(edContactHeight2->Text.c_str()));       // golden :22321
    edContactRelativeZ2->Text=ConvertTouMType(Teach.iContactZ2Relative-Pos);    // golden :22322
}

// ---------------------------------------------------------------------------
//  InitChkInSHLatchTask -- golden :22608-22615
// ---------------------------------------------------------------------------
void TfContact::InitChkInSHLatchTask()                                          //KenHsieh 20250811 : InSht sensor latch for contact
{
    iChkInSHLatchTask=1;                                                        // golden :22610
    iSH1AutoLtcErr=0;                                                           // golden :22611
    iSH2AutoLtcErr=0;                                                           // golden :22612
    ErrPart_InSH1Ltc="";                                                        // golden :22613
    ErrPart_InSH2Ltc="";                                                        // golden :22614
}

// =============================================================================
//  END OF TRANSLATED BODIES.
//
//  The 90 gated methods declared in forms/fContact.h are DELIBERATELY NOT
//  DEFINED above.  That is the interlock, not an omission -- see this file's
//  head banner and the GATE tables in forms/fContact.h.  If you came here
//  looking for one of them and found nothing, that is the design working.
// =============================================================================
