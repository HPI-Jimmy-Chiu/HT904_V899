// =============================================================================
//  PMAlarm/PMAlarmSystem.h  --  Preventive-Maintenance (PM) alarm/date engine
//
//  Faithful translation of golden PMAlarm/PMAlarmSystem.h (153 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (pure-translation completion campaign, group
//    "pmalarm").  Sibling of PMAlarm/uTimeTool.{h,cpp}, SAME wave -- this
//    header's `uTimeTool utimetool;` member (golden :72) is that unit.
//
//  ROLE: two value classes, no state machine, no free functions:
//   * PMItemList -- a std::vector<AnsiString>-backed list of PM checklist items
//     (plus optional +/- limit-value side lists) with an INI load/save pair.
//     6 named PMItemList members hold the 3 calendar-cadence lists
//     (Month/Quarter/Year) and the 3 domain lists (Temperature/ESD/IonFan);
//     5 more (PMItem_List/_Check/_ListTemperature/_ListESD/_CheckESD/
//     _ListIonFan/_CheckIonFan -- golden :76-91) exist as PLAIN DATA MEMBERS
//     that PMAlarmSystem's own methods never populate (LoadPMAlarm only
//     ClearLists them, golden :227-235) -- kept verbatim, not pruned.
//   * PMAlarmSystem -- the PM due-date engine: PM_DATE (day-of-month, 1..30),
//     ALARM_DAYS / STOP_DAYS windows around it, dtNEXT_PM_DATE (the actual next
//     PM due TDateTime), IsAlarmDate()/IsStopDate() date-window predicates, and
//     ~20 plain Get/Set accessor pairs for free-text PM record fields
//     (ExecutiveItem/Owner, ConformItem/Owner, per-domain doc-version strings,
//     MachineNo, SaveFilePath, StartMonth).
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim: see this unit's .cpp banner for
//  the full function-by-function accounting (both files are translated in the
//  same wave and share one dependency-verification pass); every declaration in
//  THIS header maps 1:1 to a real, non-gated body in the .cpp.
//
//  VCL/Borland conversions: no __fastcall / __property / __published anywhere
//  in golden's PMAlarmSystem.h -- none reproduced here either.  `TStringList
//  *slItemList` etc. (golden :37-39) stay raw owning pointers exactly as
//  golden wrote them (manually new'd in the ctor, manually deleted in the
//  dtor -- see .cpp); this is BCB6 idiom, not RAII, and is kept verbatim per
//  the "no redesign" rule.  `typedef struct {...} OFFSET_TEMPERATURE_VALUE;`
//  (golden :42-49) is C-style typedef'd anonymous-struct, kept exactly as
//  golden wrote it (its 5 fields are declared but this wave's PMAlarmSystem.cpp
//  never reads or writes `Temprature_Value` -- data member kept for byte-
//  identical class layout with golden, not exercised by any translated method).
//
//  Big5: the one Chinese comment (golden :59, on dtNEXT_PM_DATE) decoded via
//  cp950 and preserved as UTF-8, in place.  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include <vector> ; #include "uTimeTool.h" ; using namespace std;
// =============================================================================
#ifndef PMAlarmSystemH
#define PMAlarmSystemH
//---------------------------------------------------------------------------
#include <vector>
#include "uTimeTool.h"
using namespace std;
//---------------------------------------------------------------------------
typedef vector< AnsiString > PM_LIST;
typedef vector< bool > CHECK_LIST;
typedef vector< AnsiString > PM_LIMIT_P;
typedef vector< AnsiString > PM_LIMIT_N;

