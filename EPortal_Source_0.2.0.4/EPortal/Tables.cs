using System;
using System.Text;

class KnownPerson : UcnGroup
{
    public KnownPerson()
        : base("T_KNOWN_PERSON")
    {
        Add(new TChar("F_INVOLVEMENT", () => involvement, x => involvement = x));
        Add(new TString("F_PERSONAL_NO", () => personalNo, x => personalNo = x));
        Add(new TDateTime("F_BIRTH_DATE", () => birthDate, x => birthDate = x));
    }

    public static KnownPerson Seek(Connection conn, string ucn, char ucnType, char involvement)
    {
        KnownPerson person = new KnownPerson();

        person.ucn = ucn;
        person.ucnType = ucnType;
        person.involvement = involvement;
        person.Get(conn, null);
        return person;
    }

    public char involvement;
    public string personalNo;
    public DateTime birthDate;
}

class Sender : PersonGroup
{
    public Sender()
        : base("T_SENDER")
    {
        Add(new TInt("F_COURT", () => court, x => court = x));
    }

    public static Sender Seek(Connection conn, string ucn, char ucnType)
    {
        Sender sender = new Sender();

        sender.ucn = ucn;
        sender.ucnType = ucnType;
        sender.Get(conn, null);
        return sender;
    }

    public int court;
}

class Court : TGroup
{
    public Court()
        : base("T_COURT")
    {
        Add(new TInt("F_COURT", () => court, x => court = x, TField.ExactKey));
        Add(new TChar("F_COURT_TYPE", () => courtType, x => courtType = x));
        Add(new TString("F_NAME", () => name, x => name = x));
    }

    public int court;
    public char courtType;
    public string name;
}

class ConfigData : TGroup
{
    public ConfigData()
        : base("T_CONFIG_DATA")
    {
        Add(new TString("F_CONFIG", () => config, x => config = x, TField.ExactKey));
        Add(new TString("F_SECTION", () => section, x => section = x, TField.ExactKey));
        Add(new TString("F_NAME", () => name, x => name = x, TField.ExactKey));
        Add(new TString("F_VALUE", () => value, x => value = x));
    }

    public string config;
    public string section;
    public string value;
    public string name;

    public string GetValue(Connection conn, string name)
    {
        this.name = name;
        return Try(conn, "F_VALUE") ? value : "";
    }

    public string GetExtension(Connection conn, string name)
    {
        string backup = value;

        GetValue(conn, name);

        if (value != "")
        {
            value = value.Split(' ')[0];

            int lessIndex = value.IndexOf('<');

            if (lessIndex >= 0)
            {
                if (value[lessIndex + 2] == '>')
                    value = value.Substring(0, lessIndex);
                else
                    value = value.Substring(0, lessIndex) + value[lessIndex + 1];
            }
        }
        else
            value = backup;

        return value;
    }
}

class Request : OriginContainer
{
    public Request() : base("T_REQUEST") { }
}

