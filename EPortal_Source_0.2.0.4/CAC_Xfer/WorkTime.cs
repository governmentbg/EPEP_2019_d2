using System;
using System.Threading;

public class WorkTime
{
    public readonly string StartName;
    public readonly string FinalName;

    public WorkTime(string StartName, string FinalName)
    {
        this.StartName = StartName;
        this.FinalName = FinalName;
        start = Utility.NewTime(8, 0, 0);
        final = Utility.NewTime(20, 0, 0);
    }

    public void GetStart(string s) { start = Range.GetTime(StartName, s, false); }
    public void GetFinal(string s) { final = Range.GetTime(FinalName, s, false); }

    public void CheckMinimal(int hours)
    {
        if (final - start < new TimeSpan(10, 0, 0))
            throw new RangeException("{0} - {1} < 10 hours", FinalName, StartName);
    }

    public bool IsIdle()
    {
        DateTime time = Utility.NowTime(false);
        return time < start || time > final;
    }

    public bool Paused()
    {
        if (IsIdle())
        {
            Thread.Sleep(1000);
            return true;
        }

        return false;
    }

    private DateTime start;
    private DateTime final;
}
