// =============================================================================
//  forms/fCleaning.h  --  non-VCL stand-in for golden's fCleaning form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtual destructor; TfLotInfoEdit now comes from
//  forms/FormWidgets.h.  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FCLEANING_H
#define FORMS_FCLEANING_H

#include "forms/FormWidgets.h"
// AI(W906-FW3-CLN1) 20260827: MouseDown handlers below carry golden's full
// TMouseButton/TShiftState signature -- both types now have a port
// (vclcompat/ShiftState.h, landed 20260826 by a sibling wave, see that
// file's own banner for the policy: "新翻的 handler 從此保留 golden 的完整
// 簽章"). Same include-and-signature shape as forms/fLotInfo.h:17,
// forms/fObserver.h:256, forms/fYieldMonitoring.h:214, etc.
#include "vclcompat/ShiftState.h"   // TMouseButton / TShiftState

// ===========================================================================
//  TfCleaningUpDown -- facade-local stand-in for golden `TUpDown *udDeviceCT`
//  (uCleaning.h:34). vclcompat has no TUpDown port anywhere (`grep -rn
//  "class TUpDown" vclcompat/` -- 0 hits, 20260827); compose over TControl
//  rather than fork vclcompat (forms/fDynamicTemp.h precedent: "compose,
//  don't fork").  Only Position/Min/Max/Increment are read by this wave's
//  delivered bodies (udDeviceCTChangingEx reads ->Position; edDevicePicesClick
//  reads ->Max/->Min).  Defaults seeded from golden uCleaning.dfm:1863-1875
//  (`Min = 0` / `Position = 0` explicit there; `Max`/`Increment` are NOT
//  overridden in the .dfm, so real Borland ComCtrls.hpp TUpDown's own
//  defaults apply -- Max=100, Increment=1).
// ===========================================================================
class TfCleaningUpDown : public vclcompat::TControl
{
public:
    int Position;
    int Min;
    int Max;
    int Increment;
    TfCleaningUpDown() : Position(0), Min(0), Max(100), Increment(1) {}
};

// ===========================================================================
//  W906-AutoCleanFoundation ADD (20260721): TfCleaning -- non-VCL stand-in
//  (golden AutoClean/uCleaning.h `class TfCleaning : public TForm`). Golden's
//  real class backs the 2,916-line AutoClean settings-panel VCL form
//  (uCleaning.cpp) -- entirely OUT OF SCOPE this wave, per the task brief;
//  this is ONLY a minimal facade stand-in for the handful of members the
//  translated AutoClean/AutoClean.cpp (+ this wave's Part D free functions)
//  actually deref, same "form as junk drawer" idiom as TfMain/TfLotInfo/
//  TfSortCT above.
// ===========================================================================
class TfCleaning
{
public:
    TfLotInfoEdit *edCleaningCount;   // [DATA] golden uCleaning.h:69 (TEdit*); SearchCleanNum (next-wave)
    int  iDeviceCount;                 // [DATA] golden uCleaning.h -- offline default 0
    bool bResetCleanCount;             // [DATA] golden uCleaning.h -- offline default false
    bool b1x2SiteAbClosePutDummy;      // [DATA] golden uCleaning.h -- ACTIVE: SetShuttleIcForSpecialMode (AutoClean.cpp) + CleanPadCountCanSupport2Arm

    // -----------------------------------------------------------------------
    //  AI(W906-FW3-CLN1) 20260827: FW3-CLN1 wave -- widget/data members added
    //  to back the 30 methods delivered this wave (see forms/fCleaning.cpp for
    //  the full per-method translation banner). Every name matches golden
    //  uCleaning.h's own component/member name verbatim, per the task's
    //  standing rule: the 33 #if 0-gated `fCleaning->` call sites elsewhere in
    //  the tree (ProductionInfo/uPAT_Function.cpp, SECSGEM/uHGemHT9045_EC.cpp,
    //  SECSGEM/uHGemHT9045_SV.cpp, forms/fLotInfo.cpp, Command.cpp) must find a
    //  matching name+type here whenever they are eventually un-gated.
    //  edPinSingleGf/edPinSingleN/OutArmSpeed are three of those exact 33 names.
    // -----------------------------------------------------------------------
    TfLotInfoEdit *edPinSingleGf;      // [DATA] golden uCleaning.h:53 (TEdit*) -- ShowTranGfToN source
    TfLotInfoEdit *edPinSingleN;       // [DATA] golden uCleaning.h:54 (TEdit*) -- ShowTranGfToN target
    TfLotInfoEdit *XCT1;               // [DATA] golden uCleaning.h:204 (TEdit*) -- XCT1Change
    TfLotInfoEdit *XCT2;               // [DATA] golden uCleaning.h:221 (TEdit*) -- XCT1Change
    TfLotInfoEdit *OutArmSpeed;        // [DATA] golden uCleaning.h:41 (TEdit*) -- SetArmCaption (Hint line GATEd, see .cpp)
    TLabel        *lblInArm;           // [DATA] golden uCleaning.h:37 (TLabel*) -- SetArmCaption
    TLabel        *lblInArmZ;          // [DATA] golden uCleaning.h:40 (TLabel*) -- SetArmCaption
    TPageControl  *pgCleanType;        // [DATA] golden uCleaning.h:194 (TPageControl*) -- XCT1Change
    TfCleaningUpDown *udDeviceCT;      // [DATA] golden uCleaning.h:34 (TUpDown*) -- udDeviceCTChangingEx/edDevicePicesClick
    int  iPosTemp;                     // [DATA] golden uCleaning.h:338 (public int) -- udDeviceCTChangingEx
    bool b12SiteRun2x4;                // [DATA] golden uCleaning.h:360 (public bool) -- XCT1Change

