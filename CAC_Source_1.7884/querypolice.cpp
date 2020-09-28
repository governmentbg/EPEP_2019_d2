#include "cac.h"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void query_police(TWindow *parent)
{
	TTextFile tf("текстов");
	FILE *f;

	if ((f = tf.OpenRead(parent, ".")) == NULL)
		return;

	log(LOG_QUERY, CM_ADMIN_QUERY_POLICE, "ОДП/РДВР от файл");

	char ucn[SIZE_OF_UCN + 2];
	TLawsuit lawsuit;
	TSideWind sideWind;	// 2004:012 BRW: using TSideWind

	THTMLFile h;
	h.OpenTemp();
	int count = 0;
	TWaitWindow wait(parent, WAIT);

	while (fgets(ucn, sizeof ucn, f) != NULL)
	{
		if (strlen(ucn) != SIZE_OF_UCN || ucn[LENGTH_OF_UCN] != '\n')
		{
			h.printf("Невалиден текст '%s'\n", ucn);
			continue;
		}

		ucn[LENGTH_OF_UCN] = '\0';
		msql m;

		{
			TCitizen citizen;

			citizen.ucnType = UCN_CITIZEN_UCN;
			strcpy(citizen.ucn, ucn);
			citizen.Try();
			citizen.Print(m);
			h.printf("%s\n", str(m));
		}

		m.Begin(sideWind.Name, "F_NO, F_YEAR, F_TYPE");
		m.AddString("F_UCN", ucn);
		m.AddChar("F_UCN_TYPE", UCN_CITIZEN_UCN);
		m.AddChar("F_TYPE", TYPE_PUNISHMENT_LAW);

		TQuery q(str(m));
		while (q.Read())
		{
			sideWind << q;

			lawsuit.key = sideWind.key;
			if (lawsuit.Get() && lawsuit.Defendant(sideWind.involvement))
			{
				h.puts("\n\n");
				query_law(h, &lawsuit);
			}
		}

		h.puts("------------------------------------------------------------------------------\n");
		count++;
	}

	switch (count)
	{
		case 0 : h.printf("Няма проверени лица\n"); break;
		case 1 : h.puts("Проверено е едно лице\n"); break;
		default : h.printf("Проверени са %d лица\n", count);
	}

	tf.Close();
	h.Show(&wait);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
// ----- TQueryProsecSender --------------------------------------------------
class TQueryProsecSender : public TGroup
{
public:
	TQueryProsecSender();
	static TGroup *Clone();

	const char *Status();

	long eispp;
	char connectNo[SIZE_OF_CONNECT_NO];
	long year;
	char ucnType;
	char ucn[SIZE_OF_UCN];
	long instype;
};

TQueryProsecSender::TQueryProsecSender()
	: TGroup("T_QUERY_PROSEC_SENDER", TQueryProsecSender::Clone)
{
	Add(
		new TLong("F_EISPP", &eispp),
		new TString("F_CONNECT_NO", connectNo, sizeof connectNo),
		new TLong("F_YEAR", &year),
		new TChar("F_UCN_TYPE", &ucnType),
		new TString("F_UCN", ucn, sizeof ucn),
		NULL
	);
}

TGroup *TQueryProsecSender::Clone()
{
	return new TQueryProsecSender;
}

enum
{
	INSTYPE_PROSEC = 0,
	INSTYPE_INVEST = 1,
	INSTYPE_PREINV = 2,
	INSTYPE_COUNT
};

const char *TQueryProsecSender::Status()
{
	switch (instype)
	{
		case INSTYPE_PROSEC : ucnType = UCN_INSTANCE_PROSEC; break;
		case INSTYPE_INVEST : ucnType = UCN_INSTANCE_DETECT; break;
		case INSTYPE_PREINV : ucnType = UCN_INSTANCE_POLICE; break;
		default : return "невалиден <вид>";
	}

	//if (no < 1 || no > NO_MAX)
	//	return "невалиден номер";

	if (year < YEAR_MIN || year > YEAR_MAX)
		return "невалидна година";

	TSender sender;
	msql m(sender.Name, "F_UCN");

	m.AddLong("F_EISPP", eispp);
	m.AddChar("F_UCN_TYPE", ucnType);
	TQuery q(str(m));

	if (!q.Read())
		return "липсващ адресант";

	strcpy(ucn, sender.ucn);
	return NULL;
}

// ----- TQueryProsecConnect -------------------------------------------------
class TQueryProsecConnect : public TConnect
{
public:
	bool found;
	TSender connectSender;

	void Load(FILE *l, const TRCDKey &lawsuit, char connectType);
	bool Matches(const TQueryProsecSender *sender) const;
	virtual void Print(mstr &m);
};

void TQueryProsecConnect::Load(FILE *l, const TRCDKey &lawsuit, char connectType)
{
	found = TryBy(lawsuit, connectType);

	if (found)
	{
		connectSender.ucnType = ucnType;
		strcpy(connectSender.ucn, ucn);

		if (!connectSender.Try("F_NAME, F_EISPP"))
			fprintf(l, "%ld/%ld: липсващ адресант %d/%s\n", lawsuit.no, lawsuit.year, ucnType, ucn);
		else if (!connectSender.eispp)
			fprintf(l, "%ld/%ld: адресант %d/%s няма код по ЕИСПП\n", lawsuit.no, lawsuit.year, ucnType, ucn);
	}
}

bool TQueryProsecConnect::Matches(const TQueryProsecSender *sender) const
{
	return sender->eispp == connectSender.eispp && sender->year == connectYear && !strcmpi(sender->connectNo, connectNo);
}

void TQueryProsecConnect::Print(mstr &m)
{
	if (found)
		TConnect::Print(m, true, false, "с-в");
}

// ----- TQueryProsecPerson --------------------------------------------------
class TQueryProsecPerson : public TGroup
{
public:
	TQueryProsecPerson();
	static TGroup *Clone();

	long MinYear() const;
	void JudgementCriteria(msql &m);
	const char *Status(FILE *l);

	bool operator<=(const TQueryProsecPerson &that) const;

	char ucn[SIZE_OF_UCN];
	TGArray<TQueryProsecSender> senders;
	mstr name;
	char involvement;

	const TLawsuit *lawsuit;
	TQueryProsecConnect connect[INSTYPE_COUNT];

	// 2017:178
	TJudgement judgement;	// was local to person_lawsuit()
	TGArray<TOutReg> outRegs;

	void ReadMoreData();

protected:
	sign Matches(const TQueryProsecConnect &connect) const;
};

TQueryProsecPerson::TQueryProsecPerson()
	: TGroup("T_QUERY_PROSEC_PERSON", TQueryProsecPerson::Clone)
{
	Add(new TString("F_UCN", ucn, sizeof ucn));
}

TGroup *TQueryProsecPerson::Clone()
{
	return new TQueryProsecPerson;
}

long TQueryProsecPerson::MinYear() const
{
	long year = YEAR_MAX;

	for (int i = 0; i < senders.Count(); i++)
		if (senders[i]->year < year)
			year = senders[i]->year;

	return year;
}

void TQueryProsecPerson::JudgementCriteria(msql &m)
{
	m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
	m.AddString("F_UCN", ucn);
	m.AddChar("F_UCN_TYPE", UCN_CITIZEN_UCN);
	m.AddChar("F_INVOLVEMENT", involvement);
	m.AddChar("NOT F_JUDGED", JUDGED_NOT_KNOWN);
	m.AddChar("NOT F_PUNISHMENT", PUNISHMENT_NOT_KNOWN);
	m.AddOrder("F_DATE DESC");
}

sign TQueryProsecPerson::Matches(const TQueryProsecConnect &connect) const
{
	sign match = -1;  // no connect ort sender: fuzzy match

	if (connect.found && connect.connectSender.eispp)
	{
		for (int i = 0; i < senders.Count(); i++)
		{
			TQueryProsecSender *sender = senders[i];

			if (sender->ucnType == connect.ucnType)
			{
				if (connect.Matches(sender))  // perfect match
					return true;
				// has connect and sender: no fuzzy match
				match = false;
			}
		}
	}

	return match;
}

const char *TQueryProsecPerson::Status(FILE *l)
{
	connect[INSTYPE_PROSEC].Load(l, lawsuit->key, CONNECT_TYPE_PROSEC_REQUEST);
	connect[INSTYPE_INVEST].Load(l, lawsuit->key, CONNECT_TYPE_INVESTIGATION);
	connect[INSTYPE_PREINV].Load(l, lawsuit->key, CONNECT_TYPE_PRE_INVESTIGATION);

	const char *status = NULL;

	for (int instype = 0; instype < INSTYPE_COUNT; instype++)
	{
		static constant STATUSES[INSTYPE_COUNT] = { "ПП не съвпада", "СД не съвпада", "дознанието не съвпада" };
		sign result = Matches(connect[instype]);

		if (result == true)
			return NULL;

		if (result == false && status == NULL)
			status = STATUSES[instype];
	}

	return status;
}

void TQueryProsecPerson::ReadMoreData()
{
	msql m(judgement.Name);

	JudgementCriteria(m);
	outRegs.Flush();

	TQuery q(str(m));
	TDecision decision;

	if (q.Read())
	{
		judgement << q;
		decision << q;
		decision.LoadOutRegs(outRegs);
	}
	else
		judgement.date.Clear();

	if (!outRegs.Count())
		outRegs.Add(new TOutReg);
}

bool TQueryProsecPerson::operator<=(const TQueryProsecPerson &that) const
{
	return strcmp(ucn, that.ucn) <= 0;
}

#define person OFFICE_GROUP_CAST(TQueryProsecPerson)

static void person_person(mstr &m, TGroup *group)
{
	if (!any(person->name))
	{
		TCitizen citizen;

		citizen.ucnType = UCN_CITIZEN_UCN;
		strcpy(citizen.ucn, person->ucn);

		if (!citizen.Try())
			person->name.cat("Липсва в базата данни");
		else
		{
			citizen.Print(person->name);
			person->name.delead(SIZE_OF_UCN_TYPE_UCN);
		}
	}

	m.printf("%s - %s", str(person->name), Involvement->Seek(person->involvement));
}

static void person_prosec(mstr &m, TGroup *group) { person->connect[INSTYPE_PROSEC].Print(m); }

static void person_invest(mstr &m, TGroup *group)
{
	person->connect[INSTYPE_INVEST].Print(m);
	m.sep("\v");
	person->connect[INSTYPE_PREINV].Print(m);
}

static void person_lawsuit(mstr &m, TGroup *group)
{
	m.printf("%ld/%ld; %D", person->lawsuit->key.no, person->lawsuit->key.year, &person->lawsuit->date);
	person->lawsuit->forceDate.Print(m, "; %D");

	if (!person->judgement.date.Empty())
		m.printf("; %s", Punishment->Seek(person->judgement.punishment));
}

static void person_subcount(mstr &m, TGroup *group)
{
	m.printf("%d", person->outRegs.Count());
}

static void person_text(mstr &m, TGroup *group)
{
	if (!person->judgement.date.Empty())
		m.cpy(person->judgement.text);
}

#undef person

static void query_resolution(mstr &m, const char *name, char resolution)
{
	if (resolution != RESOLUTION_FAKE)
	{
		m.sep("\v");
		m.printf("%s: %s", name, Resolution->Seek(resolution));
	}
}

#define outReg OFFICE_GROUP_CAST(TOutReg)

static void outreg_appeal_vz(mstr &m, TGroup *group)
{
	query_resolution(m, "Резултат от въззивна инстанция", outReg->resultI);
	query_resolution(m, "II Резултат от въззивна инстанция", outReg->resultII);
	m.sep("\v\v");
	m.cat(outReg->remarkVZ);
}

static void outreg_appeal_ks(mstr &m, TGroup *group)
{
	query_resolution(m, "Резултат от касационна инстанция", outReg->resultKS);
	m.sep("\v\v");
	m.cat(outReg->remarkKS);
}

#undef outReg

static offield query_prosec_fields[] =
{
	{ "F_UCN",		NULL },
	{ "F_PERSON",	person_person },
	{ "F_PROSEC",	person_prosec },
	{ "F_INVEST",	person_invest },
	{ "F_LAWSUIT",	person_lawsuit },
	{ "F_SUBCOUNT",	person_subcount },
	{ "F_TEXT",		person_text },
	{ "F_APPEAL_VZ",	outreg_appeal_vz },
	{ "F_APPEAL_KS",	outreg_appeal_ks },
	{ NULL, NULL }
};

// ----- entry point ---------------------------------------------------------
void query_prosec(TWindow *parent)
{
	TTextFile tf("текстов");
	FILE *f;

	if ((f = tf.OpenRead(parent, ".")) == NULL)
		return;

	log(LOG_QUERY, CM_ADMIN_QUERY_PROSEC, "НСлС от файл");

	int lines = 0, invalid = 0;
	TWaitWindow wait(parent, WAIT);
	char text[0x100];
	TTextFile lf("журнален");
	FILE *l = lf.OpenTemp("log");
	TGSArray<TQueryProsecPerson> persons;
	TQueryProsecPerson pp;
	TQueryProsecSender ss;

	while (fgets(text, sizeof text, f) != NULL)
	{
		int f, n;

		// UCN <instype> EISPP no year
		wait.TickCount(++lines, "обработени реда");
		f = sscanf(text, "%10c %ld %ld %s %ld %n", pp.ucn, &ss.instype, &ss.eispp, ss.connectNo, &ss.year, &n);

		if (f != 5 || n != strlen(text))
		{
			fprintf(l, "ред %d: невалиден формат\n", lines);
			continue;
		}

		int index;

		pp.ucn[LENGTH_OF_UCN] = '\0';

		if (!ucn_valid(UCN_CITIZEN_UCN, pp.ucn))
		{
			fprintf(l, "ред %d: невалидно ЕГН\n", lines);
			continue;
		}

		index = persons.Find(&pp);

		if (index == INT_MAX)
		{
			persons.AddCopy(&pp);
			index = persons.Find(&pp);
		}

		TGArray<TQueryProsecSender> &senders = persons[index]->senders;
		const char *status = ss.Status();

		if (status == NULL)
			senders.AddCopy(&ss);
		else
		{
			fprintf(l, "ред %d: %s\n", lines, status);
			invalid++;
		}
	}

	TOffice office;
	TLawsuit lawsuit;

	if (office.Open("QueryPPSD.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1autogen = office.Find("tr1 autogen");
		constant tr1appeal = office.Find("tr1 appeal");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < persons.Count(); i++)
		{
			TQueryProsecPerson *person = persons[i];
			TSideWind sideWind;
			msql m("T_SIDE_WIND W JOIN T_LAWSUIT L ON L.F_NO = W.F_NO AND L.F_YEAR = W.F_YEAR AND L.F_TYPE = W.F_TYPE",
				"DISTINCT W.F_NO, W.F_YEAR, W.F_TYPE, W.F_INVOLVEMENT, L.F_KIND, L.F_DATE, L.F_FORCE_DATE, "
					"L.F_SOURCE_KIND");

			m.Add("W.F_YEAR");
			m.printf(">= %ld", person->MinYear());
			m.AddChar("W.F_TYPE", TYPE_PUNISHMENT_LAW);
			m.AddString("F_UCN", person->ucn);
			m.AddChar("F_UCN_TYPE", UCN_CITIZEN_UCN);
			m.AddOrder("F_YEAR, F_NO");
			TQuery q(str(m));

			while (q.Read())
			{
				sideWind << q;
				lawsuit << q;

				const char *status;

				if (lawsuit.Other(sideWind.involvement))
					status = "трета страна";
				else
				{
					person->lawsuit = &lawsuit;
					status = person->Status(l);
				}

				if (status)
				{
					fprintf(l, "ЕГН %s, НД %ld/%ldг: %s\n", person->ucn, lawsuit.key.no, lawsuit.key.year,
						status);
					continue;
				}

				person->involvement = sideWind.involvement;
				person->ReadMoreData();

				office.Emit(tr1start, tr1appeal, query_prosec_fields, person);
				office.Emit(tr1appeal, tr1final, query_prosec_fields, person->outRegs[0]);

				for (int i = 1; i < person->outRegs.Count(); i++)
				{
					office.Emit(tr1start, tr1autogen, null_fields, NULL);
					office.Emit(tr1appeal, tr1final, query_prosec_fields, person->outRegs[i]);
				}
			}
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}

	tf.Close();
	fprintf(l, "Валидни редове: %d\n", lines);
	fprintf(l, "Редове с грешки: %d\n", invalid);
	lf.Show(&wait);
}
#endif  // AREAL || REGIONAL
