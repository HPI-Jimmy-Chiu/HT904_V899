// =============================================================================
//  EtherCAT/MyNUEC1.cpp  --  TMyNUEC1 (FS-N12 EtherCAT current-amplifier) impl
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/EtherCAT/MyNUEC1.cpp (419 lines)
//  Translation wave: W5 comms (EtherCAT/MyNUEC1 comms-only unit)
//  Translator: AI(W5-MyNUEC1-Translate) 20260710
//
//  Vendor gating: this file's Acm_DaqDiGetByteEx / Acm_DevReadSDOData /
//  Acm_DevWriteSDOData calls are gated behind `#if HAVE_PCI1203`, REUSING the
//  flag this codebase's W4-IO wave already established for this EXACT vendor
//  API family (EtherCAT/AdvMotApi.h, same uiDevhand master handle) -- see
//  IOBackend.cpp's TPci1203Backend and MyLaneIo.cpp's "EtherCAT analog trio"
//  (GetIOValue/GetIOValueThread/SetIOValueThread), which already call these
//  SAME three functions under this SAME guard.  This unit's own spec text
//  suggested inventing/reusing a name like "HAVE_ETHERCAT"; that flag does
//  NOT exist anywhere in the translated tree (only mentioned as a *reserved,
//  not-yet-implemented* name in MIGRATION_ROADMAP.md's DEFERRED table for the
//  separate Motor/myEthercatmotor.cpp axis driver, W4-part2).  Reusing the
//  REAL, already-implemented HAVE_PCI1203 flag -- rather than introducing a
//  second flag for the identical vendor header/global -- is the more
//  faithful "reuse, don't invent" choice; flagged in this unit's report.
//  Default (HAVE_PCI1203 undefined): every method below takes the exact same
//  fallback values golden's own "Result != SUCCESS" branches already write
//  (iCurrectValue/iSettingValue=-99999, bRet=false, RetWriteSettingValue=1,
//  IO read false, CheckAmplifierExist false) -- i.e. the stub path is
//  golden's own already-modeled communication-failure path, not a new one.
// =============================================================================
#include "EtherCAT/MyNUEC1.h"
#include "MachineType.h"    // eNUEC1, eNUEC1A (enum eNUECType)
#include "cmydef.h"         // NUEC_TYPE (extern int), uiDevhand (extern unsigned int)

#include <cstdlib>          // atoi

#if HAVE_PCI1203
//AI(W906-PCI1203-IncludeFix) 20260818: was "EtherCAT/AdvMotApi.h" -- that path
//  does not exist (only EtherCAT/vendor/AdvMotApi.h does) and fails to compile
//  the moment HAVE_PCI1203 is actually turned on (verified: this exact
//  #include is what a HAVE_PCI1203=1 probe build hit first, "No such file or
//  directory"). EtherCAT/vendor/ is on ht9045_motor's PUBLIC include path
//  (CMakeLists.txt), so the bare form resolves correctly -- same form
//  EtherCAT/MyEtherCAT.cpp:114 already uses (and its own banner already
//  explains why the "EtherCAT/"-prefixed form is wrong). This file and
//  IOBackend.cpp/MyLaneIo.cpp had independently kept the wrong form; all
//  three fixed together, see those files' own AI(W906-PCI1203-IncludeFix) notes.
#include "AdvMotApi.h"           // READ-ONLY vendor header (Acm_Dev*/Acm_Daq*, SUCCESS,
                                 // ECAT_TYPE_I16/U16/BOOL -- pulled in transitively from
                                 // AdvMotDev.h/AdvMotDrv.h/AdvMotPropID.h/AdvMotErr.h)
#endif

