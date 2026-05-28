//---------------------------------------------------------------------------

#ifndef CassetteH
#define CassetteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "HTray.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "ALed.hpp"
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TfCassette : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet2;
    TTMyTray *mtCassette01;
    TTMyTray *mtCassette02;
    TTMyTray *mtCassette03;
    TTMyTray *mtCassette05;
    TTMyTray *mtCassette07;
    TTMyTray *mtCassette08;
    TTMyTray *mtCassette09;
    TTMyTray *mtCassette10;
    TTMyTray *mtLoadPort;
    TImage *Image1;
    TTMyTray *mtCassette06;
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TTMyTray *TMyTray1;
    TTMyTray *mtCassette04;
    TTMyTray *TMyTray2;
    TTMyTray *TMyTray3;
    TTMyTray *TMyTray4;
    TTMyTray *mtTrayConversion;
    TTMyTray *mtTrayAuto1;
    TTMyTray *mtTrayAuto2;
    TTMyTray *mtTrayAuto3;
    TTMyTray *mtTrayEmpty;
    TTMyTray *mtTrayLoader;
    TStringGrid *sgMRBufferList;
    TStringGrid *sgMRTrayList;
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
    TALed *RGB00;
    TALed *RGB01;
    TALed *RGB02;
    TALed *RGB03;
    TALed *RGB04;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TPanel *Panel9;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel12;
    TPanel *Panel13;
    TPanel *palMainStatus_MR;
    TTabSheet *TabSheet1;
    TPanel *Panel15;
    TStringGrid *sgTrayIDList;
    TTabSheet *TabSheet3;
    TPanel *Panel16;
    TMemo *Memo1;
    TButton *Button6;
    TCheckBox *cbLoaderConsecutiveLot;
    TCheckBox *cbChooseCassetteMode;
    TCheckBox *cbHotplateConsecutiveLot;
    TCheckBox *cbQAMode;
    TCheckBox *cbS10F5NoShow;
    TCheckBox *cbAutoMode;
    TCheckBox *cbManualAutoMode;
    TCheckBox *CheckBox7;
    TGroupBox *GroupBox1;
    TEdit *Edit2;
    TButton *Button1;
    TButton *Button3;
    TButton *Button4;
    TButton *Button2;
    TCheckBox *CheckBox10;
    TPanel *palMUnloadRobotZBase;
    TPanel *palMUnloadRobotZ;
    TTMyTray *mtMUnloadRobotZ;
    TTMyTray *mtLoadPort1;
    TTMyTray *mtLoadPort2;
    TTMyTray *mtLoadPort3;
    TTMyTray *mtUnloadPort4;
    TTMyTray *mtUnloadPort1;
    TTMyTray *mtUnloadPort2;
    TTMyTray *mtUnloadPort3;
    TTMyTray *mtLoadPort4;
    TPanel *palMLoadRobotZBase;
    TPanel *palMLoadRobotZ;
    TTMyTray *mtMLoadRobotZ;
    TCheckBox *cbPort2;
    TCheckBox *cbPort3;
    TCheckBox *cbPort4;
    TCheckBox *cbPort1;
    TCheckBox *cbTrayMode;
    TCheckBox *cbCassetteMode;
    TCheckBox *cbCleanMode;
    TCheckBox *cbAllCassetteLoader;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall CheckBox10Click(TObject *Sender);
    void __fastcall mtCassette02MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbS10F5NoShowClick(TObject *Sender);
    void __fastcall cbQAModeClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Panel14MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall mtTrayEmptyMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall cbAutoModeClick(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall GroupBox1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbPort1Click(TObject *Sender);
    void __fastcall cbTrayModeClick(TObject *Sender);
    void __fastcall cbCassetteModeClick(TObject *Sender);
    void __fastcall cbCleanModeClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfCassette(TComponent* Owner);
    bool bShow;
    bool bSize;
    void MRSystemDataSave();
    void MRSystemDataRead();
    void MRSystemDataReadData();
    TStringList *listMRSystemData;
};
//---------------------------------------------------------------------------
extern PACKAGE TfCassette *fCassette;
//---------------------------------------------------------------------------
#endif
