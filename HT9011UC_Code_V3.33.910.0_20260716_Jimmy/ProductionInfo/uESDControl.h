//---------------------------------------------------------------------------

#ifndef uESDControlH
#define uESDControlH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <FileCtrl.hpp>
#include <Buttons.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
typedef struct
{
    int iForwardTime;
    int iSetTimeCount;
    AnsiString aStartTime[20];
    AnsiString aEndTime[20];
    bool bESDReady;
} ESD_CONFIG;
extern ESD_CONFIG ESDConfig;
//---------------------------------------------------------------------------
class TESDForm : public TForm
{
__published:    // IDE-managed Components
    TPageControl *pcESDMain;
    TTabSheet *tsESDMain;
    TTabSheet *tsESDSetup;
    TPanel *pn_ESDDataItem;
    TDBGrid *ESDDBGrid;
    TDataSource *DataSourceESD;
    TTable *TableESD;
    TQuery *ESDQuery;
    TFileListBox *FileListBox1;
    TDirectoryListBox *DirectoryListBox1;
    TPanel *Panel1;
    TStringGrid *sgESDSetTime;
    TLabel *Label1;
    TEdit *edESDCheckForwardTime;
    TSpeedButton *sbAddESDTimeSet;
    TSpeedButton *sbDeleteESDTime;
    TSpeedButton *sbUpdate;
    TDateTimePicker *DateTimePickerESD;
    TSpeedButton *sbExit;
    TSpeedButton *sbSetTime;
    TSpeedButton *SpeedButton1;
    TEdit *edtWESD;
    TEdit *edtLESD;
    TEdit *edtRESD;
    TSpeedButton *sbESDDataImport;
    TTabSheet *TabSheet1;
    TStringGrid *sg_List_AUTOMOTIVE;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sbUpdateClick(TObject *Sender);
    void __fastcall sgESDSetTimeSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall sbAddESDTimeSetClick(TObject *Sender);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall sbDeleteESDTimeClick(TObject *Sender);
    void __fastcall sbSetTimeClick(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall sbESDDataImportClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    bool __fastcall CheckAUTOMOTIVEByCSV(AnsiString asCheckUserName, AnsiString asCsvFilePath);
    void __fastcall edESDCheckForwardTimeClick(TObject *Sender);
private:    // User declarations
    bool SaveESDControlMsgToFile(AnsiString sFilePath, AnsiString sFileName, AnsiString sMsg); //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
    AnsiString ExangeBin2Txt(AnsiString asSource);                              //Jimmychiu 20230223 帳密權限加密，提升資訊安全
    AnsiString ExangeTxt2Bin(AnsiString asSource);                              //Jimmychiu 20230223 帳密權限加密，提升資訊安全
public:     // User declarations
    __fastcall TESDForm(TComponent* Owner);
    TStringList *ESDList;
    int iRow;
    int iCol;
    double fDatabaseTime;
    bool bCheck;
    int iWhichTimeRange;
    int iOldWhichTimeRange;
    bool bCKTime;
    bool __fastcall UpdateESDDate();
    void __fastcall CreateESDTable();
    void __fastcall CreateESDInformation(TStringList *slESD);
    void __fastcall SaveESDData();
    void __fastcall LoadESDData();
    bool __fastcall CheckTimeRange(TDateTime DT);
    TDateTime ConversionDateFormat(AnsiString aDate, AnsiString aTime);
    bool SearchAndCheckESD();
    void __fastcall CheckWhichTimeRange();
    void SetSaveESDControlMsgToFile(AnsiString sFilePath, AnsiString sFileName);//Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
    AnsiString aDebugLog;                                                       //Sam 20170822 改為 HT9045 方式
    void WriteLogs(AnsiString S);                                               //Sam 20170822 改為 HT9045 方式
    bool __fastcall FTP_ESDFile_Download(AnsiString sFTPPath,bool IsAUTOMOTIVE);//Sam 20170822 (Steven) 改為 HT9045 方式
    AnsiString __fastcall FTP_PasswordFile_Download(AnsiString sFTPPath);       //JimmyChiu 20220128 add
    void __fastcall FTPList(AnsiString Listing);                                //Sam 20170822 改為 HT9045 方式
    void __fastcall AUOTMOTIVEFTPList(AnsiString Listing);                      //KaiChen 20171116 (Steven) ：超豐 人員車規認證 (AUTOMOTIVE)
    AnsiString sSaveESDControlMsgToFileFilePath;
    AnsiString sSaveESDControlMsgToFileFileName;
    bool bESDFTPAction;
};
//---------------------------------------------------------------------------
extern PACKAGE TESDForm *ESDForm;
//---------------------------------------------------------------------------
bool SplitStrByDotSpaceOnly_ESD(char *str, char *dest, int Max);
#endif
