#include "MachineDefine.h"
#pragma hdrstop

#include "MyTempPanel.h"
#include "uTemp_Set.h"
#include "BarcodeReader.h"
#include "myQwertyKeyBoard.h"
#include "ATC_Handler_Side.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TList *listNormal;
TList *listArm1;
TList *listArm2;
//TList *listDut;
//TList *listNoUse;
//TList *listHeater;
//---------------------------------------------------------------------------
__fastcall TMyTempPanel::TMyTempPanel(AnsiString Alias, int iTag)
{
    AnsiString Str;
    palTemp         =new TPanel(fTemp_Set->tsUnUse);

    labName         =new TLabel(palTemp);                                       //name

    edLow           =new TEdit(palTemp);
    edMid           =new TEdit(palTemp);
    edLowbase       =new TEdit(palTemp);
    edBase          =new TEdit(palTemp);
    edHighBase      =new TEdit(palTemp);
    edSHighBase     =new TEdit(palTemp);                                        //Ztex 2024.07.27 Add 6 Point Temperature Offset

    edKit_Low       =new TEdit(palTemp);                                        //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Mid       =new TEdit(palTemp);
    edKit_Lowbase   =new TEdit(palTemp);
    edKit_Base      =new TEdit(palTemp);
    edKit_HighBase  =new TEdit(palTemp);

    edOffset        =new TEdit(palTemp);

    edSingleLimit   =new TEdit(palTemp);
    edIndiTemp      =new TEdit(palTemp);
    edInitTempOffset=new TEdit(palTemp);
    edEOTTempOffset =new TEdit(palTemp);
    palLine         =new TPanel(palTemp);

    iIndexTag=iTag;

    palTemp->Left      =0;
    palTemp->Top       =0;
    palTemp->Width     =793;
    palTemp->Height    =27;
    palTemp->DragKind  =dkDock;
    palTemp->Color     =(TColor)0x00C2B8A6;
    palTemp->BevelInner=bvNone;
    palTemp->BevelOuter=bvNone;
    palTemp->Visible   =true;

    labName->Parent=palTemp;
    labName->AutoSize=false;
    labName->Left = 5;
    labName->Top = 2;
    labName->Width = 80;
    labName->Height = 16;
    labName->Caption=Alias;
    labName->Font->Charset = DEFAULT_CHARSET;
    labName->Font->Color = clNavy;
    labName->Font->Size = 10;
    labName->Font->Name = "MS Sans Serif";
    labName->ParentFont = false;

    edLow->Parent=palTemp;
    Str.sprintf("edLow%03d", iTag);
    edLow->Name=Str;
    edLow->AutoSize=false;
    edLow->Left = 90;
    edLow->Top = 2;
    edLow->Width = 50;                                                          //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edLow->Height = 21;
    edLow->ImeMode= imClose;
    edLow->ParentFont = false;
    edLow->Enabled =true;
    edLow->OnMouseDown=edBaseMouseDown;

    edKit_Low->Parent=palTemp;                                                  //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Low->AutoSize=false;
    edKit_Low->Left = 90;
    edKit_Low->Top = 2;
    edKit_Low->Width = 60;
    edKit_Low->Height = 21;
    edKit_Low->ImeMode= imClose;
    edKit_Low->ParentFont = false;
    edKit_Low->Enabled =true;
    edKit_Low->OnMouseDown=edBaseMouseDown;
    edKit_Low->Color=clMedGray;

    edMid->Parent=palTemp;
    Str.sprintf("edMid%03d", iTag);
    edMid->Name=Str;
    edMid->AutoSize=false;
    edMid->Left = 141;                                                          //151->141 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edMid->Top = 2;
    edMid->Width = 50;                                                          //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edMid->Height = 21;
    edMid->ImeMode= imClose;
    edMid->ParentFont = false;
    edMid->Enabled =true;
    edMid->OnMouseDown=edBaseMouseDown;

    edKit_Mid->Parent=palTemp;                                                  //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Mid->AutoSize=false;
    edKit_Mid->Left = 151;
    edKit_Mid->Top = 2;
    edKit_Mid->Width = 60;
    edKit_Mid->Height = 21;
    edKit_Mid->ImeMode= imClose;
    edKit_Mid->ParentFont = false;
    edKit_Mid->Enabled =true;
    edKit_Mid->OnMouseDown=edBaseMouseDown;
    edKit_Mid->Color=clMedGray;

    edLowbase->Parent=palTemp;
    Str.sprintf("edLowbase%03d", iTag);
    edLowbase->Name=Str;
    edLowbase->AutoSize=false;
    edLowbase->Left = 192;                                                      //192->212 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edLowbase->Top = 2;
    edLowbase->Width = 50;                                                      //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edLowbase->Height = 21;
    edLowbase->ImeMode= imClose;
    edLowbase->ParentFont = false;
    edLowbase->Enabled =true;
    edLowbase->OnMouseDown=edBaseMouseDown;

    edKit_Lowbase->Parent=palTemp;                                              //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Lowbase->AutoSize=false;
    edKit_Lowbase->Left = 212;
    edKit_Lowbase->Top = 2;
    edKit_Lowbase->Width = 60;
    edKit_Lowbase->Height = 21;
    edKit_Lowbase->ImeMode= imClose;
    edKit_Lowbase->ParentFont = false;
    edKit_Lowbase->Enabled =true;
    edKit_Lowbase->OnMouseDown=edBaseMouseDown;
    edKit_Lowbase->Color=clMedGray;

    edBase->Parent=palTemp;
    Str.sprintf("edBase%03d", iTag);
    edBase->Name=Str;
    edBase->AutoSize=false;
    edBase->Left = 243;                                                         //192->273 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edBase->Top = 2;
    edBase->Width = 50;                                                         //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edBase->Height = 21;
    edBase->ImeMode= imClose;
    edBase->ParentFont = false;
    edBase->Enabled =true;
    edBase->OnMouseDown=edBaseMouseDown;

    edKit_Base->Parent=palTemp;                                                 //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Base->AutoSize=false;
    edKit_Base->Left = 273;
    edKit_Base->Top = 2;
    edKit_Base->Width = 60;
    edKit_Base->Height = 21;
    edKit_Base->ImeMode= imClose;
    edKit_Base->ParentFont = false;
    edKit_Base->Enabled =true;
    edKit_Base->OnMouseDown=edBaseMouseDown;
    edKit_Base->Color=clMedGray;

    edHighBase->Parent=palTemp;
    Str.sprintf("edHighBase%03d", iTag);
    edHighBase->Name=Str;
    edHighBase->AutoSize=false;
    edHighBase->Left = 294;                                                     //294->334 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edHighBase->Top = 2;
    edHighBase->Width = 50;                                                     //60->50 //Ztex 2024.07.27 Add 6 Point Temperature Offset
    edHighBase->Height = 21;
    edHighBase->ImeMode= imClose;
    edHighBase->ParentFont = false;
    edHighBase->Enabled =true;
    edHighBase->OnMouseDown=edBaseMouseDown;

    edKit_HighBase->Parent=palTemp;                                             //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_HighBase->AutoSize=false;
    edKit_HighBase->Left = 334;
    edKit_HighBase->Top = 2;
    edKit_HighBase->Width = 60;
    edKit_HighBase->Height = 21;
    edKit_HighBase->ImeMode= imClose;
    edKit_HighBase->ParentFont = false;
    edKit_HighBase->Enabled =true;
    edKit_HighBase->OnMouseDown=edBaseMouseDown;
    edKit_HighBase->Color=clMedGray;

    edSHighBase->Parent=palTemp;                                                //Ztex 2024.07.27 Add 6 Point Temperature Offset ==>
    Str.sprintf("edSHighBase%03d", iTag);
    edSHighBase->Name=Str;
    edSHighBase->AutoSize=false;
    edSHighBase->Left = 345;
    edSHighBase->Top = 2;
    edSHighBase->Width = 50;
    edSHighBase->Height = 21;
    edSHighBase->ImeMode= imClose;
    edSHighBase->ParentFont = false;
    edSHighBase->Enabled =true;
    edSHighBase->OnMouseDown=edBaseMouseDown;                                   //Ztex 2024.07.27 Add 6 Point Temperature Offset <==

    edOffset->Parent=palTemp;
    Str.sprintf("edOffset%03d", iTag);
    edOffset->Name=Str;
    edOffset->AutoSize=false;
    edOffset->Left = 415;
    edOffset->Top = 2;
    edOffset->Width = 60;
    edOffset->Height = 21;
    edOffset->BorderStyle=bsNone;
    edOffset->Text = "0";
    edOffset->Color = clBtnFace;
    edOffset->ParentFont = false;
    edOffset->Enabled =true;
    edOffset->OnMouseDown=edBaseMouseDown;

    edSingleLimit->Parent=palTemp;
    Str.sprintf("edSingleLimit%03d", iTag);
    edSingleLimit->Name=Str;
    edSingleLimit->AutoSize=false;
    edSingleLimit->Left = 494;
    edSingleLimit->Top = 2;
    edSingleLimit->Width = 60;
    edSingleLimit->Height = 21;
    edSingleLimit->ParentFont = false;
    edSingleLimit->Enabled =true;
    edSingleLimit->OnMouseDown=edLimitMouseDown;

    edIndiTemp->Parent=palTemp;
    Str.sprintf("edIndiTemp%03d", iTag);
    edIndiTemp->Name=Str;
    edIndiTemp->AutoSize=false;
    edIndiTemp->Left = 573;
    edIndiTemp->Top = 2;
    edIndiTemp->Width = 60;
    edIndiTemp->Height = 21;
    edIndiTemp->ParentFont = false;
    edIndiTemp->Enabled =true;
    edIndiTemp->OnMouseDown=edIndiviMouseDown;

    edInitTempOffset->Parent=palTemp;
    Str.sprintf("edInitTempOffset%03d", iTag);
    edInitTempOffset->Name=Str;
    edInitTempOffset->AutoSize=false;
    edInitTempOffset->Left = 652;
    edInitTempOffset->Top = 2;
    edInitTempOffset->Width = 60;
    edInitTempOffset->Height = 21;
    edInitTempOffset->ParentFont = false;
    edInitTempOffset->Enabled =true;
    edInitTempOffset->OnMouseDown=edinitialMouseDown;                           //kevin 20210421 edBaseMouseDown; ->edinitialMouseDown 獨立 範圍
    //edInitTempOffset->OnMouseDown=edBaseMouseDown;

    edEOTTempOffset->Parent=palTemp;
    Str.sprintf("edEOTTempOffset%03d", iTag);
    edEOTTempOffset->Name=Str;
    edEOTTempOffset->AutoSize=false;
    edEOTTempOffset->Left = 731;
    edEOTTempOffset->Top = 2;
    edEOTTempOffset->Width = 60;
    edEOTTempOffset->Height = 21;
    edEOTTempOffset->ParentFont = false;
    edEOTTempOffset->Enabled =true;
    edEOTTempOffset->OnMouseDown=edBaseMouseDown;

    palLine->Caption="";
    palLine->Parent=palTemp;
    palLine->Height=1;
    palLine->BevelInner=bvNone;
    palLine->BevelOuter=bvNone;
    palLine->Align=alBottom;

    edLow->Tag           =iTag;
    edMid->Tag           =iTag;
    edLowbase->Tag       =iTag;
    edBase->Tag          =iTag;
    edHighBase->Tag      =iTag;

    edKit_Low->Tag       =iTag;                                                 //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Mid->Tag       =iTag;
    edKit_Lowbase->Tag   =iTag;
    edKit_Base->Tag      =iTag;
    edKit_HighBase->Tag  =iTag;

    edOffset->Tag        =iTag;
    edSingleLimit->Tag   =iTag;
    edIndiTemp->Tag      =iTag;
    edInitTempOffset->Tag=iTag;
    edEOTTempOffset->Tag =iTag;
    iOffsetByRecipeMaxLimit = 60;
    iOffsetByRecipeMinLimit = -60;

}
//---------------------------------------------------------------------------
__fastcall TMyTempPanel::~TMyTempPanel()
{
    try
    {
        delete labName;
        delete edLow;
        delete edMid;
        delete edLowbase;
        delete edBase;
        delete edHighBase;

        delete edKit_Low;                                                       //JerryYang 20221003 Analog要求新增Kit三點offset
        delete edKit_Mid;
        delete edKit_Lowbase;
        delete edKit_Base;
        delete edKit_HighBase;

        delete edOffset;
        delete edSingleLimit;
        delete edIndiTemp;
        delete edInitTempOffset;
        delete edEOTTempOffset;
        delete palLine;
        palTemp->Parent=NULL;
        delete palTemp;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyTempPanel");
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::SetParent(TTabSheet *tsTemp)                      //目前設定值
{
    palTemp->Parent=tsTemp;
    palTemp->Align=alTop;
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::SetEnable(bool bflag)
{
    edLow->Enabled=bflag;
    edMid->Enabled=bflag;
    edLowbase->Enabled=bflag;
    edBase->Enabled=bflag;
    edHighBase->Enabled=bflag;
    edSHighBase->Enabled=bflag;                                                 //Ztex 2024.07.27 Add 6 Point Temperature Offset

    edKit_Low->Enabled=bflag;                                                   //JerryYang 20221003 Analog要求新增Kit三點offset
    edKit_Mid->Enabled=bflag;
    edKit_Lowbase->Enabled=bflag;
    edKit_Base->Enabled=bflag;
    edKit_HighBase->Enabled=bflag;

    edOffset->Enabled=bflag;
    edSingleLimit->Enabled=bflag;
    edIndiTemp->Enabled=bflag;
    edInitTempOffset->Enabled=bflag;
    edEOTTempOffset->Enabled=bflag;
    labName->Enabled=bflag;
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::edBaseMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TEdit *Buffer;
    Buffer=(TEdit *)Sender;
    dTempMax=fTemp_Set->MaxTempSetting();                                       //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    dTempMin=fTemp_Set->MinTempSetting();                                       //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    double fTemp = 0;                                                           //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    if(Buffer->Tag==1 && Tri_Temp_Machine!=1)                                   //20140320 wei //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        if(Barcode_Reader(bcTemperature)==0)                                    // 20140103 wei KYEC Barcode Reader
        {
            return;
        }
    }

    if(INSTALL_HEAT_GUN && (Buffer->Tag==tcHeatGun1 || Buffer->Tag==tcHeatGun2)) //kevin 20200101  hot gun
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iHeaterGunH , (double)InputLimit.iHeaterGunL);
    }
    else
    {
        if(Tri_Temp_Machine==1)                                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            if((Buffer->Tag>=tcDUT1 && Buffer->Tag<=tcDUT4) ||
               (Buffer->Tag>=tcOutSht1 && Buffer->Tag<=tcBase6) ||
               (Buffer->Tag>=tcDoor1 && Buffer->Tag<=tcDoor2) )                 //Ztex 2023.10.23 Add Index Door Heater
            {
                if(Buffer->Tag>=tcDUT1 && Buffer->Tag<=tcDUT4)
                {
                    if(SetHeaterTemp_MaxIndex - Temperature.iTriTempDefault_Ini[1] > 30)
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 30.00, 0.00);
                    }
                    else
                    {
                        fTemp = SetHeaterTemp_MaxIndex - Temperature.iTriTempDefault_Ini[1];
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, fTemp, 0.00);
                    }
                }
                else if(Buffer->Tag>=tcOutSht1 && Buffer->Tag<=tcOutSht2)
                {
                    if(SetHeaterTemp_MaxOutSht - Temperature.iTriTempDefault_Ini[0] > 30)
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 30.00, 0.00);
                    }
                    else
                    {
                        fTemp = SetHeaterTemp_MaxOutSht - Temperature.iTriTempDefault_Ini[0];
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, fTemp, 0.00);
                    }
                }
                else
                {
                    if(SetHeaterTemp_MaxBase - Temperature.iTriTempDefault_Ini[2] > 30)
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 30.00, 0.00);
                    }
                    else
                    {
                        fTemp = SetHeaterTemp_MaxBase - Temperature.iTriTempDefault_Ini[2];
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, fTemp, 0.00);
                    }
                }
            }
            else if(((Buffer->Tag>=tcAa1 && Buffer->Tag<=tcBd2) ||
                     (Buffer->Tag>=tcAe1 && Buffer->Tag<=tcBh2)) ||
                     (Buffer->Tag>=tcHotPlate1 && Buffer->Tag<=tcShuttle2) ||
                     (Buffer->Tag>=tcHotPlate3 && Buffer->Tag<=tcShuttle4))
            {
                if(Temperature.fWorkTemperBase>0)//Temperature > 0
                {
                    if((dTempMax - Temperature.fWorkTemperBase)>30)
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 30.00, -30.00);
                    }
                    else
                    {
                        fTemp = dTempMax - Temperature.fWorkTemperBase;
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, fTemp, (-1)*fTemp);
                    }
                }
                else                             //Temperature < 0
                {
                    if(abs(dTempMin - Temperature.fWorkTemperBase)>30)
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 30.00, -30.00);
                    }
                    else
                    {
                        fTemp = dTempMin - Temperature.fWorkTemperBase;
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, fTemp, (-1)*fTemp);
                    }
                }
            }
        }
        else
        {
            if(CosFunction.bOffsetTempByRecipeMinMaxLimit &&
               Sender==edOffset)
            {
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)Temperature.iOffsetByRecipeMaxLimit, (double)Temperature.iOffsetByRecipeMinLimit);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iTempHigh, (double)InputLimit.iTempLow);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::edLimitMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.0, 12.0);
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::edIndiviMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TEdit *Buffer;                                                              //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    Buffer=(TEdit *)Sender;                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    dTempMax=fTemp_Set->MaxTempSetting();                                       //Steven 20170427 : 回傳機台可以用的最大溫度值
    dTempMin=fTemp_Set->MinTempSetting();
    if(Tri_Temp_Machine==1)                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        if(Buffer->Tag>=tcDUT1 && Buffer->Tag<=tcDUT4)
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)SetHeaterTemp_MaxIndex, 20.00);
        }
        else if(Buffer->Tag>=tcOutSht1 && Buffer->Tag<=tcOutSht2)
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)SetHeaterTemp_MaxOutSht, 20.00);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)SetHeaterTemp_MaxBase, 20.00);
        }
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, dTempMax, dTempMin);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::edinitialMouseDown(TObject *Sender,               //kevin 20210421 獨立offset range
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TEdit *Buffer;
    Buffer=(TEdit *)Sender;
    if(Buffer->Tag==1)
    {
        if(Barcode_Reader(bcTemperature)==0)                                    // 20140103 wei KYEC Barcode Reader
        {
            return;
        }
    }
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(INSTALL_HEAT_GUN &&
           (Buffer->Tag==tcHeatGun1 ||Buffer->Tag==tcHeatGun2))                 //kevin 20200101  hot gun
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iHeaterGunH , (double)InputLimit.iHeaterGunL);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iIlitialTempHigh, (double)InputLimit.iIlitialTempLow);
        }
    }
    else
    {
        if(INSTALL_HEAT_GUN &&
           (Buffer->Tag==tcHeatGun1 ||Buffer->Tag==tcHeatGun2))                 //kevin 20200101  hot gun
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iHeaterGunH , (double)InputLimit.iHeaterGunL);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iTempHigh, (double)InputLimit.iTempLow);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::SetCaption(AnsiString Name)
{
    labName->Caption=Name;
}
//---------------------------------------------------------------------------
void __fastcall TMyTempPanel::SetIndexTag(int iTag)
{
    iIndexTag=iTag;
}
//---------------------------------------------------------------------------
AnsiString TMyTempPanel::GetCaption()
{
    return labName->Caption;
}
//---------------------------------------------------------------------------