// ---------------------------------------------------------------------------
//  Ctor / dtor
//  golden EtherCAT/MyNUEC1.cpp:14-64
// ---------------------------------------------------------------------------
// AI(W5-MyNUEC1-Translate) 20260710: golden's `: TComponent(Owner)` base
// mem-initializer is dropped along with the TComponent base itself (see
// MyNUEC1.h header comment for the full "Owner" ctor-bug diagnosis and the
// in-wave precedent this follows, CanBus/cMyNUDN1.h).  No base ctor call
// remains here because there is no base left to initialize.
TMyNUEC1::TMyNUEC1(unsigned char _iRing, unsigned char _iDesMACID, unsigned char _iAmplifier)
{
    iRing=_iRing;
    iDesMACID=_iDesMACID;
    iAmplifier=_iAmplifier;
    DataSize=128;

    if(NUEC_TYPE==eNUEC1)  //0:NUEC1
    {
        //CurValue
        //method 1, high/low byte: Index=6+N*4(low byte) Index=7+N*4(high byte)
        CurValue_N=4;
        //method 2: Index=0x6100+N*0x10  SubIndex=0x11
        CurValue_Index=0x6100;
        CurValue_SubIndex=0x11;

        //Set IO Value
        SetValue_Index=0x8002;
        SetValue_SubIndex=0x1;

        //IO
        IO_Index=0x3600;        //NG: NU-EC1 does not support reading 16 at once, so still use the old method reading 8 at a time
        IO_SubIndex=0x01;

        iMethod=1;              //NUEC1: both old and new methods can read all 16
    }
    else                  //1:NUEC1A (used on the more expensive HT505S)
    {
        //CurValue
        //method 1, high/low byte: Index=6+N*2(low byte) Index=7+N*2(high byte); on NU-EC1A this method only reads 8
        CurValue_N=2;
        //method 2: Index=0x8001+N*0x10  SubIndex=0x05 (manual says this also works)
        //CurValue_Index=0x8001;
        //CurValue_SubIndex=0x05;
        CurValue_Index=0x6100;
        CurValue_SubIndex=0x11;

        //Set IO Value
        SetValue_Index=0x6100;
        SetValue_SubIndex=0x12;

        //IO
        IO_Index=0x3600;       //NG: NU-EC1A supports reading 16 at once but it's slower, so still use the old method reading 8 at a time
        IO_SubIndex=0x01;

        iMethod=1;              //NUEC1A: old method reads at most 8, so use the new method
    }
    ZeroMemory(iCurrectValue, sizeof(iCurrectValue));
    ZeroMemory(iSettingValue, sizeof(iSettingValue));
    ZeroMemory(iNeedSettingValue, sizeof(iNeedSettingValue));
    ZeroMemory(RetWriteSettingValue, sizeof(RetWriteSettingValue));
    // AI(W5-MyNUEC1-Translate) 20260710: golden ctor does NOT explicitly zero
    // `bNeedDosetValue` (unlike the four ZeroMemory calls above it) -- it
    // relies on the SAME VCL NewInstance zero-fill-on-allocate guarantee
    // discussed above (bNeedDosetValue[16] is a plain bool array member, so
    // it is already all-false at this point in the real binary).  Plain C++
    // `new TMyNUEC1(...)` gives no such guarantee, so this line is ADDED
    // (not present in golden's text) to reproduce golden's ACTUAL runtime
    // behavior (all-false at construction) rather than golden's literal
    // absence of code.  golden EtherCAT/MyNUEC1.cpp:60-63.
    ZeroMemory(bNeedDosetValue, sizeof(bNeedDosetValue));
    // AI(W5-MyNUEC1-Translate) 20260710: SAME reasoning applies to `IOArea`
    // (union member; golden NEVER writes it in the ctor, ONLY inside
    // ReadCurrentIO()'s two branches, which always fully repopulate either
    // IOWord or IOCharL+IOCharH before any bGetIO() read).  Found empirically
    // while writing this unit's own test (tests/test_MyNUEC1.cpp): a
    // stack-local TMyNUEC1 with this line omitted left IOArea holding
    // indeterminate stack bytes, causing a direct-poke bitfield test to read
    // back a neighboring bit as garbage-1 instead of 0.  Golden's real binary
    // never observes this because VCL zero-fills the WHOLE instance
    // (including union members) before the ctor runs; plain C++ `new
    // TMyNUEC1(...)` (golden's actual allocation site, MyEtherCAT.cpp:40)
    // gives no such guarantee.  Added to reproduce the ACTUAL golden runtime
    // behavior (all-zero IOArea until the first ReadCurrentIO() call).
    ZeroMemory(&IOArea, sizeof(IOArea));
}

TMyNUEC1::~TMyNUEC1()
{
}

// ---------------------------------------------------------------------------
//  ReadCurrentValue (all amplifiers)
//  golden EtherCAT/MyNUEC1.cpp:66-76
// ---------------------------------------------------------------------------
bool TMyNUEC1::ReadCurrentValue()
{
    bool bRet=true;
    for(int i=0;i<iAmplifier;i++)
    {
        if(ReadCurrentValue(i)==false)
            bRet=false;
    }
    return bRet;
}

