#ifndef __MNET_H__
#define __MNET_H__


#include "Type_def.h"

#ifdef __cplusplus
	extern "C" {
#endif

//
// MotionNet Slave Type Definition
//
#define AXIS_GROUP   0xA0 

#define G9103_M101    0xA2   
#define G9003_M1X1    0xA3
#define G9004_M104    0xA4
#define G9004_NPM_FUA4    0xA5  // MNET-BCD5030A4 MNET-BCD4020FUA4
#define G9004_NPM_M204    0xA6  // MNET-M204
#define G9004_M204    0xA7 // SYNTEK-M204
		
#define IO_32_GROUP   (G9002_Q32 >> 4)
#define G9002_Q32     0xB0
#define G9002_I8Q24   0xB1
#define G9002_I16Q16  0xB2
#define G9002_I24Q8   0xB3
#define G9002_I32     0xB4
		
#define IO_16_GROUP  (G9102_I16 >>4)
#define G9102_I16     0xC0
#define G9102_I14Q2   0xC1
#define G9102_I12Q4   0xC2
#define G9102_I10Q6   0xC3
#define G9102_I8Q8    0xC4
#define G9102_I6Q10   0xC5
#define G9102_I4Q12   0xC6
#define G9102_Q16     0xC7

#define G9205_I16     0xC8
#define G9205_I14Q2   0xC9
#define G9205_I12Q4   0xCA
#define G9205_I10Q6   0xCB
#define G9205_I8Q8    0xCC
#define G9205_I6Q10   0xCD
#define G9205_I4Q12   0xCE
#define G9205_Q16     0xCF


		
#define AD_DA_GROUP  (G9004_A104 >>4)
#define G9004_A104    0xD0  // analog output module 
#define G9004_A180    0xD1  // analog input module
//
// PPCIE-8601
I16 PASCAL _pcie8601_open(I16* existcard);
// PCI-L122
I16 PASCAL _pci_l122_open(U16 *existcard);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// USB_L111
I16 PASCAL _USB_L111_initial( U16* exist_usb_l111);
I16 PASCAL _USB_L111_mnet_initial( U16 USB_IP );
I16 PASCAL _USB_L111_mnet_GetRing( U16 USB_IP );
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
I16 PASCAL _mnet_close( void);
//
// Ring Operation
I16 PASCAL _mnet_reset_ring( U16 RingNo );
I16 PASCAL _mnet_start_ring( U16 RingNo );
I16 PASCAL _mnet_stop_ring( U16 RingNo );
I16 PASCAL _mnet_get_ring_status( U16 RingNo, U16 *Status);

// Soft Watch Dog
 //typedef void  (WINAPI *CallbackSoftWatchDog)(U16,U16,U16);
 typedef void (PASCAL *WatchDogUserCbk)(U16, U16, U16);
 #define WATCHDOG_CALLBACK_COUNT 7
 // ===== Soft Watch Dog Event ==========
 typedef enum
 {
	 SWD_Cbk_Unknown = -1,	// Unknown
	 SWD_Cbk_IOPC    =  2,	// IOPC ==> the monitored input port changed
	 SWD_Cbk_EIOE    =  3,	// EIOE ==> I/O communication error
	 SWD_Cbk_EDTE    =  4, 	// EDTE ==> date communication error
	 SWD_Cbk_ERAE    =  5, 	// ERAE ==> local device reception process error
	 SWD_Cbk_CAER    =  6, 	// CAER ==> CPU access error
 }
EN_SWD_EVENT;  

I16 PASCAL _mnet_enable_soft_watchdog(U16 RingNo, HANDLE *User_hEvent);
I16 PASCAL _mnet_disable_soft_watchdog(U16 RingNo);
// I16 PASCAL _mnet_link_soft_watchdog_callback(U16 RingNo, CallbackSoftWatchDog CbkFunc);
I16 PASCAL _mnet_set_ring_quality_param(U16 RingNo, U16 ContinueErr, U16 ErrorRate);
I16 PASCAL _mnet_watchdog_link(U16 RingNo, U16 CbkEvent, WatchDogUserCbk CbkFunc);
I16 PASCAL _mnet_get_com_status(U16 RingNo);
I16 PASCAL _mnet_clear_ring_error(U16 RingNo);

//
// Baud Rate Setting
I16 PASCAL _mnet_set_ring_config( U16 RingNO, U16 BaudRate);
//

// Slaves
I16 PASCAL _mnet_get_ring_active_table( U16 RingNo, U32 *DevTable);
I16 PASCAL _mnet_get_slave_type( U16 RingNo, U16 SlaveIP, U8 *SlaveType);
I16 PASCAL _mnet_get_slave_type_mem(U16 RingNo, U16 SlaveIP, U8 *SlaveType);
I16 PASCAL _mnet_get_error_device(U16 RingNo);
I16 PASCAL _mnet_get_slave_error_table (U16 RingNo, U32 *ErrorTable);
I16 PASCAL _mnet_clear_slave_error_flag (U16 RingNo, U32 *ErrorTable);
I16 PASCAL _mnet_save_ring_active_table(U16 RingNo,char *FilePath);
I16 PASCAL _mnet_load_ring_active_table(U16 RingNo,U32 *DevTable,char *FilePath);

//
// Others
I16 PASCAL _mnet_get_DLL_path( char *lpFilePath, U32 nSize, U32 *nLength );
I16 PASCAL _mnet_get_DLL_version( char *lpBuf, U32 nSize, U32 *nLength );

// ==== G9001A New Functions ============================================
I16 PASCAL _mnet_get_com_error_counter(U16 RingNo, U16 *GetValue);
I16 PASCAL _mnet_get_cyclic_cycle_counter(U16 RingNo, U16 *GetValue);
I16 PASCAL _mnet_read_RENV0_register(U16 RingNo, U16 *GetValue);
I16 PASCAL _mnet_write_RENV0_register(U16 RingNo, U16 SetValue);
I16 PASCAL _mnet_read_receive_address_register(U16 RingNo, U16 *GetValue);
I16 PASCAL _mnet_reset_com_error_counter(U16 RingNo);
//===== Broadcast Commands support by G9001A==============================
I16 PASCAL _mnet_multi_sd_stop(U16 RingNo,U8 Group);
I16 PASCAL _mnet_multi_emg_stop(U16 RingNo,U8 Group);
I16 PASCAL _mnet_multi_imd_stop(U16 RingNo,U8 Group);
//
// Interrupts
//
//typedef void (PASCAL *UserCbk)(U16, U16, U16);
#define CALLBACK_COUNT 7
//
// Interrupt Types
typedef enum
{
	EINT_Unknown =   -1, // Unknown
	EINT_CEND    = 0x01, // Reserved   ( transmitting FIFO buffer ready )
	EINT_BRKF    = 0x02, // Reserved   ( communication break )
 	EINT_IOPC    = 0x04, // the monitored input port changed
	EINT_EIOE    = 0x08, // I/O communication error
	EINT_EDTE    = 0x10, // date communication error
	EINT_ERAE    = 0x20, // local device reception process error
	EINT_CAER    = 0x40, // CPU access error
	EINT_ALL     = 0x7F, // all
}
EInterrupt;
//
// Index of Callback Function Array
typedef enum
{
	ECbk_Unknown = -1,	// Unknown
	ECbk_CEND    =  0,	// Reserved (CEND ==> transmitting FIFO buffer ready)
	ECbk_BRKF    =  1,	// Reserved (BRKF ==> communication break)
	ECbk_IOPC    =  2,	// IOPC ==> the monitored input port changed
	ECbk_EIOE    =  3,	// EIOE ==> I/O communication error
	ECbk_EDTE    =  4, 	// EDTE ==> date communication error
	ECbk_ERAE    =  5, 	// ERAE ==> local device reception process error
	ECbk_CAER    =  6, 	// CAER ==> CPU access error
}
EN_INT_EVENT;


//
// Error Code passed to user callback when error INT occurs
typedef enum
{
	EErr_Unknown =   -1, // Unknown
	EErr_EIOE    = 0x30, // EIOE ==> I/O communication error
	EErr_EDTE    = 0x40, // EDTE ==> date communication error
	//====== local device reception process error ===========
	EErr_ERAE_1  = 0x51, // ERAE case 1
	EErr_ERAE_2  = 0x52, // ERAE case 2
	EErr_ERAE_3  = 0x53, // ERAE case 3
	//====== CPU access error ===========
	EErr_CAER_1  = 0x61, // CAER case 1
	EErr_CAER_2  = 0x62, // CAER case 2
	EErr_CAER_3  = 0x63, // CAER case 3
	EErr_CAER_4  = 0x64, // CAER case 4
}
EErrorCode;
//
// User callback function prototype template
// void PASCAL UserCbk_CEND(U16 RingNo, U16 Useless1, U16 Useless2);
// void PASCAL UserCbk_BRKF(U16 RingNo, U16 Useless1, U16 Useless2);
void PASCAL UserCbk_IOPC(U16 RingNo, U16 SlaveIP, U16 Port);
void PASCAL UserCbk_EIOE(U16 RingNo, U16 SlaveIP, U16 Code);
void PASCAL UserCbk_EDTE(U16 RingNo, U16 SlaveIP, U16 Code);
void PASCAL UserCbk_ERAE(U16 RingNo, U16 SlaveIP, U16 Code);
void PASCAL UserCbk_CAER(U16 RingNo, U16 SlaveIP, U16 Code);
//
//I16 PASCAL _mnet_int_enable( U16 RingNo, U16 Enable);
//I16 PASCAL _mnet_int_link( U16 RingNo, U16 IntEvent, UserCbk CbkFunc );
//I16 PASCAL _mnet_int_clear( U16 RingNo, U16 IntBits);
//I16 PASCAL _mnet_int_mask( U16 RingNo, U16 IntBits);
// I16 PASCAL _mnet_int_auto_break( U16 RingNo, U16 Enable);
// I16 PASCAL _mnet_int_auto_clear( U16 RingNo, U16 Enable);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////                      //////////////////////////////
////////////////////////////  Digital I/O Slaves  //////////////////////////////
////////////////////////////                      //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
I16 PASCAL _mnet_io_output( U16 RingNo, U16 SlaveIP, U8 PortNo, U8 Val );
I16 PASCAL _mnet_io_input( U16 RingNo, U16 SlaveIP, U8 PortNo );
I16 PASCAL _mnet_bit_io_output(U16 RingNo, U16 SlaveIP, U8 PortNo, U8 BitNo, U8 On_Off);
I16 PASCAL _mnet_bit_io_input(U16 RingNo, U16 SlaveIP, U8 PortNo, U8 BitNo, U8 *On_Off);
I16 PASCAL _mnet_io_input_monitor( U16 RingNo, U16 SlaveIP, U8 PortNo, U16 Enable);
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////                      //////////////////////////////
//////////////////////////// 1-Axis Motion Slaves //////////////////////////////
////////////////////////////                      //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Initial
I16 PASCAL _mnet_m1_initial( U16 RingNo, U16 SlaveIP );
//
// Pulse I/O Configuration
I16 PASCAL _mnet_m1_set_pls_outmode( U16 RingNo, U16 SlaveIP, U16 pls_outmode);
I16 PASCAL _mnet_m1_set_pls_iptmode( U16 RingNo, U16 SlaveIP, U16 pls_iptmode, U16 pls_iptdir);
I16 PASCAL _mnet_m1_set_feedback_src( U16 RingNo, U16 SlaveIP, U16 FbkSrc );
I16 PASCAL _mnet_m1_set_move_ratio( U16 RingNo, U16 SlaveIP, F64 move_ratio );
//
// Interface I/O Configuration
I16 PASCAL _mnet_m1_set_alm( U16 RingNo, U16 SlaveIP, U16 alm_logic, U16 alm_mode);
I16 PASCAL _mnet_m1_set_inp( U16 RingNo, U16 SlaveIP, U16 inp_enable, U16 inp_logic );
I16 PASCAL _mnet_m1_set_erc( U16 RingNo, U16 SlaveIP, U16 erc_logic, U16 erc_on_time, U16 erc_off_time);
I16 PASCAL _mnet_m1_set_erc_on( U16 RingNo, U16 SlaveIP, I16 on_off );
I16 PASCAL _mnet_m1_set_svon( U16 RingNo, U16 SlaveIP, U16 on_off );
I16 PASCAL _mnet_m1_set_ralm( U16 RingNo, U16 SlaveIP, U16 on_off );
I16 PASCAL _mnet_m1_set_sd( U16 RingNo, U16 SlaveIP, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode);
I16 PASCAL _mnet_m1_set_ltc_logic( U16 RingNo, U16 SlaveIP, U16 ltc_logic );
I16 PASCAL _mnet_m1_set_pcs( U16 RingNo, U16 SlaveIP, U16 pcs_logic );
I16 PASCAL _mnet_m1_set_clr( U16 RingNo, U16 SlaveIP, U16 clr_logic );
I16 PASCAL _mnet_m1_dio_output( U16 RingNo, U16 SlaveIP, U16 DoNo, U16 on_off );
I16 PASCAL _mnet_m1_dio_input( U16 RingNo, U16 SlaveIP, U16 DiNo );
I16 PASCAL _mnet_m1_set_mechanical_input_filter( U16 RingNo, U16 SlaveIP, U16 on_off );
// I/O Status
I16 PASCAL _mnet_m1_get_io_status( U16 RingNo, U16 SlaveIP, U32 *IO_status);
//
// Speed Pattern
I16 PASCAL _mnet_m1_set_speed_pattern( U16 RingNo, U16 SlaveIP, U8 Pattern );
//
// Velocity Control
I16 PASCAL _mnet_m1_v_move( U16 RingNo, U16 SlaveIP, U8 Dir);
I16 PASCAL _mnet_m1_v_change( U16 RingNo, U16 SlaveIP, F64 NewVel, F64 Time);
I16 PASCAL _mnet_m1_fix_speed_range( U16 RingNo, U16 SlaveIP, F64 MaxVel );
I16 PASCAL _mnet_m1_unfix_speed_range( U16 RingNo, U16 SlaveIP );
//
// Position Control
I16 PASCAL _mnet_m1_set_tmove_speed( U16 RingNo, U16 SlaveIP, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m1_set_smove_speed( U16 RingNo, U16 SlaveIP, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m1_start_r_move( U16 RingNo, U16 SlaveIP, I32 Distance);
I16 PASCAL _mnet_m1_start_a_move( U16 RingNo, U16 SlaveIP, I32 Pos);
//
// Stop
I16 PASCAL _mnet_m1_sd_stop( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1_emg_stop( U16 RingNo, U16 SlaveIP );
//
// Motion Status
I16 PASCAL _mnet_m1_motion_done( U16 RingNo, U16 SlaveIP, U16 *MoSt );
//
// Home
I16 PASCAL _mnet_m1_start_home_move( U16 RingNo, U16 SlaveIP, U8 Dir);
I16 PASCAL _mnet_m1_set_home_config( U16 RingNo, U16 SlaveIP, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out );
I16 PASCAL _mnet_m1_start_home_search( U16 RingNo, U16 SlaveIP, U8 Dir, F64 ORGOffset );
I16 PASCAL _mnet_m1_start_home_escape( U16 RingNo, U16 SlaveIP, U8 Dir);
//
// Counter Operating
I16 PASCAL _mnet_m1_set_command( U16 RingNo, U16 SlaveIP, I32 Cmd);
I16 PASCAL _mnet_m1_get_command( U16 RingNo, U16 SlaveIP, I32 *Cmd);
I16 PASCAL _mnet_m1_reset_command( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1_get_position( U16 RingNo, U16 SlaveIP, I32 *Pos);
I16 PASCAL _mnet_m1_set_position( U16 RingNo, U16 SlaveIP, I32 Pos);
I16 PASCAL _mnet_m1_reset_position( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1_set_error_counter( U16 RingNo, U16 SlaveIP, I32 ErrCnt );
I16 PASCAL _mnet_m1_get_error_counter( U16 RingNo, U16 SlaveIP, I32 *ErrCnt );
I16 PASCAL _mnet_m1_reset_error_counter( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1_get_current_speed( U16 RingNo, U16 SlaveIP, F64 *speed);
//
// Position Compare and Latch
I16 PASCAL _mnet_m1_set_comparator_mode( U16 RingNo, U16 SlaveIP, I16 CompNo, I16 CmpSrc, I16 CmpMethod, I16 CmpAction );
I16 PASCAL _mnet_m1_set_comparator_data( U16 RingNo, U16 SlaveIP, I16 CompNo, F64 Pos);
I16 PASCAL _mnet_m1_set_trigger_comparator( U16 RingNo, U16 SlaveIP, U16 CmpSrc , U16 CmpMethod);
I16 PASCAL _mnet_m1_set_trigger_comparator_data( U16 RingNo, U16 SlaveIP,  F64 Data );
I16 PASCAL _mnet_m1_get_comparator_data( U16 RingNo, U16 SlaveIP, I16 CompNo, F64 *Pos);
I16 PASCAL _mnet_m1_set_soft_limit( U16 RingNo, U16 SlaveIP, I32 PLimit, I32 MLimit );
I16 PASCAL _mnet_m1_enable_soft_limit( U16 RingNo, U16 SlaveIP, U8 Action );
I16 PASCAL _mnet_m1_disable_soft_limit( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1_get_latch_data( U16 RingNo, U16 SlaveIP, I16 LatchNo, F64 *Pos);
I16 PASCAL _mnet_m1_set_auto_trigger( U16 RingNo, U16 SlaveIP, U16 CmpSrc, U16 CmpMethod, U16 Interval, U16 on_off );
//
// Load Configuration File
I16 PASCAL _mnet_m1_load_motion_file( U16 RingNo, U16 SlaveIP, char *FilePath );
//
// Destination Change
I16 PASCAL _mnet_m1_start_p_change( U16 RingNo, U16 SlaveIP, I32 NewPos);
I16 PASCAL _mnet_m1_start_d_change( U16 RingNo, U16 SlaveIP, I32 NewDist );
//
// Backlash Correction
I16 PASCAL _mnet_m1_set_backlash( U16 RingNo, U16 SlaveIP, U16 Value, U16 Enable, U16 CntSrc );
//
// Synchronize
I16 PASCAL _mnet_m1_set_sta_trigger( U16 RingNo, U16 SlaveIP,  U16 TriType);
I16 PASCAL _mnet_m1_sync_v_move( U16 RingNo, U16 SlaveIP, U8 Dir);
I16 PASCAL _mnet_m1_start_sync_r_move( U16 RingNo, U16 SlaveIP, I32 Distance);
I16 PASCAL _mnet_m1_start_sync_a_move( U16 RingNo, U16 SlaveIP, I32 Pos);
I16 PASCAL _mnet_m1_start_sync_tr_line( U16 RingNo, U16* SlaveIP, I32 DistX, I32 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m1_start_sync_ta_line( U16 RingNo, U16* SlaveIP, I32 PosX, I32 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m1_start_sync_sr_line( U16 RingNo, U16* SlaveIP, I32 DistX, I32 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m1_start_sync_sa_line( U16 RingNo, U16* SlaveIP, I32 PosX, I32 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m1_sync_sd_stop( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m1_sync_imd_stop( U16 RingNo, U16 SlaveIP );
//
// FUA3
I16 PASCAL _mnet_m1_fua3_sync( U16 RingNo, U16 SlaveIP, U16 on_off );
I16 PASCAL _mnet_m1_fua3_powerdown( U16 RingNo, U16 SlaveIP, U16 on_off );
//
// Others
I16 PASCAL _mnet_m1_get_speed_range( U16 RingNo, U16 SlaveIP, F64 *MinVel, F64 *MaxVel );
////////////////////////////////////////////////////////////////////////////////
/////////////////////////                            ///////////////////////////
///////////////////////// 4-Axis Motion Slaves(M1X4) ///////////////////////////
/////////////////////////                            ///////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Initial
I16 PASCAL _mnet_m4_initial( U16 RingNo, U16 SlaveIP );
//
// Pulse I/O Configuration
I16 PASCAL _mnet_m4_set_pls_outmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_outmode);
I16 PASCAL _mnet_m4_set_pls_iptmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_iptmode, U16 pls_iptdir);
I16 PASCAL _mnet_m4_set_feedback_src( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 FbkSrc );
//
// Interface I/O Configuration
I16 PASCAL _mnet_m4_set_alm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 alm_logic, U16 alm_mode);
I16 PASCAL _mnet_m4_set_inp( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 inp_enable, U16 inp_logic );
I16 PASCAL _mnet_m4_set_erc( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 erc_logic, U16 erc_on_time, U16 erc_off_time);
I16 PASCAL _mnet_m4_set_erc_on( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 on_off );
I16 PASCAL _mnet_m4_set_svon( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off );
I16 PASCAL _mnet_m4_set_ralm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off );
I16 PASCAL _mnet_m4_set_sd( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode);
I16 PASCAL _mnet_m4_set_ell( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 ell_logic );
I16 PASCAL _mnet_m4_set_mechanical_input_filter( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off, U16 FilterLength );
// I/O Status
I16 PASCAL _mnet_m4_get_io_status( U16 RingNo, U16 SlaveIP, U16 AxisNo, U32 *IO_status);
//
// Velocity Control
I16 PASCAL _mnet_m4_v_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir);
I16 PASCAL _mnet_m4_fix_speed_range( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel );
I16 PASCAL _mnet_m4_unfix_speed_range( U16 RingNo, U16 SlaveIP, U16 AxisNo );
//I16 PASCAL _mnet_m4_v_change( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 NewVel, F64 Time);
//
// Position Control
I16 PASCAL _mnet_m4_set_tmove_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_set_smove_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m4_start_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance);
I16 PASCAL _mnet_m4_start_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos);
I16 PASCAL _mnet_m4_start_p_change(U16 RingNo, U16 SlaveIP,I16 AxisNo, F64 NewPos);
//
I16 PASCAL _mnet_m4_tr_move_xy( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_ta_move_xy( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_sr_move_xy( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m4_sa_move_xy( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m4_tr_move_zu( U16 RingNo, U16 SlaveIP, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_ta_move_zu( U16 RingNo, U16 SlaveIP, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_sr_move_zu( U16 RingNo, U16 SlaveIP, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m4_sa_move_zu( U16 RingNo, U16 SlaveIP, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m4_start_tr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_start_ta_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_start_sr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m4_start_sa_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m4_start_tr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_start_ta_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_start_sr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m4_start_sa_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec , F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m4_start_tr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_start_ta_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m4_start_sr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m4_start_sa_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
// Stop
I16 PASCAL _mnet_m4_sd_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo );
I16 PASCAL _mnet_m4_emg_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo );
//
// Motion Status
I16 PASCAL _mnet_m4_motion_done( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *MoSt );
//
// Home
I16 PASCAL _mnet_m4_set_home_config( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out );
I16 PASCAL _mnet_m4_start_home_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, U8 Dir);
//
// Counter Operating
I16 PASCAL _mnet_m4_get_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Cmd);
I16 PASCAL _mnet_m4_set_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Cmd);
I16 PASCAL _mnet_m4_reset_command( U16 RingNo, U16 SlaveIP, U16 AxisNo );
I16 PASCAL _mnet_m4_get_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *Pos);
I16 PASCAL _mnet_m4_set_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos);
I16 PASCAL _mnet_m4_reset_position( U16 RingNo, U16 SlaveIP, U16 AxisNo );
I16 PASCAL _mnet_m4_get_current_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 *speed);
//
// Position Compare and Latch
I16 PASCAL _mnet_m4_set_ltc_logic( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 ltc_logic );
I16 PASCAL _mnet_m4_get_latch_data( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 LatchNo, F64 *Pos);
I16 PASCAL _mnet_m4_set_comparator_data( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CompNo, F64 Pos);
I16 PASCAL _mnet_m4_set_comparator_mode( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CompNo, I16 CmpMethod);
I16 PASCAL _mnet_m4_set_trigger_comparator_mode( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpMethod);
I16 PASCAL _mnet_m4_set_trigger_comparator_data( U16 RingNo, U16 SlaveIP, U16 AxisNo,  F64 Data );
I16 PASCAL _mnet_m4_set_trigger_output_time( U16 RingNo, U16 SlaveIP, U16 Time);
I16 PASCAL _mnet_m4_set_auto_trigger( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 CmpMethod, U16 Interval, U16 Logic, U16 on_off );
//
// Load Configuration File
I16 PASCAL _mnet_m4_load_motion_file( U16 RingNo, U16 SlaveIP, char *FilePath );
//
// MOF
I16 PASCAL _mnet_m4_set_pulser_iptmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 InputMode, I16 Inverse);
I16 PASCAL _mnet_m4_set_mof_config( U16 RingNo, U16 SlaveIP, I16 MOF_mode);
I16 PASCAL _mnet_m4_disable_mof( U16 RingNo, U16 SlaveIP );
I16 PASCAL _mnet_m4_get_mof_status( U16 RingNo, U16 SlaveIP, U16 *MOF_sts);
I16 PASCAL _mnet_m4_manual_mpg( U16 RingNo, U16 SlaveIP, I16 MPG_axis_sel );
I16 PASCAL _mnet_m4_set_mof_jog_velocity( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, I16 feedrate);
I16 PASCAL _mnet_m4_set_mof_step_distance( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 step_dist );
I16 PASCAL _mnet_m4_set_mof_step_velocity( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, I16 feedrate);
I16 PASCAL _mnet_m4_set_mof_mpg_velocity( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
//
// Synchronous Move - STA
I16 PASCAL _mnet_m4_sync_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance);
I16 PASCAL _mnet_m4_sync_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos); 
//
// Synchronous Move - internal
I16 PASCAL _mnet_m4_set_triggered_r_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Distance, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m4_set_triggered_a_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 Pos, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m4_set_triggered_tr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m4_set_triggered_ta_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m4_set_triggered_sr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m4_set_triggered_sa_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing);
//
// Others
I16 PASCAL _mnet_m4_set_continuous_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Enable);
I16 PASCAL _mnet_m4_set_velocity_limit( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel );

I16 PASCAL _mnet_m4_open_move_check(I16 Enable);
////////////////////////////////////////////////////////////////////////////////
/////////////////////////                            ///////////////////////////
///////////////////////// 4-Axis Motion Slaves(M204) ///////////////////////////
/////////////////////////                            ///////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Initial
I16 PASCAL _mnet_m204_initial( U16 RingNo, U16 SlaveIP );
//
// Pulse I/O Configuration
I16 PASCAL _mnet_m204_set_pls_outmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_outmode);
I16 PASCAL _mnet_m204_set_pls_iptmode( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 pls_iptmode, U16 pls_iptdir);
I16 PASCAL _mnet_m204_set_feedback_src( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 FbkSrc );
//
// Interface I/O Configuration
I16 PASCAL _mnet_m204_set_alm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 alm_logic, U16 alm_mode);
I16 PASCAL _mnet_m204_set_inp( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 inp_enable, U16 inp_logic );
I16 PASCAL _mnet_m204_set_erc( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 erc_logic, U16 erc_on_time, U16 erc_off_time);
I16 PASCAL _mnet_m204_set_erc_on( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 on_off );
I16 PASCAL _mnet_m204_set_sd( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 sd_enable, I16 sd_logic, I16 sd_latch, I16 sd_mode);
I16 PASCAL _mnet_m204_set_svon( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off );
I16 PASCAL _mnet_m204_set_ralm( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 on_off );
I16 PASCAL _mnet_m204_set_ltc_logic( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 ltc_logic );
I16 PASCAL _mnet_m204_set_el(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 el_mode, I16 el_logic);

