// =============================================================================
//  SgdToXLS.h  --  one-function facade: dump a TStringGrid to a raw .xls file.
//
//  Faithful translation of golden SgdToXLS.h (11 lines, BCB6, Big5/cp950 --
//  this file has NO Chinese text at all).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("tiny" group -- five very small units).
//
//  ROLE: declares the single free function SGDToXLS(TStringGrid*, AnsiString),
//  a thin forwarder to XLSFile.pas's StringGridToXLS -- see SgdToXLS.cpp's
//  banner for the full gate rationale (StringGridToXLS is Object Pascal, not
//  C++, and out of this wave's authorized scope; there is no substrate for it
//  anywhere in this port tree).
//
//  golden :6 `#include <Grids.hpp>` (for TStringGrid) -> vclcompat/StringGrid.h
//  (`vclcompat::TStringGrid`), the same substitution cMyDB.h:66 already makes
//  for the same VCL type.
//
//  NO __fastcall.  golden :8 carries `void __fastcall SGDToXLS(...)`;
//  __fastcall is a REAL MinGW keyword and a declaration/definition calling-
//  convention mismatch would produce two differently-mangled symbols, so it
//  is dropped from BOTH sides (vclcompat/vcl_compat.h:24-54), same as every
//  other ported unit.
//
//  Big5: this file has no Chinese text at all. Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef SgdToXLSH
#define SgdToXLSH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"   // AnsiString
#include "vclcompat/StringGrid.h"   //AI(W906-PT-W2) 20260807: was <Grids.hpp> (TStringGrid) -- same substitution cMyDB.h:66 makes

using vclcompat::TStringGrid;

void SGDToXLS(TStringGrid *strGrid, AnsiString Path);

#endif
