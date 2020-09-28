using System;
using System.Text;

// deprecated, use SqlSelect instead
public class Criteria
{
    public Criteria(string table, string fields)
    {
        select = new StringBuilder();
        AppendFormat("SELECT {0} FROM {1}", fields, table);
        first = true;
    }

    public void Append(string s) { select.Append(s); }

    public void AppendFormat(string format, object arg0)
    {
        select.AppendFormat(format, arg0);
    }

    public void AppendFormat(string format, params object[] args)
    {
        select.AppendFormat(format, args);
    }

    public void AddField(string name)
    {
        AppendFormat(" {0} {1} ", first ? "WHERE" : "AND", name);
        first = false;
    }

    public void AddString(string name, string s)
    {
        AddField(name);
        AppendFormat("= '{0}'", s);
    }

    public void AddChar(string name, char c) { AddString(name, c.ToString()); }

    public void AddInt(string name, int i)
    {
        AddField(name);
        AppendFormat("= {0}", i);
    }

    public void AddDateTime(string name, DateTime stamp)
    {
        AddString(name, String.Format("{0}.{1}.{2} {3}:{4}:{5}", stamp.Day, stamp.Month, stamp.Year, stamp.Hour,
            stamp.Minute, stamp.Second));
    }
    
    public void AddLike(string name, string s)
    {
        AddField(name);
        AppendFormat("LIKE '{0}%'", s);
    }

    public void AddSet(string name, string set)
    {
        AddField(name);
        Utility.AddSet(select, set);
    }

    public void AddOption(string name, string s)
    {
        if (s.Length == 0)
        {
            AddField(name);
            Append("IS NULL");
        }
        else
            AddString(name, s);
    }

    public override string ToString()
    {
        return select.ToString();
    }

    protected StringBuilder select;
    private bool first;
}
