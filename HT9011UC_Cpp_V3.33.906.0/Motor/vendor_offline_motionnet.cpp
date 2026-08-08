// =============================================================================
//  Motor/vendor_offline_motionnet.cpp
//    OFFLINE LINK LAYER for the MotionNet / MN200 / PCI-L1xx vendor DLLs.
//
//  AI(W906-PT-W3-integrate) 20260808.  THIS FILE IS NOT A TRANSLATION of any
//  golden unit -- it is a build artefact, in the same family as
//  acatchtray_shims.cpp / acarry_shims.cpp / canary_support.cpp: it exists so
//  the faithfully-translated motor drivers LINK on a machine that has no
//  MotionNet hardware and no vendor import library.
//
//  WHY IT WAS NEEDED (measured, not assumed)
//  ------------------------------------------------------------------------
//  Before PT-W3 the tree looked green on this, but only by accident of static
//  archives: Motor/Hontech_M4.cpp (_mnet_m4_*/_mnet_m204_*) and
//  EtherCAT/MyEtherCAT.cpp had carried these unresolved references for waves --
//  an archive member is extracted ONLY to resolve a symbol that is still
//  undefined, and nothing had ever referenced them, so the linker never looked
//  inside.  PT-W3 tripped it: Motor/mymotor.cpp lost its local `MNetLog` stub
//  (it had to go -- myMN200motor.h:192 declares `extern bool MNetLog(AnsiString)`
//  and the stub's `static void` return type made it an ambiguating
//  redeclaration, a hard error), so mymotor.cpp's three golden MNetLog call
//  sites now reference the real body in Motor/myMN200motor.cpp, which extracted
//  that object and with it 124 mn_* vendor calls.
//    Verified with nm --undefined-only over every built archive: the exact set
//  any registered TU references is 173 entry points -- 30 from mn200.h, 140
//  from CMNet.h, 1 from PCI_L112.h, 2 from PCI_L122.H.  This file defines
//  exactly those 173, no more.  (`Acm_*`, the Advantech EtherCAT family, is NOT
//  in the set and needs nothing here: every Acm_ call in the tree sits inside
//  `#ifdef SOFT_SIMULTE`, which this build does not define -- checked, e.g.
//  EtherCAT/MyEtherCAT.cpp:455 is inside the #ifdef opened at :411.)
//
//  RETURN SEMANTICS -- "there is no card", the vendor's own code for it
//  ------------------------------------------------------------------------
//  Every body returns the FAILURE constant its own vendor header defines, and
//  touches nothing:
//    mn_* / mn200_*  -> ERROR_NO_CARD_FOUND  (mn200.h:642      == -100)
//    _mnet_*         -> ERR_Invalid_Hardware (CMNETError.h:6   ==   -4)
//    _l112_*/_l122_* -> ERR_NoCardFound      (PCI_L122_Err.h:6 ==   -1)
//  This is deliberately NOT `SUCCESS`.  Returning success would tell the ported
//  drivers that a MotionNet card is present and healthy, and they would then
//  believe position/motion-done readings that never came from hardware.  The
//  failure return puts them on the SAME path golden takes on a machine where
//  bring-up fails, which golden already handles everywhere -- so no new
//  behaviour is invented here, and no motion decision is faked.
//
//  ONE CAVEAT, STATED PLAINLY: out-parameters are left UNTOUCHED rather than
//  zeroed.  Zeroing looked safer but is not -- several of these signatures pass
//  pointers that are INPUTS (mn_linen_move's `SPEED_PAR *pSpeedPar` and
//  `long DevPos[]`, mn_set_group's `BYTE bDevNo[]`, _mnet_*_load_motion_file's
//  `char *FilePath`), and a blanket memset would corrupt caller data.  A caller
//  that reads an out-param after a failure return was already wrong against the
//  real DLL; this layer does not make it more wrong.
//
//  SIGNATURES ARE NOT HAND-TYPED.  Each definition's parameter list is copied
//  verbatim from the READ-ONLY vendor header (Motor/vendor/*, never edited),
//  which means the compiler validates all 173 signatures (a mismatch is an
//  overload, and then the link stays broken and says so) and the linker
//  validates all 173 stdcall @N decorations.  That check earned its keep on the
//  first try -- see the MN200DLL_EXPORTS note below.  Default arguments are
//  dropped from the definitions only because C++ forbids repeating them.
//
//  MN200DLL_EXPORTS (see this file's entry in CMakeLists.txt) -- and the WRONG
//  lever I reached for first, written down because the trap is easy to fall into:
//  mn200.h mirrors its API THREE times and THE MIRRORS DO NOT AGREE ON
//  SIGNATURES.  `mn_fix_move` alone appears as
//      :307  SPEED_PAR *pSpeedPar                      (#ifdef MN200DLL_VB)
//      :426  double* dSpeedPar, BYTE* dSpeedPar_Opt    (a second variant)
//      :545  SPEED_PAR SpeedPar  -- BY VALUE            (the MOTIONNETDLL_API one)
//  Defining MN200DLL_VB does select plain (non-dllimport) declarations, which is
//  what a static definition needs -- but it also silently reshapes 4 signatures,
//  and the build said so immediately (10 errors, "cannot convert SPEED_PAR to
//  SPEED_PAR*", myMN200motor.cpp:696 and friends): golden was written against the
//  MOTIONNETDLL_API block.  Diffing the two blocks' NAME sets (identical 86) was
//  NOT enough -- the parameter lists had to be diffed too.
//    The correct lever is MN200DLL_EXPORTS, which mn200.h:1-5 reads to flip
//  MOTIONNETDLL_API from __declspec(dllimport) to __declspec(dllexport) WITHOUT
//  changing which declaration block is active.  dllexport makes callers emit a
//  direct `_mn_open_all@4` reference instead of the `_imp__mn_open_all@4` thunk
//  that only a real import library could satisfy, so the definitions below
//  resolve.  Probed before adopting: `nm` shows `U _mn_open_all@4` on the caller
//  side and `T _mn_open_all@4` on this side.  Every mn_*/mn200_* declaration used
//  below therefore comes from the MOTIONNETDLL_API block, and mn200.h is the only
//  file tree-wide that reads either macro.
//
//  RETIREMENT: when a real MN200 / MotionNet SDK is linked (32-bit BCB6 .lib is
//  unusable here -- see docs/ KNOWLEDGE + the driver 64-bit feasibility audit),
//  delete this file and link the vendor library instead.  Nothing else has to
//  change: no consumer knows this layer exists.
// =============================================================================
#include "MachineDefine.h"

