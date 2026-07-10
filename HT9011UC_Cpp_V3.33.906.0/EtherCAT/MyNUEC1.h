// =============================================================================
//  EtherCAT/MyNUEC1.h  --  TMyNUEC1 (FS-N12 EtherCAT current-amplifier) class
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/EtherCAT/MyNUEC1.h (150 lines)
//                    + EtherCAT/MyNUEC1.cpp (419 lines, bodies)
//  Translation wave: W5 comms (EtherCAT/MyNUEC1 comms-only unit)
//  Translator: AI(W5-MyNUEC1-Translate) 20260710
//
//  SCOPE: this is a "clean leaf" comms unit -- TMyNUEC1 talks directly to the
//  Advantech EtherCAT master (uiDevhand, cmydef.h) via Acm_Daq*/Acm_Dev* SDO
//  calls, with NO dependency on main.h/csystem/fMain/cContact god-stack.
//  Explicitly NOT translated by this unit (see MIGRATION_ROADMAP.md / report):
//    - EtherCAT/MyEtherCAT.cpp/.h (TMyEtherCAT) -- shares the SAME uiDevhand
//      master handle and re-initializes MOT[] motor axes as a side effect,
//      entangling it with the separately-deferred Motor EtherCAT axis driver
//      (Motor/myEthercatmotor.cpp, W4-part2).  Needs a joint decision with the
//      motor-HAL wave, not this unit.
//    - CanBus/cMyDNM100UD.cpp, CCLink/MyCCLinkSensor.cpp -- consumers of
//      TMyNUEC1::iGetRing/iGetDesMACID/iGetAmplifier (see property note below);
//      out of scope, flagged in this unit's report for the integrate agent.
//
//  AI(W5-MyNUEC1-Translate) 20260710: __property (BCB6, read-only, no setter)
//  -> plain getter METHODS (established project convention, see cpublic.h
//  "__property->getter" comments).  Golden call sites use the BARE property
//  name with NO parens (e.g. `NUEC1Item[i]->iGetRing`, MyEtherCAT.cpp:346 /
//  CanBus/cMyDNM100UD.cpp:304 / CCLink/MyCCLinkSensor.cpp:2784) -- when those
//  files are translated in a later wave they must add `()` at each call site.
//  Flagged explicitly in this unit's report (cross-unit risk).
//
//  AI(W5-MyNUEC1-Translate) 20260710: `class PACKAGE TMyNUEC1 : public
//  TComponent` -> plain `class TMyNUEC1` (TComponent base DROPPED).  Cross-
//  tree grep of every NUEC1Item[] use site (EtherCAT/MyEtherCAT.cpp,
//  CCLink/MyCCLinkSensor.cpp) confirms the object is only ever `new`'d
//  (MyEtherCAT.cpp:40), `delete`'d (:38), and driven via its own methods/
//  fields -- never through ->Owner/->Name/->FreeNotification/VCL streaming or
//  any other TComponent service.  The base is vestigial VCL boilerplate, same
//  call the sibling CanBus/cMyNUDN1.h translation made for its structurally-
//  identical "amplifier station wrapper" class (see that file's header
//  comment) and the W4 HTMotor/TMyMotor translation made earlier (no VCL base
//  either).  Aligning with that established in-wave precedent rather than
//  keeping a second, inconsistent VCL-base-retaining style for what is
//  otherwise the same class shape.
//    Dropping the base also moots golden's ctor bug (see below) -- there is
//  no base ctor argument left to mis-supply.  golden `: TComponent(Owner)`
//  (EtherCAT/MyNUEC1.cpp:15) references `Owner`, which is NEITHER a ctor
//  parameter NOR a data member declared anywhere in this class; it compiles
//  under BCB6 only because an unqualified name in a base mem-initializer is
//  looked up as an (implicit `this->`) INHERITED member of the class under
//  construction -- i.e. it reads the not-yet-initialized `TComponent::Owner`
//  property of the base subobject before that base's own ctor has run.
//  Textbook UB in ISO C++; in the real compiled BCB6 binary it is harmless
//  ONLY because Delphi/VCL's TObject::NewInstance zero-fills the whole
//  instance before any ctor body/init-list runs, so the read comes back
//  0/NULL -- functionally identical to `TComponent(NULL)`.  MyEtherCAT.cpp:18
//  (TMyEtherCAT's own ctor, out of scope for this unit) has the IDENTICAL
//  pattern.
// =============================================================================
#ifndef MyNUEC1H
#define MyNUEC1H

#include "vclcompat/vcl_compat.h"   // AnsiString, __fastcall/__property neutralization notes
#include <windows.h>                // byte / BYTE / ZeroMemory (same convention as MyLaneIo.h)

// MaxFSN12 (golden ref: EtherCAT/MyEtherCAT.h:7 `#define MaxFSN12 16`).
// Redeclared locally rather than #include "MyEtherCAT.h" -- MyEtherCAT.cpp/.h
// are explicitly NOT translated by this unit (see scope note above); pulling
// in that header would drag in TMyEtherCAT/uiDevhand-adjacent motor-axis
// coupling this unit must not touch.  Same pattern as IOBackend.h's local
// IO_MAXRing/IO_MAXIP/IO_MAXPort redeclaration (avoids pulling in the full
// MN200 motor chain into a pure-IO header).
#ifndef MaxFSN12
#define MaxFSN12 16
#endif

