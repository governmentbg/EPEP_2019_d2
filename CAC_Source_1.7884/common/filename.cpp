#include "common.h"

const char *basename(const char *name)
{
	const char *base = NULL;

	for (const char *s = name; *s; s++)
		if (strchr(":/\\", *s))
			base = s;

	return base ? base + 1 : name;
}

char *build_fn(const char *dir, const char *name, char *path)
{
	if (path != dir)
		strcpy(path, dir);

	if (*dir && !strchr(":/\\", dir[strlen(dir) - 1]))
		strcat(path, "\\");

	return strcat(path, name);
}

const char *file_ext(const char *name)
{
	const char *base = basename(name);
	const char *ext = strrchr(base, '.');

	return ext && ext > name ? ext + 1 : base + strlen(base);
}

bool matches_exts(const char *name, const char *ext, ...)
{
	const char *tExt = file_ext(name);

	if (any(tExt))
	{
		va_list ap;
		va_start(ap, ext);

		do
		{
			if (!strcmpi(tExt, ext))
			{
				va_end(ap);
				return true;
			}
		} while ((ext = va_arg(ap, const char *)) != NULL);

		va_end(ap);
	}

	return false;
}
