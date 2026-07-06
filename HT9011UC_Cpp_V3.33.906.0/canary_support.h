// =============================================================================
//  canary_support.h  --  W6.1 canary-support scaffold (Empty-tray SM)
//
//  Translation wave: W6.0 (canary-support scaffold for asendic_Empty)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  Collects the small substrate/scaffold gaps the W6.1 Empty-tray canary needs
//  that are NOT yet in the translated global headers:
//
//    1. __FUNC__ macro
//       BCB6 compiler builtin (current function name as a string).  Standard C++
//       spells it __func__.  asendic_Empty.cpp passes __FUNC__ to functions that
//       take AnsiString (which has a const char* ctor), so __func__ works.
//
//    2. LAST_GENERAL_SET / LastSet (MINIMAL shim)
//       The golden LastSet.h (1162 lines, LAST_GENERAL_SET) is gated out of the
//       target (MachineDefine.h #if 0 block).  The canary touches ONLY
//       LastSet.iRealDummy (23 refs, all comparisons vs DUMMY/REALLY).  This is
//       a MINIMAL shim with just iRealDummy.  TODO(W6.x): replace with the full
//       translated LastSet.h when the LastSet subsystem lands.
//
//    3. Free-function decls the canary calls that are not in a shared target
//       header:
//         ShowErrorMessage  (golden note.h)        -- drives Retry/Skip in the SM
//         RecordProcess     (golden cMyDB.h)
//         ShowUnloaderTrayMessage (golden mymessbox.h)
//         ReadWriteTrayID   (golden csystem.h -- already declared via csystem.h)
//         WhichAutoNeedTray (golden acatchtray.h)
//         ShowMyMessage     (golden mymessbox.h -- matches database.cpp local decl)
//       Sim bodies live in canary_support.cpp.
//
//  EventReport (SECS) is NOT declared here -- the single EventReport call in the
//  canary is gated #if 0 (W7-SECS).  MyMessageBox (VCL) is NOT declared here --
//  the pre-alarm block that uses it is gated #if 0 (W6.x).
// =============================================================================
#ifndef canary_supportH
#define canary_supportH

#include "vclcompat/vcl_compat.h"   // AnsiString

// ---------------------------------------------------------------------------
//  1. __FUNC__  (BCB6 builtin -> standard __func__)
// ---------------------------------------------------------------------------
#ifndef __FUNC__
#define __FUNC__ __func__
#endif

