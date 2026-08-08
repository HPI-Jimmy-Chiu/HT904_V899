// =============================================================================
//  test_ga1_cprod.cpp -- GA-1-B2 cprod.cpp ungate: behavior tests for 5
//  representative newly-ungated functions.
//
//  Translator: AI(GA1-B2) 20260804
//
//  LINK STRATEGY (self-contained, matches tests/test_ga1_cmydb.cpp precedent):
//  cprod.cpp itself now #includes a wide header surface (common.h/database.h/
//  csystem.h/mycylin.h/mysensor.h/Motor/mymotor.h/Automation/AMR.h/
//  aHotPlateSubstrate.h/canary_support.h/acatchtray_shims.h/forms facades/
//  atester_shims.h/Automation/SCK_ART_Remainder.h/Interface/InterfaceSYS.h).
//  Some of those headers' REAL .cpp counterparts are lightweight and get linked
//  for real below (common.cpp, LastSet.cpp, canary_support.cpp, mycylin.cpp,
//  mysensor.cpp, cmydef.cpp, cpublic.cpp, myTimer.cpp, vclcompat) -- verified,
//  by direct linking, to introduce no further undefined symbols of their own.
//  Others (Motor/mymotor.cpp, Automation/AMR.cpp, atester_shims.cpp,
//  acatchtray_shims.cpp, forms/*.cpp, csystem.cpp, Automation/
//  SCK_ART_Remainder.cpp, Public/HTMD5.cpp) pull their OWN multi-file
//  dependency chains (MyLaneIO/TLaneIO/HTMotor methods, AGV_* predicates,
//  fSCKART, ...) that are unrelated to anything this test exercises -- for
//  those, this file provides minimal "god-stack-free" stand-in definitions of
//  ONLY the specific extern symbols cprod.o's compiled code references
//  (verified empirically by iterative linking, not guessed), exactly the same
//  discipline test_ga1_cmydb.cpp already established for cMyDB.cpp's surface.
//
//  WHY THE STAND-INS ARE SAFE FOR THIS TEST: every stood-in symbol is either
//  (a) on a code path this test's chosen 5 functions never execute (verified:
//  GetSiteCount/EnableFix3UseCylinder/ReadEventLogAutoSaveInfo/
//  SaveEventLogAutoSaveInfo/CheckFileExist/CheckFileCanAccess/RUN_INFO::
//  AddAlarm touch NONE of MOT[]/InArmSuck/FTestSuck/TestSocket/AMR/COM2/
//  TastCategory/fMain/fLotInfo/fAGV/the 7 customer functions), or (b) a value
//  that does not change this test's assertions (fMain/fLotInfo/fAGV as NULL,
//  guarded by != NULL checks at every LIVE call site -- see cprod.cpp; the one
//  exception, CustomerFunctionSelect's unconditional fAGV->IsSPIL_AMR(), is why
//  this test does not call CustomerFunctionSelect()).
//
//  NOT RE-VERIFIED HERE: the 7 customer-function stand-ins (InitialCosFunction/
//  KoreaFunction/VTEST_Funtion/SingaporeFunction/SPILFunction/MaximFunction/
//  SIGURDFunction) are declared in CosFunction.h/cprod.h but have ZERO bodies
//  anywhere in the ported tree (golden CosFunction.cpp has no port yet, a
//  future wave's work) -- see _ga1_b2_report.md. This test's stand-ins let
//  cprod.o LINK; they do not make CustomerFunctionSelect's customer-specific
//  business logic real, and this test does not call it.
//
//  Reproduce (no CMake changes made; standalone manual build, MinGW g++ 6.3.0):
//    cd D:\HT9045\HT9011UC_Cpp_V3.33.906.0
//    g++ -std=c++17 -I . -c cprod.cpp cmydef.cpp cpublic.cpp myTimer.cpp common.cpp \
//        LastSet.cpp canary_support.cpp mycylin.cpp mysensor.cpp
//    g++ -std=c++17 -I . -c vclcompat/AnsiString.cpp vclcompat/TStringList.cpp \
//        vclcompat/TDateTime.cpp vclcompat/SysUtils.cpp vclcompat/IniFiles.cpp \
//        vclcompat/StringGrid.cpp vclcompat/Comm.cpp
//    g++ -std=c++17 -I . -c tests/test_ga1_cprod.cpp -o test_ga1_cprod.o
//    g++ cprod.o cmydef.o cpublic.o myTimer.o common.o LastSet.o canary_support.o \
//        mycylin.o mysensor.o AnsiString.o TStringList.o TDateTime.o SysUtils.o \
//        IniFiles.o StringGrid.o Comm.o test_ga1_cprod.o -lpsapi -lversion \
//        -o test_ga1_cprod.exe
//    ./test_ga1_cprod.exe
// =============================================================================
#include "vclcompat/vcl_compat.h"
#include "cprod.h"
#include "cmydef.h"
#include "common.h"
#include "database.h"
#include "mycylin.h"
#include "mysensor.h"
#include "MyLaneIo.h"
#include "Public/MyProductionRecord.h"
#include "Motor/mymotor.h"
#include "Automation/AMR.h"
#include "aHotPlateSubstrate.h"
#include "canary_support.h"
#include "acatchtray_shims.h"
#include "forms/fMain.h"
#include "forms/fLotInfo.h"
#include "forms/fAGV.h"
#include "atester_shims.h"
#include "Automation/SCK_ART_Remainder.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <io.h>
#include <fcntl.h>

