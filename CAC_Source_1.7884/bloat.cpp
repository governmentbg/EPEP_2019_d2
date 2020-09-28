#include "cac.h"

void adjust_law_college(char lawKind, TCharAliasFace *college)
{
	Kind->Seek(lawKind);
	char type = Kind->type;

	if (strchr(Default->colleges, type))
		college->SetC(type);
	else
	{
		message(MB_ICONWARNING | MB_OK, "Внимание! Документа е за %s, а Вие нямате права за работа с %s %s.",
			Type->Seek(type), College->Seek(type), COLLEGE);
	}
}

#if COURT_TYPE == COURT_APPEAL
char suggest_ses_kind(TLawsuit *, const char *kinds)
{
	return *kinds;
}
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static char suggest_ses_kind(const char last_kind, char type, char last_resort, bool is_split, const char *results)
{
#define EXIRES(result) strchr(results, result)
#if COURT_TYPE == COURT_REGIONAL
	// 2009:140 LPR: II -> I
	if (!is_split && type == TYPE_MARRIAGE_LAW)
	{
		// II.3
		if (EXIRES(RESULT_LEFT_1ST) || EXIRES(RESULT_LEFT_2ND))
			return KIND_2ND_SESSION;
		// II.3.5
		//if (!session)
		//	return KIND_PEACE_SESSION;
		// II.4
		if (last_kind == KIND_2ND_SESSION)
			return KIND_2ND_SESSION;
		// II.5
		if (EXIRES(RESULT_ENTER_OPEN) || EXIRES(RESULT_LEFT_MONTHLY) || EXIRES(RESULT_LEFT_UNMONTHLY))
			return KIND_1ST_SESSION;
		// II.6
		if (last_kind == KIND_1ST_SESSION)
			return KIND_1ST_SESSION;
	}
#endif  // REGIONAL

	// 2009:140 LPR: I -> II
	if (!is_split && strchr(TYPE_CITIZALIKE_LAWS, type))
	{
		// I.3
		if (EXIRES(RESULT_LEFT_1ST))
			return KIND_2ND_SESSION;
		// I.4
		if (last_kind == KIND_1ST_SESSION)
			return KIND_1ST_SESSION;
		// I.5
		if (last_kind == KIND_2ND_SESSION)
			return KIND_2ND_SESSION;
	}

#if COURT_TYPE == COURT_REGIONAL
	// III
	if (is_split)
	{
		// III.3
		if (EXIRES(RESULT_LEFT_3PH) || EXIRES(RESULT_LEFT_2ND3PH))
			return KIND_2ND_SESSION;
		// III.4
		if (EXIRES(RESULT_LEFT_2ND2PH) || EXIRES(RESULT_LEFT_1ST2PH))
			return KIND_2ND_SESSION;
		// III.5
		if (EXIRES(RESULT_ALLOW_SPLIT))
			return KIND_1ST_SESSION;
		// III.6
		if (EXIRES(RESULT_LEFT_2ND1PH) || EXIRES(RESULT_LEFT_1ST1PH))
			return KIND_2ND_SESSION;
		// III.7 == I.4
		//if (!session || last_kind == KIND_1ST_SESSION)
		if (last_kind == KIND_1ST_SESSION)
			return KIND_1ST_SESSION;
		// III.8 == I.5
		if (last_kind == KIND_2ND_SESSION)
			return KIND_2ND_SESSION;
	}
#endif  // REGIONAL

	return last_resort;
#undef EXIRES
}

char suggest_ses_kind(TLawsuit *lawsuit, const char *kinds)
{
	TGSArray<TRCDKeyContainer> array;
	TSession tSession;

	tSession.key = lawsuit->key;
	container_events(&tSession, &array);

	int count = array.Count();
	mstr results;

	for (int index = 0; index < count; index++)
		if (strchr(KIND_OPEN_SESSIONS, array[index]->kind))
			results.cach(((TSession *) array[index])->result);

	*Subject << lawsuit;
	if (!Subject->Get())
		fatal("suggest_ses_kind: missing subject %s", lawsuit->Code());

	// On NULL, the 1st strchr() will crash - this is intentional
	return *(strchr(kinds, (suggest_ses_kind(count-- ? array[count]->kind : KIND_NULL, lawsuit->key.type, *kinds,
	// 2017:079 LPR: don't use ALLOW_SPLIT for AdmC/OC/BC
	#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		false, str(results)))));
	#endif
	#if COURT_TYPE == COURT_REGIONAL
		strchr(Subject->Results(lawsuit->kind), RESULT_ALLOW_SPLIT) != NULL, str(results)))));
	#endif
}

static char suggest_ses_kind_1(const char last_kind, const mstr &results, const TLawsuit *lawsuit)
{
#if COURT_TYPE == COURT_REGIONAL
	*Subject << lawsuit;

	if (!Subject->Get())
		fatal("suggest_ses_kind: missing subject %s", lawsuit->Code());

	bool is_split = strchr(Subject->Results(lawsuit->kind), RESULT_ALLOW_SPLIT);

	// 2009:140 LPR: II -> I
	if (!is_split && lawsuit->key.type == TYPE_MARRIAGE_LAW)
	{
		// II.3
		if (results.chr(RESULT_LEFT_1ST) || results.chr(RESULT_LEFT_2ND))
			return KIND_2ND_SESSION;
		// II.3.5
		//if (!last_kind)
		//	return KIND_PEACE_SESSION;
		// II.4
		if (last_kind == KIND_2ND_SESSION)
			return KIND_2ND_SESSION;
		// II.5
		if (results.chr(RESULT_ENTER_OPEN) || results.chr(RESULT_LEFT_MONTHLY) || results.chr(RESULT_LEFT_UNMONTHLY))
			return KIND_1ST_SESSION;
		// II.6
		if (last_kind == KIND_1ST_SESSION)
			return KIND_1ST_SESSION;
	}
#endif  // REGIONAL

	// 2009:140 LPR: I -> II
	if (strchr(TYPE_CITIZALIKE_LAWS, lawsuit->key.type))
	{
		// I.3
		if (results.chr(RESULT_LEFT_1ST))
			return KIND_2ND_SESSION;
		// I.4
		if (last_kind == KIND_1ST_SESSION)
			return KIND_1ST_SESSION;
		// I.5
		if (last_kind == KIND_2ND_SESSION)
			return KIND_2ND_SESSION;
	}

#if COURT_TYPE == COURT_REGIONAL
	// III
	if (is_split)
	{
		// III.3
		if (results.chr(RESULT_LEFT_3PH) || results.chr(RESULT_LEFT_2ND3PH))
			return KIND_2ND_SESSION;
		// III.4
		if (results.chr(RESULT_LEFT_2ND2PH) || results.chr(RESULT_LEFT_1ST2PH))
			return KIND_2ND_SESSION;
		// III.5
		if (results.chr(RESULT_ALLOW_SPLIT))
			return KIND_1ST_SESSION;
		// III.6
		if (results.chr(RESULT_LEFT_2ND1PH) || results.chr(RESULT_LEFT_1ST1PH))
			return KIND_2ND_SESSION;
		// III.7 == I.4
		//if (!last_kind || last_kind == KIND_1ST_SESSION)
		if (last_kind == KIND_1ST_SESSION)
			return KIND_1ST_SESSION;
		// III.8 == I.5
		if (last_kind == KIND_2ND_SESSION)
			return KIND_2ND_SESSION;
	}
#endif  // REGIONAL

	return KIND_INVALID;
}

char suggest_ses_kind_1(TLawsuit *lawsuit, const char *kinds)
{
	TSession tSession;
	msql m;
	char last_kind = KIND_NULL;
	mstr results;

	tSession.key = lawsuit->key;
	tSession.FloatCriteria(m, "F_KIND, F_RESULT");
	m.AddChars("F_KIND", KIND_OPEN_SESSIONS);
	m.AddOrder("F_DATE, F_TIME");

	TQuery q(str(m));
	while (q.Read())
	{
		tSession << q;
		results.cach(tSession.result);
		last_kind = tSession.kind;
	}

	const char *suggest = strchr(kinds, suggest_ses_kind_1(last_kind, results, lawsuit));

	return suggest ? *suggest : *kinds;
}
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

// 2011:164 LPR: rewritten: m, full text, maxlen, skip alnum end
void transfer_dec_text(TCharAliasFace *result, TStringFace *text)
{
	if (strchr(RESULT_TEXTUALS, result->C()) && strlen(text->S()) < SIZE_OF_DECISION_TEXT - SIZE_OF_LONGER_NAME)
	{
		mstr m(text->S());
		int maxlen = 0;

		for (const char *res = RESULT_TEXTUALS; *res; res++)
		{
			int len = strlen(Result->Seek(*res));

			if (!strncmp(str(m), Result->name, len))
				maxlen = len;
		}

		bool insert = false;
		const char *s = str(m);

		if (!maxlen)
		{
			if (*s && !isalnum(*s))
				mbk_insert(&m, " ", 0, 1);
			insert = true;
		}
		else if (!isalnum(s[maxlen]))
		{
			m.delead(maxlen);
			insert = true;
		}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
		if (insert)
#endif  // APPEAL || ADMIN || MILITARY
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
		// 2014:255 hack to clear previous TEXTUAL text
		if (insert && result->C() != RESULT_READY_EXEC_PROT)
#endif  // AREAL || REGIONAL
		{
			Result->Seek(result->C());
			mbk_insert(&m, Result->name, 0, strlen(Result->name));
		}

		text->SetS(str(m));
	}
}

static bool equal_session_person(TPersonAliasFace *first, TPersonAliasFace *second)
{
	return first && second && atob(first) && atob(second) && !strcmp(first->S(), second->S());
}

