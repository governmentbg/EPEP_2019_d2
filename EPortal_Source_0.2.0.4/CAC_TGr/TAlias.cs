using System;
using System.Collections.Generic;

public class GetCacheException : Exception
{
    public GetCacheException(string source)
        : base(String.Format("Empty cache loaded for {0}.", source))
    {
    }
}

public abstract class TAlias : TGroup
{
    protected TAlias(string Table) : base(Table)
    {
    }

    protected override string ExactFields()
    {
        string fields = ValueField().Name;

        if (GroupField() != null)
            fields = String.Format("{0}, {1}", GroupField().Name, fields);

        return fields;
    }

    protected override string FloatFields()
    {
        return GroupField() != null ? GroupField().Name : base.FloatFields();
    }

    protected TString nameField;
    protected abstract TField ValueField();
    protected virtual TField GroupField() { return null; }

    public List<TAlias> LoadCache(Connection conn, string order)
    {
        List<TAlias> cache = new List<TAlias>();
        SqlSelect select = new SqlSelect(Table, null);

        if (order != null)
            select.AddOrder(order);

        LoadArray(conn, select, cache);

        if (cache.Count == 0)
            throw new GetCacheException(Table);

        return cache;
    }
}

public class TCharAlias : TAlias
{
    protected TCharAlias(string Table) : base(Table) { }
    protected override TField ValueField() { return valueField; }

    protected static TCharAlias Seek(List<TAlias> cache, char value)
    {
        foreach (TCharAlias alias in cache)
            if (alias.valueField.GetValue() == value)
                return alias;

        throw new GetDataException(cache[0].Table);
    }

    protected TChar valueField;
}

public class TIntAlias : TAlias
{
    protected TIntAlias(string Table) : base(Table) { }
    protected override TField ValueField() { return valueField; }

    protected static TIntAlias Seek(List<TAlias> cache, int value)
    {
        foreach (TIntAlias alias in cache)
            if (alias.valueField.GetValue() == value)
                return alias;

        throw new GetDataException(cache[0].Table);
    }

    protected TInt valueField;
}

public class TIntCharAlias : TIntAlias
{
    protected TIntCharAlias(string Table) : base(Table) { }
    protected override TField GroupField() { return groupField; }

    protected static TIntCharAlias Seek(List<TAlias> cache, int value, char group)
    {
        foreach (TIntCharAlias alias in cache)
            if (alias.valueField.GetValue() == value && alias.groupField.GetValue() == group)
                return alias;

        throw new GetDataException(cache[0].Table);
    }

    protected TChar groupField;
}

public class TIntIntAlias : TIntAlias
{
    protected TIntIntAlias(string Table) : base(Table) { }
    protected override TField GroupField() { return groupField; }

    protected static TIntIntAlias Seek(List<TAlias> cache, int value, int group)
    {
        foreach (TIntIntAlias alias in cache)
            if (alias.valueField.GetValue() == value && alias.groupField.GetValue() == group)
                return alias;

        throw new GetDataException(cache[0].Table);
    }

    protected TInt groupField;
}

public class TStringAlias : TAlias
{
    protected TStringAlias(string Table) : base(Table) { }
    protected override TField ValueField() { return valueField; }

    protected static TStringAlias Seek(List<TAlias> cache, string value)
    {
        foreach (TStringAlias alias in cache)
            if (alias.valueField.GetValue() == value)
                return alias;

        throw new GetDataException(cache[0].Table);
    }

    protected TString valueField;
}
