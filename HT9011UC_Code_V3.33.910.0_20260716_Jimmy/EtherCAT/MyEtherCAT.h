//---------------------------------------------------------------------------
#ifndef MyEtherCATH
#define MyEtherCATH
//---------------------------------------------------------------------------
#include "MyNUEC1.h"
#define MaxNUEC1 16
#define MaxFSN12 16
class PACKAGE TMyEtherCAT : public TComponent
{
    private:

    protected:
        TTimer *tmrReadInputData;
        void __fastcall tmrReadInputDataTimer(TObject *Sender);

        unsigned char iRing;                                        //I/O 使用 Ring1，Ring0 保留給馬達

        //Devices
        unsigned short iTotalDevices;                               //共接了幾組 NU-EC1
        unsigned short iDesMACIDList [MaxNUEC1];                    //NU-EC1 各別 SlaveID
        unsigned char iAmplifierList [MaxFSN12];                    //NU-EC1 個別接的 Amplifier 數量

        //EX
        int iEtherCATTask;
        bool bInitialOK;        //Main Form 初始化完成在 True
        bool bHasErr;
        AnsiString ErrMess;
        AnsiString SysMess;
        bool bReOpenPCI1203;

    public:
        __fastcall TMyEtherCAT(unsigned short _iTotalDevices,unsigned char *_iDesMACIDList,unsigned char *_iAmplifierList);
        __fastcall ~TMyEtherCAT();
        void TaskClear();             //初始化
        TMyNUEC1 *NUEC1Item[MaxNUEC1];
        void Start();
        void Pause();
        AnsiString GetEtherCATErrDesc(int ret);                                 //Eastsun 20260710整合
    __published:

        __property bool         HasError        ={read=bHasErr,         write=bHasErr,          default=false};   //是否有錯誤
        __property AnsiString   ErrorMessage    ={read=ErrMess,         write=ErrMess,          default=NULL};
        __property AnsiString   SystemMessage   ={read=SysMess,         write=SysMess,          default=NULL};    //錯誤訊息
        __property bool InitialOK              ={read=bInitialOK,write=bInitialOK,default=false};        //Main Form 好了就 True
};
//---------------------------------------------------------------------------
extern PACKAGE TMyEtherCAT *MyEtherCAT;
extern int CheckPCI_EtherCatState();                                            //Sam 20230811 : 新增 EtherCAT 檢查功能
extern int OpenEtherCatMastCard();                                              //Sam 20230707 : EtherCAT Shuttle sensor
extern bool INSTALL_ETHETCAT();                                                 //Sam 20230210 : 新增 VacuumUnit 通訊模組
#endif