static bool equal_session_person(TPersonAliasFace *first, TPersonAliasFace *second, const char *firstName,
	const char *secondName,	const char *spers_what)
{
	if (equal_session_person(first, second))
	{
		mstr m;
		size_t len;

		m.printf("%s и ", firstName);
		len = m.len();
		m.cat(secondName);
		strlwr(ncstr(m) + len);
		error("%s %s не могат да бъдат едно и също лице.", str(m), spers_what);
		return true;
	}

	return false;
}

bool equal_ses_person(TPersonAliasFace *persons[], constant names[], size_t count, const char *spers_what)
{
	for (int i = 0; i < count - 1; i++)
		for (int n = i + 1; n < count; n++)
			if (equal_session_person(persons[i], persons[n], names[i], names[n], spers_what))
				return true;
	return false;
}

bool equal_ses_person(TPersonAliasFace *person, TPersonAliasFace *persons[], size_t count)
{
	for (int i = 0; i < count; i++)
		if (equal_session_person(person, persons[i]))
			return true;

	return false;
}

#if COURT_TYPE == COURT_APPEAL
bool present_ses_person(char kind, TPersonAliasFace *judge, const char *name, const char *type)
{
	return !judge || atob(judge) ||
		ask("Делото е %s, но в заседанието липсва %s %s. %s", Kind->Seek(kind), name, type, Q_CONTINUE);
}
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
bool present_ses_person(char kind, TPersonAliasFace *judge, const char *name, const char *type)
{
	return !strchr(KIND_2ND_LAWSUITS, kind) || !judge || atob(judge) ||
		ask("Делото е %s, но в заседанието липсва %s %s. %s", Kind->Seek(kind), name, type, Q_CONTINUE);
}
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
bool present_ses_person(char, TPersonAliasFace *, const char *, const char *)
{
	return true;
}
#endif  // REGIONAL

// ----- TAppealDecision -----------------------------------------------------
struct TAppealDecision
{
	char type;
	char kind;
	const char *decisionKinds;
};

// ----- KIND_ ---------------------------------------------------------------
static const TAppealDecision AppealDecisions[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ 'l', 'ь', "Шълм" },
	{ 'l', '±', "Шълм" },	// 2008:028 cp from ь
	{ 'l', 'н', "Шълм" },
	{ 'l', 'о', "Шълм" },
	{ 'l', 'р', "Шълм" },
	{ 'l', 'е', "Шлм" },	// 2007:310 +лм
	{ 'i', 'н', "ьн±" },	// 2008:028 +±
	{ 'i', 'р', "ор" },
	{ 'r', 'н', KIND_INDOCS },
	{ 'r', 'р', KIND_INDOCS },
#endif  // APPEAL
	// 2010:067 LPR: port from INSTANCE, some elements may be useless
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	// 2009:237 +R where Ш
	{ 'l', 'Х', "ШълRм" },
	{ 'l', '¦', "ШълRм" },	// 2008:028 cp from Х
	{ 'l', 'Ь', "ШълмR" },
	{ 'l', 'а', "ШълRм" },
	{ 'l', 'в', "ШълмR" },
	{ 'l', 'е', "ШлмR" },	// 2007:310 +лм
	{ 'l', 'з', "ШR" },
	{ 'l', 'д', "ШъR" },
	{ 'l', '±', "ШъR" },	// 2008:028 cp from д
	{ 'l', 'µ', "ШъR" },	// 2007:288 copied from д
	{ 'l', 'н', "ШълмR" },
	{ 'l', '§', "ШълмR" },	// 2007:288 copied from н
	{ 'l', 'о', "ШъR" },
	{ 'l', 'р', "ШълмR" },
	{ 'i', 'Ь', "ХЬе¦" "дн" },	// 2008:028 +¦; 2016:032 +дн
	{ 'i', 'в', "аве" },
	{ 'i', 'н', "дне±" },
	{ 'i', 'р', "оре" },
	{ 'i', '§', "µ§" "дн" },	// 2016:032 +дн
	{ 'r', 'Ь', KIND_1ST_INDOCS },
	{ 'r', 'в', KIND_1ST_INDOCS },
	{ 'r', 'н', KIND_2ND_INDOCS },
	{ 'r', 'р', KIND_2ND_INDOCS },
	{ 'l', '"', "ШълмR" },
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	// 2008:059 +Ъ where S; 2008:070 +м where in area
	// 2016:236 +П where E; 2017:202 +‡ where needed
	{ 'l', 'Х', "EПGSЪм‡" },
	{ 'l', '¦', "EПGSЪм" },	// 2008:028 cp from Х
	{ 'l', 'Ь', "EПGSЪм‡" },
	{ 'l', 'а', "EПGSЪ" },
	{ 'l', 'в', "EПGSЪм" },
	{ 'l', 'е', "EПSЪмG" },	// 2007:310 +Gм; 2009:330 +G: was missing
	{ 'l', 'з', "EП" },
	{ 'l', 'д', "EП‡" },
	{ 'l', '±', "EП" },	// 2008:028 cp from д
	{ 'l', 'н', "SЪм‡" },
	{ 'l', 'о', "EПGSм" },	// 2008:204 LRQ: +GSм
	{ 'l', 'р', "EПGSЪм" },
	{ 'l', '¦', "‡" },
	{ 'l', '±', "‡" },
	{ 'i', 'Ь', "ХЬе¦" },	// 2008:028 +¦
	{ 'i', 'в', "ав" },
	{ 'i', 'н', "дн±о" },	// 2013:347 +о
	{ 'r', 'Ь', KIND_INDOCS },
	{ 'r', 'в', KIND_INDOCS },
	{ 'r', 'н', KIND_INDOCS },
#endif  // REGIONAL
	{ '\0', '\0', "" }
};

void adjust_appeal_decisions(char type, TCharAliasFace *kind, TCharAliasFace *decisionKind)
{
	const TAppealDecision *decision;

	for (decision = AppealDecisions; decision->kind; decision++)
		if (decision->type == type && decision->kind == kind->C())
			break;

	decisionKind->SetXCrit(decision->decisionKinds);
}

void adjust_judgement_priority(const TJudgement *judgement, TCheckFace *check)
{
	msql m(judgement->Name, "F_NO");

	m.Add(judgement, "F_NO, F_YEAR, F_TYPE, F_UCN, F_UCN_TYPE");
	m.Add("NOT");
	m.printf("(F_DATE = %ld AND F_KIND = '%c')", judgement->date.Value(), judgement->kind);
	m.AddFlags("F_FLAGS_X", JUDGEMENT_X_PRIORITY, JUDGEMENT_X_PRIORITY, JUDGEMENT_X_ALL);

	TQuery q(str(m));
	if (!q.Read())
	{
		check->EnableWindow(true);
		check->Check();
	}
}

char adjust_xxx_kind(char kind, TIndexFace *index, const char *kinds)
{
	if (index->L() > 0)
		kind = index2kind(index->L(), kinds);

	return kind;
}

void adjust_xxx_kind(TRCDKeyContainer *container, TIndexFace *index, const char *kinds)
{
	//if (index->L() > 0)
	//	container->kind = index2kind(index->L(), kinds);
	container->kind = adjust_xxx_kind(container->kind, index, kinds);
}

void adjust_xxx_index(char kind, TIndexFace *index, const char *kinds)
{
	if (kind && strchr(kinds, kind))
	{
		index->SetL(kind2index(kind, kinds));
		index->EnableWindow(true);
	}
	else
	{
		index->EnableWindow(false);
		index->SetL(0);
	}
}

char *adjust_xxx_kinds(char *kinds, const char *visibleKinds, const char *allKinds)
{
	if (strchr(kinds, *visibleKinds))
		strcat(kinds, allKinds + 1);

	return kinds;
}

// ----- KIND_ ---------------------------------------------------------------
static constant IndocKinds[] =
{
#if COURT_TYPE == COURT_APPEAL
	// 2007:051 VRQ; 2007:265 +PЁ +Аю
	"7Ж",
	"0Ч",
	"8w",
	"jЭ",
	"mҐ",
	"Гy",
	"fґ",
	"PА",
	"Ёю",
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL
	"Жб",
	//"су",
	"Чэ",
	//"тхй",
	"9ш",
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	// 2008:333 copied from AREAL; 2008:339 cleaned up -> empty
#endif  // MILITARY
	NULL
};

const char *adjust_indoc_kinds(char kind)
{
	constant *kinds;

	for (kinds = IndocKinds; *kinds != NULL; kinds++)
		if (strchr(*kinds, kind) != NULL)
			return *kinds;

	return NULL;
}

struct TLawsuitAccomply
{
	char type;
	const char *accDecs;
	const char *accSets;
	const char *accSess;
};

// ----- ACCOMPLY_ -----------------------------------------------------------
static const TLawsuitAccomply LawsuitAccomplys[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ TYPE_CITIZEN_LAW,	"nxc",	"ngo",	"ngov" },
	{ TYPE_RECITIZEN_LAW,	"nxc",	"ngo",	"ngov" },	// 2007:265
	{ TYPE_TRADE_LAW,		"nxc",	"ngo",	"ngov" },
	{ TYPE_RETRADE_LAW,	"nxc",	"ngo",	"ngov" },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ TYPE_ADMIN_LAW,		"nx",		"ng",		"ngsap" },
	// 2007:134 LPR: copied from above
	{ TYPE_TRICKY_LAW,	"nx",		"ng",		"ngsap" },
	{ TYPE_CITIZEN_LAW,	"nx",		"ng",		"ngsap" },
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	{ TYPE_PUNISHMENT_LAW,	"nx",		"nekmhjw",	"n" },
	{ TYPE_CITIZEN_LAW,	"nxcrb",	"ng",		"ngov" },
	{ TYPE_TRADE_LAW,		"nxc",	"ngx",	"ngvapx" },
	{ TYPE_ADMIN_LAW,		"nx",		"ng",		"ngsap" },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	// 2008:333 copied from AREAL; 2008:339 cleaned up
	{ TYPE_PUNISHMENT_LAW,	"nx",		"nekmhjw",	"n" },
	{ TYPE_CITIZEN_LAW,	"nxcrb",	"ng",		"ngov" },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ TYPE_PUNISHMENT_LAW,	"nx",		"ngo",	"ngo" },
	{ TYPE_CITIZEN_LAW,	"nxcb",	"ngo",	"ngo" },
	{ TYPE_MARRIAGE_LAW,	"nxcb",	"ngo",	"ngo" },
	{ TYPE_ADMIN_LAW,		"nxcb",	"ngo",	"ngo" },
