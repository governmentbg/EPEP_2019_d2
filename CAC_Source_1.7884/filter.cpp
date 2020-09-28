#include "cac.h"

// ----- TWSTR ---------------------------------------------------------------
TWSTR::TWSTR(const char *s, int len, bool crash)
{
	size_t size = (len == -1 ? strlen(s) : len) + 1;
	ws = new wchar_t[size];

	if (!MultiByteToWideChar(1251, 0, s, size, ws, size))
	{
		delete ws;
		ws = NULL;
		error("Грешка %lu при UNICODE конверсия.", GETLASTERROR);

		if (crash)
			failure();
	}
}

// ----- TBSTR ---------------------------------------------------------------
TBSTR::TBSTR(const char *s, int len, bool crash)
{
	if (len == -1)
		len = strlen(s);

	TWSTR ws(s, len, crash);

	if (ws.wstr())
		Allocate(ws.wstr(), len);
	else
		bs = NULL;
}

void TBSTR::Allocate(const wchar_t *ws, size_t len)
{
	if ((bs = SysAllocStringLen(ws, len)) == NULL)
		out_of_memory();
}

// ----- OLE -----------------------------------------------------------------
#define GET DISPATCH_PROPERTYGET
#define PUT DISPATCH_PROPERTYPUT
#define METHOD DISPATCH_METHOD

#define W_VERB	((WORD) 0xFF)
#define W_OPTION	0x100

static void check_hr(HRESULT hr, const char *s1, const char *s2 = NULL, WORD wFlags = 0)
{
	if (hr != S_OK && !(wFlags & W_OPTION))
	{
		if (s2)
			fatal("Грешка %lx при комуникация с OLE (%s:%s).", (unsigned long) hr, s1, s2);
		else
			fatal("Грешка %lx при комуникация с OLE (%s).", (unsigned long) hr, s1);
	}
}

static inline HRESULT ole_dispid(IDispatch *iDispatch, const char *name, DISPID *dispID)
{
	return IDispatch_GetIDsOfNames(iDispatch, IID_NULL, TWSTR(name).wspr(), 1, LOCALE_SYSTEM_DEFAULT, dispID);
}

static DISPID ole_dispid(IDispatch *iDispatch, const char *name, WORD wFlags)
{
	DISPID dispID = 0;
	HRESULT hr = ole_dispid(iDispatch, name, &dispID);
	check_hr(hr, "IDispatch_GetIDsOfNames", name, wFlags);
	return dispID;
}

static VARIANT oleparams[16], oleresult;
static DISPID dispidPropertyPut = DISPID_PROPERTYPUT;
static DISPPARAMS dispparams = { oleparams, &dispidPropertyPut, 0, 0 };
#define var oleparams
#define res oleresult

// 2008:106 LPR: #define cArgs -> argument, #define cNamedArgs -> automatic
static bool ole_invoke(IDispatch *iDispatch, const char *name, WORD wFlags, unsigned cArgs)
{
	UINT err;
	HRESULT hr;

	dispparams.cArgs = cArgs;
	dispparams.cNamedArgs = (wFlags & PUT) != 0;
	hr = IDispatch_Invoke(iDispatch, ole_dispid(iDispatch, name, wFlags), IID_NULL, LOCALE_SYSTEM_DEFAULT, wFlags & W_VERB,
		&dispparams, &oleresult, NULL, &err);
	check_hr(hr, "Dispatch_Invoke", name, wFlags);
	return hr == S_OK;
}

