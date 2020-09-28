#include "cac.h"

#if COURT_TYPE == COURT_AREAL
void import_reregistered(TWindow *parent)
{
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenRead(parent, ".")) == NULL)
		return;

	char s[0x100];
	TLawsuit lawsuit;
	TDamn damn;

	THTMLFile h;
	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	int lines = 0, count = 0, invalid = 0, missing = 0, ignored = 0, wrong = 0;

	log(LOG_IMPORT, CM_ADMIN_IMPORT_REREGISTERED, "пререгистрация АВ");
	h.puts("Импорт на данни за пререгистрация в АВ\n\n");

	lawsuit.key.type = TYPE_FIRM_LAW;
	while (fgets(s, sizeof s, f) != NULL)
	{
		lines++;

		if (sscanf(s, "%ld %ld", &lawsuit.key.no, &lawsuit.key.year) != 2)
		{
			h.printf("Ред %d: невалиден формат\n", lines);
			invalid++;
		}
		else if (lawsuit.key.no < 1 || lawsuit.key.no > NO_MAX)
		{
			h.printf("Ред %d: невалиден номер на дело\n", lines);
			invalid++;
		}
		else if (lawsuit.key.year < YEAR_MIN || lawsuit.key.year > YEAR_MAX)
		{
			h.printf("Ред %d: невалидна година\n", lines);
			invalid++;
		}
		else
		{
			h.printf("Фирмено дело %ld/%ld: ", lawsuit.key.no, lawsuit.key.year);
			if (!find_damn(&lawsuit, &damn, false))
			{
				h.printf("%s\n", MISSING_DATA);
				h.puts("липсващи в базата, непопълнени или блокирани от друг потребител данни\n");
				missing++;
			}
			else if (damn.reRegister == Today)
			{
				h.puts("вече е пререгистрирано на днешна дата\n");
				ignored++;
			}
			else
			{
				damn.reRegister = Today;
				if (damn.Update("F_REREGISTER"))
				{
					h.puts("пререгистрирано\n");
					count++;
				}
				else
				{
					h.puts("грешка при запис\n");
					wrong++;
				}
			}
		}
	}

	tf.Close();

	h.putc('\n');
	h.printf("Прочетени редове: %d\n", lines);
	h.printf("Редове с грешки: %d\n", invalid);
	h.printf("Актуализирани дела: %d\n", count);
	h.printf("Липсващи дела: %d\n", missing);
	h.printf("Игнорирани дела: %d\n", ignored);
	h.printf("Дела с грешки: %d\n", wrong);

	h.Show(&wait, false);
}
#endif  // AREAL

static void strfix(char *s, size_t size)
{
	s[size - 1] = '\0';
	strimall(s);
}

static void import_name(char *s, const char *buffer, size_t size)
{
	memcpy(s, buffer, size - 1);
	strfix(s, size);
}

void import_known(TWindow *parent)
{
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenRead(parent, OUT_PATH)) == NULL)
		return;

	TWaitWindow wait(parent, WAIT);
	char s[103];	// 100 chars \n + bad + \0
	TCitizen citizen;
	TKnownPerson known;
	long courtArea;
	int line = 0, count = 0, ignored = 0;

	while (fgets(s, sizeof s, f) != NULL)
	{
		line++;
		if (strlen(s) == 101 && s[100] == '\n')
			s[100] = '\0';
		if (strlen(s) != 100)
		{
			error("Ред %d: невалидна дължина.", line);
			break;
		}
		// 2008:245 URQ: FIX: pre-clear reName/reFamily
		// 2008:245 LPR: FIX: reName 15s->15c, strimall()
		// 2013:074 IRQ: FIX: known.ucn 10s->10c
		int nscan = sscanf(s, "%c%10c%c%5ld%5ld%5ld",
			&known.ucnType, known.ucn, &known.involvement,
			&courtArea, &known.flags, &known.csjid);
		// 2010:112 LPR: sscanf() -> import_name()
		import_name(citizen.name, s + 27, sizeof citizen.name);
		import_name(citizen.family, s + 41, sizeof citizen.family);
		import_name(citizen.reName, s + 63, sizeof citizen.reName);
		import_name(citizen.reFamily, s + 78, sizeof citizen.reFamily);

		if (nscan != 6 ||
			known.ucnType != UCN_CITIZEN_UCN || !ucn_valid(known.ucnType, known.ucn) ||
			!any(citizen.name) || !any(citizen.family))
		{
			error("Ред %d: невалиден формат.", line);
			break;
		}

		if (known.Try())
			ignored++;
		else
		{
			citizen.ucnType = known.ucnType;
			strcpy(citizen.ucn, known.ucn);
			strcpy(citizen.country, BUILDIN_COUNTRY);
			*citizen.reCountry = '\0';
			if ((citizen.Try() || citizen.Insert()) && known.Insert())
				count++;
		}
	}

	tf.Close();

	if (count || ignored)
		message("%d импортирани лица, %d игнорирани.", count, ignored);
}

