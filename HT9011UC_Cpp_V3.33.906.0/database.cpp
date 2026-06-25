// ===========================================================================
//  database.cpp  -- PARTIAL translation (W3-cont2: config-table loaders batch)
//  Golden ref: D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\database.cpp
//
//  BCB6 preamble (dropped per migration plan):
//    #include "MachineDefine.h" / #pragma hdrstop / #pragma package(smart_init)
//    / #pragma resource "*.dfm"  -- none carry portable meaning.
//
//  WHAT IS TRANSLATED (active code):
//    SYSTEM_MODULAR::LoadIoData()          (database.cpp:1548-1614)
//    SYSTEM_MODULAR::LoadMotData()         (database.cpp:1616-1682)
//    TIODATA::TIODATA(AnsiString)          (database.cpp:1756-1903)
//    TIOTABLENO::TIOTABLENO()              (database.cpp:1905-1923)
//    TIOTABLENO::SetIOTableNo(AnsiString)  (database.cpp:1925-2045)
//    TMOTNO::TMOTNO()                      (database.cpp:2047-2079)
//    TMOTNO::SetMOTTableNo(AnsiString)     (database.cpp:2081-2298)
//    TMOTDATA::TMOTDATA(AnsiString)        (database.cpp:2300-2874)
//
//  WHAT IS GATED (#if 0 // TODO(wave)):
//    TDataModule1 ctor / DataModule1 global (database.cpp:24, 33-41)
//      -> BDE TTable / TDataModule; deferred to BDE wave.
//    SYSTEM_MODULAR ctor/dtor + SystemModularInitial + InstallColorBinDisplay
//      (database.cpp:43-288, 1539-1546, 1684-1729, 1731-1754)
//      -> calls ReadGeneralIni + SECS (HT9045Gem) + BinDisp; deferred.
//    ReadGeneralIni (database.cpp:301-1537) -> cmydef/cprod full surface; deferred.
//    LogSoftwareOnTime / SoftwareExeTimer globals (database.cpp:28-31, 2876-2917)
//      -> #ifdef DEBUG_SOFTWARE_EXEC_TIME, effectively no-op; stub provided.
//
//  CROSS-HEADER DEPENDENCIES (active in this batch):
//    MachineType.h  -> enum eIOType {eMotionNet=0, eISABase=1, ePCI1735U=2,
//                      ePCI1203=3, ePLCbase=4}  (MachineType.h:717-722)
//    common.h:67-68 -> extern AnsiString IoTablePath; MotTablePath;
//    cmydef.h:2977  -> extern int INDEX_MOTION_CARD;
//    cMyDB.h:20     -> void __fastcall MyDBIProcess(AnsiString,AnsiString,AnsiString="")
//    mymessbox.h:58 -> void ShowMyMessage(AnsiString,AnsiString="",...)
//    vclcompat       -> HexStrToInt(char*) and HexStrToInt(AnsiString) overloads
//
//  FAITHFULNESS NOTES:
//    * The copy-paste bug at database.cpp:1671 (LoadMotData catch block logs
//      "SYSTEM_MODULAR::LoadIoData" instead of "LoadMotData") is PRESERVED.
//    * SetMOTTableNo: the `//    else` comment-out at database.cpp:2091 that
//      causes the name-scan block to run unconditionally (even on Count mismatch)
//      is PRESERVED -- see comment at that location.
//    * Port HEX-parse rule for TIODATA: HexStrToInt(...c_str()) (database.cpp:1828).
//    * Port HEX-parse rule for TMOTDATA MC88X1: HexStrToInt(SL->Strings[...])
//      WITHOUT .c_str() (database.cpp:2419) -- different overload, preserved.
//    * TIODATA::iInType defaults to 0 (not -1) when empty (database.cpp:1845).
//      In the Count<14 fallback path iInType=-1 (database.cpp:1892).
//    * TMOTDATA bHasNullData -> iEnable=0 at end of valid-row path (database.cpp:2835-2838).
// ===========================================================================

#include "vclcompat/vcl_compat.h"   // replaces <vcl.h>; AnsiString/TStringList/FileExists/...
#include "database.h"
#include "MachineType.h"            // enum eIOType (eMotionNet/eISABase/ePCI1735U/ePLCbase)
#include "common.h"                 // extern AnsiString IoTablePath / MotTablePath (common.h:67-68)
#include "cmydef.h"                 // extern int INDEX_MOTION_CARD (cmydef.h:2977)

// ---------------------------------------------------------------------------
//  MyDBIProcess / ShowMyMessage forward declarations
//  cMyDB.h and mymessbox.h pull in VCL headers that are not yet portable.
//  For the loader batch we declare only the signatures we call, matching
//  cMyDB.h:20 and mymessbox.h:58 exactly.
// ---------------------------------------------------------------------------
void __fastcall MyDBIProcess(AnsiString asTable, AnsiString S1, AnsiString S2="");
void ShowMyMessage(AnsiString S1, AnsiString S2="", AnsiString S3="", bool Ok=false, bool bServoOff=false);

// ---------------------------------------------------------------------------
//  Global instances  (database.cpp:27)
// ---------------------------------------------------------------------------
SYSTEM_MODULAR HSys;    // global system parameter aggregate

// ---------------------------------------------------------------------------
//  LogSoftwareOnTime stub  (database.cpp:2876-2917)
//  Original body is entirely inside #ifdef DEBUG_SOFTWARE_EXEC_TIME (off in
//  normal builds).  Provide an empty stub so the loader ctor-chain compiles.
// ---------------------------------------------------------------------------
void LogSoftwareOnTime(AnsiString /*Flag*/)
{
#ifdef DEBUG_SOFTWARE_EXEC_TIME
    // TODO(wave): translate TMyStringList / TQPF_Timer log path when needed.
#endif
}

// ---------------------------------------------------------------------------
//  #if 0: TDataModule1 global + ctor  (database.cpp:24, 33-41)
//  Needs TDataModule / TTable from BDE (<Db.hpp> / <DBTables.hpp>).
// ---------------------------------------------------------------------------
#if 0 // TODO(wave-BDE): TDataModule1 ctor -- database.cpp:33-41
TDataModule1 *DataModule1;

__fastcall TDataModule1::TDataModule1(TComponent* Owner)
    : TDataModule(Owner)
{
    CylinderTable->DatabaseName=ALIAS;
    SwitchTable->DatabaseName=ALIAS;
    MotorTable->DatabaseName=ALIAS;
    SensorTable->DatabaseName=ALIAS;
    SuckerTable->DatabaseName=ALIAS;
}
#endif // TODO(wave-BDE)

