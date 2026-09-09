//---------------------------------------------------------------------------

#ifndef PMAlarmShowDataH
#define PMAlarmShowDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>

#include "PMAlarmSystem.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>

//---------------------------------------------------------------------------
class TfPMAlarm_ShowData : public TForm
{
__published:    // IDE-managed Components
    TStringGrid *sg_PMAlarmShowData;
    TComboBox *cb_PMAlarmChoiseFile;
    TPanel *pnlPMAlarmBeforeDays;
    TPanel *pnlPMAlarm_Title;
    void __fastcall cb_PMAlarmChoiseFileChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfPMAlarm_ShowData(TComponent* Owner);

    void LoadFileFromCSV(AnsiString sFullFileName);
    void RefreshFileList();

    AnsiString sDataFilePath; //20150707

};
//---------------------------------------------------------------------------
extern PACKAGE TfPMAlarm_ShowData *fPMAlarm_ShowData;
//---------------------------------------------------------------------------
#endif
