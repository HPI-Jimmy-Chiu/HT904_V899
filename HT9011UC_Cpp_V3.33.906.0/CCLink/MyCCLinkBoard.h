// =============================================================================
//  CCLink/MyCCLinkBoard.h  --  Mitsubishi CC-Link master-board driver wrapper
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CCLink/MyCCLinkBoard.h
//  Translation wave: W5 (CCLink driver pair)
//  Translator: AI(W5-CCLink-Translate) 20260710
//
//  TMyCCLinkBoard wraps the Mitsubishi "Mdfunc" CC-Link master board DLL
//  (CCLink/Mdfunc.h, READ-ONLY vendor/definition header -- never modified) and
//  exposes a switch(Task)-stepped polling API (DoConnectBoard/DoReadCurrValue/
//  DoReadSetValue/DoReadOnOff/DoCheckWr0000/DoSetValue/DoSetMultiValue) that
//  TMyCCLinkCtrl (MyCCLink.h/.cpp) pumps every tick.
//
//  Key changes vs. BCB6 original (faithful translation):
//    - `PACKAGE`/`__fastcall` neutralized via vclcompat/vcl_compat.h (was <vcl.h>
//      through MachineDefine.h).
//    - `class PACKAGE TMyCCLinkBoard : public TComponent` -> derives from
//      vclcompat::TComponent (Comm.h) instead of VCL Classes::TComponent; no
//      other VCL widget/form coupling exists in this class (confirmed by recon:
//      zero TForm/TControl/TEdit/... references in MyCCLinkBoard.cpp).
//    - __published properties (BCB6 syntax has no standard-C++ equivalent) are
//      FLATTENED per-property:
//        * Status {read=CheckBoardStatus, write=bStatus} -- DEAD/UNUSED golden
//          quirk: CheckBoardStatus() returns void, not bool, so this property's
//          own declaration is a pre-existing BCB6 type mismatch. Grep across the
//          FULL golden tree confirms `->Status` (the property) is never read
//          anywhere (CheckBoardStatus() the METHOD is called directly instead,
//          from DoConnectBoard()). Preserved as a documented dead/broken
//          artifact; NOT exposed as a callable property-equivalent here since
//          there is no real call site to translate faithfully.
//        * HasError/ErrorMessage/ConnectModuleNo/Path {read=field, write=field}
//          (trivial pass-through, no side effect) -- flattened to plain PUBLIC
//          data members (bHasErr / ErrMess / iConnectModule / lPath), same
//          names, moved out of `private:` into `public:`. Internal method
//          bodies already reference these by bare field name throughout
//          MyCCLinkBoard.cpp, so this section-move requires zero internal
//          rewrites.
//        * SensorKeyLock {read=bKeyLock, write=SetSensorKeyLock} -- write goes
//          through a method with (currently no-op, commented-out) side effect.
//          golden declares SetSensorKeyLock as `protected`, but BCB6 properties
//          can surface a protected read/write handler as an externally callable
//          member (the property itself is __published i.e. public); standard
//          C++ has no such indirection, so SetSensorKeyLock is made PUBLIC here
//          (the only way TMyCCLinkCtrl -- a different class -- can reach it,
//          exactly mirroring golden's actual cross-class call
//          `CCLinkBoard->SensorKeyLock=true;` in MyCCLink.cpp). bKeyLock is a
//          plain public field for the read side.
//    - AI(W5-CCLink-Translate) 20260710: golden `TMyCCLinkBoard::DoSetMultiValue()`
//      case 5 (golden .cpp:701) reads its loop bound from `fCCLink->iAllSetCount`,
//      a field on the not-yet-translated TfCCLink form (golden
//      CCLink/MyCCLinkSensor.h:421, deferred W7-UI). The ONE real call site
//      (golden MyCCLinkSensor.cpp:776) always passes that SAME value as the
//      `Count` argument to `TMyCCLinkCtrl::DoSetMultiValue(...)`, so behavior is
//      preserved by threading Count through as a member here (iAllSetCount,
//      set via SetAllSetCount()) instead of reaching into the deferred form.
//      This is a discrepancy from the literal golden text -- see report.
//    - Vendor Mdfunc.h calls (mdOpen/mdClose/mdBdRst/mdSend/mdReceive/mdDevSet/
//      mdDevRst) are NOT declared in this header; they are wrapped by small
//      static free functions local to MyCCLinkBoard.cpp, gated `#if HAVE_CCLINK`
//      (default OFF), mirroring IOBackend.cpp's HAVE_MN200/HAVE_PCI1203/HAVE_MNET
//      pattern. See MyCCLinkBoard.cpp for the gate.
// =============================================================================
#ifndef MyCCLinkBoardH
#define MyCCLinkBoardH

#include "vclcompat/vcl_compat.h"   // neutralize PACKAGE/__fastcall/__published; AnsiString/TObject
#include "myTimer.h"                // TQPF_Timer (already translated, W0-TAIL)