// ---------------------------------------------------------------------------
//  #if 0: SYSTEM_MODULAR ctor  (database.cpp:43-288)
//  Calls ReadGeneralIni() + SystemModularInitial() + mapATCAlarmCode seeding.
//  Deferred: ReadGeneralIni needs ~1240-line cmydef/cprod surface; ctor also
//  calls OpenGeneralIniFile / InitialMemory / InitCommonString (common.cpp).
// ---------------------------------------------------------------------------
#if 0 // TODO(wave): SYSTEM_MODULAR ctor -- database.cpp:43-288
_fastcall SYSTEM_MODULAR::SYSTEM_MODULAR()
{
    LogSoftwareOnTime("SYSTEM_MODULAR, Start");
    SysTimer.LatchCycleTime(true);
    // InitialMemory();
    // InitCommonString();
    // OpenGeneralIniFile();
    BinDisCtrl = NULL;
    MyGem      = NULL;
    // ATKRecipeInfo = new ATK_RECIPE_INFO();
    // ReadGeneralIni();
    // SystemModularInitial();
    // AddSpace(asGeneralPath);
    mapATCAlarmCode.clear();
    // ... mapATCAlarmCode["ALM001"] = "WAR15200"; ... (database.cpp:58-288)
}
#endif // TODO(wave)

// ---------------------------------------------------------------------------
//  #if 0: SYSTEM_MODULAR dtor  (database.cpp:1731-1754)
// ---------------------------------------------------------------------------
#if 0 // TODO(wave): SYSTEM_MODULAR dtor -- database.cpp:1731-1754
_fastcall SYSTEM_MODULAR::~SYSTEM_MODULAR()
{
    try
    {
        // delete BinDisCtrl; delete ATKRecipeInfo; CloseGeneralIniFile();
        // ...
    }
    catch(...)
    {
    }
}
#endif // TODO(wave)

// ---------------------------------------------------------------------------
//  #if 0: SYSTEM_MODULAR::SystemModularInitial  (database.cpp:1539-1546)
//  Creates HT9045Gem (SECS) + InstallColorBinDisplay.
// ---------------------------------------------------------------------------
void SYSTEM_MODULAR::SystemModularInitial()
{
#if 0 // TODO(wave-SECS): SystemModularInitial -- database.cpp:1539-1546
    MyGem = new HT9045Gem("HT9045", HGem);
    if(NUMBER_PANEL_TYPE==3 || NUMBER_PANEL_TYPE==4)
        InstallColorBinDisplay(NUMBER_PANEL_TYPE);
#endif
}

// ---------------------------------------------------------------------------
//  #if 0: ReadGeneralIni  (database.cpp:301-1537, ~1240 lines)
//  Deferred: pulls ~hundreds of cmydef/cprod globals via CheckAndReadIniDataGeneral.
//  Also calls LoadIoData() at database.cpp:1091 (gated internally behind IO_CARD_TYPE).
//  The LoadIoData/LoadMotData functions themselves are active in this batch;
//  only their ReadGeneralIni call-site is deferred.
// ---------------------------------------------------------------------------
#if 0 // TODO(wave): ReadGeneralIni -- database.cpp:301-1537
void SYSTEM_MODULAR::ReadGeneralIni()
{
    // ~1240 lines of CheckAndReadIniDataGeneral assignments
    // ...
    // LoadIoData();   // database.cpp:1091 (inside IO_CARD_TYPE check)
    // LoadMotData();  // called after LoadIoData
}
#endif // TODO(wave)

// ---------------------------------------------------------------------------
//  #if 0: InstallColorBinDisplay  (database.cpp:1684-1729)
// ---------------------------------------------------------------------------
#if 0 // TODO(wave-UI): InstallColorBinDisplay -- database.cpp:1684-1729
void SYSTEM_MODULAR::InstallColorBinDisplay(int iType)
{
    // TMyBinDispHT9046 / BinDisCtrl / eBinDispTotal / etc.
}
#endif // TODO(wave-UI)

// ===========================================================================
//  SYSTEM_MODULAR::LoadIoData()  (database.cpp:1548-1614)
//
//  Loads IO_Table.csv from IoTablePath (common.h extern).
//  Steps:
//    1. Set IoTablePath.
//    2. FileExists guard; ShowMyMessage if not found.
//    3. Allocate TStringList, delete + clear existing IOTable / mapIOTable
//       (memory-leak fix: Steven 20260612).
//    4. LoadFromFile.
//    5. Count<=1 -> ShowMyMessage data-lose.
//    6. SetIOTableNo(header row) -> iResult.
//    7. Gate: iResult==eioTotal(15) exactly.
//    8. For each data row i=1..Count-1:
//         push_back(new TIODATA(row)) then dedup map keyed by Alias.
//         Index arithmetic: IOTable[i-1] is the just-pushed row.
//    9. catch(...) -> MyDBIProcess + ShowMyMessage "opened by other software".
//   10. delete StrList.
// ===========================================================================
void SYSTEM_MODULAR::LoadIoData()
{
    int iResult;
    AnsiString Str;
    IoTablePath="D:\\HT9045\\System\\IO_Table.csv";      // database.cpp:1552
    if(FileExists(IoTablePath))
    {
        TStringList *StrList=new TStringList();

        mapIOTable.clear();
        for(int i=0; i<(int)IOTable.size(); i++)         // Steven 20260612: fix memory leak
            delete IOTable[i];
        IOTable.clear();
        try
        {
            StrList->LoadFromFile(IoTablePath);

            if(StrList->Count<=1)
            {
                Str.sprintf("File %s data is lose!", IoTablePath);
                ShowMyMessage(Str, "IO file data error!");
            }
            else
            {
                iResult=HSys.IoNo.SetIOTableNo(StrList->Strings[0]);
                if(iResult==HSys.IoNo.eioTotal)
                {
                    for(int i=1; i<StrList->Count; i++)
                    {
                        IOTable.push_back(new TIODATA(StrList->Strings[i]));
                        mapIOTableIter=mapIOTable.find(IOTable[i-1]->Alias);
                        Str=IOTable[i-1]->Alias;
                        if(Str!="")
                        {
                            if(mapIOTableIter==mapIOTable.end())
                            {
                                mapIOTable[IOTable[i-1]->Alias]=i-1;
                            }
                            else
                            {
                                Str.sprintf("IO %s alias is duplicated!", IOTable[i-1]->Alias);
                                ShowMyMessage(Str, "IO alias duplicated!");
                            }
                        }
                    }
                }
                else
                {
                    Str.sprintf("File %s data is mistake! (%d)", IoTablePath, iResult);
                    ShowMyMessage(Str, "IO file data error!");
                }
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "SYSTEM_MODULAR::LoadIoData");
            Str.sprintf("File %s is opened by other software!", IoTablePath);
            ShowMyMessage(Str, "IO file opened by other software!");
        }
        delete StrList;
    }
    else
    {
        Str.sprintf("File %s is not exist!", IoTablePath);
        ShowMyMessage(Str, "IO file not found!");
    }
}

