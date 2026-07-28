// ===========================================================================
//  LoadCCD/LoadCCDMap_layout.gen.cpp
//  DO NOT EDIT -- GENERATED FILE. See the matching _layout.gen.h banner.
// ===========================================================================
#include "LoadCCDMap_layout.gen.h"

namespace ht9045_layout {

const DfmCustomProp kLoadCCDMapCustomProps[] = {
    { "LEDStyle", "LEDSqLarge" },
    { "Interval", "500" },
};

const DfmControlLayout kLoadCCDMapLayout[] = {
    { "LoadCCD", "LoadCCD", "", "TLoadCCD", "", "", 0, 0, "ROOT", false, true, 354, true, 257, true, 759, true, 603, false, 0, -1, "", "", true, "MS Sans Serif", -11, "", true, -2147483640, "clWindowText", true, 12761254, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1", "Panel1", "IDD_LOADCCDMAP_PANEL1", "TPanel", "LoadCCD", "LoadCCD", 0, 1, "SUBDLG", false, true, 0, true, 0, true, 751, true, 572, true, 0, -1, "alClient", "", false, "", 0, "", false, 0, "", true, 12761254, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1.Label1", "Label1", "IDC_LOADCCDMAP_LABEL1", "TLabel", "LoadCCD.Panel1", "LoadCCD.Panel1", 0, 2, "LEAF", true, true, 3, true, 24, true, 19, true, 13, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1.LedAConnect", "LedAConnect", "IDC_LOADCCDMAP_LEDACONNECT", "TMyLed", "LoadCCD.Panel1", "LoadCCD.Panel1", 1, 2, "LEAF", true, true, 331, true, 13, true, 23, true, 23, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 2, &kLoadCCDMapCustomProps[0] },
    { "LoadCCD.Panel1.edtPort", "edtPort", "IDC_LOADCCDMAP_EDTPORT", "TEdit", "LoadCCD.Panel1", "LoadCCD.Panel1", 2, 2, "LEAF", false, true, 51, true, 16, true, 121, true, 21, true, 1, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1.btnConnect", "btnConnect", "IDC_LOADCCDMAP_BTNCONNECT", "TButton", "LoadCCD.Panel1", "LoadCCD.Panel1", 3, 2, "LEAF", false, true, 191, true, 12, true, 75, true, 29, true, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1.Memo1", "Memo1", "IDC_LOADCCDMAP_MEMO1", "TMemo", "LoadCCD.Panel1", "LoadCCD.Panel1", 4, 2, "LEAF", false, true, 8, true, 56, true, 601, true, 497, true, 3, -1, "", "", false, "", 0, "", false, 0, "", true, 14670284, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1.CCD1", "CCD1", "IDC_LOADCCDMAP_CCD1", "TButton", "LoadCCD.Panel1", "LoadCCD.Panel1", 5, 2, "LEAF", false, true, 624, true, 24, true, 75, true, 25, true, 2, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1.CCD2", "CCD2", "IDC_LOADCCDMAP_CCD2", "TButton", "LoadCCD.Panel1", "LoadCCD.Panel1", 6, 2, "LEAF", false, true, 624, true, 64, true, 75, true, 25, true, 4, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Panel1.btnCCDTest", "btnCCDTest", "IDC_LOADCCDMAP_BTNCCDTEST", "TButton", "LoadCCD.Panel1", "LoadCCD.Panel1", 7, 2, "LEAF", false, true, 624, true, 100, true, 75, true, 25, true, 5, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.ServerSocket1", "ServerSocket1", "", "TServerSocket", "LoadCCD", "LoadCCD", 1, 1, "NONVISUAL", false, true, 440, true, 8, false, 0, false, 0, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "LoadCCD.Timer1", "Timer1", "", "TTimer", "LoadCCD", "LoadCCD", 2, 1, "NONVISUAL", false, true, 632, true, 136, false, 0, false, 0, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
};

const int kLoadCCDMapLayoutCount = 12;

} // namespace ht9045_layout