#endif  // REGIONAL
	{ '\0',			"n",		"n",		"n" }
};

const char *lawsuit_accomplys(char type, char kind)
{
	TLawsuitAccomply const *accomply;

	for (accomply = LawsuitAccomplys; accomply->type; accomply++)
		if (accomply->type == type)
			break;

// 2007:134 LPR: new style court type check
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (type == TYPE_PUNISHMENT_LAW)
	{
		if (strchr(KIND_JUDGEMENTS, kind) || strchr(KIND_BULWARKS, kind))
			kind = KIND_DECISION;
		else if (strchr(KIND_DECISIONS, kind))
			kind = KIND_JUDGEMENT;
	}
#endif  // AREAL || MILITARY || REGIONAL
	if (strchr(KIND_DECISIONS, kind))
		return accomply->accDecs;
	// 2008:059 LPR: +EXECUTIVES == SETTLEMENTS
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (strchr(KIND_SETTLEMENTS, kind))
		return accomply->accSets;
#endif  // ADMIN || APPEAL || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	if (strchr(KIND_SETTLEMENTS, kind) || strchr(KIND_EXEC_PROTS, kind))
		return accomply->accSets;
#endif  // REGIONAL
	if (strchr(KIND_SESSIONS, kind))
		return accomply->accSess;

	return ACCOMPLY_NULLS;
}

void kill_forward_sides(const TRCDKeyContainer *container, const TSideWind *origin)
{
	msql m(origin->Name);
	TSideWind sideWind;
	CDate date;
	TExpense expense;
	TBaseWind linkWind("T_LINK_WIND", NULL);
	TSubpoena subpoena;
	TSubpoenaKind subpoenaKind;
	// 2010:050 FIX: eventStatus is primary key
	char eventStatus;
	TBaseWind tSubpoena("T_SUBPOENA", NULL);
	tSubpoena.Add(new TChar("F_EVENT_STATUS", &eventStatus));

	m.Add(origin, "F_UCN, F_UCN_TYPE, F_NO, F_YEAR, F_INVOLVEMENT");
	m.Add("F_DATE");
	m.printf(">= %ld", origin->date.Value());
	m.AddChars("NOT F_TYPE", TYPE_ORIGINS);	// 2014:261
	m.AddOrder("F_DATE");

	TQuery q(str(m));
	while (q.Read())
	{
		sideWind << q;
		date = sideWind.date;

		// 2008:059 FIX: KIND_DECISIONS -> KIND_ENDOCS
		if (strchr(KIND_ENDOCS, sideWind.kind))
		{
			TDecision decision;

			decision << sideWind;
			if (decision.Try("F_FROM_DATE"))
				date = decision.fromDate;
		}

		tSubpoena << sideWind;
		tSubpoena.ExactCriteria(m);

		TQuery r(str(m));
		bool subexist = r.Read();
		bool suberror = false;

		m.cpy("Лицето ");
		sideWind.PrintGroup(m);
		// 2008:059 FIX: sideWind.date -> date
		m.printf(" фигурира като \"%s\" и в списъка със страните за %s от %Dг.", Involvement->Seek(sideWind.involvement),
			Kind->Seek(sideWind.kind), &date);

		if (subexist)
			m.cat(" За него има и изпратени призовки/съобщения.");

		if (ask("%s\n\nЖелаете ли да го изтриете като страна по това събитие?", str(m)) && sideWind.Delete(true))
		{
			log(LOG_DELETE, IDDELETE, container, &sideWind);

			expense << sideWind;
			if (!expense.Delete(false))
				error("Не се изтриха всички глоби/възнаграждения за страната!");

			linkWind << sideWind;
			linkWind.Delete(true);
			linkWind.ExactCriteria(m);
			TQuery p(str(m));
			if (p.Read())
				error("Не се изтриха всички връзки на страната!");

			while (subexist)
			{
				subpoena << r;
				subpoenaKind.Seek(subpoena.subpoenaKind);

				if (subpoena.Delete(true))
				{
					m.clear();
					m.printf("%s %s %s %D", subpoenaKind.alias, UCNType->Seek(subpoena.ucnType), subpoena.ucn,
						&subpoena.date);
					log(LOG_DELETE, IDDELETE, container, str(m));
				}
				else
				{
					suberror = true;
					break;
				}

				subexist = r.Read();
			}

			if (suberror)
				error("Не се изтриха всички призовки/съобщения за страната!");
		}
	}
}

bool return_date_locked(const TLawsuit *lawsuit)
{
	*Subject << lawsuit;

	if (!Subject->Get() || strchr(Subject->Attrib(lawsuit->kind), POST_ENRETURN))
		return false;

	return LOCK_RETURN != 0;
}

struct TKind2SourceKinds
{
	char kind;
	char connectType;
	const char *sourceKinds;
};

