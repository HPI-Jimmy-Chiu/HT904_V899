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
//  BUILD: wired into tests/CMakeLists.txt as the `test_uHGemEquipment` target
//  (`ctest -R uHGemEquipment` or `ctest --test-dir build`), linking
//  ht9045_secsgem + ht9045_public + vclcompat. AI(W906-uHGemEquipment-
//  ConnLifecycle) 20260717: the connection-lifecycle slice added below pulls
//  in ClientSocket.cpp/ServerSocket.cpp/WinSocketErrorCode.cpp transitively
//  via those libraries -- if ever building this file standalone (outside
//  CMake) for quick iteration, add those three .cpp files to the command
//  alongside the ones already listed for the original StringGrid-family
//  slice (uHGemEquipment.cpp, StringGrid.cpp, AnsiString.cpp, TStringList.cpp,
//  SysUtils.cpp, TDateTime.cpp, IniFiles.cpp, SecsEventType.cpp).
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
//
//  AI(W906-uHGemEquipment-ConnLifecycle) 20260717: EXTENDED for the new
//  TCP/IP connection-lifecycle slice (clientGem/srvGem event handlers,
//  DoOpenCommuncation/DoOnLine/OnlineLocalOrRemote, Connect/DisConnect/
//  IsConnect/OnLine family, CheckSocketActiveFalse, ClearDefaultEvenReport,
//  GetTimeInfo). Golden reference for the new slice:
//    HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//      (:315-348, 392-439, 2100-2142, 3382-3490, 3604-3645, 4988-5008,
//       5146-5159, 5548-5644, 6812-6910)
//
//  *** SAFETY NOTE for the NEW tests -- read before extending further ***
//  srvGemClientError (tested in [12] below) faithfully calls
//  SaveSECSGEMErrToLog, which -- matching golden exactly -- unconditionally
//  APPENDS to a hardcoded absolute path outside this repo/migration tree:
//  D:\SECS_GEM_LOGS\<yyyy>\<mm_dd>\SECSGEM_ErrLog_<hh>.txt (creating that
//  directory tree via ForceDirectories if absent). This is a real,
//  unavoidable side effect of exercising srvGemClientError's genuine
//  translated behavior (there is no GemSystemPath-style parameter to redirect
//  it, unlike ReadAlamData/WriteAlamData above) -- but it is fundamentally
//  LOWER RISK than SaveEventReportData's own hardcoded-path danger (which
//  OVERWRITES real production CEID/ReportID configuration): this is a small,
//  append-only diagnostic log line, never destructive to existing data, and
//  no worse than what golden's own production binary already does on every
//  real socket error.
//
//  AI(W906-uHGemEquipment-ConnLifecycle) 20260717: per explicit user
//  direction, exercising this real path during dev/test is acceptable -- the
//  actual requirement is that ctest must not leave the real archive dirtier
//  after a run than before it (a first run of this test, before this note,
//  did leave 2 stray lines behind in D:\SECS_GEM_LOGS\2026\07_17\
//  SECSGEM_ErrLog_10.txt -- left in place per user decision, now treated as
//  the accepted baseline). Test [12] below therefore captures that target
//  file's exact pre-test byte content (using the same yyyy/mm_dd/hh tokens
//  SaveSECSGEMErrToLog itself computes) and restores it verbatim immediately
//  after the assertions -- deleting the file/newly-created parent
//  directories entirely if none existed before this run. Repeat ctest runs
//  should therefore leave D:\SECS_GEM_LOGS byte-for-byte unchanged.
//  SaveSECSGEMErrToLog is otherwise NOT called directly by any other test
//  below (matching the SaveEventReportData precedent).
//
//  AI(W906-uHGemEquipment-BucketB) 20260717: EXTENDED again for the ~15
//  widget stand-ins (THGemRadioGroup/THGemEdit/THGemCheckBox/THGemComboBox/
//  THGemPanel/THGemSpeedButton/THGemMemo) and their first real consumers:
//  InitialHGem/SaveSystemDefault (scratch GemSystemIniPath, never the real
//  D:\HT9045\SECS tree -- same scratch-path discipline as test [8]'s
//  GemSystemPath), DoUpdateStatus (panel/button/EventReport refresh, INCLUDING
//  the KYEC 30-second forced-disconnect branch), ManualCreatergRoleClick,
//  ProcessShow, and StringOut(2-arg). New includes: cmydef.h (CUSTOMER_CODE/
//  CC_KYEC_LEE/CC_MAXIM_THAILAND -- this test now links ht9045_core/
//  ht9045_globals too, see tests/CMakeLists.txt's own updated comment) and
//  SECSGEM/SecsEventReport.h (g_SimLastEventReportCeid/g_SimEventReportCount/
//  ResetSimEventReport -- DoUpdateStatus's own EventReport(...) calls are
//  observed through this existing Sim counter, same as csystem.cpp's already-
//  established convention).
//
//  AI(W906-uHGemEquipment-BucketC) 20260717: EXTENDED again -- see this
//  file's own T1-T10 block near the end (socket receive pump / real HTimer /
//  T3 timeout / HSMS control-message handshake / Timer1Timer). Test [22]'s
//  own KYEC branch (previously "structural only" against an always-fires
//  HTimer stand-in) is UPDATED here too: SECSGEM_DoSeparate is now the REAL
//  elapsed-time HTimer (D6), so that assertion now correctly expects the
//  30-second forced-disconnect to NOT fire on the same poll it was armed --
//  see that test's own updated comment.
// =============================================================================
#include "SECSGEM/uHGemEquipment.h"
#include "SECSGEM/SecsEventReport.h"   // g_SimLastEventReportCeid / g_SimEventReportCount / ResetSimEventReport
#include "cmydef.h"                     // CUSTOMER_CODE / CC_KYEC_LEE / CC_MAXIM_THAILAND / CosFunction
// AI(W906-uHGemEquipment-BucketC) 20260717: D3 -- HSys.MyGem seam. Needed so
// this test binary can exercise the null-guard sites (DoConnect/
// DoProcessSFNoResponse/ProcessSocketReceiveData's catch) AND (T7) wire a
// REAL HTGem instance to HSys.MyGem to prove the SendLocalDataHook seam works
// end-to-end.
#include "database.h"            // HSys / SYSTEM_MODULAR
#include "SECSGEM/uHGemClass.h"  // HTGem (T7)
#include "Config.h"              // IniConfig.bEnable_SECS_GEM (T10)

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>
#include <windows.h>   // ::Sleep (GemTimer elapsed-time test)

// AI(W906-uHGemEquipment-BucketC) 20260717: bSECSGEM_DoSeparate has EXTERNAL
// linkage in uHGemEquipment.cpp (a plain file-scope global, golden :24) --
// not declared in uHGemEquipment.h itself (an internal implementation detail
// shared between DoUpdateStatus and Timer1Timer). Declared here, test-only,
// so T10 (below) can observe its latch behavior directly. Its siblings
// `SECSGEM_DoSeparate`/`SECSGEM_DoSeparateWait` (the two HTimer globals right
// next to it in that file) canNOT be similarly declared here -- HTimer is a
// deliberately TU-local (anonymous-namespace) type, not nameable from this
// separate translation unit; their behavior is instead observed indirectly
// (test [22]'s KYEC-branch assertions, and T10's own bWaitHTimer-path check).
extern bool bSECSGEM_DoSeparate;

// ---------------------------------------------------------------------------
//  MyDBIProcess (2-arg) -- this test binary does not link aHotPlateSubstrate.cpp
//  (the one real definition elsewhere in this tree); ReadAlamData's catch-block
//  needs SOME definition to link against (same pattern as
//  tests/test_FTPClient_EventHandlers.cpp / test_uHGemClass.cpp).
// ---------------------------------------------------------------------------
void MyDBIProcess(AnsiString /*S1*/, AnsiString /*S2*/) {}

// ---------------------------------------------------------------------------
//  AI(W906-uHGemEquipment-BucketC) 20260717: MyDBIProcess (3-arg, EXTERNAL
//  linkage) / ShowMyMessage -- as of this wave, uHGemEquipment.cpp's new
//  `#include "database.h"` (D3) means this test binary now links ht9045_db
//  (database.cpp, which defines the referenced-by-this-TU global `HSys`).
//  Static-archive linking pulls in database.cpp's WHOLE object file once
//  `HSys` is referenced -- which means database.cpp's OWN unresolved
//  externals (LoadIoData/LoadMotData's calls to a 3-arg MyDBIProcess and
//  ShowMyMessage, database.cpp:64-65) must ALSO resolve, even though this
//  test never calls LoadIoData/LoadMotData itself. Same stub shape already
//  established by tests/test_config_loaders.cpp for the identical situation.
//  Distinct overload from the 2-arg MyDBIProcess just above (different
//  arity -- no collision).
// ---------------------------------------------------------------------------
void __fastcall MyDBIProcess(AnsiString /*asTable*/, AnsiString /*S1*/, AnsiString /*S2*/) {}
void ShowMyMessage(AnsiString /*S1*/, AnsiString /*S2*/, AnsiString /*S3*/, bool /*Ok*/, bool /*bServoOff*/) {}

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
//  [9] Connect / DisConnect / IsConnect state transitions
// ===========================================================================
static void test_connect_lifecycle()
{
    printf("\n[9] Connect/DisConnect/IsConnect\n");

    THGem gem;
    CHECK(gem.IsConnect() == false, "fresh THGem: IsConnect()==false");

    gem.Connect();
    CHECK(gem.bStartConnect == true && gem.bAutoConnect == true, "Connect() sets bStartConnect/bAutoConnect");
    CHECK(gem.IsConnect() == false, "Connect() alone does not flip bConnect (DoConnect's job, out of this wave's scope)");

    // Guard: golden `if(bConnect==true) return;` -- Connect() while already
    // connected is a no-op (does not even touch bStartConnect/bAutoConnect).
    gem.bConnect = true;
    gem.bStartConnect = false;
    gem.bAutoConnect = false;
    gem.Connect();
    CHECK(gem.bStartConnect == false && gem.bAutoConnect == false,
          "Connect() while bConnect==true early-returns (golden guard) -- leaves bStartConnect/bAutoConnect untouched");

    gem.DisConnect();
    CHECK(gem.IsConnect() == false && gem.bStartConnect == false && gem.bAutoConnect == false,
          "DisConnect() clears bConnect/bStartConnect/bAutoConnect");
}

