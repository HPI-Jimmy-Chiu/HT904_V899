// ===========================================================================
//  vclcompat/TrayCore.h
//  Framework-free state/geometry core for the "Tray" custom-control family
//  (D5: lives in its OWN file, NOT in vclcompat/Controls.h -- these are
//  behaviour cores, not value-holder stand-ins).
//
//  Golden sources (D:\HT9045\elec\myvcl\, cp950/big5-decoded, verified
//  D:\HT9045 §4-V3): HTray.h/.cpp (TTMyTray : TCustomControl, 517+114 lines,
//  246 .dfm instances) and HTray256.h/.cpp (TTMyTray256 : TCustomControl,
//  389+92 lines, 0 .dfm instances but 7 dynamic `new` sites --
//  cBinSel.cpp:129-132/:232-235, uYieldMonitoring.{cpp,h} -- .dfm histograms
//  cannot see dynamically-created controls, see §4-V12 / KNOWLEDGE gotcha
//  #13).
//
//  SCOPE: this header carries every piece of TTMyTray/TTMyTray256 STATE and
//  GEOMETRY (cell grid, colour map, visibility, block layout, hit-testing).
//  It deliberately excludes anything that touches a VCL Canvas (DrawTray,
//  DrawSingleIC, DrawLine, WndProc, CreateWnd) -- that half moves to the
//  W7-C2 HDC renderer (vclcompat/render/TrayRender.*), which will consume
//  this core's public state directly (CellRect/GetCellData/GetCellText/
//  ColorMap/...) instead of duplicating any of this logic.
//
//  AI(W906-W7-C1) 20260728: promoted from the scratchpad prototype
//  (...\scratchpad\b2\traycore.h/.cpp, 30/30 passing) -- std::string swapped
//  for vclcompat::AnsiString per this wave's brief, full public surface
//  restored to match golden (SetBlockYItem/SetXBlockWidth/SetYBlockWidth/
//  SetLineWidth/SetEdgeWidth/SetDirectWidth/SetTrayDirect/SetShowFont/
//  SetShape/SetColorMap/SetFrameColor/SetTrayColor were all missing from the
//  prototype), and every method re-checked line-by-line against golden
//  (prototype's self-declared "30/30" was NOT taken on faith, per this
//  project's standing rule to independently re-verify FAITHFUL/complete
//  self-claims).
// ===========================================================================
#ifndef VCLCOMPAT_TRAYCORE_H
#define VCLCOMPAT_TRAYCORE_H

#include "vclcompat/AnsiString.h"

namespace vclcompat {

// ---------------------------------------------------------------------------
//  TColor + the small subset of golden Graphics.hpp colour constants this
//  core's ctors literally assign. TU-local shim, matching the established
//  convention already used elsewhere in this tree (cmydef.h:16,
//  acatchtray_shims.h:41-44, SECSGEM/uHGemEquipment.h:391-398 each declare
//  their own local copy rather than share one umbrella colour header).
//  Guarded (unlike some of those) because THIS wave puts three sibling
//  headers -- TrayCore.h / BtnPanelCore.h / LedCore.h -- into the same
//  directory with an identical block each, and a later wave (W7-C4) is
//  documented to end up #include-ing more than one of them from the same
//  translation unit (fMain.cpp will eventually own LEDs, tray grids AND
//  button panels together) -- so, unlike the older single-purpose shims,
//  collision here is a real near-term risk, not a hypothetical one.
// ---------------------------------------------------------------------------
#ifndef HT9045_W7C1_TCOLOR_SHIM
#define HT9045_W7C1_TCOLOR_SHIM
typedef int TColor;                           // golden Graphics.hpp TColor (0x00BBGGRR int)
const TColor clBlack   = 0x00000000;
const TColor clGreen   = 0x00008000;
const TColor clRed     = 0x000000FF;
const TColor clBlue    = 0x00FF0000;
const TColor clLime    = 0x0000FF00;
const TColor clYellow  = 0x0000FFFF;
const TColor clWhite   = 0x00FFFFFF;
const TColor clSilver  = 0x00C0C0C0;
const TColor clBtnFace = TColor(0x8000000F);  // system-colour reference (COLOR_BTNFACE); stored
                                               // verbatim, unresolved -- resolving it needs a real
                                               // GetSysColor() call, which is a renderer (W7-C2/C5)
                                               // concern, not a framework-free-core one.
#endif // HT9045_W7C1_TCOLOR_SHIM

// golden HTray.h:14 (also HTray256.h:14 as TTray256DirectStyle -- identical
// value set, TTMyTray256 has no block support so it reuses this one style
// enum rather than a second copy).
enum TTrayDirectStyle { csNull, csLeftTop, csLeftBottom, csRightTop, csRightBottom };

// Output of CellRect() -- the geometry half of golden's DrawSingleIC, split
// out so a renderer can ask "where would cell (X,Y) paint" without a Canvas.
struct TrayCellRect { int Left, Top, Right, Bottom; };

// ===========================================================================
//  TrayCore -- golden TTMyTray (HTray.h/.cpp). MAX_ITEM=100, MAX_COLOR_INDEX=32
//  (HTray.h:11-12; the comment there, decoded: "MAX_ITEM 100 // shared with
//  HTray256.cpp" -- historical, HTray256 actually uses its own 300, see
//  Tray256Core below).
// ===========================================================================
class TrayCore {
public:
    static const int MAX_COLOR_INDEX = 32;   // HTray.h:11
    static const int MAX_ITEM        = 100;  // HTray.h:12

