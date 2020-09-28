#include "cac.h"

// ----- TField --------------------------------------------------------------
TField::TField(const char *name)
	: TObject(name), Quoted(false)
{
}

void TField::Quote(mstr &m) const
{
	if (IsNULL())
		m.cat("NULL");
	else if (Quoted)
	{
		size_t len = m.len() + 1;

		m.cach('\'');
		Print(m);
		m.quote(len);
		m.cach('\'');
	}
	else
		Print(m);
}

static char *WindowName(TWindow *window)
{
	char *name;
	return window->SendMessage(WM_FACE_NAME, 0, (LPARAM) &name) == MW_NAMED_FACE ? name : "";
}

void TField::WindowImport(TWindow *window, void *group)
{
	char *name = WindowName(window);

	if (*name)
	{
		TField *target = ((TGroup *) group)->Find(name);

		if (target)
			target->ImportFromControl(dynamic_cast<TControl *> (window));
	}
}

TField::TField(const TField &that)
	: TObject(that.Name)
{
}

// ----- TFixedField ---------------------------------------------------------
TFixedField::TFixedField(const char *name, void *data, size_t size)
	: TField(name), Data((pBYTE) data), Size(size)
{
	Nullify();
}

bool TFixedField::IsNULL() const
{
	for (size_t n = 0; n < Size; n++)
		if (Data[n])
			return false;

	return true;
}

void TFixedField::Nullify()
{
	memset(Data, '\0', Size);
}

void TFixedField::SetData(const void *data)
{
	memcpy(Data, data, Size);
}

void TFixedField::ExportToControl(TControl *control) const
{
	if (control->Transfer(Data, tdSetData) != Size)
		fatal("%s: wrong export to control", Name);
}

void TFixedField::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	if (Size != desc->iLen)
		fatal("%s: %s: wrong import from query", query->Name, Name);

	if (!query->GetField(desc, Data))
		Nullify();
}

void TFixedField::ImportFromField(const TField *field)
{
	if (Size != ((const TFixedField *) field)->Size)
		fatal("%s: %s: wrong import from field", field->Name, Name);

	memcpy(Data, ((const TFixedField *) field)->Data, Size);
}

void TFixedField::ImportFromControl(TControl *control)
{
	if (control->Transfer(Data, tdGetData) != Size)
		fatal("%s: wrong import from control", Name);
}

// ----- TBaseString ---------------------------------------------------------
TBaseString::TBaseString(const char *name, size_t size)
	: TField(name), Size(size)
{
	Quoted = true;
}

bool TBaseString::IsNULL() const
{
	return !*S();
}

void TBaseString::Nullify()
{
	SetS("");
}

void TBaseString::Print(mstr &m) const
{
	m.cat(S());
}

void TBaseString::ExportToControl(TControl *control) const
{
	if (Size > ((TStatic *) control)->TextLimit)
		fatal("%s: wrong export to control", Name);

	control->Transfer((char *) S(), tdSetData);
}

void TBaseString::ImportFromField(const TField *field)
{
	const TString *string = (const TString *) field;

	if (strlen(string->S()) >= Size)
		fatal("%s: %s: wrong import from field", Name, field->Name);

	SetS(string->S());
}

void TBaseString::ImportFromControl(TControl *control)
{
	if (Size < ((TStatic *) control)->TextLimit)
		fatal("%s: wrong import from control", Name);

	control->Transfer((char *) S(), tdGetData);
	strrep((char *) S(), '\'', '"');	// length of S is never changed
}

// ----- TString -------------------------------------------------------------
TString::TString(const char *name, char *data, size_t size)
	: TBaseString(name, size), Data(data)
{
	Nullify();
}

const char *TString::S() const
{
	return (const char *) Data;
}

void TString::SetS(const char *s)
{
	strcpy(Data, s);
}

void TString::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	if (Size != desc->iLen)
		fatal("%s: %s: wrong import from query", query->Name, Name);

	if (query->GetField(desc, (pBYTE) Data))
		strimtrail(Data);
	else
		Nullify();
}

// ----- TLongString ---------------------------------------------------------
TLongString::TLongString(const char *name, char *data, size_t size)
	: TString(name, data, size)
{
}