// The vendor headers are READ-ONLY (AGENTS.md forbidden-to-modify list).  They
// are included, never edited: including them is exactly what makes the compiler
// check every signature below against the declaration the CALLERS see.
#include "Motor/vendor/mn200.h"      // mn_*/mn200_* + SUCCESS/ERROR_NO_CARD_FOUND
#include "Motor/vendor/CMNet.h"      // _mnet_* ring/M4/M204 API
#include "Motor/vendor/CMNETError.h" // ERR_Invalid_Hardware
#include "Motor/vendor/PCI_L112.h"   // _l112_open
#include "Motor/vendor/PCI_L122.H"   // _l122_open / _l122_lio_input_read
// PCI_L122_Err.h is deliberately NOT included: the two vendor error headers are
// mutually exclusive in one TU.  CMNETError.h:2 does `#define ERR_NoError 0`
// while PCI_L122_Err.h:5 declares `ERR_NoError=0` as an ENUMERATOR, so the macro
// rewrites the enumerator and the enum stops compiling.  (Tried it -- 4 errors.)
// Neither header may be edited, so the one constant needed from it is restated
// here with its citation.  Value re-read from PCI_L122_Err.h:6 this pass.
enum { W906_PCI_L1xx_ERR_NoCardFound = -1 };   // PCI_L122_Err.h:6 `ERR_NoCardFound=-1`

