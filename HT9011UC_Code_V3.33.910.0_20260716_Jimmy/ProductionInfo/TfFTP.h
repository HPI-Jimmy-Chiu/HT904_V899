//---------------------------------------------------------------------------

#ifndef TfFTPH
#define TfFTPH
//---------------------------------------------------------------------------
#include <NMFtp.hpp>
#include <map>
//---------------------------------------------------------------------------
class TfFTP                                                                     //JimmyChiu 20220120 add FTP class
{
private:                                                                        // User declarations
    TNMFTP *pFTP;
    bool bflag;
    AnsiString asUserID;
    AnsiString asPassword;
    AnsiString asHost;
    int  Vendor;
    int  TimeOut;
    int  Port;
    bool Passive;
    bool bNListOk;
    bool bChangeDirOk;

    std::map<int, AnsiString> mapFileList;
    void __fastcall GetFileList(AnsiString asFilePath);
    TStringList* SplitPath(AnsiString path);
    bool NavigateToCommonPrefix();
    bool NavigateOrCreateFolders(TStringList *targetFolders, AnsiString originalDir);
    AnsiString CleanPath(AnsiString path);
public:                                                                         // User declarations
    TfFTP(AnsiString _asUserID, AnsiString _asPassword, AnsiString _asHost);
    ~TfFTP();

    TStringList *lFolder;
    TStringList *lTempFolder;

    void Close();                                                               //Jimmychiu 20220901 新增Close FTP
    bool Connect();
    bool Connect(AnsiString _asUserID, AnsiString _asPassword, AnsiString _asHost, int _TimeOut=20000, int _Vendor=NMOS_AUTO, bool _Passive=true, int _Port=21);
    bool ChangeDir(AnsiString asFilePath);
    bool ChangeDirectories(AnsiString asFilePath);                              //Steven 20250716 : 多層次FTP資料夾切換
    bool DownloadFilterFile(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError);
    AnsiString DownloadFilterFile_Get1stFileName(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError);
    AnsiString DownloadFilterFile_GetLastFileName(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFilter, AnsiString asError);
    bool Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString asFileName, AnsiString asError);
    bool Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sSourceFileName, AnsiString sTargetFileName, AnsiString asError);
    bool Delete(AnsiString sSourcesFilePath, AnsiString asFilter, AnsiString asError);
    std::map<int, AnsiString> GetAllFolder(AnsiString &sSourcesFilePath, AnsiString asError);
    void CheckFTPFilePath(AnsiString &aspath);
    void CheckLocalFilePath(AnsiString &aspath);
    bool Rename(const AnsiString &sourcePath, const AnsiString &targetPath, const AnsiString &sSourFileName, const AnsiString &sTarFileName);
    AnsiString RemoveAllTrailingSlashes(AnsiString sInput);

    bool bError;
    void __fastcall NMFTP2Success(TCmdType Trans_Type);                         //Steven 20220510 : 針對FTP動作做紀錄
    void __fastcall NMFTP2TransactionStop(TObject *Sender);
    void __fastcall NMFTP2TransactionStart(TObject *Sender);
    void __fastcall NMFTP2ConnectionFailed(TObject *Sender);
    void __fastcall NMFTP2UnSupportedFunction(TCmdType Trans_Type);
    void __fastcall NMFTP2Failure(bool &Handled, TCmdType Trans_Type);
    void __fastcall NMFTP2AuthenticationFailed(bool &Handled);
    void __fastcall NMFTP2Error(TComponent *Sender, WORD Errno, AnsiString Errmsg);
    void __fastcall NMFTP2Status(TComponent *Sender, AnsiString Status);
    void __fastcall NMFTP2Connect(TObject *Sender);
    void __fastcall NMFTP2Disconnect(TObject *Sender);
    void __fastcall NMFTP2ListItem(AnsiString Listing);
};
#endif
