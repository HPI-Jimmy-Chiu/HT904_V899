//---------------------------------------------------------------------------

#ifndef cTrayFormH
#define cTrayFormH
//---------------------------------------------------------------------------

#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "HTray.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
const int MAX_COLORSENSOR_NUM=16;                                               //defined by MU-N//Jimmychiu 20230630 : add color sensor MU-N in Loader
//---------------------------------------------------------------------------
class uColorSensorInfo                                                          //Jimmychiu 20230630 : add color sensor MU-N in Loader
{
private:                                                                        // User declarations
    AnsiString asTemp;
public:                                                                         // User declarations
    uColorSensorInfo(AnsiString _asName,AnsiString *_asArrBooleanData,TGroupBox *gbUseLoaderColorSensor,int iTop,int iLeft);
    ~uColorSensorInfo();
    TCheckBox* cbLoaderColorSensorMUN[MAX_COLORSENSOR_NUM];
    TEdit *edEnable;
    AnsiString asName;
    AnsiString *asArrBooleanData;
    void SetCB_Name(AnsiString asCommaText);
    void LoadColorSensorEnable();
    void SaveColorSensorEnable();
    bool IsColorEable(int iInput,AnsiString &sErrorMsg);
    void SetColorEnable(AnsiString sColor,bool bEnable);
    void SetAllEnable(bool bEnable);
    AnsiString GetColorName(int iIndex);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfTrayForm : public TForm
{
__published:                                                                    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TPanel *Panel1;
    TLabel *Label1;
    TEdit *TrayName1;
    TPanel *Panel2;
    TImage *Image1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TEdit *XST1;
    TEdit *XPitch1;
    TEdit *XCT1;
    TEdit *YST1;
    TEdit *YPitch1;
    TEdit *YCT1;
    TEdit *Tp1Thick;
    TEdit *Tp1TickUp;
    TPanel *Panel3;
    TLabel *Label10;
    TEdit *TrayName2;
    TPanel *Panel5;
    TLabel *Label19;
    TEdit *TrayName3;
    TPanel *Panel4;
    TImage *Image2;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TEdit *XST2;
    TEdit *XPitch2;
    TEdit *XCT2;
    TEdit *YST2;
    TEdit *YPitch2;
    TEdit *YCT2;
    TEdit *Tp2Thick;
    TEdit *Tp2TickUp;
    TPanel *Panel6;
    TImage *Image3;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TEdit *XST3;
    TEdit *XPitch3;
    TEdit *XCT3;
    TEdit *YST3;
    TEdit *YPitch3;
    TEdit *YCT3;
    TEdit *Tp3Thick;
    TEdit *Tp3TickUp;
    TGroupBox *GroupBox1;
    TImage *Image4;
    TTMyTray *TMyTray2;
    TTMyTray *TMyTray1;
    TTMyTray *TMyTray3;
    TImage *Image5;
    TImage *Image6;
    TEdit *edMemo1;
    TEdit *edMemo2;
    TEdit *edMemo3;
    TComboBox *cbCopyFrom;
    TComboBox *cbTrayType3;
    TComboBox *cbTrayType2;
    TComboBox *cbTrayType1;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TSpeedButton *spbCopy;
    TTabSheet *tsBinBox;
    TImage *imgDevice180;
    TImage *imgDevice90;
    TImage *imgDevice270;
    TImage *imgTray0;
    TImage *imgTray180;
    TImage *imgDevice0;
    TPanel *pnlBixBox;
    TLabel *Label31;
    TLabel *lblBinBoxAlarm;
    TLabel *Label32;
    TLabel *Label33;
    TEdit *edtBinBoxName;
    TEdit *edtBinBoxAlarm;
    TEdit *edtBinBoxNow;
    TButton *btnBinBoxReset;
    TRadioGroup *rgTrayDir;
    TRadioGroup *rgDeviceDir;
    TGroupBox *gb_TrayBlock1;
    TLabel *Label34;
    TLabel *Label35;
    TEdit *XBStart1;
    TEdit *XBP1;
    TLabel *Label36;
    TEdit *YBStart1;
    TLabel *Label37;
    TEdit *YBP1;
    TImage *Image7;
    TLabel *Lable1;
    TLabel *Lable2;
    TEdit *XBItem1;
    TEdit *YBItem1;
    TGroupBox *gb_TrayBlock2;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TImage *Image8;
    TLabel *Label46;
    TLabel *Label47;
    TEdit *XBStart2;
    TEdit *XBP2;
    TEdit *YBStart2;
    TEdit *YBP2;
    TEdit *XBItem2;
    TEdit *YBItem2;
    TGroupBox *gb_TrayBlock3;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TLabel *Label45;
    TImage *Image9;
    TLabel *Label48;
    TLabel *Label49;
    TEdit *XBStart3;
    TEdit *XBP3;
    TEdit *YBStart3;
    TEdit *YBP3;
    TEdit *XBItem3;
    TEdit *YBItem3;
    TEdit *YBTypeSize1;
    TLabel *Label50;
    TEdit *YBTypeSize2;
    TLabel *Label51;
    TEdit *YBTypeSize3;
    TLabel *Label52;
    TCheckBox *chkEnableIndicator;
    TImage *imgTray0_Flip;
    TImage *imgTray180_Flip;
    TLabel *Lab2XPickup;
    TLabel *Lab1XPickup;
    TLabel *Lab3XPickup;
    TTabSheet *tsColorSensor;
    TGroupBox *gbUseLoaderColorSensor;
    TLabel *lbRT_LCS;
    TLabel *lbFT_LCS;
    TCheckBox *cbUseThickTray;
    TPanel *pnlUnloadTrayCount;
    TLabel *Label53;
    TEdit *edUnloadTrayCount;
    TTabSheet *TabSheet4;
    TPanel *Panel7;
    TLabel *labYStart;
    TLabel *labYPitch;
    TLabel *labYDivision;
    TEdit *edBoatYStart;
    TEdit *edBoatYPitch;
    TEdit *edBoatYDivision;
    TImage *imgBoatForm;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall PageControl1Change(TObject *Sender);
    void __fastcall XST1KeyPress(TObject *Sender, char &Key);
    void __fastcall XCT1KeyPress(TObject *Sender, char &Key);
    void __fastcall YST1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall XCT1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall YCT1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall cbTrayType1Change(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall spbCopyClick(TObject *Sender);
    void __fastcall edtBinBoxAlarmKeyPress(TObject *Sender, char &Key);
    void __fastcall edtBinBoxAlarmMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnBinBoxResetClick(TObject *Sender);
    void __fastcall rgDeviceDirClick(TObject *Sender);
    void __fastcall rgTrayDirClick(TObject *Sender);
    void __fastcall TrayName1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Tp1TickUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall XPitch1Click(TObject *Sender);
    void __fastcall edUnloadTrayCountMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall YPitch1Click(TObject *Sender);
private:                                                                        // User declarations
    TEdit *TrayEdit[3][17];                                                     //2014-03-04    Dell    10->14 (for SPIL WLP Add Tray Block)

    void __fastcall LoadImage();
    //Jimmychiu 20230630 : add color sensor MU-N in Loader
    //<==
    void InitialColorSensorGUI();                                               //Jimmychiu 20230630 : add color sensor MU-N in Loader
    void LoadColorSensorMapping();                                              //Jimmychiu 20230630 : add color sensor MU-N in Loader
    void LoadColorSensorEnable();                                               //Jimmychiu 20230630 : add color sensor MU-N in Loader
    void SaveColorSensorEnable();                                               //Jimmychiu 20230630 : add color sensor MU-N in Loader
    /************************************/
    std::map<int,uColorSensorInfo*> mapCSInfo;
    void AddColorSensor(AnsiString asName,AnsiString *_asArrBooleanData,int iTop,int iLeft);
    int GetColorSensorSize(){return mapCSInfo.size();}
    void DeleteColorSensor(){mapCSInfo.erase(GetColorSensorSize()-1);}
    void ClearAllColorSensor();
    /************************************/
    //<==
    //Jimmychiu 20230630 : add color sensor MU-N in Loader
public:                                                                         // User declarations
    __fastcall TfTrayForm(TComponent* Owner);
    void __fastcall ReadFile();
    void __fastcall UpDateType();
    void __fastcall ShowTypePage(int iTag, int index);
    void __fastcall SaveSetupFile(AnsiString szDir,AnsiString S);               //kevin 20180926
    void __fastcall DoIniDataToForm();                                          //kevin 20181109
    bool fShow;
    //Jimmychiu 20230630 : add color sensor MU-N in Loader
    //<==
    uColorSensorInfo* GetColorSensor(int iIndex);
    uColorSensorInfo* GetColorSensor(AnsiString asName);
    AnsiString asErrorMsg;
    bool IsEnableColorSensor();
    //<==
    //Jimmychiu 20230630 : add color sensor MU-N in Loader
};
//---------------------------------------------------------------------------
extern PACKAGE TfTrayForm *fTrayForm;
//---------------------------------------------------------------------------
#endif
