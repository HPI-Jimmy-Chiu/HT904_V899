//---------------------------------------------------------------------------

#ifndef myMN200motorH
#define myMN200motorH


#include "HTMotor.h"
#include "MN200.h"
#include "database.h"
const int MAXRing=4;      //Steven 20140828 : 2 --> 4 for XY-Pitch
const int MAXIP=64;
const int MAXPort=4;
//---------------------------------------------------------------------------
class TMyMN200Motor:public HTMotor
{
    private:
        char  cDeviceName[16];
//        short Id ;
        int   iSMCHomeObjectTask;
        int   OldSpeed;
        bool  Open_MN200Card();
        void  MN200SoftLimitEnable(bool bFlag);
        bool  MN200MotHome();
        void  SetEncodeMultiple(int iMultiple);
        void  SetServoAlarm();
        void  SetMN200InType();
        void  GetMN200ErrorMessage(int nErrCode, AnsiString FUNC="");           //Steven 20220520 : 針對MN200增加錯誤訊息的紀錄
        SPEED_PAR MN200SpeedPar;
        int LP, LN;
    protected:
    public:
        __fastcall TMyMN200Motor(int Addr);
        ~TMyMN200Motor();
        virtual int     InitMotor(int IoAddress);
        virtual void    SetSpeed(unsigned int x);
        virtual void    SetInitSpeed(unsigned int x);
        virtual void    SetServoAlarmOn(bool Value);
        virtual void    SetAcc(double a);
        virtual void    SetDec(double a);
        virtual int     ReadPos();
        virtual void    ScanMotorStatus(bool *Led);
        virtual bool    MoveTo(int Tar);
        virtual void    Stop();
        virtual void    DecStop();
        virtual bool    JogP();
        virtual bool    JogN();
        virtual bool    HomeObject();
        virtual void    SetRange(unsigned int a);
        virtual bool    GetAlarm(void);
        virtual bool    HomeFlag(void);

        virtual bool    LinearAxisMoveTo(int iPortID[8],long lPos[8],bool bFlag);
        virtual void    EnableTrigger(bool bFlag,int iMode,long lValue);
        virtual bool    ResetPos(int Pulse);
        virtual bool    MotionDone();
        virtual int     ReadRealPos();
        virtual int     ReadEnCoderRealPos();
        virtual int     SetCommand(int p);
        virtual int     SetPosition(int p);
        virtual void    SetServoOn(bool IsOn);
        virtual void    SetSoftLimit(int iPLimit, int iNLimit);

        //-----------------------------
//        virtual void    SetEnableLatch(bool a);
        virtual bool    MoveToPos(int Tar);
        virtual void    ResetLatch();
        virtual int     GetLatchTotalLen();
        virtual int     GetLatchBuffer(int LatchInputNo, short *AxisCounterNo, long *LatchDataTable);
        virtual int     GetLatchBuffer(int LatchInputNo, unsigned short *AxisCounterNo, long *LatchDataTable) {return 0;};
        virtual bool    GetLatchIOStatus(unsigned short LatchInputNo);
        virtual void    SetFIFOLatchSrc(int AxisCounterNo, int LatchInputNo, int bEnable); //ChungHung 20130625 add 跟原本Motor Enable衝突   Enable-->bEnable

        virtual int  SetGroup(BYTE bGrpNo, BYTE bNumDev, BYTE bDevNo[]);
        virtual int  LineNMove(BYTE bDevNo[], long DevPos[], BYTE bNumDev);
};

//------------
//Isaac 20181212 (Steven) : Baud Rate防呆功能
//=>start
typedef struct
{
    public:
    int RaudRate[MAXIP];
    int Type[MAXIP];
    void MyMNetLine()
    {
        ZeroMemory(RaudRate, sizeof(RaudRate));
        ZeroMemory(Type, sizeof(Type));
    };

}MyMNetLine;
extern MyMNetLine myLine[MAXRing];
void ShowMNetTree(TTreeView *TView);
//<==end
//Isaac 20181212 (Steven) : Baud Rate防呆功能
//PISO-MN200====================================================================
extern DWORD MN_200_ErrorTable[4];                                              //jou 2016-04-29 修正記憶體破壞 MN_200_ErrorTable[2] -> MN_200_ErrorTable[4]
extern void OpenPCI132Card(bool bfirst);
extern void ResetMNet(int iRingNo, AnsiString EngMessage, AnsiString ChtMessage, bool bShowMess);
extern bool GetMN200_Error_Code(int iRing, int iCode, AnsiString *EngStr, AnsiString *ChStr, int iIP=-1);
extern bool MN200_Line_status_OK(int iRing, WORD pD, AnsiString *EngStr, AnsiString *ChStr);
extern bool bResetMNet;                                                         //jou 2012-01-10 避免24V還沒開啟就在重置先達模組
extern bool MNetLog(AnsiString Message);                                        //Steven 20110406
extern int  CheckPCI_L112State();
extern int  CheckPCI_MN200State();
extern int GetAD8Value_HotGenFlow(unsigned int iLineNo, unsigned int iDevNo, unsigned int iChannelNo,  AnsiString asMode);  //KaiChen 20190729 ：Hot Gun Flow
#endif
