// =============================================================================
//  Motor/Hontech_M4.cpp  -- Hontech MotionNet M4/M204 dispatch shim
//
//  Faithful translation of golden Motor/Hontech_M4.cpp (1987 lines, BCB6, pure
//  ASCII -- zero non-ASCII bytes, no Big5/cp950 decoding needed).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (motor group).
//
//  ROLE: 66 free functions, all named _Hon_m4_<verb>, all PASCAL calling
//  convention.  Every one of them: (1) reads RingData[RingNo].IP[SlaveIP] to
//  find the per-axis T-curve/S-curve parameter cache (M2X4_Param) when the
//  call needs it, (2) asks _mnet_get_slave_type_mem() which physical MotionNet
//  slave type answers at (RingNo,SlaveIP), then (3) fans out to either the
//  G9004_M204 arm (SYNTEK-M204: some calls translate directly to a
//  _mnet_m204_* primitive, others first cache T/S-curve speed profile fields
//  into RingData and replay them on the next motion call -- e.g.
//  _Hon_m4_set_tmove_speed just stores StrVel/MaxVel/Tacc/Tdec into
//  M2x4ParamInfo->M2X4_T_Curve_* and returns ERR_NoError; the actual
//  _mnet_m204_tv_move happens later, inside _Hon_m4_v_move / _start_r_move /
//  _start_a_move / _sync_r_move / etc., which read the cached T_Curve_Flag /
//  S_Curve_Flag to decide whether the M204 arm exists at all -- so v_move on
//  an M204 slave with neither flag set silently falls through to
//  ReturnCode=0 -> ERR_NoError, a golden quirk preserved verbatim, not
//  "fixed"), or the G9004_M104 arm (plain _mnet_m4_* 1:1 pass-through), or
//  else returns ERR_FunctionNotSupportThisDevice for any other SlaveType.
//  This file is a pure protocol-translation layer: it owns ONE file-scope
//  global (RingData[8], golden :9) and touches no other global in the tree.
//
//  WAVE SCOPE -- ACTIVE (faithful, verbatim -- all 66 golden functions, whole
//  file, every branch, every magic number, every golden comment preserved):
//    _Hon_m4_initial                      golden :11-66
//    _Hon_m4_set_pls_outmode              golden :69-96
//    _Hon_m4_set_pls_iptmode              golden :98-125
//    _Hon_m4_set_feedback_src             golden :127-154
//    _Hon_m4_set_alm                      golden :157-184
//    _Hon_m4_set_inp                      golden :186-213
//    _Hon_m4_set_erc                      golden :215-242
//    _Hon_m4_set_erc_on                   golden :244-271
//    _Hon_m4_set_svon                     golden :273-300
//    _Hon_m4_set_ralm                     golden :302-329
//    _Hon_m4_set_sd                       golden :331-358
//    _Hon_m4_set_ell                      golden :360-390
//    _Hon_m4_get_io_status                golden :393-421
//    _Hon_m4_v_move                       golden :424-467
//    _Hon_m4_set_tmove_speed              golden :470-502
//    _Hon_m4_set_smove_speed              golden :504-538
//    _Hon_m4_start_r_move                 golden :540-573
//    _Hon_m4_start_a_move                 golden :575-608
//    _Hon_m4_start_tr_line2               golden :610-637
//    _Hon_m4_start_ta_line2               golden :639-666
//    _Hon_m4_start_sr_line2               golden :668-695
//    _Hon_m4_start_sa_line2               golden :697-724
//    _Hon_m4_start_tr_line3               golden :726-753
//    _Hon_m4_start_ta_line3               golden :755-782
//    _Hon_m4_start_sr_line3               golden :784-811
//    _Hon_m4_start_sa_line3               golden :813-840
//    _Hon_m4_start_tr_line4               golden :842-869
//    _Hon_m4_start_ta_line4               golden :871-898
//    _Hon_m4_start_sr_line4               golden :900-927
//    _Hon_m4_start_sa_line4               golden :929-956
//    _Hon_m4_sd_stop                      golden :959-992
//    _Hon_m4_emg_stop                     golden :994-1021
//    _Hon_m4_motion_done                  golden :1024-1051
//    _Hon_m4_set_home_config              golden :1054-1083
//    _Hon_m4_start_home_move              golden :1085-1128
//    _Hon_m4_get_command                  golden :1131-1158
//    _Hon_m4_set_command                  golden :1160-1187
//    _Hon_m4_reset_command                golden :1189-1216
//    _Hon_m4_get_position                 golden :1218-1245
//    _Hon_m4_set_position                 golden :1247-1274
//    _Hon_m4_reset_position               golden :1276-1303
//    _Hon_m4_get_current_speed            golden :1305-1332
//    _Hon_m4_set_ltc_logic                golden :1335-1362
//    _Hon_m4_get_latch_data               golden :1364-1391
//    _Hon_m4_set_trigger_comparator_mode  golden :1393-1431
//    _Hon_m4_set_trigger_comparator_data  golden :1433-1459
//    _Hon_m4_load_motion_file             golden :1462-1489
//    _Hon_m4_sync_r_move                  golden :1492-1525
//    _Hon_m4_sync_a_move                  golden :1527-1560
//    _Hon_m4_set_triggered_r_move         golden :1563-1596
//    _Hon_m4_set_triggered_a_move         golden :1598-1631
//    _Hon_m4_set_triggered_tr_line2       golden :1633-1660
//    _Hon_m4_set_triggered_ta_line2       golden :1662-1689
//    _Hon_m4_set_triggered_sr_line2       golden :1691-1718
//    _Hon_m4_set_triggered_sa_line2       golden :1720-1747
//    _Hon_m4_set_continuous_move          golden :1750-1777
//    _Hon_m4_set_FIFO_latch_src           golden :1779-1797
//    _Hon_m4_get_latch_data_from_buffer   golden :1799-1817
//    _Hon_m4_reset_latch_FIFO             golden :1819-1837
//    _Hon_m4_get_latch_FIFO_length        golden :1839-1857
//    _Hon_m4_set_axis_counter             golden :1859-1877
//    _Hon_m4_get_axis_counter             golden :1879-1897
//    _Hon_m4_set_velocity_limit           golden :1899-1926
//    _Hon_m4_set_soft_limit               golden :1928-1946
//    _Hon_m4_enable_soft_limit            golden :1948-1966
//    _Hon_m4_disable_soft_limit           golden :1968-1986
//  Plus the one file-scope global: Ring_Slot RingData[8]  golden :9.
//  SATISFIED-BY-SUBSTRATE: none -- this unit has no dependency on any other
//  translated .cpp in the tree; every symbol it calls beyond its own 66
//  functions is a vendor DLL entry point declared in Motor/vendor/CMNet.h.
//
//  GATE REGISTER -- NONE.  Every _mnet_m4_* / _mnet_m204_* /
//  _mnet_get_slave_type_mem symbol this file calls was checked against
//  Motor/vendor/CMNet.h (grepped 20260807: extracted every distinct
//  _mnet_m204_xxx / _mnet_m4_xxx identifier referenced in Hontech_M4.cpp and
//  diffed against the same pattern over CMNet.h -- the diff is EMPTY, i.e.
//  all 196 distinct call sites resolve to a declaration already staged in
//  this tree Motor/vendor/CMNet.h).  These are real DLL-imported vendor entry
//  points (DLLDIR __declspec(dllimport) in golden own convention), the same
//  class of external call already accepted without gating for every other
//  CSmc.h / CMNet.h consumer in this tree (see group guidance: include them,
//  never edit them) -- not a "no compiled body anywhere in the port tree" gap
//  in the HARD RULE 6 sense, since the body legitimately lives in the vendor
//  DLL/import-lib, never in this source tree, in golden either.
//
//  VCL/Borland conversions: #pragma hdrstop (golden :2) and #pragma
//  package(smart_init) (golden :8) dropped.  No __fastcall / __property /
//  __published anywhere in this file -- golden has none (it is a plain
//  PASCAL-calling-convention C dispatch shim, never a VCL class).  Golden two
//  commented-out includes (//#include "CMNet.h" / //#include "CMNETError.h",
//  golden :6-7) are preserved verbatim as dead comments for provenance --
//  they were already commented out in golden, this translation changes
//  nothing about them.
//
//  INCLUDE-GRAPH NOTE: see Hontech_M4.h own banner for why that header now
//  includes "CMNETError.h" directly (golden relied on MachineDefine.h
//  now-gated god-include-list to supply it transitively; this tree de-VCL'd
//  MachineDefine.h no longer does).  Nothing in this .cpp own body changed to
//  compensate -- the fix lives entirely in the header.
//
//  BUILD INTEGRATION NOTE (for the main loop, not resolved by this wave):
//  this file is not yet wired into CMakeLists.txt ht9045_motor target (that
//  target own comment block still says brand drivers are "deferred to
//  W4-part2").  When it is wired in, target_include_directories(ht9045_motor
//  ...) needs Motor/vendor added so the bare #include "CMNET.h" /
//  "CMNETError.h" / "Type_def.h" in Hontech_M4.h resolve (this translator own
//  -fsyntax-only self-check passes that -I explicitly; see the command
//  reported in the structured result).
//
//  Big5: N/A -- golden file is pure ASCII (verified before translation: 0
//  bytes > 0x7F in the golden .cpp AND .h).  Final gate: ZERO U+FFFD
//  (trivially satisfied).
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "Hontech_M4.h"
//   //#include "CMNet.h"          -- already commented out in golden
//   //#include "CMNETError.h"     -- already commented out in golden
//   ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + portable STL + using namespace std); windows.h supplies the PASCAL calling-convention macro
#include "Hontech_M4.h"             // this unit own contract: M2X4_Param/Device_Slot/Ring_Slot + the 66 _Hon_m4_* prototypes; also now pulls CMNETError.h (see that header banner)

