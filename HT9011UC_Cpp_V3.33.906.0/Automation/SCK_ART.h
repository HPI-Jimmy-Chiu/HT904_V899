// Automation/SCK_ART.h
// Standard C++ translation of BCB6 Automation/SCK_ART.h/.cpp (V3.33.906.0_20260618) -- 8-FUNCTION
// EXTRACT, NOT the whole file.
//
// Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.cpp (4392 lines) and
//                HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.h  (370 lines).
//
// AI(W5-SckArt-Translate) 20260710: new file. Faithful translation of exactly 8 TfSCKART member
// functions (the SCK/ATK "Auto Retest" ART lot-tracking logic), extracted as FREE FUNCTIONS over an
// explicit state struct instead of TfSCKART class methods -- see "WHY FREE FUNCTIONS, NOT A CLASS"
// below. Automation/SCK_ART.cpp (4392 lines total, a VCL form TfSCKART mixing this ART state-machine
// logic with ~150 __published widget members) is otherwise completely OUT OF SCOPE this wave.
//
// Functions translated (golden TfSCKART method -> this file's free function):
//   TfSCKART::SetLotStatus(int)                      -> SckArt_SetLotStatus            (golden :639-667)
//   TfSCKART::GetLotStatus()                         -> SckArt_GetLotStatus            (golden :669-672)
//   TfSCKART::CheckLoadingCount()                    -> SckArt_CheckLoadingCount       (golden :930-993)
//   TfSCKART::CheckInArmNeedVariModeFIX()            -> SckArt_CheckInArmNeedVariModeFIX (golden :995-1029)
//   TfSCKART::CheckOutArmNeedVariModeFIX(int)        -> SckArt_CheckOutArmNeedVariModeFIX (golden :1031-1070)
//   TfSCKART::CheckNeedRT()                          -> SckArt_CheckNeedRT             (golden :1072-1230)
//   TfSCKART::DoAutoSocketOff(bool)                  -> SckArt_DoAutoSocketOff         (golden :1232-1359)
//   TfSCKART::DoChkInputCntAlarm(bool)               -> SckArt_DoChkInputCntAlarm      (golden :4359-4391)
//
// DISCREPANCY vs the hand-off prompt's line estimates (golden wins; recon was approximate, verified
// against golden directly): CheckNeedRT actually spans golden :1072-1230 (not a rough single-line
// guess); the SCK_ART.h field-subset the prompt estimated at "~259-264" turned out, on direct golden
// read, to include 2 fields these 8 functions do NOT touch (iWaitGPIBLotR :259, iCurrentFlexARTStep
// :264 -- both are only referenced by OTHER TfSCKART call sites / the csystem.cpp W7C1 seam, never
// inside these 8 function bodies) and to MISS one field they DO need (iCurrentStatus, declared far
// earlier at golden SCK_ART.h:251). This file's SckArtState below reflects the VERIFIED field set,
// not the recon estimate.
//
// ---------------------------------------------------------------------------------------------
// WHY FREE FUNCTIONS OVER SckArtState, NOT TfSCKART CLASS METHODS (read before wiring this in)
// ---------------------------------------------------------------------------------------------
// A class named `TfSCKART` with a global `fSCKART` pointer ALREADY EXISTS in this tree --
// FormsFacade.h/.cpp (a W6.2/W6.3/W6.5 non-VCL offline stub with iInputJamCnt/iFTRTCount/iInputCount
// + stub CheckLoadingCount()/AddOutputJamCnt()), and it is a HARD-BOUNDARY file for this wave (listed
// among the files this unit may not touch). Reusing the golden class name here would be an ODR
// collision (duplicate `class TfSCKART` / duplicate `extern TfSCKART *fSCKART` across translation
// units already linking FormsFacade.cpp). So: this file defines a plainly-named, non-conflicting
// state struct (SckArtState) holding exactly the golden TfSCKART data members these 8 functions touch,
// and 8 free functions taking `SckArtState&` in place of the implicit `this`. See the translate
// report's FormsFacade section for the exact integration recipe (fold these fields/methods onto the
// real FormsFacade::TfSCKART once that's safe to do serially).
//
// ---------------------------------------------------------------------------------------------
// SCOPE OF THE ONE VCL WIDGET TOUCH (extract-calc-core, per hand-off instruction)
// ---------------------------------------------------------------------------------------------
// SetLotStatus's only VCL touch is `cbLotStatus->ItemIndex=iStatus;` (a TComboBox selection WRITE,
// golden :664). Turned into an explicit output parameter (outComboItemIndex) instead of touching a
// widget -- see SckArt_SetLotStatus below.
//
// ---------------------------------------------------------------------------------------------
// GATED DEPENDENCIES (verified ABSENT from the already-translated substrate; each gated via a local
// TU-level macro in SCK_ART.cpp, mirroring the project's established "#if 0 // TODO(W7)" /
// W64B_-style local-macro convention -- see aTester_Front.cpp's file-header gate list for the
// precedent this follows). Every gate is re-cited at its call site in the .cpp:
//   1. TfMainHanaART::NeedToRT() -- golden main.h (HANA ART helper); FormsFacade.h's TfMainHanaART
//      stub has IsHanaArtAvailable()/AddNewTrayHead() but NOT NeedToRT(). SAME gap already flagged +
//      solved (offline default 0) in csystem.cpp's W7C2_TfHanaSeam::NeedToRT() -- this file reuses the
//      identical default (0 = "waiting command") for consistency, via a local macro (not a class, to
//      respect the FormsFacade.h hard boundary).
//   2. TfSCKART::AccessFile(bool,int=-1) -- golden SCK_ART.h:271 (SCK_ART.cpp body not in this wave's
//      8-function scope). File-persistence side effect only; does not affect CheckNeedRT's control
//      flow / return value, so parked no-op.
//   3. LastSet.iSCKART_RTUnitCount (golden LastSet.h:410), .iHdRejectATFail/ATPass/BT (golden
//      LastSet.h:23/22/24), .lSCKARTBinCT[256] (golden LastSet.h:507), .bUseTestSocketEE[2][4][8]
//      (golden LastSet.h:392) -- none of these five fields are in canary_support.h's minimal
//      LAST_GENERAL_SET shim yet. TU-local static stand-ins (zero-initialized, matching the shim's
//      existing all-fields-default-0/false posture) -- see the translate report for the exact fields
//      to fold into the real shim. CROSS-UNIT NOTE: csystem.cpp's own W7C2_TfSCKARTSeam ALREADY
//      carries a DIFFERENT copy of iSCKART_RTUnitCount and lSCKARTBinCT[256] (same golden fields) as
//      its own TU-local statics -- when both this file's fields and csystem.cpp's seam are retired in
//      favour of the real LastSet, make sure only ONE copy survives.
//   4. ArmData[0]/[1] (golden cSocket.h:180, extern TArm*[3]) with per-site TMySocket::GetPCA()
//      (golden cSocket.h:41/91) -- the whole per-arm/per-socket pass-count-average tracker is absent
//      from the translated tree (cSocket.h/.cpp not translated at all yet; related to the atester.cpp
//      GetTesterResult/ProcessTestResult DEFERRED entry, same bin/yield-tracking family). TU-local
//      stand-in returns 0.0 (conservative: yields the same as "no data yet"); given
//      TestIF_File.dSCKART_AutoSocketOffYield is a non-negative configured threshold, dTargetYield
//      (=dMaxYield-threshold) stays <=0 with this stand-in, so the socket-disable body is naturally
//      inert offline (matches this project's "gate leaves the branch reachable-but-harmless" norm) --
//      see the .cpp for the exact reasoning at the call site.
// ---------------------------------------------------------------------------------------------

