// ----- TImportLawer --------------------------------------------------------
struct TImportLawer
{
public:
	TImportLawer(long tLine);

	void Parse(const char *line);
	bool Equal(const TImportLawer *that) const;
	void Error(const char *s);

	char name[SIZE_OF_CITIZEN_NAME];
	char reName[SIZE_OF_CITIZEN_NAME];
	char family[SIZE_OF_CITIZEN_NAME];
	char reFamily[SIZE_OF_CITIZEN_NAME];
	char personalNo[SIZE_OF_LAWER_PERSONAL_NO];

	long line;
	mstr errors;

protected:
	const char *Scan(const char *start, char *field, size_t fieldSize) const;
	static bool CheckNames(const char *names);
};

TImportLawer::TImportLawer(long tLine)
	: line(tLine)
{
	*reName = '\0';
	*reFamily = '\0';
}

bool TImportLawer::Equal(const TImportLawer *that) const
{
	mstr m;
	mstr tm;

	m.printf("%s~%s~%s~%s", name, family, reName, reFamily);
	tm.printf("%s~%s~%s~%s", that->name, that->family, that->reName, that->reFamily);
	return !m.cmp(tm);
}

bool TImportLawer::CheckNames(const char *names)
{
	long parts = 1;

	for (const char *s = names; *s; s++)
	{
		if (*s == '-')
			parts++;
		else if (*s < 'А' || *s > 'Я')
			return false;
	}

	const char *s = strchr(names, '-');

	if (s == names || (s && s[1] == '\0'))
		return false;

	return parts <= 2;
}

void TImportLawer::Error(const char *message)
{
	errors.printf("Ред %ld: %s\n", line, message);
}

const char *TImportLawer::Scan(const char *start, char *field, size_t fieldSize) const
{
	const char *end = strchr(start, ' ');

	if (!end)
		end = start + strlen(start);

	size_t len = end - start;

	if (len >= fieldSize)
		return NULL;

	memcpy(field, start, len);
	field[len] = '\0';
	return end + 1;
}

void TImportLawer::Parse(const char *line)
{
	long fields = 1;
	const char *s;

	for (s = line; *s; s++)
		if (*s == ' ')
			fields++;

	if (fields != 3 && fields != 4)
		Error("невалиден брой полета");
	else
	{
		char families[SIZE_OF_CITIZEN_FAMILY + SIZE_OF_CITIZEN_REFAMILY];

		if ((s = Scan(line, name, sizeof name)) == NULL)
			Error("собственото име е прекалено дълго");
		else if (!CheckNames(name))
			Error("невалиден формат на собственото име");
		else if (fields == 4 && (s = Scan(s, reName, sizeof reName)) == NULL)
			Error("презимето е прекалено дълго");
		else if (!CheckNames(reName))
			Error("невалидни символи в презимето");
		else if ((s = Scan(s, families, sizeof families)) == NULL)
			Error("фамилията е прекалено дълга");
		else if (!CheckNames(families))
			Error("невалиден формат на фамилията");
		else if (Scan(s, personalNo, sizeof personalNo) == NULL || strlen(personalNo) < LENGTH_OF_LAWER_PERSONAL_NO)
			Error("невалидна дължина на личния номер");
		else if (!long_valid(personalNo))
			Error("невалидни символи в личния номер");
		else if ((s = strchr(families, '-')) != NULL)
		{
			size_t len = s - families;

			if (len > LENGTH_OF_CITIZEN_FAMILY || strlen(s + 1) > LENGTH_OF_CITIZEN_REFAMILY)
				Error("невалидна дължина на първата или втората фамилия");
			else
			{
				memcpy(family, families, len);
				family[len] = '\0';
				strcpy(reFamily, s + 1);
			}
		}
		else if (strlen(families) > LENGTH_OF_CITIZEN_FAMILY)
			Error("невалидна дължина на фамилията");
		else	// single family
			strcpy(family, families);
	}
}

// ----- import_personal_no --------------------------------------------------
static bool la_isspace(char c) { return c == ' ' || c == '\t'; }

static void la_compress(char *const line)
{
	char *beg = line;

	while (*beg)
	{
		if (la_isspace(*beg))
		{
			char *end;

			*beg++ = ' ';
			for (end = beg; la_isspace(*end); end++);
			if (end > beg)
				strmove(beg, end);
		}
		else
			beg++;
	}
}

static const char *write_personal_no(msql &m, TGroup *group)
{
	m.Begin(group->Name, MQ_UPDATE);
	group->Quote(m, "F_PERSONAL_NO");
	group->ExactCriteria(m, MQ_SUBSEQ);
	return group->DataBase->Execute(str(m), ET_SILENT) ? "ОК (номерът е записан)" : "грешка при запис";
}

