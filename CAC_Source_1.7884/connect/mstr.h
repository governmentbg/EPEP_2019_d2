// ----- rtl extras ----------------------------------------------------------
int strindex(const char *s, char c);	// INT_MAX if missing
int strindex(const char *s, const char *t);
char *strnew(const char *s);			// strdup() via new
int strrindex(const char *s, char c);
char *strrep(char *s, char source, char target);

// ----- mstr ----------------------------------------------------------------
class mstr : protected memblock_t
{
public:
	mstr();
	mstr(const char *s);
	~mstr() { mbk_clear(this); }

	char operator[](size_t n) { return data[n]; }
	void cach(char c) { catn(&c, 1); }
	void cach_nx(char c);
	void catn(const char *s, size_t len);
	void cat(const char *s);
	void cat(const mstr &m) { cat(m.str()); }
	void cat_nx(const char *chars);
	void cat_quot(const char *s);
	void cat_quoted(const char *prefix, const char *s);
	void catset(const char *set);
	void clear();
	int cmp(const char *s) const { return strcmp(str(), s); }
	int cmp(const mstr &m) const { return cmp(m.str()); }
	const char *chr(int c) const { return strchr(str(), c); }
	void compress(size_t pos = 0, const char *spaces = NULL);
	void cpy(const char *s);
	void cpy(const mstr &m) { cpy(m.str()); }
	void cut(size_t len);
	void cut(const char *s) { cut(s - str()); }
	void cutell(size_t len);
	void delead(size_t len) { mbk_remove(this, 0, len); }
	void delead(const char *s) { delead(s - str()); }
	const char *end() { return str() + len(); }
	void fixize();
	int index(char c) const { return strindex(str(), c); }
	int index(const char *s) { return strindex(str(), s); }
	size_t len() const { return size - 1; }
	void lower() { strlwr(ncstr()); }
	char *ncstr() const { return (char *) data; }
	char *newstr() const { return strnew(str()); }
	virtual void puts(const char *s) { cat(s); }
	void quote(size_t len);
	void pad(size_t len);
	void printf(const char *format, ...);
	void rep(char source, char target) { strrep(ncstr(), source, target); }
	const char *rchr(int c) const { return strrchr(str(), c); }
	int rindex(char c) const { return strrindex(str(), c); }
	void sep(const char *s);
	void setlen(size_t len);
	void _setsize(size_t size) { mbk_resize(this, size); }
	const char *str() const { return (const char *) data; }
	void trim_all() { trim_lead(); trim_trail(); }
	void trim_at(int pos);
	void trim_lead() { trim_at(0); }
	void trim_trail();
	void upper() { strupr(ncstr()); }
	virtual void vprintf(const char *format, va_list ap);
};

// ----- functions -----------------------------------------------------------
inline bool any(const mstr &m) { return m.len(); }
inline bool any(const char *s) { return *s; }
void itom(int i, mstr &m);						// cat, 0 -> nothing
inline char *ncstr(mstr &m) { return m.ncstr(); }		// exception
inline const char *str(const mstr &m) { return m.str(); }
inline const char *str(const mstr *m) { return m->str(); }	// _ only

bool strequal(const char *s1, const char *s2);
inline void strmove(char *s1, const char *s2) { memmove(s1, s2, strlen(s2) + 1); }
char *compress(char *const s, const char *spaces = NULL);	// used by mstr
void squeeze(char *s);	// remove spaces adjanced to puncts
const char *skiplead(const char *s);	// leading spaces
