//---------------------------------------------------------------------------

#ifndef QAModeH
#define QAModeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfQAMode : public TForm
{
__published:    // IDE-managed Components
    TButton *btnApply;
    TButton *btnOk;
    TPageControl *pgcQAMode;
    TTabSheet *tsQAMode;
    TTabSheet *tsQASampling;
    TGroupBox *gbQAMode;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *labQAModeBin;
    TEdit *edQAMode;
    TRadioGroup *rgQARunMode;
    TComboBox *cbQAModeBin;
    TGroupBox *gbLoaderDirection;
    TImage *Image1;
    TCheckBox *cbTrayEndDoQAModeAgain;
    TPanel *pnlQASample;
    TCheckBox *cbTempOffset;
    TComboBox *cbbQASampleBin;
    TComboBox *cbbQASampleTray;
    TLabel *lblQASampleBin;
    TLabel *lblQASampleTray;
    TLabel *Label2;
    TEdit *edtQASampleCnt;
    TCheckBox *cbTrayEndCloseYield100Site;
    TCheckBox *cbQAD22Double;
    TComboBox *cbbQAD22Double;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall edQAModeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnApplyClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Image1Click(TObject *Sender);
    void __fastcall edtQASampleCntMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    void __fastcall ShowTrayDirectIMG();
public:     // User declarations
    __fastcall TfQAMode(TComponent* Owner);
    void DoIniDataToForm();
    void DoFormToData();
    void ReadFile();
    bool fShow;
};
//---------------------------------------------------------------------------
extern PACKAGE TfQAMode *fQAMode;
//---------------------------------------------------------------------------
extern void QABackupStatus(bool bBackup);   //Sam 20231117 : 整合到 QA 模式
#endif