class Lawsuit : OriginContainer
{
    public Lawsuit() : base("T_LAWSUIT")
    {
        Add(new TInt("F_SUBJECT", () => subject, x => subject = x));
        Add(new TInt("F_SOURCE_NO", () => source.no, x => source.no = x));
        Add(new TInt("F_SOURCE_YEAR", () => source.year, x => source.year = x));
        Add(new TChar("F_SOURCE_TYPE", () => source.type, x => source.type = x));
        Add(new TString("F_JUDGE", () => judge, x => judge = x));
        Add(new TInt("F_COMPOSITION", () => composition, x => composition = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
    }

    public int subject;
    public RCDKey source;
    public string judge;
    public int composition;
    public int flags;

    public static int FourteenValue(int ecode, char kind)
    {
        int instance = Kind.Seek(kind).what % What.Factor;
        int index = Array.FindIndex(FourteenMappings, m => m.ecode == ecode && m.instance == instance);

        if (index == -1)
            throw new GetDataException("T_FOURTEEN_MAPPING");

        return FourteenMappings[index].value;
    }

    private struct FourteenMapping
    {
        public FourteenMapping(int value, int instance, int ecode)
        {
            this.value = value;
            this.instance = instance;
            this.ecode = ecode;
        }

        public int value;
        public int instance;
        public int ecode;
    }

    public static class ECode
    {
        public const int
            Punishment = 1,
            Citizen = 2,
            Admin = 3,
            Firm = 4,
            Trade = 5;
    }

    public const int Limited = 0x10;

    private static readonly FourteenMapping[] FourteenMappings = new FourteenMapping[]
    {
        new FourteenMapping(1, 1, ECode.Citizen),
        new FourteenMapping(2, 1, ECode.Punishment),
        new FourteenMapping(5, 2, ECode.Citizen),
        new FourteenMapping(6, 2, ECode.Punishment),
        new FourteenMapping(7, 1, ECode.Admin),
        new FourteenMapping(7, 2, ECode.Admin),
        new FourteenMapping(8, 1, ECode.Firm),
        new FourteenMapping(9, 1, ECode.Trade),
        new FourteenMapping(9, 2, ECode.Trade),
        new FourteenMapping(10, 2, ECode.Firm)
    };
}

class SideWind : BaseWind
{
    public SideWind()
        : base("T_SIDE_WIND")
    {
        Add(new TChar("F_STATE", () => state, x => state = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
    }

    public char state;
    public int flags;

    public bool Appealer()
    {
        return involvement == Involvement.Appealer || Const.StateAppealers.IndexOf(state) != -1;
    }
}

class Citizen : PersonGroup
{
    public Citizen()
        : base("T_CITIZEN")
    {
        Add(new TString("F_RENAME", () => reName, x => reName = x));
        Add(new TString("F_FAMILY", () => family, x => family = x));
        Add(new TString("F_REFAMILY", () => reFamily, x => reFamily = x));
    }

    public string reName;
    public string family;
    public string reFamily;

    public static Citizen GetForEP(Connection conn, string ucn, char ucnType)
    {
        Citizen citizen = new Citizen();

        citizen.ucn = ucn;
        citizen.ucnType = ucnType;
        citizen.Get(conn, null);

        if (citizen.ucnType != UcnType.CitizenUcn)
            citizen.ucn = null;

        if (citizen.reName == "")
            citizen.reName = null;

        if (citizen.reFamily == "")
            citizen.reFamily = null;

        return citizen;
    }

    public string GetFullName()
    {
        StringBuilder fullName = new StringBuilder(name);

        if (!String.IsNullOrEmpty(reName))
            fullName.AppendFormat(" {0}", reName);

        fullName.AppendFormat(" {0}", family);

        if (!String.IsNullOrEmpty(reFamily))
            fullName.AppendFormat(" {0}", reFamily);

        return fullName.ToString();
    }

    public static string GetFullName(Connection conn, string ucn, char ucnType)
    {
        Citizen citizen = new Citizen();

        citizen.ucn = ucn;
        citizen.ucnType = ucnType;
        citizen.Get(conn, null);
        return citizen.GetFullName();
    }
}

class Firm : JudicialGroup
{
    public Firm()
        : base("T_FIRM")
    {
        Add(new TString("F_UFN", () => ufn, x => ufn = x));
    }

    public string ufn;

    public static Firm GetForEP(Connection conn, string ucn, char ucnType)
    {
        Firm firm = new Firm();

        firm.ucn = ucn;
        firm.ucnType = ucnType;
        firm.Get(conn, null);

        if (firm.bulstat == "")
            firm.bulstat = null;

        return firm;
    }
}

class Address : AnyAddress
{
    public Address()
        : base("T_ADDRESS")
    {
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(new TInt("F_STREET", () => street, x => street = x));
        Add(new TString("F_ADR_NO", () => adrNo, x => adrNo = x));
        Add(new TString("F_SUB_NO", () => subNo, x => subNo = x));
        Add(new TString("F_ENTRANCE", () => entrance, x => entrance = x));
        Add(new TString("F_FLOOR", () => floor, x => floor = x));
        Add(new TString("F_APARTMENT", () => apartment, x => apartment = x));
        Add(new TString("F_BLOCK_NO", () => blockNo, x => blockNo = x));
        Add(new TString("F_EMAIL", () => email, x => email = x));
    }

    public int uclp;
    public int street;
    public string adrNo;
    public string subNo;
    public string entrance;
    public string floor;
    public string apartment;
    public string blockNo;
    public string email;

    private struct AddressPart
    {
        public AddressPart(string prefix, string text)
        {
            this.prefix = prefix;
            this.text = text;
        }

        public string prefix;
        public string text;
    }

    protected override int DefaultAddress { get { return 1; } }

    protected static string FilterPrefixes(string text, string[] prefixes)
    {
        foreach (string prefix in prefixes)
        {
            int prefLen = prefix.Length;

            if (text.Length > prefLen && text.StartsWith(prefix) && !Char.IsLetter(text[prefLen]))
            {
                int i;

                for (i = prefLen; i < text.Length && (Char.IsWhiteSpace(text[i]) || text[i] == '.'); i++);
                text = text.Substring(i);
                break;
            }
        }

        return text;
    }

    protected static readonly string[] UclpPrefixes = { "ÃÐÀÄ", "ÃÐ", "ÑÅËÎ", "Ñ", "ÌÅÑÒÍÎÑÒ", "Ì" };

    public override string GetFullText(Connection conn)
    {
        UCLP uclp = UCLP.Seek(conn, this.uclp);
        //string filtered = FilterPrefixes(uclp.name, UclpPrefixes);
        Area area = Area.Seek(uclp.area);
        Region region = Region.Seek(uclp.area, uclp.region);
        StringBuilder fullText = new StringBuilder();

        //if (area.name != filtered || region.name != filtered)
            fullText.AppendFormat("Îáëàñò {0}, Îáùèíà {1}, ", area.name, region.name);

        fullText.Append(uclp.name);

        if (street != 0)
            fullText.AppendFormat(", {0}", Street.Seek(conn, this.uclp, street).name);

        AddressPart[] parts = new AddressPart[]
        {
            new AddressPart("No ", adrNo),
            new AddressPart("Áëîê ", blockNo),
            new AddressPart("Ïîäí.", subNo),
            new AddressPart("Âõ.", entrance),
            new AddressPart("Åò.", floor),
            new AddressPart("Àï.", apartment)
        };

        foreach (AddressPart part in parts)
            if (part.text != "")
                fullText.AppendFormat(", {0}{1}", part.prefix, part.text);

        return fullText.ToString();
    }
}

class ForeignAddress : AnyAddress
{
    public ForeignAddress()
        : base("T_FOREIGN_ADDRESS")
    {
        Add(new TString("F_COUNTRY", () => country, x => country = x));
        Add(new TString("F_FOREIGN", () => foreign, x => foreign = x));
    }

    public string country;
    public string foreign;

    protected override int DefaultAddress { get { return -1; } }

    public override string GetFullText(Connection conn)
    {
        StringBuilder fullText = new StringBuilder(Country.Seek(country).name);

        if (foreign != "")
            fullText.AppendFormat(" {0}", foreign);

        return fullText.ToString();
    }
}

class UCLP : TGroup
{
    public UCLP()
        : base("T_UCLP")
    {
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x, TField.ExactKey));
        Add(new TInt("F_AREA", () => area, x => area = x));
        Add(new TInt("F_REGION", () => region, x => region = x));
        Add(new TString("F_NAME", () => name, x => name = x));
    }

    public int uclp;
    public int area;
    public int region;
    public string name;

    public static UCLP Seek(Connection conn, int uclp)
    {
        UCLP alias = new UCLP();

        alias.uclp = uclp;
        alias.Get(conn, null);
        return alias;
    }
}

class Street : TGroup
{
    public Street()
        : base("T_STREET")
    {
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x, TField.ExactKey));
        Add(new TInt("F_STREET", () => street, x => street = x, TField.ExactKey));
        Add(new TString("F_NAME", () => name, x => name = x));
    }

