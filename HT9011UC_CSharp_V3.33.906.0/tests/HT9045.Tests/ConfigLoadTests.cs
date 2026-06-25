using System;
using System.Collections.Generic;
using System.IO;
using Xunit;
using HT9045.Infrastructure.Config;

namespace HT9045.Tests
{
    // ----------------------------------------------------------------------
    //  Phase-2 (W1) config-loader unit tests.
    //
    //  Exercises HandlerHardwareConfig / MotTable / IoTable against trimmed,
    //  ASCII-only fixtures under TestData\ (copied next to the test assembly).
    //  ASCII-only means the Big5 decode path is a no-op, so these tests verify
    //  PARSER semantics (key lookup, column order, enum mapping) independent of
    //  encoding — encoding itself is covered by Big5RoundTripTests.
    //
    //  Ground truth being reproduced (from the real system\ files):
    //    MOTION_CARD_TYPE=1 (MotionCard_Contec), IO_CARD_TYPE=2 (NewIO_MN200),
    //    TTL_CARD_TYPE=2, INDEX_MOTION_CARD=0, Mot rows CardModel=SMC,
    //    IO_Table ISABase column (eMotionNet=0).
    // ----------------------------------------------------------------------
    public class ConfigLoadTests
    {
        private static string FixtureDir
        {
            get { return Path.Combine(AppContext.BaseDirectory, "TestData"); }
        }

        private static string GerneralIni
        {
            get { return Path.Combine(FixtureDir, "Gerneral.ini"); }
        }

        private static string MotTableCsv
        {
            get { return Path.Combine(FixtureDir, "Mot_Table.csv"); }
        }

        private static string IoTableCsv
        {
            get { return Path.Combine(FixtureDir, "IO_Table.csv"); }
        }

        // ------------------------------------------------------------------
        //  Sanity: fixtures were copied to the output directory.
        // ------------------------------------------------------------------
        [Fact]
        public void Fixtures_AreCopiedNextToTestAssembly()
        {
            Assert.True(File.Exists(GerneralIni), "missing fixture: " + GerneralIni);
            Assert.True(File.Exists(MotTableCsv), "missing fixture: " + MotTableCsv);
            Assert.True(File.Exists(IoTableCsv), "missing fixture: " + IoTableCsv);
        }

        // ==================================================================
        //  HandlerHardwareConfig (Gerneral.ini)
        // ==================================================================

        [Fact]
        public void HandlerHardwareConfig_MotionCardType_MapsToContec()
        {
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(MotionCardType.MotionCard_Contec, cfg.MOTION_CARD_TYPE);
        }

        [Fact]
        public void HandlerHardwareConfig_IoCardType_MapsToNewIoMn200_AndRequiresIoTable()
        {
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(IoCardType.NewIO_MN200, cfg.IO_CARD_TYPE);
            // IO_CARD_TYPE==NewIO_MN200 must trigger the LoadIoData branch
            // (database.cpp:1088-1092 mirror).
            Assert.True(cfg.RequiresIoTableLoad);
        }

        [Fact]
        public void HandlerHardwareConfig_TtlCardType_IsTwo()
        {
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(2, cfg.TTL_CARD_TYPE);
        }

        [Fact]
        public void HandlerHardwareConfig_IndexMotionCard_Zero_MeansIndexUsesGalil()
        {
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(0, cfg.INDEX_MOTION_CARD);
            Assert.True(cfg.IndexUsesGalil);
        }

        [Fact]
        public void HandlerHardwareConfig_MotionnetSpeed_MapsTo10M()
        {
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(MotionnetSpeed.COMMSPEED_10M, cfg.MOTIONNET_SPEED);
        }

        [Fact]
        public void HandlerHardwareConfig_TtlCardUseAddress_IsOne()
        {
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(1, cfg.TTL_CARD_USE_ADDRESS);
        }

        [Fact]
        public void HandlerHardwareConfig_VibrationCardQty_IsTwo()
        {
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(2, cfg.VibrationCardQty);
        }

        [Fact]
        public void HandlerHardwareConfig_HeaterType_DefaultsFromSystemSection()
        {
            // USE_16_HEATER=0 -> eht4Heater (read from [System]).
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(HeaterType.eht4Heater, cfg.USE_16_HEATER);
        }

        [Fact]
        public void HandlerHardwareConfig_HeaterCtrlType_ReadFromTempCtrlSection_MapsToWt404()
        {
            // HEATER_CTRL_TYPE lives in [TempCtrl], NOT [System]. Value 4 = TC_WT404.
            // This proves the loader honours the section boundary.
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);
            Assert.Equal(TempCtrlType.TC_WT404, cfg.HEATER_CTRL_TYPE);
        }

        [Fact]
        public void HandlerHardwareConfig_MissingKey_FallsBackToDefault()
        {
            // Loading an absent file must not throw; all fields carry C++ defaults.
            string bogus = Path.Combine(FixtureDir, "does_not_exist.ini");
            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(bogus);
            Assert.Equal(MotionCardType.MotionCard_SYN, cfg.MOTION_CARD_TYPE); // default 0
            Assert.Equal(IoCardType.MotionnetIO_L112, cfg.IO_CARD_TYPE);       // default 0
            Assert.False(cfg.RequiresIoTableLoad);
        }

