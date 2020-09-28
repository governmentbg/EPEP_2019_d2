TEPortGuid:

    public Guid? GetGuid()
    {
        if (guid != null)
            return new Guid(guid);

        return null;
    }

    public static Guid? GetGuid(Connection conn, EPortSend send)
    {
        EPortGuid guid = new EPortGuid();

        guid.CopyData(send);
        return guid.GetGuid(conn);
    }

    public static Guid? GetGuid(Connection conn, RCDKey key)
    {
        EPortGuid guid = new EPortGuid();

        guid.key = key;
        return guid.GetGuid(conn);
    }

SqlValueBuilder.CheckName():

        bool firstChar = true;

        foreach (char c in name)
        {
            if ((Char.ToUpper(c) < 'A' || Char.ToUpper(c) > 'Z') && c != '_')
            {
                if (firstChar)
                    throw new ArgumentException(String.Format("Invalid first character in {0} name '{1}'.", what, name));

                if (c < '0' || c > '9')
                    throw new ArgumentException(String.Format("Invalid subsequent character in {0} name '{1}'.", what, name));
            }

            firstChar = false;
        }

Program.MainLoop():

            using (Connection conn = new Connection())
            {
                Guid guid = new Guid("79b882e9-e65e-44e6-9869-d980cfa83ac1");
                EPortSend send = new EPortSend();

                EPortSend.DeleteConflictingGuid(conn, guid);

                send.no = 8829;
                send.year = 2016;
                send.type = 'r';
                send.what = 10000;
                send.kind = 'w';
                send.date = 96462868;
                send.electric = 2;
                send.InsertGuid(conn, guid);
            }

Sql:
    insert into t_eport_send (f_what, f_no, f_year, f_type, f_kind, f_date, f_electric, f_record_no, f_oper,
        f_oper_date, f_status)
    select f_what, f_no, f_year, f_type, f_kind, f_date, f_electric, 1, 'd', f_date, 's' from t_eport_guid;

    insert into t_eport_guid (F_GUID, F_WHAT, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE)
    values ('79b882e9-e65e-44e6-9869-d980cfa83ac1', 100, 8829, 2016, 'r', 'w', 96462868);

Lawsuit.FourteenValue():

        if (Const.CourtType == CourtType.Appeal)
        {
            if (type == Type.Lawsuit.AppealReCitizen)
                type = Type.Lawsuit.Citizen;
            else if (type == Type.Lawsuit.AppealReTrade)
                type = Type.Lawsuit.Trade;
        }
        else if (Const.CourtType == CourtType.Admin && type == Type.Lawsuit.AdminTricky)
            type = Type.Lawsuit.Punishment;

Type.Lawsuit:

    public static class Lawsuit
    {
        public const char
            Punishment = 'p',
            Citizen = 'c',
            Trade = 't',
            Admin = 'a',
            Firm = 'd',
            Marriage = 'm',
            AdminTricky = 't',
            AppealReCitizen = 'a',
            AppealReTrade = 'd';
    }

Program.MainLoop():

            {
                int CourtCode;
                string CourtName;
                char CourtType;

                string InstanceSelf = "0000000007";
                string ConfigNameGlobal = "*GLOBAL";

                Connection.Init("AppName");

                using (Connection conn = new Connection())
                {
                    Sender sender = Sender.Seek(conn, InstanceSelf, UcnType.Self);
                    Court court = new Court();
                    
                    court.court = sender.court;  // TODO: make alias
                    court.Get(conn, null);
                    CourtCode = court.court;
                    CourtName = court.name;
                    CourtType = court.courtType;
                }
            }

Program.MainLoop():

            {
                EPortSend send = new EPortSend();
                SqlSelect select = new SqlSelect(send.Name, "F_PROBA");

                const string WrongFields = "F_WHAT, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE" +
                    "F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_EVENT_STATUS, F_SUBPOENA_NO, F_ELECTRIC";

                send.AddFields(select, WrongFields);
            }

EPortSend:

    public void Generate(Connection conn)
    {
        SqlSelect select = new SqlSelect(Name, "MAX(F_RECORD_NO) AS F_RECORD_NO");

        AddFields(select, "F_NO, F_YEAR, F_TYPE, F_WHAT");

        using (Query q = new Query(conn, select))
            recordNo = q.Read() ? q.GetInt("F_RECORD_NO") + 1 : 1;
    }

    public void FlushSides(Connection conn)
    {
        SqlDelete delete = new SqlDelete(Name);

        AddFields(delete, "F_NO, F_YEAR, F_TYPE");
        delete.AddInt("F_WHAT", What.SideWind);
        conn.Execute(delete);
    }

Transfer.CreateElectric():

                        resend.Generate(conn);

Transfer.SendData():

            SqlSelect select = new SqlSelect(send.Name, "DISTINCT F_NO, F_YEAR, F_TYPE");

            conn = new Connection();

            // TODO: run this only if needed
            select.AddCharSet("F_STATUS", Status.Sendables);
            select.AddInt("F_WHAT", What.SideWindIntr);

            using (Query q = new Query(conn, select))
            {
                while (q.Read())
                {
                    send.GetData(q);
                    send.FlushSides(conn);
                }
            }

