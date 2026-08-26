//---------------------------------------------------------------------------

#ifndef OCRH
#define OCRH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SPComm.hpp"
#include <ExtCtrls.hpp>

#include "MachineType.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ScktComp.hpp>
#include <FileCtrl.hpp>    //Steven 20130809 : OK
//------------------------------------------------------------------------------
//  從OCR接收訊息
//------------------------------------------------------------------------------
//#include "MachineType.h"
//enum eOCRType{eocrUninstal=0, eocrUseMessage=1, eocrUseRS23=2};     //Steven 20120716 : OCR
typedef struct
{
    unsigned int iType;  // 0 : normal model, 1 : 高雄, 2 : 舊版CCD, 3 : OCR
    char cSendData[300];
    int  iCustomer;    //客戶碼
    bool bError;
}OCRMessage;
//---------------------------------------------------------------------------
class TfOCR : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TPanel *palLeft;
    TGroupBox *gbFileName;
    TEdit *edFileName;
    TGroupBox *gbInsp;
    TEdit *edInsp;
    TPanel *btSend;
    TRadioGroup *rgCommadn;
    TCheckBox *cbEnableOcr;
    TButton *Button1;
    TPanel *palRight;
    TMemo *memoOcrCommand;
    TGroupBox *gbMatchImage;
    TImage *imgMatch;
    TGroupBox *gbOCRImage;
    TImage *imgOCR;
    TComm *CommOcr;
    TTimer *TimerOcrOn;
    TTimer *TimerOcrChangeFile;
    TComm *CommOcr_Tester;
    TTimer *Timer1;
    TTabSheet *tsOCR_Cognex;
    TPanel *Panel1;
    TLabel *Label14;
    TLabel *Label15;
    TSpeedButton *spInternetConnect;
    TSpeedButton *spInternetDisConnec;
    TEdit *IPAddress1;
    TEdit *IPPort1;
    TButton *Button2;
    TButton *Button3;
    TMemo *Memo2;
    TClientSocket *ClientSocket1;
    TClientSocket *ClientSocket2;
    TPanel *Panel2;
    TRadioGroup *RadioGroup1;
    TSpeedButton *SpeedButton2;
    TEdit *edtSendCmd;
    TMemo *Memo1;
    TEdit *IPAddress2;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *IPPort2;
    TSpeedButton *SpeedButton4;
    TTabSheet *tsOCR_Cognex_Setting;
    TGroupBox *GroupBox1;
      TRadioGroup *rgOCRTriggerMode;
    TLabel *Label85;
    TLabel *Label86;
    TEdit *edOCRRetry;
    TEdit *edOCRSkip;
    TEdit *edOCRWordCount;
    TPanel *Panel4;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TGroupBox *GroupBox2;
    TLabel *Label17;
    TLabel *Label7;
    TSpeedButton *spbResetCom;
    TSpeedButton *SpeedButton1;
    TComboBox *cbCommReader1;
    TEdit *edSetBlueLight;
    TScrollBar *sbSetBlueLight;
    TButton *btSetBlueLight;
    TComm *CommOcr_Light;
    TLabel *Label5;
    TEdit *edSetRedLight;
    TScrollBar *sbSetRedLight;
    TButton *btSetRedLight;
    TMemo *Memo3;
    TFileListBox *FileListBox1;
    TDirectoryListBox *DirectoryListBox1;
    TCheckBox *ckDisabledKeyin;
    TCheckBox *cbCheckBarCodeMap;
    TCheckBox *ckOCRCheckHasIC;
    TCheckBox *ckOCRMoveSRead;
    TLabel *Label3;
    TEdit *edOCRWordType;
    TCheckBox *ckStartposshift;
    TEdit *edStartposshift;
    TCheckBox *ckOCRLightChange;
    TCheckBox *ckOCRLightNoDown;
    TCheckBox *ckCompareOCRData;
    TGroupBox *GroupBox3;
    TCheckBox *ckOCRAndBinLog;
    TCheckBox *ckOCRBinLogAddMark;
    TCheckBox *ckWordCount;
    TButton *Button4;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall CommOcrReceiveData(TObject *Sender, Pointer Buffer,
          WORD BufferLength);
    void __fastcall btSendClick(TObject *Sender);
    void __fastcall TimerOcrOnTimer(TObject *Sender);
    void __fastcall btSendMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btSendMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall TimerOcrChangeFileTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall CommOcr_TesterReceiveData(TObject *Sender,
          Pointer Buffer, WORD BufferLength);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall spInternetConnectClick(TObject *Sender);
    void __fastcall spInternetDisConnecClick(TObject *Sender);
    void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall RadioGroup1Click(TObject *Sender);
    void __fastcall ClientSocket2Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket2Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket2Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall SpeedButton4Click(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall edOCRRetryClick(TObject *Sender);
    void __fastcall spbResetComClick(TObject *Sender);
    void __fastcall btSetBlueLightClick(TObject *Sender);
    void __fastcall sbSetBlueLightChange(TObject *Sender);
    void __fastcall sbSetRedLightChange(TObject *Sender);
    void __fastcall edSetBlueLightChange(TObject *Sender);
    void __fastcall edSetRedLightChange(TObject *Sender);
    void __fastcall btSetRedLightClick(TObject *Sender);
    void __fastcall IPPort1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edSetBlueLightClick(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall edStartposshiftClick(
          TObject *Sender);
private:    // User declarations
    void __fastcall ProcessOcrCommand();     //處理傳入的資料
    void WriteOcrInfoToMemo(AnsiString asInfo);
      //AI(ht9045-v899) 20260505: create OCR area simulation controls without editing BCB6 dfm layout
      void __fastcall CreateOCRSimulationControls();
      void __fastcall btnOCRSimRunClick(TObject *Sender);
      void __fastcall btnOCRSimStopClick(TObject *Sender);
      void __fastcall tmrOCRSimTimer(TObject *Sender);
      void StopOCRSimulation(AnsiString asStatus, bool bRestoreData=true);
      void UpdateOCRSimulationStatus(AnsiString asStatus);
      TTabSheet *tsOCRSim;
      TGroupBox *gbOCRSim;
      TLabel *lbOCRSimInfo;
      TLabel *lbOCRSimLoopCaption;
      TLabel *lbOCRSimStatus;
      TEdit *edOCRSimLoopTimes;
      TButton *btnOCRSimRun;
      TButton *btnOCRSimStop;
      TTimer *tmrOCRSim;
      TCheckBox *cbOCRSimBypassAOI;                                             //AI(ht9045-v899) 20260511: bypass AOI/vision TCP traffic during simulation so handler motion can be verified without vision PC online
      bool bOCRSimUiCreated;
    eOCRType    _OcrCommType;
    AnsiString  _ImgPath;
    int         _iOcrChangeFileTask;
    bool        _bNeedToDeleteOcrFile;
    //JimmyChiu 20210922 Avoid overflow
    AnsiString LogMsg;
public:     // User declarations
    __fastcall TfOCR(TComponent* Owner);
    int         iOcrWithTesterTask;
    void __fastcall InitialOCRCwithTesterCom(AnsiString    ComPort,
                                         TParity       Parity      =None,
                                         unsigned      Rate        =9600,
                                         TByteSize     ByteSize    =_8,
                                         bool          ParityCheck =false,
                                         TStopBits     StopBits    =_1);

    enum TesterComIndex{
                     testerLotIDOK,
                     testerLotIDNG,
                     testerLotEnd,
                     testerProgramOK,
                     testerProgramNG,
                     testerLotID,
                     testerProgram,
                     TESTER_MAX_CMD //這個一定得在最下面,不可以動
                    };
    AnsiString      sTesterLotId;
    AnsiString      sTesterPGM;
    AnsiString      TesterReceiveBuffer;
    AnsiString      sTester_Send[TESTER_MAX_CMD];
    AnsiString      sTester_Recv[TESTER_MAX_CMD];
    bool            bTester_ReceiveOK[TESTER_MAX_CMD];
    bool            bHaveLotID;
    bool            bSendOCRGetLotID;

    void __fastcall SendTester(int iTesterIndex);
    void __fastcall ProcessTesterCommand();
    int  __fastcall OcrWithTester();
    void __fastcall InitOcrWithTester();

    enum OCRComIndex{ocrVisionOn        =0,
                     ocrVisionOff       =1,
                     ocrStartOk         =2,
                     ocrStartNoReady    =3,

                     ocrRecipeOk        =4,
                     ocrRecipeNoModel   =5,
                     ocrRecipeNoROI     =6,
                     ocrRecipeNoFont    =7,
                     ocrRecipeNoLotID   =8,

                     //ocrStartNoGolden   =3,      //ChungHung 20120830 add OCR Function delete
                     //ocrStartNoRoi      =4,
                     //ocrStartNoMatch    =5,
                     //ocrStartRepeat     =6,

                     ocrEnd             =9,
                     ocrFileOk          =10,
                     ocrClear           =11,
                     //ocrRelease         =11,       //ChungHung 20120830 add OCR Function delete

                     ocrInspOk          =12,
                     ocrInspNg          =13,
                     //ocrInspErr         =13,       //ChungHung 20120830 add OCR Function delete
                     ocrInspErr2001     =14,     //ChungHung 20120830 add OCR Function add
                     ocrInspErr2002     =15,     //ChungHung 20120830 add OCR Function add
                     ocrInspErr2003     =16,     //ChungHung 20120830 add OCR Function add
                     ocrInspErr2004     =17,     //ChungHung 20120830 add OCR Function add
                     ocrInspErr3001     =18,     //ChungHung 20120830 add OCR Function add
                     ocrInspErr3002     =19,     //ChungHung 20120830 add OCR Function add

                     ocrTrainOK         =20,     //ChungHung 20120830 add OCR Function add
                     ocrTrainNG         =21,     //ChungHung 20120830 add OCR Function add

                     ocrLot             =22,     //ChungHung 20120830 add OCR Function add

                     //ocrGrabTimeOut     =15,       //ChungHung 20120830 add OCR Function delete
                     //ocrReadErr         =16,       //ChungHung 20120830 add OCR Function delete
                     OCR_MAX_CMD //這個一定得在最下面,不可以動
                    };

    AnsiString      OCRReceiveBuffer;
    AnsiString      sOCR_Send[OCR_MAX_CMD];
    AnsiString      sOCR_Recv[OCR_MAX_CMD];
    bool            bOcr_ReceiveOK[OCR_MAX_CMD];
    bool            bSendOcrSendStart;
    void            SetOcrCommType(eOCRType Type)  {_OcrCommType=Type;};
    void            SetOcrEnable(bool Enable)      {cbEnableOcr->Checked=Enable;};          //啟動OCR
    void            SetOcrFileName(AnsiString Name);                                        //設定工作檔
    void            SetOcrInspText(AnsiString Name);                                        //設定辨識文字
    bool            GetOcrEnable()                 {return cbEnableOcr->Checked;};
    AnsiString      GetOcrFileName()               {return edFileName->Text;};
    AnsiString      GetOcrInspText()               {return edInsp->Text;};
    void            SendOCR(OCRComIndex OcrIndex, bool bSetToFalse=true);
    void            ShowOCRImg();
    void            ShowMatchImg();
    void            InitInsp();
    bool            bShow;
//    HWND            HOCRWind;
//    OCRMessage      *OCRReceve;
//    OCRMessage      *OCRSend;
    int  __fastcall CheckInspectionResult();
    void __fastcall SendOcrStart();
    void __fastcall DoOCRReleaseAndInspEnd();
    void __fastcall InitialOCRPara(bool bSendRealCCDStart=true);
    void __fastcall InitialOCRCom(AnsiString    ComPort,
                                         TParity       Parity      =None,
                                         unsigned      Rate        =38400,
                                         TByteSize     ByteSize    =_8,
                                         bool          ParityCheck =false,
                                         TStopBits     StopBits    =_1);

    void DoConnect();               //wei 20150720 OCR連線
    void DoDisConnect();            //wei 20150720 OCR連線
      //AI(ht9045-v899) 20260522: centralize OCR capture trigger mode so auto flow and manual button use the same switch/SE8 behavior
      bool IsOCRCommandTrigger();
      bool StartOCRCaptureTrigger(bool bBypassVision=false);
      void StopOCRCaptureTrigger();
    bool OCRIn();
    int OCRChangeFile(bool bReset=false);
    void fOCR_ReadFile();
    void fOCR_DoIniDataToForm();

    void __fastcall RS232Init();
    void StopAllCom();
    void SaveOCRImage();
    void DeleteOCRImage();
    void ReadOCRImage();    //wei 20161122 OCR 存圖檔名加入輸入值
    bool CheckOCRWordType(AnsiString asOCR);  //wei 20161128 確認字串各自Type是否正確
    void ChangeLightValue(int Channel, int Value);
    void SaveOCRAndBinData(AnsiString asOCRCode, AnsiString iBinData, int iBinFailCate);        //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
    void UpLoadOCRAndBinLog();                                                                  //KenHsieh 20230502 : 利揚要求上傳OCR + BIN Log上傳至Host
    TStringList *ListOCRByLot;                                                                  //KenHsieh 20220825 : 新增OCR比對功能
    TStringList *ListOCRDataLog;                                                                //KenHsieh 20230406 : 新增OCR Data + Bin Log功能
};
//---------------------------------------------------------------------------
extern PACKAGE TfOCR *fOCR;
//---------------------------------------------------------------------------
#endif
