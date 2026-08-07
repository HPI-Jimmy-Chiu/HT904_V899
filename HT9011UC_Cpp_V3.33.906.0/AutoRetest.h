// =============================================================================
//  AutoRetest.h  --  Auto-Retest (ART) top-level flow: prototypes + task cursors
//
//  Faithful translation of golden AutoRetest.h (30 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-ART) 20260807
//  Translation wave: W906-ART (the ART TOP-LEVEL FLOW -- the driver that sits
//  above the already-translated asendic_Auto_RT / asendic_Loader_RT tray SMs and
//  the acatchtray tray-arm engine; golden AutoRetest.cpp is their only caller of
//  InitAutoTrayToRearTask / InitLoadNewAutoTrayToCarTask / DoLoadNewEmptyTrayToCar_RT)
//
//  ROLE: declares the six ART state machines and their `int` cursors --
//    DoAutoRetest              master ART sequencer (Task 1/2/100/110/130/135/
//                              140/150/160/170/200/205/210/215/220/221/225/250/300)
//    DoTrayArm_AutoRT          tray-arm: pull finished trays out of Auto1..6 and
//                              hand them back to the Loader
//    DoPickFromAuto_AutoRT     tray-arm pick sub-SM (per-Auto timers, shared cursor)
//    DoPlaceToLoader_AutoRT    tray-arm place-to-Loader sub-SM
//    DoAuto_AutoRT             per-Auto tray in/out feed during ART
//    DoLoader_AutoRT           Loader-side tray recycling during ART
//  plus the three GPIB entry points golden main.cpp:15657/:15674/:15689 calls
//  (GPIB_RemoteCommand / GPIB_QueryData / GPIB_SetData).
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim):  all 9 golden functions + every file-scope global.
//   SATISFIED-BY-SHIM (so it LINKS): 7 TU-LOCAL seams inside AutoRetest.cpp only
//     (fMain->SetLotState / ->tGPIBMsg / ->CheckBox1 / ->ChangeTempMode /
//      ->RENESAS_Server->iFTRTCntTmpFor70 / ->hanaART->IsContactAvailable /
//      fFTPClient).  NOTHING is added to any shared header by this unit.
//   GATE (#if 0): NONE.
//
//  ADDED THAT THE GOLDEN HEADER DOES NOT CARRY: nothing.  This header is a
//  line-for-line mirror; the only additions are the two #includes golden reached
//  through the MachineDefine.h god-header (MachineType.h for MAX_AUTO_TRAY and
//  the vclcompat umbrella for AnsiString), the same idiom asendic_Auto_RT.h uses.
//
//  Big5: golden AutoRetest.h carries three Chinese-free trailing author comments
//  (`//jou 2015-09-21 Auto Retest function`); they are preserved verbatim,
//  column position and all.  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef AutoRetestH
#define AutoRetestH
//---------------------------------------------------------------------------

#include "vclcompat/vcl_compat.h"   // AnsiString (golden reached it via <vcl.h>/MachineDefine.h)
#include "MachineType.h"            // MAX_AUTO_TRAY (golden reached it via MachineDefine.h)

extern int iAutoRetestTask;
extern int iTrayArm_AutoRT_Task;
extern int iTrayArm_PickFromAuto_AutoRT_Task;
extern int iTrayArm_PlaceToLoad_AutoRT_Task;
extern int iAuto_AutoRT_Task[MAX_AUTO_TRAY];
extern int iLoader_AutoRT_Task;

extern bool bTrayArmOK_AutoRT;
extern bool bAutoOK_AutoRT[MAX_AUTO_TRAY];
extern bool bLoadOK_AutoRT;

extern bool DoAutoRetest(bool bReset=false);

extern bool DoTrayArm_AutoRT(bool bReset=false);
extern bool DoPickFromAuto_AutoRT(int Pos, bool bReset=false);
extern bool DoPlaceToLoader_AutoRT(bool bReset=false);

extern bool DoAuto_AutoRT(int Pos, bool bReset=false);
extern bool DoLoader_AutoRT(bool bReset=false);

extern int GPIB_RemoteCommand(AnsiString asCommand);    //jou 2015-09-21 Auto Retest function
extern AnsiString GPIB_QueryData(AnsiString asCommand); //jou 2015-09-21 Auto Retest function
extern int GPIB_SetData(AnsiString asCommand);          //jou 2015-09-21 Auto Retest function
#endif
