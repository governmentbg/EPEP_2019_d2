// ----- offield -------------------------------------------------------------
struct offield
{
	const char *name;
	void (*print)(mstr &m, TGroup *group);
};

extern const offield null_fields[];

// ----- TOffice -------------------------------------------------------------
#define OFFICE_GROUP_CAST(type) TYPECHECK_PTR_CAST(type, group)

enum TOfficeCopy
{
	OFFICE_COPY_NULL,		// ADMC export only; 2017:289 full support
	OFFICE_COPY_ALL,
	OFFICE_COPY_HEADER,
	OFFICE_SKIP_HEADER
};

class TOffice : public TObject
{
public:
	TOffice();
	~TOffice();

	const char *mainStart;
	const char *mainFinal;

	void InitAutoGen(long tAutoGen) { officeGroup.InitAutoGen(tAutoGen); }
	long GetIncrAutoGen() { return officeGroup.GetIncrAutoGen(); }
	static bool ValidTargetName(const char *name);
	bool Open(const char *tName, TOfficeCopy copy = OFFICE_COPY_ALL, const char *target = NULL);
	const char *Find(const char *tMark);
	void Emit(const char *start, const char *final, const offield *const fields, TGroup *group);
	void EmitBreak() { h.MsPageBr(); }
	void EmitBreak(const char *start, const char *final);		// </table>, Break, Emit
	void LineBreak() { h.putc('\n'); }
	bool Show(TWaitWindow *wait);
	void ShowSync(TWaitWindow *wait, sign sync, const char *fallWillOpen, const char *fallWasSaved);
	void Close() { h.Close(); }
	void SaveBody(const char *target) { h.SaveBody(target); }
	void PutRaw(const char *s) { h.PutRaw(s); }
	void SetStub(const char *s) { stub = s; }		// 2017:285 +related

	// 2014:248 LPR: was in printsub
	static void Emit(mstr &m, const offield *fields, TGroup *group, const char *stemplate);

	// 2016:349 LPR: was long autogen
	class TOfficeGroup : public TDummyGroup
	{
	public:
		TOfficeGroup();

		void InitAutoGen(long tAutoGen) { autogen = tAutoGen; }
		long GetIncrAutoGen() { return autogen++; }

	private:
		long autogen;			// 2013:070 LPR: +related
	};

protected:
	THTMLFile h;
	const char *name;
	char full[SIZE_OF_PATH];
	char target_base[SIZE_OF_TWICE_BASE];
	bool wordFile;
	const char *stub;

	static bool ValidBaseName(const char *name, size_t maxlen);

private:
	char *buffer;

	struct offstr : public mstr
	{
	public:
		offstr() { }
		void reset();

		bool wild;
		long cache;
	};

	offstr values[0x100];
	TOfficeGroup officeGroup;
};

bool show_office(const char *name, const offield *fields, TGroup *group, TOfficeCopy copy = OFFICE_COPY_ALL);
