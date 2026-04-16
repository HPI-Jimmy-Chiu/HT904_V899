//---------------------------------------------------------------------------


#ifndef ufrmProdInfoH
#define ufrmProdInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "uRENESAS_Other.h"
//---------------------------------------------------------------------------
class TframeProdInfo : public TFrame
{
__published:	// IDE-managed Components
    TTimer *ShowTimer;
    TLabeledEdit *lbledtLotNo;
    TLabeledEdit *lbledtLotID;
    TLabeledEdit *lbledtHdParameter;
    TLabeledEdit *lbledtProduct;
    TLabeledEdit *lbledtStepCode;
    TLabeledEdit *lbledtQty;
    TLabeledEdit *lbledtDate;
    void __fastcall ShowTimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    TRENESAS_RECV_CMD_DATA ShowData;
    __fastcall TframeProdInfo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TframeProdInfo *frameProdInfo;
//---------------------------------------------------------------------------
#endif
