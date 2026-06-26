// =============================================================================
//  aHotPlateSubstrate.h  --  W6.2 in-arm HotPlate substrate shim
//
//  Translation wave: W6.2 (IN-ARM foundation -- shared HP pick/place geometry)
//  Translator: AI(W6.2-INARM) 20260626
//
//  WHY THIS FILE EXISTS
//  --------------------
//  The two shared HP-geometry leaf modules translated this wave --
//  ainarm_SearchPickPlate.cpp (mirrors golden MyKitSuck.h + ainarm2.h + HTEditList.h)
//  ainarm_SearchPlacePlate.cpp
//  pump a substrate that the proven W6.0/W6.1 scaffold deliberately DEFERRED:
//
//    (a) TMyKitSuck InArmSuck / FLCarryKit / BLCarryKit / OutArmSuck
//        -- golden MyKitSuck.h:357.  This is exactly the "KitSuck grid"
//           csystem_predicates.cpp documents as the W6.x dependency
//           (HT9045_KITSUCK_GRID_AVAILABLE 0).  W6.2 is the wave that
//           introduces it.  Mirrored MINIMALLY: only the members these two
//           modules deref are declared (documented inline); offline bodies are
//           no-ops / conservative defaults over the Sim HAL.
//
//    (b) PickFromHPList (uPlateInfo*) -- golden Public/HTEditList.h:191/244.
//        The HP pick/place "team list".  Mirrored MINIMALLY to the called
//        surface (GetHPFirstTeam* / SetArrPlateXY / SetPlateSuck /
//        DataForwardAndNextTeam / GetHPFirstTeamToList / SaveFile).  Needs
//        uPoint2D (golden HTEditList.h:323) + uBasicPickPlace base.
//
//    (c) ainarm2.h cursors / arrays -- golden ainarm2.h:62-114:
//        iPickPlate[2]/iPickPlateX[2]/iPickPlateY[2]/iPlacePlate[2]/...,
//        iInArmPickFromHotPlateTask, iHotWhichShuttle/iHotWhichKit/
//        iHotPlateCount[2][50][50], InArmSuckUse[][], bPickFromHotplate,
//        InArmSiteMapData (strAUTOSITEMAP), bInArmCheckDestroyACT[][],
//        ZAxisDown.  Mirrored from the golden ainarm2.h declarations verbatim.
//
//    (d) extern decls (bodies live in the later ainarm core, W6.x/W7) for the
//        in-arm engine helpers the leaves call:
//        bUseAxExPicker / bUseAxxGPicker (golden ainarm9045.h),
//        CloseSiteState / InitInArmTask / SetRunStartMode /
//        TransferHotPlateRatio / InspectInArmPosition /
//        MoveInArmZToPlateSafe / IsCheckInArmDestroyActiveFinish /
//        DoInArm_9045_SuckerMap / SetShuttleToHasNullIC_9045 /
//        AdjustShuttlePlaceOrder / SetInArmHome / AddInArmPickerCount /
//        InArmSubSpeed / InArmAddSpeed / GetInArmPitchX_9045 /
//        GetInArmPitchY_9045 / GetInRotationAngRecord-on-record /
//        StopAllMotor / MyDBIProcess.
//        (InArmContinuousMove_9045 already has a W4 sim stub in Motor/mymotor.h.)
//        (DoAutoSiteMappingDropError already declared in csystem.h.)
//        Offline sim bodies are in aHotPlateSubstrate.cpp.
//
//    (e) Rotate / Laser micro-shims the place-swap leaf derefs:
//        tRotate.ActiveRotate (golden aRotateKIT.h), USE_ROTATE_KIT /
//        iRotate_Type / eInOutArm1Motor already in cmydef.h/MachineType.h,
//        TInLaserCheck + LaserCheckPos (golden -- laser-distance check list).
//
//  SCOPE: strictly the members the two modules deref.  No scope creep.  When the
//  real MyKitSuck.h / HTEditList.h / ainarm2.h / aRotateKIT.h land (W6.x/W7),
//  these shims are replaced and csystem_predicates.cpp may flip
//  HT9045_KITSUCK_GRID_AVAILABLE to 1.
//
//  Big5: every Chinese comment decoded cleanly via cp950 and is preserved as
//  UTF-8.  NO U+FFFD is emitted.
// =============================================================================
#ifndef aHotPlateSubstrateH
#define aHotPlateSubstrateH

#include "vclcompat/vcl_compat.h"   // AnsiString
#include "cmydef.h"                 // MAX_ARM_Row/Col, X_PITCH_COUNT, IC consts
#include "Motor/mymotor.h"          // class TTrayMotor (CopyFromTray/CopyToTray)
#include "Public/MyProductionRecord.h"  // TMyProductionRecord (golden by-value PordRec)
#include "myTimer.h"                // TQPF_Timer (W6.5: TMyKitSuck::TSoakTime)

// ---------------------------------------------------------------------------
//  TList  -- golden VCL Classes.hpp TList (pointer array).  Not in vcl_compat;
//  HasHotReadyIC_9045() does `new TList()` + ->Count / ->Items[i] / ->Clear().
//  MINIMAL mirror of the touched surface (Steven 20240311 list walk).
// ---------------------------------------------------------------------------
#ifndef HT9045_TLIST_SHIM
#define HT9045_TLIST_SHIM
#include <vector>
class TList
{
    std::vector<void*> v;
public:
    int    Count;                                   // VCL TList::Count (property)
    TList() : Count(0) { Items.pv=&v; }
    int    Add(void* p)     { v.push_back(p); Count=(int)v.size(); return Count-1; }
    void   Clear()          { v.clear(); Count=0; }
    // VCL spells element access `list->Items[i]`.  Provide an Items proxy so the
    // golden `(uPoint2D*)lsPoint2D->Items[i]` reads compile unchanged.
    struct ItemsProxy {
        const std::vector<void*>* pv;
        void* operator[](int i) const { return (*pv)[i]; }
    } Items;
};
#endif // HT9045_TLIST_SHIM

// ============================================================================
//  (a) TMyKitSuck grid  -- golden MyKitSuck.h:14/151
//      MINIMAL mirror: only the members ainarm_SearchPickPlate.cpp /
//      ainarm_SearchPlacePlate.cpp dereference.  _MAX_SUCK_ROW/COL kept = golden.
// ============================================================================
#define _MAX_SUCK_ROW_ITEM 4
#define _MAX_SUCK_COL_ITEM 8

