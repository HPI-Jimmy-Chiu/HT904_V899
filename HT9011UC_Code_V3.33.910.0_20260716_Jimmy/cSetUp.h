//---------------------------------------------------------------------------
#ifndef cSetUpH
#define cSetUpH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <jpeg.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfSetup : public TForm
{
__published:    // IDE-managed Components
    TPanel *palLeft;
    TRadioButton *RadioButton6;
    TPanel *Panel3;
    TGroupBox *gbShuttleMode;
    TRadioButton *rgShtModeNormal;
    TRadioButton *rgShtModeOneSide;
    TGroupBox *grpShuttleSelect;
    TRadioButton *rgUseSht1;
    TRadioButton *rgUseSht2;
    TGroupBox *grpIndexOption;
    TCheckBox *chkOffCenterkit;
    TCheckBox *chkNS7000CS;
    TCheckBox *chkRotateShuttle;
    TCheckBox *cbNS8000H;
    TCheckBox *chkOctal80;
    TCheckBox *cbOctal16Site;
    TCheckBox *cbOctal12Site;
    TCheckBox *cb16DirectHeater;
    TCheckBox *cb12Site10DirectHeater;
    TRadioGroup *rgUseSuckMode;
    TRadioGroup *rgSelectSearchLast;
    TGroupBox *grpHandlingMode;
    TImage *Image1;
    TLabel *lblXPitch;
    TLabel *lblXPMM;
    TLabel *lblYPitch;
    TLabel *labYOffset;
    TScrollBar *ScrollBar1;
    TPanel *Panel1;
    TEdit *XPitch;
    TEdit *YPitch;
    TRadioGroup *rgInOutArmYPitch;
    TEdit *edYOffset;
    TPanel *Panel2;
    TSpeedButton *sbUpdate;
    TSpeedButton *sbtExit;
    TCheckBox *cbSquareOctalLayout;
    TCheckBox *cb2CableLayoutKit;
    TCheckBox *cbHotechLayoutKit2x2;
    TCheckBox *cb1CableLayoutKit;
    TCheckBox *cbQualSite2X2Shift;
    TPanel *paQualSite2X2Shift;
    TLabel *Label1;
    TEdit *XShiftPitch;
    TLabel *Label2;
    TCheckBox *cbSingleSiteSingleHeater;
    TButton *btAutoShuttlePitch;
    TCheckBox *cbNSKitPress;
    TGroupBox *grpFunction;
    TGroupBox *gbArm1PickArm2Test;
    TCheckBox *Arm1PickArm2Test;
    TCheckBox *cbCheckArm2Vacuum;
    TGroupBox *gbOcr;
    TCheckBox *cbOcrFunction;
    TEdit *edOcrText;
    TGroupBox *gbSocketClamp;
    TCheckBox *cbUseSLKClamp;
    TRadioGroup *rgseparabilityTest;
    TGroupBox *grpSocketSensor;
    TCheckBox *chkCheckFloating;
    TCheckBox *cbArm1UseHeat;
    TCheckBox *cbRTCICResidueCheck;
    TCheckBox *cbRTC20OverFlowCheck;
    TCheckBox *cbRTC20GiveWayCheck;
    TCheckBox *cbRTC20CheckFunction;
    TCheckBox *chk2x2Use16siteSLK;
    TCheckBox *cb16change12DirectHeater;
    TScrollBox *scrlbxSocketSensor;
    TPanel *Panel4;
    TCheckBox *cbdisibleinitialcheck;
    TCheckBox *cbSocketSensor;
    TComboBox *CoSocketCombo;
    TRadioGroup *rgYOffset;
    TRadioGroup *rgYPitchOffsetMode;
        TCheckBox *cbIndSLK;
    TCheckBox *cb16change8DirectHeater;
    TCheckBox *cbI21;
    TPanel *Panel5;
    TGroupBox *grpSiteMap;
    TLabel *labRowA;
    TLabel *labRowB;
    TLabel *labColA;
    TLabel *labColB;
    TLabel *labColC;
    TLabel *labColD;
    TLabel *labColE;
    TLabel *labColF;
    TLabel *labColG;
    TLabel *labColH;
    TSpeedButton *btnLUpToRDownN;
    TSpeedButton *btnLUpToRDownZ;
    TSpeedButton *btnLDownToRUpZ;
    TSpeedButton *btnRUpToLDownZ;
    TSpeedButton *btnRDownToLUpZ;
    TSpeedButton *btnRUpToLDownN;
    TLabel *labRowC;
    TLabel *labRowD;
    TComboBox *cbAa;
    TComboBox *cbAb;
    TComboBox *cbAc;
    TComboBox *cbAd;
    TComboBox *cbAe;
    TComboBox *cbAf;
    TComboBox *cbAg;
    TComboBox *cbAh;
    TComboBox *cbBa;
    TComboBox *cbBb;
    TComboBox *cbBc;
    TComboBox *cbBd;
    TComboBox *cbBe;
    TComboBox *cbBf;
    TComboBox *cbBg;
    TComboBox *cbBh;
    TComboBox *cbCa;
    TComboBox *cbCb;
    TComboBox *cbCc;
    TComboBox *cbCd;
    TComboBox *cbCe;
    TComboBox *cbCf;
    TComboBox *cbCg;
    TComboBox *cbCh;
    TComboBox *cbDa;
    TComboBox *cbDb;
    TComboBox *cbDc;
    TComboBox *cbDd;
    TComboBox *cbDe;
    TComboBox *cbDf;
    TComboBox *cbDg;
    TComboBox *cbDh;
    TCheckBox *cbUseRotateForHT7000HPKit;
    TCheckBox *cb1x2Use1x4siteSLK;
    TPanel *Panel6;
    TPanel *Panel7;
    TGroupBox *grpUseXCenterPitch;
    TLabel *Label3;
    TCheckBox *chkUseXCenterPitch;
    TEdit *edtXCenterPitch;
    TCheckBox *cbF18;
    TGroupBox *gbPreciser;
    TLabel *Label4;
    TLabel *Label5;
    TCheckBox *cbEnablePreciser;
    TEdit *edPreciserXPitch;
    TEdit *edPreciserYPitch;
    TCheckBox *cbEnabledPreciserRT;
    TCheckBox *cbUseTesterDry;
    TPanel *Panel8;
    TGroupBox *grpAutoSiteMap;
    TCheckBox *chkSiteMapOpenSite;
    TCheckBox *chkSiteMapOneCycle;
    TGroupBox *gbOutUseBackRow;
    TCheckBox *cbOutUseBackRow;
    TGroupBox *gbInUseBackRow;
    TCheckBox *cbInUseBackRow;
    TGroupBox *gbRTC;
    TLabel *lbShowMessage;
    TCheckBox *cbEnableRealTimeCCD;
    TCheckBox *cbEnableSocketFloat;
    TEdit *edtRTCFileName;
    TGroupBox *grpHeadCondition;
    TCheckBox *cbO12_LifeTimeCount;
    TCheckBox *cbO13_LifeTimeCount;
    TCheckBox *cbO14_LifeTimeCount;
    TCheckBox *cbSingleUseOtherSuck;
    TCheckBox *cb6CableLayoutKit;
    TRadioButton *rbTemp;
    TPageControl *pgcASE;
    TTabSheet *tsCheckTorque;
    TTabSheet *tsTrayMap;
    TLabel *lblD01_1;
    TLabel *lblD01_2;
    TLabel *lblGetValueDelayTime;
    TCheckBox *chkReadTorque;
    TEdit *edDelayTime;
    TEdit *edOverRange;
    TLabel *Label32;
    TLabel *Label42;
    TLabel *Label43;
    TEdit *edAuto1Count;
    TEdit *edAuto2Count;
    TEdit *edAuto3Count;
    TCheckBox *bUseTrayMap;
    TCheckBox *bUseLoadCCD;
    TCheckBox *bDoubleUnloadTray;
    TEdit *edtGetValueDelayTime;
    TCheckBox *cbEnableStepAsideMode;
    TPanel *palVisibleIndex;
    TCheckBox *cbAutoCleanAlso;
    TCheckBox *chk12SiteUse2x8SLK;
    TCheckBox *cbArm1OnlyPlaceArm2TestAndSuck;
    TCheckBox *cbSingleInArmUseOtherSuck;
    TCheckBox *cbPreventDropfunction;                                            //Eastsun 20260710整合
    TCheckBox *cbUse1x3siteSLK;
    TCheckBox *cbEnablePreciserHotPlate;
    void __fastcall ScrollBar1Change(TObject *Sender);
    void __fastcall rgShtModeNormalClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall XPitchKeyPress(TObject *Sender, char &Key);
    void __fastcall XPitchMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall rgUseSuckModeClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall btnLUpToRDownNClick(TObject *Sender);
    void __fastcall cbEnableRealTimeCCDClick(TObject *Sender);
    void __fastcall cbAaChange(TObject *Sender);
    void __fastcall cbAaDropDown(TObject *Sender);
    void __fastcall RadioButton1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall rgShtModeNormalMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Arm1PickArm2TestClick(TObject *Sender);
    void __fastcall cbOctal12SiteClick(TObject *Sender);
    void __fastcall chkOffCenterkitClick(TObject *Sender);
    void __fastcall cbQualSite2X2ShiftClick(TObject *Sender);
    void __fastcall edOcrTextMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbUseSLKClampClick(TObject *Sender);
    void __fastcall btAutoShuttlePitchClick(TObject *Sender);
    void __fastcall XShiftPitchMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall CoSocketComboChange(TObject *Sender);
    void __fastcall rgSensor1Click(TObject *Sender);
    void __fastcall rgYPitchOffsetModeClick(TObject *Sender);
    void __fastcall cbI21Click(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall edOverRangeClick(TObject *Sender);
    void __fastcall edDelayTimeClick(TObject *Sender);
    void __fastcall edAuto1CountClick(TObject *Sender);
    void __fastcall edtGetValueDelayTimeClick(TObject *Sender);
private:    // User declarations
    bool __fastcall DoPassword();
    bool __fastcall DoPasswordCheck();                                          //JerryYang 20160425 修改site map 需要密碼
    bool bSavePressed;
    int iTestModeOcr;                                                           //Steven 20120716 : OCR
    bool bOCRNeedPassword;                                                      //ChungHung 20130118 add OCR - 關閉要密碼
    int SitCH;
    bool bShuttleMode ;
public:     // User declarations
    __fastcall TfSetup(TComponent* Owner);
    void __fastcall CompChange(int iMode);
    bool __fastcall CHSetError();
    void __fastcall ReadFile();
    bool fShow;
    void __fastcall DoIniDataToForm();
    void __fastcall ReadUseSuckModeFile();
    bool bFirstTime;
    bool bNeedPassword;
    TStringList *tSiteMap;                                                      //Steven 20140510 : Secs Gem
    void __fastcall SaveSetupFile(AnsiString szDir);                            //kevin 20140105 存檔
    bool CheckShuttlePitch();                                                   //wei 20160914 Auto Shuttle Sensor
    int GetTestMode(AnsiString sTestMode);                                      //Steven 20250321 : 整合TestMode轉換
    void CheckSTMMode();
    typedef struct                                                              //Jimmychiu 20241203 : Disable opening all sites when changing shuttle mode.
    {
        bool bNormal;
        bool bOneSide;
        bool bUsedSht1;
        bool bUsedSht2;
        void Clear()
        {
            bNormal=false;
            bOneSide=false;
            bUsedSht1=false;
            bUsedSht2=false;
        }
    }SetUpShuttleMode;
    SetUpShuttleMode shtMode;
    void SetShtMode(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2);
    bool VertifyShtModeisDiff(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2);

    bool bSiteMapHasChange;                                                     //Steven 20250102 : 紀錄test mode與site map是否有變更
    int  iTestMode;
};
//---------------------------------------------------------------------------
extern PACKAGE TfSetup *fSetup;
//---------------------------------------------------------------------------
#endif
