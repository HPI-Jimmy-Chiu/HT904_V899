// =============================================================================
//  FormsFacade.h  --  non-VCL stand-in for the VCL form pointers the SMs deref
//
//  Translation wave: W6.0 (scaffold) ; EXTENDED W6.2 (in-arm HP geometry)
//  Translator: AI(W6.0-SCAFFOLD) 20260626 ; AI(W6.2-INARM) 20260626
//
//  W6 DECOUPLING STRATEGY (form-pointer cut)
//  -----------------------------------------
//  The BCB6 state machines reach UI/automation state through global VCL TForm
//  pointers (fMain / fAGV / fSortCT / fLotInfo / fOffSet / fSCKART / ...).  Those
//  forms cannot be pulled into the portable build (they derive from VCL TForm).
//  Instead we expose ONLY the members a given sub-wave's SM actually
//  dereferences, on a non-VCL facade, and grow it per sub-wave.
//
//  W6.1 SURFACE = fAGV ONLY (Empty-tray canary).  -- unchanged below.
//
//  W6.2 ADDITIONS (HP pick/place geometry leaves -- exactly the members
//  ainarm_SearchPickPlate.cpp / ainarm_SearchPlacePlate.cpp deref):
//    TfMain    fMain    : DebugOneCycleHotPlate / Pause / ShowTestHeadComp /
//                         ReStartAutoSiteMapping / slAutoSiteMapLog
//    TfSortCT  fSortCT  : pnlHP1.Caption / pnlHP2.Caption
//    TfLotInfo fLotInfo : cbRunMode.Visible / cbRunMode.Text
//    TfOffSet  fOffSet  : UseAutoOffsetFunction / UseInArmSetupTeach
//    TfSCKART  fSCKART  : iInputJamCnt
//  Each member is documented [DATA]/[METHOD] with its golden home.  All methods
//  are offline no-ops / false (a handler with no UI / no auto-offset / no pause).
//  The sbStateRecordClick(sbStateRecord) line in CheckHasSpaceToPlace_9045 is
//  gated #if 0 in the .cpp, so TSpeedButton/TObject are NOT pulled in and those
//  two TfMain members are intentionally NOT added.  cb1 is referenced only
//  inside #ifdef SOFT_SIMULTE (undefined) so it is not added either.
// =============================================================================
#ifndef FormsFacadeH
#define FormsFacadeH

#include "vclcompat/vcl_compat.h"   // AnsiString

// ---------------------------------------------------------------------------
//  TfAGV -- non-VCL stub (W6.1).  Mirrors ONLY the one method the canary calls.
//  Golden: bool TfAGV::IsATK_AMR();  (Automation/AGV.h:205)
//  In the offline sim there is no ATK AMR present, so IsATK_AMR() returns false
//  (the DUMMY non-AMR path -- faithful to a handler with no AMR attached).
// ---------------------------------------------------------------------------
class TfAGV
{
public:
    bool IsATK_AMR();
    bool Use_AMR();                 // [METHOD] golden AGV.h -- W6.3: offline no AMR -> false
};

extern TfAGV *fAGV;     // golden: extern PACKAGE TfAGV *fAGV; (AGV.h:210)

// ===========================================================================
//  W6.2 -- TMyStringList facade-local stub for slAutoSiteMapLog
//  Golden type is TMyStringList (cmydef), which in the target is only
//  forward-declared (cmydef.h:15).  The leaves call exactly ONE method on it:
//  AddTextWithDateTime(AnsiString).  Provide a tiny facade-local type exposing
//  only that, as an append/log no-op (offline -> no file/UI sink).
// ===========================================================================
class TfMainSiteMapLog
{
public:
    void AddTextWithDateTime(AnsiString /*S*/) {}   // golden TMyStringList::AddTextWithDateTime
};