// ----- KIND_ ---------------------------------------------------------------
static const TKind2SourceKinds Kind2SourceKinds[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ 'W', CONNECT_TYPE_RETURNED_LAWSUIT, "lLq" },
	{ 'H', CONNECT_TYPE_RETURNED_LAWSUIT, "vV" },
	{ 'Z', CONNECT_TYPE_RETURNED_LAWSUIT, "l" },
	{ 'N', CONNECT_TYPE_RETURNED_LAWSUIT, "vM" },
	{ 'T', CONNECT_TYPE_RETURNED_LAWSUIT, "l" },
	{ 'n', CONNECT_TYPE_RETURNED_LAWSUIT, "vФ" },
	// 2007:265 LRQ: no RECITIZEN/RETRADE for now
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	// 2007:257 LRQ: FIX: -ЯMЅn
	{ 'я', CONNECT_TYPE_JURISDICTION, "PщЁАюЕSЗ" },
	{ 'Н', CONNECT_TYPE_JURISDICTION, "КaјВЋ)" },	// 2014:213 FIX: +)
	{ 'Z', CONNECT_TYPE_JURISDICTION, "j" },
	{ 'N', CONNECT_TYPE_JURISDICTION, "U" },	// 2008:296

	{ 'я', CONNECT_TYPE_RETURNED_LAWSUIT, "PщЁАюЕSЗ" },
	{ 'Н', CONNECT_TYPE_RETURNED_LAWSUIT, "КaјВCЋ)" },	// 2014:213 FIX: +)
	{ 'Я', CONNECT_TYPE_RETURNED_LAWSUIT, "Ц" },
	{ 'M', CONNECT_TYPE_RETURNED_LAWSUIT, "ц" },
	{ 'Ѕ', CONNECT_TYPE_RETURNED_LAWSUIT, "ы" },
	{ 'Z', CONNECT_TYPE_RETURNED_LAWSUIT, "j" },
	{ 'n', CONNECT_TYPE_RETURNED_LAWSUIT, "й" },
	{ 'z', CONNECT_TYPE_RETURNED_LAWSUIT, "џ"},	// 2008:294 TRQ: FIX
	{ 'N', CONNECT_TYPE_RETURNED_LAWSUIT, "U" },	// 2008:296
	{ 'E', CONNECT_TYPE_RETURNED_LAWSUIT, "X" },	// 2008:304 LRQ: ~
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	// 2009:072 +'[' 'Z'-JM; +'{' 'T'-JM; +']' 'N'-v; +'}' 'n'-v
	{ 'O', CONNECT_TYPE_JURISDICTION, "u" },
	{ 'D', CONNECT_TYPE_JURISDICTION, "Џџ" },
	{ 'I', CONNECT_TYPE_JURISDICTION, "zXBK" },	// 2009:182 -E
	{ 'Z', CONNECT_TYPE_JURISDICTION, "Бг" },
	{ '[', CONNECT_TYPE_JURISDICTION, "JM" },
	{ 'я', CONNECT_TYPE_JURISDICTION, "Pыџ" },
	{ 'Н', CONNECT_TYPE_JURISDICTION, "Sыю" },
	{ 'T', CONNECT_TYPE_JURISDICTION, "Бг" },		// 2007:250 YQ -> гM
	{ '{', CONNECT_TYPE_JURISDICTION, "JM" },
	{ 'ж', CONNECT_TYPE_JURISDICTION, "щ" },

	{ 'O', CONNECT_TYPE_RETURNED_LAWSUIT, "u" },
	{ 'D', CONNECT_TYPE_RETURNED_LAWSUIT, "Џџ" },
	{ 'I', CONNECT_TYPE_RETURNED_LAWSUIT, "zXBK" },	// 2009:182 -E
	{ 'Z', CONNECT_TYPE_RETURNED_LAWSUIT, "Бг" },
	{ '[', CONNECT_TYPE_RETURNED_LAWSUIT, "JM" },
	{ 'я', CONNECT_TYPE_RETURNED_LAWSUIT, "Pыџ" },
	{ 'Н', CONNECT_TYPE_RETURNED_LAWSUIT, "Sюы" },
	{ 'T', CONNECT_TYPE_RETURNED_LAWSUIT, "Бгћ" },	// 2007:250 YQ -> гM
	{ '{', CONNECT_TYPE_RETURNED_LAWSUIT, "JM" },
	{ 'ж', CONNECT_TYPE_RETURNED_LAWSUIT, "щ" },

	{ 'W', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'C', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'U', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'H', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'П', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'N', CONNECT_TYPE_RETURNED_LAWSUIT, "l" },	// 2009:182 -Ѕ
	{ ']', CONNECT_TYPE_RETURNED_LAWSUIT, "v" },
	{ 'Я', CONNECT_TYPE_RETURNED_LAWSUIT, "Ц" },
	{ 'Е', CONNECT_TYPE_RETURNED_LAWSUIT, "ы" },
	{ 'n', CONNECT_TYPE_RETURNED_LAWSUIT, "l" },	// 2007:250 ѕ -> Ѕ; 2009:182 -Ѕ
	{ '}', CONNECT_TYPE_RETURNED_LAWSUIT, "v" },

	// 2007:215 LPR: common connect scheme; 2009:091 FIX: +[{]}
	{ 'O', CONNECT_TYPE_RERETURNED_LAWSUIT, "u" },
	{ 'O', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	// 2007:221 LRQ: all lawsuit kinds support renewed
	{ 'D', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'I', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'Z', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ '[', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'я', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'Н', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'T', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ '{', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'ж', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },

	{ 'W', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'C', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'U', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'H', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'П', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'N', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ ']', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'Я', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'Е', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'n', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ '}', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	// 2008:333 copied from AREAL -'ж'; 2008:339 cleaned up
	{ 'O', CONNECT_TYPE_JURISDICTION, "u" },
	{ 'Г', CONNECT_TYPE_JURISDICTION, "ґ" },
	{ 'D', CONNECT_TYPE_JURISDICTION, "Џџ" },
	{ 'I', CONNECT_TYPE_JURISDICTION, "EzXBKJ" },	// 2009:322 +J
	{ 'Z', CONNECT_TYPE_JURISDICTION, "Б" },

	{ 'O', CONNECT_TYPE_RETURNED_LAWSUIT, "u" },
	{ 'D', CONNECT_TYPE_RETURNED_LAWSUIT, "Џџ" },
	{ 'I', CONNECT_TYPE_RETURNED_LAWSUIT, "EzXBKJ" },	// 2009:322 +J
	{ 'Z', CONNECT_TYPE_RETURNED_LAWSUIT, "Б" },

	{ 'W', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'C', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'U', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'H', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'П', CONNECT_TYPE_RETURNED_LAWSUIT, "lvLV" },
	{ 'N', CONNECT_TYPE_RETURNED_LAWSUIT, "l" },
	{ ']', CONNECT_TYPE_RETURNED_LAWSUIT, "v" },

	{ 'O', CONNECT_TYPE_RERETURNED_LAWSUIT, "u" },
	{ 'O', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'D', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'I', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'Z', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },

	{ 'W', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'C', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'U', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'H', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'П', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
	{ 'N', CONNECT_TYPE_RENEWED_LAWSUIT, "є" },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ 'm', CONNECT_TYPE_RETURNED_LAWSUIT, "navoUY" },
	{ 'c', CONNECT_TYPE_RETURNED_LAWSUIT, "navoUY" },
	{ 'Z', CONNECT_TYPE_RETURNED_LAWSUIT, "navoUY" },	// 2007:032 LRQ: FIX
	{ 'O', CONNECT_TYPE_RETURNED_LAWSUIT, "navoUY" },
	{ 'R', CONNECT_TYPE_RETURNED_LAWSUIT, "navoUY" },
	{ 'D', CONNECT_TYPE_RETURNED_LAWSUIT, "navoUY" },
	{ 'I', CONNECT_TYPE_RETURNED_LAWSUIT, "navoUY" },

	{ 'm', CONNECT_TYPE_JURISDICTION, "j" },
	{ 'c', CONNECT_TYPE_JURISDICTION, "j" },
	{ 'Z', CONNECT_TYPE_JURISDICTION, "j" },	// 2007:032 LRQ: FIX
	{ 'O', CONNECT_TYPE_JURISDICTION, "j" },
	{ 'R', CONNECT_TYPE_JURISDICTION, "j" },
	{ 'D', CONNECT_TYPE_JURISDICTION, "j" },
	{ 'I', CONNECT_TYPE_JURISDICTION, "j" },

	// 2007:215 LPR: common connect scheme
	{ 'O', CONNECT_TYPE_RERETURNED_LAWSUIT, "б" },
	{ 'D', CONNECT_TYPE_RERETURNED_LAWSUIT, "б" },
	{ 'O', CONNECT_TYPE_RENEWED_LAWSUIT, "єҐ" },
	// 2007:221 LRQ: all lawsuit kinds support renewed
	{ 'm', CONNECT_TYPE_RENEWED_LAWSUIT, "єҐ" },
	{ 'c', CONNECT_TYPE_RENEWED_LAWSUIT, "єҐ" },
	{ 'Z', CONNECT_TYPE_RENEWED_LAWSUIT, "єҐ" },
	{ 'R', CONNECT_TYPE_RENEWED_LAWSUIT, "єҐ" },
	{ 'D', CONNECT_TYPE_RENEWED_LAWSUIT, "єҐ" },
	{ 'I', CONNECT_TYPE_RENEWED_LAWSUIT, "єҐ" },
#endif  // REGIONAL
	{ '\0', '\0', "" }
};

const char *adjust_source_kinds(char kind, const char *predefined)
{
	for (const TKind2SourceKinds *k2sk = Kind2SourceKinds; k2sk->kind; k2sk++)
		if (k2sk->kind == kind && strchr(predefined, k2sk->connectType))
			return k2sk->sourceKinds;

	fatal("%d/%s: unable to adjust_source_kinds", kind, predefined);
	return NULL;
}

struct TUserProfile
{
	char post;
	const char *menus;
	const char *functions;
};

// ----- POST_ ---------------------------------------------------------------
// ----- RIGHT_ --------------------------------------------------------------
static const TUserProfile UserProfiles[] =
{
	// 2010:104 +a
	{ 'r',	"Rqb",	"jHYvPnxZeX5Q" },
	{ 'c',	"RCqb",	"jlgH2YFNGWvPInxZeEVXrLfQz" },
	{ 's',	"Sqb",	"lgYFGWvPILfQzo" },	// 2008:165 TRQ: +o
	{ 'j',	"Jqb",	"lg2PfQz" },
	{ 'y',	"Jqb",	"lg2PfQz" },
	{ '1',	"Jqb",	"lg2P0fQz" },
	{ '2',	"Jqb",	"lg2P0fQz" },
	{ 'h',	"hqb",	"ijvP5Qzk" },		// 2007:241 -I
	{ 'w',	"3qb",	"L5a" },			// 2010:056 -s
	{ '\0',	NULL,	NULL }
};

void adjust_user_profile(char post, TCharListFace *menus, TCharListFace *functions, TCharListFace *colleges)
{
	if (menus->Empty() && functions->Empty() && (colleges->Empty() || post != POST_DELIVERER))
	{
		for (const TUserProfile *profile = UserProfiles; profile->post; profile++)
		{
			if (profile->post == post)
			{
				menus->SetS(profile->menus);
				functions->SetS(profile->functions);
				if (post == POST_DELIVERER)
					colleges->SetS(COLLEGE_ALLS);
				break;
			}
		}
	}
}

struct TConnectKindTypes
{
	const char *kinds;	// 2008:288 kind -> kinds
	const char *connectTypes;
};

// ----- KIND_ ---------------------------------------------------------------
// ----- CONNECT_TYPE_ -------------------------------------------------------
static const TConnectKindTypes ConnectKindTypes[] =
{
	// N.B. indocs should be before law kinds
	// 2008:203 +M where G; 2009:182 +P for many punishment
	// 2010:168 +O where F; 2010:274 +W where Q; 2012:045 +O for U/I
#if COURT_TYPE == COURT_APPEAL
	{ "$",	"OM" },           // 2017:311
	{ "W",	"FOIGQWM" },
	{ "H",	"FOIGQWMP" },	// 2018-03-22: +P
	{ "U",	"O" },		// 2009:182
	{ "П",	"FOIGM" },		// 2015:141 TRQ: copied from area
	{ "[]",	"OM" },		// 2015:170; 2016:111 +ї
	{ "ї",	"OM" "FIGQW" },	// 2016:342 IRQ/p.48.1: ї +FIGQW
	// 2007:265 LPR: comout, these should be automatic
	//{ 'Z',	"H" },
	//{ 'N',	"H" },
	//{ 'T',	"H" },
	//{ 'n',	"H" },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ "єXЏџ",	"E" },	// 2008:288 LRQ
	{ "я",	"T" },
	{ "MnN",	"E" },
#endif  // ADMIN
	// 2013:239 +d for O
#if COURT_TYPE == COURT_AREAL
	{ "aЏ",	"FOIGM" },		// 2008:203 LRQ: override 'D' rule
	{ "К",	"IGQWVMOF" },	// 2010:061 LRQ: 'I' +F
	{ "eh",	"IGQWVMOP" },	// 2016:294 PRJ: 'I' +P
	{ "В",	"n" },		// 2011:147; 2018-05-08: FIX: was \\ 
	{ "70",	"IGQWVMOP" },	// 2017:096
	{ "O",	"FOIGQWMd" },
	{ "D",	"FOI" },		// 2007:213
	{ "I",	"IGQWVMO" },
	{ "W",	"FOIGQWM" },
	{ "H",	"IGQWM" },
	{ "U",	"FOI" },		// 2007:213
	{ "П",	"FOIGM" },		// 2007:316 LRQ; 2009:166 TRQ: +GM
	//{ "C",	"" },			// 2009:182
	{ "я",	"T" },
	// 2010:285 TRQ: FIX: moved from non-predefined to common
	// 2010:286 LPR: moved here from get_all_possible_connects
	{ KIND_INCONSISTENCE_INDOCS, CONNECT_TYPE_FIRM_DEBTORS },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	// 2008:333 copied from AREAL; 2008:339 cleaned up
	{ "aЏ",	"FOIGM" },
	{ "К",	"IGQWVMOF" },	// 2010:061 LRQ: 'I' +F
	{ "eh",	"IGQWVMOP" },	// 2016:294 PRJ: 'I' +P
	{ "O",	"FOIGQWMd" },
	{ "DU",	"FOI" },
	{ "I",	"IGQWVMO" },
	{ "W",	"FOIGQWM" },
	{ "H",	"IGQWM" },
	{ "П",	"FOI" },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ "ш",	"IGKQWMOu" },
	{ "щ",	"u" },
	// 2007:303 +K where IGQ
	{ "P",	"eIGKQWEFOM" },	// 2011:147
	{ "O",	"FOIGKQWMd" },
	//{ "R",	"" },			// 2009:182
	{ "D",	"IGKQWEFOM" },	// 2007:213 +F
	{ "I",	"IGKQWMO" },
#endif  // REGIONAL
	{ NULL, NULL }
};

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static void add_subseq_connects(char *possible, const char *already)
{
	if (strchr(already, CONNECT_TYPE_CONFLICT_INTEREST) || strchr(already, CONNECT_TYPE_STRIFE_JURISDICTION))
		strcach(possible, CONNECT_TYPE_COMPETENT_HIGH);
}

