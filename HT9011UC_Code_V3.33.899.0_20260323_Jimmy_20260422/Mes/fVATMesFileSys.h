//---------------------------------------------------------------------------

#ifndef fVATMesFileSysH
#define fVATMesFileSysH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "cJSON.h"
#include <map>
//---------------------------------------------------------------------------
class uReadDeviceNumer                                                          //Jimmychiu 20230925 : read 2did in json file
{
private:
    cJSON* SearchObject(cJSON *InNode,AnsiString sFilter);
    std::map<int,AnsiString> map2D;
public:
    uReadDeviceNumer(){}
    ~uReadDeviceNumer(){}
    std::map<int,AnsiString> GetList2D(){return map2D;}
    void List2DClear(){map2D.clear();}
    int List2DGetSize(){return map2D.size();}
    void List2DoAdd(AnsiString info){map2D[map2D.size()]=info;}
    bool Read2DFile(AnsiString sFileNameWithPath);
    void WriteFile(AnsiString asLocationFile,AnsiString asContent);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfMesSystem : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tabsInformation;
    TTabSheet *tabsSet;
    TTabSheet *tabsLog;
    TListBox *listbMesFileLog;
    TLabeledEdit *LabeledEditLotNo;
    TButton *buttonDownloadLotInfor;
    TIdHTTP *IdHTTPMESSystem;
    TLabeledEdit *LabeledHandlerDevice;
    TLabeledEdit *LabeledTestTemp;
    TLabeledEdit *LabeledSoakTime;
    TLabeledEdit *LabeledEditLotSize;
    TLabeledEdit *LabeledEditOPID;
    TGroupBox *groupbMesFileTemp;
    TLabeledEdit *LabeledURL;
    TLabeledEdit *LabeledACode;
    TLabeledEdit *LabeledAction;
    TLabeledEdit *LabeledEditContFail;
    TLabeledEdit *LabeledLeadCount;
    TLabeledEdit *LabeledAutoBin1;
    TLabeledEdit *LabeledAutoBin2;
    TLabeledEdit *LabeledAutoBin3;
    TLabeledEdit *LabeledFixBin1;
    TLabeledEdit *LabeledFixBin2;
    TLabeledEdit *LabeledFixBin3;
    TLabeledEdit *LabeledSiteGap;
    TLabeledEdit *LabeledForcePerPin;
    TLabeledEdit *lbledtHandlerDeviceLowYield;
    TLabeledEdit *lbledtProcess;
    TLabeledEdit *lbledtLabeledFTAutoBin1;
    TLabeledEdit *lbledtLabeledFTAutoBin2;
    TLabeledEdit *lbledtSummaryReportPath;
    TLabeledEdit *lbledtByTimeOEEpath;
    TLabeledEdit *lbledtLabeledFTAutoBin3;
    TLabeledEdit *LabeledFTFixBin1;
    TLabeledEdit *LabeledFTFixBin2;
    TLabeledEdit *LabeledFTFixBin3;
    TLabeledEdit *lbledtUPHReportPath;
    TLabeledEdit *lbledtCustCode;
    TLabeledEdit *lbledtCustPart;
    TLabeledEdit *lbledtInternalLot;
    TGroupBox *grpCreateManualEOCAP;
    TLabeledEdit *lbledtCreateManualEOCAP_URL;
    TButton *btnSendJamCode;
    TListBox *lstCreateManualEOCAP;
    TButton *buttonSave;
    TCheckBox *chkCreateManualEOCAP;
    TLabeledEdit *lbledtCreateManualEOCAP_JamCode;
    TLabeledEdit *lbledtCustLotNum;
    TIdHTTP *IdHTTPqueryEocapStatus;
    TLabeledEdit *lbledtQueryEocapStatusURL;
    TButton *btnQueryEocapStatus;
    TListBox *lstQueryEocapStatus;
    TLabel *labEocapStatus;
    TLabeledEdit *lbledtDevicePassBins;
    TLabeledEdit *lbledtDeviceRetestBins;
    TTabSheet *tabsInformation2;
    TLabeledEdit *lbledtHandlerDeviceATCcheck;
    TLabeledEdit *lbledtHandlerDeviceSiteCheck;
    TLabeledEdit *lbledtHandlerDeviceSOTdelay;
    TLabeledEdit *lbledtHandlerDeviceTestTempVar;
    TLabeledEdit *lbledtHandlerDeviceGPIBaddress;
    TLabeledEdit *lbledtHandlerDeviceOpenBins;
    TLabeledEdit *lbledtHandlerID;
    TLabeledEdit *lbledtHandlerDeviceHotplateXDevision;
    TLabeledEdit *lbledtHandlerDeviceHotplateYDevision;
    TLabeledEdit *lbledtHandlerDeviceHotplateXStart;
    TLabeledEdit *lbledtHandlerDeviceHotplateYStart;
    TLabeledEdit *lbledtHandlerDeviceHotplateXPitch;
    TLabeledEdit *lbledtHandlerDeviceHotplateYPitch;
    TLabeledEdit *lbledtHandlerDeviceTrayXDevision;
    TLabeledEdit *lbledtHandlerDeviceTrayYDevision;
    TLabeledEdit *lbledtHandlerDeviceTrayXStart;
    TLabeledEdit *lbledtHandlerDeviceTrayYStart;
    TLabeledEdit *lbledtHandlerDeviceTrayXPitch;
    TLabeledEdit *lbledtHandlerDeviceTrayYPitch;
    TLabeledEdit *lbledtHandlerDeviceSiteLayouts;
    TLabeledEdit *lbledtHandlerDeviceCleanPadLimit;
    TCheckBox *chkTrayHotplateCheck;
    TGroupBox *grpGetRcsCheckingResult;
    TLabeledEdit *lbledtGetRcsCheckingResultUrl;
    TLabeledEdit *lbledtGetRcsCheckingResultACode;
    TLabeledEdit *lbledtGetRcsCheckingResultAction;
    TLabeledEdit *lbledtGetRcsCheckingResultItemName;
    TListBox *lstGetRcsCheckingResult;
    TButton *btnGetRcsCheckingResult;
    TCheckBox *chkGetRcsCheckingResult;
    TLabeledEdit *lbledtHandlerDeviceInshuttleSensorCheck;
    TLabeledEdit *lbledtTestTimePcs;
    TLabeledEdit *lbledtC1;
    TLabeledEdit *lbledtTestSites;
    TLabeledEdit *lbledtIndexTime;
    TLabeledEdit *lbledtHandlerDeviceNotRepeatableArea;
    TLabeledEdit *lbledtHandlerDeviceICNum;
    void __fastcall buttonSaveClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall buttonDownloadLotInforClick(TObject *Sender);
    void __fastcall btnSendJamCodeClick(TObject *Sender);
    void __fastcall btnQueryEocapStatusClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnGetRcsCheckingResultClick(TObject *Sender);
private:    // User declarations
    bool bShow;
    bool bNewBinCheck;
    int iSiteLayouts[2][4];
    int iGetRcsCount;
    int iOEETotalPass;
    int iPlanOut12h;
    double dTestTime;
    double dIndexTime;
    TStringList *asUnloadBin[eTrayCount];
    TStringList *asFTUnloadBin[eTrayCount];
    TStringList *asPassUnloadBin;
    TStringList *asFailUnloadBin;
    TDateTime dtOEETimeStart;
    TDateTime dtOEETimeEnd;
    TDateTime dtOEETimeDiff;
    TStringList *stOEEState;
    TStringList *stOEEState12hList;

