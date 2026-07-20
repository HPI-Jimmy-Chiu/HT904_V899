// ===========================================================================
//  database.h  -- PARTIAL translation (W3-cont2: config-table loaders batch)
//  Golden ref: D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\database.h
//
//  WHAT IS TRANSLATED (this batch):
//    TIODATA        struct + ctor         (database.h:17-38)
//    TIOTABLENO     struct + methods      (database.h:40-61)
//    TMOTDATA       struct + ctor         (database.h:63-98)
//    TMOTNO         struct + methods      (database.h:100-135)
//    SYSTEM_MODULAR class (loader members only; all other members gated)
//                                         (database.h:193-287)
//    extern SYSTEM_MODULAR HSys           (database.h:288)
//
//  WHAT IS GATED (#if 0 // TODO(wave)):
//    TDataModule1 class + extern DataModule1
//      -> BDE TTable / TDataModule / VCL form; needs DBTables.hpp. (database.h:139-187)
//    TMyBinDispCtrl *BinDisCtrl member of SYSTEM_MODULAR
//      -> UI wave; forward-declared opaque. (database.h:204)
//    ReadGeneralIni() method declaration
//      -> ~1240-line cmydef/cprod globals surface; deferred. (database.h:268)
//    All SYSTEM_MODULAR members not needed by the loader batch
//      (com-ports, CCD/RFID arrays, etc.) (database.h:209-249 / 252-286)
//    ATKRecipeInfo pointer + ATK_RECIPE_INFO include
//
//  AI(W906-SysModWire) 20260720: HTGem *MyGem member of SYSTEM_MODULAR is NO
//  LONGER gated -- wired for real by SystemModularInitial (database.cpp),
//  see that member's own comment below (database.h:~233).
//
//  ENCODING NOTE: original is Big5; this translation is UTF-8.  Chinese
//  comments in the golden are replaced with their semantic equivalents in ASCII.
// ===========================================================================
#ifndef DATABASEH
#define DATABASEH

#include "vclcompat/vcl_compat.h"   // AnsiString, TStringList, FileExists, ...
#include <map>
#include <vector>
#include "myTimer.h"                // TQPF_Timer (database.h:12)
using namespace std;

// ---------------------------------------------------------------------------
//  Forward declarations for opaque pointer members (deferred waves)
// ---------------------------------------------------------------------------
class HTGem;             // SECS wave -- uHGemClass.h (database.h:15)
class TMyBinDispCtrl;    // UI wave   -- MyBinDisp.h  (database.h:5)

// ===========================================================================
//  TIODATA  (database.h:17-38)
//  One row of IO_Table.csv after CSV parse + name-based column resolution.
// ===========================================================================
typedef struct TIODATA
{
    int         Tag;            // index == HSys.IOTable.size() at ctor time
    AnsiString  _CommaText;     // raw CSV row string (or "" if Count<14)
    AnsiString  Type;           // IO type string  (col eioType)
    AnsiString  Alias;          // IO alias string (col eioAlias)
    int         iLane;          // -1 if empty; sets bHasNullData when eMotionNet
    int         iModuleType;    // -1 if empty (no null-flag)
    int         iIP;            // -1 if empty; sets bHasNullData when eMotionNet
    int         iPort;          // -1 if empty (+bHasNullData); HEX parse when ISABase in {1,2,4}
    int         iBit;           // -1 if empty (+bHasNullData)
    int         iInType;        // 0 if empty (NOT -1; contrast other fields)
    int         iISABase;       // eMotionNet(0) if empty; else atoi
    int         iEnable;        // 0 if empty OR bHasNullData
    int         iOnAlarmTime;   // -1 if empty
    int         iOffAlarmTime;  // -1 if empty
    int         iOnDelayTime;   // -1 if empty
    int         iOffDelayTime;  // -1 if empty
    bool        bDelete;        // always false at construction

    TIODATA(AnsiString Str=AnsiString(""));
}TIODATA;

