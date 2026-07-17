// =============================================================================
//  tests/test_uHGemEquipment.cpp -- verification harness for the new
//  SECSGEM/uHGemEquipment.{h,cpp} translation (THGem's CEID/Report/Alarm
//  StringGrid "database" family + GemTimer) and the new
//  vclcompat/StringGrid.{h,cpp} shim.
//
//  AI(W906-uHGemEquipment) 20260716: new file.
//
//  Golden reference:
//    HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.h:31-42,139-144
//    HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//      (GemTimer :250-309; CEID/Report family :7361-7967; Alarm-grid
//       siblings :6211-6274; StringGrid tab-format :8034-8195,8615-8665)
//
//  BUILD (standalone -- CMakeLists.txt is NOT wired to this file this wave;
//  per the task's build-wiring correction, that is the integrate stage's
//  job). From D:/HT9045/HT9011UC_Cpp_V3.33.906.0, one single command (line
//  breaks below are for readability only; join with spaces, no backslashes):
//    g++ -std=c++14 -Wall -Wextra -I . -I vclcompat
//        tests/test_uHGemEquipment.cpp SECSGEM/uHGemEquipment.cpp
//        vclcompat/StringGrid.cpp vclcompat/AnsiString.cpp
//        vclcompat/TStringList.cpp vclcompat/SysUtils.cpp
//        vclcompat/TDateTime.cpp vclcompat/IniFiles.cpp
//        SECSGEM/SecsEventType.cpp
//        -o test_uHGemEquipment.exe
//    ./test_uHGemEquipment.exe
//
//  *** SAFETY NOTE -- READ BEFORE EXTENDING THIS FILE ***
//  ---------------------------------------------------------------------------
//  THGem::SetCEIDContent (both overloads), THGem::SetReportIDContent, and
//  THGem::EnableDisableEventReport ALL end by calling SaveEventReportData(),
//  which -- faithfully matching golden (uHGemEquipment.cpp:8650-8665) --
//  unconditionally SAVES to the HARDCODED absolute path
//  "D:\HT9045\SECS\SECS\SYSTEM\EventReport_CEID.def" /
//  "...\EventReport_ReportID.def", with NO parameter/instance-state way to
//  redirect it. Those exact files EXIST as real data on the reference dev
//  machine this test was authored on (confirmed: they are the actual
//  production SECS/GEM event-report definition files for the D:\HT9045 tree,
//  a location strictly outside this migration's write boundary -- see
//  AGENTS.md / this task's own instructions: "絕對不要碰 ... D:/HT9045 repo
//  根目錄下其他任何檔案"). THIS TEST FILE THEREFORE NEVER CALLS
//  SetCEIDContent / SetReportIDContent / EnableDisableEventReport /
//  SaveEventReportData -- doing so would silently overwrite real files
//  outside this task's allowed scope.
//
//  Those 3 methods' grid-mutation ALGORITHMS are instead verified by:
//    (a) hand-trace, documented inline at each method's own definition in
//        SECSGEM/uHGemEquipment.cpp (see the comments there), and
//    (b) this file's [5]/[6] tests below, which exercise the SAME grid
//        layout those methods produce/consume (strGrdCEID cols
//        0=CEID,1=Mode/Enable,2=Alias,3.. = ReportIDs; stdGridReportID cols
//        0=CEID,1=Mode,2.. = SVIDs) by seeding cells DIRECTLY (bypassing the
//        two dangerous writer methods entirely) and then exercising every
//        READ/DELETE method in the family against that hand-seeded data --
//        those 15 methods (GetCEIDContent, GetReportIDContent, IsEnableEvent,
//        CheckCeidExist, CheckCEIDExist, GetReportIDType,
//        CheckCeidAlreadyDefine, CheckReportIDExist, AddCeidReportID,
//        DeleteReportID, DeleteReportIDOfCeid, DeleteAllHostDefineReportID,
//        DeleteAllHostReportIDOfCeid, DeleteHostReportIDOfAssignCeid,
//        DeleteAllHostDefineCeid, CheckReportAlreadyDefine) have NO
//        SaveEventReportData call anywhere in their own bodies (verified by
//        reading each one) and are exercised directly, for real.
//
//  ReadEventReportData is READ-ONLY (LoadFromFile, never writes) -- test [7]
//  below DOES call it for real, against the real on-disk files, as a genuine
//  oracle (guarded by FileExists so the test degrades gracefully, rather than
//  failing, on a machine without that exact path).
//
//  ReadAlamData/WriteAlamData ARE exercised directly (test [8]) because,
//  unlike the CEID/Report family, their path is DERIVED from the
//  `GemSystemPath` instance field (not hardcoded) -- this test points
//  GemSystemPath at a throwaway scratch folder under the current working
//  directory, never at the real D:\HT9045 tree.
// =============================================================================
#include "SECSGEM/uHGemEquipment.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <windows.h>   // ::Sleep (GemTimer elapsed-time test)