// ===========================================================================
//  SYSTEM_MODULAR::LoadMotData()  (database.cpp:1616-1682)
//
//  Symmetric to LoadIoData.  Key difference: gate uses >=emotTotal-1 (>=28)
//  instead of exact ==eioTotal.  (Steven 20231218 HT7080B -- tolerates one
//  missing column, e.g. SimulateSpeed.)
//  FAITHFULNESS: catch block logs "SYSTEM_MODULAR::LoadIoData" (copy-paste bug
//  at database.cpp:1671) -- preserved exactly.
// ===========================================================================
void SYSTEM_MODULAR::LoadMotData()
{
    AnsiString Str;
    int iResult;
    MotTablePath="D:\\HT9045\\System\\Mot_Table.csv";    // database.cpp:1620
    if(FileExists(MotTablePath))
    {
        TStringList *StrList=new TStringList();

        mapMotTable.clear();
        for(int i=0; i<(int)MotTable.size(); i++)        // Steven 20260612: fix memory leak
            delete MotTable[i];
        MotTable.clear();
        try
        {
            StrList->LoadFromFile(MotTablePath);

            if(StrList->Count<=1)
            {
                Str.sprintf("File %s data is lose!", MotTablePath);
                ShowMyMessage(Str, "Motor file data error!");
            }
            else
            {
                iResult=HSys.MotNo.SetMOTTableNo(StrList->Strings[0]);
                if(iResult>=HSys.MotNo.emotTotal-1)      // Steven 20231218 HT7080B: >= not ==
                {
                    for(int i=1; i<StrList->Count; i++)
                    {
                        MotTable.push_back(new TMOTDATA(StrList->Strings[i]));
                        mapMotTableIter=mapMotTable.find(MotTable[i-1]->No);
                        Str=MotTable[i-1]->No;
                        if(Str!="")
                        {
                            if(mapMotTableIter==mapMotTable.end())
                            {
                                mapMotTable[MotTable[i-1]->No]=i-1;
                            }
                            else
                            {
                                Str.sprintf("Motor %s alias is duplicated!", MotTable[i-1]->No);
                                ShowMyMessage(Str, "Motor alias duplicated!");
                            }
                        }
                    }
                }
                else
                {
                    Str.sprintf("File %s data is mistake! (%d)", MotTablePath, iResult);
                    ShowMyMessage(Str, "Motor file data error!");
                }
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "SYSTEM_MODULAR::LoadIoData"); // NOTE: copy-paste bug from database.cpp:1671 -- preserved faithfully
            Str.sprintf("File %s is opened by other software!", MotTablePath);
            ShowMyMessage(Str, "Motor file opened by other software!");
        }
        delete StrList;
    }
    else
    {
        Str.sprintf("File %s is not exist!", MotTablePath);
        ShowMyMessage(Str, "Motor file not found!");
    }
}