// ---------------------------------------------------------------------------
//  Stand-ins: TMyMotor/TTrayMotor/TMyTray (Motor/mymotor.h + mytray.h) -- only
//  the ctor/dtor + the 2 methods cprod.cpp:1533/:1554 actually references
//  (MOT[MMAuto1].Tray.FullIC(), MOT[...].SetTraySingleData(...), inside
//  ReversionEmptyPoint -- not one of this test's 5 targets, but MOT[] must
//  still link because it is a live global array in cprod.o).
// ---------------------------------------------------------------------------
TMyMotor::TMyMotor() {}
TMyMotor::~TMyMotor() {}
TTrayMotor::TTrayMotor() {}
TMyTray::TMyTray() {}
TMyTray::~TMyTray() {}
bool TMyTray::FullIC() { return false; }
void TTrayMotor::SetTraySingleData(int, int, int, int) {}
TTrayMotor MOT[MAX_TRAY_MOTOR];

// ---------------------------------------------------------------------------
//  Stand-in: TMyKitSuck (aHotPlateSubstrate.h) -- only the ctor (InArmSuck/
//  FTestSuck/TestSocket are live globals in cprod.o; ProcessLastSetIni_Count
//  reads FTestSuck.iShtRow/iShtCol, ARM_OFFSET reads InArmSuck.iMotRow/iMotCol
//  -- zero-initialized here is a safe/valid state for those loop bounds, just
//  not golden's real teach-derived row/col counts).
// ---------------------------------------------------------------------------
TMyKitSuck::TMyKitSuck() {}
TMyKitSuck InArmSuck, FTestSuck, TestSocket;

// ---------------------------------------------------------------------------
//  Stand-ins: TLaneIO/MyLaneIO (MyLaneIo.h) -- needed transitively by
//  mycylin.cpp/mysensor.cpp's own IO-backed methods (Cylinder[]/Sen[]'s real
//  switch/sensor reads), not by anything this test calls directly.
// ---------------------------------------------------------------------------
TLaneIO::TLaneIO() {}
TLaneIO MyLaneIO;
void TLaneIO::IOBitOn(int, int, int, int, int, AnsiString) {}
void TLaneIO::IOBitOff(int, int, int, int, int, AnsiString) {}
bool TLaneIO::IOOutBitStatus(int, int, int, int, int, AnsiString) { return false; }
bool TLaneIO::IOInputBit(int, int, int, int, int, AnsiString) { return false; }

// TMyProductionRecord (Public/MyProductionRecord.h) -- ctor only; a
// by-value member of TMyKitSuck (aHotPlateSubstrate.h).
TMyProductionRecord::TMyProductionRecord() {}

