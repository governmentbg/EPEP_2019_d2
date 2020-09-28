using System;
using System.Collections.Generic;
using System.IO;
using System.ServiceModel;
using System.Text;
using www.abbaty.com.eCase.v2;      // official
//using EPortal.eCaseService;       // testing

static class Transfer
{
    public static void SendData()
    {
        Connection conn = null;

        try
        {
            SqlSelect select = new SqlSelect(template.Name, null);
            List<EPortSend> array = new List<EPortSend>();

            proxy = null;
            conn = new Connection();
            select.AddCharSet("F_STATUS", Status.Sendables);
            select.AddOrder("F_NO, F_YEAR, F_TYPE, F_RECORD_NO");
            template.LoadArray(conn, select, array);

            foreach (EPortSend send in array)
            {
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

                SendElement(conn, send, "Found");
            }

            foreach (EPortSend send in Cache)
                SendElement(conn, send, "Cache");
        }
        finally
        {
            Cache.Clear();
            Utility.Dispose(conn);

            if (proxy != null)
            {
                Log.Info("disconnecting");
                // don't know why, but works for EISPP
                proxy.Close();
                proxy.Abort();
            }
        }
    }

    static void LogDetailError(FaultException<InfocaseFault> ex)
    {
        StringBuilder sb = new StringBuilder();

        sb.AppendFormat("Remote error, code={0}", ex.Detail.ErrorCode);

        if (ex.Detail.ErrorField != null)
            sb.AppendFormat(", field={0}", ex.Detail.ErrorField);

        sb.AppendFormat(", reason={0}", ex.Detail.Reason);
        Log.Error(sb.ToString());
    }

    private static readonly EPortSend template = new EPortSend();
    private static List<EPortSend> Cache = new List<EPortSend>();

    private static void SendElement(Connection conn, EPortSend send, string where)
    {
        Guid? guid = FetchObjectId(conn, send);

        send.LogText(where, guid);
        send.MarkWait(conn);

        try
        {
            SendRedirect(conn, send, guid);
        }
        catch (NotImplementedException ex)
        {
            Log.Error(ex);
            send.Delete(conn);
        }
        catch (GetDataException ex)
        {
            Log.Error(ex);
            send.MarkError(conn);
        }
        catch (FaultException<InfocaseFault> ex)
        {
            LogDetailError(ex);
            send.MarkError(conn);
        }
        catch (RangeException ex)
        {
            throw ex;
        }
        catch (Exception ex)
        {
            Log.Error(ex);
        }
    }

    // Generic methods

    private static void CreateResend(Connection conn, EPortSend resend, char oper)
    {
        resend.DeleteConflicting(conn);
        resend.oper = oper;
        resend.status = Status.Wait;
        resend.Insert(conn);
        Cache.Add(resend);
    }

    static IeCaseServiceClient proxy = null;

    private delegate bool DeleteHandler(Guid guid);

    static void DeleteObject(Connection conn, EPortSend send, Guid? guid, DeleteHandler deleteHandler)
    {
        if (guid != null)
        {
            send.LogText(String.Format("proxy.{0}", deleteHandler.Method.Name), guid);

            try
            {
                deleteHandler((Guid) guid);
            }
            catch (FaultException<InfocaseFault> ex)
            {
                if (ex.Detail.ErrorCode != 3)
                    throw ex;
                // 2018-10-18
                Log.Info(ex.ToString());
            }

            send.DeleteGuid(conn, (Guid) guid, "");
        }

        if (send.recordNo != 0)
            send.Delete(conn, true);
    }

    static void DeleteElectricFile(Connection conn, EPortSend parentSend, Guid parentGuid, EPortSend fileSend, Guid fileGuid,
        DeleteHandler deleteHandler)
    {
        parentSend.LogText(String.Format("proxy.{0}", deleteHandler.Method.Name), parentGuid);

        try
        {
            deleteHandler(parentGuid);
        }
        catch (FaultException<InfocaseFault> ex)
        {
            if (ex.Detail.ErrorCode != 3)
                throw ex;
        }

        fileSend.DeleteGuid(conn, fileGuid, "");
    }

    static void CleanupElectric(Connection conn, EPortSend parentSend, Guid? parentGuid, int what, char electric,
        DeleteHandler deleteHandler)
    {
        if (parentGuid != null)
        {
            EPortSend fileSend = new EPortSend(parentSend, what, electric);
            Guid? fileGuid = fileSend.GetGuid(conn);

            if (fileGuid != null)
                DeleteElectricFile(conn, parentSend, (Guid) parentGuid, fileSend, (Guid) fileGuid, deleteHandler);
        }
    }

    static void DeleteElectric(Connection conn, EPortSend fileSend, Guid? fileGuid, int parentWhat,
        DeleteHandler deleteHandler)
    {
        if (fileGuid != null)
        {
            EPortSend parentSend = new EPortSend(fileSend, parentWhat, '\0');
            Guid? parentGuid = parentSend.GetGuid(conn);

            if (parentGuid != null)
            {
                DeleteElectricFile(conn, parentSend, (Guid) parentGuid, fileSend, (Guid) fileGuid, deleteHandler);
                fileSend.Delete(conn, true);
            }
        }
    }

    static void CleanupRequest(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupElectric(conn, send, guid, What.RequestFile, ElectricType.Image,
            new DeleteHandler(proxy.DeleteIncomingDocumentFile));
    }

    private delegate void CleanupHandler(Connection conn, EPortSend send, Guid? guid);

