// =============================================================================
//  test_observer_core.cpp -- FW-3 cObserver Wave 1 test coverage
//
//  AI(W906-FW3-Observer-W1) 20260818: new file.
//
//  Covers (per this wave's task brief):
//   (a) CalculateStopTime/UnCalculateStopTime -- round-trip symmetry AND
//       known literal values, for both the <=60-minute and >60-minute
//       branches.
//   (b) GetEventLogText's 3 filter kinds (JAM/WAR/MES prefix filter,
//       UnitName substring filter, "show all") + the file-not-found ->
//       "No Record!!" branch. Drives it through cbbMonthChange (the real
//       call path, not a hand-rolled shortcut) so the W906_EVENTLOG_ROOT
//       redirect (cObserver.cpp, this wave) is exercised end to end, against
//       a scratch CSV this test writes itself (quoted fields, an embedded
//       comma AND embedded double-space, verifying vclcompat::TStringList's
//       CommaText parser does not shift columns on either).
//   (c) One BtnQueryClick eQueryType branch's SQL text (Alarm_History),
//       via the PORT-ONLY BuildWhereQuery_OccurDateTimeRange/
//       BuildQuery_AlarmHistory extraction (forms/fObserver.h) -- NOT by
//       calling BtnQueryClick itself (its real MyDBVProcess/MyDBVEventFreq
//       calls sqlite3_get_table(dbReadOnly,...) unconditionally with no
//       NULL-handle guard -- cMyDB.cpp:1584 -- so invoking it here would
//       need a real, opened SQLite handle just to avoid a crash; not this
//       wave's job, and NOT cMyDB mock/behaviour-fake either way).
//
//  SAFETY -- constructing a real TfObserver touches TWO global config paths
//  --------------------------------------------------------------------------
//  TfObserver's ctor (golden :137-346) calls GetObserAuth() (reads/seeds
//  D:\HT9045\config\Security_new.def via the global `AuthPath`) and
//  CheckAndReadIniDataGeneral() (reads/seeds D:\HT9045\system\Gerneral.ini
//  via the global `INIFileGeneral`, opened from `asGeneralPath`) -- BOTH
//  hardcoded production paths (common.cpp:89/:102). `INIFileGeneral` starts
//  NULL and CheckAndReadIniDataGeneral operates on it with NO NULL check
//  (common.cpp:1478), so constructing a TfObserver WITHOUT first opening it
//  would crash, not silently write. This test redirects BOTH globals to a
//  scratch directory before constructing anything, restores them in main()'s
//  teardown -- the SAME idiom tests/test_ga1_cprod.cpp already established
//  for `AuthPath` (its own ScratchDir()/MyForceDirectories precedent,
//  :322-325), extended here to `asGeneralPath`/OpenGeneralIniFile() (both
//  `extern`-declared in common.h, common.h:60/:71/:213) for the second path.
// =============================================================================
#include "forms/fObserver.h"
#include "common.h"        // AuthPath, asGeneralPath, OpenGeneralIniFile/CloseGeneralIniFile, MyForceDirectories
#include "cmydef.h"        // CUSTOMER_CODE
#include "Config.h"        // IniConfig (bSPILFunction)

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__MINGW32__)
// AI(W906-FW0) 20260817 precedent (tests/test_bootstrap.cpp:65-67): -std=c++17
// defines __STRICT_ANSI__, which hides MinGW's _putenv declaration.
extern "C" int _putenv(const char *);
#endif

// ---------------------------------------------------------------------------
//  Minimal CHECK harness (matches tests/test_ga1_cprod.cpp precedent)
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;
#define CHECK(cond, msg) do { \
        if (cond) { g_pass++; } \
        else { g_fail++; std::printf("FAIL: %s (line %d)\n", msg, __LINE__); } \
    } while (0)

static AnsiString ScratchDir()
{
    return AnsiString("test_observer_core_scratch\\");
}

