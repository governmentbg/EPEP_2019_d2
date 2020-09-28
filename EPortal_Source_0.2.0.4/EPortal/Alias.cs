using System;
using System.Collections.Generic;
using System.Text;

// TODO? implement AddExact() and get un-cached elements

public class Alias : TGroup
{
    protected enum CacheIndex
    {
        Type,
        Kind,
        Result,
        Subject,
        College,
        ConnectType,
        ConnectKind,
        Composition,
        Hall,
        Country,
        Area,
        Region,
        Involvement,
        Resolution,
        SubpoenaKind,
        CacheCount
    }

    protected Alias(string Name, string nameField)
        : base(Name)
    {
        Add(new TString(nameField, () => name, x => name = x));
    }

    public string name;

    protected void InitCache(Connection conn, CacheIndex cacheIndex)
    {
        SqlSelect select = new SqlSelect(Name, null);
        int index = (int) cacheIndex;

        if (Cache[index] != null)
            throw new RangeException(String.Format("Caching index {0} twice", index));

        Cache[index] = new List<Alias>();
        LoadArray(conn, select, Cache[index]);
    }

    protected static List<Alias>[] Cache;
    static Alias() { Cache = new List<Alias>[(int) CacheIndex.CacheCount]; }

    public static void Init(Connection conn)
    {
        new Type().InitCache(conn, CacheIndex.Type);
        new Kind().InitCache(conn, CacheIndex.Kind);
        new Result().InitCache(conn, CacheIndex.Result);
        new Subject().InitCache(conn, CacheIndex.Subject);
        new College().InitCache(conn, CacheIndex.College);
        new ConnectType().InitCache(conn, CacheIndex.ConnectType);
        new ConnectKind().InitCache(conn, CacheIndex.ConnectKind);
        new Composition().InitCache(conn, CacheIndex.Composition);
        new Hall().InitCache(conn, CacheIndex.Hall);
        new Country().InitCache(conn, CacheIndex.Country);
        new Area().InitCache(conn, CacheIndex.Area);
        new Region().InitCache(conn, CacheIndex.Region);
        new Involvement().InitCache(conn, CacheIndex.Involvement);
        new Resolution().InitCache(conn, CacheIndex.Resolution);
        new SubpoenaKind().InitCache(conn, CacheIndex.SubpoenaKind);
    }
}

public class CharAlias : Alias
{
    protected CharAlias(string Name, string valueField, string nameField)
        : base(Name, nameField)
    {
        Add(new TChar(valueField, () => value, x => value = x));
    }

    protected static CharAlias Seek(CacheIndex cacheIndex, char value)
    {
        foreach (CharAlias alias in Cache[(int) cacheIndex])
            if (alias.value == value)
                return alias;

        throw new GetDataException(cacheIndex.ToString());
    }

    protected char value;
}

public class IntAlias : Alias
{
    protected IntAlias(string Name, string valueField, string nameField)
        : base(Name, nameField)
    {
        Add(new TInt(valueField, () => value, x => value = x));
    }

    protected static IntAlias Seek(CacheIndex cacheIndex, int value)
    {
        foreach (IntAlias alias in Cache[(int) cacheIndex])
            if (alias.value == value)
                return alias;

        throw new GetDataException(cacheIndex.ToString());
    }

    protected int value;
}

public class IntCharAlias : IntAlias
{
    protected IntCharAlias(string Name, string valueField, string basicField, string nameField)
        : base(Name, valueField, nameField)
    {
        Add(new TChar(basicField, () => basic, x => basic = x));
    }

    protected static IntCharAlias Seek(CacheIndex cacheIndex, int value, char basic)
    {
        foreach (IntCharAlias alias in Cache[(int) cacheIndex])
            if (alias.value == value && alias.basic == basic)
                return alias;

        throw new GetDataException(cacheIndex.ToString());
    }

    protected char basic;
}

public class IntIntAlias : IntAlias
{
    protected IntIntAlias(string Name, string valueField, string basicField, string nameField)
        : base(Name, valueField, nameField)
    {
        Add(new TInt(basicField, () => basic, x => basic = x));
    }

    protected static IntIntAlias Seek(CacheIndex cacheIndex, int value, int basic)
    {
        foreach (IntIntAlias alias in Cache[(int) cacheIndex])
            if (alias.value == value && alias.basic == basic)
                return alias;

        throw new GetDataException(cacheIndex.ToString());
    }

    protected int basic;
}

public class StringAlias : Alias
{
    protected StringAlias(string Name, string valueField, string nameField)
        : base(Name, nameField)
    {
        Add(new TString(valueField, () => value, x => value = x));
    }

    protected static StringAlias Seek(CacheIndex cacheIndex, string value)
    {
        foreach (StringAlias alias in Cache[(int) cacheIndex])
            if (alias.value == value)
                return alias;

        throw new GetDataException(cacheIndex.ToString());
    }

    protected string value;
}

public class Type : CharAlias
{
    public Type() : base("T_TYPE", "F_TYPE", "F_NAME")
    {
        Add(new TInt("F_ECODE", () => ecode, x => ecode = x));
    }

    public int ecode;

    public const char
        Request = 'r',
        InReg = 'i',
        Surround = 's',
        Empty = 'Z';

    public const string Lawsuits = "pctadm";

    public static int IncomingWhat(char incomingType)
    {
        switch (incomingType)
        {
            case Request : return What.Request;
            case InReg : return What.InReg;
            case Surround : return What.Surround;
            default : throw new RangeException("Not an incoming type '{0}'.", incomingType);
        }
    }

