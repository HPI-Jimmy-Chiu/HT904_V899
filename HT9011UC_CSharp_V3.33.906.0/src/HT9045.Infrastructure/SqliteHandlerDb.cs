using System;
using Microsoft.Data.Sqlite;

namespace HT9045.Infrastructure
{
    /// <summary>
    /// IHandlerDb implementation backed by Microsoft.Data.Sqlite 8.0.0.
    ///
    /// BCB6 equivalent: the inline sqlite3_open / sqlite3_prepare_v2 /
    /// sqlite3_step / sqlite3_column_text calls scattered through cMyDB and
    /// related modules.  This class centralises connection lifetime so the
    /// rest of Phase 1 stays free of sqlite3.h concerns.
    ///
    /// Thread safety: NOT thread-safe in Phase 1.  The BCB6 code used a single
    /// DB connection per module; multi-threaded access is a Phase 3 concern.
    /// </summary>
    public class SqliteHandlerDb : IHandlerDb, IDisposable
    {
        private SqliteConnection _connection = null;
        private bool _disposed = false;

        // ------------------------------------------------------------------ //
        //  IHandlerDb
        // ------------------------------------------------------------------ //

        public void Open(string dbPath)
        {
            if (_disposed) throw new ObjectDisposedException("SqliteHandlerDb");
            if (_connection != null) return;   // idempotent

            if (dbPath == null) throw new ArgumentNullException("dbPath");

            string connStr = new SqliteConnectionStringBuilder
            {
                DataSource = dbPath,
                // CREATE the file if it does not exist (matches sqlite3_open behaviour)
                Mode = SqliteOpenMode.ReadWriteCreate
            }.ToString();

            _connection = new SqliteConnection(connStr);
            _connection.Open();
        }

        /// <summary>
        /// Execute a scalar SQL query.  nameValuePairs must contain an even
        /// number of strings (name, value, name, value, ...).
        /// Returns null when the result set is empty.
        /// </summary>
        public string QueryScalar(string sql, params string[] nameValuePairs)
        {
            if (_disposed) throw new ObjectDisposedException("SqliteHandlerDb");
            if (_connection == null)
                throw new InvalidOperationException("Call Open() before QueryScalar().");
            if (sql == null) throw new ArgumentNullException("sql");

            if (nameValuePairs != null && nameValuePairs.Length % 2 != 0)
                throw new ArgumentException(
                    "nameValuePairs must contain an even number of elements (name/value pairs).",
                    "nameValuePairs");

            using (SqliteCommand cmd = _connection.CreateCommand())
            {
                cmd.CommandText = sql;

                if (nameValuePairs != null)
                {
                    for (int i = 0; i < nameValuePairs.Length; i += 2)
                    {
                        string paramName  = nameValuePairs[i];
                        string paramValue = nameValuePairs[i + 1];
                        cmd.Parameters.AddWithValue(paramName, (object)paramValue ?? DBNull.Value);
                    }
                }

                object result = cmd.ExecuteScalar();
                if (result == null || result == DBNull.Value) return null;
                return result.ToString();
            }
        }

        public void Close()
        {
            if (_connection == null) return;
            _connection.Close();
            _connection.Dispose();
            _connection = null;
        }

        // ------------------------------------------------------------------ //
        //  IDisposable
        // ------------------------------------------------------------------ //

        public void Dispose()
        {
            if (_disposed) return;
            Close();
            _disposed = true;
        }
    }
}