#ifndef AUTOMATION_SCK_ART_CORE_H
#define AUTOMATION_SCK_ART_CORE_H

#include "vclcompat/vcl_compat.h"   // AnsiString

// ---------------------------------------------------------------------------
// SckArtState -- the minimal own-field subset of golden TfSCKART (Automation/SCK_ART.h) that the 8
// functions below read/write. Field names/types verbatim from golden; NOT the whole TfSCKART (~150
// widget members + ~40 other data members are out of scope -- see file-head note above).
// ---------------------------------------------------------------------------
struct SckArtState
{
    AnsiString sLOTSTATUS;        // golden SCK_ART.h:236
    int    iCurrentStatus;        // golden SCK_ART.h:251
    int    iTesterType;           // golden SCK_ART.h:249  (0:Flex, 1:93K)
    int    iInputCount;           // golden SCK_ART.h:245  (ART input/lot qty)
    int    iFTRTCount;            // golden SCK_ART.h:248  (FT/RT attempt count)
    int    iManualRejectCnt;      // golden SCK_ART.h:250
    int    iNeedRT;               // golden SCK_ART.h:285  (0:no RT, 1:need RT, 2:final RT)
    double dCurrYield;            // golden SCK_ART.h:261
    int    iCurrent93KARTStep;    // golden SCK_ART.h:263
    bool   bLdCntExdInputCnt;     // golden SCK_ART.h:350
    bool   bBackUpInArmMode;      // golden SCK_ART.h:279
    bool   bBackUpOutArmMode;     // golden SCK_ART.h:280

