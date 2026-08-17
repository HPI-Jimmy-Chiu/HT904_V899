// ===========================================================================
//  ui/layout/CustomCtrlAttach.h -- attach MFC shells to template-created
//  HT9045.* windows + push D12 custom props
//  (docs/DESIGN_GA4_UI_ENGINES.md D-GA4-8).
//
//  AI(W906-GA4) 20260804: NEW FILE, hand-authored contract (main loop).
//
//  WHY THIS PASS EXISTS: HT9045_RegisterAllCustomClasses registers every
//  custom class with lpfnWndProc = DefWindowProc ("MFC subclasses later" --
//  its own banner). A dialog template therefore creates HT9045.ALed etc. as
//  inert holes; CLedCtrl's PreSubclassWindow is explicitly documented to run
//  on the SubclassWindow path. This pass is that "later":
//    for each layout row whose vcl_class is shell-backed
//        (TALed/TMyLed/TMyLedLane -> CLedCtrl,
//         TBtnPanel/TBtnPanelLane -> CBtnPanelCtrl,
//         TTMyTray/TTMyTray256    -> CTrayCtrl)
//      new shell -> SubclassWindow(hwnd) -> rt.AddShell (rt owns it)
//      push the row's D12 props into the shell/core:
//        LED: true/false/true_font/false_font colours (resolved ints pushed
//        with TColor semantics -- READ vclcompat/LedCore.h for the actual
//        setter signatures before writing a line, do not guess), LEDStyle /
//        Interval / Value / Blink via the shell forwarders or
//        Core()+SyncFromCore();
//        BtnPanel: True*/False* colours (caption already in the template);
//        Tray: XItem/YItem/Direct*/EdgeWidth/LineWidth from the custom bag.
//  Placeholder classes (StringGrid/Shape/Chart/...) are NOT attached --
//  visible inert holes during bring-up are the recorded intent
//  (RegisterCustomClasses.cpp banner).
// ===========================================================================
#ifndef HT9045_UI_LAYOUT_CUSTOMCTRLATTACH_H
#define HT9045_UI_LAYOUT_CUSTOMCTRLATTACH_H

#include "FormRuntime.h"

namespace ht9045_ui {

struct AttachStats {
    int leds;        // CLedCtrl shells attached
    int btnpanels;   // CBtnPanelCtrl shells attached
    int trays;       // CTrayCtrl shells attached
    int missing;     // shell-backed rows whose HWND was not found (should be 0)
};
AttachStats AttachCustomControls(FormRuntime& rt);

} // namespace ht9045_ui

#endif // HT9045_UI_LAYOUT_CUSTOMCTRLATTACH_H
