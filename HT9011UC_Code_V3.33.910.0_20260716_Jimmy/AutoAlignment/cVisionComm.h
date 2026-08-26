//---------------------------------------------------------------------------
#include <vcl.h>
#include <ScktComp.hpp>
#ifndef cVisionCommH
#define cVisionCommH
//---------------------------------------------------------------------------
class TVisionComm
{
    private:
        HTimer tCCDChangeSetupFile;
        int iCount;
    public:
        __fastcall TVisionComm(AnsiString asInputCameraID);
        __fastcall ~ TVisionComm();

        TClientSocket *clntsckt;
        TMemo *memoCommLog;

        AnsiString asStatus;    //Vision Live?
        AnsiString sCurrentGF;  //Vision 當前工作檔
        double dVisionVer;      //Vision 版本

        bool bGetSE9;           //通訊取得 Reset 狀態
        bool bGetVR;            //通訊取得 Vision 版本狀態
        bool bGetGF;            //通訊取得 Vision 工作檔狀態
        bool bGetLF;            //通訊設定 Vision 工作檔狀態
        bool bGetCF;            //通訊產生 Vision 工作檔資料狀態
        bool bGetST;            //通訊設定 Vision 時間狀態

        void Connect();

        void ChangeSetupFile();
        void ConnectOkInitial();
        int iConnectionOkTask;
        void ProcessGetData();

        //參數
        AnsiString asIP;
        int iPort;
        AnsiString asCameraID;      //0: In Top, 1: In Bottom, 2: Out Top, 3: Out Bottom
        //變數
        TStringList *SocketReceiveList;
        bool bEnableVision;
        AnsiString sSendCMD;

        int iResult;            //0:Wait Result ; 1:Complete ; 2:Can not find ; 3:Type Different 4:Receive Vision Error

        int dCenterOffsetPixelX;    //Vision 計算出來的影像中心的偏差 X 單位：Pixel
        int dCenterOffsetPixelY;    //Vision 計算出來的影像中心的偏差 Y 單位：Pixel

        void SendCommand(AnsiString Msg, AnsiString Msg2="");
        void AddCommandLog(AnsiString Str);
};
#endif