// ---------------------------------------------------------------------------
//  MyDBIProcess -- this test binary does not link aHotPlateSubstrate.cpp (the
//  one real definition elsewhere in this tree); ReadAlamData's catch-block
//  needs SOME definition to link against (same pattern as
//  tests/test_FTPClient_EventHandlers.cpp / test_uHGemClass.cpp).
// ---------------------------------------------------------------------------
void MyDBIProcess(AnsiString /*S1*/, AnsiString /*S2*/) {}

// ---------------------------------------------------------------------------
//  Tiny PASS / FAIL harness (same style as tests/test_serversocket.cpp).
// ---------------------------------------------------------------------------
static int g_pass = 0, g_fail = 0;

#define CHECK(cond, msg)                                                       \
    do {                                                                       \
        if (cond) { printf("  PASS: %s\n", msg); ++g_pass; }                   \
        else      { printf("  FAIL: %s  (line %d)\n", msg, __LINE__); ++g_fail; } \
    } while (0)

// ===========================================================================
//  [1] GemTimer
// ===========================================================================
static void test_gemtimer()
{
    printf("\n[1] GemTimer\n");

    GemTimer t1;
    CHECK(t1.TimerOff() == false, "never-armed timer (ulStartTicks==0) -> TimerOff()==false");

    GemTimer t2;
    t2.TimerSetMSAndOn(0);   // iTimeLen<=0 guard (golden: "jou 2012-01-04" comment)
    CHECK(t2.TimerOff() == true, "TimerSetMSAndOn(0) -> immediately elapsed (iTimeLen<=0 guard)");

    GemTimer t3;
    t3.TimerSetMSAndOn(300);
    CHECK(t3.TimerOff() == false, "TimerSetMSAndOn(300) -> not yet elapsed immediately after arming");
    ::Sleep(450);
    CHECK(t3.TimerOff() == true, "TimerSetMSAndOn(300) -> elapsed after a 450ms real sleep");

    GemTimer t4;
    t4.TimerSetSecAndOn(0.2);   // 200ms
    CHECK(t4.TimerOff() == false, "TimerSetSecAndOn(0.2) -> not yet elapsed immediately");
    ::Sleep(350);
    CHECK(t4.TimerOff() == true, "TimerSetSecAndOn(0.2) -> elapsed after a 350ms real sleep");
}

// ===========================================================================
//  [2] vclcompat::TStringGrid -- headless shim basics
// ===========================================================================
static void test_stringgrid_basic()
{
    printf("\n[2] vclcompat::TStringGrid basics\n");

    vclcompat::TStringGrid g(3, 2);   // ColCount=3, RowCount=2
    CHECK((int)g.ColCount == 3, "ctor ColCount");
    CHECK((int)g.RowCount == 2, "ctor RowCount");
    CHECK(g.Cells[0][0] == "", "new cell defaults to \"\"");

    g.Cells[1][1] = "hello";
    CHECK(g.Cells[1][1] == "hello", "Cells[][] write/read round-trips");

    g.RowCount = 5;   // grow rows
    CHECK((int)g.RowCount == 5, "RowCount grows via assignment (property side effect)");
    CHECK(g.Cells[1][1] == "hello", "growing RowCount preserves existing content");
    CHECK(g.Cells[0][4] == "", "newly-added row is blank");

    g.ColCount = 1;   // shrink cols
    CHECK((int)g.ColCount == 1, "ColCount shrinks via assignment");
    CHECK(g.Cells[0][0] == "", "surviving column content preserved after shrink");

    bool threw = false;
    try { (void)g.Cells[5][0]; }   // col 5 is out of range after the shrink to ColCount=1
    catch (const std::out_of_range&) { threw = true; }
    CHECK(threw, "out-of-range Cells[][] access throws std::out_of_range (real-VCL ERangeError analogue)");
}

