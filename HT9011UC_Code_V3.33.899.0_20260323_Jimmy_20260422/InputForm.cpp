#include "MachineDefine.h"
#pragma hdrstop

#include "INPUT.h"
#include "InputForm.h"
#include "common.h"
#include "cmydef.h"
#include "cprod.h"
#include "OCR.h"
#include "cMyDB.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfInputForm *fInputForm;

AnsiString sResult="";
//---------------------------------------------------------------------------
__fastcall TfInputForm::TfInputForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfInputForm::Button1Click(TObject *Sender)
{
    if(bTrayIDclose==false &&
       ((fInputForm->Edit1->Text!="" && fInputForm->Edit1->Text!=0) ||
        (TestIF.bOcrFunction && CosFunction.bTrayOCR)))
    {
        Close();
    }

    if(bTrayIDclose==true && fInputForm->Edit3->Text!="")
    {
        bTrayIDclose=false;
        Close();
    }

    if(bAutoRetestJam)      //wei 20160302 Jam Skip輸入顆數
    {
        if(fInputForm->Edit3->Text!="" && ((atoi(fInputForm->Edit3->Text.c_str())<=iJamSkipICCount && atoi(fInputForm->Edit3->Text.c_str())>0) || (fInputForm->Edit3->Text=="NA")))
            Close();
        else if(CUSTOMER_CODE==CC_KYEC_LEE && bNeedKeyInSkipIC)
        {
            iJamSkipICCount=atoi(fInputForm->Edit3->Text.c_str());
            Close();
        }
        else
        {
            fInputForm->Edit3->Text=iJamSkipICCount;
        }
    }
}
//---------------------------------------------------------------------------
int ShowMyInput(AnsiString aCaption,AnsiString aString)
{
    fInputForm->Height=156;
    fInputForm->Width=395;
    fInputForm->Edit2->Visible=false;
    fInputForm->Edit1->Visible=true;
    fInputForm->Edit3->Visible=false;
    fInputForm->Edit1->Text="";
    fInputForm->Caption=aCaption;
    fInputForm->Label1->Caption=aString;

    fInputForm->ShowModal();

    return atoi(fInputForm->Edit1->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfInputForm::Edit1Click(TObject *Sender)
{
    MyInputBox((TEdit*)Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfInputForm::Edit1KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
AnsiString ShowMyInput1(AnsiString aCaption,AnsiString aString)
{
    int i=0;
    fInputForm->Height=918;
    fInputForm->Width=888;
    fInputForm->Edit1->Visible=false;
    fInputForm->Edit2->Visible=true;
    fInputForm->Edit3->Visible=false;

    if(bOCRNoIC==false)
        asBarCode.Delete(1,1);
    fInputForm->Edit2->Text=asBarCode.c_str();
    fInputForm->Caption=aCaption;
    fInputForm->Label1->Caption=aString;

    fInputForm->ShowModal();

    i=fInputForm->Edit2->Text.Pos("?");
    if((fInputForm->Edit2->Text!="" &&
        fInputForm->Edit2->Text.Length()==IniConfig.iOCRWordCount && i==0) ||
       fInputForm->Edit2->Text.UpperCase()=="NOIC")
    {
        if(fInputForm->Edit2->Text.UpperCase()!="NOIC")   //wei 20161128 (Steven) 確認字串各自Type是否正確
        {
            if(fOCR->CheckOCRWordType(fInputForm->Edit2->Text)==false)
            {
                return "ERROR";
            }
            else
            {
                return fInputForm->Edit2->Text.c_str();
            }
        }
        else
        {
            return fInputForm->Edit2->Text.c_str();
        }
    }
    return "";  //Steven 20160108 : for reduce warning
}
//---------------------------------------------------------------------------
AnsiString ShowMyInputSkip(AnsiString aCaption,AnsiString aString)  //wei 20160302 Jam Skip輸入顆數
{
    AnsiString Result="NA";

    fInputForm->Edit2->Visible=false;
    fInputForm->Edit1->Visible=false;
    fInputForm->Edit3->Visible=true;
    fInputForm->Edit3->Text=iJamSkipICCount;
    fInputForm->Caption=aCaption;
    fInputForm->Label1->Caption=aString;

    fInputForm->ShowModal();
    if(fInputForm->Edit3->Text!="")
    {
        iJamSkipIC=fInputForm->Edit3->Text;
        Result=fInputForm->Edit3->Text;
    }
    return Result;
}
//---------------------------------------------------------------------------
AnsiString ShowMyInputTrayID(AnsiString aCaption,AnsiString aString)
{
    fInputForm->bTrayIDclose=true;
    fInputForm->Height=156;
    fInputForm->Width=395;
    fInputForm->Edit1->Visible=false;
    fInputForm->Edit2->Visible=false;
    fInputForm->Edit3->Visible=true;
    fInputForm->Edit3->Text="";
    fInputForm->Caption=aCaption;
    fInputForm->Label1->Caption=aString;

    fInputForm->ShowModal();

    return fInputForm->Edit3->Text;
}
//---------------------------------------------------------------------------
