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
