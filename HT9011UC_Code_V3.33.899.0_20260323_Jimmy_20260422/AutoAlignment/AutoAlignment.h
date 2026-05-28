//---------------------------------------------------------------------------

#ifndef AutoAlignmentH
#define AutoAlignmentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "HTray.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include <Buttons.hpp>

enum ALIGNMENTCMD { LOAD_FILE =0,
                    UNIT_CENTER,
                    UNIT_LEFT,
                    UNIT_RIGHT,
                    UNIT_UP,
                    UNIT_DOWN,
                    TEACH_GOLDEN,
                    TEACH_TRAY,
                    TEACH_HOTPLATE,
                    TEACH_SHUTTLE,
                    WAIT_TEACH_GOLDEN,
                    AUTO_GOLDEN,
                    AUTO_TRAY,
                    AUTO_HOTPLATE,
                    AUTO_SHUTTLE,
                    TEACH_AUTOCLEAN,
                    AUTO_AUTOCLEAN,
                    TOTAL};  //KenHsieh 20211214 : AOA add AutoClean
//---------------------------------------------------------------------------
class TfAutoAlignment : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TGroupBox *GroupBox47;
    TLabel *Label819;
    TLabel *Label820;
    TGroupBox *GroupBox46;
    TCheckBox *cbAutoAlignmentTray_AfterHome;
    TCheckBox *cbAutoAlignmentTray_ChangeSetup;
    TCheckBox *cbAutoAlignmentTray_InitialStart;
    TCheckBox *cbAutoAlignmentTray_EveryTray;
    TCheckBox *cbAutoAlignmentTray_OneCycle;
    TCheckBox *cbAutoAlignmentTray_AfterHome_Z;
    TCheckBox *cbAutoAlignmentTray_ChangeSetup_Z;
    TCheckBox *cbAutoAlignmentTray_InitialStart_Z;
    TCheckBox *cbAutoAlignmentTray_EveryTray_Z;
    TCheckBox *cbAutoAlignmentTray_OneCycle_Z;
    TGroupBox *grpInputArea;
    TCheckBox *cbAutoAlignmentCK_ChangeSetup;
    TCheckBox *cbAutoAlignmentCK_InitialStart;
    TCheckBox *cbAutoAlignmentCK_AfterHome;
    TCheckBox *cbAutoAlignmentCK_OneCycle;
    TCheckBox *cbAutoAlignmentCK_ChangeSetup_Z;
    TCheckBox *cbAutoAlignmentCK_InitialStart_Z;
    TCheckBox *cbAutoAlignmentCK_AfterHome_Z;
    TCheckBox *cbAutoAlignmentCK_OneCycle_Z;
    TEdit *edAutoAlignment_DeviceThick;
    TPageControl *PageControl19;
    TTabSheet *TabSheet33;
    TGroupBox *GroupBox53;
    TLabel *Label821;
    TLabel *Label822;
    TLabel *Label823;
    TLabel *Label824;
    TEdit *edAutoAlignmentTray_InArmZRealaseOffset;
    TEdit *edAutoAlignmentTray_InArmZPickUpOffset;
    TGroupBox *GroupBox54;
    TLabel *Label825;
    TLabel *Label826;
    TLabel *Label827;
    TLabel *Label828;
    TEdit *edAutoAlignmentTray_OutArmZRealaseOffset;
    TEdit *edAutoAlignmentTray_OutArmZPickUpOffset;
    TTabSheet *TabSheet36;
    TGroupBox *GroupBox57;
    TLabel *Label829;
    TLabel *Label830;
    TLabel *Label831;
    TLabel *Label832;
    TEdit *edAutoAlignmentCK_InArmZRealaseOffset;
    TEdit *edAutoAlignmentCK_InArmZPickUpOffset;
    TGroupBox *GroupBox58;
    TLabel *Label833;
    TLabel *Label834;
    TLabel *Label835;
    TLabel *Label836;
    TEdit *edAutoAlignmentCK_OutArmZRealaseOffset;
    TEdit *edAutoAlignmentCK_OutArmZPickUpOffset;
    TCheckBox *cbEnabledAutoAlignment;
    TPageControl *PageControl18;
    TTabSheet *TabSheet34;
    TLabel *Label805;
    TTMyTray *myAlignmentTray;
    TGroupBox *GroupBox49;
    TLabel *Label810;
    TLabel *Label807;
    TEdit *edtAlignmentPointX;
    TEdit *edtAlignmentPointY;
    TButton *btnAutoAlignmentTrayPointCal;
    TTabSheet *TabSheet35;
    TLabel *Label806;
    TTMyTray *myAlignmentPlate;
    TGroupBox *GroupBox50;
    TLabel *Label817;
    TLabel *Label818;
    TEdit *edtAlignmentPlatePointX;
    TEdit *edtAlignmentPlatePointY;
    TButton *btnAutoAlignmentPlatePointCal;
    TTimer *Timer1;
    TClientSocket *ClientSocket2;
    TClientSocket *ClientSocket1;
    TGroupBox *GroupBox3;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edtInArmAddress;
    TEdit *edtInArmPort;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *edtOutArmAddress;
    TEdit *edtOutArmPort;
    TButton *btnConnect;
    TButton *btnDisconnect;
    TGroupBox *GroupBox4;
    TLabel *Label5;
    TComboBox *cbInArmCommand;
    TLabel *Label6;
    TComboBox *cbOutArmCommand;
    TMemo *Memo1;
    TMemo *Memo2;
    TButton *Button1;
    TButton *Button2;
    TEdit *edtFileName;
    TLabel *Label7;
    TPanel *Panel2;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TLabel *Label395;
    TEdit *edAOADecodetimeout;
    TPanel *pnlUseFixTray;
    TCheckBox *cbAOA_UseFix1;
    TCheckBox *cbAOA_UseFix3;
    TCheckBox *cbAOA_UseFix2;
    TCheckBox *cbAOA_UseFix4;
    TCheckBox *cbAOA_UseFix5;
    TCheckBox *cbAOA_UseFix6;
    void __fastcall btnConnectClick(TObject *Sender);
    void __fastcall btnDisconnectClick(TObject *Sender);
    void __fastcall ClientSocket1Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket2Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket2Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket2Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket2Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall edtFileNameChange(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall btnAutoAlignmentTrayPointCalClick(TObject *Sender);
    void __fastcall btnAutoAlignmentPlatePointCalClick(TObject *Sender);
    void __fastcall cbAutoAlignmentTray_AfterHomeClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall edAutoAlignment_DeviceThickMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edtInArmPortMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edAutoAlignmentTray_InArmZRealaseOffsetClick(
          TObject *Sender);
    void __fastcall edtAlignmentPointXMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations

public:     // User declarations
    __fastcall TfAutoAlignment(TComponent* Owner);
    bool    bShow;
    bool    bNeedHome;          //KenHsieh 20211202 : 修改重新打開AOA後，先做Onecycle再回Home重做AOA
    bool bInArmSendTeachCmd;
    bool bOutArmSendTeachCmd;
    void __fastcall TcpipOpen();
    void __fastcall TcpipClose();
    void __fastcall ChangeFileName(AnsiString FileName);
    void __fastcall InArmSendCommand(ALIGNMENTCMD cmd);
    int __fastcall InArmGetResult(ALIGNMENTCMD cmd,double &dOffsetX,double &dOffsetY);
    void __fastcall OutArmSendCommand(ALIGNMENTCMD cmd);
    int __fastcall OutArmGetResult(ALIGNMENTCMD cmd,double &dOffsetX,double &dOffsetY);

    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm();
    void DoAutoAlignment();
};
//---------------------------------------------------------------------------
extern PACKAGE TfAutoAlignment *fAutoAlignment;
//---------------------------------------------------------------------------
void InitInArmPickerXYAndPitchAutoAlignment();
int DoInArmPickerXYAndPitchAutoAlignment(int iBasePicker, bool bAll, int *iXPos, int *iYPos, int *iZPos); //0: 未完成 1: 成功 2: 中斷