    static void CleanupRange(Connection conn, EPortSend send, int minWhat, int maxWhat, string idFields,
        CleanupHandler cleanupHandler, DeleteHandler deleteHandler)
    {
        SqlSelect select = new SqlSelect(EPortSend.EPortGuidName, "F_GUID, F_WHAT, " + idFields);

        select.AddRange("F_WHAT", minWhat, maxWhat);
        send.AddFields(select, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");

        using (Query q = new Query(conn, select))
        {
            while (q.Read())
            {
                Guid reguid = new Guid(q.GetString("F_GUID"));
                EPortSend resend = new EPortSend(send, q.GetInt("F_WHAT"), ElectricType.None);
                
                resend.GetData(q, idFields);

                if (cleanupHandler != null)
                    cleanupHandler(conn, resend, reguid);

                DeleteObject(conn, resend, reguid, deleteHandler);
            }
        }
    }

    static void CleanupSideWind(Connection conn, EPortSend send, Guid? guid)
    {
        int what = Involvement.AssignWhat(send.involvement);
        SqlSelect select = new SqlSelect(EPortSend.EPortGuidName, "F_GUID");

        select.AddInt("F_WHAT", what);
        send.AddFields(select, "F_NO, F_YEAR, F_TYPE, F_UCN_TYPE, F_UCN, F_INVOLVEMENT");

        using (Query q = new Query(conn, select))
        {
            while (q.Read())
            {
                Guid reguid = new Guid(q.GetString("F_GUID"));
                EPortSend resend = new EPortSend(send, what, ElectricType.None);
                
                DeleteObject(conn, resend, reguid, what == What.PersonAssignment ?
                    new DeleteHandler(proxy.DeletePersonAssignment) : new DeleteHandler(proxy.DeleteLawyerAssignment));
            }
        }
    }

    static void CleanupJudges(Connection conn, EPortSend send, int minWhat, int maxWhat, DeleteHandler deleteHandler)
    {
        CleanupRange(conn, send, minWhat, maxWhat, "F_UCN_TYPE, F_UCN", null, deleteHandler);
    }

    static void CleanupSubpoena(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupElectric(conn, send, guid, send.what - What.SubpoenaBase + What.SubpoenaFileBase, ElectricType.Text,
            new DeleteHandler(proxy.DeleteSummonFile));
    }

    static void CleanupMessages(Connection conn, EPortSend send, int what)
    {
        CleanupRange(conn, send, what, what, "F_UCN_TYPE, F_UCN, F_INVOLVEMENT, F_EVENT_STATUS, F_SUBPOENA_NO",
            new CleanupHandler(CleanupSubpoena), new DeleteHandler(proxy.DeleteSummon));
    }

    static void CleanupSession(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupJudges(conn, send, What.ParticipantPresident, What.ParticipantJudge,
            new DeleteHandler(proxy.DeleteHearingParticipant));
        CleanupMessages(conn, send, What.SessionSubpoena);
        CleanupElectric(conn, send, guid, What.PrivateProtocolFile, ElectricType.Text,
            new DeleteHandler(proxy.DeletePrivateProtocolFile));
        CleanupElectric(conn, send, guid, What.PublicProtocolFile, ElectricType.Filter,
            new DeleteHandler(proxy.DeletePublicProtocolFile));
    }

    static void CleanupSessEndoc(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupJudges(conn, send, What.ActPreparatorPresident, What.ActPreparatorJudge,
            new DeleteHandler(proxy.DeleteActPreparator));
        CleanupMessages(conn, send, What.EndocMessage);

        if (!SessEndoc.IsEPortSession(send.kind))
        {
            CleanupElectric(conn, send, guid, What.PrivateActFile, ElectricType.Text,
                new DeleteHandler(proxy.DeletePrivateActFile));
            CleanupElectric(conn, send, guid, What.PublicActFile, ElectricType.Filter,
                new DeleteHandler(proxy.DeletePublicActFile));
        }
    }

    static void CleanupDecision(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupJudges(conn, send, What.ActPreparatorPresident, What.ActPreparatorJudge,
            new DeleteHandler(proxy.DeleteActPreparator));
        CleanupMessages(conn, send, What.EndocMessage);
        CleanupElectric(conn, send, guid, What.PrivateActFile, ElectricType.Text,
            new DeleteHandler(proxy.DeletePrivateActFile));
        CleanupElectric(conn, send, guid, What.PublicActFile, ElectricType.Filter,
            new DeleteHandler(proxy.DeletePublicActFile));

        // the widest check for motivable
        if (Type.Seek(send.key.type).ecode == Lawsuit.ECode.Punishment && send.kind != Kind.Bulwark)
        {
            CleanupElectric(conn, send, guid, What.PrivateMotiveFile, ElectricType.Text,
                new DeleteHandler(proxy.DeletePrivateMotiveFile));
            CleanupElectric(conn, send, guid, What.PublicMotiveFile, ElectricType.Filter,
                new DeleteHandler(proxy.DeletePublicMotiveFile));
        }
    }

    static void CleanupInReg(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupMessages(conn, send, What.AppealMessage);
    }

    static void CleanupOutReg(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupElectric(conn, send, guid, What.OutRegFile, ElectricType.Text,
            new DeleteHandler(proxy.DeleteOutgoingDocumentFile));
    }

    static void CleanupSurround(Connection conn, EPortSend send, Guid? guid)
    {
        CleanupElectric(conn, send, guid, What.SurroundFile, ElectricType.Image,
            new DeleteHandler(proxy.DeleteIncomingDocumentFile));
    }

    static Guid? FetchObjectId(Connection conn, EPortSend send)
    {
        Guid? guid = send.GetGuid(conn);

        //if (guid = null)
        //    call a proxy method to select guid by key fields

        return guid;
    }

    static void WriteSuccess(Connection conn, EPortSend send, Guid? oldGuid, Guid newGuid)
    {
        send.LogReceived(oldGuid, newGuid);

        if (newGuid != oldGuid)
        {
            if (oldGuid != null)
                send.DeleteGuid(conn, (Guid) oldGuid, " obsoleted");

            EPortSend.DeleteConflictingGuid(conn, newGuid);
            send.InsertGuid(conn, newGuid);
        }

        send.Delete(conn, true);
    }

    static bool LimitedLawsuit(Connection conn, RCDKey origin)
    {
        if (Type.Lawsuits.IndexOf(origin.type) == -1)
            return false;

        Lawsuit lawsuit = new Lawsuit();

        lawsuit.key = origin;
        lawsuit.Get(conn, "F_FLAGS");
        return (lawsuit.flags & Lawsuit.Limited) != 0;
    }

    static void CreateElectricResend(Connection conn, string name, EPortSend resend, char electric, RCDKey origin)
    {
        try
        {
            // !File.Exists() is too broad
            File.GetAttributes(name);

            if (electric != ElectricType.Filter || !LimitedLawsuit(conn, origin))
            {
                resend.text = name;
                CreateResend(conn, resend, Oper.Write);
            }
        }
        catch (FileNotFoundException)
        {
            if (resend.GetGuid(conn) != null)
                CreateResend(conn, resend, Oper.Delete);
        }
        catch (DirectoryNotFoundException)
        {
        }
        catch (IOException ex)
        {
            Log.Error(ex);
        }
    }

    static void UpdateElectric(Connection conn, EPortSend send, char kind, int date, int what, char electric, RCDKey origin)
    {
        EPortSend resend = new EPortSend(send, what, electric);
        string name = Electric.Name(resend, kind, date, electric);

        if (name != null)
        {
            string mimeType = MimeType.Get(name);

            if (mimeType != null)
                CreateElectricResend(conn, name, resend, electric, origin);
        }
    }

    static void UpdateElectric(Connection conn, EPortSend send, int what, char electric, RCDKey origin)
    {
        UpdateElectric(conn, send, send.kind, send.date, what, electric, origin);
    }    

    static byte[] ReadElectricFile(Connection conn, EPortSend send, out string mimeType)
    {
        byte[] content = null;
        string name = send.text;

        mimeType = MimeType.Get(name);

        try
        {
            content = File.ReadAllBytes(name);

            if (MimeType.Ambiguous(name))
                mimeType = MimeType.Reget(content);
        }
        catch (FileNotFoundException)
        {
            send.LogText(String.Format("File {0} not found for", name), null);
            send.Delete(conn, true);
        }

        return content;
    }

    static Guid? TryElectricParentGuid(Connection conn, EPortSend send, int what)
    {
        EPortSend parent = new EPortSend(send, what, ElectricType.None);
        Guid? guid = parent.GetGuid(conn);

        if (guid == null)
        {
            parent.LogText("guid not found for", null);
            send.Delete(conn, true);
        }

        return guid;
    }

    // Specific methods

    static Guid WriteRequest(Connection conn, EPortSend send, Guid? oldGuid)
    {
        IncomingDocument document = new IncomingDocument();
        Request request = new Request();

        document.IncomingDocumentId = oldGuid;
        request.key = send.key;
        request.Get(conn, "F_KIND, F_DATE");

        document.CaseId = null;
        document.CourtCode = Const.CourtCode.ToString();
        document.IncomingNumber = send.key.no;
        document.IncomingDate = IntDate.ToDate(request.date);
        document.IncomingDocumentTypeCode = Kind.Seek(request.kind).xfer.ToString();
        document.Person = null;

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertIncomingDocument", null);
            newGuid = (Guid) proxy.InsertIncomingDocument(document);
        }
        else
        {
            send.LogText("proxy.UpdateIncomingDocument", oldGuid);
            newGuid = (Guid) proxy.UpdateIncomingDocument(document);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        UpdateElectric(conn, send, request.kind, request.date, What.RequestFile, ElectricType.Image, request.key);
        return newGuid;
    }

    static void WriteIncomingFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, Type.IncomingWhat(send.key.type));

