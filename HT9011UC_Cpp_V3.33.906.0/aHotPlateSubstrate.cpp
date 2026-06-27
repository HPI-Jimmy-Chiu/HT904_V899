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
// -- W6.3 ADD: TRAY-ARM KitSuck object (golden MyKitSuck.h:367) -----------------
TMyKitSuck CatchTraySuck;
// -- W6.4 ADD: TESTER/INDEX KitSuck objects (golden MyKitSuck.h:363/364/368) -----
TMyKitSuck TestSocket;
TMyKitSuck FTestSuck;
TMyKitSuck BTestSuck;

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
// -- W6.3 ADD: tray-arm-touched TMySucker surface (golden MyKitSuck.h) ----------
//    Offline: Reset() clears the suck/destroy task (no-op over the Sim HAL).
//    Enable/OnAlarmTime are plain data members (default-init below by the object).
void TMySucker::Reset()      {}

// ---- W6.4 ADD: TMyKitSuck ctor (homes the tester decode grid) ----------------
//  Offline-safe init.  The golden TMyKitSuck has many more members; we only home
//  the ones the translated engines actually read.  iShtRow/iShtCol default to a
//  small 2-row x 1-col shuttle-site grid (any real load overwrites them via
//  cinitial), so the decode loops run a deterministic (small, in-bounds) pass and
//  the empty grid yields "no result yet" (-1).  iBinData/cDeviceInf/cSBin start
//  cleared; Item/iWhichSite are int grids left to the loader.
TMyKitSuck::TMyKitSuck()
{
    iShtRow = 2;                 // golden :163 -- default shuttle-site rows
    iShtCol = 1;                 // golden :164 -- default shuttle-site cols
    // W6.2b(2x4_16): pitch-step selectors the 2x4_16 SMs read (golden :167-170).
    iXStep = 1;                  // default 1x step (loader pulls overwrite via cinitial)
    iYStep = 1;
    iPickKitStep = 0;
    for(int i=0;i<_MAX_SUCK_ROW_ITEM;i++)
        for(int j=0;j<_MAX_SUCK_COL_ITEM;j++)
        {
            iBinData[i][j]  = -1;     // -1 = no decoded bin yet
            cDeviceInf[i][j]= "";
            cSBin[i][j]     = "";
            // W6.5: zero the shuttle clean-out grids the carry engine writes.
            iWhichAuto[i][j]  = 0;
            bPass[i][j]       = false;
            bNeedReTest[i][j] = false;
        }
}

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

// -- W6.4 ADD: tested-state predicates the test-cycle dispatcher (DoTestY case
//    50) derefs.  Faithful golden bodies (MyKitSuck.cpp :1663/:1685/:1701) -- the
//    AlreadyTest() side effect that clamps over-range bins to TEST_PASS+iTestBin
//    Count is preserved verbatim.
bool TMyKitSuck::AlreadyTest()                                                  // golden MyKitSuck.cpp:1663
{
    bool flag=false;                                                            //Steven 20180907 : 修正避免完全沒IC也回True
    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<iMaxCol; j++)
        {
            if(Item[i][j]>=TEST_PASS)                                           // && Item[i][j]<(TEST_PASS+iTestBinCount))//Steven 20200507 : 修正Test time out之後按SKIP,機台會hang up
            {
                flag=true;
            }

            if(Item[i][j]>(TEST_PASS+iTestBinCount-1) &&
               Item[i][j]!=(TEST_PASS+iTestBinCount))                           //Steven 20121112 : RS232支援32Bin 14->iTestBinCount-1, 1016->TEST_PASS+iTestBinCount
            {
                Item[i][j]=TEST_PASS+iTestBinCount;
            }
        }
    }
    return flag;
}
bool TMyKitSuck::AlreadyTestNotIncludeErrorBin()                                //Steven 20200611 : for Murata, 2DID NG不測試  golden :1685
{
    bool flag=false;
    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<iMaxCol; j++)
        {
            if(Item[i][j]>=TEST_PASS && Item[i][j]<(TEST_PASS+iTestBinCount))
            {
                flag=true;
            }
        }
    }
    return flag;
}
bool TMyKitSuck::PartAlreadyTest()                                              // golden MyKitSuck.cpp:1701
{
    bool flag1=false, flag2=false;

    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<iMaxCol; j++)
        {
            if(Item[i][j]==HAS_IC)
                flag1=true;

            if(Item[i][j]>=TEST_PASS)
                flag2=false;
        }
    }

    if(flag1 && flag2)
        return true;
    else
        return false;
}

