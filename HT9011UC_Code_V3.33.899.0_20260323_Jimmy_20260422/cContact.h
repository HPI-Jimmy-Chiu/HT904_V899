//---------------------------------------------------------------------------

#ifndef cContactH
#define cContactH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
//#include "htimer.h"
#include "myTimer.h"
#include "ALed.hpp"
#include "MyLed.h"
#include <Graphics.hpp>
#include "MachineType.h"
//---------------------------------------------------------------------------
extern const int CONTACT_NORMAL            ;
extern const int CONTACT_AUTO_GET_HEIGHT   ;
extern const int CONTACT_MANUAL_GET_HEIGHT ;
extern const int CONTACT_TEST              ;
extern const int AUTO_CONTACT_TEST         ;                                    //Steven 20150224 : Auto Contact Test
extern const int CONTACT_IN_SHUTTLE_CHECK  ;
extern const int CONTACT_OUT_SHUTTLE_CHECK ;
extern const int CONTACT_DEVICE_MAP_CHECK  ;                                    //Steven 20190910 : Qualcomm功能
extern const int CONTACT_DEVICE_LOOP_TEST  ;                                    //Ztex 2023.11.19 Add CONTACT_DEVICE_LOOP_TEST
//---------------------------------------------------------------------------
class cDMP_RICHTEK                                                              //Jimmychiu 20251222 : Device Map Test By P&P
{
public:     // User declarations
    cDMP_RICHTEK()
    {
        Clear();
    }
    ~cDMP_RICHTEK(){}
    //parameter
    int  iNowDevicePos;
    //function
    void Clear()
    {
        iNowDevicePos=0;
    }
    bool TransformRICHTEKMap();
    bool DMP_RICHTEK_Finish();
    void GetMapTrayPosFromEncorder(int idata, int &iRow, int &iCol);
    void DoNextTurn(){iNowDevicePos=iNowDevicePos+1;}
};
//---------------------------------------------------------------------------
class TfContact : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlBottom;
    TSpeedButton *sbtExit;
    TSpeedButton *spbSave;
    TButton *btnStart;
    TButton *btnPause;
    TPanel *pnlSLKSetting;
    TGroupBox *gbContactForce;
    TImage *imgSLK;
    TLabel *lblContactForce;
    TLabel *lblForceN;
    TLabel *lblMaxForce;
    TLabel *lblForcePerDevice;
    TLabel *lblForcePerDeviceKg;
    TLabel *lblForcePerDeviceN;
    TLabel *lblYDimension;
    TLabel *lblXDimension;
    TLabel *lblReadEP;
    TEdit *edAirForce;
    TEdit *edAirForceN;
    TEdit *edForcePerDeviceKG;
    TEdit *edForcePerDeviceN;
    TEdit *edYDimension;
    TEdit *edXDimension;
    TScrollBar *scrbSLK;
    TButton *btnTStart;
    TButton *btnTStep;
    TButton *SetMPaBtn;
    TRadioGroup *rgKitDiameter;
    TPanel *btContactForce;
    TPanel *palOTD_4;
    TPanel *pnlTop;
    TGroupBox *gbContactHeight;
    TImage *imgIndex;
    TLabel *lblReleaseHeight;
    TLabel *lblPickHeight;
    TLabel *lblDropOffset;
    TLabel *lblContactHeight;
    TLabel *lblReleaseHeightMM;
    TLabel *lblPickHeightMM;
    TLabel *lblDropOfsMM;
    TLabel *lblContactHighMM;
    TEdit *edReleaseHeight1;
    TEdit *edPickUp1;
    TEdit *edDropOffset1;
    TEdit *edtSht1;
    TEdit *edtSht2;
    TEdit *edtArm1;
    TEdit *edContactHeight1;
    TEdit *edReleaseHeight2;
    TEdit *edPickUp2;
    TEdit *edDropOffset2;
    TEdit *edContactHeight2;
    TEdit *edtArm2;
    TGroupBox *gbContactParameter;
    TLabel *lblContactMode;
    TLabel *lblVacMode;
    TLabel *lblDummyContact;
    TLabel *lblDropWaitTime;
    TLabel *lblSuckShuttleDeviceAfterTested;
    TLabel *lblShuttleWaitingOutSiteChamber;
    TLabel *lblDropSpeed;
    TComboBox *cbContactMode;
    TComboBox *cbVacuumMode;
    TPanel *pnlDummyContact;
    TRadioButton *rbNormal;
    TRadioButton *rbDummyMode;
    TEdit *edDropWaitTime;
    TRadioGroup *rgSuckShuttleDeviceAfterTested;
    TRadioGroup *rgShuttleWaitingOutSiteChamber;
    TEdit *edDropSpeed;
    TGroupBox *gbDynaTemp;
    TButton *btAt;
    TButton *btShowDynaTemp;
    TButton *btTempOffset;
    TEdit *edSpeed;
    TEdit *edSpeedZ;
    TLabel *lblIndexSpeedY;
    TLabel *lblIndexSpeedZ;
    TRadioGroup *rgPSDT;
    TLabel *lblPSDT;
    TLabel *lblOrgHeight;
    TEdit *edContactBackUp1;
    TEdit *edContactBackUp2;
    TPanel *pnlTorqueControl;
    TLabel *lblTorqueMax;
    TLabel *lblTorqueCompare;
    TEdit *edtTorqueMax;
    TEdit *edtTorqueCmp;
    TLabel *labContactOffset;
    TEdit *edContactOffsetArm1;
    TEdit *edContactOffsetArm2;
    TPanel *palOTD_6;
    TPanel *pnlSensorAdj;
    TMyLed *ledOTD;
    TTimer *OTDTimer;
    TLabel *lblLayoutkit;
    TLabel *lblSLKWarning;
    TLabel *lblOrgPick;
    TEdit *edOrgPick1;
    TEdit *edOrgPick2;
    TPanel *palHeightCalibration;
    TPanel *pnlFuncSelect;
    TCheckBox *chkShuttle;
    TCheckBox *cbRTCModel;
    TLabel *lblContactOfsMM;
    TLabel *lblOrgHeightMM;
    TPanel *panHeightMode;
    TGroupBox *rgHandlerMode;
    TLabel *lbAutoContactTest;
    TRadioButton *rbModeNormal;
    TRadioButton *rbAutoHeight;
    TRadioButton *rbManualHeight;
    TRadioButton *rbContactTest;
    TEdit *edAutoContactTest;
    TGroupBox *gbContactStatus;
    TMemo *Memo1;
    TGroupBox *gbHeightStatus;
    TLabel *lblContact;
    TLabel *lblTorque;
    TLabel *lblArm1Sts;
    TLabel *lblArm2Sts;
    TLabel *lblContactMM;
    TLabel *lblTorque1;
    TPanel *palArm1Height;
    TPanel *palArm2Height;
    TPanel *PnlTorue0;
    TPanel *PnlTorue1;
    TPanel *palTestSocket;
    TLabel *labD41;
    TComboBox *coD41;
    TEdit *edD41;
    TLabel *lblICCheck;
    TPanel *pnlPurge;
    TLabel *lbAirPurgeBeforePickFromShuttle;
    TLabel *lblPurgeTime;
    TLabel *lblPurgeInterval;
    TLabel *lblPurgeOffset;
    TLabel *lblPurgeTimeS;
    TLabel *lblPurgeIntervalT;
    TLabel *lblPurgeOffsetMM;
    TRadioGroup *rgPurgeBeforePickShuttle;
    TEdit *edtPurgeBeforePickShuttleTime;
    TEdit *edtPurgeBeforePickShuttleInterval;
    TEdit *edtPurgeBdforePickShuttleOffSet;
    TLabel *lblPinCount;
    TEdit *edPinCount;
    TEdit *edForcePerPinN;
    TEdit *edForcePerPinG;
    TLabel *lblPerGf;
    TLabel *lblPerPinN;
    TRadioButton *rbAutoContactTest;
    TRadioButton *rbStepContactTest;
    TButton *btEditTray;
    TLabel *labSetKg;
    TEdit *edSetKg;
    TLabel *labTransfer;
    TEdit *edTransfer;
    TSpeedButton *spbOneCycle;
    TMyLed *ledOneCycle;
    TLabel *labShtOffset;
    TEdit *edShtPickOffset1;
    TEdit *edShtPickOffset2;
    TLabel *lblOrgPickMM;
    TLabel *lblShtPickOfsMM;
    TButton *btOffset;
    TCheckBox *cb2DMatrix;
    TLabel *lblEPValueKpa;
    TLabel *lblEPValueKg;
    TPanel *palSoakTimeWating;
    TLabel *lblWarning1;
    TPanel *palSoakTime;
    TLabel *lbHisiVer;
    TButton *btBarcode;
    TLabel *lblAirForce;
    TEdit *edAirKPA;
    TLabel *labMPa;
    TTimer *TimerEP;
    TCheckBox *cbSFCAutoTune;
    TPanel *palSFCInformation;
    TPanel *plIndexArmJogMove;
    TLabel *lblJogMove;
    TButton *btnIndexArmJogMove_Up;
    TButton *btnIndexArmJogMove_Down;
    TComboBox *cbIndexArmJogMove_mm;
    TLabel *lblJogMoveMM;
    TLabel *lblSuckDevicesDuringTest;
    TRadioGroup *rgSuckDevicesDuringTest;
    TLabel *lblIndexUpSpeed;
    TRadioGroup *rgIndexUpSpeed;
    TRadioGroup *rgOutKitDiameter;
    TCheckBox *chkUseAddWeight;
    TPanel *palTorqueArm1;
    TPanel *palTorqueArm2;
    TCheckBox *cbEnableUK;
    TCheckBox *chkTeachInOutArmZ;
    TRadioGroup *rgSuckShuttleDeviceWaitOnShuttle;
    TLabel *lblSuckShuttleDeviceWaitOnShuttle;
    TPanel *pnlHandlerSatus;
    TLabel *labDelayStatus;
    TLabel *lblCountDown;
    TButton *btnTempSet;
    TLabel *lblMinForce;
    TRadioButton *rbDeviceMapping;
    TGroupBox *gbDoubleEPSet;
    TLabel *lblDieForceEP;
    TLabel *lblLoadCellHeight;
    TEdit *edLoadCellHeight1;
    TEdit *edLoadCellHeight2;
    TLabel *lblLoadCellHighMM;
    TRadioButton *rbLoadCellAutoHigh;
    TRadioGroup *rgDieForceKitDiameter;
    TLabel *lblDieForce;
    TEdit *edDoubleForce;
    TLabel *lblDieForceKg;
    TLabel *lblNoOfBall;
    TEdit *edtPinOfDie;
    TEdit *edAutoKSHTReleaseOfs;
    TLabel *lblReleaseOfs;
    TButton *btnTrayMap;
    TLabel *lblMaxForcePerIC;
    TCheckBox *chk2DID;
    TSpeedButton *AutoZTeachButton;
    TEdit *edOutDiameter;
    TLabel *lblUpWaitTime;
    TEdit *edUpWaitTime;
    TLabel *lblUpSpeed;
    TEdit *edUpSpeed;
    TLabel *lblUpOffset;
    TLabel *lblUpOfsMM;
    TEdit *edUpOffset2;
    TEdit *edUpOffset1;
    TButton *btTCPIP;
    TEdit *edDropByPassDetect;
    TLabel *labbUserDefMaxContactHeight;
    TPanel *pnlSidePush;
    TLabel *lbTesterSidePush;
    TRadioGroup *rgTesterSidePush;
    TEdit *edSidePushWaitTime;
    TLabel *lblSidePushWaitTime;
    TRadioGroup *rgSidePushMode;
    TLabel *lblSitePushMode;
    TRadioButton *rbTemp;
    TLabel *labEPValue;
    TLabel *labEPValue_1032;
    TLabel *lblReadEP2;
    TCheckBox *cbRTCAutoTuning;
    TLabel *lblDropByPassDetect;
    TCheckBox *chk_K_Temperature;
    TRadioButton *rbKTempIndexMove;
    TRadioButton *rbDeviceLoopTest;
    TEdit *edDeviceLoopTest;
    TCheckBox *cbOneTouchAutoContactHight;
    TTimer *timerContact;
    TEdit *edDieForcePerPinN;
    TEdit *edDieForcePerPinG;
    TLabel *Label1;
    TLabel *Label2;
    TCheckBox *cbTestContactMode;
    TEdit *edTestContactCount;
    TLabel *lblNowCount;
    TLabel *lblCount;
    TLabel *lblTsetSec;
    TEdit *edTestSec;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edDropWaitTimeKeyPressInt(TObject *Sender, char &Key);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall edPinCountChange(TObject *Sender);
    void __fastcall edForcePerPinNChange(TObject *Sender);
    void __fastcall edDieForcePerPinGChange(TObject *Sender);
    void __fastcall scrbSLKChange(TObject *Sender);
    void __fastcall edPinCountKeyPress(TObject *Sender, char &Key);
    void __fastcall edDropWaitTimeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Edit16MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnTStartClick(TObject *Sender);
    void __fastcall btnTStepClick(TObject *Sender);
    void __fastcall edContactHeight1Change(TObject *Sender);
    void __fastcall edAirForceChange(TObject *Sender);
    void __fastcall btnStartClick(TObject *Sender);
    void __fastcall btnPauseClick(TObject *Sender);
    void __fastcall cbContactModeChange(TObject *Sender);
    void __fastcall pnlSensorAdjClick(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall btContactForceClick(TObject *Sender);
    void __fastcall btShowDynaTempClick(TObject *Sender);
    void __fastcall btAtClick(TObject *Sender);
    void __fastcall btTempOffsetClick(TObject *Sender);
    void __fastcall edSpeedZMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edD41Click(TObject *Sender);
    void __fastcall coD41Change(TObject *Sender);
    void __fastcall edContactOffsetArm1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall palOTD_4Click(TObject *Sender);
    void __fastcall palOTD_6Click(TObject *Sender);
    void __fastcall OTDTimerTimer(TObject *Sender);
    void __fastcall rgKitDiameterClick(TObject *Sender);
    void __fastcall edD41MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall rbModeNormalClick(TObject *Sender);
    void __fastcall btEditTrayClick(TObject *Sender);
    void __fastcall spbOneCycleClick(TObject *Sender);
    void __fastcall edShtPickOffset1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btOffsetClick(TObject *Sender);
    void __fastcall btBarcodeClick(TObject *Sender);
    void __fastcall TimerEPTimer(TObject *Sender);
    void __fastcall labMPaDblClick(TObject *Sender);
    void __fastcall btnIndexArmJogMove_UpClick(TObject *Sender);
    void __fastcall btnIndexArmJogMove_DownClick(TObject *Sender);
    void __fastcall chkUseAddWeightClick(TObject *Sender);
    void __fastcall rgOutKitDiameterClick(TObject *Sender);
    void __fastcall cbEnableUKClick(TObject *Sender);
    void __fastcall btnTempSetClick(TObject *Sender);
    void __fastcall edtPinOfDieMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edDoubleForceMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edAutoKSHTReleaseOfsMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnTrayMapClick(TObject *Sender);
    void __fastcall edForcePerDeviceKGClick(TObject *Sender);
    void __fastcall edtPinOfDieKeyPress(TObject *Sender, char &Key);
    void __fastcall AutoZTeachButtonClick(TObject *Sender);
    void __fastcall edForcePerPinNMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btTCPIPClick(TObject *Sender);
    void __fastcall edForcePerPinGMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall edDropByPassDetectMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall rgDieForceKitDiameterClick(TObject *Sender);
    void __fastcall cbRTCAutoTuningClick(TObject *Sender);
    void __fastcall timerContactTimer(TObject *Sender);
private:    // User declarations
    void __fastcall LoadImage();
    bool bAutoHighFinish;
    int iSpeedZ;
    int iSpeedZSlow;
    int iSpeed;
    int TorqueData;
    bool MotorStatus;
    bool bContinueContact;
    TQPF_Timer hContactDeley;
    TQPF_Timer DriverDelay;
    int RELEASE_UP_BIG  ;
    int RELEASE_UP_SMALL;
    void __fastcall Inital_ZTask();
    //bool __fastcall TestArmSuckUse(int X, int Y);
    bool __fastcall DoZ1PickFromShuttle();
    bool __fastcall DoZ2PickFromShuttle();
    bool __fastcall Do_Z1_AutoGetHeight();
    bool __fastcall Do_Z2_AutoGetHeight();
    bool __fastcall Do_ContactTest_32Site();                                    //Steven 20140512: For HT-9047
    bool __fastcall DoZPlaceToShuttle();
    bool CheckContactSettingChange();                                           //JerryYang 20170213 (Steven) 修改Run check觸發條件
     bool bOldRTCAutoTuning;                                                    //Sam 20240711 : RTC Auto Tuning

    //---------------------------------------------------------------------------
    enum eROILearningCase
    {
        rlcLightOn=1,                                                           //jou 2012-03-29 RTC啟動時,自動將燈箱打開
        rlcInitTask,
        rlcSendPassOn,
        rlcWaitPassOn,
        rlcSendReall,
        rlcSendReallDelay,
        rlcSendArmWork,
        rlcGetArmWork,
        rlcWaitReall,
        rlcSendPassOff,
        rlcWaitPassOff,
        rlcZ1DownZ2Up,
        rlcZ1UpZ2Down,
        rlcArmMoveCount,
        rlcSendArmIndex1,
        rlcWaitArmIndex1,
        rlcSendArmIndex2,
        rlcWaitArmIndex2,
        rlcArm2Down,
        rlcArm2Up,
        rlcDropICMoveZ,
        rlcDropICMoveY,
        rlcArmAwayForCheckSocket,
        rlcArm2BackToCenter,
        rlcWaitForCheckSocket,
        rlcGetFullOk,
        rlcGetHome,
        rlcArmOpen,
        rlcOnlyFullview,
        rlcTriggerAutoSTD,                                                      //JerryYang 20240829 : SPIL訓永 要求手動觸發RTC AUTO STD
        rlcResultAutoSTD,
    };

    //-------------------------------
    enum eAutoContactTestCase
    {
        actInitial=1,
        actArm2ToCenter,
        actArm2ZDown,
        actZ1DownZ2Up,
        actStartDelayTimeArm1,
        actSOTArm1,
        actGetTestResult1,
        actArmMoveCount1,
        actZ1UpZ2Down,
        actStartDelayTimeArm2,
        actSOTArm2,
        actGetTestResult2,
        actArmMoveCount2,
        actDropICMoveZ,
        actDropICMoveY,
        actDropICAlarm,
        actGetHome,
        actArmOpen,
        actHasErrorBin,
        actFinish
    };

    enum eStepContactTestLoadCase
    {
        sctlInit=1,
        sctlStep1,
        sctlLoadTray,
        sctlEditTray,
        sctlEditTrayOK,
        sctlFindICOnLoad,
        sctlInXYToLoad,
        sctlInZToLoadPick,
        sctlInLoadPick,
        sctlInLoadPickErrZUp,
        sctlInLoadPickErrAlarm,
        sctlInZLoadUp,
        sctlInXYToShuttle,
        sctlInZShuttlePut,
        sctlInShuttlePut,
        sctlInZShutleUp,
        sctlInputDevice,
        sctlBottom2DInit,
        sctlBottom2DScan

    };

    enum eStepContactTestUnloadCase
    {
        sctuInit=1,
        sctuInXYToShuttle,
        sctuInZToShuttlePick,
        sctuInShuttlePick,
        sctuInShuttlePickErrZUp,
        sctuInShuttlePickErrAlarm,
        sctuInZShutleUp,
        sctuInSerachLoader,
        sctuInXYToLoad,
        sctuInZToLoadPut,
        sctuInLoadPut,
        sctuInZLoadUp,
        sctuInXYAway,
        sctuInLoadPutTstart,
        sctuInLoadPutTstartZsafe
    };

    int  iFTestBackItem[4][8];
    int  iBTestBackItem[4][8];

    bool TestZ1Y2OutRandge();                                                   //Steven 20141007 : 換位置
    void TestZ1Y2SetPos();                                                      //Steven 20141007 : 換位置
    bool TestZ2Y1OutRandge();                                                   //Steven 20141007 : 換位置
    void TestZ2Y1SetPos();                                                      //Steven 20141007 : 換位置
    void SetContactMode();                                                      //Steven 20150224 : Auto Contact Test
    void ChangeContactMode(bool ret);                                           //Steven 20150224 : Auto Contact Test
    int  itemp;                                                                 //短暫判斷使用 //JimmyChiu 20211028 : All speed can set by speed setting.
    bool brecordmsgLock;
    cDMP_RICHTEK dmp_RICHTEK;
public:     // User declarations
    int iDoFullViewCheck;                                                       //ChungHung 20130122 add for SCK
    int Z_Height_Task;
    int Z_PickFromSocketTask;
    bool bSetupStart;
    __fastcall TfContact(TComponent* Owner);
    __fastcall ~TfContact();                                                    //JimmyChiu 20220121 add
    void __fastcall ReadFile();
    bool fShow;
    void __fastcall DoIniDataToForm();
    void __fastcall ShowArmAndDeviceForce();
    bool bSetupStep;
    bool bOneCycleFinish;
    double dDutCount;
    int CarlibrationTask;

    void __fastcall InitCarlibrationTask();                                     //auto high start
    void __fastcall OneCycleProcess();
    void __fastcall DoTestContactFunction();
    int GetAutoHeightMaxKGTorque();                                             //Steven 20170720 (wei) : for low contact force

    bool __fastcall Do_ROILearning(bool bQuickLearn=false);                     //Steven 20110827 : Real Time CCD - ROI Learning
    void __fastcall InitROILearningTask();
    bool __fastcall Do_AutoContactTest(bool Reset=false);                       //Steven 20150224 : Auto Contact Test
    bool __fastcall DoStepContactLoadDevice(bool Reset=false);                  //Steven 20150811 : Step by Step Contact Test
    bool __fastcall DoStepContactUnloadDevice(bool Reset=false);                //Steven 20150811 : Step by Step Contact Test

    bool __fastcall Do2DIDMapCheck(bool Reset=false);                           //JerryYang 20250220 : 2DID硬體順序檢查功能
    bool __fastcall Do2DIDMapCheckLoadDevice(bool Reset=false);                 //JerryYang 20250220 : 2DID硬體順序檢查功能
    bool __fastcall Do2DIDMapCheckUnloadDevice(bool Reset=false);               //JerryYang 20250220 : 2DID硬體順序檢查功能

    int ROILearningTimes;                                                       //Steven 20120222 : ROI的學習次數
    void __fastcall DutCount();
    void InitDoFullViewCheck();                                                 //ChungHung 20130122 add for SCK
    bool DoFullViewCheck();                                                     //ChungHung 20130122 add for SCK
    void __fastcall SaveSetupFile(AnsiString szDir, AnsiString S);              //kevin 20140105 儲存 檔案另存 jobfile
    bool Do_LoadCellAutoHigh(int iIndex);                                       //kevin 20190909 add load cell autohigh
    bool bUniversalkitflag[2];
    bool bSetHasIC;                                                             //Steven 20190626 : Fixed 2DID error in contact test
    bool DoDeviceMapCheck(bool Reset=false);                                    //Steven 20190910 : Qualcomm功能
    int iDeviceMapCheckTask;
    int i2DIDCheckTask;                                                         //JerryYang 20250220 : 2DID硬體順序檢查功能
    int i2DIDMapCheckLoadTask;                                                  //JerryYang 20250220 : 2DID硬體順序檢查功能
    int i2DIDMapCheckUnloadTask;                                                //JerryYang 20250220 : 2DID硬體順序檢查功能

    int iDMCTrayX[MAX_SOCKET_TOTAL], iDMCTrayY[MAX_SOCKET_TOTAL], iDMCArm, iDMCChannel[MAX_SOCKET_TOTAL];
    void TransformDMCMap();                                                     //Steven 20220510 : For QTI SD Device Map Function
    void ResetDMCTrayData();
    bool bWaitLoadXY, bTrayFeed, bPlaceLoad;
    int iTrayX, iTrayY;
    void CountDieForceKg(bool bByPinCount);
    bool DoSocketSensorCheckRemainIC();                                         //Isaac 20220120 : 全智要求在contact test和autoheight前，用socket sensor檢查socket是否有殘料，預防IC壓傷
    int ROILearningTask;
    int AutoContactTestTask;
    int StepContactTestLoadTask;
    int StepContactTestUnloadTask;
    void InitContactEdtList();                                                  //Steven 20190614 : 重新整理Edit to 變數的方式
    void InitContactCbList();
    TStringList *sOutDiameter;                                                  //kevin  20210814 add OutDiameter data 40,50,75,108
    void ATC_SwitchTjSignal(int iIndexArm, bool bSnedArmDown=true);             //Ifor 20210622 add: ATC Switch TJ
    void SetIndexDownPos();
    int GotIndexYSpeed(int _fixValue);                                          //JimmyChiu 20211028 : All speed can set by speed setting. iContactMode==CONTACT_DEVICE_MAP_CHECK
    int GotIndexZSpeed(int _fixValue);                                          //JimmyChiu 20211028 : All speed can set by speed setting. iContactMode==CONTACT_DEVICE_MAP_CHECK
    double CalculateTotalAirForce(double dBallCount,double dSingleGf);          //JimmyChiu 20220119 獨立計算空氣壓力
    double GetMaxIndexForceLimit();                                             //JimmyChiu 20220119 獨立檢查空氣壓力
    double GetMinForce(double dKitDiameter,int iTag);                           //JimmyChiu 20220119 獨立檢查最小空壓
    void TestZ_CompensationHight();                                             //Ifor 20200318 : add 鋼徑80mmn 因為機台Hold不住變形需補下壓高度
    bool DoIndecxCHECkFunction(bool reset);                                     //kevin 20220426 index check

    int  iDoArm1PlaceToShuttleTask;                                             //JimmyChiu 20220708 : add Index Arm Axis
    bool DoArm1PlaceToShuttle(bool bIsFirst);
    int  iDoArm2PlaceToShuttleTask;
    bool DoArm2PlaceToShuttle(bool bIsFirst);

    void ClearTTLSignal();
    double dMinForce;
    int iRTCAutoTuningTask;                                                     //Sam 20230419 : 新增 RTC Auto Tuning 功能
    void InitRTCAutoTuning();
    bool DoRTCAutoTuning();
    bool DoContactDeviceLoopTest(bool reset);                                   //Ztex 2023.11.19 Add CONTACT_DEVICE_LOOP_TEST
    bool DoContactKTemperatureTest(bool reset);                                 //Ztex 2024.03.26 Add Contact Mode K Temperature

    bool __fastcall RestoreData_2D_Check();                                     //JerryYang 20250220 : 2DID硬體順序檢查功能  //JerryYang 20250428 : 改成回傳bool
    bool IsRun2DCheck();
    void __fastcall Inital_Z1PickFromSocketTask();                              //JerryYang 20250515 : 一丟一測Auto height把IC丟到SOCKET
    bool __fastcall DoZ1PickFromSocket();

    enum e2DIDCheckStep                                                         //JerryYang 20250220 : 2DID硬體順序檢查功能
    {
        eSHT1RowA=0,
        eSHT1RowB,
        eSHT2RowA,
        eSHT2RowB,
        eBottomBarCode1_1,
        eBottomBarCode1_2,
        eBottomBarCode2_1,
        eBottomBarCode2_2,
        eCOMPLETE
    };
    void ShowMSG(AnsiString asStr);
};
//---------------------------------------------------------------------------
extern PACKAGE TfContact *fContact;
extern const int iSuckDelay;
//---------------------------------------------------------------------------
#endif
