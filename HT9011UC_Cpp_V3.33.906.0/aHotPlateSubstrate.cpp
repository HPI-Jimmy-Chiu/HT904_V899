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
