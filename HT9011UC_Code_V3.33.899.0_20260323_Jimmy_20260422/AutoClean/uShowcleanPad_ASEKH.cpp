#include "MachineDefine.h"
#pragma hdrstop

#include "uShowcleanPad_ASEKH.h"
#include "cprod.h"
#include "common.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCleanPadFrom *CleanPadFrom;
//---------------------------------------------------------------------------
__fastcall TCleanPadFrom::TCleanPadFrom(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCleanPadFrom::FormShow(TObject *Sender)
{
    CleanPadFrom->Top=164;
    CleanPadFrom->Left=387;
    AnsiString BmpPath ="D:\\HT9045\\BMP\\";
    if(HotPlateForm.XDivision==6)
    {
        switch(TestIF_File.iTestMode)
        {
            case DualSite: //1x2
//            case DualSiteBS: //1x1 busyshuttle
                Image1->Picture->LoadFromFile(BmpPath+"1_2-8.bmp");   //kevin 20160304
                break;
//            case QualSite2X2BS: //2x2 busyshuttle
            case QualSite2X2: //2x2
                Image1->Picture->LoadFromFile(BmpPath+"2_2-8.bmp");   //kevin 20160304
                break;

            case QualSite1X4: //1x4
            case _8Site1X4:  //ChungHung 20150528 add for 海思 _8Site1x4
               // Image1->Picture->LoadFromFile(BmpPath+"4-6.bmp");
                Image1->Picture->LoadFromFile(BmpPath+"1_4-8.bmp");   //kevin 20160304
                break;
            case _8Site2X4: //2x4
                Image1->Picture->LoadFromFile(BmpPath+"8-6.bmp");
                break;
            case _12Site2X6: //2x6  kevin 20151223 add
                Image1->Picture->LoadFromFile(BmpPath+"12-6.bmp");
                break;
            case _16Site4X4: //Sam 20190226 : 16Site4X4
            case _16Site2X8: //2x8
                Image1->Picture->LoadFromFile(BmpPath+"16-6.bmp");
                break;
        }
    }
    else
    {
        switch(TestIF_File.iTestMode)
        {
            case DualSite: //1x2
//            case DualSiteBS: //1x1 busyshuttle
                break;
            case QualSite2X2: //2x2
//            case QualSite2X2BS: //2x2 busyshuttle
                Image1->Picture->LoadFromFile(BmpPath+"2_2-8.bmp");   //kevin 20160304
                break;
            case QualSite1X4: //1x4
            case _8Site1X4: //ChungHung 20150528 add for 海思 _8Site1x4
                Image1->Picture->LoadFromFile(BmpPath+"1_4-8.bmp");   //kevin 20160304
                break;
            case _8Site2X4: //2x4
                Image1->Picture->LoadFromFile(BmpPath+"8-8.bmp");
                break;
            case _12Site2X6: //2x6  kevin 20151223 add
                Image1->Picture->LoadFromFile(BmpPath+"12-8.bmp");
                break;
            case _16Site4X4: //Sam 20190226 : 16Site4X4
            case _16Site2X8: //2x8
                Image1->Picture->LoadFromFile(BmpPath+"16-8.bmp");
                break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TCleanPadFrom::SpeedButton1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TCleanPadFrom::FormDestroy(TObject *Sender)
{
    try
    {
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TCleanPadFrom::FormDestroy");
    }
    LogSoftwareOffTime("TCleanPadFrom, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------

