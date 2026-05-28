//---------------------------------------------------------------------------

#ifndef MyCCLinkSensorH
#define MyCCLinkSensorH
//---------------------------------------------------------------------------

#include "MyCCLink.h"
#include "cMyDNM100UD.h"
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "ALed.hpp"
#include "HTray.h"
//---------------------------------------------------------------------------
class TfCCLink : public TForm
{
__published:    // IDE-managed Components
    TTimer *Timer1;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *tsCCLinkSetting;
    TRadioGroup *rgInputMethod;
    TGroupBox *gbPercentage;
    TEdit *edPercentage;
    TLabel *labPercentage;
    TSpeedButton *spbSave;
    TGroupBox *gbDefaultValue;
    TEdit *edDefaultValue;
    TTabSheet *tsSocketAndRotate;
    TPanel *palSocketSensor;
    TLabel *labSocketSensor;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label21;
    TPanel *palSocketSensor1;
    TALed *SocketLED1;
    TALed *SocketLED2;
    TALed *SocketLED3;
    TALed *SocketLED4;
    TALed *SocketLED5;
    TALed *SocketLED6;
    TALed *SocketLED7;
    TALed *SocketLED8;
    TButton *btIn3_1;
    TButton *btIn3_2;
    TButton *btIn3_3;
    TButton *btIn3_4;
    TButton *btIn3_5;
    TButton *btIn3_8;
    TButton *btIn3_7;
    TButton *btIn3_6;
    TEdit *edIn3_8;
    TEdit *edIn3_7;
    TEdit *edIn3_6;
    TEdit *edIn3_5;
    TEdit *edIn3_4;
    TEdit *edIn3_3;
    TEdit *edIn3_2;
    TEdit *edIn3_1;
    TTMyTray *mtInSocketBase;
    TButton *SetSocket;
    TPanel *palRotate;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TButton *SetRotate;
    TLabel *labRotateSensor;
    TPanel *palSensorBg1;
    TPanel *palSensorBgL;
    TPanel *palInSht1;
    TLabel *labInSht1;
    TLabel *Label2;
    TLabel *Label10;
    TLabel *Label14;
    TLabel *Label19;
    TLabel *Label20;
    TALed *MyLedLane17;
    TEdit *edIn1_9;
    TButton *btIn1_9;
    TTMyTray *mtIn3;
    TPanel *palInSht1_1;
    TALed *ledInSht1_1;
    TALed *ledInSht1_2;
    TALed *ledInSht1_3;
    TALed *ledInSht1_4;
    TALed *ledInSht1_5;
    TALed *ledInSht1_6;
    TALed *ledInSht1_7;
    TALed *ledInSht1_8;
    TButton *btIn1_1;
    TButton *btIn1_2;
    TButton *btIn1_3;
    TButton *btIn1_4;
    TButton *btIn1_5;
    TButton *btIn1_8;
    TButton *btIn1_7;
    TButton *btIn1_6;
    TEdit *edIn1_8;
    TEdit *edIn1_7;
    TEdit *edIn1_6;
    TEdit *edIn1_5;
    TEdit *edIn1_4;
    TEdit *edIn1_3;
    TEdit *edIn1_2;
    TEdit *edIn1_1;
    TTMyTray *mtIn1;
    TButton *btSetIn1;
    TPanel *palInSht2;
    TLabel *labInSht2;
    TLabel *Label5;
    TLabel *Label11;
    TLabel *Label15;
    TLabel *Label3;
    TLabel *Label18;
    TALed *MyLedLane18;
    TButton *btIn2_9;
    TEdit *edIn2_9;
    TTMyTray *mtIn4;
    TPanel *palInSht2_1;
    TALed *ledInSht2_1;
    TALed *ledInSht2_2;
    TALed *ledInSht2_3;
    TALed *ledInSht2_4;
    TALed *ledInSht2_5;
    TALed *ledInSht2_6;
    TALed *ledInSht2_7;
    TALed *ledInSht2_8;
    TButton *btIn2_1;
    TButton *btIn2_2;
    TButton *btIn2_3;
    TButton *btIn2_4;
    TButton *btIn2_5;
    TButton *btIn2_8;
    TButton *btIn2_7;
    TButton *btIn2_6;
    TEdit *edIn2_8;
    TEdit *edIn2_7;
    TEdit *edIn2_6;
    TEdit *edIn2_5;
    TEdit *edIn2_4;
    TEdit *edIn2_3;
    TEdit *edIn2_2;
    TEdit *edIn2_1;
    TTMyTray *mtIn2;
    TButton *btSetIn2;
    TButton *btShuttlePositionMove;
    TPanel *palSensorBgR;
    TPanel *palOutSht2;
    TLabel *labOutSht2;
    TALed *MyLedLane19;
    TTMyTray *mtOut2;
    TEdit *edOut2;
    TButton *btOut2;
    TPanel *palOutSht1;
    TLabel *labOutSht1;
    TALed *MyLedLane20;
    TTMyTray *mtOut1;
    TEdit *edOut1;
    TButton *btOut1;
    TPanel *palButton;
    TSpeedButton *sbExit;
    TSpeedButton *sbReset;
    TLabel *lbErrMess;
    TPanel *palRotate1;
    TALed *RotateLED1;
    TALed *RotateLED2;
    TALed *RotateLED3;
    TALed *RotateLED4;
    TEdit *edIn3_9;
    TButton *btIn3_9;
    TTMyTray *mtInOutRotateSen;
    TEdit *edIn3_10;
    TButton *btIn3_10;
    TEdit *edIn3_11;
    TButton *btIn3_11;
    TEdit *edIn3_12;
    TButton *btIn3_12;
    TPanel *palColorSensor;
    TLabel *labColorSensor;
    TLabel *Label4;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TPanel *palColorSensor1;
    TALed *ColorLED1;
    TALed *ColorLED2;
    TALed *ColorLED3;
    TALed *ColorLED4;
    TALed *ColorLED5;
    TEdit *edIn3_13;
    TButton *btIn3_13;
    TTMyTray *mtColorSen;
    TEdit *edIn3_14;
    TButton *btIn3_14;
    TEdit *edIn3_15;
    TButton *btIn3_15;
    TEdit *edIn3_16;
    TButton *btIn3_16;
    TEdit *edIn3_17;
    TButton *btIn3_17;
    TButton *SetColor;
    TALed *ColorLED6;
    TEdit *edIn3_18;
    TButton *btIn3_18;
    TButton *btSave;
    TButton *btRead;
    TTabSheet *tsErrMess;
    TMemo *mmoErrMess;
    TPanel *Panel2;
    TPanel *Panel1;
    TPanel *palAmplifier0;
    TPanel *palAmplifier1;
    TPanel *palAmplifier2;
    TPanel *palAmplifier3;
    TPanel *palAmplifier4;
    TPanel *palAmplifier5;
    TPanel *palAmplifier6;
    TPanel *palAmplifier7;
    TPanel *palAmplifier8;
    TPanel *palAmplifier9;
    TPanel *palAmplifier10;
    TPanel *palAmplifier11;
    TPanel *palAmplifier12;
    TPanel *palAmplifier13;
    TPanel *palAmplifier14;
    TPanel *palAmplifier15;
    TALed *ColorLED8;
    TALed *ColorLED7;
    TEdit *edIn3_20;
    TButton *btIn3_20;
    TEdit *edtIn3_19;
    TButton *btIn3_19;
    TGroupBox *grpCanBusSeach;
    TButton *btnCanBusSeach;
    TMemo *mmoCanBusLog;
    TTimer *tmrCanBusSearch;
    TButton *btnReadCanBusSetting;
    TTabSheet *tsSocketBase2;
    TPanel *Panel3;
    TLabel *Label1;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TPanel *Panel4;
    TALed *SocketLED9;
    TALed *SocketLED10;
    TALed *SocketLED11;
    TALed *SocketLED12;
    TALed *SocketLED13;
    TALed *SocketLED14;
    TALed *SocketLED15;
    TALed *SocketLED16;
    TButton *btIn4_1;
    TButton *btIn4_2;
    TButton *btIn4_3;
    TButton *btIn4_4;
    TButton *btIn4_5;
    TButton *btIn4_8;
    TButton *btIn4_7;
    TButton *btIn4_6;
    TEdit *edIn4_8;
    TEdit *edIn4_7;
    TEdit *edIn4_6;
    TEdit *edIn4_5;
    TEdit *edIn4_4;
    TEdit *edIn4_3;
    TEdit *edIn4_2;
    TEdit *edIn4_1;
    TTMyTray *mtInSocketBase9_16;
    TButton *setSocket9_24;
    TPanel *Panel5;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TPanel *Panel6;
    TALed *SocketLED17;
    TALed *SocketLED18;
    TALed *SocketLED19;
    TALed *SocketLED20;
    TALed *SocketLED21;
    TALed *SocketLED22;
    TALed *SocketLED24;
    TALed *SocketLED23;
    TEdit *edIn4_9;
    TButton *btIn4_9;
    TTMyTray *mtInSocketBase17_24;
    TEdit *edIn4_10;
    TButton *btIn4_10;
    TEdit *edIn4_11;
    TButton *btIn4_11;
    TEdit *edIn4_12;
    TButton *btIn4_12;
    TEdit *edIn4_13;
    TButton *btIn4_13;
    TEdit *edIn4_14;
    TButton *btIn4_14;
    TEdit *edIn4_16;
    TButton *btIn4_16;
    TEdit *edIn4_15;
    TButton *btIn4_15;
    TButton *Button1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sbExitClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall sbResetClick(TObject *Sender);
    void __fastcall edIn2_1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btIn2_8MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall edPercentageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall edDefaultValueMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btSetIn2Click(TObject *Sender);
    void __fastcall btSetIn1Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall SetSocketClick(TObject *Sender);
    void __fastcall SetRotateClick(TObject *Sender);
    void __fastcall btIn3_1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btIn3_9MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btShuttlePositionMoveClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btIn3_13MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall SetColorClick(TObject *Sender);
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall btReadClick(TObject *Sender);
    void __fastcall btnCanBusSeachClick(TObject *Sender);
    void __fastcall tmrCanBusSearchTimer(TObject *Sender);
    void __fastcall btnReadCanBusSettingClick(TObject *Sender);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall btIn4_1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:    // User declarations
public:     // User declarations
    TTMyTray *TMyTrayPtr[11];                                                   //kevin 20130420[6];
    TEdit *EditPtr[56];                                                         //kevin 20130420 [20];
    TButton *ButtonPtr[56];                                                     //kevin 20130420 [20];
    TALed *LamePtr[56];                                                         //kevin 20130420 [20];

