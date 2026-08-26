//---------------------------------------------------------------------------

#ifndef LiftCountH
#define LiftCountH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfLiftCount : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *grpSocketMap;
    TPanel *pl_Socket_1;
    TPanel *pl_Socket_2;
    TPanel *pl_Socket_3;
    TPanel *pl_Socket_4;
    TGroupBox *grpArm1TIMMap;
    TPanel *pl_Arm1_TIM1;
    TPanel *pl_Arm1_TIM2;
    TPanel *pl_Arm1_TIM3;
    TPanel *pl_Arm1_TIM4;
    TGroupBox *grpArm2TIMMap;
    TPanel *pl_Arm2_TIM1;
    TPanel *pl_Arm2_TIM2;
    TPanel *pl_Arm2_TIM3;
    TPanel *pl_Arm2_TIM4;
    TButton *btSave;
    TButton *btRecover;
    TLabel *Lab_Socket1;
    TLabel *Lab_Socket2;
    TLabel *Lab_Socket3;
    TLabel *Lab_Socket4;
    TLabel *Lab_Arm1_1;
    TLabel *Lab_Arm1_2;
    TLabel *Lab_Arm1_3;
    TLabel *Lab_Arm1_4;
    TLabel *Lab_Arm2_1;
    TLabel *Lab_Arm2_2;
    TLabel *Lab_Arm2_3;
    TLabel *Lab_Arm2_4;
    TButton *btEXIT;
    TTimer *LiftCountTimer;
    void __fastcall FormShow(TObject *Sender);        
    void __fastcall pl_Socket_1Click(TObject *Sender);
    void __fastcall btRecoverClick(TObject *Sender);
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall btEXITClick(TObject *Sender);
    void __fastcall LiftCountTimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfLiftCount(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfLiftCount *fLiftCount;
//---------------------------------------------------------------------------
#endif
