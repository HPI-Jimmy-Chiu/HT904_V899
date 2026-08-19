// =============================================================================
//  forms/fSetup.cpp  --  definitions for the fSetup facade
//
//  AI(W906-W7-L2) 20260803: first home for TfSetup; see forms/fSetup.h for the
//  golden citations and the branch the offline value selects.  Follows
//  forms/fNote.cpp exactly.
// =============================================================================
#include "forms/fSetup.h"

// fShow=false is golden's own ctor value, csetup.cpp:163, inside `__fastcall
// TfSetup::TfSetup(TComponent* Owner)` at csetup.cpp:123.  It is NOT chosen for
// being "safe" -- forms/fSetup.h names the arm it opens (golden ckernel.cpp
// :413-439, the START-time motor-home ladder, which can refuse the start).
TfSetup::TfSetup() : fShow(false) {}

// Golden creates this form UNCONDITIONALLY at startup -- HT9045.cpp:183
// `Application->CreateForm(__classid(TfSetup), &fSetup);`, in the same flat
// unguarded CreateForm list as fHome.  Golden ckernel.cpp:411 dereferences
// fSetup with no null guard, which only makes sense because of that.  So a live
// instance is the faithful offline state; NULL would be a segfault at :411.
TfSetup *fSetup = new TfSetup();

// =============================================================================
//  AI(W906-FW3-Setup-WA) 20260820: 14 (a) 顯示/唯讀 methods, cSetUp Wave A.
//  See forms/fSetup.h's file-head banner for the golden-span table, GATE
//  REGISTER, deferred-methods list, and the RadioButton1KeyDown GOLDEN
//  ODDITY. Everything above this block is UNTOUCHED W7-L2 content.
// =============================================================================

int TfSetup::GetTestMode(AnsiString sTestMode)                                  //Steven 20250321 : 把X字串轉換 -- golden cSetUp.cpp:2179-2189
{
    for(int i=0; i<TotalTestMode; i++)
    {
        if(sTestMode==TestSiteFileName[0][i])
        {
            return i;
        }
    }
    return 0;
}

void TfSetup::SetShtMode(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2) //Jimmychiu 20241203 -- golden cSetUp.cpp:4708-4715
{
    shtMode.Clear();
    shtMode.bNormal=bNormal;
    shtMode.bOneSide=bOneSide;
    shtMode.bUsedSht1=bUsedSht1;
    shtMode.bUsedSht2=bUsedSht2;
}