static void add_startup_connects(char *possible, const char *already)
{
	strcat(possible, CONNECT_TYPE_STARTUPS);
	add_subseq_connects(possible, already);
}
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
static void add_2ndary_connects(char *possible, char primary)
{
	if (!strchr(possible, primary))
		strcach(possible, primary);

	// 2015:078 REM: filtered by CONNECT_TYPE_EXCLUSIVES
//	if (!strchr(already, CONNECT_TYPE_CONFLICT_INTEREST))
		strcach(possible, CONNECT_TYPE_HIGH_JURISDICTION);

	// 2015:078 REM: filtered by CONNECT_TYPE_EXCLUSIVES
//	if (!strchr(already, CONNECT_TYPE_HIGH_JURISDICTION))
		strcach(possible, CONNECT_TYPE_CONFLICT_INTEREST);

	if (primary)
		strcach(possible, CONNECT_TYPE_AFTER_ADMINISTER);
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_APPEAL
static void add_recall_connects(char *possible, char indocKind)
{
	if (strchr(KIND_CT274_INDOCS, indocKind) || indocKind == KIND_SPEEDUP_REQUEST)
		strcach(possible, CONNECT_TYPE_RECALL_LAWSUIT);
}
#endif  // COURT_APPEAL

#if COURT_TYPE == COURT_ADMIN
static char lower_connect(char indocKind)
{
	return strchr(KIND_RENEW_JUMP_INDOCS, indocKind) ? '\0' : CONNECT_TYPE_LOWER_INSTANCE;
}
#endif  // ADMIN

static void get_all_possible_connects(char *possible, char indocKind, char lawKind, const char *already, const char *predefined)
{
#if COURT_TYPE == COURT_APPEAL
	// 2007:265 Bloat +related
	char tLowerType = strchr(KIND_RECALL_LAWS, lawKind) ? CONNECT_TYPE_RECALL_LAWSUIT : CONNECT_TYPE_LOWER_INSTANCE;

	// 2015:170 PRQ/LPR: direct hack
	// 2016:342 IRQ/p.48.1: RENEW -> RENEW_LIMITED
	if (strchr(KIND_RENEW_LIMITED_LAWS, lawKind) && indocKind)
		strcpy(possible, CONNECT_TYPE_LOWER_INSTANCES);
	else
#endif  // APPEAL
	if (predefined)
	{
		strcpy(possible, predefined);
#if COURT_TYPE == COURT_APPEAL
		add_2ndary_connects(possible, tLowerType);
		add_recall_connects(possible, indocKind);
		// 2017:003 TRQ/FIX: copied from below
		if (lawKind == KIND_RENEW_UNLIMITED_LAW)
			strcach(possible, CONNECT_TYPE_RECALL_LAWSUIT);
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		if (strchr(KIND_1ST_LAWSUITS, lawKind))
			add_subseq_connects(possible, already);
		// 2007:242 LPR/LRQ: dynamic virtual lower instance
		else if (strchr(KIND_2ND_LAWSUITS, lawKind))
		{
		#if COURT_TYPE == COURT_ADMIN
			add_2ndary_connects(possible, lower_connect(indocKind));
		#endif  // ADMIN
		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
			add_2ndary_connects(possible, CONNECT_TYPE_LOWER_INSTANCE);
		#endif  // AREAL || MILITARY
		}
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
		add_subseq_connects(possible, already);
#endif  // REGIONAL
	}
	else
	{
		*possible = '\0';
		strcach(possible, CONNECT_TYPE_RETURNED_LAWSUIT);
		// 2007:239 LRP: FIX: only areal & regional support renewed
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		strcach(possible, CONNECT_TYPE_RENEWED_LAWSUIT);
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL
		add_2ndary_connects(possible, indocKind != KIND_CANCEL_ENACTMENT ? tLowerType : '\0');
		add_recall_connects(possible, indocKind);

		if (lawKind == KIND_RENEW_UNLIMITED_LAW)
			strcach(possible, CONNECT_TYPE_RECALL_LAWSUIT);
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
		if (strchr(KIND_1ST_INDOCS, indocKind))
			add_startup_connects(possible, already);
		else
			add_2ndary_connects(possible, lower_connect(indocKind));
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		if (strchr(KIND_1ST_INDOCS, indocKind))
		{
			add_startup_connects(possible, already);
			if (strchr(KIND_RERETURNED_DOCS, indocKind))
				strcach(possible, CONNECT_TYPE_RERETURNED_LAWSUIT);
		}
		else
			add_2ndary_connects(possible, CONNECT_TYPE_LOWER_INSTANCE);
#endif  // AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
		add_startup_connects(possible, already);
		if (strchr(KIND_RERETURNED_DOCS, indocKind))
			strcach(possible, CONNECT_TYPE_RERETURNED_LAWSUIT);
#endif  // REGIONAL
		if (strchr(already, CONNECT_TYPE_RETURNED_LAWSUIT) || strchr(already, CONNECT_TYPE_RENEWED_LAWSUIT))
			strcach(possible, CONNECT_TYPE_HIGHER_INSTANCE);
	}

	for (const TConnectKindTypes *kindTypes = ConnectKindTypes; kindTypes->kinds; kindTypes++)
	{
		// 2008:203 LRQ: also compare against indocKind
		if (strchr(kindTypes->kinds, lawKind) || strchr(kindTypes->kinds, indocKind))
		{
			strcat(possible, kindTypes->connectTypes);
			break;
		}
	}
}

void adjust_possible_connects(char *possible, char indocKind, char lawKind, const char *already, const char *predefined)
{
	get_all_possible_connects(possible, indocKind, lawKind, already, predefined);

	for (const char *alred = already; *alred; alred++)
	{
		char *found = strchr(possible, *alred);

		if (found)
		{
			strmove(found, found + 1);
			if (strchr(CONNECT_TYPE_EXCLUSIVES, *alred))
				for (const char *excls = CONNECT_TYPE_EXCLUSIVES; *excls; excls++)
					if ((found = strchr(possible, *excls)) != NULL)
						strmove(found, found + 1);
		}
	}
}

bool connect_types_valid(char indocKind, char lawKind, const char *entered, const char *predefined)
{
	char mandatory[CONNECT_TYPES_MAX];
	char possible[CONNECT_TYPES_MAX];

	get_all_possible_connects(possible, indocKind, lawKind, entered, predefined);

	if (predefined)
		strcpy(mandatory, predefined);
	else
	{
		*mandatory = '\0';
		if (strchr(possible, CONNECT_TYPE_HIGHER_INSTANCE))
			strcach(mandatory, CONNECT_TYPE_HIGHER_INSTANCE);
	}

	// 2007:242 LPR: dynamic lower: moved from !predefined block above
#if COURT_TYPE == COURT_APPEAL
	// 2007:265 Bloat +related; 2008:316 LPR: also handle FIRST laws
	char lowerType = '\0';

	if (strchr(KIND_RECALL_LAWS, lawKind))
		lowerType = CONNECT_TYPE_RECALL_LAWSUIT;
	else if (!strchr(KIND_FIRST_LAWS, lawKind))
		lowerType = CONNECT_TYPE_LOWER_INSTANCE;

	if (lowerType && strchr(possible, lowerType))
		strcach(mandatory, lowerType);
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(possible, CONNECT_TYPE_LOWER_INSTANCE))
		strcach(mandatory, CONNECT_TYPE_LOWER_INSTANCE);
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

	for (const char *mand = mandatory; *mand; mand++)
		if (!strchr(entered, *mand))
			return error("Данните за %s са задължителни.", ConnectType->Seek(*mand));

	for (const char *entr = entered; *entr; entr++)
		if (!strchr(possible, *entr))
			return error("Не се поддържат данни за %s.", ConnectType->Seek(*entr));

	return true;
}

