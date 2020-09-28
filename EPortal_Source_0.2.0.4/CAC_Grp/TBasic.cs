using System;

public class TAnnounceGroup : TGroup
{
    public TAnnounceGroup(string Table) : base(Table)
    {
        Add(new TInt("F_NO", () => key.no, x => key.no = x));
        Add(new TInt("F_YEAR", () => key.year, x => key.year = x));
        Add(new TChar("F_KIND", () => key.kind, x => key.kind = x));
        Add(new TInt("F_COURT", () => court, x => court = x));
    }

    public TAnnounceKey key;
    public int court;
}

public struct TAnnounceKey
{
    public int no;
    public int year;
    public char kind;
}

public abstract class TAnyAddress : TUCNGroup
{
    public TAnyAddress(string Table) : base(Table)
    {
        Add(new TInt("F_ADDRESS", () => address, x => address = x));
    }

    public int address;

    protected override string ExactFields()
    {
        return FloatFields() + ", F_ADDRESS";
    }

    protected override string FloatFields()
    {
        return base.ExactFields();
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
        string fullText = (new TAddress()).TryFullText(conn, ucn, ucnType);

        return fullText != null ? fullText : (new TForeignAddress()).TryFullText(conn, ucn, ucnType);
    }
}

public class TBaseComposition : TIntCharAlias
{
    protected TBaseComposition() : base("T_COMPOSITION")
    {
        Add(valueField = new TInt("F_COMPOSITION", () => composition, x => composition = x));
        Add(groupField = new TChar("F_COLLEGE", () => college, x => college = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
    }

    public int composition;
    public char college;
    public string name;
}

public class TBaseWind : TUCNGroup
{
    protected TBaseWind(string table)
        : base(table)
    {
        Add(new TInt("F_NO", () => key.no, x => key.no = x));
        Add(new TInt("F_YEAR", () => key.year, x => key.year = x));
        Add(new TChar("F_TYPE", () => key.type, x => key.type = x));
        Add(new TChar("F_KIND", () => kind, x => kind = x));
        Add(new TInt("F_DATE", () => date, x => date = x));
        Add(new TChar("F_INVOLVEMENT", () => involvement, x => involvement = x));
    }

    public TRCDKey key;
    public char kind;
    public int date;
    public char involvement;

    protected override string ExactFields()
    {
        return "F_UCN, F_UCN_TYPE, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_INVOLVEMENT";
    }

    protected override string FloatFields()
    {
        return "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE";
    }

    public string PrintNames(Connection conn)
    {
        string names = "";

        if (TUCNType.Citizens.IndexOf(ucnType) != -1)
        {
            TCitizen citizen = new TCitizen();

            citizen.ucn = ucn;
            citizen.ucnType = ucnType;

            if (citizen.Try(conn, null))
            {
                names = String.Format("{0} {1} {2} {3}", citizen.name, citizen.reName, citizen.family, citizen.reFamily);
                names = names.Replace("  ", " ");
            }
        }
        else if (TUCNType.Firms.IndexOf(ucnType) != -1)
        {
            TFirm firm = new TFirm();

            firm.ucn = ucn;
            firm.ucnType = ucnType;

            if (firm.Try(conn, null))
                names = firm.name;
        }

        return names;
    }
}

public class TDatedContainer : TRCDKeyContainer
{
    public TDatedContainer(string Table) : base(Table)
    {
    }

    protected override string ExactFields()
    {
        return FloatFields() + ", F_KIND, F_DATE";
    }

    protected override string FloatFields()
    {
        return base.ExactFields();
    }
}

public class TIndexContainer : TDatedContainer
{
    public TIndexContainer(string Table) : base(Table)
    {
        Add(new TChar("F_RESULT", () => result, x => result = x));
    }

    public char result;
}

public class TJudicialGroup : TPersonGroup
{
    public TJudicialGroup(string Table) : base(Table)
    {
        Add(new TString("F_BULSTAT", () => bulstat, x => bulstat = x));
    }

    public string bulstat;
}

public class TPersonGroup : TUCNGroup
{
    public TPersonGroup(string Table) : base(Table)
    {
        Add(new TString("F_NAME", () => name, x => name = x));
    }

    public string name;
}

public class TPrimeContainer : TRCDKeyContainer
{
    public TPrimeContainer(string Table) : base(Table)
    {
        Add(new TInt("F_LAWSUIT_NO", () => lawsuit.no, x => lawsuit.no = x));
        Add(new TInt("F_LAWSUIT_YEAR", () => lawsuit.year, x => lawsuit.year = x));
        Add(new TChar("F_LAWSUIT_TYPE", () => lawsuit.type, x => lawsuit.type = x));
    }

    public TRCDKey lawsuit;

    protected override string FloatFields()
    {
        return "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE";
    }
}

public struct TRCDKey
{
    public int no;
    public int year;
    public char type;
}

public class TUCNGroup : TGroup
{
    public TUCNGroup(string Table) : base(Table)
    {
        Add(new TString("F_UCN", () => ucn, x => ucn = x));
        Add(new TChar("F_UCN_TYPE", () => ucnType, x => ucnType = x));
    }

    protected override string ExactFields()
    {
        return "F_UCN, F_UCN_TYPE";
    }

    public string ucn;
    public char ucnType;
}

public class TOriginContainer : TRCDKeyContainer
{
    public TOriginContainer(string Table) : base(Table)
    {
        Add(new TChar("F_COLLEGE", () => college, x => college = x));
        Add(new TString("F_EISPP_NMR", () => eisppNmr, x => eisppNmr = x));
    }

    public char college;
    public string eisppNmr;
}

public class TRCDKeyContainer : TRCDKeyGroup
{
    public TRCDKeyContainer(string Table) : base(Table)
    {
        Add(new TChar("F_KIND", () => kind, x => kind = x));
        Add(new TInt("F_DATE", () => date, x => date = x));
    }

    public char kind;
    public int date;
}

public class TRCDKeyGroup : TGroup
{
    public TRCDKeyGroup(string Table) : base(Table)
    {
        Add(new TInt("F_NO", () => key.no, x => key.no = x));
        Add(new TInt("F_YEAR", () => key.year, x => key.year = x));
        Add(new TChar("F_TYPE", () => key.type, x => key.type = x));
    }

    protected override string ExactFields()
    {
        return "F_NO, F_YEAR, F_TYPE";
    }

    public TRCDKey key;
}
