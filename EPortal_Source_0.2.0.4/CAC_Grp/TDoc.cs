using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

public class TAddress : TAnyAddress
{
    public TAddress() : base("T_ADDRESS")
    {
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(new TInt("F_STREET", () => street, x => street = x));
        Add(new TString("F_ADR_NO", () => adrNo, x => adrNo = x));
        Add(new TString("F_SUB_NO", () => subNo, x => subNo = x));
        Add(new TString("F_ENTRANCE", () => entrance, x => entrance = x));
        Add(new TString("F_FLOOR", () => floor, x => floor = x));
        Add(new TString("F_APARTMENT", () => apartment, x => apartment = x));
        Add(new TString("F_BLOCK_NO", () => blockNo, x => blockNo = x));
        Add(new TString("F_PHONE", () => phone, x => phone = x));
        Add(new TString("F_TELEX", () => telex, x => telex = x));
        Add(new TString("F_FAX", () => fax, x => fax = x));
        Add(new TString("F_EMAIL", () => email, x => email = x));
        Add(new TString("F_REMARK", () => remark, x => remark = x));
    }

    public int uclp;
    public int street;
    public string adrNo;
    public string subNo;
    public string entrance;
    public string floor;
    public string apartment;
    public string blockNo;
    public string phone;
    public string telex;
    public string fax;
    public string email;
    public string remark;

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

    protected static readonly string[] UclpPrefixes = { "ГРАД", "ГР", "СЕЛО", "С", "МЕСТНОСТ", "М" };

    public override string GetFullText(Connection conn)
    {
        TUCLP uclp = TUCLP.Seek(conn, this.uclp);
        //string filtered = FilterPrefixes(uclp.name, UclpPrefixes);
        TArea area = TArea.Seek(uclp.area);
        TRegion region = TRegion.Seek(uclp.area, uclp.region);
        StringBuilder fullText = new StringBuilder();

        //if (area.name != filtered || region.name != filtered)
            fullText.AppendFormat("Област {0}, Община {1}, ", area.name, region.name);

        fullText.Append(uclp.name);

        if (street != 0)
            fullText.AppendFormat(", {0}", TStreet.Seek(conn, this.uclp, street).name);

        AddressPart[] parts = new AddressPart[]
        {
            new AddressPart("No ", adrNo),
            new AddressPart("Блок ", blockNo),
            new AddressPart("Подн.", subNo),
            new AddressPart("Вх.", entrance),
            new AddressPart("Ет.", floor),
            new AddressPart("Ап.", apartment)
        };

        foreach (AddressPart part in parts)
            if (part.text != "")
                fullText.AppendFormat(", {0}{1}", part.prefix, part.text);

        return fullText.ToString();
    }
}

public class TAnnounce : TAnnounceGroup
{
    public TAnnounce() : base("T_ANNOUNCE")
    {
        Add(new TInt("F_ANNOUNCE", () => announce, x => announce = x));
        Add(new TInt("F_SUBPOENA_DATE", () => subpoenaDate, x => subpoenaDate = x));
        Add(new TChar("F_SUBPOENA_KIND", () => subpoenaKind, x => subpoenaKind = x));
        Add(new TInt("F_SESSION_DATE", () => sessionDate, x => sessionDate = x));
        Add(new TInt("F_SIDE_NO", () => sideNo, x => sideNo = x));
        Add(new TString("F_SUBPOENA_TEXT", () => subpoenaText, x => subpoenaText = x));
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(new TInt("F_STREET", () => street, x => street = x));
        Add(new TString("F_DELIVERER_UCN", () => delivererUCN, x => delivererUCN = x));
        Add(new TInt("F_SUBPOENA_RECEIVED", () => subpoenaReceived, x => subpoenaReceived = x));
        Add(new TInt("F_RECEIVED_TIME", () => receivedTime, x => receivedTime = x));
        Add(new TChar("F_RECEIVED_STATUS", () => receivedStatus, x => receivedStatus = x));
        Add(new TString("F_RECEIVED_TEXT", () => receivedText, x => receivedText = x));
        Add(new TInt("F_SUBPOENA_RETURNED", () => subpoenaReturned, x => subpoenaReturned = x));
        Add(new TInt("F_COMPOSITION", () => composition, x => composition = x));
        Add(new TString("F_ADR_NO", () => adrNo, x => adrNo = x));
        Add(new TString("F_SUB_NO", () => subNo, x => subNo = x));
        Add(new TString("F_ENTRANCE", () => entrance, x => entrance = x));
        Add(new TString("F_FLOOR", () => floor, x => floor = x));
        Add(new TString("F_APARTMENT", () => apartment, x => apartment = x));
        Add(new TString("F_BLOCK_NO", () => blockNo, x => blockNo = x));
        Add(new TInt("F_REQUEST_ID", () => requestId, x => requestId = x));
        Add(new TString("F_BARCODE", () => barcode, x => barcode = x));
        Add(new TDouble("F_COORD_N", () => coordN, x => coordN = x));
        Add(new TDouble("F_COORD_E", () => coordE, x => coordE = x));
    }