// ===========================================================================
//  TIOTABLENO  (database.h:40-61)
//  Holds name-resolved column indices for IO_Table.csv.
//  Default ctor sets positional fallbacks; SetIOTableNo() overwrites via
//  name-scan (AnsiPos substring, last-match-wins).
// ===========================================================================
typedef struct TIOTABLENO
{
    int eioType;         // col index for "IOType"       (fallback 0)
    int eioAlias;        // col index for "Alias"        (fallback 1)
    int eioLane;         // col index for "Lane"         (fallback 2)
    int eioModuleType;   // col index for "ModuleType"   (fallback 3)
    int eioIP;           // col index for "IP"           (fallback 4)
    int eioPort;         // col index for "Port"         (fallback 5)
    int eioBit;          // col index for "Bit"          (fallback 6)
    int eioInType;       // col index for "InType"       (fallback 7)
    int eioISABase;      // col index for "ISABase"      (fallback 8)
    int eioEnable;       // col index for "Enable"       (fallback 9)
    int eioOnAlarmTime;  // col index for "OnAlarmTime"  (fallback 10)
    int eioOffAlarmTime; // col index for "OffAlarmTime" (fallback 11)
    int eioOnDelayTime;  // col index for "OnDelayTime"  (fallback 12)
    int eioOffDelayTime; // col index for "OffDelayTime" (fallback 13)
    int eioNote;         // col index for "Note"         (fallback 14)
    int eioTotal;        // expected column count == 15

    int SetIOTableNo(AnsiString Str);   // name-scan header row; returns eioTotal on success
    TIOTABLENO();
}TIOTABLENO;

// ===========================================================================
//  TMOTDATA  (database.h:63-98)
//  One row of Mot_Table.csv after CSV parse + name-based column resolution.
// ===========================================================================
typedef struct TMOTDATA
{
    AnsiString  _CommaText;     // raw CSV row
    AnsiString  No;             // Motorname  (col emotNo)
    AnsiString  Alias;          // Alias      (col emotAlias)
    int         iEnable;        // 0 if empty (or -1 in Count<29 path)
    int         iBoardID;       // -1 if empty; 0 if MC88X1
    int         iPort;          // -1 if empty; HexStrToInt(AnsiString) for MC88X1
    int         iIP;            // -1 unless CardModel=="SYNTEK"
    double      dGearRatio;     // atof; default 1.0
    int         iSoftLimitN;    // default -999999
    int         iSoftLimitP;    // default  999999
    AnsiString  CardModel;      // e.g. "SMC", "MC88X1", "SYNTEK"

    int         iDirection;     // default 0
    int         iHomeDirectior; // note: misspelling preserved from BCB6 original
    double      dAcc;           // atof; 1.0 for MC88X1 / INDEX_MOTION_CARD==0 index motors
    double      dDec;           // atof; 1.0 for MC88X1 / INDEX_MOTION_CARD==0 index motors
    int         iInitSpeed;     // default 100
    int         iHomeHighSpeed; // default 100
    int         iHomeLowSpeed;  // default 100
    int         iJogHighSpeed;  // default 100
    int         iJogLowSpeed;   // default 100
    int         iRange;         // 10 for MC88X1; default 1
    int         iRate;          // default 1
    int         iServoAlarmOn;  // default 0
    int         i1P2P;          // default 0
    int         iSensorType;    // default 0
    int         iEncodeType;    // 0 for MC88X1; default 0
    int         iPickLimit;     // only for MTestZ1/Z2; default 0
    int         iLimitLogic;    // 0 for MC88X1 / INDEX_MOTION_CARD==0 index motors
    int         iIn1Logic;      // 0 for MC88X1 / INDEX_MOTION_CARD==0 index motors
    int         iSimulateSpeed; // 1000 for MC88X1; default 10000

    TMOTDATA(AnsiString Str=AnsiString(""));
}TMOTDATA;

// ===========================================================================
//  TMOTNO  (database.h:100-135)
//  Holds name-resolved column indices for Mot_Table.csv.
// ===========================================================================
typedef struct TMOTNO
{
    int emotNo;             // col index for "Motorname"      (fallback 0)
    int emotAlias;          // col index for "Alias"          (fallback 1)
    int emotEnable;         // col index for "Enable"         (fallback 2)
    int emotBoardID;        // col index for "BoardID"        (fallback 3)
    int emotPort;           // col index for "Port"           (fallback 4)
    int emotIP;             // col index for "IP"             (fallback 5)
    int emotGearRatio;      // col index for "GearRatio"      (fallback 6)
    int emotSoftLimitN;     // col index for "SoftLimitN"     (fallback 7)
    int emotSoftLimitP;     // col index for "SoftLimitP"     (fallback 8)
    int emotCardModel;      // col index for "CardModel"      (fallback 9)
    int emotDirection;      // col index for "Direction"      (fallback 10)
    int emotHomeDirectior;  // col index for "HomeDirectior"  (fallback 11) -- sic, misspelling
    int emotAcc;            // col index for "Acc"            (fallback 12)
    int emotDec;            // col index for "Dec"            (fallback 13)
    int emotInitSpeed;      // col index for "InitSpeed"      (fallback 14)
    int emotHomeHighSpeed;  // col index for "HomeHighSpeed"  (fallback 15)
    int emotHomeLowSpeed;   // col index for "HomeLowSpeed"   (fallback 16)
    int emotJogHighSpeed;   // col index for "JogHighSpeed"   (fallback 17)
    int emotJogLowSpeed;    // col index for "JogLowSpeed"    (fallback 18)
    int emotRange;          // col index for "Range"          (fallback 19)
    int emotRate;           // col index for "Rate"           (fallback 20)
    int emotServoAlarmOn;   // col index for "ServoAlarmOn"   (fallback 21)
    int emot1P2P;           // col index for "1P2P"           (fallback 22)
    int emotSensorType;     // col index for "SensorType"     (fallback 23)
    int emotEncodeType;     // col index for "EncodeType"     (fallback 24)
    int emotPickLimit;      // col index for "PickLimit"      (fallback 25)
    int emotLimitLogic;     // col index for "LimitLogic"     (fallback 26)
    int emotIn1Logic;       // col index for "In1Logic"       (fallback 27)
    int emotSimulateSpeed;  // col index for "SimulateSpeed"  (fallback 28)
    int emotTotal;          // expected column count == 29

    TMOTNO();
    int SetMOTTableNo(AnsiString Str);  // name-scan header row; returns emotTotal on full success
}TMOTNO;

// ---------------------------------------------------------------------------
// #if 0 // TODO(wave): TDataModule1 (BDE / VCL TDataModule)
//   class TDataModule1 : public TDataModule  { ... TTable *MotorTable; ... }
//   database.h:139-187; needs <Db.hpp> / <DBTables.hpp>; deferred to BDE wave.
//   extern PACKAGE TDataModule1 *DataModule1;  (database.h:187)
// #endif
// ---------------------------------------------------------------------------

// ===========================================================================
//  SYSTEM_MODULAR  (database.h:193-287)
//  Global system parameter aggregate.  PARTIAL: only the loader-batch members
//  are active.  All other members (com-ports, CCD, RFID, SECS, BinDisp, etc.)
//  are gated #if 0 // TODO(wave).
// ===========================================================================
class SYSTEM_MODULAR
{
    private:
        // (database.h:197-198) -- gated: dNumberPanelDelay, bUseSocketTemp
#if 0 // TODO(wave): private members used only by ReadGeneralIni / InstallColorBinDisplay
        double dNumberPanelDelay;   // Sam 20240604
        bool   bUseSocketTemp;
#endif

    protected:
#if 0 // TODO(wave-UI): InstallColorBinDisplay -- TMyBinDispHT9046 / BDE
        void InstallColorBinDisplay(int iType);
#endif