void TLongString::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	mstr m;

	if (query->GetText(desc, &m, true))
	{
		if (m.len() >= Size)
			fatal("%s: %s: wrong import from query", query->Name, Name);

		strcpy(Data, str(m));
		strimtrail(Data);		// 2016:313 LPR: IB/FB CHAR(500)	
	}
	else
		Nullify();
}

// ----- TName ---------------------------------------------------------------
TName::TName(const char *name, char *data, size_t size)
	: TString(name, data, size)
{
}

void TName::ImportFromControl(TControl *control)
{
	TString::ImportFromControl(control);
	compress((char *) Data);
}

// ----- TJudicialName -------------------------------------------------------
TJudicialName::TJudicialName(const char *name, char *data, size_t size)
	: TName(name, data, size)
{
}

void TJudicialName::ImportFromControl(TControl *control)
{
	TName::ImportFromControl(control);

	if (COURT_CODE == COURT_SOFGS)
		squeeze((char *) Data);
}

// ----- TText ---------------------------------------------------------------
TText::TText(const char *name, mstr *data, size_t size)
	: TBaseString(name, size), Data(data)
{
	Nullify();
}

const char *TText::S() const
{
	return ncstr(*Data);
}

void TText::SetS(const char *s)
{
	Data->cpy(s);
}

void TText::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	if (!query->GetText(desc, Data, true))
		Nullify();
}

void TText::ImportFromControl(TControl *control)
{
	mbk_resize(Data, Size);
	TBaseString::ImportFromControl(control);
	Data->fixize();
}

// ----- TShort --------------------------------------------------------------
TShort::TShort(const char *name, short *data)
	: TFixedField(name, data, sizeof(short))
{
	SetH(0);
}

void TShort::ExportToControl(TControl *control) const
{
	((TLongFace *) (control))->SetL(H());
}

void TShort::ImportFromControl(TControl *control)
{
	SetH((short) ((TLongFace *) control)->L());
}

void TShort::Print(mstr &m) const
{
	itom(H(), m);
}

// ----- TLong ---------------------------------------------------------------
TLong::TLong(const char *name, long *data)
	: TFixedField(name, data, sizeof(long))
{
	SetL(0);
}

void TLong::ExportToControl(TControl *control) const
{
	((TLongFace *) (control))->SetL(L());
}

void TLong::ImportFromControl(TControl *control)
{
	SetL(((TLongFace *) control)->L());
}

void TLong::Print(mstr &m) const
{
	itom(L(), m);
}

// ----- TCheck --------------------------------------------------------------
TCheck::TCheck(const char *name, long *data, long tMask)
	: TLong(name, data), mask(tMask)
{
}

void TCheck::ExportToControl(TControl *control) const
{
	TYPECHECK_PTR_CAST(TCheckFace, control)->SetCheck(L() == -1 ? BF_GRAYED : L() & mask ? BF_CHECKED : BF_UNCHECKED);
}

void TCheck::ImportFromControl(TControl *control)
{
	switch (TYPECHECK_PTR_CAST(TCheckFace, control)->GetCheck())
	{
		case BF_CHECKED : SetL(L() == -1 ? mask : L() | mask); break;
		case BF_UNCHECKED : SetL(L() == -1 ? 0 : L() & ~mask); break;
		case BF_GRAYED : SetL(-1); break;
	}
}

// ----- TQuick --------------------------------------------------------------
TQuick::TQuick(const char *name, long *data)
	: TLong(name, data)
{
}

void TQuick::ExportToControl(TControl *control) const
{
	TYPECHECK_PTR_CAST(TQuickFace, control)->SetL(L());
}

void TQuick::ImportFromControl(TControl *control)
{
	SetL(TYPECHECK_PTR_CAST(TQuickFace, control)->L());
}

// ----- TRadio --------------------------------------------------------------
TRadio::TRadio(const char *name, long *data, long tValue)
	: TLong(name, data), value(tValue)
{
}

void TRadio::ExportToControl(TControl *control) const
{
	TYPECHECK_PTR_CAST(TRadioFace, control)->SetCheck(L() == value ? BF_CHECKED : BF_UNCHECKED);
}

void TRadio::ImportFromControl(TControl *control)
{
	if (TYPECHECK_PTR_CAST(TRadioFace, control)->GetCheck() == BF_CHECKED)
		SetL(value);
}