        if (parentGuid != null)
        {
            IncomingDocumentFile file = new IncomingDocumentFile();
            string mime;

            file.IncomingDocumentContent = ReadElectricFile(conn, send, out mime);

            if (file.IncomingDocumentContent != null)
            {
                file.IncomingDocumentFileId = oldGuid;
                file.IncomingDocumentId = (Guid) parentGuid;
                file.IncomingDocumentMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertIncomingDocumentFile", null);
                    newGuid = (Guid) proxy.InsertIncomingDocumentFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdateIncomingDocumentFile", oldGuid);
                    newGuid = (Guid) proxy.UpdateIncomingDocumentFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static Guid FetchRequestId(Connection conn, RCDKey key)
    {
        EPortSend send = new EPortSend();

        send.key = key;
        send.what = What.Request;

        Guid? result = FetchObjectId(conn, send);

        if (result == null)
        {
            send.LogText("Auto-Send Request", null);
            result = WriteRequest(conn, send, null);
        }

        return (Guid) result;
    }

    class SideWindQuery : Query
    {
        public SideWindQuery(Connection conn, string fields, RCDKey key, char kind, int date)
            : base(conn, CreateSelect(fields, key, kind, date))
        {
        }

        private static string CreateSelect(string fields, RCDKey key, char kind, int date)
        {
            SideWind sideWind = new SideWind();
            SqlSelect select = new SqlSelect(sideWind.Name, fields);

            sideWind.key = key;
            sideWind.kind = kind;
            sideWind.date = date;
            sideWind.AddFloat(select);
            return select;
        }
    }

    static void CreateSideWinds(Connection conn, Lawsuit lawsuit)
    {
        const string fields = "F_UCN, F_UCN_TYPE, F_INVOLVEMENT";

        using (Query q = new SideWindQuery(conn, fields, lawsuit.key, lawsuit.kind, lawsuit.date))
        {
            EPortSend resend = new EPortSend();

            resend.what = What.SideWind;
            resend.key = lawsuit.key;

            while (q.Read())
            {
                resend.GetData(q, fields);

                if (resend.GetGuid(conn) == null)
                    CreateResend(conn, resend, Oper.Write);
            }
        }
    }

    static Guid WriteLawsuit(Connection conn, EPortSend send, Lawsuit lawsuit, Guid? oldGuid)
    {
        Case eCase = new Case();
        int ecode = Type.Seek(send.key.type).ecode;

        eCase.CaseId = oldGuid;
        eCase.CourtCode = Const.CourtCode.ToString();
        eCase.CaseKindCode = Kind.Seek(lawsuit.kind).xfer.ToString();
        eCase.CaseTypeCode = ecode.ToString("D4");

        if (lawsuit.source.type == Type.Request)
            eCase.IncomingDocumentId = FetchRequestId(conn, lawsuit.source);

        eCase.Number = lawsuit.key.no;
        eCase.CaseYear = lawsuit.key.year;
        eCase.FormationDate = IntDate.ToDate(lawsuit.date);
        eCase.DepartmentName = College.Seek(lawsuit.college).name;
        if (send.text == "1")
            eCase.PanelName = Composition.Seek(lawsuit.composition, lawsuit.key.type).name;

        //Subject subject;
        //
        //subject = Subject.Seek(lawsuit.subject, lawsuit.key.type);
        //eCase.StatisticCode = subject.Code();
        //eCase.LegalSubject = subject.name;

        //eCase.CaseCode = Lawsuit.FourteenValue(ecode, lawsuit.kind).ToString("D3");
        //eCase.Status = "";

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertCase", null);
            newGuid = (Guid) proxy.InsertCase(eCase);
        }
        else
        {
            send.LogText("proxy.UpdateCase", oldGuid);
            newGuid = (Guid) proxy.UpdateCase(eCase);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        return newGuid;
    }

    static Guid WriteLawsuit(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Lawsuit lawsuit = new Lawsuit();

        lawsuit.key = send.key;
        lawsuit.Get(conn, null);
        return WriteLawsuit(conn, send, lawsuit, oldGuid);
    }

    static Guid FetchLawsuitId(Connection conn, RCDKey key)
    {
        EPortSend send = new EPortSend();

        send.key = key;
        send.what = What.Lawsuit;

        Guid? result = FetchObjectId(conn, send);

        if (result == null)
        {
            Lawsuit lawsuit = new Lawsuit();

            lawsuit.key = key;
            lawsuit.Get(conn, null);
            send.LogText("Auto-Send Lawsuit", null);
            result = WriteLawsuit(conn, send, lawsuit, null);

            try
            {
                CreateSideWinds(conn, lawsuit);
            }
            catch (Exception ex)
            {
                Log.Error(ex);
            }
        }

        return (Guid) result;
    }

    static Person TryCitizen(Connection conn, string ucn, char ucnType)
    {
        Person person = null;

        if (UcnType.Citizens.IndexOf(ucnType) != -1)
        {
            Citizen citizen = Citizen.GetForEP(conn, ucn, ucnType);

            person = new Person();
            person.EGN = citizen.ucn;
            person.Firstname = citizen.name;
            person.Secondname = citizen.reName;
            person.Lastname = citizen.family;
            person.Address = AnyAddress.GetFullText(conn, ucn, ucnType);
        }
        
        return person;
    }

    static Entity TryFirm(Connection conn, string ucn, char ucnType)
    {
        Entity entity = null;

        if (UcnType.Firms.IndexOf(ucnType) != -1)
        {
            Firm firm = Firm.GetForEP(conn, ucn, ucnType);

            entity = new Entity();
            entity.Name = firm.name;
            entity.Bulstat = firm.bulstat;
            entity.Address = AnyAddress.GetFullText(conn, ucn, ucnType);
        }

        return entity;
    }

    static Guid WriteSideWind(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Side side = new Side();

        side.SideId = oldGuid;
        side.CaseId = FetchLawsuitId(conn, send.key);
        side.SideInvolvementKindCode = Involvement.Seek(send.involvement).xfer.ToString();
        side.Person = TryCitizen(conn, send.ucn, send.ucnType);

        if (side.Person == null)
            side.Entity = TryFirm(conn, send.ucn, send.ucnType);

        side.IsActive = true;
        side.InsertDate = DateTime.Now;

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertSide", null);
            newGuid = (Guid) proxy.InsertSide(side);
        }
        else
        {
            send.LogText("proxy.UpdateSide", oldGuid);
            newGuid = (Guid) proxy.UpdateSide(side);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        return newGuid;
    }

    static Guid FetchSideWindId(Connection conn, EPortSend resend)
    {
        Guid? result = resend.GetGuid(conn);

        if (result == null)
            result = WriteSideWind(conn, resend, null);

        return (Guid) result;
    }

    static void UpdateJudge(Connection conn, EPortSend send, string ucn, int what)
    {
        EPortSend resend = new EPortSend(send, what, ElectricType.None);
        const string fields = "F_UCN_TYPE, F_UCN";
        SqlSelect select = new SqlSelect(EPortSend.EPortGuidName, fields);
            
        resend.AddFields(select, "F_WHAT, F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");

        using (Query q = new Query(conn, select))
        {
            while (q.Read())
            {
                resend.GetData(q, "F_UCN_TYPE, F_UCN");

                if (resend.ucn != ucn)
                    CreateResend(conn, resend, Oper.Delete);
            }
        }

        if (!Ucn.IsEmpty(ucn))
        {
            resend.ucn = ucn;
            resend.ucnType = UcnType.CitizenUcn;
            CreateResend(conn, resend, Oper.Write);
        }
    }

    static void Update4thJudge(Connection conn, EPortSend send, Session session, int what)
    {
        string ucn = session.judge;

        if (ucn == session.president || ucn == session.reJudgeX || ucn == session.judgeX)
            ucn = "";

        UpdateJudge(conn, send, ucn, what);
    }

    static bool FilterSessions(Connection conn)
    {
        ConfigData configData = new ConfigData();

        configData.config = Const.ConfigNameGlobal;
        configData.section = "filter";
        return configData.GetValue(conn, "SESSIONS") != "0";
    }

    static Guid WriteSession(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Hearing hearing = new Hearing();
        Session session = new Session();

        hearing.HearingId = oldGuid;
        hearing.CaseId = FetchLawsuitId(conn, send.key);
        session.key = send.key;
        session.kind = send.kind;
        session.date = send.date;
        session.Get(conn, null);

        hearing.HearingType = Kind.Seek(send.kind).what == What.OpenSession ? "Открито" : "Закрито";
        hearing.HearingResult = session.result == Result.Fixed ? null : Result.Seek(session.result).name;
        hearing.Date = IntDate.ToDate(send.date);

        if (!Ucn.IsEmpty(session.secretar))
            hearing.SecretaryName = Citizen.GetFullName(conn, session.secretar, UcnType.CitizenUcn);

        if (!Ucn.IsEmpty(session.prosecutor))
            hearing.ProsecutorName = Citizen.GetFullName(conn, session.prosecutor, UcnType.CitizenUcn);

        if (session.hall != 0)
            hearing.CourtRoom = Hall.Seek(session.hall).name;

        if (Result.Unlinkeds.IndexOf(session.result) != -1)
            hearing.IsCanceled = true;

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertHearing", null);
            newGuid = (Guid) proxy.InsertHearing(hearing);
        }
        else
        {
            send.LogText("proxy.UpdateHearing", oldGuid);
            newGuid = (Guid) proxy.UpdateHearing(hearing);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        UpdateJudge(conn, send, session.president, What.ParticipantPresident);
        UpdateJudge(conn, send, session.reJudgeX, What.ParticipantReJudgeX);
        UpdateJudge(conn, send, session.judgeX, What.ParticipantJudgeX);
        Update4thJudge(conn, send, session, What.ParticipantJudge);
        UpdateElectric(conn, send, What.PrivateProtocolFile, ElectricType.Text, send.key);

        if ((session.flags & Session.Private) == 0 && FilterSessions(conn))
            UpdateElectric(conn, send, What.PublicProtocolFile, ElectricType.Filter, send.key);

        return newGuid;
    }

    static Guid FetchSessionId(Connection conn, RCDKey key, char kind, int date)
    {
        EPortSend send = new EPortSend();

        send.key = key;
        send.kind = kind;
        send.date = date;
        send.what = What.Session;

        Guid? result = FetchObjectId(conn, send);

        if (result == null)
        {
            send.LogText("Auto-Send Session", null);
            result = WriteSession(conn, send, null);
        }

        return (Guid) result;
    }

    static void WriteParticipant(Connection conn, EPortSend send, Guid? oldGuid)
    {
        HearingParticipant participant = new HearingParticipant();

        participant.HearingParticipantId = oldGuid;
        participant.HearingId = FetchSessionId(conn, send.key, send.kind, send.date);
        participant.JudgeName = Citizen.GetFullName(conn, send.ucn, send.ucnType);

        switch (send.what)
        {
            case What.ParticipantPresident : participant.Role = "председател"; break;
            case What.ParticipantJudge : participant.Role = "докладчик"; break;
            default : participant.Role = "член на състав"; break;
        }

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertHearingParticipant", null);
            newGuid = (Guid) proxy.InsertHearingParticipant(participant);
        }
        else
        {
            send.LogText("proxy.UpdateHearingParticipant", oldGuid);
            newGuid = (Guid) proxy.UpdateHearingParticipant(participant);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
    }

    static void WritePrivateProtocolFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, What.Session);

        if (parentGuid != null)
        {
            PrivateProtocolFile file = new PrivateProtocolFile();
            string mime;

            file.ProtocolContent = ReadElectricFile(conn, send, out mime);

            if (file.ProtocolContent != null)
            {
                file.PrivateProtocolFileId = oldGuid;
                file.HearingId = (Guid) parentGuid;
                file.ProtocolMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertPrivateProtocolFile", null);
                    newGuid = (Guid) proxy.InsertPrivateProtocolFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdatePrivateProtocolFile", oldGuid);
                    newGuid = (Guid) proxy.UpdatePrivateProtocolFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static void WritePublicProtocolFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, What.Session);

        if (parentGuid != null)
        {
            PublicProtocolFile file = new PublicProtocolFile();
            string mime;

            file.ProtocolContent = ReadElectricFile(conn, send, out mime);

            if (file.ProtocolContent != null)
            {
                file.PublicProtocolFileId = oldGuid;
                file.HearingId = (Guid) parentGuid;
                file.ProtocolMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertPublicProtocolFile", null);
                    newGuid = (Guid) proxy.InsertPublicProtocolFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdatePublicProtocolFile", oldGuid);
                    newGuid = (Guid) proxy.UpdatePublicProtocolFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static char SessionResult2DecisionKind(char sessionResult)
    {
        switch (sessionResult)
        {
            case Result.WithDecree : return Kind.Decree;
            case Result.WithOrder :
            case Result.NonJoinOrder :
            case Result.HoldOrder : return Kind.Order;
        }

        return Const.CourtType == CourtType.Regional ? Kind.SettRegi : Kind.SettNReg;
    }

    static Guid WriteSessEndoc(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Act act = new Act();
        SessEndoc session = new SessEndoc();

        session.key = send.key;
        session.kind = send.kind;
        session.date = send.date;
        session.Get(conn, null);

        act.ActId = oldGuid;
        act.ActKindCode = Kind.Seek(SessionResult2DecisionKind(session.result)).xfer.ToString();
        act.CaseId = FetchLawsuitId(conn, send.key);

        if (session.forceDate != 0)
            act.DateInPower = IntDate.ToDate(session.forceDate);

        act.DateSigned = IntDate.ToDate(send.date);
        if (session.IsEPortSession())
            act.HearingId = FetchSessionId(conn, session.key, session.kind, session.date);

        if (send.text == "1" && session.eventNo != 0)
            act.Number = session.eventNo;

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertAct", null);
            newGuid = (Guid) proxy.InsertAct(act);
        }
        else
        {
            send.LogText("proxy.UpdateAct", oldGuid);
            newGuid = (Guid) proxy.UpdateAct(act);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        UpdateJudge(conn, send, session.president, What.ActPreparatorPresident);
        UpdateJudge(conn, send, session.reJudgeX, What.ActPreparatorReJudgeX);
        UpdateJudge(conn, send, session.judgeX, What.ActPreparatorJudgeX);
        Update4thJudge(conn, send, session, What.ActPreparatorJudge);

        if (!session.IsEPortSession() && FilterSessions(conn))
        {
                UpdateElectric(conn, send, What.PrivateActFile, ElectricType.Text, send.key);

                if ((session.flags & Session.Private) == 0)
                    UpdateElectric(conn, send, What.PublicActFile, ElectricType.Filter, send.key);
        }

        return newGuid;
    }

    static Guid WriteDecision(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Act act = new Act();
        Decision decision = new Decision();
        Session session = new Session();

        decision.key = send.key;
        decision.kind = send.kind;
        decision.date = send.date;
        decision.Get(conn, null);
        session.key = send.key;
        session.kind = decision.sessionKind;
        session.date = send.date;
        session.Get(conn, null);

        act.ActId = oldGuid;
        act.ActKindCode = Kind.Seek(send.kind).xfer.ToString();
        act.CaseId = FetchLawsuitId(conn, send.key);
        
        if (decision.forceDate != 0)
            act.DateInPower = IntDate.ToDate(decision.forceDate);

        act.DateSigned = IntDate.ToDate(decision.fromDate);
        act.HearingId = FetchSessionId(conn, session.key, session.kind, session.date);

        if (send.text == "1" && decision.eventNo != 0)
            act.Number = decision.eventNo;

        if ((decision.flags & Decision.Motives) != 0 && decision.returned != 0)
            act.MotiveDate = IntDate.ToDate(decision.returned);

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertAct", null);
            newGuid = (Guid) proxy.InsertAct(act);
        }
        else
        {
            send.LogText("proxy.UpdateAct", oldGuid);
            newGuid = (Guid) proxy.UpdateAct(act);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        UpdateJudge(conn, send, session.president, What.ActPreparatorPresident);
        UpdateJudge(conn, send, session.reJudgeX, What.ActPreparatorReJudgeX);
        UpdateJudge(conn, send, session.judgeX, What.ActPreparatorJudgeX);
        Update4thJudge(conn, send, session, What.ActPreparatorJudge);
        UpdateElectric(conn, send, What.PrivateActFile, ElectricType.Text, send.key);

        if ((decision.flags & Decision.Public) != 0)
            UpdateElectric(conn, send, What.PublicActFile, ElectricType.Filter, send.key);

        // the widest check for motivable
        if (Type.Seek(send.key.type).ecode == Lawsuit.ECode.Punishment && send.kind != Kind.Bulwark)
        {
            UpdateElectric(conn, send, What.PrivateMotiveFile, ElectricType.Text, send.key);

            if ((decision.flags & Decision.Public) != 0)
                UpdateElectric(conn, send, What.PublicMotiveFile, ElectricType.Filter, send.key);
        }

        return newGuid;
    }

    static int Kind2EndocWhat(char kind)
    {
        return Kind.Seek(kind).what >= What.Decision ? What.Decision : What.SessEndoc;
    }

    static Guid FetchEndocId(Connection conn, RCDKey key, char kind, int date)
    {
        EPortSend send = new EPortSend();
        Guid? result;

        send.key = key;
        send.kind = kind;
        send.date = date;
        send.what = Kind2EndocWhat(kind);
        result = FetchObjectId(conn, send);

        if (result == null)
        {
            if (send.what == What.Decision)
            {
                send.LogText("Auto-Send Decision", null);
                result = WriteDecision(conn, send, null);
            }
            else
            {
                send.LogText("Auto-Send SessEndoc", null);
                result = WriteSessEndoc(conn, send, null);
            }
        }

        return (Guid) result;
    }

    static void WriteActPreparator(Connection conn, EPortSend send, Guid? oldGuid)
    {
        ActPreparator preparator = new ActPreparator();

        preparator.ActPreparatorId = oldGuid;
        preparator.ActId = FetchEndocId(conn, send.key, send.kind, send.date);
        preparator.JudgeName = Citizen.GetFullName(conn, send.ucn, send.ucnType);

        switch (send.what)
        {
            case What.ActPreparatorPresident : preparator.Role = "председател"; break;
            case What.ActPreparatorJudge : preparator.Role = "докладчик"; break;
            default : preparator.Role = "член на състав"; break;
        }

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertActPreparator", null);
            newGuid = (Guid) proxy.InsertActPreparator(preparator);
        }
        else
        {
            send.LogText("proxy.UpdateActPreparator", oldGuid);
            newGuid = (Guid) proxy.UpdateActPreparator(preparator);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
    }

    static void WritePrivateActFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, Kind2EndocWhat(send.kind));

        if (parentGuid != null)
        {
            PrivateActFile file = new PrivateActFile();
            string mime;

            file.PrivateActContent = ReadElectricFile(conn, send, out mime);

            if (file.PrivateActContent != null)
            {
                file.PrivateActFileId = oldGuid;
                file.ActId = (Guid) parentGuid;
                file.PrivateActMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertPrivateActFile", null);
                    newGuid = (Guid) proxy.InsertPrivateActFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdatePrivateActFile", oldGuid);
                    newGuid = (Guid) proxy.UpdatePrivateActFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static void WritePublicActFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, Kind2EndocWhat(send.kind));

        if (parentGuid != null)
        {
            PublicActFile file = new PublicActFile();
            string mime;

            file.PublicActContent = ReadElectricFile(conn, send, out mime);

            if (file.PublicActContent != null)
            {
                file.PublicActFileId = oldGuid;
                file.ActId = (Guid) parentGuid;
                file.PublicActMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertPublicActFile", null);
                    newGuid = (Guid) proxy.InsertPublicActFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdatePublicActFile", oldGuid);
                    newGuid = (Guid) proxy.UpdatePublicActFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static void WritePrivateMotiveFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, What.Decision);

        if (parentGuid != null)
        {
            PrivateMotiveFile file = new PrivateMotiveFile();
            string mime;

            file.PrivateMotiveContent = ReadElectricFile(conn, send, out mime);

            if (file.PrivateMotiveContent != null)
            {
                file.PrivateMotiveFileId = oldGuid;
                file.ActId = (Guid) parentGuid;
                file.PrivateMotiveMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertPrivateMotiveFile", null);
                    newGuid = (Guid) proxy.InsertPrivateMotiveFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdatePrivateMotiveFile", oldGuid);
                    newGuid = (Guid) proxy.UpdatePrivateMotiveFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static void WritePublicMotiveFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, What.Decision);

        if (parentGuid != null)
        {
            PublicMotiveFile file = new PublicMotiveFile();
            string mime;

            file.PublicMotiveContent = ReadElectricFile(conn, send, out mime);

            if (file.PublicMotiveContent != null)
            {
                file.PublicMotiveFileId = oldGuid;
                file.ActId = (Guid) parentGuid;
                file.PublicMotiveMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertPublicMotiveFile", null);
                    newGuid = (Guid) proxy.InsertPublicMotiveFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdatePublicMotiveFile", oldGuid);
                    newGuid = (Guid) proxy.UpdatePublicMotiveFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static SideWind FetchAppealerSide(Connection conn, RCDKeyContainer container)
    {
        const string fields = "F_UCN, F_UCN_TYPE, F_INVOLVEMENT, F_STATE";

        using (Query q = new SideWindQuery(conn, fields, container.key, container.kind, container.date))
        {
            SideWind sideWind = new SideWind();

            while (q.Read())
            {
                sideWind.GetData(q, fields);

                if (sideWind.Appealer())
                    return sideWind;
            }
        }

        throw new GetDataException(container.Name + " -> APPEALER");
    }

    static Guid FetchAppealerId(Connection conn, InReg inReg)
    {
        SideWind sideWind = FetchAppealerSide(conn, inReg);
        EPortSend resend = new EPortSend();

        resend.what = What.SideWind;
        resend.key = inReg.lawsuit;
        resend.ucn = sideWind.ucn;
        resend.ucnType = sideWind.ucnType;
        resend.involvement = sideWind.involvement;
        return FetchSideWindId(conn, resend);
    }

    static void WriteInReg(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Appeal appeal = new Appeal();
        InReg inReg = new InReg();

        appeal.AppealId = oldGuid;
        inReg.key = send.key;
        inReg.Get(conn, null);
        appeal.ActId = FetchEndocId(conn, inReg.lawsuit, send.text == "1" ? inReg.sessionKind : inReg.decisionKind,
            inReg.decisionDate);
        appeal.AppealKindCode = Kind.Seek(inReg.kind).xfer.ToString();
        appeal.SideId = FetchAppealerId(conn, inReg);
        appeal.DateFiled = IntDate.ToDate(inReg.date);

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertAppeal", null);
            newGuid = (Guid) proxy.InsertAppeal(appeal);
        }
        else
        {
            send.LogText("proxy.UpdateAppeal", oldGuid);
            newGuid = (Guid) proxy.UpdateAppeal(appeal);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
    }

    static void WriteOutReg(Connection conn, EPortSend send, Guid? oldGuid)
    {
        OutgoingDocument outgoing = new OutgoingDocument();
        OutReg outReg = new OutReg();

        outReg.key = send.key;
        outReg.Get(conn, null);
        outgoing.CaseId = FetchLawsuitId(conn, outReg.lawsuit);

        if (outReg.receiverType == UcnType.Reserved)
        {
            SideWind sideWind = new SideWind();
            const string fields = "F_UCN, F_UCN_TYPE";

            using (Query q = new SideWindQuery(conn, fields, outReg.key, outReg.kind, outReg.date))
            {
                while (q.Read())
                {
                    sideWind.GetData(q, fields);
                    outgoing.Person = TryCitizen(conn, sideWind.ucn, sideWind.ucnType);

                    if (outgoing.Person != null)
                        break;

                    outgoing.Entity = TryFirm(conn, sideWind.ucn, sideWind.ucnType);

                    if (outgoing.Entity != null)
                        break;
                }
            }
        }
        else
        {
            Sender sender = Sender.Seek(conn, outReg.receiver, outReg.receiverType);

            outgoing.Entity = new Entity();
            outgoing.Entity.Name = sender.name;
            outgoing.Entity.Address = AnyAddress.GetFullText(conn, outReg.receiver, outReg.receiverType);
        }
        
        if (outgoing.Person == null && outgoing.Entity == null)
        {
            Log.Error("Failed to fill Person or Entity");
            send.Delete(conn, true);
        }
        else
        {
            outgoing.OutgoingDate = IntDate.ToDate(outReg.date);
            outgoing.OutgoingDocumentId = oldGuid;
            outgoing.OutgoingDocumentTypeCode = Kind.Seek(outReg.kind).xfer.ToString();
            outgoing.OutgoingNumber = send.key.no;

            Guid newGuid;

            if (oldGuid == null)
            {
                send.LogText("proxy.InsertOutgoingDocument", null);
                newGuid = (Guid) proxy.InsertOutgoingDocument(outgoing);
            }
            else
            {
                send.LogText("proxy.UpdateOutgoingDocument", oldGuid);
                newGuid = (Guid) proxy.UpdateOutgoingDocument(outgoing);
            }

            WriteSuccess(conn, send, oldGuid, newGuid);
            UpdateElectric(conn, send, outReg.kind, outReg.date, What.OutRegFile, ElectricType.Text, outReg.lawsuit);
        }
    }

    static void WriteOutgoingFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, What.OutReg);

        if (parentGuid != null)
        {
            OutgoingDocumentFile file = new OutgoingDocumentFile();
            string mime;

            file.OutgoingDocumentContent = ReadElectricFile(conn, send, out mime);

            if (file.OutgoingDocumentContent != null)
            {
                file.OutgoingDocumentFileId = oldGuid;
                file.OutgoingDocumentId = (Guid) parentGuid;
                file.OutgoingDocumentMimeType = mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertOutgoingDocumentFile", null);
                    newGuid = (Guid) proxy.InsertOutgoingDocumentFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdateOutgoingDocumentFile", oldGuid);
                    newGuid = (Guid) proxy.UpdateOutgoingDocumentFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    // similar to WriteRequest... may be partially combined
    static Guid WriteSurround(Connection conn, EPortSend send, Guid? oldGuid)
    {
        IncomingDocument document = new IncomingDocument();
        Surround surround = new Surround();

        document.IncomingDocumentId = oldGuid;
        surround.key = send.key;
        surround.Get(conn, "F_KIND, F_DATE, F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE");

        document.CaseId = FetchLawsuitId(conn, surround.lawsuit);
        document.CourtCode = Const.CourtCode.ToString();
        document.IncomingNumber = send.key.no;
        document.IncomingDate = IntDate.ToDate(surround.date);
        document.IncomingDocumentTypeCode = Kind.Seek(surround.kind).xfer.ToString();

        //document.Person = null;
        // 2018-10-08: LPR: fill person or entity
        SideWind appealer = FetchAppealerSide(conn, surround);

        document.Person = TryCitizen(conn, appealer.ucn, appealer.ucnType);

        if (document.Person == null)
            document.Entity = TryFirm(conn, appealer.ucn, appealer.ucnType);

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertIncomingDocument", null);
            newGuid = (Guid) proxy.InsertIncomingDocument(document);
        }
        else
        {
            send.LogText("proxy.UpdateIncomingDocument", oldGuid);
            newGuid = (Guid) proxy.UpdateIncomingDocument(document);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        UpdateElectric(conn, send, surround.kind, surround.date, What.SurroundFile, ElectricType.Image, surround.key);
        return newGuid;
    }

    static void WriteSubpoena(Connection conn, EPortSend send, Guid? oldGuid, Guid parentId)
    {
        EPortSend user = new EPortSend(RCDKey.Empty, send.ucn, send.ucnType, Involvement.RegisterWhat(send.involvement));
        Guid? userId = FetchObjectId(conn, user);

        if (userId != null)
        {
            Summon summon = new Summon();
            Subpoena subpoena = new Subpoena();
            EPortSend resend = new EPortSend();

            resend.what = What.SideWind;
            subpoena.key = resend.key = send.key;
            subpoena.kind = send.kind;
            subpoena.date = send.date;
            subpoena.ucn = resend.ucn = send.ucn;
            subpoena.ucnType = resend.ucnType = send.ucnType;
            subpoena.involvement = resend.involvement = send.involvement;
            subpoena.eventStatus = send.eventStatus;
            subpoena.subpoenaNo = send.subpoenaNo;
            subpoena.Get(conn, null);

            summon.SummonId = oldGuid;
            summon.ParentId = parentId;
            summon.SideId = FetchSideWindId(conn, resend);
            summon.SummonKind = SubpoenaKind.Seek(subpoena.subpoenaKind).name;
            summon.SummonTypeCode = (send.what - What.SubpoenaBase).ToString();
            summon.DateCreated = IntDate.ToDate(subpoena.subpoenaDate);

            if (subpoena.subpoenaReceived != 0)
                summon.DateServed = IntDate.ToDate(subpoena.subpoenaReceived);

            summon.Addressee = send.text;
            summon.Address = subpoena.GetAddress().GetFullText(conn);
            summon.Subject = summon.SummonKind;  // ??

            Guid newGuid;

            if (oldGuid == null)
            {
                send.LogText("proxy.InsertSummon", null);
                newGuid = (Guid) proxy.InsertSummon(summon, (Guid) userId);
            }
            else
            {
                send.LogText("proxy.UpdateSummon", oldGuid);
                newGuid = (Guid) proxy.UpdateSummon(summon);
            }

            WriteSuccess(conn, send, oldGuid, newGuid);
            UpdateElectric(conn, send, send.what - What.SubpoenaBase + What.SubpoenaFileBase, ElectricType.Text, send.key);
        }
        else
        {
            user.LogText("guid not found for", null);
            send.Delete(conn, true);
        }
    }

    static void WriteEndocMessage(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid parentId = FetchEndocId(conn, send.key, send.kind, send.date);

        WriteSubpoena(conn, send, oldGuid, parentId);
    }

    static void WriteAppealMessage(Connection conn, EPortSend send, Guid? oldGuid)
    {
        InReg inReg = new InReg();
        Guid parentId;

        inReg.key = send.key;
        inReg.Get(conn, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE, F_DECISION_KIND, F_DECISION_DATE");
        parentId = FetchEndocId(conn, inReg.lawsuit, inReg.decisionKind, inReg.decisionDate);
        WriteSubpoena(conn, send, oldGuid, parentId);
    }

    static void WriteSessionMessage(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid parentId = FetchSessionId(conn, send.key, send.kind, send.date);

        WriteSubpoena(conn, send, oldGuid, parentId);
    }

    static void WriteSummonFile(Connection conn, EPortSend send, Guid? oldGuid)
    {
        Guid? parentGuid = TryElectricParentGuid(conn, send, send.what - What.SubpoenaFileBase + What.SubpoenaBase);

        if (parentGuid != null)
        {
            SummonFile file = new SummonFile();
            string mime;

            file.Content = ReadElectricFile(conn, send, out mime);

            if (file.Content != null)
            {
                file.SummonFileId = oldGuid;
                file.SummonId = (Guid) parentGuid;
                file.MimeType= mime;

                Guid newGuid;

                if (oldGuid == null)
                {
                    send.LogText("proxy.InsertSummonFile", null);
                    newGuid = (Guid) proxy.InsertSummonFile(file);
                }
                else
                {
                    send.LogText("proxy.UpdateSummonFile", oldGuid);
                    newGuid = (Guid) proxy.UpdateSummonFile(file);
                }

                WriteSuccess(conn, send, oldGuid, newGuid);
            }
        }
    }

    static DateTime Ucn2DateTime(string ucn)
    {
        int year = Convert.ToInt32(ucn.Substring(0, 2));
        int month = Convert.ToInt32(ucn.Substring(2, 2));
        int day = Convert.ToInt32(ucn.Substring(4, 2));

        if (month >= 40)
        {
            month -= 40;
            year += 2000;
        }
        else if (month >= 20)
        {
            month -= 20;
            year += 1800;
        }
        else
            year += 1900;

        return new DateTime(year, month, day);
    }

    static string GetEMail(Connection conn, string ucn, char ucnType)
    {
        SqlSelect select = new SqlSelect("T_SIDE_WIND W JOIN T_INREG I ON I.F_NO = W.F_NO AND I.F_YEAR = W.F_YEAR " +
            "AND I.F_TYPE = W.F_TYPE JOIN T_INREG_EVENT IE ON IE.F_NO = W.F_NO AND IE.F_YEAR = W.F_YEAR " +
            "AND IE.F_TYPE = W.F_TYPE", "I.F_EPORTAL_MAIL");

        select.AddString("W.F_UCN", ucn);
        select.AddChar("W.F_UCN_TYPE", ucnType);
        select.AddChar("W.F_TYPE", Type.InReg);
        select.AddString("NOT I.F_EPORTAL_MAIL", null);
        select.AddChar("IE.F_RESULT", Const.ResultEPortalAllowed);

        using (Query q = new Query(conn, select))
        {
            if (q.Read())
                return q.GetString("F_EPORTAL_MAIL");
        }

        return null;
    }

    static Guid WritePersonRegistration(Connection conn, EPortSend send, Guid? oldGuid)
    {
        PersonRegistration person = new PersonRegistration();
        Citizen citizen = Citizen.GetForEP(conn, send.ucn, send.ucnType);

        person.PersonRegistrationId = oldGuid;
        person.Name = citizen.GetFullName();
        person.EGN = citizen.ucn;
        person.Address = AnyAddress.GetFullText(conn, send.ucn, send.ucnType);
        person.Email = GetEMail(conn, send.ucn, send.ucnType);
        person.BirthDate = Ucn2DateTime(citizen.ucn);

        if (person.Email == null)
            throw new GetDataException("T_OUTREG.F_EPORTAL_MAIL");

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertPersonRegistration", null);
            newGuid = (Guid) proxy.InsertPersonRegistration(person);
        }
        else
        {
            send.LogText("proxy.UpdatePersonRegistration", oldGuid);
            //newGuid = (Guid) proxy.UpdatePersonRegistration(person);
            throw new NotImplementedException("proxy.UpdatePersonRegistration");
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
        return newGuid;
    }

    static Guid FetchLawyerByNumber(Connection conn, EPortSend send, KnownPerson person)
    {
        if (person.personalNo == "")
            throw new GetDataException("T_KNOWN_PERSON.F_PERSONAL_NO");

        send.LogText("proxy.GetLawyerByNumber", null);

        Lawyer lawyer = proxy.GetLawyerByNumber(person.personalNo);
        Guid guid;

        if (lawyer != null && lawyer.LawyerId != null)
        {
            guid = (Guid) lawyer.LawyerId;
            send.LogReceived(null, guid);
        }
        else
        {
            guid = EPortSend.NewGuid(conn);
            send.LogText("Not found, generated", guid);
        }

        return guid;
    }

    static void WriteLawyerRegistration(Connection conn, EPortSend send, Guid? oldGuid)
    {
        LawyerRegistration lawyer = new LawyerRegistration();
        Citizen citizen = Citizen.GetForEP(conn, send.ucn, send.ucnType);
        KnownPerson person = KnownPerson.Seek(conn, send.ucn, send.ucnType, Involvement.Legalese);

        lawyer.LawyerRegistrationId = oldGuid;
        lawyer.LawyerId = FetchLawyerByNumber(conn, send, person);
        lawyer.Email = GetEMail(conn, send.ucn, send.ucnType);

        if (citizen.ucn == null || citizen.ucn.StartsWith("0000"))
            lawyer.BirthDate = person.birthDate;
        else
            lawyer.BirthDate = Ucn2DateTime(citizen.ucn);

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertLawyerRegistration", null);
            newGuid = (Guid) proxy.InsertLawyerRegistration(lawyer);
        }
        else
        {
            send.LogText("proxy.UpdateLawyerRegistration", oldGuid);
            //newGuid = (Guid) proxy.UpdateLawyerRegistration(lawyer);
            throw new NotImplementedException("proxy.UpdateLawyerRegistration");
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
    }

    static Guid FetchPersonId(Connection conn, EPortSend send)
    {
        EPortSend person = new EPortSend(RCDKey.Empty, send.ucn, send.ucnType, What.PersonRegistration);
        Guid? guid = FetchObjectId(conn, person);

        if (guid == null)
            throw new GetDataException("T_EPORT_GUID.F_PERSON_GUID");

        return (Guid) guid;
    }

    static void WritePersonAssignment(Connection conn, EPortSend send, Guid? oldGuid)
    {
        PersonAssignment assign = new PersonAssignment();
        EPortSend resend = new EPortSend(send, What.SideWind, ElectricType.None);

        assign.PersonAssignmentId = oldGuid;
        assign.Date = DateTime.Now;
        assign.SideId = FetchSideWindId(conn, resend);
        assign.PersonRegistrationId = FetchPersonId(conn, send);
        assign.IsActive = true;

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertPersonAssignment", null);
            newGuid = (Guid) proxy.InsertPersonAssignment(assign);
        }
        else
        {
            send.LogText("proxy.UpdatePersonAssignment", oldGuid);
            newGuid = (Guid) proxy.UpdatePersonAssignment(assign);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
    }

    static Guid FetchLawyerId(Connection conn, EPortSend send)
    {
        EPortSend lawyer = new EPortSend(RCDKey.Empty, send.ucn, send.ucnType, What.LawyerRegistration);
        Guid? guid = FetchObjectId(conn, lawyer);

        if (guid == null)
            throw new GetDataException("T_EPORT_GUID.F_LAWYER_GUID");

        return (Guid) guid;
    }

    static void WriteLawyerAssignment(Connection conn, EPortSend send, Guid? oldGuid)
    {
        LawyerAssignment assign = new LawyerAssignment();
        EPortSend resend = new EPortSend(send, What.SideWind, ElectricType.None);

        assign.LawyerAssignmentId = oldGuid;
        assign.Date = DateTime.Now;
        assign.SideId = FetchSideWindId(conn, resend);
        assign.LawyerRegistrationId = FetchLawyerId(conn, send);
        assign.IsActive = true;

        Guid newGuid;

        if (oldGuid == null)
        {
            send.LogText("proxy.InsertLawyerAssignment", null);
            newGuid = (Guid) proxy.InsertLawyerAssignment(assign);
        }
        else
        {
            send.LogText("proxy.UpdateLawyerAssignment", oldGuid);
            newGuid = (Guid) proxy.UpdateLawyerAssignment(assign);
        }

        WriteSuccess(conn, send, oldGuid, newGuid);
    }

    static void SendRedirect(Connection conn, EPortSend send, Guid? guid)
    {
        if (send.oper == Oper.Write)
        {
            switch (send.what)
            {
                case What.Request : WriteRequest(conn, send, guid); break;
                case What.RequestFile : WriteIncomingFile(conn, send, guid); break;
                case What.Lawsuit : WriteLawsuit(conn, send, guid); break;
                case What.SideWind : WriteSideWind(conn, send, guid); break;
                case What.Session : WriteSession(conn, send, guid); break;
                case What.ParticipantPresident :
                case What.ParticipantReJudgeX :
                case What.ParticipantJudgeX :
                case What.ParticipantJudge : WriteParticipant(conn, send, guid); break;
                case What.PrivateProtocolFile : WritePrivateProtocolFile(conn, send, guid); break;
                case What.PublicProtocolFile : WritePublicProtocolFile(conn, send, guid); break;
                case What.SessEndoc : WriteSessEndoc(conn, send, guid); break;
                case What.Decision : WriteDecision(conn, send, guid); break;
                case What.ActPreparatorPresident :
                case What.ActPreparatorReJudgeX :
                case What.ActPreparatorJudgeX : 
                case What.ActPreparatorJudge : WriteActPreparator(conn, send, guid); break;
                case What.PrivateActFile : WritePrivateActFile(conn, send, guid); break;
                case What.PublicActFile : WritePublicActFile(conn, send, guid); break;
                case What.PrivateMotiveFile : WritePrivateMotiveFile(conn, send, guid); break;
                case What.PublicMotiveFile : WritePublicMotiveFile(conn, send, guid); break;
                case What.InReg : WriteInReg(conn, send, guid); break;
                case What.OutReg : WriteOutReg(conn, send, guid); break;
                case What.OutRegFile : WriteOutgoingFile(conn, send, guid); break;
                case What.Surround : WriteSurround(conn, send, guid); break;
                case What.SurroundFile : WriteIncomingFile(conn, send, guid); break;
                case What.EndocMessage : WriteEndocMessage(conn, send, guid); break;
                case What.AppealMessage : WriteAppealMessage(conn, send, guid); break;
                //case What.LawsuitMessage :
                case What.SessionSubpoena : WriteSessionMessage(conn, send, guid); break;
                case What.EndocMessageFile :
                case What.AppealMessageFile :
                case What.SessionSubpoenaFile : WriteSummonFile(conn, send, guid); break;
                case What.PersonRegistration : WritePersonRegistration(conn, send, guid); break;
                case What.LawyerRegistration : WriteLawyerRegistration(conn, send, guid); break;
                case What.PersonAssignment : WritePersonAssignment(conn, send, guid); break;
                case What.LawyerAssignment : WriteLawyerAssignment(conn, send, guid); break;
                default : throw new NotImplementedException(String.Format("Write what={0} not implemented.", send.what));
            }
        }
        else  // delete
        {
            CleanupHandler cleanupHandler = null;
            DeleteHandler deleteHandler = null;
            int parentWhat = 0;  // 2018-10-01: LPR: for proper File deletion

            switch (send.what)
            {
                case What.Request :
                {
                    cleanupHandler = new CleanupHandler(CleanupRequest);
                    deleteHandler = new DeleteHandler(proxy.DeleteIncomingDocument);
                    break;
                }
                case What.RequestFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeleteIncomingDocumentFile);
                    parentWhat = What.Request;
                    break;
                }
                case What.SurroundFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeleteIncomingDocumentFile);
                    parentWhat = What.Surround;
                    break;
                }
                case What.Lawsuit : deleteHandler = new DeleteHandler(proxy.DeleteCase); break;
                case What.SideWind :
                {
                    cleanupHandler = new CleanupHandler(CleanupSideWind);
                    deleteHandler = new DeleteHandler(proxy.DeleteSide);
                    break;
                }
                case What.Session :
                {
                    cleanupHandler = new CleanupHandler(CleanupSession);
                    deleteHandler = new DeleteHandler(proxy.DeleteHearing);
                    break;
                }
                case What.ParticipantPresident :
                case What.ParticipantReJudgeX :
                case What.ParticipantJudgeX :
                case What.ParticipantJudge :
                {
                    deleteHandler = new DeleteHandler(proxy.DeleteHearingParticipant);
                    break;
                }
                case What.PrivateProtocolFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeletePrivateProtocolFile);
                    parentWhat = What.Session;
                    break;
                }
                case What.PublicProtocolFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeletePublicProtocolFile);
                    parentWhat = What.Session;
                    break;
                }
                case What.SessEndoc :
                {
                    cleanupHandler = new CleanupHandler(CleanupSessEndoc);
                    deleteHandler = new DeleteHandler(proxy.DeleteAct);
                    break;
                }
                case What.Decision :
                {
                    cleanupHandler = new CleanupHandler(CleanupDecision);
                    deleteHandler = new DeleteHandler(proxy.DeleteAct);
                    break;
                }
                case What.ActPreparatorPresident :
                case What.ActPreparatorReJudgeX :
                case What.ActPreparatorJudgeX : 
                case What.ActPreparatorJudge :
                {
                    deleteHandler = new DeleteHandler(proxy.DeleteActPreparator);
                    break;
                }
                case What.PrivateActFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeletePrivateActFile);
                    parentWhat = Kind2EndocWhat(send.kind);
                    break;
                }
                case What.PublicActFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeletePublicActFile);
                    parentWhat = Kind2EndocWhat(send.kind);
                    break;
                }
                case What.PrivateMotiveFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeletePrivateMotiveFile);
                    parentWhat = What.Decision;
                    break;
                }
                case What.PublicMotiveFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeletePublicMotiveFile);
                    parentWhat = What.Decision;
                    break;
                }
                case What.InReg :
                {
                    cleanupHandler = new CleanupHandler(CleanupInReg);
                    deleteHandler = new DeleteHandler(proxy.DeleteAppeal);
                    break;
                }
                case What.OutReg :
                {
                    cleanupHandler = new CleanupHandler(CleanupOutReg);
                    deleteHandler = new DeleteHandler(proxy.DeleteOutgoingDocument);
                    break;
                }
                case What.OutRegFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeleteOutgoingDocumentFile);
                    parentWhat = What.OutReg;
                    break;
                }
                case What.Surround :
                {
                    cleanupHandler = new CleanupHandler(CleanupSurround);
                    deleteHandler = new DeleteHandler(proxy.DeleteIncomingDocument);
                    break;
                }
                case What.EndocMessage : 
                case What.AppealMessage :
                //case What.LawsuitMessage :
                case What.SessionSubpoena :
                {
                    cleanupHandler = new CleanupHandler(CleanupSubpoena);
                    deleteHandler = new DeleteHandler(proxy.DeleteSummon);
                    break;
                }
                case What.EndocMessageFile :
                case What.AppealMessageFile :
                //case What.LawsuitMessageFile :
                case What.SessionSubpoenaFile :
                {
                    deleteHandler = new DeleteHandler(proxy.DeleteSummonFile);
                    parentWhat = send.what - What.SubpoenaFileBase + What.SubpoenaBase;
                    break;
                }
                case What.PersonAssignment : deleteHandler = new DeleteHandler(proxy.DeletePersonAssignment); break;
                case What.LawyerAssignment : deleteHandler = new DeleteHandler(proxy.DeleteLawyerAssignment); break;
                default : throw new NotImplementedException(String.Format("Delete what={0} not implemented.", send.what));
            }

            if (cleanupHandler != null)
                cleanupHandler(conn, send, guid);

            if (parentWhat != 0)
                DeleteElectric(conn, send, guid, parentWhat, deleteHandler);
            else
                DeleteObject(conn, send, guid, deleteHandler);
        }
    }
}
