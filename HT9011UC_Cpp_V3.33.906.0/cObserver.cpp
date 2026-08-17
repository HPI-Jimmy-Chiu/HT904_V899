// =============================================================================
//  cObserver.cpp  --  FW-3 cObserver Wave 1: read-only display core
//
//  Translation wave: FW-3 cObserver Wave 1
//  Translator: AI(W906-FW3-Observer-W1) 20260818
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/cObserver.cpp (5,425
//  lines) + cObserver.h (567 lines), Big5/cp950. Decoded this wave with
//  `python3 -c "open(path,'rb').read().decode('cp950').encode('utf-8')"` --
//  0 U+FFFD over both files (measured before any line below was written).
//
//  ROLE
//  ----
//  golden TfObserver's "OEE Observer" read-only display core: constructor
//  data-init, the Contact-Count grids (StringGrid2/3, WriteContactKind), the
//  Bin/Yield tray displays (mtRow[]/ChartYield), the EventLogTxt CSV reader
//  (GetEventLogText + its 3 UI triggers), the SQLite MDB query dispatcher
//  (BtnQueryClick, 19 eQueryType branches) and btReportClick. See
//  forms/fObserver.h for the full WAVE SCOPE table, GATE REGISTER and the
//  four facade widget types this file's methods are written against --
//  not duplicated here to avoid the two files drifting apart.
//
//  ABSENCE-CLAIM TIMESTAMPS (commands + when run, this wave, before writing
//  the citing code below -- re-run at hand-off per project policy)
//  --------------------------------------------------------------------------
//    fMain->AlarmUnitMap  : `grep -n "AlarmUnitMap" forms/fMain.h` -> 0 hits (20260818)
//    fSecurity facade     : `ls forms/*.h` -> fAGV/fAOI/fCleaning/fFixAICCD/fHome/
//                            fLotInfo/fMain/fNote/fOCR/fOffSet/fProductionInfo/
//                            fRotate/fSCKART/fSetup/fShowMessage/fShuttleMove/
//                            fSortCT/fTrayForm -- no fSecurity.h (20260818)
//    DoProduction_Summary_Report / CountMTBF bodies outside Wave 1 ranges:
//                            `grep -n "DoProduction_Summary_Report\|CountMTBF"`
//                            over the cp950-decoded golden cObserver.cpp,
//                            filtered to Wave-1's own byte ranges -- 0 hits
//                            inside range, only the declaration + call sites
//                            (20260818)
//    TChart opacity        : cMyDB.h:75 `class TChart;` (opaque) + cMyDB.h:
//                            112-113 "BODY GATED (no TChart port, see file
//                            head)" on MyDBVAxleEventCount/MyDBVUnitEventCount
//                            (read directly, 20260818)
//
//  DESIGN NOTE -- the (long) cast on every ->GetTotal()/->GetPassCT() site
//  --------------------------------------------------------------------------
//  golden assigns `unsigned long` (TMySocket::GetTotal/GetPassCT's real
//  return type, cSocket.h:115/117) directly into an AnsiString lvalue/ctor
//  argument. vclcompat::AnsiString has ctor/operator= overloads for
//  int/unsigned int/long/long long/double (vclcompat/AnsiString.h:66-89) but
//  NOT unsigned long, so the implicit conversion golden relies on is
//  ambiguous here. An EARLIER wave already hit this identical golden pattern
//  translating cSocket.cpp itself and settled on an explicit `(long)` cast
//  (cSocket.cpp:838 `AnsiString((long)ArmSKET[i][j]->GetTotal())`, repeated
//  9 more times in that file) -- this file applies the SAME established cast
//  throughout WriteContactKind rather than inventing a second convention.
//
//  DESIGN NOTE -- DateSeparator='-'/'/ ' assignments dropped (inert)
//  --------------------------------------------------------------------------
//  Golden BtnQueryClick sets `DateSeparator='-';` three times (:2516/:2639/
//  :2699). Three EARLIER waves already hit this exact golden idiom and
//  established the same conclusion (cMyDB.cpp:897-900, PMAlarm/
//  PMAlarmSystem.cpp:464/493, SECSGEM/uHGemEquipment.cpp:3591-3595):
//  vclcompat::FormatDateTime always uses the literal separator characters
//  baked into its own format-string argument (e.g. "yyyy-mm-dd" already has
//  its own '-'), never consults a global DateSeparator to substitute into, so
//  the assignment has no observable effect in the ported build. Dropped
//  (inert), each site marked with a one-line pointer to this note rather than
//  repeating the full explanation three times.
//
//  DESIGN NOTE -- `.FormatString(fmt)` -> `FormatDateTime(fmt, dt)`
//  --------------------------------------------------------------------------
//  golden calls `<TDateTime>.FormatString(fmt)` throughout BtnQueryClick.
//  vclcompat::TDateTime has no FormatString member (established already:
//  ainarm9045.cpp:7219-7221, atester.cpp:9061/:9236, cprod.cpp:3334-3335,
//  Automation/SCK_ART_Remainder.cpp:1428) -- the free function
//  `FormatDateTime(fmt, dt)` is the real, already-used translation.
// =============================================================================
#include "forms/fObserver.h"

#include "MachineType.h"      // Type_HT9045, NN_1Row/NN_2Row, QualSite2X2N/_6Site2X3N/_8Site2X4N,
                               //   CC_SINOICTECH/CC_AMKOR_Philippines/CC_Microchip_Phil,
                               //   MAX_SOCKET_ROW/COL/TOTAL, TEST_MAX_BIN
#include "cmydef.h"            // TColor, HistroyBin/HistroyPassFail, CUSTOMER_CODE, MachineTypeChoice,
                               //   asTempCtrl[]/tcTotalCount
#include "cprod.h"             // RUN_INFO/RunInfo, SYSTEM_TEST_IF/TestIF
#include "aHotPlateSubstrate.h" // TMyKitSuck/TestSocket (NOT mykitsuck.h -- see KNOWLEDGE.md two-TMyKitSuck ODR gotcha)
#include "cSocket.h"           // TArm/TMySocket, ArmData[3]/ArmHistory[3]
#include "cinitial.h"          // IsNNMode()
#include "Config.h"            // IniConfig (bSPILFunction)
#include "common.h"            // CheckAndReadIniDataGeneral, AuthPath/asGeneralPath/OpenGeneralIniFile (test seam)
#include "cAuthority.h"        // GetObserAuth()
#include "cMyDB.h"             // MyDBVProcess/MyDBVProcessFilter/MyDBVEventFreq/MyDBQTotalLoader/MyDBQTimeData
#include "Public/HTMD5.h"      // SearchFileAll

#include <cstdlib>             // atoi

// ---------------------------------------------------------------------------
//  TU-local VCL standard-colour shim.
//
//  golden's file-scope TC[] cycle (below) and the ctor's mtXxx->SetColorMap()
//  calls need 16 standard VCL Graphics.hpp colour constants. This project's
//  established convention (per vclcompat/TrayCore.h's own banner, itself
//  citing cmydef.h:16 / acatchtray_shims.h:41-44 / SECSGEM/uHGemEquipment.h:
//  391-398) is: no shared umbrella colour header -- each TU that needs a
//  colour name not already visible declares its own local copy. Checked this
//  wave (grep, 20260818) against every header this file includes
//  (cmydef.h/cprod.h/cpublic.h/common.h/Config.h/aHotPlateSubstrate.h/
//  cMyDB.h/cAuthority.h/cSocket.h/cinitial.h/MachineType.h): none define any
//  of the 16. (vclcompat/TrayCore.h -- pulled in transitively via
//  forms/fObserver.h -- DOES define 6 of them, but inside `namespace
//  vclcompat`, so they do not collide with these GLOBAL-scope names golden's
//  own unqualified spelling needs.) Values are the standard Delphi
//  Graphics.pas TColor literals ($00BBGGRR); the two *system* colours
//  (clInactiveCaption/clHighlight) are the raw $80000000|COLOR_* sentinel,
//  stored unresolved -- same posture as TrayCore.h's own clBtnFace precedent
//  (resolving a system colour needs a real GetSysColor() call, a renderer
//  concern, out of scope for a headless facade).
// ---------------------------------------------------------------------------
#ifndef HT9045_W906_FW3OBS_TCOLOR_SHIM
#define HT9045_W906_FW3OBS_TCOLOR_SHIM
static const TColor clBlack           = 0x00000000;
static const TColor clMaroon          = 0x00000080;
static const TColor clGreen           = 0x00008000;
static const TColor clOlive           = 0x00008080;
static const TColor clNavy            = 0x00800000;
static const TColor clPurple          = 0x00800080;
static const TColor clTeal            = 0x00808000;
static const TColor clGray            = 0x00808080;
static const TColor clSilver          = 0x00C0C0C0;
static const TColor clRed             = 0x000000FF;
static const TColor clLime            = 0x0000FF00;
static const TColor clBlue            = 0x00FF0000;
static const TColor clFuchsia         = 0x00FF00FF;
static const TColor clAqua            = 0x00FFFF00;
static const TColor clInactiveCaption = TColor(0x80000003);   // COLOR_INACTIVECAPTION
static const TColor clHighlight       = TColor(0x8000000D);   // COLOR_HIGHLIGHT
#endif // HT9045_W906_FW3OBS_TCOLOR_SHIM

// ---------------------------------------------------------------------------
//  File-scope data (golden cObserver.cpp:51-58) -- landed in golden's own
//  shape per this wave's task brief, `static` (internal linkage) since golden
//  cObserver.h declares none of these `extern` (grepped the full header this
//  wave -- 0 hits for any of the 7 names below), so nothing outside this TU
//  can legitimately reference them; `static` only removes a needless
//  external-linkage symbol, it does not change any observable behaviour.
//  Of the 7, only sCounterColKind/mtRow/bShowYieldSeries/bShowYieldAll have a
//  consumer inside Wave 1's translated methods; sSocketCT/sSKETTotal/
//  sPassSKET/TesterCH are placeholder data for a later wave's methods
//  (ProcessRunInfo/WriteCategoryData/... -- outside this wave's scope) and
//  are landed here only so this file's global-data shape matches golden's,
//  per the task brief.
// ---------------------------------------------------------------------------
static char sSocketCT[MAX_SOCKET_COL][TEST_MAX_BIN + 1][64] __attribute__((unused));
static AnsiString sSKETTotal[TEST_MAX_BIN + 1], sPassSKET[TEST_MAX_BIN + 1];
static AnsiString sCounterColKind[MAX_SOCKET_ROW][MAX_SOCKET_COL], TesterCH[MAX_SOCKET_ROW][MAX_SOCKET_COL];
static TfObserverTray *mtRow[MAX_SOCKET_ROW];
static bool bShowYieldSeries[MAX_SOCKET_ROW][MAX_SOCKET_COL];
static bool bShowYieldAll[MAX_SOCKET_ROW];

// golden cObserver.cpp:86-101 (TC[] colour-cycle table, tcTotalCount==70
// entries -- MachineType.h). `static`, same internal-linkage rationale as
// the block above (cObserver.h declares no `extern TColor TC[]` either).
static TColor TC[tcTotalCount] = {
    clSilver,          clRed,      clLime,         clInactiveCaption,  clBlue,
    clFuchsia,         clAqua,     clHighlight,    clBlack,            clMaroon,
    clGreen,           clOlive,    clNavy,         clPurple,           clTeal,
    clGray,            clSilver,   clRed,          clLime,             clInactiveCaption,
    clBlue,            clFuchsia,  clAqua,         clHighlight,        clBlack,
    clMaroon,          clGreen,    clOlive,        clNavy,             clPurple,
    clTeal,            clGray,     clSilver,       clRed,              clLime,
    clInactiveCaption, clBlue,     clFuchsia,      clAqua,             clHighlight,
    clBlack,           clMaroon,   clGreen,        clOlive,            clNavy,
    clPurple,          clTeal,     clGray,         clSilver,           clRed,
    clLime,            clPurple,   clTeal,         clGray,             clSilver,
    clInactiveCaption, clBlue,     clFuchsia,      clAqua,             clHighlight,
    clBlack,           clMaroon,   clGreen,        clOlive,            clNavy,
    clGray,            clSilver,   clRed,          clLime,             clPurple,
    clTeal
};

// golden cObserver.cpp:103-123 (eQueryType) -- TU-local (golden declares it
// at file scope in the .cpp, not in cObserver.h), used by BtnQueryClick.
enum eQueryType
{
    Event_Log = 0,
    Process_Record,
    Message_Record,
    Motion_Record,
    Production_Record,
    Alarm_History,
    Alarm_History_JAM,
    Alarm_History_MES,
    Alarm_History_WAR,
    Alarm_Statistics,
    Alarm_Stat_Day,
    Jam_Chart_Summary,
    Jam_Chart_InArm,
    Jam_Chart_OutArm,
    Jam_Chart_Index,
    Jam_Chart_InShuttle,
    Jam_Chart_OutShuttle,
    Alarm_Code_List,
    Production_Summary_Report,
    Lot_Infomation
};

// =============================================================================
//  TfObserver::TfObserver() -- golden :137-346
//
//  Split into two clearly separated sections:
//   (A) PORT-ONLY bootstrap: `new`-allocates every widget stand-in member
//       (a real .dfm does this at design/load time; a headless facade has no
//       .dfm, so this section stands in for that -- NOT a translation of any
//       golden C++ line) and pre-populates ChartYield (32 series) / Chart2
//       (1 series) to match golden's own .dfm wiring (32 named
//       SeriesAa..SeriesDh LineSeries components on ChartYield, 1 BarSeries2
//       on Chart2 -- see forms/fObserver.h's DESIGN NOTES). Grid initial
//       ColCount/RowCount are golden's OWN .dfm design-time values, read from
//       tools/dfm2rc/ir_out/cObserver.dfm.ir.json this wave (cited per grid).
//   (B) The golden-translated ctor body itself (:137-346), faithful line by
//       line, operating on the now-constructed members from (A).
// =============================================================================
TfObserver::TfObserver()
{
    // --- (A) PORT-ONLY bootstrap -------------------------------------------
    mtRowA          = new TfObserverTray();
    mtRowB          = new TfObserverTray();
    mtRowC          = new TfObserverTray();
    mtRowD          = new TfObserverTray();
    mtCategorySum   = new TfObserverTray();
    mtRowName       = new TfObserverTray();
    mtNo            = new TfObserverTray();
    mtTotalName     = new TfObserverTray();
    myCategoryName  = new TfObserverTray();
    mtChName        = new TfObserverTray();
    mtDutName       = new TfObserverTray();
    mtArmName       = new TfObserverTray();
    mtCategoryNo    = new TfObserverTray();
    mtHeadTotal     = new TfObserverTray();
    mtSockTotal     = new TfObserverTray();
    mtPassHead      = new TfObserverTray();
    mtPassSocket    = new TfObserverTray();
    mtIfError       = new TfObserverTray();
    mtTotal         = new TfObserverTray();
    mtCategoryTotal = new TfObserverTray();

    // golden .dfm design-time ColCount/RowCount (tools/dfm2rc/ir_out/cObserver.dfm.ir.json, read 20260818):
    StringGrid2                = new TfObserverGrid(17, 4);    // StringGrid2: ColCount=17 RowCount=4
    StringGrid3                = new TfObserverGrid(9, 4);     // StringGrid3: ColCount=9  RowCount=4
    StringGrid5                = new TfObserverGrid(10, 9);    // StringGrid5: ColCount=10 RowCount=9
    TimeInfoGrid                = new TfObserverGrid(6, 15);    // TimeInfoGrid: ColCount=6 RowCount=15
    strngrdTestTime             = new TfObserverGrid(6, 15);    // strngrdTestTime: ColCount=6 RowCount=15
    // AI(W906-FW3-Observer-W1) 20260818: sgTimeData constructed with 11
    // columns, NOT the .dfm's own reported ColCount=10 (tools/dfm2rc/ir_out/
    // cObserver.dfm.ir.json). Measured this wave (a real out_of_range thrown
    // constructing a TfObserver and traced to this exact site): the ctor
    // body just below writes `sgTimeData->Cells[10][0]="Last 9";` -- column
    // INDEX 10, which needs ColCount>=11 (0-based). Whatever real BCB6's
    // TStringGrid does with a design-time ColCount=10 and a Cells[10][...]
    // write is not re-derivable from a .dfm value alone; vclcompat's
    // TStringGrid deliberately throws on out-of-range Cells access instead
    // of silently truncating (vclcompat/StringGrid.h's own "OUT-OF-RANGE
    // BEHAVIOR" note), so sizing this facade grid to fit what the ACTUAL
    // ported code writes -- not the possibly-stale .dfm figure -- is what
    // makes the translated ctor run at all. RowCount=22 is unaffected (still
    // the .dfm's own value; nothing in Wave 1's code writes past row 21).
    sgTimeData                  = new TfObserverGrid(11, 22);   // sgTimeData: ColCount=11 (see note above), RowCount=22
    sg_ListTimeReceiveInfoGrid  = new TfObserverGrid(3, 15);    // sg_ListTimeReceiveInfoGrid: ColCount=3 RowCount=15
    strngrdJamLog                = new TfObserverGrid(9, 5);     // strngrdJamLog: ColCount=9 (ctor body resizes to 6), RowCount default 5
    strngrdIndeAirOn1            = new TfObserverGrid(4, 105);   // strngrdIndeAirOn1: ColCount=4 RowCount=105
    strngrdIndeAirOn2            = new TfObserverGrid(4, 105);   // strngrdIndeAirOn2: ColCount=4 RowCount=105
    strngrdEventLog              = new TfObserverGrid(9, 5);     // strngrdEventLog: ColCount=9, RowCount default 5 (dfm doesn't set RowCount)
    strngrdMDBQuery              = new TfObserverGrid(9, 5);     // strngrdMDBQuery: ColCount=9, RowCount default 5

    labMachineID    = new TPanel();
    labSerialNo     = new TPanel();
    pnlTotalCount   = new TPanel();
    lbltTotalLoader = new TPanel();

    cbbTempChart    = new TComboBox();
    cbDisplayData   = new TComboBox();
    cbbEventLogYear = new TComboBox();
    cbbMonth        = new TComboBox();
    cbbFilter       = new TComboBox();

    lstEventLog = new TListBox();

    rgContactCountKinds       = new TRadioGroup();
    rgContactCountKindsForm   = new TRadioGroup();
    rgContactCountHistory     = new TRadioGroup();
    rgContactCountHistoryForm = new TRadioGroup();

    ChartYield = new TfObserverChart();
    TempChart  = new TfObserverChart();
    Chart2     = new TfObserverChart();
    // golden .dfm wires 32 named TLineSeries (SeriesAa..SeriesDh) onto
    // ChartYield at design time -- UpdateYieldChart's `for(iRow<MAX_SOCKET_TOTAL)
    // ChartYield->Series[iRow]` assumes they already exist. Pre-populate here
    // (PORT-ONLY, not a golden ctor line -- see banner).
    for (int i = 0; i < MAX_SOCKET_TOTAL; i++)
        ChartYield->AddSeries(new TfObserverChartSeries());
    // golden .dfm wires 1 BarSeries2 onto Chart2 (BtnQueryClick's Jam Chart
    // branch indexes Chart2->Series[0]). PORT-ONLY, see above.
    Chart2->AddSeries(new TfObserverChartSeries());

    DateTimePicker1 = new TfObserverDateTimePicker();
    DateTimePicker2 = new TfObserverDateTimePicker();
    DateTimePicker3 = new TfObserverDateTimePicker();
    DateTimePicker4 = new TfObserverDateTimePicker();

    // --- (B) golden-translated ctor body, :137-346 --------------------------
    int iRow, iCol;
    mtRow[0] = mtRowA;
    mtRow[1] = mtRowB;
    mtRow[2] = mtRowC;
    mtRow[3] = mtRowD;
    AnsiString s = "";
    int iColorCT = 0;
    for (iRow = 0; iRow < MAX_SOCKET_ROW; iRow++)
    {
        mtRow[iRow]->Core.SetColorMap(0, (TColor)0x00C2B8A6);
        mtRow[iRow]->Core.SetColorMap(1, clGreen);
        mtRow[iRow]->Core.SetColorMap(2, clRed);
        mtRow[iRow]->Tag = iRow;
        bShowYieldAll[iRow] = false;
        for (iCol = 0; iCol < MAX_SOCKET_COL; iCol++)
        {
            mtRow[iRow]->Core.SetColorMap(3 + iCol, TC[iColorCT]);
            iColorCT++;

            bShowYieldSeries[iRow][iCol] = true;
        }
    }

    mtCategorySum   ->Core.SetColorMap(0, clInactiveCaption);   //Steven 20131030 : 客戶嫌不容易看,改顏色
    mtRowName       ->Core.SetColorMap(0, clInactiveCaption);
    mtNo            ->Core.SetColorMap(0, clInactiveCaption);
    mtTotalName     ->Core.SetColorMap(0, clInactiveCaption);
    myCategoryName  ->Core.SetColorMap(0, clInactiveCaption);
    mtChName        ->Core.SetColorMap(0, clInactiveCaption);
    mtDutName       ->Core.SetColorMap(0, clInactiveCaption);
    mtArmName       ->Core.SetColorMap(0, clInactiveCaption);
    mtCategoryNo    ->Core.SetColorMap(0, (TColor)0x00DFD9CC);
    mtHeadTotal     ->Core.SetColorMap(0, (TColor)0x00DFD9CC);
    mtSockTotal     ->Core.SetColorMap(0, (TColor)0x00DFD9CC);
    mtPassHead      ->Core.SetColorMap(0, (TColor)0x00DFD9CC);
    mtPassSocket    ->Core.SetColorMap(0, (TColor)0x00DFD9CC);
    mtIfError       ->Core.SetColorMap(0, (TColor)0x00DFD9CC);
    mtTotal         ->Core.SetColorMap(0, (TColor)0x00DFD9CC);
    mtCategoryTotal ->Core.SetColorMap(0, (TColor)0x00DFD9CC);

    // golden :181-184 `DcSG2=GetDC(StringGrid2->Handle); DcSG3=...; DcSG5=...;
    // pCanvas=new TCanvas;` -- GDI canvas setup, consumed only by the
    // DrawCell*/DrawCenterLine family (out of scope this wave, see project
    // rule). No facade HWND/HDC to bind offline; dropped, not gated (there is
    // no observable effect to preserve: nothing in Wave 1's methods reads
    // DcSG2/DcSG3/DcSG5/pCanvas).

    if (MachineTypeChoice == Type_HT9045)   //9045
    {
        StringGrid2->ColCount = 9;
        StringGrid2->DefaultColWidth = 90;
        StringGrid3->ColCount = 9;
        StringGrid3->DefaultColWidth = 90;
    }
    else
    {
        StringGrid2->ColCount = 17;
        StringGrid2->DefaultColWidth = 47;
        StringGrid3->ColCount = 17;
        StringGrid3->DefaultColWidth = 47;
    }

    TimeInfoGrid->Cells[1][0] = "Site      ";        TimeInfoGrid->Cells[1][1] = "Start Time";
    TimeInfoGrid->Cells[2][1] = "End   Time";        TimeInfoGrid->Cells[3][1] = "Test  Time";
    TimeInfoGrid->Cells[4][1] = "Index Cycle Time";  TimeInfoGrid->Cells[5][1] = "Index Time";
    TimeInfoGrid->Cells[0][14] = "Average";
    TimeInfoGrid->Cells[0][11] = "Now";              TimeInfoGrid->Cells[0][10] = "Last 1";
    TimeInfoGrid->Cells[0][9] = "Last 2";            TimeInfoGrid->Cells[0][8] = "Last 3";
    TimeInfoGrid->Cells[0][7] = "Last 4";            TimeInfoGrid->Cells[0][6] = "Last 5";
    TimeInfoGrid->Cells[0][5] = "Last 6";            TimeInfoGrid->Cells[0][4] = "Last 7";
    TimeInfoGrid->Cells[0][3] = "Last 8";            TimeInfoGrid->Cells[0][2] = "Last 9";

    strngrdTestTime->Cells[1][0] = "Site      ";      strngrdTestTime->Cells[1][1] = "Start Time";
    strngrdTestTime->Cells[2][1] = "End   Time";      strngrdTestTime->Cells[3][1] = "Test  Time";
    strngrdTestTime->Cells[4][1] = "Index Cycle Time"; strngrdTestTime->Cells[5][1] = "Index Time";
    strngrdTestTime->Cells[0][14] = "Average";
    strngrdTestTime->Cells[0][11] = "Now";            strngrdTestTime->Cells[0][10] = "Last 1";
    strngrdTestTime->Cells[0][9] = "Last 2";          strngrdTestTime->Cells[0][8] = "Last 3";
    strngrdTestTime->Cells[0][7] = "Last 4";          strngrdTestTime->Cells[0][6] = "Last 5";
    strngrdTestTime->Cells[0][5] = "Last 6";          strngrdTestTime->Cells[0][4] = "Last 7";
    strngrdTestTime->Cells[0][3] = "Last 8";          strngrdTestTime->Cells[0][2] = "Last 9";

    iTotoalTestTime = 0;
    GetObserAuth();                                    //Steven 20090827 : 用來決定某些畫面是否顯示
    SetSiteYieldDiagram();                              //Steven 20100126
    iShowYieldChart = 0;                                //Steven 20100818 : 只顯示Yield Chart
    labMachineID->Caption = IniConfig.SocketHandlerID;

    sgTimeData->Cells[1][0] = "Current";                //JerryYang 20151209
    sgTimeData->Cells[2][0] = "Last 1";
    sgTimeData->Cells[3][0] = "Last 2";
    sgTimeData->Cells[4][0] = "Last 3";
    sgTimeData->Cells[5][0] = "Last 4";
    sgTimeData->Cells[6][0] = "Last 5";
    sgTimeData->Cells[7][0] = "Last 6";
    sgTimeData->Cells[8][0] = "Last 7";
    sgTimeData->Cells[9][0] = "Last 8";
    sgTimeData->Cells[10][0] = "Last 9";
    sgTimeData->Cells[0][0] = "Motion Part";

    sgTimeData->DefaultColWidth = 60;
    sgTimeData->ColWidths[0] = 200;
    sgTimeData->Cells[0][1] = "DoArmPickFromLoadStage_9045_2x8_8";
    sgTimeData->Cells[0][2] = "DoArmPickFromLoadStage_9045_2x8_8  OK";
    sgTimeData->Cells[0][3] = "DoPlaceToHotPlate_9045_2x8_8";
    sgTimeData->Cells[0][4] = "DoPlaceToHotPlate_9045_2x8_8  OK";
    sgTimeData->Cells[0][5] = "DoInArmPickFromHotPlate_9045_2x8_8  OK";
    sgTimeData->Cells[0][6] = "DoPlaceToShuttle_9045_2x8_8";
    sgTimeData->Cells[0][7] = "DoPlaceToShuttle_9045_2x8_8   OK";
    sgTimeData->Cells[0][8] = "MoveInArm2XYToShuttle1_9045_2x8_8";
    sgTimeData->Cells[0][9] = "MoveInArm2XYToShuttle1_9045_2x8_8  OK";
    sgTimeData->Cells[0][10] = "MoveInArm2XYToShuttle2_9045_2x8_8";
    sgTimeData->Cells[0][11] = "MoveInArm2XYToShuttle2_9045_2x8_8  OK";
    sgTimeData->Cells[0][12] = "InArmAtShuttle1Delay";
    sgTimeData->Cells[0][13] = "InArmAtShuttle1Delay OK";
    sgTimeData->Cells[0][14] = "InArm2AtShuttle2Delay";
    sgTimeData->Cells[0][15] = "InArm2AtShuttle2Delay  OK";
    sgTimeData->Cells[0][18] = "Drop Contact 1";        //JerryYang 20170503 (wei) drop contact的index cycle time分成三段來計時
    sgTimeData->Cells[0][19] = "Drop Contact 2";
    sgTimeData->Cells[0][20] = "Drop Contact 3";
    sgTimeData->Cells[0][21] = "Total";
    RunInfo.SoftwareDate = AnsiString(__DATE__) + "  " + AnsiString(__TIME__);   //Steven 20140429 : For SECS_GEM

    if (IniConfig.bSPILFunction == true)   //Steven 20240604 : SPIL格式的event log
    {
        strngrdEventLog->ColCount = 9;
        strngrdEventLog->ColWidths[0] = 50;    //No.
        strngrdEventLog->ColWidths[1] = 50;    //UnitName
        strngrdEventLog->ColWidths[2] = 50;    //AlarmCode
        strngrdEventLog->ColWidths[3] = 100;   //OccurDateTime
        strngrdEventLog->ColWidths[4] = 50;    //Recovery
        strngrdEventLog->ColWidths[5] = 50;    //StopedTime
        strngrdEventLog->ColWidths[6] = 50;    //Duplicate
        strngrdEventLog->ColWidths[7] = 200;   //Message
        strngrdEventLog->ColWidths[8] = 390;   //ErrPart
        strngrdEventLog->Cells[0][0] = "No.";
    }
    else   //Steven 20170929 (wei) : 使用 Event Log Text當作顯示畫面
    {
        strngrdEventLog->ColCount = 9;
        strngrdEventLog->ColWidths[0] = 60;    //Date
        strngrdEventLog->ColWidths[1] = 70;    //Time
        strngrdEventLog->ColWidths[2] = 70;    //UnitName
        strngrdEventLog->ColWidths[3] = 50;    //AlarmCode
        strngrdEventLog->ColWidths[4] = 50;    //Recovery
        strngrdEventLog->ColWidths[5] = 70;    //StopedTime
        strngrdEventLog->ColWidths[6] = 50;    //Duplicate
        strngrdEventLog->ColWidths[7] = 390;   //Message
        strngrdEventLog->ColWidths[8] = 390;   //ErrPart
    }

    // golden :290-294 (bSavePrecautionRecordFinish/bStartPrecautionRecord/
    // bChangeReciepeSaveMajorMaintenanceRecord/bShowMajorMaintenanceRecord/
    // asStartPrecautionRecordMOId) -- Precaution-record members with no
    // consumer anywhere in Wave 1's method list; not added to
    // forms/fObserver.h (adding a member no delivered method touches would be
    // inventing surface, the same discipline forms/fMain.h's own banner
    // states). Left for the wave that translates the Precautions tab.

    sg_ListTimeReceiveInfoGrid->Cells[1][0] = "Site      ";   sg_ListTimeReceiveInfoGrid->Cells[1][1] = "Recevie Data";
    sg_ListTimeReceiveInfoGrid->Cells[2][1] = "Recevie Time"; //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
    sg_ListTimeReceiveInfoGrid->Cells[0][14] = "Average";
    sg_ListTimeReceiveInfoGrid->Cells[0][11] = "Now";         sg_ListTimeReceiveInfoGrid->Cells[0][10] = "Last 1";
    sg_ListTimeReceiveInfoGrid->Cells[0][9] = "Last 2";       sg_ListTimeReceiveInfoGrid->Cells[0][8] = "Last 3";
    sg_ListTimeReceiveInfoGrid->Cells[0][7] = "Last 4";       sg_ListTimeReceiveInfoGrid->Cells[0][6] = "Last 5";
    sg_ListTimeReceiveInfoGrid->Cells[0][5] = "Last 6";       sg_ListTimeReceiveInfoGrid->Cells[0][4] = "Last 7";
    sg_ListTimeReceiveInfoGrid->Cells[0][3] = "Last 8";       sg_ListTimeReceiveInfoGrid->Cells[0][2] = "Last 9";

    strngrdJamLog->ColCount = 6;   //KaiChen 20200618 ：矽格，增加Jam統計頁面
    strngrdJamLog->ColWidths[0] = 50;
    strngrdJamLog->ColWidths[1] = 100;   //UnitName
    strngrdJamLog->ColWidths[2] = 100;   //AlarmCode
    strngrdJamLog->ColWidths[3] = 400;   //Message
    strngrdJamLog->ColWidths[4] = 100;
    strngrdJamLog->ColWidths[5] = 100;
    strngrdJamLog->Cells[0][0] = "No";
    strngrdJamLog->Cells[1][0] = "UnitName";
    strngrdJamLog->Cells[2][0] = "AlarmCode";
    strngrdJamLog->Cells[3][0] = "Message";
    strngrdJamLog->Cells[4][0] = "Count";
    strngrdJamLog->Cells[5][0] = "Rate (%)";

    strngrdIndeAirOn1->Cells[1][0] = "          ";  strngrdIndeAirOn1->Cells[1][1] = "Start Time";   //Sam 20220329 : Record Index Air On Time
    strngrdIndeAirOn1->Cells[2][1] = "End   Time";  strngrdIndeAirOn1->Cells[3][1] = "Air On Time";
    for (int i = 0; i < 100; i++)
    {
        s.sprintf("Last %d", i);
        strngrdIndeAirOn1->Cells[0][101 - i] = s;
    }

    strngrdIndeAirOn2->Cells[1][0] = "          ";  strngrdIndeAirOn2->Cells[1][1] = "Start Time";
    strngrdIndeAirOn2->Cells[2][1] = "End   Time";  strngrdIndeAirOn2->Cells[3][1] = "Air On Time";
    for (int i = 0; i < 100; i++)
    {
        s.sprintf("Last %d", i);
        strngrdIndeAirOn2->Cells[0][101 - i] = s;
    }

    labSerialNo->Caption = CheckAndReadIniDataGeneral("Version", "Serial No", AnsiString("29818"));

    cbbTempChart->Items->Clear();
    cbbTempChart->Items->Add("All");
    for (int j = 0; j < tcTotalCount; j++)   //Steven 20210621 : 動態產生溫度線段
    {
        TfObserverChartSeries *series = TempChart->AddSeries(new TfObserverChartSeries());
        series->Title = asTempCtrl[j];
        cbbTempChart->Items->Add(asTempCtrl[j]);
    }
}

TfObserver::~TfObserver()
{
    delete mtRowA; delete mtRowB; delete mtRowC; delete mtRowD;
    delete mtCategorySum; delete mtRowName; delete mtNo; delete mtTotalName;
    delete myCategoryName; delete mtChName; delete mtDutName; delete mtArmName;
    delete mtCategoryNo; delete mtHeadTotal; delete mtSockTotal; delete mtPassHead;
    delete mtPassSocket; delete mtIfError; delete mtTotal; delete mtCategoryTotal;

    delete StringGrid2; delete StringGrid3; delete StringGrid5;
    delete TimeInfoGrid; delete strngrdTestTime; delete sgTimeData;
    delete sg_ListTimeReceiveInfoGrid; delete strngrdJamLog;
    delete strngrdIndeAirOn1; delete strngrdIndeAirOn2;
    delete strngrdEventLog; delete strngrdMDBQuery;

    delete labMachineID; delete labSerialNo; delete pnlTotalCount; delete lbltTotalLoader;

    delete cbbTempChart; delete cbDisplayData; delete cbbEventLogYear;
    delete cbbMonth; delete cbbFilter;

    delete lstEventLog;

    delete rgContactCountKinds; delete rgContactCountKindsForm;
    delete rgContactCountHistory; delete rgContactCountHistoryForm;

    delete ChartYield; delete TempChart; delete Chart2;

    delete DateTimePicker1; delete DateTimePicker2; delete DateTimePicker3; delete DateTimePicker4;
}

// =============================================================================
//  SetSiteYieldDiagram -- golden :771-837
// =============================================================================
void TfObserver::SetSiteYieldDiagram()   //Steven 20090714
{
    AnsiString str;

    if (TestSocket.iShtRow <= 2)
    {
        mtRowA->Core.Width = 860;
        mtRowB->Core.Width = 860;
    }
    else
    {
        mtRowA->Core.Width = 420;
        mtRowB->Core.Width = 420;
    }

    for (int i = 0; i < TestSocket.iMaxRow; i++)
    {
        if (i < TestSocket.iShtRow)
        {
            mtRow[i]->Visible = true;
            for (int j = TestSocket.iShtCol; j < TestSocket.iMaxCol; j++)
            {
                bShowYieldSeries[i][j] = false;
            }
        }
        else
        {
            mtRow[i]->Visible = false;
            for (int j = 0; j < TestSocket.iMaxCol; j++)
            {
                bShowYieldSeries[i][j] = false;
            }
        }
        mtRow[i]->Core.SetYItem(TestSocket.iShtCol + 1);

        if (bShowYieldAll[i])
            mtRow[i]->Core.SetCellNumber(0, 0, AnsiString("Show All"));
        else
            mtRow[i]->Core.SetCellNumber(0, 0, AnsiString("Hide All"));

        mtRow[i]->Core.SetCellColorIndex(0, 0, 3 + TestSocket.iMaxCol + 1);
        mtRow[i]->Core.SetCellNumber(1, 0, AnsiString("Now"));
        mtRow[i]->Core.SetCellNumber(2, 0, AnsiString("Last 1"));
        mtRow[i]->Core.SetCellNumber(3, 0, AnsiString("Last 2"));
        mtRow[i]->Core.SetCellNumber(4, 0, AnsiString("Last 3"));
        mtRow[i]->Core.SetCellNumber(5, 0, AnsiString("Last 4"));
        mtRow[i]->Core.SetCellNumber(6, 0, AnsiString("Last 5"));
        mtRow[i]->Core.SetCellNumber(7, 0, AnsiString("Last 6"));
        mtRow[i]->Core.SetCellNumber(8, 0, AnsiString("Last 7"));
        mtRow[i]->Core.SetCellNumber(9, 0, AnsiString("Last 8"));
        mtRow[i]->Core.SetCellNumber(10, 0, AnsiString("Last 9"));

        for (int j = 0; j < TestSocket.iMaxCol; j++)
        {
            str.sprintf("%c%c", 'A' + i, 'a' + j);
            mtRow[i]->Core.SetCellNumber(0, j + 1, str);

            if (bShowYieldSeries[i][j])
                mtRow[i]->Core.SetCellColorIndex(0, j + 1, 3 + j);
            else
                mtRow[i]->Core.SetCellColorIndex(0, j + 1, 0);
        }
    }

    UpdateBin();   //Steven 20131030 : 改完後要更新
}

// =============================================================================
//  UpdateYieldChart -- golden :838-864
// =============================================================================
void TfObserver::UpdateYieldChart()
{
    int iLast, iSeries;
    AnsiString str;
    RUN_INFO *RIF;
    RIF = &RunInfo;

    for (int iRow = 0; iRow < MAX_SOCKET_TOTAL; iRow++)   //Steven 20100126 : 將全部的線都清空
        ChartYield->Series[iRow]->Clear();

    for (int iRow = 0; iRow < MAX_SOCKET_ROW; iRow++)
    {
        for (int iCol = 0; iCol < MAX_SOCKET_COL; iCol++)
        {
            iSeries = iRow * MAX_SOCKET_COL + iCol;
            for (iLast = 0; iLast < 25; iLast++)
            {
                if ((iLast % 2) == 0)  str.sprintf("%02d:%02d", RIF->iYieldHour[iLast], RIF->iYieldMin[iLast]);
                else                    str = AnsiString("");   //Steven 20210625 : 修正sprintf("")會跳例外的問題
                ChartYield->Series[iSeries]->AddY(RIF->iYieldChart[iRow][iCol][iLast], str, TC[iSeries]);
            }

            ChartYield->Series[iSeries]->Active = bShowYieldSeries[iRow][iCol];
        }
    }
}

// =============================================================================
//  SpeedButton1Click -- golden :865-872 (清空BIN的歷史資料)
// =============================================================================
void TfObserver::SpeedButton1Click(void * /*Sender*/)
{
    for (int iRow = 0; iRow < MAX_SOCKET_ROW; iRow++)
        for (int iCol = 0; iCol < MAX_SOCKET_COL; iCol++)
            for (int iLast = 0; iLast < 21; iLast++)   //Eastsun 20260526 #026-1.33 Ifor 20221026 add:KYEC 要求增加比數10 => 21
                HistroyBin[iRow][iCol][iLast] = 0;
}

// =============================================================================
//  UpdateBin -- golden :873-904
// =============================================================================
void TfObserver::UpdateBin()
{
    int ir, ic;

    for (int iLast = 0; iLast < 21; iLast++)   //Eastsun 20260526 #026-1.34 Ifor 20221026 add:KYEC 要求增加比數10 => 21
    {
        for (int iRow = 0; iRow < MAX_SOCKET_ROW; iRow++)
        {
            for (int iCol = 0; iCol < MAX_SOCKET_COL; iCol++)
            {
                ir = iLast + 1;
                ic = iCol + 1;
                if (HistroyBin[iRow][iCol][iLast] != 0 && HistroyBin[iRow][iCol][iLast] != -1)   //kevin 20150304  關SITE不顯示 //A Row
                {
                    mtRow[iRow]->Core.SetCellNumber(ir, ic, HistroyBin[iRow][iCol][iLast]);
                    if (HistroyPassFail[iRow][iCol][iLast])
                        mtRow[iRow]->Core.SetCellColorIndex(ir, ic, 1);
                    else
                        mtRow[iRow]->Core.SetCellColorIndex(ir, ic, 2);
                }
                else
                {
                    mtRow[iRow]->Core.SetCellNumber(ir, ic, AnsiString(""));
                    mtRow[iRow]->Core.SetCellColorIndex(ir, ic, 0);
                }
            }
        }
    }
}

// =============================================================================
//  WriteContactKind -- golden :1301-1648
// =============================================================================
void TfObserver::WriteContactKind()
{
    // NOTE: iArm/iRow initialized to 0 here (golden leaves them uninitialized
    // at this point) purely to silence a -Wmaybe-uninitialized warning under
    // -Wall -Wextra; behaviourally inert -- every code path below that READS
    // iArm/iRow is reached only when IsNNMode()==NN_2Row/NN_1Row, the SAME
    // condition that WRITES them just above, in every one of golden's own
    // three near-identical blocks (rgContactCountKinds's Total/Kinds/% and
    // rgContactCountHistory's Total/Kinds/%). Verified by reading all 6
    // sub-blocks end to end this wave.
    int iArm = 0, iRow = 0;
    AnsiString Result = "";

    if (rgContactCountKinds->ItemIndex < 0)   //Steven 20210802 : 修正-1的狀況
        rgContactCountKinds->ItemIndex = 0;

    if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
    {
        if (rgContactCountKinds->ItemIndex < 2)
        {
            iArm = 1;
            iRow = rgContactCountKinds->ItemIndex;
        }
        else
        {
            iArm = 0;
            iRow = rgContactCountKinds->ItemIndex - 2;
        }
    }
    else if (IsNNMode() == NN_1Row)
    {
        if (rgContactCountKinds->ItemIndex == 0)
        {
            iArm = 1;
            iRow = 0;
        }
        else if (rgContactCountKinds->ItemIndex == 1)
        {
            iArm = 0;
            iRow = 0;
        }
    }

    if (rgContactCountKindsForm->ItemIndex == 0)   //Total
    {
        for (int i = 0; i < MAX_SOCKET_COL; i++)
        {
            if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
            }
            else if (TestIF.iTestMode == QualSite2X2N)
            {
                if (i < 2)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                    sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if (TestIF.iTestMode == _6Site2X3N)   //Steven 20220425 : 2X3NN Mode
            {
                if (i < 3)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                    sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if (TestIF.iTestMode == _8Site2X4N)   //Wei 20231211 : 2X4NN Mode
            {
                if (i < 4)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                    sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else
            {
                for (int j = 0; j < 2; j++)
                {
                    if (j == 0)
                        StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal());   //Arm1
                    else
                        StringGrid2->Cells[2 * i + 2][2] = AnsiString((long)ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal());   //Arm2

                    sCounterColKind[0][i] = AnsiString((long)(ArmData[0]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal() + ArmData[1]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal()));
                }
            }
        }
    }
    else if (rgContactCountKindsForm->ItemIndex == 1)   //Kinds
    {
        for (int i = 0; i < MAX_SOCKET_COL; i++)
        {
            if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
            }
            else if (TestIF.iTestMode == QualSite2X2N)
            {
                if (i < 2)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                    sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if (TestIF.iTestMode == _6Site2X3N)   //Steven 20220425 : 2X3NN Mode
            {
                if (i < 3)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                    sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if (TestIF.iTestMode == _8Site2X4N)   //Wei 20231211 : 2X4NN Mode
            {
                if (i < 4)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                    sCounterColKind[0][i] = AnsiString((long)ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else
            {
                for (int j = 0; j < 2; j++)
                {
                    if (j == 0)
                        StringGrid2->Cells[2 * i + 1][2] = AnsiString((long)ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT());   //Arm1
                    else
                        StringGrid2->Cells[2 * i + 2][2] = AnsiString((long)ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT());   //Arm2
                }
                sCounterColKind[0][i] = AnsiString((long)(ArmData[0]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT() + ArmData[1]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT()));
            }
        }
    }
    else   //%
    {
        for (int i = 0; i < MAX_SOCKET_COL; i++)
        {
            if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid2->Cells[2 * i + 1][2] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                sCounterColKind[0][i] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
            }
            else if (TestIF.iTestMode == QualSite2X2N)
            {
                if (i < 2)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                    sCounterColKind[0][i] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if (TestIF.iTestMode == _6Site2X3N)   //Steven 20220425 : 2X3NN Mode
            {
                if (i < 3)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                    sCounterColKind[0][i] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if (TestIF.iTestMode == _8Site2X4N)   //Wei 20231211 : 2X4NN Mode
            {
                if (i < 4)
                {
                    StringGrid2->Cells[2 * i + 1][2] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                    sCounterColKind[0][i] = AnsiString((double)ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else
            {
                for (int j = 0; j < 2; j++)
                {
                    if (j == 0)
                        StringGrid2->Cells[2 * i + 1][2] = Result.sprintf("%3.1f%%", ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA());   //Arm1
                    else
                        StringGrid2->Cells[2 * i + 2][2] = Result.sprintf("%3.1f%%", ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA());   //Arm2
                }
                sCounterColKind[0][i] = Result.sprintf("%3.1f%%", (ArmData[0]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA() + ArmData[1]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA()) / 2);
            }
        }
    }

    if (rgContactCountHistory->ItemIndex < 0)   //Steven 20210802 : 修正-1的狀況
        rgContactCountHistory->ItemIndex = 0;

    if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
    {
        if (rgContactCountHistory->ItemIndex < 2)
        {
            iArm = 1;
            iRow = rgContactCountHistory->ItemIndex;
        }
        else
        {
            iArm = 0;
            iRow = rgContactCountHistory->ItemIndex - 2;
        }
    }
    else if (IsNNMode() == NN_1Row)
    {
        if (rgContactCountHistory->ItemIndex == 0)
        {
            iArm = 1;
            iRow = 0;
        }
        else if (rgContactCountHistory->ItemIndex == 1)
        {
            iArm = 0;
            iRow = 0;
        }
    }

    //History
    if (rgContactCountHistoryForm->ItemIndex == 0)   //Total
    {
        for (int i = 0; i < MAX_SOCKET_COL; i++)
        {
            if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
            }
            else if (TestIF.iTestMode == QualSite2X2N)
            {
                if (i < 2)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                    sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if (TestIF.iTestMode == _6Site2X3N)   //Steven 20220425 : 2X3NN Mode
            {
                if (i < 3)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                    sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if (TestIF.iTestMode == _8Site2X4N)   //Wei 20231211 : 2X4NN Mode
            {
                if (i < 4)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                    sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else
            {
                for (int j = 0; j < 2; j++)
                {
                    if (j == 0)
                        StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal());   //Arm1
                    else
                        StringGrid3->Cells[2 * i + 2][2] = AnsiString((long)ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal());   //Arm2
                }
                sCounterColKind[1][i] = AnsiString((long)(ArmHistory[0]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal() + ArmHistory[1]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal()));
            }
        }
    }
    else if (rgContactCountHistoryForm->ItemIndex == 1)   //Kinds
    {
        for (int i = 0; i < MAX_SOCKET_COL; i++)
        {
            if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
            }
            else if (TestIF.iTestMode == QualSite2X2N)
            {
                if (i < 2)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                    sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if (TestIF.iTestMode == _6Site2X3N)   //Steven 20220425 : 2X3NN Mode
            {
                if (i < 3)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                    sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if (TestIF.iTestMode == _8Site2X4N)   //Wei 20231211 : 2X4NN Mode
            {
                if (i < 4)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                    sCounterColKind[1][i] = AnsiString((long)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else
            {
                for (int j = 0; j < 2; j++)
                {
                    if (j == 0)
                        StringGrid3->Cells[2 * i + 1][2] = AnsiString((long)ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT());   //Arm1
                    else
                        StringGrid3->Cells[2 * i + 2][2] = AnsiString((long)ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT());   //Arm2
                }
                sCounterColKind[1][i] = AnsiString((long)(ArmHistory[0]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT() + ArmHistory[1]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT()));
            }
        }
    }
    else   //%
    {
        for (int i = 0; i < MAX_SOCKET_COL; i++)
        {
            if (IsNNMode() == NN_2Row)   //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid3->Cells[2 * i + 1][2] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                sCounterColKind[1][i] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
            }
            else if (TestIF.iTestMode == QualSite2X2N)
            {
                if (i < 2)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                    sCounterColKind[1][i] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if (TestIF.iTestMode == _6Site2X3N)   //Steven 20220425 : 2X3NN Mode
            {
                if (i < 3)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                    sCounterColKind[1][i] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if (TestIF.iTestMode == _8Site2X4N)   //Wei 20231211 : 2X4NN Mode
            {
                if (i < 4)
                {
                    StringGrid3->Cells[2 * i + 1][2] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                    sCounterColKind[1][i] = AnsiString((double)ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else
            {
                for (int j = 0; j < 2; j++)
                {
                    if (j == 0)
                        StringGrid3->Cells[2 * i + 1][2] = Result.sprintf("%3.1f%%", ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA());   //Arm1
                    else
                        StringGrid3->Cells[2 * i + 2][2] = Result.sprintf("%3.1f%%", ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA());   //Arm2
                }
                sCounterColKind[1][i] = Result.sprintf("%3.1f%%", (ArmHistory[0]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA() + ArmHistory[1]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA()) / 2);
            }
        }
    }
}

// =============================================================================
//  rgContactCountKinds/History/Form ClickX4 -- golden :3219-3241
// =============================================================================
void TfObserver::rgContactCountKindsClick(void * /*Sender*/)
{
    WriteContactKind();
    StringGrid2->Repaint();
}

void TfObserver::rgContactCountHistoryClick(void * /*Sender*/)
{
    WriteContactKind();
    StringGrid3->Repaint();
}

void TfObserver::rgContactCountKindsFormClick(void * /*Sender*/)
{
    WriteContactKind();
    StringGrid2->Refresh();
}

void TfObserver::rgContactCountHistoryFormClick(void * /*Sender*/)
{
    WriteContactKind();
    StringGrid3->Refresh();
}

// =============================================================================
//  CalculateStopTime -- golden :1686-1707
//  DEVIATION: public+static, golden private+instance -- see forms/fObserver.h
// =============================================================================
AnsiString TfObserver::CalculateStopTime(int Sec)
{
    int i_mm;
    AnsiString AS_Time, ss, mm, hh;

    ss = Sec % 60;
    i_mm = Sec / 60;
    if (i_mm > 60)
    {
        mm = i_mm % 60;
        hh = i_mm / 60;
        if (mm.Length() == 1) mm = "0" + mm;
        AS_Time = hh + ":" + mm + ":" + ss;
    }
    else
    {
        mm = i_mm;
        if (mm.Length() == 1) mm = "0" + mm;
        AS_Time = "0:" + mm + ":" + ss;
    }
    return AS_Time;
}

// =============================================================================
//  UnCalculateStopTime -- golden :2237-2253
//  DEVIATION: public+static, golden private+instance -- see forms/fObserver.h
//
//  NOTE (recon judgment, kept verbatim): the final `i_s=atoi(asSubBuffer1.
//  SubString(1, iPos)...)` line reuses `iPos` from the PRECEDING SubString
//  call (asSubBuffer.Pos(":"), the position of the mm/ss separator) rather
//  than recomputing asSubBuffer1.Pos(":"). This reads like a stale-variable
//  bug on first glance, but is not one for any input CalculateStopTime itself
//  produces: mm is always exactly 2 digits (zero-padded above), so that
//  colon's 1-based position is always 3 regardless of how many digits hh has;
//  SubString(1,3) on the (<=2-char) seconds remainder simply clips to the
//  string's actual length (vclcompat::AnsiString::SubString, matching real
//  BCB6 behaviour) rather than reading garbage, so atoi() still parses the
//  correct value. Traced by hand for "0:05:23" and "12:05:5" this wave.
//  Translated verbatim (not "fixed") per project policy.
// =============================================================================
int TfObserver::UnCalculateStopTime(AnsiString Time)
{
    AnsiString asSubBuffer, asSubBuffer1;
    int i_h, i_m, i_s, i_Time, iPos;
    if (Time == "") return 0;

    iPos = Time.Pos(":");
    i_h = atoi(Time.SubString(1, iPos).c_str()) * 60 * 60;
    asSubBuffer = Time.SubString(iPos + 1, Time.Length());
    iPos = asSubBuffer.Pos(":");
    i_m = atoi(asSubBuffer.SubString(1, iPos).c_str()) * 60;
    asSubBuffer1 = asSubBuffer.SubString(iPos + 1, Time.Length());
    i_s = atoi(asSubBuffer1.SubString(1, iPos).c_str());
    i_Time = i_h + i_m + i_s;
    return i_Time;
}

// =============================================================================
//  lstEventLogClick / cbbMonthChange / GetEventLogText / btnQueryEventLogTxtClick
//  -- golden :3757-3762 / :3763-3799 / :3801-3971 / :4423-4426
// =============================================================================
void TfObserver::lstEventLogClick(void * /*Sender*/)
{
    cbbFilter->ItemIndex = 0;
    GetEventLogText();
}

void TfObserver::cbbMonthChange(void * /*Sender*/)
{
    AnsiString sBeginPath = "";
    TStringList *tsFileName;
    tsFileName = new TStringList();
    tsFileName->Clear();
    lstEventLog->Clear();
    // AI(W906-FW3-Observer-W1) 20260818: root literal is call-time
    // getenv-overridable, same W906_BINCOUNT_PATH precedent (csystem.cpp:
    // 30984, established 20260817) -- lets a test point this at a scratch
    // dir instead of the real D:\HT9045_Log tree. Production (env unset)
    // keeps golden's own literal.
    AnsiString eventLogRoot = getenv("W906_EVENTLOG_ROOT") ? AnsiString(getenv("W906_EVENTLOG_ROOT"))
                                                            : AnsiString("D:\\HT9045_Log\\EventLogTxt");
    sBeginPath.sprintf("%s\\%s\\%s\\", eventLogRoot, cbbEventLogYear->Text, cbbMonth->Text);
    SearchFileAll(sBeginPath, "*.CSV", tsFileName);

    AnsiString s = "";
    for (int i = 0; i < tsFileName->Count; i++)
    {
        if (CUSTOMER_CODE == CC_SINOICTECH)   //otis 20221128 隱藏EventLogTxt儲存路徑
        {
            s = AnsiString(tsFileName->Strings[i]).SubString(35, 20);   // Strings[i] is a StringsProxy (operator AnsiString() only) -- explicit convert before .SubString()
            lstEventLog->Items->Add(s);
        }
        else
        {
            lstEventLog->Items->Add(tsFileName->Strings[i]);
        }
    }

    if (tsFileName->Count >= 1)   //JerryYang 20171101 (wei) 改成>=1
    {
        lstEventLog->ItemIndex = tsFileName->Count - 1;
    }
    else
    {
        lstEventLog->ItemIndex = -1;
    }
    cbbFilter->ItemIndex = 0;
    GetEventLogText();
    tsFileName->Clear();
    delete tsFileName;
}

void TfObserver::GetEventLogText()
{
    int x = 1, iJamCol = 0;
    AnsiString Str;
    for (int i = 1; i < strngrdEventLog->RowCount; i++)
    {
        strngrdEventLog->ClearRow(i);   // golden ->Rows[i]->Clear() -- see forms/fObserver.h DESIGN NOTES
    }

    if (lstEventLog->Items->Count < 1)   // golden ->Count (TListBox shortcut for ->Items->Count -- vclcompat::TListBox has no ->Count of its own)
    {
        strngrdEventLog->RowCount = 2;
        strngrdEventLog->Cells[1][1] = "No Record!!";
    }
    else
    {
        AnsiString FileName = lstEventLog->Items->Strings[lstEventLog->ItemIndex];
        AnsiString eventLogRoot = getenv("W906_EVENTLOG_ROOT") ? AnsiString(getenv("W906_EVENTLOG_ROOT"))
                                                                : AnsiString("D:\\HT9045_Log\\EventLogTxt");
        if (CUSTOMER_CODE == CC_SINOICTECH)   //otis 20221128 將隱藏的EventLogTxt儲存路徑還原，才能讀取檔案
        {
            AnsiString S = "";
            S.sprintf("%s\\%s\\%s\\%s.csv", eventLogRoot, cbbEventLogYear->Text, cbbMonth->Text, FileName);
            FileName = S;
        }

        if (FileExists(FileName))
        {
            TStringList *tsLogFile, *tsRow;
            tsLogFile = new TStringList();
            tsRow     = new TStringList();
            tsLogFile->LoadFromFile(FileName);
            if (tsLogFile->Count > 10000)   //JerryYang 20191115 event log超過1萬行不要顯示，避免顯示在string grid會發生記憶體不夠用的情況
            {
                strngrdEventLog->RowCount = 2;
                strngrdEventLog->Cells[1][1] = "Event log over 10000 rows!!";
                tsLogFile->Clear();
                tsRow->Clear();
                delete tsLogFile;
                delete tsRow;
                return;
            }

            if (cbbFilter->ItemIndex == 0)
            {
                strngrdEventLog->RowCount = tsLogFile->Count;
                for (int i = 0; i < tsLogFile->Count; i++)
                {
                    tsRow->Clear();
                    tsRow->CommaText = tsLogFile->Strings[i];

                    if (IniConfig.bSPILFunction == true)   //Steven 20240604 : SPIL格式的event log
                    {
                        if (i > 0)
                            strngrdEventLog->Cells[0][i] = i;

                        for (int j = 0; j < tsRow->Count; j++)
                        {
                            if (i == 0 && j == 0)
                                strngrdEventLog->Cells[0][0] = "No.";
                            else
                                strngrdEventLog->Cells[j + 1][i] = tsRow->Strings[j];
                        }
                    }
                    else
                    {
                        for (int j = 0; j < tsRow->Count; j++)
                        {
                            strngrdEventLog->Cells[j][i] = tsRow->Strings[j];
                        }
                    }
                }
            }
            else if (cbbFilter->Text.AnsiPos("JAM only") != 0 ||   //JAM only
                     cbbFilter->Text.AnsiPos("WAR only") != 0 ||   //WAR only
                     cbbFilter->Text.AnsiPos("MES only") != 0)     //MES only
            {
                if (cbbFilter->Text.AnsiPos("JAM only") != 0)   //Steven 20250422 : Add filter for event log
                {
                    Str = "JAM";
                }
                else if (cbbFilter->Text.AnsiPos("WAR only") != 0)
                {
                    Str = "WAR";
                }
                else   //if(cbbFilter->Text.AnsiPos("MES only"))
                {
                    Str = "MES";
                }
                strngrdEventLog->RowCount = 2;
                for (int i = 1; i < tsLogFile->Count; i++)
                {
                    tsRow->Clear();
                    tsRow->CommaText = tsLogFile->Strings[i];
                    if (tsRow->Count > 3)
                    {
                        if (IniConfig.bSPILFunction == true)   //JerryYang 20250428 : fix SPIL event log
                        {
                            iJamCol = 1;
                        }
                        else
                        {
                            iJamCol = 3;
                        }

                        if (AnsiString(tsRow->Strings[iJamCol]).AnsiPos(Str) == 1)   // Strings[] is a StringsProxy -- explicit convert before .AnsiPos()
                        {
                            strngrdEventLog->RowCount = strngrdEventLog->RowCount + 1;   // RowCountProxy has no operator++
                            for (int j = 0; j < tsRow->Count; j++)
                            {
                                strngrdEventLog->Cells[j][x] = tsRow->Strings[j];
                            }
                            x++;
                        }
                    }
                }

                if (strngrdEventLog->RowCount == 2)
                {
                    strngrdEventLog->Cells[1][1] = "No Record!!";
                }
            }
            else
            {
                strngrdEventLog->RowCount = 2;
                for (int i = 1; i < tsLogFile->Count; i++)
                {
                    tsRow->Clear();
                    tsRow->CommaText = tsLogFile->Strings[i];
                    if (tsRow->Count > 2)
                    {
                        if (IniConfig.bSPILFunction == true)   //JerryYang 20250428 : fix SPIL event log
                        {
                            iJamCol = 0;
                        }
                        else
                        {
                            iJamCol = 2;
                        }

                        if (AnsiString(tsRow->Strings[iJamCol]).AnsiPos(cbbFilter->Text) != 0)   // Strings[] is a StringsProxy -- explicit convert before .AnsiPos()
                        {
                            strngrdEventLog->RowCount = strngrdEventLog->RowCount + 1;   // RowCountProxy has no operator++
                            for (int j = 0; j < tsRow->Count; j++)
                            {
                                strngrdEventLog->Cells[j][x] = tsRow->Strings[j];
                            }
                            x++;
                        }
                    }
                }

                if (strngrdEventLog->RowCount == 2)
                {
                    strngrdEventLog->Cells[1][1] = "No Record!!";
                }
            }

            if (strngrdEventLog->RowCount > 2)   //Jou 20220117 : foxed for evelt log reader
                strngrdEventLog->FixedRows = 1;

            tsLogFile->Clear();
            tsRow->Clear();
            delete tsLogFile;
            delete tsRow;
        }
        else
        {
            strngrdEventLog->RowCount = 2;
            strngrdEventLog->Cells[1][1] = "No Record!!";
        }
    }
}

void TfObserver::btnQueryEventLogTxtClick(void * /*Sender*/)
{
    GetEventLogText();
}

// =============================================================================
//  btReportClick -- golden :3186-3206
// =============================================================================
void TfObserver::btReportClick(void * /*Sender*/)
{
    if (cbDisplayData->Text == "Event Log")
    {
        strngrdMDBQuery->ColCount = 10;
        strngrdMDBQuery->ColWidths[0] = 33;    //Index
        strngrdMDBQuery->ColWidths[1] = 90;    //UnitName
        strngrdMDBQuery->ColWidths[2] = 75;    //AlarmCode
        strngrdMDBQuery->ColWidths[3] = 100;   //DateTime
        strngrdMDBQuery->ColWidths[4] = 50;    //Recovery
        strngrdMDBQuery->ColWidths[5] = 50;    //StopTime
        strngrdMDBQuery->ColWidths[6] = 50;    //Duplicate
        strngrdMDBQuery->ColWidths[7] = 390;   //Message
        strngrdMDBQuery->ColWidths[8] = 100;   //Duplicate
        strngrdMDBQuery->ColWidths[9] = 100;   //Message
        strngrdMDBQuery->Cells[0][0] = "No.";
        strngrdMDBQuery->Cells[0][8] = "Jam Stop time";
        strngrdMDBQuery->Cells[0][9] = "Operation Stop time";
    }
}

// =============================================================================
//  BuildWhereQuery_OccurDateTimeRange / BuildQuery_AlarmHistory -- PORT-ONLY
//  testability extractions out of BtnQueryClick (golden :2551-2556 /
//  :2568-2569). See forms/fObserver.h's banner on these two declarations for
//  why (BtnQueryClick's real SQL calls need a live SQLite handle this test
//  wave will not open). Pure string assembly, no DB access, identical output
//  to inlining the same sprintf() -- BtnQueryClick below calls these instead
//  of repeating the format strings.
// =============================================================================
AnsiString TfObserver::BuildWhereQuery_OccurDateTimeRange(TDateTime d1, TDateTime t1, TDateTime d2, TDateTime t2)
{
    AnsiString WhereQuery;
    WhereQuery.sprintf(" WHERE (OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s')",
            FormatDateTime("yyyy-mm-dd", d1), FormatDateTime("hh:nn:ss", t1),
            FormatDateTime("yyyy-mm-dd", d2), FormatDateTime("hh:nn:ss", t2));
    return WhereQuery;
}

AnsiString TfObserver::BuildQuery_AlarmHistory(AnsiString WhereQuery)
{
    AnsiString asQuery;
    asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s ORDER BY OccurDateTime DESC", WhereQuery);
    return asQuery;
}

// =============================================================================
//  BtnQueryClick -- golden :2421-2776
// =============================================================================
void TfObserver::BtnQueryClick(void * /*Sender*/)   //查詢
{
    AnsiString asQuery, WhereQuery;

    if (cbDisplayData->ItemIndex < Jam_Chart_Summary)
    {
        strngrdMDBQuery->Visible = true;
        Chart2->Visible = false;

        for (int i = 0; i < strngrdMDBQuery->RowCount; i++)   //Steven 20120222 : 要把多的欄位清空
        {
            strngrdMDBQuery->ClearRow(i);
        }

        if (cbDisplayData->ItemIndex == Event_Log)   //EventLog
        {
            strngrdMDBQuery->ColCount = 9;
            strngrdMDBQuery->ColWidths[0] = 33;    //Index
            strngrdMDBQuery->ColWidths[1] = 60;    //UnitName
            strngrdMDBQuery->ColWidths[2] = 60;    //AlarmCode
            strngrdMDBQuery->ColWidths[3] = 110;   //DateTime
            strngrdMDBQuery->ColWidths[4] = 50;    //Recovery
            strngrdMDBQuery->ColWidths[5] = 50;    //StopTime
            strngrdMDBQuery->ColWidths[6] = 50;    //Duplicate
            strngrdMDBQuery->ColWidths[7] = 390;   //Message
            strngrdMDBQuery->ColWidths[8] = 390;   //ErrPart
            strngrdMDBQuery->Cells[0][0] = "No.";
        }
        else if (cbDisplayData->ItemIndex == Production_Record)   //Steven 20140819 : Production View
        {
            strngrdMDBQuery->ColCount = 26;
            for (int i = 0; i < strngrdMDBQuery->ColCount; i++)
            {
                strngrdMDBQuery->ColWidths[i] = 50;
            }
        }
        else if (cbDisplayData->ItemIndex > Event_Log &&
                 cbDisplayData->ItemIndex < Alarm_History)
        {
            strngrdMDBQuery->ColCount = 3;
            strngrdMDBQuery->DefaultColWidth = 260;
        }
        else if (cbDisplayData->ItemIndex >= Alarm_History &&
                 cbDisplayData->ItemIndex <= Alarm_History_WAR)   //Alarm History
        {
            strngrdMDBQuery->ColCount = 11;
            strngrdMDBQuery->DefaultColWidth = 70;
            strngrdMDBQuery->ColWidths[0] = 20;    //Index
            strngrdMDBQuery->ColWidths[1] = 50;    //AlarmCode
            strngrdMDBQuery->ColWidths[2] = 70;    //UnitName
            strngrdMDBQuery->ColWidths[3] = 250;   //Message
            strngrdMDBQuery->ColWidths[4] = 70;    //ErrPart
            strngrdMDBQuery->ColWidths[5] = 55;    //Recovery
            strngrdMDBQuery->ColWidths[6] = 40;    //Duplicate
            strngrdMDBQuery->ColWidths[7] = 60;    //Date
            strngrdMDBQuery->ColWidths[8] = 50;    //Time
            strngrdMDBQuery->ColWidths[9] = 50;    //StopTime
            strngrdMDBQuery->ColWidths[10] = 40;   //Temperature
        }
        else if (cbDisplayData->ItemIndex == Alarm_Statistics)
        {
            strngrdMDBQuery->ColCount = 8;
            strngrdMDBQuery->ColWidths[0] = 33;   //Index
            strngrdMDBQuery->ColWidths[1] = 90;   //UnitName
            strngrdMDBQuery->ColWidths[2] = 75;   //AlarmCode
            strngrdMDBQuery->ColWidths[3] = 390;  //Message
            strngrdMDBQuery->ColWidths[4] = 50;   //Count
            strngrdMDBQuery->ColWidths[5] = 50;   //Rate
            strngrdMDBQuery->ColWidths[6] = 75;   //StopTime
            strngrdMDBQuery->ColWidths[7] = 75;   //MTTR
            strngrdMDBQuery->Cells[0][0] = "No.";
        }
        else if (cbDisplayData->ItemIndex == Alarm_Stat_Day)
        {
            strngrdMDBQuery->ColCount = 9;
            strngrdMDBQuery->ColWidths[0] = 33;   //Index
            strngrdMDBQuery->ColWidths[1] = 60;   //Date
            strngrdMDBQuery->ColWidths[2] = 90;   //UnitName
            strngrdMDBQuery->ColWidths[3] = 75;   //AlarmCode
            strngrdMDBQuery->ColWidths[4] = 330;  //Message
            strngrdMDBQuery->ColWidths[5] = 50;   //Count
            strngrdMDBQuery->ColWidths[6] = 50;   //Rate
            strngrdMDBQuery->ColWidths[7] = 75;   //StopTime
            strngrdMDBQuery->ColWidths[8] = 75;   //MTTR
            strngrdMDBQuery->Cells[0][0] = "No.";
        }
        else if (cbDisplayData->ItemIndex == Lot_Infomation)
        {
            strngrdMDBQuery->ColCount = 7;
            strngrdMDBQuery->DefaultColWidth = 100;
        }

        strngrdMDBQuery->Cells[0][0] = "No.";
        // DateSeparator='-'; dropped (inert) -- see file-head DESIGN NOTE.
        if (cbDisplayData->ItemIndex == Lot_Infomation)
        {
            WhereQuery.sprintf(" WHERE (StartTime >= '%s %s' AND EndTime <= '%s %s')",
                        FormatDateTime("yyyy-mm-dd", DateTimePicker1->Date), FormatDateTime("hh:nn:ss", DateTimePicker2->DateTime),
                        FormatDateTime("yyyy-mm-dd", DateTimePicker3->Date), FormatDateTime("hh:nn:ss", DateTimePicker4->DateTime));
        }
        else if (cbDisplayData->ItemIndex == Alarm_Statistics ||
                 cbDisplayData->ItemIndex == Alarm_History_JAM ||
                 cbDisplayData->ItemIndex == Alarm_Stat_Day)
        {   //Steven 20110810 Start : Optimizing queries
            if (IniConfig.bSPILFunction == true)   //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
            {   //20111110 Dell
                WhereQuery.sprintf(" WHERE ((OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s') "
                            "  AND (AlarmCode > 'JAM01' AND AlarmCode < 'JAM99') "
                           " AND (Duplicate=0 OR Duplicate=2)) ",
                        FormatDateTime("yyyy-mm-dd", DateTimePicker1->Date), FormatDateTime("hh:nn:ss", DateTimePicker2->DateTime),
                        FormatDateTime("yyyy-mm-dd", DateTimePicker3->Date), FormatDateTime("hh:nn:ss", DateTimePicker4->DateTime));
            }
            else
            {
                WhereQuery.sprintf(" WHERE ((OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s') "
                            "  AND (AlarmCode > 'JAM01' AND AlarmCode < 'JAM99') "
                           " AND Duplicate=0) ",
                        FormatDateTime("yyyy-mm-dd", DateTimePicker1->Date), FormatDateTime("hh:nn:ss", DateTimePicker2->DateTime),
                        FormatDateTime("yyyy-mm-dd", DateTimePicker3->Date), FormatDateTime("hh:nn:ss", DateTimePicker4->DateTime));
            }
            //Steven 20110810 End
        }
        else
        {
            // PORT-ONLY extraction, identical output -- see forms/fObserver.h's
            // BuildWhereQuery_OccurDateTimeRange banner.
            WhereQuery = BuildWhereQuery_OccurDateTimeRange(DateTimePicker1->Date, DateTimePicker2->DateTime,
                                                              DateTimePicker3->Date, DateTimePicker4->DateTime);
        }

        if (cbDisplayData->ItemIndex == Event_Log)
            asQuery.sprintf("SELECT * FROM EventLogView %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if (cbDisplayData->ItemIndex == Process_Record)
            asQuery.sprintf("SELECT Process, OccurDateTime FROM Process %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if (cbDisplayData->ItemIndex == Message_Record)
            asQuery.sprintf("SELECT Message, OccurDateTime FROM Message %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if (cbDisplayData->ItemIndex == Motion_Record)
            asQuery.sprintf("SELECT Motion,  OccurDateTime FROM Motion  %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if (cbDisplayData->ItemIndex == Production_Record)   //Steven 20140819 : Production View
            asQuery.sprintf("SELECT * FROM ProductionView %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if (cbDisplayData->ItemIndex == Alarm_History)
            // PORT-ONLY extraction, identical output -- see forms/fObserver.h's
            // BuildQuery_AlarmHistory banner.
            asQuery = BuildQuery_AlarmHistory(WhereQuery);
        else if (cbDisplayData->ItemIndex == Alarm_History_JAM)
            asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if (cbDisplayData->ItemIndex == Alarm_History_MES)
            asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s AND (AlarmCode > 'M' AND AlarmCode < 'N') ORDER BY OccurDateTime DESC", WhereQuery);   //Steven 20110810 Start : Optimizing queries
        else if (cbDisplayData->ItemIndex == Alarm_History_WAR)
            asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s AND (AlarmCode > 'W' AND AlarmCode < 'X') ORDER BY OccurDateTime DESC", WhereQuery);   //Steven 20110810 Start : Optimizing queries
        else if (cbDisplayData->ItemIndex == Alarm_Statistics)
        {
            asQuery.sprintf(
                    "SELECT                                                                                     "
                    "      UnitName.UnitName,                                                                   "
                    "      AlarmList.AlarmCode,                                                                 "
                    "      (AlarmList.Message || ' ' ||  MotorAlarmList.MotMess) Message,                       "
                    "      Count(AlarmList.AlarmCode) Count,                                                    "
                    "      Sum(EventLog.StopedTime) StopTime,                                                   "
                    "      AVG(EventLog.StopedTime) MTTR                                                        "
                    "FROM                                                                                       "
                    "      EventLog                                                                             "
                    "      INNER JOIN AlarmList ON (EventLog.ID_AlarmList = AlarmList.ID_AlarmList)             "
                    "      INNER JOIN MotorAlarmList ON (EventLog.ID_MotorList = MotorAlarmList.ID_MotorList)   "
                    "      INNER JOIN UnitName ON (AlarmList.UnitNo = UnitName.ID_UnitNo)                       "
                    "%s                                                                                         "
                    "GROUP BY                                                                                   "
                    "       AlarmCode                                                                           "
                    "ORDER BY                                                                                   "
                    "      Count DESC                                                                           ", WhereQuery);
        }
        else if (cbDisplayData->ItemIndex == Alarm_Stat_Day)   //根據日期作Group
        {
            asQuery.sprintf(
                    "SELECT                                                                                     "
                    "      Date(EventLog.OccurDateTime) Date,                                                    "
                    "      UnitName.UnitName,                                                                   "
                    "      AlarmList.AlarmCode,                                                                 "
                    "      (AlarmList.Message || ' ' ||  MotorAlarmList.MotMess) Message,                       "
                    "      Count(AlarmList.AlarmCode) Count,                                                    "
                    "      Sum(EventLog.StopedTime) StopTime,                                                   "
                    "      AVG(EventLog.StopedTime) MTTR                                                        "
                    "FROM                                                                                       "
                    "      EventLog                                                                             "
                    "      INNER JOIN AlarmList ON (EventLog.ID_AlarmList = AlarmList.ID_AlarmList)             "
                    "      INNER JOIN MotorAlarmList ON (EventLog.ID_MotorList = MotorAlarmList.ID_MotorList)   "
                    "      INNER JOIN UnitName ON (AlarmList.UnitNo = UnitName.ID_UnitNo)                       "
                    "%s                                                                                         "
                    "GROUP BY                                                                                   "
                    "       AlarmCode, Date(OccurDateTime)                                                      "
                    "ORDER BY                                                                                   "
                    "      Date DESC, Count DESC                                                                ", WhereQuery);
        }
        else if (cbDisplayData->ItemIndex == Lot_Infomation)
        {
            asQuery.sprintf("SELECT * FROM CountPerLot %s ORDER BY StartTime DESC", WhereQuery);
        }

        if (cbDisplayData->ItemIndex == Alarm_Statistics ||
            cbDisplayData->ItemIndex == Alarm_Stat_Day)
        {
            pnlTotalCount->Caption = MyDBVEventFreq(asQuery, strngrdMDBQuery);
        }
        else
        {
            if (bFilterTheAgainData && cbDisplayData->ItemIndex == Event_Log)   //Steven 20120222 : 過濾掉Duplicate的訊息
                pnlTotalCount->Caption = MyDBVProcessFilter(asQuery, strngrdMDBQuery);
            else
                pnlTotalCount->Caption = MyDBVProcess(asQuery, strngrdMDBQuery);
        }
        strngrdMDBQuery->RowCount = (strngrdMDBQuery->RowCount + 1);
        for (int i = 0; i < strngrdMDBQuery->ColCount; i++)
            strngrdMDBQuery->Cells[i][strngrdMDBQuery->RowCount - 1] = "";
        // DateSeparator='-'; dropped (inert) -- see file-head DESIGN NOTE.
        strngrdMDBQuery->Cells[1][strngrdMDBQuery->RowCount - 1] = FormatDateTime("yyyy-mm-dd", DateTimePicker1->Date) + " " + FormatDateTime("hh:nn:ss", DateTimePicker2->DateTime);
        strngrdMDBQuery->Cells[2][strngrdMDBQuery->RowCount - 1] = FormatDateTime("yyyy-mm-dd", DateTimePicker3->Date) + " " + FormatDateTime("hh:nn:ss", DateTimePicker4->DateTime);
        strngrdMDBQuery->Cells[0][strngrdMDBQuery->RowCount - 1] = "Data Period";
    }
    else if (cbDisplayData->ItemIndex == Alarm_Code_List)   //Steven 20150204 : 新增Alarm Code List 查詢
    {
        strngrdMDBQuery->Visible = true;
        Chart2->Visible = false;

        for (int i = 0; i < strngrdMDBQuery->RowCount; i++)   //Steven 20120222 : 要把多的欄位清空
        {
            strngrdMDBQuery->ClearRow(i);
        }

        strngrdMDBQuery->ColCount = 7;
        strngrdMDBQuery->ColWidths[1] = 100;   //Unit Name
        strngrdMDBQuery->ColWidths[2] = 100;   //Alarm Code
        strngrdMDBQuery->ColWidths[3] = 400;   //Message
        strngrdMDBQuery->ColWidths[4] = 100;   //Level
        strngrdMDBQuery->ColWidths[5] = 100;   //Red Background
        strngrdMDBQuery->ColWidths[6] = 100;   //Silent Mode

        asQuery.sprintf("SELECT UnitName.UnitName, AlarmCode, Message FROM AlarmList INNER JOIN UnitName ON (AlarmList.UnitNo = UnitName.ID_UnitNo) ORDER by AlarmList.UnitNo, ID_AlarmList");
        MyDBVProcess(asQuery, strngrdMDBQuery);
        strngrdMDBQuery->Cells[4][0] = "Level";
        strngrdMDBQuery->Cells[5][0] = "Red Background";
        strngrdMDBQuery->Cells[6][0] = "Silent Mode";

        // AI(W906-FW3-Observer-W1) 20260818: GATED -- golden :2668-2684, the
        // per-row Level/Silent/RedBackground lookup loop. Needs BOTH
        // `fMain->AlarmUnitMap` (grepped forms/fMain.h in full this wave: 0
        // hits, no such member) AND a `fSecurity` facade (grepped forms/*.h:
        // no fSecurity.h exists anywhere in the tree). Two independent
        // missing dependencies -- see forms/fObserver.h GATE REGISTER (1).
        // Columns 4-6 are simply left at whatever MyDBVProcess populated
        // (blank), the faithful "no lookup happened" default.
#if 0
        int iLevel;
        bool bSilent, bRedBG;
        AnsiString UnitName;
        for (int i = 2; i < strngrdMDBQuery->RowCount; i++)   //Steven 20161124 : Add display level set on Jam Code
        {
            UnitName = fMain->AlarmUnitMap[strngrdMDBQuery->Cells[1][i]];   //Steven 20231127 : 整理Alarm Unit
            iLevel   = fSecurity->GetJamLevel(UnitName, strngrdMDBQuery->Cells[2][i]);
            bSilent  = fSecurity->GetJemSilent(UnitName, strngrdMDBQuery->Cells[2][i]);
            bRedBG   = fSecurity->GetJemRed(UnitName, strngrdMDBQuery->Cells[2][i]);
            if (iLevel == 0)
                strngrdMDBQuery->Cells[4][i] = "Operator";
            else if (iLevel == 1)
                strngrdMDBQuery->Cells[4][i] = "Engineer";
            else if (iLevel == 2)
                strngrdMDBQuery->Cells[4][i] = "Supervisor";
            else
                strngrdMDBQuery->Cells[4][i] = "HonPrec";
            strngrdMDBQuery->Cells[5][i] = (bSilent) ? "V" : "";
            strngrdMDBQuery->Cells[6][i] = (bRedBG) ? "V" : "";
        }
#endif
    }
    else if (cbDisplayData->ItemIndex == Production_Summary_Report)   //Sam 20210107 : Summary Report fuction
    {
        DoProduction_Summary_Report(FormatDateTime("yyyy-mm-dd", DateTimePicker1->Date),
                                    FormatDateTime("hh:nn:ss", DateTimePicker2->DateTime),
                                    FormatDateTime("yyyy-mm-dd", DateTimePicker3->Date),
                                    FormatDateTime("hh:nn:ss", DateTimePicker4->DateTime));
    }
    else   //Jam Chart
    {
        strngrdMDBQuery->Visible = false;
        Chart2->Visible = true;
        AnsiString StartTime = "", EndTime = "";
        Chart2->Series[0]->Clear();
        // DateSeparator='-'; dropped (inert) -- see file-head DESIGN NOTE.
        StartTime = FormatDateTime("yyyy-mm-dd", DateTimePicker1->Date) + " " + FormatDateTime("hh:nn:ss", DateTimePicker2->DateTime);
        EndTime   = FormatDateTime("yyyy-mm-dd", DateTimePicker3->Date) + " " + FormatDateTime("hh:nn:ss", DateTimePicker4->DateTime);
        Chart2->Title.Text->Clear();
        Chart2->Title.Text->Add(cbDisplayData->Text);
        Chart2->Title.Text->Add("(" + StartTime + " ~ " + EndTime + ")");

        if (cbDisplayData->ItemIndex == Jam_Chart_Summary)
        {
            WhereQuery = " WHERE (AlarmList.AlarmCode>'J' AND (AlarmList.AlarmCode<'K') AND EventLog.Duplicate=0 AND OccurDateTime >= '" + StartTime + "' AND OccurDateTime <= '" + EndTime + "')";
            asQuery.sprintf("SELECT                                                                         \n"
                            "      ID_UnitNo No,                                                            \n"
                            "      UnitName.UnitName,                                                       \n"
                            "      Count(UnitName.UnitName) Count                                           \n"
                            "  FROM                                                                         \n"
                            "      AlarmList                                                                \n"
                            "      INNER JOIN UnitName ON (AlarmList.UnitNo      = UnitName.ID_UnitNo)      \n"
                            "      INNER JOIN EventLog ON (EventLog.ID_AlarmList = AlarmList.ID_AlarmList)  \n"
                            "  %s                                                                           \n"
                            "  GROUP BY                                                                     \n"
                            "      UnitName.UnitName                                                        \n"
                            "  ORDER BY                                                                     \n"
                            "      No ASC                                                                   ", WhereQuery);

            // AI(W906-FW3-Observer-W1) 20260818: GATED -- golden :2723
            // `pnlTotalCount->Caption=MyDBVUnitEventCount(asQuery, Chart2);`.
            // cMyDB.h keeps `class TChart;` a permanently opaque forward decl
            // (cMyDB.h:75) and cMyDB.cpp ALREADY gates this function's BODY
            // for that reason (cMyDB.h:112 "BODY GATED (no TChart port)").
            // Chart2 here is TfObserverChart* (this wave's own stand-in, a
            // DIFFERENT type from the opaque golden TChart*), so the call
            // cannot even bind -- see forms/fObserver.h GATE REGISTER (2).
#if 0
            pnlTotalCount->Caption = MyDBVUnitEventCount(asQuery, Chart2);
#endif
        }
        else if (cbDisplayData->ItemIndex == Jam_Chart_InArm)
        {
            // AI(W906-FW3-Observer-W1) 20260818: GATED, same TChart-opacity
            // reason as above -- golden :2726 `MyDBVAxleEventCount(100,190,
            // StartTime,EndTime,Chart2);` (InArm).
#if 0
            pnlTotalCount->Caption = MyDBVAxleEventCount(100, 190, StartTime, EndTime, Chart2);   //InArm
#endif
        }
        else if (cbDisplayData->ItemIndex == Jam_Chart_OutArm)
        {
            // AI(W906-FW3-Observer-W1) 20260818: GATED, same reason -- golden
            // :2728 `MyDBVAxleEventCount(200,290,...);` (OutArm).
#if 0
            pnlTotalCount->Caption = MyDBVAxleEventCount(200, 290, StartTime, EndTime, Chart2);   //OutArm
#endif
        }
        else if (cbDisplayData->ItemIndex == Jam_Chart_Index)
        {
            // AI(W906-FW3-Observer-W1) 20260818: GATED, same reason -- golden
            // :2730 `MyDBVAxleEventCount(300,390,...);` (Index).
#if 0
            pnlTotalCount->Caption = MyDBVAxleEventCount(300, 390, StartTime, EndTime, Chart2);   //Index
#endif
        }
        else if (cbDisplayData->ItemIndex == Jam_Chart_InShuttle)
        {
            // AI(W906-FW3-Observer-W1) 20260818: GATED, same reason -- golden
            // :2732 `MyDBVAxleEventCount(400,490,...);` (In Shuttle).
#if 0
            pnlTotalCount->Caption = MyDBVAxleEventCount(400, 490, StartTime, EndTime, Chart2);   //In Shuttle
#endif
        }
        else if (cbDisplayData->ItemIndex == Jam_Chart_OutShuttle)
        {
            // AI(W906-FW3-Observer-W1) 20260818: GATED, same reason -- golden
            // :2734 `MyDBVAxleEventCount(500,590,...);` (Out Shuttle).
#if 0
            pnlTotalCount->Caption = MyDBVAxleEventCount(500, 590, StartTime, EndTime, Chart2);   //Out Shuttle
#endif
        }
    }

    //Steven 20190714 : 數字不準, mark
    //==>
    if (CUSTOMER_CODE == CC_AMKOR_Philippines ||   //JerryYang 20230721 : add
        CUSTOMER_CODE == CC_Microchip_Phil)        //JerryYang 20200929 ATP Jayvee要求恢復MTBF顯示
    {
        strngrdMDBQuery->RowCount = strngrdMDBQuery->RowCount + 2;   //Steven 20101103 Start : 報表加上Total Loader與Total Count
        for (int i = 0; i < strngrdMDBQuery->ColCount; i++)   //Steven 20101121 : 要清空最後兩排
        {
            strngrdMDBQuery->Cells[i][strngrdMDBQuery->RowCount - 1] = "";
            strngrdMDBQuery->Cells[i][strngrdMDBQuery->RowCount - 2] = "";
        }

        strngrdMDBQuery->Cells[0][strngrdMDBQuery->RowCount - 2] = "Total Loader";
        strngrdMDBQuery->Cells[1][strngrdMDBQuery->RowCount - 2] = lbltTotalLoader->Caption;
        strngrdMDBQuery->Cells[0][strngrdMDBQuery->RowCount - 1] = "Total Count";
        strngrdMDBQuery->Cells[1][strngrdMDBQuery->RowCount - 1] = pnlTotalCount->Caption;
        //Steven 20101103 End

        AnsiString StartDateTime = FormatDateTime("yyyy-mm-dd", DateTimePicker1->Date) + " " + FormatDateTime("hh:nn:ss", DateTimePicker2->DateTime);
        AnsiString EndDateTime   = FormatDateTime("yyyy-mm-dd", DateTimePicker3->Date) + " " + FormatDateTime("hh:nn:ss", DateTimePicker4->DateTime);
        lbltTotalLoader->Caption = MyDBQTotalLoader(StartDateTime, EndDateTime);   //Steven 20101105

        if (cbDisplayData->ItemIndex < Jam_Chart_Summary)   //Steven 20120628 : 顯示機台稼動時間
        {
            strngrdMDBQuery->RowCount = strngrdMDBQuery->RowCount + 1;   // RowCountProxy has no operator++
            MyDBQTimeData(StartDateTime, EndDateTime, strngrdMDBQuery->RowCount, strngrdMDBQuery);
        }

        if (cbDisplayData->ItemIndex == Alarm_Statistics ||   //Steven 20170309 (wei) add MTBF(PauseTime/Jam)
            cbDisplayData->ItemIndex == Alarm_Stat_Day   ||
            cbDisplayData->ItemIndex == Alarm_History_JAM ||
            cbDisplayData->ItemIndex == Alarm_History)   //JerryYang 20180619 (wei) : 新增可自定義Jam code是否列入MTBF計算
        {
            CountMTBF();
        }
    }
    //<==
    //Steven 20190714 : 數字不準, mark
}

// =============================================================================
//  DoProduction_Summary_Report / CountMTBF -- GATE REGISTER (3): documented
//  no-op stubs. Both are golden TfObserver member functions whose bodies sit
//  outside every one of Wave 1's assigned golden byte ranges (see the
//  ABSENCE-CLAIM note at file head). Declared here only so BtnQueryClick's
//  two call sites (golden :2688-2691 and :2770) compile and are observably
//  reached -- same "documented GAP, not a silent drop" idiom as
//  forms/fMain.h's SetMainRunStartMode.
// =============================================================================
void TfObserver::DoProduction_Summary_Report(AnsiString /*asStartData*/, AnsiString /*asStartTime*/,
                                              AnsiString /*asEndData*/, AnsiString /*asEndTime*/)
{
    // GAP: golden body lives outside Wave 1's byte ranges -- no-op offline.
}

// AI(W906-FW3-Observer-W2) 20260818: Wave 1's no-op stub retired -- real body
// below (golden :3683-3752). See the FW3-Obs2 APPEND banner further down this
// file for the full Wave 2 scope/GATE register; this is the ONE authorized
// exception to "append-only" for this wave (task brief: "把 stub 換成真本體算
// append-only 的唯一例外"), everything else this wave adds is appended after
// the file's previous end (originally right after this function).
void TfObserver::CountMTBF()                                                    //Steven 20170309 (wei) add MTBF(PauseTime/Jam)
{
    AnsiString asMTBA, asMUBA;
    int iMins, iUnit, iSGCount, iUPH, iMTBFTime = 0;                             //Isaac 20180418 add iMTBFTime  //JerryYang 20180518 : add
    double dJamCount, dLoadingCount, dTime;

    dJamCount     = atof(pnlTotalCount->Caption.c_str());
    dLoadingCount = atof(lbltTotalLoader->Caption.c_str());

    if (dJamCount == 0)                                                         //Alick 20161130 add MTBF(PauseTime/Jam)
    {
        asMTBA.sprintf("0 / 1");
    }
    else
    {
        iMTBFTime = iPauseTime + iProductTime + iJamTime;                       //Isaac 20180418 add iMTBFTime
        if (iMTBFTime != 0)
        {
            iMins = iMTBFTime / 60;                                             //JerryYang 20180619 : ASE_CL 尚智要求主MTBF單位改為min
            iMins = iMins / dJamCount;                                          //Isaac 20180417 (Steven) 修正MTBF公式(pause+production+jam)/jamcount
            asMTBA.sprintf("1 / %d", iMins);
        }
        else
        {
            asMTBA.sprintf("%d / 0", (int)dJamCount);
        }
    }

    if (dJamCount == 0)                                                         //Alick 20161130 add MUBF(LoadingCount/Jam)
    {
        asMUBA.sprintf("0 / 1");
    }
    else
    {
        if (dLoadingCount != 0)
        {
            iUnit = dLoadingCount / dJamCount;
            asMUBA.sprintf("1 / %d", iUnit);
        }
        else
        {
            asMUBA.sprintf("%d / 0", (int)dJamCount);
        }
    }

    dTime = double(DateTimePicker3->Date - DateTimePicker1->Date + DateTimePicker4->Time - DateTimePicker2->Time) * 24;
    if (dTime == 0)
    {
        iUPH = dLoadingCount;
    }
    else
    {
        iUPH = dLoadingCount / dTime;
    }

    iSGCount = strngrdMDBQuery->RowCount;
    strngrdMDBQuery->RowCount = iSGCount + 7;

    strngrdMDBQuery->Cells[1][iSGCount + 2] = "MTBA";
    strngrdMDBQuery->Cells[2][iSGCount + 2] = asMTBA;
    strngrdMDBQuery->Cells[3][iSGCount + 2] = "[mins]";                         //JerryYang 20180619 (wei) : ASE_CL 尚智要求主MTBF單位改為min

    strngrdMDBQuery->Cells[1][iSGCount + 3] = "MUBA";
    strngrdMDBQuery->Cells[2][iSGCount + 3] = asMUBA;
    strngrdMDBQuery->Cells[3][iSGCount + 3] = "[unit]";

    strngrdMDBQuery->Cells[1][iSGCount + 6] = "UPH";
    strngrdMDBQuery->Cells[2][iSGCount + 6] = iUPH;
    strngrdMDBQuery->Cells[3][iSGCount + 6] = "[unit/H]";
}

// =============================================================================
//  -- FW3-Obs2 APPEND --  FW-3 cObserver Wave 2: OEE/statistics subset
// =============================================================================
//  Translation wave: FW-3 cObserver Wave 2
//  Translator: AI(W906-FW3-Observer-W2) 20260818
//  Golden source: same as Wave 1's file-head banner (HT9011UC_Code_V3.33.906.0_
//  20260618/cObserver.cpp, cp950, 0 U+FFFD -- re-verified this wave with the
//  identical `python3 -c "open(path,'rb').read().decode('cp950').encode('utf-8')"`
//  command, 20260818).
//
//  See forms/fObserver.h's "-- FW3-Obs2 ADD --" block for the full WAVE SCOPE
//  table and GATE REGISTER (W2-1..W2-5) -- not duplicated here per Wave 1's
//  own "don't let the two files drift apart" convention.
//
//  ABSENCE-CLAIM TIMESTAMPS (commands + when run, this wave)
//  --------------------------------------------------------------------------
//    RecordMonitoringIndexCycleTime_New body anywhere in the golden tree:
//        `grep -n "RecordMonitoringIndexCycleTime_New"` over the cp950-decoded
//        golden cObserver.cpp -> 0 hits besides its own two call sites
//        (golden :2176/:1794-ish is a DIFFERENT function, RecordMonitoringIndexCycleTime,
//        not the _New variant) (20260818)
//    RecordTimeInfo / RecordMonitoringIndexCycleTime / RecordMonitoringIndexCycleTime_New
//    / SendTestResultToHttp already real (non-stub) anywhere in the C++ port tree:
//        `grep -rn "RecordTimeInfo\|RecordMonitoringIndexCycleTime_New\|SendTestResultToHttp"
//         --include=*.cpp --include=*.h .` over HT9011UC_Cpp_V3.33.906.0 -> 0 real
//        definitions (only local no-op shadows: atester_32Site.cpp/aTester_Front.cpp/
//        aTester_Rear.cpp each already carry their OWN TU-local
//        `SendTestResultToHttp` stub-plus-`#define`; csystem.cpp carries its own
//        TU-local `IniRecordMonitoringIndexCycleTime` stub-plus-`#define` for a
//        DIFFERENT, unrelated free function this wave also gives a real body to
//        -- see the integration-gap note below) (20260818)
//    fMain->slTestLog facade member: `grep -n "slTestLog" forms/fMain.h` -> 0
//        hits; already gated at cprod.cpp:2936-2939 for the identical reason
//        (20260818)
//
//  INTEGRATION GAP (not this wave's job, flagged for the main loop)
//  --------------------------------------------------------------------------
//  csystem.cpp:12108-12109 already has
//      `static void W7G3_IniRecordMonitoringIndexCycleTime(){}`
//      `#define IniRecordMonitoringIndexCycleTime  W7G3_IniRecordMonitoringIndexCycleTime`
//  (added when no real global existed yet). That `#define` is TU-local to
//  csystem.cpp (preprocessor, not linkage) so it does NOT collide with the
//  REAL, global `void IniRecordMonitoringIndexCycleTime();` this wave adds
//  below -- but it DOES mean csystem.cpp's two call sites (csystem.cpp:9935/
//  :12860) still reach the old no-op, not this wave's real translation, until
//  a later wave removes that local shadow. Same shape of gap as the
//  SendTestResultToHttp stubs this wave adds its OWN copy of, just discovered
//  from the other direction (a real body arriving after 3 stub shadows already
//  existed, instead of before).
//
//  DESIGN NOTE -- `fObserver->X` inside a TfObserver MEMBER method -> `X`
//  --------------------------------------------------------------------------
//  Golden member methods (RecordIndexCycle) read/write `fObserver->TimeInfoGrid`
//  etc. rather than using implicit `this`. In real BCB6 this is inert: `fObserver`
//  is the module-level global VCL sets to the one live TfObserver instance, so for
//  any method CALLED ON that instance, `fObserver` and `this` are the same pointer.
//  Translated as implicit `this` (i.e. just `TimeInfoGrid->...`) rather than
//  reintroducing the global by name -- Wave 1's own banner already ruled out
//  redeclaring `fObserver` in this header (would collide with the live
//  `TfObserverShim* fObserver`, atester_shims.h, exactly like the two-TMyKitSuck
//  trap). See forms/fObserver.h's W906Obs2_InstanceRegistrar banner for how the
//  free (non-member) functions below reach the same state instead.
//
//  DESIGN NOTE -- golden `SetCellNumber(x, y, someAnsiString.c_str())` -> `.Core.SetCellNumber(x, y, someAnsiString)`
//  --------------------------------------------------------------------------
//  Golden's TTMyTray::SetCellNumber has separate `char*` and `AnsiString`
//  overloads; several golden call sites explicitly select the `char*` one via
//  `.c_str()` even when the argument is already an AnsiString. This port's
//  vclcompat::TrayCore folds both into ONE `SetCellNumber(int,int,const
//  AnsiString&)` (forms/fObserver.h's own DESIGN NOTE on TfObserverTray) --
//  passing the AnsiString directly is the SAME text, just without the
//  redundant round-trip through `const char*`. Applied throughout
//  WriteCategoryData below without a comment at every individual site (same
//  "explain once" convention as Wave 1's `(long)` cast / `.FormatString` notes).
// =============================================================================
#include "forms/fMain.h"   // TfMain/fMain -- RecordIndexCycle's fMain->cbSetupFileName->Text
#include "LastSet.h"       // LastSet global -- GetMachineData/ProcessRunInfo/RecordIndexCycle
                            //   (NOT transitively pulled in by any of Wave 1's own includes)
#include <cstdio>          // sprintf into the raw char[64] sSocketCT buffers (WriteCategoryData)

// ---------------------------------------------------------------------------
//  File-scope data this wave's free functions need (golden cObserver.cpp:63-81,
//  the TestTimeInfo struct + TestSocketTimeInfo[2]/OEERecevieTimeInfo[2]
//  globals). Landed here (Wave 2's own append point) rather than moved up next
//  to Wave 1's file-scope block (append-only rule -- Wave 1's block is
//  existing content). Only the TWO instances of TestTimeInfo this wave's
//  in-scope functions actually touch are declared -- golden ALSO declares
//  `TestTimeInfoRecord[2][10]` (golden :72) and a SEPARATE
//  `TestReceiveTimeInfo` struct + `TestReceiveTimeInfoRecord[2][10]` (golden
//  :76-81), but both are read/written ONLY inside RecordTimeInfo (golden
//  :1846-2134), which is explicitly excluded this wave (task brief: "290 行
//  未 recon 完") -- not added (no delivered method touches them, Wave 1's own
//  "don't invent surface" discipline).
// ---------------------------------------------------------------------------
struct TestTimeInfo
{
    int iStartMin;
    int iStartSec;
    int iStartMSec;
    int iEndMin;
    int iEndSec;
    int iEndMSec;
};
static TestTimeInfo TestSocketTimeInfo[2];
static TestTimeInfo OEERecevieTimeInfo[2];

// ---------------------------------------------------------------------------
//  W906Obs2_InstanceRegistrar::ctor -- see forms/fObserver.h's banner on this
//  type for the full rationale. `W906Obs2_Instance` is TU-local (internal
//  linkage, this file only) and intentionally never cleared on destruction
//  (see that same banner's SAFETY paragraph).
// ---------------------------------------------------------------------------
static TfObserver *W906Obs2_Instance = 0;
W906Obs2_InstanceRegistrar::W906Obs2_InstanceRegistrar(TfObserver *self)
{
    W906Obs2_Instance = self;
}

// =============================================================================
//  GetMachineData -- golden :755-767
// =============================================================================
void TfObserver::GetMachineData()
{
    labPowerOnTime->Caption   = ConvertMSecToTime(LastSet.SystemAccSecond[0][stPowerOn]);
    labRunningTime->Caption   = ConvertMSecToTime(LastSet.SystemAccSecond[0][stStartTime]);
    labProductTime->Caption   = ConvertMSecToTime(LastSet.SystemAccSecond[0][stProductTime]);

    if (CUSTOMER_CODE == CC_AMKOR_Korea)   //jou 2012-05-04 CC_AMKOR_Korea不自動清除Loader Count
        labLoadingCount->Caption = LastSet.SendCT[0];
    else
        labLoadingCount->Caption = LastSet.SendCT[1];   //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位

    ProcessRunInfo();
}

// =============================================================================
//  Timer1Timer -- golden :708-753 (Close() -> facade no-op, see forms/fObserver.h DEVIATION note)
// =============================================================================
void TfObserver::Timer1Timer(void * /*Sender*/)
{
    if (bShow == false)
        return;

    GetMachineData();

    static Word iSystemSec = 0, iSystemMin = 0;   //Steven 20100818 Start: 按下Show Yield Chart按鈕一分鐘後要隱藏。
    static bool bStartShowYieldChart = false;
    static int  iCT = 0;

    if (iShowYieldChart == 1)
    {
        bStartShowYieldChart = true;
        iShowYieldChart = 2;
        iCT = 0;
    }
    else if (iShowYieldChart == 2)
    {
        if (bStartShowYieldChart)
        {
            iSystemSec = SystemSec;
            iSystemMin = SystemMin;
            bStartShowYieldChart = false;
        }
        else
        {
            if (SystemMin != iSystemMin)
            {
                if ((SystemSec + 60 - iSystemSec) > 60)
                {
                    iSystemSec = SystemSec;
                    iSystemMin = SystemMin;
                    iCT++;
                }
            }

            if (iCT >= 2)
            {
                iCT = 0;
                Close();
            }
        }
    }
    //Steven 20100818 End
}

void TfObserver::Close()
{
    // DEVIATION: golden TForm::Close() (:748) closes/hides the Observer
    // dialog. No TForm base here (see forms/fObserver.h banner) -- no-op,
    // matching Wave 1's own GDI/window-method convention
    // (TfObserverGrid::Repaint/Refresh). The counting logic that decides WHEN
    // to call it (iShowYieldChart/iCT/iSystemSec/iSystemMin, above) is
    // translated in full and genuinely observable.
}

// =============================================================================
//  UpdateTempChart / cbbTempChartChange -- golden :905-943 / :945-948
// =============================================================================
void TfObserver::UpdateTempChart()   //Steven 20090827
{
    AnsiString S;
    if      (IniConfig.iL10TempRecordInterval == 0) S = " (5  Sec)";
    else if (IniConfig.iL10TempRecordInterval == 2) S = " (30 Sec)";
    else if (IniConfig.iL10TempRecordInterval == 3) S = " (1  Min)";
    else if (IniConfig.iL10TempRecordInterval == 4) S = " (5  Min)";
    else if (IniConfig.iL10TempRecordInterval == 5) S = " (10 Min)";
    else                                             S = " (15 Sec)";

    TempChart->Title.Text->Clear();
    TempChart->Title.Text->Add("Temperature Chart of " + cbbTempChart->Text + S);

    if (cbbTempChart->ItemIndex == 0)
    {
        for (int j = 0; j < tcTotalCount; j++)   //Steven 20210621 : 動態產生溫度線段
        {
            TempChart->Series[j]->Clear();
            if (bUT150Install[j])
            {
                for (int i = 0; i < 60; i++)
                {
                    TempChart->Series[j]->AddY(dTempHistroy[j][i], AnsiString(" "), TC[j]);   //Steven 20140923 : Index使用EJ1N版32組加熱器
                }
            }
        }
    }
    else
    {
        for (int j = 0; j < tcTotalCount; j++)
            TempChart->Series[j]->Clear();

        int index = cbbTempChart->ItemIndex - 1;
        for (int i = 0; i < 60; i++)
        {
            TempChart->Series[index]->AddY(dTempHistroy[index][i], AnsiString(" "), TC[index]);   //Steven 20140923 : Index使用EJ1N版32組加熱器
        }
    }
}

void TfObserver::cbbTempChartChange(void * /*Sender*/)
{
    UpdateTempChart();
}

// =============================================================================
//  ProcessRunInfo -- golden :2254-2331
// =============================================================================
void TfObserver::ProcessRunInfo()
{
    AnsiString str = "";
    int dSec;
    int unit;

    dSec = ((LastSet.SystemAccSecond[0][stPauseTime] +                          //Isaac 20180417 (Steven) 修正MTBA公式(pause+production+jam)/jamcount
             LastSet.SystemAccSecond[0][stProductTime] +
             LastSet.SystemAccSecond[0][stJamTime]) / 1000);                    //Steven 20140816 : 改用秒為單位

    if (LastSet.iJamCount[1] == 0)
    {
        str.sprintf("0 / %s", ConvertSecondToSPC(dSec));
    }
    else
    {
        if (dSec != 0)
        {
            dSec = ChangeToFloatNonPcnt((double)(dSec), (double)(LastSet.iJamCount[1]));
            str.sprintf("1 / %s", ConvertSecondToSPC(dSec));
        }
        else
        {
            str.sprintf("%d / 0", LastSet.iJamCount[1]);
        }
    }
    labMTBA->Caption = str;

    if (LastSet.iJamCount[1] == 0)
    {
        str.sprintf("0 / %d unit", LastSet.SendCT[1]);
    }
    else
    {
        if (IniConfig.iUserLanguage == eulSingapore)   //Steven 20120807 : 新加坡要求顯示數量
        {
            str.sprintf("%d / %d unit", LastSet.iJamCount[1], LastSet.SendCT[1]);
        }
        else
        {
            unit = LastSet.SendCT[1];                  //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位
            if (unit != 0)
            {
                unit /= LastSet.iJamCount[1];           //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位
                str.sprintf("1 / %d unit", unit);
            }
            else
            {
                str.sprintf("%d / 0 unit", LastSet.iJamCount[1]);   //Steven 20110103
            }
        }
    }
    labMUBA->Caption = str;

    labMTBF->Caption = ConvertSecondToSPC(LastSet.SystemAccSecond[0][stPowerOn] / 1000);

    if (IniConfig.bVTESTFunction == true)   //jou 20210108 : 上海偉測要求新增每日jam rate統計
    {
        if (LastSet.iDayJamCount == 0)
        {
            str.sprintf("0 / 1 unit");
        }
        else
        {
            unit = LastSet.iDaySendCT;
            if (unit != 0)
            {
                unit /= LastSet.iDayJamCount;
                str.sprintf("1 / %d unit", unit);
            }
            else
            {
                str.sprintf("%d / 0 unit", LastSet.iDayJamCount);
            }
        }
        pnlDayJamRate->Caption = str;
    }
}

// =============================================================================
//  RecordIndexTime -- golden :2814-2903
// =============================================================================
void TfObserver::RecordIndexTime(double fData)   //jou 2010-12-22 新增index time ave.
{
    double fAve, iCT = 0.0;
    double temp;
    double dIndexCycleTime[9];

    for (int i = 9; i >= 0; i--)
    {
        fRecordIndexTime[i + 1] = fRecordIndexTime[i];
    }
    fRecordIndexTime[0] = fData;

    fAve = 0;
    for (int i = 0; i < 10; i++)
    {
        if (fRecordIndexTime[i] != 0)
        {
            fAve += fRecordIndexTime[i];
            iCT++;
        }
    }

    if (iCT == 0)
        fRecordIndexTime[11] = 0;
    else
        fRecordIndexTime[11] = fAve / iCT;

    if (CosFunction.bOEEFunction)   //Steven 20180417 (Jou) : OEE功能
    {                               //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        if (bTestIndexZ == true && iTestIndexZCount < 12)
        {
            bTestIndexZ = false;
            for (int j = 8; j >= 0; j--)
            {
                dRecordIndexZTime[j + 1] = dRecordIndexZTime[j];
            }
            dRecordIndexZTime[0] = fData;
            if (iTestIndexZCount >= 11)
            {
                for (int x = 0; x < 11; x++)
                {
                    for (int y = x; y <= 9; y++)
                    {
                        if (dRecordIndexZTime[y] > dRecordIndexZTime[x])
                        {
                            temp = dRecordIndexZTime[y];
                            dRecordIndexZTime[y] = dRecordIndexZTime[x];
                            dRecordIndexZTime[x] = temp;
                        }
                    }
                }

                double dTimeTotal = 0.0;
                for (int k = 0; k <= 4; k++)
                {
                    dTimeTotal += dRecordIndexZTime[k];
                }
                sTestIndexZTime = FloatToStr(dTimeTotal / 5.0);

                for (int i = 0; i < 9; i++)   //Sam 20180802 (wei) : OEE 32Site 修正
                {
                    dIndexCycleTime[i] = StrToFloatDef(TimeInfoGrid->Cells[4][i + 3], 0.0);   //Sam 20180822 : OEE 顯示修正
                }

                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 8 - i; j++)   //Sam 20201216 : 修正記憶體溢位
                        if (dIndexCycleTime[j + 1] < dIndexCycleTime[j])
                        {
                            temp = dIndexCycleTime[j];
                            dIndexCycleTime[j] = dIndexCycleTime[j + 1];
                            dIndexCycleTime[j + 1] = temp;
                        }
                }
                dTimeTotal = 0;
                for (int i = 0; i < 5; i++)
                {
                    dTimeTotal += dIndexCycleTime[4 + i];
                }
                dOEEIndexCycleTime = dTimeTotal / 5.0;
            }
            else
            {
                // AI(W906-FW3-Observer-W2) 20260818: golden's own '==' typo
                // (cObserver.cpp:2897) -- a no-op COMPARISON, not the
                // assignment `sTestIndexZTime="";` the surrounding code
                // clearly intends. Translated verbatim per project policy:
                // sTestIndexZTime silently keeps whatever value it already
                // had on this path (it is NOT reset to "").
                sTestIndexZTime == "";
                dOEEIndexCycleTime = 0;   //Sam 20180802 (wei) : OEE 32Site 修正
            }
            iTestIndexZCount++;
        }
    }
}

// =============================================================================
//  AddTimeData -- golden :2905-2966
// =============================================================================
void TfObserver::AddTimeData(int iRow, double Time)   //JerryYang 20151209
{
    for (int i = 10; i > 1; i--)
    {
        sgTimeData->Cells[i][iRow] = sgTimeData->Cells[i - 1][iRow];
    }

    sgTimeData->Cells[1][iRow] = AnsiString(Time);
    if (iRow == 2)
    {
        for (int i = 0; i < 9; i++)
        {
            if (sgTimeData->Cells[i + 1][2] != "" && sgTimeData->Cells[i + 1][1] != "")
            {
                sgTimeData->Cells[i + 1][17] = FloatToStr(StrToFloat(sgTimeData->Cells[i + 1][2]) - StrToFloat(sgTimeData->Cells[i + 1][1]));
            }
        }
    }
    else if (iRow == 7)
    {
        sgTimeData->Cells[1][16] = FloatToStr(StrToFloat(sgTimeData->Cells[1][7]) - StrToFloat(sgTimeData->Cells[1][6]));
        if (sgTimeData->Cells[2][7] != "" && sgTimeData->Cells[2][6] != "")
            sgTimeData->Cells[2][16] = FloatToStr(StrToFloat(sgTimeData->Cells[2][7]) - StrToFloat(sgTimeData->Cells[2][6]));
    }
    else if (iRow == 9)
    {
        sgTimeData->Cells[1][17] = FloatToStr(StrToFloat(sgTimeData->Cells[1][9]) - StrToFloat(sgTimeData->Cells[1][8]));
        if (sgTimeData->Cells[2][9] != "" && sgTimeData->Cells[2][8] != "")
            sgTimeData->Cells[2][17] = FloatToStr(StrToFloat(sgTimeData->Cells[2][9]) - StrToFloat(sgTimeData->Cells[2][8]));
    }
    else if (iRow == 11)
    {
        sgTimeData->Cells[1][18] = FloatToStr(StrToFloat(sgTimeData->Cells[1][11]) - StrToFloat(sgTimeData->Cells[1][10]));
        if (sgTimeData->Cells[2][11] != "" && sgTimeData->Cells[2][10] != "")
            sgTimeData->Cells[2][18] = FloatToStr(StrToFloat(sgTimeData->Cells[2][11]) - StrToFloat(sgTimeData->Cells[2][10]));
    }
    else if (iRow == 13)
    {
        sgTimeData->Cells[1][19] = FloatToStr(StrToFloat(sgTimeData->Cells[1][13]) - StrToFloat(sgTimeData->Cells[1][9]));
        if (sgTimeData->Cells[2][13] != "" && sgTimeData->Cells[2][9] != "")
            // AI(W906-FW3-Observer-W2) 20260818: golden's own copy/paste bug
            // (cObserver.cpp:2945) -- both operands read Cells[2][9]. The line
            // right above (Cells[1][19]) pairs [1][13]-[1][9], and the guard
            // just above THIS line tests BOTH Cells[2][13] and Cells[2][9] are
            // non-empty, strongly suggesting the intent was
            // `Cells[2][13]-Cells[2][9]` (matching the [1][19] line's own
            // shape) -- instead this computes Cells[2][9]-Cells[2][9], always
            // "0". Kept verbatim per project policy: translate what golden
            // DOES, not what it probably meant; not corrected without an
            // explicit user decision to change behaviour.
            sgTimeData->Cells[2][19] = FloatToStr(StrToFloat(sgTimeData->Cells[2][9]) - StrToFloat(sgTimeData->Cells[2][9]));
    }
    else if (iRow == 15)
    {
        sgTimeData->Cells[1][20] = FloatToStr(StrToFloat(sgTimeData->Cells[1][15]) - StrToFloat(sgTimeData->Cells[1][14]));
        if (sgTimeData->Cells[2][15] != "" && sgTimeData->Cells[2][14] != "")
            sgTimeData->Cells[2][20] = FloatToStr(StrToFloat(sgTimeData->Cells[2][15]) - StrToFloat(sgTimeData->Cells[2][14]));
    }
    else if (iRow == 20)   //JerryYang 20170503 (wei) drop contact的index cycle time分成三段來計時
    {
        for (int i = 0; i < 9; i++)
        {
            if (sgTimeData->Cells[i + 1][18] != "" && sgTimeData->Cells[i + 1][19] != "" && sgTimeData->Cells[i + 1][20] != "")
            {
                sgTimeData->Cells[i + 1][21] = FloatToStr(StrToFloat(sgTimeData->Cells[i + 1][18]) + StrToFloat(sgTimeData->Cells[i + 1][19]) + StrToFloat(sgTimeData->Cells[i + 1][20]));
            }
        }
    }

    if (bShow)
        sgTimeData->Refresh();
}

// =============================================================================
//  RecordInArmTime -- golden :2968-2999
// =============================================================================
void TfObserver::RecordInArmTime()   //Steven 20140930 : For XY-Pitch
{
    double fAve, iCT = 0.0;
    for (int i = 9; i >= 0; i--)
    {
        fRecordInArmTime[i + 1] = fRecordInArmTime[i];
    }

    fRecordInArmTime[0] = tRecordInArmTimer.LatchCycleTime() / 1000.0;
    tRecordInArmTimer.LatchCycleTime(true);

    fAve = 0;
    for (int i = 0; i < 10; i++)
    {
        if (fRecordInArmTime[i] != 0)
        {
            fAve += fRecordInArmTime[i];
            iCT++;
        }
    }

    if (iCT == 0.0)
        fRecordInArmTime[11] = 0;
    else
        fRecordInArmTime[11] = fAve / iCT;

    if (bShow)
    {
        for (int i = 0; i < 11; i++)
            TimeInfoGrid_InArm->Cells[1][i + 1] = fRecordInArmTime[i];
    }
}

// =============================================================================
//  WriteCategoryData -- golden :3274-3547
//
//  Two golden patterns below LOOK like bugs and are translated VERBATIM
//  (project policy:照翻，並在 //AI 註解寫下它為什麼看起來錯):
//   (1) The `mtIfError->SetCellNumber(..., iCat)` line in BOTH percent-mode
//       branches reads `iCat` BEFORE the `for(iCat=0;...)` loop right after it
//       (re)sets it fresh. Traced by hand this wave: every loop that touches
//       `iCat` earlier in the SAME function (the `mtCategoryTotal` loop, and
//       every `mtCategoryNo` loop from a PRIOR outer-loop iteration) always
//       runs to full completion, so `iCat` is reliably `iTestBinCount` at
//       every one of these read sites -- i.e. this consistently (not
//       randomly) lands on the SAME index the NUMBER-mode branches spell out
//       literally as `[iTestBinCount]` just above. Not "fixed" to the literal
//       spelling, to keep this file line-for-line diffable against golden.
//   (2) In the `bUseTwoArm32Site==false` branches, `mtHeadTotal`/`mtPassHead`
//       index by `iCol*2+iArm` (0..15, one cell per arm/col pair) but
//       `mtSockTotal`/`mtPassSocket` index by `iCol+iArm` (0..8, OVERLAPPING:
//       e.g. iCol=0,iArm=1 and iCol=1,iArm=0 both write index 1). Harmless in
//       VALUE (the written value never depends on iArm, so an overlapping
//       write repeats the same value), but it means mtSockTotal/mtPassSocket
//       never populate the full 16-cell range mtHeadTotal/mtPassHead do.
//       Translated verbatim (marked at both occurrences below).
// =============================================================================
void TfObserver::WriteCategoryData()
{
    int iArm, iRow, iCol, iCat;
    AnsiString Result = "";
    TastCategory.UpdataCount(true);
    RowNo = rgRowNo->ItemIndex;
    AnsiString aStr;

    if (bUseTwoArm32Site == true)   //Steven 20210630 : 修正32site的socket資料顯示
    {
        if (RowNo == 0)
            mtRowName->Core.SetCellNumber(0, 0, AnsiString("Arm 1"));
        else
            mtRowName->Core.SetCellNumber(0, 0, AnsiString("Arm 2"));

        for (int i = 0; i < mtChName->Core.FXItem; i++)
        {
            if (IsNNMode() == NN_1Row)
            {
                if (RowNo == 0)
                    iRow = i / 8 + 1;
                else
                    iRow = i / 8;
            }
            else
            {
                if (RowNo == 0)
                    iRow = i / 8 + 2;
                else
                    iRow = i / 8;
            }
            iCol = i % 8;

            aStr.sprintf("Col-%c", 'a' + iCol);
            mtDutName->Core.SetCellNumber(i, 0, aStr);

            if (TestIF.iSiteMap[iRow][iCol] > 0)
            {
                aStr.sprintf("CH%d", TestIF.iSiteMap[iRow][iCol]);
                mtChName->Core.SetCellNumber(i, 0, aStr);
            }
            else
            {
                mtChName->Core.SetCellNumber(i, 0, AnsiString("----"));
            }
        }
    }
    else
    {
        if (RowNo == 0)   //jou 20171023 (wei) : 修正yield RowB顯示錯誤
            mtRowName->Core.SetCellNumber(0, 0, AnsiString("RowA"));
        else
            mtRowName->Core.SetCellNumber(0, 0, AnsiString("RowB"));

        for (int i = 0; i < mtChName->Core.FXItem; i++)
        {
            if (TestIF.iSiteMap[RowNo][i] > 0)
            {
                aStr.sprintf("CH%d", TestIF.iSiteMap[RowNo][i]);
                mtChName->Core.SetCellNumber(i, 0, aStr);
            }
            else
            {
                mtChName->Core.SetCellNumber(i, 0, AnsiString("----"));
            }
        }
    }

    if (rgRowNo->ItemIndex == 2 || rgRowNo->ItemIndex == 3)
        RowNo = 0;

    TastCategory.UpdataCount(true);   //Steven 20250514 : 統一計算數量
    mtTotal->Core.SetCellNumber(0, 0, AnsiString(" Total "));
    mtTotal->Core.SetCellNumber(0, 2, AnsiString(" Total "));

    if (rbSocketNumber->Checked ||
        rbSocketPercent->Checked ||
        IsNNMode() == NN_1Row)
        mtCategoryNo->Core.SetXItem(8);
    else
        mtCategoryNo->Core.SetXItem(16);

    if (bUseTwoArm32Site == true)   //Steven 20210630 : 修正32site的socket資料顯示
    {
        if (RowNo == 0)
        {
            iArm = 0;
        }
        else
        {
            iArm = 1;
        }

        if (rbHeadNumber->Checked || rbSocketNumber->Checked)   //顯示數字的話
        {
            //By Socket顯示
            mtTotal->Core.SetCellNumber(0, 1, TastCategory.iTotalSocket);
            mtTotal->Core.SetCellNumber(0, 3, TastCategory.iPassSocket);
            mtTotal->Core.SetCellNumber(0, 4, TastCategory.iTotalCategory[iTestBinCount]);

            for (iCat = 0; iCat < iTestBinCount; iCat++)
            {
                mtCategoryTotal->Core.SetCellNumber(0, iCat, TastCategory.iTotalCategory[iCat]);   //填右側下面總和
            }
            //處理其他的Col
            for (int i = 0; i < mtChName->Core.FXItem; i++)
            {
                if (IsNNMode() == NN_1Row)   //Ifor 20260401 : 修正bUseTwoArm32Site ARM1資料未更新問題(與百分比分支對齊iRow計算)
                {
                    if (RowNo == 0)
                        iRow = i / 8 + 1;
                    else
                        iRow = i / 8;
                }
                else
                {
                    if (RowNo == 0)
                        iRow = i / 8 + 2;
                    else
                        iRow = i / 8;
                }
                iCol = i % 8;
                //處理最下面Row的Total數字
                mtHeadTotal->Core.SetCellNumber(i,  0, TastCategory.iCountHeadTotal[iArm][iRow][iCol]);
                mtPassHead ->Core.SetCellNumber(i,  0, TastCategory.iCountPassHead[iArm][iRow][iCol]);
                mtSockTotal ->Core.SetCellNumber(i, 0, TastCategory.iCountHeadTotal[iArm][iRow][iCol]);   //Head Total
                mtPassSocket->Core.SetCellNumber(i, 0, TastCategory.iCountPassHead[iArm][iRow][iCol]);    //Pass Total

                //處理每個DUT的數字
                mtIfError->Core.SetCellNumber(i, 0, TastCategory.iCountCategory[iArm][iRow][iCol][iTestBinCount]);   //IFErr
                //By Arm顯示
                for (iCat = 0; iCat < iTestBinCount; iCat++)
                    mtCategoryNo->Core.SetCellNumber(i, iCat, TastCategory.iCountCategory[iArm][iRow][iCol][iCat]);
            }
        }
        else   //if(rbSocketPercent->Checked || rbHeadPercent->Checked)
        {
            mtTotal->Core.SetCellNumber(0, 1, ChangeToPercentage(TastCategory.iTotalSocket, TastCategory.iTotalSocket));
            mtTotal->Core.SetCellNumber(0, 3, ChangeToPercentage(TastCategory.iPassSocket,  TastCategory.iTotalSocket));
            mtTotal->Core.SetCellNumber(0, 4, ChangeToPercentage(TastCategory.iTotalCategory[iTestBinCount],  TastCategory.iTotalSocket));
            for (iCat = 0; iCat < iTestBinCount; iCat++)
            {
                mtCategoryTotal->Core.SetCellNumber(0, iCat, ChangeToPercentage(TastCategory.iTotalCategory[iCat], TastCategory.iTotalSocket));
            }

            //處理其他的Col
            for (int i = 0; i < mtChName->Core.FXItem; i++)
            {
                if (IsNNMode() == NN_1Row)
                {
                    if (RowNo == 0)
                        iRow = i / 8 + 1;
                    else
                        iRow = i / 8;
                }
                else
                {
                    if (RowNo == 0)
                        iRow = i / 8 + 2;
                    else
                        iRow = i / 8;
                }
                iCol = i % 8;
                mtSockTotal ->Core.SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[iArm][iRow][iCol], TastCategory.iTotalSocket));   //Head Total
                mtPassSocket->Core.SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountPassHead[iArm][iRow][iCol], TastCategory.iTotalSocket));
                mtHeadTotal ->Core.SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[iArm][iRow][iCol], TastCategory.iTotalSocket));
                mtPassHead->Core.SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountPassHead[iArm][iRow][iCol], TastCategory.iCountHeadTotal[iArm][iRow][iCol]));

                //處理每個DUT的數字 -- see WriteCategoryData's file-head DESIGN NOTE (1): `iCat`
                //here is whatever the LAST completed `for(iCat=0;...)` loop left it at
                //(iTestBinCount), same index the NUMBER-mode branch above spells literally.
                mtIfError->Core.SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountCategory[iArm][iRow][iCol][iCat], TastCategory.iCountSocketTotal[iRow][iCol]));   //IFErr
                //By Arm顯示
                for (iCat = 0; iCat < iTestBinCount; iCat++)   //category
                {
                    mtCategoryNo->Core.SetCellNumber(i, iCat, ChangeToPercentage(TastCategory.iCountCategory[iArm][iRow][iCol][iCat], TastCategory.iCountHeadTotal[iArm][iRow][iCol]));
                }
            }
        }
    }
    else
    {
        //顯示數字的話
        if (rbHeadNumber->Checked || rbSocketNumber->Checked)
        {
            //By Socket顯示
            mtTotal->Core.SetCellNumber(0, 1, TastCategory.iTotalSocket);
            mtTotal->Core.SetCellNumber(0, 3, TastCategory.iPassSocket);
            mtTotal->Core.SetCellNumber(0, 4, TastCategory.iTotalCategory[iTestBinCount]);

            for (iCat = 0; iCat < iTestBinCount; iCat++)
            {
                mtCategoryTotal->Core.SetCellNumber(0, iCat, TastCategory.iTotalCategory[iCat]);   //填右側下面總和
            }
            //處理其他的Col
            for (iCol = 0; iCol < NEW_MAX_Index_Col; iCol++)   //最大是8
            {
                //處理最下面Row的Total數字
                for (iArm = 0; iArm < 2; iArm++)   //arm
                {
                    mtHeadTotal->Core.SetCellNumber(iCol * 2 + iArm, 0, TastCategory.iCountHeadTotal[iArm][RowNo][iCol]);
                    mtPassHead ->Core.SetCellNumber(iCol * 2 + iArm, 0, TastCategory.iCountPassHead[iArm][RowNo][iCol]);
                    // See file-head DESIGN NOTE (2): iCol+iArm OVERLAPS across
                    // iCol/iArm pairs (unlike iCol*2+iArm just above) -- verbatim.
                    mtSockTotal ->Core.SetCellNumber(iCol + iArm,  0, TastCategory.iCountHeadTotal[0][RowNo][iCol] + TastCategory.iCountHeadTotal[1][RowNo][iCol]);   //Head Total
                    mtPassSocket->Core.SetCellNumber(iCol + iArm,  0, TastCategory.iCountPassHead[0][RowNo][iCol] + TastCategory.iCountPassHead[1][RowNo][iCol]);     //Pass Total
                }

                //處理每個DUT的數字
                mtIfError->Core.SetCellNumber(iCol, 0, TastCategory.iCountCategory[0][RowNo][iCol][iTestBinCount] + TastCategory.iCountCategory[1][RowNo][iCol][iTestBinCount]);   //IFErr
                if (rbSocketNumber->Checked)
                {
                    for (iCat = 0; iCat < iTestBinCount; iCat++)   //category    //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    {
                        sprintf(sSocketCT[iCol][iCat], "%d", TastCategory.iCountCategory[0][RowNo][iCol][iCat] + TastCategory.iCountCategory[1][RowNo][iCol][iCat]);
                        mtCategoryNo->Core.SetCellNumber(iCol, iCat, AnsiString(sSocketCT[iCol][iCat]));   //填入中下面的數值
                    }
                }
                else
                {
                    //By Arm顯示
                    for (iArm = 0; iArm < 2; iArm++)   //arm
                    {
                        for (iCat = 0; iCat < iTestBinCount; iCat++)   //category //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        {
                            mtCategoryNo->Core.SetCellNumber(iCol * 2 + iArm, iCat, TastCategory.iCountCategory[iArm][RowNo][iCol][iCat]);
                        }
                    }
                }
            }
        }
        else   //if(rbSocketPercent->Checked || rbHeadPercent->Checked)
        {
            mtTotal->Core.SetCellNumber(0, 1, ChangeToPercentage(TastCategory.iTotalSocket, TastCategory.iTotalSocket));
            mtTotal->Core.SetCellNumber(0, 3, ChangeToPercentage(TastCategory.iPassSocket,  TastCategory.iTotalSocket));
            mtTotal->Core.SetCellNumber(0, 4, ChangeToPercentage(TastCategory.iTotalCategory[iTestBinCount],  TastCategory.iTotalSocket));
            for (iCat = 0; iCat < iTestBinCount; iCat++)
            {
                mtCategoryTotal->Core.SetCellNumber(0, iCat, ChangeToPercentage(TastCategory.iTotalCategory[iCat], TastCategory.iTotalSocket));
            }

            //處理其他的Col
            for (iCol = 0; iCol < NEW_MAX_Index_Col; iCol++)   //最大是8
            {
                for (iArm = 0; iArm < 2; iArm++)   //arm
                {
                    mtSockTotal ->Core.SetCellNumber(iCol + iArm, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[0][RowNo][iCol] + TastCategory.iCountHeadTotal[1][RowNo][iCol], TastCategory.iTotalSocket));   //Head Total
                    mtPassSocket->Core.SetCellNumber(iCol + iArm, 0, ChangeToPercentage(TastCategory.iCountPassHead[0][RowNo][iCol] + TastCategory.iCountPassHead[1][RowNo][iCol], TastCategory.iTotalSocket));
                    mtHeadTotal ->Core.SetCellNumber(iCol * 2 + iArm, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[iArm][RowNo][iCol], TastCategory.iTotalSocket));
                    mtPassHead->Core.SetCellNumber(iCol * 2 + iArm, 0, ChangeToPercentage(TastCategory.iCountPassHead[iArm][RowNo][iCol], TastCategory.iCountHeadTotal[iArm][RowNo][iCol]));
                }

                //處理每個DUT的數字 -- see file-head DESIGN NOTE (1): `iCat` here is
                //whatever the LAST completed `for(iCat=0;...)` loop left it at.
                mtIfError->Core.SetCellNumber(iCol, 0, ChangeToPercentage(TastCategory.iCountCategory[0][RowNo][iCol][iCat] + TastCategory.iCountCategory[1][RowNo][iCol][iCat], TastCategory.iCountSocketTotal[RowNo][iCol]));   //IFErr
                if (rbSocketPercent->Checked)
                {
                    //By Socket顯示                +iArm
                    for (iCat = 0; iCat < iTestBinCount; iCat++)   //category    //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    {
                        sprintf(sSocketCT[iCol][iCat], "%s",
                                ChangeToPercentage(TastCategory.iCountCategory[0][RowNo][iCol][iCat] + TastCategory.iCountCategory[1][RowNo][iCol][iCat], TastCategory.iCountSocketTotal[RowNo][iCol]).c_str());   // .c_str(): plain ::sprintf's "..." cannot bind a non-trivial AnsiString directly (GCC hard error) -- same text, see file-head DESIGN NOTE
                        mtCategoryNo->Core.SetCellNumber(iCol, iCat, AnsiString(sSocketCT[iCol][iCat]));   //填入中下面的數值
                    }
                }
                else
                {
                    //By Arm顯示
                    for (iArm = 0; iArm < 2; iArm++)   //arm
                    {
                        for (iCat = 0; iCat < iTestBinCount; iCat++)   //category
                        {
                            mtCategoryNo->Core.SetCellNumber(iCol * 2 + iArm, iCat, ChangeToPercentage(TastCategory.iCountCategory[iArm][RowNo][iCol][iCat], TastCategory.iCountHeadTotal[iArm][RowNo][iCol]));
                        }
                    }
                }
            }
        }
    }
}

// =============================================================================
//  GetTimeDataText -- golden :4795-4839
// =============================================================================
void TfObserver::GetTimeDataText()   //Steven 20190903 : Time Data顯示
{
    AnsiString Str;   // golden declares this but never reads/writes it in this range -- verbatim
    for (int i = 1; i < strngrdTimeData->RowCount; i++)
    {
        strngrdTimeData->ClearRow(i);   // golden ->Rows[i]->Clear() -- see forms/fObserver.h DESIGN NOTES (Wave 1)
    }

    if (lstTimeData->Items->Count < 1)   // golden ->Count shortcut (Wave 1's own lstEventLog precedent)
    {
        strngrdTimeData->RowCount = 2;
        strngrdTimeData->Cells[1][1] = "No Record!!";
    }
    else
    {
        AnsiString FileName = lstTimeData->Items->Strings[lstTimeData->ItemIndex];
        if (FileExists(FileName))
        {
            TStringList *tsLogFile, *tsRow;
            tsLogFile = new TStringList();
            tsRow     = new TStringList();
            tsLogFile->LoadFromFile(FileName);

            strngrdTimeData->RowCount = tsLogFile->Count;
            for (int i = 0; i < tsLogFile->Count; i++)
            {
                tsRow->Clear();
                tsRow->CommaText = tsLogFile->Strings[i];
                for (int j = 0; j < tsRow->Count; j++)
                {
                    strngrdTimeData->Cells[j][i] = tsRow->Strings[j];
                }
            }
            tsLogFile->Clear();
            tsRow->Clear();
            delete tsLogFile;
            delete tsRow;
        }
        else
        {
            strngrdTimeData->RowCount = 2;
            strngrdTimeData->Cells[1][1] = "No Record!!";
        }
    }
}

// =============================================================================
//  RecordIndexCycle -- golden :4846-4887
// =============================================================================
void TfObserver::RecordIndexCycle(bool bReset)   //Sam 20200916 : Add Index Cycle Time Record
{
    TStringList *sIndexCycleTimeCnt;
    static int iRecordIndexCycleTimeCnt = 0;

    AnsiString sFilePath = "D:\\HT9045_log\\IndexCycleTimeRecord\\";
    AnsiString sFilePathName = "";

    if (IniConfig.bD70IndexCycleTimeRecord == false)
    {
        iRecordIndexCycleTimeCnt = 0;
        return;
    }

    MyForceDirectories(sFilePath, "TfObserver::RecordIndexCycle");

    if (bReset)
    {
        iRecordIndexCycleTimeCnt = 0;
        return;
    }

    // golden `fObserver->TimeInfoGrid` -> implicit `this` (see file-head DESIGN
    // NOTE); golden `Now().FormatString(fmt)` -> `FormatDateTime(fmt, Now())`
    // (Wave 1's own established conversion, file-head DESIGN NOTE).
    sRecordIndexCycleTime[iRecordIndexCycleTimeCnt] =
        FormatDateTime("yyyy-mm-dd hh:nn:ss", Now()) + "," + IntToStr(iRecordIndexCycleTimeCnt + 1) + "," + TimeInfoGrid->Cells[4][11];
    iRecordIndexCycleTimeCnt++;
    if (iRecordIndexCycleTimeCnt >= 10)
    {
        sIndexCycleTimeCnt = new TStringList();
        sIndexCycleTimeCnt->Clear();
        sIndexCycleTimeCnt->Add("Record Time,No,Index Cycle Time");
        for (int i = 0; i < iRecordIndexCycleTimeCnt; i++)
        {
            sIndexCycleTimeCnt->Add(sRecordIndexCycleTime[i]);
        }

        sFilePathName.sprintf("%s%s_Temperature[%d]_AutoTray1Dir[%d]_%s.csv", sFilePath, fMain->cbSetupFileName->Text,
                               LastSet.iTemperature, TrayForm.Auto[0].Direction, FormatDateTime("yyyymmdd_hhnnss", Now()));
        sIndexCycleTimeCnt->SaveToFile(sFilePathName);
        sIndexCycleTimeCnt->Clear();
        delete sIndexCycleTimeCnt;
        iRecordIndexCycleTimeCnt = 0;
    }
}

// =============================================================================
//  RecordStartTestTime -- golden :2136-2147 (file-scope free function)
// =============================================================================
void RecordStartTestTime()
{
    iCurrentTime = 0;
    GetTimeInfo();
    for (int i = 0; i < 2; i++)
    {
        TestSocketTimeInfo[i].iStartMin  = SystemMin;
        TestSocketTimeInfo[i].iStartSec  = SystemSec;
        TestSocketTimeInfo[i].iStartMSec = SystemMSec;
    }
    sBufferSOT = FormatDateTime("yyyymmdd_hhnnss", Now());   //wei 20181211 (Steven) : 更換位置SOT
}

// =============================================================================
//  RecordEndTestTime -- golden :2150-2235 (file-scope free function)
//  GATE stubs for its 4 unported callees -- see forms/fObserver.h GATE
//  REGISTER W2-1/W2-2/W2-3/W2-4 for the full rationale on each.
// =============================================================================
static void RecordMonitoringIndexCycleTime_New()
{
    // GAP (GATE REGISTER W2-1): not declared anywhere in the golden tree
    // outside this one call site; not one of this wave's named targets.
}

static void RecordMonitoringIndexCycleTime()
{
    // GAP (GATE REGISTER W2-2): real golden body exists (cObserver.cpp:
    // 1709-1751) but is explicitly excluded this wave (task brief: "彈訊息").
}

static void RecordTimeInfo()
{
    // GAP (GATE REGISTER W2-3): real golden body (cObserver.cpp:1846-2134,
    // ~290 lines) explicitly excluded this wave (task brief: "290 行未 recon 完").
}

// GATE REGISTER W2-4: golden itself only forward-declares this LOCALLY
// (cObserver.cpp:2149, right before RecordEndTestTime) -- same gap already
// solved 3x elsewhere (atester_32Site.cpp:394/397, aTester_Front.cpp:3264,
// aTester_Rear.cpp:3148). TU-local stub returning 1 (== upload OK, matching
// golden's own offline convention) + #define shadow.
static int W7Obs_SendTestResultToHttp() { return 1; }
#define SendTestResultToHttp W7Obs_SendTestResultToHttp

int RecordEndTestTime(int iArm)   //Sam 20201231 : 修正關 Arm 後，Index Cycle time 異常。0:arm1 1:arm2 2:雙Arm
{
    AnsiString str;
    int iCount = 0, iMaxSite, iSiteCh;
    GetTimeInfo();
    for (int i = 0; i < 2; i++)
    {
        TestSocketTimeInfo[i].iEndMin  = SystemMin;
        TestSocketTimeInfo[i].iEndSec  = SystemSec;
        TestSocketTimeInfo[i].iEndMSec = SystemMSec;
    }
    sBufferEOT = FormatDateTime("yyyymmdd_hhnnss", Now());   //wei 20181211 (Steven) : 更換位置SOT

    if ((TestIF_File.iShuttleMode == 1 && TestIF_File.iShuttle_Sel == 0 && iArm == 0) ||
        (TestIF_File.iShuttleMode == 1 && TestIF_File.iShuttle_Sel == 1 && iArm == 1) ||
        TestIF_File.iShuttleMode == 0 ||
        iArm == 2)   //Sam 20201231 : 修正關 Arm 後，Index Cycle time 異常 1:arm1 2:arm2
    {
        RecordTimeInfo();
    }

    int iRet = SendTestResultToHttp();

    if (IniConfig.bSPILFunction == true &&
        TestIF_File.bIndexCycleTimeMonitor == true)   //JerryYang 20220824 : SPIL index cycle time monitor
    {
        RecordMonitoringIndexCycleTime_New();
    }
    else
    {
        if (TestIF_File.bIndexCycleTimeMonitor == true && bResetflag == true)   //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
        {
            RecordMonitoringIndexCycleTime();
        }
    }

    if (IniConfig.bN28_SCK_OEE)   //Steven 20210608 : JSCK OEE Function.
    {
        TStringList *List1 = new TStringList();
        TStringList *List2 = new TStringList();
        List1->Clear();
        List1->Add(IniConfig.sN28_IP);   //IP
        str.sprintf("%04d/%02d/%02d %02d:%02d:%02d:%03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
        List1->Add(str);   //yyyy/mm/dd hh:mm:ss:sss
        str.sprintf("%0.3f", RunInfo.dTestTimeSec);
        List1->Add(str);   //Test time
        List1->Add(RunInfo.IndexCycleTime);   //Index time
        List1->Add("0.00");   //0.00
        if (TestIF.iTestMode == _32Site4X8N)
            iMaxSite = 32;
        else
            iMaxSite = 16;

        for (int i = 0; i < iMaxSite; i++)
        {
            List2->Add("0");
        }

        for (int i = 0; i < TestSocket.iShtRow; i++)
        {
            for (int j = 0; j < TestSocket.iShtCol; j++)
            {
                if (TestSocket.Item[i][j] != HAS_NULL_IC &&   //kevin 20120618  沒有IC 就不要設定避免畫面被誤解
                    TestSocket.Item[i][j] != NULL_IC)
                {
                    iSiteCh = TestIF_File.iSiteMap[i][j] - 1;
                    if (iSiteCh >= 0 && iSiteCh < iMaxSite)
                    {
                        List2->Strings[iSiteCh] = TestSocket.iBinData[i][j];
                        iCount++;
                    }
                }
            }
        }
        List1->Add(iCount);   //Enabled site
        List1->Add(List2->CommaText);   //site1 tested bin, ... site16 tested bin
        str = StringReplace(List1->CommaText, "\"", "", TReplaceFlags() << rfReplaceAll);
        str = StringReplace(str, ",", ", ", TReplaceFlags() << rfReplaceAll);
        // GATE REGISTER W2-5: fMain->slTestLog is not a forms/fMain.h facade
        // member (same pre-existing gap already documented at cprod.cpp:
        // 2936-2939, NOT a new one this wave introduces).
#if 0
        fMain->slTestLog->AddText(str);
#endif
        List1->Clear();
        List2->Clear();
        delete List1;
        delete List2;
    }
    return iRet;
}

// =============================================================================
//  RecordReceiveTestTime -- golden :4755-4764 (file-scope free function).
//  NOTE: recon's own byte-range citation (:4755-4794) also spans the NEXT
//  golden function, pgcMessageChange (:4766-4793) -- NOT one of this wave's
//  named targets and NOT translated (it reads lstTimeData/SearchFileAll
//  against a hardcoded "D:\\HT9045_Log\\TimeData\\%d\\" path, a whole
//  separate UI action). Verified by reading golden text end to end this wave.
// =============================================================================
void RecordReceiveTestTime()   //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
{
    GetTimeInfo();
    for (int i = 0; i < 2; i++)
    {
        OEERecevieTimeInfo[i].iStartMin  = SystemMin;
        OEERecevieTimeInfo[i].iStartSec  = SystemSec;
        OEERecevieTimeInfo[i].iStartMSec = SystemMSec;
    }
}

// =============================================================================
//  IniRecordMonitoringIndexCycleTime -- golden :1836-1844 (file-scope free function)
//  See forms/fObserver.h's W906Obs2_InstanceRegistrar banner for
//  W906Obs2_Instance (golden's own `fObserver->` self-reference, reached here
//  through the registrar since this is NOT a TfObserver member).
// =============================================================================
void IniRecordMonitoringIndexCycleTime()   //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
{
    if (W906Obs2_Instance)   // PORT-ONLY guard -- golden's `fObserver` is never
                              // null once the app is running; this facade's
                              // instance may not be registered yet in an
                              // isolated caller/test. See registrar SAFETY note.
        W906Obs2_Instance->iIndexCycleTimeCount = 0;
    for (int i = 0; i < 20; i++)   //清除
    {
        queue20[i] = 0;
    }
    iMonitoringOutlierCnt = 0;
}

// =============================================================================
//  RecordIndexAirOnTime1 / RecordIndexAirOnTime2 -- golden :5331-5343 / :5346-5358
//  (file-scope free functions). Same W906Obs2_Instance guard rationale as
//  IniRecordMonitoringIndexCycleTime above.
// =============================================================================
void RecordIndexAirOnTime1()   //Sam 20220329 : Record Index Air On Time
{
    if (CosFunction.RecordIndexAirOnTime == false)
        return;
    if (!W906Obs2_Instance)   // PORT-ONLY guard, see IniRecordMonitoringIndexCycleTime above
        return;
    for (int j = 0; j < 100; j++)
    {
        if (j <= QueueAirOnTime1.iCount)
        {
            W906Obs2_Instance->strngrdIndeAirOn1->Cells[1][101 - j] = QueueAirOnTime1.GetStartTime(j);
            W906Obs2_Instance->strngrdIndeAirOn1->Cells[2][101 - j] = QueueAirOnTime1.GetEndTime(j);
            W906Obs2_Instance->strngrdIndeAirOn1->Cells[3][101 - j] = QueueAirOnTime1.GetTimeString(j);
        }
    }
}

void RecordIndexAirOnTime2()   //Sam 20220329 : Record Index Air On Time
{
    if (CosFunction.RecordIndexAirOnTime == false)
        return;
    if (!W906Obs2_Instance)   // PORT-ONLY guard, see IniRecordMonitoringIndexCycleTime above
        return;
    for (int j = 0; j < 100; j++)
    {
        if (j <= QueueAirOnTime2.iCount)
        {
            W906Obs2_Instance->strngrdIndeAirOn2->Cells[1][101 - j] = QueueAirOnTime2.GetStartTime(j);
            W906Obs2_Instance->strngrdIndeAirOn2->Cells[2][101 - j] = QueueAirOnTime2.GetEndTime(j);
            W906Obs2_Instance->strngrdIndeAirOn2->Cells[3][101 - j] = QueueAirOnTime2.GetTimeString(j);
        }
    }
}
