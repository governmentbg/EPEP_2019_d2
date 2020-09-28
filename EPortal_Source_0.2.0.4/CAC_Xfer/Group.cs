using System;
using System.Collections;

// deprecated, use TGroup instead
public class GetDataException : Exception
{
    public GetDataException(string source)
        : base(String.Format("Error reading data from {0}.", source))
    {
    }
}

public abstract class Group
{
    public Group(string Table)
    {
        this.Table = Table;
        this.fields = "*";
    }

    public Group(string Table, string fields)
    {
        this.Table = Table;
        this.fields = fields;
    }

    public Criteria BeginCriteria()
    {
        return new Criteria(Table, fields);
    }

    public void Delete(Connection conn)
    {
        string delete = ExactCriteria().ToString();

        delete = "DELETE " + delete.Substring(delete.IndexOf("FROM"));
        conn.Execute(delete);
    }

    public abstract Criteria ExactCriteria();

    public virtual Criteria FloatCriteria() { return BeginCriteria(); }

    public Query FloatQuery(Connection Conn) { return new Query(Conn, FloatCriteria().ToString()); }

    public void Get(Connection conn)
    {
        if (!Try(conn))
            throw new GetDataException(Table);
    }

    public abstract void GetData(Query q);

    public void LoadArray(Connection conn, string s, IList array)
    {
        Query q = null;

        try
        {
            q = new Query(conn, s);

            while (q.Read())
            {
                Group group = NewGroup();
                group.GetData(q);
                array.Add(group);
            }
        }
        finally
        {
            Utility.Dispose(q);
        }
    }

    public void LoadFloat(Connection conn, IList array)
    {
        Criteria criteria = FloatCriteria();
        LoadArray(conn, criteria.ToString(), array);
    }

    public Group NewGroup()
    {
        return (Group) Activator.CreateInstance(GetType());
    }

    protected void Seek(Connection conn) { Get(conn); }

    public virtual bool Try(Connection conn)
    {
        Criteria criteria = ExactCriteria();
        Query q = null;

        try
        {
            q = new Query(conn, criteria.ToString());
            if (q.Read())
            {
                GetData(q);
                return true;
            }
        }
        finally
        {
            Utility.Dispose(q);
        }

        return false;
    }

    public readonly string Table;
    protected string fields;
}
