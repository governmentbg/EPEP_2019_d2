// ----- TCACConnect ---------------------------------------------------------
class TCACConnect : public TVObject
{
public:
	TCACConnect(const char *name);
	virtual TCACDataBase *NewDataBase() const = 0;

	virtual bool Load(char *value) = 0;
	virtual const char *SyntaxError(char *value) = 0;
	virtual void SyntaxWarn(char *value, mstr &m) = 0;
	virtual bool Save(char *value) = 0;
	virtual bool Delete(char *value) = 0;
};

// ----- TODBCConnect --------------------------------------------------------
class TODBCConnect : public TCACConnect
{
public:
	TODBCConnect(const char *name) : TCACConnect(name) { }

	static bool Exist(const char *name, char *value);
	virtual bool Delete(char *value);

protected:
	virtual const char *DriverName() = 0;

	bool DeleteDSN(mstr &m);
	bool ConfigDSN(const char *oper, WORD request, const char *attributes);
};

// ----- TPostGresConnect ----------------------------------------------------
class TPostGresConnect : public TODBCConnect
{
public:
	TPostGresConnect(const char *name) : TODBCConnect(name) { }
	virtual TCACDataBase *NewDataBase() const;

	virtual bool Load(char *value);
	virtual const char *SyntaxError(char *value);
	virtual void SyntaxWarn(char *value, mstr &m);
	virtual bool Save(char *value);

protected:
	virtual const char *DriverName();
};

// ----- TFireBirdConnect ----------------------------------------------------
class TFireBirdConnect : public TODBCConnect
{
public:
	TFireBirdConnect(const char *name) : TODBCConnect(name) { }
	virtual TCACDataBase *NewDataBase() const;

	virtual bool Load(char *value);
	virtual const char *SyntaxError(char *value);
	virtual void SyntaxWarn(char *value, mstr &m);
	virtual bool Save(char *value);

protected:
	virtual const char *DriverName();
};

#ifdef __BORLANDC__
// ----- TBDEConnect ---------------------------------------------------------
class TBDEConnect : public TCACConnect
{
public:
	TBDEConnect(const char *name) : TCACConnect(name) { }
	virtual TCACDataBase *NewDataBase() const;

	virtual bool Load(char *value);
	virtual const char *SyntaxError(char *value);
	virtual void SyntaxWarn(char *value, mstr &m);
	virtual bool Save(char *value);
	virtual bool Delete(char *);
};
#endif  // __BORLANDC__
