#include "cac.h"

// ----- encoding ------------------------------------------------------------
enum
{
	BASE_PERIOD	= 65,
	MIN_DELTA	= 18,
	BASE_COUNT	= 33728,
	CALC_COUNT	= 64,
	RAND_RANGE	= 1008
};

enum { LONG_START	= -681879291 }; //-101902203;

static unsigned short bases[BASE_COUNT] =
{
#include "hbase66k.cpp"
};

struct TDateCell
{
	bool initialized;
	long base;
	long calc[CALC_COUNT];

	void Initialize();
};

void TDateCell::Initialize()
{
	if (!initialized)
	{
		long vcalc = base;

		my_srand(vcalc);
		for (int n = 0; n < CALC_COUNT; n++)
		{
			unsigned short input;

			my_rand();
			while ((input = my_rand()) >= (BASE_PERIOD * RAND_RANGE) / 2);
			calc[n] = vcalc += (input % RAND_RANGE) + MIN_DELTA;
			my_rand();
		}

		initialized = true;
	}
}

static TDateCell *data = NULL;

// ----- extern --------------------------------------------------------------
void initialize_dates()
{
	long value = LONG_START - (BASE_PERIOD * RAND_RANGE) / 2;

	data = new TDateCell[BASE_COUNT];
	for (int i = 0; i < BASE_COUNT; i++)
	{
		data[i].initialized = false;
		data[i].base = value += bases[i] + BASE_PERIOD * MIN_DELTA;
	}
}

void shutdown_dates()
{
	delete[] data;
	data = NULL;
}

// ----- CDate ---------------------------------------------------------------
constant
	C_DATE_STD = "%D",
	C_DATE_GOD = "%Dã.";

