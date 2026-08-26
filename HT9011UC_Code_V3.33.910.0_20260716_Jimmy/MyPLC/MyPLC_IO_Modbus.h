#ifndef MyPLC_IO_ModbusH
#define MyPLC_IO_ModbusH

#include "ModbusTCPClient.h"
#include "cMyDef.h"

//Schneider(研五版本)
#define READ_BYTE_COUNT_INP_DATA_SCHNEIDER  20
#define READ_BYTE_COUNT_OSSD_SCHNEIDER      2
#define INPUT_MAX_REGISTER_SCHNEIDER        (READ_BYTE_COUNT_INP_DATA_SCHNEIDER + READ_BYTE_COUNT_OSSD_SCHNEIDER)   //掃30001-30022          22

#define PLCIO_INPUT_START_PORT_NUMBER_SCHNEIDER       0x300
#define PLCIO_INPUT_START_PORT_NUMBER_SCHNEIDER_UB    0x303                     //Upper bound

//ReeR(SemiS2版本)
#define READ_BYTE_COUNT_INP_DATA_REER       30
#define READ_BYTE_COUNT_OSSD_REER           eReadByte_OSSD_Count
#define INPUT_MAX_REGISTER_REER             (READ_BYTE_COUNT_INP_DATA_REER + READ_BYTE_COUNT_OSSD_REER)             //掃0x400~0x41D共15個register即30個Byte + OSSD的4個register即8個Byte 共38

#define PLCIO_INPUT_START_PORT_NUMBER_REER            0x400
#define PLCIO_INPUT_START_PORT_NUMBER_REER_UB         0x407                     //Upper bound

//maximum of all size in every case
#define READ_BYTE_COUNT_INP_DATA            ((READ_BYTE_COUNT_INP_DATA_SCHNEIDER) > (READ_BYTE_COUNT_INP_DATA_REER) ? (READ_BYTE_COUNT_INP_DATA_SCHNEIDER) : (READ_BYTE_COUNT_INP_DATA_REER))  //實際讀取Input data的Byte數量
#define READ_BYTE_COUNT_OSSD                ((READ_BYTE_COUNT_OSSD_SCHNEIDER    ) > (READ_BYTE_COUNT_OSSD_REER    ) ? (READ_BYTE_COUNT_OSSD_SCHNEIDER    ) : (READ_BYTE_COUNT_OSSD_REER    ))  //實際讀取OSSD data的Byte數量

#define INPUT_MAX_REGISTER                  (READ_BYTE_COUNT_INP_DATA + READ_BYTE_COUNT_OSSD)  //所有廠牌共用此參數所以得要宣告單項最大長度的加總作為宣告的最大長度

//把非台積機台使用的Schneider模組與台積使用的Safe PLCIO功能使用的ReeR模組之不同參數整合在這邊,直接拿一包即可使用
typedef struct T_PLCIOInfo
{
    WORD m_wInputStartRegNum;
    WORD m_wInputEndRegNum;         //Read until this register number
    int m_iFuncCode;
    int m_iReadRegOffset;           //Register offset:0x400, 而41040(十進制)稱作Register number
    int m_iReadByteCount_InpData;
    int m_iReadByteCount_OSSD;
    int m_iReadByteCount;

    T_PLCIOInfo()
    {
        m_wInputStartRegNum = PLCIO_INPUT_START_PORT_NUMBER_SCHNEIDER;
        m_wInputEndRegNum = PLCIO_INPUT_START_PORT_NUMBER_SCHNEIDER_UB;
        m_iFuncCode = MODBUS_FC_READ_INPUT_REGISTERS;
        m_iReadRegOffset = 0x00;
        m_iReadByteCount_InpData = READ_BYTE_COUNT_INP_DATA_SCHNEIDER;
        m_iReadByteCount_OSSD = READ_BYTE_COUNT_OSSD_SCHNEIDER;
        m_iReadByteCount = m_iReadByteCount_InpData + m_iReadByteCount_OSSD;
    }
}TPLCIOInfo;

enum{
    eReadByte_OSSD_0 = 0,   //Reer OSSD_Byte_0(或另外稱呼為OSSD01~OSSD08):0x40f High byte
    eReadByte_OSSD_1,       //Reer OSSD_Byte_1(或另外稱呼為OSSD09~OSSD16):0x40f Low byte
    eReadByte_OSSD_2,       //Reer OSSD_Byte_2(或另外稱呼為OSSD17~OSSD24):0x410 High byte
    eReadByte_OSSD_3,       //Reer OSSD_Byte_3(或另外稱呼為OSSD25~OSSD32):0x410 Low byte

    eReadByte_OSSD_Count,
};

