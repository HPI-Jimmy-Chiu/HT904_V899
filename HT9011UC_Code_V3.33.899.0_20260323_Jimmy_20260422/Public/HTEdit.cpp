#include "MachineDefine.h"
#pragma hdrstop

#include "HTEdit.h"

#include "myQwertyKeyBoard.h"
#include "cMydef.h"
#include "BarcodeReader.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall THTEdit::THTEdit()
{
    DisablePopupMenu=new TPopupMenu(NULL);
    HTCheckRange=true;
    iBarcodeReadType=bcTotal;
}
//---------------------------------------------------------------------------
__fastcall THTEdit::~THTEdit()
{
    try
    {
        TEdit        *Ed = dynamic_cast <TEdit          *>(SourceControl);
        TLabeledEdit *LEd= dynamic_cast <TLabeledEdit   *>(SourceControl);      //Steven 20230224 : Edit List改支援多型態元件
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
void THTEdit::OnWriteEdit(TCustomEdit *_SourceEdit)
{
    SourceControl                 =_SourceEdit;

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
                if(iTransformType!=EUNone)                                      //Steven 20230905 : HTEditList加入型態轉換的Flag
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
                if(iTransformType!=EUNone)                                      //Steven 20230905 : HTEditList加入型態轉換的Flag
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
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetContentType(TEditContent P)
{
    HTContentType=P;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetDefaultValue(AnsiString P)
{
    HTDefaultValue=P;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetCheckRange(bool P)
{
    HTCheckRange=P;
}
//---------------------------------------------------------------------------
//void __fastcall THTEdit::SetFileName(AnsiString P)
//{
//    HTFileName=P;
//}
//---------------------------------------------------------------------------
//void __fastcall THTEdit::SetPath(AnsiString P)
//{
//    HTPath=P;
//}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetIniGroupName(AnsiString P)
{
    HTIniGroupName=P;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetIniKeyName(AnsiString P)
{
    HTIniKeyName=P;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::EditIntKeyPress(TObject *Sender, char &Key)
{

}
//---------------------------------------------------------------------------
void __fastcall THTEdit::EditFloatKeyPress(TObject *Sender, char &Key)
{

}
//---------------------------------------------------------------------------
void __fastcall THTEdit::EditPathKeyPress(TObject *Sender, char &Key)
{

}
//---------------------------------------------------------------------------
void THTEdit::ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, AnsiString Default)
{
    bVisible        =Visible;
    bEnable         =Enable;
    bReadFromFile   =ReadFromFile;
    DefaultValue    =Default;
}
//---------------------------------------------------------------------------
void THTEdit::ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, bool Default)
{
    bVisible        =Visible;
    bEnable         =Enable;
    bReadFromFile   =ReadFromFile;
    DefaultValue    =Default?"1":"0";
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::EditClick(TObject *Sender)
{
    if(iBarcodeReadType!=bcTotal)
        Barcode_Reader(iBarcodeReadType);

    if(fQwertyKey==NULL)
        Application->CreateForm(__classid(TfQwertyKey), &fQwertyKey);
    if(fQwertyKey2==NULL)
        Application->CreateForm(__classid(TfQwertyKey), &fQwertyKey2);          //Steven 20150505 : 剛好在輸入時,Alarm會卡死

    if(HTContentType==ECText)                                                   //純文字
    {
        fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_NO_SYMBOL|N_NO_SPACE);
    }
    else if(HTContentType==ECInteger || HTContentType==ECPosInt || HTContentType==ECNegInt || HTContentType==ECPort)        //數字
    {
        if(iTransformType==EUuMToMM)                                            //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else if(iTransformType==EUMSToSec)                                      //顯示為Sec, 存檔為MS
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_INTEGER, 0, CheckRange, atoi(HTCustomMax.c_str()), atoi(HTCustomMin.c_str()));
    }
    else if(HTContentType==ECDouble || HTContentType==ECPosDouble || HTContentType==ECNegDouble || HTContentType==ECPercent)         //浮點數
    {
        if(iTransformType==EUuMToMM)                                            //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else if(iTransformType==EUMSToSec)                                      //顯示為Sec, 存檔為MS
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
        else
            fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_DOUBLE, 3, CheckRange, atof(HTCustomMax.c_str()), atof(HTCustomMin.c_str()));
    }
    else if(HTContentType==ECIPAddr)
    {
        fQwertyKey->ShowQwertyKey((TEdit*)SourceControl, N_IP_ADDR);
    }
    else if(HTContentType==ECFileName)                                          //檔案名稱或路徑
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
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::DisableEventOverlap()                                  //取消覆蓋 OnClickEvent,也就是使用Edit本身的OnClickEvent
{
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
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::EditChange(TObject *Sender)
{
    if(HTCheckRange)                                                            //Steven 20190826 : 修正參數範圍比對
    {
        if(Content==ECInteger    ||                                             //只存入有數值的
           Content==ECDouble     ||
           Content==ECPosInt     ||
           Content==ECPosDouble  ||
           Content==ECNegInt     ||
           Content==ECNegDouble  ||
           Content==ECPort)                                                     //Steven 20230221 : 小鍵盤新增通訊埠上下限
        {
            TCustomEdit  *CEd = dynamic_cast <TCustomEdit *>(SourceControl);    //Steven 20230224 : Edit List改支援多型態元件
            if(CEd!=NULL)
            {
                double dValue=atof(CEd->Text.c_str());
                double dMax  =atof(HTCustomMax.c_str());
                double dMin  =atof(HTCustomMin.c_str());

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

    if(FOldOnChange!=NULL)
    {
        FOldOnChange(this);
    }
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetCustomMax(AnsiString Max)
{
    if(HTContentType==ECPosInt || HTContentType==ECPosDouble)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;
    }
    else if(HTContentType==ECNegInt || HTContentType==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;
    }
    else if(HTContentType==ECPort)  //Steven 20230221 : 小鍵盤新增通訊埠上下限
    {
        if(atof(Max.c_str())>65535)
            Max=65535;
    }
    else if(HTContentType==ECPercent)
    {
        if(atof(Max.c_str())>100.0)
            Max=100.0;

        if(atof(Max.c_str())<0.0)
            Max=0.0;
    }

    if(HTContentType==ECBool)
    {
        Max=1;
    }
    else
    {
        if(atof(Max.c_str())>HTMax)
            Max=HTMax;
    }

    HTCustomMax=Max;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetCustomMin(AnsiString Min)
{
    if(HTContentType==ECPosInt || HTContentType==ECPosDouble)
    {
        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(HTContentType==ECNegInt || HTContentType==ECNegDouble)
    {
        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(HTContentType==ECPort)
    {
        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(HTContentType==ECPercent)
    {
        if(atof(Min.c_str())>100.0)
            Min=100.0;

        if(atof(Min.c_str())<0.0)
            Min=0.0;
    }

    if(HTContentType==ECBool)
    {
        Min=0;
    }
    else
    {
        if(atof(Min.c_str())<HTMin)
            Min=HTMin;
    }

    HTCustomMin=Min;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetDefaultLimitation(AnsiString Max, AnsiString Min)
{
    if(HTContentType==ECPosInt || HTContentType==ECPosDouble || HTContentType==ECPercent)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;

        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(HTContentType==ECPort)
    {
        if(atof(Max.c_str())>65535)
            Max=65535;

        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(HTContentType==ECNegInt || HTContentType==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;

        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(HTContentType==ECPercent)
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
    else if(HTContentType==ECBool)
    {
        Max=1;
        Min=0;
    }


    if(atof(Max.c_str())<atof(Min.c_str()))
    {
        HTMax=Min;
        HTMin=Max;
        HTCustomMax=Min;
        HTCustomMin=Max;
    }
    else
    {
        HTMax=Max;
        HTMin=Min;
        HTCustomMax=Max;
        HTCustomMin=Min;
    }
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetMax(AnsiString Max)
{
    if(HTContentType==ECPosInt || HTContentType==ECPosDouble)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;
    }
    else if(HTContentType==ECNegInt || HTContentType==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;
    }
    else if(HTContentType==ECPort)
    {
        if(atof(Max.c_str())>65535)
            Max=65535;
    }
    else if(HTContentType==ECPercent)
    {
        if(atof(Max.c_str())>100.0)
            Max=100.0;

        if(atof(Max.c_str())<0.0)
            Max=0.0;
    }
    else if(HTContentType==ECBool)
    {
        Max=1;
    }

    HTMax=Max;
    HTCustomMax=Max;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetMin(AnsiString Min)
{
    if(HTContentType==ECPosInt || HTContentType==ECPosDouble)
    {
        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(HTContentType==ECNegInt || HTContentType==ECNegDouble)
    {
        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(HTContentType==ECPort)
    {
        if(atof(Min.c_str())<0)
            Min=0;

        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(HTContentType==ECPercent)
    {
        if(atof(Min.c_str())>100.0)
            Min=100.0;

        if(atof(Min.c_str())<0.0)
            Min=0.0;
    }
    else if(HTContentType==ECBool)
    {
        Min=0;
    }

    HTMin=Min;
    HTCustomMin=Min;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetCustomLimitation(AnsiString Max, AnsiString Min)
{
    if(HTContentType==ECPosInt || HTContentType==ECPosDouble)
    {
        if(atof(Max.c_str())<0.0)
            Max=0;

        if(atof(Min.c_str())<0.0)
            Min=0;
    }
    else if(HTContentType==ECNegInt || HTContentType==ECNegDouble)
    {
        if(atof(Max.c_str())>0.0)
            Max=0;

        if(atof(Min.c_str())>0.0)
            Min=0;
    }
    else if(HTContentType==ECPort)
    {
        if(atof(Max.c_str())>65535)
            Max=65535;

        if(atof(Min.c_str())<0)
            Min=0;
    }
    else if(HTContentType==ECPercent)
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
    else if(HTContentType==ECBool)
    {
        Max=1;
        Min=0;
    }

    if(atof(Max.c_str())>HTMax)
        Max=HTMax;

    if(atof(Min.c_str())<HTMin)
        Min=HTMin;

    if(atof(Max.c_str())<atof(Min.c_str()))
    {
        HTCustomMax=Min;
        HTCustomMin=Max;
    }
    else
    {
        HTCustomMax=Max;
        HTCustomMin=Min;
    }
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetFileInformation(AnsiString GroupName, AnsiString KeyName)
{
    HTIniGroupName  =GroupName;
    HTIniKeyName    =KeyName;
}
//---------------------------------------------------------------------------
Variant __fastcall THTEdit::ReadParamter()
{
    return FParemeter;
}
//---------------------------------------------------------------------------
void __fastcall THTEdit::SetParamter(Variant VData)
{
    FParemeter=VData;
}
//---------------------------------------------------------------------------
void THTEdit::GetDefaultPosition()
{
    Par     =SourceControl->Parent;
    iTop    =SourceControl->Top;
    iLeft   =SourceControl->Left;
    iHeight =SourceControl->Height;
    iWidth  =SourceControl->Width;
}
//---------------------------------------------------------------------------
void THTEdit::SetToDefaultPosition()
{
    if(SourceControl->Parent!=Null && Par!=NULL && SourceControl->Parent!=Par)
    {
        SourceControl->Align =alNone;
        SourceControl->Parent=Par;
        SourceControl->Top   =iTop;
        SourceControl->Left  =iLeft;
        SourceControl->Height=iHeight;
        SourceControl->Width =iWidth;
    }
}
//---------------------------------------------------------------------------
//__fastcall THTCheckBox::THTCheckBox()
//{
//    Par=NULL;
//}
//---------------------------------------------------------------------------
//__fastcall THTCheckBox::~THTCheckBox()
//{
//
//}
//---------------------------------------------------------------------------
//void __fastcall THTCheckBox::SetFileInformation(AnsiString GroupName, AnsiString KeyName)
//{
//    HTIniGroupName  =GroupName;
//    HTIniKeyName    =KeyName;
//}
//---------------------------------------------------------------------------
//void __fastcall THTCheckBox::SetIniGroupName(AnsiString P)
//{
//    HTIniGroupName=P;
//}
//---------------------------------------------------------------------------
//void __fastcall THTCheckBox::SetIniKeyName(AnsiString P)
//{
//    HTIniKeyName=P;
//}
//---------------------------------------------------------------------------
//void __fastcall THTCheckBox::SetDefaultValue(bool P)
//{
//    HTDefaultValue=P;
//}
//---------------------------------------------------------------------------
//bool __fastcall THTCheckBox::ReadParamter()
//{
//    return FParemeter;
//}
//---------------------------------------------------------------------------
//void __fastcall THTCheckBox::SetParamter(bool VData)
//{
//    FParemeter=VData;
//}
//---------------------------------------------------------------------------
//void THTCheckBox::ChangeProperty(bool Visible, bool Enable, bool ReadFromFile, bool ForceValue)
//{
//    bVisible=Visible;
//    bEnable=Enable;
//    bReadFromFile=ReadFromFile;
//    bForceValue=ForceValue;
//}
//---------------------------------------------------------------------------
//void THTCheckBox::GetDefaultPosition()  //Steven 20210730 : 快速搜尋Config
//{
//    Par     =CheckBox->Parent;
//    iTop    =CheckBox->Top;
//    iLeft   =CheckBox->Left;
//    iHeight =CheckBox->Height;
//    iWidth  =CheckBox->Width;
//}
//---------------------------------------------------------------------------
//void THTCheckBox::SetToDefaultPosition()    //Steven 20210730 : 快速搜尋Config
//{
//    if(CheckBox->Parent!=Null && Par!=NULL && CheckBox->Parent!=Par)
//    {
//        CheckBox->Align =alNone;
//        CheckBox->Parent=Par;
//        CheckBox->Top   =iTop;
//        CheckBox->Left  =iLeft;
//        CheckBox->Height=iHeight;
//        CheckBox->Width =iWidth;
//    }
//}
//---------------------------------------------------------------------------
