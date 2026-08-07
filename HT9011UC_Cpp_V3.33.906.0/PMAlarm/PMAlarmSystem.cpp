// =============================================================================
//  PMAlarm/PMAlarmSystem.cpp  --  Preventive-Maintenance (PM) alarm/date engine
//
//  Faithful translation of golden PMAlarm/PMAlarmSystem.cpp (536 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (pure-translation completion campaign, group
//    "pmalarm").  Sibling of PMAlarm/uTimeTool.{h,cpp}, SAME wave -- see that
//    unit's own banner; both land together and this file's `uTimeTool
//    utimetool;` member (PMAlarmSystem.h:72) is that unit's class.
//
//  ROLE: see PMAlarmSystem.h's banner for the full class-level role statement.
//  This .cpp is every out-of-line member of PMItemList (14 functions) and
//  PMAlarmSystem (37 functions) -- 51 golden function bodies, nothing else.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim -- all 51 golden function bodies, whole file):
//     -- PMItemList (14) --
//     PMItemList::PMItemList()            golden :14-20
//     PMItemList::~PMItemList()           golden :22-52
//     PMItemList::LoadList                golden :54-85
//     PMItemList::SaveList                golden :87-117
//     PMItemList::ClearList               golden :119-124
//     PMItemList::PushItem                golden :126-129
//     PMItemList::PushLimitP              golden :131-134
//     PMItemList::PushLimitN              golden :136-139
//     PMItemList::AddItem                 golden :141-143 (empty body, verbatim)
//     PMItemList::DeleteItem              golden :145-147 (empty body, verbatim)
//     PMItemList::GetItem                 golden :149-155
//     PMItemList::GetLimitP               golden :157-163
//     PMItemList::GetLimitN               golden :165-171
//     PMItemList::GetCount                golden :173-176
//     -- PMAlarmSystem (37) --
//     PMAlarmSystem::PMAlarmSystem()      golden :180-213
//     LoadPMAlarm                         golden :215-271
//     SavePMAlarm                         golden :273-301
//     SavePMAlarmForGreatekServerUpdate   golden :303-316
//     SetPMDate                           golden :318-326
//     SetPMAlarmDays                      golden :328-339
//     SetPMStopDays                       golden :341-351
//     GetPMDate / GetPMAlarmDays / GetPMStopDays   golden :353-366
//     SetNextPMDate / GetNextPMDate       golden :368-376
//     IsAlarmDate                         golden :378-392
//     IsStopDate                          golden :394-408
//     IsEnable                            golden :410-413
//     GetExecutiveItem/SetExecutiveItem   golden :415-423
//     GetExecutiveOwner/SetExecutiveOwner golden :425-433
//     GetConformItem/SetConformItem       golden :435-443
//     GetConformOwner/SetConformOwner     golden :445-453
//     GetGerneralPMDocVer/SetGerneralPMDocVer       golden :455-463
//     GetTemperaturePMDocVer/SetTemperaturePMDocVer golden :465-473
//     GetESDPMDocVer/SetESDPMDocVer       golden :475-483
//     GetIonFanPMDocVer/SetIonFanPMDocVer golden :485-493
//     GetMachineNo/SetMachineNo           golden :495-503
//     GetSaveFilePath/SetSaveFilePath     golden :505-522
//     GetStartMonth/SetStartMonth         golden :524-533
//   SATISFIED-BY-SUBSTRATE: TStringList/TIniFile/AnsiString/TDateTime/Now (all
//     vclcompat, ambient via vcl_compat.h's `using vclcompat::...;` block);
//     MyDBIProcess(AnsiString,AnsiString) (real 2-arg body,
//     aHotPlateSubstrate.h:924/.cpp:1030 -- the SAME standing substitution for
//     golden cMyDB.h already recorded at EJ1N/TextProcess.cpp's own banner and
//     Public/MyStringList.cpp's DEPENDENCY VERIFICATION block; golden's one
//     call site here, :50, already passes exactly 2 args, so it translates
//     unchanged); sPMList_Month/_Quarter/_Year/_Temperature/_ESD/_IonFan and
//     sPMSetting (real AnsiString globals, common.h:124-138 / common.cpp:148-
//     162, already ported).
//   GATE REGISTER -- exactly ONE gate, a data-stand-in and a UI-stand-in pair,
//   both re-cited at their call sites below:
//    (1) ShowMessage(AnsiString)  golden Dialogs.hpp, called at golden :387 and
//        :403 (both inside `if(bShowMsg==true)` in IsAlarmDate/IsStopDate).
//        The real VCL modal dialog has NO stand-in anywhere in this tree today
//        (grepped the whole port tree for a ShowMessage global before writing
//        this file) -- this is the EXACT same gap as
//        SECSGEM/SecsSvEcRegistration.cpp's own `Gated_ShowMessage` (that
//        file's :35-44) and SECSGEM/uHGemEquipment.cpp's `Gated_ShowMessage`
//        (:3259-3274), and it is handled with the SAME established idiom: a
//        local no-op `Gated_ShowMessage(const AnsiString&)` replaces the call.
//        WHY A NO-OP IS THE FAITHFUL DEFAULT: both call sites are advisory-only
//        -- golden's own control flow does not branch on the dialog's result
//        (there is none; ShowMessage has no return value) and both are
//        immediately followed by `return true;`, unconditionally, whether or
//        not the message ever appears. A no-op changes NOTHING about
//        IsAlarmDate's/IsStopDate's boolean result or dtNEXT_PM_DATE.
//        BEHAVIOUR DELTA, STATED PLAINLY: on a real machine, calling
//        IsAlarmDate(true) once the alarm window is reached pops a "PM 時間到
//        達警告!" modal; calling IsStopDate(true) once the stop window is
//        reached pops "PM 時間到達停止!". In this build both are silently
//        skipped. RETIRE THIS GATE the moment a real ShowMessage (or this
//        tree's own ShowMyMessage wrapper, canary_support.h/mymessbox.h, which
//        is a DIFFERENT golden symbol and not a substitute -- see
//        SECSGEM/uHGemEquipment.cpp:3259-3261's own note on that distinction)
//        is wired for W7-UI.
//
//  ONE MORE ITEM, NOT A GATE (a write with zero readers in this unit, handled
//  by OMISSION, not by a stand-in -- see item below the includes):
//    `DateSeparator='/';` golden :287 and :309.  See the inline comment at
//    each call site; the same reasoning, and the same disposition (omit, do
//    not shim), is already on record at cMyDB.cpp:897-899 and
//    SECSGEM/uHGemEquipment.cpp:3591-3599 for the identical BCB6 idiom.
//
//  GOLDEN QUIRK, PRESERVED VERBATIM: `PMAlarmSystem::SaveGerneralPMAlarmToCSV()`
//  is DECLARED in golden PMAlarmSystem.h:115 but has NO DEFINITION anywhere in
//  golden -- not in this .cpp, not in any other translated or untranslated
//  golden unit (grepped the whole golden tree, .svn pristine copies included,
//  before writing this banner). The only function BY THAT NAME that IS defined
//  in golden is a DIFFERENT class's method, `TfPMAlarmInterFace::
//  SaveGerneralPMAlarmToCSV()` (golden PMAlarm/PMAlarmInterFace.cpp:1066, a VCL
//  form outside this wave's scope) -- and every unqualified call to
//  `SaveGerneralPMAlarmToCSV()` in golden (PMAlarmInterFace.cpp:280/702/1139/
//  1181) resolves to THAT class's own member, not PMAlarmSystem's. No golden
//  call site anywhere reads `<PMAlarmSystem-instance>.SaveGerneralPMAlarmToCSV()`
//  or `->SaveGerneralPMAlarmToCSV()` (grepped for both). A C++ member function
//  that is declared but never odr-used (never called, never has its address
//  taken) needs no definition to link -- exactly BCB6's own situation here.
//  Reproduced verbatim: PMAlarmSystem.h keeps the declaration (byte-identical
//  to golden), and this .cpp defines nothing for it, because golden's own .cpp
//  defines nothing for it either. This is not a stub-that-is-a-defect (rule 5)
//  because there is no golden body of this method to be missing -- inventing
//  one would be adding behaviour golden never had.
//
//  MINIMAL NECESSARY ADAPTATIONS (2 kinds, both flagged inline with //AI at
//  the line, forced by this tree's vclcompat shapes -- no behaviour change):
//   (a) golden :74/:78/:82 `slItemXxx->Append(sItem)` -- BCB6 TStrings::Append.
//       vclcompat::TStringList's equivalent is `Add` (vclcompat/TStringList.h
//       :162; there is no `Append` member at all, confirmed by the
//       -fsyntax-only probe this file was iterated against). Same net effect
//       (push one string onto the end of the list); golden discards Append's
//       return value (it has none, BCB6 Append is a procedure) and this port
//       discards Add's `int` return value the same way, so the substitution
//       is a rename only.
//   (b) golden :383/:399 `dtNEXT_PM_DATE - _ALARM_DAYS` / `dtNEXT_PM_DATE + 1`
//       -- BCB6 TDateTime<->int arithmetic. vclcompat::TDateTime supports
//       both an implicit `operator double()` AND an implicit
//       `TDateTime(double)` constructor (vclcompat/TDateTime.h:36/43), so
//       `TDateTime - int` is AMBIGUOUS under g++ (built-in `double - int` via
//       the conversion operator, vs. `TDateTime::operator-(TDateTime)` via
//       the converting constructor -- both equally good). Disambiguated with
//       `TDateTime(<lhs>.Val() - _ALARM_DAYS)` / `TDateTime(<lhs>.Val() + 1)`,
//       the EXACT idiom already on record at cMyDB.cpp:902/1112 and
//       cpublic.cpp:461 for the identical `TDateTime +/- literal` ambiguity
//       ("Now()-1 ambiguous -> Val()-1.0"). Same OLE-serial arithmetic, same
//       numeric result -- the .Val()-then-rewrap is a disambiguation, not a
//       semantic change.
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :3 and :9). No __fastcall / __property / __published
//  anywhere in golden's PMAlarmSystem.cpp -- none reproduced here either.
//  Numeric semantics kept EXACT: SetPMAlarmDays'/SetPMStopDays' `int iD =
//  iDays;` (unsigned-to-signed narrowing, golden :330/:343) and the dead
//  `//20150707 return false;` comment at :334 are both kept verbatim, not
//  "fixed" or dropped.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "PMAlarmSystem.h" ; #include "common.h"
//   ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + portable STL + using namespace std)
#include "PMAlarmSystem.h"          // this unit's own contract (PMItemList + PMAlarmSystem, both defined below)
#include "common.h"                 // sPMList_Month/_Quarter/_Year/_Temperature/_ESD/_IonFan, sPMSetting (common.h:124-138)
#include "aHotPlateSubstrate.h"     // MyDBIProcess (golden cMyDB.h) -- real 2-arg body at aHotPlateSubstrate.cpp:1030.
                                    // !! DO NOT ADD "cMyDB.h" TO THIS TU !! -- its golden 3-arg-with-default
                                    // MyDBIProcess spelling would make the 2-argument call below AMBIGUOUS
                                    // against this 2-arg one (same precedent as Public/MyStringList.cpp's banner).