// ----- TDouble ------------------------------------------------------------
TDouble::TDouble(const char *name, double *data)
	: TFixedField(name, data, sizeof(double))
{
	SetD(0.0);
}

void TDouble::Print(mstr &) const
{
	fatal("%s: abstract call to Print", Name);
}

// ----- TMoney -------------------------------------------------------------
TMoney::TMoney(const char *name, double *data)
	: TDouble(name, data)
{
}

void TMoney::Print(mstr &m) const
{
	m.printf("%.2lf", D());
}

void TMoney::ExportToControl(TControl *control) const
{
	((TMoneyFace *) control)->SetM(M());
}

void TMoney::ImportFromControl(TControl *control)
{
	SetM(((TMoneyFace *) control)->M());
}

// ----- TCoord -------------------------------------------------------------
TCoord::TCoord(const char *name, double *data)
	: TDouble(name, data)
{
}

void TCoord::Print(mstr &m) const
{
	m.printf("%.14lf", D());
}

// ----- TChar ---------------------------------------------------------------
TChar::TChar(const char *name, char *data)
	: TFixedField(name, data, sizeof(char))
{
	Quoted = true;
	*Data = '\0'; 
}

void TChar::ExportToControl(TControl *control) const
{
	char s[2] = { C(), '\0' };
	control->Transfer((void *) s, tdSetData);
}

void TChar::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	char s[2];

	if (query->GetField(desc, (pBYTE) s))
		SetC(*s);
	else
		Nullify();
}

void TChar::ImportFromControl(TControl *control)
{
	char s[2];
	((TControl *) control)->Transfer(s, tdGetData);
	*Data = *s;
}

void TChar::Print(mstr &m) const
{
	m.cach(*Data);
}

// ----- TStampBase ----------------------------------------------------------
TStampBase::TStampBase(const char *name, CDateTime *data)
	: TFixedField(name, data, sizeof(CDateTime))
{
}

void TStampBase::Print(mstr &m) const
{
	Stamp().Print(m);
}

// ----- TTimeStamp ----------------------------------------------------------
TTimeStamp::TTimeStamp(const char *name, CDateTime *data)
	: TStampBase(name, data)
{
	Quoted = true;
}

void TTimeStamp::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	CDateTime *stamp = (CDateTime *) Data;

	if (!stamp->Get(desc, query))
		Nullify();
}

// ----- TDate ---------------------------------------------------------------
TDate::TDate(const char *name, CDate *data, bool today)
	: TFixedField(name, data, sizeof(CDate))
{
	if (today)
		SetDate(Today);
}

void TDate::Print(mstr &m) const
{
	Date().Print(m);
}

void TDate::Quote(mstr &m) const
{
	if (IsNULL())
		TField::Quote(m);
	else
		m.printf("%ld", Date().Value());
}

void TDate::ExportToControl(TControl *control) const
{
	((TDateFace *) control)->SetDate(Date());
}

void TDate::ImportFromControl(TControl *control)
{
	SetDate(((TDateFace *) control)->Date());
}

void TDate::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	long value;

	if (query->GetField(desc, (pBYTE) &value))
		((CDate *) Data)->SetValue(value);
	else
		Nullify();
}

// ----- TTime ---------------------------------------------------------------
TTime::TTime(const char *name, CTime *time)
	: TFixedField(name, time, sizeof(CTime))
{
}

void TTime::Print(mstr &m) const
{
	Time().Print(m);
}

void TTime::Quote(mstr &m) const
{
	if (IsNULL())
		TField::Quote(m);
	else
		m.printf("%ld", Time().Value());
}

void TTime::ExportToControl(TControl *control) const
{
	((TTimeFace *) control)->SetTime(Time());
}

void TTime::ImportFromControl(TControl *control)
{
	SetTime(((TTimeFace *) control)->Time());
}

void TTime::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	long value;

	if (query->GetField(desc, (pBYTE) &value))
		((CTime *) Data)->SetValue(value);
	else
		Nullify();
}

// ----- TDateTime -----------------------------------------------------------
TDateTime::TDateTime(const char *name, CDateTime *data)
	: TStampBase(name, data)
{
}

void TDateTime::Quote(mstr &m) const
{
	if (IsNULL())
		TField::Quote(m);
	else
		m.printf("%ld", Stamp().Value());
}

