//---------------------------------------------------------------------------
//---------------------RD6 Evan 2025-10-21------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "FileTransfer.h"
//#include "cMydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmFileTransfer *frmFileTransfer;
//---------------------------------------------------------------------------
__fastcall TfrmFileTransfer::TfrmFileTransfer(TComponent* Owner)
    : TForm(Owner)
{
    // --- 狀態變數初始化 ---
    m_IncomingFileName = "";
    m_ExpectedFileSize = 0;
    m_TotalBytesReceived = 0;
    m_SaveDirectory = "D:\\ATC\\Data\\SaveFile\\";
    m_FileStream = NULL;
    m_FileToSendPath = "";
    m_DataSocket = NULL; // 初始化資料 Socket 指標
}
//---------------------------------------------------------------------------

//@brief 設定接收檔案的儲存目錄

void TfrmFileTransfer::SetSaveDirectory(const AnsiString& dirPath)
{
    //dirPath="C:\\Users\\evanhsieh\\Desktop\\BCBtest\\Network\\SaveFile"
    AnsiString sPath = dirPath;
    if (!DirectoryExists(sPath))
    {
        ForceDirectories(sPath); // 使用 ForceDirectories 確保完整路徑被建立
    }
    m_SaveDirectory = IncludeTrailingBackslash(dirPath); // 確保路徑以 '\' 結尾
}
//---------------------------------------------------------------------------
//@brief (發送方) 初始化檔案傳輸請求
bool TfrmFileTransfer::InitiateFileTransfer(TCustomWinSocket *CommandSocket, TCustomWinSocket
*DataSocket,const AnsiString& FilePath, TProgressBar* ProgressBar)
{
    if (CommandSocket == NULL  || CommandSocket->Connected == false)
    {
//        ShowMessage("錯誤：指令 Socket 未連線。");
        return false;
    }

    if (DataSocket == NULL || DataSocket->Connected == false)
    {
//        ShowMessage("錯誤：檔案資料 Socket 未連線。");
        return false;
    }

    if (!FileExists(FilePath))
    {
//        ShowMessage("錯誤：檔案 '" + FilePath + "' 不存在。");
        return false;
    }

    // 使用 TFileStream 來安全地獲取檔案大小
    TFileStream *fs = new TFileStream(FilePath, fmOpenRead | fmShareDenyWrite);
    __int64 fileSize = 0;
    try
    {
        fileSize = fs->Size;
    }
    __finally
    {
        delete fs;
    }

    // 從完整路徑中提取檔案名稱
    AnsiString fileName = ExtractFileName(FilePath);

    // 格式化指令: @1132,2,檔名,檔案大小#
    AnsiString command = AnsiString().sprintf("@1132,2,%s,%lld#", fileName.c_str(),fileSize);

    // 儲存檔案路徑和 "資料 Socket"，以便在收到確認後使用
    m_FileToSendPath = FilePath;
    m_DataSocket = DataSocket;

    // 透過指令 Socket發送指令
    CommandSocket->SendText(command);

    // 初始化進度條
    if (ProgressBar)
    {
        ProgressBar->Max = fileSize;
        ProgressBar->Position = 0;
    }

//    ShowMessage("已透過指令 Socket 發送檔案傳輸請求: " + fileName);
    return true;
}
//---------------------------------------------------------------------------
// 此函式只處理指令
void TfrmFileTransfer::OnCommandSocketRead(TCustomWinSocket *Socket, TProgressBar* ProgressBar, AnsiString asBuffer, bool bRead)
{
//    if(bRead==true)
//        Sleep(20);
//    int dataSize = Socket->ReceiveLength();
//    if(bRead==true)
//        if (dataSize <= 0) return;

//    std::vector<char> buffer(dataSize + 1, 0); // 額外一個位元組確保字串結尾是 null
//    int bytesRead = Socket->ReceiveBuf(&buffer[0], dataSize);
//    if(bRead==true)
//    {
//        if (bytesRead > 0)
//        {
//            // 指令 Socket 收到的永遠是指令，直接交給 ProcessCommand 處理
//    //        AnsiString receivedText = AnsiString(&buffer[0]);
//            ProcessCommand(Socket, asBuffer, ProgressBar);
//        }
//    }
//    else
    {
        ProcessCommand(Socket, asBuffer, ProgressBar);
    }
}
//---------------------------------------------------------------------------
// 此函式處理檔案資料
void TfrmFileTransfer::OnDataSocketRead(TCustomWinSocket *Socket, TProgressBar* ProgressBar)
{
    int dataSize = Socket->ReceiveLength();
    if (dataSize <= 0) return;

    std::vector<char> buffer(dataSize);
    int bytesRead = Socket->ReceiveBuf(&buffer[0], dataSize);

    if (bytesRead > 0)
    {
        // 檔案 Socket 收到的是檔案內容，直接交給 ReceiveFileDataChunk 處理
        ReceiveFileDataChunk(&buffer[0], bytesRead, ProgressBar);
    }
}
//---------------------------------------------------------------------------
// @brief (接收方) 處理 Socket 讀取事件的核心函式
//void TForm2::OnSocketRead(TCustomWinSocket *Socket, TProgressBar* ProgressBar)
//{
//    // 獲取 Socket 中可讀取的數據量
//    int dataSize = Socket->ReceiveLength();
//    if (dataSize <= 0) return;
//
//    // 建立一個足夠大的緩衝區來接收數據
//    std::vector<char> buffer(dataSize);
//
//    // 從 Socket 讀取數據到緩衝區
//    int bytesRead = Socket->ReceiveBuf(&buffer[0], dataSize);
//
//    if (bytesRead > 0)
//    {
//        // 核心邏輯：判斷目前是否處於接收檔案的狀態
//        if (m_IsReceivingFile)
//        {
//         // 如果是，則將收到的數據視為檔案內容進行處理
//         ReceiveFileDataChunk(Socket, &buffer[0], bytesRead, ProgressBar);
//        }
//        else
//        {
//         // 如果不是，則將收到的數據視為指令進行處理
//         // 將 char 緩衝區轉換為 AnsiString，並確保以 null 結尾
//         AnsiString receivedText = AnsiString(&buffer[0], bytesRead);
//         ProcessCommand(Socket, receivedText, ProgressBar);
//        }
//    }
//}
//---------------------------------------------------------------------------
//@brief (接收方) 處理收到的指令
void TfrmFileTransfer::ProcessCommand(TCustomWinSocket *Socket, const AnsiString& Command,
                            TProgressBar* ProgressBar)
{
    // 檢查是否為檔案傳輸請求指令: @1132,2,檔名,大小#
    if (Command.Pos("@1132") != 0)
    {
        if (Command.Pos("@1132,2,") == 1)
        {
            AnsiString temp = Command;
            temp.Delete(temp.Length(), 1);
            temp.Delete(1, 1);

            TStringList *params = new TStringList();
            try
            {
    //            ExtractStrings(AnsiString(",").c_str(), "", temp.c_str(), params);
                params->CommaText = temp;
                if (params->Count == 4)
                {
                    // 在開始新的接收任務前，先呼叫 CancelTransfer
                    // 這會關閉任何可能由上一次失敗傳輸遺留下來的、未關閉的檔案資料流
                    // 從而解決 EFCreateError 的問題。
                    CancelTransfer();
                    m_IncomingFileName = params->Strings[2];
                    m_ExpectedFileSize = StrToInt64(params->Strings[3]);
                    m_TotalBytesReceived = 0;

                    // 準備檔案路徑
                    AnsiString fullPath = m_SaveDirectory + m_IncomingFileName;
                    // 先刪除可能存在的舊檔案
                    if (FileExists(fullPath))
                    {
                        DeleteFile(fullPath);
                    }
                    m_FileStream = new TFileStream(fullPath, fmCreate);

                    if (ProgressBar)
                    {
                        ProgressBar->Max = m_ExpectedFileSize;
                        ProgressBar->Position = 0;
                    }
    //                ShowMessage("收到檔案請求: " + m_IncomingFileName + ", 大小: " + AnsiString
    //                (m_ExpectedFileSize) + " bytes.\n準備從檔案 Socket 接收...");

                    // 回傳確認指令，告訴對方可以開始傳送檔案了
                    Socket->SendText("@1132,1,1#");
                }
            }
            __finally
            {
                delete params;
            }
        }
        // 檢查是否為對方準備好的確認指令: @1132,1,1#
        else if (Command.Pos("@1132,1") !=0)
        {
    //        ShowMessage("對方已準備好，開始透過檔案 Socket 傳送檔案: " + m_FileToSendPath);
            if (!m_FileToSendPath.IsEmpty())
            {
                // 透過 StartActualFileSend
                StartActualFileSend(ProgressBar);
            }
        }
    }
}
//---------------------------------------------------------------------------
//@brief (接收方) 接收並寫入檔案數據塊
void TfrmFileTransfer::ReceiveFileDataChunk(char* buffer, int dataSize, TProgressBar* ProgressBar)
{
    // 檔案流應該在 ProcessCommand 收到請求時就已經建立好了
    if (m_FileStream == NULL)
    {
        // 理論上不應該發生，但作為防呆
//        ShowMessage("錯誤：檔案流未準備好，無法接收檔案資料。");
        return;
    }
    // ========== 修改開始 ==========
    // 使用 try...catch 來捕捉寫入檔案時可能發生的錯誤
    try
    {
        m_FileStream->Write(buffer, dataSize);
        m_TotalBytesReceived += dataSize;

        if (ProgressBar)
        {
            ProgressBar->Position = m_TotalBytesReceived;
        }

        // 檢查是否已接收完所有資料
        if (m_TotalBytesReceived >= m_ExpectedFileSize)
        {
//            ShowMessage("檔案 '" + m_IncomingFileName + "' 接收完成！");

            // 成功完成，關閉檔案並重設狀態
            // 這部分邏輯移到 CancelTransfer 中，此處直接呼叫
            CancelTransfer();
        }
    }
    catch (Exception &E)
    {
        // 如果寫入檔案時發生錯誤
//        ShowMessage("寫入檔案時發生錯誤: " + E.Message);
        // 呼叫 CancelTransfer 來清理失敗的傳輸
        CancelTransfer();
    }
    // ========== 修改結束 ==========
}
//---------------------------------------------------------------------------
//@brief (發送方) 實際傳送檔案內容
// 使用成員變數 m_DataSocket 來傳送
void TfrmFileTransfer::StartActualFileSend(TProgressBar* ProgressBar)
{
    if (m_DataSocket == NULL || m_DataSocket->Connected == false)
    {
//        ShowMessage("錯誤：檔案資料 Socket 未連線，無法傳送。");
        m_FileToSendPath = "";
        return;
    }

    TFileStream *fs = new TFileStream(m_FileToSendPath, fmOpenRead | fmShareDenyWrite);
    try
    {
        char buffer[4096];
        int bytesRead;
        int bytesSentInChunk = 0;
        int result = 0;
        DWORD chunkStartTime =0;
        __int64 totalSent = 0;
        bool sendError = false;

        while ((bytesRead = fs->Read(buffer, sizeof(buffer))) > 0)
        {
            bytesSentInChunk = 0;
            // 為傳送單一區塊設置超時，例如 15 秒
            chunkStartTime = GetTickCount();

            // 這個內部迴圈確保 buffer 中的所有資料都被送出
            while (bytesSentInChunk < bytesRead)
            {
                result = m_DataSocket->SendBuf(buffer + bytesSentInChunk, bytesRead - bytesSentInChunk);

                if (result > 0)
                {
                    // 成功送出部分或全部資料，重設超時起始時間
                    bytesSentInChunk += result;
                    chunkStartTime = GetTickCount(); // 有進度就重設計時
                }
                else // result <= 0，可能發生錯誤或暫時阻塞
                {
                    // ========== 處理暫時阻塞==========

                    // 1. 讓出 CPU，處理 UI 和其他網路事件。
                    //    這非常重要，它讓 Socket 元件有機會在背景清空傳送緩衝區。
                    Application->ProcessMessages();

                    // 2. 檢查 Socket 是否在處理過程中斷線了。
                    //    如果真的斷線，就中止傳輸。
                    if (m_DataSocket->Connected == false)
                    {
//                        ShowMessage("傳送檔案資料失敗，連線已中斷。");
                        sendError = true;
                        break;
                    }

                    // 3. 檢查是否超時，防止在網路極度壅塞或對方無回應時無限迴圈。
                    //    如果超過 15 秒還無法送出一個區塊的任何資料，就判斷為超時。
                    if ((GetTickCount() - chunkStartTime) > 15000) // 15 秒超時
                    {
//                         ShowMessage("傳送檔案資料逾時，連線可能已無回應。");
                         sendError = true;
                         break;
                    }
                    // 如果沒有斷線且沒有超時，迴圈會繼續，在下一次迭代中嘗試再次傳送剩餘的資料。
                }
            }

            if(sendError)
            {
                break; // 如果發生錯誤，跳出外部讀取檔案的迴圈
            }

            totalSent += bytesRead;
            if (ProgressBar)
            {
                ProgressBar->Position = totalSent;
            }

            // 在每送完一個大區塊後，也處理一下訊息，讓進度條能即時更新
            Application->ProcessMessages();
        }
    }
    __finally
    {
        delete fs;
        m_FileToSendPath = "";
        m_DataSocket = NULL;
    }
}
//---------------------------------------------------------------------------
/**
 * @brief CancelTransfer 函式
 *        這個函式負責在傳輸中斷或失敗時，釋放檔案控制並重設相關狀態。
 */
void TfrmFileTransfer::CancelTransfer()
{
    // 如果檔案資料流物件還存在，表示有未完成的傳輸
    if (m_FileStream)
    {
        // 刪除 (關閉) 檔案資料流，釋放對檔案的鎖定
        delete m_FileStream;
        m_FileStream = NULL;
    }
    // 重設狀態變數，為下一次傳輸做準備
    m_IncomingFileName = "";
    m_ExpectedFileSize = 0;
    m_TotalBytesReceived = 0;
    m_FileToSendPath = "";
}
// ================================