void import_informed(TWindow *parent, const char *ucn)
{
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenRead(parent, ".")) == NULL)
		return;

	char s[0x100];
	TInformed informed;
	TLawsuit lawsuit;

	THTMLFile h;
	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	int lines = 0, count = 0, wrong = 0, ignored = 0;

	log(LOG_IMPORT, CM_ADMIN_INFORMEDS, "дела за изпращане на е-мейли");
	h.puts("Импорт на дела за изпращане на е-мейли\n\n");

	while (fgets(s, sizeof s, f) != NULL)
	{
		int n;

		lines++;

		if (sscanf(s, "%ld %c %ld %ld%n", &Court->court, &informed.key.type, &informed.key.no, &informed.key.year, &n) != 4)
			h.printf("Ред %d: невалиден формат\n", lines);
		else if (!Court->Try())
			h.printf("Ред %d: невалиден съд\n", lines);
		else if (!informed.key.type || !strchr(TYPE_LAWSUITS, informed.key.type))
			h.printf("Ред %d: невалиден вид дело\n", lines);
		else if (informed.key.no < 1 || informed.key.no > NO_MAX)
			h.printf("Ред %d: невалиден номер на дело\n", lines);
		else if (informed.key.year < YEAR_MIN || informed.key.year > YEAR_MAX)
			h.printf("Ред %d: невалидна година\n", lines);
		else
		{
			strmove(s, s + n);
			strimall(s);

			if (strlen(s) >= SIZE_OF_EMAIL)
				h.printf("Ред %d: прекалено дълъг е-мейл\n", lines);
			else if (Court->court == COURT_CODE)
			{
				Type->Seek(informed.key.type);
				h.printf("%s %ld/%ld: ", Type->alias, informed.key.no, informed.key.year);

				lawsuit.key = informed.key;
				strcpy(informed.ucn, ucn);

				if (!lawsuit.Try("F_TYPE"))
					h.puts("няма такова дело\n");
				// 2013:116 LPR: see DELETE below
				//else if (informed.Try())
				//	h.puts("вече съществува\n");
				else
				{
					strcpy(informed.email, s);

					if (!count && !wrong)
					{
						msql m(informed.Name, MQ_DELETE);

						m.AddString("F_UCN", ucn);
						informed.DataBase->Execute(str(m), ET_ERROR);
					}

					if (informed.Insert())
					{
						h.puts("импортирано\n");
						count++;
					}
					else
					{
						h.puts("грешка при запис\n");
						wrong++;
					}
				}
			}
			else
				ignored++;
		}
	}

	tf.Close();

	h.putc('\n');
	h.printf("Прочетени редове: %d\n", lines);
	h.printf("Импортирани дела: %d\n", count);
	h.printf("Грешки при запис: %d\n", wrong);
	h.printf("Игнорирани дела: %d (за други съдилища)\n", ignored);

	h.Show(&wait, false);
}

