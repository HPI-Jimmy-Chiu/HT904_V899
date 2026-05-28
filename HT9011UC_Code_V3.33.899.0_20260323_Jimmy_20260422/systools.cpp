#include "MachineDefine.h"
#pragma hdrstop

#include "systools.h"
#include "cmydef.h"
#include "HS_Function.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSysTools *FormSysTools;
//---------------------------------------------------------------------------
__fastcall TFormSysTools::TFormSysTools(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::OpenFormData(AnsiString Filename)
{
    INIFile1=new TIniFile(Filename);
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::CloseFormData()
{
    delete INIFile1;
}
//---------------------------------------------------------------------------
double __fastcall TFormSysTools::CheckAndReadIniData(AnsiString Group, AnsiString Name, double Value)
{
    if(!INIFile1->ValueExists(Group, Name))
        INIFile1->WriteFloat(Group, Name, Value);
    else
        Value=INIFile1->ReadFloat(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
int __fastcall TFormSysTools::CheckAndReadIniData(AnsiString Group, AnsiString Name, int Value)
{
    if(!INIFile1->ValueExists(Group, Name))
        INIFile1->WriteInteger(Group, Name, Value);
    else
        Value=INIFile1->ReadInteger(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
bool __fastcall TFormSysTools::CheckAndReadIniData(AnsiString Group, AnsiString Name, bool Value)
{
    if(!INIFile1->ValueExists(Group, Name))
        INIFile1->WriteBool(Group, Name, Value);
    else
        Value=INIFile1->ReadBool(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormSysTools::CheckAndReadIniData(AnsiString Group, AnsiString Name, AnsiString Value)
{
    if(!INIFile1->ValueExists(Group, Name))
        INIFile1->WriteString(Group, Name, Value);
    else
        Value=INIFile1->ReadString(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
TDateTime __fastcall TFormSysTools::CheckAndReadIniData(AnsiString Group, AnsiString Name, TDateTime Value)
{
    if(!INIFile1->ValueExists(Group, Name))
        INIFile1->WriteDateTime(Group, Name, Value);
    else
        Value=INIFile1->ReadDateTime(Group, Name, Value);
    return Value;
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::WriteIniData( AnsiString Group, AnsiString Name, bool bValue)
{
    AnsiString Str, Str1, Str2;
    bool ret;
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置

    ret=INIFile1->ReadBool(Group, Name, bValue);
    if(ret!=bValue)
    {
        Str1.sprintf("%s_%s Offset change Value", Group , Name);
        Str2.sprintf("%d==>%d", ret, bValue);
        RecordChangeLogProcess(Str1, Str2);                                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }
    INIFile1->WriteBool(Group, Name, bValue);

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true && bHasChange==true)                                  //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::WriteIniData(AnsiString Group, AnsiString Name, int Value)
{
    AnsiString Str, Str1, Str2;
    int  ret;
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置

    ret=INIFile1->ReadInteger(Group, Name, Value);
    if(ret!=Value)
    {
        Str1.sprintf("%s_%s change Value", Group , Name);
        Str2.sprintf("%d==>%d", ret, Value);
        RecordChangeLogProcess(Str1, Str2);                                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }
    INIFile1->WriteInteger(Group, Name, Value);

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::WriteIniData(AnsiString Group, AnsiString Name, unsigned int Value)
{
    AnsiString Str, Str1, Str2;
    unsigned int ret;
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置

    ret=INIFile1->ReadInteger(Group, Name, Value);
    if(ret!=Value)
    {
        Str1.sprintf("%s_%s Offset change Value", Group , Name);
        Str2.sprintf("%d==>%d", ret, Value);
        RecordChangeLogProcess(Str1, Str2);                                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }
    INIFile1->WriteInteger(Group, Name, Value);

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::WriteIniData(AnsiString Group, AnsiString Name, double Value)
{
    AnsiString Str, Str1, Str2;
    double ret;
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置

    Str.sprintf("%0.4f", Value);                                                //Steven 20150723 : double資料存檔前都補成4個0

    ret=INIFile1->ReadFloat(Group, Name, Value);
    if(ret!=Str)
    {
        Str1.sprintf("%s_%s Offset change Value", Group , Name);
        Str2.sprintf("%0.4f==>%0.4f", ret, Value);
        RecordChangeLogProcess(Str1, Str2);                                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }
    INIFile1->WriteString(Group, Name, Str);

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::WriteIniData(AnsiString Group, AnsiString Name, AnsiString Value)
{
    AnsiString Str, Str1, Str2;
    AnsiString ret;
    bool bHasChange=false;                                                      //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
    bool bStrIsFloat1=false;
    bool bStrIsFloat2=false;
    double freg=0;

    ret=INIFile1->ReadString(Group, Name, Value);
    bStrIsFloat1=TryStrToFloat(ret.c_str(), freg);
    bStrIsFloat2=TryStrToFloat(Value.c_str(), freg);
    if((bStrIsFloat1==true && bStrIsFloat2==true && atof(ret.c_str())!=atof(Value.c_str())) ||
       (bStrIsFloat1==false && bStrIsFloat2==false && ret!=Value))
    {
        Str1.sprintf("%s_%s change Value", Group , Name);
        Str2.sprintf("%s==>%s", ret, Value);
        RecordChangeLogProcess(Str1, Str2);                                     //wei 20180625 offset Change log紀錄
        bHasChange=true;
    }
    INIFile1->WriteString(Group, Name, Value);

    if(CosFunction.bUseChangeLogByLot==true &&                                  //Ifor 20191004 : Change Log By Lot 需放在最下面，避免開檔後存錯位置
       bSysLotStart==true &&
       bHasChange==true)                                                        //Ifor 20191002 : add Change Log By Lot
    {
        FormHS->RecordChangeLogByLot(Str1, Str2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSysTools::WriteIniData(AnsiString Group, AnsiString Name, TDateTime Value)
{
    INIFile1->WriteDateTime(Group, Name, Value);
}
//---------------------------------------------------------------------------
bool __fastcall TFormSysTools::CheckSectionExist(AnsiString Group)
{
    return INIFile1->SectionExists(Group);
}
//---------------------------------------------------------------------------
bool __fastcall TFormSysTools::CheckKeyExist(AnsiString Group, AnsiString Name)
{
    return INIFile1->ValueExists(Group, Name);
}
//---------------------------------------------------------------------------