void TDateTime::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	long value;

	if (query->GetField(desc, (pBYTE) &value))
		((CDateTime *) Data)->SetValue(value);
	else
		Nullify();
}

// ----- TCharAlias ----------------------------------------------------------
TCharAlias::TCharAlias(const char *name, char *data)
	: TChar(name, data)
{
}

void TCharAlias::ExportToControl(TControl *control) const
{
	((TCharAliasFace *) control)->SetC(C());
}

void TCharAlias::ImportFromControl(TControl *control)
{
	SetC(((TCharAliasFace *) control)->C());
}

// ----- TListAlias ----------------------------------------------------------
TListAlias::TListAlias(const char *name, char *data)
	: TChar(name, data)
{
}

void TListAlias::ExportToControl(TControl *control) const
{
	((TCharListFace *) control)->SetC(C());
}

void TListAlias::ImportFromControl(TControl *control)
{
	SetC(((TCharListFace *) control)->C());
}

// ----- TStringAlias --------------------------------------------------------
TStringAlias::TStringAlias(const char *name, char *data, size_t size)
	: TString(name, data, size)
{
}

void TStringAlias::ExportToControl(TControl *control) const
{
	((TStringAliasFace *) control)->SetS(S());
}

void TStringAlias::ImportFromControl(TControl *control)
{
	SetS(((TStringAliasFace *) control)->S());
}

// ----- TPersonAlias --------------------------------------------------------
TPersonAlias::TPersonAlias(const char *name, char *data, size_t size)
:
	TStringAlias(name, data, size)
{
#if RANGECHECK
	if (size != SIZE_OF_UCN)
		fatal("%s: invalid person size %d", Name, size);
#endif  // RANGECHECK
}

const char *TPersonAlias::S() const
{
	return etz((const char *) Data);
}

void TPersonAlias::SetS(const char *s)
{
	TString::SetS(etz(s));
}

void TPersonAlias::ExportToControl(TControl *control) const
{
	((TPersonAliasFace *) control)->SetS(S());
}

void TPersonAlias::ImportFromControl(TControl *control)
{
	SetS(((TPersonAliasFace *) control)->S());
}

// ----- TLongAlias ----------------------------------------------------------
TLongAlias::TLongAlias(const char *name, long *data)
	: TLong(name, data)
{
}

void TLongAlias::ExportToControl(TControl *control) const
{
	((TLongAliasFace *) control)->SetL(L());
}

void TLongAlias::ImportFromControl(TControl *control)
{
	SetL(((TLongAliasFace *) control)->L());
}

// ----- TCharList -----------------------------------------------------------
TCharList::TCharList(const char *name, char *data, size_t size)
	: TString(name, data, size)
{
}

void TCharList::ExportToControl(TControl *control) const
{
	((TCharArrayListFace *) control)->SetS(S());
}

void TCharList::ImportFromControl(TControl *control)
{
	SetS(((TCharArrayListFace *) control)->S());
}

// ----- TCharArray ----------------------------------------------------------
TCharArray::TCharArray(const char *name, char *data, size_t size)
	: TCharList(name, data, size)
{
}

void TCharArray::Print(mstr &m) const
{
	char *starmap = new char[Size];

	memset(starmap, ' ', Size - 1);
	starmap[Size - 1] = '\0';

	for (pBYTE ts = (pBYTE) Data; *ts; ts++)
		starmap[*ts - 1] = '*';

	m.cat(starmap);
	delete[] starmap;
	m.trim_trail();
}

void TCharArray::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	if (Size != desc->iLen)
		fatal("%s: %s: wrong import from query", query->Name, Name);

	char *starmap = new char[Size];

	if (query->GetField(desc, (pBYTE) starmap))
	{
		*Data = '\0';
		for (const char *s = starmap; *s; s++)
			if (*s == '*')
				strcach((char *) Data, (char) (s - starmap + 1));
	}
	else
		Nullify();

	delete[] starmap;
}

// ----- TTextList -----------------------------------------------------------
TTextList::TTextList(const char *name, mstr *data, size_t size)
	: TText(name, data, size)
{
}

void TTextList::ExportToControl(TControl *control) const
{
	((TCharArrayListFace *) control)->SetS(S());
}

