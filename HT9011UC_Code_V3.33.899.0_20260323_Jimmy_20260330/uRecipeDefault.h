//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfRecipeDefault : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pagecTimer;
    TTabSheet *tsIndex;
    TPanel *pnlReceipeDefaultIndex;
    TLabel *lbIndexunit_Speed;
    TLabel *lbIndexunit_VaccumCheckTime;
    TLabel *lbIndexunit_CounterAirOnTime;
    TLabel *lbIndexunit_DestoryAgainTime;
    TLabel *lbIndexunit_DestoryAgainCount;
    TPanel *pnlIndexunit_DestoryAgainCount;
    TPanel *pnlIndexunit_DestoryAgainTime;
    TPanel *pnlIndexunit_CounterAirOnTime;
    TPanel *pnlIndexunit_VaccumCheckTime;
    TPanel *pnlIndexunit_Speed;
    TTabSheet *tsInputArm;
    TPanel *pnlReceipeDefaultInputArm;
    TLabel *lbInput_XYSpeed;
    TLabel *lbInput_ZSpeed;
    TLabel *lbInput_PitchSpeed;
    TLabel *lbInput_XYAcc;
    TLabel *lbInput_ZAcc;
    TLabel *lbInput_PitchAcc;
    TLabel *lbInput_VacumCheckTime;
    TLabel *lbInput_ArmAirOnTime;
    TLabel *lbInput_ArmShuttleWaitTime;
    TLabel *lbInput_ArmDestroyAgainTime;
    TLabel *lbInput_ArmDestroyAgainCount;
    TPanel *pnlInput_ArmDestroyAgainCount;
    TPanel *pnlInput_ArmDestroyAgainTime;
    TPanel *pnlInput_ArmShuttleWaitTime;
    TPanel *pnlInput_ArmAirOnTime;
    TPanel *pnlInput_VacumCheckTime;
    TPanel *pnlInput_PitchAcc;
    TPanel *pnlInput_ZAcc;
    TPanel *pnlInput_XYAcc;
    TPanel *pnlInput_PitchSpeed;
    TPanel *pnlInput_ZSpeed;
    TPanel *pnlInput_XYSpeed;
    TTabSheet *tsOutputArm;
    TPanel *pnlReceipeDefaultOutputArm;
    TLabel *lbOutput_ZSpeed;
    TLabel *lbOutput_ZAcc;
    TLabel *lbOutput_PitchAcc;
    TLabel *lbAutoClean_InArmSpeed;
    TLabel *lbOutput_ArmAirOnTime;
    TLabel *lbOutput_ArmDestroyAgainTime;
    TLabel *lbOutput_ArmDestroyAgainCount;
    TLabel *lbOutput_OutXYAcc;
    TLabel *lbOutput_XYSpeed;
    TLabel *lbOutput_PitchSpeed;
    TPanel *pnlOutput_ZSpeed;
    TPanel *pnlOutput_PitchSpeed;
    TPanel *pnlOutput_ZAcc;
    TPanel *pnlOutput_PitchAcc;
    TPanel *pnlOutput_VacumCheckTime;
    TPanel *pnlOutput_ArmAirOnTime;
    TPanel *pnlOutput_ArmDestroyAgainTime;
    TPanel *pnlOutput_ArmDestroyAgainCount;
    TPanel *pnlOutput_OutXYAcc;
    TPanel *pnlOutput_XYSpeed;
    TTabSheet *tsShuttle;
    TPanel *pnlReceipeDefaultShuttle;
    TLabel *lbShuttle_Shuttle1Speed;
    TLabel *lbShuttle_Shuttle2Speed;
    TLabel *lbShuttle_Shuttle1Accel;
    TLabel *lbShuttle_Shuttle2Accel;
    TPanel *pnlShuttle_Shuttle2Accel;
    TPanel *pnlShuttle_Shuttle1Accel;
    TPanel *pnlShuttle_Shuttle2Speed;
    TPanel *pnlShuttle_Shuttle1Speed;
    TTabSheet *tsTrayArm;
    TPanel *pnlReceipeDefaultTrayArm;
    TLabel *lbTrayArm_XSpeed;
    TLabel *lbTrayArm_XAcc;
    TLabel *lbTrayArm_RetryCount;
    TLabel *lbTrayArm_VacuumCheckTime;
    TPanel *pnlTrayArm_VacuumCheckTime;
    TPanel *pnlTrayArm_RetryCount;
    TPanel *pnlTrayArm_XAcc;
    TPanel *pnlTrayArm_XSpeed;
    TLabel *lbTrayArm_CounterAirONTime;
    TPanel *pnlTrayArm_CounterAirONTime;
    TLabel *lbTrayArm_HandDownTime;
    TPanel *pnlTrayArm_HandDownTime;
    TTabSheet *tsAutoClean;
    TPanel *pnlReceipeDefaultAutoClean;
    TLabel *lbAutoClean_AlarmCount;
    TLabel *lbAutoClean_OutArmSpeed;
    TLabel *lbAutoClean_ShuttleSpeed;
    TLabel *lbAutoClean_RotationStageSpeed;
    TLabel *lbAutoClean_ContactTime;
    TLabel *lbAutoClean_ContactCount;
    TPanel *pnlAutoClean_RotationStageSpeed;
    TPanel *pnlAutoClean_ShuttleSpeed;
    TPanel *pnlAutoClean_OutArmSpeed;
    TPanel *pnlAutoClean_AlarmCount;
    TPanel *pnlAutoClean_ContactTime;
    TPanel *pnlAutoClean_ContactCount;
    TLabel *lblAutoCleaning_ModeIntervalContactCount;
    TPanel *pnlAutoCleaning_ModeIntervalContactCount;
    TLabel *lblAutoCleaning_ModeSocketAlarmCount;
    TPanel *pnlAutoCleaning_ModeSocketAlarmCount;
    TTabSheet *tsYield;
    TPanel *pnlReceipeDefaultYield;
    TLabel *lblLowYieldEnable_FT;
    TLabel *lblLowYield_FT;
    TLabel *lblLowYieldIg;
    TLabel *lblContsFailSocketAlarmCT_FT;
    TLabel *lblContsFailHeadAlarmCT_FT;
    TLabel *lblAlarm4IntervalYieldYield;
    TLabel *lblAlarm4IntervalYieldIntervalCount;
    TLabel *lblAlarm4IntervalYieldContinueCount;
    TPanel *pnlContsFailHeadAlarmCT_FT;
    TPanel *pnlContsFailSocketAlarmCT_FT;
    TPanel *pnlLowYieldIg;
    TPanel *pnlLowYield_FT;
    TPanel *pnlLowYieldEnable_FT;
    TPanel *pnlAlarm4IntervalYieldYield;
    TPanel *pnlAlarm4IntervalYieldIntervalCount;
    TPanel *pnlAlarm4IntervalYieldContinueCount;
    TLabel *lblSiteToSiteYield;
    TPanel *pnlSiteToSiteYield;
    TLabel *lblHeadToHeadYield;
    TPanel *pnlHeadToHeadYield;
    TLabel *lblAlarm4ContinueCount;
    TLabel *lblAlarm4IntervalCount;
    TLabel *lblAlarm4ContinueType;
    TPanel *pnlAlarm4ContinueType;
    TPanel *pnlAlarm4IntervalCount;
    TPanel *pnlAlarm4ContinueCount;
    TPanel *pnlAlarm4EnableIntervalYield;
    TLabel *lblAlarm4EnableIntervalYield;
    TTabSheet *tsTester;
    TPanel *pnlReceipeDefaultTester;
    TPanel *pnlInitialMaxTest;
    TPanel *pnlMaxTestTime;
    TPanel *pnlStartDelayTime;
    TLabel *lblStartDelayTime;
    TLabel *lblMaxTestTime;
    TLabel *lblInitialMaxTest;
//    void __fastcall FormShow(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfRecipeDefault(TComponent* Owner);
//    bool __fastcall SearchPanenColor(TWinControl *PCtrl);

};
//---------------------------------------------------------------------------
extern PACKAGE TfRecipeDefault *fRecipeDefault;
//---------------------------------------------------------------------------
#endif
