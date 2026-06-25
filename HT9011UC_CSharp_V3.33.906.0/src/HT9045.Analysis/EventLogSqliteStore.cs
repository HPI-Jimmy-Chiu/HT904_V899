using System;
using System.Collections.Generic;
using Microsoft.Data.Sqlite;

namespace HT9045.Analysis
{
    /// <summary>
    /// Persists <see cref="EventLogRow"/> data to a local SQLite database and
    /// exposes query helpers that prove the C# &lt;-&gt; SQLite chain.
    ///
    /// Table schema mirrors the EventLog export columns:
    ///   event_log(id INTEGER PK, no TEXT, unit_name TEXT, alarm_code TEXT,
    ///             date TEXT, time TEXT, recovery TEXT, stoped_time TEXT,
    ///             duplicate TEXT, message TEXT)
    ///
    /// Column name 'stoped_time' preserves the BCB6 misspelling from the CSV
    /// header ('StopedTime') so future cross-checks against the writer source
    /// (cMyDB.cpp, note.cpp) remain unambiguous.
    ///
    /// This class does NOT implement IHandlerDb because IHandlerDb.QueryScalar
    /// returns a single scalar — insufficient for the multi-column aggregation
    /// in QuerySummary.  It uses Microsoft.Data.Sqlite directly (same NuGet
    /// dependency transitively available via HT9045.Infrastructure).
    /// </summary>
    public class EventLogSqliteStore
    {
        private const string CreateTableSql = @"
CREATE TABLE IF NOT EXISTS event_log (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    no          TEXT,
    unit_name   TEXT,
    alarm_code  TEXT,
    date        TEXT,
    time        TEXT,
    recovery    TEXT,
    stoped_time TEXT,
    duplicate   TEXT,
    message     TEXT
);";

        private const string InsertSql = @"
INSERT INTO event_log
    (no, unit_name, alarm_code, date, time, recovery, stoped_time, duplicate, message)
VALUES
    (@no, @unit_name, @alarm_code, @date, @time, @recovery, @stoped_time, @duplicate, @message);";

        // ------------------------------------------------------------------ //
        //  Public API
        // ------------------------------------------------------------------ //

        /// <summary>
        /// Create the 'event_log' table in <paramref name="dbPath"/> (creating
        /// the file if needed) and bulk-insert <paramref name="rows"/> inside a
        /// single transaction.
        ///
        /// Idempotent with respect to table creation (CREATE TABLE IF NOT EXISTS).
        /// Rows are always appended — call this only once per DB file, or on a
        /// fresh DB, to avoid duplicates.
        /// </summary>
        /// <param name="rows">Rows from EventLogReader.Read().</param>
        /// <param name="dbPath">Path to the target SQLite file.</param>
        public void Ingest(IList<EventLogRow> rows, string dbPath)
        {
            if (rows == null)    throw new ArgumentNullException("rows");
            if (dbPath == null)  throw new ArgumentNullException("dbPath");

            string connStr = BuildConnectionString(dbPath);

            using (var conn = new SqliteConnection(connStr))
            {
                conn.Open();

                // Create table.
                using (var cmd = conn.CreateCommand())
                {
                    cmd.CommandText = CreateTableSql;
                    cmd.ExecuteNonQuery();
                }

                // Bulk insert in a single transaction for performance.
                using (var tx = conn.BeginTransaction())
                {
                    using (var cmd = conn.CreateCommand())
                    {
                        cmd.Transaction  = tx;
                        cmd.CommandText  = InsertSql;

                        // Prepare parameters once; rebind values per row.
                        cmd.Parameters.Add("@no",          SqliteType.Text);
                        cmd.Parameters.Add("@unit_name",   SqliteType.Text);
                        cmd.Parameters.Add("@alarm_code",  SqliteType.Text);
                        cmd.Parameters.Add("@date",        SqliteType.Text);
                        cmd.Parameters.Add("@time",        SqliteType.Text);
                        cmd.Parameters.Add("@recovery",    SqliteType.Text);
                        cmd.Parameters.Add("@stoped_time", SqliteType.Text);
                        cmd.Parameters.Add("@duplicate",   SqliteType.Text);
                        cmd.Parameters.Add("@message",     SqliteType.Text);

                        foreach (EventLogRow row in rows)
                        {
                            cmd.Parameters["@no"].Value          = NullSafe(row.No);
                            cmd.Parameters["@unit_name"].Value   = NullSafe(row.UnitName);
                            cmd.Parameters["@alarm_code"].Value  = NullSafe(row.AlarmCode);
                            cmd.Parameters["@date"].Value        = NullSafe(row.Date);
                            cmd.Parameters["@time"].Value        = NullSafe(row.Time);
                            cmd.Parameters["@recovery"].Value    = NullSafe(row.Recovery);
                            cmd.Parameters["@stoped_time"].Value = NullSafe(row.StopedTime);
                            cmd.Parameters["@duplicate"].Value   = NullSafe(row.Duplicate);
                            cmd.Parameters["@message"].Value     = NullSafe(row.Message);

                            cmd.ExecuteNonQuery();
                        }
                    }

                    tx.Commit();
                }
            }
        }