// ===========================================================================
//  [10] clientGemConnect / Disconnect / Error / Connecting handler behavior
// ===========================================================================
static void test_clientgem_handlers()
{
    printf("\n[10] clientGemConnect/Disconnect/Error/Connecting\n");

    THGem gem;

    gem.clientGemConnect(gem.clientGem, gem.clientGem->Socket);
    CHECK(gem.WaitShowString->Count > 0 && gem.WaitShowString->GetString(gem.WaitShowString->Count - 1) == "Connect",
          "clientGemConnect logs \"Connect\" via StringOut (WaitShowString)");
    CHECK(gem.LogDataString->GetString(gem.LogDataString->Count - 1) == "Connect",
          "clientGemConnect's StringOut also appends to LogDataString");

    gem.bConnect = true;
    gem.clientGemDisconnect(gem.clientGem, gem.clientGem->Socket);
    CHECK(gem.bConnect == false, "clientGemDisconnect clears bConnect");
    CHECK(gem.bAutoConnect == true, "clientGemDisconnect sets bAutoConnect when bConnect was true (golden: reconnect-on-drop)");

    gem.bConnect = false;
    gem.bAutoConnect = false;
    gem.clientGemDisconnect(gem.clientGem, gem.clientGem->Socket);
    CHECK(gem.bAutoConnect == false, "clientGemDisconnect leaves bAutoConnect false when bConnect was already false");

    int errCode = 123;
    gem.clientGem->Active = true;   // Sim mode: pretend already connected
    gem.clientGemError(gem.clientGem, gem.clientGem->Socket, Scktcomp::eeGeneral, errCode);
    CHECK(gem.clientGem->Active == false, "clientGemError forces clientGem->Active=false");
    CHECK(errCode == 0, "clientGemError zeroes the ErrorCode out-param");
    CHECK(gem.bTCPIP_Error == true, "clientGemError sets bTCPIP_Error");

    gem.clientGemConnecting(gem.clientGem, gem.clientGem->Socket);
    CHECK(gem.WaitShowString->GetString(gem.WaitShowString->Count - 1) == "connecting",
          "clientGemConnecting logs \"connecting\"");
}

// ===========================================================================
//  [11] DoOpenCommuncation -- passive (srvGem) role open-guard behavior.
//  SAFETY-CRITICAL golden behavior: srvGem->Open() is called ONLY when
//  srvGem->Active==false -- never a blind Close()+Open() (see the .cpp's own
//  citation of golden's "斷線重連的秘密" warning). Verified here by accepting a
//  Sim connection and confirming it SURVIVES a re-entry into the state
//  machine while already Active (a blind Close()+Open() would have dropped it).
// ===========================================================================
static void test_doopencommuncation_open_guard()
{
    printf("\n[11] DoOpenCommuncation -- passive (srvGem) open-guard behavior\n");

    THGem gem;
    gem.bUseClientSocket = false;
    gem.srvGem->Port = 6000;
    gem.bOpenCommuncation = true;

    CHECK(gem.iOpenCommuncationTask == 1, "fresh THGem starts at iOpenCommuncationTask==1");
    gem.DoOpenCommuncation();   // case 1: bOpenCommuncation==true -> arms 0.5s delay, Task=100
    CHECK(gem.iOpenCommuncationTask == 100, "case 1: bOpenCommuncation==true advances Task to 100");

    ::Sleep(600);   // let DelayOpenCommuncation's 0.5s arm elapse
    CHECK(gem.srvGem->Active == false, "srvGem starts inactive (Sim default)");
    gem.DoOpenCommuncation();   // case 100: timer elapsed, Port!=0, inactive -> Open() -> Task=200
    CHECK(gem.srvGem->Active == true, "DoOpenCommuncation opens srvGem when it was inactive");
    CHECK(gem.iOpenCommuncationTask == 200, "Task advances to 200 after a successful Open()");

    // Accept a Sim connection so a blind Close()+Open() would be OBSERVABLE
    // (it would drop this connection -- Close() always empties Connections[]).
    gem.srvGem->SimAcceptConnection("10.1.1.5", 4000);
    CHECK(gem.srvGem->Socket->ActiveConnections == 1, "Sim-accepted one connection");

    // Re-enter case 100 with srvGem ALREADY Active.
    gem.iOpenCommuncationTask = 100;
    gem.DoOpenCommuncation();
    CHECK(gem.srvGem->Active == true, "srvGem stays Active across a re-entry with Active==true already");
    CHECK(gem.srvGem->Socket->ActiveConnections == 1,
          "open-guard preserved: the Sim-accepted connection survives (no blind Close()+Open())");
}

// ===========================================================================
//  [12] srvGemClientConnect / Disconnect / Error state updates
// ===========================================================================
static void test_srvgemclient_handlers()
{
    printf("\n[12] srvGemClientConnect/Disconnect/Error\n");

    THGem gem;

    // srvGemClientConnect needs an accepted connection so Socket->LocalPort/
    // LocalAddress (the event's own Socket param) and srvGem->Socket->
    // Connections[i] (the loop) are both meaningful.
    TCustomWinSocket *conn = gem.srvGem->SimAcceptConnection("10.1.1.9", 5555);
    gem.srvGemClientConnect(gem.srvGem, conn);
    CHECK(gem.bServoSocketConnect == true, "srvGemClientConnect sets bServoSocketConnect");
    CHECK(gem.bReceiveMultiConnect == false, "srvGemClientConnect: bReceiveMultiConnect stays false with only 1 connection");

    TCustomWinSocket *conn2 = gem.srvGem->SimAcceptConnection("10.1.1.10", 5556);
    gem.srvGemClientConnect(gem.srvGem, conn2);
    CHECK(gem.bReceiveMultiConnect == true, "srvGemClientConnect sets bReceiveMultiConnect once ActiveConnections>1");

    gem.iOpenCommuncationTask = 999;
    gem.srvGemClientDisconnect(gem.srvGem, conn);
    CHECK(gem.bServoSocketConnect == false, "srvGemClientDisconnect clears bServoSocketConnect");
    CHECK(gem.iOpenCommuncationTask == 1,
          "srvGemClientDisconnect resets iOpenCommuncationTask to 1 (Timer1Task's own reset is deferred, see .cpp comment)");

    // srvGemClientError -- see this file's own top-of-file SAFETY NOTE: this
    // real call appends one line to D:\SECS_GEM_LOGS\... via SaveSECSGEMErrToLog.
    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: per explicit user
    // direction, using the real D:\SECS_GEM_LOGS path during dev/test is fine
    // -- what's NOT fine is leaving it dirtier after the test than before.
    // Capture the exact target file's pre-test content (computed with the
    // SAME yyyy/mm_dd/hh tokens SaveSECSGEMErrToLog itself uses) and restore
    // it verbatim afterward, so repeat ctest runs don't keep appending lines
    // to a real operational log forever.
    TDateTime tdNow = Now();
    AnsiString logDirYear = "D:\\SECS_GEM_LOGS\\" + FormatDateTime("yyyy", tdNow);
    AnsiString logDirDay = logDirYear + "\\" + FormatDateTime("mm_dd", tdNow);
    AnsiString logFilePath = logDirDay + "\\SECSGEM_ErrLog_" + FormatDateTime("hh", tdNow) + ".txt";
    bool logFileExistedBefore = FileExists(logFilePath);
    bool logDayDirExistedBefore = DirectoryExists(logDirDay);
    bool logYearDirExistedBefore = DirectoryExists(logDirYear);
    std::string logFileOriginalContent;
    if (logFileExistedBefore)
    {
        FILE *rf = fopen(logFilePath.c_str(), "rb");
        if (rf)
        {
            char buf[65536];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), rf)) > 0)
                logFileOriginalContent.append(buf, n);
            fclose(rf);
        }
    }

    int errCode = 42;
    gem.bServoSocketConnect = true;
    gem.srvGemClientError(gem.srvGem, conn, Scktcomp::eeGeneral, errCode);
    CHECK(gem.bServoSocketConnect == false, "srvGemClientError clears bServoSocketConnect");
    CHECK(errCode == 0, "srvGemClientError zeroes the ErrorCode out-param");
    CHECK(gem.bTCPIP_Error == true, "srvGemClientError sets bTCPIP_Error");

    // Restore D:\SECS_GEM_LOGS to exactly its pre-test state.
    if (logFileExistedBefore)
    {
        FILE *wf = fopen(logFilePath.c_str(), "wb");
        if (wf)
        {
            fwrite(logFileOriginalContent.data(), 1, logFileOriginalContent.size(), wf);
            fclose(wf);
        }
    }
    else
    {
        remove(logFilePath.c_str());
        if (!logDayDirExistedBefore)
            RemoveDir(logDirDay);
        if (!logYearDirExistedBefore)
            RemoveDir(logDirYear);
    }
}

// ===========================================================================
//  [13] CheckSocketActiveFalse
// ===========================================================================
static void test_check_socket_active_false()
{
    printf("\n[13] CheckSocketActiveFalse\n");

    THGem gem;

    gem.bUseClientSocket = true;
    CHECK(gem.clientGem->Active == false, "fresh clientGem starts inactive (Sim default)");
    CHECK(gem.CheckSocketActiveFalse() == true, "bUseClientSocket==true + clientGem inactive -> true");
    gem.clientGem->Active = true;
    CHECK(gem.CheckSocketActiveFalse() == false, "bUseClientSocket==true + clientGem active -> false");

    gem.bUseClientSocket = false;
    CHECK(gem.srvGem->Active == false, "fresh srvGem starts inactive (Sim default)");
    CHECK(gem.CheckSocketActiveFalse() == true, "bUseClientSocket==false + srvGem inactive -> true");
    gem.srvGem->Open();
    CHECK(gem.CheckSocketActiveFalse() == false, "bUseClientSocket==false + srvGem active -> false");
}

