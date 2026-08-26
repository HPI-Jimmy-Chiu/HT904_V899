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
