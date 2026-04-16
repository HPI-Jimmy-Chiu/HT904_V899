#ifndef __HONTECH_M4_H__
#define __HONTECH_M4_H__

#ifdef DLLDIR_EX
#define DLLDIR  __declspec(dllexport)     // export DLL information
#else
#define DLLDIR  __declspec(dllimport)     // import DLL information
#endif

#include "Type_def.h"
#include "CMNET.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct
{
    I16 M2X4_ell_mode ;

    U8  M2X4_T_Curve_Flag ;
    F64 M2X4_T_Curve_StrVel ;
    F64 M2X4_T_Curve_MaxVel ;
    F64 M2X4_T_Curve_Tacc ;
    F64 M2X4_T_Curve_Tdec ;

    U8  M2X4_S_Curve_Flag ;
    F64 M2X4_S_Curve_StrVel ;
    F64 M2X4_S_Curve_MaxVel ;
    F64 M2X4_S_Curve_Tacc ;
    F64 M2X4_S_Curve_Tdec ;
    F64 M2X4_S_Curve_SVacc ;
    F64 M2X4_S_Curve_SVdec ;

    I16 TriggerType;
    U16 Pulse_width;

    I16 CmpSrc;
    I16 CmpMethod;
    F64 CmpData;
} M2X4_Param, *pM2X4_Param;

typedef struct
{
    M2X4_Param  Axis[4];
} Device_Slot, *pDevice_Slot;

