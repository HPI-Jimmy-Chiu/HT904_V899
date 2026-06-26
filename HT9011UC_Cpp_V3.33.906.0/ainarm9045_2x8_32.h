// =============================================================================
//  ainarm9045_2x8_32.h  --  In-Arm 2x8 / 32-site variant public interface
//
//  Faithful translation of golden  ainarm9045_2x8_32.h  (BCB6, Big5/cp950).
//  Golden: HT9011UC_Code_V3.33.906.0_20260618/ainarm9045_2x8_32.h (18 ln)
//  Translator: AI(W6.2b-INARM) 20260626
//
//  -------------------------------------------------------------------------
//  DEAD-FILE STATUS (verified against the GOLDEN engine, not assumed):
//  -------------------------------------------------------------------------
//  The golden in-arm engine (ainarm9045.cpp) routes iInArmType==e9045_2x8_32
//  to the 2x8_8 family -- NOT to the functions declared here:
//      golden ainarm9045.cpp:4639-4641  DoInArm_9045_2x8_32  -> DoInArm_9045_2x8_8()
//      golden ainarm9045.cpp:4804-4806  ..._SuckerMap         -> DoInArm_9045_2x8_8_SuckerMap()
//      golden ainarm9045.cpp:8829-8831  PlaceToShuttle        -> DoInArmPlaceToShuttle_9045_2x8_8()
//  So NONE of the symbols below are ever called by the engine; the C++ target
//  engine mirrors this (target ainarm9045.cpp:914 e9045_2x8_32 -> DoInArm_9045_2x8_8()).
//
//  This module is ALSO not self-compiling in the golden as written:
//    * it depends pervasively (41 sites) on  Prod.fInArmSuckUse_TwoArm32Site  --
//      a member that is ABSENT from golden cprod.h by design (golden cprod.h has
//      only fInArmSuckUse / fInArmSuckUse_9To16 / fInArmSuck4x8).  Per the W6.2b
//      directive this symbol must NOT be invented (doing so would violate the
//      faithful-translation rule), so the bodies that touch it cannot be ACTIVE.
//    * its own signatures are internally inconsistent: GetNowSiteKitMode_2x8_32
//      is DEFINED 0-arg (golden :194) but CALLED 2-arg (golden :372/:437/:909),
//      and CheckXYPitch_2x8_32 is declared 4-arg here but DEFINED 6-arg
//      (golden :426) -- i.e. the golden TU does not compile as written.
//
//  Therefore this variant is translated FAITHFULLY but the verbatim body is kept
//  behind `#if 0 // TODO(W7: needs Prod.fInArmSuckUse_TwoArm32Site, absent from
//  golden by design)` in the .cpp, with ACTIVE offline stubs for the
//  header-declared entry points so the TU links cleanly when registered.  Active
//  SM case coverage is honestly 0 for this dead variant (see .cpp report).
//
//  vclcompat AnsiString (1-based) replaces VCL AnsiString.  No __fastcall /
//  __property / PACKAGE.  Big5 comments decoded via cp950 -> UTF-8; no U+FFFD.
//
//  GOLDEN HEADER (verbatim, for reverse-lookup):
//    void DoInArm_9045_2x8_32();
//    void DoInArm_9045_2x8_32_SuckerMap();
//    void InArmZNeedDown_2x8_32(int iSht,bool bPlace);
//    void CheckXYPitch_2x8_32(int *iX,int *iY,int iSht,bool bPlace);
//    bool InArmSideAllClose_32(int iSht, int iKit);
//    bool DoPlaceToShuttle_9045_2x8_32();                       //kevin 20180312
//    bool MoveInArmXYPickHotPlate_2x8_32(bool OnlyRow2);        //kevin 20210208
//    bool MoveArmXYToLoaderStage_9045_2x8_32();                 //kevin 20210208
//    bool CleanPad_PlaceToShuttle_2x8_32(int iSht);
//    int  GetShuttleState_4x8_32(int iSht, bool bPick);
//    bool DoArmPickFromLoadStage_9045_2x8_32();
//    bool DoPlaceToShuttle_9045_2x8_32();
// =============================================================================
//---------------------------------------------------------------------------
#ifndef ainarm9045_2x8_32H
#define ainarm9045_2x8_32H
//---------------------------------------------------------------------------

// Header-declared entry points.  Signatures kept as in the golden header (the
// .cpp self-inconsistent 6-arg CheckXYPitch_2x8_32 definition lives inside the
// #if 0 gate; the ACTIVE stub matches THIS 4-arg golden-header declaration).
void DoInArm_9045_2x8_32();
void DoInArm_9045_2x8_32_SuckerMap();
void InArmZNeedDown_2x8_32(int iSht, bool bPlace);
void CheckXYPitch_2x8_32(int *iX, int *iY, int iSht, bool bPlace);
bool InArmSideAllClose_32(int iSht, int iKit);
bool DoPlaceToShuttle_9045_2x8_32();                                            //kevin 20180312 define
bool MoveInArmXYPickHotPlate_2x8_32(bool OnlyRow2);                             //kevin 20210208 add define
bool MoveArmXYToLoaderStage_9045_2x8_32();                                      //kevin 20210208
bool CleanPad_PlaceToShuttle_2x8_32(int iSht);
int  GetShuttleState_4x8_32(int iSht, bool bPick);

bool DoArmPickFromLoadStage_9045_2x8_32();

#endif
