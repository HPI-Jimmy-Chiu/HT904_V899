//---------------------------------------------------------------------------
// uFtpUploadThread.cpp
// FTP 背景上傳執行緒（fire-and-forget）實作
// //AI(ht9045-v899) 20260612(CASE-20260611-001): 新增 FTP 背景上傳 thread 骨架（S1）。
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S2 完成 — UploadOneJob 改用 WinINet
//   真實上傳（FtpPutFile）；背景 log 改成每日輪替檔（仿 EventLogTxt 每日命名慣例）。
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 完成 — Enqueue 去重（jobKind+localFile 識別鍵
//   + in-flight key）、新增 EnqueueUpload job 組裝 helper；log 改每日子資料夾。
//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "uFtpUploadThread.h"
#pragma package(smart_init)

#include <stdio.h>
#include <windows.h>
#include <wininet.h>   //AI(ht9045-v899) 20260612(CASE-20260611-001): WinINet FTP API（InternetOpen/InternetConnect/FtpPutFile）
//---------------------------------------------------------------------------
// 全域單例指標 FtpUploadThd 的「唯一正式定義」已於 S4 移至 main.cpp（與 ScanBtnThd 鄰近），
// extern 宣告在 uFtpUploadThread.h。本檔不再給定義，避免 duplicate symbol link error。
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S4 移除此處暫時定義，改由 main.cpp 定義。

