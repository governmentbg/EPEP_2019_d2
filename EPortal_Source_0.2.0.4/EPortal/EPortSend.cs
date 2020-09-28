using System;
using System.Text;

class EPortSend : RCDKeyContainer
{
    public EPortSend()
        : base("T_EPORT_SEND", 0)
    {
        Add(new TInt("F_RECORD_NO", () => recordNo, x => recordNo = x, TField.ExactKey));
        Add(new TInt("F_WHAT", () => what, x => what = x));
        Add(new TString("F_UCN", () => ucn, x => ucn = x));
        Add(new TChar("F_UCN_TYPE", () => ucnType, x => ucnType = x));
        Add(new TChar("F_INVOLVEMENT", () => involvement, x => involvement = x));
        Add(new TChar("F_EVENT_STATUS", () => eventStatus, x => eventStatus = x));
        Add(new TInt("F_SUBPOENA_NO", () => subpoenaNo, x => subpoenaNo = x));
        Add(new TChar("F_ELECTRIC", () => electric, x => electric = x));
        Add(new TChar("F_OPER", () => oper, x => oper = x));
        Add(new TChar("F_STATUS", () => status, x => status = x));
        Add(new TString("F_TEXT", () => text, x => text = x));
    }

    public EPortSend(EPortSend send, int what, char electric)
        : this()
    {
        key = send.key;
        kind = send.kind;
        date = send.date;
        ucn = send.ucn;
        ucnType = send.ucnType;
        involvement = send.involvement;
        eventStatus = send.eventStatus;
        subpoenaNo = send.subpoenaNo;
        this.what = what;
        this.electric = electric;
    }

    public EPortSend(RCDKey key, string ucn, char ucnType, int what)
        : this()
    {
        this.what = what;
        this.key = key;
        this.ucn = ucn;
        this.ucnType = ucnType;
    }

    public int recordNo;
    public int what;
    public string ucn;
    public char ucnType;
    public char involvement;
    public char eventStatus;
    public int subpoenaNo;
    public char electric;
    public char oper;
    public char status;
    public string text;

    private const int TextMaxLen = 100;

    public override void AddFloat(SqlBuilder builder)
    {
        AddFields(builder, CommonFields);
    }

    public Guid? GetGuid(Connection conn)
    {
        SqlSelect select = new SqlSelect(EPortGuidName, "F_GUID");

        AddFloat(select);
        
        using (Query q = new Query(conn, select))
            if (q.Read())
                return new Guid(q.GetString("F_GUID"));

        return null;
    }

    private void GenerateRecordNo(Connection conn)
    {
        SqlSelect select = new SqlSelect(Name, "MAX(F_RECORD_NO) AS F_RECORD_NO");

        AddFields(select, "F_NO, F_YEAR, F_TYPE");

        using (Query q = new Query(conn, select))
            recordNo = q.Read() ? q.GetInt("F_RECORD_NO") + 1 : 1;
    }

    public void Insert(Connection conn)
    {
        // TODO? use more plain insert
        /*SqlInsert insert = new SqlInsert(Name);

        AddFields(insert, CommonFields);
        GenerateRecordNo(conn);
        AddFields(insert, "F_RECORD_NO, F_OPER, F_STATUS");
        conn.Execute(insert);
        LogText("SqlInsert send", null);*/
        GenerateRecordNo(conn);
        base.Insert(conn, null);
        LogText("SqlInsert send", null);
    }

    public void DeleteConflicting(Connection conn)
    {
        SqlDelete delete = new SqlDelete(Name);

        AddFields(delete, CommonFields);
        conn.Execute(delete);
    }

    public void Delete(Connection conn)
    {
        Delete(conn, true);
        LogText("SqlDelete send", null);
    }

    public static Guid NewGuid(Connection conn)
    {
        bool exists;
        Guid guid;

        do  // though for(;;) is simpler
        {
            SqlSelect select = new SqlSelect(EPortGuidName, "F_GUID");

            guid = Guid.NewGuid();
            select.AddString("F_GUID", guid.ToString());

            using (Query q = new Query(conn, select))
                exists = q.Read();

        } while (exists);

        return guid;
    }

    public void InsertGuid(Connection conn, Guid guid)
    {
        SqlInsert insert = new SqlInsert(EPortGuidName);

        insert.AddString("F_GUID", guid.ToString());
        insert.AddDateTime("F_STAMP", DateTime.Now);
        AddFields(insert, CommonFields);
        conn.Execute(insert);
        LogText("SqlInsert", guid);
    }

    public static void DeleteConflictingGuid(Connection conn, Guid guid)
    {
        SqlSelect select = new SqlSelect(EPortGuidName, CommonFields);

        select.AddString("F_GUID", guid.ToString());

        using (Query q = new Query(conn, select))
        {
            if (q.Read())
            {
                EPortSend send = new EPortSend();

                send.GetData(q, CommonFields);
                send.DeleteGuid(conn, guid, " conflicting");
            }
        }
    }

    public void DeleteGuid(Connection conn, Guid guid, string what)
    {
        SqlDelete delete = new SqlDelete(EPortGuidName);

        delete.AddString("F_GUID", guid.ToString());
        conn.Execute(delete);
        LogText(String.Format("SqlDelete{0}", what), guid);
    }

    public void MarkWait(Connection conn)
    {
        if (status != Status.Wait)
        {
            SqlUpdate update = new SqlUpdate(Name);

            update.AddChar("F_STATUS", Status.Wait);
            update.Where();
            AddExact(update);
            conn.Execute(update);
        }
    }

    public void MarkError(Connection conn)
    {
        if (recordNo != 0)
        {
            SqlUpdate update = new SqlUpdate(Name);

            update.AddChar("F_STATUS", Status.Error);
            update.Where();
            AddExact(update);
            update.AddChar("F_STATUS", Status.Wait);
            conn.Execute(update);
        }
    }

    public void LogText(string text, Guid? guid)
    {
        StringBuilder sb = new StringBuilder(text);

        if (guid != null)
            sb.AppendFormat(" guid={0}", guid);

        if (key.type != Type.Empty)
            sb.AppendFormat(" type={0} {1}/{2}", key.type, key.no, key.year);

        sb.AppendFormat(" recN={0}", recordNo != 0 ? recordNo.ToString() : "//");

        sb.AppendFormat(" what={0}", what);

        if (oper != '\0')
            sb.AppendFormat(" oper={0}", oper);

        if (kind != '\0')
            sb.AppendFormat(" kind={0}", kind);

        if (date != 0)
            sb.AppendFormat(" date={0}", date);

        if (!String.IsNullOrEmpty(ucn))
            sb.AppendFormat(" ucn={0}, ucnType={1}", ucn, ucnType);

        if (involvement != '\0')
            sb.AppendFormat(" invl={0}", involvement);

        if (eventStatus != '\0')
            sb.AppendFormat(" evts={0}", eventStatus);

        if (subpoenaNo != 0)
            sb.AppendFormat(" subN={0}", subpoenaNo);

        if (electric != ElectricType.None)
            sb.AppendFormat(" elec={0}", electric);

        Log.Info(sb.ToString());
    }

    public void LogReceived(Guid? oldGuid, Guid newGuid)
    {
        LogText(String.Format("Received guid {0} for", newGuid), oldGuid != newGuid ? oldGuid : null);
    }

    public const string EPortGuidName = "T_EPORT_GUID";
    private const string CommonFields = "F_WHAT, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, " +
        "F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_EVENT_STATUS, F_SUBPOENA_NO, F_ELECTRIC";
}