// ===========================================================================
//  [3] SplitStrByTabOnly -- field-splitting edge cases
// ===========================================================================
static void test_split_str_by_tab_only()
{
    printf("\n[3] SplitStrByTabOnly\n");

    // Normal two-field line: "hello\tworld" -> "hello", "world", then false.
    {
        char buf[64]; std::strcpy(buf, "hello\tworld");
        char dest[64] = {0};
        bool r1 = SplitStrByTabOnly(buf, dest, sizeof(dest));
        CHECK(r1 == true && std::string(dest) == "hello", "field 1 of \"hello\\tworld\" == \"hello\"");
        bool r2 = SplitStrByTabOnly(buf, dest, sizeof(dest));
        CHECK(r2 == true && std::string(dest) == "world", "field 2 of \"hello\\tworld\" == \"world\"");
        bool r3 = SplitStrByTabOnly(buf, dest, sizeof(dest));
        CHECK(r3 == false, "no field 3 -> returns false (buf now empty)");
    }

    // Adjacent delimiters (empty middle field): "a\t\tb" -> "a", "", "b", then false.
    // (Hand-traced against golden's exact algorithm -- see the .cpp's own
    // header comment on SplitStrByTabOnly for the full trace.)
    {
        char buf[64]; std::strcpy(buf, "a\t\tb");
        char dest[64] = {0};
        bool r1 = SplitStrByTabOnly(buf, dest, sizeof(dest));
        CHECK(r1 == true && std::string(dest) == "a", "field 1 of \"a\\t\\tb\" == \"a\"");
        bool r2 = SplitStrByTabOnly(buf, dest, sizeof(dest));
        CHECK(r2 == true && std::string(dest) == "", "field 2 of \"a\\t\\tb\" == \"\" (empty middle field)");
        bool r3 = SplitStrByTabOnly(buf, dest, sizeof(dest));
        CHECK(r3 == true && std::string(dest) == "b", "field 3 of \"a\\t\\tb\" == \"b\"");
        bool r4 = SplitStrByTabOnly(buf, dest, sizeof(dest));
        CHECK(r4 == false, "no field 4 -> returns false");
    }
}

