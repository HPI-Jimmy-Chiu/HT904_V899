// =============================================================================
//  Public/HTEdit.cpp  --  THTEdit method bodies.
//
//  Faithful translation of golden Public/HTEdit.cpp (688 lines, BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 ("hteditlist" group).  See Public/HTEdit.h's
//  banner for why this file exists even though only Public/HTEditList.cpp was
//  named in the wave's file list (hard, unavoidable type dependency).
//
//  ROLE: see HTEdit.h banner.
//
//  WAVE SCOPE -- every golden function, ACTIVE vs GATED:
//    ACTIVE (real, faithful bodies):
//      THTEdit::THTEdit()                golden :13-18   (ctor)
//      THTEdit::~THTEdit()               golden :20-58   (dtor -- GATED interior, see GATE (1))
//      THTEdit::SetContentType           golden :130-133
//      THTEdit::SetDefaultValue          golden :135-138
//      THTEdit::SetCheckRange            golden :140-143
//      THTEdit::SetIniGroupName          golden :155-158
//      THTEdit::SetIniKeyName            golden :160-163
//      THTEdit::EditIntKeyPress          golden :165-167  (golden body is itself empty)
//      THTEdit::EditFloatKeyPress        golden :169-171  (golden body is itself empty)
//      THTEdit::EditPathKeyPress         golden :173-175  (golden body is itself empty)
//      THTEdit::ChangeProperty(...,AnsiString) golden :177-183
//      THTEdit::ChangeProperty(...,bool)        golden :185-191
//      THTEdit::DisableEventOverlap      golden :244-266  (GATED interior, see GATE (2))
//      THTEdit::EditChange               golden :268-303  (GATED interior, see GATE (3); range-clamp math is REAL)
//      THTEdit::SetCustomMax             golden :305-342
//      THTEdit::SetCustomMin             golden :344-381
//      THTEdit::SetDefaultLimitation     golden :383-443
//      THTEdit::SetMax                   golden :445-477
//      THTEdit::SetMin                   golden :479-514
//      THTEdit::SetCustomLimitation      golden :516-578  (one comparison adapted, see GATE (4))
//      THTEdit::SetFileInformation       golden :580-584
//      THTEdit::ReadParamter             golden :586-589
//      THTEdit::SetParamter              golden :591-594
//    GATED (documented no-op / safe-default stubs -- live-widget-window glue
//    with nothing in this substrate to bind to; every gate keeps golden's call
//    VERBATIM in the `#if 0` arm):
//      THTEdit::OnWriteEdit              golden :60-128   -- GATE (5)
//      THTEdit::EditClick                golden :193-242  -- GATE (6)
//      THTEdit::GetDefaultPosition       golden :596-603  -- GATE (7)
//      THTEdit::SetToDefaultPosition     golden :605-616  -- GATE (8)
//    NOT TRANSLATED: the entire commented-out THTCheckBox class (golden
//      :618-688) -- dead code in golden itself (every line already `//`
//      commented there), nothing to port.
//
//  GATE REGISTER -- 8 gates, all because vclcompat/Controls.h's widget
//  stand-ins (TCustomEdit/TEdit/TLabeledEdit/TComboBox/TRadioGroup/TCheckBox/
//  TControl) carry none of OnClick/OnChange/OnKeyPress/PopupMenu/PasswordChar/
//  Parent/Name/Tag/Top/Left/Height/Width/Align (verified: read the whole file,
//  Controls.h's own MEASURED PROPERTY COVERAGE note lists exactly which
//  properties are modeled, and none of these are on it) -- and because
//  Barcode_Reader/TfQwertyKey/fQwertyKey/fQwertyKey2 (myQwertyKeyBoard.h /
//  BarcodeReader.h) have NO compiled body anywhere in the port tree (grepped
//  20260807, only .rc/.json/.md hits). Since real VCL never constructs any of
//  these widget stand-ins as EC/SV-registered objects either (see Controls.h's
//  own SCOPE BOUNDARY (1)), every one of golden's dynamic_cast branches here is
//  reachable-but-always-NULL today -- these gates cost nothing until the W7 UI
//  substrate lands real widgets.
//   (1) ~THTEdit() interior (golden :24-52): the OnClick/OnKeyPress/OnChange
//       restore-on-destroy block. FAITHFUL DEFAULT: no-op -- OnWriteEdit (GATE 5)
//       never actually assigns Ed->OnClick/OnKeyPress/OnChange in this build
//       (they don't exist on TEdit/TLabeledEdit), so FOldOnClickEvent/
//       FOldKeyPressEvent/FOldOnChange are always NULL and this block would be
//       a no-op even if compiled. DisablePopupMenu is still freed for real
//       (kept ACTIVE, not gated -- see the ctor/dtor bodies below).
//   (2) DisableEventOverlap interior (golden :246-265): same OnClick/OnKeyPress/
//       OnChange restore, same FAITHFUL DEFAULT reasoning as GATE (1).
//   (3) EditChange interior (golden :280-296, the `TCustomEdit *CEd=...; if
//       (CEd!=NULL){ clamp CEd->Text to [Min,Max] }` block) -- CEd->Text DOES
//       exist (vclcompat::TCustomEdit::Text is real), so this branch is left
//       ACTIVE/real; only the trailing `FOldOnChange(this);` forward-call
//       (golden :299-302) is gated, for the same FOldOnChange-is-always-NULL
//       reason as GATE (1) (kept as a real `if(FOldOnChange!=NULL)` guard that
//       can never fire, which is honest rather than deleted).
//   (4) SetCustomLimitation (golden :562,565): `atof(Max.c_str())>HTMax` /
//       `atof(Min.c_str())<HTMin` compare a `double` against golden's AnsiString
//       HTMax/HTMin directly. Real BCB6 AnsiString tolerates this via an
//       implicit numeric conversion path vclcompat::AnsiString does not
//       provide (verified: no `operator double()` anywhere in
//       vclcompat/AnsiString.h). FAITHFUL ADAPTATION (not a behaviour change):
//       both sides converted via atof() for the comparison
//       (`atof(Max.c_str())>atof(MaxValue.c_str())`), which is what the
//       surrounding code's own idiom (every other comparison in this same
//       function already goes through atof()) shows was always the intent.
//   (5) OnWriteEdit (golden :60-128): assigns Ed->OnClick/OnChange/OnKeyPress/
//       PopupMenu and LEd->(same), none of which exist on vclcompat::TEdit/
//       TLabeledEdit. FAITHFUL DEFAULT: no-op. BEHAVIOUR DELTA: on a real
//       machine, clicking/tabbing into an edit widget pops the on-screen
//       keypad (EditClick, GATE 6) and filters keystrokes
//       (EditIntKeyPress/EditFloatKeyPress/EditPathKeyPress, ACTIVE above but
//       themselves empty in golden); offline, with no window subsystem, there
//       is no click/keypress event stream to wire in the first place, so the
//       delta is unobservable until the W7 UI substrate exists.
//   (6) EditClick (golden :193-242): pops golden's on-screen keypad
//       (fQwertyKey->ShowQwertyKey(...)) after checking iBarcodeReadType via
//       Barcode_Reader(). Both symbols have NO compiled body anywhere in the
//       port tree (grepped 20260807: only tools/dfm2rc/*.json and *.md hits).
//       FAITHFUL DEFAULT: no-op body that still forwards to FOldOnClickEvent
//       if non-NULL (matching golden's own trailing behaviour) -- which, per
//       GATE (5), is always NULL today, so the net effect is a true no-op.
//   (7)/(8) GetDefaultPosition / SetToDefaultPosition (golden :596-616): read/
//       write SourceControl->Parent/Top/Left/Height/Width/Align, none of which
//       exist on vclcompat::TControl (verified: TControl's whole surface is
//       Visible/Enabled/hCtl/Click()). FAITHFUL DEFAULT: Par=NULL,
//       iTop=iLeft=iHeight=iWidth=0 -- these 4 ints have exactly ONE consumer
//       tree-wide (SetToDefaultPosition itself, also gated), so zeroing them
//       is inert, not a silent data loss.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#include "MachineDefine.h"
#include "HTEdit.h"
#include "cMyDB.h"          // MyDBIProcess(AnsiString,AnsiString) -- aHotPlateSubstrate.cpp:1030 body; declared cMyDB.h
#include "cmydef.h"         // bcTotal (MachineType.h enum, pulled transitively) -- ctor default for iBarcodeReadType
//---------------------------------------------------------------------------
THTEdit::THTEdit()                                                              // golden :13-18
{
    DisablePopupMenu = NULL;                                                    // GATE (see file banner): golden `new TPopupMenu(NULL)` -- no live widget can ever attach a popup menu offline (TCustomEdit/TEdit carry no PopupMenu member), so a real allocation would be a permanent, zero-effect leak. NULL is the faithful default; the destructor's NULL-guarded delete below still matches golden's own shape.
    CheckRange      = true;
    iBarcodeReadType= bcTotal;
}
//---------------------------------------------------------------------------
THTEdit::~THTEdit()                                                            // golden :20-58
{
    try
    {
        // GATE (1): golden restores Ed/LEd->OnClick/OnKeyPress/OnChange from
        // FOldOnClickEvent/FOldKeyPressEvent/FOldOnChange here. Faithful
        // default: no-op (see GATE REGISTER (1) above).
#if 0
        TEdit        *Ed = dynamic_cast <TEdit          *>(SourceControl);
        TLabeledEdit *LEd= dynamic_cast <TLabeledEdit   *>(SourceControl);
        if(FOldOnClickEvent!=NULL)
        {
            if(Ed!=NULL)
                Ed->OnClick=FOldOnClickEvent;
            else if(Ed!=LEd)
                LEd->OnClick=FOldOnClickEvent;
        }
        if(FOldKeyPressEvent!=NULL)
        {
            if(Ed!=NULL)
                Ed->OnKeyPress=FOldKeyPressEvent;
            else if(Ed!=LEd)
                LEd->OnKeyPress=FOldKeyPressEvent;
        }
        if(FOldOnChange!=NULL)
        {
            if(Ed!=NULL)
                Ed->OnChange=FOldOnChange;
            else if(Ed!=LEd)
                LEd->OnChange=FOldOnChange;
        }
#endif
        if(DisablePopupMenu!=NULL)
            delete DisablePopupMenu;
        DisablePopupMenu=NULL;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~THTEdit");
    }
}
//---------------------------------------------------------------------------
void THTEdit::OnWriteEdit(TCustomEdit *_SourceEdit)                            // golden :60-128 -- GATE (5)
{
    SourceControl = _SourceEdit;
#if 0
    TEdit        *Ed = dynamic_cast <TEdit          *>(SourceControl);
    TLabeledEdit *LEd= dynamic_cast <TLabeledEdit   *>(SourceControl);          //Steven 20230224 : Edit List改支援多型態元件
    if(Ed!=NULL)
    {
        FOldOnClickEvent    =Ed->OnClick;
        FOldKeyPressEvent   =Ed->OnKeyPress;
        FOldOnChange        =Ed->OnChange;

        Ed->OnClick         =EditClick;
        Ed->OnChange        =EditChange;

        switch(HTContentType)
        {
            case ECInteger:
                if(iTransformType!=EUNone)                                     //Steven 20230905 : HTEditList加入型態轉換的Flag
                    Ed->OnKeyPress =EditFloatKeyPress;
                else
                    Ed->OnKeyPress =EditIntKeyPress;
                break;
            case ECDouble:
                Ed->OnKeyPress     =EditFloatKeyPress;
                break;
            case ECText:
                Ed->OnKeyPress     =EditPathKeyPress;
                break;
            case ECPassword:    //密碼文
                Ed->PasswordChar   ='*';
                break;
            default:
                break;
        }
        Ed->PopupMenu=DisablePopupMenu;
    }
    else if(Ed!=LEd)
    {
        FOldOnClickEvent    =LEd->OnClick;
        FOldKeyPressEvent   =LEd->OnKeyPress;
        FOldOnChange        =LEd->OnChange;

        LEd->OnClick        =EditClick;
        LEd->OnChange       =EditChange;

        switch(HTContentType)
        {
            case ECInteger:
                if(iTransformType!=EUNone)                                     //Steven 20230905 : HTEditList加入型態轉換的Flag
                    Ed->OnKeyPress  =EditFloatKeyPress;
                else
                    LEd->OnKeyPress =EditIntKeyPress;
                break;
            case ECDouble:
                LEd->OnKeyPress     =EditFloatKeyPress;
                break;
            case ECText:
                LEd->OnKeyPress     =EditPathKeyPress;
                break;
            case ECPassword:    //密碼文
                LEd->PasswordChar   ='*';
                break;
            default:
                break;
        }
        LEd->PopupMenu=DisablePopupMenu;
    }
#else
    // GATE (5): see file banner. Nothing on vclcompat::TEdit/TLabeledEdit to
    // bind OnClick/OnChange/OnKeyPress/PasswordChar/PopupMenu to -- no-op.
    (void)0;
#endif
}
//---------------------------------------------------------------------------
void THTEdit::SetContentType(TEditContent P)                                   // golden :130-133
{
    Content=P;
}
//---------------------------------------------------------------------------
void THTEdit::SetDefaultValue(AnsiString P)                                    // golden :135-138
{
    DefaultValue=P;
}
//---------------------------------------------------------------------------
void THTEdit::SetCheckRange(bool P)                                            // golden :140-143
{
    CheckRange=P;
}
//---------------------------------------------------------------------------
void THTEdit::SetIniGroupName(AnsiString P)                                    // golden :155-158
{
    IniGroupName=P;
}
//---------------------------------------------------------------------------
void THTEdit::SetIniKeyName(AnsiString P)                                      // golden :160-163
{
    IniKeyName=P;
}
//---------------------------------------------------------------------------
void THTEdit::EditIntKeyPress(TObject * /*Sender*/, char & /*Key*/)            // golden :165-167 (golden body itself empty)
{
}
//---------------------------------------------------------------------------
void THTEdit::EditFloatKeyPress(TObject * /*Sender*/, char & /*Key*/)          // golden :169-171 (golden body itself empty)
{
}
//---------------------------------------------------------------------------
void THTEdit::EditPathKeyPress(TObject * /*Sender*/, char & /*Key*/)           // golden :173-175 (golden body itself empty)
{
}
//---------------------------------------------------------------------------
void THTEdit::ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, AnsiString Default)  // golden :177-183
{
    bVisible        =Visible;
    bEnable         =Enable;
    bReadFromFile   =ReadFromFile;
    DefaultValue    =Default;
}
//---------------------------------------------------------------------------
void THTEdit::ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, bool Default)         // golden :185-191
{
    bVisible        =Visible;
    bEnable         =Enable;
    bReadFromFile   =ReadFromFile;
    DefaultValue    =Default?"1":"0";
}
//---------------------------------------------------------------------------
void THTEdit::EditClick(TObject * /*Sender*/)                                  // golden :193-242 -- GATE (6)
{
#if 0
    if(iBarcodeReadType!=bcTotal)
        Barcode_Reader(iBarcodeReadType);

    if(fQwertyKey==NULL)
        Application->CreateForm(__classid(TfQwertyKey), &fQwertyKey);
    if(fQwertyKey2==NULL)
        Application->CreateForm(__classid(TfQwertyKey), &fQwertyKey2);         //Steven 20150505 : 剛好在輸入時,Alarm會卡死

    if(HTContentType==ECText)                                                  //純文字
    {
        fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_NO_SYMBOL|N_NO_SPACE);
    }
    else if(HTContentType==ECInteger || HTContentType==ECPosInt || HTContentType==ECNegInt || HTContentType==ECPort)        //數字
    {
        if(iTransformType==EUuMToMM)                                           //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else if(iTransformType==EUMSToSec)                                     //顯示為Sec, 存檔為MS
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_INTEGER, 0, CheckRange, atoi(HTCustomMax.c_str()), atoi(HTCustomMin.c_str()));
    }
    else if(HTContentType==ECDouble || HTContentType==ECPosDouble || HTContentType==ECNegDouble || HTContentType==ECPercent)         //浮點數
    {
        if(iTransformType==EUuMToMM)                                           //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else if(iTransformType==EUMSToSec)                                     //顯示為Sec, 存檔為MS
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
    }
    else if(HTContentType==ECIPAddr)
    {
        fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_IP_ADDR);
    }
    else if(HTContentType==ECFileName)                                         //檔案名稱或路徑
    {
        fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_NO_SYMBOL);
    }
    else if(HTContentType==ECPassword)
    {
        fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_PASSWORD);
    }

    if(FOldOnClickEvent!=NULL)
    {
        FOldOnClickEvent(this);
    }
