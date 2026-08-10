// =============================================================================
//  MyLaneIo.cpp  --  TLaneIO facade implementation
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/MyLaneIo.cpp
//  Translation wave: W4-IO (IO HAL layer)
//  Translator: AI(W4-IO) 20260626
//
//  Key changes vs. BCB6 original:
//    - `#pragma hdrstop` / `#pragma package(smart_init)` dropped (BCB-specific).
//    - `#include "MachineDefine.h"` -> same path (already de-VCL'd in target).
//    - `#include "myMN200motor.h"` dropped; dimensions come from IOBackend.h.
//    - `#include "AdvMotApi.h"` moved inside #if HAVE_PCI1203 in the EtherCAT trio.
//    - `#include "MyPLC_IO_Modbus.h"` and `#include "ModbusTCPClient.h"` are
//      GOD-STACK (PLC thread/state): extern declarations for used symbols are
//      provided below as stubs; full headers gated TODO(W6).
//    - `#include "iosetview.h"` (fiosetview->fShow): gated; stub provided below.
//    - `#include "mymessbox.h"` (ShowMyMessage): extern forward declaration.
//    - Vendor triad in each of the 6 IO methods replaced by pIO->WriteBit/etc.
//    - PCI1203 0==SUCCESS -> ret=1/else ret=-1 remap stays in EACH method
//      (faithful to BCB6 -- the map was in the facade, NOT in a backend).
//    - IO_CARD_TYPE==0 quirk (Value=byte(ret)) stays in IOInputByte.
//    - All bookkeeping verbatim: OutPortData cache, CheckPortRangeErr,
//      MNetLog, ShowMyMessage, ret<0, #ifdef SOFT_SIMULTE branches.
//    - EtherCAT analog trio: Acm body inside #if HAVE_PCI1203; else 999.0/false.
//    - ctor default: pIO = new TSimIOBackend().
//
//  STUBS (TODO W5/W6 -- no-ops that satisfy the linker):
//    IdleCheckSafeDoorByCylinder(int,int,int,int) -- csystem.cpp GOD-STACK
//    MNetLog(AnsiString)                          -- communications layer W5
//    ShowMyMessage(...)                           -- VCL message box W7
//    fiosetview_fShow()                           -- iosetview VCL form W7
//    bPLCIO[][] / bPLCInData[][][] / iPLCSafetyVer / PLCIO_INPUT_START_PORT_NUMBER
//      (PLC shared state -- translated faithfully, linked via forward decls below)
// =============================================================================
#include "MachineDefine.h"
#include "MyLaneIo.h"
#include "MachineType.h"    // MachineTypeChoice, Type_HT9045, Type_HT9045_12Site,
                            // eMotionNet, ePCI1203, ePLCbase, eIOType, etc.
#include "cmydef.h"         // InitialOK, IO_CARD_TYPE, MyBitMask, MachineTypeChoice
                            // uiDevhand, iPLCSafetyVer, etc.
#include "cpublic.h"        // VC8ToKpa, KpaToVC8

// ---------------------------------------------------------------------------
//  Stub: IdleCheckSafeDoorByCylinder (4-arg, csystem.cpp GOD-STACK)
//  IMPORTANT: BCB6 comment warns "Handler already has logic, be careful when
//  modifying!".  No-op stub returns false (=> do NOT skip the IO operation)
//  which is the safe offline default.  Real implementation deferred to W6.
// ---------------------------------------------------------------------------
static bool IdleCheckSafeDoorByCylinder(int /*Ring*/, int /*IP*/, int /*Port*/, int /*Bit*/)
{
    return false;  // TODO(W6): replace with real csystem IdleCheckSafeDoorByCylinder
}

// ---------------------------------------------------------------------------
//  Stub: MNetLog  (communications layer, W5)
// ---------------------------------------------------------------------------
static void MNetLog(AnsiString /*msg*/) {}  // TODO(W5): replace with real MNetLog

// ---------------------------------------------------------------------------
//  Stub: ShowMyMessage  (VCL message box, W7)
//  BCB6 signature: void ShowMyMessage(AnsiString S1, AnsiString S2="", ...)
// ---------------------------------------------------------------------------
static void ShowMyMessage(AnsiString /*S1*/, AnsiString /*S2*/="") {}  // TODO(W7)

