// =============================================================================
//  forms/fSetup.cpp  --  definitions for the fSetup facade
//
//  AI(W906-W7-L2) 20260803: first home for TfSetup; see forms/fSetup.h for the
//  golden citations and the branch the offline value selects.  Follows
//  forms/fNote.cpp exactly.
// =============================================================================
#include "forms/fSetup.h"
// AI(W906-FW-UNGATE-W29) 20260826: WA-1 開閘所需 —— fQwertyKey (:406) +
// TfQwertyKey::ShowQwertyKey (:370)。forms/fQwertyKey.cpp 與本檔同屬
// ht9045_forms target (CMakeLists.txt:692/:699)，沒有新的 link edge。
// 只加在 .cpp，不动 forms/fSetup.h 的 include 圖。
#include "forms/fQwertyKey.h"

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

// AI(W906-FW-UNGATE-W29) 20260826: WA-1 GATE OPENED (5 methods).
// 舊理由「fQwertyKey has no port anywhere in this tree」（標 20260820）
// 已於 20260824 死於 wave FW-QWKEY1 (fc08e09)：class TfQwertyKey
// forms/fQwertyKey.h:294、全域 forms/fQwertyKey.cpp:41-42、ShowQwertyKey
// 本體 ACTIVE forms/fQwertyKey.cpp:166。完整證據、兄弟站點清單、
// 以及「開閘後這五支實際會做什麼」見 forms/fSetup.h 的
// GATE REGISTER (WA-1)。
//
// 簽章：golden 的 `TObject *Sender` 還原成 `TEdit *Sender`（golden 自己
// 的 cast 目標，慣例同 cSetUp.cpp:1131 XPitchMouseDown / cSpeed.cpp:1449）；
// edOcrTextMouseDown 的 `TMouseButton Button, TShiftState Shift, int X,
// int Y` 在 golden 本體內都沒被讀到，依同一慣例 drop。
//
// ⚠ fQwertyKey 平時是 NULL（唯一建立點 Public/HTEdit.cpp:311-320 的
// lazy new），且 ShowQwertyKey 本體第一行就 deref fQwertyKey->bShow
// (forms/fQwertyKey.cpp:168)。這五支今天沒任何 caller，所以到不了；
// 安全是因為沒接線，不是因為有守衛。

void TfSetup::edOcrTextMouseDown(TEdit *Sender)                                // golden cSetUp.cpp:4631-4635
{
    fQwertyKey->ShowQwertyKey(Sender, N_NO_SYMBOL|N_NO_SPACE);
}

void TfSetup::edOverRangeClick(TEdit *Sender)                                  // golden cSetUp.cpp:4839-4842
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 0, true, 100.0, 0.0);
}

void TfSetup::edDelayTimeClick(TEdit *Sender)                                  // golden cSetUp.cpp:4844-4847
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 0, true, 10.0, 0.05);
}

void TfSetup::edAuto1CountClick(TEdit *Sender)                                 // golden cSetUp.cpp:4849-4852
{
    fQwertyKey->ShowQwertyKey(Sender, N_INTEGER, 0, true, 40, 2);
}

void TfSetup::edtGetValueDelayTimeClick(TEdit *Sender)                         // golden cSetUp.cpp:4854-4857
{
    fQwertyKey->ShowQwertyKey(Sender, N_DOUBLE, 1, true, 0, 1);
}
