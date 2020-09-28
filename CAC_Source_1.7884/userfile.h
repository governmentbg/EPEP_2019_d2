// ----- TUserFile -----------------------------------------------------------
class TUserFile
{
public:
	TUserFile(const char *tWhat);
	~TUserFile();

	// fopen() and display error unless silent
	FILE *Open(const char *name, const char *mode);
	// read the entire file and display error
	char *ReadAll(const char *name, long &size);
	// read the entire file and display error
	bool ReadAll(const char *name, memblock_t *m, bool text, long limit = INT_MAX, long *mtime = NULL);
	// write, display error and remove on failure
	bool WriteAll(const char *name, const void *data, long size = -1, long mtime = 0);
	// write into an OpenTemp file and return name
	const char *WriteTemp(const char *ext, const void *data, long size = -1);
	// fflush() ferror() fclose() and display error
	bool Close();
	// unlink() and display error
	bool Remove();
	// <name>: <error> <func> <what>
	bool Error(const char *func);
	// CopyFile(FALSE) and display error
	static bool Copy(const char *source, const char *target);

protected:
	const char *what;
	const char *name;
	char temp[SIZE_OF_PATH];

	const char *rdwr;
	FILE *f;

	// temporary name, fatal on error
	FILE *OpenTemp(const char *suffix, const char *ext, const char *mode, bool autoUnlink);
	// low-level silent fopen()
	FILE *OpenFile(const char *tName, const char *mode);
	// <name>: <error> <func> <what>
	void GetError(mstr &m, const char *func);
	// write into already open file
	bool WriteAll(const void *text, long size, time_t mtime);
};

// ----- TTextFile -----------------------------------------------------------
class TWaitWindow;

class TTextFile : protected TUserFile
{
public:
	TTextFile(const char *what = NULL);

	// default ext "txt", mode always "w"
	FILE *OpenTemp(const char *ext = NULL);
	// existing name, mode always "a"
	FILE *ReopenTemp();
	// open as "wb", write text and close
	void Save(const char *name, const char *text);
	// set wait message and launch notepad
	static bool Show(const char *name, TWaitWindow *wait);
	// close if needed and invoke Show(name)
	bool Show(TWaitWindow *wait);
	// 2014:027 LPR: get open/save name
	void Close() { TUserFile::Close(); }
	void Remove() { TUserFile::Remove(); }
	// show file open dialog for .txt files and open "r"
	FILE *OpenRead(TWindow *parent, char *initialDir);
	// show file save dialog for .txt files [with default name] and open "w"
	FILE *OpenWrite(TWindow *parent, char *initialDir, const char *defaultName = NULL);
	// 2014:344 distrib protocol
	char *ReadAll();
};

// ----- THTMLFile -----------------------------------------------------------
#ifdef putc
#undef putc
#endif  // putc

class THTMLFile : protected TUserFile
{
public:
	THTMLFile(const char *what = NULL);

	// "htm", "wb", header
	const char *OpenTemp();
	// "htm", "wb", header ? "_header"
	const char *OpenOffice(bool header);
	// emit html footer if needed and close
	bool Close();
	// set wait message and edit/view
	static bool Show(const char *name, TWaitWindow *wait, bool editor, bool office = false);
	// close if needed and invoke Show(name); 2016:021 sign -1 = header (was bool false)
	bool Show(TWaitWindow *wait = NULL, bool editor = true, sign office = -1);
	bool SaveBody(const char *target);	// 2017:219 for corrupt 

	static void puts(FILE *f, const char *s, char br);
	void puts(const char *s, char br) { puts(f, s, br); }
	virtual void puts(const char *s) { puts(s, '\n'); }
	void putc(char c) { puts(C2S[c]); }
	virtual void vprintf(const char *format, va_list ap);
	void printf(const char *format, ...);
	// print date if not empty, no default format
	void putd(const char *format, const CDate &date);

	// email xfer
	const char *OpenEMail();	// "htm", "wb+", no neader
	void ReadBack(char *s, long offset, int len);

	// office
	void MsPageBr();
	bool OpenRetry(const char *tName);
	void Write(const char *s, int size) { fwrite(s, size, 1, f); }
	void PutRaw(const char *s) { fputs(s, f); }

	// querydoc
	void qutn(const char *s);
	void qutp(const char *s, size_t size);
	void qutp(const mstr &m, size_t size) { qutp(str(m), size); }
	void qutp(TBaseString *str) { qutp(str->S(), str->Size); }
	void qutt(int n);
	void font(bool small);

protected:
	bool header;
	static DWORD lastTicks;
};

// ----- TXMLError -----------------------------------------------------------
class TRCDKeyContainer;

class TXMLError
{
public:
	TXMLError(const char *lead, const char *format, va_list ap);

	void Handle();
	void Handle(TRCDKeyContainer *container);

protected:
	mstr m;
	bool silent;	// if *format == '-'
};

void xml_error(const char *lead, const char *format, ...);
void xml_error_now(TRCDKeyContainer *container, const char *lead, const char *format, ...);

// ----- XMLError2 -----------------------------------------------------------
class TXMLError2 : public TXMLError
{
public:
	TXMLError2(const char *lead, const char *format, va_list ap);
};

// ----- TXMLFile ------------------------------------------------------------
class TRCDKey;

class TXMLFile : protected TUserFile
{
public:
	TXMLFile(const char *what = NULL);

	static constant BOM;
	static constant HEADER;

	bool OpenBank(mstr &e, const char *suffix, TRCDKeyContainer *container);
	bool OpenLegal(mstr &e, const char *name, TRCDKeyContainer *container);
	bool OpenSms(mstr &e, const char *name);
	void OpenTag(const char *tag) { OpenTag(tag, true); }
	void CloseTag(const char *tag) { CloseTag(tag, true); }
	// open tag, output data and close tag
	void Emit(const char *tag, const char *s);
	void Emit(const char *tag, long l);
	void EmitM(const char *tag, double d);	// money
	void Emit(const char *tag, const CDate &date);
	void Emit(const char *tag, const CTime &time);	// may be empty
	void Emit(const char *tag, const CDate &date, const CTime &time);	
	void EmitBase64(const char *tag, char *data, long size);	// 2013:344
	// 2011:292; 2013:344 LPR: +what (was "graphical")
	bool EmitFile(const char *tag, const char *name, const char *what);
	void PrintF(const char *tag, const char *format, ...);
	void PutRaw(const char *s, bool indent);
	void Put(const char *s);
#if RANGECHECK
	bool Close();	// check tag level
#else  // RANGECHECK
	bool Close() { return TUserFile::Close(); }
#endif  // RANGECHECK
	bool IsValid() { return f && !ferror(f); }
	void Remove();	// Close and Remove
	// 2012:171
	char *ReadAll(const char *name);
	bool OpenWrite(const char *name);
	void OpenAttrib(const char *name) { fprintf(f, "%s=\"", name); }
	void CloseAttrib() { fputc('"', f); }
	void OpenObject(const char *name);
	void CloseHeader() { fputs(">\n", f); }
	void CloseObject(const char *name) { CloseTag(name, true); }

protected:
	int level;

	bool OpenLL(mstr &e, const char *what, TRCDKeyContainer *container);
	void Indent();
	void OpenTag(const char *tag, bool user);
	void CloseTag(const char *tag, bool user);
	char *Parse(const char *tText);
};

void utf8_parse(char *text);
void utf8_write(char c, mstr &m);