// for some reason any successful replacement blocks further finds and
// obtaining a new iFind; well, re-obtaining iContent and then iFind works
static bool filter_replace(IDispatch *iActive, BSTR bsFind, BSTR bsReplace, int skip)
{
	IDispatch *iContent = NULL, *iFind = NULL;
	bool result = true;

	try
	{
		ole_invoke(iActive, "Content", GET, 0);
		iContent = (IDispatch *) res.punkVal;

		ole_invoke(iContent, "Find", GET, 0);
		iFind = (IDispatch *) res.punkVal;

		var[0].vt = VT_BOOL;
		var[0].boolVal = VARIANT_FALSE;	// format
		var[1].vt = VT_I4;
		var[1].lVal = 0;				// wrap, wdFindStop
		var[2].vt = VT_BOOL;
		var[2].boolVal = VARIANT_TRUE;	// forward
		var[3].vt = VT_BOOL;
		var[3].boolVal = VARIANT_FALSE;	// all word forms
		var[4].vt = VT_BOOL;
		var[4].boolVal = VARIANT_FALSE;	// sounds like
		var[5].vt = VT_BOOL;
		var[5].boolVal = VARIANT_FALSE;	// match wildcards
		var[6].vt = VT_BOOL;
		var[6].boolVal = VARIANT_FALSE;	// whole words
		var[7].vt = VT_BOOL;
		// match case
		var[7].boolVal = FILTER_OLE_CASE ? VARIANT_TRUE : VARIANT_FALSE;
		var[8].vt = VT_BSTR;
		var[8].bstrVal = bsFind;

		for (int i = 0; i < skip; i++)
		{
			ole_invoke(iFind, "Execute", METHOD, 9);
			if (res.boolVal != VARIANT_TRUE)
				fatal("OLE грешка при търсене на текст.");
		}

		var[0].vt = VT_I4;
		var[0].boolVal = 1;			// replace, once
		var[1].vt = VT_BSTR;
		var[1].bstrVal = bsReplace;
		var[2].vt = VT_BOOL;
		var[2].boolVal = VARIANT_FALSE;	// format
		var[3].vt = VT_I4;
		var[3].lVal = 0;				// wrap, wdFindStop
		var[4].vt = VT_BOOL;
		var[4].boolVal = VARIANT_TRUE;	// forward
		var[5].vt = VT_BOOL;
		var[5].boolVal = VARIANT_FALSE;	// all word forms
		var[6].vt = VT_BOOL;
		var[6].boolVal = VARIANT_FALSE;	// sounds like
		var[7].vt = VT_BOOL;
		var[7].boolVal = VARIANT_FALSE;	// match wildcards
		var[8].vt = VT_BOOL;
		var[8].boolVal = VARIANT_FALSE;	// whole words
		var[9].vt = VT_BOOL;
		// match case
		var[9].boolVal = FILTER_OLE_CASE ? VARIANT_TRUE : VARIANT_FALSE;
		var[10].vt = VT_BSTR;
		var[10].bstrVal = bsFind;		// text to replace

		ole_invoke(iFind, "Execute", METHOD, 11);
		if (res.boolVal != VARIANT_TRUE)
			result = error("OLE грешка при замяна на текст.");
	}
	catch (...)
	{
		result = false;
	}

	if (iContent)
	{
		if (iFind)
			IDispatch_Release(iFind);
		IDispatch_Release(iContent);
	}

	return result;
}

bool filter_replace(IDispatch *iActive, const char *text, int start, int len, const char *rep, int replen)
{
	try
	{
		const char *pos = text + start;
		int skip = 0;

		for (const char *s = text; (s = strchr(s, *pos)) < pos; s++)
			if ((FILTER_OLE_CASE ? memcmp(s, pos, len) : memicmp(s, pos, len)) == 0)
				skip++;

		return filter_replace(iActive, TBSTR(pos, len).bstr(), TBSTR(rep, replen).bstr(), skip);
	}
	catch (...)
	{
	}

	return false;
}

static const wchar_t *strchr_w(const wchar_t *text, wchar_t c)
{
	const wchar_t *ws;
	for (ws = text; *ws && *ws != c; ws++);
	return *ws == c ? ws : NULL;
}

static inline wchar_t toupper_w(wchar_t c)
{
	return (wchar_t) CharUpperW((LPWSTR) c);
}

static int memicmp_w(const wchar_t *ws1, const wchar_t *ws2, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		if (toupper_w(ws1[i]) != toupper_w(ws2[i]))
			return 1;

	return 0;
}

bool filter_replace(IDispatch *iActive, const wchar_t *text, int start, int len, const wchar_t *rep, int replen)
{
	const wchar_t *pos = text + start;
	int skip = 0;

	for (const wchar_t *ws = text; (ws = strchr_w(ws, *pos)) < pos; ws++)
		if ((FILTER_OLE_CASE ? memcmp(ws, pos, len * sizeof(wchar_t)) : memicmp_w(ws, pos, len)) == 0)
			skip++;

	return filter_replace(iActive, TBSTR(pos, len).bstr(), TBSTR(rep, replen).bstr(), skip);
}

static constant FILTER_APPLICATION = "Word.Application";