// ===========================================================================
//  TIODATA::TIODATA(AnsiString Str)  (database.cpp:1756-1903)
//
//  Row constructor.  SL->CommaText=Str does CSV parse (quote-aware).
//  VALID path: SL->Count >= 14 (note: >=14, not ==15; eioNote col is optional).
//  INVALID path (Count<14): all string fields="", all int fields=-1.
//    Exception: iInType=-1 in invalid path (vs. 0 in valid-empty path).
//
//  Column reads use name-resolved indices from HSys.IoNo (SetIOTableNo must
//  have been called before the first TIODATA ctor is invoked by LoadIoData).
//
//  Field parse order (matches database.cpp:1778-1880):
//    ISABase  FIRST (KenHsieh 20260421: ISABase needs to come first)
//    Lane     (empty -> -1; if eMotionNet -> bHasNullData)
//    ModuleType
//    IP       (empty -> -1; if eMotionNet -> bHasNullData)
//    Port     (empty -> -1 + bHasNullData;
//              non-empty: if ISABase in {1,2,4} -> HexStrToInt(.c_str())
//                         else atoi)
//    Bit      (empty -> -1 + bHasNullData)
//    InType   (empty -> 0, NOT -1)
//    Enable   (empty OR bHasNullData -> 0)
//    OnAlarmTime / OffAlarmTime / OnDelayTime / OffDelayTime  (empty -> -1)
//
//  Alias branches (database.cpp:1767-1776): all five branches assign the same
//  column index (eioAlias) -- the Cylinder_On/Off / Sucker_On/Off AnsiPos
//  checks are semantically dead but MUST be reproduced faithfully.
//
//  Tag and bDelete are always set (database.cpp:1900-1901).
// ===========================================================================
TIODATA::TIODATA(AnsiString Str)
{
    bool bHasNullData=false;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count>=14)
    {
        _CommaText=Str;
        Type=SL->Strings[HSys.IoNo.eioType];

        // Alias -- all five branches assign the same index (dead-equal, preserved faithfully)
        // database.cpp:1767-1776
        if(Type.AnsiPos(AnsiString("Cylinder_On"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Cylinder_Off"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Sucker_On"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else if(Type.AnsiPos(AnsiString("Sucker_Off"))!=0)
            Alias=SL->Strings[HSys.IoNo.eioAlias];
        else
            Alias=SL->Strings[HSys.IoNo.eioAlias];

        // ISABase FIRST (KenHsieh 20260421) -- database.cpp:1778-1785
        if(AnsiString(SL->Strings[HSys.IoNo.eioISABase])==AnsiString(""))
        {
            iISABase=eMotionNet;
        }
        else
        {
            iISABase=atoi(AnsiString(SL->Strings[HSys.IoNo.eioISABase]).c_str());
        }

        // Lane -- database.cpp:1787-1796
        if(AnsiString(SL->Strings[HSys.IoNo.eioLane])==AnsiString(""))
        {
            iLane=-1;
            if(iISABase==eMotionNet)            // Steven 20231218 HT7080B
                bHasNullData=true;
        }
        else
        {
            iLane=atoi(AnsiString(SL->Strings[HSys.IoNo.eioLane]).c_str());
        }

        // ModuleType -- database.cpp:1798-1805
        if(AnsiString(SL->Strings[HSys.IoNo.eioModuleType])==AnsiString(""))
        {
            iModuleType=-1;
        }
        else
        {
            iModuleType=atoi(AnsiString(SL->Strings[HSys.IoNo.eioModuleType]).c_str());
        }

        // IP -- database.cpp:1807-1816
        if(AnsiString(SL->Strings[HSys.IoNo.eioIP])==AnsiString(""))
        {
            iIP=-1;
            if(iISABase==eMotionNet)            // Steven 20231218 HT7080B
                bHasNullData=true;
        }
        else
        {
            iIP=atoi(AnsiString(SL->Strings[HSys.IoNo.eioIP]).c_str());
        }

        // Port -- database.cpp:1818-1831
        // HEX rule: if ISABase in {eISABase(1), ePCI1735U(2), ePLCbase(4)} use HexStrToInt(.c_str())
        // else atoi.  Note: TIODATA uses HexStrToInt(.c_str()) -- the char* overload.
        if(AnsiString(SL->Strings[HSys.IoNo.eioPort])==AnsiString(""))
        {
            iPort=-1;
            bHasNullData=true;
        }
        else
        {
            if(iISABase==eISABase   ||          // Nickliu 20230310: add Suck ISABase Type
               iISABase==ePCI1735U  ||
               iISABase==ePLCbase)
                iPort=HexStrToInt(AnsiString(SL->Strings[HSys.IoNo.eioPort]).c_str());
            else
                iPort=atoi(AnsiString(SL->Strings[HSys.IoNo.eioPort]).c_str());
        }

        // Bit -- database.cpp:1833-1841
        if(AnsiString(SL->Strings[HSys.IoNo.eioBit])==AnsiString(""))
        {
            iBit=-1;
            bHasNullData=true;
        }
        else
        {
            iBit=atoi(AnsiString(SL->Strings[HSys.IoNo.eioBit]).c_str());
        }

        // InType -- database.cpp:1843-1850  NOTE: empty -> 0, NOT -1
        if(AnsiString(SL->Strings[HSys.IoNo.eioInType])==AnsiString(""))
        {
            iInType=0;
        }
        else
        {
            iInType=atoi(AnsiString(SL->Strings[HSys.IoNo.eioInType]).c_str());
        }

        // Enable -- database.cpp:1852-1859  empty OR bHasNullData -> 0
        if(AnsiString(SL->Strings[HSys.IoNo.eioEnable])==AnsiString("") || bHasNullData)
        {
            iEnable=0;
        }
        else
        {
            iEnable=atoi(AnsiString(SL->Strings[HSys.IoNo.eioEnable]).c_str());
        }

        // OnAlarmTime -- database.cpp:1861-1864
        if(AnsiString(SL->Strings[HSys.IoNo.eioOnAlarmTime])==AnsiString(""))
            iOnAlarmTime=-1;
        else
            iOnAlarmTime=atoi(AnsiString(SL->Strings[HSys.IoNo.eioOnAlarmTime]).c_str());

        // OffAlarmTime -- database.cpp:1866-1869
        if(AnsiString(SL->Strings[HSys.IoNo.eioOffAlarmTime])==AnsiString(""))
            iOffAlarmTime=-1;
        else
            iOffAlarmTime=atoi(AnsiString(SL->Strings[HSys.IoNo.eioOffAlarmTime]).c_str());

        // OnDelayTime -- database.cpp:1871-1875  (blank line in original preserved as intent)
        if(AnsiString(SL->Strings[HSys.IoNo.eioOnDelayTime])==AnsiString(""))

            iOnDelayTime=-1;
        else
            iOnDelayTime=atoi(AnsiString(SL->Strings[HSys.IoNo.eioOnDelayTime]).c_str());

        // OffDelayTime -- database.cpp:1877-1880
        if(AnsiString(SL->Strings[HSys.IoNo.eioOffDelayTime])==AnsiString(""))
            iOffDelayTime=-1;
        else
            iOffDelayTime=atoi(AnsiString(SL->Strings[HSys.IoNo.eioOffDelayTime]).c_str());
    }
    else
    {
        // Count < 14: all fields blank / -1  (database.cpp:1883-1898)
        // NOTE: iInType=-1 here, contrasting iInType=0 in the empty-cell case above.
        _CommaText      ="";
        Type            ="";
        Alias           ="";
        iLane           =-1;
        iModuleType     =-1;
        iIP             =-1;
        iPort           =-1;
        iBit            =-1;
        iInType         =-1;
        iISABase        =-1;
        iEnable         =-1;
        iOnAlarmTime    =-1;
        iOffAlarmTime   =-1;
        iOnDelayTime    =-1;
        iOffDelayTime   =-1;
    }
    // Always set (database.cpp:1900-1901)
    Tag=HSys.IOTable.size();
    bDelete=false;
    delete SL;
}

// ===========================================================================
//  TIOTABLENO::TIOTABLENO()  (database.cpp:1905-1923)
//  Sets positional fallback indices.  Overwritten by SetIOTableNo name-scan.
// ===========================================================================
TIOTABLENO::TIOTABLENO()
{
    eioType             =0;
    eioAlias            =1;
    eioLane             =2;
    eioModuleType       =3;
    eioIP               =4;
    eioPort             =5;
    eioBit              =6;
    eioInType           =7;
    eioISABase          =8;
    eioEnable           =9;
    eioOnAlarmTime      =10;
    eioOffAlarmTime     =11;
    eioOnDelayTime      =12;
    eioOffDelayTime     =13;
    eioNote             =14;
    eioTotal            =15;
}

// ===========================================================================
//  TIOTABLENO::SetIOTableNo(AnsiString Str)  (database.cpp:1925-2045)
//
//  Name-based column resolver.  SL->CommaText=Str (header row CSV split).
//  Pattern per column:
//    field = -1;
//    for i in 0..Count-1: if AnsiString(SL->Strings[i]).AnsiPos(token)!=0 -> field=i  // LAST wins
//    if field==-1 -> bResult=<positional fallback index>
//
//  Guard: scan runs ONLY if SL->Count==eioTotal(15).
//    If Count!=eioTotal: bResult=false(0), skip entire scan.
//  Success: returns eioTotal(15) when all 15 columns resolve.
//
//  AnsiPos is 1-based substring; !=0 means "token found anywhere in cell".
//  No break in the inner loop -> LAST matching column wins.
// ===========================================================================
int TIOTABLENO::SetIOTableNo(AnsiString Str)
{
    int bResult=eioTotal;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count!=eioTotal)
    {
        bResult=false;
    }
    else
    {
        eioType=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("IOType"))!=0)
                eioType=i;
        if(eioType==-1)
            bResult=0;

        eioAlias=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Alias"))!=0)
                eioAlias=i;
        if(eioAlias==-1)
            bResult=1;

        eioLane=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Lane"))!=0)
                eioLane=i;
        if(eioLane==-1)
            bResult=2;

        eioModuleType=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("ModuleType"))!=0)
                eioModuleType=i;
        if(eioModuleType==-1)
            bResult=3;

        eioIP=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("IP"))!=0)
                eioIP=i;
        if(eioIP==-1)
            bResult=4;

        eioPort=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Port"))!=0)
                eioPort=i;
        if(eioPort==-1)
            bResult=5;

        eioBit=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Bit"))!=0)
                eioBit=i;
        if(eioBit==-1)
            bResult=6;

        eioInType=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("InType"))!=0)
                eioInType=i;
        if(eioInType==-1)
            bResult=7;

        eioISABase=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("ISABase"))!=0)
                eioISABase=i;
        if(eioISABase==-1)
            bResult=8;

        eioEnable=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Enable"))!=0)
                eioEnable=i;
        if(eioEnable==-1)
            bResult=9;

        eioOnAlarmTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("OnAlarmTime"))!=0)
                eioOnAlarmTime=i;
        if(eioOnAlarmTime==-1)
            bResult=10;

        eioOffAlarmTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("OffAlarmTime"))!=0)
                eioOffAlarmTime=i;
        if(eioOffAlarmTime==-1)
            bResult=11;

        eioOnDelayTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("OnDelayTime"))!=0)
                eioOnDelayTime=i;
        if(eioOnDelayTime==-1)
            bResult=12;

        eioOffDelayTime=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("OffDelayTime"))!=0)
                eioOffDelayTime=i;
        if(eioOffDelayTime==-1)
            bResult=13;

        eioNote=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Note"))!=0)
                eioNote=i;
        if(eioNote==-1)
            bResult=14;
    }

    delete SL;
    return bResult;
}