#if COURT_TYPE == COURT_AREAL
void import_bankrupts(TWindow *parent, const char *creditor, TGArray<TBankrupt> *bankrupts)
{
	TBankrupt bankrupt;
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenRead(parent, ".")) == NULL)
		return;

	strcpy(bankrupt.creditor, creditor);

	char s[0x100];
	constant bulstat = s;

	THTMLFile h;
	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	int lines = 0, invalid = 0, ignored = 0, duplicate = 0, imported = 0, failed = 0;
	h.puts("Импорт на данни за дела по несъстоятелност\n\n");

	while (fgets(s, sizeof s, f) != NULL)
	{
		char *p;

		lines++;

		for (p = s; *p && !isspace(*p); p++)
		{
			if (!isalnum(*p))
			{
				p = s;
				break;
			}
		}

		*p++ = '\0';

		if (strlen(bulstat) < 9 || strlen(bulstat) > LENGTH_OF_BULSTAT)
		{
			h.printf("Ред %d: невалиден БУЛСТАТ/ЕИК\n", lines);
			invalid++;
			continue;
		}

		char *name = p;

		compress(name);

		while (*p)
		{
			if (strchr(FILTER_QUOTES, *p))
				*p = '"';
			else if (strchr(FILTER_MINUSES, *p))
				*p = '-';
			else if (!isalnum(*p) && !isspace(*p) && !strchr(".:,&!/", *p))
			{
				*name = '\0';
				break;
			}

			p++;
		}

		if (strlen(name) < 1 || strlen(name) > LENGTH_OF_FIRM_NAME)
		{
			h.printf("Ред %d: невалидно име\n", lines);
			invalid++;
			continue;
		}

		TFirm firm;
		msql m(firm.Name);

		m.AddString("F_BULSTAT", bulstat);
		TQuery q(str(m));

		if (q.Read())
		{
			firm << q;

			if (q.Read())
			{
				h.printf("%s: има две или повече фирми с този БУЛСТАТ/ЕИК\n", bulstat);
				duplicate++;
				//continue;		// 2013:127 MRQ/LRQ: comout
			}

			bankrupt << firm;

			if (bankrupt.Try())
			{
				h.printf("%s: вече фигурира в списъка\n", bulstat);
				ignored++;
				continue;
			}
		}
		else
		{
			firm.ucnType = UCN_FIRM_CODE;
			strcpy(firm.name, name);
			strupr(firm.name);
			strcpy(firm.bulstat, bulstat);

			if (!firm.Insert())
			{
				h.printf("%s: %s: грешка при запис\n", firm.Name, bulstat);
				failed++;
				continue;
			}

			bankrupt << firm;
		}

		if (bankrupt.Insert())
		{
			bankrupts->AddCopy(&bankrupt);
			imported++;
		}
		else
		{
			h.printf("%s: %s: грешка при запис\n", bankrupt.Name, bulstat);
			failed++;
		}
	}

	tf.Close();

	h.putc('\n');
	h.printf("Прочетени редове: %d\n", lines);
	h.printf("Редове с грешки: %d\n", invalid);
	h.printf("Добавени фирми: %d\n", imported);
	h.printf("Игнорирани фирми: %d\n", ignored);
	h.printf("Дублирани БУЛСТАТ/ЕИК: %d\n", duplicate);
	h.printf("Грешки при запис: %d\n", failed);

	h.Show(&wait, false);
}
#endif  // AREAL

