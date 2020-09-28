// ----- TWSTR ---------------------------------------------------------------
class TWSTR
{
public:
	TWSTR(const char *s, int len = -1, bool crash = true);
	~TWSTR() { delete[] ws; }

	wchar_t *wstr() { return ws; }
	wchar_t **wspr() { return &ws; }

protected:
	wchar_t *ws;
};

// ----- TBSTR ---------------------------------------------------------------
class TBSTR
{
public:
	TBSTR(const char *s, int len = -1, bool crash = true);
	TBSTR(const wchar_t *ws, size_t len) { Allocate(ws, len); }
	~TBSTR() { SysFreeString(bs); }

	BSTR bstr() { return bs; }

protected:
	void Allocate(const wchar_t *ws, size_t len);

	BSTR bs;
};

// ----- functions -----------------------------------------------------------
bool filter_replace(IDispatch *iActive, const char *text, int start, const int len, const char *rep, const int replen);
bool filter_replace(IDispatch *iActive, const wchar_t *text, int start, const int len, const wchar_t *rep, const int replen);
void filter_office(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, bool always);
bool filter_number(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, const char *tName,
	long filter, const TRegisterAct *act);
bool filter_display(TWaitWindow *wait, const char *name, const char *fallWillOpen, const char *fallWasSaved);