bool check_set_session(TSession *session)
{
	msql m;

	session->ExactCriteria(m, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_TIME");
	m.AddChars("F_RESULT", RESULT_UNLINKEDS);

	TQuery q(str(m));
	if (!q.Read())
		return session->Insert();

	TSession tSession;

	tSession << q;
	if (session->time == tSession.time)
	{
		return ask("За посочените дата и час има %s със статус \"отменено\". Желаете ли да бъде възстановен "
			"статус \"насрочено\" за заседанието?", Kind->Seek(session->kind)) && session->Update();
	}
	else if (ask("За посочената дата има %s от %d.%02dч. със статус \"отменено\". Желаете ли да бъде променен "
		"часа на заседанието и да бъде възстановен неговия статус \"насрочено\"?", Kind->Seek(session->kind),
		tSession.time.hour, tSession.time.minute) && session->Update())
	{
		TBaseWind subpoena("T_SUBPOENA", NULL);

		subpoena << tSession;
		if (!subpoena.Delete(false))
			error("Старите съобщения по заседанието не са изтрити напълно!");
		return true;
	}

	return false;
}

void adjust_lawsuit_kinds(const char *predefined, char *kinds)
{
	*kinds = '\0';

	for (const TKind2SourceKinds *k2sk = Kind2SourceKinds; k2sk->kind; k2sk++)
	{
		if (
			strchr(predefined, k2sk->connectType) &&
			strchr(Default->colleges, kind2type(k2sk->kind))
#if COURT_TYPE == COURT_MILITARY
			&& (CourtType == COURT_MILITARY) == (strchr(KIND_1ST_LAWSUITS, k2sk->kind) != NULL)
#endif  // MILITARY
		)
			strcach(kinds, k2sk->kind);
	}
}

void adjust_lawsuit_limited(const TLawsuit *lawsuit, TCheckFace *limited, bool clear)
{
	Subject->Seek(lawsuit->key.type, lawsuit->subject);

	if (strchr(Subject->Attrib(lawsuit->kind), POST_LIMITED))
	{
		limited->SetCheck(BF_CHECKED);
		limited->EnableWindow(false);
	}
	else
	{
		if (clear)
			limited->SetCheck(BF_UNCHECKED);

		limited->EnableWindow(can_func(RIGHT_LIMITED));
	}
}

bool lawsuit_finished_valid(const TLawInterval &interval, TDateFace *finished)
{
	TSession session;
	bool found = find_finalizer(interval, session, interval.date, interval.finished);
	bool result = true;

	if (found)
	{
		if (finished->Date().Empty())
		{
			result = ask("Датата на свършване не е попълнена, но по делото има приключващо заседание "
				"или съдебен акт. %s", Q_CONTINUE);
		}
		else if (session.final != finished->Date())
		{
			result = ask("Указаната дата на свършване не съответства на нито едно заседание или съдебен акт "
				"приключващи делото. %s", Q_CONTINUE);
		}
	}
	else if (!finished->Date().Empty())
		result = ask("Указана е дата на свършване, но няма заседание или съдебен акт приключващи делото. %s", Q_CONTINUE);

	return result;
}

void adjust_ses_final(TSession *session, const char *results, CDate &date)
{
	if (strchr(results, session->result))
		session->final = date.Empty() ? session->date : date;
	else
		session->final.Clear();
}

static bool subject_flags_dangers(long subject, long flags)
{
	TGArray<TDangerSubject> dangerSubjects;

	DangerSubject->subject = subject;
	// note: always cached
	DangerSubject->LoadData(&dangerSubjects, NULL, flags);
	return dangerSubjects.Count();
}

void adjust_lawsuit_danger(TLawsuit *lawsuit, long flags, const char *what)
{
	if (lawsuit->danger && subject_flags_dangers(lawsuit->subject, flags))
	{
		lawsuit->danger = 0;
		info("Заради промяната на %s е необходимо завишената опасност да се въведе наново.", what);
	}
}

bool lawsuit_danger_valid(const TLawsuit *lawsuit, long flags)
{
	if (strchr(KIND_DANGER_LAWS, lawsuit->kind) && !lawsuit->danger && subject_flags_dangers(lawsuit->subject, flags))
		return ask("Не са попълнени данни за завишена обществена опасност. %s", Q_CONTINUE);

	return true;
}

long four_teen_year(const char *s)
{
	return atof(s) / 10000000000.0;
}

long four_teen_court(const char *s)
{
	return atof(s + 4) / 10000000.0;
}

long four_teen_type(const char *s)
{
	return atof(s + 7) / 100000.0;
}

long four_teen_no(const char *s)
{
	return atof(s + 9);
}

void update_prelawsuit_sides(TSideWind *sideWind, TLawsuit *lawsuit, const char involvement)
{
	msql m(sideWind->Name);

	m.AddLong("F_NO", lawsuit->source.no);
	m.AddLong("F_YEAR", lawsuit->source.year);
	m.AddChar("F_TYPE", TYPE_REQUEST);
	m.Add(sideWind, "F_UCN, F_UCN_TYPE, F_INVOLVEMENT");

	TQuery q(str(m));
	TSideWind source, target;
	TRequestResolution resolution;

	while (q.Read())
	{
	#if TESTVER  // 2017:111
		source << q;
		TRCDKeyContainer *const container = kind2RCDKeyContainer(source.kind);
		TVirtPtr VP(container);
		*container << source;
	#else  // TESTVER
		source << q;
		TRCDKeyContainer *container = kind2RCDKeyContainer(source.kind);
		*container << source;
	#endif  // TESTVER

		if (container->Get() && !blocked_sidewind(&source, container->Status(), true))
		{
			TGArray<TLinkWind> links;	// 2010:341 +related
			TLinkWind linkWind;

			linkWind << container;
			linkWind.LoadFloat(&links);
			target << source;
			target.involvement = involvement;
			update_sidewind(&source, &target, &links);
		}
	#if TESTVER
	#else  // TESTVER
		delete container;
	#endif  // TESTVER
	}
}

long flag_key_no(const TRCDKey *key)
{
	bool cut = false;

	if (COURT_N10K && strchr(TYPE_LAWSUITS, key->type))
	{
		switch (COURT_CODE)
		{
		#if COURT_TYPE == COURT_AREAL
			case COURT_SOFGS : cut = key->year <= 2008 && key->type != TYPE_TRADE_LAW; break;
		#endif  // COURT_AREAL
			// more courts and conditions here...
			default : cut = true;
		}
	}

	return cut ? key->no % 10000 : key->no;
}

void add_event_kinds(char type, mstr &kinds)
{
	switch (type)
	{
		case TYPE_REQUEST :
		{
			kinds.cat(KIND_REQUEST_RESOLUTIONS);
			kinds.cat(KIND_V_REQUESTS);
			break;
		}
		case TYPE_INREG : kinds.cat(KIND_VISIBLE_EVENTS); break;
		case TYPE_PROVE : kinds.cach(KIND_PROVE_ACTION); break;
		case TYPE_SURROUND :
		case TYPE_OUTREG : break;
		case TYPE_EMPTY : break;
	#if COURT_TYPE == COURT_REGIONAL
		// 2009:208 FIX: former build_sessk()
		case TYPE_CITIZEN_LAW: kinds.cach(KIND_126_LISTEN_SESSION);  // falldown
	#endif  // REGIONAL
		default :
		{
			if (strchr(TYPE_LAWSUITS, type))
			{
				kinds.cat(type2ses_kinds(type));
				kinds.cat(KIND_VISIBLE_CLOSEDS);
				kinds.cat(KIND_ENDOCS);			// 2013:232 // VISIBLE
				kinds.cat(KIND_V_ARCHIVES);
				kinds.cat(KIND_V_LAWSUITS);
			}
			else if (!strchr(COLLEGE_COMMONS, type))
				fatal("%d: type not supported by type event kinds", type);
		}
	}

	if (strchr(TYPE_MOVABLES, type))
		kinds.cat(KIND_MOVEMENTS);
	if (strchr(TYPE_SURROUNDABLES, type))
		kinds.cach(KIND_V_SURROUND);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(TYPE_PROVABLES, type))
		kinds.cach(KIND_PROVE);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	kinds.cach(KIND_V_EMPTY);
}

struct TAdjustSurroundKind
{
	const char *types;
	const char *surrLawKinds;
	const char *surrDecKinds;
};

// ----- KIND_ ---------------------------------------------------------------
static const TAdjustSurroundKind AdjustSurroundKinds[] =
{
	// 2010:194 +¤ for p; 2013:336 +`,
#if COURT_TYPE == COURT_APPEAL
	// 2017:048 +#&.;
	{ "p",	"т=>¤°ќё`,#&.;",		"?@ё,#" },
	{ "ctad",	"т=>¤°ё`,#&.;",		"?@^_|~ё,#" },
	{ "i",	"тё",				"ё" },		// 2011:018
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ "atc",	"=>¤°ё`,#&.;",		"?@^|ё,#" },	// 2017:048 +#&.;
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	{ "p",	"=>ќ¤°ё/‘`,",		"?@ё," },		// 2011:278 +/; 2018-03-19: +‘
	{ "ct",	":<=>¤°ёi$`,",		"?@^_|~ё," },	// 2010:176 +i; 2011:315 +$
	{ "a",	"=>¤°ё`,",			"?@^|ё," },
	{ "d",	"o€©ё`,",			"ё," },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	// 2017:048 +#&.;
	{ "p",	"ќё¤°/`,#&.;",		"ё,#" },		// 2012:153 +/
	{ "c",	"ё¤°`,#&.;",		"ё,#" },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	// 2017:047 +#&.; 2018-06-04: +ы +эю
	{ "p",	"=>ќё¤°/`,#&.;ыэю",	"ё,#ы" },		// 2011:278 +/
	// 2010:176 +i; 2013:178 +$; 2017:202 separated from m and +†
	{ "c",	":<=>¤°ёi$`,#&.;†ъыэюя","?@^_|~ё,#†ы" },	// 2018-06-04: +ъя
	{ "m",	":<=>¤°ёi$`,#&.;ыэю",	"?@^_|~ё,#ы" },	// 2010:176 +i; 2013:178 +$
