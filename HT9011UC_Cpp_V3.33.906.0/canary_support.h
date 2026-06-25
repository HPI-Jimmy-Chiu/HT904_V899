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
    // TODO(W6.x): the other ~460 LAST_GENERAL_SET fields land with the full
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