Transfer.SendData():

            foreach (EPortSend cached in Cache.Distinct(EPortSend.KeyComparer))
                if (cached.what == What.SideWindIntr)
                    cached.FlushSides(conn);

Transfer.SendData():

            using (Query q = new Query(conn, select))
            {
                while (q.Read())
                {
                    send.GetData(q);

                    if (send.oper != Oper.Write && send.oper != Oper.Delete)
                        throw new RangeException("Invalid send operation '{0}'.", send.oper);

                    if (proxy == null)
                    {
                        try
                        {
                            Log.Info("creating connection");
                            proxy = new IeCaseServiceClient("BasicHttpBinding_IeCaseService");
                        }
                        catch (Exception ex)
                        {
                            Log.Error(ex);
                            return;
                        }
                    }

                    SendElement(conn, send);
                }
            }

EPortSend:

    public class KeyComparerImpl : IEqualityComparer<EPortSend>
    {
        public bool Equals(EPortSend x, EPortSend y)
        {
            return x.key.no == y.key.no && x.key.year == y.key.year && x.key.type == y.key.type;
        }

        public virtual int GetHashCode(EPortSend x)
        {
            return (x.key.no << 16) + ((x.key.year % 100) << 8) + (int) x.key.type;  // ignores highest key.no bit
        }
    }

    public static readonly KeyComparerImpl KeyComparer = new KeyComparerImpl();

Transfer:

    static void CreateElectric(Connection conn, EPortSend send, int what, char primary, params char[] others)
    {
        if (!CreateElectric(conn, send, what, primary))
            foreach (char electric in others)
                if (CreateElectric(conn, send, what, electric))
                    break;
    }

Transfer.WriteRequest():

        CreateElectric(conn, send, What.IncomingFile, ElectricType.Image, ElectricType.Text);

EPortSend:

    public Guid? GetGuid(Connection conn, string fields)
    {
        SqlSelect select = new SqlSelect(EPortGuidName, "F_GUID");

        AddFields(select, fields);
        
        using (Query q = new Query(conn, select))
            if (q.Read())
                return new Guid(q.GetString("F_GUID"));

        return null;
    }

    public Guid? GetGuid(Connection conn) { return GetGuid(conn, CommonFields); }

EPortSend:

    public void DeleteConflicting(Connection conn, int whats)
    {
        SqlDelete delete = new SqlDelete(Name);

        AddFields(delete, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
        delete.AddRange("F_WHAT", whats, What.Factor + whats - 1);
        conn.Execute(delete);
    }

Transfer:

    static Query NewSideWindQuery(Connection conn, string fields, RCDKey key, char kind, int date)
    {
        SideWind sideWind = new SideWind();
        SqlSelect select = new SqlSelect(sideWind.Name, fields);

        sideWind.key = key;
        sideWind.kind = kind;
        sideWind.date = date;
        return new Query(conn, select);
    }

Address:

    public static string GetEMail(Connection conn, string ucn, char ucnType)
    {
        SqlSelect select = new SqlSelect("T_ADDRESS", "F_EMAIL");

        select.AddString("F_UCN", ucn);
        select.AddChar("F_UCN_TYPE", ucnType);
        select.AddString("NOT F_EMAIL", null);

        using (Query q = new Query(conn, select))
            return q.Read() ? q.GetString("F_EMAIL") : null;
    }

Transfer:

    static Guid FetchPersonId(Connection conn, EPortSend send)
    {
        EPortSend person = new EPortSend(RCDKey.Empty, send.ucn, send.ucnType, What.PersonRegistration);
        Guid? result = FetchObjectId(conn, person);

        if (result == null)
        {
            send.LogText("Auto-Send Person", null);
            result = WritePersonRegistration(conn, person, null);
        }

        return (Guid) result;
    }

    static Guid FetchLawerId(Connection conn, string ucn, char ucnType)
    {
        EPortSend send = new EPortSend(RCDKey.Empty, ucn, ucnType, What.LawyerRegistration);
        Guid? result = FetchObjectId(conn, send);
        Guid newGuid;

        if (result != null)
            newGuid = (Guid) result;
        else
        {
            send.LogText("Auto-Query Lawyer", null);

            KnownPerson person = KnownPerson.Seek(conn, ucn, ucnType, Involvement.Legalese);

            if (person.personalNo == "")
                throw new GetDataException("T_KNOWN_PERSON.F_PERSONAL_NO");

            send.LogText("proxy.GetLawyerByNumber", null);

            Lawyer lawyer = proxy.GetLawyerByNumber(person.personalNo);

            newGuid = (Guid) lawyer.LawyerId;
            send.LogReceived(null, newGuid);
            EPortSend.DeleteConflictingGuid(conn, newGuid);
            send.InsertGuid(conn, newGuid);
        }

        return newGuid;
    }