    protected override string ExactFields()
    {
        return FloatFields() + ", F_ANNOUNCE";
    }

    protected override string FloatFields()
    {
        return "F_NO, F_YEAR, F_KIND, F_COURT";
    }

    public int announce;
    public int subpoenaDate;
    public char subpoenaKind;
    public int sessionDate;
    public int sideNo;
    public string subpoenaText;
    public int uclp;
    public int street;
    public string delivererUCN;
    public int subpoenaReceived;
    public int receivedTime;
    public char receivedStatus;
    public string receivedText;
    public int subpoenaReturned;
    public int composition;
    public string adrNo;
    public string subNo;
    public string entrance;
    public string floor;
    public string apartment;
    public string blockNo;
    public int requestId;
    public string barcode;
    public double coordN;
    public double coordE;
}

public class TAnnounceVisit : TAnnounceGroup
{
    public TAnnounceVisit() : base("T_ANNOUNCE_VISIT")
    {
        Add(new TInt("F_ANNOUNCE", () => announce, x => announce = x));
        Add(new TDateTime("F_STAMP", () => stamp, x => stamp = x));
        Add(new TDouble("F_COORD_N", () => coordN, x => coordN = x));
        Add(new TDouble("F_COORD_E", () => coordE, x => coordE = x));
    }

    public int announce;
    public DateTime stamp;
    public double coordN;
    public double coordE;

    protected override string ExactFields()
    {
        return FloatFields() + ", F_STAMP";
    }

    protected override string FloatFields()
    {
        return "F_NO, F_YEAR, F_KIND, F_COURT, F_ANNOUNCE";
    }
}

public class TCitizen : TPersonGroup
{
    public TCitizen() : base("T_CITIZEN")
    {
        Add(new TString("F_RENAME", () => reName, x => reName = x));
        Add(new TString("F_FAMILY", () => family, x => family = x));
        Add(new TString("F_REFAMILY", () => reFamily, x => reFamily = x));
        Add(new TString("F_COUNTRY", () => country, x => country = x));
        Add(new TString("F_RECOUNTRY", () => reCountry, x => reCountry = x));
        Add(new TString("F_RANK", () => rank, x => rank = x));
        Add(new TString("F_LNC", () => lnc, x => lnc = x));
    }

    public string reName;
    public string family;
    public string reFamily;
    public string country;
    public string reCountry;
    public string rank;
    public string lnc;
}

public class TConfigData : TGroup
{
    public TConfigData() : base("T_CONFIG_DATA")
    {
        Add(new TString("F_CONFIG", () => config, x => config = x));
        Add(new TString("F_SECTION", () => section, x => section = x));
        Add(new TString("F_NAME", () => name, x => name = x));
        Add(new TString("F_VALUE", () => value, x => value = x));
    }

    protected override string ExactFields()
    {
        return "F_CONFIG, F_SECTION, F_NAME";
    }