// ===========================================================================
//  TfMain -- non-VCL stub (golden main.h, TfMain:public TForm)
//  ONLY the members the W6.2 HP geometry leaves deref.
// ===========================================================================
// ---------------------------------------------------------------------------
//  W6.3: TfMainTrayPanel -- the mtAuto1/mtAuto2/mtAuto3 status panels.  Golden
//  type is TPanel*; the catchtray engine only assigns ->Color (a TColor).  Stub
//  ->Color as an int (TColor is an int RGB code in the VCL).  TColor + the
//  clWhite/clYellow constants live in vclcompat (vcl_compat.h).
// ---------------------------------------------------------------------------
struct TfMainTrayPanel { int Color; bool Visible; TfMainTrayPanel():Color(0),Visible(false){} };  // [DATA] golden main.h (TPanel* mtAutoN / TLabel* lbCCDStatus->Visible W6.4)

// ---------------------------------------------------------------------------
//  W6.3: TfMainHanaART -- fMain->hanaART (golden main.h, HANA ART helper).  The
//  catchtray count helpers (AddFixTrayCount/AddTrayCount) call exactly two
//  methods on it; offline (no HANA link) IsHanaArtAvailable() is false so the
//  count helpers early-out, and AddNewTrayHead is a no-op.
// ---------------------------------------------------------------------------
class TfMainHanaART
{
public:
    bool IsHanaArtAvailable();      // [METHOD] golden -- offline: false
    void AddNewTrayHead(int iAuto); // [METHOD] golden -- offline: no-op
};

// ---------------------------------------------------------------------------
//  W6.5: SHUTTLE-ENGINE (acarry.cpp) fMain sub-objects.
//    * TfMainCheckBox  -- golden TCheckBox* (cbShowShuttleSensor /
//      cbTestOutShuttleSensor / cbShowInShuttleSensor).  Engine reads .Checked
//      (offline false -> the debug-log / sensor-confirm paths are skipped, the
//      proven DUMMY posture).
//    * TfMainGrid      -- golden THeatTable* (htShullte0 / htShullte1).  Engine
//      calls SetCellColorIndex(col,row,idx) only inside the Motor-View page
//      guard (pgMain->ActivePageIndex==emp7TabSheet21, both 0 offline so equal --
//      but SetCellColorIndex is a harmless no-op so it is safe either way).
//    * TfMainMemoLines / TfMainMemo -- golden TMemo* (meShuttle1 / meShuttle2).
//      Engine calls ->Lines->Add / ->Lines->Count / ->Clear inside the
//      cbShowInShuttleSensor.Checked guard (false offline) -> no-op.
//    * TfMainInplace   -- golden cInplace (InArmPlacementEnable()/iNo9ShtErrICCt/
//      bNo9ShtErrNo).  Offline InArmPlacementEnable()==false makes the No9
//      sub-blocks inert (the AutoLatch checks still run their main path).
// ---------------------------------------------------------------------------
struct TfMainCheckBox { bool Checked; TfMainCheckBox():Checked(false){} };       // [DATA] golden TCheckBox*
struct TfMainGrid     { void SetCellColorIndex(int /*col*/,int /*row*/,int /*idx*/){} }; // [METHOD] golden THeatTable*
struct TfMainMemoLines{ int Count; void Add(AnsiString /*s*/){} TfMainMemoLines():Count(0){} }; // [DATA] golden TStrings*
struct TfMainMemo     { TfMainMemoLines *Lines; void Clear(){} TfMainMemo(){ Lines=new TfMainMemoLines(); } }; // [DATA] golden TMemo*
struct TfMainPageControl { int ActivePageIndex; TfMainPageControl():ActivePageIndex(0){} };      // [DATA] golden TPageControl* (pgMain)
class TfMainInplace
{
public:
    int  iNo9ShtErrICCt[2];          // [DATA]   golden cInplace -- per-shuttle No9 err count
    bool bNo9ShtErrNo[2][8];         // [DATA]   golden cInplace -- per-site No9 err flag
    bool InArmPlacementEnable();     // [METHOD] golden cInplace -- offline: false (No9 inert)
    TfMainInplace();
};

