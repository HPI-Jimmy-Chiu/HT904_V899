//---------------------------------------------------------------------------

#ifndef RPDefaultH
#define RPDefaultH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
typedef struct
{
    TLabel  *PLabel;
    TPanel  *PPanel;

} RECEIPE_DEFAULT ;

enum eRPVisible
    {   eRPVisibleIndex     =0,
        eRPVisibleInput     =1,
        eRPVisibleOutput    =2,
        eRPVisibleShuttle   =3,
        eRPVisibleTrayArm   =4,
        eRPVisibleAutoClean =5,
        eRPVisibleYield     =6,
        eRPVisibleTester    =7,
        eRPVisibleCnt       =8
    };

class TfRPDefault : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *gbTestingStopTime;
    TPanel *pnl1;
    TLabel *label1;
    TLabel *label2;
    TSpeedButton *spbReplyRPDefault;
    TSpeedButton *spbSetRPDefault;
    TPanel *pnl2;
    TPanel *pnlRPDefaultFalseColor;
    TPanel *pnlRPDefaultTrueColor;
    TStringGrid *strngrdReciepeParameterDefault;
    TSpeedButton *spbDefaulteViewShow;
    TSpeedButton *spbSave;
    TSpeedButton *spbExit;
    void __fastcall spbDefaulteViewShowClick(TObject *Sender);
    void __fastcall spbExitClick(TObject *Sender);
    void __fastcall spbSetRPDefaultClick(TObject *Sender);
    void __fastcall spbReplyRPDefaultClick(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall InitialComponentHead();
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfRPDefault(TComponent* Owner);
    void __fastcall SetReciepeParameterDefault(TWinControl *PCtrl);
    void __fastcall CompareRPDefaultAndValue(TStringList *aName,TStringList *aDefault,TStringList *aValue);
    bool bReceipeDefaultDiffShow;
    TStringList *RP_Name;
    TStringList *RP_Default;
    TStringList *RP_Value;
    void __fastcall ShowMonitoredParameter();       //Isaac 20210222 : 全智要求把參數拉出來看，方便監控
};
//---------------------------------------------------------------------------
extern PACKAGE TfRPDefault *fRPDefault;
//---------------------------------------------------------------------------
#endif