#if defined(INSTANCE) && defined(APPEAL)
// CAC
static const char *write_personal_no(const TImportLawer *lawer)
{
	msql m("T_CITIZEN C JOIN T_KNOWN_PERSON K ON K.F_UCN = C.F_UCN AND K.F_UCN_TYPE = C.F_UCN_TYPE", "K.*");

	m.AddString("C.F_NAME", lawer->name);
	m.AddString("C.F_RENAME", lawer->reName);
	m.AddString("C.F_FAMILY", lawer->family);
	m.AddString("C.F_REFAMILY", lawer->reFamily);
	m.AddChar("K.F_INVOLVEMENT", INVOLVEMENT_LEGALESE);

	TQuery q(str(m));
	if (!q.Read())
		return "липсва лице с юридическа правоспособност с посочените имена";

	TKnownPerson person;
	person << q;

	if (q.Read())
		return "две или повече лица с юридическа правоспособност с посочените имена";

	if (any(person.personalNo))
	{
		return strcmp(person.personalNo, lawer->personalNo) ? "лицето вече е регистрирано с различен личен номер!" :
			"ОК (личния номер вече е указан)";
	}

	strcpy(person.personalNo, lawer->personalNo);
	return write_personal_no(m, &person);
}
#else  // INSTANCE && APPEAL
// WEB
static const char *write_personal_no(const TImportLawer *lawer)
{
	mstr names(lawer->name);

	if (any(lawer->reName))
		names.printf(" %s", lawer->reName);

	names.printf(" %s", lawer->family);

	if (any(lawer->reFamily))
		names.printf("-%s", lawer->reFamily);

	msql m(User->Name);

	m.AddString("F_NAME", str(names));
	m.AddChars("F_CATEGORY", CATEGORY_REGULARS);
	m.AddLong("F_PURPOSE", PURPOSE_LAWER);
	TQuery q(str(m));

	if (!q.Read())
	{
		m.Begin(User->Name);

		m.AddString("F_NAME", str(names));
		TQuery r(str(m));

		if (!r.Read())
			return "липсва лице с посочените имена";

		*User << r;

		if (!strchr(CATEGORY_REGULARS, User->category))
			return "съществува лице с посочените имена, но с невалидна категория";

		return "съществува лице с посочените имена, но с невалидна цел на регистрация";
	}

	*User << q;

	if (q.Read())
		return "два или повече адвоката с посочените имена";

	if (any(User->personalNo))
	{
		return strcmp(User->personalNo, lawer->personalNo) ? "лицето вече е регистрирано с различен личен номер!" :
			"ОК (личния номер вече е указан)";
	}

	strcpy(User->personalNo, lawer->personalNo);
	return write_personal_no(m, User);
}
#endif  // INSTANCE && APPEAL

void import_personal_no(TWindow *parent)
{
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenRead(parent, ".")) == NULL)
		return;

	THTMLFile h;
	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);

	log(LOG_IMPORT, CM_ADMIN_IMPORT_PERSONAL_NO, "на лични номера на адвокати");
	h.puts("Импорт на лични номера на адвокати\n\n");

	char line[0x100];
	long lineNo = 0;
	TIArray<TImportLawer> lawers;

	while (fgets(line, sizeof line, f) != NULL)
	{
		TImportLawer *lawer = new TImportLawer(++lineNo);

		strupr(line);
		strimall(line);
		la_compress(line);

		for (char *s = line; *s; s++)
			if (strchr(FILTER_MINUSES, *s))
				*s = '-';

		lawer->Parse(line);
		lawers.Add(lawer);
	}

	while (lawers.Count())
	{
		const TImportLawer *lawer = lawers[0];

		if (any(lawer->errors))
			h.puts(str(lawer->errors));
		else
		{
			mstr equals;

			for (int index = 1; index < lawers.Count(); )
			{
				TImportLawer *tLawer = lawers[index];

				if (!any(tLawer->errors) && tLawer->Equal(lawer))
				{
					if (!strcmp(tLawer->personalNo, lawer->personalNo))
					{
						mstr duplicate;
						duplicate.printf("дублира ред %ld", lawer->line);
						tLawer->Error(str(duplicate));
					}
					else
					{
						equals.printf(", %ld", tLawer->line);
						lawers.Remove(index);
						continue;
					}
				}

				index++;
			}

			if (any(equals))
				h.printf("Редове %ld%s: еднакви имена\n", lawer->line, str(equals));
			else
				h.printf("Ред %ld: %s\n", lawer->line, write_personal_no(lawer));
		}

		lawers.Remove(0);
	}

	h.Show(&wait, false);
}
