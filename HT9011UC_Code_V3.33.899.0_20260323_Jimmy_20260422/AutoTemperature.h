//---------------------------------------------------------------------------

#ifndef AutoTemperatureH
#define AutoTemperatureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include "SPComm.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <vector>
using namespace std;
#define ATC_MaxGroup 20
//---------------------------------------------------------------------------
typedef struct {
    int iThermoCtrlType;
    int iCheckIntervalTime;
    int iCalibrationRange;
    double dCalibrationRange[3];
    int iReadScanTime;
    int iOffsetLimit;
    int iSingleOffset;
    int iDisplayColumn;
    int iOffsetMethod;
} ACT_DATA;

enum eACTThermoType{Type_Agilent34970A=0, Type_DeltaDTB4824=1};  //Steven 20210510 : 自動K溫使用DTB4824
//---------------------------------------------------------------------------
class PACKAGE TMyATPanel : public TComponent
{
    private:    // User declarations
        int       iStartX;
        int       iStartY;
        int       _iOffsetWidth;
    public:
        TGroupBox *gbMeasurePoint;
        TComboBox *cbChannel;
        TCheckBox *cbEnable;
        TPanel    *pnlTemp;
        TImage    *ImgOriginalOffset;
        int       _Index;
        int       _ChannelNo;
        int       _Group;
        bool      _Enable;
        AnsiString _GroupName;
        double    fTemperatureData;
        __fastcall TMyATPanel(TComponent* Owner, int index, AnsiString Name);
        __fastcall TMyATPanel(TComponent* Owner, int index, AnsiString Name, int iGroupNum, AnsiString GroupName);
        //iGroupNum = 對應到的實體溫控器位置

        __fastcall ~TMyATPanel();
        void __fastcall SetParent               (TWinControl *Sender);
        void __fastcall gbMeasurePointMouseUp   (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall gbMeasurePointMouseDown (TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall gbMeasurePointMouseMove (TObject *Sender, TShiftState Shift, int X, int Y);
        void __fastcall cbChannelChange         (TObject *Sender);
        void __fastcall ImageShowOriginalOffset (double low, double middle, double high);
        void __fastcall ImageShowCurrentOffset(int base, double dCurrentOffset);
};
//---------------------------------------------------------------------------
class TACTForm : public TForm
{
__published:    // IDE-managed Components
    TComm *ACTCom;
    TPageControl *pcACT;
    TTabSheet *tsTemperature;
    TTabSheet *tsSetup;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TComboBox *cbBaudRate;
    TComboBox *cbByteSize;
    TComboBox *cbStopBit;
    TComboBox *cbParity;
    TComboBox *cbDevice;
    TButton *btnUpdate;
    TPanel *pnlData;
    TPanel *pnlBase01;
    TPanel *pnlBase02;
    TPanel *pnlBase03;
    TPanel *pnlBaseTemp01;
    TPanel *pnlBaseTemp02;
    TPanel *pnlBaseTemp03;
    TCheckBox *cbBase01;
    TCheckBox *cbBase02;
    TCheckBox *cbBase03;
    TBitBtn *btnAutoStart;
    TTimer *TimerACT;
    TGroupBox *gbACTSetting;
    TEdit *edtCheckIntervalTime;
    TEdit *edtCalibrationRange;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TEdit *edtReadScanTime;
    TLabel *Label10;
    TPanel *Panel3;
    TMemo *MemoLog;
    TEdit *edtOffsetLimit;
    TLabel *Label11;
    TScrollBox *ScrollBox1;
    TGroupBox *gbMeasurePoint;
    TPanel *pnlTemp;
    TCheckBox *cbUse01;
    TComboBox *cbMeasurePoint;
    TBitBtn *btnDutOnOff;
    TGroupBox *gbDisplay;
    TLabel *Label12;
    TEdit *edColumns;
    TBitBtn *btShowPos;
    TImage *ImgOriginalOffset;
    TLabel *labTimer;
    TMemo *MemoOffset;
    TPanel *palSaveLog;
    TLabel *Label13;
    TEdit *edSingleOffset;
    TLabel *Label14;
    TComboBox *cbbThermoCtrlType;
    TRadioGroup *rgOffsetMethod;
    TCheckBox *chkAutoStart;
    TEdit *edtAutoStartHr;
    TEdit *edtAutoStartMin;
    TLabel *Label15;
    TLabel *Label16;
    TEdit *edtRange1;
    TEdit *edtRange2;
    TEdit *edtRange3;
    void __fastcall ACTComReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnUpdateClick(TObject *Sender);
    void __fastcall btnAutoStartClick(TObject *Sender);
    void __fastcall TimerACTTimer(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall btnDutOnOffClick(TObject *Sender);
    void __fastcall btShowPosClick(TObject *Sender);
    void __fastcall palSaveLogClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall edtAutoStartHrMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtAutoStartMinMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtRange1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edtCheckIntervalTimeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
    bool SaveCommData( const TComm* Comm );
    bool LoadCommData(TComm* Comm );
    void ShowDefaultPos(bool bHide=false);
    AnsiString CommName;
public:     // User declarations
    __fastcall TACTForm(TComponent* Owner);

    bool CloseCommPort();
    bool OpenCommPort();
    void SendCommand(  AnsiString sData  );
    void ShowCommData( AnsiString sType , AnsiString sData  );
    void ChangeStartState();
    void SaveACTData( ACT_DATA& Data );
    void LoadACTData( ACT_DATA& Data );
    void UpdateTemperatureData();
    bool bCommConnect;
    bool bAutoStart;

    ACT_DATA ACTData;

    TCheckBox*  _cbBase[3];
    TPanel*     _pnlBaseTemp[3];
    AnsiString  FilePath;
    int iTotalGroupCount;
    int iGroupSet[ATC_MaxGroup];
    int iGroupCount[ATC_MaxGroup];  //各群組內的數量
    double dGroupMax[ATC_MaxGroup];
    double dGroupMin[ATC_MaxGroup];
    double dGroupSum[ATC_MaxGroup];
    double dGroupAvg[ATC_MaxGroup];
    bool bBaseFinish[5];
    vector< TMyATPanel* > myATPal;
    void DTB4824_ReadPV(int Addr);
    AnsiString SENDBUFF_DTB;
    AnsiString READBUFF_DTB;
    int iReceiveData;
    int iAddr;
    TQPF_Timer tReadTimeOut;
    TQPF_Timer tTempScanTimer;
    TQPF_Timer tCheckIntervalTimer;
    TQPF_Timer tBaseTempBlinkTimer;
};
//---------------------------------------------------------------------------
extern PACKAGE TACTForm *ACTForm;
//---------------------------------------------------------------------------
#endif