// Carry-kit presence leaves: this is the W6.0-gated "KitSuck grid" predicate
// surface (csystem_predicates.cpp HT9045_KITSUCK_GRID_AVAILABLE).  Offline =
// "no IC present" -> conservative.
bool TMyKitSuck::UseSiteHasIC()              { return false; }
bool TMyKitSuck::UseSiteNoIC()               { return true;  }   // no IC => "no IC at use sites" true
// W6.2b1x1: FAITHFUL golden MyKitSuck.cpp:273 -- "all use-sites carry an IC"
// (true iff no NULL_IC over the iShtRow x iShtCol shuttle grid).
bool TMyKitSuck::UseSiteFullIC()                                                //Ifor 20161215
{
    for(int i=0; i<iShtRow; i++)
    {
        for(int j=0; j<iShtCol; j++)
        {
            if(Item[i][j]==NULL_IC)
            {
                return false;
            }
        }
    }
    return true;
}
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

// -- W6.2c(OUT-ARM) ADD: golden MyKitSuck.cpp:369 (Steven 20241017) verbatim.
//    Clears unused nozzles (beyond iShtRow/iShtCol) + maps HAS_NULL_IC->NULL_IC.
#ifndef HT9045_KITSUCK_SETUNUSE_BODY
#define HT9045_KITSUCK_SETUNUSE_BODY
void TMyKitSuck::SetUnuseAndHasNullICToNullIC()                                 //Steven 20241017 : 清除沒用到的料 與 HAS_NULL_IC的料
{
    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<iMaxCol; j++)
        {
            if(i>=iShtRow ||
               j>=iShtCol ||
               Item[i][j]==HAS_NULL_IC)
            {
                SetItemData(i, j, NULL_IC);
            }
        }
    }
}
#endif

// -- W6.2b(2x4_16) ADD: golden TMyKitSuck methods the 2x4_16 in-arm SMs call.
//    Guarded so a parallel sibling variant that adds the same body does not
//    produce a duplicate definition.
#ifndef HT9045_KITSUCK_2x4_16_BODIES
#define HT9045_KITSUCK_2x4_16_BODIES
void TMyKitSuck::SetAll(int Type)                                              // golden MyKitSuck.cpp ::SetAll
{
    for(int i=0; i<iMaxRow; i++)
        for(int j=0; j<iMaxCol; j++)
            SetItemData(i, j, Type);
}
bool TMyKitSuck::All_HasIC()                                                   // golden MyKitSuck.cpp ::All_HasIC (UI bLed/pLed gate dropped offline)
{
    for(int i=0; i<iMaxRow; i++)
        for(int j=0; j<iMaxCol; j++)
            if(Item[i][j]==NULL_IC)
                return false;
    return true;
}
void TMyKitSuck::SetType1ToType2ByPickCol(int Type1, int Type2)               // golden-by-name (2x4_16 dead-by-design; faithful remap)
{
    for(int i=0; i<iPickRow; i++)
        for(int j=0; j<iPickCol; j++)
            if(Item[i][j]==Type1)
                SetItemData(i, j, Type2);
}
#endif
// -- W6.3 ADD: tray-arm-touched TMyKitSuck predicates (golden MyKitSuck.h) ------
//    Offline: the tray-arm shuttle-side suck/destroy have no real vacuum line, so
//    both report "finished" -> DoCatchTray's early-out guard (golden :6017-6018)
//    takes its deterministic "not waiting on suck/destroy" branch.
bool TMyKitSuck::IsShtSuckFinish()     { return true; }      // golden :337
bool TMyKitSuck::IsShtDestroyFinish()  { return true; }      // golden :338

// -- W6.5 ADD: shuttle clean-out / realIC->interface-bin TMyKitSuck methods -----
//    FAITHFUL translations of the golden MyKitSuck.cpp bodies (NOT stubs): they
//    walk Item[i][j] over [iMaxRow][iMaxCol] -- both members already on this
//    substrate.  Offline the global carry-kits have iMaxRow==iMaxCol==0 (value-
//    initialized) so the loops naturally no-op: CountRealIC->0, HasDefineIC->false,
//    SetAllRealIC2InterfaceBin->no write -- the deterministic empty-grid result.
//    With a populated grid (a test seeding iMaxRow/iMaxCol+Item) the bodies behave
//    exactly as golden.
void TMyKitSuck::SetAllRealIC2InterfaceBin()                                    // golden MyKitSuck.cpp:SetAllRealIC2InterfaceBin
{
    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<iMaxCol; j++)
        {
            if(Item[i][j]!=NULL_IC && Item[i][j]!=HAS_NULL_IC)
            {
                SetItemData(i, j, TEST_PASS+iTestBinCount);                     //Steven 20150203 : Fixed for Sucker Status
                iBinData[i][j]=iTestBinCount;
                bPass[i][j]=false;
                bNeedReTest[i][j]=false;
            }
        }
    }
}
//------------------------------------------------------------------------------
int TMyKitSuck::CountRealIC()                                                   // golden MyKitSuck.cpp:CountRealIC
{
    int iHasNullICCount=0;
    int iNullICCount=0;
    int iOtherCount=0;
    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<iMaxCol; j++)
        {
            if(Item[i][j]==HAS_NULL_IC)
                iHasNullICCount++;
            else if(Item[i][j]==NULL_IC)
                iNullICCount++;
            else
                iOtherCount++;
        }
    }

    return iOtherCount;
}
//------------------------------------------------------------------------------
bool TMyKitSuck::HasDefineIC(int IC_TYPE)                                       //Steven 20130620 : 增加Function  (golden MyKitSuck.cpp:HasDefineIC)
{
    bool bHasDefineIC=false;
    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<iMaxCol; j++)
        {
            if(IC_TYPE==NULL_IC && Item[i][j]==IC_TYPE)                         //Steven 20160530 : fixed for HasDefineIC
            {
                bHasDefineIC=true;
            }
            else
            {
                if(Item[i][j]!=NULL_IC)
                {
                    if(Item[i][j]==IC_TYPE)
                    {
                        bHasDefineIC=true;
                    }
                }
            }
        }
    }
    return bHasDefineIC;
}

