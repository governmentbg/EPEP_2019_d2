#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef __BORLANDC__
#error __BORLANDC__ rand() only
#endif

#if __BORLANDC__ != 0x500
#error __BORLANDC__ 0x500 rand() only
#endif

typedef unsigned __u32;
typedef unsigned short __u16;
typedef unsigned char __u8;

static int i;
static char d;

static int digitize(char *s, __u32 u, int nd, int br)
{
	for (i = 0; i < nd; i++)
	{
		if (i == br) *s++ = '-';
		d = (char) (u % 33);
		*s++ = (char) ((d < 10) ? '0' + d : 'A' + d - 10);
		u /= 33;
	}

	return u == 0;
}

static __u8 b[16];
static __u16 m;
static __u8 *pm = (__u8 *) &m;
static double k;
static float c;
static __u8 *pk = (__u8 *) &k;
static __u8 *pc = (__u8 *) &c;
static int sm;
static int r, p;
static int un, uc, cn;
/* 2008:025 +related */
static int bk;
/* 2010:047 +related */
#define hf (*(__u16 *) (b + 14))
static int fl;
/* 2010:152 +related */
static int tvalue;

#define stm(year, month, day) ((year - 2010) * 372 + month * 31 + day)
#define assert1(cond, mask) if (!(cond)) e |= mask

#include <windows.h>

static void compile(char *sn)
{
	time_t timer = time(NULL);
	__u8 e = 0;

	srand(timer);
	do
	{
		r = rand() % 1001 + bk + 1000;
		for (i = 2; i * i <= r; i++) if (r % i == 0) break;
	} while (i * i <= r);

	p = (r - bk - 518) / (230 + bk / 1000);
	c = r * r;
	c *= r;

	k = un * 1000000 + uc * 10 + cn;
	for (i = 0; i < p; i++) k = sqrt(k);

	b[3] = pc[0];
	b[4] = pk[0];
	b[5] = pc[1];
	b[6] = pk[1];
	b[7] = pc[2];
	b[8] = pk[2];
	b[9] = pk[3];
	b[10] = pk[4];
	b[11] = pc[3];
	b[12] = pk[5];
	b[14] = pk[6];
	b[15] = pk[7];

	sm = 0;
	for (i = 0; i < 8; i++) sm += pk[i];
	for (i = 0; i < 4; i++) sm += pc[i];

	/* TODO: should not be loop */
	for (m = 17000; m < 18035; m++) if ((sm + m) % 0x330 == 0x020D) break;

	b[0x2] = pm[0];
	b[0xD] = pm[1];

	if (fl)
		hf = (__u16) ((hf - 0x3FED) * 105 + fl + 19);

	if (bk <= 2000)
	{
		struct tm *tblock = localtime(&timer);
		int tstore;

		assert1(b[0xB] >= 0x4F || b[0xB] <= 0x51, 0x01);
		assert1(pm[1] >= 0x42 && pm[1] <= 0x45, 0x02);
		b[0xD] -= (char) (((b[0xB] & 0x01) << 4) + ((b[0xB] & 0x02) << 2));

		tvalue = stm(tblock->tm_year + 1901, tblock->tm_mon, tblock->tm_mday) - stm(2010, 5, 1);
		assert1(tvalue <= 0xFFC, 0x04);
		srand(tvalue * 22 + 0xAD5);
		tstore = rand() % 0x2000 + rand() % 0x2000;
		b[0xB] = (char) (tstore & 0xFF);
		b[0xD] -= (char) ((tstore & 0x2000) >> 8);
		b[1] = (char) ((tstore >> 8) & 0x1F);
	}

	assert1(digitize(sn + 0, *(__u32 *) (b + 2), 7, 5), 0x10);
	assert1(digitize(sn + 8, *(__u32 *) (b + 6), 7, 3), 0x20);
	assert1(digitize(sn + 16, hf | (b[1] << 15), 4, 1), 0x40);
	assert1(digitize(sn + 21, *(__u32 *) (b + 10), 7, 2), 0x80);

	sn[29] = '\0';
	if (e) fprintf(stderr, "c.%d\n", e);
}

static __u32 dedigitize(char *s, int nd, int br)
{
	__u32 u = 0;

	s += nd;
	for (i = 0; i <= nd; i++)
	{
		if (i != nd - br)
		{
			d = (char) (isdigit(*s) ? *s - '0' : *s - 'A' + 10);
			u = u * 33 + d;
		}
		s--;
	}

	return u;
}

