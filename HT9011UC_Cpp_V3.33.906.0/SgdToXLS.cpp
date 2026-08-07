// =============================================================================
//  SgdToXLS.cpp  --  one-function facade: dump a TStringGrid to a raw .xls
//                    file (BIFF5 binary), forwarding to XLSFile.pas's
//                    StringGridToXLS.
//
//  Faithful translation of golden SgdToXLS.cpp (17 lines, BCB6, Big5/cp950 --
//  one Chinese comment, golden :6, decoded and preserved below).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//
//  ROLE: SGDToXLS(TStringGrid*, AnsiString) is a one-line forwarder to
//  StringGridToXLS(grid, fname) -- the entire unit exists only to let C++
//  callers (cObserver.cpp, main.cpp, SECSGEM/uHGemEquipment.cpp,
//  uLotInfo.cpp, per a whole-golden-tree grep) reach that Pascal routine
//  without including <Grids.hpp>/<XLSfile.hpp> everywhere themselves.
//
//  WAVE SCOPE -- ACTIVE (golden's own body, verbatim) vs GATE:
//    SGDToXLS(TStringGrid*, AnsiString)   golden :12-15.  ACTIVE: the
//      function itself (the one golden function this file defines) is
//      translated verbatim -- it is nothing but the forwarding call.
//
//  GATE REGISTER -- exactly ONE gate.
//   (1) StringGridToXLS(grid, fname)   golden :14; real implementation
//       golden XLSFile.pas (procedure StringGridToXLS, XLSFile.pas -- a
//       hand-rolled BIFF5 .xls binary writer: TXLSWriter opens fname,
//       writes a BOF/DIMENSIONS/cell-stream/EOF record sequence, one
//       CellStr() per grid cell, capped at TXLSWriter's 65535-row BIFF5
//       limit). THIS IS OBJECT PASCAL, NOT C++ -- a different source
//       language entirely, not merely an untranslated C++ TU. Grepped the
//       WHOLE port tree for `StringGridToXLS` / `XLSfile`: the only hits are
//       two comments (MachineDefine.h:83, MachineType.h:122) noting that
//       golden's own XLSfile.hpp include and USEUNIT("XLSfile.pas") linkage
//       directive were never carried into the portable W0 umbrella -- zero
//       compiled bodies, in this or any prior wave. Translating a raw BIFF5
//       binary-format writer is a distinct, non-trivial unit of work (byte-
//       level file-format engineering, not mechanical BCB6->C++17 syntax
//       translation) and is out of this wave's authorized scope (one .pas
//       file plus a new binary-writer class is not "this unit's own two
//       files"). Handled with the tree's #if 0/#else macro-pair idiom:
//       golden's call stays VERBATIM in the #if 0 arm; the ACTIVE arm is a
//       no-op.
//       WHY NO-OP IS THE FAITHFUL DEFAULT: SGDToXLS's own contract is
//       "write grid to an .xls file at Path" with a `void` return -- there is
//       no success/failure signal for a caller to branch on (grepped all
//       four golden call sites: none inspect a return value or check for the
//       file's existence afterward; they all fire-and-forget after a
//       "Export to Excel" style UI action). A no-op therefore changes no
//       CONTROL FLOW anywhere -- the only observable difference is that the
//       target .xls file is not created/overwritten on disk.
//       BEHAVIOUR DELTA, STATED PLAINLY: on a real machine, calling
//       SGDToXLS() produces a BIFF5 .xls file at the given Path containing
//       one string cell per grid cell (capped at 65535 rows); this port
//       leaves that file untouched (neither created nor modified). Retire
//       this gate by translating XLSFile.pas's TXLSWriter/StringGridToXLS
//       into a C++ BIFF5 writer (or, more simply, swapping the file FORMAT
//       for a CSV/XLSX writer if a future wave decides format fidelity is
//       not required, but that would be a DELIBERATE deviation from golden,
//       not a mechanical translation, and is explicitly NOT done here).
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :2 and :9). No __fastcall / __property / __published
//  survive (dropped from SGDToXLS's own definition, matching
//  vclcompat/vcl_compat.h:24-54's established treatment). golden :6
//  `#include "XLSfile.hpp"` dropped along with the gated call it existed
//  for. SOFT_SIMULTE is not referenced by this unit. No numeric semantics in
//  this file to preserve or alter (the whole body is one forwarding call).
//
//  Big5: golden's one Chinese comment (golden :6, "Steven 20140307 :
//  加快Compiler" -- "speed up the compiler", referring to why XLSfile.hpp was
//  included directly rather than through a god-header) decoded via cp950 and
//  preserved as UTF-8 below, moved to sit beside the gated #include it
//  originally annotated (see GATE (1)). Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "SgdToXLS.h"
//   #include "XLSfile.hpp"  //Steven 20140307 : 加快Compiler
//   ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + portable STL + using namespace std)
#include "SgdToXLS.h"               // this unit's own contract (SGDToXLS, defined below)
// GATE (1): golden :6 `#include "XLSfile.hpp"` -- Steven 20140307 : 加快Compiler
// ("speed up the compiler"). XLSfile.hpp/.pas (StringGridToXLS's real BIFF5
// writer) has no C++ port anywhere in this tree -- see the gate register
// above. Not included here; the call it would enable is gated at its own
// site below instead.
//---------------------------------------------------------------------------
void SGDToXLS(TStringGrid *strGrid, AnsiString Path)
{
#if 0 // TODO(XLSFile.pas): StringGridToXLS is Object Pascal, no C++ substrate -- see file head GATE (1).
    StringGridToXLS(strGrid, Path);
#else
    (void)strGrid; (void)Path;
#endif
}
//---------------------------------------------------------------------------