//---------------------------------------------------------------------------
// 背景 log 路徑：沿用專案慣例 D:\HT9045_Log\<子資料夾>
//   FACT 慣例來源：ATCInterface.cpp:567 "D:\HT9045_Log\Comm_ATC\"、
//                  AutoAlignment\CAlignmentB.cpp:1246 "D:\HT9045_Log\AutoAlignFlow"。
// //AI(ht9045-v899) 20260612(CASE-20260611-001): 背景 log 改「每日一個子資料夾 + 每日一檔」，
//   避免單一資料夾檔案過多：D:\HT9045_Log\FtpUpload\YYYYMMDD\FtpUploadThread_YYYYMMDD.log。
static const char *FTPUP_LOG_ROOT = "D:\\HT9045_Log";
static const char *FTPUP_LOG_DIR  = "D:\\HT9045_Log\\FtpUpload";
//---------------------------------------------------------------------------
__fastcall TFtpUploadThread::TFtpUploadThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
    // 兩個 manual-reset event，初始皆為未signaled
    eJob = CreateEvent(NULL, true, false, NULL);   // 有工作
    eEnd = CreateEvent(NULL, true, false, NULL);   // 結束

    bEndThread = false;
    sInFlightKey = "";   //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 初始無 in-flight job

    csQueue = new TCriticalSection();
    csResult = new TCriticalSection();   //AI(ht9045-v899) 20260612(CASE-20260611-001): S8 結果回報佇列專用鎖
}
//---------------------------------------------------------------------------
__fastcall TFtpUploadThread::~TFtpUploadThread()
{
    try
    {
        if(eJob) { CloseHandle(eJob); eJob = NULL; }
        if(eEnd) { CloseHandle(eEnd); eEnd = NULL; }
        if(csQueue) { delete csQueue; csQueue = NULL; }
        if(csResult) { delete csResult; csResult = NULL; }   //AI(ht9045-v899) 20260612(CASE-20260611-001): S8 釋放結果回報鎖
    }
    catch(...)
    {
        // 解構期間不丟例外、不碰 VCL 全域
    }
}
//---------------------------------------------------------------------------
// 背景獨立純文字 log：純 Win32/CRT，不碰 TMyStringList / MyDBIProcess（非 thread-safe）
// //AI(ht9045-v899) 20260612(CASE-20260611-001): 改每日子資料夾 + 每日輪替檔，跨日自動換子資料夾+檔名。
void __fastcall TFtpUploadThread::WriteBgLog(const AnsiString &asMsg)
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    // 依當下日期組出當日子資料夾：D:\HT9045_Log\FtpUpload\YYYYMMDD
    char szDateDir[MAX_PATH];
    _snprintf(szDateDir, sizeof(szDateDir) - 1,
              "%s\\%04d%02d%02d",
              FTPUP_LOG_DIR, st.wYear, st.wMonth, st.wDay);
    szDateDir[sizeof(szDateDir) - 1] = 0;

    // 逐層建立目錄：Win32 CreateDirectory 不會自動建中間層，必須一層一層建。
    //   已存在會回 false（ERROR_ALREADY_EXISTS），可安全忽略。
    CreateDirectory(FTPUP_LOG_ROOT, NULL);   // 第1層 D:\HT9045_Log
    CreateDirectory(FTPUP_LOG_DIR,  NULL);   // 第2層 D:\HT9045_Log\FtpUpload
    CreateDirectory(szDateDir,      NULL);   // 第3層 D:\HT9045_Log\FtpUpload\YYYYMMDD

    // 當日檔名放在當日子資料夾內：<日期夾>\FtpUploadThread_YYYYMMDD.log（跨日自動換新檔）
    char szFile[MAX_PATH];
    _snprintf(szFile, sizeof(szFile) - 1,
              "%s\\FtpUploadThread_%04d%02d%02d.log",
              szDateDir, st.wYear, st.wMonth, st.wDay);
    szFile[sizeof(szFile) - 1] = 0;

    FILE *fp = fopen(szFile, "a");
    if(fp)
    {
        fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond,
                asMsg.c_str());
        fclose(fp);
    }
}
//---------------------------------------------------------------------------
// 組 job 識別鍵：(iJobKind + sLocalFile)
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 去重識別鍵。
//   理由：同一類型 + 同一本地檔完整路徑 = 同一件工作（fire-and-forget 語意）。
//   sRemotePath 不納入：同一本地檔的重複上傳請求，即使 remote 略異仍屬「第一次做完前的重複請求」，應省略。
AnsiString __fastcall TFtpUploadThread::MakeJobKey(const TFtpUploadJob &job)
{
    AnsiString asKey;
    asKey.sprintf("%d|%s", job.iJobKind, job.sLocalFile.c_str());
    return asKey;
}
//---------------------------------------------------------------------------
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S8 worker 端推入「放棄」回報（thread-safe）
//   以 c_str() 重建 AnsiString 強制配置獨立緩衝區（refcount=1），避免跟主執行緒 COW race。
//   另用 csResult（不是 csQueue），避免與上傳主路徑鎖競爭、亦無鎖序問題。
void __fastcall TFtpUploadThread::PushResult(const AnsiString &asMsg)
{
    AnsiString asCopy = AnsiString(asMsg.c_str());
    csResult->Acquire();
    try
    {
        lstResults.push_back(asCopy);
    }
    __finally
    {
        csResult->Release();
    }
}
//---------------------------------------------------------------------------
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S8 主執行緒撈一筆回報（深拷貝）；空佇列回傳 false。
bool __fastcall TFtpUploadThread::FetchResult(AnsiString &asMsg)
{
    bool bHas = false;
    csResult->Acquire();
    try
    {
        if(!lstResults.empty())
        {
            asMsg = AnsiString(lstResults.front().c_str());
            lstResults.pop_front();
            bHas = true;
        }
    }
    __finally
    {
        csResult->Release();
    }
    return bHas;
}
//---------------------------------------------------------------------------
// 將工作放入佇列（執行緒安全 + 去重）
void __fastcall TFtpUploadThread::Enqueue(const TFtpUploadJob &job)
{
    // 以 c_str() 重建 AnsiString，強制配置獨立緩衝區（refcount=1），
    // 避免呼叫端與 thread 之間共用 COW 緩衝造成 reference count race。
    TFtpUploadJob jobCopy;
    jobCopy.sLocalFile  = AnsiString(job.sLocalFile.c_str());
    jobCopy.sRemotePath = AnsiString(job.sRemotePath.c_str());
    jobCopy.sHost       = AnsiString(job.sHost.c_str());
    jobCopy.sUser       = AnsiString(job.sUser.c_str());
    jobCopy.sPwd        = AnsiString(job.sPwd.c_str());
    jobCopy.iPort       = job.iPort;
    jobCopy.iRetryLeft  = job.iRetryLeft;
    jobCopy.iJobKind    = job.iJobKind;

    AnsiString asKey = MakeJobKey(jobCopy);

    // //AI(ht9045-v899) 20260612(CASE-20260611-001): 去重檢查與 in-flight key 比對必須同一把鎖
    //   （csQueue），避免與 PopJob 設 in-flight / Execute 清 in-flight race。
    bool bDup = false;
    csQueue->Acquire();
    try
    {
        // (1) 是否有同鍵 job 正在上傳中（in-flight）
        if(asKey != "" && sInFlightKey == asKey)
        {
            bDup = true;
        }
        else
        {
            // (2) 佇列中是否已有同鍵的待處理 job
            for(std::list<TFtpUploadJob>::iterator it = lstJobs.begin();
                it != lstJobs.end(); ++it)
            {
                if(MakeJobKey(*it) == asKey)
                {
                    bDup = true;
                    break;
                }
            }
        }

        if(!bDup)
            lstJobs.push_back(jobCopy);
    }
    __finally
    {
        csQueue->Release();
    }

    if(bDup)
    {
        // 命中去重 — 不入列，在鎖外寫一筆背景 log（避免持鎖做檔案 IO）
        AnsiString asLog;
        asLog.sprintf("DEDUP skip: kind=%d file=%s already queued or in-flight",
                      jobCopy.iJobKind, jobCopy.sLocalFile.c_str());
        WriteBgLog(asLog);
        return;
    }

    // 喚醒 Execute
    SetEvent(eJob);
}
//---------------------------------------------------------------------------
// job 組裝 helper：來源參數 -> TFtpUploadJob -> Enqueue（含去重）
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 提供主執行緒方便呼叫介面，給 S6/S7 的 uLotInfo 用。
//   內部不碰 VCL/全域；AnsiString 深拷貝改由 Enqueue 負責。
void __fastcall TFtpUploadThread::EnqueueUpload(int jobKind, AnsiString localFile,
        AnsiString remotePath, AnsiString host, AnsiString user, AnsiString pwd,
        int port, int retry)
{
    TFtpUploadJob job;
    job.iJobKind    = jobKind;
    job.sLocalFile  = localFile;
    job.sRemotePath = remotePath;
    job.sHost       = host;
    job.sUser       = user;
    job.sPwd        = pwd;
    job.iPort       = port;
    job.iRetryLeft  = retry;

    Enqueue(job);   // Enqueue 內含深拷貝 + 去重
}
//---------------------------------------------------------------------------
// 由佇列取出一筆工作（執行緒安全）；回傳 true 表示有取到
bool __fastcall TFtpUploadThread::PopJob(TFtpUploadJob &job)
{
    bool bHas = false;
    csQueue->Acquire();
    try
    {
        if(!lstJobs.empty())
        {
            job = lstJobs.front();
            lstJobs.pop_front();
            bHas = true;
            // //AI(ht9045-v899) 20260612(CASE-20260611-001): 取出即標記為 in-flight
            //   （同一把鎖內，避免與 Enqueue 去重檢查 race）。
            sInFlightKey = MakeJobKey(job);
        }
    }
    __finally
    {
        csQueue->Release();
    }
    return bHas;
}
//---------------------------------------------------------------------------
// 實際上傳一筆工作（WinINet FTP）
// //AI(ht9045-v899) 20260612(CASE-20260611-001): S2 實作 WinINet 真實上傳。
//   流程：InternetOpen -> InternetConnect(FTP) -> FtpPutFile(BINARY) -> CloseHandle。
//   連線逾時 5000ms（比照現有 PTI TimeOut=5000 語意）。
//   依 job.iRetryLeft 重試，失敗間隔 ::Sleep（背景 thread、不凍結 UI）。
//   全程僅用 Win32/WinINet，不碰 fFTPClient / FileListBox1 / VCL / MOT[] / Sen[]。
//   失敗只記背景 log（檔名、host、GetLastError、InternetGetLastResponseInfo），不跳 alarm。
void __fastcall TFtpUploadThread::UploadOneJob(const TFtpUploadJob &job)
{
    // 總嘗試次數 = 首次 1 次 + iRetryLeft 次重試（至少 1 次）
    int iAttemptsLeft = job.iRetryLeft + 1;
    if(iAttemptsLeft < 1) iAttemptsLeft = 1;

    const DWORD dwSleepMs    = 2000;   // 重試間隔（背景 thread，可安全 ::Sleep）
    const DWORD dwTimeoutMs  = 5000;   // 連線逾時（比照 PTI TimeOut=5000）

    // //AI(ht9045-v899) 20260612(CASE-20260611-001): 連線參數與遠端路徑預設取自 job；
    //   以下用區域變數承接,正式組建(未定義 SOFT_SIMULTE)時與 job.* 完全相同,行為不變。
    AnsiString asHost   = job.sHost;
    AnsiString asUser   = job.sUser;
    AnsiString asPwd    = job.sPwd;
    int        iPort    = job.iPort;
    AnsiString asRemote = job.sRemotePath;

#ifdef SOFT_SIMULTE
    // //AI(ht9045-v899) 20260612(CASE-20260611-001): 本機驗證模式,比照 GetFTP_Setting 的
    //   #ifdef SOFT_SIMULTE 做法,把背景上傳導向本機模擬 FTP(127.0.0.1,帳密同 HONPREC/27025312)。
    //   遠端固定測試夾 /HT9045_PTI_BgTest/,依 job 類型加前綴(JAM_=JamAlarm / SID_=SocketID)方便驗證。
    //   ※ 此區塊只在定義 SOFT_SIMULTE 的本機組建生效,正式客戶組建完全不受影響。
    asHost = "127.0.0.1";
    asUser = "HONPREC";
    asPwd  = "27025312";
    iPort  = 21;
    {
        int iSep = job.sLocalFile.LastDelimiter("\\/");
        AnsiString asLeaf = (iSep > 0)
            ? job.sLocalFile.SubString(iSep + 1, job.sLocalFile.Length() - iSep)
            : job.sLocalFile;
        AnsiString asPrefix = (job.iJobKind == 0) ? "JAM_" : "SID_";
        asRemote = "/HT9045_PTI_BgTest/" + asPrefix + asLeaf;
    }
    // //AI(ht9045-v899) 20260612(CASE-20260611-001): 本機測試一律維持 PASV(與正式客戶版同一條路徑),
    //   不在 SOFT_SIMULTE 偷改 Active,否則測到的不是客戶實際走的路徑。
    //   loopback PASV 逾時(err=12002)屬 FileZilla 被動埠/防火牆環境問題,應由伺服器設定解決,不靠改 code 繞。
    WriteBgLog("SIMULATE redirect -> host=127.0.0.1 remote=" + asRemote);
#endif

    bool      bOK       = false;
    int       iAttempt  = 0;
    DWORD     dwErr     = 0;
    AnsiString asResp;

    DWORD dwStart = GetTickCount();

    while(iAttemptsLeft > 0 && !bEndThread)
    {
        iAttempt++;
        bOK    = false;
        dwErr  = 0;
        asResp = "";

        // 1) 開啟 WinINet session
        HINTERNET hSession = InternetOpen("HT9045_FtpUpload",
                                          INTERNET_OPEN_TYPE_PRECONFIG,
                                          NULL, NULL, 0);
        if(hSession == NULL)
        {
            dwErr = GetLastError();
        }
        else
        {
            // 設定連線逾時 5000ms
            DWORD dwTimeout = dwTimeoutMs;
            InternetSetOption(hSession, INTERNET_OPTION_CONNECT_TIMEOUT,
                              &dwTimeout, sizeof(dwTimeout));

            // 2) 連線 FTP（被動模式較易穿越防火牆）
            HINTERNET hConn = InternetConnect(hSession,
                                              asHost.c_str(),
                                              (INTERNET_PORT)iPort,
                                              asUser.c_str(),
                                              asPwd.c_str(),
                                              INTERNET_SERVICE_FTP,
                                              INTERNET_FLAG_PASSIVE,
                                              0);
            if(hConn == NULL)
            {
                dwErr = GetLastError();
            }
            else
            {
                // //AI(ht9045-v899) 20260612(CASE-20260611-001): 上傳前先嘗試建立遠端目錄(正式+模擬共用)。
                //   FtpPutFile 只建檔不建目錄,若 FTP 端目標目錄不存在會上傳失敗;客戶端亦可能發生(改路徑/新站台)。
                //   從遠端「檔案」路徑反推「目錄」(最後一個 / 或 \ 之前)再 FtpCreateDirectory:
                //     - 已存在 / 無建目錄權限 → 只回 FALSE,直接忽略(只建空目錄,不覆蓋/不刪除既有內容,零風險);
                //     - 多層父目錄缺失 → 單次僅能建一層仍失敗,屆時 FtpPutFile 失敗走原重試/GIVEUP,行為不比現況差。
                //   最壞=與現況相同(上傳失敗),最好=救回上傳,故正式版亦套用。
                {
                    int iDirSep = asRemote.LastDelimiter("\\/");
                    if(iDirSep > 1)   // >1 排除「根目錄底下檔案」(開頭單一 '/'),根目錄不需建
                    {
                        AnsiString asRemoteDir = asRemote.SubString(1, iDirSep - 1);
                        FtpCreateDirectory(hConn, asRemoteDir.c_str());
                    }
                }
                // 3) 上傳檔案（二進位）
                if(FtpPutFile(hConn,
                              job.sLocalFile.c_str(),
                              asRemote.c_str(),
                              FTP_TRANSFER_TYPE_BINARY,
                              0))
                {
                    bOK = true;
                }
                else
                {
                    dwErr = GetLastError();

                    // 取 FTP 伺服器最後回應字串（協助診斷）
                    char  szResp[512];
                    DWORD dwRespLen = sizeof(szResp) - 1;
                    DWORD dwRespErr = 0;
                    szResp[0] = 0;
                    if(InternetGetLastResponseInfo(&dwRespErr, szResp, &dwRespLen))
                    {
                        if(dwRespLen >= sizeof(szResp)) dwRespLen = sizeof(szResp) - 1;
                        szResp[dwRespLen] = 0;
                        asResp = szResp;
                    }
                }
                // connect handle 與 session handle 都要關
                InternetCloseHandle(hConn);
            }
            InternetCloseHandle(hSession);
        }

        if(bOK)
            break;

        iAttemptsLeft--;
        // 還有重試機會 -> 記一筆失敗 + 短暫 sleep 後再試
        if(iAttemptsLeft > 0 && !bEndThread)
        {
            AnsiString asRetry;
            asRetry.sprintf("UPLOAD FAIL (attempt %d) host=%s file=%s remote=%s err=%u resp=%s ; will retry",
                            iAttempt,
                            asHost.c_str(),
                            job.sLocalFile.c_str(),
                            asRemote.c_str(),
                            (unsigned)dwErr,
                            asResp.c_str());
            WriteBgLog(asRetry);
            ::Sleep(dwSleepMs);
        }
    }

    DWORD dwCost = GetTickCount() - dwStart;

    AnsiString asLog;
    if(bOK)
    {
        // 成功也寫 log（檔名、耗時）
        asLog.sprintf("UPLOAD OK kind=%d host=%s file=%s remote=%s attempts=%d cost=%ums",
                      job.iJobKind,
                      asHost.c_str(),
                      job.sLocalFile.c_str(),
                      asRemote.c_str(),
                      iAttempt,
                      (unsigned)dwCost);
    }
    else
    {
        // iRetryLeft 用完仍失敗 -> 只記 log，不跳 alarm、不碰 VCL
        asLog.sprintf("UPLOAD GIVEUP kind=%d host=%s file=%s remote=%s attempts=%d err=%u resp=%s cost=%ums",
                      job.iJobKind,
                      asHost.c_str(),
                      job.sLocalFile.c_str(),
                      asRemote.c_str(),
                      iAttempt,
                      (unsigned)dwErr,
                      asResp.c_str(),
                      (unsigned)dwCost);
    }
    WriteBgLog(asLog);

    // //AI(ht9045-v899) 20260612(CASE-20260611-001): S8 只有「放棄(GIVEUP)」才回報主 EventLog（訊息型、不跳 alarm）；
    //   成功與重試中間失敗只寫背景 log，避免主 EventLog 噪音。不在此碰 SaveEventLogInfo（非 thread-safe）。
    if(!bOK)
    {
        AnsiString asRpt;
        asRpt.sprintf("FTP background upload give up (kind=%d) file=%s host=%s err=%u",
                      job.iJobKind, job.sLocalFile.c_str(), asHost.c_str(), (unsigned)dwErr);
        PushResult(asRpt);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFtpUploadThread::Execute()
{
    WriteBgLog("FtpUploadThread started.");

    HANDLE hWait[2];
    hWait[0] = eJob;
    hWait[1] = eEnd;

    do
    {
        // 等待「有工作」或「結束」其一被signaled
        WaitForMultipleObjects(2, hWait, false, INFINITE);

        if(bEndThread)
            break;

        // 先重置 eJob 再清佇列：避免「清空後才進來的新工作」漏掉喚醒
        // （Enqueue 一定在 push 之後 SetEvent，故重置在前不會 lost-wakeup）。
        ResetEvent(eJob);

        TFtpUploadJob job;
        while(PopJob(job))
        {
            if(bEndThread)
                break;
            UploadOneJob(job);   // 上傳在鎖外進行（可能數秒），只有取 job/設 in-flight 與清 in-flight 才持鎖

            // //AI(ht9045-v899) 20260612(CASE-20260611-001): 上傳結束 -> 清除 in-flight 標記（鎖內短暫更新）
            csQueue->Acquire();
            try { sInFlightKey = ""; }
            __finally { csQueue->Release(); }
        }
    }
    while(!bEndThread);

    WriteBgLog("FtpUploadThread stopped.");
}
//---------------------------------------------------------------------------
// 通知結束：設旗標 + 喚醒 Execute（讓它跳出迴圈）
void __fastcall TFtpUploadThread::EndThread()
{
    bEndThread = true;
    SetEvent(eEnd);
    SetEvent(eJob);
}
//---------------------------------------------------------------------------
