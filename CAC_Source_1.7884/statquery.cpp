#include "cac.h"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
class TStatQueryFirstGroup : public TBookGroup
{
public:
	TStatQueryFirstGroup();

	virtual void FormCriteria(msql &m);

	TGArray<TSender> senders;
	long created;
};

#include "StatQueryFirstDialog.h"
#include "StatQueryFirstDialog.cpp"

TStatQueryFirstGroup::TStatQueryFirstGroup()
	: TBookGroup("T_LAWSUIT L JOIN T_CONNECT C ON L.F_NO = C.F_NO AND L.F_YEAR = C.F_YEAR AND L.F_TYPE = C.F_TYPE",
		"F_TYPE", "F_DATE", false)
{
	Add(
		new TRadio("F_FINISHED_DATE", &created, FALSE),
		new TRadio("F_LAWSUIT_DATE", &created, TRUE),
		NULL
	);

	minDate.Clear();
	maxDate.Clear();
}

void TStatQueryFirstGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "L.F_NO, L.F_YEAR, L.F_KIND, L.F_SUBJECT, C.F_CONNECT_KIND, C.F_CONNECT_NO, C.F_CONNECT_YEAR");
	// 2014:079 LRQ: F_DATE -> F_FINISHED; 2016:040 LPR: option
	m.AddRange(created ? "L.F_DATE" : "L.F_FINISHED", minDate, maxDate);
	m.AddString("C.F_UCN", senders[0]->ucn);
	m.AddChar("C.F_UCN_TYPE", senders[0]->ucnType);
	m.AddChar("C.F_CONNECT_TYPE", CONNECT_TYPE_LOWER_INSTANCE);
	m.AddOrder("L.F_YEAR, L.F_NO, L.F_TYPE");
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

class TStatQuerySecondGroup : public TDummyGroup
{
public:
	TStatQuerySecondGroup();

	bool operator<=(const TStatQuerySecondGroup &that);

	long no;
	long year;
	char kind;
	long subject;
	char connectKind;
	char connectNo[SIZE_OF_CONNECT_NO];
	long connectYear;
	char lowerType;
	long lowerNo;
	long lowerSubject;
	char lowerSubjectName[SIZE_OF_SUBJECT_NAME];
	long count;
};

TStatQuerySecondGroup::TStatQuerySecondGroup()
{
	Add(
		new TLong("F_NO", &no),
		new TLong("F_YEAR", &year),
		new TCharAlias("F_KIND", &kind),
		new TLong("F_SUBJECT", &subject),
		new TChar("F_CONNECT_KIND", &connectKind),
		new TString("F_CONNECT_NO", connectNo, sizeof connectNo),
		new TLong("F_CONNECT_YEAR", &connectYear),
		new TCharAlias("F_LOWER_TYPE", &lowerType),
		new TLong("F_LOWER_NO", &lowerNo),
		new TLong("F_LOWER_SUBJECT", &lowerSubject),
		new TLongString("F_LOWER_SUBJECT_NAME", lowerSubjectName, sizeof lowerSubjectName),
		new TLong("F_COUNT", &count),
		NULL
	);
}