    public:
        // Ctor/dtor are #if 0 gated in database.cpp (ctor calls ReadGeneralIni
        // + SystemModularInitial; dtor deletes BinDisCtrl/ATKRecipeInfo).
        // The default compiler-generated ctor/dtor are used for the loader batch.
        // (database.h:205-206)
#if 0 // TODO(wave): explicit BCB6 ctor/dtor -- call sites: database.cpp:43, 1731
        _fastcall SYSTEM_MODULAR::SYSTEM_MODULAR();
        _fastcall SYSTEM_MODULAR::~SYSTEM_MODULAR();
#endif

        TQPF_Timer SysTimer;    // (database.h:207)

#if 0 // TODO(wave): com-port / CCD / RFID / ATC / Laser / Barcode / GroundMan
      // members (database.h:209-249 / 252-286) -- all pulled in by ReadGeneralIni
        AnsiString sNumberPanelComPort;
        // ... (full list in golden database.h:209-249, 252-286) ...
        AnsiString asLASER_COM[4];
        AnsiString asBarCodeComPort[4];
        AnsiString asRFIDCom;
        // ...
#endif

        // AI(W906-SysModWire) 20260720: wired for real -- SystemModularInitial
        // (database.cpp) now assigns this (golden database.h:250). NULL via
        // static zero-init of the global HSys ONLY -- do not stack-allocate
        // SYSTEM_MODULAR (its implicit ctor leaves this pointer
        // uninitialized; only the global HSys's static storage guarantees a
        // zero start).
        HTGem *MyGem;

        // BinDisp member -- forward-declared opaque (database.h:204)
        TMyBinDispCtrl *BinDisCtrl;  // opaque; NULL until UI wave wires InstallColorBinDisplay

        // ATC alarm map (database.h:255-256) -- populated in ctor; gated pending ctor wave
        map<AnsiString, AnsiString>              mapATCAlarmCode;
        map<AnsiString, AnsiString>::iterator    mapATCAlarmCodeIter;

        // -----------------------------------------------------------------------
        //  Config-table loader members (ACTIVE this batch)
        //  (database.h:257-266)
        // -----------------------------------------------------------------------
        void LoadIoData();      // parses IO_Table.csv  -> IOTable / mapIOTable
        void LoadMotData();     // parses Mot_Table.csv -> MotTable / mapMotTable

        TIOTABLENO IoNo;        // name-resolved IO column indices
        TMOTNO     MotNo;       // name-resolved Motor column indices

        vector <TIODATA *>  IOTable;        // one entry per IO_Table.csv data row
        map<AnsiString, AnsiString> mapIOTable;
        map<AnsiString, AnsiString>::iterator mapIOTableIter;

        vector <TMOTDATA *> MotTable;       // one entry per Mot_Table.csv data row
        map<AnsiString, AnsiString> mapMotTable;
        map<AnsiString, AnsiString>::iterator mapMotTableIter;

        // -----------------------------------------------------------------------
        //  Deferred methods (database.h:268)
        // -----------------------------------------------------------------------
#if 0 // TODO(wave): ReadGeneralIni -- ~1240 lines, needs full cmydef/cprod surface
        void ReadGeneralIni();
#endif
        void SystemModularInitial();    // real as of W906-SysModWire (wires MyGem) -- see database.cpp

#if 0 // TODO(wave): array members dependent on iTotalFunction / CCD / RFID
      // (database.h:270-286)
        AnsiString asCCDTrayIP[iTotalFunction];
        AnsiString asCCDTrayPort[iTotalFunction];
        AnsiString asFix2BGAAICCDIP[2];
        AnsiString asFix2BGAAICCDPort[2];
        AnsiString asRFIDComPort[2];
        int RFIDBaudRate;
        int RFIDByteSize;
        int RFIDStopBit;
        AnsiString RFIDParity;
        AnsiString asCCDAlignIP[4];
        AnsiString asCCDAlignPort[4];
        AnsiString asCCDBarCodeIP[4];
        AnsiString asCCDBarCodePort[4];
#endif
};

// Global SYSTEM_MODULAR instance (database.cpp:27 / database.h:288)
extern SYSTEM_MODULAR HSys;

#endif // DATABASEH
