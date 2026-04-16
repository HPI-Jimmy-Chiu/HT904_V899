//---------------------------------------------------------------------------
#ifndef MyCCLinkBoardH
#define MyCCLinkBoardH
//#include "HTimer.h"
#include "myTimer.h"
//---------------------------------------------------------------------------
typedef struct
{
    bool  bHasInstall;       //是否有安裝
    bool  bOnOff[16];        //是否有安裝
    int   iCurrValue[16];    //目前的數值
    int   iSetValue[16];     //設定的門檻值
    int   iSensorNo;         //控制幾個Sensor
    short wwStart;
    short ryStart;
}MyCCLinkNCUL1;
//---------------------------------------------------------------------------
class PACKAGE TMyCCLinkBoard : public TComponent
{
    private :
        AnsiString ErrMess;
        int  iConnectModule;        //連接了幾個模組
        bool bHasErr;
        bool bStatus;
        bool bKeyLock;
        long lPath;                 //連線位置
        unsigned char  iData[32];   //收到的資料
        int iSensorNo;
        bool MemoryNotEnough;        //Steven 20140330 : 記憶體不足處理
    protected:  // User declarations
        int   Result;
        short StaNo;
        short NetNo;
        unsigned char Data[32];
        TQPF_Timer CCLinkDelay;

        int  DoCCLinkSWQuest(int iDevNo);
        bool DoRYOnOff(int Index, int iRyDevNo, bool bOnOff);  // 開始或結束寫入iRyDevNo為指定的記憶體位址
        bool DoWriteWW(int Index, short sWwDevNo, short Value);
        int  DoReadWr02(int Index); //讀值

        bool ConnectBoard();        //開始連線
        void DisConnectBoard();     //結束連線
        int  GetSensorNo(int Value);//檢查每個NU-CL1裝了幾個Sensor

        int CCLinkHexStrToInt(AnsiString str);
        void __fastcall CheckBoardStatus();             //檢查連線狀態
        void __fastcall SetSensorKeyLock(bool Value);
        void SetProcessMemory();        //Steven 20140330 : 記憶體不足處理
        AnsiString CmdResponseList[12];
        AnsiString MyErrMes(int iDevNo, int iResult);    //取得某變數時發生的錯誤
    public:
        __fastcall TMyCCLinkBoard::TMyCCLinkBoard(TComponent* Owner);
        __fastcall TMyCCLinkBoard::~TMyCCLinkBoard();
        int  iCCLinkBoardTask;
        int  iReadSetValueTask;
        int  iDoSetValueTask;
        int  iDoSetMultiValueTask;
        bool DoConnectBoard();
        bool DoReadCurrValue();
        bool DoReadSetValue();
        bool DoReadOnOff();
        bool DoCheckWr0000();
        bool DoSetValue();          //針對單一Sensor設定值
        bool DoSetMultiValue();     //設定整排的Sensor
        void ResetBoard();          //重設卡片
        short sSetNu;       //要被設定的NU-CL1
        short sSetSen;      //要被設定的Sensor
        int   iSetValue;    //要被設定的值
        int   iSetMultiValue[16];   //要被設定的值
        void ClearData();
        MyCCLinkNCUL1 MyNCUL[10];
    __published:
        __property bool       Status          ={read=CheckBoardStatus, write=bStatus,          default=false};   //連線狀態
        __property bool       HasError        ={read=bHasErr,          write=bHasErr,          default=false};   //是否有錯誤
        __property bool       SensorKeyLock   ={read=bKeyLock,         write=SetSensorKeyLock, default=false};   //按鍵鎖
        __property AnsiString ErrorMessage    ={read=ErrMess,          write=ErrMess,          default=NULL};    //錯誤訊息
        __property int        ConnectModuleNo ={read=iConnectModule,   write=iConnectModule,   default=0};       //連接的模組數
        __property long       Path            ={read=lPath,            write=lPath,            default=81};      //系統位置
};

#endif
