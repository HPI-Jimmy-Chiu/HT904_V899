using System;
using System.Collections.Generic;
using System.Text;

namespace HT9045.Analysis
{
    /// <summary>
    /// Aggregated analysis result produced by <see cref="EventLogAnalyzer.Summarize"/>.
    ///
    /// Mirrors the PoC analysis spec:
    ///   (2) TotalRows, DateRange, DistinctDates
    ///   (3) Per-UnitName record counts
    ///   (4) Top-N AlarmCode frequencies
    ///   (5) StopedTime aggregates (sum / max)
    ///   (6) Duplicate breakdown + alarm-only subset counts
    /// </summary>
    public class EventLogSummary
    {
        // ------------------------------------------------------------------ //
        //  (2) File-level stats
        // ------------------------------------------------------------------ //

        /// <summary>Total data rows parsed (excluding header).</summary>
        public int TotalRows { get; set; }

        /// <summary>
        /// Earliest occurrence date found in the Date column.
        /// Null when no row has a parseable date.
        /// </summary>
        public DateTime? MinDate { get; set; }

        /// <summary>
        /// Latest occurrence date found in the Date column.
        /// Null when no row has a parseable date.
        /// </summary>
        public DateTime? MaxDate { get; set; }

        /// <summary>
        /// All distinct date values (YYYY-MM-DD strings), sorted ascending.
        /// Count gives the number of production days in this export.
        /// </summary>
        public List<string> DistinctDates { get; set; }

        // ------------------------------------------------------------------ //
        //  (3) Per-UnitName record counts
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Row count per UnitName, descending by count.
        /// Key = UnitName string (e.g. "Process", "Motion", "Message").
        /// Value = row count.
        /// </summary>
        public Dictionary<string, int> RowsByUnitName { get; set; }

        // ------------------------------------------------------------------ //
        //  (4) Top-N AlarmCode frequency
        // ------------------------------------------------------------------ //

        /// <summary>
        /// AlarmCode frequency list, descending.  Only non-empty AlarmCode
        /// values (empty and "(null)" already normalised away by EventLogReader).
        /// Each entry: (AlarmCode, Count).
        /// </summary>
        public List<KeyValuePair<string, int>> TopAlarmCodes { get; set; }

        // ------------------------------------------------------------------ //
        //  (5) StopedTime aggregates
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Sum of all numeric StopedTime values (total downtime seconds).
        /// 0 when no alarm rows carry StopedTime (e.g. the 2023_07 sample file).
        /// Corresponds to Sum(EventLog.StopedTime) in cObserver.cpp:2584.
        /// </summary>
        public double TotalStopedTimeSeconds { get; set; }

        /// <summary>
        /// Largest single StopedTime value (longest single alarm downtime).
        /// 0 when no numeric StopedTime present.
        /// </summary>
        public double LongestStopedTimeSeconds { get; set; }

        /// <summary>
        /// Number of alarm rows that have a non-empty numeric StopedTime.
        /// Useful to distinguish "no downtime" from "no downtime data".
        /// </summary>
        public int RowsWithStopedTime { get; set; }

        // ------------------------------------------------------------------ //
        //  (6) Duplicate breakdown + alarm-only
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Count of alarm rows with Duplicate="0" (first/new occurrences).
        /// This is the denominator used by cObserver.cpp:2545,2708 for alarm stats.
        /// </summary>
        public int AlarmRowsDuplicateZero { get; set; }

        /// <summary>
        /// Count of alarm rows with Duplicate != "0" (repeat/duplicate events).
        /// </summary>
        public int AlarmRowsDuplicateNonZero { get; set; }

        /// <summary>
        /// Count of rows whose AlarmCode starts with "JAM" or "WAR"
        /// (Type 1/2 in AlarmList, cMyDB.cpp:616).
        /// </summary>
        public int JamWarRowCount { get; set; }

        /// <summary>
        /// Count of rows whose AlarmCode starts with "MES" (message-class).
        /// </summary>
        public int MesRowCount { get; set; }

        // ------------------------------------------------------------------ //
        //  Human-readable summary
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Formats a concise multi-line summary suitable for console output
        /// or unit-test assertion messages.
        /// </summary>
        public override string ToString()
        {
            var sb = new StringBuilder();
            sb.AppendLine("=== EventLog Summary ===");
            sb.AppendLine("TotalRows        : " + TotalRows);

            if (MinDate.HasValue && MaxDate.HasValue)
            {
                sb.AppendLine("DateRange        : " + MinDate.Value.ToString("yyyy-MM-dd")
                              + " .. " + MaxDate.Value.ToString("yyyy-MM-dd"));
            }
            else
            {
                sb.AppendLine("DateRange        : (no date data)");
            }

            sb.AppendLine("DistinctDates    : " + (DistinctDates != null ? DistinctDates.Count : 0));

            sb.AppendLine("--- UnitName counts (desc) ---");
            if (RowsByUnitName != null)
            {
                foreach (var kv in RowsByUnitName)
                    sb.AppendLine("  " + kv.Key.PadRight(24) + " : " + kv.Value);
            }

            sb.AppendLine("--- Top AlarmCodes ---");
            if (TopAlarmCodes != null)
            {
                int rank = 1;
                foreach (var kv in TopAlarmCodes)
                {
                    sb.AppendLine("  " + rank.ToString().PadLeft(2) + ". "
                                  + kv.Key.PadRight(12) + " : " + kv.Value);
                    rank++;
                }
            }

            if (RowsWithStopedTime > 0)
            {
                sb.AppendLine("TotalStopedTime  : " + TotalStopedTimeSeconds + " s");
                sb.AppendLine("LongestStopedTime: " + LongestStopedTimeSeconds + " s");
                sb.AppendLine("RowsWithStopedT  : " + RowsWithStopedTime);
            }
            else
            {
                sb.AppendLine("StopedTime       : no downtime data in this export");
            }

            sb.AppendLine("JAM/WAR rows     : " + JamWarRowCount);
            sb.AppendLine("MES rows         : " + MesRowCount);
            sb.AppendLine("Duplicate=0      : " + AlarmRowsDuplicateZero);
            sb.AppendLine("Duplicate!=0     : " + AlarmRowsDuplicateNonZero);

            return sb.ToString();
        }
    }
}
