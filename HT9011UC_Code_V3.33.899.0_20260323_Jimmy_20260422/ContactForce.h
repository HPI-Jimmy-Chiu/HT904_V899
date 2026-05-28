//---------------------------------------------------------------------------

#ifndef ContactForceH
#define ContactForceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Vector>

using namespace std;
//---------------------------------------------------------------------------
class THTSLKClass : public TComponent
{
    private:    // User declarations

    public:
        THTSLKClass(AnsiString Dia, int Tag, bool bDefault);

        TGroupBox *gbLoadRate;
        TLabel *lblDiameter;
        TLabel *lblHotOffset;
        TLabel *lblContactOffset;
        TLabel *lblDiameter_NS;
        TLabel *lblContactOffset_NS;//kevin 20170807 (Steven) NS offset
        TEdit *edtHotOffset;
        TTrackBar *trckbrDiameter;
        TTrackBar *trckbrDiameter_NS;
        TEdit *edtLoadRate;
        TEdit *edtLoadRate_NS;
        TEdit *edtContactOffset;
        TEdit *edtContactOffset_NS; //kevin 20170807 (Steven) NS offset
        AnsiString sDiameter;
        void __fastcall trckbrDiameter_Change(TObject *Sender);
        void __fastcall trckbrDiameter_NSChange(TObject *Sender);
        double dLoadRate;
        double dLoadRate_NS;
        double dDiameter;
        double dHotOffset;
        double dContactOffset;
        double dContactOffset_NS; //kevin 20170807 (Steven) NS offset
        double dMinForce;
        double dMaxForce;
        bool   bShow;
        int    iTag;
};
//---------------------------------------------------------------------------
class THTDieForceSLKClass : public TComponent
{
    private:    // User declarations

    public:
        THTDieForceSLKClass(AnsiString Dia, int Tag, bool bDefault);

        TGroupBox *gbDieForceLoadRate;
        TLabel *lblDieForceDiameter;
        TLabel *lblDieForceContactOffset;
        TTrackBar *trckbrDieForceDiameter;
        TEdit *edtDieForceLoadRate;
        TEdit *edtDieForceContactOffset;
        AnsiString sDiameter;
        void __fastcall trckbrDieForceDiameter_Change(TObject *Sender);
        double dLoadRate;
        double dDiameter;
        double dHotOffset;
        double dContactOffset;
        double dMinForce;
        double dMaxForce;
        bool   bShow;
        int    iTag;
};
//---------------------------------------------------------------------------
class THTDieForceOneByOneSLKClass : public TComponent
{
    private:    // User declarations

    public:
        THTDieForceOneByOneSLKClass(AnsiString Dia, int Tag, bool bDefault);

        TGroupBox *gbDieForceOneByOneLoadRate;
        TLabel *lblDieForceOneByOneDiameter;
        TLabel *lblDieForceOneByOneContactOffset;
        TTrackBar *trckbrDieForceOneByOneDiameter;
        TEdit *edtDieForceOneByOneLoadRate;
        TEdit *edtDieForceOneByOneContactOffset;
        AnsiString sDiameter;
        void __fastcall trckbrDieForceOneByOneDiameter_Change(TObject *Sender);
        double dLoadRate;
        double dDiameter;
        double dHotOffset;
        double dContactOffset;
        double dMinForce;
        double dMaxForce;
        bool   bShow;
        int    iTag;
};
//---------------------------------------------------------------------------
class THTSLKIndClass : public TComponent
{
    private:    // User declarations

    public:
        THTSLKIndClass(AnsiString Dia, int Tag, bool bDefault);