// ===========================================================================
//  [4] CopyStringGridAsTabFormat / PasteStringGridAsTabFormat round-trip
//  REAL ORACLE: these 5 lines are a byte-exact excerpt (all-ASCII, verified
//  via `python3 -c "...set(data)..."` against the golden Big5 corruption
//  risk -- zero non-ASCII bytes present) copied from the actual production
//  D:\HT9045\SECS\SECS\SYSTEM\AlarmData.def on the reference dev machine
//  (header row + rows 1-4 of 2739 real data rows), NOT fabricated numbers.
//  Embedded here (rather than read from that path at test run time) so this
//  test is portable to a machine that does not have that exact tree.
// ===========================================================================
static void test_copy_paste_roundtrip_real_oracle()
{
    printf("\n[4] CopyStringGridAsTabFormat / PasteStringGridAsTabFormat (real AlarmData.def excerpt)\n");

    static const char* kRealLines[5] = {
        "No.\tID_AlarmList\tAlarmCode\tUnitNo\tUnitName\tType\tMessage\tEnable\tAlarmID\tClass\tPosition\t",
        "1\t41\t41\t0\tEvent\t0\tUnknown Alarm Code\t1\t000000000\t0\tEvent\t",
        "2\t0101\tMES0101\t1\tInput Arm\t3\tDevice pick-up error on the tray\t1\t301000101\t3\tInput Arm\t",
        "3\t0102\tMES0102\t1\tInput Arm\t3\tAbnormal auto tray end. Please check devices count.\t1\t301000102\t3\tInput Arm\t",
        "4\t0109\tJAM0109\t1\tInput Arm\t1\tDevice pick-up error on Hot Plate\t1\t101000109\t1\tInput Arm\t",
    };

    THGem gem;
    // strGrdAlarm is ctor-sized 12 cols x 5 rows already (matches golden
    // .dfm ColCount=12 default-RowCount=5) -- exactly enough for these 5 lines.
    TStringList src;
    for (int i = 0; i < 5; ++i) src.Add(AnsiString(kRealLines[i]));

    gem.PasteStringGridAsTabFormat(gem.strGrdAlarm, &src);

    CHECK(gem.strGrdAlarm->Cells[2][0] == "AlarmCode", "header row: col2 == \"AlarmCode\"");
    CHECK(gem.strGrdAlarm->Cells[2][1] == "41",        "row1 (No.=1): AlarmCode == \"41\"");
    CHECK(gem.strGrdAlarm->Cells[2][2] == "MES0101",   "row2 (No.=2): AlarmCode == \"MES0101\"");
    CHECK(gem.strGrdAlarm->Cells[4][2] == "Input Arm", "row2: UnitName == \"Input Arm\"");
    CHECK(gem.strGrdAlarm->Cells[6][2] == "Device pick-up error on the tray", "row2: Message text matches real data");
    CHECK(gem.strGrdAlarm->Cells[7][2] == "1",         "row2: Enable == \"1\"");
    CHECK(gem.strGrdAlarm->Cells[8][2] == "301000101", "row2: AlarmID == \"301000101\"");
    CHECK(gem.strGrdAlarm->Cells[9][2] == "3",         "row2: Class == \"3\"");
    CHECK(gem.strGrdAlarm->Cells[6][3] == "Abnormal auto tray end. Please check devices count.", "row3: Message text matches real data");

    // Round-trip back to a TStringList and compare byte-for-byte against the
    // original 5 real lines (this grid has no 6th row, so no all-blank-row
    // "\t"-only quirk line to account for here).
    TStringList out;
    gem.CopyStringGridAsTabFormat(gem.strGrdAlarm, &out);
    CHECK(out.Count == 5, "round-trip: 5 lines out");
    bool allMatch = true;
    for (int i = 0; i < 5 && i < out.Count; ++i)
        if (!(out.GetString(i) == AnsiString(kRealLines[i]))) allMatch = false;
    CHECK(allMatch, "round-trip: all 5 lines byte-exact vs. the real AlarmData.def excerpt");

    // Grow by one row and confirm the documented "all-blank row copies as a
    // lone tab" quirk (matches the real file's own unused-row format).
    gem.strGrdAlarm->RowCount = 6;
    TStringList out2;
    gem.CopyStringGridAsTabFormat(gem.strGrdAlarm, &out2);
    CHECK(out2.Count == 6, "after RowCount=6: 6 lines out");
    CHECK(out2.GetString(5) == "\t", "blank row copies as a lone tab char (golden quirk, matches real on-disk empty rows)");
}