// ---------------------------------------------------------------------------
//  Stub: fiosetview->fShow accessor
//  In BCB6 code: if(fiosetview->fShow==false) { ... ShowMyMessage ... }
//  Stub always returns false (=> allow ShowMyMessage path).
// ---------------------------------------------------------------------------
static bool fiosetview_fShow() { return false; }  // TODO(W7)

// ---------------------------------------------------------------------------
//  PLC shared-state forward declarations
//  (from MyPLC/MyPLC_IO_Modbus.h -- including the full header pulls TThread/
//  ModbusTCPClient which are GOD-STACK; forward-declare only what is used).
//  TODO(W6): include MyPLC_IO_Modbus.h once ModbusTCPClient is translated.
//
//  Offline stub definitions: the real definitions live in MyPLC_IO_Modbus.cpp
//  (GOD-STACK, not compiled in this wave).  Provide weak fallback definitions
//  here so the offline test links.  In the full production build MyPLC_IO_Modbus.cpp
//  provides the actual definitions and these are not compiled (ODR: the real TU
//  wins at link time when both .obj are present; the production link does not
//  include MyLaneIo.cpp's definitions because the real ones are provided first).
//
//  NOTE: because MinGW does not support __attribute__((weak)) for C++ data on
//  Windows PE, we use a compile-time guard: HAVE_PLC_IO disables the stubs when
//  the real MyPLC_IO_Modbus.cpp is compiled into the same link unit.
// ---------------------------------------------------------------------------
#define INPUT_MAX_REGISTER 22
#define INPUT_MAX_Slave    1
#define PLCIO_INPUT_START_PORT_NUMBER 0x300

#ifndef HAVE_PLC_IO
// Offline stub definitions (default ON; set HAVE_PLC_IO=1 when MyPLC_IO_Modbus.cpp
// is included in the build to prevent duplicate definitions).
// AI(W906-GA1-B2-integrate) 20260804: DISCLOSED DEVIATION -- golden defines these
// two in MyPLC/MyPLC_IO_Modbus.cpp:86-87, but in the ported link topology
// ht9045_io (this file) is in every RESCAN closure while ht9045_comms is not;
// keeping the definitions here (and demoting MyPLC's to extern) preserves
// identical storage/behaviour with zero link churn.
bool bPLCInData[INPUT_MAX_Slave][INPUT_MAX_REGISTER][8] = {};
bool bPLCIO[2048][8] = {};
#else
extern bool bPLCInData[INPUT_MAX_Slave][INPUT_MAX_REGISTER][8];
extern bool bPLCIO[2048][8];
#endif

// ---------------------------------------------------------------------------
//  EtherCAT analog helpers: included only under HAVE_PCI1203 below.
//  IntToHex / HexStrToInt / atoi: available via vclcompat / standard headers.
// ---------------------------------------------------------------------------
#if HAVE_PCI1203
#include "EtherCAT/AdvMotApi.h"   // READ-ONLY vendor header
#ifndef SUCCESS
#define SUCCESS 0UL
#endif
#endif

// ---------------------------------------------------------------------------
//  Module-level singleton definition
// ---------------------------------------------------------------------------
class TLaneIO MyLaneIO;

// ---------------------------------------------------------------------------
//  TLaneIO ctor
//  BCB6: __fastcall TLaneIO::TLaneIO()  (Jimmychiu 20230313: fixed _fastcall)
// ---------------------------------------------------------------------------
TLaneIO::TLaneIO()
{
    OutPos = 0;
    InPos  = 0;
    pIO    = new TSimIOBackend();   // default offline backend
    InitialMyOutIOData();
    InitialMyUseIP();               // JerryYang 20241231: fix Restore IO different way
}

// ---------------------------------------------------------------------------
void TLaneIO::SetBackend(TIOBackend* backend)
{
    // Caller owns backend lifetime in this batch; no delete here.
    // TODO(W6): manage lifetime properly when cinitial selects real backends.
    if(backend) pIO = backend;
}