// NewRecordProcess (acatchtray_shims.h 3-arg, S2/S3 defaulted) -- recording stand-in
static int g_NewRecordProcessCount = 0;
void NewRecordProcess(AnsiString, AnsiString, AnsiString) { g_NewRecordProcessCount++; }

// MyDBIProcess (aHotPlateSubstrate.h 2-arg) -- recording stand-in
static AnsiString g_LastMyDBIProcessS1, g_LastMyDBIProcessS2;
static int g_MyDBIProcessCount = 0;
void MyDBIProcess(AnsiString S1, AnsiString S2)
{
    g_LastMyDBIProcessS1 = S1;
    g_LastMyDBIProcessS2 = S2;
    g_MyDBIProcessCount++;
}

// ---------------------------------------------------------------------------
//  Stand-in: TCOM2Shim (atester_shims.h) -- ctor only (matches the real
//  atester_shims.cpp's own documented offline default: bCCDDummyRum=true).
// ---------------------------------------------------------------------------
TCOM2Shim::TCOM2Shim() { bCCDDummyRum = true; }
static TCOM2Shim g_TestCOM2Obj;
TCOM2Shim *COM2 = &g_TestCOM2Obj;

// IsNNMode/ADAM_Rang (atester_shims.h) -- not linking real atester_shims.cpp
int IsNNMode() { return 0; }
void ADAM_Rang(int) {}

// ---------------------------------------------------------------------------
//  Stand-in: TTeraPowerAMR (Automation/AMR.h) -- ctor + the ONE method
//  ATK_RECIPE_INFO::SaveFile references (GetNormalFailBin()); not linking the
//  real Automation/AMR.cpp, which pulls in AGV_IsATK_AMR/AGV_IsSPIL_AMR/
//  AGV_Use_AMR (unrelated Automation-module predicates).
// ---------------------------------------------------------------------------
TTeraPowerAMR::TTeraPowerAMR() {}
AnsiString TTeraPowerAMR::GetNormalFailBin() { return ""; }
TTeraPowerAMR AMR;

// ---------------------------------------------------------------------------
//  Stand-in: TastCategory -- this test's OWN instance, not linking the real
//  (195KB) Automation/SCK_ART_Remainder.cpp.
//  AI(W906-PT-W3-integrate) 20260808: was `W5SckArtRem_TastCategoryStub`, the
//  TU-local stub type SCK_ART_Remainder.h used to declare.  That stub is retired
//  (SCK_ART_Remainder.h gate #16) now that cSocket.cpp defines the real
//  `TEST_CATEGORY TastCategory` -- so this stand-in switches to golden's real
//  type (cSocket.h:144-174).  It is STORAGE ONLY: cprod.cpp's paths under test
//  read fields, never call ClearCount/UpdataCount/UpdataYield, so no cSocket.cpp
//  link edge is needed here (verified: no such call on this test's 5 call paths).
// ---------------------------------------------------------------------------
TEST_CATEGORY TastCategory = {};

// ---------------------------------------------------------------------------
//  fMain/fLotInfo/fAGV -- NULL. Every LIVE (ungated) dereference in cprod.cpp
//  is guarded by a "!= NULL" check EXCEPT CustomerFunctionSelect's
//  unconditional fAGV->IsSPIL_AMR() -- this test does not call
//  CustomerFunctionSelect(), so that single exception is never reached.
// ---------------------------------------------------------------------------
TfMain *fMain = NULL;
TfLotInfo *fLotInfo = NULL;
TfAGV *fAGV = NULL;

// HasICUnderMachine/HasAnyICInMachine (csystem.h) -- not linking real
// csystem.cpp (267KB); not on any of this test's 5 call paths.
bool HasICUnderMachine() { return false; }
bool HasAnyICInMachine() { return false; }