// ===========================================================================
//  [14] ClearDefaultEvenReport -- repeatedly deletes every Mode==1
//  (Handler-defined) stdGridReportID row, and scrubs strGrdCEID's references
//  to each deleted ReportID (via the already-tested DeleteReportID /
//  DeleteReportIDOfCeid). No SaveEventReportData call in its own body
//  (verified by reading its definition) -- safe to call directly.
// ===========================================================================
static void test_clear_default_even_report()
{
    printf("\n[14] ClearDefaultEvenReport\n");

    THGem gem;
    gem.stdGridReportID->Cells[0][1] = 10; gem.stdGridReportID->Cells[1][1] = 1;
    gem.stdGridReportID->Cells[0][2] = 20; gem.stdGridReportID->Cells[1][2] = 0;
    gem.stdGridReportID->Cells[0][3] = 30; gem.stdGridReportID->Cells[1][3] = 1;

    gem.strGrdCEID->Cells[0][1] = 5;
    gem.strGrdCEID->Cells[3][1] = 10;
    gem.strGrdCEID->Cells[4][1] = 30;

    gem.ClearDefaultEvenReport();

    CHECK(gem.stdGridReportID->Cells[0][1] == "20", "ClearDefaultEvenReport removes every Mode==1 row (10,30); surviving row (20) shifts up");
    CHECK(gem.stdGridReportID->Cells[0][2] == "", "no Mode==1 rows remain after ClearDefaultEvenReport");
    CHECK(gem.strGrdCEID->Cells[3][1] == "" && gem.strGrdCEID->Cells[4][1] == "",
          "ClearDefaultEvenReport also scrubs strGrdCEID's references to the deleted ReportIDs (10,30)");
}

// ===========================================================================
//  [15] GetTimeInfo -- TimeString/GemClock population (the in-scope half;
//  the disk-free-space/memory-status tail is a documented gated stub, see
//  the .cpp's own comment at GetTimeInfo's definition).
// ===========================================================================
static void test_get_time_info()
{
    printf("\n[15] GetTimeInfo -- TimeString/GemClock population\n");

    THGem gem;
    CHECK(gem.TimeString == "", "fresh THGem: TimeString starts empty");

    gem.GetTimeInfo();
    CHECK(gem.TimeString.Length() == 23, "TimeString matches \"yyyy-mm-dd hh:nn:ss.zzz\" length (23 chars)");
    CHECK(gem.TimeString[5] == '-' && gem.TimeString[8] == '-', "TimeString has '-' separators at the expected positions");

    CHECK(gem.iTimeFormat == 0, "iTimeFormat defaults to 0 (gate-stub default -- golden's own ctor never inits it either, see header note)");
    CHECK(gem.GemClock.Length() == 12, "default iTimeFormat -> GemClock is the 12-byte 'else' branch format");

    gem.iTimeFormat = 2;   // 14-byte format
    gem.GetTimeInfo();
    CHECK(gem.GemClock.Length() == 14, "iTimeFormat==2 -> GemClock is the 14-byte format");

    gem.iTimeFormat = 3;   // ISO8601-ish, 19 bytes
    gem.GetTimeInfo();
    CHECK(gem.GemClock.Length() == 19 && gem.GemClock[5] == '-' && gem.GemClock[11] == 'T',
          "iTimeFormat==3 -> GemClock is the 19-byte ISO8601-ish format with a literal 'T'");
}

// ===========================================================================
//  [16] OnLine family / DoOnLine / small setters (OnLine/OnLineLocal/
//  OnLineRemote/OffLine/IsOnLine/GetOnLineMode/SetEstablishCommunicationsTryCount/
//  SetCanAcceptHostOnLineRequest/CloseCommuncation)
// ===========================================================================
static void test_online_family_and_misc()
{
    printf("\n[16] OnLine family / DoOnLine / misc small setters\n");

    THGem gem;

    gem.OnLine(true);
    CHECK(gem.GetOnLineMode() == true, "OnLine(true) sets OnLineLocal mode");
    CHECK(gem.IsOnLine() == false, "OnLine(Mode) alone does not yet flip bOnLine (DoOnLine's job)");
    CHECK(gem.iStartOnLineTask == 1, "OnLine(Mode) arms iStartOnLineTask=1");

    bool r1 = gem.DoOnLine();   // case 1 -> Task=200
    CHECK(r1 == false && gem.iStartOnLineTask == 200, "DoOnLine case 1 advances Task to 200, returns false");
    bool r2 = gem.DoOnLine();   // case 200 -> OnlineLocalOrRemote(); bOnLine=true; return true
    CHECK(r2 == true && gem.IsOnLine() == true && gem.bStartOnLine == false,
          "DoOnLine case 200 completes: bOnLine=true, bStartOnLine=false, returns true");

    gem.OffLine();
    CHECK(gem.IsOnLine() == false && gem.bStartOnLine == false, "OffLine clears bOnLine/bStartOnLine");

    gem.OnLineRemote();
    CHECK(gem.GetOnLineMode() == false, "OnLineRemote sets OnLineLocal mode false");
    gem.OnLineLocal();
    CHECK(gem.GetOnLineMode() == true, "OnLineLocal sets OnLineLocal mode true");

    gem.SetEstablishCommunicationsTryCount(7);
    CHECK(gem.iEstablishCommunicationsTryCount == 7, "SetEstablishCommunicationsTryCount stores the value");

    gem.SetCanAcceptHostOnLineRequest(true);   // golden: truly empty body -- must not crash
    CHECK(true, "SetCanAcceptHostOnLineRequest (golden empty body) does not crash");

    gem.clientGem->Active = true;
    gem.bCloseCommuncation = false;
    gem.CloseCommuncation();
    CHECK(gem.bCloseCommuncation == true && gem.clientGem->Active == false,
          "CloseCommuncation sets bCloseCommuncation and forces clientGem->Active=false");
}

// ===========================================================================
//  [17] StringOut(1-arg) / StringBinaryOut -- SaveSECSGEMErrToLog is
//  deliberately NOT called directly here (hardcoded absolute path outside
//  the repo -- see this file's own SAVE-related SAFETY notes above and at
//  the top of this file); it IS exercised indirectly via srvGemClientError
//  in test [12] above.
// ===========================================================================
static void test_stringout_and_binaryout()
{
    printf("\n[17] StringOut(1-arg) / StringBinaryOut\n");

    THGem gem;
    int before = gem.WaitShowString->Count;
    gem.StringOut("hello");
    CHECK(gem.WaitShowString->Count == before + 1 && gem.WaitShowString->GetString(before) == "hello",
          "StringOut appends to WaitShowString");
    CHECK(gem.LogDataString->Count == before + 1 && gem.LogDataString->GetString(before) == "hello",
          "StringOut also appends to LogDataString");

    int logBefore = gem.LogDataString->Count;
    gem.StringBinaryOut("ignored");
    CHECK(gem.LogDataString->Count == logBefore, "StringBinaryOut (golden: fully commented-out body) is a true no-op");
}

// ===========================================================================
//  [18] InitialHGem / SaveSystemDefault round-trip -- scratch GemSystemIniPath
//  (NOT the real D:\HT9045\SECS tree), same discipline as test [8]'s
//  GemSystemPath. Runs under CUSTOMER_CODE==CC_MAXIM_THAILAND so
//  ckAddDefaultReport's ReadIniData round-trip branch is the one actually
//  exercised (golden forces ckAddDefaultReport->Checked=true unconditionally
//  for every OTHER customer -- see InitialHGem's own definition).
// ===========================================================================
static void test_initialhgem_savesystemdefault_roundtrip()
{
    printf("\n[18] InitialHGem / SaveSystemDefault (scratch ini path)\n");

    const AnsiString kScratchDir = "uHGemEquipment_test_scratch_ini";
    ForceDirectories(kScratchDir);
    const AnsiString kIniPath = kScratchDir + "\\GemTest.ini";
    DeleteFile(kIniPath);   // start clean, in case a prior run left it behind

    int savedCustomerCode = CUSTOMER_CODE;
    CUSTOMER_CODE = CC_MAXIM_THAILAND;

    THGem gemWriter;
    gemWriter.GemSystemIniPath = kIniPath;
    gemWriter.OnLineOrOffLine->Items.Count = 2;   // matches golden .dfm Items.Strings count
    gemWriter.RemoteOrLocal->Items.Count = 2;
    gemWriter.rgRole->Items.Count = 2;

    gemWriter.rgRole->ItemIndex = 1;   // [2] Active role
    gemWriter.edtIP->Text = "10.20.30.40";
    gemWriter.edtPort->Text = "5100";
    gemWriter.edDeviceID->Text = "7";
    gemWriter.edtT3TimeOut->Text = "45";
    gemWriter.GemCheckBoxAcceptHostOnlineRequest->Checked = false;
    gemWriter.ckAddDefaultReport->Checked = false;
    gemWriter.SaveSystemDefault();
    CHECK(FileExists(kIniPath), "SaveSystemDefault created the scratch ini file");

    THGem gemReader;
    gemReader.GemSystemIniPath = kIniPath;
    gemReader.OnLineOrOffLine->Items.Count = 2;
    gemReader.RemoteOrLocal->Items.Count = 2;
    gemReader.rgRole->Items.Count = 2;
    gemReader.InitialHGem();

    CHECK(gemReader.rgRole->ItemIndex == 1, "InitialHGem round-trips rgRole->ItemIndex (ActiveOrPassive)");
    CHECK(gemReader.bUseClientSocket == true, "InitialHGem derives bUseClientSocket from rgRole->ItemIndex==1");
    CHECK(gemReader.edtIP->Text == "10.20.30.40", "InitialHGem round-trips edtIP->Text (Address)");
    CHECK(gemReader.clientGem->Address == "10.20.30.40", "InitialHGem also propagates edtIP->Text into clientGem->Address");
    CHECK(gemReader.edtIP->Enabled == true, "InitialHGem: bUseClientSocket==true -> edtIP->Enabled=true");
    CHECK(gemReader.edDeviceID->Text == "7", "InitialHGem round-trips edDeviceID->Text (DeviceID)");
    CHECK(gemReader.T3TimeOut == 45, "InitialHGem round-trips T3TimeOut (re-parsed via atoi after ReadIniData)");
    CHECK(gemReader.GemCheckBoxAcceptHostOnlineRequest->Checked == false, "InitialHGem round-trips AcceptHostOnlineRequest");
    CHECK(gemReader.ckAddDefaultReport->Checked == false,
          "InitialHGem round-trips AddDefaultReport under CC_MAXIM_THAILAND (the one customer where it's actually read from ini)");

    CUSTOMER_CODE = savedCustomerCode;
    DeleteFile(kIniPath);
    RemoveDir(kScratchDir);
}

