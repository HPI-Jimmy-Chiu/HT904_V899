//------------------------------------------------------------------------------

#ifndef cBuilderH
#define cBuilderH
//------------------------------------------------------------------------------
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//------------------------------------------------------------------------------
class TfBuilder : public TForm
{
__published:    // IDE-managed Components
    TGroupBox *GroupBox5;
    TLabel *Label78;
    TLabel *Label79;
    TComboBox *cbSourceFile;
    TEdit *edNewFileName;
    TButton *btCreateSetupFile;
    TGroupBox *GroupBox6;
    TLabel *Label81;
    TButton *btDeleteSetupFile;
    TComboBox *cbDeleteFile;
    TGroupBox *GroupBox7;
    TLabel *labDir;
    TCheckListBox *CheckListBox1;
    TDirectoryListBox *DirectoryListBox1;
    TDriveComboBox *DriveComboBox1;
    TSpeedButton *spbImport;
    TSpeedButton *spbExport;
    TPanel *spbExit;
    void __fastcall cbSourceFileChange(TObject *Sender);
    void __fastcall edNewFileNameChange(TObject *Sender);
    void __fastcall btCreateSetupFileClick(TObject *Sender);
    void __fastcall cbDeleteFileChange(TObject *Sender);
    void __fastcall btDeleteSetupFileClick(TObject *Sender);
    void __fastcall DirectoryListBox1Change(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall spbExportClick(TObject *Sender);
    void __fastcall spbImportClick(TObject *Sender);
    void __fastcall spbExitClick(TObject *Sender);
    void __fastcall edNewFileNameMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edNewFileNameKeyPress(TObject *Sender, char &Key);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
    void __fastcall InitCompData();
    void __fastcall ShowDirBoxPath();
    void __fastcall CopySourTarget(AnsiString Sour);
    bool __fastcall NeedExport(AnsiString sName);
public:     // User declarations
    __fastcall TfBuilder(TComponent* Owner);
    void __fastcall DeleteSetupFile(AnsiString DeleteFileName);                         //Steven 20110305
    void __fastcall bSaveAsJobFile(AnsiString SourceFileFileName, AnsiString FileName); //kevin 20150107 ¥t¦sjobfile
    void __fastcall Create2DCodeWorkFile();                                             //wei 20160802 add 2DCodeCreateWorkFile
    void __fastcall bSaveAllFillOrFile(AnsiString SourceFileFileName, int iFile);       //kevin 20180824 SAVE ALL FILE
    AnsiString asBackupCreate;
    bool fShow;
};
//------------------------------------------------------------------------------
extern PACKAGE TfBuilder *fBuilder;
//------------------------------------------------------------------------------
#endif
