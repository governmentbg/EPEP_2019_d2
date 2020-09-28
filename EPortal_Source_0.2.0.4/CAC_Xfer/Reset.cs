using System;
using System.Globalization;
using System.Text.RegularExpressions;

public partial class Reset
{
    public const bool Chars = true, Regex = false;
    public static readonly ResetDate Date = new ResetDate();

    public Reset(string Name, string s, bool charset)
    {
        this.Name = Name;

        if (charset)
        {
            Set = s;
            Expr = String.Format("^[{0}]$", s);
        }
        else
        {
            Set = null;
            Expr = String.Format("^{0}$", s);
        }

        regex = new Regex(Expr);
    }

    private const string CitizenNameStart = "[А-Яа-яA-Za-z-]";

    public static readonly Reset  // common CAC resets
        CitizenName = new Reset("CitizenName", CitizenNameStart + "{1,14}", Reset.Regex),
        CitizenReName = new Reset("CitizenReName", CitizenNameStart + "{0,15}", Reset.Regex),
        CitizenFamily = new Reset("CitizenFamily", CitizenNameStart + "{1,22}", Reset.Regex),
        CitizenReFamily = new Reset("CitizenReFamily", CitizenNameStart + "{0,22}", Reset.Regex),
        FirmName = new Reset("FirmName", "[А-Яа-яA-Za-z0-9\"\\-:,&!/]" +
            "[А-Яа-яA-Za-z0-9 .\"\\-:,&!/]{2,99}", Reset.Regex),
        Ucn = new Reset("Ucn", "[0-9]{10}", Reset.Regex),
        ValidUcn = new Reset("Ucn", "[0-9][0-9](0[1-9]|[1-9][0-9]){2}[0-9]{4}", Reset.Regex),
        Uid = new Reset("Uid", "[0-9A-Za-z.-]{22}", Reset.Regex),
        UcnType = new Reset("UcnType", "CfFd", Reset.Chars),  // 2018-05-03: -c
        Bulstat = new Reset("Bulstat", "[А-ЯA-Z0-9]{9,15}", Reset.Regex),
        Image = new Reset("Image", "01", Reset.Chars);

    public readonly string Name;
    public readonly string Expr;
    public readonly string Set;

    public bool Contains(char c)
    {
        if (Set == null)
            throw new ArgumentException(String.Format("{0} contains '{1}': not a charset", Name, c));

        return Set.Contains(c.ToString());
    }

    public bool IsMatch(string s)
    {
        return regex.IsMatch(s);
    }

    public string Get(string s)
    {
        if (!IsMatch(s))
            throw new RangeException("{0} '{1}' : does not match '{2}'.", Name, s, Expr);
        return s;
    }

    public char Get(char c)
    {
        if (!Contains(c))
            throw new RangeException("{0} '{1}' : outside '{2}'.", Name, c, Set);
        return c;
    }

    public string GetOpt(string s)
    {
        return String.IsNullOrEmpty(s) ? "" : Get(s);
    }

    public char GetChar(string s)
    {
        return Utility.ToChar(Name, Get(s));
    }

    private static readonly byte[] UcnMulti = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };

    public static char UcnLastChar(string ucn)
    {
        int sum = 0;

        for (int index = 0; index < UcnMulti.Length; index++)
            sum += UcnMulti[index] * (ucn[index] - '0');

        return (char) ((sum % 11 % 10) + '0');
    }

    private Regex regex;

    public class ResetDate : Reset
    {
        public ResetDate() :
            base("Date", "[0-3]?[0-9][.-][0-1]?[0-9][.-][12][0-37-9][0-9][0-9]", Reset.Regex)
        {
        }

        public DateTime Parse(string s)
        {
            string[] DateFormats = { "d.M.yyyy", "d-M-yyyy" };
            return DateTime.ParseExact(s, DateFormats, CultureInfo.CurrentCulture, DateTimeStyles.None);
        }

        public string Format(DateTime stamp)
        {
            return String.Format("{0}.{1}.{2} {3}:{4:D2}:{5:D2}", stamp.Day, stamp.Month, stamp.Year, stamp.Hour,
                stamp.Minute, stamp.Second);
        }

        public new bool IsMatch(string s)
        {
            DateTime date;

            try
            {
                date = Parse(Reset.Date.Get(s));
            }
            catch
            {
                return false;
            }

            return Range.Year.IsMatch(date.Year);
        }

        public new string Get(string s)
        {
            if (!base.IsMatch(s))
                throw new RangeException("Invalid date '{0}'.", s);
            return s;
        }

        public string ValidRange(string start, string final)
        {
            if (!base.IsMatch(start))
                return "Invalid start date";

            if (!base.IsMatch(final))
                return "Invalid final date";

            if (Parse(start) > Parse(final))
                return "Start date > final";

            return null;
        }

        public void CheckRange(string start, string final)
        {
            string error = ValidRange(start, final);

            if (error != null)
                throw new RangeException("{0}, range '{1}'..'{2}'.", error, start, final);
        }
    }
}