long import_announces(TWindow *parent, const CDate &date, TGroupArray *deliverers)
{
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenRead(parent, OUT_PATH)) == NULL)
		return 0;

	THTMLFile h;
	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	int lines = 0, invalid = 0, imported = 0, failed = 0;
	char s[0x100];
	TSender sender;
	int court = 0;

	h.puts("Импорт на призовки/съобщения\n\n");

	while (fgets(s, sizeof s, f) != NULL)
	{
		const char *errstr = NULL;
		TAnnounce announce;
		int nscan = 0;
		bool old_format = strlen(s) == 163;		// 2015:085 +related
		TUser user;

		h.printf("Ред %d: ", ++lines);

		if (!old_format && strlen(s) != 183)
			errstr = "невалидна дължина на реда";
		else if ((nscan = sscanf(s, "%3d%5d%4d%1c%1c%5d%5d%7c%2d%2d%4d%10d%1c%2c%2c%3c%5c%100c%10c%10d",
			&announce.court, &announce.key.no, &announce.key.year, &announce.key.kind, &announce.subpoenaKind,
			&announce.uclp, &announce.street, announce.adrNo, &announce.sessionDate.day, &announce.sessionDate.month,
			&announce.sessionDate.year, &announce.sideNo, announce.subNo, announce.entrance, announce.floor,
			announce.apartment, announce.blockNo, announce.subpoenaText,	// old format
			user.ucn, &announce.requestId)) != (old_format ? 18 : 20))
		{
			errstr = "невалиден формат";
		}
		else
		{
			sprintf(sender.ucn, "%010d", announce.court);
			sender.ucnType = announce.SenderType();
			UCLP->uclp = Street->uclp = announce.uclp;
			Street->street = announce.street;
			user.ucn[LENGTH_OF_UCN] = '\0';

			if (announce.court == COURT_CODE)
				errstr = "данните са на този съд";
			else if (!sender.Try())
				errstr = "съда не е адресант за призовки/съобщения";
			else if (!announce.key.no || announce.key.year < YEAR_MIN || announce.key.year >= YEAR_MAX)
				errstr = "невалиден номер дело или година";
			else if (!strchr(CONNECT_KIND_ANNOUNCES, announce.key.kind))
				errstr = "невалиден вид дело/документ";
			else if (!strchr(SUBPOENA_KIND_GENERICS, announce.subpoenaKind))
				errstr = "невалиден вид призовка/съобщение";
			else if (announce.uclp <= 0 || announce.street < 0)	// street 0 allowed
				errstr = "невалидно населено място/улица";
			else if (!UCLP->Try() || (announce.street && !Street->Try()))
				errstr = "липсващо населено място/улица";
			else if (!announce.sessionDate.Empty() && !announce.sessionDate.Valid())
				errstr = "невалидна дата на заседание";
			else if (is_real_ucn(UCN_CITIZEN_UCN, user.ucn))
			{
				const char *ucn;

				for (ucn = user.ucn; *ucn; ucn++)
					if (!isdigit(*ucn))
						break;

				if (ucn - user.ucn < LENGTH_OF_UCN || !ucn_valid(UCN_CITIZEN_UCN,
					user.ucn))
				{
					errstr = "невалидно ЕГН на призовкаря";
				}
				else if (!user.Try("F_POST") || user.post != POST_DELIVERER)
					h.puts("няма призовкар с посоченото ЕГН; ");
				else if (!(user.flags & FLAG_ACTIVE))
					h.puts("призовкаря с посоченото ЕГН е неактивен; ");
				else
					strcpy(announce.delivererUcn, user.ucn);
			}
		}

		if (errstr)
		{
			if (nscan >= 3)
			{
				h.printf("%ld, %ld, %ld", announce.court, announce.key.no, announce.key.year);

				if (nscan >= 4)
				{
					if (strchr(CONNECT_KIND_ANNOUNCES, announce.key.kind))
						h.printf(" %s", ConnectKind->Seek(announce.key.kind));
					else
						h.printf(" %d", announce.key.kind);
				}

				h.puts(": ");
			}

			h.printf("%s\n", errstr);
			invalid++;
		}
		else
		{
			strfix(announce.adrNo, sizeof announce.adrNo);
			// 2016:032 IRQ: FIX: was receivedText
			strfix(announce.subpoenaText, sizeof announce.subpoenaText);
			strfix(announce.subNo, sizeof announce.subNo);
			strfix(announce.entrance, sizeof announce.entrance);
			strfix(announce.floor, sizeof announce.floor);
			strfix(announce.apartment, sizeof announce.apartment);
			strfix(announce.blockNo, sizeof announce.blockNo);

			announce.subpoenaDate = date;
			if (!atob(announce.delivererUcn))
			{
				strcpy(announce.delivererUcn, obtain_deliverer(deliverers, announce.uclp, announce.street,
					announce.adrNo, announce.blockNo));
			}
			announce.receivedStatus = SRS_FOR_DELIVER;

			if (announce.Insert())
			{
				h.puts("зареден\n");
				imported++;
				court = announce.court;
			}
			else
			{
				h.puts("грешка при запис\n");
				failed++;
			}
		}
	}

	tf.Close();

	h.putc('\n');
	h.printf("Прочетени редове: %d\n", lines);
	h.printf("Редове с грешки: %d\n", invalid);
	h.printf("Заредени призовки/съобщения: %d\n", imported);
	h.printf("Грешки при запис: %d\n", failed);

	h.Show(&wait, false);

	if (imported)
		tf.Remove();

	return court;
}

#include "import_personal_no.cpp"

enum
{
	SUBPOENA_MISSING,
	SUBPOENA_MISMATCH,
	SUBPOENA_IMPORTED,
	SUBPOENA_FAILED
};