// =============================================================================
//  (a) CalculateStopTime / UnCalculateStopTime
// =============================================================================
static void Test_CalculateStopTime_KnownValuesAndRoundTrip()
{
    // <=60-minute branch (golden's `else` arm: "0:MM:S", MM zero-padded, S NOT).
    CHECK(TfObserver::CalculateStopTime(0) == "0:00:0", "CalculateStopTime(0) == \"0:00:0\"");
    CHECK(TfObserver::CalculateStopTime(125) == "0:02:5", "CalculateStopTime(125) == \"0:02:5\" (2 min 5 sec)");
    CHECK(TfObserver::CalculateStopTime(59) == "0:00:59", "CalculateStopTime(59) == \"0:00:59\" (0 min 59 sec)");

    // >60-minute branch (golden's `if(i_mm>60)` arm: "H:MM:S", note i_mm==60
    // exactly (60 minutes on the nose) does NOT take this branch -- golden's
    // own `>` not `>=`, translated verbatim, not "fixed").
    CHECK(TfObserver::CalculateStopTime(3725) == "1:02:5", "CalculateStopTime(3725) == \"1:02:5\" (1hr 2min 5sec)");
    // AI(W906-FW3-Observer-W1) 20260818: CalculateStopTime(3600): i_mm=60,
    // golden's condition is `i_mm>60` (strictly greater), so exactly 60
    // minutes does NOT take the hh:mm:ss arm -- it falls into the `else`
    // arm, which formats i_mm (60) as mm directly ("60", already 2 chars, no
    // padding needed) with a literal "0:" hour prefix: "0:60:0". This is
    // golden's own apparent-but-harmless quirk (60 minutes displays as
    // "0:60:0" instead of "1:00:0") -- translated verbatim, documented in
    // cObserver.cpp is not needed since it falls straight out of the
    // >-vs->= comparison already called out there for the CalculateStopTime
    // translation. Asserted here as a KNOWN VALUE, not "fixed".
    CHECK(TfObserver::CalculateStopTime(3600) == "0:60:0", "CalculateStopTime(3600) == \"0:60:0\" (i_mm==60 is NOT >60 -- golden's own quirk, verbatim)");

    // Round-trip: UnCalculateStopTime(CalculateStopTime(Sec)) == Sec, both branches.
    int samples[] = { 0, 5, 59, 60, 61, 125, 3599, 3600, 3601, 3725, 7325 };
    for (size_t i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i)
    {
        int sec = samples[i];
        AnsiString formatted = TfObserver::CalculateStopTime(sec);
        int roundTripped = TfObserver::UnCalculateStopTime(formatted);
        CHECK(roundTripped == sec, "round-trip UnCalculateStopTime(CalculateStopTime(Sec)) == Sec");
    }

    // golden :2241 `if(Time=="") return 0;`
    CHECK(TfObserver::UnCalculateStopTime("") == 0, "UnCalculateStopTime(\"\") == 0 (empty-string guard)");
}

// =============================================================================
//  (c) BtnQueryClick's Alarm_History branch SQL text, via the PORT-ONLY
//      extraction (see forms/fObserver.h banner on these two methods).
// =============================================================================
static void Test_BuildQuery_AlarmHistory()
{
    TDateTime d1 = EncodeDate(2026, 1, 1);
    TDateTime t1 = EncodeTime(0, 0, 0, 0);
    TDateTime d2 = EncodeDate(2026, 12, 31);
    TDateTime t2 = EncodeTime(23, 59, 59, 0);

    AnsiString whereQuery = TfObserver::BuildWhereQuery_OccurDateTimeRange(d1, t1, d2, t2);
    CHECK(whereQuery == " WHERE (OccurDateTime >= '2026-01-01 00:00:00' AND OccurDateTime <= '2026-12-31 23:59:59')",
          "BuildWhereQuery_OccurDateTimeRange builds golden's exact WHERE clause text");

    AnsiString asQuery = TfObserver::BuildQuery_AlarmHistory(whereQuery);
    AnsiString expected = AnsiString("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, "
                                      "DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature "
                                      "FROM AlarmHistoryView") + " " + whereQuery + " ORDER BY OccurDateTime DESC";
    CHECK(asQuery == expected, "BuildQuery_AlarmHistory (golden's Alarm_History eQueryType branch) builds the exact SELECT text");
}

