// =============================================================================
//  OCRInsp.h  --  OCR tray-inspection ENGINE, public surface
//
//  Faithful translation of golden OCRInsp.h (23 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-OCRInsp) 20260807
//  Translation wave: W906-OCRInsp (pure-translation completion campaign).
//  Sibling of the asendic_* tray family: golden asendic_Loader.cpp is the only
//  caller of InitOCRFlow/CleanOCRData, and csystem.cpp is the only caller of
//  DoOCRFlow/DoOCRFlow1.
//
//  ROLE: declares the OCR read cursor (iOCRFlow), the current tray cell cursor
//  (iOCRPosC/iOCRPosR), the InArm-suck sub-cursor (iCheckOCRInArmSuckTask), the
//  two OCR flow pumps (DoOCRFlow = RS232/message OCR, DoOCRFlow1 = TrayArm+CCD
//  OCR), and the OCR-simulation log/bypass hooks.
//
//  CONTENT IS GOLDEN VERBATIM -- every line below is golden OCRInsp.h:1-23 with
//  no reordering, no renaming, no added declaration.  The include guard spelling
//  (OCRINSP_H) is golden's own.
//
//  OWNERSHIP NOTE for 4 of these declarations (bOCRSimBypassAOI /
//  bOCRSimLogEnable / OCRSimLogClear / OCRSimLogWrite): golden DECLARES them
//  here but DEFINES them in golden OCR.cpp (:56, :58, :432, :448) -- the OCR
//  VCL form, which has no translated home (ported forms/fOCR.h is only the
//  ring-light facade).  OCRInsp.cpp therefore owns their definitions for now;
//  see the RETIREMENT DEBT block in OCRInsp.cpp before translating OCR.cpp.
//
//  VCL/Borland conversions: none needed -- golden OCRInsp.h has no VCL type, no
//  __fastcall, no __property, no pragma.
//
//  Big5: golden's Chinese comments decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef OCRINSP_H
#define OCRINSP_H
void InitOCRFlow(bool bTrain);
void OCRNGSetToHASIC();
int DoOCRFlow();                                                                // 0:未完成 1:檢測完成(OK) 2:檢測結果(NG) 3:Training NG
int DoOCRFlow1();                                                               // 0:未完成 1:檢測完成(OK) 2:檢測結果(NG) 3:Training NG
//AI(ht9045-v899) 20260526: return simulation-only alarm stops to the OCR timer without changing production retry codes
const int OCR_SIM_RET_TRIGGER_FAIL=4;
const int OCR_SIM_RET_CAPTURE_TIMEOUT=5;
void CleanOCRData();
extern int iOCRFlow;
//AI(ht9045-v899) 20260505: expose current OCR tray position for OCR area simulation status and restore
extern int iOCRPosC;
extern int iOCRPosR;
extern int iCheckOCRInArmSuckTask;
//AI(ht9045-v899) 20260511: when true, OCR area simulation skips AOI socket dependency (no vision PC needed); SwOCRTigger still fires so handler motion is exercised but result is faked locally
extern bool bOCRSimBypassAOI;
//AI(ht9045-v899) 20260522: expose simulation-only OCR flow log hooks without enabling production logging
extern bool bOCRSimLogEnable;
void OCRSimLogClear(int iLoopNow, int iLoopTotal);
void OCRSimLogWrite(const char* cMessage);
#endif
