using System;

namespace HT9045.Infrastructure.Config
{
    // ======================================================================
    //  Enum mirrors — defined alongside the config class so consumers need
    //  only one using directive.  Values match the C++ #define / enum values
    //  exactly; do not renumber.
    // ======================================================================

    /// <summary>
    /// Motion controller card driver family.
    /// Source: cmydef.h lines 90-92.
    /// Read site: database.cpp:1082 (MOTION_CARD_TYPE).
    /// </summary>
    public enum MotionCardType
    {
        MotionCard_SYN     = 0,   // SYN-TEK
        MotionCard_Contec  = 1,
        // 2 is unused in the C++ codebase
        PCI_P64C64         = 3,   // HT7080B
    }

    /// <summary>
    /// Digital I/O card driver.
    /// Source: cmydef.h lines 85-88.
    /// Read site: database.cpp:1084.  When NewIO_MN200 or PCI_MC88X1,
    /// LoadIoData() is invoked to parse IO_Table.csv (database.cpp:1088-1092).
    /// </summary>
    public enum IoCardType
    {
        MotionnetIO_L112   = 0,
        MotionnetIO_MN200  = 1,
        NewIO_MN200        = 2,
        PCI_MC88X1         = 3,   // also covers PCI_P64C64 variant
    }

    /// <summary>
    /// IO base type — corresponds to eIOType in MachineType.h lines 712-717.
    /// Used for the ISABase column in IO_Table.csv.
    /// </summary>
    public enum IoBaseType
    {
        eMotionNet  = 0,
        eISABase    = 1,
        ePCI1735U   = 2,
        ePCI1203    = 3,
        ePLCbase    = 4,
    }

    /// <summary>
    /// Index heater configuration.
    /// Source: eHeaterType in MachineType.h lines 651-657.
    /// Read site: database.cpp:621 (USE_16_HEATER).
    /// Values 2 (eht16HeaterEJ1N) and 4 (eht32HeaterKT4H) trigger a special
    /// EJ1N read-fix branch at database.cpp:622-624.
    /// </summary>
    public enum HeaterType
    {
        eht4Heater       = 0,
        eht16Heater      = 1,   // KT4H 16-channel
        eht16HeaterEJ1N  = 2,
        eht32HeaterEJ1N  = 3,
        eht32HeaterKT4H  = 4,
        eht16HeaterDTME08 = 5,
        eht32HeaterDTME08 = 6,
    }

    /// <summary>
    /// Temperature controller hardware type.
    /// Source: TC_* defines in MyTemptureSet.h lines 4-8.
    /// NOTE: the runtime default constant KT4H resolves to 1 (cmydef.cpp:223).
    /// The actual enum integers are offset by one from the C++ raw integers
    /// (TC_UT100=1 … TC_TMC401=5) — C# enum preserves those raw values.
    /// Read site: database.cpp:432 (HEATER_CTRL_TYPE, section [TempCtrl]).
    /// </summary>
    public enum TempCtrlType
    {
        TC_UT100   = 1,
        TC_KT4H    = 2,
        TC_DT4848  = 3,
        TC_WT404   = 4,
        TC_TMC401  = 5,
    }

    /// <summary>
    /// Motionnet bus communication speed index.
    /// Source: mn200.h lines 11-14.
    /// Read site: database.cpp:1083 (MOTIONNET_SPEED).
    /// </summary>
    public enum MotionnetSpeed
    {
        COMMSPEED_2_5M = 0,
        COMMSPEED_5M   = 1,
        COMMSPEED_10M  = 2,
        COMMSPEED_20M  = 3,
    }

    // ======================================================================
    //  Typed config object
    // ======================================================================

    /// <summary>
    /// Typed hardware-selection configuration loaded from Gerneral.ini.
    ///
    /// Field order matches the database.cpp read sequence so that auditing
    /// against the C++ source is straightforward.  All field names preserve
    /// the verbatim INI key names used in the C++ codebase.
    ///
    /// Usage:
    ///   var cfg = HandlerHardwareConfig.Load(@"D:\HT9045\system\Gerneral.ini");
    ///   if (cfg.MOTION_CARD_TYPE == MotionCardType.MotionCard_Contec) { ... }
    /// </summary>
    public sealed class HandlerHardwareConfig
    {
        // ------------------------------------------------------------------
        //  [System] section — motion / IO card selection
        //  (read order follows database.cpp starting at line 1077)
        // ------------------------------------------------------------------

