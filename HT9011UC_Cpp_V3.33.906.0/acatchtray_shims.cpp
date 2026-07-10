// =============================================================================
//  acatchtray_shims.cpp  --  offline bodies for the W6.3 TrayArm cross-module shims
//
//  Translation wave: W6.3 (TRAY-ARM ENGINE -- acatchtray.cpp)
//  Translator: AI(W6.3-CATCHTRAY) 20260626
//
//  All bodies are CONSERVATIVE offline defaults over the Sim HAL.  See
//  acatchtray_shims.h for per-symbol rationale + golden provenance.
//    * Tray-Mapping / RFID / CCD predicates report "nothing to do" (false) so the
//      Tray-Mapping case arms fall through deterministically.
//    * AMR / Magazine feeds report "done / nothing" so the AMR dispatch (dormant
//      offline because TrayForm.bEnableAMR / fAGV->Use_AMR() are false) links.
//    * MyMessageBox is never visible; LoadCCD reports "no remain IC" (0).
//
//  Big5: Chinese comments preserved as UTF-8 (cp950).  ZERO U+FFFD.
// =============================================================================
#include "acatchtray_shims.h"
#include "MachineType.h"            // eTrayCount / ePortTotal / MAX_AUTO_TRAY (array dims)

// ---- BoolToStr (golden SysUtils.hpp) ---------------------------------------
AnsiString BoolToStr(bool B, bool /*UseBoolStrs*/)
{
    return B ? AnsiString("True") : AnsiString("False");
}

// ---- SECS_EVENT + EventReport -----------------------------------------------
// AI(W5-comms-Integrate) 20260710: now provided for real by ht9045_secsgem
// (SECSGEM/SecsEventType.cpp defines SECS_EVENT; SECSGEM/SecsEventReport.cpp
// defines EventReport()) -- see acatchtray_shims.h.  Removed the local
// placeholder definitions (would otherwise duplicate-define both symbols).

// ---- TfTrayMapping (offline: no CCD / laser / RFID) ------------------------
TLdRFIDShim::TLdRFIDShim()
    : bCommConnect(false), bClearCmd1(false), bReadUID1(false) {}
void TBtnShim::Click() {}
void TListTrayIDShim::Clear() { Text = ""; }
void TListTrayIDShim::Add(AnsiString s) { Text = Text + s + "\r\n"; }
void TYieldShim::AddYieldData(bool /*bPass*/) {}

TfTrayMapping::TfTrayMapping()
    : eAOIType_TrayDeviceCheckLaser(0),
      bDoTrayDeviceCheckFromLoader(false),
      asErrorLaserScan(""),
      iAutoRetryCount(0)
{
    ldRFID              = new TLdRFIDShim();
    btnLdRFIDConn       = new TBtnShim();
    btnLdRFIDClear1     = new TBtnShim();
    btnLdRFIDReadBlock1 = new TBtnShim();
    btnReadUID1         = new TBtnShim();
    cbBarCodeSimulate   = new TBtnShim();
    listTrayIDByLot     = new TListTrayIDShim();
}
void TfTrayMapping::WritePickMapLog() {}
bool TfTrayMapping::IsRunDeviceRemainLaser()        { return false; }
bool TfTrayMapping::GetTrayDeviceCheckIsRemainIC()  { return false; }
bool TfTrayMapping::DoLaserScanFromLoader(int, bool, bool) { return false; }
bool TfTrayMapping::DoAutoSkip(int)                 { return false; }
bool TfTrayMapping::CheckRunAutoRetry(int)          { return false; }
bool TfTrayMapping::DoCoverTrayID(int)              { return false; }
bool TfTrayMapping::DoCoverTrayIDKeyence(int)       { return false; }
bool TfTrayMapping::DoTrayIDKeyence()               { return false; }
bool TfTrayMapping::DoTrayIDKeyence2(int)           { return false; }
bool TfTrayMapping::DoTrayID2CCD()                  { return false; }
bool TfTrayMapping::DoTrayIDCCD()                   { return false; }
bool TfTrayMapping::DoTrayMapCCD(int)               { return false; }
TfTrayMapping *fTrayMapping = new TfTrayMapping();

// ---- MyMessageBox (golden mymessbox.h) -------------------------------------
TMyMessageBoxShim::TMyMessageBoxShim() : Visible(false), fShow(false) {}
void TMyMessageBoxShim::Close() {}
TMyMessageBoxShim *MyMessageBox = new TMyMessageBoxShim();

// ---- LoadCCD (golden LoadCCDMap.h) -----------------------------------------
int  TLoadCCDShim::LoadCCDData()   { return 0; }    // offline: no remain IC -> proceed
void TLoadCCDShim::InitialLoadCCD(){}
TLoadCCDShim *LoadCCD = new TLoadCCDShim();

// ---- AMR (golden AMR.h) -- AI(W5-Automation-Integrate) 20260710: TAMRShim/AMR
//      stopgap REMOVED (ODR collision with the real Automation/AMR.cpp `AMR`
//      global now that it exists) -- see acatchtray_shims.h comment.

