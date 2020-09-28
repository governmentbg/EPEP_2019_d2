#include "cac.h"

// ----- offield -------------------------------------------------------------
const offield null_fields[] =
{
	{ NULL, NULL }
};

void TOffice::offstr::reset()
{
	clear();
	wild = false;
	cache = 0;
}

// ----- TOffice -------------------------------------------------------------
TOffice::TOffice()
	: TObject(full), buffer(NULL), wordFile(false), stub("")
{
}

TOffice::TOfficeGroup::TOfficeGroup()
	: autogen(1)
{
}

TOffice::~TOffice()
{
	delete[] buffer;
}

struct TOfficeData
{
	char source_base_noext[SIZE_OF_BASE_NAME];
	size_t source_len;
	char header_basename[SIZE_OF_TWICE_BASE];		// "?" for none
	bool copy_header;
	char filelist_basename[SIZE_OF_PATH];
	char target_dir_[SIZE_OF_PATH];			// not SIZE_OF_DIR
	char target_base_noext[SIZE_OF_TWICE_BASE];
	size_t target_len;
};

static char *office_read_file(const char *name)
{
	TUserFile uf("шаблонен");
	long size;
	char *buffer = uf.ReadAll(name, size);

	if (buffer && size != strlen(buffer))
		fatal("%s: съдържа двоични нули.", name);

	return buffer;
}

static char *office_replace_base(const char *const buffer, const TOfficeData *d)
{
	char *const start = new char[strlen(buffer) + 8192];

	strcpy(start, *buffer ? buffer : " ");
	delete[] buffer;

	for (char *s = start + 1; *s; s++)
	{
		bool sep_equal = s[-1] == '=';
		bool replace = false;
		char *quote;  // or great

		if ((sep_equal || s[-1] == '\"') && !strncmpi(s, d->source_base_noext, d->source_len))
		{
			char *post_base = s + d->source_len;

			quote = strchr(post_base, sep_equal ? '>' : '"');

			if (quote && quote - s < 0x80)
			{
				const char *dot;
				
				for (dot = post_base; dot < quote; dot++)
					if (*dot == '.')
						break;

				if (quote - dot >= 4 && quote - dot <= 5)  // >= 2 or 3 ?
				{
					if (dot == post_base)
					{
						if (quote - dot == 4 && !strncmpi(dot, ".htm", 4))
							replace = true;
					}
					else if (*post_base == '_')
					{
						const char *p;

						for (p = post_base; p < quote; p++)
							if (!isalnum(*p) && !ispunct(*p) && *p != '_')
								break;

						if (p == quote)
							replace = true;
					}
				}
			}
		}

		if (replace)
		{
			strmove(s + d->target_len, s + d->source_len);
			memcpy(s, d->target_base_noext, d->target_len);
			s = quote + d->target_len - d->source_len;
		}
	}

	return start;
}

static void office_copy_file(const char *source, const void *data)
{
	const TOfficeData *d = (const TOfficeData *) data;
	const char *source_base = basename(source);
	bool header = !strcmpi(source_base, d->header_basename);
	mstr target(d->target_dir_);

	target.cat(d->target_base_noext);
	target.cat(source_base + d->source_len);

	if (header || !strcmpi(source_base, d->filelist_basename))
	{
		if (!header || d->copy_header)
		{
			char *buffer = office_read_file(source);

			if (buffer)
			{
				TUserFile uf("шаблонен");

				buffer = office_replace_base(buffer, d);
				uf.WriteAll(str(target), buffer);
				delete[] buffer;
			}
		}
	}
	else if (!CopyFile(source, str(target), FALSE))
		error("%s: грешка %lu при копиране в %s.", source, GETLASTERROR, str(target));
}

bool TOffice::ValidBaseName(const char *base, size_t maxlen)
{
	if (strlen(base) > maxlen)
		return error("%s: името трябва да бъде до %u символа.", base, maxlen);

	if (strchr(base, ' '))
		return error("%s: името не може да съдържа интервали.", base);

	if (*base == '.')
		return error("%s: името не може да започва с точка.", base);

	return true;
}

bool TOffice::ValidTargetName(const char *name)
{
	if (strlen(name) > LENGTH_OF_OFFICE_TARGET)
		return error("%s: името трябва да бъде до %d символа.", name, LENGTH_OF_OFFICE_TARGET);

	const char *base = basename(name);

	if (!any(base) || base == name)
		return error("%s: необходими са директория и име на файл.", name);

	if (!html_name(base))
		return error("%s: разширението трябва да бъде htm или html.", base);

	return ValidBaseName(base, LENGTH_OF_TWICE_BASE);
}

