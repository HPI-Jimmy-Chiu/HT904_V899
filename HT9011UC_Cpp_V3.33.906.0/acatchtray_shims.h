// =============================================================================
//  acatchtray_shims.h  --  W6.3 offline cross-module shims for the TrayArm engine
//
//  Translation wave: W6.3 (TRAY-ARM ENGINE -- acatchtray.cpp)
//  Translator: AI(W6.3-CATCHTRAY) 20260626
//
//  WHY THIS FILE EXISTS
//  --------------------
//  The faithfully-transcribed TrayArm engine (acatchtray.cpp) reaches a handful
//  of cross-module subsystems that have NO translated home this wave.  Rather
//  than gate hundreds of call sites inside the verbatim switch(Task) SMs (which
//  would obscure the cursor flow we are trying to preserve), this header exposes
//  ONLY the surface the engine dereferences, on transparently-named non-VCL
//  stand-ins, with offline-safe bodies (false / 0 / no-op) in acatchtray_shims.cpp.
//  This mirrors the FormsFacade form-pointer cut already used by the in/out-arm
//  engine waves -- it is the "genuinely cleaner" shim the task brief permits.
//
//  Each symbol cites its golden home.  When the real subsystem lands (W7:
//  cTrayMapping.h / Magazine.h / OCR.h / AMR.h / uHGemHT9045 SECS / LoadCCDMap.h),
//  these shims are replaced and the call sites bind to the real bodies unchanged.
//
//  GATED (NOT shimmed here -- left as `#if 0` at the single call site, since the
//  offline guard already makes them dead):
//    * HSys.BinDisCtrl->FlashPro / ClearAutoChangingWarn  (database.h TMyBinDispCtrl
//      is forward-decl-only; HSys.BinDisCtrl==NULL offline so the calls never run).
//
//  Big5: Chinese comments decoded via cp950, preserved UTF-8.  ZERO U+FFFD.
// =============================================================================
#ifndef acatchtray_shimsH
#define acatchtray_shimsH

#include "vclcompat/vcl_compat.h"   // AnsiString

// ===========================================================================
//  TColor + clWhite/clYellow  -- golden VCL Graphics.hpp.  The place-to-Auto SM
//  assigns fMain->mtAutoN->Color = cPtr[iTemp] where cPtr[2]={clWhite,clYellow}.
//  In the VCL TColor is an int RGB code; provide the int alias + the two consts.
// ===========================================================================
#ifndef HT9045_TCOLOR_SHIM
#define HT9045_TCOLOR_SHIM
typedef int TColor;                         // golden Graphics.hpp TColor (int)
const TColor clWhite  = 0x00FFFFFF;         // golden Graphics.hpp clWhite
const TColor clYellow = 0x0000FFFF;         // golden Graphics.hpp clYellow
#endif

// ===========================================================================
//  BoolToStr  -- golden SysUtils.hpp.  DoPlaceToBuffer's time-out log formats
//  several bool flags via BoolToStr.  vclcompat does not provide it; supply a
//  free function matching the VCL spelling ("-1"/"0" by default, but the engine
//  only uses it inside a log sprintf, so "True"/"False" is faithful enough).
// ===========================================================================
AnsiString BoolToStr(bool B, bool UseBoolStrs = false);   // golden SysUtils.hpp

// ===========================================================================
//  SECS_EVENT (ETypeStruct)  -- golden SECSGEM/uHGemHT9045.h:313.  The engine
//  reads SECS_EVENT.<CEID> (an anonymous-enum value accessed via the instance)
//  and passes it to EventReport(unsigned).  MINIMAL mirror: only the CEID names
//  the TrayArm engine references.  Values match the golden enum ordinals so any
//  future log comparison stays faithful.
// ===========================================================================
struct ETypeStruct
{
    enum
    {
        TrayTestFinish          = 67,   // golden uHGemHT9045.h
        AGVSupplement           = 272,
        UnloaderTrayIDReadOK    = 285,
        LoaderTrayIDReadFail    = 287,
        AGVLdID                 = 291,
        BundleCompleteIDRead    = 242,
        BundleEnd_IDREAD_Auto1  = 246,
        BundleEnd_IDREAD_Auto2  = 248,
        BundleEnd_IDREAD_Auto3  = 252,
        BundleEnd_IDREAD_Auto4  = 254,
        BundleEnd_IDREAD_Auto5  = 256,
        BundleEnd_IDREAD_Auto6  = 258
    };
};
extern struct ETypeStruct SECS_EVENT;       // golden uHGemHT9045.h:313

//  EventReport -- golden SECSGEM/UsecegemMainFrom.h:327.  Offline: no SECS link.
void EventReport(unsigned Ceid);

