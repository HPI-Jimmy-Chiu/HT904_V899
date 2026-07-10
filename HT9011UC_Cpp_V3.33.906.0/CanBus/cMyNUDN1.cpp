// =============================================================================
//  CanBus/cMyNUDN1.cpp  --  TMyNUDN1 implementation
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CanBus/cMyNUDN1.cpp (~160 lines)
//  Translation wave: W5-CanBus (leaf unit)
//  Translator: AI(W5-CanBus-Translate) 20260710
//
//  Key changes vs. BCB6 original:
//    - `#include "MachineDefine.h"` / `#pragma hdrstop` / `#pragma package(...)`
//      dropped (BCB-specific); replaced by vclcompat umbrella + <windows.h>
//      (see cMyNUDN1.h for the byte/BYTE/WORD rationale).
//    - `#include "cMyDNM100UD.h"` dropped.  That header (and its sibling .cpp)
//      is explicitly OUT OF SCOPE for this unit -- see cMyNUDN1.h's header
//      comment for the "Owner" ctor-bug note.  This file only needed 3
//      constants + 1 union from it (NUDN1ID/SETW/SETVALUE/UNIONWORD, used by
//      iSetIO_Value_Send below); those are duplicated locally right below
//      with golden line citations instead of pulling the whole header in.
//      `const` at namespace scope has INTERNAL linkage in C++ (unlike C), so
//      this duplication is ODR-safe even after cMyDNM100UD.h is translated
//      and both end up in the same link -- as long as the values stay
//      identical there is no redefinition/link conflict.  TODO(W5-CanBus):
//      once cMyDNM100UD.h is translated, delete this local block and
//      `#include "CanBus/cMyDNM100UD.h"` instead.
//    - Every real DNM100_* vendor call (DNM100_AddDevice/DNM100_AddIOConnection/
//      DNM100_WriteOutputData/DNM100_ReadInputData/DNM100_SendExplicitMSG_W --
//      all from CanBus/DNM100API.h + CanBus/DNM100_BCB.lib, a 泓格/ICP DAS
//      DeviceNet-master vendor SDK) is gated behind `#if HAVE_DNM100`
//      (default OFF -- this machine's active hardware inventory per
//      KNOWLEDGE.md does not list a DNM100UD board; CanBus is W5/未啟用).
//      The `#else` stub simulates a benign all-zero-device / success result,
//      mirroring the TSimIOBackend convention used by the W4-IO HAL
//      (IOBackend.cpp) -- i.e. compiles and is exercisable offline with NO
//      vendor SDK, and a later HAVE_DNM100 build swaps in the real board
//      calls with zero code-shape change.
//    - Ctor: preserves golden's actual (non-commented) behavior exactly --
//      only the 7 scalar config fields are assigned.  Golden's large
//      commented-out block (Initail() call + array zero-init + the two
//      DNM100_Add* calls + iRawItem copy) is DEAD CODE in golden (literally
//      `//`-commented out), left there as historical documentation that the
//      2-phase ctor/Initail() split was deliberately factored out.  Preserved
//      here as a comment for the same reason -- NOT executed, matching
//      golden.  Concretely this means iSetIO_Value[]/iGetIO_Value[]/
//      InAreaMonitor are left uninitialized until a caller separately invokes
//      Initail() (see CanBus/cMyDNM100UD.cpp:52-58 ctor then :127 Initail()
//      call sites, confirmed by cross-tree grep).
// =============================================================================
#include "CanBus/cMyNUDN1.h"

#include <cstring>   // memset

#if HAVE_DNM100
#include "CanBus/DNM100API.h"   // READ-ONLY vendor header (泓格 DNM100UD SDK)
#endif

// ---------------------------------------------------------------------------
//  Local copies of constants/types normally pulled from sibling
//  CanBus/cMyDNM100UD.h (golden cMyDNM100UD.h:6 / :9 / :14 / :93-100).
//  See file-header note above for why these are duplicated instead of
//  #include-d, and why the duplication is ODR-safe.
// ---------------------------------------------------------------------------
const WORD NUDN1ID   = 0x66;   // golden cMyDNM100UD.h:6   Class ID : NUDN1 ID
const byte SETW      = 0x10;   // golden cMyDNM100UD.h:9   Service Code : Setting Write
const byte SETVALUE  = 0x65;   // golden cMyDNM100UD.h:14  Attribute ID : Setting Value

