// ===========================================================================
//  Motor/TrayStepMotor_layout.gen.cpp
//  DO NOT EDIT -- GENERATED FILE. See the matching _layout.gen.h banner.
// ===========================================================================
#include "TrayStepMotor_layout.gen.h"

namespace ht9045_layout {

// (no custom-control properties in this form)

const DfmControlLayout kTrayStepMotorLayout[] = {
    { "dmTrayMotor", "dmTrayMotor", "", "TdmTrayMotor", "", "", 0, 0, "ROOT", false, true, 752, true, 341, true, 215, true, 165, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "dmTrayMotor.tmrTrayStepMotor", "tmrTrayStepMotor", "", "TTimer", "dmTrayMotor", "dmTrayMotor", 0, 1, "NONVISUAL", false, true, 116, true, 8, false, 0, false, 0, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
    { "dmTrayMotor.comTrayStepMotor", "comTrayStepMotor", "", "TComm", "dmTrayMotor", "dmTrayMotor", 1, 1, "NONVISUAL", false, true, 26, true, 6, false, 0, false, 0, false, 0, -1, "", "", false, "", 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", false, 0, "", 0, nullptr },
};

const int kTrayStepMotorLayoutCount = 3;

} // namespace ht9045_layout