//==============================================================================
//  (e) TMyProductionRecord bodies the leaves call (Public/MyProductionRecord.h
//      is declaration-only; supply the four touched bodies as offline no-ops).
//==============================================================================
TMyProductionRecord::TMyProductionRecord() {}                       // golden ctor (Public/MyProductionRecord.h declares it; body deferred)
void TMyProductionRecord::AddErrorRecord(AnsiString, bool, int, int, int, int, int) {}
void TMyProductionRecord::AddHPRecord(int, int, int) {}
void TMyProductionRecord::AddInArmHotplatePickRecord(int, int) {}   //Sam 20200716
void TMyProductionRecord::AddTestRecord(int, int) {}               // W6.2b(2x4_16): golden dead method; offline no-op
AnsiString TMyProductionRecord::GetInRotationAngRecord() { return ""; } //Sam 20221103

//==============================================================================
//  IsFLCarrKitAllHasIC / IsBLCarrKitAllHasIC -- EXPORTED by
//  ainarm_SearchPickPlate.h but DEFINED in the golden ainarm core
//  (ainarm9045.cpp / acarry.cpp, W6.x/W7).
//  AI(W6.5-CARRY) 20260626: the W6.5 wave landed acarry.cpp, which carries the
//  REAL golden bodies (acarry.cpp:175 / :197).  Removed the W6.2 placeholder
//  stubs here to resolve the multiple-definition (the real bodies now own them).
//==============================================================================
#if 0 // W6.5: superseded by the real bodies translated into acarry.cpp (:175/:197)
bool IsFLCarrKitAllHasIC() { return false; }                       //ChungHung 20111230
bool IsBLCarrKitAllHasIC() { return false; }                       //ChungHung 20111230
#endif

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
void uPlateInfo::AddHPSuckGroup() {}                                        // W6.2b1x1: golden HTEditList.h:202 -- offline list no-op
void uPlateInfo::UpdateHPSuckGroup(int, int, int, int, int) {}             // W6.2b1x1: golden HTEditList.h:204 -- offline list no-op
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
//AI(W6.2c-INARM-batch3) 20260626: single definition of the ainarm2 debug latch
// (golden ainarm2.cpp:52).  Cleared by DoInArm_9045_2x3_6_14 / _2x4_4; offline
// default false, no hardware side effect.  extern in aHotPlateSubstrate.h.
bool bInArmHasHotIC = false;                               // golden ainarm2.cpp:52 (Sam 20211012)

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
//AI(W6.2c-INARM-batch4) 20260626: the placeholder DEFINITIONS of iCloseSiteModeFor2x6
// (was =0, e2x6Standard) and iCloseSiteModeFor2x8 (was =0, e2x8Standard) were REMOVED.
// The per-site variants ainarm9045_2x6_8.cpp (iCloseSiteModeFor2x6=e2x6Standard) and
// ainarm9045_2x8_8.cpp (iCloseSiteModeFor2x8=e2x8Standard) now own the REAL defs; the
// extern decls in aHotPlateSubstrate.h satisfy consumers (acarry.cpp / ainarm2.cpp).
// Values identical (both e*Standard==0) so numeric behavior is unchanged.
//==============================================================================
//  XPHSuckToSht_2x8_8_OutArm (golden aoutarm9045_2x8_8.h:7 / aoutarm9045_2x8_8.cpp:31,
//  JerryYang 20250711) -- OFFLINE home for the in-arm 2x8_8 family until the out-arm
//  2x8_8 wave lands.  Zero-init (the in-arm CheckSTMMode_2x8_8 writes the live cells
//  before any read; no read precedes a write offline).  When the out-arm file is
//  translated it OWNS this def and this offline one is removed.
//==============================================================================
int XPHSuckToSht_2x8_8_OutArm[e2x8ModeTotal][2][8] = {{{0}}};

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
//AI(W6.2c-INARM-batch3) 20260626: definition moved to ainarm9045_1x4_4.cpp:111
// (`int iCloseSiteModeFor1x4=e1x4Standard;`) -- that variant landed ACTIVE this
// batch and now OWNS the symbol.  Removed the duplicate substrate definition to
// avoid a multiple-definition link error; the extern in aHotPlateSubstrate.h:531
// still serves the substrate-only consumers (ainarm9045.cpp / inarm tests).

