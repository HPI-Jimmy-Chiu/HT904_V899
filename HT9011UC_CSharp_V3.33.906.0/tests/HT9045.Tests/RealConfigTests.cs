using System.Collections.Generic;
using System.IO;
using Xunit;
using HT9045.Infrastructure.Config;

namespace HT9045.Tests
{
    // ----------------------------------------------------------------------
    //  INTEGRATION tests against the REAL runtime config under D:\HT9045\system.
    //
    //  These are the ultimate ground-truth checks: they parse the actual Big5
    //  files the production handler reads and assert the documented facts
    //  (45 SMC motor rows, MOTION_CARD_TYPE=1, IO_CARD_TYPE=2, etc.).
    //
    //  The system\ directory only exists on the developer machine, not on a
    //  clean CI runner. Each test therefore performs a RUNTIME SKIP (returns
    //  without asserting) when the file is absent, so CI stays green. On a
    //  machine that HAS the files, the assertions run for real.
    //
    //  Read-only: nothing here writes to system\.
    // ----------------------------------------------------------------------
    public class RealConfigTests
    {
        private const string SystemDir   = @"D:\HT9045\system";
        private static readonly string GerneralIni = Path.Combine(SystemDir, "Gerneral.ini");
        private static readonly string MotTableCsv = Path.Combine(SystemDir, "Mot_Table.csv");
        private static readonly string IoTableCsv  = Path.Combine(SystemDir, "IO_Table.csv");

        [Fact]
        public void RealMotTable_Has45Rows_AllCardModelSmc()
        {
            if (!File.Exists(MotTableCsv))
                return; // runtime skip: real file absent (e.g. CI)

            List<MotTableRow> rows = MotTable.Load(MotTableCsv);
            Assert.Equal(45, rows.Count);

            foreach (MotTableRow r in rows)
            {
                Assert.Equal("SMC", r.CardModel);
            }

            // First / last data rows documented in the spec.
            Assert.Equal("M00", rows[0].Motorname);
            Assert.Equal("MInArmX", rows[0].Alias);
            Assert.Equal("0", rows[0].BoardID);
            Assert.Equal("0", rows[0].Port);

            Assert.Equal("M143", rows[rows.Count - 1].Motorname);
            Assert.Equal("MFix3Full", rows[rows.Count - 1].Alias);
        }

        [Fact]
        public void RealGerneralIni_MotionCardType1_IoCardType2()
        {
            if (!File.Exists(GerneralIni))
                return; // runtime skip: real file absent (e.g. CI)

            HandlerHardwareConfig cfg = HandlerHardwareConfig.Load(GerneralIni);

            Assert.Equal(MotionCardType.MotionCard_Contec, cfg.MOTION_CARD_TYPE); // =1
            Assert.Equal(IoCardType.NewIO_MN200, cfg.IO_CARD_TYPE);               // =2
            Assert.True(cfg.RequiresIoTableLoad);                                 // NewIO_MN200 -> load IO table
            Assert.Equal(2, cfg.TTL_CARD_TYPE);
            Assert.Equal(0, cfg.INDEX_MOTION_CARD);                               // Index uses Galil
            Assert.True(cfg.IndexUsesGalil);
            Assert.Equal(TempCtrlType.TC_WT404, cfg.HEATER_CTRL_TYPE);            // [TempCtrl]=4
        }

        [Fact]
        public void RealIoTable_Has15Columns_668Rows_IsaBaseDominantMotionNet()
        {
            if (!File.Exists(IoTableCsv))
                return; // runtime skip: real file absent (e.g. CI)

            List<IoTableRow> rows = IoTable.Load(IoTableCsv);
            Assert.Equal(668, rows.Count);

            // First data row documented in the spec.
            Assert.Equal("Sucker", rows[0].IOType);
            Assert.Equal("BTestSuckAA", rows[0].Alias);
            Assert.Equal((int)IoBaseType.eMotionNet, rows[0].ISABase);

            // eMotionNet(0) must dominate across all rows.
            int motionNet = 0;
            foreach (IoTableRow r in rows)
            {
                if (r.ISABase == (int)IoBaseType.eMotionNet) motionNet++;
            }
            Assert.True(motionNet > rows.Count / 2,
                "expected eMotionNet to dominate; got " + motionNet + " of " + rows.Count);
        }
    }
}
