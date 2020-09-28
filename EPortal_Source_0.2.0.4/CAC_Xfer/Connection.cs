using System;
using System.Configuration;
using System.Data.Common;
using System.Data.Odbc;
using System.Data.OleDb;
using System.Text;
using System.Threading;

public class Connection : IDisposable
{
    public Connection() { Open(ProviderName, ConnectionString); }

    public Connection(string providerName, string connectionString) { Open(providerName, connectionString); }

    public static void Init(string SourceName)
    {
        ConnectionStringSettingsCollection settings = ConfigurationManager.ConnectionStrings;

        if (settings != null)
            foreach (ConnectionStringSettings css in settings)
                if (css.Name == SourceName)
                {
                    ProviderName = css.ProviderName;
                    ConnectionString = css.ConnectionString;
                }

        if (String.IsNullOrEmpty(ProviderName) || String.IsNullOrEmpty(ConnectionString))
            throw new RangeException("providerName and connectionString are mandatory");
    }

    public bool Execute(string command) { return new Command(this, command).Execute(); }
    public bool Execute(string format, params object[] args) { return Execute(String.Format(format, args)); }

    public int ExecCount(string command) { return new Command(this, command).ExecCount(); }
    public int ExecCount(string format, params object[] args) { return ExecCount(String.Format(format, args)); }

    public void Dispose()
    {
        Utility.Dispose(connection);
        connection = null;
    }

    public DbConnection Conn { get { return connection; } }

    protected DbConnection connection;

    private void Open(string providerName, string connectionString)
    {
        switch (providerName.ToLower())
        {
            case "lcpi.ibprovider" :
            {
                OleDbConnectionStringBuilder alias = new OleDbConnectionStringBuilder();

                alias.Provider = "LCPI.IBProvider";
                alias.Add("Location", connectionString);
                alias.Add("User ID", "????????");
                alias.Add("password", "????????");
                alias.Add("ctype", "win1251");
                alias.Add("auto_commit", true);
                connection = new OleDbConnection(alias.ToString());
                break;
            }
            case "odbc.firebird" :
            {
                OdbcConnectionStringBuilder alias = new OdbcConnectionStringBuilder();

                alias.Driver = "Firebird/InterBase(r) driver";
                alias.Add("DBNAME", connectionString);
                alias.Add("UID", "????????");
                alias.Add("PWD", "????????");
                alias.Add("CHARSET", "win1251");
                alias.Add("DIALECT", "1");
                connection = new OdbcConnection(alias.ToString());
                break;
            }
            case "odbc.postgres" :
            case "odbc.postgresql" :
            {
                OdbcConnectionStringBuilder alias = new OdbcConnectionStringBuilder();
                string[] words = connectionString.Split(':');

                if (words.Length < 1 || words.Length > 3)
                    throw new ArgumentException("PostgreSQL syntax: SERVER:DATABASE or SERVER:PORT:DATABASE");

                alias.Driver = "PostgreSQL ODBC Driver(ANSI)";
                alias.Add("Servername", words[0]);

                if (words.Length == 2)
                    alias.Add("Database", words[1]);
                else
                {
                    alias.Add("Port", words[1]);
                    alias.Add("Database", words[2]);
                }

                byte[] key = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                byte[] buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                    for (int i = 0; i < buffer.Length; i++)
                        buffer[i] ^= key[i % key.Length];

                alias.Add("Username", "????????");
                alias.Add("Password", "{" + Encoding.UTF8.GetString(buffer) + "}");
                connection = new OdbcConnection(alias.ToString());
                break;
            }
            default: throw new ArgumentException("The '" + providerName + "' provider is not supported.");
        }

        if (!Machine.Interactive)
        {
            for (int i = 1; i <= (Machine.HasEntryPoint ? 8 : 15); i++)
            {
                try
                {
                    connection.Open();
                    return;
                }
                catch (DbException)
                {
                }

                Thread.Sleep(i * 1000);
            }
        }

        connection.Open();
    }

    private static string
        ProviderName = null,
        ConnectionString = null;
}

public class Command : IDisposable
{
    public Command(Connection conn, string CommandText)
    {
        command = conn.Conn.CreateCommand();
        command.CommandText = CommandText;
    }

    public bool Execute()
    {
        int execute = command.ExecuteNonQuery();
        return execute > 0 || execute == -1;
    }

    public int ExecCount()
    {
        int execute = command.ExecuteNonQuery();
        return Math.Max(execute, 0);
    }

    public void Dispose()
    {
        Utility.Dispose(command);
        command = null;
    }

    protected DbCommand command;
}

public class Query : Command
{
    public Query(Connection conn, string select) : base(conn, select) { reader = command.ExecuteReader(); }

    public Query(Connection conn, string format, params object[] args) : base(conn, String.Format(format, args))
    {
        reader = command.ExecuteReader();
    }

    public bool Read() { return reader.Read(); }

    public int FieldCount { get { return reader.FieldCount; } }

    public string FieldName(int index) { return reader.GetName(index); }

    public char GetChar(string name)
    {
        return Utility.ToChar(name, GetString(name));
    }

    public string GetString(string name)
    {
        int index = reader.GetOrdinal(name);
        return reader.IsDBNull(index) ? "" : reader.GetString(index).Trim();
    }

    public int GetInt(string name)
    {
        int index = reader.GetOrdinal(name);
        return reader.IsDBNull(index) ? 0 : reader.GetInt32(index);
    }

    public string GetMoney(string name)
    {
        int index = reader.GetOrdinal(name);
        return reader.IsDBNull(index) ? "" : Utility.ToMoney(reader.GetDouble(index));
    }

    public double GetDouble(string name)
    {
        int index = reader.GetOrdinal(name);
        return reader.IsDBNull(index) ? 0.0 : reader.GetDouble(index);
    }

    public DateTime GetDateTime(string name)
    {
        int index = reader.GetOrdinal(name);
        return reader.IsDBNull(index) ? DateTime.MinValue : reader.GetDateTime(index);
    }

    public new void Dispose()
    {
        base.Dispose();
        Utility.Dispose(reader);
        reader = null;
    }

    protected DbDataReader reader;
}
