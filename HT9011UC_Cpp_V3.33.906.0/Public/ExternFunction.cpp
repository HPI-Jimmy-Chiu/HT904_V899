// ===========================================================================
//  Public/ExternFunction.cpp  -- W1 portable translation
//  BCB6 golden source: HT9011UC_Code_V3.33.906.0_20260618/Public/ExternFunction.cpp
//
//  Translated: W1 batch, pure-logic leaf functions only, PLUS DeleteDirectory
//  (AI(W906-uHGemClass-Micro6) 20260721: vclcompat/SysUtils.h now has the
//  FindFirst/FindNext shim this needed -- see ExternFunction.h's own updated
//  note).
//  Deferred (VCL-form-coupled):
//    StringGrid_Insert_Row   -> W7 (UI wave)
//    StringGrid_Delete_Row   -> W7 (UI wave)
//    StatusBar_ItemText      -> W7 (UI wave)
//    ShowRecordTime          -> W7 (UI wave, depends on TEdit)
// ===========================================================================

// BCB6 had:  #include "MachineDefine.h"  / #pragma hdrstop
// Replaced with the vclcompat umbrella header.
#include "vclcompat/vcl_compat.h"

#include "Public/ExternFunction.h"

#include <ctime>    // clock(), clock_t

// ---------------------------------------------------------------------------
//  Math_AdjustAngle
//  BCB6 source: ExternFunction.cpp:126-151
//
//  Two-branch normalisation:
//    bSign == true  : angle is already negative (or zero); only bring it into
//                     (-360, 0] by adding 360 while <= -360.  Positive angles
//                     (and those already in (-360,0]) pass through unchanged.
//    bSign == false : wrap into [0, 360) by adding 360 while < 0, then
//                     subtracting 360 while >= 360.
// ---------------------------------------------------------------------------
double Math_AdjustAngle(double fAngle, bool bSign)
{
    double adj_angle = fAngle;

    // BCB6 ExternFunction.cpp:130-135
    if (bSign == true && adj_angle < 0)
    {
        while (adj_angle <= -360)
        {
            adj_angle += 360;
        }
    }
    else  // BCB6 ExternFunction.cpp:138-147
    {
        while (adj_angle < 0)
        {
            adj_angle += 360;
        }

        while (adj_angle >= 360)
        {
            adj_angle -= 360;
        }
    }

    return adj_angle;  // BCB6 ExternFunction.cpp:150
}

// ---------------------------------------------------------------------------
//  String_JustNumber
//  BCB6 source: ExternFunction.cpp:153-185
//
//  1-based loop over sStr (after stripping leading spaces).
//  Accumulates AdjustStr while characters are numeric / dot / sign.
//  Inserts '0' before a leading dot, or before a dot that immediately follows
//  a sign character, to canonicalise ".5" -> "0.5" and "+.5" -> "+0.5".
// ---------------------------------------------------------------------------
AnsiString String_JustNumber(AnsiString sStr)
{
    AnsiString AdjustStr("");                  // BCB6 ExternFunction.cpp:155
    String_EraseLSpace(sStr);                  // BCB6 ExternFunction.cpp:156
    int length = sStr.Length();                // BCB6 ExternFunction.cpp:157
    bool first_dot = false;                    // BCB6 ExternFunction.cpp:158

    for (int i = 1; i <= length; i++)          // BCB6 ExternFunction.cpp:159  (1-based)
    {
        bool is_num  = (sStr[i] >= '0' && sStr[i] <= '9');                    // :161
        bool is_dot  = (first_dot == false && sStr[i] == '.');                 // :162
        bool is_sign = (i == 1 && (sStr[i] == '-' || sStr[i] == '+'));         // :163

        if (is_num == false && is_dot == false && is_sign == false)            // :165-168
        {
            break;
        }

        if (is_dot == true)                                                    // :170
        {
            if (i == 1)                                                        // :172-174
            {
                AdjustStr = "0";
            }
            else if (AdjustStr[AdjustStr.Length()] == '-' ||                   // :175-178
                     AdjustStr[AdjustStr.Length()] == '+')
            {
                AdjustStr += '0';
            }
            first_dot = true;                                                  // :180
        }
        AdjustStr += sStr[i];                                                  // :182
    }
    return AdjustStr;                                                          // :184
}