        TGroupBox *gbLoadRateInd;
        TLabel *lblDiameterInd;
        TLabel *lblContactOffsetInd;
        TTrackBar *trckbrDiameterInd;
        TEdit *edtLoadRateInd;
        TEdit *edtContactOffsetInd;
        AnsiString sDiameter;
        void __fastcall trckbrDiameterInd_Change(TObject *Sender);
        double dLoadRate;
        double dDiameter;
        double dContactOffset;
        double dMinForce;
        double dMaxForce;
        bool   bShow;
        int    iTag;
};
//---------------------------------------------------------------------------
class TfContactForce : public TForm
{
__published:    // IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *tsD25;
    TTabSheet *tsNewMethod;
    TGroupBox *GroupBox1;
    TLabel *lab30mm_10kg;
    TLabel *lab30mm_60kg;
    TLabel *lab30mm10kgNum;
    TLabel *lab30mm60kgNum;
    TTrackBar *tb30mm_10kg;
    TTrackBar *tb30mm_60kg;
    TGroupBox *GroupBox2;
    TLabel *lab40mm_10kg;
    TLabel *lab40mm_60kg;
    TLabel *lab40mm60kgNum;
    TLabel *lab40mm10kgNum;
    TTrackBar *tb40mm_10kg;
    TTrackBar *tb40mm_60kg;
    TGroupBox *GroupBox3;
    TLabel *lab60mm_10kg;
    TLabel *lab60mm_60kg;
    TLabel *lab60mm60kgNum;
    TLabel *lab60mm10kgNum;
    TTrackBar *tb60mm_10kg;
    TTrackBar *tb60mm_60kg;
    TPanel *Panel1;
    TButton *btSave;
    TButton *btExit;
    TLabel *labD25_1;
    TLabel *labD25_3;
    TLabel *labD25_2;
    TLabel *labD25_4;
    TLabel *labD25_5;
    TLabel *labD25_6;
    TTrackBar *tbD25_Index60mm;
    TTrackBar *tbD25_Index30mm;
    TTrackBar *tbD25_Index40mm;
    TEdit *edD25_60mm;
    TEdit *edD25_40mm;
    TEdit *edD25_30mm;
    TGroupBox *GroupBox4;
    TLabel *lab56mm_10kg;
    TLabel *lab56mm_60kg;
    TLabel *lab56mm60kgNum;
    TLabel *lab56mm10kgNum;
    TTrackBar *tb56mm_10kg;
    TTrackBar *tb56mm_60kg;
    TTrackBar *tbD60_Index56mm;
    TLabel *labD60;
    TLabel *labD60_1;
    TEdit *edD60_56mm;
    TTabSheet *tsDynamicKit;
    TScrollBox *scrlbxDynamicKit;
    TGroupBox *gbLoadRate;
    TLabel *lblDiameter;
    TLabel *lblHotOffset;
    TLabel *lblDiameter_NS;
    TTrackBar *trckbrDiameter;
    TEdit *edtHotOffset;
    TTrackBar *trckbrDiameter_NS;
    TGroupBox *grpAddNewKit;
    TEdit *edtLoadRate;
    TEdit *edtLoadRate_NS;
    TEdit *edtCurrentType;
    TEdit *edtVisible;
    TEdit *edtContactOffset;
    TLabel *lblContactOffset;
    TEdit *edtContactOffset_NS;
    TLabel *lblContactOffset_NS;
    TTabSheet *tsEPCalibration;
    TTabSheet *TabSheet1;
    TScrollBox *scrlbxDynamicKitInd;
    TGroupBox *gbLoadRateInd;
    TLabel *lblDiameterInd;
    TLabel *lblContactOffsetInd;
    TTrackBar *trckbrDiameterInd;
    TEdit *edtLoadRateInd;
    TEdit *edtContactOffsetInd;
    TGroupBox *grpAddNewKitInd;
    TEdit *edtCurrentTypeInd;
    TEdit *edtVisibleInd;
    TTabSheet *tsDieForceDynamicKit;
    TScrollBox *scrlbxDieForceDynamicKit;
    TGroupBox *gbDieForceLoadRate;
    TLabel *lblDieForceDiameter;
    TLabel *lblDieForceContactOffset;
    TTrackBar *trckbrDieForceDiameter;
    TEdit *edtDieForceLoadRate;
    TEdit *edtDieForceContactOffset;
    TGroupBox *grpDieForceAddNewKit;
    TEdit *edtDieForceCurrentType;
    TEdit *edtDieForceVisible;
    TTabSheet *tsContactHighOffset;
    TGroupBox *GroupBox5;
    TGroupBox *gbEPSetting;
    TLabel *Label11;
    TLabel *Label13;
    TEdit *edMaxKpa;
    TEdit *edMinMpa;
    TGroupBox *gbEPReadSetting;
    TLabel *Label12;
    TLabel *Label1;
    TEdit *edMaxMpaFB;
    TEdit *edtMinMpaFB;
    TGroupBox *grpEPForDieForce;
    TGroupBox *GroupBox6;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *edMaxKpaDual;
    TEdit *edMinMpaDual;
    TGroupBox *GroupBox7;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *edMaxMpaFBDual;
    TEdit *edtMinMpaFBDual;
    TGroupBox *grpEPForArm2;
    TGroupBox *gbEPSetting_1032;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *edMaxKpa_1032;
    TEdit *edMinMpa_1032;
    TGroupBox *gbEPReadSetting_1032;
    TLabel *Label8;
    TLabel *Label9;
    TEdit *edMaxMpaFB_1032;
    TEdit *edtMinMpaFB_1032;
    TGroupBox *grpATCTempOffset;
    TLabel *lbArm1;
    TLabel *lb4;
    TLabel *lbArm2;
    TEdit *edtArm1Offset_01;
    TEdit *edtArm1Offset_02;
    TEdit *edtArm1Offset_03;
    TEdit *edtArm1Offset_04;
    TEdit *edtArm1Offset_05;
    TEdit *edtArm1Offset_06;
    TEdit *edtArm1Offset_07;
    TEdit *edtArm2Offset_01;
    TEdit *edtArm2Offset_02;
    TEdit *edtArm2Offset_03;
    TEdit *edtArm2Offset_04;
    TEdit *edtArm2Offset_05;
    TEdit *edtArm2Offset_06;
    TEdit *edtArm2Offset_07;
    TEdit *edtArm1Offset_08;
    TEdit *edtArm1Offset_09;
    TEdit *edtArm1Offset_10;
    TEdit *edtArm1Offset_11;
    TEdit *edtArm2Offset_08;
    TEdit *edtArm2Offset_09;
    TEdit *edtArm2Offset_10;
    TEdit *edtArm2Offset_11;
    TEdit *edtArm1Offset_12;
    TEdit *edtArm1Offset_13;
    TEdit *edtArm1Offset_14;
    TEdit *edtArm2Offset_12;
    TEdit *edtArm2Offset_13;
    TEdit *edtArm2Offset_14;
    TEdit *edtArm1Offset_15;
    TEdit *edtArm2Offset_15;
    TGroupBox *GroupBox8;
    TLabel *Label10;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TEdit *edtMinVol;
    TEdit *edtMaxVol;
    TEdit *edtMidVol;
    TButton *Button1;
    TTabSheet *tsDieForceOneByOneKit;
    TScrollBox *scrlbxDieForceOneByOneDynamicKit;
    TGroupBox *gbDieForceOneByOneLoadRate;
    TLabel *lblDieForceOneByOneDiameter;
    TLabel *lblDieForceOneByOneContactOffset;
    TTrackBar *trckbrDieForceOneByOneDiameter;
    TEdit *edtDieForceOneByOneLoadRate;
    TEdit *edtDieForceOneByOneContactOffset;
    TGroupBox *grpDieForceOneByOneAddNewKit;
    TEdit *edtDieForceOneByOneCurrentType;
    TEdit *edtDieForceOneByOneVisible;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btExitClick(TObject *Sender);
    void __fastcall btSaveClick(TObject *Sender);
    void __fastcall tb30mm_10kgChange(TObject *Sender);
    void __fastcall edtNewDiameterClick(TObject *Sender);
    void __fastcall edtHotOffsetClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall edMaxKpaClick(TObject *Sender);
    void __fastcall edtContactOffsetClick(TObject *Sender);
    void __fastcall edtContactOffset_NSClick(TObject *Sender);
    void __fastcall edD25_60mmMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall edMinMpaClick(TObject *Sender);
    void __fastcall edMaxMpaFBClick(TObject *Sender);
    void __fastcall edtLoadRateIndClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
private:    // User declarations
    TEdit *pEmptyEdit;
public:     // User declarations
    __fastcall TfContactForce(TComponent* Owner);
    bool fShow;
    AnsiString FileName;
    bool bHasFile;
    void ShowValue();
    void ReadFile();
    void WriteFile();

    vector<THTSLKClass *>SLKClass;
    vector<THTSLKIndClass *>SLKIndClass;
    TStringList *slSLKTypeInd;
    TStringList *slSLKTypeIndVisible;
    TStringList *slSLKType;
    TStringList *slSLKTypeVisible;
    //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    //==>
    vector<THTDieForceSLKClass *>DieForceSLKClass;
    TStringList *slDieForceSLKType;
    TStringList *slDieForceSLKTypeVisible;
    //<==
    //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    //AI(ht9045-v899) 20260430: Multi EP half (INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI) per-site Die Force container.
    //   Half: 4 entries per Kit (Arm1_1, Arm1_2, Arm2_1, Arm2_2). iTag = i*4 + j, j=0..3.
    vector<THTDieForceOneByOneSLKClass *>DieForceOneByOneSLKClass;
    TStringList *slDieForceOneByOneSLKType;
    TStringList *slDieForceOneByOneSLKTypeVisible;
};
//---------------------------------------------------------------------------
extern PACKAGE TfContactForce *fContactForce;
//---------------------------------------------------------------------------
#endif
