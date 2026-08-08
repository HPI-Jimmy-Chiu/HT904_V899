// =============================================================================
//  cInArmPlacement.h  --  public interface of cInArmPlacement (the in-arm
//                         "pickup error IC" auto-recycle/placement helper).
//
//  Faithful translation of golden cInArmPlacement.h (65 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3, group "misc_a".
//
//  ROLE: declares cInArmPlacement -- a small state-machine helper the in-arm
//  engine uses to recycle an IC previously marked HAS_SKIP_IC (Loader) or an
//  error IC parked on the in-shuttle, moving it to a dedicated "Placement"
//  parking nozzle/position via the in-arm's own placement suck
//  (InArmPlaceSuck) and cylinder (C_TrayX_UpDown).  Bodies: cInArmPlacement.cpp
//  -- read its file head for the full gate register.
//
//  VCL/Borland conversions:
//    golden :6  `#include <StdCtrls.hpp>` dropped -- it existed only for the
//      (never-used-by-this-class) VCL control base; nothing in this class
//      derives from a VCL control.
//    golden :7  `#include <map>` dropped -- unused by the class body (no
//      std::map member declared anywhere in golden cInArmPlacement.h).
//    golden :8  `#include "myTimer.h"` kept AS-IS -- this tree's own
//      myTimer.h already carries TQPF_Timer (de-VCL'd, see that header).
//  No __fastcall / __property / __published anywhere in golden's header.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef cInArmPlacementH
#define cInArmPlacementH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"   //AI(W906-PT-W3) 20260807: was <StdCtrls.hpp> (VCL control base, unused by this class); umbrella gives AnsiString/TStringList/... every ported header uses
#include "myTimer.h"
//---------------------------------------------------------------------------
class cInArmPlacement                                                           //JimmyChiu 20220908 add Pickup Error Placement
{
private:    // User declarations
    //for shuttle
    int iShuttleLine;
    int iWhichShuttle;
    int iSuckToRecycleFromShuttleTask;
    int iSuckFromShuttleToPlaceTask;
    int iSuckFromShuttleTask;
    int iPlaceSuckToPlacementTask;
    //for Loader
    int iSuckToRecycleFromLoaderTask;
    int iSuckFromLoaderToPlaceTask;
    int iSuckFromLoaderTask;
    //
    TQPF_Timer timeArmDownDelay;
    enum ShuttleRow
    {
        ShRow1=0,
        ShRow2
    };
public:     // User declarations
    cInArmPlacement();
    ~cInArmPlacement();
    void InitialDatas();
    //parameter
    AnsiString asErrorInArmPlacement;
    ////for shuttle
    void InitialShuttleDatas();
    int  DoCheckShuttleSensor();
    void GetShuttlePos(int iSht, int iRow, int iCol, int &iPosX, int &iPosY);
    void GetPlacementPos(int &iPosX, int &iPosY);
    bool MoveInArmXY(int iPosX, int iPosY);
    void DoInArmPlaceDown(bool bDown);
    void DoInArmPlaceSuck(bool bSuck);
    bool CheckInArmPlaceSuckHasIC();
    bool InArmPlacementEnable();
    //process
    bool DoSuckToRecycleFromShuttle(bool bIsFirst, AnsiString &asError);
    bool DoSuckFromShuttleToPlace(bool bIsFirst, int iSht, int iCol, AnsiString &asError);
    bool DoSuckFromShuttle(bool bIsFirst, int iSht, int iRow, int iCol, AnsiString &asError);
    bool DoPlaceSuckToPlacement(bool bIsFirst, AnsiString &asError);

    ////for Loader
    bool DoCheckLoaderHasErrorIC(int &iRow, int &iCol);
    void GetLoaderPos(int iRow, int iCol, int &iPosX, int &iPosY);

    //process
    bool DoSuckToRecycleFromLoader(bool bIsFirst, AnsiString &asError);
    bool DoSuckFromLoaderToPlace(bool bIsFirst, int iRow, int iCol, AnsiString &asError);
    bool DoSuckFromLoader(bool bIsFirst, int iRow, int iCol, AnsiString &asError);

    bool bRunNo9;                                                               //KenHsieh 20240131 : add No9動畫功能
    bool bNo9Action;                                                            //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
    int  iNo9ShtErrICCt[2];                                                     //KenHsieh 20251105 : Auto In shuttle latch combine No9 func.
    bool bNo9ShtErrNo[2][8];
};
//---------------------------------------------------------------------------
#endif
