// =============================================================================
//  aHotPlateSubstrate.cpp  --  W6.2 in-arm HotPlate substrate (offline sim bodies)
//
//  Translation wave: W6.2 (IN-ARM foundation -- shared HP pick/place geometry)
//  Translator: AI(W6.2-INARM) 20260626
//
//  Provides the offline definitions/bodies for the substrate declared in
//  aHotPlateSubstrate.h:
//    (a) the KitSuck grid objects + minimal TMyKitSuck / TMySucker bodies
//    (b) PickFromHPList (uPlateInfo) + uPoint2D, minimally
//    (c) the ainarm2 cursors / arrays + InArmSiteMapData + ZAxisDown
//    (d) the in-arm engine helper stubs (real bodies live in the later ainarm
//        core, W6.x/W7)
//    (e) tRotate / LaserCheckPos + TMyProductionRecord bodies the leaves call
//
//  All bodies are CONSERVATIVE offline defaults over the Sim HAL:
//    * vacuum Suck()/sensors report "no IC / not finished" (false)
//    * the team-list (PickFromHPList) is empty -> Get* return false, so the
//      geometry leaves take their "nothing to do" branch deterministically
//    * engine helpers that gate motion return "not finished" (false) / no-op
//  This is faithful to a handler with no HotPlate populated and is exactly the
//  surface the W6.2 canary asserts against.
//
//  Big5: Chinese comments preserved as UTF-8 (cp950).  NO U+FFFD.
// =============================================================================
#include "aHotPlateSubstrate.h"

//==============================================================================
//  (a) KitSuck grid objects (golden MyKitSuck.h:357-366) -- offline instances
//==============================================================================
TMyKitSuck InArmSuck;
TMyKitSuck FLCarryKit;
TMyKitSuck BLCarryKit;
TMyKitSuck OutArmSuck;
// -- W6.2c ADD: OUT-ARM KitSuck objects (golden MyKitSuck.h:359/361/366) -------
TMyKitSuck OutArm2Suck;
TMyKitSuck FRCarryKit;
TMyKitSuck BRCarryKit;

// ---- TMySucker bodies -------------------------------------------------------
//  Offline: no real vacuum line.  Suck() never reports "finished" (the leaves
//  treat that as "still building vacuum"); On()/Off() are no-ops.
bool TMySucker::Suck()    { return false; }
bool TMySucker::Destroy() { return false; }   // W6.2b: offline destroy never "finished" -> SM holds
void TMySucker::On()      {}
void TMySucker::Off()     {}
// -- W6.2c ADD: out-arm-touched TMySucker surface (golden MyKitSuck.h) ---------
//    Offline: no real vacuum line.  OnSuck/OnDestroy/OffDestroy/Normal are
//    solenoid no-ops; GetStatus() reports "no IC held" (false) so the out-arm
//    destroy-confirm SM (CheckOutArmDestroyActive case 300) takes its
//    "destroy finished" branch deterministically.
void TMySucker::OnSuck()     {}
void TMySucker::OnDestroy()  {}
void TMySucker::OffDestroy() {}
void TMySucker::Normal()     {}
bool TMySucker::GetStatus()  { return false; }   // offline: vacuum sensor reads "no IC"

// ---- TMyKitSuck bodies (only the called methods) ----------------------------
void TMyKitSuck::ResetAll() {}                              // golden :289 -- reset SuckTask

bool TMyKitSuck::HasIC()
{
    for(int i=0;i<_MAX_SUCK_ROW_ITEM;i++)
        for(int j=0;j<_MAX_SUCK_COL_ITEM;j++)
            if(Item[i][j]==HAS_IC || Item[i][j]==HAS_HOT_IC) return true;
    return false;
}

bool TMyKitSuck::HasType(int IC_TYPE)
{
    for(int i=0;i<_MAX_SUCK_ROW_ITEM;i++)
        for(int j=0;j<_MAX_SUCK_COL_ITEM;j++)
            if(Item[i][j]==IC_TYPE) return true;
    return false;
}

