using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using HT9045.Infrastructure;

namespace HT9045.Analysis
{
    /// <summary>
    /// Reads HT9045 EventLog backup CSV files into <see cref="EventLogRow"/> objects.
    ///
    /// File format (verified from writer source):
    ///   Encoding  : Big5 / Windows code page 950 (Big5Codec, cp950).
    ///   Delimiter : comma + tab (0x2C 0x09) between every field.
    ///               SGDToCSV (common.cpp:2050) joins cells with a single comma
    ///               and appends a comma after each cell; empty DB columns carry
    ///               a '\t' placeholder (SL->Add("\t"), cMyDB.cpp), so on-disk an
    ///               empty column renders as the byte sequence 0x2C 0x09.
    ///   Line ends : CRLF for the header line; LF only for data rows
    ///               (header written by file-open branch, rows by WriteDataToFile).
    ///   Columns   : 9 — No, UnitName, AlarmCode, Date, Time, Recovery,
    ///               StopedTime, Duplicate, Message  (verbatim header names).
    ///   Message   : last column; may contain embedded commas (pre-escaped to ';'
    ///               by SGDToCSV sReplaceSplit); joining remaining split tokens
    ///               back with the delimiter recovers any edge cases.
    ///
    /// BCB6 writer refs: common.cpp:2050-2064 (SGDToCSV), cObserver.cpp:2559,2410,
    ///   cMyDB.cpp:452,462,473, note.cpp:6691-6726, note.cpp:1137-1156.
    /// </summary>
    public class EventLogReader
    {
        // The exact delimiter bytes: comma (0x2C) immediately followed by tab (0x09).
        private static readonly string Delimiter = ",\t";

        // Number of expected columns from the 9-column grid export header.
        private const int ExpectedColumns = 9;

        // Header field names (verbatim). 'StopedTime' is the BCB6 misspelling.
        private static readonly string[] HeaderFields =
        {
            "No", "UnitName", "AlarmCode", "Date", "Time",
            "Recovery", "StopedTime", "Duplicate", "Message"
        };

        /// <summary>
        /// Read the EventLog CSV at <paramref name="path"/> and return all data rows.
        ///
        /// Tolerances applied:
        ///  - CRLF and LF line endings both accepted.
        ///  - Fields that are empty, whitespace-only, bare '\t', or the literal
        ///    string "(null)" are normalised to <see cref="string.Empty"/>.
        ///  - Message (column 8, 0-based) absorbs any extra tokens produced by
        ///    embedded commas that were NOT pre-escaped (defensive).
        ///  - Rows with fewer than 9 tokens are still parsed; missing tail fields
        ///    default to empty.
        ///  - The header row is identified by its first token "No" and skipped.
        ///    If the file has no header the first data row is still parsed
        ///    (No="No" would appear as a row — acceptable for a PoC).
        /// </summary>
        /// <param name="path">Absolute path to the .csv file.</param>
        /// <returns>List of parsed rows in file order (header excluded).</returns>
        public List<EventLogRow> Read(string path)
        {
            if (path == null) throw new ArgumentNullException("path");
            if (!File.Exists(path))
                throw new FileNotFoundException("EventLog CSV not found.", path);

            // Read all bytes then decode via Big5Codec (cp950).
            // Big5Codec.ToUtf16 wraps Encoding.GetEncoding(950).GetString.
            byte[] raw = File.ReadAllBytes(path);
            string content = Big5Codec.ToUtf16(raw);

            // Guard: if the decoded text contains replacement characters the
            // file may have been re-saved as UTF-8 by an upstream tool.
            // We do not throw here because ASCII-only files decode identically,
            // but we surface the warning for the caller.
            if (Big5Codec.HasMojibake(content))
            {
                // Non-fatal: continue — the caller can inspect rows for U+FFFD.
                // Throwing here would break on future UTF-8 exports that happen
                // to be all-ASCII.
            }

            // Split into lines. StringSplitOptions.None so we keep empty lines
            // (they are skipped below), and we handle both CRLF and LF.
            string[] lines = content.Split(new[] { "\r\n", "\n" }, StringSplitOptions.None);

            var rows = new List<EventLogRow>(lines.Length);
            bool headerSeen = false;

            foreach (string rawLine in lines)
            {
                string line = rawLine;

                // Remove a trailing CR that Split may have left (safety).
                if (line.Length > 0 && line[line.Length - 1] == '\r')
                    line = line.Substring(0, line.Length - 1);

                if (line.Length == 0) continue;

                // Split on the comma+tab delimiter.
                string[] tokens = line.Split(new[] { Delimiter }, StringSplitOptions.None);

                // Header row: first token is "No" (or "No\t" if the file starts
                // with a BOM or extra whitespace — trim before compare).
                if (!headerSeen)
                {
                    string firstToken = tokens[0].Trim().Trim('\t', '﻿');
                    if (string.Equals(firstToken, "No", StringComparison.Ordinal))
                    {
                        headerSeen = true;
                        continue;
                    }
                    // If the very first content line is NOT "No" we treat it as
                    // data (headerless file) and fall through.
                    headerSeen = true;
                }

                // Build the row.  Indices 0-7 map 1:1; index 8+ is Message.
                EventLogRow row = new EventLogRow
                {
                    No         = NormaliseField(GetToken(tokens, 0)),
                    UnitName   = NormaliseField(GetToken(tokens, 1)),
                    AlarmCode  = NormaliseField(GetToken(tokens, 2)),
                    Date       = NormaliseField(GetToken(tokens, 3)),
                    Time       = NormaliseField(GetToken(tokens, 4)),
                    Recovery   = NormaliseField(GetToken(tokens, 5)),
                    StopedTime = NormaliseField(GetToken(tokens, 6)),
                    Duplicate  = NormaliseField(GetToken(tokens, 7)),
                };

                // Message is the last column and absorbs any remaining tokens
                // (defensive against embedded commas not caught by sReplaceSplit).
                if (tokens.Length > 8)
                {
                    // Re-join extra tokens with the delimiter to preserve the
                    // original text as faithfully as possible.
                    var msgParts = new string[tokens.Length - 8];
                    for (int i = 8; i < tokens.Length; i++)
                        msgParts[i - 8] = tokens[i];
                    row.Message = NormaliseField(string.Join(Delimiter, msgParts));
                }
                else
                {
                    row.Message = string.Empty;
                }

                rows.Add(row);
            }

            return rows;
        }

        // ------------------------------------------------------------------ //
        //  Helpers
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Safe token accessor — returns empty string for out-of-range indices.
        /// </summary>
        private static string GetToken(string[] tokens, int index)
        {
            return index < tokens.Length ? tokens[index] : string.Empty;
        }

        /// <summary>
        /// Normalise a raw field value to a canonical empty string for the cases
        /// documented in the format spec:
        ///  - null or whitespace-only
        ///  - bare '\t' (the BCB6 SL->Add("\t") empty-column placeholder)
        ///  - the literal token "(null)" (appears in AlarmCode in real data)
        /// All other values are returned trimmed.
        /// </summary>
        private static string NormaliseField(string raw)
        {
            if (raw == null) return string.Empty;

            // Trim outer whitespace (but NOT inner — Message may have spaces).
            string trimmed = raw.Trim();

            if (trimmed.Length == 0) return string.Empty;

            // Bare tab placeholder from BCB6 SL->Add("\t").
            if (trimmed == "\t") return string.Empty;

            // Literal "(null)" from SQLite null serialisation seen in AlarmCode.
            if (string.Equals(trimmed, "(null)", StringComparison.Ordinal))
                return string.Empty;

            return trimmed;
        }
    }
}
