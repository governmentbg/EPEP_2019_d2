void SetWaitCursor(bool wait);
bool atob(const char *ucn);	// atof(s) != 0
long atobc(constant ucn[], size_t count);
void catset(char *s, const char *set);
bool exist(const char *path, bool file = true);
bool exist_document(const char *name);	// error message if not exist
char *globcmpi(char *text, const char *patt);	// endpos or NULL
bool intersect(const char *s1, const char *s2);
bool not_inset(const char *test, const char *full);
inline bool non_equal(const char *s1, const char *s2) { return not_inset(s1, s2) || not_inset(s2, s1); }
int scprintf(char *s, const char *format, ...);
char *split_half(char *s);
char *split_at(char *s, int len);
void strcach(char *s, char c);
void strcach_nx(char *s, char c);
void strcat_nx(char *s, const char *chars);
bool strcatell(char *buffer, const char *s, const char *sep, int size);
void stremove(char *from, const char *what);
void stremove(char *from, char c);
void strimall(char *s);
void strimtrail(char *s);
bool strinex(const char *inex, char c);
bool strinex2(const char *inex, char c1, char c2);
bool strmatch(const char xCrit, const char *xData);
bool strmatch(const char *xCrit, const char *xData);
char *strpad(char *s, int len);
char *strsetlen(char *s, int len);
char *strzcpy(char *s, const char *ts, size_t size);
void sum_in_words(double sum, mstr &m, const char *currency, bool zerofrac);
inline void sum_in_words(double sum, mstr &m) { sum_in_words(sum, m, NULL, true); }
long stat2utime(long st_time);	// 2017:019 FIX: these differ by TZ
bool alnumdif(char c1, char c2);	// 2017:082
bool kowpress(char *buffer);		// 2017:081 keywords
bool absolute(const char *path);	// 2017:292
// 2017:299; 2018-10-30: defaultChar -> '?' or similar letter
char *tocp1251(const wchar_t *ws);
char *cliptext(TWindow *parent);

// ----- TStringArray --------------------------------------------------------
#define TStringArray TNTStringArray
class TStringArray : public TPointerArray
{
public:
	TStringArray(bool sorted = false);	// true = case-insensitive sort

	char *operator[](int index) const { return (char *) ItemAt(index); }
	void Add(const char *s) { TPointerArray::Add(s); }
	void Insert(const char *s, int index) { TPointerArray::Insert(s, index); }
	int Find(const char *s) { return TPointerArray::Find(s); }

protected:
	static void DeleteString(void *s);
};