//==============================================================================
//  (D) [W6.2b] fBarCode shim (golden BarCode.h TfBarCode) -- offline: no CCD,
//      every bottom-2DID scan reports "finished" so the additional-fn SM
//      advances rather than hangs.
//==============================================================================
void TfBarCode_Shim::InitBottom2DIDScan()       {}
bool TfBarCode_Shim::DoBottom2DIDScan()         { return true; }
bool TfBarCode_Shim::DoBottom2DID_8CCD_Scan()   { return true; }

// -- W6.5 ADD: in/out-shuttle 2D-barcode + shuttle-float-check bodies the carry
//    engine derefs (golden BarCode/BarCode.h).  Offline: no CCD hardware, so the
//    Initial* are no-ops, the scan/trigger/CCD/float checks report false (nothing
//    detected), and IsSHT2DIDScanFinish reports true (scan considered complete) so
//    Do_Auto_SHT1/2 never parks forever on a 2DID that never arrives.
void TfBarCode_Shim::InitialBarcodeScanInShuttle1(bool /*bClear2DID*/)  {}      // golden :799
void TfBarCode_Shim::InitialBarcodeScanInShuttle2(bool /*bClear2DID*/)  {}      // golden :800
void TfBarCode_Shim::InitialBarcodeScanOutShuttle1()                    {}      // golden :801
void TfBarCode_Shim::InitialBarcodeScanOutShuttle2()                    {}      // golden :802
bool TfBarCode_Shim::DoBarcodeScanInShuttle_1(bool /*bErrorSkip*/) { return false; }  // golden :803
bool TfBarCode_Shim::DoBarcodeScanInShuttle_2(bool /*bErrorSkip*/) { return false; }  // golden :804
bool TfBarCode_Shim::DoBarcodeTriggerInShuttle_1()                 { return false; }  // golden :805
bool TfBarCode_Shim::DoBarcodeTriggerInShuttle_2()                 { return false; }  // golden :806
bool TfBarCode_Shim::DoBarcodeCCDInShuttle_1(bool /*bVerify*/)     { return false; }  // golden :807
bool TfBarCode_Shim::DoBarcodeCCDInShuttle_2(bool /*bVerify*/)     { return false; }  // golden :808
bool TfBarCode_Shim::DoBarcodeScanOutShuttle_1()                   { return false; }  // golden :810
bool TfBarCode_Shim::DoBarcodeScanOutShuttle_2()                   { return false; }  // golden :811
void TfBarCode_Shim::InitialShuttleFloatCheck1()                   {}                 // golden :888
void TfBarCode_Shim::InitialShuttleFloatCheck2()                   {}                 // golden :889
bool TfBarCode_Shim::DoShuttleFloatCheck_1()                       { return false; }  // golden :890
bool TfBarCode_Shim::DoShuttleFloatCheck_2()                       { return false; }  // golden :891
bool TfBarCode_Shim::IsSHT2DIDScanFinish(int /*SHT*/)              { return true;  }  // golden :942
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
// RowCanDualSite (golden ainarm2.cpp:1554, ChungHung 20150528).  W6.2c batch-4: the
// 2x2_8_Hot place-to-HP SM derefs it (GetPlaceHotPlate_4/_8/_8All).  The golden body
// reads TestIF.iTestMode/iUseSuckMode + ArmCanSuck4IC(0) + HotPlateForm.XDivision and
// returns false on every branch EXCEPT the ArmCanSuck4IC(0)==false early-out (which
// the Sim HAL does not model); 3 of 4 returns are false.  Offline: false (the
// dominant/faithful outcome -- no dual-site over the Sim Suck grid).
bool RowCanDualSite() { return false; }                    //golden ainarm2.cpp:1554
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

//==============================================================================
//  (E) [W6.2b1x1] ainarm2.h engine shims the in-arm per-site VARIANT SMs call.
//      Shared across every in-arm variant; defined ONCE here.  golden homes
//      cited per symbol.  Cursor-init + small row-scan helpers are FAITHFUL
//      translations; HAL/UI-bound bodies are conservative offline stubs whose
//      terminal value keeps the variant SM cursor flowing (never hangs).
//      AI(W6.2b-INARM-1x1_1) 20260626.
//==============================================================================

// -- data ---------------------------------------------------------------------
TMyKitSuck  ptrInSHTBackup;              // golden MyKitSuck.h:380
TMyKitSuck *ptrInSHT = &FLCarryKit;      // golden MyKitSuck.h:379 -- point at a LIVE grid so
                                         //   GetShuttleState_1x1_1's ptrInSHT->ArmUpSideAllTypeIC() is non-null offline.