// 2008:136 LPR: re-organization for multiply replacements
// 2008:136 LPR: rewritten w/ classes for many members
class TFilterDocument
{
public:
	TFilterDocument();
	~TFilterDocument();

	bool IsOpen();
	void FindReplace(TReplacePair *pair) { GetText(); pair->FindReplace(text, iActive); }

	void Filter(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, bool always);
	bool Number(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, const char *tName,
		long filter, const TRegisterAct *act);
	bool Display(TWaitWindow *wait, const char *name, const char *fallWillOpen, const char *fallWasSaved);

protected:
	void GetText();
	const char *Initialize(const char *name, char *tName = NULL);
	void Shutdown();
	bool CheckNo(const char *eventNo);
	void Replace(char *pos, int len, const char *rep);
	void PlaceNo(const char *eventNo, const CDate &date);
	bool FallBack(const char *fallWillOpen, const char *fallWasSaved, const char *name, TWaitWindow *wait = NULL);
	void Visualize();
	void WaitFor(TWaitWindow *wait, const char *name);

	IDispatch *iApplication, *iDocuments;
	IDispatch *iActive, *iWebOptions;
	IDispatch *iProperties, *iTitle;
	char *text;
};

TFilterDocument::TFilterDocument()
	: text(NULL)
{
	iApplication = iDocuments = iActive = iWebOptions = iProperties = iTitle = NULL;
}

TFilterDocument::~TFilterDocument()
{
	delete[] text;
}

bool TFilterDocument::IsOpen()
{
	DISPID dispID;
	HRESULT hr = ole_dispid(iApplication, "Creator", &dispID);

	switch (hr)
	{
		case RPC_E_RETRY :
		case RPC_E_SERVERCALL_RETRYLATER :
		case RPC_E_SERVERCALL_REJECTED :
		case RPC_E_CALL_REJECTED :
		case RPC_E_CALL_CANCELED : return true;
	}

	return hr == S_OK;
}

void TFilterDocument::GetText()
{
	delete[] text;
	text = NULL;

	IDispatch *iContent = NULL;

	try
	{
		ole_invoke(iActive, "Content", GET, 0);
		iContent = (IDispatch *) res.punkVal;
		ole_invoke(iContent, "Text", GET, 0);

		BSTR bstr = res.bstrVal;
		unsigned size = SysStringLen(bstr) + 1;

		text = new char[size + 30];	// 2008:136 LPR: +30 for numbering
		if (!WideCharToMultiByte(1251, 0, bstr, -1, text, size, NULL, NULL))
			fatal("Грешка %lu при UNICODE конверсия.", GETLASTERROR);
		SysFreeString(bstr);
	}
	catch (...)
	{
		if (iContent)
			IDispatch_Release(iContent);
		throw;
	}

	IDispatch_Release(iContent);
}

