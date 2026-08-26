//---------------------------------------------------------------------------

#ifndef AGVH
#define AGVH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ALed.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfAGV : public TForm
{
__published:    // IDE-managed Components
    TPageControl *AGV;
    TTabSheet *tsAGV_Setting;
    TPanel *Panel2;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TPanel *Panel3;
    TGroupBox *GroupBox1;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TEdit *edE84_2_TP1;
    TEdit *edE84_2_TP2;
    TEdit *edE84_2_TP3;
    TEdit *edE84_2_TP4;
    TEdit *edE84_2_TP5;
    TEdit *edE84_2_TP6;
    TEdit *edE84_2_TA1;
    TEdit *edE84_2_TA2;
    TEdit *edE84_2_TA3;
    TEdit *edE84_2_TD0;
    TEdit *edE84_2_TD1;
    TGroupBox *gbE84TimeOut;
    TLabel *labTP1;
    TLabel *labTP2;
    TLabel *labTP3;
    TLabel *labTP4;
    TLabel *labTP5;
    TLabel *labTP6;
    TLabel *labTA1;
    TLabel *labTA2;
    TLabel *labTA3;
    TLabel *LabTD0;
    TLabel *LabTD1;
    TEdit *edE84_1_TP1;
    TEdit *edE84_1_TP2;
    TEdit *edE84_1_TP3;
    TEdit *edE84_1_TP4;
    TEdit *edE84_1_TP5;
    TEdit *edE84_1_TP6;
    TEdit *edE84_1_TA1;
    TEdit *edE84_1_TA2;
    TEdit *edE84_1_TA3;
    TEdit *edE84_1_TD0;
    TEdit *edE84_1_TD1;
    TCheckBox *cbEnableAGVFunction;
    TButton *btInitalLoad;
    TButton *btInitalUnLoad;
    TGroupBox *GroupBox4;
    TEdit *edAuto1Count;
    TEdit *edAuto2Count;
    TEdit *edAuto3Count;
    TLabel *Label32;
    TLabel *Label42;
    TLabel *Label43;
    TTimer *Timer1;
    TTimer *Timer2;
    TGroupBox *GroupBox10;
    TLabel *Label19;
    TLabel *Label20;
    TMemo *mmE84Log;
    TPanel *Panel18;
    TLabel *Label128;
    TLabel *Label129;
    TLabel *Label130;
    TLabel *Label132;
    TLabel *Label133;
    TLabel *Label134;
    TLabel *Label135;
    TLabel *Label136;
    TLabel *Label140;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TALed *ALed1;
    TALed *ALed2;
    TALed *ALed3;
    TALed *ALed4;
    TALed *ALed5;
    TALed *ALed6;
    TALed *ALed7;
    TALed *ALed8;
    TALed *ALed10;
    TALed *ALed11;
    TALed *ALed12;
    TALed *ALed13;
    TALed *ALed14;
    TALed *ALed15;
    TALed *ALed16;
    TALed *ALed17;
    TALed *ALed18;
    TALed *ALed9;
    TPanel *Panel1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TALed *ALed19;
    TALed *ALed20;
    TALed *ALed21;
    TALed *ALed22;
    TALed *ALed23;
    TALed *ALed24;
    TALed *ALed25;
    TALed *ALed26;
    TALed *ALed27;
    TALed *ALed28;
    TALed *ALed29;
    TALed *ALed30;
    TALed *ALed31;
    TALed *ALed32;
    TALed *ALed33;
    TALed *ALed34;
    TALed *ALed35;
    TALed *ALed36;
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btInitalLoadClick(TObject *Sender);
    void __fastcall btInitalUnLoadClick(TObject *Sender);
    void __fastcall edAuto1CountMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edE84_1_TP1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall edAGVWorryingwattingtimeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
public:        // User declarations
    __fastcall TfAGV(TComponent* Owner);
    bool fShow;
    void __fastcall ShowE84Sensor();
    void __fastcall DoE84Loader();
    void __fastcall DoE84Unloader();
    void __fastcall DoE84LoaderScan(int Pos);
    void __fastcall DoE84UnloaderScan(int Pos);
    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm();

    void ShowE84Log(AnsiString str, int iFunction, int iLoader);

    void CheckSECSValue();
    bool bE84LoaderFinishflag[3];
    bool bE84UnloaderFinishflag[3];

    bool bE84Loaderflag[3];
    bool bE84Unloaderflag[3];

    void __fastcall E84StatusChange();
    void __fastcall NowE84Status();
    void E84StatusLog(AnsiString str, bool bflag);

    bool bOfflineTesterFlag;

    bool bATK_AMR_DoLotEndSent;                                                 //AI(general) 20260402 (RogerYang) : ATK AMR Final Lot End提前送出DoLotEnd旗標
    bool bATK_AMR_DoHostLotStart;                                               //RogerYang 20260402 : add for secs lotstart flag
    bool bATKAMR_GET_LOTORDER0_Ready;

    bool IsSPIL_AMR();
    bool IsATK_AMR();
    bool Use_AMR();
    bool IsTFAMD_AMR();                                                          //Ifor 20260715 : TFAMD AMR (CC_AMD_M + A68 on + A65 off, no ID reader)
    void ScanLoadPort();
};
//---------------------------------------------------------------------------
extern PACKAGE TfAGV *fAGV;

enum eATkTrayFeed{eAtkTfInit        =0,
                  eAtkTfFeedAuto    =1,
                  eAtkTfAutoToAMR   =2,
                  eAtkTfPutIDTray   =3,
                  eAtkTfPutCover    =4,
                  eAtkTfPutEmptyTray=5,
                  eAtkTfMoveFixIC   =6,
                  eAtkTfFeedFix     =7,
                  eAtkTfFixToAMR    =8,
                  eAtkTfNormalFeed  =9,
                 };

void InitialE84LoadTask();
void InitialE84UnLoaderTask();
void InitialE84LoadSensor();
void InitialE84UnloadSensor();
void InitialLoaderScanTask();
void InitialUnLoaderScanTask();

extern bool bIsStackBusy(int iPortID);                                          //JerryYang 20250521 : For AMR
extern void bScanLoadPortState_SPIL();
extern void bScanUnLoadPortState_SPIL(int iPos);
extern void bScanFixPortState_SPIL(int iPos);

extern void bScanLoadPortState_Analog(int iPos);
extern void bScanUnLoadPortState_Analog(int iPos);
extern void bScanFixPortState_Analog(int iPos);

extern AnsiString sDCC_ATK;
extern AnsiString sOutputBinCode;
extern int iLoadStateATK[etTrayCount];
extern int iCurrStateATK[etTrayCount];
extern void bScanLoadPortState_ATK();
//---------------------------------------------------------------------------
#endif