TMyKitSuck  OutArmSuckBackup;            // golden MyKitSuck.h (SetInOutArmParameter_* target)
int  iInXPToSht[X_PITCH_COUNT]                = {0};   // golden ainarm2.h:36
int  iZPosToSht[MAX_ARM_Row][MAX_ARM_Col]     = {{0}}; // golden ainarm2.h:37
bool bZFlgToSht[MAX_ARM_Row][MAX_ARM_Col]     = {{false}}; // golden ainarm2.h:38
TQPF_Timer InArmReleaseDelayToHot;       // golden ainarm2.h:45
TQPF_Timer InArmReleaseDelay;            // golden ainarm2.h:46
TQPF_Timer MyInArmAtShuttleTimer;        // golden ainarm2.h:47
TQPF_Timer hInArmYpitchHomeTimer;        // golden ainarm2.h:212 (offline: TQPF_Timer, see .h note)
// -- W6.2c(OUT-ARM) ADD: out-arm sibling of hInArmYpitchHomeTimer.  Golden
//    aoutarm.h:213 (HTimer); exposed offline as TQPF_Timer (same Off()/
//    SetSecAndOn() surface), mirroring the in-arm precedent above.  Single
//    definition; the 6 out-arm variants forward-declare `extern TQPF_Timer`.
TQPF_Timer hOutArmYpitchHomeTimer;       // golden aoutarm.h:213 (offline: TQPF_Timer)
int  iBackInArmHotCount  = 0;            // golden ainarm2.h:76
bool InArmXMoveSafe      = false;        // golden ainarm2.h:83
bool bPlaceToShuttle2Step = false;       // golden ainarm2.h:56
int  iInArmPlaceToHotPlateTask    = 1;   // golden ainarm2.h (cursor; InitInArmPlaceToHotPlateTask sets 1)
int  iInArmPlaceToShuttleTask     = 1;   // golden ainarm2.h (cursor; InitInArmPlaceToShuttleTask sets 1)
int  iInArmTryPickFromHotPlateTask = 1;  // golden ainarm2.h (cursor; InitInArmTryPickFromHotPlateTask sets 1)
int  iBackupPlate = 0, iBackupPlateC = 0, iBackupPlateR = 0; // golden ainarm2.h (HP-check backup pos)
// flag1_1x2_2[2] (golden ainarm2.cpp:41 / ainarm2.h:9): plain process-state flag
// pair shared by the 1x2_2 / 1x2_2_14 / 1x2_4_Hot in-arm SMs (HotPlate XDivision==3
// path).  Offline-safe: zero-init, no hardware.  AI(W6.2c-INARM) 20260626.
bool flag1_1x2_2[2] = {false,false};                          // golden ainarm2.cpp:41

