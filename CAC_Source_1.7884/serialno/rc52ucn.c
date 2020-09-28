#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "rc51.c"

int main(int argc, char **argv)
{
	static unsigned char rc5_key[16] = { 0x86, 0xFC, 0x4D, 0x3B, 0x79, 0x9A, 0x7E, 0xB5, 0xE5, 0x6D, 0xB0, 0xAA, 0x95,
		0x44, 0x72, 0xE0 };
	symmetric_key sym_key;
	int i;

	assert(argc >= 2);
	assert(rc5_setup(rc5_key, sizeof rc5_key, 19, &sym_key) == CRYPT_OK);

	for (i = 1; i < argc; i++)
	{
		const char *arg = argv[i];
		const char *s;
		unsigned long output[2];
		unsigned long input[2];

		assert(strlen(arg) == 16);
		for (s = arg; *s; s++)
			assert(isxdigit(*s));

		sscanf(arg, "%8lx%8lx", output, output + 1);
		rc5_ecb_decrypt((const unsigned char *) output, (unsigned char *) input, &sym_key);
		printf("%ld", input[1]);

		if (input[1] < 1000000000)
		{
			char value[10];
			int n, sum = 0;
			static char multi[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };

			sprintf(value, "%ld", input[1]);
			for (n = 0; n < 9; n++)
				sum += multi[n] * (value[n] - '0');

			sum = sum % 11 % 10;
			printf("%d", sum);
		}

		putchar('\n');
	}

	return 0;
}
