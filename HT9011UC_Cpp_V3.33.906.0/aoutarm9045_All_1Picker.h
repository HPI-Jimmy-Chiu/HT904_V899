//---------------------------------------------------------------------------
//  aoutarm9045_All_1Picker.h  --  out-arm All_1Picker variant header
//
//  Translation wave: W6.2c-OUT (out-arm per-config variant: All_1Picker).
//  Translator: AI(W6.2c-All_1Picker) 20260627
//  Golden source: aoutarm9045_All_1Picker.h (7 lines) -- mirror name + relative path = ROOT
//
//  FAITHFUL mirror of the golden header.  CARVE-OUT (DoOutArm-ONLY): the sole
//  engine-dispatched entry point for this config is DoOutArm_9045_All_1Picker()
//  -- it is the FIRST branch of the DoOutArm_9045() dispatch ladder
//  (`if(USE_PICKER_COUNT==ep1Picker) DoOutArm_9045_All_1Picker();`, picker-gated,
//  NOT an iInArmType enum).  There is NO DoPickFromShuttle_9045_All_1Picker:
//  this file's pick helper is named DoPickFromShuttle_9045_All_1Pick(int) (note
//  `_1Pick`, not `_1Picker`) and is FILE-LOCAL (called only inside
//  DoOutArm_9045_All_1Picker, NOT in the engine DoPickFromShuttle ladder).
//
//  The golden .h exports exactly these two symbols (VERBATIM signatures /
//  default args):
//    void DoOutArm_9045_All_1Picker();
//    bool MoveOutArmToShuttleIncludeZ_9045_All_1Pick(int iSht, bool bZDown=false);
//  All names / values / signatures preserved VERBATIM so the team can
//  reverse-look-up the BCB6 version.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_All_1PickerH
#define aoutarm9045_All_1PickerH
//---------------------------------------------------------------------------
void DoOutArm_9045_All_1Picker();
bool MoveOutArmToShuttleIncludeZ_9045_All_1Pick(int iSht, bool bZDown=false);
#endif
