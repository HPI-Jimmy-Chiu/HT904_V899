using System;

namespace HT9045.Infrastructure.Config
{
    /// <summary>
    /// One data row from Mot_Table.csv.
    /// Fields are declared in the EXACT column order defined in the spec
    /// (mirrors cinitial.cpp / database.cpp column consumption order).
    ///
    /// Mot_Table.csv header (verbatim):
    ///   Motorname,Alias,BoardID,Port,Direction,IP,GearRatio,HomeDirectior,
    ///   HomeHighSpeed,HomeLowSpeed,InitSpeed,JogHighSpeed,JogLowSpeed,Rate,
    ///   SoftLimitN,SoftLimitP,Enable,ServoAlarmOn,Range,1P2P,SensorType,
    ///   SimulateSpeed,CardModel,Acc,Dec,EncodeType,PickLimit,LimitLogic,In1Logic
    /// </summary>
    public class MotTableRow
    {
        // col 0
        public string Motorname     { get; private set; }
        // col 1
        public string Alias         { get; private set; }
        // col 2
        public string BoardID       { get; private set; }
        // col 3
        public string Port          { get; private set; }
        // col 4
        public string Direction     { get; private set; }
        // col 5
        public string IP            { get; private set; }
        // col 6
        public string GearRatio     { get; private set; }
        // col 7 — note: C++ source has "HomeDirectior" (typo preserved)
        public string HomeDirectior { get; private set; }
        // col 8
        public string HomeHighSpeed { get; private set; }
        // col 9
        public string HomeLowSpeed  { get; private set; }
        // col 10
        public string InitSpeed     { get; private set; }
        // col 11
        public string JogHighSpeed  { get; private set; }
        // col 12
        public string JogLowSpeed   { get; private set; }
        // col 13
        public string Rate          { get; private set; }
        // col 14
        public string SoftLimitN    { get; private set; }
        // col 15
        public string SoftLimitP    { get; private set; }
        // col 16
        public string Enable        { get; private set; }
        // col 17
        public string ServoAlarmOn  { get; private set; }
        // col 18
        public string Range         { get; private set; }
        // col 19 — header token is "1P2P"
        public string OneP2P        { get; private set; }
        // col 20
        public string SensorType    { get; private set; }
        // col 21
        public string SimulateSpeed { get; private set; }
        // col 22 — kept as string; all rows in production file are "SMC"
        public string CardModel     { get; private set; }
        // col 23
        public string Acc           { get; private set; }
        // col 24
        public string Dec           { get; private set; }
        // col 25
        public string EncodeType    { get; private set; }
        // col 26
        public string PickLimit     { get; private set; }
        // col 27
        public string LimitLogic    { get; private set; }
        // col 28
        public string In1Logic      { get; private set; }

        /// <summary>
        /// Construct a row by resolving each column via the header NAME (robust to
        /// column reordering — mirrors database.cpp SetMOTTableNo). This is the path
        /// MotTable.Load uses. The header tokens are the verbatim Mot_Table.csv names.
        /// </summary>
        public static MotTableRow FromNamedRow(CsvHeaderMap map, string[] f)
        {
            if (map == null) throw new ArgumentNullException("map");
            if (f == null) throw new ArgumentNullException("f");

            var r = new MotTableRow();
            r.Motorname     = map.Value(f, "Motorname");
            r.Alias         = map.Value(f, "Alias");
            r.BoardID       = map.Value(f, "BoardID");
            r.Port          = map.Value(f, "Port");
            r.Direction     = map.Value(f, "Direction");
            r.IP            = map.Value(f, "IP");
            r.GearRatio     = map.Value(f, "GearRatio");
            r.HomeDirectior = map.Value(f, "HomeDirectior"); // C++ typo preserved
            r.HomeHighSpeed = map.Value(f, "HomeHighSpeed");
            r.HomeLowSpeed  = map.Value(f, "HomeLowSpeed");
            r.InitSpeed     = map.Value(f, "InitSpeed");
            r.JogHighSpeed  = map.Value(f, "JogHighSpeed");
            r.JogLowSpeed   = map.Value(f, "JogLowSpeed");
            r.Rate          = map.Value(f, "Rate");
            r.SoftLimitN    = map.Value(f, "SoftLimitN");
            r.SoftLimitP    = map.Value(f, "SoftLimitP");
            r.Enable        = map.Value(f, "Enable");
            r.ServoAlarmOn  = map.Value(f, "ServoAlarmOn");
            r.Range         = map.Value(f, "Range");
            r.OneP2P        = map.Value(f, "1P2P");
            r.SensorType    = map.Value(f, "SensorType");
            r.SimulateSpeed = map.Value(f, "SimulateSpeed");
            r.CardModel     = map.Value(f, "CardModel");
            r.Acc           = map.Value(f, "Acc");
            r.Dec           = map.Value(f, "Dec");
            r.EncodeType    = map.Value(f, "EncodeType");
            r.PickLimit     = map.Value(f, "PickLimit");
            r.LimitLogic    = map.Value(f, "LimitLogic");
            r.In1Logic      = map.Value(f, "In1Logic");
            return r;
        }

        /// <summary>
        /// Positional fallback: construct from an already-split array assuming the
        /// canonical physical column order. Kept for direct/legacy callers; the
        /// name-based <see cref="FromNamedRow"/> is preferred (handles reordering).
        /// Missing trailing fields default to empty string — tolerant of short rows.
        /// </summary>
        public static MotTableRow FromFields(string[] f)
        {
            if (f == null) throw new ArgumentNullException("f");

            var r = new MotTableRow();
            r.Motorname     = Get(f, 0);
            r.Alias         = Get(f, 1);
            r.BoardID       = Get(f, 2);
            r.Port          = Get(f, 3);
            r.Direction     = Get(f, 4);
            r.IP            = Get(f, 5);
            r.GearRatio     = Get(f, 6);
            r.HomeDirectior = Get(f, 7);
            r.HomeHighSpeed = Get(f, 8);
            r.HomeLowSpeed  = Get(f, 9);
            r.InitSpeed     = Get(f, 10);
            r.JogHighSpeed  = Get(f, 11);
            r.JogLowSpeed   = Get(f, 12);
            r.Rate          = Get(f, 13);
            r.SoftLimitN    = Get(f, 14);
            r.SoftLimitP    = Get(f, 15);
            r.Enable        = Get(f, 16);
            r.ServoAlarmOn  = Get(f, 17);
            r.Range         = Get(f, 18);
            r.OneP2P        = Get(f, 19);
            r.SensorType    = Get(f, 20);
            r.SimulateSpeed = Get(f, 21);
            r.CardModel     = Get(f, 22);
            r.Acc           = Get(f, 23);
            r.Dec           = Get(f, 24);
            r.EncodeType    = Get(f, 25);
            r.PickLimit     = Get(f, 26);
            r.LimitLogic    = Get(f, 27);
            r.In1Logic      = Get(f, 28);
            return r;
        }

        private static string Get(string[] f, int idx)
        {
            if (idx >= f.Length) return string.Empty;
            return (f[idx] ?? string.Empty).Trim();
        }

        private MotTableRow() { }
    }
}
