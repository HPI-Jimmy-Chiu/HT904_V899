// =============================================================================
//  OmronLaser/LaserSensor.h  --  Omron laser distance-sensor debug/calibration
//                                 form (TfLaserSensor) + the shared serial
//                                 protocol tables it owns.
//
//  Faithful translation of golden OmronLaser/LaserSensor.h (250 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W3-laser) 20260807
//  Translation wave: W906-PT-W3 ("laser" group).  First of 3 files (this one
//  is the shared base fLaserSensor->... that LaserSensorInArm.cpp and
//  LaserSensorShuttle.cpp both dereference); the two derived units land in
//  the SAME wave.
//
//  ROLE: golden TfLaserSensor is a VCL TForm (LaserSensor.dfm exists,
//  246+ widget instances) that is simultaneously (a) a manual calibration/
//  debug screen with ~20 button-click handlers a human operator presses, and
//  (b) the SOLE OWNER of the two Timer-driven serial state machines
//  (Timer1Timer / TimerInArmTimer) that actually drive the Omron laser
//  controllers over RS-232 -- LaserSensorInArm.cpp/LaserSensorShuttle.cpp
//  never touch CommLaser*->WriteCommData themselves; they set a task cursor
//  + bool flag on fLaserSensor and poll fLaserSensor->bInArm/bSh1_2Row/...
//  until the Timer state machine (running on this class) clears it. That
//  makes this form's OWN behaviour "ACTIVE" for this wave's purpose, unlike
//  the established forms/fXxx.h facade convention (fMain.cpp etc: only the
//  members OTHER files dereference are real, the form's OWN business logic
//  is out of scope) -- documented deliberate DEVIATION, see LaserSensor.cpp's
//  banner for the full reasoning.
//
//  THIS HEADER'S OWN SCOPE DECISION (mirrors the tree's established forms
//  convention of only carrying members some .cpp body actually touches --
//  aRotateKIT.cpp's "not one symbol referenced" precedent, forms/fMain.h's
//  "grow it per sub-wave" precedent): golden's PURELY decorative/layout
//  widgets -- ScrollBox1, gbLaserPanel_* group boxes, Panel1, Label1-7,
//  GroupBox5/GroupBox6, tsLaserSensor/tsLaserSet/TabSheet1-4/tsplate1/
//  tsPlate2, gpPlate1_Golden/gpPlate2_Golden, gbShuttle1_Golden/
//  gbShuttle2_Golden, PageControl2, and every palLaserValue_OutShuttle*/
//  gbLaserPanel_OutShuttle* pair -- are OMITTED.  Verified (grep over the
//  cp950-decoded golden .cpp, this wave): zero of these are dereferenced
//  anywhere in any golden TfLaserSensor method body; they exist only to be
//  laid out by the .dfm, which this campaign does not hand-translate (see
//  docs/PT_CAMPAIGN_PLAN.md SS1).  This drops 30 golden header lines to 0
//  ported members; nothing downstream can observe the removal because
//  nothing downstream ever read them.
//
//  WIDGET TYPES: stock VCL controls (TButton/TCheckBox/TEdit/TMemo/
//  TSpeedButton/TPageControl) come from vclcompat/Controls.h, matching the
//  forms/FormWidgets.h convention ("an existing vclcompat/Controls.h type,
//  or -- for custom control families -- a dedicated Core"). TTMyTray (12
//  members: mtPlate1/2 + _Golden, mtShuttle1/2 + _Golden, mtOutShuttle1/2 +
//  _Golden) maps onto vclcompat::TrayCore, exactly as forms/FormWidgets.h's
//  own banner names as the W7-C4 target for this custom-control family (that
//  wave had not yet repointed fMain's tray members when it wrote that note;
//  this NEW file starts there directly since it has no legacy struct to
//  migrate away from). TComm (CommLaser1/2/InArm/OutArm) maps onto
//  vclcompat/Comm.h's Spcomm::TComm, which is a COMPLETE existing substrate
//  (SIM/offline mode by construction -- see that header's own banner) with
//  a real OnReceiveData closure slot, so the ctor wires it for real instead
//  of gating it.
//
//  TU-LOCAL STAND-INS ADDED HERE (none touch any pre-existing file):
//    * TTimer -- golden TTimer (Timer1/TimerInArm) has no port anywhere in
//      this tree (grepped). Only ->Enabled is ever touched (FormDestroy).
//      Modeled exactly like vclcompat::TControl's own boolean properties.
//    * TLaserClickButton : vclcompat::TButton -- golden's `.dfm` wires each
//      button's OnClick to its *Click method (BCB6 TControl::Click() does
//      `if (Assigned(OnClick)) OnClick(Self);`).  vclcompat::TButton's own
//      Click() is a documented permanent no-op (Controls.h: "offline: with
//      no window there is no handler to dispatch to") because until now
//      nothing in this tree called a facade button's ->Click() and expected
//      a SIDE EFFECT.  LaserSensorInArm.cpp/LaserSensorShuttle.cpp are the
//      FIRST consumers that do exactly that (fLaserSensor->btGetValueInArm
//      ->Click() etc, 11 call sites total, grepped this wave) -- they are
//      not manual UI presses, they are the production floating-check state
//      machines driving this form programmatically. A plain TButton would
//      silently turn every one of those 11 calls into a no-op and break the
//      whole subsystem headless. This TU-local subclass overrides Click()
//      to invoke a bound std::function, giving BCB6's real OnClick dispatch
//      semantics without touching vclcompat/Controls.h itself (which stays
//      the tree-wide, zero-behaviour-change permanent no-op for every OTHER
//      button that nobody calls ->Click() on programmatically). Only the 11
//      buttons with a real external ->Click() caller (or an internal one --
//      btConnect, called from this class's own TimerInArmTimer) are typed
//      TButton* but allocated as `new TLaserClickButton(...)`; every other
//      button member stays a plain vclcompat::TButton (matches golden's
//      declared type in both cases -- only the allocated CONCRETE class
//      differs, invisible to callers because Click() is virtual).
//
//  WAVE SCOPE -- ACTIVE vs GATED (7 golden functions gated OUT WHOLE, per an
//  already-established tree precedent, not a new one this wave invents):
//   GATED, signature has NO PORT ANYWHERE (matches common.h's own
//   `TSpeedButton *SButton`-under-`#if 0` precedent for "golden TYPE has no
//   port" and EJ1N/MyOmronPanel.h's identical citation for the SAME two
//   types on the SAME class of golden method): TMouseButton/TShiftState do
//   not exist anywhere in this port tree (grepped, tree-wide, this wave --
//   the ONE other hit is EJ1N/MyOmronPanel.h's own citation of this same
//   fact). The following 7 golden methods take one or both of those types
//   and are therefore OMITTED from the ACTIVE class body entirely, exactly
//   as EJ1N/MyOmronPanel.h omitted its 3 mouse handlers for the identical
//   reason:
//     edICThicknessMouseDown        golden :1283-1287
//     mtPlate2_GoldenMouseDown      golden :1613-1621
//     mtPlate1_GoldenMouseDown      golden :1623-1631
//     edDeviceXOffsetMouseDown      golden :1633-1640
//     edDeviceYOffsetMouseDown      golden :1643-1650
//     edOutDeviceXOffsetMouseDown   golden :1653-1660
//     edOutDeviceYOffsetMouseDown   golden :1663-1670
//   BEHAVIOUR DELTA: none observable today -- every one of these bodies only
//   ever called `fQwertyKey->ShowQwertyKey(...)`. [UPDATE 20260824: the
//   "zero port" citation EXPIRED -- FW-QWKEY1 (fc08e09) landed the keyboard;
//   these handlers are still UNTRANSLATED (translation debt, queued), which
//   is a different thing from a dead dependency.] A manual operator editing
//   a calibration cell by mouse-click still cannot happen without a live
//   window, so the observable delta remains none today.
//
//   FormClose signature TRIMMED (not gated -- SAME established precedent as
//   SECSGEM/uHGemEquipment.h's THGem::FormClose, golden's OWN comment there
//   reads "TCloseAction stand-in exists anywhere in this tree; dropped
//   rather than [gated]"): golden `FormClose(TObject *Sender, TCloseAction
//   &Action)` becomes `FormClose()` here -- TCloseAction has no port
//   anywhere (grepped) and the body never reads/writes Action or Sender.
//
//  GATE REGISTER (call-level, #if 0/#else): NONE in this header -- every
//  symbol this header's declarations depend on (vclcompat::TrayCore,
//  Spcomm::TComm, vclcompat::TButton/TCheckBox/TEdit/TMemo/TSpeedButton/
//  TPageControl) has a real compiled body already in this tree. Any
//  call-level gate needed by a METHOD BODY is registered in
//  LaserSensor.cpp's own banner (this file is declarations only).
//
//  VCL/Borland conversions: no TForm base (matches the tree's own forms/
//  facade convention -- fMain/fLotInfo/... are plain classes, not VCL
//  forms); `__fastcall`/`__published`/`PACKAGE` neutralised to nothing;
//  `Pointer` -> `void*`; `WORD`/BCB6 `byte` -> Spcomm::Word / `unsigned char`
//  (no `byte` typedef exists anywhere in this port -- grepped -- so the
//  bare type is used directly rather than inventing a new one-off alias).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include <Classes.hpp> / <Controls.hpp> / <StdCtrls.hpp> / <Forms.hpp> /
//     "SPComm.hpp" / <ComCtrls.hpp> / <ExtCtrls.hpp> / "ALed.hpp" /
//     "HTray.h" / "MachineType.h" / <Buttons.hpp>
//
//   NOT re-included here, and why:
//     Classes.hpp/Controls.hpp/StdCtrls.hpp/Forms.hpp/ComCtrls.hpp/
//       ExtCtrls.hpp/Buttons.hpp -- VCL headers; superseded by
//       vclcompat/Controls.h (stock widgets) per the tree's standard
//       de-VCL'ing convention.
//     "ALed.hpp" -- golden TALed custom LED control has zero members in
//       THIS class (grepped: no `TALed*` field anywhere in golden
//       LaserSensor.h); not needed.
//     "HTray.h" -- golden TTMyTray; superseded by vclcompat/TrayCore.h.
// =============================================================================
#ifndef LaserSensorH
#define LaserSensorH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"    // AnsiString, TDateTime/Now, Spcomm::TComm (via the umbrella), TObject
#include "vclcompat/Controls.h"      // TButton/TCheckBox/TEdit/TMemo/TSpeedButton/TPageControl
#include "vclcompat/TrayCore.h"      // vclcompat::TrayCore -- golden TTMyTray substrate
#include "MachineType.h"             // eLaserStatus (lsNormal/lsChecking/lsNeedCheck/lsFail), eEndCode (ec00.../ecTotal)
#include <functional>
//---------------------------------------------------------------------------

