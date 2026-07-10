// =============================================================================
//  CanBus/cMyNUDN1.h  --  TMyNUDN1 (single NUDN1 DeviceNet station wrapper)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CanBus/cMyNUDN1.h (~140 lines)
//  Translation wave: W5-CanBus (leaf unit)
//  Translator: AI(W5-CanBus-Translate) 20260710
//
//  TMyNUDN1 wraps ONE NUDN1 sensor-amplifier station (ICP DAS FS-N12N style
//  fiber-optic amplifier) that sits behind a DNM100UD DeviceNet master board
//  (see sibling CanBus/cMyDNM100UD.h -- TMyDNM100UD_Card owns up to 16 of
//  these).  It reads/writes the station's DeviceNet "I/O Communication" poll
//  area (Monitor mode) and can send an explicit "SetValue" message to change
//  one amplifier channel's threshold.
//
//  Key changes vs. BCB6 original:
//    - `class PACKAGE TMyNUDN1 : public TComponent` -> plain `class TMyNUDN1`
//      (TComponent base dropped).  Cross-tree grep of every NUDN1Item[] use
//      site (CanBus/cMyDNM100UD.cpp, CCLink/MyCCLinkSensor.cpp) shows this
//      object is only ever `new`'d, `delete`'d, and used via its own methods/
//      fields -- never through ->Name, ->Owner, ->FreeNotification, VCL
//      streaming, or any other TComponent service.  The inheritance is
//      vestigial VCL boilerplate.  Same call the W4 HTMotor/TMyMotor
//      translation made (TMyMotor has no VCL base either).  vclcompat has no
//      TComponent shim; per the translation-unit rules we do not invent one
//      for an unused base.
//    - IMPORTANT GOLDEN GOTCHA (found while translating THIS file, not just
//      its cMyDNM100UD.cpp sibling -- flagged to the integrate agent):
//      golden cMyNUDN1.cpp's real ctor initializer is
//          TMyNUDN1::TMyNUDN1(...): TComponent(Owner)
//      but `Owner` is NEITHER a ctor parameter NOR a data member declared
//      anywhere in this class.  TComponent (VCL base) does expose a public
//      `Owner` property/member, so this compiles under BCB6 only because,
//      inside a base-class mem-initializer, an unqualified name is looked up
//      as an (implicit `this->`) inherited member of the class under
//      construction -- i.e. it reads `this->Owner`, which at that point in
//      construction is a NOT-YET-INITIALIZED inherited field.  That is
//      textbook UB (uninitialized read used as the AOwner argument to the
//      TComponent base ctor).  cMyDNM100UD.cpp's ctor has the exact same
//      `: TComponent(Owner)` pattern (that is the "unresolved Owner ctor bug"
//      this unit's task spec deferred cMyDNM100UD.cpp for).  Dropping the
//      TComponent base here (see previous bullet) makes the bug moot for
//      THIS file -- there is no base ctor argument to mis-supply -- but
//      whoever eventually translates cMyDNM100UD.cpp needs to make the same
//      call (or an equivalent fix) for its TMyDNM100UD_Card ctor.
//    - `byte`/`BYTE`/`WORD` from <windows.h> (same convention as MyLaneIo.h).
//    - `__fastcall` dropped (neutralized to nothing anyway).
//    - `__published: __property ... ={read=...}` (3 read-only properties)
//      translated to plain getter methods with the SAME NAMES as the golden
//      property identifiers (iGetActivedBoardNo/iGetDesMACID/iGetAmplifier),
//      matching the cpublic.h W0-TAIL __property->getter convention.  Golden
//      call sites read `->iGetDesMACID` (property, no parens); translated
//      call sites (in the still-deferred cMyDNM100UD.cpp) will need
//      `->iGetDesMACID()` (method call, with parens) -- noted for whoever
//      translates that file next.
//    - Ctor's 7th parameter is spelled `iAmplifier` in golden's header
//      declaration (same spelling as the private member) but `_iAmplifier`
//      in golden's .cpp definition -- a harmless prototype-only naming
//      mismatch (parameter names in a declaration need not match a later
//      definition in C++).  Normalized to `_iAmplifier` here for clarity;
//      no behavior change.
// =============================================================================
#ifndef cMyNUDN1H
#define cMyNUDN1H

