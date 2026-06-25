// =============================================================================
//  mysensor.cpp  --  TMySensor per-point input sensor reader
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/mysensor.cpp
//  Translation wave: W4-IO (IO HAL layer)
//  Translator: AI(W4-IO) 20260626
//
//  Key changes vs. BCB6 original:
//    - `#pragma hdrstop` / `#pragma package(smart_init)` dropped.
//    - `#include "uPadInterface.h"` (Ken 20210702) is GOD-STACK (VCL form).
//      The iControlPanelMode==1 && fPadInterface->IsPadKey(Name) branch is
//      gated `#if 0 // TODO(W6)` throughout.
//    - eISABase/ePCI1735U raw IOInputBit(Port,Bit) free-func (myio.cpp, W6)
//      is a no-op stub returning false.
//    - eMotionNet||ePCI1203||ePLCbase -> MyLaneIO.IOInputBit  FULLY ACTIVE.
//    - GetTrayBuildState / SetTrayBuildState / ClrTrayBuildState are NOT defined
//      here (they are defined in the main app cinitial/tray module, GOD-STACK).
//      Forward declarations are in mysensor.h.  They are not called in this file.
//    - Commented-out CheckSenPortRangeErr body preserved as comment (BCB6 .cpp:181).
// =============================================================================
#include "MachineDefine.h"
#include "mysensor.h"
#include "MyLaneIo.h"
#include "cmydef.h"     // eMotionNet / ePCI1203 / ePLCbase / eISABase / ePCI1735U

// ---------------------------------------------------------------------------
//  Stub: raw-port IOInputBit(Port,Bit) free-func (myio.cpp, W6)
// ---------------------------------------------------------------------------
static bool IOInputBit(int /*Port*/, int /*Bit*/) { return false; }  // TODO(W6)

// ---------------------------------------------------------------------------
//  Global arrays
// ---------------------------------------------------------------------------
class TMySensor Sen[MAX_SENSOR_ITEM];
class TMySensor SenBackUp[MAX_TTL_BIT];  // Alick 20161011 (Steven): TTL 8-site

// ---------------------------------------------------------------------------
//  Ctor
//  BCB6: _fastcall TMySensor::TMySensor()
// ---------------------------------------------------------------------------
TMySensor::TMySensor()
{
    Name   = "";
    Ring   = 0;
    IP     = 0;
    Port   = 0;
    Bit    = 0;
    Type   = 0;
    Enable = 0;
    Using  = "";
    ISABase = 0;
}

// ---------------------------------------------------------------------------
//  Dtor
//  BCB6: _fastcall TMySensor::~TMySensor()
// ---------------------------------------------------------------------------
TMySensor::~TMySensor()
{
    Enable = false;
}

// ---------------------------------------------------------------------------
//  Status()
//  BCB6: mysensor.cpp:37-77
//  Returns logical sensor state after Type inversion.
// ---------------------------------------------------------------------------
bool TMySensor::Status()
{
    bool ret = false;

#if 0   // TODO(W6): fPadInterface / iControlPanelMode (VCL GOD-STACK, Ken 20210702)
    if(iControlPanelMode==1 && fPadInterface->IsPadKey(Name))
    {
        ret = fPadInterface->ProcessScanKey(Name);
        State = 1;
        return ret;
    }
    else
#endif
    {
        if(Enable == false)
        {
            State = -1;
            return false;
        }

        if(ISABase == eMotionNet ||
           ISABase == ePCI1203   ||  // Sam 20230724: add PCI1203 IO support
           ISABase == ePLCbase)
        {
            ret = MyLaneIO.IOInputBit(Ring, IP, Port, Bit, ISABase, Name);
        }
        else if(ISABase == eISABase  ||  // Nickliu 20230309
                ISABase == ePCI1735U)    // Alick 20160809
        {
            ret = IOInputBit(Port, Bit);  // TODO(W6)
        }

        if(Type)
        {
            State = 1;
            return ret;
        }
        else
        {
            State = 0;
            return !ret;
        }
    }
}

// ---------------------------------------------------------------------------
//  IsOn()
//  BCB6: mysensor.cpp:79-117
// ---------------------------------------------------------------------------
bool TMySensor::IsOn()
{
#if 0   // TODO(W6): fPadInterface / iControlPanelMode
    if(iControlPanelMode==1 && fPadInterface->IsPadKey(Name))
    {
        return fPadInterface->ProcessScanKey(Name);
    }
    else
#endif
    {
        if(Enable == false)
        {
            State = -1;
            return false;
        }

        bool ret = false;
        if(ISABase == eMotionNet ||
           ISABase == ePCI1203   ||  // Sam 20230724
           ISABase == ePLCbase)
        {
            ret = MyLaneIO.IOInputBit(Ring, IP, Port, Bit, ISABase, Name);
        }
        else if(ISABase == eISABase  ||  // Nickliu 20230309
                ISABase == ePCI1735U)    // Alick 20160809
        {
            ret = IOInputBit(Port, Bit);  // TODO(W6)
        }

        if(Type)
        {
            State = 1;
            return ret;
        }
        else
        {
            State = 0;
            return !ret;
        }
    }
}

// ---------------------------------------------------------------------------
//  IsOff()
//  BCB6: mysensor.cpp:119-159
// ---------------------------------------------------------------------------
bool TMySensor::IsOff()
{
    bool ret = false;

#if 0   // TODO(W6): fPadInterface / iControlPanelMode
    if(iControlPanelMode==1 && fPadInterface->IsPadKey(Name))
    {
        ret = fPadInterface->ProcessScanKey(Name);
        State = 0;
        return !ret;
    }
    else
#endif
    {
        if(Enable == false)
        {
            State = -1;
            return false;
        }

        if(ISABase == eMotionNet ||
           ISABase == ePCI1203   ||  // Sam 20230724
           ISABase == ePLCbase)
        {
            ret = MyLaneIO.IOInputBit(Ring, IP, Port, Bit, ISABase, Name);
        }
        else if(ISABase == eISABase  ||  // Nickliu 20230309
                ISABase == ePCI1735U)    // Alick 20160809
        {
            ret = IOInputBit(Port, Bit);  // TODO(W6)
        }

        if(Type)
        {
            State = 0;
            return !ret;
        }
        else
        {
            State = 1;
            return ret;
        }
    }
}

// ---------------------------------------------------------------------------
//  CopySensor()
//  BCB6: mysensor.cpp:161-178
//  Pure field copy -- no IO operations.
// ---------------------------------------------------------------------------
void CopySensor(TMySensor *Source, TMySensor *Target)
{
    if(Source->ISABase == eISABase  ||   // Nickliu 20230309
       Source->ISABase == ePCI1735U)     // Alick 20160809
    {
        Target->Port    = Source->Port;
        Target->Bit     = Source->Bit;
        Target->Type    = Source->Type;
    }
    else
    {
        Target->Ring    = Source->Ring;
        Target->IP      = Source->IP;
        Target->Port    = Source->Port;
        Target->Bit     = Source->Bit;
        Target->Type    = Source->Type;
    }
}

// ---------------------------------------------------------------------------
//  CheckSenPortRangeErr -- commented out in BCB6 .cpp:181-232
//  Not translated (body was already disabled upstream).
// ---------------------------------------------------------------------------
