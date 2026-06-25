using System;
using System.Collections.Generic;

namespace HT9045.Analysis
{
    /// <summary>
    /// Computes aggregated statistics from a list of <see cref="EventLogRow"/>
    /// objects.  The analysis mirrors the PoC spec (six points) and the
    /// aggregation logic used by cObserver.cpp:2584-2585 for the real handler.
    ///
    /// This class is stateless; call <see cref="Summarize"/> with any row list.
    /// </summary>
    public class EventLogAnalyzer
    {
        /// <summary>
        /// Top-N AlarmCode frequency cutoff used when TopN is not specified.
        /// </summary>
        public const int DefaultTopN = 10;

        /// <summary>
        /// Build a full <see cref="EventLogSummary"/> from the supplied rows.
        /// </summary>
        /// <param name="rows">Rows as returned by <see cref="EventLogReader.Read"/>.</param>
        /// <param name="topN">
        /// How many distinct AlarmCode values to surface in
        /// <see cref="EventLogSummary.TopAlarmCodes"/>.  Defaults to
        /// <see cref="DefaultTopN"/>.
        /// </param>
        public EventLogSummary Summarize(IList<EventLogRow> rows, int topN = DefaultTopN)
        {
            if (rows == null) throw new ArgumentNullException("rows");

            var summary = new EventLogSummary
            {
                TotalRows        = rows.Count,
                DistinctDates    = new List<string>(),
                RowsByUnitName   = new Dictionary<string, int>(StringComparer.Ordinal),
                TopAlarmCodes    = new List<KeyValuePair<string, int>>(),
            };

            // Working collections.
            var alarmCodeCounts = new Dictionary<string, int>(StringComparer.Ordinal);
            var dateSet         = new SortedSet<string>(StringComparer.Ordinal);
            DateTime? minDate   = null;
            DateTime? maxDate   = null;

            double totalStoped  = 0.0;
            double longestStoped = 0.0;
            int    rowsWithStoped = 0;

            int dupZero    = 0;
            int dupNonZero = 0;
            int jamWarCount = 0;
            int mesCount    = 0;

            foreach (EventLogRow row in rows)
            {
                // ---- (3) UnitName counts ----
                if (!string.IsNullOrEmpty(row.UnitName))
                {
                    int existing;
                    summary.RowsByUnitName.TryGetValue(row.UnitName, out existing);
                    summary.RowsByUnitName[row.UnitName] = existing + 1;
                }

                // ---- (2) Date range ----
                if (!string.IsNullOrEmpty(row.Date))
                {
                    dateSet.Add(row.Date);
                    DateTime? occ = row.OccurDateTime;
                    if (occ.HasValue)
                    {
                        DateTime d = occ.Value;
                        if (!minDate.HasValue || d < minDate.Value) minDate = d;
                        if (!maxDate.HasValue || d > maxDate.Value) maxDate = d;
                    }
                }

                // ---- (4) AlarmCode frequency ----
                if (!string.IsNullOrEmpty(row.AlarmCode))
                {
                    int cnt;
                    alarmCodeCounts.TryGetValue(row.AlarmCode, out cnt);
                    alarmCodeCounts[row.AlarmCode] = cnt + 1;
                }

                // ---- (5) StopedTime ----
                double? st = row.StopedTimeSeconds;
                if (st.HasValue)
                {
                    totalStoped  += st.Value;
                    rowsWithStoped++;
                    if (st.Value > longestStoped) longestStoped = st.Value;
                }

                // ---- (6) Alarm-specific breakdowns ----
                if (row.IsAlarm || row.IsMessage)
                {
                    if (row.IsAlarm)  jamWarCount++;
                    if (row.IsMessage) mesCount++;

                    // Duplicate breakdown applies to rows that have the Duplicate field set.
                    int? dup = row.DuplicateValue;
                    if (dup.HasValue)
                    {
                        if (dup.Value == 0) dupZero++;
                        else                 dupNonZero++;
                    }
                }
            }

            // Materialise results.
            summary.MinDate               = minDate;
            summary.MaxDate               = maxDate;
            summary.DistinctDates.AddRange(dateSet);

            summary.TotalStopedTimeSeconds  = totalStoped;
            summary.LongestStopedTimeSeconds = longestStoped;
            summary.RowsWithStopedTime       = rowsWithStoped;

            summary.AlarmRowsDuplicateZero    = dupZero;
            summary.AlarmRowsDuplicateNonZero = dupNonZero;
            summary.JamWarRowCount            = jamWarCount;
            summary.MesRowCount               = mesCount;

            // ---- (3) Sort RowsByUnitName descending ----
            summary.RowsByUnitName = SortDictionaryDescending(summary.RowsByUnitName);

            // ---- (4) Top-N AlarmCode ----
            summary.TopAlarmCodes = TopNDescending(alarmCodeCounts, topN);

            return summary;
        }

        // ------------------------------------------------------------------ //
        //  Helpers — avoid LINQ to stay C# 7.3 / net48 without extra imports
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Return a new dictionary sorted by value descending.
        /// Uses insertion sort — adequate for the small cardinality of UnitName.
        /// </summary>
        private static Dictionary<string, int> SortDictionaryDescending(Dictionary<string, int> source)
        {
            // Collect pairs, sort by value desc then key asc (stable presentation).
            var pairs = new List<KeyValuePair<string, int>>(source);
            pairs.Sort((a, b) =>
            {
                int cmp = b.Value.CompareTo(a.Value);
                if (cmp != 0) return cmp;
                return string.Compare(a.Key, b.Key, StringComparison.Ordinal);
            });

            var result = new Dictionary<string, int>(pairs.Count, StringComparer.Ordinal);
            foreach (var kv in pairs)
                result[kv.Key] = kv.Value;
            return result;
        }

        /// <summary>
        /// Return the top-N entries from a frequency dictionary, sorted by
        /// value descending.  Returns fewer than N when the dictionary is smaller.
        /// </summary>
        private static List<KeyValuePair<string, int>> TopNDescending(
            Dictionary<string, int> source, int n)
        {
            var pairs = new List<KeyValuePair<string, int>>(source);
            pairs.Sort((a, b) =>
            {
                int cmp = b.Value.CompareTo(a.Value);
                if (cmp != 0) return cmp;
                return string.Compare(a.Key, b.Key, StringComparison.Ordinal);
            });

            int take = Math.Min(n, pairs.Count);
            var result = new List<KeyValuePair<string, int>>(take);
            for (int i = 0; i < take; i++)
                result.Add(pairs[i]);
            return result;
        }
    }
}
