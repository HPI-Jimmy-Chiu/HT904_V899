//---------------------------------------------------------------------------

#ifndef uCleaningH
#define uCleaningH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include "HTray.h"
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfCleaning : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *grpTrayData;
    TPanel *pnlLeft;
    TGroupBox *grpCleanPara;
    TLabel *lblEnableFunction;
    TRadioGroup *rgAutoCleanOnOff;
    TGroupBox *grpCleanMode;
    TLabel *lblContactCnt;
    TEdit *edIntervalContact;
    TGroupBox *grpCleanDevice;
    TLabel *Label279;
    TLabel *Label288;
    TLabel *Label289;
    TLabel *Label290;
    TButton *btnResetCleanCount;
    TRadioGroup *rgKitPosition;
    TEdit *edDevicePices;
    TUpDown *udDeviceCT;
    TEdit *edAlarmCount;
    TGroupBox *grpSpeed;
    TLabel *lblInArm;
    TLabel *lblShuttle;
    TLabel *lblIndex;
    TLabel *lblInArmZ;
    TEdit *OutArmSpeed;
    TEdit *ShuttleSpeed;
    TEdit *IndexArmSpeed;
    TEdit *edtInArmZSpeed;
    TPanel *pnlCenter;
    TGroupBox *grpContactPara;
    TGroupBox *grpContactForce;
    TLabel *lblArmTotalForceKg;
    TLabel *lblForcePerPinN;
    TLabel *lblForcePerPinG;
    TLabel *lblArmTotalForceN;
    TLabel *lblPinCount;
    TEdit *edPinSingleN;
    TEdit *edPinSingleGf;
    TEdit *edPinsCount;
    TGroupBox *grpHeight;
    TLabel *lblIndexToSocketShiftHigh;
    TLabel *lblIndexToSocketOffset;
    TEdit *edACContactShiftHeight;
    TEdit *edACContactCleanHeight;
    TCheckBox *chkAutoCleanMode1;
    TCheckBox *chkAutoCleanMode2;
    TCheckBox *chkAutoCleanMode3;
    TCheckBox *chkAutoCleanMode4;
    TCheckBox *chkAutoCleanMode5;
    TCheckBox *chkAutoCleanMode6;
    TLabel *Label21;
    TRadioGroup *rgShuttleCheck;
    TEdit *edCleaningCount;
    TGroupBox *grpBufferKitLT;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label33;
    TEdit *edtBufferKitLTX;
    TEdit *edtBufferKitLTY;
    TGroupBox *grpBufferKitRT;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TEdit *edtBufferKitRTX;
    TEdit *edtBufferKitRTY;
    TGroupBox *grpBufferKitLD;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TEdit *edtBufferKitLDX;
    TEdit *edtBufferKitLDY;
    TGroupBox *grpBufferKitRD;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TEdit *edtBufferKitRDX;
    TEdit *edtBufferKitRDY;
    TGroupBox *gbYieldAlarm;
    TLabel *Label44;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TEdit *edtFailAlarmSiteYieldDifferent;
    TCheckBox *cbFailAlarmSiteYieldDifferent;
    TEdit *edtFailAlarmSiteYield;
    TCheckBox *cbFailAlarmLowYield;
    TEdit *edtLowYieldLimit;
    TEdit *edtLowYieldCount;
    TGroupBox *gbConsFailureSocket;
    TLabel *Label50;
    TLabel *Label51;
    TEdit *edtConseFailureCountBySocket_Retest;
    TEdit *edtConseFailureCountBySocket_Normal;
    TGroupBox *gbConsFailureHead;
    TLabel *Label48;
    TLabel *Label49;
    TEdit *edtConseFailureCountByHead_Retest;
    TEdit *edtConseFailureCountByHead_Normal;
    TCheckBox *cbConseFailureBySocket_Normal;
    TCheckBox *cbConseFailureByHead_Normal;
    TCheckBox *cbConseFailureBySocket_Retest;
    TCheckBox *cbConseFailureByHead_Retest;
    TPageControl *pcCleanYield;
    TTabSheet *tsOffset;
    TTabSheet *tsYield;
    TTabSheet *tsFailure;
    TSpeedButton *sbTrayAssign;
    TPanel *pnlNotify;
    TLabel *lblIndexToShtPickOffset;
    TEdit *edIndexPickOffset;
    TPanel *pnlInArmOffset;
    TLabel *lblInArmToSht1PickOffset;
    TLabel *lblInArmToSht1PlaceOffset;
    TLabel *lblInArmToSht1XOffset;
    TLabel *lblInArmToSht1YOffset;
    TLabel *lblInArmToSht2PickOffset;
    TLabel *lblInArmToSht2PlaceOffset;
    TLabel *lblInArmToSht2XOffset;
    TLabel *lblInArmToSht2YOffset;
    TLabel *lblInArmToShtPitchOffset;
    TEdit *edShuttle1PickOffset;
    TEdit *edtShuttle1PlaceOffset;
    TEdit *edtShuttle1XOffset;
    TEdit *edtShuttle1YOffset;
    TEdit *edShuttle2PickOffset;
    TEdit *edtShuttle2PlaceOffset;
    TEdit *edtShuttle2XOffset;
    TEdit *edtShuttle2YOffset;
    TEdit *edtShuttlePitch;
    TGroupBox *gbCleanKitOffset;
    TLabel *lblInArmKitXOffset;
    TLabel *lblInArmKitYOffset;
    TLabel *lblInArmKitPickOffset;
    TLabel *lblInArmKitPlaceOffset;
    TLabel *lblInArmKitXPitchOffset;
    TEdit *edtHotplateXOffset;
    TEdit *edtHotplateYOffset;
    TEdit *edtHotplatePlaceOffset;
    TEdit *edtHotplatePickOffset;
    TEdit *edtHotplatePitchOffset;
    TPanel *pnlCleanPadAlign;
    TTMyTray *tmyAutoClean;
    TComboBox *cbbSelectTray;
    TPanel *palBtn;
    TSpeedButton *sbCleanSave;
    TSpeedButton *sbCleanExit;
    TPanel *pnlCleanArm;
    TCheckBox *chkCleanOtherArm;
    TCheckBox *chkbAutoClean_UseTray;
    TRadioGroup *rgAutoCleanSelectArm;
    TPanel *pnlContactMode;
    TLabel *lblContactMode;
    TLabel *lblContactTime;
    TLabel *lblCleanCount;
    TLabel *lblCleanCountTimes;
    TLabel *lblContactTimeSec;
    TEdit *edContactTime;
    TEdit *edACContactCount;
    TComboBox *ContactMode;
    TLabel *lblIndexToShtPlaceOffset;
    TEdit *edIndexReleaseOffset;
    TLabel *lbl2;
    TImage *ImgCleanUnit;
    TEdit *edCleanPadDeviation;
    TLabel *labUnit_Thickness;
    TEdit *edDropOffset1;
    TLabel *lblDropOffset;
    TEdit *edAutoCleanAirForce_Kg;
    TEdit *edAutoCleanAirForce_N;
    TPanel *pnlKitSetting;
    TPanel *Panel7;
    TEdit *PageTypeName;
    TRadioGroup *rgCleanKitType;
    TPageControl *pgCleanType;
    TTabSheet *tsKit;
    TImage *Image3;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TEdit *XST1;
    TEdit *XPitch1;
    TEdit *XCT1;
    TEdit *YST1;
    TEdit *YPitch1;
    TEdit *YCT1;
    TCheckBox *chkUseNSKit;
    TCheckBox *chkE43;
    TButton *btFocusOnly;
    TTabSheet *tabTray;
    TImage *Image1;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TLabel *Label42;
    TLabel *Label43;
    TEdit *XPitch2;
    TEdit *XST2;
    TEdit *XCT2;
    TEdit *YST2;
    TEdit *YPitch2;
    TEdit *YCT2;
    TButton *btInclude;
    TTabSheet *tsSmart;
    TLabel *lbl2edAutoCleanParm;
    TLabel *lblContactMode_Smart;
    TComboBox *cbbACSmart_ContactMode;
    TLabel *lblDropOffset1_Smart1;
    TEdit *edtACSmart_DropOffset1;
    TLabel *lblDropOffset1_Smart2;
    TLabel *lblContactTime_Smart;
    TEdit *edtACSmart_ContactTime;
    TLabel *lblACContactCount_Smart;
    TEdit *edtACSmart_ACContactCount;
    TButton *btnResetInterval;
    TEdit *edAdaptiveIntervalAdj;
    TLabel *lblAdaptiveInterval;
    TEdit *edAdaptiveIntervalMin;
    TEdit *edAdaptiveIntervalMax;
    TGroupBox *grpDieForce;
    TEdit *edtAutoCleanDieForce;
    TCheckBox *chkACSmart;
    TEdit *edACSmartCTF;
    TEdit *edtACSmart;
    TComboBox *cbbCleanPadCount;
    TTabSheet *tsTimeCT;
    TCheckBox *chkTimeCT;
    TEdit *edTimeCT;
    TLabel *lblSpeed;
    TLabel *lblVacuum;
    TLabel *lblAirOn;
    TLabel *lblDelayTime;
    TEdit *edtIndexVacuum;
    TEdit *edtInArmVacuum;
    TEdit *edtIndexAirOn;
    TEdit *edtInArmAirOn;
    TButton *btnStartAutoClean;
    TLabeledEdit *edACInitalContactCount;
    TEdit *edIndexArmAutoCleanCnt;
    TLabel *lbledIndexArmAutoCleanCnt;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edPinSingleGfClick(TObject *Sender);
    void __fastcall edPinSingleNClick(TObject *Sender);
    void __fastcall edPinsCountClick(TObject *Sender);
    void __fastcall sbCleanSaveClick(TObject *Sender);
    void __fastcall sbCleanExitClick(TObject *Sender);
    void __fastcall edACContactShiftHeightClick(TObject *Sender);
    void __fastcall XST1Click(TObject *Sender);
    void __fastcall XST1KeyPress(TObject *Sender, char &Key);
    void __fastcall XCT1Click(TObject *Sender);
    void __fastcall XCT1KeyPress(TObject *Sender, char &Key);
    void __fastcall rgKitPositionClick(TObject *Sender);
    void __fastcall YCT1Change(TObject *Sender);
    void __fastcall udDeviceCTClick(TObject *Sender, TUDBtnType Button);
    void __fastcall edCleanCountClick(TObject *Sender);                         //Ifor 20171024 : add 可更改Auto Clean 計數
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnResetCleanCountClick(TObject *Sender);
    void __fastcall XCT1Change(TObject *Sender);
    void __fastcall rgCleanKitTypeClick(TObject *Sender);
    void __fastcall udDeviceCTChangingEx(TObject *Sender,
          bool &AllowChange, short NewValue, TUpDownDirection Direction);
    void __fastcall btIncludeClick(TObject *Sender);
    void __fastcall edtLowYieldLimitKeyPress(TObject *Sender, char &Key);
    void __fastcall edtLowYieldLimitClick(TObject *Sender);
    void __fastcall edtLowYieldCountClick(TObject *Sender);

    void __fastcall rgAutoCleanOnOffClick(TObject *Sender);
    void __fastcall chkAutoCleanMode6MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sbTrayAssignClick(TObject *Sender);
    void __fastcall XPitch2Change(TObject *Sender);
        void __fastcall cbbSelectTrayChange(TObject *Sender);
    void __fastcall edtHotplatePickOffsetClick(TObject *Sender);

    void __fastcall edDevicePicesClick(TObject *Sender);
    void __fastcall edDropOffset1Click(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall edtBufferKitLTXClick(TObject *Sender);
    void __fastcall edAutoCleanAirForce_KgClick(TObject *Sender);
    void __fastcall edAutoCleanAirForce_NClick(TObject *Sender);
    void __fastcall edtACSmartClick(TObject *Sender);
    void __fastcall edACContactCleanHeightClick(TObject *Sender);
    void __fastcall btnResetIntervalClick(TObject *Sender);
    void __fastcall edAdaptiveIntervalMaxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edAdaptiveIntervalMinMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edAdaptiveIntervalAdjMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edACSmartCTFMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtACSmart_ContactTimeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtACSmartMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbbCleanPadCountChange(TObject *Sender);
    void __fastcall edTimeCTClick(TObject *Sender);
    void __fastcall edtIndexVacuumClick(TObject *Sender);
    void __fastcall btnStartAutoCleanClick(TObject *Sender);
    void __fastcall edACInitalContactCountClick(TObject *Sender);
private:    // User declarations
    void __fastcall ShowTranGfToN();
    void __fastcall LoadImage();
    void SetArmCaption();
    void __fastcall DrawAutoClean();
    //void __fastcall SetDeviceMaxMin();
    int __fastcall SetDeviceMaxMin();                                           //kevin 20150521 change int
public:     // User declarations
    __fastcall TfCleaning(TComponent* Owner);
    bool bResetCleanCount;
    void __fastcall LoadAutoCleanData();
    void __fastcall SaveAutoCleanData();
    void __fastcall ShowCleanContactForce();
    void __fastcall ShowTranNToGf();
    int iPosTemp;
    void ChangeEditToHPMode();                                                  //Steven20160628 : 整合Auto Clean畫面顯示
    void __fastcall DoSetRPDefault(TWinControl *PCtrl, int iSection);
    void __fastcall DoReplyDefaultToForm(TWinControl *PCtrl, int iSection);
    void __fastcall SearchRecipeParameter(TWinControl *PCtrl, int iSection);
    int GetMinCleanPadCount();                                                  //Steven 20221006 : 計算最少需要的Clean Pad數量
    bool CleanPadCountCanSupport2Arm();                                         //Steven 20221006 : 修正雙arm auto clean只有一半的clean pad
    bool b1x2SiteAbClosePutDummy;
    bool fShow;
    void SetDeviceInTray(int iXItem, int iYItem, int iDeviceNum, int iMode);    //Jimmychiu 20221027 優化Cleanpad配置方式
    void SetCleanCellValue(int iXpos, int iYpos, int iType, int iMode);         //Jimmychiu 20221027 統一Cleanpad配置公式  iMode:0=SetCellColorIndex    1=SetTraySingleData
    //enum
    enum eSetCleanPadMode
    {
        eUcleanUsed=0,
        eAutoCleanUsed=1  ,
    };
//    bool bUse8Picker;
    bool __fastcall CheckSmartAutoClean(AnsiString aAlarmCode, AnsiString ErrPart); //Sam 20250916 : Alarm後需要清除資料才能Start                 //Sam 20230111 : Smart Auto Clean
    void __fastcall ResetSmartAutoClean();                                      //Sam 20230111 : Smart Auto Clean
    bool CheckSmartAutoCleanCanStart();                                         //Sam 20250916 : Alarm後需要清除資料才能Start
    void ChangeACSmartInterval(int iChangeMode, AnsiString sMsg);               //Sam 20240726 : AI Clean
    bool b12SiteRun2x4;
    int iDeviceCount;
};
//---------------------------------------------------------------------------
extern PACKAGE TfCleaning *fCleaning;
//---------------------------------------------------------------------------
#endif