// ===========================================================================
//  TfTrayMapping (fTrayMapping)  -- golden cTrayMapping.h.  The TrayArm engine
//  derefs the Tray-Mapping / device-remain / cover-tray-ID / RFID surface.
//  MINIMAL mirror of exactly the members the engine touches; every predicate is
//  false / no-op offline (no CCD / no laser / no RFID reader present), so the
//  Tray-Mapping case arms compile and take their deterministic "nothing to map"
//  fall-through.  TODO(W7): replace with the real translated cTrayMapping.h.
// ===========================================================================
//  ldRFID nested object (golden cTrayMapping.h TfTrayMapping::ldRFID)
class TLdRFIDShim
{
public:
    bool       bCommConnect;        // RFID reader link up?  (offline false)
    bool       bClearCmd1;          // clear-cmd ack
    bool       bReadUID1;           // UID read done?
    AnsiString sBlockID1;           // read block-ID
    AnsiString sUID1;               // read UID
    TLdRFIDShim();
};
//  a clickable-button stand-in (golden TButton*; engine only calls ->Click())
class TBtnShim { public: void Click(); };
//  a TStringList-ish stand-in for listTrayIDByLot (Clear/Add/Text)
class TListTrayIDShim
{
public:
    AnsiString Text;
    void Clear();
    void Add(AnsiString s);
};
//  a yield-recorder stand-in for yieldRemainIC (AddYieldData(bool))
class TYieldShim { public: void AddYieldData(bool bPass); };

class TfTrayMapping
{
public:
    // -- AOI-type selector the device-remain laser scan passes (golden enum) ----
    int  eAOIType_TrayDeviceCheckLaser;         // golden cTrayMapping.h
    bool bDoTrayDeviceCheckFromLoader;          // golden -- device-remain-from-loader flag
    AnsiString asErrorLaserScan;                // golden -- laser-scan error string
    int  iAutoRetryCount;                       // golden -- auto-retry counter
    TLdRFIDShim     *ldRFID;                     // golden -- loader-car RFID reader
    TBtnShim        *btnLdRFIDConn;              // golden -- RFID connect button
    TBtnShim        *btnLdRFIDClear1;            // golden -- RFID clear button
    TBtnShim        *btnLdRFIDReadBlock1;        // golden -- RFID read-block button
    TBtnShim        *btnReadUID1;                // golden -- RFID read-UID button
    TBtnShim        *cbBarCodeSimulate;          // golden -- (referenced; sim checkbox)
    TListTrayIDShim *listTrayIDByLot;            // golden -- per-lot tray-ID list
    TYieldShim       yieldRemainIC;              // golden -- device-remain yield recorder

    void WritePickMapLog();                      // golden -- offline no-op
    bool IsRunDeviceRemainLaser();               // golden -- offline false
    bool GetTrayDeviceCheckIsRemainIC();         // golden -- offline false
    bool DoLaserScanFromLoader(int eType, bool bStart, bool bCheck=false);  // golden
    bool DoAutoSkip(int eType);                  // golden -- offline false
    bool CheckRunAutoRetry(int eType);           // golden -- offline false
    bool DoCoverTrayID(int iPos);                // golden -- offline false
    bool DoCoverTrayIDKeyence(int iPos);         // golden -- offline false
    bool DoTrayIDKeyence();                      // golden -- offline false (case 4200, 0-arg)
    bool DoTrayIDKeyence2(int i);                // golden -- offline false
    bool DoTrayID2CCD();                         // golden -- offline false
    bool DoTrayIDCCD();                          // golden -- offline false
    bool DoTrayMapCCD(int iMap);                 // golden -- offline false (case 5100/6030, 1-arg)
    TfTrayMapping();
};
extern TfTrayMapping *fTrayMapping;             // golden cTrayMapping.h: extern TfTrayMapping *fTrayMapping

// ===========================================================================
//  MyMessageBox  -- golden mymessbox.h (TMyMessageBox*).  The buffer-place SM
//  closes it before raising a pre-alarm.  Offline: never visible -> Close() no-op.
// ===========================================================================
class TMyMessageBoxShim
{
public:
    bool Visible;       // golden -- dialog visible?  (offline false)
    bool fShow;         // golden -- show flag        (offline false)
    void Close();       // golden -- offline no-op
    TMyMessageBoxShim();
};
extern TMyMessageBoxShim *MyMessageBox;         // golden mymessbox.h

// ===========================================================================
//  LoadCCD  -- golden LoadCCDMap.h (TLoadCCD*).  DoCatchTray case 150/160 reads
//  the loader tray-map CCD.  Offline: LoadCCDData() returns 0 ("no remain IC,
//  proceed"); InitialLoadCCD() no-op.
// ===========================================================================
class TLoadCCDShim
{
public:
    int  LoadCCDData();     // golden LoadCCDMap.h -- offline: 0 (OK, no remain)
    void InitialLoadCCD();  // golden LoadCCDMap.h -- offline no-op
};
extern TLoadCCDShim *LoadCCD;                   // golden LoadCCDMap.h

// ===========================================================================
//  AMR  -- golden AMR.h (TAMR AMR).  DoPlaceTrayToAuto case 510 calls
//  AMR.SupplyCover(AutoTarget).  Offline: no-op.
// ===========================================================================
class TAMRShim { public: void SupplyCover(int iAuto); };
extern TAMRShim AMR;                            // golden AMR.h: TAMR AMR