// Carry-kit presence leaves: this is the W6.0-gated "KitSuck grid" predicate
// surface (csystem_predicates.cpp HT9045_KITSUCK_GRID_AVAILABLE).  Offline =
// "no IC present" -> conservative.
bool TMyKitSuck::UseSiteHasIC()              { return false; }
bool TMyKitSuck::UseSiteNoIC()               { return true;  }   // no IC => "no IC at use sites" true
bool TMyKitSuck::LeftSideNoIC(int)           { return true;  }
bool TMyKitSuck::RightSideNoIC(int)          { return true;  }
bool TMyKitSuck::ArmUpSideNoIC()             { return true;  }
bool TMyKitSuck::ArmDownSideNoIC()           { return true;  }

void TMyKitSuck::SetItemData(int iRow, int iCol, int data, int /*iTarget*/)
{
    if(iRow<0||iRow>=_MAX_SUCK_ROW_ITEM||iCol<0||iCol>=_MAX_SUCK_COL_ITEM) return;
    Item[iRow][iCol]=data;
}

void TMyKitSuck::CopyFromTray(int iSuckR, int iSuckC, int iSuckData,
                              class TTrayMotor & /*Mot*/, int /*TrayR*/, int /*TrayC*/,
                              int /*iTrayData*/, int /*iTarget*/, bool /*bHP2Inarm*/)
{
    if(iSuckR<0||iSuckR>=_MAX_SUCK_ROW_ITEM||iSuckC<0||iSuckC>=_MAX_SUCK_COL_ITEM) return;
    Item[iSuckR][iSuckC]=iSuckData;                         // faithful: nozzle now carries iSuckData
}

void TMyKitSuck::CopyToTray(int iSuckR, int iSuckC, int iSuckData,
                            class TTrayMotor & /*Mot*/, int /*TrayR*/, int /*TrayC*/,
                            int /*iTrayData*/, int /*iTarget*/)
{
    if(iSuckR<0||iSuckR>=_MAX_SUCK_ROW_ITEM||iSuckC<0||iSuckC>=_MAX_SUCK_COL_ITEM) return;
    Item[iSuckR][iSuckC]=iSuckData;                         // faithful: nozzle cleared to iSuckData
}

// ---- W6.2b: members/methods the in-arm ENGINE SMs deref (golden MyKitSuck.h)
//      Offline: the grid is empty (no real vacuum line); predicates report
//      "no IC / finished" so the engine SMs take their deterministic path.
bool TMyKitSuck::HasRealIC()
{
    for(int i=0;i<_MAX_SUCK_ROW_ITEM;i++)
        for(int j=0;j<_MAX_SUCK_COL_ITEM;j++)
            if(Item[i][j]==HAS_IC || Item[i][j]==HAS_HOT_IC) return true;
    return false;
}
bool TMyKitSuck::NoIC()                { return !HasRealIC(); }
bool TMyKitSuck::IsPickSuckFinish()    { return true; }     // offline: nothing left to suck
bool TMyKitSuck::IsPickDestroyFinish() { return true; }     // offline: nothing left to destroy
bool TMyKitSuck::IsPickFinish()        { return true; }     // offline: pick cycle done
void TMyKitSuck::ClearAll()
{
    for(int i=0;i<_MAX_SUCK_ROW_ITEM;i++)
        for(int j=0;j<_MAX_SUCK_COL_ITEM;j++)
            Item[i][j]=NULL_IC;
}
void TMyKitSuck::SetAllToNullIC()      { ClearAll(); }       // golden :286

//==============================================================================
//  (e) TMyProductionRecord bodies the leaves call (Public/MyProductionRecord.h
//      is declaration-only; supply the four touched bodies as offline no-ops).
//==============================================================================
TMyProductionRecord::TMyProductionRecord() {}                       // golden ctor (Public/MyProductionRecord.h declares it; body deferred)
void TMyProductionRecord::AddErrorRecord(AnsiString, bool, int, int, int, int, int) {}
void TMyProductionRecord::AddHPRecord(int, int, int) {}
void TMyProductionRecord::AddInArmHotplatePickRecord(int, int) {}   //Sam 20200716
AnsiString TMyProductionRecord::GetInRotationAngRecord() { return ""; } //Sam 20221103

//==============================================================================
//  IsFLCarrKitAllHasIC / IsBLCarrKitAllHasIC -- EXPORTED by
//  ainarm_SearchPickPlate.h but DEFINED in the golden ainarm core
//  (ainarm9045.cpp / acarry.cpp, W6.x/W7).  Offline: the carry-kit grid has no
//  IC -> "not all has IC" is false.  AI(W6.2-INARM) 20260626.
//==============================================================================
bool IsFLCarrKitAllHasIC() { return false; }                       //ChungHung 20111230
bool IsBLCarrKitAllHasIC() { return false; }                       //ChungHung 20111230