// ---- TMySucker (golden MyKitSuck.h:14) -- only the touched surface ----------
class TMySucker
{
public:
    bool       Error;           // golden :57   -- pick/place vacuum error flag
    int        OnDelayTime;     // golden :64   -- HP vacuum on-delay (centi-sec)
    AnsiString sName;           // golden :158  -- nozzle position label
    // -- W6.2b-canary ADD: CheckInArmSuckICFallDownToHasNullIC (csystem.cpp,
    //    golden csystem.cpp:1677) reads .SenUsing!="" to decide whether this
    //    vacuum line has a sensor wired before drop-detecting.  golden
    //    MyKitSuck.h:81 verbatim.  Offline-safe: plain data, default empty ->
    //    the drop-detect loop skips every nozzle (no sensor configured).
    AnsiString SenUsing;        // golden :81   -- the vacuum sensor name this nozzle uses ("" = none)

    bool Suck();                // golden :89   -- vacuum ON / destroy OFF (returns "suck finished")
    bool Destroy();             // golden :90   -- destroy(blow) ON (returns "destroy finished")  // W6.2b: ProcessSCKARTLoadingCount case 1
    void On();                  // golden :91   -- vacuum solenoid ON
    void Off();                 // golden :92   -- vacuum solenoid OFF

    // -- W6.2c ADD: members the OUT-ARM ENGINE (aoutarm9045.cpp) derefs ----------
    //    golden MyKitSuck.h member names verbatim.  Added only because the
    //    out-arm destroy-confirm / safe-move / init-state code reads them.
    void OnSuck();              // golden :95   -- vacuum-on (suck) solenoid
    void OnDestroy();           // golden :97   -- destroy(blow) solenoid ON
    void OffDestroy();          // golden :98   -- destroy(blow) solenoid OFF
    void Normal();              // golden :99   -- return nozzle to idle/normal
    bool GetStatus();           // golden :103  -- read vacuum sensor (true=still holding)
    int  iMotNo;                // golden :138  -- the Z motor index for this nozzle
    int  iMyRow;                // golden :139  -- physical row of this nozzle
    int  iMyCol;                // golden :140  -- physical col of this nozzle

    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs ----------
    //    golden MyKitSuck.h member names verbatim.  CatchTraySuck.Suck[0][0] reads
    //    .Enable (whether this vacuum line is configured) + .OnAlarmTime (vacuum-on
    //    alarm window, centi-sec) and calls Reset() (golden MyKitSuck.h).
    bool Enable;                // golden :55   -- vacuum line configured/installed
    int  OnAlarmTime;           // golden :63   -- vacuum-on alarm window (centi-sec)
    void Reset();               // golden :94   -- reset this nozzle's suck/destroy task

    // -- W6.2b(2x4_16) ADD: the in-arm place-to-shuttle SM reads the per-nozzle
    //    "needs destroy(blow-off)" flag (golden MyKitSuck.h:120/124, inline body).
    //    Offline-safe: the flag is plain data, default-false; no HW touched.
#ifndef HT9045_SUCKER_NEEDDESTROY_ADDED
#define HT9045_SUCKER_NEEDDESTROY_ADDED
    bool bNeedDestroy;          // golden MyKitSuck.h:120
    bool GetNeedDestroyStatus() { return bNeedDestroy; }   // golden MyKitSuck.h:124
#endif
};

