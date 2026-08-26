#include "MachineDefine.h"
#pragma hdrstop

#include "Hontech_M4.h"

//#include "CMNet.h"
//#include "CMNETError.h"
#pragma package(smart_init)
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