// Customer-function group (CosFunction.h) -- golden CosFunction.cpp has NO
// port anywhere in this tree (a future wave's work; see _ga1_b2_report.md).
// Stand-ins here ONLY so cprod.o links; this test does not call
// CustomerFunctionSelect() (the one live caller of these 7).
void InitialCosFunction() {}
void KoreaFunction() {}
void VTEST_Funtion() {}
void SingaporeFunction() {}
void SPILFunction() {}
void MaximFunction() {}
void SIGURDFunction() {}

// SearchFile/md5_Folder (Public/HTMD5.h) -- needed transitively by cpublic.o's
// own (unrelated) checksum feature, not by anything this test calls.
int SearchFile(AnsiString, AnsiString, TStrings*) { return 0; }
AnsiString md5_Folder(AnsiString) { return ""; }

// UseCanBusOrEtherCAT (CCLink/MyCCLinkSensor_predicates.h) -- needed
// transitively by cpublic.o's own (unrelated) code.
bool UseCanBusOrEtherCAT() { return false; }

// SystemNG -- golden main-side global, no translated home anywhere in this
// tree (acarry_shims.h's own note); canary_support.cpp's ClearAllAlarm writes
// it. Offline default false.
bool SystemNG = false;

// ---------------------------------------------------------------------------
//  Minimal CHECK harness (matches tests/test_ga1_lastset.cpp precedent)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg) do { \
        if (cond) { g_pass++; } \
        else { g_fail++; std::printf("FAIL: %s (line %d)\n", msg, __LINE__); } \
    } while (0)

static AnsiString ScratchDir()
{
    return AnsiString("test_ga1_cprod_scratch\\");
}

// =============================================================================
//  Test 1 -- GetSiteCount(bool): seed TestIF_File.iTestMode, verify the
//  site-count table across several real golden test modes (fully ungated,
//  zero narrow gates -- pure calculation over the live TestIF_File global).
// =============================================================================
static void Test_GetSiteCount()
{
    TestIF_File.iTestMode = SingleSite;
    CHECK(GetSiteCount(true) == 1, "GetSiteCount SingleSite -> 1");

    TestIF_File.iTestMode = DualSite;
    CHECK(GetSiteCount(true) == 2, "GetSiteCount DualSite -> 2");

    TestIF_File.iTestMode = QualSite2X2;
    CHECK(GetSiteCount(true) == 4, "GetSiteCount QualSite2X2 -> 4");

    TestIF_File.iTestMode = _8Site2X4;
    CHECK(GetSiteCount(true) == 8, "GetSiteCount _8Site2X4 -> 8");

    TestIF_File.iTestMode = _16Site2X8;
    CHECK(GetSiteCount(true) == 16, "GetSiteCount _16Site2X8 -> 16");

    TestIF_File.iTestMode = _32Site4X8M;
    CHECK(GetSiteCount(true) == 32, "GetSiteCount _32Site4X8M -> 32");

    // IncludeCloseSite==false path: counts iSiteMap[][] entries >0, independent
    // of iTestMode.
    for (int i = 0; i < MAX_SOCKET_ROW; i++)
        for (int j = 0; j < MAX_SOCKET_COL; j++)
            TestIF_File.iSiteMap[i][j] = 0;
    TestIF_File.iSiteMap[0][0] = 1;
    TestIF_File.iSiteMap[0][1] = 2;
    TestIF_File.iSiteMap[1][0] = -1;                 // closed site, golden counts only >0
    CHECK(GetSiteCount(false) == 2, "GetSiteCount(false) counts only iSiteMap>0 entries");
}