const char *TFilterDocument::Initialize(const char *name, char *tName)
{
	CLSID clsid;
	const char *application = FILTER_APPLICATION;

	check_hr(CLSIDFromProgID(TWSTR(application).wstr(), &clsid), "CLSIDFromProgID", application);
	check_hr(CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IDispatch,
		(void **) &iApplication), "CoCreateInstance", application);

	ole_invoke(iApplication, "Documents", GET, 0);
	iDocuments = (IDispatch *) res.punkVal;

	// 2016:110 URQ/TRQ: shortened support
	char ttName[SIZE_OF_PATH];

	if (!tName)
		tName = ttName;

	if (name_shortened(name, tName, false))
		name = tName;

	if (COURT_CODE == COURT_SOFGS && WORD_OPEN_7A)
	{
		int argc = 0;
		TBSTR bsEmpty("");

		ole_invoke(iApplication, "Version", GET, 0);

		if (res.bstrVal[0] <= 5 && WORD_OPEN_7A > 1)	// versions 10 to 50
		{
			var[argc].vt = VT_BOOL;
			var[argc++].boolVal = VARIANT_TRUE;		// no encoding dialog
			var[argc].vt = VT_I4;
			var[argc++].lVal = 0;				// direction: left to right
			var[argc].vt = VT_NULL;
			var[argc++].boolVal = VARIANT_TRUE;		// open and repair
			var[argc].vt = VT_BOOL;
			var[argc++].boolVal = VARIANT_TRUE;		// visible
			var[argc].vt = VT_BSTR;
			var[argc++].bstrVal = bsEmpty.bstr();	// document encoding
			var[argc].vt = VT_I4;
			var[argc++].lVal = 0;				// open format auto
			var[argc].vt = VT_BSTR;
			var[argc++].bstrVal = bsEmpty.bstr();	// template write password
			var[argc].vt = VT_BSTR;
			var[argc++].bstrVal = bsEmpty.bstr();	// document write password
		}

		SysFreeString(res.bstrVal);

		var[argc].vt = VT_BOOL;
		var[argc++].boolVal = VARIANT_FALSE;	// revert to saved
		var[argc].vt = VT_BSTR;
		var[argc++].bstrVal = bsEmpty.bstr();	// template read password
		var[argc].vt = VT_BSTR;
		var[argc++].bstrVal = bsEmpty.bstr();	// document read password
		var[argc].vt = VT_BOOL;
		var[argc++].boolVal = VARIANT_FALSE;	// add to recent files
		var[argc].vt = VT_BOOL;
		var[argc++].boolVal = VARIANT_FALSE;	// read only
		var[argc].vt = VT_BOOL;
		var[argc++].boolVal = VARIANT_FALSE;	// confirm conversions
		var[argc].vt = VT_BSTR;
		TBSTR bsOpen(name);
		var[argc++].bstrVal = bsOpen.bstr();

		ole_invoke(iDocuments, "Open", METHOD, argc);

	}
	else
	{
		var[0].vt = VT_BSTR;
		TBSTR bsOpen(name);
		var[0].bstrVal = bsOpen.bstr();
		ole_invoke(iDocuments, "Open", METHOD, 1);
	}

	ole_invoke(iApplication, "ActiveDocument", GET, 0);
	iActive = (IDispatch *) res.punkVal;

	if (FILTER_HIDE_SPELL)
	{
		var[0].vt = VT_BOOL;
		var[0].boolVal = VARIANT_FALSE;
		ole_invoke(iActive, "ShowSpellingErrors", PUT | W_OPTION, 1);
		ole_invoke(iActive, "ShowGrammaticalErrors", PUT | W_OPTION, 1);
	}

	GetText();
	return name;
}

void TFilterDocument::Shutdown()
{
	if (iApplication)
	{
		if (iDocuments)
		{
			if (iActive)
			{
				if (iTitle)
					IDispatch_Release(iTitle);
				if (iProperties)
					IDispatch_Release(iProperties);
				if (iWebOptions)
					IDispatch_Release(iWebOptions);
				var[0].vt = VT_BOOL;
				var[0].boolVal = VARIANT_FALSE;
				ole_invoke(iActive, "Close", METHOD | W_OPTION, 1);
				IDispatch_Release(iActive);
			}
			IDispatch_Release(iDocuments);
		}
		ole_invoke(iApplication, "Quit", METHOD | W_OPTION, 0);
		IDispatch_Release(iApplication);

		iApplication = NULL;
	}
}

#include "FindReplaceListBox.h"
#include "FindReplaceListBox.cpp"

#include "FindReplaceDialog.h"
#include "FindReplaceDialog.cpp"

bool TFilterDocument::CheckNo(const char *eventNo)
{
	char *s;

	if ((s = strstr(text, eventNo)) == NULL || (s > text && isdigit(s[-1])) || isdigit(*(s += strlen(eventNo))))
		return false;

	while (isspace(*s))
		s++;
	if (*s++ != '/')
		return false;

	int i, n;
	long l;

	return sscanf(s, "%d.%d.%4ld", &i, &i, &l, &n) == 3 && n <= 20;
}

void TFilterDocument::Replace(char *pos, int len, const char *rep)
{
	int replen = strlen(rep);

	filter_replace(iActive, text, pos - text, len, rep, replen);
	strmove(pos + replen, pos + len);
	memcpy(pos, rep, replen);
}