    TEdit *EdInSht1[8];
    TEdit *EdInSht2[8];
    TButton *BtnInSht1[8];
    TButton *BtnInSht2[8];
    TALed *LedInSht1[8];
    TALed *LedInSht2[8];

    TMyCCLinkCtrl *MyCCLink;
    TMyDNM100UD_Card *DNM100UD;
    int iSensorNo[11];                                                          //kevin 20130420 [6];
    unsigned short iTotalDevices;
    unsigned char  iDesMACIDList [16];                                          //所有 NUDN1 站別清單
    unsigned char  iAmplifierList[16];                                          //所有 NUDN1 接著 FS-N12N 數量
    void __fastcall ReadSetting();                                              //Steven 20110221
    void __fastcall SaveSetting();                                              //Steven 20110221
    __fastcall TfCCLink(TComponent* Owner);
    int iInputMethod;
    int iPercentage;
    int iDefaultValue;
    int iMinValue;
    void __fastcall ShowUseSensor(int iType,int iPitch);
    bool bShow;                                                                 //Steven 20121015 : Alarm時要到前面
    int iAutoRetryCnt;                                                          //JerryYang 20160729 重新連線次數
    int iAllSetCount;
    void __fastcall SaveSetupFile();                                            //wei 20180622 (Steven) : CCLink Value Save File
    void __fastcall ReadSetupFile();                                            //wei 20180622 (Steven) : CCLink Value Save File
    bool bNewFile;                                                              //wei 20180622 (Steven) : CCLink Value Save File
    void __fastcall GetStationStart();                                          //JerryYang 20200327 取得socket/ rotate/ color起始站號

