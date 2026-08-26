//---------------------------------------------------------------------------

#ifndef OmronEJ1NH
#define OmronEJ1NH
//---------------------------------------------------------------------------
#include "MyOmronPanel.h"
//#include "HTimer.h"

#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include "PComm.h"
#include "MachineType.h"
#include "SPComm.hpp"
#include "ALed.hpp"
#include "MyLed.h"
//---------------------------------------------------------------------------
const int TOTAL_OMRON_UNIT=8;//kevin 20130520   4;

enum eResponseCode{rc0000=0, rc1001, rc1002, rc1101, rc1003, rc1100,
                   rc110B,   rc2201, rc2202, rc2203, rc0402, rc3003,
                   rc7011,   rc7012, rc7013, rc7014, rc7015, rc7016,
                   rc7020,   rc7021, rc7030, rc7041, rc7042, rcTotalCount};
//enum eEndCode{ec00=0, ec0F, ec10, ec11, ec12, ec13, ec14, ec16, ec18, ecTotal};   //Steven 20140313 : 換到MachineType.h
//---------------------------------------------------------------------------
class TfOmron : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel7;
    TPanel *Panel1;
    TGroupBox *GroupBox2;
    TScrollBox *ScrollBox1;
    TGroupBox *GroupBox;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TEdit *edSV;
    TPanel *Panel9;
    TMemo *meLogs;
    TPageControl *PageControl1;
    TTabSheet *tsAuto;
    TTabSheet *tsManual;
    TLabel *labSTX;
    TGroupBox *gbBccArea;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *labETX;
    TComboBox *cbUnitAddress;
    TGroupBox *gbFinsMiniCommand;
    TLabel *Label8;
    TLabel *Label9;
    TComboBox *cbMrcSrc;
    TPageControl *pcMrcSrc;
    TTabSheet *tsReadVariable;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label11;
    TEdit *Edit7;
    TEdit *Edit8;
    TEdit *Edit9;
    TComboBox *ComboBox5;
    TTabSheet *tbWriteVariable;
    TLabel *Label28;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label12;
    TLabel *Label38;
    TLabel *Label40;
    TEdit *Edit6;
    TEdit *Edit10;
    TEdit *Edit11;
    TComboBox *ComboBox3;
    TTabSheet *tsCompositeRead;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label27;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label24;
    TEdit *Edit4;
    TEdit *Edit12;
    TEdit *Edit13;
    TEdit *Edit14;
    TComboBox *ComboBox6;
    TEdit *Edit15;
    TComboBox *ComboBox7;
    TEdit *Edit16;
    TComboBox *ComboBox8;
    TEdit *Edit17;
    TComboBox *ComboBox9;
    TEdit *Edit18;
    TTabSheet *tsCompositeWrite;
    TLabel *Label21;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label29;
    TLabel *Label37;
    TEdit *Edit3;
    TEdit *Edit19;
    TEdit *Edit20;
    TEdit *Edit21;
    TComboBox *ComboBox1;
    TEdit *Edit22;
    TComboBox *ComboBox2;
    TEdit *Edit23;
    TComboBox *ComboBox10;
    TEdit *Edit24;
    TComboBox *ComboBox11;
    TEdit *Edit25;
    TEdit *Edit26;
    TEdit *Edit27;
    TEdit *Edit28;
    TEdit *Edit29;
    TTabSheet *tsCompositeReadReg;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label39;
    TLabel *Label41;
    TEdit *Edit5;
    TEdit *Edit30;
    TEdit *Edit31;
    TEdit *Edit32;
    TComboBox *ComboBox12;
    TEdit *Edit33;
    TComboBox *ComboBox13;
    TEdit *Edit34;
    TComboBox *ComboBox14;
    TEdit *Edit35;
    TComboBox *ComboBox15;
    TEdit *Edit36;
    TTabSheet *tsEchoTest;
    TLabel *Label6;
    TEdit *Edit1;
    TTabSheet *tsOperationCommand;
    TLabel *Label7;
    TLabel *Label10;
    TComboBox *ComboBox4;
    TEdit *Edit2;
    TButton *btSend;
    TButton *btGetModuler;
    TButton *btRunStop;
    TRadioGroup *rgSensorType;
    TButton *btSetThermoType;
    TLabel *Label26;
    TRadioGroup *rgRunStop;
    TPanel *Panel6;
    TPanel *Panel8;
    TButton *Button1;
    TTimer *Timer2;
    TTabSheet *tsSimple;
    TButton *btSetSV;
    TButton *btResetCOM;
    TButton *Button3;
    TPanel *Panel10;
    TGroupBox *GroupBox1;
    TEdit *Edit37;
    TImage *Image1;
    TCheckBox *cbEnableAT;
    TGroupBox *gbAT;
    TButton *btAT;
    TButton *btATOff;
    TCheckBox *cb40PercentAT;
    TButton *btAT1by1;
    TButton *btRT;
    TButton *btRT1by1;
    TButton *btRTOff;
    TTabSheet *tsRecord;
    TMemo *Memo1;
    TPanel *Panel11;
    TCheckBox *cbGetRecord;
    TButton *btSaveData;
    TComm *CommOmron;
    TGroupBox *grpStatus;
    TMyLed *ledEJ1N1;
    TMyLed *ledEJ1N2;
    TMyLed *ledEJ1N3;
    TMyLed *ledEJ1N4;
    TMyLed *ledEJ1N5;
    TMyLed *ledEJ1N6;
    TMyLed *ledEJ1N7;
    TMyLed *ledEJ1N8;
    TLabel *Label25;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TLabel *Label48;
    TLabel *Label49;
    TLabel *Label50;
    TLabel *Label51;
    TLabel *Label52;
    void __fastcall btSendClick(TObject *Sender);
    void __fastcall cbMrcSrcChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btGetModulerClick(TObject *Sender);
    void __fastcall btSetSVClick(TObject *Sender);
    void __fastcall btATClick(TObject *Sender);
    void __fastcall btRunStopClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btSetThermoTypeClick(TObject *Sender);
    void __fastcall btResetCOMClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall btATOffClick(TObject *Sender);
    void __fastcall btAT1by1Click(TObject *Sender);
    void __fastcall btRTClick(TObject *Sender);
    void __fastcall btRT1by1Click(TObject *Sender);
    void __fastcall btRTOffClick(TObject *Sender);
    void __fastcall btSaveDataClick(TObject *Sender);
    void __fastcall CommOmronReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall Edit1Click(TObject *Sender);