void TFilterDocument::PlaceNo(const char *eventNo, const CDate &date)
{
	char *beg = strstr(text, "...");
	char *s = strchr(text, '…');
	int begLen = 3;

	if (s && (!beg || s < beg))
	{
		beg = s;
		begLen = 1;
	}

	// 2008:140 LPR: Word fails on exactly "/…/…", so avoid it for now
	if (beg && (beg == text || beg[-1] != '/'))
	{
		s = beg + begLen;

		while (isspace(*s) && s - beg <= begLen + 5)
			s++;

		if (*s == '/')
		{
			char *slash = s;

			s++;
			while (isspace(*s) && s - slash <= 6)
				s++;

			if (!memcmp(s, "...", 3) || *s == '…')
			{
				mstr tDate;
				char *end = s;
				int endLen = *end == '…' ? 1 : 3;

				s += endLen;
				while (isspace(*s) && s - end <= endLen + 3)
					s++;

				// 2008:140 LPR: Word fails on exactly "…/…"
				if (end == slash + 1)
				{
					end--;
					tDate.cat("/");
					endLen += 1;
				}

				tDate.printf("%D", &date);

				if (memcmp(s, "г.", 2))
					tDate.cat("г.");

				Replace(end, endLen, str(tDate));
				Replace(beg, begLen, eventNo);
			}
		}
	}
}

bool TFilterDocument::FallBack(const char *fallWillOpen, const char *fallWasSaved, const char *name, TWaitWindow *wait)
{
	if (!fallWasSaved)
		return false;

	mstr m;

	if (THTMLFile::Show(name, wait, true))
		m.printf("%s за редактиране. Прегледайте го, при необходимост редактирайте съдържанието му и го", fallWillOpen);
	else
	{
		m.printf("%s под името %s. Отворете го, прегледайте съдържанието му, при необходимост го редактирайте и",
			fallWasSaved, name);
	}

	message("%s запишете, след което потвърдете.", str(m));
	return true;
}

static void pairs2mstr(TIArray<TReplacePair> *pairs, mstr &m)
{
	for (int i = 0; i < pairs->Count(); i++)
	{
		TReplacePair *pair = (*pairs)[i];

		strrep(pair->find, '\n', ' ');
		strrep(pair->find, '\v', ' ');
		m.printf("%s\v%s\n", pair->find, pair->replace);
	}
}