// ---- TMyKitSuck (golden MyKitSuck.h:151) -- MINIMAL mirror ------------------
//  PordRec[][] is TMyProductionRecord (Public/MyProductionRecord.h); the leaves
//  call AddErrorRecord / AddInArmHotplatePickRecord / AddHPRecord /
//  GetInRotationAngRecord on it.
class TMyKitSuck
{
public:
    // data grids the leaves read/write (golden member names verbatim)
    TMySucker           Suck   [_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :155
    int                 Item   [_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :180
    int                 iWhichSite[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :188
    TMyProductionRecord PordRec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :246

    // -- W6.2b ADD: members the in-arm ENGINE (ainarm9045.cpp) SMs deref --------
    //    golden MyKitSuck.h member names verbatim.  Added only because the
    //    engine task SMs / dispatch ladder read them; offline-safe defaults.
    int  iMaxRow;               // :159  (DoInArm_9045 guard `i<InArmSuck.iMaxRow`)
    // additional-function SM (DoInArmAdditionalFunction) "already done" flags
    bool bAlreadyDieClean;      // :220  (Die Clean done)
    bool bAlreadyPreciser;      // :221  (Preciser done)
    bool bAlready2DID;          // :222  (Bottom 2DID done)
    bool bAlreadyRotate;        // :223  (Rotate done)
    // pick SM / DoInArm_9045 predicates
    bool HasRealIC();           // :312  (any nozzle carries a real IC)
    bool NoIC();                // :316  (no nozzle carries any IC)
    bool IsPickSuckFinish();    // :360  (all pick-suck done)
    bool IsPickDestroyFinish(); // :362  (all pick-destroy done)
    bool IsPickFinish();        // :364  (pick cycle finished)
    void ClearAll();            // :290  (clear the whole grid)
    void SetAllToNullIC();      // :286  (set every nozzle to NULL_IC)

    // scalar topology / shuttle-kit selectors (golden :167-205)
    int  iMaxCol;               // :160
    int  iModeX;                // :169
    int  iPickRow;              // :161
    int  iPickCol;              // :162
    int  iShtRow;               // :163
    int  iShtKitStep;           // :166
    int  iWhichSht;             // :200
    int  iWhichKit;             // :201
    int  iWhichShtPickFor32;    // :203
    int  iWhichKitPickFor32;    // :204
    int  HotCount;              // :205
    int  iWhichShuttleBackup;   // :234
    int  iWhichKitBackup;       // :235

    // -- W6.2c ADD: members the OUT-ARM ENGINE (aoutarm9045.cpp) derefs ----------
    //    golden MyKitSuck.h member names verbatim.  Added only because the
    //    out-arm safe-move / cell-pos / additional-fn SM read them.
    int  iMotRow;               // :156  (MoveOutArmToAutoSafe_9045 motor-grid walk)
    int  iMotCol;               // :157  (MoveOutArmToAutoSafe_9045 motor-grid walk)
    int  iPickStep;             // :166  (GetOutArmToShtCellPos col step)
    bool bAlreadyAOI;           // :351  (DoOutArmAdditionalFunction AOI done flag)
    bool bAlreadyFixAI;         // :353  (DoOutArmAdditionalFunction FixAI done flag)

    // -- W6.3 ADD: members the TRAY-ARM ENGINE (acatchtray.cpp) derefs ----------
    //    golden MyKitSuck.h member names verbatim.  CatchTraySuck.iWhichTray (15x:
    //    記錄Tray從哪來的) + IsShtSuckFinish()/IsShtDestroyFinish() (DoCatchTray
    //    early-out guard, golden acatchtray.cpp:6017-6018).
    int  iWhichTray;            // :194  (which tray this kit picked: 0 loader/1 empty/2 color/4 auto2)
    bool IsShtSuckFinish();     // :337  (shuttle-side suck finished -- offline true)
    bool IsShtDestroyFinish();  // :338  (shuttle-side destroy finished -- offline true)

    // -- W6.4 ADD: members the TESTER/INDEX ENGINE (atester.cpp) derefs ----------
    //    golden MyKitSuck.h member names verbatim.  The result-decode consumer
    //    (GetTesterResult / ProcessTestResult / CheckIndexArmInitState) walks the
    //    shuttle-site result grid via iShtRow/iShtCol (iShtRow already above) and
    //    stores decoded bin / barcode / sub-bin into iBinData / cDeviceInf / cSBin.
    int  iShtCol;               // :164  (decode loop column bound)
    int  iBinData [_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];  // :189  (decoded bin number)
    AnsiString cDeviceInf[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :228 (per-site 2DID/barcode)
    AnsiString cSBin     [_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :230 (per-site software-bin label)

    // -- W6.5 ADD: members the SHUTTLE/CARRY ENGINE (acarry.cpp) derefs ----------
    //    golden MyKitSuck.h member names verbatim.  The shuttle clean-out /
    //    realIC->interface-bin conversion (Do_Auto_SHT1/2 + DoCheckShuttle*EmptyIC)
    //    writes iWhichAuto/bPass/bNeedReTest, reads the soak timer TSoakTime, and
    //    calls SetAllRealIC2InterfaceBin/CountRealIC/HasDefineIC.  All grids
    //    offline-zeroed in the ctor; the methods carry the FAITHFUL golden bodies
    //    (they touch only iMaxRow/iMaxCol/Item/iBinData -- all present here).
    int  iWhichAuto  [_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :178 (per-site Auto-bin target)
    bool bPass       [_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :187 (per-site pass flag)
    bool bNeedReTest [_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; // :188 (per-site retest flag)
    TQPF_Timer  TSoakTime;      // :242  (Index Pick up need wait Soak Time)
    void SetAllRealIC2InterfaceBin();   // :276  (faithful golden body)
    int  CountRealIC();                 // :315  (faithful golden body)
    bool HasDefineIC(int IC_TYPE);      // :297  (faithful golden body)

    TMyKitSuck();               // ctor: home iShtRow/iShtCol + init grids offline-safe

    // methods the leaves call (golden signatures preserved) -------------------
    void ResetAll();                                                    // :289
    bool HasIC();                                                       // :310
    bool HasType(int);                                                  // :324
    bool AlreadyTest();                                                 // :318 (W6.4: DoTestY case 50)
    bool AlreadyTestNotIncludeErrorBin();                              // :319 (W6.4)
    bool PartAlreadyTest();                                            // :320 (W6.4: DoTestY case 50)
    bool UseSiteHasIC();                                               // :340 (=W6.0 gated predicate leaf)
    bool UseSiteNoIC();                                                // :341
    bool UseSiteFullIC();                                             // :339 (W6.2b1x1: FAITHFUL -- iShtRow/iShtCol scan)
    bool LeftSideNoIC(int MiddleValue);                                // :349
    bool RightSideNoIC(int MiddleValue);                               // :348
    bool ArmUpSideNoIC();                                              // :352
    bool ArmDownSideNoIC();                                            // :353
    // -- W6.2b1x1 ADD: members the in-arm 1x1_1 variant SMs deref.  golden
    //    MyKitSuck.h names verbatim.  ArmUpSideAllTypeIC (golden :334) is a small
    //    HAL-free row scan -> FAITHFUL body.  SetPickerCount(7-arg, golden :250)
    //    is a topology setter -> FAITHFUL body (writes the present scalar members;
    //    iShtCnt/iPickKitStep added below because the golden body assigns them).
    bool ArmUpSideAllTypeIC(int IC_TYPE, int iOffset, int iCol);       // :334 (Steven 20220930)
    void SetPickerCount(int _iPickRow, int _iPickCol, int _iShtRow,
                        int _iShtCol, int _iPickStep, int _iKitStep,
                        int _iShtStep);                                // :250
    int  iShtCnt;                // :165 (=iShtRow*iShtCol; SetPickerCount writes it).
                                 //       iPickKitStep already declared below (W6.2b 2x4_16).
    void SetItemData(int iRow, int iCol, int data, int iTarget=-1);    // :283
    void CopyFromTray(int iSuckR, int iSuckC, int iSuckData, class TTrayMotor &Mot,
                      int TrayR, int TrayC, int iTrayData, int iTarget=-1,
                      bool bHP2Inarm=false);                            // :276
    void CopyToTray  (int iSuckR, int iSuckC, int iSuckData, class TTrayMotor &Mot,
                      int TrayR, int TrayC, int iTrayData, int iTarget=-1); // :277

    // -- W6.2b(2x4_16) ADD: golden TMyKitSuck members the 2x4_16 in-arm SMs deref.
    //    All are genuine golden MyKitSuck.h API (cited per line); guarded so a
    //    parallel sibling variant editing this header does not double-declare.
#ifndef HT9045_KITSUCK_2x4_16_ADDED
#define HT9045_KITSUCK_2x4_16_ADDED
    int  iXStep;                // golden MyKitSuck.h:169 (X pitch 要設幾格)
    int  iYStep;                // golden MyKitSuck.h:170 (Y pitch 要設幾格)
    int  iPickKitStep;          // golden MyKitSuck.h:167
    void SetAll(int Type);      // golden MyKitSuck.h:274 / .cpp -- set every nozzle to Type
    bool All_HasIC();           // golden MyKitSuck.h:307 / .cpp -- every used nozzle has IC
    // golden has the type1->type2 remap; in golden it is referenced only by the
    // 2x4_16 family and never defined (dead-by-design); we supply the obvious
    // faithful body (remap Item==Type1 -> Type2 over the pick grid) offline-safe.
    void SetType1ToType2ByPickCol(int Type1, int Type2); // golden-by-name (2x4_16-local)
#endif

    // -- W6.2c(1x2_4_Hot) ADD: golden TMyKitSuck "have real IC" side-scan API the
    //    1x2_4_Hot in-arm place-to-shuttle SM derefs (golden mykitsuck.cpp).  All
    //    are genuine golden API (cited per line); guarded so a parallel sibling
    //    variant editing this header does not double-declare.  FAITHFUL bodies are
    //    in aHotPlateSubstrate.cpp (Item-grid scans only -- no HAL).
#ifndef HT9045_KITSUCK_HAVEREALIC_ADDED
#define HT9045_KITSUCK_HAVEREALIC_ADDED
    bool ArmUpSideHaveRealIC(bool left=true);    // golden mykitsuck.cpp (ChungHung 20130708 left=true)
    bool ArmDownSideHaveRealIC(bool left=true);  // golden mykitsuck.cpp (ChungHung 20130708 left=true)
    bool ArmLeftSideHaveRealIC(int MiddleValue); // golden mykitsuck.cpp
    bool ArmRightSideHaveRealIC(int MiddleValue);// golden mykitsuck.cpp (For 1x2 & 2x2)
#endif
};

extern TMyKitSuck InArmSuck;     // golden MyKitSuck.h:357
extern TMyKitSuck FLCarryKit;    // golden MyKitSuck.h:358
extern TMyKitSuck BLCarryKit;    // golden MyKitSuck.h:360
extern TMyKitSuck OutArmSuck;    // golden MyKitSuck.h:366
// -- W6.2c ADD: the OUT-ARM KitSuck objects the out-arm engine SMs read --------
extern TMyKitSuck OutArm2Suck;   // golden MyKitSuck.h:366 (HT-9046AU sort arm; DoOutArmAfterPlaceToAuto case 5000)
extern TMyKitSuck FRCarryKit;    // golden MyKitSuck.h:359 (DoOutArmIonFanGiveWay FRCarryKit.HasIC())
extern TMyKitSuck BRCarryKit;    // golden MyKitSuck.h:361 (DoOutArmIonFanGiveWay BRCarryKit.HasIC())
// -- W6.3 ADD: the TRAY-ARM KitSuck object the catchtray engine SMs read --------
extern TMyKitSuck CatchTraySuck; // golden MyKitSuck.h:367 (TrayArm dual-suck grid; DoCatchTray/DoCatchFromLoader/...)
// -- W6.4 ADD: the TESTER/INDEX KitSuck objects the tester engine SMs read -------
extern TMyKitSuck TestSocket;    // golden MyKitSuck.h:368 (the test-result grid; decode/ProcessTestResult target)
extern TMyKitSuck FTestSuck;     // golden MyKitSuck.h:363 (front test-head suck grid)
extern TMyKitSuck BTestSuck;     // golden MyKitSuck.h:364 (rear  test-head suck grid)

// ============================================================================
//  (b) uPlateInfo (PickFromHPList)  -- golden HTEditList.h
//      uPoint2D is ALREADY declared in cmydef.h (W0-TAIL stub, golden HTEditList.h
//      :323, same {int X; int Y;} layout); its ctor/Clear bodies were deferred --
//      aHotPlateSubstrate.cpp supplies them for W6.2.  Do NOT redeclare it here.
// ============================================================================
// uPlateInfo: MINIMAL mirror -- only the called methods (golden HTEditList.h:244)
class uPlateInfo
{
public:
    uPlateInfo();
    ~uPlateInfo();
    // 取得第一筆資料的全部吸嘴資料
    bool GetHPFirstTeam(int *iP, int (*PlateR)[MAX_ARM_Col],
                        int (*PlateC)[MAX_ARM_Col], bool (*Suck)[MAX_ARM_Col]);
    // 取得第一筆資料的加熱盤座標與 Shuttle 位置
    bool GetHPFirstTeamPlate(int &iP, int &iR, int &iC, int &iSht, int &iKit);
    // 取得第一筆資料中, 吸嘴資料是否需要交換
    bool GetHPFirstTeamSuckUse(bool bSuck[MAX_ARM_Row][MAX_ARM_Col]);
    // 取得第一筆資料中, 吸嘴 Z 軸使用與否的資料
    bool GetHPFirstTeamMotUse(bool bSuck[MAX_ARM_Row][MAX_ARM_Col]);
    // 取得第一筆資料, 並轉換成陣列
    bool GetHPFirstTeamToList(int *iP, TList* lsPoint2D);
    // 確認是否需要多次吸料, 並刪除舊資料
    bool DataForwardAndNextTeam();
    // 編輯已儲存資料
    bool SetPlateSuck(int _iP, int _iRow, int _iCol, bool bSuck);
    void SetArrPlateXY(int iSuckRow, int iSuckCol, int _iP, int iPlateR,
                       int iPlateC, int _iSite=-1);
    void SaveFile(AnsiString sFileName);
    // -- W6.2b1x1 ADD: HP-suck-group recorders the in-arm HP-place SM
    //    (DoPlaceToHotPlate_9045_1x1_1) calls (golden HTEditList.h:202/204).
    //    Offline: list bookkeeping no-ops (the placed-group record is not
    //    consumed offline; the SM only needs the calls to be linkable/no-throw).
    void AddHPSuckGroup();                                              // golden HTEditList.h:202
    void UpdateHPSuckGroup(int iP, int iR, int iC, int iSht, int iKit); // golden HTEditList.h:204
};

extern uPlateInfo *PickFromHPList;   // golden HTEditList.h:294

extern AnsiString sHPPickRec;            // golden HTEditList.h:16 (used in SOFT_SIMULTE-gated SaveFile)
extern AnsiString sHPPickRecException;   // golden HTEditList.h:17

// ============================================================================
//  (c) ainarm2.h cursors / arrays  -- golden ainarm2.h
// ============================================================================
extern const bool ZAxisDown;                                            // golden ainarm2.h:42

extern int iPickPlate[2],  iPickPlateX[2],  iPickPlateY[2];             // golden ainarm2.h:62
extern int iPlacePlate[2], iPlacePlateX[2], iPlacePlateY[2];           // golden ainarm2.h:63

extern bool InArmSuckUse[MAX_ARM_Row][MAX_ARM_Col];                    // golden ainarm2.h:67
extern bool bPickFromHotplate;                                          // golden ainarm2.h:53
extern bool bInArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col];           // golden ainarm2.h:39
// W6.2c ADD: 1x2_* HotPlate XDivision==3 flag pair shared by 1x2_2/1x2_2_14/
// 1x2_4_Hot in-arm SMs.  Single canonical decl (def in aHotPlateSubstrate.cpp);
// guarded so a sibling variant's local extern is harmless.  AI(W6.2c-INARM) 20260626.
#ifndef HT9045_flag1_1x2_2_DECLARED
#define HT9045_flag1_1x2_2_DECLARED
extern bool flag1_1x2_2[2];                                            // golden ainarm2.cpp:41 / ainarm2.h:9
#endif

extern int iHotCount;                                                   // golden ainarm2.h:77 (同一次 Shuttle 的 IC 計數)
extern int iHotPlateCount  [2][50][50];                                 // golden ainarm2.h:78
extern int iHotWhichKit    [2][50][50];                                 // golden ainarm2.h:83
extern int iHotWhichShuttle [2][50][50];                                // golden ainarm2.h:84

extern bool bPitchOver12000;                                            // golden ainarm2.h:162 (jou 20100120)

// strAUTOSITEMAP (golden ainarm2.h:10) + InArmSiteMapData (golden ainarm2.h:34)
typedef struct                                                          //Sam 20181201 : Auto Alignment
{
    int iP;
    int iPlateR;
    int iPlateC;
    int iSuckR;
    int iSuckC;
    void ClearData()
    {
        iP=-1;
        iPlateR=-1;
        iPlateC=-1;
        iSuckR=-1;
        iSuckC=-1;
    };
    void UpdateData(int _iP, int _iPlateR, int _iPlateC, int _iSuckR, int _iSuckC)
    {
        iP      =_iP;
        iPlateR =_iPlateR;
        iPlateC =_iPlateC;
        iSuckR  =_iSuckR;
        iSuckC  =_iSuckC;
    };
}strAUTOSITEMAP;
extern strAUTOSITEMAP InArmSiteMapData;                                 //Steven 20211209 : 紀錄Site map資料

// ============================================================================
//  (e) Rotate / Laser micro-shims  -- golden aRotateKIT.h + laser-distance list
// ============================================================================
//  tRotate.ActiveRotate -- the only member the place-swap leaf reads.
struct tRotateShim                                                      // golden RotateKit/fRotate.h (TRotate tRotate)
{
    bool ActiveRotate;
    // -- W6.2b1x1 ADD: members the in-arm SuckerMap (DoInArm_9045_1x1_1_SuckerMap)
    //    reads for rotator DUT-data routing.  golden RotateKit/fRotate.h:33/51.
    int  RotateDutDate[4][4][8];                                        // golden :33 (0:In 1:Out 2:In RT 3:Out RT)
    bool bRotateUseRTmode;                                              // golden :51 (jou 20231122)
};
extern tRotateShim tRotate;

//  TInLaserCheck + LaserCheckPos -- golden laser-distance check list.
//  Place-swap leaf only does LaserCheckPos.push_back(new TInLaserCheck(iP,iC,iR)).
class TInLaserCheck
{
public:
    TInLaserCheck(int iP, int iPlateC, int iPlateR);
    int iP;
    int iPlateC;
    int iPlateR;
};
#include <vector>
extern std::vector<TInLaserCheck*> LaserCheckPos;

// ============================================================================
//  (d) in-arm engine helper externs  (bodies live in the later ainarm core,
//      W6.x/W7; offline sim bodies are in aHotPlateSubstrate.cpp)
// ============================================================================
// These overlap with ainarm9045.h (the engine's public header, where they now
// have REAL bodies + default args).  When ainarm9045.h is already included
// (e.g. in ainarm9045.cpp) skip the redeclaration so the default args are not
// given twice.  The already-translated leaves include only this header and rely
// on the InArmLeftSide* default arg (=2), so keep the decls for them.
#ifndef ainarm9045H
extern bool bUseAxExPicker();                                           //golden ainarm9045.h
extern bool bUseAxxGPicker();                                           //golden ainarm9045.h
extern bool InArmLeftSideHasIC(int iRow=2);                             //golden ainarm9045.h:109
extern bool InArmLeftSideNoIC(int iRow=2);                              //golden ainarm9045.h:108
#endif
extern void ResetShuttleWhichKit();                                     //golden ainarm2.h:133 (Steven 20140710)

// per-site close-site-mode selectors referenced by HotPlateYPitchCanPutAll().
// Golden homes: ainarm9045_2x6_8.h / ainarm9045_2x8_8.h (per-site modules, W6.x/W7).
// e2x8OneByOne already lives in target MachineType.h; only these are missing:
extern int iCloseSiteModeFor2x6;        //golden ainarm9045_2x6_8.h:23 (Steven 20240417)
extern int iCloseSiteModeFor2x8;        //golden ainarm9045_2x8_8.h:28 (JerryYang 20190729)
enum { e2x6OneByOne = 2 };              //golden ainarm9045_2x6_8.h:10 (enum e2x6Mode)
extern int  CloseSiteState(bool bPlace=true);                          //golden ainarm2.h:156
extern void InitInArmTask();
extern void SetRunStartMode(int iMode);
extern void TransferHotPlateRatio(bool bPick, int *iX, int *iY);        //Steven 20110324
extern void InspectInArmPosition(int iMot, int iSuckR, int iSuckC,
                                 int iPlateR, int iPlateC, bool bPlace);
extern bool MoveInArmZToPlateSafe(int Task);
extern bool IsCheckInArmDestroyActiveFinish();                          //ChungHung 20111229
extern void DoInArm_9045_SuckerMap();                                   //Steven 20220531
extern void SetShuttleToHasNullIC_9045(int iSht, int iKit);
extern void AdjustShuttlePlaceOrder(int iReset=-1);
extern void SetInArmHome();
extern void AddInArmPickerCount(int iSuckR, int iSuckC);
extern void InArmSubSpeed();
extern void InArmAddSpeed();
// NOTE (W6.2b): GetInArmPitchX_9045 / GetInArmPitchY_9045 / bUseAxExPicker /
//   bUseAxxGPicker / InArmLeftSideHasIC / InArmLeftSideNoIC / InspectInArmPosition
//   / IsCheckInArmDestroyActiveFinish / DoInArm_9045_SuckerMap /
//   SetShuttleToHasNullIC_9045 / AddInArmPickerCount are now REAL-DEFINED in
//   ainarm9045.cpp (the in-arm engine).  Their offline stub bodies were removed
//   from aHotPlateSubstrate.cpp to avoid ODR/link collisions.  The extern decls
//   for those symbols remain valid (same prototypes); the two pitch helpers the
//   already-translated leaves call are forward-declared here so they keep
//   compiling against the engine definition:
#ifndef ainarm9045H   // default args live in ainarm9045.h; leaves pass all args explicitly
extern int  GetInArmPitchX_9045(int iMovePitchX, int i, int iOffsetPos);        //golden ainarm9045.h:23 (REAL home: ainarm9045.cpp)
extern int  GetInArmPitchY_9045(int iMovePitchY, int iOffsetPos);               //golden ainarm9045.h:25 (REAL home: ainarm9045.cpp)
#endif
extern void StopAllMotor();
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

// ============================================================================
//  (A) [W6.2b] in-arm ENGINE cursors owned by not-yet-translated ainarm2.cpp.
//      The central pick SM binds `int &Task=iPickFromLoadStageTask`; DoInArm_9045
//      guards read iArmTask.  Golden ainarm2.h:97/101.  init to 1 (matching
//      golden InitInArmTask()).  Defined in aHotPlateSubstrate.cpp.
// ============================================================================
extern int iArmTask;                    //golden ainarm2.h:97  : master in-arm SM cursor
extern int iPickFromLoadStageTask;      //golden ainarm2.h:101 : central pick SM cursor
extern bool bPickFromLoader;            //golden ainarm2.h:54  : "Loader 吸取完成" flag (ProcessSCKARTLoadingCount)

// ============================================================================
//  (A2) [W6.2c] OUT-ARM ENGINE cursors owned by not-yet-translated aoutarm2.cpp.
//      DoOutArmPlaceToAuto_9045 binds `int &Task=iPlaceToAutoTask`;
//      DoOutArmAfterPlaceToAuto binds iDoOutArmAfterPlaceToAutoTask; the place SM
//      reads iWhichAuto/iWhichBuff/iWhichMag (golden aoutarm.h:18-20 -- but those
//      are ALREADY in cmydef.h, so only the three task cursors + the Fix3 cursor
//      live here).  init to 1 (golden InitOutArmTask/InitPlaceToAutoTask).
//      Golden aoutarm.h:49/15/51.  Defined in aHotPlateSubstrate.cpp.
// ============================================================================
extern int iPlaceToAutoTask;               //golden aoutarm.h:49  : central out-place SM cursor
extern int iDoOutArmAfterPlaceToAutoTask;  //golden aoutarm.h:15  : after-place SM cursor
extern int iFix3CanFullTask;               //golden aoutarm.h:51  : Fix3 full-tray SM cursor

// ============================================================================
//  (B) [W6.2b] per-variant close-site selector for 1x4, mirroring the
//      iCloseSiteModeFor2x6 / 2x8 pattern above.  GetJStep() reads it.
//      Golden home: ainarm9045_1x4_4.h:11/19 (per-site module, W7).
// ============================================================================
extern int iCloseSiteModeFor1x4;        //golden ainarm9045_1x4_4.h:11
enum { e1x4CloseAbAc = 3 };             //golden ainarm9045_1x4_4.h:19

// ============================================================================
//  (D) [W6.2b] fBarCode form-pointer for the additional-function SM (Bottom
//      2DID).  Mirrors the FormsFacade offline-stub pattern: every scan returns
//      false offline (no CCD).  Golden home: BarCode.h (TfBarCode).
//      Only the three methods the SM derefs are mirrored.
// ============================================================================
class TfBarCode_Shim
{
public:
    void InitBottom2DIDScan();          //golden BarCode.h : init bottom-2DID scan
    bool DoBottom2DIDScan();            //golden BarCode.h : 1-CCD bottom 2DID scan
    bool DoBottom2DID_8CCD_Scan();      //golden BarCode.h : 8-CCD bottom 2DID scan  //KaiChen 20200513

    // -- W6.5 ADD: in/out-shuttle 2D-barcode + shuttle-float-check methods the
    //    SHUTTLE/CARRY engine (acarry.cpp Do_Auto_SHT1/2) derefs.  golden
    //    BarCode/BarCode.h signatures verbatim (default args preserved).  Offline:
    //    every scan / trigger / float-check returns false (no CCD); the Initial*
    //    are no-ops; IsSHT2DIDScanFinish returns true (no 2DID pending) so the SM
    //    does not park forever waiting on a scan that never arrives.
    void InitialBarcodeScanInShuttle1(bool bClear2DID=true);   // golden BarCode.h:799
    void InitialBarcodeScanInShuttle2(bool bClear2DID=true);   // golden BarCode.h:800
    void InitialBarcodeScanOutShuttle1();                      // golden BarCode.h:801
    void InitialBarcodeScanOutShuttle2();                      // golden BarCode.h:802
    bool DoBarcodeScanInShuttle_1(bool bErrorSkip=false);      // golden BarCode.h:803
    bool DoBarcodeScanInShuttle_2(bool bErrorSkip=false);      // golden BarCode.h:804
    bool DoBarcodeTriggerInShuttle_1();                        // golden BarCode.h:805
    bool DoBarcodeTriggerInShuttle_2();                        // golden BarCode.h:806
    bool DoBarcodeCCDInShuttle_1(bool bVerify=false);          // golden BarCode.h:807
    bool DoBarcodeCCDInShuttle_2(bool bVerify=false);          // golden BarCode.h:808
    bool DoBarcodeScanOutShuttle_1();                          // golden BarCode.h:810
    bool DoBarcodeScanOutShuttle_2();                          // golden BarCode.h:811
    void InitialShuttleFloatCheck1();                          // golden BarCode.h:888
    void InitialShuttleFloatCheck2();                          // golden BarCode.h:889
    bool DoShuttleFloatCheck_1();                              // golden BarCode.h:890
    bool DoShuttleFloatCheck_2();                              // golden BarCode.h:891
    bool IsSHT2DIDScanFinish(int SHT);                         // golden BarCode.h:942
};
extern TfBarCode_Shim *fBarCode;        //golden BarCode.h : TfBarCode *fBarCode

// ============================================================================
//  (E) [W6.2b1x1] ainarm2.h engine shims the per-site in-arm VARIANT SMs call.
//      These symbols live in the not-yet-translated ainarm2.cpp (W7) and are
//      SHARED across every in-arm variant; they are therefore defined ONCE here
//      (single TU -> no ODR collision when sibling variant files are translated
//      in the same parallel batch).  Each cites its golden home; every body is
//      offline-safe.  Guards are belt-and-braces against a future direct ainarm2
//      translation also declaring them.  AI(W6.2b-INARM-1x1_1) 20260626.
// ============================================================================
#ifndef HT9045_AINARM2_INARM_SHIMS
#define HT9045_AINARM2_INARM_SHIMS

// -- data (golden ainarm2.h) -------------------------------------------------
extern TMyKitSuck *ptrInSHT;            // golden MyKitSuck.h:379 (Auto Clean Kit use; GetShuttleState_1x1_1 derefs)
extern TMyKitSuck  ptrInSHTBackup;      // golden MyKitSuck.h:380
extern TMyKitSuck  OutArmSuckBackup;    // golden MyKitSuck.h (SetInOutArmParameter_* CopyInitSuck target)
extern int  iInXPToSht[X_PITCH_COUNT];  // golden ainarm2.h:36
extern int  iZPosToSht[MAX_ARM_Row][MAX_ARM_Col];   // golden ainarm2.h:37
extern bool bZFlgToSht[MAX_ARM_Row][MAX_ARM_Col];   // golden ainarm2.h:38
extern TQPF_Timer InArmReleaseDelayToHot;           // golden ainarm2.h:45 (JerryYang 20160127)
extern TQPF_Timer InArmReleaseDelay;                // golden ainarm2.h:46
extern TQPF_Timer MyInArmAtShuttleTimer;            // golden ainarm2.h:47 (Steven 20151201)
extern int  iBackInArmHotCount;         // golden ainarm2.h:76
extern bool InArmXMoveSafe;             // golden ainarm2.h:83
extern bool bPlaceToShuttle2Step;       // golden ainarm2.h:56 (Steven 20160721)
// golden type is HTimer (cpublic.h fwd); offline we use TQPF_Timer (same Off()/
// SetSecAndOn() surface the SM calls) to avoid a cross-include of atester_shims.h.
extern TQPF_Timer hInArmYpitchHomeTimer; // golden ainarm2.h:212 (kevin 20180822 Ypitch)
// per-site task cursors the variant SMs bind `int &Task=...` to (golden ainarm2.h)
extern int  iInArmPlaceToHotPlateTask;  // golden ainarm2.h (DoPlaceToHotPlate_9045_* cursor)
extern int  iInArmPlaceToShuttleTask;   // golden ainarm2.h (DoInArmPlaceToShuttle_9045_* cursor)
extern int  iInArmTryPickFromHotPlateTask; // golden ainarm2.h (DoInArmTryPickFromHotPlate_* cursor)
extern int  iInArmPickFromHotPlateTask;  // golden ainarm2.h (DoInArmPickFromHotPlate_* cursor; DEFINED in ainarm_SearchPickPlate.cpp)
// HotPlate-check backup-position scalars (golden ainarm2.h; BackupPlacePos/RestorePlacePos)
extern int  iBackupPlate, iBackupPlateC, iBackupPlateR;

// -- functions (golden ainarm2.cpp) ------------------------------------------
extern void CopyInitSuck(TMyKitSuck *Source, TMyKitSuck *Target,
                         int SourceR, int SourceC, int TargetR, int TargetC); // golden MyKitSuck.cpp:1111
extern void SetInArmNeedDestory(bool bPlace, int iShtRow, int iShtCol, int iRow, int iCol); // golden ainarm2.h:230
extern void TransferInShuttleRatio(int iSht, int *iXPos, int *iYPos, int iRow, int iCol);   // golden ainarm2.h:159
extern void ResetInToShtFlag();                                              // golden ainarm2.h:40
extern void SetInArm_Unuse_SuckToNullICForHP();                              // golden ainarm2.h:153
extern void AdjustShuttleWhichKitOrder();                                    // golden ainarm2.h:130
extern bool CheckInArmFloating(bool bReset=false);                           // golden OmronLaser/LaserSensorInArm.h:30
extern void SetShuttlefCanMoveL(int iShuttle, bool bCanMoveL, AnsiString sFun, AnsiString sTask=""); // golden ainarm2.h:231
extern void AdjustShtOrderWhenPlaceToSht(int iMode);                         // golden ainarm2.h:132
extern void InArmAddSpeedDisplay();                                          // golden ainarm2.h:206
extern void InArmSubSpeedDisplay();                                          // golden ainarm2.h:207
extern void InitInOCRWaitTask();                                             // golden ainarm2.h:232
extern bool OCRMoveInArm2XYToWait();                                         // golden ainarm2.h:233
extern bool IsHotPlateCheckFinsih();                                         // golden ainarm2.h:166
extern void BackupPlacePos();                                               // golden ainarm2.h:164
extern void RestorePlacePos();                                              // golden ainarm2.h:165
extern void InitInArmPickFromHotPlateTask340();                             // golden ainarm2.cpp:628
extern void InitInArmPickFromHotPlateTask50();                              // golden ainarm2.cpp:633
extern void InitInArmPlaceToShuttleTask();                                  // golden ainarm2.cpp:647
extern void InitInArmPlaceToHotPlateTask();                                 // golden ainarm2.cpp:1539
extern void InitInArmPlaceToHotPlateTask400();                              // golden ainarm2.cpp:1544
extern void InitInArmPlaceToHotPlateTask100();                              // golden ainarm2.cpp:1549
extern void InitArmPickFromLoadStageTask();                                 // golden ainarm2.cpp:1016
extern void InitInArmTryPickFromHotPlateTask();                             // golden ainarm2.cpp:613
extern bool CheckInArmSuckInitial();                                        // golden ainarm2.cpp:1207
extern void SetInArmUseSuckToHasTrySuckIC(int iSht, int iKit);              // golden ainarm2.cpp:261
extern void DisableAutoSiteMapWhenCleanOut();                               // golden ainarm2.cpp:2786
extern void SetMotorSpeed();                                                // golden cinitial.cpp:5022
extern bool DoInArmAutoSiteMapping();                                       // golden (declared csystem.h:200)
extern void SearchPlateToPlace();                                           // golden ainarm2.h:112 (DEFINED in ainarm_SearchPlacePlate.cpp)
extern void InitInArmPickFromHotPlateTask();                                // golden ainarm2.h:115 (DEFINED in ainarm_SearchPickPlate.cpp)
extern void SetInArmUseSuckToHasNullIC(int iSht, int iKit);                 // golden ainarm2.h:151 (DEFINED in ainarm9045.cpp engine)
// in-arm phase flags the DoInArm_9045_1x1_1 master SM toggles (golden ainarm2.h
// :51/:53; DEFINED in acatchtray.cpp this wave).  Declared here so the variant TU sees them.
extern bool bPlaceToHotplate;                                               // golden ainarm2.h:51
extern bool bPlaceShuttle;                                                  // golden ainarm2.h:53
extern int  iInRotateFinish;                                               // golden (DEFINED in acatchtray.cpp)
extern bool bHangTimePause;                                                // golden (also declared atester.h:43; DEFINED in ainarm9045.cpp)
extern const bool ZAxisNotDown;                                             // golden ainarm2.h:43 (sibling of ZAxisDown)
// CheckInArmDestroyICFail is declared in csystem.h:88; redeclared here so the
// substrate TU can DEFINE the offline body (no built csystem.cpp definition).
extern bool CheckInArmDestroyICFail();                                      // golden csystem.cpp (Steven 20111223)

// ----------------------------------------------------------------------------
//  W6.2c batch-1: cross-variant in-arm shim symbols the 1x2_2 / 1x2_2_14 /
//  1x2_4_Hot / 1x3_2_14 / 1x3_4 SMs deref but that had no single shared home.
//  Reusing existing offline definitions where they exist; defining the genuinely
//  missing ones ONCE (aHotPlateSubstrate.cpp).  AI(W6.2c-INARM) 20260626.
// ----------------------------------------------------------------------------
// These offline shim symbols are already DEFINED in the 2x4_16 shim TU
// (ainarm9045_2x4_16_shims.cpp).  Declare the exact ones the W6.2c variants deref
// HERE (NOT by including ainarm9045_2x4_16_shims.h, which carries default-arg
// decls that collide with the engine's own definitions in ainarm9045.cpp).
extern void AdjustShuttlePlaceOrder_AutoSiteMapping();                      // golden ainarm2.h (def ainarm9045_2x4_16_shims.cpp)
extern void InitInArmTryPickFromHotPlateTask100();                          // golden ainarm2.h (def ainarm9045_2x4_16_shims.cpp)
extern void DoJudgeInputShuttleNeedChangeToNullIC();                        // golden (def ainarm9045_2x4_16_shims.cpp)
extern DWORD MyTickCount();                                                 // golden common.h:259 (def ainarm9045_2x4_16_shims.cpp)
// fYieldMonitoring: the live def (ainarm9045_2x4_16_shims.cpp) is typed
// TfYieldMonitoring_2x4_16*.  Mirror that exact type + decl so the variant
// `fYieldMonitoring->DoAutoCloseSite(..)` call binds to the same symbol.
#ifndef HT9045_YIELDMON_FACADE_DECLARED
#define HT9045_YIELDMON_FACADE_DECLARED
class TfYieldMonitoring_2x4_16 { public: void DoAutoCloseSite(bool /*bFlag*/) {} };
extern TfYieldMonitoring_2x4_16 *fYieldMonitoring;                          // golden uYieldMonitoring.h:fYieldMonitoring
#endif // HT9045_YIELDMON_FACADE_DECLARED
// Shuttle-soak start ticks (golden atester.h:81/:83; DEFINED in atester.cpp).
extern DWORD dwStartShuttle1Soak;                                           // golden atester.h:81
extern DWORD dwStartShuttle2Soak;                                           // golden atester.h:83
// AutoTeach Z form pointer (golden InOutArmZteach.h: TfInOutArmZteach* Zteach).
// Only Zteach->fShow is read (false offline).  Minimal offline facade; single
// canonical decl + def (aHotPlateSubstrate.cpp).  Guarded so the 1x2_2 local
// stand-in (now removed) cannot double-declare.
#ifndef HT9045_Zteach_DECLARED
#define HT9045_Zteach_DECLARED
struct TfInOutArmZteach_Facade { bool fShow; TfInOutArmZteach_Facade():fShow(false){} };
extern TfInOutArmZteach_Facade *Zteach;                                     // golden InOutArmZteach.h:Zteach
#endif
// ainarm2 helpers with NO live golden home (golden ainarm2.cpp:2812 / :533).
// Offline-safe: the Auto-Site-Mapping side-effect is skipped; the "clear-all-hot-IC
// then pick-load-IC" gate reports not-satisfied (conservative "nothing to do").
extern void DoCheckAutoSiteMappingPosition();                               // golden ainarm2.h:198 / ainarm2.cpp:2812
extern bool CheckClearAllHotICThenPickLoadIC();                             // golden ainarm2.h:168 / ainarm2.cpp:533

#endif // HT9045_AINARM2_INARM_SHIMS

#endif // aHotPlateSubstrateH