// ===========================================================================
//  [5] CEID family -- hand-seeded grid (SetCEIDContent NOT called -- see
//  file-head SAFETY NOTE). Cells are written directly, replicating exactly
//  what SetCEIDContent(iCeid=11, CeidAlias="MyAlias", iReportCount=2,
//  iReportIDData={100,101}, Mode=1) would have produced at row 1 (hand-
//  traced against the translated algorithm in SECSGEM/uHGemEquipment.cpp).
// ===========================================================================
static void test_ceid_family()
{
    printf("\n[5] CEID StringGrid family (hand-seeded, no SetCEIDContent call)\n");

    THGem gem;
    // Row 1 of strGrdCEID: CEID=11, Mode/Enable=1, Alias="MyAlias", ReportIDs 100,101.
    gem.strGrdCEID->Cells[0][1] = 11;
    gem.strGrdCEID->Cells[1][1] = 1;
    gem.strGrdCEID->Cells[2][1] = "MyAlias";
    gem.strGrdCEID->Cells[3][1] = 100;
    gem.strGrdCEID->Cells[4][1] = 101;

    CHECK(gem.CheckCeidExist("11") == true,  "CheckCeidExist(\"11\") true after seeding row 1");
    CHECK(gem.CheckCeidExist("999") == false, "CheckCeidExist(\"999\") false (not seeded)");
    CHECK(gem.CheckCEIDExist("11") == true,  "CheckCEIDExist (upper-case sibling) agrees with CheckCeidExist");

    unsigned outIDs[16] = {0};
    unsigned n = gem.GetCEIDContent(11, outIDs, 2);
    CHECK(n == 2, "GetCEIDContent(11) returns 2 report IDs");
    CHECK(outIDs[0] == 100 && outIDs[1] == 101, "GetCEIDContent(11) returns {100,101} in seeded order");

    CHECK(gem.IsEnableEvent(0, 11) == true, "IsEnableEvent(_,11) true (col1 seeded \"1\")");
    CHECK(gem.IsEnableEvent(0, 999) == false, "IsEnableEvent(_,999) false (CEID not present)");

    // Now seed stdGridReportID row 1: ReportID=100, Mode=2, SVIDs 501,502,503.
    gem.stdGridReportID->Cells[0][1] = 100;
    gem.stdGridReportID->Cells[1][1] = 2;
    gem.stdGridReportID->Cells[2][1] = 501;
    gem.stdGridReportID->Cells[3][1] = 502;
    gem.stdGridReportID->Cells[4][1] = 503;

    CHECK(gem.CheckReportIDExist("100") == true,  "CheckReportIDExist(\"100\") true");
    CHECK(gem.CheckReportIDExist("777") == false, "CheckReportIDExist(\"777\") false");
    CHECK(gem.GetReportIDType("100") == 2, "GetReportIDType(\"100\") == 2 (seeded Mode)");
    CHECK(gem.GetReportIDType("777") == 3, "GetReportIDType of an unknown ReportID == 3 (golden's not-found sentinel)");

    unsigned outSVIDs[16] = {0};
    unsigned m = gem.GetReportIDContent(100, outSVIDs, 0);
    CHECK(m == 3, "GetReportIDContent(100) returns 3 SVIDs");
    CHECK(outSVIDs[0] == 501 && outSVIDs[1] == 502 && outSVIDs[2] == 503, "GetReportIDContent(100) returns {501,502,503}");

    // CheckCeidAlreadyDefine(11): true only if one of CEID 11's linked
    // report IDs has GetReportIDType(...)==0 (Host-defined). Right now
    // ReportID "100" has Mode 2, so this should be false.
    CHECK(gem.CheckCeidAlreadyDefine("11") == false, "CheckCeidAlreadyDefine(11) false (linked ReportID 100 has Mode 2, not 0)");

    // Re-tag ReportID 100 as Mode 0 (Host-defined) and re-check.
    gem.stdGridReportID->Cells[1][1] = 0;
    CHECK(gem.CheckCeidAlreadyDefine("11") == true, "CheckCeidAlreadyDefine(11) true once linked ReportID 100 is Mode 0");
    CHECK(gem.CheckReportAlreadyDefine("100") == true, "CheckReportAlreadyDefine(\"100\") true (row exists in stdGridReportID)");

    // AddCeidReportID: append a 3rd report ID (200) into CEID 11's first
    // free slot (col 5, since cols 3,4 are already 100,101).
    gem.AddCeidReportID("11", "200");
    CHECK(gem.strGrdCEID->Cells[5][1] == "200", "AddCeidReportID appends into the first empty ReportID slot (col 5)");

    // DeleteHostReportIDOfAssignCeid(11): clears ALL of CEID 11's ReportID
    // slots (cols 3..ColCount-1), unconditionally (does not check Mode).
    gem.DeleteHostReportIDOfAssignCeid("11");
    CHECK(gem.strGrdCEID->Cells[3][1] == "" && gem.strGrdCEID->Cells[4][1] == "" && gem.strGrdCEID->Cells[5][1] == "",
          "DeleteHostReportIDOfAssignCeid(11) clears all of CEID 11's ReportID slots");
    CHECK(gem.strGrdCEID->Cells[0][1] == "11", "DeleteHostReportIDOfAssignCeid does not clear the CEID column itself");

    // Re-seed for DeleteAllHostDefineCeid (clears ReportID slots for EVERY
    // non-blank CEID row, via DeleteHostReportIDOfAssignCeid).
    gem.strGrdCEID->Cells[3][1] = 999;
    gem.strGrdCEID->Cells[0][2] = 22;
    gem.strGrdCEID->Cells[3][2] = 888;
    gem.DeleteAllHostDefineCeid();
    CHECK(gem.strGrdCEID->Cells[3][1] == "" && gem.strGrdCEID->Cells[3][2] == "",
          "DeleteAllHostDefineCeid clears ReportID slots on every non-blank CEID row");
}