class TfMain
{
public:
    void DebugOneCycleHotPlate(AnsiString sfunc); // [METHOD] golden main.h:1565 -- debug log sink (empty)
    bool Pause(AnsiString Func);                  // [METHOD] golden main.h:1249 -- offline never pauses -> false
    void ShowTestHeadComp(bool bRefresh);         // [METHOD] golden main.h:1296 -- empty
    void ReStartAutoSiteMapping(bool bStart);     // [METHOD] golden main.h:1331 -- empty
    TfMainSiteMapLog *slAutoSiteMapLog;           // [DATA]   golden main.h:1486 (TMyStringList*) -- new in ctor
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs ----------
    void CleanOut(AnsiString Func);               // [METHOD] golden main.h -- offline: clean-out no-op
    void DoStateRecord(int i, bool b);            // [METHOD] golden main.h -- offline: state-record sink no-op
    TfMainTrayPanel *mtAuto1;                     // [DATA]   golden main.h (TPanel* mtAuto1)
    TfMainTrayPanel *mtAuto2;                     // [DATA]   golden main.h (TPanel* mtAuto2)
    TfMainTrayPanel *mtAuto3;                     // [DATA]   golden main.h (TPanel* mtAuto3)
    TfMainHanaART   *hanaART;                     // [DATA]   golden main.h (HANA ART helper)
    // -- W6.4 ADD: members the TESTER/INDEX ENGINE (atester.cpp) derefs ----------
    void LightOn();                               // [METHOD] golden main.h -- offline: CCD light no-op (DoTestHeadMotor CCD path)
    TfMainTrayPanel *lbCCDStatus;                 // [DATA]   golden main.h (TLabel* lbCCDStatus); reuse panel stub (->Visible via .Color)
    // -- W6.5 ADD: members the SHUTTLE ENGINE (acarry.cpp) derefs ----------------
    TfMainCheckBox *cbShowShuttleSensor;          // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false
    TfMainCheckBox *cbTestOutShuttleSensor;       // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false
    TfMainCheckBox *cbShowInShuttleSensor;        // [DATA]   golden main.h (TCheckBox*) -- offline Checked=false
    TfMainGrid     *htShullte0;                   // [DATA]   golden main.h (THeatTable* shuttle-1 grid)
    TfMainGrid     *htShullte1;                   // [DATA]   golden main.h (THeatTable* shuttle-2 grid)
    TfMainMemo     *meShuttle1;                   // [DATA]   golden main.h (TMemo* shuttle-1 log)
    TfMainMemo     *meShuttle2;                   // [DATA]   golden main.h (TMemo* shuttle-2 log)
    TfMainInplace  *cInplace;                     // [DATA]   golden main.h (cInplace placement helper)
    TfMainPageControl *pgMain;                    // [DATA]   golden main.h (TPageControl*) -- ActivePageIndex==0 offline
    int emp7TabSheet21;                           // [DATA]   golden main.h (Motor-View tab index) -- 0 offline (==pgMain->ActivePageIndex)
    void AddShuttleMessage(int iSht, AnsiString S);            // [METHOD] golden main.h -- offline log sink no-op
    void Reset(AnsiString Func);                              // [METHOD] golden main.h -- offline: no-op
    void BtnOneCycleClick(void *Sender);                      // [METHOD] golden main.h -- offline: no-op
    void BtnResetClick(void *Sender);                         // [METHOD] golden main.h -- offline: no-op
    void JSCC_ResetForShuttleLoseIC();                        // [METHOD] golden main.h -- offline: no-op (0-arg)
    void ResetRecordforPiggyBack(AnsiString S);               // [METHOD] golden main.h -- offline: no-op
    TfMain();
};
extern TfMain *fMain;