// I/O Status
I16 PASCAL _mnet_m204_get_io_status( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *io_sts);
//
// Velocity Control
I16 PASCAL _mnet_m204_tv_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc );
I16 PASCAL _mnet_m204_sv_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 SVacc );
I16 PASCAL _mnet_m204_v_change( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 NewVel, F64 Time);
I16 PASCAL _mnet_m204_fix_speed_range( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel );
I16 PASCAL _mnet_m204_unfix_speed_range( U16 RingNo, U16 SlaveIP, U16 AxisNo );
//
// Position Control
I16 PASCAL _mnet_m204_start_tr_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m204_start_tr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_line2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m204_start_tr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_line3( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m204_start_tr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_line4( U16 RingNo, U16 SlaveIP, F64 DistX, F64 DistY, F64 DistZ, F64 DistU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_line4( U16 RingNo, U16 SlaveIP, F64 PosX, F64 PosY, F64 PosZ, F64 PosU, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m204_start_tr_arc2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 OffsetCx, F64 OffsetCy, F64 OffsetEx, F64 OffsetEy, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_arc2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 Cx, F64 Cy, F64 Ex, F64 Ey, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_arc2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 OffsetCx, F64 OffsetCy, F64 OffsetEx, F64 OffsetEy, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_arc2( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 Cx, F64 Cy, F64 Ex, F64 Ey, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m204_start_tr_arc_xyz( U16 RingNo, U16 SlaveIP, F64 OffsetCx, F64 OffsetCy, F64 OffsetEx, F64 OffsetEy, F64 DistZ, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_arc_xyz( U16 RingNo, U16 SlaveIP, F64 Cx, F64 Cy, F64 Ex, F64 Ey, F64 PosZ, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_arc_xyz( U16 RingNo, U16 SlaveIP, F64 OffsetCx, F64 OffsetCy, F64 OffsetEx, F64 OffsetEy, F64 DistZ, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_arc_xyz( U16 RingNo, U16 SlaveIP, F64 Cx, F64 Cy, F64 Ex, F64 Ey, F64 PosZ, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
// Conti Move
I16 PASCAL _mnet_m204_start_tr_line2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_line2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_line2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_line2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
I16 PASCAL _mnet_m204_start_tr_arc2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 OffsetCx, F64 OffsetCy, F64 OffsetEx, F64 OffsetEy, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_ta_arc2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 Cx, F64 Cy, F64 Ex, F64 Ey, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec );
I16 PASCAL _mnet_m204_start_sr_arc2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 OffsetCx, F64 OffsetCy, F64 OffsetEx, F64 OffsetEy, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
I16 PASCAL _mnet_m204_start_sa_arc2_conti( U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 Cx, F64 Cy, F64 Ex, F64 Ey, I16 DIR, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec );
//
// Stop
I16 PASCAL _mnet_m204_sd_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Tdec );
I16 PASCAL _mnet_m204_emg_stop( U16 RingNo, U16 SlaveIP, U16 AxisNo );
//
// Motion Status
I16 PASCAL _mnet_m204_motion_done( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *McSts);
//
// Home
I16 PASCAL _mnet_m204_set_home_config( U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 home_mode, U16 org_logic, U16 ez_logic, U16 ez_count, U16 erc_out );
I16 PASCAL _mnet_m204_start_home_move( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc );
I16 PASCAL _mnet_m204_escape_home( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc );
I16 PASCAL _mnet_m204_home_search( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 StrVel, F64 MaxVel, F64 Tacc, F64 ORG_Offset );
//
// Counter Operating
I16 PASCAL _mnet_m204_get_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *cmd);
I16 PASCAL _mnet_m204_get_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *pos);
I16 PASCAL _mnet_m204_get_current_speed( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 *speed);
I16 PASCAL _mnet_m204_get_error_counter( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 *error);
I16 PASCAL _mnet_m204_set_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 cmd);
I16 PASCAL _mnet_m204_set_position( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 pos);
I16 PASCAL _mnet_m204_reset_error_counter( U16 RingNo, U16 SlaveIP, U16 AxisNo );
I16 PASCAL _mnet_m204_get_rest_command( U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 *rest_command);
I16 PASCAL _mnet_m204_reset_command( U16 RingNo, U16 SlaveIP, U16 AxisNo );
I16 PASCAL _mnet_m204_reset_position( U16 RingNo, U16 SlaveIP, U16 AxisNo );
//
// Others
I16 PASCAL _mnet_m204_get_sts( U16 RingNo, U16 SlaveIP, U16 AxisNo, U32 *sts);

//Compare Function
I16 PASCAL _mnet_m204_set_general_comparator(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpSrc, I16 CmpMethod, I16 CmpAction, F64 Data);
I16 PASCAL _mnet_m204_set_trigger_comparator(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpSrc, I16 CmpMethod, F64 Data);

I16 PASCAL _mnet_m204_set_trigger_type(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 TriggerType);
I16 PASCAL _mnet_m204_set_auto_compare( U16 RingNo, U16 SlaveIP, U16 CompareNo, U16 AxisCounterNo, I16 DIR, I32 Start, U16 Interval, I32 TriggerNumber);
I16 PASCAL _mnet_m204_trigger_output_pulse_width( U16 RingNo, U16 SlaveIP, U16 CompareNo, U16 Time);
I16 PASCAL _mnet_m204_enable_auto_compare( U16 RingNo, U16 SlaveIP, U16 CompareNo, U16 Enable);

I16 PASCAL _mnet_m204_set_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I32 PLimit, I32 NLimit);
I16 PASCAL _mnet_m204_enable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Action);
I16 PASCAL _mnet_m204_disable_soft_limit(U16 RingNo, U16 SlaveIP, U16 AxisNo);

I16 PASCAL _mnet_m204_check_compare_data(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 CmpType, F64 *Pos);
//SYNC
I16 PASCAL _mnet_m204_sync_tr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m204_sync_ta_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m204_sync_sr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec);
I16 PASCAL _mnet_m204_sync_sa_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec);
I16 PASCAL _mnet_m204_sync_tr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m204_sync_ta_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec);
I16 PASCAL _mnet_m204_sync_sr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec);
I16 PASCAL _mnet_m204_sync_sa_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec);