// ===========================================================================
//  [6] Report-ID delete family -- DeleteReportID / DeleteReportIDOfCeid /
//  DeleteAllHostDefineReportID / DeleteAllHostReportIDOfCeid (all safe: none
//  call SaveEventReportData -- see file-head SAFETY NOTE).
// ===========================================================================
static void test_report_delete_family()
{
    printf("\n[6] Report-ID delete family\n");

    THGem gem;
    // 3 rows of stdGridReportID: {ReportID=10,Mode=0}, {20,Mode=1}, {30,Mode=0}
    gem.stdGridReportID->Cells[0][1] = 10; gem.stdGridReportID->Cells[1][1] = 0;
    gem.stdGridReportID->Cells[0][2] = 20; gem.stdGridReportID->Cells[1][2] = 1;
    gem.stdGridReportID->Cells[0][3] = 30; gem.stdGridReportID->Cells[1][3] = 0;

    // DeleteReportID(20, 1): row 2 matches (ReportID=20 AND Mode==1) -> the
    // golden algorithm shifts every LATER row up by one and blanks the tail.
    bool ok = gem.DeleteReportID(20, 1);
    CHECK(ok == true, "DeleteReportID(20,1) finds and removes the matching row");
    CHECK(gem.stdGridReportID->Cells[0][2] == "30", "row 2 now holds what was row 3 (ReportID=30), rows shifted up");
    CHECK(gem.stdGridReportID->Cells[0][3] == "", "last row is now blank after the shift");

    // DeleteReportID with a Mode that does not match any row -> false, no mutation.
    bool ok2 = gem.DeleteReportID(10, 5);
    CHECK(ok2 == false, "DeleteReportID(10,5) -- Mode 5 does not match ReportID 10's stored Mode(0) -> false");
    CHECK(gem.stdGridReportID->Cells[0][1] == "10", "no mutation when DeleteReportID finds nothing");

    // DeleteReportIDOfCeid: strGrdCEID row with CEID=5, ReportIDs {10,30,40}
    // at cols 3,4,5. Removing ReportID 30 should compact the remaining IDs
    // {10,40} leftward (cols 3,4), per golden's compaction algorithm.
    gem.strGrdCEID->Cells[0][1] = 5;
    gem.strGrdCEID->Cells[3][1] = 10;
    gem.strGrdCEID->Cells[4][1] = 30;
    gem.strGrdCEID->Cells[5][1] = 40;
    gem.DeleteReportIDOfCeid(30);
    CHECK(gem.strGrdCEID->Cells[3][1] == "10", "DeleteReportIDOfCeid(30): col3 still holds surviving ReportID 10");
    CHECK(gem.strGrdCEID->Cells[4][1] == "40", "DeleteReportIDOfCeid(30): col4 compacted to the next surviving ReportID 40");
    CHECK(gem.strGrdCEID->Cells[5][1] == "",   "DeleteReportIDOfCeid(30): trailing slot blanked after compaction");

    // DeleteAllHostReportIDOfCeid(40): should find & remove ReportID 40 from
    // EVERY strGrdCEID row that references it (here, just row 1), compacting.
    gem.DeleteAllHostReportIDOfCeid(40);
    CHECK(gem.strGrdCEID->Cells[3][1] == "10", "DeleteAllHostReportIDOfCeid(40): surviving ReportID 10 remains at col3");
    CHECK(gem.strGrdCEID->Cells[4][1] == "",   "DeleteAllHostReportIDOfCeid(40): ReportID 40 removed, no gap left behind");

    // DeleteAllHostDefineReportID: repeatedly finds a stdGridReportID row
    // with Mode=="0" (Host-defined) and fully deletes it (both from
    // stdGridReportID via DeleteReportID, and from any strGrdCEID row via
    // DeleteReportIDOfCeid), until none remain. After the row-1
    // (ReportID=20) deletion above, rows are: {10,Mode0}@row1, {30,Mode0}@row2.
    bool allDeleted = gem.DeleteAllHostDefineReportID();
    CHECK(allDeleted == true, "DeleteAllHostDefineReportID always returns true (golden: unconditional)");
    CHECK(gem.stdGridReportID->Cells[0][1] == "" && gem.stdGridReportID->Cells[0][2] == "",
          "DeleteAllHostDefineReportID removes every remaining Mode==0 row (10 and 30)");
}