// ---------------------------------------------------------------------------
//  String_EraseLSpace
//  BCB6 source: ExternFunction.cpp:187-193
//
//  Removes all leading space characters from sStr in place (1-based Delete).
// ---------------------------------------------------------------------------
void String_EraseLSpace(AnsiString& sStr)
{
    while (sStr.Length() >= 1 && sStr[1] == ' ')    // BCB6 :189
    {
        sStr.Delete(1, 1);                           // BCB6 :191 -- 1-based Delete
    }
}

// ---------------------------------------------------------------------------
//  String_EraseRSpace
//  BCB6 source: ExternFunction.cpp:195-203
//
//  Removes all trailing space characters from sStr in place.
//  BCB6 re-reads Length() after each Delete; faithfully preserved.
// ---------------------------------------------------------------------------
void String_EraseRSpace(AnsiString& sStr)
{
    int length = sStr.Length();                              // BCB6 :197
    while (length >= 1 && sStr[length] == ' ')              // BCB6 :198
    {
        sStr.Delete(length, 1);                              // BCB6 :200 -- 1-based Delete
        length = sStr.Length();                              // BCB6 :201
    }
}

// ---------------------------------------------------------------------------
//  RecordTime
//  BCB6 source: ExternFunction.cpp:205-220
//
//  Static-local stopwatch.  Returns 0 on start; returns elapsed clock ticks
//  on read (bIsStart == false).  The return type is int, matching BCB6.
//  Overflow at ~2^31 clock ticks is the same risk as in BCB6.
// ---------------------------------------------------------------------------
int RecordTime(bool bIsStart)
{
    static clock_t tStartTime = clock();    // BCB6 :207 -- static local, same semantics

    if (bIsStart == true)                   // BCB6 :209-213
    {
        tStartTime = clock();
        return 0;
    }
    else                                    // BCB6 :214-218
    {
        clock_t tEndTime = clock();
        return static_cast<int>(tEndTime - tStartTime);
    }
}

// ---------------------------------------------------------------------------
//  DeleteDirectory
//  BCB6 source: ExternFunction.cpp:249-280 ("2014.06.12, Joye, ASE-SG")
//
//  AI(W906-uHGemClass-Micro6) 20260721: translated using vclcompat/SysUtils.h's
//  FindFirst/FindNext/FindClose/TSearchRec/RemoveDir/DeleteFile/
//  DirectoryExists -- their semantics (FindFirst/FindNext return 0 on a
//  match, non-0 when exhausted; faAnyFile/faDirectory bit values) already
//  match BCB6's own SysUtils exactly (see that header's own comment), so this
//  body is a direct, unmodified transcription of golden's logic.
//
//  Golden quirks preserved verbatim (see ExternFunction.h's own comment for
//  the full list): nonexistent sDir -> true (not an error); individual
//  DeleteFile() failures are never checked; a failed recursive call on a
//  subdirectory `break`s the enumeration early but still falls through to
//  FindClose()+RemoveDir(sDir) (RemoveDir on a non-empty directory then
//  typically fails, propagating as this call's own return value) rather than
//  returning false immediately.
// ---------------------------------------------------------------------------
bool DeleteDirectory(AnsiString sDir)
{
    if (DirectoryExists(sDir) == false)                  // BCB6 :251-254
    {
        return true;
    }

    AnsiString sDirFile = sDir + "\\*.*";                // BCB6 :256
    TSearchRec sr;
    if (FindFirst(sDirFile, faAnyFile, sr) == 0)          // BCB6 :258
    {
        do
        {
            if (!(sr.Attr & faDirectory))                // BCB6 :262-266 -- plain file
            {
                AnsiString sFilePath = sDir + "\\" + sr.Name;
                DeleteFile(sFilePath);
            }
            else                                          // BCB6 :267-275 -- directory entry
            {
                if (sr.Name != "." && sr.Name != "..")
                {
                    AnsiString sSubDir = sDir + "\\" + sr.Name;
                    if (DeleteDirectory(sSubDir) == false)
                        break;                            // BCB6 :273 -- see file-head quirk note
                }
            }
        } while (FindNext(sr) == 0);                      // BCB6 :276
        FindClose(sr);                                    // BCB6 :277
    }
    return RemoveDir(sDir);                               // BCB6 :279
}

