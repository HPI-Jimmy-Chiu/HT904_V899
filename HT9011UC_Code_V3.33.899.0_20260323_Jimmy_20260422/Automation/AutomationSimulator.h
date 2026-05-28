//---------------------------------------------------------------------------

#ifndef AutomationSimulatorH
#define AutomationSimulatorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMain : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox1;
    TComboBox *cbMachineModel;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edMachineID;
    TLabel *Label3;
    TEdit *enVersion;
    TLabel *Label4;
    TComboBox *cbConnectStatus;
    TRadioGroup *rgACK;
    TLabel *Label5;
    TComboBox *cbHandlerStatus;
    TLabel *lbSystemStatus;
private:    // User declarations
public:     // User declarations
    __fastcall TMain(TComponent* Owner);
    void ShowOLPState(int iState);
};
//---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
//---------------------------------------------------------------------------
#endif
