using System;
using System.Text;

// TODO? CheckText returns text
// TODO? CheckName regex
// TODO? QuoteEscape
public abstract class SqlValueBuilder
{
    protected SqlValueBuilder()
    {
        firstName = true;
    }

    protected void CheckText(string text)
    {
        if (text != null && text.IndexOf('\'') != -1)
            throw new RangeException("SQL text '{0}' may not contain apostrophe(s).", text);
    }

    protected abstract void AppendValue(string name, string value);

    public override abstract string ToString();

    public static implicit operator string(SqlValueBuilder builder)
    {
        return builder.ToString();
    }
    
    public void AddString(string name, string s)
    {
        AppendValue(name, !String.IsNullOrEmpty(s) ? String.Format("'{0}'", s.Replace("'", "''")) : null);
    }

    public void AddChar(string name, char c)
    {
        AddString(name, c != '\0' ? c.ToString() : null);
    }

    public void AddInt(string name, int i)
    {
        AppendValue(name, i != 0 ? i.ToString() : null);
    }

    public void AddZInt(string name, int i)
    {
        AppendValue(name, i.ToString());
    }

    public void AddDouble(string name, double d)
    {
        AppendValue(name, d != 0.0 ? d.ToString() : null);
    }

    public void AddZDouble(string name, double d)
    {
        AppendValue(name, d.ToString());
    }

    public void AddDateTime(string name, DateTime stamp)
    {
        AddString(name, stamp != DateTime.MinValue ? String.Format("{0}.{1}.{2} {3}:{4}:{5}",
            stamp.Day, stamp.Month, stamp.Year, stamp.Hour, stamp.Minute, stamp.Second) : null);
    }

    protected bool firstName;
}

// TODO? AppendName check name
// TODO? AppendQuoted, AppendUnqouted
// TODO? AppendSeparator
public class SqlCriteriaBuilder : SqlValueBuilder
{
    protected SqlCriteriaBuilder(string format, params object[] args)
    {
        builder = new StringBuilder();
        builder.AppendFormat(format, args);
        nextOr = false;
    }

    protected virtual void AppendName(string name)
    {
        CheckText(name);
        builder.AppendFormat(" {0} {1} ", firstName ? "WHERE" : nextOr ? "OR" : "AND", name);
        firstName = false;
        nextOr = false;
    }

    public void AppendNameValue(string name, string value)
    {
        AppendName(name);
        builder.Append(value);
    }

    protected override void AppendValue(string name, string value)
    {
        AppendName(name);

        if (value != null)
            builder.AppendFormat("= {0}", value);
        else
            builder.Append("IS NULL");
    }

    public void AddLike(string name, string s)
    {
        AppendName(name);
        builder.AppendFormat("LIKE '{0}%'", s.Replace("'", "''"));
    }

    protected void AddCharSet(string set)
    {
        bool firstElem = true;

        if (set.Length == 0)
            throw new RangeException("Attempt to AddCharSet with an empty set.");

        foreach (char c in set)
        {
            builder.AppendFormat("{0}'{1}'", firstElem ? "IN (" : ", ", c != '\'' ? c.ToString() : "''");
            firstElem = false;
        }

        builder.Append(")");
    }

    public void AddCharSet(string name, string set)
    {
        AppendName(name);
        AddCharSet(set);
    }

    public void AddIntSet(string name, int[] set)
    {
        if (set.Length == 0)
            throw new RangeException("Attempt to AddIntSet with an empty set.");

        AppendName(name);

        bool firstElem = true;

        foreach (int i in set)
        {
            builder.AppendFormat("{0}{1}", firstElem ? "IN (" : ", ", i);
            firstElem = false;
        }

        builder.Append(")");
    }

    public void AddRange(string name, int min, int max)
    {
        AppendName(name);
        builder.AppendFormat("BETWEEN {0} AND {1}", min, max);
    }

    public override string ToString()
    {
        return builder.ToString();
    }

    protected StringBuilder builder;
    protected bool nextOr;
}

public class SqlSelect : SqlCriteriaBuilder
{
    public SqlSelect(string tables, string fields)
        : base("SELECT {0} FROM {1}", fields != null ? fields : "*", tables)
    {
        CheckText(tables);
        CheckText(fields);
    }

    public void AddIsNull(string name, bool isNull)
    {
        AppendName(isNull ? name : "NOT " + name);
        builder.Append("IS NULL");
    }

    public void AddOrder(string fields)
    {
        CheckText(fields);
        builder.AppendFormat(" ORDER BY {0}", fields);
    }

    public void CloseParen() { builder.Append(")"); }
    public void NextOr() { nextOr = true; }
}

public class SqlUpdate : SqlCriteriaBuilder
{
    public SqlUpdate(string table)
        : base("UPDATE {0}", table)
    {
        CheckText(table);
        wherePhase = false;
    }

    protected override void AppendName(string name)
    {
        if (wherePhase)
            base.AppendName(name);
        else
        {
            CheckText(name);
            builder.AppendFormat("{0} {1} ", firstName ? " SET" : ",", name);
            firstName = false;
        }
    }

    protected override void AppendValue(string name, string value)
    {
        if (wherePhase)
            base.AppendValue(name, value);
        else
        {
            AppendName(name);
            builder.AppendFormat("= {0}", value != null ? value : "NULL");
        }
    }

    public void Where()
    {
        if (wherePhase)
            throw new RangeException("Duplicate sql update Where call.");

        wherePhase = true;
        firstName = true;
    }

    private bool wherePhase;
}

public class SqlDelete : SqlCriteriaBuilder
{
    public SqlDelete(string table)
        : base("DELETE FROM {0}", table)
    {
        CheckText(table);
    }
}

public class SqlInsert : SqlValueBuilder
{
    public SqlInsert(string table)
    {
        insert = new StringBuilder();
        values = new StringBuilder();
        CheckText(table);
        insert.AppendFormat("INSERT INTO {0}", table);
        values.Append("VALUES");
    }

    public void AppendNameValue(string name, string value)
    {
        AppendValue(name, value);
    }

    protected override void AppendValue(string name, string value)
    {
        CheckText(name);
        insert.AppendFormat(firstName ? " ({0}" : ", {0}", name);
        values.AppendFormat(firstName ? " ({0}" : ", {0}", value != null ? value : "NULL");
        firstName = false;
    }

    public override string ToString()
    {
        return String.Format("{0}) {1})", insert, values);
    }

    private StringBuilder insert;
    private StringBuilder values;
}