        /// <summary>
        /// Whether an air/heat-gun mechanism is installed.
        /// 0 = not installed; non-zero = installed.
        /// database.cpp:1077
        /// </summary>
        public int INSTALL_HEAT_GUN { get; private set; }

        /// <summary>
        /// Whether ATC3.5 heat-gun is installed.
        /// 0 = not installed; non-zero = installed.
        /// database.cpp:1078
        /// </summary>
        public int INSTALL_ATC_HEAT_GUN { get; private set; }

        /// <summary>
        /// Selects HotPlate module type/variant.
        /// 0 = default; no named enum at read site.
        /// database.cpp:1079
        /// </summary>
        public int USE_HOTPLATE_TYPE { get; private set; }

        /// <summary>
        /// Whether magnetic linear scale (encoder) feedback is used.
        /// 0 = off; non-zero = on.
        /// database.cpp:1080
        /// </summary>
        public int USE_MAGNETIC_SCALE { get; private set; }

        /// <summary>
        /// Selects the motion controller card driver family.
        /// database.cpp:1082
        /// </summary>
        public MotionCardType MOTION_CARD_TYPE { get; private set; }

        /// <summary>
        /// Motionnet bus communication speed index.
        /// Default: COMMSPEED_20M (3).
        /// database.cpp:1083
        /// </summary>
        public MotionnetSpeed MOTIONNET_SPEED { get; private set; }

        /// <summary>
        /// Selects the digital I/O card driver.
        /// When NewIO_MN200 or PCI_MC88X1, IO_Table.csv must be loaded.
        /// database.cpp:1084
        /// </summary>
        public IoCardType IO_CARD_TYPE { get; private set; }

        /// <summary>
        /// Whether Index Arm axes are present.  Section: [IndexDriver].
        /// 0 = no; non-zero = yes.
        /// database.cpp:1086
        /// </summary>
        public int USE_INDEX_ARM_AXES { get; private set; }

        /// <summary>
        /// Selects TTL signal card mode.
        /// ==2 sets TTLRS232VerCheck = 7071601 (RS232 standalone card).
        /// database.cpp:1094
        /// </summary>
        public int TTL_CARD_TYPE { get; private set; }

        /// <summary>
        /// Whether the TTL card carries/uses a board address.
        /// 0 = no address; 1 = use address.
        /// database.cpp:1099
        /// </summary>
        public int TTL_CARD_USE_ADDRESS { get; private set; }

        /// <summary>
        /// Number of vibration (knocker) motor communication channels.
        /// Clamped to [0, 2] in C++ via CheckRange.
        /// Default: 2.
        /// database.cpp:1313
        /// </summary>
        public int VibrationCardQty { get; private set; }

        /// <summary>
        /// Selects the motion card driving the Index axes
        /// (MTestY1/MTestZ1/MTestZ2/MTestY2).
        /// ==0 means Index uses Galil (gated throughout cinitial.cpp/database.cpp).
        /// database.cpp:1345
        /// </summary>
        public int INDEX_MOTION_CARD { get; private set; }

        // ------------------------------------------------------------------
        //  [TempCtrl] section
        //  NOTE: this key lives in [TempCtrl], NOT [System].
        // ------------------------------------------------------------------

        /// <summary>
        /// Temperature controller hardware type.
        /// Section: [TempCtrl] — easy to confuse with [System].
        /// Default: KT4H (runtime constant = 1, cmydef.cpp:223).
        /// database.cpp:432
        /// </summary>
        public TempCtrlType HEATER_CTRL_TYPE { get; private set; }

        // ------------------------------------------------------------------
        //  [System] section — heater configuration
        // ------------------------------------------------------------------

        /// <summary>
        /// Index heater configuration.
        /// Values 2 and 4 trigger EJ1N read-fix branches (database.cpp:622-624).
        /// Default: eht4Heater (0).
        /// database.cpp:621
        /// </summary>
        public HeaterType USE_16_HEATER { get; private set; }

        // ------------------------------------------------------------------
        //  Convenience derivations (no additional INI reads; computed from
        //  the above after loading, mirroring common C++ gating conditions).
        // ------------------------------------------------------------------

