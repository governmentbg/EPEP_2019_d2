#ifndef COURT_TYPE
#include <assert.h>
#endif  // COURT_TYPE
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "sn_cls.h"

// ----- sn_cls --------------------------------------------------------------
double sn_cls::get_full()
{
	double full;

	full = court;
	full *= 1024;
	full += users;
	full *= 1024;
	full += flags;
	full *= 4;
	full += 1;
	full *= 128;
	full += z_year;
	full *= 16;
	full += month;
	full *= 32;
	full += day;

	return full;
}

#ifndef COURT_TYPE
sn_cls::VK sn_cls::vk[SN_MAXKEYS] =
	{ 0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

const Byte *sn_cls::find_key(int version)
{
	int i;

	for (i = 0; i < SN_MAXKEYS - 1; i++)
		if (version == vk[i].version)
			break;

	assert(version == vk[i].version);
	return vk[i].key;
}

// ----- sn_enc --------------------------------------------------------------
void sn_enc::make_key(int version)
{
	const Byte *key = find_key(version);
	seed_key(key, version);
	Aes_SetKey_Enc(aes, key, 16);
}

void sn_enc::encode()
{
	unsigned u32 = court;

	u32 <<= 10;
	u32 += users;
	u32 <<= 10;
	u32 += flags;
	u32 <<= 2;
	u32 += 1;
	memcpy(input + 2, &u32, sizeof u32);

	unsigned short u16 = z_year;

	u16 <<= 4;
	u16 += month;
	u16 <<= 5;
	u16 += day;
	memcpy(input + 6, &u16, sizeof u16);

	double full = get_full();
	double *root = (double *) (input + 8);
	double test;

	*root = sqrt(full);
	assert(input[15] == 0x41);
	test = *root * *root;
	test = test - floor(test) >= 0.5 ? ceil(test) : floor(test);
	//assert(test == full);
	assert(!memcmp(&test, &full, sizeof test));

	my_srand(*seed);
	unsigned short start = my_rand();
	unsigned short *rnd = (unsigned short *) input;

	for (*rnd = start + 1; *rnd != start; (*rnd)++)
	{
		Aes_Encode(aes, (UInt32 *) block, (const UInt32 *) input);

		if (block[0] == block[15])
			break;
	}

	assert(*rnd != start);

	int xbit = 24 + my_rand() % 72;	//int xbit = 0;
	char *w = sn;
	int d = 0;

	for (int i = 0; i < 120; i++)
	{
		d <<= 1;
		d |= (block[xbit / 8] & (1 << (xbit % 8))) != 0;

		if (i % 5 == 4)
		{
			*w++ = (d < 10) ? '0' + d : 'A' + d + (d >= 14 || (my_rand() % 2) ? -10 : +12);
			d = 0;

			if (i % 30 == 29)
				*w++ = "-"[i == 119];
		}

		if (++xbit == 120)
			xbit = 0;
	}
}

// ----- sn_dec --------------------------------------------------------------
void sn_dec::verify(sn_enc &that, int version)
{
	make_key(version);
	strcpy(sn, that.sn);
	assert(decode() == 0);
	assert(court == that.court && users == that.users && flags == that.flags);
	assert(day == that.day && month == that.month && z_year == that.z_year);
}
#endif  // COURT_TYPE

void sn_dec::make_key(const Byte *key, int version)
{
	seed_key(key, version);
	Aes_SetKey_Dec(aes, key, 16);
}

bool sn_dec::precode()
{
	if (strlen(sn) != 27)
		return false;

	my_srand(*seed);
	my_rand();

	int xbit = 24 + my_rand() % 72;	//int xbit = 0;
	const char *r = sn;

	for (int i = 0; i < 27; i++, r++)
	{
		char c = *r;

		if (i % 7 < 6)
		{
			int d;

			if (c >= '0' && c <= '9')
				d = c - '0';
			else if (c >= 'A' && c <= 'Z')
				d = (c - 'A') % 22 + 10;
			else
				return false;

			for (int n = 4; n >= 0; n--)
			{
				if (d & (1 << n))
					block[xbit / 8] |= (Byte) (1 << (xbit % 8));
				else
					block[xbit / 8] &= (Byte) ~(1 << (xbit % 8));

				if (++xbit == 120)
					xbit = 0;
			}
		}
		else if (c != '-')
			return false;
	}

	return true;
}

int sn_dec::decode()
{
	if (!precode())
		return SNE_SYNTAX;

	block[15] = block[0];
	Aes_Decode(aes, (UInt32 *) input, (const UInt32 *) block);

	if (input[15] != 0x41)
		return SNE_BVALUE;

	unsigned u32;

	memcpy(&u32, input + 2, sizeof u32);
	u32 >>= 2;
	flags = u32 & 0x3FF;
	u32 >>= 10;
	users = u32 & 0x3FF;
	u32 >>= 10;
	court = u32;

	unsigned short u16;

	memcpy(&u16, input + 6, sizeof u16);
	day = u16 & 0x1F;
	u16 >>= 5;
	month = u16 & 0x0F;
	u16 >>= 4;
	z_year = u16;

	double root = sqrt(get_full());

	if (memcmp(&root, input + 8, sizeof root))
		return SNE_CVALUE;

	if (court < 1 || court > 999)
		return SNE_COURT;

	if (users < 1 || users > 551)
		return SNE_USERS;

	return 0;
}