// ---------------------------------------------------------------------------
//  ReadCurrentValue (one amplifier)
//  golden EtherCAT/MyNUEC1.cpp:78-125
// ---------------------------------------------------------------------------
bool TMyNUEC1::ReadCurrentValue(int iAmplifierNo)
{
    bool bRet=true;
#if HAVE_PCI1203
    if(iMethod==0)
    {
        unsigned long Result1,Result2;
        unsigned char HighValue;
        unsigned char LowValue;
        AnsiString strLow="",strHigh="",strLowHigh="";

        Result1=Acm_DaqDiGetByteEx(uiDevhand, iRing, iDesMACID, 6+(iAmplifierNo*CurValue_N), &LowValue);
        Result2=Acm_DaqDiGetByteEx(uiDevhand, iRing, iDesMACID, 7+(iAmplifierNo*CurValue_N), &HighValue);

        if(Result1==SUCCESS && Result2==SUCCESS)
        {
            strLow=IntToHex(LowValue,2);
            strHigh=IntToHex(HighValue,2);
            AnsiString strLowHigh=strHigh+strLow;
            // AI(W5-MyNUEC1-Translate) 20260710: golden roundtrips through
            // AnsiString: HexStrToInt() parses the hex text to an int, then
            // assigns that int BACK into an AnsiString (formats as decimal
            // text, e.g. 2690 -> "2690"), and atoi() re-parses that decimal
            // text.  Net effect is a no-op re-encode of HexStrToInt()'s
            // return value.  Preserved verbatim (not simplified) -- faithful
            // BCB6 idiom, do not "clean up".
            strLowHigh=HexStrToInt(strLowHigh);
            iCurrectValue[iAmplifierNo]=atoi(strLowHigh.c_str());
        }
        else
        {
            iCurrectValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
    else
    {
        unsigned long Result;
        short iCurValue;
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, CurValue_Index+(iAmplifierNo*0x0010), CurValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iCurValue);
        if(Result==SUCCESS)
        {
            iCurrectValue[iAmplifierNo]=iCurValue;
        }
        else
        {
            iCurrectValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
#else
    // No vendor EtherCAT SDK linked (HAVE_PCI1203 undefined): mirror golden's
    // own "Result != SUCCESS" failure path (same as both branches above).
    iCurrectValue[iAmplifierNo]=-99999;
    bRet=false;
#endif
    return bRet;
}

// ---------------------------------------------------------------------------
//  iGetValue
//  golden EtherCAT/MyNUEC1.cpp:127-133
// ---------------------------------------------------------------------------
int TMyNUEC1::iGetValue(int iAmplifierNo)
{
    if(0<=iAmplifierNo && iAmplifierNo<MaxFSN12)
        return iCurrectValue[iAmplifierNo];
    else
        return 0;
}

// ---------------------------------------------------------------------------
//  ReadCurrentIO
//  golden EtherCAT/MyNUEC1.cpp:135-169
// ---------------------------------------------------------------------------
bool TMyNUEC1::ReadCurrentIO()
{
    bool bRet=false;
#if HAVE_PCI1203
    if(NUEC_TYPE==eNUEC1 || NUEC_TYPE==eNUEC1A)  //faster path used by BOTH NUEC1 and NUEC1A
    {
        unsigned long Result1;
        unsigned long Result2;
        unsigned char HighValue;
        unsigned char LowValue;
        // AI(W5-MyNUEC1-Translate) 20260710: `strLamp` is declared but never
        // used anywhere in this function in golden either (dead local,
        // presumably a leftover from removed lamp-state-decode code implied
        // by the comment below).  Preserved verbatim, not removed.
        AnsiString strLamp;
        //after reading, convert to binary: 0=off 1=on
        //10000010 >> bit 2 and bit 8 on
        Result1=Acm_DaqDiGetByteEx(uiDevhand, iRing,  iDesMACID, 0, &LowValue);
        Result2=Acm_DaqDiGetByteEx(uiDevhand, iRing,  iDesMACID, 1, &HighValue);
        if(Result1==SUCCESS && Result2==SUCCESS)
        {
            IOArea.Area.IOCharL.iRaw=LowValue;
            IOArea.Area.IOCharH.iRaw=HighValue;
            bRet=true;
        }
        else
        {
            bRet=false;
        }
    }
    else     //NUEC1A only can also use this method, but it is slower.
    {
        unsigned long Result;
        unsigned short iValue;
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, IO_Index, IO_SubIndex,  ECAT_TYPE_I16, DataSize, &iValue);
        if(Result==SUCCESS)
        {
            IOArea.Area.IOWord.iRaw=iValue;
            bRet=true;
        }
        else
        {
            bRet=false;
        }
    }
#else
    bRet=false;
#endif
    return bRet;
}

// ---------------------------------------------------------------------------
//  bGetIO
//  golden EtherCAT/MyNUEC1.cpp:171-184
// ---------------------------------------------------------------------------
bool TMyNUEC1::bGetIO(int iAmplifierNo)
{
    // AI(W5-MyNUEC1-Translate) 20260710: golden bound check is `<=MaxFSN12`
    // (16), not `<MaxFSN12` -- off-by-one vs iGetValue's `<MaxFSN12` check
    // above.  Harmless here (not a memory-safety bug) only because
    // _IOArea::CurrectOutput/CurrectOutput2's final bare `else` silently
    // aliases iAmplifierNo==16 to the same bit as iAmplifierNo==15 (see
    // MyNUEC1.h comment).  Preserved verbatim, not fixed.
    if(iAmplifierNo>=0 && iAmplifierNo<=MaxFSN12)
    {
        if(NUEC_TYPE==eNUEC1 || NUEC_TYPE==eNUEC1A)    //faster path used by BOTH NUEC1 and NUEC1A
            return IOArea.CurrectOutput2(iAmplifierNo);
        else
            return IOArea.CurrectOutput(iAmplifierNo);
    }
    else
    {
        return false;
    }
}

// ---------------------------------------------------------------------------
//  ReadSettingValue (all amplifiers)
//  golden EtherCAT/MyNUEC1.cpp:186-196
// ---------------------------------------------------------------------------
bool TMyNUEC1::ReadSettingValue()
{
    bool bRet=true;
    for(int i=0;i<iAmplifier;i++)
    {
        if(ReadSettingValue(i)==false)
            bRet=false;
    }
    return bRet;
}

// ---------------------------------------------------------------------------
//  ReadSettingValue (one amplifier)
//  golden EtherCAT/MyNUEC1.cpp:198-224
//  AI(W5-MyNUEC1-Translate) 20260710: golden's two NUEC_TYPE branches are
//  IDENTICAL in behavior (`SetValue_Index+iAmplifierNo*0x0010` vs
//  `SetValue_Index+(iAmplifierNo*0x0010)` -- same value, extra parens only)
//  -- a genuine BCB6 duplicate-branch quirk (matches the project's documented
//  "asymmetric twin functions" gotcha class, here manifesting as a SYMMETRIC
//  twin that never diverged).  Preserved verbatim, not collapsed to one path.
// ---------------------------------------------------------------------------
bool TMyNUEC1::ReadSettingValue(int iAmplifierNo)
{
    bool bRet=true;
#if HAVE_PCI1203
    unsigned long Result;
    short iSetValue;
    if(NUEC_TYPE==eNUEC1)
    {
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSetValue);
        if(Result==SUCCESS)
        {
            iSettingValue[iAmplifierNo]=iSetValue;
        }
        else
        {
            iSettingValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
    else
    {
        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+(iAmplifierNo*0x0010), SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSetValue);
        if(Result==SUCCESS)
        {
            iSettingValue[iAmplifierNo]=iSetValue;
        }
        else
        {
            iSettingValue[iAmplifierNo]=-99999;
            bRet=false;
        }
    }
#else
    iSettingValue[iAmplifierNo]=-99999;
    bRet=false;
#endif
    return bRet;
}

// ---------------------------------------------------------------------------
//  iGetIO_Value
//  golden EtherCAT/MyNUEC1.cpp:226-232
// ---------------------------------------------------------------------------
int TMyNUEC1::iGetIO_Value(int iAmplifierNo)
{
    // AI(W5-MyNUEC1-Translate) 20260710: REAL out-of-bounds read in golden --
    // bound check is `<=MaxFSN12` (16) but iSettingValue[] has only 16 slots
    // (valid indices 0-15), so iAmplifierNo==16 reads one `int` PAST the end
    // of the array (unlike bGetIO's off-by-one above, there is no
    // catch-all-`else` helper here to absorb it).  This is genuine UB in the
    // original BCB6 binary too (not something the C++ port introduces).
    // Preserved verbatim per project convention (do not "fix" a faithfully
    // reproduced BCB6 quirk); flagged prominently here and in this unit's
    // translation report as a real memory-safety finding, not merely a
    // stylistic oddity.
    if(0<=iAmplifierNo && iAmplifierNo<=MaxFSN12)
        return iSettingValue[iAmplifierNo];
    else
        return 0;
}

// ---------------------------------------------------------------------------
//  SetSetIOValue
//  golden EtherCAT/MyNUEC1.cpp:234-238
//  AI(W5-MyNUEC1-Translate) 20260710: no bounds check on iAmplifierNo at all
//  (missing-validation gotcha, matches project's documented pattern class).
//  Preserved verbatim.
// ---------------------------------------------------------------------------
void TMyNUEC1::SetSetIOValue(int iAmplifierNo,int iValue)
{
    bNeedDosetValue[iAmplifierNo]=true;
    iNeedSettingValue[iAmplifierNo]=iValue;
}

// ---------------------------------------------------------------------------
//  WriteSettingValue (all pending amplifiers)
//  golden EtherCAT/MyNUEC1.cpp:240-252
// ---------------------------------------------------------------------------
bool TMyNUEC1::WriteSettingValue()
{
    bool bRet=true;
    for(int i=0;i<iAmplifier;i++)
    {
        if(bNeedDosetValue[i])
        {
            if(WriteSettingValue(i,iNeedSettingValue[i])==false)
                 bRet=false;
        }
    }
    return bRet;
}

// ---------------------------------------------------------------------------
//  WriteSettingValue (one amplifier)
//  golden EtherCAT/MyNUEC1.cpp:254-336
//  RetWriteSettingValue[] error codes (golden-defined, preserved verbatim):
//    0 = ok   1 = 1st write failed   2 = allow-write failed (NUEC1A only)
//    3 = readback SDO failed (or NUEC1 readback mismatch: see below)
//    4 = device reports bSettingError (NUEC1A only)
//    5 = readback SDO failed (NUEC1A only)   6 = readback value mismatch (NUEC1A only)
//    NUEC1 path reuses code 3 for BOTH "readback SDO failed" and (nothing --
//    NUEC1 has no distinct mismatch code; see golden .cpp:284-299) -- i.e.
//    NUEC1's own "value mismatch after write" case ALSO reports 3, unlike
//    NUEC1A's dedicated 6.  Preserved verbatim, not harmonized.
// ---------------------------------------------------------------------------
bool TMyNUEC1::WriteSettingValue(int iAmplifierNo,int iValue)
{
    bool bRet=true;
    RetWriteSettingValue[iAmplifierNo]=0;  //clear status before write
    (void)iValue;  // AI(W5-MyNUEC1-Translate) 20260710: only read inside the
                   // #if HAVE_PCI1203 guard below; silences -Wunused-parameter
                   // on the default (no vendor SDK) build path.
#if HAVE_PCI1203
    unsigned long Result;
    int InputValue=0;
    short InputValue2=0;
    bool bAllow=true;
    bool bSettingError;
    short iSettingValue;
    short iSettingValue2;
    //Index 7004, SubIndex 17-32
    //write Setting Value
    InputValue=iValue;
    InputValue2=iValue;
    if(NUEC_TYPE==eNUEC1)
    {
        Result=Acm_DevWriteSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &InputValue2);
        if(Result==SUCCESS)
        {
            Result=Acm_DevReadSDOData(uiDevhand, iRing,  iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSettingValue2);
            if(Result==SUCCESS)
            {
                if(InputValue2==iSettingValue2)
                {
                    bNeedDosetValue[iAmplifierNo]=false;
                    RetWriteSettingValue[iAmplifierNo]=0;
                }
                else
                {
                    RetWriteSettingValue[iAmplifierNo]=3;
                    bRet=false;
                }
            }
            else
            {
                RetWriteSettingValue[iAmplifierNo]=2;
                bRet=false;
            }
        }
        else
        {
            RetWriteSettingValue[iAmplifierNo]=1;
            bRet=false;
        }
    }
    else    //NUEC1A write sequence is more involved
    {
        Result=Acm_DevWriteSDOData(uiDevhand, iRing, iDesMACID, 0x7004, 17+iAmplifierNo,  ECAT_TYPE_I16, DataSize, &InputValue);
        if(Result==SUCCESS)
        {
            //allow write
            //Index 7004, SubIndex 1-16, 0:not allowed 1:allowed
            Result=Acm_DevWriteSDOData(uiDevhand, iRing,  iDesMACID, 0x7004, 1+iAmplifierNo,  ECAT_TYPE_BOOL, DataSize, &bAllow);
            if(Result==SUCCESS)
            {
                //confirm write had no error
                //Index 6004, SubIndex 17-32, False=no error True=error
                Result=Acm_DevReadSDOData(uiDevhand, iRing,  iDesMACID, 0x6004, 17+iAmplifierNo,  ECAT_TYPE_BOOL, DataSize, &bSettingError);
                if(Result==SUCCESS)
                {
                    if(bSettingError)
                    {
                        RetWriteSettingValue[iAmplifierNo]=4;
                        bRet=false;
                    }
                    else
                    {
                        Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, SetValue_Index+iAmplifierNo*0x0010, SetValue_SubIndex,  ECAT_TYPE_I16, DataSize, &iSettingValue);
                        if(Result==SUCCESS)
                        {
                            if(InputValue==iSettingValue)
                            {
                                bNeedDosetValue[iAmplifierNo]=false;
                                RetWriteSettingValue[iAmplifierNo]=0;
                            }
                            else
                            {
                                RetWriteSettingValue[iAmplifierNo]=6;
                                bRet=false;
                            }
                        }
                        else
                        {
                            RetWriteSettingValue[iAmplifierNo]=5;
                            bRet=false;
                        }
                    }
                }
                else
                {
                    RetWriteSettingValue[iAmplifierNo]=3;
                    bRet=false;
                }
            }
            else
            {
                RetWriteSettingValue[iAmplifierNo]=2;
                bRet=false;
            }
        }
        else
        {
            RetWriteSettingValue[iAmplifierNo]=1;
            bRet=false;
        }
    }
#else
    // No vendor EtherCAT SDK linked (HAVE_PCI1203 undefined): mirror golden's
    // own "1st write call failed" fallback (RetWriteSettingValue=1), the same
    // value BOTH NUEC_TYPE branches above already write when their first
    // Acm_DevWriteSDOData call doesn't return SUCCESS.
    RetWriteSettingValue[iAmplifierNo]=1;
    bRet=false;
#endif
    return bRet;
}