    protected override string FloatFields()
    {
        return "F_CONFIG";
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

public class TDecision : TDatedContainer
{
    public TDecision() : base("T_DECISION")
    {
        Add(new TInt("F_FROM_DATE", () => fromDate, x => fromDate = x));
        Add(new TChar("F_RESULT", () => result, x => result = x));
        Add(new TInt("F_RETURNED", () => returned, x => returned = x));
        Add(new TString("F_TEXT", () => text, x => text = x));
        Add(new TInt("F_ENTER_DATE", () => enterDate, x => enterDate = x));
        Add(new TChar("F_SESSION_KIND", () => sessionKind, x => sessionKind = x));
        Add(new TString("F_ACCOMPLYS", () => accomplys, x => accomplys = x));
        Add(new TString("F_SYNDICATE", () => syndicate, x => syndicate = x));
        Add(new TInt("F_EVENT_NO", () => eventNo, x => eventNo = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TString("F_MOTIVES", () => motives, x => motives = x));
        Add(new TInt("F_FORCE_DATE", () => forceDate, x => forceDate = x));
        Add(new TString("F_OPINION", () => opinion, x => opinion = x));
        Add(new TChar("F_SLC_REASON", () => slcReason, x => slcReason = x));
        Add(new TInt("F_FLAGS_X", () => flagsX, x => flagsX = x));
        Add(new TInt("F_PUBLIC_DATE", () => publicDate, x => publicDate = x));
    }

    public int fromDate;
    public char result;
    public int returned;
    public string text;
    public int enterDate;
    public char sessionKind;
    public string accomplys;
    public string syndicate;
    public int eventNo;
    public int flags;
    public string motives;
    public int forceDate;
    public string opinion;
    public char slcReason;
    public int flagsX;
    public int publicDate;

    public const uint
        Confess = 0x02,
        Absence = 0x04,
        Public  = 0x08,
        Motives = 0x10,
        FAll    = 0x1E,
        //
        XContract = 0x01,
        XAppeal   = 0x02,
        XPublTReg = 0x04,
        XFAll     = 0x07;
}

public class TFirm : TJudicialGroup
{
    public TFirm() : base("T_FIRM")
    {
        Add(new TString("F_UFN", () => ufn, x => ufn = x));
        Add(new TString("F_COUNTRY", () => country, x => country = x));
    }

    public string ufn;
    public string country;
}

public class TForeignAddress : TAnyAddress
{
    public TForeignAddress() : base("T_FOREIGN_ADDRESS")
    {
        Add(new TString("F_COUNTRY", () => country, x => country = x));
        Add(new TString("F_FOREIGN", () => foreign, x => foreign = x));
    }

    public string country;
    public string foreign;

    protected override int DefaultAddress { get { return -1; } }

    public override string GetFullText(Connection conn)
    {
        StringBuilder fullText = new StringBuilder(TCountry.Seek(country).name);

        if (foreign != "")
            fullText.AppendFormat(" {0}", foreign);

        return fullText.ToString();
    }
}

public class TInReg : TPrimeContainer
{
    public TInReg() : base("T_INREG")
    {
        Add(new TString("F_TEXT", () => text, x => text = x));
        Add(new TInt("F_DECISION_NO", () => decision.no, x => decision.no = x));
        Add(new TInt("F_DECISION_YEAR", () => decision.year, x => decision.year = x));
        Add(new TChar("F_DECISION_TYPE", () => decision.type, x => decision.type = x));
        Add(new TChar("F_DECISION_KIND", () => decisionKind, x => decisionKind = x));
        Add(new TInt("F_DECISION_DATE", () => decisionDate, x => decisionDate = x));
        Add(new TString("F_JUDGE", () => judge, x => judge = x));
        Add(new TChar("F_SESSION_KIND", () => sessionKind, x => sessionKind = x));
        Add(new TChar("F_COLLEGE", () => college, x => college = x));
        Add(new TInt("F_SURROUND_NO", () => surround.no, x => surround.no = x));
        Add(new TInt("F_SURROUND_YEAR", () => surround.year, x => surround.year = x));
        Add(new TChar("F_SURROUND_TYPE", () => surround.type, x => surround.type = x));
        Add(new TString("F_DIRECTION", () => direction, x => direction = x));
        Add(new TString("F_RECEIVER", () => receiver, x => receiver = x));
        Add(new TChar("F_RECEIVER_TYPE", () => receiverType, x => receiverType = x));
        Add(new TInt("F_EXTRA_NO", () => extraNo, x => extraNo = x));
        Add(new TString("F_EPORTAL_MAIL", () => eportalMail, x => eportalMail = x));
        Add(new TInt("F_EPORTAL_MODE", () => eportalMode, x => eportalMode = x));
    }

    public string text;
    public TRCDKey decision;
    public char decisionKind;
    public int decisionDate;
    public string judge;
    public char sessionKind;
    public char college;
    public TRCDKey surround;
    public string direction;
    public string receiver;
    public char receiverType;
    public int extraNo;
    public string eportalMail;
    public int eportalMode;
}

public class TInRegEvent : TIndexContainer
{
    public TInRegEvent() : base("T_INREG_EVENT")
    {
        Add(new TString("F_TEXT", () => text, x => text = x));
        Add(new TInt("F_OUT_NO", () => outNo, x => outNo = x));
        Add(new TInt("F_OUT_YEAR", () => outYear, x => outYear = x));
        Add(new TString("F_JUDGE", () => judge, x => judge = x));
        Add(new TInt("F_EVENT_NO", () => eventNo, x => eventNo = x));
        Add(new TInt("F_RETURNED", () => returned, x => returned = x));
    }

    public string text;
    public int outNo;
    public int outYear;
    public string judge;
    public int eventNo;
    public int returned;
}

public class TLawsuit : TOriginContainer
{
    public TLawsuit() : base("T_LAWSUIT")
    {
        Add(new TInt("F_SOURCE_NO", () => source.no, x => source.no = x));
        Add(new TInt("F_SOURCE_YEAR", () => source.year, x => source.year = x));
        Add(new TChar("F_SOURCE_TYPE", () => source.type, x => source.type = x));
        Add(new TChar("F_SOURCE_KIND", () => sourceKind, x => sourceKind = x));
        Add(new TInt("F_SUBJECT", () => subject, x => subject = x));
        Add(new TInt("F_COMPOSITION", () => composition, x => composition = x));
        Add(new TString("F_JUDGE", () => judge, x => judge = x));
        Add(new TChar("F_AGE", () => age, x => age = x));
        Add(new TInt("F_FINISHED", () => finished, x => finished = x));
        Add(new TChar("F_GRANTED", () => granted, x => granted = x));
        Add(new TString("F_TEXT", () => text, x => text = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TString("F_IMPORTANT", () => important, x => important = x));
        Add(new TString("F_REJUDGE", () => reJudge, x => reJudge = x));
        Add(new TInt("F_QUICK", () => quick, x => quick = x));
        Add(new TInt("F_FLAGS_X", () => flagsX, x => flagsX = x));
    }

    public TRCDKey source;
    public char sourceKind;
    public int subject;
    public int composition;
    public string judge;
    public char age;
    public int finished;
    public char granted;
    public string text;
    public int flags;
    public string important;
    public string reJudge;
    public int quick;
    public int flagsX;

    public const uint
        Important = 0x01,
        Corrupt   = 0x04,
        NewGpk    = 0x08,
        Limited   = 0x10,
        FAll      = 0x1D,
        //
        XDoubleX  = 0x01,
        XBankrupt = 0x02,
        XChildren = 0x04,
        XComplex  = 0x08,
        XFALL     = 0x0F;
}

public class TLinkWind : TBaseWind
{
    public TLinkWind() : base("T_LINK_WIND") { AddFields(); }
    public TLinkWind(string Table) : base(Table) { AddFields(); }

    private void AddFields()
    {
        Add(new TString("F_PROXY_UCN", () => proxyUCN, x => proxyUCN = x));
        Add(new TChar("F_PROXY_UCN_TYPE", () => proxyUCNType, x => proxyUCNType = x));
        Add(new TChar("F_PROXY_INVOLVEMENT", () => proxyInvolvement, x => proxyInvolvement = x));
        Add(new TChar("F_PROXY_KIND", () => proxyKind, x => proxyKind = x));
        Add(new TString("F_REDIRECT_UCN", () => redirectUCN, x => redirectUCN = x));
        Add(new TChar("F_REDIRECT_UCN_TYPE", () => redirectUCNType, x => redirectUCNType = x));
        Add(new TChar("F_REDIRECT_INVOLVEMENT", () => redirectInvolvement, x => redirectInvolvement = x));
        Add(new TChar("F_REDIRECT_KIND", () => redirectKind, x => redirectKind = x));
    }

    protected override string ExactFields()
    {
        return base.ExactFields() + ", F_PROXY_UCN, F_PROXY_UCN_TYPE, F_PROXY_INVOLVEMENT";
    }

    protected void GetSide(TSideWind sideWind, char tUCNType, string tUCN, char tInvolvement)
    {
        sideWind.key = key;
        sideWind.kind = kind;
        sideWind.date = date;
        sideWind.ucnType = tUCNType;
        sideWind.ucn = tUCN;
        sideWind.involvement = tInvolvement;
    }

    public void GetOrdinary(TSideWind sideWind) { GetSide(sideWind, ucnType, ucn, involvement); }
    public void GetProxy(TSideWind sideWind) { GetSide(sideWind, proxyUCNType, proxyUCN, proxyInvolvement); }
    public void GetRedirect(TSideWind sideWind) { GetSide(sideWind, redirectUCNType, redirectUCN, redirectInvolvement); }

    public string PrintSideLinks(Connection conn, bool newLines, bool ordinaryName, List<TLinkWind> linkWinds, char nbsp)
    {
        TLinkWind linkWind = this;
        StringBuilder sb = new StringBuilder();
        List<TLinkWind> tLinkWinds;

        if (linkWinds == null)
        {
            tLinkWinds = new List<TLinkWind>();
            tLinkWinds.Add(linkWind);
            linkWinds = tLinkWinds;
        }

        if (ordinaryName)
        {
            bool first = true;

            foreach (TLinkWind tLinkWind in linkWinds)
            {
                if (first)
                    first = false;
                else
                    sb.Append(", ");

                sb.Append(tLinkWind.PrintNames(conn));
            }
        }

        if (linkWind.Proxied())
        {
            TSideWind proxy = new TSideWind();
            TSideWind redirect = new TSideWind();
            string proxyNames;
            string proxyInvolvement;
            bool redirectSeparator = false;

            linkWind.GetProxy(proxy);
            proxyNames = proxy.PrintNames(conn);
            proxyInvolvement = TInvolvement.Seek(linkWind.proxyInvolvement).name;

            if (TInvolvement.LegalSpeakers.IndexOf(proxy.involvement) != -1 ||
                linkWind.proxyKind == TProxyKind.AsWithInvolvement)
            {
                switch (linkWind.proxyKind)
                {
                    case TProxyKind.IndirectAsAny1 :
                    {
                        if (!newLines)
                            sb.Append(",");

                        sb.AppendFormat("\nпредставляван{0} от {1}\n{2}", linkWinds.Count == 1 ? "(а)" : "и",
                            proxyInvolvement, proxyNames);
                        break;
                    }
                    case TProxyKind.IndirectAsAny2 :
                    {
                        sb.AppendFormat("\nчрез {0}\n{1}", proxyInvolvement, proxyNames);
                        break;
                    }
                    case TProxyKind.IndirectAsAny3 :
                    {
                        sb.AppendFormat("\nсъс съгласието на {0}\n{1}", proxyInvolvement, proxyNames);
                        break;
                    }
                    case TProxyKind.ConsentOfIndirect :
                    {
                        sb.AppendFormat("{0}\nсъс съгласието на\n{1} като {2}", nbsp, proxyNames, proxyInvolvement);
                        redirectSeparator = true;
                        break;
                    }
                    default :
                    {
                        string prefix = String.Format("{0}\nкато {1} на{2}\n", proxyNames, proxyInvolvement, nbsp);

                        sb.Insert(0, prefix, 1);
                        redirectSeparator = true;
                        break;
                    }
                }
            }
            else if (proxy.involvement == TInvolvement.Parent)  // can't happen, Parent is LegalSpeaker
            {
                sb.AppendFormat("\nсъс съгласието на {0}{1}\n{2}", proxyInvolvement, nbsp, proxyNames);
                redirectSeparator = true;
            }
            else
            {
                sb.AppendFormat("{0}\nчрез {1}\n{2}", nbsp, proxyInvolvement, proxyNames);
                redirectSeparator = true;
            }
            
            if (linkWind.Redirected())
            {
                if (redirectSeparator)
                    sb.Append(nbsp);
                linkWind.GetRedirect(redirect);
                sb.AppendFormat("\nчрез {0} ", TInvolvement.Seek(redirect.involvement).name);
                sb.Append(redirect.PrintNames(conn));
            }
        }

        if (newLines)
            sb.Append("\n");
        else
            sb.Replace('\n', ' ');

        string result = sb.ToString();

        if (!ordinaryName && result[0] == nbsp)
            result = result.Substring(1).TrimStart();

        return result;
    }

    public bool Proxied()
    {
        return proxyUCNType != '\0' && proxyUCN.Length > 0 && proxyInvolvement != '\0';
    }

    public bool Redirected()
    {
        return redirectUCNType != '\0' && redirectUCN.Length > 0 && redirectInvolvement != '\0';
    }

    public bool SameLinkage(TLinkWind that)
    {
        return involvement == that.involvement &&
            proxyUCNType == that.proxyUCNType && proxyUCN == that.proxyUCN &&
            proxyInvolvement == that.proxyInvolvement && proxyKind == that.proxyKind &&
            redirectUCNType == that.redirectUCNType && redirectUCN == that.redirectUCN &&
            redirectInvolvement == that.redirectInvolvement && redirectKind == that.redirectKind;
    }

    public string proxyUCN;
    public char proxyUCNType;
    public char proxyInvolvement;
    public char proxyKind;
    public string redirectUCN;
    public char redirectUCNType;
    public char redirectInvolvement;
    public char redirectKind;
}

public class TLog : TGroup
{
    public TLog() : base("T_LOG")
    {
        Add(new TInt("F_LOG", () => log, x => log = x));
        Add(new TString("F_UCN", () => ucn, x => ucn = x));
        Add(new TChar("F_GOP", () => gop, x => gop = x));
        Add(new TInt("F_OPC", () => opc, x => opc = x));
        Add(new TDateTime("F_STAMP", () => stamp, x => stamp = x));
        Add(new TInt("F_NO", () => key.no, x => key.no = x));
        Add(new TInt("F_YEAR", () => key.year, x => key.year = x));
        Add(new TChar("F_TYPE", () => key.type, x => key.type = x));
        Add(new TChar("F_KIND", () => kind, x => kind = x));
        Add(new TDateTime("F_DATE", () => date, x => date = x));
        Add(new TString("F_TEXT", () => text, x => text = x));
        resBlock = new int[4];
        Add(new TInt("F_RES_1", () => resBlock[0], x => resBlock[0] = x));
        Add(new TInt("F_RES_2", () => resBlock[1], x => resBlock[1] = x));
        Add(new TInt("F_RES_3", () => resBlock[2], x => resBlock[2] = x));
        Add(new TInt("F_RES_4", () => resBlock[3], x => resBlock[3] = x));
    }

    protected override string ExactFields()
    {
        return "F_LOG";
    }

    public int log;
    public string ucn;
    public char gop;
    public int opc;
    public DateTime stamp;
    public TRCDKey key;
    public char kind;
    public DateTime date;
    public string text;
    public int[] resBlock;
}

public class TOutReg : TPrimeContainer
{
    public TOutReg() : base("T_OUTREG")
    {
        Add(new TString("F_RECEIVER", () => receiver, x => receiver = x));
        Add(new TInt("F_IN_NO", () => inNo, x => inNo = x));
        Add(new TInt("F_IN_YEAR", () => inYear, x => inYear = x));
        Add(new TChar("F_RESULT", () => result, x => result = x));
        Add(new TString("F_REMARK", () => remark, x => remark = x));
        Add(new TInt("F_RETURNED", () => returned, x => returned = x));
        Add(new TChar("F_RESULT_I", () => resultI, x => resultI = x));
        Add(new TChar("F_RESULT_II", () => resultII, x => resultII = x));
        Add(new TChar("F_RESULT_KS", () => resultKS, x => resultKS = x));
        Add(new TChar("F_RESULT_MJ", () => resultMJ, x => resultMJ = x));
        Add(new TChar("F_COLLEGE", () => college, x => college = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TString("F_VSS_INDEX", () => vssIndex, x => vssIndex = x));
        Add(new TChar("F_RECEIVER_TYPE", () => receiverType, x => receiverType = x));
        Add(new TInt("F_FLAGS_X", () => flagsX, x => flagsX = x));
        Add(new TChar("F_DECISION_KIND", () => decisionKind, x => decisionKind = x));
        Add(new TInt("F_DECISION_DATE", () => decisionDate, x => decisionDate = x));
        Add(new TString("F_REMARK_VZ", () => remarkVZ, x => remarkVZ = x));
        Add(new TString("F_REMARK_KS", () => remarkKS, x => remarkKS = x));
        Add(new TString("F_TEXT", () => text, x => text = x));
    }

    public string receiver;
    public int inNo;
    public int inYear;
    public char result;
    public string remark;
    public int returned;
    public char resultI;
    public char resultII;
    public char resultKS;
    public char resultMJ;
    public char college;
    public int flags;
    public string vssIndex;
    public char receiverType;
    public int flagsX;
    public char decisionKind;
    public int decisionDate;
    public string remarkVZ;
    public string remarkKS;
    public string text;

    public const uint
        Regular   = 0x01,
        R         = 0x02,
        AR        = 0x03,
        Package   = 0x06,
        Postal    = 0x0F,
        WriteMail = 0x02,
        Electron  = 0x03,
        //
        XCourier  = 0x10,
        XFax      = 0x20,
        XEmail    = 0x40,
        XRestart  = 0x80,
        XElectron = 0x70,
        XFAll     = 0xF0;
}

public class TRequest : TOriginContainer
{
    public TRequest() : base("T_REQUEST")
    {
        Add(new TString("F_TEXT", () => text, x => text = x));
        Add(new TInt("F_SUBJECT", () => subject, x => subject = x));
        Add(new TInt("F_QUICK", () => quick, x => quick = x));
        Add(new TString("F_DIRECTION", () => direction, x => direction = x));
        Add(new TChar("F_LAW_KIND", () => lawKind, x => lawKind = x));
    }

    public string text;
    public int subject;
    public int quick;
    public string direction;
    public char lawKind;
}

public class TSender : TPersonGroup
{
    public TSender() : base("T_SENDER")
    {
        Add(new TInt("F_COURT", () => court, x => court = x));
        Add(new TInt("F_EISPP", () => eispp, x => eispp = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
    }

    protected override string FloatFields()
    {
        return "F_UCN_TYPE";
    }

    public static TSender Seek(Connection conn, string ucn, char ucnType)
    {
        TSender sender = new TSender();

        sender.ucn = ucn;
        sender.ucnType = ucnType;
        sender.Get(conn, null);
        return sender;
    }

    public int court;
    public int eispp;
    public int flags;
}

public class TSession : TDatedContainer
{
    public TSession() : base("T_SESSION")
    {
        Add(new TInt("F_END", () => end, x => end = x));
        Add(new TChar("F_COMPOSITION", () => composition, x => composition = x));
        Add(new TString("F_JUDGE", () => judge, x => judge = x));
        Add(new TString("F_SECRETAR", () => secretar, x => secretar = x));
        Add(new TString("F_PROSECUTOR", () => prosecutor, x => prosecutor = x));
        Add(new TString("F_JUROR", () => juror, x => juror = x));
        Add(new TString("F_REJUROR", () => reJuror, x => reJuror = x));
        Add(new TChar("F_RESULT", () => result, x => result = x));
        Add(new TString("F_JUDGE_X", () => judgeX, x => judgeX = x));
        Add(new TString("F_REJUDGE_X", () => reJudgeX, x => reJudgeX = x));
        Add(new TString("F_PRESIDENT", () => president, x => president = x));
        Add(new TInt("F_RETURNED", () => returned, x => returned = x));
        Add(new TString("F_TEXT", () => text, x => text = x));
        Add(new TString("F_RESERVE", () => reserve, x => reserve = x));
        Add(new TInt("F_HALL", () => hall, x => hall = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TInt("F_EVENT_NO", () => eventNo, x => eventNo = x));
        Add(new TInt("F_FINAL", () => final, x => final = x));
        Add(new TInt("F_FORCE_DATE", () => forceDate, x => forceDate = x));
        Add(new TChar("F_SLC_REASON", () => slcReason, x => slcReason = x));
        Add(new TInt("F_FLAGS_X", () => flagsX, x => flagsX = x));
        Add(new TInt("F_FLAGS_Y", () => flagsY, x => flagsY = x));
    }

    public int end;
    public char composition;
    public string judge;
    public string secretar;
    public string prosecutor;
    public string juror;
    public string reJuror; 
    public char result;
    public string judgeX;
    public string reJudgeX;
    public string president;
    public int returned;
    public string text;
    public string reserve;
    public int hall;
    public int flags;
    public int eventNo;
    public int final;
    public int forceDate;
    public char slcReason;
    public int flagsX;
    public int flagsY;

    public const uint
        Proceed = 0x01,
        Chanjed = 0x02,
        Expert  = 0x10,
        Private = 0x20,
        FAll    = 0x33,
        //
        XClosedD = 0x01,
        XFirSet  = 0x02,
        XAppeal  = 0x04,
        XRecujed = 0x08,
        XFAll    = 0x0F,
        //
        YPublTReg = 0x04,
        YRestart  = 0x01,
        YFAll     = 0x04;
}

public class TSideWind : TBaseWind
{
    public TSideWind() : base("T_SIDE_WIND")
    {
        Add(new TChar("F_STATE", () => state, x => state = x));
        Add(new TInt("F_FLAGS", () => flags, x => flags = x));
        Add(new TChar("F_FROM_SIDE", () => fromSide, x => fromSide = x));
        Add(new TString("F_UNWANTED", () => unwanted, x => unwanted = x));
    }

    public bool Appealer()
    {
        return involvement == TInvolvement.Appealer || TSideState.Appealers.IndexOf(state) != -1;
    }

    public char state;
    public int flags;
    public char fromSide;
    public string unwanted;

    public const uint
        Young = 0x01,
        FAll  = 0x07;
}

public class TSubpoena : TLinkWind
{
    public TSubpoena() : base("T_SUBPOENA")
    {
        Add(new TChar("F_EVENT_STATUS", () => eventStatus, x => eventStatus = x));
        Add(new TInt("F_SUBPOENA_NO", () => subpoenaNo, x => subpoenaNo = x));
        Add(new TChar("F_SUBPOENA_KIND", () => subpoenaKind, x => subpoenaKind = x));
        Add(new TInt("F_SUBPOENA_DATE", () => subpoenaDate, x => subpoenaDate = x));
        Add(new TChar("F_OFFICIALLY_NOTIFIED", () => officiallyNotified, x => officiallyNotified = x));
        Add(new TInt("F_SIDE_NO", () => sideNo, x => sideNo = x));
        Add(new TInt("F_EXPENSE", () => expense, x => expense = x));
        Add(new TInt("F_UCLP", () => uclp, x => uclp = x));
        Add(new TInt("F_STREET", () => street, x => street = x));
        Add(new TString("F_SUB_NO", () => subNo, x => subNo = x));
        Add(new TString("F_ENTRANCE", () => entrance, x => entrance = x));
        Add(new TString("F_FLOOR", () => floor, x => floor = x));
        Add(new TString("F_APARTMENT", () => apartment, x => apartment = x));
        Add(new TString("F_BLOCK_NO", () => blockNo, x => blockNo = x));
        Add(new TString("F_PHONE", () => phone, x => phone = x));
        Add(new TString("F_TELEX", () => telex, x => telex = x));
        Add(new TString("F_FAX", () => fax, x => fax = x));
        Add(new TString("F_EMAIL", () => email, x => email = x));
        Add(new TString("F_REMARK", () => remark, x => remark = x));
        Add(new TInt("F_SUBPOENA_RECEIVED", () => subpoenaReceived, x => subpoenaReceived = x));
        Add(new TInt("F_RECEIVED_TIME", () => receivedTime, x => receivedTime = x));
        Add(new TChar("F_RECEIVED_STATUS", () => receivedStatus, x => receivedStatus = x));
        Add(new TString("F_RECEIVED_TEXT", () => receivedText, x => receivedText = x));
        Add(new TInt("F_SUBPOENA_RETURNED", () => subpoenaReturned, x => subpoenaReturned = x));
        Add(new TString("F_DELIVERER_UCN", () => delivererUCN, x => delivererUCN = x));
        Add(new TString("F_USER_UCN", () => userUCN, x => userUCN = x));
        Add(new TInt("F_BY_POST", () => byPost, x => byPost = x));
        Add(new TInt("F_BY_POST_X", () => byPostX, x => byPostX = x));
        Add(new TInt("F_REGARD", () => regard, x => regard = x));
        Add(new TString("F_LRECEIVER_UCN", () => lReceiverUCN, x => lReceiverUCN = x));
        Add(new TChar("F_LRECEIVER_UCN_TYPE", () => lReceiverUCNType, x => lReceiverUCNType = x));
        Add(new TInt("F_OUTREG_NO", () => outRegNo, x => outRegNo = x));
        Add(new TInt("F_OUTREG_YEAR", () => outRegYear, x => outRegYear = x));
        Add(new TChar("F_MULTI", () => multi, x => multi = x));
        Add(new TString("F_ADR_NO", () => adrNo, x => adrNo = x));
        Add(new TString("F_COUNTRY", () => country, x => country = x));
        Add(new TString("F_FOREIGN", () => foreign, x => foreign = x));
        Add(new TInt("F_ANNOUNCE_NO", () => announceNo, x => announceNo = x));
        Add(new TInt("F_REQUEST_ID", () => requestId, x => requestId = x));
        Add(new TInt("F_EXEC_LIST_NO", () => execListNo, x => execListNo = x));
        Add(new TChar("F_EXEC_LIST_KIND", () => execListKind, x => execListKind = x));
        Add(new TDouble("F_COORD_N", () => coordN, x => coordN = x));
        Add(new TDouble("F_COORD_E", () => coordE, x => coordE = x));
        Add(new TInt("F_VALID_UNTIL", () => validUntil, x => validUntil = x));
    }

    protected override string ExactFields()
    {
        return "F_UCN, F_UCN_TYPE, " + FloatFields() + ", F_INVOLVEMENT, F_SUBPOENA_NO";
    }

    protected override string FloatFields()
    {
        return base.FloatFields() + ", F_EVENT_STATUS";
    }

    public char eventStatus;
    public int subpoenaNo;
    public char subpoenaKind;
    public int subpoenaDate;
    public char officiallyNotified;
    public int sideNo;
    public int expense;
    public int uclp;
    public int street;
    public string subNo;
    public string entrance;
    public string floor;
    public string apartment;
    public string blockNo;
    public string phone;
    public string telex;
    public string fax;
    public string email;
    public string remark;
    public int subpoenaReceived;
    public int receivedTime;
    public char receivedStatus;
    public string receivedText;
    public int subpoenaReturned;
    public string delivererUCN;
    public string userUCN;
    public int byPost;
    public int byPostX;
    public int regard;
    public string lReceiverUCN;
    public char lReceiverUCNType;
    public int outRegNo;
    public int outRegYear;
    public char multi;
    public string adrNo;
    public string country;
    public string foreign;
    public int announceNo;
    public int requestId;
    public int execListNo;
    public char execListKind;
    public double coordN;
    public double coordE;
    public int validUntil;

    public void AddMulti(SqlCriteriaBuilder builder)
    {
        AddFloat(builder);
        AddFields(builder, "F_SUBPOENA_NO");
        builder.AddChar("F_MULTI", TSubpoena.Multi);
    }

    public void AddRegard(SqlCriteriaBuilder builder, bool stickers)
    {
        AddFields(builder, "F_UCN, F_UCN_TYPE, " + FloatFields() + ", F_INVOLVEMENT");
        builder.AddInt("F_REGARD", subpoenaNo);
        builder.AddCharSet(stickers ? "F_SUBPOENA_KIND" : "NOT F_SUBPOENA_KIND", TSubpoenaKind.Stickers);
    }

    public string PrintNotifieds(Connection conn, List<TLinkWind> pLinks, string sidesep, string invlsep, char nbsp)
    {
        List<TLinkWind> links = new List<TLinkWind>(pLinks);
        StringBuilder sb = new StringBuilder();

        if (links.Count == 0)
            links.Add(this);

        for (;;)
        {
            List<TLinkWind> tLinks = new List<TLinkWind>();
            TLinkWind linkWind = links[0];

            tLinks.Add(linkWind);
            links.RemoveAt(0);

            int index = 0;

            while (index < links.Count)
            {
                TLinkWind tLinkWind = links[index];

                if (tLinkWind.SameLinkage(linkWind))
                {
                    tLinks.Add(tLinkWind);
                    links.RemoveAt(index);
                }
                else
                    index++;
            }

            sb.Append("До ");
            sb.Append(linkWind.PrintSideLinks(conn, false, true, tLinks, nbsp));

            if (linkWind.Proxied() && linkWind.proxyKind == TProxyKind.AsWithInvolvement)
                sb.Append("");  // involvements already shown, skip lead involvement
            else
                sb.AppendFormat("{0}като {1}", invlsep, TInvolvement.Seek(linkWind.involvement).name);

            if (links.Count > 0)
                sb.Append(sidesep);
            else
                break;
        }

        return sb.ToString();
    }

    public string PrintNotifieds(Connection conn, string sidesep, string invlsep, char nbsp)
    {
        List<TLinkWind> links = new List<TLinkWind>();

        if (multi == TSubpoena.Leader)
        {
            SqlSelect select = new SqlSelect(Table, null);

            AddMulti(select);
            LoadArray(conn, select, links);
        }

        return Regex.Replace(PrintNotifieds(conn, links, sidesep, invlsep, nbsp), "\\s+", " ");
    }

    public const char
        Single = 's',
        Multi  = 'm',
        Leader = 'l';
}

public class TSubpoenaVisit : TRCDKeyGroup
{
    public TSubpoenaVisit() : base("T_SUBPOENA_VISIT")
    {
        Add(new TChar("F_KIND", () => kind, x => kind = x));
        Add(new TInt("F_DATE", () => date, x => date = x));
        Add(new TInt("F_SUBPOENA_NO", () => subpoenaNo, x => subpoenaNo = x));
        Add(new TDateTime("F_STAMP", () => stamp, x => stamp = x));
        Add(new TDouble("F_COORD_N", () => coordN, x => coordN = x));
        Add(new TDouble("F_COORD_E", () => coordE, x => coordE = x));
    }

    public char kind;
    public int date;
    public int subpoenaNo;
    public DateTime stamp;
    public double coordN;
    public double coordE;

    protected override string ExactFields()
    {
        return FloatFields() + ", F_STAMP";
    }

    protected override string FloatFields()
    {
        return "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_SUBPOENA_NO";
    }
}
