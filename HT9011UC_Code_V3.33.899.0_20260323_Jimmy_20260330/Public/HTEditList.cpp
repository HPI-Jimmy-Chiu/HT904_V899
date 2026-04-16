//---------------------------------------------------------------------------
//Steven 20170629 (jou) : 第一次使用HTEditList在HT-9045裡面
//---------------------------------------------------------------------------
// HTEditList需搭配HTEdit使用
// 為了節省存檔與讀檔時間, 每個INI檔案需要帶一個HTEditList,
//
// 新增元件與變數: HTEditList->Add( Edit元件,
//                                  參數指標,
//                                  參數型態,               //參考 HTEdit.h裡面的 TEditContent
//                                  IniGroupName,           //給空值的話,預設使用FormName
//                                  IniKeyName,             //給空值的話,預設使用元件Name
//                                  DefaultValue,
//                                  DisableEventOverlap,    //確認是否不要使用Class內建的Click與Change Event
//                                  MinValue,
//                                  MaxValue);
//
// 存檔          : HTEditList->SaveEditTextToFile(Path, FileName);
// 讀檔          : HTEditList->ReadEditTextFromFile(Path, FileName);
// 資料交換      : HTEditList->InitialDataToEdit();
//
// 注意事項1 :  讀檔後會同時將資料轉給元件與參數, 所以針對各個客戶的限制處理使用 SetCustomerLimitationForConfig()
//              處理好後, 要將參數在指向回元件, 使用HTEditList->InitialDataToEdit();
//
// 注意事項2 :  針對上下限值為動態的,例如Contact中的Shuttle Pick & Release Height, 必須與Class內的事件脫節使用
//              HTEditList->FEditList[i]->DisableEventOverlap();
//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "HTEditList.h"
#include "cMyDB.h"
#include "cpublic.h"
#include "cmydef.h"
#include "common.h"
#include "FileInfo.h"
//---------------------------------------------------------------------------
//#pragma startup EditLimitInit
//#pragma exit EditLimitFree

#pragma package(smart_init)

//---------------------------------------------------------------------------
HTEditList *elConfig;                                                           //這個是可以從別的機台copy的
HTEditList *elUdUld;
HTEditList *elTrayForm;
HTEditList *elContact;
HTEditList *elLaser;
HTEditList *elConfig_byRecipe;                                                  //Sam 20220921 : config儲存跟隨recipe
HTEditList *cbLastSet;                                                          //這個是不可以從別的機台copy的
HTEditList *elVacuumUnit;                                                       //Sam 20230210 : 新增 VacuumUnit 通訊模組
//HTEditList *cbTest;
HTEditList *elTeach;
//uHPPickPos *HPTryPickPos;

uPlateInfo *PickFromHPList;
uPlateInfo *PlaceToHPList;

uPlateInfo *PlaceToCleanList;

