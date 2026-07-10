// =============================================================================
//  CCLink/MyCCLink.h  --  top-level CC-Link comm state machine (TMyCCLinkCtrl)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CCLink/MyCCLink.h
//  Translation wave: W5 (CCLink driver pair)
//  Translator: AI(W5-CCLink-Translate) 20260710
//
//  TMyCCLinkCtrl owns a TMyCCLinkBoard (MyCCLinkBoard.h/.cpp) and drives its
//  switch(Task)-stepped connect/read/write sequence via Timer1Timer(), used to
//  operate a Keyence Line-Sensor over CC-Link (golden file banner: "用CC Link
//  通訊來操作Keyence的Line Sensor").
//
//  Key changes vs. BCB6 original (faithful translation):
//    - `class PACKAGE TMyCCLinkCtrl : public TComponent` -> derives from
//      vclcompat::TComponent instead of VCL Classes::TComponent.
//    - `TTimer *Timer1` (30ms interval, OnTimer=Timer1Timer) -- NO vclcompat
//      TTimer primitive exists in this tree yet. Per scope, this unit does NOT
//      invent one; `bTimerEnabled` substitutes for `Timer1->Enabled` (gates
//      Timer1Timer() the way a disabled VCL TTimer would stop firing its
//      event), and `Timer1Timer(TObject *Sender=NULL)` is exposed as a public
//      method for an external driver to call every ~30ms (see report: flagged
//      as a dependency on a future vclcompat::TTimer shim or the W6.6 tick
//      spine, NOT built here).
//    - __published properties flattened (BCB6 syntax has no standard-C++
//      equivalent). ALL of TMyCCLinkCtrl's properties are TRIVIAL
//      read=field/write=field pass-throughs EXCEPT CCLinkStatus (whose write=
//      handler WriteCCLinkStatus has a real side effect). The trivial ones are
//      flattened to plain PUBLIC data members (same names, section-moved out
//      of `private:` -- internal method bodies in the .cpp already reference
//      them by bare field name, so this requires zero body rewrites).
//      CCLinkStatus's write side stays a public METHOD (WriteCCLinkStatus,
//      moved from protected to public: BCB6 properties can surface a
//      protected handler externally, and golden actually calls
//      `MyCCLink->CCLinkStatus=true;` from the deferred TfCCLink form at
//      MyCCLinkSensor.cpp:2542; standard C++ has no such indirection so the
//      method itself must be public). `iCCLinkStatus` is a plain public field
//      for the read side; direct writes to it bypass WriteCCLinkStatus's side
//      effects (bHasErr/ErrMess reset + iCCLinkTask=3 advance) -- callers that
//      want those side effects must call WriteCCLinkStatus(bool) instead.
//    - AI(W5-CCLink-Translate) 20260710: golden ctor does
//      `CCLinkBoard=new TMyCCLinkBoard(fCCLink);` (the GLOBAL TfCCLink form
//      pointer), NOT the ctor's own `Owner` parameter. At the one real
//      construction call site (golden MyCCLinkSensor.cpp:393
//      `MyCCLink=new TMyCCLinkCtrl(this)`), Owner IS ALWAYS fCCLink, so this
//      translation constructs CCLinkBoard with `Owner` instead -- behavior-
//      equivalent, and avoids depending on the undefined TfCCLink type here.
//      Flagged as a discrepancy from the literal golden text (see report).
//    - AI(W5-CCLink-Translate) 20260710: 2 touches into the not-yet-translated
//      TfCCLink FORM (golden CCLink/MyCCLinkSensor.h, W7-UI deferred) are
//      hosted here instead, on the nearest translated owner:
//        * bNewFile        stand-in for fCCLink->bNewFile      (MyCCLinkSensor.h:424)
//        * iAutoRetryCnt   stand-in for fCCLink->iAutoRetryCnt (MyCCLinkSensor.h:420)
//      A 3rd touch, fCCLink->SaveSetupFile() (a VCL-widget-reading form method,
//      MyCCLinkSensor.h:422), is gated `#if 0 // TODO(W7-UI)` in the .cpp (no
//      plain-field substitute is possible -- it reads dozens of form widgets).
// =============================================================================
#ifndef MyCCLinkH
#define MyCCLinkH
#include "MyCCLinkBoard.h"
//---------------------------------------------------------------------------
class TMyCCLinkCtrl : public TComponent
{
    protected:
        int iCCLinkTask;
        TQPF_Timer CCLinkTimer;

        // AI(W5-CCLink-Translate) 20260710: substitutes for golden's `TTimer *Timer1`
        // (Enabled/Interval=30ms) -- see header banner above. No real periodic driver exists
        // yet; Timer1Timer() checks this flag the way a disabled VCL TTimer would stop firing.
        bool bTimerEnabled;

    public:
        TMyCCLinkCtrl(TComponent* Owner);
        ~TMyCCLinkCtrl();

        bool bSetMultiValue;   // golden: ALREADY a plain public field (NOT a __published
                                // property) in the original -- asymmetric with SetValue/bSetValue
                                // below; preserved faithfully.
        TMyCCLinkBoard *CCLinkBoard;

        void DoSetValue(short sNu, short sSen, int Value);                  // set one sensor's threshold
        void DoSetMultiValue(short sNu, short sSen, int *Value, int Count); // set several sensors' thresholds

        // golden __fastcall Timer1Timer(TObject*Sender) -- was the 30ms VCL TTimer OnTimer
        // callback; now a public method for an external driver to pump periodically (see
        // bTimerEnabled note above). Sender defaults to NULL (unused inside the body, matches
        // golden which never reads Sender either).
        void Timer1Timer(TObject *Sender = 0);

        // golden write= handler for the CCLinkStatus property (was `protected`; made public --
        // see header banner note: BCB6 properties can surface a protected handler externally,
        // C++ has no such indirection and golden's only external caller is the deferred TfCCLink
        // form at MyCCLinkSensor.cpp:2542 `MyCCLink->CCLinkStatus=true;`).
        void WriteCCLinkStatus(bool Value);

        // ---- flattened __published properties (see header banner note) --------------------
        bool       bHasErr;        // golden HasError      (read=bHasErr,       write=bHasErr)
        bool       iCCLinkStatus;  // golden CCLinkStatus  READ side (read=iCCLinkStatus); WRITE
                                   // side is WriteCCLinkStatus() below (real side effect)
        long       lPath;         // golden Path          (read=lPath,          write=lPath, default 81)
        int        iNucl1No;      // golden Nucl1No       (read=iNucl1No,       write=iNucl1No, default 2)
        AnsiString ErrMess;       // golden ErrorMessage  (read=ErrMess,        write=ErrMess)
        AnsiString SysMess;       // golden SystemMessage (read=SysMess,        write=SysMess)
        bool       bSetValue;     // golden SetValue      (read=bSetValue,      write=bSetValue)
        bool       bInitialOK;    // golden InitialOK     (read=bInitialOK,     write=bInitialOK)
        bool       bReady;        // golden Ready         (read=bReady,         write=bReady)

        // AI(W5-CCLink-Translate) 20260710: stand-ins for the 2 fCCLink-form fields this
        // subsystem's own state machine touches (see header banner note above). Nothing in this
        // translated scope currently sets bNewFile=true (that trigger lives in the deferred
        // TfCCLink form, golden MyCCLinkSensor.cpp:720); iAutoRetryCnt is incremented/reset by
        // TMyCCLinkBoard::DoConnectBoard() via the reference parameter added to that method.
        bool bNewFile;
        int  iAutoRetryCnt;
};

#endif
