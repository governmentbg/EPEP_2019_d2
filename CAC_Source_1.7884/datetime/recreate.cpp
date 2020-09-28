#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "longdate.h"
#include "myrand.h"

static unsigned short bases[BASE_COUNT] =
{
#include "hbase66k.cpp"
};

static TDateData data[BASE_COUNT];

int main()
{
	int i, n;
	long value, vcalc;
	FILE *check;

	value = LONG_START - (BASE_PERIOD * RAND_RANGE) / 2;
	for (i = 0; i < BASE_COUNT; i++)
	{
		unsigned short input;

		data[i].base = value += bases[i] + BASE_PERIOD * MIN_DELTA;
		my_srand(value);

		vcalc = value;
		for (n = 0; n < CALC_COUNT; n++)
		{
			my_rand();
			while ((input = my_rand()) >= (BASE_PERIOD * RAND_RANGE) / 2);
			data[i].calc[n] = vcalc += (input % RAND_RANGE) + MIN_DELTA;
			my_rand();
		}
	}

	assert((check = fopen("RECR.DAT", "wb")) != NULL);
	for (i = 0; i < BASE_COUNT; i++)
	{
		assert(fwrite(&data[i].base, sizeof(unsigned long), 1, check) == 1);
		for (n = 0; n < CALC_COUNT; n++)
			assert(fwrite(&data[i].calc[n], sizeof(unsigned long), 1, check) == 1);
	}
	assert(fclose(check) == 0);

	return 0;
}
