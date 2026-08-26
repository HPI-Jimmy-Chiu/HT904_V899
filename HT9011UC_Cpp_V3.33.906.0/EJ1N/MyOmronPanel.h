// =============================================================================
//  MyOmronPanel.h  --  TMyOmronPanel, a custom VCL panel component that shows
//                       one Omron-style temperature channel (PV/SV/AT/alarm
//                       text over a painted TImage, plus a draggable
//                       TGroupBox frame and a numeric-entry TEdit).
//
//  Faithful translation of golden EJ1N/MyOmronPanel.h (51 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 group "ej1n".  Third EJ1N/ file to land.
//
//  CONSUMERS -- READ THIS BEFORE TRUSTING ANY GATE BELOW.
//
//  AI(W906-PT-W2-integrate) 20260807: this banner previously opened with
//
//      "ZERO CONSUMERS: grepped the ENTIRE golden 906 tree (every .cpp/.h, not
//       just EJ1N/) for MyOmronPanel/TMyOmronPanel -- the only two hits are
//       this class's own .h/.cpp. Nothing anywhere ever constructs, extends, or
//       references this class ... every gate below carries NO behaviour-delta
//       risk to any other translated unit."
//
//  THAT IS FALSE and is corrected here.  A grep of the golden tree returns
//  SEVEN files, not two:
//      EJ1N/MyOmronPanel.{h,cpp}   (this class)
//      EJ1N/fDTME08.{h,cpp}        -- fDTME08.cpp:112-128 AddSingleView() does
//                                     `new TMyOmronPanel(this)` and then calls
//                                     SetSP / SetEvent / SetPV / SetRunStop /
//                                     SetDegree and sets GroupBox->Parent /
//                                     Caption / Left / Top
//      EJ1N/OmronEJ1N.{h,cpp}      -- OmronEJ1N.cpp:97-110 builds a
//                                     TOTAL_OMRON_UNIT x 4 grid of them
//      EJ1N/OmronThermo.cpp
//  and HT9045.bpr lists MyOmronPanel.cpp (:254), OmronEJ1N.cpp (:255,
//  FORMNAME="fOmron") and fDTME08.cpp (:513, FORMNAME="frmDTME08") as real
//  compiled members of the shipping project.  These are live consumers, not
//  dead code.
//
//  THE GATES BELOW ARE STILL CORRECT -- TMouseButton/TShiftState and the Canvas
//  painting surface genuinely have no port in this tree, and the golden bodies
//  are preserved verbatim in their #if 0 arms.  What was wrong was the
//  certification that they are RISK-FREE.  The real delta, stated plainly:
//  once fDTME08.cpp and OmronEJ1N.cpp are ported -- both are in EJ1N/, i.e. the
//  next wave to touch this area -- the Omron / DTME08 temperature-channel
//  display grid will render no on-screen digits (GATE 1) and cannot be dragged
//  (GATE 2), and the setpoint keypad will not open (GATE 3).  Retiring these
//  gates is a prerequisite for those two units, not an optional cleanup.
//
//  PROCESS NOTE: the original claim came from a grep that missed its own
//  matches.  An absence-claim is a measurement; state the command, and re-run
//  it at the END of the unit's work.  Three PT-W2 units shipped a false
//  absence-claim (this one, Monitor/MonitorTCPIP.cpp, Public/HTEditList.cpp).
//
//  ROLE: a self-contained custom TComponent (NOT a TControl -- it owns a
//  TGroupBox/TEdit/TCheckBox/TImage quartet as public members rather than
//  being one itself, exactly matching golden's own class shape).  8 public
//  Set* "display" methods paint text fields onto ImgOmronPanel's Canvas;
//  3 private mouse handlers implement click-and-drag repositioning of
//  GroupBox; SetSettingSV/SetSV/SetVisible additionally update real state
//  (dSettingSV/dSV, the edSV/GroupBox widgets) alongside their display call.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim vs GATED:
//   ACTIVE (faithful, verbatim data-flow -- the non-drawing part of every
//     golden function): ctor's widget allocation + dPv/dSettingSV init,
//     dtor, SetPV's `dPv=atof(...)` assignment, SetSettingSV's dSettingSV
//     assignment + edSV->Text write (vclcompat::TEdit::Text is real),
//     SetSV's dSV assignment, SetVisible's GroupBox->Visible write
//     (vclcompat::TControl::Visible is real), GetSettingSV/GetSV/GetPV/
//     SetAT(bool) (header inlines, all plain field/Checked access -- real).
//   GATED (#if 0, registered below): every Canvas/paint call (SetPV/
//     SetDegree/SetRunStop/SetAT/SetInputErr/SetEvent/SetSP/SetBG's drawing
//     halves), the ENTIRE widget layout/style block in the constructor
//     (Left/Top/Width/Height/Parent/Font/BorderStyle/AutoSize/ParentFont/
//     DragKind -- none of these properties exist on vclcompat::TControl by
//     DESIGN, see GATE (1)), the 3 mouse-drag handlers (GATE (2): their
//     parameter types TMouseButton/TShiftState do not exist anywhere in this
//     port), and setEditValueClick's virtual-keyboard popup (GATE (3):
//     fQwertyKey real since FW-QWKEY1; GATE (3) OPENED 20260824).
//
//  GATE REGISTER -- 3 gates, all grounded in an exhaustive tree-wide grep
//  (not guessed), all pre-existing-substrate-scope facts, not new gaps this
//  translation introduces:
//   (1) EVERY widget geometry/style property golden's ctor sets --
//       ->Left/->Top/->Width/->Height/->Parent/->Font->.../->BorderStyle/
//       ->AutoSize/->ParentFont/->DragKind (golden :17-24,26-30,42-54,57-63)
//       -- plus the whole TImage/TCanvas surface every Set* display method
//       paints through (->Canvas->Font/->Brush/->Pen/->Rectangle/->TextOutA/
//       ->TextWidth/->TextHeight, golden :32,105-198).  vclcompat/Controls.h
//       is EXPLICIT that its TControl base carries ONLY Visible/Enabled by
//       design (that header's own "DEFAULT-VALUE RULE" / "MEASURED PROPERTY
//       COVERAGE" sections enumerate exactly what is and is not modeled, and
//       geometry/Font/BorderStyle/DragKind are on the documented "NOT
//       covered" side); there is no vclcompat TImage/TCanvas type anywhere
//       in the tree at all (grepped: zero `class TImage`/`class TCanvas`
//       hits outside this comment). This is the SAME substrate boundary
//       vclcompat/LedCore.h's own banner describes for TALed's Paint/
//       CreateLedBitmap ("those move to the W7-C2 HDC renderer... which
//       doesn't exist yet") -- this file follows that exact precedent:
//       the framework-free ACTIVE arm keeps every state-carrying side effect
//       a real renderer would need to reproduce (dPv/dSettingSV/dSV, the
//       widgets' Text/Visible/Checked) and drops only the pixels.
//       BEHAVIOUR DELTA: none observable (zero consumers, see above); on a
//       real machine this panel would show no on-screen digits until a
//       future W7-C2-style HDC renderer is built to replay this state.
//   (2) GroupBox1MouseUp/GroupBox1MouseDown/GroupBox1MouseMove (golden
//       :15-17) -- their golden parameter types `TMouseButton`/
//       `TShiftState` (Controls.hpp) do not exist ANYWHERE in this port
//       (grepped tree-wide). Unlike gate (1), this is not just "the body
//       has no substrate" -- the SIGNATURE itself cannot be spelled in this
//       tree, so these 3 declarations are omitted from the ACTIVE class body
//       entirely (matching the tree's own common.h precedent for
//       `TSpeedButton *SButton` -- a member gated whole because its golden
//       TYPE has no port -- see that file's SPEEDBUTTON_PTR block). The
//       ctor's ->OnMouseUp/->OnMouseDown/->OnMouseMove wiring and
//       ->DragKind=dkDock assignment (golden :21-24) that would reference
//       them are folded into GATE (1)'s same #if 0 span. BEHAVIOUR DELTA:
//       the panel cannot be dragged by mouse in this build (it never could,
//       offline, with no window anyway -- zero consumers construct one).
//   (3) `fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0,
//       180);` (golden :219, setEditValueClick) -- fQwertyKey (the on-screen
//       numeric-entry keyboard form) is real since FW-QWKEY1 (fc08e09);
//       GATE (3) OPENED 20260824 (FW-QWKEY2), superseding the citation by
//       Automation/AGV_PortScan.h's own banner ("fQwertyKey virtual keyboard
//       popup (VCL input helper, W7)") for the identical golden idiom on a
//       different TEdit. N_INTEGER itself IS real (cmydef.h/.cpp) -- only
//       the form pointer was missing until 20260824. Call live below;
//       headless no-op until HTEdit GATE (6) wires the keyboard instance.
//
//  VCL/Borland conversions: `__fastcall` dropped from every method (this
//  header drops it from declarations; golden's own .cpp still shows it on
//  definitions per BCB6 convention -- irrelevant here, this is pure
//  declaration).  `PACKAGE` (golden :10, `class PACKAGE TMyOmronPanel`)
//  neutralized to nothing by vcl_compat.h.  `__published:` (golden :12)
//  mapped like every other ported form class: the one published member
//  (setEditValueClick) is declared `public` here (matches vcl_compat.h's own
//  `#define __published public`).  TColor: this port's shared
//  HT9045_W7C1_TCOLOR_SHIM (vclcompat/LedCore.h /
//  vclcompat/BtnPanelCore.h / vclcompat/TrayCore.h, `typedef int TColor` +
//  the clXxx constant set) is reused rather than re-declared -- but that
//  shared block does NOT carry `clGray` (golden's own ctor call
//  `SetBG(clGray, clGray)`, golden :33), so this file adds its OWN
//  `const TColor clGray` locally (0x00808080, the real Win32 COLORREF value
//  for VCL clGray) -- the one new symbol this header defines outside the
//  shared guard, and it cannot collide with a shared re-definition because
//  no existing vclcompat header declares it (grepped).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================

