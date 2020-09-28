// tmp
const char *basename(const char *name);
enum { SIZE_OF_DIR = 200 + 1 };
// ".\\TMP"
extern char *TEMPDIR;
// Invoke obtain_tempdir() before use!
extern char *tempdir;
// Obtain tempdir from TMP, TEMP or as last resort TEMPDIR
void obtain_tempdir();
// misc
bool exist(const char *path, bool file = true);
// utility
void calc_simple_xor(unsigned char *password, size_t size);
void memzero_s(char *p, size_t size);
class DtorZero	// 2016:174
{
public:
	DtorZero(char *tData, size_t tSize) : data(tData), size(tSize) { }
	~DtorZero() { memzero_s(data, size); }

private:
	char *const data;
	const size_t size;
};

// memblock
extern constant AppNameName;
#define APPNAMENAME AppNameName
#define MessageBoxTitle AppNameName

// datetime
struct CDateTime
{
	int year, month, day;
	int hour, minute, second;
};

// database
enum { FLAG_ACTIVE = 1 };
inline void failure() { ExitProcess(1); }