static int import_subpoena(const TSubpoena &subpoena)
{
	msql m(subpoena.Name);

	m.AddLong("F_REQUEST_ID", subpoena.requestId);
	// 2018-07-17: FIX: non-multi, like export
	m.AddChar("NOT F_MULTI", SUBPOENA_MULTI);

	TQuery q(str(m));

	if (!q.Read())
		return SUBPOENA_MISSING;

	TSubpoena tSubpoena;
	TRCDKey origin;

	tSubpoena << q;

	if (!tSubpoena.FindOrigin(origin) || origin.no != subpoena.key.no || tSubpoena.sideNo != subpoena.sideNo)
		return SUBPOENA_MISMATCH;

	static constant RECEIVE_FIELDS = "F_SUBPOENA_RECEIVED, F_RECEIVED_TIME, F_RECEIVED_STATUS, F_RECEIVED_TEXT, "
		"F_SUBPOENA_RETURNED";
	bool sameServe = (const TSubServeData &) subpoena == (const TSubServeData &) tSubpoena;

	tSubpoena.ImportFields(&subpoena, RECEIVE_FIELDS);
	m.Begin(tSubpoena.Name, MQ_UPDATE);
	tSubpoena.Quote(m, RECEIVE_FIELDS);
	m.AddLong("F_REQUEST_ID", tSubpoena.requestId);

#if TESTVER  // 2018-07-09
	if (sameServe || tSubpoena.DataBase->Execute(str(m), ET_ERROR))
#else  // TESTVER
	if (sameServe || AppName->Execute(str(m), ET_ERROR))
#endif  // TESTVER
	{
		serve_stickers(&tSubpoena);
		return SUBPOENA_IMPORTED;
	}

	return SUBPOENA_FAILED;
}

static constant SUBPOENA_RESULT_TEXTS[] = { "грешка при четене", "несъвпадение в данните", "зареден", "грешка при запис" };

bool import_subpoenas(TWindow *parent)
{
	TTextFile tf;
	FILE *f;

	if ((f = tf.OpenRead(parent, OUT_PATH)) == NULL)
		return false;

	THTMLFile h;
	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	int lines = 0, invalid = 0, imported = 0;
	int failed = 0, missing = 0, mismatch = 0;
	char s[0x200];
	TSender sender;
	int court;

	h.puts("Импорт на данни за връчване\n\n");

	while (fgets(s, sizeof s, f) != NULL)
	{
		const char *errstr = NULL;
		TSubpoena subpoena;
		int nscan = 0;

		h.printf("Ред %d: ", ++lines);

		if (strlen(s) != 295)
			errstr = "невалидна дължина на реда";
		else if ((nscan = sscanf(s, "%3d%10d%2d%2d%4d%2d%2d%1c%250c%2d%2d%4d%5d%5d",
			&court, &subpoena.requestId, &subpoena.subpoenaReceived.day, &subpoena.subpoenaReceived.month,
			&subpoena.subpoenaReceived.year, &subpoena.receivedTime.hour, &subpoena.receivedTime.minute,
			&subpoena.receivedStatus, subpoena.receivedText, &subpoena.subpoenaReturned.day,
			&subpoena.subpoenaReturned.month, &subpoena.subpoenaReturned.year, &subpoena.key.no,
			&subpoena.sideNo)) != 14)
		{
			errstr = "невалиден формат";
		}
		else if (court != COURT_CODE)
			errstr = "невалиден съд";
		else if (!subpoena.subpoenaReceived.Valid())
			errstr = "невалидна дата на връчване";
		else if (!subpoena.receivedTime.Empty() && !subpoena.receivedTime.Valid())
			errstr = "невалиден час на връчване";
		else if (!strchr(SRS_DELIVEREDS, subpoena.receivedStatus))
			errstr = "невалиден статус на връчване";
		else if (!subpoena.subpoenaReturned.Valid())
			errstr = "невалидна дата на връщане";

		if (errstr)
		{
			if (nscan >= 1)
			{
				h.printf("%3ld", court);

				if (nscan >= 2)
					h.printf(",  %010ld", subpoena.requestId);

				h.puts(": ");
			}

			h.printf("%s\n", errstr);
			invalid++;
		}
		else
		{
			strfix(subpoena.receivedText, sizeof subpoena.receivedText);
			strrep(subpoena.receivedText, '\a', '\r');
			strrep(subpoena.receivedText, '\b', '\n');

			int result = import_subpoena(subpoena);

			switch (result)
			{
				case SUBPOENA_MISSING : missing++; break;
				case SUBPOENA_MISMATCH : mismatch++; break;
				case SUBPOENA_IMPORTED : imported++; break;
				default : failed++;  // SUBPOENA_FAILED
			}

			h.printf("%s\n", SUBPOENA_RESULT_TEXTS[result]);
		}
	}

	tf.Close();

	h.putc('\n');
	h.printf("Прочетени редове: %d\n", lines);
	h.printf("Редове с грешки: %d\n", invalid + mismatch);
	h.printf("Заредени призовки/съобщения: %d\n", imported);
	h.printf("Грешки при четене: %d\n", missing);
	h.printf("Грешки при запис: %d\n", failed);

	h.Show(&wait, false);

	if (imported)
		tf.Remove();

	return imported;
}