// ===========================================================================
//  TMOTNO::TMOTNO()  (database.cpp:2047-2079)
//  Sets positional fallback indices.  Overwritten by SetMOTTableNo name-scan.
// ===========================================================================
TMOTNO::TMOTNO()
{
    emotNo              =0;
    emotAlias           =1;
    emotEnable          =2;
    emotBoardID         =3;
    emotPort            =4;
    emotIP              =5;
    emotGearRatio       =6;
    emotSoftLimitN      =7;
    emotSoftLimitP      =8;
    emotCardModel       =9;
    emotDirection       =10;
    emotHomeDirectior   =11;
    emotAcc             =12;
    emotDec             =13;
    emotInitSpeed       =14;
    emotHomeHighSpeed   =15;
    emotHomeLowSpeed    =16;
    emotJogHighSpeed    =17;
    emotJogLowSpeed     =18;
    emotRange           =19;
    emotRate            =20;
    emotServoAlarmOn    =21;
    emot1P2P            =22;
    emotSensorType      =23;
    emotEncodeType      =24;
    emotPickLimit       =25;
    emotLimitLogic      =26;
    emotIn1Logic        =27;
    emotSimulateSpeed   =28;
    emotTotal           =29;
}

// ===========================================================================
//  TMOTNO::SetMOTTableNo(AnsiString Str)  (database.cpp:2081-2298)
//
//  Name-based column resolver for Mot_Table.csv.
//  Differences from SetIOTableNo:
//    1. Init: bResult=emotTotal(29).
//    2. Count check: Count!=HSys.MotNo.emotTotal -> bResult=-2.
//       KEY: the `else` before the scan block is COMMENTED OUT at database.cpp:2091
//       (`//    else`), so the name-scan runs UNCONDITIONALLY even when Count
//       mismatches.  A real 29-col header resolves all columns and overwrites
//       bResult=-2; LoadMotData's gate (>=emotTotal-1 == >=28) then passes.
//    3. 29 logical columns instead of 15.
//    4. Token "HomeDirectior" (sic, misspelling in BCB6 -- preserved).
//    5. Success: returns emotTotal(29) when all 29 columns resolve.
//
//  AnsiPos substring, LAST-wins, no break -- same as SetIOTableNo.
// ===========================================================================
int TMOTNO::SetMOTTableNo(AnsiString Str)
{
    int bResult=emotTotal;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count!=HSys.MotNo.emotTotal)
    {
        bResult=-2;
    }
//    else                    // database.cpp:2091 -- this `else` is COMMENTED OUT;
                              // scan runs unconditionally.  PRESERVED FAITHFULLY.
    {
        emotNo=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Motorname"))!=0)
                emotNo=i;
        if(emotNo==-1)
            bResult=0;

        emotAlias=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Alias"))!=0)
                emotAlias=i;
        if(emotAlias==-1)
            bResult=1;

        emotEnable=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Enable"))!=0)
                emotEnable=i;
        if(emotEnable==-1)
            bResult=2;

        emotBoardID=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("BoardID"))!=0)
                emotBoardID=i;
        if(emotBoardID==-1)
            bResult=3;

        emotPort=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Port"))!=0)
                emotPort=i;
        if(emotPort==-1)
            bResult=4;

        emotIP=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("IP"))!=0)
                emotIP=i;
        if(emotIP==-1)
            bResult=5;

        emotGearRatio=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("GearRatio"))!=0)
                emotGearRatio=i;
        if(emotGearRatio==-1)
            bResult=6;

        emotSoftLimitN=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("SoftLimitN"))!=0)
                emotSoftLimitN=i;
        if(emotSoftLimitN==-1)
            bResult=7;

        emotSoftLimitP=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("SoftLimitP"))!=0)
                emotSoftLimitP=i;
        if(emotSoftLimitP==-1)
            bResult=8;

        emotCardModel=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("CardModel"))!=0)
                emotCardModel=i;
        if(emotCardModel==-1)
            bResult=9;

        emotDirection=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Direction"))!=0)
                emotDirection=i;
        if(emotDirection==-1)
            bResult=10;

        emotHomeDirectior=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("HomeDirectior"))!=0) // sic: misspelling preserved
                emotHomeDirectior=i;
        if(emotHomeDirectior==-1)
            bResult=11;

        emotAcc=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Acc"))!=0)
                emotAcc=i;
        if(emotAcc==-1)
            bResult=12;

        emotDec=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Dec"))!=0)
                emotDec=i;
        if(emotDec==-1)
            bResult=13;

        emotInitSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("InitSpeed"))!=0)
                emotInitSpeed=i;
        if(emotInitSpeed==-1)
            bResult=14;

        emotHomeHighSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("HomeHighSpeed"))!=0)
                emotHomeHighSpeed=i;
        if(emotHomeHighSpeed==-1)
            bResult=15;

        emotHomeLowSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("HomeLowSpeed"))!=0)
                emotHomeLowSpeed=i;
        if(emotHomeLowSpeed==-1)
            bResult=16;

        emotJogHighSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("JogHighSpeed"))!=0)
                emotJogHighSpeed=i;
        if(emotJogHighSpeed==-1)
            bResult=17;

        emotJogLowSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("JogLowSpeed"))!=0)
                emotJogLowSpeed=i;
        if(emotJogLowSpeed==-1)
            bResult=18;

        emotRange=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Range"))!=0)
                emotRange=i;
        if(emotRange==-1)
            bResult=19;

        emotRate=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("Rate"))!=0)
                emotRate=i;
        if(emotRate==-1)
            bResult=20;

        emotServoAlarmOn=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("ServoAlarmOn"))!=0)
                emotServoAlarmOn=i;
        if(emotServoAlarmOn==-1)
            bResult=21;

        emot1P2P=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("1P2P"))!=0)
                emot1P2P=i;
        if(emot1P2P==-1)
            bResult=22;

        emotSensorType=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("SensorType"))!=0)
                emotSensorType=i;
        if(emotSensorType==-1)
            bResult=23;

        emotEncodeType=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("EncodeType"))!=0)
                emotEncodeType=i;
        if(emotEncodeType==-1)
            bResult=24;

        emotPickLimit=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("PickLimit"))!=0)
                emotPickLimit=i;
        if(emotPickLimit==-1)
            bResult=25;

        emotLimitLogic=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("LimitLogic"))!=0)
                emotLimitLogic=i;
        if(emotLimitLogic==-1)
            bResult=26;

        emotIn1Logic=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("In1Logic"))!=0)
                emotIn1Logic=i;
        if(emotIn1Logic==-1)
            bResult=27;

        emotSimulateSpeed=-1;
        for(int i=0; i<SL->Count; i++)
            if(AnsiString(SL->Strings[i]).AnsiPos(AnsiString("SimulateSpeed"))!=0)
                emotSimulateSpeed=i;
        if(emotSimulateSpeed==-1)
            bResult=28;
    }
    delete SL;
    return bResult;
}