// -- TMyKitSuck methods (golden MyKitSuck.cpp) --------------------------------
// FAITHFUL: small HAL-free row scan (golden MyKitSuck.cpp:881).
bool TMyKitSuck::ArmUpSideAllTypeIC(int IC_TYPE, int iOffset, int iCol)         //Steven 20220930
{
    for(int j=0; j<iCol; j++)
    {
        if(Item[0][j+iOffset]!=IC_TYPE)
            return false;
    }
    return true;
}
// FAITHFUL: twin of ArmUpSideAllTypeIC, scans the down-side row Item[1][..]
// (golden MyKitSuck.cpp:971, Steven 20220930).  AI(W6.2c-INARM-batch2) 20260626.
bool TMyKitSuck::ArmDownSideAllTypeIC(int IC_TYPE, int iOffset, int iCol)       //Steven 20220930
{
    for(int j=0; j<iCol; j++)
    {
        if(Item[1][j+iOffset]!=IC_TYPE)
            return false;
    }
    return true;
}
//----------------------------------------------------------------------------
//AI(W6.2c-INARM-batch3) 20260626: golden MyKitSuck.cpp:514 (Steven 20221005).
// FAITHFUL per-row "has the specified IC?" query -- pure Item[iRow][*] scan, no
// HAL.  Added for GetShuttleState_1x4_4 / _2x3_6 / _All_1Pick (batch-3) which
// call InArmSuck.RowHasDefineIC(0/1, NULL_IC).  Body verbatim from golden.
bool TMyKitSuck::RowHasDefineIC(int iRow, int IC_TYPE)                          //Steven 20221005 : 針對上下排判斷有沒有指定的IC
{
    bool bHasDefineIC=false;
    for(int j=0; j<iMaxCol; j++)
    {
        if(IC_TYPE==NULL_IC && Item[iRow][j]==IC_TYPE)
        {
            bHasDefineIC=true;
        }
        else
        {
            if(Item[iRow][j]!=NULL_IC)
            {
                if(Item[iRow][j]==IC_TYPE)
                {
                    bHasDefineIC=true;
                }
            }
        }
    }

    return bHasDefineIC;
}
// FAITHFUL Item-grid side-scans (golden MyKitSuck.cpp:730/:768/:853/:943).
// Used by the 1x2_4_Hot in-arm place-to-shuttle SM (DoInArmPlaceToShuttle).
// Pure offline scan over the present Item grid -- no HAL.  Declared in
// aHotPlateSubstrate.h (left=true default lives there, NOT here).
// AI(W6.2c-INARM-1x2_4_Hot) 20260626.
bool TMyKitSuck::ArmLeftSideHaveRealIC(int MiddleValue)
{
    int iHasNullICCount=0;
    int iNullICCount=0;
    int iOtherCount=0;
    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<MiddleValue; j++)
        {
            if(Item[i][j*2]==HAS_NULL_IC)
                iHasNullICCount++;
            else if(Item[i][j*2]==NULL_IC)
                iNullICCount++;
            else
                iOtherCount++;
        }
    }

    if(iOtherCount)
        return true;
    else
        return false;
}
bool TMyKitSuck::ArmRightSideHaveRealIC(int MiddleValue)                        //For 1x2 & 2x2使用的
{
    int iHasNullICCount=0;
    int iNullICCount=0;
    int iOtherCount=0;

    for(int i=0; i<iMaxRow; i++)
    {
        for(int j=0; j<MiddleValue; j++)
        {
            if(Item[i][1+j*2]==HAS_NULL_IC)
                iHasNullICCount++;
            else if(Item[i][1+j*2]==NULL_IC)
                iNullICCount++;
            else
                iOtherCount++;
        }
    }

    if(iOtherCount)                                                             //JerryYang 20170703 (Steven) Fix 2x2_8 Hang up
        return true;
    else
        return false;
}
bool TMyKitSuck::ArmUpSideHaveRealIC(bool left)                                 //ChungHung 20130708 add left=true
{
    int iHasNullICCount=0;
    int iNullICCount=0;
    int iOtherCount=0;

    int offset=0;
    if(left)
        offset=0;
    else
        offset=4;

    for(int j=0; j<4; j++)
    {
        if(Item[0][j+offset]==HAS_NULL_IC)                                      //ChungHung 20130708 add left=true
            iHasNullICCount++;
        else if(Item[0][j+offset]==NULL_IC)                                     //ChungHung 20130708 add left=true
            iNullICCount++;
        else
            iOtherCount++;
    }

    if(iOtherCount)
        return true;
    else
        return false;
}
bool TMyKitSuck::ArmDownSideHaveRealIC(bool left)                               //ChungHung 20130708 add left=true;
{
    int iHasNullICCount=0;
    int iNullICCount=0;
    int iOtherCount=0;

    int offset=0;
    if(left)
        offset=0;
    else
        offset=4;

    for(int j=0; j<4; j++)
    {
        if(Item[1][j+offset]==HAS_NULL_IC)
            iHasNullICCount++;
        else if(Item[1][j+offset]==NULL_IC)
            iNullICCount++;
        else
            iOtherCount++;
    }

    if(iOtherCount)
        return true;
    else
        return false;
}
// FAITHFUL "NULL_IC -> HAS_NULL_IC" grid promotion (golden MyKitSuck.cpp:340-352,
// Steven 20150203 : Fixed for Sucker Status).  W6.2c batch-4: used by the 2x2_8_Hot
// in-arm place-to-shuttle SM (FLCarryKit/BLCarryKit, 4 sites).  Pure Item-grid scan
// over iShtRow/iShtCol -- no HAL.
void TMyKitSuck::SetNullIcToHasNullIc()
{
    for(int i=0; i<iShtRow; i++)
    {
        for(int j=0; j<iShtCol; j++)
        {
            if(Item[i][j]==NULL_IC)
            {
                SetItemData(i, j, HAS_NULL_IC);                                 //Steven 20150203 : Fixed for Sucker Status
            }
        }
    }
}
// FAITHFUL topology setter (golden MyKitSuck.cpp:206).
void TMyKitSuck::SetPickerCount(int _iPickRow, int _iPickCol, int _iShtRow, int _iShtCol, int _iPickStep, int _iKitStep, int _iShtStep)
{
    iPickRow    =_iPickRow;
    iPickCol    =_iPickCol;
    iShtRow     =_iShtRow;
    iShtCol     =_iShtCol;
    iShtCnt     =iShtRow*iShtCol;
    iPickStep   =_iPickStep;                                                    //Pitch倍數, 13吸嘴就寫2, 14吸嘴就寫3
    iPickKitStep=_iKitStep;
    iShtKitStep =_iShtStep;
}

// -- functions ----------------------------------------------------------------
// CopyInitSuck (golden MyKitSuck.cpp:1111): the golden body copies ~30 grid
// members; the offline substrate TMyKitSuck carries the subset the in-arm SMs
// actually read.  Conservative offline body: copy the present members so the
// OutArmSuck<->OutArmSuckBackup round-trip in SetInOutArmParameter_1x1_1 is
// data-faithful for those fields.
void CopyInitSuck(TMyKitSuck *Source, TMyKitSuck *Target, int SourceR, int SourceC, int TargetR, int TargetC)
{
    if(Source==0 || Target==0) return;
    Target->Item     [TargetR][TargetC] = Source->Item     [SourceR][SourceC];
    Target->iWhichSite[TargetR][TargetC]= Source->iWhichSite[SourceR][SourceC];
    Target->iBinData [TargetR][TargetC] = Source->iBinData [SourceR][SourceC];
    Target->bPass    [TargetR][TargetC] = Source->bPass    [SourceR][SourceC];
    Target->bNeedReTest[TargetR][TargetC]=Source->bNeedReTest[SourceR][SourceC];
    Target->cDeviceInf[TargetR][TargetC]= Source->cDeviceInf[SourceR][SourceC];
    Target->cSBin    [TargetR][TargetC] = Source->cSBin    [SourceR][SourceC];
}