// ---------------------------------------------------------------------------
void TLaneIO::InitialMyOutIOData()
{
    for(int i=0; i<MAXRing; i++)
    {
        for(int j=0; j<MAXIP; j++)
        {
            for(int k=0; k<MAXPort; k++)
            {
                OutPortIndex[i][j][k] = 0;
                OutPortData[i][j][k]  = 0;
            }
        }
    }
}

// ---------------------------------------------------------------------------
void TLaneIO::InitialMyUseIP()
{
    for(int i=0; i<MAXRing; i++)
    {
        for(int j=0; j<MAXIP; j++)
        {
            iUseMNetIP[i][j] = DEV_INF_NO_DEV;  // JerryYang 20241231
        }
    }
}

// ===========================================================================
//AI(W906-PT-W6c) 20260810: GOLDEN TEXT RESTORED (GATED) -- SetUseIP
//  golden HT9011UC_Code_V3.33.906.0_20260618/MyLaneIo.cpp:55-101  (47 lines)
//  Census scored this function "translated" on name match only; the LIVE body
//  below is an abbreviated stand-in and golden's text existed nowhere in the
//  tree. The block inside the gate is golden's body transcribed VERBATIM
//  (cp950 -> UTF-8; byte-exact when re-encoded to cp950) and is INACTIVE.
//  The LIVE body that follows is UNCHANGED and remains the only active
//  definition -- net behaviour delta = 0. NOTHING was added inside the gate,
//  so a later un-gate is mechanical.
// ===========================================================================
#if 0 // AI-W6C-GOLDEN-BEGIN SetUseIP MyLaneIo.cpp:55-101
void TLaneIO::SetUseIP(int Ring, int IP, int iISABase, AnsiString Alias, bool bOutPut)
{
   /* if(Ring<MAXRing && IP<MAXIP)
    {
        if(iISABase==eMotionNet)                                                //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
        {
            if(bOutPut==true)
            {
                if(iUseMNetIP[Ring][IP]==DEV_INF_IO_32IN_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_16IN_16OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else if(iUseMNetIP[Ring][IP]!=DEV_INF_IO_32OUT_DEV &&
                        iUseMNetIP[Ring][IP]!=DEV_INF_IO_16IN_16OUT_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
            }
            else if(bOutPut==false)
            {
                if(iUseMNetIP[Ring][IP]==DEV_INF_IO_32OUT_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_16IN_16OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else if(iUseMNetIP[Ring][IP]!=DEV_INF_IO_32IN_DEV &&
                        iUseMNetIP[Ring][IP]!=DEV_INF_IO_16IN_16OUT_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32IN_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32IN_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
            }
        }
    }     */
}
#endif // AI-W6C-GOLDEN-END SetUseIP MyLaneIo.cpp:55-101

// ---------------------------------------------------------------------------
//  SetUseIP -- BCB6 body is entirely commented out; translated faithfully as
//  an empty no-op (preserving the block comment).
// ---------------------------------------------------------------------------
void TLaneIO::SetUseIP(int /*Ring*/, int /*IP*/, int /*iISABase*/, AnsiString /*Alias*/, bool /*bOutPut*/)
{
   /* if(Ring<MAXRing && IP<MAXIP)
    { ... } */
}