#include "vclcompat/vcl_compat.h"   // __fastcall/__published neutralization (kept for parity with CanBus siblings)
#include <windows.h>                // byte / BYTE / WORD / DWORD

// ---------------------------------------------------------------------------
//  TMyNUDN1 -- one NUDN1 DeviceNet station (golden cMyNUDN1.h:5-...)
// ---------------------------------------------------------------------------
class TMyNUDN1
{
private:
    // Initail() 取得的資料 (settings captured at construction time)
    unsigned char   iActivedBoardNo;   // DNM100UD 卡號 (which DNM100UD board this station lives on)
    unsigned char   iDesMACID;         // NUDN1 站號 (DeviceNet MAC ID of this station)
    unsigned char   iType;             // IO Communication Type (ConType_* -- see DNM100API.h)
    unsigned short  iInputLen;         // 接收資料的長度 (poll input length, bytes)
    unsigned short  iOutputLen;        // 傳送資料的長度 (poll output length, bytes)
    unsigned short  iEPR;
    int             iAmplifier;        // 最多 16 個 (amplifier channel count, 1~16)

public:
    TMyNUDN1(unsigned char _iActivedBoardNo,
             unsigned char _iDesMACID,
             unsigned char _iType,
             unsigned short _iInputLen,
             unsigned short _iOutputLen,
             unsigned short _iEPR,
             int _iAmplifier);

    ~TMyNUDN1();

    void    Initail();
    int     iGetValue   (int iAmplifierNo);   // NUDN1 第幾個 Amplifier 的 Currect Value
    bool    bGetIO      (int iAmplifierNo);   // NUDN1 第幾個 Amplifier 的 IO (0~15)

    int     iSetIO_Value_Send(int iAmplifierNo, int iSetValue);  // NUDN1 設定第幾個 Amplifier 的 IO 閥值 (0~15)
    int     iSetIO_Value[16];   // NUDN1 設定閥值 (pending -1 = none, otherwise the value to send)
    int     iGetIO_Value[16];   // NUDN1 所有 Amplifier IO 閥值目前數值陣列

    // --- Use I/O Communication (pool) -------------------------------------
    // 讀取 NUDN1 IO 資料.  default:2, 0:Standard mode; 1:External input mode;
    // 2:Monitor mode; 3:Full mode; 4:Minimum mode  (golden only implements
    // mode 2 -- modes 0/1/3/4 are empty branches in golden, preserved as-is).
    int iReadInputData(int iOperatingMode);

