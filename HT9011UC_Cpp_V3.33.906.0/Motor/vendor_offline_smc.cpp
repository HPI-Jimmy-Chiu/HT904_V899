// =============================================================================
//  Motor/vendor_offline_smc.cpp
//    OFFLINE LINK LAYER for the CONTEC SMC motion-controller SDK (SmcW* API).
//
//  AI(W906-PT-W5a-integrate) 20260809.  NOT a translation of any golden unit -- a
//  build artefact, and the THIRD of its family.  Read
//  Motor/vendor_offline_motionnet.cpp's banner first; the reasoning is identical
//  and only the parts specific to SMC are repeated here.
//
//  WHY IT EXISTS -- AND WHY THE BUILD WAS GREEN WITHOUT IT UNTIL NOW
//  Motor/mySMCmotor.cpp has been in ht9045_motor for waves, and its 46 SmcW* calls
//  have been unresolved the whole time.  Nothing ever broke, because a static
//  archive member is extracted ONLY to resolve a symbol that is still undefined --
//  and NOTHING in the tree referenced TMySMCMotor, so mySMCmotor.cpp.obj was never
//  pulled and its 46 undefined references were never looked at.
//    PT-W5a changed that.  cinitial.cpp's newly translated InitialMotorParameter()
//  contains golden's driver ladder, and one arm of it is
//  `MOT[i].Motor = new TMySMCMotor(iAdder)` -- the first and only consumer of that
//  class in this tree.  The linker now extracts the member, and the 46 references
//  become 46 hard errors.
//    MEASURED, not predicted (the audit that found it did this, and I re-ran it):
//      nm --undefined-only libht9045_motor.a  -> mySMCmotor.cpp.obj carries exactly
//        46 distinct `_SmcW*@N` symbols
//      nm --defined-only lib*.a (all archives) -> ZERO of them defined anywhere
//    CSmc.h declares them `long WINAPI SmcWxxx(...)` inside extern "C" -- WINAPI is
//  plain __stdcall with NO __declspec(dllimport) -- so callers already emit direct
//  `_SmcWxxx@N` references that a static definition satisfies.  No macro lever is
//  needed here (unlike MN200, which required MN200DLL_EXPORTS).  Checked before
//  writing this.
//
//  RETURN SEMANTICS -- non-zero, i.e. "the call failed"
//  CSmc.h ships no error-code table (its only error-related entry point is
//  SmcWGetErrorString(long ErrorCode, char*)), so there is no vendor constant to
//  quote the way the MotionNet layer quotes ERROR_NO_CARD_FOUND.  The convention is
//  read off the CALLER instead: Motor/mySMCmotor.cpp:395-398 does
//      ret = SmcWInit(cDeviceName, &CardId[iBoardID]);
//      if(ret != 0) return false;
//  i.e. zero is success and any non-zero is failure.  Every body below therefore
//  returns SMC_OFFLINE_NO_CARD (-1).
//    Deliberately NOT 0.  Returning success would tell the ported driver that a
//  CONTEC card initialised, after which it would trust positions, limit status and
//  motion-done flags that no hardware produced.  The failure return puts it on the
//  path golden already takes on a machine where the card is absent -- a path golden
//  handles (mySMCmotor.cpp's own `return false`), rather than behaviour invented
//  here.
//
//  OUT-PARAMETERS ARE LEFT UNTOUCHED, same reason as the sibling layers: several of
//  these signatures pass pointers that are INPUTS, and a blanket memset would
//  corrupt caller data.  A caller that reads an out-param after a failure return was
//  already wrong against the real DLL.
//
//  SIGNATURES ARE NOT HAND-TYPED: every parameter list is copied verbatim from
//  Motor/vendor/CSmc.h (AGENTS.md forbidden-to-modify; included, never edited), so
//  the compiler validates all 46 signatures and the linker validates all 46 @N
//  decorations.  The emitted set is exactly the 46 that nm reported -- no more.
//
//  RETIREMENT: when a real CONTEC SMC SDK is linked, delete this file and link the
//  vendor library.  No consumer knows this layer exists.
// =============================================================================
#include "MachineDefine.h"

#include <windows.h>                 // HWND / WINAPI, needed by CSmc.h
#include "Motor/vendor/CSmc.h"       // READ-ONLY vendor header: the SmcW* declarations