// ===========================================================================
//  TMOTDATA::TMOTDATA(AnsiString Str)  (database.cpp:2300-2874)
//
//  Row constructor.  VALID path: SL->Count >= HSys.MotNo.emotTotal (29).
//  All field reads guard `HSys.MotNo.emotXxx < 0 || SL->Strings[emotXxx]==""`.
//  The <0 guard (Steven 20231218/20240103) treats a name-unresolved column
//  the same as an empty cell.
//
//  SPECIAL CASE (database.cpp:2353-2363):
//    INDEX_MOTION_CARD==0 AND Alias in {MTestY1, MTestZ1, MTestZ2, MTestY2}:
//    force CardModel="SMC", iBoardID=-1, iPort=-1, iIP=-1,
//    dAcc=1.0, dDec=1.0, iLimitLogic=0, iIn1Logic=0.
//    (Skip all those field reads.)
//
//  Port HEX rule for MC88X1 (database.cpp:2419):
//    HexStrToInt(SL->Strings[HSys.MotNo.emotPort])  -- AnsiString overload,
//    NOT .c_str().  Contrast TIODATA which uses .c_str().
//
//  IP only read if CardModel=="SYNTEK" (database.cpp:2424-2445); else iIP=-1.
//
//  PickLimit only read if Alias in {MTestZ1, MTestZ2} (database.cpp:2792-2810);
//  warn always (no iEnable guard here) -- preserved faithfully.
//
//  bHasNullData -> iEnable=0 at end of valid path (database.cpp:2835-2838).
//
//  INVALID path (Count<29): full set of safe defaults (database.cpp:2842-2872).
// ===========================================================================
TMOTDATA::TMOTDATA(AnsiString Str)
{
    AnsiString Msg;
    bool bHasNullData=false;
    TStringList *SL=new TStringList();
    SL->CommaText=Str;

    if(SL->Count>=HSys.MotNo.emotTotal)
    {
        _CommaText=Str;

        // Enable (database.cpp:2311-2319)
        if(HSys.MotNo.emotEnable<0 ||           // Steven 20240103: add guard
           AnsiString(SL->Strings[HSys.MotNo.emotEnable])==AnsiString(""))
        {
            iEnable=0;
        }
        else
        {
            iEnable=atoi(AnsiString(SL->Strings[HSys.MotNo.emotEnable]).c_str());
        }

        // Motorname -> No (database.cpp:2321-2335)
        if(HSys.MotNo.emotNo<0 ||               // Steven 20240103: add guard
           AnsiString(SL->Strings[HSys.MotNo.emotNo])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("MotorName is NULL in %s", MotTablePath);
                ShowMyMessage(Msg, "MotorName is NULL!");
            }
            bHasNullData=true;
            No="";
        }
        else
        {
            No=SL->Strings[HSys.MotNo.emotNo];
        }

        // Alias (database.cpp:2337-2351)
        if(HSys.MotNo.emotAlias<0 ||            // Steven 20240103: add guard
           AnsiString(SL->Strings[HSys.MotNo.emotAlias])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Alias of %s is NULL in %s", No, MotTablePath);
                ShowMyMessage(Msg, "Alias is NULL!");
            }
            bHasNullData=true;
            Alias="";
        }
        else
        {
            Alias=SL->Strings[HSys.MotNo.emotAlias];
        }

        // Special case: INDEX_MOTION_CARD==0 and Alias is an index motor
        // (database.cpp:2353-2363)
        if(INDEX_MOTION_CARD==0 && (Alias=="MTestY1" || Alias=="MTestZ1" || Alias=="MTestZ2" || Alias=="MTestY2"))
        {
            CardModel="SMC";
            iBoardID=-1;
            iPort=-1;
            iIP=-1;
            dAcc=1.0;
            dDec=1.0;
            iLimitLogic=0;
            iIn1Logic=0;
        }
        else
        {
            // CardModel (database.cpp:2366-2380)
            if(HSys.MotNo.emotCardModel<0 ||    // Steven 20231218 HT7080B
               AnsiString(SL->Strings[HSys.MotNo.emotCardModel])==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("CardModel of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "CardModel is NULL!");
                }
                bHasNullData=true;
                CardModel="";
            }
            else
            {
                CardModel=SL->Strings[HSys.MotNo.emotCardModel];
            }

            // BoardID (database.cpp:2382-2403)
            if(CardModel=="MC88X1")             // Steven 20231218 HT7080B
            {
                iBoardID=0;
            }
            else
            {
                if(HSys.MotNo.emotBoardID<0 ||
                   AnsiString(SL->Strings[HSys.MotNo.emotBoardID])==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("Alias of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "Alias is NULL!");
                    }
                    bHasNullData=true;
                    iBoardID=-1;
                }
                else
                {
                    iBoardID=atoi(AnsiString(SL->Strings[HSys.MotNo.emotBoardID]).c_str());
                }
            }

            // Port (database.cpp:2405-2422)
            // MC88X1: HexStrToInt(AnsiString overload) -- NOT .c_str() (database.cpp:2419)
            if(HSys.MotNo.emotPort<0 ||         // Steven 20231218 HT7080B
               AnsiString(SL->Strings[HSys.MotNo.emotPort])==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("Port of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "Port is NULL!");
                }
                bHasNullData=true;
                iPort=-1;
            }
            else
            {
                if(CardModel=="MC88X1")         // Steven 20231218 HT7080B
                    iPort=HexStrToInt(SL->Strings[HSys.MotNo.emotPort]); // AnsiString overload
                else
                    iPort=atoi(AnsiString(SL->Strings[HSys.MotNo.emotPort]).c_str());
            }

            // IP: only read for SYNTEK (database.cpp:2424-2445)
            if(CardModel=="SYNTEK")
            {
                if(HSys.MotNo.emotIP<0 ||       // Steven 20240103: add guard
                   AnsiString(SL->Strings[HSys.MotNo.emotIP])==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("IP of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "IP is NULL!");
                    }
                    bHasNullData=true;
                    iIP=-1;
                }
                else
                {
                    iIP=atoi(AnsiString(SL->Strings[HSys.MotNo.emotIP]).c_str());
                }
            }
            else
            {
                iIP=-1;
            }

            // Acc / Dec / LimitLogic / In1Logic: MC88X1 gets fixed values (database.cpp:2447-2519)
            if(CardModel=="MC88X1")             // Steven 20231218 HT7080B
            {
                dAcc=1.0;
                dDec=1.0;
                iLimitLogic=0;
                iIn1Logic=0;
            }
            else
            {
                // Acc (database.cpp:2456-2470)
                if(HSys.MotNo.emotAcc<0 ||
                   AnsiString(SL->Strings[HSys.MotNo.emotAcc])==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("ACC of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "ACC is NULL!");
                    }
                    bHasNullData=true;
                    dAcc=1.0;
                }
                else
                {
                    dAcc=atof(AnsiString(SL->Strings[HSys.MotNo.emotAcc]).c_str());
                }

                // Dec (database.cpp:2472-2486)
                if(HSys.MotNo.emotDec<0 ||
                   AnsiString(SL->Strings[HSys.MotNo.emotDec])==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("DEC of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "DEC is NULL!");
                    }
                    bHasNullData=true;
                    dDec=1.0;
                }
                else
                {
                    dDec=atof(AnsiString(SL->Strings[HSys.MotNo.emotDec]).c_str());
                }

                // LimitLogic (database.cpp:2488-2502)
                if(HSys.MotNo.emotLimitLogic<0 ||
                   AnsiString(SL->Strings[HSys.MotNo.emotLimitLogic])==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("LimitLogic of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "LimitLogic is NULL!");
                    }
                    bHasNullData=true;
                    iLimitLogic=0;
                }
                else
                {
                    iLimitLogic=atoi(AnsiString(SL->Strings[HSys.MotNo.emotLimitLogic]).c_str());
                }

                // In1Logic (database.cpp:2504-2518)
                if(HSys.MotNo.emotIn1Logic<0 ||
                   AnsiString(SL->Strings[HSys.MotNo.emotIn1Logic])==AnsiString(""))
                {
                    if(iEnable)
                    {
                        Msg.sprintf("In1Logic of %s is NULL in %s", Alias, MotTablePath);
                        ShowMyMessage(Msg, "In1Logic is NULL!");
                    }
                    bHasNullData=true;
                    iIn1Logic=0;
                }
                else
                {
                    iIn1Logic=atoi(AnsiString(SL->Strings[HSys.MotNo.emotIn1Logic]).c_str());
                }
            }
        } // end of not-INDEX_MOTION_CARD-index-motor else block

        // GearRatio (database.cpp:2522-2536)
        if(HSys.MotNo.emotGearRatio<0 ||        // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotGearRatio])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("GearRatio of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "GearRatio is NULL!");
            }
            bHasNullData=true;
            dGearRatio=1.0;
        }
        else
        {
            dGearRatio=atof(AnsiString(SL->Strings[HSys.MotNo.emotGearRatio]).c_str());
        }

        // Direction (database.cpp:2538-2552)
        if(HSys.MotNo.emotDirection<0 ||        // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotDirection])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Direction of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "Direction is NULL!");
            }
            bHasNullData=true;
            iDirection=0;
        }
        else
        {
            iDirection=atoi(AnsiString(SL->Strings[HSys.MotNo.emotDirection]).c_str());
        }

        // HomeDirectior (sic: misspelling preserved) (database.cpp:2554-2568)
        if(HSys.MotNo.emotHomeDirectior<0 ||    // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotHomeDirectior])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeDirectior of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "HomeDirectior is NULL!");
            }
            bHasNullData=true;
            iHomeDirectior=0;
        }
        else
        {
            iHomeDirectior=atoi(AnsiString(SL->Strings[HSys.MotNo.emotHomeDirectior]).c_str());
        }

        // SoftLimitN (database.cpp:2570-2584)
        if(HSys.MotNo.emotSoftLimitN<0 ||       // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotSoftLimitN])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SoftLimitN of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "SoftLimitN is NULL!");
            }
            bHasNullData=true;
            iSoftLimitN=-999999;
        }
        else
        {
            iSoftLimitN=atoi(AnsiString(SL->Strings[HSys.MotNo.emotSoftLimitN]).c_str());
        }

        // SoftLimitP (database.cpp:2586-2600)
        if(HSys.MotNo.emotSoftLimitP<0 ||       // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotSoftLimitP])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SoftLimitP of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "SoftLimitP is NULL!");
            }
            bHasNullData=true;
            iSoftLimitP=999999;
        }
        else
        {
            iSoftLimitP=atoi(AnsiString(SL->Strings[HSys.MotNo.emotSoftLimitP]).c_str());
        }

        // InitSpeed (database.cpp:2602-2616)
        if(HSys.MotNo.emotInitSpeed<0 ||        // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotInitSpeed])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("InitSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "InitSpeed is NULL!");
            }
            bHasNullData=true;
            iInitSpeed=100;
        }
        else
        {
            iInitSpeed=atoi(AnsiString(SL->Strings[HSys.MotNo.emotInitSpeed]).c_str());
        }

        // HomeHighSpeed (database.cpp:2618-2632)
        if(HSys.MotNo.emotHomeHighSpeed<0 ||    // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotHomeHighSpeed])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeHighSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "HomeHighSpeed is NULL!");
            }
            bHasNullData=true;
            iHomeHighSpeed=100;
        }
        else
        {
            iHomeHighSpeed=atoi(AnsiString(SL->Strings[HSys.MotNo.emotHomeHighSpeed]).c_str());
        }

        // HomeLowSpeed (database.cpp:2634-2648)
        if(HSys.MotNo.emotHomeLowSpeed<0 ||     // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotHomeLowSpeed])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("HomeLowSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "HomeLowSpeed is NULL!");
            }
            bHasNullData=true;
            iHomeLowSpeed=100;
        }
        else
        {
            iHomeLowSpeed=atoi(AnsiString(SL->Strings[HSys.MotNo.emotHomeLowSpeed]).c_str());
        }

        // JogHighSpeed (database.cpp:2650-2664)
        if(HSys.MotNo.emotJogHighSpeed<0 ||     // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotJogHighSpeed])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("JogHighSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "JogHighSpeed is NULL!");
            }
            bHasNullData=true;
            iJogHighSpeed=100;
        }
        else
        {
            iJogHighSpeed=atoi(AnsiString(SL->Strings[HSys.MotNo.emotJogHighSpeed]).c_str());
        }

        // JogLowSpeed (database.cpp:2666-2680)
        if(HSys.MotNo.emotJogLowSpeed<0 ||      // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotJogLowSpeed])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("JogLowSpeed of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "JogLowSpeed is NULL!");
            }
            bHasNullData=true;
            iJogLowSpeed=100;
        }
        else
        {
            iJogLowSpeed=atoi(AnsiString(SL->Strings[HSys.MotNo.emotJogLowSpeed]).c_str());
        }

        // Range: MC88X1 -> 10, else read (database.cpp:2682-2703)
        if(CardModel=="MC88X1")                 // Steven 20231218 HT7080B
        {
            iRange=10;
        }
        else
        {
            if(HSys.MotNo.emotRange<0 ||
               AnsiString(SL->Strings[HSys.MotNo.emotRange])==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("Range of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "Range is NULL!");
                }
                bHasNullData=true;
                iRange=1;
            }
            else
            {
                iRange=atoi(AnsiString(SL->Strings[HSys.MotNo.emotRange]).c_str());
            }
        }

        // Rate (database.cpp:2705-2719)
        if(HSys.MotNo.emotRate<0 ||             // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotRate])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("Rate of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "Rate is NULL!");
            }
            bHasNullData=true;
            iRate=1;
        }
        else
        {
            iRate=atoi(AnsiString(SL->Strings[HSys.MotNo.emotRate]).c_str());
        }

        // ServoAlarmOn (database.cpp:2721-2735)
        if(HSys.MotNo.emotServoAlarmOn<0 ||     // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotServoAlarmOn])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("ServoAlarmOn of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "ServoAlarmOn is NULL!");
            }
            bHasNullData=true;
            iServoAlarmOn=0;
        }
        else
        {
            iServoAlarmOn=atoi(AnsiString(SL->Strings[HSys.MotNo.emotServoAlarmOn]).c_str());
        }

        // 1P2P (database.cpp:2737-2751)
        if(HSys.MotNo.emot1P2P<0 ||             // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emot1P2P])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("1P2P of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "1P2P is NULL!");
            }
            bHasNullData=true;
            i1P2P=0;
        }
        else
        {
            i1P2P=atoi(AnsiString(SL->Strings[HSys.MotNo.emot1P2P]).c_str());
        }

        // SensorType (database.cpp:2753-2767)
        if(HSys.MotNo.emotSensorType<0 ||       // Steven 20231218 HT7080B
           AnsiString(SL->Strings[HSys.MotNo.emotSensorType])==AnsiString(""))
        {
            if(iEnable)
            {
                Msg.sprintf("SensorType of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "SensorType is NULL!");
            }
            bHasNullData=true;
            iSensorType=0;
        }
        else
        {
            iSensorType=atoi(AnsiString(SL->Strings[HSys.MotNo.emotSensorType]).c_str());
        }

        // EncodeType: MC88X1 -> 0, else read (database.cpp:2769-2790)
        if(CardModel=="MC88X1")                 // Steven 20231218 HT7080B
        {
            iEncodeType=0;
        }
        else
        {
            if(HSys.MotNo.emotEncodeType<0 ||   // Steven 20231218 HT7080B
               AnsiString(SL->Strings[HSys.MotNo.emotEncodeType])==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("EncodeType of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "EncodeType is NULL!");
                }
                bHasNullData=true;
                iEncodeType=0;
            }
            else
            {
                iEncodeType=atoi(AnsiString(SL->Strings[HSys.MotNo.emotEncodeType]).c_str());
            }
        }

        // PickLimit: only for MTestZ1/MTestZ2 (database.cpp:2792-2810)
        // NOTE: warn without iEnable guard (no `if(iEnable)`) -- preserved faithfully.
        if(Alias=="MTestZ1" || Alias=="MTestZ2")
        {
            if(HSys.MotNo.emotPickLimit<0 ||    // Steven 20240103: add guard
               AnsiString(SL->Strings[HSys.MotNo.emotPickLimit])==AnsiString(""))
            {
                Msg.sprintf("Motor PickLimit of %s is NULL in %s", Alias, MotTablePath);
                ShowMyMessage(Msg, "Motor PickLimit is NULL!");
                iPickLimit=0;
                bHasNullData=true;
            }
            else
            {
                iPickLimit=atoi(AnsiString(SL->Strings[HSys.MotNo.emotPickLimit]).c_str());
            }
        }
        else
        {
            iPickLimit=0;
        }

        // SimulateSpeed: MC88X1 -> 1000, else read (database.cpp:2812-2833)
        if(CardModel=="MC88X1")                 // Steven 20231218 HT7080B
        {
            iSimulateSpeed=1000;
        }
        else
        {
            if(HSys.MotNo.emotSimulateSpeed<0 || // Steven 20231218 HT7080B
               AnsiString(SL->Strings[HSys.MotNo.emotSimulateSpeed])==AnsiString(""))
            {
                if(iEnable)
                {
                    Msg.sprintf("SimulateSpeed of %s is NULL in %s", Alias, MotTablePath);
                    ShowMyMessage(Msg, "SimulateSpeed is NULL!");
                }
                bHasNullData=true;
                iSimulateSpeed=10000;
            }
            else
            {
                iSimulateSpeed=atoi(AnsiString(SL->Strings[HSys.MotNo.emotSimulateSpeed]).c_str());
            }
        }

        // Any missing field disables the motor (database.cpp:2835-2838)
        if(bHasNullData)
        {
            iEnable=0;
        }
    }
    else
    {
        // Count < 29: full set of safe defaults (database.cpp:2842-2872)
        _CommaText      ="";
        No              ="";
        Alias           ="";
        iEnable         =-1;
        iBoardID        =-1;
        iPort           =-1;
        iIP             =-1;
        dGearRatio      =1.0;
        iSoftLimitN     =-999999;
        iSoftLimitP     =999999;
        CardModel       ="";
        iDirection      =0;
        iHomeDirectior  =0;
        dAcc            =1.0;           // Steven 20231218 HT7080B
        dDec            =1.0;           // Steven 20231218 HT7080B
        iInitSpeed      =100;
        iHomeHighSpeed  =100;
        iHomeLowSpeed   =100;
        iJogHighSpeed   =100;
        iJogLowSpeed    =100;
        iRange          =1;
        iRate           =1;
        iServoAlarmOn   =0;
        i1P2P           =0;
        iSensorType     =0;
        iEncodeType     =0;
        iPickLimit      =0;
        iLimitLogic     =0;
        iIn1Logic       =0;
        iSimulateSpeed  =10000;
    }
    delete SL;
}
