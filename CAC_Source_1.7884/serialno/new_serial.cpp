#if DEBUG
#include <assert.h>
#else  // DEBUG
#define assert(h)
#endif  // DEBUG
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "myrand.h"

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

static int VERSION;
static int DELTA;
static int RANGE1;
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

class sne : public sn
{
public:
	bool encode();

private:
	static void encode(char *s, unsigned u, int nd, int br);
};

bool sne::encode()
{
	my_srand(((day << 27) + (month << 23) + (year << 16) + (users << 10) + (VERSION % 1027)) % 1000);
	reserved = my_rand() % 517;

	for (int i = 0; i < 1000; i++)
	{
		// pass F
		cval1 = RANGE1 + i + DELTA;
		my_srand(cval1 * cval1 + day);
		cval2 = RANGE2 + my_rand() % 2000;

		double x1 = ((((cval1 / 524231.0) + users) / 1027.0 + flags) / 32771.0 + day) / 33.0;
		double x2 = (((((cval2 / 524231.0) + reserved) / 517.0 + court) / 1027.0 + year) / 129.0 + month) / 15.0;

		*d1 = VALUE / pow(x1, 1.0 / POWER);
		*d2 = VALUE / pow(x2, 1.0 / POWER) + cval2 / 543201.0;

		assert((b[7] == 0x3F && (b[6] & 0xF0) == 0xF0) || (b[7] == 0x40 && (b[6] & 0xF0) == 0x00));
		assert((b[0x0F] == 0x3F && (b[0xE] & 0xF0) == 0xF0) || (b[0x0F] == 0x40 && (b[0xE] & 0xF0) == 0x00));

		// pass B
		encode(s + 0, (b[0] << 16) + (b[3] << 8) + ((b[6] & 0x0F) << 4) + (b[0xE] & 0x0F), 5, -1);
		encode(s + 5, (b[0xA] << 16) + (b[0xD] << 8) + b[2], 5, 1);
		encode(s + 11, (b[5] << 16) + (b[9] << 8) + b[0xC], 5, 2);
		encode(s + 17, (b[1] << 16) + (b[4] << 8) + b[8], 5, 3);
		encode(s + 23, (b[0xB] << 2) + ((b[7] & 0x01) << 1) + (b[0x0F] & 0x01), 2, -1);

		{
			int n = 0;

			for (int i = 0; i < 25; i++)
				n += s[i];

			n %= 33;
			s[25] = (char) ((n < 10) ? '0' + n : 'A' + n - 10);
		}
		strcpy(s + 26, "0");	// free bits, highest 5-digit bits are free too

		// check
		snd snd;

		strcpy(snd.s, s);

		if (snd.decode() == 0)
		{
			assert(!memcmp(snd.b, b, sizeof b));
			assert(snd.day == day && snd.month == month && snd.year == year);
			assert(snd.court == court && snd.flags == flags && snd.reserved == reserved);
			return true;
		}
	}

	return false;
}

void sne::encode(char *s, unsigned u, int nd, int br)
{
	for (int i = 0; i < nd; i++)
	{
		if (i == br) *s++ = '-';
		char d = (char) (u % 33);
		*s++ = (char) ((d < 10) ? '0' + d : 'A' + d - 10);
		u /= 33;
	}

	assert(!u);
}

int main(int argc, char **argv)
{
	int e;

	assert(argc == 4 || argc == 6);
	VERSION = atoi(argv[1]);
	RANGE1 = atoi(argv[2]);
	DELTA = ((VERSION * VERSION) % 64 - 32);

	if (argc == 4)
	{
		snd sn;

		strcpy(sn.s, argv[3]);
		e = sn.decode();

		if (e)
			printf("%d", e);
		else
			printf("%d %d %d %d %d %d", sn.users, sn.court, sn.flags, sn.day, sn.month, sn.year);
	}
	else
	{
		sne sn;

		sn.users = atoi(argv[3]) + 1;
		sn.court = atoi(argv[4]);
		sn.flags = atoi(argv[5]) + ((GetTickCount() & 0x1F) << 10);

		if (argc == 6)
		{
			time_t timer = time(NULL);
			struct tm *tblock = localtime(&timer);

			sn.day = tblock->tm_mday;
			sn.month = tblock->tm_mon + 1;
			sn.year = tblock->tm_year - 70;
		}
		else
		{
			assert(argc == 9);

			sn.day = atoi(argv[6]);
			sn.month = atoi(argv[7]);
			sn.year = atoi(argv[8]);
		}

		e = !sn.encode();
		puts(e ? "FE" : sn.s);
	}

	return e;
}