    union InArea_Output
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
        } IO;
    };

    union InArea_Status
    {
        unsigned short iRaw;
        struct
        {
            unsigned char ErrorID                   :4;
            unsigned char ErrorCode                 :3;
            unsigned char UpdatingSensorSetting     :1;
            unsigned char SensorEEPROMBusy          :1;
            unsigned char SensorExternalInputBusy   :1;
            unsigned char SensorSettingError        :1;
            unsigned char reserved                  :1;
            unsigned char SensorReady               :1;
            unsigned char WarningStatus             :1;
            unsigned char ErrorStatus               :1;
        } Status;
    };

    // I/O Communication (pool) For NUDN1 Monitor mode
    union InArea_Monitor_Mode
    {
        unsigned char iRawItem[40];
        struct
        {
            InArea_Output   Output;
            InArea_Status   Status;
            unsigned short  reserved:16;
            InArea_Output   Property;
            unsigned short  Value01 :16;
            unsigned short  Value02 :16;
            unsigned short  Value03 :16;
            unsigned short  Value04 :16;
            unsigned short  Value05 :16;
            unsigned short  Value06 :16;
            unsigned short  Value07 :16;
            unsigned short  Value08 :16;
            unsigned short  Value09 :16;
            unsigned short  Value10 :16;
            unsigned short  Value11 :16;
            unsigned short  Value12 :16;
            unsigned short  Value13 :16;
            unsigned short  Value14 :16;
            unsigned short  Value15 :16;
            unsigned short  Value16 :16;
        } InArea;

        // NOTE: "Currect" (should be "Correct") is the golden spelling of both
        // members below -- preserved verbatim (not "fixed"), matching the
        // no-cleanup rule for faithful gaps/quirks.
        int CurrectValue(int iAmplifierNo)
        {
            if(iAmplifierNo==0)         return InArea.Value01;
            else if(iAmplifierNo==1)    return InArea.Value02;
            else if(iAmplifierNo==2)    return InArea.Value03;
            else if(iAmplifierNo==3)    return InArea.Value04;
            else if(iAmplifierNo==4)    return InArea.Value05;
            else if(iAmplifierNo==5)    return InArea.Value06;
            else if(iAmplifierNo==6)    return InArea.Value07;
            else if(iAmplifierNo==7)    return InArea.Value08;
            else if(iAmplifierNo==8)    return InArea.Value09;
            else if(iAmplifierNo==9)    return InArea.Value10;
            else if(iAmplifierNo==10)   return InArea.Value11;
            else if(iAmplifierNo==11)   return InArea.Value12;
            else if(iAmplifierNo==12)   return InArea.Value13;
            else if(iAmplifierNo==13)   return InArea.Value14;
            else if(iAmplifierNo==14)   return InArea.Value15;
            else                        return InArea.Value16;
        }

        bool CurrectOutput(int iAmplifierNo)
        {
            if(iAmplifierNo==0)         return (InArea.Output.IO.No00==1)?true:false;
            else if(iAmplifierNo==1)    return (InArea.Output.IO.No01==1)?true:false;
            else if(iAmplifierNo==2)    return (InArea.Output.IO.No02==1)?true:false;
            else if(iAmplifierNo==3)    return (InArea.Output.IO.No03==1)?true:false;
            else if(iAmplifierNo==4)    return (InArea.Output.IO.No04==1)?true:false;
            else if(iAmplifierNo==5)    return (InArea.Output.IO.No05==1)?true:false;
            else if(iAmplifierNo==6)    return (InArea.Output.IO.No06==1)?true:false;
            else if(iAmplifierNo==7)    return (InArea.Output.IO.No07==1)?true:false;
            else if(iAmplifierNo==8)    return (InArea.Output.IO.No08==1)?true:false;
            else if(iAmplifierNo==9)    return (InArea.Output.IO.No09==1)?true:false;
            else if(iAmplifierNo==10)   return (InArea.Output.IO.No10==1)?true:false;
            else if(iAmplifierNo==11)   return (InArea.Output.IO.No11==1)?true:false;
            else if(iAmplifierNo==12)   return (InArea.Output.IO.No12==1)?true:false;
            else if(iAmplifierNo==13)   return (InArea.Output.IO.No13==1)?true:false;
            else if(iAmplifierNo==14)   return (InArea.Output.IO.No14==1)?true:false;
            else                        return (InArea.Output.IO.No15==1)?true:false;
        }
    } InAreaMonitor;
    // --- Use I/O Communication (pool) --------------------------------------

    // golden __published __property (read-only) -> plain getter methods.
    // Same identifier as the golden property name; callers change `->Foo`
    // (property read) to `->Foo()` (method call).
    byte iGetActivedBoardNo() const { return iActivedBoardNo; }   // NUDN1 裝在哪張 DNM100UD
    byte iGetDesMACID()       const { return iDesMACID; }         // NUDN1 站號
    int  iGetAmplifier()      const { return iAmplifier; }        // NUDN1 的 Amplifier 數量(1~16)
};
#endif
