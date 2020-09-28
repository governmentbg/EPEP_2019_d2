using System;
using System.Text;

public static class Tests
{
    private static readonly string[] TableNames = new string[]
    {
        // Nom
        "TArea", "TCollege", "TComposition", "TConfigName", "TConnectKind", "TConnectType", "TCountry", "TCourt",
        "TFineComposition", "THall", "TInvolvement", "TKind", "TKnownPerson", "TReceivedStatus", "TRegion", "TResolution",
        "TResult", "TStreet", "TSubject", "TSubpoenaKind", "TType", "TUCLP", "TUCNType", "TUser",
        // Doc
        "TAddress", "TAnnounce", "TAnnounceVisit", "TCitizen", "TConfigData", "TDecision", "TFirm", "TForeignAddress",
        "TInReg", "TInRegEvent", "TLawsuit", "TLinkWind", "TOutReg", "TRequest", "TSender", "TSession", "TSideWind",
        "TSubpoena", "TSubpoenaVisit"
    };

    private static void DumpKey(TGroup group, bool exact)
    {
        string keyName = exact ? "exact" : "float";
        SqlSelect select = new SqlSelect(group.Table, null);

        try
        {
            if (group.Table == "T_LINK_WIND")
                Console.Write("");

            if (exact)
                group.AddExact(select);
            else
                group.AddFloat(select);

            Console.WriteLine(String.Format("{0}: {1}: {2}", group.Table, keyName, select.ToString()));
        }
        catch (NotImplementedException)
        {
            Console.WriteLine("{0}: {1}: SELECT * FROM {0} [ni]", group.Table, keyName);
        }
        catch (Exception ex)
        {
            Console.WriteLine(String.Format("{0}: failed to add {1}", group.Table, keyName));
            Console.WriteLine(ex.ToString());
        }
    }

    private static void DumpKeys()
    {
        foreach (string tableName in TableNames)
        {
            try
            {
                Type type = Type.GetType(tableName);
                TGroup group = (TGroup) Activator.CreateInstance(type);

                DumpKey(group, true);
                DumpKey(group, false);
            }
            catch (Exception ex)
            {
                Console.WriteLine(tableName + ": failed to create group");
                Console.WriteLine(ex.ToString());
            }
        }

        TSubpoena subpoena = new TSubpoena();
        SqlSelect select = new SqlSelect(subpoena.Table, null);

        subpoena.AddRegard(select, false);
        Console.WriteLine("T_SUBPOENA: regard: {0}", select.ToString());

        select = new SqlSelect(subpoena.Table, null);
        subpoena.AddMulti(select);
        Console.WriteLine("T_SUBPOENA: multi: {0}", select.ToString());
    }

    public static void Main()
    {
        Console.OutputEncoding = Encoding.GetEncoding(1251);
        DumpKeys();
        Console.ReadKey();
    }
}