void InitInArmCCDAlignmentTeachUnitProcess();
int DoInArmCCDAlignmentTeachUnitProces(double &ResolutionX, double &ResolutionY, double &dRadianX, double &dRadianY, int iTeach);  //0: Correction mm/pixel 1: Teach Pattern 2: Teach Pattern and Correction mm/pixel

void InitInArmAlignment();
bool DoInArmTeachAlignment(unsigned long &lAction);

void InitDoInArmTeachAlignmentProcessTask();
bool DoInArmTeachAlignmentProcess(unsigned long &lAction);

void InitInArmXYAutoTeachProcess();
int DoInArmXYAlignmentTeachProcess(int iArea, int bTeach);                      //iArea 0=loader 1=plate1 2=plate2 3:Shuttle1 4:Shuttle2  5:AutoClean

void CheckInArmXYScaleByAutoTeach(int &iXPos, int &iYPos, int iArea);           //index 0:Loader 1:Plate1 2:Plate2 3:Shuttle1 4:Shuttle2
void InitDoAutoRunLoaderTray();
bool DoAutoRunLoaderTray(int iArea);
void DoInArmXYOffsetClear();
void SaveInArmAutoAlignmentLog(int iArea, int iIndexX, int iIndexY);            //KenHsieh 20220111 : AOA add Save Log