//==============================================================================
//  (b) uPoint2D + uPlateInfo (PickFromHPList) -- empty offline team list
//==============================================================================
uPoint2D::uPoint2D()              : X(0), Y(0) {}
uPoint2D::uPoint2D(int x, int y)  : X(x), Y(y) {}
void uPoint2D::Clear()            { X=0; Y=0; }

uPlateInfo::uPlateInfo()  {}
uPlateInfo::~uPlateInfo() {}

// Offline: the HP team list is empty.  Every "get first team" returns false so
// the geometry SMs/leaves take the deterministic "no team to pick/place" path.
bool uPlateInfo::GetHPFirstTeam(int *iP, int (*)[MAX_ARM_Col],
                                int (*)[MAX_ARM_Col], bool (*)[MAX_ARM_Col])
{
    if(iP) *iP=0;
    return false;
}
bool uPlateInfo::GetHPFirstTeamPlate(int &iP, int &iR, int &iC, int &iSht, int &iKit)
{
    iP=0; iR=0; iC=0; iSht=0; iKit=0;
    return false;
}
bool uPlateInfo::GetHPFirstTeamSuckUse(bool [MAX_ARM_Row][MAX_ARM_Col]) { return false; }
bool uPlateInfo::GetHPFirstTeamMotUse(bool [MAX_ARM_Row][MAX_ARM_Col])  { return false; }
bool uPlateInfo::GetHPFirstTeamToList(int *iP, TList *) { if(iP) *iP=0; return false; }
bool uPlateInfo::DataForwardAndNextTeam() { return false; }
bool uPlateInfo::SetPlateSuck(int, int, int, bool) { return false; }
void uPlateInfo::SetArrPlateXY(int, int, int, int, int, int) {}
void uPlateInfo::SaveFile(AnsiString) {}

uPlateInfo  g_PickFromHPList;
uPlateInfo *PickFromHPList = &g_PickFromHPList;

AnsiString sHPPickRec          = "";
AnsiString sHPPickRecException = "";

//==============================================================================
//  (c) ainarm2.h cursors / arrays (golden ainarm2.h) -- definitions
//==============================================================================
const bool ZAxisDown = true;                               // golden ainarm2.h:42 (Z 下降)

int iPickPlate [2] = {0,0},  iPickPlateX [2] = {0,0},  iPickPlateY [2] = {0,0};
int iPlacePlate[2] = {0,0},  iPlacePlateX[2] = {0,0},  iPlacePlateY[2] = {0,0};

bool InArmSuckUse[MAX_ARM_Row][MAX_ARM_Col]          = {{false}};
bool bPickFromHotplate                               = false;
bool bInArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col] = {{false}};

int iHotCount = 0;                                         // golden ainarm2.h:77
int iHotPlateCount [2][50][50] = {{{0}}};
int iHotWhichKit   [2][50][50] = {{{0}}};
int iHotWhichShuttle[2][50][50]= {{{0}}};

bool bPitchOver12000 = false;                              // golden ainarm2.h:162

strAUTOSITEMAP InArmSiteMapData = { -1, -1, -1, -1, -1 };  //Steven 20211209 : 紀錄Site map資料

//==============================================================================
//  (e) Rotate / Laser micro-shims
//==============================================================================
tRotateShim tRotate = { false };

TInLaserCheck::TInLaserCheck(int _iP, int _iPlateC, int _iPlateR)
    : iP(_iP), iPlateC(_iPlateC), iPlateR(_iPlateR) {}
std::vector<TInLaserCheck*> LaserCheckPos;

//==============================================================================
//  per-site close-site-mode selectors (golden ainarm9045_2x6_8.h / _2x8_8.h)
//==============================================================================
int iCloseSiteModeFor2x6 = 0;       // e2x6Standard
int iCloseSiteModeFor2x8 = 0;