    TrayCore();                                          // HTray.cpp:19-60

    // -- geometry outputs (HTray.h:18-23; recomputed by CaculateTrayParameter) --
    int iStartX, iStartY, iPitchX, iPitchY, iXWidth, iYWidth;

    // -- extents. Golden's ctor hardcodes Width=100/Height=200 (HTray.cpp:29-30)
    //    regardless of the real .dfm size; the real VCL control gets its true
    //    extent from CreateWnd->WM_SIZE before first paint. A framework-free
    //    core has no window, so SetExtents() below is this wave's stand-in for
    //    that VCL plumbing (NOT a golden-named method -- documented deviation,
    //    required because "a WM_SIZE arrives" isn't a thing here). --
    int Width, Height;
    void SetExtents(int w, int h);                       // NEW (not golden): sets Width/Height
                                                          // then re-derives geometry, replacing
                                                          // golden's CreateWnd/WndProc WM_SIZE hook
                                                          // (HTray.cpp:62-90, out of scope: no VCL
                                                          // window in a framework-free core).

    // -- published state (HTray.h:26-49) --
    int FXItem, FYItem, FXVisibleItem, FYVisibleItem;
    int FLineWidth, FEdgeWidth, FXBlock, FYBlock, FXBlockWidth, FYBlockWidth, FDirectWidth;
    bool FShowFont, FShape;
    TTrayDirectStyle FTrayDirect;
    TColor      ColorMap[MAX_COLOR_INDEX];
    int         CellColorIndex[MAX_ITEM][MAX_ITEM];
    AnsiString  CellText[MAX_ITEM][MAX_ITEM];
    bool bXVisible[MAX_ITEM], bYVisible[MAX_ITEM];
    TColor FFrameColor, FTrayColor;

    // -- property setters (HTray.h __published block) --
    void SetXItem(int value);                            // HTray.cpp:105-122
    void SetYItem(int value);                            // HTray.cpp:124-140 -- GOLDEN BUG, see .cpp
    void SetBlockXItem(int value);                       // HTray.cpp:142-162
    void SetBlockYItem(int value);                       // HTray.cpp:164-184 -- GOLDEN BUG, see .cpp
    void SetXBlockWidth(int value);                      // HTray.cpp:186-194
    void SetYBlockWidth(int value);                      // HTray.cpp:196-204
    void SetLineWidth(int value);                        // HTray.cpp:346-351 (DrawTray call dropped:
                                                          // rendering side effect, moved to W7-C2)
    void SetEdgeWidth(int value);                        // HTray.cpp:353-358 (ditto)
    void SetDirectWidth(int value);                      // HTray.cpp:360-366
    void SetTrayDirect(TTrayDirectStyle value);          // HTray.cpp:368-372 (ditto)
    void SetColorMap(int Index, TColor Color);           // HTray.cpp:374-380 (ditto)
    void SetFrameColor(TColor Color);                    // HTray.cpp:206-210 (ditto)
    void SetTrayColor(TColor Color);                     // HTray.cpp:212-216 (ditto)
    void SetShowFont(bool value);                        // HTray.cpp:390-394 (ditto)
    void SetShape(bool value);                           // HTray.cpp:396-400 (ditto)

    // -- cell data (HTray.h public block) --
    void SetCellColorIndex(int X, int Y, int Index);     // HTray.cpp:382-388 (DrawSingleIC dropped)
    void SetCellNumber(int x, int y, const AnsiString& s); // HTray.cpp:410-416 (char* overload folded
                                                            // in -- AnsiString ctor already accepts
                                                            // const char*, see AnsiString.h; DrawSingleIC
                                                            // side effect dropped, see above)
    void SetCellNumber(int x, int y, int s);             // HTray.cpp:418-423
    void SetCellNumber(int x, int y, double c);          // HTray.cpp:425-430
    int  GetCellData(int i, int j) const;                // HTray.cpp:432-435
    AnsiString GetCellText(int X, int Y) const;          // HTray.cpp:456-459
    void ClearCell();                                    // HTray.cpp:92-103 (DrawTray call dropped)
    int  ConvertIndexCells(int& X, int& Y) const;        // HTray.cpp:437-454 -- GOLDEN BUG, see .cpp
    void SetXVisible(int x, bool bVisible);              // HTray.cpp:480-492
    void SetYVisible(int y, bool bVisible);              // HTray.cpp:494-506
    AnsiString SaveCellTextToString() const;             // HTray.cpp:461-478, I/O split out: golden's
                                                          // SaveCellTextToFile builds a TStringList and
                                                          // calls ->SaveToFile(FileName); a core has no
                                                          // file system concern, so this returns the
                                                          // joined text and the SHELL is responsible for
                                                          // the actual file write (same string content,
                                                          // "\r\n"-joined rows, matching golden verbatim).