// ===========================================================================
//  [19] ManualCreatergRoleClick -- bShow gate + bUseClientSocket/edtIP->Enabled toggle
// ===========================================================================
static void test_manualcreatergroleclick()
{
    printf("\n[19] ManualCreatergRoleClick\n");

    THGem gem;
    gem.bShow = false;
    gem.rgRole->ItemIndex = 1;
    gem.bUseClientSocket = false;
    gem.ManualCreatergRoleClick(NULL);
    CHECK(gem.bUseClientSocket == false, "ManualCreatergRoleClick: bShow==false -> guarded no-op (golden's own bShow gate)");

    gem.bShow = true;
    gem.ManualCreatergRoleClick(NULL);
    CHECK(gem.bUseClientSocket == true, "ManualCreatergRoleClick: bShow==true, rgRole->ItemIndex==1 -> bUseClientSocket=true");
    CHECK(gem.edtIP->Enabled == true, "ManualCreatergRoleClick: bUseClientSocket==true -> edtIP->Enabled=true");

    gem.rgRole->ItemIndex = 0;
    gem.ManualCreatergRoleClick(NULL);
    CHECK(gem.bUseClientSocket == false, "ManualCreatergRoleClick: rgRole->ItemIndex==0 -> bUseClientSocket=false");
    CHECK(gem.edtIP->Enabled == false, "ManualCreatergRoleClick: bUseClientSocket==false -> edtIP->Enabled=false");
}

// ===========================================================================
//  [20] ProcessShow -- flushes WaitShowString into DB (via the global HGem
//  pointer, matching golden's own `HGem->` dereference -- see this file's
//  header comment on THGem::ProcessShow).
// ===========================================================================
static void test_processshow()
{
    printf("\n[20] ProcessShow\n");

    THGem gem;
    gem.DB = new THGemMemo();
    THGem *savedHGem = HGem;
    HGem = &gem;

    gem.WaitShowString->Add("line1");
    gem.WaitShowString->Add("line2");
    gem.ProcessShow();
    CHECK(gem.DB->Lines->Count == 2, "ProcessShow: DB->Lines receives the 2 flushed lines");
    CHECK(gem.DB->Lines->GetString(0) == "line1" && gem.DB->Lines->GetString(1) == "line2",
          "ProcessShow: DB->Lines content matches WaitShowString, in order");
    CHECK(gem.WaitShowString->Count == 0, "ProcessShow clears WaitShowString after flushing");

    gem.DB->Clear();
    gem.ProcessShow();   // WaitShowString now empty -> golden's own `if(Count!=0)` guard -> no-op
    CHECK(gem.DB->Lines->Count == 0, "ProcessShow: empty WaitShowString -> guarded no-op");

    HGem = savedHGem;
    delete gem.DB;
    gem.DB = NULL;
}

// ===========================================================================
//  [21] StringOut(AnsiString,TColor) 2-arg overload -- DB!=NULL guard, appends
//  to both DB->Lines and LogDataString, sets DB->SelStart. The `C` (color)
//  parameter is accepted but never actually used anywhere in golden's own
//  body (verified by reading it) -- not asserted on here for that reason.
// ===========================================================================
static void test_stringout_2arg()
{
    printf("\n[21] StringOut(AnsiString,TColor) 2-arg overload\n");

    THGem gem;
    int before = gem.LogDataString->Count;
    gem.StringOut("ignored-no-db", clRed);
    CHECK(gem.LogDataString->Count == before, "StringOut(2-arg): DB==NULL -> guarded no-op");

    gem.DB = new THGemMemo();
    gem.StringOut("hello-color", clLime);
    CHECK(gem.DB->Lines->Count == 1 && gem.DB->Lines->GetString(0) == "hello-color",
          "StringOut(2-arg): DB!=NULL -> appends to DB->Lines");
    CHECK(gem.LogDataString->GetString(gem.LogDataString->Count - 1) == "hello-color",
          "StringOut(2-arg) also appends to LogDataString");
    CHECK(gem.DB->SelStart == gem.DB->Lines->Count - 1, "StringOut(2-arg) sets DB->SelStart to the last line's index");

    delete gem.DB;
    gem.DB = NULL;
}

// ===========================================================================
//  [22] DoUpdateStatus -- throttle, normal Disconnected/Offline refresh +
//  EventReport(141), the client-socket-active + OnLine-Local transitions +
//  EventReport(92), and the KYEC 30-second forced-disconnect branch (now
//  exercised against the REAL elapsed-time HTimer, D6 -- see the assertion's
//  own updated comment below for exactly what changed vs the prior
//  always-fires stand-in).
// ===========================================================================
static void test_do_update_status()
{
    printf("\n[22] DoUpdateStatus\n");

    THGem gem;
    gem.SECSConnectionState = new THGemPanel();
    gem.GEMCommunicatingState = new THGemPanel();
    gem.GemPanelControlState = new THGemPanel();
    gem.BtnEnableComm = new THGemSpeedButton();
    gem.GemBtnOfflineRequest = new THGemSpeedButton();
    gem.GemBtnOnlineRequest = new THGemSpeedButton();
    gem.GemBtnOnlineRemote = new THGemSpeedButton();
    gem.GemBtnOnlineLocal = new THGemSpeedButton();

    // --- throttle: only every 10th call does real work ---------------------
    for (int i = 0; i < 9; ++i)
        gem.DoUpdateStatus();
    CHECK(gem.SECSConnectionState->Caption == "", "DoUpdateStatus: throttled (9 calls < 10) -- no refresh yet");

    // --- 10th call: passive (srvGem) role, disconnected, offline -----------
    ResetSimEventReport();
    gem.DoUpdateStatus();
    CHECK(gem.SECSConnectionState->Caption == "SECS GEM Disconnection" && gem.SECSConnectionState->Color == clRed,
          "DoUpdateStatus: srvGem inactive -> \"SECS GEM Disconnection\" / clRed");
    CHECK(gem.GEMCommunicatingState->Caption == "1:Disable" && gem.GEMCommunicatingState->Color == clRed,
          "DoUpdateStatus: bConnect==false, non-KYEC/SIGURD -> \"1:Disable\" / clRed");
    CHECK(gem.GemPanelControlState->Caption == "Off Line" && gem.GemPanelControlState->Color == clRed,
          "DoUpdateStatus: bOnLine==false -> \"Off Line\" / clRed");
    CHECK(gem.GemBtnOfflineRequest->Enabled == false && gem.GemBtnOnlineRequest->Enabled == true,
          "DoUpdateStatus: IsOnLine()==false -> Offline button disabled, Online button enabled");
    CHECK(gem.GemBtnOnlineRemote->Enabled == false && gem.GemBtnOnlineLocal->Enabled == true,
          "DoUpdateStatus: GetOnLineMode()==false -> Remote button disabled, Local button enabled");
    CHECK(gem.BtnEnableComm->Enabled == true, "DoUpdateStatus: !IsConnect() -> BtnEnableComm enabled");
    CHECK(g_SimLastEventReportCeid == 141 && g_SimEventReportCount == 1,
          "DoUpdateStatus: GemControlState 0->1 transition fires EventReport(141) exactly once");

    // --- switch to client-socket role, becomes Active -----------------------
    gem.bUseClientSocket = true;
    gem.clientGem->Active = true;
    gem.ctUpdateStatus = 9;
    gem.DoUpdateStatus();
    CHECK(gem.SECSConnectionState->Caption == "SECS GEM Connection" && gem.SECSConnectionState->Color == clLime,
          "DoUpdateStatus: clientGem becomes Active -> \"SECS GEM Connection\" / clLime");
    CHECK(gem.bConnect == false,
          "DoUpdateStatus: golden quirk preserved verbatim -- bConnect forced false even while the client socket is Active");

    // --- go OnLine (Local): GemControlState 1->2, EventReport(141) then (92) ---
    gem.bOnLine = true;
    gem.bOnLineLocal = true;
    gem.ctUpdateStatus = 9;
    gem.DoUpdateStatus();
    CHECK(gem.GemPanelControlState->Caption == "On Line Local" && gem.GemPanelControlState->Color == clLime,
          "DoUpdateStatus: bOnLine+bOnLineLocal -> \"On Line Local\" / clLime");
    CHECK(gem.GemBtnOfflineRequest->Enabled == true && gem.GemBtnOnlineRequest->Enabled == false,
          "DoUpdateStatus: IsOnLine()==true -> Offline button enabled, Online button disabled");
    CHECK(gem.GemBtnOnlineRemote->Enabled == true && gem.GemBtnOnlineLocal->Enabled == false,
          "DoUpdateStatus: GetOnLineMode()==true -> Remote button enabled, Local button disabled");
    CHECK(g_SimLastEventReportCeid == 92 && g_SimEventReportCount == 3,
          "DoUpdateStatus: GemControlState 1->2 fires EventReport(141), then the OldGemControlState change fires EventReport(92) -- 2 more reports (cumulative 3)");

    // --- KYEC 30-second forced-disconnect branch (STRUCTURAL ONLY) ---------
    // Needs 2 consecutive primed calls to observe for real: call A settles
    // GEMCommunicatingState->Caption to "1:OffLine" (the KYEC guard inside
    // THIS SAME call still reads the STALE pre-call Caption, so it does not
    // yet fire); call B then sees the now-settled "1:OffLine" and the guard
    // fires.
    int savedCustomerCode = CUSTOMER_CODE;
    THGem *savedHGem = HGem;
    CUSTOMER_CODE = CC_KYEC_LEE;
    HGem = &gem;

    gem.flag2UpdateStatus = true;   // force the GEMCommunicatingState block to re-evaluate this call
    gem.ctUpdateStatus = 9;
    gem.DoUpdateStatus();   // call A
    CHECK(gem.GEMCommunicatingState->Caption == "1:OffLine",
          "DoUpdateStatus (KYEC): bConnect==false -> \"1:OffLine\" (not \"1:Disable\") once CUSTOMER_CODE==CC_KYEC_LEE");

    gem.ctUpdateStatus = 9;
    gem.DoUpdateStatus();   // call B: KYEC guard now sees "SECS GEM Connection" + "1:OffLine" -> ARMS SECSGEM_DoSeparate.SetSecAndOn(30)
    // AI(W906-uHGemEquipment-BucketC) 20260717: UPDATED -- SECSGEM_DoSeparate
    // is now the REAL elapsed-time HTimer (D6), so arming it with
    // SetSecAndOn(30) inside call B and polling Off() a few microseconds
    // later (same call) correctly returns false -- the forced-disconnect
    // does NOT fire immediately anymore (this is the FIX for the prior
    // Bucket-B FLAGGED LIMITATION, where an always-fires stub made this
    // branch trigger on the very same poll it was armed). clientGem->Active
    // therefore stays true. Real elapsed-time semantics for THIS SAME HTimer
    // type are separately verified for real in test [T2] below (which can
    // afford a short real Sleep(); this 30-second golden constant cannot).
    CHECK(gem.clientGem->Active == true,
          "DoUpdateStatus (KYEC): forced-disconnect branch arms a REAL 30-second HTimer (D6) -- "
          "does NOT fire on the same poll it was armed (clientGem->Active stays true)");

    CUSTOMER_CODE = savedCustomerCode;
    HGem = savedHGem;

    delete gem.SECSConnectionState;
    delete gem.GEMCommunicatingState;
    delete gem.GemPanelControlState;
    delete gem.BtnEnableComm;
    delete gem.GemBtnOfflineRequest;
    delete gem.GemBtnOnlineRequest;
    delete gem.GemBtnOnlineRemote;
    delete gem.GemBtnOnlineLocal;
}