#endif  // REGIONAL
	{ NULL, NULL, NULL }
};

const char *adjust_surround_kinds(const TRCDKeyContainer *container)
{
	// 2009:231 FIX: regression: +f
#if COURT_TYPE == COURT_AREAL
	if (strchr(TYPE_TRADEALIKE_LAWS, container->key.type) && !strchr(KIND_SESSIONS_ENDOCS, container->kind) &&
		strchr(KIND_INCONSISTENCE_INDOCS, ((TLawsuit *) container)->sourceKind))
	{
		return ":<=>¤°ёf$`›•";	// 2011:318 +$; 2013:336 +`; 2018-06-04: FIX: ›• were for regular t
	}
#endif  // AREAL
#if COURT_TYPE == COURT_REGIONAL
	// 2017:206 special support for „
	if (container->kind == KIND_EUHER_CERT)
		return "?@^_|~ё,#†„";
#endif  // REGIONAL

	for (const TAdjustSurroundKind *adjust = AdjustSurroundKinds; adjust->types; adjust++)
		if (strchr(adjust->types, container->key.type))
			return strchr(KIND_SESSIONS_ENDOCS, container->kind) ? adjust->surrDecKinds : adjust->surrLawKinds;
	// 2018-06-04 support Э and d/ь
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	static constant PCC_SUR = "ёd";
#endif  // AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	static constant PCC_SUR = "ёь";
#endif  // REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (container->kind == KIND_CALL_APPEAL && ((const TInReg *) container)->lawsuit.type == TYPE_CITIZEN_LAW)
		return "ёЭ";

	if (container->kind == KIND_PRIVATE_CALL_APPEAL && ((const TInReg *) container)->lawsuit.type == TYPE_CITIZEN_LAW)
		return PCC_SUR;
#endif  // AREAL || MILITARY || REGIONAL

	return KIND_ANOTHER_SURROUNDS;
}

// ----- SUBPOENA_KIND_ ------------------------------------------------------
static constant SubpoenaKindEquivalences[] =
{
	// 2010:166 +w; 2010:180 LPR/LRQ: -ЧШyw; 2014:067 +‡ except AdmC
#if COURT_TYPE == COURT_APPEAL
	"ЙЩ",
	"bjlавенђћџsјк",	// 2014:078 +ђћџsј; 2016:111 +к
	"бгдЎ",
	"жз",
	"ъю",
	"яї‡",
	"ГиЌ",		// 2014:078 +Ќ
	"ДйЋ",		// 2014:078 +Ћ
	"И¶",			// 2014:072
	"мо»П",		// 2015:187; 2016:054 +»П
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	"bjlаве9ћџј",	// 2014:048 +9; 2014:064 +ћџј
	"ЙЩ",
	"ЪЫЬ",
	"бгдЎ",
	"Г2",			// 2014:048
	"Д3",			// 2014:048
	"И¶",			// 2014:072
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	"ъью",
	"їя‡",
	"бгдЎ",
	"ДйЋ",		// 2014:064 +Ћ
	"жз",
	"ГиЌ",		// 2014:064 +Ќ
	"лм",
	"bljавенђћџsј",	// 2014:064 +ђћџsј
	"ЙЩ",
	"29Љ",		// 2011:039; 2012:062 +Љ
	"И¶",			// 2014:072
	"]{",			// 2017:307
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	"bjlнсфк",	// 2009:321 +к
	"Гд",
	"Де",
	"ЙЩ",
	"ъю",
	"яї‡",
	"29Љ",		// 2012:115 cp from area
	"птуЎ",		// 2013:234
	"И¶",			// 2014:072
	"]{",			// 2017:310
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	"bjlавенђћџsј",	// 2014:064 +ђћџsј
	"ГиЌ",		// 2014:064 +Ќ
	"ДйЋ",		// 2014:064 +Ћ
	"ЙЩUV",		// 2014:035 +UV
	"ЪЫЬ15G",		// 2014:035 +15G
	"ъю",
	"яї‡",
	"29Љ",		// 2011:039; 2012:062 +Љ
	"бЎ",			// 2013:234
	// 2014:035
	"Б0",
	"ИR¶§",		// 2014:072 +¶§
	"Кd",
	"Лf",
	"Нm",
	"]{",			// 2017:305
#endif  // REGIONAL
	{ NULL }
};

static char *adjust_whatever_kinds(char *kinds, constant equivalences[])
{
	for (constant *equ = equivalences; *equ; equ++)
	{
		const char *s;

		for (s = *equ; *s; s++)
			if (strchr(kinds, *s))
				break;

		if (*s)
			for (s = *equ; *s; s++)
				if (!strchr(kinds, *s))
					strcach(kinds, *s);
	}

	return kinds;
}

char *adjust_subpoena_kinds(char *subpoenaKinds)
{
	return adjust_whatever_kinds(subpoenaKinds, SubpoenaKindEquivalences);
}

struct TXferIndocKind
{
	long xferKind;
	long appealKind;
	char college;
	char indocKind;
	char lawKind;
};

// ----- KIND_ ---------------------------------------------------------------
static const TXferIndocKind XferIndocKinds[] =
{
#if COURT_TYPE == COURT_AREAL
	{ 8001, 6002, 'c', 'Ч', '?' },
	{ 8001, 6002, 't', 'Ч', '?' },
	{ 2001, 6001, 'p', '1', 'W' },	// XXX
	{ 2001, 6002, 'p', 'ў', '?' },
	{ 2001, 6003, 'p', '5', 'W' },	// XXX
	{ 2001, 6004, 'p', 'Є', '?' },
	{ 2002, 6001, 'p', '1', 'C' },	// XXX
	{ 2002, 6002, 'p', 'Ў', '?' },
//	{ 2002, 6003, ...
//	{ 2002, 6004, ...
	{ 2003, 6001, 'p', '1', 'П' },	// XXX
	{ 2003, 6002, 'p', 'й', '?' },
	{ 2003, 6003, 'p', '5', 'П' },	// XXX
	{ 2003, 6004, 'p', '“', '?' },
	{ 2003, 6005, 'p', 'ц', '?' },
	{ 2003, 6007, 'p', 'g', '?' },
	{ 2004, 6001, 'p', '1', 'H' },	// XXX
	{ 2004, 6002, 'p', '4', '?' },
	{ 2004, 6003, 'p', '5', 'H' },	// XXX
	{ 2004, 6004, 'p', '6', '?' },
	{ 2010, 6001, 'c', 'Ж', '?' },
	{ 2010, 6001, 't', 'Ж', '?' },
	{ 2010, 6002, 'c', 'Ч', '?' },
	{ 2010, 6002, 't', 'Ч', '?' },
	{ 2010, 6014, 'c', 'Ж', '?' },
	{ 2010, 6014, 't', 'Ж', '?' },
	{ 2011, 6001, 'c', 'Ж', '?' },
	{ 2011, 6001, 't', 'Ж', '?' },
	{ 2011, 6002, 'c', 'Ч', '?' },
	{ 2011, 6002, 't', 'Ч', '?' },
	{ 2011, 6014, 'c', 'Ж', '?' },
	{ 2011, 6014, 't', 'Ж', '?' },
	{ 2022, 6001, 'c', 'Ж', '?' },
	{ 2022, 6002, 'c', 'Ч', '?' },
	{ 2022, 6014, 'c', 'Ж', '?' },
#endif  // AREAL
	{ 0, 0, NULL },
};

void xfer_indoc_kind(TRequest *request, long xferKind, long appealKind, char college)
{
	if (xferKind > 8002 && xferKind <= 8999)
		xferKind = 8001;

	for (const TXferIndocKind *xf = XferIndocKinds; xf->xferKind; xf++)
	{
		if (xf->xferKind == xferKind && xf->appealKind == appealKind && xf->college == college)
		{
			if (xf->lawKind != '?')
				request->lawKind = xf->lawKind;

			request->kind = xf->indocKind;
			return;
		}
	}

	message("Вида на входящия документ не може да бъде определен. Необходимо е да изберете вид на входящия документ.");
}

#if TESTVER
void dump_xfer_indoc_kinds()
{
	FILE *f = fopen("c:\\tmp\\__" BASEVER, "w");

	for (const TXferIndocKind *xf = XferIndocKinds; xf->xferKind; xf++)
	{
		Kind->kind = xf->indocKind;

		if (Kind->Try())
			fprintf(f, "%c %s\n", xf->college, Kind->Seek(xf->indocKind));
		else
			error("kind %c is missing", xf->indocKind);
	}

	fclose(f);
}
#endif  // TESTVER

void adjust_cancelaw(TSession *session)
{
	// 2015:078 inlined find_canceler()
	if (strchr(RESULT_COMPLETIZEDS, session->result) && !strchr(RESULT_ALL_CANCELEDS, session->result))
	{
		TDecision decision;
		msql m(decision.Name, "F_RESULT");

		m.Add(session, "F_NO, F_YEAR, F_TYPE, F_DATE");
		m.AddChar("F_SESSION_KIND", session->kind);
		m.AddChars("F_RESULT", RESULT_CANCELAWS);

		TQuery q(str(m));
		if (q.Read())
		{
			decision << q;
			session->result = decision.result;
		}
	}
}

