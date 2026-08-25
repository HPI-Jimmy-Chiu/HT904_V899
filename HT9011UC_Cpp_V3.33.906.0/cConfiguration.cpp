// =============================================================================
//  cConfiguration.cpp  --  configuration-form display side: pure
//                           ShowQwertyKey one-liners + the Tray/HP StringGrid
//                           read/edit/reload cluster (batch-5 cConfiguration
//                           Wave A -- see forms/fConfiguration.h banner
//                           FIRST, it documents the full 28-method
//                           classification, GATE register, DEVIATION list,
//                           and TfConfigurationGrid design notes; not
//                           repeated in full here).
//
//  Faithful translation of golden cConfiguration.cpp (7,808 lines, BCB6,
//  cp950 -- decoded with `python3 -c "open(path,'rb').read()
//  .decode('cp950')"`, 0 U+FFFD, 20260820).
//  Translator: AI(W906-FW3-Config-WA) 20260820.
//  Translation wave: FW-3 cConfiguration Wave A (batch 5, display-only
//  mandate, per docs/RECON_cConfiguration_displayside.md section 6).
//  Facade: forms/fConfiguration.h (NEW this wave -- no prior facade existed
//  anywhere in this tree, verified by grep 20260820).
//
//  WAVE SCOPE / GATE REGISTER (WA-1) / DEVIATION / DESIGN NOTES -- all in
//  forms/fConfiguration.h. Summary: 28 of 129 golden TfConfiguration::
//  methods (see docs/RECON_cConfiguration_displayside.md section 1 for the
//  full 129-method inventory), 289 golden lines by this wave's own per-
//  method accounting (the recon's section 6.4 rollup states 354; see this
//  file's facade header for the reconciliation of that discrepancy). 17 of
//  the 28 touch GATE WA-1 (fQwertyKey; OPENED 20260824 by FW-QWKEY2 --
//  entity real since FW-QWKEY1); the remaining 11 are fully real.
//
//  fConfiguration global is NOT declared this wave -- see forms/
//  fConfiguration.h's own INTEGRATION STATUS note (same posture as
//  cTemperFrom.cpp/cSpeed.cpp: the ctor is (b) write-path and out of this
//  wave's scope, so there is nothing faithful to construct a live instance
//  from yet).
// =============================================================================
#include "forms/fConfiguration.h"

#include "cmydef.h"      // CUSTOMER_CODE/CC_SCC/CC_SCK/CC_AMD_M/CC_ETRENDTECH (EnableRMSFunc)
#include "Config.h"      // IniConfig (EnableRMSFunc: bShowLotInfo)
#include "CosFunction.h" // CosFunction (EnableRMSFunc: bEnableRMS)
#include "common.h"      // TrayTablePath/PlateTablePath (sbtReloadTrayClick/sbtReloadHPClick)

// AI(W906-FW-CFG-W1) 20260825: the registration table InitConfigEdtList_ItemA/
// B/C builds needs the edit-list layer. Both were already fully ported; this
// TU simply had no reason to see them until now.
#include "Public/HTEditList.h"  // HTEditList + the elConfig / elConfig_byRecipe / cbLastSet globals (:247/:252/:253) and the TEditContent enum (ECBool/ECInteger/ECText)
#include "LastSet.h"            // LAST_GENERAL_SET LastSet (:587) -- ItemA registers several fields against it
// AI(W906-FW-CFG-W2) 20260825: TU-LOCAL FORWARD DECLARATION -- CheckFile.
// ItemD reads the Security_new.def input limit for D41IndexCheckOffset through
// it. The real declaration is cAuthority.h:83 and the real body
// cAuthority.cpp:503, but this TU CANNOT include cAuthority.h: that header
// pulls in language.h, whose `class TWinControl` (language.h:79) collides with
// the one Public/HTEdit.h:140 already defines, and HTEdit.h arrives here via
// the HTEditList.h include above. Measured, not assumed -- adding the include
// produced exactly that redefinition error and nothing else.
// A TU-local declaration is this tree's established answer to a header pair
// that cannot coexist; database.cpp:96 does the same thing for ShowMyMessage,
// and cObserver.cpp carries the identical pattern. Signature copied verbatim
// from cAuthority.h:83 so the two cannot disagree.
AnsiString CheckFile(AnsiString szDir, AnsiString str);

#include <cstdlib>       // atoi/atof
#include "forms/fQwertyKey.h"  // AI(W906-FW-QWKEY2) 20260824: fQwertyKey extern for un-gated ShowQwertyKey sites (real since FW-QWKEY1 fc08e09; latent until HTEdit GATE (6) wiring)

// -----------------------------------------------------------------------------
//  6.1 bucket -- pure ShowQwertyKey one-liners, GATE WA-1 (see forms/
//  fConfiguration.h banner; OPENED 20260824). The 13 one-liners below are
//  live, latent until HTEdit GATE (6) wires the keyboard instance.
// -----------------------------------------------------------------------------

// -- edSetTempClick (golden cConfiguration.cpp:5962-5965) -- WA-1 -----------
void TfConfiguration::edSetTempClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824) -- see forms/fConfiguration.h
    // GATE WA-1 OPENED 20260824 (FW-QWKEY2): fQwertyKey real since FW-QWKEY1 (fc08e09).
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.0, 300.0);
}

// -- edSetTempKeyDown (golden cConfiguration.cpp:5967-5971) -- WA-1 ---------
void TfConfiguration::edSetTempKeyDown(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.0, 300.0);
}

// -- edE31_Fi1XClick (golden cConfiguration.cpp:5983-5986) -- WA-1 ----------
void TfConfiguration::edE31_Fi1XClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 6, true, 0.95, 1.05);
}

// -- edD25_60mmClick (golden cConfiguration.cpp:6440-6443) -- WA-1 ----------
void TfConfiguration::edD25_60mmClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.5, -0.5);   //2014-06-26 Dell for TSMC high-temp Load cell offset
}

// -- edD60_56mmClick (golden cConfiguration.cpp:6580-6583) -- WA-1 ----------
void TfConfiguration::edD60_56mmClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.5, -0.5);   //2014-06-26 Dell for TSMC high-temp Load cell offset
}

// -- edA32_1Click (golden cConfiguration.cpp:7611-7614) -- WA-1 -------------
void TfConfiguration::edA32_1Click(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL|N_NO_SPACE);
}

// -- edtC15Click (golden cConfiguration.cpp:7675-7678) -- WA-1 --------------
void TfConfiguration::edtC15Click(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    // GOLDEN ODDITY (recorded, not "fixed"): Min/Max args are reversed
    // (1440, 30) exactly as golden wrote them -- preserved verbatim.
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 1440, 30);
}

// -- edL09_Sh1LClick (golden cConfiguration.cpp:7680-7683) -- WA-1 ----------
void TfConfiguration::edL09_Sh1LClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    // GOLDEN ODDITY: Min/Max reversed (1000, -1000), preserved verbatim.
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1000, -1000);
}

// -- edN05_AmbTempClick (golden cConfiguration.cpp:7685-7688) -- WA-1 -------
void TfConfiguration::edN05_AmbTempClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 15.0, 50.0);
}

// -- edN06_PortClick (golden cConfiguration.cpp:7690-7693) -- WA-1 ----------
void TfConfiguration::edN06_PortClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}

// -- edN06_PasswordClick (golden cConfiguration.cpp:7695-7698) -- WA-1 ------
void TfConfiguration::edN06_PasswordClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_PASSWORD);
}

// -- edtSetIPSCQtyClick (golden cConfiguration.cpp:7700-7703) -- WA-1 -------
void TfConfiguration::edtSetIPSCQtyClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    // GOLDEN ODDITY: Min/Max reversed (60000, 0), preserved verbatim.
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 60000, 0);
}

// -- edN04_ModelClick (golden cConfiguration.cpp:7705-7708) -- WA-1 ---------
void TfConfiguration::edN04_ModelClick(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SPACE);
}

// -- edA22_2Click (golden cConfiguration.cpp:6593-6601) -- WA-1 (partial) ---
// Frank 20161109 add magnetic scale
void TfConfiguration::edA22_2Click(TObject *Sender)
{
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824) -- see forms/fConfiguration.h
    // GATE WA-1 OPENED 20260824. The edA22_3 nudge below is REAL and unaffected.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.01, 10.0);

    if(atof(edA22_3->Text.c_str())<atof(edA22_2->Text.c_str()))
    {
        edA22_3->Text=atof(edA22_2->Text.c_str())+0.01;
    }
}

// -- edA22_3Click (golden cConfiguration.cpp:6603-6607) -- WA-1 (partial) ---
// Frank 20161109 add magnetic scale
void TfConfiguration::edA22_3Click(TObject *Sender)
{
    double dCount=atof(edA22_2->Text.c_str())+0.01;
    // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see edSetTempClick above.
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, dCount, 10.0);
    (void)dCount;   // AI(W906-FW-QWKEY2) 20260824: gate opened, dCount consumed again; harmless cast kept
}

// -- EnableRMSFunc (golden cConfiguration.cpp:7726-7735) -- REAL, no gate ---
bool TfConfiguration::EnableRMSFunc()
{
    bool bret=false;
    bret=CosFunction.bEnableRMS ||
         (IniConfig.bShowLotInfo && (CUSTOMER_CODE==CC_SCC    ||                //Steven 20101102 : RMS
                                     CUSTOMER_CODE==CC_SCK     ||
                                     CUSTOMER_CODE==CC_AMD_M   ||               //Ifor 20200812 add: TF-AMD USE RMS
                                     CUSTOMER_CODE==CC_ETRENDTECH));            //Steven 20230302 : add for Etrendtech
    return bret;
}

// -----------------------------------------------------------------------------
//  6.2 bucket -- Tray/HP StringGrid cluster (read/edit/reload only; the
//  paired Update buttons -- sbUpdateTrayClick/sbUpdateHPClick -- are (b)
//  write-path and NOT declared/defined here).
// -----------------------------------------------------------------------------

// -- strngrdTraySelectCell (golden cConfiguration.cpp:6839-6844) -- REAL ----
void TfConfiguration::strngrdTraySelectCell(int ACol, int ARow)
{
    iSelTrayRow=ARow;
    iSelTrayCol=ACol;
}

// -- strngrdTrayDblClick (golden cConfiguration.cpp:6846-6849) -- REAL ------
void TfConfiguration::strngrdTrayDblClick()
{
    // AI(W906-FW3-Config-WA) 20260820: DEVIATION, see forms/fConfiguration.h
    // banner -- TSpeedButton::Click() is a real, compiling, tree-wide
    // documented offline no-op (vclcompat/Controls.h TControl::Click()),
    // NOT a missing-symbol gate. Translated as-is.
    btnModifyTray->Click();
}

// -- btnModifyTrayClick (golden cConfiguration.cpp:6851-6876) -- WA-1 (partial) --
void TfConfiguration::btnModifyTrayClick()
{
    if(iSelTrayRow>0 && iSelTrayCol>=0)
    {
        edtTemp->Text=strngrdTray->Cells[iSelTrayCol][iSelTrayRow];
        if(strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Package Type"))!=0 ||
           strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Group"))!=0     ||
           strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Memo"))!=0)
        {
            // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see forms/fConfiguration.h.
            fQwertyKey->ShowQwertyKey(edtTemp, N_NO_SYMBOL);
        }
        else if(strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Columns (X)"))!=0 ||
                strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("Rows (Y)"))!=0     ||
                strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("BlockNumberX"))!=0 ||
                strngrdTray->Cells[iSelTrayCol][0].AnsiPos(AnsiString("BlockNumberY"))!=0)
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_INTEGER, 0, true, 0, 1000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_DOUBLE, 2, true, 0.00, 1000.00);
        }
        strngrdTray->Cells[iSelTrayCol][iSelTrayRow]=edtTemp->Text;
    }
    btnModifyTray->Down=false;
}

// -- btnAddTrayClick (golden cConfiguration.cpp:6878-6888) -- REAL ----------
void TfConfiguration::btnAddTrayClick()
{
    int iRow=strngrdTray->RowCount;
    strngrdTray->RowCount=iRow+1;
    for(int j=0; j<strngrdTray->ColCount; j++)
    {
        strngrdTray->Cells[j][iRow]="";
    }
    btnAddTray->Down=false;
    strngrdTray->Row=strngrdTray->RowCount-1;
}

// -- btnDeleteTrayClick (golden cConfiguration.cpp:6890-6904) -- REAL -------
void TfConfiguration::btnDeleteTrayClick()
{
    if(iSelTrayRow<=0)
        return;
    Tag=atoi(strngrdTray->Cells[0][iSelTrayRow].c_str());
    for(int i=iSelTrayRow; i<strngrdTray->RowCount-1; i++)
    {
        for(int j=0; j<strngrdTray->ColCount; j++)
        {
            strngrdTray->Cells[j][i]=strngrdTray->Cells[j][i+1];
        }
    }
    strngrdTray->RowCount=strngrdTray->RowCount-1;
    btnDeleteTray->Down=false;
}

// -- sbtReloadTrayClick (golden cConfiguration.cpp:6931-6975) -- REAL -------
void TfConfiguration::sbtReloadTrayClick()
{
    TStringList *List=new TStringList();
    AnsiString S1, S2;
    int iPos=0;

    strngrdTray->ColCount=16;
    strngrdTray->Font->Size=10;
    strngrdTray->DefaultColWidth=80;
    strngrdTray->ColWidths[0]=200;

    for(int i=0; i<strngrdTray->RowCount; i++)
    {
        for(int j=0; j<16; j++)
            strngrdTray->Cells[j][i]="";
    }

    List->LoadFromFile(TrayTablePath);
    strngrdTray->RowCount=List->Count;

    for(int i=0; i<List->Count; i++)
    {
        int j=0;
        S1=List->Strings[i];
        S1=StringReplace(S1, "\"", "", TReplaceFlags()<<rfReplaceAll);
        do
        {
            iPos=S1.AnsiPos(",");
            if(iPos>0)
            {
                S2=S1.SubString(0, iPos-1);
                S1=S1.SubString(iPos+1, S1.Length());
                strngrdTray->Cells[j][i]=S2;
                j++;
            }
        }while(iPos>0);
    }

    if(strngrdTray->RowCount>1)
        strngrdTray->FixedRows=1;
    strngrdTray->FixedCols=0;
    List->Clear();
    delete List;
    sbtReloadTray->Down=false;
}

// -- strngrdHPSelectCell (golden cConfiguration.cpp:6977-6982) -- REAL ------
void TfConfiguration::strngrdHPSelectCell(int ACol, int ARow)
{
    iSelHPRow=ARow;
    iSelHPCol=ACol;
}

// -- strngrdHPDblClick (golden cConfiguration.cpp:6984-6987) -- REAL --------
void TfConfiguration::strngrdHPDblClick()
{
    // AI(W906-FW3-Config-WA) 20260820: DEVIATION, see strngrdTrayDblClick above.
    btnModifyHP->Click();
}

// -- btnModifyHPClick (golden cConfiguration.cpp:6989-7012) -- WA-1 (partial) --
void TfConfiguration::btnModifyHPClick()
{
    if(iSelHPRow>0 && iSelHPCol>=0)
    {
        edtTemp->Text=strngrdHP->Cells[iSelHPCol][iSelHPRow];
        if(strngrdHP->Cells[iSelHPCol][0].AnsiPos(AnsiString("Package Type"))!=0 ||
           strngrdHP->Cells[iSelHPCol][0].AnsiPos(AnsiString("Group"))!=0     ||
           strngrdHP->Cells[iSelHPCol][0].AnsiPos(AnsiString("Memo"))!=0)
        {
            // AI(W906-FW3-Config-WA) 20260820: GATE WA-1 (OPENED 20260824), see forms/fConfiguration.h.
            fQwertyKey->ShowQwertyKey(edtTemp, N_NO_SYMBOL);
        }
        else if(strngrdHP->Cells[iSelHPCol][0].AnsiPos(AnsiString("Columns (X)"))!=0 ||
                strngrdHP->Cells[iSelHPCol][0].AnsiPos(AnsiString("Rows (Y)"))!=0)
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_INTEGER, 0, true, 0, 1000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey(edtTemp, N_DOUBLE, 2, true, 0.00, 1000.00);
        }
        strngrdHP->Cells[iSelHPCol][iSelHPRow]=edtTemp->Text;
    }
    btnModifyHP->Down=false;
}

// -- btnAddHPClick (golden cConfiguration.cpp:7014-7024) -- REAL -----------
void TfConfiguration::btnAddHPClick()
{
    int iRow=strngrdHP->RowCount;
    strngrdHP->RowCount=iRow+1;
    for(int j=0; j<strngrdHP->ColCount; j++)
    {
        strngrdHP->Cells[j][iRow]="";
    }
    btnAddHP->Down=false;
    strngrdHP->Row=strngrdHP->RowCount-1;
}

// -- btnDeleteHPClick (golden cConfiguration.cpp:7026-7040) -- REAL --------
void TfConfiguration::btnDeleteHPClick()
{
    if(iSelHPRow<=0)
        return;
    Tag=atoi(strngrdHP->Cells[0][iSelHPRow].c_str());
    for(int i=iSelHPRow; i<strngrdHP->RowCount-1; i++)
    {
        for(int j=0; j<strngrdHP->ColCount; j++)
        {
            strngrdHP->Cells[j][i]=strngrdHP->Cells[j][i+1];
        }
    }
    strngrdHP->RowCount=strngrdHP->RowCount-1;
    btnDeleteHP->Down=false;
}

// -- sbtReloadHPClick (golden cConfiguration.cpp:7042-7086) -- REAL --------
void TfConfiguration::sbtReloadHPClick()
{
    TStringList *List=new TStringList();
    AnsiString S1, S2;
    int iPos=0;

    strngrdHP->ColCount=16;
    strngrdHP->Font->Size=10;
    strngrdHP->DefaultColWidth=80;
    strngrdHP->ColWidths[0]=200;

    for(int i=0; i<strngrdHP->RowCount; i++)
    {
        for(int j=0; j<16; j++)
            strngrdHP->Cells[j][i]="";
    }

    List->LoadFromFile(PlateTablePath);
    strngrdHP->RowCount=List->Count;

    for(int i=0; i<List->Count; i++)
    {
        int j=0;
        S1=List->Strings[i];
        S1=StringReplace(S1, "\"", "", TReplaceFlags()<<rfReplaceAll);
        do
        {
            iPos=S1.AnsiPos(",");
            if(iPos>0)
            {
                S2=S1.SubString(0, iPos-1);
                S1=S1.SubString(iPos+1, S1.Length());
                strngrdHP->Cells[j][i]=S2;
                j++;
            }
        }while(iPos>0);
    }

    if(strngrdHP->RowCount>1)
        strngrdHP->FixedRows=1;
    strngrdHP->FixedCols=0;
    List->Clear();
    delete List;
    sbtReloadHP->Down=false;
}

