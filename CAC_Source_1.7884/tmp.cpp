#include "cac.h"

// ----- TUnlinkFile ---------------------------------------------------------
struct TUnlinkFile
{
	char base[SIZE_OF_TWICE_BASE];
	bool wordFile;
	bool unreadOnly;

	bool operator<=(const TUnlinkFile &that);
};

bool TUnlinkFile::operator<=(const TUnlinkFile &that)
{
	return strcmpi(base, that.base) <= 0;
}

static TISArray<TUnlinkFile> *unlinks = NULL;

// ----- functions -----------------------------------------------------------
constant TEMPDIR = ".\\TMP";
const char *tempdir = NULL;

void initialize_tmp()
{
	char *vars[] = { "TMP", "TEMP", "USERPROFILE", NULL };
	char *dir;
	static char buffer[SIZE_OF_DIR];

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

	unlinks = new TISArray<TUnlinkFile>;
}

void add_unlink_file(const char *base, bool wordFile, bool unreadOnly)
{
#if RANGECHECK
	if (base != basename(base))
		fatal("%s: adding non-base name to unlink", base);
#endif  // RANGECHECK

	TUnlinkFile *unlf = new TUnlinkFile;

	strcpy(unlf->base, base);
	unlf->wordFile = wordFile;
	unlf->unreadOnly = unreadOnly;

	if (unlinks->Find(unlf) == INT_MAX)
		unlinks->Add(unlf);
	else
		delete unlf;
}

static void action_unlink_file(const char *name, const void *) { unlink(name); }

void shutdown_tmp()
{
	for (int i = 0; i < unlinks->Count(); i++)
	{
		TUnlinkFile *unlf = (*unlinks)[i];
		char target[SIZE_OF_PATH];

		build_fn(tempdir, unlf->base, target);

		if (unlf->unreadOnly)
			SetFileAttributes(target, FILE_ATTRIBUTE_NORMAL);

		if (!unlink(target) && unlf->wordFile)
			html_group_action(target, action_unlink_file, NULL);
	}

	delete unlinks;
}