bool TOffice::Open(const char *tName, TOfficeCopy copy, const char *target)
{
	TOfficeData d;
	const char *tBase = basename(tName);

	if (tBase != tName || strcmpi(file_ext(tBase), "htm"))
		fatal("%s: изисква се базово име с разширение htm.", tName);

	if (!ValidBaseName(tBase, 14))
		failure();

	// fillout source names + mode
	strcpy(d.source_base_noext, tName);
	*strrchr(d.source_base_noext, '.') = '\0';
	d.source_len = strlen(d.source_base_noext);
	name = tName;

	strcpy(d.header_basename, d.source_base_noext);
	strcat(d.header_basename, "_header.htm");
	d.copy_header = copy != OFFICE_SKIP_HEADER;

	if (copy == OFFICE_COPY_HEADER)
		name = d.header_basename;

	strcpy(d.filelist_basename, d.source_base_noext);
	strcat(d.filelist_basename, "_filelist.xml");

	// read template
	char input[SIZE_OF_PATH];
	build_fn(HTML_OVER_PATH, name, input);

	if (!any(HTML_OVER_PATH) || !exist(input))
		build_fn("htm", name, input);

	delete[] buffer;
	buffer = office_read_file(input);

	if (!buffer)
		return false;

	if (copy == OFFICE_COPY_NULL)
	{
		mainStart = buffer;
		mainFinal = buffer + strlen(buffer);
		return true;
	}

	const char *wordDocument = strstr(buffer, "content=Word.Document");
	wordFile = wordDocument && strstr(wordDocument, "<body");

	// compute target names + open output
	if (!target)
	{
		strcpy(full, h.OpenOffice(copy == OFFICE_COPY_HEADER));
		strcpy(target_base, basename(full));

		if (copy == OFFICE_COPY_HEADER)
			strcpy(strrchr(target_base, '_'), ".htm");
	}
	else
	{
		if (*target)
			strcpy(target_base, basename(target));

		if (basename(target) == target)
		{
			build_fn(tempdir, target_base, full);
		#if COURT_TYPE == COURT_ADMIN
			if (copy == OFFICE_COPY_ALL)
				copy = OFFICE_COPY_NULL;
		#endif  // ADMIN
		}
		else if (target != full)
			strcpy(full, target);

		if (copy == OFFICE_COPY_HEADER)
			strcpy(strrchr(full, '.'), "_header.htm");

		// 2008:078 LPR: descriptive EACCES message and retry ability
		// TODO? move to userfile
		while (!h.OpenRetry(full))
		{
			mstr m;
			m.printf("%s: грешка %d при отваряне на изходен файл", full, errno);

			if (errno == EACCES)
			{
				if (ask("%s.\n\nАко %s е отворен в Word го затворете или запишете под друго име.\n"
					"\nЖелаете ли да опитате отново?", str(m), full))
				{
					continue;
				}
			}
			else
				error("%s.", str(m));

			return false;
		}
	}

	strcpy(d.target_dir_, full);
	*basename(d.target_dir_) = '\0';
	strcpy(d.target_base_noext, target_base);
	*strrchr(d.target_base_noext, '.') = '\0';
	d.target_len = strlen(d.target_base_noext);

	// fixup, copy extras and mark for unlink
	if (wordFile)
	{
		buffer = office_replace_base(buffer, &d);

		if (copy != OFFICE_COPY_HEADER && copy != OFFICE_COPY_NULL)
			html_group_action(input, office_copy_file, &d);
	}

	mainStart = buffer;
	mainFinal = buffer + strlen(buffer);
	int target_dir_len = strlen(d.target_dir_) - 1;

	if (copy != OFFICE_COPY_HEADER && !strncmpi(d.target_dir_, tempdir, target_dir_len) &&
		strchr("/\\", tempdir[target_dir_len]))
	{
		add_unlink_file(target_base, wordFile, false);
	}

	return true;
}

const char *TOffice::Find(const char *tMark)
{
	char mark[SIZE_OF_NAME];
	char *s;

	sprintf(mark, "<!-- %s -->", tMark);
	int len = strlen(mark);
	if ((s = strstr(buffer, mark)) == NULL)
		fatal("%s: липсващ маркер %s.", name, tMark);
	if (s[len] == '\r' && s[len + 1] == '\n')
		len += 2;
	strmove(s, s + len);
	mainFinal -= len;

	return s;
}

#define offgroup OFFICE_GROUP_CAST(TOffice::TOfficeGroup)

static void offgroup_autogen(mstr &m, TGroup *group) { m.printf("%ld", offgroup->GetIncrAutoGen()); }

#undef offgroup

static offield global_fields[] =
{
	{ "F_AUTOGEN",		offgroup_autogen },
	{ "F_GOD",			generic_god_suffix },
	{ "F_COURT_LOGO",		generic_court_logo },
	{ "F_SERVICE_PHONE",	generic_service_phone },
	{ "F_SERVICE_FAX",	generic_service_fax },
	{ "F_TODAY",		generic_today },
	{ "F_COURT_ADDRESS_1",	generic_court_address_1 },
	{ "F_COURT_ADDRESS_2",	generic_court_address_2 },
	{ "F_COURT_ADDRESS",	generic_court_address },
	{ "F_COURT",		generic_court_name },
	{ "F_COURT_TYPE",		generic_court_type },
	{ "F_USER_NAME",		generic_user_name },
	{ "F_USER_POST",		generic_user_post },
	{ "F_COURT_UCLP",		generic_court_uclp },
	{ NULL, NULL }
};

