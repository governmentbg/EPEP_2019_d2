using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class TGroup
{
    public TGroup(string Name)
    {
        this.Name = Name;
        CombinedFlags = 0;
        fldList = new List<TField>();
    }

    public readonly string Name;

    public void Add(TField field)
    {
        if (TryFind(field.Name) != null)
            throw new RangeException("Duplicate field name {0}.", field.Name);

        fldList.Add(field);
        CombinedFlags |= field.Flags;
    }

    public virtual void AddExact(SqlBuilder builder)
    {
        foreach (TField field in ParseFields(null, TField.ExactKey, TField.ExactKey))
            field.Add(builder);
    }

    public virtual void AddFloat(SqlBuilder builder) { }

    public void AddFields(SqlValueBuilder builder, string fields)
    {
        foreach (string name in SplitCommas(fields))
            Find(name).Add(builder);
    }

    public virtual void Delete(Connection conn, bool exact)
    {
        SqlDelete delete = new SqlDelete(Name);

        if (exact)
            AddExact(delete);
        else
            AddFloat(delete);

        conn.Execute(delete);
    }

    public TField TryFind(string name)
    {
        return fldList.Find(f => f.Name == name);
    }

    public TField Find(string name)
    {
        TField field = TryFind(name);

        if (field == null)
            throw new RangeException("Invalid field name {0}.", name);

        return field;
    }

    public void Get(Connection conn, string fields)
    {
        if (!Try(conn, fields))
            throw new GetDataException(Name);
    }

    public void CopyData(TGroup group, string fields)
    {
        bool sameName = Name == group.Name;

        foreach (TField field in ParseFields(fields, 0, 0))
        {
            if (sameName || (field.Flags & TField.InMemory) == 0)
                field.Get(group.Find(field.Name));
        }
    }

    public void GetData(Query q, string fields)
    {
        foreach (TField field in ParseFields(fields, TField.InMemory, 0))
            field.Get(q);
    }

    public virtual void Insert(Connection conn, string fields)
    {
        SqlInsert insert = new SqlInsert(Name);

        foreach (TField field in ParseFields(fields, TField.InMemory, 0))
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

                group.GetData(q, null);
                array.Add(group);
            }
        }
    }

    public void LoadFloat(Connection conn, IList array)
    {
        SqlSelect select = new SqlSelect(Name, null);

        AddFloat(select);
        LoadArray(conn, select, array);
    }

    public TGroup NewGroup()
    {
        return (TGroup) Activator.CreateInstance(GetType());
    }

    protected List<TField> ParseFields(string fields, uint mask, uint value)
    {
        if ((~mask & value) != 0)
            throw new RangeException("value {0:X} othside mask {1:X}", value, mask);

        if (fields != null && fields != "*")
        {
            List<TField> result = new List<TField>();

            foreach (string name in SplitCommas(fields))
                result.Add(Find(name));

            return result;
        }

        if (mask == 0 || (mask == TField.InMemory && value == 0 && (CombinedFlags & TField.InMemory) == 0))
            return fldList;

        return fldList.FindAll(f => (f.Flags & mask) == value);
    }

    protected string[] SplitCommas(string fields)
    {
        return fields.Replace(" ", "").Split(',');
    }

    public virtual bool Try(Connection conn, string fields)
    {
        SqlSelect select = new SqlSelect(Name, fields);

        AddExact(select);

        using (Query q = new Query(conn, select))
        {
            if (q.Read())
            {
                GetData(q, fields);
                return true;
            }
        }

        return false;
    }

    public virtual void Update(Connection conn, string fields)
    {
        SqlUpdate update = new SqlUpdate(Name);

        foreach (TField field in ParseFields(fields, TField.ExactKey | TField.InMemory, 0))
            field.Add(update);

        update.Where();
        AddExact(update);
        conn.Execute(update);
    }

    protected uint CombinedFlags;
    private List<TField> fldList;
}
