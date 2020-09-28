using System;
using System.Text;

class UcnGroup : TGroup
{
    public UcnGroup(string Name)
        : base(Name)
    {
        Add(new TString("F_UCN", () => ucn, x => ucn = x, TField.ExactKey));
        Add(new TChar("F_UCN_TYPE", () => ucnType, x => ucnType = x, TField.ExactKey));
    }

    public string ucn;
    public char ucnType;
}

class PersonGroup : UcnGroup
{
    public PersonGroup(string Name)
        : base(Name)
    {
        Add(new TString("F_NAME", () => name, x => name = x));
    }

    public string name;
}

class JudicialGroup : PersonGroup
{
    public JudicialGroup(string Name)
        : base(Name)
    {
        Add(new TString("F_BULSTAT", () => bulstat, x => bulstat = x));
    }

    public string bulstat;
}

struct RCDKey
{
    public int no;
    public int year;
    public char type;

    public static readonly RCDKey Empty = new RCDKey();

    static RCDKey()
    {
        Empty.no = 1;
        Empty.year = Const.CacYearMin;
        Empty.type = Type.Empty;
    }
}

class RCDKeyGroup : TGroup
{
    public RCDKeyGroup(string Name)
        : base(Name)
    {
        Add(new TInt("F_NO", () => key.no, x => key.no = x, TField.ExactKey));
        Add(new TInt("F_YEAR", () => key.year, x => key.year = x, TField.ExactKey));
        Add(new TChar("F_TYPE", () => key.type, x => key.type = x, TField.ExactKey));
    }

    public RCDKey key;
}

class RCDKeyContainer : RCDKeyGroup
{
    public RCDKeyContainer(string Name, uint KindDateFlags)
        : base(Name)
    {
        Add(new TChar("F_KIND", () => kind, x => kind = x, KindDateFlags));
        Add(new TInt("F_DATE", () => date, x => date = x, KindDateFlags));
    }

    public char kind;
    public int date;
}

class OriginContainer : RCDKeyContainer
{
    public OriginContainer(string Name)
        : base(Name, 0)
    {
        Add(new TChar("F_COLLEGE", () => college, x => college = x));
    }

    public char college;
}

class DatedContainer : RCDKeyContainer
{
    public DatedContainer(string Name) : base(Name, TField.ExactKey) { }
}

class IndexContainer : DatedContainer
{
    public IndexContainer(string Name)
        : base(Name)
    {
        Add(new TChar("F_RESULT", () => result, x => result = x));
    }

    public char result;
}

class PrimeContainer : RCDKeyContainer
{
    public PrimeContainer(string Name)
        : base(Name, 0)
    {
        Add(new TInt("F_LAWSUIT_NO", () => lawsuit.no, x => lawsuit.no = x));
        Add(new TInt("F_LAWSUIT_YEAR", () => lawsuit.year, x => lawsuit.year = x));
        Add(new TChar("F_LAWSUIT_TYPE", () => lawsuit.type, x => lawsuit.type = x));
    }

    public RCDKey lawsuit;
}

abstract class AnyAddress : UcnGroup
{
    public AnyAddress(string Name)
        : base(Name)
    {
        Add(new TInt("F_ADDRESS", () => address, x => address = x, TField.ExactKey));
    }

    public int address;

    public override void AddFloat(SqlBuilder builder)
    {
        AddFields(builder, "F_UCN, F_UCN_TYPE");
    }

    protected abstract int DefaultAddress { get; }
    public abstract string GetFullText(Connection conn);

    protected string TryFullText(Connection conn, string ucn, char ucnType)
    {
        this.ucn = ucn;
        this.ucnType = ucnType;
        this.address = DefaultAddress;
        return Try(conn, null) ? GetFullText(conn) : null;
    }

    public static string GetFullText(Connection conn, string ucn, char ucnType)
    {
        string fullText = new Address().TryFullText(conn, ucn, ucnType);

        return fullText != null ? fullText : new ForeignAddress().TryFullText(conn, ucn, ucnType);
    }
}

class BaseWind : DatedContainer
{
    protected BaseWind(string table)
        : base(table)
    {
        Add(new TString("F_UCN", () => ucn, x => ucn = x, TField.ExactKey));
        Add(new TChar("F_UCN_TYPE", () => ucnType, x => ucnType = x, TField.ExactKey));
        Add(new TChar("F_INVOLVEMENT", () => involvement, x => involvement = x, TField.ExactKey));
    }

    public string ucn;
    public char ucnType;
    public char involvement;

    public override void AddFloat(SqlBuilder builder)
    {
        AddFields(builder, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
    }
}