class PMItemList
{
    private:
    protected:
        PM_LIST _LIST;
        PM_LIMIT_P _LIMIT_P;
        PM_LIMIT_N _LIMIT_N;
        unsigned int _MAX_ITEM;
    public:
        PMItemList();
        ~PMItemList();
        void LoadList( AnsiString sPath );
        void SaveList( AnsiString sPath );
        void ClearList();
        void PushItem( AnsiString sItem );
        void PushLimitP( AnsiString sLimitP );
        void PushLimitN( AnsiString sLimitN );
        void AddItem( unsigned int iIndex , AnsiString sItem );
        void DeleteItem( unsigned int iIndex );
        AnsiString GetItem( unsigned int iIndex );
        AnsiString GetLimitP( unsigned int iIndex );
        AnsiString GetLimitN( unsigned int iIndex );
        int GetCount();
        TStringList *slItemList;
        TStringList *slItemLimitP;
        TStringList *slItemLimitN;
};

typedef struct
{
    AnsiString sTempBase;
    AnsiString sTempOffset[22];
    double fTempBase[2];
    double fTempOffsetHigt[22];
    double fTempOffsetLow[22];
}OFFSET_TEMPERATURE_VALUE;

class PMAlarmSystem
{
    private:
    protected:
        bool _ENABLE;
        int _PM_DATE;
        int _ALARM_DAYS;
        int _STOP_DAYS;
        TDateTime  dtNEXT_PM_DATE;  //JimmyChiu 20220118 更新PM時間讀取方式

        AnsiString _ExecutiveItem;
        AnsiString _ExecutiveOwner;
        AnsiString _ConformItem;
        AnsiString _ConformOwner;
        AnsiString _GerneralPMDocVer;
        AnsiString _TemperaturePMDocVer;
        AnsiString _ESDPMDocVer;
        AnsiString _IonFanPMDocVer;
        AnsiString _MachineNo;
        AnsiString _SaveFilePath;
        AnsiString _StartMonth;
        uTimeTool utimetool;
    public:
        PMAlarmSystem();

        PMItemList PMItem_Month;
        PMItemList PMItem_Quarter;
        PMItemList PMItem_Year;

        PMItemList PMItem_Temperature;
        PMItemList PMItem_ESD;
        PMItemList PMItem_IonFan;

        PMItemList PMItem_List;
        PMItemList PMItem_Check;

        PMItemList PMItem_ListTemperature;
        PMItemList PMItem_ListESD;
        PMItemList PMItem_CheckESD;
        PMItemList PMItem_ListIonFan;
        PMItemList PMItem_CheckIonFan;

        OFFSET_TEMPERATURE_VALUE Temprature_Value;

        void LoadPMAlarm();
        void SavePMAlarm();
        void SavePMAlarmForGreatekServerUpdate();

        void SetPMDate( unsigned int iDate );
        bool SetPMAlarmDays( unsigned int iDays );
        void SetPMStopDays( unsigned int iDays );

        int GetPMDate();
        int GetPMAlarmDays();
        int GetPMStopDays();

        void SetNextPMDate( TDateTime dtNextDate );
        TDateTime GetNextPMDate();

        bool IsAlarmDate(bool bShowMsg = false);
        bool IsStopDate(bool bShowMsg = false);

        bool IsEnable();

        void SaveGerneralPMAlarmToCSV();

        AnsiString GetExecutiveItem();
        void SetExecutiveItem( AnsiString sData );

        AnsiString GetExecutiveOwner();
        void SetExecutiveOwner( AnsiString sData );

        AnsiString GetConformItem();
        void SetConformItem( AnsiString sData );

        AnsiString GetConformOwner();
        void SetConformOwner( AnsiString sData );

        AnsiString GetGerneralPMDocVer();
        void SetGerneralPMDocVer( AnsiString sData );

        AnsiString GetTemperaturePMDocVer();
        void SetTemperaturePMDocVer( AnsiString sData );

        AnsiString GetESDPMDocVer();
        void SetESDPMDocVer( AnsiString sData );

        AnsiString GetIonFanPMDocVer();
        void SetIonFanPMDocVer( AnsiString sData );

        AnsiString GetMachineNo();
        void SetMachineNo( AnsiString sData );

        AnsiString GetSaveFilePath();
        void SetSaveFilePath( AnsiString sData );

        AnsiString GetStartMonth();
        void SetStartMonth( AnsiString sData );
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