    bool __fastcall CheckEocapStatus(AnsiString asString);                      //jou 20221104 : VTest CreateManualEOCAP function;
    int __fastcall TimeToSec(AnsiString asString);
public:     // User declarations
    __fastcall TfMesSystem(TComponent* Owner);
    __fastcall ~TfMesSystem();

    bool bFirstMaterialsQA;
    bool bDownloadLotInforFlag;
    bool bVATToChangeWorkFile;
    bool bCanShowMESReport;
    bool bCreateManualEOCAP_OK;                                                 //jou 20221104 : VTest CreateManualEOCAP function;
    int iRunSecondTemp;
    int iUPHXCount;
    AnsiString asVATToChangeWorkFileName,asShowJam;

    void __fastcall DoEnableLotInforForMesFile(bool bFlag);
    void __fastcall DoInitialLotInfor();
    bool __fastcall CheckLotInfor();
    void __fastcall VTestSummaryReport();
    void __fastcall VTestUPHReport();                                           //jou 20220525 : lot end 時輸出 UPH report

    bool bFormShowJustInitial;
    DWORD64 dwStartTimeForAlways;
//    AnsiString asLotStartTimeForAlways;
//    long lRunSecondForAlways;
    int iJamRateTotalForAlways;
//    int iBinCTForAlways[4][20];
//    unsigned int iSiteBinCTForAlways[MAX_SOCKET_ROW][MAX_SOCKET_COL][TEST_MAX_BIN];
//    unsigned int iSiteTotalCTForAlways[MAX_SOCKET_ROW][MAX_SOCKET_COL];
//    bool bRecordUseSiteTemp[MAX_SOCKET_ROW][MAX_SOCKET_COL];
    TStringList *stringlRecordReport;
    void __fastcall DoRecordReportByTime(bool bRecrodOEE=false);                //marvin 20200424 (Kirin) Added always record report by time.
    void __fastcall DoInitailRecordReportByTime();
    void __fastcall DoSaveReportByTime();

