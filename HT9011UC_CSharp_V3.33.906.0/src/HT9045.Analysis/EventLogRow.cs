using System;

namespace HT9045.Analysis
{
    /// <summary>
    /// One row from the HT9045 EventLog backup CSV export.
    ///
    /// Column order and names mirror the DB-grid header produced by
    /// SGDToCSV (common.cpp:2050) / cObserver.cpp:2559 SELECT * FROM EventLogView.
    ///
    /// Header (verbatim, comma+tab delimited):
    ///   No,\tUnitName,\tAlarmCode,\tDate,\tTime,\tRecovery,\tStopedTime,\tDuplicate,\tMessage
    ///
    /// Field semantics: see EventLog format spec in project task notes.
    /// BCB6 writer source: cMyDB.cpp, note.cpp, cObserver.cpp.
    /// </summary>
    public class EventLogRow
    {
        // ------------------------------------------------------------------ //
        //  CSV columns — strings exactly as parsed from file
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Grid row counter (1-based). NOT in the DB; assigned at export time
        /// by the StringGrid row index (cObserver.cpp export loop).
        /// </summary>
        public string No { get; set; }

        /// <summary>
        /// Logical source unit. e.g. "Process", "Motion", "Message", "Motor",
        /// "InArm", "OutArm". Populated by UnitName table lookup in
        /// MyDBIEvent (cMyDB.cpp:653-668) for alarm rows.
        /// </summary>
        public string UnitName { get; set; }

        /// <summary>
        /// Alarm/event code: "JAMxxxx", "WARxxxx", "MESxxxx", or empty.
        /// Empty cells are stored as the '\t' placeholder in BCB6 code
        /// (SL->Add("\t") in cMyDB.cpp); "(null)" also appears in real data.
        /// Both are normalised to empty string by EventLogReader.
        /// </summary>
        public string AlarmCode { get; set; }

        /// <summary>
        /// Occurrence date string "YYYY-MM-DD" from DATE(OccurDateTime)
        /// in the export query (cObserver.cpp:2559).
        /// </summary>
        public string Date { get; set; }

        /// <summary>
        /// Occurrence time string "HH:MM:SS" from TIME(OccurDateTime).
        /// Millis are dropped in the grid export (present only in live EventLogTxt).
        /// </summary>
        public string Time { get; set; }

        /// <summary>
        /// Operator recovery action: "SKIP", "RETRY", "HOME", or empty.
        /// Set by note.cpp:2816-2826 (only when KeyCode != 0).
        /// Empty for non-alarm rows.
        /// </summary>
        public string Recovery { get; set; }

        /// <summary>
        /// [sic] Machine stop duration in seconds while the alarm halted
        /// production before operator recovery (PassTime, note.cpp:1141).
        /// Numeric string when present; empty for non-alarm rows.
        /// Note: customer-independent emit path fills this; JCET-gated path
        /// may leave it blank (see project memory ht9045-motor-alarm-emit-paths).
        /// </summary>
        public string StopedTime { get; set; }

        /// <summary>
        /// Duplicate/repeat-error flag (iDuplicateError, note.cpp:802-813):
        /// "0"=first/new occurrence, "1"=duplicate ('Again!!'),
        /// "2"/"3"/"4"=other repeat classes. Empty for non-alarm rows.
        /// Alarm-stats queries filter Duplicate=0 (cObserver.cpp:2545,2708).
        /// </summary>
        public string Duplicate { get; set; }

        /// <summary>
        /// Human-readable description. For alarms = AlarmList.Message
        /// (may contain Big5 Traditional Chinese decoded to UTF-16 by
        /// EventLogReader via Big5Codec). For Process/Message rows = logged text.
        /// Last column — trailing fields after position 8 are joined back.
        /// </summary>
        public string Message { get; set; }

        // ------------------------------------------------------------------ //
        //  Derived / parsed helpers
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Parses Date + Time into a DateTime value.
        /// Returns null when Date or Time is empty/unparseable.
        /// Format: "yyyy-MM-dd HH:mm:ss" per the export query split.
        /// </summary>
        public DateTime? OccurDateTime
        {
            get
            {
                if (string.IsNullOrWhiteSpace(Date) || string.IsNullOrWhiteSpace(Time))
                    return null;

                DateTime dt;
                if (DateTime.TryParseExact(
                        Date.Trim() + " " + Time.Trim(),
                        "yyyy-MM-dd HH:mm:ss",
                        System.Globalization.CultureInfo.InvariantCulture,
                        System.Globalization.DateTimeStyles.None,
                        out dt))
                    return dt;

                return null;
            }
        }

        /// <summary>
        /// Returns true when AlarmCode is non-empty and starts with "JAM" or "WAR"
        /// (Type=1 or Type=2 in AlarmList, cMyDB.cpp:616).
        /// </summary>
        public bool IsAlarm
        {
            get
            {
                if (string.IsNullOrEmpty(AlarmCode)) return false;
                return AlarmCode.StartsWith("JAM", StringComparison.OrdinalIgnoreCase)
                    || AlarmCode.StartsWith("WAR", StringComparison.OrdinalIgnoreCase);
            }
        }

        /// <summary>
        /// Returns true when AlarmCode starts with "MES" (message-class alarm).
        /// </summary>
        public bool IsMessage
        {
            get
            {
                if (string.IsNullOrEmpty(AlarmCode)) return false;
                return AlarmCode.StartsWith("MES", StringComparison.OrdinalIgnoreCase);
            }
        }

        /// <summary>
        /// Parses StopedTime as double seconds. Returns null when empty or
        /// non-numeric (handles both the all-empty sample case and the
        /// JCET-gated blank case described in project memory).
        /// </summary>
        public double? StopedTimeSeconds
        {
            get
            {
                if (string.IsNullOrWhiteSpace(StopedTime)) return null;
                double v;
                if (double.TryParse(StopedTime.Trim(),
                        System.Globalization.NumberStyles.Any,
                        System.Globalization.CultureInfo.InvariantCulture, out v))
                    return v;
                return null;
            }
        }

        /// <summary>
        /// Parses Duplicate as int. Returns null when empty / non-numeric.
        /// </summary>
        public int? DuplicateValue
        {
            get
            {
                if (string.IsNullOrWhiteSpace(Duplicate)) return null;
                int v;
                if (int.TryParse(Duplicate.Trim(), out v)) return v;
                return null;
            }
        }
    }
}
