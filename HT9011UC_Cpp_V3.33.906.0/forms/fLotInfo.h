// =============================================================================
//  forms/fLotInfo.h  --  non-VCL stand-in for golden's fLotInfo form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtualisation; the widget stand-ins (TfLotInfoRunMode /
//  TfLotInfoLabel / TfLotInfoEdit / TfLotInfoStatusLabel / TfLotInfoPanel /
//  TfMainMemo / TfLedValue) now come from forms/FormWidgets.h.
//  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FLOTINFO_H
#define FORMS_FLOTINFO_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfLotInfo -- non-VCL stub (golden uLotInfo.h).  cbRunMode is TComboBox* in
//  the golden; the leaves deref ->Visible (bool) and ->Text.Pos("RT")
//  (AnsiString).  Used inside the iResetSiteMappingStep==2 ASM path
//  (offline: Visible=false -- see forms/FormWidgets.h, that default is
//  LOAD-BEARING: ainarm_SearchPlacePlate.cpp:4811 reads it).
// ===========================================================================
class TfLotInfo
{
public:
    TfLotInfoRunMode *cbRunMode;                  // [DATA] golden uLotInfo.h:307 (TComboBox*)
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs -----------
    TfLotInfoLabel   *labNowLoaderTrayID;         // [DATA] golden uLotInfo.h:1078 (TLabel* loader tray-ID label)
    TfLotInfoEdit    *edtSysLotID;                // [DATA] golden uLotInfo.h:302  (TEdit* system lot-ID edit)
    // AI(W906-AutoCleanFoundation) 20260721: golden uLotInfo.h:1264 `int
    // iUnloaderTask[3];` (Eastsun 20260515 F011) -- the backing store
    // InitialUnLoaderTask (below) writes. Was missing entirely (the previous
    // stand-in's InitialUnLoaderTask didn't even declare it) -- see
    // InitialUnLoaderTask's own comment for the behaviour-change note.
    int iUnloaderTask[3];                         // [DATA] golden uLotInfo.h:1264
    // golden uLotInfo.cpp:16250-16253 -- REAL one-line body (was a total no-op
    // before that wave). 5 existing call sites in acatchtray.cpp (all inside
    // `#ifdef SOFT_SIMULTE`, which is #undef'd/commented-out in MachineType.h
    // -- verified via grep -- so this is currently dormant on every compiled
    // path; a genuine no-op -> real-write behaviour change ONLY if/when
    // SOFT_SIMULTE is ever defined). No existing test references
    // InitialUnLoaderTask or iUnloaderTask (grepped tests/ -- zero hits).
    virtual void InitialUnLoaderTask(int iPos);    // [METHOD] golden uLotInfo.h -- REAL body: iUnloaderTask[iPos]=1;
    // -- W5-Automation INTEGRATE ADD: members Automation/AMR.cpp + HANA_ART.cpp derefs --
    TfLotInfoRunMode *cbProcess;                  // [DATA] golden uLotInfo.h:470 (TComboBox*; only ->Text used)
    virtual void RefreshAMR();                    // [METHOD] golden uLotInfo.h:1416 -- offline: no UI to refresh (no-op)
    virtual void SetLotID(AnsiString ID, bool bReadFromFile=false);       // [METHOD] golden uLotInfo.h:1310 -- offline no-op
    virtual void SetLotStart(AnsiString sFunc, bool bReadFromFile=false); // [METHOD] golden uLotInfo.h:1311 -- offline no-op
    // -- W5-Final-TesterTCPSocket INTEGRATE ADD: members Interface/TesterTCP_Socket.cpp
    //    derefs (golden TesterTCP.cpp:170/180/621 labTCPIPStatus, :290/294 mmTesterLog) --
    TfLotInfoStatusLabel *labTCPIPStatus;         // [DATA] golden uLotInfo.h:465 (TLabel* TCP/IP link-status)
    TfMainMemo           *mmTesterLog;            // [DATA] golden uLotInfo.h:468 (TMemo* tester comm-log) -- reuse TfMainMemo shape (fAGV->mmE84Log precedent)
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713): members
    //    Automation/AGV_PortScan.cpp derefs (AMR SPIL port-scan LEDs +
    //    E84 unloader tray-count scan's "remove tray" prompt panel) ----------
    TfLedValue     *ALedLoader;                   // [DATA] golden uLotInfo.h:971 (TALed*)
    TfLedValue     *aLedAuto[3];                  // [DATA] golden uLotInfo.h:965,969,970 (TALed* aLedAuto1/2/3, consolidated into an array -- same convention as fSortCT->pnlTrayCnt[6])
    TfLotInfoPanel *palRemoveTray;                 // [DATA] golden uLotInfo.h:828 (TPanel*, Enabled+Visible) -- see the ctor: its true/true initial state is set EXPLICITLY
    // AI(W906-Save2DSortingSummary) 20260723: 6 new members Automation/SCK_ART_Remainder.cpp's
    // SckArtRem_Save2DSortingSummary derefs (golden SCK_ART.cpp:3402-4061, only ->Text read/each --
    // reuse the existing TfLotInfoEdit shape, same idiom as edtSysLotID above).
    // mmo2DLotInfo is a TMemo* in golden (uLotInfo.h:846, `sList->Text=fLotInfo->mmo2DLotInfo->Text;`,
    // golden :3628) but only ->Text is ever read here too, so the same minimal shape applies (no
    // TStrings/Lines surface needed, unlike fObserver->memoLotSummary's whole-list-assign shape).
    TfLotInfoEdit *edtCusLotID;                    // [DATA] golden uLotInfo.h:623 (TEdit* customer lot-ID edit)
    TfLotInfoEdit *edtCusDevGrp;                   // [DATA] golden uLotInfo.h:624 (TEdit* customer device-group edit)
    TfLotInfoEdit *edtCusStep;                     // [DATA] golden uLotInfo.h:1038 (TEdit* customer step-no edit)
    TfLotInfoEdit *edtDevice;                      // [DATA] golden uLotInfo.h:625 (TEdit* device-name edit)
    TfLotInfoEdit *edtSysOperatorID;                // [DATA] golden uLotInfo.h:306 (TEdit* system operator-ID edit)
    TfLotInfoEdit *mmo2DLotInfo;                    // [DATA] golden uLotInfo.h:846 (TMemo* 2D lot-info source -- only ->Text read)
    // AI(W906-SaveTestSummaryTSV) 20260728: 4 new members Automation/SCK_ART_Remainder.cpp's
    // SckArtRem_SaveTestSummaryTSV derefs (golden SCK_ART.cpp:2884/3087, only ->Text read each --
    // reuse the existing TfLotInfoEdit shape, same idiom as edtCusLotID above).
    // golden edtASECL_LotID/edInsertion/edFlowID are TEdit* (uLotInfo.h:576/1044/1046); lbledtCustomer
    // is a TLabeledEdit* (uLotInfo.h:609, golden SckArtRem_SaveSummaryTrayFeed's own dependency) --
    // only ->Text is ever read from it here too, so the same minimal shape applies.
    TfLotInfoEdit *edtASECL_LotID;                  // [DATA] golden uLotInfo.h:576 (TEdit* ASE-CL lot-ID edit)
    TfLotInfoEdit *edInsertion;                     // [DATA] golden uLotInfo.h:1044 (TEdit* insertion-code edit)
    TfLotInfoEdit *edFlowID;                        // [DATA] golden uLotInfo.h:1046 (TEdit* flow-ID edit)
    // AI(W906-SaveSummaryTrayFeed) 20260728: golden SCK_ART.cpp:3157 (`fLotInfo->lbledtCustomer->Text`).
    TfLotInfoEdit *lbledtCustomer;                  // [DATA] golden uLotInfo.h:609 (TLabeledEdit* customer name edit)
    // AI(W906-W7-L1-Wave0) 20260801: W7-L1 Wave-0 ADD -- the 7 fLotInfo members
    // the asendic_* tray SM family dereferences, landed in one serialized pass so
    // the four parallel translation agents cannot collide on this header.  Every
    // golden line was re-read from the cp950-decoded golden uLotInfo.h this pass.
    // The first three are golden TLabel* whose Caption golden reads back through
    // atoi() (asendic_Loader.cpp:2069-2070), so they are ordinary Caption storage.
    //
    // OFFLINE-DEFAULT CONSEQUENCE, RECORDED BECAUSE IT IS NOT NEUTRAL: all three
    // Captions default to "" and atoi("")==0, which makes golden's KYEC-AMR
    // overflow guard at asendic_Loader.cpp:2068-2070
    //   (... atoi(LabDiffTrayCount->Caption)==0 &&
    //        (atoi(labLoaderTrayCount->Caption)-atoi(labNowTrayCount->Caption))==0)
    // evaluate TRUE whenever the surrounding TrayForm.bEnableAMR arm is entered.
    // A test that turns bEnableAMR on must therefore set all three captions
    // deliberately rather than relying on the constructed state.
    TfLotInfoLabel *LabDiffTrayCount;               // [DATA] golden uLotInfo.h:1086 (TLabel*) -- Caption read via atoi()
    TfLotInfoLabel *labLoaderTrayCount;             // [DATA] golden uLotInfo.h:1080 (TLabel*) -- Caption read via atoi()
    TfLotInfoLabel *labNowTrayCount;                // [DATA] golden uLotInfo.h:1084 (TLabel*) -- Caption read via atoi()
    TfLotInfoLabel *labNowAuto1TrayID;              // [DATA] golden uLotInfo.h:799 (TLabel*) -- Caption written (asendic_Auto.cpp:561)
    TfLotInfoLabel *labNowAuto2TrayID;              // [DATA] golden uLotInfo.h:802 (TLabel*) -- (asendic_Auto.cpp:562)
    TfLotInfoLabel *labNowAuto3TrayID;              // [DATA] golden uLotInfo.h:804 (TLabel*) -- (asendic_Auto.cpp:563)
    // Golden uLotInfo.h:1026 is a TCheckBox*, read-only in this family
    // (asendic_Auto.cpp:1994 `->Checked==true`).  Reuses the TfMainCheckBox alias
    // (forms/FormWidgets.h:114 -> vclcompat::TCheckBox, default Checked=false) --
    // the same alias fMain->chkE84IDTray uses, so no new widget stand-in is needed.
    TfMainCheckBox *cbFirstTrayCheckOnUnloader;     // [DATA] golden uLotInfo.h:1026 (TCheckBox*) -- offline Checked=false
    TfLotInfo();
    virtual ~TfLotInfo() {}
};
extern TfLotInfo *fLotInfo;

#endif // FORMS_FLOTINFO_H