// ###########################################################################
// #                                                                         #
// #   PT-W8 APPEND (20260811)  --  the four widget-coupled functions        #
// #                                                                         #
// ###########################################################################
//
//  NOTE ON THE FILE-HEAD BANNER ABOVE
//  ----------------------------------
//  Lines 9-13 ("Deferred (VCL-form-coupled): StringGrid_Insert_Row ->  W7 ...")
//  are SUPERSEDED by this section.  Three of the four are translated below;
//  StatusBar_ItemText is gated (GATE PTW8-EF-3) because the TStatusBar type
//  does not exist anywhere in this tree.  This wave was append-only on this
//  mirror and could not rewrite that banner, nor Public/ExternFunction.h's
//  lines 5-7 which say the same thing -- retiring both is a main-loop action
//  item, reported.
//
//  ORDERING NOTE: the four functions below are in GOLDEN order relative to each
//  other (:8, :53, :119, :222).  They necessarily sit AFTER DeleteDirectory
//  (golden :249) in this file because the wave was append-only; that is a file
//  layout artefact only.
//
//  ROLE
//  ----
//  These are golden's generic widget helpers -- row insert/delete on a
//  TStringGrid, one status-bar panel text setter, and a stopwatch read-out into
//  a TEdit.  Nothing pumps them: every one is called directly from a VCL form's
//  event handler (button click / grid edit), i.e. from the UI thread, so their
//  "pump" is the Windows message loop.  None of them touches the machine.
//
//  WAVE SCOPE (one line per golden function; golden file = Public/ExternFunction.cpp)
//  ---------------------------------------------------------------------------
//    StringGrid_Insert_Row  golden :8    ACTIVE  (gates PTW8-EF-1a, PTW8-EF-1b)
//    StringGrid_Delete_Row  golden :53   ACTIVE  (gates PTW8-EF-1a, PTW8-EF-1b, PTW8-EF-2)
//    StatusBar_ItemText     golden :119  GATED   (gate PTW8-EF-3 -- whole function, no TStatusBar type)
//    ShowRecordTime         golden :222  ACTIVE  (no gate -- vclcompat::TEdit has Text)
//
//  GATE REGISTER
//  -------------
//  --- GATE PTW8-EF-1a -- golden Public/ExternFunction.cpp:20 and :61 -------
//    GOLDEN LINE (both functions):
//      TempGrid=new TStringGrid((TForm*)pSG->ParentWindow);
//    ABSENCE CLAIM + COMMAND + TIME: vclcompat::TStringGrid (vclcompat/
//      StringGrid.h:88) has NO ParentWindow member and NO owner-taking ctor --
//      its only ctor is `explicit TStringGrid(int initialColCount = 5,
//      int initialRowCount = 5)` (StringGrid.h:95).  There is also no TForm
//      type in this tree to cast to:
//        grep -rn "FixedCols\|FixedRows\|ParentWindow" vclcompat/ forms/
//          (from D:/HT9045/HT9011UC_Cpp_V3.33.906.0)  ->  0 non-comment matches
//          [first run 2026-08-11T11:31+0800; RE-RUN before delivery
//           2026-08-11T11:57+0800 -- still 0]
//    WHAT WAS DONE: `TempGrid = new TStringGrid();`
//    WHY IT IS FAITHFUL: the owner argument is pure VCL lifetime plumbing --
//      it makes the TForm free the grid if the programmer forgets.  Golden does
//      not forget (both functions `delete TempGrid;` explicitly, golden :48 and
//      :88), and TempGrid is never parented, shown, or painted; it is used only
//      as a Cells[][] scratch buffer.  Dropping the owner therefore changes
//      nothing observable.  The default-constructed size (5 cols x 5 rows) is
//      the SAME state golden's `new TStringGrid(owner)` starts in: real VCL
//      TCustomGrid.Create sets ColCount:=5, RowCount:=5, FixedCols:=1,
//      FixedRows:=1, and golden never assigns TempGrid->ColCount.
//    REAL-MACHINE DIFFERENCE: none.
//
//  --- GATE PTW8-EF-1b -- golden :25, :27, :34, :36, :42, :44 (Insert_Row) --
//                          golden :66, :68, :74, :76           (Delete_Row) --
//    GOLDEN EXPRESSION (all of them): TempGrid->FixedCols
//    ABSENCE CLAIM: same command/time as PTW8-EF-1a -- vclcompat::TStringGrid
//      has no FixedCols member ("NO rendering, NO FixedRows/FixedCols/Options
//      /..." -- StringGrid.h:19-21 says so itself).
//    WHAT WAS DONE: replaced by the file-scope constant
//      kVclTStringGridDefaultFixedCols = 1 (defined below with its derivation).
//    WHY IT IS FAITHFUL: this is TempGrid's OWN FixedCols, not pSG's.  TempGrid
//      is constructed two statements earlier and golden never assigns its
//      FixedCols, so its value is provably the VCL design-time default, which
//      is 1 (Delphi Grids.pas TCustomGrid.Create: FFixedCols := 1).  The
//      constant is therefore the exact value golden evaluates, not an estimate.
//      NOTE this makes explicit a GOLDEN QUIRK that is easy to miss: because
//      the loop bounds come from TempGrid's DEFAULTS rather than from pSG,
//      both helpers only ever shift columns 1..4 of pSG (ColCount 5 -
//      FixedCols 1 = 4 columns starting at index 1) no matter how many columns
//      pSG actually has.  Preserved exactly -- see the per-function notes.
//    REAL-MACHINE DIFFERENCE: none (same constant, same quirk).
//
//  --- GATE PTW8-EF-2 -- golden Public/ExternFunction.cpp:91 ----------------
//    GOLDEN LINE:
//      else if(bKeepLastData && iRow==pSG->FixedRows)
//    ABSENCE CLAIM: same command/time as PTW8-EF-1a -- no FixedRows member.
//    WHAT WAS DONE: replaced by the file-scope constant
//      kVclTStringGridDefaultFixedRows = 1.
//    THIS ONE IS A REAL DEGRADATION, unlike EF-1a/EF-1b: pSG->FixedRows is a
//      PER-INSTANCE property of the CALLER's grid, set from that grid's .dfm,
//      not a freshly-constructed default.  The constant is correct only for a
//      grid that left FixedRows at the VCL default of 1 -- which is most of
//      them, but demonstrably not all: golden's own strGrdAlarmOld sets
//      FixedRows=0 (uHGemEquipment.dfm:375-383, quoted in
//      vclcompat/StringGrid.h:11).
//    HOW REAL-MACHINE BEHAVIOUR DIFFERS: for a grid whose .dfm sets FixedRows
//      to anything other than 1, a Delete_Row call with bKeepLastData==true and
//      iRow == that grid's real FixedRows takes golden's "keep the last row's
//      data, just report one fewer" branch, whereas here it falls through to
//      the final `else` and actually DECREMENTS RowCount.  Both branches clear
//      the last row identically; the difference is one row of grid height and a
//      returned count that is one larger.  No machine motion, no interlock.
//    MAIN-LOOP HAND-OFF: adding `int FixedRows; int FixedCols;` to
//      vclcompat::TStringGrid (additive, matching that header's own
//      "extend additively" instruction at StringGrid.h:24-26) retires EF-1b and
//      EF-2 together.  NOT done here -- vclcompat/ is outside this wave's
//      write boundary.
//
//  --- GATE PTW8-EF-3 -- golden Public/ExternFunction.cpp:119-124 -----------
//    WHOLE FUNCTION GATED: bool StatusBar_ItemText(TStatusBar*, int, AnsiString)
//    ABSENCE CLAIM + COMMAND + TIME: there is no TStatusBar type -- and no
//      TStatusPanel / Panels collection -- anywhere in this tree:
//        grep -rn "TStatusBar" --include=*.cpp --include=*.h .
//              | grep -v "^./build" | grep -v "^./tools/"    (piped, one line)
//          (from D:/HT9045/HT9011UC_Cpp_V3.33.906.0)  ->  0 matches
//        grep -rn "class TStatusBar\|struct TStatusBar" --include=*.h .
//          ->  0 matches
//          [run 2026-08-11T11:38+0800; RE-RUN before delivery
//           2026-08-11T11:57+0800 -- still 0]
//      (The 356 raw hits an unfiltered grep reports are ALL inside build*/ and
//      tools/dfm2rc/ generated .gen.cpp string tables -- the literal text
//      "TStatusBar" as .dfm class metadata, not a C++ type.  Recording that
//      here so the next agent does not have to re-derive it.)
//    WHY GATED RATHER THAN STOOD UP: the brief for this wave forbids inventing
//      a new widget struct and forbids editing vclcompat/ or forms/.  The
//      function's parameter type cannot even be named, so the SIGNATURE, not
//      just the body, has to be gated.
//    WHY IT SHOULD BE GATED (the trap-3 question, asked properly): golden's
//      body is a two-line bounds check plus one text assignment -- trivially
//      translatable the moment the type exists.  It is gated purely because the
//      type is missing, and for no other reason: there is no hidden hazard, no
//      hard-coded path, no ordering trap.  This one really is "publish the type
//      and un-gate it".
//    OFFLINE BEHAVIOUR / REAL-MACHINE DIFFERENCE: the function does not exist
//      offline, so any future caller will fail to LINK (loudly) rather than
//      silently no-op -- which is the correct posture for a symbol whose only
//      job is to paint text a headless build has nowhere to paint.  On a real
//      machine it writes sText into status-bar panel iIndex and returns true,
//      or returns false for an out-of-range index.
//
//  Toolchain: MinGW g++ 6.3+, C++17; verified with
//    g++ -std=c++17 -fsyntax-only -DMN200DLL_EXPORTS -D_WIN32_WINNT=0x0601
//        -DWINVER=0x0601 -I. -IMotor -IMotor/vendor -IEtherCAT/vendor
//        -Ithird_party/sqlite3 Public/ExternFunction.cpp
// ===========================================================================

