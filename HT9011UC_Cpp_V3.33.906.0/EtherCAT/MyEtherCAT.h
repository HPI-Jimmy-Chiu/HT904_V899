// =============================================================================
//  EtherCAT/MyEtherCAT.h  --  TMyEtherCAT (EtherCAT FS-N12 fleet controller)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/EtherCAT/MyEtherCAT.h (50 lines)
//                    + EtherCAT/MyEtherCAT.cpp (489 lines, bodies -- see .cpp)
//  Translation wave: W906-PT-W2 fieldbus (CanBus + EtherCAT group)
//  Translator: AI(W906-PT-W2) 20260807
//
//  ROLE: TMyEtherCAT owns up to MaxNUEC1(16) TMyNUEC1 (EtherCAT/MyNUEC1.h/.cpp,
//  ALREADY translated in an earlier wave -- see that unit) FS-N12 current-
//  sense-amplifier stations on EtherCAT Ring1 and polls them via a
//  switch(Task)-stepped state machine (tmrReadInputDataTimer), driven off a
//  30ms VCL TTimer in golden.  Two free functions living in the same golden
//  .cpp (CheckPCI_EtherCatState, OpenEtherCatMastCard) plus INSTALL_ETHETCAT()
//  manage the shared PCI1203 EtherCAT master card (global uiDevhand) this
//  class's NUEC1Item[] stations, the separately-owned CC-Link IO ring
//  (fCCLink), and the TOTAL_MOTOR EtherCAT motor axes (MOT[]) all multiplex
//  onto.  See the .cpp banner for the full GATE REGISTER.
//
//  Key changes vs. BCB6 original (faithful translation):
//    - `class PACKAGE TMyEtherCAT : public TComponent` -> plain `class
//      TMyEtherCAT` (TComponent base DROPPED).  golden's own ctor
//      initializer is `: TComponent(Owner)` (MyEtherCAT.cpp:18) where
//      `Owner` is NEITHER a ctor parameter NOR a data member of this class
//      -- the EXACT "Owner ctor bug" already diagnosed for the sibling
//      TMyNUEC1 (see EtherCAT/MyNUEC1.h banner, which explicitly flags
//      "MyEtherCAT.cpp:18 (TMyEtherCAT's own ctor, out of scope for this
//      unit) has the IDENTICAL pattern" as a note for whoever translates
//      this file next -- that's this unit).  Cross-tree grep (20260807) of
//      every real construction/use site of the golden `MyEtherCAT` global
//      confirms the object is only ever `new`'d and driven via its own
//      methods/fields, never through ->Owner/->Name/VCL streaming.  Dropping
//      the base moots the ctor bug (no base ctor argument left to
//      mis-supply) -- same call made for TMyNUEC1/TMyNUDN1/TMyDNM100UD_Card.
//    - `TTimer *tmrReadInputData` (30ms interval, OnTimer=tmrReadInputDataTimer)
//      -> `bool bTimerEnabled` substituting for `tmrReadInputData->Enabled`,
//      matching the established convention (CCLink/MyCCLink.h's
//      `bTimerEnabled` substituting for `Timer1->Enabled`).  Start()/Pause()
//      (golden's own public API, MyEtherCAT.cpp:58-65) toggle the flag;
//      tmrReadInputDataTimer(TObject *Sender=0) is exposed as a public
//      method for an external driver to pump every ~30ms (no vclcompat
//      TTimer primitive exists in this tree yet -- same gap CCLink/MyCCLink.h
//      already flagged, not invented here either).
//    - `__fastcall` dropped from tmrReadInputDataTimer's declaration AND its
//      .cpp definition together (kept in lockstep per vcl_compat.h's
//      __fastcall guardrail note -- both sides agree, so no ABI mismatch).
//    - golden `byte`/`BYTE`/`WORD`-flavoured members kept as the plain
//      unsigned char/unsigned short they are declared as (matches MyNUEC1.h).
//    - __published properties (HasError/ErrorMessage/SystemMessage/
//      InitialOK) flattened to plain public data members -- all four are
//      TRIVIAL read=field/write=field pass-throughs in golden (no
//      side-effecting setter), same flattening CCLink/MyCCLink.h used for
//      its own trivial properties.
// =============================================================================
#ifndef MyEtherCATH
#define MyEtherCATH

#include "vclcompat/vcl_compat.h"   // AnsiString, TObject, __fastcall/__property neutralization
#include "EtherCAT/MyNUEC1.h"       // TMyNUEC1

#ifndef MaxNUEC1
#define MaxNUEC1 16
#endif
#ifndef MaxFSN12
#define MaxFSN12 16
#endif

// ---------------------------------------------------------------------------
//  TMyEtherCAT -- fleet of up to MaxNUEC1 TMyNUEC1 stations on EtherCAT Ring1.
//  golden EtherCAT/MyEtherCAT.h:8-44
// ---------------------------------------------------------------------------
class TMyEtherCAT
{
protected:
    // AI(W906-PT-W2) 20260807: substitutes for golden's `TTimer
    // *tmrReadInputData` (Enabled/Interval=30ms) -- see header banner above.
    // No real periodic driver exists yet; tmrReadInputDataTimer() checks this
    // flag the way a disabled VCL TTimer would stop firing its event.
    bool bTimerEnabled;

    unsigned char  iRing;                      // I/O 使用 Ring1，Ring0 保留給馬達
    unsigned short iTotalDevices;              // 共接了幾組 NU-EC1
    unsigned short iDesMACIDList [MaxNUEC1];   // NU-EC1 各別 SlaveID
    unsigned char  iAmplifierList[MaxFSN12];   // NU-EC1 個別接的 Amplifier 數量

    int  iEtherCATTask;
    bool bReOpenPCI1203;

public:
    TMyEtherCAT(unsigned short _iTotalDevices, unsigned char *_iDesMACIDList, unsigned char *_iAmplifierList);
    ~TMyEtherCAT();

    void TaskClear();             // 初始化
    TMyNUEC1 *NUEC1Item[MaxNUEC1];
    void Start();
    void Pause();

    // golden __fastcall tmrReadInputDataTimer(TObject*Sender) -- was the
    // 30ms VCL TTimer OnTimer callback; now a public method for an external
    // driver to pump periodically (see bTimerEnabled note above). Sender
    // defaults to NULL/0 (unused inside the body, matches golden which never
    // reads Sender either).
    void tmrReadInputDataTimer(TObject *Sender = 0);

    // ---- flattened __published properties (see header banner note) --------
    bool       bHasErr;      // golden HasError      (read=bHasErr,  write=bHasErr,  default=false) 是否有錯誤
    AnsiString ErrMess;      // golden ErrorMessage  (read=ErrMess,  write=ErrMess)
    AnsiString SysMess;      // golden SystemMessage (read=SysMess,  write=SysMess)            錯誤訊息
    bool       bInitialOK;   // golden InitialOK     (read=bInitialOK, write=bInitialOK, default=false) Main Form 好了就 True
};
//---------------------------------------------------------------------------
extern TMyEtherCAT *MyEtherCAT;
extern int  CheckPCI_EtherCatState();     // Sam 20230811 : 新增 EtherCAT 檢查功能
extern int  OpenEtherCatMastCard();       // Sam 20230707 : EtherCAT Shuttle sensor
extern bool INSTALL_ETHETCAT();           // Sam 20230210 : 新增 VacuumUnit 通訊模組
#endif
