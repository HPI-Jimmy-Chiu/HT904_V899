//---------------------------------------------------------------------------
#ifndef OCRINSP_H
#define OCRINSP_H
void InitOCRFlow(bool bTrain);
void OCRNGSetToHASIC();
int DoOCRFlow();                                                                // 0:未完成 1:檢測完成(OK) 2:檢測結果(NG) 3:Training NG
int DoOCRFlow1();                                                               // 0:未完成 1:檢測完成(OK) 2:檢測結果(NG) 3:Training NG
void CleanOCRData();
extern int iOCRFlow;
extern int iCheckOCRInArmSuckTask;
#endif
