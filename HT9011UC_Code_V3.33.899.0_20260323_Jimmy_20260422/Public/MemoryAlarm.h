//---------------------------------------------------------------------------

#ifndef MemoryAlarmH
#define MemoryAlarmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMemoryAlarmForm : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlMemoryAlarm;
    TLabel *lbMemoryAlarm;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
public:     // User declarations
    __fastcall TMemoryAlarmForm(TComponent* Owner);

    bool bShow;
    TStringList *ansiFile;
    TList       *listPtr;

};
//---------------------------------------------------------------------------
extern PACKAGE TMemoryAlarmForm *MemoryAlarmForm;
//---------------------------------------------------------------------------
void ShowMemoryAlarm(AnsiString sAlarm);
void WriteAlarmLogs(AnsiString sAlarmLog);
#endif