        /// <summary>
        /// Recompute per-UnitName row counts by querying the 'event_log' table
        /// in <paramref name="dbPath"/>.  This proves the C# &lt;-&gt; SQLite
        /// chain without re-reading the CSV file.
        ///
        /// Mirrors the in-memory result of EventLogAnalyzer.Summarize().RowsByUnitName.
        ///
        /// SQL equivalent:
        ///   SELECT unit_name, COUNT(*) AS cnt
        ///   FROM event_log
        ///   WHERE unit_name IS NOT NULL AND unit_name != ''
        ///   GROUP BY unit_name
        ///   ORDER BY cnt DESC, unit_name ASC
        /// </summary>
        /// <param name="dbPath">Path to the SQLite file created by Ingest().</param>
        /// <returns>Dictionary keyed by UnitName, value = row count.</returns>
        public Dictionary<string, int> QuerySummary(string dbPath)
        {
            if (dbPath == null) throw new ArgumentNullException("dbPath");

            const string sql = @"
SELECT unit_name, COUNT(*) AS cnt
FROM   event_log
WHERE  unit_name IS NOT NULL AND unit_name != ''
GROUP  BY unit_name
ORDER  BY cnt DESC, unit_name ASC;";

            string connStr = BuildConnectionString(dbPath);
            var result = new Dictionary<string, int>(StringComparer.Ordinal);

            using (var conn = new SqliteConnection(connStr))
            {
                conn.Open();

                using (var cmd = conn.CreateCommand())
                {
                    cmd.CommandText = sql;

                    using (SqliteDataReader reader = cmd.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            string unitName = reader.IsDBNull(0) ? string.Empty : reader.GetString(0);
                            int    count    = reader.GetInt32(1);
                            if (!string.IsNullOrEmpty(unitName))
                                result[unitName] = count;
                        }
                    }
                }
            }

            return result;
        }

        /// <summary>
        /// Total row count in the 'event_log' table.
        /// Quick sanity check after Ingest().
        /// </summary>
        public int QueryTotalRows(string dbPath)
        {
            if (dbPath == null) throw new ArgumentNullException("dbPath");

            string connStr = BuildConnectionString(dbPath);

            using (var conn = new SqliteConnection(connStr))
            {
                conn.Open();

                using (var cmd = conn.CreateCommand())
                {
                    cmd.CommandText = "SELECT COUNT(*) FROM event_log;";
                    object result = cmd.ExecuteScalar();
                    if (result == null || result == DBNull.Value) return 0;
                    return Convert.ToInt32(result);
                }
            }
        }

        // ------------------------------------------------------------------ //
        //  Helpers
        // ------------------------------------------------------------------ //

        private static string BuildConnectionString(string dbPath)
        {
            return new SqliteConnectionStringBuilder
            {
                DataSource = dbPath,
                Mode       = SqliteOpenMode.ReadWriteCreate
            }.ToString();
        }

        /// <summary>
        /// Map an empty string to DBNull so SQLite stores NULL rather than ''.
        /// Empty fields in the CSV (normalised to "" by EventLogReader) should
        /// be NULL in the DB to keep aggregation queries clean.
        /// </summary>
        private static object NullSafe(string value)
        {
            return string.IsNullOrEmpty(value) ? (object)DBNull.Value : value;
        }
    }
}