// =============================================================================
//  Bucket C (W906-uHGemEquipment-BucketC 20260717): socket receive pump / T3
//  timeout / HSMS control-message handshake / Timer1Timer master state
//  machine. Golden reference: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/
//  uHGemEquipment.cpp (clientGemRead :9008-9028; ProcessSocketReceiveData
//  :9030-9207; Timer1Timer :5176-5527; DoConnect :3536-3599; DoSelect
//  :3496-3509; DoSeparate :3518-3531; DoProcessSFNoResponse :4604-4694;
//  SelectRsp/DeselectRsp/LinktestRsp :8707-8765; ProcessReceiceData
//  :8772-8989; CheckSFCodeResponse :7020-7058; SendLocalData :1985-2107).
//
//  *** SIDE-EFFECT WARNING (read before extending) ***
//  Every test below that pumps ProcessSocketReceiveData (directly or via
//  Timer1Timer) reaches SaveSECSGEMTextToLog(), which -- like the
//  already-committed SaveSECSGEMErrToLog -- unconditionally APPENDS to
//  D:\SECS_GEM_LOGS\<yyyy>\<mm_dd>\SECSGEM_TextLog_<hh>.txt (creating the
//  directory tree if absent). Same acceptable-risk posture as test [12]'s own
//  SAFETY NOTE (append-only diagnostic text, not destructive to existing
//  data) -- NOT separately byte-identity-guarded per test here (that would
//  require the same capture/restore dance at every single call site below);
//  instead this translation agent's own build+verify pass performs ONE
//  before/after byte-identity check across the ENTIRE test run (see the
//  final report) covering both SECSGEM_ErrLog_<hh>.txt (test [12]) and
//  SECSGEM_TextLog_<hh>.txt (all of Bucket C).
// =============================================================================

// ---------------------------------------------------------------------------
//  BucketCPanels -- RAII helper: the 3 panels + 5 speed buttons DoUpdateStatus
//  unconditionally dereferences (golden does NOT null-guard the 5
//  TSpeedButton*s -- see uHGemEquipment.h's own THGemSpeedButton comment).
//  Needed by any test that calls Timer1Timer() (which calls DoUpdateStatus()
//  on every pump) -- test [22] above wires the identical set inline; this
//  factors it out for the several Bucket-C tests that need the same thing.
// ---------------------------------------------------------------------------
namespace {
struct BucketCPanels
{
    THGem &g;
    explicit BucketCPanels(THGem &gem) : g(gem)
    {
        g.SECSConnectionState = new THGemPanel();
        g.GEMCommunicatingState = new THGemPanel();
        g.GemPanelControlState = new THGemPanel();
        g.BtnEnableComm = new THGemSpeedButton();
        g.GemBtnOfflineRequest = new THGemSpeedButton();
        g.GemBtnOnlineRequest = new THGemSpeedButton();
        g.GemBtnOnlineRemote = new THGemSpeedButton();
        g.GemBtnOnlineLocal = new THGemSpeedButton();
    }
    ~BucketCPanels()
    {
        delete g.SECSConnectionState; g.SECSConnectionState = NULL;
        delete g.GEMCommunicatingState; g.GEMCommunicatingState = NULL;
        delete g.GemPanelControlState; g.GemPanelControlState = NULL;
        delete g.BtnEnableComm; g.BtnEnableComm = NULL;
        delete g.GemBtnOfflineRequest; g.GemBtnOfflineRequest = NULL;
        delete g.GemBtnOnlineRequest; g.GemBtnOnlineRequest = NULL;
        delete g.GemBtnOnlineRemote; g.GemBtnOnlineRemote = NULL;
        delete g.GemBtnOnlineLocal; g.GemBtnOnlineLocal = NULL;
    }
    BucketCPanels(const BucketCPanels&) = delete;
    BucketCPanels& operator=(const BucketCPanels&) = delete;
};
} // anonymous namespace

// ===========================================================================
//  [T1] vclcompat::TMemoryStream shim -- write/read/seek/clear/LoadFromStream.
// ===========================================================================
static void test_memorystream_shim()
{
    printf("\n[T1] vclcompat::TMemoryStream shim\n");

    TMemoryStream ms;
    CHECK((int)ms.Size == 0, "fresh TMemoryStream: Size==0");
    CHECK(ms.Position == 0, "fresh TMemoryStream: Position==0");

    const char data1[] = "Hello";
    ms.WriteBuffer(data1, 5);
    CHECK((int)ms.Size == 5, "WriteBuffer(5 bytes) at Position 0 -> Size==5");
    CHECK(ms.Position == 5, "WriteBuffer advances Position by Count");

    // WriteBuffer at Position 0 with a SHORTER write must KEEP the longer
    // existing tail (real VCL TStream::WriteBuffer semantics) -- only Clear()
    // resets Size, not a short overwrite.
    ms.Position = 0;
    const char data2[] = "Wr";
    ms.WriteBuffer(data2, 2);
    CHECK((int)ms.Size == 5, "WriteBuffer at Position 0 with a SHORTER write keeps the longer existing tail (Size still 5)");
    char readback[8] = {0};
    ms.Position = 0;
    ms.ReadBuffer(readback, 5);
    CHECK(std::string(readback, 5) == "Wrllo", "content is \"Wr\"+\"llo\" (tail beyond the short write survived)");

    // Seek(0, soFromEnd) append pattern.
    ms.Seek(0, soFromEnd);
    CHECK(ms.Position == 5, "Seek(0, soFromEnd) -> Position==Size");
    const char data3[] = "!!";
    ms.WriteBuffer(data3, 2);
    CHECK((int)ms.Size == 7, "append via Seek(0,soFromEnd)+WriteBuffer grows Size");

    ms.Seek(0, soFromBeginning);
    CHECK(ms.Position == 0, "Seek(0, soFromBeginning) -> Position==0");
    char full[16] = {0};
    ms.ReadBuffer(full, 7);
    CHECK(std::string(full, 7) == "Wrllo!!", "full round-trip content matches expected");

    ms.Clear();
    CHECK((int)ms.Size == 0 && ms.Position == 0, "Clear() resets Size and Position to 0");

    // LoadFromStream: replaces dst's content wholesale, rewinds Source to 0.
    TMemoryStream src;
    const char data4[] = "abcdef";
    src.WriteBuffer(data4, 6);
    TMemoryStream dst;
    const char preexisting[] = "XXXXXXXXXX";
    dst.WriteBuffer(preexisting, 10);
    dst.LoadFromStream(&src);
    CHECK((int)dst.Size == 6, "LoadFromStream replaces dst's content with src's (Size==6, not 10)");
    CHECK(dst.Position == 0, "LoadFromStream leaves dst->Position at 0");
    CHECK(src.Position == 0, "LoadFromStream rewinds Source to 0 as a side effect");
    char dstRead[8] = {0};
    dst.ReadBuffer(dstRead, 6);
    CHECK(std::string(dstRead, 6) == "abcdef", "dst content matches src's content byte-for-byte");

    // ReadBuffer shortfall -> tripwire throw.
    // NOTE: the obvious variable name `small` collides with a Windows
    // <rpcndr.h> macro (`small` -> `char`, an old MIDL basic-type alias
    // pulled in transitively via windows.h) -- named `tinyStream` instead.
    bool threw = false;
    TMemoryStream tinyStream;
    const char tiny[] = "ab";
    tinyStream.WriteBuffer(tiny, 2);
    tinyStream.Position = 0;
    char buf3[8];
    try { tinyStream.ReadBuffer(buf3, 3); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw, "ReadBuffer shortfall throws std::runtime_error (EReadError tripwire)");
}

// ===========================================================================
//  [T2] HTimer real elapsed-time semantics.
//
//  DEVIATION (flagged prominently, per this wave's own instructions): the
//  REAL HTimer (D6, uHGemEquipment.cpp) is DELIBERATELY TU-local (anonymous
//  namespace -- see that file's own comment on why: ODR-safety vs.
//  atester_shims.h's own separate HTimer stub). It therefore has INTERNAL
//  linkage and its type is not nameable from this SEPARATE translation unit
//  -- `HTimer t;` cannot compile here. This test instead instantiates a
//  byte-for-byte MIRROR of that exact struct (the SAME field list and
//  Off()/SetSecAndOn() bodies, ported from the SAME golden
//  D:\HT9045\elec\Component\htimer.cpp this wave's real HTimer itself was
//  ported from) to verify the ALGORITHM directly with real short sleeps.
//  This is NOT the same as testing the literal TU-local symbol -- if
//  uHGemEquipment.cpp's own HTimer is ever edited, this mirror must be
//  updated to match by hand; there is no compiler enforcement linking the
//  two. The REAL TU-local instances' actual wiring/behavior is separately
//  verified INDIRECTLY: test [22]'s KYEC branch (arms
//  SECSGEM_DoSeparate.SetSecAndOn(30), confirms it does NOT fire on the same
//  poll) and T10 below (SECSGEM_DoSeparateWait's 5-second window blocking
//  Timer1Timer's pump, sleepless variant). A future wave should consider
//  promoting a single shared vclcompat/HTimer (already flagged as a risk in
//  the design brief) to close this testability gap for good.
// ===========================================================================
namespace {
struct HTimerMirror
{
    DWORD ulStartTicks;
    int   iTimeLen;
    bool  Paused;
    bool  InUsed;

