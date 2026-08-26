//---------------------------------------------------------------------------

#ifndef uYieldMonitoringH
#define uYieldMonitoringH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "HTray.h"
#include <Forms.hpp>
#include "HTray256.h"
//---------------------------------------------------------------------------
class TfYieldMonitoring : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlBottom;
    TButton *btnApply;
    TButton *btnOk;
    TPanel *pnlTop;
    TPageControl *pgcMode;
    TTabSheet *tsNormal;
    TTabSheet *tsReTest;
        TTabSheet *tsAutoRetest;
    TTabSheet *tsAutoRetest1;
    TGroupBox *gbA10;
    TLabel *lblARTLimit;
    TEdit *edAutoRetestLimitFile;
    TLabel *lblAutoLeastRetestLimitFile;
    TEdit *edAutoLeastRetestLimitFile;
    TCheckBox *ckUseLeastRetestTimes;
    TPanel *pnlARTFailYiel;
    TLabel *lblFailYieldRate_ARTFile;
    TEdit *edFailYieldRate_ARTFile;
    TTimer *Timer1;
    TPanel *palART_FT;
    TPanel *palART_RT;
    TRadioGroup *rgRT_ART;
    TEdit *edFailYieldRate_ARTRTFile;
    TEdit *edFailYieldMinRate_ARTRTFile;
    TEdit *edFailYieldMaxRate_ARTRTFile;
    TComboBox *cbUseRTFailYieldModel;
    TCheckBox *ckUseFailNoDistinction;
    TTabSheet *tsAlarm4;
    TPageControl *pgcBySiteByBinPercentCompare_FT;
    TTabSheet *tsBinAlarm1_FT;
    TLabel *labByBinFailureCatCT_FT;
    TLabel *labByBinFailure_FT;
    TEdit *edByBinFailureCatCT_FT;
    TTabSheet *tsSpecBinBySiteCompare_FT;
    TLabel *labByBinSiteGapCatCT_FT;
    TLabel *labByBinSiteGapCat_FT;
    TEdit *edByBinSiteGapCatCT_FT;
    TTabSheet *tsSpecBinByArmPerSiteCompare_FT;
    TLabel *labByArmSiteGapCatCT_FT;
    TLabel *labByArmSiteGapCat_FT;
    TEdit *edByArmSiteGapCatCT_FT;
    TPageControl *pgcBySiteByBinPercentCompare_RT;
    TTabSheet *tsBinAlarm1_RT;
    TLabel *labByBinFailureCatCT_RT;
    TLabel *labByBinFailure_RT;
    TEdit *edByBinFailureCatCT_RT;
    TTabSheet *tsSpecBinBySiteCompare_RT;
    TLabel *labByBinSiteGapCatCT_RT;
    TLabel *labByBinSiteGapCat_RT;
    TEdit *edByBinSiteGapCatCT_RT;
    TTabSheet *tsSpecBinByArmPerSiteCompare_RT;
    TLabel *labByArmSiteGapCatCT_RT;
    TLabel *labByArmSiteGapCat_RT;
    TEdit *edByArmSiteGapCatCT_RT;
    TPanel *pnlFT_Left;
    TGroupBox *gbPiggyBack;
    TRadioGroup *rgPiggyBack_FT;
    TPanel *palContinuousPass_FT;
    TLabel *labContinuousPass_FT;
    TLabel *labContinuousPass1_FT;
    TCheckBox *cbContinuousPass_FT;
    TComboBox *cobContinuousPass_FT;
    TEdit *edContinuousPass_FT;
    TPanel *palContinuPassSkt_FT;
    TLabel *labContinuPassSkt_FT;
    TLabel *labContinuPassSkt1_FT;
    TCheckBox *cbContinuPassSkt_FT;
    TEdit *edContinuPassSkt_FT;
    TPanel *palContinuousLoader_FT;
    TLabel *labContinuousLoader_FT;
    TLabel *labContinuousLoader1_FT;
    TCheckBox *cbContinuousLoad_FT;
    TEdit *edContinuousLoad_FT;
    TPanel *palContactCount_FT;
    TLabel *labContactCountFT;
    TLabel *labContactCount1FT;
    TCheckBox *cbContactCountFT;
    TEdit *edContactCountFT;
    TGroupBox *gbYieldAlarm;
    TPanel *pnlFT_Center;
    TGroupBox *gbLoadcell;
    TCheckBox *cbLoadCellMeasure;
    TEdit *edLoadCellMeasure;
    TGroupBox *gbAlarm;
    TGroupBox *gbContsFailBySocket_FT;
    TLabel *labContsFailBySocket_FT;
    TRadioButton *rbContsFailBySocket_FTOn;
    TRadioButton *rbContsFailBySocket_FTOff;
    TEdit *edContsFailSocketAlarmCT_FT;
    TGroupBox *gbContsFailByHead_FT;
    TLabel *labContsFailByHead_FT;
    TRadioButton *rbContsFailByHead_FTOn;
    TRadioButton *rbContsFailByHead_FTOff;
    TEdit *edContsFailHeadAlarmCT_FT;
    TGroupBox *gbLowYieldAutoClean;
    TLabel *lblAutoCleanLowYieldLimit;
    TLabel *lblAutoCleanLowYieldCount;
    TLabel *lblCloseSite;
    TEdit *edAutoCleanLowYieldLimit;
    TEdit *edAutoCleanLowYieldCount;
    TRadioGroup *rgAutoCleanYieldOnOff;
    TEdit *eCloseSite;
    TTabSheet *tsAutoSiteOff;
    TGroupBox *gbAutoSiteOff;
    TLabel *labSiteYieldCmp1SiteCnt_FT;
    TEdit *edtSiteYieldCmp1SiteCnt_FT;
    TRadioGroup *rgCloseSiteOnHPAction;
    TPanel *pnlAutoSiteOff_Right;
    TGroupBox *grpAutoSiteOffWhenRT;
    TLabel *lblAutoSiteOffWhenRT;
    TRadioGroup *rgAutoSiteOffWhenRT;
    TEdit *edtAutoSiteOffWhenRT;
    TTabSheet *tsYield;
    TGroupBox *gbOtherYieldAlarmFT;
    TPanel *palIntervalLowYeildBySite_FT;
    TLabel *lblIntervalLowYieldBySite_FTCap;
    TLabel *lblIntervalLowYieldBySite_FT;
    TCheckBox *cbIntervalLowYieldBySite_FT;
    TEdit *edIntervalLowYieldBySite_FT;
    TEdit *edIntervalLowYieldBySiteIg_FT;
    TPanel *palIntervalLowYeildByTotal_FT;
    TLabel *lblIntervalLowYieldByTotal_FTCap;
    TLabel *lblIntervalLowYieldByTotal_FT;
    TCheckBox *cbIntervalLowYieldByTotal_FT;
    TEdit *edIntervalLowYieldByTotal_FT;
    TEdit *edIntervalLowYieldByTotalIg_FT;
    TGroupBox *gbOtherYieldAlarmRT;
    TPanel *palIntervalLowYeildBySite_RT;
    TLabel *lblIntervalLowYieldBySite_RTCap;
    TLabel *lblIntervalLowYieldBySite_RT;
    TCheckBox *cbIntervalLowYieldBySite_RT;
    TEdit *edIntervalLowYieldBySite_RT;
    TEdit *edIntervalLowYieldBySiteIg_RT;
    TPanel *palIntervalLowYeildByTotal_RT;
    TLabel *lblIntervalLowYieldByTotal_RTCap;
    TLabel *lblIntervalLowYieldByTotal_RT;
    TCheckBox *cbIntervalLowYieldByTotal_RT;
    TEdit *edIntervalLowYieldByTotal_RT;
    TEdit *edIntervalLowYieldByTotalIg_RT;
    TGroupBox *grpSpecialLowYield;
    TLabel *lblLowYieldIg_Special1;
    TLabel *lblLowYieldIg_Special2;
    TCheckBox *cbLowYield_Special;
    TEdit *edLowYield_Special;
    TEdit *edLowYieldIg_Special1;
    TEdit *edLowYieldIg_Special2;
    TEdit *edtAutoSiteOffAlarm;
    TCheckBox *chkAutoSiteOffAlarm;
    TPanel *pnlChackInterval;
    TPanel *pnlAlarmFT;
    TLabel *lblLevel;
    TCheckBox *cbAllSiteFail;
    TEdit *edAllSiteFailCount;
    TPanel *palConsFailIgnore_FT;
    TCheckBox *cbContsFailIgnore_FT;
    TEdit *edContsFailIgnore_FT;
    TCheckBox *chkAutoSiteOffByContiFail;
    TCheckBox *chkAutoSiteOffByArmContiFail;
    TCheckBox *cbYieldAlarmByBin;
    TLabel *lblCheckInteval;
    TGroupBox *grpLowYield_FT;
    TLabel *labLowYield1_FT;
    TCheckBox *cbLowYield_FT;
    TEdit *edLowYield_FT;
    TEdit *edLowYieldIg_FT;
    TGroupBox *grpSiteYieldDifferent_FT;
    TLabel *labSiteYieldDifferent1_FT;
    TEdit *edSiteYieldDifferentIg_FT;
    TCheckBox *cbSiteYieldDifferent_FT;
    TEdit *edSiteYieldDifferent_FT;
    TCheckBox *chkAutoSiteOffByArmBySite;
    TGroupBox *grpSiteYieldCmp_FT;
    TLabel *labSiteYieldCmp1_FT;
    TEdit *edSiteYieldCmpIg_FT;
    TEdit *edSiteYieldCmp_FT;
    TCheckBox *cbSiteYieldCmp_FT;
    TCheckBox *chkAutoSiteOff;
    TGroupBox *grpLowYieldByTotal_FT;
    TLabel *labLowYieldByTotal1_FT;
    TCheckBox *cbLowYieldByTotal_FT;
    TEdit *edLowYieldByTotal_FT;
    TEdit *edLowYieldByTotalIg_FT;
    TPanel *pnlRT_Left;
    TGroupBox *gbPiggyBackRT;
    TRadioGroup *rgPiggyBack_RT;
    TPanel *palContinuousPass_RT;
    TLabel *labContinuousPass_RT;
    TLabel *labContinuousPass1_RT;
    TCheckBox *cbContinuousPass_RT;
    TComboBox *cobContinuousPass_RT;
    TEdit *edContinuousPass_RT;
    TPanel *palContinuPassSkt_RT;
    TLabel *labContinuPassSkt_RT;
    TLabel *labContinuPassSkt1_RT;
    TCheckBox *cbContinuPassSkt_RT;
    TEdit *edContinuPassSkt_RT;
    TPanel *palContinuousLoader_RT;
    TLabel *labContinuousLoader_RT;
    TLabel *labContinuousLoader1_RT;
    TCheckBox *cbContinuousLoad_RT;
    TEdit *edContinuousLoad_RT;
    TPanel *palContactCount_RT;
    TLabel *labContactCountRT;
    TLabel *labContactCount1RT;
    TCheckBox *cbContactCountRT;
    TEdit *edContactCountRT;
    TGroupBox *gbYieldAlarmRT;
    TPanel *pnlRT_Center;
    TGroupBox *gbAlarmRT;
    TLabel *lblAlarmLevelRT;
    TGroupBox *gbContsFailBySocket_RT;
    TLabel *labContsFailBySocket_RT;
    TRadioButton *rbContsFailBySocket_RTOn;
    TRadioButton *rbContsFailBySocket_RTOff;
    TEdit *edContsFailSocketAlarmCT_RT;
    TGroupBox *gbContsFailByHead_RT;
    TLabel *labContsFailByHead_RT;
    TRadioButton *rbContsFailByHead_RTOn;
    TRadioButton *rbContsFailByHead_RTOff;
    TEdit *edContsFailHeadAlarmCT_RT;
    TPanel *palConsFailIgnore_RT;
    TCheckBox *cbContsFailIgnore_RT;
    TEdit *edContsFailIgnore_RT;
    TCheckBox *cbAllSiteFail_RT;
    TEdit *edAllSiteFailCountRT;
    TGroupBox *grpLowYield_RT;
    TLabel *labLowYield1_RT;
    TCheckBox *cbLowYield_RT;
    TEdit *edLowYield_RT;
    TEdit *edLowYieldIg_RT;
    TGroupBox *grpSiteYieldDifferent_RT;
    TLabel *labSiteYieldDifferent1_RT;
    TEdit *edSiteYieldDifferentIg_RT;
    TCheckBox *cbSiteYieldDifferent_RT;
    TEdit *edSiteYieldDifferent_RT;
    TGroupBox *grpSiteYieldCmp_RT;
    TCheckBox *cbSiteYieldCmp_RT;
    TEdit *edSiteYieldCmp_RT;
    TLabel *labSiteYieldCmp1_RT;
    TEdit *edSiteYieldCmpIg_RT;
    TGroupBox *grpLowYieldByTotal_RT;
    TCheckBox *cbLowYieldByTotal_RT;
    TEdit *edLowYieldByTotal_RT;
    TLabel *labLowYieldByTotal1_RT;
    TEdit *edLowYieldByTotalIg_RT;
    TGroupBox *grpAlarm4;
    TGroupBox *grpAlarm4_SiteYieldOverAlert;
    TLabel *lblSiteYieldOverAlertCount;
    TCheckBox *cb_SiteYieldOverAlert;
    TEdit *ed_SiteYieldOverAlert;
    TEdit *ed_SiteYieldOverAlertCount;
    TGroupBox *grpAlarm4_ContinueType;
    TGroupBox *grAlarm4_SiteToSiteYieldEnable;
    TGroupBox *grpAlarm4_HeadToHeadYieldEnable;
    TLabel *lblHeadToHeadYieldCount;
    TCheckBox *cb_HeadToHeadYieldEnable;
    TEdit *ed_HeadToHeadYield;
    TEdit *ed_HeadToHeadYieldCount;
    TLabel *lblSiteToSiteYieldCount;
    TCheckBox *cb_SiteToSiteYieldEnable;
    TEdit *ed_SiteToSiteYield;
    TEdit *ed_SiteToSiteYieldCount;
    TLabel *lbAlarm4IntervalCount;
    TLabel *lbAlarm4ContinueCount;
    TCheckBox *cbAlarm4ContinueType;
    TEdit *edAlarm4IntervalCount;
    TEdit *edAlarm4ContinueCount;
    TGroupBox *grpAlarm4_EnableIntervalYield;
    TLabel *lblAlarm4IntervalYieldIntervalCount;
    TLabel *lblAlarm4IntervalYieldContinueCount;
    TCheckBox *cb_Alarm4EnableIntervalYield;
    TEdit *ed_Alarm4IntervalYieldIntervalCount;
    TEdit *ed_Alarm4IntervalYieldContinueCount;
    TEdit *ed_Alarm4IntervalYieldYield;
    TLabel *lblAlarm4IntervalYieldYield;
    TPanel *pnlAlarm_Center;
    TGroupBox *grpAlarm5;
    TPanel *pnlAlarm5_BySiteIntervalContactCnt;
    TLabel *lblAlarm5_BySiteIntervalContactCnt;
    TEdit *edAlarm5_BySiteIntervalContactCnt;
    TGroupBox *grpAlarm5_BySiteLowYield;
    TLabel *lblAlarm5_BySiteLowYieldRej;
    TCheckBox *cbAlarm5_BySiteLowYieldEnable;
    TEdit *edAlarm5_BySiteLowYield;
    TEdit *edAlarm5_BySiteLowYieldRej;
    TGroupBox *grpAlarm5_BySiteCmpYield;
    TLabel *lblAlarm5_BySiteCmpYieldRej;
    TCheckBox *cbAlarm5_BySiteCmpYieldEnable;
    TEdit *edAlarm5_BySiteCmpYield;
    TEdit *edAlarm5_BySiteCmpYieldRej;
    TGroupBox *grpAlarm5_BySiteAlarmYield;
    TLabel *lblAlarm5_OSBin;
    TLabel *lblAlarm5_BySiteAlarmYieldRej;
    TCheckBox *cbAlarm5_BySiteAlarmYieldEnable;
    TEdit *edAlarm5_BySiteAlarmYield;
    TEdit *edAlarm5_BySiteAlarmYieldRej;
    TEdit *edAlarm5_OSBin;
    TGroupBox *grpAlarm5_BySitePreCmpYieldEnable;
    TLabel *lblAlarm5_BySitePreCmpYieldRej;
    TCheckBox *cbAlarm5_BySitePreCmpYieldEnable;
    TEdit *edAlarm5_BySitePreCmpYield;
    TEdit *edAlarm5_BySitePreCmpYieldRej;
    TGroupBox *grpLowYieldByPicker_FT;
    TLabel *lblLowYieldByPicker_FT;
    TEdit *edtLowYieldByPickerIg_FT;
    TEdit *edtLowYieldByPicker_FT;
    TCheckBox *chkLowYieldByPicker_FT;
    TCheckBox *chkchkAutoSiteOffByPicker;
    TGroupBox *grpLowYieldByPicker_RT;
    TLabel *lblLowYieldByPicker_RT;
    TEdit *edtLowYieldByPickerIg_RT;
    TEdit *edtLowYieldByPicker_RT;
    TCheckBox *chkLowYieldByPicker_RT;
    TCheckBox *chkchkAutoSiteOffByPicker_RT;
    TCheckBox *chkSpcBinCntAsContinue_FT;
    TCheckBox *chkSpcBinCntAsContinue_RT;
    TRadioGroup *rgAutoSiteOn;
    TEdit *edInput;
    TLabel *labLimite;
    TGroupBox *grpCreateManualEOCAP;
    TCheckBox *chkCreateManualEOCAP;
    TGroupBox *grpAdaptiveYield_FT;
    TLabel *lblContsLowerAlarmNor_FT;
    TLabel *lblContsLowerAlarmMin_FT;
    TLabel *lblAdaptiveYield_FT;
    TCheckBox *cbAdaptiveLowYield_FT;
    TButton *btnResetInterval;
    TEdit *edlContsLowerAlarmNor_FT;
    TEdit *edlContsLowerAlarmMin_FT;
    TEdit *edAdaptiveYieldMax_FT;
    TEdit *edAdaptiveYieldMin_FT;
    TGroupBox *grpAdaptiveYield_RT;
    TLabel *lbllContsLowerAlarmNor_RT;
    TLabel *lbllContsLowerAlarmMin_RT;
    TLabel *lblAdaptiveYield_RT;
    TCheckBox *cbAdaptiveLowYield_RT;
    TEdit *edlContsLowerAlarmNor_RT;
    TEdit *edlContsLowerAlarmMin_RT;
    TEdit *edAdaptiveYieldMin_RT;
    TEdit *edAdaptiveYieldMax_RT;
    TComboBox *cbUseFTFailYieldModel;
    TEdit *edFailYieldRate_ARTFTFile;
    TEdit *edFailYieldMinRate_ARTFTFile;
    TEdit *edFailYieldMaxRate_ARTFTFile;
    TRadioGroup *rgFT_ART;
    TScrollBox *scrlbxBinAlarm1_FT;
    TCheckBox *cbByBinFailureCatXX_FT;
    TEdit *edByBinFailureCatXX_FT;
    TScrollBox *scrlbxBinAlarm1_RT;
    TScrollBox *scrlbxBinAlarm2_RT;
    TScrollBox *scrlbxBinAlarm3_RT;
    TScrollBox *scrlbxBinAlarm2_FT;
    TScrollBox *scrlbxBinAlarm3_FT;
    TComboBox *cbbClosedSiteBin;
    TRadioGroup *rgAutoSiteOnByGPIB;
    TTabSheet *TabSheet1;
    TLabel *Label9;
    TLabel *labByBinLimitCount;
    TCheckBox *cbBybinLimitCat0_FT;
    TEdit *edByBinLimitCountCat0_FT;
    TCheckBox *cbBybinLimitCat1_FT;
    TEdit *edByBinLimitCountCat1_FT;
    TCheckBox *cbBybinLimitCat2_FT;
    TCheckBox *cbBybinLimitCat3_FT;
    TCheckBox *cbBybinLimitCat4_FT;
    TEdit *edByBinLimitCountCat3_FT;
    TCheckBox *cbBybinLimitCat5_FT;
    TCheckBox *cbBybinLimitCat6_FT;
    TCheckBox *cbBybinLimitCat7_FT;
    TCheckBox *cbBybinLimitCat8_FT;
    TCheckBox *cbBybinLimitCat9_FT;
    TEdit *edByBinLimitCountCat9_FT;
    TCheckBox *cbBybinLimitCat10_FT;
    TEdit *edByBinLimitCountCat10_FT;
    TCheckBox *cbBybinLimitCat11_FT;
    TEdit *edByBinLimitCountCat11_FT;
    TCheckBox *cbBybinLimitCat12_FT;
    TEdit *edByBinLimitCountCat12_FT;
    TCheckBox *cbBybinLimitCat13_FT;
    TEdit *edByBinLimitCountCat13_FT;
    TEdit *edByBinLimitCountCat2_FT;
    TEdit *edByBinLimitCountCat8_FT;
    TEdit *edByBinLimitCountCat7_FT;
    TEdit *edByBinLimitCountCat6_FT;
    TEdit *edByBinLimitCountCat4_FT;
    TCheckBox *cbBybinLimitCat14_FT;
    TEdit *edByBinLimitCountCat14_FT;
    TCheckBox *cbBybinLimitCat15_FT;
    TEdit *edByBinLimitCountCat15_FT;
    TEdit *edByBinLimitCountCatCT_FT;
    TEdit *edByBinLimitCountCat5_FT;
    TTabSheet *TabSheet2;
    TLabel *Label11;
    TLabel *labByBinLimitCountRT;
    TCheckBox *cbBybinLimitCat0_RT;
    TEdit *edByBinLimitCountCat0_RT;
    TCheckBox *cbBybinLimitCat1_RT;
    TEdit *edByBinLimitCountCat1_RT;
    TCheckBox *cbBybinLimitCat2_RT;
    TCheckBox *cbBybinLimitCat3_RT;
    TCheckBox *cbBybinLimitCat4_RT;
    TEdit *edByBinLimitCountCat3_RT;
    TCheckBox *cbBybinLimitCat5_RT;
    TCheckBox *cbBybinLimitCat6_RT;
    TCheckBox *cbBybinLimitCat7_RT;
    TCheckBox *cbBybinLimitCat8_RT;
    TEdit *edByBinLimitCountCat5_RT;
    TCheckBox *cbBybinLimitCat9_RT;
    TEdit *edByBinLimitCountCat9_RT;
    TCheckBox *cbBybinLimitCat10_RT;
    TEdit *edByBinLimitCountCat10_RT;
    TCheckBox *cbBybinLimitCat11_RT;
    TEdit *edByBinLimitCountCat11_RT;
    TCheckBox *cbBybinLimitCat12_RT;
    TEdit *edByBinLimitCountCat12_RT;
    TCheckBox *cbBybinLimitCat13_RT;
    TEdit *edByBinLimitCountCat13_RT;
    TEdit *edByBinLimitCountCat2_RT;
    TEdit *edByBinLimitCountCat8_RT;
    TEdit *edByBinLimitCountCat7_RT;
    TEdit *edByBinLimitCountCat6_RT;
    TEdit *edByBinLimitCountCat4_RT;
    TCheckBox *cbBybinLimitCat14_RT;
    TEdit *edByBinLimitCountCat14_RT;
    TCheckBox *cbBybinLimitCat15_RT;
    TEdit *edByBinLimitCountCat15_RT;
    TEdit *edByBinLimitCountCatCT_RT;
    TLabeledEdit *edSlidingWindowSize;
    TCheckBox *cbSlidingWindowYield;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnApplyClick(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall rgPiggyBack_FTClick(TObject *Sender);
    void __fastcall edContactCountFTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edLowYield_RTMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edContactCountFTChange(TObject *Sender);
    void __fastcall edContsFailSocketAlarmCT_FTMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edQAModeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall rgQARunModeClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall edContsFailSocketAlarmCT_RTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edContsFailHeadAlarmCT_FTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edContsFailHeadAlarmCT_RTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall rgBinAlarmByClick(TObject *Sender);
    void __fastcall cbLowYield_FTMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall cbSiteYieldDifferent_FTMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall rbContsFailBySocket_FTOnMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall rbContsFailByHead_FTOnMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall rbContsFailBySocket_RTOnMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall rbContsFailByHead_RTOnMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbLowYield_RTMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall cbSiteYieldDifferent_RTMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edAutoCleanLowYieldLimitMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall mtBinSelectYieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtTrayNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edLowYieldIg_FTMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edAutoRetestLimitFileMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edFailYieldRate_ARTFTFileKeyPress(TObject *Sender,
          char &Key);
    void __fastcall edFailYieldRate_ARTFTFileMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edContsFailIgnore_FTMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edAutoLeastRetestLimitFileMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edFailYieldMaxRate_ARTFTFileMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edFailYieldMaxRate_ARTRTFileMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall edAlarm4ContinueCountMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SaveSetupFileToConfig(AnsiString szDir, AnsiString S);
    void __fastcall edtSiteYieldCmp1SiteCnt_FTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edAlarm5_OSBinMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtAutoSiteOffWhenRTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbLowYieldByTotal_RTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbIntervalLowYieldBySite_FTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbIntervalLowYieldByTotal_FTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edIntervalLowYieldBySiteIg_FTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbIntervalLowYieldBySite_RTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbIntervalLowYieldByTotal_RTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbLowYieldByTotal_FTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbSiteYieldCmp_RTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall edtAutoSiteOffAlarmMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnResetIntervalClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:    // User declarations
    void DoFormToData();
    void ChangeData(TWinControl *PCtrl);
    void __fastcall CheckSettingNo();                                           //Steven 20110506 Start: 加入範圍保護

    void __fastcall mtTrayNameSetColor(int tag);                                //kevin 20150703
    void __fastcall InitDataToEdit(int tag);                                    //kevin 20150703
public:     // User declarations
    __fastcall TfYieldMonitoring(TComponent* Owner);
    void ReadFile();
    void DoIniDataToForm();

    int    iMinYield;
    int    iMaxYield;
    double dMinYield;
    double dMaxYield;
    int    iMinCount;
    int    iMaxCount;

    TCheckBox *cbByBinSiteGapCat_FT[TEST_MAX_BIN];
    TCheckBox *cbByBinSiteGapCat_RT[TEST_MAX_BIN];
    TEdit     *edByBinSiteGapCat_FT[TEST_MAX_BIN];
    TEdit     *edByBinSiteGapCat_RT[TEST_MAX_BIN];
    TCheckBox *cbByArmSiteGapCat_FT[TEST_MAX_BIN];
    TCheckBox *cbByArmSiteGapCat_RT[TEST_MAX_BIN];
    TEdit     *edByArmSiteGapCat_FT[TEST_MAX_BIN];
    TEdit     *edByArmSiteGapCat_RT[TEST_MAX_BIN];
    TCheckBox *cbByBinFailureCat_FT[TEST_MAX_BIN];
    TCheckBox *cbByBinFailureCat_RT[TEST_MAX_BIN];
    TEdit     *edByBinFailureCat_FT[TEST_MAX_BIN];
    TEdit     *edByBinFailureCat_RT[TEST_MAX_BIN];

    void CalculateSiteYield();
    void CheckBySiteYieldAlarm();
    void CheckBySiteByArmYieldAlarm();
    void CheckLowYieldAlarm();
    void CheckLowYieldAlarmByTotal();
    void CheckLowYieldAlarmSpecial();                                           //Sam 20210505 : PTI 要求的兩段 Low Yeild
    void CheckByPickerYieldAlarm();                                             //Steven 20230223 : 根據Index吸嘴比較良率
    void ClearYieldCount();                                                     //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
    void DoAutoCloseSite(int iAllSiteOn);                                       //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
    void __fastcall SaveSetupFile(AnsiString szDir,AnsiString S);               //kevin 20150105  void SaveFile();
    int iAutoClean_FailAlarmSiteYieldIntervalCount;                             //ChungHung 20131223 add for SCK
    int iLowYieldContactCount;                                                  //Steven 20141212 : Yield控制使用Contact Count
    int iFailAlarmSiteYieldIntervalCount;
    int iPickerYieldIntervalCount;                                              //Steven 20230223 : 根據Index吸嘴比較良率
    int iFailAlarmSiteMaxYieldIntervalCount;                                    //jou 2014-08-14 Site Compare Low Yield alarm
    bool bShowSiteYield[32];                                                    //Steven 20100113 : 8->16  //Dell 2013-03-05   16->32
    double dSiteYield[4][8];
    double dPickerYield[2][2][8];                                               //Steven 20230223 : 根據Index吸嘴比較良率
    double dMaxPickerYield;                                                     //Steven 20230223 : 根據Index吸嘴比較良率
    int iAlarmSiteYieldCmpCnt[4][8];                                            //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
    double dIndexZ1Yield, dIndexZ2Yield;
    bool bFirstCount;
    bool fShow;
    bool bHasCloseSite;                                                         //Isaac 20171227 : 記錄low yield auto site off log，移到外層
    void ClearAutoSiteOffStatus();                                              //Steven 20200409 : 修正清除count之後,不能開site的問題
    void CheckIntervalLowYieldAlarmBySite();                                    //wei 20180606 Interval Low Yield By Site
    void CheckIntervalLowYieldAlarmByTotal();                                   //wei 20180718 Interval Low Yield By Total

    void __fastcall DoSetRPDefault(TWinControl *PCtrl,int iSection);            //Sam 20201209 : Default Recipe ChangeLog
    void __fastcall DoReplyDefaultToForm(TWinControl *PCtrl,int iSection);      //Sam 20201209 : Default Recipe ChangeLog
    void __fastcall SearchRecipeParameter(TWinControl *PCtrl,int iSection);     //Sam 20201209 : Default Recipe ChangeLog

    int iLowYieldByTotalContactCount;                                           //Kaichen 20190628 : Low Yield ByTotal 控制使用 Contact Count
    bool CanAutoCloseSite(int iAllSiteOn);                                      //Steven 20230315 : 整合自動關Site功能的判斷
    void DoRTAutoSocketOff();                                                   //Steven 20200205 : 切到RT的時候,要關閉Socket
    void SetClosedSiteBin();                                                    //Steven 20240409 : 關site的位置有IC不測試送指定 bin
    bool bGetGPIBAutoSiteOff;                                                   //JimmyChiu 20250715 : Auto site on/off by GPIB

    //=== Sliding Window Yield Ring Buffer ===                                   //Steven 20260331
    struct TSWRing {
        unsigned char bPass[2000];
        int  iHead;
        int  iCount;
        int  iPassCT;
        unsigned long iLastBySiteTotal;
        unsigned long iLastBySitePass;
    };
    TSWRing SW[2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    void SWRingReset(int arm, int row, int col);
    void SWRingPush(int arm, int row, int col, bool bIsPass, int N);
    void CheckLowYieldAlarm_SW();                                               //Steven 20260331 : Sliding Window Low Yield
    //=== End Sliding Window ===
};
//---------------------------------------------------------------------------
extern PACKAGE TfYieldMonitoring *fYieldMonitoring;
//---------------------------------------------------------------------------
#endif
