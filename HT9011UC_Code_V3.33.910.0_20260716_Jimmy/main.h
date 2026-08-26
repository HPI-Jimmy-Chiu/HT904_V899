//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include "PowerSavingMode.h"
//---------------------------------------------------------------------------
#include "ALed.hpp"
#include "butPa1.h"
#include "HTray.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include "MyStringList.h"
#include "MyMemo.h"
#include <ScktComp.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include "MyMemo.h"
#include "BtnPanelLane.h"
#include "MyLed.h"
#include <ActnList.hpp>
#include <map>
#include "myTimer.h"
#include "uPadInterface.h"                                                      //Ken 20210702 AddPadInterface
#include "AutoTeach.h"
#include "cInArmPlacement.h"
#include "MyKitSuck.h"
#include "uPAT_Function.h"
#include "HANA_ART.h"
#include "uPAT_Function.h"
#include "uRENESAS_Server.h"                                                    //RogerYang 20250812 : 瑞薩FT-CT
using namespace std;
//==============================================================================
//Click Event,Auto Save log
//==>
//---------------------------------------------------------------------------
class TMyProc
{
private:    // User declarations
public:     // User declarations
    AnsiString asOwnName;                                                       //記錄元件名稱//
    TWndMethod OldWinProc;
    AnsiString asCaption;
    AnsiString asOwner;
    void __fastcall MyMessage(TMessage & Message) ;
};
//---------------------------------------------------------------------------
//<==
//Click Event,Auto Save log
//---------------------------------------------------------------------------
class TfMain : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pgMain;
    TTabSheet *tsMain;
    TTabSheet *tsMotionView;
    TGroupBox *gbControlBtn;
    TBtnPanel *BtnReset;
    TBtnPanel *BtnPause;
    TBtnPanel *BtnHome;
    TBtnPanel *BtnStart;
    TBtnPanel *BtnOneCycle;
    TBtnPanel *BtnCleanOut;
    TBtnPanel *BtnAlarmReset;
    TBtnPanel *BtnTrayEnd;
    TScrollBox *ScrollBox1;
    TTimer *Timer1;
    TTimer *Timer2;
    TPopupMenu *PopupMenu4;
    TMenuItem *RecordSuckRealTimeVacuumOnOffTime;
    TMenuItem *ModifySuckVacuumOnOffTime;
    TTimer *Timer3;
    TMenuItem *SetToDefineValue1;
    TSpeedButton *sbStateRecord;
    TImage *ImageRecord;
    TStringGrid *StringGrid6;
    TStringGrid *StringGrid7;
    TStringGrid *StringGrid9;
    TStringGrid *StringGrid11;
    TSpeedButton *sbTeaching;
    TSpeedButton *sbReturnToMain;
    TTimer *Timer4;
    TLabel *iWhichKitLabel;
    TButton *Button1;
    TTimer *TimerESD;
    TTimer *TimerTemperatureStorageMinute;
    TButton *Button3;
    TTimer *Timer5;
    TTimer *Timer6;
    TTimer *Timer7;
    TGroupBox *gbRemoteControl;
    TBtnPanel *BtnSTEP;
    TBtnPanel *BtnT_Start;
    TBtnPanel *BtnZUpDown;
    TButton *Button4;
    TButton *Button5;
    TSpeedButton *sbpContact2;
    TTimer *TimerScanKey;
    TTimer *Timer8;
    TGroupBox *gbSimulate;
    TCheckBox *cb1;
    TRadioGroup *rgHP;
    TEdit *edHPX;
    TEdit *edHPY;
    TLabel *Label11;
    TLabel *Label15;
    TCheckBox *cbIndexDrop;
    TSpeedButton *sbShuttleMaintain;
    TTimer *Timer9;
    TCheckBox *chkInPickLoadError;
    TServerSocket *TeraTCPResultServer;
    TServerSocket *TCPCommandServer;
    TLabel *Label1;
    TLabel *Label2;
    TTimer *TimerDLL;
    TLabel *LabZ1_Test;
    TLabel *LabZ2_Test;
    TOpenDialog *OpenDialog1;
    TSaveDialog *SaveDialog1;
    TPopupMenu *PopupMenu1;
    TButton *Button2;
    TCheckBox *chkCleanPadPickErr;
    TCheckBox *chkInToShtDrop;
    TCheckBox *chkInFromLoadDrop;
    TPanel *pnlBackGround;
    TPageControl *pgMotionView;
    TTabSheet *tsMotorView;
    TPanel *Panel17;
    TCheckBox *cbCheckEncoderEveryTime;
    TScrollBox *PanelMain3;
    TStringGrid *StringGrid1;
    TStringGrid *StringGrid3;
    TTabSheet *tsActionView;
    TPanel *PanelMain6;
    TLabel *lbEnCoder0;
    TLabel *lbEnCoder1;
    TLabel *Label109;
    TLabel *AutoCleanContactCountLabel;
    TLabel *lblShuttle1;
    TLabel *lblShuttle2;
    TTMyTray *mtAuto6;
    TTMyTray *mtAuto5;
    TTMyTray *mtAuto4;
    TPageControl *PageControl6;
    TTabSheet *TabSheet17;
    TTMyTray *mtFix2;
    TTMyTray *mtFix3;
    TTMyTray *mtFix1;
    TTMyTray *mtFix4;
    TTMyTray *mtFix5;
    TTMyTray *mtFix6;
    TPanel *Panel60;
    TTMyTray *mtPlate1;
    TTMyTray *mtPlate2;
    TTMyTray *mtLoader;
    TTMyTray *mtEmpty;
    TTMyTray *mtColor;
    TTMyTray *mtAuto1;
    TTMyTray *mtAuto2;
    TTMyTray *mtAuto3;
    TPanel *Panel62;
    TPanel *pnlInSht1;
    TALed *led_FLCarryKitAa;
    TALed *led_FLCarryKitAb;
    TALed *led_FLCarryKitAc;
    TALed *led_FLCarryKitAd;
    TALed *led_FLCarryKitBa;
    TALed *led_FLCarryKitBb;
    TALed *led_FLCarryKitBc;
    TALed *led_FLCarryKitBd;
    TCheckBox *CheckBox4;
    TTMyTray *mtEmpty1;
    TPanel *pnlInSht2;
    TALed *led_BLCarryKitAa;
    TALed *led_BLCarryKitAb;
    TALed *led_BLCarryKitAc;
    TALed *led_BLCarryKitAd;
    TALed *led_BLCarryKitBa;
    TALed *led_BLCarryKitBb;
    TALed *led_BLCarryKitBc;
    TALed *led_BLCarryKitBd;
    TPanel *palArm2;
    TALed *led_BTestSuckAa;
    TALed *led_BTestSuckAb;
    TALed *led_BTestSuckAc;
    TALed *led_BTestSuckAd;
    TLabel *lbT1;
    TALed *led_BTestSuckBa;
    TALed *led_BTestSuckBb;
    TALed *led_BTestSuckBc;
    TALed *led_BTestSuckBd;
    TALed *led_BTestSuckAe;
    TALed *led_BTestSuckAf;
    TALed *led_BTestSuckAg;
    TALed *led_BTestSuckAh;
    TALed *led_BTestSuckBe;
    TALed *led_BTestSuckBf;
    TALed *led_BTestSuckBg;
    TALed *led_BTestSuckBh;
    TPanel *palArm1;
    TALed *led_FTestSuckAa;
    TALed *led_FTestSuckAb;
    TALed *led_FTestSuckAc;
    TALed *led_FTestSuckAd;
    TLabel *lbT0;
    TALed *led_FTestSuckBa;
    TALed *led_FTestSuckBb;
    TALed *led_FTestSuckBc;
    TALed *led_FTestSuckBd;
    TALed *led_FTestSuckAe;
    TALed *led_FTestSuckAf;
    TALed *led_FTestSuckAg;
    TALed *led_FTestSuckAh;
    TALed *led_FTestSuckBe;
    TALed *led_FTestSuckBf;
    TALed *led_FTestSuckBg;
    TALed *led_FTestSuckBh;
    TGroupBox *grp8SiteBelow;
    TALed *ledSocketAa;
    TALed *ledSocketAc;
    TALed *ledSocketBa;
    TALed *ledSocketBc;
    TALed *ledSocketAb;
    TALed *ledSocketBb;
    TALed *ledSocketAd;
    TALed *ledSocketBd;
    TALed *ledSocketAe;
    TALed *ledSocketAg;
    TALed *ledSocketBe;
    TALed *ledSocketBg;
    TALed *ledSocketAf;
    TALed *ledSocketBf;
    TALed *ledSocketAh;
    TALed *ledSocketBh;
    TALed *ledSocketCa;
    TALed *ledSocketCc;
    TALed *ledSocketDa;
    TALed *ledSocketDc;
    TALed *ledSocketCb;
    TALed *ledSocketDb;
    TALed *ledSocketCd;
    TALed *ledSocketDd;
    TALed *ledSocketCe;
    TALed *ledSocketCg;
    TALed *ledSocketDe;
    TALed *ledSocketDg;
    TALed *ledSocketCf;
    TALed *ledSocketDf;
    TALed *ledSocketCh;
    TALed *ledSocketDh;
    TPanel *pnlOutSht1;
    TALed *led_FRCarryKitAa;
    TALed *led_FRCarryKitAb;
    TALed *led_FRCarryKitAc;
    TALed *led_FRCarryKitAd;
    TALed *led_FRCarryKitBa;
    TALed *led_FRCarryKitBb;
    TALed *led_FRCarryKitBc;
    TALed *led_FRCarryKitBd;
    TPanel *pnlOutSht2;
    TALed *led_BRCarryKitAa;
    TALed *led_BRCarryKitAb;
    TALed *led_BRCarryKitAc;
    TALed *led_BRCarryKitAd;
    TALed *led_BRCarryKitBa;
    TALed *led_BRCarryKitBb;
    TALed *led_BRCarryKitBc;
    TALed *led_BRCarryKitBd;
    TPanel *palShuttle1;
    TALed *led_FLCarryKit_0;
    TALed *led_FL2CarryKit_0;
    TALed *led_FLCarryKit_1;
    TALed *led_FL2CarryKit_1;
    TALed *led_FRCarryKit_0;
    TALed *led_FR2CarryKit_0;
    TALed *led_FRCarryKit_1;
    TALed *led_FR2CarryKit_1;
    TALed *led_FLCarryKit_2;
    TALed *led_FL2CarryKit_2;
    TALed *led_FLCarryKit_3;
    TALed *led_FL2CarryKit_3;
    TALed *led_FRCarryKit_2;
    TALed *led_FR2CarryKit_2;
    TALed *led_FRCarryKit_3;
    TALed *led_FR2CarryKit_3;
    TALed *led_FL3CarryKit_0;
    TALed *led_FL4CarryKit_0;
    TALed *led_FL3CarryKit_1;
    TALed *led_FL4CarryKit_1;
    TALed *led_FL3CarryKit_2;
    TALed *led_FL4CarryKit_2;
    TALed *led_FL3CarryKit_3;
    TALed *led_FL4CarryKit_3;
    TALed *led_FR3CarryKit_0;
    TALed *led_FR4CarryKit_0;
    TALed *led_FR3CarryKit_1;
    TALed *led_FR4CarryKit_1;
    TALed *led_FR3CarryKit_2;
    TALed *led_FR4CarryKit_2;
    TALed *led_FR3CarryKit_3;
    TALed *led_FR4CarryKit_3;
    TPanel *palShuttle2;
    TALed *led_BLCarryKit_0;
    TALed *led_BL2CarryKit_0;
    TALed *led_BLCarryKit_1;
    TALed *led_BL2CarryKit_1;
    TALed *led_BRCarryKit_0;
    TALed *led_BR2CarryKit_0;
    TALed *led_BRCarryKit_1;
    TALed *led_BR2CarryKit_1;
    TALed *led_BLCarryKit_2;
    TALed *led_BL2CarryKit_2;
    TALed *led_BLCarryKit_3;
    TALed *led_BL2CarryKit_3;
    TALed *led_BRCarryKit_2;
    TALed *led_BR2CarryKit_2;
    TALed *led_BRCarryKit_3;
    TALed *led_BR2CarryKit_3;
    TALed *led_BL3CarryKit_0;
    TALed *led_BL4CarryKit_0;
    TALed *led_BL3CarryKit_1;
    TALed *led_BL4CarryKit_1;
    TALed *led_BL3CarryKit_2;
    TALed *led_BL4CarryKit_2;
    TALed *led_BL3CarryKit_3;
    TALed *led_BL4CarryKit_3;
    TALed *led_BR3CarryKit_0;
    TALed *led_BR4CarryKit_0;
    TALed *led_BR3CarryKit_1;
    TALed *led_BR4CarryKit_1;
    TALed *led_BR3CarryKit_2;
    TALed *led_BR4CarryKit_2;
    TALed *led_BR3CarryKit_3;
    TALed *led_BR4CarryKit_3;
    TTMyTray *htShullte0;
    TTMyTray *htShullte1;
    TTMyTray *tmyAutoClean;
    TTMyTray *mtOCR;
    TTMyTray *tmyOutputRotateKit;
    TTMyTray *tmyInputRotateKit;
    TTMyTray *tBulkboxKit;
    TTMyTray *mtOutArmAutoAlignmentTray;
    TTMyTray *mtInArmAutoAlignmentTray;
    TPanel *pnlMotionBtm;
    TALed *ALed1;
    TALed *ALed5;
    TLabel *Label13;
    TLabel *Label12;
    TLabel *Label10;
    TALed *aledbOneCycle_BackUp;
    TCheckBox *CheckBox1;
    TPanel *Panel7;
    TALed *ALed4;
    TPanel *palTrayZ;
    TALed *ALed3;
    TPanel *palEmptyZ;
    TALed *ALed2;
    TCheckBox *CheckBox2;
    TPanel *Panel101;
    TALed *ALed21;
    TCheckBox *ckDummyUPH;
    TEdit *edLoadCnt;
    TEdit *edtAuto2;
    TEdit *edtAuto3;
    TCheckBox *chkOneCycleContinous;
    TEdit *edtAuto4;
    TEdit *edtAuto5;
    TEdit *edtAuto6;
    TPanel *pnlTrayCar;
    TTMyTray *mtLoaderBuffer;
    TTMyTray *mtAuto1Car;
    TTMyTray *mtAuto2Car;
    TTMyTray *mtAuto3Car;
    TTMyTray *mtColor_Car;
    TTMyTray *mtEmpty_Car;
    TTMyTray *mtEmpty1_Car;
    TTMyTray *mtAuto4Car;
    TTMyTray *mtAuto5Car;
    TTMyTray *mtAuto6Car;
    TPanel *Panel100;
    TALed *ALed89;
    TLabel *lblLoadTrayCnt;
    TLabel *lblAuto1TrayCnt;
    TLabel *lblAuto2TrayCnt;
    TLabel *lblAuto3TrayCnt;
    TPanel *Panel61;
    TShape *saInArmX;
    TLabel *Label3;
    TPanel *pnlInArmX;
    TALed *ledInArmAa;
    TALed *ledInArmAb;
    TALed *ledInArmAc;
    TALed *ledInArmAd;
    TALed *ledInArmAe;
    TALed *ledInArmAf;
    TALed *ledInArmAg;
    TALed *ledInArmAh;
    TALed *ledInArmBa;
    TALed *ledInArmBb;
    TALed *ledInArmBc;
    TALed *ledInArmBd;
    TALed *ledInArmBe;
    TALed *ledInArmBf;
    TALed *ledInArmBg;
    TALed *ledInArmBh;
    TPanel *pnlUnitSpeedDisplay;
    TPanel *pnlSpeed1;
    TPanel *pnlSpeed2;
    TPanel *pnlSpeed3;
    TPanel *pnlSpeed4;
    TPanel *pnlOutArmY;
    TShape *saOutArmX;
    TPanel *pnlOutArmX;
    TALed *ledOutArmBa;
    TALed *ledOutArmBb;
    TALed *ledOutArmBc;
    TALed *ledOutArmBd;
    TALed *ledOutArmAa;
    TALed *ledOutArmAb;
    TALed *ledOutArmAc;
    TALed *ledOutArmAd;
    TALed *ledOutArmBe;
    TALed *ledOutArmBf;
    TALed *ledOutArmBg;
    TALed *ledOutArmBh;
    TALed *ledOutArmAe;
    TALed *ledOutArmAf;
    TALed *ledOutArmAg;
    TALed *ledOutArmAh;
    TPanel *pnlSimMagazineTray;
    TLabel *labSimMagazineTrayNo;
    TTMyTray *mtMagazineTray1;
    TTMyTray *mtMagazineTray2;
    TTMyTray *mtMagazineTray3;
    TTMyTray *mtMagazineTray4;
    TTMyTray *mtMagazineTray5;
    TTMyTray *mtMagazineTray6;
    TPanel *pnlSimMagazineZ;
    TTMyTray *mtMagazineTray7;
    TTMyTray *mtMagazineTray8;
    TTMyTray *mtMagazineTray9;
    TTMyTray *mtMagazineTray10;
    TTMyTray *mtMagazineTray11;
    TTMyTray *mtMagazineTray12;
    TTMyTray *mtMagazineTray13;
    TTMyTray *mtMagazineTray14;
    TStringGrid *AutoCleanStringGrid;
    TTabSheet *tsCommView;
    TPanel *PanelMain7;
    TPageControl *pcCommView;
    TTabSheet *TabSheet4;
    TPanel *pal_RS232;
    TLabel *Label16;
    TCheckBox *chkReadTorque1;
    TCheckBox *chkReadTorque2;
    TEdit *edTorue0;
    TEdit *edTorue1;
    TEdit *edtReadZ1;
    TEdit *edtReadZ2;
    TButton *btnReadZ1;
    TButton *btnReadZ2;
    TEdit *edtSetZ1;
    TEdit *edtSetZ2;
    TButton *btnSetZ1;
    TButton *btnSetZ2;
    TGroupBox *grpTorqueFunction;
    TButton *btnStartTorque;
    TButton *btnStopTorque;
    TButton *Button12;
    TButton *Button6;
    TPageControl *pgcTorque;
    TTabSheet *tsTorqueCommData;
    TListBox *ListBox14;
    TPanel *Panel192;
    TTabSheet *tsTorqueChart;
    TChart *chtTorque;
    TFastLineSeries *Series1;
    TFastLineSeries *Series2;
    TTabSheet *tsInArm;
    TStringGrid *StringGrid2;
    TEdit *VacuumOnOffTimeEdit;
    TTabSheet *TrayMap;
    TStringGrid *StringGrid8;
    TStringGrid *StringGrid10;
    TTabSheet *TabSheet5;
    TMemo *MmoTj;
    TMemo *Memo1;
    TTabSheet *TCPIPLOG;
    TMemo *Memo2;
    TTabSheet *tsTrayStepMotor;
    TMemo *mmoTrayStepMotor;
    TTabSheet *tsVibrationMotor;
    TMemo *mmoVibrationMotor;
    TTabSheet *IndexArmYPos;
    TMemo *MemoIndexPosLog;
    TPanel *palIndexShiftFunction;
    TCheckBox *cbArm1;
    TCheckBox *cbArm2;
    TCheckBox *cbBoth;
    TButton *btnRecordSHT;
    TButton *btnRecordSocket;
    TButton *btnSaveIndexPosLog;
    TButton *btnClearIndexPosLog;
    TButton *btnSaveMaxMin;
    TTabSheet *tsHeaterView;
    TPanel *PanelMain8;
    TStringGrid *StringGrid5;
    TPanel *palHP2View;
    TPanel *palHP1View;
    TStringGrid *StringGrid4;
    TTabSheet *tsLogs;
    TScrollBox *ScrollBox2;
    TGroupBox *gbGpibMonitor;
    TLabel *lblGPIBWND;
    TMemo *mmo1;
    TGroupBox *gbHomeLog;
    TMemo *MemoHome;
    TGroupBox *gbEPLog;
    TMemo *EPMemo;
    TGroupBox *gbHeaterLog;
    TMemo *MemoHeaterLog;
    TGroupBox *gbProductionLog;
    TMemo *MemoProductionLog;
    TGroupBox *gbTSDLog;
    TMyMemo *memoTSD;
    TTabSheet *tsShuttleSensor;
    TPanel *palShtSensor;
    TPanel *palOutShtSensor2;
    TPanel *palOutShtSensor1;
    TCheckBox *cbShowShuttleSensor;
    TMemo *meShuttle2;
    TMemo *meShuttle1;
    TCheckBox *cbTestOutShuttleSensor;
    TTabSheet *tsRecord;
    TPanel *PanelMain2;
    TSpeedButton *spbClearRecord;
    TStringGrid *sgDebugRecord;
    TPanel *palCounter;
    TMemo *memoAutoClean;
    TButton *btSavelog;
    TMemo *AseRecordMemo;
    TTabSheet *tsTaskList;
    TStringGrid *sgTaskList;
    TTabSheet *tsUnloaderInfo;
    TPanel *palUnloaderInfo;
    TStringGrid *sgUnloaderInfo;
    TPanel *palUnloaderInfo1;
    TRadioGroup *rgUnloaderInfo;
    TComboBox *cbUnloaderinfo;
    TTabSheet *tsMNetLog;
    TMemo *mmoMNet;
    TTabSheet *ts1;
    TEdit *edtLogin;
    TMemo *Memo3_AOA_IN;
    TMemo *Memo3_AOA_OUT;
    TPanel *Panel14;
    TLabel *labLoader_X;
    TLabel *labLoader_Y;
    TLabel *labHotplate1_X;
    TLabel *labHotplate1_Y;
    TLabel *labHotplate2_X;
    TLabel *labHotplate2_Y;
    TLabel *labInShuttle1_X;
    TLabel *labInShuttle1_Y;
    TLabel *labInShuttle2_X;
    TLabel *labInShuttle2_Y;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label14;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *labOutShuttle1_X;
    TLabel *labOutShuttle1_Y;
    TLabel *Label21;
    TLabel *labOutShuttle2_X;
    TLabel *labOutShuttle2_Y;
    TLabel *Label24;
    TLabel *labAuto1_X;
    TLabel *labAuto1_Y;
    TLabel *Label27;
    TLabel *labAuto2_X;
    TLabel *labAuto2_Y;
    TLabel *labAuto3_X;
    TLabel *Label30;
    TLabel *labAuto3_Y;
    TLabel *Label33;
    TLabel *labFix1_X;
    TLabel *labFix1_Y;
    TLabel *Label36;
    TLabel *labFix2_X;
    TLabel *labFix2_Y;
    TLabel *Label39;
    TLabel *labFix3_X;
    TLabel *labFix3_Y;
    TSpeedButton *OffsetSave;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label25;
    TEdit *Ed_InSH2Offset_X;
    TEdit *Ed_InSH1Offset_X;
    TEdit *Ed_HP2Offset_X;
    TEdit *Ed_HP1Offset_X;
    TEdit *Ed_LoaderOffset_X;
    TEdit *Ed_InSH1Offset_Y;
    TEdit *Ed_LoaderOffset_Y;
    TEdit *Ed_HP1Offset_Y;
    TEdit *Ed_HP2Offset_Y;
    TEdit *Ed_InSH2Offset_Y;
    TEdit *Ed_OutSH1Offset_X;
    TEdit *Ed_OutSH1Offset_Y;
    TEdit *Ed_OutSH2Offset_X;
    TEdit *Ed_OutSH2Offset_Y;
    TEdit *Ed_Auto1Offset_X;
    TEdit *Ed_Auto1Offset_Y;
    TEdit *Ed_Auto2Offset_X;
    TEdit *Ed_Auto2Offset_Y;
    TEdit *Ed_Auto3Offset_X;
    TEdit *Ed_Auto3Offset_Y;
    TEdit *Ed_Fix1Offset_X;
    TEdit *Ed_Fix1Offset_Y;
    TEdit *Ed_Fix2Offset_X;
    TEdit *Ed_Fix2Offset_Y;
    TEdit *Ed_Fix3Offset_X;
    TEdit *Ed_Fix3Offset_Y;
    TSpeedButton *sbLaguage;
    TSpeedButton *sbCloseProgram;
    TSpeedButton *sbSetting;
    TSpeedButton *sbConfig;
    TSpeedButton *sbOffset;
    TSpeedButton *sbSpeed;
    TSpeedButton *sbIO;
    TSpeedButton *sbMessage;
    TSpeedButton *sbCalculator;
    TPanel *Panel2;
    TLabel *labTrayName1;
    TLabel *labSetupFile;
    TLabel *labUser;
    TLabel *labTestSite;
    TLabel *labBinSetting;
    TSpeedButton *spbUserName;
    TSpeedButton *sbEngSite;
    TEdit *edTrayName1;
    TComboBox *cbUserSelect;
    TButton *btLogin;
    TPanel *palFT;
    TPanel *palRT;
    TPanel *palOffLine;
    TEdit *edSetupFileName;
    TPanel *SitePanel;
    TTMyTray *mtDutRow;
    TTMyTray *mtDutOnOff;
    TTMyTray *mtDutCol;
    TPanel *palArmSeperater;
    TPanel *palEQC;
    TPanel *Off_lineDisplay;
    TLabel *AutoCleanPad;
    TPanel *palMainStatus;
    TLabel *labHonQC;
    TLabel *PiggyBackLab;
    TLabel *lbCCDStatus;
    TLabel *labFailAlarmCnt;
    TLabel *labTesterMode;
    TLabel *lblInOutAlarm;
    TLabel *labQAMode;
    TLabel *labARTmode;
    TLabel *ARTCombine;
    TLabel *labDelayStatus;
    TLabel *lbl2DSort;
    TLabel *labARTStop;
    TCheckBox *cbDisableSiteMappingCheck;
    TPanel *palSetting;
    TLabel *labTester;
    TLabel *labRunMode;
    TLabel *labStartMode;
    TLabel *Scanner;
    TImage *imgLogo;
    TSpeedButton *spbFan;
    TSpeedButton *spbLight;
    TLabel *lbSetOpenBin;
    TALed *ledYellow;
    TALed *ledRed;
    TSpeedButton *spbChamberFan;
    TALed *ledGreen;
    TSpeedButton *PutCleanPad;
    TSpeedButton *AutoCleanButton;
    TSpeedButton *sbStateRecord2;
    TLabel *labStopTime;
    TImage *ImgInRotateAngle;
    TImage *ImgOutRotateAngle;
    TLabel *labHSATC;
    TSpeedButton *sbPEModel;
    TLabel *labGuardBand;
    TLabel *labInRotate;
    TLabel *labOutRotate;
    TComboBox *cbRunStartMode;
    TPanel *palLedTower;
    TPanel *palTesterMode;
    TPanel *palRunMode_1;
    TPanel *palTester;
    TImage *imgTester;
    TPanel *palRunMode;
    TImage *imgRunMode;
    TPanel *palStartMode;
    TImage *imgStartMode;
    TComboBox *cbSimuBinSite0;
    TComboBox *cbSimuBinSite1;
    TComboBox *cbSimuBinSite2;
    TComboBox *cbSimuBinSite3;
    TEdit *edSetOpenBin;
    TPanel *palPrime;
    TPanel *palNormal;
    TComboBox *cbbRunModeSel;
    TCheckBox *cb_MainAutoSkipSwitch;
    TPanel *pnlTempSetting;
    TLabel *labTemperature;
    TLabel *labSoakTime;
    TLabel *lblTemperatureMode;
    TSpeedButton *spbSet;
    TLabel *labDeg;
    TEdit *edWorkTemperBase;
    TEdit *edSoakTime;
    TPanel *palTemperatureOnOff;
    TImage *imgTempOnOff;
    TPanel *palIndivisual;
    TEdit *edATCAmbientTemper;
    TPanel *pnlSafePLC;
    TALed *ledSafePLC;
    TPanel *palConfig;
    TSpeedButton *sbExit;
    TSpeedButton *sbSelete;
    TSpeedButton *sbClear;
    TSpeedButton *sbBuilder;
    TSpeedButton *sbStartMode;
    TSpeedButton *sbPassword;
    TSpeedButton *sbConfiguration;
    TSpeedButton *sbTowerLight;
    TSpeedButton *sbDioSet;
    TSpeedButton *sbSensorLatch;
    TSpeedButton *sbOmron;
    TSpeedButton *sbSecsGem;
    TSpeedButton *sbAutoTemp;
    TSpeedButton *sbMotionView;
    TSpeedButton *spbAirConditioner;
    TSpeedButton *sbPMAlarm;
    TSpeedButton *sbMonitorView;
    TSpeedButton *btnHelp;
    TPanel *palSetup;
    TSpeedButton *sbContact;
    TSpeedButton *sbBin;
    TSpeedButton *sbTester;
    TSpeedButton *sbTempOffset;
    TSpeedButton *sbTrayForm;
    TSpeedButton *sbPlateForm;
    TSpeedButton *sbTrayAssign;
    TSpeedButton *sbExitSetup;
    TSpeedButton *sbLdUld;
    TSpeedButton *sbSetup;
    TSpeedButton *sbYield;
    TSpeedButton *sbATC;
    TSpeedButton *sbCCD;
    TSpeedButton *sbShuttleSensor;
    TSpeedButton *sbAutoClean;
    TSpeedButton *sbOCR;
    TSpeedButton *sbDynamicTemp;
    TSpeedButton *sbSocket_ASE_KR;
    TSpeedButton *sbQAMode;
    TSpeedButton *sbBarCode;
    TSpeedButton *sbRotate;
    TSpeedButton *spbLaser;
    TSpeedButton *spbAutoRetest;
    TSpeedButton *spbTrayMapping;
    TSpeedButton *spbRPDefalut;
    TSpeedButton *spbGroundMan;
    TSpeedButton *sbFixButtomCCD;
    TSpeedButton *sbAutoAlignment;
    TSpeedButton *sbMagazine;
    TStatusBar *StatusBar1;
    TPanel *labScanAOI;
    TPanel *lbSocketSenDetect;
    TPanel *labOCR;
    TPanel *labATC;
    TPanel *labRTCCD;
    TPanel *lb_PMAlarmDate;
    TPanel *lbEPenconder;
    TPanel *lbArm1Torque;
    TPanel *lbArm0Torque;
    TPanel *lbCheckSafeDoorDisable;
    TPanel *labAutoClean;
    TPanel *labTrayMap;
    TPanel *labAutomation;
    TPanel *lbFTPOnoffStatus;
    TPanel *lbMonitorConnectState;
    TSpeedButton *btnView;
    TPanel *labAutoSkip;
    TRadioGroup *rgHotplateShowMessage;
    TButton *SaveHPPick;
    TButton *Button7;
    TButton *btnSaveHPTable;
    TPanel *lbl_TriTempState;
    TPanel *pnlPowerSaving;
    TMyLed *ledTrayVibra;
    TMyLed *ledSht1Vibra;
    TMyLed *ledSht2Vibra;
    TMyLed *ledLoadVibra;
    TMyLed *ledHPVibra;
    TTimer *Timer10;
    TPanel *pnlAOAForHT9011;
    TLabel *Label26;
    TLabel *labAuto4_X;
    TLabel *labAuto4_Y;
    TLabel *Label28;
    TLabel *labAuto5_X;
    TLabel *labAuto5_Y;
    TLabel *labAuto6_X;
    TLabel *Label29;
    TLabel *labAuto6_Y;
    TLabel *Label31;
    TLabel *labFix4_X;
    TLabel *labFix4_Y;
    TLabel *Label32;
    TLabel *labFix5_X;
    TLabel *labFix5_Y;
    TLabel *Label34;
    TLabel *labFix6_X;
    TLabel *labFix6_Y;
    TEdit *Ed_Auto4Offset_X;
    TEdit *Ed_Auto4Offset_Y;
    TEdit *Ed_Auto5Offset_X;
    TEdit *Ed_Auto5Offset_Y;
    TEdit *Ed_Auto6Offset_X;
    TEdit *Ed_Auto6Offset_Y;
    TEdit *Ed_Fix4Offset_X;
    TEdit *Ed_Fix4Offset_Y;
    TEdit *Ed_Fix5Offset_X;
    TEdit *Ed_Fix5Offset_Y;
    TEdit *Ed_Fix6Offset_X;
    TEdit *Ed_Fix6Offset_Y;
    TCheckBox *chkHeaterOk;
    TLabel *LabHisi_Set;
    TLabel *labAOAFunction;
    TLabel *LabEQCCount;
    TLabel *LabATCPowerSaveSec;
    TButton *btnGroundMan;
    TSpeedButton *sbSmartSetup;
    TPanel *labAOA;
    TPanel *pnlRear;
    TPanel *pnlFront;
    TPanel *pnlSaveSummary;
    TLabel *lblAuto4TrayCnt;
    TLabel *lblAuto5TrayCnt;
    TLabel *lblAuto6TrayCnt;
    TPanel *pnlCleanCount;
    TMyLed *ledFix3Pos;
    TSpeedButton *sbVacuumUnit;
    TEdit *edtAuto1;
    TTMyTray *mtLoader2;
    TTMyTray *mtLoaderBuffer2;
    TPanel *plAutoStartFTP;
    TCheckBox *chk_Honprec_Use;
    TPanel *labPFStatus;
    TActionList *ActionList1;
    TAction *ActListenStart;
    TComboBox *cbSetupFileName;
    TLabel *lblMaintMode;
    TPanel *pnlRenesFTCT;
    TSpeedButton *spbAGV;
    TPanel *pnlEnableIdxChk;
    TButton *btnLoadRfidTray;
    TButton *btnLoadLoaderTray;
    TButton *btnLoadEmptyTray;
    TButton *btnE84Auto1;
    TButton *btnE84Auto2;
    TButton *btnE84Fix2;
    TCheckBox *chkE84LoadTray;
    TCheckBox *chkE84IDTray;
    TPanel *palOutSht;
    TALed *led_SortShtKit_0;
    TALed *led_SortSht2Kit_0;
    TALed *led_SortShtKit_1;
    TALed *led_SortSht2Kit_1;
    TALed *led_SortShtKit_2;
    TALed *led_SortSht2Kit_2;
    TALed *led_SortShtKit_3;
    TALed *led_SortSht2Kit_3;
    TALed *led_SortSht3Kit_0;
    TALed *led_SortSht4Kit_0;
    TALed *led_SortSht3Kit_1;
    TALed *led_SortSht4Kit_1;
    TALed *led_SortSht3Kit_2;
    TALed *led_SortSht4Kit_2;
    TALed *led_SortSht3Kit_3;
    TALed *led_SortSht4Kit_3;
    TPanel *pnlSortArmY;
    TShape *Shape1;
    TPanel *pnlSortArmX;
    TALed *ledSortArmAa;
    TALed *ledSortArmAb;
    TCheckBox *cbShowInShuttleSensor;
    TSpeedButton *sbSaveMMI;
    TLabel *labMultiEP;
    TLabel *labPDD;                                                             //Eastsun 20260710整合
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BtnCleanOutClick(TObject *Sender);
    void __fastcall BtnOneCycleClick(TObject *Sender);
    void __fastcall BtnStartClick(TObject *Sender);
    void __fastcall BtnPauseClick(TObject *Sender);
    void __fastcall BtnHomeClick(TObject *Sender);
    void __fastcall BtnResetClick(TObject *Sender);
    void __fastcall sbLaguageClick(TObject *Sender);
    void __fastcall SpeedButton10Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall stOperatorClick(TObject *Sender);
    void __fastcall BtnTrayEndClick(TObject *Sender);
    void __fastcall cbUserSelectChange(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall Panel42Click(TObject *Sender);
    void __fastcall btnSetZ1Click(TObject *Sender);
    void __fastcall btnSetZ2Click(TObject *Sender);
    void __fastcall btnReadZ1Click(TObject *Sender);
    void __fastcall btnReadZ2Click(TObject *Sender);
    void __fastcall labTestSiteClick(TObject *Sender);
    void __fastcall BtnAlarmResetClick(TObject *Sender);
    void __fastcall cbRunStartModeChange(TObject *Sender);
    void __fastcall edWorkTemperBaseKeyPress(TObject *Sender, char &Key);
    void __fastcall edSoakTimeKeyPress(TObject *Sender, char &Key);
    void __fastcall pgMainChange(TObject *Sender);
    void __fastcall RecordSuckRealTimeVacuumOnOffTimeClick(
          TObject *Sender);
    void __fastcall StringGrid2SelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall StringGrid2DblClick(TObject *Sender);
    void __fastcall ModifySuckVacuumOnOffTimeClick(TObject *Sender);
    void __fastcall cbSetupFileNameChange(TObject *Sender);
    void __fastcall Timer3Timer(TObject *Sender);
    void __fastcall edSoakTimeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edWorkTemperBaseMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall SetToDefineValue1Click(TObject *Sender);
    void __fastcall mtFix2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall spbFanClick(TObject *Sender);
    void __fastcall spbLightClick(TObject *Sender);
    void __fastcall edtSetZ1Click(TObject *Sender);
    void __fastcall sbStateRecordClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall labAutomationDblClick(TObject *Sender);
    void __fastcall btLoginClick(TObject *Sender);
    void __fastcall spbSetClick(TObject *Sender);
    void __fastcall spbSetMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall sbContactClick(TObject *Sender);
    void __fastcall sbBinClick(TObject *Sender);
    void __fastcall sbTesterClick(TObject *Sender);
    void __fastcall sbTempOffsetClick(TObject *Sender);
    void __fastcall sbTrayFormClick(TObject *Sender);
    void __fastcall sbPlateFormClick(TObject *Sender);
    void __fastcall sbTrayAssignClick(TObject *Sender);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall sbLdUldClick(TObject *Sender);
    void __fastcall sbSetupClick(TObject *Sender);
    void __fastcall sbYieldClick(TObject *Sender);
    void __fastcall sbBuilderClick(TObject *Sender);
    void __fastcall sbStartModeClick(TObject *Sender);
    void __fastcall sbSeleteClick(TObject *Sender);
    void __fastcall sbClearClick(TObject *Sender);
    void __fastcall sbTowerLightClick(TObject *Sender);
    void __fastcall sbPasswordClick(TObject *Sender);
    void __fastcall sbConfigurationClick(TObject *Sender);
    void __fastcall sbDioSetClick(TObject *Sender);
    void __fastcall sbSpeedClick(TObject *Sender);
    void __fastcall sbReturnToMainClick(TObject *Sender);
    void __fastcall sbOffsetClick(TObject *Sender);
    void __fastcall sbTeachingClick(TObject *Sender);
    void __fastcall sbCalculatorClick(TObject *Sender);
    void __fastcall sbMessageClick(TObject *Sender);
    void __fastcall sbIOClick(TObject *Sender);
    void __fastcall sbConfigClick(TObject *Sender);
    void __fastcall sbSettingClick(TObject *Sender);
    void __fastcall sbCloseProgramClick(TObject *Sender);
    void __fastcall sbExitSetupClick(TObject *Sender);
    void __fastcall meShuttle2DblClick(TObject *Sender);
    void __fastcall cbSetupFileNameKeyPress(TObject *Sender, char &Key);
    void __fastcall cbSetupFileNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall spbChamberFanClick(TObject *Sender);
    void __fastcall Timer4Timer(TObject *Sender);
    void __fastcall cbCheckEncoderEveryTimeClick(TObject *Sender);
    void __fastcall CheckBox2Click(TObject *Sender);
    void __fastcall StringGrid1DblClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall sbATCClick(TObject *Sender);
    void __fastcall sbCCDClick(TObject *Sender);
    void __fastcall sbShuttleSensorClick(TObject *Sender);
    void __fastcall sbAutoCleanClick(TObject *Sender);
    void __fastcall sbOmronClick(TObject *Sender);
    void __fastcall sbSensorLatchClick(TObject *Sender);
    void __fastcall sbSecsGemClick(TObject *Sender);
    void __fastcall imgTesterClick(TObject *Sender);
    void __fastcall imgRunModeClick(TObject *Sender);
    void __fastcall sbOCRClick(TObject *Sender);
    void __fastcall sbAutoTempClick(TObject *Sender);
    void __fastcall sbDynamicTempClick(TObject *Sender);
    void __fastcall sbMotionViewClick(TObject *Sender);
    void __fastcall mtDutOnOffMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall sbSocket_ASE_KRClick(TObject *Sender);
    void __fastcall AutoCleanStringGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall AutoCleanStringGridDblClick(TObject *Sender);
    void __fastcall sbRotateClick(TObject *Sender);
    void __fastcall palFTClick(TObject *Sender);
    void __fastcall palRTClick(TObject *Sender);
    void __fastcall cbbRunModeSelChange(TObject *Sender);
    void __fastcall spbClearRecordClick(TObject *Sender);
    void __fastcall sbBarCodeClick(TObject *Sender);
    void __fastcall sbQAModeClick(TObject *Sender);
    void __fastcall cbSetupFileNameDropDown(TObject *Sender);
    void __fastcall btSavelogClick(TObject *Sender);
    void __fastcall AutoCleanButtonClick(TObject *Sender);
    void __fastcall PutCleanPadClick(TObject *Sender);
        void __fastcall PiggyBackLabClick(TObject *Sender);
    void __fastcall spbAirConditionerClick(TObject *Sender);
    void __fastcall palFTMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall palFTMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall palRTMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall palRTMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall cb_MainAutoSkipSwitchClick(TObject *Sender);
    void __fastcall spbLaserClick(TObject *Sender);
        void __fastcall palHP2ViewClick(TObject *Sender);
    void __fastcall TimerESDTimer(TObject *Sender);
    void __fastcall TimerTemperatureStorageMinuteTimer(TObject *Sender);
    void __fastcall sbStateRecord2Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Timer5Timer(TObject *Sender);
    void __fastcall Timer6Timer(TObject *Sender);
    void __fastcall Timer7Timer(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall BtnZUpDownClick(TObject *Sender);
    void __fastcall BtnT_StartClick(TObject *Sender);
    void __fastcall BtnSTEPClick(TObject *Sender);
    void __fastcall edSetupFileNameKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall TimerScanKeyTimer(TObject *Sender);
    void __fastcall Timer8Timer(TObject *Sender);
    void __fastcall sbPMAlarmClick(TObject *Sender);
    void __fastcall sbMonitorViewClick(TObject *Sender);
    void __fastcall cbRunStartModeClick(TObject *Sender);
    void __fastcall sbShuttleMaintainClick(TObject *Sender);
    void __fastcall sbPEModelClick(TObject *Sender);
    void __fastcall sbEngSiteClick(TObject *Sender);
    void __fastcall Timer9Timer(TObject *Sender);
    void __fastcall spbAutoRetestClick(TObject *Sender);
    void __fastcall spbTrayMappingClick(TObject *Sender);
    void __fastcall spbRPDefalutClick(TObject *Sender);
    void __fastcall mtAuto1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall labUserClick(TObject *Sender);
    void __fastcall TeraTCPResultServerClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TeraTCPResultServerClientDisconnect(
          TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall TeraTCPResultServerClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall TCPCommandServerClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TCPCommandServerClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TCPCommandServerClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall TimerDLLTimer(TObject *Sender);
    void __fastcall palEQCClick(TObject *Sender);
    void __fastcall palEQCMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall palEQCMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall labTesterClick(TObject *Sender);
    void __fastcall spbGroundManClick(TObject *Sender);
    void __fastcall cbDisableSiteMappingCheckClick(TObject *Sender);
    void __fastcall btnRecordSHTClick(TObject *Sender);
    void __fastcall btnRecordSocketClick(TObject *Sender);
    void __fastcall btnSaveIndexPosLogClick(TObject *Sender);
    void __fastcall btnClearIndexPosLogClick(TObject *Sender);
    void __fastcall btnSaveMaxMinClick(TObject *Sender);
    void __fastcall btnStartTorqueClick(TObject *Sender);
    void __fastcall btnStopTorqueClick(TObject *Sender);
    void __fastcall Button12Click(TObject *Sender);
    void __fastcall pgcTorqueChange(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall sbFixButtomCCDClick(TObject *Sender);
    void __fastcall btnHelpClick(TObject *Sender);
    void __fastcall sbAutoAlignmentClick(TObject *Sender);
    void __fastcall OffsetSaveClick(TObject *Sender);
    void __fastcall Ed_LoaderOffset_XClick(TObject *Sender);
    void __fastcall edHPXClick(TObject *Sender);
    void __fastcall sbMagazineClick(TObject *Sender);
    void __fastcall btnViewClick(TObject *Sender);
    void __fastcall SaveHPPickClick(TObject *Sender);
    void __fastcall btnSaveHPTableClick(TObject *Sender);
    void __fastcall rgHotplateShowMessageClick(TObject *Sender);
    void __fastcall Timer10Timer(TObject *Sender);
    void __fastcall sbSmartSetupClick(TObject *Sender);
    void __fastcall btnGroundManClick(TObject *Sender);
    void __fastcall LabATCPowerSaveSecMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnlSaveSummaryMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnlSaveSummaryMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall labRunModeClick(TObject *Sender);
    void __fastcall labTemperatureClick(TObject *Sender);
    void __fastcall sbVacuumUnitClick(TObject *Sender);
    void __fastcall pnlSimMagazineTrayClick(TObject *Sender); //Eastsun 20260515 F021
    void __fastcall cbSetupFileNameCloseUp(TObject *Sender);
    void __fastcall sbCalculatorDblClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall spbAGVClick(TObject *Sender);
    void __fastcall btnLoadLoaderTrayClick(TObject *Sender);
    void __fastcall btnLoadRfidTrayClick(TObject *Sender);
    void __fastcall btnE84Auto1Click(TObject *Sender);                          //AI(general) 20260401 (RogerYang) : ATK AMR manual tray pickup

    void __fastcall btnE84Fix2Click(TObject *Sender);
    void __fastcall edtLoginKeyPress(TObject *Sender, char &Key);
    void __fastcall edtLoginMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall sbSaveMMIClick(TObject *Sender);
private:    // User declarations
    TQPF_Timer WakeupGPIBdelay;
    TQPF_Timer WakeupEventLogSaverdelay;                                        //Frank 20140902
    TQPF_Timer WakeupESDdelay;
    TQPF_Timer WakeupHeartBeatdelay;                                            //Steven 20211104 : 增加機台的心跳紀錄
    int     iSaveImgae;
    int     iSaveImageCT;
    int     iSaveImageTask;
    AnsiString NewPath;
    //bool    bSystemClose; kevin 20150423
    int     OldTestMode;
    int     oldLastSetiTester;
    int     iSaveLogTimePeridCount;                                             //JerryYang 20151026 ProductionData存檔時間計數

    AnsiString IconPath;
    void    StateRecordImage();

    bool Define_Suck1_Flag;
    bool Define_Suck2_Flag;
    bool GetZ1Data;
    bool GetZ2Data;
    bool LevelChange;
//    TPanel *MyUseHead[4][8];
    TSpeedButton *MyToolBtn[10];
    TSpeedButton *MyMaintanBtn[10];
    TSpeedButton *MyMainBtn[10];
    void __fastcall ShowVacuumOnOffTime();
    void ShowMainSafeDoorLabel();
    void __fastcall OnHotKey(Messages::TMessage&);                              //jou 2010-09-02 start : UPH顯示/隱藏 改跟NS一樣,使用Hot key開關訊息
    void __fastcall WriteTempData();                                            //Steven 20110613 Start: Corn GPIB
    AnsiString __fastcall TempDataStrings();                                    //Sam 20220408 : Novatek 新增 SET_ALL?
    void __fastcall WriteSetTempStatus();
    void __fastcall WriteSetTestTempStatus();                                   //Ifor 20210623 add: Test Temp Change
    void __fastcall WriteSoakTimeData();
    void __fastcall WriteSetSoakTimeStatus();

    void __fastcall WriteSetSetupFile();                                        //Ifor 20231101 add:FTP Function
    void __fastcall WriteFTPDownSetupFile();                                    //Ifor 20231101 add:FTP Function
    void __fastcall WriteHeadContactCount();
    void Index16Heater(bool bHeaterMode);                                       //Steven 20111207 : 獨立加熱開關
    bool ResetServoOff();                                                       //Steven 20120322 : 按下Reset後,要Servo Off In Out Arm
    AnsiString TemperatureBackup;                                               //Steven 20120910 : 溫度的Edit暫存
    AnsiString TemperatureSoakTimeBackup;                                       //kevin 20160908 soakTime
    HDC PtrDC;                                                                  //Auto Clean
    TCanvas *pCanvas;                                                           //Auto Clean
    int oldLastiTestBinCount;                                                   //kevin 20140305 256bin 16->256 通知gpib
    bool oldbA10_6;
//    int oldLastiTestMode;                                                     //Isaac 20200903 :TTL RS232通訊，移動到public
    bool CheckAuto1OnlyBin1();                                                  //Alick  20160802 add for Auto1 only setting Bin1 Function
    void __fastcall WriteSetTempStatus_SIGURD();                                //KaiChen 20180910 ：Add GPIB SETTEMP
    void __fastcall WriteSetSoakTimeStatus_SIGURD();                            //KaiChen 20180910 ：Add GPIB SETSOAK
    //void NextDayRefresh();                                                    //KaiChen 20210127 ：矽格，跨日處理
    bool __fastcall DoPassword();
    AnsiString SDataPath;                                                       //KenHsieh 20230105 : 新增StateRecord 另存路徑
    bool bManualStateRecord;                                                    //KenHsieh 20230105 : 新增StateRecord 另存路徑
public:     // User declarations
    #pragma option push
    BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(WM_COPYDATA, Messages::TMessage, OnMyCopyMsg);
        MESSAGE_HANDLER(WM_HOTKEY, Messages::TMessage, OnHotKey);               //jou 2010-09-02 start : UPH顯示/隱藏 改跟NS一樣,使用Hot key開關訊息
    END_MESSAGE_MAP(TForm);
    #pragma option pop

    bool    bShowInOutAlarm;
    bool    bChangeTemp;
    bool    bStopChangeTempVisible;
    bool    bFind;                                                              //Find GPIB flag
    bool    bTimer1Begin;
    HWND    HVisionWnd;
    HWND    HESDWnd;
    HWND    HEventLogWnd;
    HWND    HAutoUpdateWnd;
    HWND    HHeartBeat;                                                         //Steven 20211104 : 增加機台的心跳紀錄
    int     oldGpibAddress;
    HWND    HCCDVisionWnd;                                                      //kevin 20110811
    TDateTime dtLightOnTime;
    AnsiString __fastcall WriteSiteMapData(bool bGPIB=true);                    //Sam 20170801 (Steven) 移植超豐 OEE 功能 form HT-7045
    __fastcall TfMain(TComponent* Owner);
    void __fastcall SaveRunMode();                                              //JerryYang 20161027 把Run mode存起來
    void __fastcall ShowShutte2();
    void __fastcall ProcessKeyFlush();
    void __fastcall ProcessTimeUpdate(bool);
    void __fastcall ProcessICHotTime(bool bForceAcess=false);                   //JerryYang 20160909 add bForceAcess,存state record的時候不要被retrun,避免Hot plate缺資料
    void __fastcall ProcessSensorScan();
    void __fastcall UpdateMainScreen(bool);
    void __fastcall UpdateRecordScreen(bool);
    void __fastcall UpdateMotorScreen(bool bRefrash=false);
    bool fShow;
    TEdit *SetupEdit[100];
    void __fastcall UpdateMainOperateMode();
    void __fastcall CleanOutFunction();
    void __fastcall TrayEndFunction();
    void __fastcall UpdateLanguage();
    void __fastcall ScanKey();
    bool fForGetCompomentName;
    void __fastcall ChangeLevelAttr();
    void __fastcall UpdateMotorHomeLed();
    bool __fastcall CheckCanChangeRealDummy();
    void InitialTrayFeedTask(AnsiString Func);
    void __fastcall Clarn_Data(int Tag, AnsiString Msg="");
    bool __fastcall Check_AllMOT_Home();
    void __fastcall DoChangeLevel();
    bool __fastcall Pause(AnsiString Func);                                     //kevin 20141108
    bool __fastcall Reset(AnsiString Func);                                     //kevin 20141108
    void __fastcall ResetForESC(AnsiString Msg);                                //Steven 20201022 : For RFMD Empty Socket Check Funstion.
    void __fastcall ProcessForESC();                                            //Steven 20260121 : For RFMD Empty Socket Check Funstion.
    bool bTriggerESC;
    void __fastcall ResetRecordforPiggyBack(AnsiString asWriteMessage);         //jou 2015-08-21 After conditional error, all units will go to Error bin ,follow by Index check or piggy back check
    void __fastcall JSCC_ResetForShuttleLoseIC();
    void __fastcall SPIL_ResetForIndexDrop();
    bool __fastcall CleanOut(AnsiString Func);                                  //kevin 20141108
    TEdit  *PalCat[8];
    TEdit  *PalCat16[16];
//    long Gali_Command(AnsiString Data);
    void __fastcall MainFormChange();
    bool  WorkFileLoad;
    int   iCallVisionFileLoadTask;
    void __fastcall TestFunction();
    void __fastcall OnMyCopyMsg(Messages::TMessage &msg);
    void __fastcall DrawTestSitePanel();
    void __fastcall ChangeArmSiteView(bool bIintial);                           //ChungHung 20130910 alter for SCK can close site by Index  //Steven 20170125 (Wei) : Mark
    void __fastcall CallVisionInspection(int Command);
    void __fastcall ProcessHVisionConnect();
    void __fastcall WakeupGPIB(AnsiString FuncName);
    void __fastcall WakeupEventLogSaver();                                      //Frank 20140902
    void __fastcall WakeupESD();
    void __fastcall WakeupHeartBeat();                                          //Steven 20211104 : 增加機台的心跳紀錄

    bool __fastcall Start(AnsiString Func);                                     //kevin 20141108
    bool __fastcall Home(AnsiString Func);                                      //kevin 20141108
    void __fastcall CloseGpibProgram(AnsiString Src="");
    void __fastcall SendMessageToGpibProg();
    bool bSetBin;
    void __fastcall UpdateRunInfo();
    void __fastcall AutoOnecycleHomeStartTimeCount();                           //Isaac 20210821 : 全智要求定時onecycle回home,start

    void __fastcall SetMachineTypeVisibleItem_Magazine();
    void __fastcall SetMotorMonitor();
    void __fastcall SetTrayColorMap();
    void __fastcall UpdateSystemMessage();
    void __fastcall UpdateIndexScreenPos();
    void __fastcall RecordTemp();
    bool bScanTesterConnect;
    int iScannerResult;
    void __fastcall ShowNowStatus(TColor cFontColor,AnsiString Capstr);
    void __fastcall MainFormSizeToEpson(bool Change);
    void __fastcall SetInitialData();
    void __fastcall DoShowUserDefFrom();
    void __fastcall DoReadLastData();
    void __fastcall ShowTestHeadComp(bool bRefesh);
    void __fastcall ShowTestHeadComp1(bool bRefesh);
    bool __fastcall CheckSiteMapIsStander();
    void __fastcall ToolLoadICO();
    void __fastcall LookForFile();
    void __fastcall TfMain::SetStartModeData();
    void __fastcall LoadRunModePicture();
    void __fastcall LoadTestModePicture();
    void __fastcall LoadStartModePicture();
    void __fastcall SaveFormPos();
    void __fastcall ShowFormPos();
    void __fastcall LoadImage();
    void __fastcall AdjZLedPos();
    void __fastcall DoMainPadProcess();
    void __fastcall InitDIOStstus(bool bNeedOn);                                //Steven 20110920
    void __fastcall ProceeToolBar();
    bool bRecordSuckRealTimeVacuumOnOffTime;
    int iStringGrid2Col;
    int iStringGrid2Row;
    void __fastcall InitialSuperVisorPassword(int CustomerCode);

    void LightOn();
    void __fastcall IndexHeatMode();
    void __fastcall HotplateHeatMode();
    void __fastcall ShowOLPState(int iState);                                   // 2009.12.02 , Joye
    int __fastcall SetTemp(bool bAsk, double fWorkTemp, double fSoakTime);
    int __fastcall ChangeTesterConnect(int Mode,bool Msg=true, bool bRemote=false);
    int __fastcall ChangeTempMode(int Mode,bool Msg, bool bRefresh=false, bool bGPIB=false, bool bSetTempByDLL=false);  //JerryYang 20181126 add bSetTempByDLL
    int __fastcall ChangeSetUpFile(AnsiString FileName);
    bool __fastcall CanChangeSite(bool bNoIncludeHotplate=false);               //ChungHung 20120713 add OneCycle Can Enable Site
    int __fastcall TimerRecordLoaderDate();                                     //Steven 20101105
    void __fastcall SetLotState(int iState);
    bool bStartKeyPressCheck;                                                   //Steven 20110309 : for 安全門未關按Start時IndexArm會先動作
    void __fastcall TfMain::TemperatureEditDisable();

    void __fastcall ReStartAutoSiteMapping(bool bStart);                        //jou 2011-03-24 start : Auto Site Mapping
    void __fastcall AutoSiteMappignCleanOut(bool bShowAlarm);
//    void __fastcall RunStartModeChange();
    void __fastcall SetMainRunStartMode(int iSetMode);
    void SetOpenBin();

    bool RunTestProgram(bool bNeedTest, bool *bSiteOnOff=NULL);

    TPowerSaving tPSM;   //Dell 20110418
    void  __fastcall EnabledSetupFile(bool bEnabled);

    void __fastcall ShowRTCState(int iState);                                   //----- by dell ccd realtime-------------
    void __fastcall ShowATCState(int iState);                                   //Steven 20120410 : Hontech ATC
    bool bNeedRestartProgram;                                                   //Steven 20110927
    //---------------------------------------
    void __fastcall ShowFormByInitPos();                                        //ChungHung 20110822

    bool SendCCD(int Command, AnsiString Data);                                 //---CCD INTERFACE ----//kevin 20110811
    double EpChangeRecord(double oldData, double NewData);                      //kevin 20111216 EPRECORD
    void bEnableAutoclean();                                                    //kevin 20120518 秀 autoclean button

    enum eMainPage7{emp7TabSheet20=0, emp7TabSheet21=1};
    enum eMainPage1{emp1TabSheet7=0, emp1TabSheet10=1, emp1TabSheet12=2, emp1TabSheet15=3, emp1ts1=4, emp1tsShuttleSensor=5};
    void __fastcall EdSVMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);   //Steven 20120220 : Omron EJ1N溫控器
    void ChangeATCSiteUse();                                                    //Steven 20120523 : ATC
//    eRunStartMode GetRunStartModeNum(AnsiString ModeName);                    //Steven 20120609 : 修改Start Mode切換方式
    void __fastcall ShowOCRState(int iState);                                   //ChungHung 20120830 add OCR Function add
    void AddShuttleMessage(int iWhichSht, AnsiString Msg);                      //Steven 20130614 : 紀錄Shuttle Latch的位置
    void AddAutoCleanMessage(AnsiString Msg);                                   //Steven 20130618 : 紀錄Auto Clean的狀態
    bool __fastcall SetUpFileUseKitNum(AnsiString Filter);                      //ChungHung 20130402 alter for SCK
    bool __fastcall SetUpfileUseProduct(AnsiString Filter);                     //ChungHung 20130402 alter for SCK

    bool __fastcall CanChangeToSocket();                                        //kevin 20130425
    bool __fastcall ChangeToSocket(char *str);                                  //kevin 20130425
    void __fastcall WritePERSITETemperature();                                  //kevin 20130425 //2013.01.11 Q_Q TSMC GPIB COMMAND
    AnsiString __fastcall PERSITETemperatureStrings();                          //Sam 20220408 : Novatek 新增 SET_ALL?
    void __fastcall WriteHandlerID();                                           //kevin 20130425 //2013.01.11 Q_Q TSMC GPIB COMMAND
    void __fastcall WriteArmTemperature( int iType);                            //kevin 20130425 //2013.01.11 Q_Q TSMC GPIB COMMAND
    void __fastcall WriteArmStatus();                                           //kevin 20130425 //2013.01.11 Q_Q TSMC GPIB COMMAND
    AnsiString __fastcall ArmStatusStrings();                                   //Sam 20220408 : Novatek 新增 SET_ALL?
    void __fastcall WriteArmForce();                                            //kevin 20130425 //2013.01.11 Q_Q TSMC GPIB COMMAND
    void __fastcall HandlerInformationRecord();                                 //kevin 20130425 //2013.01.11 Q_Q TSMC GPIB COMMAND

    AnsiString __fastcall WriteBinMap(bool bGPIB=true);                         //ChungHung 20150217 add for SCK request
    void __fastcall WriteSetBinMap(AnsiString BinData);                         //Steven 20230210 : Set Bin Map.
    void __fastcall WriteTestMode();                                            //ChungHung 20150217 add for SCK request
    bool __fastcall CanChangeToHotTemp(int i);                                  //kevin 20130425
    bool __fastcall CanChangeToAmbientTemp();                                   //kevin 20130425
//    void __fastcall WriteSocketData();                                        //kevin 20130425
    void __fastcall SetSocketData();                                            //kevin 20130425
//    void __fastcall WriteTemperatureReady();                                  //kevin 20130425 2012.02.22 , Joye , Add GPIB Command

    void __fastcall WriteStartMode_NS();                                        //ChungHung alter 20130510 Start
    void __fastcall WriteAssign_NS();
    void __fastcall WriteTemp_NS();
    void __fastcall WriteForce_NS();

    int __fastcall RefreshTempData(bool bTransfer=false, int iArm=0, int iSite=0);  //JerryYang 20190812 新增sitemap to 溫控器轉換, 輸入site,輸出Heater位置
    bool bRefresh;                                                              //kevin 20130812  JOBFILE ASE高雄
    void __fastcall CleanYieldCount();                                          //kevin 20131009 清除yield計數
    TStringList *tMonitorIndex;
    TStringList *tTestResult;                                                   //Steven 20140510 : Secs Gem
    TStringList *tSiteOnOff[2];
    TStringList *tESDError;                                                     //Steven 20140521 : ESD
    TStringList *tGPIBMsg;                                                      //Steven 20190116 : GPIB顯示MSG
    TStringList *tBarCodeList;                                                  //Steven 20150713 : for 2D Code
    void InitialBarCodeList();                                                  //Steven 20150713 : for 2D Code
    void __fastcall CheckON_LINE();                                             //kevin 20140407 檢查OP模式強制ON_LINE
    void OverBelowRangeTemp(int iNowChannel, int iNowTemp, int iSetTemp, int iRange);
    void SendMSG_CMD(int CMD);                                                  //Steven 20140922 : Arm2當作指紋測試
    void SendMSG_CMD(int CMD, AnsiString Message);                              //Steven 20190401 : 整合SendMessage
    void SendMSG_CMD_DeviceMapSRQ(int iStatus);                                 //Steven 20191009 : Device Map for Qualcomm
    void SendMSG_TestMode();
    bool CheckSiteMapState();                                                   // AutoSitMap site必須全開   kevin 20150115
    bool CheckSiteMapState(bool bDelete);                                       //delete  AuToSitMap.txt  kevin 20150120

    bool bJobFileProd;                                                          //kevin 20141017 生產中不能任意改工作檔

    void __fastcall AppException(TObject *Sender, Exception *E);                //ChungHung 20141226 add catch exception
    void __fastcall UnloaderInfo(TStringGrid *sgTemp);                          //ChungHung 20150205 add for ATK

    bool bAutoCleanTest;
    bool bCloseExcelflag;
    bool bCloseExcelfinishflag;

    void __fastcall RotateChangePicture();                                      //Frank 20150710 add
    void __fastcall ChangePassword();                                           //wei 20150803 SECSGEM更改PASSWORD

    void WriteNowAllTempData();                                                 //Frank 20150729 add Send Now All Temp Value To Test
    void WriteChkSetup();                                                       //wei 20151125 Add CHKSETUP? Command
    void WriteHandlerTestArmEncoder();                                          //wei 20160122
    void WriteHandlerTestArmEP();                                               //wei 20160122
    void CheckOTDStatus();                                                      //wei 20150810  OTD Log
    bool bCloseByAutoUpdate;
    bool bCloseByAutoUpdate2;
    bool ReadTempData();                                                        //生產中讀取溫度資料 ase  kevin 20150821

    int iOperatorModeCount;                                                     //Ifor 20151027 換至public
    int iOpenA01Count;                                                          //Sam 20220929 : Config A01 自動開啟。
    void __fastcall MachineStatus();                                            //JerryYang 20151109 回覆tester機台狀態
    void SetSiteMapData();                                                      //wei 20151127 GPIB Change Site Map
    bool ChangeToSiteMap(char *str);                                            //wei 20151127 GPIB Change Site Map
    void SetAlarmSetup();                                                       //wei 20151125 Add CHKSETUP? Command
    bool ChangeToAlarmSetup(char *str);                                         //wei 20151127 GPIB Change Site Map
    bool ChangeToAlarmSetup_SG(char *str);
    void RunCheckStart();                                                       //Ifor 20151208 :新增Run Check 副程式
    AnsiString ReadEPData();                                                    //kevin 20160106 讀取EP資料
    bool CheckSiteMapPriority();                                                //AutoSitMap site  順序判斷   kevin 20160125

    bool CreateAndOpenMap();                                                    //JerryYang 20181126 (Steven) : support Epson DLL function
    int  GetBinCount(int iCategNum);
    int  SetTrayBinByDLL(int iTrayNum, LPSTR asCategories, int iFail);
    int  GetTrayBinByDLL(int iTrayNum);
    int  SetSiteMapByDLL(LPSTR cSiteMap, int iNoOfSites);
    int  GetSiteMappingByDLL(LPSTR cSiteMap);
    int  SetTempByDLL(int iTempModeEPSON, double dTempVal);
    int  GetTempSettingByDLL();
    int  GetBinCountByDLL(int iCategNum);
    int  ClearBinCountByDLL();
    int  GetSortCountByDLL(int nTrayNum);
    int  ClearSortCountByDLL();
    int  FTPDownloadByDLL(LPSTR cRecipeName);
    int  GetHandlerStatusByDll();
    double GetAlarmStatusByDll();
    int  RemoteControl(int iMode);
    int  GetBinCountPerSiteByDLL(int iCategNum,int iSiteNum);
    int  GetTempActualByDLL();
    bool SettingsIsWindowOpened();
    int  SetProdModeByDll(int iProdMode);                                       //JerryYang 20220311 : ATP鎖定Critical parameter
    int  GetProdModeByDll();

    void __fastcall GetCZtesterBin();                                           //JerryYang 20160308 回傳各Bin數量
    void __fastcall GetCZSoakTime();                                            //JerryYang 20160308 回傳Soak time
    void __fastcall GetCDHandlerID();
    void __fastcall GetCZJamCode();                                             //JerryYang 20160308 回傳Jam code
    void __fastcall GetCZSiteMap(bool bSendGPIB=true);                          //JerryYang 20160308 回傳Site Map
    void __fastcall GetCZAllMassTemp();                                         //JerryYang 20160308 回傳all mass temp
    void __fastcall GetCZDoubleContactCount();                                  //Isaac 20210706 : add MSG_CMD_DoubleContactCount指令，詢問handler doublecontact次數
    bool CheckARTSetupFile();                                                   //wei 20160115 確認Setup File 是否可以跑ART模式
    bool ATCAmbientTemperCheck();
    void RecordJamRateByTime();                                                 // 2015.11.11 , Joye , Add Jam Rate Record
    bool NETDownloadDataCheck();                                                // 2013.12.03 , Joye , KYEC FTP
    bool ChoiceJobName(AnsiString Data);                                        //kevin 20110317  ASE高雄 要另一執行檔去開啟換TYPE'
    bool CheckInOutArmZHomeSensor();                                            //Frank 20160612 add Check In/Out Arm Home Sensor Function
    void SetNormalOrPrime();                                                    //Steven 20160629
    bool CheckSLKSensor();                                                      //JerryYang 20160712
    void AddTorqueLog(AnsiString Msg);                                          //Steven 20160908 : 整合成Function
    TMyStringList *slMNetLog;                                                   //Steven 20161115 : MNet Log改新版存檔
    TMyStringList *slTTLLog;                                                    //Steven 20161115 : TTL Log改新版存檔
    TMyStringList *slHeaterLog;                                                 //Steven 20161115 : Heater Log改新版存檔
    TMyStringList *slJamAlarmLog;                                               //RogerYang 20170405 (Steven) : Jam Alarm Log
    TMyStringList *slSocketIdProductData;                                       //Sam 20170516    : Socket Id Product Data
    TMyStringList *slTimeData;                                                  //JerryYang 20180515 記錄Time data
    TMyStringList *slInputDataLog;                                              //Sam 20171208 (wei) : PTI Lot End 上報 Input 數量
    TMyStringList *slQtyLog;
    TMyStringList *slAutoSiteMapLog;                                            //Steven 20211209 : 紀錄Auto Site Map動作
    TStringList   *slLowYieldAlarm;
    TMyStringList *slRecordRunState;                                            //Ifor 20230420 add:ASEM要求新增Run Status Log Function
    TMyStringList *slTorqueLog;                                                 //KaiHuang 20201222 : For ASE 高雄 扭力值存Log
    TMyStringList *slTorqueLogNew;
    TMyStringList *slTestLog;                                                   //Steven 20210608 : JSCK OEE Function.
    TMyStringList *slIndexYMaxMinShift;                                         //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值
    TMyStringList *slTorquMUClog;                                               //kevin 20211111 : MCU Comman log
    TMyStringList *slLotInfolog;                                                //Steven 20221225 : add for CyuEan
    TMyStringList *slProdRecordLog;                                             //JerryYang 20230721 : Analog要求production record
    TMyStringList *slTriTempDoorlog;                                            //Steven 20230810 : 三溫開關門log
    TMyStringList *slUploadFile;                                                //Steven 20250716 : 整合上傳的功能
    TMyStringList *slDewPointLog[3];                                            //Steven 20230810 : 三溫露點計log
    AnsiString SVID1190_OSSetup;                                                //Steven 20230505 : 取得OS Tester資訊

    map<AnsiString, AnsiString>::iterator AlarmCodeIter;                        //Steven 20170202 (wei): Fixed for Unknown Alarm Code
    map<AnsiString, AnsiString>AlarmCodeMap;
    map<AnsiString, AnsiString>AlarmUnitMap;                                    //Steven 20231127 : 整理Alarm Unit
    TStringList *UnitNameMap;
    TStringList *AlarmCodeList;
    TStringList *ImpParaCheck;                                                  //Steven 20220311 : GM Test工作檔比對功能
    TStringList *ImpParaCheckRecipe;
    bool ProcessARTMessage();
    void __fastcall BackupSetupFile();                                          //Ifor 20170620 (wei) add Backup Setup File
    bool CheckAutoOnlySetOneBin();                                              //Ifor 20171213 (Steven) : Auto Tray 設定Pass時僅可設定一個Bin

    struct sort                                                                 //KaiChen 20171225 (Steven)：Add Speed Display
    {
        String name;
        int Speed;
    }Speedlist[4], list[4], temp;

    void __fastcall ReadLightScaleTable();
    void __fastcall HanderTcpIp();                                              //Isaac 20170613 (wei) TCP/IP
    void __fastcall HandlerTCPIPResultSendProcess(String sMessage);             //Isaac 20170613 (wei) TCP/IP
    void __fastcall HandlerTeraTResultSendProcess(String sMessage);             //Sam 20230421 : 整理 TCP Log
    void __fastcall TCPIPCommunicationLog(AnsiString Str);                      //Isaac 20170613 (wei) TCP/IP
    void UpdateTaskList();
    void SaveTaskList(AnsiString NewPath);                                      //Steven 20200304 : 儲存紀錄Task+time的方式
    HANDLE hFileMapping;                                                        //JerryYang 20181126 (Steven) : support Epson DLL function
    bool bOpenFileMap;
    void WriteSiteOnOff();
    void AutoSiteOnOff(AnsiString buffer);                                      //JimmyChiu 20250715 : Auto site on/off by GPIB
    void WriteNumOfSites();
    AnsiString GetSamSungTmp(bool bSend=true);                                  //Steven 20191112 : 三星格式
    AnsiString GetSamSungMap(bool bSend=true);
    AnsiString GetSamSungSoakTime(bool bSend=true);
    void ModifyTester(int iMode);                                               //Steven 20191218 : 整合修改LastSet.iTester
    void UploadProdLog();                                                       //JerryYang 20200706 AM 0100 上傳Production log
    void SetSiteOnOff(AnsiString hexStr);
    bool ParseHexToBoolArray(AnsiString hexStr, bool* bArr);
    void HexCharToBits(char hexChar, bool* bArr, int startIndex);
    void __fastcall SetComponentClick(TWinControl *PCtrl);                      //Click Event,Auto Save log
    void __fastcall DeletePtr();
    TStringList *BV;
    TList       *BL;
    void __fastcall SearchComponentParent(TWinControl *PCtrl,TMyProc *PP);      //搜尋物件上的Parent Path//
    void __fastcall SearchForm();

    TQPF_Timer tSoakTimer;
    TQPF_Timer tInitSoakTimer;
    TQPF_Timer tESDAlarmTimer;
    TQPF_Timer tAutoStartTimer;
    TQPF_Timer tAutoStartTimeOutTimer;
    TQPF_Timer tAutoAlignmentForm;
    int iAutoStartTask;
    bool bHasCleanCount;                                                        //Ifor 20191016 : add KYEC清除Yield過程中不需再次登入權限
    int iCanCloseCleanOut;                                                      //Ifor 20220427 add:按下Clean Out未Start可取消
    int iResetCurrent;
    bool bIndexPickErrorNeedShow;
    void __fastcall ShowAOAState(int iState);                                   //Ifor 20230712 add: 新增AOA狀態顯示
    void __fastcall ShowPDDState(int iState);                                   //Frank 20260710 ADD//Eastsun 20260710整合
    bool bDuplicatedIORestartProgram;                                           //Ifor 20231120 add: IO 重複關閉程式
    bool bAMDRs232ConnectError;                                                 //Ifor 20200220 : add AMD Rs232 Connect Error Alarm
    TStringList *UnloadTrayLog;                                                 //Steven 20200409 : production log by unloader tray存檔並上傳FTP
    TStringList *RTMCommand;                                                    //Steven 20200409 : JSCC RTM功能
    void __fastcall GetTTLState();                                              //Isaac 20200903 :TTL RS232通訊
    void __fastcall Send_Command_TTL(AnsiString asStr);                         //Isaac 20200903 :TTL RS232通訊
    int oldLastiTestMode;                                                       //Isaac 20200903 :TTL RS232通訊
    void AddIndexPosLog(AnsiString Msg, bool bSave=false);
    void DebugOneCycleHotPlate(AnsiString sfunc);                               //Sam 20210915 : 增加 OneCycle Hotpalte Debug Log

    int __fastcall FTClick(bool bMan=false);                                    //RogerYang 20260320 : Add for JCET權限輸入
    int __fastcall RTClick(bool bMan=false);                                    //RogerYang 20260320 : Add for JCET權限輸入
    int __fastcall RunModeSelChange();
    void __fastcall RunICModeChange(bool bDefineMode=false, int iMode=2);       //Steven 20220616 : IC Run Mode變更

    void SetSiteMapData_SIGURD();                                               //KaiChen 20181129 ：Add GPIB SETSITEMAP_
    AnsiString GetTestIFSiteMap();                                              //KaiChen 20181129 ：Add GPIB SETSITEMAP_
    void ChkStatus();                                                           //KaiChen 20180910 ：Add GPIB CHKSTATUS?
    void GetBinCategory();                                                      //KaiChen 20180913 ：Add GPIB GETBINCATEGORY?
    void GetSetUpFileName();                                                    //KaiChen 20181022 ：Add GPIB GETSETUPFILENAME?
    void GetHandlerID_Sigurd();                                                 //KaiChen 20200507 ：Add GPIB HANDLERID?
    void __fastcall ChangeRunModeAfterTrayFeed();                               //KaiChen 20180919 ：TrayFeed 後 RunStartMode 切回 Initial
    void SetSetupFileName();                                                    //KaiChen 20190613 ：Add GPIB SGSETUP_
    bool ChangeSetupFileName(char *str);                                        //KaiChen 20190613 ：Add GPIB SGSETUP_
    void PPSELECTLoadFile();                                                    //Richard 20220929 :Add for UTAC 讀檔
    void PPSELECTAskFile();                                                     //Richard 20220929 :Add for UTAC 讀檔 詢問Handler當前檔名
    void SetStartMode();                                                        //KaiChen 20180910 ：Add GPIB SetStartMode_
    bool ChangeHandlerStartMode(char *str);                                     //KaiChen 20180910 ：Add GPIB SetStartMode_
    void CheckList();                                                           //KaiChen 20190613 ：Add GPIB CHECKLIST?
    void SetBinPosChange();                                                     //KaiChen 20190706 ：Add GPIB BINPOS_
    AnsiString BinPosChange(char *str);                                         //KaiChen 20190706 ：Add GPIB BINPOS_
    void GetSGFTPSTATUS();                                                      //Sam 20210329 : Add GPIB SGFTP_STATUS
    void SetSGFTP();                                                            //Sam 20210329 : Add GPIB SGFTP_ SGFTP_ON/SGFTP_OFF
    void SetNONDOUBLEBIN();                                                     //Sam 20210329 : Add GPIB NONDOUBLEBIN_
    void SetBINCOUNT();                                                         //Sam 20210329 : Add GPIB BINCOUNT_
    void SetSGOSBIN();                                                          //Sam 20210406 : Add GPIB SGOSBIN_
    void SetSGCONTFAIL();                                                       //Sam 20210406 : Add GPIB SGOSBIN_
    void SetTesterID();                                                         //Sam 20210617 : Add GPIB SETTESTERID
    void GetTesterID();                                                         //Sam 20210617 : Add GPIB SETTESTERID
    void SetSBinData();                                                         //Steven 20220421 : Amlogic需要收SBIN
    void GetAutoClean();                                                        //Sam 20220408 : Novatek 新增 AUTOCLEAN?
    void GetDUTCHK();                                                           //Steven 20250701 : for DOOSAN TESNA
    void GetFFC();                                                              //Steven 20250701 : for Ampere
    void GetTJFunction();
    void GetPowerFollowing();
    AnsiString AutoCleanStrings();                                              //Sam 20220408 : Novatek 新增 AUTOCLEAN?
    void GetForcePerPinN();                                                     //Sam 20220408 : Novatek 新增 DEVICEFORCEPERPIN?
    AnsiString ForcePerPinNStrings();                                           //Sam 20220408 : Novatek 新增 DEVICEFORCEPERPIN?
    void GetContactHeight();                                                    //Sam 20220408 : Novatek 新增 ARMCONTACTHIGHVALUE?
    AnsiString ContactHeightStrings();                                          //Sam 20220408 : Novatek 新增 ARMCONTACTHIGHVALUE?
    void GetYieldContinusFail();                                                //Sam 20220408 : Novatek 新增 YIELDCONTINUESFAIL?
    AnsiString YieldContinusFailStrings();                                      //Sam 20220408 : Novatek 新增 YIELDCONTINUESFAIL?
    void GetYieldSiteCompare();                                                 //Sam 20220408 : Novatek 新增 YIELDSITEUNBALANCE?
    AnsiString YieldSiteCompareStrings();                                       //Sam 20220408 : Novatek 新增 YIELDSITEUNBALANCE?
    void GetDUTStaus();                                                         //Sam 20220408 : Novatek 新增 DUTSTATUS?
    AnsiString DUTStausStrings();                                               //Sam 20220408 : Novatek 新增 DUTSTATUS?
    void GetUPH();                                                              //Sam 20220408 : Novatek 新增 UPH?
    AnsiString UPHStrings();                                                    //Sam 20220408 : Novatek 新增 UPH?
    void GetIndexCycleTime();                                                   //Sam 20220408 : Novatek 新增 INDEXCYCLETIME?
    AnsiString IndexCycleTimeStrings();                                         //Sam 20220408 : Novatek 新增 INDEXCYCLETIME?
    void GetTempOfs();                                                          //Sam 20220408 : Novatek 新增 GETTEMPOFFSET?
    AnsiString TempOfsStrings();                                                //Sam 20220408 : Novatek 新增 GETTEMPOFFSET?
    void GetTempRange();                                                        //Sam 20220408 : Novatek 新增 GETTEMPERATURETOLERANCE?
    AnsiString TempRangeStrings();                                              //Sam 20220408 : Novatek 新增 GETTEMPERATURETOLERANCE?
    void GetVacuumAir();                                                        //Sam 20220408 : Novatek 新增 VACUUMAIR?
    AnsiString VacuumAirStrings();                                              //Sam 20220408 : Novatek 新增 VACUUMAIR?
    void GetAll();                                                              //Sam 20220408 : Novatek 新增 SET_ALL?
    void GetHandlerVersion();                                                   //Sam 20220408 : Novatek 新增 HANDLERVERSION?
    AnsiString HandlerVersionStrings();                                         //Sam 20220408 : Novatek 新增 HANDLERVERSION?
    void __fastcall DoStateRecord(int iShowAlarm, bool bManual=false);          //Steven 20220716 : 把State record獨立出來, 避免抓圖的時候被Alarm擋住   //KenHsieh 20230116 : 區分手動或自動
    AnsiString GetCheckListPath(bool bIsBackupPath=false);

    void __fastcall ShowFTPState(bool bState);                                  //Isaac 20200730 : FTP在主畫面顯示ON/OFF
    void __fastcall sb_Gali_LogClick();
    cInArmPlacement *cInplace;                                                  //JimmyChiu 20220908 add Pickup Error Placement
    bool bReceivePPSELECT;                                                      //Richard 20220929 :Add for UTAC
    int iFileOkPPSELECT;                                                        //Richard 20220929 :Add for UTAC
    void DisableSiteMappingCheck(bool bSelect, AnsiString Func);                //Steven 20230130 : 加上紀錄
    void GetShuttleMode();                                                      //Sam 20230130 : Add GPIB GETSHUTTLEMODE?
    void SetMaxTest();                                                          //Sam 20230201 : Add GPIB SETMAXTEST_
    void GetMaxTest();                                                          //Sam 20230201 : Add GPIB GETMAXTEST
    void SetMaxInitialTest();                                                   //Sam 20230201 : Add GPIB SETINITIALMAXTEST_
    void GetMaxInitialTest();                                                   //Sam 20230201 : Add GPIB GETINITIALMAXTEST
    void SetAICCD();                                                            //Sam 20231108 : Add GPIB SETAICCD_
    void GetAICCD();                                                            //Sam 20240826 : Add GPIB GETAICCD?
    void SetOSBIN();                                                            //Sam 20250115 : Add GPIB SETOSBIN_
    void GetOSBIN();                                                            //Sam 20250115 : Add GPIB GETOSBIN?
    AnsiString GetSiteState();                                                  //Sam 20230511 : 機台資料變更後須上傳 FTP
    void ChangeStateUploadServer();                                             //Sam 20230511 : 機台資料變更後須上傳 FTP
    void TransformTcGPIBData(AnsiString asStr);
    bool bHandlerResultConnect;
    void __fastcall RecordCommunication(AnsiString aTitle, AnsiString Command);
    void Get2DID_OrderBySites(TMyKitSuck *kit, TStringList *sSourceList);       //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
    void WriteREADYNEXTSHOT();                                                  //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
    AnsiString GetREADYNEXTSHOT();                                              //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
    void WriteNEXT2DID();                                                       //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
    AnsiString GetNEXT2DID();                                                   //Jimmychiu 20231011 : #[SCK_HT9046LS] Request for GPIB command adding for next 2DID information
    bool CheckOLPError();                                                       //Sam 20230921 : Bin 設定錯誤不能啟動
    void __fastcall WriteMultiZoneTemp();                                       //wei 20240617 Multi Zone
    void __fastcall WriteMultiZoneEnable();                                     //wei 20240617 Multi Zone
    void __fastcall ReadWaterValve();
    void __fastcall ReadDynamicPID();

    enum ePalFunc{PalFuncArm1Torque =0,
                  PalFuncArm0Torque =1,
                  PalFuncPMAlarm    =2,
                  PalFuncEPenconder =3,
                  PalFuncSafeDoor   =4,
                  PalFuncOCR        =5,
                  PalFuncATC        =6,
                  PalFuncScanAOI    =7,
                  PalFuncSocketSen  =8,
                  PalFuncRTC        =9,
                  PalFuncAutomation =10,
                  PalFuncFTP        =11,
                  PalFuncAutoClean  =12,
                  PalFuncTrayMap    =13,
                  PalFuncMonitor    =14,
                  PalFuncAutoSkip   =15,
                  PalFuncTriTemp    =16,
                  PalFuncPowerSave  =17,
                  PalFuncSummary    =18,
                  PalFuncCleanCnt   =19,
                  PalFuncAutoFTP    =20,
                  PalFuncRnsFTCT    =21,                                        //RogerYang 20251106 : Add 瑞薩 FTCT
                  PalEnableIdxChk   =22,                                        //RogerYang 20251222 : 丁曉東說無論如何都要顯示(給稽核看)
                  PalFuncTotal
    };

    TPanel *palFunc[PalFuncTotal];
    int __fastcall ShowFunctions();                                             //Steven 20240123 : 顯示功能列表
    bool bShowFunc;
    void __fastcall TfMain::ATC_TJ_SITEMAP();                                   //JerryYang 20231128 : 傳送ATC site map
    bool IsStackHasLess16Bin(int iStack);
    PAT_Function *patFunc;                                                      //Jimmychiu 20241110 : add PAT Class
    void GetSiteMappingForSIGURD(LPSTR cSiteMap);                               //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
    int GetShtModeFlag();                                                       //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
    bool GetSiteOnOffByChannel(int iCh,int iTolRow,int iTolCol);                //Jimmychiu 20241203 : add get site on off for SIGURD_PeiXing
    bool SetSiteOnOffByChannel(int iCh,bool bSwitch);
    uHANA_ART* hanaART;                                                         //JimmyChiu 20241023 : HANA ART Function
    void RunBatchCopyRecipe(AnsiString source_folder);
    void Tri_Temp_Set_Site();                                                   //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    void SET_ESD_Tri_Temp(int iTemperature);                                    //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    void Tri_Temp_ChangeATCSiteUse();                                           //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    int iHasChangeFile;
    bool bHasPin1Error;
    bool IsStackHasBin(int iStack);
    void GetSocketCounter();                                                    //Ifor 20250607 add:Get Socket Counter
    void GetTIMCounter();                                                       //Ifor 20250607 add:Get TIM Counter

    bool bNeedClearFile;
    AnsiString PPID;
    void __fastcall ChangeMainFormWitdh();                                      //RogerYang 20250728 Vtest設開關決定要不要顯示View
    TRENESAS_Server *RENESAS_Server;                                            //Kirin 20191213 只有RENESAS需要開啟
    TQPF_Timer PAT_Rpt_Delay;
    void RunBatchCopyRecipe();
    TMachineTimeManager machineTime;                                            //Jimmychiu 20250916 : 新增機台運作狀態紀錄
    void DoFTRTClick(bool bIsRT, bool bIsMan=false);                            //RogerYang 20260410 : 整合並區分手動按下還是程式按下

    void RunStartByHotKey();
    void SaveDecisionVariables(AnsiString NewPath);
    void BackUpDataToServerByDisk(AnsiString asDirPath,AnsiString sFileName);   //Eastsun 20260520 : 整合
    bool bShowRunModeStatus;                                                    //Frank 20260710 ADD//Eastsun 20260710整合
    bool CheckRunMode();                                                        //Frank 20260710 ADD//Eastsun 20260710整合
};
//---------------------------------------------------------------------------
extern bool bEcho, bUnderTest;
extern PACKAGE TfMain *fMain;
extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");//Steven 20120615 : 改成可用序號(Mode)或名稱(ModeText)進行模式改變
//---------------------------------------------------------------------------
#endif