// ---- missing free functions (offline-safe) ---------------------------------
bool MoveInArm2XYToWait()                       { return true;  }
bool MoveInArm2XYToShuttle2Wait()               { return true;  }
bool CheckInArmFinishAllPickerAction()          { return true;  }
bool IsMoveInArm2XYToWait()                     { return true;  }
bool IsMoveInArm2XYToShuttle2Wait()             { return false; }  // golden ainarm2.h:142 (Sam 20250522) -- offline: NOT at wait
bool CheckOutArmAutoAlignmentTrayModeBeUse(int) { return false; }
void InitProcessSingleMotorTask(int)            {}
bool ProcessSingleMotorHome(int)                { return true;  }
int  SearchNeedAMRUnload(bool)                  { return -1;    }
int  SearchNeedAMRUnloadCount()                 { return 0;     }
AnsiString AMRUnloadBin(int)                    { return "";    }
bool DoMagazineAMRTrayFeed(int)                 { return true;  }
void InitialDoMagazineAMRTrayFeed()             {}
void InitAutoColorReceiveTask()                 {}
// InitAutoEmptyReceiveTask: REAL home asendic_Empty.cpp (W6.1 canary) -> NOT redefined here (ODR)
bool DoAutoColor()                              { return true;  }
void SetUnloaderInfoFile(int)                   {}
void InitDoOutArmTeachAlignmentProcessTask()    {}
void NewRecordProcess(AnsiString, AnsiString, AnsiString) {}

// ---- missing file-scope globals --------------------------------------------
//  iCoverTrayIDTask is a per-port cursor array; size to the cover-tray-ID port
//  count.  iKeyenceCoverTrayID_LoaderCar etc. index it (MachineType.h enum);
//  64 is comfortably above the port count.  iReadCIDAction = ePortTotal (idle).
int iCoverTrayIDTask[64] = {0};                 // golden cmydef / cTrayMapping
int iReadCIDAction       = 0;                   // golden -- ePortTotal at rest (set in SM)
//  NOTE: bPlaceToHotplate / bPlaceShuttle / iInArmWaitPosition / iInRotateFinish
//  are DEFINED in acatchtray.cpp (it owns them this wave) -- not here (ODR).
//  NOTE: the TrayArm-subsystem globals (bEject / bPurgeOutAllDevice / iThisPortNo
//  / iThisPortStatus / iPortStatus / ASE_OutTrayNum / iReceiveAutoTrayTask /
//  bOldAutoHasTray / bBoatChangeCasset) + the Tray-Mapping inits + the out-arm
//  MoveOutArmXY_ToFix_Tray_Full(bool) are DEFINED below in the consolidated W7
//  block -- not duplicated here.

// ---- W7 ADD: per-Auto receive / stack-cylinder helpers (offline-safe) ------
bool DoAutoReceiveBinTray(int /*iWhichAuto*/)        { return false; }   // offline: no receive
bool AutoCylinderUp(int, int, int, bool)             { return true;  }   // offline: reached
bool AutoCylinderMiddle(int, int, int, bool)         { return true;  }   // offline: reached
bool AutoCylinderLower(int, int, int, bool)          { return true;  }   // offline: reached
void SetMotorScaleSpeed(int, int)                    {}                  // offline: no-op
void ClearAutoChangingWarn(int)                      {}                  // offline: BinDisp warn-clear no-op

// =============================================================================
//  (W7) TrayArm-subsystem GLOBALS + functions whose golden definitions live in
//  not-yet-translated modules / gated cmydef.cpp/cprod.cpp blocks (KYEC-AMR /
//  Boat-Carrier / Tray-Mapping / cover-tray / port-status).  The engine reads
//  them; their extern decls are in cmydef.h / csystem.h / acatchtray.h.  Provide
//  offline-safe DEFINITIONS here so the engine LINKS.  Golden homes noted.
//  TODO(W7): remove as cmydef.cpp / the owning subsystems are ungated/translated.
// =============================================================================
#include "cmydef.h"     // MAX_AUTO_TRAY, eTrayCount, ePortTotal

// -- data globals (offline-safe init) -----------------------------------------
bool bBoatChangeCasset[3]            = {false,false,false};   // golden cmydef.cpp:5874 (Ifor 20251220 Boat Change)
bool bEject                          = false;                 // golden cmydef.cpp:5937 (JerryYang 20251020 half-clean)
bool bPurgeOutAllDevice              = false;                 // golden cmydef.cpp:5939 (JerryYang 20251020 half-clean)
bool bTrayArmIsPickColor             = false;                 // golden cmydef.cpp:5912 (JerryYang 20250113 Color TrayID)
bool bOldAutoHasTray[MAX_AUTO_TRAY]  = {false,false,false,false,false,false}; // golden cmydef.cpp (acatchtray.h:53 extern)
int  ASE_OutTrayNum[eTrayCount]      = {0};                   // golden cmydef.cpp:5795 (KYEC AMR output tray count)
int  iThisPortNo                     = 0;                     // golden cmydef.cpp:5843
int  iThisPortStatus                 = 0;                     // golden cmydef.cpp:5844
int  iPortStatus[ePortTotal]         = {0};                   // golden cmydef.cpp:5845
int  iReceiveAutoTrayTask[MAX_AUTO_TRAY] = {0};               // golden csystem.h:67 (auto-receive cursors)

// -- functions (offline-safe) -------------------------------------------------
bool DoLockUnloader(int /*iAuto*/)         { return true; }   // golden csystem.h:259 -- offline: unloader locked OK
void InitialTrayIDTask()                   {}                 // golden cTrayMapping -- reset tray-ID SM (no-op)
void InitialTrayID2Task()                  {}                 // golden cTrayMapping -- reset tray-ID-2 SM (no-op)
void InitialTrayMapTask(int /*iTrayMap*/)  {}                 // golden cTrayMapping -- reset tray-map SM (no-op)
void InitialCoverTrayIDTask(int /*iPos*/)  {}                 // golden cTrayMapping -- reset cover-tray-ID SM (no-op)
//  MoveOutArmXY_ToFix_Tray_Full(bool): golden aoutarm side has a 0-arg file-local
//  static; the TrayArm engine calls the 1-arg (bMoveY) form (golden decl :68).
//  Offline: out-arm reaches the fix-tray-full XY immediately.
bool MoveOutArmXY_ToFix_Tray_Full(bool /*bMoveY*/) { return true; }   // golden aoutarm (decl acatchtray.cpp:68)
