// ----- const ---------------------------------------------------------------
enum
{
	XML_INCLUDED	= 0x01,
	XML_REQUIRED	= 0x02,
	XML_EDITABLE	= 0x04,
	XML_UNKNOWN		= 0x10,
	XML_NULLABLE	= 0x20
};

enum
{
	SID_RESERVED	= 30,
	SID_MAXIMUM		= 65530	// also base for re-read sid-s
};

enum
{
	SID_FZL		= 'f',
	SID_PNE		= 'p',
	SID_NPRFZLPNE	= 'q',
	SID_NFL		= 'n'
};

// ----- TEisFile ------------------------------------------------------------
class TEisFile : public TXMLFile
{
public:
	TEisFile(long tSid);

	void OpenWrite(const char *name);
	FILE *OpenQuery(const char *name);
	void Close();

	bool IsOpen() { return f != NULL; }
	long Generate() { return ++sid; }

	long sid;
};

// ----- TXMLItem ------------------------------------------------------------
class TXMLItem
{
public:
	TXMLItem(const char *name);
	virtual ~TXMLItem();

	constant Name;
	long flags;

	virtual char *Read(char *text) = 0;
	virtual void Write(TEisFile &x) = 0;
	virtual bool RealNULL() = 0;
	virtual bool IsNULL();		// required && nullable -> not null
	virtual bool IsValid(mstr &m) = 0;
	virtual void Apply(TSbeDesc *desc);

protected:
	const char *value;

	bool IsValid(mstr &m, const char *what);
};

// ----- TXMLAttrib ----------------------------------------------------------
class TXMLAttrib : public TXMLItem
{
public:
	TXMLAttrib(const char *name) : TXMLItem(name) { }

	virtual char *Read(char *text);
	virtual void Write(TEisFile &x);
	virtual bool IsValid(mstr &m);

protected:
	virtual void Parse(char *text) = 0;
	virtual void EmitX(TEisFile &x) = 0;
};

// ----- TXMLString ----------------------------------------------------------
class TXMLString : public TXMLAttrib
{
public:
	TXMLString(const char *name, char *tData, int tSize);

protected:
	char *data;
	int size;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
	virtual bool RealNULL();
};

// ----- TXMLShort -----------------------------------------------------------
class TXMLShort : public TXMLAttrib
{
public:
	TXMLShort(const char *name, short *tData);

protected:
	short *data;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
	virtual bool RealNULL();
};

// ----- TXMLInt -------------------------------------------------------------
class TXMLInt : public TXMLAttrib
{
public:
	TXMLInt(const char *name, long *tData);

	TIntegerArray lset;

	virtual void Apply(TSbeDesc *desc);
	bool IsValid(mstr &m, long l);

protected:
	long *data;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
	virtual bool RealNULL();
	virtual bool IsValid(mstr &m);
};

// ----- TXMLSid -------------------------------------------------------------
class TXMLSid : public TXMLInt
{
public:
	TXMLSid(const char *name, long *data, char tPFix = '\0');

protected:
	char pfix;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
};

// ----- TXMLMoney -----------------------------------------------------------
class TXMLMoney : public TXMLAttrib
{
public:
	TXMLMoney(const char *name, double *tData);

protected:
	double *data;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
	virtual bool RealNULL();
};

// ----- TXMLDate ------------------------------------------------------------
class TXMLDate : public TXMLAttrib
{
public:
	TXMLDate(const char *name, CDate *tDate);

protected:
	CDate &date;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
	virtual bool RealNULL();
};

// ----- TXMLTime ------------------------------------------------------------
class TXMLTime : public TXMLAttrib
{
public:
	TXMLTime(const char *name, CTime *tTime);

protected:
	CTime &time;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
	virtual bool RealNULL();
};

// ----- TXMLUnknownAttrib ---------------------------------------------------
class TXMLUnknownAttrib : public TXMLAttrib
{
public:
	TXMLUnknownAttrib(const char *name);
	virtual ~TXMLUnknownAttrib() { delete[] data; }

protected:
	char *data;

	virtual void Parse(char *text);
	virtual void EmitX(TEisFile &x);
	virtual bool RealNULL();
};

// ----- TXMLStruct ----------------------------------------------------------
class TXMLStruct : public TXMLItem
{
public:
	TXMLStruct(const char *name) : TXMLItem(name) { }

