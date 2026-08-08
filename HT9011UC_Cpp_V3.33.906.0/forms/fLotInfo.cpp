// =============================================================================
//  forms/fLotInfo.cpp  --  definitions for the fLotInfo facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Bodies moved VERBATIM, with
//  exactly ONE addition, marked below: palRemoveTray's Enabled/Visible are now
//  set explicitly in the ctor, because the bespoke TfLotInfoPanel type they
//  used to carry defaulted them to true/true while the unified
//  vclcompat::TPanel defaults every control to false (see
//  vclcompat/Controls.h's DEFAULT-VALUE RULE).  Setting them here keeps the
//  CONSTRUCTED STATE byte-for-byte identical to before the refactor, which is
//  what W7-F0's zero-behaviour-change contract requires.
// =============================================================================
#include "forms/fLotInfo.h"

// --- W6.2: TfLotInfo -------------------------------------------------------
TfLotInfo::TfLotInfo()
{
    cbRunMode = new TfLotInfoRunMode();             // offline: Visible=false
    // -- W6.3 ADD --
    labNowLoaderTrayID = new TfLotInfoLabel();
    edtSysLotID        = new TfLotInfoEdit();
    // -- W5-Automation ADD --
    cbProcess          = new TfLotInfoRunMode();
    // -- W5-Final-TesterTCPSocket ADD --
    labTCPIPStatus = new TfLotInfoStatusLabel();
    mmTesterLog    = new TfMainMemo();
    // -- W5-Automation ADD (AGV_PortScan unit, 20260713) -----------------------
    ALedLoader    = new TfLedValue();
    for(int i=0;i<3;i++) aLedAuto[i] = new TfLedValue();
    palRemoveTray = new TfLotInfoPanel();
    // AI(W906-W7-F0) 20260728: STATE-PRESERVING ADDITION (the only behavioural
    // line added anywhere by the F0 refactor, and it exists precisely to add
    // NOTHING behaviourally).  The retired bespoke type was
    // `struct TfLotInfoPanel { bool Enabled; bool Visible;
    //  TfLotInfoPanel():Enabled(true),Visible(true){} };` -- true/true, per its
    // own W5-Automation note ("ordinary VCL TPanel design-time defaults ...
    // no consumer depends on the initial value": both fields are only ever
    // WRITTEN, by Automation/AGV_PortScan.cpp:305-306, and tests/
    // test_agv_portscan.cpp:382-387 sets them true itself before asserting they
    // become false).  The unified vclcompat::TPanel defaults them to false, so
    // they are restored here rather than left to drift.
    palRemoveTray->Enabled = true;
    palRemoveTray->Visible = true;
    // -- W906-AutoCleanFoundation ADD (20260721) ------------------------------
    for(int iW906AC=0; iW906AC<3; iW906AC++) iUnloaderTask[iW906AC] = 0;
    // -- AI(W906-Save2DSortingSummary) 20260723 ADD: 6 new TfLotInfoEdit members --
    edtCusLotID       = new TfLotInfoEdit();
    edtCusDevGrp      = new TfLotInfoEdit();
    edtCusStep        = new TfLotInfoEdit();
    edtDevice         = new TfLotInfoEdit();
    edtSysOperatorID  = new TfLotInfoEdit();
    mmo2DLotInfo      = new TfLotInfoEdit();
    // -- AI(W906-SaveTestSummaryTSV) 20260728 ADD: 3 new TfLotInfoEdit members + 1 more
    //    (lbledtCustomer, AI(W906-SaveSummaryTrayFeed) 20260728) --
    edtASECL_LotID    = new TfLotInfoEdit();
    edInsertion       = new TfLotInfoEdit();
    edFlowID          = new TfLotInfoEdit();
    lbledtCustomer    = new TfLotInfoEdit();
    // -- AI(W906-W7-L1-Wave0) 20260801 ADD: the 7 W7-L1 fLotInfo members (see
    //    forms/fLotInfo.h for per-member golden citations and for why the ""
    //    Caption default is behaviourally load-bearing on the KYEC-AMR arm) --
    LabDiffTrayCount           = new TfLotInfoLabel();   // golden uLotInfo.h:1086
    labLoaderTrayCount         = new TfLotInfoLabel();   // golden uLotInfo.h:1080
    labNowTrayCount            = new TfLotInfoLabel();   // golden uLotInfo.h:1084
    labNowAuto1TrayID          = new TfLotInfoLabel();   // golden uLotInfo.h:799
    labNowAuto2TrayID          = new TfLotInfoLabel();   // golden uLotInfo.h:802
    labNowAuto3TrayID          = new TfLotInfoLabel();   // golden uLotInfo.h:804
    cbFirstTrayCheckOnUnloader = new TfMainCheckBox();   // golden uLotInfo.h:1026 -- offline Checked=false
    // AI(W906-PT-W3-integrate) 20260808: golden uLotInfo.h:266 -- only ->Click() is
    // touched, and that is an inherited offline no-op; see forms/fLotInfo.h for why
    // golden's OnClick chain cannot run here.
    btClearBarcodeList         = new vclcompat::TButton();  // golden uLotInfo.h:266
}
// AI(W906-AutoCleanFoundation) 20260721: golden uLotInfo.cpp:16250-16253 --
// REAL one-line body (was a total no-op stub before that wave). See
// forms/fLotInfo.h's iUnloaderTask/InitialUnLoaderTask member comments for the
// behaviour-change + dormant-call-site (SOFT_SIMULTE undefined) analysis.
void TfLotInfo::InitialUnLoaderTask(int iPos) { iUnloaderTask[iPos]=1; }
// -- W5-Automation ADD: AMR.cpp + HANA_ART.cpp method sinks (all offline no-op) --
void TfLotInfo::RefreshAMR() {}                                            // offline: no UI to refresh
void TfLotInfo::SetLotID(AnsiString /*ID*/, bool /*bReadFromFile*/) {}     // offline no-op
void TfLotInfo::SetLotStart(AnsiString /*sFunc*/, bool /*bReadFromFile*/) {} // offline no-op
TfLotInfo *fLotInfo = new TfLotInfo();
