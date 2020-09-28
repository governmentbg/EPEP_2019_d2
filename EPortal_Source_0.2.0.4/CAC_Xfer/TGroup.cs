using System;
using System.Collections;
using System.Collections.Generic;

public class TGroup
{
    public TGroup(string Table)
    {
        this.Table = Table;
        fldList = new List<TField>();
    }

    public readonly string Table;

    public void Add(TField field)
    {
        if (Find(field.Name) != null)
            throw new RangeException("Duplicate field name {0}.", field.Name);

        fldList.Add(field);
    }

    public virtual void AddExact(SqlCriteriaBuilder builder)
    {
        AddFields(builder, ExactFields());
    }

    public virtual void AddFloat(SqlCriteriaBuilder builder)
    {
        AddFields(builder, FloatFields());
    }

    public void AddFields(SqlCriteriaBuilder builder, string fields)
    {
        foreach (string name in SplitCommas(fields))
            Get(name).Add(builder);
    }

    public virtual void Delete(Connection conn, bool exact)
    {
        SqlDelete delete = new SqlDelete(Table);

        if (exact)
            AddExact(delete);
        else
            AddFloat(delete);

        conn.Execute(delete);
    }

    protected virtual string ExactFields()
    {
        throw new NotImplementedException(Table + ":ExactFields");
    }

    protected virtual string FloatFields()
    {
        throw new NotImplementedException(Table + ":FloatFields");
    }

    public TField Find(string name)
    {
        return fldList.Find(f => f.Name == name);
    }

    public TField Get(string name)
    {
        TField field = Find(name);

        if (field == null)
            throw new RangeException("Invalid field name {0}", name);

        return field;
    }

    public void Get(Connection conn, string fields)
    {
        if (!Try(conn, fields))
            throw new GetDataException(Table);
    }

    public void Copy(TGroup group, string fields)
    {
        foreach (TField field in ParseFields(fields))
            field.Get(group.Get(field.Name));
    }

    public void Get(Query q, string fields)
    {
        foreach (TField field in ParseFields(fields))
        {
            try
            {
                field.Get(q);
            }
            catch (IndexOutOfRangeException)
            {
                if (fields != null && fields != "*")
                    throw;
            }
        }
    }

    public virtual void Insert(Connection conn, string fields)
    {
        SqlInsert insert = new SqlInsert(Table);

        foreach (TField field in ParseFields(fields))
            field.Add(insert);

        conn.Execute(insert);
    }

    public void LoadArray(Connection conn, string select, IList array)
    {
        using (Query q = new Query(conn, select))
        {
            while (q.Read())
            {
                TGroup group = NewGroup();

                group.Get(q, null);
                array.Add(group);
            }
        }
    }

    public void LoadFloat(Connection conn, IList array)
    {
        SqlSelect select = new SqlSelect(Table, null);

        AddFloat(select);
        LoadArray(conn, select, array);
    }

    public TGroup NewGroup()
    {
        return (TGroup) Activator.CreateInstance(GetType());
    }

    protected List<TField> ParseFields(string fields)
    {
        if (fields != null && fields != "*")
        {
            List<TField> result = new List<TField>();

            foreach (string name in SplitCommas(fields))
                result.Add(Get(name));

            return result;
        }

        return fldList;
    }

    protected string[] SplitCommas(string fields)
    {
        return fields.Replace(" ", "").Split(',');
    }

    public virtual bool Try(Connection conn, string fields)
    {
        SqlSelect select = new SqlSelect(Table, fields);

        AddExact(select);

        using (Query q = new Query(conn, select))
        {
            if (q.Read())
            {
                Get(q, fields);
                return true;
            }
        }

        return false;
    }

    public virtual bool Update(Connection conn, string fields)
    {
        SqlUpdate update = new SqlUpdate(Table);
        string[] keys = SplitCommas(ExactFields());

        foreach (TField field in ParseFields(fields))
            if (Array.FindIndex(keys, key => key == field.Name) == -1)
                field.Add(update);

        update.Where();
        AddExact(update);
        return conn.Execute(update);
    }

    protected uint CombinedFlags;
    private List<TField> fldList;
}
