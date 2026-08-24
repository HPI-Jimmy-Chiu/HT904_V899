// =============================================================================
//  Password.cpp  --  the ONE TfPassword method that needs an ht9045_sm-layer
//  symbol: sbPasswordModifyOKClick (ShowMyMessage -> canary_support.h:80,
//  body canary_support.cpp -- ht9045_sm).
//
//  AI(W906-FW-QWKEY1) 20260824: new file, translation wave FW-QWKEY1.
//  Golden source of truth: HT9011UC_Code_V3.33.906.0_20260618/
//  Password.cpp:284-297 (cp950, 0 U+FFFD, this wave).  WHY THIS FILE EXISTS
//  SEPARATELY FROM forms/fPassword.cpp: read forms/fPassword.h's banner
//  (sbPasswordModifyOKClick entry) -- the same root-file split cSetUp.cpp
//  established, and cSetUp.cpp::cbI21Click is the existing precedent for
//  resolving this exact ShowMyMessage symbol from a root ht9045_sm TU.
//
//  The EvenLogUSER.PassWord[..] write below is a pure IN-PROCESS assignment
//  and stays ACTIVE; the on-disk persistence it feeds, SavePasswordFile(),
//  has its entire write block gated (forms/fPassword.cpp GATE (P-S1)), so
//  calling it here is a faithful no-op today.
//
//  CMAKE STATUS -- NOT WIRED by this wave (CMakeLists.txt outside the write
//  boundary).  Suggested: ht9045_sm source list, next to cSetUp.cpp.
// =============================================================================
#include "forms/fPassword.h"

#include "Config.h"             // IniConfig.bA02DisableSaveParsWhenSwitchToOp (:274)
#include "cmydef.h"             // AccessLevel (:3503)
#include "canary_support.h"     // ShowMyMessage (:80)

//---------------------------------------------------------------------------
void TfPassword::sbPasswordModifyOKClick()                                      // golden :284-297 -- `TObject *Sender` dropped, never read
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
    }
    else if(edModifyPassword->Text==edReModifyPassword->Text)
    {
        EvenLogUSER.PassWord[cob_UseIDList->ItemIndex]=edModifyPassword->Text;
        SavePasswordFile();                                                     // write block gated inside -- GATE (P-S1), forms/fPassword.cpp
    }
    Close();
}
//---------------------------------------------------------------------------
