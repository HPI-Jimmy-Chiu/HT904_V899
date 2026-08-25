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
