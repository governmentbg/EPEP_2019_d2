#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sn_cls.h"
#include "tests.h"

// testsn: test <root | aes128 | batch | block>
// encode: version users court flags [day month year]
// decode: version serialno

int main(int argc, char **argv)
{
	int error;

	assert(argc == 8 || argc == 5 || argc == 3);
	sn_cls::init();

	if (argc >= 5)
	{
		sn_enc sne;

		sne.users = atoi(argv[2]);
		sne.court = atoi(argv[3]);
		sne.flags = atoi(argv[4]);
		assert(sne.court >= 1 && sne.users >= 1 && sne.flags >= 0);
		assert(sne.court <= 999 && sne.users <= 550 && sne.flags <= 1023);
		sne.users += 1;	// for server/admin

		if (argc == 5)
		{
			time_t timer = time(NULL);
			struct tm *tblock = localtime(&timer);

			sne.day = tblock->tm_mday;
			sne.month = tblock->tm_mon + 1;
			sne.z_year = tblock->tm_year - 70;
		}
		else
		{
			sne.day = atoi(argv[5]);
			sne.month = atoi(argv[6]);
			sne.z_year = atoi(argv[7]) - 1970;
			assert(sne.day >= 1 && sne.month >= 1 && sne.z_year >= 0);
			assert(sne.day <= 31 && sne.month <= 12 && sne.z_year <= 67);
		}

		sne.make_key(atoi(argv[1]));
		sne.encode();
		sn_dec().verify(sne, atoi(argv[1]));
		puts(sne.sn);
	}
	else if (!strcmp(argv[1], "test"))
	{
		if (!strcmp(argv[2], "root"))
			test_root();
		else if (!strcmp(argv[2], "aes128"))
			test_aes128();
		else
		{
			assert(!strcmp(argv[2], "batch") || !strcmp(argv[2], "block"));
			test_batch(argv[2][1] == 'l');
		}
	}
	else
	{
		sn_dec snd;

		assert(strlen(argv[2]) == 27);
		strcpy(snd.sn, argv[2]);
		snd.make_key(atoi(argv[1]));
		error = snd.decode();

		if (error)
			fprintf(stderr, "error %d\n", error);
		else
		{
			printf("%3d (+1)  %d  %-2d  %2d %2d %d\n", snd.users - 1, snd.court, snd.flags, snd.day, snd.month,
				snd.z_year + 1970);
		}
	}

	return 0;
}
