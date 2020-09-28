using System;
using System.Globalization;
using System.IO;
using System.Text;

public static class Utility
{
    public static string Normalize(string s)
    {
        return s != null ? s.Replace('\'', '"') : "";
    }

    public static string Null(string s)
    {
        return s != null ? s : "(null)";
    }

    public static void Dispose(IDisposable id)
    {
        if (id != null)
            id.Dispose();
    }

    public static char ToChar(string name, string s)
    {
        if (s.Length > 1)
            throw new RangeException("'{0}' can not be converted to {1}.", s, name);

        return s.Length > 0 ? s[0] : '\0';
    }

    public static char GetChar(string name, string s)
    {
        return ToChar(name, Normalize(s));
    }

    public static void AddSet(StringBuilder select, string set)
    {
        if (set.Length == 1)
            select.AppendFormat("= {0}", set);
        else
        {
            bool first = true;

            foreach (char c in set)
            {
                select.AppendFormat("{0}'{1}'", first ? "IN (" : ", ", c);
                first = false;
            }
            select.Append(")");
        }
    }

    public static string HexString(byte[] bytes)
    {
        StringBuilder hex = new StringBuilder();

        foreach (byte b in bytes)
            hex.AppendFormat("{0:X2}", b);

        return hex.ToString();
    }

    public static string ToMoney(double value)
    {
        return value.ToString("F2", MoneyFormat);
    }

    public static int ErrorCode(Exception ex)
    {
        return System.Runtime.InteropServices.Marshal.GetHRForException(ex) & 0xFFFF;
    }

    public static DateTime NewTime(int hour, int minute, int second)
    {
        return new DateTime(DateTime.MinValue.Year, DateTime.MinValue.Month, DateTime.MinValue.Day, hour, minute, second);
    }

    public static DateTime NowTime(bool seconds)
    {
        DateTime now = DateTime.Now;
        return NewTime(now.Hour, now.Minute, seconds ? now.Second : 0);
    }

    private const int
        PublicKeyLength = 128,
        KeyHeaderLength = 12;

    public static string PublicKey(string value)
    {
        StringBuilder hex = new StringBuilder();
        value = value.ToUpper();

        foreach (char c in value)
        {
            if (char.IsDigit(c) || (c >= 'A' && c <= 'F'))
                hex.Append(c);
            else if (!char.IsWhiteSpace(c))
                throw new RangeException("Invalid public key character(s).");
        }

        if (hex.Length < (PublicKeyLength + KeyHeaderLength) * 2)
            throw new RangeException("Public key too short.");

        return hex.ToString();
    }

    public const string CP1251 =
        // control
        "\uFFFF\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u0009\u000A\u000B\u000C\u000D\u000E\u000F" +
        "\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F" +
        // ascii-h
        "\u0020\u0021\u0022\u0023\u0024\u0025\u0026\u0027\u0028\u0029\u002A\u002B\u002C\u002D\u002E\u002F" +
        "\u0030\u0031\u0032\u0033\u0034\u0035\u0036\u0037\u0038\u0039\u003A\u003B\u003C\u003D\u003E\u003F" +
        "\u0040\u0041\u0042\u0043\u0044\u0045\u0046\u0047\u0048\u0049\u004A\u004B\u004C\u004D\u004E\u004F" +
        "\u0050\u0051\u0052\u0053\u0054\u0055\u0056\u0057\u0058\u0059\u005A\u005B\u005C\u005D\u005E\u005F" +
        "\u0060\u0061\u0062\u0063\u0064\u0065\u0066\u0067\u0068\u0069\u006A\u006B\u006C\u006D\u006E\u006F" +
        "\u0070\u0071\u0072\u0073\u0074\u0075\u0076\u0077\u0078\u0079\u007A\u007B\u007C\u007D\u007E\u2302" +
        // x11-1251
        "\u0402\u0403\u201A\u0453\u201E\u2026\u2020\u2021\u20AC\u2030\u0409\u2039\u040A\u040C\u040B\u040F" +
        "\u0452\u2018\u2019\u201C\u201D\u2022\u2013\u2014\uFFFF\u2122\u0459\u203A\u045A\u045C\u045B\u045F" +
        // win-1251
        "\u00A0\u040E\u045E\u0408\u00A4\u0490\u00A6\u00A7\u0401\u00A9\u0404\u00AB\u00AC\u00AD\u00AE\u0407" +
        "\u00B0\u00B1\u0406\u0456\u0491\u00B5\u00B6\u00B7\u0451\u2116\u0454\u00BB\u0458\u0405\u0455\u0457" +
        "\u0410\u0411\u0412\u0413\u0414\u0415\u0416\u0417\u0418\u0419\u041A\u041B\u041C\u041D\u041E\u041F" +
        "\u0420\u0421\u0422\u0423\u0424\u0425\u0426\u0427\u0428\u0429\u042A\u042B\u042C\u042D\u042E\u042F" +
        "\u0430\u0431\u0432\u0433\u0434\u0435\u0436\u0437\u0438\u0439\u043A\u043B\u043C\u043D\u043E\u043F" +
        "\u0440\u0441\u0442\u0443\u0444\u0445\u0446\u0447\u0448\u0449\u044A\u044B\u044C\u044D\u044E\u044F" ;

    private static readonly NumberFormatInfo MoneyFormat;

    static Utility()
    {
        CultureInfo culture = new CultureInfo("bg-BG");
        MoneyFormat = (NumberFormatInfo) culture.NumberFormat.Clone();
        MoneyFormat.NumberDecimalSeparator = ".";
    }
}