// Parameter names are kept verbatim from the headers for readability even though
// no body reads one; this silences the resulting -Wunused-parameter noise
// file-wide rather than sprinkling 500 `(void)x;` casts.
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ---- mn200.h -- 30 entry points, offline return ERROR_NO_CARD_FOUND
short __stdcall mn200_get_di(BYTE bCardID, BYTE* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn200_get_do(BYTE bCardID, BYTE* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn200_set_do(BYTE bCardID, BYTE bData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_clear_slave_error_flag(BYTE bLineNo, DWORD *ErrorTable) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_fix_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, long Position, BYTE bMoveType) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_ai(BYTE bLineNo, BYTE bDevNo, BYTE bChannelNo, float* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_cmdcounter(BYTE bLineNo, BYTE bDevNo, long* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_dev_info(BYTE bLineNo, BYTE bDevNo, BYTE* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_enccounter(BYTE bLineNo, BYTE bDevNo, long* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_error_status(BYTE bLineNo, BYTE bDevNo, DWORD* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_line_status(BYTE bLineNo, WORD* pData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_mdio_status(BYTE bLineNo, BYTE bDevNo, MOTION_IO* MotionIO) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_get_slave_error_table(BYTE bLineNo, DWORD ErrorTable[]) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_home_search(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection, long OrgWidth, BYTE bHomeMode, BYTE bEZcount) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_linen_move(BYTE bLineNo, BYTE bDevNo[], SPEED_PAR SpeedPar, long DevPos[], BYTE bNumDev) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_motion_done(BYTE bLineNo, BYTE bDevNo, BYTE* pDone) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_open_all(BYTE* pNumLine) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_reset(BYTE bLineNo) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_servo_on(BYTE bLineNo, BYTE bDevNo, BYTE bServoOn) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_set_cmdcounter(BYTE bLineNo, BYTE bDevNo, long Data) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_set_comm_speed(BYTE bLineNo, BYTE bCommSpeed) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_set_customization(BYTE bLineNo, BYTE bDevNo, WORD wCustParam, WORD wValue) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_set_enccounter(BYTE bLineNo, BYTE bDevNo, long Data) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_set_group(BYTE bLineNo, BYTE bGrpNo, BYTE bNumDev, BYTE bDevNo[]) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_set_motion_cfg(BYTE bLineNo, BYTE bDevNo, MotionConfig CfgItem, DWORD dwData) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_set_softlimit(BYTE bLineNo, BYTE bDevNo, BYTE bSWLimitEnable, BYTE bCmpSource, BYTE bStopMode, long LimitPositive, long LimitNegaitive) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_start_line(BYTE bLineNo, BYTE* pNumDev) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_stop_line(BYTE bLineNo) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_stop_move(BYTE bLineNo, BYTE bDevNo, BYTE bStopMode) { return ERROR_NO_CARD_FOUND; }
short __stdcall mn_velocity_move(BYTE bLineNo, BYTE bDevNo, SPEED_PAR SpeedPar, BYTE bDirection) { return ERROR_NO_CARD_FOUND; }

// ---- CMNet.h -- 140 entry points, offline return ERR_Invalid_Hardware
I16 __stdcall _mnet_clear_slave_error_flag(U16 RingNo, U32 *ErrorTable) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_get_ring_active_table(U16 RingNo, U32 *DevTable) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_get_ring_status(U16 RingNo, U16 *Status) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_get_slave_error_table(U16 RingNo, U32 *ErrorTable) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_get_slave_type(U16 RingNo, U16 SlaveIP, U8 *SlaveType) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_get_slave_type_mem(U16 RingNo, U16 SlaveIP, U8 *SlaveType) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_load_ring_active_table(U16 RingNo, U32 *DevTable, char *FilePath) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_disable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_emg_stop(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_enable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Action) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, I32 *GetValue) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_command(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *cmd) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_current_speed(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 *speed) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_io_status(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *io_sts) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_latch_FIFO_length(U16 RingNo, U16 SlaveIP, U16 *length) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_latch_data(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 LatchNo, I32 *Pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_latch_data_from_buffer(U16 RingNo, U16 SlaveIP, U16 BufferNo, U16 *AxisCounterNo, U16 *LatchDataCnt, I32 *LatchDataTable) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_get_position(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_initial(U16 RingNo, U16 SlaveIP) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_load_motion_file(U16 RingNo, U16 SlaveIP, char *FilePath) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_motion_done(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *McSts) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_reset_command(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_reset_latch_FIFO(U16 RingNo, U16 SlaveIP) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_reset_position(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_sd_stop(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_FIFO_latch_src(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 LatchInputNo, U16 Enable) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_alm(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 alm_logic, U16 alm_mode) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 CntMode, U16 CntDir, I32 SetValue) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_command(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 cmd) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_continuous_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Conti_logic) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_el(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 el_mode, I16 el_logic) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_erc(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 erc_logic, U16 erc_on_time, U16 erc_off_time) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_erc_on(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 on_off) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_feedback_src(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 FbkSrc) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_home_config(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_inp(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 inp_enable, U16 inp_logic) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_ltc_logic(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 ltc_logic) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_pls_iptmode(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_iptmode, U16 pls_iptdir) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_pls_outmode(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_outmode) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_position(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_ralm(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_sd(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 PLimit, I32 NLimit) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_svon(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_trigger_comparator(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpSrc, I16 CmpMethod, F64 Data) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_trigger_type(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 TriggerType) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_set_velocity_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_home_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sa_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sa_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sa_line4(U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sa_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sr_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sr_line4(U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_sr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_ta_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_ta_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_ta_line4(U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_ta_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_tr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_tr_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_tr_line4(U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_start_tr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_sv_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 SVacc) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_sync_sa_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_sync_sr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_sync_ta_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_sync_tr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_tirgger_sa_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 Timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_tirgger_sr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 Timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_trigger_output_pulse_width(U16 RingNo, U16 SlaveIP, U16 CompareNo, U16 Time) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_trigger_sa_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_trigger_sr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_trigger_ta_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_trigger_ta_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 Timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_trigger_tr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_trigger_tr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 Timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m204_tv_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_emg_stop(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_get_command(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Cmd) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_get_current_speed(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 *speed) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_get_io_status(U16 RingNo, U16 SlaveIP, U16 AxisNo, U32 *IO_status) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_get_latch_data(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 LatchNo, F64 *Pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_get_position(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_initial(U16 RingNo, U16 SlaveIP) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_load_motion_file(U16 RingNo, U16 SlaveIP, char *FilePath) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_motion_done(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *MoSt) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_reset_command(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_reset_position(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_sd_stop(U16 RingNo, U16 SlaveIP, U16 AxisNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_alm(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 alm_logic, U16 alm_mode) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_command(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Cmd) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_continuous_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Enable) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_ell(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 ell_logic) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_erc(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 erc_logic, U16 erc_on_time, U16 erc_off_time) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_erc_on(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 on_off) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_feedback_src(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 FbkSrc) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_home_config(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_inp(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 inp_enable, U16 inp_logic) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_ltc_logic(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 ltc_logic) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_pls_iptmode(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_iptmode, U16 pls_iptdir) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_pls_outmode(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_outmode) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_position(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_ralm(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_sd(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_smove_speed(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_svon(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_tmove_speed(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_trigger_comparator_data(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Data) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_trigger_comparator_mode(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpMethod) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_triggered_a_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_triggered_r_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_triggered_sa_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_triggered_sr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_triggered_ta_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_triggered_tr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_set_velocity_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_a_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_home_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_r_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_sa_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_sa_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_sa_line4(U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_sr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_sr_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_sr_line4(U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_ta_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_ta_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_ta_line4(U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_tr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_tr_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_start_tr_line4(U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_sync_a_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_sync_r_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_m4_v_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_reset_ring(U16 RingNo) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_save_ring_active_table(U16 RingNo, char *FilePath) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_set_ring_config(U16 RingNO, U16 BaudRate) { return ERR_Invalid_Hardware; }
I16 __stdcall _mnet_start_ring(U16 RingNo) { return ERR_Invalid_Hardware; }

// ---- PCI_L112.h -- 1 entry points, offline return W906_PCI_L1xx_ERR_NoCardFound
I16 __stdcall _l112_open(I16* existcard) { return W906_PCI_L1xx_ERR_NoCardFound; }

// ---- PCI_L122.H -- 2 entry points, offline return W906_PCI_L1xx_ERR_NoCardFound
I16 __stdcall _l122_lio_input_read(U16 CardSwitchNo, U16 *data) { return W906_PCI_L1xx_ERR_NoCardFound; }
I16 __stdcall _l122_open(I16 *existcards) { return W906_PCI_L1xx_ERR_NoCardFound; }

// -- end of the 173-entry offline vendor layer --------------------------------