// =============================================================================
//  (b) GetEventLogText -- 3 filter kinds + file-not-found, via cbbMonthChange
//      (real call path) against a scratch CSV + W906_EVENTLOG_ROOT redirect.
// =============================================================================
static void WriteScratchCsv(const AnsiString &path)
{
    std::FILE *fp = std::fopen(path.c_str(), "wb");
    CHECK(fp != NULL, "(setup) scratch EventLogTxt CSV created");
    if (!fp) return;
    // 9-column non-SPIL layout (golden ctor's strngrdEventLog->ColWidths[0..8]
    // comments: Date,Time,UnitName,AlarmCode,Recovery,StopedTime,Duplicate,
    // Message,ErrPart). Row 0 is a header row (every filter branch starts its
    // scan at i=1, skipping it). Two Message fields are deliberately
    // CSV-quoted: one with an embedded COMMA (the classic column-shift
    // hazard), one with an embedded DOUBLE SPACE (quoted even though not
    // strictly required, to confirm vclcompat::TStringList's CommaText
    // parser accepts unnecessary quoting too).
    const char *rows =
        "Date,Time,UnitName,AlarmCode,Recovery,StopedTime,Duplicate,Message,ErrPart\r\n"
        "2026-08-18,09:00:00,InArm,JAM0301,1,12,0,\"Loader, jam near site A\",InArm1\r\n"
        "2026-08-18,09:05:00,OutArm,WAR16102,1,3,0,\"Clean pad worn out\",OutArm2\r\n"
        "2026-08-18,09:10:00,Shuttle,MES0101,1,5,0,\"Barcode read fail  extra space\",Shuttle1\r\n"
        "2026-08-18,09:15:00,Index,ALM0001,1,8,0,\"Generic alarm ignored by JAM WAR MES\",Index1\r\n";
    std::fputs(rows, fp);
    std::fclose(fp);
}

