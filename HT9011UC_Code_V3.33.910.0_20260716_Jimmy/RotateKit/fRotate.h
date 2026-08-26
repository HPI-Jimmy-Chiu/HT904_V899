//---------------------------------------------------------------------------

#ifndef fRotateH
#define fRotateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <jpeg.hpp>
#include <Buttons.hpp>
#include "HTray.h"
#include <ComCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------

//const int MaxDutType =2;
//const int MaxDut_Col = 8;   //Ifor 20170407 4 => 8
//const int MaxDut_Row = 4;
enum {tDutType_4=0,
      tDutType_8=1,
      tDutType_1=2,
      tDutType_2=3,
      tDutType_Total};
//---------------------------------------------------------------------------
struct TRotate
{
    bool ActiveRotate;
    bool bUseDifferentAngle;
    double FromTrayAngle;
    int RotateDutDate[4][4][8];                                                 //Ifor 20170406 add In/Out Rotate  //0:In 1:out 2:In RT 3:Out RT
    int DutNum;
    int ColCount;
    int RowCount;
    int RotationTimeIn;                                                         //Ifor 20170414 (Steven) RotationTime ==> RotationTimeIn
    int RotationTimeOut;                                                        //Ifor 20170414 (Steven) add Out Rotate 臂锣Ω计
//    int iRotateDegree_In[4][8];
//    int iRotateDegree_Out[4][8];
    int RotationCount[8];                                                       //程à琌90,360/90 程4Ω    //Ifor 20170407 (Steven) 4 => 8
    int iCurrentRotDegree_In[2][4];
    int iCurrentRotDegree_Out[2][4];

    double RotationSequence[4][8];
    double RotateKit_PitchX;
    double RotateKit_PitchY;
    int OutRotationCount[8];                                                    //kevin 20131003 out rotate 程à琌90,360/90 程4Ω   //Ifor 20170407 4 => 8

    bool bPassBinNoRotate;                                                      //jou 20231020 : Pass bin no rotate
    bool bRotateUseRTmode;                                                      //jou 20231122 : Rotate Use RT mode
    int RotationTimeInRT;                                                       //jou 20231122 : Rotate Use RT mode
    int RotationTimeOutRT;                                                      //jou 20231122 : Rotate Use RT mode
    int RotationCountRT[8];                                                     //jou 20231122 : Rotate Use RT mode
    int OutRotationCountRT[8];                                                  //jou 20231122 : Rotate Use RT mode
    bool bART_RT_NoRotate;                                                      //Sam 20240809 : ART RT No Rotate
    int iRotateOffset[2];                                                       //Ifor 20251210 add:Rotate Offset
};

class TFrmRotate : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TRadioGroup *rgDutNum;
    TRadioGroup *rgSelect;
    TGroupBox *GroupBox1;
    TStringGrid *sgFTA;
    TSpeedButton *spbSave;
    TSpeedButton *sbtExit;
    TCheckBox *cbActiveRotate;
    TLabel *Label_Err;
    TTMyTray *tmyRotatePosion;
    TLabel *LabelRotatePositon;
    TPageControl *pgcRotate;
    TTabSheet *tsM1RotateIn;
    TTabSheet *tsM8RotateIn;
    TStringGrid *sgRotateIn;
    TTabSheet *tsM1RotateOut;
    TTabSheet *tsM8RotateOut;
    TCheckBox *cbOutRotateDifferentAngle;
    TStringGrid *sgRotateOut;
    TEdit *Edit1;
    TCheckBox *chkPassBinNoRotate;
    TTabSheet *tsM1RotateInRT;
    TTabSheet *tsM1RotateOutRT;
    TStringGrid *sgRotateInRT;
    TStringGrid *sgRotateOutRT;
    TCheckBox *chkRotateUseRTmode;
    TCheckBox *chkART_RTNoRotate;
    TLabeledEdit *YPitch;
    TLabeledEdit *XPitch;
    TGroupBox *grpRotateOffset;
    TLabel *Label1;
    TEdit *edInRotate_0;
    TEdit *edOutRotate_0;
    TLabel *Label5;
    TLabel *Label6;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgRotateInDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall rgDutNumClick(TObject *Sender);
    void __fastcall sgRotateInSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall sgFTADrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall sgFTASelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall spbSaveClick(TObject *Sender);
    void __fastcall sbtExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall XPitchKeyPress(TObject *Sender, char &Key);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall rgSelectClick(TObject *Sender);
    void __fastcall cbOutRotateDifferentAngleClick(TObject *Sender);
    void __fastcall YPitchMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
    void __fastcall chkRotateUseRTmodeClick(TObject *Sender);
    void __fastcall edInRotate_0MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
public:     // User declarations
    __fastcall TFrmRotate(TComponent* Owner);
    bool fShow;
    void SetGridDraw();
    bool bIsAngleZero;                                                           //Eastsun 20260515 F022: D12
    void DrawState(TStringGrid *lo,int x,int y,TRect &Rect);
    int  iRotateDutDate[4][4][8];                                               //Ifor 20170406 add In/Out Rotate
    int  iFromTrayAngle;
    void fRotate_ReadFile();
    void DoIniDataToForm();
    bool bShowRotateBySite;

    bool b8RotateInHome[2][4];                                                  //Steven 20170329 : Add individual rotate motor
    bool b8RotateOutHome[2][4];                                                 //Steven 20170329 : Add individual rotate motor
    bool bRotateInHome;                                                         //kevin 20130706 rotat single home
    bool bRotateOutHome;                                                        //kevin 20130706 rotat single home
    void InitialInRotateHome();                                                 //Steven 20170329 : Add individual rotate motor
    void InitialOutRotateHome();                                                //Steven 20170329 : Add individual rotate motor
    bool DoInRotateHome();                                                      //Steven 20170329 : Add individual rotate motor
    bool DoOutRotateHome();                                                     //Steven 20170329 : Add individual rotate motor
    void SetInRotateSpeed(int iACDCSP, int iBodySP);                            //Steven 20170329 : Add individual rotate motor
    void SetOutRotateSpeed(int iACDCSP, int iBodySP);                           //Steven 20170329 : Add individual rotate motor
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmRotate *FrmRotate;
extern TRotate tRotate;
//---------------------------------------------------------------------------
#endif
