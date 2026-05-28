#include "MachineDefine.h"
#pragma hdrstop

#include "cCounterSel.h"

#include "cprod.h"
#include "cpublic.h"

#include "cMyDef.h"
#include "main.h"
#include "common.h"
#include "cSortCT.h"
#include "cTestCategory.h"
#include "cShowBinSelect.h"
#include "cContactCT.h"
#include "uLotInfo.h"
#include "uShowMessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfCounterSel *fCounterSel;
//---------------------------------------------------------------------------
__fastcall TfCounterSel::TfCounterSel(TComponent* Owner)
    : TForm(Owner)
{
    NeedRef=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCounterSel::FormShow(TObject *Sender)
{
    ProcessLastSetIni_Visible(bReadFile);
    cbUPH->Checked              = IniConfig.bShowUPH;
    cbIndexTime->Checked        = IniConfig.bShowIndexTime;
    cbContactHeight->Checked    = IniConfig.bShowContactHeight;   //Steven 20100827
    cbCycleTime->Checked        = IniConfig.bShowTimeInfo;        //Steven 20100827
    rbLoadingCount_On->Checked  = IniConfig.bShowLoaderCT;
    rbLoadingCount_Off->Checked =!IniConfig.bShowLoaderCT;
    rbContactCount_On->Checked  = IniConfig.bShowContactCT;
    rbContactCount_Off->Checked =!IniConfig.bShowContactCT;
    rbTestCategory_On->Checked  = IniConfig.bShowTestCate;
    rbTestCategory_Off->Checked =!IniConfig.bShowTestCate;
    rgTestCategory->ItemIndex   = IniConfig.iShowCateByArm;
    rbScanner_On->Checked       = IniConfig.bShowScanCate;
    rbScanner_Off->Checked      =!IniConfig.bShowScanCate;
    rbTemperature_On->Checked   = IniConfig.bShowTemper;
    rbTemperature_Off->Checked  =!IniConfig.bShowTemper;
    rbBinAssign_On->Checked     = IniConfig.bShowBinCT;
    rbBinAssign_Off->Checked    =!IniConfig.bShowBinCT;

    NeedRef=false;
    Top=20;
    Left=250;
    fShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfCounterSel::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    IniConfig.bShowUPH          =cbUPH->Checked;
    IniConfig.bShowIndexTime    =cbIndexTime->Checked;
    IniConfig.bShowTimeInfo     =cbCycleTime->Checked;
    IniConfig.bShowContactHeight=cbContactHeight->Checked;
    IniConfig.bShowLoaderCT     =rbLoadingCount_On->Checked;
    IniConfig.bShowContactCT    =rbContactCount_On->Checked;
    IniConfig.bShowScanCate     =rbScanner_On->Checked;
    IniConfig.bShowTemper       =rbTemperature_On->Checked;
    IniConfig.bShowTestCate     =rbTestCategory_On->Checked;
    IniConfig.bShowBinCT        =rbBinAssign_On->Checked;
    IniConfig.iShowCateByArm    =rgTestCategory->ItemIndex;

    //Steven 20100827 Start
    if(IniConfig.bShowIndexTime==false)
        fMain->StatusBar1->Panels->Items[0]->Text="";
    fShowMessage->lblIndexCycleTime->Visible=IniConfig.bShowTimeInfo;
    fShowMessage->lblTestTime->Visible=IniConfig.bShowTimeInfo;

    fShowBinSelect->Tab_UPH->TabVisible=IniConfig.bShowUPH;

    if(IniConfig.bShowContactHeight==false)
    {
        fMain->lbArm0Torque->Caption="";
        fMain->lbArm0Torque->Caption="";
    }
    //Steven 20100827 End

    //jou 980721 start : Return Default Value
    if(cbDefaultValue->Checked)
    {
        //Steven 20090811 : Use INI file to define form position
        cbDefaultValue->Checked=false;
        AnsiString szDir=fCounterSel->CheckFormIni(AuthPath, "FormPos.def");
        ReplaceIniData(szDir, "fMain",            "X", "defaultX");
        ReplaceIniData(szDir, "fMain",            "Y", "defaultY");
        ReplaceIniData(szDir, "fSortCT",          "X", "defaultX");
        ReplaceIniData(szDir, "fSortCT",          "Y", "defaultY");
        ReplaceIniData(szDir, "fTestCategory",    "X", "defaultX");
        ReplaceIniData(szDir, "fTestCategory",    "Y", "defaultY");
        ReplaceIniData(szDir, "fShowBinSelect",   "X", "defaultX");
        ReplaceIniData(szDir, "fShowBinSelect",   "Y", "defaultY");
        ReplaceIniData(szDir, "fContactCT",       "X", "defaultX");
        ReplaceIniData(szDir, "fContactCT",       "Y", "defaultY");
        ReplaceIniData(szDir, "fLotInfo",         "X", "defaultX");
        ReplaceIniData(szDir, "fLotInfo",         "Y", "defaultY");
    }
    //jou 980721 end
    ProcessLastSetIni_Visible(bWriteFile);
    NeedRef=true;
    fShow=false;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfCounterSel::CheckFormIni(AnsiString szDir, AnsiString str)
{
    MyForceDirectories(szDir);
    szDir+=str;
    if(!FileExists(szDir))
    {
        WriteIniData(szDir, "fMain",            "X",   0);
        WriteIniData(szDir, "fMain",            "Y",   0);
        WriteIniData(szDir, "fSortCT",          "X", 756);
        WriteIniData(szDir, "fSortCT",          "Y",   0);
        WriteIniData(szDir, "fTestCategory",    "X", 756);
        WriteIniData(szDir, "fTestCategory",    "Y", 284);
        WriteIniData(szDir, "fShowBinSelect",   "X", 756);
        WriteIniData(szDir, "fShowBinSelect",   "Y", 476);
        WriteIniData(szDir, "fContactCT",       "X", 463);
        WriteIniData(szDir, "fContactCT",       "Y", 554);
        WriteIniData(szDir, "fLotInfo",         "X", 201);
        WriteIniData(szDir, "fLotInfo",         "Y", 554);
        WriteIniData(szDir, "fMain",            "defaultX",   0);
        WriteIniData(szDir, "fMain",            "defaultY",   0);
        WriteIniData(szDir, "fSortCT",          "defaultX", 756);
        WriteIniData(szDir, "fSortCT",          "defaultY",   0);
        WriteIniData(szDir, "fTestCategory",    "defaultX", 756);
        WriteIniData(szDir, "fTestCategory",    "defaultY", 284);
        WriteIniData(szDir, "fShowBinSelect",   "defaultX", 756);
        WriteIniData(szDir, "fShowBinSelect",   "defaultY", 476);
        WriteIniData(szDir, "fContactCT",       "defaultX", 463);
        WriteIniData(szDir, "fContactCT",       "defaultY", 554);
        WriteIniData(szDir, "fLotInfo",         "defaultX", 201);
        WriteIniData(szDir, "fLotInfo",         "defaultY", 554);
    }
    return szDir;
}
//---------------------------------------------------------------------------
void __fastcall TfCounterSel::spbExitClick(TObject *Sender)
{
    spbExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
