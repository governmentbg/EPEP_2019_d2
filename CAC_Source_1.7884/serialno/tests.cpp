#include <assert.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>

#include "sn_cls.h"
#include "tests.h"

void test_root()
{
	for (int court = 10; court < 1023; court++)
	{
		double full = court, root, test;
		byte *bytes = (byte *) &root;

		full *= 1024;	// users
		full += 550;
		full *= 1024;	// flags
		full += 1023;
		full *= 4;		// reserved
		full += 3;
		full *= 128;	// year
		full += 67;
		full *= 16;		// month
		full += 12;
		full *= 32;		// day
		full += 31;

		root = sqrt(full);
		test = root * root;
		test = test - floor(test) >= 0.5 ? ceil(test) : floor(test);
		assert(test == full);

		for (int byte = 0; byte < sizeof root; byte++)
			printf("%02X%c", bytes[byte], " \n"[byte == sizeof root - 1]);
	}
}

void test_aes128()
{
	UInt32 aes[AES_NUM_IVMRK_WORDS];
	Byte key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	Byte input[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	Byte block[16];
	Byte check[16] = { 0x39, 0x25, 0x84, 0x1D, 0x02, 0xDC, 0x09, 0xFB, 0xDC, 0x11, 0x85, 0x97, 0x19, 0x6A, 0x0B, 0x32 };
	Byte output[16];

	Aes_SetKey_Enc(aes, key, 16);
	Aes_Encode(aes, (UInt32 *) block, (const UInt32 *) input);
	assert(!memcmp(block, check, 16));

	Aes_SetKey_Dec(aes, key, 16);
	Aes_Decode(aes, (UInt32 *) output, (const UInt32 *) block);
	assert(!memcmp(output, input, 16));
}

class sn_enc_test : public sn_enc
{
public:
	sn_enc_test() { make_key(vk[0].version); }

	void output_block() { fwrite(block, sizeof block, 1, stdout); }
};

class sn_dec_test : public sn_dec
{
public:
	void verify(sn_enc &that) { sn_dec::verify(that, vk[0].version); }
};

void test_batch(bool binary)
{
	sn_enc_test sne;
	srand(GetTickCount());

	for (int i = 0; i < 100000; i++)
	{
		sne.court = 1 + rand() % 999;
		sne.users = 1 + rand() % 550;
		sne.flags = rand() % 1024;

		sne.day = 1 + rand() % 31;
		sne.month = 1 + rand() % 12;
		sne.z_year = rand() % 68;

		sne.encode();
		sn_dec_test().verify(sne);

		if (binary)
		{
			int output = fileno(stdout);

			assert(!isatty(output));
			setmode(output, O_BINARY);
			sne.output_block();
		}
		else
			puts(sne.sn);
	}
}