    // Golden ctor (SCK_ART.cpp:36-56) zero/NONE-initializes every field this struct mirrors.
    SckArtState();
};

// ---------------------------------------------------------------------------
// SckArt_SetLotStatus -- golden TfSCKART::SetLotStatus(int) (SCK_ART.cpp:639-667).
//   Extract-calc-core: golden `cbLotStatus->ItemIndex=iStatus;` (VCL TComboBox write, golden :664)
//   becomes the optional out-param outComboItemIndex (if non-null, *outComboItemIndex=iStatus,
//   verbatim value) instead of touching a widget. sLOTSTATUS / iCurrentStatus updated in st.
// ---------------------------------------------------------------------------
void SckArt_SetLotStatus(SckArtState &st, int iStatus, int *outComboItemIndex = 0);

// SckArt_GetLotStatus -- golden TfSCKART::GetLotStatus() (SCK_ART.cpp:669-672). Trivial accessor.
AnsiString SckArt_GetLotStatus(const SckArtState &st);

// ---------------------------------------------------------------------------
// SckArt_CheckLoadingCount -- golden TfSCKART::CheckLoadingCount() (SCK_ART.cpp:930-993).
//   Returns 0:not enough, 1:same, 2:over count, -1: neither the ART-input-count nor the P57-loader-
//   count mode applies (golden's own explicit trailing `return -1;`, SCK_ART.cpp:992 -- verbatim,
//   not an invented sentinel).
// ---------------------------------------------------------------------------
int SckArt_CheckLoadingCount(SckArtState &st);

// SckArt_CheckInArmNeedVariModeFIX -- golden TfSCKART::CheckInArmNeedVariModeFIX() (SCK_ART.cpp:995-1029).
void SckArt_CheckInArmNeedVariModeFIX(SckArtState &st);

// SckArt_CheckOutArmNeedVariModeFIX -- golden TfSCKART::CheckOutArmNeedVariModeFIX(int) (SCK_ART.cpp:1031-1070).
//   iAuto: golden Prod.iIsFailT6[iAuto] index (which Auto-tray lane).
void SckArt_CheckOutArmNeedVariModeFIX(SckArtState &st, int iAuto);

// SckArt_CheckNeedRT -- golden TfSCKART::CheckNeedRT() (SCK_ART.cpp:1072-1230). See file-head gated-
// dependency notes #1-#3 above for the HANA-ART / AccessFile / LastSet stand-ins this body uses.
void SckArt_CheckNeedRT(SckArtState &st);

// SckArt_DoAutoSocketOff -- golden TfSCKART::DoAutoSocketOff(bool bAllSiteOn=false) (SCK_ART.cpp:1232-1359).
// See file-head gated-dependency notes #3-#4 above for the LastSet.bUseTestSocketEE / ArmData-GetPCA
// stand-ins this body uses.
void SckArt_DoAutoSocketOff(SckArtState &st, bool bAllSiteOn = false);

// SckArt_DoChkInputCntAlarm -- golden TfSCKART::DoChkInputCntAlarm(bool) (SCK_ART.cpp:4359-4391).
bool SckArt_DoChkInputCntAlarm(const SckArtState &st, bool bExcess);

#endif // AUTOMATION_SCK_ART_CORE_H