//------------------------------------------------------------------------------
Ring_Slot RingData[8];
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_initial( U16 RingNo, U16 SlaveIP )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    I16 i = 0;
    pDevice_Slot DeviceInfo;

    DeviceInfo = &RingData[RingNo].IP[SlaveIP];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    for(i=0; i<4; i++)
    {
        DeviceInfo->Axis[i].M2X4_ell_mode = 0;
        DeviceInfo->Axis[i].M2X4_T_Curve_Flag = 0;
        DeviceInfo->Axis[i].M2X4_T_Curve_StrVel = 0;
        DeviceInfo->Axis[i].M2X4_T_Curve_MaxVel = 1000;
        DeviceInfo->Axis[i].M2X4_T_Curve_Tacc = 0.1;
        DeviceInfo->Axis[i].M2X4_T_Curve_Tdec = 0.1;

        DeviceInfo->Axis[i].M2X4_S_Curve_Flag = 0;
        DeviceInfo->Axis[i].M2X4_S_Curve_StrVel = 0;
        DeviceInfo->Axis[i].M2X4_S_Curve_MaxVel = 1000;
        DeviceInfo->Axis[i].M2X4_S_Curve_Tacc = 0.1;
        DeviceInfo->Axis[i].M2X4_S_Curve_Tdec = 0.1;
        DeviceInfo->Axis[i].M2X4_S_Curve_SVacc = 200;
        DeviceInfo->Axis[i].M2X4_S_Curve_SVdec = 200;

        DeviceInfo->Axis[i].Pulse_width = 2047;
        DeviceInfo->Axis[i].TriggerType = 0;

        DeviceInfo->Axis[i].CmpMethod = 0;
        DeviceInfo->Axis[i].CmpSrc = 0;
    }

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_initial(RingNo, SlaveIP);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_initial(RingNo, SlaveIP);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Pulse I/O Configuration
I16 PASCAL _Hon_m4_set_pls_outmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_outmode)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_pls_outmode( RingNo, SlaveIP, AxisNo, pls_outmode);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_pls_outmode( RingNo, SlaveIP, AxisNo, pls_outmode);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_pls_iptmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_iptmode, U16 pls_iptdir)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_pls_iptmode( RingNo, SlaveIP, AxisNo, pls_iptmode, pls_iptdir);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_pls_iptmode( RingNo, SlaveIP, AxisNo, pls_iptmode, pls_iptdir);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_feedback_src( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 FbkSrc )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_feedback_src( RingNo, SlaveIP, AxisNo, FbkSrc );

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_feedback_src( RingNo, SlaveIP, AxisNo, FbkSrc );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Interface I/O Configuration
I16 PASCAL _Hon_m4_set_alm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 alm_logic, U16 alm_mode)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_alm( RingNo, SlaveIP, AxisNo, alm_logic, alm_mode);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_alm( RingNo, SlaveIP, AxisNo, alm_logic, alm_mode);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_inp( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 inp_enable, U16 inp_logic )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_inp( RingNo, SlaveIP, AxisNo, inp_enable, inp_logic );

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_inp( RingNo, SlaveIP, AxisNo, inp_enable, inp_logic );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_erc( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 erc_logic, U16 erc_on_time, U16 erc_off_time)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_erc( RingNo, SlaveIP, AxisNo, erc_logic, erc_on_time, erc_off_time);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_erc( RingNo, SlaveIP, AxisNo, erc_logic, erc_on_time, erc_off_time);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_erc_on( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 on_off )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_erc_on( RingNo, SlaveIP, AxisNo, on_off );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_erc_on( RingNo, SlaveIP, AxisNo, on_off );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_svon( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_svon( RingNo, SlaveIP, AxisNo, on_off );

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_svon( RingNo, SlaveIP, AxisNo, on_off );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_ralm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_ralm( RingNo, SlaveIP, AxisNo, on_off );

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_ralm( RingNo, SlaveIP, AxisNo, on_off );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_sd( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_sd( RingNo, SlaveIP, AxisNo, sd_enable, sd_logic, sd_latch, sd_mode);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_sd( RingNo, SlaveIP, AxisNo, sd_enable, sd_logic, sd_latch, sd_mode);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_ell( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 ell_logic )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_el( RingNo, SlaveIP, AxisNo, M2x4ParamInfo->M2X4_ell_mode, ell_logic );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_ell( RingNo, SlaveIP, AxisNo, ell_logic );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// I/O Status
I16 PASCAL _Hon_m4_get_io_status( U16 RingNo, U16 SlaveIP, U16 AxisNo, U32 *IO_status)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    U16 M2x4Io_sts = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_io_status( RingNo, SlaveIP, AxisNo, &M2x4Io_sts) ;
        *IO_status = (U32)M2x4Io_sts;
            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_get_io_status( RingNo, SlaveIP, AxisNo, IO_status);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Velocity Control
I16 PASCAL _Hon_m4_v_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
        {
            if(Dir == 1)
                ReturnCode =_mnet_m204_tv_move(RingNo, SlaveIP, AxisNo, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc);
            else
                ReturnCode =_mnet_m204_tv_move(RingNo, SlaveIP, AxisNo, -1 * M2x4ParamInfo->M2X4_T_Curve_StrVel, -1 * M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc);
        }
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
        {
            if(Dir == 1)
                ReturnCode =_mnet_m204_sv_move(RingNo, SlaveIP, AxisNo, M2x4ParamInfo->M2X4_S_Curve_StrVel, M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc, M2x4ParamInfo->M2X4_S_Curve_SVacc);
            else
                ReturnCode =_mnet_m204_sv_move(RingNo, SlaveIP, AxisNo, -1 * M2x4ParamInfo->M2X4_S_Curve_StrVel, -1 * M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc, M2x4ParamInfo->M2X4_S_Curve_SVacc);
        }

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_v_move( RingNo, SlaveIP, AxisNo, Dir);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Position Control
I16 PASCAL _Hon_m4_set_tmove_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        M2x4ParamInfo->M2X4_T_Curve_Flag = 1;
        M2x4ParamInfo->M2X4_S_Curve_Flag = 0;
        M2x4ParamInfo->M2X4_T_Curve_StrVel = StrVel;
        M2x4ParamInfo->M2X4_T_Curve_MaxVel = MaxVel;
        M2x4ParamInfo->M2X4_T_Curve_Tacc = Tacc;
        M2x4ParamInfo->M2X4_T_Curve_Tdec = Tdec;

        return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_tmove_speed( RingNo, SlaveIP, AxisNo, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_smove_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        M2x4ParamInfo->M2X4_S_Curve_Flag = 1;
        M2x4ParamInfo->M2X4_T_Curve_Flag = 0;
        M2x4ParamInfo->M2X4_S_Curve_StrVel = StrVel;
        M2x4ParamInfo->M2X4_S_Curve_MaxVel = MaxVel;
        M2x4ParamInfo->M2X4_S_Curve_Tacc = Tacc;
        M2x4ParamInfo->M2X4_S_Curve_Tdec = Tdec;
        M2x4ParamInfo->M2X4_S_Curve_SVacc = SVacc;
        M2x4ParamInfo->M2X4_S_Curve_SVdec = SVdec;

        return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_smove_speed( RingNo, SlaveIP, AxisNo, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
            ReturnCode =_mnet_m204_start_tr_move(RingNo, SlaveIP, AxisNo, (F64)Distance, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc, M2x4ParamInfo->M2X4_T_Curve_Tdec);
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
            ReturnCode =_mnet_m204_start_sr_move(RingNo, SlaveIP, AxisNo, (F64)Distance, M2x4ParamInfo->M2X4_S_Curve_StrVel, M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc, M2x4ParamInfo->M2X4_S_Curve_Tdec, M2x4ParamInfo->M2X4_S_Curve_SVacc, M2x4ParamInfo->M2X4_S_Curve_SVdec);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_r_move( RingNo, SlaveIP, AxisNo, Distance);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
            ReturnCode =_mnet_m204_start_ta_move(RingNo, SlaveIP, AxisNo, (F64)Pos, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc, M2x4ParamInfo->M2X4_T_Curve_Tdec);
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
            ReturnCode =_mnet_m204_start_sa_move(RingNo, SlaveIP, AxisNo, (F64)Pos, M2x4ParamInfo->M2X4_S_Curve_StrVel, M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc, M2x4ParamInfo->M2X4_S_Curve_Tdec, M2x4ParamInfo->M2X4_S_Curve_SVacc, M2x4ParamInfo->M2X4_S_Curve_SVdec);

        if(ReturnCode != 0)
            return ReturnCode;
        else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_a_move( RingNo, SlaveIP, AxisNo, Pos);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_tr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_tr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_tr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_ta_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_ta_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_ta_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_sr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_sr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_sr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_sa_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_sa_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_sa_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_tr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_tr_line3( RingNo, SlaveIP, AxisArray, DistX, DistY, DistZ, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_tr_line3( RingNo, SlaveIP, AxisArray, DistX, DistY, DistZ, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_ta_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_ta_line3( RingNo, SlaveIP, AxisArray, PosX, PosY, PosZ, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_ta_line3( RingNo, SlaveIP, AxisArray, PosX, PosY, PosZ, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_sr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_sr_line3( RingNo, SlaveIP, AxisArray, DistX, DistY, DistZ, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_sr_line3( RingNo, SlaveIP, AxisArray, DistX, DistY, DistZ, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_sa_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec , F64 SVacc, F64 SVdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_sa_line3( RingNo, SlaveIP, AxisArray, PosX, PosY, PosZ, StrVel, MaxVel, Tacc, Tdec , SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_sa_line3( RingNo, SlaveIP, AxisArray, PosX, PosY, PosZ, StrVel, MaxVel, Tacc, Tdec , SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_tr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_tr_line4( RingNo, SlaveIP, DistX, DistY, DistZ, DistU, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_tr_line4( RingNo, SlaveIP, DistX, DistY, DistZ, DistU, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_ta_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_ta_line4( RingNo, SlaveIP, PosX, PosY, PosZ, PosU, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_ta_line4( RingNo, SlaveIP, PosX, PosY, PosZ, PosU, StrVel, MaxVel, Tacc, Tdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_sr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_sr_line4( RingNo, SlaveIP, DistX, DistY, DistZ, DistU, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_sr_line4( RingNo, SlaveIP, DistX, DistY, DistZ, DistU, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_sa_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_start_sa_line4( RingNo, SlaveIP, PosX, PosY, PosZ, PosU, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_sa_line4( RingNo, SlaveIP, PosX, PosY, PosZ, PosU, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Stop
I16 PASCAL _Hon_m4_sd_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
            ReturnCode = _mnet_m204_sd_stop( RingNo, SlaveIP, AxisNo, M2x4ParamInfo->M2X4_T_Curve_Tdec);
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
            ReturnCode = _mnet_m204_sd_stop( RingNo, SlaveIP, AxisNo, M2x4ParamInfo->M2X4_S_Curve_Tdec);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_sd_stop( RingNo, SlaveIP, AxisNo );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_emg_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_emg_stop( RingNo, SlaveIP, AxisNo );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_emg_stop( RingNo, SlaveIP, AxisNo );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Motion Status
I16 PASCAL _Hon_m4_motion_done( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *MoSt )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_motion_done( RingNo, SlaveIP, AxisNo, MoSt );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_motion_done( RingNo, SlaveIP, AxisNo, MoSt );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Home
I16 PASCAL _Hon_m4_set_home_config( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(home_mode == 1)
            home_mode = 2;
        ReturnCode = _mnet_m204_set_home_config( RingNo, SlaveIP, AxisNo, home_mode, org_logic, ez_logic, ez_count, erc_out );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_home_config( RingNo, SlaveIP, AxisNo, home_mode, org_logic, ez_logic, ez_count, erc_out );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_start_home_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
        {
            if(Dir == 1)
                ReturnCode = _mnet_m204_start_home_move( RingNo, SlaveIP, AxisNo, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc);
            else
                ReturnCode = _mnet_m204_start_home_move( RingNo, SlaveIP, AxisNo, -1 * M2x4ParamInfo->M2X4_T_Curve_StrVel, -1 * M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc);
        }
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
        {
            if(Dir == 1)
                ReturnCode = _mnet_m204_start_home_move( RingNo, SlaveIP, AxisNo, M2x4ParamInfo->M2X4_S_Curve_StrVel, M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc);
            else
                ReturnCode = _mnet_m204_start_home_move( RingNo, SlaveIP, AxisNo, -1 * M2x4ParamInfo->M2X4_S_Curve_StrVel, -1 * M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc);
        }

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_start_home_move( RingNo, SlaveIP, AxisNo, Dir);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Counter Operating
I16 PASCAL _Hon_m4_get_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Cmd)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_command( RingNo, SlaveIP, AxisNo, Cmd);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_get_command( RingNo, SlaveIP, AxisNo, Cmd);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Cmd)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_command( RingNo, SlaveIP, AxisNo, Cmd);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_command( RingNo, SlaveIP, AxisNo, Cmd);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_reset_command( U16 RingNo, U16 SlaveIP, U16 AxisNo )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_reset_command( RingNo, SlaveIP, AxisNo );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_reset_command( RingNo, SlaveIP, AxisNo );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_get_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Pos)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_position( RingNo, SlaveIP, AxisNo, Pos);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_get_position( RingNo, SlaveIP, AxisNo, Pos);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_position( RingNo, SlaveIP, AxisNo, Pos);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_position( RingNo, SlaveIP, AxisNo, Pos);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_reset_position( U16 RingNo, U16 SlaveIP, U16 AxisNo )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_reset_position( RingNo, SlaveIP, AxisNo );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_reset_position( RingNo, SlaveIP, AxisNo );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_get_current_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 *speed)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_current_speed( RingNo, SlaveIP, AxisNo, speed);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_get_current_speed( RingNo, SlaveIP, AxisNo, speed);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Position Compare and Latch
I16 PASCAL _Hon_m4_set_ltc_logic( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 ltc_logic )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_ltc_logic( RingNo, SlaveIP, AxisNo, ltc_logic );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_ltc_logic( RingNo, SlaveIP, AxisNo, ltc_logic );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_get_latch_data( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 LatchNo, F64 *Pos)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_latch_data( RingNo, SlaveIP, AxisNo, LatchNo, ((I32*) Pos));

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_get_latch_data( RingNo, SlaveIP, AxisNo, LatchNo, Pos);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_trigger_comparator_mode( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpMethod)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(CmpMethod != 0)
            M2x4ParamInfo->CmpMethod = CmpMethod + 2; //M1X4 Different with M2X4

        ReturnCode +=_mnet_m204_set_trigger_type(RingNo, SlaveIP, AxisNo, M2x4ParamInfo->TriggerType);

        if(AxisNo < 2) //M2X4 CompareNo only have 2
            ReturnCode +=_mnet_m204_trigger_output_pulse_width(RingNo, SlaveIP, AxisNo, M2x4ParamInfo->Pulse_width);

        ReturnCode +=_mnet_m204_set_trigger_comparator(RingNo, SlaveIP, AxisNo, M2x4ParamInfo->CmpSrc, M2x4ParamInfo->CmpMethod, M2x4ParamInfo->CmpData);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_trigger_comparator_mode( RingNo, SlaveIP, AxisNo, CmpMethod);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_trigger_comparator_data( U16 RingNo, U16 SlaveIP, U16 AxisNo,  F64 Data )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        M2x4ParamInfo->CmpData = Data;
        return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_trigger_comparator_data( RingNo, SlaveIP, AxisNo,  Data );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Load Configuration File
I16 PASCAL _Hon_m4_load_motion_file( U16 RingNo, U16 SlaveIP, char *FilePath )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_load_motion_file( RingNo, SlaveIP, FilePath );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_load_motion_file( RingNo, SlaveIP, FilePath );

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Synchronous Move - STA
I16 PASCAL _Hon_m4_sync_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
            ReturnCode = _mnet_m204_sync_tr_move( RingNo, SlaveIP, AxisNo, Distance, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc, M2x4ParamInfo->M2X4_T_Curve_Tdec);
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
            ReturnCode = _mnet_m204_sync_sr_move( RingNo, SlaveIP, AxisNo, Distance, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc, M2x4ParamInfo->M2X4_T_Curve_Tdec, M2x4ParamInfo->M2X4_S_Curve_SVacc, M2x4ParamInfo->M2X4_S_Curve_SVdec);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_sync_r_move( RingNo, SlaveIP, AxisNo, Distance);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_sync_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
            ReturnCode = _mnet_m204_sync_ta_move( RingNo, SlaveIP, AxisNo, Pos, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc, M2x4ParamInfo->M2X4_T_Curve_Tdec);
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
            ReturnCode = _mnet_m204_sync_sa_move( RingNo, SlaveIP, AxisNo, Pos, M2x4ParamInfo->M2X4_S_Curve_StrVel, M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc, M2x4ParamInfo->M2X4_S_Curve_Tdec, M2x4ParamInfo->M2X4_S_Curve_SVacc, M2x4ParamInfo->M2X4_S_Curve_SVdec);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_sync_a_move( RingNo, SlaveIP, AxisNo, Pos);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Synchronous Move - internal
I16 PASCAL _Hon_m4_set_triggered_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance, U16 SrcAxisNo, U8 timing)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
            ReturnCode = _mnet_m204_trigger_tr_move( RingNo, SlaveIP, AxisNo, Distance, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc, M2x4ParamInfo->M2X4_T_Curve_Tdec, SrcAxisNo, timing);
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
            ReturnCode = _mnet_m204_tirgger_sr_move( RingNo, SlaveIP, AxisNo, Distance, M2x4ParamInfo->M2X4_S_Curve_StrVel, M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc, M2x4ParamInfo->M2X4_S_Curve_Tdec, M2x4ParamInfo->M2X4_S_Curve_SVacc, M2x4ParamInfo->M2X4_S_Curve_SVdec, SrcAxisNo, timing);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_triggered_r_move( RingNo, SlaveIP, AxisNo, Distance, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_triggered_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos, U16 SrcAxisNo, U8 timing)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;
    pM2X4_Param M2x4ParamInfo;

    M2x4ParamInfo = &RingData[RingNo].IP[SlaveIP].Axis[AxisNo];

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        if(M2x4ParamInfo->M2X4_T_Curve_Flag == 1)
            ReturnCode = _mnet_m204_trigger_ta_move( RingNo, SlaveIP, AxisNo, Pos, M2x4ParamInfo->M2X4_T_Curve_StrVel, M2x4ParamInfo->M2X4_T_Curve_MaxVel, M2x4ParamInfo->M2X4_T_Curve_Tacc, M2x4ParamInfo->M2X4_T_Curve_Tdec, SrcAxisNo, timing);
        else if(M2x4ParamInfo->M2X4_S_Curve_Flag == 1)
            ReturnCode = _mnet_m204_tirgger_sa_move( RingNo, SlaveIP, AxisNo, Pos, M2x4ParamInfo->M2X4_S_Curve_StrVel, M2x4ParamInfo->M2X4_S_Curve_MaxVel, M2x4ParamInfo->M2X4_S_Curve_Tacc, M2x4ParamInfo->M2X4_S_Curve_Tdec, M2x4ParamInfo->M2X4_S_Curve_SVacc, M2x4ParamInfo->M2X4_S_Curve_SVdec, SrcAxisNo, timing);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_triggered_a_move( RingNo, SlaveIP, AxisNo, Pos, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_triggered_tr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_trigger_tr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_triggered_tr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_triggered_ta_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_trigger_ta_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_triggered_ta_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_triggered_sr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_trigger_sr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_triggered_sr_line2( RingNo, SlaveIP, AxisArray, DistX, DistY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_triggered_sa_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_trigger_sa_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_triggered_sa_line2( RingNo, SlaveIP, AxisArray, PosX, PosY, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec, SrcAxisNo, timing);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
// Others
I16 PASCAL _Hon_m4_set_continuous_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Enable)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_continuous_move( RingNo, SlaveIP, AxisNo, Enable);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_continuous_move( RingNo, SlaveIP, AxisNo, Enable);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_FIFO_latch_src(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 LatchInputNo, U16 Enable)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_FIFO_latch_src( RingNo, SlaveIP, AxisCounterNo, LatchInputNo, Enable);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_get_latch_data_from_buffer( U16 RingNo, U16 SlaveIP, U16 BufferNo, U16 *AxisCounterNo, U16 *LatchDataCnt, I32 *LatchDataTable)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_latch_data_from_buffer( RingNo, SlaveIP, BufferNo, AxisCounterNo, LatchDataCnt, LatchDataTable);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_reset_latch_FIFO(U16 RingNo, U16 SlaveIP)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_reset_latch_FIFO( RingNo, SlaveIP);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_get_latch_FIFO_length(U16 RingNo, U16 SlaveIP, U16 *length)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_latch_FIFO_length( RingNo, SlaveIP, length);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 CntMode, U16 CntDir,I32 SetValue)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_axis_counter( RingNo, SlaveIP, AxisCounterNo, CntMode, CntDir, SetValue);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_get_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, I32 *GetValue)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_get_axis_counter( RingNo, SlaveIP, AxisCounterNo, GetValue);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_velocity_limit( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel )
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_velocity_limit( RingNo, SlaveIP, AxisNo, MaxVel);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else if(SlaveType == G9004_M104)
    {
        ReturnCode = _mnet_m4_set_velocity_limit( RingNo, SlaveIP, AxisNo, MaxVel);

        if(ReturnCode != 0)
            return ReturnCode;
        else
            return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_set_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 PLimit, I32 NLimit)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_set_soft_limit( RingNo, SlaveIP, AxisNo, PLimit, NLimit);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_enable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Action)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_enable_soft_limit( RingNo, SlaveIP, AxisNo, Action);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
I16 PASCAL _Hon_m4_disable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo)
{
    U8 SlaveType = 0;
    I16 ReturnCode = 0;

    _mnet_get_slave_type_mem( RingNo, SlaveIP, &SlaveType);

    if(SlaveType == G9004_M204)
    {
        ReturnCode = _mnet_m204_disable_soft_limit( RingNo, SlaveIP, AxisNo);

            if(ReturnCode != 0)
                return ReturnCode;
            else
                return ERR_NoError;
    }
    else
        return ERR_FunctionNotSupportThisDevice;
}
//------------------------------------------------------------------------------
