using System;
using System.Collections.Generic;
using System.IO;

namespace HT9045.Infrastructure.Config
{
    /// <summary>
    /// Ordered, read-only INI-file reader mirroring the BCB6
    /// ReadPrivateProfileString / CheckAndReadIniDataGeneral semantics.
    ///
    /// Files are assumed to be Big5 (cp950) encoded — the same encoding used by
    /// the BCB6 source and the runtime .ini files under system/.
    /// Lookup is case-insensitive on both section and key names, and returns the
    /// FIRST match found (file-order preserved), matching Windows API behaviour.
    ///
    /// Inline comments introduced by ';' after the value are stripped.
    /// Blank lines and lines whose first non-space character is ';' or '#' are
    /// ignored.
    /// </summary>
    public sealed class IniFile
    {
        // Ordered list of entries — preserves file-order for diagnostic use.
        private readonly List<Entry> _entries = new List<Entry>();
        private bool _loaded;
        private readonly string _filePath;

        public IniFile(string filePath)
        {
            if (filePath == null) throw new ArgumentNullException("filePath");
            _filePath = filePath;
        }

        // ------------------------------------------------------------------ //
        //  Public API                                                          //
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Returns the raw string value for [section]/key, or
        /// <paramref name="defaultValue"/> when the entry is absent.
        /// Mirrors CheckAndReadIniDataGeneral(section, key, &amp;out, default).
        /// </summary>
        public string GetString(string section, string key, string defaultValue = "")
        {
            EnsureLoaded();
            Entry e;
            if (TryFind(section, key, out e))
                return e.Value;
            return defaultValue;
        }

        /// <summary>
        /// Parses the value as int (base-10).  Returns <paramref name="defaultValue"/>
        /// when the entry is absent or the value is not a valid integer.
        /// Mirrors CheckAndReadIniDataGeneral(section, key, &amp;intOut, default).
        /// </summary>
        public int GetInt(string section, string key, int defaultValue = 0)
        {
            return GetIntDef(section, key, defaultValue);
        }

        /// <summary>
        /// Explicit alias for <see cref="GetInt"/> — provided so callers can
        /// match the BCB6 pattern of passing an explicit default literal.
        /// </summary>
        public int GetIntDef(string section, string key, int defaultValue)
        {
            EnsureLoaded();
            Entry e;
            if (!TryFind(section, key, out e))
                return defaultValue;

            int parsed;
            if (int.TryParse(e.Value, out parsed))
                return parsed;
            return defaultValue;
        }

        // ------------------------------------------------------------------ //
        //  Private helpers                                                     //
        // ------------------------------------------------------------------ //

        private bool TryFind(string section, string key, out Entry result)
        {
            string sLow = (section ?? string.Empty).ToLowerInvariant();
            string kLow = (key     ?? string.Empty).ToLowerInvariant();

            foreach (Entry e in _entries)
            {
                if (e.SectionLow == sLow && e.KeyLow == kLow)
                {
                    result = e;
                    return true;
                }
            }
            result = default(Entry);
            return false;
        }

        private void EnsureLoaded()
        {
            if (_loaded) return;
            _loaded = true;

            if (!File.Exists(_filePath)) return;

            // Decode Big5 bytes to UTF-16 before parsing — never use StreamReader
            // with default encoding on these files (would silently produce mojibake).
            byte[] raw  = File.ReadAllBytes(_filePath);
            string text = Big5Codec.ToUtf16(raw);

            string currentSection    = string.Empty;
            string currentSectionLow = string.Empty;

            // Split on LF; strip CR so CRLF and LF files both work.
            string[] lines = text.Split('\n');
            foreach (string rawLine in lines)
            {
                string line = rawLine.TrimEnd('\r').Trim();

                if (line.Length == 0)
                    continue;

                char first = line[0];
                if (first == ';' || first == '#')
                    continue;

                if (first == '[')
                {
                    int close = line.IndexOf(']');
                    if (close > 1)
                    {
                        currentSection    = line.Substring(1, close - 1).Trim();
                        currentSectionLow = currentSection.ToLowerInvariant();
                    }
                    continue;
                }

                int eq = line.IndexOf('=');
                if (eq <= 0) continue;

                string k = line.Substring(0, eq).Trim();
                string v = line.Substring(eq + 1).Trim();

                // Strip inline comment — BCB6 .ini values occasionally carry '; remark'
                int commentIdx = v.IndexOf(';');
                if (commentIdx >= 0)
                    v = v.Substring(0, commentIdx).Trim();

                _entries.Add(new Entry(currentSection, currentSectionLow, k, v));
            }
        }

        // ------------------------------------------------------------------
        //  Value type — no auto-properties to stay within LangVersion 7.3.
        // ------------------------------------------------------------------
        private struct Entry
        {
            public readonly string Section;
            public readonly string SectionLow;
            public readonly string Key;
            public readonly string KeyLow;
            public readonly string Value;

            public Entry(string section, string sectionLow, string key, string value)
            {
                Section    = section;
                SectionLow = sectionLow;
                Key        = key;
                KeyLow     = key.ToLowerInvariant();
                Value      = value;
            }
        }
    }
}