void InitOutArmCCDAlignmentTeachUnitProcess();
int DoOutArmCCDAlignmentTeachUnitProces(double &ResolutionX, double &ResolutionY, double &dRadianX, double &dRadianY, int iTeach);      //0: Correction mm/pixel 1: Teach Pattern 2: Teach Pattern and Correction mm/pixel

void InitOutArmPickerXYAndPitchAutoAlignment();
int DoOutArmPickerXYAndPitchAutoAlignment(int iBasePicker, bool bAll, int *iXPos, int *iYPos, int *iZPos); //0: 未完成 1: 成功 2: 中斷

void InitOutArmXYAutoTeachProcess();
int DoOutArmXYAlignmentTeachProcess(int iArea, int bTeach);                     //iArea 0=auto1 1=auto2 2=auto3 3:fix1 4:fix2 5:fix3 6:shuttle1 7:shuttle2

void InitDoAutoRunUnloaderTray();
bool DoAutoRunUnloaderTray(int iArea);

void InitOutArmAlignment();
bool DoOutArmTeachAlignment(unsigned long &lAction);

void InitDoOutArmTeachAlignmentProcessTask();
bool DoOutArmTeachAlignmentProcess(unsigned long &lAction);

void CheckOutArmXYScaleByAutoTeach(int &iXPos, int &iYPos, int iArea);          //index 0:Auto1 1:Auto2 2:Auto3 3:Fix1 4:Fix2 5:Fix3 6:Shuttle1 7:Shuttle2

void DoOutArmXYOffsetClear();

bool bAutoAlignmentCheckHasTray(int iArea);                                     //kenhsieh 20211005 : 檢查是否有Tray
void SaveOutArmAutoAlignmentLog(int iArea, int iIndexX, int iIndexY);           //KenHsieh 20220111 : AOA add Save Log


void InitDoOutArmTeachAlignmentProcessTask();
bool DoOutArmTeachAlignmentProcess(unsigned long &lAction);
void CheckOutArmXYScaleByAutoTeach(int &iXPos, int &iYPos, int iArea);

bool CheckInArmAutoAlignmentTrayModeBeUse(int iMode, bool bSet=false);
bool CheckInArmAutoAlignmentCKModeBeUse(int iMode, bool bSet=false);
bool CheckOutArmAutoAlignmentTrayModeBeUse(int iMode, bool bSet=false);
bool CheckOutArmAutoAlignmentCKModeBeUse(int iMode, bool bSet=false);
void VerifyNeedDoAlignment(unsigned long Tray, unsigned long Kit, bool bSet=false);
#endif