bool TStatQuerySecondGroup::operator<=(const TStatQuerySecondGroup &that)
{
	Kind->Seek(kind);
	long order = Kind->OrderField()->L();
	Kind->Seek(that.kind);
	long that_order = Kind->OrderField()->L();

	return order != that_order ? order < that_order :
		subject != that.subject ? subject < that.subject :
		lowerType != that.lowerType ? lowerType < that.lowerType :
		lowerSubject != that.lowerSubject ? lowerSubject < that.lowerSubject :
		strcmpi(lowerSubjectName, that.lowerSubjectName) <= 0;
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
void stat_query_first(TWindow *parent)
{
	TStatQueryFirstGroup first;

	if (TStatQueryFirstDialog(parent, &first).Execute() == IDOK)
	{
		msql m;
		first.FormCriteria(m);
		TQuery q(str(m));

		TUserFile uf("текстов");
		FILE *f;
		char name[SIZE_OF_PATH];

		sprintf(name, "%s\\%d_%0D_%0D.txt", OUT_PATH, atoi(first.senders[0]->ucn), &first.minDate, &first.maxDate);

		if ((f = uf.Open(name, "w")) != NULL)
		{
			TStatQuerySecondGroup second;
			int count = 0;

			fprintf(f, "%d %d %d", atoi(first.senders[0]->ucn), first.minDate.Value(), first.maxDate.Value());

			if (first.created)
				fputs(" 1", f);

			fputc('\n', f);

			while (q.Read())
			{
				second << q;
				second.lowerType = connectKind2Type(second.connectKind);
				second.lowerNo = atoi(second.connectNo);

				fprintf(f, "%d %ld %ld %ld %d %ld %ld\n", second.kind, second.no, second.year, second.subject,
					second.lowerType, second.lowerNo, second.connectYear);

				count++;
			}

			uf.Close();
			message("Изходен файл: %s\n\n%d експортирани дела.", name, count);
		}
	}
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

class TStatQueryThirdGroup : public TDummyGroup
{
public:
	virtual void Print(mstr &m);

	bool Read(FILE *f);

	char name[SIZE_OF_PATH];
	long court;
	long minDate;
	long maxDate;
	int created;
};

void TStatQueryThirdGroup::Print(mstr &m)
{
	CDate tMinDate, tMaxDate;

	tMinDate.SetValue(minDate);
	tMaxDate.SetValue(maxDate);
	m.clear();
	m.printf("%s ", basename(name));
	get_sencor_name(UCN_INSTANCE_LOWER, court, m);
	m.printf(" %0D - %0D - %s", &tMinDate, &tMaxDate, created ? "За актуализация" : "За справка");
}

bool TStatQueryThirdGroup::Read(FILE *f)
{
	char buffer[0x100];

	if (!fgets(buffer, sizeof buffer, f))
		return false;

	int nscan = sscanf(buffer, "%d%d%d%d", &court, &minDate, &maxDate, &created);

	if (nscan == 3)
		created = 0;
	else if (nscan != 4)
		return false;

	return true;
}

void stat_query_second(TWindow *parent)
{
	TUserFile ufi("текстов");
	FILE *in;
	char buffer[0x100];
	TStatQueryThirdGroup third;
	mstr base;

	base.printf("%03d_*.TXT", COURT_CODE);

	if (!GetOpenFileName(parent, third.name, "Текстови файлове", OUT_PATH, str(base), sizeof third.name) ||
		(in = ufi.Open(third.name, "r")) == NULL)
	{
		return;
	}

	if (!third.Read(in))
	{
		error("%s: грешка при четене на заглавния ред.", third.name);
		return;
	}

	if (third.court != COURT_CODE)
	{
		error("%s: файлът е за %s.", Court->Seek(third.court), third.name);
		return;
	}

	TUserFile ufo("текстов");
	FILE *out;
	TStatQuerySecondGroup second;
	int lines = 0, count = 0, missing = 0, invalid = 0;
	TLawsuit lawsuit;

	strcpy(file_ext(third.name) - any(file_ext(third.name)), "_dop.txt");
	if ((out = ufo.Open(third.name, "w")) == NULL)
		return;

	fprintf(out, "%d %d %d", third.court, third.minDate, third.maxDate);

	if (third.created)
		fputs(" 1", out);

	fputc('\n', out);

	while (fgets(buffer, sizeof buffer, in))
	{
		lines++;

		if (sscanf(buffer, "%d%ld%ld%ld%d%ld%ld", &second.kind, &second.no, &second.year, &second.subject,
			&second.lowerType, &second.lowerNo, &second.connectYear) == 7)
		{
			lawsuit.key.no = second.lowerNo;
			lawsuit.key.year = second.connectYear;
			lawsuit.key.type = second.lowerType;

			if (lawsuit.Try("F_SUBJECT"))
			{
				second.lowerSubject = lawsuit.subject;
				strcpy(second.lowerSubjectName, Subject->Seek(second.lowerType, lawsuit.subject));
				count++;
			}
			else
			{
				error("Ред %d: липсващо дело.", lines);
				second.lowerSubject = 0;
				*second.lowerSubjectName = '\0';
				missing++;
			}

			fprintf(out, "%d %ld %ld %ld %d %ld %ld %ld %s\n", second.kind, second.no, second.year, second.subject,
				second.lowerType, second.lowerNo, second.connectYear, second.lowerSubject, second.lowerSubjectName);
		}
		else
		{
			error("Ред %d: невалиден формат.", lines);
			invalid++;
		}
	}

	ufi.Close();
	ufo.Close();

	message("Изходен файл: %s\n\n"
		"%d прочетени реда, %d експортирани дела, %d липсващи дела, %d невалидни реда.",
		third.name, lines, count, missing, invalid);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY

#define second OFFICE_GROUP_CAST(TStatQuerySecondGroup)

static void second_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(second->kind)); }

static void second_subject(mstr &m, TGroup *group)
{
	m.printf("%s %s", TSubject::Code(second->subject, second->kind),
		Subject->Seek(kind2type(second->kind), second->subject));
}

#undef second

static const offield second_fields[] =
{
	{ "F_KIND", second_kind },
	{ "F_SUBJECT", second_subject },
	{ "F_LOWER_SUBJECT", NULL },
	{ "F_LOWER_SUBJECT_NAME", NULL },
	{ "F_COUNT", NULL },
	{ NULL, NULL }
};

#define third OFFICE_GROUP_CAST(TStatQueryThirdGroup)

static void third_min_date(mstr &m, TGroup *group)
{
	CDate date;
	date.SetValue(third->minDate);
	date.Print(m);
}

static void third_max_date(mstr &m, TGroup *group)
{
	CDate date;
	date.SetValue(third->maxDate);
	date.Print(m);
}

#undef third

static const offield third_fields[] =
{
	{ "F_MIN_DATE", third_min_date },
	{ "F_MAX_DATE", third_max_date },
	{ NULL, NULL }
};

class TThirdStatQuery
{
public:
	TThirdStatQuery(const TGArray<TStatQueryThirdGroup> *tFiles) : files(tFiles) { }

	void Execute();

protected:
	const TGArray<TStatQueryThirdGroup> *files;

	virtual bool Start() = 0;
	virtual void Process(const TStatQuerySecondGroup &second) = 0;
	virtual void Final() = 0;
};

void TThirdStatQuery::Execute()
{
	if (Start())
	{
		TUserFile uf("текстов");
		TStatQuerySecondGroup second;

		for (int i = 0; i < files->Count(); i++)
		{
			FILE *f;
			char buffer[0x100];
			int lines = 1;
			const char *fileName = (*files)[i]->name;

			if ((f = uf.Open(fileName, "r")) == NULL)
				return;

			fgets(buffer, sizeof buffer, f);

			while (fgets(buffer, sizeof buffer, f) != NULL)
			{
				int n;

				lines++;

				if (sscanf(buffer, "%d%ld%ld%ld%d%ld%ld%ld%n", &second.kind, &second.no, &second.year,
					&second.subject, &second.lowerType, &second.lowerNo, &second.connectYear, &second.lowerSubject,
					&n) != 8)
				{
					error("%s: грешка в ред %d", fileName, lines);
					continue;
				}

				strcpy(second.lowerSubjectName, buffer + n);
				strimall(second.lowerSubjectName);
				Process(second);
			}

			uf.Close();
		}

		Final();
	}
}

class TThirdStatQuerySummary : public TThirdStatQuery
{
public:
	TThirdStatQuerySummary(const TGArray<TStatQueryThirdGroup> *files) : TThirdStatQuery(files) { }

protected:
	virtual bool Start();
	virtual void Process(const TStatQuerySecondGroup &second);
	virtual void Final();

	static constant Name;
	TOffice office;
	TGSArray<TStatQuerySecondGroup> array;
};

constant TThirdStatQuerySummary::Name = "StatQuery.htm";

bool TThirdStatQuerySummary::Start()
{
	if (!office.Open(Name, OFFICE_COPY_HEADER))
		return false;

	office.Emit(NULL, NULL, third_fields, (*files)[0]);
	office.Close();
	return true;
}

void TThirdStatQuerySummary::Process(const TStatQuerySecondGroup &second)
{
	int n = array.Find(&second);

	if (n != INT_MAX)
		array[n]->count++;
	else
	{
		TStatQuerySecondGroup *tSecond = new TStatQuerySecondGroup;

		*tSecond << second;
		tSecond->count = 1;
		array.Add(tSecond);
	}
}

void TThirdStatQuerySummary::Final()
{
	if (office.Open(Name, OFFICE_SKIP_HEADER, ""))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < array.Count(); i++)
			office.Emit(tr1start, tr1final, second_fields, array[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

class TThirdStatQueryMigrate : public TThirdStatQuery
{
public:
	TThirdStatQueryMigrate(const TGArray<TStatQueryThirdGroup> *files) : TThirdStatQuery(files) { }

protected:
	virtual bool Start();
	virtual void Process(const TStatQuerySecondGroup &second);
	virtual void Final();

	TTextFile tf;
	FILE *f;
	TLawsuit lawsuit;
};

bool TThirdStatQueryMigrate::Start()
{
	f = tf.OpenTemp("log");
	fputs("Зареждане на предмети от първоинстанционни дела\n\n", f);
	return true;
}

void TThirdStatQueryMigrate::Process(const TStatQuerySecondGroup &second)
{
	lawsuit.key.no = second.no;
	lawsuit.key.year = second.year;
	lawsuit.key.type = kind2type(second.kind);

	fprintf(f, "%s %ld/%ld: ", Type->Seek(lawsuit.key.type), lawsuit.key.no, lawsuit.key.year);

	if (!lawsuit.Try())
		fputs("грешка при четене", f);
	else if (lawsuit.kind != second.kind)
		fputs("точния вид не съвпада\n", f);
	else if (lawsuit.subject)
		fputs("вече има нов предмет\n", f);
	else
	{
		lawsuit.subject = second.subject;

		if (lawsuit.Update("F_SUBJECT"))
			fprintf(f, "предмет %s\n", lawsuit.Code());
		else
			fputs("грешка при запис\n", f);
	}
}

void TThirdStatQueryMigrate::Final()
{
	tf.Show(NULL);
}

static void stat_query_third(const TGArray<TStatQueryThirdGroup> *files)
{
	if ((*files)[0]->created)
		TThirdStatQueryMigrate(files).Execute();
	else
		TThirdStatQuerySummary(files).Execute();
}

#include "StatQueryListWindow.h"
#include "StatQueryListWindow.cpp"
#include "StatQueryThirdDialog.h"
#include "StatQueryThirdDialog.cpp"

void stat_query_third(TWindow *parent)
{
	TStatQueryThirdDialog(parent).Execute();
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY
