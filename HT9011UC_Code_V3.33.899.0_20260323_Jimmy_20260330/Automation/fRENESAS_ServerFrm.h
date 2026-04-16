//---------------------------------------------------------------------------

#ifndef fRENESAS_ServerFrmH
#define fRENESAS_ServerFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "ufrmProdInfo.h"
//---------------------------------------------------------------------------
class TfServerFrm : public TForm
{
__published:	// IDE-managed Components
    TTimer *ProcTimer;
    TframeProdInfo *frameProdInfo1;
    TGroupBox *grpFTCTStatus;
    TPanel *pnlOnLine;
    TGroupBox *grpTestMode;
    TPanel *pnlTestMode;
    TGroupBox *GroupBox1;
    TLabeledEdit *lbledtHdPass;
    TLabeledEdit *lbledtHdFail;
    TLabeledEdit *lbledtHdRejectATPass;
    TLabeledEdit *lbledtHdRejectATFail;
    TLabeledEdit *lbledtHdRejectBT;
    TLabeledEdit *lbledtTesterMatch;
    TLabeledEdit *lbledtTesterUnmatch;
    TLabeledEdit *lbledtTesterIgnore;
    TLabeledEdit *lbledtHdPickUp;
    TGroupBox *grpHandlerSeq;
    TPanel *pnlHandlerSeq;
    TGroupBox *GroupBox2;
    TPanel *pnlFTCTLastSeqStatus;
    TGroupBox *grpAfter71RecvTimeOut;
    TGroupBox *grpNowCounter;
    TPanel *pnlNowCounter;
    TLabel *lblNowCounterUnit;
    TGroupBox *grpSetCounter;
    TLabel *lblSetCounterUnit;
    TPanel *pnlSetCounter;
    TLabeledEdit *lbledtHdPass1;
    TLabeledEdit *lbledtHdPass2;
    TLabeledEdit *lbledtHdRejectATPass1;
    TLabeledEdit *lbledtHdRejectATPass2;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ProcTimerTimer(TObject *Sender);
    void __fastcall lbledtInputerQtyClick(TObject *Sender);
    void __fastcall frameProdInfo1ShowTimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfServerFrm(TComponent* Owner);
    int iServerListenStatus;
    int iOnLineStatus;
    //Kirin 20200112 hdParamter Show
    //==>
    int iHdPass;
    int iHdFail;
    int iHdRejectATPass;    //Kirin 20200109 紀錄測試完畢被拔除的OK IC數
    int iHdRejectATFail;    //Kirin 20200109 紀錄測試完畢被拔除的NG IC數
    int iHdRejectBT;        //Kirin 20200109 紀錄測試完畢被拔除的未測 IC數
    int iTesterMatch;       //Kirin 20200109 紀錄通訊測試完畢OK IC數
    int iTesterUnmatch;     //Kirin 20200109 紀錄通訊測試完畢NG IC數
    int iTestIgnore;        //Kirin 20200109 紀錄E97 IC數
    int iHdPickUp;          //Kirin 20200112 Loader入料數
    int iHdPass1;           //Kirin 20221115 add two pass bin
    int iHdPass2;           //Kirin 20221115 add two pass bin
    int iHdRejectATPass1;   //Kirin 20221115 add two pass bin
    int iHdRejectATPass2;   //Kirin 20221115 add two pass bin
    //<==
    //Kirin 20200112 hdParamter Show
    int iNowCounter;
    int iSetCounter;    
    AnsiString TestMode;
    AnsiString asFTCTLastSeqStatus; //Kirin 20200207 增加最後送出狀態來看
    AnsiString asHandlerLastSeqStatus; //Kirin 20200207 增加最後送出狀態來看
    void AddRecvLog(AnsiString asRecvData);
    //void __fastcall OnProcTimer(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TfServerFrm *fServerFrm;
//---------------------------------------------------------------------------
#endif
