using System;
using System.Collections.Generic;

namespace HT9045.Infrastructure.Config
{
    /// <summary>
    /// Maps CSV header column names to their physical index so that data rows are
    /// read BY NAME, not by fixed position. This mirrors the golden C++ behaviour:
    /// database.cpp (SetMOTTableNo / SetIOTableNo) resolves Mot_Table / IO_Table
    /// columns by header token and is therefore robust to column reordering between
    /// machine config files. (The C++ uses AnsiPos substring-contains with
    /// last-match-wins; we use exact case-insensitive header tokens — behaviour-
    /// equivalent for every valid, distinctly-named header, and safer than the
    /// substring quirk. Last occurrence wins, matching the C++.)
    ///
    /// WHY THIS MATTERS: a positional loader silently mis-maps every field if a
    /// customer's CSV has its columns in a different order than the dev file. The
    /// C++ tolerates reordering; the C# port must too.
    /// </summary>
    public sealed class CsvHeaderMap
    {
        private readonly Dictionary<string, int> _index =
            new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);

        public CsvHeaderMap(string[] headerColumns)
        {
            if (headerColumns == null) throw new ArgumentNullException("headerColumns");
            for (int i = 0; i < headerColumns.Length; i++)
            {
                string key = (headerColumns[i] ?? string.Empty).Trim();
                if (key.Length == 0) continue;
                _index[key] = i; // last occurrence wins (matches C++ last-match-wins)
            }
        }

        /// <summary>Physical index of <paramref name="name"/>, or -1 if absent.</summary>
        public int IndexOf(string name)
        {
            int idx;
            return _index.TryGetValue((name ?? string.Empty).Trim(), out idx) ? idx : -1;
        }

        public bool Has(string name) { return IndexOf(name) >= 0; }

        public int Count { get { return _index.Count; } }

        /// <summary>
        /// Trimmed value at the column named <paramref name="name"/> in this row's
        /// <paramref name="fields"/>, or empty string if the column is absent or the
        /// row is short. (Empty-on-absent matches the C++ tolerant short-row read.)
        /// </summary>
        public string Value(string[] fields, string name)
        {
            if (fields == null) return string.Empty;
            int idx = IndexOf(name);
            if (idx < 0 || idx >= fields.Length) return string.Empty;
            return (fields[idx] ?? string.Empty).Trim();
        }
    }
}