    // -- geometry / hit-test halves of DrawSingleIC/CaculateTrayParameter,
    //    with the Canvas calls stripped (renderer territory, W7-C2) --
    void CaculateTrayParameter();                        // HTray.cpp:262-270 -- GOLDEN BUG, see .cpp
    bool CellRect(int X, int Y, TrayCellRect& r) const;  // geometry half of DrawSingleIC
                                                          // (HTray.cpp:230-260) -- GOLDEN BUG, see .cpp
};

// ===========================================================================
//  Tray256Core -- golden TTMyTray256 (HTray256.h/.cpp). Same shape as
//  TrayCore but MAX_ITEM=300, MAX_COLOR_INDEX=32, and NO block/visibility
//  support (HTray256.h has no FXBlock/FYBlock/bXVisible/bYVisible at all --
//  verified directly against the golden header, not assumed from symmetry
//  with TrayCore).
// ===========================================================================
class Tray256Core {
public:
    static const int MAX_COLOR_INDEX = 32;    // HTray256.h:11
    static const int MAX_ITEM        = 300;   // HTray256.h:12

    Tray256Core();                                       // HTray256.cpp:18-49

    int iStartX, iStartY, iPitchX, iPitchY, iXWidth, iYWidth;
    int Width, Height;
    void SetExtents(int w, int h);                       // NEW (not golden): see TrayCore::SetExtents

    int FXItem, FYItem;
    int FLineWidth, FEdgeWidth, FDirectWidth;
    bool FShowFont, FShape;
    TTrayDirectStyle FTrayDirect;
    TColor      ColorMap[MAX_COLOR_INDEX];
    int         CellColorIndex[MAX_ITEM][MAX_ITEM];
    AnsiString  CellText[MAX_ITEM][MAX_ITEM];
    TColor FFrameColor, FTrayColor;

    void SetXItem(int value);                            // HTray256.cpp:92-102
    void SetYItem(int value);                            // HTray256.cpp:104-113
    void SetLineWidth(int value);                        // HTray256.cpp:253-258
    void SetEdgeWidth(int value);                        // HTray256.cpp:260-265
    void SetDirectWidth(int value);                      // HTray256.cpp:267-273
    void SetTrayDirect(TTrayDirectStyle value);          // HTray256.cpp:275-279
    void SetColorMap(int Index, TColor Color);           // HTray256.cpp:281-287
    void SetFrameColor(TColor Color);                    // HTray256.cpp:115-119
    void SetTrayColor(TColor Color);                     // HTray256.cpp:121-125
    void SetShowFont(bool value);                        // HTray256.cpp:297-301
    void SetShape(bool value);                           // HTray256.cpp:303-307

    void SetCellColorIndex(int X, int Y, int Index);     // HTray256.cpp:289-295
    void SetCellNumber(int x, int y, const AnsiString& s); // HTray256.cpp:309-315
    void SetCellNumber(int x, int y, int s);             // HTray256.cpp:317-322
    void SetCellNumber(int x, int y, double c);          // HTray256.cpp:324-329
    int  GetCellData(int i, int j) const;                // HTray256.cpp:331-334
    AnsiString GetCellText(int X, int Y) const;          // HTray256.cpp:355-358
    void ClearCell();                                    // HTray256.cpp:81-90
    int  ConvertIndexCells(int& X, int& Y) const;        // HTray256.cpp:336-353 (no block offsets to
                                                          // ignore here -- TTMyTray256 never had them,
                                                          // so this one does NOT carry TrayCore's bug 2)
    AnsiString SaveCellTextToString() const;             // HTray256.cpp:360-377, I/O split out (see
                                                          // TrayCore::SaveCellTextToString)

    void CaculateTrayParameter();                        // HTray256.cpp:169-177 -- GOLDEN BUG, see .cpp
    bool CellRect(int X, int Y, TrayCellRect& r) const;  // geometry half of DrawSingleIC
                                                          // (HTray256.cpp:140-167)
};

} // namespace vclcompat
#endif // VCLCOMPAT_TRAYCORE_H
