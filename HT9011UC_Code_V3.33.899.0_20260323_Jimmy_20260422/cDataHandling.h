//---------------------------------------------------------------------------

#ifndef cDataHandlingH
#define cDataHandlingH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfDataHandling : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox1;
    TComboBox *ComboBox1;
    TGroupBox *GroupBox2;
    TCheckBox *CheckBox1;
    TCheckBox *CheckBox2;
    TCheckBox *CheckBox3;
    TGroupBox *GroupBox3;
    TCheckListBox *CheckListBox1;
    TSpeedButton *sbtExit;
    TSpeedButton *spbExe;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ComboBox1Change(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall spbExeClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfDataHandling(TComponent* Owner);
    void __fastcall InitCompPara();
};
//---------------------------------------------------------------------------
extern PACKAGE TfDataHandling *fDataHandling;
//---------------------------------------------------------------------------
#endif