// ResetInToShtFlag (golden ainarm2.cpp:124): zero the in->shuttle pitch/Z flags.
void ResetInToShtFlag()
{
    for(int i=0;i<X_PITCH_COUNT;i++) iInXPToSht[i]=0;
    for(int i=0;i<MAX_ARM_Row;i++)
        for(int j=0;j<MAX_ARM_Col;j++){ iZPosToSht[i][j]=0; bZFlgToSht[i][j]=false; }
}

// cursor-init helpers (golden ainarm2.cpp) -- FAITHFUL.
void InitInArmPickFromHotPlateTask340() { iInArmPickFromHotPlateTask=340; }     // golden :628
void InitInArmPickFromHotPlateTask50()                                          // golden :633
{
    if(bPickFormHotplateRetry==true)                                            //Ifor 20160616
        iInArmPickFromHotPlateTask=190;
    else
        iInArmPickFromHotPlateTask=50;
}
void InitInArmPlaceToShuttleTask()                                              // golden :647
{
    iInArmPlaceToShuttleTask=1;
    if(TestIF_File.UseRotateForHT7000HPKit==false)                              //Sam 20250428
        iInRotateFinish=2;                                                      //Ifor 20211220
}
void InitInArmPlaceToHotPlateTask()    { iInArmPlaceToHotPlateTask=1;   }       // golden :1539
void InitInArmPlaceToHotPlateTask400() { iInArmPlaceToHotPlateTask=400; }       // golden :1544
void InitInArmPlaceToHotPlateTask100() { iInArmPlaceToHotPlateTask=100; }       // golden :1549
void InitInArmTryPickFromHotPlateTask(){ iInArmTryPickFromHotPlateTask=1; }     // golden :613

// BackupPlacePos / RestorePlacePos (golden ainarm2.cpp:2185/:2192) -- FAITHFUL.
void BackupPlacePos()
{
    iBackupPlate =iPlacePlate[0];
    iBackupPlateC=iPlacePlateX[0];
    iBackupPlateR=iPlacePlateY[0];
}
void RestorePlacePos()
{
    iPlacePlate[0] =iBackupPlate;
    iPlacePlateX[0]=iBackupPlateC;
    iPlacePlateY[0]=iBackupPlateR;
}

// speed-display nudgers (golden ainarm2.cpp:2934/:2939) -- FAITHFUL.
void InArmAddSpeedDisplay() { iInArmSpeed1++; }                                 //KaiChen 20171225
void InArmSubSpeedDisplay() { iInArmSpeed1--; }                                 //KaiChen 20171225

// InitArmPickFromLoadStageTask (golden ainarm2.cpp:1016): the golden body resets
// the central pick cursor + several Loader-search statics (HAL-bound).  Offline:
// reset the cursor so the engine's DoInArmPickFromLoadStage_9045 SM restarts.
void InitArmPickFromLoadStageTask() { iPickFromLoadStageTask=1; }

// SetInArm_Unuse_SuckToNullICForHP (golden ainarm2.cpp:320): turns unused-site
// nozzles into NULL_IC before HP place.  Offline conservative no-op (the Sim grid
// is driven by the engine pick SM; nothing to reclassify offline).
void SetInArm_Unuse_SuckToNullICForHP() {}

// W6.2c: cross-variant in-arm shims with NO live golden home.  AI(W6.2c-INARM) 20260626.
// Zteach (golden InOutArmZteach.h): AutoTeach Z form.  Offline stand-in; fShow==false
// so the variant SMs take the non-teach path verbatim.  Single canonical def.
TfInOutArmZteach_Facade  g_ZteachFacade;
TfInOutArmZteach_Facade *Zteach = &g_ZteachFacade;                               // golden InOutArmZteach.h:Zteach
// DoCheckAutoSiteMappingPosition (golden ainarm2.cpp:2812): drives the JCET/ASE
// Auto-Site-Mapping step on a HW-bound site-map check.  Offline-safe no-op (no
// site-map HW; bRunAutoSiteMapping path is inert offline).
void DoCheckAutoSiteMappingPosition() {}
// CheckClearAllHotICThenPickLoadIC (golden ainarm2.cpp:533): true iff the HP grid
// still holds >= iLimit real ICs (so "clear all hot IC before picking load IC").
// Offline conservative: report not-satisfied so the SM takes the normal pick path.
bool CheckClearAllHotICThenPickLoadIC() { return false; }