typedef struct
{
    Device_Slot IP[64];
} Ring_Slot, *pRing_Slot;






    I16 DLLDIR PASCAL _Hon_m4_initial( U16 RingNo, U16 SlaveIP );
    //
    // Pulse I/O Configuration
    I16 DLLDIR PASCAL _Hon_m4_set_pls_outmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_outmode);
    I16 DLLDIR PASCAL _Hon_m4_set_pls_iptmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_iptmode, U16 pls_iptdir);
    I16 DLLDIR PASCAL _Hon_m4_set_feedback_src( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 FbkSrc );
    //
    // Interface I/O Configuration
    I16 DLLDIR PASCAL _Hon_m4_set_alm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 alm_logic, U16 alm_mode);
    I16 DLLDIR PASCAL _Hon_m4_set_inp( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 inp_enable, U16 inp_logic );
    I16 DLLDIR PASCAL _Hon_m4_set_erc( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 erc_logic, U16 erc_on_time, U16 erc_off_time);
    I16 DLLDIR PASCAL _Hon_m4_set_erc_on( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 on_off );
    I16 DLLDIR PASCAL _Hon_m4_set_svon( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off );
    I16 DLLDIR PASCAL _Hon_m4_set_ralm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off );
    I16 DLLDIR PASCAL _Hon_m4_set_sd( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode);
    I16 DLLDIR PASCAL _Hon_m4_set_ell( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 ell_logic );

    // I/O Status
    I16 DLLDIR PASCAL _Hon_m4_get_io_status( U16 RingNo, U16 SlaveIP, U16 AxisNo, U32 *IO_status);
    //
    // Velocity Control
    I16 DLLDIR PASCAL _Hon_m4_v_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir);

    //
    // Position Control
    I16 DLLDIR PASCAL _Hon_m4_set_tmove_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
    I16 DLLDIR PASCAL _Hon_m4_set_smove_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
    I16 DLLDIR PASCAL _Hon_m4_start_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance);
    I16 DLLDIR PASCAL _Hon_m4_start_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos);
    //

    I16 DLLDIR PASCAL _Hon_m4_start_tr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
    I16 DLLDIR PASCAL _Hon_m4_start_ta_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
    I16 DLLDIR PASCAL _Hon_m4_start_sr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
    I16 DLLDIR PASCAL _Hon_m4_start_sa_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
    //
    I16 DLLDIR PASCAL _Hon_m4_start_tr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
    I16 DLLDIR PASCAL _Hon_m4_start_ta_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
    I16 DLLDIR PASCAL _Hon_m4_start_sr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
    I16 DLLDIR PASCAL _Hon_m4_start_sa_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec , F64 SVacc, F64 SVdec );
    //
    I16 DLLDIR PASCAL _Hon_m4_start_tr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
    I16 DLLDIR PASCAL _Hon_m4_start_ta_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
    I16 DLLDIR PASCAL _Hon_m4_start_sr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
    I16 DLLDIR PASCAL _Hon_m4_start_sa_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
    //
    // Stop
    I16 DLLDIR PASCAL _Hon_m4_sd_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo );
    I16 DLLDIR PASCAL _Hon_m4_emg_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo );
    //
    // Motion Status
    I16 DLLDIR PASCAL _Hon_m4_motion_done( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *MoSt );
    //
    // Home
    I16 DLLDIR PASCAL _Hon_m4_set_home_config( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out );
    I16 DLLDIR PASCAL _Hon_m4_start_home_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir);
    //
    // Counter Operating
    I16 DLLDIR PASCAL _Hon_m4_get_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Cmd);
    I16 DLLDIR PASCAL _Hon_m4_set_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Cmd);
    I16 DLLDIR PASCAL _Hon_m4_reset_command( U16 RingNo, U16 SlaveIP, U16 AxisNo );
    I16 DLLDIR PASCAL _Hon_m4_get_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Pos);
    I16 DLLDIR PASCAL _Hon_m4_set_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos);
    I16 DLLDIR PASCAL _Hon_m4_reset_position( U16 RingNo, U16 SlaveIP, U16 AxisNo );
    I16 DLLDIR PASCAL _Hon_m4_get_current_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 *speed);
    //
    // Position Compare and Latch
    I16 DLLDIR PASCAL _Hon_m4_set_ltc_logic( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 ltc_logic );
    I16 DLLDIR PASCAL _Hon_m4_get_latch_data( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 LatchNo, F64 *Pos);
    I16 DLLDIR PASCAL _Hon_m4_set_trigger_comparator_mode( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpMethod);
    I16 DLLDIR PASCAL _Hon_m4_set_trigger_comparator_data( U16 RingNo, U16 SlaveIP, U16 AxisNo,  F64 Data );
    //
    // Load Configuration File
    I16 DLLDIR PASCAL _Hon_m4_load_motion_file( U16 RingNo, U16 SlaveIP, char *FilePath );
    //
    // Synchronous Move - STA
    I16 DLLDIR PASCAL _Hon_m4_sync_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance);
    I16 DLLDIR PASCAL _Hon_m4_sync_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos);
    //
    // Synchronous Move - internal
    I16 DLLDIR PASCAL _Hon_m4_set_triggered_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance, U16 SrcAxisNo, U8 timing);
    I16 DLLDIR PASCAL _Hon_m4_set_triggered_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos, U16 SrcAxisNo, U8 timing);
    I16 DLLDIR PASCAL _Hon_m4_set_triggered_tr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing);
    I16 DLLDIR PASCAL _Hon_m4_set_triggered_ta_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing);
    I16 DLLDIR PASCAL _Hon_m4_set_triggered_sr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing);
    I16 DLLDIR PASCAL _Hon_m4_set_triggered_sa_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing);
    //
    // Others
    I16 DLLDIR PASCAL _Hon_m4_set_continuous_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Enable);

    I16 DLLDIR PASCAL _Hon_m4_set_FIFO_latch_src(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 LatchInputNo, U16 Enable);
    I16 DLLDIR PASCAL _Hon_m4_get_latch_data_from_buffer( U16 RingNo, U16 SlaveIP, U16 BufferNo, U16 *AxisCounterNo, U16 *LatchDataCnt, I32 *LatchDataTable);
    I16 DLLDIR PASCAL _Hon_m4_reset_latch_FIFO(U16 RingNo, U16 SlaveIP);
    I16 DLLDIR PASCAL _Hon_m4_get_latch_FIFO_length(U16 RingNo, U16 SlaveIP, U16 *length);

    I16 DLLDIR PASCAL _Hon_m4_set_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 CntMode, U16 CntDir,I32 SetValue);
    I16 DLLDIR PASCAL _Hon_m4_get_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, I32 *GetValue);

    I16 DLLDIR PASCAL _Hon_m4_set_velocity_limit( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel );

    I16 DLLDIR PASCAL _Hon_m4_set_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 PLimit, I32 NLimit);
    I16 DLLDIR PASCAL _Hon_m4_enable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Action);
    I16 DLLDIR PASCAL _Hon_m4_disable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo);
    // }



#ifdef __cplusplus
    }
#endif











#endif //__HONTECH_M4_H__