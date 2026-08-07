// =============================================================================
//  CanBus/cMyDNM100UD.h  --  TMyDNM100UD_Card (DNM100UD DeviceNet master board)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CanBus/cMyDNM100UD.h (105 lines)
//                    + CanBus/cMyDNM100UD.cpp (~585 lines, bodies -- see .cpp)
//  Translation wave: W906-PT-W2 fieldbus (CanBus + EtherCAT group)
//  Translator: AI(W906-PT-W2) 20260807
//
//  ROLE: TMyDNM100UD_Card owns up to 16 TMyNUDN1 (CanBus/cMyNUDN1.h/.cpp,
//  ALREADY translated in an earlier wave -- see that unit) NUDN1 DeviceNet
//  amplifier stations behind ONE 泓格/ICP DAS DNM100UD DeviceNet-master PCI
//  board, and polls them via a switch(Task)-stepped state machine
//  (tmrReadInputDataTimer), driven off a 30ms VCL TTimer in golden. It also
//  exposes an "Explicit Messaging" test API (iTestGetIO/iTestGetValue/
//  iTestGetIO_Value/iTestSetIO_Value) for ad-hoc single-amplifier reads/
//  writes outside the poll cycle.
//
//  Key changes vs. BCB6 original (faithful translation):
//    - `class PACKAGE TMyDNM100UD_Card : public TComponent` -> plain `class
//      TMyDNM100UD_Card` (TComponent base DROPPED).  golden's own ctor
//      initializer is `: TComponent(Owner)` (cMyDNM100UD.cpp:14) where
//      `Owner` is NEITHER a ctor parameter NOR a data member of this class
//      -- the EXACT "Owner ctor bug" already diagnosed for the sibling
//      TMyNUDN1 (see CanBus/cMyNUDN1.h banner, which explicitly names THIS
//      file's ctor as having "the exact same `: TComponent(Owner)` pattern"
//      and defers the actual fix call to "whoever eventually translates
//      cMyDNM100UD.cpp" -- that's this unit). Cross-tree grep (20260807) of
//      every real construction/use site of a `TMyDNM100UD_Card` confirms the
//      object is only ever `new`'d and driven via its own methods/fields,
//      never through ->Owner/->Name/VCL streaming. Dropping the base moots
//      the ctor bug (no base ctor argument left to mis-supply) -- same call
//      made for TMyNUEC1/TMyNUDN1/TMyEtherCAT.
//    - `TTimer *tmrReadInputData` (30ms interval, OnTimer=tmrReadInputDataTimer)
//      -> `bool bTimerEnabled` substituting for `tmrReadInputData->Enabled`,
//      matching the established convention (CCLink/MyCCLink.h's
//      `bTimerEnabled`, EtherCAT/MyEtherCAT.h's own `bTimerEnabled`).
//      tmrReadInputDataTimer(TObject *Sender=0) is exposed as a public
//      method for an external driver to pump every ~30ms (no vclcompat
//      TTimer primitive exists in this tree yet -- same gap flagged
//      repeatedly, not invented here either).
//    - `byte`/`BYTE`/`WORD` from <windows.h> kept (same convention as
//      MyLaneIo.h / CanBus/cMyNUDN1.h).
//    - `__fastcall` dropped throughout (neutralized to nothing anyway; kept
//      in lockstep between this header's declarations and the .cpp
//      definitions per vcl_compat.h's __fastcall guardrail note).
//    - __published properties flattened per the SAME rule the sibling
//      CanBus/cMyNUDN1.h and EtherCAT/MyNUEC1.h/MyEtherCAT.h classes used:
//        * HasError/InitialCardOK/ErrorMessage/SystemMessage/InitialOK are
//          trivial read=field/write=field pairs over fields that golden
//          declares `protected:` (bHasErr/bInitialCardOK/ErrMess/SysMess/
//          bInitialOK) -- i.e. the property's ONLY job in golden is to make
//          an otherwise-protected field externally reachable. Translated by
//          moving those 5 fields into the `public:` section under their
//          EXISTING names (no second "HasError"-spelled alias introduced --
//          no call site anywhere in this tree's translated scope reads the
//          property spelling, only the field spelling, so there is nothing
//          to preserve beyond public reachability).
//        * DLLVer/FirmwareVer are likewise trivial read=field/write=field
//          properties, but over fields golden ALREADY declares `public:`
//          (asDLLVer/asFirmwareVer) -- i.e. in golden BOTH `->asDLLVer` and
//          `->DLLVer` reach the SAME storage. Vestigial VCL alias; not
//          reproduced (asDLLVer/asFirmwareVer already satisfy every real use).
//        * `iTotalBoards` (read=TotalBoards, NO write=) is the one genuinely
//          READ-ONLY property. Translated to a public getter METHOD named
//          identically to the golden property (`iTotalBoards()`) per the
//          cpublic.h "__property->getter" convention used throughout this
//          tree (TMyNUEC1::iGetRing() etc.) -- golden call sites read the
//          bare property name with no parens; a future caller must add `()`.
//          `TotalBoards` itself stays a protected field (set internally by
//          the ctor's DNM100_TotalDNM100Board() lookup).
// =============================================================================
#ifndef cMyDNM100UDH
#define cMyDNM100UDH

#include "vclcompat/vcl_compat.h"   // AnsiString, TObject, __fastcall/__property neutralization
#include "CanBus/cMyNUDN1.h"       // TMyNUDN1
#include <windows.h>                // byte / BYTE / WORD / DWORD