// ===========================================================================
//  TfSortCT -- non-VCL stub (golden cSortCT.h).  pnlHP1/pnlHP2 are TPanel* in
//  the golden; the leaves only assign ->Caption an AnsiString (HowManyIC()).
//  Stub each as a tiny {AnsiString Caption;} so `fSortCT->pnlHP1->Caption=...`
//  compiles.  Used inside `if(CosFunction.bShowHPICCount)` (default false).
// ===========================================================================
struct TfSortCTPanel { AnsiString Caption; };     // [DATA] golden TPanel* (cSortCT.h:61/63)
class TfSortCT
{
public:
    TfSortCTPanel *pnlHP1;
    TfSortCTPanel *pnlHP2;
    // -- W6.3 ADD: tray panels the TRAY-ARM ENGINE (acatchtray.cpp) derefs -------
    //    golden cSortCT.h TPanel* members; the catchtray engine only assigns
    //    ->Caption (AnsiString; HowManyIC() returns int -> AnsiString int-ctor).
    TfSortCTPanel *pnlLoad;                       // [DATA] golden cSortCT.h (loader IC-count panel)
    TfSortCTPanel *pnlLoadCID;                    // [DATA] golden cSortCT.h (loader cover-ID panel)
    TfSortCTPanel *pnlCoverTrayD;                 // [DATA] golden cSortCT.h (color cover-tray-ID panel)
    TfSortCTPanel *pnlTrayCnt[6];                 // [DATA] golden cSortCT.h (per-Auto tray-count panels)
    TfSortCT();
};
extern TfSortCT *fSortCT;

// ===========================================================================
//  TfLotInfo -- non-VCL stub (golden uLotInfo.h).  cbRunMode is TComboBox* in
//  the golden; the leaves deref ->Visible (bool) and ->Text.Pos("RT")
//  (AnsiString).  Stub cbRunMode as {bool Visible; AnsiString Text;}.
//  Used inside the iResetSiteMappingStep==2 ASM path (offline: Visible=false).
// ===========================================================================
struct TfLotInfoRunMode { bool Visible; AnsiString Text; TfLotInfoRunMode():Visible(false){} };
struct TfLotInfoLabel   { AnsiString Caption; };  // [DATA] golden uLotInfo.h (TLabel* labNowLoaderTrayID)
struct TfLotInfoEdit    { AnsiString Text; };     // [DATA] golden uLotInfo.h (TEdit*  edtSysLotID)
class TfLotInfo
{
public:
    TfLotInfoRunMode *cbRunMode;                  // [DATA] golden uLotInfo.h:307 (TComboBox*)
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs -----------
    TfLotInfoLabel   *labNowLoaderTrayID;         // [DATA] golden uLotInfo.h (loader tray-ID label)
    TfLotInfoEdit    *edtSysLotID;                // [DATA] golden uLotInfo.h (system lot-ID edit)
    void InitialUnLoaderTask(int iUnloader);      // [METHOD] golden uLotInfo.h -- offline: no-op (SOFT_SIMULTE AMR path)
    TfLotInfo();
};
extern TfLotInfo *fLotInfo;

// ===========================================================================
//  TfOffSet -- non-VCL stub (golden cOffSet.h).  Both methods return false
//  offline (no auto-offset / no setup-teach configured).
// ===========================================================================
class TfOffSet
{
public:
    bool UseAutoOffsetFunction(AnsiString sName);  // [METHOD] golden cOffSet.h:488 -> false
    bool UseInArmSetupTeach(int iWhich);           // [METHOD] golden cOffSet.h:489 -> false
};
extern TfOffSet *fOffSet;

// ===========================================================================
//  TfSCKART -- non-VCL stub (golden Automation/SCK_ART.h).  iInputJamCnt is
//  only incremented (inside `if(CosFunction.bUseSCKART)`, default false).
// ===========================================================================
class TfSCKART
{
public:
    int iInputJamCnt;                              // [DATA] golden SCK_ART.h:288
    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs -----------
    int iFTRTCount;                                // [DATA] golden SCK_ART.h (FT/RT count; CC_TERAPOWER ART)
    int iInputCount;                               // [DATA] golden SCK_ART.h (ART input count)
    int  CheckLoadingCount();                      // [METHOD] golden SCK_ART.h -- offline: 0 (no ART loading mismatch; golden compares ==0/==1/==2)
    // -- W6.5 ADD: member the SHUTTLE ENGINE (acarry.cpp) derefs ----------------
    void AddOutputJamCnt(int row, int col, int ret, int iBinOnCarryKit=0);  // [METHOD] golden Automation/SCK_ART.h:302 -- offline: no-op (inside if(bUseSCKART), default false)
    TfSCKART();
};
extern TfSCKART *fSCKART;

#endif // FormsFacadeH