//==============================================================================
//  (A) [W6.2b] in-arm ENGINE cursors owned by ainarm2.cpp (not-yet-translated).
//      init to 1 (golden InitInArmTask()).  Defined here so ainarm9045.cpp's
//      central pick SM (`int &Task=iPickFromLoadStageTask`) and DoInArm_9045
//      (reads iArmTask) link.  Golden ainarm2.h:97/101.
//==============================================================================
int iArmTask               = 1;
int iPickFromLoadStageTask = 1;
bool bPickFromLoader       = false;     //golden ainarm2.h:54

//==============================================================================
//  (A2) [W6.2c] OUT-ARM ENGINE cursors owned by aoutarm2.cpp (not-yet-translated).
//      init to 1 (golden InitOutArmTask/InitPlaceToAutoTask/InitialFix3CanFullTask).
//      Defined here so aoutarm9045.cpp's DoOutArmPlaceToAuto_9045
//      (`int &Task=iPlaceToAutoTask`) and DoOutArmAfterPlaceToAuto link.
//      Golden aoutarm.h:49/15/51.
//==============================================================================
int iPlaceToAutoTask              = 1;
int iDoOutArmAfterPlaceToAutoTask = 1;
int iFix3CanFullTask              = 1;

//==============================================================================
//  (B) [W6.2b] per-variant close-site selector for 1x4 (golden ainarm9045_1x4_4.h)
//==============================================================================
int iCloseSiteModeFor1x4 = 0;       // e1x4Standard (offline: not closing 2 site)

//==============================================================================
//  (D) [W6.2b] fBarCode shim (golden BarCode.h TfBarCode) -- offline: no CCD,
//      every bottom-2DID scan reports "finished" so the additional-fn SM
//      advances rather than hangs.
//==============================================================================
void TfBarCode_Shim::InitBottom2DIDScan()       {}
bool TfBarCode_Shim::DoBottom2DIDScan()         { return true; }
bool TfBarCode_Shim::DoBottom2DID_8CCD_Scan()   { return true; }
static TfBarCode_Shim g_fBarCode;
TfBarCode_Shim *fBarCode = &g_fBarCode;

//==============================================================================
//  HP pick/place "part OK" / retry flags.  These are EXPORTED by
//  ainarm_SearchPickPlate.h but the golden DEFINES them in ainarm9045.cpp (the
//  in-arm core, W6.x/W7).  Define them here for W6.2 so the leaf modules link.
//  AI(W6.2-INARM) 20260626.  Golden homes: ainarm9045.cpp.
//==============================================================================
bool bPickFormHotplatePartOK         = false;   //ChungHung 20120112
bool bInArmToPickHotPlatePos         = false;   //ChungHung 20110323
bool bPickFormHotplateRetry          = false;   //Ifor 20160616
bool bInArmTryPickFromHotPlateFinish = false;   //ChungHung 20120206

//==============================================================================
//  (d) in-arm engine helper stubs (real bodies live in the later ainarm core,
//      W6.x/W7).  Offline defaults keep the geometry leaves linkable and the
//      canary deterministic.
//==============================================================================
//  [W6.2b PRE-STEP] 11 stubs REMOVED from here -- ainarm9045.cpp now REAL-defines
//  them (avoid ODR/link collision):
//    bUseAxExPicker, bUseAxxGPicker, InArmLeftSideHasIC, InArmLeftSideNoIC,
//    InspectInArmPosition, IsCheckInArmDestroyActiveFinish,
//    DoInArm_9045_SuckerMap, SetShuttleToHasNullIC_9045, AddInArmPickerCount,
//    GetInArmPitchX_9045, GetInArmPitchY_9045.
//  The remaining stubs below stay -- their real bodies live in not-yet-translated
//  modules (ainarm2.cpp / cmydef.cpp / motor speed ctrl, W7).
//==============================================================================
void ResetShuttleWhichKit() {}                             //golden ainarm2.h:133
int  CloseSiteState(bool /*bPlace*/) { return 0; }         // no site closed offline
void InitInArmTask()  {}
void SetRunStartMode(int) {}
void TransferHotPlateRatio(bool, int *, int *) {}          //Steven 20110324 : ratio xform no-op
bool MoveInArmZToPlateSafe(int) { return true; }           // offline: Z reaches safe immediately
void AdjustShuttlePlaceOrder(int) {}
void SetInArmHome() {}
void InArmSubSpeed() {}
void InArmAddSpeed() {}
void StopAllMotor() {}
void MyDBIProcess(AnsiString, AnsiString) {}
