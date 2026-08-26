//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ufrmProdInfo.h"
//#include "TDataRecord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TframeProdInfo *frameProdInfo;
//---------------------------------------------------------------------------
__fastcall TframeProdInfo::TframeProdInfo(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TframeProdInfo::ShowTimerTimer(TObject *Sender)
{
    {
       // lbledtLotNo->Text=NewLastSet->asRENESASLotNo;
       // lbledtLotID->Text=NewLastSet->asRENESASLotID;
       // lbledtHdParameter->Text=NewLastSet->asRENESASHdParameter;
       // lbledtProduct->Text=NewLastSet->asRENESASProduct;
       // lbledtStepCode->Text=NewLastSet->asRENESASStepCode;
        //lbledtQty->Text=NewLastSet->asRENESASQty;
       // lbledtDate->Text=NewLastSet->asRENESASLotStartTime;
    }
}
//---------------------------------------------------------------------------
