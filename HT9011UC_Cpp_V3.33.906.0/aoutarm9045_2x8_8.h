//---------------------------------------------------------------------------
//  aoutarm9045_2x8_8.h  --  out-arm 2x8_8 per-site-config variant header
//
//  Translation wave: W6.2c-OUT (out-arm per-site-config variant: 2x8_8, two-row
//  8-picker 32-site).  Translator: AI(W6.2c-OUT-2x8_8) 20260627
//  Golden source: aoutarm9045_2x8_8.h (8 lines) -- mirror name + relative path = ROOT.
//
//  FAITHFUL mirror of the golden header.  The golden header exports:
//    void DoOutArm_9045_2x8_8();                          (engine-dispatched entry)
//    bool MoveOutArmToShuttleIncludeZ_9045_2x8_8(iSht, iKit, bZDown=false);
//    extern int XPHSuckToSht_2x8_8_OutArm[e2x8ModeTotal][2][8];  (defined in .cpp)
//  In addition the second engine-dispatched entry point DoPickFromShuttle_9045_2x8_8
//  is declared here (the engine aoutarm9045.cpp also declares it as an extern for
//  its DoPickFromShuttle dispatch ladder, which routes BOTH iInArmType==e9045_2x8_8
//  AND iInArmType==e9045_2x8_32 here).
//
//  e2x8ModeTotal is the GLOBAL enum from MachineType.h (authoritative); this header
//  only MIRRORS the golden extern array decl and does NOT redefine the enum, so the
//  including TU must have MachineType.h visible (the .cpp pulls it before this .h).
//  All names / values / signatures preserved VERBATIM.
//---------------------------------------------------------------------------
#ifndef aoutarm9045_2x8_8H
#define aoutarm9045_2x8_8H
//---------------------------------------------------------------------------
void DoOutArm_9045_2x8_8();
bool DoPickFromShuttle_9045_2x8_8(int iSht);
bool MoveOutArmToShuttleIncludeZ_9045_2x8_8(int iSht, int iKit, bool bZDown=false);
extern int XPHSuckToSht_2x8_8_OutArm[e2x8ModeTotal][2][8];                      //JerryYang 20250711 : fix for 2x8特殊關SITE
#endif