    HTimerMirror() : ulStartTicks(0), iTimeLen(0), Paused(false), InUsed(false) {}
    void SetSec(double iTime) { iTimeLen = static_cast<int>(iTime * 1000.0); }
    void On() { ulStartTicks = ::GetTickCount(); InUsed = true; }
    void SetSecAndOn(double iTime)
    {
        ulStartTicks = 0; iTimeLen = 0; InUsed = false; Paused = false;
        SetSec(iTime);
        On();
    }
    bool Off()
    {
        if (Paused) return false;
        if (ulStartTicks == 0) return false;
        if (iTimeLen <= 0) return true;
        DWORD ulLimited = ulStartTicks + static_cast<DWORD>(iTimeLen);
        DWORD ulNowTicks = ::GetTickCount();
        DWORD ulNowTicksOver = 0;
        if (ulLimited < ulStartTicks)
        {
            ulLimited = 0xFFFFFFFF - ulStartTicks + static_cast<DWORD>(iTimeLen);
            ulNowTicksOver = 0xFFFFFFFF - ulStartTicks + ulNowTicks;
            if (ulNowTicksOver < ulStartTicks && ulNowTicksOver > ulLimited) { InUsed = false; return true; }
        }
        else
        {
            if (ulNowTicks >= (ulStartTicks + static_cast<DWORD>(iTimeLen))) { InUsed = false; return true; }
        }
        return false;
    }
};
} // anonymous namespace

static void test_htimer_real_semantics()
{
    printf("\n[T2] HTimer real elapsed-time semantics (mirror -- see this test's own DEVIATION comment above)\n");

    HTimerMirror t1;
    CHECK(t1.Off() == false, "never-armed HTimer -> Off()==false");

    HTimerMirror t2;
    t2.SetSecAndOn(0.2);   // 200ms
    CHECK(t2.Off() == false, "SetSecAndOn(0.2) -> not yet elapsed immediately");
    ::Sleep(250);
    CHECK(t2.Off() == true, "SetSecAndOn(0.2) -> elapsed after a 250ms real sleep");
    CHECK(t2.Off() == true, "re-poll after expiry stays true (InUsed=false does not change Off()'s return)");
}

// ===========================================================================
//  [T3] Select handshake e2e (server role, Sim): clientGemRead (wired in the
//  ctor, D... this wave) -> ProcessSocketReceiveData -> ProcessReceiceData
//  (control head, real) -> SelectRsp -> SendLocalDataFrom -> real bytes on
//  the wire, exercised through a full Timer1Timer pump.
// ===========================================================================
static void test_select_handshake_e2e()
{
    printf("\n[T3] Select handshake e2e (server role, Sim, via Timer1Timer)\n");

    bool savedInitialOK = InitialOK;
    bool savedEnableSecsGem = IniConfig.bEnable_SECS_GEM;
    THGem *savedHGem = HGem;

    THGem g;
    BucketCPanels panels(g);
    HGem = &g;
    g.DB = new THGemMemo();
    g.OnLineOrOffLine->Items.Count = 2;
    g.RemoteOrLocal->Items.Count = 2;
    g.rgRole->Items.Count = 2;
    const AnsiString kScratchDir = "uHGemEquipment_test_scratch_t3";
    ForceDirectories(kScratchDir);
    g.GemSystemIniPath = kScratchDir + "\\Gem.ini";
    InitialOK = true;
    IniConfig.bEnable_SECS_GEM = true;

    g.Timer1Timer(NULL);   // pump #1: Timer1Task 1->10 (case 1 is unconditional); InitialHGem() runs (bFirstEntry)
    CHECK(g.Timer1Task == 10, "T3: after pump #1, Timer1Task advances 1->10");
    CHECK(g.bUseClientSocket == false, "T3: server (passive) role -- rgRole->ItemIndex defaults to 0");

    g.srvGem->Open();
    TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.5.5.5", 4001);
    CHECK(g.bServoSocketConnect == true, "T3: SimAcceptConnection fires srvGemClientConnect -> bServoSocketConnect=true");

    // Frame layout re-verified against SecsWireCodec::ProcessRemoteHead
    // (SecsWireCodec.cpp:1383-1398): [0..3]=length(=10, big-endian) [4..5]=
    // DeviceID(0xFFFF) [6]=MessageID_S|W_Bit(0x00) [7]=MessageID_F(0x00)
    // [8]=PType(0x00) [9]=SType(0x01=Select.req) [10..13]=SystemByte(=1).
    unsigned char frame[14] = {
        0x00,0x00,0x00,0x0A, 0xFF,0xFF, 0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x01
    };
    conn->SimPushReceive(frame, 14);   // -> clientGemRead (wired) -> RecvMemoryBuffer

    g.Timer1Timer(NULL);   // pump #2: ProcessSocketReceiveData decodes + dispatches -> SelectRsp -> SendLocalDataFrom

    const std::vector<char> &tx = conn->SimTxBuffer();
    CHECK(tx.size() == 14, "T3: SelectRsp's reply is a 14-byte HSMS frame (control message, no data items)");
    if (tx.size() == 14)
    {
        CHECK((unsigned char)tx[9] == 2, "T3: reply SType byte (offset 9) == 2 (Select.rsp)");
        unsigned echoedSystemByte = (static_cast<unsigned>((unsigned char)tx[10]) << 24) |
                                    (static_cast<unsigned>((unsigned char)tx[11]) << 16) |
                                    (static_cast<unsigned>((unsigned char)tx[12]) << 8)  |
                                     static_cast<unsigned>((unsigned char)tx[13]);
        CHECK(echoedSystemByte == 1, "T3: reply SystemByte echoes the request's SystemByte (1)");
    }
    CHECK((int)g.RecvMemoryBuffer->Size == 0, "T3: RecvMemoryBuffer drained after processing");
    CHECK((int)g.ProcBuffer->Size == 0, "T3: ProcBuffer cleared after a fully-consumed message");
    CHECK(g.bFirstBlock == true, "T3: bFirstBlock restored to true after a fully-consumed message");

    InitialOK = savedInitialOK;
    IniConfig.bEnable_SECS_GEM = savedEnableSecsGem;
    HGem = savedHGem;
    DeleteFile(g.GemSystemIniPath);
    RemoveDir(kScratchDir);
    delete g.DB; g.DB = NULL;
}

// ===========================================================================
//  [T4] Frame reassembly -- multiple messages in one read; a message split
//  across two reads (TempProcBuffer partial-frame path); a too-short
//  garbage read. Calls ProcessSocketReceiveData DIRECTLY (not via
//  Timer1Timer) -- T3 above already proved the full pump wiring; these
//  sub-tests focus on the reassembly bookkeeping itself.
// ===========================================================================
static void test_frame_reassembly()
{
    printf("\n[T4] Frame reassembly (multi-message / split-frame / garbage)\n");

    // (a) two concatenated Select.req frames in ONE SimPushReceive -> BOTH
    // get processed within a single ProcessSocketReceiveData call (its own
    // do-while walks every complete message still in the buffer).
    {
        THGem g;
        TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.1", 4100);
        g.srvGem->Open();
        unsigned char frame1[14] = { 0,0,0,0x0A, 0xFF,0xFF, 0,0,0,0x01, 0,0,0,0x01 };   // SystemByte=1
        unsigned char frame2[14] = { 0,0,0,0x0A, 0xFF,0xFF, 0,0,0,0x01, 0,0,0,0x02 };   // SystemByte=2
        unsigned char both[28];
        std::memcpy(both, frame1, 14);
        std::memcpy(both + 14, frame2, 14);
        conn->SimPushReceive(both, 28);
        g.ProcessSocketReceiveData();

        const std::vector<char> &tx = conn->SimTxBuffer();
        CHECK(tx.size() == 28, "T4(a): two concatenated Select.req frames -> two 14-byte Select.rsp replies (28 bytes total)");
        if (tx.size() == 28)
        {
            CHECK((unsigned char)tx[9] == 2 && (unsigned char)tx[13] == 1, "T4(a): reply #1 SType==2 (Select.rsp), echoes SystemByte==1");
            CHECK((unsigned char)tx[23] == 2 && (unsigned char)tx[27] == 2, "T4(a): reply #2 SType==2 (Select.rsp), echoes SystemByte==2");
        }
        CHECK((int)g.ProcBuffer->Size == 0, "T4(a): ProcBuffer fully drained after both messages consumed");
    }

    // (b) one frame split across two SimPushReceive calls -> the
    // TempProcBuffer partial-frame path (golden :9123-9136) buffers the
    // first chunk and waits; only the SECOND chunk completes the message.
    //
    // JUDGMENT CALL (flagged): a REAL, unpadded control message (Select.req)
    // is ALWAYS exactly 14 bytes (Value==10 -- CreateLocalHead's own
    // invariant; no DataItemOut call ever rides along a control message), so
    // there is no way to split ONE at a byte boundary that both (i) supplies
    // >=14 bytes up front (else the "<14 byte" ERROR path fires instead,
    // clearing the buffer outright rather than waiting) and (ii) still has
    // more to come. This test therefore uses an ARTIFICIALLY padded
    // Select.req (declared Value=11 instead of 10, with one extra trailing
    // byte) purely to give the frame a length beyond the 14-byte minimum --
    // NOT a real protocol shape, just the minimum change needed to make the
    // partial-frame path reachable at all with a hand-built buffer.
    // ProcessReceiceData's control-message head only ever reads Remote.SType
    // (a fixed-offset field within the first 14 bytes), so the trailing pad
    // byte does not affect dispatch -- SelectRsp() still fires for real once
    // reassembly completes.
    {
        THGem g;
        TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.2", 4101);
        g.srvGem->Open();
        unsigned char frame[15] = { 0,0,0,0x0B, 0xFF,0xFF, 0,0,0,0x01, 0,0,0,0x03, 0x99 };   // Value=11 (padded), SystemByte=3

        conn->SimPushReceive(frame, 14);   // first 14 of 15 bytes -- 1 byte still missing
        g.ProcessSocketReceiveData();
        CHECK(conn->SimTxBuffer().size() == 0, "T4(b): no reply yet after only the first 14 of 15 bytes arrived");
        CHECK(g.bFirstBlock == false, "T4(b): bFirstBlock cleared while a partial frame is buffered");

        conn->SimPushReceive(frame + 14, 1);   // the missing trailing byte
        g.ProcessSocketReceiveData();
        const std::vector<char> &tx = conn->SimTxBuffer();
        CHECK(tx.size() == 14, "T4(b): reply appears ONLY after the second chunk completes the frame");
        if (tx.size() == 14)
            CHECK((unsigned char)tx[9] == 2 && (unsigned char)tx[13] == 3, "T4(b): reply is Select.rsp echoing SystemByte==3");
        CHECK(g.bFirstBlock == true, "T4(b): bFirstBlock restored true once the reassembled frame is fully consumed");
    }

    // (c) garbage <14 bytes -> golden's own hardcoded error string, buffers cleared.
    {
        THGem g;
        TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.3", 4102);
        unsigned char garbage[5] = { 1,2,3,4,5 };
        conn->SimPushReceive(garbage, 5);
        g.ProcessSocketReceiveData();
        bool found = false;
        for (int i = 0; i < g.WaitShowString->Count; i++)
            if (g.WaitShowString->GetString(i) == "Err : Socket Buffer Length less than 14 byte ")
                found = true;
        CHECK(found, "T4(c): <14-byte garbage read logs golden's own hardcoded error string");
        CHECK((int)g.ProcBuffer->Size == 0, "T4(c): ProcBuffer cleared after the malformed-length error path");
        CHECK(conn->SimTxBuffer().size() == 0, "T4(c): no reply sent for malformed input");
    }
}

