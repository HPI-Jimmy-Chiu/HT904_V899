//---------------------------------------------------------------------------

#ifndef AutoTeachH
#define AutoTeachH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "butPa1.h"
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfAutoTeach : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pc_AutoTeach;
    //TTabSheet *ts_DoAutoHeight;
    TTabSheet *ts_DoPickLoaderTray;
    TPanel *Panel1;
    TTabSheet *ts_DoPlaceShuttle;
    TTabSheet *ts_DoPickShuttle;
    TSpeedButton *sb_DoClose;
    TSpeedButton *sb_DoNext;
    TLabel *lb_Message;
    TTabSheet *ts_DoPlaceAutoTray;
    TTabSheet *ts_DoPlaceHotPlateTray;
    TTabSheet *ts_DoHomeing;
    TMemo *mo_ShowMessage;
    TSpeedButton *sb_DoRetry;
    TImage *img_ShowImage;
    TPanel *pan_AutoOffsetMove;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label4;
    TCheckBox *cb_AutoOffsetPositionCheck;
    TPanel *Panel2;
    TLabel *lb_JogMove;
    TSpeedButton *sb_AutoOffsetYRear;
    TSpeedButton *sb_AutoOffsetXLeft;
    TSpeedButton *sb_AutoOffsetXRight;
    TSpeedButton *sb_AutoOffsetYFront;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TPanel *Panel3;
    TSpeedButton *sb_AutoOffsetZDown;
    TLabel *Label2;
    TSpeedButton *sb_AutoOffsetZUp;
    TLabel *Label9;
    TLabel *Label10;
    TPanel *Panel4;
    TSpeedButton *sb_AutoOffsetPitchYClose;
    TSpeedButton *sb_AutoOffsetPitchXClose;
    TSpeedButton *sb_AutoOffsetPitchXOpen;
    TSpeedButton *sb_AutoOffsetPitchYOpen;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label11;
    TTabSheet *ts_DoShuttleSensorCheckL;
    TTabSheet *ts_DoShuttleSensorCheckR;
    TTabSheet *ts_DoAutoTeachFinish;
    TLabel *lb_DoingItem;
    TPanel *pn_Information;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TComboBox *cb_AdjustUnit;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label22;
    TEdit *ed_XOffsetValue;
    TLabel *Label23;
    TEdit *ed_YOffsetValue;
    TLabel *Label24;
    TEdit *ed_XPitchOffsetValue;
    TEdit *ed_YPitchOffsetValue;
    TLabel *Label25;
    TLabel *Label26;
    TEdit *ed_ZOffsetValue;
    TLabel *Label16;
    TEdit *ed_HintString;
    TLabel *Label306;
    TLabel *Label307;
    TLabel *edTorue1;
    TLabel *edTorue0;
    TLabel *Label310;
    TLabel *Label314;
    TLabel *Label315;
    TLabel *Label316;
    TLabel *Label320;
    TLabel *Label321;
    TLabel *Label322;
    TLabel *Label317;
    TLabel *Label318;
    TLabel *Label319;
    TTabSheet *ts_DoAutoHeight;
    TTabSheet *ts_DoPickHotPlateTray;
    TRadioGroup *rg_AdjustUnit;
    void __fastcall sb_DoNextClick(TObject *Sender);
    void __fastcall sb_DoCloseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sb_AutoOffsetXRightClick(TObject *Sender);
    void __fastcall sb_DoRetryClick(TObject *Sender);
    void __fastcall sb_AutoOffsetYFrontClick(TObject *Sender);
    void __fastcall sb_AutoOffsetXLeftClick(TObject *Sender);
    void __fastcall sb_AutoOffsetYRearClick(TObject *Sender);
    void __fastcall sb_AutoOffsetZUpClick(TObject *Sender);
    void __fastcall sb_AutoOffsetZDownClick(TObject *Sender);
    void __fastcall sb_AutoOffsetPitchYOpenClick(TObject *Sender);
    void __fastcall sb_AutoOffsetPitchXOpenClick(TObject *Sender);
    void __fastcall sb_AutoOffsetPitchYCloseClick(TObject *Sender);
    void __fastcall sb_AutoOffsetPitchXCloseClick(TObject *Sender);
    void __fastcall ed_XOffsetValueMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ed_ZOffsetValueMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    int     iATStatus;
    void    ShowSetFormPos(TForm *tTForm,bool bOpen,bool bhide);
    int     iAutoTeachStep;
    bool    bIsFirstTime;
    int     iLoaderCnt;
    int     iNextStep;
    enum    eATStatus
    {
        Idle=0,
        Run,
        Pause
    };