// ---------------------------------------------------------------------------
//  2. LAST_GENERAL_SET / LastSet  (MINIMAL shim -- iRealDummy only)
//     Golden field type: int iRealDummy;  (LastSet.h:54)
// ---------------------------------------------------------------------------
struct LAST_GENERAL_SET
{
    int iRealDummy;     // golden LastSet.h:54.  ==DUMMY(0) in the sim canary path.
    // AI(W6.2-INARM) 20260626: the W6.2 HP pick/place geometry leaves
    // (ainarm_SearchPickPlate/PlacePlate) deref three more LastSet fields.
    // Added to this shared minimal shim (golden field types from LastSet.h):
    int iRunStartMode;  // golden LastSet.h -- run-start mode (rsmAutoSiteMap/rsmQAMode/...)
    int iTemperature;   // golden LastSet.h -- temperature mode (Tempture_Hot/...)
    int iTester;        // golden LastSet.h -- tester link (OFF_LINE/...)
    // AI(W7-CATCHTRAY) 20260626: the TrayArm engine (acatchtray.cpp) derefs the
    // ART tray-count + ATK-tray-feed + Loader-count LastSet fields.  Added to this
    // shared minimal shim (golden field TYPES verbatim from LastSet.h):
    int  iUnloaderTrayCount_ART[256];   // golden LastSet.h:205 -- per-Auto ART unload tray count
    int  iEmptyTrayCount_ART;           // golden LastSet.h:373
    int  iColorTrayCount_ART;           // golden LastSet.h:374
    bool bLoaderTrayCount_ART;          // golden LastSet.h:370
    bool bCleanOut_ART;                 // golden LastSet.h:377
    int  iUnloadFixTray;                // golden LastSet.h:120 -- ATK AMR tray-feed mode (eAtkTf*)
    int  iOutTrayNum[256];              // golden LastSet.h:458 -- production-log output tray serial
    int  iSCKARTInputCT;                // golden LastSet.h:406 -- SCK ART input count
    int  iP57_InputCT;                  // golden LastSet.h:496 -- P57 Loader-count AutoCleanOut
    long SendCT[4];                     // golden LastSet.h:14  -- per-port send count
    // AI(W6.4-TESTER) 20260626: the TESTER/INDEX engine (atester.cpp DoTestY case
    // 60/260 + CheckTwoArmSiteMap/CheckIndexArmInitState) derefs the socket-purge
    // count + the per-arm use-socket map.  Added to this shared minimal shim
    // (golden field TYPES verbatim from LastSet.h; [2][MAX_SOCKET_ROW=4][MAX_SOCKET_COL=8]):
    int  iD47SocketTestedCount;         // golden LastSet.h -- socket-purge tested count (D47)
    bool bUseTestSocket[2][4][8];       // golden LastSet.h -- per-arm[0/1] site enable map
    // AI(W6.5-CARRY) 20260626: the SHUTTLE/CARRY engine (acarry.cpp Do_Auto_SHT1/2
    // case 250 purge-fan branch) compares the local purge counter vs
    // LastSet.iShuttlePurgeCount (golden acarry.cpp:3693).  Added to this shared
    // minimal shim (golden field TYPE verbatim from LastSet.h:199):
    int  iShuttlePurgeCount;            // golden LastSet.h:199 -- In-Shuttle purge-fan threshold
    // AI(W6.2b-2x4_16) 20260626: the in-arm place-to-shuttle SM increments the
    // place-to-shuttle counter (golden DoPlaceToShuttle_9045_* :LastSet.lShuttleCount++).
    long lShuttleCount;                 // golden LastSet.h:393 -- count place to shuttle
    // AI(W7T1-Integrate) 20260701: the un-gated DoTestHeadMotor down-press / socket-IC-check
    // tree derefs the D41 test-socket IC-check-skip flag + the initial-IC-check-position mode
    // (golden atester.cpp case 12000/14000/600 socket-check branches).  Added to this shared
    // minimal shim (golden field TYPES verbatim from LastSet.h:132 bool / :274 int).
    bool bD41TestSocketICCheckSkip;     // golden LastSet.h:132 -- [D41] skip test-socket IC vacuum check
    int  iD41SocketInitialICCheckPosition; // golden LastSet.h:274 -- [D41] initial IC-check position (1=above socket)
    // TODO(W6.x): the other ~450 LAST_GENERAL_SET fields land with the full
    //             translated LastSet.h.
};
extern LAST_GENERAL_SET LastSet;    // golden: extern LAST_GENERAL_SET LastSet; (LastSet.h:514)

// ---------------------------------------------------------------------------
//  3. Free-function decls (golden homes noted) + sim bodies in canary_support.cpp
// ---------------------------------------------------------------------------
//  ShowErrorMessage -- golden note.h:466.  In the sim it returns the Kxxx code
//  that drives the SM's Retry/Skip branch (default K_RETRY).  Signature VERBATIM.
int  ShowErrorMessage(AnsiString Code, int KCode, int Pos,
                      bool bDuplicateErr=false, AnsiString errPart=" ");         //Steven 20091231

//  RecordProcess -- golden cMyDB.h:63.  Sim: logs to stdout.
void RecordProcess(AnsiString S, AnsiString S2="");

//  ShowUnloaderTrayMessage -- golden mymessbox.h:60.  Sim: logs (no UI).
void ShowUnloaderTrayMessage(AnsiString S1, AnsiString S2);                      //Jou 20150721

//  WhichAutoNeedTray -- golden acatchtray.h:8.  Sim: 0 (no auto needs a tray).
int  WhichAutoNeedTray();

//  ShowMyMessage -- golden mymessbox.h:58.  Signature matches the database.cpp
//  local forward-decl (S3 defaulted) so the two TUs agree.  Sim: logs (no UI).
void ShowMyMessage(AnsiString S1, AnsiString S2="", AnsiString S3="",
                   bool Ok=false, bool bServoOff=false);

#endif // canary_supportH
