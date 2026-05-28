//---------------------------------------------------------------------------

#ifndef cOffSetH
#define cOffSetH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
#include <StdCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TfOffSet : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsInOutArmOffset;
    TTabSheet *tsIndexOffset;
    TImage *Image1;
    TPanel *Panel5;
    TLabel *labWarningForStop;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TPanel *pnlPicker;
    TImage *Image2;
    TPanel *palOffsetParts;
    TPanel *pnlMain;
    TSpeedButton *btnToIndexOffset;
    TSpeedButton *sbLoader;
    TSpeedButton *sbAuto1;
    TSpeedButton *sbAuto2;
    TSpeedButton *sbAuto3;
    TSpeedButton *sbFix1;
    TSpeedButton *sbFix2;
    TSpeedButton *sbFix3;
    TSpeedButton *sbHp1;
    TSpeedButton *sbHp2;
    TSpeedButton *sbInSh1;
    TSpeedButton *sbInSh2;
    TSpeedButton *sbOutSh2;
    TSpeedButton *sbOutSh1;
    TSpeedButton *sbAutoClean;
    TPanel *Panel6;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *lblPickUp;
    TLabel *lblPitchX1;
    TLabel *lblRelease;
    TLabel *Label6;
    TEdit *edArmX;
    TEdit *edArmY;
    TEdit *edPickUp;
    TEdit *edPitchX1;
    TCheckBox *CheckBox1;
    TEdit *edRelease;
    TLabel *LabRelsA;
    TLabel *LabPickA;
    TLabel *LabRelsB;
    TLabel *LabPickB;
    TLabel *LabRelsD;
    TLabel *LabPickD;
    TLabel *LabRelsF;
    TLabel *LabPickF;
    TLabel *LabRelsH;
    TLabel *LabPickH;
    TLabel *LabRelsC;
    TLabel *LabPickC;
    TLabel *LabRelsE;
    TLabel *LabPickE;
    TLabel *LabRelsG;
    TLabel *LabPickG;
    TEdit *EdtRelsA;
    TEdit *EditPickA;
    TEdit *EdtRelsB;
    TEdit *EditPickB;
    TEdit *EdtRelsD;
    TEdit *EditPickD;
    TEdit *EdtRelsF;
    TEdit *EditPickF;
    TEdit *EdtRelsH;
    TEdit *EditPickH;
    TEdit *EdtRelsC;
    TEdit *EditPickC;
    TEdit *EdtRelsE;
    TEdit *EditPickE;
    TEdit *EdtRelsG;
    TEdit *EditPickG;
    TLabel *labWarningForZ;
    TSpeedButton *sbOCR;
    TSpeedButton *sbInRotate;
    TSpeedButton *sbOutRotate;
    TLabel *lblOutArm;
    TPanel *pan_AutoOffsetMove;
    TSpeedButton *sb_AutoOffsetDown;
    TSpeedButton *sb_AutoOffsetLeft;
    TSpeedButton *sb_AutoOffsetRight;
    TSpeedButton *sb_AutoOffsetUp;
    TCheckBox *cb_AutoOffsetPositionCheck;
    TLabel *lblReleaseRange;
    TLabel *lblPitchX1Range;
    TLabel *lblArmXRange;
    TLabel *lblArmYRange;
    TLabel *lblPickUpRange;
    TLabel *lblPitchY;
    TLabel *lblPitchYRange;
    TLabel *lblPitchX2;
    TLabel *lblPitchX2Range;
    TEdit *edPitchX2;
    TEdit *edPitchY;
    TPanel *Pnl_AOI;
    TSpeedButton *sbBGAView;
    TSpeedButton *sbPADView;
    TCheckBox *ck_AOI;
    TSpeedButton *sbLoaderB;
    TSpeedButton *sbInSh1LB;
    TSpeedButton *sbInSh1RA;
    TSpeedButton *sbInSh1RB;
    TSpeedButton *sbInSh2RA;
    TSpeedButton *sbInSh2RB;
    TSpeedButton *sbInSh2LB;
    TGroupBox *grpAutoClean;
    TGroupBox *GroupBox2;
    TSpeedButton *sbInSh1RA_AutoClean;
    TSpeedButton *sbInSh1RB_AutoClean;
    TSpeedButton *sbInSh1LB_AutoClean;
    TSpeedButton *sbInSh1_AutoClean;
    TGroupBox *GroupBox4;
    TSpeedButton *sbInSh2_AutoClean;
    TSpeedButton *sbInSh2RA_AutoClean;
    TSpeedButton *sbInSh2RB_AutoClean;
    TSpeedButton *sbInSh2LB_AutoClean;
    TSpeedButton *sbAutoSh2;
    TSpeedButton *sbAutoSh1;
    TSpeedButton *sbPreciser;
    TPanel *palPreciser;
    TLabel *lab;
    TLabel *Label12;
    TEdit *edPreciserOpen;
    TEdit *edPreciserClose;
    TLabel *Label17;
    TLabel *Label18;
    TSpeedButton *sbOutSh2RA;
    TSpeedButton *sbOutSh2RB;
    TSpeedButton *sbOutSh2LB;
    TSpeedButton *sbOutSh1RA;
    TSpeedButton *sbOutSh1RB;
    TSpeedButton *sbOutSh1LB;
    TTimer *TimerSetupTeach;
    TPanel *Pnl_ScanAOI;
    TSpeedButton *sbScanAOI;
    TCheckBox *ck_ScanAOI;
    TTimer *Timer1;
    TSpeedButton *sbInPlacement;
    TSpeedButton *btnBottom2D;
    TSpeedButton *sbAuto4;
    TSpeedButton *sbAuto5;
    TSpeedButton *sbAuto6;
    TSpeedButton *sbFix4;
    TSpeedButton *sbFix5;
    TSpeedButton *sbFix6;
    TPanel *pnlIndex;
    TSpeedButton *btnTrayOfsLoader;
    TSpeedButton *btnTrayOfsEmpty;
    TSpeedButton *btnTrayOfsColor;
    TSpeedButton *btnTrayOfsAuto1;
    TSpeedButton *btnTrayOfsAuto2;
    TSpeedButton *btnTrayOfsAuto3;
    TSpeedButton *IndexOffSetBT1;
    TSpeedButton *IndexOffSetBT2;
    TImage *Image3;
    TSpeedButton *btnToArmOffset;
    TSpeedButton *btnTrayOfsAuto4;
    TSpeedButton *btnTrayOfsAuto5;
    TSpeedButton *btnTrayOfsAuto6;
    TPanel *pnlIndexOfs;
    TLabel *lblShowMessage;
    TLabel *Label9;
    TPanel *Panel2;
    TLabel *Label7;
    TLabel *lblLoadZ;
    TEdit *edtTrayArmX;
    TEdit *edlLoadZ;
    TPanel *Panel3;
    TLabel *Label170;
    TLabel *Label171;
    TLabel *Label172;
    TLabel *Label26;
    TLabel *lblShtRight;
    TLabel *lblShtLeft;
    TLabel *labShtFor2D;
    TEdit *IndexArmOffSet2;
    TEdit *IndexArmOffSet3;
    TEdit *IndexArmOffSet1;
    TEdit *IndexArmOffSet4;
    TEdit *IndexArmOffSet5;
    TEdit *IndexArmOffSet6;
    TEdit *edShtFor2D;
    TPanel *pnlIndexOffset;
    TPanel *pnlTrayXART;
    TLabel *Label11;
    TEdit *edtARTPlace;
    TPanel *pnlOEE_Contact;
    TLabel *Label19;
    TLabel *lbFinalAirForce;
    TEdit *edOffsetContactForce;
    TTabSheet *tsArmOffset;
    TStringGrid *sgOffsetList;
    TSpeedButton *btnBack;
    TSpeedButton *btnOffsetList;
    TSpeedButton *sbTopView;
    TTabSheet *tsSetupTeach;
    TGroupBox *grpSetupTeach;
    TGroupBox *grpInArm;
    TPanel *palLoader;
    TPanel *palSht1;
    TPanel *palSht2;
    TPanel *palHP1;
    TPanel *palHP2;
    TGroupBox *grpOutArm;
    TPanel *palOutSht1;
    TPanel *palOutSht2;
    TPanel *palAuto1;
    TPanel *palAuto2;
    TPanel *palAuto3;
    TButton *btnSortAuto1;
    TButton *btnSortAuto2;
    TButton *btnSortAuto3;
    TLabel *LabPickO;
    TLabel *LabRelsO;
    TLabel *LabPickM;
    TLabel *LabRelsM;
    TLabel *LabPickK;
    TLabel *LabRelsK;
    TLabel *LabPickP;
    TLabel *LabRelsP;
    TLabel *LabPickN;
    TLabel *LabRelsN;
    TLabel *LabPickL;
    TLabel *LabRelsL;
    TLabel *LabPickJ;
    TLabel *LabRelsJ;
    TLabel *LabPickI;
    TLabel *LabRelsI;
    TEdit *EditPickO;
    TEdit *EdtRelsO;
    TEdit *EditPickM;
    TEdit *EdtRelsM;
    TEdit *EditPickK;
    TEdit *EdtRelsK;
    TEdit *EditPickP;
    TEdit *EdtRelsP;
    TEdit *EditPickN;
    TEdit *EdtRelsN;
    TEdit *EditPickL;
    TEdit *EdtRelsL;
    TEdit *EditPickJ;
    TEdit *EdtRelsJ;
    TEdit *EditPickI;
    TEdit *EdtRelsI;
    TLabel *lblPitchX3;
    TLabel *lblPitchX4;
    TEdit *edPitchX4;
    TEdit *edPitchX3;
    TPanel *palFix1;
    TPanel *palFix2;
    TPanel *palFix3;
    TButton *btnSortFix1;
    TButton *btnSortFix2;
    TButton *btnSortFix3;
    TPanel *grpOutArm456;
    TPanel *palAuto4;
    TPanel *palAuto5;
    TPanel *palAuto6;
    TPanel *palFix4;
    TPanel *palFix5;
    TPanel *palFix6;
    TButton *btnSortAuto4;
    TButton *btnSortAuto5;
    TButton *btnSortAuto6;
    TButton *btnSortFix4;
    TButton *btnSortFix5; 
    TButton *btnSortFix6;
        TEdit *EdtOffsetBX;
        TLabel *LabOffsetBX;
        TLabel *LabOffsetBY;
        TEdit *EdtOffsetBY;
        TLabel *Label5;
        TLabel *Label8;
        TLabel *LabOffsetAX;
        TEdit *EdtOffsetAX;
        TEdit *EdtOffsetAY;
        TLabel *LabOffsetAY;
        TEdit *EdtOffsetCY;
        TEdit *EdtOffsetCX;
        TLabel *LabOffsetCY;
        TLabel *LabOffsetCX;
        TEdit *EdtOffsetDY;
        TEdit *EdtOffsetDX;
        TLabel *LabOffsetDX;
        TLabel *LabOffsetDY;
        TEdit *EdtOffsetEX;
        TEdit *EdtOffsetEY;
        TLabel *LabOffsetEX;
        TLabel *LabOffsetEY;
        TEdit *EdtOffsetFX;
        TEdit *EdtOffsetFY;
        TLabel *LabOffsetFX;
        TLabel *LabOffsetFY;
        TEdit *EdtOffsetGX;
        TEdit *EdtOffsetGY;
        TLabel *LabOffsetGX;
        TLabel *LabOffsetGY;
        TEdit *EdtOffsetHX;
        TEdit *EdtOffsetHY;
        TLabel *LabOffsetHX;
        TLabel *LabOffsetHY;
        TEdit *EdtOffsetIX;
        TEdit *EdtOffsetIY;
        TEdit *EdtOffsetJX;
        TEdit *EdtOffsetJY;
        TLabel *LabOffsetJX;
        TLabel *LabOffsetJY;
        TLabel *LabOffsetIX;
        TLabel *LabOffsetIY;
        TEdit *EdtOffsetLX;
        TEdit *EdtOffsetLY;
        TLabel *LabOffsetLX;
        TLabel *LabOffsetLY;
        TEdit *EdtOffsetKX;
        TEdit *EdtOffsetKY;
        TLabel *LabOffsetKX;
        TLabel *LabOffsetKY;
        TEdit *EdtOffsetNX;
        TEdit *EdtOffsetNY;
        TLabel *LabOffsetNX;
        TLabel *LabOffsetNY;
        TEdit *EdtOffsetMX;
        TEdit *EdtOffsetMY;
        TLabel *LabOffsetMX;
        TLabel *LabOffsetMY;
        TEdit *EdtOffsetPX;
        TEdit *EdtOffsetPY;
        TLabel *LabOffsetPX;
        TLabel *LabOffsetPY;
        TEdit *EdtOffsetOX;
        TEdit *EdtOffsetOY;
        TLabel *LabOffsetOX;
        TLabel *LabOffsetOY;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall CheckBox1Click(TObject *Sender);
    void __fastcall edArmXKeyPress(TObject *Sender, char &Key);
    void __fastcall IndexOffSetBT2Click(TObject *Sender);
    void __fastcall edArmXMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall edReleaseMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall btnToIndexOffsetClick(TObject *Sender);
    void __fastcall btnToArmOffsetClick(TObject *Sender);
    void __fastcall IndexArmOffSet3MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sb_AutoOffsetUpClick(TObject *Sender);
    void __fastcall sb_AutoOffsetDownClick(TObject *Sender);
    void __fastcall sb_AutoOffsetRightClick(TObject *Sender);
    void __fastcall sb_AutoOffsetLeftClick(TObject *Sender);
    void __fastcall ck_AOIClick(TObject *Sender);
    void __fastcall IndexArmOffSet5MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edShtFor2DMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall TimerSetupTeachTimer(TObject *Sender);
    void __fastcall btnSortAuto1Click(TObject *Sender);
    void __fastcall edPreciserOpenMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall IndexArmOffSet1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall IndexArmOffSet1KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall IndexArmOffSet3KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall IndexArmOffSet3KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall IndexArmOffSet3Change(TObject *Sender);
    void __fastcall ck_ScanAOIClick(TObject *Sender);
    void __fastcall edOffsetContactForceMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edOffsetContactForceChange(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btnBackClick(TObject *Sender);
    void __fastcall btnOffsetListClick(TObject *Sender);
private:    // User declarations
    TSpeedButton *OffSetSelBot[OfsTotal];
    TSpeedButton *OfffSetSpecSelBot[trayOfsTotal];
    TEdit *MyPickEdit[2][8];
    TEdit *MyRelsEdit[2][8];
    TEdit *MyXOffsetEdit[2][8];
    TEdit *MyYOffsetEdit[2][8];
    TEdit *MySingleOffsetTEditX[2][8];                                          //Eastsun 20251224 : 吸嘴單獨offset功能UI陣列
    TEdit *MySingleOffsetTEditY[2][8];
    TLabel *MyPickLab[2][8];
    TLabel *MyRelsLab[2][8];
    TLabel *MySingleOffsetLabX[2][8];                                           //Eastsun 20251224 : 吸嘴單獨offset功能UI陣列
    TLabel *MySingleOffsetLabY[2][8];

    bool bUseUpdate;

    void __fastcall LoadImage(bool bInArm=true);
    void SetXYPitchVCLVisible(bool bVisible);
    bool __fastcall SaveSetupFile(AnsiString szDir,AnsiString szFilePath,int iSelPartData, int SpecialMode, bool bReset);//kevin 20150105
    double dTemp;                                                               //JimmyChiu 20220121 add
    void __fastcall GridEditExit();
public:     // User declarations
    __fastcall TfOffSet(TComponent* Owner);
    bool fShow;
    int iNowOffsetSel;
    bool bEnterSpecialOffset;
    AnsiString LastFileName;                                                    //kevin 20171117 change
    int iSpecialOffSetSel;                                                      //kevin 20181112
    int iIndexChange;                                                           //kevin 20211211 index socket value change
    int iOffsetMap[OfsTotal];

    void __fastcall SpBotSelClick(TObject *Sender);
    void __fastcall ShowOneByOneOffSet();
    void __fastcall ShowOffSetList();
    bool __fastcall SaveFile(int iSelPartData, int SpecialMode, bool bReset);
    void __fastcall ReadFile();
    void __fastcall DoIniDataToForm(int iNowOffsetSel, int SpecialMode);
    void __fastcall ClearIndexOffset();
    bool __fastcall UseAutoOffsetFunction(AnsiString asWhich);
    bool __fastcall UseInArmSetupTeach(int iArea);                              //JerryYang 20180921 Setup Teach功能
    bool __fastcall UseOutArmSetupTeach(int iArea);
    AnsiString GetOffsetPath(AnsiString FileFolder="");                         //Steven 20190109 : 整合Offset路徑
    void ShowFinalAirForce();                                                   //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
    AnsiString Tri_Position_Offset();                                           //Ztex 2024.03.29 Tri Temp Position Offset
};
//---------------------------------------------------------------------------
extern PACKAGE TfOffSet *fOffSet;
extern AnsiString CapStr[];                                                     //kevin 20171121
extern const int iSaveStander;                                                  //kevin 20181112 add
extern const int iSaveSpecial;
extern const int iSaveAll;
extern AnsiString CapStrInput[InOfsTotal];
extern AnsiString CapStrOutput[OutOfsTotal];
//---------------------------------------------------------------------------
#endif
