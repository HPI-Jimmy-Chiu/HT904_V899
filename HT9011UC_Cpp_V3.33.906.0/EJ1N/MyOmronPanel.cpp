// =============================================================================
//  MyOmronPanel.cpp  --  TMyOmronPanel, a custom VCL panel component (see
//                         this unit's own MyOmronPanel.h banner for the full
//                         ROLE / ZERO CONSUMERS / GATE REGISTER writeup --
//                         not repeated here in full, only re-cited at each
//                         call site).
//
//  Faithful translation of golden EJ1N/MyOmronPanel.cpp (222 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 group "ej1n".  Third EJ1N/ file to land.
//
//  WAVE SCOPE -- ACTIVE vs GATED (golden line numbers; see MyOmronPanel.h
//  for the full 3-gate rationale -- restated briefly at each site below):
//   TMyOmronPanel::TMyOmronPanel   golden :10-65  -- ACTIVE: widget alloc +
//     the 8 SetXxx display calls (now state-only) + dPv=dSettingSV=0.0.
//     GATED: every Left/Top/Width/Height/Parent/Font/BorderStyle/AutoSize/
//     ParentFont/DragKind/OnMouseUp/OnMouseDown/OnMouseMove/OnClick
//     assignment and the TImage allocation (GATE 1+2).
//   TMyOmronPanel::~TMyOmronPanel  golden :67-69  -- ACTIVE, EXTENDED: golden
//     body is empty (relies on real VCL's TComponent-owns-children auto free,
//     which this port's vclcompat::TComponent does not implement -- it has
//     no owned-children list at all, see vclcompat/Comm.h). Explicitly
//     deletes GroupBox/edSV/cbEnableAT here instead of reproducing an
//     unreachable-in-this-substrate "leak nothing ever frees" -- same
//     correctness fix already applied by this wave's OWN substrate
//     (vclcompat/Controls.h's TMemo/TComboBox/TListBox dtors, each flagged
//     "AI(W906-F0fix) 20260728: LOW-6, was never freed... freed now for
//     correctness"). Zero consumers anywhere ever construct a
//     TMyOmronPanel, so this is unreachable in practice either way; recorded
//     for whichever future wave does exercise it.
//   TMyOmronPanel::GroupBox1MouseUp/MouseDown/MouseMove  golden :71-103  --
//     GATE (2): omitted entirely, see .h banner GATE (2).
//   TMyOmronPanel::SetPV           golden :105-119 -- ACTIVE: `dPv=atof(...)`.
//     GATED: the Canvas paint (GATE 1).
//   TMyOmronPanel::SetDegree/SetRunStop/SetAT(AnsiString,TColor)/
//     SetInputErr/SetEvent/SetSP  golden :121-191 -- 100% GATED (GATE 1):
//     each is PURE Canvas paint with no other state to preserve.
//   TMyOmronPanel::SetBG           golden :193-198 -- 100% GATED (GATE 1).
//   TMyOmronPanel::SetVisible      golden :200-203 -- ACTIVE, real
//     (vclcompat::TControl::Visible, inherited by TGroupBox, is real).
//   TMyOmronPanel::SetSettingSV    golden :205-209 -- ACTIVE: dSettingSV
//     assignment + edSV->Text write (vclcompat::TCustomEdit::Text is real).
//   TMyOmronPanel::SetSV           golden :211-215 -- ACTIVE: dSV assignment
//     + the SetSP(...) call (itself 100% gated, see above).
//   TMyOmronPanel::setEditValueClick golden :217-220 -- GATE (3): fQwertyKey
//     has no port (established deferred surface, see .h banner GATE 3).
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :2 and :8). `__fastcall` dropped from every definition.
//  `myQwertyKeyBoard.h` (golden :5) not included -- its only referenced
//  symbol, fQwertyKey, is gated (GATE 3); `cmydef.h` (golden :6) IS included,
//  for the real N_INTEGER constant cited (unreachable) inside that same gate.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "MyOmronPanel.h"
//   #include "myQwertyKeyBoard.h"   -- NOT included, see banner above
//   #include "cmydef.h" ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella)
#include "EJ1N/MyOmronPanel.h"      // this unit's own contract
#include "cmydef.h"                 // N_INTEGER (real) -- cited inside GATE (3) only
//---------------------------------------------------------------------------
TMyOmronPanel::TMyOmronPanel(TComponent* Owner) : TComponent(Owner)
{
    GroupBox        =new TGroupBox();
    edSV            =new TEdit();                                           //設定值
    cbEnableAT      =new TCheckBox();

    //AI(W906-PT-W2) 20260807: GATE (1)+(2) -- golden's TImage allocation,
    //  every widget geometry/style/Font/BorderStyle/AutoSize/ParentFont/
    //  DragKind property, and the 3 mouse-event + 1 click-event wirings
    //  (golden :14,17-30,32,42-55,57-63). None of Left/Top/Width/Height/
    //  Parent/Font/BorderStyle/AutoSize/ParentFont/DragKind exist on
    //  vclcompat::TControl by design (see MyOmronPanel.h banner GATE (1));
    //  GroupBox1MouseUp/Down/Move (GATE (2)) and setEditValueClick's OnClick
    //  wiring have no callee signature this tree can express / no callee
    //  substrate (GATE (3)) respectively. Preserved verbatim:
#if 0
    ImgOmronPanel   =new TImage(this);

    GroupBox->Left      =0;
    GroupBox->Top       =0;
    GroupBox->Width     =206;
    GroupBox->Height    =120;
    GroupBox->DragKind  =dkDock;
    GroupBox->OnMouseUp =GroupBox1MouseUp;
    GroupBox->OnMouseDown=GroupBox1MouseDown;
    GroupBox->OnMouseMove=GroupBox1MouseMove;

    ImgOmronPanel->Parent=GroupBox;
    ImgOmronPanel->Top=20;
    ImgOmronPanel->Left=4;
    ImgOmronPanel->Width=197;
    ImgOmronPanel->Height=95;

    ImgOmronPanel->Canvas->Font->Name ="Arial";
#endif
    SetBG        (clGray, clGray);                                              //設定背景
    SetPV        ("##.##",       clYellow);                                     //顯示量測值
    SetDegree    ("℃",          clYellow);                                     //顯示量測單位
    SetRunStop   ("STOP",        clBlack);                                      //顯示目前運作模式
    SetAT        ("AT",          clBlack);                                      //顯示AT是否啟動
    SetInputErr  ("Input Error", clBlack);                                      //感溫線異常
    SetEvent     ("Event",       clBlack);                                      //警報事件
    SetSP        ("SP",          TColor(0x004080FF));                           //目前設定值

#if 0
    edSV->Parent=GroupBox;
    edSV->AutoSize=false;
    edSV->Left = 72;
    edSV->Top = 85;
    edSV->Width = 128;
    edSV->Height = 28;
    edSV->BorderStyle=bsNone;
#endif
    edSV->Text = "0";
#if 0
    edSV->Font->Charset = DEFAULT_CHARSET;
    edSV->Font->Color = clBlack;
    edSV->Font->Size = 18;
    edSV->Font->Name = "Arial";
    edSV->ParentFont = false;
    edSV->OnClick=setEditValueClick;
#endif

#if 0
    cbEnableAT->Parent  =GroupBox;
    cbEnableAT->Left    =48;
    cbEnableAT->Top     =76;
    cbEnableAT->Width   =17;
    cbEnableAT->Height  =17;
    cbEnableAT->Color   =clGray;
#endif
    cbEnableAT->Caption ="";
    dPv=dSettingSV=0.0;
}
//---------------------------------------------------------------------------
TMyOmronPanel::~TMyOmronPanel()
{
    //AI(W906-PT-W2) 20260807: golden body is empty (golden :67-69), relying
    //  on real VCL's TComponent-owns-children auto-free; this port's
    //  vclcompat::TComponent has no owned-children list to reproduce that
    //  with (see MyOmronPanel.h banner, ~TMyOmronPanel entry). Explicitly
    //  freeing the 3 real substrate widgets here instead of leaving them
    //  permanently unreachable -- same correctness-fix class already applied
    //  by vclcompat/Controls.h's own TMemo/TComboBox/TListBox dtors.
    delete GroupBox;
    delete edSV;
    delete cbEnableAT;
}
//---------------------------------------------------------------------------
//AI(W906-PT-W2) 20260807: GATE (2) -- golden GroupBox1MouseUp/
//  GroupBox1MouseMove/GroupBox1MouseDown (golden :71-103) omitted entirely:
//  their `TMouseButton`/`TShiftState` parameter types have no port anywhere
//  in this tree. See MyOmronPanel.h banner GATE (2) for the full citation.
//  Golden bodies kept here verbatim, in comment form, for provenance:
//
//  void __fastcall TMyOmronPanel::GroupBox1MouseUp(TObject *Sender,
//        TMouseButton Button, TShiftState Shift, int X, int Y)
//  {
//      TGroupBox *Ptr;
//      Ptr=(TGroupBox *)Sender;
//      if(Ptr->Tag!=0)
//      {
//          Ptr->Tag=0;
//      }
//  }
//  void __fastcall TMyOmronPanel::GroupBox1MouseMove(TObject *Sender,
//        TShiftState Shift, int X, int Y)
//  {
//      TGroupBox *Ptr;
//      Ptr=(TGroupBox *)Sender;
//      if(Ptr->Tag==1)
//      {
//          Ptr->Left=Ptr->Left+(X-iStartX);
//          Ptr->Top =Ptr->Top+(Y-iStartY);
//      }
//  }
//  void __fastcall TMyOmronPanel::GroupBox1MouseDown(TObject *Sender,
//        TMouseButton Button, TShiftState Shift, int X, int Y)
//  {
//      TGroupBox *Ptr;
//      Ptr=(TGroupBox *)Sender;
//      if(Ptr->Tag!=1)
//          Ptr->Tag=1;
//      iStartX=X;
//      iStartY=Y;
//  }
//---------------------------------------------------------------------------
void TMyOmronPanel::SetPV(AnsiString PV, TColor Color)               //顯示量測值
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- golden's Canvas paint (golden
    //  :107-117, drawing PV onto ImgOmronPanel) has no substrate (no
    //  TImage/TCanvas type in this port). The one state-carrying side
    //  effect, `dPv=atof(PV.c_str())`, is preserved ACTIVE below. Verbatim:
#if 0
    int iTextWidth, iTextHeight;
    //溫度--------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=36;
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(PV);

    iTextWidth=120;                                                             //Ifor 20241129 add: 避免文字大小不同殘影問題
    ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (44-iTextHeight)/2, "    ");
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(PV);

    ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (44-iTextHeight)/2, PV);
#endif
    dPv=atof(PV.c_str());                                                       //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetDegree(AnsiString Degree, TColor Color)       //顯示量測單位
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- 100% Canvas paint, no other state.
#if 0
    int iTextWidth, iTextHeight;
    //單位---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(Degree);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(Degree);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2, Degree);
#endif
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetRunStop(AnsiString RunStop, TColor Color)     //顯示目前運作模式
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- 100% Canvas paint, no other state.
#if 0
    int iTextWidth, iTextHeight;
    //Run / Stop---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(RunStop);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(RunStop);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+20, RunStop);
#endif
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetAT(AnsiString AT, TColor Color)               //顯示AT是否啟動
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- 100% Canvas paint, no other state.
#if 0
    int iTextWidth, iTextHeight;
    //AT---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(AT);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(AT);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+58, AT);
#endif
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetInputErr(AnsiString InputErr, TColor Color)   //感溫線異常
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- 100% Canvas paint, no other state.
#if 0
    int iTextWidth, iTextHeight;
    //Input Error---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(InputErr);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(InputErr);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+39, InputErr);