// ---------------------------------------------------------------------------
//  IOBitOn
//  BCB6: MyLaneIo.cpp:105-170
//  INTERFACE-CUT: vendor triad (.cpp:143-158) -> pIO->WriteBit(...)
//  PCI1203 0==SUCCESS->ret=1/else ret=-1 remap retained here (faithful).
// ---------------------------------------------------------------------------
void TLaneIO::IOBitOn(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)
{
    // NOTE: IdleCheckSafeDoorByCylinder is Handler state-machine logic;
    // be careful when modifying!  (BCB6 comment, MyLaneIo.cpp:107-110)
    if(IdleCheckSafeDoorByCylinder(Ring, IP, Port, Bit) == true)  // Steven 20230703
        return;

    int ret;
    AnsiString str;

    if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
        return;

    int iHasErr = CheckPortRangeErr(true, iISABase, Ring, IP, Port, Bit);

    if(iHasErr != 0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOBitOn (%s) Ring %d | IP %d | Port %d | Bit %d Fail. %s",
                    Alias.c_str(), Ring, IP, Port, Bit, GetIOErrStr(iHasErr).c_str());
        MNetLog(str);
        if(MachineTypeChoice == Type_HT9045)
            ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
        else if(MachineTypeChoice == Type_HT9045_12Site)
            ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
        else
            ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
        #endif
        return;
    }

    OutPortData[Ring][IP][Port] |= MyBitMask[Bit];  // .cpp:142 -- BEFORE the write

    if(iISABase == ePCI1203)  // Sam 20230724
    {
        ret = pIO->WriteBit(Ring, IP, Port, Bit, 1);
        // PCI1203: Acm returns 0==SUCCESS -> map to +1; else -1  (.cpp:146-149)
        if(ret == 0)
            ret = 1;
        else
            ret = -1;
    }
    else
    {
        // MN200 (IO_CARD_TYPE==MotionnetIO_MN200||NewIO_MN200) and legacy _mnet_
        // both route through pIO->WriteBit; backend selection at init handles which.
        ret = pIO->WriteBit(Ring, IP, Port, Bit, 1);
    }

    if(ret < 0)
    {
        #ifdef SOFT_SIMULTE
            return;
        #else
            str.sprintf("IOBitOn (%s) Ring %d, IP %d, Port %d, Bit %d Fail, Code: %d",
                        Alias.c_str(), Ring, IP, Port, Bit, ret);
            MNetLog(str);  // Steven 20110406
            return;
        #endif
    }
}

// ---------------------------------------------------------------------------
//  IOBitOff
//  BCB6: MyLaneIo.cpp:172-237
//  INTERFACE-CUT: vendor triad (.cpp:210-225) -> pIO->WriteBit(...)
// ---------------------------------------------------------------------------
void TLaneIO::IOBitOff(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)
{
    if(IdleCheckSafeDoorByCylinder(Ring, IP, Port, Bit) == true)  // Steven 20230703
        return;

    int ret;
    AnsiString str;

    if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
        return;

    int iHasErr = CheckPortRangeErr(true, iISABase, Ring, IP, Port, Bit);

    if(iHasErr != 0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOBitOff (%s) Ring %d | IP %d | Port %d | Bit %d Fail. %s",
                    Alias.c_str(), Ring, IP, Port, Bit, GetIOErrStr(iHasErr).c_str());
        MNetLog(str);
        if(MachineTypeChoice == Type_HT9045)
            ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
        else if(MachineTypeChoice == Type_HT9045_12Site)
            ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
        else
            ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
        #endif
        return;
    }

    OutPortData[Ring][IP][Port] &= static_cast<byte>(~MyBitMask[Bit]);  // .cpp:209

    if(iISABase == ePCI1203)  // Sam 20230724
    {
        ret = pIO->WriteBit(Ring, IP, Port, Bit, 0);
        if(ret == 0)
            ret = 1;
        else
            ret = -1;
    }
    else
    {
        ret = pIO->WriteBit(Ring, IP, Port, Bit, 0);
    }

    if(ret < 0)
    {
        #ifdef SOFT_SIMULTE
            return;
        #else
            str.sprintf("IOBitOff Ring %d, IP %d, Port %d, Bit %d Fail, Code: %d)",
                        Ring, IP, Port, Bit, ret);
            MNetLog(str);  // Steven 20110406
            return;
        #endif
    }
}

