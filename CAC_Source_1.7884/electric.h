enum
{
	ACTION_VIEW		= 0,
	ACTION_EDIT		= 1,
	ACTION_OPEN		= 2,
	ACTION_COUNT	= 3
};

struct TDoubleExt
{
	char type[SIZE_OF_EXT];
	char ltype[SIZE_OF_EXT];
};

struct TElectricType
{
	const char *whats;
	const char *what;
	const char *awhat;
	TDoubleExt ext[ELECTRIC_EXTS_MAX];
	char sourcePath[SIZE_OF_DIR];
	char docketPath[SIZE_OF_DIR];
	struct TElectricAction
	{
		char program[SIZE_OF_PATH];
		char arguments[SIZE_OF_ARGS];
	} Actions[ACTION_COUNT];
};

enum
{
	TYPE_TEXT		= 0,
	TYPE_IMAGE		= 1,
	TYPE_HTML		= 2,
	TYPE_COUNT		= 3,
	TYPE_EISPP		= 3,
	TYPE_PLAIN		= 4,
	TYPE_PORTREG	= 5,
	TYPE_TOTAL		= 6,
#if TESTVER
	TYPE_OFFICE		= 7,
#endif  // TESTVER
	TYPE_SBE_DESC	= 10
};

enum
{
	EXTRA_NO_XFER_MOTIVES	= 65025,
	EXTRA_NO_SUBPLIST_FIXED	= 65026,
	EXTRA_NO_SUBPLIST_OTHER = 65027,
	EXTRA_NO_OUTREG_SUP_APP	= 65028,
	EXTRA_NO_PROTECTIVE	= 65029,
	EXTRA_NO_IMMED_PROTECT	= 65530,
	EXTRA_NO_EXECUTIVE_BASE	= 65531,
	EXTRA_NO_EXECUTIVE_LAST = 65534
};

extern const long FILE_SIZE_MAX[TROUBLE_COUNT];		// 2015:292
extern TElectricType Electrics[TYPE_TOTAL];

bool make_directory(const char *name);	// note: file name is passed, not directory
bool name_shortened(const char *name, char *tName, bool force);	// !PK ? docx -> doc
const CDate &electric_date(const TRCDKeyContainer *container);
bool electric_matches(const char *name, int type, bool ltype, bool primary);

inline bool has_word_ext(const char *name) { return matches_exts(name, "doc", "docx", NULL); }
inline bool has_html_ext(const char *name) { return matches_exts(name, "htm", "html", NULL); }
inline bool has_docx_2cc(const char *data, size_t size) { return size >= 2 && data[0] == 'P' && data[1] == 'K'; }
inline const char *get_proper_word_ext(const char *data, size_t size) { return has_docx_2cc(data, size) ? "docx" : "doc"; } 

void electric_name(const TRCDKeyContainer *container, const char *dir, char *name, int type, int extraNo = -1);
void electric_name(const TRCDKeyContainer *container, const CDate &date, char *name, int type, int extraNo = -1);
void electric_name(const TRCDKeyContainer *container, char *name, int type, int extraNo = -1);
void electric_name(const TRCDKeyContainer *container, const CDate &date, TStringFace *name, int type, int extraNo = -1);
void electric_name(const TRCDKeyContainer *container, TStringFace *name, int type, int extraNo = -1);
void electric_name(const TSubpoena *subpoena, char *name, int type);

bool electric_move(TWindow *parent, const TRCDKeyContainer *container, const CDate &date, const char *name, int type,
	const TRegisterAct *act, int extraNo = -1);
bool electric_move(TWindow *parent, const TRCDKeyContainer *container, const char *name, int type, int extraNo);
void electric_move(const TSubpoena *subpoena, const char *name);
// 2015:331 parent = try to wait to finish and re-focus parent
bool electric_action(const char *name, int type, int action, TWindow *parent = NULL);
void electric_action(const TRCDKeyContainer *container, int type, int action, TWindow *parent = NULL);

void electric_edit(TWindow *parent, TStringFace *name, bool update, int type, bool primary = true);
bool electric_exist(const TRCDKeyContainer *container, int type);

// 2008:107
#define VOID_TRUE ((void *) true)
#define VOID_FALSE ((void *) false)
#define BUILD_REDIR -1

bool lock_unlock_file(const char *name, bool lock);
void lock_unlock_file(const char *name, const void *lock);
void clear_archive_attr(const char *name, const void * = NULL);	// 2011:138 LPR: also a group action
bool unlock_delete_file(const char *name, bool unlock);
void unlock_delete_file(const char *name, const void *unlock);

bool copy_overead_file(const char *source, const char *where, int build, bool makedir);
void copy_overead_file(const char *name, const void *dir);
bool finish_find(HANDLE hFind, const char *name, bool is_html);
void html_group_action(const char *name, void (*action)(const char *name, const void *data), const void *data);

void float_electric(TWindow *parent);	// 2010:004 LPR: entry point, moved here from getfloat
void electric_log(char gop, const TRCDKeyContainer *container, const char *name, int extraNo);
void electric_log(const TRCDKeyContainer *container, const char *tName, const TRegisterAct *act);
void electric_rename(const char *name, TRCDKeyContainer *target, int type, bool html_group);
void electric_rename(TRCDKeyContainer *source, TRCDKeyContainer *target);
void electric_rename(TOriginContainer *container);
void electric_copy(const char *source, const char *target, sign group_action = -1);
void electric_temp(const TRCDKeyContainer *container, char *name, int type);
bool electric_rekind_access_check();
bool electric_rekind(const TRCDKeyContainer *container, char targetKind, const CDate &targetDate);
