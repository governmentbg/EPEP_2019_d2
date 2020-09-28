#include "cac.h"

// tmp
const char *basename(const char *name)
{
	const char *base = NULL;

	for (const char *s = name; *s; s++)
		if (strchr(":/\\", *s))
			base = s;

	return base ? base + 1 : name;
}

char *TEMPDIR = ".\\TMP";
char *tempdir = NULL;

void obtain_tempdir()
{
	char *vars[] = { "TMP", "TEMP", "USERPROFILE", NULL };
	char *dir;
	static char buffer[201];

	tempdir = TEMPDIR;
	for (int i = 0; vars[i]; i++)
	{
		if ((dir = getenv(vars[i])) != NULL && exist(dir, false))
		{
			DWORD result = GetShortPathName(dir, buffer, sizeof buffer);

			if (result > 0 && result < sizeof buffer && !strchr(buffer, ' '))
			{
				tempdir = buffer;
				break;
			}
		}
	}
}

// misc
extern "C" int __NTerror(void);

bool exist(const char *path, bool file)
{
	struct stat statbuf;

	if (stat(path, &statbuf))
	{
		// LPR: stat() fails on network volumes/resources, try again
		DWORD dwAttributes;

		if ((dwAttributes = GetFileAttributes(path)) == 0xFFFFFFFF)
		{
			__NTerror();
			return false;
		}

		if (!(dwAttributes && FILE_ATTRIBUTE_DIRECTORY) != file)
		{
			errno = file ? EISDIR : ENOTDIR;
			return false;
		}

		return true;
	}

	if (!S_ISDIR(statbuf.st_mode) != file)
	{
		errno = file ? EISDIR : ENOTDIR;
		return false;
	}

	return true;
}

// utility
void calc_simple_xor(unsigned char *password, size_t size)
{
	static unsigned char key[] = { 0xC4, 0x0A, 0xA5, 0x57, 0xB1, 0xD4, 0x1E, 0x9D, 0x51, 0xBF, 0xD2, 0x70, 0x04, 0x24,
		0xC6, 0x8B, 0xFE, 0x4D, 0x94, 0x79, 0x14, 0xA4, 0x1D, 0x24 };

	for (int i = 0; i < size - 1; i++)
		password[i] ^= key[i % sizeof key];

	password[size - 1] = '\0';
}

void memzero_s(char *p, size_t size)
{
	memset(p, '\0', size);
	// avoid memset optimize-out
	for (int i = 0; i < size; i++)
		if (p[i])
			fatal("memzero_s() failure");
}