bool TfSetup::VertifyShtModeisDiff(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2) //Jimmychiu 20241203 -- golden cSetUp.cpp:4717-4730
{
    if(shtMode.bNormal!=bNormal ||
       shtMode.bOneSide!=bOneSide ||
       shtMode.bUsedSht1!=bUsedSht1 ||
       shtMode.bUsedSht2!=bUsedSht2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// golden cSetUp.cpp:3403-3407 -- `TObject *Sender` dropped, never read.
void TfSetup::XPitchKeyPress(char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}

// golden cSetUp.cpp:4443-4449 -- `WORD &Key, TShiftState Shift` dropped,
// never read (see forms/fConfiguration.h's own precedent for this exact
// drop, WA-8). `Sender` is typed directly as TRadioButton* (golden casts it
// with `(TRadioButton *)Sender`), so that C-style cast collapses; the local
// `Ptr` is kept to stay line-for-line faithful to golden's body. DFM-ORPHANED
// -- see forms/fSetup.h's GOLDEN ODDITY note; translated verbatim anyway.
void TfSetup::RadioButton1KeyDown(TRadioButton *Sender)
{
    TRadioButton *Ptr;
    Ptr=Sender;
    SitCH=Ptr->Checked;
}

// golden cSetUp.cpp:4772-4777 -- `Sender` typed directly as TRadioGroup*
// (golden casts it with `(TRadioGroup *)Sender`), so that C-style cast
// collapses; the local `Temp` is kept to stay line-for-line faithful.
void TfSetup::rgSensor1Click(TRadioGroup *Sender)
{
    TRadioGroup *Temp=Sender;
    if(Temp->Visible==true && Temp->ItemIndex==0)
        Temp->ItemIndex=1;
}

// golden cSetUp.cpp:1264-1282 -- `TObject *Sender` dropped, never read.
void TfSetup::rgShtModeNormalClick()
{
    if(rgShtModeOneSide->Checked==false)
    {
        rgUseSht1->Checked=false;
        rgUseSht2->Checked=false;
        rgUseSht1->Enabled=false;
        rgUseSht2->Enabled=false;
        rgUseSuckMode->Enabled=true;                                           //Steven 20101013
    }
    else
    {
        rgUseSht1->Checked=true;
        rgUseSht1->Enabled=true;
        rgUseSht2->Enabled=true;
        rgUseSuckMode->ItemIndex=0;                                            //Steven 20101013 : 用Arm使用多子吸嘴會Hang Up
        rgUseSuckMode->Enabled=false;                                          //Steven 20101013
    }
}

// golden cSetUp.cpp:4464-4474 -- `TObject *Sender` dropped, never read.
void TfSetup::Arm1PickArm2TestClick()
{
    if(Arm1PickArm2Test->Checked)                                              //kevin 20150127 Arm1 下壓 arm2 測試
    {
        gbShuttleMode->Visible=false;                                          //only use shuttle 1
    }
    else
    {
        gbShuttleMode->Visible=IniConfig.bD30EnableSiteModeSelect;
    }
}

// golden cSetUp.cpp:4637-4643 -- `TObject *Sender` dropped, never read.
void TfSetup::cbUseSLKClampClick()                                              //JerryYang 20160714 特殊化模組化SLK就直接關掉分離測試模式
{
    if(cbUseSLKClamp->Checked==true)                                           //JerryYang 20160714 bUseSLKClamp
        rgseparabilityTest->Enabled=true;
    else
        rgseparabilityTest->Enabled=false;
}

// golden cSetUp.cpp:4779-4802 -- `TObject *Sender` dropped, never read.
void TfSetup::rgYPitchOffsetModeClick()
{
    if(rgYPitchOffsetMode->ItemIndex==1)                                       //Steven 20201013 : NN mode add Y-Pitch offset selection
    {
        labYOffset->Caption="Y Offset (mm)";                                   //KenHsieh 20231227 : add Y Center Pitch mode
        labYOffset->Width=93;
        edYOffset->Enabled=false;
        edYOffset->Text=10;
    }
    else if(rgYPitchOffsetMode->ItemIndex==0)                                  //KenHsieh 20231227 : add Y Center Pitch mode
    {
        labYOffset->Caption="Y Offset (mm)";
        labYOffset->Width=93;
        edYOffset->Text= FormatFloat("0.000", TestIF_File.dSiteYOffset);       //Steven 20140512 : For HT-9047
        edYOffset->Enabled=true;
    }
    else
    {
        labYOffset->Caption="Y Center Pitch (mm)";                             //KenHsieh 20231227 : add Y Center Pitch mode
        labYOffset->Width=134;
        edYOffset->Text= FormatFloat("0.000", TestIF_File.dSiteYOffset);
        edYOffset->Enabled=true;
    }
}

// WA-1 GATE (5 methods) -- golden `fQwertyKey->ShowQwertyKey(...)` one-liners;
// fQwertyKey has no port anywhere in this tree (see forms/fSetup.h's GATE
// REGISTER). Each golden `TObject *Sender` (plus, for edOcrTextMouseDown,
// `TMouseButton Button, TShiftState Shift, int X, int Y`) becomes unused once
// the call is gated, so every parameter is dropped. Bodies intentionally
// empty -- matches forms/fConfiguration.h's own WA-1 precedent for the
// identical situation.

void TfSetup::edOcrTextMouseDown()                                             // golden cSetUp.cpp:4631-4635
{
    // gated: fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL|N_NO_SPACE);
}

void TfSetup::edOverRangeClick()                                               // golden cSetUp.cpp:4839-4842
{
    // gated: fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 100.0, 0.0);
}

void TfSetup::edDelayTimeClick()                                               // golden cSetUp.cpp:4844-4847
{
    // gated: fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 10.0, 0.05);
}

void TfSetup::edAuto1CountClick()                                              // golden cSetUp.cpp:4849-4852
{
    // gated: fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 40, 2);
}

void TfSetup::edtGetValueDelayTimeClick()                                      // golden cSetUp.cpp:4854-4857
{
    // gated: fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 1, true, 0, 1);
}
