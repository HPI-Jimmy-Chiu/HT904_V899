// =============================================================================
//  myQwertyKeyBoard.cpp  --  the ONE TfQwertyKey method that needs
//  ht9045_sm-layer symbols: FormDestroy (MyDBIProcess -> aHotPlateSubstrate
//  .cpp, LogSoftwareOffTime -> acarry_shims.cpp:255, both ht9045_sm).
//
//  AI(W906-FW-QWKEY1) 20260824: new file, translation wave FW-QWKEY1.
//  Golden source of truth: HT9011UC_Code_V3.33.906.0_20260618/
//  myQwertyKeyBoard.cpp:152-167 (cp950, 0 U+FFFD, this wave).  WHY THIS FILE
//  EXISTS SEPARATELY FROM forms/fQwertyKey.cpp: read forms/fQwertyKey.h's
//  banner (FormDestroy entry) -- the same root-file split cSetUp.cpp /
//  cTemperFrom.cpp established for methods whose dependencies live above the
//  ht9045_forms link diet; TfSetup::FormDestroy (cSetUp.cpp:538) already
//  resolves these exact two symbols this exact way.
//
//  CMAKE STATUS -- NOT WIRED by this wave (CMakeLists.txt outside the write
//  boundary).  Suggested: ht9045_sm source list, next to cSetUp.cpp.
// =============================================================================
#include "forms/fQwertyKey.h"

#include "MachineType.h"            // vec_clr (:1610) + `using std::vector` (:7)
#include "cmydef.h"                 // LogSoftwareOffTime (:5032; offline no-op body acarry_shims.cpp:255)
#include "aHotPlateSubstrate.h"     // MyDBIProcess(AnsiString,AnsiString) (:933) -- the 177-TU header, NOT mykitsuck.h (two-TMyKitSuck gotcha)

//---------------------------------------------------------------------------
void TfQwertyKey::FormDestroy()                                                 // golden :152-167 -- `TObject *Sender` dropped, never read
{
    try
    {
        for(vector<TQwertyKeyClass *>::iterator iter=QwertyKey.begin(); iter!=QwertyKey.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(QwertyKey);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfQwertyKey::FormDestroy");
    }
    LogSoftwareOffTime("TfQwertyKey, FormDestroy");                             //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