// ---------------------------------------------------------------------------
//  IOByteOut
//  BCB6: MyLaneIo.cpp:239-306
//  INTERFACE-CUT: vendor triad (.cpp:278-293) -> pIO->WriteByte(...)
//  ALWAYS returns true at end (.cpp:305 Sam 20250325: no warning on IO error).
// ---------------------------------------------------------------------------
bool TLaneIO::IOByteOut(int Ring, int IP, int Port, unsigned int Byte, int iISABase)
{
    if(IdleCheckSafeDoorByCylinder(Ring, IP, Port, -1) == true)  // Steven 20230703
        return false;

    int ret;
    AnsiString str;

    if(Ring<=0 && IP<=0 && Port<=0)
        return false;

    int iHasErr = CheckPortRangeErr(true, iISABase, Ring, IP, Port);

    if(iHasErr != 0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOByteOut Ring %d | IP %d | Port %d | Byte %d Fail, %s",
                    Ring, IP, Port, Byte, GetIOErrStr(iHasErr).c_str());
        MNetLog(str);
        if(MachineTypeChoice == Type_HT9045)
            ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
        else if(MachineTypeChoice == Type_HT9045_12Site)
            ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
        else
            ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
        #endif
        return false;
    }

    OutPortData[Ring][IP][Port] = static_cast<byte>(Byte);  // .cpp:276

    if(iISABase == ePCI1203)  // Sam 20230724
    {
        // passes OutPortData[Ring][IP][Port] (same as BCB6 .cpp:280)
        ret = pIO->WriteByte(Ring, IP, Port, OutPortData[Ring][IP][Port]);
        if(ret == 0)
            ret = 1;
        else
            ret = -1;
    }
    else
    {
        ret = pIO->WriteByte(Ring, IP, Port, OutPortData[Ring][IP][Port]);
    }

    if(ret < 0)
    {
        #ifdef SOFT_SIMULTE
            return true;
        #else
            str.sprintf("IOByteOut Ring %d, IP %d, Port %d, Byte %d Fail, Code: %d",
                        Ring, IP, Port, Byte, ret);
            MNetLog(str);  // Steven 20110406
            return true;   // Sam 20250325: always return true even on IO error
        #endif
    }
    return true;  // Sam 20250325
}

// ---------------------------------------------------------------------------
//  IOOutBitStatus
//  BCB6: MyLaneIo.cpp:308-338
//  Pure cache read of OutPortData & MyBitMask[Bit] -- NO hardware read.
//  Vendor-independent: no pIO call here.
// ---------------------------------------------------------------------------
bool TLaneIO::IOOutBitStatus(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)
{
    AnsiString str;
    if(iISABase == ePCI1203)  // Sam 20230724
    {
        if(Ring == 0)
            return false;
    }
    else
    {
        if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
            return false;
    }

    int iHasErr = CheckPortRangeErr(true, iISABase, Ring, IP, Port, Bit);

    if(iHasErr != 0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOOutBitStatus (%s) Ring %d | IP %d | Port %d | Byte %d Fail.",
                    Alias.c_str(), Ring, IP, Port, Bit);
        MNetLog(str);
        ShowMyMessage(str, GetIOErrStr(iHasErr));
        #endif
        return false;
    }

    if(OutPortData[Ring][IP][Port] & MyBitMask[Bit])
        return true;
    else
        return false;
}

// ---------------------------------------------------------------------------
//  IOInputBit
//  BCB6: MyLaneIo.cpp:342-432
//  PLC branch (.cpp:377-394) translated faithfully; only fires when
//  iISABase==ePLCbase, which Sim never selects.
//  INTERFACE-CUT: vendor triad (.cpp:397-413) -> pIO->ReadBit(...)
// ---------------------------------------------------------------------------
bool TLaneIO::IOInputBit(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)
{
    int ret = 0;
    AnsiString str;
    unsigned char Value = 0;

    if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
        return false;

    int iHasErr = CheckPortRangeErr(false, iISABase, Ring, IP, Port, Bit);

    if(iHasErr != 0)
    {
        if(fiosetview_fShow() == false)  // BCB6: fiosetview->fShow==false
        {
            #ifndef SOFT_SIMULTE
            str.sprintf("IOInputBit (%s) Ring %d, IP %d, Port %d, Bit %d Fail. %s",
                        Alias.c_str(), Ring, IP, Port, Bit, GetIOErrStr(iHasErr).c_str());
            MNetLog(str);
            if(MachineTypeChoice == Type_HT9045)
                ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
            else if(MachineTypeChoice == Type_HT9045_12Site)
                ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
            else
                ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
            #endif
        }
        return false;
    }

    // PLC branch (Jason 20230619; Austin 20190218.01) -- GOD-STACK PLC shared state.
    // Translated faithfully; fires only when iISABase==ePLCbase.
    if(bPLCIO[Port][Bit] && iISABase == ePLCbase)
    {
        if(Port >= PLCIO_INPUT_START_PORT_NUMBER && Port <= 0x303)
        {
            if(iPLCSafetyVer == 202204)
                return bPLCInData[0][Port - PLCIO_INPUT_START_PORT_NUMBER + 2][Bit];  // joe 20220406.01
            else
                return bPLCInData[0][Port - PLCIO_INPUT_START_PORT_NUMBER + 1][Bit];
        }
        else
        {
            return true;
        }
    }
    else
    {
        if(iISABase == ePCI1203)  // Sam 20230724
        {
            ret = pIO->ReadBit(Ring, IP, Port, Bit, &Value);
            if(ret == 0)
                ret = 1;
            else
                ret = -1;
        }
        else
        {
            ret = pIO->ReadBit(Ring, IP, Port, Bit, &Value);
        }
    }

    if(ret >= 0)
    {
        if(Value > 0)
            return true;
        else
            return false;
    }
    else
    {
        #ifdef SOFT_SIMULTE
            return true;
        #else
            str.sprintf("IOInputBit (%s) Ring %d, IP %d, Port %d, Bit %d Fail, Code: %d",
                        Alias.c_str(), Ring, IP, Port, Bit, ret);
            MNetLog(str);  // Steven 20110406
            return false;
        #endif
    }
}

