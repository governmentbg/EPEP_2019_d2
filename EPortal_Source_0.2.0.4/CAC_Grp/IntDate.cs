﻿using System;

public class IntDate : IntDate66k
{
    private class DateException : Exception
    {
        public DateException(Int32 value) : base(String.Format("Invalid date value {0}", value)) { }
    }

    public static int ToInt(int yyyymmdd)
    {
        string s;
        s = "" + yyyymmdd;
        s = s.Substring(6) + "." + s.Substring(4,2) + "." + s.Substring(0,4); 
        return ToInt(s);
    }

    public static int ToInt(DateTime date)
    {
        Int32 Seed = Randomize(date);
        int hour = my_rand(ref Seed) % 26 - 1;
        int minute = my_rand(ref Seed) % 60;
        return Value(date, hour, minute);
    }

    public static int ToInt(string s)
    {
        return ToInt(Reset.Date.Parse(s));
    }

    public static int Today()
    {
        return ToInt(DateTime.Now.ToString("dd.MM.yyyy"));
    }

    public static DateTime ToDate(int value)
    {
        int hour, minute;
        DateTime date = SetValue(value, out hour, out minute);
        Int32 Seed = Randomize(date);
        if (hour != my_rand(ref Seed) % 26 - 1 || minute != my_rand(ref Seed) % 60)
            throw new DateException(value);
        return date;
    }

    public static Int32 Value(DateTime date, Int32 hour, Int32 minute)
    {
        Int32 full_value = (date.Year - Range.Year.Min) * 580320 + (date.Month - 1) * 48360 + (date.Day - 1) * 1560 +
            (hour + 1) * 60 + minute;
        return get_base(full_value / MinDelta) + full_value % MinDelta;
    }

    protected static DateTime SetValue(Int32 value, out int hour, out int minute)
    {
        int i;
        int l = 0, h = (BaseCount * BasePeriod) - 1;
        Int32 base_;

        for (; ; )
        {
            i = (l + h) / 2;
            base_ = get_base(i);

            if (value < base_)
                h = i - 1;
            else if (value >= base_ + MinDelta)
                l = i + 1;
            else
                break;

            if (h < l)
                throw new DateException(value);
        }

        Int32 full_value = i * MinDelta + value - base_;

        minute = full_value % 60;
        full_value /= 60;
        hour = full_value % 26 - 1;
        full_value /= 26;
        int day = full_value % 31 + 1;
        full_value /= 31;
        int month = full_value % 12 + 1;
        full_value /= 12;
        int year = full_value + Range.Year.Min;

        return new DateTime(year, month, day);
    }

    protected static Int32 Randomize(DateTime date)
    {
        return (date.Year - Range.Year.Min) * 372 + (date.Month - 1) * 31 + date.Day;
    }
}

public static class IntTime
{
    public static DateTime ToTime(int value)
    {
        DateTime today = DateTime.Today;
        return new DateTime(today.Year, today.Month, today.Day, value / 60, value % 60, 0);
    }
}
