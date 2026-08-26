//---------------------------------------------------------------------------
#ifndef loginH
#define loginH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfLogin : public TForm
{
__published:    // IDE-managed Components
    TLabel *labUserName;
    TEdit *edUserName;
    TLabel *labOldPassword;
    TEdit *edLoginOldPassword;
    TSpeedButton *sbOk;
    TRadioGroup *rgLoginOption;
    TLabel *labNewPassword;
    TEdit *edLoginNewPassword;
    TComboBox *cbLoginUserName; //0:new , 1:delete , 2:modify //ChungHung 20110920 add
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sbOkClick(TObject *Sender);
    void __fastcall rgLoginOptionClick(TObject *Sender);
private:    // User declarations
public:     // User declarations

    __fastcall TfLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfLogin *fLogin;
//---------------------------------------------------------------------------
#endif