union UNIONWORD   // golden cMyDNM100UD.h:93-100
{
    int WHOLE_WORD;
    struct
    {
        char Lo_BYTE;
        char Hi_BYTE;
    } BYTES;
};

// ---------------------------------------------------------------------------
//  ctor / dtor
// ---------------------------------------------------------------------------
TMyNUDN1::TMyNUDN1(unsigned char _iActivedBoardNo, unsigned char _iDesMACID, unsigned char _iType,
                   unsigned short _iInputLen, unsigned short _iOutputLen, unsigned short _iEPR,
                   int _iAmplifier)
    // golden: ": TComponent(Owner)" here -- TComponent base dropped (see
    // cMyNUDN1.h header comment: this class's TComponent inheritance is
    // unused VCL boilerplate, and golden's base-class-init argument `Owner`
    // is itself a latent bug -- an unqualified name that resolves to
    // `this->Owner`, an inherited-but-not-yet-constructed member, i.e. an
    // uninitialized read used as the AOwner ctor argument).  Dropping the
    // base makes that bug moot for this file.
{
    iActivedBoardNo = _iActivedBoardNo;
    iDesMACID       = _iDesMACID;
    iType           = _iType;
    iInputLen       = _iInputLen;
    iOutputLen      = _iOutputLen;
    iEPR            = _iEPR;
    iAmplifier      = _iAmplifier;

    // golden ctor body below this point is ENTIRELY commented out (`//`) --
    // i.e. dead code in the original, NOT executed.  Preserved verbatim as
    // documentation of the intended (but factored-out) 2-phase init:
    //
    //     Initail();
    //     for(int i=0; i<16; i++)
    //     {
    //         iGetIO_Value[i]=0;
    //         iSetIO_Value[i]=-1;
    //     }
    //     DNM100_AddDevice(iActivedBoardNo,iDesMACID,2500);
    //     DNM100_AddIOConnection(iActivedBoardNo,iDesMACID,iType,iInputLen,iOutputLen,iEPR);
    //     for(i=0; i<40; i++)
    //     {
    //         InAreaMonitor.iRawItem[i]=iInitailRaw[i];
    //     }
    //
    // Real init happens when the owning TMyDNM100UD_Card calls Initail()
    // separately after construction (golden CanBus/cMyDNM100UD.cpp:127).
}

TMyNUDN1::~TMyNUDN1()
{
}

// ---------------------------------------------------------------------------
//  Initail() -- second-phase init (registers this station with the DNM100UD
//  master + zeroes the value/setpoint arrays).  golden cMyNUDN1.cpp:39-56.
// ---------------------------------------------------------------------------
void TMyNUDN1::Initail()
{
    int i;
    unsigned char iInitailRaw[40];   // NOTE(golden gap): never initialized before use below --
                                     // preserved faithfully, NOT fixed (see loop at bottom).
    for(int i=0; i<16; i++)          // NOTE: shadows the outer `int i` above (golden as-is;
                                     // harmless here since the outer `i` is only used, never
                                     // read-before-write, in the second loop below).
    {
        iGetIO_Value[i] = 0;
        iSetIO_Value[i] = -1;
    }

#if HAVE_DNM100
    DNM100_AddDevice(iActivedBoardNo, iDesMACID, 2500);
    DNM100_AddIOConnection(iActivedBoardNo, iDesMACID, iType, iInputLen, iOutputLen, iEPR);
#endif
    // #else (no vendor SDK): nothing to register offline -- matches golden's
    // own lack of return-code checking on these two calls (fire-and-forget).

    for(i = 0; i < 40; i++)
    {
        // GOLDEN BUG (preserved, not fixed): iInitailRaw[] above is never
        // written before this copy, so this reads 40 bytes of indeterminate
        // stack memory into InAreaMonitor.  Faithful translation of golden
        // cMyNUDN1.cpp:52-55.
        InAreaMonitor.iRawItem[i] = iInitailRaw[i];
    }
}

