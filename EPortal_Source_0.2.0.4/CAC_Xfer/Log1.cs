using System;
using System.Diagnostics;
using System.Reflection;

public static class Log1
{
    public class Mark
    {
        public const char
            Info = '.',
            Error = '!';
    }

    public static void Init(string eventSource)
    {
        try
        {
            if (!EventLog.SourceExists(eventSource))
                throw new ApplicationException("The event source was not found.");

            eventLog = new EventLog(LogName, ".", eventSource);
        }
        catch (Exception ex)
        {
            Abort(ex);
        }
    }

    public static void Info(string s) { Emit(Mark.Info, s, 0); }
    public static void Info(string s, short opc) { Emit(Mark.Info, s, opc); }
    public static void Error(string s) { Emit(Mark.Error, s, 0); }
    public static void Error(string s, short opc) { Emit(Mark.Error, s, opc); }
    public static void Error(Exception ex) { Emit(Mark.Error, ex.ToString(), 0); }
    public static void Error(Exception ex, short opc) { Emit(Mark.Error, ex.ToString(), 0); }

    public static void Emit(char mark, string s, short opc)
    {
        EventLogEntryType logType = (mark == Mark.Error) ? EventLogEntryType.Error : EventLogEntryType.Information;

        try
        {
            eventLog.WriteEntry(s, logType, opc);
            Machine.Display(Prefix(mark) + s);
        }
        catch (Exception ex)
        {
            Abort(ex);
        }
    }

    private static string Prefix(char mark)
    {
        return String.Format("{0} {1:dd.MM.yyyy HH:mm:ss} ", mark, DateTime.Now);
    }

    private static void Abort(Exception ex)
    {
        try
        {
            Machine.Display(Prefix(Mark.Error) + ex.ToString());
        }
        catch
        {
        }

        Machine.Terminate();
    }

    private static string LogName = "Lawsuit_Management";
    private static EventLog eventLog = null;
}
