#include "database.h"

void memzero_s(char *p, size_t size)
{
	memset(p, '\0', size);
	// avoid memset optimize-out
	for (size_t i = 0; i < size; i++)
		if (p[i])
			fatal("memzero_s() failure");
}

void calc_simple_xor(unsigned char *password, size_t size)
{
	static unsigned char key[] = { 0xC4, 0x0A, 0xA5, 0x57, 0xB1, 0xD4, 0x1E, 0x9D, 0x51, 0xBF, 0xD2, 0x70, 0x04, 0x24,
		0xC6, 0x8B, 0xFE, 0x4D, 0x94, 0x79, 0x14, 0xA4, 0x1D, 0x24 };

	for (size_t i = 0; i < size - 1; i++)
		password[i] ^= key[i % sizeof key];

	password[size - 1] = '\0';
}
