using System;

static class UcnType
{
    public const char
        CitizenUcn = 'c',
        Self = 'b',
        Version = 'B',
        Reserved = 'z';

    public const string
        Citizens = "cC",
        Firms = "fF";
}

static class Status
{
    public const char
        Send = 'S',
        Wait = 'W',
        Error = 'E';

    public const string Sendables = "SW";
}

static class What
{
    public const int
        Factor = 100,
        Request = 100,
        Lawsuit = 200,
        SideWind = 300,
        Session = 400,
        OpenSession = 400,
        SessEndoc = 450,
        Decision = 500,
        InReg = 800,
        OutReg = 900,
        Surround = 1000,
        SubpoenaBase = 5000,
        EndocMessage = 5001,
        AppealMessage = 5002,
        // see CAC eportal.cpp
        //LawsuitMessage = 5003,
        SessionSubpoena = 5004,
        RequestFile = 10000,
        SurroundFile = 10100,
        PersonRegistration = 10200,
        LawyerRegistration = 10201,
        ParticipantPresident = 10300,
        ParticipantReJudgeX = 10301,
        ParticipantJudgeX = 10302,
        ParticipantJudge = 10303,
        ActPreparatorPresident = 10400,
        ActPreparatorReJudgeX = 10401,
        ActPreparatorJudgeX = 10402,
        ActPreparatorJudge = 10403,
        PrivateProtocolFile = 10500,
        PublicProtocolFile = 10501,
        PrivateActFile = 10600,
        PublicActFile = 10601,
        MotiveHigh = 107,
        PrivateMotiveFile = 10700,
        PublicMotiveFile = 10701,
        OutRegFile = 10800,
        SubpoenaFileBase = 10900,
        EndocMessageFile = 10901,
        AppealMessageFile = 10902,
        //LawsuitMessageFile = 10903,
        SessionSubpoenaFile = 10904,
        PersonAssignment = 11000,
        LawyerAssignment = 11001;
}

static class Oper
{
    public const char
        Write = 'w',
        Delete = 'd';
}

static class ElectricType
{
    public const char
        None = '\0',
        Text = '0',
        Image = '1',
        Filter = '2';

    public const int Count = 3;
}

static class CourtType
{
    public const char
        Appeal = '3',
        Admin = '2',
        Areal = '5',
        Military = '6',
        Regional = '7',
        MilitaryAppeal = '9';
}

static class Ucn
{
    public static bool IsEmpty(this string ucn)
    {
        return String.IsNullOrEmpty(ucn) || ucn == Ucn.Empty;
    }

    private const string Empty = "0000000000";
}
