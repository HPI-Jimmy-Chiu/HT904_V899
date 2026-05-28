//---------------------------------------------------------------------------

#ifndef PrecautionH
#define PrecautionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfPrecaution : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnPrecautionsRecord;
    TPanel *pnMaintenanceRecord;
    TPanel *pnMaintenanceRecordTitle;
    TMemo *MemoHandlerPrecautionRecord;
    TPanel *pnPrecautionRecordDocumentNoTitle;
    TPanel *Panel91;
    TPanel *Panel92;
    TPanel *Panel93;
    TPanel *Panel94;
    TPanel *Panel95;
    TPanel *pnPrecautionStartTime;
    TPanel *Panel96;
    TPanel *pnPrecautionEndTime;
    TPanel *pnPrecautionRecordDocumentNo;
    TPanel *pnNoteContents;
    TPanel *pnApprovedManager;
    TPanel *pnWatchmakers;
    TPanel *Panel1;
    TPanel *pnCloseType;
    TPanel *Panel3;
    TPanel *pnCloseName;
    TPanel *pnPRSpecification;
    TTimer *tm_CheckEditEmpty;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall tm_CheckEditEmptyTimer(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfPrecaution(TComponent* Owner);
    void __fastcall DoIniDataToForm();//Sam 20180423 (wei) : 變更 Precaution 方式
    bool fShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfPrecaution *fPrecaution;
//---------------------------------------------------------------------------
#endif
