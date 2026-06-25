using System;
using System.Collections.Generic;
using System.IO;

namespace HT9045.Infrastructure
{
    /// <summary>
    /// Minimal INI-file reader that preserves ordered-read semantics.
    ///
    /// BCB6 callers use ReadPrivateProfileString / WritePrivateProfileString via
    /// Windows API, which reads keys in file-order (not alphabetically).  This
    /// skeleton replicates that behaviour by scanning lines top-to-bottom.
    ///
    /// Phase 1: read-only skeleton.  Write-back and section enumeration come in
    /// Phase 2 once the key list is known from the BCB6 .ini files.
    ///
    /// Encoding: files are Big5; pass the path and the codec handles it.
    /// </summary>
    public class ConfigLoader
    {
        private readonly string _filePath;

        // Flat ordered list of (section, key, value) triples — preserves file order.
        private readonly List<IniEntry> _entries = new List<IniEntry>();
        private bool _loaded = false;

        public ConfigLoader(string filePath)
        {
            if (filePath == null) throw new ArgumentNullException("filePath");
            _filePath = filePath;
        }

        /// <summary>
        /// Read a value from [section] key=value, returning defaultValue when
        /// the section or key is absent.  Mirrors ReadPrivateProfileString.
        /// First call triggers a lazy load of the file (Big5-decoded).
        /// </summary>
        public string ReadKey(string section, string key, string defaultValue = "")
        {
            EnsureLoaded();

            string sectionLower = (section ?? "").ToLower();
            string keyLower     = (key    ?? "").ToLower();

            foreach (IniEntry e in _entries)
            {
                if (e.Section.ToLower() == sectionLower &&
                    e.Key.ToLower()     == keyLower)
                {
                    return e.Value;
                }
            }
            return defaultValue;
        }

        // --------------- private helpers ---------------

        private void EnsureLoaded()
        {
            if (_loaded) return;
            _loaded = true;

            if (!File.Exists(_filePath)) return;

            // Big5 decode first; then parse lines.
            byte[] raw  = File.ReadAllBytes(_filePath);
            string text = Big5Codec.ToUtf16(raw);

            string currentSection = "";
            foreach (string rawLine in text.Split('\n'))
            {
                string line = rawLine.Trim('\r').Trim();

                if (line.Length == 0 || line[0] == ';' || line[0] == '#')
                    continue;

                if (line[0] == '[')
                {
                    int close = line.IndexOf(']');
                    currentSection = close > 1 ? line.Substring(1, close - 1).Trim() : "";
                    continue;
                }

                int eq = line.IndexOf('=');
                if (eq <= 0) continue;

                string k = line.Substring(0, eq).Trim();
                string v = line.Substring(eq + 1).Trim();

                // Strip inline comment (BCB6 .ini files occasionally have ; after value)
                int commentIdx = v.IndexOf(';');
                if (commentIdx >= 0) v = v.Substring(0, commentIdx).Trim();

                _entries.Add(new IniEntry(currentSection, k, v));
            }
        }

        // Simple value object — no auto-property syntax to keep LangVersion 7.3 style.
        private struct IniEntry
        {
            public readonly string Section;
            public readonly string Key;
            public readonly string Value;

            public IniEntry(string section, string key, string value)
            {
                Section = section;
                Key     = key;
                Value   = value;
            }
        }
    }
}
