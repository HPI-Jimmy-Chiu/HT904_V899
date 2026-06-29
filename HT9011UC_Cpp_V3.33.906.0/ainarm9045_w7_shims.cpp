// =============================================================================
//  ainarm9045_w7_shims.cpp  --  W7-A1 in-arm SHUTTLE-MOTION seam offline bodies
//
//  Translation wave: W7-A1 (in-arm shuttle floating/latch PAIR)
//  Integrate step: AI(W7-A1-INTEGRATE) 20260629
//
//  Faithful OFFLINE link stubs for the shuttle-motion engine symbols the un-gated
//  DoInArmCheckShuttleFloating / CheckInShuttleSensor_Latch reference but whose
//  real home (golden ainarm2.h / the shuttle-motion engine) is not translated yet.
//  Declared in ainarm9045_w7_shims.h.
//
//  Offline contract (the Sim HAL has no real shuttle shake/knock/vibrate motor):
//    * DoShakeShuttle/DoKnockShuttle/DoKnockShuttleFirst/DoVibrateShuttle ->
//      converge immediately, return TRUE ("done") so the floating SM's
//      shake/knock/vibrate loop (cases 1401/1430/1440/1411/1260) advances instead
//      of spinning.  Each callsite is `if(DoXxxShuttle(iSht)) { Task=... }`, so
//      true == "motion finished, proceed".
//    * RecordShtSuperflous -> log no-op (golden ainarm2.h:218: it only appends a
//      "device corrected by vibrate/shake" record; no machine state).
//    * IndexZCanMove[2] -> plain global bool[2], default {false,false} (golden
//      ainarm2.h:48); floating case 9000 sets both true.
//    * bShuttleKnock -> plain global bool, default false (golden ainarm2.h:85);
//      read at floating case 1411, reset false at case 9000.
//
//  IsTestZ1/Z2NotSafeShuttleNCanNotMove are DEFINED in acarry.cpp -- intentionally
//  NOT defined here (ODR); declared in the header only.
//
//  Big5: no Chinese in this file; ZERO U+FFFD.
// =============================================================================
#include "ainarm9045_w7_shims.h"
#include "csystem.h"               // InitDoInShZHome / DoInShZHome decls (golden csystem.h:296-297)
#include <cstdio>

#ifndef HT9045_AINARM_W7_SHIMS_DEFINED
#define HT9045_AINARM_W7_SHIMS_DEFINED

// ---- shuttle-motion engine helpers (offline: converge immediately) ----------
bool DoShakeShuttle(int /*iShuttle*/, bool /*bNeedInitial*/)        { return true; } // golden ainarm2.h:189 -- Sim HAL: shake done
bool DoKnockShuttle(int /*iShuttle*/, bool /*bNeedInitial*/)        { return true; } // golden ainarm2.h:190 -- Sim HAL: knock done
bool DoKnockShuttleFirst(int /*iShuttle*/, bool /*bNeedInitial*/)   { return true; } // golden ainarm2.h:191 -- Sim HAL: first-knock done
bool DoVibrateShuttle(int /*iShuttle*/, bool /*bNeedInitial*/)      { return true; } // golden ainarm2.h:214 -- Sim HAL: vibrate done

// ---- RecordShtSuperflous (golden ainarm2.h:218): log no-op offline -----------
void RecordShtSuperflous(int /*iShuttle*/)                          {}             // Sim HAL: no superfluous-IC log sink

// ---- plain globals (golden ainarm2.h:48 / :85) ------------------------------
bool IndexZCanMove[2] = { false, false };   // golden ainarm2.h:48 -- per-index Z-can-move interlock
bool bShuttleKnock    = false;              // golden ainarm2.h:85 -- shuttle-knock-in-progress flag

// ---- In-Shuttle Z-HOME helpers (golden csystem.cpp:24461-24530, csystem.h:296-297) ----
//  EXPOSED as a link gap by un-gating CheckInShuttleSensor_Latch: the latch SM
//  references InitDoInShZHome()/DoInShZHome() (cases ~2004-2197), but the golden
//  definitions live in csystem.cpp's W7-gated region (translated csystem.cpp marks
//  these TODO(W7)).  Offline these are NEVER executed -- the latch's verbatim entry
//  guard returns 1 BEFORE its switch under the sim canary -- so a faithful link stub
//  suffices: Init is a no-op; DoInShZHome converges immediately (golden case-120
//  terminal sets bResult=true) -> returns true ("Z home done") on the Sim HAL, which
//  has no real In-Shuttle Z motor to home.  iInShZHomeTask owned here per golden :24461.
int  iInShZHomeTask = 1;                                                       // golden csystem.cpp:24461
void InitDoInShZHome()              { iInShZHomeTask = 1; }                     // golden csystem.cpp:24462-24465
bool DoInShZHome(int /*iShuttle*/)  { return true; }                           // golden csystem.cpp:24467 (Sim HAL: Z-home done)

#endif // HT9045_AINARM_W7_SHIMS_DEFINED