	char *ReadStruct(char *text);

	virtual void Apply(TSbeDesc *desc);
	void Apply(TGArray<TSbeDesc> *descs);
	void Apply(const char *name, int flags, const char *value = NULL);
	void Apply(TSbeDesc *desc, int skip);

protected:
	virtual void ApplyStandard(TSbeDesc *desc, int skip) = 0;
};

// ----- TXMLObject ----------------------------------------------------------
class TXMLObject : public TXMLStruct
{
public:
	TXMLObject(const char *name) : TXMLStruct(name) { }

	TIArray<TXMLAttrib> attribs;
	TIArray<TXMLStruct> objects;

	TXMLItem *Find(const char *name, sign attr);	// -1 = all
	TXMLAttrib *FindAttrib(const char *name) { return (TXMLAttrib *) Find(name, 1); }
	TXMLObject *FindObject(const char *name) { return (TXMLObject *) Find(name, 0); }
	bool Includes(const char *name) { return Find(name, -1)->flags & XML_INCLUDED; }
	virtual char *Read(char *text);
	virtual void Write(TEisFile &x);

	virtual bool RealNULL();
	virtual bool IsValid(mstr &m);

protected:
	virtual void ApplyStandard(TSbeDesc *desc, int skip);
	virtual char *ReadStandard(char *text);
	friend class TXMLArray;
};

class TXMLUnknownObject : public TXMLObject
{
public:
	TXMLUnknownObject(const char *name) : TXMLObject(strnew(name)) { flags |= XML_UNKNOWN; }
};

// ----- TXMLRequiredObject --------------------------------------------------
class TXMLRequiredObject : public TXMLObject
{
public:
	TXMLRequiredObject(const char *name) : TXMLObject(name) { flags = XML_INCLUDED | XML_REQUIRED; }
};

// ----- TXMLGroup -----------------------------------------------------------
class TXMLArray;

class TXMLGroup : public TXMLObject, public TGroup
{
public:
	TXMLGroup(const char *xmlName, const char *groupName = NULL, TGroup *(*tClone)() = NULL);

	static void LoadArray(TXMLArray *array, const char *criteria, TXMLArray *descript = NULL);
};

// ----- TXMLUnknownGroup ----------------------------------------------------
class TXMLUnknownGroup : public TXMLGroup
{
public:
	TXMLUnknownGroup() : TXMLGroup(NULL) { }
	TXMLUnknownGroup(const char *name) : TXMLGroup(name) { flags |= XML_UNKNOWN; }
};

// ----- TXMLArray -----------------------------------------------------------
class TXMLArray : public TXMLStruct, public TGroupArray
{
public:
	TXMLArray(const char *name);

	virtual char *Read(char *text);
	virtual void Write(TEisFile &x);

	virtual bool RealNULL();
	virtual bool IsValid(mstr &m);

	TXMLGroup *NewObject();	// NewTObject() + ApplyDescs()
	void ApplyDescs(TXMLGroup *group);	// apply all stored descs

protected:
	TGArray<TSbeDesc> descs;

	TXMLGroup *ObjectAt(int i) { return (TXMLGroup *) (*this)[i]; }
	virtual TXMLGroup *NewTObject() = 0;
	virtual void ApplyStandard(TSbeDesc *desc, int skip);		// store descs
};

// ----- TXArray -------------------------------------------------------------
template <class T> class TXArray : public TXMLArray
{
public:
	TXArray(const char *name) : TXMLArray(name) { }

	T *operator[](int index) const { return (T *) ItemAt(index); }
	int Add(T *t) { return TXMLArray::Add(t); }
	void Insert(T *t, int index) { TXMLArray::Insert(t, index); }
	T *NewObject() { return (T *) TXMLArray::NewObject(); }

protected:
	virtual TXMLGroup *NewTObject() { return new T; }
};

// ----- TXMLUnknownArray ----------------------------------------------------
class TXMLUnknownArray : public TXArray<TXMLUnknownGroup>
{
public:
	TXMLUnknownArray(const char *name) : TXArray<TXMLUnknownGroup>(name) { }

protected:
	virtual TXMLGroup *NewTObject() { return new TXMLUnknownGroup(Name); }
};

#define TXARRAY(T) TXArray<T>(#T)