private:    // User declarations
    TQPF_Timer OmronDelay;
    void Get4in1SVValue(int Chanel, AnsiString Command);
    void GetPVValue(int Chanel, AnsiString Command);
    AnsiString SendBufferASCII;
    AnsiString SendBufferTEXT;
    void SendCommand(AnsiString Command, AnsiString Func);
    void WriteInfoToMemo(AnsiString asInfo);
    void SetPanelPos(bool bInitial=true);
    void EnableButton();
    bool __fastcall CheckEndAndResponseCode(AnsiString Buffer);
    void SetButton(int Function);
    bool bSaveData;
    TMyLed *LedEJ1N[8];
    AnsiString LogMsg;

public:     // User declarations
    __fastcall TfOmron(TComponent* Owner);
    AnsiString sResponseCode[2][rcTotalCount];
    AnsiString sEndCode[2][ecTotal];
    bool bSendCommand;
    TMyOmronPanel *myPal    [TOTAL_OMRON_UNIT][4];
    double dTempValue       [TOTAL_OMRON_UNIT][4];
    double dSetValue        [TOTAL_OMRON_UNIT][4];
    bool   bATStart         [TOTAL_OMRON_UNIT][4];
    bool   bRunStopStatus   [TOTAL_OMRON_UNIT][4];
    bool   bInputError      [TOTAL_OMRON_UNIT][4];
    bool   bEvent           [TOTAL_OMRON_UNIT][4][3];
    int    bModulType       [TOTAL_OMRON_UNIT]; //0: 未安裝, 2: TC2, 4:TC4
    bool   bReadError; //通訊異常

    bool bGetComponent;
    bool bReadPV;
    bool bReadSV;
    bool bReceivePV[TOTAL_OMRON_UNIT];  //JerryYang 20201123 Omron通訊修正為一問一答
    bool bReceiveSV[TOTAL_OMRON_UNIT];
    bool bRecvData;
    int  iSetSV;
    bool bSetAT;
    bool bAT;
    bool bATOneByOne;
    bool bSetRT;
    bool bRT;
    bool bRTOneByOne;
    bool bRunStop;
    bool bSetThermoType;
    bool bResetCom;
    bool bOmronCommOK;                                                          //AI(appexception-av-log-flood) 20260617 (RogerYang) : CommOmron 是否開啟成功，false 時禁止 WriteCommData，避免 ntdll Access violation 灌爆 log
    bool bGetComponentFinish;

    int iOmronTask;
    int iGetComponentTask;
    int iGetPVTask;
    int iGetSVTask;
    int iSend1CmdTask;
    int iSetThermoTypeTask;
    int iResetComTask;
    int iSetSVTask;
    int iRunStopTask;
    int iATTask;
    int iATOneByOneTask;
    int iRTTask;
    int iRTOneByOneTask;

    bool DoGetComponent();
    bool DoGetPV();
    bool DoGetSV();
    bool DoSetSV();
    bool DoSend1Cmd();
    bool DoSetThermoType();
    bool DoResetCom();
    bool DoRunStop();
    bool DoAT();
    bool DoATOneByOne();
    bool DoRT();
    bool DoRTOneByOne();
    void ClearOmronLog();   //JerryYang 20201123 清除非當月份的Omron通訊log

    void Main232();
    void Timer1Timer();
    bool RS232_OpenDevice();
    void RS232_CloseDevice();
    bool bShow;

    int         iCode;
    int         iCommPort;
    int         bInitComm;
    AnsiString  asTStrTemp;
    AnsiString  asRStr;
    AnsiString  asTStr;
    AnsiString  asBCC;
    AnsiString  asBCCConfirm;
    int         iChanel;
    int         iShowLevel;

    //JimmyChiu 20210923 : Index使用DTME08版加熱器
    void Initial_DTME08();
    void SetBtnParameter(TButton *_btn, int _index);
    void __fastcall btnDTME08Click(TObject *Sender);
};
//---------------------------------------------------------------------------
class TRS232Thread : public TThread
{
    private:
    protected:
        void __fastcall Execute();
    public:
        __fastcall TRS232Thread(bool CreateSuspended);
        void __fastcall TRS232Thread::RS232ThreadProcess(void);
};
extern TRS232Thread *My232Thread;
//---------------------------------------------------------------------------
class TOmronProcessThread : public TThread
{
    private:
    protected:
        void __fastcall Execute();
    public:
        __fastcall TOmronProcessThread(bool CreateSuspended);
        void __fastcall TOmronProcessThread::OmronThreadProcess(void);
};
extern TOmronProcessThread *MyOmronThread;
//---------------------------------------------------------------------------
extern PACKAGE TfOmron *fOmron;
//---------------------------------------------------------------------------
#endif