        [Fact]
        public void HandlerHardwareConfig_VibrationCardQty_ClampedToTwo()
        {
            // CheckRange clamps VibrationCardQty to [0,2]. Build an inline INI to
            // prove the clamp without depending on a separate fixture file.
            string tmp = Path.Combine(Path.GetTempPath(),
                "ht9045_vib_clamp_" + Guid.NewGuid().ToString("N") + ".ini");
            try
            {
                File.WriteAllText(tmp, "[System]\r\nVibrationCardQty=9\r\n");
                HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(tmp);
                Assert.Equal(2, cfg.VibrationCardQty);
            }
            finally
            {
                if (File.Exists(tmp)) File.Delete(tmp);
            }
        }

        // ==================================================================
        //  MotTable (Mot_Table.csv)
        // ==================================================================

        [Fact]
        public void MotTable_Load_SkipsHeader_ReturnsAllDataRows()
        {
            List<MotTableRow> rows = MotTable.Load(MotTableCsv);
            // Fixture has header + 3 data rows.
            Assert.Equal(3, rows.Count);
        }

        [Fact]
        public void MotTable_FirstRow_MapsColumnsByCorrectPosition()
        {
            List<MotTableRow> rows = MotTable.Load(MotTableCsv);
            MotTableRow first = rows[0];
            // First data row = M00,MInArmX (BoardID=0,Port=0) — mirrors real file.
            Assert.Equal("M00", first.Motorname);
            Assert.Equal("MInArmX", first.Alias);
            Assert.Equal("0", first.BoardID);
            Assert.Equal("0", first.Port);
        }

        [Fact]
        public void MotTable_AllRows_HaveCardModelSmc()
        {
            // CardModel is at column index 22; the physical column order differs
            // from the emot* enum order, so a positional read of the WRONG index
            // would not land on "SMC". This guards the mapping.
            List<MotTableRow> rows = MotTable.Load(MotTableCsv);
            Assert.NotEmpty(rows);
            foreach (MotTableRow r in rows)
            {
                Assert.Equal("SMC", r.CardModel);
            }
        }

        [Fact]
        public void MotTable_TrailingEmptyColumns_ToleratedAsEmptyString()
        {
            // PickLimit (col 26) is empty in the fixture rows; In1Logic (col 28)
            // is the last column. Parser must tolerate empty cells.
            List<MotTableRow> rows = MotTable.Load(MotTableCsv);
            MotTableRow first = rows[0];
            Assert.Equal(string.Empty, first.PickLimit);
            Assert.Equal("1", first.In1Logic); // M00 row ends ...,,1,1
        }

        [Fact]
        public void MotTable_LastRow_IsExtraM143()
        {
            List<MotTableRow> rows = MotTable.Load(MotTableCsv);
            MotTableRow last = rows[rows.Count - 1];
            Assert.Equal("M143", last.Motorname);
            Assert.Equal("MFix3Full", last.Alias);
            Assert.Equal("SMC", last.CardModel);
        }

        [Fact]
        public void MotTable_MissingFile_ReturnsEmptyList()
        {
            List<MotTableRow> rows = MotTable.Load(Path.Combine(FixtureDir, "no_such_mot.csv"));
            Assert.Empty(rows);
        }

        // ==================================================================
        //  IoTable (IO_Table.csv)
        // ==================================================================

        [Fact]
        public void IoTable_Load_SkipsHeader_ReturnsAllDataRows()
        {
            List<IoTableRow> rows = IoTable.Load(IoTableCsv);
            // Fixture has header + 3 data rows.
            Assert.Equal(3, rows.Count);
        }

        [Fact]
        public void IoTable_FirstRow_MapsColumnsAndIsaBaseToMotionNet()
        {
            List<IoTableRow> rows = IoTable.Load(IoTableCsv);
            IoTableRow first = rows[0];
            // First data row = Sucker,BTestSuckAA (Lane=0,ModuleType=1,IP=7,Port=0,
            // Bit=4,InType=1,ISABase=0,Enable=1) — mirrors real file.
            Assert.Equal("Sucker", first.IOType);
            Assert.Equal("BTestSuckAA", first.Alias);
            Assert.Equal("0", first.Lane);
            Assert.Equal("1", first.ModuleType);
            Assert.Equal("7", first.IP);
            Assert.Equal("0", first.Port);
            Assert.Equal("4", first.Bit);
            Assert.Equal("1", first.InType);
            Assert.Equal((int)IoBaseType.eMotionNet, first.ISABase); // 0
            Assert.Equal("1", first.Enable);
        }

        [Fact]
        public void IoTable_IsaBaseColumn_MapsBothMotionNetAndIsaBase()
        {
            // Dominant value is eMotionNet(0); the trailing SwClear0 row carries
            // ISABase=1 (eISABase) — confirm both map correctly.
            List<IoTableRow> rows = IoTable.Load(IoTableCsv);
            IoTableRow swClear = rows[rows.Count - 1];
            Assert.Equal("SwClear0", swClear.Alias);
            Assert.Equal((int)IoBaseType.eISABase, swClear.ISABase); // 1
        }

        [Fact]
        public void IoTable_MostRows_AreMotionNet()
        {
            List<IoTableRow> rows = IoTable.Load(IoTableCsv);
            int motionNet = 0;
            foreach (IoTableRow r in rows)
            {
                if (r.ISABase == (int)IoBaseType.eMotionNet) motionNet++;
            }
            // 2 of the 3 fixture rows are eMotionNet (Sucker + Sensor).
            Assert.Equal(2, motionNet);
        }

        [Fact]
        public void IoTable_MissingFile_ReturnsEmptyList()
        {
            List<IoTableRow> rows = IoTable.Load(Path.Combine(FixtureDir, "no_such_io.csv"));
            Assert.Empty(rows);
        }
    }
}
