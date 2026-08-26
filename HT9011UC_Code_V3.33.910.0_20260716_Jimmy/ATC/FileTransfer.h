//---------------------------------------------------------------------------

#ifndef FileTransferH
#define FileTransferH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <ComCtrls.hpp>
#include <Filectrl.hpp>
#include <SysUtils.hpp>
#include <vector>
//---------------------------------------------------------------------------

class TfrmFileTransfer : public TForm
{
__published:    // IDE-managed Components
private:    // User declarations

    // --- 檔案傳輸相關的狀態變數 ---
    AnsiString m_IncomingFileName; // 預期要接收的檔案名稱
    __int64 m_ExpectedFileSize;    // 預期要接收的檔案大小
    __int64 m_TotalBytesReceived;  // 目前已經接收到的檔案大小
    AnsiString m_SaveDirectory;    // 檔案儲存的目錄
    TFileStream* m_FileStream;     // 用於寫入檔案的檔案流
    AnsiString m_FileToSendPath;   // (發送方) 準備要發送的檔案路徑
    TCustomWinSocket* m_DataSocket;// 新增一個成員變數來保存資料傳輸的 Socket

public:        // User declarations
    __fastcall TfrmFileTransfer(TComponent* Owner);
    // --- 檔案傳輸相關的函式宣告 ---

    /**
     * @brief 設定接收檔案的儲存目錄
     * @param dirPath 儲存目錄的路徑 (例如: "C:\\Downloads\\")
     */
    void SetSaveDirectory(const AnsiString& dirPath);

    /**
     * @brief (傳送方) 初始化檔案傳輸請求
     * @param CommandSocket 用於傳送指令的 Socket
     * @param DataSocket    用於傳送檔案資料的 Socket
     * @param FilePath 要傳送的完整檔案路徑 (例如: "C:\\Data\\report.zip")
     * @param ProgressBar (可選) 用於顯示進度的 TProgressBar 元件
     * @return bool 是否成功發送請求
     */
     bool InitiateFileTransfer(TCustomWinSocket *CommandSocket, TCustomWinSocket *DataSocket,
                          const AnsiString& FilePath, TProgressBar* ProgressBar = NULL);

    /**
     * @brief (接收方) 處理 "指令" Socket 的讀取事件
     * @param Socket 觸發事件的 TCustomWinSocket 元件
     * @param ProgressBar (可選) 用於顯示進度的 TProgressBar 元件
     */
//    void OnCommandSocketRead(TCustomWinSocket *Socket, TProgressBar* ProgressBar = NULL);
    void TfrmFileTransfer::OnCommandSocketRead(TCustomWinSocket *Socket, TProgressBar* ProgressBar, AnsiString asBuffer , bool bRead);
    /**
     * @brief (接收方) 處理 "檔案資料" Socket 的讀取事件
     * @param Socket 觸發事件的檔案資料 Socket
     * @param ProgressBar (可選) 進度條
     */
    void OnDataSocketRead(TCustomWinSocket *Socket, TProgressBar* ProgressBar = NULL);

    /**
     * @brief 當傳輸中斷或失敗時，呼叫此函式來清理資源
     */
    void CancelTransfer();
    bool bStartFileTransfer;

private:

    /**
     * @brief (傳送方) 實際開始傳送檔案內容
     * @param ProgressBar (可選) 進度條
     */
    // 不需要 Socket 參數，它會使用內部儲存的 m_DataSocket
    void StartActualFileSend(TProgressBar* ProgressBar = NULL);

    /**
     * @brief (接收方) 處理收到的指令
     * @param Socket 觸發事件的 TCustomWinSocket 元件
     * @param Command 收到的指令字串
     * @param ProgressBar (可選) 用於顯示進度的 TProgressBar 元件
     */
    void ProcessCommand(TCustomWinSocket *Socket, const AnsiString& Command, TProgressBar*
    ProgressBar = NULL);

    /**
     * @brief (接收方) 接收並寫入檔案數據塊
     * @param buffer 包含檔案數據的緩衝區
     * @param dataSize 數據大小
     * @param ProgressBar (可選) 進度條
     */
    void ReceiveFileDataChunk(char* buffer, int dataSize, TProgressBar* ProgressBar = NULL);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFileTransfer *frmFileTransfer;
//---------------------------------------------------------------------------
#endif