// BCB6 ORIGINAL header (mirrored as a comment for provenance):
//   //---------------------------------------------------------------------------
//   #ifndef MyOmronPanelH / #define MyOmronPanelH
//   #include <Classes.hpp>
//   using namespace std;
// =============================================================================

//---------------------------------------------------------------------------

#ifndef MyOmronPanelH
#define MyOmronPanelH

#include "vclcompat/vcl_compat.h"   // AnsiString; TComponent (via Comm.h, in the umbrella)
#include "vclcompat/Controls.h"     // TGroupBox / TCustomEdit+TEdit / TCheckBox (real substrate)
#include "vclcompat/LedCore.h"      // TColor + clBlack/clYellow (the shared
#include "vclcompat/ShiftState.h"   // AI(W906-FW-SIG-W17) 20260826
                                    //   HT9045_W7C1_TCOLOR_SHIM guard block --
                                    //   NOT brought to global scope by that
                                    //   header, so pulled in explicitly below)

using vclcompat::TColor;
using vclcompat::clBlack;
using vclcompat::clYellow;

//---------------------------------------------------------------------------
//AI(W906-PT-W2) 20260807: golden ctor calls `SetBG(clGray, clGray)` (golden
//  :33). This port's shared HT9045_W7C1_TCOLOR_SHIM block (LedCore.h /
//  BtnPanelCore.h / TrayCore.h) does not define clGray -- added here, the
//  real Win32 COLORREF for VCL's clGray, scoped to this header only.
const TColor clGray = TColor(0x00808080);