static void Test_GetEventLogText_FiltersAndNotFound()
{
    AnsiString root = ScratchDir() + "eventlog_root";
    AnsiString yearDir = root + "\\2026";
    AnsiString monthDir = yearDir + "\\08";
    MyForceDirectories(monthDir);
    AnsiString csvPath = monthDir + "\\sample.csv";
    WriteScratchCsv(csvPath);

    int savedCustomerCode = CUSTOMER_CODE;
    bool savedSpil = IniConfig.bSPILFunction;
    CUSTOMER_CODE = 0;                    // not CC_SINOICTECH -- plain full-path branch in GetEventLogText
    IniConfig.bSPILFunction = false;      // non-SPIL 9-column layout (matches the CSV header above)

    AnsiString savedEventLogRootEnv = getenv("W906_EVENTLOG_ROOT") ? AnsiString(getenv("W906_EVENTLOG_ROOT")) : AnsiString("");
    bool hadEventLogRootEnv = getenv("W906_EVENTLOG_ROOT") != NULL;
    AnsiString envAssign = AnsiString("W906_EVENTLOG_ROOT=") + root;
    _putenv(envAssign.c_str());

    {
        TfObserver observer;
        observer.cbbEventLogYear->Text = "2026";
        observer.cbbMonth->Text = "08";

        // --- baseline: cbbMonthChange search-populates lstEventLog, sets
        //     cbbFilter->ItemIndex=0 ("show all"), and calls GetEventLogText
        //     itself (golden :3796) -- exercises the W906_EVENTLOG_ROOT
        //     redirect end to end, not a hand-set lstEventLog shortcut.
        observer.cbbMonthChange(NULL);
        CHECK(observer.lstEventLog->Items->Count == 1, "(setup) SearchFileAll found exactly the 1 scratch CSV under W906_EVENTLOG_ROOT");
        CHECK(observer.strngrdEventLog->RowCount == 5, "GetEventLogText (show all): RowCount == 5 (1 header + 4 data rows)");
        CHECK(observer.strngrdEventLog->Cells[3][1] == "JAM0301", "show-all: row 1 AlarmCode column not shifted by the quoted-comma Message field");
        CHECK(observer.strngrdEventLog->Cells[7][1] == "Loader, jam near site A", "show-all: row 1 Message field (embedded comma) parsed verbatim, not truncated at the comma");
        CHECK(observer.strngrdEventLog->Cells[8][1] == "InArm1", "show-all: row 1 ErrPart column (the field AFTER the quoted Message) not shifted");
        CHECK(observer.strngrdEventLog->Cells[7][3] == "Barcode read fail  extra space", "show-all: row 3 Message field (embedded double-space, unnecessarily quoted) parsed verbatim");

        // --- JAM only filter (iJamCol=3, AlarmCode, prefix match) ----------
        observer.cbbFilter->ItemIndex = 1;
        observer.cbbFilter->Text = "JAM only";
        observer.GetEventLogText();
        CHECK(observer.strngrdEventLog->RowCount == 3, "JAM only filter: RowCount == 3 (2 fixed + 1 match: JAM0301)");
        CHECK(observer.strngrdEventLog->Cells[3][1] == "JAM0301", "JAM only filter: the one matching row's AlarmCode is JAM0301");
        CHECK(observer.strngrdEventLog->Cells[7][1] == "Loader, jam near site A", "JAM only filter: matching row's Message column not shifted by its embedded comma");

        // --- WAR only filter -------------------------------------------------
        observer.cbbFilter->ItemIndex = 2;
        observer.cbbFilter->Text = "WAR only";
        observer.GetEventLogText();
        CHECK(observer.strngrdEventLog->RowCount == 3, "WAR only filter: RowCount == 3 (2 fixed + 1 match: WAR16102)");
        CHECK(observer.strngrdEventLog->Cells[3][1] == "WAR16102", "WAR only filter: the one matching row's AlarmCode is WAR16102");

        // --- MES only filter -------------------------------------------------
        observer.cbbFilter->ItemIndex = 3;
        observer.cbbFilter->Text = "MES only";
        observer.GetEventLogText();
        CHECK(observer.strngrdEventLog->RowCount == 3, "MES only filter: RowCount == 3 (2 fixed + 1 match: MES0101)");
        CHECK(observer.strngrdEventLog->Cells[3][1] == "MES0101", "MES only filter: the one matching row's AlarmCode is MES0101");

        // --- UnitName filter (the "else" branch, iJamCol=2, substring match) -
        observer.cbbFilter->ItemIndex = 4;
        observer.cbbFilter->Text = "Shuttle";
        observer.GetEventLogText();
        CHECK(observer.strngrdEventLog->RowCount == 3, "UnitName filter \"Shuttle\": RowCount == 3 (2 fixed + 1 match)");
        CHECK(observer.strngrdEventLog->Cells[2][1] == "Shuttle", "UnitName filter: the one matching row's UnitName is Shuttle");
        CHECK(observer.strngrdEventLog->Cells[3][1] == "MES0101", "UnitName filter: matched row is the Shuttle/MES0101 one, not shifted");

        // --- file-not-found -> "No Record!!" ----------------------------------
        observer.lstEventLog->Clear();
        observer.lstEventLog->Items->Add(root + "\\2026\\08\\does_not_exist.csv");
        observer.lstEventLog->ItemIndex = 0;
        observer.cbbFilter->ItemIndex = 0;
        observer.GetEventLogText();
        CHECK(observer.strngrdEventLog->RowCount == 2, "file-not-found: RowCount == 2 (the fixed \"No Record!!\" shape)");
        CHECK(observer.strngrdEventLog->Cells[1][1] == "No Record!!", "file-not-found: Cells[1][1] == \"No Record!!\"");

        // --- empty lstEventLog -> "No Record!!" (golden's OTHER no-record path) -
        observer.lstEventLog->Clear();
        observer.lstEventLog->ItemIndex = -1;
        observer.GetEventLogText();
        CHECK(observer.strngrdEventLog->RowCount == 2, "empty lstEventLog: RowCount == 2");
        CHECK(observer.strngrdEventLog->Cells[1][1] == "No Record!!", "empty lstEventLog: Cells[1][1] == \"No Record!!\"");
    }

    if (hadEventLogRootEnv)
    {
        AnsiString restore = AnsiString("W906_EVENTLOG_ROOT=") + savedEventLogRootEnv;
        _putenv(restore.c_str());
    }
    else
    {
        _putenv("W906_EVENTLOG_ROOT=");   // MinGW/MSVC _putenv: "NAME=" removes the variable
    }
    CUSTOMER_CODE = savedCustomerCode;
    IniConfig.bSPILFunction = savedSpil;
}

int main()
{
    // --- one-time safety redirect: BOTH production config paths TfObserver's
    //     ctor touches, BEFORE constructing anything -- see file-head SAFETY
    //     note. `AuthPath` follows tests/test_ga1_cprod.cpp's own established
    //     precedent (:322-325); `asGeneralPath`/OpenGeneralIniFile is the
    //     analogous extension for INIFileGeneral, using the same extern
    //     globals + public open function common.h already provides.
    AnsiString savedAuthPath = AuthPath;
    AnsiString savedGeneralPath = asGeneralPath;
    AuthPath = ScratchDir();
    MyForceDirectories(AuthPath);
    asGeneralPath = ScratchDir() + "general_test.ini";
    OpenGeneralIniFile();

    Test_CalculateStopTime_KnownValuesAndRoundTrip();
    Test_BuildQuery_AlarmHistory();
    Test_GetEventLogText_FiltersAndNotFound();

    CloseGeneralIniFile();
    AuthPath = savedAuthPath;
    asGeneralPath = savedGeneralPath;

    std::printf("%d/%d checks passed (test_observer_core)\n", g_pass, g_pass + g_fail);
    return g_fail == 0 ? 0 : 1;
}