// ===========================================================================
//  Missing free functions the engine calls (no translated home this wave).
//  All offline-safe.  Golden homes noted.  These keep the verbatim SM bodies
//  linkable; their real bodies live in main.cpp / ainarm2.cpp / Magazine.cpp /
//  cSortCT.cpp / Automation (W7).
// ===========================================================================
//  in-arm position / picker helpers (golden ainarm9045.h / ainarm2.h / main.h)
bool MoveInArm2XYToWait();                       // golden -- offline: reached (true)
bool MoveInArm2XYToShuttle2Wait();               // golden -- offline: reached (true)
bool CheckInArmFinishAllPickerAction();          // golden -- offline: finished (true)
bool IsMoveInArm2XYToWait();                     // golden -- offline: at wait (true)
// W6.2c batch-4 (2x8_8): golden ainarm2.h:142 (Sam 20250522).  Offline: return FALSE
// so the golden guard `if(IsMoveInArm2XYToShuttle2Wait()==false) MoveInArm2XYToShuttle2Wait();`
// runs the offline-available move (the faithful not-already-at-wait path).
bool IsMoveInArm2XYToShuttle2Wait();             // golden ainarm2.h:142 -- offline: NOT at wait (false)
//  out-arm auto-alignment selector (golden aoutarm*.h)
bool CheckOutArmAutoAlignmentTrayModeBeUse(int iMode);  // golden -- offline false
//  single-motor home helpers for under-conveyor Z (golden mymotor/main)
void InitProcessSingleMotorTask(int iMot);       // golden -- offline no-op
bool ProcessSingleMotorHome(int iMot);           // golden -- offline: home done (true)
//  AMR / Magazine cover-tray feed (golden AMR.h / Magazine.h)
int  SearchNeedAMRUnload(bool bAlarm);           // golden -- offline: -1 (nothing)
int  SearchNeedAMRUnloadCount();                 // golden -- offline: 0
AnsiString AMRUnloadBin(int iBinPos);            // golden -- offline: ""
bool DoMagazineAMRTrayFeed(int iWhichAuto);      // golden -- offline: done (true)
void InitialDoMagazineAMRTrayFeed();             // golden -- offline no-op
//  receive-tray task inits (golden asendic / main)
void InitAutoColorReceiveTask();                 // golden -- offline no-op
void InitAutoEmptyReceiveTask();                 // golden -- offline no-op
bool DoAutoColor();                              // golden asendic -- offline: idle (true)
//  unloader-info / teach-alignment (golden main / cSortCT)
void SetUnloaderInfoFile(int iUnloader);         // golden -- offline no-op  (also extern in acatchtray.cpp)
void InitDoOutArmTeachAlignmentProcessTask();    // golden -- offline no-op
//  production-log sink (golden cMyDB.h)
void NewRecordProcess(AnsiString S1, AnsiString S2="", AnsiString S3="");  // golden -- log sink

//  per-Auto receive-bin-tray flow (golden asendic / cSortCT) -- offline: false
bool DoAutoReceiveBinTray(int iWhichAuto);       // golden -- offline: no receive
//  per-Auto stack cylinder up/middle/lower helpers (golden asendic auto-cylinder)
bool AutoCylinderUp(int iAuto, int iUp, int iSel, bool bReset=false);      // golden -- offline: reached
bool AutoCylinderMiddle(int iAuto, int iUp, int iSel, bool bReset=false);  // golden -- offline: reached
bool AutoCylinderLower(int iAuto, int iUp, int iSel, bool bReset=false);   // golden -- offline: reached
//  motor scale-speed setter + BinDisp warn-clear (golden main / MyBinDisp) -- no-op
void SetMotorScaleSpeed(int iMot, int iSp);      // golden -- no-op
void ClearAutoChangingWarn(int iAuto);           // golden MyBinDisp -- no-op

// ===========================================================================
//  eATkTrayFeed enum members the count helper reads (golden Automation/AGV.h:212).
//  AddTrayCount compares LastSet.iUnloadFixTray against these two; AGV.h is not
//  in scope this wave, so mirror the two ordinals verbatim.
// ===========================================================================
//  eAtkTfFeedFix / eAtkTfFixToAMR / eAtkTfPutIDTray / eAtkTfPutCover /
//  eAtkTfPutEmptyTray (golden Automation/AGV.h:215-220) are defined directly in
//  acatchtray.cpp (it owns those mirrors this wave) -- NOT here, to avoid a
//  duplicate-enumerator clash.

// ===========================================================================
//  Missing file-scope globals the RFID SM reads (golden cmydef / cTrayMapping).
// ===========================================================================
extern int iCoverTrayIDTask[];                   // golden -- per-port cover-tray-ID SM cursors
extern int iReadCIDAction;                       // golden -- which-port-reading-CID action
//  NOTE: bPlaceToHotplate / bPlaceShuttle / iInArmWaitPosition / iInRotateFinish
//  are defined directly in acatchtray.cpp (it owns them this wave).

#endif // acatchtray_shimsH