//Trigger Move
I16 PASCAL _mnet_m204_trigger_tr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 Timing);
I16 PASCAL _mnet_m204_trigger_ta_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 Timing);
I16 PASCAL _mnet_m204_tirgger_sr_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 Timing);
I16 PASCAL _mnet_m204_tirgger_sa_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 Pos, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 Timing);

I16 PASCAL _mnet_m204_trigger_tr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m204_trigger_ta_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m204_trigger_sr_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing);
I16 PASCAL _mnet_m204_trigger_sa_line2(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 timing);

I16 PASCAL _mnet_m204_trigger_tr_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 Timing);
I16 PASCAL _mnet_m204_trigger_ta_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, U16 SrcAxisNo, U8 Timing);
I16 PASCAL _mnet_m204_trigger_sr_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 DistX, F64 DistY, F64 DistZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 Timing);
I16 PASCAL _mnet_m204_trigger_sa_line3(U16 RingNo, U16 SlaveIP, I16 *AxisArray, F64 PosX, F64 PosY, F64 PosZ, F64 StrVel, F64 MaxVel, F64 Tacc, F64 Tdec, F64 SVacc, F64 SVdec, U16 SrcAxisNo, U8 Timing);

I16 PASCAL _mnet_m204_set_continuous_move(U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 Conti_logic);
I16 PASCAL _mnet_m204_check_continuous_buffer(U16 RingNo, U16 SlaveIP, U16 AxisNo, U16 *on_off);