// AdjustShuttleWhichKitOrder (golden ainarm2.cpp:849): re-orders which Shuttle/Kit
// the in-arm targets next.  Offline no-op (single-site: order is invariant).
void AdjustShuttleWhichKitOrder() {}

// AdjustShtOrderWhenPlaceToSht (golden ainarm2.cpp:772): post-place shuttle-order
// adjust.  Offline no-op (single-site).
void AdjustShtOrderWhenPlaceToSht(int /*iMode*/) {}

// SetInArmNeedDestory (golden ainarm2.cpp:3897): marks a nozzle as needing a
// destroy (blow-off) before/after place.  Offline no-op (no real vacuum line;
// TMySucker::Destroy() already reports "not finished" so the SM is consistent).
void SetInArmNeedDestory(bool /*bPlace*/, int /*iShtRow*/, int /*iShtCol*/, int /*iRow*/, int /*iCol*/) {}

// TransferInShuttleRatio (golden ainarm2.cpp:1972): in-shuttle software gear-ratio
// X/Y transform.  Offline: identity (gear ratio applies a 1:1 mapping when the
// ratio params are unset).  Pointers are caller-owned and non-null at the call site.
void TransferInShuttleRatio(int /*iSht*/, int * /*iXPos*/, int * /*iYPos*/, int /*iRow*/, int /*iCol*/) {}

// CheckInArmFloating (golden OmronLaser/LaserSensorInArm.h:30): laser float check.
// Offline: no laser -> report "finished/ok" (true) so the place SM advances.
bool CheckInArmFloating(bool /*bReset*/) { return true; }

// SetShuttlefCanMoveL (golden ainarm2.cpp:3910): set per-shuttle can-move-left
// interlock flag.  Offline: write the engine-visible MOT[] flag faithfully so the
// in-arm/shuttle handshake is consistent.
void SetShuttlefCanMoveL(int iShuttle, bool bCanMoveL, AnsiString /*sFun*/, AnsiString /*sTask*/)
{
    int mot = (iShuttle==0) ? MInShuttle1 : MInShuttle2;
    MOT[mot].fCanMoveL = bCanMoveL;
}

// InitInOCRWaitTask / OCRMoveInArm2XYToWait (golden ainarm2.cpp:3950/:3955): OCR
// tray-arm wait handshake.  Offline: no OCR -> the move "completes" immediately.
void InitInOCRWaitTask() {}
bool OCRMoveInArm2XYToWait() { return true; }

// IsHotPlateCheckFinsih (golden ainarm2.cpp:2199): scans the HP grids vs a
// site-mode count to decide whether the HP try-pick sweep is done.  The golden
// body reads HotPlateForm / TestIF.iTestMode / MOT[MMPlate*] (HAL+form bound).
// Offline conservative: report finished (true) so the try-pick sweep terminates
// rather than looping over a HotPlate that is never populated offline.
bool IsHotPlateCheckFinsih() { return true; }

// CheckInArmSuckInitial (golden ainarm2.cpp:1207): integrates all suck-init errors
// into one alarm; reads the vacuum sensors.  Offline: vacuum is idealized -> no
// init error -> report ready (true) so DoInArm_9045_1x1_1 case 10 proceeds.
bool CheckInArmSuckInitial() { return true; }

// SetInArmUseSuckToHasTrySuckIC (golden ainarm2.cpp:261): mark in-use nozzles
// HAS_TRY_SUCK_IC for the HotPlate try-pick.  Offline conservative: the single
// 1-picker path (the only one this single-site variant drives) sets nozzle (0,0).
void SetInArmUseSuckToHasTrySuckIC(int /*iSht*/, int /*iKit*/)
{
    InArmSuck.SetItemData(0, 0, HAS_TRY_SUCK_IC);                               //JerryYang 20251013 (offline subset of golden)
}

// DisableAutoSiteMapWhenCleanOut (golden ainarm2.cpp:2786): clears the auto-site-
// map run flag when Loader has no IC during clean-out.  Offline no-op (auto site
// map is not exercised offline).
void DisableAutoSiteMapWhenCleanOut() {}

// SetMotorSpeed (golden cinitial.cpp:5022): pushes the configured motor speed
// table to the controllers.  Offline no-op (no real controllers).
void SetMotorSpeed() {}

// DoInArmAutoSiteMapping (declared csystem.h:200): auto-site-map step pump.
// Offline: not running auto-site-map -> false (DoInArm_9045_1x1_1 falls through).
bool DoInArmAutoSiteMapping() { return false; }

// ZAxisNotDown (golden ainarm2.h:43): the "Z stays up" flag passed to
// MoveInArmXYToWaitTrayArm.  Sibling of ZAxisDown (already defined above).
const bool ZAxisNotDown = false;

// CheckInArmDestroyICFail (golden csystem.cpp / declared csystem.h:88): checks
// whether a blown-off (destroyed) IC is stuck.  Offline: no real vacuum -> no
// destroy failure -> report ok (true) so the place SM proceeds.
bool CheckInArmDestroyICFail() { return true; }
