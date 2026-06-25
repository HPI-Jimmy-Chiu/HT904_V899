using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace HT9045.Infrastructure.Config
{
    /// <summary>
    /// Loads Mot_Table.csv into a list of <see cref="MotTableRow"/>.
    ///
    /// Rules mirroring database.cpp behaviour:
    ///  - First non-empty line is the header; it is skipped (not validated).
    ///  - Subsequent non-empty lines are data rows; blank lines are skipped.
    ///  - File is Big5-encoded (cp950); read via Big5Codec (Encoding 950).
    ///  - Split on comma; trailing empty fields are tolerated (short row is OK).
    ///  - Column order is positional and MUST match MotTableRow.FromFields index.
    /// </summary>
    public static class MotTable
    {
        private static readonly Encoding Big5 = Encoding.GetEncoding(950);

        /// <summary>
        /// Parse <paramref name="path"/> and return all data rows.
        /// Returns an empty list when the file does not exist.
        /// Throws <see cref="IOException"/> on read errors.
        /// </summary>
        public static List<MotTableRow> Load(string path)
        {
            if (path == null) throw new ArgumentNullException("path");

            var result = new List<MotTableRow>();

            if (!File.Exists(path))
                return result;

            // Big5 decode
            byte[] raw  = File.ReadAllBytes(path);
            string text = Big5.GetString(raw);

            CsvHeaderMap header = null;
            foreach (string rawLine in text.Split('\n'))
            {
                // Strip CR (Windows \r\n files), then trim surrounding whitespace
                string line = rawLine.TrimEnd('\r').Trim();

                if (line.Length == 0)
                    continue;

                // First non-empty line is the header: build the name->index map once
                // so data rows are resolved BY NAME (robust to column reordering).
                if (header == null)
                {
                    header = new CsvHeaderMap(line.Split(','));
                    continue;
                }

                string[] fields = line.Split(',');
                result.Add(MotTableRow.FromNamedRow(header, fields));
            }

            return result;
        }
    }
}