//---------------------------------------------------------------------------
class TMyOmronPanel : public TComponent
{
    // __published (golden :12) -- vcl_compat.h maps __published -> public.
    public:
        void setEditValueClick(TObject *Sender);
    private:    // User declarations
        //AI(W906-PT-W2) 20260807: GATE (2) -- golden `void __fastcall
        //  GroupBox1MouseUp/GroupBox1MouseDown/GroupBox1MouseMove(...)`
        //  (golden :15-17) omitted: their `TMouseButton`/`TShiftState`
        //  parameter types have no port anywhere in this tree (grepped).
        //  See this header's own banner GATE (2). Golden signatures kept
        //  here verbatim, in comment form, for provenance:
        //AI(W906-FW-SIG-W17) 20260826: GATE (2) 的前提已經不成立——
        //  `TMouseButton`/`TShiftState` 現在有 port（vclcompat/ShiftState.h，
        //  commit f184093）。三支因此改回**真正的宣告**，簽章與 golden 逐字相同；
        //  本體早在 PT-W2（20260807/20260811）就翻好了，只是當時這個 header 在
        //  該波的寫入邊界外，只能落成檔案層自由函式並在 .cpp 檔尾留下交接說明。
        //  ⚠ 注意 MouseMove 的簽章**少一個 TMouseButton**（golden :17 就是如此，
        //  VCL 的 OnMouseMove 本來就沒有 Button），三支不是同一個形狀。
        void GroupBox1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void GroupBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void GroupBox1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
        double dPv;//JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double dSettingSV;      //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double dSV;             //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
    public:
        TMyOmronPanel(TComponent* Owner);
        void SetBG        (TColor OutLine, TColor BackGround); //設定背景
        void SetPV        (AnsiString PV, TColor Color);       //顯示量測值
        void SetDegree    (AnsiString Degree, TColor Color);   //顯示量測單位
        void SetRunStop   (AnsiString RunStop, TColor Color);  //顯示目前運作模式
        void SetAT        (AnsiString AT, TColor Color);       //顯示AT是否啟動
        void SetInputErr  (AnsiString InputErr, TColor Color); //感溫線異常
        void SetEvent     (AnsiString Event, TColor Color);    //警報事件
        void SetSP        (AnsiString SP, TColor Color);       //目前設定值
        ~TMyOmronPanel();
        TGroupBox *GroupBox;
        //AI(W906-PT-W2) 20260807: GATE (1) -- golden `TImage *ImgOmronPanel;`
        //  (golden :33) omitted: there is no vclcompat TImage/TCanvas type
        //  anywhere in this port (grepped). See this header's own banner
        //  GATE (1). Every Set* method below that painted through it is
        //  reduced to its state-only remainder in the .cpp.
        TEdit     *edSV;        //設定值
        TCheckBox *cbEnableAT;
        int       iStartX;
        int       iStartY;
        int       iPort;
        double    fWeight;          //量測值
        double    fWeightDev;       //差異值
        void      SetVisible(bool bSW);  //Jimmychiu 20221123 add Groupbox visible item
        void      SetSettingSV(double dsv);             //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double    GetSettingSV(){return dSettingSV;}    //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        void      SetSV(double dsv);                    //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double    GetSV(){return dSV;}                  //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        int iMyTag;
        double GetPV(){return dPv;}
        void SetAT(bool bOn){cbEnableAT->Checked=bOn;}
};
#endif