void TOffice::Emit(const char *start, const char *final, const offield *const fields, TGroup *group)
{
	if (!start)
		start = mainStart;

	if (!final)
		final = mainFinal;
#if RANGECHECK
	else if (final < start)
		fatal("%s: emit final < start", name);
#endif  // RANGECHECK

	const char *emit = start, *s;
	const offield *field;
	offstr *o;

	for (field = fields, o = values; field->name; field++, o++)
	{
		o->reset();

		if (*field->name == '*')
		{
			o->wild = true;
			o->cache = strlen(field->name + 1);
		}
	}

	while ((s = strstr(emit, "{F_")) != NULL && s < final)
	{
		char fieldName[SIZE_OF_OFFIELD_NAME];

		s++;
		h.Write(emit, s - emit - 1);
		emit = s;
		strzcpy(fieldName, s, sizeof fieldName);

		if ((s = strchr(s, '}')) == NULL)
		{
			error("%s: отворено поле %s...", name, fieldName);
			break;
		}

		if (s - emit >= sizeof fieldName)
		{
			error("%s: прекалено дълго име на поле %s...", name, fieldName);
			continue;
		}

		fieldName[s - emit] = '\0';
		emit = s + 1;

		for (s = fieldName + 2; *s; s++)
			if (islocal(*s) || (!isupper(*s) && !isdigit(*s) && *s != '_'))
					break;
		if (*s)
		{
			error("%s: невалидно име на поле %s.", name, fieldName);
			continue;
		}

		for (field = fields, o = values; field->name; field++, o++)
			if ((o->wild ? memcmp(field->name + 1, fieldName, o->cache) : strcmp(field->name, fieldName)) == 0)
				break;

		if (field->name)
		{
			if (o->wild || !o->cache)
			{
				if (o->wild)
				{
					o->clear();
					group->SetName(fieldName);
				}
				else
					o->cache = true;

				if (field->print)
					field->print(*o, group);
				else
				{
				#if RANGECHECK
					TField *tField = group->Find(field->name);

					if (tField)
						tField->Print(*o);
					else
						error("office: %s: field not found", field->name);
				#else  // RANGECHECK
					group->Find(field->name)->Print(*o);
				#endif  // RANGECHECK
				}
			}

			const char *text = str(*o);
			h.puts(any(text) ? text : stub, '\v');
		}
		else
		{
			for (field = global_fields; field->name; field++)
				if (!strcmp(field->name, fieldName))
					break;

			if (field->name)
			{
				mstr w;
				field->print(w, &officeGroup);
				h.puts(str(w), '\v');
			}
			else
				h.PutRaw("&nbsp;&nbsp;&nbsp;");	// ms-specific
		}
	}

	if (emit < final)
		h.Write(emit, final - emit);
}

void TOffice::EmitBreak(const char *start, const char *final)
{
	h.PutRaw("</table>");
	EmitBreak();
	Emit(start, final, null_fields, NULL);
}

bool TOffice::Show(TWaitWindow *wait)
{
	// 2016:004 LPR: WEB_FIX.html etc. should be open by EDITOR
	//return h.Show(wait, wordFile, wordFile);
	return h.Show(wait, true, wordFile);
}

void TOffice::ShowSync(TWaitWindow *wait, sign sync, const char *fallWillOpen, const char *fallWasSaved)
{
	if (sync)
	{
		if (sync == true)
			Close();

		filter_display(wait, Name, fallWillOpen, fallWasSaved);
	}
	else
		Show(wait);
}

void TOffice::Emit(mstr &m, const offield *fields, TGroup *group, const char *stemplate)
{
	const char *name = group->Name;
	const char *emit = stemplate;
	const char *s;
	const offield *field;

	while ((s = strstr(emit, "{F_")) != NULL)
	{
		char fieldName[SIZE_OF_OFFIELD_NAME];

		s++;
		mbk_insert(&m, emit, m.len(), s - emit - 1);
		emit = s;
		strzcpy(fieldName, s, sizeof fieldName);

		if ((s = strchr(s, '}')) == NULL)
		{
			error("%s: отворено поле %s...", name, fieldName);
			break;
		}

		if (s - emit >= sizeof fieldName)
		{
			error("%s: прекалено дълго име на поле %s...", name, fieldName);
			continue;
		}

		fieldName[s - emit] = '\0';
		emit = s + 1;

		for (s = fieldName + 2; *s; s++)
			if (islocal(*s) || (!isupper(*s) && !isdigit(*s) && *s != '_'))
					break;

		for (field = fields; field->name; field++)
			if (!strcmp(field->name, fieldName))
				break;

		if (field->name)
		{
			mstr w;

			if (field->print)
				field->print(w, group);
			else
				group->Find(field->name)->Print(w);

			m.cat(w);
		}
		else
			m.cat("\xA0\xA0\xA0");
	}

	m.cat(emit);
}

bool show_office(const char *name, const offield *fields, TGroup *group, TOfficeCopy copy)
{
	TOffice office;

	if (office.Open(name, copy))
	{
		office.Emit(NULL, NULL, fields, group);
		return office.Show(NULL);
	}

	return false;
}