    public int uclp;
    public int street;
    public string name;

    public static Street Seek(Connection conn, int uclp, int street)
    {
        Street alias = new Street();

        alias.uclp = uclp;
        alias.street = street;
        alias.Get(conn, null);
        return alias;
    }
}

class Session : DatedContainer
{
    public Session()
        : base("T_SESSION")
    {
        Add(new TChar("F_RESULT", () => result, x => result = x));
        Add(new TString("F_SECRETAR", () => secretar, x => secretar = x));
        Add(new TString("F_PROSECUTOR", () => prosecutor, x => prosecutor = x));
        Add(new TString("F_PRESIDENT", () => president, x => president = x));
        Add(new TString("F_REJUDGE_X", () => reJudgeX, x => reJudgeX = x));
        Add(new TString("F_JUDGE_X", () => judgeX, x => judgeX = x));
        Add(new TString("F_JUDGE", () => judge, x => judge = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TInt("F_FORCE_DATE", () => forceDate, x => forceDate = x));
        Add(new TInt("F_EVENT_NO", () => eventNo, x => eventNo = x));
        Add(new TInt("F_HALL", () => hall, x => hall = x));
    }

    public char result;
    public string secretar;
    public string prosecutor;
    public string president;
    public string reJudgeX;
    public string judgeX;
    public string judge;
    public int flags;
    public int forceDate;
    public int eventNo;
    public int hall;

    public const uint Private = 0x20;
}

class SessEndoc : Session
{
    // 2018-10-08: must be in sync with CAC, assuming IsEPortSessEndoc() == true
    public static bool IsEPortSession(char tKind) { return Kind.Seek(tKind).what == What.OpenSession; }
    public bool IsEPortSession() { return IsEPortSession(kind); }
}

class Decision : DatedContainer
{
    public Decision()
        : base("T_DECISION")
    {
        Add(new TInt("F_FROM_DATE", () => fromDate, x => fromDate = x));
        Add(new TChar("F_SESSION_KIND", () => sessionKind, x => sessionKind = x));
        Add(new TInt("F_RETURNED", () => returned, x => returned = x));
        Add(new TInt("F_FORCE_DATE", () => forceDate, x => forceDate = x));
        Add(new TInt("F_EVENT_NO", () => eventNo, x => eventNo = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TString("F_MOTIVES", () => motives, x => motives = x));
    }

    public int fromDate;
    public char sessionKind;
    public int returned;
    public int forceDate;
    public int eventNo;
    public int flags;
    public string motives;

    public const uint Motives = 0x10;
    public const uint Public = 0x08;
}

class InReg : PrimeContainer
{
    public InReg()
        : base("T_INREG")
    {
        Add(new TChar("F_DECISION_KIND", () => decisionKind, x => decisionKind = x));
        Add(new TInt("F_DECISION_DATE", () => decisionDate, x => decisionDate = x));
        Add(new TChar("F_SESSION_KIND", () => sessionKind, x => sessionKind = x));
    }