AnsiString sHPPickRec                   =AnsiString("d:\\HT9045\\system\\PickHPRec.json");
AnsiString sHPPickRecException          =AnsiString("d:\\HT9045\\system\\PickHPRecException.json");
AnsiString sHPPlaceRec                  =AnsiString("d:\\HT9045\\system\\PlaceHPRec.json");
AnsiString sCleanPlaceRec               =AnsiString("d:\\HT9045\\system\\PlaceCleanRec.json");
//---------------------------------------------------------------------------
//void EditLimitInit()
//{
//    EditList=new HTEditList;
//}
//---------------------------------------------------------------------------
//void EditLimitFree()
//{
//    if(EditList!=NULL)
//    {
//        delete EditList;
//        EditList=NULL;
//    }
//}
//==============================================================================
//HTEditList
//==============================================================================
HTEditList::HTEditList()
{
    FEditList=new TList;
    bAlarmLimitation=false;
    iDecimalPoint=6;                                                            //Steven 20200514 : 統一浮點數
    iBarcodeReadType=bcTotal;
    bFontBlue=false;                                                            //Sam 20220921 : config儲存跟隨recipe
}
//---------------------------------------------------------------------------
HTEditList::~HTEditList()
{
    try
    {
        ClearALL();
        delete FEditList;
        FEditList=NULL;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~HTEditList");
    }
}
//---------------------------------------------------------------------------
void HTEditList::Add(TControl  *SoureCtrl,
                     void *Par,
                     TEditContent Content,
                     AnsiString GroupName,
                     AnsiString KeyName,
                     bool bVisible,
                     bool bEnable,
                     bool bReadFromFile,                                        //是否從檔案讀取
                     AnsiString DefValue,
                     bool bDisableEventOverlap,
                     AnsiString Min,
                     AnsiString Max,
                     int iTransform)
{
    TCustomEdit  *CEd = dynamic_cast <TCustomEdit *>(SoureCtrl);
    TComboBox    *Cbb = dynamic_cast <TComboBox   *>(SoureCtrl);
    TRadioGroup  *RdG = dynamic_cast <TRadioGroup *>(SoureCtrl);
    TCheckBox    *Chb = dynamic_cast <TCheckBox   *>(SoureCtrl);                //Steven 20230224 : Edit List改支援多型態元件
    TDateTimePicker *Ttp = dynamic_cast <TDateTimePicker   *>(SoureCtrl);
    AnsiString sPath=AuthPath+"config.ini";
    AnsiString sOldData="", sNewData="";
    if(Chb!=NULL)
    {
        THTEdit *Item           =new THTEdit;
        Item->Content           =ECBool;
        Item->SourceControl     =Chb;
        Item->DefaultValue      =DefValue;

        bool *SS;
        SS=(bool *)Par;
        Item->bParameter        =SS;
        Item->SetFileInformation(GroupName, KeyName);
        Chb->Checked            =*Item->bParameter;                             //Steven 20230502 : 修正bool沒有跟著存讀檔
        Item->GetDefaultPosition();
        Item->MaxValue          =1;
        Item->MinValue          =0;
        Item->CheckRange        =true;
        FEditList->Add(Item);

        Item->iTransformType    =iTransform;
        Item->bVisible          =bVisible;
        Item->bEnable           =bEnable;
        Item->bReadFromFile     =bReadFromFile;
        Item->iTag              =FEditList->Count-1;
        Item->SourceControl->Tag=FEditList->Count-1;                            //Steven 20230218 : 加上Tag, 方便使用陣列
    }
    else if(CEd!=NULL)
    {
        THTEdit *Item           =new THTEdit;
        if(Content==ECText || Content==ECPassword)
        {
            Item->Content           =Content;
            Item->SourceControl     =CEd;
            Item->DefaultValue      =DefValue;

            Item->OnWriteEdit(CEd);
            AnsiString *SS;
            SS=(AnsiString *)Par;
            Item->sParameter=SS;
            Item->GetDefaultPosition();
            if(CEd->Parent!=NULL)                                               //Sam 20250327 : 修正 Config 新舊版升級資料遺失問題
            {
                sOldData=ReadIniData(sPath, CEd->Parent->Name, CEd->Name, AnsiString(""));
                sNewData=ReadIniData(sPath, GroupName, KeyName, AnsiString(""));
                if(sNewData=="" && sOldData!="")
                    WriteIniData(sPath, GroupName, KeyName, sOldData);
            }
            Item->SetFileInformation(GroupName, KeyName);                       //Jimmy 20230417 : Fixed for 存檔異常

            if(Max==AnsiString("") && Min==AnsiString(""))
            {
                Item->CheckRange=false;
            }
            else
            {
                Item->CheckRange=true;
                if(atof(Min.c_str())>atof(Max.c_str()))
                {
                    Item->MaxValue=Min;
                    Item->MinValue=Max;
                }
                else
                {
                    Item->MaxValue=Max;
                    Item->MinValue=Min;
                }
            }
            FEditList->Add(Item);
        }
        else
        {
            Item->Content           =Content;
            Item->SourceControl     =CEd;
            Item->DefaultValue      =DefValue;

            Item->OnWriteEdit(CEd);
            if(Content==ECText            ||    //純文字
               Content==ECFileName        ||    //檔案名稱或路徑
               Content==ECPassword)             //密碼
            {
                AnsiString *SS;
                SS=(AnsiString *)Par;
                Item->sParameter=SS;
            }
            else if(Content==ECInteger    ||    //數字
                    Content==ECPosInt     ||    //正整數
                    Content==ECNegInt     ||    //負數
                    Content==ECPort)            //通訊埠
            {
                int *SS;
                SS=(int *)Par;
                Item->iParameter=SS;
            }
            else if(Content==ECDouble     ||    //浮點數
                    Content==ECPosDouble  ||    //正浮點數
                    Content==ECNegDouble  ||    //負浮點數
                    Content==ECPercent)         //百分比
            {
                double *SS;
                SS=(double *)Par;
                Item->dParameter=SS;
            }
            else if(Content==ECBool)        //Steven 20230502 : 修正bool沒有跟著存讀檔
            {
                bool *SS;
                SS=(bool *)Par;
                Item->bParameter=SS;
            }

            Item->SetFileInformation(GroupName, KeyName);
            if(Content==ECPort)            //通訊埠
            {
                Item->MaxValue=65535;
                Item->MinValue=0;
            }
            else if(Max==AnsiString("") && Min==AnsiString(""))
            {
                Item->CheckRange=false;
            }
            else
            {
                Item->CheckRange=true;
                if(atof(Min.c_str())>atof(Max.c_str()))
                {
                    Item->MaxValue=Min;
                    Item->MinValue=Max;
                }
                else
                {
                    Item->MaxValue=Max;
                    Item->MinValue=Min;
                }
            }
            Item->GetDefaultPosition();
            FEditList->Add(Item);
        }
        if(bDisableEventOverlap)
        {
            Item->DisableEventOverlap();
        }

        Item->bVisible          =bVisible;
        Item->bEnable           =bEnable;
        Item->bReadFromFile     =bReadFromFile;
        Item->iTransformType    =iTransform;
        Item->iTag              =FEditList->Count-1;
        Item->SourceControl->Tag=FEditList->Count-1;                            //Steven 20230218 : 加上Tag, 方便使用陣列
    }
    else if(Cbb!=NULL)                                                          //Steven 20230224 : Edit List改支援多型態元件
    {
        THTEdit *Item       =new THTEdit;
        Item->Content       =ECPosInt;
        Item->SourceControl =Cbb;
        Item->DefaultValue  =DefValue;

        int *SS;
        SS=(int *)Par;
        Item->iParameter=SS;

        Item->SetFileInformation(GroupName, KeyName);
        Item->MaxValue          =Cbb->Items->Count-1;
        Item->MinValue          =0;
        Item->CheckRange        =true;
        Cbb->ItemIndex          =*Item->iParameter;
        Item->GetDefaultPosition();
        FEditList->Add(Item);

        Item->bVisible          =bVisible;
        Item->bEnable           =bEnable;
        Item->bReadFromFile     =bReadFromFile;
        Item->iTransformType    =iTransform;
        Item->iTag              =FEditList->Count-1;
        Item->SourceControl->Tag=FEditList->Count-1;                            //Steven 20230218 : 加上Tag, 方便使用陣列
    }
    else if(RdG!=NULL)                                                          //Steven 20230224 : Edit List改支援多型態元件
    {
        THTEdit *Item           =new THTEdit;
        Item->Content           =ECPosInt;
        Item->SourceControl     =RdG;
        Item->DefaultValue      =DefValue;

        int *SS;
        SS=(int *)Par;
        Item->iParameter=SS;
        Item->SetFileInformation(GroupName, KeyName);
        RdG->ItemIndex          =*Item->iParameter;
        Item->MaxValue          =RdG->Items->Count-1;
        Item->MinValue          =0;
        Item->CheckRange        =true;
        Item->GetDefaultPosition();
        FEditList->Add(Item);

        Item->bVisible          =bVisible;
        Item->bEnable           =bEnable;
        Item->bReadFromFile     =bReadFromFile;
        Item->iTransformType    =iTransform;
        Item->iTag              =FEditList->Count-1;
        Item->SourceControl->Tag=FEditList->Count-1;                            //Steven 20230218 : 加上Tag, 方便使用陣列
    }
    else if(Ttp!=NULL)
    {
        THTEdit *Item           =new THTEdit;
        Item->Content           =Content;
        Item->SourceControl     =Ttp;
        Item->DefaultValue      =DefValue;

        double *SS;
        SS=(double *)Par;
        Item->dParameter=SS;

        Item->SetFileInformation(GroupName, KeyName);
        SetItemLimit(Item,Content,Min,Max);
        Item->GetDefaultPosition();
        FEditList->Add(Item);

        Item->bVisible          =bVisible;
        Item->bEnable           =bEnable;
        Item->bReadFromFile     =bReadFromFile;
        Item->iTransformType    =iTransform;
        Item->iTag              =FEditList->Count-1;
        Item->SourceControl->Tag=FEditList->Count-1;                            //Steven 20230218 : 加上Tag, 方便使用陣列
    }
}
//---------------------------------------------------------------------------
void HTEditList::AddInteger(TEdit *SoureEdit, int *Par, int Min, int Max, int DefValue, AnsiString GroupName, AnsiString KeyName)
{
//    if(SoureEdit!=NULL)
//    {
//        THTEdit *Item       =new THTEdit;
//        Item->Content       =ECInteger;
//        Item->SourceEdit    =SoureEdit;
//        Item->DefaultValue  =DefValue;
//        Item->iParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//        if(Max==0 && Min==0)
//        {
//            Item->CheckRange=false;
//        }
//        else
//        {
//            Item->CheckRange=true;
//            if(Min>Max)
//            {
//                Item->MaxValue=Min;
//                Item->MinValue=Max;
//            }
//            else
//            {
//                Item->MaxValue=Max;
//                Item->MinValue=Min;
//            }
//        }
//        FEditList->Add(Item);
//        Item->iTag=FEditList->Count-1;
//        Item->SourceEdit->Tag=FEditList->Count-1;                               //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
}
//---------------------------------------------------------------------------
void HTEditList::AddInteGerP(TEdit *SoureEdit, int *Par, int Min, int Max, int DefValue, AnsiString GroupName, AnsiString KeyName)
{
//    if(SoureEdit!=NULL)
//    {
//        THTEdit *Item       =new THTEdit;
//        Item->Content       =ECPosInt;
//        Item->SourceEdit    =SoureEdit;
//        Item->DefaultValue  =DefValue;
//        Item->iParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//
//        Item->CheckRange=true;
//        if(Min>Max)
//        {
//            Item->MaxValue=Min;
//            Item->MinValue=Max;
//        }
//        else
//        {
//            Item->MaxValue=Max;
//            Item->MinValue=Min;
//        }
//        FEditList->Add(Item);
//        Item->iTag=FEditList->Count-1;
//        Item->SourceEdit->Tag=FEditList->Count-1;                               //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
}
//---------------------------------------------------------------------------
void HTEditList::AddIntegerN(TEdit *SoureEdit, int *Par, int Min, int Max, int DefValue, AnsiString GroupName, AnsiString KeyName)
{
//    if(SoureEdit!=NULL)
//    {
//        THTEdit *Item       =new THTEdit;
//        Item->Content       =ECNegInt;
//        Item->SourceEdit    =SoureEdit;
//        Item->DefaultValue  =DefValue;
//        Item->iParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//
//        Item->CheckRange=true;
//        if(Min>Max)
//        {
//            Item->MaxValue=Min;
//            Item->MinValue=Max;
//        }
//        else
//        {
//            Item->MaxValue=Max;
//            Item->MinValue=Min;
//        }
//        FEditList->Add(Item);
//        Item->iTag=FEditList->Count-1;
//        Item->SourceEdit->Tag=FEditList->Count-1;                               //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
}
//---------------------------------------------------------------------------
void HTEditList::AddDouble(TEdit *SoureEdit, double *Par, double Min, double Max, double DefValue, AnsiString GroupName, AnsiString KeyName)
{
//    if(SoureEdit!=NULL)
//    {
//        THTEdit *Item       =new THTEdit;
//        Item->Content       =ECDouble;
//        Item->SourceEdit    =SoureEdit;
//        Item->DefaultValue  =DefValue;
//        Item->dParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//        if(Max==0.0 && Min==0.0)
//        {
//            Item->CheckRange=false;
//        }
//        else
//        {
//            Item->CheckRange=true;
//            if(Min>Max)
//            {
//                Item->MaxValue=Min;
//                Item->MinValue=Max;
//            }
//            else
//            {
//                Item->MaxValue=Max;
//                Item->MinValue=Min;
//            }
//        }
//        FEditList->Add(Item);
//        Item->iTag=FEditList->Count-1;
//        Item->SourceEdit->Tag=FEditList->Count-1;                               //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
}
//---------------------------------------------------------------------------
void HTEditList::AddDoubleP(TEdit *SoureEdit, double *Par, double Min, double Max, double DefValue, AnsiString GroupName, AnsiString KeyName)
{
//    if(SoureEdit!=NULL)
//    {
//        THTEdit *Item       =new THTEdit;
//        Item->Content       =ECPosDouble;
//        Item->SourceEdit    =SoureEdit;
//        Item->DefaultValue  =DefValue;
//        Item->dParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//
//        Item->CheckRange=true;
//        if(Min>Max)
//        {
//            Item->MaxValue=Min;
//            Item->MinValue=Max;
//        }
//        else
//        {
//            Item->MaxValue=Max;
//            Item->MinValue=Min;
//        }
//        FEditList->Add(Item);
//        Item->iTag=FEditList->Count-1;
//        Item->SourceEdit->Tag=FEditList->Count-1;                               //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
}
//---------------------------------------------------------------------------
void HTEditList::AddDoubleN(TEdit *SoureEdit, double *Par, double Min, double Max, double DefValue, AnsiString GroupName, AnsiString KeyName)
{
//    if(SoureEdit!=NULL)
//    {
//        THTEdit *Item       =new THTEdit;
//        Item->Content       =ECNegDouble;
//        Item->SourceEdit    =SoureEdit;
//        Item->DefaultValue  =DefValue;
//        Item->dParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//
//        Item->CheckRange=true;
//        if(Min>Max)
//        {
//            Item->MaxValue=Min;
//            Item->MinValue=Max;
//        }
//        else
//        {
//            Item->MaxValue=Max;
//            Item->MinValue=Min;
//        }
//        FEditList->Add(Item);
//        Item->iTag=FEditList->Count-1;
//        Item->SourceEdit->Tag=FEditList->Count-1;                               //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
}
//---------------------------------------------------------------------------
void HTEditList::AddPercent(TEdit *SoureEdit, double *Par, double Min, double Max, double DefValue, AnsiString GroupName, AnsiString KeyName)
{
//    if(SoureEdit!=NULL)
//    {
//        THTEdit *Item       =new THTEdit;
//        Item->Content       =ECPercent;
//        Item->SourceEdit    =SoureEdit;
//        Item->DefaultValue  =DefValue;
//        Item->dParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//
//        Item->CheckRange=true;
//        if(Min>Max)
//        {
//            Item->MaxValue=Min;
//            Item->MinValue=Max;
//        }
//        else
//        {
//            Item->MaxValue=Max;
//            Item->MinValue=Min;
//        }
//        FEditList->Add(Item);
//        Item->iTag=FEditList->Count-1;
//        Item->SourceEdit->Tag=FEditList->Count-1;                               //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
}
//---------------------------------------------------------------------------
void HTEditList::ClearALL()
{
    if(FEditList!=NULL)
        FEditList->Clear();
}
//---------------------------------------------------------------------------
void HTEditList::SetBarcodeType(int iBarcodeType)
{
    for(int i=0; i<FEditList->Count; i++)
    {
        THTEdit *Temp;
        Temp=(THTEdit*)FEditList->Items[i];
        Temp->iBarcodeReadType=iBarcodeType;
    }
}
//---------------------------------------------------------------------------
bool HTEditList::SaveEditTextToFile(AnsiString Path, AnsiString FileName)
{
    bool bResult=true, bRet;
    AnsiString sParentName, sEditName;
    AnsiString Str4, Str5, Ret;
    int iCurr, iMax, iMin, iRet;
    double dCurr, dMax, dMin;
    TEditContent ContentType;
    MyForceDirectories(Path);

    IncludeTrailingPathDelimiter(Path);
    AnsiString sFullFileName=Path+FileName;

    TMemIniFile *ini;
    ini = new TMemIniFile(sFullFileName);

    AnsiString Str, Str2, Str3;
    map<AnsiString, AnsiString>mapKeyValue;
    map<AnsiString, AnsiString>::iterator iterKeyValue;

    mapKeyValue.clear();
    for(int i=0; i<FEditList->Count; i++)
    {
        bRet=false;
        THTEdit *Temp;
        Temp=(THTEdit*)FEditList->Items[i];

        if(Temp->SourceControl->Parent!=NULL)
        {
            TWinControl* PP=Temp->SourceControl->Parent;
            sParentName=PP->Name;
        }
        else
        {
            sParentName="NoParent";
        }
        sEditName  =Temp->SourceControl->Name;
        ContentType=Temp->Content;

        if(Temp->IniGroupName=="" || Temp->IniKeyName=="")
        {
            if(bAlarmLimitation)
            {
                if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                    Str2.sprintf("Please check the group and key for Edit. @%s, [%s]->%s", sParentName, Temp->IniGroupName, Temp->IniGroupName);
                else
                    Str2.sprintf("Please check the group and key for Edit. %s->%s", sParentName, sEditName);
                Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
            }
            bResult=false;
        }
        else
        {
            Str.sprintf("%s_%s", Temp->IniGroupName, Temp->IniKeyName);
            iterKeyValue=mapKeyValue.find(Str);

            if(iterKeyValue==mapKeyValue.end() || mapKeyValue.begin()==mapKeyValue.end())  //確認是不是不同元件在同一個位置存檔
            {
                iCurr=0, iMax=0, iMin=0;
                dCurr=0, dMax=0, dMin=0;
                Str2.sprintf("%s::%s", sParentName, sEditName);
                mapKeyValue[Str]=Str2;
                TCustomEdit *CEd = dynamic_cast <TCustomEdit *>(Temp->SourceControl);
                TComboBox   *Cbb = dynamic_cast <TComboBox   *>(Temp->SourceControl);
                TRadioGroup *RdG = dynamic_cast <TRadioGroup *>(Temp->SourceControl);
                TCheckBox   *Chb = dynamic_cast <TCheckBox   *>(Temp->SourceControl);   //Steven 20230224 : Edit List改支援多型態元件
                TDateTimePicker *Ttp = dynamic_cast <TDateTimePicker   *>(Temp->SourceControl);

                if(Temp->bReadFromFile==false)
                {
                    bRet=false;
                }
                else if(Temp->CheckRange)
                {
                    if(ContentType==ECBool)
                    {
                        iCurr=Chb->Checked;
                        if(iCurr==0 || iCurr==1)
                        {
                            bRet=true;
                        }
                        else
                        {
                            bResult=false;
                            if(bAlarmLimitation)
                            {
                                if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                                    Str2.sprintf("Please check the data range. @%s, [%s]->%s, max:1, min:0, current:%d", sParentName, Temp->IniGroupName, Temp->IniGroupName,  iCurr);
                                else
                                    Str2.sprintf("Please check the data range. %s->%s, max:1, min:0, current:%d", sParentName, sEditName, iCurr);
                                Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
                            }
                        }
                    }
                    else if(ContentType==ECInteger || ContentType==ECPosInt || ContentType==ECNegInt || ContentType==ECPort)        //數字
                    {
                        if(CEd!=NULL)
                        {
                            dCurr=atof(CEd->Text.c_str());
                            if(Temp->iTransformType==EUuMToMM)                  //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                            {
                                dCurr=(double)dCurr*(double)100.0;
                                iMax =atof(Temp->MaxValue.c_str())*(double)100.0;
                                iMin =atof(Temp->MinValue.c_str())*(double)100.0;
                            }
                            else if(Temp->iTransformType==EUMSToSec)            //顯示為Sec, 存檔為MS
                            {
                                dCurr=(double)dCurr*(double)1000.0;
                                iMax =atof(Temp->MaxValue.c_str())*(double)1000.0;
                                iMin =atof(Temp->MinValue.c_str())*(double)1000.0;
                            }
                            else
                            {
                                iMax =atoi(Temp->MaxValue.c_str());
                                iMin =atoi(Temp->MinValue.c_str());
                            }
                            iCurr=dCurr;
                        }
                        else if(Cbb!=NULL)
                        {
                            iCurr=Cbb->ItemIndex;
                            iMax =atoi(Temp->MaxValue.c_str());
                            iMin =atoi(Temp->MinValue.c_str());
                        }
                        else if(RdG!=NULL)
                        {
                            iCurr=RdG->ItemIndex;
                            iMax =atoi(Temp->MaxValue.c_str());
                            iMin =atoi(Temp->MinValue.c_str());
                        }

                        if(iCurr<=iMax && iCurr>=iMin)
                        {
                            bRet=true;
                        }
                        else
                        {
                            bResult=false;
                            if(bAlarmLimitation)
                            {
                                if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                                    Str2.sprintf("Please check the data range. @%s, [%s]->%s, max:%d, min:%d, current:%d", sParentName, Temp->IniGroupName, Temp->IniGroupName, iMax, iMin, iCurr);
                                else
                                    Str2.sprintf("Please check the data range. %s->%s, max:%d, min:%d, current:%d", sParentName, sEditName, iMax, iMin, iCurr);
                                Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
                            }
                        }
                    }
                    else if(ContentType==ECDouble || ContentType==ECPosDouble || ContentType==ECNegDouble || ContentType==ECPercent)         //浮點數
                    {
                        if(CEd!=NULL)
                        {
                            if(Temp->iTransformType==EUuMToMM)                  //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                            {
                                dCurr=atof(CEd->Text.c_str())*(double)100.0;
                                dMax =atof(Temp->MaxValue.c_str())*(double)100.0;
                                dMin =atof(Temp->MinValue.c_str())*(double)100.0;
                            }
                            else if(Temp->iTransformType==EUMSToSec)            //顯示為Sec, 存檔為MS
                            {
                                dCurr=atof(CEd->Text.c_str())*(double)1000.0;   //Jerry Yang 20240314 : iCurr --> dCurr
                                dMax =atof(Temp->MaxValue.c_str())*(double)1000.0;
                                dMin =atof(Temp->MinValue.c_str())*(double)1000.0;
                            }
                            else
                            {
                                dCurr=atof(CEd->Text.c_str());
                                dMax =atof(Temp->MaxValue.c_str());
                                dMin =atof(Temp->MinValue.c_str());
                            }

                            if(dCurr>=dMin && dCurr<=dMax)
                            {
                                if(iDecimalPoint<=1)                            //Steven 20200514 : 統一浮點數
                                    Str2=CEd->Text;
                                else if(iDecimalPoint==1)                       //Steven 20200514 : 統一浮點數
                                    Str2.sprintf("%0.1f", dCurr);
                                else if(iDecimalPoint==2)
                                    Str2.sprintf("%0.2f", dCurr);
                                else if(iDecimalPoint==3)
                                    Str2.sprintf("%0.3f", dCurr);
                                else if(iDecimalPoint==4)
                                    Str2.sprintf("%0.4f", dCurr);
                                else if(iDecimalPoint==5)
                                    Str2.sprintf("%0.5f", dCurr);
                                else
                                    Str2.sprintf("%0.6f", dCurr);
                                CEd->Text=Str2;
                                bRet=true;
                            }
                            else
                            {
                                bResult=false;
                                if(bAlarmLimitation)
                                {
                                    if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                                        Str2.sprintf("Please check the data range. @%s, [%s]->%s, max:%f, min:%f, current:%f", sParentName, Temp->IniGroupName, Temp->IniGroupName, dMax, dMin, dCurr);
                                    else
                                        Str2.sprintf("Please check the data range. %s->%s, max:%f, min:%f, current:%f", sParentName, sEditName, dMax, dMin, dCurr);
                                    Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
                                }
                            }
                        }
                        else if(Ttp!=NULL)
                        {
                            dCurr=Ttp->DateTime;
                            dMax =atof(Temp->MaxValue.c_str());
                            dMin =atof(Temp->MinValue.c_str());
                            if(dCurr>=dMin && dCurr<=dMax)
                            {
                                bRet=true;
                            }
                            else
                            {
                                bResult=false;
                                if(bAlarmLimitation)
                                {
                                    if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                                        Str2.sprintf("Please check the data range. @%s, [%s]->%s, max:%f, min:%f, current:%f", sParentName, Temp->IniGroupName, Temp->IniGroupName, dMax, dMin, dCurr);
                                    else
                                        Str2.sprintf("Please check the data range. %s->%s, max:%f, min:%f, current:%f", sParentName, sEditName, dMax, dMin, dCurr);
                                    Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(ContentType==ECDouble || ContentType==ECPosDouble || ContentType==ECNegDouble || ContentType==ECPercent)         //Steven 20200514 : 統一浮點數
                    {
                        if(CEd!=NULL)
                        {
                            dCurr=atof(CEd->Text.c_str());
                            if(iDecimalPoint<=1)                                //Steven 20200514 : 統一浮點數
                                Str2=CEd->Text;
                            if(iDecimalPoint==1)
                                Str2.sprintf("%0.1f", dCurr);
                            else if(iDecimalPoint==2)
                                Str2.sprintf("%0.2f", dCurr);
                            else if(iDecimalPoint==3)
                                Str2.sprintf("%0.3f", dCurr);
                            else if(iDecimalPoint==4)
                                Str2.sprintf("%0.4f", dCurr);
                            else if(iDecimalPoint==5)
                                Str2.sprintf("%0.5f", dCurr);
                            else
                                Str2.sprintf("%0.6f", dCurr);
                            CEd->Text=Str2;
                        }
                    }
                    bRet=true;
                }

                if(bRet)
                {
                    if(Chb!=NULL)
                    {
                        //Steven 20200514 : 加上Change Log
                        //==>
                        Ret=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                        iRet=atoi(Ret.c_str());
                        if(iRet!=Chb->Checked)
                        {
                            Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
                            Str5.sprintf("%d==>%d", iRet, Chb->Checked);
                            RecordChangeLogProcess(Str4, Str5);       //wei 20180625 offset Change log紀錄
                        }
                        //<==
                        //Steven 20200514 : 加上Change Log
                        ini->WriteInteger(Temp->IniGroupName, Temp->IniKeyName, Chb->Checked?1:0);
                    }
                    else if(CEd!=NULL)
                    {
                        if(Temp->iTransformType==EUuMToMM)                      //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                        {
                            dCurr=atof(CEd->Text.c_str())*(double)100.0;
                            iCurr=dCurr;
                            Ret=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                            if(atoi(Ret.c_str())!=iCurr)
                            {
                                Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
                                Str5.sprintf("%s==>%d", Ret, iCurr);
                                RecordChangeLogProcess(Str4, Str5);             //wei 20180625 offset Change log紀錄
                            }

                            ini->WriteString(Temp->IniGroupName, Temp->IniKeyName, AnsiString(iCurr));
                        }
                        else if(Temp->iTransformType==EUMSToSec)                //顯示為Sec, 存檔為MS
                        {
                            dCurr=atof(CEd->Text.c_str())*(double)1000.0;
                            iCurr=dCurr;
                            Ret=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                            if(atoi(Ret.c_str())!=iCurr)
                            {
                                Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
                                Str5.sprintf("%s==>%d", Ret, iCurr);
                                RecordChangeLogProcess(Str4, Str5);             //wei 20180625 offset Change log紀錄
                            }
                            ini->WriteString(Temp->IniGroupName, Temp->IniKeyName, AnsiString(iCurr));
                        }
                        else
                        {
                            //Steven 20200514 : 加上Change Log
                            //==>
                            Ret=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                            if(Ret!=CEd->Text)
                            {
                                Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
                                Str5.sprintf("%s==>%s", Ret, CEd->Text);
                                RecordChangeLogProcess(Str4, Str5);             //wei 20180625 offset Change log紀錄
                            }
                            //<==
                            //Steven 20200514 : 加上Change Log
                            ini->WriteString(Temp->IniGroupName, Temp->IniKeyName, CEd->Text);
                        }
                    }
                    else if(Cbb!=NULL)
                    {
                        //Steven 20200514 : 加上Change Log
                        //==>
                        Ret=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                        iRet=atoi(Ret.c_str());
                        if(iRet!=Cbb->ItemIndex)
                        {
                            Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
                            Str5.sprintf("%d==>%d", iRet, Cbb->ItemIndex);
                            RecordChangeLogProcess(Str4, Str5);       //wei 20180625 offset Change log紀錄
                        }
                        //<==
                        //Steven 20200514 : 加上Change Log
                        ini->WriteInteger(Temp->IniGroupName, Temp->IniKeyName, Cbb->ItemIndex);
                    }
                    else if(RdG!=NULL)
                    {
                        //Steven 20200514 : 加上Change Log
                        //==>
                        Ret=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                        iRet=atoi(Ret.c_str());
                        if(iRet!=RdG->ItemIndex)
                        {
                            Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
                            Str5.sprintf("%d==>%d", iRet, RdG->ItemIndex);
                            RecordChangeLogProcess(Str4, Str5);                 //wei 20180625 offset Change log紀錄
                        }
                        //<==
                        //Steven 20200514 : 加上Change Log
                        ini->WriteInteger(Temp->IniGroupName, Temp->IniKeyName, RdG->ItemIndex);
                    }
                    else if(Ttp!=NULL)
                    {
                        AnsiString sNowStr=FloatToStr(Ttp->DateTime);
                        Ret=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                        if(Ret!=sNowStr)
                        {
                            Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
                            Str5.sprintf("%s==>%s", Ret, sNowStr);
                            RecordChangeLogProcess(Str4, Str5);             //wei 20180625 offset Change log紀錄
                        }
                        ini->WriteString(Temp->IniGroupName, Temp->IniKeyName, sNowStr);
                    }

                    if(ContentType==ECBool)
                    {
                        if(Chb!=NULL)
                            *Temp->bParameter=Chb->Checked;                     //Steven 20230502 : 修正bool沒有跟著存讀檔
                    }
                    else if(ContentType==ECInteger || ContentType==ECPosInt || ContentType==ECNegInt || ContentType==ECPort)
                    {
                        if(Temp->iParameter!=NULL)
                        {
                            if(CEd!=NULL)
                            {
                                if(Temp->iTransformType==EUuMToMM)              //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                                    dCurr=atof(CEd->Text.c_str())*(double)100.0;
                                else if(Temp->iTransformType==EUMSToSec)        //顯示為Sec, 存檔為MS
                                    dCurr=atof(CEd->Text.c_str())*(double)1000.0;
                                else
                                    dCurr=atoi(CEd->Text.c_str());
                                iCurr=dCurr;
                                *Temp->iParameter=iCurr;
                            }
                            else if(Cbb!=NULL)
                            {
                                *Temp->iParameter=Cbb->ItemIndex;
                            }
                            else if(RdG!=NULL)
                            {
                                *Temp->iParameter=RdG->ItemIndex;
                            }
                        }
                    }
                    else if(ContentType==ECDouble || ContentType==ECPosDouble || ContentType==ECNegDouble || ContentType==ECPercent)
                    {
                        if(Temp->dParameter!=NULL && CEd!=NULL)
                        {
                            if(Temp->iTransformType==EUuMToMM)                  //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                                dCurr=atof(CEd->Text.c_str())*(double)100.0;
                            else if(Temp->iTransformType==EUMSToSec)            //顯示為Sec, 存檔為MS
                                dCurr=atof(CEd->Text.c_str())*(double)1000.0;
                            else
                                dCurr=atof(CEd->Text.c_str());
                            *Temp->dParameter=dCurr;
                        }
                        else if(Temp->dParameter!=NULL && Ttp!=NULL)
                        {
                            *Temp->dParameter=Ttp->DateTime;
                        }
                    }
                    else
                    {
                        if(CEd!=NULL)
                            *Temp->sParameter=CEd->Text;
                    }
                }
            }
            else
            {
                bResult=false;
                Str2=mapKeyValue[Str];
                if(bAlarmLimitation)
                {
                    Str3.sprintf("%s %s got duplicate save position with %s::%s!!", Str, Str2, sParentName, sEditName);
                    Application->MessageBoxA(Str3.c_str(), "WARNING!", MB_OK);
                }
            }
        }
    }

    ini->UpdateFile();
    delete ini;

    return bResult;
}
//---------------------------------------------------------------------------
bool HTEditList::ReadEditTextFromFile(AnsiString Path, AnsiString FileName)
{
    bool bResult=true;
    AnsiString Str, Str2, Str3;
    AnsiString Ret;
    AnsiString sParentName, sEditName;
    int iCurr=0, iMax=0, iMin=0, iDef=0;
    double dCurr=0, dMax=0, dMin=0;
    TEditContent ContentType;

    IncludeTrailingPathDelimiter(Path);
    AnsiString sFullFileName=Path+FileName;

    TMemIniFile *ini;
    ini = new TMemIniFile(sFullFileName);
    map<AnsiString, AnsiString>mapKeyValue;
    map<AnsiString, AnsiString>::iterator iterKeyValue;

    mapKeyValue.clear();
    for(int i=0; i<FEditList->Count; i++)
    {
        THTEdit *Temp;
        Temp=(THTEdit*)FEditList->Items[i];
        if(Temp->SourceControl->Parent!=NULL)
        {
            TWinControl* PP=Temp->SourceControl->Parent;
            sParentName=PP->Name;
        }
        else
        {
            sParentName="NoParent";
        }
        ContentType=Temp->Content;
        sEditName  =Temp->SourceControl->Name;

        if(Temp->IniGroupName=="" || Temp->IniKeyName=="")
        {
            if(bAlarmLimitation)
            {
                if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                    Str2.sprintf("Please check the group and key for Edit. @%s, [%s]->%s", sParentName, Temp->IniGroupName, Temp->IniGroupName);
                else
                    Str2.sprintf("Please check the group and key for Edit. %s->%s", sParentName, sEditName);
                Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
            }
        }
        else
        {
            Str.sprintf("%s_%s", Temp->IniGroupName, Temp->IniKeyName);
            iterKeyValue=mapKeyValue.find(Str);
            if(iterKeyValue==mapKeyValue.end() || mapKeyValue.begin()==mapKeyValue.end())  //確認是不是不同元件在同一個位置存檔
            {
                iCurr=0, iMax=0, iMin=0, iDef=0;
                dCurr=0, dMax=0, dMin=0;
                TCustomEdit *CEd = dynamic_cast <TCustomEdit *>(Temp->SourceControl);
                TComboBox   *Cbb = dynamic_cast <TComboBox   *>(Temp->SourceControl);
                TRadioGroup *RdG = dynamic_cast <TRadioGroup *>(Temp->SourceControl);
                TCheckBox   *Chb = dynamic_cast <TCheckBox   *>(Temp->SourceControl);
                TDateTimePicker *Ttp = dynamic_cast <TDateTimePicker   *>(Temp->SourceControl);
                Str2.sprintf("%s::%s", sParentName, sEditName);
                mapKeyValue[Str]=Str2;

                if(Temp->bReadFromFile==false)
                {
                    if(CEd!=NULL)
                    {
                        CEd->Text=Temp->DefaultValue;
                    }
                    else if(Cbb!=NULL)
                    {
                        Cbb->ItemIndex=atoi(Temp->DefaultValue.c_str());
                    }
                    else if(RdG!=NULL)
                    {
                        RdG->ItemIndex=atoi(Temp->DefaultValue.c_str());
                    }
                    else if(Chb!=NULL)
                    {
                        Chb->Checked=(atoi(Temp->DefaultValue.c_str())==1)?true:false;  //Steven 20230502 : 修正bool沒有跟著存讀檔
                    }
                }
                else
                {
                    if(CEd!=NULL)
                    {
                        Str=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                        if(ContentType==ECInteger || ContentType==ECPosInt || ContentType==ECNegInt || ContentType==ECPort)        //數字
                        {
                            dCurr=atof(Str.c_str());
                            if(Temp->iTransformType==EUuMToMM)                  //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                                Str.sprintf("%0.3f", (double)dCurr/(double)100.0);
                            else if(Temp->iTransformType==EUMSToSec)            //顯示為Sec, 存檔為MS
                                Str.sprintf("%0.3f", (double)dCurr/(double)1000.0);
                        }
                        else if(ContentType==ECDouble || ContentType==ECPosDouble || ContentType==ECNegDouble || ContentType==ECPercent)    //浮點數
                        {
                            dCurr=atof(Str.c_str());
                            if(Temp->iTransformType==EUuMToMM)                  //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                                Str.sprintf("%0.3f", (double)dCurr/(double)100.0);
                            else if(Temp->iTransformType==EUMSToSec)            //顯示為Sec, 存檔為MS
                                Str.sprintf("%0.3f", (double)dCurr/(double)1000.0);
//                            else                                              //Steven 20240614 : Mark
//                                Str.sprintf("%0.3f", dCurr);
                        }
                        CEd->Text=Str;
                    }
                    else if(Cbb!=NULL)
                    {
                        iDef=atoi(Temp->DefaultValue.c_str());
                        Cbb->ItemIndex=ini->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, iDef);
                    }
                    else if(RdG!=NULL)
                    {
                        iDef=atoi(Temp->DefaultValue.c_str());
                        RdG->ItemIndex=ini->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, iDef);
                    }
                    else if(Chb!=NULL)
                    {
                        iDef=atoi(Temp->DefaultValue.c_str());
                        iDef=ini->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, iDef);
                        Chb->Checked=(iDef==1)?true:false;                      //Steven 20230502 : fixed for EditList
                    }
                    else if(Ttp!=NULL)
                    {
                        Str=ini->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                        dCurr=atof(Str.c_str());
                        Ttp->DateTime=dCurr;
                    }
                }

                if(Temp->CheckRange)
                {
                    if(ContentType==ECInteger || ContentType==ECPosInt || ContentType==ECNegInt || ContentType==ECPort)        //數字
                    {
                        if(Temp->iTransformType==EUuMToMM)                      //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                        {
                            if(CEd!=NULL)
                            {
                                iCurr=ConvertDoubleStrToInt(CEd->Text, 100.0);
                                iMax =ConvertDoubleStrToInt(Temp->MaxValue, 100.0);
                                iMin =ConvertDoubleStrToInt(Temp->MinValue, 100.0);
                            }
                        }
                        else if(Temp->iTransformType==EUMSToSec)                //顯示為Sec, 存檔為MS
                        {
                            if(CEd!=NULL)
                            {
                                iCurr=ConvertDoubleStrToInt(CEd->Text, 1000.0);
                                iMax =ConvertDoubleStrToInt(Temp->MaxValue, 1000.0);
                                iMin =ConvertDoubleStrToInt(Temp->MinValue, 1000.0);
                            }
                        }
                        else
                        {
                            if(CEd!=NULL)
                                iCurr=atoi(CEd->Text.c_str());
                            else if(Cbb!=NULL)
                                iCurr=Cbb->ItemIndex;
                            else if(RdG!=NULL)
                                iCurr=RdG->ItemIndex;
                            iMax =atoi(Temp->MaxValue.c_str());
                            iMin =atoi(Temp->MinValue.c_str());
                        }

                        if(iCurr<=iMax && iCurr>=iMin)
                        {
                            *Temp->iParameter=iCurr;
                        }
                        else
                        {
                            bResult=false;
                            if(bAlarmLimitation)
                            {
                                if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                                    Str2.sprintf("Please check the data range. @%s, [%s]->%s, max:%d, min:%d, current:%d", sParentName, Temp->IniGroupName, Temp->IniGroupName, iMax, iMin, iCurr);
                                else
                                    Str2.sprintf("Please check the data range. %s->%s, max:%d, min:%d, current:%d", sParentName, sEditName, iMax, iMin, iCurr);
                                Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
                            }
                        }
                    }
                    else if(ContentType==ECDouble || ContentType==ECPosDouble || ContentType==ECNegDouble || ContentType==ECPercent)    //浮點數
                    {
                        if(CEd!=NULL)
                        {
                            dCurr=atof(CEd->Text.c_str());
                            if(Temp->iTransformType==EUuMToMM)                      //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                            {
                                dMax =atof(Temp->MaxValue.c_str())*(double)100.0;
                                dMin =atof(Temp->MinValue.c_str())*(double)100.0;
                                dCurr=(double)dCurr*(double)100.0;
                            }
                            else if(Temp->iTransformType==EUMSToSec)                //顯示為Sec, 存檔為MS
                            {
                                dMax =atof(Temp->MaxValue.c_str())*(double)1000.0;
                                dMin =atof(Temp->MinValue.c_str())*(double)1000.0;
                                dCurr=(double)dCurr*(double)1000.0;
                            }
                            else
                            {
                                dMax =atof(Temp->MaxValue.c_str());
                                dMin =atof(Temp->MinValue.c_str());
                            }

                            if(dCurr<=dMax && dCurr>=dMin)
                            {
                                Str2.sprintf("%0.3f", dCurr);                       //小數點最少顯示三位
                                if(CEd!=NULL)
                                    CEd->Text=Str2;
                                *Temp->dParameter=dCurr;
                            }
                            else
                            {
                                bResult=false;
                                if(bAlarmLimitation)
                                {
                                    if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                                        Str2.sprintf("Please check the data range. @%s, [%s]->%s, max:%f, min:%f, current:%f", sParentName, Temp->IniGroupName, Temp->IniGroupName, dMax, dMin, dCurr);
                                    else
                                        Str2.sprintf("Please check the data range. %s->%s, max:%f, min:%f, current:%f", sParentName, sEditName, dMax, dMin, dCurr);
                                    Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
                                }
                            }
                        }
                        else if(Ttp!=NULL)
                        {
                            dCurr=Ttp->DateTime;
                            dMax =atof(Temp->MaxValue.c_str());
                            dMin =atof(Temp->MinValue.c_str());
                            if(dCurr<=dMax && dCurr>=dMin)
                            {
                                *Temp->dParameter=dCurr;
                            }
                            else
                            {
                                bResult=false;
                                if(bAlarmLimitation)
                                {
                                    if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
                                        Str2.sprintf("Please check the data range. @%s, [%s]->%s, max:%f, min:%f, current:%f", sParentName, Temp->IniGroupName, Temp->IniGroupName, dMax, dMin, dCurr);
                                    else
                                        Str2.sprintf("Please check the data range. %s->%s, max:%f, min:%f, current:%f", sParentName, sEditName, dMax, dMin, dCurr);
                                    Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
                                }
                            }
                        }
                    }
                    else if(ContentType==ECBool)
                    {
                        if(CEd!=NULL)                                           //Steven 20230502 : 修正bool沒有跟著存讀檔
                            *Temp->bParameter=(atoi(CEd->Text.c_str())==0)?false:true;
                        else if(Cbb!=NULL)
                            *Temp->bParameter=(Cbb->ItemIndex==0)?false:true;
                        else if(RdG!=NULL)
                            *Temp->bParameter=(RdG->ItemIndex==0)?false:true;
                        else if(Chb!=NULL)
                            *Temp->bParameter=Chb->Checked;
                    }
                    else
                    {
                        if(CEd!=NULL)
                            *Temp->sParameter=CEd->Text;
                    }
                }
                else
                {
                    if(ContentType==ECInteger || ContentType==ECPosInt || ContentType==ECNegInt || ContentType==ECPort)
                    {
                        if(CEd!=NULL)
                            *Temp->iParameter=atoi(CEd->Text.c_str());
                    }
                    else if(ContentType==ECDouble || ContentType==ECPosDouble || ContentType==ECNegDouble || ContentType==ECPercent)    //浮點數
                    {
                        if(CEd!=NULL)
                            *Temp->dParameter=atof(CEd->Text.c_str());
                        else if(Ttp!=NULL)
                            *Temp->dParameter=Ttp->DateTime;
                    }
                    else if(ContentType==ECBool)
                    {
                        if(CEd!=NULL)                                           //Steven 20230502 : 修正bool沒有跟著存讀檔
                            *Temp->bParameter=(atoi(CEd->Text.c_str())==0)?false:true;
                        else if(Cbb!=NULL)
                            *Temp->bParameter=(Cbb->ItemIndex==0)?false:true;
                        else if(RdG!=NULL)
                            *Temp->bParameter=(RdG->ItemIndex==0)?false:true;
                        else if(Chb!=NULL)
                            *Temp->bParameter=Chb->Checked;
                    }
                    else
                    {
                        if(CEd!=NULL)
                            *Temp->sParameter=CEd->Text;
                    }
                }
            }
            else
            {
                bResult=false;
                if(bAlarmLimitation)
                {
                    Str2=mapKeyValue[Str];
                    Str3.sprintf("%s %s got duplicate read position with %s::%s!!", Str, Str2, sParentName, sEditName);
                    Application->MessageBoxA(Str3.c_str(), "WARNING!", MB_OK);
                }
            }
        }
    }

    ini->UpdateFile();
    delete ini;
    return bResult;
}
//---------------------------------------------------------------------------
void HTEditList::InitialDataToEdit()
{
    double dCurr;
    AnsiString Str;
    TEditContent ContentType;
    for(int i=0; i<FEditList->Count; i++)
    {
        THTEdit *Temp;
        Temp=(THTEdit*)FEditList->Items[i];

        ContentType=Temp->Content;
        TCustomEdit *CEd = dynamic_cast <TCustomEdit *>(Temp->SourceControl);
        TComboBox   *Cbb = dynamic_cast <TComboBox   *>(Temp->SourceControl);
        TRadioGroup *RdG = dynamic_cast <TRadioGroup *>(Temp->SourceControl);
        TCheckBox   *Chb = dynamic_cast <TCheckBox   *>(Temp->SourceControl);   //Steven 20230224 : Edit List改支援多型態元件
        TDateTimePicker *Ttp = dynamic_cast <TDateTimePicker   *>(Temp->SourceControl);

        if(ContentType==ECInteger || ContentType==ECPosInt || ContentType==ECNegInt || ContentType==ECPort)        //數字
        {
            if(CEd!=NULL)
            {
                if(Temp->iTransformType==EUuMToMM)                              //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                {
                    Str=(AnsiString)*Temp->iParameter;
                    dCurr=ConvertIntStrToDouble(Str, 100.0);
                    Str.sprintf("%0.3f", dCurr);
                }
                else if(Temp->iTransformType==EUMSToSec)                        //顯示為Sec, 存檔為MS
                {
                    Str=(AnsiString)*Temp->iParameter;
                    dCurr=ConvertIntStrToDouble(Str, 1000.0);
                    Str.sprintf("%0.3f", dCurr);
                }
                else
                {
                    Str=AnsiString(*Temp->iParameter);
                }
                CEd->Text=AnsiString(Str);
            }
            else if(Cbb!=NULL)
            {
                Cbb->ItemIndex=(*Temp->iParameter);
            }
            else if(RdG!=NULL)
            {
                RdG->ItemIndex=(*Temp->iParameter);
            }
        }
        else if(ContentType==ECDouble || ContentType==ECPosDouble || ContentType==ECNegDouble || ContentType==ECPercent)    //浮點數
        {
            if(CEd!=NULL)
            {
                if(Temp->iTransformType==EUuMToMM)                              //顯示為MM, 存檔為uM //Steven 20230905 : HTEditList加入型態轉換的Flag
                    Str.sprintf("%0.3f", (double)*Temp->dParameter/(double)100.0);
                else if(Temp->iTransformType==EUMSToSec)                        //顯示為Sec, 存檔為MS
                    Str.sprintf("%0.3f", (double)*Temp->dParameter/(double)1000.0);
                else
                    Str=AnsiString(*Temp->dParameter);

                CEd->Text=Str;
            }
            else if(Ttp!=NULL)
            {
                Ttp->DateTime=*Temp->dParameter;
            }
        }
        else if(ContentType==ECBool)
        {
            if(Chb!=NULL)
                Chb->Checked=(*Temp->bParameter);                               //Steven 20230502 : 修正bool沒有跟著存讀檔
        }
        else
        {
            if(CEd!=NULL)
                CEd->Text=AnsiString(*Temp->sParameter);
        }

        Temp->SourceControl->Visible=Temp->bVisible;
        Temp->SourceControl->Enabled=Temp->bEnable;

        if(bFontBlue)                                                           //Sam 20220921 : config儲存跟隨recipe
        {
            TEdit        *Ed = dynamic_cast <TEdit *>(Temp->SourceControl);
            TLabeledEdit *LEd= dynamic_cast <TLabeledEdit *>(Temp->SourceControl);
            if(Ed!=NULL)
            {
                Ed->Font->Color=clBlue;
                Ed->ShowHint=true;
                Ed->Hint="This parameter by setup file";
            }
            else if(LEd!=NULL)
            {
                LEd->Font->Color=clBlue;
                LEd->ShowHint=true;
                LEd->Hint="This parameter by setup file";
            }
            else if(Cbb!=NULL)
            {
                Cbb->Font->Color=clBlue;
                Cbb->ShowHint=true;
                Cbb->Hint="This parameter by setup file";
            }
            else if(RdG!=NULL)
            {
                RdG->Font->Color=clBlue;
                RdG->ShowHint=true;
                RdG->Hint="This parameter by setup file";
            }
            else if(Chb!=NULL)
            {
                Chb->Font->Color=clBlue;
                Chb->ShowHint=true;
                Chb->Hint="This parameter by setup file";
            }
            else if(Ttp!=NULL)
            {
                Ttp->Font->Color=clBlue;
                Ttp->ShowHint=true;
                Ttp->Hint="This parameter by setup file";
            }
        }
    }
}
//---------------------------------------------------------------------------
enum eCustomLimit
{
    eFormName   =0,
    eEditName   =1,
    eDataType   =2,
    eDefaultMin =3,
    eMinimum    =4,
    eDefaultMax =5,
    eMaxmum     =6,

