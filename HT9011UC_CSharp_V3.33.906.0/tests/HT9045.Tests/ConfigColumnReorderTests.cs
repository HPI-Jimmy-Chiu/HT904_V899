using HT9045.Infrastructure.Config;
using Xunit;

namespace HT9045.Tests
{
    /// <summary>
    /// Proves the config CSV loaders resolve columns BY HEADER NAME (mirroring the
    /// golden C++ database.cpp SetMOTTableNo/SetIOTableNo), so a machine file whose
    /// columns are in a different physical order than the dev file still maps
    /// correctly. A positional loader would silently mis-map here — that was the bug
    /// this guards against.
    /// </summary>
    public class ConfigColumnReorderTests
    {
        [Fact]
        public void MotTable_ReorderedColumns_ResolveByName()
        {
            // CardModel deliberately moved to the FRONT; Motorname/Port shuffled.
            string[] header = { "CardModel", "Port", "Motorname", "Alias", "BoardID", "Direction" };
            string[] row    = { "SMC",       "3",    "M00",       "MInArmX", "0",      "1" };

            var map = new CsvHeaderMap(header);
            MotTableRow r = MotTableRow.FromNamedRow(map, row);

            Assert.Equal("SMC", r.CardModel);
            Assert.Equal("M00", r.Motorname);
            Assert.Equal("MInArmX", r.Alias);
            Assert.Equal("0", r.BoardID);
            Assert.Equal("3", r.Port);
            // Columns absent from this reordered header resolve to empty (tolerant).
            Assert.Equal(string.Empty, r.GearRatio);
        }

        [Fact]
        public void MotTable_Positional_WouldMismap_WhenReordered()
        {
            // Same reordered row read POSITIONALLY (the old behaviour) maps the field
            // at index 22 (CardModel slot) to nothing and CardModel to the wrong cell.
            string[] row = { "SMC", "3", "M00", "MInArmX", "0", "1" };
            MotTableRow bad = MotTableRow.FromFields(row);

            // Positional: index 0 is treated as Motorname, so "SMC" lands in Motorname,
            // and CardModel (index 22) is empty -> demonstrably wrong vs name-based.
            Assert.Equal("SMC", bad.Motorname);          // wrong field got the value
            Assert.Equal(string.Empty, bad.CardModel);   // CardModel lost
        }

        [Fact]
        public void IoTable_ReorderedColumns_ResolveByName_IsaBaseInt()
        {
            // ISABase moved away from its canonical position 8.
            string[] header = { "ISABase", "IOType", "Alias", "Lane", "IP", "Port", "Bit", "Enable" };
            string[] row    = { "1",       "Switch",  "SwClear0", "0", "42", "0",   "4",   "1" };

            var map = new CsvHeaderMap(header);
            IoTableRow r = IoTableRow.FromNamedRow(map, row);

            Assert.Equal(1, r.ISABase);           // eISABase, resolved by name not position
            Assert.Equal("Switch", r.IOType);
            Assert.Equal("SwClear0", r.Alias);
            Assert.Equal("42", r.IP);
        }

        [Fact]
        public void IoTable_MissingIsaBase_DefaultsToMotionNet()
        {
            string[] header = { "IOType", "Alias", "Lane", "IP", "Port", "Bit" }; // no ISABase
            string[] row    = { "Sucker", "BTestSuckAA", "0", "7", "0", "4" };

            var map = new CsvHeaderMap(header);
            IoTableRow r = IoTableRow.FromNamedRow(map, row);

            Assert.Equal(0, r.ISABase); // empty/absent -> 0 = eMotionNet (matches database.cpp)
        }
    }
}