        /// <summary>
        /// True when IO_CARD_TYPE requires IO_Table.csv to be loaded
        /// (NewIO_MN200 or PCI_MC88X1), mirroring the branch at
        /// database.cpp:1088-1092.
        /// </summary>
        public bool RequiresIoTableLoad
        {
            get
            {
                return IO_CARD_TYPE == IoCardType.NewIO_MN200 ||
                       IO_CARD_TYPE == IoCardType.PCI_MC88X1;
            }
        }

        /// <summary>
        /// True when INDEX_MOTION_CARD == 0, meaning the Index axes are
        /// driven by Galil — this gates numerous branches in cinitial.cpp
        /// and database.cpp.
        /// </summary>
        public bool IndexUsesGalil
        {
            get { return INDEX_MOTION_CARD == 0; }
        }

        // ------------------------------------------------------------------
        //  Private constructor — callers must use Load().
        // ------------------------------------------------------------------
        private HandlerHardwareConfig() { }

        // ------------------------------------------------------------------
        //  Factory
        // ------------------------------------------------------------------

        /// <summary>
        /// Reads Gerneral.ini at <paramref name="iniPath"/> (Big5 encoded) and
        /// returns a populated <see cref="HandlerHardwareConfig"/>.
        ///
        /// Missing keys fall back to the same defaults used in the C++ source.
        /// The method never throws for a missing file; all fields will carry
        /// their defaults in that case.
        /// </summary>
        public static HandlerHardwareConfig Load(string iniPath)
        {
            if (iniPath == null) throw new ArgumentNullException("iniPath");

            var ini = new IniFile(iniPath);
            var cfg = new HandlerHardwareConfig();

            // --- [System] — card selection (database.cpp:1077-1099) -------

            cfg.INSTALL_HEAT_GUN     = ini.GetInt("System", "INSTALL_HEAT_GUN",     0);
            cfg.INSTALL_ATC_HEAT_GUN = ini.GetInt("System", "INSTALL_ATC_HEAT_GUN", 0);
            cfg.USE_HOTPLATE_TYPE    = ini.GetInt("System", "USE_HOTPLATE_TYPE",     0);
            cfg.USE_MAGNETIC_SCALE   = ini.GetInt("System", "USE_MAGNETIC_SCALE",    0);

            cfg.MOTION_CARD_TYPE = (MotionCardType)ini.GetInt("System", "MOTION_CARD_TYPE", (int)MotionCardType.MotionCard_SYN);
            cfg.MOTIONNET_SPEED  = (MotionnetSpeed)ini.GetInt("System", "MOTIONNET_SPEED",  (int)MotionnetSpeed.COMMSPEED_20M);
            cfg.IO_CARD_TYPE     = (IoCardType)    ini.GetInt("System", "IO_CARD_TYPE",      (int)IoCardType.MotionnetIO_L112);

            // [IndexDriver] section
            cfg.USE_INDEX_ARM_AXES = ini.GetInt("IndexDriver", "USE_INDEX_ARM_AXES", 0);

            cfg.TTL_CARD_TYPE         = ini.GetInt("System", "TTL_CARD_TYPE",         0);
            cfg.TTL_CARD_USE_ADDRESS  = ini.GetInt("System", "TTL_CARD_USE_ADDRESS",  0);

            // --- [System] — vibration / index motion (database.cpp:1313,1345) ---
            cfg.VibrationCardQty  = Clamp(ini.GetInt("System", "VibrationCardQty", 2), 0, 2);
            cfg.INDEX_MOTION_CARD = ini.GetInt("System", "INDEX_MOTION_CARD", 0);

            // --- [TempCtrl] section (database.cpp:432) ----------------------
            // Default constant KT4H resolves to 1 at runtime (cmydef.cpp:223).
            cfg.HEATER_CTRL_TYPE = (TempCtrlType)ini.GetInt("TempCtrl", "HEATER_CTRL_TYPE", (int)TempCtrlType.TC_KT4H);

            // --- [System] — heater type (database.cpp:621) -----------------
            cfg.USE_16_HEATER = (HeaterType)ini.GetInt("System", "USE_16_HEATER", (int)HeaterType.eht4Heater);

            return cfg;
        }

        // ------------------------------------------------------------------
        //  Private utility
        // ------------------------------------------------------------------

        // int.Clamp / Math.Clamp are not available in net48 + C# 7.3, so
        // implement inline.
        private static int Clamp(int value, int min, int max)
        {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }
    }
}
