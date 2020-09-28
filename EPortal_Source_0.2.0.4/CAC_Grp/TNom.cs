using System;

public class TArea : TIntAlias
{
    public TArea() : base("T_AREA")
    {
        Add(valueField = new TInt("F_AREA", () => area, x => area = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
    }

    public int area;
    public string name;

    public static TArea Seek(int area) { return (TArea) Seek(TAliasCache.AreaCache, area); }
}

public class TCollege : TCharAlias
{
    public TCollege() : base("T_COLLEGE")
    {
        Add(valueField = new TChar("F_COLLEGE", () => college, x => college = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
    }

    public char college;
    public string name;

    public static TCollege Seek(char college) { return (TCollege) Seek(TAliasCache.CollegeCache, college); }
}

public class TComposition : TBaseComposition
{
    public TComposition()
    {
    }

    public override void AddExact(SqlCriteriaBuilder builder)
    {
        AddFloat(builder);
        builder.AddInt("F_COMPOSITION", composition);
    }

    public override void AddFloat(SqlCriteriaBuilder builder)
    {
        builder.AddChar("F_COLLEGE", DefaultCollege);
    }

    public static TComposition Seek(int composition, char college)
    {
        return (TComposition) Seek(TAliasCache.CompositionCache, composition, college);
    }

    public const char DefaultCollege = 'p';
}

public class TConnectKind : TCharAlias
{
    public TConnectKind() : base("T_CONNECT_KIND")
    {
        Add(valueField = new TChar("F_CONNECT_KIND", () => connectKind, x => connectKind = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TInt("F_XFER", () => xfer, x => xfer = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
    }

    public char connectKind;
    public string name;
    public int xfer;
    public string alias;

    public static TConnectKind Seek(char connectKind)
    {
        return (TConnectKind) Seek(TAliasCache.ConnectKindCache, connectKind);
    }
}

public class TConfigName : TStringAlias  // no caching
{
    public TConfigName() : base("T_CONFIG_NAME")
    {
        Add(valueField = new TString("F_CONFIG", () => config, x => config = x));
        nameField = valueField;
        Add(new TInt("F_NO", () => no, x => no = x));
        Add(new TInt("F_DATE", () => date, x => date = x));
        Add(new TString("F_VERSION", () => version, x => version = x));
    }

    public string config;
    public int no;
    public int date;
    public string version;

    public TConfigName Seek(Connection conn, string config)
    {
        TConfigName configName = new TConfigName();

        configName.config = config;
        configName.Get(conn, null);
        return configName;
    }

    public const string Global = "*GLOBAL";
}

public class TConnectType : TCharAlias
{
    public TConnectType() : base("T_CONNECT_TYPE")
    {
        Add(valueField = new TChar("F_CONNECT_TYPE", () => connectType, x => connectType = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
        Add(new TString("F_CONNECT_KINDS", () => connectKinds, x => connectKinds = x));
        Add(new TChar("F_SENDER_TYPE", () => senderType, x => senderType = x));
        Add(new TInt("F_XFER", () => xfer, x => xfer = x));
        Add(new TInt("F_EISPP", () => eispp, x => eispp = x));
    }

    public char connectType;
    public string name;
    public string alias;
    public string connectKinds;
    public char senderType;
    public int xfer;
    public int eispp;

    public static TConnectType Seek(char connectType)
    {
        return (TConnectType) Seek(TAliasCache.ConnectTypeCache, connectType);
    }
}

public class TCountry : TStringAlias
{
    public TCountry() : base("T_COUNTRY")
    {
        Add(valueField = new TString("F_COUNTRY", () => country, x => country = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TInt("F_EISPP", () => eispp, x => eispp = x));
    }

    public string country;
    public string name;
    public int eispp;

    public static TCountry Seek(string country) { return (TCountry) Seek(TAliasCache.CountryCache, country); }
}

public class TCourt : TIntAlias
{
    public TCourt() : base("T_COURT")
    {
        Add(valueField = new TInt("F_COURT", () => court, x => court = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TChar("F_COURT_TYPE", () => courtType, x => courtType = x));
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(new TInt("F_COURT_AREA", () => courtArea, x => courtArea = x));
        Add(new TInt("F_EISPP", () => eispp, x => eispp = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
    }

    public int court;
    public string name;
    public char courtType;
    public int uclp;
    public int courtArea;
    public int eispp;
    public int flags;

    public static TCourt Seek(int court) { return (TCourt) Seek(TAliasCache.CourtCache, court); }

    public const uint XferMotives = 0x01;
}

public class TFineComposition : TBaseComposition
{
    public TFineComposition()
    {
        Add(new TString("F_JUDGE", () => judge, x => judge = x));
        Add(new TString("F_SECRETAR", () => secretar, x => secretar = x));
        Add(new TString("F_JUDGE_X", () => judgeX, x => judgeX = x));
        Add(new TString("F_REJUDGE_X", () => reJudgeX, x => reJudgeX = x));
        Add(new TString("F_PRESIDENT", () => president, x => president = x));
    }

    public string judge;
    public string secretar;
    public string judgeX;
    public string reJudgeX;
    public string president;
}

public class THall : TIntAlias
{
    public THall() : base("T_HALL")
    {
        Add(valueField = new TInt("F_HALL", () => hall, x => hall = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
        Add(new TString("F_ADDRESS", () => address, x => address = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
    }

    public int hall;
    public string name;
    public string alias;
    public string address;
    public int flags;

    public static THall Seek(int hall) { return (THall) Seek(TAliasCache.HallCache, hall); }
}

public class TInvolvement : TCharAlias
{
    public TInvolvement() : base("T_INVOLVEMENT")
    {
        Add(valueField = new TChar("F_INVOLVEMENT", () => involvement, x => involvement = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TInt("F_XFER", () => xfer, x => xfer = x));
    }

    public char involvement;
    public string name;
    public int xfer;

    public static TInvolvement Seek(char involvement)
    {
        return (TInvolvement) Seek(TAliasCache.InvolvementCache, involvement);
    }

    public const char
        Legalese = 'İ',
        Parent   = 'R',
        Appealer = 'æ';

    public const string
        LegalSpeakers = "BUYRğ",
        Lawyers = "ÈèADL";
}

public class TKind : TCharAlias
{
    public TKind() : base("T_KIND")
    {
        Add(valueField = new TChar("F_KIND", () => kind, x => kind = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TChar("F_TYPE", () => type, x => type = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
        Add(new TInt("F_XFER", () => xfer, x => xfer = x));
        Add(new TInt("F_EISPP", () => eispp, x => eispp = x));
        Add(new TInt("F_WHAT", () => what, x => what = x));
    }

    public char kind;
    public string name;
    public char type;
    public string alias;
    public int xfer;
    public int eispp;
    public int what;

    public static TKind Seek(char kind) { return (TKind) Seek(TAliasCache.KindCache, kind); }

    public const char
        Bulwark = 'Ì',
        Motives = 'õ';
}

public class TKnownPerson : TUCNGroup
{
    public TKnownPerson() : base("T_KNOWN_PERSON")
    {
        Add(new TChar("F_INVOLVEMENT", () => involvement, x => involvement = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TInt("F_CSJID", () => csjid, x => csjid = x));
        Add(new TString("F_PERSONAL_NO", () => personalNo, x => personalNo = x));
        Add(new TDateTime("F_BIRTH_DATE", () => birthDate, x => birthDate = x));
    }

    protected override string ExactFields()
    {
        return "F_UCN, F_UCN_TYPE, " + FloatFields();
    }

    protected override string FloatFields()
    {
        return "F_INVOLVEMENT";
    }

    public static TKnownPerson Seek(Connection conn, string ucn, char ucnType, char involvement)
    {
        TKnownPerson person = new TKnownPerson();

        person.ucn = ucn;
        person.ucnType = ucnType;
        person.involvement = involvement;
        person.Get(conn, null);
        return person;
    }

    public char involvement;
    public int flags;
    public int csjid;
    public string personalNo;
    public DateTime birthDate;
}

public class TPost : TCharAlias
{
    public TPost() : base("T_POST")
    {
        Add(valueField = new TChar("F_POST", () => post, x => post = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
    }

    public char post;
    public string name;

    public static TPost Seek(char post) { return (TPost) Seek(TAliasCache.PostCache, post); }

    public const char Deliverer = 'w';
}

public class TReceivedStatus : TCharAlias
{
    public TReceivedStatus() : base("T_RECEIVED_STATUS")
    {
        Add(valueField = new TChar("F_RECEIVED_STATUS", () => receivedStatus, x => receivedStatus = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
        Add(new TChar("F_CLERK", () => clerk, x => clerk = x));
        Add(new TChar("F_DELIVERER", () => deliverer, x => deliverer = x));
        Add(new TChar("F_ALLOW_DATE_RECEIVED", () => allowDateReceived, x => allowDateReceived = x));
        Add(new TChar("F_ALLOW_DATE_RETURNED", () => allowDateReturned, x => allowDateReturned = x));
        Add(new TChar("F_ALLOW_SCHEDULE", () => allowSchedule, x => allowSchedule = x));
        Add(new TChar("F_SHOW_SUBPOENA_DATE", () => showSubpoenaDate, x => showSubpoenaDate = x));
    }

    public char receivedStatus;
    public string name;
    public string alias;
    public char clerk;
    public char deliverer;
    public char allowDateReceived;
    public char allowDateReturned;
    public char allowSchedule;
    public char showSubpoenaDate;

    public static TReceivedStatus Seek(char kind)
    {
        return (TReceivedStatus) Seek(TAliasCache.ReceivedStatusCache, kind);
    }

    public const char
        None        = 'n',
        Sent        = 's',
        ForDeliver  = 'l',
        NotReceived = 'f',
        Received    = 'r',
        Incoming    = 'd',
        Created     = 'c',
        FortySeven  = 'v';

    public const string
        Initials    = "sl" + "cd",
        InitMobiles = "sl" + "d",
        DelivBasics = "rf",
        Delivereds  = "rf" + "vwxy";
}

public class TRegion : TIntIntAlias
{
    public TRegion() : base("T_REGION")
    {
        Add(valueField = new TInt("F_REGION", () => region, x => region = x));
        Add(groupField = new TInt("F_AREA", () => area, x => area = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
    }

    public int region;
    public int area;
    public string name;

    public static TRegion Seek(int area, int region) { return (TRegion) Seek(TAliasCache.RegionCache, region, area); }
}

public class TResolution : TCharAlias
{
    public TResolution() : base("T_RESOLUTION")
    {
        Add(valueField = new TChar("F_RESOLUTION", () => resolution, x => resolution = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
    }

    public char resolution;
    public string name;

    public static TResolution Seek(char resolution) { return (TResolution) Seek(TAliasCache.ResolutionCache, resolution); }
}

public class TResult : TCharAlias
{
    public TResult() : base("T_RESULT")
    {
        Add(valueField = new TChar("F_RESULT", () => result, x => result = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TInt("F_XFER", () => xfer, x => xfer = x));
    }

    public char result;
    public string name;
    public int xfer;

    public static TResult Seek(char result) { return (TResult) Seek(TAliasCache.ResultCache, result); }

    public const char Fixed = 'f';
}

public class TStreet : TIntIntAlias
{
    public TStreet() : base("T_STREET")
    {
        Add(groupField = new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(valueField = new TInt("F_STREET", () => street, x => street = x));
        Add(new TString("F_NAME", () => name, x => name = x));
    }

    public int uclp;
    public int street;
    public string name;

    public static TStreet Seek(Connection conn, int uclp, int street)
    {
        TStreet alias = new TStreet();

        alias.uclp = uclp;
        alias.street = street;
        alias.Get(conn, null);
        return alias;
    }
}

public class TSubject : TIntCharAlias
{
    public TSubject() : base("T_SUBJECT")
    {
        Add(valueField = new TInt("F_SUBJECT", () => subject, x => subject = x));
        Add(groupField = new TChar("F_TYPE", () => type, x => type = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
    }

    public override void AddFloat(SqlCriteriaBuilder builder) { }

    protected override string ExactFields()
    {
        return "F_SUBJECT, F_TYPE";
    }
    
    protected override string FloatFields()
    {
        throw new NotImplementedException(Table + ":FloatFields");
    }

    public int subject;
    public char type;
    public string name;
    public string alias;

    public static TSubject Seek(int subject, char type) { return (TSubject) Seek(TAliasCache.SubjectCache, subject, type); }
}

public class TSubpoenaKind : TCharAlias
{
    public TSubpoenaKind() : base("T_SUBPOENA_KIND")
    {
        Add(valueField = new TChar("F_SUBPOENA_KIND", () => subpoenaKind, x => subpoenaKind = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
        Add(new TChar("F_GENERIC_KIND", () => genericKind, x => genericKind = x));
        Add(new TString("F_INFO", () => info, x => info = x));
    }

    public char subpoenaKind;
    public string name;
    public string alias;
    public char genericKind;
    public string info;

    public static TSubpoenaKind Seek(char subpoenaKind)
    {
        return (TSubpoenaKind) Seek(TAliasCache.SubpoenaKindCache, subpoenaKind);
    }

    public const string Stickers = "ÕÖ";
}

public class TType : TCharAlias
{
    public TType() : base("T_TYPE")
    {
        Add(valueField = new TChar("F_TYPE", () => type, x => type = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TString("F_ALIAS", () => alias, x => alias = x));
        Add(new TInt("F_ECODE", () => ecode, x => ecode = x));
    }

    public char type;
    public string name;
    public string alias;
    public int ecode;

    public static TType Seek(char type) { return (TType) Seek(TAliasCache.TypeCache, type); }

    public const char
        Request = 'r',
        InReg = 'i',
        Surround = 's',
        OutReg = 'o';

    public const string Lawsuits = "pctadm";
}

public class TUCLP : TIntAlias
{
    public TUCLP() : base("T_UCLP")
    {
        Add(valueField = new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(new TInt("F_AREA", () => area, x => area = x));
        Add(new TInt("F_REGION", () => region, x => region = x));
        Add(new TString("F_NAME", () => name, x => name = x));
    }

    protected override string FloatFields()
    {
        return "F_AREA, F_REGION";
    }

    public int uclp;
    public int area;
    public int region;
    public string name;

    public static TUCLP Seek(Connection conn, int uclp)
    {
        TUCLP alias = new TUCLP();

        alias.uclp = uclp;
        alias.Get(conn, null);
        return alias;
    }
}

public class TUCNType : TCharAlias
{
    public TUCNType() : base("T_UCN_TYPE")
    {
        Add(valueField = new TChar("F_UCN_TYPE", () => ucnType, x => ucnType = x));
        Add(nameField = new TString("F_NAME", () => name, x => name = x));
        Add(new TChar("F_UXN_TYPE", () => uxnType, x => uxnType = x));
    }

    public char ucnType;
    public string name;
    public char uxnType;

    public static TUCNType Seek(char ucnType) { return (TUCNType) Seek(TAliasCache.UCNTypeCache, ucnType); }

    public const char
        Self = 'b',
        CitizenUCN = 'c';

    public const string
        Citizens = "cC",
        Firms = "fF";
}

public class TUser : TStringAlias
{
    public TUser() : base("T_USER")
    {
        Add(valueField = new TString("F_UCN", () => ucn, x => ucn = x));
        Add(new TString("F_LOGIN", () => login, x => login = x));
        Add(new TChar("F_POST", () => post, x => post = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TString("F_MD5SUM", () => md5sum, x => md5sum = x));
    }

    public string ucn;
    public string login;
    public char post;
    public int flags;
    public string md5sum;

    public const int Active = 1;
}
