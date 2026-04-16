//---------------------------------------------------------------------------

#ifndef MagazineH
#define MagazineH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "HTray.h"
//---------------------------------------------------------------------------
class TfMagazine : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlMagazineTrayOut;
    TLabel *Label15;
    TCheckBox *chkMag1;
    TCheckBox *chkMag2;
    TCheckBox *chkMag3;
    TCheckBox *chkMag4;
    TCheckBox *chkMag5;
    TCheckBox *chkMag6;
    TCheckBox *chkMag7;
    TCheckBox *chkMag8;
    TCheckBox *chkMag9;
    TCheckBox *chkMag10;
    TCheckBox *chkMag11;
    TCheckBox *chkMag12;
    TCheckBox *chkMag13;
    TCheckBox *chkMag14;
    TSpeedButton *sbtExit;
    TSpeedButton *spbSave;
    TRadioGroup *rgMagTraySource;
    TGroupBox *GroupBox1;
    TEdit *edInMagOfs16;
    TEdit *edInMagOfs15;
    TEdit *edInMagOfs14;
    TEdit *edInMagOfs13;
    TEdit *edInMagOfs12;
    TEdit *edInMagOfs11;
    TEdit *edInMagOfs10;
    TEdit *edInMagOfs09;
    TEdit *edInMagOfs08;
    TEdit *edInMagOfs07;
    TEdit *edInMagOfs06;
    TEdit *edInMagOfs05;
    TEdit *edInMagOfs04;
    TEdit *edInMagOfs03;
    TEdit *edInMagOfs02;
    TEdit *edInMagOfs01;
    TEdit *edOutAuto3Ofs01;
    TEdit *edOutAuto3Ofs02;
    TEdit *edOutAuto3Ofs03;
    TEdit *edOutAuto3Ofs04;
    TEdit *edOutAuto3Ofs05;
    TEdit *edOutAuto3Ofs06;
    TEdit *edOutAuto3Ofs07;
    TEdit *edOutAuto3Ofs08;
    TEdit *edOutAuto3Ofs09;
    TEdit *edOutAuto3Ofs10;
    TEdit *edOutAuto3Ofs11;
    TEdit *edOutAuto3Ofs12;
    TEdit *edOutAuto3Ofs13;
    TEdit *edOutAuto3Ofs14;
    TEdit *edOutAuto3Ofs15;
    TEdit *edOutAuto3Ofs16;
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
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TRadioGroup *rgMagFixType;
    TGroupBox *GroupBox2;
    TLabel *labSlot1Bin;
    TLabel *labSlot2Bin;
    TLabel *labSlot3Bin;
    TLabel *labSlot4Bin;
    TLabel *labSlot5Bin;
    TLabel *labSlot6Bin;
    TLabel *labSlot7Bin;
    TLabel *labSlot8Bin;
    TLabel *labSlot9Bin;
    TLabel *labSlot10Bin;
    TLabel *labSlot11Bin;
    TLabel *labSlot12Bin;
    TLabel *labSlot13Bin;
    TLabel *labSlot14Bin;
    TGroupBox *gbBinBuffer;
    TLabel *labSlot1;
    TLabel *labSlot2;
    TLabel *labSlot3;
    TLabel *labSlot4;
    TLabel *labSlot5;
    TLabel *labSlot6;
    TLabel *labSlot7;
    TLabel *labSlot8;
    TLabel *labSlot9;
    TLabel *labSlot10;
    TLabel *labSlot11;
    TLabel *labSlot12;
    TLabel *labSlot13;
    TLabel *labSlot14;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TTMyTray *mtTrayBuffer1;
    TTMyTray *mtTrayBuffer2;
    TTMyTray *mtTrayBuffer3;
    TSpeedButton *btnMagazineTrayOut;
    TRadioGroup *rgMagDisplayOrder;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall edInMagOfs01Click(TObject *Sender);
    void __fastcall rgMagFixTypeClick(TObject *Sender);
    void __fastcall rgMagTraySourceClick(TObject *Sender);
    void __fastcall btnMagazineTrayOutClick(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfMagazine(TComponent* Owner);
    bool fShow;
    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm();
    TEdit *EditInMag[16];
    TEdit *EditOutAuto[16];
    int iInMagOfs[16];
    int iOutAutoOfs[16];
};
//---------------------------------------------------------------------------
extern PACKAGE TfMagazine *fMagazine;

extern bool CheckPlaceToMagazineTray(int iWhichTray);
extern bool CheckPlaceToBufferTray(int iWhichMagazine);

extern void InitialDoPickFromMagazineBuffer();
extern bool DoPickFromMagazineBuffer();
//extern void InitialDoPlaceToMagazineBuffer();
//extern bool DoPlaceToMagazineBuffer();
extern void InitialAuto3MagazineTask();
extern void DoAuto3Magazine();
extern void InitialMagazineUpDown();
extern void InitialMagazineCheck();
extern bool DoMagazineTrayFeed(bool bSelect=false);
extern void InitialCatchTrayChangeTray();
extern void InitialCatchTrayGetNewTray();
extern bool DoCatchTrayGetNewTray(int iWitchTray);
extern bool DoCatchTrayChangeTray(int iWitchTray);
extern bool DoMagazineUpDown(int iWitchTray, int iType);                        // 2012.11.05 , Joye , HT-7046
extern bool SetOutArm_9045_Pick_Buffer();
extern void InitialTakeOffAuto3TrayCover();
extern void InitialDoMagazineTrayFeedTask();
extern bool DoBufferTraySkipProcess_9045();
extern bool CheckBuffIsSameBin();
extern bool DoMagazineInitCheck();
extern bool bCheckMagazineSafeSensor();                                         //Sam 20221116 : Magazine TrayArm ¦Û°Ê¸É Tray
extern void InitialDoMagazineScanHasTray();
extern bool DoMagazineScanHasTray();
//---------------------------------------------------------------------------
#endif