// ---------------------------------------------------------------------------
//  TMyNUEC1 -- one FS-N12 EtherCAT slave, up to 16 current-sense amplifiers.
//  golden EtherCAT/MyNUEC1.h:5-149
// ---------------------------------------------------------------------------
class TMyNUEC1
{
private:
    unsigned char  iRing;               // which PCI1203 Ring this NUEC1 lives on
    unsigned char  iDesMACID;           // NUEC1 slave station no. (11/12/13...)
    unsigned char  iAmplifier;          // number of enabled Amplifiers (max 16)
    unsigned short DataSize;            // fixed 128
    unsigned short CurValue_N;
    unsigned short CurValue_Index;
    unsigned short CurValue_SubIndex;
    unsigned short SetValue_Index;
    unsigned short SetValue_SubIndex;
    unsigned short IO_Index;
    unsigned short IO_SubIndex;
    int  iCurrectValue[16];             // [sic] golden spelling "Currect" kept verbatim
    int  iSettingValue[16];
    int  iMethod;                       // 0: old method (HT505S). 1: new (optimized read)

public:
    TMyNUEC1(unsigned char _iRing, unsigned char _iDesMACID, unsigned char _iAmplifier);
    ~TMyNUEC1();

    bool bNeedDosetValue[16];           // per-amplifier "write pending" flags
    int  iNeedSettingValue[16];         // per-amplifier pending write value

    bool ReadCurrentValue();                          // read all iAmplifier current values
    bool ReadCurrentValue(int iAmplifierNo);           // read one amplifier's current value
    int  iGetValue(int iAmplifierNo);                  // return cached current value

    bool ReadCurrentIO();                              // read the whole NUEC1's current IO word
    bool bGetIO(int iAmplifierNo);                      // return one amplifier's IO bit (0~15)

    bool ReadSettingValue();                            // read all iAmplifier setting values
    bool ReadSettingValue(int iAmplifierNo);            // read one amplifier's setting value
    int  iGetIO_Value(int iAmplifierNo);                // return cached setting value

    void SetSetIOValue(int iAmplifierNo, int iValue);   // queue a write
    bool WriteSettingValue();                            // flush all pending writes
    bool WriteSettingValue(int iAmplifierNo, int iValue); // write one amplifier's setting
    int  RetWriteSettingValue[16];                      // per-amplifier write error code (0=ok)
    void ClearError();

    bool       CheckAmplifierExist(int iAmplifierNo);   // Sam 20230814: amplifier presence check
    AnsiString CheckAmplifierExist();                    // Sam 20230814: same, all amplifiers, "OK"/error text

    // -------------------------------------------------------------------
    // AI(W5-MyNUEC1-Translate) 20260710: hardware wire-format bit-packed
    // unions, preserved EXACTLY per translation-unit spec -- do NOT "clean
    // up" into shift/mask decode helpers even though the layout looks
    // unusual.  golden EtherCAT/MyNUEC1.h:53-115.
    //
    // NOTE (bitfield packing, documented not "fixed"): `_IOWord`/`_IOChar`
    // alias a scalar (`iRaw`) with a run of `unsigned char NNN:1` bitfields.
    // C++ leaves cross-type bitfield/scalar union aliasing and bit
    // allocation order implementation-defined; this was written for
    // Borland bcc32 (x86, little-endian).  MinGW g++ on the same x86/x64
    // little-endian target packs runs of same-width bitfields identically
    // (LSB-first within each declared storage unit), so the layout carries
    // over faithfully on this project's toolchain; it is NOT portable to
    // an arbitrary compiler/ABI in the abstract.
    // -------------------------------------------------------------------
    union _IOWord
    {
        unsigned short iRaw;
        struct
        {
            unsigned char No00:1;
            unsigned char No01:1;
            unsigned char No02:1;
            unsigned char No03:1;
            unsigned char No04:1;
            unsigned char No05:1;
            unsigned char No06:1;
            unsigned char No07:1;
            unsigned char No08:1;
            unsigned char No09:1;
            unsigned char No10:1;
            unsigned char No11:1;
            unsigned char No12:1;
            unsigned char No13:1;
            unsigned char No14:1;
            unsigned char No15:1;
        }IO;
    };

    union _IOChar
    {
        unsigned char iRaw;
        struct
        {
            unsigned char No00:1;
            unsigned char No01:1;
            unsigned char No02:1;
            unsigned char No03:1;
            unsigned char No04:1;
            unsigned char No05:1;
            unsigned char No06:1;
            unsigned char No07:1;
        }IO;
    };

    union _IOArea
    {
        struct
        {
            _IOWord IOWord;
            _IOChar IOCharL;
            _IOChar IOCharH;
        }Area;

