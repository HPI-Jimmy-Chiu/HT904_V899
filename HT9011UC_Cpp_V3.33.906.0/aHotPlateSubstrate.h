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

    bool Suck();                // golden :89   -- vacuum ON / destroy OFF (returns "suck finished")
    void On();                  // golden :91   -- vacuum solenoid ON
    void Off();                 // golden :92   -- vacuum solenoid OFF
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

    // methods the leaves call (golden signatures preserved) -------------------
    void ResetAll();                                                    // :289
    bool HasIC();                                                       // :310
    bool HasType(int);                                                  // :324
    bool UseSiteHasIC();                                               // :340 (=W6.0 gated predicate leaf)
    bool UseSiteNoIC();                                                // :341
    bool LeftSideNoIC(int MiddleValue);                                // :349
    bool RightSideNoIC(int MiddleValue);                               // :348
    bool ArmUpSideNoIC();                                              // :352
    bool ArmDownSideNoIC();                                            // :353
    void SetItemData(int iRow, int iCol, int data, int iTarget=-1);    // :283
    void CopyFromTray(int iSuckR, int iSuckC, int iSuckData, class TTrayMotor &Mot,
                      int TrayR, int TrayC, int iTrayData, int iTarget=-1,
                      bool bHP2Inarm=false);                            // :276
    void CopyToTray  (int iSuckR, int iSuckC, int iSuckData, class TTrayMotor &Mot,
                      int TrayR, int TrayC, int iTrayData, int iTarget=-1); // :277
};

extern TMyKitSuck InArmSuck;     // golden MyKitSuck.h:357
extern TMyKitSuck FLCarryKit;    // golden MyKitSuck.h:358
extern TMyKitSuck BLCarryKit;    // golden MyKitSuck.h:360
extern TMyKitSuck OutArmSuck;    // golden MyKitSuck.h:366

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
struct tRotateShim                                                      // golden aRotateKIT.h tRotate
{
    bool ActiveRotate;
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
extern bool bUseAxExPicker();                                           //golden ainarm9045.h
extern bool bUseAxxGPicker();                                           //golden ainarm9045.h
extern bool InArmLeftSideHasIC(int iRow=2);                             //golden ainarm9045.h:109
extern bool InArmLeftSideNoIC(int iRow=2);                              //golden ainarm9045.h:108
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
extern int  GetInArmPitchX_9045(int iMovePitchX, int i, int iOffsetPos);
extern int  GetInArmPitchY_9045(int iMovePitchY, int iOffsetPos);
extern void StopAllMotor();
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

#endif // aHotPlateSubstrateH