void TTextList::ImportFromControl(TControl *control)
{
	SetS(((TCharArrayListFace *) control)->S());
}

// ----- TBLOB ---------------------------------------------------------------
TBLOB::TBLOB(const char *name, TMemBlock *data)
	: TField(name), Data(data)
{
	Nullify();
}

void TBLOB::Print(mstr &m) const
{
	m.cat("?");
}

bool TBLOB::IsNULL() const
{
	return !Data->size;
}

void TBLOB::Nullify()
{
	mbk_clear(Data);
}

TCACParameter *TBLOB::CreateParam() const
{
	return new TCACParameter(Data->data, Data->size);
}

void TBLOB::ExportToControl(TControl *) const
{
	fatal("%s: abstract call to ExportToControl", Name);
}

void TBLOB::ImportFromQuery(const pFLDDesc desc, const TQuery *query)
{
	if (!query->GetBLOB(desc, Data))
		Nullify();
}

void TBLOB::ImportFromField(const TField *field)
{
	memblock_t *data = ((const TBLOB *) field)->Data;
	mbk_resize(Data, data->size);
	memcpy(Data->data, data->data, data->size);
}

void TBLOB::ImportFromControl(TControl *)
{
	fatal("%s: abstract call to ImportFromControl", Name);
}

// ----- KeyWords ------------------------------------------------------------
TKeyWords::TKeyWords(const char *name, char *data)
	: TString(name, data, SIZE_OF_KEYWORDS)
{
}

void TKeyWords::ExportToControl(TControl *control) const
{
	char buffer[SIZE_OF_EXPANDED_KEYWORDS];

	strcpy(buffer, S());

	if (any(buffer))
	{
		char *s;

		for (s = buffer + 1; *s; s++)
		{
			if (alnumdif(s[-1], *s))
			{
				strmove(s + 1, s);
				*s++ = ' ';
			}
		}
	}

	TYPECHECK_PTR_CAST(TKeyWordsFace, control)->SetS(buffer);
}

void TKeyWords::ImportFromControl(TControl *control)
{
	char buffer[SIZE_OF_EXPANDED_KEYWORDS];

	strcpy(buffer, TYPECHECK_PTR_CAST(TKeyWordsFace, control)->S());

	if (!kowpress(buffer))
		fatal("%s: wrong import from control", Name);

	SetS(buffer);
}

// ----- TGroup --------------------------------------------------------------
TGroup::TGroup(const char *name, TGroup *(*const tClone)())
	: TTable(name), clone(tClone ? tClone : TGroup::Clone)
{
}

TGroup::~TGroup()
{
}

TGroup *TGroup::Clone()
{
	fatal("abstract call to Clone");
	return NULL;
}

#if RANGECHECK
void TGroup::Add(TField *field)
{
	if (Find(field->Name))
		fatal("%s::Add: duplicate %s", Name, field->Name);

	TIArray<TField>::Add(field);
}
#endif  // RANGECHECK

void TGroup::Add(TField *field1, TField *field2, ...)
{
	Add(field1);

	if (field2)
	{
		va_list ap;
		TField *field;

		Add(field2);

		va_start(ap, field2);
		while ((field = va_arg(ap, TField *)) != NULL)
		{
		#if TYPECHECK
			if (dynamic_cast<TField *> (field) == NULL)
				fatal("%s::Add: invalid field", Name);
		#endif  // TYPECHECK
			Add(field);
		}
		va_end(ap);
	}
}

void TGroup::AddExtraText(const char *s)
{
	size_t size = strlen(s) + 1;
	char *ts = new char[size];

	Add(new TString("F_EXTRA_TEXT", ts, size));
	memcpy(ts, s, size);
}

bool TGroup::Delete(bool exact)
{
	msql w;

#if TESTVER
	if (strstr(Name, "INTERVAL"))
		MessageBeep(-1);
#endif  // TESTVER

	if (exact)
		ExactCriteria(w);
	else
	{
		FloatCriteria(w);
		w.CutOrder();
	}

	msql m(Name, MQ_DELETE);

	m.cat(strstr(str(w), " WHERE"));
	return dataBase->ExecCount(str(m), ET_DELETE, Name, "изтриване") >= exact;
}

void TGroup::DeleteExtraText()
{
	TString *extraText = FindExtraText();

	if (extraText)
	{
		delete[] extraText->S();
		Remove(extraText->Name);
	}
}

