using System.Collections.Generic;

public static class TAliasCache
{
    public static void Init(Connection conn)
    {
        AreaCache = (new TArea()).LoadCache(conn, "F_AREA");
        CollegeCache = (new TCollege()).LoadCache(conn, "F_ORDER");
        CourtCache = (new TCourt()).LoadCache(conn, "F_COURT");
        CompositionCache = (new TComposition()).LoadCache(conn, "F_COLLEGE, F_COMPOSITION");
        ConnectKindCache = (new TConnectKind()).LoadCache(conn, "F_ORDER");
        ConnectTypeCache = (new TConnectType()).LoadCache(conn, "F_ORDER");
        CountryCache = (new TCountry()).LoadCache(conn, "F_NAME");
        HallCache = (new THall()).LoadCache(conn, "F_HALL");
        InvolvementCache = (new TInvolvement()).LoadCache(conn, "F_ORDER");
        KindCache = (new TKind()).LoadCache(conn, "F_ORDER");
        PostCache = (new TPost()).LoadCache(conn, "F_ORDER");
        ReceivedStatusCache = (new TReceivedStatus()).LoadCache(conn, "F_ORDER");
        RegionCache = (new TRegion()).LoadCache(conn, "F_AREA, F_REGION");
        ResolutionCache = (new TResolution()).LoadCache(conn, "F_ORDER");
        ResultCache = (new TResult()).LoadCache(conn, "F_ORDER");
        SubjectCache = (new TSubject()).LoadCache(conn, "F_TYPE, F_SUBJECT");
        SubpoenaKindCache = (new TSubpoenaKind()).LoadCache(conn, "F_ORDER");
        TypeCache = (new TType()).LoadCache(conn, "F_ORDER");
        UCNTypeCache = (new TUCNType()).LoadCache(conn, "F_ORDER");
    }

    public static List<TAlias> AreaCache;
    public static List<TAlias> CollegeCache;
    public static List<TAlias> CourtCache;
    public static List<TAlias> CompositionCache;
    public static List<TAlias> ConnectKindCache;
    public static List<TAlias> ConnectTypeCache;
    public static List<TAlias> CountryCache;
    public static List<TAlias> HallCache;
    public static List<TAlias> InvolvementCache;
    public static List<TAlias> KindCache;
    public static List<TAlias> PostCache;
    public static List<TAlias> ReceivedStatusCache;
    public static List<TAlias> RegionCache;
    public static List<TAlias> ResolutionCache;
    public static List<TAlias> ResultCache;
    public static List<TAlias> SubjectCache;
    public static List<TAlias> SubpoenaKindCache;
    public static List<TAlias> TypeCache;
    public static List<TAlias> UCNTypeCache;
}