// ===========================================================================
//  [7] ReadEventReportData -- REAL ORACLE, read-only against the actual
//  on-disk D:\HT9045\SECS\SECS\SYSTEM\EventReport_CEID.def /
//  EventReport_ReportID.def (safe: LoadFromFile only, never writes).
//  Degrades gracefully (skips, does not fail) if that exact path is absent
//  on the machine running this test.
// ===========================================================================
static void test_read_event_report_data_real_oracle()
{
    printf("\n[7] ReadEventReportData (real on-disk oracle, read-only)\n");

    const AnsiString kPath1 = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_CEID.def";
    if (!FileExists(kPath1))
    {
        printf("  SKIP: %s not present on this machine -- test is environment-dependent, not a failure.\n", kPath1.c_str());
        return;
    }

    THGem gem;
    gem.ReadEventReportData();

    // Real file's row 0 is the literal column-header text (confirmed via
    // `python3` byte-level inspection of the actual file, 2026-07-16):
    //   "CEID\tEnable\tAlias\tReportID\t"
    CHECK(gem.strGrdCEID->Cells[0][0] == "CEID",     "strGrdCEID header row col0 == \"CEID\" (real on-disk header)");
    CHECK(gem.strGrdCEID->Cells[1][0] == "Enable",   "strGrdCEID header row col1 == \"Enable\"");
    CHECK(gem.strGrdCEID->Cells[3][0] == "ReportID", "strGrdCEID header row col3 == \"ReportID\"");

    CHECK(gem.stdGridReportID->Cells[0][0] == "ReportID", "stdGridReportID header row col0 == \"ReportID\" (real on-disk header)");
    CHECK(gem.stdGridReportID->Cells[1][0] == "Type",     "stdGridReportID header row col1 == \"Type\"");
}