// ===========================================================================
//  [T5] Data message with gated dispatcher: a real, syntactically-valid
//  S1F14 (W=0) frame, built via a throwaway SecsWireCodec's own real
//  encoder (InitLocalHead/DataItemOut) rather than hand-encoded bytes.
// ===========================================================================
static void test_data_message_gated_dispatch()
{
    printf("\n[T5] Data message (S1F14, W=0) -- gated dispatcher: no crash, no reply, SReceiveData decoded\n");

    THGem g;
    TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.9", 4200);
    g.srvGem->Open();

    SecsWireCodec builder;
    builder.InitLocalHead(1, 14, 0);   // S1,F14, W=0 -- shape of golden's own [S1F14] Connect Request Acknowledge
    builder.DataItemOut(HType.ASCII_TYPE, AnsiString("OK"));
    conn->SimPushReceive(builder.LocalBuffer.data(), static_cast<int>(builder.LocalLength_4));

    bool threw = false;
    try { g.ProcessSocketReceiveData(); }
    catch (...) { threw = true; }
    CHECK(threw == false, "T5: data-message path does not throw (Ifor-20260421 try/catch never needed to fire)");

    CHECK(g.WireCodec.Remote.MessageID_S == 1 && g.WireCodec.Remote.MessageID_F == 14,
          "T5: WireCodec.Remote decodes MessageID_S/F == 1/14");
    CHECK(conn->SimTxBuffer().size() == 0, "T5: gated S,F dispatch tail sends no reply (HSys.MyGem stays NULL)");
    CHECK(g.WireCodec.SReceiveData->Count > 0, "T5: ShowSML's real ProcessSML decode populated SReceiveData with tokens while bReceiveData was true");
    CHECK(g.WireCodec.bReceiveData == false, "T5: bReceiveData reset to false once the message is fully consumed");
}

// ===========================================================================
//  [T6] T3 send-side record: an outbound W=1 request records "S F+1
//  SystemByte" into SFCodeResponseList/TimeLeft (T3TimeOut=30 golden ctor
//  default -> TimeLeft "300").
// ===========================================================================
static void test_t3_send_side_record()
{
    printf("\n[T6] T3 send-side record (SFCodeResponseList/TimeLeft)\n");

    THGem g;
    TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.10", 4300);
    g.srvGem->Open();
    CHECK(g.bServoSocketConnect == true, "T6: SimAcceptConnection sets bServoSocketConnect (fixture precondition)");

    g.WireCodec.InitLocalHead(1, 13, 1);   // S1,F13, W=1 -- a request expecting a reply -> T3 record armed
    g.SendLocalData();

    AnsiString expected = AnsiString(1) + " " + AnsiString(14) + " " + AnsiString(g.WireCodec.Local.SystemByte);
    int idx = g.SFCodeResponseList->Items->IndexOf(expected);
    CHECK(idx != -1, "T6: SendLocalData (W=1, odd F) records \"S F+1 SystemByte\" into SFCodeResponseList");
    CHECK(idx >= 0 && g.TimeLeft->GetString(idx) == "300",
          "T6: matching TimeLeft slot == \"300\" (T3TimeOut=30 golden ctor default * 10)");
    CHECK(conn->SimTxBuffer().size() > 0, "T6: SendLocalData put real bytes on the wire (srvGem path)");
}

// ===========================================================================
//  [T7] T3 timeout -> S9F9 through the cross-class seam: a REAL HTGem
//  instance's own WireCodec, hooked to forward through THGem's real send --
//  proving the SendLocalDataHook seam (D2) works end-to-end TODAY, without
//  waiting for the future SystemModularInitial wiring wave.
// ===========================================================================
static void test_t3_timeout_s9f9_cross_class_seam()
{
    printf("\n[T7] T3 timeout -> S9F9 through the cross-class seam (HTGem.WireCodec.SendLocalDataHook)\n");

    THGem g;
    TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.11", 4400);
    g.srvGem->Open();

    HTGem hgem;
    HSys.MyGem = &hgem;
    hgem.WireCodec.SendLocalDataHook = [&g](SecsWireCodec &wc) { g.SendLocalDataFrom(wc); };

    // Seed one T3 record via the SAME send path T6 exercises.
    g.WireCodec.InitLocalHead(1, 13, 1);
    g.SendLocalData();
    AnsiString expected = AnsiString(1) + " " + AnsiString(14) + " " + AnsiString(g.WireCodec.Local.SystemByte);
    int seededIndex = g.SFCodeResponseList->Items->IndexOf(expected);
    CHECK(seededIndex != -1, "T7: fixture -- T3 record seeded (same pattern as T6)");

    conn->SimClearTx();
    g.TimeLeft->Strings[seededIndex] = "1";   // one tick from expiry
    g.SystemSec = 1;                          // != iOldSecProcessSFNoResponse's ctor default (0)

    g.DoProcessSFNoResponse();

    CHECK(g.SFCodeResponseList->Items->IndexOf(expected) == -1, "T7: expired T3 record removed from SFCodeResponseList");
    const std::vector<char> &tx = conn->SimTxBuffer();
    CHECK(tx.size() >= 14, "T7: DoProcessSFNoResponse's S9F9 send produced real bytes on the wire via the cross-class hook");
    if (tx.size() >= 14)
        CHECK((unsigned char)tx[6] == 0x09 && (unsigned char)tx[7] == 0x09, "T7: reply header is S9F9 (Transaction Timer Timeout)");
    std::string body(tx.begin() + 14, tx.end());
    CHECK(body.find("T3   time out") != std::string::npos, "T7: S9F9 payload carries the \"T3   time out\" text");

    HSys.MyGem = NULL;
}

// ===========================================================================
//  [T8] CheckSFCodeResponse response-matching: exact match removes the
//  record; a mismatch leaves it untouched; a TimeLeft/SFCodeResponseList
//  COUNT mismatch clears both lists outright (golden's own recovery path).
// ===========================================================================
static void test_checksfcoderesponse()
{
    printf("\n[T8] CheckSFCodeResponse response-matching\n");

    THGem g;

    g.SFCodeResponseList->Items->Add("1 14 5");
    g.TimeLeft->Add("300");
    g.WireCodec.Remote.MessageID_S = 1;
    g.WireCodec.Remote.MessageID_F = 14;
    g.WireCodec.Remote.SystemByte = 5;
    g.CheckSFCodeResponse();
    CHECK(g.SFCodeResponseList->Items->Count == 0 && g.TimeLeft->Count == 0,
          "T8: exact-match response removes the ONE seeded record from both lists");

    g.SFCodeResponseList->Items->Add("1 14 5");
    g.TimeLeft->Add("300");
    g.WireCodec.Remote.SystemByte = 999;   // no longer matches "1 14 5"
    g.CheckSFCodeResponse();
    CHECK(g.SFCodeResponseList->Items->Count == 1 && g.TimeLeft->Count == 1,
          "T8: mismatched response leaves the seeded record untouched");

    g.TimeLeft->Add("150");   // now TimeLeft->Count(2) != SFCodeResponseList->Items->Count(1)
    g.CheckSFCodeResponse();
    CHECK(g.SFCodeResponseList->Items->Count == 0 && g.TimeLeft->Count == 0,
          "T8: TimeLeft/SFCodeResponseList count mismatch -> CheckSFCodeResponse clears BOTH lists");
}

