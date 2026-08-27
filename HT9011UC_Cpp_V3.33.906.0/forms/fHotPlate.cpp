// =============================================================================
//  forms/fHotPlate.cpp  --  definitions for the fHotPlate facade
//
//  AI(W906-FW3-HSP1) 20260828: new file, FW wave FW3-HSP1. See forms/
//  fHotPlate.h for the full GATE REGISTER / ACTIVE evidence / DEVIATIONS.
//  Includes listed below with the same "what each is for" convention as
//  forms/fCleaning.cpp:8-18 / forms/fLotInfo.cpp:18-36.
// =============================================================================
#include "forms/fHotPlate.h"
#include "cprod.h"              // TestIF_File/TestIF/HotPlateForm/HotPlateForm_File (cprod.cpp, ht9045_globals)
#include "cmydef.h"             // HOT_PLATE_POSITION/iHPWideHP/iHPNoUse8suck/USE_PRECISER/iPreciserInstallArea (cmydef.cpp, ht9045_globals)
#include "Config.h"             // IniConfig (Config.cpp, ht9045_globals)
#include "CosFunction.h"        // CosFunction.bAutoCleanUseHPSetByRecipe (CosFunction.cpp, ht9045_globals)
#include "common.h"             // OnlyNumberInPut/OnlyNumberAndDotInPut (common.cpp, ht9045_core)
#include "forms/fQwertyKey.h"   // fQwertyKey->ShowQwertyKey (real, forms/fQwertyKey.cpp, ht9045_forms)
#include "vclcompat/SysUtils.h" // FormatFloat (vclcompat/SysUtils.cpp, vclcompat)

// AI(W906-FW3-HSP1) 20260828: TfHotPlate/fHotPlate were FREE tree-wide --
// same idiom as forms/fCleaning.cpp:45 (`TfCleaning *fCleaning = new
// TfCleaning();`). Ctor is a plain field-only body (golden :28-31 is empty
// past `: TForm(Owner)`), so this static-init `new` touches nothing beyond
// its own already-`new`'d widget stand-ins -- no SIOF risk (docs/
// KNOWLEDGE.md "static-init ctor 不可碰 NULL 全域").
TfHotPlate *fHotPlate = new TfHotPlate();

TfHotPlate::TfHotPlate()
{
    // golden :28-31 -- empty ctor body past `: TForm(Owner)`. Widget fields
    // are NSDMI-`new`'d in the header (matches forms/fCleaning.cpp's
    // established shape for this class of trivial ctor).
}

// ---------------------------------------------------------------------------
void TfHotPlate::DoIniDataToForm()   // golden :310-365
{
    HotPlateName->Text = HotPlateForm_File.Alias;

    XST1->Text    = FormatFloat("0.000", HotPlateForm_File.XStart);   // golden :314
    YST1->Text    = FormatFloat("0.000", HotPlateForm_File.YStart);
    XPitch1->Text = FormatFloat("0.000", HotPlateForm_File.XPitch);
    YPitch1->Text = FormatFloat("0.000", HotPlateForm_File.YPitch);

    XCT1->Text = HotPlateForm_File.XDivision;
    YCT1->Text = HotPlateForm_File.YDivision;

    if (CosFunction.bAutoCleanUseHPSetByRecipe)                                 // golden :322
    {
        if (TestIF_File.iAutoClean_Function && IniConfig.bE43AutoCleanUseHotplate)
        {
            cbEnableHP1->Visible = false;
        }
        else
        {
            cbEnableHP1->Visible = true;
        }
    }
    else
    {
        if (IniConfig.bE43AutoCleanUseHotplate)
        {
            cbEnableHP1->Visible = false;
        }
        else
        {
            cbEnableHP1->Visible = true;
        }
    }

    if (USE_PRECISER == 1 && iPreciserInstallArea == 2)                         // golden :346
    {
        cbEnableHP1->Visible = false;
    }

    cbEnableHP2->Checked = HotPlateForm_File.iPlateSelect & 0x02;
    cbEnableHP1->Checked = HotPlateForm_File.iPlateSelect & 0x01;

    if (IniConfig.bHotPlateMove1CM)                                             // golden :354
    {
        chkUseWideHotplate->Checked = HotPlateForm_File.bUseWideHotplate;
    }
    else
    {
        chkUseWideHotplate->Checked = true;
    }

    if (IniConfig.bVTESTFunction == true)                                      // golden :363
        chkTrayHotplateCheck->Checked = HotPlateForm_File.bTrayHotplateCheck;
}