#else
    // GATE (6): see file banner. Barcode_Reader/TfQwertyKey/fQwertyKey/
    // fQwertyKey2 have no compiled body anywhere in the port tree. Faithful
    // default: forward to the previous click handler if one was ever wired
    // (per GATE (5) it never is today, so this is a true no-op).
    if(FOldOnClickEvent!=NULL)
        FOldOnClickEvent(this);
#endif
}
//---------------------------------------------------------------------------
void THTEdit::DisableEventOverlap()                                            // golden :244-266 -- GATE (2)
{
#if 0
    TEdit        *Ed = dynamic_cast <TEdit          *>(SourceControl);
    TLabeledEdit *LEd= dynamic_cast <TLabeledEdit   *>(SourceControl);
    if(Ed!=NULL)
    {
        if(FOldOnClickEvent!=NULL)
            Ed->OnClick    =FOldOnClickEvent;
        if(FOldKeyPressEvent!=NULL)
            Ed->OnKeyPress =FOldKeyPressEvent;
        if(FOldOnChange!=NULL)
            Ed->OnChange   =FOldOnChange;
    }
    else if(LEd!=NULL)
    {
        if(FOldOnClickEvent!=NULL)
            LEd->OnClick    =FOldOnClickEvent;
        if(FOldKeyPressEvent!=NULL)
            LEd->OnKeyPress =FOldKeyPressEvent;
        if(FOldOnChange!=NULL)
            LEd->OnChange   =FOldOnChange;
    }
#else
    // GATE (2): see file banner / GATE (1) -- no-op.
    (void)0;
#endif
}
//---------------------------------------------------------------------------
void THTEdit::EditChange(TObject * /*Sender*/)                                 // golden :268-303 -- GATE (3) on the trailing forward-call only
{
    if(CheckRange)                                                             //Steven 20190826 : 修正參數範圍比對
    {
        if(Content==ECInteger    ||                                            //只存入有數值的
           Content==ECDouble     ||
           Content==ECPosInt     ||
           Content==ECPosDouble  ||
           Content==ECNegInt     ||
           Content==ECNegDouble  ||
           Content==ECPort)                                                    //Steven 20230221 : 小鍵盤新增通訊埠上下限
        {
            TCustomEdit  *CEd = dynamic_cast <TCustomEdit *>(SourceControl);    //Steven 20230224 : Edit List改支援多型態元件
            if(CEd!=NULL)
            {
                double dValue=atof(CEd->Text.c_str());
                double dMax  =atof(CustomMaxValue.c_str());
                double dMin  =atof(CustomMinValue.c_str());

                if(dValue<dMin)
                {
                    CEd->Text=dMin;
                }
                else if(dValue>dMax)
                {
                    CEd->Text=dMax;
                }
            }
        }
    }

    // GATE (3): see file banner -- FOldOnChange is always NULL in this
    // substrate (OnWriteEdit, GATE 5, never assigns it a real handler), so
    // this guarded call is honest dead code rather than deleted behaviour.
    if(FOldOnChange!=NULL)
    {
        FOldOnChange(this);
    }
}
//---------------------------------------------------------------------------
void THTEdit::SetCustomMax(AnsiString Max)                                     // golden :305-342
{
    if(Content==ECPosInt || Content==ECPosDouble)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;
    }
    else if(Content==ECNegInt || Content==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;
    }
    else if(Content==ECPort)  //Steven 20230221 : 小鍵盤新增通訊埠上下限
    {
        if(atof(Max.c_str())>65535)
            Max=65535;
    }
    else if(Content==ECPercent)
    {
        if(atof(Max.c_str())>100.0)
            Max=100.0;

        if(atof(Max.c_str())<0.0)
            Max=0.0;
    }

    if(Content==ECBool)
    {
        Max=1;
    }
    else
    {
        if(atof(Max.c_str())>atof(MaxValue.c_str()))
            Max=MaxValue;
    }

    CustomMaxValue=Max;
}
//---------------------------------------------------------------------------
void THTEdit::SetCustomMin(AnsiString Min)                                     // golden :344-381
{
    if(Content==ECPosInt || Content==ECPosDouble)
    {
        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(Content==ECNegInt || Content==ECNegDouble)
    {
        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(Content==ECPort)
    {
        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(Content==ECPercent)
    {
        if(atof(Min.c_str())>100.0)
            Min=100.0;

        if(atof(Min.c_str())<0.0)
            Min=0.0;
    }

    if(Content==ECBool)
    {
        Min=0;
    }
    else
    {
        if(atof(Min.c_str())<atof(MinValue.c_str()))
            Min=MinValue;
    }

    CustomMinValue=Min;
}
//---------------------------------------------------------------------------
void THTEdit::SetDefaultLimitation(AnsiString Max, AnsiString Min)             // golden :383-443
{
    if(Content==ECPosInt || Content==ECPosDouble || Content==ECPercent)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;

        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(Content==ECPort)
    {
        if(atof(Max.c_str())>65535)
            Max=65535;

        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(Content==ECNegInt || Content==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;

        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(Content==ECPercent)
    {
        if(atof(Max.c_str())>100.0)
            Max=100.0;

        if(atof(Max.c_str())<0.0)
            Max=0.0;

        if(atof(Min.c_str())>100.0)
            Min=100.0;

        if(atof(Min.c_str())<0.0)
            Min=0.0;
    }
    else if(Content==ECBool)
    {
        Max=1;
        Min=0;
    }

    if(atof(Max.c_str())<atof(Min.c_str()))
    {
        MaxValue=Min;
        MinValue=Max;
        CustomMaxValue=Min;
        CustomMinValue=Max;
    }
    else
    {
        MaxValue=Max;
        MinValue=Min;
        CustomMaxValue=Max;
        CustomMinValue=Min;
    }
}
//---------------------------------------------------------------------------
void THTEdit::SetMax(AnsiString Max)                                           // golden :445-477
{
    if(Content==ECPosInt || Content==ECPosDouble)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;
    }
    else if(Content==ECNegInt || Content==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;
    }
    else if(Content==ECPort)
    {
        if(atof(Max.c_str())>65535)
            Max=65535;
    }
    else if(Content==ECPercent)
    {
        if(atof(Max.c_str())>100.0)
            Max=100.0;

        if(atof(Max.c_str())<0.0)
            Max=0.0;
    }
    else if(Content==ECBool)
    {
        Max=1;
    }

    MaxValue=Max;
    CustomMaxValue=Max;
}
//---------------------------------------------------------------------------
void THTEdit::SetMin(AnsiString Min)                                           // golden :479-514
{
    if(Content==ECPosInt || Content==ECPosDouble)
    {
        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(Content==ECNegInt || Content==ECNegDouble)
    {
        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(Content==ECPort)
    {
        if(atof(Min.c_str())<0)
            Min=0;

        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(Content==ECPercent)
    {
        if(atof(Min.c_str())>100.0)
            Min=100.0;

        if(atof(Min.c_str())<0.0)
            Min=0.0;
    }
    else if(Content==ECBool)
    {
        Min=0;
    }

    MinValue=Min;
    CustomMinValue=Min;
}
//---------------------------------------------------------------------------
void THTEdit::SetCustomLimitation(AnsiString Max, AnsiString Min)              // golden :516-578 -- GATE (4) on two comparisons
{
    if(Content==ECPosInt || Content==ECPosDouble)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;

        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(Content==ECNegInt || Content==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;

        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(Content==ECPort)
    {
        if(atof(Max.c_str())>65535)
            Max=65535;

        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(Content==ECPercent)
    {
        if(atof(Max.c_str())>100.0)
            Max=100.0;

        if(atof(Max.c_str())<0.0)
            Max=0.0;

        if(atof(Min.c_str())>100.0)
            Min=100.0;

        if(atof(Min.c_str())<0.0)
            Min=0.0;
    }
    else if(Content==ECBool)
    {
        Max=1;
        Min=0;
    }

    // GATE (4): see file banner -- golden compares `atof(Max.c_str())>HTMax`
    // (double vs AnsiString) directly; adapted to atof() on both sides.
    if(atof(Max.c_str())>atof(MaxValue.c_str()))
        Max=MaxValue;

    if(atof(Min.c_str())<atof(MinValue.c_str()))
        Min=MinValue;

    if(atof(Max.c_str())<atof(Min.c_str()))
    {
        CustomMaxValue=Min;
        CustomMinValue=Max;
    }
    else
    {
        CustomMaxValue=Max;
        CustomMinValue=Min;
    }
}
//---------------------------------------------------------------------------
void THTEdit::SetFileInformation(AnsiString GroupName, AnsiString KeyName)     // golden :580-584
{
    IniGroupName    =GroupName;
    IniKeyName      =KeyName;
}
//---------------------------------------------------------------------------
Variant THTEdit::ReadParamter()                                                // golden :586-589
{
    return FParemeter;
}
//---------------------------------------------------------------------------
void THTEdit::SetParamter(Variant VData)                                      // golden :591-594
{
    FParemeter=VData;
}
//---------------------------------------------------------------------------
void THTEdit::GetDefaultPosition()                                            // golden :596-603 -- GATE (7)
{
#if 0
    Par     =SourceControl->Parent;
    iTop    =SourceControl->Top;
    iLeft   =SourceControl->Left;
    iHeight =SourceControl->Height;
    iWidth  =SourceControl->Width;
#else
    // GATE (7): see file banner. No Parent/Top/Left/Height/Width on
    // vclcompat::TControl -- faithful default: zero/NULL (these 4 ints have
    // exactly one consumer tree-wide, SetToDefaultPosition, also gated).
    Par     =NULL;
    iTop    =0;
    iLeft   =0;
    iHeight =0;
    iWidth  =0;
#endif
}
//---------------------------------------------------------------------------
void THTEdit::SetToDefaultPosition()                                          // golden :605-616 -- GATE (8)
{
#if 0
    if(SourceControl->Parent!=Null && Par!=NULL && SourceControl->Parent!=Par)
    {
        SourceControl->Align =alNone;
        SourceControl->Parent=Par;
        SourceControl->Top   =iTop;
        SourceControl->Left  =iLeft;
        SourceControl->Height=iHeight;
        SourceControl->Width =iWidth;
    }
#else
    // GATE (8): see file banner -- no live widget geometry to move offline.
    (void)0;
#endif
}
//---------------------------------------------------------------------------