static void decompile(char *sn)
{
	char *s;
	float f;
	int n;
	__u32 u;
	__u8 e = 0;

	assert1(strlen(sn) == 25 + 4, 0x01);
	while (strlen(sn) < 25 + 4) strcat(sn, "?");
	sn[25 + 4] = '\0';

	for (s = sn; *s; s++)
	{
		if ((s - sn) % 6 == 5) { assert1(*s == '-', 0x02); }
		else
		{
			*s = (char) toupper(*s);
			assert1(isdigit(*s) || (*s >= 'A' && *s <= 'W'), 0x02);
		}
	}

	*(__u32 *) (b + 2) = dedigitize(sn + 0, 7, 5);
	*(__u32 *) (b + 6) = dedigitize(sn + 8, 7, 3);
	u = dedigitize(sn + 16, 4, 1);
	b[1] = (char) (u >> 15);
	hf = (__u16) (u & 0x7FFF);
	*(__u32 *) (b + 10) = dedigitize(sn + 21, 7, 2);

	b[0] = (char) (0x40 - (b[0xD] & 0xF8));
	if (bk <= 2000)
	{
		int tstore = ((b[0] & 0x20) << 8) | (b[1] << 8) | b[0xB];

		for (tvalue = 0; tvalue <= 0xFFC; tvalue++)
		{
			srand(tvalue * 22 + 0xAD5);
			if (rand() % 0x2000 + rand() % 0x2000 == tstore)
				break;
		}
		assert1(tvalue <= 0xFFC, 0xE1);
		b[0xB] = (char) ((0x50 | ((b[0] & 0x10) >> 4)) - ((b[0] & 0x08) >> 2));
		b[0xD] = (char) (0x40 | (b[0xD] & 0x07));
	}

	if (hf >= 0x3FED)
		fl = 0;
	else
	{
		fl = hf % 105 - 19;
		hf = (__u16) (hf / 105 + 0x3FED);
	}

	pm[0] = b[0x2];
	pm[1] = b[0xD];

	pc[0] = b[3];
	pk[0] = b[4];
	pc[1] = b[5];
	pk[1] = b[6];
	pc[2] = b[7];
	pk[2] = b[8];
	pk[3] = b[9];
	pk[4] = b[10];
	pc[3] = b[11];
	pk[5] = b[12];
	pk[6] = b[14];
	pk[7] = b[15];

	sm = 0;
	for (i = 0; i < 8; i++) sm += pk[i];
	for (i = 0; i < 4; i++) sm += pc[i];
	assert1((sm + m) % 0x330 == 0x020D, 0x04);

	for (r = bk + 1000; r <= bk + 2000; r++)
	{
		f = r * r;
		f *= r;
		if (f == c) break;
	}
	assert1(r <= bk + 2000, 0x08);

	for (i = 2; i * i <= r; i++) if (r % i == 0) break;
	assert1(i * i > r, 0x10);

	p = (r - bk - 518) / (230 + bk / 1000);
	for (i = 0; i < p; i++) k *= k;
	n = k;

	un = k / 1000000;
	uc = n / 10 % 100000;
	cn = n % 10;

	assert1(un > 0, 0x20);
	assert1(uc > 0, 0x40);
	assert1(cn > 0, 0x80);

	if (e) fprintf(stderr, "d.%d\n", e);
}

#define DC 3
#define RC 4
#define RF 5
#define _C 6

int main(int argc, char **argv)
{
	char sn[25 + 4 + 1];
	char _sn[25 + 4 + 1];
	int _un, _uc, _cn, _fl;

	bk = atoi(argv[argc - 1]);

	if (argc != _C)
	{
		decompile(argv[1]);
		printf("%d %d %d %d", un, uc, cn, fl);
		if (argc == DC)
		{
			tvalue += stm(2010, 5, 1);
			printf("%d %d %d\n", tvalue / 372, (tvalue % 372) / 31, tvalue % 31);
		}
		printf("\n");
	}

	if (argc != DC)
	{
		_sn[0] = '\0';

		if (argc == _C)
		{
			_un = atoi(argv[1]);
			_uc = atoi(argv[2]);
			_cn = atoi(argv[3]);
			_fl = atoi(argv[4]);
		}
		else
		{
			_un = un;
			_uc = uc;
			_cn = cn;
			if (argc == RC)
				_fl = fl;
			else
				_fl = atoi(argv[3]);
			bk = atoi(argv[2]);
		}

		do
		{
			un = _un;
			uc = _uc;
			cn = _cn;
			fl = _fl;
			compile(sn);
			decompile(sn);
			if (strcmp(sn, _sn)) putchar('.');
			strcpy(_sn, sn);
		} while (un != _un  || uc != _uc || cn != _cn || fl != _fl);
		printf("\r%s     \n", sn);
	}

	return 0;
}
