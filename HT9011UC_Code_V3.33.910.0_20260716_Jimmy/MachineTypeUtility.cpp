//---------------------------------------------------------------------------
// MachineTypeUtility.cpp
//AI(ht9045-heater-control) 20260618 (RogerYang) : 簿从承祇2type放北家舱砞﹚
//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "MachineType.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
extern std::vector<int> g_vecHeaterTypeIdxForShow;   // defined in HandlerSys.cpp

#if EN_HEATER_SHEET
bool g_bGetStaTable_HeaterInsOpt_Already = false;
int  g_StaTable_HeaterInsOpt[eHeaterType_Count];

AnsiString g_HeaterInsOptStr[eHeaterInsOpt_Count] = {
                                                        "TC401",            //0 = TC401
                                                        "Panasonic KT4H",   //1 = KT4H
                                                        "Omron E5DC",       //2 = E5DC
                                                        "No Heater",        //3 = NoHeater
                                                        "DTK4848",          //4 = DTK4848
                                                    };

// per-channel heater brand table (replaces single global TC401HeaterControl).
// occupy = show a combobox for this channel on the Heater setting page.
// NOTE(RogerYang): index 55..70 occupy values are a draft - confirm per machine.
THeaterInsInfo g_tHeaterInsInfo[eHeaterType_Count]={
    { THeaterInsInfo(true , "HotPlate1",  "HeaterInsOpt_HotPlate1",   DEFAULT_HEATER_INS_OPT) },  //0 HotPlate1
    { THeaterInsInfo(true , "HotPlate2",  "HeaterInsOpt_HotPlate2",   DEFAULT_HEATER_INS_OPT) },  //1 HotPlate2
    { THeaterInsInfo(true , "Shuttle1",   "HeaterInsOpt_Shuttle1",    DEFAULT_HEATER_INS_OPT) },  //2 Shuttle1
    { THeaterInsInfo(true , "Shuttle2",   "HeaterInsOpt_Shuttle2",    DEFAULT_HEATER_INS_OPT) },  //3 Shuttle2
    { THeaterInsInfo(true , "Head1",      "HeaterInsOpt_Head1",       DEFAULT_HEATER_INS_OPT) },  //4 Head1
    { THeaterInsInfo(true , "Head2",      "HeaterInsOpt_Head2",       DEFAULT_HEATER_INS_OPT) },  //5 Head2
    { THeaterInsInfo(true , "Head3",      "HeaterInsOpt_Head3",       DEFAULT_HEATER_INS_OPT) },  //6 Head3
    { THeaterInsInfo(true , "Head4",      "HeaterInsOpt_Head4",       DEFAULT_HEATER_INS_OPT) },  //7 Head4
    { THeaterInsInfo(true , "Socket",     "HeaterInsOpt_Socket",      DEFAULT_HEATER_INS_OPT) },  //8 Socket
    { THeaterInsInfo(true , "Chamber",    "HeaterInsOpt_Chamber",     DEFAULT_HEATER_INS_OPT) },  //9 Chamber
    { THeaterInsInfo(true , "CCD",        "HeaterInsOpt_CCD",         DEFAULT_HEATER_INS_OPT) },  //10 CCD
    { THeaterInsInfo(false, "Aa1",        "HeaterInsOpt_Aa1",         DEFAULT_HEATER_INS_OPT) },  //11 Aa1
    { THeaterInsInfo(false, "Ab1",        "HeaterInsOpt_Ab1",         DEFAULT_HEATER_INS_OPT) },  //12 Ab1
    { THeaterInsInfo(false, "Ac1",        "HeaterInsOpt_Ac1",         DEFAULT_HEATER_INS_OPT) },  //13 Ac1
    { THeaterInsInfo(false, "Ad1",        "HeaterInsOpt_Ad1",         DEFAULT_HEATER_INS_OPT) },  //14 Ad1
    { THeaterInsInfo(false, "Ba1",        "HeaterInsOpt_Ba1",         DEFAULT_HEATER_INS_OPT) },  //15 Ba1
    { THeaterInsInfo(false, "Bb1",        "HeaterInsOpt_Bb1",         DEFAULT_HEATER_INS_OPT) },  //16 Bb1
    { THeaterInsInfo(false, "Bc1",        "HeaterInsOpt_Bc1",         DEFAULT_HEATER_INS_OPT) },  //17 Bc1
    { THeaterInsInfo(false, "Bd1",        "HeaterInsOpt_Bd1",         DEFAULT_HEATER_INS_OPT) },  //18 Bd1
    { THeaterInsInfo(false, "Aa2",        "HeaterInsOpt_Aa2",         DEFAULT_HEATER_INS_OPT) },  //19 Aa2
    { THeaterInsInfo(false, "Ab2",        "HeaterInsOpt_Ab2",         DEFAULT_HEATER_INS_OPT) },  //20 Ab2
    { THeaterInsInfo(false, "Ac2",        "HeaterInsOpt_Ac2",         DEFAULT_HEATER_INS_OPT) },  //21 Ac2
    { THeaterInsInfo(false, "Ad2",        "HeaterInsOpt_Ad2",         DEFAULT_HEATER_INS_OPT) },  //22 Ad2
    { THeaterInsInfo(false, "Ba2",        "HeaterInsOpt_Ba2",         DEFAULT_HEATER_INS_OPT) },  //23 Ba2
    { THeaterInsInfo(false, "Bb2",        "HeaterInsOpt_Bb2",         DEFAULT_HEATER_INS_OPT) },  //24 Bb2
    { THeaterInsInfo(false, "Bc2",        "HeaterInsOpt_Bc2",         DEFAULT_HEATER_INS_OPT) },  //25 Bc2
    { THeaterInsInfo(false, "Bd2",        "HeaterInsOpt_Bd2",         DEFAULT_HEATER_INS_OPT) },  //26 Bd2
    { THeaterInsInfo(true , "HeatGun1",   "HeaterInsOpt_HeatGun1",    DEFAULT_HEATER_INS_OPT) },  //27 HeatGun1
    { THeaterInsInfo(true , "HeatGun2",   "HeaterInsOpt_HeatGun2",    DEFAULT_HEATER_INS_OPT) },  //28 HeatGun2
    { THeaterInsInfo(true , "DUT1",       "HeaterInsOpt_DUT1",        DEFAULT_HEATER_INS_OPT) },  //29 DUT1
    { THeaterInsInfo(true , "DUT2",       "HeaterInsOpt_DUT2",        DEFAULT_HEATER_INS_OPT) },  //30 DUT2
    { THeaterInsInfo(true , "DUT3",       "HeaterInsOpt_DUT3",        DEFAULT_HEATER_INS_OPT) },  //31 DUT3
    { THeaterInsInfo(true , "DUT4",       "HeaterInsOpt_DUT4",        DEFAULT_HEATER_INS_OPT) },  //32 DUT4
    { THeaterInsInfo(false, "Ae1",        "HeaterInsOpt_Ae1",         DEFAULT_HEATER_INS_OPT) },  //33 Ae1
    { THeaterInsInfo(false, "Af1",        "HeaterInsOpt_Af1",         DEFAULT_HEATER_INS_OPT) },  //34 Af1
    { THeaterInsInfo(false, "Ag1",        "HeaterInsOpt_Ag1",         DEFAULT_HEATER_INS_OPT) },  //35 Ag1
    { THeaterInsInfo(false, "Ah1",        "HeaterInsOpt_Ah1",         DEFAULT_HEATER_INS_OPT) },  //36 Ah1
    { THeaterInsInfo(false, "Be1",        "HeaterInsOpt_Be1",         DEFAULT_HEATER_INS_OPT) },  //37 Be1
    { THeaterInsInfo(false, "Bf1",        "HeaterInsOpt_Bf1",         DEFAULT_HEATER_INS_OPT) },  //38 Bf1
    { THeaterInsInfo(false, "Bg1",        "HeaterInsOpt_Bg1",         DEFAULT_HEATER_INS_OPT) },  //39 Bg1
    { THeaterInsInfo(false, "Bh1",        "HeaterInsOpt_Bh1",         DEFAULT_HEATER_INS_OPT) },  //40 Bh1
    { THeaterInsInfo(false, "Ae2",        "HeaterInsOpt_Ae2",         DEFAULT_HEATER_INS_OPT) },  //41 Ae2
    { THeaterInsInfo(false, "Af2",        "HeaterInsOpt_Af2",         DEFAULT_HEATER_INS_OPT) },  //42 Af2
    { THeaterInsInfo(false, "Ag2",        "HeaterInsOpt_Ag2",         DEFAULT_HEATER_INS_OPT) },  //43 Ag2
    { THeaterInsInfo(false, "Ah2",        "HeaterInsOpt_Ah2",         DEFAULT_HEATER_INS_OPT) },  //44 Ah2
    { THeaterInsInfo(false, "Be2",        "HeaterInsOpt_Be2",         DEFAULT_HEATER_INS_OPT) },  //45 Be2
    { THeaterInsInfo(false, "Bf2",        "HeaterInsOpt_Bf2",         DEFAULT_HEATER_INS_OPT) },  //46 Bf2
    { THeaterInsInfo(false, "Bg2",        "HeaterInsOpt_Bg2",         DEFAULT_HEATER_INS_OPT) },  //47 Bg2
    { THeaterInsInfo(false, "Bh2",        "HeaterInsOpt_Bh2",         DEFAULT_HEATER_INS_OPT) },  //48 Bh2
    { THeaterInsInfo(true , "2D",         "HeaterInsOpt_2D",          DEFAULT_HEATER_INS_OPT) },  //49 2D
    { THeaterInsInfo(true , "LB",         "HeaterInsOpt_LB",          DEFAULT_HEATER_INS_OPT) },  //50 LB
    { THeaterInsInfo(true , "IndexESD",   "HeaterInsOpt_IndexESD",    DEFAULT_HEATER_INS_OPT) },  //51 IndexESD
    { THeaterInsInfo(true , "CCD_2",      "HeaterInsOpt_CCD_2",       DEFAULT_HEATER_INS_OPT) },  //52 CCD_2
    { THeaterInsInfo(true , "ATCHotAir1", "HeaterInsOpt_ATCHotAir1",  DEFAULT_HEATER_INS_OPT) },  //53 ATCHotAir1
    { THeaterInsInfo(true , "ATCHotAir2", "HeaterInsOpt_ATCHotAir2",  DEFAULT_HEATER_INS_OPT) },  //54 ATCHotAir2
    { THeaterInsInfo(false , "OutSht1",    "HeaterInsOpt_OutSht1",     DEFAULT_HEATER_INS_OPT) },  //55 OutSht1
    { THeaterInsInfo(false , "OutSht2",    "HeaterInsOpt_OutSht2",     DEFAULT_HEATER_INS_OPT) },  //56 OutSht2
    { THeaterInsInfo(false , "Base1",      "HeaterInsOpt_Base1",       DEFAULT_HEATER_INS_OPT) },  //57 Base1
    { THeaterInsInfo(false , "Base2",      "HeaterInsOpt_Base2",       DEFAULT_HEATER_INS_OPT) },  //58 Base2
    { THeaterInsInfo(false , "Base3",      "HeaterInsOpt_Base3",       DEFAULT_HEATER_INS_OPT) },  //59 Base3
    { THeaterInsInfo(false , "Base4",      "HeaterInsOpt_Base4",       DEFAULT_HEATER_INS_OPT) },  //60 Base4
    { THeaterInsInfo(false , "Base5",      "HeaterInsOpt_Base5",       DEFAULT_HEATER_INS_OPT) },  //61 Base5
    { THeaterInsInfo(false , "Base6",      "HeaterInsOpt_Base6",       DEFAULT_HEATER_INS_OPT) },  //62 Base6
    { THeaterInsInfo(false , "HotPlate3",  "HeaterInsOpt_HotPlate3",   DEFAULT_HEATER_INS_OPT) },  //63 HotPlate3
    { THeaterInsInfo(false , "HotPlate4",  "HeaterInsOpt_HotPlate4",   DEFAULT_HEATER_INS_OPT) },  //64 HotPlate4
    { THeaterInsInfo(false , "Shuttle3",   "HeaterInsOpt_Shuttle3",    DEFAULT_HEATER_INS_OPT) },  //65 Shuttle3
    { THeaterInsInfo(false , "Shuttle4",   "HeaterInsOpt_Shuttle4",    DEFAULT_HEATER_INS_OPT) },  //66 Shuttle4
    { THeaterInsInfo(false , "Door1",      "HeaterInsOpt_Door1",       DEFAULT_HEATER_INS_OPT) },  //67 Door1
    { THeaterInsInfo(false , "Door2",      "HeaterInsOpt_Door2",       DEFAULT_HEATER_INS_OPT) },  //68 Door2
    { THeaterInsInfo(false , "LBUp",       "HeaterInsOpt_LBUp",        DEFAULT_HEATER_INS_OPT) },  //69 LBUp
    { THeaterInsInfo(false , "LBDown",     "HeaterInsOpt_LBDown",      DEFAULT_HEATER_INS_OPT) },  //70 LBDown
};
#endif
//---------------------------------------------------------------------------
#if !EN_HEATER_SHEET
extern int TC401HeaterControl;
#endif
//---------------------------------------------------------------------------
bool IsValEqual_HeaterInsOpt(int iHeaterTypeIdx, int iHeaterInsOpt_Cmp)
{
    if(iHeaterInsOpt_Cmp<0 || iHeaterInsOpt_Cmp>=eHeaterInsOpt_Count)
        return false;
#if !EN_HEATER_SHEET
    return (iHeaterInsOpt_Cmp==TC401HeaterControl);
#else
    if(iHeaterTypeIdx<0 || iHeaterTypeIdx>=eHeaterType_Count)
        return false;
    return (iHeaterInsOpt_Cmp==g_tHeaterInsInfo[iHeaterTypeIdx].GetHeaterInsOpt());
#endif
}
//---------------------------------------------------------------------------
// machine-level NoHeater: always judged by global rgHeaterType(=TC401HeaterControl),
// independent of EN_HEATER_SHEET / per-channel g_tHeaterInsInfo table.
bool IsNoHeaterMachine()                                                        //RogerYang 20260630 : Add NoHeater 耞
{
    return (TC401HeaterControl==NoHeater);
}
//---------------------------------------------------------------------------
bool IsAllValEqual_HeaterInsOpt(int iHeaterInsOpt_Cmp, bool bJustForShowItem, int *piHeaterInsOpt_AllSame)
{
    if(iHeaterInsOpt_Cmp<0 || iHeaterInsOpt_Cmp>=eHeaterInsOpt_Count)
        return false;
#if !EN_HEATER_SHEET
    return (iHeaterInsOpt_Cmp==TC401HeaterControl);
#else
    bool bIsAllSame = true;
    int  iHeaterInsOpt_Tmp = INVALID_INT_VAL_NEG;
    for(int i = 0; i<eHeaterType_Count; ++i)
    {
        if(bJustForShowItem && !g_tHeaterInsInfo[i].GetOccupy())
            continue;
        if(INVALID_INT_VAL_NEG == iHeaterInsOpt_Tmp)
            iHeaterInsOpt_Tmp = g_tHeaterInsInfo[i].GetHeaterInsOpt();
        if(iHeaterInsOpt_Tmp != g_tHeaterInsInfo[i].GetHeaterInsOpt())
        {
            bIsAllSame = false;
            break;
        }
    }
    if(bIsAllSame)
    {
        if(NULL != piHeaterInsOpt_AllSame)
            *piHeaterInsOpt_AllSame = iHeaterInsOpt_Tmp;
    }
    return (bIsAllSame && iHeaterInsOpt_Cmp == iHeaterInsOpt_Tmp);
#endif
}
//---------------------------------------------------------------------------
int GetFirstHeaterInsOpt(bool bJustForShowItem)
{
#if !EN_HEATER_SHEET
    return TC401HeaterControl;
#else
    int iTypeIdx = -1;
    if(!bJustForShowItem)
        iTypeIdx = 0;
    else
    {
        if(g_vecHeaterTypeIdxForShow.size()>0)
            iTypeIdx = g_vecHeaterTypeIdxForShow[0];
        else
            iTypeIdx = -1;
    }
    if(0<=iTypeIdx && iTypeIdx<eHeaterType_Count)
        return g_tHeaterInsInfo[iTypeIdx].GetHeaterInsOpt();
    else
        return -1;
#endif
}
//---------------------------------------------------------------------------
bool IsAllSame_HeaterInsOpt(bool bJustForShowItem, int *piFirstHeaterInsOpt)
{
#if !EN_HEATER_SHEET
    if(NULL != piFirstHeaterInsOpt)
        *piFirstHeaterInsOpt = TC401HeaterControl;
    return true;
#else
    int iInsOpt = GetFirstHeaterInsOpt(bJustForShowItem);
    if(NULL != piFirstHeaterInsOpt)
        *piFirstHeaterInsOpt = iInsOpt;
    if(0<=iInsOpt && iInsOpt<eHeaterInsOpt_Count)
        return IsAllValEqual_HeaterInsOpt(iInsOpt, bJustForShowItem);
    else
        return false;
#endif
}
//---------------------------------------------------------------------------
bool GetStaTable_HeaterInsOpt(int *pStaTable, bool bJustForShowItem)
{
    if(NULL==pStaTable)
        return false;
    int  iHeaterInsOpt = 0;
    bool bStaAllEle = true;
    memset(pStaTable, 0, sizeof(int)*eHeaterType_Count);
#if !EN_HEATER_SHEET
    iHeaterInsOpt = TC401HeaterControl;
    if(iHeaterInsOpt<0 || iHeaterInsOpt>=eHeaterInsOpt_Count)
        bStaAllEle = false;
    else
        pStaTable[iHeaterInsOpt] = eHeaterType_Count;
#else
    for(int i = 0; i<eHeaterType_Count; ++i)
    {
        if(bJustForShowItem && !g_tHeaterInsInfo[i].GetOccupy())
            continue;
        iHeaterInsOpt = g_tHeaterInsInfo[i].GetHeaterInsOpt();
        if(iHeaterInsOpt<0 || iHeaterInsOpt>=eHeaterInsOpt_Count)
        {
            bStaAllEle = false;
            continue;
        }
        pStaTable[iHeaterInsOpt]++;
    }
#endif
    return bStaAllEle;
}
#if EN_HEATER_SHEET
//---------------------------------------------------------------------------
bool IsExistVal_HeaterInsOpt(int iHeaterInsOpt_Cmp, bool bJustForShowItem)
{
    if(iHeaterInsOpt_Cmp<0 || iHeaterInsOpt_Cmp>=eHeaterInsOpt_Count)
        return false;
    if(!g_bGetStaTable_HeaterInsOpt_Already)
    {
        GetStaTable_HeaterInsOpt(g_StaTable_HeaterInsOpt, bJustForShowItem);  //fix: arg order (int*, bool)
        g_bGetStaTable_HeaterInsOpt_Already = true;
    }
    return (g_StaTable_HeaterInsOpt[iHeaterInsOpt_Cmp]>0);
}
//---------------------------------------------------------------------------
bool GetCtrlItemVisProp(int iHeaterTypeIdx)
{
    if(iHeaterTypeIdx<0 || iHeaterTypeIdx>=eHeaterType_Count)
        return false;
    if(!g_tHeaterInsInfo[iHeaterTypeIdx].GetOccupy())
        return false;

    switch(iHeaterTypeIdx)
    {
        case tcHotPlate1:   //{   return (HotPlateForm.iPlateSelect==1 || HotPlateForm.iPlateSelect==3);  }
        case tcHotPlate2:   //{   return (HotPlateForm.iPlateSelect==2 || HotPlateForm.iPlateSelect==3);  }
        case tcShuttle1:
        case tcShuttle2:        break;
        case tcHead1:
        case tcHead2:
        case tcHead3:
        case tcHead4:
        {
            if(USE_16_HEATER==eht16Heater       ||
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterKT4H   ||
               USE_16_HEATER==eht16HeaterDTME08 ||
               USE_16_HEATER==eht32HeaterDTME08)
                return true;
            else
                return false;
        }
        case tcSocket:      //{   return (iSocketBaseTempCount==eDut1ea); }
        case tcChamber:         break;
        case tcCCD:         {   return (RTC_TemperNumber>=1);   }
        case tcCCD_2:       {   return (RTC_TemperNumber>=2);   }
        case tcHeatGun1:
        case tcHeatGun2:    {   return (INSTALL_HEAT_GUN>0);  }
        case tcDUT1:
        case tcDUT2:
        case tcDUT3:
        case tcDUT4:
        {
            if(iSocketBaseTempCount==eDut2ea && (iHeaterTypeIdx==tcDUT1 || iHeaterTypeIdx==tcDUT2))
                return true;
            else if(iSocketBaseTempCount==eDut4ea && (iHeaterTypeIdx==tcDUT1 || iHeaterTypeIdx==tcDUT2 || iHeaterTypeIdx==tcDUT3 || iHeaterTypeIdx==tcDUT4))
                return true;
            else
                return false;
        }
        case tc2D:          {   return (CCD2_TEMPER>0);         }
        case tcLB:          {   return (LB_TEMP>0);             }
        case tcIndexESD:    {   return (Index_ESDAir>0);        }
        case tcATCHotAir1:
        case tcATCHotAir2:  {   return (INSTALL_ATC_HEAT_GUN>0);}
    }
    return true;   //fix: default visible for occupied channels (also covers 906-only channels)
}
#endif
//---------------------------------------------------------------------------