    int iCanBusSrachTask;                                                       //Sam 20210518 : 新增 CanBus 軟體配置
    int iIsSearchOKErrCnt;
    int iSearchCnt;
    WORD SearchTotalDevices;
    BYTE SearchDesMACID[500];
    BYTE SearchType[500];
    WORD SearchDeviceInputLen[500];
    WORD SearchDeviceOutputLen[500];

    void SetInitialOK(bool bInitailOK);                                         //Sam 20230707 : EtherCAT Shuttle sensor
    void Reset();
    int GetValue(int iNU, int iAMPNo);
    int GetIOValue(int iNU, int iAMPNo);
    bool GetIO(int iNU, int iAMPNo);
    void SetIOValue(int iNU, int iAMPNo, int iValue);
    bool bHasError();
    AnsiString GetErrorMessage();
};
//---------------------------------------------------------------------------
extern PACKAGE TfCCLink *fCCLink;
//---------------------------------------------------------------------------
extern bool UseCanBusOrEtherCAT();                                              //Sam 20230707 : EtherCAT Shuttle sensor
extern bool UseSensorMod3();                                                    //Sam 20230707 : EtherCAT Shuttle sensor
void SetIOTableByNUEC1(int iNu, int iSenStart, int *iAmpSort, int iAmpNum);     //Sam 20230725 : EtherCAT NUEC1 IO 取代線路 IO
extern void SetIOTableByNUEC1();                                                //Sam 20230725 : EtherCAT NUEC1 IO 取代線路 IO
#endif