//---------------------------------------------------------------------------
// KEYC NUDN1 -- golden cMyDNM100UD.h:6-16
//---------------------------------------------------------------------------
const WORD NUDN1ID      = 0x66;  // Class ID : NUDN1 ID

const byte SETR         = 0x0E;  // Service Code : Setting Read
const byte SETW         = 0x10;  // Service Code : Setting Write

// Instance ID N*10+1
const byte GETOUTPUT    = 0x68;  // Attribute ID : OutPut
const byte GETCURRECT   = 0x69;  // Attribute ID : Get Currect Value
// Instance ID N*10+2
const byte SETVALUE     = 0x65;  // Attribute ID : Setting Value

// ---------------------------------------------------------------------------
//  TMyDNM100UD_Card -- 泓格 DeviceNet Master PCI Board, up to 16 TMyNUDN1
//  stations.  golden CanBus/cMyDNM100UD.h:20-92
// ---------------------------------------------------------------------------
class TMyDNM100UD_Card
{
protected:
    // AI(W906-PT-W2) 20260807: substitutes for golden's `TTimer
    // *tmrReadInputData` (Enabled/Interval=30ms) -- see header banner above.
    // No real periodic driver exists yet; tmrReadInputDataTimer() checks
    // this flag the way a disabled VCL TTimer would stop firing its event.
    bool bTimerEnabled;

    // ---- Boards --------------------------------------------------------
    unsigned char iActivedBoardNo;          // 泓格 DNM100UD Board No.
    unsigned char TotalBoards;              // read-only externally via iTotalBoards() below
    unsigned char iBoardIDList[16];
    unsigned char iCurrectBaudRateType;
    unsigned char iBaudRateType;            // 0:500k ; 1:250k ; 2:125k  (bit/s)

    // ---- Devices ---------------------------------------------------------
    unsigned short iTotalDevices;
    unsigned char  iDesMACIDList [16];      // 所有 NUDN1 站別清單
    unsigned char  iAmplifierList[16];      // 所有 NUDN1 接的 FS-N12N 數量
    unsigned char  iType;                   // NUDN1 IO 通訊模式

    // ---- EX --------------------------------------------------------------
    int MyDNM100_SendExplicitMSG_W(BYTE DesMACID, BYTE ServiceID, WORD ClassID, WORD InstanceID, WORD DataLen, BYTE *DATA, WORD GetDataLen);
    bool bUseRespValueDelay;     // 使用 RespValueDelay 較舊版V260需要延遲,新版之後V270可以不用延遲
    bool bActivedBoard;
    int iDNM100UDTask;

public:
    TMyDNM100UD_Card(unsigned short _iTotalDevices, unsigned char *_iDesMACIDList, unsigned char *_iAmplifierList);
    ~TMyDNM100UD_Card();
    // 初始化
    int Initail();
    void TaskClear();

    TMyNUDN1 *NUDN1Item[16];
    AnsiString asDLLVer;
    AnsiString asFirmwareVer;
    bool bCanBusSeach;
    AnsiString asGetBaudRate();
    bool DNM100IsShutDownNeedPowerOffPC;    // DNM100 卡掛了需要重開電腦才會復活(Reset沒有用)
    int DNM100InitailCount;

    // golden __fastcall tmrReadInputDataTimer(TObject*Sender) -- was the
    // 30ms VCL TTimer OnTimer callback; now a public method for an external
    // driver to pump periodically (see bTimerEnabled note above). Sender
    // defaults to NULL/0 (unused inside the body, matches golden which never
    // reads Sender either).
    void tmrReadInputDataTimer(TObject *Sender = 0);

    // Use Explicit Messaging iAmplifierNo : 1~16
    //==>            iTestGetIO
    int iTestGetIO(BYTE DesMACID, byte iAmplifierNo);                        // Sensor 輸出的 IO (測試使用。讀取 IO 要等 I/O Communication 執行完)
    int iTestGetValue(BYTE DesMACID, byte iAmplifierNo);                     // Sensor 目前數值  (測試使用。讀取數值要等 I/O Communication 執行完)
    int iTestGetIO_Value(BYTE DesMACID, byte iAmplifierNo);                  // IO 閥值目前數值  (測試使用)
    int iTestSetIO_Value(BYTE DesMACID, byte iAmplifierNo, int iSetValue);   // IO 閥值設定      (測試使用)

    //....後面可依據功能需求增加
    //..
    //.
    //<==
    // Use Explicit Messaging

    AnsiString asSearchErrCodeMsg(int i);

    // ---- flattened __published properties (see header banner note) --------
    bool       bHasErr;         // golden HasError      (read=bHasErr,        write=bHasErr,        default=false) 是否有錯誤
    bool       bInitialCardOK;  // golden InitialCardOK (read=bInitialCardOK, write=bInitialCardOK,  default=false)
    AnsiString ErrMess;         // golden ErrorMessage  (read=ErrMess,        write=ErrMess)
    AnsiString SysMess;         // golden SystemMessage (read=SysMess,        write=SysMess)          錯誤訊息
    bool       bInitialOK;      // golden InitialOK     (read=bInitialOK,     write=bInitialOK, default=false) Main Form 好了就 True

    // golden read-only property `iTotalBoards` (read=TotalBoards, default=0)
    // -- see header banner note.
    unsigned char iTotalBoards() const { return TotalBoards; }
};
//---------------------------------------------------------------------------
union UNIONWORD
{
    int WHOLE_WORD;
    struct
    {
        char Lo_BYTE;
        char Hi_BYTE;
    } BYTES;
};
//---------------------------------------------------------------------------
#endif