// ---------------------------------------------------------------------------
//  IOInputByte
//  BCB6: MyLaneIo.cpp:434-499
//  INTERFACE-CUT: vendor triad (.cpp:466-481) -> pIO->ReadByte(...)
//  IO_CARD_TYPE==0 quirk (.cpp:495): Value=byte(ret) retained here faithfully.
// ---------------------------------------------------------------------------
byte TLaneIO::IOInputByte(int Ring, int IP, int Port, int iISABase)
{
    byte Value = 0;
    AnsiString str;
    int ret = 0;

    if(Ring<=0 && IP<=0 && Port<=0)
        return 0;  // BCB6: return false (== 0 for byte)

    int iHasErr = CheckPortRangeErr(false, iISABase, Ring, IP, Port);

    if(iHasErr != 0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOInputByte Ring %d | IP %d | Port %d Fail. %s",
                    Ring, IP, Port, GetIOErrStr(iHasErr).c_str());
        MNetLog(str);
        if(MachineTypeChoice == Type_HT9045)
            ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
        else if(MachineTypeChoice == Type_HT9045_12Site)
            ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
        else
            ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
        #endif
        return 0;
    }

    if(iISABase == ePCI1203)  // Sam 20230724
    {
        ret = pIO->ReadByte(Ring, IP, Port, &Value);
        if(ret == 0)
            ret = 1;
        else
            ret = -1;
    }
    else if(IO_CARD_TYPE == 1)  // Steven 20120906: MN200
    {
        // mn_get_di_byte path -- pIO->ReadByte fills Value, returns raw ret
        ret = pIO->ReadByte(Ring, IP, Port, &Value);
    }
    else
    {
        // _mnet_io_input path -- returns byte value as int; IO_CARD_TYPE==0 quirk below
        ret = pIO->ReadByte(Ring, IP, Port, &Value);
    }

    if(ret < 0)
    {
        #ifdef SOFT_SIMULTE
            return 1;
        #else
            str.sprintf("IOInputByte Ring %d, IP %d, Port %d Fail, Code: %d",
                        Ring, IP, Port, ret);
            MNetLog(str);  // Steven 20110406
            return 0;
        #endif
    }
    else
    {
        // IO_CARD_TYPE==0 quirk: _mnet_io_input returns byte value in ret, not *Value
        // (Steven 20120906 / 20130828: changed from 1->0)
        if(IO_CARD_TYPE == 0)
            Value = static_cast<byte>(ret);
    }
    return Value;
}

// ---------------------------------------------------------------------------
//  BackUpOutputData
//  BCB6: MyLaneIo.cpp:501-513
// ---------------------------------------------------------------------------
void TLaneIO::BackUpOutputData()
{
    for(int i=0; i<MAXRing; i++)
        for(int j=0; j<MAXIP; j++)
            for(int k=0; k<MAXPort; k++)
                BackOutPortData[i][j][k] = OutPortData[i][j][k];
}