    public static Type Seek(char type) { return (Type) CharAlias.Seek(CacheIndex.Type, type); }
}

public class Kind : CharAlias
{
    public Kind()
        : base("T_KIND", "F_KIND", "F_NAME")
    {
        Add(new TInt("F_XFER", () => xfer, x => xfer = x));
        Add(new TInt("F_WHAT", () => what, x => what = x));
    }

    public int xfer;
    public int what;

    public const char
        Bulwark = 'М',
        Motives = 'х',
        Decree = '-',
        Order = 'м',
        SettRegi = 'S',
        SettNReg = 'л';

    public static Kind Seek(char kind) { return (Kind) CharAlias.Seek(CacheIndex.Kind, kind); }
}

public class Result : CharAlias
{
    public Result() : base("T_RESULT", "F_RESULT", "F_NAME") { }

    public const char
        Fixed = 'f',
        WithDecree = 'н',
        WithOrder = 'З',
        NonJoinOrder = 'І',
        HoldOrder = 'D';

    public const string Unlinkeds = "АЄ";

    public static Result Seek(char result) { return (Result) CharAlias.Seek(CacheIndex.Result, result); }
}

public class Subject : IntCharAlias
{
    public Subject() : base("T_SUBJECT", "F_SUBJECT", "F_TYPE", "F_NAME") { }

    public static Subject Seek(int subject, char type)
    {
        return (Subject) IntCharAlias.Seek(CacheIndex.Subject, subject, type);
    }

    // TODO? implement the full coding
    public string Code()
    {
        switch (value)
        {
            case Const.Subject800A : return "800А";
            case Const.Subject900A : return "900А";
            default : return (value / Const.SubjectMultiply).ToString();
        }
    }
}

public class College : CharAlias
{
    public College() : base("T_COLLEGE", "F_COLLEGE", "F_NAME") { }

    public static College Seek(char college) { return (College) CharAlias.Seek(CacheIndex.College, college); }
}

public class ConnectType : CharAlias
{
    public ConnectType() : base("T_CONNECT_TYPE", "F_CONNECT_TYPE", "F_NAME") { }

    public static ConnectType Seek(char connectType)
    {
        return (ConnectType) CharAlias.Seek(CacheIndex.ConnectType, connectType);
    }
}

public class ConnectKind : CharAlias
{
    public ConnectKind() : base("T_CONNECT_KIND", "F_CONNECT_KIND", "F_NAME") { }

    public static ConnectKind Seek(char connectKind)
    {
        return (ConnectKind) CharAlias.Seek(CacheIndex.ConnectKind, connectKind);
    }
}

public class Composition : IntCharAlias
{
    public Composition() : base("T_COMPOSITION", "F_COMPOSITION", "F_COLLEGE", "F_NAME") { }

    public static Composition Seek(int composition, char college)
    {
        return (Composition) IntCharAlias.Seek(CacheIndex.Composition, composition, college);
    }
}

public class Hall : IntAlias
{
    public Hall() : base("T_HALL", "F_HALL", "F_NAME") { }

    public static Hall Seek(int hall) { return (Hall) IntAlias.Seek(CacheIndex.Hall, hall); }
}

public class Country : StringAlias
{
    public Country() : base("T_COUNTRY", "F_COUNTRY", "F_NAME") { }

    public static Country Seek(string country) { return (Country) StringAlias.Seek(CacheIndex.Country, country); }
}

public class Area : IntAlias
{
    public Area() : base("T_AREA", "F_AREA", "F_NAME") { }

    public static Area Seek(int area) { return (Area) IntAlias.Seek(CacheIndex.Area, area); }
}

public class Region : IntIntAlias
{
    public Region() : base("T_REGION", "F_REGION", "F_AREA", "F_NAME") { }

    public static Region Seek(int area, int region) { return (Region) IntIntAlias.Seek(CacheIndex.Region, region, area); }
}

public class Involvement : CharAlias
{
    public Involvement()
        : base("T_INVOLVEMENT", "F_INVOLVEMENT", "F_NAME")
    {
        Add(new TInt("F_XFER", () => xfer, x => xfer = x));
    }

    public int xfer;

    public const char Legalese = 'Э';
    public const char Appealer = 'ж';

    public static Involvement Seek(char involvement)
    {
        return (Involvement) CharAlias.Seek(CacheIndex.Involvement, involvement);
    }

    private const string Lawyers = "ИиADL";

    public static int RegisterWhat(char involvement)
    {
        return Lawyers.IndexOf(involvement) == -1 ? What.PersonRegistration : What.LawyerRegistration;
    }

    public static int AssignWhat(char involvement)
    {
        return Lawyers.IndexOf(involvement) == -1 ? What.PersonAssignment : What.LawyerAssignment;
    }
}

public class Resolution : CharAlias
{
    public Resolution() : base("T_RESOLUTION", "F_RESOLUTION", "F_NAME") { }

    public static Resolution Seek(char resolution)
    {
        return (Resolution) CharAlias.Seek(CacheIndex.Resolution, resolution);
    }
}

public class SubpoenaKind : CharAlias
{
    public SubpoenaKind() : base("T_SUBPOENA_KIND", "F_SUBPOENA_KIND", "F_ALIAS") { }

    public static SubpoenaKind Seek(char subpoenaKind)
    {
        return (SubpoenaKind) CharAlias.Seek(CacheIndex.SubpoenaKind, subpoenaKind);
    }
}