#endif
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetEvent(AnsiString Event, TColor Color)         //警報事件
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- 100% Canvas paint, no other state.
#if 0
    int iTextWidth, iTextHeight;
    //事件---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    if(Event=="Event")                                                          //Ifor 20241129 add: 避免文字大小不同殘影問題
    {
        iTextWidth=80;
        iTextHeight=16;
        ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (21-iTextHeight)/2+44, "                      ");
    }
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(Event);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(Event);
    ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (21-iTextHeight)/2+44, Event);
#endif
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetSP(AnsiString SP, TColor Color)               //目前設定值
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- 100% Canvas paint, no other state.
#if 0
    int iTextWidth, iTextHeight;
    //SP---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(SP);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(SP);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+77, SP);
#endif
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetBG(TColor OutLine, TColor BackGround)         //設定背景
{
    //AI(W906-PT-W2) 20260807: GATE (1) -- 100% Canvas paint, no other state.
#if 0
    ImgOmronPanel->Canvas->Brush->Color=BackGround;
    ImgOmronPanel->Canvas->Pen->Color  =OutLine;
    ImgOmronPanel->Canvas->Rectangle(TRect(0, 0,ImgOmronPanel->Width, ImgOmronPanel->Height));
#endif
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetVisible(bool bSW)                                        //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
{
    GroupBox->Visible=bSW;
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetSettingSV(double dsv)                                    //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
{
    dSettingSV=dsv;
    edSV->Text=AnsiString(dSettingSV);
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetSV(double dsv)                                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
{
    dSV=dsv;
    SetSP(AnsiString().sprintf("%03.1f", dSV), TColor(0x004080FF));
}
//---------------------------------------------------------------------------
void TMyOmronPanel::setEditValueClick(TObject *Sender)
{
    //AI(W906-PT-W2) 20260807: GATE (3) -- golden `fQwertyKey->ShowQwertyKey(
    //  (TEdit*)Sender, N_INTEGER, 0, true, 0, 180);` (golden :219). fQwertyKey
    //  has no port anywhere in this tree (an already-established deferred
    //  surface -- see MyOmronPanel.h banner GATE (3)). ACTIVE arm: no-op (a
    //  virtual keyboard cannot show without a live window regardless).
#if 0
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 180);      //小鍵盤
#endif
}
//---------------------------------------------------------------------------
