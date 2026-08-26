//---------------------------------------------------------------------------

#ifndef uTrayEditFormH
#define uTrayEditFormH
//---------------------------------------------------------------------------

#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "HTray.h"
//---------------------------------------------------------------------------
class TTrayEditForm : public TForm
{
__published:    // IDE-managed Components
    TTMyTray *mtLoaderBuffer;
    TTimer *Timer1;
    TSpeedButton *spbUpdate;
    TSpeedButton *SpeedButton2;
    TEdit *edtYPos;
    TLabel *Label1;
    TEdit *edtXPos;
    TLabel *Label2;
    TButton *Button1;
    TBitBtn *BitBtn1;
    TButton *Button2;
    TCheckBox *CheckBox1;
    TComboBox *cbBinCount;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall mtLoaderBufferMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall mtLoaderBufferMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall mtLoaderBufferMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall spbUpdateClick(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall edtXPosClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:    // User declarations

public:     // User declarations
    __fastcall TTrayEditForm(TComponent* Owner);
    void __fastcall Change();
    void __fastcall ShowTray();
    void __fastcall SetTray();
    void __fastcall SaveJPG(AnsiString S); //ChungHung 20130715 add
    int iSendCT;
    int  iHasMap;   //Steven 20150224 : Auto Contact Test
    bool fShow;
    bool CheckCanEdit_SG(int MotorIndexIndex); //KaiChen 20201112 ：矽格-湖口，確認是否可以編輯 Loader/Unloader資料
};
//---------------------------------------------------------------------------
extern PACKAGE TTrayEditForm *TrayEditForm;
void EditTray(int MotorIndexIndex, int iHasMap=0);         //Steven 20150224 : Auto Contact Test
//---------------------------------------------------------------------------
#endif