// Includes for the PT-W8 append.  Placed HERE rather than at the top of the
// file because this wave was append-only on this mirror.
#include "vclcompat/Controls.h"     // TEdit (golden <StdCtrls.hpp>) -- Text lives on TCustomEdit
#include "vclcompat/StringGrid.h"   // TStringGrid (golden <Grids.hpp>)

#include <cstdlib>                  // atoi

//  StringGrid.h deliberately does NOT export its class into the global
//  namespace (see its footer note) -- consumers do it themselves.  This is the
//  same one-line `using` that forms/FormWidgets.h:96 already performs; done
//  here instead of #including that header so this low-level Public/ TU does not
//  acquire a dependency on the forms layer.  vclcompat/Controls.h:476 already
//  exports TEdit itself, so no `using` is needed for that one.
using vclcompat::TStringGrid;

// ---------------------------------------------------------------------------
//  VCL TStringGrid design-time defaults -- GATES PTW8-EF-1b and PTW8-EF-2.
//
//  Delphi/BCB6 Grids.pas TCustomGrid.Create initialises FColCount:=5,
//  FRowCount:=5, FFixedCols:=1, FFixedRows:=1.  vclcompat::TStringGrid already
//  replicates the first two (StringGrid.h:90-95 -- its ctor defaults are
//  5 and 5, and its own comment cites the same VCL default); it does not model
//  the fixed-row/column bands at all.  These two constants supply them.
//
//  For TempGrid (a freshly constructed scratch grid whose FixedCols golden
//  never assigns) the first constant is EXACT.  For pSG->FixedRows it is only
//  the common case -- see gate PTW8-EF-2 for the honest limits.
// ---------------------------------------------------------------------------
static const int kVclTStringGridDefaultFixedCols = 1;   // Grids.pas TCustomGrid.Create: FFixedCols := 1
static const int kVclTStringGridDefaultFixedRows = 1;   // Grids.pas TCustomGrid.Create: FFixedRows := 1