//  See the banner: CSmc.h has no error-code table, so this file names the value it
//  returns instead of quoting a constant that does not exist.  -1 is simply "not 0",
//  which is what every caller in Motor/mySMCmotor.cpp tests for.
enum { SMC_OFFLINE_NO_CARD = -1 };

// Parameter names are kept verbatim from the header for readability even though no
// body reads one; this silences the resulting -Wunused-parameter noise file-wide
// rather than sprinkling casts through 46 signatures.
#pragma GCC diagnostic ignored "-Wunused-parameter"

long WINAPI SmcWCountEvent(short DevId, short AxisNo, HWND hMsgWnd, short EventMode, short CountType, long Count) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWExit(short DevId) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetCountPulse(short DevId, short AxisNo, long *CountPulse) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetDigitalIn(short DevId, short AxisNo, short *InData) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetErrorString(long ErrorCode, char *ErrorString) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetLatchDataFromBufferEx(short DevId, short BufferNo, short *AxisCounterNo, short *LatchDataCnt, long *LatchDataTable, short *UpCnt, short *DownCnt) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetLatchFIFOLength(short DevId, short AxisNo, short *length) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetLimitStatus(short DevId, short AxisNo, short *LimitSts) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetMoveStatus(short DevId, short AxisNo, short *MoveSts) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWGetOutPulse(short DevId, short AxisNo, long *OutPulse) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWInit(char * DeviceName, short *DevId) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWMotionDecStop(short DevId, short AxisNo) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWMotionStart(short DevId, short AxisNo) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWMotionStop(short DevId, short AxisNo) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWResetLatchFIFO(short DevId) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetAccelTime(short DevId, short AxisNo, double AccelTime) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankAccelTime(short DevId, short AxisNo, short BankNo, double AccelTime) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankDecelTime(short DevId, short AxisNo, short BankNo, double DecelTime) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankDistance(short DevId, short AxisNo, short BankNo, long StopPosition) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankInterpolation(short DevId, short AxisNo, short BankNo, short InterType, short InterAxis, short Reserved) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankNumber(short DevId, short AxisNo, short BankNum) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankReady(short DevId, short AxisNo, short MotionType) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankSSpeed(short DevId, short AxisNo, short BankNo, double SSpeed) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankStartSpeed(short DevId, short AxisNo, short BankNo, double StartSpeed) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetBankTargetSpeed(short DevId, short AxisNo, short BankNo, double TargetSpeed) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetCountPulse(short DevId, short AxisNo, long CountPulse) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetCtrlInOutLog(short DevId, short AxisNo, short CtrlInOutLog) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetCtrlTypeIn(short DevId, short AxisNo, short CtrlIn) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetCtrlTypeOut(short DevId, short AxisNo, short CtrlOut1, short CtrlOut2, short CtrlOut3) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetDecelTime(short DevId, short AxisNo, double DecelTime) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetDigitalOut(short DevId, short AxisNo, short OutData, short OutDataEnable) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetEncType(short DevId, short AxisNo, short EncType) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetFIFOLatchSrc(short DevId, short AxisNo, short LatchAxisNo, short Enable) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetInitParam(short DevId, short AxisNo) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetOrgLog(short DevId, short AxisNo, short OrgLog) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetOrgMode(short DevId, short AxisNo, short LimitTurn, short OrgType, short EndDir, short ZCount) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetOutPulse(short DevId, short AxisNo, long OutPulse) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetPulseType(short DevId, short AxisNo, short PulseMode, short DirTimer) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetReady(short DevId, short AxisNo, short MotionType, short StartDir) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetReadyEx(short DevId, short AxisNo, short MotionType, short StartDir) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetResolveSpeed(short DevId, short AxisNo, double ResolveSpeed) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetSoftLimit(short DevId, short AxisNo, short PLimMode, short MLimMode, long PLimCount, long MLimCount) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetStartSpeed(short DevId, short AxisNo, double StartSpeed) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetStopPosition(short DevId, short AxisNo, short Coodinate, long StopPosition) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWSetTargetSpeed(short DevId, short AxisNo, double TargetSpeed) { return SMC_OFFLINE_NO_CARD; }
long WINAPI SmcWStopEvent(short DevId, short AxisNo, HWND hMsgWnd, short EventMode) { return SMC_OFFLINE_NO_CARD; }

// -- end of the 46-entry offline CONTEC SMC layer -------------------------------