// =============================================================================
//  Test 2 -- EnableFix3UseCylinder(): seed AUTO_EMPTY_COLOR/FIX3_FULL_PLACE,
//  verify both the early-return branch and each of the 3 "true" cylinder/
//  stepper-motor types (fully ungated, zero narrow gates).
// =============================================================================
static void Test_EnableFix3UseCylinder()
{
    int savedAEC = AUTO_EMPTY_COLOR;
    int savedF3P = FIX3_FULL_PLACE;

    AUTO_EMPTY_COLOR = 3;   // >=3 short-circuits to false regardless of FIX3_FULL_PLACE
    FIX3_FULL_PLACE = Fix3K_UseCylinder;
    CHECK(EnableFix3UseCylinder() == false, "EnableFix3UseCylinder: AUTO_EMPTY_COLOR>=3 -> false");

    AUTO_EMPTY_COLOR = 0;
    FIX3_FULL_PLACE = Fix3K_Uninstall;
    CHECK(EnableFix3UseCylinder() == false, "EnableFix3UseCylinder: Fix3K_Uninstall -> false");

    FIX3_FULL_PLACE = Fix3K_UseCylinder;
    CHECK(EnableFix3UseCylinder() == true, "EnableFix3UseCylinder: Fix3K_UseCylinder -> true");

    FIX3_FULL_PLACE = Fix3K_UseCylinder46LA;
    CHECK(EnableFix3UseCylinder() == true, "EnableFix3UseCylinder: Fix3K_UseCylinder46LA -> true");

    FIX3_FULL_PLACE = Fix3K_UseStepperMotor;
    CHECK(EnableFix3UseCylinder() == true, "EnableFix3UseCylinder: Fix3K_UseStepperMotor -> true");

    AUTO_EMPTY_COLOR = savedAEC;
    FIX3_FULL_PLACE = savedF3P;
}

// =============================================================================
//  Test 3 -- SaveEventLogAutoSaveInfo() / ReadEventLogAutoSaveInfo(): the
//  GA-1-B2 priority-3 function, fully ungated (zero narrow gates). Seeds
//  IniConfig fields, saves to a real scratch .ini file, then reads it back and
//  verifies the round trip -- including the FormatDateTime mechanical fix
//  (golden .FormatString) and the "empty -> fall back 1 day" branch.
// =============================================================================
static void Test_EventLogAutoSaveInfo()
{
    AnsiString savedAuthPath = AuthPath;
    AuthPath = ScratchDir();
    MyForceDirectories(AuthPath);
    AnsiString iniPath = AuthPath + "config.ini";
    std::remove(iniPath.c_str());

    IniConfig.bEventLogAutoSaveFunction = true;
    IniConfig.dtEventLogLastRecordDate = TDateTime(45000.5);   // arbitrary fixed date+time

    SaveEventLogAutoSaveInfo();
    CHECK(FileExists(iniPath), "SaveEventLogAutoSaveInfo wrote config.ini");
    AnsiString savedStr = IniConfig.sEvenLogDataTime;
    CHECK(savedStr.Length() == 19, "SaveEventLogAutoSaveInfo formatted 'yyyy/mm/dd hh:mm:ss' (19 chars)");

    // Corrupt the in-memory value, then verify ReadEventLogAutoSaveInfo restores
    // it from the file just written (round trip through the real ini file).
    IniConfig.sEvenLogDataTime = "";
    ReadEventLogAutoSaveInfo();
    CHECK(IniConfig.sEvenLogDataTime == savedStr,
          "ReadEventLogAutoSaveInfo round-trips SaveEventLogAutoSaveInfo's exact string");
    // dtEventLogLastRecordDate is reconstructed via SystemTimeToDateTime(SysTime)
    // from the parsed string -- verify it lands within the same calendar day/
    // hour:minute:second as the original seed (round-trip through GetLocalTime's
    // real Win32 struct + integer SubString parses, so sub-second precision is
    // not preserved -- only compare the string forms, both already asserted equal
    // above, plus a sanity range check that the reconstructed date is not the
    // TDateTime(0) failure value).
    CHECK(IniConfig.dtEventLogLastRecordDate.Val() > 40000.0,
          "ReadEventLogAutoSaveInfo reconstructed a real (non-zero-epoch) TDateTime");

    // Empty/NULL fallback branch: CheckAndReadIniData on a missing key falls
    // back to SDate = FormatDateTime(fmt, Now()-1) -- verify it is non-empty and
    // correctly formatted (also exercises the Now()-1 ambiguity fix).
    std::remove(iniPath.c_str());
    IniConfig.sEvenLogDataTime = "";
    ReadEventLogAutoSaveInfo();
    CHECK(IniConfig.sEvenLogDataTime.Length() == 19,
          "ReadEventLogAutoSaveInfo falls back to a well-formed 'yesterday' string when the ini has no prior value");

    std::remove(iniPath.c_str());
    AuthPath = savedAuthPath;
}