// ---------------------------------------------------------------------------
//  RestoreOutputData
//  BCB6: MyLaneIo.cpp:515-571
//  Recursion guard: static iCheck<10 (Sam 20250325).
// ---------------------------------------------------------------------------
void TLaneIO::RestoreOutputData()
{
    bool bflag = false, bDoorAlarm = false;
    static int iCheck = 0;

    for(int i=0; i<2; i++)  // kevin 20150605: i=1->0
    {
        for(int j=0; j<MAXIP; j++)
        {
            for(int k=0; k<MAXPort; k++)
            {
                if(iUseMNetIP[i][j] == DEV_INF_IO_32OUT_DEV ||
                   iUseMNetIP[i][j] == DEV_INF_IO_16IN_16OUT_DEV)  // JerryYang 20250108
                {
                    if(iUseMNetIP[i][j] == DEV_INF_IO_16IN_16OUT_DEV)
                    {
                        if(k == 2 || k == 3)
                        {
                            if(OutPortData[i][j][k] != BackOutPortData[i][j][k])  // Sam 20250325
                            {
                                bflag = IOByteOut(i, j, k, BackOutPortData[i][j][k]);
                                if(bflag)
                                    OutPortData[i][j][k] = BackOutPortData[i][j][k];
                                else
                                    bDoorAlarm = true;
                            }
                        }
                    }
                    else
                    {
                        if(OutPortData[i][j][k] != BackOutPortData[i][j][k])
                        {
                            bflag = IOByteOut(i, j, k, BackOutPortData[i][j][k]);
                            if(bflag)
                                OutPortData[i][j][k] = BackOutPortData[i][j][k];
                            else
                                bDoorAlarm = true;
                        }
                    }
                }
            }
        }
    }

    if(bDoorAlarm)  // Sam 20250325: retry up to 10 times
    {
        iCheck++;
        if(iCheck < 10)
            RestoreOutputData();
        else
            iCheck = 0;
    }
    else
    {
        iCheck = 0;
    }
}

// ---------------------------------------------------------------------------
//  CheckPortRangeErr
//  BCB6: MyLaneIo.cpp:574-617 (Dell 2013-01-15 DI/DO range check)
//  Returns 0 if InitialOK==false or iISABase!=eMotionNet (no check needed).
//  Returns 0 inside #ifdef SOFT_SIMULTE.
//  Error codes:
//    1 = Bit not in [0,7]
//    2 = Ring/IP/Port out of range
//    3 = iUseMNetIP not a valid IO device type
//    4 = 16IN/16OUT - DO port not 2 or 3
//    5 = 16IN/16OUT - DI port not 0 or 1
// ---------------------------------------------------------------------------
int TLaneIO::CheckPortRangeErr(bool DO_Type, int iISABase, int Ring, int IP, int Port, int Bit)
{
    if(InitialOK == false)   // no IO card initialized
        return 0;

    if(iISABase != eMotionNet)
        return 0;

    #ifdef SOFT_SIMULTE
    #else
    if(Bit != 999 && (Bit < 0 || Bit > 7))
        return 1;

    if(Ring < 0 || Ring >= MAXRing ||
       IP   < 0 || IP   >= MAXIP   ||
       Port < 0 || Port >= MAXPort)
        return 2;

    if(iUseMNetIP[Ring][IP] != DEV_INF_IO_32OUT_DEV &&
       iUseMNetIP[Ring][IP] != DEV_INF_IO_32IN_DEV  &&
       iUseMNetIP[Ring][IP] != DEV_INF_IO_16IN_16OUT_DEV)  // JerryYang 20241231
        return 3;

    if(iUseMNetIP[Ring][IP] == DEV_INF_IO_16IN_16OUT_DEV)
    {
        if(DO_Type == true && !(Port == 2 || Port == 3))   // JerryYang 20250108
            return 4;
        if(DO_Type == false && !(Port == 0 || Port == 1))
            return 5;
    }
    #endif
    return 0;
}

