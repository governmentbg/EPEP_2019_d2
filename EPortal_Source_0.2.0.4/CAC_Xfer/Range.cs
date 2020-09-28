using System;
using System.Globalization;
using System.IO;

public class RangeException : Exception
{
    public RangeException(string message) : base(message) { }
    public RangeException(string format, params object[] args) : base(String.Format(format, args)) { }
}

public class Range
{
    public Range(string Name, int Min, int Max)
    {
        this.Name = Name;
        this.Min = Min;
        this.Max = Max;
    }

    public static readonly Range  // common CAC ranges
        Court = new Range("Court", 1, 999),
        Area = new Range("Area", 1, 99),
        No = new Range("No", 1, 999999),
        Year = new Range("Year", 1970, 2037),
        Kind = new Range("Kind", 33, 255),
        Image = new Range("Image", 0, 1),
        PortalNo = new Range("PortalNo", 0, 190),
        Month = new Range("Month", 1, 12),
        BirthYear = new Range("BirthYear", 1870, 2037);

    public readonly string Name;
    public readonly int Min;
    public readonly int Max;

    public bool IsMatch(int i) { return i >= Min && i <= Max; }
    public int Get(int i) { return Get(Name, Min, Max, i); }
    public int Get(string s) { return Get(Name, Min, Max, s); }

    public static int Get(string name, int min, int max, int i)
    {
        if (i < min || i > max)
            throw new RangeException("{0} {1} : must be {2}...{3}.", name, i, min, max);

        return i;
    }

    public static int Get(string name, int min, int max, string s)
    {
        int i;

        try
        {
            if (s == null)
                throw new NullReferenceException();

            i = Convert.ToInt32(s);
        }
        catch (Exception ex)
        {
            throw new RangeException("{0} '{1}' : {2}", name, s, ex.Message);
        }

        return Get(name, min, max, i);
    }

    public static DateTime GetDate(string name, string value, string format)
    {
        try
        {
            return DateTime.ParseExact(value, format, CultureInfo.CurrentCulture);
        }
        catch (Exception ex)
        {
            throw new RangeException("{0} '{1}' : {2}", name, value, ex.Message);
        }
    }

    public static DateTime GetTime(string name, string value, bool seconds)
    {
        string[] parts = value.Split(':');
        int hour, minute, second;

        if (parts.Length != (seconds ? 3 : 2))
            throw new RangeException("{0}: invalid time format", name);

        hour = GetTimeValue(name, "hour", parts[0], 23);
        minute = GetTimeValue(name, "minute", parts[1], 59);
        second = seconds ? GetTimeValue(name, "second", parts[2], 59) : 0;

        return Utility.NewTime(hour, minute, second);
    }

    private static int GetTimeValue(string name, string part, string s, int max)
    {
        int i;

        try
        {
            i = Convert.ToInt32(s);
            if (i < 0 || i > max)
                throw new Exception();
        }
        catch
        {
            throw new RangeException("{0} '{1}': invalid {2}", name, s, part);
        }

        return i;
    }
}
