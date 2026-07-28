// ===========================================================================
//  uhome_layout.gen.cpp
//  DO NOT EDIT -- GENERATED FILE. See the matching _layout.gen.h banner.
// ===========================================================================
#include "uhome_layout.gen.h"

namespace ht9045_layout {

const DfmCustomProp kuhomeCustomProps[] = {
    { "LEDStyle", "LEDSqLarge" },
};

const DfmControlLayout kuhomeLayout[] = {
    { "fHome", "fHome", "", "TfHome", "", "", 0, 0, "ROOT", false, true, 661, true, 500, false, 0, false, 0, false, 0, -1, "", "", true, "MS Sans Serif", -11, "", true, -2147483640, "clWindowText", true, 12761254, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.palUnUse", "palUnUse", "IDC_UHOME_PALUNUSE", "TPanel", "fHome", "fHome", 0, 1, "LEAF", false, true, 532, true, 0, true, 185, true, 41, true, 1, 0, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel2", "Panel2", "IDD_UHOME_PANEL2", "TPanel", "fHome", "fHome", 1, 1, "SUBDLG", false, true, 136, true, 104, true, 393, true, 249, true, 3, -1, "", "", false, "", 0, "", false, 0, "", true, 9534289, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel2.Bevel1", "Bevel1", "IDC_UHOME_BEVEL1", "TBevel", "fHome.Panel2", "fHome.Panel2", 0, 2, "LEAF", true, true, 24, true, 24, true, 345, true, 97, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel2.Label26", "Label26", "IDC_UHOME_LABEL26", "TLabel", "fHome.Panel2", "fHome.Panel2", 1, 2, "LEAF", true, true, 128, true, 56, true, 136, true, 37, false, 0, -1, "", "", true, "MS Sans Serif", -32, "", true, 16777215, "clWhite", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel2.SpeedButton1", "SpeedButton1", "IDC_UHOME_SPEEDBUTTON1", "TSpeedButton", "fHome.Panel2", "fHome.Panel2", 2, 2, "LEAF", true, true, 104, true, 160, true, 201, true, 57, false, 0, -1, "", "", true, "新細明體", -32, "fsBold", true, -2147483640, "clWindowText", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel1", "Panel1", "IDD_UHOME_PANEL1", "TPanel", "fHome", "fHome", 2, 1, "SUBDLG", false, true, 0, true, 44, true, 1084, true, 460, true, 2, -1, "alClient", "", false, "", 0, "", false, 0, "", true, 12761254, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel1.Name", "Name", "IDC_UHOME_NAME", "TLabel", "fHome.Panel1", "fHome.Panel1", 0, 2, "LEAF", true, true, 24, true, 48, true, 28, true, 13, false, 0, 0, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel1.ledHome", "ledHome", "IDC_UHOME_LEDHOME", "TALed", "fHome.Panel1", "fHome.Panel1", 1, 2, "LEAF", true, true, 68, true, 44, true, 22, true, 22, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 1, &kuhomeCustomProps[0] },
    { "fHome.Panel1.edPos", "edPos", "IDC_UHOME_EDPOS", "TEdit", "fHome.Panel1", "fHome.Panel1", 2, 2, "LEAF", false, true, 100, true, 44, true, 121, true, 21, true, 3, 0, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel1.pnlLine1", "pnlLine1", "IDC_UHOME_PNLLINE1", "TPanel", "fHome.Panel1", "fHome.Panel1", 3, 2, "LEAF", false, true, 255, true, 0, true, 5, true, 500, true, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel1.pnlLine2", "pnlLine2", "IDC_UHOME_PNLLINE2", "TPanel", "fHome.Panel1", "fHome.Panel1", 4, 2, "LEAF", false, true, 525, true, 0, true, 5, true, 500, true, 1, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel1.pnlLine3", "pnlLine3", "IDC_UHOME_PNLLINE3", "TPanel", "fHome.Panel1", "fHome.Panel1", 5, 2, "LEAF", false, true, 795, true, 0, true, 5, true, 500, true, 2, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel104", "Panel104", "IDC_UHOME_PANEL104", "TPanel", "fHome", "fHome", 3, 1, "LEAF", false, true, 0, true, 0, true, 1084, true, 44, true, 0, -1, "alTop", "", true, "Arial", -19, "", true, -2147483643, "clWindow", true, 9534289, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel3", "Panel3", "IDD_UHOME_PANEL3", "TPanel", "fHome", "fHome", 4, 1, "SUBDLG", false, true, 0, true, 504, true, 1084, true, 122, true, 4, -1, "alBottom", "", false, "", 0, "", false, 0, "", true, 12761254, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel3.sbAbortHome", "sbAbortHome", "IDC_UHOME_SBABORTHOME", "TSpeedButton", "fHome.Panel3", "fHome.Panel3", 0, 2, "LEAF", true, true, 856, true, 4, true, 217, true, 117, false, 0, -1, "", "", true, "Arial", -24, "", true, -2147483640, "clWindowText", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Panel3.ListBox1", "ListBox1", "IDC_UHOME_LISTBOX1", "TListBox", "fHome.Panel3", "fHome.Panel3", 1, 2, "LEAF", false, true, 0, true, 0, true, 845, true, 122, true, 0, -1, "alLeft", "", true, "MS Sans Serif", -16, "", true, -2147483640, "clWindowText", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "fHome.Timer1", "Timer1", "", "TTimer", "fHome", "fHome", 5, 1, "NONVISUAL", false, true, 39, true, 13, false, 0, false, 0, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
};

const int kuhomeLayoutCount = 18;

} // namespace ht9045_layout
