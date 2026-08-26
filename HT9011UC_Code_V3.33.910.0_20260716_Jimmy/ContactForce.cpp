#include "MachineDefine.h"
#pragma hdrstop

#include "ContactForce.h"
//#include "INPUT.h"

#include "cContact.h"
#include "cprod.h"
#include "adam6024.h"
#include "cmydef.h"
#include "common.h"
#include "cpublic.h"
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfContactForce *fContactForce;
bool bNeedWriteFile=false;                                                      //Ifor 20200904 add:KYEC 缸徑自動轉換 30=>28  60=>58
TEdit *IndexZOffsetEdit[2][16];                                                 //Ifor 20190730 : add FFC Temperature Offset   //Ifor 20230206 add: 10 => 11   //Ifor 20230829 add: 11 => 16
//---------------------------------------------------------------------------
THTSLKClass::THTSLKClass(AnsiString Dia, int Tag, bool bDefault) : TComponent(Owner)
{
    AnsiString Str1, Str2;
    sDiameter   =Dia;
    iTag        =Tag;
    bShow       =bDefault;

    Str1.sprintf("gbLoadRate_%s", Dia);
    Str2.sprintf("Load rate of %s mm", Dia);
    gbLoadRate                  =new TGroupBox(fContactForce->scrlbxDynamicKit);
    gbLoadRate->Parent          =fContactForce->scrlbxDynamicKit;
    gbLoadRate->Name            =Str1;
    gbLoadRate->Caption         =Str2;
    gbLoadRate->Font->Size      =12;
    gbLoadRate->Font->Name      ="MS Sans Serif";
    gbLoadRate->Font->Charset   =DEFAULT_CHARSET;
    gbLoadRate->Top             =100*Tag;
    gbLoadRate->Height          =100;
    gbLoadRate->Align           =alTop;

    Str1.sprintf("lblDiameter_%s", Dia);
    Str2.sprintf("%s mm :", Dia);
    lblDiameter=new TLabel(gbLoadRate);
    lblDiameter->Parent         =gbLoadRate;
    lblDiameter->Name           =Str1;
    lblDiameter->Top            =31;
    lblDiameter->Left           =6;
    lblDiameter->Caption        =Str2;

    Str1.sprintf("lblDiameter_NS_%s", Dia);
    Str2.sprintf("%s mm for NS :", Dia);
    lblDiameter_NS=new TLabel(gbLoadRate);
    lblDiameter_NS->Parent      =gbLoadRate;
    lblDiameter_NS->Name        =Str1;
    lblDiameter_NS->Top         =63;
    lblDiameter_NS->Left        =6;
    lblDiameter_NS->Caption     =Str2;

    Str1.sprintf("lblHotOffset_%s", Dia);
    Str2.sprintf("%s mm offset by heater mode:", Dia);
    lblHotOffset=new TLabel(gbLoadRate);
    lblHotOffset->Parent        =gbLoadRate;
    lblHotOffset->Name          =Str1;
    lblHotOffset->Top           =30;
    lblHotOffset->Left          =464;
    lblHotOffset->Caption       =Str2;

    Str1.sprintf("lblContactOffset_%s", Dia);
    Str2.sprintf("%s mm contact offset:", Dia);
    lblContactOffset=new TLabel(gbLoadRate);
    lblContactOffset->Parent    =gbLoadRate;
    lblContactOffset->Name      =Str1;
    lblContactOffset->Top       =62;
    lblContactOffset->Left      =464;
    lblContactOffset->Caption   =Str2;

    Str1.sprintf("edtHotOffset_%s", Dia);
    edtHotOffset=new TEdit(gbLoadRate);
    edtHotOffset->Parent        =gbLoadRate;
    edtHotOffset->Name          =Str1;
    edtHotOffset->Top           =26;
    edtHotOffset->Left          =678;
    edtHotOffset->Height        =28;
    edtHotOffset->Width         =80;
    edtHotOffset->Text          ="";
    edtHotOffset->OnClick       =fContactForce->edtHotOffsetClick;

    Str1.sprintf("edtContactOffset_%s", Dia);
    edtContactOffset=new TEdit(gbLoadRate);
    edtContactOffset->Parent        =gbLoadRate;
    edtContactOffset->Name          =Str1;
    edtContactOffset->Top           =58;
    edtContactOffset->Left          =622;
    edtContactOffset->Height        =28;
    edtContactOffset->Width         =80;
    edtContactOffset->Text          ="";
    edtContactOffset->OnClick       =fContactForce->edtContactOffsetClick;

    Str1.sprintf("trckbrDiameter_%s", Dia);
    trckbrDiameter=new TTrackBar(gbLoadRate);
    trckbrDiameter->Parent      =gbLoadRate;
    trckbrDiameter->Name        =Str1;
    trckbrDiameter->Position    =100;
    trckbrDiameter->Top         =26;
    trckbrDiameter->Left        =108;
    trckbrDiameter->Height      =35;
    trckbrDiameter->Width       =275;
    trckbrDiameter->Min         =80;
    trckbrDiameter->Max         =150;
    trckbrDiameter->OnChange    =trckbrDiameter_Change;

    Str1.sprintf("trckbrDiameter_NS_%s", Dia);
    trckbrDiameter_NS=new TTrackBar(gbLoadRate);
    trckbrDiameter_NS->Parent   =gbLoadRate;
    trckbrDiameter_NS->Name     =Str1;
    trckbrDiameter_NS->Position =100;
    trckbrDiameter_NS->Top      =58;
    trckbrDiameter_NS->Left     =108;
    trckbrDiameter_NS->Height   =35;
    trckbrDiameter_NS->Width    =275;
    trckbrDiameter_NS->Min      =80;
    trckbrDiameter_NS->Max      =150;
    trckbrDiameter_NS->OnChange =trckbrDiameter_NSChange;

    Str1.sprintf("edtLoadRate%s", Dia);
    edtLoadRate=new TEdit(gbLoadRate);
    edtLoadRate->Parent         =gbLoadRate;
    edtLoadRate->Name           =Str1;
    edtLoadRate->Top            =26;
    edtLoadRate->Left           =380;
    edtLoadRate->Height         =28;
    edtLoadRate->Width          =65;
    edtLoadRate->Text           =AnsiString(trckbrDiameter->Position/100.0);
    edtLoadRate->Enabled        =false;
    edtLoadRate->OnClick       =fContactForce->edtLoadRateIndClick;

    Str1.sprintf("edtLoadRate_NS%s", Dia);
    edtLoadRate_NS=new TEdit(gbLoadRate);
    edtLoadRate_NS->Parent      =gbLoadRate;
    edtLoadRate_NS->Name        =Str1;
    edtLoadRate_NS->Top         =58;
    edtLoadRate_NS->Left        =380;
    edtLoadRate_NS->Height      =28;
    edtLoadRate_NS->Width       =65;
    edtLoadRate_NS->Text        =AnsiString(trckbrDiameter_NS->Position/100.0);
    edtLoadRate_NS->Enabled     =false;
    edtLoadRate_NS->OnClick     =fContactForce->edtLoadRateIndClick;

    Str1.sprintf("lblContactOffset_NS%s", Dia);                                 //kevin 20170807 (Steven) add
    Str2.sprintf("%s mm contact offset_NS:", Dia);
    lblContactOffset_NS=new TLabel(gbLoadRate);
    lblContactOffset_NS->Parent    =gbLoadRate;
    lblContactOffset_NS->Name      =Str1;
    lblContactOffset_NS->Top       =58;
    lblContactOffset_NS->Left      =464;
    lblContactOffset_NS->Caption   =Str2;

    Str1.sprintf("edtContactOffset_NS_%s", Dia);
    edtContactOffset_NS=new TEdit(gbLoadRate);
    edtContactOffset_NS->Parent        =gbLoadRate;
    edtContactOffset_NS->Name          =Str1;
    edtContactOffset_NS->Top           =58;
    edtContactOffset_NS->Left          =650;
    edtContactOffset_NS->Height        =28;
    edtContactOffset_NS->Width         =80;
    edtContactOffset_NS->Text          ="";
    edtContactOffset_NS->OnClick       =fContactForce->edtContactOffset_NSClick;
//    if(Tag<4)
//        btnDeleteKit->Visible=false;

        if(Dia=="80_Hi")
        {
            dDiameter=80;
        }
    else if(Dia=="40x2")                                                        //Ifor 20230830 add:40 倍立缸
    {
        dDiameter=402;
    }
    else
    {
        if(EP_Install==5)
        {
            if(Dia.Pos("Arm2_")==0)
            {
                Dia=Dia.SubString(6, Dia.Length());
            }
        }
        dDiameter=atof(Dia.c_str());
    }

    if(dDiameter<=30.0)
        dMinForce=0.5;
    else if(dDiameter<40.0)
        dMinForce=1;
    else if(dDiameter<50.0)
        dMinForce=2;
    else if(dDiameter<60.0)
        dMinForce=4;
    else if(dDiameter==402)
        dMinForce=4;
    else
        dMinForce=8;

    dMaxForce=3.14*(dDiameter/100.0)*(dDiameter/100.0)*500;
    gbLoadRate->Visible=bShow;
}
//==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
//---------------------------------------------------------------------------
THTDieForceOneByOneSLKClass::THTDieForceOneByOneSLKClass(AnsiString Dia, int Tag, bool bDefault) : TComponent(Owner)    //Ifor 20191003 : add Die Force 可以自定義Kit直徑
{
    AnsiString Str1, Str2;
    sDiameter   =Dia;
    iTag        =Tag;
    bShow       =bDefault;
    int iCount=(Tag%8)+1;

    Str1.sprintf("gbDieForceOneByOneLoadRate_%s_%d", Dia, Tag);
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        if(iCount<5)
            Str2.sprintf("DieForce One By One Load rate of %s mm Arm1_%d", Dia, iCount);
        else
            Str2.sprintf("DieForce One By One Load rate of %s mm Arm2_%d", Dia, iCount-4);
    }
    else
    {
       Str2.sprintf("DieForce One By One Load rate of %s mm", Dia);
    }
    gbDieForceOneByOneLoadRate                  =new TGroupBox(fContactForce->scrlbxDieForceOneByOneDynamicKit);
    gbDieForceOneByOneLoadRate->Parent          =fContactForce->scrlbxDieForceOneByOneDynamicKit;
    gbDieForceOneByOneLoadRate->Name            =Str1;
    gbDieForceOneByOneLoadRate->Caption         =Str2;
    gbDieForceOneByOneLoadRate->Font->Size      =12;
    gbDieForceOneByOneLoadRate->Font->Name      ="MS Sans Serif";
    gbDieForceOneByOneLoadRate->Font->Charset   =DEFAULT_CHARSET;
    gbDieForceOneByOneLoadRate->Top             =60*Tag;
    gbDieForceOneByOneLoadRate->Height          =100;
    gbDieForceOneByOneLoadRate->Align           =alTop;

    Str1.sprintf("lblDieForceOneByOneDiameter_%s_%d", Dia, Tag);
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        if(iCount<5)
            Str2.sprintf("%s mm 1_%d:", Dia, iCount);
        else
            Str2.sprintf("%s mm 2_%d:", Dia, iCount-4);
    }
    else
    {
        Str2.sprintf("%s mm :", Dia);
    }
    lblDieForceOneByOneDiameter=new TLabel(gbDieForceOneByOneLoadRate);
    lblDieForceOneByOneDiameter->Parent         =gbDieForceOneByOneLoadRate;
    lblDieForceOneByOneDiameter->Name           =Str1;
    lblDieForceOneByOneDiameter->Top            =31;
    lblDieForceOneByOneDiameter->Left           =6;
    lblDieForceOneByOneDiameter->Caption        =Str2;

    Str1.sprintf("lblDieForceOneByOneContactOffset_%s_%d", Dia, Tag);
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        if(iCount<5)
            Str2.sprintf("%s mm contact offset 1_%d:", Dia, iCount);
        else
            Str2.sprintf("%s mm contact offset 2_%d:", Dia, iCount-4);
    }
    else
    {
        Str2.sprintf("%s mm contact offset:", Dia);
    }
    lblDieForceOneByOneContactOffset=new TLabel(gbDieForceOneByOneLoadRate);
    lblDieForceOneByOneContactOffset->Parent    =gbDieForceOneByOneLoadRate;
    lblDieForceOneByOneContactOffset->Name      =Str1;
    lblDieForceOneByOneContactOffset->Top       =24;
    lblDieForceOneByOneContactOffset->Left      =464;
    lblDieForceOneByOneContactOffset->Caption   =Str2;

    Str1.sprintf("edtDieForceOneByOneContactOffset_%s_%d", Dia, Tag);
    edtDieForceOneByOneContactOffset=new TEdit(gbDieForceOneByOneLoadRate);
    edtDieForceOneByOneContactOffset->Parent        =gbDieForceOneByOneLoadRate;
    edtDieForceOneByOneContactOffset->Name          =Str1;
    edtDieForceOneByOneContactOffset->Top           =26;
    edtDieForceOneByOneContactOffset->Left          =676;
    edtDieForceOneByOneContactOffset->Height        =28;
    edtDieForceOneByOneContactOffset->Width         =80;
    edtDieForceOneByOneContactOffset->Text          ="";
    edtDieForceOneByOneContactOffset->OnClick       =fContactForce->edtContactOffsetClick;

    Str1.sprintf("trckbrDieForceOneByOneDiameter_%s_%d", Dia, Tag);
    trckbrDieForceOneByOneDiameter=new TTrackBar(gbDieForceOneByOneLoadRate);
    trckbrDieForceOneByOneDiameter->Parent      =gbDieForceOneByOneLoadRate;
    trckbrDieForceOneByOneDiameter->Name        =Str1;
    trckbrDieForceOneByOneDiameter->Position    =100;
    trckbrDieForceOneByOneDiameter->Top         =26;
    trckbrDieForceOneByOneDiameter->Left        =108;
    trckbrDieForceOneByOneDiameter->Height      =35;
    trckbrDieForceOneByOneDiameter->Width       =275;
    trckbrDieForceOneByOneDiameter->Min         =80;
    trckbrDieForceOneByOneDiameter->Max         =150;
    trckbrDieForceOneByOneDiameter->OnChange    =trckbrDieForceOneByOneDiameter_Change;

    Str1.sprintf("edtDieForceOneByOneLoadRate%s_%d", Dia, Tag);
    edtDieForceOneByOneLoadRate=new TEdit(gbDieForceOneByOneLoadRate);
    edtDieForceOneByOneLoadRate->Parent         =gbDieForceOneByOneLoadRate;
    edtDieForceOneByOneLoadRate->Name           =Str1;
    edtDieForceOneByOneLoadRate->Top            =26;
    edtDieForceOneByOneLoadRate->Left           =380;
    edtDieForceOneByOneLoadRate->Height         =28;
    edtDieForceOneByOneLoadRate->Width          =65;
    edtDieForceOneByOneLoadRate->Text           =AnsiString(trckbrDieForceOneByOneDiameter->Position/100.0);
    edtDieForceOneByOneLoadRate->Enabled        =false;

    dDiameter=atof(Dia.c_str());

    if(dDiameter<=30.0)
        dMinForce=0.5;
    else if(dDiameter<40.0)
        dMinForce=1;
    else if(dDiameter<50.0)
        dMinForce=2;
    else if(dDiameter<60.0)
        dMinForce=4;
    else
        dMinForce=8;

    dMaxForce=3.14*(dDiameter/100.0)*(dDiameter/100.0)*500;
    gbDieForceOneByOneLoadRate->Visible=bShow;
}
//<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end
//---------------------------------------------------------------------------
THTDieForceSLKClass::THTDieForceSLKClass(AnsiString Dia, int Tag, bool bDefault) : TComponent(Owner)    //Ifor 20191003 : add Die Force 可以自定義Kit直徑
{
    AnsiString Str1, Str2;
    sDiameter   =Dia;
    iTag        =Tag;
    bShow       =bDefault;

    Str1.sprintf("gbDieForceLoadRate_%s", Dia);
    Str2.sprintf("DieForce Load rate of %s mm", Dia);
    gbDieForceLoadRate                  =new TGroupBox(fContactForce->scrlbxDieForceDynamicKit);
    gbDieForceLoadRate->Parent          =fContactForce->scrlbxDieForceDynamicKit;
    gbDieForceLoadRate->Name            =Str1;
    gbDieForceLoadRate->Caption         =Str2;
    gbDieForceLoadRate->Font->Size      =12;
    gbDieForceLoadRate->Font->Name      ="MS Sans Serif";
    gbDieForceLoadRate->Font->Charset   =DEFAULT_CHARSET;
    gbDieForceLoadRate->Top             =100*Tag;
    gbDieForceLoadRate->Height          =100;
    gbDieForceLoadRate->Align           =alTop;

    Str1.sprintf("lblDieForceDiameter_%s", Dia);
    Str2.sprintf("%s mm :", Dia);
    lblDieForceDiameter=new TLabel(gbDieForceLoadRate);
    lblDieForceDiameter->Parent         =gbDieForceLoadRate;
    lblDieForceDiameter->Name           =Str1;
    lblDieForceDiameter->Top            =31;
    lblDieForceDiameter->Left           =6;
    lblDieForceDiameter->Caption        =Str2;

    Str1.sprintf("lblDieForceContactOffset_%s", Dia);
    Str2.sprintf("%s mm contact offset:", Dia);
    lblDieForceContactOffset=new TLabel(gbDieForceLoadRate);
    lblDieForceContactOffset->Parent    =gbDieForceLoadRate;
    lblDieForceContactOffset->Name      =Str1;
    lblDieForceContactOffset->Top       =62;
    lblDieForceContactOffset->Left      =464;
    lblDieForceContactOffset->Caption   =Str2;

    Str1.sprintf("edtDieForceContactOffset_%s", Dia);
    edtDieForceContactOffset=new TEdit(gbDieForceLoadRate);
    edtDieForceContactOffset->Parent        =gbDieForceLoadRate;
    edtDieForceContactOffset->Name          =Str1;
    edtDieForceContactOffset->Top           =58;
    edtDieForceContactOffset->Left          =622;
    edtDieForceContactOffset->Height        =28;
    edtDieForceContactOffset->Width         =80;
    edtDieForceContactOffset->Text          ="";
    edtDieForceContactOffset->OnClick       =fContactForce->edtContactOffsetClick;

    Str1.sprintf("trckbrDieForceDiameter_%s", Dia);
    trckbrDieForceDiameter=new TTrackBar(gbDieForceLoadRate);
    trckbrDieForceDiameter->Parent      =gbDieForceLoadRate;
    trckbrDieForceDiameter->Name        =Str1;
    trckbrDieForceDiameter->Position    =100;
    trckbrDieForceDiameter->Top         =26;
    trckbrDieForceDiameter->Left        =108;
    trckbrDieForceDiameter->Height      =35;
    trckbrDieForceDiameter->Width       =275;
    trckbrDieForceDiameter->Min         =80;
    trckbrDieForceDiameter->Max         =150;
    trckbrDieForceDiameter->OnChange    =trckbrDieForceDiameter_Change;

    Str1.sprintf("edtDieForceLoadRate%s", Dia);
    edtDieForceLoadRate=new TEdit(gbDieForceLoadRate);
    edtDieForceLoadRate->Parent         =gbDieForceLoadRate;
    edtDieForceLoadRate->Name           =Str1;
    edtDieForceLoadRate->Top            =26;
    edtDieForceLoadRate->Left           =380;
    edtDieForceLoadRate->Height         =28;
    edtDieForceLoadRate->Width          =65;
    edtDieForceLoadRate->Text           =AnsiString(trckbrDieForceDiameter->Position/100.0);
    edtDieForceLoadRate->Enabled        =false;

    dDiameter=atof(Dia.c_str());

    if(dDiameter<=30.0)
        dMinForce=0.5;
    else if(dDiameter<40.0)
        dMinForce=1;
    else if(dDiameter<50.0)
        dMinForce=2;
    else if(dDiameter<60.0)
        dMinForce=4;
    else
        dMinForce=8;

    dMaxForce=3.14*(dDiameter/100.0)*(dDiameter/100.0)*500;
    gbDieForceLoadRate->Visible=bShow;
}
//---------------------------------------------------------------------------
__fastcall TfContactForce::TfContactForce(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    bHasFile=false;
    bNeedWriteFile=false;                                                       //Ifor 20200908 add:需要重新寫入檔案
    AnsiString Str;
    FileName="D:\\HT9045\\system\\ContactInfo.ini";

    if(CosFunction.bUseDynamicKitDiameter)                                      //Steven 20170605 : 可以自定義Kit直徑
    {
        if(FileExists(FileName))
        {
            bHasFile=true;
        }

        if(CUSTOMER_CODE==CC_KYEC_LEE)
        {
            edtCurrentType->Text=CheckAndReadIniData(FileName, AnsiString("SLK Type"), AnsiString("Type"),    AnsiString("28,40,58,56"));
            edtVisible->Text    =CheckAndReadIniData(FileName, AnsiString("SLK Type"), AnsiString("Visible"), AnsiString("1,1,1,1"));
            if(edtCurrentType->Text.Pos("30")==1 || edtCurrentType->Text.Pos("60")==1)  //Ifor 20200904 add:KYEC 缸徑自動轉換 30=>28  60=>58
            {
                bNeedWriteFile=true;                                            //Ifor 20200908 add:需要重新寫入檔案
            }
        }
        else
        {
            edtCurrentType->Text=CheckAndReadIniData(FileName, AnsiString("SLK Type"), AnsiString("Type"),    AnsiString("30,40,60,56"));
            edtVisible->Text    =CheckAndReadIniData(FileName, AnsiString("SLK Type"), AnsiString("Visible"), AnsiString("1,1,1,0"));
        }

        edtCurrentTypeInd->Text=CheckAndReadIniData(FileName, AnsiString("SLK Type Ind"), AnsiString("Type"),    AnsiString("20,30"));
        edtVisibleInd->Text    =CheckAndReadIniData(FileName, AnsiString("SLK Type Ind"), AnsiString("Visible"), AnsiString("1,0"));

        slSLKType=new TStringList();
        slSLKType->Clear();
        slSLKType->CommaText=edtCurrentType->Text;

        slSLKTypeVisible=new TStringList();
        slSLKTypeVisible->Clear();
        slSLKTypeVisible->CommaText=edtVisible->Text;

        for(int i=0; i<slSLKType->Count; i++)
        {
            if(slSLKType->Strings[i]!="" && atof(slSLKType->Strings[i].c_str())>15.0)
            {
                if(slSLKType->Strings[i]=="402")                                //Ifor 20230830 add:40 倍立缸
                {
                    SLKClass.push_back(new THTSLKClass("40x2", i, (bool)atoi(slSLKTypeVisible->Strings[i].c_str())));
                }
                else if(EP_Install==5)
                {
                    Str.sprintf("Arm1_%s",slSLKType->Strings[i]);
                    SLKClass.push_back(new THTSLKClass(Str, i, (bool)atoi(slSLKTypeVisible->Strings[i].c_str())));
                    Str.sprintf("Arm2_%s",slSLKType->Strings[i]);
                    SLKClass.push_back(new THTSLKClass(Str, i, (bool)atoi(slSLKTypeVisible->Strings[i].c_str())));
                }
                else
                {
                    SLKClass.push_back(new THTSLKClass(slSLKType->Strings[i], i, (bool)atoi(slSLKTypeVisible->Strings[i].c_str())));
                }

                if(CUSTOMER_CODE==CC_ASE_SG)
                {
                    if(slSLKType->Strings[i]=="80")
                    {
                        SLKClass.push_back(new THTSLKClass("80_Hi", i+1, (bool)atoi(slSLKTypeVisible->Strings[i].c_str())));
                    }
                }
            }
        }

        slSLKTypeInd=new TStringList();
        slSLKTypeInd->Clear();
        slSLKTypeInd->CommaText=edtCurrentTypeInd->Text;

        slSLKTypeIndVisible=new TStringList();
        slSLKTypeIndVisible->Clear();
        slSLKTypeIndVisible->CommaText=edtVisibleInd->Text;
        //AI(ht9045-v899) 20260504: Multi EP (INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI) SLKIndClass uses i*8+j (Arm1_1..4, Arm2_1..4). Single-force core port; iIndEPCnt==8 branch below kept for modes 0/1/2.
        if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
        {
            for(int i=0; i<slSLKTypeInd->Count; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(slSLKTypeInd->Strings[i]!="" && atof(slSLKTypeInd->Strings[i].c_str())>15.0)
                    {
                        SLKIndClass.push_back(new THTSLKIndClass(slSLKTypeInd->Strings[i], i*8+j, (bool)atoi(slSLKTypeIndVisible->Strings[i].c_str())));
                    }
                }
            }
        }
        else
        {
            for(int i=0; i<slSLKTypeInd->Count; i++)
            {
                for(int j=0; j<16; j++)
                {
                    if(slSLKTypeInd->Strings[i]!="" && atof(slSLKTypeInd->Strings[i].c_str())>15.0)
                    {
                        if((iIndEPCnt==4 ||                                         //JerryYang 20210413 : 4組獨立EP版本使用0,1,4,5
                            (iIndEPCnt==8 && TestIF_File.iTestMode==DualSite))&&    //RogerYang 20260603 : Add 8EP
                            (j==2 || j==3 || j>=6))
                        {
                            SLKIndClass.push_back(new THTSLKIndClass(slSLKTypeInd->Strings[i], i*16+j, false));
                        }
                        else if(iIndEPCnt==8 && (j>=8))                             //RogerYang 20260603 : Add 8EP
                        {
                            SLKIndClass.push_back(new THTSLKIndClass(slSLKTypeInd->Strings[i], i*16+j, false));
                        }
                        else
                        {
                            SLKIndClass.push_back(new THTSLKIndClass(slSLKTypeInd->Strings[i], i*16+j, (bool)atoi(slSLKTypeVisible->Strings[i].c_str())));
                        }
                    }
                }
            }
        }

        EP_MAXKPA  =CheckAndReadIniDataGeneral("System" , "EP_MAXKPA" , 499.0); //kevin 20170803 (wei) 避免一開始值被修改
        EP_MAXAFB  =CheckAndReadIniDataGeneral("System" , "EP_MAXA"   , 5.013); //kevin 20170803 (wei) 避免一開始值被修改
        EP_MINMPA  =CheckAndReadIniDataGeneral("System" , "EP_MINMPA" , 0.001); //JerryYang 20171023 (wei) add PA Min  //kevin 20170803 (wei) 避免一開始值被修改
//        for(int i=0; i<3; i++)
//        {
//            for(int j=0; j<11; j++) //Ifor 20230206 add: 新增四組Contact 補償 7 => 11
//            {
//                if(i==2)
//                {
//                    if(j>=7)
//                    {
//                        dIndexZOffset[i][j]=120+(6*60)+((j-6)*40);
//                    }
//                    else
//                    {
//                        dIndexZOffset[i][j]=120+(j*60);
//                    }
//                }
//                else
//                {
//                    Str.sprintf("dIndexZOffset[%d][%d]", i, j);    //JerryYang 20190910 fix ATC offset存檔問題
//                    dIndexZOffset[i][j]             =CheckRange(CheckAndReadIniDataGeneral("Test Arm", Str, 0.0), 10.0, 0.0);
//                    IndexZOffsetEdit[i][j]->Text    =dIndexZOffset[i][j];
//                }
//            }
//        }
        edMaxKpa->Text      = EP_MAXKPA;                                        //kevin 20170803 (wei) 避免一開始值被修改
        edMaxMpaFB->Text    = EP_MAXAFB;                                        //kevin 20170803 (wei) 避免一開始值被修改
        edMinMpa->Text      = EP_MINMPA;                                        //JerryYang 20171023 (wei) add PA Min  //kevin 20170803 (wei) 避免一開始值被修改

        edMaxKpa_1032->Text      = EP_MAXKPA_1032;                              //kevin 20170803 (wei) 避免一開始值被修改
        edMaxMpaFB_1032->Text    = EP_MAXAFB_1032;                              //kevin 20170803 (wei) 避免一開始值被修改
        edMinMpa_1032->Text      = EP_MINMPA_1032;                              //JerryYang 20171023 (wei) add PA Min  //kevin 20170803 (wei) 避免一開始值被修改

        if(INSTALL_DOUBLE_EP>0)                                                 //Ifor 20191003 : add Die Force 可以自定義Kit直徑
        {
            if(FileExists(FileName))
            {
                edtDieForceCurrentType->Text=CheckAndReadIniData(FileName, AnsiString("SLK Type"), AnsiString("DieForceType"),    AnsiString("20,30,40,50"));
                if(IniConfig.bSPILFunction==true)                               //JerryYang 20200805 : 國龍要求dual force缸徑由軟體決定,不要從文字檔改
                {
                    edtDieForceVisible->Text    =CheckAndReadIniData(FileName, AnsiString("SLK Type"), AnsiString("DieForceVisible"), AnsiString("1,1,1,1"));
                }
                else
                {
                    edtDieForceVisible->Text    =CheckAndReadIniData(FileName, AnsiString("SLK Type"), AnsiString("DieForceVisible"), AnsiString("1,1,1,0"));
                }
            }
            else
            {
                edtDieForceCurrentType->Text="20,30,40,50";
                edtDieForceVisible->Text    ="1,1,1,0";
            }

            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20200805 : 國龍要求dual force缸徑由軟體決定,不要從文字檔改
            {
                if(edtDieForceCurrentType->Text=="20,30,40,50")
                {
                    edtDieForceVisible->Text="1,1,1,1";
                }
            }

            slDieForceSLKType=new TStringList();
            slDieForceSLKType->Clear();
            slDieForceSLKType->CommaText=edtDieForceCurrentType->Text;

            slDieForceSLKTypeVisible=new TStringList();
            slDieForceSLKTypeVisible->Clear();
            slDieForceSLKTypeVisible->CommaText=edtDieForceVisible->Text;

            for(int i=0; i<slDieForceSLKType->Count; i++)
            {
                if(slDieForceSLKType->Strings[i]!="" && atof(slDieForceSLKType->Strings[i].c_str())>15.0)
                    DieForceSLKClass.push_back(new THTDieForceSLKClass(slDieForceSLKType->Strings[i], i, (bool)atoi(slDieForceSLKTypeVisible->Strings[i].c_str())));
            }
            //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
            slDieForceOneByOneSLKType=new TStringList();
            slDieForceOneByOneSLKType->Clear();
            slDieForceOneByOneSLKType->CommaText=edtDieForceCurrentType->Text;

            slDieForceOneByOneSLKTypeVisible=new TStringList();
            slDieForceOneByOneSLKTypeVisible->Clear();
            slDieForceOneByOneSLKTypeVisible->CommaText=edtDieForceVisible->Text;
            if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
            {
                for(int i=0; i<slDieForceOneByOneSLKType->Count; i++)
                {
                    for(int j=0; j<8; j++)
                    {
                        if(slDieForceSLKType->Strings[i]!="" && atof(slDieForceSLKType->Strings[i].c_str())>15.0)
                        {
                            DieForceOneByOneSLKClass.push_back(new THTDieForceOneByOneSLKClass(slDieForceSLKType->Strings[i], i*8+j, (bool)atoi(slDieForceOneByOneSLKTypeVisible->Strings[i].c_str())));
                        }
                    }
                }
            }
            //<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end
        }

        if(bHasFile)
            ReadFile();
        else
            WriteFile();

        slSLKType->Clear();
        delete slSLKType;
        slSLKType=NULL;
        slSLKTypeVisible->Clear();
        delete slSLKTypeVisible;
        slSLKTypeVisible=NULL;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::FormDestroy(TObject *Sender)
{
    try
    {
        if(CosFunction.bUseDynamicKitDiameter)
        {
            if(slSLKType!=NULL)
            {
                for(vector<THTSLKClass *>::iterator iter=SLKClass.begin(); iter!=SLKClass.end(); ++iter)
                {
                    delete *iter;
                }
                vec_clr(SLKClass);
            //    slSLKType->Clear();
                delete slSLKType;
            }

            if(slSLKTypeVisible!=NULL)
            {
                slSLKTypeVisible->Clear();
                delete slSLKTypeVisible;
            }
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfContactForce::FormDestroy");
    }
    LogSoftwareOffTime("TfContactForce, FormDestroy");                          //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::FormShow(TObject *Sender)
{
    fShow=true;
    ReadFile();
    Left=10;                                                                    //Steven 20220407 : 避免跑到畫面外
    Top=10;

    tsNewMethod->TabVisible         =(WEIGHT_CALIBRATION);                      //Steven 20150224 : Contact畫面可以調整D25設定值
    if(CosFunction.bUseDynamicKitDiameter)                                      //Steven 20170605 : 可以自定義Kit直徑  //Jimmychiu 20230630 : Individual EP No effect contact force offset of 40mm compliance
    {
        //AI(ht9045-v899) 20260526: show Multi EP site trim page (TabSheet1) for outer EP per-site calibration.
        if((INSTALL_DOUBLE_EP==DOUBLE_EP_INDIVIAL || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI) && TestIF_File.bIndEPSLK==true)
        {
            tsDynamicKit->TabVisible=false;
            TabSheet1->TabVisible=true;
            tsDieForceDynamicKit->TabVisible=false;
            tsDieForceOneByOneKit->TabVisible=true;
        }
        else
        {
            tsDynamicKit->TabVisible=true;
            TabSheet1->TabVisible=false;
            tsDieForceOneByOneKit->TabVisible=false;
        }
    }

    tsD25       ->TabVisible        =(!CosFunction.bUseDynamicKitDiameter && CosFunction.bHiSiliconFunction==false);     //Steven 20170605 : 可以自定義Kit直徑
    tsEPCalibration->TabVisible     =true;                                      //JerryYang 20171102 (wei) 新增頁面方便校正PA對應電壓
    grpEPForDieForce->Visible       =(INSTALL_DOUBLE_EP==DOUBLE_EP_NORMAL || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI); //AI(ht9045-v899) 20260430: also show for Multi EP half (inner cyl Die Force).
    //AI(ht9045-v899) 20260430: 16 EP independent + Multi EP half use 2 boards (Arm1 + Arm2); show Arm2 EP calibration UI.
    grpEPForArm2->Visible           =(EP_Install==5 || iIndEPCnt==16 || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI);

    tsNewMethod      ->Enabled=(AccessLevel>=iDefHonPrecLevel || CUSTOMER_CODE==CC_HONPREC_QC)?true:false;
    tsDynamicKit     ->Enabled=(AccessLevel>=iDefHonPrecLevel || CUSTOMER_CODE==CC_HONPREC_QC)?true:false;
    tsD25            ->Enabled=(AccessLevel>=iDefHonPrecLevel || CUSTOMER_CODE==CC_HONPREC_QC)?true:false;
    tsEPCalibration  ->Enabled=(AccessLevel>=iDefEngineerLevel || CUSTOMER_CODE==CC_HONPREC_QC)?true:false; //JerryYang 20171102 (wei) Engineer以上權限才能修改

    bool bNSKit=false,bNSKitSwitch=false;
    if(CosFunction.bEPUseNSSLK==true)                                           //kevin 20170804 (Steven) EP表頭另一種TYPE
    {
        bNSKit=true;                                                            //使用NS KIT
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            if(TestIF_File.bNSKitPress)
                bNSKitSwitch=true;                                              //使用NS KIT
        }
        else
        {
            bNSKitSwitch=true;
        }
    }

    bool bUseNSKit=false;
    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        switch(TestIF.iTestMode)
        {
            case SingleSite:
            case DualSite:
                if(CosFunction.bCanUseBias==true && TestIF_File.bNS7000kit==true)
                {
                    bUseNSKit=true;
                }
                else
                {
                    bUseNSKit=false;
                }
                break;
            case QualSite1X4:
                if((CosFunction.bCanUseBias==true && TestIF_File.bNS7000kit==true) || TestIF_File.bNS7000CS==true)
                {
                    bUseNSKit=true;
                }
                else
                {
                    bUseNSKit=false;
                }
                break;
            case QualSite2X2:
                if((CosFunction.bCanUse2x2Bias==true && TestIF_File.bNS7000kit==true) || TestIF_File.bNS7000CS==true)
                {
                    bUseNSKit=true;
                }
                else
                {
                    bUseNSKit=false;
                }
            case DualSite2x1:
                if(TestIF_File.bNS7000CS==true)
                {
                    bUseNSKit=true;
                }
                else
                {
                    bUseNSKit=false;
                }
                break;
            case TriSite1X3:
            case _8Site1X4:
                if(ATC_SYSTEM>eATC30 && TestIF_File.bNS7000CS==true)
                {
                    bUseNSKit=true;
                }
                else
                {
                    bUseNSKit=false;
                }
                break;
            case _8Site2X4:
                if((ATC_SYSTEM>eATC30 && TestIF_File.bNS7000CS==true) ||
                   (USE_16_HEATER!=eht4Heater && TestIF_File.bNS8000CS==true))
                {
                    bUseNSKit=true;
                }
                else
                {
                    bUseNSKit=false;
                }
                break;
            default :
                bUseNSKit=false;
                break;
        }
//        if(TestIF_File.bNS7000kit || TestIF_File.bNS7000CS || TestIF_File.bNS8000CS)
//        {
//            bUseNSKit=true;
//        }
//        else
//        {
//            bUseNSKit=false;
//        }
    }
    else
    {
        bUseNSKit=false;
    }

    for(unsigned int i=0; i<SLKClass.size(); i++)
    {
        if((bNSKit && bNSKitSwitch) || bUseNSKit==true)                         //kevin 20170808 (Steven) add NS offset
        {
            SLKClass[i]->trckbrDiameter_NS->Visible =true;
            SLKClass[i]->lblDiameter_NS->Visible    =true;
            SLKClass[i]->edtLoadRate_NS->Visible    =true;

            SLKClass[i]->trckbrDiameter->Visible =false;
            SLKClass[i]->lblDiameter->Visible    =false;
            SLKClass[i]->edtLoadRate->Visible    =false;

            SLKClass[i]->edtHotOffset->Visible      =false;
            SLKClass[i]->lblHotOffset->Visible      =false;

            SLKClass[i]->lblContactOffset_NS->Visible =true;
            SLKClass[i]->edtContactOffset_NS->Visible =true;

            SLKClass[i]->edtContactOffset->Visible    =false;
            SLKClass[i]->lblContactOffset->Visible    =false;
        }
        else
        {
            SLKClass[i]->trckbrDiameter->Visible =true;
            SLKClass[i]->lblDiameter->Visible    =true;
            SLKClass[i]->edtLoadRate->Visible    =true;

            SLKClass[i]->trckbrDiameter_NS->Visible =false;
            SLKClass[i]->lblDiameter_NS->Visible    =false;
            SLKClass[i]->edtLoadRate_NS->Visible    =false;

            SLKClass[i]->lblContactOffset_NS->Visible =false;
            SLKClass[i]->edtContactOffset_NS->Visible =false;

            SLKClass[i]->edtContactOffset->Visible    =true;
            SLKClass[i]->lblContactOffset->Visible    =true;
        }

        SLKClass[i]->trckbrDiameter->Min        =80;
        SLKClass[i]->trckbrDiameter->Max        =150;
        SLKClass[i]->trckbrDiameter_NS->Min     =80;
        SLKClass[i]->trckbrDiameter_NS->Max     =150;
        SLKClass[i]->edtHotOffset->Visible      =(CosFunction.bUseLoadCellOffsetByHeater);
        SLKClass[i]->lblHotOffset->Visible      =(CosFunction.bUseLoadCellOffsetByHeater);

        //SLKClass[i]->trckbrDiameter_NS->Visible =(CosFunction.bEPUseNSSLK);
        //SLKClass[i]->lblDiameter_NS->Visible    =(CosFunction.bEPUseNSSLK);
        //SLKClass[i]->edtLoadRate_NS->Visible    =(CosFunction.bEPUseNSSLK);

//        if(i>3) //只有Hontech權限可以增減SLK
//            SLKClass[i]->btnDeleteKit->Visible=(AccessLevel>=iDefHonPrecLevel);
    }
//    grpAddNewKit->Visible=(AccessLevel>=iDefHonPrecLevel);
//    gbEPSetting->Visible=(AccessLevel>=iDefHonPrecLevel);
    if(INSTALL_DOUBLE_EP==1)                                                    //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    {
        tsDieForceDynamicKit->TabVisible=true;
        for(unsigned int i=0; i<DieForceSLKClass.size(); i++)
        {
            DieForceSLKClass[i]->trckbrDieForceDiameter->Min        =80;
            DieForceSLKClass[i]->trckbrDieForceDiameter->Max        =150;
        }
    }
    else
    {
        tsDieForceDynamicKit->TabVisible=false;
    }

    tb30mm_10kg->Position=IniConfig.iContactForceMap[0][0];
    tb30mm_60kg->Position=IniConfig.iContactForceMap[0][1];
    tb40mm_10kg->Position=IniConfig.iContactForceMap[1][0];
    tb40mm_60kg->Position=IniConfig.iContactForceMap[1][1];
    tb60mm_10kg->Position=IniConfig.iContactForceMap[2][0];
    tb60mm_60kg->Position=IniConfig.iContactForceMap[2][1];
    tb56mm_10kg->Position=IniConfig.iContactForceMap[3][0];                     //wei 20151005 add 56mm
    tb56mm_60kg->Position=IniConfig.iContactForceMap[3][1];                     //wei 20151005 add 56mm

    //Steven 20150224 : Contact畫面可以調整D25設定值
    tbD25_Index60mm->Position   =LastSet.dIndexLoadRate[0][0]*100;
    tbD60_Index56mm->Position   =LastSet.dIndexLoadRate[0][1]*100;              //wei 20151005 add 56mm
    tbD25_Index40mm->Position   =LastSet.dIndexLoadRate[0][2]*100;
    tbD25_Index30mm->Position   =LastSet.dIndexLoadRate[0][3]*100;

    edD25_60mm->Text            =LastSet.dIndexLoadRate[2][0];                  //2014-06-26    Dell    for TSMC 高溫Load cell offse
    edD60_56mm->Text            =LastSet.dIndexLoadRate[2][1];                  //2014-06-26    Dell    for TSMC 高溫Load cell offset    //wei 20151005 add 56mm
    edD25_40mm->Text            =LastSet.dIndexLoadRate[2][2];                  //2014-06-26    Dell    for TSMC 高溫Load cell offset
    edD25_30mm->Text            =LastSet.dIndexLoadRate[2][3];                  //2014-06-26    Dell    for TSMC 高溫Load cell offset

    ShowValue();

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    fContact->ShowArmAndDeviceForce();
    DeviceForm.dPress=atof(fContact->edAirForce->Text.c_str());
    DeviceForm_File.dPress=DeviceForm.dPress;                                   //jou 2014-09-06 修正開啟程式的時候EP異常
    ADAM_WriteVoltage(DeviceForm.dPress);

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::btExitClick(TObject *Sender)
{
    ADAM_WriteVoltage(DeviceForm.dPress);
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::btSaveClick(TObject *Sender)
{
    IniConfig.iContactForceMap[0][0]=tb30mm_10kg->Position;
    IniConfig.iContactForceMap[0][1]=tb30mm_60kg->Position;
    IniConfig.iContactForceMap[1][0]=tb40mm_10kg->Position;
    IniConfig.iContactForceMap[1][1]=tb40mm_60kg->Position;
    IniConfig.iContactForceMap[2][0]=tb60mm_10kg->Position;
    IniConfig.iContactForceMap[2][1]=tb60mm_60kg->Position;
    IniConfig.iContactForceMap[3][0]=tb56mm_10kg->Position;                     //wei 20151005 add 56mm
    IniConfig.iContactForceMap[3][1]=tb56mm_60kg->Position;                     //wei 20151005 add 56mm

    //Steven 20150224 : Contact畫面可以調整D25設定值
    LastSet.dIndexLoadRate[0][0]    =tbD25_Index60mm->Position/100.0;           //jou 2011-06-10
    LastSet.dIndexLoadRate[0][1]    =tbD60_Index56mm->Position/100.0;           //jou 2011-06-10     //wei 20151005 add 56mm
    LastSet.dIndexLoadRate[0][2]    =tbD25_Index40mm->Position/100.0;           //Steven 20110704
    LastSet.dIndexLoadRate[0][3]    =tbD25_Index30mm->Position/100.0;           //jou 2011-06-10

    LastSet.dIndexLoadRate[2][0]    =CheckRange(atof(edD25_60mm->Text.c_str()), 0.5,   -0.5);//2014-06-26    Dell    for TSMC 高溫Load cell offset
    LastSet.dIndexLoadRate[2][1]    =CheckRange(atof(edD60_56mm->Text.c_str()), 0.5,   -0.5);//2014-06-26    Dell    for TSMC 高溫Load cell offset          //wei 20151005 add 56mm
    LastSet.dIndexLoadRate[2][2]    =CheckRange(atof(edD25_40mm->Text.c_str()), 0.5,   -0.5);//2014-06-26    Dell    for TSMC 高溫Load cell offset
    LastSet.dIndexLoadRate[2][3]    =CheckRange(atof(edD25_30mm->Text.c_str()), 0.5,   -0.5);//2014-06-26    Dell    for TSMC 高溫Load cell offset

    SaveLastSetIni();

    WriteFile();
    //AI(ht9045-v899) 20260526: reload saved one-by-one load rates so the next TfContact SAVE uses current trim values.
    ReadFile();
    ADAM_WriteVoltage(DeviceForm.dPress);
}
//---------------------------------------------------------------------------
void TfContactForce::ReadFile()
{
    AnsiString sGroup;
    double dTemp;
    bool bHasDiameter=false;
    AnsiString Str;
    IndexZOffsetEdit[0][0]=edtArm1Offset_01;    IndexZOffsetEdit[0][1]=edtArm1Offset_02;    IndexZOffsetEdit[0][2]=edtArm1Offset_03;    IndexZOffsetEdit[0][3]=edtArm1Offset_04;
    IndexZOffsetEdit[0][4]=edtArm1Offset_05;    IndexZOffsetEdit[0][5]=edtArm1Offset_06;    IndexZOffsetEdit[0][6]=edtArm1Offset_07;    IndexZOffsetEdit[0][7]=edtArm1Offset_08;
    IndexZOffsetEdit[0][8]=edtArm1Offset_09;    IndexZOffsetEdit[0][9]=edtArm1Offset_10;    IndexZOffsetEdit[0][10]=edtArm1Offset_11;   IndexZOffsetEdit[0][11]=edtArm1Offset_12;
    IndexZOffsetEdit[0][12]=edtArm1Offset_13;   IndexZOffsetEdit[0][13]=edtArm1Offset_14;   IndexZOffsetEdit[0][14]=edtArm1Offset_15;

    IndexZOffsetEdit[1][0]=edtArm2Offset_01;    IndexZOffsetEdit[1][1]=edtArm2Offset_02;    IndexZOffsetEdit[1][2]=edtArm2Offset_03;    IndexZOffsetEdit[1][3]=edtArm2Offset_04;
    IndexZOffsetEdit[1][4]=edtArm2Offset_05;    IndexZOffsetEdit[1][5]=edtArm2Offset_06;    IndexZOffsetEdit[1][6]=edtArm2Offset_07;    IndexZOffsetEdit[1][7]=edtArm2Offset_08;
    IndexZOffsetEdit[1][8]=edtArm2Offset_09;    IndexZOffsetEdit[1][9]=edtArm2Offset_10;    IndexZOffsetEdit[1][10]=edtArm2Offset_11;   IndexZOffsetEdit[1][11]=edtArm2Offset_12;
    IndexZOffsetEdit[1][12]=edtArm2Offset_13;   IndexZOffsetEdit[1][13]=edtArm2Offset_14;   IndexZOffsetEdit[1][14]=edtArm2Offset_15;

    //jou 20171117 (Steven) : EP校正欄位(從Contact頁面可以進入此setting)
    if(EP_Install!=0)                                                           //Steven 20140524 : 配合統一由外面讀取
    {
        EP_MAXKPA           =CheckAndReadIniDataGeneral("System" , "EP_MAXKPA" , 499.0);
        EP_MAXAFB           =CheckAndReadIniDataGeneral("System" , "EP_MAXA"   , 5.013);
        EP_MINMPA           =CheckAndReadIniDataGeneral("System" , "EP_MINMPA"   , 0.001);  //JerryYang 20171023 add PA Min
        EP_MinAFB           =CheckAndReadIniDataGeneral("System" , "EP_MINA_FeedBack", 0.908);
        edMaxKpa->Text      =EP_MAXKPA;
        edMaxMpaFB->Text    =EP_MAXAFB;
        edtMinMpaFB->Text   =EP_MinAFB;
        edMinMpa->Text      =EP_MINMPA;                                         //JerryYang 20171023 add PA Min

        EP_MAXKPA_1032           =CheckAndReadIniDataGeneral("System" , "EP_MAXKPA_1032" , 499.0);
        EP_MAXAFB_1032           =CheckAndReadIniDataGeneral("System" , "EP_MAXA_1032"   , 5.013);
        EP_MINMPA_1032           =CheckAndReadIniDataGeneral("System" , "EP_MINMPA_1032"   , 0.001);  //JerryYang 20171023 add PA Min
        EP_MinAFB_1032           =CheckAndReadIniDataGeneral("System" , "EP_MINA_FeedBack_1032", 0.908);
        edMaxKpa_1032->Text      =EP_MAXKPA_1032;
        edMaxMpaFB_1032->Text    =EP_MAXAFB_1032;
        edtMinMpaFB_1032->Text   =EP_MinAFB_1032;
        edMinMpa_1032->Text      =EP_MINMPA_1032;                               //JerryYang 20171023 add PA Min

        EPDual_MAXKPA=CheckAndReadIniDataGeneral("System" , "EPDual_MAXKPA", 899.0);    //kevin 20200325 add dual force EP
        EPDual_MAXAFB=CheckAndReadIniDataGeneral("System" , "EPDual_MAXAFB", 4.905);
        EPDual_MINMPA=CheckAndReadIniDataGeneral("System" , "EPDual_MINMPA", 0.001);
        EPDual_MinAFB=CheckAndReadIniDataGeneral("System" , "EPDual_MinAFB", 0.968);     //kevin 20200325 add dual force EP

        edMaxKpaDual->Text      =EPDual_MAXKPA;
        edMaxMpaFBDual->Text    =EPDual_MAXAFB;
        edtMinMpaFBDual->Text   =EPDual_MinAFB;
        edMinMpaDual->Text      =EPDual_MINMPA;                                 //JerryYang 20171023 add PA Min
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<15; j++)                                                 //Ifor 20230206 add: 新增四組Contact 補償 7 => 11  //Ifor 20230829 add: 800KG 11 -> 15
        {
            if(i==2)
            {
                if(j>=7)
                {
                    dIndexZOffset[i][j]=120+(6*60)+((j-6)*40);
                }
                else
                {
                    dIndexZOffset[i][j]=120+(j*60);
                }
            }
            else
            {
                Str.sprintf("dIndexZOffset[%d][%d]", i, j);                     //JerryYang 20190910 fix ATC offset存檔問題
                dIndexZOffset[i][j]             =CheckRange(CheckAndReadIniDataGeneral("Test Arm", Str, 0.0), 10.0, 0.0);
                IndexZOffsetEdit[i][j]->Text    =dIndexZOffset[i][j];
            }
        }
    }

    if(CosFunction.bUseDynamicKitDiameter==false || bHasFile==false)            //Steven 20170605 : 可以自定義Kit直徑
        return;

    for(unsigned int i=0; i<SLKClass.size(); i++)
    {
        if(CUSTOMER_CODE==CC_ASE_SG && SLKClass[i]->dDiameter==80)
        {
            if(bHasDiameter==false)
            {
                bHasDiameter=true;
                sGroup.sprintf("Diameter_%0.3fmm", SLKClass[i]->dDiameter);
                tsContactHighOffset->TabVisible=SLKClass[i]->bShow;
            }
            else
            {
                sGroup.sprintf("Diameter_%0.3f_Himm", SLKClass[i]->dDiameter);
            }
        }
        else if(EP_Install==5)
        {
            sGroup.sprintf("Diameter_%smm", SLKClass[i]->sDiameter);
        }
        else
        {
            sGroup.sprintf("Diameter_%0.3fmm", SLKClass[i]->dDiameter);
        }
        SLKClass[i]->dLoadRate                  =CheckRange(CheckAndReadIniData(FileName, sGroup, "LoadRate",      1.0), 0.8, 1.5);
        SLKClass[i]->dLoadRate_NS               =CheckRange(CheckAndReadIniData(FileName, sGroup, "LoadRate_NS",   1.0), 0.8, 1.5);
        SLKClass[i]->dHotOffset                 =CheckRange(CheckAndReadIniData(FileName, sGroup, "HotOffset",     0.0), 0.5, -0.5);
        SLKClass[i]->dContactOffset             =CheckRange(CheckAndReadIniData(FileName, sGroup, "ContactOffset", 0.0), 10.0, -10.0);
        SLKClass[i]->dContactOffset_NS          =CheckRange(CheckAndReadIniData(FileName, sGroup, "ContactOffset_NS", 0.0), 10.0, -10.0); //kevin 20170807 (Steven) add NS offset

        dTemp=SLKClass[i]->dLoadRate*(double)100.0;                             //為了避免浮點數計算錯誤,要先強制為double
        SLKClass[i]->trckbrDiameter->Position   =int(dTemp);
        dTemp=SLKClass[i]->dLoadRate_NS*(double)100.0;
        SLKClass[i]->trckbrDiameter_NS->Position=int(dTemp);

        SLKClass[i]->edtHotOffset->Text         =SLKClass[i]->dHotOffset;
        SLKClass[i]->edtContactOffset->Text     =SLKClass[i]->dContactOffset;
        SLKClass[i]->edtContactOffset_NS->Text  =SLKClass[i]->dContactOffset_NS;//kevin 20170807 (Steven) add NS offset
    }

    if(INSTALL_DOUBLE_EP>0)                                                     //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    {
        for(unsigned int i=0; i<DieForceSLKClass.size(); i++)
        {
            sGroup.sprintf("DieForceDiameter_%0.3fmm", DieForceSLKClass[i]->dDiameter);
            DieForceSLKClass[i]->dLoadRate      =CheckRange(CheckAndReadIniData(FileName, sGroup, "LoadRate",      1.0), 0.8, 1.5);
            DieForceSLKClass[i]->dContactOffset =CheckRange(CheckAndReadIniData(FileName, sGroup, "ContactOffset", 1.0), 10.0, -10.0);

            dTemp=DieForceSLKClass[i]->dLoadRate*(double)100.0;
            DieForceSLKClass[i]->trckbrDieForceDiameter->Position   =int(dTemp);
            DieForceSLKClass[i]->edtDieForceContactOffset->Text     =DieForceSLKClass[i]->dContactOffset;
            //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
            if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
            {
                for(int j=0; j<8; j++)
                {
                    if(j<4)
                        sGroup.sprintf("DieForceDiameter_%0.3fmm_Arm1_%d", DieForceOneByOneSLKClass[i*8+j]->dDiameter, j+1);
                    else
                        sGroup.sprintf("DieForceDiameter_%0.3fmm_Arm2_%d", DieForceOneByOneSLKClass[i*8+j]->dDiameter, j-3);
                    DieForceOneByOneSLKClass[i*8+j]->dLoadRate                  =CheckRange(CheckAndReadIniData(FileName, sGroup, "LoadRate",      1.0), 0.8, 1.5);
                    DieForceOneByOneSLKClass[i*8+j]->dContactOffset             =CheckRange(CheckAndReadIniData(FileName, sGroup, "ContactOffset", 0.0), 10.0, -10.0);

                    dTemp=DieForceOneByOneSLKClass[i*8+j]->dLoadRate*(double)100.0;
                    DieForceOneByOneSLKClass[i*8+j]->trckbrDieForceOneByOneDiameter->Position   =int(dTemp);
                    DieForceOneByOneSLKClass[i*8+j]->edtDieForceOneByOneContactOffset->Text     =DieForceOneByOneSLKClass[i*8+j]->dContactOffset;
                }
            }
            //<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end
        }
    }

    if(bNeedWriteFile==true)                                                    //Ifor 20200904 add:KYEC 缸徑自動轉換 30=>28  60=>58
    {
        bNeedWriteFile=false;
        for(unsigned int i=0; i<SLKClass.size(); i++)
        {
            if(SLKClass[i]->dDiameter==30)
            {
                SLKClass[i]->dDiameter=28;
                SLKClass[i]->sDiameter="28";
            }
            else if(SLKClass[i]->dDiameter==60)
            {
                SLKClass[i]->dDiameter=58;
                SLKClass[i]->sDiameter="58";
            }
            SLKClass[i]->bShow=true;
        }
        edtCurrentType->Text="28,40,58,56";
        WriteIniData(FileName, "SLK Type", "Type", edtCurrentType->Text);
        edtVisible->Text="1,1,1,1";
        WriteIniData(FileName, "SLK Type", "Visible", edtVisible->Text);

        fContact->rgKitDiameter->Items->Clear();
        for(unsigned int i=0; i<fContactForce->SLKClass.size(); i++)
        {
            if(fContactForce->SLKClass[i]->bShow)
            {
                fContact->rgKitDiameter->Items->Add(fContactForce->SLKClass[i]->sDiameter);
            }
        }
        WriteFile();
    }

    //AI(ht9045-v899) 20260504: full port V874.3 SLKIndClass ==MULTI ReadFile (Arm1_1..4 / Arm2_1..4). SPEC-V899-MultiEP-FullPort.
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        for(int i=0; i<slSLKTypeInd->Count; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(j<4)
                    sGroup.sprintf("Diameter_%0.3fmm_Arm1_%d", SLKIndClass[i*8+j]->dDiameter, j+1);
                else
                    sGroup.sprintf("Diameter_%0.3fmm_Arm2_%d", SLKIndClass[i*8+j]->dDiameter, j-3);
                SLKIndClass[i*8+j]->dLoadRate                  =CheckRange(CheckAndReadIniData(FileName, sGroup, "LoadRate",      1.0), 0.8, 1.5);
                SLKIndClass[i*8+j]->dContactOffset             =CheckRange(CheckAndReadIniData(FileName, sGroup, "ContactOffset", 0.0), 10.0, -10.0);

                dTemp=SLKIndClass[i*8+j]->dLoadRate*(double)100.0;
                SLKIndClass[i*8+j]->trckbrDiameterInd->Position   =int(dTemp);
                SLKIndClass[i*8+j]->edtContactOffsetInd->Text     =SLKIndClass[i*8+j]->dContactOffset;
            }
        }
    }
    else
    {
    for(int i=0; i<slSLKTypeInd->Count; i++)
    {
        for(int j=0; j<16; j++)
        {
            sGroup.sprintf("Diameter_%0.3fmm_%d", SLKIndClass[i*16+j]->dDiameter, j);
            SLKIndClass[i*16+j]->dLoadRate                  =CheckRange(CheckAndReadIniData(FileName, sGroup, "LoadRate",      1.0), 0.8, 1.5);
            SLKIndClass[i*16+j]->dContactOffset             =CheckRange(CheckAndReadIniData(FileName, sGroup, "ContactOffset", 0.0), 10.0, -10.0);

            dTemp=SLKIndClass[i*16+j]->dLoadRate*(double)100.0;
            SLKIndClass[i*16+j]->trckbrDiameterInd->Position   =int(dTemp);
            SLKIndClass[i*16+j]->edtContactOffsetInd->Text     =SLKIndClass[i*16+j]->dContactOffset;
        }
    }
    }
}
//---------------------------------------------------------------------------
void TfContactForce::WriteFile()
{
    AnsiString sGroup;
    double dTemp;
    bool bHasDiameter=false;
    AnsiString Str;

    //jou 20171117 (Steven) : EP校正欄位(從Contact頁面可以進入此setting)
    if(CosFunction.bUseDynamicKitDiameter==true)                                //Steven 20170605 : 可以自定義Kit直徑
    {
        if(bHasFile==false)                                                     //把舊的資料讀取出來
        {
            SLKClass[0]->trckbrDiameter->Position=LastSet.dIndexLoadRate[0][3]*100;
            SLKClass[1]->trckbrDiameter->Position=LastSet.dIndexLoadRate[0][2]*100;
            SLKClass[2]->trckbrDiameter->Position=LastSet.dIndexLoadRate[0][0]*100;
            SLKClass[3]->trckbrDiameter->Position=LastSet.dIndexLoadRate[0][1]*100;

            SLKClass[0]->trckbrDiameter_NS->Position=LastSet.dIndexLoadRate[1][3]*100;
            SLKClass[1]->trckbrDiameter_NS->Position=LastSet.dIndexLoadRate[1][2]*100;
            SLKClass[2]->trckbrDiameter_NS->Position=LastSet.dIndexLoadRate[1][0]*100;
            SLKClass[3]->trckbrDiameter_NS->Position=LastSet.dIndexLoadRate[1][1]*100;

            SLKClass[0]->edtHotOffset->Text=LastSet.dIndexLoadRate[2][3];
            SLKClass[1]->edtHotOffset->Text=LastSet.dIndexLoadRate[2][2];
            SLKClass[2]->edtHotOffset->Text=LastSet.dIndexLoadRate[2][0];
            SLKClass[3]->edtHotOffset->Text=LastSet.dIndexLoadRate[2][1];
        }
        else if(EP_Install==5)
        {
            dTemp=double(SLKClass[0]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[0][3]=dTemp;
            dTemp=double(SLKClass[2]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[0][2]=dTemp;
            dTemp=double(SLKClass[4]->trckbrDiameter->Position)/100.0;          //Ifor 20200408 : [1] --> [2]
            LastSet.dIndexLoadRate[0][0]=dTemp;
            dTemp=double(SLKClass[6]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[0][1]=dTemp;

            dTemp=double(SLKClass[1]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[1][3]=dTemp;
            dTemp=double(SLKClass[3]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[1][2]=dTemp;
            dTemp=double(SLKClass[5]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[1][0]=dTemp;
            dTemp=double(SLKClass[7]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[1][1]=dTemp;

            LastSet.dIndexLoadRate[2][3]=atof(SLKClass[0]->edtHotOffset->Text.c_str());
            LastSet.dIndexLoadRate[2][2]=atof(SLKClass[1]->edtHotOffset->Text.c_str());
            LastSet.dIndexLoadRate[2][0]=atof(SLKClass[2]->edtHotOffset->Text.c_str());
            LastSet.dIndexLoadRate[2][1]=atof(SLKClass[3]->edtHotOffset->Text.c_str());
        }
        else
        {
            dTemp=double(SLKClass[0]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[0][3]=dTemp;
            dTemp=double(SLKClass[1]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[0][2]=dTemp;
            dTemp=double(SLKClass[2]->trckbrDiameter->Position)/100.0;          //Ifor 20200408 : [1] --> [2]
            LastSet.dIndexLoadRate[0][0]=dTemp;
            dTemp=double(SLKClass[3]->trckbrDiameter->Position)/100.0;
            LastSet.dIndexLoadRate[0][1]=dTemp;

            dTemp=double(SLKClass[0]->trckbrDiameter_NS->Position)/100.0;
            LastSet.dIndexLoadRate[1][3]=dTemp;
            dTemp=double(SLKClass[1]->trckbrDiameter_NS->Position)/100.0;
            LastSet.dIndexLoadRate[1][2]=dTemp;
            dTemp=double(SLKClass[2]->trckbrDiameter_NS->Position)/100.0;
            LastSet.dIndexLoadRate[1][0]=dTemp;
            dTemp=double(SLKClass[3]->trckbrDiameter_NS->Position)/100.0;
            LastSet.dIndexLoadRate[1][1]=dTemp;

            LastSet.dIndexLoadRate[2][3]=atof(SLKClass[0]->edtHotOffset->Text.c_str());
            LastSet.dIndexLoadRate[2][2]=atof(SLKClass[1]->edtHotOffset->Text.c_str());
            LastSet.dIndexLoadRate[2][0]=atof(SLKClass[2]->edtHotOffset->Text.c_str());
            LastSet.dIndexLoadRate[2][1]=atof(SLKClass[3]->edtHotOffset->Text.c_str());
        }

        for(unsigned int i=0; i<SLKClass.size(); i++)
        {
            if(CUSTOMER_CODE==CC_ASE_SG && SLKClass[i]->dDiameter==80)
            {
                if(bHasDiameter==false)
                {
                    bHasDiameter=true;
                    sGroup.sprintf("Diameter_%0.3fmm", SLKClass[i]->dDiameter);
                }
                else
                {
                    sGroup.sprintf("Diameter_%0.3f_Himm", SLKClass[i]->dDiameter);
                }
            }
            else if(EP_Install==5)
            {
                sGroup.sprintf("Diameter_%smm", SLKClass[i]->sDiameter);
            }
            else
            {
                sGroup.sprintf("Diameter_%0.3fmm", SLKClass[i]->dDiameter);
            }
            //為了避免浮點數計算錯誤,要先強制為double
            //==>
            dTemp=double(SLKClass[i]->trckbrDiameter->Position)/100.0;
            WriteIniData(FileName, sGroup, "LoadRate",      dTemp);
            dTemp=double(SLKClass[i]->trckbrDiameter_NS->Position)/100.0;
            WriteIniData(FileName, sGroup, "LoadRate_NS",   dTemp);
            //<==
            //為了避免浮點數計算錯誤,要先強制為double
            WriteIniData(FileName, sGroup, "HotOffset",     SLKClass[i]->edtHotOffset->Text);
            WriteIniData(FileName, sGroup, "ContactOffset", SLKClass[i]->edtContactOffset->Text);
            WriteIniData(FileName, sGroup, "ContactOffset_NS", SLKClass[i]->edtContactOffset_NS->Text);//kevin 20170807 (Steven) Add NS offset
        }
        //AI(ht9045-v899) 20260504: full port V874.3 SLKIndClass ==MULTI WriteFile (Arm1_1..4 / Arm2_1..4). SPEC-V899-MultiEP-FullPort.
        if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
        {
            for(int i=0; i<slSLKTypeInd->Count; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(j<4)
                        sGroup.sprintf("Diameter_%0.3fmm_Arm1_%d", SLKIndClass[i*8+j]->dDiameter, j+1);
                    else
                        sGroup.sprintf("Diameter_%0.3fmm_Arm2_%d", SLKIndClass[i*8+j]->dDiameter, j-3);
                    dTemp=double(SLKIndClass[i*8+j]->trckbrDiameterInd->Position)/100.0;
                    WriteIniData(FileName, sGroup, "LoadRate",      dTemp);
                    WriteIniData(FileName, sGroup, "ContactOffset", SLKIndClass[i*8+j]->edtContactOffsetInd->Text);
                }
            }

            for(int ii=0; ii<slDieForceOneByOneSLKType->Count; ii++)
            {
                for(int j=0; j<8; j++)
                {
                    if(j<4)
                        sGroup.sprintf("DieForceDiameter_%0.3fmm_Arm1_%d", DieForceOneByOneSLKClass[ii*8+j]->dDiameter, j+1);
                    else
                        sGroup.sprintf("DieForceDiameter_%0.3fmm_Arm2_%d", DieForceOneByOneSLKClass[ii*8+j]->dDiameter, j-3);
                    dTemp=double(DieForceOneByOneSLKClass[ii*8+j]->trckbrDieForceOneByOneDiameter->Position)/100.0;
                    WriteIniData(FileName, sGroup, "LoadRate",      dTemp);
                    WriteIniData(FileName, sGroup, "ContactOffset", DieForceOneByOneSLKClass[ii*8+j]->edtDieForceOneByOneContactOffset->Text);
                }
            }
        }
        else
        {
            for(int i=0; i<slSLKTypeInd->Count; i++)
            {
                for(int j=0; j<16; j++)
                {
                    sGroup.sprintf("Diameter_%0.3fmm_%d", SLKIndClass[i*16+j]->dDiameter, j);
                    dTemp=double(SLKIndClass[i*16+j]->trckbrDiameterInd->Position)/100.0;
                    WriteIniData(FileName, sGroup, "LoadRate",      dTemp);
                    WriteIniData(FileName, sGroup, "ContactOffset", SLKIndClass[i*16+j]->edtContactOffsetInd->Text);
                }
            }
        }

        if(bHasFile==false)
        {
            bHasFile=true;
        }
    }

    if(INSTALL_DOUBLE_EP==1 || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)                            //Ifor 20191003 : add Die Force 可以自定義Kit直徑 //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合
    {
        for(unsigned int i=0; i<DieForceSLKClass.size(); i++)
        {
            sGroup.sprintf("DieForceDiameter_%0.3fmm", DieForceSLKClass[i]->dDiameter);
            dTemp=double(DieForceSLKClass[i]->trckbrDieForceDiameter->Position)/100.0;
            WriteIniData(FileName, sGroup, "LoadRate",      dTemp);
            WriteIniData(FileName, sGroup, "ContactOffset", DieForceSLKClass[i]->edtDieForceContactOffset->Text);
        }
    }

    WriteIniDataGeneral("System", "EP_MAXKPA", edMaxKpa->Text);
    WriteIniDataGeneral("System", "EP_MAXA"  , edMaxMpaFB->Text);
    WriteIniDataGeneral("System", "EP_MINMPA", edMinMpa->Text);                 //JerryYang 20171023 (wei) add PA Min
    WriteIniDataGeneral("System" , "EP_MINA_FeedBack", edtMinMpaFB->Text);

    WriteIniDataGeneral("System", "EP_MAXKPA_1032", edMaxKpa_1032->Text);
    WriteIniDataGeneral("System", "EP_MAXA_1032"  , edMaxMpaFB_1032->Text);
    WriteIniDataGeneral("System", "EP_MINMPA_1032", edMinMpa_1032->Text);       //JerryYang 20171023 (wei) add PA Min
    WriteIniDataGeneral("System", "EP_MINA_FeedBack_1032", edtMinMpaFB_1032->Text);

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<15; j++)                                                 //Ifor 20230206 add: 新增四組Contact 補償 7 => 11  //Ifor 20230829 add: 800KG 11 -> 15
        {
            Str.sprintf("dIndexZOffset[%d][%d]", i, j);                         //JerryYang 20190910 fix ATC offset存檔問題
            WriteIniDataGeneral("Test Arm", Str, IndexZOffsetEdit[i][j]->Text); //JerryYang 20171023 (wei) add PA Min
        }
    }
    ReadFile();
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::tb30mm_10kgChange(TObject *Sender)
{
    TTrackBar *Ptr=(TTrackBar *)Sender;
    ADAM_DirectWriteData(Ptr->Position,0);                                      //Ifor 20150709 ：加入設備位置
    ShowValue();
}
//---------------------------------------------------------------------------
void TfContactForce::ShowValue()
{
    lab30mm10kgNum->Caption=tb30mm_10kg->Position;
    lab30mm60kgNum->Caption=tb30mm_60kg->Position;
    lab40mm10kgNum->Caption=tb40mm_10kg->Position;
    lab40mm60kgNum->Caption=tb40mm_60kg->Position;
    lab60mm10kgNum->Caption=tb60mm_10kg->Position;
    lab60mm60kgNum->Caption=tb60mm_60kg->Position;
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edtNewDiameterClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 15.0, 100.0);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edtHotOffsetClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.5, -0.5);
}
//------------------------------------------------------------------------------
void __fastcall THTSLKClass::trckbrDiameter_Change(TObject *Sender)
{
    edtLoadRate->Text=AnsiString(trckbrDiameter->Position/100.0);
}
//------------------------------------------------------------------------------
void __fastcall THTDieForceSLKClass::trckbrDieForceDiameter_Change(TObject *Sender)
{
    edtDieForceLoadRate->Text=AnsiString(trckbrDieForceDiameter->Position/100.0);
}
//---------------------------------------------------------------------------
//==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
//---------------------------------------------------------------------------
void __fastcall THTDieForceOneByOneSLKClass::trckbrDieForceOneByOneDiameter_Change(TObject *Sender)
{
    edtDieForceOneByOneLoadRate->Text=AnsiString(trckbrDieForceOneByOneDiameter->Position/100.0);
}
//<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end
void __fastcall THTSLKClass::trckbrDiameter_NSChange(TObject *Sender)
{
    edtLoadRate_NS->Text=AnsiString(trckbrDiameter_NS->Position/100.0);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edMaxKpaClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 400, 950);   //JerryYang 20171211 (Steven) EP Max KPA 900 -> 900
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edtContactOffsetClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 10.0, -10.0);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edtContactOffset_NSClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 10.0, -10.0); //kevin 20170807 (Steven) add
}
//---------------------------------------------------------------------------
THTSLKIndClass::THTSLKIndClass(AnsiString Dia, int Tag, bool bDefault) : TComponent(Owner)
{
    AnsiString Str1, Str2;
    sDiameter   =Dia;
    iTag        =Tag;
    bShow       =bDefault;
    //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
    int iCount=(Tag%8)+1;
    //<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end

    Str1.sprintf("gbLoadRate_%s_%d", Dia, Tag);
    //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        if(iCount<5)
            Str2.sprintf("Load rate of %s mm Arm1_%d", Dia, iCount);
        else
            Str2.sprintf("Load rate of %s mm Arm2_%d", Dia, iCount-4);
    }
    else
    //<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end
    Str2.sprintf("Load rate of %s mm", Dia);
    gbLoadRateInd                  =new TGroupBox(fContactForce->scrlbxDynamicKitInd);
    gbLoadRateInd->Parent          =fContactForce->scrlbxDynamicKitInd;
    gbLoadRateInd->Name            =Str1;
    gbLoadRateInd->Caption         =Str2;
    gbLoadRateInd->Font->Size      =12;
    gbLoadRateInd->Font->Name      ="MS Sans Serif";
    gbLoadRateInd->Font->Charset   =DEFAULT_CHARSET;
    gbLoadRateInd->Top             =100*Tag;
    gbLoadRateInd->Height          =100;
    gbLoadRateInd->Align           =alTop;

    Str1.sprintf("lblDiameter_%s_%d", Dia, Tag);
    //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        if(iCount<5)
            Str2.sprintf("%s mm 1_%d:", Dia, iCount);
        else
            Str2.sprintf("%s mm 2_%d:", Dia, iCount-4);
    }
    else
    //<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end
    Str2.sprintf("%s mm_%d:", Dia, Tag);
    lblDiameterInd=new TLabel(gbLoadRateInd);
    lblDiameterInd->Parent         =gbLoadRateInd;
    lblDiameterInd->Name           =Str1;
    lblDiameterInd->Top            =31;
    lblDiameterInd->Left           =6;
    lblDiameterInd->Caption        =Str2;

    Str1.sprintf("lblContactOffset_%s_%d", Dia, Tag);
    //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        if(iCount<5)
            Str2.sprintf("%s mm contact offset 1_%d:", Dia, iCount);
        else
            Str2.sprintf("%s mm contact offset 2_%d:", Dia, iCount-4);
    }
    else
    //<== //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 end
    Str2.sprintf("%s mm contact offset%d:", Dia, Tag);
    lblContactOffsetInd=new TLabel(gbLoadRateInd);
    lblContactOffsetInd->Parent    =gbLoadRateInd;
    lblContactOffsetInd->Name      =Str1;
    lblContactOffsetInd->Top       =62;
    lblContactOffsetInd->Left      =464;
    lblContactOffsetInd->Caption   =Str2;

    Str1.sprintf("edtContactOffset_%s_%d", Dia, Tag);
    edtContactOffsetInd=new TEdit(gbLoadRateInd);
    edtContactOffsetInd->Parent        =gbLoadRateInd;
    edtContactOffsetInd->Name          =Str1;
    edtContactOffsetInd->Top           =58;
    edtContactOffsetInd->Left          =676;
    edtContactOffsetInd->Height        =28;
    edtContactOffsetInd->Width         =80;
    edtContactOffsetInd->Text          ="";
    edtContactOffsetInd->OnClick       =fContactForce->edtContactOffsetClick;

    Str1.sprintf("trckbrDiameter_%s_%d", Dia, Tag);
    trckbrDiameterInd=new TTrackBar(gbLoadRateInd);
    trckbrDiameterInd->Parent      =gbLoadRateInd;
    trckbrDiameterInd->Name        =Str1;
    trckbrDiameterInd->Position    =100;
    trckbrDiameterInd->Top         =26;
    trckbrDiameterInd->Left        =108;
    trckbrDiameterInd->Height      =35;
    trckbrDiameterInd->Width       =275;
    trckbrDiameterInd->Min         =80;
    trckbrDiameterInd->Max         =150;
    trckbrDiameterInd->OnChange    =trckbrDiameterInd_Change;

    Str1.sprintf("edtLoadRate%s_%d", Dia, Tag);
    edtLoadRateInd=new TEdit(gbLoadRateInd);
    edtLoadRateInd->Parent         =gbLoadRateInd;
    edtLoadRateInd->Name           =Str1;
    edtLoadRateInd->Top            =26;
    edtLoadRateInd->Left           =380;
    edtLoadRateInd->Height         =28;
    edtLoadRateInd->Width          =65;
    edtLoadRateInd->Text           =AnsiString(trckbrDiameterInd->Position/100.0);
    edtLoadRateInd->Enabled        =false;

    dDiameter=atof(Dia.c_str());

    if(dDiameter<=30.0)
        dMinForce=0.5;
    else if(dDiameter<40.0)
        dMinForce=1;
    else if(dDiameter<50.0)
        dMinForce=2;
    else if(dDiameter<60.0)
        dMinForce=4;
    else
        dMinForce=8;

    dMaxForce=3.14*(dDiameter/100.0)*(dDiameter/100.0)*500;
    gbLoadRateInd->Visible=bShow;
}
//------------------------------------------------------------------------------
void __fastcall THTSLKIndClass::trckbrDiameterInd_Change(TObject *Sender)
{
    edtLoadRateInd->Text=AnsiString(trckbrDiameterInd->Position/100.0);
}
//------------------------------------------------------------------------------
void __fastcall TfContactForce::edD25_60mmMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 3, true, 0.5, -0.5);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edMinMpaClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, -1.0, 10.0);  //JerryYang 20171211 (Steven) EP Min KPA 0 -> -1
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edMaxMpaFBClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.0, 6.0);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::edtLoadRateIndClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 1.15, 0.8);
}
//---------------------------------------------------------------------------
void __fastcall TfContactForce::Button1Click(TObject *Sender)
{
    double dMinVol=1.0, dMidVol=1.0, dMaxVol=1.0;

    dMinVol=edtMinVol->Text.ToDouble();

    dMidVol=edtMidVol->Text.ToDouble();

    dMaxVol=(((dMidVol-dMinVol)/5)*9)+dMinVol;

    edtMaxVol->Text=dMaxVol;
}
//---------------------------------------------------------------------------