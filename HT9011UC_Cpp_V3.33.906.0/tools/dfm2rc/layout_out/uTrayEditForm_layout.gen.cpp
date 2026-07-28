// ===========================================================================
//  uTrayEditForm_layout.gen.cpp
//  DO NOT EDIT -- GENERATED FILE. See the matching _layout.gen.h banner.
// ===========================================================================
#include "uTrayEditForm_layout.gen.h"

namespace ht9045_layout {

const DfmCustomProp kuTrayEditFormCustomProps[] = {
    { "XItem", "5" },
    { "YItem", "8" },
};

const DfmControlLayout kuTrayEditFormLayout[] = {
    { "TrayEditForm", "TrayEditForm", "", "TTrayEditForm", "", "", 0, 0, "ROOT", false, true, 640, true, 45, true, 520, true, 699, false, 0, -1, "", "", true, "MS Sans Serif", -11, "", true, -2147483640, "clWindowText", true, 12761254, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.spbUpdate", "spbUpdate", "IDC_UTRAYEDITFORM_SPBUPDATE", "TSpeedButton", "TrayEditForm", "TrayEditForm", 0, 1, "LEAF", true, true, 224, true, 576, true, 137, true, 33, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.SpeedButton2", "SpeedButton2", "IDC_UTRAYEDITFORM_SPEEDBUTTON2", "TSpeedButton", "TrayEditForm", "TrayEditForm", 1, 1, "LEAF", true, true, 368, true, 576, true, 129, true, 33, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.Label1", "Label1", "IDC_UTRAYEDITFORM_LABEL1", "TLabel", "TrayEditForm", "TrayEditForm", 2, 1, "LEAF", true, true, 176, true, 636, true, 7, true, 13, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.Label2", "Label2", "IDC_UTRAYEDITFORM_LABEL2", "TLabel", "TrayEditForm", "TrayEditForm", 3, 1, "LEAF", true, true, 24, true, 636, true, 7, true, 13, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.mtLoaderBuffer", "mtLoaderBuffer", "IDC_UTRAYEDITFORM_MTLOADERBUFFER", "TTMyTray", "TrayEditForm", "TrayEditForm", 4, 1, "LEAF", false, true, 6, true, 2, true, 491, true, 567, false, 0, -1, "", "", false, "", 0, "", false, 0, "", true, 16777215, "clWhite", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 2, &kuTrayEditFormCustomProps[0] },
    { "TrayEditForm.edtYPos", "edtYPos", "IDC_UTRAYEDITFORM_EDTYPOS", "TEdit", "TrayEditForm", "TrayEditForm", 5, 1, "LEAF", false, true, 200, true, 632, true, 121, true, 24, true, 6, -1, "", "", true, "MS Sans Serif", -13, "", true, -2147483640, "clWindowText", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.edtXPos", "edtXPos", "IDC_UTRAYEDITFORM_EDTXPOS", "TEdit", "TrayEditForm", "TrayEditForm", 6, 1, "LEAF", false, true, 48, true, 632, true, 121, true, 24, true, 5, -1, "", "", true, "MS Sans Serif", -13, "", true, -2147483640, "clWindowText", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.Button1", "Button1", "IDC_UTRAYEDITFORM_BUTTON1", "TButton", "TrayEditForm", "TrayEditForm", 7, 1, "LEAF", false, true, 336, true, 632, true, 113, true, 25, true, 7, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.BitBtn1", "BitBtn1", "IDC_UTRAYEDITFORM_BITBTN1", "TBitBtn", "TrayEditForm", "TrayEditForm", 8, 1, "LEAF", false, true, 8, true, 568, true, 97, true, 25, true, 1, 0, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.Button2", "Button2", "IDC_UTRAYEDITFORM_BUTTON2", "TButton", "TrayEditForm", "TrayEditForm", 9, 1, "LEAF", false, true, 108, true, 568, true, 75, true, 25, true, 2, 0, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.CheckBox1", "CheckBox1", "IDC_UTRAYEDITFORM_CHECKBOX1", "TCheckBox", "TrayEditForm", "TrayEditForm", 10, 1, "LEAF", false, true, 384, true, 612, true, 97, true, 17, true, 4, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.cbBinCount", "cbBinCount", "IDC_UTRAYEDITFORM_CBBINCOUNT", "TComboBox", "TrayEditForm", "TrayEditForm", 11, 1, "LEAF", false, true, 8, true, 600, true, 145, true, 21, true, 3, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "TrayEditForm.Timer1", "Timer1", "", "TTimer", "TrayEditForm", "TrayEditForm", 12, 1, "NONVISUAL", false, true, 96, true, 8, false, 0, false, 0, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
};

const int kuTrayEditFormLayoutCount = 14;

} // namespace ht9045_layout
