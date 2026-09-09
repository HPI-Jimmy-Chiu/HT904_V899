//---------------------------------------------------------------------------
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