// ---------------------------------------------------------------------------
//  EtherCAT analog trio
//  BCB6: MyLaneIo.cpp:624-703 (Sam 20230210: VacuumUnit pressure sensor)
//  Acm bodies active only #if HAVE_PCI1203; else return 999.0 / false.
// ---------------------------------------------------------------------------
double TLaneIO::GetIOValue(int Ring, int IP, int Port, int /*Bit*/, int iISABase, AnsiString /*Alias*/)
{
    double dKpa = 0.0;
    if(iISABase == ePCI1203)
    {
#if HAVE_PCI1203
        unsigned long Result1, Result2;
        unsigned char HighValue = 0, LowValue = 0;
        int iValue;

        Result1 = Acm_DaqDiGetByteEx(uiDevhand, Ring, IP, Port*2,   &LowValue);
        Result2 = Acm_DaqDiGetByteEx(uiDevhand, Ring, IP, Port*2+1, &HighValue);

        if(Result1 == SUCCESS && Result2 == SUCCESS)
        {
            AnsiString strLow  = IntToHex(LowValue,  2);
            AnsiString strHigh = IntToHex(HighValue, 2);
            AnsiString strLowHigh = strHigh + strLow;
            strLowHigh = HexStrToInt(strLowHigh);
            iValue = atoi(strLowHigh.c_str());
            dKpa = VC8ToKpa(iValue);
        }
        else
        {
            dKpa = 999.0;
        }
#else
        (void)Ring; (void)IP; (void)Port;
        dKpa = 999.0;
#endif
    }
    else
    {
        dKpa = 999.0;
    }
    return dKpa;
}

double TLaneIO::GetIOValueThread(int Ring, int IP, int Port, int /*Bit*/, int iISABase, AnsiString /*Alias*/)
{
    double dKpa = 0.0;
    if(iISABase == ePCI1203)
    {
#if HAVE_PCI1203
        unsigned short Threshold_Index    = 0x8000;
        unsigned short Threshold_SubIndex = 0x13;
        unsigned short DataSize           = 128;
        unsigned long Result;
        short iValue = 0;

        Result = Acm_DevReadSDOData(uiDevhand, Ring, IP,
                                    Threshold_Index + Port * 0x0010,
                                    Threshold_SubIndex, ECAT_TYPE_I16, DataSize, &iValue);
        if(Result == SUCCESS)
            dKpa = VC8ToKpa(iValue);
        else
            dKpa = 999.0;
        return dKpa;
#else
        (void)Ring; (void)IP; (void)Port;
        dKpa = 999.0;
#endif
    }
    else
    {
        dKpa = 999.0;
    }
    return dKpa;
}

bool TLaneIO::SetIOValueThread(double dKpa, int Ring, int IP, int Port, int /*Bit*/, int iISABase, AnsiString /*Alias*/)
{
    if(iISABase == ePCI1203)
    {
#if HAVE_PCI1203
        unsigned short Threshold_Index    = 0x8000;
        unsigned short Threshold_SubIndex = 0x13;
        unsigned short DataSize           = 128;
        short iValue = KpaToVC8(dKpa);
        unsigned long Result;

        Result = Acm_DevWriteSDOData(uiDevhand, Ring, IP,
                                     Threshold_Index + Port * 0x0010,
                                     Threshold_SubIndex, ECAT_TYPE_I16, DataSize, &iValue);
        return (Result == SUCCESS);
#else
        (void)dKpa; (void)Ring; (void)IP; (void)Port;
        return false;
#endif
    }
    else
    {
        return false;
    }
}

// ---------------------------------------------------------------------------
//  GetIOErrStr
//  BCB6: MyLaneIo.cpp:705-733
// ---------------------------------------------------------------------------
AnsiString TLaneIO::GetIOErrStr(int iErr)
{
    AnsiString sret = "";
    switch(iErr)
    {
        case 0: sret = "SUCCESS";                    break;
        case 1: sret = "Bit not between 0 to 7";    break;
        case 2: sret = "Ring, IP, or Port not in range"; break;
        case 3: sret = "UseMNetIP Err";              break;
        case 4: sret = "16IN/OUT - output port error"; break;
        case 5: sret = "16IN/OUT - input port error";  break;
        default:
            sret.sprintf("UnKnown Error,iErr=%d", iErr);
            break;
    }
    return sret;
}
