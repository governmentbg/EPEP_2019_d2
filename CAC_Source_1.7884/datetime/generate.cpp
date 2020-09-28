#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "longdate.h"
#include "myrand.h"

static TDateData data[BASE_COUNT];

/* we want this program clear, not optimal */

int main()
{
	FILE *input64k, *full, *bases, *hbase64k;
	int i, n;
	long value, vbase;

	assert((input64k = fopen("INPUT66K.DAT", "rb")) != NULL);
	value = LONG_START;
	for (i = 0; i < BASE_COUNT; i++)
	{
		unsigned short input;

		do
		{
			assert(fread(&input, sizeof input, 1, input64k) == 1);
		} while (input >= BASE_PERIOD * RAND_RANGE);

		data[i].base = value += (input % RAND_RANGE) + MIN_DELTA;
		my_srand(data[i].base);

		for (n = 0; n < CALC_COUNT; n++)
		{
			my_rand();
			while ((input = my_rand()) >= (BASE_PERIOD * RAND_RANGE) / 2);
			data[i].calc[n] = value += (input % RAND_RANGE) + MIN_DELTA;
			my_rand();
		}
	}
	assert(fclose(input64k) == 0);

	assert((full = fopen("FULL.DAT", "wb")) != NULL);
	for (i = 0; i < BASE_COUNT; i++)
	{
		assert(fwrite(&data[i].base, sizeof(long), 1, full) == 1);
		for (n = 0; n < CALC_COUNT; n++)
			assert(fwrite(&data[i].calc[n], sizeof(long), 1, full) == 1);
	}
	assert(fclose(full) == 0);

	assert((bases = fopen("BASES.DAT", "wb")) != NULL);
	assert((hbase64k = fopen("hbase66k.cpp", "w")) != NULL);
	value = LONG_START - (BASE_PERIOD * RAND_RANGE) / 2;
	for (i = 0; i < BASE_COUNT; i++)
	{
		unsigned short h;

		vbase = data[i].base - BASE_PERIOD * MIN_DELTA - value;
		assert(vbase <= 0xFFFF);

		if ((i % 0x10) == 0)
			fputc('\t', hbase64k);
		fprintf(hbase64k, "0x%04X", vbase);
		if (i < BASE_COUNT - 1)
			fputc(',', hbase64k);
		fputc((i % 0x10) < 0x0F ? ' ' : '\n', hbase64k);

		h = (unsigned short) vbase;
		assert(fwrite(&h, sizeof h, 1, bases) == 1);
		value = data[i].base;
	}
	assert(fclose(hbase64k) == 0);
	assert(fclose(bases) == 0);

	return 0;
}