// =============================================================================
// FW-CFG-W1 -- InitConfigEdtList_ItemA / ItemB / ItemC (merged 20260825)
// =============================================================================
// AI(W906-FW-CFG-W1) 20260825: golden cConfiguration.cpp:378-977, transcribed
// VERBATIM (cp950 -> UTF-8). This family is a pure registration table: every
// line is `elConfig->Add(widget, &IniConfig.field, type, section, key, ...)`,
// and the port's HTEditList::Add signature (Public/HTEditList.h:187-199) is
// argument-for-argument identical to golden's, so no adaptation is needed and
// none was invented. Deviations, if any, are marked inline.
void TfConfiguration::InitConfigEdtList_ItemA()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;

    if(IniConfig.bVTESTFunction==true   ||
       CUSTOMER_CODE==CC_AMKOR_China    ||
       CUSTOMER_CODE==CC_ASE_KaohSiung  ||
       CUSTOMER_CODE==CC_KYEC_LEE       ||
       CUSTOMER_CODE==CC_QUALCOMM       ||
       IniConfig.bSPILFunction==true    ||
       CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20220215 : ASE-CL松諭要求鎖定A01
    {
        elConfig->Add(cbA01, &IniConfig.bA01AutoSwitchToOperatorMode,           ECBool,     "Function", "bAutoSwitchToOperatorMode",            bShow, bDisable, bFixedValue, 1);
    }
    else
    {
        elConfig->Add(cbA01, &IniConfig.bA01AutoSwitchToOperatorMode,           ECBool,     "Function", "bAutoSwitchToOperatorMode",            bShow, bEnable, bReadFromFile, (int)LastSet.bAutoSwitchToOperatorMode);
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150206 超過時間變成op模式
    {
        elConfig->Add(edA01,                &IniConfig.iA01ChangeOpTime,        ECInteger,  "Function",  "iChangeOpTime",   bShow, bDisable, bFixedValue, 60);
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_ASE_CL)                                         //wei 20150903 鎖定A01
    {
        elConfig->Add(edA01,                &IniConfig.iA01ChangeOpTime,        ECInteger,  "Function",  "iChangeOpTime",   bShow, bDisable, bFixedValue, 600);
    }
    else if(IniConfig.bSPILFunction==true)                                      //JerryYang 20200311 矽品國龍說固定180秒
    {
        elConfig->Add(edA01,                &IniConfig.iA01ChangeOpTime,        ECInteger,  "Function",  "iChangeOpTime",   bShow, bDisable, bFixedValue, 180);
    }
    else if(CUSTOMER_CODE==CC_AMKOR_China ||
            CUSTOMER_CODE==CC_QUALCOMM)
    {
        elConfig->Add(edA01,                &IniConfig.iA01ChangeOpTime,        ECInteger,  "Function",  "iChangeOpTime",   bShow, bEnable, bReadFromFile, 60,     false,  10,     180);
    }
    else if(IniConfig.bVTESTFunction==true)
    {
        elConfig->Add(edA01,                &IniConfig.iA01ChangeOpTime,        ECInteger,  "Function",  "iChangeOpTime",   bShow, bDisable, bFixedValue, 120);
    }
    else if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                   //Sam 20250220 : 北興俊堯要求時間要 Disable
    {
        elConfig->Add(edA01,                &IniConfig.iA01ChangeOpTime,        ECInteger,  "Function",  "iChangeOpTime",   bShow, bDisable, bReadFromFile, 60,     false,  10,     1000);
    }
    else
    {
        elConfig->Add(edA01,                &IniConfig.iA01ChangeOpTime,        ECInteger,  "Function",  "iChangeOpTime",   bShow, bEnable, bReadFromFile, 60,     false,  10,     1000);
    }

    if(CUSTOMER_CODE==CC_JCET ||                                                //JerryYang 20170705 (Steven) add for JCET 按start要切為OP權限改為選項
       CUSTOMER_CODE==CC_SJ_Semiconductor)                                      //RogerYang 20251224 張寧要求打開
        elConfig->Add(cbA01_1, &IniConfig.bA01PressStartAutoSwitchToOperatorMode, ECBool,   "Function",     "bPressStartAutoSwitchToOperatorMode",  bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA01_1, &IniConfig.bA01PressStartAutoSwitchToOperatorMode, ECBool,   "Function",     "bPressStartAutoSwitchToOperatorMode",  bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_SJ_Semiconductor)                                      //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
    {
        elConfig->Add(cbA01_2, &IniConfig.bA02DisableSaveParsWhenSwitchToOp,    ECBool,   "Function",     "b2DisableSaveParsWhenSwitchToOp",  bShow, bEnable, bReadFromFile, 1);
    }
    else
    {
        elConfig->Add(cbA01_2, &IniConfig.bA02DisableSaveParsWhenSwitchToOp,    ECBool,   "Function",     "b2DisableSaveParsWhenSwitchToOp",  bNoShow, bDisable, bFixedValue, 0);   //RogerYang 20260312 : Fix
    }

    if(IniConfig.bFTBin2RTBin)                                                  //ChungHung 20120912 add Bin Setting like Epson
    {
        if(CUSTOMER_CODE==CC_ASE_CL)                                            //JerryYang 20220215 : ASE-CL松諭要求
        {
            elConfig->Add(cbA02,    &IniConfig.bA02BinModelPrime,               ECBool,     "Function", "BinModelPrime",                        bShow, bDisable, bFixedValue, 0);
        }
        else
        {
            elConfig->Add(cbA02,    &IniConfig.bA02BinModelPrime,               ECBool,     "Function", "BinModelPrime",                        bShow, bEnable, bReadFromFile, 0);
        }
    }
    else
    {
        elConfig->Add(cbA02,    &IniConfig.bA02BinModelPrime,                   ECBool,     "Function", "BinModelPrime",                        bNoShow, bDisable, bFixedValue, 0);
    }

    if(CosFunction.bLockA03ByFile)                                              //JerryYang 20260504 : for SPIL -- A03 Enable
    {
        elConfig->Add(cbA03,    &IniConfig.bA03UseAfterHomeCarryAndSuckIcToRBin,ECBool,     "Function", "bA03UseAfterHomeCarryAndSuckIcToRBin", bShow, IniConfig.bA03_Enable, bReadFromFile, 0);
    }
    else if(CUSTOMER_CODE==CC_SCS ||
       CUSTOMER_CODE==CC_KYEC_LEE ||
       CUSTOMER_CODE==CC_ASE_M ||                                               //Ifor 20190926 : add ASEM強制開啟[A03]
       CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20200915 add: ASE SG 強制開啟[A03]
        elConfig->Add(cbA03,    &IniConfig.bA03UseAfterHomeCarryAndSuckIcToRBin,ECBool,     "Function", "bA03UseAfterHomeCarryAndSuckIcToRBin", bShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbA03,    &IniConfig.bA03UseAfterHomeCarryAndSuckIcToRBin,ECBool,     "Function", "bA03UseAfterHomeCarryAndSuckIcToRBin", bShow, bEnable, bReadFromFile, 0);

    elConfig->Add(cbA04,        &IniConfig.bA04LoaderTraySplitFailCanSkip,      ECBool,     "Function", "bA04LoaderTraySplitFailCanSkip",       bShow, bEnable, bReadFromFile, LastSet.bLoaderTraySplitFailCanSkip?"1":"0");
    elConfig->Add(cbA05,        &IniConfig.bA05UseAutoDocking,                  ECBool,     "Function", "bUseAutoDocking",                      bShow, bEnable, bReadFromFile, 0);                            //ChungHung 20120718 add UseAutoDocking Check Sensor
    if(IniConfig.bKoreaFunction)                                                //ChungHung 20130528 SCK要求AutoClean後要自動檢測是否Loader有補Tray
        elConfig->Add(cbA08,    &IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain, ECBool, "Function", "bLastLoaderAutoCleanOutAndCheckAgain", bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA08,    &IniConfig.bA08LastLoaderAutoCleanOutAndCheckAgain, ECBool, "Function", "bLastLoaderAutoCleanOutAndCheckAgain", bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //ChungHung 20130910 alter for SCK can close site by Index
    {
        elConfig->Add(cbA09,    &IniConfig.bA09_ByArmCloseSite,                 ECBool,     "Function", "bCloseSiteByIndexArm",                 bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(chkA09_1, &IniConfig.bA09_1_AutoCloseArm,                 ECBool,     "Function", "bA09_1_AutoCloseArm",                  bNoShow, bDisable, bFixedValue, 0);     //Steven 20220819 : 單Arm Site全關時, 就把Arm關了
    }
    else
    {
        elConfig->Add(cbA09,    &IniConfig.bA09_ByArmCloseSite,                 ECBool,     "Function", "bCloseSiteByIndexArm",                 bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(chkA09_1, &IniConfig.bA09_1_AutoCloseArm,                 ECBool,     "Function", "bA09_1_AutoCloseArm",                  bShow, bEnable, bReadFromFile, 0);      //Steven 20220819 : 單Arm Site全關時, 就把Arm關了
    }

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20170119 TSMC 命名ART==>ATR
    {
        gbA10->Caption="[A10] Auto Tray retest parameter (ATR)";
        cbA10->Caption="[A10-1] Enable ATR";
    }
    else
    {
        gbA10->Caption="[A10] Auto retest parameter (ART)";
        cbA10->Caption="[A10-1] Enable ART";
    }

    // AI(W906-FW-CFG-W1) 20260825: GATE (CFG1-fMain). golden reads
    // `fMain->cbRunStartMode->Text`, the run-mode combo on the MAIN form.
    // forms/fMain.h's TfMain does not carry that member -- its only mention is
    // a comment.
    //   cmd: grep -n "cbRunStartMode" forms/fMain.h -> 1 hit, a comment (20260825)
    // ACTIVE ARM: golden's own `else`, `cbA10->Enabled=true`, is emitted
    // unconditionally below. That is not a made-up default -- it is the branch
    // golden itself takes on every machine except a CC_KYEC_LEE handler that is
    // ALREADY running in an ART start mode, where golden greys out the "Enable
    // ART" checkbox so the operator cannot switch it off mid-mode. Losing that
    // one interlock is stated plainly rather than hidden: on KYEC_LEE, the
    // checkbox stays clickable. It is a UI enable flag, not a machine action,
    // and nothing in this port reads cbA10->Enabled back.
#if 0 // GATE (CFG1-fMain) -- fMain->cbRunStartMode has no port (grep + date above)
    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       (fMain->cbRunStartMode->Text.Pos("ART") ||
        fMain->cbRunStartMode->Text==StartModeName[rsmAutoRetest]))             //Ifor 20220506 Add ART 模式不可關閉ART
    {
        cbA10->Enabled=false;
    }
    else
#endif // GATE (CFG1-fMain)
    {
        cbA10->Enabled=true;
    }

    gbA10->Visible              =(USE_AUTO_RETEST==eartInstall &&               //ChungHung 20140317 add Auto Retest
                                  CUSTOMER_CODE!=CC_ASE_KaohSiung);
    if(USE_AUTO_RETEST==eartUninstall)                                          //ChungHung 20140317 add Auto Retest
        elConfig->Add(cbA10,    &IniConfig.bA10_AutoReTest,                     ECBool,     "Function", "bAutoReTest",                          bNoShow, bDisable, bFixedValue, 0);
    else
        elConfig->Add(cbA10,    &IniConfig.bA10_AutoReTest,                     ECBool,     "Function", "bAutoReTest",                          bShow, bEnable, bReadFromFile, 0);

    if(CUSTOMER_CODE==CC_SCK)                                                   //Steven 20171211 (Wei) : Auto correction for SCK ART
    {
        elConfig->Add(cbA10_3,  &IniConfig.bA10TestModeForART,                  ECBool,     "Function", "EnableA10TestModeForART",              bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbA10_5,  &IniConfig.bA10_5SCKART_AutoCorrection,         ECBool,     "Function", "bA10_5SCKART_AutoCorrection",          bShow, bEnable, bReadFromFile, 1);
    }
    else if(USE_AUTO_RETEST==eartInstall && CosFunction.bUseSCKART)
    {
        elConfig->Add(cbA10_3,  &IniConfig.bA10TestModeForART,                  ECBool,     "Function", "EnableA10TestModeForART",              bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbA10_5,  &IniConfig.bA10_5SCKART_AutoCorrection,         ECBool,     "Function", "bA10_5SCKART_AutoCorrection",          bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA10_3,  &IniConfig.bA10TestModeForART,                  ECBool,     "Function", "EnableA10TestModeForART",              bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbA10_5,  &IniConfig.bA10_5SCKART_AutoCorrection,         ECBool,     "Function", "bA10_5SCKART_AutoCorrection",          bNoShow, bEnable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_HANA_MICRON)                                           //JimmyChiu 20241023 HANA ART Function
    {
        elConfig->Add(cbA10_6,              &IniConfig.bA10_6_HANA_ART_TestMode_Enable,  ECBool,     "Function", "bA10_6_HANA_ART_TestMode_Enable",   bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbA10_6_HANA_ARTMode, &IniConfig.iA10_6_HANA_ART_TestMode,         ECInteger,  "Function", "iA10_6_HANA_ART_TestMode",          bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA10_6,              &IniConfig.bA10_6_HANA_ART_TestMode_Enable,  ECBool,     "Function", "bA10_6_HANA_ART_TestMode_Enable",   bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbA10_6_HANA_ARTMode, &IniConfig.iA10_6_HANA_ART_TestMode,         ECInteger,  "Function", "iA10_6_HANA_ART_TestMode",          bNoShow, bDisable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_AMKOR_Japan)                                           //RogerYang 20251108 : 瑞薩FTCT
    {
        elConfig->Add(cbA10_7,              &IniConfig.bA10_7_Renesas_FTCT,     ECBool,     "Function", "bA10_7_Renesas_FTCT",   bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA10_7,              &IniConfig.bA10_7_Renesas_FTCT,     ECBool,     "Function", "bA10_7_Renesas_FTCT",   bNoShow, bDisable, bFixedValue, 0);
    }

    palA10_ART->Visible         =false;
    elConfig->Add(edA10_2,      &IniConfig.iAutoRetestLimit,                    ECInteger,  "Auto Retest Parameter", "iAutoRetestLimit",        bShow, bEnable, bReadFromFile, 3,      false,  1,      10);
    elConfig->Add(edA10_3,      &IniConfig.iFailYieldRate_ART,                  ECInteger,  "Auto Retest Parameter", "iFailYieldRate_ART",      bShow, bEnable, bReadFromFile, 10,     false,  1,      100);
    elConfig->Add(edA10_4,      &IniConfig.iARTTrayArmSpeed,                    ECInteger,  "Auto Retest Parameter", "A10_4_ARTTrayArmSpeed",   bShow, bEnable, bReadFromFile, 30,     false,  1,      100);
    if(USE_AUTO_RETEST==eartInstall && CosFunction.bUseSCKART)                  //Steven 20161208 : ART need to set to 32binGS for ATK
        elConfig->Add(cbA10_3_ARTTestMode,  &IniConfig.iA10TestModeForART,      ECInteger,  "Function",  "A10TestModeForART",                   bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA10_3_ARTTestMode,  &IniConfig.iA10TestModeForART,      ECInteger,  "Function",  "A10TestModeForART",                   bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)             //2013-04-23    Dell     // 20140103 wei KYEC Barcode Reader
    {
        elConfig->Add(cbA11,    &IniConfig.bA11BarcodeTime,                     ECBool,     "Barcode Reader", "bBarcodeTime",                   bShow, bDisable, bFixedValue, 1);
        elConfig->Add(edA11,    &IniConfig.iA11BarcodeTime,                     ECInteger,  "Barcode Reader", "iBarcodeTime",                   bShow, bEnable, bReadFromFile, 60,     false,  10,     1000);          //20140310 wei : [A09]   Barcode Reader持續時間
    }
    else
    {
        elConfig->Add(cbA11,    &IniConfig.bA11BarcodeTime,                     ECBool,     "Barcode Reader", "bBarcodeTime",                   bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edA11,    &IniConfig.iA11BarcodeTime,                     ECInteger,  "Barcode Reader", "iBarcodeTime",                   bNoShow, bDisable, bFixedValue, 60);
    }

    if(USE_AUTO_RETEST==eartInstall)                                            //ChungHung 20141001 add
    {
        pnlA12->Visible=true;                                                   //wei 20150826 拍拍Tray X軸 Offset
        elConfig->Add(cbA12,    &IniConfig.bA12ClearLoaderDevice,               ECBool,     "Function", "bClearLoaderDevice",                   bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edA12,    &IniConfig.iClearLoaderCount,                   ECInteger,  "Function", "iClearLoaderCount",                    bShow, bEnable, bReadFromFile, 2,      false,  1,      10);            //wei 20150810 拍拍Tray次數設定
        elConfig->Add(edtA12_1, &IniConfig.iCleanLoaderOffset,                  ECInteger,  "Function", "iCleanLoaderOffset",                   bShow, bEnable, bReadFromFile, 0,      false,  0,      500);           //wei 20150826 拍拍Tray X軸 Offset
    }
    else
    {
        pnlA12->Visible=false;                                                  //wei 20150826 拍拍Tray X軸 Offset
        elConfig->Add(cbA12,    &IniConfig.bA12ClearLoaderDevice,               ECBool,     "Function", "bClearLoaderDevice",                   bNoShow, bDisable, bFixedValue, 0);
    }

    if(CosFunction.bEnableBarcodeReader)                                        //Frank 20150909 : CC_AMKOR 需要使用BarcodeReader讀取工作檔
        elConfig->Add(cbA14,    &IniConfig.bA14UseBarCodeSetWorkFile,           ECBool,     "Function", "bUseBarCodeSetWorkFile",               bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA14,    &IniConfig.bA14UseBarCodeSetWorkFile,           ECBool,     "Function", "bUseBarCodeSetWorkFile",               bNoShow, bDisable, bFixedValue, 0);

    if(CosFunction.bStartESDAutoDecayFunction &&                                //Ifor 20150924 :Add Auto Decay Test
       USE_NOVX3360==true)                                                      //Ifor 20210302 add:有安裝SIMCO才顯示與使用
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE && bEnable_KLT_Function==false)
        {
            elConfig->Add(cbA15,    &IniConfig.bA15AutoDecayTest,               ECBool,     "Function", "bAutoDecayTest",                       bShow, bDisable, bFixedValue, 1);    //Ifor 20220823 add: KYEC 樹榮要求[A15]強制開啟
        }
        else
        {
            elConfig->Add(cbA15,    &IniConfig.bA15AutoDecayTest,               ECBool,     "Function", "bAutoDecayTest",                       bShow, bEnable, bReadFromFile, 0);
        }
        elConfig->Add(edA15_ESDReportTime,      &IniConfig.dESDDataReportTime,  ECDouble,   "Index",    "dESDDataReportTime",                   bShow, bEnable, bReadFromFile, 10.0,   false,  0.01,   600.0);         //2~600秒 //Ifor 20160321 : edESDReportTime
        elConfig->Add(cbA15_1,    &IniConfig.bA15_1ESDGiveWayFunction,          ECBool,     "Function", "bA15_1ESDGiveWayFunction",             bShow, bEnable, bReadFromFile, 0);   //Eastsun 20260521 整合
    }
    else
    {
        elConfig->Add(cbA15,    &IniConfig.bA15AutoDecayTest,                   ECBool,     "Function", "bAutoDecayTest",                       bNoShow, bDisable, bFixedValue, 0);
        edA15_ESDReportTime->Visible=false;
        elConfig->Add(cbA15_1,    &IniConfig.bA15_1ESDGiveWayFunction,          ECBool,     "Function", "bA15_1ESDGiveWayFunction",             bNoShow, bDisable, bFixedValue, 0);  //Eastsun 20260521 整合
    }

    if(CUSTOMER_CODE==CC_KYEC_CHEN)                                             //wei 20150831
    {
        lblA16->Visible=true;
        elConfig->Add(chA16,    &IniConfig.bA16ContactTestDropContact,          ECBool,     "Function", "bContactTestDropContact",              bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        lblA16->Visible=false;
        elConfig->Add(chA16,    &IniConfig.bA16ContactTestDropContact,          ECBool,     "Function", "bContactTestDropContact",              bNoShow, bDisable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_ASE_M ||                                               //Ifor 20190618 : add ASEM 強制 Disable RESET Button
       CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20200915 add: ASE SG 強制 Disable RESET Button
    {
        elConfig->Add(cbA17_1,  &IniConfig.bA17RESETButtonDisable,              ECBool,     "Function", "bRESETButtonDisable",                  bShow, bDisable, bFixedValue, 1);
    }
    else
    {
        elConfig->Add(cbA17_1,  &IniConfig.bA17RESETButtonDisable,              ECBool,     "Function", "bRESETButtonDisable",                  bShow, bEnable, bReadFromFile, 0);  //kevin 20151113 RESET 按鍵使用
    }

    if(CUSTOMER_CODE==CC_UMC)                                                   //JimmuyChiu 20211012   Add Reset->Clean out with no testing
        elConfig->Add(cbA17_2,  &IniConfig.bA17_1RESETCleanOutWithoutTest,      ECBool,     "Function", "bA17_1RESETCleanOutWithoutTest",       bShow, bDisable, bReadFromFile, 1);
    else
        elConfig->Add(cbA17_2,  &IniConfig.bA17_1RESETCleanOutWithoutTest,      ECBool,     "Function", "bA17_1RESETCleanOutWithoutTest",       bShow, bEnable, bReadFromFile, 0);

    if(CosFunction.bUsePMAlarmFunction)                                         //wei 20160225 PMAlarmFunction
        elConfig->Add(cbA19,    &IniConfig.bA19UsePMAlarmFunction,              ECBool,     "Function", "bUsePMAlarmFunction",                  bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA19,    &IniConfig.bA19UsePMAlarmFunction,              ECBool,     "Function", "bUsePMAlarmFunction",                  bNoShow, bDisable, bFixedValue, 0);

    elConfig->Add(cbA20_1,      &IniConfig.bA20_1CheckRTCFunction,              ECBool,     "Function", "bA20_1CheckRTCFunction",               bNoShow, bEnable, bFixedValue, 0);  //wei 20171023 Disable Start比對
    elConfig->Add(cbA20_2,      &IniConfig.bA20_2CheckTrayIDFunction,           ECBool,     "Function", "bA20_2CheckTrayIDFunction",            bNoShow, bEnable, bFixedValue, 0);
    elConfig->Add(cbA20_3,      &IniConfig.bA20_3CheckAutocleanFunction,        ECBool,     "Function", "bA20_3CheckAutocleanFunction",         bNoShow, bEnable, bFixedValue, 0);
    elConfig->Add(cbA20_4,      &IniConfig.bA20_4CheckContsFailFunction,        ECBool,     "Function", "bA20_4CheckContsFailFunction",         bNoShow, bEnable, bFixedValue, 0);
    elConfig->Add(cbA20_5,      &IniConfig.bA20_5CheckOCRFunction,              ECBool,     "Function", "bA20_5CheckOCRFunction",               bNoShow, bEnable, bFixedValue, 0);

    if(USE_ROTATE_KIT==1 && iRotate_Type!=eCynRotate)                           //JerryYang 20160825 Rotate sensor偵測異常要先試著旋轉三次再跳alarm
        elConfig->Add(cbA21,    &IniConfig.bA21RotateDetectErrNeedShake,        ECBool,     "Function", "bRotateDetectErrNeedShake",            bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA21,    &IniConfig.bA21RotateDetectErrNeedShake,        ECBool,     "Function", "bRotateDetectErrNeedShake",            bNoShow, bEnable, bFixedValue, 0);

    if(USE_MAGNETIC_SCALE)
    {
        gbA22->Visible=true;
        elConfig->Add(cbA22_1,  &IniConfig.bA22MagneticScale,                   ECBool,     "Function", "bA22MagneticScale",                    bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edA22_2,  &IniConfig.dA22MagneticScaleKeepRunRange,       ECDouble,   "Function", "dMagneticScaleKeepRunRange",           bShow, bEnable, bReadFromFile, 0.01,   true,   0.01,   10.0);
        elConfig->Add(edA22_3,  &IniConfig.dA22MagneticScaleStopRunRange,       ECDouble,   "Function", "dMagneticScaleStopRunRange",           bShow, bEnable, bReadFromFile, 0.01,   true,   0.01,   10.0);
    }
    else
    {
        gbA22->Visible=false;
        elConfig->Add(cbA22_1,  &IniConfig.bA22MagneticScale,                   ECBool,     "Function", "bA22MagneticScale",                    bNoShow, bEnable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_JCET)                                                  //JerryYang 20170421 (Steven) JCET吳如春要求必須輸入lot no才能start   //JerryYang 20170706 (Steven) 重新啟用A23
        elConfig->Add(cbA23,    &IniConfig.bA23CheckLotNoInSLTReport,           ECBool,     "Function", "bA23CheckLotNoInSLTReport",            bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA23,    &IniConfig.bA23CheckLotNoInSLTReport,           ECBool,     "Function", "bA23CheckLotNoInSLTReport",            bNoShow, bEnable, bFixedValue, 0);

    if(CosFunction.bUseAutoBackUpSetupFile)                                     //Ifor 20170508 (wei) add Auto BackUp Setup File ,& Last Data
        elConfig->Add(cbA24,    &IniConfig.bA24AutoBackupSetupFile,             ECBool,     "Function", "bA24AutoBackupSetupFile",              bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbA24,    &IniConfig.bA24AutoBackupSetupFile,             ECBool,     "Function", "bA24AutoBackupSetupFile",              bNoShow, bEnable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20170916 (Steven) 移植超豐外部呼叫執行檔功能 form HT-7045
    {
        grpA25->Visible=true;
        elConfig->Add(edA25_1,  &IniConfig.asA25RunExecutFile,                  ECText,     "Function", "asA25RunExecutFile",                   bShow, bEnable, bReadFromFile, "C:\\GTK\\Switch.exe");
        elConfig->Add(edA25_2,  &IniConfig.asA25RunExecutButtonName,            ECText,     "Function", "asA25RunExecutButtonName",             bShow, bEnable, bReadFromFile, "SWITCH");
    }
    else
    {
        grpA25->Visible=false;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //KaiChen 20171225 (Steven)：Add Speed Display
        elConfig->Add(cbA26,    &IniConfig.bA26MotorSpeedSortDisplay,           ECBool,     "Function", "bA26MotorSpeedSortDisplay",            bShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbA26,    &IniConfig.bA26MotorSpeedSortDisplay,           ECBool,     "Function", "bA26MotorSpeedSortDisplay",            bShow, bEnable, bReadFromFile, 0);

    spbA27->Visible             =(CosFunction.bConfigStandard);                 //KaiChen 20171113 ：超豐 將 config.ini 另外存成 config_Standard.ini      //JimmyChiu 20220114 CUSTOMER_CODE==CC_Greatek => CosFunction.bConfigStandard
    elConfig->Add(cbA27,        &IniConfig.bA27EnableLightScale,                ECBool,     "Function", "bA27EnableLightScale",                 bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(cbA27_1,      &IniConfig.bA27_1LogEnableLightScaleData,       ECBool,     "Function", "bA27_1LogEnableLightScaleData",        bShow, bEnable, bReadFromFile, 0);  //KaiChen 20171228 ：Log Light Scale Data
    if(CUSTOMER_CODE==CC_Greatek)
    {
        grpA28->Visible=true;                                                   //KaiChen 20171111 (Steven) ：超豐 開啟指定路徑的 HTML 檔案(PM SOP)
        elConfig->Add(edtA28_1, &IniConfig.asA28_1ShowPmSopReadFilePath,        ECText,     "Function", "asA26_1ShowPmSopReadFilePath",         bShow, bEnable, bReadFromFile, "");     //Sam 20220926 : 整理
    }
    else
    {
        grpA28->Visible=false;
        elConfig->Add(edtA28_1, &IniConfig.asA28_1ShowPmSopReadFilePath,        ECText,     "Function", "asA26_1ShowPmSopReadFilePath",         bNoShow, bEnable, bFixedValue, "");
    }

    elConfig->Add(cbA29,        &IniConfig.bA29EnableAutoCleanFunction,         ECBool,     "Function", "bA29EnableAutoCleanFunction",          bNoShow, bEnable, bFixedValue, 0);  //wei 20171023 Auto clean開關

    if(CosFunction.bSetupTeach)                                                 //JerryYang 20180921 Setup Teach功能
    {
        elConfig->Add(cbA30,    &IniConfig.bA30SetupTeachFunction,              ECBool,     "Function", "bA30SetupTeachFunction",               bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA30,    &IniConfig.bA30SetupTeachFunction,              ECBool,     "Function", "bA30SetupTeachFunction",               bNoShow, bEnable, bFixedValue, 0);
    }

    if(USE_AutoCleanIonFan==1 && CosFunction.bIOTriggerIonFanAutoClean)         //Isaac 20210609 : IO觸發IonFan清針
    {
        gbA31->Visible=true;
        elConfig->Add(cbA31,    &IniConfig.bA31EnableAutoCleanIonFanFunction,   ECBool, "Function", "bA33EnableAutoCleanIonFanFunction",          bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA31_1,  &IniConfig.bA31AutoCleanIonFanInitialStart,     ECBool, "Function", "bA33AutoCleanIonFanFunctionInitialStart",    bShow, bEnable, bReadFromFile, 1);
    }
    else
    {
        gbA31->Visible=false;
        elConfig->Add(cbA31,    &IniConfig.bA31EnableAutoCleanIonFanFunction,   ECBool, "Function", "bA33EnableAutoCleanIonFanFunction",          bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbA31_1,  &IniConfig.bA31AutoCleanIonFanInitialStart,     ECBool, "Function", "bA33AutoCleanIonFanFunctionInitialStart",    bNoShow, bDisable, bFixedValue, 0);
    }

    grpA32_1->Visible=false;
    if(IniConfig.bSIGURDFunction)
    {
        grpA32->Visible=true;
        elConfig->Add(cbA32,    &IniConfig.bA32EnableFTPAutomation,             ECBool,     "Function", "bA32EnableFTPAutomation",              bShow, bEnable, bReadFromFile, 1);   //KaiChen 20190530 ：Sigurd FTP Automation
        elConfig->Add(edA32_1,  &IniConfig.asA32_1_HandlerID,                   ECText,     "Function", "asA32_1_HandlerID",                    bShow, bEnable, bReadFromFile, "");    //KaiChen 20190530 ：Sigurd FTP Automation
        elConfig->Add(cbA32_2,  &IniConfig.bA32_2ReturnHandlerID2OI,            ECBool,     "Function", "bA32_2ReturnHandlerID2OI",             bShow, bEnable, bReadFromFile, 1);   //KaiChen 20200618 ：矽格，可以選擇是否回傳 HandlerID 給 OI

        if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                  //Sam 20220620 : 中興廠新增 A32-3 功能 for 93K function
            elConfig->Add(cbA32_3, &IniConfig.bA32_3For93KFunction,             ECBool,     "Function", "bA32_3For93KFunction",                 bShow,  bEnable, bReadFromFile, 0);
        else
            elConfig->Add(cbA32_3, &IniConfig.bA32_3For93KFunction,             ECBool,     "Function", "bA32_3For93KFunction",                 bNoShow,bEnable, bFixedValue, 0);

        elConfig->Add(cbA32_01, &IniConfig.bA32EnableCheckList[eCL_Temperature],ECBool, "FTPAutomation", "CheckList_Enable_Temperature",                bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_02, &IniConfig.bA32EnableCheckList[eCL_Alarm],      ECBool, "FTPAutomation", "CheckList_Enable_Alarm",                      bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_03, &IniConfig.bA32EnableCheckList[eCL_FT_Yield],   ECBool, "FTPAutomation", "CheckList_Enable_FT_Yield",                   bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_04, &IniConfig.bA32EnableCheckList[eCL_SiteMapping],ECBool, "FTPAutomation", "CheckList_Enable_SiteMapping",                bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_05, &IniConfig.bA32EnableCheckList[eCL_Speed],      ECBool, "FTPAutomation", "CheckList_Enable_Speed",                      bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_06, &IniConfig.bA32EnableCheckList[eCL_Contact],    ECBool, "FTPAutomation", "CheckList_Enable_Contact",                    bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_07, &IniConfig.bA32EnableCheckList[eCL_Category],   ECBool, "FTPAutomation", "CheckList_Enable_Category",                   bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_08, &IniConfig.bA32EnableCheckList[eCL_BinSetting], ECBool, "FTPAutomation", "CheckList_Enable_BinSetting",                 bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_09, &IniConfig.bA32EnableCheckList[eCL_TrayForm],   ECBool, "FTPAutomation", "CheckList_Enable_TrayForm",                   bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(cbA32_10, &IniConfig.bA32EnableCheckList[eCL_HotPlate],   ECBool, "FTPAutomation", "CheckList_Enable_HotPlate",                   bShow, bEnable, bReadFromFile, 1);

        elConfig->Add(cbA33,    &IniConfig.bA33SetICToErrBinAfterOutShtLossIC,  ECBool, "Function", "bA33SetMachineIC2ErrorBinAfterOutShuttleLossIC",   bShow, bEnable, bReadFromFile, 0);  //KaiChen 20200304 ：矽格-湖口，要求OutShuttle Loss IC 時機台上的IC放到R道
    }
    else
    {
        grpA32->Visible=false;
        elConfig->Add(cbA32,    &IniConfig.bA32EnableFTPAutomation,             ECBool, "Function", "bA32EnableFTPAutomation",                          bNoShow, bEnable, bFixedValue, 0);  //KaiChen 20190530 ：Sigurd FTP Automation
        elConfig->Add(cbA32_2,  &IniConfig.bA32_2ReturnHandlerID2OI,            ECBool, "Function", "bA32_2ReturnHandlerID2OI",                         bNoShow, bEnable, bFixedValue, 0);  //KaiChen 20200618 ：矽格，可以選擇是否回傳 HandlerID 給 OI
        elConfig->Add(cbA33,    &IniConfig.bA33SetICToErrBinAfterOutShtLossIC,  ECBool, "Function", "bA33SetMachineIC2ErrorBinAfterOutShuttleLossIC",   bNoShow, bEnable, bFixedValue, 0);  //KaiChen 20200304 ：矽格-湖口，要求OutShuttle Loss IC 時機台上的IC放到R道
    }

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20220215 : 松諭要求的功能Out shuttle lose IC 以及 out arm pick up error set to error bin
    {
        elConfig->Add(cbA35,    &IniConfig.bA35SetErrBinWhenOutShtLoseAndPickupErr, ECBool, "Function", "bA35SetErrBinWhenOutShtLoseAndOutShtPickupErr", bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA35,    &IniConfig.bA35SetErrBinWhenOutShtLoseAndPickupErr, ECBool, "Function", "bA35SetErrBinWhenOutShtLoseAndOutShtPickupErr", bNoShow, bEnable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_Microchip_Phil)                                        //JerryYang 20210901 : Microchip要求開安全門要分ERROR BIN
    {
        elConfig->Add(cbA36,    &IniConfig.bA36OpenDoorSetErrBin,               ECBool, "Function", "bA36OpenDoorSetErrBin",        bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA36,    &IniConfig.bA36OpenDoorSetErrBin,               ECBool, "Function", "bA36OpenDoorSetErrBin",        bNoShow, bEnable, bFixedValue, 0);
    }

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220923 : add SPIL ART LOT START/LOT END timeout機制
    {
        elConfig->Add(cbA37,    &IniConfig.bA37LotStartLotEnd,                  ECBool, "Function", "bA32LotStartLotEnd",           bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA37,    &IniConfig.bA37LotStartLotEnd,                  ECBool, "Function", "bA32LotStartLotEnd",           bNoShow, bEnable, bFixedValue, 0);
    }

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220923 : add for SLT lot summary
    {
        elConfig->Add(cbA38,    &IniConfig.bA38_SLT_Summary,                    ECBool, "Function", "bA35_SLT_Summary",             bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA38,    &IniConfig.bA38_SLT_Summary,                    ECBool, "Function", "bA35_SLT_Summary",             bNoShow, bEnable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_ASE_M)                                                 //Ifor 20230420 add:ASEM要求新增Run Status Log Function
    {
        elConfig->Add(cbA39,    &IniConfig.bA39RecordRunState,                  ECBool, "Function", "bA39RecordRunState",           bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edA39,    &IniConfig.iA39RecordTime,                      ECInteger, "Function", "iA39RecordTime",            bShow, bEnable, bReadFromFile, 5,     false,  5,     1000);          //20140310 wei : [A09]   Barcode Reader持續時間
    }
    else
    {
        elConfig->Add(cbA39,    &IniConfig.bA39RecordRunState,                  ECBool, "Function", "bA39RecordRunState",           bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edA39,    &IniConfig.iA39RecordTime,                      ECInteger, "Function", "iA39RecordTime",            bNoShow, bDisable, bReadFromFile, 60,     false,  5,     1000);          //20140310 wei : [A09]   Barcode Reader持續時間
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE && bEnable_KLT_Function==false)               //Ifor 20201126 add:Contact 頁面開啟時不記錄2D 資料
        elConfig->Add(cbA40,    &IniConfig.bA40DoNotRecord2DDataWhenContactShow,ECBool, "Function", "bA40DoNotRecord2DDataWhenContactShow", bShow, bEnable, bReadFromFile, 1);
    else
        elConfig->Add(cbA40,    &IniConfig.bA40DoNotRecord2DDataWhenContactShow,ECBool, "Function", "bA40DoNotRecord2DDataWhenContactShow", bNoShow, bEnable, bFixedValue, 0);

    if(CosFunction.bNonCenterModeCanUseShtOffset)                               //Steven 20200715 : for Tinton
    {
        elConfig->Add(chkA50,        &IniConfig.bA50Enable1x4BiasYOffset,       ECBool, "Function", "bA32Enable1x4BiasYOffset",     bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(chkA50,        &IniConfig.bA50Enable1x4BiasYOffset,       ECBool, "Function", "bA32Enable1x4BiasYOffset",     bNoShow, bEnable, bFixedValue, 0);
    }

    if(CosFunction.bCanDisableQAMode)                                           //JerryYang 20200312 EQC mode新增function on/off，功能關閉時無法切EQC mode
    {
        elConfig->Add(cbA51,        &IniConfig.bA51EnableEQCMode,               ECBool, "Function", "bA31EnableEQCMode",            bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA51,        &IniConfig.bA51EnableEQCMode,               ECBool, "Function", "bA31EnableEQCMode",            bNoShow, bEnable, bFixedValue, 0);
    }

    if(CosFunction.bUsePMAlarmFunction && CosFunction.bOEEFunction)             //JimmyChiu 20230315 : PM Alarm Update From Server by FTP
    {
        grpA55->Visible=true;
        elConfig->Add(cbA55, &IniConfig.bA55EnablePMAlarmUpdateFromServerbyFTP, ECBool, "Function", "bA55EnablePMAlarmUpdateFromServerbyFTP", bShow, bEnable, bReadFromFile, 1);
    }
    else
    {
        grpA55->Visible=false;
        elConfig->Add(cbA55, &IniConfig.bA55EnablePMAlarmUpdateFromServerbyFTP, ECBool, "Function", "bA55EnablePMAlarmUpdateFromServerbyFTP", bNoShow, bDisable, bFixedValue, 0);
    }

    if(CosFunction.bManualSteplAutoTeach)                                       //JimmyChiu 20211020 : Auto alignment mode
    {
        gbA56->Visible=true;
        elConfig->Add(cbA56_1,  &IniConfig.bA56EnableAutoTeachFunciton,                 ECBool,     "Function",    "bA56EnableAutoTeachFunciton",              bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(edA56_2,  &IniConfig.iA56ShuttlePickUpOffsetWhenAutoTeach,        ECInteger,  "Function",    "iA56ShuttlePickUpOffsetWhenAutoTeach",     bShow, bEnable, bReadFromFile, 1,   true,   1,   2000);
        elConfig->Add(edA56_3,  &IniConfig.iA56SocketPickUpOffsetWhenAutoTeachOnly,     ECInteger,  "Function",    "iA56SocketPickUpOffsetWhenAutoTeachOnly",  bShow, bEnable, bReadFromFile, 1,   true,   1,   2000);
    }
    else
    {
        gbA56->Visible=false;
        elConfig->Add(cbA56_1,  &IniConfig.bA56EnableAutoTeachFunciton,                 ECBool,     "Function",    "bA56EnableAutoTeachFunciton",              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edA56_2,  &IniConfig.iA56ShuttlePickUpOffsetWhenAutoTeach,        ECInteger,  "Function",    "iA56ShuttlePickUpOffsetWhenAutoTeach",     bNoShow, bDisable, bFixedValue, 1);
        elConfig->Add(edA56_3,  &IniConfig.iA56SocketPickUpOffsetWhenAutoTeachOnly,     ECInteger,  "Function",    "iA56SocketPickUpOffsetWhenAutoTeachOnly",  bNoShow, bDisable, bFixedValue, 1);
    }

    gbA57->Visible=(CosFunction.bSaveArmSpeedByMachine ||
                    CosFunction.bSaveTemperatureByMachine);                     //JimmyChiu 20220618 : save by machine
    elConfig->Add(cbA57_1,  &IniConfig.bA57_1SaveArmSpeedByMachine,             ECBool, "Function", "bA57_1SaveArmSpeedByMachine",      (CosFunction.bSaveArmSpeedByMachine)?bShow:bNoShow, bEnable,    (CosFunction.bSaveArmSpeedByMachine)?bReadFromFile:bFixedValue,     0);
    elConfig->Add(cbA57_2,  &IniConfig.bA57_2SaveTemperatureByMachine,          ECBool, "Function", "bA57_2SaveTemperatureByMachine",   (CosFunction.bSaveTemperatureByMachine)?bShow:bNoShow, bEnable, (CosFunction.bSaveTemperatureByMachine)?bReadFromFile:bFixedValue,  0);
    elConfig->Add(cbA57_3,  &IniConfig.bA57_3SaveOffsetByMachine,               ECBool, "Function", "bA57_3SaveOffsetByMachine",        (CosFunction.bSaveOffsetByMachine)?bShow:bNoShow, bEnable,      (CosFunction.bSaveOffsetByMachine)?bReadFromFile:bFixedValue,       0);

    if(IniConfig.bSPILFunction==true)                                           //Jimmychiu 20230925 : Show Close Sites Alarm When Start
    {
        elConfig->Add(cbA58,    &IniConfig.bShowCloseSiteAlarmWhenStart,        ECBool,    "PrecautionRecord", "bShowCloseSiteAlarmWhenStart", bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA58,    &IniConfig.bShowCloseSiteAlarmWhenStart,        ECBool,    "PrecautionRecord", "bShowCloseSiteAlarmWhenStart", bNoShow, bDisable, bFixedValue, 0);
    }

    if(CosFunction.bEnableHandlerResultServer)                                  //Sam 20240304 : 新增 AMR 功能
    {
        grpA60->Visible=true;
        elConfig->Add(cbA60_1,  &IniConfig.bA60EnableAMR,                       ECBool,"Function", "bA60EnableAMR",             bShow, bEnable, bReadFromFile, 0);
        // AI(W906-FW-CFG-W1) 20260825: GATE (CFG1-Font). `->Font->Color` is a
        // pure rendering property. vclcompat::TLabel (Controls.h:274-280)
        // carries Caption and Color but no Font object, which is that facade's
        // stated scope call ("no rendering"), not an oversight -- vclcompat/
        // Controls.h:180 does define a TFont, but no widget owns one.
        //   cmd: grep -n "Font" vclcompat/Controls.h -> only the standalone
        //        class TFont at :180; no widget member anywhere (20260825)
        // ACTIVE ARM: the five labels still exist and still register nothing --
        // only their blue tint is lost. The five elConfig_byRecipe->Add calls
        // right below, which are the actual purpose of this branch, all run.
#if 0 // GATE (CFG1-Font) -- vclcompat::TLabel has no ->Font (grep + date above)
        lblA60_1->Font->Color=clBlue;
        lblA60_2->Font->Color=clBlue;
        lblA60_3->Font->Color=clBlue;
        lblA60_4->Font->Color=clBlue;
        lblA60_5->Font->Color=clBlue;
#endif // GATE (CFG1-Font)
        elConfig_byRecipe->Add(edA60_1,     &IniConfig.iA60NotifyQty[0],        ECInteger,  "Function",     "iA60NotifyQtyLoader",    bShow, bEnable, bReadFromFile, 10     ,false, 1,       100);
        elConfig_byRecipe->Add(edA60_2,     &IniConfig.iA60NotifyQty[3],        ECInteger,  "Function",     "iA60NotifyQtyAuto1",     bShow, bEnable, bReadFromFile, 10     ,false, 1,       100);
        elConfig_byRecipe->Add(edA60_3,     &IniConfig.iA60NotifyQty[4],        ECInteger,  "Function",     "iA60NotifyQtyAuto2",     bShow, bEnable, bReadFromFile, 10     ,false, 1,       100);
        elConfig_byRecipe->Add(edA60_4,     &IniConfig.iA60NotifyQty[5],        ECInteger,  "Function",     "iA60NotifyQtyAuto3",     bShow, bEnable, bReadFromFile, 10     ,false, 1,       100);
        elConfig_byRecipe->Add(edA60_5,     &IniConfig.iA60LoaderQtyAtOneTime,  ECInteger,  "Function",     "iA60QtyAtOneTime",       bShow, bEnable, bReadFromFile, 9      ,false, 1,       100);
    }
    else
    {
        grpA60->Visible=false;
        elConfig->Add(cbA60_1,  &IniConfig.bA60EnableAMR,                       ECBool,"Function", "bA60EnableAMR",             bNoShow, bEnable, bFixedValue, 0);
    }

    if(USE_COVER_TRAYID!=tCIDNotUse)                                            //JerryYang 20250304 : A65有安裝硬體才Show出來
    {
        elConfig->Add(cbA65,    &IniConfig.bA65_BundleIDList,                   ECBool,    "Function",          "bA65_BundleIDList",           bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbA68,    &IniConfig.bA68_AutoLoadUnload,                 ECBool,    "Function",          "bA68_AutoLoadUnload",         bShow, bEnable, bReadFromFile, 0);   //JerryYang 20250220 : add
        elConfig->Add(cbA69,    &IniConfig.bA69_ScanBinLabelToBinCodeEdit,     ECBool,    "Function",          "bA69_ScanBinLabelToBinCodeEdit", bShow, bEnable, bReadFromFile, 0);   //JerryYang 20260409 : add
    }
    else
    {
        elConfig->Add(cbA65,    &IniConfig.bA65_BundleIDList,                   ECBool,    "Function",          "bA65_BundleIDList",           bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbA68,    &IniConfig.bA68_AutoLoadUnload,                 ECBool,    "Function",          "bA68_AutoLoadUnload",         bNoShow, bDisable, bFixedValue, 0);  //JerryYang 20250220 : add
        elConfig->Add(cbA69,    &IniConfig.bA69_ScanBinLabelToBinCodeEdit,     ECBool,    "Function",          "bA69_ScanBinLabelToBinCodeEdit", bNoShow, bDisable, bFixedValue, 0);  //JerryYang 20260409 : add
    }

    elConfig->Add(cbA66,        &IniConfig.bA66_2D_Sort,                        ECBool,    "Function",          "bA66_2D_Sort",                bShow, bEnable, bReadFromFile, 0);

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //Jeff 20241001 add Continuous Mode disable  clean MUBA
       elConfig->Add(cbA61,    &IniConfig.bA61DisableCleanMUBA,                 ECBool, "Function", "bA61DisableCleanMUBA",            bShow, bEnable, bReadFromFile, 0);
    else
       elConfig->Add(cbA61,    &IniConfig.bA61DisableCleanMUBA,                 ECBool, "Function", "bA61DisableCleanMUBA",            bNoShow, bDisable, bReadFromFile, 0);
    elConfig->Add(cbA62,       &IniConfig.bA62bUseStopMachineArmHome,           ECBool, "Function", "bA62bUseStopMachineArmHome",      bShow, bEnable, bReadFromFile, 0);//Ztex 2024.10.30 Add Use Stop Machine In/Out Arm Need To Home

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20241028 : 矽品彰化要求 特定ALARM要觸發ONE CYCLE
    {
        elConfig->Add(cbA67,    &IniConfig.bA67TriggerOneCycleWhenAlarm,        ECBool,    "Function", "bA67TriggerOneCycleWhenAlarm", bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA67,    &IniConfig.bA67TriggerOneCycleWhenAlarm,        ECBool,    "Function", "bA67TriggerOneCycleWhenAlarm", bNoShow, bDisable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_GIGAS)
    {
        elConfig->Add(cbA71,    &IniConfig.bA71UseBackupNowRecipe,              ECBool,    "Function",     "bA71UseBackupNowRecipe",            bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(edA71,    &IniConfig.sA71FolderPath,                      ECText,    "Function",     "sA71FolderPath",                    bShow, bEnable, bReadFromFile, "D:\\Run");
    }
    else
    {
        elConfig->Add(cbA71,    &IniConfig.bA71UseBackupNowRecipe,              ECBool,    "Function",     "bA71UseBackupNowRecipe",            bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edA71,    &IniConfig.sA71FolderPath,                      ECText,    "Function",     "sA71FolderPath",                    bNoShow, bDisable, bFixedValue, "D:\\Run");
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Eastsun 20260519 整合
    {
        elConfig->Add(cbA72,  &IniConfig.bA72_FixAOIButton,               ECBool, "Function", "bA72_FixAOIButton",        bShow, bEnable,      bReadFromFile,       1);
    }
    else
    {
        elConfig->Add(cbA72,  &IniConfig.bA72_FixAOIButton,               ECBool, "Function", "bA72_FixAOIButton",        bNoShow, bEnable,      bReadFromFile,       0);
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)   //EastSun 20260508 : A41 KYEC user manage FT/RT Bin Share (skip LotStart override)
    {
        elConfig->Add(cbA73,    &IniConfig.bA73UserOverrideFTRTBin,            ECBool, "Function", "bA73UserOverrideFTRTBin",         bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbA73,    &IniConfig.bA73UserOverrideFTRTBin,            ECBool, "Function", "bA73UserOverrideFTRTBin",         bNoShow, bDisable, bReadFromFile, 0);
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)   //EastSun 20260508 : A41 KYEC user manage FT/RT Bin Share (skip LotStart override)
    {
        elConfig->Add(cbA74,    &IniConfig.bA74AOIFailCountLinkLotRunMode,            ECBool, "Function", "bA74AOIFailCountLinkLotRunMode",         bShow, bEnable, bReadFromFile, 1);
    }
    else
    {
        elConfig->Add(cbA74,    &IniConfig.bA74AOIFailCountLinkLotRunMode,            ECBool, "Function", "bA74AOIFailCountLinkLotRunMode",         bNoShow, bDisable, bReadFromFile, 0);
    }

    elConfig->Add(cbA75, &IniConfig.bA75DownloadItemByAccessLevel, ECBool, "Function", "bA75DownloadItemByAccessLevel", bShow, bEnable, bReadFromFile, 0);   //AI(safety-critical-change) 20260611 (RogerYang) : 下載複寫項目依權限控管(OP禁用)

    if(IniConfig.bSPILFunction==true)
    {
        elConfig->Add(cbA81WaitSECS, &IniConfig.bA81WaitSECS, ECBool, "SECS GEM", "Wait SECS",                         bShow, bEnable, bReadFromFile, 0);       //KevinCheng 20250922 Wait SECS
    }
    else
    {
        elConfig->Add(cbA81WaitSECS, &IniConfig.bA81WaitSECS, ECBool, "SECS GEM", "Wait SECS",                         bNoShow, bDisable, bFixedValue, 0);
    }
}

// AI(W906-FW-CFG-W1) 20260825: golden cConfiguration.cpp:979-1035, transcribed
// VERBATIM (cp950 -> UTF-8). This family is a pure registration table: every
// line is `elConfig->Add(widget, &IniConfig.field, type, section, key, ...)`,
// and the port's HTEditList::Add signature (Public/HTEditList.h:187-199) is
// argument-for-argument identical to golden's, so no adaptation is needed and
// none was invented. Deviations, if any, are marked inline.
void TfConfiguration::InitConfigEdtList_ItemB()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;

//    tsb00->TabVisible=(CUSTOMER_CODE==CC_Greatek  ||
//                       CUSTOMER_CODE==CC_PTI);                                //Sam 20240804 : 新增 PTI
    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
    {
        grpB01->Visible=true;
        grpB02->Visible=true;
        elConfig->Add(cbB01,    &IniConfig.bB01_UsePrecautionRecordFunction,            ECBool,     "PrecautionRecord",     "bB01_UsePrecautionRecordFunction",             bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbB02,    &IniConfig.bB02_HanderMajorMaintenanceRecordFunction,   ECBool,     "PrecautionRecord",     "bB02_HanderMajorMaintenanceRecordFunction",    bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edB01,    &IniConfig.iB01_AutoWakeupPrecautionRecordFormTime,     ECInteger,  "PrecautionRecord",     "iB01_AutoWakeupPrecautionRecordFormTime",      bShow, bEnable, bReadFromFile, 10,   false,  0.01,   600.0);
        elConfig->Add(edB01_1,  &IniConfig.asB01_PrecautionRecordSavePath,              ECText,     "PrecautionRecord",     "asB01_PrecautionRecordSavePath",               bShow, bEnable, bReadFromFile, "");
        elConfig->Add(edB02,    &IniConfig.asB02_HanderMajorMaintenanceRecordSavePath,  ECText,     "PrecautionRecord",     "asB02_HanderMajorMaintenanceRecordSavePath",   bShow, bEnable, bReadFromFile, "");
    }
    else
    {
        grpB01->Visible=false;
        grpB02->Visible=false;
        elConfig->Add(cbB01,    &IniConfig.bB01_UsePrecautionRecordFunction,            ECBool,     "PrecautionRecord",     "bB01_UsePrecautionRecordFunction",             bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbB02,    &IniConfig.bB02_HanderMajorMaintenanceRecordFunction,   ECBool,     "PrecautionRecord",     "bB02_HanderMajorMaintenanceRecordFunction",    bNoShow, bDisable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_PTI)                                                   //Sam 20231115 : PTI 新增 Tester report
    {
        grpB03->Visible=true;
        elConfig         ->Add(cbB03,       &IniConfig.bB03_TesterReport,   ECBool,     "Report",   "bB03_TesterReport",    bShow, bEnable, bReadFromFile, 0);
        elConfig_byRecipe->Add(edB03_1,     &IniConfig.sB03_Customer,       ECText,     "Report",   "sB03_Customer",        bShow, bEnable, bReadFromFile, "Customer");     //Sam 20231115 : PTI 新增 Tester report
        elConfig_byRecipe->Add(edB03_2,     &IniConfig.sB03_DeviceID,       ECText,     "Report",   "sB03_DeviceID",        bShow, bEnable, bReadFromFile, "DeviceID");     //Sam 20231115 : PTI 新增 Tester report
    }
    else
    {
        grpB03->Visible=false;
        elConfig->Add(cbB03,    &IniConfig.bB03_TesterReport,               ECBool,     "Report",   "bB03_TesterReport",    bNoShow, bDisable, bFixedValue, 0);
    }

    elConfig->Add(cbB05,        &IniConfig.bB05_OSReport,                   ECBool,     "Report",   "bB05_OSReport",        bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edtB05,       &IniConfig.sB05_OSReportPath,               ECText,     "Report",   "sB05_OSReportPath",    bShow, bEnable, bReadFromFile, "");             //Steven 20250513 : OS Report
    if(CUSTOMER_CODE==CC_PANTHER)
    {
        elConfig->Add(cbB11Enable,          &IniConfig.bB11UsePATServerFile,  ECBool,  "PrecautionRecord",     "bB11UsePATServerFile",  bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edB11PATServerPath,   &IniConfig.sB11PATServerPath,     ECText,  "PrecautionRecord",     "sB11PATServerPath",     bShow, bEnable, bReadFromFile, "D:\\HT9045\\system\\PATFolder");
        elConfig->Add(cbB12Enable,          &IniConfig.bB12UsePATSetup,       ECBool,  "PrecautionRecord",     "bB12UsePATSetup",       bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edB12Path,            &IniConfig.sB12PATSetupPath,      ECText,  "PrecautionRecord",     "sB12PATSetupPath",      bShow, bEnable, bReadFromFile, "D:\\HT9045\\system\\PATFolder");
        elConfig->Add(edB13UploadPath,      &IniConfig.sB13PATJobUploadPath,  ECText,  "PrecautionRecord",     "sB13PATJobUploadPath",  bShow, bEnable, bReadFromFile, "D:\\HT9045\\system\\PATFolder");
        elConfig->Add(edB13DownloadPath,    &IniConfig.sB13PATJobDownloadPath,ECText,  "PrecautionRecord",     "sB13PATJobDownloadPath",bShow, bEnable, bReadFromFile, "D:\\HT9045\\system\\PATFolder");
        elConfig->Add(edB14IntervalTime,    &IniConfig.iB14IntervalTime,      ECInteger,"PrecautionRecord",     "iB14IntervalTime",      bShow, bEnable, bReadFromFile, 20,   false,  0,   600);
        elConfig->Add(edB14ReportRealTime,  &IniConfig.sB14RealTimePath,      ECText,  "PrecautionRecord",     "sB14RealTimePath",      bShow, bEnable, bReadFromFile, "D:\\HT9045\\system\\PATFolder");
    }
    else
    {
        pal_B11->Visible=false;
    }
}

// AI(W906-FW-CFG-W1) 20260825: golden cConfiguration.cpp:1037-1242, transcribed
// VERBATIM (cp950 -> UTF-8). This family is a pure registration table: every
// line is `elConfig->Add(widget, &IniConfig.field, type, section, key, ...)`,
// and the port's HTEditList::Add signature (Public/HTEditList.h:187-199) is
// argument-for-argument identical to golden's, so no adaptation is needed and
// none was invented. Deviations, if any, are marked inline.
void TfConfiguration::InitConfigEdtList_ItemC()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;

    if(CUSTOMER_CODE==CC_ASE_CL || CUSTOMER_CODE==CC_HONPREC_QC)                //wei 20160215大風扇方向
        elConfig->Add(cbC01,    &IniConfig.bC01_FanDirection,                   ECBool, "Function", "bFanDirection",                    bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbC01,    &IniConfig.bC01_FanDirection,                   ECBool, "Function", "bFanDirection",                    bNoShow, bEnable, bFixedValue, 0);

    if(IniConfig.bEnableCCDUSETCPIP==true)
        cbLastSet->Add(cbC02,   &IniConfig.bC02InstallCCD,                      ECBool, "Index",    "bC02InstallCCD",                   bShow, bEnable, bReadFromFile, LastSet.bC02InstallCCD?"1":"0");
    else
        cbLastSet->Add(cbC02,   &IniConfig.bC02InstallCCD,                      ECBool, "Index",    "bC02InstallCCD",                   bNoShow, bDisable, bFixedValue, 0);

    if(USE_AUTO_RETEST==eartInstall)
    {
        if((SubMachineType==Type_HT9046AU))                                     //RogerYang 20250508 9046AU No Catch Cylinder
        {
            cbLastSet->Add(cbC03,   &IniConfig.bC03UseCatchTray,                    ECBool, "Tray",     "bC03UseCatchTray",                 bShow, bDisable, bFixedValue, 0);
        }
        else
        {
            cbLastSet->Add(cbC03,   &IniConfig.bC03UseCatchTray,                    ECBool, "Tray",     "bC03UseCatchTray",                 bShow, bDisable, bFixedValue, 1);
        }
    }
    else
        cbLastSet->Add(cbC03,   &IniConfig.bC03UseCatchTray,                    ECBool, "Tray",     "bC03UseCatchTray",                 bShow, bEnable, bReadFromFile, LastSet.bC03UseCatchTray?"1":"0");

    if(CUSTOMER_CODE==CC_HONPREC_QC)
        elConfig->Add(cbC04,    &IniConfig.bC04EnableTestTempIC,                ECBool, "Index",    "bC04EnableTestTempIC",             bShow, bEnable, bReadFromFile, LastSet.bEnableTestTempIC?"1":"0");
    else
        elConfig->Add(cbC04,    &IniConfig.bC04EnableTestTempIC,                ECBool, "Index",    "bC04EnableTestTempIC",             bNoShow, bEnable, bFixedValue, 0);

    if(IniConfig.bPowerSaveFunction)
    {
        gbC05->Visible=true;
        elConfig->Add(rgC05,        &IniConfig.iPowersaveMode,                  ECInteger,  "OutPowerSave", "PowersaveMode",            bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC05_Motor,  &IniConfig.bC05_PowerSaveMotor,             ECBool,     "OutPowerSave", "UsepowerSaveMotor",        bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC05_Temp,   &IniConfig.bC05_PowerSaveTemp,              ECBool,     "OutPowerSave", "UsepowerSaveTemp",         bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC05_Vacuum, &IniConfig.bC05_PowerSaveVacuum,            ECBool,     "OutPowerSave", "UsepowerSaveVacuum",       bShow, bEnable, bReadFromFile, 0);      //Steven 20221215 : Power saving for vacuum pump
        if(ATC_SYSTEM!=eNewATCSystem)
        {
            elConfig->Add(cbC05_ATC,&IniConfig.bC05_PowerSaveATC,               ECBool,     "OutPowerSave", "UsepowerSaveATC",          bNoShow, bDisable, bFixedValue, 0);      //Ifor 20240401 : Power saving for ATC System
        }
        else
        {
            elConfig->Add(cbC05_ATC,&IniConfig.bC05_PowerSaveATC,               ECBool,     "OutPowerSave", "UsepowerSaveATC",          bShow, bEnable, bReadFromFile, 0);      //Ifor 20240401 : Power saving for ATC System
        }
        elConfig->Add(edC05_Motor,  &IniConfig.iHaltTime_Motor,                 ECInteger,  "OutPowerSave", "HaltTime_Motor",           bShow, bEnable, bReadFromFile, 1,      false,  1,      200);            //kevin 20110328設定省電模式時間
        elConfig->Add(edC05_Temp,   &IniConfig.iHaltTime_Temp,                  ECInteger,  "OutPowerSave", "HaltTime_Temp",            bShow, bEnable, bReadFromFile, 1,      false,  1,      200);
        elConfig->Add(edC05_Vacuum, &IniConfig.iC05HaltTime_Vacuum,             ECInteger,  "OutPowerSave", "HaltTime_Vacuum",          bShow, bEnable, bReadFromFile, 1,      false,  1,      200);            //Steven 20221215 : Power saving for vacuum pump
        elConfig->Add(edC05_ATC,    &IniConfig.iHaltTime_ATC,                   ECInteger,  "OutPowerSave", "HaltTime_ATC",             bShow, bEnable, bReadFromFile, 1,      false,  1,      200);            //Ifor 20240401 : Power saving for ATC System
    }
    else
    {
        gbC05->Visible=false;
        elConfig->Add(rgC05,        &IniConfig.iPowersaveMode,                  ECInteger,  "OutPowerSave", "PowersaveMode",            bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbC05_Motor,  &IniConfig.bC05_PowerSaveMotor,             ECBool,     "OutPowerSave", "UsepowerSaveMotor",        bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbC05_Temp,   &IniConfig.bC05_PowerSaveTemp,              ECBool,     "OutPowerSave", "UsepowerSaveTemp",         bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbC05_Vacuum, &IniConfig.bC05_PowerSaveVacuum,            ECBool,     "OutPowerSave", "UsepowerSaveVacuum",       bNoShow, bDisable, bFixedValue, 0);         //Steven 20221215 : Power saving for vacuum pump
        elConfig->Add(cbC05_ATC,    &IniConfig.bC05_PowerSaveATC,               ECBool,     "OutPowerSave", "UsepowerSaveATC",          bNoShow, bDisable, bFixedValue, 0);         //Ifor 20240401 : Power saving for ATC System
        elConfig->Add(edC05_Motor,  &IniConfig.iHaltTime_Motor,                 ECInteger,  "OutPowerSave", "HaltTime_Motor",           bNoShow, bDisable, bFixedValue, 1);         //kevin 20110328設定省電模式時間
        elConfig->Add(edC05_Temp,   &IniConfig.iHaltTime_Temp,                  ECInteger,  "OutPowerSave", "HaltTime_Temp",            bNoShow, bDisable, bFixedValue, 1);
        elConfig->Add(edC05_Vacuum, &IniConfig.iC05HaltTime_Vacuum,             ECInteger,  "OutPowerSave", "HaltTime_Vacuum",          bNoShow, bDisable, bFixedValue, 1);         //Steven 20221215 : Power saving for vacuum pump
        elConfig->Add(edC05_ATC,    &IniConfig.iHaltTime_ATC,                   ECInteger,  "OutPowerSave", "HaltTime_ATC",             bNoShow, bDisable, bFixedValue, 1);         //Ifor 20240401 : Power saving for ATC System
    }

    if(IniConfig.bCanByPassIonFan)                                              //Steven 20111013
    {
        gbC06->Visible=true;
        elConfig->Add(cbC06_01, &IniConfig.bC06_ByPassIonFan[0],                ECBool,     "bCanByPassIonFan", "Fan01",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_02, &IniConfig.bC06_ByPassIonFan[1],                ECBool,     "bCanByPassIonFan", "Fan02",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_03, &IniConfig.bC06_ByPassIonFan[2],                ECBool,     "bCanByPassIonFan", "Fan03",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_04, &IniConfig.bC06_ByPassIonFan[3],                ECBool,     "bCanByPassIonFan", "Fan04",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_05, &IniConfig.bC06_ByPassIonFan[4],                ECBool,     "bCanByPassIonFan", "Fan05",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_06, &IniConfig.bC06_ByPassIonFan[5],                ECBool,     "bCanByPassIonFan", "Fan06",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_07, &IniConfig.bC06_ByPassIonFan[6],                ECBool,     "bCanByPassIonFan", "Fan07",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_08, &IniConfig.bC06_ByPassIonFan[7],                ECBool,     "bCanByPassIonFan", "Fan08",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_09, &IniConfig.bC06_ByPassIonFan[8],                ECBool,     "bCanByPassIonFan", "Fan09",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_10, &IniConfig.bC06_ByPassIonFan[9],                ECBool,     "bCanByPassIonFan", "Fan10",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_11, &IniConfig.bC06_ByPassIonFan[10],               ECBool,     "bCanByPassIonFan", "Fan11",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbC06_12, &IniConfig.bC06_ByPassIonFan[11],               ECBool,     "bCanByPassIonFan", "Fan12",                bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        gbC06->Visible=false;
        elConfig->Add(cbC06_01, &IniConfig.bC06_ByPassIonFan[0],                ECBool,     "bCanByPassIonFan", "Fan01",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_02, &IniConfig.bC06_ByPassIonFan[1],                ECBool,     "bCanByPassIonFan", "Fan02",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_03, &IniConfig.bC06_ByPassIonFan[2],                ECBool,     "bCanByPassIonFan", "Fan03",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_04, &IniConfig.bC06_ByPassIonFan[3],                ECBool,     "bCanByPassIonFan", "Fan04",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_05, &IniConfig.bC06_ByPassIonFan[4],                ECBool,     "bCanByPassIonFan", "Fan05",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_06, &IniConfig.bC06_ByPassIonFan[5],                ECBool,     "bCanByPassIonFan", "Fan06",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_07, &IniConfig.bC06_ByPassIonFan[6],                ECBool,     "bCanByPassIonFan", "Fan07",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_08, &IniConfig.bC06_ByPassIonFan[7],                ECBool,     "bCanByPassIonFan", "Fan08",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_09, &IniConfig.bC06_ByPassIonFan[8],                ECBool,     "bCanByPassIonFan", "Fan09",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_10, &IniConfig.bC06_ByPassIonFan[9],                ECBool,     "bCanByPassIonFan", "Fan10",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_11, &IniConfig.bC06_ByPassIonFan[10],               ECBool,     "bCanByPassIonFan", "Fan11",                bNoShow, bEnable, bFixedValue, 0);
        elConfig->Add(cbC06_12, &IniConfig.bC06_ByPassIonFan[11],               ECBool,     "bCanByPassIonFan", "Fan12",                bNoShow, bEnable, bFixedValue, 0);
    }

    cbC07->Visible=(INSTALL_OCR);
    #ifdef MTK_Version
    elConfig->Add(cbC08,        &IniConfig.bC08_SocketSensor,                   ECBool,     "Index", "bUseSocketSensor",                bShow, bDisable, bFixedValue, 1);
    elConfig->Add(cbC08_1,      &IniConfig.bC08_1_CheckSocketSensorDetectON,    ECBool,     "Index", "bCheckSocketSensorDetectON",      bShow, bDisable, bFixedValue, 1);       //Isaac 20201130 : Socket Sensor按Start後要偵測是否啟動功能
    #else
    elConfig->Add(cbC08,        &IniConfig.bC08_SocketSensor,                   ECBool,     "Index", "bUseSocketSensor",                bShow, bEnable, bReadFromFile, 0);      //kevin 20130504 使用socket sensor detect 功能 piggy back
    elConfig->Add(cbC08_1,      &IniConfig.bC08_1_CheckSocketSensorDetectON,    ECBool,     "Index", "bCheckSocketSensorDetectON",      bShow, bEnable, bReadFromFile, 0);      //Isaac 20201130 : Socket Sensor按Start後要偵測是否啟動功能
    #endif
    elConfig->Add(cbC09,        &IniConfig.bC09_CarRecord,                      ECBool,     "Car Record", "bCarRecord",                 bNoShow, bEnable, bFixedValue, 0);      //wei 2013-12-09
    elConfig->Add(edC09,        &IniConfig.iCarRecordDelayTime,                 ECInteger,  "Car Record", "iCarRecordDelayTime",        bShow, bEnable, bReadFromFile,  200,   false,  1,      200);            //wei 2013-12-09

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20170125 add   //Ifor 20150724 :Enable ESD COM ERR Report Function
    {
        if(ESD_Monitor==false && USE_NOVX3360==false)
            elConfig->Add(cbC10,&IniConfig.bEnable_ESD_COMERR_Report,           ECBool,     "Index", "bEnable_ESD_COMERR_Report",       bNoShow, bEnable, bFixedValue, 0);
        else
            elConfig->Add(cbC10,&IniConfig.bEnable_ESD_COMERR_Report,           ECBool,     "Index", "bEnable_ESD_COMERR_Report",       bShow, bDisable, bFixedValue, 1);
    }
    else
    {
        elConfig->Add(cbC10,    &IniConfig.bEnable_ESD_COMERR_Report,           ECBool,     "Index", "bEnable_ESD_COMERR_Report",       bShow, bEnable, bReadFromFile, 0);
    }

    #ifdef SOFT_SIMULTE
    IniConfig.bC11UseMonitorView=false;
    #else
    if(CosFunction.bUseMonitorView)                                             //JerryYang 20160621 錄影監視功能
        elConfig->Add(cbC11,    &IniConfig.bC11UseMonitorView,                  ECBool,     "Function", "bC11UseMonitorView",           bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbC11,    &IniConfig.bC11UseMonitorView,                  ECBool,     "Function", "bC11UseMonitorView",           bNoShow, bEnable, bFixedValue, 0);
    #endif

    if(CosFunction.bUsePEModelFunction)                                         //Ifor 20170125 (Steven) add PE Mode Switch
        elConfig->Add(cbC12,    &IniConfig.bC12UsePEMode,                       ECBool,     "Function", "bC12UsePEMode",                bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbC12,    &IniConfig.bC12UsePEMode,                       ECBool,     "Function", "bC12UsePEMode",                bNoShow, bEnable, bFixedValue, 0);

    if(CosFunction.bNeedToRestartGroundWhenInitialStart)                        //Sam 20220107 : 矽格北興 Initail Start 要重啟 GroundMan
        elConfig->Add(cbC13,    &IniConfig.bC13NeedToRestartGroundWhenInitialStart, ECBool, "Function", "bC13NeedToRestartGroundWhenInitialStart", bShow, bEnable, bReadFromFile, 1);
    else
        elConfig->Add(cbC13,    &IniConfig.bC13NeedToRestartGroundWhenInitialStart, ECBool, "Function", "bC13NeedToRestartGroundWhenInitialStart", bNoShow, bEnable, bFixedValue, 0);

    elConfig->Add(chkC14,       &IniConfig.bC14SaveBinDisplayLog,               ECBool,     "Function", "bC14SaveBinDisplayLog",        bShow, bEnable, bReadFromFile, 0);  //Steven 20220309 : BinDisplay Log

    if(CosFunction.bUseBarCoderChangeSetupFile)                                 //Sam 20230320 : 使用 BarCodeReader 來輸入切換 SetupFile。
        elConfig->Add(cbC16,    &IniConfig.bC16UseBarCoderChangeSetupFile,      ECBool,     "Function", "bC16UseBarCoderChangeSetupFile", bShow, bEnable, bReadFromFile, 1);
    else
        elConfig->Add(cbC16,    &IniConfig.bC16UseBarCoderChangeSetupFile,      ECBool,     "Function", "bC16UseBarCoderChangeSetupFile", bNoShow, bEnable, bFixedValue, 0);

    if(USE_COLORSENSOR_MUN==eCSMUN_Install_Loader)                              //Jimmychiu 20230630 : add color sensor MU-N in Loader
    {
        elConfig->Add(cbC17,    &IniConfig.bC17UseLoaderColorSensor,            ECBool,     "Function", "bC20UseLoaderColorSensor", bShow,   bEnable, bReadFromFile, 1);
        elConfig->Add(edtC17,   &IniConfig.iC17DelayTimes,                      ECInteger,  "Function", "iC20DelayTimes",           bShow,   bEnable, bReadFromFile, 3,      false,  1,      10);
        elConfig->Add(cbC17_1,  &IniConfig.bC17_1_SkipAlarm,                    ECBool,     "Function", "bC20_1_SkipAlarm",         bNoShow, bEnable, bReadFromFile, 0);
        grpC17->Visible=true;
    }
    else
    {
        elConfig->Add(cbC17,    &IniConfig.bC17UseLoaderColorSensor,            ECBool,     "Function", "bC20UseLoaderColorSensor", bNoShow, bEnable, bFixedValue, 0);
        grpC17->Visible=false;
    }

    if(USE_VibrationCommunication)                                              //JerryYang 20230814 : add震動馬達通訊調速版本
    {
        grpC21->Visible=true;
        elConfig->Add(edtC21_HP1,       &IniConfig.iC21MotSp_HP1[0],            ECInteger,  "C16Vibrate",       "C16VibrateMotSp_HP1",         bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_HP2,       &IniConfig.iC21MotSp_HP2[0],            ECInteger,  "C16Vibrate",       "C16VibrateMotSp_HP2",         bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_SHT1,      &IniConfig.iC21MotSp_SHT1[0],           ECInteger,  "C16Vibrate",       "C16VibrateMotSp_SHT1",        bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_SHT2,      &IniConfig.iC21MotSp_SHT2[0],           ECInteger,  "C16Vibrate",       "C16VibrateMotSp_SHT2",        bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto1,     &IniConfig.iC21MotSp_AUTO1[0],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO1",       bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto2,     &IniConfig.iC21MotSp_AUTO2[0],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO2",       bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto3,     &IniConfig.iC21MotSp_AUTO3[0],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO3",       bShow, bEnable, bReadFromFile, 50,      false,  1,      60);

        elConfig->Add(edtC21_HP1_m,     &IniConfig.iC21MotSp_HP1[1],            ECInteger,  "C16Vibrate",       "C16VibrateMotSp_HP1_M",       bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_HP2_m,     &IniConfig.iC21MotSp_HP2[1],            ECInteger,  "C16Vibrate",       "C16VibrateMotSp_HP2_M",       bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_SHT1_m,    &IniConfig.iC21MotSp_SHT1[1],           ECInteger,  "C16Vibrate",       "C16VibrateMotSp_SHT1_M",      bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_SHT2_m,    &IniConfig.iC21MotSp_SHT2[1],           ECInteger,  "C16Vibrate",       "C16VibrateMotSp_SHT2_M",      bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto1_m,   &IniConfig.iC21MotSp_AUTO1[1],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO1_M",     bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto2_m,   &IniConfig.iC21MotSp_AUTO2[1],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO2_M",     bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto3_m,   &IniConfig.iC21MotSp_AUTO3[1],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO3_M",     bShow, bEnable, bReadFromFile, 50,      false,  1,      60);

        elConfig->Add(edtC21_HP1_b,     &IniConfig.iC21MotSp_HP1[2],            ECInteger,  "C16Vibrate",       "C16VibrateMotSp_HP1_B",       bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_HP2_b,     &IniConfig.iC21MotSp_HP2[2],            ECInteger,  "C16Vibrate",       "C16VibrateMotSp_HP2_B",       bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_SHT1_b,    &IniConfig.iC21MotSp_SHT1[2],           ECInteger,  "C16Vibrate",       "C16VibrateMotSp_SHT1_B",      bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_SHT2_b,    &IniConfig.iC21MotSp_SHT2[2],           ECInteger,  "C16Vibrate",       "C16VibrateMotSp_SHT2_B",      bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto1_b,   &IniConfig.iC21MotSp_AUTO1[2],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO1_B",     bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto2_b,   &IniConfig.iC21MotSp_AUTO2[2],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO2_B",     bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
        elConfig->Add(edtC21_Auto3_b,   &IniConfig.iC21MotSp_AUTO3[2],          ECInteger,  "C16Vibrate",       "C16VibrateMotSp_AUTO3_B",     bShow, bEnable, bReadFromFile, 50,      false,  1,      60);
    }
    else
    {
        grpC21->Visible=false;
    }

    if(CUSTOMER_CODE==CC_ASE_CL)            //JerryYang 20250120 : add
    {
        elConfig->Add(cbC24,    &IniConfig.bC24InitialStartCheckCylinder,           ECBool,     "Function", "bC24InitialStartCheckCylinder",       bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbC24,    &IniConfig.bC24InitialStartCheckCylinder,           ECBool,     "Function", "bC24InitialStartCheckCylinder",       bNoShow, bEnable, bFixedValue, 0);
    }
}

// =============================================================================
// FW-CFG-W2 -- InitConfigEdtList_ItemD / InitConfigEdtList_ItemE / InitConfigEdtList_ItemF / InitConfigEdtList_ItemG / InitConfigEdtList_ItemL / InitConfigEdtList_ItemM / InitConfigEdtList_ItemO (merged 20260825)
// =============================================================================
// AI(W906-FW-CFG-W2) 20260825: golden cConfiguration.cpp:1244-1759, transcribed
// VERBATIM (cp950 -> UTF-8). Registration table only -- the port's
// HTEditList::Add signature is argument-for-argument golden's, so no
// adaptation was needed and none was invented. See FW-CFG-W1's banner in
// forms/fConfiguration.h for why this family is the wave's point.
void TfConfiguration::InitConfigEdtList_ItemD()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;

    if(USE_IO_CHANGE_TOQUE==true)                                               //jou 2013-07-12 兩個funtion不能同時開
        elConfig->Add(cbD01,        &IniConfig.bD01EnableReadTorque,            ECBool,     "Index", "bD01EnableReadTorque",            bShow, bDisable, bFixedValue, 0);
    else if(CUSTOMER_CODE==CC_KYEC_XILINX &&
            IniConfig.bChangeKitNoHardStop==true &&
            IniConfig.bRemeberAutoHeight==true)                                 //Frank 20170626 (Steven) add Xilinx 浮動Shuttle Kit 強制開啟[D01]
        elConfig->Add(cbD01,        &IniConfig.bD01EnableReadTorque,            ECBool,     "Index", "bD01EnableReadTorque",            bShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbD01,        &IniConfig.bD01EnableReadTorque,            ECBool,     "Index", "bD01EnableReadTorque",            bShow, bEnable, bReadFromFile, LastSet.bEnableReadTorque?"1":"0");

    #ifndef ASE_KaohSiung                                                       //KaiHuang 20201221 Add
        elConfig->Add(edD01,        &IniConfig.iD01ReadTorqueTimeCount,         ECInteger,  "Index", "iD01ReadTorqueTimeCount",         bShow, bEnable, bReadFromFile,  200,   false,  0,      1000);           //0~1000次
        elConfig->Add(cbD01_1,      &IniConfig.bD01_1EnableReadAndCheckTorque,  ECBool,     "Index", "bD01_1EnableReadAndCheckTorque",  bShow  , bEnable , bReadFromFile, 0);
        elConfig->Add(edD01_Xilinx, &IniConfig.dD01ReadTorque,                  ECDouble,   "Index", "dD01ReadTorque",                  bNoShow, bDisable, bFixedValue, 10.0,  false,  10.0,   100.0);          //Frank 20170626 (Steven) add Xilinx 浮動Shuttle Kit 強制開啟[D01]
        elConfig->Add(edD01DelayTime_Xilinx,&IniConfig.dD01ReadTorqueDelayTime, ECDouble,   "Index", "dD01ReadTorqueDelayTime",         bNoShow, bDisable, bFixedValue, 1.0,   false,  0.01,   5.0);            //Frank 20170626 (Steven) add Xilinx 浮動Shuttle Kit 強制開啟[D01]
    #else
        //借來用
        elConfig->Add(edD01,        &IniConfig.iD01ReadTorqueTimeCount,         ECInteger,  "Index", "iD01ReadTorqueTimeCount",         bNoShow, bDisable, bReadFromFile,  200,   false,  0,      1000);
        elConfig->Add(cbD01_1,      &IniConfig.bD01_1EnableReadAndCheckTorque,  ECBool,     "Index", "bD01_1EnableReadAndCheckTorque",  bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edD01_Xilinx, &IniConfig.dD01ReadTorque,                  ECDouble,   "Index", "dD01ReadTorque",                  bShow,   bEnable,  bReadFromFile,  3.0,   false,  0.1,    100.0);
        elConfig->Add(edD01DelayTime_Xilinx,&IniConfig.dD01ReadTorqueDelayTime, ECDouble,   "Index", "dD01ReadTorqueDelayTime",         bShow,   bEnable,  bReadFromFile,  0.1,   false,  0.1,   10.0);
    #endif
    elConfig->Add(cbD02,        &IniConfig.bD02OffReadTorqueDuringTest,         ECBool,     "Index", "bD02OffReadTorqueDuringTest",     bShow, bEnable, bReadFromFile,  LastSet.bOffReadTorqueDuringTest?"1":"0");
    elConfig->Add(cbD04,        &IniConfig.bD04MinForceByFile,                  ECBool,     "Contact Force", "bD04MinForceByFile",      bShow, bEnable, bReadFromFile,  0);                                     //Steven 20190314 : Min force is read from file
    elConfig->Add(edtD04,       &IniConfig.dD04MinForceByFile,                  ECDouble,   "Contact Force", "dD04MinForceByFile",      bShow, bEnable, bReadFromFile,  1.0,   false,  1.0,    120.0);          //Steven 20190314 : Min force is read from file
    elConfig->Add(edtD04_20mm,  &IniConfig.dD04MinForceByFile_20mm,             ECDouble,   "Contact Force", "dD04MinForceByFile_20mm", bShow, bEnable, bReadFromFile,  0.5,   false,  1.0,    120.0);          //Steven 20220822 : Min force for different SLK
    elConfig->Add(edtD04_30mm,  &IniConfig.dD04MinForceByFile_30mm,             ECDouble,   "Contact Force", "dD04MinForceByFile_30mm", bShow, bEnable, bReadFromFile,  1.5,   false,  1.0,    120.0);
    elConfig->Add(edtD04_40mm,  &IniConfig.dD04MinForceByFile_40mm,             ECDouble,   "Contact Force", "dD04MinForceByFile_40mm", bShow, bEnable, bReadFromFile,  4.0,   false,  1.0,    120.0);
    elConfig->Add(edtD04_60mm,  &IniConfig.dD04MinForceByFile_60mm,             ECDouble,   "Contact Force", "dD04MinForceByFile_60mm", bShow, bEnable, bReadFromFile,  8.0,   false,  1.0,    120.0);
    elConfig->Add(edtD04_80mm,  &IniConfig.dD04MinForceByFile_80mm,             ECDouble,   "Contact Force", "dD04MinForceByFile_80mm", bShow, bEnable, bReadFromFile,  15.0,  false,  1.0,    180.0);          //Ifor 20240620 : add缸徑 80
    elConfig->Add(cbD05,        &IniConfig.bD05ContactCountAlarm,               ECBool,     "Index", "bD05ContactCountAlarm",           bNoShow, bEnable, bFixedValue,  0);                                     //wei 20170327
    elConfig->Add(edD05,        &IniConfig.iD05_ContactCountAlarm,              ECInteger,  "Contact Force", "iD05_ContactCountAlarm",  bNoShow, bDisable, bFixedValue, 1,     false,  0,      5000);           //wei 20170327 add
    elConfig->Add(cbD05_1,      &IniConfig.bD05_1SaveSocketCntByHandler,        ECBool,     "Index", "bD05_1SaveSocketCntByHandler",    bShow, bEnable, bReadFromFile,  0);                                     //Steven 20250807 : By handler save contact count
    elConfig->Add(cbD06,        &IniConfig.bD06ContactOffsetDefaultValue,       ECBool,     "Index", "bD06ContactOffsetDefaultValue",   bShow, bEnable, bReadFromFile,  0);                                     //JimmyChiu 20220120 add ContactOffsetDefaultValue
    elConfig->Add(edD06,        &IniConfig.dD06_ContactOffsetDefaultValue,      ECDouble,   "Contact Force", "dD06_ContactOffsetDefaultValue",   bShow, bEnable, bReadFromFile,  3,     false,  0,      10);    //JimmyChiu 20220120 add ContactOffsetDefaultValue
    if(CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_ASE_KaohSiung_K12)
        elConfig->Add(cbD10,    &IniConfig.bD10ManualHeightComptibleWithNS,     ECBool,     "Index", "bD10ManualHeightComptibleWithNS", bShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbD10,    &IniConfig.bD10ManualHeightComptibleWithNS,     ECBool,     "Index", "bD10ManualHeightComptibleWithNS", bShow, bEnable, bReadFromFile, LastSet.bManualHeightComptibleWithNS?"1":"0");

    elConfig->Add(cbD11,        &IniConfig.bD11NoIcSkipAutoHeight,              ECBool,     "Index", "bNoIcSkipAutoHeight",             bShow, bEnable, bReadFromFile, 0);      //Steven 20110726
//    if(IniConfig.bSPILFunction==true)                                         //JerryYang 20241011 : Mark掉 //Steven 20140220 : 用生產ep去做蝦頭auto high
//        elConfig->Add(cbD12,    &IniConfig.bD12UseDeviceFormPressDoShtHeight,   ECBool,     "Index", "bUseDeviceFormPressDoShtHeight",  bShow, bDisable, bFixedValue, 1);
//    else
        elConfig->Add(cbD12,    &IniConfig.bD12UseDeviceFormPressDoShtHeight,   ECBool,     "Index", "bUseDeviceFormPressDoShtHeight",  bShow, bEnable, bReadFromFile, 0);

    elConfig->Add(cdD13,        &IniConfig.bD13CheckIndexHomeSensor,            ECBool,     "Index", "bCheckIndexHomeSensor",           bShow, bEnable, bReadFromFile, 0);      //Steven 20140828 : 歸零後檢查Index位置
    elConfig->Add(cbD14,        &IniConfig.bD14_AutoHeightUseSetTorque,         ECBool,     "Index", "bD14_AutoHeightUseSetTorque",     bShow, bEnable, bReadFromFile, 0);      //Steven 20141105 : 使用指定的扭力進行Auto Height
    elConfig->Add(edD14,        &IniConfig.iD14_AutoHeightUseSetTorque,         ECInteger,  "Index", "iD14_AutoHeightUseSetTorque",     bShow, bEnable, bReadFromFile,  15,    false,  5,      30);             //kevin 20210730 10->5
    elConfig->Add(cbD15,        &IniConfig.bD15_AutoContactTest,                ECBool,     "Index", "bD15_AutoContactTest",            bShow, bEnable, bReadFromFile, 0);      //Steven 20150224 : Auto Contact Test
    elConfig->Add(cbD16,        &IniConfig.bD16_StepContactTest,                ECBool,     "Index", "bD16_StepContactTest",            bShow, bEnable, bReadFromFile, 0);      //Steven 20150811 : Step by Step Contact Test
    elConfig->Add(rgD17,        &IniConfig.iD17_UseHardwareHeightToContact,     ECInteger,  "Index", "bD17_UseHardwareHeightToContact", bShow, bEnable, bReadFromFile, 0);      //Steven 20170411 (wei) : SCK的SIP怕刮傷所以Contact Height使用硬體高度
    elConfig->Add(edtD17_3,     &IniConfig.dD17_3CheckEPLeakage,                ECDouble,   "Index", "dD17_3CheckEPLeakage",            bShow, bEnable, bReadFromFile, 1.0, false,  1.0,    3.0);    //Steven 20231025 : 充氣跟不充氣都做一次auto height, 然後檢查有沒有漏氣
    elConfig->Add(chkD18,       &IniConfig.bD18_AutoHeightWhenChangeRecipe,     ECBool,     "Index", "bD18_AutoHeightWhenChangeRecipe", bShow, bEnable, bReadFromFile, 0);      //Steven 20221013 : ATK希望更換工作檔時, 需要提示有沒有做Auto Height.

    elConfig->Add(cbD21,        &IniConfig.bD21EnableFinishTestUpWait,          ECBool,     "Tester", "bD21EnableFinishTestUpWait",     bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edD21_mm,     &IniConfig.iD21FinishTestUpWaitHeight,          ECInteger,  "Tester", "iD21FinishTestUpWaitHeight",     bShow, bEnable, bReadFromFile, LastSet.iFinishTestUpWaitHeight,       false,  0,      10, EUuMToMM);  //0~10mm
    elConfig->Add(edD21_Sec,    &IniConfig.iD21FinishTestUpWaitTime,            ECInteger,  "Tester", "iD21FinishTestUpWaitTime",       bShow, bEnable, bReadFromFile, (int)LastSet.iD21FinishTestUpWaitTime, false,  0.1, 100.0, EUMSToSec);  //0.1~100Sec

    elConfig->Add(cbD22_1,      &IniConfig.bD22SupportMultiDoubleContact,       ECBool,     "Index", "bD22SupportMultiDoubleContact",   bShow, bEnable, bReadFromFile, LastSet.bSupportMultiDoubleContact?"1":"0");
    elConfig->Add(cbD22_2,      &IniConfig.bD22DoubleContactNoNeedReContact,    ECBool,     "Index", "bDoubleContactNoNeedReContact",   bShow, bEnable, bReadFromFile, 0);      //Steven 20131202 : Double Contact不需要Index Arm上下動
    elConfig->Add(coD22,        &IniConfig.iD22DoubleContactCount,              ECInteger,  "Index", "iD22DoubleContactCount",          bShow, bEnable, bReadFromFile, LastSet.iDoubleContactCount);

    if(CUSTOMER_CODE==CC_AnalogDevice_Phil)                                     //JerryYang 20230721 : Analog double contact鎖定送0X42
    {
        elConfig->Add(cbD22_3_, &IniConfig.bD22DoubleContactUseDiffSRQ,         ECBool,     "Index", "bD22DoubleContactUseDiffSRQ",     bShow, bDisable, bFixedValue, 1);       //Steven 20230508 : 南茂鐘永生說要使用0x41  //JerryYang 20221004 : Double contact改成可以選擇不同的測試訊號
        elConfig->Add(coD22_3,  &IniConfig.iD23_DoubleContactSRQ,               ECInteger,  "Index", "iD23_DoubleContactSRQ",           bShow, bDisable, bFixedValue, 2);
    }
    else
    {
        elConfig->Add(cbD22_3_, &IniConfig.bD22DoubleContactUseDiffSRQ,         ECBool,     "Index", "bD22DoubleContactUseDiffSRQ",     bShow, bEnable, bReadFromFile, 0);       //Steven 20230508 : 南茂鐘永生說要使用0x41  //JerryYang 20221004 : Double contact改成可以選擇不同的測試訊號
        elConfig->Add(coD22_3,  &IniConfig.iD23_DoubleContactSRQ,               ECInteger,  "Index", "iD23_DoubleContactSRQ",           bShow, bEnable, bReadFromFile, 0);
    }

    /*  //Sam 20231117 : 整合到 QA 模式 Mark
    if(CosFunction.bVerifyMode)                                                 //Sam 20221012 : 新增 VerifyMode 功能
        elConfig->Add(cbD22_3,  &IniConfig.bD22VerifyMode,                      ECBool,     "Index", "bD22VerifyMode",                  bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD22_3,  &IniConfig.bD22VerifyMode,                      ECBool,     "Index", "bD22VerifyMode",                  bNoShow, bDisable, bFixedValue, 0);
    */
    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20230909 : pass bin也可以設定Double contact
    {
        elConfig->Add(cbD22_4, &IniConfig.bD22_4_PassBinCanDoubleContact,       ECBool,     "Index", "bD22_4_PassBinCanDoubleContact",  bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbD22_4, &IniConfig.bD22_4_PassBinCanDoubleContact,       ECBool,     "Index", "bD22_4_PassBinCanDoubleContact",  bNoShow, bDisable, bFixedValue, 0);
    }

    elConfig->Add(cbD23,        &IniConfig.bD23EveryDeviceDoubleContactFirstNoTesting, ECBool, "Index", "bEveryDeviceDoubleContactFirstNoTestting", bShow, bEnable, bReadFromFile, 0); //ChungHung 20140709 add for SPIL
    elConfig->Add(edD23,        &IniConfig.iD23_MultiContactCount,              ECInteger,  "Index", "iMultiContactCount",              bShow, bEnable, bReadFromFile,  2,     false,  2,      30);             //Steven 20151001 : Add for TSMC

    if(IniConfig.bIndexEveryTimeCheckEP==true)
        elConfig->Add(cbD24,    &IniConfig.bD24EnableEPCheckFuntion,            ECBool,     "EP Check", "EnableEPCheckFuntion",         bShow, bEnable, bReadFromFile, 0);
    else                                                                        ECBool,
        elConfig->Add(cbD24,    &IniConfig.bD24EnableEPCheckFuntion,            ECBool,     "EP Check", "EnableEPCheckFuntion",         bNoShow, bEnable, bFixedValue, 0);

    if(EP_Install==3 || EP_Install==5)
    {
        elConfig->Add(cbD26,    &IniConfig.bD26EnableEPEncoderRange,            ECBool,     "Index", "bEnableEPEncoderRange",           bShow, bEnable, bReadFromFile, 0);      //ChungHung 20111217
        elConfig->Add(cbD26_1,  &IniConfig.bD26EnableEPLog,                     ECBool,     "Index", "bEnableEPLog",                    bShow, bEnable, bReadFromFile, 0);      //Ifor 20150706 :新增EP LOG 開關
        elConfig->Add(cbD26_2,  &IniConfig.bD26EnableEncodeShow,                ECBool,     "Index", "bEnableEncodeShow",               bShow, bEnable, bReadFromFile, 0);      //Ifor 20160304 :新增EP encoder value Show 開關
        if(CosFunction.bHiSiliconFunction && CUSTOMER_CODE==CC_ASE_KaohSiung)   //kevin 20200313 add eP range 5
            elConfig->Add(edD26,    &IniConfig.iD26EPEncoderRange,              ECInteger,  "Index", "iEPEncoderRange",                 bShow, bDisable, bFixedValue,  5);     //kevin 20200313 add eP range 5
        else
            elConfig->Add(edD26,    &IniConfig.iD26EPEncoderRange,              ECInteger,  "Index", "iEPEncoderRange",                 bShow, bEnable, bReadFromFile,  100,   false,  10,     100);            //ChungHung 20111217
    }
    else
    {
        elConfig->Add(cbD26,    &IniConfig.bD26EnableEPEncoderRange,            ECBool,     "Index", "bEnableEPEncoderRange",           bNoShow, bEnable, bFixedValue, 0);      //ChungHung 20111217
        elConfig->Add(cbD26_1,  &IniConfig.bD26EnableEPLog,                     ECBool,     "Index", "bEnableEPLog",                    bNoShow, bEnable, bFixedValue, 0);      //Ifor 20150706 :新增EP LOG 開關
        elConfig->Add(cbD26_2,  &IniConfig.bD26EnableEncodeShow,                ECBool,     "Index", "bEnableEncodeShow",               bNoShow, bEnable, bFixedValue, 0);      //Ifor 20160304 :新增EP encoder value Show 開關
        elConfig->Add(edD26,    &IniConfig.iD26EPEncoderRange,                  ECInteger,  "Index", "iEPEncoderRange",                 bNoShow, bDisable, bFixedValue, 100);
    }

    if(INSTALL_DOUBLE_EP==DOUBLE_EP_NORMAL || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)                            //AI(ht9045-v899) 20260504: V899 wrote ==true (==1) which dropped Multi EP path; restored V874 behavior.
    {
        elConfig->Add(cbD26_3,  &IniConfig.bD26_3EnableDualEPEncoderRange,      ECBool,     "Index", "bD26_3EnableDualEPEncoderRange",  bShow, bEnable, bReadFromFile, 0);
        if(CUSTOMER_CODE==CC_KYEC_LEE && bEnable_KLT_Function==true)
        {
            cbD26_3->Caption="[D26_3] Enable Dual EP encoder range +,-                   Kg";
        }
        elConfig->Add(edD26_3,  &IniConfig.iD26_3DualEPEncoderRange,            ECInteger,  "Index",  "iD26_3DualEPEncoderRange",       bShow, bEnable, bReadFromFile,  100,   false,  1,      100);
    }
    else
    {
        elConfig->Add(cbD26_3,  &IniConfig.bD26_3EnableDualEPEncoderRange,      ECBool,      "Index", "bD26_3EnableDualEPEncoderRange", bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edD26_3,  &IniConfig.iD26_3DualEPEncoderRange,            ECInteger,   "Index", "iD26_3DualEPEncoderRange",       bNoShow, bDisable, bFixedValue, 100);
    }

    if(CUSTOMER_CODE==CC_JCET)                                                  //Richard 20230426 : JECT ADD 固定值改為百分比
        elConfig->Add(coD26,    &IniConfig.iD26_3FixValueOrPercentage,          ECInteger,  "Index", "iD26_3FixValueOrPercentage",      bShow, bEnable, bReadFromFile, 0); //Richard 20230428 : EP固定值或百分比
    else
        elConfig->Add(coD26,    &IniConfig.iD26_3FixValueOrPercentage,          ECInteger,  "Index", "iD26_3FixValueOrPercentage",      bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_ATMEL || CUSTOMER_CODE==CC_QUALCOMM)                   //Steven 20131203 : bD27UseSingleSite85kg ATK強制開啟
        elConfig->Add(cbD27,    &IniConfig.bD27UseSingleSite85kg,               ECBool,     "Index", "UseSingleSite85kg",               bShow, bDisable, bFixedValue, 1);        //2012-01-03    Dell 在1X2模式下關Site能達85kg
    else
        elConfig->Add(cbD27,    &IniConfig.bD27UseSingleSite85kg,               ECBool,     "Index", "UseSingleSite85kg",               bShow, bEnable, bReadFromFile, 0);
    if(CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20251017 Add:ASE SG 要求強制關閉[D28]
        elConfig->Add(cbD28,        &IniConfig.bD28MaxForceLimitByDiameter,         ECBool,     "Index", "bD28MaxForceLimitByDiameter",     bShow, bDisable, bFixedValue, 0);      //Steven 20200813 : 用缸徑計算最大壓力
    else
        elConfig->Add(cbD28,        &IniConfig.bD28MaxForceLimitByDiameter,         ECBool,     "Index", "bD28MaxForceLimitByDiameter",     bShow, bDisable, bFixedValue, 1);      //Steven 20200813 : 用缸徑計算最大壓力

    if(CosFunction.bHaveIndexContactDelay)                                      //Steven 20140519 : [D29] Index下壓後多Delay 0.4秒 (For SPIL Low Yield)
        elConfig->Add(cbD29,    &IniConfig.bD29EnableIndexContactDelay,         ECBool,      "Index", "bEnableIndexContactDelay",       bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD29,    &IniConfig.bD29EnableIndexContactDelay,         ECBool,      "Index", "bEnableIndexContactDelay",       bNoShow, bEnable, bFixedValue, 0);

    elConfig->Add(cbD30,        &IniConfig.bD30EnableSiteModeSelect,            ECBool,     "Index", "bD30EnableSiteModeSelect",        bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(cbD31,    &IniConfig.bD31RTCChangeRecipeNeedreCreateModel,    ECBool,     "Configuration", "bRTCChangeRecipeNeedreCreateModel", bShow, bEnable, bReadFromFile, 0);//jou 2012-03-01 [D31] RTC Change Recipe Need reCreate RTC Model
    elConfig->Add(cbD32,    &IniConfig.bD32_35TrayPitchIndexDOnMoreThen500MS,   ECBool,     "Index", "b35TrayPitchIndexDOnMoreThen500MSec", bShow, bEnable, bReadFromFile, 0);      //Steven 20120727 : Tray Pitch 大於35mm的話 Index吹氣至少要0.5秒
    elConfig->Add(cbD33,        &IniConfig.bD33RTCInitStartVerify,              ECBool,     "Configuration", "bRTCInitStartVerify",     bShow, bEnable, bReadFromFile, 0);      //Handler Use Model Verify
    elConfig->Add(cbD34,        &IniConfig.bD34GailDMCProtection,               ECBool,     "Index", "bGailDMCProtection",              bShow, bEnable, bReadFromFile, 0);      //ChungHung 20131230 add for ATK    //Steven 20140315 : 改成可以選擇[D34]

    if(IniConfig.bHaveRTCCheckSiteMap)                                          //Steven 20140513
        elConfig->Add(cbD35,    &IniConfig.bD35RTCCheckSiteMap,                 ECBool,     "Index", "bRTCCheckSiteMap",                bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD35,    &IniConfig.bD35RTCCheckSiteMap,                 ECBool,     "Index", "bRTCCheckSiteMap",                bNoShow, bEnable, bFixedValue, 0);

    if(CosFunction.bRTCAutoModelVerify)
    {
        elConfig->Add(cbD36,    &IniConfig.bD36EnableRTCAutoModelVerify,        ECBool,     "Index", "bEnableRTCAutoModelVerify",           bShow, bEnable, bReadFromFile, 0);      //jou 2014-06-24 RTC 自動進行Model驗證
        elConfig->Add(cbD36_1,  &IniConfig.bD36_1EnableRTCAutoModelVerifyLive,  ECBool,     "Index", "bD36_1EnableRTCAutoModelVerifyLive",  bShow, bEnable, bReadFromFile, 0);      //jou 2014-06-24 RTC 自動進行Model驗證
        elConfig->Add(cbD36_2,  &IniConfig.bD36_2AfterOneCycleNeedAutoVerify,   ECBool,     "Index", "bD36_2AfterOneCycleNeedAutoVerify",   bShow, bEnable, bReadFromFile, 0);      //JerryYang 20201116 : One cycle後要執行RTC auto verify
    }
    else
    {
        elConfig->Add(cbD36,    &IniConfig.bD36EnableRTCAutoModelVerify,        ECBool,     "Index", "bEnableRTCAutoModelVerify",           bNoShow, bEnable, bFixedValue, 0);      //jou 2014-06-24 RTC 自動進行Model驗證
        elConfig->Add(cbD36_1,  &IniConfig.bD36_1EnableRTCAutoModelVerifyLive,  ECBool,     "Index", "bD36_1EnableRTCAutoModelVerifyLive",  bNoShow, bEnable, bFixedValue, 0);      //jou 2014-06-24 RTC 自動進行Model驗證
        elConfig->Add(cbD36_2,  &IniConfig.bD36_2AfterOneCycleNeedAutoVerify,   ECBool,     "Index", "bD36_2AfterOneCycleNeedAutoVerify",   bNoShow, bEnable, bFixedValue, 0);      //JerryYang 20201116 : One cycle後要執行RTC auto verify
    }

    if(CosFunction.bManualProcess)                                              //ChungHung 20150526 add for QualComm US
        elConfig->Add(cbD37,    &IniConfig.bD37EnableManualProcess,             ECBool,     "Index", "bEnableManualProcess",                bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD37,    &IniConfig.bD37EnableManualProcess,             ECBool,     "Index", "bEnableManualProcess",                bNoShow, bEnable, bFixedValue, 0);

    elConfig->Add(cbD38,        &IniConfig.bD38IndexPutICToShtNoWaitMotion,     ECBool,     "Index", "bD38IndexPutICToShtNoWaitMotion",     bShow, bEnable, bReadFromFile, 0);      //Steven 20181228 : Add Index Action   //ChungHung 20171116 modify for Index Action

    if(IniConfig.bSPILFunction==true ||                                         //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
       CUSTOMER_CODE==CC_AMKOR_China || IniConfig.bKoreaFunction ||
       CUSTOMER_CODE==CC_KYEC_LEE    ||                                         //wei 20150624 KYEC強制打開
       IniConfig.bMaximFunction ||                                              //JerryYang 20190522 Maxim統一軟體功能
       CUSTOMER_CODE==CC_QUALCOMM)                                              //JerryYang 20170412 (Steven) add QUALCOMM
    {
        elConfig->Add(cbD40,    &IniConfig.bD40IndexICFallDownMustPressFMotorDown, ECBool,  "Index", "bD40IndexICFallDownMustPressFMotorDown", bShow, bDisable, bFixedValue, 1);
    }
    else
    {
        elConfig->Add(cbD40,    &IniConfig.bD40IndexICFallDownMustPressFMotorDown, ECBool,  "Index", "bD40IndexICFallDownMustPressFMotorDown", bShow, bEnable, bReadFromFile, LastSet.bIndexICFallDownMustPressFMotorDown?"1":"0");
    }

    if(IniConfig.bTestIcCheckInContact==true)                                   //ChungHung 20140327 add by Customer
    {
        if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                                   //Jeff 20241001 add for enable and disable checkvacum in contact
           CUSTOMER_CODE==CC_ASE_KaohSiung_K12)
        {
           pnlD41->Visible=true;

           labD41_1->Visible=false;
           labD41_3->Visible=false;
           labD41_2->Visible=false;
           labD41->Visible=false;
        }
        else
        {
           pnlD41->Visible=false;
        }
        elConfig->Add(edD41,    &IniConfig.dD41SocketInitialCheckOffset,        ECDouble,   "Index", "fD41SocketInitialICCheckPositionOffset",  bNoShow, bDisable, bFixedValue, LastSet.dD41SocketInitialCheckOffset, false, 0.0, 10.0);        //Steven 20100818 : [D41]
        elConfig->Add(edD41_2,  &IniConfig.fIndexCheckOffset,                   ECDouble,   "Index", "fIndexCheckOffset",                       bNoShow, bDisable, bFixedValue, 0.0, false, 0.0, 10.0);       //ChungHung 20140807 add for ATK TestZ_Test + fIndexCheckOffset
        elConfig->Add(coD41,    &IniConfig.iD41SocketInitialICCheckPosition,    ECInteger,  "Index", "iD41SocketInitialICCheckPosition",        bNoShow, bDisable, bFixedValue, LastSet.iD41SocketInitialICCheckPosition);
    }
    else
    {
        pnlD41->Visible=true;
        if(CosFunction.bIndexZDownToAboveSocket)
        {
            if(IniConfig.bIndexArm2SupplyLight)
                elConfig->Add(edD41,&IniConfig.dD41SocketInitialCheckOffset,    ECDouble,   "Index", "fD41SocketInitialICCheckPositionOffset",  bShow, bEnable, bReadFromFile, LastSet.dD41SocketInitialCheckOffset, false, 0.0, 20.0);        //Steven 20100818 : [D41]
            else
                elConfig->Add(edD41,&IniConfig.dD41SocketInitialCheckOffset,    ECDouble,   "Index", "fD41SocketInitialICCheckPositionOffset",  bShow, bEnable, bReadFromFile, LastSet.dD41SocketInitialCheckOffset, false, 0.0, 10.0);        //Steven 20100818 : [D41]

            elConfig->Add(edD41_2,  &IniConfig.fIndexCheckOffset,               ECDouble,   "Index", "fIndexCheckOffset",                       bNoShow, bDisable, bFixedValue, 0.0, false);
        }
        else
        {
            double dMaxOffset=10.0;
            AnsiString szDirSecurity=CheckFile(AuthPath, "Security_new.def");   //Jimmychiu 20231128 : Edit Max index check offset by file
            dMaxOffset=CheckRange(CheckAndReadIniData(szDirSecurity, "Input Limit", "D41IndexCheckOffset", 10.0), 5.0, 100.0);
            elConfig->Add(edD41,&IniConfig.dD41SocketInitialCheckOffset,        ECDouble,   "Index", "fD41SocketInitialICCheckPositionOffset",  bNoShow, bDisable, bFixedValue, LastSet.dD41SocketInitialCheckOffset);
            elConfig->Add(edD41_2,  &IniConfig.fIndexCheckOffset,               ECDouble,   "Index", "fIndexCheckOffset",                       bShow, bEnable, bReadFromFile, 0.0, false, 0.0, dMaxOffset);       //ChungHung 20140807 add for ATK TestZ_Test + fIndexCheckOffset
        }

        elConfig->Add(coD41,    &IniConfig.iD41SocketInitialICCheckPosition,    ECInteger,  "Index", "iD41SocketInitialICCheckPosition",        bShow, bEnable, bReadFromFile, LastSet.iD41SocketInitialICCheckPosition);

        if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
           CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                 //kevin 20200716 add ASE_KH 卡輸入
        {
            elConfig->Add(cbD41,&IniConfig.bD41CheckbySetup,                    ECBool,     "Index", "bD41CheckbySetup",                    bShow, bEnable, bReadFromFile, 0); //kevin 20200801 by setup 功能只能用一次
        }
        else
        {
            elConfig->Add(cbD41,&IniConfig.bD41CheckbySetup,                    ECBool,     "Index", "bD41CheckbySetup",                    bNoShow, bDisable, bFixedValue, 0);                         //kevin 20200801 by setup 功能只能用一次
        }
    }

    elConfig->Add(cbD42,        &IniConfig.bD42IndexPickICShuttlePause,         ECBool,     "Index", "bD42IndexPickICShuttlePause",         bShow, bEnable, bReadFromFile, LastSet.bIndexPickICShuttlePause?"1":"0");
    elConfig->Add(cbD43,        &IniConfig.bD43IndexDropErrorCanRetryandSkip,   ECBool,     "Index", "bIndexDropErrorCanRetryandSkip",      bShow, bEnable, bReadFromFile, 0);  //ChungHung 20120717 add Index Drop Error Can Retry and Start
    elConfig->Add(cbD43_1,      &IniConfig.bD43AutoRetryWhenIndexPickErr,       ECBool,     "Index", "bD43AutoRetryWhenIndexPickErr",       bShow, bEnable, bReadFromFile, 0);  //Steven 20170105 : Index吸取異常要退出來用Shuttle Sensor檢查後 再進去吸一次
    elConfig->Add(cbD43_2,      &IniConfig.bD43IndexPickErrCheckSocket,         ECBool,     "Index", "bD43IndexPickErrCheckSocket",         bShow, bEnable, bReadFromFile, 0);  //Steven 20190115 : SCC要求吸取異常要檢查Socket

    if(CUSTOMER_CODE==CC_ASE_CL         ||                                      //JerryYang 20210129 : ASE-CL松諭要求強制開啟
       CUSTOMER_CODE==CC_SIGURD_HUKOU   ||                                      //Sam 20220527 : 矽格湖口 Jonas 要求強制開啟
       CUSTOMER_CODE==CC_SIGURD_PeiXing)                                        //Sam 20240606 : 北興廠也要強制開啟
    {
        elConfig->Add(cbD44,    &IniConfig.bD44CheckIndexICDestroy,             ECBool,     "Index", "bD44CheckIndexICDestroy",             bShow, bDisable, bFixedValue, 1);
    }
    else
    {
        elConfig->Add(cbD44,    &IniConfig.bD44CheckIndexICDestroy,             ECBool,     "Index", "bD44CheckIndexICDestroy",             bShow, bEnable, bReadFromFile, LastSet.bCheckIndexICDestroy?"1":"0");
    }

    if(INDEX_SUCKER_TYPE==1)                                                    //kevin 20121023 設定範圍避免過久壓降掉料
        elConfig->Add(edD44,    &IniConfig.iD44TestHeadCheckVacuumTime,         ECInteger,  "Index", "iD44TestHeadCheckVacuumTime",         bShow, bEnable, bReadFromFile, (int)LastSet.iTestHeadCheckVacuumTime, false,  0.1, 0.15, EUMSToSec);  //0.1~100Sec
    else
        elConfig->Add(edD44,    &IniConfig.iD44TestHeadCheckVacuumTime,         ECInteger,  "Index", "iD44TestHeadCheckVacuumTime",         bShow, bEnable, bReadFromFile, (int)LastSet.iTestHeadCheckVacuumTime, false,  0.1, 100.0, EUMSToSec);  //0.1~100Sec

//    elConfig->Add(edD44_Height, &IniConfig.iD44TestHeadCheckVacuumHeight,       ECInteger,  "Index", "TestHeadCheckVacuumHeight",           bShow, bEnable, bReadFromFile,  0,     false,  0,      2000);        //wei 20150609 延遲破壞高度
    elConfig->Add(cbD45,        &IniConfig.bD45UseOutArmCheckIndex,             ECBool,     "Index", "bD45UseOutArmCheckIndex",             bShow, bEnable, bReadFromFile, "1");

    elConfig->Add(edD46, &IniConfig.iD46WaitIndexDestroyTime,                   ECInteger,  "Index", "iD46WaitIndexDestroyTime",            bShow, bEnable, bReadFromFile,  (int)LastSet.iWaitIndexDestroyTime,     false,  5,      15);

    if(CUSTOMER_CODE==CC_ASE_CL)
        elConfig->Add(cbD47,    &IniConfig.bD47EnableSocketPurgeFunction,       ECBool,     "Index", "bD47EnableSocketPurgeFunction",       bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD47,    &IniConfig.bD47EnableSocketPurgeFunction,       ECBool,     "Index", "bD47EnableSocketPurgeFunction",       bNoShow, bDisable, bFixedValue, 0);

    elConfig->Add(cbbD47,       &IniConfig.iD47SocketPurgeType,                 ECInteger,  "Index", "iD47SocketPurgeType",                 bShow, bEnable, bReadFromFile, 0);    //Steven 20190703 : Socket Purge include shuttle
    elConfig->Add(edD47_Count,  &IniConfig.iD47SocketPurgeCount,                ECInteger,  "Index", "iD47SocketPurgeCount",                bShow, bEnable, bReadFromFile,  (int)LastSet.iWaitIndexDestroyTime, false, 0,    10);
    elConfig->Add(edD47_Time,   &IniConfig.iD47SocketPurgeTime,                 ECInteger,  "Index", "iD47SocketPurgeTime",                 bShow, bEnable, bReadFromFile,  (int)LastSet.iSocketPurgeTime,      false, 0,    10);
    cbLastSet->Add(edD47_3,     &LastSet.iD47SocketTestedCount,                 ECInteger,  "Index", "iD47SocketTestedCount",               bShow, bEnable, bReadFromFile,  10,    false,  0   , 1000000);
    elConfig->Add(edtD47_5,     &IniConfig.dD47_5_ContactOffset,                ECDouble,   "Index", "dD47_5_ContactOffset",                bShow, bEnable, bReadFromFile,  0.0,   false,  5.0, -5.0);
    elConfig->Add(edtD47_6,     &IniConfig.dD47_6_ShuttleOffset,                ECDouble,   "Index", "dD47_6_ShuttleOffset",                bShow, bEnable, bReadFromFile,  0.0,   false,  5.0, -5.0);

    elConfig->Add(cbD48,        &IniConfig.bD48PowerOffEmgCanNotUseZ1Z2,        ECBool, "Index", "bD48DisablePowerOffEmgStopZ1Z2KeyButton", bShow, bEnable, bReadFromFile, LastSet.bD48PowerOffEmgCanNotUseZ1Z2?"1":"0");
    if(CosFunction.bRTCAlarmSetIndexToErrBin)                                   //JerryYang 20160712 for 力成發生RTC Alarm時把Index上所有IC設為Errorbin
        elConfig->Add(cbD49,    &IniConfig.bD49RTCAlarmSetIndexToErrBin,        ECBool,     "Index", "bD49RTCAlarmSetIndexToErrBin",        bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD49,    &IniConfig.bD49RTCAlarmSetIndexToErrBin,        ECBool,     "Index", "bD49RTCAlarmSetIndexToErrBin",        bNoShow, bEnable, bFixedValue, 0);

    if(CosFunction.bIndexPickErrSkipNeedCheckVac)                               //jou 20171031 (Steven) : 增加開關JSCC要求index pick up error 需再慢速下降吸一次
        elConfig->Add(cbD50,    &IniConfig.bD50IndexPickErrSkipNeedCheckVac,    ECBool,     "Index", "bD50IndexPickErrSkipNeedCheckVac",    bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD50,    &IniConfig.bD50IndexPickErrSkipNeedCheckVac,    ECBool,     "Index", "bD50IndexPickErrSkipNeedCheckVac",    bNoShow, bEnable, bFixedValue, 0);
    elConfig->Add(cbD51, &IniConfig.bD51UseOnecycleCleanOutFinishTestArmAtRear, ECBool,     "Index", "bD51UseOnecycleCleanOutFinishTestArmAtRear", bShow, bEnable, bReadFromFile, LastSet.bUseOnecycleCleanOutFinishTestArmAtRear?"1":"0");
    elConfig->Add(cbD52,        &IniConfig.bD52InterFaceErrHeadNeedUp,          ECBool,     "Index", "bD52InterFaceErrHeadNeedUp",          bShow, bEnable, bReadFromFile, LastSet.bInterFaceErrHeadNeedUp?"1":"0");
    elConfig->Add(cbD53,        &IniConfig.bD53CCDLightOn,                      ECBool,     "Index", "bD53CCDLightOn",                      bShow, bEnable, bReadFromFile, LastSet.bCCDLightOn?"1":"0");

    elConfig->Add(edD53,        &IniConfig.iD53LightOnMin,                      ECInteger,  "Index", "iD53LightOnMin",                      bShow, bEnable, bReadFromFile,  (int)LastSet.iLightOnMin,    false,  1,      10000);
    elConfig->Add(cbD54,        &IniConfig.bD54SlowDown,                        ECBool,     "Index", "bSlowDown",                           bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edD54,        &IniConfig.iD54SlowDownScale,                   ECInteger,  "Index", "iSlowDownScale",                      bShow, bEnable, bReadFromFile,  50,    false,  1,      100);
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        elConfig->Add(cbD55,    &IniConfig.bD55DisableIndexCheck,               ECBool,     "Index", "bDisableIndexCheck",                  bNoShow, bDisable, bFixedValue, 0);
    }
    // AI(W906-FW-CFG-W2) 20260825: GATE (CFG2-COM2) -- NARROWEST FORM. golden's
    // condition is `REAL_TIME_CCD==false || (COM2!=NULL && COM2->bCCDDummyRum)`.
    // The first disjunct is real and stays LIVE; only the second is gated.
    // COM2 exists in this tree as TCOM2Shim (atester_shims.h:405), but that
    // shim deliberately carries a scoped member list and `bCCDDummyRum` is NOT
    // one of its members -- it appears there only inside comments.
    //   cmd: grep -n "bCCDDummyRum" atester_shims.h atester_shims.cpp
    //        -> 3 hits, all comment text, no member (20260825)
    // Gating the WHOLE arm would have been wrong: `REAL_TIME_CCD==false` is the
    // ordinary case on every machine without real-time CCD, and golden shows
    // that arm's checkbox as visible-but-disabled. Keeping it live preserves
    // that. What is lost is narrow and stated: on a machine with RTC ON that is
    // ALSO running the CCD in dummy mode, golden takes this arm and this port
    // falls through to the next one, so cbD55 ends up enabled instead of
    // disabled. A UI enable flag; no machine action depends on it.
    else if(REAL_TIME_CCD==false
#if 0 // GATE (CFG2-COM2) -- COM2->bCCDDummyRum has no port (grep + date above)
            || (COM2!=NULL && COM2->bCCDDummyRum)
#endif // GATE (CFG2-COM2)
           )                                                                    //RTC 沒有開不能Disable IndexCheck
    {
        elConfig->Add(cbD55,    &IniConfig.bD55DisableIndexCheck,               ECBool,     "Index", "bDisableIndexCheck",                  bShow, bDisable, bFixedValue, 0);
    }
    else if(IniConfig.bM0109RTCOffCheckYieldPiggyBack)                          //Steven 20160727 : Fixed for SCK
    {
        elConfig->Add(cbD55,    &IniConfig.bD55DisableIndexCheck,               ECBool,     "Index", "bDisableIndexCheck",                  bShow, bDisable, bFixedValue, 1);
    }
    else                                                                        //ChungHung 20120710 add SCK Disable IndexCheck
    {
        elConfig->Add(cbD55,    &IniConfig.bD55DisableIndexCheck,               ECBool,     "Index", "bDisableIndexCheck",                  bShow, bEnable, bReadFromFile, 0);
    }
    elConfig->Add(cbD56,        &IniConfig.bD56YieldPiggyBackEnable,            ECBool,     "Index", "bYieldPiggyBackEnable",               bShow, bEnable, bReadFromFile, 0);  //kevin 20131009 強致 Enable Yield 裡面piggyback 功能

    if(IniConfig.bKoreaFunction || CUSTOMER_CODE==CC_ASE_KaohSiung)             //kevin 20141203 關site顯示 site號碼
        elConfig->Add(cbD57,    &IniConfig.bD57SiteMapCloseDisplay,             ECBool,     "Index", "bSiteMapCloseDisplay",                bShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbD57,    &IniConfig.bD57SiteMapCloseDisplay,             ECBool,     "Index", "bSiteMapCloseDisplay",                bShow, bEnable, bReadFromFile, 0);

    elConfig->Add(cbD58,        &IniConfig.bD58UseArm1PickPlaceArm2Test,        ECBool,     "Index", "bUseArm1PickPlaceArm2Test",           bShow, bEnable, bReadFromFile, 0);  //kevin 20150127 讓setup 出現功能  Arm1 下壓 arm2 測試
    if(MachineTypeChoice==Type_HT9046_LS)                                       //Steven 20150910 : 32Site 雙Arm一起吸放
        elConfig->Add(cbD59,    &IniConfig.bD59_32SitePnpTogether,              ECBool,     "Index", "bD59_32SitePnpTogether",              bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD59,    &IniConfig.bD59_32SitePnpTogether,              ECBool,     "Index", "bD59_32SitePnpTogether",              bNoShow, bDisable, bFixedValue, 0);

    if(CosFunction.bIndexArmVacOffErrNeedPiggyBack)                             //JerryYang 20160815 Index arm 發生Vaccum off error要做piggy back
        elConfig->Add(cbD61,    &IniConfig.bD61IndexArmVacOffErrNeedPiggyBack,  ECBool,     "Index", "bD61IndexArmVacOffErrNeedPiggyBack",  bShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbD61,    &IniConfig.bD61IndexArmVacOffErrNeedPiggyBack,  ECBool,     "Index", "bD61IndexArmVacOffErrNeedPiggyBack",  bShow, bEnable, bReadFromFile, 0);

    elConfig->Add(cbD62,        &IniConfig.bD62PickUpErrorNeedPurge,            ECBool,     "Index", "bD62PickUpErrorNeedPurge",            bShow, bEnable, bReadFromFile, 0);  //Steveb 20161024 : 吸取異常需要吹氣一次

    if(CosFunction.bIndexPickErrSkipBlowAirTime)                                //Frank 20171213 (Steven) : Index Pick Err In Shuttle Skip and Blow Air
    {
        elConfig->Add(edD62,    &IniConfig.iD62IndexBlowAirTime,                ECInteger,  "Index", "iD62IndexBlowAirTime",                bShow, bEnable, bReadFromFile,  0,     false,  1,      10);
        labD62->Visible=true;
    }
    else
    {
        elConfig->Add(edD62,    &IniConfig.iD62IndexBlowAirTime,                ECInteger,  "Index", "iD62IndexBlowAirTime",                bNoShow, bDisable, bFixedValue, 0);
        labD62->Visible=false;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        pnlD63->Visible=true;
        if(CosFunction.bHiSiliconFunction==true)
        {
            elConfig->Add(cbD63,&IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange, ECBool,"Index", "bD63CheckIndexZHomeToZPhaseDistanceRange", bShow, bDisable, bFixedValue, 1);   //kevin 20170515 (wei) Zphase home
            elConfig->Add(cbD63_1,  &IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess,   ECBool,"Index", "bD63_1FindMotorPhaseEveryGoHomeProcess"  , bShow, bEnable, bReadFromFile, 1);
        }
        else
        {
            elConfig->Add(cbD63,&IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange, ECBool,"Index", "bD63CheckIndexZHomeToZPhaseDistanceRange", bShow, bDisable, bFixedValue, 0);  //kevin 20170515 (wei) Zphase home
            elConfig->Add(cbD63_1,  &IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess,   ECBool,"Index", "bD63_1FindMotorPhaseEveryGoHomeProcess"  , bShow, bDisable, bFixedValue, 0);
        }
        elConfig->Add(cbD64,    &IniConfig.bD64IndexPickErrOnlySKIP,                 ECBool,"Index", "bD64IndexPickErrOnlySKIP",                 bShow, bEnable, bReadFromFile, 0);     //kevin 20171103 (wei) index pick up error only skip
    }
    else if(CosFunction.bHiSiliconFunction==true)                               //Ifor 20170809 (wei) add 海思版本強制開啟 Z Phase 功能
    {
        pnlD63->Visible=true;
        elConfig->Add(cbD63,    &IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange, ECBool,"Index", "bD63CheckIndexZHomeToZPhaseDistanceRange", bShow, bDisable, bFixedValue, 1);      //kevin 20170515 (wei) Zphase home
        elConfig->Add(cbD63_1,  &IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess,   ECBool,"Index", "bD63_1FindMotorPhaseEveryGoHomeProcess"  , bShow, bEnable,  bReadFromFile, 0);
        elConfig->Add(cbD64,    &IniConfig.bD64IndexPickErrOnlySKIP,                 ECBool,"Index", "bD64IndexPickErrOnlySKIP",                 bShow, bDisable, bFixedValue, 0);      //kevin 20171103 (wei) index pick up error only skip
    }
    else if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                    //kevin 20180727 move
    {
        pnlD63->Visible=true;
        elConfig->Add(cbD63,    &IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange, ECBool,"Index", "bD63CheckIndexZHomeToZPhaseDistanceRange", bShow, bEnable, bReadFromFile, 0);     //kevin 20170515 (wei) Zphase home
        elConfig->Add(cbD63_1,  &IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess,   ECBool,"Index", "bD63_1FindMotorPhaseEveryGoHomeProcess"  , bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbD64,    &IniConfig.bD64IndexPickErrOnlySKIP,                 ECBool,"Index", "bD64IndexPickErrOnlySKIP",                 bShow, bEnable, bReadFromFile, 0);     //kevin 20171103 (wei) index pick up error only skip
    }
    else if(CUSTOMER_CODE==CC_GIGAS || CUSTOMER_CODE==CC_HONPREC_QC)            //Isaac 20201110 : Index Y find motor phase
    {
        pnlD63->Visible=true;
        elConfig->Add(cbD63,    &IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange, ECBool,"Index", "bD63CheckIndexZHomeToZPhaseDistanceRange", bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbD63_1,  &IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess,   ECBool,"Index", "bD63_1FindMotorPhaseEveryGoHomeProcess"  , bShow, bEnable, bReadFromFile, 0);     //Isaac 20201110 : Index Y find motor phase
    }
    else if(IniConfig.bSPILFunction)                                            //Isaac 20201110 : Index Y find motor phase
    {
        pnlD63->Visible=true;
        elConfig->Add(cbD63,    &IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange, ECBool,"Index", "bD63CheckIndexZHomeToZPhaseDistanceRange", bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbD63_1,  &IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess,   ECBool,"Index", "bD63_1FindMotorPhaseEveryGoHomeProcess"  , bShow, bEnable, bReadFromFile, 0);     //Isaac 20201110 : Index Y find motor phase
        elConfig->Add(cbD64,    &IniConfig.bD64IndexPickErrOnlySKIP,                 ECBool,"Index", "bD64IndexPickErrOnlySKIP",                 bShow, bDisable, bFixedValue, 0);      //kevin 20171103 (wei) index pick up error only skip
    }
    else
    {
        pnlD63->Visible=false;
        elConfig->Add(cbD63,    &IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange, ECBool,"Index", "bD63CheckIndexZHomeToZPhaseDistanceRange", bShow, bDisable, bFixedValue, 0);      //kevin 20170515 (wei) Zphase home
        elConfig->Add(cbD63_1,  &IniConfig.bD63_1FindMotorPhaseEveryGoHomeProcess,   ECBool,"Index", "bD63_1FindMotorPhaseEveryGoHomeProcess"  , bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbD64,    &IniConfig.bD64IndexPickErrOnlySKIP,                 ECBool,"Index", "bD64IndexPickErrOnlySKIP",                 bShow, bEnable, bReadFromFile, 0);     //kevin 20171103 (wei) index pick up error only skip
    }

    if(IniConfig.bSPILFunction==true && CosFunction.bHiSiliconFunction==true)   //JerryYang 20170921 (Steven) SPIL守志要求range要鎖定不得修改
        elConfig->Add(edD63,    &IniConfig.iD63IndexZHomeToZPhaseRange,         ECInteger,  "Index", "iD63IndexZHomeToZPhaseRange",             bShow, bDisable, bFixedValue,   10,    false,  1,      10);       //Isaac 20201124 : default:50->10,Range:1~1000->1~10
    else
        elConfig->Add(edD63,    &IniConfig.iD63IndexZHomeToZPhaseRange,         ECInteger,  "Index", "iD63IndexZHomeToZPhaseRange",             bShow, bEnable, bReadFromFile,  10,    false,  1,      10);           //kevin 20170515 (wei) add Z phase range   //Isaac 20201124 : default:100->10,Range:1~1000->1~10

    if(CUSTOMER_CODE==CC_JCET)                                                  //Ifor 20171123 add
        elConfig->Add(cbD65,    &IniConfig.bD65EnableCheckSocketsensorFunction, ECBool,     "Index", "bD65EnableCheckSocketsensorFunction",     bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD65,    &IniConfig.bD65EnableCheckSocketsensorFunction, ECBool,     "Index", "bD65EnableCheckSocketsensorFunction",     bNoShow, bEnable, bFixedValue, 0);
    elConfig->Add(cbD66,        &IniConfig.bD66Initialstartautoheight,          ECBool,     "Index", "bD66Initialstartautoheight",              bNoShow, bEnable, bFixedValue, 0);          //wei 20180227 Initial start auto height

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20190907 Arm 測區次數道量測 功能;
        elConfig->Add(cbD67,    &IniConfig.bD67LoadCellMeasure,                 ECBool,     "Index", "bD67LoadCellMeasure",                     bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD67,    &IniConfig.bD67LoadCellMeasure,                 ECBool,     "Index", "bD67LoadCellMeasure",                     bNoShow, bEnable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20190925 : add KYEC 要求contact Hight 高度變更超過設定需報警
    {
        labD68_1->Visible=true;
        elConfig->Add(edD68,    &IniConfig.dD68DistanceRange,                   ECDouble,   "Index", "dD68DistanceRange",                       bShow, bEnable, bReadFromFile,  10,   false,  0.01,      10.0);        //Ifor 20190925 : add KYEC 要求contact Hight 高度變更超過設定需報警
    }
    else
    {
        labD68_1->Visible=false;
        elConfig->Add(edD68,    &IniConfig.dD68DistanceRange,                   ECDouble,   "Index", "dD68DistanceRange",                       bNoShow, bDisable, bFixedValue,  10,   false,  0.01,      10.0);
    }

    if(CosFunction.bAfterAutoCleanNoIndexCheck)                                 //Steven 20191212 : 劉仁洲說Auto Clean只要作一次Index Check
        elConfig->Add(rgD69,    &IniConfig.iD69IndexCheckModeForAutoClean,      ECInteger,  "Index", "iD69IndexCheckModeForAutoClean",          bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(rgD69,    &IniConfig.iD69IndexCheckModeForAutoClean,      ECInteger,  "Index", "iD69IndexCheckModeForAutoClean",          bNoShow, bEnable, bFixedValue, 0);

    if(CosFunction.bIndexCycleTimeRecord)                                       //Sam 20200916 : Add Index Cycle Time Record
        elConfig->Add(cbD70,    &IniConfig.bD70IndexCycleTimeRecord,            ECBool,     "Index", "bD70IndexCycleTimeRecord",                bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbD70,    &IniConfig.bD70IndexCycleTimeRecord,            ECBool,     "Index", "bD70IndexCycleTimeRecord",                bNoShow, bEnable, bFixedValue, 0);

    if(CosFunction.bIndexCheckCanTurnOff)                                       //Isaac 20211019 : 可選擇做index check的時機
    {
        elConfig->Add(rgD71,    &IniConfig.iD71IndexCheckOnOffMode,             ECInteger,  "Index", "iD71IndexCheckOnOffMode",                 bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(rgD71,    &IniConfig.iD71IndexCheckOnOffMode,             ECInteger,  "Index", "iD71IndexCheckOnOffMode",                 bNoShow, bEnable, bFixedValue, 0);
    }

    elConfig->Add(cbD72,        &IniConfig.bD72NNModeMoveShtAfterContact,       ECBool,     "Index", "bD72NNModeMoveShtAfterContact",           bShow, bEnable, bReadFromFile, 0);                  //Steven 20220531 : index下壓之後才能移動shuttle
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        elConfig->Add(cbD73,    &IniConfig.bD73ContactModeFast,                 ECBool,     "Index", "bD73ContactModeFast",                     bShow, bDisable, bFixedValue, 1);                   //kevin 20220827 : 高雄 Contact mode fast
    else
        elConfig->Add(cbD73,    &IniConfig.bD73ContactModeFast,                 ECBool,     "Index", "bD73ContactModeFast",                     bShow, bEnable, bReadFromFile, 0);                  //kevin 20220817 : Conttact mode 加速

    if(REAL_TIME_CCD)                         //Sam 20240117 : OneCycle 完成做完 Full view check 後都需要做 RTC Learning golden
    {
        elConfig->Add(cbD74,        &IniConfig.bD74RTCAutoTuning, ECBool,  "Index", "bD74RTCAutoTuning", bShow, bEnable, bReadFromFile, 0);      //Sam 20240711 : RTC Auto Tuning
        if(CUSTOMER_CODE==CC_SIGURD_HUKOU)     //Sam 20240709 : 矽格湖口家葳要求要強制鎖定開啟
            elConfig->Add(cbD75,    &IniConfig.bD75OneCycleFinishedAlawayLearnRTCGolden, ECBool,"Index", "bD75OneCycleFinishedAlawayLearnRTCGolden", bShow, bDisable, bFixedValue, 0);
        else
            elConfig->Add(cbD75,    &IniConfig.bD75OneCycleFinishedAlawayLearnRTCGolden, ECBool,"Index", "bD75OneCycleFinishedAlawayLearnRTCGolden", bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbD74,    &IniConfig.bD74RTCAutoTuning                       , ECBool, "Index", "bD74RTCAutoTuning",                        bNoShow, bEnable, bFixedValue, 0);   //Sam 20240711 : RTC Auto Tuning
        elConfig->Add(cbD75,    &IniConfig.bD75OneCycleFinishedAlawayLearnRTCGolden, ECBool, "Index", "bD75OneCycleFinishedAlawayLearnRTCGolden", bNoShow, bEnable, bFixedValue, 0);
    }

    if(CosFunction.bUseIndexCheckHasICNeedPurge==true)                          //Ifor 20200622 add:Index Check Has IC Need Purge
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE  && bEnable_KLT_Function==false)//Ifor 20230619 add:KYEC要求強制開啟D68功能
        {
            elConfig->Add(cbD78,&IniConfig.bD78EnableIndexCheckHasICNeedPurge,  ECBool,     "Index", "bD78EnableIndexCheckHasICNeedPurge",  bShow, bDisable, bFixedValue, 1);
        }
        else
        {
            elConfig->Add(cbD78,&IniConfig.bD78EnableIndexCheckHasICNeedPurge,  ECBool,     "Index", "bD78EnableIndexCheckHasICNeedPurge",  bShow, bEnable, bReadFromFile, 0);
        }
    }
    else
    {
        elConfig->Add(cbD78,    &IniConfig.bD78EnableIndexCheckHasICNeedPurge,  ECBool,     "Index", "bD78EnableIndexCheckHasICNeedPurge",  bNoShow, bEnable, bFixedValue, 0);
    }

    if(CosFunction.bUseIndexPickShuttleErrNeedPurge==true)                      //Ifor 20200622 add:Index Pick Shuttle Err Need Purge
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE  && bEnable_KLT_Function==false)          //Ifor 20230619 add:KYEC要求強制開啟D69功能
        {
            elConfig->Add(cbD79,&IniConfig.bD79EnableIndexPickShuttleErrNeedPurge,ECBool,   "Index", "bD79EnableIndexPickShuttleErrNeedPurge", bShow, bDisable, bFixedValue, 1);
        }
        else
        {
            elConfig->Add(cbD79,&IniConfig.bD79EnableIndexPickShuttleErrNeedPurge,ECBool,   "Index", "bD79EnableIndexPickShuttleErrNeedPurge", bShow, bEnable, bReadFromFile, 0);
        }
    }
    else
    {
        elConfig->Add(cbD79,    &IniConfig.bD79EnableIndexPickShuttleErrNeedPurge,ECBool,   "Index", "bD79EnableIndexPickShuttleErrNeedPurge", bNoShow, bEnable, bFixedValue, 0);
    }

    elConfig->Add(cbD80,        &IniConfig.bD80AfterOutSHToRightSiteIndexCheck, ECBool,     "Index", "bD80EnableIndexPickShuttleErrNeedPurge", bShow, bEnable, bReadFromFile, 1);

    elConfig->Add(cbD81,        &IniConfig.bD81IndexCheckVacuumOnShuttle,       ECBool,     "Index", "bD81IndexCheckVacuumOnShuttle",          bShow, bEnable, bReadFromFile, 0);
    if(CosFunction.bSocketSensorCheckICAtArmDown)                               //Jimmychiu 20250826 : 每次下壓確認有IC在socket
    {
        elConfig->Add(cbD82,    &IniConfig.bD82CheckIndexHasIC,                 ECBool,     "Index", "bD82CheckIndexHasIC",                    bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbD82,    &IniConfig.bD82CheckIndexHasIC,                 ECBool,     "Index", "bD82CheckIndexHasIC",                    bNoShow, bDisable, bFixedValue, 0);
    }
}

// AI(W906-FW-CFG-W2) 20260825: golden cConfiguration.cpp:1761-2111, transcribed
// VERBATIM (cp950 -> UTF-8). Registration table only -- the port's
// HTEditList::Add signature is argument-for-argument golden's, so no
// adaptation was needed and none was invented. See FW-CFG-W1's banner in
// forms/fConfiguration.h for why this family is the wave's point.
void TfConfiguration::InitConfigEdtList_ItemE()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;

//    cbTest->Add(edE31_Au1Y,   &IniConfig.dTrayYScale[eAuto1],                ECDouble,   "In/Out Arm", "dTrayYScaleAuto1",                   bShow, bEnable, bReadFromFile, LastSet.fTrayYScale[0], true, 0.95, 1.05);

    cbLastSet->Add(cbE30,       &IniConfig.bE30InArmUseDifferentScale,          ECBool,     "In/Out Arm", "bE30InArmUseDifferentScale",         bShow, bEnable, bReadFromFile, LastSet.bE30InArmUseDifferentScale?"1":"0");         //Steven 20090710

    if(Tri_Temp_Machine==1)
    {
        cbLastSet->Add(cbE30_1,&IniConfig.bE30_1InArmUseDifferentScale_Hot,     ECBool,     "In/Out Arm", "bE30_1InArmUseDifferentScale_Hot",     bShow, bEnable, bReadFromFile, 0);      //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature -->
        cbLastSet->Add(cbE30_2,&IniConfig.bE30_2InArmUseDifferentScale_Cold,    ECBool,     "In/Out Arm", "bE30_2InArmUseDifferentScale_Cold",    bShow, bEnable, bReadFromFile, 0);
    }

    cbLastSet->Add(cbE31,       &IniConfig.bE31OutArmUseDifferentScale,         ECBool,     "In/Out Arm", "bE31OutArmUseDifferentScale",        bShow, bEnable, bReadFromFile, LastSet.bE31OutArmUseDifferentScale?"1":"0");        //Steven 20090710
    if(Tri_Temp_Machine==1)
    {
        cbLastSet->Add(cbE31_1,        &IniConfig.bE31_1OutArmUseDifferentScale_Hot,     ECBool,     "In/Out Arm", "bE31_1OutArmUseDifferentScale_Hot",    bShow, bEnable, bReadFromFile, 0);
        cbLastSet->Add(edE31_1_Au1X,   &IniConfig.dTrayXScale_Hot[eAuto1],      ECDouble,   "In/Out Arm", "dTrayXScaleAuto1_Hot",               bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Hot[0], false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Au1Y,   &IniConfig.dTrayYScale_Hot[eAuto1],      ECDouble,   "In/Out Arm", "dTrayYScaleAuto1_Hot",               bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Hot[0], false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Au2X,   &IniConfig.dTrayXScale_Hot[eAuto2],      ECDouble,   "In/Out Arm", "dTrayXScaleAuto2_Hot",               bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Hot[1], false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Au2Y,   &IniConfig.dTrayYScale_Hot[eAuto2],      ECDouble,   "In/Out Arm", "dTrayYScaleAuto2_Hot",               bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Hot[1], false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Au3X,   &IniConfig.dTrayXScale_Hot[eAuto3],      ECDouble,   "In/Out Arm", "dTrayXScaleAuto3_Hot",               bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Hot[2], false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Au3Y,   &IniConfig.dTrayYScale_Hot[eAuto3],      ECDouble,   "In/Out Arm", "dTrayYScaleAuto3_Hot",               bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Hot[2], false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Au4X,   &IniConfig.dTrayXScale_Hot[eAuto4],      ECDouble,   "In/Out Arm", "dTrayXScaleAuto4_Hot",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Au4Y,   &IniConfig.dTrayYScale_Hot[eAuto4],      ECDouble,   "In/Out Arm", "dTrayYScaleAuto4_Hot",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Au5X,   &IniConfig.dTrayXScale_Hot[eAuto5],      ECDouble,   "In/Out Arm", "dTrayXScaleAuto5_Hot",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Au5Y,   &IniConfig.dTrayYScale_Hot[eAuto5],      ECDouble,   "In/Out Arm", "dTrayYScaleAuto5_Hot",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Au6X,   &IniConfig.dTrayXScale_Hot[eAuto6],      ECDouble,   "In/Out Arm", "dTrayXScaleAuto6_Hot",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Au6Y,   &IniConfig.dTrayYScale_Hot[eAuto6],      ECDouble,   "In/Out Arm", "dTrayYScaleAuto6_Hot",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Fi1X,   &IniConfig.dTrayXScale_Hot[eFix1],       ECDouble,   "In/Out Arm", "dTrayXScaleFix1_Hot",                bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Hot[3], false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Fi1Y,   &IniConfig.dTrayYScale_Hot[eFix1],       ECDouble,   "In/Out Arm", "dTrayYScaleFix1_Hot",                bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Hot[3], false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Fi2X,   &IniConfig.dTrayXScale_Hot[eFix2],       ECDouble,   "In/Out Arm", "dTrayXScaleFix2_Hot",                bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Hot[4], false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Fi2Y,   &IniConfig.dTrayYScale_Hot[eFix2],       ECDouble,   "In/Out Arm", "dTrayYScaleFix2_Hot",                bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Hot[4], false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Fi3X,   &IniConfig.dTrayXScale_Hot[eFix3],       ECDouble,   "In/Out Arm", "dTrayXScaleFix3_Hot",                bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Hot[5], false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Fi3Y,   &IniConfig.dTrayYScale_Hot[eFix3],       ECDouble,   "In/Out Arm", "dTrayYScaleFix3_Hot",                bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Hot[5], false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Fi4X,   &IniConfig.dTrayXScale_Hot[eFix4],       ECDouble,   "In/Out Arm", "dTrayXScaleFix4_Hot",                bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Fi4Y,   &IniConfig.dTrayYScale_Hot[eFix4],       ECDouble,   "In/Out Arm", "dTrayYScaleFix4_Hot",                bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Fi5X,   &IniConfig.dTrayXScale_Hot[eFix5],       ECDouble,   "In/Out Arm", "dTrayXScaleFix5_Hot",                bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Fi5Y,   &IniConfig.dTrayYScale_Hot[eFix5],       ECDouble,   "In/Out Arm", "dTrayYScaleFix5_Hot",                bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_1_Fi6X,   &IniConfig.dTrayXScale_Hot[eFix6],       ECDouble,   "In/Out Arm", "dTrayXScaleFix6_Hot",                bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_1_Fi6Y,   &IniConfig.dTrayYScale_Hot[eFix6],       ECDouble,   "In/Out Arm", "dTrayYScaleFix6_Hot",                bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(cbE31_2,        &IniConfig.bE31_2OutArmUseDifferentScale_Cold,   ECBool,     "In/Out Arm", "bE31_2OutArmUseDifferentScale_Cold",   bShow, bEnable, bReadFromFile, 0);
        cbLastSet->Add(edE31_2_Au1X,   &IniConfig.dTrayXScale_Cold[eAuto1],     ECDouble,   "In/Out Arm", "dTrayXScaleAuto1_Cold",              bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Cold[0], false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Au1Y,   &IniConfig.dTrayYScale_Cold[eAuto1],     ECDouble,   "In/Out Arm", "dTrayYScaleAuto1_Cold",              bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Cold[0], false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Au2X,   &IniConfig.dTrayXScale_Cold[eAuto2],     ECDouble,   "In/Out Arm", "dTrayXScaleAuto2_Cold",              bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Cold[1], false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Au2Y,   &IniConfig.dTrayYScale_Cold[eAuto2],     ECDouble,   "In/Out Arm", "dTrayYScaleAuto2_Cold",              bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Cold[1], false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Au3X,   &IniConfig.dTrayXScale_Cold[eAuto3],     ECDouble,   "In/Out Arm", "dTrayXScaleAuto3_Cold",              bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Cold[2], false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Au3Y,   &IniConfig.dTrayYScale_Cold[eAuto3],     ECDouble,   "In/Out Arm", "dTrayYScaleAuto3_Cold",              bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Cold[2], false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Au4X,   &IniConfig.dTrayXScale_Cold[eAuto4],     ECDouble,   "In/Out Arm", "dTrayXScaleAuto4_Cold",              bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Au4Y,   &IniConfig.dTrayYScale_Cold[eAuto4],     ECDouble,   "In/Out Arm", "dTrayYScaleAuto4_Cold",              bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Au5X,   &IniConfig.dTrayXScale_Cold[eAuto5],     ECDouble,   "In/Out Arm", "dTrayXScaleAuto5_Cold",              bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Au5Y,   &IniConfig.dTrayYScale_Cold[eAuto5],     ECDouble,   "In/Out Arm", "dTrayYScaleAuto5_Cold",              bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Au6X,   &IniConfig.dTrayXScale_Cold[eAuto6],     ECDouble,   "In/Out Arm", "dTrayXScaleAuto6_Cold",              bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Au6Y,   &IniConfig.dTrayYScale_Cold[eAuto6],     ECDouble,   "In/Out Arm", "dTrayYScaleAuto6_Cold",              bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Fi1X,   &IniConfig.dTrayXScale_Cold[eFix1],      ECDouble,   "In/Out Arm", "dTrayXScaleFix1_Cold",               bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Cold[3], false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Fi1Y,   &IniConfig.dTrayYScale_Cold[eFix1],      ECDouble,   "In/Out Arm", "dTrayYScaleFix1_Cold",               bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Cold[3], false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Fi2X,   &IniConfig.dTrayXScale_Cold[eFix2],      ECDouble,   "In/Out Arm", "dTrayXScaleFix2_Cold",               bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Cold[4], false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Fi2Y,   &IniConfig.dTrayYScale_Cold[eFix2],      ECDouble,   "In/Out Arm", "dTrayYScaleFix2_Cold",               bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Cold[4], false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Fi3X,   &IniConfig.dTrayXScale_Cold[eFix3],      ECDouble,   "In/Out Arm", "dTrayXScaleFix3_Cold",               bShow, bEnable, bReadFromFile, LastSet.fTrayXScale_Cold[5], false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Fi3Y,   &IniConfig.dTrayYScale_Cold[eFix3],      ECDouble,   "In/Out Arm", "dTrayYScaleFix3_Cold",               bShow, bEnable, bReadFromFile, LastSet.fTrayYScale_Cold[5], false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Fi4X,   &IniConfig.dTrayXScale_Cold[eFix4],      ECDouble,   "In/Out Arm", "dTrayXScaleFix4_Cold",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Fi4Y,   &IniConfig.dTrayYScale_Cold[eFix4],      ECDouble,   "In/Out Arm", "dTrayYScaleFix4_Cold",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Fi5X,   &IniConfig.dTrayXScale_Cold[eFix5],      ECDouble,   "In/Out Arm", "dTrayXScaleFix5_Cold",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Fi5Y,   &IniConfig.dTrayYScale_Cold[eFix5],      ECDouble,   "In/Out Arm", "dTrayYScaleFix5_Cold",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);

        cbLastSet->Add(edE31_2_Fi6X,   &IniConfig.dTrayXScale_Cold[eFix6],      ECDouble,   "In/Out Arm", "dTrayXScaleFix6_Cold",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
        cbLastSet->Add(edE31_2_Fi6Y,   &IniConfig.dTrayYScale_Cold[eFix6],      ECDouble,   "In/Out Arm", "dTrayYScaleFix6_Cold",               bShow, bEnable, bReadFromFile, 1.0, false, 0.95, 1.05);
    }
    cbLastSet->Add(cbE32,       &IniConfig.bE32ShuttleUseDifferentScale,        ECBool,     "In/Out Arm", "bE32ShuttleUseDifferentScale",       bShow, bEnable, bReadFromFile, LastSet.bE32ShuttleUseDifferentScale?"1":"0");       //Steven 20090717

    if(Tri_Temp_Machine==1)
    {
        cbLastSet->Add(cbE32_1,&IniConfig.bE32_1ShuttleUseDifferentScale_Hot,   ECBool,     "In/Out Arm", "bE32_1ShuttleUseDifferentScale_Hot",   bShow, bEnable, bReadFromFile, 0);
        cbLastSet->Add(cbE32_2,&IniConfig.bE32_2ShuttleUseDifferentScale_Cold,  ECBool,     "In/Out Arm", "bE32_1ShuttleUseDifferentScale_Cold",  bShow, bEnable, bReadFromFile, 0);      //Ztex 2024.11.18 Add In\Out\Sht Different Scale By Temperature <--
    }

    cbLastSet->Add(edE31_Au1X,   &IniConfig.dTrayXScale[eAuto1],                ECDouble,   "In/Out Arm", "dTrayXScaleAuto1",                   bShow, bEnable, bReadFromFile, LastSet.fTrayXScale[0], true, 0.95, 1.05);
    cbLastSet->Add(edE31_Au1Y,   &IniConfig.dTrayYScale[eAuto1],                ECDouble,   "In/Out Arm", "dTrayYScaleAuto1",                   bShow, bEnable, bReadFromFile, LastSet.fTrayYScale[0], true, 0.95, 1.05);

    cbLastSet->Add(edE31_Au2X,   &IniConfig.dTrayXScale[eAuto2],                ECDouble,   "In/Out Arm", "dTrayXScaleAuto2",                   bShow, bEnable, bReadFromFile, LastSet.fTrayXScale[1], true, 0.95, 1.05);
    cbLastSet->Add(edE31_Au2Y,   &IniConfig.dTrayYScale[eAuto2],                ECDouble,   "In/Out Arm", "dTrayYScaleAuto2",                   bShow, bEnable, bReadFromFile, LastSet.fTrayYScale[1], true, 0.95, 1.05);

    cbLastSet->Add(edE31_Au3X,   &IniConfig.dTrayXScale[eAuto3],                ECDouble,   "In/Out Arm", "dTrayXScaleAuto3",                   bShow, bEnable, bReadFromFile, LastSet.fTrayXScale[2], true, 0.95, 1.05);
    cbLastSet->Add(edE31_Au3Y,   &IniConfig.dTrayYScale[eAuto3],                ECDouble,   "In/Out Arm", "dTrayYScaleAuto3",                   bShow, bEnable, bReadFromFile, LastSet.fTrayYScale[2], true, 0.95, 1.05);

    cbLastSet->Add(edE31_Au4X,   &IniConfig.dTrayXScale[eAuto4],                ECDouble,   "In/Out Arm", "dTrayXScaleAuto4",                   bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);
    cbLastSet->Add(edE31_Au4Y,   &IniConfig.dTrayYScale[eAuto4],                ECDouble,   "In/Out Arm", "dTrayYScaleAuto4",                   bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);

    cbLastSet->Add(edE31_Au5X,   &IniConfig.dTrayXScale[eAuto5],                ECDouble,   "In/Out Arm", "dTrayXScaleAuto5",                   bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);
    cbLastSet->Add(edE31_Au5Y,   &IniConfig.dTrayYScale[eAuto5],                ECDouble,   "In/Out Arm", "dTrayYScaleAuto5",                   bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);

    cbLastSet->Add(edE31_Au6X,   &IniConfig.dTrayXScale[eAuto6],                ECDouble,   "In/Out Arm", "dTrayXScaleAuto6",                   bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);
    cbLastSet->Add(edE31_Au6Y,   &IniConfig.dTrayYScale[eAuto6],                ECDouble,   "In/Out Arm", "dTrayYScaleAuto6",                   bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);

    cbLastSet->Add(edE31_Fi1X,   &IniConfig.dTrayXScale[eFix1],                 ECDouble,   "In/Out Arm", "dTrayXScaleFix1",                    bShow, bEnable, bReadFromFile, LastSet.fTrayXScale[3], true, 0.95, 1.05);
    cbLastSet->Add(edE31_Fi1Y,   &IniConfig.dTrayYScale[eFix1],                 ECDouble,   "In/Out Arm", "dTrayYScaleFix1",                    bShow, bEnable, bReadFromFile, LastSet.fTrayYScale[3], true, 0.95, 1.05);

    cbLastSet->Add(edE31_Fi2X,   &IniConfig.dTrayXScale[eFix2],                 ECDouble,   "In/Out Arm", "dTrayXScaleFix2",                    bShow, bEnable, bReadFromFile, LastSet.fTrayXScale[4], true, 0.95, 1.05);
    cbLastSet->Add(edE31_Fi2Y,   &IniConfig.dTrayYScale[eFix2],                 ECDouble,   "In/Out Arm", "dTrayYScaleFix2",                    bShow, bEnable, bReadFromFile, LastSet.fTrayYScale[4], true, 0.95, 1.05);

    cbLastSet->Add(edE31_Fi3X,   &IniConfig.dTrayXScale[eFix3],                 ECDouble,   "In/Out Arm", "dTrayXScaleFix3",                    bShow, bEnable, bReadFromFile, LastSet.fTrayXScale[5], true, 0.95, 1.05);
    cbLastSet->Add(edE31_Fi3Y,   &IniConfig.dTrayYScale[eFix3],                 ECDouble,   "In/Out Arm", "dTrayYScaleFix3",                    bShow, bEnable, bReadFromFile, LastSet.fTrayYScale[5], true, 0.95, 1.05);

    cbLastSet->Add(edE31_Fi4X,   &IniConfig.dTrayXScale[eFix4],                 ECDouble,   "In/Out Arm", "dTrayXScaleFix4",                    bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);
    cbLastSet->Add(edE31_Fi4Y,   &IniConfig.dTrayYScale[eFix4],                 ECDouble,   "In/Out Arm", "dTrayYScaleFix4",                    bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);

    cbLastSet->Add(edE31_Fi5X,   &IniConfig.dTrayXScale[eFix5],                 ECDouble,   "In/Out Arm", "dTrayXScaleFix5",                    bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);
    cbLastSet->Add(edE31_Fi5Y,   &IniConfig.dTrayYScale[eFix5],                 ECDouble,   "In/Out Arm", "dTrayYScaleFix5",                    bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);

    cbLastSet->Add(edE31_Fi6X,   &IniConfig.dTrayXScale[eFix6],                 ECDouble,   "In/Out Arm", "dTrayXScaleFix6",                    bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);
    cbLastSet->Add(edE31_Fi6Y,   &IniConfig.dTrayYScale[eFix6],                 ECDouble,   "In/Out Arm", "dTrayYScaleFix6",                    bShow, bEnable, bReadFromFile, 1.0, true, 0.95, 1.05);

    if(CosFunction.bHotModeUseDiffScale)            //JerryYang 20250120 : add
    {
        cbLastSet->Add(cbE30_1,  &IniConfig.bE30_1InArmUseDifferentScale_Hot,     ECBool,   "In/Out Arm", "bE30_1InArmUseDifferentScale_Hot",     bShow, bEnable, bReadFromFile, 0);
        cbLastSet->Add(cbE32_1,  &IniConfig.bE32_1ShuttleUseDifferentScale_Hot,   ECBool,   "In/Out Arm", "bE32_1ShuttleUseDifferentScale_Hot",   bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        cbLastSet->Add(cbE30_1,  &IniConfig.bE30_1InArmUseDifferentScale_Hot,     ECBool,   "In/Out Arm", "bE30_1InArmUseDifferentScale_Hot",     bShow, bDisable, bFixedValue, 0);
        cbLastSet->Add(cbE32_1,  &IniConfig.bE32_1ShuttleUseDifferentScale_Hot,   ECBool,   "In/Out Arm", "bE32_1ShuttleUseDifferentScale_Hot",   bShow, bDisable, bFixedValue, 0);
    }

    elConfig->Add(cbE33,        &IniConfig.bE33InOutArmZOffsetSameOne,          ECBool,     "In/Out Arm", "bE33InOutArmZOffsetSameOne",         bShow, bEnable, bReadFromFile, LastSet.bInOutArmZOffsetSameOne?"1":"0");            //jou 2010-06-21
    if(CUSTOMER_CODE==CC_KYEC_LEE)
        elConfig->Add(cbE34,    &IniConfig.bE34InOutArmPitchZOffsetSameOne,     ECBool,     "In/Out Arm", "bE34InOutArmPitchZOffsetSameOne",    bShow, bDisable, bFixedValue, 0);         //Ifor 20160920 KYEC 客戶 喬智要求反灰不可修改，強制設定不使用E34   功能(pick/release offset 要獨立)
    else
        elConfig->Add(cbE34,    &IniConfig.bE34InOutArmPitchZOffsetSameOne,     ECBool,     "In/Out Arm", "bE34InOutArmPitchZOffsetSameOne",    bShow, bEnable, bReadFromFile, LastSet.bInOutArmPitchZOffsetSameOne?"1":"0"); //jou 2010-12-20 Pitch ,& Z 縮減為一個

    if(IniConfig.bEnableInOutArmPlaceSkipSuckDetect)
        elConfig->Add(cbE35,    &IniConfig.bInOutArmPlaceSkipSuckDetect,        ECBool,     "In/Out Arm", "bInOutArmPlaceSkipSuckDetect",       bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbE35,    &IniConfig.bInOutArmPlaceSkipSuckDetect,        ECBool,     "In/Out Arm", "bInOutArmPlaceSkipSuckDetect",       bNoShow, bEnable, bFixedValue, 0);

    elConfig->Add(edE36,        &IniConfig.iInArm60mmOffset,                    ECInteger,  "In/Out Arm", "iInArm60mmOffset",                   bShow, bEnable, bReadFromFile,  0,     false,  100,    -100);           //jou 2011-08-05 In Arm Y Pitch 60mm Offset
    elConfig->Add(edE37,        &IniConfig.iOutArm60mmOffset,                   ECInteger,  "In/Out Arm", "iOutArm60mmOffset",                  bShow, bEnable, bReadFromFile,  3,     false,  100,    -100);           //jou 2011-08-05 Out Arm Y Pitch 60mm Offset

    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        elConfig->Add(cbE38,    &IniConfig.bE38CheckHotPlateWhileInitialStart,  ECBool,         "In/Out Arm", "CheckHotPlateWhileInitialStart", bShow, bDisable, bFixedValue, 0);                       //Ifor 20170417 (wei) KYEC 客戶 喬智要求反灰不可修改，強制設定不使用E38   功能(Initial Start Check HotPlate)
        elConfig->Add(cbE39,&IniConfig.bE39CheckHotPlateAfterCleanOutAndBeforeTrayFeed, ECBool, "In/Out Arm", "CheckHotPlateAfterCleanOutAndBeforeTrayFeed", bShow, bDisable, bFixedValue, 0);          //Ifor 20170417 (wei) KYEC 客戶 喬智要求反灰不可修改，強制設定不使用E39   功能(Clean Out Tray Feed之前Check HotPlate)
        elConfig->Add(cbE39_1,  &IniConfig.bE39_1PutTheDevicesToErrorBin,       ECBool,         "In/Out Arm", "PutTheDevicesToErrorBin",        bShow, bDisable, bFixedValue, 0);                       //Ifor 20170417 (wei) KYEC 客戶 喬智要求反灰不可修改，強制設定不使用E39_1 功能(HotPlate Check 放至 Err bin)
    }
    else
    {
        elConfig->Add(cbE38,    &IniConfig.bE38CheckHotPlateWhileInitialStart,  ECBool,         "In/Out Arm", "CheckHotPlateWhileInitialStart", bShow, bEnable, bReadFromFile, 0);                      //ChungHung 20120206 Hotplate check
        elConfig->Add(cbE39,&IniConfig.bE39CheckHotPlateAfterCleanOutAndBeforeTrayFeed, ECBool, "In/Out Arm", "CheckHotPlateAfterCleanOutAndBeforeTrayFeed", bShow, bEnable, bReadFromFile, 0);         //ChungHung 20120206 Hotplate check
        elConfig->Add(cbE39_1,  &IniConfig.bE39_1PutTheDevicesToErrorBin,       ECBool,         "In/Out Arm", "PutTheDevicesToErrorBin",        bShow, bEnable, bReadFromFile, 0);
    }

    elConfig->Add(cbE40,        &IniConfig.bE40ClearAllHotICThenPickLoadIC,     ECBool,     "In/Out Arm", "bClearAllHotICThenPickLoadIC",       bShow, bEnable, bReadFromFile, 0);                      //ChungHung 20120514 Clear HotIC then Pick Load IC
    elConfig->Add(cbE41,        &IniConfig.bE41_35TrayPitchInOutSpeedSmall80Percent, ECBool,"In/Out Arm", "b35TrayPitchInOutSpeedSmall80Percent", bShow, bEnable, bReadFromFile, 0);                    //Steven 20120727 : Tray Pitch 大於35mm的話 In Out Arm需要小於80%
    if(CUSTOMER_CODE==CC_SCC)
        elConfig->Add(cbE42,    &IniConfig.bE42OutShuttleAlarmInArmServoOff,    ECBool,     "In/Out Arm", "bOutShuttleAlarmInArmServoOff",      bShow, bDisable, bFixedValue, 1);                       //ChungHung 20120723 Output Shuttle Alarm InArm Servo off
    else
        elConfig->Add(cbE42,    &IniConfig.bE42OutShuttleAlarmInArmServoOff,    ECBool,     "In/Out Arm", "bOutShuttleAlarmInArmServoOff",      bShow, bEnable, bReadFromFile, 0);                      //ChungHung 20120723 Output Shuttle Alarm InArm Servo off

    if(CosFunction.bAutoCleanUseHPSetByRecipe)                                  //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    {
        cbE43->Visible=false;
    }
    else if(IniConfig.bEnableAutoCleanFunction)                                 //Steven 20231121 : 有Auto Clean就可以選擇使用加熱盤
    {
        elConfig->Add(cbE43,    &IniConfig.bE43AutoCleanUseHotplate,            ECBool,     "In/Out Arm", "bAutoCleanUseHotplate",              bShow, bEnable, bReadFromFile, 0);                      //ChungHung 20131120 AutoClean use Hotplate1cbE38,&IniConfig.bE38CheckHotPlateWhileInitialStart}       //ChungHung 20120206 Hotplate check
    }
    else
    {
        elConfig->Add(cbE43,    &IniConfig.bE43AutoCleanUseHotplate,            ECBool,     "In/Out Arm", "bAutoCleanUseHotplate",              bNoShow, bDisable, bFixedValue, 0);                     //ChungHung 20140408 add only for SCK
    }

    if(CUSTOMER_CODE==CC_SJ_Semiconductor)
        elConfig->Add(chkE43_1, &IniConfig.bE43_1_AutoCleanCountSaveFolder,     ECBool,     "In/Out Arm", "bE43_1_AutoCleanCountSaveFolder",    bShow, bEnable, bReadFromFile, 1);                      //Steven 20250527 : Save auto clean count to folder
    else
        elConfig->Add(chkE43_1, &IniConfig.bE43_1_AutoCleanCountSaveFolder,     ECBool,     "In/Out Arm", "bE43_1_AutoCleanCountSaveFolder",    bShow, bEnable, bReadFromFile, 0);                      //Steven 20250527 : Save auto clean count to folder

    if(CosFunction.bLoseDeviceOutShuttleServoOff==false)                        //ChungHung 20140522 add OutShuttle lose devices can servo off
        elConfig->Add(cbE44, &IniConfig.bE44EnableLoseDeviceOutShuttleServoOff, ECBool,     "In/Out Arm", "bEnableLoseDeviceOutShuttleServoOff", bNoShow, bDisable, bFixedValue, 0);
    else
        elConfig->Add(cbE44, &IniConfig.bE44EnableLoseDeviceOutShuttleServoOff, ECBool,     "In/Out Arm", "bEnableLoseDeviceOutShuttleServoOff", bShow, bEnable, bReadFromFile, 0);

    if(Tri_Temp_Machine==1)                                                     //Wenqi 20240516 Modify offset by workfile
        elConfig->Add(cbE45,    &IniConfig.bE45_AllSetupFileUseOneFile,         ECBool,     "In/Out Arm", "bE45_AllSetupFileUseOneFile",        bShow, bEnable, bReadFromFile, 0);
    else if(CUSTOMER_CODE==CC_KYEC_LEE)
        elConfig->Add(cbE45,    &IniConfig.bE45_AllSetupFileUseOneFile,         ECBool,     "In/Out Arm", "bE45_AllSetupFileUseOneFile",        bShow, bDisable, bFixedValue, 1);                       //wei 20151104 複製機台的offset避免offset讀不到資料  //Ifor 20191001 : KYEC [E45] false => true
    else if(CUSTOMER_CODE==CC_ASE_CL)   //JerryYang 20250120 : ASECL SONG要求強制關閉
        elConfig->Add(cbE45,    &IniConfig.bE45_AllSetupFileUseOneFile,         ECBool,     "In/Out Arm", "bE45_AllSetupFileUseOneFile",        bShow, bDisable, bFixedValue, 0);                       //wei 20151104 複製機台的offset避免offset讀不到資料  //Ifor 20191001 : KYEC [E45] false => true
    else
        elConfig->Add(cbE45,    &IniConfig.bE45_AllSetupFileUseOneFile,         ECBool,     "In/Out Arm", "bE45_AllSetupFileUseOneFile",        bShow, bEnable, bReadFromFile, 0);                      //Steven 20140827 : 所有工作檔共用同一個Offset檔案

    elConfig->Add(cbE46,        &IniConfig.bE46_LoaderUse2Offset,               ECBool,     "In/Out Arm", "bE46_LoaderUse2Offset",              bShow, bEnable, bReadFromFile, 0);                      //Steven 20140827 : Loader可以分別調前後排的Offset
    elConfig->Add(cbE47,        &IniConfig.bE47_ShuttleUse4Offset,              ECBool,     "In/Out Arm", "bE47_ShuttleUse4Offset",             bShow, bEnable, bReadFromFile, 0);                      //Steven 20140827 : Shuttle可以分別調前後排的Offset
    elConfig->Add(cbE48,        &IniConfig.bE48_ShuttleUse4Offset_Autoclean,    ECBool,     "In/Out Arm", "bE48_ShuttleUse4Offset_Autoclean",   bShow, bEnable, bReadFromFile, 0);                      //20140923 wei :  Auto clean Shuttle可以分別調前後排的Offset
    elConfig->Add(cbE49,        &IniConfig.bE49_LoaderOnlyRetryAndCleanOut,     ECBool,     "In/Out Arm", "bE49_LoaderOnlyRetryAndCleanOut",    bShow, bEnable, bReadFromFile, 0);                      //Steven 20141105 : Loader吸取異常只能Retry與CleanOut

    if(CosFunction.bOutArmPickUpErrBySetup==true)                               //JerryYang 20220923 : E50改成選項
    {
        rgE50->Visible=false;
    }
    else
    {
        elConfig->Add(rgE50,    &IniConfig.iE50_OutArmPickUpErrorOption,        ECInteger,  "InOutArm", "iE50_OutArmPickUpErrorOption",         bShow, bEnable, bReadFromFile, 0);                      //Steven 20181226 : 測試Time out可以按的按鈕
    }

    elConfig->Add(cbE51,        &IniConfig.bE51_EnableInArmZADC,                ECBool,     "In/Out Arm", "bE51_EnableInArmZADC",               bShow, bEnable, bReadFromFile, 0);                      //Steven 20141212 : 使用固定的ADC
    elConfig->Add(edE51,        &IniConfig.iE51_EnableInArmZADC,                ECInteger,  "In/Out Arm", "iE51_EnableInArmZADC",               bShow, bEnable, bReadFromFile,  100,   false,  1,      100);            //Steven 20141212 : 使用固定的ADC
    elConfig->Add(cdE52,        &IniConfig.bE52_EnableOutArmZADC,               ECBool,     "In/Out Arm", "bE52_EnableOutArmZADC",              bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edE52,        &IniConfig.iE52_EnableOutArmZADC,               ECInteger,  "In/Out Arm", "iE52_EnableOutArmZADC",              bShow, bEnable, bReadFromFile,  100,   false,  1,      100);            //Steven 20141212 : 使用固定的ADC

    if(CosFunction.bLowYieldAutoSiteOff)
        elConfig->Add(cbE53,    &IniConfig.bE53LowYieldAutoClean,               ECBool,     "In/Out Arm", "bE53LowYieldAutoClean",              bNoShow, bDisable, bFixedValue, 0);                     //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
    else
        elConfig->Add(cbE53,    &IniConfig.bE53LowYieldAutoClean,               ECBool,     "In/Out Arm", "bE53LowYieldAutoClean",              bShow, bEnable, bReadFromFile, 0);                      //kevin 20160802 : CosFunction.bLowYieldAutoClean --> IniConfig.bE53LowYieldAutoClean

    elConfig->Add(cbE54,        &IniConfig.bE54CheckCloseSiteNoIC,              ECBool,     "In/Out Arm", "bE54CheckCloseSiteNoIC",             bShow, bEnable, bReadFromFile, 0);                      //Steven 20160922 : 因為OneCycle永遠先跑蝦頭1 檢查加熱盤錯誤功能與ByArmCloseSite衝突

    if(CosFunction.bUseFix3FullTray && FIX3_FULL_PLACE==Fix3K_Uninstall)        //Ifor 20161209 如果有安裝 Fix3 Full Place 強制關閉 Fix3 Full Tray 功能
        elConfig->Add(cbE55,    &IniConfig.bE55UseFix3FullTray,                 ECBool,     "In/Out Arm", "bE55UseFix3FullTray",                bShow, bEnable, bReadFromFile, 0);                      //Ifor 20161121 : add Use Fix3 Full Tray Function
    else
        elConfig->Add(cbE55,    &IniConfig.bE55UseFix3FullTray,                 ECBool,     "In/Out Arm", "bE55UseFix3FullTray",                bNoShow, bDisable, bFixedValue, 0);

    if(CosFunction.bAutoSkipNoDropError)
    {
        elConfig->Add(cbE56,    &IniConfig.bE56LoaderRetryAtSamePosition,       ECBool,     "In/Out Arm", "bE56LoaderRetryAtSamePosition",      bShow, bDisable, bFixedValue, 1);                       //Steven 20170828 (wei) : Loader吸取異常時要在同一個位置作Retry
    }
    else
    {
        elConfig->Add(cbE56,    &IniConfig.bE56LoaderRetryAtSamePosition,       ECBool,     "In/Out Arm", "bE56LoaderRetryAtSamePosition",      bShow, bEnable, bReadFromFile, 0);                      //Steven 20170828 (wei) : Loader吸取異常時要在同一個位置作Retry
    }

    elConfig->Add(cbE57,        &IniConfig.bE57HPCanUseAnotherVacuumDelay,      ECBool,     "In/Out Arm", "bE57HPCanUseAnotherVacuumDelay",     bShow, bEnable, bReadFromFile, 0);                      //Steven 20180125 (Jou) : 加熱盤的真空等待時間
    elConfig->Add(cbE58,        &IniConfig.bE57YPitchHome,                      ECBool,     "In/Out Arm", "bE57YPitchHome",                     bShow, bEnable, bReadFromFile, 0);                      //kevin 20180822 (Steven) : in out arm Y pitch 放完IC 歸 Y PITCH HOME
    elConfig->Add(cbE59,        &IniConfig.bE59GroupOffsetFile,                 ECBool,     "In/Out Arm", "bE59GroupOffsetFile",                bShow, bEnable, bReadFromFile, 0);                      //Steven 20190327 : Offset file使用中括號做群組
    if(CosFunction.bAutoSkipNoDropError)
    {
        elConfig->Add(cbE60,    &IniConfig.bE60PickLoaderDropAutoSkip,          ECBool,     "In/Out Arm", "bE60PickLoaderDropAutoSkip",         bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbE60,    &IniConfig.bE60PickLoaderDropAutoSkip,          ECBool,     "In/Out Arm", "bE60PickLoaderDropAutoSkip",         bNoShow, bDisable, bFixedValue, 0);
    }
    elConfig->Add(cbE61,        &IniConfig.bE61InArmStandbyPosOnLoader,         ECBool,     "In/Out Arm", "bE61InArmStandbyPosOnLoader",        bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(cbE62,        &IniConfig.bE62TryPickLastRow,                  ECBool,     "In/Out Arm", "bE62TryPickLastRow",                 bShow, bEnable, bReadFromFile, 0);                      //JerryYang 20200422 Auto skip次數到達後, 自動再去最後一排吸吸看
    elConfig->Add(cbE63,        &IniConfig.bE63RetryPickLoader,                 ECBool,     "In/Out Arm", "bE63RetryPickLoader",                bShow, bEnable, bReadFromFile, 0);
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20210324 add:KYEC 暫不使用等待後續討論結束
    {
        elConfig->Add(cbE64,    &IniConfig.bE64_50TrayPitchInOutSpeedSmall50Percent, ECBool, "In/Out Arm", "bE64_50TrayPitchInOutSpeedSmall50Percent", bShow, bDisable, bFixedValue, 1);     //Ifor 20201224 add: Tray Pitch 大於50mm 或 Tray  X-Division=1, In Out Arm需要小於50%
        elConfig->Add(chkE65,   &IniConfig.bE65_ClearTrayDataWhenOutArmDestoryErr,   ECBool, "In/Out Arm", "bE65_ClearTrayDataWhenOutArmDestoryErr", bShow, bDisable, bFixedValue, 0);                  //Steven 20210316 : 掉料的時候, 清除Unloader tray上的資料
    }
    else
    {
        elConfig->Add(cbE64,    &IniConfig.bE64_50TrayPitchInOutSpeedSmall50Percent,    ECBool, "In/Out Arm",   "bE64_50TrayPitchInOutSpeedSmall50Percent", bShow, bEnable, bReadFromFile, 0);     //Ifor 20201224 add: Tray Pitch 大於50mm 或 Tray  X-Division=1, In Out Arm需要小於50%
        elConfig->Add(chkE65,   &IniConfig.bE65_ClearTrayDataWhenOutArmDestoryErr,      ECBool, "In/Out Arm",   "bE65_ClearTrayDataWhenOutArmDestoryErr", bShow, bEnable, bReadFromFile, 0);                  //Steven 20210316 : 掉料的時候, 清除Unloader tray上的資料
    }

    elConfig->Add(chkE66,       &IniConfig.bE66_LogHotPlateAction,              ECBool,     "In/Out Arm", "bE66_LogHotPlateAction",             bShow, bEnable, bReadFromFile, 0);                      //Steven 20211104 : 紀錄加熱盤的動作
    elConfig->Add(chkE67,       &IniConfig.bE67_LoadPickerrorMoveWaitpos,       ECBool,     "In/Out Arm", "bE67_LoadPickerrorMoveWaitpos",      bShow, bEnable, bReadFromFile, 0);                      //kevin 20220723  : Load Pick up error Move wait pos
    elConfig->Add(cbE68,        &IniConfig.bE68RecheckInOutArmICFallDown,       ECBool,     "In/Out Arm", "bE68RecheckInOutArmICFallDown",      bShow, bEnable, bReadFromFile, 0);                      //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
    elConfig->Add(edE68,        &IniConfig.iE68RecheckInOutArmICFallDown,       ECInteger,  "In/Out Arm", "iE68RecheckInOutArmICFallDown",      bShow, bEnable, bReadFromFile,  3,     false,  1,      100);              //Sam 20221006 : In/Out Arm IC 掉落狀態多檢查幾次再報警
    elConfig->Add(chkE69,       &IniConfig.bE69_PickupErrorPlacement,           ECBool,     "In/Out Arm", "bE69_PickupErrorPlacement",          (USE_InPlacement==eartUninstall)?bNoShow:bShow, (USE_InPlacement==eartUninstall)?bDisable:bEnable, bReadFromFile, 0);  //JimmyChiu 20220908 add Pickup Error Placement
    if(CosFunction.bUseTrayThickAdjustZHeight==true)                            //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE  && bEnable_KLT_Function==false)          //Ifor 20221220 add:KYEC要求強制開啟並反灰
        {
            elConfig->Add(chkE70,&IniConfig.bE70_UseTrayThickAdjustZHeight,     ECBool,     "Tray", "bE70_UseTrayThickAdjustZHeight" ,          bShow, bDisable, bFixedValue, 1);       //Ifor 20221220 add:新增選項開啟或關閉使用Tray 厚度 自動補償Z軸高度
        }
        else
        {
            elConfig->Add(chkE70,&IniConfig.bE70_UseTrayThickAdjustZHeight,     ECBool,     "Tray", "bE70_UseTrayThickAdjustZHeight" ,          bShow, bEnable, bReadFromFile, 0);      //Ifor 20221220 add:新增選項開啟或關閉使用Tray 厚度 自動補償Z軸高度
        }
    }
    else
    {
        elConfig->Add(chkE70,&IniConfig.bE70_UseTrayThickAdjustZHeight,         ECBool,     "Tray", "bE70_UseTrayThickAdjustZHeight" ,          bNoShow, bDisable, bFixedValue, 0);     //Ifor 20221220 add:新增選項開啟或關閉使用Tray 厚度 自動補償Z軸高度
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE && bEnable_KLT_Function==false)               //Ifor 20221220 add:KYEC要求強制開啟並反灰
    {
        elConfig->Add(chkE71,   &IniConfig.bE71_10TrayPitchLockTrayAssign,      ECBool,     "Tray",  "bE71_10TrayPitchLockTrayAssign",          bShow, bDisable, bFixedValue, 1);      //Ifor 20221220 add:新增Tray Pitch 小於10mm 強制鎖定Loader To Empty Color To Auto
    }
    else
    {
        elConfig->Add(chkE71,   &IniConfig.bE71_10TrayPitchLockTrayAssign,      ECBool,     "Tray",  "bE71_10TrayPitchLockTrayAssign",          bShow, bEnable, bReadFromFile, 0);      //Ifor 20221220 add:新增Tray Pitch 小於10mm 強制鎖定Loader To Empty Color To Auto
    }
    elConfig->Add(chkE72,       &IniConfig.bE72_InarmPickICNeedWaitSH,          ECBool,     "In/Out Arm", "bE72_InarmPickICNeedWaitSH",         bShow, bEnable, bReadFromFile, 0);      //KenHsieh 20230614 : Inarm pick IC from tray需待SH到位且為可取放料狀態

    elConfig->Add(chkE73,       &IniConfig.bE73_InOutZStepMotorLossCheck,       ECBool,     "Tray",  "bE73_InOutZStepMotorLossCheck",           bShow, bEnable, bReadFromFile, 0);      //Ifor 20221220 add:新增Tray Pitch 小於10mm 強制鎖定Loader To Empty Color To Auto
    elConfig->Add(edE73,        &IniConfig.iE73StepMotorCheckCnt,               ECInteger,  "In/Out Arm",           "iE73StepMotorCheckCnt",            bShow, bEnable, bReadFromFile,  5,   false,  3,      10);               //JerryYang 20231113 : add

    #ifdef SOFT_SIMULTE
    elConfig->Add(chkE74,       &IniConfig.bE74_InspectArmPosition,             ECBool,     "Tray",  "bE74_InspectArmPosition",                 bShow, bEnable, bReadFromFile, 0);      //Jimmychiu 20240408 : debug for inarm position
    #else
    elConfig->Add(chkE74,       &IniConfig.bE74_InspectArmPosition,             ECBool,     "Tray",  "bE74_InspectArmPosition",                 bNoShow, bDisable, bFixedValue, 0);      //Jimmychiu 20240408 : debug for inarm position
    #endif
    if(CosFunction.bOneByOneWhenPickErrAtLoader==true)
    {
        elConfig->Add(cbkE78,       &IniConfig.bE78OneByOneWhenPickErrAtLoader,     ECBool,     "Tray",  "bE78OneByOneWhenPickErrAtLoader",         bShow, bEnable, bReadFromFile, 1);  //Jimmychiu 20250924 : Suck one by one when a pickup error occurs at the loader.
    }

    if(CosFunction.bFillTheTrayAfterOutArmPlace==true)                          //Jimmychiu 20240726 : Fill The Tray After Out Arm Place
    {
        elConfig->Add(cbE85_Enable,     &IniConfig.bE85_FillTray_Enable,        ECBool,     "Tray",  "bE85_FillTray_Enable",                    bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbE85_Auto1,      &IniConfig.bE85_FillTray_Auto1,         ECBool,     "Tray",  "bE85_FillTray_Auto1",                     bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbE85_Auto3,      &IniConfig.bE85_FillTray_Auto3,         ECBool,     "Tray",  "bE85_FillTray_Auto3",                     bShow, bEnable, bReadFromFile, 0);
    }
    gbE85->Visible=CosFunction.bFillTheTrayAfterOutArmPlace;
    if(CUSTOMER_CODE==CC_ASE_CL)        //JerryYang 20250120 : add
    {
        elConfig->Add(cbE86,       &IniConfig.bE86_InArmPickErrOnLoaderOnlyCanSKIP, ECBool,   "In/Out Arm",  "bE72_InArmPickErrOnLoaderOnlyCanSKIP", bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbE87,       &IniConfig.bE87PickupErrorAtLoaderNeedOpenDoor,  ECBool,   "In/Out Arm",  "bE73PickupErrorAtLoaderNeedOpenDoor",  bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbE88,       &IniConfig.bE88_InArmHeightFollow7000,           ECBool,   "In/Out Arm",  "bE75_InArmHeightFollow7000",           bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbE89,       &IniConfig.bE89_InArmHotPlatePitchUseScale,      ECBool,   "In/Out Arm",  "bE76_InArmHotPlatePitchUseScale",      bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbE86,       &IniConfig.bE86_InArmPickErrOnLoaderOnlyCanSKIP, ECBool,   "In/Out Arm",  "bE72_InArmPickErrOnLoaderOnlyCanSKIP", bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbE87,       &IniConfig.bE87PickupErrorAtLoaderNeedOpenDoor,  ECBool,   "In/Out Arm",   "bE73PickupErrorAtLoaderNeedOpenDoor", bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbE88,       &IniConfig.bE88_InArmHeightFollow7000,           ECBool,   "In/Out Arm",  "bE75_InArmHeightFollow7000",           bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbE89,       &IniConfig.bE89_InArmHotPlatePitchUseScale,      ECBool,   "In/Out Arm",  "bE76_InArmHotPlatePitchUseScale",      bNoShow, bDisable, bFixedValue, 0);
    }
    //Steven 20260428 [E90]: Fix Tray Full retreat OutArm Y by -15000 for back-door tray pickup; ASE_SG forced ON
    if(CUSTOMER_CODE==CC_ASE_SG)
        elConfig->Add(cbE90,   &IniConfig.bE90_OutArmFixFullExtraY, ECBool, "In/Out Arm", "bE90_OutArmFixFullExtraY", bNoShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbE90,   &IniConfig.bE90_OutArmFixFullExtraY, ECBool, "In/Out Arm", "bE90_OutArmFixFullExtraY", bShow,   bEnable,  bReadFromFile, 0);
}

// AI(W906-FW-CFG-W2) 20260825: golden cConfiguration.cpp:2113-2328, transcribed
// VERBATIM (cp950 -> UTF-8). Registration table only -- the port's
// HTEditList::Add signature is argument-for-argument golden's, so no
// adaptation was needed and none was invented. See FW-CFG-W1's banner in
// forms/fConfiguration.h for why this family is the wave's point.
void TfConfiguration::InitConfigEdtList_ItemF()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;

    elConfig->Add(cbF01,        &IniConfig.bF01ShakeShuttleWhenJam,             ECBool,     "Shuttle", "bF01ShakeShuttleWhenJam",               bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edF01,        &IniConfig.iF01ShuttleShakeSpeed,               ECInteger,  "Shuttle", "iF01ShuttleShakeSpeed",                 bShow, bEnable, bReadFromFile, LastSet.iShuttleShakeSpeed,    false,  1,      100);//Steven 20120802 : Shuttle搖搖速度   //Steven 20160531 (wei) : 50 --> 1
    elConfig->Add(cbF03,        &IniConfig.bF03OutputShuttleSkipICMiss,         ECBool,     "Shuttle", "bF03OutputShuttleSkipICMiss",           bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edF05,        &IniConfig.iF05ShuttlePurgeCount,               ECInteger,  "Shuttle", "iF05ShuttlePurgeCount",                 bShow, bEnable, bReadFromFile, LastSet.iShuttlePurgeCount,    false,  1,      100);
    elConfig->Add(cbF05,        &IniConfig.bF05EnableShtPurgeFunction,          ECBool,     "Shuttle", "bF05EnableShtPurgeFunction",            bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(cbF06,        &IniConfig.bF06InitialICCheck,                  ECBool,     "Shuttle", "bF06InitialICCheck",                    bShow, bEnable, bReadFromFile, 0);

//    if(ENABLE_OUT_SHUTTLEY_LATCH==false)                                        //Steven 20180329 (Jou) : 使用Y-Latch偵測置偏
//        elConfig->Add(rgF07,    &IniConfig.iF07OutShuttleSensorMode,            ECInteger,  "Shuttle", "iF07OutShuttleSensorMode",              bShow, bDisable, bFixedValue, 0);
//    else
        elConfig->Add(rgF07,    &IniConfig.iF07OutShuttleSensorMode,            ECInteger,  "Shuttle", "iF07OutShuttleSensorMode",              bShow, bEnable, bReadFromFile, LastSet.iOutShuttleSensorMode);
    elConfig->Add(cbF09,        &IniConfig.bF09CheckICWhichFirstTimeLoad,       ECBool,     "Shuttle", "bF09CheckICWhichFirstTimeLoad",         bShow, bEnable, bReadFromFile, 0);                  //Steven 20090907 [F09]
    if(ENABLE_OUT_SHUTTLE_SENEOR==false)
        elConfig->Add(cbF11,    &IniConfig.bF11OutShtUseFrontRearSensor,        ECBool,     "Shuttle", "bF11OutShtUseFrontRearSensor",          bNoShow, bDisable, bFixedValue, 0);                 //JerryYang 20190401 out shuttle前後對照，硬體選項沒有勾就不要顯示
    else if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)
        elConfig->Add(cbF11,    &IniConfig.bF11OutShtUseFrontRearSensor,        ECBool,     "Shuttle", "bF11OutShtUseFrontRearSensor",          bShow, bDisable, bFixedValue, 1);                   //jou 2013-04-09 KYEC 邱凱偉要求強制打開
    else
        elConfig->Add(cbF11,    &IniConfig.bF11OutShtUseFrontRearSensor,        ECBool,     "Shuttle", "bF11OutShtUseFrontRearSensor",          bShow, bEnable, bReadFromFile, 0);                  //jou 2010-09-09 [F11] Out Shuttle 使用前後對照Sensor檢查殘留IC

    if(IniConfig.bHaveRotateShuttle && CUSTOMER_CODE!=CC_ASE_KaohSiung)
    {
        labF12->Visible=true;
        elConfig->Add(cbF12,    &IniConfig.bRotateShNeedCheck,                  ECBool,     "Rotate Shuttle", "bRotateShNeedCheck",             bShow, bEnable, bReadFromFile, 0);                  //Steven 20110802 : 轉轉蝦頭要檢查有沒有轉頭
        elConfig->Add(EdF12,    &IniConfig.dRoShCheckDelayTime,                 ECDouble,   "Rotate Shuttle", "dRoShCheckDelayTime",            bShow, bEnable, bReadFromFile,  0.01,  false, 1.0,     0.01);            //Steven 20110802 : 轉轉蝦頭要檢查有沒有轉頭的延遲時間
    }
    else
    {
        labF12->Visible=false;
        elConfig->Add(cbF12,    &IniConfig.bRotateShNeedCheck,                  ECBool,     "Rotate Shuttle", "bRotateShNeedCheck",             bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(EdF12,    &IniConfig.dRoShCheckDelayTime,                 ECDouble,   "Rotate Shuttle", "dRoShCheckDelayTime",            bNoShow, bDisable, bFixedValue, 0.01,  false, 1.0,     0.01);
    }

    if(IniConfig.bHaveRotateShuttle)                                            //Steven 20110819--------
    {
        gbF13_RotateShtSpeed->Visible=(CUSTOMER_CODE!=CC_ASE_KaohSiung);
        elConfig->Add(edF13_ADC,&IniConfig.iRotateADC,                          ECInteger,  "Configuration", "Rotate ADC",                      bShow, bEnable, bReadFromFile,  10,    false, 1,       100);            //Steven 20101018 : 轉轉蝦頭的加減速
        elConfig->Add(edF13_Ini,&IniConfig.iInitSpeed,                          ECInteger,  "Configuration", "RotateInitSpeed",                 bShow, bEnable, bReadFromFile,  50,    false, 1,       100);            //kevin 20110531 旋轉SHUTTLE 鎖最高速度
        elConfig->Add(edF13_Jog,&IniConfig.iPJogHighSpeed,                      ECInteger,  "Configuration", "RotateJogHighSpeed",              bShow, bEnable, bReadFromFile,  1000,  false, 1,       5000);           //kevin 20110531 旋轉SHUTTLE 鎖最高速度
    }
    else
    {
        gbF13_RotateShtSpeed->Visible=false;
        elConfig->Add(edF13_ADC,&IniConfig.iRotateADC,                          ECInteger,  "Configuration", "Rotate ADC",                      bNoShow, bDisable, bFixedValue, 10,    false, 1,       100);            //Steven 20101018 : 轉轉蝦頭的加減速
        elConfig->Add(edF13_Ini,&IniConfig.iInitSpeed,                          ECInteger,  "Configuration", "RotateInitSpeed",                 bNoShow, bDisable, bFixedValue, 50,    false, 1,       100);            //kevin 20110531 旋轉SHUTTLE 鎖最高速度
        elConfig->Add(edF13_Jog,&IniConfig.iPJogHighSpeed,                      ECInteger,  "Configuration", "RotateJogHighSpeed",              bNoShow, bDisable, bFixedValue, 1000,  false, 1,       5000);           //kevin 20110531 旋轉SHUTTLE 鎖最高速度
    }

    elConfig->Add(cbF14,        &IniConfig.bF14KnockShuttle,                    ECBool,     "Shuttle", "bKnockShuttle",                         bShow, bEnable, bReadFromFile,  0);                                     //Steven 20120801 : Shuttle敲敲
    elConfig->Add(cbF14_1,      &IniConfig.bF14_1KnockShuttleFirst,             ECBool,     "Shuttle", "bKnockShuttleFirst",                    bShow, bEnable, bReadFromFile, 0);                                      //jou 2015-12-09 SCS 要求 Shuttle 每次入料前 敲擊
    elConfig->Add(edF14,        &IniConfig.dF14KnockShuttleInterval,            ECDouble,   "Shuttle", "dKnockShuttleInterval",                 bShow, bEnable, bReadFromFile,  0.1,   false, 1.0,     0.01);           //Steven 20120801 : Shuttle敲敲
    elConfig->Add(edF14_No,     &IniConfig.iF14KnockShuttleNo,                  ECInteger,  "Shuttle", "iKnockShuttleNo",                       bShow, bEnable, bReadFromFile,  3,     false, 3,       100);            //wei 20121206
    elConfig->Add(edF14_1,      &IniConfig.dF14KnockShuttleIntervalFirst,       ECDouble,   "Shuttle", "dF14KnockShuttleIntervalFirst",         bShow, bEnable, bReadFromFile,  0.1,   false, 1.0,     0.01);           //jou 2015-12-09 SCS 要求 Shuttle 每次入料前 敲擊
    elConfig->Add(edF14_1_No,   &IniConfig.iF14KnockShuttleNoFirst,             ECInteger,  "Shuttle", "iF14KnockShuttleNoFirst",               bShow, bEnable, bReadFromFile,  3,     false, 1,       100);            //jou 2015-12-09 SCS 要求 Shuttle 每次入料前 敲擊

    elConfig->Add(cbF15,        &IniConfig.bF15OutShuttleLoseICNeedPWD,         ECBool,     "Shuttle", "OutShuttleLoseICNeedPWD",               bShow, bEnable, bReadFromFile, 0);                  //ChungHung 20120912 Amkor 需求Shuttle lose ic need password
    elConfig->Add(cbF16,        &IniConfig.bF16CheckShuttleSensorBroken,        ECBool,     "Shuttle", "CheckShuttleSensorBroken",              bShow, bEnable, bReadFromFile, 0);                  //2014-01-06    Dell    for TSMC 確認shuttle 有沒有斷線

    elConfig->Add(cbF17,        &IniConfig.bF17Sht1First,                       ECBool,     "Shuttle", "bF17Sht1First",                         bShow, bEnable, bReadFromFile, 0);                  //Steven 20160926 : One cycle後要先跑蝦頭一

    if(CosFunction.bF18F22InshuttleDetectSaveByRecipe)                          //JimmyChiu 20220601 : F18/F22 in shuttle A7 sensor進出檢測功能跟隨recipe是否啟用
        elConfig_byRecipe->Add(cbF18,    &IniConfig.bF18InshuttleDetect,        ECBool,     "Shuttle", "bInshuttleDetect",                      bShow, bEnable, bReadFromFile, 0);                  //kevin 20141213 20140206 input SHUTTLE 第9顆sensor 進入偵測是否有ic
    else if(IniConfig.bSPILFunction==true)
        elConfig->Add(cbF18,    &IniConfig.bF18InshuttleDetect,                 ECBool,     "Shuttle", "bInshuttleDetect",                      bNoShow, bDisable, bFixedValue, 0);                 //kevin 20141213 20140206 input SHUTTLE 第9顆sensor 進入偵測是否有ic
    else
        elConfig->Add(cbF18,    &IniConfig.bF18InshuttleDetect,                 ECBool,     "Shuttle", "bInshuttleDetect",                      bShow, bEnable, bReadFromFile, 0);                  //kevin 20141213 20140206 input SHUTTLE 第9顆sensor 進入偵測是否有ic

    if(CosFunction.bLockF19AlwaysOn)                                            //Steven 20150709 : Out shuttle lose IC要做Piggyback
        elConfig->Add(cbF19,    &IniConfig.bF19OutShuttleLoseICNeedPiggyback,   ECBool,     "Shuttle", "bOutShuttleLoseICNeedPiggyback",        bShow, bDisable, bFixedValue, 1);
    else
        elConfig->Add(cbF19,    &IniConfig.bF19OutShuttleLoseICNeedPiggyback,   ECBool,     "Shuttle", "bOutShuttleLoseICNeedPiggyback",        bShow, bEnable, bReadFromFile, 0);

    if(CUSTOMER_CODE==CC_PTI ||                                                 //Alick 20160815 add for 力成 Input Shuttle 檢測IC是否放到外圍，避免壓壞
       CUSTOMER_CODE==CC_SCC)                                                   //JerryYang 20210107 : Add SCC
    {
        cbF20Label2->Visible=true;
        elConfig->Add(cbF20,    &IniConfig.bF20InShuttleProminentDetect,        ECBool,     "Shuttle", "bF20InShuttleProminentDetect",          bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        cbF20Label2->Visible=false;
        elConfig->Add(cbF20,    &IniConfig.bF20InShuttleProminentDetect,        ECBool,     "Shuttle", "bF20InShuttleProminentDetect",          bNoShow, bDisable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_KYEC_XILINX)
        elConfig->Add(cbF21,    &IniConfig.bF21InOutArmZMotorPrivate,           ECBool,     "Shuttle", "bF21InOutArmZMotorPrivate",             bNoShow, bDisable, bFixedValue, 0);                 //kevin 20161005 shuttle移動時判斷 In Out Arm Z軸在Home sensor  //kevin 20180525 Ifor 20180228 (Steven) KYEC暫時關閉避免16 Site 放料異常(蝦頭2 Kit1 吸嘴上下跳動)
    else if(CUSTOMER_CODE==CC_KYEC_LEE)                                      //Eastsun 20260515 F022: D6 Z home sensor fix
        elConfig->Add(cbF21,    &IniConfig.bF21InOutArmZMotorPrivate,           ECBool,     "Shuttle", "bF21InOutArmZMotorPrivate",             bShow, bDisable, bFixedValue, 1);                 //kevin 20161005 shuttle移動時判斷 In Out Arm Z軸在Home sensor  //kevin 20180525 Ifor 20180228 (Steven) KYEC暫時關閉避免16 Site 放料異常(蝦頭2 Kit1 吸嘴上下跳動)
    else
        elConfig->Add(cbF21,    &IniConfig.bF21InOutArmZMotorPrivate,           ECBool,     "Shuttle", "bF21InOutArmZMotorPrivate",             bShow, bEnable, bReadFromFile, 0);

    if(CosFunction.bF18F22InshuttleDetectSaveByRecipe)                          //JimmyChiu 20220601 : F18/F22 in shuttle A7 sensor進出檢測功能跟隨recipe是否啟用
    {
        elConfig_byRecipe->Add(cbF22, &IniConfig.bF22InShuttleDetectOutNoIC,    ECBool,     "Shuttle",  "bF22InshuttleDetectOutNoIC",           bShow, bEnable, bReadFromFile, 0);                  //kevin 20161108 shuttle 出來撿測有無IC殘留
    }
    else
    {
        elConfig->Add(cbF22,    &IniConfig.bF22InShuttleDetectOutNoIC,          ECBool,     "Shuttle",  "bF22InshuttleDetectOutNoIC",           bShow, bEnable, bReadFromFile, 0);                  //kevin 20161108 shuttle 出來撿測有無IC殘留
    }

    if(CosFunction.bKnockerSetBySetupFile==false)                               //JerryYang 20200504 修正開啟程式時振動功能失效問題
    {
        if(ShuttleVibration_INSTALL)                                            //JerryYang 20171006 (wei) Shuttle 震動馬達
        {
            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20220923 : SPIL強制啟用shuttle震動馬達功能
            {
                elConfig->Add(cbF23,    &IniConfig.bF23ShuttleVibration,        ECBool,     "Shuttle", "bF23ShuttleVibration",                  bShow, bDisable, bFixedValue, 1);
            }
            else
            {
                elConfig->Add(cbF23,    &IniConfig.bF23ShuttleVibration,        ECBool,     "Shuttle", "bF23ShuttleVibration",                  bShow, bEnable, bReadFromFile, 0);
            }

            if(CUSTOMER_CODE==CC_TERAPOWER ||                                   //Sam 20210520 : TPW 修改震動時間 Range
               CUSTOMER_CODE==CC_AMKOR_Korea)
                elConfig->Add(edF23,    &IniConfig.iF23ShuttleVibrationTime,    ECInteger,  "Shuttle", "iF23ShuttleVibrationTime",              bShow, bEnable, bReadFromFile,  20,   false, 1,      100);
            else
                elConfig->Add(edF23,    &IniConfig.iF23ShuttleVibrationTime,    ECInteger,  "Shuttle", "iF23ShuttleVibrationTime",              bShow, bEnable, bReadFromFile,  20,   false, 20,     100);
            elConfig->Add(edtF23_1,     &IniConfig.iF23ShuttleVibrationCount,   ECInteger,  "Shuttle", "iF23ShuttleVibrationCount",             bShow, bEnable, bReadFromFile,  1,    false, 1,      3);    //JerryYang 20191113 shuttle振動馬達可設定次數
        }
        else
        {
            elConfig->Add(cbF23,    &IniConfig.bF23ShuttleVibration,            ECBool,     "Shuttle", "bF23ShuttleVibration",                  bNoShow, bDisable, bFixedValue, 0);
            edF23->Visible=false;
            edtF23_1->Visible=false;
        }
    }
    else
    {
        cbF23->Visible=false;
        edF23->Visible=false;
        edtF23_1->Visible=false;
    }

    elConfig->Add(cbF24,        &IniConfig.bF24OutShuttleLoseIcOpenIndexDoor,   ECBool,     "Shuttle", "OutShuttleLoseIcOpenIndexDoor",         bShow, bEnable, bReadFromFile, 0);                              //kevin 20180725 (wei) add out shuttle lose IC push Z1 open index door
    elConfig->Add(cbF25,        &IniConfig.bF25VibrateForOutShuttle,            ECBool,     "Shuttle", "bF25VibrateForOutShuttle",              bShow, bEnable, bReadFromFile, 0);                              //kevin 20180725 (wei) add out shuttle lose IC push Z1 open index door
    elConfig->Add(edF25,        &IniConfig.iF25VibrateTime,                     ECInteger,  "Shuttle", "iF25VibrateTime",                       bShow, bEnable, bReadFromFile,  20,    false, 20,      100);    //JerryYang 20171205 (Steven) shuttle震動馬達功能
    elConfig->Add(rgF26,        &IniConfig.iF26OutShuttleJamSelectSkipOrRetry,  ECInteger,  "Shuttle", "iF25OutShuttleJamSelectSkipOrRetry",    bShow, bEnable, bReadFromFile,  0,    false, 2,      0);        //KaiChen 20181211 ：Out shuttle Jam Select Skip or Retry;

    if(CUSTOMER_CODE==CC_JCET)
        elConfig->Add(chkF27,   &IniConfig.bF27OutShtLoseICNeedToPick,          ECBool,     "Shuttle", "bF27OutShtLoseICNeedToPick",            bShow, bEnable, bReadFromFile, 0);                  //Steven 20220120 : 吳如春要求out shuttle lose IC時, out arm還是要下去吸料
    else
        elConfig->Add(chkF27,   &IniConfig.bF27OutShtLoseICNeedToPick,          ECBool,     "Shuttle", "bF27OutShtLoseICNeedToPick",            bNoShow, bDisable, bFixedValue, 0);

    elConfig->Add(cbF28,        &IniConfig.bF28IndexCheckShuttlePos,            ECBool,     "Shuttle", "bF26IndexCheckShuttlePos",              bShow, bEnable, bReadFromFile, 0);                  //kevin 20220512 add  Index check shuttle pos for Sensor

    if(ShuttleVibration_INSTALL)                                                //Sam 20210602 : 每次都要強制震動
        elConfig->Add(cbF29,    &IniConfig.bF29AlwaysVibrateOnShuttle,          ECBool,     "Shuttle", "bF26AlwaysVibrateOnShuttle",            bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbF29,    &IniConfig.bF29AlwaysVibrateOnShuttle,          ECBool,     "Shuttle", "bF26AlwaysVibrateOnShuttle",            bNoShow, bDisable, bFixedValue, 0);

    if(MachineTypeChoice==Type_HT9046_LS && SubMachineType==Type_HT9016C)       //Jimmychiu 20230628 Add TempPosShift for HT9016C
        elConfig->Add(cbF30,    &IniConfig.bF30InShuttleSensorFollow16site,     ECBool,     "Shuttle", "bF26InShuttleSensorFollow16site",       bShow, bEnable, bReadFromFile, 1);
    else
        elConfig->Add(cbF30,    &IniConfig.bF30InShuttleSensorFollow16site,     ECBool,     "Shuttle", "bF26InShuttleSensorFollow16site",       bShow, bEnable, bReadFromFile, 0);                  //JerryYang 20210426 : 2x4, 2x6 follow 2x8 shuttle sensor位置

    //Ztex 2023.04.19 Add HT-1032 TriTemp Function ==>
    elConfig->Add(cbF31,    &IniConfig.bF31_CheckShtMoveCnt,    ECBool,         "F_Shuttle", "bF31_CheckShtMoveCnt",                  bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edF31_1,  &IniConfig.iShtMoveCntSet[0],       ECInteger,      "F_Shuttle", "F31_1_iShuttleMotorMoveCountSet",       bShow, bEnable, bReadFromFile,  20,    false, 0,      1000000);
    elConfig->Add(edF31_2,  &IniConfig.iShtMoveCntSet[1],       ECInteger,      "F_Shuttle", "F31_2_iShuttleMotorMoveCountSet",       bShow, bEnable, bReadFromFile,  20,    false, 0,      1000000);
    elConfig->Add(edF31_3,  &IniConfig.iShtMoveCntNow[0],       ECInteger,      "F_Shuttle", "F31_3_iShuttleMotorMoveCountNow",       bShow, bEnable, bReadFromFile,  20,    false, 0,      1000000);
    elConfig->Add(edF31_4,  &IniConfig.iShtMoveCntNow[1],       ECInteger,      "F_Shuttle", "F31_4_iShuttleMotorMoveCountNow",       bShow, bEnable, bReadFromFile,  20,    false, 0,      1000000);
    elConfig->Add(edF31_5,  &IniConfig.iShtMoveCntHis[0],       ECInteger,      "F_Shuttle", "F31_5_iShuttleMotorMoveCountHistroy",   bShow, bEnable, bReadFromFile,  20,    false, 0,      1000000);
    elConfig->Add(edF31_6,  &IniConfig.iShtMoveCntHis[1],       ECInteger,      "F_Shuttle", "F31_6_iShuttleMotorMoveCountHistroy",   bShow, bEnable, bReadFromFile,  20,    false, 0,      1000000);
    //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==

    if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
      (SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus))   //KenHsieh 20230829 : add Check In Shuttle Sensor By pass
    {
        elConfig->Add(cbF32,    &IniConfig.bF32CheckInSHSenBypass,              ECBool,     "Shuttle",  "bF30CheckInSHSenBypass",               bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edF32,    &IniConfig.iF32InSHSenBypassValue,              ECInteger,  "Shuttle",  "iF30InSHSenBypassValue",               bShow, bEnable, bReadFromFile,  10,    false, 10,      4095);
    }
    else
    {
        elConfig->Add(cbF32,    &IniConfig.bF32CheckInSHSenBypass,              ECBool,     "Shuttle",  "bF30CheckInSHSenBypass",               bNoShow, bDisable, bFixedValue, 0);
        edF32->Visible=false;
    }

    if(IniConfig.bSPILFunction==true &&
       (BAR_CODE_INSTALL==ebctUseCCDMode ||                                     //JerryYang 20250220 : 2DID硬體順序檢查功能
       BAR_CODE_INSTALL==ebctInShtIntel ||
       BAR_CODE_INSTALL==ebctEtherNetCCD))
    {
        elConfig->Add(cbF33_Check2DHardware,    &IniConfig.bF33_Check2DHardware,ECBool,     "Shuttle",  "bF33_Check2DHardware",                 bShow, bEnable, bReadFromFile, 1);
    }
    else
    {
        elConfig->Add(cbF33_Check2DHardware,    &IniConfig.bF33_Check2DHardware,ECBool,     "Shuttle",  "bF33_Check2DHardware",                 bNoShow, bDisable, bFixedValue, 0);
    }

    if(IniConfig.bMaximFunction || CUSTOMER_CODE==CC_AnalogDevice_Phil)
    {
        elConfig->Add(cbF34,        &IniConfig.bF34OutShtPickErrSetErrBin,           ECBool,     "Shuttle",  "bF34OutShtPickErrSetErrBin",      bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbF35,        &IniConfig.bF35OutShtLoseNeedSetErrBin,          ECBool,     "Shuttle",  "bF35OutShtLoseNeedSetErrBin",     bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbF34,        &IniConfig.bF34OutShtPickErrSetErrBin,           ECBool,     "Shuttle",  "bF34OutShtPickErrSetErrBin",      bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbF35,        &IniConfig.bF35OutShtLoseNeedSetErrBin,          ECBool,     "Shuttle",  "bF35OutShtLoseNeedSetErrBin",     bNoShow, bDisable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_ASE_CL)        //JerryYang 20250120 : add
    {
        elConfig->Add(cbF36,   &IniConfig.bF36OutShtLoseICResetSetAllToErr,          ECBool,     "Shuttle",     "bF36OutShtLoseICResetSetAllToErr",    bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbF36,   &IniConfig.bF36OutShtLoseICResetSetAllToErr,          ECBool,      "Shuttle",     "bF36OutShtLoseICResetSetAllToErr",    bNoShow, bDisable, bFixedValue, 0);
    }
}

// AI(W906-FW-CFG-W2) 20260825: golden cConfiguration.cpp:2330-2419, transcribed
// VERBATIM (cp950 -> UTF-8). Registration table only -- the port's
// HTEditList::Add signature is argument-for-argument golden's, so no
// adaptation was needed and none was invented. See FW-CFG-W1's banner in
// forms/fConfiguration.h for why this family is the wave's point.
void TfConfiguration::InitConfigEdtList_ItemG()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;

    elConfig->Add(cbG01,        &IniConfig.bG01Show_TestRate,                   ECBool,     "Visible", "bG01Show_TestRate",                     bShow, bEnable, bReadFromFile, LastSet.bShow_TestRate?"1":"0"); //Steven 20090715
    elConfig->Add(cbG04,        &IniConfig.bG04ShowFailAlarmCount,              ECBool,     "Visible", "bG04ShowFailAlarmCount",                bShow, bEnable, bReadFromFile, LastSet.bShowFailAlarmCount?"1":"0");            //jou 980717 add from ht9040 show alarm enabled
    elConfig->Add(cbG05,        &IniConfig.bG05ShowSpeedMessage,                ECBool,     "Visible", "bG05ShowSpeedMessage",                  bShow, bEnable, bReadFromFile, LastSet.bShowSpeedMessage?"1":"0");              //Steven 20090720 show speed at uShowMessage
    elConfig->Add(cbG06,        &IniConfig.bG06HomeinitialCheckZ1,              ECBool,     "Visible", "bHomeinitialCheckZ1",                   bShow, bEnable, bReadFromFile, 0);                  //kevin 20131218 歸hom前檢查是否有tray放在hotplate 造成機構損壞 按z1 確認
    elConfig->Add(cbG07,        &IniConfig.bG07MultiColorForFailBin,            ECBool,     "Visible", "bSupportMultiColorForFailBin",          bShow, bEnable, bReadFromFile, 0);                  //Steven 20160310 : 改成有顏色的fail bin

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20160413 顯示Are you sure
        elConfig->Add(cbG08,    &IniConfig.bG08VisibleAreyousure,               ECBool,     "Visible", "bVisibleAreyousure",                    bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbG08,    &IniConfig.bG08VisibleAreyousure,               ECBool,     "Visible", "bVisibleAreyousure",                    bNoShow, bDisable, bFixedValue, 0);

    if(CosFunction.bG09NeedPasswordWhenEditSiteMap)                             //JerryYang 20160425 修改Site map需要密碼
        elConfig->Add(cbG09,    &IniConfig.bG09NeedPasswordWhenEditSiteMap,     ECBool,     "Function", "bNeedPasswordWhenEditSiteMap",         bShow, bEnable, bReadFromFile, 0);                  //JerryYang 20160425 修改Site map需要密碼
    else
        elConfig->Add(cbG09,    &IniConfig.bG09NeedPasswordWhenEditSiteMap,     ECBool,     "Function", "bNeedPasswordWhenEditSiteMap",         bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_ASE_CL)        //JerryYang 20250120 : add
    {
        elConfig->Add(cbG10,        &IniConfig.bG10ShowImmediateUPH,                ECBool,     "Visible", "bG10ShowImmediateUPH",                  bShow, bDisable, bFixedValue, 1);                  //Steven 20160727 : Show immediate UPH
    }
    else
    {
        elConfig->Add(cbG10,        &IniConfig.bG10ShowImmediateUPH,                ECBool,     "Visible", "bG10ShowImmediateUPH",                  bShow, bEnable, bReadFromFile, 0);                  //Steven 20160727 : Show immediate UPH
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        elConfig->Add(cbG11,    &IniConfig.bG11ASEReport,                       ECBool,     "Visible", "bG11ASEReport",                         bShow, bEnable, bReadFromFile, 0);                  //kevin 20170306
    else
        elConfig->Add(cbG11,    &IniConfig.bG11ASEReport,                       ECBool,     "Visible", "bG11ASEReport",                         bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_OSE || CUSTOMER_CODE==CC_ASE_KaohSiung)                //kevin 20180221 (Steven) Arm 1 Arm2 吸取IC 做CONTRACT MODE
        elConfig->Add(cbG12,    &IniConfig.bG12ContractModeManualMessage,       ECBool,     "Visible", "bG12ContractModeManualMessage",         bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbG12,    &IniConfig.bG12ContractModeManualMessage,       ECBool,     "Visible", "bG12ContractModeManualMessage",         bNoShow, bDisable, bFixedValue, 0);

    elConfig->Add(cbG13,        &IniConfig.bG13ShowTempOffsetOnContact,         ECBool,     "Visible", "bG13ShowTempOffsetOnContact",           bShow, bEnable, bReadFromFile, 0);

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //Steven 20231018 : Fixed for G14
    {
        elConfig->Add(cbG14,    &IniConfig.bG14UseStartSoundAlarm,              ECBool,     "Visible", "bG14UseStartSoundAlarm",                bShow, bEnable, bReadFromFile, 0);                  //kevin 20201116  Start 發出聲音 不動 5sec
        elConfig->Add(edG14,    &IniConfig.iG14StartWarrTime,                   ECInteger,  "Visible", "iStartWarrTime",                        bShow, bEnable, bReadFromFile,  3,     false, 1,       10);     //kevin 20201124 run Start alarm
    }
    else
    {
        elConfig->Add(cbG14,    &IniConfig.bG14UseStartSoundAlarm,              ECBool,     "Visible", "bG14UseStartSoundAlarm",                bNoShow, bDisable, bFixedValue, 0);                 //kevin 20201116  Start 發出聲音 不動 5sec
        elConfig->Add(edG14,    &IniConfig.iG14StartWarrTime,                   ECInteger,  "Visible", "iStartWarrTime",                        bNoShow, bDisable, bFixedValue, 3);                 //kevin 20201124 run Start alarm
    }

    elConfig->Add(cbG15,        &IniConfig.bG15LoadInputCount,                  ECBool,     "Visible", "bG15LoadInputCount",                    bShow, bEnable, bReadFromFile, 0);                  //kevin 20211106 輸入顆數達成就 Clean out        //JimmyChiu 20220124 fix name
    elConfig->Add(chkG16,       &IniConfig.bG16BinDispNeedAlarm,                ECBool,     "Visible", "bG16BinDispNeedAlarm",                  bShow, bEnable, bReadFromFile, 0);                  //Steven 20211130 : JSCC要求Bin顯示器異常要alarm

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        elConfig->Add(cbG17,    &IniConfig.b17bUseLoadCCDTrayMap,               ECBool,     "Visible", "b17bUseLoadCCDTrayMap",                 bShow, bEnable, bReadFromFile, 0);                  //kevin 20220610  Load 到定位通知eKeep 拍照，ekeep 自己按Start
        elConfig->Add(cbG17_1,  &IniConfig.b17bUseLoadCCDTrayMapTrayend,        ECBool,     "Visible", "b17bUseLoadCCDTrayMapTrayend",          bShow, bEnable, bReadFromFile, 0);                  //kevin 20230530 trayend 拍照
    }
    else
    {
        elConfig->Add(cbG17,    &IniConfig.b17bUseLoadCCDTrayMap,               ECBool,     "Visible", "b17bUseLoadCCDTrayMap",                 bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbG17_1,  &IniConfig.b17bUseLoadCCDTrayMapTrayend,        ECBool,     "Visible", "b17bUseLoadCCDTrayMapTrayend",          bNoShow, bDisable, bFixedValue, 0);
    }
    elConfig->Add(cbG18_1,      &IniConfig.b18bUseAutoTrayMap,                  ECBool,     "Visible", "b18bUseAutoTrayMap",                    bShow, bEnable, bReadFromFile, 0);                  //kevin 20220610 add Auto 123 Tray Map
    elConfig->Add(cbG18_2,      &IniConfig.b18bDoubleUnloadTray,                ECBool,     "Visible", "b18bDoubleUnloadTray",                  bShow, bEnable, bReadFromFile, 0);                  //kevin 20220610 add Unload Tray 2  倍 的設定 放空盤
    elConfig->Add(edG18Auto1Count, &IniConfig.iUnloaderTrayCount[0],            ECInteger,  "Visible", "iAuto1Count",                           bShow, bEnable, bReadFromFile,  3,     false, 2,       40);
    elConfig->Add(edG18Auto2Count, &IniConfig.iUnloaderTrayCount[1],            ECInteger,  "Visible", "iAuto2Count",                           bShow, bEnable, bReadFromFile,  3,     false, 2,       40);
    elConfig->Add(edG18Auto3Count, &IniConfig.iUnloaderTrayCount[2],            ECInteger,  "Visible", "iAuto3Count",                           bShow, bEnable, bReadFromFile,  3,     false, 2,       40);

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        elConfig->Add(cbG19,    &IniConfig.b19InSHAutoCleanPA,                  ECBool,     "Visible", "b19InSHAutoCleanPA",                    bShow, bEnable, bReadFromFile, 0);                  //kevin 20230530 add Autoclean shuttle define new 參數
        elConfig->Add(cbG20,    &IniConfig.b20FixfullWaitpos,                   ECBool,     "Visible", "b20FixfullWaitpos",                     bShow, bEnable, bReadFromFile, 0);                  //kevin 20230816 out arm move Wait
        elConfig->Add(cbG21,    &IniConfig.b21colorfulltray,                    ECBool,     "Visible", "b21colorfulltray",                      bShow, bEnable, bReadFromFile, 0);                  //kevin 20230918 colortray no Alarm
    }
    else
    {
        elConfig->Add(cbG19,    &IniConfig.b19InSHAutoCleanPA,                  ECBool,     "Visible", "b19InSHAutoCleanPA",                    bNoShow, bDisable, bFixedValue, 0);                 //kevin 20230530 add Autoclean shutt
        elConfig->Add(cbG20,    &IniConfig.b20FixfullWaitpos,                   ECBool,     "Visible", "b20FixfullWaitpos",                     bNoShow, bDisable, bFixedValue, 0);                 //kevin 20230816 out arm move Wait
        elConfig->Add(cbG21,    &IniConfig.b21colorfulltray,                    ECBool,     "Visible", "b21colorfulltray",                      bNoShow, bDisable, bFixedValue, 0);                 //kevin 20230918 colortray no Alarm
    }
    elConfig->Add(cbG22,        &IniConfig.bG22NoticeTakeoutTray,               ECBool,     "Visible", "bG22NoticeTakeoutTray",                 bShow, bEnable, bReadFromFile, 0);                  //JerryYang 20231218 : G22提醒人員取tray功能
    elConfig->Add(cbG23,        &IniConfig.bG23DiasbleFuncStatusView,           ECBool,     "Visible", "bG23DiasbleFuncStatusView",             bShow, bEnable, bReadFromFile, IniConfig.bVTESTFunction==true?"1":"0");    //RogerYang 20250728 Vtest設開關決定要不要顯示View
    elConfig->Add(cbG24,        &IniConfig.bG24DisableSECSGEMStatus,            ECBool,     "Visible", "bG24DisableSECSGEMStatus",              bShow, bEnable, bReadFromFile, 0);                  //RogerYang 20251222 : 丁曉東要求新增選項(G24)是否顯示
}

// AI(W906-FW-CFG-W2) 20260825: golden cConfiguration.cpp:2873-3118, transcribed
// VERBATIM (cp950 -> UTF-8). Registration table only -- the port's
// HTEditList::Add signature is argument-for-argument golden's, so no
// adaptation was needed and none was invented. See FW-CFG-W1's banner in
// forms/fConfiguration.h for why this family is the wave's point.
void TfConfiguration::InitConfigEdtList_ItemL()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;

    elConfig->Add(cbL03,        &IniConfig.bL03SocketAirCoolingCT,              ECBool,     "Tempture", "bL03SocketAirCoolingCT",           bShow, bEnable, bReadFromFile, 0);      //jou 2016-04-28 Socket Air Cooling contact count trun on
    elConfig->Add(edL03,        &IniConfig.iL03SocketAirCoolingCT,              ECInteger,  "Tempture", "iL03SocketAirCoolingCT",           bShow, bEnable, bReadFromFile,  2,     false, 0,       50);     //jou 2016-04-28 Socket Air Cooling contact count trun on

    if(CosFunction.bHiSiliconFunction==true && CUSTOMER_CODE==CC_KYEC_LEE)
        elConfig->Add(edL04,    &IniConfig.iL04TemptureRange,                   ECInteger,  "Tempture", "iL04TemptureRange",                bShow, bEnable, bReadFromFile,  3);     //Ifor 20160311 客戶要求L04~L06強制寫入3    //Ifor 20220419 add:連軍要求不卡控 L04~L06
    else if(CosFunction.bHiSiliconFunction==true)                               //Ifor 20160406 海思 ATC 相關參數寫死
        elConfig->Add(edL04,    &IniConfig.iL04TemptureRange,                   ECInteger,  "Tempture", "iL04TemptureRange",                bShow, bDisable, bReadFromFile, 2,     false, 1,       10);
    else if(CUSTOMER_CODE==CC_ASE_CL)                                           //JerryYang 20200127 : 松諭要求把溫度range下限改為正負1度
        elConfig->Add(edL04,    &IniConfig.iL04TemptureRange,                   ECInteger,  "Tempture", "iL04TemptureRange",                bShow, bEnable, bReadFromFile,  2,     false, 1,       10);
    else if(IniConfig.bVTESTFunction==true)
        elConfig_byRecipe->Add(edL04,       &IniConfig.iL04TemptureRange,       ECInteger,  "Tempture", "iL04TemptureRange",                bShow, bEnable, bReadFromFile,  3,     false, 2,       10);
    else
        elConfig->Add(edL04,    &IniConfig.iL04TemptureRange,                   ECInteger,  "Tempture", "iL04TemptureRange",                bShow, bEnable, bReadFromFile,  2,     false, 2,       10);

    if(CosFunction.bHiSiliconFunction==true && CUSTOMER_CODE==CC_KYEC_LEE)
        elConfig->Add(edL05,    &IniConfig.iL05ChamberTemptureRange,            ECInteger,  "Tempture", "iL05ChamberTemptureRange",         bShow, bEnable, bReadFromFile,   3); //Ifor 20160311 客戶要求L04~L06強制寫入3    //Ifor 20220419 add:連軍要求不卡控 L04~L06
    else if(CosFunction.bHiSiliconFunction==true)
        elConfig->Add(edL05,    &IniConfig.iL05ChamberTemptureRange,            ECInteger,  "Tempture", "iL05ChamberTemptureRange",         bShow, bDisable, bReadFromFile, 2,     false, 2,       30);
    else
        elConfig->Add(edL05,    &IniConfig.iL05ChamberTemptureRange,            ECInteger,  "Tempture", "iL05ChamberTemptureRange",         bShow, bEnable,  bReadFromFile, 2,     false, 2,       30);

    if(CosFunction.bHiSiliconFunction==true && CUSTOMER_CODE==CC_KYEC_LEE)
        elConfig->Add(edtL06,   &IniConfig.iAmbTemperatureRange,                ECInteger,  "Temperature", "iAmbTemperatureRange",          bShow, bEnable, bReadFromFile,   3); //Ifor 20160311 客戶要求L04~L06強制寫入3    //Ifor 20220419 add:連軍要求不卡控 L04~L06
    else if(CUSTOMER_CODE==CC_ChipMos_ZHUBEI)                                   //Steven 20230410 : 南茂鍾永生要求修改
        elConfig->Add(edtL06,   &IniConfig.iAmbTemperatureRange,                ECInteger,  "Temperature", "iAmbTemperatureRange",          bShow, bDisable, bReadFromFile, 2,     false, 0,       5);
    else if(CosFunction.bHiSiliconFunction==true)
        elConfig->Add(edtL06,   &IniConfig.iAmbTemperatureRange,                ECInteger,  "Temperature", "iAmbTemperatureRange",          bShow, bDisable, bReadFromFile, 2,     false, 0,       10);
    else
        elConfig->Add(edtL06,   &IniConfig.iAmbTemperatureRange,                ECInteger,  "Temperature", "iAmbTemperatureRange",          bShow, bEnable,  bReadFromFile, 2,     false, 0,       10);         //jou 2013-04-11 Ambient Temperture Range

    if(CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_ASE_KaohSiung_K12)
        elConfig->Add(cbL07,    &IniConfig.bL07UseSingleTenmpertureLimit,       ECBool,     "Tempture", "bL07UseSingleTenmpertureLimit",    bShow, bDisable, bFixedValue, 0);
    else
        elConfig->Add(cbL07,    &IniConfig.bL07UseSingleTenmpertureLimit,       ECBool,     "Tempture", "bL07UseSingleTenmpertureLimit",    bShow, bEnable, bReadFromFile, 0);

    elConfig->Add(edL08_Over,   &IniConfig.iSocketTemptureRangeOver,            ECInteger,  "Tempture", "iSocketTemptureRange",             bShow, bEnable,  bReadFromFile, 3,     false, 2,       30);         //Steven 20140308 : DUT溫度限制改成上下限分開
    elConfig->Add(edL08_Under,  &IniConfig.iSocketTemptureRangeUnder,           ECInteger,  "Tempture", "iSocketTemptureRangeUnder",        bShow, bEnable,  bReadFromFile, 3,     false, 2,       30);         //Steven 20140308 : DUT溫度限制改成上下限分開

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        elConfig->Add(cbL09,    &IniConfig.bL09HotTempShuttleNoAddPos,          ECBool,     "Tempture", "bL23HotTempShuttlenoAddPos",       bShow, bEnable, bFixedValue, 1);        //kevin 20200812 : ASEKH 高溫shuttle不補位置
    else
        elConfig->Add(cbL09,    &IniConfig.bL09HotTempShuttleNoAddPos,          ECBool,     "Tempture", "bL23HotTempShuttlenoAddPos",       bShow, bEnable, bReadFromFile, 0);      //kevin 20200718 : ASEKH 高溫shuttle不補位置

    if(CUSTOMER_CODE==CC_ASE_KaohSiung && CosFunction.bHiSiliconFunction==true)
        elConfig->Add(cbL10,    &IniConfig.bL10IndexTestlogTemp,                ECBool,     "Tempture", "bL10IndexTestlogTemp",             bShow, bEnable, bFixedValue, 1);        //kevin 20200227 add 下壓溫度記 錄
    else
        elConfig->Add(cbL10,    &IniConfig.bL10IndexTestlogTemp,                ECBool,     "Tempture", "bL10IndexTestlogTemp",             bShow, bEnable, bReadFromFile, 0);

    elConfig->Add(rgL10,  &IniConfig.iL10TempRecordInterval,                    ECInteger,  "Index", "iL10TempRecordInterval",                bShow, bEnable,  bReadFromFile, LastSet.iTempRecordInterval);   //Steven 20100225 //Steven 20231017 : fixed

    if(CosFunction.bHiSiliconFunction)                                          //Ifor 20160406 海思 ATC 相關參數寫死
    {
        elConfig->Add(edL11_1,  &IniConfig.iATCTemperatureRange,                ECInteger,  "Index", "iATCTemperatureRange",                bShow, bDisable, bFixedValue,   3);                                 //Ifor 20160406 L11-1 強制設定3度
    }
    else
    {
        if(IniConfig.bVTESTFunction==true)
            elConfig->Add(edL11_1,  &IniConfig.iATCTemperatureRange,                ECInteger,  "Index", "iATCTemperatureRange",                bShow, bEnable,  bReadFromFile, 3,     false, 1,       3);          //jou 20240426 : VTEST 要求 ATC 溫度限制設定1~3度
        else
            elConfig->Add(edL11_1,  &IniConfig.iATCTemperatureRange,                ECInteger,  "Index", "iATCTemperatureRange",                bShow, bEnable,  bReadFromFile, 10,     false, 1,       30);        //Eliot 2015_0105 2~50度
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE && CosFunction.bHiSiliconFunction==true)      //Ifor 20160310  取消水冷機自動關閉功能
        elConfig->Add(cbL11_2,  &IniConfig.bL11_2ATCChillerProtectedFunction,   ECBool,     "Index", "bATCChillerProtectedFunction",        bShow, bDisable, bFixedValue, 0);
    else
        elConfig->Add(cbL11_2,  &IniConfig.bL11_2ATCChillerProtectedFunction,   ECBool,     "Index", "bATCChillerProtectedFunction",        bShow, bEnable, bReadFromFile, 0);      // 2012.05.07  Joye  Chiller

    elConfig->Add(edL11_1_2,    &IniConfig.dATCTemperatureCheckTime,            ECDouble,   "Index", "dATCTemperatureCheckTime",            bShow, bEnable,  bReadFromFile, 10.0,   false, 0.01,    180.0);     //Steven 20121222 : ATC Temperature Over Check Time
    elConfig->Add(edL11_2,      &IniConfig.iATCChillerCheckTime,                ECInteger,  "Index", "iATCChillerCheckTime",                bShow, bEnable,  bReadFromFile, 5,      false, 5,       300);
    elConfig->Add(edL11_4,      &IniConfig.iATCTemperatureOverLimit,            ECInteger,  "Index", "iATCTemperatureOverLimit",            bShow, bEnable,  bReadFromFile, 130,    false, 2,       130);       //Steven 20140916 : [L11-4] ATC的最高上限溫度

    if(ATC_SYSTEM==eATCUninstall || ATC_SYSTEM==eNonChamber)
    {
        elConfig->Add(cbL11_1,  &IniConfig.bL11_1ATCTemperatureOverAlarm,       ECBool,     "Index", "bATCTemperatureOverAlarm",            bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbL11_6,  &IniConfig.bL11_6ATCUseTemperatureOutsideAlarm, ECBool,     "Tempture", "bATCUseTemperatureOutsideAlarm",   bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbL11_7,  &IniConfig.bL11_7ATCUseMaxSurgeAlarm,           ECBool,     "Tempture", "bATCUseMaxSurgeAlarm",             bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbL11_8,  &IniConfig.bL11_8ATCUseTemperatureCompare,      ECBool,     "Tempture", "bATCUseTemperatureCompare",        bNoShow, bDisable, bFixedValue, 0);
    }
    else if(CosFunction.bHiSiliconFunction==true)
    {
        elConfig->Add(cbL11_1,  &IniConfig.bL11_1ATCTemperatureOverAlarm,       ECBool,     "Index", "bATCTemperatureOverAlarm",            bShow, bDisable, bFixedValue, 1);
        elConfig->Add(cbL11_6,  &IniConfig.bL11_6ATCUseTemperatureOutsideAlarm, ECBool,     "Tempture", "bATCUseTemperatureOutsideAlarm",   bShow, bDisable, bFixedValue, 1);
        elConfig->Add(edL11_6_Outside,    &IniConfig.iATCTemperatureOutside,    ECInteger,  "Tempture", "iATCTemperatureOutside",           bShow, bDisable, bFixedValue, 3);   //Ifor 20160406 海思版本ATC功能[L11-6]強制設定 3度
        elConfig->Add(edL11_6_Continuous, &IniConfig.iATCTemperatureContinuous, ECInteger,  "Tempture", "iATCTemperatureContinuous",        bShow, bDisable, bFixedValue, 3);   //Ifor 20160406 海思版本ATC功能[L11-6]強制設定 3秒
        elConfig->Add(cbL11_7,  &IniConfig.bL11_7ATCUseMaxSurgeAlarm,           ECBool,     "Tempture", "bATCUseMaxSurgeAlarm",             bShow, bDisable, bFixedValue, 1);
        elConfig->Add(edL11_7_MaxSurge,   &IniConfig.iATCMaxSurgeAlarm,         ECInteger,  "Tempture", "iATCMaxSurgeAlarm",                bShow, bDisable, bFixedValue, 7);   //Ifor 20160406 海思版本ATC功能[L11-7]強制設定 7度
        elConfig->Add(cbL11_8,  &IniConfig.bL11_8ATCUseTemperatureCompare,      ECBool,     "Tempture", "bATCUseTemperatureCompare",        bShow, bDisable, bFixedValue, 1);
    }
    else
    {
        if(IniConfig.bVTESTFunction==true)
            elConfig->Add(cbL11_1,  &IniConfig.bL11_1ATCTemperatureOverAlarm,       ECBool,     "Index", "bATCTemperatureOverAlarm",            bShow, bDisable, bFixedValue, 1);      // 2012.05.11  Joye  ATC Temperature Over Check
        else
            elConfig->Add(cbL11_1,  &IniConfig.bL11_1ATCTemperatureOverAlarm,       ECBool,     "Index", "bATCTemperatureOverAlarm",            bShow, bEnable, bReadFromFile, 0);      // 2012.05.11  Joye  ATC Temperature Over Check
        elConfig->Add(cbL11_6,  &IniConfig.bL11_6ATCUseTemperatureOutsideAlarm, ECBool,     "Tempture", "bATCUseTemperatureOutsideAlarm",   bShow, bEnable, bReadFromFile, 0);      //Ifor 20150911 : [L11-6] ATC突波於設定範圍內且持續發生超過設定時間後發出Alarm
        elConfig->Add(edL11_6_Outside,    &IniConfig.iATCTemperatureOutside,    ECInteger,  "Tempture", "iATCTemperatureOutside",           bShow, bEnable,  bReadFromFile, 2,      false, 1,       30);    //Ifor 20150911 : [L11-6] ATC 突波超出設定溫度
        elConfig->Add(edL11_6_Continuous, &IniConfig.iATCTemperatureContinuous, ECInteger,  "Tempture", "iATCTemperatureContinuous",        bShow, bEnable,  bReadFromFile, 3,      false, 1,       30);    //Ifor 20150911 : [L11-6] ATC 允許突波持續發生時間(s)
        elConfig->Add(cbL11_7,  &IniConfig.bL11_7ATCUseMaxSurgeAlarm,           ECBool,     "Tempture", "bATCUseMaxSurgeAlarm",             bShow, bEnable, bReadFromFile, 0);      //Ifor 20150911 : [L11-7] ATC突波超出最大設定值發出Alarmed
        elConfig->Add(edL11_7_MaxSurge,   &IniConfig.iATCMaxSurgeAlarm,         ECInteger,  "Tempture", "iATCMaxSurgeAlarm",                bShow, bEnable,  bReadFromFile, 7,      false, 1,       30);    //Ifor 20150911 : [L11-7] ATC 最高突波上限
        elConfig->Add(cbL11_8,  &IniConfig.bL11_8ATCUseTemperatureCompare,      ECBool,     "Tempture", "bATCUseTemperatureCompare",        bShow, bEnable, bReadFromFile, 0);      //Ifor 20150911 : [L11-8] ATC第1組sensor跟第2組sensor溫度差值超過設定值發出警報
    }

    elConfig->Add(cbL12,        &IniConfig.bL12TempErrNoCloseHeater,            ECBool,     "Tempture", "TempErrNoCloseHeater",                     bShow, bEnable, bReadFromFile, 0);                      //ChungHung 20120913 SCK  要求Temp Error 不要關閉加熱
    elConfig->Add(cbL13,     &IniConfig.bL13HotPlateAndShuttleUseOneTempOffset, ECBool,     "Tempture", "bHotPlateAndShuttleUseOneTempOffset",      bShow, bEnable, bReadFromFile, 0);        //Steven 20131023 : 加熱盤與蝦頭使用同一個溫度補償的檔案
    if(CosFunction.bChamberModeEvenBlowNeedWaitTime)                            //ChungHung 20140519 add Chamber Mode Even Blow need Wait Initial Wait time in Temp_Set
        elConfig->Add(cbL15, &IniConfig.bL15EnableChamberModeEvenBlowNeedWaitTime, ECBool,  "Tempture", "bEnableChamberModeEvenBlowNeedWaitTime",   bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbL15, &IniConfig.bL15EnableChamberModeEvenBlowNeedWaitTime, ECBool,  "Tempture", "bEnableChamberModeEvenBlowNeedWaitTime",   bNoShow, bDisable, bFixedValue, 0);

    elConfig->Add(cbL17,        &IniConfig.bL17HeadHeaterOnWhenCloseSite,       ECBool,     "Tempture", "bHeadHeaterOnWhenCloseSite",       bShow, bEnable, bReadFromFile, 0);      //Steven 20150803 : 關Site的地方也要開啟加熱 (For ATK)

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20160107 No FullSite Add Offset
        elConfig->Add(cbL18,    &IniConfig.bL18NofullsiteaddTemperatureoffset,  ECBool,     "Tempture", "NofullsiteaddTemperatureoffset",   bShow, bEnable, bReadFromFile, 0);
    else
        elConfig->Add(cbL18,    &IniConfig.bL18NofullsiteaddTemperatureoffset,  ECBool,     "Tempture", "NofullsiteaddTemperatureoffset",   bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_JCET)
        elConfig->Add(cbL19,    &IniConfig.bL19OpenHeatDoorgiveupchamberhot,    ECBool,     "Tempture", "bL19OpenHeatDoorgiveupchamberhot", bShow, bEnable, bReadFromFile, 0);      //kevin 20170520 (wei) 開chamber門只有不加熱chamber
    else
        elConfig->Add(cbL19,    &IniConfig.bL19OpenHeatDoorgiveupchamberhot,    ECBool,     "Tempture", "bL19OpenHeatDoorgiveupchamberhot", bNoShow, bDisable, bFixedValue, 0);

    elConfig->Add(cbL20,        &IniConfig.bL20AbientGuardBand,                 ECBool,     "Tempture", "bL20AbientGuardBand",              bShow, bEnable, bReadFromFile, 0);      //kevin 20180115 (Steven) add Amient Guard Band
    elConfig->Add(cbL21,        &IniConfig.bL21PowerOffTemperature,             ECBool,     "Tempture", "bL21PowerOffTemperature",          bShow, bEnable, bReadFromFile, 0);      //kevin 20181112 (Steven) : power off 開 Chamber door 斷所有加熱電

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        elConfig->Add(cbL22,    &IniConfig.bL22Enable3SigmaTempMonitor,         ECBool,     "Tempture", "bL22Enable3SigmaTempMonitor",      bShow, bEnable, bReadFromFile, 0);      //kevin 20200521 : ASEKH 3SIGMA  溫度統計
    else
        elConfig->Add(cbL22,    &IniConfig.bL22Enable3SigmaTempMonitor,         ECBool,     "Tempture", "bL22Enable3SigmaTempMonitor",      bNoShow, bDisable, bFixedValue, 0);      //kevin 20200521 : ASEKH 3SIGMA  溫度統計

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20210122 : ASE-CL新增待溫功能
    {
        elConfig->Add(cbL24,    &IniConfig.bL24HeaterStableTime,                ECBool,     "Tempture", "bL24HeaterStableTime",             bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edL24,    &IniConfig.iL24HeaterStableTime,                ECInteger,  "Index", "iL24HeaterStableTime",                bShow, bEnable, bReadFromFile,  30, false, 10, 1800);
    }
    else
    {
        elConfig->Add(cbL24,    &IniConfig.bL24HeaterStableTime,                ECBool,     "Tempture", "bL24HeaterStableTime",             bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edL24,    &IniConfig.iL24HeaterStableTime,                ECInteger,  "Index", "iL24HeaterStableTime",                bNoShow, bDisable, bFixedValue,  0);
    }

    if(CosFunction.bHiSiliconFunction==true ||
       TestIF_File.i2DIDFormat==eAMD ||
       CosFunction.bATCUseHandlerRecipeName==true)
    {
        elConfig->Add(cbL25,    &IniConfig.bL25_1ATCFileNameWithTemp,           ECBool,     "Tempture", "bL25_1ATCFileNameWithTemp",        bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        elConfig->Add(cbL25,    &IniConfig.bL25_1ATCFileNameWithTemp,           ECBool,     "Tempture", "bL25_1ATCFileNameWithTemp",        bNoShow, bDisable, bFixedValue, 0);
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                               //Sam 20231214 : Temp offset use ready temp range
        elConfig->Add(cbL28,    &IniConfig.bL28TempOfsUseReadyTempRange,        ECBool,     "Tempture", "bL28TempOfsUseReadyTempRange",     bShow, bDisable, bFixedValue, 1);
    else if(CUSTOMER_CODE==CC_ASE_SG)                                           //Ifor 20251105 add: ASE SG 客戶要求[L28]強制關閉不顯示
        elConfig->Add(cbL28,    &IniConfig.bL28TempOfsUseReadyTempRange,        ECBool,     "Tempture", "bL28TempOfsUseReadyTempRange",     bNoShow, bDisable, bFixedValue, 0);
    else
        elConfig->Add(cbL28,    &IniConfig.bL28TempOfsUseReadyTempRange,        ECBool,     "Tempture", "bL28TempOfsUseReadyTempRange",     bShow, bEnable, bReadFromFile, 0);

    elConfig->Add(cbL29,        &IniConfig.bL29AmbientNotShowTemp,              ECBool,     "Tempture", "bL29AmbientNotShowTemp",           bShow, bEnable, bReadFromFile, 0);      //Sam 20221101 : 常溫模式不顯示溫度

    if(CosFunction.bUse1CableLayoutKitByConfig)                                 //Sam 20210715 : Use 1CableLayoutKit By Config
    {
        gbL30->Visible=true;
        elConfig->Add(cbL30,    &IniConfig.bL30Use1CableLayoutKitByConfig,      ECBool,     "Tempture", "bL30Use1CableLayoutKitByConfig",   bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbL30_1,  &IniConfig.bL30Use1CableLayoutKit,              ECBool,     "Tempture", "bL30Use1CableLayoutKit",           bShow, bEnable, bReadFromFile, 0);
    }
    else
    {
        gbL30->Visible=false;
        elConfig->Add(cbL30,    &IniConfig.bL30Use1CableLayoutKitByConfig,      ECBool,     "Tempture", "bL30Use1CableLayoutKitByConfig",   bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbL30_1,  &IniConfig.bL30Use1CableLayoutKit,              ECBool,     "Tempture", "bL30Use1CableLayoutKit",           bNoShow, bDisable, bFixedValue, 0);
    }

    //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    if(Tri_Temp_Machine==1)
    {
        elConfig->Add(edtL09_1,   &IniConfig.iL09_1HightTemp_Sht_Shift,         ECInteger,  "Tempture", "iL09_1HightTemp_Sht_Shift",            bShow, bEnable, bReadFromFile,  15,  false, -100, 100.0);//Ztex 2024.02.21 Add Change Temp When Shuttle Auto Shift Pos
        elConfig->Add(edtL09_2,   &IniConfig.iL09_2LowTemp_Sht_Shift,           ECInteger,  "Tempture", "iL09_2LowTemp_Sht_Shift",              bShow, bEnable, bReadFromFile,  -40, false, -100, 100.0);//Ztex 2024.02.21 Add Change Temp When Shuttle Auto Shift Pos

        elConfig->Add(cbL32_1,    &IniConfig.bL32_1ManuDefrost,                 ECBool,     "Tempture", "bL32_1ManualDefrost",                  bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbL32_2,    &IniConfig.bL32_2AutoDefrostFunction,         ECBool,     "Tempture", "bL32_2AutoDefrostFunction",            bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edL32_4,    &IniConfig.iL32_4SetDefrostTemp,              ECInteger,  "Tempture", "iL32_4SetDefrostTempature",            bShow, bEnable, bReadFromFile,  10, false, 80, 100.0);
        elConfig->Add(edL32_5,    &IniConfig.iL32_5SetDefrostTime,              ECInteger,  "Tempture", "iL32_5SetDefrostTime",                 bShow, bEnable, bReadFromFile,  10, false, 10, 60.0);
        elConfig->Add(edL32_6,    &IniConfig.iL32_6LowTempRunAlarmDegree,       ECInteger,  "Tempture", "iL32_6LowTempRunAlarmDegree",          bShow, bEnable, bReadFromFile,  30, false, 10, 40);
        elConfig->Add(edL32_7_1,  &IniConfig.iL32_7LowTempRunAlarmHour,         ECInteger,  "Tempture", "iL32_7LowTempRunAlarmHour",            bShow, bEnable, bReadFromFile,  2, false, 2, 72);
        elConfig->Add(edL32_7_2,  &IniConfig.iL32_7LowTempRunAlarmMin,          ECInteger,  "Tempture", "iL32_7LowTempRunAlarmMin",             bShow, bEnable, bReadFromFile,  30, false, 0, 59);
        elConfig->Add(edL32_8,    &IniConfig.iL32_8SetAirStreamTemp,            ECInteger,  "Tempture", "iL32_8SetAirStreamTemp",               bShow, bEnable, bReadFromFile,  40, false, 40, 180);

        elConfig->Add(cbL33_1,    &IniConfig.bL33_1CheckDoorOpenForTriTemp,     ECBool,     "Tempture", "bL33_1CheckDoorOpenForTriTemp",        bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbL33_2,    &IniConfig.bL33_2DoorOpenRunDefrost,          ECBool,     "Tempture", "bL33_2DoorOpenRunDefrost",             bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edL33_3,    &IniConfig.iL33_3BDoorOpenTimeForLowTemp,     ECInteger,  "Tempture", "iL33_3BDoorOpenTimeForLowTemp",        bShow, bEnable, bReadFromFile,  10, false, 10, 200.0);
        elConfig->Add(edL33_4,    &IniConfig.iL33_4SDoorOpenTimeForLowTemp,     ECInteger,  "Tempture", "iL33_4SDoorOpenTimeForLowTemp",        bShow, bEnable, bReadFromFile,  10, false, 10, 200.0);
        elConfig->Add(edL33_5,    &IniConfig.iL33_5DoorOpenTempForLowTemp,      ECInteger,  "Tempture", "iL33_5DoorOpenTempForLowTemp",         bShow, bEnable, bReadFromFile,  10, false, -55, 40.0);
        elConfig->Add(edL33_6,    &IniConfig.iL33_6DoorOpenTempForHotTemp,      ECInteger,  "Tempture", "iL33_6DoorOpenTempForHotTemp",         bShow, bEnable, bReadFromFile,  10, false, 40, 175.0);

        elConfig->Add(cbL34_1,    &IniConfig.bL34_1DelayOfFixDoorOpen,          ECBool,     "Tempture", "bL34_1DelayOfFixDoorOpen",             bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edL34_2,    &IniConfig.iL34_2DelaySecOfFixDoorOpen,       ECInteger,  "Tempture", "iL34_2DelaySecOfFixDoorOpen",          bShow, bEnable, bReadFromFile,  10, false, 10, 100.0);
        elConfig->Add(edL34_3,    &IniConfig.dL34_3DewPointOfFixDoorOpen,       ECDouble,   "Tempture", "dL34_3DewPointOfFixDoorOpen",          bShow, bEnable,  bReadFromFile, 10.0,   false, 0.00,    -60.0);     //Steven 20121222 : ATC Temperature Over Check Time
        elConfig->Add(edL34_4,    &IniConfig.iL34_4OpenAuto3TrackGateSec,       ECInteger,  "Tempture", "iL34_4OpenAuto3TrackGateSec",          bShow, bEnable, bReadFromFile,  30, false, 10, 1800);
        elConfig->Add(cbL34_5,    &IniConfig.bL34_5FixTrayDoorCynAutoOpen,      ECBool,     "Tempture", "bL34_5FixTrayDoorCynAutoOpen",         bShow, bEnable, bReadFromFile, 0);

        elConfig->Add(cbL35_1,    &IniConfig.bL35_1OverSetTempOpenFan,          ECBool,     "Tempture", "bL35_1OverSetTempOpenFan",             bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edL35_2,    &IniConfig.iL35_2OpenFanTemp,                 ECInteger,  "Tempture", "iL35_2OpenFanTemp",                    bShow, bEnable, bReadFromFile,  30, false, 40, 175);

        elConfig->Add(edtL36_1,   &IniConfig.iL36_1Tri_Temp_Rang_ATC,           ECInteger,  "Tempture", "iL36_1Tri_Temp_Rang_ATC",                            bShow, bEnable, bReadFromFile, 3, false, 1,    10.0);
        elConfig->Add(edtL36_2,   &IniConfig.iL36_2Tri_Temp_Rang_Heater,        ECInteger,  "Tempture", "iL36_2Tri_Temp_Rang_Heater",                         bShow, bEnable, bReadFromFile, 3, false, 1,    20.0);

        elConfig->Add(cbL37,      &IniConfig.bL37UnDockTurnOffAir,              ECBool,     "Tempture", "bL37UnDockTurnOffAir",               bShow, bEnable, bReadFromFile, 0);

        elConfig->Add(cbL39_1,    &IniConfig.bL39_1AutoRunWhenTempOk,           ECBool,     "Tempture", "bL39_1AutoRunWhenTempOk",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbL39_2,    &IniConfig.bL39_2WaitTempstabilize,           ECBool,     "Tempture", "bL39_2WaitTempstabilize",                bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edtL39_2,   &IniConfig.iL39_2WaitTempstabilize,           ECInteger,  "Tempture", "iL39_2WaitTempstabilize",                bShow, bEnable, bReadFromFile,  3, false, 1, 180);

        elConfig->Add(edL40,      &IniConfig.iL40ImmediateTempExceedsAlarm,     ECInteger,  "Tempture", "iL40ImmediateTempExceedsAlarm",     bShow, bEnable, bReadFromFile,  7, false, 1, 10);

        elConfig->Add(edL41,      &IniConfig.iL41TemperatureAlarmSecond,        ECInteger,  "Tempture", "iL41TemperatureAlarmSecond",     bShow, bEnable, bReadFromFile,  3, false, 1, 10);

        elConfig->Add(cbL42,      &IniConfig.bL42_UseOutShuttleDesoakTime,      ECBool,     "Tempture", "bL42_UseOutShuttleDesoakTime",         bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edtL42,     &IniConfig.iL42_UseOutShuttleDesoakTime,      ECInteger,  "Tempture", "iL42_UseOutShuttleDesoakTime",         bShow, bEnable, bReadFromFile,  30, false, 0, 120);

        elConfig->Add(cbL44,      &IniConfig.bL44_SetColdAirSwitchTemp,         ECBool,     "Tempture", "bL44_SetColdAirSwitchTemp",         bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edtL44,     &IniConfig.iL44_SetColdAirSwitchTemp,         ECInteger,  "Tempture", "iL44_SetColdAirSwitchTemp",         bShow, bEnable, bReadFromFile,  30, false, 0, 25);

        elConfig->Add(cbL45,      &IniConfig.bL45_SetDewPointOffset,            ECBool,     "Tempture", "bL45_SetDewPointOffset",         bShow, bEnable, bReadFromFile, 0);                //Ztex 2025.04.01 Add Set Dew Point Offset
        elConfig->Add(edtL45,     &IniConfig.iL45_SetDewPointOffset,            ECInteger,  "Tempture", "iL45_SetDewPointOffset",         bShow, bEnable, bReadFromFile,  30, false, -25, 25);//Ztex 2025.04.01 Add Set Dew Point Offset
        elConfig->Add(cbL46,      &IniConfig.bL46_AStreamErrorCompressOnecycle, ECBool,     "Tempture", "bL43_AStreamErrorCompressOnecycle",    bShow, bEnable, bReadFromFile, 0);//Ztex 2024.10.01 Add AStream Error Compress Onecycle
        //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    }
    else
    {
        elConfig->Add(cbL42,     &IniConfig.bL42_UseOutShuttleDesoakTime,       ECBool,     "Tempture", "bL42_UseOutShuttleDesoakTime",         bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edtL42,    &IniConfig.iL42_UseOutShuttleDesoakTime,       ECInteger,  "Tempture", "iL42_UseOutShuttleDesoakTime",         bNoShow, bDisable, bFixedValue, 0);
    }

    if(ATC_SYSTEM==eNewATCSystem)                                               //KenHsieh 20240216 : add ATC Power Follow Function
        elConfig->Add(cbL43,    &IniConfig.bL43EnableATCPowerFollow,            ECBool,     "Tempture", "bL43EnableATCPowerFollow",             bShow,   bEnable,  bReadFromFile, 0);
    else
        elConfig->Add(cbL43,    &IniConfig.bL43EnableATCPowerFollow,            ECBool,     "Tempture", "bL43EnableATCPowerFollow",             bNoShow, bDisable, bFixedValue, 0);
}

// AI(W906-FW-CFG-W2) 20260825: golden cConfiguration.cpp:3120-3161, transcribed
// VERBATIM (cp950 -> UTF-8). Registration table only -- the port's
// HTEditList::Add signature is argument-for-argument golden's, so no
// adaptation was needed and none was invented. See FW-CFG-W1's banner in
// forms/fConfiguration.h for why this family is the wave's point.
void TfConfiguration::InitConfigEdtList_ItemM()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;
    tsM00->TabVisible=(CUSTOMER_CODE==CC_SCK || CUSTOMER_CODE==CC_ASE_CL);                                  //ChungHung 20131009 add for SCK
    if(CUSTOMER_CODE==CC_SCK || CUSTOMER_CODE==CC_ASE_CL)
    {
        elConfig->Add(cbM01,    &IniConfig.bM01EnableMonitorFunction,           ECBool,     "Monitor", "bEnableMonitorFunction",                        bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_01, &IniConfig.bM0101ContactModeUseDifferentSpeed,  ECBool,     "Monitor", "bMonitorContactModeMustSelectDifferentSpeed",   bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_02, &IniConfig.bM0102SiteYieldDifferentMustOn,      ECBool,     "Monitor", "bMonitorSiteYieldDifferentMustOn",              bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_03, &IniConfig.bM0103ContinueFailBySocketMustOn,    ECBool,     "Monitor", "bMonitorSiteYieldContinueFailBySocketMustOn",   bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_04, &IniConfig.bM0104ContinueFailByHeadMustOn,      ECBool,     "Monitor", "bMonitorSiteYieldContinueFailByHeadMustOn",     bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_05, &IniConfig.bM0105InOutArmDeviceCheckMustOn,     ECBool,     "Monitor", "bMonitorInOutArmDeviceCheckMustOn",             bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_06, &IniConfig.bM0106IndexDeviceCheckDestoryMustOn, ECBool,     "Monitor", "bMonitorIndexArmDeviceCheckDestoryMustOn",      bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_07, &IniConfig.bM0107AutoSpeedMustOn,               ECBool,     "Monitor", "bMonitorAutoSpeedMustOn",                       bShow, bEnable, bReadFromFile, 0);  //ChungHung 20131009 add for SCK
        elConfig->Add(cbM01_08, &IniConfig.bM0108EveryFirstDeviceMustOn,        ECBool,     "Monitor", "bMonitorEveryFirstDeviceMustEnabled",           bShow, bEnable, bReadFromFile, 0);  //ChungHung 20141210 add for SCK want to add Monitor every first device have delay time
        elConfig->Add(cbM01_09, &IniConfig.bM0109RTCOffCheckYieldPiggyBack,     ECBool,     "Monitor", "bM0109RTCOffCheckYieldPiggyBack",               bShow, bEnable, bReadFromFile, 0);  //ChungHung 20150613 add for SCK want to check and show message
        elConfig->Add(cbM01_10, &IniConfig.bM1010Disable_I12,                   ECBool,     "Monitor", "bM1010Disable_I12",                             bShow, bEnable, bReadFromFile, 0);  //Steven 20160727 : For SCK
        elConfig->Add(cbM01_12, &IniConfig.bM1012EnableAutoClean,               ECBool,     "Monitor", "bM1012EnableAutoClean",                         bShow, bEnable, bReadFromFile, 0);  //Steven 20160727 : For SCK
        elConfig->Add(cbM01_13, &IniConfig.bM1013Enable2DID,                    ECBool,     "Monitor", "bM1013Enable2DID",                              bShow, bEnable, bReadFromFile, 0);  //Steven 20160727 : For SCK
        elConfig->Add(cbM01_14, &IniConfig.bM1014EnableATC,                     ECBool,     "Monitor", "bM1014EnableATC",                               bShow, bEnable, bReadFromFile, 0);  //Steven 20191129 : For SCK
    }
    else
    {
        elConfig->Add(cbM01,    &IniConfig.bM01EnableMonitorFunction,           ECBool,     "Monitor", "bEnableMonitorFunction",                        bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_01, &IniConfig.bM0101ContactModeUseDifferentSpeed,  ECBool,     "Monitor", "bMonitorContactModeMustSelectDifferentSpeed",   bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_02, &IniConfig.bM0102SiteYieldDifferentMustOn,      ECBool,     "Monitor", "bMonitorSiteYieldDifferentMustOn",              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_03, &IniConfig.bM0103ContinueFailBySocketMustOn,    ECBool,     "Monitor", "bMonitorSiteYieldContinueFailBySocketMustOn",   bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_04, &IniConfig.bM0104ContinueFailByHeadMustOn,      ECBool,     "Monitor", "bMonitorSiteYieldContinueFailByHeadMustOn",     bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_05, &IniConfig.bM0105InOutArmDeviceCheckMustOn,     ECBool,     "Monitor", "bMonitorInOutArmDeviceCheckMustOn",             bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_06, &IniConfig.bM0106IndexDeviceCheckDestoryMustOn, ECBool,     "Monitor", "bMonitorIndexArmDeviceCheckDestoryMustOn",      bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_07, &IniConfig.bM0107AutoSpeedMustOn,               ECBool,     "Monitor", "bMonitorAutoSpeedMustOn",                       bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_08, &IniConfig.bM0108EveryFirstDeviceMustOn,        ECBool,     "Monitor", "bMonitorEveryFirstDeviceMustEnabled",           bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_09, &IniConfig.bM0109RTCOffCheckYieldPiggyBack,     ECBool,     "Monitor", "bM0109RTCOffCheckYieldPiggyBack",               bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_10, &IniConfig.bM1010Disable_I12,                   ECBool,     "Monitor", "bM1010Disable_I12",                             bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_12, &IniConfig.bM1012EnableAutoClean,               ECBool,     "Monitor", "bM1012EnableAutoClean",                         bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_13, &IniConfig.bM1013Enable2DID,                    ECBool,     "Monitor", "bM1013Enable2DID",                              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbM01_14, &IniConfig.bM1014EnableATC,                     ECBool,     "Monitor", "bM1014EnableATC",                               bNoShow, bDisable, bFixedValue, 0); //Steven 20191129 : For SCK
    }
}

// AI(W906-FW-CFG-W2) 20260825: golden cConfiguration.cpp:3921-4095, transcribed
// VERBATIM (cp950 -> UTF-8). Registration table only -- the port's
// HTEditList::Add signature is argument-for-argument golden's, so no
// adaptation was needed and none was invented. See FW-CFG-W1's banner in
// forms/fConfiguration.h for why this family is the wave's point.
void TfConfiguration::InitConfigEdtList_ItemO()
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true, bDisable=false;
    bool bReadFromFile=true, bFixedValue=false;
    AnsiString str;
    bool bDefaultO10=false;

    bDefaultO10=IniConfig.bSPILFunction;                    //JerryYang 20251013 : fix O10預設值錯誤

    elConfig->Add(cbO01,        &IniConfig.bO01_ResetNeedClearAndCheckHP,       ECBool,     "Count", "bResetNeedClearAndCheckHotPlate",                 bShow, bEnable, bReadFromFile, 0);
    if(IniConfig.bSPILFunction || IniConfig.bResetClearArmIC==true)
    {
        elConfig->Add(cbO02,    &IniConfig.bO02ResetNotClearPlate,              ECBool,     "Count", "bO02ResetNotClearPlate",                          bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(cbO05,    &IniConfig.bO05ResetNeedRemoveAllTray,          ECBool,     "Count", "bO05ResetNeedRemoveAllTray",                      bShow, bEnable, bReadFromFile, 0);      //Eliot 2010_03_05
    }
    else
    {
        elConfig->Add(cbO02,    &IniConfig.bO02ResetNotClearPlate,              ECBool,     "Count", "bO02ResetNotClearPlate",                          bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbO05,    &IniConfig.bO05ResetNeedRemoveAllTray,          ECBool,     "Count", "bO05ResetNeedRemoveAllTray",                      bNoShow, bDisable, bFixedValue, 0);
    }

    if(IniConfig.bEventLogAutoSaveFunction)                                     //Steven 20110221 Start : EventLogAutoSave
    {
        elConfig->Add(cbO06,                &IniConfig.bO06_EventLogAutoSave,           ECBool, "Event Log", "EnableAutoSaveEventLog",                  bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(chkO06UseNetDrive,    &IniConfig.bAlarmStatistAutoSaveNetDrive,   ECBool, "Event Log", "bAlarmStatistAutoSaveNetDrive",           bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(chkO06TimePeriod,     &IniConfig.bO06SaveLogTimePeriod,           ECBool, "Event Log", "EnanleTimePeriodSaveLog",                 bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(chkO06AlarmHistroy,   &IniConfig.bEnableAlarmHistroyAutoSave,     ECBool, "Event Log", "EnableAutoSaveAlarmHistroy",              bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(chkO06AlarmStatist,   &IniConfig.bEnableAlarmStatistAutoSave,     ECBool, "Event Log", "EnableAutoSaveAlarmStatist",              bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(chkO06Production,     &IniConfig.bEnableProductionAutoSave,       ECBool, "Event Log", "EnableAutoSaveProductiont",               bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edO06_FilePath,       &IniConfig.asEventLogAutoSavePath,          ECText, "Event Log", "AutoSaveEventLogPath",                    bShow, bEnable, bReadFromFile, "D:\\RMS");
        elConfig->Add(edtO06AlarmHistroy,   &IniConfig.asAlarmHistroyAutoSavePath,      ECText, "Event Log", "AutoSaveAlarmHistroyPath",                bShow, bEnable, bReadFromFile, "D:\\RMS");
        elConfig->Add(edtO06AlarmStatist,   &IniConfig.asAlarmStatistAutoSavePath,      ECText, "Event Log", "AutoSaveAlarmStatistPath",                bShow, bEnable, bReadFromFile, "D:\\RMS");
        elConfig->Add(edtO06Production,     &IniConfig.asProductionAutoSavePath,        ECText, "Event Log", "AutoSaveProductionPath",                  bShow, bEnable, bReadFromFile, "D:\\RMS");          //Steven 20140816 : Production Data
        elConfig->Add(edtO06_Remote,        &IniConfig.asAlarmRemoteDirectory,          ECText, "Event Log", "asAlarmLocalDirectory",                   bShow, bEnable, bReadFromFile, "Z:");               //Steven 20140306 : 使用網路硬碟
        elConfig->Add(edtO06_Local,         &IniConfig.asAlarmLocalDirectory,           ECText, "Event Log", "asAlarmRemoteDirectory",                  bShow, bEnable, bReadFromFile, "\\\\NET_DRVE");     //Steven 20140306 : 使用網路硬碟
        elConfig->Add(cbO06TimePeriod,      &IniConfig.iO06SaveLogTimePeriod,           ECInteger, "Event Log", "TimePeriodSaveLog",                    bShow, bEnable, bReadFromFile, 0); //JerryYang 20151026 : ProductionData週期時間(0->10mins,1->30mins)
    }
    else
    {
        elConfig->Add(cbO06,                &IniConfig.bO06_EventLogAutoSave,           ECBool, "Event Log", "EnableAutoSaveEventLog",                  bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(chkO06UseNetDrive,    &IniConfig.bAlarmStatistAutoSaveNetDrive,   ECBool, "Event Log", "bAlarmStatistAutoSaveNetDrive",           bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(chkO06TimePeriod,     &IniConfig.bO06SaveLogTimePeriod,           ECBool, "Event Log", "EnanleTimePeriodSaveLog",                 bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(chkO06AlarmHistroy,   &IniConfig.bEnableAlarmHistroyAutoSave,     ECBool, "Event Log", "EnableAutoSaveAlarmHistroy",              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(chkO06AlarmStatist,   &IniConfig.bEnableAlarmStatistAutoSave,     ECBool, "Event Log", "EnableAutoSaveAlarmStatist",              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(chkO06Production,     &IniConfig.bEnableProductionAutoSave,       ECBool, "Event Log", "EnableAutoSaveProductiont",               bNoShow, bDisable, bFixedValue, 0);
    }

//    if(IniConfig.bSPILFunction==true)     //JerryYang 20250305 : mark掉, 暫時先用Event log Saver                                           //Steven 20240604 : SPIL格式的event log
//    {
//        elConfig->Add(cbO10_EventLogSaver,  &IniConfig.bO10UseEventLogSaver,            ECBool, "Event Log", "bO10UseEventLogSaver",                    bNoShow, bDisable, bFixedValue, 0);      //Steven 20140902 : 使用外掛小程式
//    }
//    else
    {
        if(IniConfig.bEventLogAutoSaveFunction ||
           CosFunction.bUseMDB)
            elConfig->Add(cbO10_EventLogSaver,  &IniConfig.bO10UseEventLogSaver,        ECBool, "Event Log", "bO10UseEventLogSaver",                    bShow, bEnable, bReadFromFile, bDefaultO10?"1":"0");      //JerryYang 20251013 : fix O10預設值錯誤  //Steven 20140902 : 使用外掛小程式
    }

    elConfig->Add(cbO07,                    &IniConfig.bFTContinueON,           ECBool,     "Count", "bFTContinueON",                                   bShow, bEnable, bReadFromFile, 0);      //kevin FT MODE 不能關連續FAIL

    int iMaxFTContinue=100;
    if(CosFunction.bOEEFunction==true)                                          //Jimmychiu 20231026 : no limited max consecutive faulure count in Greatek
    {
        iMaxFTContinue=99999;
    }

    elConfig->Add(edO07,                &IniConfig.iFTMAXValue,                 ECInteger,  "Count", "iFTMAXValue",                                     bShow, bEnable, bReadFromFile, 5,  false,  2,      iMaxFTContinue);            //kevin 20121008 FT MODE 連續FAIL 最大數量
    elConfig->Add(cbO09,                &IniConfig.bInitialStartNeedAsk,        ECBool,     "Count", "bInitialStartNeedAsk",                            bShow, bEnable, bReadFromFile, 0);      //ChungHung 20130529 add manual change start mode 如果是InitialStart 按下Start時要詢問
    elConfig->Add(cbO11,                &IniConfig.bRecordJamRateByTime,        ECBool,     "Count", "Record Jam Rate By Time",                         bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(edtO11,               &IniConfig.iRecordJamRateIntervalTime,  ECInteger,  "Count", "Record Jam Rate Interval Time",                   bShow, bEnable, bReadFromFile, 1,  false,  1,      999999);     // 2015.11.11 , Joye , Add Jam Rate Record

    if(CosFunction.bUseHeadContactCount==true && CUSTOMER_CODE!=CC_SCC)         //Ifor 20191128 : Fix 銦片 Life Time 功能無顯示
    {
        if((CUSTOMER_CODE==CC_KYEC_LEE && CosFunction.bHiSiliconFunction==true) || //Ifor 20180919 (Steven) : KYEC 要求海思版第一個Life Time Count [O12]功能強制開啟並反灰
           CUSTOMER_CODE==CC_AMD_M)
            elConfig->Add(cbO12_LifeTimeCount,  &IniConfig.bLifeTimeCount[0],   ECBool,     "O_Count", "bLifeTimeCount 1",                              bShow, bDisable, bFixedValue, 1);        //wei 20160509 Life Time Count
        else if(IniConfig.bVTESTFunction)                                       //AI(ht9045-config) 20260508 (RogerYang) : VTEST O12跟隨Recipe(configByRecipe.ini)
            elConfig_byRecipe->Add(cbO12_LifeTimeCount, &IniConfig.bLifeTimeCount[0], ECBool, "O_Count", "bLifeTimeCount 1",                            bShow, bEnable, bReadFromFile, 0);
        else
            elConfig->Add(cbO12_LifeTimeCount,  &IniConfig.bLifeTimeCount[0],   ECBool,     "O_Count", "bLifeTimeCount 1",                              bShow, bEnable, bReadFromFile, 0);

        if(IniConfig.bVTESTFunction)                                            //AI(ht9045-config) 20260508 (RogerYang) : VTEST只用O12，隱藏O13/O14及HeadCondition2/3
        {
            elConfig->Add(cbO13_LifeTimeCount,  &IniConfig.bLifeTimeCount[1],   ECBool,     "O_Count", "bLifeTimeCount 2",                              bNoShow, bDisable, bFixedValue, 0);
            elConfig->Add(cbO14_LifeTimeCount,  &IniConfig.bLifeTimeCount[2],   ECBool,     "O_Count", "bLifeTimeCount 3",                              bNoShow, bDisable, bFixedValue, 0);
            elConfig->Add(edHeadCondition1,     &IniConfig.ContactConditionName[0], ECText,     "O_Count", "O14_ContactConditionName1",                     bShow, bEnable, bReadFromFile, "HeadCondition1");
            elConfig->Add(edHeadCondition2,     &IniConfig.ContactConditionName[1], ECText,     "O_Count", "O15_ContactConditionName2",                     bNoShow, bDisable, bReadFromFile, "HeadCondition2");
            elConfig->Add(edHeadCondition3,     &IniConfig.ContactConditionName[2], ECText,     "O_Count", "O16_ContactConditionName3",                     bNoShow, bDisable, bReadFromFile, "HeadCondition3");
        }
        else
        {
            elConfig->Add(cbO13_LifeTimeCount,      &IniConfig.bLifeTimeCount[1],   ECBool,     "O_Count", "bLifeTimeCount 2",                              bShow, bEnable, bReadFromFile, 0);
            elConfig->Add(cbO14_LifeTimeCount,      &IniConfig.bLifeTimeCount[2],   ECBool,     "O_Count", "bLifeTimeCount 3",                              bShow, bEnable, bReadFromFile, 0);
            elConfig->Add(edHeadCondition1,     &IniConfig.ContactConditionName[0], ECText,     "O_Count", "O14_ContactConditionName1",                     bShow, bEnable, bReadFromFile, "HeadCondition1");
            elConfig->Add(edHeadCondition2,     &IniConfig.ContactConditionName[1], ECText,     "O_Count", "O15_ContactConditionName2",                     bShow, bEnable, bReadFromFile, "HeadCondition2");
            elConfig->Add(edHeadCondition3,     &IniConfig.ContactConditionName[2], ECText,     "O_Count", "O16_ContactConditionName3",                     bShow, bEnable, bReadFromFile, "HeadCondition3");
        }
    }
    else
    {
        elConfig->Add(cbO12_LifeTimeCount,  &IniConfig.bLifeTimeCount[0],       ECBool,     "O_Count", "bLifeTimeCount 1",                              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbO13_LifeTimeCount,  &IniConfig.bLifeTimeCount[1],       ECBool,     "O_Count", "bLifeTimeCount 2",                              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(cbO14_LifeTimeCount,  &IniConfig.bLifeTimeCount[2],       ECBool,     "O_Count", "bLifeTimeCount 3",                              bNoShow, bDisable, bFixedValue, 0);
        elConfig->Add(edHeadCondition1,     &IniConfig.ContactConditionName[0], ECText,     "O_Count", "O14_ContactConditionName1",                     bNoShow, bDisable, bFixedValue, "HeadCondition1");
        elConfig->Add(edHeadCondition2,     &IniConfig.ContactConditionName[1], ECText,     "O_Count", "O15_ContactConditionName2",                     bNoShow, bDisable, bFixedValue, "HeadCondition2");
        elConfig->Add(edHeadCondition3,     &IniConfig.ContactConditionName[2], ECText,     "O_Count", "O16_ContactConditionName3",                     bNoShow, bDisable, bFixedValue, "HeadCondition3");
    }

    elConfig->Add(cbbO15_1,     &IniConfig.iO15_SaveFilePeriod,                 ECInteger,  "Event Log", "iO15_SaveFilePeriod",                         bShow, bEnable, bReadFromFile,  6);     //Steven 20170829 (wei) : Event Log檔案存檔設定
    elConfig->Add(chkO15_2,     &IniConfig.bO15_EventLogFileNameWithMachineID,  ECBool,     "Event Log", "bO15_EventLogFileNameWithMachineID",          bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(chkO15_3,     &IniConfig.bO15_EventLogSaveSameFolder,         ECBool,     "Event Log", "bO15_EventLogSaveSameFolder",                 bShow, bEnable, bReadFromFile, 0);       //KaiChen 20180322 ：矽格-湖口 新增 將 Event Log 放在同個資料夾

    if(CosFunction.bConAlarmNeedKeyInPassword)
    {
        elConfig->Add(chkO16,   &IniConfig.bO16ConAlarmNeedKeyInPasswordCT,     ECBool,     "Count", "bO16ConAlarmNeedKeyInPasswordCT",                 bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edtO16,   &IniConfig.iO16ConAlarmNeedKeyInPasswordCT,     ECInteger,  "Count", "Con Alarm Need KeyIn Password CT",                bShow, bEnable, bReadFromFile,  3,     false,  1,      100);
    }
    else
    {
        elConfig->Add(chkO16,   &IniConfig.bO16ConAlarmNeedKeyInPasswordCT,     ECBool,     "Count", "bO16ConAlarmNeedKeyInPasswordCT",                 bNoShow, bDisable, bFixedValue, 0);
        edtO16->Visible=false;
    }

    if(CosFunction.bConAlarmInTimeLevelUp)                                      //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
    {
        elConfig->Add(chkO17,       &IniConfig.bO17EnableLevelUpWhenContiAlarm, ECBool,     "Count", "bO17EnableLevelUpWhenContiAlarm",                 bShow, bEnable, bReadFromFile, 0);
        elConfig->Add(edtO17_Count, &IniConfig.iO17LevelUpWhenContiAlarmCount,  ECInteger,  "Count", "iO17LevelUpWhenContiAlarmCount",                  bShow, bEnable, bReadFromFile,  3,     false,  1,      100);            //Steven 20210127 : 連續alarm次數
        elConfig->Add(edtO17_Time,  &IniConfig.iO17LevelUpWhenContiAlarmTime,   ECInteger,  "Count", "iO17LevelUpWhenContiAlarmTime",                   bShow, bEnable, bReadFromFile,  30,    false,  1,      100000);         //Steven 20210127 : 連續alarm時間
    }
    else
    {
        elConfig->Add(chkO17,       &IniConfig.bO17EnableLevelUpWhenContiAlarm, ECBool,     "Count", "bO17EnableLevelUpWhenContiAlarm",                 bNoShow, bDisable, bFixedValue, 0);
        edtO17_Count->Visible=false;
        edtO17_Time->Visible=false;
        lblO17->Visible=false;
    }

    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        elConfig->Add(cbO18,        &IniConfig.bO18SafeDoorOnOffDurationDetect, ECBool,     "Count", "bO18SafeDoorOnOffDurationDetect",                 bShow, bEnable, bReadFromFile, 1);
        elConfig->Add(edtO18,       &IniConfig.iO18SafeDoorOnOffDurationHour,   ECInteger,  "Count", "iO18SafeDoorOnOffDurationHour",                   bShow, bEnable, bReadFromFile,  168,     false,  1,      168);           //JerryYang 20210112 : 安全門檢查機制,長時間未開啟要跳出alarm
    }
    else
    {
        elConfig->Add(cbO18,        &IniConfig.bO18SafeDoorOnOffDurationDetect, ECBool,     "Count", "bO18SafeDoorOnOffDurationDetect",                 bNoShow, bDisable, bFixedValue, 0);
        edtO18->Visible=false;
    }

    elConfig->Add(cbO19_1,      &IniConfig.bO19_AutoRecordReportByEveryDay,     ECBool,     "Event Log", "bO19_AutoRecordReportByEveryDay",             !IniConfig.bVTESTFunction, bEnable, bReadFromFile, 0);      //RogerYang 20251104 : 偉測MTBF文件生成，只設定week
    elConfig->Add(cbO19_2,      &IniConfig.bO19_AutoRecordReportByEveryWeek,    ECBool,     "Event Log", "bO19_AutoRecordReportByEveryWeek",            bShow, bEnable, bReadFromFile, 0);
    elConfig->Add(coO19_3,      &IniConfig.iO19_WeekPeriod,                     ECInteger,  "Event Log", "iO19_WeekPeriod",                             bShow, bEnable, bReadFromFile, 0);      //Sam 20210107 : Summary Report fuction
    elConfig->Add(coO19_5,      &IniConfig.iO19_MonthPeriod,                    ECInteger,  "Event Log", "iO19_MonthPeriod",                            !IniConfig.bVTESTFunction, bEnable, bReadFromFile, 0);      //RogerYang 20251104 : 偉測MTBF文件生成，只設定week //Sam 20210107 : Summary Report fuction
    elConfig->Add(cbO19_4,      &IniConfig.bO19_AutoRecordReportByEveryMonth,   ECBool,     "Event Log", "bO19_AutoRecordReportByEveryMonth",           !IniConfig.bVTESTFunction, bEnable, bReadFromFile, 0);      //RogerYang 20251104 : 偉測MTBF文件生成，只設定week
    elConfig->Add(edtO19_6,     &IniConfig.asO19_SavePath,                      ECText,     "Event Log", "asO19_SavePath",                              bShow, bEnable, bReadFromFile, "D:\\HT9045_Log\\Product_Loader");       //Sam 20210107 : Summary Report fuction
    elConfig->Add(cbO20,        &IniConfig.bO20InOutArmPickerLifeTimeCount,     ECBool,     "O_Count", "bO20InOutArmPickerLifeTimeCount",               bShow, bEnable, bReadFromFile, 0);      //wei 20160509 Life Time Count

    elConfig->Add(cbO20_1,      &IniConfig.bO20_1ClearLifeTimeWhenInitialStart, ECBool,     "O_Count", "bO20_1ClearLifeTimeWhenInitialStart",           bShow, bEnable, bReadFromFile, 0);      //Steven 20240531 : initial start的時候要清除資料

    elConfig->Add(cbO21,        &IniConfig.bO21FTAfterTrayEndClearFailBinCount, ECBool,     "O_Count", "bO21FTAfterTrayEndClearFailBinCount",           bShow, bEnable, bReadFromFile, 0);      //Frank 20241114 : Add
    elConfig->Add(chkO22,       &IniConfig.bO22_ClearSortCntByDoubleClick,      ECBool,     "O_Count", "bO22_ClearSortCntByDoubleClick",                bShow, bEnable, bReadFromFile, 1);      //Steven 20241206 : 點兩下可以清除數量

    if(CUSTOMER_CODE==CC_JCET ||
       CUSTOMER_CODE==CC_SCC)                                                   //Steven 20250428 : add SCC
        elConfig->Add(chkO23,   &IniConfig.bO23_InputLotIDByBarcode,            ECBool,     "O_Count", "bO23_InputLotIDByBarcode",                      bShow, bEnable, bReadFromFile, 1);      //Steven 20241224 : LotID只能用Barcode
    else
        elConfig->Add(chkO23,   &IniConfig.bO23_InputLotIDByBarcode,            ECBool,     "O_Count", "bO23_InputLotIDByBarcode",                      bNoShow, bDisable, bFixedValue, 0);

    if(CUSTOMER_CODE==CC_JCET ||                                                //Steven 20250519 : Production Log By Lot
       CUSTOMER_CODE==CC_SCC ||
       CUSTOMER_CODE==CC_JSI_HAOXING)                                           //Frank 20241114 : Add
        elConfig->Add(chkO24,   &IniConfig.bO24_ProductionLogByLot,             ECBool,     "O_Count", "bO24_ProductionLogByLot",                       bShow, bEnable, bReadFromFile, 1);
    else
        elConfig->Add(chkO24,   &IniConfig.bO24_ProductionLogByLot,             ECBool,     "O_Count", "bO24_ProductionLogByLot",                       bShow, bEnable, bReadFromFile, 0);
}