// =============================================================================
//  Test 4 -- CheckFileExist(AnsiString) / CheckFileCanAccess(char*): fully
//  ungated (the _rtl_open/_rtl_close/access BCB6-RTL-internal mechanical
//  translation to open/close/access). Real temp-file round trip: missing file,
//  existing readable file, and (Windows-permitting) a locked/exclusive file.
// =============================================================================
static void Test_CheckFileExist()
{
    AnsiString dir = ScratchDir();
    MyForceDirectories(dir);
    AnsiString path = dir + "checkfile_test.txt";
    std::remove(path.c_str());

    CHECK(CheckFileExist(path) == false, "CheckFileExist: missing file -> false");

    std::FILE *fp = std::fopen(path.c_str(), "w");
    CHECK(fp != NULL, "(setup) temp file created");
    if (fp) { std::fputs("hello", fp); std::fclose(fp); }

    CHECK(CheckFileExist(path) == true, "CheckFileExist: existing readable file -> true");

    char pathBuf[512];
    std::strncpy(pathBuf, path.c_str(), sizeof(pathBuf) - 1);
    pathBuf[sizeof(pathBuf) - 1] = '\0';
    CHECK(CheckFileCanAccess(pathBuf) == true, "CheckFileCanAccess: existing file opens O_RDWR -> true");

    std::remove(path.c_str());
    CHECK(CheckFileCanAccess(pathBuf) == false, "CheckFileCanAccess: missing file -> false");
}

// =============================================================================
//  Test 5 -- RUN_INFO::AddAlarm(AnsiString,AnsiString): fully ungated. Seeds a
//  fresh RunInfo state and verifies the by-lot JAM_COUNT map (vByLotJam)
//  correctly creates-then-increments per JamCode, and ignores non-"JAM"-coded
//  messages (golden's `if(Jam.AnsiPos("JAM")!=0)` guard).
// =============================================================================
static void Test_RunInfoAddAlarm()
{
    RunInfo.vByLotJam.clear();
    RunInfo.vDailyJam.clear();

    RunInfo.AddAlarm("JAM0301", "Loader jam");
    CHECK(RunInfo.vByLotJam.count("JAM0301") == 1, "AddAlarm: first JAM0301 creates a vByLotJam entry");
    CHECK(RunInfo.vByLotJam["JAM0301"].iCount == 1, "AddAlarm: first JAM0301 has iCount==1");
    CHECK(RunInfo.vByLotJam["JAM0301"].Message == "Loader jam", "AddAlarm: Message captured verbatim");

    RunInfo.AddAlarm("JAM0301", "Loader jam");
    CHECK(RunInfo.vByLotJam["JAM0301"].iCount == 2, "AddAlarm: second JAM0301 increments to iCount==2");

    RunInfo.AddAlarm("JAM0560", "Shuttle jam");
    CHECK(RunInfo.vByLotJam.count("JAM0560") == 1, "AddAlarm: a different JamCode gets its own entry");
    CHECK(RunInfo.vByLotJam.size() == 2, "AddAlarm: exactly 2 distinct JamCodes tracked");

    RunInfo.AddAlarm("WAR16132", "not a JAM code");
    CHECK(RunInfo.vByLotJam.count("WAR16132") == 0,
          "AddAlarm: golden's AnsiPos(\"JAM\")!=0 guard ignores non-JAM-coded messages");
}

int main()
{
    Test_GetSiteCount();
    Test_EnableFix3UseCylinder();
    Test_EventLogAutoSaveInfo();
    Test_CheckFileExist();
    Test_RunInfoAddAlarm();

    std::printf("%d/%d checks passed (test_ga1_cprod)\n", g_pass, g_pass + g_fail);
    return g_fail == 0 ? 0 : 1;
}