        // AI(W5-MyNUEC1-Translate) 20260710: golden has NO explicit bounds
        // check inside these two helpers -- the chained if/else-if ladder's
        // final bare `else` silently catches iAmplifierNo==15 AND any
        // out-of-range value (>=16 or negative) and returns the No15/No07
        // bit.  Preserved verbatim (see also bGetIO's off-by-one caller-side
        // bound check below, which lets iAmplifierNo==MaxFSN12(16) through
        // to here).  golden EtherCAT/MyNUEC1.h:96-128.
        bool CurrectOutput(int iAmplifierNo)
        {
            if(iAmplifierNo==0)         return (Area.IOWord.IO.No00==1)?true:false;
            else if(iAmplifierNo==1)    return (Area.IOWord.IO.No01==1)?true:false;
            else if(iAmplifierNo==2)    return (Area.IOWord.IO.No02==1)?true:false;
            else if(iAmplifierNo==3)    return (Area.IOWord.IO.No03==1)?true:false;
            else if(iAmplifierNo==4)    return (Area.IOWord.IO.No04==1)?true:false;
            else if(iAmplifierNo==5)    return (Area.IOWord.IO.No05==1)?true:false;
            else if(iAmplifierNo==6)    return (Area.IOWord.IO.No06==1)?true:false;
            else if(iAmplifierNo==7)    return (Area.IOWord.IO.No07==1)?true:false;
            else if(iAmplifierNo==8)    return (Area.IOWord.IO.No08==1)?true:false;
            else if(iAmplifierNo==9)    return (Area.IOWord.IO.No09==1)?true:false;
            else if(iAmplifierNo==10)   return (Area.IOWord.IO.No10==1)?true:false;
            else if(iAmplifierNo==11)   return (Area.IOWord.IO.No11==1)?true:false;
            else if(iAmplifierNo==12)   return (Area.IOWord.IO.No12==1)?true:false;
            else if(iAmplifierNo==13)   return (Area.IOWord.IO.No13==1)?true:false;
            else if(iAmplifierNo==14)   return (Area.IOWord.IO.No14==1)?true:false;
            else                        return (Area.IOWord.IO.No15==1)?true:false;
        };
        bool CurrectOutput2(int iAmplifierNo)
        {
            if(iAmplifierNo==0)         return (Area.IOCharL.IO.No00==1)?true:false;
            else if(iAmplifierNo==1)    return (Area.IOCharL.IO.No01==1)?true:false;
            else if(iAmplifierNo==2)    return (Area.IOCharL.IO.No02==1)?true:false;
            else if(iAmplifierNo==3)    return (Area.IOCharL.IO.No03==1)?true:false;
            else if(iAmplifierNo==4)    return (Area.IOCharL.IO.No04==1)?true:false;
            else if(iAmplifierNo==5)    return (Area.IOCharL.IO.No05==1)?true:false;
            else if(iAmplifierNo==6)    return (Area.IOCharL.IO.No06==1)?true:false;
            else if(iAmplifierNo==7)    return (Area.IOCharL.IO.No07==1)?true:false;
            else if(iAmplifierNo==8)    return (Area.IOCharH.IO.No00==1)?true:false;
            else if(iAmplifierNo==9)    return (Area.IOCharH.IO.No01==1)?true:false;
            else if(iAmplifierNo==10)   return (Area.IOCharH.IO.No02==1)?true:false;
            else if(iAmplifierNo==11)   return (Area.IOCharH.IO.No03==1)?true:false;
            else if(iAmplifierNo==12)   return (Area.IOCharH.IO.No04==1)?true:false;
            else if(iAmplifierNo==13)   return (Area.IOCharH.IO.No05==1)?true:false;
            else if(iAmplifierNo==14)   return (Area.IOCharH.IO.No06==1)?true:false;
            else                        return (Area.IOCharH.IO.No07==1)?true:false;
        };
    }IOArea;

    // -------------------------------------------------------------------
    // AI(W5-MyNUEC1-Translate) 20260710: __property (read-only) -> getter
    // methods.  golden `byte` (BCB6/Windows.h typedef, identical to
    // `unsigned char`) kept as `unsigned char` here to match the private
    // members' own declared type.  golden `default=0` is VCL streaming
    // metadata only (a .dfm-load-time fallback for a NEVER-set property
    // slot); has no runtime effect for a read-only property and is not
    // reproduced.  golden EtherCAT/MyNUEC1.h:145-148.
    //
    // CROSS-UNIT NOTE: golden callers use the bare property name with NO
    // parens (`->iGetRing`, not `->iGetRing()`) -- see MyEtherCAT.cpp:346,
    // CanBus/cMyDNM100UD.cpp:304/328/etc., CCLink/MyCCLinkSensor.cpp:2784-85.
    // Those 3 files are out of scope for this unit; when translated they
    // must add `()` at each call site.
    // -------------------------------------------------------------------
    unsigned char iGetRing()       const { return iRing; }
    unsigned char iGetDesMACID()   const { return iDesMACID; }
    unsigned char iGetAmplifier()  const { return iAmplifier; }
};

#endif