void TFilterDocument::Filter(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name,
	bool always)
{
	char tName[SIZE_OF_PATH];
	electric_name(container, date, tName, TYPE_HTML);

	if (!always && (!exist(name) || exist(tName)))
		return;

	TSession *session = NULL;
	TStringArray extraUcns;

	if (strchr(KIND_SESSIONS, container->kind))
		session = (TSession *) container;
	else if (strchr(KIND_ENDOCS, container->kind) || strchr(KIND_ENDOC_RIDERS, container->kind))
	{
		const TDecision *decision = (const TDecision *) container;

		session = new TSession;
		session->key = decision->key;
		session->date = decision->date;
		session->kind = decision->sessionKind;

		if (!session->Get("F_SECRETAR, " JUROR_NAMES_UNQUOTED))
		{
			delete session;
			session = NULL;
		}
	}

	if (session)
	{
		// LRQ: SCC: judge names should be visible
		// IRQ: SCC: secretar name should be visible
		extraUcns.Add(session->juror);
		extraUcns.Add(session->reJuror);
		extraUcns.Add(session->jurorX);
		extraUcns.Add(session->reserve);
	}

	// MAIN filtering
	bool saved = false;
	// 2008:137 LPR: +related: save in temp -> wait -> xfer if saved
	// 2009:162 wait += find/replace
	char temp[SIZE_OF_PATH];
	int strenght = 1 <<
		(strchr(KIND_SESSIONS, container->kind) &&
		((TSession *) container)->PersonalData()) <<
		(strchr(KIND_ENDOCS, container->kind) &&
		((TDecision *) container)->PersonalData());

	try
	{
		if (iApplication)
			GetText();	// reget if modified
		else
			Initialize(name);

		TPersonal personal(container, &extraUcns);
		TReplacePairs &repairs = personal.repairs;

		{
			TWaitWindow wait(parent, "Обезличаване и замяна на двойки текстове...");
			mstr title;

			personal.Filter(text, strenght, iActive);
			wait.SetText("Запис на документа във временен файл...");

			ole_invoke(iActive, "WebOptions", GET, 0);
			iWebOptions = (IDispatch *) res.punkVal;

			var[0].vt = VT_BOOL;
			var[0].boolVal = VARIANT_FALSE;
			ole_invoke(iWebOptions, "OrganizeInFolder", PUT, 1);

			// 2017:328
			var[0].vt = VT_BOOL;
			var[0].boolVal = VARIANT_FALSE;
		#if TESTVER
			//ole_invoke(iWebOptions, "OptimizeForBrowser", PUT, 1);
		#else  // TESTVER
			//ole_invoke(iWebOptions, "OptimizeForBrowser", PUT | W_OPTION, 1);
		#endif  // TESTVER

			// 2010:160 FIX: set document title
			ole_invoke(iActive, "BuiltInDocumentProperties", GET, 0);
			iProperties = (IDispatch *) res.punkVal;

			var[0].vt = VT_I4;
			var[0].lVal = 1;
			ole_invoke(iProperties, "Item", GET, 1);
			iTitle = (IDispatch *) res.punkVal;

			var[0].vt = VT_BSTR;
			title.printf("%s по %s %ld/%ldг.", Kind->Seek(kind2VisibleKind(container->kind)),
				Type->Seek(container->key.type), container->key.no, container->key.year);
			TBSTR bsTitle(str(title));
			var[0].bstrVal = bsTitle.bstr();
			ole_invoke(iTitle, "Value", PUT, 1);

			// slight chance of overwriting existing file
			build_fn(tempdir, basename(tName), temp);

			var[0].vt = VT_I4;
			var[0].lVal = FILTER_TYPE ? 10 : 8;		// [filtered] HTML
			var[1].vt = VT_BSTR;
			TBSTR bsSave(temp);
			var[1].bstrVal = bsSave.bstr();
			ole_invoke(iActive, "SaveAs", METHOD, 2);
			saved = true;
		}

		Visualize();
		// 2009:166 LPR: moved hFile loop in find replace dialog as timer
		{
			// 2017:199 LPR: show/hide alls here
			TWindowHideAlls hide(parent);
			TFindReplaceDialog(parent, this, &repairs.pairs).Execute();
		}

		mstr m;
		bool exist = repairs.Try("F_NO");

		pairs2mstr(&repairs.pairs, m);

		if (m.cmp(repairs.text))
		{
			if (any(m))
			{
				repairs.text.cpy(m);
				repairs.Save(exist);
			}
			else if (exist)
				repairs.Delete(true);
		}
	}
	catch (...)
	{
		Shutdown();

		if (saved)
			FallBack("Обезличения документ ще бъде отворен", "Обезличения документ е записан", temp);
	}

	extraUcns.Clear();
	if (!strchr(KIND_SESSIONS, container->kind))
		delete session;

	if (saved)
	{
		if (copy_overead_file(temp, tName, false, true))
		{
			html_group_action(tName, unlock_delete_file, VOID_TRUE);
			html_group_action(temp, copy_overead_file, tName);
			// 2011:138
			html_group_action(tName, clear_archive_attr, NULL);
			electric_log(LOG_ATTACH, container, tName, 0);
		}

	#if TESTVER
		// 2017:244
		if (strcmpi(name, temp))
		{
			unlock_delete_file(temp, false);
			html_group_action(temp, unlock_delete_file, VOID_FALSE);
		}
	#else  // TESTVER
		unlock_delete_file(temp, false);
		html_group_action(temp, unlock_delete_file, VOID_FALSE);
	#endif
	}
}

bool TFilterDocument::Number(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name,
	const char *tName, long filter, const TRegisterAct *act)
{
	bool saved = false;
	// 2008:137 LPR: +related: save in temp -> atomic CopyFile()
	char temp[SIZE_OF_PATH];
	bool result = false;

	try
	{
		Initialize(name);

		if (FILTER_NUMBER)
		{
			TLong *eventNo = (TLong *) container->Find("F_EVENT_NO");

			if (eventNo && eventNo->L())
			{
				char tEventNo[SIZE_OF_EVENT_NO];
				itoa(eventNo->L(), tEventNo, 10);

				if (!CheckNo(tEventNo))
				{
					TDate *date = (TDate *) container->Find("F_FROM_DATE");
					PlaceNo(tEventNo, date ? date->Date() : container->date);
				}
			}
		}

		if (FILTER_RUN_MACRO)
		{
			TBSTR bsMacro("CAC_FILTER_MACRO");

			var[0].vt = VT_BSTR;
			var[0].bstrVal = bsMacro.bstr();
			//memset(oleparams, '\0', sizeof oleparams);
			ole_invoke(iApplication, "Run", METHOD, 1);
		}

		// 2015:024 LPR/IRQ: original ext if any and needed
		const char *ext = file_ext(name);

		// 2015:287 LPR: tmpnamext() -> use tName basename
		build_fn(tempdir, basename(tName), temp);

		if (any(ext) && strcmpi(file_ext(temp), ext))
			strcpy(file_ext(temp), ext);

		var[0].vt = VT_BSTR;
		TBSTR bsSave(temp);
		var[0].bstrVal = bsSave.bstr();
		ole_invoke(iActive, "SaveAs", METHOD, 1);
		saved = true;
		result = copy_overead_file(temp, tName, false, true);

		if (result)
		{
			clear_archive_attr(name);
			electric_log(container, tName, act);
		}
	}
	catch (...)
	{
		Shutdown();
	}

	if (filter)
		Filter(parent, container, date, tName, saved);
	else
		Shutdown();

#if TESTVER
	if (saved)
#else  // TESTVER
	// 2017:243 LPR: temp may be removed by Filter -> exist()
	if (saved && exist(temp))
#endif  // TESTVER
		unlock_delete_file(temp, false);  // must be after shutdown

	return result;
}