// ---------------------------------------------------------------------------
void TfHotPlate::XST1KeyPress(char &Key)   // golden :367-371
{
    if (OnlyNumberAndDotInPut(Key) == false)
        Key = 0;   // golden `Key=NULL;` on a char -- faithful as 0
}

// ---------------------------------------------------------------------------
void TfHotPlate::YCT1KeyPress(char &Key)   // golden :373-377
{
    if (OnlyNumberInPut(Key) == false)
        Key = 0;
}

// ---------------------------------------------------------------------------
void TfHotPlate::HotPlateNameMouseDown(TObject *Sender, TMouseButton Button,
                                        TShiftState Shift, int X, int Y)   // golden :633-637
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
}

// ---------------------------------------------------------------------------
int TfHotPlate::CheckHotPlate8PickMode()   // golden :639-680, JerryYang 20161007 簡化Hot plate offset判斷式
{
    double dHotPlateWidth = (2 * HotPlateForm_File.XStart +
                              HotPlateForm_File.XPitch * (HotPlateForm_File.XDivision - 1));
    if (IniConfig.bHotPlateMove1CM &&
        HotPlateForm.bUseWideHotplate == false)                                 // ChungHung Add 20101025 CC_ASE_CL HotPlate Offset 10mm
    {
        return iHPWideHP;
    }
    else if (HOT_PLATE_POSITION == 1 && dHotPlateWidth <= 170)
    {
        if (TestIF.iTestMode == QualSite2X2 &&
            TestIF.iUseSuckMode == 8 &&
            HotPlateForm_File.XDivision == 6)                                   // JerryYang 20161222 (jou) 目前不支援6x11 2x2_8吸嘴
        {
            return iHPNoUse8suck;
        }
        else
        {
            return iHPWideHP;
        }
    }
    else if (HotPlateForm_File.XDivision == 6 &&
             HotPlateForm_File.XPitch > 27.0)                                   // Steven 20180516 : 6x11加大 X-Pitch 35mm
    {
        return iHPWideHP;
    }
    else if (TestIF.iTestMode == DualSite &&
             HOT_PLATE_POSITION == 1)                                           // JerryYang 20170603 (wei) hot plate pin在右邊裝Dual site 4吸嘴
    {
        return iHPWideHP;
    }
    else if (TestIF.iTestMode == QualSite2X2 &&
             TestIF.iUseSuckMode == 8 &&
             HotPlateForm_File.XDivision == 12)                                 // Steven 20180409 (Jou) : Add 2x2 support X=12
    {
        return iHPWideHP;
    }
    else
    {
        return iHPNoUse8suck;
    }
}

// ---------------------------------------------------------------------------
void TfHotPlate::sbtExitClick(TObject *Sender)   // golden :627-631
{
    (void)Sender;
    sbtExit->Down = false;
    Close();   // DEVIATION D-3 -- port-only no-op (golden inherited TForm::Close)
}

// -- GATE REGISTER -- see forms/fHotPlate.h for the full reasoning per entry.
// FormShow / ReadFile / FormClose / cbSelectHPFromDBChange / spbSaveClick /
// SaveSetupFile / XST1MouseDown / XCT1MouseDown / ShowTypePage /
// SetArmHotPlateYPitch / LoadImage are declared in forms/fHotPlate.h and
// intentionally have NO definition here -- calling any of them is a LINK
// ERROR by design (same idiom as forms/fMesSystem.cpp's GATED bucket).