// ===========================================================================
//  [T9] DoConnect state walk.
// ===========================================================================
static void test_doconnect_state_walk()
{
    printf("\n[T9] DoConnect state walk\n");

    THGem g;
    TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.20", 4500);
    g.srvGem->Open();
    conn->SimClearTx();

    CHECK(g.iStartConnectTask == 1, "T9: fresh THGem starts at iStartConnectTask==1");
    int ret1 = g.DoConnect();   // case 1: DoSelect() sends Select.req, arms ConnectDelay, Task=100
    CHECK(ret1 == 0 && g.iStartConnectTask == 100, "T9: DoConnect case 1 sends Select.req and advances to Task 100");
    CHECK(conn->SimTxBuffer().size() == 14, "T9: DoSelect's Select.req produced a 14-byte control frame on the wire");
    if (conn->SimTxBuffer().size() == 14)
        CHECK((unsigned char)conn->SimTxBuffer()[9] == 1, "T9: sent frame's SType byte == 1 (Select.req)");

    g.bWaitSelectRsp = true;   // simulate a Select.rsp having arrived (bypassing a full T3-style e2e injection)
    int ret2 = g.DoConnect();   // case 100: bWaitSelectRsp==true -> S1F13 (D3 null-guarded, HSys.MyGem==NULL) -> Task=200
    CHECK(ret2 == 0 && g.iStartConnectTask == 200,
          "T9: DoConnect case 100 (bWaitSelectRsp) advances to Task 200 -- S1F13 null-guard did not crash with HSys.MyGem==NULL");

    g.bWaitEstablishCommunicationsResponse = true;
    g.bWaitEstablishCommunicationsResponseError = false;
    int ret3 = g.DoConnect();   // case 200: success -> bConnect=true, Task=1, return 1
    CHECK(ret3 == 1 && g.bConnect == true && g.iStartConnectTask == 1,
          "T9: DoConnect case 200 (no error) completes -- bConnect=true, Task resets to 1, returns 1");

    // ConnectDelay timeout -> return 2 branch.
    g.bConnect = false;
    int ret4 = g.DoConnect();   // case 1 again: re-arms ConnectDelay.TimerSetSecAndOn(3), Task=100
    CHECK(ret4 == 0 && g.iStartConnectTask == 100, "T9: DoConnect re-armed (case 1) for the ConnectDelay-timeout sub-test");
    ::Sleep(3100);
    int ret5 = g.DoConnect();   // case 100: bWaitSelectRsp==false, ConnectDelay elapsed -> Task=1, return 2
    CHECK(ret5 == 2 && g.iStartConnectTask == 1, "T9: ConnectDelay's 3-second timeout -> DoConnect returns 2, resets Task to 1");
}

// ===========================================================================
//  [T10] Timer1Timer's IniConfig.bEnable_SECS_GEM==false branch (forced
//  disconnect) + the SECSGEM_DoSeparateWait 5-second re-arm window.
//
//  IMPORTANT (flagged): Timer1Timer's own `bSendDoSeparate`/
//  `bSetDoSeparateWait`/`bWaitHTimer`/`bTimerRunning` are golden FUNCTION-STATIC
//  locals (persist across EVERY THGem instance AND every call, for the
//  process's entire lifetime -- a faithful translation of golden's own
//  shape, not something this wave changed). `bSECSGEM_DoSeparate`/
//  `SECSGEM_DoSeparate`/`SECSGEM_DoSeparateWait` are likewise genuine
//  file-scope globals (golden :24/:4746/:5175). This test is written to be
//  the FIRST (and only) one in this binary that ever drives
//  IniConfig.bEnable_SECS_GEM==false through Timer1Timer -- every earlier
//  Bucket-C test that pumps Timer1Timer (T3) runs with bEnable_SECS_GEM==true,
//  so the disable branch below is never entered by them and these
//  statics/globals are still at their process-start defaults when this test
//  begins. Re-ordering the test sequence in main() below could break this
//  assumption -- keep this test LAST among the Timer1Timer-pumping tests.
// ===========================================================================
static void test_timer1timer_disable_branch()
{
    printf("\n[T10] Timer1Timer disable branch (IniConfig.bEnable_SECS_GEM==false)\n");

    bool savedEnableSecsGem = IniConfig.bEnable_SECS_GEM;
    bool savedInitialOK = InitialOK;
    THGem *savedHGem = HGem;

    THGem g;
    BucketCPanels panels(g);
    HGem = &g;
    g.DB = new THGemMemo();
    InitialOK = true;

    TCustomWinSocket *conn = g.srvGem->SimAcceptConnection("10.0.0.30", 4600);
    g.srvGem->Open();
    g.clientGem->Active = true;   // so "both sockets closed" is observable on both

    IniConfig.bEnable_SECS_GEM = false;
    g.Timer1Timer(NULL);   // srvGem->Socket->ActiveConnections!=0 -> DoSeparate() + Close() both -> bSECSGEM_DoSeparate=true

    CHECK(conn->SimTxBuffer().size() == 14, "T10: disable branch sends a real Separate_req control frame");
    if (conn->SimTxBuffer().size() == 14)
        CHECK((unsigned char)conn->SimTxBuffer()[9] == 9, "T10: sent frame's SType byte == 9 (Separate_req)");
    CHECK(g.srvGem->Active == false && g.clientGem->Active == false,
          "T10: disable branch closes BOTH srvGem and clientGem");
    CHECK(bSECSGEM_DoSeparate == true, "T10: bSECSGEM_DoSeparate latches true after the forced disconnect");

    // Re-enable: Timer1Timer's own `if(bSetDoSeparateWait && bSECSGEM_DoSeparate)`
    // now arms SECSGEM_DoSeparateWait.SetSecAndOn(5) and takes the early-return
    // "bWaitHTimer" path -- sleepless variant: confirm the pump returns
    // immediately without advancing Timer1Task (the real 5-second wait, D6,
    // has not elapsed).
    IniConfig.bEnable_SECS_GEM = true;
    int taskBefore = g.Timer1Task;
    g.Timer1Timer(NULL);
    CHECK(g.Timer1Task == taskBefore,
          "T10: SECSGEM_DoSeparateWait's 5-second window blocks the pump -- Timer1Task does not advance on this call");

    HGem = savedHGem;
    InitialOK = savedInitialOK;
    IniConfig.bEnable_SECS_GEM = savedEnableSecsGem;
    delete g.DB; g.DB = NULL;
}

// ===========================================================================
//  AI(W906-uHGemEquipment-BucketC) 20260717: T3/T4/T5 (via
//  ProcessSocketReceiveData, directly or through Timer1Timer) all reach the
//  now-real SaveSECSGEMTextToLog(), which -- exactly like the
//  already-committed SaveSECSGEMErrToLog test [12] guards against -- APPENDS
//  to a real, hardcoded, outside-the-repo path:
//  D:\SECS_GEM_LOGS\<yyyy>\<mm_dd>\SECSGEM_TextLog_<hh>.txt. Per this wave's
//  own hard test-hygiene rule (any test reaching that logger must leave the
//  archive byte-for-byte unchanged afterward), this capture/restore pair
//  wraps the WHOLE Bucket-C block in main() below in ONE shot (restoring
//  after every individual SaveSECSGEMTextToLog call would be impractically
//  invasive) -- same technique as test [12]'s own inline capture/restore,
//  factored into two reusable functions here since it now brackets several
//  tests instead of just one.
// ===========================================================================
struct TextLogSnapshot
{
    AnsiString filePath;
    bool fileExistedBefore;
    bool dayDirExistedBefore;
    bool yearDirExistedBefore;
    std::string originalContent;
};

static TextLogSnapshot CaptureTextLogSnapshot()
{
    TextLogSnapshot snap;
    TDateTime tdNow = Now();
    AnsiString dirYear = "D:\\SECS_GEM_LOGS\\" + FormatDateTime("yyyy", tdNow);
    AnsiString dirDay = dirYear + "\\" + FormatDateTime("mm_dd", tdNow);
    snap.filePath = dirDay + "\\SECSGEM_TextLog_" + FormatDateTime("hh", tdNow) + ".txt";
    snap.fileExistedBefore = FileExists(snap.filePath);
    snap.dayDirExistedBefore = DirectoryExists(dirDay);
    snap.yearDirExistedBefore = DirectoryExists(dirYear);
    if (snap.fileExistedBefore)
    {
        FILE *rf = fopen(snap.filePath.c_str(), "rb");
        if (rf)
        {
            char buf[65536];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), rf)) > 0)
                snap.originalContent.append(buf, n);
            fclose(rf);
        }
    }
    return snap;
}

static void RestoreTextLogSnapshot(const TextLogSnapshot &snap)
{
    // Recompute the directory paths from the SAME captured filePath (avoids
    // re-deriving yyyy/mm_dd separately and risking an hour-boundary mismatch
    // between capture and restore -- same accepted latent limitation as
    // test [12]'s own inline version, which this mirrors).
    if (snap.fileExistedBefore)
    {
        FILE *wf = fopen(snap.filePath.c_str(), "wb");
        if (wf)
        {
            fwrite(snap.originalContent.data(), 1, snap.originalContent.size(), wf);
            fclose(wf);
        }
    }
    else
    {
        remove(snap.filePath.c_str());
        AnsiString dirDay = ExtractFilePath(snap.filePath);
        // ExtractFilePath keeps the trailing backslash; strip it before
        // ExtractFilePath'ing the parent again for the year directory.
        AnsiString dirDayNoSlash = dirDay;
        if (dirDayNoSlash.Length() > 0 && dirDayNoSlash[dirDayNoSlash.Length()] == '\\')
            dirDayNoSlash.SetLength(dirDayNoSlash.Length() - 1);
        AnsiString dirYear = ExtractFilePath(dirDayNoSlash);
        if (!snap.dayDirExistedBefore)
            RemoveDir(dirDayNoSlash);
        if (!snap.yearDirExistedBefore)
            RemoveDir(dirYear);
    }
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
    test_connect_lifecycle();
    test_clientgem_handlers();
    test_doopencommuncation_open_guard();
    test_srvgemclient_handlers();
    test_check_socket_active_false();
    test_clear_default_even_report();
    test_get_time_info();
    test_online_family_and_misc();
    test_stringout_and_binaryout();
    test_initialhgem_savesystemdefault_roundtrip();
    test_manualcreatergroleclick();
    test_processshow();
    test_stringout_2arg();
    test_do_update_status();

    // Bucket C (W906-uHGemEquipment-BucketC 20260717)
    test_memorystream_shim();
    test_htimer_real_semantics();
    // T3/T4/T5 reach the real SaveSECSGEMTextToLog() -- see the
    // CaptureTextLogSnapshot/RestoreTextLogSnapshot comment above.
    TextLogSnapshot textLogSnap = CaptureTextLogSnapshot();
    test_select_handshake_e2e();
    test_frame_reassembly();
    test_data_message_gated_dispatch();
    RestoreTextLogSnapshot(textLogSnap);
    test_t3_send_side_record();
    test_t3_timeout_s9f9_cross_class_seam();
    test_checksfcoderesponse();
    test_doconnect_state_walk();
    test_timer1timer_disable_branch();

    printf("\n=== RESULT: %d passed, %d failed ===\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}