void TGroup::ExactCriteria(msql &, const char *)
{
	fatal("%s: abstract call to ExactCriteria", Name);
}

static bool WindowMatchesName(TWindow *window, void *name)
{
	return !strcmp(WindowName(window), (char *) name);
}

void TGroup::ExportToWindow(TWindow *window) const
{
	for (int i = 0; i < Count(); i++)
	{
		TField *source = (*this)[i];
		TWindow *target = window->FirstThat(WindowMatchesName, (void *) source->Name);

		if (target)
			source->ExportToControl(dynamic_cast<TControl *> (target));
	}
}

TField *TGroup::Find(const char *name) const
{
	for (int i = 0; i < Count(); i++)
	{
		TField *field = (*this)[i];

		if (!strcmpi(field->Name, name))	// 2012:282 EISPP: cmp -> cmpi
			return field;
	}

	return NULL;
}

TIArray<FLDDesc> *TGroup::Parse(const char *fields) const
{
	GetDescs();

	if (!fields)
		return descs;

	TIArray<FLDDesc> *tDescs = new TIArray<FLDDesc>;

#if RANGECHECK
	if (strchr(fields, '.'))
		error("Parse: %s: dot(s)", fields);
#endif  // RANGECHECK

	for (;;)
	{
		const char *end = strstr(fields, ", ");
		const char *dot = strchr(fields, '.');
		int i;

		if (!end)
			end = fields + strlen(fields);

		if (dot && dot < end)
			fields = dot + 1;

		for (i = 0; i < descs->Count(); i++)
		{
			pFLDDesc desc = (*descs)[i];

			if (!strncmpi(desc->szName, fields, end - fields) && !desc->szName[end - fields])
			{
				tDescs->Add(desc);
				break;
			}
		}

		if (i == descs->Count())
			fatal("%s: %s: unknown field", Name, fields);

		if (*end)
			fields = end + 2;
		else
			break;
	}

	return tDescs;
}

void TGroup::Quote(mstr &m, const char *fields)
{
	TIArray<FLDDesc> *tDescs = Parse(fields);
	bool first = true;
	params.Flush();

	for (int i = 0; i < tDescs->Count(); i++)
	{
		pFLDDesc desc = (*tDescs)[i];

		if (fields || !desc->iUnUsed[0])
		{
			TField *field = Find(desc->szName);

			if (!field)
				fatal("%s::Quote: missing %s", Name, desc->szName);

			m.printf(first ? "%s = " : ", %s = ", field->Name);
			QuoteField(m, field);
			first = false;
		}
	}

	Clear(tDescs);
}

void TGroup::Clear(TIArray<FLDDesc> *tDescs) const
{
	if (tDescs != descs)
	{
		tDescs->Clear();
		delete tDescs;
	}
}

void TGroup::FloatCriteria(msql &, const char *)
{
	fatal("%s: abstract call to FloatCriteria", Name);
}

bool TGroup::Get(const char *fields)
{
	return Try(fields) ? true : say_no_data(Name);
}

bool TGroup::Get(const msql &m)
{
	cac_assert(m.index(" WHERE ") != INT_MAX);
	cac_assert(m.index(Name) < m.index(" WHERE "));

	TQuery q(str(m));
	if (q.Read())
	{
		*this << q;
		return true;
	}

	return say_no_data(Name);
}

void TGroup::GroupCriteria(char *s, const char *fields, bool exact)
{
	msql m;

	if (exact)
		ExactCriteria(m, fields);
	else
		FloatCriteria(m, fields);

	strcpy(s, str(m));
	strcach(s, ';');
}

void TGroup::ImportFromQuery(const TQuery *query)
{
	for (int i = 0; i < Count(); i++)
	{
		TField *target = (*this)[i];
		const pFLDDesc source = query->Find(target->Name);

		if (source)
			target->ImportFromQuery(source, query);
	}
}

void TGroup::ImportFromGroup(const TGroup *group)
{
	for (int i = 0; i < group->Count(); i++)
	{
		TField *source = (*group)[i];
		TField *target = Find(source->Name);

		if (target)
			target->ImportFromField(source);
	}
}

void TGroup::ImportFromWindow(const TWindow *window)
{
	// ForEach requires non-const window
	((TWindow *) window)->ForEach(TField::WindowImport, this);
}

