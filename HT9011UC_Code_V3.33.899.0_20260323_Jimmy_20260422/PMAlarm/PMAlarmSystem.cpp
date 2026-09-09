//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop
#include "PMAlarmSystem.h"
#include "common.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// PM Item Lister Class
//---------------------------------------------------------------------------
PMItemList::PMItemList()
{
    slItemList = new TStringList;
    slItemLimitP = new TStringList;
    slItemLimitN = new TStringList;
    ClearList();
}
//---------------------------------------------------------------------------
PMItemList::~PMItemList()
{
    try
    {
        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        //==>
        if(slItemList!=NULL)
        {
            slItemList->Clear();
            delete slItemList;
        }
        if(slItemLimitP!=NULL)
        {
            slItemLimitP->Clear();
            delete slItemLimitP;
        }
        if(slItemLimitN!=NULL)
        {
            slItemLimitN->Clear();
            delete slItemLimitN;
        }
        //<==
        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~PMItemList");
    }
}
//---------------------------------------------------------------------------
void PMItemList::LoadList( AnsiString sPath )
{
    TIniFile *pIniFile = new TIniFile( sPath );

    int iMaxItem = pIniFile->ReadInteger( "INFO" , "MaxItem" , 0 );

    ClearList();

    slItemList->Clear();
    slItemLimitP->Clear();
    slItemLimitN->Clear();

    AnsiString sIndex("");
    AnsiString sItem("");

    for( int i = 0 ; i < iMaxItem ; i++ )
    {
        sIndex = AnsiString("Item") + AnsiString(i+1);
        sItem = pIniFile->ReadString( "LIST" , sIndex , "" );
        _LIST.push_back( sItem );
        slItemList->Append(sItem);
        sIndex = AnsiString("LimitP") + AnsiString(i+1);
        sItem = pIniFile->ReadString( "LIMIT_P" , sIndex , "" );
        _LIMIT_P.push_back( sItem );
        slItemLimitP->Append(sItem);
        sIndex = AnsiString("LimitN") + AnsiString(i+1);
        sItem = pIniFile->ReadString( "LIMIT_N" , sIndex , "" );
        _LIMIT_N.push_back( sItem );
        slItemLimitN->Append(sItem);

    }
    delete pIniFile;
}
//---------------------------------------------------------------------------
void PMItemList::SaveList( AnsiString sPath )
{
    TIniFile *pIniFile = new TIniFile( sPath );

    int iMaxItem = _LIST.size();

    pIniFile->WriteInteger( "INFO" , "MaxItem" , iMaxItem );

    AnsiString sIndex("");
    AnsiString sItem("");

    for( int i = 0 ; i < iMaxItem ; i++ )
    {

        sIndex = AnsiString("Item") + AnsiString(i+1);
        sItem = _LIST[i];
        pIniFile->WriteString( "LIST" , sIndex , sItem );

        if(sPath==sPMList_ESD || sPath==sPMList_IonFan)
        {
            sIndex = AnsiString("LimitP") + AnsiString(i+1);
            sItem = _LIMIT_P[i];
            pIniFile->WriteString( "LIMIT_P" , sIndex , sItem );

            sIndex = AnsiString("LimitN") + AnsiString(i+1);
            sItem = _LIMIT_N[i];
            pIniFile->WriteString( "LIMIT_N" , sIndex , sItem );
        }
    }

    delete pIniFile;
}
//---------------------------------------------------------------------------
void PMItemList::ClearList()
{
    _LIST.clear();
    _LIMIT_P.clear();
    _LIMIT_N.clear();
}
//---------------------------------------------------------------------------
void PMItemList::PushItem( AnsiString sItem )
{
    _LIST.push_back( sItem );
}
//---------------------------------------------------------------------------
void PMItemList::PushLimitP( AnsiString sLimitP )
{
    _LIMIT_P.push_back( sLimitP );
}
//---------------------------------------------------------------------------
void PMItemList::PushLimitN( AnsiString sLimitN )
{
    _LIMIT_N.push_back( sLimitN );
}
//---------------------------------------------------------------------------
void PMItemList::AddItem( unsigned int iIndex , AnsiString sItem )
{

}
//---------------------------------------------------------------------------
void PMItemList::DeleteItem( unsigned int iIndex )
{
}
//---------------------------------------------------------------------------
AnsiString PMItemList::GetItem( unsigned int iIndex )
{
    if( iIndex >= _LIST.size() )
        return "";

    return _LIST[iIndex];
}
//---------------------------------------------------------------------------
AnsiString PMItemList::GetLimitP( unsigned int iIndex )
{
    if( iIndex >= _LIMIT_P.size() )
        return "";

    return _LIMIT_P[iIndex];
}
//---------------------------------------------------------------------------
AnsiString PMItemList::GetLimitN( unsigned int iIndex )
{
    if( iIndex >= _LIMIT_N.size() )
        return "";

    return _LIMIT_N[iIndex];
}
//---------------------------------------------------------------------------
int PMItemList::GetCount()
{
    return _LIST.size();
}
//---------------------------------------------------------------------------
// PM Alarm System Class
//---------------------------------------------------------------------------
PMAlarmSystem::PMAlarmSystem()
{
    _ENABLE         = false;
    _PM_DATE        = 0;
    _ALARM_DAYS     = 0;
    _STOP_DAYS      = 0;

    _ExecutiveItem = "";
    _ExecutiveOwner = "";
    _ConformItem = "";
    _ConformOwner = "";
    _GerneralPMDocVer = "";
    _TemperaturePMDocVer = "";
    _ESDPMDocVer = "";
    _IonFanPMDocVer = "";
    _MachineNo = "";
    _SaveFilePath = "";
    _StartMonth = "";

    PMItem_Month.ClearList();
    PMItem_Quarter.ClearList();
    PMItem_Year.ClearList();

    PMItem_Temperature.ClearList();
    PMItem_ESD.ClearList();
    PMItem_IonFan.ClearList();

//    AnsiString sPMAlarmDir = "D:\\HT9045\\newtesthander\\system\\PMAlarm";
//
//    if( DirectoryExists( sPMAlarmDir ) == false )
//    {
//        CreateDir( sPMAlarmDir );
//    }
}
//---------------------------------------------------------------------------
void PMAlarmSystem::LoadPMAlarm()
{
    AnsiString sTempDate="";
    IsAlarmDate(); // test
    PMItem_Month.LoadList(      sPMList_Month   );
    PMItem_Quarter.LoadList(    sPMList_Quarter );
    PMItem_Year.LoadList(       sPMList_Year    );
    PMItem_Temperature.LoadList(       sPMList_Temperature    );
    PMItem_ESD.LoadList(       sPMList_ESD    );
    PMItem_IonFan.LoadList(       sPMList_IonFan    );
    //PMItem_List.LoadList(       sPMItem_List    );
    //PMItem_Check.LoadList(      sPMItem_Check   );
    PMItem_List.ClearList();
    PMItem_Check.ClearList();

    PMItem_ListTemperature.ClearList();
    //PMItem_CheckTemperature.ClearList();
    PMItem_ListESD.ClearList();
    PMItem_CheckESD.ClearList();
    PMItem_ListIonFan.ClearList();
    PMItem_CheckIonFan.ClearList();

    TIniFile *pIniFile = new TIniFile( sPMSetting );

    _PM_DATE    = pIniFile->ReadInteger( "Setting" , "PM_DATE"       ,  0  );
    _ALARM_DAYS = pIniFile->ReadInteger( "Setting" , "ALARM_DAYS"    ,  0  );
    _STOP_DAYS  = pIniFile->ReadInteger( "Setting" , "STOP_DAYS"     ,  0  );

    sTempDate=pIniFile->ReadString("Setting", "Next_PM_Date", "2021/01/01");    //Jimmychiu 20230407 : 改變時間存讀格式
    if(utimetool.CheckSavestringLength(sTempDate))
    {
        dtNEXT_PM_DATE=utimetool.SavestringToTDatetme(sTempDate);
    }
    else
    {
        dtNEXT_PM_DATE=TDateTime(atoi(sTempDate.SubString(1,4).c_str()),
                                 atoi(sTempDate.SubString(6,2).c_str()),
                                 atoi(sTempDate.SubString(9,2).c_str()));
    }
    if( _PM_DATE <= 0 )
    {
        _ENABLE = false;
    }
    _ExecutiveItem = pIniFile->ReadString( "Setting" , "ExecutiveItem" , "" );
    _ExecutiveOwner = pIniFile->ReadString( "Setting" , "ExecutiveOwner" , "" );
    _ConformItem = pIniFile->ReadString( "Setting" , "ConformItem" , "" );
    _ConformOwner = pIniFile->ReadString( "Setting" , "ConformOwner" , "" );
    _GerneralPMDocVer = pIniFile->ReadString( "Setting" , "GerneralPMDocVer" , "" );
    _TemperaturePMDocVer = pIniFile->ReadString( "Setting" , "TemperaturePMDocVer" , "" );
    _ESDPMDocVer = pIniFile->ReadString( "Setting" , "ESDPMDocVer" , "" );
    _IonFanPMDocVer = pIniFile->ReadString( "Setting" , "IonFanPMDocVer" , "" );
    _MachineNo = pIniFile->ReadString( "Setting" , "MachineNo" , "" );
    _SaveFilePath = pIniFile->ReadString( "Setting" , "SaveFilePath" , "" );
    _StartMonth = pIniFile->ReadString( "Setting" , "StartMonth" , "" );
    delete pIniFile;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SavePMAlarm()
{
    PMItem_Month.SaveList(      sPMList_Month   );
    PMItem_Quarter.SaveList(    sPMList_Quarter );
    PMItem_Year.SaveList(       sPMList_Year    );
    PMItem_Temperature.SaveList(       sPMList_Temperature    );
    PMItem_ESD.SaveList(       sPMList_ESD    );
    PMItem_IonFan.SaveList(       sPMList_IonFan    );

    TIniFile *pIniFile = new TIniFile( sPMSetting );

    pIniFile->WriteInteger( "Setting" , "PM_DATE"       , _PM_DATE      );
    pIniFile->WriteInteger( "Setting" , "ALARM_DAYS"    , _ALARM_DAYS   );
    pIniFile->WriteInteger( "Setting" , "STOP_DAYS"     , _STOP_DAYS    );
    DateSeparator='/';  // Sam 20210225 : 增加保護
    pIniFile->WriteString( "Setting" , "Next_PM_Date" , utimetool.TDatetmeToSavestring(dtNEXT_PM_DATE));
    pIniFile->WriteString( "Setting" , "ExecutiveItem" , _ExecutiveItem );
    pIniFile->WriteString( "Setting" , "ExecutiveOwner" , _ExecutiveOwner );
    pIniFile->WriteString( "Setting" , "ConformItem" , _ConformItem );
    pIniFile->WriteString( "Setting" , "ConformOwner" , _ConformOwner );
    pIniFile->WriteString( "Setting" , "GerneralPMDocVer" , _GerneralPMDocVer );
    pIniFile->WriteString( "Setting" , "TemperaturePMDocVer" , _TemperaturePMDocVer );
    pIniFile->WriteString( "Setting" , "ESDPMDocVer" , _ESDPMDocVer );
    pIniFile->WriteString( "Setting" , "IonFanPMDocVer" , _IonFanPMDocVer );
    pIniFile->WriteString( "Setting" , "MachineNo" , _MachineNo );
    pIniFile->WriteString( "Setting" , "SaveFilePath" , _SaveFilePath );
    pIniFile->WriteString( "Setting" , "StartMonth" , _StartMonth );
    delete pIniFile;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SavePMAlarmForGreatekServerUpdate()
{
    TIniFile *pIniFile = new TIniFile( sPMSetting );
    pIniFile->WriteInteger("Setting" , "PM_DATE"       , _PM_DATE      );
    pIniFile->WriteInteger("Setting" , "ALARM_DAYS"    , _ALARM_DAYS   );
    pIniFile->WriteInteger("Setting" , "STOP_DAYS"     , _STOP_DAYS    );
    DateSeparator='/';  // Sam 20210225 : 增加保護
    pIniFile->WriteString( "Setting" , "Next_PM_Date" , utimetool.TDatetmeToSavestring(dtNEXT_PM_DATE));
//
    pIniFile->WriteString("Setting" , "MachineNo" , _MachineNo );
    pIniFile->WriteString("Setting" , "SaveFilePath" , _SaveFilePath );
    pIniFile->WriteString("Setting" , "StartMonth" , _StartMonth );
    delete pIniFile;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetPMDate( unsigned int iDate )
{
    if( iDate <= 0 || iDate > 30 )
    {
        return;
    }

    _PM_DATE = iDate;
}
//---------------------------------------------------------------------------
bool PMAlarmSystem::SetPMAlarmDays( unsigned int iDays )
{
    int iD = iDays;

    if( iD < 0 || _PM_DATE - iD <= 0 )
    {
        //20150707 return false;
    }

    _ALARM_DAYS = iDays;
    return true;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetPMStopDays( unsigned int iDays )
{
    int iD = iDays;

    if( iD < 0 || _PM_DATE + iD > 30 )
    {
        return;
    }

    _STOP_DAYS = iDays;
}
//---------------------------------------------------------------------------
int PMAlarmSystem::GetPMDate()
{
    return _PM_DATE;
}
//---------------------------------------------------------------------------
int PMAlarmSystem::GetPMAlarmDays()
{
    return _ALARM_DAYS;
}
//---------------------------------------------------------------------------
int PMAlarmSystem::GetPMStopDays()
{
    return _STOP_DAYS;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetNextPMDate( TDateTime dtNextDate )
{
    dtNEXT_PM_DATE = dtNextDate;
}
//---------------------------------------------------------------------------
TDateTime PMAlarmSystem::GetNextPMDate()
{
    return dtNEXT_PM_DATE;
}
//---------------------------------------------------------------------------
bool PMAlarmSystem::IsAlarmDate(bool bShowMsg)
{
    //20150707 TDateTime dtNow = Now() + _ALARM_DAYS;
    TDateTime dtNow = Now(); //20150707

    if( dtNow >= (dtNEXT_PM_DATE - _ALARM_DAYS) )
    {
        if(bShowMsg == true)
        {
            ShowMessage("PM 時間到達警告!");
        }
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool PMAlarmSystem::IsStopDate(bool bShowMsg)
{
    //20150707 TDateTime dtNow = Now() - _STOP_DAYS;
    TDateTime dtNow = Now(); //20150707

    if( dtNow >= (dtNEXT_PM_DATE + 1) ) //Sam 20190925 Modify PM StopDate
    {
        if(bShowMsg == true)
        {
            ShowMessage("PM 時間到達停止!");
        }
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool PMAlarmSystem::IsEnable()
{
    return _ENABLE;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetExecutiveItem()
{
    return _ExecutiveItem;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetExecutiveItem( AnsiString sData )
{
    _ExecutiveItem = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetExecutiveOwner()
{
    return _ExecutiveOwner;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetExecutiveOwner( AnsiString sData )
{
    _ExecutiveOwner = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetConformItem()
{
    return _ConformItem;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetConformItem( AnsiString sData )
{
    _ConformItem = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetConformOwner()
{
    return _ConformOwner;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetConformOwner( AnsiString sData )
{
    _ConformOwner = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetGerneralPMDocVer()
{
    return _GerneralPMDocVer;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetGerneralPMDocVer( AnsiString sData )
{
    _GerneralPMDocVer = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetTemperaturePMDocVer()
{
    return _TemperaturePMDocVer;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetTemperaturePMDocVer( AnsiString sData )
{
    _TemperaturePMDocVer = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetESDPMDocVer()
{
    return _ESDPMDocVer;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetESDPMDocVer( AnsiString sData )
{
    _ESDPMDocVer = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetIonFanPMDocVer()
{
    return _IonFanPMDocVer;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetIonFanPMDocVer( AnsiString sData )
{
    _IonFanPMDocVer = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetMachineNo()
{
    return _MachineNo;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetMachineNo( AnsiString sData )
{
    _MachineNo = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetSaveFilePath()
{
    if(_SaveFilePath.SubString(_SaveFilePath.Length(),1) != "\\")
    {
        _SaveFilePath = _SaveFilePath + "\\";
    }

    return _SaveFilePath;
}
//---------------------------------------------------------------------------
void PMAlarmSystem::SetSaveFilePath( AnsiString sData )
{
    if(sData.SubString(sData.Length(),1) != "\\")
    {
        sData = sData + "\\";
    }
    _SaveFilePath = sData;
}
//-------------------------------------------------------------------------
AnsiString PMAlarmSystem::GetStartMonth()
{
    return _StartMonth;
}
void PMAlarmSystem::SetStartMonth( AnsiString sData )
{
    _StartMonth = sData;
}
//-------------------------------------------------------------------------