enum eResponseCodeL{rcl0000=0, rcl1001, rcl1002, rcl1003, rcl1101,
                    rcl1103, rcl1104, rcl2203, rcl2204, rcl2205, rclTotalCount};

//---------------------------------------------------------------------------
//  TTimer -- see file-head TU-LOCAL STAND-INS note.  Only ->Enabled is ever
//  touched by golden (FormDestroy).  Guarded because a later wave that ports
//  another Timer-owning form will want the identical minimal stand-in.
//---------------------------------------------------------------------------
#ifndef HT9045_LASERSENSOR_TTIMER_SHIM
#define HT9045_LASERSENSOR_TTIMER_SHIM
class TTimer : public vclcompat::TObject
{
public:
    bool Enabled;
    TTimer() : Enabled(false) {}
    virtual ~TTimer() {}
};
#endif // HT9045_LASERSENSOR_TTIMER_SHIM

//---------------------------------------------------------------------------
//  TLaserClickButton -- see file-head TU-LOCAL STAND-INS note.  Gives the 11
//  buttons that a real caller ->Click()s BCB6's actual OnClick-dispatch
//  semantics; every other button member stays a plain vclcompat::TButton.
//---------------------------------------------------------------------------
class TLaserClickButton : public vclcompat::TButton
{
public:
    std::function<void()> OnClickFn;
    virtual void Click() { if(OnClickFn) OnClickFn(); }
    virtual ~TLaserClickButton() {}
};