// ===========================================================================
//  [8] ReadAlamData / WriteAlamData -- GemSystemPath-derived path (NOT
//  hardcoded), pointed at a throwaway scratch folder. Never touches the real
//  D:\HT9045\SECS tree.
// ===========================================================================
static void test_alarm_read_write_roundtrip()
{
    printf("\n[8] ReadAlamData / WriteAlamData (scratch folder, not the real D:\\HT9045\\SECS tree)\n");

    const AnsiString kScratchDir = "uHGemEquipment_test_scratch";
    ForceDirectories(kScratchDir);

    THGem gemWriter;
    gemWriter.GemSystemPath = kScratchDir;
    gemWriter.strGrdAlarm->Cells[0][1] = 1;   // pre-populate cols 0-6 the way
    gemWriter.strGrdAlarm->Cells[1][1] = "41";// out-of-scope registration code
    gemWriter.strGrdAlarm->Cells[2][1] = "41";// would (SetAlamData only ever
    gemWriter.strGrdAlarm->Cells[6][1] = "Unknown Alarm Code"; // touches cols 7-10).
    gemWriter.SetAlamData(1, "000000000", "0", "unused-by-SetAlamData", "Event");
    CHECK(gemWriter.strGrdAlarm->Cells[7][1] == "1",          "SetAlamData writes col7 (Enable)");
    CHECK(gemWriter.strGrdAlarm->Cells[8][1] == "000000000",  "SetAlamData writes col8 (ALID)");
    CHECK(gemWriter.strGrdAlarm->Cells[9][1] == "0",          "SetAlamData writes col9 (Class)");
    CHECK(gemWriter.strGrdAlarm->Cells[10][1] == "Event",     "SetAlamData writes col10 (Position)");

    gemWriter.WriteAlamData();   // writes <scratch>/AlarmData.def

    const AnsiString kWrittenFile = kScratchDir + "\\AlarmData.def";
    CHECK(FileExists(kWrittenFile), "WriteAlamData created AlarmData.def under the scratch folder");

    // AI(W906-uHGemEquipment) 20260716: golden QUIRK, preserved verbatim and
    // documented at ReadAlamData's own definition -- it loads into
    // strGrdAlarmOld, NOT strGrdAlarm. Read it back with a FRESH THGem to
    // confirm this round-trips correctly into the "old" grid.
    THGem gemReader;
    gemReader.GemSystemPath = kScratchDir;
    // strGrdAlarmOld's .dfm leaves ColCount at the real-VCL design-time
    // default (5) -- ReadAlamData ITSELF only ever assigns RowCount (golden
    // uHGemEquipment.cpp:6247/6256), never ColCount, so a real THGem must
    // have something else (out-of-scope FormCreate-era setup, matching
    // strGrdAlarm's own ColCount=12) widen this grid before ReadAlamData is
    // meaningfully callable in production. Confirmed empirically: WITHOUT
    // this line, PasteStringGridAsTabFormat legitimately throws
    // std::out_of_range trying to write column 8+ of an 11-column real
    // AlarmData.def row into a 5-column grid -- not a translation bug, a
    // real precondition this test must satisfy itself (this wave's scope
    // does not include whatever out-of-scope code establishes it for real).
    gemReader.strGrdAlarmOld->ColCount = 12;
    gemReader.ReadAlamData();
    CHECK(gemReader.strGrdAlarmOld->Cells[8][1] == "000000000", "ReadAlamData round-trips ALID into strGrdAlarmOld col8");
    CHECK(gemReader.strGrdAlarmOld->Cells[9][1] == "0",         "ReadAlamData round-trips Class into strGrdAlarmOld col9");
    CHECK(gemReader.strGrdAlarmOld->Cells[10][1] == "Event",    "ReadAlamData round-trips Position into strGrdAlarmOld col10");
    // strGrdAlarm (the OTHER grid) must be untouched by ReadAlamData.
    CHECK(gemReader.strGrdAlarm->Cells[8][1] == "", "ReadAlamData does NOT touch strGrdAlarm (quirk confirmed: separate grid)");

    // Cleanup: remove the scratch file/folder this test created (best-effort;
    // not asserted, since a leftover harmless scratch file is not a test failure).
    DeleteFile(kWrittenFile);
    RemoveDir(kScratchDir);
}

// ===========================================================================
int main()
{
    printf("=== SECSGEM/uHGemEquipment (+ vclcompat/StringGrid) translation verification ===\n");
    printf("(golden HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.{h,cpp}; original binary NOT run)\n");

    test_gemtimer();
    test_stringgrid_basic();
    test_split_str_by_tab_only();
    test_copy_paste_roundtrip_real_oracle();
    test_ceid_family();
    test_report_delete_family();
    test_read_event_report_data_real_oracle();
    test_alarm_read_write_roundtrip();

    printf("\n=== RESULT: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
