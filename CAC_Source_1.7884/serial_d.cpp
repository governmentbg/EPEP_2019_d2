static bool is_prime(int value)
{
	for (int i = 2; i * i <= value; i++)
		if (value % i == 0)
			return false;

	return true;
}

class sn
{
public:
	sn();

	char s[27 + 1];
	int day, flags, users, cval1;
	int month, year, court, reserved, cval2;		// 7-bit year

protected:
	unsigned char b[16];

	double *d1;
	double *d2;
};

sn::sn()
	: d1((double *) b), d2 ((double *) (b + 8))
{
}

#define RANGE2 ((RANGE1 - 19000) * 2 + 56000)
#define VALUE 1
#define POWER (3.1415926535897932384 * 2)

class snd : public sn
{
public:
	int decode();

private:
	static unsigned decode(char *s, int nd, int br);
};

#define E(cond, flag) if (!(cond)) e |= flag

enum
{
	SNE_FORMAT	= 0x001,
	SNE_DAY	= 0x002,
	SNE_MONTH	= 0x004,
	SNE_YEAR	= 0x008,
	SNE_VDATE	= 0x010,
	SNE_CKSUM	= 0x020,
	SNE_RANGE	= 0x040,
	SNE_PRIME	= 0x080,
	SNE_CVAL2	= 0x100,
	SNE_RRND	= 0x200,
	SNE_BITS	= 0x400,
	SNE_CTYPE	= 0x800
};

int snd_check(const char *s)
{
	int n = 0;

	if (strlen(s) != 27 || s[26] != '0')
		return SNE_FORMAT;

	for (int i = 0; i < 25; s++, i++)
	{
		if ((i % 7 == 6) ? *s != '-' : !isupper(*s) && !isdigit(*s))
			return SNE_FORMAT;

		n += *s;
	}

	n %= 33;

	return ((n < 10) ? '0' + n : 'A' + n - 10) != *s ? SNE_CKSUM : 0;
}

int snd::decode()
{
	int e = snd_check(s);

	if (e)
		return e;

	// pass B
	unsigned u;

	u = decode(s + 0, 5, -1);
	b[0] = (unsigned char) (u >> 16);
	b[3] = (unsigned char) (u >> 8);
	b[6] = (unsigned char) ((u >> 4) & 0x0F);
	b[0xE] = (unsigned char) (u & 0x0F);
	u = decode(s + 5, 5, 1);
	b[0xA] = (unsigned char) (u >> 16);
	b[0xD] = (unsigned char) (u >> 8);
	b[2] = (unsigned char) u;
	u = decode(s + 11, 5, 2);
	b[5] = (unsigned char) (u >> 16);
	b[9] = (unsigned char) (u >> 8);
	b[0xC] = (unsigned char) u;
	u = decode(s + 17, 5, 3);
	b[1] = (unsigned char) (u >> 16);
	b[4] = (unsigned char) (u >> 8);
	b[8] = (unsigned char) u;
	u = decode(s + 23, 2, -1);
	b[0xB] = (unsigned char) (u >> 2);

	E((b[1] | b[2] | b[4]) & 0x01, SNE_BITS);

	if (u & 0x2)
	{
		b[6] |= 0xF0;
		b[7] = 0x3F;
	}
	else
		b[7] = 0x40;

	if (u & 0x01)
	{
		b[0xE] |= 0xF0;
		b[0xF] = 0x3F;
	}
	else
		b[0x0F] = 0x40;

	// pass F
	double x1 = VALUE / pow(*d1, POWER);

	day = (int) (x1 *= 33.0);
	E(day >= 1 && day <= 31, SNE_DAY);
	x1 -= day;
	flags = (int) (x1 *= 32771.0);
	x1 -= flags;
	users = (int) (x1 *= 1027.0);
	x1 -= users;
	cval1 = (int) (x1 *= 524231.0);
	E(cval1 >= RANGE1 && cval1 < RANGE1 + 1000, SNE_RANGE);

	my_srand(cval1 * cval1 + day);
	cval2 = RANGE2 + my_rand() % 2000;
	double x2 = VALUE / pow(*d2 - cval2 / 543201.0, POWER);
	cval1 -= DELTA;
	E(is_prime(cval1), SNE_PRIME);

	month = (int) (x2 *= 15.0);
	E(month >= 1 && month <= 12, SNE_MONTH);
	x2 -= month;
	year = (int) (x2 *= 129.0);
	E(year >= 42 && year <= 67, SNE_YEAR);
	x2 -= year;
	court = (int) (x2 *= 1027.0);
	x2 -= court;
	reserved = (int) (x2 *= 517.0);
	x2 -= reserved;
	E(cval2 == (int) (x2 *= 524231.0), SNE_CVAL2);

	my_srand(((day << 27) + (month << 23) + (year << 16) + (users << 10) + (VERSION % 1027)) % 1000);
	E(reserved != my_rand(), SNE_RRND);

	return e;
}

unsigned snd::decode(char *s, int nd, int br)
{
	unsigned u = 0;

	s += nd + (br != -1);
	for (int i = 0; i < nd; i++)
	{
		s--;
		if (i == nd - br) s--;
		char d = (char) (isdigit(*s) ? *s - '0' : *s - 'A' + 10);
		u = u * 33 + d;
	}

	return u;
}