//---------------------------------------------------------------------------
class TfLaserSensor
{
private:    // User declarations
    AnsiString sResponseCode[2][rclTotalCount];
    AnsiString sEndCode[2][ecTotal];
    void ProcessReceiveData(int iCom, AnsiString Data);
    void ProcessReceiveData0101(int iCom, AnsiString Data);

public:     // User declarations
    TfLaserSensor();
    virtual ~TfLaserSensor() {}
    void WriteInfoToMemo(AnsiString asInfo);
    int  iMaxChannel;           //最多幾顆
    int  iCurrentChannel;       //目前做到第幾顆
    int  iCurrentChannelSht;

    int  iMaxStep;              //要掃幾次
    int  iCurrentStep;          //目前掃到第幾次

    int  iLaserTask;
    int  iLaser1Task;
    int  iLaser2Task;
    int  iLaserInArmTask;
    int  iLaserOutArmTask;                                                          //Eastsun 20260525 laser 整合
    bool bRecvData1, bRecvData2, bRecvDataIn, bRecvDataOut;                       //Eastsun 20260525 laser 整合 bRecvDataOut

    bool bSetToZero;

    bool bSetToZeroSh1;
    bool bSetToZeroSh2;
    bool bSetToZeroInArm;

    bool bSh1_2Row;
    bool bSh1_1Row;
    bool bSh2_2Row;
    bool bSh2_1Row;
    bool bInArm;
    bool bGetInArm;                                                                 //Eastsun 20260525 laser 整合
    bool bGetOutArm;                                                                //Eastsun 20260525 laser 整合