// ---------------------------------------------------------------------------
//  iReadInputData -- poll the DeviceNet I/O-Communication area.
//  golden cMyNUDN1.cpp:57-109.  Only mode 2 (Monitor mode) is implemented in
//  golden; modes 0/1/3/4 are empty branches (return 0), preserved as-is.
// ---------------------------------------------------------------------------
int TMyNUDN1::iReadInputData(int iOperatingMode)
{
    int iRet, i;
    unsigned char iIOData[50];
    unsigned char iInputData[50];
    unsigned short iLen;   // Sam 20210518 : fix InputData being zeroed out on a read error

    iLen = iInputLen;      // Sam 20210518 : fix InputData being zeroed out on a read error

    memset(iIOData, 0, 50);

    for(i = 0; i < 50; i++)
    {
        iInputData[i] = 0;
    }

    if(iOperatingMode == 0)          // NUDN1 I/O Communication Standard mode
    {
    }
    else if(iOperatingMode == 1)     // NUDN1 I/O Communication External input mode
    {
    }
    else if(iOperatingMode == 2)     // NUDN1 I/O Communication Monitor mode
    {
#if HAVE_DNM100
        iRet = DNM100_WriteOutputData(iActivedBoardNo, iDesMACID, iType, iOutputLen, iIOData);
        if(iRet != 0)
            return -iRet;

        iRet = DNM100_ReadInputData(iActivedBoardNo, iDesMACID, iType, &iLen, iInputData);   // Sam 20210518
        if(iRet == 0)
        {
            for(i = 0; i < iLen; i++)   // Sam 20210518
            {
                InAreaMonitor.iRawItem[i] = iInputData[i];
            }
        }
        else
        {
            return -iRet;
        }
#else
        // No vendor SDK: simulate a successful poll of an all-zero device
        // (iInputData[] is already zeroed above).  Mirrors the TSimIOBackend
        // "benign success" default used elsewhere in the HAL (IOBackend.cpp).
        iRet = 0;
        (void)iRet;
        for(i = 0; i < iLen; i++)
        {
            InAreaMonitor.iRawItem[i] = iInputData[i];
        }
#endif
    }
    else if(iOperatingMode == 3)     // NUDN1 I/O Communication Full mode
    {
    }
    else if(iOperatingMode == 4)     // NUDN1 I/O Communication Minimum mode
    {
    }
    else
    {
        return -1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
//  iGetValue / bGetIO -- pure reads off the cached InAreaMonitor snapshot
//  (populated by iReadInputData above).  No vendor calls; unchanged.
//  golden cMyNUDN1.cpp:111-133.
// ---------------------------------------------------------------------------
int TMyNUDN1::iGetValue(int iAmplifierNo)
{
    if(iAmplifierNo >= 0 && iAmplifierNo <= 15)
    {
        return InAreaMonitor.CurrectValue(iAmplifierNo);
    }
    else
    {
        return 0;
    }
}

bool TMyNUDN1::bGetIO(int iAmplifierNo)
{
    if(iAmplifierNo >= 0 && iAmplifierNo <= 15)
    {
        return InAreaMonitor.CurrectOutput(iAmplifierNo);
    }
    else
    {
        return false;
    }
}

// ---------------------------------------------------------------------------
//  iSetIO_Value_Send -- send an explicit "SetValue" message to change one
//  amplifier channel's threshold.  golden cMyNUDN1.cpp:135-160.
// ---------------------------------------------------------------------------
int TMyNUDN1::iSetIO_Value_Send(int iAmplifierNo, int iSetValue)
{
    int iRet;
    UNIONWORD iSetValue_HL;
    unsigned char AttributeID[3];
    // Input
    unsigned char  ServiceID       = SETW;
    unsigned short ClassID         = NUDN1ID;
    unsigned short InstanceID      = (iAmplifierNo + 1) * 10 + 2;
    unsigned short AttributeIDLen  = 3;
    iSetValue_HL.WHOLE_WORD        = (unsigned short)iSetValue;
    AttributeID[0]                 = SETVALUE;
    AttributeID[1]                 = iSetValue_HL.BYTES.Lo_BYTE;   // 低位元
    AttributeID[2]                 = iSetValue_HL.BYTES.Hi_BYTE;   // 高位元

    if(iAmplifierNo >= 0 && iAmplifierNo <= 15)
    {
#if HAVE_DNM100
        iRet = DNM100_SendExplicitMSG_W(iActivedBoardNo, iDesMACID, ServiceID, ClassID, InstanceID,
                                         AttributeIDLen, AttributeID);
#else
        // No vendor SDK: simulate a successful send.
        iRet = 0;
        // Keep the prepared message fields referenced when HAVE_DNM100 is off
        // (avoid -Wunused-but-set-variable on ClassID/InstanceID/AttributeIDLen/
        // AttributeID/ServiceID -- they exist purely to be handed to the real
        // vendor call in the #if branch above).
        (void)ServiceID; (void)ClassID; (void)InstanceID; (void)AttributeIDLen; (void)AttributeID[0];
#endif
    }
    else
    {
        iRet = -1;
    }
    return -iRet;
}
