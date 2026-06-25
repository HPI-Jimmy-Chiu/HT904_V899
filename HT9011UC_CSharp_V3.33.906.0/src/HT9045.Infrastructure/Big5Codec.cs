using System;
using System.Text;

namespace HT9045.Infrastructure
{
    /// <summary>
    /// Static helper for Big5 (code page 950) / UTF-16 round-trip conversion.
    /// The BCB6 source files are Big5-encoded; this codec is used when reading
    /// raw bytes from those files (e.g. .ini, .dat, .csv) and when writing
    /// strings back without corrupting DBCS characters.
    ///
    /// See MEMORY note "HT9045 Big5 檔編輯會亂碼" — Edit/Write on Big5 files
    /// can produce UTF-8 mojibake; always go through this class.
    /// </summary>
    public static class Big5Codec
    {
        // cp950 = Windows Big5 (Traditional Chinese).
        // GetEncoding throws if the encoding is not installed; on a pure .NET
        // Framework 4.8 install it is always present.  On .NET Core the caller
        // must have registered CodePagesEncodingProvider first (Phase 2 concern).
        private static readonly Encoding _big5 = Encoding.GetEncoding(950);

        /// <summary>
        /// Decode a raw Big5 byte array to a .NET UTF-16 string.
        /// Equivalent to the Python  bytes.decode('cp950')  path used for repair.
        /// </summary>
        public static string ToUtf16(byte[] big5Bytes)
        {
            if (big5Bytes == null) throw new ArgumentNullException("big5Bytes");
            return _big5.GetString(big5Bytes);
        }

        /// <summary>
        /// Encode a .NET string to Big5 bytes.
        /// Characters outside Big5's repertoire are replaced by the encoding's
        /// fallback (a '?' byte) — same behaviour as BCB6 AnsiString.
        /// </summary>
        public static byte[] ToBig5(string utf16)
        {
            if (utf16 == null) throw new ArgumentNullException("utf16");
            return _big5.GetBytes(utf16);
        }

        /// <summary>
        /// Returns true when the string contains one or more Unicode replacement
        /// characters (U+FFFD), which indicates that an upstream byte-to-string
        /// conversion used the wrong encoding (mojibake).
        /// Use this as a guard before persisting data back to Big5 files.
        /// </summary>
        public static bool HasMojibake(string text)
        {
            if (text == null) throw new ArgumentNullException("text");
            return text.IndexOf('�') >= 0;
        }
    }
}