// ----- KIND_ ---------------------------------------------------------------
// 2011:178 FIX: made l,v leaders
constant GeneralKindEquivalences[] =
{
#if COURT_TYPE == COURT_APPEAL
	"l132)",
	"L5ВX/",
	"v4ц",
	"6V",
	"Qq",
	"8w",
	"jЭ",
	"mMҐФ",
	"Гy",
	"fґ",
	"Жж",
	"Чч",
	"7g",			// 2012:020
	"0h",			// 2012:020
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	"9P",
	"чщ",
	"eЁ",
	"AАwj",
	"шю",
	"hЕ",
	"kS{}",		// 2016:113 +{}
	"sЗ",
	"bК",
	"ьa",
	"xј",
	"їВ",
	"BC",
	"ЂЋuU" "HIJK",	// 2016:201
	"бЦ",
	"Єцgй",
	"эы",
	"єXЏџ",
	"Ґґ",			// 2012:079
	"€)",			// 2013:232
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	"Au",
	"hz",
	"xX",
	"bB",
	"kK",
	"l1Ж9PАі",
	"v4ЧшюўйЎ",
	"5L",
	"6VЄ“",
	"цбЦ",
	"эыg",
	"wБ",			// 2010:039 TRQ: -ђ
	"jJ",
	"Гг",			// 2010:039 TRQ: -Ђћ
	"mM",
	"Ђћ.",		// 2010:039; 2014:176 +.
	"sS",
	"чщ",
	"ьЁїф·",		// 2010:260 +•·
	"aЏ",
	"јџ",
	"\\",			// 2011:147; 2018-05-08: comout
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	"Au",
	"eE",
	"hz",
	"xX",
	"bB",
	"kK",
	"l1237АФ",
	"v40ўй",
	"гґ",
	"5LВт",
	"6VЄ“",
	"wБ",
	"ьЁф",
	"aЏ",
	"jјџЈJ",		// 2010:102 LPR/REQ: leading fake j
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	//"MC",		// 2010:208 (marriage) vs. (citizen)
	//"hЎў",		// 2010:106 +Ўў
	"Aб",
	"7Ћ",
	"aАц",
	"XЁф",
	"ЗU",
	"КY",
	"єҐ",
	"іy",
	"їz",
	"шщ",
#endif  // REGIONAL
	NULL
};

char *adjust_general_kinds(char *generalKinds)
{
	return adjust_whatever_kinds(generalKinds, GeneralKindEquivalences);
}

char kind2GeneralKind(char kind)
{
	for (constant *equ = GeneralKindEquivalences; *equ; equ++)
		if (strchr(*equ, kind))
			return **equ;

	return kind;
}

void add_postal_buttons(TDialog *dialog, TCheckBox *buttons[])
{
	TCheckBox *tButtons[IDC_POSTAL_INDEX_COUNT];

	if (!buttons)
		buttons = tButtons;

	buttons[IDC_POSTAL_INDEX_NONE] = new TRadioFace(dialog, IDC_GENERIC_SEND_NONE, "F_FLAGS_NONE");
	buttons[IDC_POSTAL_INDEX_REGULAR] = new TRadioFace(dialog, IDC_GENERIC_SEND_REGULAR, "F_FLAGS_REGULAR");
	buttons[IDC_POSTAL_INDEX_R] = new TRadioFace(dialog, IDC_GENERIC_SEND_R, "F_FLAGS_R");
	buttons[IDC_POSTAL_INDEX_AR] = new TRadioFace(dialog, IDC_GENERIC_SEND_AR, "F_FLAGS_AR");
	buttons[IDC_POSTAL_INDEX_PACKAGE] = new TRadioFace(dialog, IDC_GENERIC_SEND_PACKAGE, "F_FLAGS_PACKAGE");
	// 2009:161
	buttons[IDC_POSTAL_INDEX_COURIER] = new TCheckFace(dialog, IDC_GENERIC_SEND_COURIER, "F_FLAGS_COURIER");
	buttons[IDC_POSTAL_INDEX_FAX] = new TCheckFace(dialog, IDC_GENERIC_SEND_FAX, "F_FLAGS_FAX");
	buttons[IDC_POSTAL_INDEX_EMAIL] = new TCheckFace(dialog, IDC_GENERIC_SEND_EMAIL, "F_FLAGS_EMAIL");
}

void set_direction(const TRCDKey &lawKey, TStringFace *direction)
{
	// 2010:263 LPR: any -> empty, pre-check key fields
	if (!any(direction) && lawKey.no && lawKey.year && lawKey.type &&
		(strchr(TYPE_LAWSUITS, lawKey.type) || lawKey.type == TYPE_INREG))
	{
		// 2017:111 testver delete -> VirtPtr; 2017:137 official
		TRCDKeyContainer *const container = type2RCDKeyContainer(lawKey.type);
		TVirtPtr VP(container);
		TString *judge = (TString *) container->Find("F_JUDGE");

		container->key = lawKey;

		if (container->Try("F_JUDGE") && atob(judge->S()) && strcmp(judge->S(), INTER))
		{
			// 2017:111 testver s[] -> msql; 2017:137 permanent
			mstr m("На доклад на съдия-докл. ");

			print_user(m, judge->S());
			m.cut(SIZE_OF_DIRECTION);	// 2017:111 was missing
			direction->SetS(str(m));
		}
	}
}

const char *romanian_name(long composition)
{
	enum { ROMAN_COUNT = 9 };
	static constant roman[ROMAN_COUNT] = { "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
	static const limit[ROMAN_COUNT] = { 100, 90, 50, 40, 10, 9, 5, 4, 1 };

	static char name[SIZE_OF_ALIAS];
	int value = composition;

	*name = '\0';
	for (int i = 0; i < ROMAN_COUNT; i++)
	{
		while (value >= limit[i])
		{
			strcat(name, roman[i]);
			value -= limit[i];
		}
	}

	return name;
}

char kind2LawKind(char reqKind, char type)
{
	// 2017:087 based on kind2LawKinds
	mstr lawKinds;
	kind2LawKinds(reqKind, C2S[type], lawKinds);
	return *str(lawKinds);
}

void kind2LawKinds(char reqKind, const char *types, mstr &lawKinds)
{
	char kind;

	Kind->Seek(reqKind);
	lawKinds.clear();
	// Note: using *lk directly catches '\0'
	for (const char *lk = Kind->lawKinds; (kind = *lk) != '\0'; lk++)
		if (strchr(types, kind2type(kind)))
			lawKinds.cach(kind);

	if (!any(lawKinds))
		fatal("%d, %s: no suitable law kinds", reqKind, types);
}

long atobc(TPersonAliasFace *persons[], size_t count)
{
	long result = 0;

	for (int i = 0; i < count; i++)
	{
		TPersonAliasFace *person = persons[i];

		if (person && atob(person))
			result++;
	}

	return result;
}

// ----- TRequestSubject -----------------------------------------------------
struct TRequestSubject
{
	char kind;
	long subject;
};

// ----- SUBJECT_ ------------------------------------------------------------
static const TRequestSubject RequestSubjects[] =
{
#if COURT_TYPE == COURT_REGIONAL
	{ KIND_EUHER_REQUEST,	120400 },
#endif  // REGIONAL
	{ '\0', 0 }
};

long suggest_lawsuit_subject(char requestKind)
{
	for (const TRequestSubject *rs = RequestSubjects; rs->kind; rs++)
		if (rs->kind == requestKind)
			return rs->subject;
	return 0;
}

bool fix_incoming_type(TRCDKeyGroup *incoming, const char *types)
{
	if (incoming->key.type != TYPE_INCOMING)
		return true;

	TRequest request;
	TSurround surround;
	TInReg inReg;

	TRCDKeyContainer *containers[] = { &request, &surround, &inReg, NULL };
	TRCDKeyContainer *found = NULL;

	for (int i = 0; containers[i] != NULL; i++)
	{
		TRCDKeyContainer *container = containers[i];

		container->key.no = incoming->key.no;
		container->key.year = incoming->key.year;

		if (container->Try("F_TYPE"))
		{
			if (found)
			{
				return error("Намерени са два или повече документа %ld/%ld.\nУкажете точния вид.", incoming->key.no,
					incoming->key.year);
			}

			found = container;
		}
	}

	if (!found)
		return say_no_data("T_INCOMING");

	if (!strchr(types, found->key.type))
	{
		return error("%s %ld/%ld - Вида на документа не съответства на избраната функция.", Type->Seek(found->key.type),
			incoming->key.no, incoming->key.year);
	}

	incoming->key.type = found->key.type;
	return true;
}

char receiver_ucn_type(char ucnType)
{
	return ucnType == UCN_EXECUTOR_UCN ? UCN_CITIZEN_UCN : ucnType == UCN_EXECUTOR_CODE ? UCN_CITIZEN_CODE : ucnType;
}

// ----- TLawKind2ForTeenCode ------------------------------------------------
struct TLawKind2ForTeenCode
{
	const char *lawsuitKinds;
	const char *fourTeenCode;
};

static const TLawKind2ForTeenCode LawKind2ForTeenCodes[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ KIND_VZ_PUNISHMENT_LAWS, FOUR_TEEN_2ND_PUNISHMENT },
	{ KIND_VZ_CITIZEN_LAWS,    FOUR_TEEN_2ND_CITIZEN },
	{ KIND_TRADE_LAWS,         FOUR_TEEN_TRADE },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	//KIND_MARRIAGE_LAWS
	{ KIND_CITIZEN_LAWS,       FOUR_TEEN_1ST_CITIZEN },
	{ KIND_PUNISHMENT_LAWS,    FOUR_TEEN_1ST_PUNISHMENT },
	{ KIND_ADMIN_LAWS,         FOUR_TEEN_ADMINISTRATIVE },
#endif  // REGIONAL
	{ NULL, FOUR_TEEN_UNKNOWN },
};

const char *four_teen_code(char lawsuitKind)
{
	const TLawKind2ForTeenCode *lk2ft;

	for (lk2ft = LawKind2ForTeenCodes; lk2ft->lawsuitKinds; lk2ft++)
		if (strchr(lk2ft->lawsuitKinds, lawsuitKind))
			break;

	return lk2ft->fourTeenCode;
}