    public char decisionKind;
    public int decisionDate;
    public char sessionKind;
}

class OutReg : PrimeContainer
{
    public OutReg()
        : base("T_OUTREG")
    {
        Add(new TString("F_RECEIVER", () => receiver, x => receiver = x));
        Add(new TChar("F_RECEIVER_TYPE", () => receiverType, x => receiverType = x));
    }

    public string receiver;
    public char receiverType;
}

class Subpoena : BaseWind
{
    public Subpoena()
        : base("T_SUBPOENA")
    {
        Add(new TChar("F_EVENT_STATUS", () => eventStatus, x => eventStatus = x, TField.ExactKey));
        Add(new TInt("F_SUBPOENA_NO", () => subpoenaNo, x => subpoenaNo = x, TField.ExactKey));
        Add(new TChar("F_SUBPOENA_KIND", () => subpoenaKind, x => subpoenaKind = x));
        Add(new TInt("F_SUBPOENA_DATE", () => subpoenaDate, x => subpoenaDate = x));
        Add(new TInt("F_SUBPOENA_RECEIVED", () => subpoenaReceived, x => subpoenaReceived = x));
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(new TInt("F_STREET", () => street, x => street = x));
        Add(new TString("F_ADR_NO", () => adrNo, x => adrNo = x));
        Add(new TString("F_SUB_NO", () => subNo, x => subNo = x));
        Add(new TString("F_ENTRANCE", () => entrance, x => entrance = x));
        Add(new TString("F_FLOOR", () => floor, x => floor = x));
        Add(new TString("F_APARTMENT", () => apartment, x => apartment = x));
        Add(new TString("F_BLOCK_NO", () => blockNo, x => blockNo = x));
        Add(new TString("F_COUNTRY", () => country, x => country = x));
        Add(new TString("F_FOREIGN", () => foreign, x => foreign = x));
    }

    public AnyAddress GetAddress()
    {
        AnyAddress anyAddress = null;

        if (uclp != 0)
        {
            Address address = new Address();

            address.uclp = uclp;
            address.street = street;
            address.adrNo = adrNo;
            address.subNo = subNo;
            address.entrance = entrance;
            address.floor = floor;
            address.apartment = apartment;
            address.blockNo = blockNo;
            anyAddress = address;
        }
        else if (country != "")
        {
            ForeignAddress foreignAddress = new ForeignAddress();

            foreignAddress.country = country;
            foreignAddress.foreign = foreign;
            anyAddress = foreignAddress;
        }

        return anyAddress;
    }

    public char eventStatus;
    public int subpoenaNo;
    public char subpoenaKind;
    public int subpoenaDate;
    public int subpoenaReceived;
    // bulgarian address
    public int uclp;
    public int street;
    public string adrNo;
    public string subNo;
    public string entrance;
    public string floor;
    public string apartment;
    public string blockNo;
    // foreigh address
    public string country;
    public string foreign;
}

class Surround : PrimeContainer
{
    public Surround()
        : base("T_SURROUND")
    {
    }
}