int CDate::dom[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int CDate::DaysInMonth(long year, int month)
{
	return dom[month - 1] + (month == 2 && Leap(year));
}

void CDate::InitAgos()
{
	YearAgo = Today;

	YearAgo.year--;
	// 2008:060 FIX: was YearAgo.day++;
	if (!YearAgo.Valid())
		YearAgo += 1;

	WeekAgo = Today;
	WeekAgo += -7;
}

void CDate::InitDates()
{
	time_t timer = time(NULL);
	struct tm *tblock = localtime(&timer);

	_CurrentDateTime.day = tblock->tm_mday;
	_CurrentDateTime.month = tblock->tm_mon + 1;
	_CurrentDateTime.year = tblock->tm_year + 1900;
	InitAgos();
}

bool CDate::Leap(long year)
{
	return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
}

CDate::CDate(int tDay, int tMonth, long tYear)
{
	day = tDay;
	month = tMonth;
	year = tYear;
}

CDate::CDate(const char *ucn)
{
	Clear();

	if (strlen(ucn) == 10)
	{
		int i;

		for (i = 0; i < 10; i++)
			if (!isdigit(ucn[i]))
				break;

		// YYMMDDNNNN
		if (i == 10 && ucn[2] <= '5')
		{
			day = (ucn[4] - '0') * 10 + (ucn[5] - '0');
			month = (ucn[2] - '0') * 10 + (ucn[3] - '0');
			year = (ucn[0] - '0') * 10 + (ucn[1] - '0');

			if (month >= 40)
			{
				month -= 40;
				year += 2000;
			}
			else if (month >= 20)
			{
				month -= 20;
				year += 1800;
			}
			else
				year += 1900;
		}
	}
}

void CDate::AddMonths(int count)
{
	while (count-- > 0)
	{
		if (++month > 12)
		{
			month = 1;
			year++;
		}
	}

	if (day > DaysInMonth())
		day = DaysInMonth();
}

void CDate::Advance(char interval, long count, long delta)
{
	switch (interval)
	{
		case INTERVAL_EXACT :
		{
			if (count)
				fatal("%c, %d: bad advance/count combination", interval, count);
			break;
		}
		case INTERVAL_WORK_DAYS :
		{
			while (count-- > 0)
			{
				*this += 1;
				while (!Work())
					*this += 1;
			}
			break;
		}
		case INTERVAL_DAYS : *this += count + 1; break;
		case INTERVAL_WEEKS : *this += 7 * count; break;
		case INTERVAL_MONTHS : AddMonths(count); break;
		case INTERVAL_YEARS :
			year += count;
			if (day > DaysInMonth())
				day = DaysInMonth();
			break;
		default : fatal("Advance: unknown interval %d", interval);
	}

	*this += delta;

	while (!Work())
		*this += delta >= 0 ? 1 : -1;
}

void CDate::Clear()
{
	day = month = year = 0;
}

int CDate::Compare(const CDate &date) const
{
	return CValue() - date.CValue();
}

int CDate::CValue() const
{
	return year * 10000 + month * 100 + day;
}

static unsigned Jday(unsigned m, unsigned d, unsigned y)
{
	unsigned long c, ya;

	if (m > 2)
		m -= 3;
	else
	{
		m += 9;
		y--;
	}

	c = y / 100;
	ya = y - 100 * c;

	return ((146097L * c) >> 2) + ((1461 * ya) >> 2) + (153 * m + 2)/5 + d + 1721119L;
}

int CDate::DayOfWeek() const
{
	return (((((Jday(month, day, year) + 1) % 7) + 6) % 7) + 1);
}

int CDate::DayOfYear() const
{
	int doy = day;

	for (int tMonth = 1; tMonth < month; tMonth++)
		doy += DaysInMonth(year, tMonth);

	return doy;
}

int CDate::DaysInMonth() const
{
	return dom[month - 1] + (month == 2 && Leap());
}

bool CDate::Empty() const
{
	return !CValue();
}

bool CDate::Leap() const
{
	return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
}

void CDate::Print(mstr &m, const char *format) const
{
	if (!Empty())
		m.printf(format, (DateValue *) this);
}

void CDate::Randomize() const
{
	my_srand((year - YEAR_MIN) * 372 + (month - 1) * 31 + day);
}

void CDate::SetValue(long value)
{
	int hour, minute;

	SetValue(value, hour, minute);
	Randomize();
	if (hour != my_rand() % 26 - 1 || minute != my_rand() % 60)
		fatal("CDate::SetValue %ld", value);
}

void CDate::YearsBack(int years)
{
	year -= years;

	if (year < YEAR_MIN)
		*this = DATE_MIN;
	else if (!Valid())
		*this += 1;
}

void CDate::YearsForward(int years)
{
	year += years;

	if (year < YEAR_MAX)
		*this = DATE_MAX;
	else if (!Valid())
		*this += -1;
}

static long get_base(int index)
{
	TDateCell *data = ::data + index / BASE_PERIOD;
	int calc_index = index % BASE_PERIOD;
	data->Initialize();
	return calc_index == 0 ? data->base : data->calc[calc_index - 1];
}

enum
{
	DATE_VALUE_NUL = -681864178,
	DATE_VALUE_MIN = -681863977,
	DATE_VALUE_MAX = +452949629
};

void CDate::SetValue(long value, int &hour, int &minute)
{
	if (value == DATE_VALUE_NUL)
	{
		*this = DATE_NUL;
		hour = minute = 0;
	}
	else
	{
		int i;
		int l = 0, h = (BASE_COUNT * BASE_PERIOD) - 1;
		long base;

		for (;;)
		{
			i = (l + h) / 2;
			base = get_base(i);

			if (value < base)
				h = i - 1;
			else if (value >= base + MIN_DELTA)
				l = i + 1;
			else
				break;

			if (h < l)
				fatal("CDate::SetValue %ld", value);
		}

		long full_value = i * MIN_DELTA + value - base;

		minute = full_value % 60;
		full_value /= 60;
		hour = full_value % 26 - 1;
		full_value /= 26;
		day = full_value % 31 + 1;
		full_value /= 31;
		month = full_value % 12 + 1;
		full_value /= 12;
		year = full_value + YEAR_MIN;

		if (!Valid())
			fatal("CDate::SetValue %ld", value);
	}
}

bool CDate::Valid(long yearMin) const
{
	return year >= yearMin && year <= YEAR_MAX && month >= 1 && month <= 12 && day >= 1 && day <= DaysInMonth();
}

long CDate::Value() const
{
#if RANGECHECK
	if (Empty())
		fatal("CDate::Value Empty");
#endif  // RANGECHECK
	if (*this == DATE_NUL)
		return DATE_VALUE_NUL;

	Randomize();
	int hour = my_rand() % 26 - 1;	// 2011:181 FIX: -1
	int minute = my_rand() % 60;
	return Value(hour, minute);
}

long CDate::Value(int hour, int minute) const
{
	long full_value = (year - YEAR_MIN) * 580320 + (month - 1) * 48360 + (day - 1) * 1560 + (hour + 1) * 60 + minute;
	return get_base(full_value / MIN_DELTA) + full_value % MIN_DELTA;
}

bool CDate::Work() const
{
	return Calendar->Work(year, month, day);
}

const CDate DATE_NUL(1, 1, YEAR_NUL);
const CDate DATE_MIN(1, 1, YEAR_MIN);
const CDate DATE_MAX(31, 12, YEAR_MAX);
CDate WeekAgo;
CDate YearAgo;

CDate &operator+=(CDate &date, int days)
{
	if (days > 0)
	{
		do
		{
			if (++date.day > date.DaysInMonth())
			{
				if (++date.month > 12)
				{
					date.year++;
					date.month = 1;
				}
				date.day = 1;
			}
		} while (--days > 0);
	}
	else if (days < 0)
	{
		do
		{
			if (--date.day < 1)
			{
				if (--date.month < 1)
				{
					date.year--;
					date.month = 12;
				}
				date.day = date.DaysInMonth();
			}
		} while (++days < 0);
	}

	return date;
}

int operator-(const CDate &date2, const CDate &date1)
{
	return (date2.year - date1.year) * 12 + date2.month - date1.month - (date2.day < date1.day);
}

// ----- CTime ---------------------------------------------------------------
constant C_TIME_STD = "%T";

CTime::CTime(int tHour, int tMinute, int tSecond)
{
	hour = tHour;
	minute = tMinute;
	second = tSecond;
}

void CTime::Clear()
{
	hour = minute = second = 0;
}

int CTime::Compare(const CTime &time) const
{
	return CValue() - time.CValue();
}

int CTime::CValue() const
{
	return hour * 10000 + minute * 100 + second;
}

bool CTime::Empty() const
{
	return !CValue();
}

void CTime::Print(mstr &m, const char *format) const
{
	if (!Empty())
		m.printf(format, (TimeValue *) this);
}

void CTime::SetValue(long value)
{
	if (value < 0 || value >= 1440)
		fatal("CTime::SetValue value %ld", value);

	hour = value / 60;
	minute = value % 60;
}

bool CTime::Valid() const
{
	return hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60;
}

long CTime::Value() const
{
	if (second)
		fatal("CTime::Value contains second");

	return hour * 60 + minute;
}

int operator-(const CTime &time2, const CTime &time1)
{
	return (time2.hour - time1.hour) * 60 + time2.minute - time1.minute;
}

CTime &operator+=(CTime &time, int minutes)
{
	int value = time.hour * 60 + time.minute + minutes;

	if (value < 0 || (time.hour = value / 60) >= 24)
		fatal("time over/underflow: v = %d, tm = %d, m = %d", value, time.minute, minutes);

	time.minute = value % 60;
	return time;
}

static CTime Now(bool seconds)
{
	CDateTime::SyncServer(SYNC_AVERAGE);
	return CTime(_CurrentDateTime.hour, _CurrentDateTime.minute, seconds ? _CurrentDateTime.second : 0);
}

CTime Now() { return Now(true); }
CTime LNow() { return Now(false); }

const CTime TIME_MIN(0, 0, 0);
const CTime TIME_MAX(23, 59, 59);
const CTime TIME_PM(12, 30, 0);
const CTime TIME_LMAX(23, 59, 0);

// ----- CDateTime -----------------------------------------------------------
constant
	C_DATE_TIME_STD		= "%d.%d.%ld %d:%02d:%02d",
	C_DATE_TIME_FIXIZE	= "%2d.%2d.%ld %2d:%02d:%02d";

CDateTime::CDateTime()
{
}

CDateTime::CDateTime(int day, int month, long year, int hour, int minute, int second)
	: CDate(day, month, year), CTime(hour, minute, second)
{
}

int CDateTime::cache_count[SYNC_COUNT] = { 0, 5, 10 };
int CDateTime::cache_delta[SYNC_COUNT] = { 0, 4, 40 };

#if TESTVER
extern FILE *datim;
#endif  // TESTVER

class IDateTime : public ISetDateTime
{
public:
	IDateTime(CDateTime *tStamp) : stamp(tStamp) { }

	virtual void set(int year, int month, int day, int hour, int minute, int second);

private:
	CDateTime *stamp;
};

void IDateTime::set(int year, int month, int day, int hour, int minute, int second)
{
	stamp->year = year;
	stamp->month = month;
	stamp->day = day;
	stamp->hour = hour;
	stamp->minute = minute;
	stamp->second = second;
}

void CDateTime::SyncServer(SYNC_SERVER sync)
{
	static time_t synced = 0;
	static CDateTime ServerDateTime;
	static int count = 100;
	time_t current = time(NULL);
	int delta = current - synced;
	bool overflow = (ServerDateTime.second + delta) >= 60;

	if (count >= cache_count[sync] || current < synced || delta >= cache_delta[sync] ||
		(overflow && ServerDateTime.hour == 23 && ServerDateTime.minute == 59))
	{
		TCACDataBase::DefaultDB->GetNow(IDateTime(&ServerDateTime));

	#if TESTVER
		if (datim)
		{
			if (count >= cache_count[sync])
				fprintf(datim, "expired count %d ", count);
			else if (delta >= cache_delta[sync])
				fprintf(datim, "expired delta %d ", delta);
			else
				fputs("expired, other ", datim);

			fprintf(datim, "-> %lD %lT\n", &(CDate &) ServerDateTime, &(const CTime &) ServerDateTime);
		}
	#endif  // TESTVER

		_CurrentDateTime = ServerDateTime;
		synced = current;
		count = 0;
	}
	else
	{
		_CurrentDateTime = ServerDateTime;
		_CurrentDateTime.second += delta;
		count++;

		if (overflow)
		{
			_CurrentDateTime.second -= 60;
			(CTime &) _CurrentDateTime += 1;
		}

	#if TESTVER
		if (datim)
		{
			fprintf(datim, "%lD %lT + %d ", &(CDate &) ServerDateTime, &(const CTime &) ServerDateTime, delta);
			fprintf(datim, "-> %lD %lT\n", &(CDate &) _CurrentDateTime, &(const CTime &) _CurrentDateTime, delta);
		}
	#endif  // TESTVER
	}
}

void CDateTime::Clear()
{
	CDate::Clear();
	CTime::Clear();
}

bool CDateTime::Get(const pFLDDesc desc, const TQuery *query)
{
	return query->GetStamp(desc, IDateTime(this));
}

int CDateTime::Compare(const CDateTime &stamp) const
{
	int result = CDate::Compare(stamp);
	return result ? result : CTime::Compare(stamp);
}

bool CDateTime::Empty() const
{
	return CDate::Empty() && CTime::Empty();
}

void CDateTime::Print(mstr &m, const char *format) const
{
	if (!Empty())
	{
		if (format == C_DATE_STD || format == C_DATE_GOD)
			CDate::Print(m, format);
		else
			m.printf(format, day, month, year, hour, minute, second);
	}
}

void CDateTime::SetValue(long value)
{
	CDate::SetValue(value, hour, minute);
	second = 0;

	if (!CTime::Valid())
		fatal("CDateTime::SetValue %ld", value);
}

bool CDateTime::Valid() const
{
	return CDate::Valid() && CTime::Valid();
}

long CDateTime::Value() const
{
	if (second)
		fatal("CDateTime::Value contains second");

	return CDate::Value(hour, minute);
}

const CDateTime DATETIME_MIN(1, 1, YEAR_MIN, 0, 0, 0);
const CDateTime DATETIME_MAX(31, 12, YEAR_MAX, 23, 59, 59);
CDateTime _CurrentDateTime;
const CDate &Today = _CurrentDateTime.Date();
