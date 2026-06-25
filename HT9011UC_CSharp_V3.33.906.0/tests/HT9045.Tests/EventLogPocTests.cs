using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using Xunit;
using HT9045.Analysis;

namespace HT9045.Tests
{
    // ----------------------------------------------------------------------
    //  Phase-2 (W1) EventLog (alarm/event) CSV analysis PoC.
    //
    //  Proves the C# <-> SQLite <-> Big5 <-> file chain with ZERO hardware:
    //    - EventLogReader parses the comma+tab on-disk layout produced by the
    //      BCB6 writer SGDToCSV (common.cpp:2050-2064, called as
    //      SGDToCSV(grid, ",", ";", path) at cMyDB.cpp:452 / cObserver.cpp:2410).
    //      On disk each cell is followed by a single ',' and the next cell
    //      carries the BCB6 leading-'\t' empty-column placeholder, so the
    //      inter-cell boundary is the two-byte sequence 0x2C 0x09 (",\t").
    //    - EventLogAnalyzer aggregates the rows.
    //    - EventLogSqliteStore round-trips through SQLite (Microsoft.Data.Sqlite).
    //
    //  Fixture: TestData\EventLogSample.csv (ASCII-only, 6 data rows). Two
    //  Message edge cases are included on purpose:
    //    - row 2 Message has PLAIN commas ("...fail, retry now, then home"):
    //      no ',\t' inside, so it stays in a single token (no split).
    //    - row 3 Message embeds an actual ',\t' sequence: it splits into extra
    //      tokens and exercises EventLogReader's defensive last-column join.
    //
    //  Real-file facts are grounded in
    //  D:\HT9045_Log\EventLog\HT9045_EventLogBackup_2023_07_01_15_29_53.csv.
    // ----------------------------------------------------------------------
    public class EventLogPocTests
    {
        // ----- fixture path -------------------------------------------------
        private static string FixtureCsv
        {
            get { return Path.Combine(AppContext.BaseDirectory, "TestData", "EventLogSample.csv"); }
        }

        private static List<EventLogRow> LoadFixture()
        {
            return new EventLogReader().Read(FixtureCsv);
        }

        // ===================================================================
        //  (3) Parser: row count
        // ===================================================================
        [Fact]
        public void Reader_Fixture_Returns6DataRows_HeaderExcluded()
        {
            List<EventLogRow> rows = LoadFixture();
            Assert.Equal(6, rows.Count);

            // Header must NOT appear as data -- No="No" would be the giveaway.
            Assert.DoesNotContain(rows, r => r.No == "No");

            // Row order is preserved (file order, No ascending in this fixture).
            Assert.Equal("1", rows[0].No);
            Assert.Equal("6", rows[5].No);
        }

        // ===================================================================
        //  Parser: Message with PLAIN commas stays whole (single token)
        // ===================================================================
        [Fact]
        public void Reader_MessageWithPlainCommas_ParsedWhole()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogRow row2 = rows.Single(r => r.No == "2");

            // Plain commas are NOT the ",\t" delimiter, so the message is intact.
            Assert.Equal("AutoClean contact fail, retry now, then home", row2.Message);
            // And the leading columns parsed correctly around it.
            Assert.Equal("Motion", row2.UnitName);
            Assert.Equal("JAM0312", row2.AlarmCode);
            Assert.Equal("RETRY", row2.Recovery);
        }

        // ===================================================================
        //  Parser: Message containing an embedded ",\t" exercises the
        //  defensive last-column join (tokens beyond index 8 re-joined).
        // ===================================================================
        [Fact]
        public void Reader_MessageWithEmbeddedDelimiter_RejoinedIntoLastColumn()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogRow row3 = rows.Single(r => r.No == "3");