// ---------------------------------------------------------------------------
//  ClearError
//  golden EtherCAT/MyNUEC1.cpp:338-343
// ---------------------------------------------------------------------------
void TMyNUEC1::ClearError()
{
    ZeroMemory(bNeedDosetValue, sizeof(bNeedDosetValue));
    ZeroMemory(RetWriteSettingValue, sizeof(RetWriteSettingValue));
}

// ---------------------------------------------------------------------------
//  CheckAmplifierExist (one amplifier)   -- Sam 20230814: EtherCAT amplifier presence check
//  golden EtherCAT/MyNUEC1.cpp:345-355
// ---------------------------------------------------------------------------
bool TMyNUEC1::CheckAmplifierExist(int iAmplifierNo)
{
    bool bRet=false;
    (void)iAmplifierNo;  // AI(W5-MyNUEC1-Translate) 20260710: only read inside
                         // the #if HAVE_PCI1203 guard below; silences
                         // -Wunused-parameter on the default build path.
#if HAVE_PCI1203
    unsigned long Result;
    unsigned short iValue;

    Result=Acm_DevReadSDOData(uiDevhand, iRing, iDesMACID, 0x8000+iAmplifierNo*0x0010, 0x00,  ECAT_TYPE_U16, DataSize, &iValue);
    if(Result==SUCCESS && iValue==20)
        bRet=true;
#endif
    return bRet;
}

// ---------------------------------------------------------------------------
//  CheckAmplifierExist (all amplifiers, summary text)   -- Sam 20230814
//  golden EtherCAT/MyNUEC1.cpp:357-378
// ---------------------------------------------------------------------------
AnsiString TMyNUEC1::CheckAmplifierExist()
{
    AnsiString sRet="";
    int AmpCnt=0;
    if(iAmplifier>0)
    {
        for(int i=0; i<iAmplifier; i++)
        {
            if(CheckAmplifierExist(i))
                AmpCnt++;
        }

        if(iAmplifier==AmpCnt)
            sRet="OK";
        else
            sRet.printf("NU-EC1[%d] Amp is %d less than %d", iDesMACID, AmpCnt, iAmplifier);
    }
    else
    {
        sRet="OK";
    }
    return sRet;
}