//---------------------------------------------------------------------------
// GATE (1) -- see banner GATE REGISTER.  golden Dialogs.hpp `void
// ShowMessage(const AnsiString&)`; no stand-in for the real VCL modal exists
// anywhere in this tree.  Same idiom as SECSGEM/SecsSvEcRegistration.cpp's and
// SECSGEM/uHGemEquipment.cpp's own `Gated_ShowMessage` -- kept file-local
// (anonymous namespace) so it cannot collide with either of those.
//---------------------------------------------------------------------------
namespace {
void Gated_ShowMessage(const AnsiString & /*S*/)
{
    // TODO(W7-UI): wire to a real modal ShowMessage dialog.
}
} // namespace
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
        //AI(W906-PT-W2) 20260807: golden :74 `->Append(sItem)` -- see banner
        //  adaptation (a); vclcompat::TStringList's equivalent is Add.
        slItemList->Add(sItem);
        sIndex = AnsiString("LimitP") + AnsiString(i+1);
        sItem = pIniFile->ReadString( "LIMIT_P" , sIndex , "" );
        _LIMIT_P.push_back( sItem );
        slItemLimitP->Add(sItem);
        sIndex = AnsiString("LimitN") + AnsiString(i+1);
        sItem = pIniFile->ReadString( "LIMIT_N" , sIndex , "" );
        _LIMIT_N.push_back( sItem );
        slItemLimitN->Add(sItem);
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
        //AI(W906-PT-W2) 20260807: golden :250-252 `TDateTime(y,m,d)` (BCB6's
        //  date-only 3-arg constructor) -- this port's vclcompat::TDateTime
        //  has no such overload; EncodeDate(y,m,d) is the tree's established
        //  free-function equivalent (same substitution as uTimeTool.cpp's
        //  banner adaptation (b); pinned by tests/test_vclcompat.cpp:329-330).
        dtNEXT_PM_DATE=EncodeDate(atoi(sTempDate.SubString(1,4).c_str()),
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
    //AI(W906-PT-W2) 20260807: golden :287 `DateSeparator='/';` -- a BCB6
    //  SysUtils locale global; vclcompat has no such global (grepped the whole
    //  tree). OMITTED, not shimmed: the very next line's
    //  utimetool.TDatetmeToSavestring() formats with the fixed picture string
    //  "yyyymmddhhnnss" (uTimeTool.cpp), which contains no '/' token for
    //  DateSeparator to ever substitute into -- behaviourally inert here, the
    //  same disposition already on record at cMyDB.cpp:897-899 and
    //  SECSGEM/uHGemEquipment.cpp:3591-3599 for the identical idiom.
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
    //AI(W906-PT-W2) 20260807: golden :309 `DateSeparator='/';` -- same inert
    //  omission as SavePMAlarm's :287 above (see that comment); the following
    //  TDatetmeToSavestring() call uses the same '/'-free picture string.
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

    //AI(W906-PT-W2) 20260807: golden :383 `dtNEXT_PM_DATE - _ALARM_DAYS` --
    //  see banner adaptation (b); TDateTime(<lhs>.Val()-_ALARM_DAYS)
    //  disambiguates the TDateTime<->double dual-conversion overload clash.
    if( dtNow >= TDateTime(dtNEXT_PM_DATE.Val() - _ALARM_DAYS) )
    {
        if(bShowMsg == true)
        {
            //AI(W906-PT-W2) 20260807: GATE (1) (banner GATE REGISTER). golden
            //  :387 `ShowMessage("PM 時間到達警告!");` -- real VCL modal, no
            //  stand-in anywhere in this tree. Gated_ShowMessage is a local
            //  no-op; both call sites are advisory-only (see banner for why
            //  this changes no return value / no state).
            Gated_ShowMessage("PM 時間到達警告!");
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

    //AI(W906-PT-W2) 20260807: golden :399 `dtNEXT_PM_DATE + 1` -- same
    //  disambiguation as IsAlarmDate above (banner adaptation (b)).
    if( dtNow >= TDateTime(dtNEXT_PM_DATE.Val() + 1) ) //Sam 20190925 Modify PM StopDate
    {
        if(bShowMsg == true)
        {
            //AI(W906-PT-W2) 20260807: GATE (1), same as IsAlarmDate above.
            //  golden :403 `ShowMessage("PM 時間到達停止!");`.
            Gated_ShowMessage("PM 時間到達停止!");
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

//---------------------------------------------------------------------------
void PMAlarmSystem::SetStartMonth( AnsiString sData )
{
    _StartMonth = sData;
}
//-------------------------------------------------------------------------