I16 PASCAL _mnet_m204_load_motion_file( U16 RingNo, U16 SlaveIP, char *FilePath);

I16 PASCAL _mnet_m204_set_soft_sta_signal( U16 RingNo, U16 SlaveIP, I16 on_off);
I16 PASCAL _mnet_m204_set_soft_stp_signal( U16 RingNo, U16 SlaveIP, I16 on_off);

I16 PASCAL _mnet_m204_set_FIFO_latch_src(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 LatchInputNo, U16 Enable);
I16 PASCAL _mnet_m204_get_latch_data( U16 RingNo, U16 SlaveIP, U16 AxisNo, I16 LatchNo, I32 *Pos);
I16 PASCAL _mnet_m204_get_latch_data_from_buffer( U16 RingNo, U16 SlaveIP, U16 BufferNo, U16 *AxisCounterNo, U16 *LatchDataCnt, I32 *LatchDataTable);
I16 PASCAL _mnet_m204_reset_latch_FIFO(U16 RingNo, U16 SlaveIP);
I16 PASCAL _mnet_m204_get_latch_FIFO_length(U16 RingNo, U16 SlaveIP, U16 *length);

I16 PASCAL _mnet_m204_set_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, U16 CntMode, U16 CntDir,I32 SetValue);
I16 PASCAL _mnet_m204_get_axis_counter(U16 RingNo, U16 SlaveIP, U16 AxisCounterNo, I32 *GetValue);

I16 PASCAL _mnet_m204_set_velocity_limit( U16 RingNo, U16 SlaveIP, U16 AxisNo, F64 MaxVel );
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
	}
#endif



#endif //__MNET_H__

