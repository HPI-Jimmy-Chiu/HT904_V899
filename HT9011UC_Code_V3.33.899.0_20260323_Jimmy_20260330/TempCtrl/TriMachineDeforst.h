//---------------------------------------------------------------------------

#ifndef TriMachineDeforstH
#define TriMachineDeforstH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "butPa1.h"
//---------------------------------------------------------------------------
class TfrmDefrost : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox1;
    TPageControl *pcTriTempControl;
    TTabSheet *tsTriTemp_12ChShow;
    TPanel *pnHotPlate1;
    TLabel *lbHotPlate1;
    TPanel *pnHotPlate1TempShow;
    TPanel *pnHotPlate2;
    TLabel *lbHotPlate2;
    TPanel *pnHotPlate2TempShow;
    TPanel *pnShuttle1;
    TLabel *lbShuttle1;
    TPanel *pnShuttle1TempShow;
    TPanel *pnShuttle2;
    TLabel *lbShuttle2;
    TPanel *pnShuttle2TempShow;
    TPanel *pnHead1;
    TLabel *lbHead1;
    TPanel *pnHead1TempShow;
    TPanel *pnHead2;
    TLabel *lbHead2;
    TPanel *pnHead2TempShow;
    TPanel *pnHead5;
    TLabel *lbHead5;
    TPanel *pnHead5TempShow;
    TPanel *pnHead6;
    TLabel *lbHead6;
    TPanel *pnHead6TempShow;
    TPanel *pnHead3;
    TLabel *lbHead3;
    TPanel *pnHead3TempShow;
    TPanel *pnHead4;
    TLabel *lbHead4;
    TPanel *pnHead4TempShow;
    TPanel *pnHead7;
    TLabel *lbHead7;
    TPanel *pnHead7TempShow;
    TPanel *pnHead8;
    TLabel *lbHead8;
    TPanel *pnHead8TempShow;
    TTabSheet *tsTriTemp_20ChShow;
    TPanel *pnHead1_1;
    TLabel *lbHead1_1;
    TPanel *pnHead1_1TempShow;
    TPanel *pnHead1_3;
    TLabel *lbHead1_3;
    TPanel *pnHead1_3TempShow;
    TPanel *pnHead1_5;
    TLabel *lbHead1_5;
    TPanel *pnHead1_5TempShow;
    TPanel *pnHead1_7;
    TLabel *lbHead1_7;
    TPanel *pnHead1_7TempShow;
    TPanel *pnHead1_8;
    TLabel *lbHead1_8;
    TPanel *pnHead1_8TempShow;
    TPanel *pnHead1_6;
    TLabel *lbHead1_6;
    TPanel *pnHead1_6TempShow;
    TPanel *pnHead1_4;
    TLabel *lbHead1_4;
    TPanel *pnHead1_4TempShow;
    TPanel *pnHead1_2;
    TLabel *lbHead1_2;
    TPanel *pnHead1_2TempShow;
    TPanel *pnHead2_1;
    TLabel *lbHead2_1;
    TPanel *pnHead2_1TempShow;
    TPanel *pnHead2_3;
    TLabel *lbHead2_3;
    TPanel *pnHead2_3TempShow;
    TPanel *pnHead2_5;
    TLabel *lbHead2_5;
    TPanel *pnHead2_5TempShow;
    TPanel *pnHead2_7;
    TLabel *lbHead2_7;
    TPanel *pnHead2_7TempShow;
    TPanel *pnHead2_8;
    TLabel *lbHead2_8;
    TPanel *pnHead2_8TempShow;
    TPanel *pnHead2_6;
    TLabel *lbHead2_6;
    TPanel *pnHead2_6TempShow;
    TPanel *pnHead2_4;
    TLabel *lbHead2_4;
    TPanel *pnHead2_4TempShow;
    TPanel *pnHead2_2;
    TLabel *lbHead2_2;
    TPanel *pnHead2_2TempShow;
    TPanel *pnShuttle1_2;
    TLabel *lbShuttle1_2;
    TPanel *pnShuttle1_2TempShow;
    TPanel *pnShuttle2_2;
    TLabel *lbShuttle2_2;
    TPanel *pnShuttle2_2TempShow;
    TPanel *pnHotPlate1_2;
    TLabel *lbHotPlate1_2;
    TPanel *pnHotPlate1_2TempShow;
    TPanel *pnHotPlate2_2;
    TLabel *lbHotPlate2_2;
    TPanel *pnHotPlate2_2TempShow;
    TGroupBox *GroupBox2;
    TGroupBox *GroupBox3;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *lblSetTemp;
    TLabel *lblSetTime;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
    TComboBox *ComboBox1;
    TButton *Button1;
    TButton *Button2;
    TGroupBox *GroupBox4;
    TStringGrid *sgTriMachineAutoDefrost;
    TPanel *pnTempSetHead1;
    TPanel *pnTempOffsetHead1;
    TPanel *pnTempSetHead3;
    TPanel *pnTempOffsetHead3;
    TPanel *pnTempSetHead2;
    TPanel *pnTempOffsetHead2;
    TPanel *pnTempSetHead5;
    TPanel *pnTempOffsetHead5;
    TPanel *pnTempSetHead6;
    TPanel *pnTempOffsetHead6;
    TPanel *pnTempSetHead8;
    TPanel *pnTempOffsetHead8;
    TPanel *pnTempHotPlate2;
    TPanel *pnTempOffsetHotPlate12;
    TPanel *pnTempSetHead7;
    TPanel *pnTempOffsetHead7;
    TPanel *pnTempHotPlate1;
    TPanel *pnTempOffsetHotPlate11;
    TPanel *pnTempSetHead4;
    TPanel *pnTempOffsetHead4;
    TPanel *pnTempSetShuttle2;
    TPanel *pnTempOffsetShuttle2;
    TPanel *pnTempSetShuttle1;
    TPanel *pnTempOffsetShuttle1;
    TPanel *pnTempSetHead1_1;
    TPanel *Panel26;
    TPanel *pnTempSetHead1_2;
    TPanel *Panel28;
    TPanel *Panel29;
    TPanel *Panel30;
    TPanel *Panel31;
    TPanel *Panel32;
    TPanel *Panel33;
    TPanel *Panel34;
    TPanel *pnTempSetHead1_3;
    TPanel *Panel36;
    TPanel *pnTempSetHead1_4;
    TPanel *Panel38;
    TPanel *Panel39;
    TPanel *Panel40;
    TPanel *Panel41;
    TPanel *Panel42;
    TPanel *Panel43;
    TPanel *Panel44;
    TPanel *Panel45;
    TPanel *Panel46;
    TPanel *Panel47;
    TPanel *Panel48;
    TPanel *Panel49;
    TPanel *Panel50;
    TPanel *Panel51;
    TPanel *Panel52;
    TPanel *Panel53;
    TPanel *Panel54;
    TPanel *Panel55;
    TPanel *Panel56;
    TPanel *Panel57;
    TPanel *Panel58;
    TPanel *Panel59;
    TPanel *Panel60;
    TPanel *Panel61;
    TPanel *Panel62;
    TPanel *Panel63;
    TPanel *Panel64;
    TPanel *pnlChangeMsg_Chi;
    TPanel *pnlChangeMsg_Eng;
    TTimer *tmrGetTemperatureStatus;
    TTimer *tmrClose;
    TGroupBox *gbAutoDefrostProduction;
    TLabel *m_labTmr;
    TLabel *lblMessage1;
    TBtnPanel *btnForceDefrost;
    TBtnPanel *btnCloseFormWaitDefrost;
    TBtnPanel *btnClose;
    TTimer *tmrMessageFlash;
    TBtnPanel *btnDefrostTermination;
    void __fastcall tmrGetTemperatureStatusTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall tmrCloseTimer(TObject *Sender);
    void __fastcall tmrMessageFlashTimer(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall btnCloseFormWaitDefrostClick(TObject *Sender);
    void __fastcall btnForceDefrostClick(TObject *Sender);
    void __fastcall btnDefrostTerminationClick(TObject *Sender);
private:    // User declarations
    TDateTime     m_dtTime;
public:     // User declarations
    __fastcall TfrmDefrost(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDefrost *frmDefrost;
//---------------------------------------------------------------------------
#endif
