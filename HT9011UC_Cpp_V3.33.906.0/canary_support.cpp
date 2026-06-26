// =============================================================================
//  canary_support.cpp  --  definitions for the W6.1 canary-support scaffold
//
//  Translation wave: W6.0 (canary-support scaffold for asendic_Empty)
//  Translator: AI(W6.0-SCAFFOLD) 20260626
//
//  Defines the minimal LastSet shim + sim bodies for the free functions the
//  Empty-tray canary calls.  See canary_support.h for the per-symbol rationale
//  and golden provenance.
// =============================================================================
#include "canary_support.h"
#include "csystem.h"    // ReadWriteTrayID decl (golden csystem.h:245)
#include "cmydef.h"     // K_RETRY (extern const int)
#include "cpublic.h"    // RespondASECom decl (gated body in cpublic.cpp)
#include <cstdio>

// ---------------------------------------------------------------------------
//  LastSet  (MINIMAL shim)
//  Zero-initialised -> iRealDummy == 0 == DUMMY.  The smoke test sets it
//  explicitly (DUMMY for the dummy-feed path); production LastSet lands W6.x.
// ---------------------------------------------------------------------------
LAST_GENERAL_SET LastSet = {0};

// ---------------------------------------------------------------------------
//  bIsCatchingFromBuffer / bIsPlacingToBuffer
//  Declared `extern bool` in cmydef.h, but their DEFINITIONS (cmydef.cpp:5946-
//  5947) fall inside the cmydef.cpp `#if 0 // TODO(W6)` gate (5806-6016), so the
//  ht9045_globals archive does not provide them yet.  They are the two newest
//  TrayArm-buffer flags (JerryYang 20250828 / RogerYang 20260225) and the canary
//  reads both (DoAutoEmpty case 0 + case 100).  Provide the definitions here as
//  a W6.1 scaffold; when the cmydef.cpp gate is opened (W6.x) remove these two.
// ---------------------------------------------------------------------------
bool bIsPlacingToBuffer   = false;      //JerryYang 20250828 (golden cmydef.cpp:5946)
bool bIsCatchingFromBuffer = false;     //RogerYang 20260225 (golden cmydef.cpp:5947)

// ---------------------------------------------------------------------------
//  ShowErrorMessage -- golden note.h:466.
//  In the offline sim there is no operator to press Retry/Skip/Home, and the
//  Empty-tray SM uses the return value to choose its recovery branch.  Returning
//  K_RETRY keeps the SM on the "retry" arm (it re-attempts rather than skipping
//  or homing), which is the safe, faithful default for an unattended sim.
//  Logs the alarm so the smoke test trace shows which error path was taken.
// ---------------------------------------------------------------------------
int ShowErrorMessage(AnsiString Code, int KCode, int Pos,
                     bool /*bDuplicateErr*/, AnsiString /*errPart*/)
{
    std::printf("  [ShowErrorMessage] Code=%s KCode=%d Pos=%d\n",
                Code.c_str(), KCode, Pos);
    return K_RETRY;     // TODO(W6.x): real operator-driven Retry/Skip/Home dialog
}

// ---------------------------------------------------------------------------
//  RecordProcess -- golden cMyDB.h:63.  Sim: log to stdout (no DB write).
// ---------------------------------------------------------------------------
void RecordProcess(AnsiString S, AnsiString S2)
{
    if(S2.Length() > 0)
        std::printf("  [RecordProcess] %s | %s\n", S.c_str(), S2.c_str());
    else
        std::printf("  [RecordProcess] %s\n", S.c_str());
}

// ---------------------------------------------------------------------------
//  ShowUnloaderTrayMessage -- golden mymessbox.h:60.  Sim: log (no UI).
// ---------------------------------------------------------------------------
void ShowUnloaderTrayMessage(AnsiString S1, AnsiString S2)
{
    std::printf("  [ShowUnloaderTrayMessage] %s | %s\n", S1.c_str(), S2.c_str());
}

// ---------------------------------------------------------------------------
//  WhichAutoNeedTray -- golden acatchtray.cpp:410.  W7: the REAL definition now
//  lives in the translated acatchtray.cpp (the TrayArm engine OWNS it).  The
//  W6.1 sim stub that used to live here (return 0) was REMOVED to avoid an ODR /
//  link collision (mirrors how the in-arm engine removed pitch-helper stubs from
//  aHotPlateSubstrate.cpp).  The decl remains in canary_support.h / acatchtray.h.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  ShowMyMessage -- golden mymessbox.h:58.  Sim: log (no modal dialog).
// ---------------------------------------------------------------------------
void ShowMyMessage(AnsiString S1, AnsiString S2, AnsiString /*S3*/,
                   bool /*Ok*/, bool /*bServoOff*/)
{
    if(S2.Length() > 0)
        std::printf("  [ShowMyMessage] %s | %s\n", S1.c_str(), S2.c_str());
    else
        std::printf("  [ShowMyMessage] %s\n", S1.c_str());
}

// ---------------------------------------------------------------------------
//  ReadWriteTrayID -- golden csystem.h:245.  Sim: no-op (no RFID/2D reader).
//  Declared in csystem.h; defined here for the canary so the SM links.
// ---------------------------------------------------------------------------
void ReadWriteTrayID(bool /*bRead*/)
{
    // TODO(W6.x): wire to TrayID[][] read/write when the tray-ID subsystem lands.
}

// ---------------------------------------------------------------------------
//  RespondASECom -- declared in cpublic.h; its golden body (cpublic.cpp:729) is
//  GATED there (#if 0 // TODO(W6)) because it derefs ASESendMessage (god-stack).
//  Faithful to the golden: the real send only happens for CC_ASE_KaohSiung; for
//  every other customer (and offline) it returns false.  The sim has no ASE
//  Kaohsiung link, so this is the faithful false path.
//  When cpublic.cpp's gate is opened (W6.x), remove this definition.
// ---------------------------------------------------------------------------
bool RespondASECom(AnsiString /*S1*/)
{
#if 0   // TODO(W6.x): ASESendMessage->SendToASEData (god-stack); CC_ASE_KaohSiung
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        ASESendMessage->SendToASEData(S1);
        return true;
    }
#endif
    return false;   // offline / non-ASE-Kaohsiung: faithful false path
}