    int __fastcall RunModeRW(bool bRead,AnsiString asLot,AnsiString asRunMode);
    void __fastcall LoadStringlRecordReport();
    void __fastcall DeleteStringlRecordReport();
    void __fastcall RecordByTimeOEE(int iHour);
    void __fastcall SaveStringlRecordReport();
    void __fastcall SendJamCode(AnsiString asJamCode);                          //jou 20221104 : VTest CreateManualEOCAP function;
    bool __fastcall QueryEocapStatus(bool bManualTest);                         //jou 20221104 : VTest CreateManualEOCAP function;
    bool __fastcall CheckVTENGmode(AnsiString asLotId);
    bool __fastcall GetRcsCheckingResult(bool bShowAlarm);                      //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult
    AnsiString asGetRcsCheckingResult;                                          //jou 20230621 : VTEST Handler即時監控 GetRcsCheckingResult

    uReadDeviceNumer uRDN;                                                      //Jimmychiu 20230925 : read 2did in json file
    std::map<int,AnsiString> map2DIDFromServer;
    bool Get2DIDFromServer(AnsiString asServerIP);
    bool IsMatchServerData(AnsiString as2DID);
    void RecordMsg(AnsiString asMsg);
    AnsiString GetServerWebAPI(AnsiString asURL);

    void SetOEEState(int iState);
    int iOEEState;
    int iOEEStateChangeCnt;                                                     //RogerYang 20250529 加入保護，曾發生狀態莫名其妙一直切換導致LOG暴增
    void AddOEEState(AnsiString asOEECode);
    void WriteOEEState(AnsiString asString);

    bool AutoSiteMapPass(int iBin);
    void ClearMesData();
    void CleaOEEState12hList();

    bool bNoRTBinFlag[3];                                                       //RogerYang 20250604 偉測不可複測bin功能
    bool NeedNoRTBinID(int iFix);
};
//---------------------------------------------------------------------------
extern PACKAGE TfMesSystem *fMesSystem;
//---------------------------------------------------------------------------
#endif
