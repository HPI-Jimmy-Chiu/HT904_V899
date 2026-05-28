//---------------------------------------------------------------------------

#ifndef cHotPlateH
#define cHotPlateH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfHotPlate : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TEdit *HotPlateName;
    TGroupBox *GroupBox1;
    TImage *Image1;
    TCheckBox *cbEnableHP2;
    TImage *Image2;
    TCheckBox *cbEnableHP1;
    TGroupBox *GroupBox2;
    TImage *Image3;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *XST1;
    TEdit *XPitch1;
    TEdit *XCT1;
    TEdit *YST1;
    TEdit *YPitch1;
    TEdit *YCT1;
    TComboBox *cbSelectHPFromDB;
    TLabel *Label8;
    TCheckBox *chkUseWideHotplate;
    TPanel *Panel2;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TCheckBox *chkTrayHotplateCheck;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall XST1KeyPress(TObject *Sender, char &Key);
    void __fastcall YCT1KeyPress(TObject *Sender, char &Key);
    void __fastcall XST1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall XCT1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall cbSelectHPFromDBChange(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall HotPlateNameMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
private:    // User declarations
    void __fastcall LoadImage();

public:     // User declarations
    __fastcall TfHotPlate(TComponent* Owner);
    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm();
    int __fastcall CheckHotPlate8PickMode();
    void SetArmHotPlateYPitch();        //Steven 20180122 : For Y-Pitch at Hot Plate
    void __fastcall SaveSetupFile(AnsiString szDir,AnsiString S);//kevin 20180926
    bool fShow;
    void ShowTypePage(int index);//JimmyChiu 20220901 add ShowTypePage func in cHotPlate.cpp
};
//---------------------------------------------------------------------------
extern PACKAGE TfHotPlate *fHotPlate;
//---------------------------------------------------------------------------
#endif
