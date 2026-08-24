// ===========================================================================
//  vclcompat/ScrollBar.h
//  Headless BCB6 TScrollBar compatibility shim (VCL StdCtrls.hpp).
//
//  AI(W906-FW-SETUP-D) 20260824: new file.
//
//  FIRST CONSUMER: forms/fSetup.h's TfSetup facade -- golden cSetUp.h:44
//  `TScrollBar *ScrollBar1;` (the test-mode selector: golden's ctor sets
//  ->Max per MachineTypeChoice, cSetUp.cpp:164-181, and CHSetError /
//  rgUseSuckModeClick / CheckShuttlePitch all dispatch on ->Position).
//
//  SCOPE (user ruling 20260824, deliberately narrow): a HEADLESS FIELD BAG.
//  Position/Min/Max/LargeChange/SmallChange/Kind plain-data fields plus
//  OnChange/OnScroll event-slot STORAGE -- nothing else. No scrolling is
//  simulated (scrolling is the browser's job in this tree's UI=web /
//  logic=C++ split), no rendering, no keyboard/mouse surface, no SetParams.
//
//  KNOWN, DELIBERATE SEMANTIC GAPS (all consequences of "field bag, don't
//  simulate" -- documented so no caller assumes otherwise):
//    * Real VCL SetPosition FIRES OnChange and CLAMPS Position into
//      [Min,Max]; assigning `->Position=` here does NEITHER (plain int).
//      Golden cascades of the shape "FormShow sets ScrollBar1->Position,
//      which fires ScrollBar1Change" therefore do NOT self-trigger offline;
//      the wave that ports such a caller must call the handler explicitly
//      (and say so), exactly like every other facade's dropped-VCL-event
//      wiring in this tree.
//    * Real VCL SetMin/SetMax re-clamp Position; these fields don't.
//    * The OnChange/OnScroll slots below are STORAGE ONLY -- nothing in this
//      class ever invokes them (there is no internal state change that could).
//      They exist (per the same user ruling) so the future web write-path
//      wiring has a real slot to bind, following the established
//      VCL-__closure -> std::function convention of vclcompat/ClientSocket.h
//      (:314-318) and vclcompat/Comm.h (:116-120). Contrast the stock-widget
//      posture in forms/fTemp_Set.h GATE(G-Delegate): stock vclcompat types
//      carry NO delegate slot at all -- this type is the deliberate,
//      user-ruled exception, not a drifting new idiom.
//
//  DEFAULTS are the real VCL TScrollBar design-time defaults (Min=0 Max=100
//  Position=0 SmallChange=1 LargeChange=1 Kind=sbHorizontal) -- the
//  StringGrid.h precedent for a NEW type with no prior bespoke stand-ins to
//  stay bug-compatible with (Controls.h's all-false/0 DEFAULT-VALUE RULE
//  protects pre-existing branch behaviour of REPLACED types; nothing replaced
//  here). Golden's own ctor overwrites Max anyway (cSetUp.cpp:164-181).
//
//  PROXY AUDIT (vclcompat proxy copy-assign trap, memory
//  ht9045-v906-vclcompat-proxy-copy-assign-trap): this class has ZERO
//  property proxies -- every field is a plain int/enum/std::function, so
//  proxy-to-proxy operator= silent no-op cannot arise here. Recorded so the
//  checklist item is visibly discharged, not skipped.
//
//  NAME COLLISION -- READ BEFORE ADDING A GLOBAL `using`
//  -------------------------------------------------------------------------
//  handlerlog.h:122 ALREADY defines a GLOBAL-namespace `class TScrollBar :
//  public TControl` (header-local stand-in, Position only, first consumer
//  TMyLog::Control_Log_Value), and handlerlog.cpp is one of forms/fSetup.h's
//  three pre-existing includers -- so this type is deliberately NOT brought
//  into the global namespace (same posture as vclcompat/StringGrid.h's
//  file-tail note, and unlike vclcompat/Controls.h's using-block). Consumers
//  spell it `vclcompat::TScrollBar`, fully qualified. A later consolidation
//  wave may retire handlerlog.h's local stand-in onto this one; that is a
//  cross-file edit outside this wave's boundary (handlerlog.h/.cpp are
//  pre-existing files).
//
//  BASE CLASS: vclcompat::TControl -- same TObject-rooted single-inheritance
//  chain as every stock widget in Controls.h, so the SS4-V2/SS9-R8 void*-
//  overload trap (SetSVDataPointer et al.) resolves to the TObject* overload,
//  and dynamic_cast between this and the stock set stays well-formed.
// ===========================================================================
#ifndef VCLCOMPAT_SCROLLBAR_H
#define VCLCOMPAT_SCROLLBAR_H

#include "vclcompat/Controls.h"   // TControl (-> TObject) base
#include <functional>

namespace vclcompat {

// golden StdCtrls.hpp TScrollBarKind (Delphi declaration order preserved).
enum TScrollBarKind { sbHorizontal, sbVertical };

// golden StdCtrls.hpp TScrollCode (Delphi declaration order preserved) --
// exists ONLY as the OnScroll slot's parameter type; nothing here produces
// one.
enum TScrollCode { scLineUp, scLineDown, scPageUp, scPageDown, scPosition,
                   scTrack, scTop, scBottom, scEndScroll };

// golden Classes.hpp TNotifyEvent / StdCtrls.hpp TScrollEvent, as
// std::function per the ClientSocket.h/Comm.h convention (assignable from a
// lambda/bound handler in translated code; empty by default).
typedef std::function<void(TObject* Sender)> TScrollBarNotifyEvent;
typedef std::function<void(TObject* Sender, TScrollCode ScrollCode,
                           int &ScrollPos)> TScrollBarScrollEvent;

class TScrollBar : public TControl {
public:
    int Position;          // golden ->Position (NO OnChange fire / clamp on
                           // assignment -- see file-head SEMANTIC GAPS)
    int Min;               // golden ->Min  (no re-clamp side effect)
    int Max;               // golden ->Max  (no re-clamp side effect)
    int LargeChange;       // golden ->LargeChange
    int SmallChange;       // golden ->SmallChange
    TScrollBarKind Kind;   // golden ->Kind

    // Event-slot STORAGE only -- never invoked by this class (see file head).
    TScrollBarNotifyEvent OnChange;
    TScrollBarScrollEvent OnScroll;

    TScrollBar()
        : Position(0), Min(0), Max(100), LargeChange(1), SmallChange(1),
          Kind(sbHorizontal) {}
    virtual ~TScrollBar() {}
};

} // namespace vclcompat

// Deliberately NOT brought into the global namespace -- handlerlog.h:122
// already owns global `::TScrollBar` and handlerlog.cpp includes
// forms/fSetup.h; see the NAME COLLISION note in the file head.

#endif // VCLCOMPAT_SCROLLBAR_H
