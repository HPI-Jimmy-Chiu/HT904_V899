//---------------------------------------------------------------------------

#ifndef ARMSLogH
#define ARMSLogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfARMSLog : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnARMSLog;
    TShape *ShapeInfoBox1;
    TShape *ShapeInfoBox2;
    TShape *ShapeInfoBox3;
    TShape *ShapeInfoBox4;
    TShape *ShapeInfoBox5;
    TShape *ShapeInfoBox6;
    TShape *ShapeInfoBox7;
    TShape *ShapeInfoBox8;
    TShape *ShapeInfoBox9;
    TShape *ShapeInfoBox10;
    TShape *ShapeInfoBox11;
    TShape *ShapeInfoBox12;
    TShape *ShapeInfoBox13;
    TShape *ShapeInfoBox14;
    TShape *ShapeInfoBox15;
    TShape *ShapeInfoBox16;
    TShape *ShapeInfoBox17;
    TShape *ShapeInfoBox18;
    TShape *ShapeInfoBox19;
    TShape *ShapeInfoBox20;
    TShape *ShapeInfoBox21;
    TShape *ShapeInfoBox22;
    TShape *ShapeInfoBox23;
    TShape *ShapeInfoBox24;
    TShape *ShapeInfoBox25;
    TShape *ShapeInfoBox26;
    TShape *ShapeInfoBox27;
    TShape *ShapeInfoBox28;
    TShape *ShapeInfoBox52;
    TShape *ShapeInfoBox51;
    TShape *ShapeInfoBox50;
    TShape *ShapeInfoBox49;
    TShape *ShapeInfoBox48;
    TShape *ShapeInfoBox47;
    TShape *ShapeInfoBox46;
    TShape *ShapeInfoBox45;
    TShape *ShapeInfoBox44;
    TShape *ShapeInfoBox43;
    TShape *ShapeInfoBox42;
    TShape *ShapeInfoBox41;
    TShape *ShapeInfoBox40;
    TShape *ShapeInfoBox39;
    TShape *ShapeInfoBox38;
    TShape *ShapeInfoBox37;
    TShape *ShapeInfoBox36;
    TShape *ShapeInfoBox35;
    TShape *ShapeInfoBox34;
    TShape *ShapeInfoBox33;
    TShape *ShapeInfoBox32;
    TShape *ShapeInfoBox31;
    TShape *ShapeInfoBox30;
    TShape *ShapeInfoBox29;
    TShape *ShapeInfoBox53;
    TShape *ShapeInfoBox54;
    TPanel *pnARMSTitle;
    TMemo *MemoARMSLog;
    TPanel *pnButtonTitle;
    TSpeedButton *sbExit;
    TSpeedButton *sbSave;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall sbSaveClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfARMSLog(TComponent* Owner);

    bool bShow;
    bool bSaveReady;
    AnsiString SavePath;
    AnsiString SaveName;

    void SaveMemoLog();
};
//---------------------------------------------------------------------------
extern PACKAGE TfARMSLog *fARMSLog;
//---------------------------------------------------------------------------
#endif