bool TFilterDocument::Display(TWaitWindow *wait, const char *name, const char *fallWillOpen, const char *fallWasSaved)
{
	try
	{
		char tName[SIZE_OF_PATH];

		name = Initialize(name, tName);
		wait->ShowWindow(SW_SHOWNOACTIVATE);
		Visualize();
		WaitFor(wait, name);
		return true;
	}
	catch (...)
	{
		Shutdown();
		return FallBack(fallWillOpen, fallWasSaved, name);
	}
}

void TFilterDocument::Visualize()
{
	var[0].vt = VT_BOOL;
	var[0].boolVal = VARIANT_TRUE;
	ole_invoke(iApplication, "Visible", PUT, 1);
}

struct TFindWordWindow
{
	const char *base;
	HWND hWnd;
};

static BOOL CALLBACK find_word_window(HWND hWnd, LPARAM lParam)
{
	TFindWordWindow *find = (TFindWordWindow *) lParam;
	size_t len = strlen(find->base);
	char buffer[0x100];

	// 2016:155 LPR: compare base name case-insensitive
	if (GetClassName(hWnd, buffer, sizeof buffer) && !strcmp(buffer, "OpusApp") &&
		GetWindowText(hWnd, buffer, sizeof buffer) && !strncmpi(buffer, find->base, len))
	{
		static constant READ_ONLY = " (Read-Only)";
		size_t read_onlen = strlen(READ_ONLY);

		if (!strncmp(buffer + len, READ_ONLY, read_onlen))
			len += read_onlen;

		if (!strncmp(buffer + len, " - ", 3) && !strstr(buffer + len + 3, " - "))
		{
			find->hWnd = hWnd;
			return FALSE;
		}
	}

	return TRUE;
}

void TFilterDocument::WaitFor(TWaitWindow *wait, const char *name)
{
	mstr m;
	// 2014:203 LPR/URQ: force focus to OLE app
	TFindWordWindow find = { basename(name), NULL };

	m.printf("Затворете %s за да продължите...", find.base);
	// 2016:155 LPR: one more attempt to focus
	ole_invoke(iActive, "Activate", METHOD, 0);

	if (HIDE_ALL_ON_SAVE)
	{
		TWindowHideAlls hide(wait->GetParent());

		do
		{
			sleep_sync();
			wait->TickText(str(m));
		} while (IsOpen());

		sleep_sync();
		wait->DestroyWait();
	}
	else
	{
		do
		{
			sleep_sync();
			// 2014:064 LPR: SetText -> TickText to avoid flicker
			wait->TickText(str(m));

			if (!find.hWnd)
			{
				// 2016:203 LPR: removed MSO2KD "ActiveWindow", too many problems
				EnumWindows(find_word_window, (LPARAM) &find);

				if (find.hWnd)
					SetForegroundWindow(find.hWnd);
			}
		} while (IsOpen());

		sleep_sync();
		wait->SetText(WAIT);
		sleep_sync();
	}
}

void filter_office(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, bool always)
{
	TFilterDocument().Filter(parent, container, date, name, always);
}

bool filter_number(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, const char *tName,
	long filter, const TRegisterAct *act)
{
	return TFilterDocument().Number(parent, container, date, name, tName, filter, act);
}

bool filter_display(TWaitWindow *wait, const char *name, const char *fallWillOpen, const char *fallWasSaved)
{
	return TFilterDocument().Display(wait, name, fallWillOpen, fallWasSaved);
}
