using System;

public abstract class TField
{
    public TField(string Name) { this.Name = Name; }

    public readonly string Name;

    public abstract void Get(TField field);
    public abstract void Get(Query q);
    public abstract void Add(SqlValueBuilder builder);
}

public class TChar : TField
{
    public TChar(string Name, Func<char> getter, Action<char> setter)
        : base(Name)
    {
        this.getter = getter;
        this.setter = setter;
    }

    public char GetValue() { return getter(); }

    public override void Get(TField field)
    {
        setter(((TChar) field).getter());
    }

    public override void Get(Query q)
    {
        setter(q.GetChar(Name));
    }

    public override void Add(SqlValueBuilder builder)
    {
       builder.AddChar(Name, getter());
    }

    protected readonly Func<char> getter;
    protected readonly Action<char> setter;
}

public class TString : TField
{
    public TString(string Name, Func<string> getter, Action<string> setter)
        : base(Name)
    {
        this.getter = getter;
        this.setter = setter;
        setter("");  // instead of null
    }

    public string GetValue() { return getter(); }

    public override void Get(TField field)
    {
        setter(((TString) field).getter());
    }

    public override void Get(Query q)
    {
        setter(q.GetString(Name));
    }

    public override void Add(SqlValueBuilder builder)
    {
       builder.AddString(Name, getter());
    }

    protected readonly Func<string> getter;
    protected readonly Action<string> setter;
}

public class TInt : TField
{
    public TInt(string Name, Func<int> getter, Action<int> setter)
        : base(Name)
    {
        this.getter = getter;
        this.setter = setter;
    }

    public int GetValue() { return getter(); }

    public override void Get(TField field)
    {
        setter(((TInt) field).getter());
    }

    public override void Get(Query q)
    {
        setter(q.GetInt(Name));
    }

    public override void Add(SqlValueBuilder builder)
    {
       builder.AddInt(Name, getter());
    }

    protected readonly Func<int> getter;
    protected readonly Action<int> setter;
}

public class TZInt : TInt
{
    public TZInt(string Name, Func<int> getter, Action<int> setter)
        : base(Name, getter, setter)
    {
    }

    public override void Add(SqlValueBuilder builder)
    {
        builder.AddZInt(Name, getter());
    }
}

public class TDouble : TField
{
    public TDouble(string Name, Func<double> getter, Action<double> setter)
        : base(Name)
    {
        this.getter = getter;
        this.setter = setter;
    }

    public double GetValue() { return getter(); }

    public override void Get(TField field)
    {
        setter(((TDouble) field).getter());
    }

    public override void Get(Query q)
    {
        setter(q.GetDouble(Name));
    }

    public override void Add(SqlValueBuilder builder)
    {
       builder.AddDouble(Name, getter());
    }

    protected readonly Func<double> getter;
    protected readonly Action<double> setter;
}

public class TZDouble : TDouble
{
    public TZDouble(string Name, Func<double> getter, Action<double> setter)
        : base(Name, getter, setter)
    {
    }

    public override void Add(SqlValueBuilder builder)
    {
        builder.AddZDouble(Name, getter());
    }
}

public class TDateTime : TField
{
    public TDateTime(string Name, Func<DateTime> getter, Action<DateTime> setter)
        : base(Name)
    {
        this.getter = getter;
        this.setter = setter;
    }

    public DateTime GetValue() { return getter(); }

    public override void Get(TField field)
    {
        setter(((TDateTime) field).getter());
    }

    public override void Get(Query q)
    {
        setter(q.GetDateTime(Name));
    }

    public override void Add(SqlValueBuilder builder)
    {
       builder.AddDateTime(Name, getter());
    }

    protected readonly Func<DateTime> getter;
    protected readonly Action<DateTime> setter;
}
