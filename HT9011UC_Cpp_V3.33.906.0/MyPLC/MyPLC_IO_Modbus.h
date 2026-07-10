//------------------------------------------------------------------------------
// AI(W5-MyPLC-Translate) 20260710: BCB6 -> standard C++17 translation.
// Provenance: HT9011UC_Code_V3.33.906.0_20260618/MyPLC/MyPLC_IO_Modbus.h
// Changes from BCB6 original:
//   - `class TPLCIOThread : public TThread` -> STANDALONE class (no base).
//     Golden's TThread is VCL Classes::TThread -- not present anywhere in
//     this repo snapshot and no other in-tree file needs a second
//     TThread-derived class this wave (grepped: ScanBtnThread.h/
//     uHeaterThread.h/uShuttleThread.h also derive from it, but none are
//     translated yet). Rather than invent a shared vclcompat/TThread.h (out
//     of this unit's authorized scope -- only ClientSocket.h/.cpp were
//     authorized), this mirrors the acarry.cpp (W6.5) HThreadCtrlShuttle
//     idiom: an OFFLINE INERT driver, no real OS thread ever spawned;
//     Resume() is a no-op; PLCIOProcess() (the Execute() do-while body's
//     per-tick payload) stays directly callable so a future orchestrator can
//     pump it once per tick, exactly like DoInArm/DoOutArm are pumped
//     without a real thread. Execute() itself is kept VERBATIM (compiles,
//     callable) but is never invoked automatically -- see MyPLC_IO_Modbus.cpp.
//   - `Byte DI_Process();`: golden's `Byte` (capital, Borland SysUtils.hpp)
//     is a DIFFERENT spelling from windows.h's lowercase `byte` used
//     elsewhere (e.g. cmydef.h's MyBitMask); a local guarded typedef
//     supplies it (DI_Process/DO_Process are themselves declared-but-never-
//     defined dead code in golden -- see the .cpp note -- kept for fidelity).
//------------------------------------------------------------------------------
#ifndef MyPLC_IO_ModbusH
#define MyPLC_IO_ModbusH

#include "ModbusTCPClient.h"

#ifndef HT9045_BYTE_TYPE_DEFINED
#define HT9045_BYTE_TYPE_DEFINED
typedef unsigned char Byte;   // Borland RTL SysUtils.hpp spelling (capital B);
                              // identical underlying type to windows.h's `byte`
                              // so a duplicate typedef elsewhere is legal C++.
#endif

#define INPUT_MAX_REGISTER 22                                                   //掃30001-30022 (scans 30001-30022)          22
#define INPUT_MAX_Slave 1                                                       //目前只用一個 (only one in use)          1
#define PLCIO_INPUT_START_PORT_NUMBER 0x300
//------------------------------------------------------------------------------
extern int iTransactionID;
extern void InitPLCIO(AnsiString asIP,int iPort);
extern void PLCIOTaskCycle();
extern int iPLCIOTask;
extern bool bScanSlave[INPUT_MAX_Slave];
extern bool bPLCInData[INPUT_MAX_Slave][INPUT_MAX_REGISTER][8];
extern bool bPLCIO[2048][8];                                                    //for 舊架構 (for the legacy architecture)
extern bool bPLCIOEffect;                                                       //已經接收過資料 (data has been received at least once)
extern bool bIOPowered;                                                         //是否有供電? 急停旋鈕解除後按下Reset才算完成解除急停狀態 (is power present? -- after releasing the E-STOP twist button, pressing Reset is what actually completes the E-STOP release)
//------------------------------------------------------------------------------
extern int iPLCStatusTask;                                                      //ben 20230913 add
extern bool bSafePLCThread;
extern void PLCStatusCheck();
//------------------------------------------------------------------------------
extern void bPLCStatusCheck();                                                  //kevin 20250407 check PLC
extern ModbusTCPClient PlcComm;                                                 //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題 (fix PLC safe-door comm delay)
//------------------------------------------------------------------------------
class TPLCIOThread
{
    private:
        bool Terminated;   // golden TThread member; defaults false, never set true offline
    protected:
        void Execute();
    public:
        int Priority;      // golden TThread::TPriority property; plain int offline (see header note)
        explicit TPLCIOThread(bool CreateSuspended);
        void PLCIOProcess(void);
        void Resume();     // golden TThread::Resume(); offline no-op (see header note)
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

        TMyPLC_IO_Modbus();

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