//---------------------------------------------------------------------------
typedef struct
{
    bool  bHasInstall;       // is this NU-CL1 module installed
    bool  bOnOff[16];        // per-sensor on/off status
    int   iCurrValue[16];    // current reading value
    int   iSetValue[16];     // configured threshold value
    int   iSensorNo;         // how many sensors this module controls (16/12/4/9/0)
    short wwStart;
    short ryStart;
} MyCCLinkNCUL1;
//---------------------------------------------------------------------------
class TMyCCLinkBoard : public TComponent
{
    private:
        // AI(W5-CCLink-Translate) 20260710: private member `iSensorNo` (below) is a
        // DIFFERENT storage location than the per-module struct field
        // `MyCCLinkNCUL1::iSensorNo` above -- a genuine golden name collision
        // (class member vs. struct member, same identifier, different meaning).
        // The class member is written ONLY by GetSensorNo() (always set to the
        // dead literal 49, see .cpp) and read ONLY by DoReadSetValue()'s loop
        // bound; the struct member holds the real per-module sensor count
        // (16/12/4/9/0). Preserved verbatim -- not unified/renamed.
        int iSensorNo;

        bool bStatus;         // backs the DEAD Status property (see header note above)
        unsigned char iData[32];   // scratch receive buffer
        bool MemoryNotEnough; // Steven 20140330: low system memory working-set recovery flag

        // AI(W5-CCLink-Translate) 20260710: substitutes for golden's
        // fCCLink->iAllSetCount read (golden .cpp:701); see class-header note above.
        int iAllSetCount;

    protected:  // User declarations
        int   Result;
        short StaNo;
        short NetNo;
        unsigned char Data[32];
        TQPF_Timer CCLinkDelay;

        int  DoCCLinkSWQuest(int iDevNo);
        bool DoRYOnOff(int Index, int iRyDevNo, bool bOnOff);   // set/reset the bit device at iRyDevNo
        bool DoWriteWW(int Index, short sWwDevNo, short Value);
        int  DoReadWr02(int Index);                             // read

        bool ConnectBoard();         // open the connection
        void DisConnectBoard();      // close the connection
        int  GetSensorNo(int Value); // how many sensors module #Value has installed

        int CCLinkHexStrToInt(AnsiString str);
        void CheckBoardStatus();      // check connection status (DEAD Status property's
                                       // read= target; called directly by DoConnectBoard(),
                                       // never via a property expression -- see header note)
        void SetProcessMemory();      // Steven 20140330: low system memory working-set recovery
        AnsiString CmdResponseList[12];
        AnsiString MyErrMes(int iDevNo, int iResult);   // error text for a given device/result code

    public:
        TMyCCLinkBoard(TComponent* Owner);
        ~TMyCCLinkBoard();

        int  iCCLinkBoardTask;
        int  iReadSetValueTask;
        int  iDoSetValueTask;
        int  iDoSetMultiValueTask;

        // AI(W5-CCLink-Translate) 20260710: +AutoRetryCnt reference parameter.
        // Golden `TMyCCLinkBoard::DoConnectBoard()` (no params) directly touches
        // `fCCLink->iAutoRetryCnt` (golden .cpp:243/248) -- a counter on the
        // not-yet-translated TfCCLink form (golden MyCCLinkSensor.h:420, read by
        // deferred W7-UI code at MyCCLinkSensor.cpp:1112). TMyCCLinkBoard has no
        // pointer back to its owning TMyCCLinkCtrl/form, so the counter is
        // threaded through as a caller-owned reference instead of reaching for an
        // undefined global. TMyCCLinkCtrl::Timer1Timer (the only caller) passes
        // its OWN iAutoRetryCnt member (see MyCCLink.h) -- the nearest translated
        // stand-in for the golden form-level counter.
        bool DoConnectBoard(int &AutoRetryCnt);
        bool DoReadCurrValue();
        bool DoReadSetValue();
        bool DoReadOnOff();
        bool DoCheckWr0000();
        bool DoSetValue();          // set a single sensor's threshold
        bool DoSetMultiValue();     // set a whole row of sensors
        void ResetBoard();          // reset the board

        short sSetNu;               // NU-CL1 module # to be configured
        short sSetSen;              // sensor # to be configured
        int   iSetValue;            // value to be set
        int   iSetMultiValue[16];   // values to be set (multi)
        void ClearData();
        MyCCLinkNCUL1 MyNCUL[10];

        // AI(W5-CCLink-Translate) 20260710: setter for iAllSetCount (see class-header
        // note + private member above). Called by TMyCCLinkCtrl::DoSetMultiValue.
        void SetAllSetCount(int Count) { iAllSetCount = Count; }

        // ---- flattened __published properties (see header note above) --------
        bool       bHasErr;         // golden HasError        (read=bHasErr, write=bHasErr)
        AnsiString ErrMess;         // golden ErrorMessage     (read=ErrMess, write=ErrMess)
        int        iConnectModule;  // golden ConnectModuleNo  (read=iConnectModule, write=iConnectModule)
        long       lPath;           // golden Path             (read=lPath, write=lPath, default 81)
        bool       bKeyLock;        // golden SensorKeyLock read side (read=bKeyLock)
        void SetSensorKeyLock(bool Value);   // golden SensorKeyLock write side (was protected;
                                              // made public -- see header note above)
};

#endif
