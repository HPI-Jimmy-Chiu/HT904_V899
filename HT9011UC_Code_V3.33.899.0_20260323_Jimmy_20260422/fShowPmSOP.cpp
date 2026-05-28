//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "fShowPmSOP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TfShowPmSOPForm *fShowPmSOPForm;
//---------------------------------------------------------------------------
__fastcall TfShowPmSOPForm::TfShowPmSOPForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
AnsiString sPathPmSOP = "";     //KaiChen 20171111 ：超豐 開啟指定路徑的 HTML 檔案(PM SOP)
//---------------------------------------------------------------------------

//KaiChen 20171111 ：超豐 開啟指定路徑的 HTML 檔案(PM SOP)
//==>
void __fastcall TfShowPmSOPForm::FormShow(TObject *Sender)
{
    cpwbrwsrPmSOP->Refresh();

    WideString URL(sPathPmSOP);
    cpwbrwsrPmSOP->Navigate(URL);

}
//<==
//KaiChen 20171111 ：超豐 開啟指定路徑的 HTML 檔案(PM SOP)

//---------------------------------------------------------------------------
