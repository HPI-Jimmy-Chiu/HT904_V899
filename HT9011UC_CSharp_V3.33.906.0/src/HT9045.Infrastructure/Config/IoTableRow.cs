using System;

namespace HT9045.Infrastructure.Config
{
    /// <summary>
    /// One data row from IO_Table.csv.
    /// Fields are declared in the EXACT column order defined in the spec
    /// (mirrors database.cpp LoadIoData column consumption order).
    ///
    /// IO_Table.csv header (verbatim):
    ///   IOType,Alias,Lane,ModuleType,IP,Port,Bit,InType,ISABase,Enable,
    ///   OnAlarmTime,OffAlarmTime,OnDelayTime,OffDelayTime,Note
    ///
    /// ISABase column maps to eIOType enum (MachineType.h:712-717):
    ///   eMotionNet=0, eISABase=1, ePCI1735U=2, ePCI1203=3, ePLCbase=4
    /// </summary>
    public class IoTableRow
    {
        // col 0
        public string IOType        { get; private set; }
        // col 1
        public string Alias         { get; private set; }
        // col 2
        public string Lane          { get; private set; }
        // col 3
        public string ModuleType    { get; private set; }
        // col 4
        public string IP            { get; private set; }
        // col 5
        public string Port          { get; private set; }
        // col 6
        public string Bit           { get; private set; }
        // col 7
        public string InType        { get; private set; }
        // col 8 — raw int; eIOType enum: eMotionNet=0, eISABase=1, ePCI1735U=2,
        //         ePCI1203=3, ePLCbase=4.  Exposed as int for direct enum comparison.
        public int    ISABase        { get; private set; }
        // col 9
        public string Enable        { get; private set; }
        // col 10
        public string OnAlarmTime   { get; private set; }
        // col 11
        public string OffAlarmTime  { get; private set; }
        // col 12
        public string OnDelayTime   { get; private set; }
        // col 13
        public string OffDelayTime  { get; private set; }
        // col 14
        public string Note          { get; private set; }

        /// <summary>
        /// Construct a row by resolving each column via the header NAME (robust to
        /// column reordering — mirrors database.cpp SetIOTableNo). This is the path
        /// IoTable.Load uses. ISABase empty/absent defaults to 0 (eMotionNet),
        /// matching database.cpp.
        /// </summary>
        public static IoTableRow FromNamedRow(CsvHeaderMap map, string[] f)
        {
            if (map == null) throw new ArgumentNullException("map");
            if (f == null) throw new ArgumentNullException("f");

            var r = new IoTableRow();
            r.IOType       = map.Value(f, "IOType");
            r.Alias        = map.Value(f, "Alias");
            r.Lane         = map.Value(f, "Lane");
            r.ModuleType   = map.Value(f, "ModuleType");
            r.IP           = map.Value(f, "IP");
            r.Port         = map.Value(f, "Port");
            r.Bit          = map.Value(f, "Bit");
            r.InType       = map.Value(f, "InType");
            r.ISABase      = ParseInt(map.Value(f, "ISABase")); // empty -> 0 = eMotionNet
            r.Enable       = map.Value(f, "Enable");
            r.OnAlarmTime  = map.Value(f, "OnAlarmTime");
            r.OffAlarmTime = map.Value(f, "OffAlarmTime");
            r.OnDelayTime  = map.Value(f, "OnDelayTime");
            r.OffDelayTime = map.Value(f, "OffDelayTime");
            r.Note         = map.Value(f, "Note");
            return r;
        }

        /// <summary>
        /// Positional fallback (canonical physical order). The name-based
        /// <see cref="FromNamedRow"/> is preferred (handles reordering).
        /// Missing trailing fields default to empty string / 0 — tolerant of short rows.
        /// </summary>
        public static IoTableRow FromFields(string[] f)
        {
            if (f == null) throw new ArgumentNullException("f");

            var r = new IoTableRow();
            r.IOType       = Get(f, 0);
            r.Alias        = Get(f, 1);
            r.Lane         = Get(f, 2);
            r.ModuleType   = Get(f, 3);
            r.IP           = Get(f, 4);
            r.Port         = Get(f, 5);
            r.Bit          = Get(f, 6);
            r.InType       = Get(f, 7);
            r.ISABase      = ParseInt(Get(f, 8));
            r.Enable       = Get(f, 9);
            r.OnAlarmTime  = Get(f, 10);
            r.OffAlarmTime = Get(f, 11);
            r.OnDelayTime  = Get(f, 12);
            r.OffDelayTime = Get(f, 13);
            r.Note         = Get(f, 14);
            return r;
        }

        private static string Get(string[] f, int idx)
        {
            if (idx >= f.Length) return string.Empty;
            return (f[idx] ?? string.Empty).Trim();
        }

        private static int ParseInt(string s)
        {
            int v;
            return int.TryParse(s, out v) ? v : 0;
        }

        private IoTableRow() { }
    }
}
