#include "MachineDefine.h"
#pragma hdrstop

#include "LastSet.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
//本檔案裡的結構請不要任意加東西
//就算要加也是加在最下面
//有加東西請跟大家說一下~~
//---------------------------------------------------------------------------
LAST_GENERAL_SET LastSet;
TECH Tech;
INFO *CmdData;          //JerryYang 20181126 (Steven) : support Epson DLL function
ALIGNTEACH AlignTeach;  //Sam 20181201 : Auto Alignment