    AnsiString GetValueMessage[2][2];
    AnsiString GetValMessASCII[2][2];
    AnsiString GetValueMessageInArm;
    AnsiString GetValMessASCIIInArm;

    AnsiString SetToZeroMessage[2][2];
    AnsiString SetToZeroMessASCII[2][2];
    AnsiString SetToZeroMessageInArm;
    AnsiString SetToZeroMessASCIIInArm;
    AnsiString GetInArmValue;                                                       //Eastsun 20260525 laser 整合
    AnsiString GetOutArmValue;                                                      //Eastsun 20260525 laser 整合

    void ReadFile();
    void ReadLaserFile();
    void SaveShuttleLaserValue(int iSht, int iInOutSht=0);  //Eastsun 20260525 laser 整合 : iInOutSht 0=InSht 1=OutSht
    void SaveInArmLaserValue();
    void WriteToMemo(vclcompat::TMemo *Memo, AnsiString Str);

    vclcompat::TPanel *MyPanel[2][2];

    bool fShow;

    void DoIniDataToForm();
    void InitLaserEdtList();               //Steven 20190614 : 重新整理Edit to 變數的方式
    int iGetInShuttleGoldenTask;                                                //Eastsun 20260525 laser 整合
    int iGetOutShuttleGoldenTask;
        //==> Eastsun 20260525 laser 整合
        void DoGetLaserValue();
    void DoGetOutLaserValue();
        //<== Eastsun 20260525 laser 整合

    // ---- .dfm widgets actually touched by some method body (see file-head
    //      SCOPE DECISION for the omitted decorative ones) --------------------
    Spcomm::TComm *CommLaser1;
    Spcomm::TComm *CommLaser2;
    Spcomm::TComm *CommLaserInArm;
    Spcomm::TComm *CommLaserOutArm;

    vclcompat::TrayCore *mtPlate1, *mtPlate2, *mtPlate1_Golden, *mtPlate2_Golden;
    vclcompat::TrayCore *mtShuttle1, *mtShuttle2, *mtShuttle1_Golden, *mtShuttle2_Golden;
    vclcompat::TrayCore *mtOutShuttle1, *mtOutShuttle2, *mtOutShuttle1_Golden, *mtOutShuttle2_Golden;

    vclcompat::TPanel *palLaserValue_InArm;
    vclcompat::TPanel *palLaserValue_InShuttle1_1, *palLaserValue_InShuttle1_2;
    vclcompat::TPanel *palLaserValue_InShuttle2_1, *palLaserValue_InShuttle2_2;

    vclcompat::TCheckBox *CheckBox1;                        // "紀錄Log" gate for WriteInfoToMemo
    vclcompat::TCheckBox *cbStartPooling;
    vclcompat::TCheckBox *cbEnableShuttleLaser;
    vclcompat::TCheckBox *cbEnableInArmLaser;
    vclcompat::TCheckBox *cbEnableInShuttleFloatingCheck;
    vclcompat::TCheckBox *cbEnableOutShuttleFloatingCheck;
    vclcompat::TCheckBox *cbLD40L;

    vclcompat::TEdit *edICThickness;
    vclcompat::TEdit *edLaserShtThreshold;
    vclcompat::TEdit *edLaserHPThreshold;
    vclcompat::TEdit *edInArmDeviceFloatingCheckRange;
    vclcompat::TEdit *edOutArmDeviceFloatingCheckRange;
    vclcompat::TEdit *edDeviceXOffset, *edDeviceYOffset;
    vclcompat::TEdit *edOutDeviceXOffset, *edOutDeviceYOffset;
    vclcompat::TEdit *edTemp;