//ReeR合併卡匣模組MOSAIC M1SC COM
//Modbus TCP/IP Register mapping 
//  Register(s)   |  Size | Name 
// 400h Low byte  | UINT8 | System status 
// 400h High byte | UINT8 | Reserved 
// 401h Low byte  | UINT8 | Input status byte 0 
// 401h High byte | UINT8 | Input status byte 1 
// 402h Low byte  | UINT8 | Input status byte 2 
// 402h High byte | UINT8 | Input status byte 3 
// 403h Low byte  | UINT8 | Input status byte 4 
// 403h High byte | UINT8 | Input status byte 5 
// 404h Low byte  | UINT8 | Input status byte 6 
// 404h High byte | UINT8 | Input status byte 7 
// 405h Low byte  | UINT8 | Input status byte 8 
// 405h High byte | UINT8 | Input status byte 9 
// 406h Low byte  | UINT8 | Input status byte 10 
// 406h High byte | UINT8 | Input status byte 11 
// 407h Low byte  | UINT8 | Input status byte 12 
// 407h High byte | UINT8 | Input status byte 13 
// 408h Low byte  | UINT8 | Input status byte 14 
// 408h High byte | UINT8 | Input status byte 15 
// 409h Low byte  | UINT8 | Restart Input byte 0 
// 409h High byte | UINT8 | Restart Input byte 1 
// 40Ah Low byte  | UINT8 | Restart Input byte 2 
// 40Ah High byte | UINT8 | Reserved 
// 40Bh Low byte  | UINT8 | Fieldbus input feedback byte 0 
// 40Bh High byte | UINT8 | Fieldbus input feedback byte 1 
// 40Ch Low byte  | UINT8 | Fieldbus input feedback byte 2 
// 40Ch High byte | UINT8 | Fieldbus input feedback byte 3 
// 40Dh Low byte  | UINT8 | Probe status byte 0 
// 40Dh High byte | UINT8 | Probe status byte 1 
// 40Eh Low byte  | UINT8 | Probe status byte 2 
// 40Eh High byte | UINT8 | Probe status byte 3 
// 40Fh High byte | UINT8 | OSSD status byte 0 
// 40Fh Low byte  | UINT8 | OSSD status byte 1 
// 410h High byte | UINT8 | OSSD status byte 2 
// 410h Low byte  | UINT8 | OSSD status byte 3 

//------------------------------------------------------------------------------
//one group is means one slave
typedef enum E_PLCGrp
{
    ePLCGrp_Safe = 0,                   //this group is the slave related to the safety PLC
	
    ePLCGrp_Count,
}EPLCGrp;
//------------------------------------------------------------------------------
extern int g_iTransactionID[ePLCGrp_Count];
extern void InitPLCIO(AnsiString asIP,int iPort, EPLCGrp ePLCGrp);
extern void PLCIOTaskCycle(EPLCGrp ePLCGrp);
extern void CloseFile_PLCRecvDataLog();
extern int g_iPLCIOTask[ePLCGrp_Count];
extern bool bScanSlave[ePLCGrp_Count];
extern bool bPLCInData[ePLCGrp_Count][INPUT_MAX_REGISTER][8];                   //所有廠牌共用此參數所以得要宣告最大長度

extern bool bPLCIO[2048][8];                                                    //for 舊架構
extern bool bPLCIOEffect;                                                       //已經接收過資料
extern bool bIOPowered;                                                         //是否有供電? 急停旋鈕解除後按下Reset才算完成解除急停狀態
//------------------------------------------------------------------------------
extern int g_iPLCStatusTask[ePLCGrp_Count];                                     //ben 20230913 add
extern void PLCStatusCheck(EPLCGrp ePLCGrp);
extern ModbusTCPClient g_PlcComm[ePLCGrp_Count];                                //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題
//------------------------------------------------------------------------------
extern bool bSafePLCThread;
extern void bPLCStatusCheck();                                                  //kevin 20250407 check PLC
//------------------------------------------------------------------------------
class TPLCIOThread : public TThread
{
    private:
    protected:
        void __fastcall Execute();
    public:
        __fastcall TPLCIOThread(bool CreateSuspended);
        void __fastcall TPLCIOThread::PLCIOProcess(void);
};
//------------------------------------------------------------------------------
class TMyPLC_IO_Modbus
{
    private:
        bool bOn;
        int iSlave;
        int iPort;
        int iBit;

    protected:
        void DO_Process(byte value);
        Byte DI_Process();

    public:
        static std::vector<TMyPLC_IO_Modbus*> PLC_IO_Members;
        int GetSlave(){return iSlave;};
        int GetPort(){return iPort;};
        int GetBit(){return iBit;};

        __fastcall TMyPLC_IO_Modbus();

        virtual void SetPortInformation(int Card, int Port, int Bit);
        virtual bool IsOn();
        virtual bool IsOff();
        virtual void On();
        virtual void Off();
        virtual void IOByteOut(int port,byte Byte);
        virtual void InitialMyOutIOData();
};
//------------------------------------------------------------------------------
extern TPLCIOThread *MyPLCIOThread;
#endif