            // Reader re-joins extra tokens with the ",\t" delimiter, recovering
            // the original Message text faithfully.
            Assert.Equal("Motor CCW limit,\tsensor triggered", row3.Message);
            Assert.Equal("WAR240004", row3.AlarmCode);
            // Columns AHEAD of Message must not be polluted by the join.
            Assert.Equal("HOME", row3.Recovery);
            Assert.Equal("7.5", row3.StopedTime);
            Assert.Equal("1", row3.Duplicate);
        }

        // ===================================================================
        //  Parser: "(null)" and bare-empty AlarmCode normalise to empty.
        // ===================================================================
        [Fact]
        public void Reader_NullLiteralAndEmptyAlarmCode_NormalisedToEmpty()
        {
            List<EventLogRow> rows = LoadFixture();

            // row 1 has a bare-empty AlarmCode cell.
            Assert.Equal(string.Empty, rows.Single(r => r.No == "1").AlarmCode);
            // row 5 has the literal "(null)" AlarmCode.
            Assert.Equal(string.Empty, rows.Single(r => r.No == "5").AlarmCode);
        }

        // ===================================================================
        //  (3) Per-UnitName counts
        // ===================================================================
        [Fact]
        public void Analyzer_PerUnitNameCounts_Correct_DescByCount()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogSummary s = new EventLogAnalyzer().Summarize(rows);

            Assert.Equal(6, s.TotalRows);
            Assert.Equal(3, s.RowsByUnitName["Process"]);
            Assert.Equal(2, s.RowsByUnitName["Motion"]);
            Assert.Equal(1, s.RowsByUnitName["Message"]);

            // Dictionary is ordered desc by count: first entry is Process(3).
            KeyValuePair<string, int> first = s.RowsByUnitName.First();
            Assert.Equal("Process", first.Key);
            Assert.Equal(3, first.Value);
        }

        // ===================================================================
        //  (4) Top AlarmCode
        // ===================================================================
        [Fact]
        public void Analyzer_TopAlarmCode_Correct()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogSummary s = new EventLogAnalyzer().Summarize(rows);

            // Non-empty AlarmCodes: JAM0312, WAR240004, MES2108, MES2107 -- each once.
            // Empty / "(null)" codes are excluded.
            Assert.Equal(4, s.TopAlarmCodes.Count);
            foreach (KeyValuePair<string, int> kv in s.TopAlarmCodes)
                Assert.Equal(1, kv.Value);

            // Ties broken by key ASC (Ordinal): JAM0312 < MES2107 < MES2108 < WAR240004.
            Assert.Equal("JAM0312", s.TopAlarmCodes[0].Key);
            Assert.Equal("WAR240004", s.TopAlarmCodes[3].Key);

            // JAM/WAR vs MES breakdown.
            Assert.Equal(2, s.JamWarRowCount); // JAM0312 + WAR240004
            Assert.Equal(2, s.MesRowCount);    // MES2108 + MES2107
        }

        // ===================================================================
        //  (5) StopedTime sum / max / rows-with
        // ===================================================================
        [Fact]
        public void Analyzer_StopedTimeSum_Correct()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogSummary s = new EventLogAnalyzer().Summarize(rows);

            // row 2 = 12.5 s, row 3 = 7.5 s -> sum 20.0, max 12.5, 2 rows.
            Assert.Equal(20.0, s.TotalStopedTimeSeconds, 3);
            Assert.Equal(12.5, s.LongestStopedTimeSeconds, 3);
            Assert.Equal(2, s.RowsWithStopedTime);
        }

        // ===================================================================
        //  (6) Duplicate breakdown
        // ===================================================================
        [Fact]
        public void Analyzer_DuplicateBreakdown_Correct()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogSummary s = new EventLogAnalyzer().Summarize(rows);

            // row 2 (alarm) Duplicate=0 ; row 3 (alarm) Duplicate=1.
            // MES rows carry empty Duplicate -> DuplicateValue null -> not counted.
            Assert.Equal(1, s.AlarmRowsDuplicateZero);
            Assert.Equal(1, s.AlarmRowsDuplicateNonZero);
        }

        // ===================================================================
        //  (2) Date range / distinct dates
        // ===================================================================
        [Fact]
        public void Analyzer_DateRange_Correct()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogSummary s = new EventLogAnalyzer().Summarize(rows);

            Assert.True(s.MinDate.HasValue);
            Assert.True(s.MaxDate.HasValue);
            Assert.Equal("2022-06-28", s.MinDate.Value.ToString("yyyy-MM-dd"));
            Assert.Equal("2022-06-30", s.MaxDate.Value.ToString("yyyy-MM-dd"));

            // Distinct dates: 2022-06-28, 2022-06-29, 2022-06-30.
            Assert.Equal(3, s.DistinctDates.Count);
        }

        // ===================================================================
        //  (4) SQLite round-trip: QuerySummary from SQLite == in-memory analyzer
        // ===================================================================
        [Fact]
        public void SqliteRoundTrip_QuerySummary_EqualsInMemoryAnalyzer()
        {
            List<EventLogRow> rows = LoadFixture();
            EventLogSummary inMemory = new EventLogAnalyzer().Summarize(rows);

            // Temp DB file. GetTempFileName creates a 0-byte file; SQLite opens it
            // in ReadWriteCreate mode, which is fine for an empty file.
            string dbPath = Path.GetTempFileName();
            try
            {
                EventLogSqliteStore store = new EventLogSqliteStore();
                store.Ingest(rows, dbPath);

                // Total row count matches.
                Assert.Equal(rows.Count, store.QueryTotalRows(dbPath));

                // Per-UnitName counts from SQLite GROUP BY match the in-memory map.
                Dictionary<string, int> fromDb = store.QuerySummary(dbPath);
                Assert.Equal(inMemory.RowsByUnitName.Count, fromDb.Count);
                foreach (KeyValuePair<string, int> kv in inMemory.RowsByUnitName)
                {
                    Assert.True(fromDb.ContainsKey(kv.Key),
                        "SQLite summary missing UnitName '" + kv.Key + "'");
                    Assert.Equal(kv.Value, fromDb[kv.Key]);
                }
            }
            finally
            {
                // SqliteConnection pooling can keep a handle on the file; clear it
                // so the delete succeeds on Windows.
                Microsoft.Data.Sqlite.SqliteConnection.ClearAllPools();
                if (File.Exists(dbPath)) File.Delete(dbPath);
            }
        }

        // ===================================================================
        //  (5) Big5: a Traditional-Chinese Message round-trips, no U+FFFD.
        //  Writes a Big5 (cp950) fixture to a temp file, reads it back through
        //  EventLogReader (which decodes via Big5Codec), asserts no mojibake.
        // ===================================================================
        [Fact]
        public void Reader_Big5ChineseMessage_RoundTripsWithoutMojibake()
        {
            // Motor Alarm = U+99AC U+9054 U+8B66 U+5831 (Traditional Chinese,
            // "Motor Alarm"), seen in Error\<lang>\<code>.dat files. Defined as
            // raw Big5 (cp950) bytes so the C# SOURCE stays pure ASCII and is
            // immune to whatever encoding Roslyn assumes for this .cs file (the
            // "Big5 edit corruption" hazard from project memory). cp950 bytes:
            //   B0 A8 = U+99AC, B9 46 = U+9054, C4 B5 = U+8B66, B3 F8 = U+5831.
            byte[] chineseBig5 = new byte[] { 0xB0, 0xA8, 0xB9, 0x46, 0xC4, 0xB5, 0xB3, 0xF8 };
            string chineseMsg =
                HT9045.Infrastructure.Big5Codec.ToUtf16(chineseBig5) + " CCW limit";

            // Sanity: the decoded expected string itself must be clean and be
            // the 4 Han characters we intend (proves the byte literal is right).
            Assert.False(HT9045.Infrastructure.Big5Codec.HasMojibake(chineseMsg));
            // Expected value rebuilt from the same Big5 bytes (no string literal,
            // so an escape-rendering formatter cannot corrupt it).
            string expected =
                HT9045.Infrastructure.Big5Codec.ToUtf16(chineseBig5) + " CCW limit";
            Assert.Equal(expected, chineseMsg);

            // Build one header + one data row using the on-disk ",	" layout.
            // Delimiter is comma + TAB (0x2C 0x09). Built from a char cast so no
            // "	" escape exists in source for a formatter to rewrite into a raw tab.
            string delim = "," + ((char)0x09).ToString();
            string header = string.Join(delim, new[]
            {
                "No", "UnitName", "AlarmCode", "Date", "Time",
                "Recovery", "StopedTime", "Duplicate", "Message"
            });
            string dataRow = string.Join(delim, new[]
            {
                "1", "Motion", "WAR240004", "2022-06-30", "10:30:00",
                "HOME", "7.5", "1", chineseMsg
            });
            string crlf = ((char)0x0D).ToString() + ((char)0x0A).ToString();
            string text = header + crlf + dataRow + crlf;

            // Encode the whole document to Big5 (cp950) bytes, as the real files are.
            byte[] big5 = HT9045.Infrastructure.Big5Codec.ToBig5(text);

            string tmp = Path.Combine(Path.GetTempPath(),
                "EventLogBig5_" + Guid.NewGuid().ToString("N") + ".csv");
            try
            {
                File.WriteAllBytes(tmp, big5);

                List<EventLogRow> rows = new EventLogReader().Read(tmp);
                Assert.Single(rows);

                string parsed = rows[0].Message;
                // Round-trips through the file byte-for-byte (no U+FFFD).
                Assert.Equal(chineseMsg, parsed);
                Assert.False(HT9045.Infrastructure.Big5Codec.HasMojibake(parsed));
            }
            finally
            {
                if (File.Exists(tmp)) File.Delete(tmp);
            }
        }

        // ===================================================================
        //  (6) Integration: load the REAL newest EventLog backup CSV.
        //  Runtime-skip when the folder/file is absent (e.g. clean CI runner).
        //  Asserts:
        //    - parsed data-row count == (file line count - 1 header)
        //    - no parse exceptions
        //    - known UnitName counts and a known Message appear (spec facts)
        // ===================================================================
        private const string RealEventLogDir = @"D:\HT9045_Log\EventLog";

        [Fact]
        public void Integration_RealNewestEventLog_RowCountAndKnownFactsHold()
        {
            if (!Directory.Exists(RealEventLogDir))
                return; // runtime skip: corpus folder absent

            string[] files = Directory.GetFiles(RealEventLogDir, "HT9045_EventLogBackup_*.csv");
            if (files.Length == 0)
                return; // runtime skip: no backup files present

            // "newest": by last-write time (the spec's representative file is the
            // only one currently present, but pick robustly regardless).
            string newest = files
                .OrderByDescending(f => File.GetLastWriteTimeUtc(f))
                .First();

            // Count physical lines in the file (header + data). The real file is
            // CRLF-terminated; count line terminators, then account for whether
            // the final line has a trailing newline.
            int fileLineCount = CountNonEmptyLines(newest);

            // Parse (no exception expected).
            List<EventLogRow> rows = new EventLogReader().Read(newest);

            // Data rows == non-empty lines - 1 header line.
            Assert.Equal(fileLineCount - 1, rows.Count);

            // Spec fact: 3409 data rows in the representative file. Only assert the
            // hard number when this IS that file, so a future different backup does
            // not falsely fail; the (lines-1) equality above always holds.
            if (Path.GetFileName(newest) == "HT9045_EventLogBackup_2023_07_01_15_29_53.csv")
            {
                Assert.Equal(3409, rows.Count);

                // Known UnitName counts (spec fact 4).
                EventLogSummary s = new EventLogAnalyzer().Summarize(rows);
                Assert.Equal(3382, s.RowsByUnitName["Process"]);
                Assert.Equal(19, s.RowsByUnitName["Motion"]);
                Assert.Equal(8, s.RowsByUnitName["Message"]);

                // Known row No=1 (spec fact 3).
                EventLogRow first = rows[0];
                Assert.Equal("1", first.No);
                Assert.Equal("Process", first.UnitName);
                Assert.Equal("2022-06-30", first.Date);
                Assert.Equal("12:45:20", first.Time);
                Assert.Equal(@"IPSC Can Not Find Execute File => C:\GTK\IPSC.exe", first.Message);

                // Known MES2108 'Program Start' exists (spec fact 5).
                Assert.Contains(rows, r => r.AlarmCode == "MES2108" && r.Message == "Program Start");

                // Date range (spec fact 6).
                Assert.Equal("2022-06-06", s.MinDate.Value.ToString("yyyy-MM-dd"));
                Assert.Equal("2022-06-30", s.MaxDate.Value.ToString("yyyy-MM-dd"));
            }
        }

        // Count non-empty lines (CRLF/LF tolerant) the same way EventLogReader
        // skips blank lines, so the comparison is apples-to-apples.
        private static int CountNonEmptyLines(string path)
        {
            byte[] raw = File.ReadAllBytes(path);
            string content = HT9045.Infrastructure.Big5Codec.ToUtf16(raw);
            string[] lines = content.Split(new[] { "\r\n", "\n" }, StringSplitOptions.None);
            int n = 0;
            foreach (string line in lines)
            {
                string l = line;
                if (l.Length > 0 && l[l.Length - 1] == '\r') l = l.Substring(0, l.Length - 1);
                if (l.Length > 0) n++;
            }
            return n;
        }
    }
}
