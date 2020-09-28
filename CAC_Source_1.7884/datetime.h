// ----- encoding ------------------------------------------------------------
void initialize_dates();
void shutdown_dates();

// ----- CDate ---------------------------------------------------------------
enum
{
	YEAR_NUL = 1901,
	YEAR_MIN = 1970,
	YEAR_MAX = 2037
};

#define GOD_SUFFIX " ã."

// 2011:129 SGS: bankrupts: ZEROED_DATES support
// 2013:199 LPR: killed: changed bankrupts output
extern constant
	C_DATE_STD,		// "%D"
	C_DATE_GOD;		// "%Dã."

struct CDate : DateValue
{
	static int DaysInMonth(long year, int month);
	static void InitDates();

	CDate() { Clear(); }
	CDate(int tDay, int tMonth, long tYear);
	CDate(const char *ucn);

	void AddMonths(int months);
	void Advance(char interval, long count, long delta);
	void Clear();
	int Compare(const CDate &date) const;
	int DayOfWeek() const;
	int DayOfYear() const;
	int DaysInMonth() const;
	char DigitOfWeek() const { return (char) (DayOfWeek() + '0'); }
	bool Empty() const;
	bool Leap() const;
	void Print(mstr &m, const char *format = C_DATE_STD) const;
	void SetValue(long value);
	bool Valid() const { return Valid(YEAR_MIN); }
	bool ValidEx() const { return Valid(YEAR_NUL); }
	long Value() const;
	bool Work() const;
	void YearsBack(int years);
	void YearsForward(int years);

protected:
	static int dom[12];

	static void InitAgos();
	static bool Leap(long year);

	int CValue() const;
	void Randomize() const;
	void SetValue(long value, int &hour, int &minute);
	long Value(int hour, int minute) const;
	bool Valid(long yearMin) const;
};

extern const CDate DATE_NUL;
extern const CDate DATE_MIN;
extern const CDate DATE_MAX;
extern CDate WeekAgo;
extern CDate YearAgo;

inline bool operator==(const CDate &date1, const CDate &date2) { return date1.Compare(date2) == 0; }
inline bool operator!=(const CDate &date1, const CDate &date2) { return date1.Compare(date2) != 0; }
inline bool operator>(const CDate &date1, const CDate &date2) { return date1.Compare(date2) > 0; }
inline bool operator>=(const CDate &date1, const CDate &date2) { return date1.Compare(date2) >= 0; }
inline bool operator<=(const CDate &date1, const CDate &date2) { return date1.Compare(date2) <= 0; }
inline bool operator<(const CDate &date1, const CDate &date2) { return date1.Compare(date2) < 0; }

CDate &operator+=(CDate &date, int days);
int operator-(const CDate &date2, const CDate &date1);	// in months

// ----- CTime ---------------------------------------------------------------
extern constant C_TIME_STD;	// "%T"

struct CTime : TimeValue
{
	CTime() { Clear(); }
	CTime(int tHour, int tMinute, int tSecond = 0);

	bool Bef() const { return (hour < 12 || (hour == 12 && minute < 30)); }
	void Clear();
	int Compare(const CTime &time) const;
	bool Empty() const;
	void Print(mstr &m, const char *format = C_TIME_STD) const;
	void SetValue(long value);
	bool Valid() const;
	long Value() const;

protected:
	int CValue() const;
};

CTime Now();
CTime LNow();		// seconds = 0
extern const CTime TIME_MIN;
extern const CTime TIME_MAX;
extern const CTime TIME_PM;	// 12:30:00
extern const CTime TIME_LMAX;	// 23:59:00

inline bool operator==(const CTime &time1, const CTime &time2) { return time1.Compare(time2) == 0; }
inline bool operator!=(const CTime &time1, const CTime &time2) { return time1.Compare(time2) != 0; }
inline bool operator>(const CTime &time1, const CTime &time2) { return time1.Compare(time2) > 0; }
inline bool operator>=(const CTime &time1, const CTime &time2) { return time1.Compare(time2) >= 0; }
inline bool operator<=(const CTime &time1, const CTime &time2) { return time1.Compare(time2) <= 0; }
inline bool operator<(const CTime &time1, const CTime &time2) { return time1.Compare(time2) < 0; }

int operator-(const CTime &time2, const CTime &time1);	// 2002:003 In minutes
CTime &operator+=(CTime &time, int minutes);

// ----- CDateTime -----------------------------------------------------------
extern constant
	C_DATE_TIME_STD,		// %d.%d.%ld %d.%02d.%02d
	C_DATE_TIME_FIXIZE;	// %2d.%2d.%ld %2d.%02d.%02d

enum SYNC_SERVER
{
	SYNC_EXACT,
	SYNC_PRECISE,
	SYNC_AVERAGE,
	SYNC_COUNT
};

class TQuery;

struct CDateTime : public CDate, public CTime
{
	CDateTime();
	CDateTime(int day, int month, long year, int hour, int minute, int second = 0);

	static void SyncServer(SYNC_SERVER sync);		// 2015:005 +related

	const CDate &Date() const { return (const CDate &) *this; }
	const CTime &Time() const { return (const CTime &) *this; }

	void Clear();
	bool Get(const pFLDDesc desc, const TQuery *query);
	int Compare(const CDateTime &stamp) const;
	bool Empty() const;
	void Print(mstr &m, const char *format = C_DATE_TIME_STD) const;
	void SetValue(long value);
	bool Valid() const;
	long Value() const;

private:
	static int cache_count[SYNC_COUNT];
	static int cache_delta[SYNC_COUNT];
};

extern const CDateTime DATETIME_MIN;
extern const CDateTime DATETIME_MAX;
extern CDateTime _CurrentDateTime;
extern const CDate &Today;

inline bool operator==(const CDateTime &stamp1, const CDateTime &stamp2) { return stamp1.Compare(stamp2) == 0; }
inline bool operator!=(const CDateTime &stamp1, const CDateTime &stamp2) { return stamp1.Compare(stamp2) != 0; }
inline bool operator>(const CDateTime &stamp1, const CDateTime &stamp2) { return stamp1.Compare(stamp2) > 0; }
inline bool operator>=(const CDateTime &stamp1, const CDateTime &stamp2) { return stamp1.Compare(stamp2) >= 0; }
inline bool operator<=(const CDateTime &stamp1, const CDateTime &stamp2) { return stamp1.Compare(stamp2) <= 0; }
inline bool operator<(const CDateTime &stamp1, const CDateTime &stamp2) { return stamp1.Compare(stamp2) < 0; }
