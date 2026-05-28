//---------------------------------------------------------------------------

#ifndef SmartDiagnosticH
#define SmartDiagnosticH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include "mycylin.h"
//---------------------------------------------------------------------------
typedef struct
{
    TPageControl *PCtrl;
    TSpeedButton *sbPtr;
    int          iTag;

} MySDPageChange;
//---------------------------------------------------------------------------
class TfSmartDiagnostic : public TForm
{
__published:	// IDE-managed Components
    TPanel *pn_SmartDiagnosticTitle;
    TSpeedButton *sb_SmartDiagnostic_Setup;
    TSpeedButton *sb_SmartDiagnostic_Summary;
    TSpeedButton *sb_SmartDiagnostic_Exit;
    TSpeedButton *sb_SmartDiagnostic_Save;
    TPageControl *pc_SmartDiagnostic;
    TTabSheet *ts_SmartDiagnostic_Setup;
    TTabSheet *ts_SmartDiagnostic_Summary;
    TPanel *pn_SmartDiagnostic_Summary;
    TStringGrid *sg_SmartDiagnostic_Summary;
    TPanel *pn_SmartDiagnostic_Setup;
    TSpeedButton *sb_SmarDiagnostic_CreateCyliderName;
    TPanel *pn_SmartDiagnostic_Time;
    TLabel *lb_SmartDiagnostic_SystemTime;
    TPanel *pn_SmartDiagnostic_CyliderManagement;
    TPanel *pn_SmartDiagnostic_CyliderManagementTop;
    TComboBox *cob_SmartDiagnostic_CyliderName;
    TSpeedButton *sb_SmartDiagnostic_ResetRecordCount;
    TStringGrid *sg_SmartDiagnostic_CyliderManagement;
    TPanel *pn_SmartDiagnostic_Parameter;
    TPanel *pn_SmartDiagnostic_ParameterTop;
    TLabel *lb_SmartDiagnostic_LimitCountValue;
    TEdit *ed_SmartDiagnostic_LimitCountValue;
    TTimer *SmartDiagnosticTimer;
    TLabel *lb_SmartDiagnostic_LimitCheckTime;
    TEdit *ed_SmartDiagnostic_LimitCheckTime;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sg_SmartDiagnostic_SummaryDrawCell(TObject *Sender,
          int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sb_SmarDiagnostic_CreateCyliderNameClick(
          TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall sb_SmartDiagnostic_ResetRecordCountClick(
          TObject *Sender);
    void __fastcall SmartDiagnosticTimerTimer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ed_SmartDiagnostic_LimitCountValueMouseDown(
          TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
          int Y);
    void __fastcall ed_SmartDiagnostic_LimitCheckTimeMouseDown(
          TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
          int Y);
private:	// User declarations

    AnsiString aSmartDiagnosticFilePath;
    AnsiString aSmartDiagnosticResetFilePath;
    AnsiString aSmartDiagnosticParaPath;
    void __fastcall FormButtonClick(TObject *Sender);
    int     iLimitCountValue;
    int     iLimitCheckTime;
    TColor  **GridColor;

public:		// User declarations
    __fastcall TfSmartDiagnostic(TComponent* Owner);

    TList   *MyPageChangeList;
    int     iRecordCyliderOnCount[MaxCylinderItem];
    int     iRecordCyliderOffCount[MaxCylinderItem];
    bool    bRecordCyStatus[MaxCylinderItem];
    bool    bStartRecord;

    void __fastcall SetCellColor(TStringGrid *sg,AnsiString aTitle, int iCol, int iRow, TColor Color);
    void __fastcall InitialStringGridColor(TStringGrid *sg, int iRow, int iCol);
    void SetVCLSizePosition(TWinControl *PCtrl , int iTop, int iLeft, int iWidth, int iHeight);
    void SetVCLToParameter(TIniFile *IniF, AnsiString Section, AnsiString Name, TObject *PCtrl, int &iValue);
    void SetParameterToShow(TIniFile *IniF, AnsiString Section, AnsiString Name, TObject *PCtrl, int &iValue);
    void InitialVariable();
    void BackupChangePage(TPageControl *pc,TSpeedButton *sb);
    void SearchChangePageButton(TWinControl *PCtrl);
    void PasteStringGridAsTabFormat(int XSTART,TStringGrid *strGrd, TStringList *memoPtr);
    void __fastcall SaveSmartDiagnosticParameter();
    void __fastcall LoadSmartDiagnosticParameter();
    void LoadSDSummaryData();
    void SaveSDSummaryData();
    void LoadSDResetSummaryData();
    void SaveSDResetSummaryData();
    void GetCyliderOnCount(AnsiString CyName);
    void GetCyliderOffCount(AnsiString CyName);
};
//---------------------------------------------------------------------------
extern PACKAGE TfSmartDiagnostic *fSmartDiagnostic;
//---------------------------------------------------------------------------
#endif
