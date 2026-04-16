#include "MachineDefine.h"
#pragma hdrstop

#include "SgdToXLS.h"

#include "XLSfile.hpp"  //Steven 20140307 : ¥[§ÖCompiler

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
void __fastcall SGDToXLS(TStringGrid *strGrid, AnsiString Path)
{
    StringGridToXLS(strGrid, Path);
}
//---------------------------------------------------------------------------