public:     // User declarations
    __fastcall TfAutoTeach(TComponent* Owner);
    enum eATActionItem
    {   eATClose            =0 ,          //°±¤î©Î¤¤Â_
        eATHome             =1 ,          //Home
        eATInShuttleSensor  =2 ,          //InShuttle Sensor
        eATOutShuttleSensor =3 ,          //OutShuttle Sensor
        eATIndexArm         =4 ,          //IndexArm
        eATLoader           =5 ,          //Pick Loader
        eATHP1              =6 ,          //Place HP2
        eATHP2              =7 ,          //Place HP1
        eATInShuttle1L      =8 ,          //Place InShuttle1
        eATInShuttle1R      =9 ,          //Place InShuttle1
        eATInShuttle2L      =10,          //Place InShuttle1
        eATInShuttle2R      =11,          //Place InShuttle2
        eATOutShuttle1L     =12,          //Pick OutShuttle1
        eATFix3             =13,          //Place Fix3
        eATAuto1            =14,          //Place Auto1
        eATAuto2            =15,          //Place Auto2
        eATAuto3            =16,          //Place Auto3
        eATFix1             =17,          //Place Fix1
        eATFix2             =18,          //Place Fix2
        eATOutShuttle1R     =19,          //Pick OutShuttle1
        eATOutShuttle2L     =20,          //Pick OutShuttle1
        eATOutShuttle2R     =21,          //Pick OutShuttle1
        eATActionFinish     =22,          //Finish
        eATTotalAction      =23          //Total
    };
    bool    bStepFinish[eATTotalAction];
    int     iATOutArmWhichKit;
    bool    bIsFormShow;
    int     iTaskPlaceToAuto;
    int     iWhichAuto;
    void    ShowOneTabSheet(TTabSheet *tsTabSheet);
    void    ShowFormAndActiveTabSheet(TTabSheet *tsTabSheet,AnsiString asFileName);
    void    FormLoadMessageAndImage(AnsiString asFileName);
    void    AutoOffsetPitchYCanVisible(bool bIsCanVisible);
    void    ShowFormAndMoveToCenter();
    void    InitialData();
    bool    DoAutoTeachProcess();
    bool    IsRun();
    void    StartAutoTeach(bool _bopen);
    void    SetState(int _state);
    int     GetState();
    void    DoNext();
    int     iTask;
    void    DoHome();
    void    DoInputShuttleDoubleDeviceSensor();
    void    DoOutputShuttleSensor();
    void    DoAutoHeight();
    void    DoLoaderPosition();
    void    DoPlaceHotPlate();                 //0:hp1 1:hp2
    void    DoInputShuttlePosition();          //0:insh1 1:insh2
    void    DoOutputShuttlePosition();         //0:outsh1 1:outsh2
    void    DoPlaceFix1();
    void    DoPlaceFix2();
    void    DoPlaceFix3();
    void    DoPlaceAuto1();
    void    DoPlaceAuto2();
    void    DoPlaceAuto3();
    void    DoFinish();
    void    DoErrorStep();
    bool    CheckGPIBisRun();
    void    UpdateInfoAutoHeight();
    void    UpdateInfoOffsetPosition();
    void    InitialInfoPlaceFixAutoTray(AnsiString asPoiMsg, TObject *Sender, int iPosIndex, TObject *Sortbtn);
    void    ShowInfoMsg(AnsiString asMsg,bool bPopMSG);
    void    ShowErrorMsg(AnsiString asMsg,bool bPopMSG);
    void    InArmSetupTeachCtl(int iIndex,bool bStart);
    void    OutArmSetupTeachCtl(int iIndex,bool bStart);
    double  GetAjustLength();
    void    CloseAllForm();
    int     SetAllICInOutArm2Fix3();
    int     GetSocketPickUpOffsetWhenAutoTeachOnly();
    int     GetShuttlePickUpOffsetWhenAutoTeach();
    void    SetIndexBinError(int iPosition);
    void    CheckLoaderCountNeedOneCycle();
    void    SetShuttleNeedTeaching();
    void    SetNextStep(int iNext);
    bool    CheckNextStep();
    bool    IsArmNeedPause(AnsiString asWhich);
    bool    IsShuttle1Enable();
    bool    IsShuttle2Enable();
    void    CheckAndAvoidHangUp();
    void    SetLoaderCount();
    bool    IsKeyStartEnable();
    void    DoAutoTeachStart();
};
//---------------------------------------------------------------------------
extern PACKAGE TfAutoTeach *fAutoTeach;                                         //JimmyChiu 20211020 : Auto alignment mode
//---------------------------------------------------------------------------
extern void bInoutArmTeach();                                                   //kevin 20210401
extern bool AutoTeachLoadTrayZ(bool Reset, int InArm);                          //kevin 20170930 (wei) load Z autohigh
extern bool AutoTeachLoadTrayZ(bool Reset, int InArm, int &iAOATask);           //kevin 20170930 load Z autohigh
extern bool AutoTeachInArmZLoaderTray(bool Reset, int InArm);                   //KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
extern int iAutoTeachInArmZTask;
extern int iInArmZTeachTask;                                                    //kevin 20171107 add
extern int iOutArmZTeachTask;                                                   //kevin 20171107 add
bool DoMoveInArmXYToLoader_AutoTeach(int iSelRow, int iRow, int iCol, bool RealMove);    //KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
extern void InitInArmAutoZTask();
bool DoMoveOutArmXY_AutoTeach(int iSelRow, int iRow, int iCol, int iAutoTray, bool RealMove);
#endif
