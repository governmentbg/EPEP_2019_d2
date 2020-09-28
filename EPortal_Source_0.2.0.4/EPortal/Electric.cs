using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

static class Electric
{
    public static string Name(EPortSend send, char kind, int date, char electric)
    {
        string name = ElectricName(send, kind, date, electric);

        return name != null ? ShortenName(name) : null;
    }

    [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
    private static extern int GetShortPathName([MarshalAs(UnmanagedType.LPTStr)] string path,
        [MarshalAs(UnmanagedType.LPTStr)] StringBuilder shortPath, int shortPathLength);

    private static string ElectricName(EPortSend send, char kind, int iDate, char electric)
    {
        int electricIndex = Convert.ToInt32(electric.ToString());
        string dir = Const.ElectricDirs[electricIndex];

        if (dir == "")
            return null;

        DateTime date = IntDate.ToDate(iDate);
        StringBuilder name = new StringBuilder();

        if (send.what / What.Factor == What.MotiveHigh)
            kind = Kind.Motives;

        name.AppendFormat("{0}\\{1:D2}{2:X2}{3:X2}{4:D2}\\", dir, send.key.no / (send.key.no <= 99999 ? 1000 : 10000),
            (int) send.key.type, CP1251.IndexOf(kind), send.key.year % 100);

        if (send.key.no <= 99999)
            name.AppendFormat("{0:D3}", send.key.no % 1000);
        else
        {
            int modNo = send.key.no % 10000;

            name.AppendFormat("{0}{1}{2}", 'a' + modNo / 22 / 22, 'a' + (modNo / 22) % 22, 'a' + modNo % 22);
        }

        name.AppendFormat("{0:X1}{1:D2}{2:D2}", date.Month, date.Day, date.Year % 100);

        if (send.subpoenaNo > 0)
            name.AppendFormat("{0:X4}", send.subpoenaNo);

        name.AppendFormat(".{0}", Const.ElectricExts[electricIndex]);
        return name.ToString();
    }

    private static string ShortenName(string name)
    {
        if (Path.GetExtension(name).ToUpper() == ".DOCX")
        {
            FileStream stream = null;
            BinaryReader reader = null;

            try
            {
                stream = File.OpenRead(name);
                reader = new BinaryReader(stream);
                byte[] twocc = reader.ReadBytes(2);

                if (twocc.Length == 2 && (twocc[0] != 0x50 || twocc[1] != 0x4B))
                {
                    StringBuilder shortName = new StringBuilder(Const.CacPathMax);
                    int result = GetShortPathName(name, shortName, shortName.Capacity);

                    if (result > 0 && result <= shortName.Capacity)
                        name = shortName.ToString();
                    else
                        Log.Info("Failed to shorten '{0}'.", name);
                }
            }
            catch
            {
            }
            finally
            {
                Utility.Dispose(reader);
                Utility.Dispose(stream);
            }
        }

        return name;
    }

    private const string CP1251 =   // N.B. ' = 0xFFFF
        // control
        "\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF" +
        "\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF\uFFFF" +
        // ascii-h
        "\u0020\u0021\u0022\u0023\u0024\u0025\u0026\uFFFF\u0028\u0029\u002A\u002B\u002C\u002D\u002E\u002F" +
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
}