void TGroup::ImportFields(const TGroup *group, const char *fields)
{
	TIArray<FLDDesc> *tDescs = Parse(fields);

	for (int i = 0; i < tDescs->Count(); i++)
	{
		TField *source = group->Find((*tDescs)[i]->szName);

		Find(source->Name)->ImportFromField(source);
	}

	Clear(tDescs);

}

bool TGroup::Insert()
{
	mstr insert;
	mstr values("VALUES");
	bool first = true;

	GetDescs();
	insert.printf("INSERT INTO %s", Name);
	params.Flush();

	for (int i = 0; i < descs->Count(); i++)
	{
		TField *field = Find((*descs)[i]->szName);

		if (field && !field->IsNULL())
		{
			insert.printf(first ? " (%s" : ", %s", field->Name);
			values.cat(first ? " (" : ", ");
			QuoteField(values, field);
			first = false;
		}
	}

	insert.cat(") ");
	values.cat(")");
	insert.cat(str(values));
	return dataBase->Execute(str(insert), ET_WRITE, Name, "добавяне", params.Count() ? &params : NULL);
}

void TGroup::LoadArray(TIArray<TGroup> *array, const char *criteria)
{
	TQuery q(criteria, dataBase);

	while (q.Read())
	{
		TGroup *group = clone();
		*group << q;
		array->Add(group);
	}
}

void TGroup::LoadFloat(TIArray<TGroup> *array, const char *fields)
{
	msql m;
	FloatCriteria(m, fields);
	LoadArray(array, str(m));
}

void TGroup::Print(mstr &)
{
	fatal("%s: abstract call to Print", Name);
}

void TGroup::QuoteField(mstr &m, const TField *field)
{
	size_t len = m.len();
	field->Quote(m);

	if (!strcmp(m.str() + len, "?"))
		params.Add(TYPECHECK_PTR_CAST(const TBLOB, field)->CreateParam());
}

void TGroup::Remove(const char *name)
{
	int index = TIntegerArray::Find((long) Find(name));

#if RANGECHECK
	if (index == INT_MAX)
		fatal("%s::Remove: missing %s", Name, name);
#endif  // RANGECHECK

	TIArray<TField>::Remove(index);
}

void TGroup::Rename(const char *oldName, const char *newName)
{
	TField *field = Find(oldName);

#if RANGECHECK
	if (!field)
		fatal("%s::Rename: missing %s", Name, oldName);
#endif  // RANGECHECK

	(const char *) (field->Name) = newName;
}

bool TGroup::Try(const char *fields)
{
	msql m;

	ExactCriteria(m, fields);
	TQuery q(str(m), dataBase);

	if (q.Read())
	{
		*this << q;
		return true;
	}

	return false;
}

bool TGroup::Update(const char *fields)
{
#if 20180522
	msql m(Name, MQ_UPDATE);
	int count;

	Quote(m, fields);
	ExactCriteria(m, MQ_SUBSEQ);
#else  // 20180522
	msql m, w;
	int count;

	ExactCriteria(w);
	m.printf("UPDATE %s SET ", Name);
	Quote(m, fields);
	m.cat(strstr(str(w), " WHERE"));
#endif  // 20180522
	count = dataBase->ExecCount(str(m), ET_WRITE, Name, "модифициране", params.Count() ? &params : NULL);
	return count ? count > 0 : say_no_data(Name);
}

TGroup::TGroup(const TGroup &that)
	: TTable(that.Name), clone(that.clone)
{
}

// _
void TGroup::ClearGroupData()
{
	for (int i = 0; i < Count(); i++)
		(*this)[i]->Nullify();
}

// ----- TGroupArray ---------------------------------------------------------
int TGroupArray::AddCopy(const TGroup *group)	// don't insert - may be sorted
{
	TGroup *tGroup = group->clone();
	*tGroup << group;
	return Add(tGroup);
}

void TGroupArray::InsertCopy(const TGroup *group, int index)
{
	TGroup *tGroup = group->clone();
	*tGroup << group;
	Insert(tGroup, index);
}

void TGroupArray::DeleteExtraText()
{
	for (int i = 0; i < Count(); i++)
		(*this)[i]->DeleteExtraText();
}
