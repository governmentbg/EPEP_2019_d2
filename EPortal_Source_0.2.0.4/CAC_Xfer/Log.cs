using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;

public class LogException : Exception
{
    public LogException(Exception innerException) : base(null, innerException) { }
}

public static class Log
{
    public class Mark
    {
        public const char
            Info = '.',
            Start = '+',
            Error = '!';
    };

    public static void Info(string s) { Emit(Mark.Info, s); }
    public static void Info(string format, params object[] args) { Emit(Mark.Info, String.Format(format, args)); }
    public static void Error(string s) { Emit(Mark.Error, s); }
    public static void Error(string format, params object[] args) { Emit(Mark.Error, String.Format(format, args)); }
    public static void Error(Exception ex) { Emit(Mark.Error, ex.ToString()); }
    public static void Emit(char mark, string s) { Emit(mark, s, DateTime.Now); }

    public static void Final(Exception ex)
    {
        if (Machine.HasEntryPoint)
        {
            Log.Error("FATAL: {0}", ex.ToString());

            if (Machine.Interactive)
                Thread.Sleep(3000);

            Environment.Exit(1);
        }
        else
        {
            Exception plex = ex;

            do
            {
                if (plex is LogException)
                    break;
            } while ((plex = plex.InnerException) != null);

            if (plex == null)
            {
                try
                {
                    Log.Error(ex);
                }
                catch
                {
                }
            }
        }
    }

    public static void Emit(char mark, string s, DateTime stamp)
    {
        string message = String.Format("{0} {1:D2}.{2:D2}.{3:D4} {4:D2}:{5:D2}:{6:D2} {7}", mark, stamp.Day, stamp.Month,
            stamp.Year, stamp.Hour, stamp.Minute, stamp.Second, s);

        try
        {
            if (Machine.Interactive)
                Console.WriteLine(message);

            writer.WriteLine(message);
        }
        catch (Exception ex)
        {
            LogAbort(ex, mark == Mark.Error ? s : null);
        }
    }

    private static void LogAbort(Exception ex, string s)
    {
        try
        {
            EventLog eventLog = new EventLog();

            if (!EventLog.SourceExists(EventSource))
                EventLog.CreateEventSource(EventSource, EventLogName);

            eventLog.Source = EventSource;

            if (s != null)
                eventLog.WriteEntry(s, EventLogEntryType.Error);

            eventLog.WriteEntry(ex.ToString(), EventLogEntryType.Error);
        }
        catch
        {
        }

        if (Machine.HasEntryPoint)
            Environment.Exit(1);
        else
            throw new LogException(ex);
    }

    private static string EventSource;
    private static string EventLogName = "Lawsuit_Management";
    private static string LogFileName;
    private static StreamWriter writer = null;

    public static void Init(string EventSource, string directory)
    {
        Log.EventSource = EventSource;

        if (writer == null)
        {
            try
            {
                if (!Path.IsPathRooted(directory))
                    throw new RangeException("{0}_Temp: must be an absolute path.", EventSource);

                DateTime now = DateTime.Now;

                LogFileName = Path.Combine(directory, String.Format("{0}_{1}_{2}.Log", EventSource, now.Year, now.Month));
                writer = new StreamWriter(LogFileName, true);
                writer.AutoFlush = true;
            }
            catch (Exception ex)
            {
                LogAbort(ex, null);
            }
        }
    }

    public static void Init(string EventLogName, string EventSource, string directory)
    {
        Log.EventLogName = EventLogName;
        Init(EventSource, directory);
    }

    public const int MaxDump = 100000;

    public static string Dump()
    {
        bool cut;
        byte[] array = new byte[MaxDump];
        int read;
        FileStream reader = File.Open(LogFileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);

        try
        {
            reader.Seek(-MaxDump, SeekOrigin.End);
            cut = true;
        }
        catch
        {
            reader.Seek(0, SeekOrigin.Begin);
            cut = false;
        }

        read = reader.Read(array, 0, MaxDump);
        string text = Encoding.UTF8.GetString(array, 0, read);

        if (cut)
        {
            int line = text.IndexOf('\n');
            if (line >= 0)
                text = "-- cut --\n" + text.Substring(line + 1);
        }

        return text;
    }
}
