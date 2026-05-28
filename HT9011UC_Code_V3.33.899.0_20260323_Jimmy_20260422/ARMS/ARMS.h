//---------------------------------------------------------------------------

#ifndef ARMSH
#define ARMSH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
typedef struct
{
    AnsiString aASEMARMSDownloadPath;
    AnsiString aASEMARMSUploadPath;
    bool bUseARMSFunction;
    bool bDownloadFormUploadPath;
    bool bRecipeDownloadIncludeOffsetFile;
    AnsiString aASEMARMSBackupUploadPath;
} ARMS_Parameter;

class TfARMS : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pcARMSFoem;
    TTabSheet *tsARMSMain;
    TTabSheet *tsARMSSetup;
    TGroupBox *gbServerPathSetup;
    TPanel *pnUploadRecipePath;
    TEdit *edUploadRecipePath;
    TSpeedButton *sbUploadRecipePath;
    TEdit *edDownloadRecipePath;
    TSpeedButton *sbDownloadRecipePath;
    TPanel *pnDownloadRecipePath;
    TGroupBox *gbARMSConfig;
    TCheckBox *cbDownloadFormUploadPath;
    TCheckBox *cbRecipeDownloadIncludeOffsetFile;
    TPanel *pnBackupUploadRecipe;
    TEdit *edBackupUploadRecipe;
    TSpeedButton *sbBackupUploadRecipe;
    TTabSheet *tsBackup;
    TPanel *pnBackup;
    TGroupBox *GroupBox1;
    TSpeedButton *sbRefreshBackupUploadloadRecipe;
    TSpeedButton *sbDeletBackupUploadRecipe;
    TSpeedButton *sbMoveToUploadRecipeFolder;
    TCheckListBox *CheckListBoxBackupUploadRecipe;
    TPanel *pnMain;
    TSpeedButton *sbMoveToDownload;
    TSpeedButton *sbARMSRecipeDownload;
    TSpeedButton *sbARMSRecipeUpload;
    TGroupBox *gbDownloadRecipe;
    TSpeedButton *sbRefreshDownloadRecipe;
    TSpeedButton *sbDeletDownloadRecipe;
    TCheckListBox *CheckListBoxDownloadRecipe;
    TGroupBox *gbUploadRecipe;
    TSpeedButton *sbRefreshUploadloadRecipe;
    TSpeedButton *sbDeletUploadRecipe;
    TCheckListBox *CheckListBoxUploadRecipe;
    TPanel *pnHandlerRecipeName;
    TComboBox *cobHandlerRecipeName;
    TDirectoryListBox *DirectoryListBox1;
    TFileListBox *FileListBox1;
    TPanel *Panel1;
    TFileListBox *FileListBoxARMS;
    TMemo *MemoRecipeLog;
    TPanel *pnMainFix3FileTitle;
    TPanel *pnButtonTitle;
    TSpeedButton *sbExit;
    TSpeedButton *sbEquipmentUpdate;
    void __fastcall sbUploadRecipePathClick(TObject *Sender);
    void __fastcall pcARMSFoemChange(TObject *Sender);
    void __fastcall sbRefreshDownloadRecipeClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sbEquipmentUpdateClick(TObject *Sender);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall sbARMSRecipeUploadClick(TObject *Sender);
    void __fastcall sbMoveToDownloadClick(TObject *Sender);
    void __fastcall sbARMSRecipeDownloadClick(TObject *Sender);
    void __fastcall sbDeletDownloadRecipeClick(TObject *Sender);
    void __fastcall FileListBoxARMSDblClick(TObject *Sender);
    void __fastcall sbMoveToUploadRecipeFolderClick(TObject *Sender);
    void __fastcall ARMSDataConversion(AnsiString asSource, AnsiString asDirection, bool bDeleteSource, bool bBackup);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
    AnsiString _FILE_PTAH;
    TIniFile* _INI_FILE;
public:     // User declarations
    __fastcall TfARMS(TComponent* Owner);
    bool fShow;
    ARMS_Parameter ARMS_PARA;

    void SetParameterToShow(TObject *PCtrl,int iValue);
    void SetParameterToShow(TObject *PCtrl,bool bValue);
    void SetParameterToShow(TObject *PCtrl,char *cValue);
    void SetParameterToShow(TObject *PCtrl,AnsiString aValue);
    void SetVCLToParameter(TComboBox *PCtrl, int &iValue);
    void SetVCLToParameter(TObject *PCtrl, char &aValue);
    void SetVCLToParameter(TObject *PCtrl, int &iValue);
    void SetVCLToParameter(TObject *PCtrl, bool &bValue);
    void SetVCLToParameter(TObject *PCtrl, AnsiString &aValue);
    void __fastcall UpdataParameter();
    bool __fastcall CheckPassword(AnsiString aPath);
    void __fastcall LoadParameter();
    void __fastcall UpdataHandlerRecipe();
    void __fastcall UploadHandlerRecipe();
    void __fastcall UnCheckedAllSelet();
    void __fastcall UpdataServerRecipe(int iTag,AnsiString sPath);
    void __fastcall UploadRecipeMoveToDownloadRecipe();
    void __fastcall DownloadRecipeFromServer(bool bflag);
    bool __fastcall CheckDirExit(int iPath);
    bool __fastcall CheckRecipeParameter();
    void __fastcall BackupHandlerRecipe(AnsiString aName);
    void __fastcall LoadRecipeLogData(AnsiString aName);
    bool __fastcall CheckListBoxIsSelete(TCheckListBox *clb);
    void __fastcall BackupRecipeMoveToUploadRecipe();
};
//---------------------------------------------------------------------------
extern PACKAGE TfARMS *fARMS;
//---------------------------------------------------------------------------
#endif
