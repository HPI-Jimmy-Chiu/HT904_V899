namespace HT9045.Infrastructure
{
    /// <summary>
    /// Minimal persistence contract for the handler's runtime database.
    ///
    /// BCB6 uses SQLite3 (sqlite3.h, sqlite3.lib) directly via C API.
    /// This interface wraps the two operations Phase 1 needs: opening a
    /// connection and running a single scalar query.
    ///
    /// The real implementation lives in SqliteHandlerDb; a stub/in-memory
    /// version can be swapped in for unit tests without touching SQLite.
    /// </summary>
    public interface IHandlerDb
    {
        /// <summary>
        /// Open the database at dbPath, creating the file if it does not exist.
        /// Must be called before any query.  Idempotent on repeated calls.
        /// </summary>
        void Open(string dbPath);

        /// <summary>
        /// Execute a scalar SQL query with optional positional parameters and
        /// return the first column of the first row as a string, or null when
        /// the result set is empty.
        ///
        /// Parameters are passed as alternating name/value pairs, e.g.:
        ///   ("SELECT val FROM t WHERE id=@id", "@id", "42")
        /// This keeps the signature simple and avoids a System.Collections
        /// dependency that would add noise in Phase 1.
        /// </summary>
        string QueryScalar(string sql, params string[] nameValuePairs);

        /// <summary>
        /// Close and dispose the underlying connection.  Safe to call when not open.
        /// </summary>
        void Close();
    }
}