    // -----------------------------------------------------------------------
    //  AI(W906-FW3-CLN2) 20260827: the three functions FW3-CLN1 had already
    //  read and cleared but forgot to write out (see docs/DEVLOG.md 20260827
    //  XIV -- they were disclosed as oversights, NOT as exclusions).
    //  edAlarmCount is one of the 33 names reserved by the #if 0-gated
    //  `fCleaning->` call sites (Command.cpp:17382, ProductionInfo/
    //  uPAT_Function.cpp:1967,1998), so the name and type must match golden.
    // -----------------------------------------------------------------------
    TfLotInfoEdit *edAlarmCount;       // [DATA] golden uCleaning.h:35 (TEdit*) -- edCleanCountClick range max
    TSpeedButton  *sbCleanExit;        // [DATA] golden uCleaning.h:166 (TSpeedButton*) -- sbCleanExitClick

    TfCleaning();
    virtual ~TfCleaning() {}

    // golden inherits Close() from TForm; this facade derives from nothing that
    // has one. Offline no-op, exactly the shape used by forms/fQwertyKey.h:366
    // and fTemp_Set.h before it -- the form is never shown in this build, so
    // there is no window to close, and a no-op is the faithful offline answer
    // rather than a guess about what closing should mean.
    void Close() {}

    // -- FW3-CLN1: pure display / UI-state methods, zero write-path, zero
    //    ht9045_sm link edge (see forms/fCleaning.cpp banner for the full
    //    per-method dependency audit) -----------------------------------------
    void ShowTranGfToN();                                          // golden :1676-1683
    void SetArmCaption();                                          // golden :55-63 (Hint line GATEd)
    void rgKitPositionClick(TObject *Sender);                      // golden :1886-1889
    void XCT1Change(TObject *Sender);                              // golden :2119-2191
    void udDeviceCTChangingEx();                                   // golden :2242-2246 (DEVIATION: all 4 params dropped, none read -- see .cpp)
    void XST1KeyPress(TObject *Sender, char &Key);                 // golden :1874-1878
    void XCT1KeyPress(TObject *Sender, char &Key);                 // golden :1880-1884
    void edtLowYieldLimitKeyPress(TObject *Sender, char &Key);     // golden :2264-2269
    void ResetSmartAutoClean();                                    // golden :2736-2745

    // -- FW3-CLN1: fQwertyKey->ShowQwertyKey passthrough handlers (22) --------
    void edtBufferKitLTXClick(TObject *Sender);                    // golden :2574-2577
    void edtHotplatePickOffsetClick(TObject *Sender);              // golden :2579-2582
    void edACContactCleanHeightClick(TObject *Sender);             // golden :2584-2594
    void edtLowYieldLimitClick(TObject *Sender);                   // golden :2596-2599
    void edtLowYieldCountClick(TObject *Sender);                   // golden :2601-2604
    void XST1Click(TObject *Sender);                               // golden :2606-2609
    void XCT1Click(TObject *Sender);                               // golden :2611-2614
    void edACContactShiftHeightClick(TObject *Sender);             // golden :2616-2619
    void edDropOffset1Click(TObject *Sender);                      // golden :2621-2624
    void edDevicePicesClick(TObject *Sender);                      // golden :2626-2629
    void edAutoCleanAirForce_KgClick(TObject *Sender);             // golden :2631-2634
    void edAutoCleanAirForce_NClick(TObject *Sender);              // golden :2636-2639
    void edtACSmartClick(TObject *Sender);                         // golden :2641-2644
    void edTimeCTClick(TObject *Sender);                           // golden :2859-2862
    void edtIndexVacuumClick(TObject *Sender);                     // golden :2864-2867
    void edACInitalContactCountClick(TObject *Sender);             // golden :2874-2877
    void edAdaptiveIntervalMaxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);   // golden :2817-2821
    void edAdaptiveIntervalMinMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);   // golden :2823-2827
    void edAdaptiveIntervalAdjMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);   // golden :2829-2833
    void edACSmartCTFMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);            // golden :2835-2839
    void edtACSmart_ContactTimeMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);  // golden :2841-2846
    void edtACSmartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);              // golden :2848-2852

    // -- FW3-CLN2 (20260827): CLN1's three disclosed oversights ---------------
    void YCT1Change(TObject *Sender);                              // golden :1891-1901
    void sbCleanExitClick(TObject *Sender);                        // golden :1868-1872
    void edCleanCountClick(TObject *Sender);                       // golden :2080-2083
};
extern TfCleaning *fCleaning;

#endif // FORMS_FCLEANING_H
