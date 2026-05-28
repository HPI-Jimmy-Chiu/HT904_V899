#include "MachineDefine.h"
#pragma hdrstop

#include "Precaution.h"

#include "cprod.h"
#include "cObserver.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfPrecaution *fPrecaution;
//---------------------------------------------------------------------------
__fastcall TfPrecaution::TfPrecaution(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfPrecaution::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    tm_CheckEditEmpty->Enabled=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
}
//---------------------------------------------------------------------------
void __fastcall TfPrecaution::FormCreate(TObject *Sender)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfPrecaution::FormShow(TObject *Sender)
{
    Width   = 585;
    Height  = 510;
    Left    =(1280-Width)/2;
    Top     =(1024-Height)/2;

    DoIniDataToForm();                                                          //Sam 20180423 (wei) : 變更 Precaution 方式
    MemoHandlerPrecautionRecord->Text=fObserver->MemoHandlerPrecautionRecord->Text;
    fShow=true;
    tm_CheckEditEmpty->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfPrecaution::tm_CheckEditEmptyTimer(TObject *Sender)
{
    bool bEmpty[10]={true, true, true, true, true, true, true, true, true, true};
    if(fShow==false)
        return;

    bEmpty[0] = pnPrecautionRecordDocumentNo->Caption.IsEmpty();
    bEmpty[1] = pnNoteContents->Caption.IsEmpty();
    bEmpty[2] = pnApprovedManager->Caption.IsEmpty();
    bEmpty[3] = pnPrecautionStartTime->Caption.IsEmpty();
    bEmpty[4] = pnCloseType->Caption.IsEmpty();
    bEmpty[5] = pnWatchmakers->Caption.IsEmpty();
    //bEmpty[6] = edPromptDay->Text.IsEmpty();

    bEmpty[7] = pnCloseName->Caption.IsEmpty();
    bEmpty[8] = pnPrecautionEndTime->Caption.IsEmpty();

    if(MemoHandlerPrecautionRecord->Lines->Count   == 0     ||
       bEmpty[0]                                   == true ||
       bEmpty[1]                                   == true ||
       bEmpty[2]                                   == true ||
       bEmpty[3]                                   == true ||
       bEmpty[4]                                   == true ||
       bEmpty[5]                                   == true //||
        //bEmpty[6]                                   == true
      )
    {
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfPrecaution::DoIniDataToForm()                                 //Sam 20180423 (wei) : 變更 Precaution 方式
{
    if(SystemInitialOK==false)
        return;

    pnPrecautionRecordDocumentNo->Caption   = fObserver->edPrecautionRecordDocumentNo->Text;
    pnNoteContents->Caption                 = fObserver->edNoteContents->Text;
    pnApprovedManager->Caption              = fObserver->edApprovedManager->Text;
    pnWatchmakers->Caption                  = fObserver->edWatchmakers->Text;
    pnPrecautionStartTime->Caption          = fObserver->pnPrecautionStartTime->Caption;
    pnPrecautionEndTime->Caption            = fObserver->pnPrecautionEndTime->Caption;
    pnCloseType->Caption                    = fObserver->cobPRFinishType->Text;
    pnCloseName->Caption                    = fObserver->edFinishName->Text;
    pnPRSpecification->Caption              = fObserver->pnPRSpecificationNO->Caption;
    MemoHandlerPrecautionRecord->Lines->Clear();
}
//---------------------------------------------------------------------------