    eCustomLimitTotal
};
//---------------------------------------------------------------------------
//把元件設定存成CSV, 方便客戶使用Excel進行編輯
//FileName由外部的 SaveFileDialog去取得
//---------------------------------------------------------------------------
void HTEditList::ExportCustomLimitToCSV(AnsiString CsvFileName)
{
    TStringList *sList;
    sList=new TStringList();
    sList->Clear();

    TStringList *slFormat;
    slFormat=new TStringList();
    slFormat->Clear();

    //第一行的標題列-------------------
    for(int j=0; j<eCustomLimitTotal; j++)                                      //預設填入空字串
    {
        slFormat->Add(" ");
    }
    slFormat->Strings[eFormName]    ="Form Name";
    slFormat->Strings[eEditName]    ="Edit Name";
    slFormat->Strings[eDataType]    ="Data Type";
    slFormat->Strings[eDefaultMin]  ="Default Minmum Value";
    slFormat->Strings[eDefaultMax]  ="Default Maxmum Value";
    slFormat->Strings[eMinimum]     ="Custom Minmum Value";
    slFormat->Strings[eMaxmum]      ="Custom Maxmum Value";
    //第一行的標題列-------------------

    for(int i=0; i<FEditList->Count; i++)
    {
        THTEdit *Temp;
        Temp=(THTEdit*)FEditList->Items[i];

        if(Temp->Content==ECInteger    ||           //只存入有數值的
           Temp->Content==ECDouble     ||
           Temp->Content==ECPosInt     ||
           Temp->Content==ECPosDouble  ||
           Temp->Content==ECNegInt     ||
           Temp->Content==ECNegDouble)
        {
            slFormat->Clear();
            for(int j=0; j<eCustomLimitTotal; j++)  //預設填入空字串
            {
                slFormat->Add(" ");
            }

            if(Temp->SourceControl->Parent!=NULL)
                slFormat->Strings[eFormName]   =Temp->SourceControl->Parent->Name;

            if(Temp->SourceControl!=NULL)
                slFormat->Strings[eEditName]   =Temp->SourceControl->Name;

            switch(Temp->Content)                   //註記資料的型態
            {
                case ECText:        slFormat->Strings[eDataType]="Text";             break;
                case ECInteger:     slFormat->Strings[eDataType]="Integer";          break;
                case ECDouble:      slFormat->Strings[eDataType]="Double";           break;
                case ECPosInt:      slFormat->Strings[eDataType]="Postive Integer";  break;
                case ECPosDouble:   slFormat->Strings[eDataType]="Postive Double";   break;
                case ECNegInt:      slFormat->Strings[eDataType]="Negtive Integer";  break;
                case ECNegDouble:   slFormat->Strings[eDataType]="Negtive Double";   break;
                case ECFileName:    slFormat->Strings[eDataType]="FileName";         break;
                case ECPassword:    slFormat->Strings[eDataType]="Password";         break;
            }

            slFormat->Strings[eDefaultMin]  =Temp->MinValue;
            slFormat->Strings[eDefaultMax]  =Temp->MaxValue;
            slFormat->Strings[eMinimum]     =Temp->CustomMaxValue;
            slFormat->Strings[eMaxmum]      =Temp->CustomMinValue;
            sList->Add(slFormat->CommaText);
        }
    }
    sList->SaveToFile(CsvFileName);
    delete sList;
    delete slFormat;
}
//---------------------------------------------------------------------------
//把元件設定從CSV轉成INI, 方便客戶使用Excel進行編輯完之後, 放到機台上使用
//FileName由外部的 OpenFileDialog去取得
//---------------------------------------------------------------------------
void HTEditList::ImportCustomLimitationFromCSV(AnsiString CSVFileName, AnsiString IniFileName)
{
    TMemIniFile *MyIniFile;
    AnsiString sGroup, sKey, sMax, sMin, sKeyName;

    if(FileExists(CSVFileName)==false)
    {
        sKeyName.sprintf("CSV file not exists!! (%s)", CSVFileName);
        Application->MessageBoxA(sKeyName.c_str(), "WARNING!", MB_OK);
        return;
    }

    TStringList *sList;
    sList=new TStringList();
    sList->Clear();

    TStringList *slFormat;
    slFormat=new TStringList();
    slFormat->Clear();

    if(FileExists(IniFileName))
        DeleteFile(IniFileName);

    MyIniFile=new TMemIniFile(IniFileName);
    sList->LoadFromFile(CSVFileName);
    for(int i=0; i<FEditList->Count; i++)
    {
        slFormat->Clear();
        slFormat->Text=sList->Strings[i];

        if(slFormat->Count==eCustomLimitTotal)                                  //數量一定要符合
        {
            sGroup  =slFormat->Strings[eFormName];
            sKey    =slFormat->Strings[eEditName];
            sMax    =slFormat->Strings[eMaxmum];
            sMin    =slFormat->Strings[eMinimum];

            sKeyName.sprintf("%s_CustomMax", sKey);
            MyIniFile->WriteString(sGroup, sKeyName, sMax);

            sKeyName.sprintf("%s_CustomMin", sKey);
            MyIniFile->WriteString(sGroup, sKeyName, sMin);
        }
    }
    delete sList;
    delete slFormat;
    delete MyIniFile;
}
//---------------------------------------------------------------------------
//把客戶設定的最大最小值寫到INI檔案
//---------------------------------------------------------------------------
//void HTEditList::SaveCustomLimitationToIni(AnsiString IniFileName)
//{
//    TMemIniFile *MyIniFile;
//    AnsiString sGroup, sKey, sMax, sMin, sKeyName;
//    MyIniFile=new TMemIniFile(IniFileName);
//    for(int i=0; i<FEditList->Count; i++)
//    {
//        THTEdit *Temp;
//        Temp=(THTEdit*)FEditList->Items[i];
//
//        if(Temp->Content==ECInteger    ||           //只針對格式是數值的
//           Temp->Content==ECDouble     ||
//           Temp->Content==ECPosInt     ||
//           Temp->Content==ECPosDouble  ||
//           Temp->Content==ECNegInt     ||
//           Temp->Content==ECNegDouble  )
//        {
//            sGroup  =Temp->SourceEdit->Parent->Name;
//            sKey    =Temp->SourceEdit->Name;
//            sMax    =Temp->CustomMaxValue;
//            sMin    =Temp->CustomMinValue;
//
//            sKeyName.sprintf("%s_CustomMax", sKey);
//            MyIniFile->WriteString(sGroup, sKeyName, sMax);
//
//            sKeyName.sprintf("%s_CustomMin", sKey);
//            MyIniFile->WriteString(sGroup, sKeyName, sMin);
//        }
//    }
//    delete MyIniFile;
//}
//---------------------------------------------------------------------------
//把客戶設定的最大最小值讀進來
//---------------------------------------------------------------------------
void HTEditList::ReadCustomLimitationFromIni(AnsiString IniFileName)
{
    TMemIniFile *MyIniFile;
    AnsiString sGroup, sKey, sMax, sMin, sKeyName, sCusMax, sCusMin;

    if(FileExists(IniFileName)==false)
    {
        sKeyName.sprintf("INI file not exists!! (%s)", IniFileName);
        Application->MessageBoxA(sKeyName.c_str(), "WARNING!", MB_OK);
        return;
    }

    MyIniFile=new TMemIniFile(IniFileName);
    for(int i=0; i<FEditList->Count; i++)
    {
        THTEdit *Temp;
        Temp=(THTEdit*)FEditList->Items[i];

        if(Temp->Content==ECInteger    ||                                       //只針對格式是數值的
           Temp->Content==ECDouble     ||
           Temp->Content==ECPosInt     ||
           Temp->Content==ECPosDouble  ||
           Temp->Content==ECNegInt     ||
           Temp->Content==ECNegDouble  )
        {
            sGroup  =Temp->SourceControl->Parent->Name;
            sKey    =Temp->SourceControl->Name;
            sMax    =Temp->MaxValue;
            sMin    =Temp->MinValue;

            sKeyName.sprintf("%s_CustomMax", sKey);
            sCusMax=MyIniFile->ReadString(sGroup, sKeyName, Temp->MaxValue);

            sKeyName.sprintf("%s_CustomMin", sKey);
            sCusMin=MyIniFile->ReadString(sGroup, sKeyName, Temp->MinValue);

            Temp->SetCustomLimitation(sCusMax, sCusMin);
        }
    }
    delete MyIniFile;
}
//---------------------------------------------------------------------------
void HTEditList::SetFontBlue()                                                  //Sam 20220921 : config儲存跟隨recipe
{
    bFontBlue=true;
}
//---------------------------------------------------------------------------
bool HTEditList::ReadAndCompareDataFromFile(AnsiString asNameWithPath1, AnsiString asNameWithPath2,TList *tlDiff,AnsiString &asError)     //JimmyChiu 20230504 : 新增資料比對顯示function
{
    if(FileExists(asNameWithPath1)==false)
    {
        asError=AnsiString("File does not exist:")+asNameWithPath1;
        return false;
    }
    if(FileExists(asNameWithPath2)==false)
    {
        asError=AnsiString("File does not exist:")+asNameWithPath2;
        return false;
    }
    if(tlDiff==NULL)
        tlDiff=new TList;
    //
    TMemIniFile *file1=new TMemIniFile(asNameWithPath1);
    TMemIniFile *file2=new TMemIniFile(asNameWithPath2);
    AnsiString as1="",as2="",asGet="";
    int i1=0,i2=0;
    for(int i=0; i<FEditList->Count; i++)
    {
        THTEdit *Temp;
        Temp=(THTEdit*)FEditList->Items[i];
        if(Temp->bReadFromFile==true)
        {
            TCustomEdit *CEd = dynamic_cast <TCustomEdit *>(Temp->SourceControl);
            TComboBox   *Cbb = dynamic_cast <TComboBox   *>(Temp->SourceControl);
            TRadioGroup *RdG = dynamic_cast <TRadioGroup *>(Temp->SourceControl);
            TCheckBox   *Chb = dynamic_cast <TCheckBox   *>(Temp->SourceControl);
            if(CEd!=NULL)
            {
                as1=file1->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                as2=file2->ReadString(Temp->IniGroupName, Temp->IniKeyName, Temp->DefaultValue);
                if(as1!=as2)
                {
                    AddDiffData(tlDiff,Temp->IniKeyName,as1,as2);
                }
            }
            else if(Cbb!=NULL)
            {
                i1=file1->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, 0);
                i2=file2->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, 0);
                if(i1!=i2)
                {
                    AddDiffData(tlDiff,Cbb->Text,IntToStr(i1),IntToStr(i2));
                }
            }
            else if(RdG!=NULL)
            {
                i1=file1->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, 0);
                i2=file2->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, 0);
                if(i1!=i2)
                {
                    AddDiffData(tlDiff,RdG->Caption,IntToStr(i1),IntToStr(i2));
                }
            }
            else if(Chb!=NULL)
            {
                i1=file1->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, 0);
                i2=file2->ReadInteger(Temp->IniGroupName, Temp->IniKeyName, 0);
                if(i1!=i2)
                {
                    AddDiffData(tlDiff,Chb->Caption,IntToStr(i1),IntToStr(i2));
                }
            }
        }
    }
    delete file1;
    delete file2;
    return true;
}
//---------------------------------------------------------------------------
void HTEditList::AddDiffData(TList *tdiff,AnsiString asContent,AnsiString a1,AnsiString a2)//JimmyChiu 20230504 : 新增資料比對顯示function
{
    TStringList* tslDiffcontent=new TStringList;
    tslDiffcontent->Add(asContent);
    tslDiffcontent->Add(a1);
    tslDiffcontent->Add(a2);
    tdiff->Add(tslDiffcontent);
}
//---------------------------------------------------------------------------
void HTEditList::SetItemLimit(THTEdit *Item,const TEditContent &Content,const AnsiString &sMin,const AnsiString &sMax)
{
    if(sMax==AnsiString("") && sMin==AnsiString(""))
    {
        Item->CheckRange=false;
    }
    else if(Content==ECPort)                                                    //通訊埠
    {
        Item->CheckRange=true;
        Item->MaxValue=65535;
        Item->MinValue=0;
    }
    else if(Content==ECBool)                                                    //Steven 20230502 : 修正bool沒有跟著存讀檔
    {
        Item->CheckRange=true;
        Item->MaxValue=1;
        Item->MinValue=0;
    }
    else if(Content==ECInteger    ||    //數字
            Content==ECPosInt     ||    //正整數
            Content==ECNegInt)          //通訊埠
    {
        Item->CheckRange=true;
        if(atoi(sMin.c_str())>atoi(sMax.c_str()))
        {
            Item->MaxValue=sMin;
            Item->MinValue=sMax;
        }
        else
        {
            Item->MaxValue=sMax;
            Item->MinValue=sMin;
        }
    }
    else if(Content==ECDouble     ||    //浮點數
            Content==ECPosDouble  ||    //正浮點數
            Content==ECNegDouble  ||    //負浮點數
            Content==ECPercent)         //百分比
    {
        Item->CheckRange=true;
        if(atof(sMin.c_str())>atof(sMax.c_str()))
        {
            Item->MaxValue=sMin;
            Item->MinValue=sMax;
        }
        else
        {
            Item->MaxValue=sMax;
            Item->MinValue=sMin;
        }
    }
}
//---------------------------------------------------------------------------
//把預設的最大最小值寫到INI檔案
//---------------------------------------------------------------------------
//void HTEditList::SaveCustomLimitationToIni(AnsiString IniFileName)
//{
//    TMemIniFile *MyIniFile;
//    AnsiString sGroup, sKey, sMax, sMin, sKeyName;
//    MyIniFile=new TMemIniFile(IniFileName);
//    for(int i=0; i<FEditList->Count; i++)
//    {
//        THTEdit *Temp;
//        Temp=(THTEdit*)FEditList->Items[i];
//
//        if(Temp->Content==ECInteger    ||           //只針對格式是數值的
//           Temp->Content==ECDouble     ||
//           Temp->Content==ECPosInt     ||
//           Temp->Content==ECPosDouble  ||
//           Temp->Content==ECNegInt     ||
//           Temp->Content==ECNegDouble  )
//        {
//            sGroup  =Temp->SourceEdit->Parent->Name;
//            sKey    =Temp->SourceEdit->Name;
//            sMax    =Temp->MaxValue;
//            sMin    =Temp->MinValue;
//
//            sKeyName.sprintf("%s_Max", sKey);
//            MyIniFile->WriteString(sGroup, sKeyName, sMax);
//
//            sKeyName.sprintf("%s_Min", sKey);
//            MyIniFile->WriteString(sGroup, sKeyName, sMin);
//        }
//    }
//    delete MyIniFile;
//}
//---------------------------------------------------------------------------
//把預設的最大最小值讀進來
//---------------------------------------------------------------------------
//void HTEditList::ReadCustomLimitationFromIni(AnsiString IniFileName)
//{
//    TMemIniFile *MyIniFile;
//    AnsiString sGroup, sKey, sMax, sMin, sKeyName, sDefMax, sDefMin;
//    MyIniFile=new TMemIniFile(IniFileName);
//
//    if(FileExists(CSVFileName)==false)
//    {
//        sKeyName.sprintf("INI file not exists!! (%s)", IniFileName);
//        Application->MessageBoxA(sKeyName, "WARNING!", MB_OK);
//        return;
//    }
//
//    MyIniFile=new TMemIniFile(IniFileName);
//    for(int i=0; i<FEditList->Count; i++)
//    {
//        THTEdit *Temp;
//        Temp=(THTEdit*)FEditList->Items[i];
//
//        if(Temp->Content==ECInteger    ||           //只針對格式是數值的
//           Temp->Content==ECDouble     ||
//           Temp->Content==ECPosInt     ||
//           Temp->Content==ECPosDouble  ||
//           Temp->Content==ECNegInt     ||
//           Temp->Content==ECNegDouble  )
//        {
//            sGroup  =Temp->SourceEdit->Parent->Name;
//            sKey    =Temp->SourceEdit->Name;
//            sMax    =Temp->MaxValue;
//            sMin    =Temp->MinValue;
//
//            sKeyName.sprintf("%s_Max", sKey);
//            sDefMax=MyIniFile->ReadString(sGroup, sKeyName, Temp->MaxValue);
//
//            sKeyName.sprintf("%s_Min", sKey);
//            sDefMin=MyIniFile->ReadString(sGroup, sKeyName, Temp->MinValue);
//
//            Temp->SetDefaultLimitation(sDefMax, sDefMin);
//        }
//    }
//    delete MyIniFile;
//}
//---------------------------------------------------------------------------
//HTCheckBoxList::HTCheckBoxList()
//{
//    CheckBoxList=new TList;
//    bAlarmLimitation=false;
//    bFontBlue=false;    //Sam 20220919 : 設定 Config 字型顏色
//}
//---------------------------------------------------------------------------
//HTCheckBoxList::~HTCheckBoxList()
//{
//    try
//    {
//        for(int i=CheckBoxList->Count-1; i>=0; i--)
//            delete CheckBoxList->Items[i];
//
//        ClearALL();
//        delete CheckBoxList;
//        CheckBoxList=NULL;
//    }
//    catch(...)
//    {
//        MyDBIProcess("Exception", "~HTCheckBoxList");
//    }
//}
//---------------------------------------------------------------------------
//void HTCheckBoxList::Add(TCheckBox *SoureCB,
//                         bool *Par,
//                         AnsiString GroupName,
//                         AnsiString KeyName,
//                         bool bVisible,
//                         bool bEnable,
//                         bool bReadFromFile,        //是否從檔案讀取
//                         bool bForceValue)          //如果不是從檔案讀取的預設值
//{
//    if(SoureCB!=NULL)
//    {
//        THTCheckBox *Item=new THTCheckBox;
//        Item->CheckBox      =SoureCB;
//        Item->bParameter    =Par;
//        Item->SetFileInformation(GroupName, KeyName);
//
//        Item->bVisible=bVisible;
//        Item->bEnable=bEnable;
//        Item->bReadFromFile=bReadFromFile;
//        Item->bForceValue=bForceValue;
//        CheckBoxList->Add(Item);
//        Item->GetDefaultPosition();
//        Item->iTag=CheckBoxList->Count-1;
//        Item->CheckBox->Tag=CheckBoxList->Count-1;                              //Steven 20230218 : 加上Tag, 方便使用陣列
//    }
//}
//---------------------------------------------------------------------------
//void HTCheckBoxList::ClearALL()
//{
//    if(CheckBoxList!=NULL)
//        CheckBoxList->Clear();
//}
//---------------------------------------------------------------------------
//bool HTCheckBoxList::SaveCheckBoxToFile(AnsiString Path, AnsiString FileName)
//{
//    bool bResult=true;
//    bool ret;
//    AnsiString sFullFileName=Path+FileName;
//    AnsiString sParentName, sCBName;
//    MyForceDirectories(Path);
//
//    TMemIniFile *ini;
//    ini = new TMemIniFile(sFullFileName);
//
//    AnsiString Str, Str2, Str3, Str4, Str5;
//    map<AnsiString, AnsiString>mapKeyValue;
//    map<AnsiString, AnsiString>::iterator iterKeyValue;
//
//    mapKeyValue.clear();
//    for(int i=0; i<CheckBoxList->Count; i++)
//    {
//        THTCheckBox *Temp;
//        Temp=(THTCheckBox*)CheckBoxList->Items[i];
//        if(Temp->CheckBox->Parent!=NULL)
//        {
//            TWinControl* PP=Temp->CheckBox->Parent;
//            sParentName=PP->Name;
//        }
//        else
//        {
//            sParentName="NoParent";
//        }
//        sCBName=Temp->CheckBox->Name;
//
//        if(Temp->IniGroupName=="" || Temp->IniKeyName=="")
//        {
//            if(bAlarmLimitation)
//            {
//                if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
//                    Str2.sprintf("Please check the group and key for Check Box. @%s, [%s]->%s", sParentName, Temp->IniGroupName, Temp->IniGroupName);
//                else
//                    Str2.sprintf("Please check the group and key for Check Box. %s->%s", sParentName, sCBName);
//                Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
//            }
//            bResult=false;
//        }
//        else
//        {
//            Str.sprintf("%s_%s", Temp->IniGroupName, Temp->IniKeyName);
//            iterKeyValue=mapKeyValue.find(Str);
//
//            if(Temp->bReadFromFile==false)
//            {
//            }
//            else if(iterKeyValue==mapKeyValue.end() || mapKeyValue.begin()==mapKeyValue.end())  //確認是不是不同元件在同一個位置存檔
//            {
//                Str2.sprintf("%s::%s", sParentName, sCBName);
//                mapKeyValue[Str]=Str2;
//
//                //Steven 20200514 : 加上Change Log
//                //==>
//                ret=ini->ReadBool(Temp->IniGroupName, Temp->IniKeyName, false);
//                if(ret!=Temp->CheckBox->Checked)
//                {
//                    Str4.sprintf("%s_%s change Value", Temp->IniGroupName , Temp->IniKeyName);
//                    Str5.sprintf("%d==>%d", ret, Temp->CheckBox->Checked);
//                    RecordChangeLogProcess(Str4.c_str(), Str5.c_str());       //wei 20180625 offset Change log紀錄
//                }
//                //<==
//                //Steven 20200514 : 加上Change Log
//
//                ini->WriteBool(Temp->IniGroupName, Temp->IniKeyName, Temp->CheckBox->Checked);
//                if(Temp->bParameter!=NULL)
//                    *Temp->bParameter=Temp->CheckBox->Checked;
//            }
//            else
//            {
//                bResult=false;
//                Str2=mapKeyValue[Str];
//                if(bAlarmLimitation)
//                {
//                    Str3.sprintf("%s %s got duplicate save position with %s::%s!!", Str, Str2, sParentName, sCBName);
//                    Application->MessageBoxA(Str3.c_str(), "WARNING!", MB_OK);
//                }
//            }
//        }
//    }
//
//    ini->UpdateFile();
//    delete ini;
//
//    return bResult;
//}
//---------------------------------------------------------------------------
//bool HTCheckBoxList::ReadCheckBoxFromFile(AnsiString Path, AnsiString FileName)
//{
//    bool bResult=true;
//    AnsiString Str, Str2, Str3;
//    AnsiString sFullFileName=Path+"\\"+FileName;
////    AnsiString sFullFileName=Path+FileName;                                 //kevin 20200827 add "\\"
//    AnsiString sParentName, sCbName;
//    MyForceDirectories(Path);
//
//    TMemIniFile *ini;
//    ini = new TMemIniFile(sFullFileName);
//    map<AnsiString, AnsiString>mapKeyValue;
//    map<AnsiString, AnsiString>::iterator iterKeyValue;
//
//    mapKeyValue.clear();
//    for(int i=0; i<CheckBoxList->Count; i++)
//    {
//        THTCheckBox *Temp;
//        Temp=(THTCheckBox*)CheckBoxList->Items[i];
//
//        if(Temp->CheckBox->Parent!=NULL)
//        {
//            TWinControl* PP=Temp->CheckBox->Parent;
//            sParentName=PP->Name;
//        }
//        else
//        {
//            sParentName="NoParent";
//        }
//        sCbName  =Temp->CheckBox->Name;
//
//        if(Temp->bReadFromFile==false)
//        {
//            Temp->CheckBox->Checked=Temp->bForceValue;
//            *Temp->bParameter=Temp->CheckBox->Checked;
//        }
//        else if(Temp->IniGroupName=="" || Temp->IniKeyName=="")
//        {
//            if(bAlarmLimitation)
//            {
//                if(Temp->IniGroupName!="" && Temp->IniGroupName!="")
//                    Str2.sprintf("Please check the group and key for Check Box. @%s, [%s]->%s", sParentName, Temp->IniGroupName, Temp->IniGroupName);
//                else
//                    Str2.sprintf("Please check the group and key for Check Box. %s->%s", sParentName, sCbName);
//                Application->MessageBoxA(Str2.c_str(), "WARNING!", MB_OK);
//            }
//        }
//        else
//        {
//            Str.sprintf("%s_%s", Temp->IniGroupName, Temp->IniKeyName);
//            iterKeyValue=mapKeyValue.find(Str);
//            if(iterKeyValue==mapKeyValue.end() || mapKeyValue.begin()==mapKeyValue.end())  //確認是不是不同元件在同一個位置存檔
//            {
//                Str2.sprintf("%s::%s", sParentName, sCbName);
//                mapKeyValue[Str]=Str2;
//                Temp->CheckBox->Checked=ini->ReadBool(Temp->IniGroupName, Temp->IniKeyName, Temp->bForceValue);
//                *Temp->bParameter=Temp->CheckBox->Checked;
//            }
//            else
//            {
//                bResult=false;
//                if(bAlarmLimitation)
//                {
//                    Str2=mapKeyValue[Str];
//                    Str3.sprintf("%s %s got duplicate read position with %s::%s!!", Str, Str2, sParentName, sCbName);
//                    Application->MessageBoxA(Str3.c_str(), "WARNING!", MB_OK);
//                }
//            }
//        }
//    }
//
//    ini->UpdateFile();
//    delete ini;
//    return bResult;
//}
//---------------------------------------------------------------------------
//void HTCheckBoxList::InitialDataToCheckBox()
//{
//    for(int i=0; i<CheckBoxList->Count; i++)
//    {
//        THTCheckBox *Temp;
//        Temp=(THTCheckBox*)CheckBoxList->Items[i];
//        Temp->CheckBox->Checked=*Temp->bParameter;
//        Temp->CheckBox->Visible=Temp->bVisible;
//        Temp->CheckBox->Enabled=Temp->bEnable;
//        if(bFontBlue)   //Sam 20220919 : 設定 Config 字型顏色
//        {
//            Temp->CheckBox->Font->Color= clBlue;
//            Temp->CheckBox->ShowHint=true;
//            Temp->CheckBox->Hint="This parameter by setup file";
//        }
//    }
//}
//---------------------------------------------------------------------------
//void HTCheckBoxList::SetFontBlue()                                            //Sam 20220919 : 設定 Config 字型顏色
//{
//    bFontBlue=true;
//}
//---------------------------------------------------------------------------
int uBasicPickPlace::GetIntegerFromJSON(cJSON* root, AnsiString TagName)
{
    cJSON* _obj=cJSON_GetObjectItem(root, TagName.c_str());
    int iReturn=0;
    if(_obj!=NULL && _obj->type==cJSON_Number)
    {
        iReturn=_obj->valueint;
    }
    return iReturn;
}
//---------------------------------------------------------------------------
double uBasicPickPlace::GetDoubleFromJSON(cJSON* root, AnsiString TagName)
{
    cJSON* _obj=cJSON_GetObjectItem(root, TagName.c_str());
    double dReturn=0.0;
    if(_obj!=NULL && _obj->type==cJSON_Number)
    {
        dReturn=_obj->valuedouble;
    }
    return dReturn;
}
//---------------------------------------------------------------------------
void uBasicPickPlace::DataForwardAndDelete(TList* list)
{
    if(list->Count==0)
    {
        return;
    }
    for(int i=1; i<list->Count; i++)
    {
        list->Move(i, i-1);
    }
    list->Delete(list->Count-1);
}
//---------------------------------------------------------------------------
AnsiString uBasicPickPlace::GetStringFromJSON(cJSON* root, AnsiString TagName)
{
    cJSON* _obj=cJSON_GetObjectItem(root, TagName.c_str());
    AnsiString sReturn="";
    if(_obj!=NULL && cJSON_IsString(_obj)==true)
    {
        sReturn=_obj->valuestring;
    }
    return sReturn;
}
//---------------------------------------------------------------------------
uHPSuckTeam::uHPSuckTeam()
{
    ClearHPSuckTeam();
}
//---------------------------------------------------------------------------
uHPSuckTeam::uHPSuckTeam(int _iP, int _iR, int _iC, int _iSht, int _iKit)
{
    ClearHPSuckTeam();
    iP  =_iP;
    iR  =_iR;
    iC  =_iC;
    iSht=_iSht;
    iKit=_iKit;
}
//---------------------------------------------------------------------------
uHPSuckTeam::~uHPSuckTeam()
{
    try
    {
        ClearHPSuckTeam();
    }
    catch(...)
    {
    }
}
//---------------------------------------------------------------------------
int uHPSuckTeam::GetUseSuckCol()
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            if(bSuck[i][j]==true)
            {
                return j;
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
void uHPSuckTeam::ClearHPSuckTeam()
{
    iP      =0;
    iR      =0;
    iC      =0;
    iSht    =0;
    iKit    =0;

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            iSite[i][j]     =-1;
            iPlateR[i][j]   =-1;
            iPlateC[i][j]   =-1;
            bSuck[i][j]     =false;
        }
    }
}
//---------------------------------------------------------------------------
uHPSuckTeam* uHPSuckTeam::Clone()
{
    uHPSuckTeam* clonedTeam = new uHPSuckTeam();
    clonedTeam->iP   =this->iP;
    clonedTeam->iR   =this->iR;
    clonedTeam->iC   =this->iC;
    clonedTeam->iSht =this->iSht;
    clonedTeam->iKit =this->iKit;
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            clonedTeam->iSite[i][j]  =this->iSite[i][j];
            clonedTeam->iPlateR[i][j]=this->iPlateR[i][j];
            clonedTeam->iPlateC[i][j]=this->iPlateC[i][j];
            clonedTeam->bSuck[i][j]  =this->bSuck[i][j];
        }
    }
    return clonedTeam;
}
//---------------------------------------------------------------------------
void uHPSuckTeam::SaveJSONFile(cJSON* root)
{
    if(root==NULL)
        return;
    cJSON_AddNumberToObject(root, "P",   iP);
    cJSON_AddNumberToObject(root, "Col", iC);
    cJSON_AddNumberToObject(root, "Row", iR);
    cJSON_AddNumberToObject(root, "Sht", iSht);
    cJSON_AddNumberToObject(root, "Kit", iKit);
    SavePlateRow(root);
    SavePlateCol(root);
    SavePlateSite(root);
    SavePlateSuck(root);
}
//---------------------------------------------------------------------------
void uHPSuckTeam::LoadJSONFile(cJSON* root)
{
    if(root==NULL)
        return;
    iP      =GetIntegerFromJSON(root, "P");
    iC      =GetIntegerFromJSON(root, "Col");
    iR      =GetIntegerFromJSON(root, "Row");
    iSht    =GetIntegerFromJSON(root, "Sht");
    iKit    =GetIntegerFromJSON(root, "Kit");
    ReadPlateRowFromJSON(root);
    ReadPlateColFromJSON(root);
    ReadPlateSiteFromJSON(root);
    ReadPlateSuckFromJSON(root);
}
//---------------------------------------------------------------------------
void uHPSuckTeam::SavePlateCol(cJSON* root)
{
    if(root==NULL)
        return;
    AnsiString asVarName="";
    int iArr[MAX_ARM_Row*MAX_ARM_Col];
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            iArr[i*MAX_ARM_Col+j]=iPlateC[i][j];
        }
    }
    cJSON *jarr=cJSON_CreateIntArray(&iArr[0], MAX_ARM_Row*MAX_ARM_Col);
    cJSON_AddItemToObject(root, "PlateCol", jarr);
}
//---------------------------------------------------------------------------
void uHPSuckTeam::ReadPlateColFromJSON(cJSON* root)
{
    cJSON *array_obj=cJSON_GetObjectItem(root, "PlateCol");
    if(cJSON_IsArray(array_obj))
    {
        int array_size=cJSON_GetArraySize(array_obj);
        if(array_size!=MAX_ARM_Row*MAX_ARM_Col)
            return;
        for(int i=0; i<array_size; i++)
        {
            cJSON *item=cJSON_GetArrayItem(array_obj, i);
            if (cJSON_IsNumber(item))
            {
                iPlateC[i/MAX_ARM_Col][i%MAX_ARM_Col]=item->valueint;
            }
        }
    }
}
//---------------------------------------------------------------------------
void uHPSuckTeam::SavePlateRow(cJSON* root)
{
    if(root==NULL)
        return;
    AnsiString asVarName="";
    int iArr[MAX_ARM_Row*MAX_ARM_Col];
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            iArr[i*MAX_ARM_Col+j]=iPlateR[i][j];
        }
    }
    cJSON *jarr=cJSON_CreateIntArray(&iArr[0], MAX_ARM_Row*MAX_ARM_Col);
    cJSON_AddItemToObject(root, "PlateRow", jarr);
}
//---------------------------------------------------------------------------
void uHPSuckTeam::ReadPlateRowFromJSON(cJSON* root)
{
    cJSON *array_obj=cJSON_GetObjectItem(root, "PlateRow");
    if(cJSON_IsArray(array_obj))
    {
        int array_size=cJSON_GetArraySize(array_obj);
        if(array_size!=MAX_ARM_Row*MAX_ARM_Col)
            return;
        for(int i=0; i<array_size; i++)
        {
            cJSON *item=cJSON_GetArrayItem(array_obj, i);
            if (cJSON_IsNumber(item))
            {
                iPlateR[i/MAX_ARM_Col][i%MAX_ARM_Col]=item->valueint;
            }
        }
    }
}
//---------------------------------------------------------------------------
void uHPSuckTeam::SavePlateSite(cJSON* root)
{
    if(root==NULL)
        return;
    AnsiString asVarName="";
    int iArr[MAX_ARM_Row*MAX_ARM_Col];
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            iArr[i*MAX_ARM_Col+j]=iSite[i][j];
        }
    }
    cJSON *jarr=cJSON_CreateIntArray(&iArr[0], MAX_ARM_Row*MAX_ARM_Col);
    cJSON_AddItemToObject(root, "Site", jarr);
}
//---------------------------------------------------------------------------
void uHPSuckTeam::ReadPlateSiteFromJSON(cJSON* root)
{
    cJSON *array_obj=cJSON_GetObjectItem(root, "Site");
    if(cJSON_IsArray(array_obj))
    {
        int array_size=cJSON_GetArraySize(array_obj);
        if(array_size!=MAX_ARM_Row*MAX_ARM_Col)
            return;
        for(int i=0; i<array_size; i++)
        {
            cJSON *item=cJSON_GetArrayItem(array_obj, i);
            if (cJSON_IsNumber(item))
            {
                iSite[i/MAX_ARM_Col][i%MAX_ARM_Col]=item->valueint;
            }
        }
    }
}
//---------------------------------------------------------------------------
void uHPSuckTeam::SavePlateSuck(cJSON* root)
{
    if(root==NULL)
        return;
    AnsiString asVarName="";
    int iArr[MAX_ARM_Row*MAX_ARM_Col];
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            iArr[i*MAX_ARM_Col+j]=(bSuck[i][j])?1:0;
        }
    }
    cJSON *jarr=cJSON_CreateIntArray(&iArr[0], MAX_ARM_Row*MAX_ARM_Col);
    cJSON_AddItemToObject(root, "Suck", jarr);
}
//---------------------------------------------------------------------------
void uHPSuckTeam::ReadPlateSuckFromJSON(cJSON* root)
{
    cJSON *array_obj=cJSON_GetObjectItem(root, "Suck");
    if(cJSON_IsArray(array_obj))
    {
        int array_size=cJSON_GetArraySize(array_obj);
        if(array_size!=MAX_ARM_Row*MAX_ARM_Col)
            return;
        for(int i=0; i<array_size; i++)
        {
            cJSON *item=cJSON_GetArrayItem(array_obj, i);
            if (cJSON_IsNumber(item))
            {
                bSuck[i/MAX_ARM_Col][i%MAX_ARM_Col]=(item->valueint==1)?true:false;
            }
        }
    }
}
//---------------------------------------------------------------------------
uHPSuckGroup::uHPSuckGroup()
{
    HPSuckTeamList=new TList;
    HPSuckTeamList->Clear();
}
//---------------------------------------------------------------------------
uHPSuckGroup::~uHPSuckGroup()
{
    try
    {
        HPSuckTeamList->Clear();
        delete HPSuckTeamList;
        HPSuckTeamList=NULL;
    }
    catch(...)
    {
    }
}
//---------------------------------------------------------------------------
void uHPSuckGroup::ClearTeamList()
{
    HPSuckTeamList->Clear();
}
//---------------------------------------------------------------------------
void uHPSuckGroup::AddHPSuckTeam(uHPSuckTeam* HPTeam)
{
    HPSuckTeamList->Add(HPTeam);
}
//---------------------------------------------------------------------------
void uHPSuckGroup::AddHPSuckTeam(int iP, int iR, int iC, int iSht, int iKit)
{
    uHPSuckTeam* HPTeam=new uHPSuckTeam(iP, iR, iC, iSht, iKit);
    HPSuckTeamList->Add(HPTeam);
}
//---------------------------------------------------------------------------
void uHPSuckGroup::DelHPSuckTeam(int iIndex)
{
    if(HPSuckTeamList->Count==0 || iIndex>=HPSuckTeamList->Count)
    {
        return;
    }
    HPSuckTeamList->Delete(iIndex);
}
//---------------------------------------------------------------------------
uHPSuckTeam* uHPSuckGroup::ExtractLastTeam()
{
    return ExtractSuckTeam(HPSuckTeamList->Count-1);
}
//---------------------------------------------------------------------------
uHPSuckTeam* uHPSuckGroup::ExtractSuckTeam(int iIndex)
{
    if(HPSuckTeamList->Count==0 || iIndex<0)
    {
        return NULL;
    }
    uHPSuckTeam* HPTeam=(uHPSuckTeam*)HPSuckTeamList->Items[iIndex];
    return HPTeam;
}
//---------------------------------------------------------------------------
int uHPSuckGroup::GetTeamCount()                                                //分幾次吸放料
{
    if(this==NULL)
    {
        return 0;
    }
    else
    {
        return HPSuckTeamList->Count;
    }
}
//---------------------------------------------------------------------------
void uHPSuckGroup::DataForward()
{
    DataForwardAndDelete(HPSuckTeamList);
}
//---------------------------------------------------------------------------
uHPSuckGroup* uHPSuckGroup::Clone()
{
    uHPSuckGroup* clonedGroup=new uHPSuckGroup();
    for(int i=0; i<GetTeamCount(); i++)
    {
        clonedGroup->HPSuckTeamList->Add(ExtractSuckTeam(i)->Clone());
    }
    return clonedGroup;
}
//---------------------------------------------------------------------------
void uHPSuckGroup::SaveJSONFile(cJSON* root)
{
    if(root==NULL)
        return;
    cJSON* rArr=cJSON_CreateArray();
    for(int i=0; i<HPSuckTeamList->Count; i++)
    {
        cJSON* jsonObj=cJSON_CreateObject();
        uHPSuckTeam* HPTeam=(uHPSuckTeam*)HPSuckTeamList->Items[i];
        HPTeam->SaveJSONFile(jsonObj);
        cJSON_AddItemToArray(rArr, jsonObj);
    }
    cJSON_AddItemToObject(root, "HPSuckTeamList", rArr);
}
//---------------------------------------------------------------------------
void uHPSuckGroup::LoadJSONFile(cJSON* root)
{
    if(root==NULL)
        return;
    cJSON *array_obj=cJSON_GetObjectItem(root, "HPSuckTeamList");
    if(cJSON_IsArray(array_obj))
    {
        int array_size=cJSON_GetArraySize(array_obj);
        for(int i=0; i<array_size; i++)
        {
            cJSON *item=cJSON_GetArrayItem(array_obj, i);
            uHPSuckTeam* HPTeam=new uHPSuckTeam();
            HPTeam->LoadJSONFile(item);
            HPSuckTeamList->Add(HPTeam);
        }
    }
}
//---------------------------------------------------------------------------
uPlateInfo::uPlateInfo()
{
    HPSuckGroupList =new TList;
    iLimitcount     =0;
    TempSuckTeam    =NULL;
    HPSuckGroupList->Clear();
}
//---------------------------------------------------------------------------
uPlateInfo::~uPlateInfo()
{
    try
    {
        HPSuckGroupList->Clear();
        delete HPSuckGroupList;
        HPSuckGroupList=NULL;
    }
    catch(...)
    {
    }
}
//---------------------------------------------------------------------------
int  uPlateInfo::GetHPSuckGroupCount()
{
    return HPSuckGroupList->Count;
}
//---------------------------------------------------------------------------
void uPlateInfo::SetTempSuckTeam(uHPSuckTeam* HPTeam)
{
    TempSuckTeam=HPTeam;
}
//---------------------------------------------------------------------------
uHPSuckTeam* uPlateInfo::GetTempSuckTeam()
{
    return TempSuckTeam;
}
//---------------------------------------------------------------------------
void uPlateInfo::AddHPSuckGroup()
{
    uHPSuckGroup* HPGroup=new uHPSuckGroup();
    AddHPSuckGroup(HPGroup);
}
//---------------------------------------------------------------------------
void uPlateInfo::AddHPSuckGroup(uHPSuckGroup* HPGroup)
{
    uHPSuckGroup* HPGroupTmp=ExtractLastGroup();                                //RogerYang 20250814 : 如果HPSuckGroupList裡面的上一個Group->HPSuckTeamList內容為空，就不要再新增Group
    if(HPGroupTmp!=NULL)
    {
        uHPSuckTeam* HPTeam=HPGroupTmp->ExtractLastTeam();
        if(HPTeam==NULL)
            return;
    }
    HPSuckGroupList->Add(HPGroup);
}
//---------------------------------------------------------------------------
void uPlateInfo::DelHPSuckGroup(int iIndex)
{
    if(HPSuckGroupList->Count==0 || iIndex>=HPSuckGroupList->Count)
    {
        return;
    }
    HPSuckGroupList->Delete(iIndex);
}
//---------------------------------------------------------------------------
void uPlateInfo::ClearGroupList()
{
    for(int i=0; i<HPSuckGroupList->Count; i++)
    {
        uHPSuckGroup* HPGroup=ExtractSuckGroup(i);
        if(HPGroup==NULL)
            continue;

        HPGroup->ClearTeamList();
    }
    HPSuckGroupList->Clear();
}
//---------------------------------------------------------------------------
uHPSuckGroup* uPlateInfo::ExtractLastGroup()
{
    return ExtractSuckGroup(HPSuckGroupList->Count-1);
}
//---------------------------------------------------------------------------
uHPSuckGroup* uPlateInfo::ExtractSuckGroup(int iIndex)
{
    if(HPSuckGroupList->Count==0 || iIndex<0)
    {
        return NULL;
    }
    uHPSuckGroup* info=(uHPSuckGroup*)HPSuckGroupList->Items[iIndex];
    return info;
}
//---------------------------------------------------------------------------
uHPSuckTeam* uPlateInfo::ExtractFirstTeam()
{
    return ExtractTeamByIndex(0, 0);
}
//---------------------------------------------------------------------------
uHPSuckTeam* uPlateInfo::ExtractLastTeam()
{
    return ExtractTeamByIndex(GetHPSuckGroupCount()-1, 0);
}
//---------------------------------------------------------------------------
uHPSuckTeam* uPlateInfo::ExtractLastTeamHasData()
{
    uHPSuckTeam *HPTeam=ExtractTeamByIndex(GetHPSuckGroupCount()-1, 0);
    if(HPTeam==NULL)
    {
        HPTeam=ExtractTeamByIndex(GetHPSuckGroupCount()-2, 0);
    }

    if(HPTeam==NULL)
    {
        return NULL;
    }
    else
    {
        return HPTeam;
    }
}
//---------------------------------------------------------------------------
uHPSuckTeam* uPlateInfo::ExtractTeamByIndex(int iGroupIndex, int iTeamIndex)
{
    uHPSuckGroup *HPGroup=ExtractSuckGroup(iGroupIndex);
    if(HPGroup==NULL)
    {
        return NULL;
    }
    else
    {
        uHPSuckTeam *HPTeam=HPGroup->ExtractSuckTeam(iTeamIndex);
        if(HPTeam==NULL)
        {
            return NULL;
        }
        else
        {
            return HPTeam;
        }
    }
}
//---------------------------------------------------------------------------
void uPlateInfo::SetArrPlateXY(int iSuckRow, int iSuckCol, int _iP, int _iRow, int _iCol, int _Site)
{
    uHPSuckGroup* HPGroup=ExtractLastGroup();
    if(HPGroup!=NULL)
    {
        uHPSuckTeam* HPTeam=HPGroup->ExtractLastTeam();
        if(HPTeam!=NULL)
        {
            HPTeam->iP                           =_iP;
//            HPTeam->dTime                        =Now();
            HPTeam->iPlateC[iSuckRow][iSuckCol]  =_iCol;
            HPTeam->iPlateR[iSuckRow][iSuckCol]  =_iRow;
            HPTeam->iSite[iSuckRow][iSuckCol]    =_Site;
            HPTeam->bSuck[iSuckRow][iSuckCol]    =true;
        }
    }
}
//---------------------------------------------------------------------------
void uPlateInfo::UpdateHPSuckGroup(int iP, int iR, int iC, int iSht, int iKit)
{
    if(GetHPSuckGroupCount()<=0)
        AddHPSuckGroup();
    uHPSuckGroup *HPGroup=ExtractLastGroup();
    HPGroup->AddHPSuckTeam(iP, iR, iC, iSht, iKit);
}
//---------------------------------------------------------------------------
uHPSuckTeam* uPlateInfo::SearchHPSuckTeamByCoordinate(int &iSuckRow, int &iSuckCol, int _iP, int _iRow, int _iCol)
{
    for(int i=0; i<HPSuckGroupList->Count; i++)
    {
        uHPSuckGroup* HPGroup=ExtractSuckGroup(i);
        if(HPGroup==NULL)
            continue;
        for(int j=0; j<HPGroup->GetTeamCount(); j++)
        {
            uHPSuckTeam* HPTeam=HPGroup->ExtractSuckTeam(j);
            if(HPTeam==NULL)
            {
                continue;
            }
            else
            {
                if(HPTeam->iP==_iP)
                {
                    for(int i=0; i<MAX_ARM_Row; i++)
                    {
                        for(int j=0; j<MAX_ARM_Col; j++)
                        {
                            if(HPTeam->iPlateC[i][j]==_iCol &&
                               HPTeam->iPlateR[i][j]==_iRow)
                            {
                                iSuckRow=i;
                                iSuckCol=j;
                                return HPTeam;
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------
bool uPlateInfo::SetPlateSuck(int _iP, int _iRow, int _iCol, bool bSuck)
{
    if(HPSuckGroupList->Count==0)
    {
        return false;
    }
    int iSuckRow=0, iSuckCol=0;
    uHPSuckTeam *HPTeam=SearchHPSuckTeamByCoordinate(iSuckRow, iSuckCol, _iP, _iRow, _iCol);
    if(HPTeam!=NULL)
    {
        HPTeam->bSuck[iSuckRow][iSuckCol]=bSuck;
    }
    return true;
}

//---------------------------------------------------------------------------
void uPlateInfo::SaveFile(AnsiString sFileName)
{
    AnsiString asGroupName="";
    cJSON* root=cJSON_CreateObject();
    for(int i=0; i<HPSuckGroupList->Count; i++)
    {
        cJSON* r_single=cJSON_CreateObject();
        uHPSuckGroup* HPGroup=(uHPSuckGroup*)HPSuckGroupList->Items[i];
        HPGroup->SaveJSONFile(r_single);
        asGroupName=AnsiString().sprintf("HPSuckGroupList_%d", i);
        cJSON_AddItemToObject(root, asGroupName.c_str(), r_single);
    }
    WriteDataToFile(sFileName.c_str(), cJSON_Print(root), true);
    cJSON_Delete(root);
}
//---------------------------------------------------------------------------
void uPlateInfo::LoadFile(AnsiString sFileName)
{
    if(HPSuckGroupList->Count>0)
    {
        HPSuckGroupList->Clear();
    }
    char* file_buf=ReadDataFromFile(sFileName);
    cJSON* loaded_root=cJSON_Parse(file_buf);
    if(!loaded_root)
    {
        return;
    }
    int iGroupNum=cJSON_GetArraySize(loaded_root);
    for(int i=0; i<iGroupNum; i++)
    {
        cJSON* r_single=cJSON_GetArrayItem(loaded_root, i);
        cJSON *array_obj=cJSON_GetObjectItem(r_single, "HPSuckTeamList");
        if(cJSON_IsArray(array_obj) &&
           cJSON_GetArraySize(array_obj)==0 &&
           i!=iGroupNum-1)                                                      //RogerYang 20250815 避免刪除掉最後一筆剛好Group內容是空的
        {
            //NULL
        }
        else
        {
            uHPSuckGroup* HPGroup=new uHPSuckGroup();
            HPGroup->LoadJSONFile(r_single);
            AddHPSuckGroup(HPGroup);
        }
    }
}
//---------------------------------------------------------------------------
void uPlateInfo::ResetFile(AnsiString sFileName)
{
    ClearGroupList();                                                           //Steven 20250519 : 避免有殘留的資料
    if(FileInfo().IsFilePathExist(sFileName))
    {
        DeleteFile(sFileName);
    }
    SaveFile(sFileName);
}
//---------------------------------------------------------------------------
bool uPlateInfo::DataForwardAndNextTeam()
{
    if(GetHPSuckGroupCount()==0)
        return false;
    uHPSuckGroup *HPGroup=ExtractSuckGroup(0);
    if(HPGroup->GetTeamCount()<=0)
    {
        return false;
    }
    else if(HPGroup->GetTeamCount()>1)
    {
        HPGroup->DataForward();
        return true;
    }
    else
    {
        HPGroup->DataForward();
        DataForwardAndDelete(HPSuckGroupList);
        return false;
    }
}
//---------------------------------------------------------------------------
bool uPlateInfo::GetHPFirstTeamSuckUse(bool bSuck[MAX_ARM_Row][MAX_ARM_Col])
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            bSuck[i][j]=false;
        }
    }
    uHPSuckTeam *HPTeam=ExtractFirstTeam();
    if(HPTeam==NULL)
    {
        return false;
    }
    else
    {
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                bSuck[i][j]=HPTeam->bSuck[i][j];
            }
        }
        return true;
    }
}
//---------------------------------------------------------------------------
bool uPlateInfo::GetHPFirstTeamMotUse(bool bSuck[MAX_ARM_Row][MAX_ARM_Col])
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            bSuck[i][j]=false;
        }
    }
    uHPSuckTeam *HPTeam=ExtractFirstTeam();
    if(HPTeam==NULL)
    {
        return false;
    }
    else
    {
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                if(HPTeam->iSite[i][j]>0)                                       //JerryYang 202050813 : fix錯誤吸嘴下降
                {
                    bSuck[i][j]=true;                                           //RogerYang 20250613 這裡是決定基準軸的位置，只看HP的suck資料可能會導致基準軸計算錯誤
                }
                else
                {
                    bSuck[i][j]=false;
                }
            }
        }
        return true;
    }
}
//---------------------------------------------------------------------------
int uPlateInfo::GetHPFirstTeamSuckCol()
{
    uHPSuckTeam *HPTeam=ExtractFirstTeam();
    if(HPTeam==NULL)
    {
        return 0;
    }
    else
    {
        for(int i=0; i<MAX_ARM_Row; i++)
        {
            for(int j=0; j<MAX_ARM_Col; j++)
            {
                if(HPTeam->bSuck[i][j]==true)
                {
                    return j;
                }
            }
        }
        return 0;
    }
}
//---------------------------------------------------------------------------
bool uPlateInfo::GetHPFirstTeamPlate(int &iP, int &iR, int &iC, int &iSht, int &iKit)
{
    uHPSuckGroup* HPGroup=ExtractSuckGroup(0);
    int iStep=0;
    if(HPGroup==NULL)
    {
        return false;
    }
    else
    {
        iStep=HPGroup->GetTeamCount();
    }
    while(iStep==0 &&                                                           //沒有Step資料的, 表示是空集合
          HPGroup!=NULL &&
          iLimitcount<500)                                                      //JerryYang 20250410 : Protect for while loop
    {
        HPGroup->DataForward();
        HPGroup=ExtractSuckGroup(0);
        iStep=HPGroup->GetTeamCount();
        iLimitcount++;
    };

    if(iLimitcount>=500)
    {
        return false;
    }
    iLimitcount=0;
    uHPSuckTeam *HPTeam=ExtractFirstTeam();
    if(HPTeam==NULL)
    {
        return false;
    }
    else
    {
        iP  =HPTeam->iP;
        iR  =HPTeam->iR;
        iC  =HPTeam->iC;
        iSht=HPTeam->iSht;
        iKit=HPTeam->iKit;
        return true;
    }
}
//---------------------------------------------------------------------------
bool uPlateInfo::GetHPFirstTeam(int *iP, int (*PlateR)[MAX_ARM_Col], int (*PlateC)[MAX_ARM_Col], bool (*Suck)[MAX_ARM_Col])
{
    return GetHPTeamSuck(0, 0, iP, PlateR, PlateC, Suck);
}
//---------------------------------------------------------------------------
bool uPlateInfo::GetHPTeamSuck(int iGroupIndex, int iTeamIndex, int *iP, int (*PlateR)[MAX_ARM_Col], int (*PlateC)[MAX_ARM_Col], bool (*Suck)[MAX_ARM_Col])
{
    uHPSuckGroup *HPGroup=ExtractSuckGroup(iGroupIndex);
    if(HPGroup==NULL)
    {
        return false;
    }
    else
    {
        uHPSuckTeam *HPTeam=HPGroup->ExtractSuckTeam(iTeamIndex);
        if(HPTeam==NULL)
        {
            return false;
        }
        else
        {
            *iP=HPTeam->iP;
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    PlateR[i][j]=HPTeam->iPlateR[i][j];
                    PlateC[i][j]=HPTeam->iPlateC[i][j];
//                    if(HPTeam->iSite[i][j]==-1 && HPTeam->bSuck[i][j]==false)
//                    {
//                        Suck[i][j]=false;
//                    }
//                    else
//                    {
//                        Suck[i][j]=true;                                           //RogerYang 20250613 這裡是決定基準軸的位置，只看HP的suck資料可能會導致基準軸計算錯誤
//                    }
                    Suck[i][j]  =HPTeam->bSuck[i][j];                           //RogerYang 20250819 改回來
                }
            }
            return true;
        }
    }
}
//---------------------------------------------------------------------------
bool uPlateInfo::GetHPFirstTeamToList(int *iP, TList* lsPoint2D)
{
    uHPSuckGroup *HPGroup=ExtractSuckGroup(0);
    if(HPGroup==NULL)
    {
        return false;
    }
    else
    {
        if(lsPoint2D==NULL)
        {
            return false;
        }
        for(int i=0; i<HPGroup->GetTeamCount(); i++)
        {
            uHPSuckTeam *HPTeam=HPGroup->ExtractSuckTeam(i);
            *iP=HPTeam->iP;
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    if(HPTeam->bSuck[i][j]==true)
                    {
                        lsPoint2D->Add(new uPoint2D(HPTeam->iPlateC[i][j], HPTeam->iPlateR[i][j]));
                    }
                }
            }
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void uPlateInfo::InsertHPGroup(int Index, void *Item)
{
    HPSuckGroupList->Insert(Index, Item);
}
//---------------------------------------------------------------------------
void uPlateInfo::TransferByuPlacePoint2DList(TList *Listpp2D)
{
    uPlacePoint2D* pp2D;
    for(int i=0; i<Listpp2D->Count; i++)
    {
        pp2D=(uPlacePoint2D*)Listpp2D->Items[i];
        AddDataByPlacePoint2D(pp2D);
    }
}
//---------------------------------------------------------------------------
void uPlateInfo::AddDataByPlacePoint2D(uPlacePoint2D* pp2D)
{
    int iPlateStep=pp2D->iHPStep;
    while(iPlateStep>=GetHPSuckGroupCount())
    {
        AddHPSuckGroup();
    }
    uHPSuckGroup* HPGroup=ExtractSuckGroup(iPlateStep);
    int iLoaderStep=pp2D->iSubStep;
    while(iLoaderStep>=HPGroup->GetTeamCount())
    {
        HPGroup->AddHPSuckTeam(-1, -1, -1, -1, -1);
    }
    uHPSuckTeam* HPTeam=HPGroup->ExtractSuckTeam(iLoaderStep);
    HPTeam->iP=pp2D->iPlate;
    if(HPTeam->iR>=0)
    {
        if(HPTeam->iR>pp2D->iPlateRow)
        {
            HPTeam->iR=pp2D->iPlateRow;
        }
    }
    else
    {
        HPTeam->iR=pp2D->iPlateRow;
    }
    if(HPTeam->iC>=0)
    {
        if(HPTeam->iC>pp2D->iPlateCol)
        {
            HPTeam->iC=pp2D->iPlateCol;
        }
    }
    else
    {
        HPTeam->iC=pp2D->iPlateCol;
    }
    HPTeam->iPlateR[pp2D->iSuckRow][pp2D->iSuckCol] =pp2D->iPlateRow;
    HPTeam->iPlateC[pp2D->iSuckRow][pp2D->iSuckCol] =pp2D->iPlateCol;
    HPTeam->bSuck[pp2D->iSuckRow][pp2D->iSuckCol]   =true;
}
//---------------------------------------------------------------------------
uPoint2D::uPoint2D()
{
    Clear();
}
//---------------------------------------------------------------------------
uPoint2D::uPoint2D(int x, int y)
{
    Clear();
    X=x;
    Y=y;
}
//---------------------------------------------------------------------------
void uPoint2D::Clear()
{
    X=0;
    Y=0;
}
//---------------------------------------------------------------------------
