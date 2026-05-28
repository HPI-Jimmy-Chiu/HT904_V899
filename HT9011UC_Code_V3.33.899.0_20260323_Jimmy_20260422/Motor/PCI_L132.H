#include "type_def.h"
#include <windows.h>



#ifdef __cplusplus
extern "C" {
#endif



I16 PASCAL _l132_open(I16* existcard);
void PASCAL _l132_close(I16 CardNo);
I16 PASCAL _l132_word_write(I16 CardNo, U32 addr ,U16 value);
I16 PASCAL _l132_word_read(I16 CardNo, U32 addr ,U16* value);
I16 PASCAL _l132_flash_erase_all(U16 CardNo,U16 flash_id);
I16 PASCAL _l132_flash_erase_section(U16 CardNo,U16 flash_id,U16 section);
I16 PASCAL _l132_write_flash(U16 CardNo,U16 flash_id ,U32 offset,U8 Data);
I16 PASCAL _l132_read_flash(U16 CardNo,U16 flash_id ,U32 offset,U8* Data);
I16 PASCAL _l132_set_pci_int(U16 CardNo );
I16 PASCAL _l132_link_interrupt(U16 CardNo,void ( __stdcall *callbackAddr)(U16 CardNo));
I16 PASCAL _l132_dsp_pci_boot(U16 CardNo);
I16 PASCAL _l132_flash_boot_code(U16 CardNo);

I16 PASCAL _l132_position_cmp(U16 CardNo ,U32 start,U32 end,U32 interval);
I16 PASCAL _l132_position_cmp_auto_supply(U16 CardNo ,U32 start,U32 end,U32 interval,U32 count);
I16 PASCAL _l132_position_cmp_table(U16 CardNo ,I32* TriggerTable,U32 Count);

I16 PASCAL _l132_set_trigger_interval(U16 CardNo ,U16 enable);
I16 PASCAL _l132_reg_routine(U16 CardNo,U16 RingNo, U16 DeviceIP,U16 station_ip,U16 Function);
I16 PASCAL _l132_get_routine_data(U16 CardNo,U16 station_ip,void *data);
I16 PASCAL _l132_set_Z_command(U16 CardNo,U16 RingNo, U16 DeviceIP,U16 delay_cnt,U16 interval_trigger,U16 count,I32 *Z_pos,I32 Trigger_x_axis_vel,U16 Dir);
I16 PASCAL _l132_set_Z_axis_data(U16 CardNo,U16 RingNo, U16 DeviceIP,U16 delay_cnt,U16 interval_trigger,U16 count);
I16 PASCAL _l132_get_Z_axis_data(U16 CardNo,I32 *z_enc, U16 count,U16 Dir);
I16 PASCAL _l132_reset_cpld_position(U16 CardNo ,U16 AxisNo);
I16 PASCAL _l132_get_Para_data(U16 CardNo,U16 para_no,I32 *data);
I16 PASCAL _l132_set_Z_working_org(U16 CardNo,U16 RingNo, U16 DeviceIP);
I16 PASCAL _l132_mnet_start_Y_a_move( U16 CardNo, I32 Pos);
I16 PASCAL _l132_check_auto_Y_move(U16 CardNo,I16* auto_Y_status);
I16 PASCAL _l132_leave_auto_Y_move(U16 CardNo);
I16 PASCAL _l132_leave_Z_tracking(U16 CardNo,U16 RingNo, U16 DeviceIP);
I16 PASCAL _l132_Zaxis_Tracking_flag_enable(U16 CardNo,U16 on_off);
I16 PASCAL _l132_2nd_trigger(U16 CardNo ,I16 delay_cnt,U16 trigger_num,U16 Enable);
I16 PASCAL _l132_2nd_auto_trigger_enable(U16 CardNo,U16 Enable,U16 count);
I16 PASCAL _l132_get_2nd_auto_trigger(U16 CardNo,I32 *data);
I16 PASCAL _l132_get_cmp_position(U16 CardNo ,I32 *cmp_pos);
I16 PASCAL _l132_get_cmp_times(U16 CardNo ,I32 *cmp_times);
I16 PASCAL _l132_get_2nd_trigger_times(U16 CardNo ,I32 *cmp_times);
I16 PASCAL _l132_get_version(U16 CardNo ,U16 *Ver ,U16 *Fix);


////////////////
I16 PASCAL _mnet_m1_structure_set_tmove_speed(U16 Block_no,U16 CardNo,U16 RingNo, U16 DeviceIP, F64 StrVel, F64 MaxVel, F64 Tacc,F64 Tdec);
I16 PASCAL _mnet_m1_structure_start_r_move(U16 Block_no, U16 CardNo,U16 RingNo, U16 DeviceIP, I32 Distance);
I16 PASCAL _mnet_m1_structure_start_a_move(U16 Block_no, U16 CardNo,U16 RingNo, U16 DeviceIP, I32 Pos);
I16 PASCAL _mnet_m1_structure_start_tr_line(U16 Block_no,U16 CardNo, U16 RingNo, U16* DeviceIP, I32 DistX, I32 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m1_structure_start_ta_line(U16 Block_no,U16 CardNo, U16 RingNo, U16* DeviceIP, I32 PosX, I32 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m1_structure_start_tr_line2(U16 Block_no,U16 CardNo, U16* RingNo, U16* DeviceIP, I32 DistX, I32 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m1_structure_start_ta_line2(U16 Block_no,U16 CardNo, U16* RingNo, U16* DeviceIP, I32 PosX, I32 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m1_structure_start_sync_tr_line(U16 Block_no,U16 CardNo, U16* RingNo, U16* DeviceIP, I32 DistX, I32 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m1_structure_start_sync_ta_line(U16 Block_no,U16 CardNo, U16* RingNo, U16* DeviceIP, I32 PosX, I32 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _l132_structure_position_cmp(U16 Block_no,U16 CardNo ,U32 start,U32 end,U32 interval);
I16 PASCAL _l132_structure_position_cmp_auto_supply(U16 Block_no,U16 CardNo ,U32 start,U32 end,U32 interval,U32 count);
I16 PASCAL _l132_structure_set_trigger_interval(U16 Block_no,U16 CardNo ,U16 enable);
I16 PASCAL _l132_structure_enbale(U16 CardNo ,U16 block ,U16 enable);
I16 PASCAL _l132_structure_block_num(U16 CardNo ,U16 act_num);
I16 PASCAL _l132_structure_block_repeat(U16 CardNo ,U16 repeat_time);
I16 PASCAL _l132_structure_get_block(U16 CardNo ,U16* actnum);
I16 PASCAL _mnet_m1_structure_motion_done(U16 Block_no, U16 CardNo,U16 RingNo, U16 DeviceIP);
I16 PASCAL _mnet_m1_structure_motion_end(U16 Block_no, U16 CardNo);
I16 PASCAL _l132_structure_workcount(U16 CardNo ,U16 *Worktime);
I16 PASCAL _l132_structure_set_workcount(U16 CardNo ,U16 act_workcount);
I16 PASCAL _l132_reset_structure_program(U16 CardNo);

I16 PASCAL _l132_set_z_tracking_mode(U16 CardNo,U16 enable);
I16 PASCAL _l132_set_z_limit_maxspeed(U16 CardNo,I32 Max_Speed);
I16 PASCAL _l132_set_z_tracking_acc_dec(U16 CardNo,F64 acc,F64 dec);
I16 PASCAL _l132_Get_Master_Version(U16 CardNo ,U16 *Ver);

////////////////
//DB
////////////////
I16 PASCAL _l132_DB_initial(U16 CardNo,U16 Gpio_enable,U16 PA_enable,U16 PB_enable,U16 PC_enable,U16 PD_enable,U16 PE_enable,U16 PF_enable);
I16 PASCAL _l132_DB_Get_Version(U16 CardNo,U16 *version);
I16 PASCAL _l132_DB_Port_Write_Byte(U16 CardNo,U16 port,U16 write_data);
I16 PASCAL _l132_DB_Port_Read_Byte(U16 CardNo,U16 port,U16 *read_data);
I16 PASCAL _l132_DB_Port_Write_Word(U16 CardNo,U16 port,U16 write_data);
I16 PASCAL _l132_DB_Port_Read_Word(U16 CardNo,U16 port,U16 *read_data);
I16 PASCAL _l132_DB_get_Axis(U16 CardNo,U16 axisno,I32 *pos);
I16 PASCAL _l132_DB_clear_Axis(U16 CardNo,U16 axisno);
I16 PASCAL _l132_DB_set_pls_iptmode_logic(U16 CardNo,U16 axisno,U16 reverse);
I16 PASCAL _l132_DB_get_setting_compare_pos(U16 CardNo,U16 axisno,I32 *pos);
I16 PASCAL _l132_DB_position_cmp_auto_supply(U16 CardNo ,U16 DB_Axis_Sel,U32 start,U32 end,U32 interval,U32 count);
I16 PASCAL _l132_DB_position_cmp(U16 CardNo ,U16 DB_Axis_Sel,U32 start,U32 end,U32 interval);
I16 PASCAL _l132_DB_set_trigger_interval(U16 CardNo ,U16 enable);
I16 PASCAL _l132_DB_get_trigger_num(U16 CardNo ,I32* counter);
I16 PASCAL _l132_DB_position_cmp_table(U16 CardNo ,U16 DB_Axis_Sel,I32* TriggerTable,U32 Count);
I16 PASCAL _l132_DB_position_cmp_table_torlerance(U16 CardNo ,I32* TriggerTable,U32 Count,U16 axisno,U16 torlerance_range,U16 enable,U16 T_Times);

///////////////
//Lighting table
//////////////
I16 PASCAL _l132_lighting_table(U16 CardNo ,U16* BankIndex,U16* light_delay,U16* TriggerCCD,U32 Count);
I16 PASCAL _l132_lighting_interval(U16 CardNo ,U16 interval);
I16 PASCAL _l132_lighting_table_enable(U16 CardNo ,U16 enable);
I16 PASCAL _l132_lighting_camera_set_Num(U16 CardNo ,U16 Number);
I16 PASCAL _l132_lighting_camera_get_Num(U16 CardNo ,U16 *Number);


I16 PASCAL _l132_PRD_trigger(U16 CardNo ,U32 microsec,U32 count);
I16 PASCAL _l132_Enable_PRD_trigger(U16 CardNo ,U16 enable);
I16 PASCAL _l132_Set_oneshut(U16 CardNo);
I16 PASCAL _l132_Set_oneshut_Pos(U16 CardNo,U32 start);
I16 PASCAL _l132_get_oneshot_num(U16 CardNo,I32* count);

I16 PASCAL _l132_DB_set_active_cam( U16 CardNo,U16 BackIndex, U16 iCurrent,U8 Action);
I16 PASCAL _l132_lighting_set_Timeout_Reg(U16 CardNo ,U16 value);

I16 PASCAL _l132_LC_Rd(U16 CardNo ,unsigned short addr);
I16 PASCAL _l132_LC_Wr(U16 CardNo ,unsigned short addr, short data);
I16 PASCAL _l132_LC_Code(U16 CardNo ,unsigned short addr);

//add 20060814
I16 PASCAL _l132_write_master_gpio( U16 CardNo,U16 value);
I16 PASCAL _l132_read_master_gpio( U16 CardNo,U16 *value);

//add 20060916
I16 PASCAL _l132_DB_enable_mem_pos(U16 CardNo ,U16 Enable);
I16 PASCAL _l132_DB_get_mem_count(U16 CardNo,I32 *num);
I16 PASCAL _l132_DB_get_mem_pos(U16 CardNo,I32 num, I32 *pos1,I32 *pos2);
I16 PASCAL _l132_cal_slave( U16 CardNo,U16 RingNo, U16 DeviceIP);
I16 PASCAL _l132_DB_enable_mem_input( U16 CardNo,U16 enable);

//engineer
I16 PASCAL _l132_DB_enable_mem_pos_Timer( U16 CardNo,U16 enable,F64 sec);

#ifdef __cplusplus
}
#endif
