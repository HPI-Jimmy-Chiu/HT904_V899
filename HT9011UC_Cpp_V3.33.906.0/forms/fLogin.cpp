// =============================================================================
//  forms/fLogin.cpp  --  TfLogin bodies (ht9045_forms link diet: vclcompat +
//  ht9045_globals + ht9045_core only -- every ACTIVE line below stays inside
//  that diet, verified per DEPENDENCY AUDIT in forms/fLogin.h).
//
//  AI(W906-FW3-LGM1) 20260828: new file, wave FW3-LGM1.
//  Golden source of truth: HT9011UC_Code_V3.33.906.0_20260618/login.cpp
//  (88 lines, cp950, 0 U+FFFD, this wave). Full banner -- scope table, GATE
//  REGISTER (LG-1), GOLDEN NOTES (G-L1/G-L2), DEVIATIONS, DEPENDENCY AUDIT,
//  CMAKE STATUS -- lives in forms/fLogin.h; this file carries per-statement
//  pointer-back comments only.
//
//  CMAKE STATUS -- NOT WIRED by this wave. See forms/fLogin.h.
// =============================================================================
#include "forms/fLogin.h"
#include "cmydef.h"        // pwPath (:3314), CUSTOMER_CODE (:3181), AccessLevel (:3503), iDefEngineerLevel (:3590)
#include "MachineType.h"   // CC_AMKOR_China (:348), CC_QUALCOMM (:377), CC_Greatek (:330)
#include "Config.h"        // IniConfig (:1499), .bN15UseESDControlMachine (:1125)
#include "vclcompat/SysUtils.h"   // FileExists (:90)

// golden login.cpp:9 -- constructed via `new` (header GLOBAL POINTER note);
// ctor-safe, see below.
TfLogin *fLogin = new TfLogin();

//---------------------------------------------------------------------------
// AI(W906-FW3-LGM1) 20260828: ctor-safety rule -- fields and this object's
// own widget stand-ins only (all `new`'d as in-class initializers in the
// header), no other global dereferenced. Matches golden exactly: the golden
// body (login.cpp:12-15) is empty besides the `: TForm(Owner)` base-class
// call, so there is nothing to move to a separate Init().
TfLogin::TfLogin()
{
}
//---------------------------------------------------------------------------
void TfLogin::FormShow()                                                       // golden :17-67 -- `TObject *Sender` dropped, never read
{
    Left = (1024 - Width) / 2;
    Top  = (768  - Height) / 2;

    edUserName->Text         = "";
    edLoginOldPassword->Text = "";
    edLoginNewPassword->Text = "";

    //jou 2011-11-24 修正login視窗開啟預設模式
    if(FileExists(pwPath))          //2012-01-03    Dell modify
    {
        labNewPassword->Visible     = false;
        edLoginNewPassword->Visible = false;

        if(CUSTOMER_CODE==CC_AMKOR_China || CUSTOMER_CODE==CC_QUALCOMM)   //JerryYang 20170412 (Steven) add QUALCOMM
        {
            if(AccessLevel<=iDefEngineerLevel)  //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel  -- see header GOLDEN NOTE (G-L1)
            {
                rgLoginOption->ItemIndex = 2;
                rgLoginOption->Visible   = false;
                rgLoginOptionClick();                                          // golden `rgLoginOptionClick(this);` -- Sender dropped (D-2)
            }
            else
            {
                rgLoginOption->ItemIndex = 0;
                rgLoginOption->Visible   = true;
            }
        }
        else
        {
            rgLoginOption->ItemIndex = 0;
        }
    }
    else
    {
        rgLoginOption->Visible = false;
    }
    //Sam 20170824 (Steven) 為超豐 ESD Control 功能 form HT-7045
    //==>
    if(CUSTOMER_CODE==CC_Greatek)
    {
        if(IniConfig.bN15UseESDControlMachine==true)
        {
            // AI(W906-FW3-LGM1) 20260828: GATE (LG-1) -- `fProductionInfo->
            // ScreenkeyboardShow()` has no port anywhere in this tree
            // (forms/fProductionInfo.h declares no such member). See header
            // GATE REGISTER (LG-1).
#if 0
            fProductionInfo->ScreenkeyboardShow();
#endif
        }
    }
    //<==
    //Sam 20170824 (Steven) 為超豐 ESD Control 功能 form HT-7045
    edLoginOldPassword->SetFocus();              //2012-01-03    Dell Fix
}
//---------------------------------------------------------------------------
void TfLogin::sbOkClick()                                                      // golden :69-75 -- `TObject *Sender` dropped, never read
{
    if(cbLoginUserName->Visible)                                              // see header GOLDEN NOTE (G-L2)
        edUserName->Text = cbLoginUserName->Text;
    sbOk->Down = false;
    Close();
}
//---------------------------------------------------------------------------
void TfLogin::rgLoginOptionClick()                                             // golden :77-102 -- `TObject *Sender` dropped, never read
{
    if(rgLoginOption->ItemIndex==1)        //2012-01-03    Dell modify 刪除不需要password
    {
        labOldPassword->Visible     = false;
        edLoginOldPassword->Visible = false;
    }
    else
    {
        labOldPassword->Visible     = true;
        edLoginOldPassword->Visible = true;
    }

    if(rgLoginOption->ItemIndex==2)
    {
        labNewPassword->Visible     = true;
        edLoginNewPassword->Visible = true;
        labOldPassword->Caption     = "OldPassword";
    }
    else
    {
        labNewPassword->Visible     = false;
        edLoginNewPassword->Visible = false;
        labOldPassword->Caption     = "Password";
    }
}
//---------------------------------------------------------------------------
