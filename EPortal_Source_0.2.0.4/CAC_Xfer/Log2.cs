using System;
using System.Data.Odbc;
using System.Diagnostics;
using System.IO;

public static class Log2
{
    public enum Level
    {
        Info = EventLogEntryType.Information,
        Error = EventLogEntryType.Error
    }

    public enum Operation
    {
        None = 0,  // legacy/import
        Init = 1,
        Exit = 2,
        Send = 3,
        Recv = 4,
        LogX = 5,
        User = 10  // app specific
    }

    public static string EventSource = null;

    public static void Init(string directory)
    {
        string journalName = Path.Combine(directory, JournalLogName);
        string parentDir = Path.GetDirectoryName(Path.GetDirectoryName(journalName));  // avoid "directory\\"

        try
        {
            if (File.Exists(journalName) || parentDir == directory || parentDir == Path.GetPathRoot(parentDir))
                connection = Open(journalName);
            else
                connection = Open(Path.Combine(parentDir, JournalLogName));
        }
        catch (Exception ex)
        {
            Abort(ex);
        }
    }

    public static int Info(Operation oper, string message) { return Emit(Level.Info, oper, message); }
    public static int Error(Operation oper, string message) { return Emit(Level.Error, oper, message); }

    public static void Extra(SqlInsert insert)
    {
        try
        {
            connection.Execute(insert);
        }
        catch (Exception ex)
        {
            Abort(ex);
        }
    }

    public static int Emit(Level level, Operation oper, string message)
    {
        try
        {
            DateTime now = DateTime.Now;
            Machine.Display(Prefix(level, now) + message);

            SqlSelect select = new SqlSelect("RDB$DATABASE", "GEN_ID(G_LOG_LOG, 1) AS F_LOG");
            int logId;

            using (Query q = new Query(connection, select))
            {
                q.Read();
                logId = q.GetInt("F_LOG");
            }

            SqlInsert insert = new SqlInsert("T_LOG");

            insert.AddInt("F_LOG", logId);
            insert.AddString("F_PROGRAM", EventSource);
            insert.AddDateTime("F_STAMP", now);
            insert.AddInt("F_LEVEL", (int) level);
            insert.AddInt("F_OPER", (int) oper);
            insert.AddString("F_MESSAGE", message);

            connection.Execute(insert);
            return logId;
        }
        catch (Exception ex)
        {
            LogEvent(level, oper, message);
            Abort(ex);
            throw ex;
        }
    }

    private static string StampString(DateTime stamp)
    {
        return stamp.ToString("");
    }

    private static string Prefix(Level level, DateTime stamp)
    {
        return String.Format("{0} {1:dd.MM.yyyy HH:mm:ss} ", (level == Level.Info) ? '.' : '!', stamp);
    }

    private static Connection Open(string journalName)
    {
        string connectionString = "127.0.0.1:" + journalName;

        try
        {
            return new Connection("odbc.firebird", connectionString);
        }
        catch (OdbcException ex)
        {
            if (ex.Message.Contains("File Database is not found"))
                throw ex;
        }

        return new Connection("lcpi.ibprovider", connectionString);
    }

    private static void LogEvent(Level level, Operation oper, string message)
    {
        try
        {
            if (EventSource != null && EventLog.SourceExists(EventSource))
            {
                EventLog eventLog = new EventLog(WindowsLogName, ".", EventSource);

                eventLog = new EventLog(WindowsLogName, ".", EventSource);
                eventLog.WriteEntry(message, (EventLogEntryType) level, (int) oper);
            }
        }
        catch
        {
        }
    }

    private static void Abort(Exception ex)
    {
        LogEvent(Level.Error, Operation.LogX, ex.ToString());

        try
        {
            Machine.Display(String.Format("! {0:dd.MM.yyyy HH:mm:ss} ", DateTime.Now) + ex.ToString());
        }
        catch
        {
        }

        Machine.Terminate();
    }

    private static string WindowsLogName = "Lawsuit_Management";
    private static string JournalLogName = "journal.gdb";
    private static Connection connection = null;
}