    vclcompat::TMemo *memoLaserSensor;
    vclcompat::TMemo *MemoStatus;
    vclcompat::TMemo *MemoSh1;
    vclcompat::TMemo *MemoSh2;

    vclcompat::TSpeedButton *sbUpdate;
    vclcompat::TSpeedButton *sbtExit;

    vclcompat::TPageControl *PageControl1;

    // buttons -- declared type matches golden (TButton*) in every case;
    // the 11 with a real ->Click() caller are ALLOCATED as
    // TLaserClickButton (see file-head note), the rest as plain TButton.
    vclcompat::TButton *btConnect;
    vclcompat::TButton *btSetToZero;
    vclcompat::TButton *btSetToZeroSh1;
    vclcompat::TButton *btSetToZeroSh2;
    vclcompat::TButton *btSetToZeroInArm;
    vclcompat::TButton *btGetValueSht2_1Row;
    vclcompat::TButton *btGetValueSht2_2Row;
    vclcompat::TButton *btGetValueSht1_1Row;
    vclcompat::TButton *btGetValueSht1_2Row;
    vclcompat::TButton *btGetValueInArm;
    vclcompat::TButton *btSaveLog;
    vclcompat::TButton *btSetPlate1GoldenToZero;
    vclcompat::TButton *btSetPlate2GoldenToZero;
    vclcompat::TButton *btGetInShuttleGolden;
    vclcompat::TButton *btGetOutShuttleGolden;
    vclcompat::TButton *btGetShValue;
    vclcompat::TButton *btGetOutShValue;

    TTimer *Timer1;
    TTimer *TimerInArm;

    // ---- event handlers / click handlers (golden __fastcall dropped) -------
    void CommLaser1ReceiveData(vclcompat::TObject *Sender, void *Buffer, Spcomm::Word BufferLength);
    void CommLaser2ReceiveData(vclcompat::TObject *Sender, void *Buffer, Spcomm::Word BufferLength);
    void CommLaserInArmReceiveData(vclcompat::TObject *Sender, void *Buffer, Spcomm::Word BufferLength);
    void CommLaserOutArmReceiveData(vclcompat::TObject *Sender, void *Buffer, Spcomm::Word BufferLength);

    void FormShow(vclcompat::TObject *Sender);
    void FormClose();                                       // golden (Sender,TCloseAction&) trimmed -- see file head
    void FormDestroy(vclcompat::TObject *Sender);

    void btSetToZeroClick(vclcompat::TObject *Sender);
    void btConnectClick(vclcompat::TObject *Sender);
    void cbStartPoolingClick(vclcompat::TObject *Sender);
    void btGetValueSht1_2RowClick(vclcompat::TObject *Sender);
    void btGetValueSht1_1RowClick(vclcompat::TObject *Sender);
    void btGetValueSht2_2RowClick(vclcompat::TObject *Sender);
    void btGetValueSht2_1RowClick(vclcompat::TObject *Sender);
    void btGetValueInArmClick(vclcompat::TObject *Sender);
    void btSetToZeroSh2Click(vclcompat::TObject *Sender);
    void btSetToZeroSh1Click(vclcompat::TObject *Sender);
    void btSetToZeroInArmClick(vclcompat::TObject *Sender);
    void Timer1Timer(vclcompat::TObject *Sender);
    void TimerInArmTimer(vclcompat::TObject *Sender);
    void sbUpdateClick(vclcompat::TObject *Sender);
    void sbtExitClick(vclcompat::TObject *Sender);
    void btSaveLogClick(vclcompat::TObject *Sender);
    void btSetPlate2GoldenToZeroClick(vclcompat::TObject *Sender);
    void btSetPlate1GoldenToZeroClick(vclcompat::TObject *Sender);
    void btGetInShuttleGoldenClick(vclcompat::TObject *Sender);
    void btGetOutShuttleGoldenClick(vclcompat::TObject *Sender);
    void btGetShValueClick(vclcompat::TObject *Sender);
    void btGetOutShValueClick(vclcompat::TObject *Sender);
};
//---------------------------------------------------------------------------
extern TfLaserSensor *fLaserSensor;
//---------------------------------------------------------------------------
extern AnsiString asLaserValue[2];                                              //Eastsun 20260525 laser 整合
#endif