// ---------------------------------------------------------------------------
//  StringGrid_Insert_Row
//  golden Public/ExternFunction.cpp:8-51
//
//  Inserts a blank row after iRow and returns the grid's new RowCount.
//
//  If iRow is at (or past) the last row, it just grows the grid.  Otherwise it
//  copies rows [iRow+1 .. end) into a scratch grid, grows pSG by one, blanks
//  the inserted row, and writes the scratch rows back one row lower.
//
//  GOLDEN QUIRK PRESERVED (see gate PTW8-EF-1b): every column loop is bounded
//  by TempGrid's OWN default geometry (ColCount 5 - FixedCols 1), not by pSG's,
//  so only pSG columns 1..4 are ever touched.  A pSG with more than 5 columns
//  silently loses the shift on columns >= 5; a pSG with fewer than 5 columns
//  indexes out of range -- which raises ERangeError in BCB6 and throws
//  std::out_of_range here (vclcompat/StringGrid.h:43-50 documents that this
//  correspondence is deliberate).  Not fixed.
//
//  PROPERTY-INCREMENT NOTE: golden writes `pSG->RowCount++` on a BCB6
//  __property.  vclcompat models RowCount as an assignment proxy with no
//  operator++ (StringGrid.h:103-110), so the identical read-modify-write is
//  spelled `pSG->RowCount = pSG->RowCount + 1;`.  Same single resize, same
//  result -- a shim mechanic, not a behaviour change.
// ---------------------------------------------------------------------------
int StringGrid_Insert_Row(TStringGrid* pSG, int iRow)
{
    int iMaxRow=pSG->RowCount;                                                  // golden :10

    if(iRow>=iMaxRow-1)                                                         // golden :12
    {
        pSG->RowCount = pSG->RowCount + 1;                                      // golden :14  `pSG->RowCount++;`
    }
    else
    {
        iRow++;                                                                 // golden :18
        TStringGrid *TempGrid;                                                  // golden :19
        TempGrid=new TStringGrid();                                             // GATE PTW8-EF-1a -- golden :20
        TempGrid->RowCount=pSG->RowCount-iRow;                                  // golden :21

        for(int r=0; r<TempGrid->RowCount; r++)                                 // golden :23
        {
            for(int c=0; c<TempGrid->ColCount-kVclTStringGridDefaultFixedCols; c++)          // GATE PTW8-EF-1b -- golden :25
            {
                TempGrid->Cells[kVclTStringGridDefaultFixedCols+c][r]=pSG->Cells[kVclTStringGridDefaultFixedCols+c][r+iRow];   // GATE PTW8-EF-1b -- golden :27
            }
        }

        pSG->RowCount = pSG->RowCount + 1;                                      // golden :31  `pSG->RowCount++;`

        // Clear Insert Row Data {
        for(int c=0; c<TempGrid->ColCount-kVclTStringGridDefaultFixedCols; c++)              // GATE PTW8-EF-1b -- golden :34
        {
            pSG->Cells[kVclTStringGridDefaultFixedCols+c][iRow]="";                          // GATE PTW8-EF-1b -- golden :36
        }
        // }

        for(int r=0; r<TempGrid->RowCount; r++)                                 // golden :40
        {
            for(int c=0; c<TempGrid->ColCount-kVclTStringGridDefaultFixedCols; c++)          // GATE PTW8-EF-1b -- golden :42
            {
                pSG->Cells[kVclTStringGridDefaultFixedCols+c][r+iRow+1]=TempGrid->Cells[kVclTStringGridDefaultFixedCols+c][r]; // GATE PTW8-EF-1b -- golden :44
            }
        }

        delete TempGrid;                                                        // golden :48
    }
    return pSG->RowCount;                                                       // golden :50
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  StringGrid_Delete_Row
//  golden Public/ExternFunction.cpp:53-117
//
//  Deletes row iRow and returns the resulting logical row count.
//
//  Three branches, all of which blank the (old) last row first:
//    iRow < RowCount-1                -> shift rows up, then RowCount--
//    bKeepLastData && iRow==FixedRows -> DO NOT shrink; report RowCount-1
//    otherwise                        -> RowCount--
//
//  GOLDEN QUIRKS PRESERVED:
//   (a) the shift loops are bounded by TempGrid's own defaults (gate
//       PTW8-EF-1b) -- only pSG columns 1..4 move -- but the "Clear Last Row
//       Data" loops (golden :81, :94, :107) correctly use pSG->ColCount and
//       start at column 0.  So a delete on a 12-column grid blanks all 12
//       cells of the last row yet only shifts 4 of them.  That asymmetry is
//       golden's, and it is reproduced.
//   (b) in the first branch, `iMaxRow` is recomputed from pSG->RowCount AFTER
//       the decrement (golden :89), so it returns the new count; in the second
//       branch it is RowCount-1 with no decrement; in the third it is again the
//       post-decrement count.  Kept verbatim.
//
//  PROPERTY-DECREMENT NOTE: `pSG->RowCount--` is spelled
//  `pSG->RowCount = pSG->RowCount - 1;` for the same proxy reason described on
//  StringGrid_Insert_Row above.
// ---------------------------------------------------------------------------
int StringGrid_Delete_Row(TStringGrid* pSG, int iRow, bool bKeepLastData)
{
    int iMaxRow=pSG->RowCount;                                                  // golden :55

    if(iRow<iMaxRow-1)                                                          // golden :57
    {
        TStringGrid *TempGrid;                                                  // golden :59
        //TempGrid = new TStringGrid( pParent );                                // golden :60 (golden's own commented-out line, kept)
        TempGrid=new TStringGrid();                                             // GATE PTW8-EF-1a -- golden :61
        TempGrid->RowCount=pSG->RowCount-iRow;                                  // golden :62

        for(int r=0; r<TempGrid->RowCount; r++)                                 // golden :64
        {
            for(int c=0; c<TempGrid->ColCount-kVclTStringGridDefaultFixedCols; c++)          // GATE PTW8-EF-1b -- golden :66
            {
                TempGrid->Cells[kVclTStringGridDefaultFixedCols+c][r]=pSG->Cells[kVclTStringGridDefaultFixedCols+c][r+iRow+1]; // GATE PTW8-EF-1b -- golden :68
            }
        }

        for(int r=0; r<TempGrid->RowCount; r++)                                 // golden :72
        {
            for(int c=0; c<TempGrid->ColCount-kVclTStringGridDefaultFixedCols; c++)          // GATE PTW8-EF-1b -- golden :74
            {
                pSG->Cells[kVclTStringGridDefaultFixedCols+c][r+iRow]=TempGrid->Cells[kVclTStringGridDefaultFixedCols+c][r];   // GATE PTW8-EF-1b -- golden :76
            }
        }

        // Clear Last Row Data {
        for(int c=0; c<pSG->ColCount; c++)                                      // golden :81
        {
            pSG->Cells[c][pSG->RowCount-1]="";                                  // golden :83
        }
        // }

        pSG->RowCount = pSG->RowCount - 1;                                      // golden :87  `pSG->RowCount--;`
        delete TempGrid;                                                        // golden :88
        iMaxRow=pSG->RowCount;                                                  // golden :89
    }
    else if(bKeepLastData && iRow==kVclTStringGridDefaultFixedRows)             // GATE PTW8-EF-2 -- golden :91 `iRow==pSG->FixedRows`
    {
        // Clear Last Row Data {
        for(int c=0; c<pSG->ColCount; c++)                                      // golden :94
        {
            pSG->Cells[c][pSG->RowCount-1]="";                                  // golden :96
        }
        // }

        // Last Row Data is keep , but Data Size must subtract 1.
        iMaxRow=pSG->RowCount-1;                                                // golden :101
    }
    else
    {
        // Clear Last Row Data {
        for(int c=0; c<pSG->ColCount; c++)                                      // golden :107
        {
            pSG->Cells[c][pSG->RowCount-1]="";                                  // golden :109
        }
        // }

        pSG->RowCount = pSG->RowCount - 1;                                      // golden :112  `pSG->RowCount--;`

        iMaxRow=pSG->RowCount;                                                  // golden :114
    }
    return iMaxRow;                                                             // golden :116
}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  StatusBar_ItemText
//  golden Public/ExternFunction.cpp:119-124
//
//  GATE PTW8-EF-3 -- WHOLE FUNCTION GATED.  There is no TStatusBar type in this
//  tree (see the register in the banner above for the absence command, its
//  timestamp, and why the raw grep's 356 hits are all generated .dfm metadata).
//  The golden body is preserved verbatim below so that publishing a TStatusBar
//  stand-in in vclcompat/Controls.h is the only thing needed to un-gate it.
// ---------------------------------------------------------------------------
#if 0   // GATE PTW8-EF-3 -- golden :119-124.  No TStatusBar type exists; the parameter type cannot be named.
bool StatusBar_ItemText(TStatusBar* pSB, int iIndex, AnsiString sText)
{
    if(iIndex>=pSB->Panels->Count || iIndex<0)  return false;
    pSB->Panels->Items[iIndex]->Text=sText;
    return true;
}
#endif
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  ShowRecordTime
//  golden Public/ExternFunction.cpp:222-246
//
//  Reads the RecordTime() stopwatch (already translated above, golden :205) and
//  writes it into a TEdit, optionally keeping a running min (iType==1) or max
//  (iType==2) against whatever the edit already displayed.
//
//  vclcompat::TEdit (vclcompat/Controls.h:271, Text inherited from
//  TCustomEdit:264) carries exactly the one member this needs, so the body is a
//  direct transcription with no gate.
//
//  GOLDEN QUIRKS PRESERVED:
//   * iType values other than 0/1/2 fall through both branches and simply store
//     the raw elapsed value -- there is no default case and no validation.
//   * atoi() on a non-numeric or empty edit yields 0, so the very first "Min."
//     call (empty edit) clamps iMSec down to 0.  That is golden's behaviour.
//   * the unit is clock() TICKS, not milliseconds, despite the `iMSec` name --
//     see Public/ExternFunction.h's note on RecordTime.  Name kept as golden
//     has it.
// ---------------------------------------------------------------------------
void ShowRecordTime(TEdit* edt, int iType)
{
    int iMSec=RecordTime(false);                                                // golden :224

    if(edt!=NULL)                                                               // golden :226
    {
        if(iType==1)   // Min.                                                  // golden :228
        {
            int iMin=atoi(edt->Text.c_str());                                   // golden :230
            if(iMSec<iMin)
            {
                iMSec=iMin;
            }
        }
        else if(iType==2)   // Max.                                             // golden :235
        {
            int iMax=atoi(edt->Text.c_str());                                   // golden :237
            if(iMSec>iMax)
            {
                iMSec=iMax;
            }
        }
        edt->Text=AnsiString(iMSec);                                            // golden :244
    }
}
//---------------------------------------------------------------------------
