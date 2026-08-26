//---------------------------------------------------------------------------
// uFtpUploadThread.h
// FTP 背景上傳執行緒（fire-and-forget）類別宣告
// //AI(ht9045-v899) 20260612(CASE-20260611-001): 新增 FTP 背景上傳 thread 骨架，
//   讓 PTI Lot End 不必等待 FTP 上傳完成（語意：Lot End 完成 != 上傳完成）。
//---------------------------------------------------------------------------
#ifndef uFtpUploadThreadH
#define uFtpUploadThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <SyncObjs.hpp>   //AI(ht9045-v899) 20260612(CASE-20260611-001): TCriticalSection 來源 header
#include <list>
//---------------------------------------------------------------------------
// 單筆 FTP 上傳工作描述
// //AI(ht9045-v899) 20260612(CASE-20260611-001): 入列前所有 AnsiString 欄位需做深拷貝，
//   避免跨 thread COW（copy-on-write）reference count race。
struct TFtpUploadJob
{
    AnsiString sLocalFile;     // 本機要上傳的檔案完整路徑
    AnsiString sRemotePath;    // 遠端目的路徑
    AnsiString sHost;          // FTP 主機
    AnsiString sUser;          // 帳號
    AnsiString sPwd;           // 密碼
    int        iPort;          // 連接埠
    int        iRetryLeft;     // 剩餘重試次數
    int        iJobKind;       // 0 = JamAlarm、1 = SocketID CSV
};
//---------------------------------------------------------------------------
// FTP 背景上傳執行緒
// 注意：Execute() 完全不碰 VCL、不碰硬體全域（MOT[]/Sen[]）、不碰 fFTPClient，
//       背景 log 亦不使用 TMyStringList / MyDBIProcess（非 thread-safe），
//       改以純 Win32/CRT（CreateDirectory + fopen）寫入獨立純文字檔。
class TFtpUploadThread : public TThread
{
private:
    HANDLE eJob;    // manual-reset event：有工作待處理
    HANDLE eEnd;    // manual-reset event：要求結束

    bool   bEndThread;

    TCriticalSection         *csQueue;   // 保護工作佇列（同時保護 lstJobs 與 sInFlightKey）
    std::list<TFtpUploadJob>  lstJobs;   // 工作佇列

    // //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 去重用 — 目前正在上傳中的 job 識別鍵
    //   （空字串=無 in-flight）。由 PopJob 取出時設定、Execute 上傳結束後清除，皆於 csQueue 鎖內更新。
    AnsiString sInFlightKey;

    // //AI(ht9045-v899) 20260612(CASE-20260611-001): S8 背景上傳「放棄(GIVEUP)」結果回報佇列。
    //   worker 重試用盡仍失敗時推入；主執行緒 Timer 撈出寫主 EventLog。
    //   採「結果佇列 + 主執行緒撈取」而非 Synchronize：避免 FormClose EndThread()+WaitFor() 與
    //   worker 卡在 Synchronize 互等的死結；worker 端永不阻塞。
    TCriticalSection         *csResult;     // 保護 lstResults（與 csQueue 分開，避免鎖序問題）
    std::list<AnsiString>     lstResults;   // 待主執行緒撈取的回報訊息

    void __fastcall PushResult(const AnsiString &asMsg);          // worker 端推入回報（thread-safe，深拷貝）

    void __fastcall WriteBgLog(const AnsiString &asMsg);          // 背景獨立純文字 log（每日子資料夾 + 每日輪替檔）
    void __fastcall UploadOneJob(const TFtpUploadJob &job);       // S2：WinINet FTP 真實上傳（失敗只記 log）
    bool __fastcall PopJob(TFtpUploadJob &job);                   // 由佇列取出一筆（深拷貝；取出即設 in-flight key）

    // //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 去重識別鍵 = (iJobKind + sLocalFile)。
    static AnsiString __fastcall MakeJobKey(const TFtpUploadJob &job);
protected:
    void __fastcall Execute();
public:
    __fastcall TFtpUploadThread(bool CreateSuspended);
    __fastcall ~TFtpUploadThread();

    // 將一筆上傳工作放入佇列（執行緒安全；內部做 AnsiString 深拷貝 + 去重後喚醒 thread）
    void __fastcall Enqueue(const TFtpUploadJob &job);
    // //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 job 組裝 helper — 給 S6/S7 的 uLotInfo 呼叫點用。
    //   把「來源參數 -> TFtpUploadJob -> Enqueue（含去重）」封裝起來；內部不碰 VCL/全域。
    void __fastcall EnqueueUpload(int jobKind, AnsiString localFile, AnsiString remotePath,
                                  AnsiString host, AnsiString user, AnsiString pwd,
                                  int port, int retry);
    // 通知執行緒結束（喚醒 + 設結束旗標，讓 Execute 跳出迴圈）
    void __fastcall EndThread();

    // //AI(ht9045-v899) 20260612(CASE-20260611-001): S8 主執行緒撈一筆背景上傳「放棄」回報；
    //   回傳 true 表示有撈到（asMsg 為深拷貝）。僅由主執行緒呼叫，撈出後由主執行緒寫 EventLog，
    //   故本函式本身不碰 VCL/全域。
    bool __fastcall FetchResult(AnsiString &asMsg);
};
//---------------------------------------------------------------------------
// 全域單例指標：生命週期（建立/釋放）於 S4 由 main.cpp 接管，本階段僅宣告 extern。
extern TFtpUploadThread *FtpUploadThd;
//---------------------------------------------------------------------------
#endif
