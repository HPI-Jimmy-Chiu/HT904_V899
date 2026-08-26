//---------------------------------------------------------------------------
#ifndef cMyDNM100UDH
#define cMyDNM100UDH
#include "cMyNUDN1.h"
//---------------------------------------------------------------------------
//KEYC NUDN1
const WORD NUDN1ID      =0x66;  //Class ID : NUDN1 ID

const byte SETR         =0x0E;  //Service Code : Setting Read
const byte SETW         =0x10;  //Service Code : Setting Write

//Instance ID N*10+1
const byte GETOUTPUT    =0x68;  //Attribute ID : OutPut
const byte GETCURRECT   =0x69;  //Attribute ID : Get Currect Value
//Instance ID N*10+2
const byte SETVALUE     =0x65;  //Attribute ID : Setting Value

//---------------------------------------------------------------------------
//DNM100UD  (泓格 DeviceNet Master PCI Board)
class PACKAGE TMyDNM100UD_Card : public TComponent
{
    private:

    protected:
        TTimer *tmrReadInputData;
        void __fastcall tmrReadInputDataTimer(TObject *Sender);
        //Boards
        unsigned char iActivedBoardNo;          //泓格 DNM100UD Board No.
        unsigned char TotalBoards;
        unsigned char iBoardIDList[16];
        unsigned char iCurrectBaudRateType;
        unsigned char iBaudRateType;            //0:500k ; 1:250k ; 2:125k  (bit/s)

        //Devices
        unsigned short iTotalDevices;
        unsigned char  iDesMACIDList    [16];   //所有 NUDN1 站別清單
        unsigned char  iAmplifierList   [16];   //所有 NUDN1 接著 FS-N12N 數量
        unsigned char  iType;                   //NUDN1 IO 通訊模式

        //EX
        int MyDNM100_SendExplicitMSG_W(BYTE DesMACID,BYTE ServiceID,WORD ClassID,WORD InstanceID,WORD DataLen,BYTE *DATA,WORD GetDataLen);
        bool bUseRespValueDelay;     //使用 RespValueDelay 韌體版本V260需要延遲，新韌體版本V270可以不用延遲
        bool bActivedBoard;
        int iDNM100UDTask;
        bool bInitialOK;        //Main Form 初始化完成在 True
        bool bInitialCardOK;    //Initail1() 正常完成
        bool bHasErr;
        AnsiString ErrMess;
        AnsiString SysMess;

    public:
        __fastcall TMyDNM100UD_Card(unsigned short _iTotalDevices,unsigned char *_iDesMACIDList,unsigned char *_iAmplifierList);
        __fastcall ~TMyDNM100UD_Card();
        //初始化
        int Initail();
        void TaskClear();

        TMyNUDN1 *NUDN1Item[16];
        AnsiString asDLLVer;
        AnsiString asFirmwareVer;
        bool bCanBusSeach;
        AnsiString  asGetBaudRate();
        bool DNM100IsShutDownNeedPowerOffPC;    //DNM100 當機了需要關閉電腦再打開(Reset 沒有用)
        int DNM100InitailCount;
        //Use Explicit Messaging iAmplifierNo : 1~16
        //==>            iTestGetIO
        int iTestGetIO(BYTE DesMACID,byte iAmplifierNo);                        //Sensor 輸出的 IO (測試使用。讀取 IO 要用 I/O Communication 比較快)
        int iTestGetValue(BYTE DesMACID,byte iAmplifierNo);                     //Sensor 目前數值  (測試使用。讀取數值要用 I/O Communication 比較快)
        int iTestGetIO_Value(BYTE DesMACID,byte iAmplifierNo);                  //IO 閥值目前數值  (測試使用)
        int iTestSetIO_Value(BYTE DesMACID,byte iAmplifierNo,int iSetValue);    //IO 閥值設定      (測試使用)

        //....後面可依據功能需求增加
        //..
        //.
        //<==
        //Use Explicit Messaging

        AnsiString  asSearchErrCodeMsg(int i);

    __published:

        __property bool         HasError        ={read=bHasErr,         write=bHasErr,          default=false};   //是否有錯誤
        __property bool         InitialCardOK   ={read=bInitialCardOK,  write=bInitialCardOK,   default=false};
        __property AnsiString   ErrorMessage    ={read=ErrMess,         write=ErrMess,          default=NULL};
        __property AnsiString   SystemMessage   ={read=SysMess,         write=SysMess,          default=NULL};    //錯誤訊息
        __property AnsiString   DLLVer          ={read=asDLLVer,        write=asDLLVer,         default=NULL};
        __property AnsiString   FirmwareVer     ={read=asFirmwareVer,   write=asFirmwareVer,    default=NULL};

        __property bool InitialOK              ={read=bInitialOK,write=bInitialOK,default=false};        //Main Form 好了就 True

        __property unsigned char iTotalBoards   ={read=TotalBoards,default=0};
};
//---------------------------------------------------------------------------
union UNIONWORD
{
    int WHOLE_WORD;
    struct
    {
        char Lo_BYTE;
        char Hi_BYTE;
    }BYTES;
};
//---------------------------------------------------------------------------
#endif

