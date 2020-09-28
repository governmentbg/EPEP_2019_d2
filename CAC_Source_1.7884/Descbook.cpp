#include "cac.h"

enum
{
	APPEALERS 			= 0,
	PROTESTS			= 1,
	APPEALERS_PROTESTS 	= 2,
	CRIMINAL_SIZE 		= 3,
	APP_RESULT_SIZE		= 4,
	RESULTS_C_SIZE 		= 6,
	RESULTS_P_SIZE		= 11
};

// ----- RESULT_ -------------------------------------------------------------
// ----- KIND_ ---------------------------------------------------------------
static constant
	// 2014:048 AppealResults[0] +À; 2016:025 +Ê where Ø; 2018-01-10: +z where G
#if COURT_TYPE == COURT_APPEAL
	// 2011:192 Ú -> v;
	ResultsC[RESULTS_C_SIZE] = { "Cv4", "Ew", "Gz", "¿¾Î", "H", "ØÊ" },					// 2011:192 +¿¾
	ResultsP[RESULTS_P_SIZE] = { "CÃv", "Ä", "È", "É", "Ë", "Ï", "Ó", "Ô", "Ö¯¿¾Î", "×", "ØÊ" },	// 2011:192 +¯¿¾
	AppealResults[APP_RESULT_SIZE] = { "NÀ", "O", "Q", "E" },
	CriminalKinds[CRIMINAL_SIZE] = { "2139ö4", "LX5ÂVqY", "qQF" };	// 2011:192 6 -> q
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	ResultsC[RESULTS_C_SIZE] = { "Cv", NULL, NULL, "¿¾", NULL, "ØÊ" },
	ResultsP[RESULTS_P_SIZE] = { "Cv", NULL, NULL, NULL, NULL, NULL, NULL, NULL, "¿¾", NULL, "ØÊ" },
	AppealResults[APP_RESULT_SIZE] = { "CÀ", "Fu", "Ì", "K" },
	CriminalKinds[CRIMINAL_SIZE] = { NULL, NULL, NULL };
#endif  // ADMIN
// 2011:193 MILITARY <- AREAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	// 2011:122 TRQ: +Ú for P, Appeal; 2011:152 Ú -> v
	// 2011:235 LPR: +w +Î where needed
	ResultsC[RESULTS_C_SIZE] = { "Cv", "Ew", "Gz", "¿¾Î", "H", "ØÊ" },					// 2011:152 +¿¾
	ResultsP[RESULTS_P_SIZE] = { "CÃv", "Ä", "È", "É", "Ë", "Ï", "Ó", "Ô", "Ö¯¿¾Î", "×", "ØÊ" },	// 2011:122 +¯¿¾
	AppealResults[APP_RESULT_SIZE] = { "CÀ", "Fu", "Ì", "K" },
	CriminalKinds[CRIMINAL_SIZE] = { "l1öv¢¡4gé", "L5Vª6“&", "–—" };
#endif  // AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	ResultsC[RESULTS_C_SIZE] = { NULL, NULL, NULL, NULL, NULL, NULL },
	ResultsP[RESULTS_P_SIZE] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
	AppealResults[APP_RESULT_SIZE] = { NULL, NULL, NULL, NULL },
	CriminalKinds[CRIMINAL_SIZE] = { NULL, NULL, NULL };
#endif  // REGIONAL

// ----- RECAP ---------------------------------------------------------------
static const
	SUBJECT		= 0x01,
	SUBJECT_NOT  	= 0x02,
	SUBJECT_NONE 	= 0x04,
	QUICK			= 0x08,
	QUICK_NOT		= 0x10,
	QUICK_NONE 		= 0x20,
	LAW_KIND		= 0x40,
	NOT_USE_FLAGS	= 0x80;

struct TAppealKind
{
	int index;
	const char *kinds;
	long subject_1;
	long subject_2;
	long flags;
};

// ----- KIND_ ---------------------------------------------------------------
// ----- SUBJECT_ ------------------------------------------------------------
#if COURT_TYPE == COURT_APPEAL
static const TAppealKind appealKinds[] =
{
	{ 1,  "ZÿTÍ",	0, 	 0, 	 LAW_KIND   		  },	// âúççèâíè
	{ 2,  "7ÆPÀV",	9050, 0, 	 QUICK_NOT  | SUBJECT_NOT },
	{ 3,  "7ÆPÀV",	9050, 0, 	 QUICK      | SUBJECT_NOT },
	{ 4,  "7ÆPÀVgæ",	9050, 0, 	 QUICK_NOT  | SUBJECT     },	// 2012:172 +gæ
	{ 5,  "Nßn½",	0, 	 0, 	 LAW_KIND   	 	  },	// ÷àñòíè
	{ 6,  "0×¨þVop",	9150, 0, 	 QUICK_NONE | SUBJECT_NOT },	// 2015:245 +op
	{ 7,  "jÝ",		9150, 0, 	 QUICK_NONE | SUBJECT_NOT },
	{ 8,  "m¥V",	9150, 0, 	 QUICK_NONE | SUBJECT_NOT },
	{ 9,  "8w",		9150, 0, 	 QUICK_NONE | SUBJECT_NOT },
	{ 10, "Ãy",		9150, 0, 	 QUICK_NONE | SUBJECT_NOT },
	{ 11, "f´", 	 9150, 0, 	 QUICK_NONE | SUBJECT_NOT },
	{ 12, "0×¨þVh÷", 	 9150, 0, 	 QUICK_NONE | SUBJECT	  },	// 2012:172 +h÷
	{  NULL, 	 	 0, 	 0, 	 NOT_USE_FLAGS 		  }
};

static const TAppealKind appealPunishmentKinds[]=
{
	{ 1,  "W",	 	 0, 	 0, 	 LAW_KIND   		  },	// âúççèâío ÍÎÕÄ
	{ 2,  "1V",	 	 0,    0, 	 NOT_USE_FLAGS 		  },
	{ 3,  "5V", 	 0, 	 0, 	 NOT_USE_FLAGS		  },
	{ 4,  "QV", 	 0, 	 0, 	 NOT_USE_FLAGS	        },
	{ 5,  "Lq2X",	 0, 	 0, 	 NOT_USE_FLAGS   	 	  },
	{ 6,  "H", 	 	 0, 	 0, 	 LAW_KIND 			  },  // âúççèâío ×ÍÄ
	{ 7,  "Ê",  	 9004, 0, 	 QUICK_NONE | SUBJECT_NOT },
	{ 8,  "", 	 	 9002, 9003, QUICK_NONE | SUBJECT 	  },
	{ 9,  "", 	 	 9001, 0, 	 QUICK_NONE | SUBJECT	  },
	{ 10, "", 	 	 9004, 0, 	 QUICK_NONE | SUBJECT	  },
	{  NULL, 	 	 0, 	 0, 	 NOT_USE_FLAGS 		  }
};
#endif  // APPEAL

// ----- TDescDecision -------------------------------------------------------
class TDescDecision : public TDecision
{
public:
	TDescDecision();
	static TGroup *Clone();

	bool operator<=(const TDescDecision &that) const;

	bool viewGrant;
	TGSArray<TOutReg> outRegs;

	void LoadOutRegs();
};

TDescDecision::TDescDecision()
	: TDecision(TDescDecision::Clone), viewGrant(false)
{
}

TGroup *TDescDecision::Clone()
{
	return new TDescDecision;
}

bool TDescDecision::operator<=(const TDescDecision &that) const
{
	return TRCDKeyContainer::operator<=(that);
}

void TDescDecision::LoadOutRegs()
{
	msql m;
	TInReg inReg;
	TOutReg outReg;

	inReg.lawsuit = key;
	inReg.FloatCriteria(m, "F_NO, F_YEAR");
	m.AddChar("F_DECISION_KIND", kind);
	m.AddDate("F_DECISION_DATE", date);

	TQuery q(str(m));
	while (q.Read())
	{
		inReg << q;
		outReg.lawsuit = key;
		outReg.FloatCriteria(m);
		m.AddLong("F_IN_NO", inReg.key.no);
		m.AddLong("F_IN_YEAR", inReg.key.year);
		outReg.LoadArray(outRegs, str(m));
	}

	outReg.lawsuit = key;
	outReg.FloatCriteria(m);
	m.AddChars("F_RECEIVER_TYPE", UCN_INSTANCE_DESCRIPTS);
	outReg.LoadArray(outRegs, str(m));
}

// ----- TDescSession --------------------------------------------------------
class TDescSession : public TSession
{
public:
	TDescSession();
	static TGroup *Clone();

	bool operator<=(const TDescSession &that) const { return date <= that.date; }

	int decRow;
	bool print;
	TGSArray<TDescDecision> decisions;

	bool LoadDecisions(const char *decisionFields);
};

TDescSession::TDescSession()
	: TSession(TDescSession::Clone), decRow(0), print(false)
{
}

TGroup *TDescSession::Clone()
{
	return new TDescSession;
}

bool TDescSession::LoadDecisions(const char *decisionFields)
{
	if (!final.Empty())
	{
		msql m("T_DECISION", decisionFields);

		m.Add(this, "F_NO, F_YEAR, F_TYPE, F_DATE");
		m.AddChar("F_SESSION_KIND", kind);
		TQuery q(str(m));

		while (q.Read())
		{
			TDescDecision *decision = new TDescDecision;

			*decision << q;
			decision->LoadOutRegs();

			if (!decisions.Count())
				decision->viewGrant = true;

			decisions.Add(decision);
		}
	}

	return decisions.Count();
}

// ----- TDescLawsuit --------------------------------------------------------
class TDescLawsuit : public TLawsuit
{
public:
	TDescLawsuit();

	TOutReg *outReg;
	TArchive archive;
	TDescDecision *decision;

	TGArray<TSideWind> lefts, rights;
	TGSArray<TDescSession> sessions;

	int instance;
	int allCount;
	int sesCount;
	int decCount;
	CDate sesDate;
	int finalizer;
	long eventText;	// 2012:082 +related

	void LoadSideWinds();
	void LoadSessions(const char *decisionFields);
	void SetData(int i, int j = 0);
	void SetOutReg(int i, int j, int r = 0);
	void SetMaxRows(TDescSession *session);
};

TDescLawsuit::TDescLawsuit()
	: allCount(0), sesCount(0), decCount(0), instance(0), finalizer(0), outReg(NULL), decision(NULL)
{
	Add(new TCheck("F_EVENT_TEXT", &eventText, TRUE));
}

void TDescLawsuit::SetOutReg(int i, int j, int r)
{
	if (sessions[i]->decisions[j]->outRegs.Count())
		outReg = sessions[i]->decisions[j]->outRegs[r];
}

void TDescLawsuit::SetData(int i, int j)
{
	if (sessions.Count())
	{
		if (j == 0)
		{
			sesCount = sessions[i]->decRow;

			if (sessions[i]->result == RESULT_PUBLIC_DECISION && sessions[i]->print)
				sesDate = sessions[i]->date;
			else
				sesDate = CDate(0, 0, 0);
		}

		if (sessions[i]->decisions.Count())
		{
			decCount = sessions[i]->decisions[j]->outRegs.Count();
			decision = sessions[i]->decisions[j];
			SetOutReg(i, j);
		}
		else
			decCount = 0;
	}
}

void TDescLawsuit::LoadSideWinds()
{
	TSideWind sideWind;

	sideWind.LoadSides(this, &lefts, &rights);
	archive << this;
	archive.Try("F_ARCHIVE_DATE, F_ARCHIVE_NO, F_ARCHIVE_YEAR, F_LINK_NO");
}

void TDescLawsuit::LoadSessions(const char *decisionFields)
{
	TDescSession session;
	TSession *finalSession = NULL;
	msql m;

	session << this;
	session.FloatCriteria(m, "F_KIND, F_DATE, F_TIME, F_RESULT, F_FINAL");
	m.AddRange("F_DATE", date, finished);  // 2017:157		 
	TQuery q(str(m));

	while (q.Read())
	{
		TDescSession *tSession = new TDescSession;

		*tSession << q;
		tSession->key = key;
		sessions.Add(tSession);

		if (!tSession->final.Empty() && (!finalSession || tSession->final < finalSession->final))
			finalSession = tSession;
	}

	if (finalSession)
	{
		TDescSession *tSession = new TDescSession;

		*tSession << finalSession;
		tSession->print = true;
		finalizer++;
		tSession->LoadDecisions(decisionFields);
		SetMaxRows(tSession);
		sessions.Insert(tSession, 0);
	}
}

void TDescLawsuit::SetMaxRows(TDescSession *session)
{
	session->decRow = session->decisions.Count();

	for (int i = 0; i < session->decisions.Count(); i++)
		if (session->decisions[i]->outRegs.Count() > 1)
			session->decRow += (session->decisions[i]->outRegs.Count() - 1);

	allCount += (session->decRow ? session->decRow : 1);
}

// ----- functions -----------------------------------------------------------
static void print_result(mstr &m, const char *results, char decResult)
{
	if (results && decResult)
		if (strchr(results, decResult))
			m.cpy("*");
}

static void print_app_result(mstr &m, const char *appealResults, char outRegResultKS)
{
	if (appealResults && outRegResultKS)
	{
		for (const char *s = appealResults; *s; s++)
		{
			AppealResolution->Seek(*s);

			if (strchr(AppealResolution->resolutions, outRegResultKS))
			{
				m.cpy("*");
				break;
			}
		}
	}
}

static void desc_name(mstr &m, TSideWind *sideWind, bool printInvolvement, const char *name)
{
	if (any(m))
		m.cat("\v");

	if (printInvolvement)
		m.printf("%s\v", Involvement->Seek(sideWind->involvement));

	if (sideWind->state == STATE_HOLD)
		m.cat("* ");

	if (sideWind->flags & SIDE_WIND_CHILD)
		m.cat("~ ");

	m.cat(name);
}

static void desc_name(mstr &m, TSideWind *sideWind, bool shortRename, bool printInvolvement)
{
	if (strchr(UCN_CITIZENS, sideWind->ucnType))
	{
		TCitizen citizen;
		citizen << sideWind;

		if (citizen.Try())
		{
			desc_name(m, sideWind, printInvolvement, citizen.name);

			if (any(citizen.reName))
			{
				if (shortRename)
				{
					if (strlen(citizen.reName) <= 2)
						m.printf(" %s", citizen.reName);
					else
						m.printf(" %c.", *citizen.reName);
				}
				else
					m.printf(" %s", citizen.reName);
			}

			m.printf(" %s", citizen.family);

			if (any(citizen.reFamily))
				m.printf("-%s", citizen.reFamily);
		}
		else
			print_missing(m, sideWind->ucnType, sideWind->ucn);
	}
	else
	{
		TFirm firm;
		firm << sideWind;

		if (firm.Try())
			desc_name(m, sideWind, printInvolvement, firm.name);
		else
			print_missing(m, sideWind->ucnType, sideWind->ucn);
	}
}

static void desc_name(mstr &m, const char *ucn, char ucnType, bool shortRename, bool printInvolvement)
{
	TSideWind sideWind;

	strcpy(sideWind.ucn, ucn);
	sideWind.ucnType = ucnType;
	desc_name(m, &sideWind, shortRename, printInvolvement);
}

static void desc_side_list(mstr &m, TGArray<TSideWind> *sideWinds, bool printInvolvement = false)
{
	for (int i = 0; i < sideWinds->Count(); i++)
		desc_name(m, (*sideWinds)[i], false, printInvolvement);
}

#define descLaw OFFICE_GROUP_CAST(TDescLawsuit)

static void desc_book_subcount_1(mstr &m, TGroup *group)
{
	if (descLaw->allCount && descLaw->allCount != 1)
		m.printf("rowspan=%d", descLaw->allCount);
}

static void desc_book_subcount_2(mstr &m, TGroup *group)
{
	if (descLaw->sesCount && descLaw->sesCount != 1)
		m.printf("rowspan=%d", descLaw->sesCount);
}

static void desc_book_subcount_3(mstr &m, TGroup *group)
{
	if (descLaw->decCount && descLaw->decCount != 1)
		m.printf("rowspan=%d", descLaw->decCount);
}

static void desc_book_law_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(descLaw->kind)); }
static void desc_book_law_composition(mstr &m, TGroup *group) { print_composition(m, descLaw->composition, "ñ-â"); }

static void desc_book_indoc_description(mstr &m, TGroup *group)
{
	TRequest request;
	request.key = descLaw->source;

	Subject->Seek(descLaw->key.type, descLaw->subject);

	if (strchr(Subject->Attrib(descLaw->kind), POST_PRECOURT))
		m.cpy("Ä ");

	if (request.Try("F_KIND, F_NO, F_DATE"))
		m.printf("%s âõ ¹%d/%D", Kind->Seek(request.kind), request.key.no, &request.date);

	print_connects(m, descLaw->key, true, false, "ñ-â");
}

static void desc_book_indoc_1(mstr &m, TGroup *group)
{
	if (CriminalKinds[APPEALERS] && any(CriminalKinds[APPEALERS]))
	{
		if (strchr(CriminalKinds[APPEALERS], descLaw->sourceKind)
		#if COURT_TYPE == COURT_MILITARY
			&& CourtType == COURT_MILITARY_APPEAL
		#endif  // MILITARY
		)
			m.cpy("*");
		else if (any(CriminalKinds[APPEALERS_PROTESTS]) && strchr(CriminalKinds[APPEALERS_PROTESTS], descLaw->sourceKind))
			m.cpy("*");
	}
}

static void desc_book_indoc_2(mstr &m, TGroup *group)
{
	if (CriminalKinds[PROTESTS] && any(CriminalKinds[PROTESTS]))
	{
		if (strchr(CriminalKinds[PROTESTS], descLaw->sourceKind)
		#if COURT_TYPE == COURT_MILITARY
			&& CourtType == COURT_MILITARY_APPEAL
		#endif  // MILITARY
		)
			m.cpy("*");
		else if (any(CriminalKinds[APPEALERS_PROTESTS]) && strchr(CriminalKinds[APPEALERS_PROTESTS], descLaw->sourceKind))
			m.cpy("*");
	}
}

static void desc_book_1inst_connect(mstr &m, TGroup *group)
{
	TConnect connect;

#if COURT_TYPE == COURT_APPEAL
	if (connect.TryBy(descLaw, CONNECT_TYPE_RECALL_LAWSUIT))
	{
		connect.composition = '\0';
		connect.Print(m, true, false, "ñ-â");
	}
#endif  // APPEAL

	if (connect.TryBy(descLaw, CONNECT_TYPE_LOWER_INSTANCE))
	{
		m.sep("\v");
		connect.composition = '\0';
		connect.Print(m, true, false, "ñ-â");
	}
}

static void desc_book_judges(mstr &m, TGroup *group)
{
	constant judges[] = { descLaw->rejudge, descLaw->judge };

	for (int i = 0; i < 2; i++)
	{
		if (atob(judges[i]))
			desc_name(m, judges[i], UCN_CITIZEN_UCN, true, false);

		if (!strcmp(judges[0], judges[1]))
			break;
	}
}

static void desc_book_subject_name(mstr &m, TGroup *group) { m.cpy(Subject->Seek(descLaw->key.type, descLaw->subject)); }
static void desc_book_text(mstr &m, TGroup *group) { m.cpy(descLaw->text); }

static void desc_book_damn(mstr &m, TGroup *group) 
{
	TDamn damn;
	
	msql p;
	p.Begin(damn.Name, "F_NAME, F_COMPANY_TYPE");
	p.AddLong("F_NO", descLaw->key.no);	
	p.AddLong("F_YEAR", descLaw->key.year);
	p.AddChar("F_TYPE", descLaw->key.type);
	TQuery q(str(p));	

	while (q.Read()) 
	{
		damn << q;	
		m.printf("%s %s", damn.name, CompanyType->Seek(damn.companyType));	
	}	
}

static void desc_book_subject(mstr &m, TGroup *group) { m.cat(descLaw->Code()); }

static void desc_book_sides(mstr &m, TGroup *group)
{
	mstr defendant;

	desc_side_list(m, &descLaw->lefts, !descLaw->rights.Count());
	desc_side_list(defendant, &descLaw->rights, !descLaw->lefts.Count());

	if (any(m) && any(defendant))
		m.printf("\vñðåùó\v%s", str(defendant));
	else if (any(defendant))
		m.cpy(defendant);
}

static void desc_book_suitors(mstr &m, TGroup *group)
{
	desc_side_list(m, &descLaw->lefts);
}

static void desc_book_defendants(mstr &m, TGroup *group)
{
	desc_side_list(m, &descLaw->rights);
}

static void desc_book_sessions(mstr &m, TGroup *group)
{
	TDescSession session;

	for (int i = descLaw->finalizer == 1 ? 1 : 0; i < descLaw->sessions.Count(); i++)
	{
		session << descLaw->sessions[i];

		if (strchr(KIND_OPEN_SESSIONS, session.kind))
			m.printf("%D, %T\v", &session.date, &session.time);
	}
}

static void desc_book_session_date(mstr &m, TGroup *group) { descLaw->sesDate.Print(m); }

static void desc_book_decision_date(mstr &m, TGroup *group)
{
	if (descLaw->decision)
		descLaw->decision->fromDate.Print(m);
}

static void desc_book_result(mstr &m, TGroup *group)
{
	TDescDecision *decision = descLaw->decision;

	if (decision)
	{
		m.cat(Kind->Seek(decision->kind));

		if (PRINT_JUDGEMENTS)
			m.printf(" ¹%d", decision->eventNo);

		m.printf(" îò %D\v", &decision->fromDate);

		if (!strchr(RESULT_WITH_READY_ANYS, decision->result))
			m.cat(Result->Seek(decision->result));
		else if (decision->viewGrant)
			m.cat(Grant->Seek(descLaw->grant));

		if (any(decision->text))
		{
			m.cat("\v");
			m.cat(decision->text);
		}

		// 2012:081
		if (descLaw->eventText && decision->PersonalData())
		{
			TJudgement judgement;
			msql w;

			judgement << decision;
			judgement.FloatCriteria(w, "F_UCN, F_UCN_TYPE, F_TEXT");
			w.CutOrder();
			w.AddIsNull("F_TEXT", false);
			w.AddOrder("F_ORDER");

			TQuery q(str(w));
			while (q.Read())
			{
				judgement << q;
				m.cat("\v\v");
				judgement.PrintNames(m);
				m.printf("\v%s", str(judgement.text));
			}
		}
	}
}

static void desc_book_result_c_value(mstr &m, TGroup *group)
{
	unsigned col;

	if (descLaw->decision && sscanf(descLaw->Name, "F_RESULT_C%d", &col) == 1 && col <= RESULTS_C_SIZE)
		print_result(m, ResultsC[col - 1], descLaw->decision->result);
}

static void desc_book_result_p_value(mstr &m, TGroup *group)
{
	unsigned col;

	if (descLaw->decision && sscanf(descLaw->Name, "F_RESULT_P%d", &col) == 1 && col <= RESULTS_P_SIZE)
		print_result(m, ResultsP[col - 1], descLaw->decision->result);
}

static void desc_book_app_result_value(mstr &m, TGroup *group)
{
	unsigned col;

	//if (sscanf(descLaw->Name, "F_APP_RESULT_%d", &col) == 1 && col <= APP_RESULT_SIZE)
	//	print_app_result(m, AppealResults[col - 1], descLaw->decision.result);
	// 2011:122 LPR: FIX: decision.result was probably copied from print_result() above
	if (descLaw->outReg && sscanf(descLaw->Name, "F_APP_RESULT_%d", &col) == 1 && col <= APP_RESULT_SIZE)
		print_app_result(m, AppealResults[col - 1], descLaw->outReg->resultKS);
}

static void desc_book_appeal(mstr &m, TGroup *group)
{
	if (descLaw->outReg)
	{
		m.printf("Èçõ. ¹%d/%D ", descLaw->outReg->key.no, &descLaw->outReg->date);

		TSender sender;
		if (get_receiver(descLaw->outReg, &sender))
			m.printf("\v%s", sender.name);
	}
}

static void desc_book_appeal_return(mstr &m, TGroup *group)
{
	if (descLaw->outReg && strchr(RESULT_APPEAL_RETURNEDS, descLaw->outReg->result))
		descLaw->outReg->returned.Print(m, "Âúðíàòî íà %D");
}

static void desc_book_appeal_result(mstr &m, TGroup *group)
{
	if (descLaw->outReg)
	{
		m.printf("%s", Resolution->Seek(descLaw->outReg->resultI));

		if (descLaw->outReg->resultKS != RESOLUTION_FAKE)
			m.printf("\v%s", Resolution->Seek(descLaw->outReg->resultKS));

		if (any(descLaw->outReg->vssIndex))
			m.printf("\vÈíäåêñ %s", descLaw->outReg->vssIndex);
	}
}

static void desc_book_appeal_return_date(mstr &m, TGroup *group)
{
	if (descLaw->outReg && strchr(RESULT_IOREG_RETURNEDS, descLaw->outReg->result))
		descLaw->outReg->returned.Print(m);
}

static void desc_book_age_1m(mstr &m, TGroup *group)
{
	if (descLaw->age == AGE_ONE_OR_LESS)
		m.cpy("*");
}

static void desc_book_age_3m(mstr &m, TGroup *group)
{
	if (descLaw->age == AGE_ONE_TO_THREE)
		m.cpy("*");
}

static void desc_book_age_6m(mstr &m, TGroup *group)
{
	if (descLaw->age == AGE_THREE_TO_SIX)
		m.cpy("*");
}

static void desc_book_age_1y(mstr &m, TGroup *group)
{
	if (descLaw->age == AGE_SIX_TO_TWELVE)
		m.cpy("*");
}

static void desc_book_age_up1y(mstr &m, TGroup *group)
{
	if (descLaw->age == AGE_MORE_THAN_TWELVE)
		m.cpy("*");
}

static void desc_book_register(mstr &m, TGroup *group)
{
	TDecision *decision = descLaw->decision;

	if (decision && decision->volume)
	{
#if COURT_TYPE == COURT_AREAL
		if (!strchr(RESULT_FIRM_ACCEPTS, decision->result))
#endif  // AREAL
		{
			m.printf("Ðåã. ¹ %d/%D, Òîì %d, Ñòð. %d", decision->partition, &decision->returned, decision->volume,
				decision->page);
		}
	}
}

static void desc_book_archive_date(mstr &m, TGroup *group) { descLaw->archive.archiveDate.Print(m); }

static void desc_book_archive_no(mstr &m, TGroup *group)
{
	if (descLaw->archive.archiveNo)
		m.printf("%d/%d", descLaw->archive.archiveNo, descLaw->archive.archiveYear);
}

static void desc_book_archive_link_no(mstr &m, TGroup *group) { itom(descLaw->archive.linkNo, m); }

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
static void desc_book_lawsuit_return(mstr &m, TGroup *group)
{
	TOutReg outReg;
	msql w;

	outReg.lawsuit = descLaw->key;
	outReg.FloatCriteria(w, "F_NO, F_DATE");
	w.AddChars("F_RECEIVER_TYPE", UCN_RETURN_LOWERS);

	TQuery q(str(w));
	if (q.Read())
	{
		outReg << q;
		m.printf("Èçõ ¹%d/%D", outReg.key.no, &outReg.date);
	}
}
#endif  // APPEAL || AREAL || ADMIN || MILITARY

#undef lawsuit

// ----- TDescSubject --------------------------------------------------------
class TDescSubject : public TSubject
{
public:
	TDescSubject() : count(0) { }

	long count;
};

// ----- TDescKind -----------------------------------------------------------
class TDescKind : public TGroup
{
public:
	TDescKind();
	static TGroup *Clone();

	char type;
	long count;
	int subjectIndex;
	int appealKinds[13];
	TGArray<TDescSubject> subjects;
	// 2015:307
	char name[SIZE_OF_NAME + 10];
	char kind;
};

TDescKind::TDescKind()
	: TGroup("T_KIND", TDescKind::Clone), count(0), subjectIndex(0)
{
	Add(
		new TChar("F_KIND", &kind),
		new TString("F_NAME", name, SIZE_OF_NAME),
		NULL
	);
}

TGroup *TDescKind::Clone()
{
	return new TDescKind;
}

#define descKind OFFICE_GROUP_CAST(TDescKind)

static void reCap_line(mstr &m, TGroup *group)
{
	if (descKind->subjectIndex)
	{
		if (descKind->subjectIndex - 1)
			m.cpy(descKind->subjects[descKind->subjectIndex - 1]->name);
		else
			m.printf("Â ò.÷. %s", descKind->subjects[descKind->subjectIndex - 1]->name);
	}
	else
		m.cpy(descKind->name);
}

static void reCap_count(mstr &m, TGroup *group)
{
	if (descKind->subjectIndex)
		m.printf("%d", descKind->subjects[descKind->subjectIndex - 1]->count);
	else
		m.printf("%d", descKind->count);
}

static void reCap_value(mstr &m, TGroup *group)
{
	int col;

	if (sscanf(descKind->Name, descKind->type == TYPE_PUNISHMENT_LAW ? "F_COUNT_P%d" : "F_COUNT_%d", &col) == 1 && col <= 14)
		m.printf("%d", descKind->appealKinds[col - 1]);
}

#undef descKind

static const offield reCap_fields[]=
{
	{ "F_LINE", reCap_line },
	{ "F_COUNT", reCap_count },
	{ "*F_COUNT_", reCap_value },
	{ NULL, NULL }
};

// ----- TDescBookGroup ------------------------------------------------------
class TDescBookGroup : public TBookGroup
{
public:
	TDescBookGroup();

	virtual void FormCriteria(msql &m);
	void CreateName(char *name);
	void ReCapName(char *name);

	long instance;
	long createBook;
	long reCap;
	long newFormat;
	long byAdminFormat;
	long byCitizenFormat;
	long byUnitedFormat;
	long instance_1st;
	long instance_2nd;
	char compositions[SIZE_OF_COMPOSITIONS];
	long eventText;

	// recap
	void LoadKinds();
	TGArray<TDescKind> kinds;
	TGArray<TDescSubject> subjects;

	void CountReCap(TDescLawsuit *lawsuit);
#if COURT_TYPE == COURT_APPEAL
	void SetAppealKindCount(char type, char kind, char sourceKind, int subject, long quick);
#else  // APPEAL
	void SetKindCount(char kind, int subject);
#endif  // APPEAL
};

TDescBookGroup::TDescBookGroup()
	: TBookGroup("T_LAW_INTERVAL I JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE",
		"I.F_TYPE", "I.F_DATE", true, true, "I.F_NO", "I.F_YEAR"), instance(0)
{
	Add(
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCheck("F_INSTANCE_1ST", &instance_1st, TRUE),
		new TCheck("F_INSTANCE_2ND", &instance_2nd, TRUE),
		new TCheck("F_CREATE_BOOK", &createBook, TRUE),
		new TCheck("F_RECAP", &reCap, TRUE),
		new TCheck("F_NEW_FORMAT", &newFormat, TRUE),
		new TRadio("F_ADMIN_LAW", &byAdminFormat, TRUE),
		new TRadio("F_CITIZEN_LAW", &byCitizenFormat, TRUE),
		new TRadio("F_CITIZEN_LAW_UNITED", &byUnitedFormat, TRUE),
		new TCheck("F_EVENT_TEXT", &eventText, TRUE),
		NULL
	);

	fields = "L.*, I.*";
}

void TDescBookGroup::FormCriteria(msql &m)
{
	TBookGroup::FormCriteria(m);

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	m.AddChars("L.F_KIND", instance == 3 ? KIND_LAWSUITS : instance == 1 ? KIND_1ST_LAWSUITS : KIND_2ND_LAWSUITS,
		KIND_LAWSUITS);
#endif  // ADMIN || AREAL || MILITARY
	m.AddCompos("L.F_COMPOSITION", compositions);
	// 2017:177 IRQ: first intervals only when by no/year
	if (by == BOOK_BY_PERIOD)
		m.AddIsNull("I.F_FLAGS", true);
	m.AddOrder("F_TYPE, F_YEAR, F_NO, F_DATE", "I.");
}

void TDescBookGroup::CreateName(char *name)
{
	sprintf(name, "InveB_f%d%c", instance, *types == TYPE_PUNISHMENT_LAW ? TYPE_PUNISHMENT_LAW : '_');

#if COURT_TYPE == COURT_ADMIN
	if (*types == TYPE_ADMIN_LAW)
		if (!byAdminFormat && !byCitizenFormat || byAdminFormat)
			strcpy(strrchr(name, 'f'), "fia");
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
	if (*types == TYPE_ADMIN_LAW)
		strcpy(strrchr(name, 'f'), "fia");
#endif  // AREAL

	strcat(name, "_.htm");

#if COURT_TYPE == COURT_ADMIN
	if (*types == TYPE_ADMIN_LAW && byUnitedFormat)
		strcpy(strrchr(name, 'f'), "fiad.htm");
#endif  // ADMIN
}

void TDescBookGroup::ReCapName(char *name)
{
	strcpy(name, "InveR_f");	// 2012:050 LPR/LRQ -"c"
#if COURT_TYPE == COURT_APPEAL
	strcat(name, *types == TYPE_PUNISHMENT_LAW ? "_pp" : "__p");
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	strcat(name, instance == 2 ? "2__" : "___");
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	strcat(name, "___");
#endif  // REGIONAL
	strcat(name, ".htm");
}

void TDescBookGroup::LoadKinds()
{
	// recap
	TDescKind *descKind = new TDescKind;
	strcpy(descKind->name, "Âñè÷êî äåëà");
	kinds.Add(descKind);

#if COURT_TYPE == COURT_APPEAL
	kinds[0]->type = types[0];
	kinds[0]->appealKinds[0] = FALSE;

	if (types[0] == TYPE_PUNISHMENT_LAW)
	{
		for (const TAppealKind *aKind = appealPunishmentKinds; aKind->kinds; aKind++)
			kinds[0]->appealKinds[aKind->index] = FALSE;
	}
	else
	{
		for (const TAppealKind *aKind = appealKinds; aKind->kinds; aKind++)
			kinds[0]->appealKinds[aKind->index] = FALSE;
	}
#else  // APPEAL
	msql m(Subject->Name);

	m.AddChar("F_TYPE", types[0]);
	m.AddLike("(F_ATTRIB_I", C2S[POST_DESCB_SUMM], true);
	m.NextOr();
	m.AddLike("F_ATTRIB_II", C2S[POST_DESCB_SUMM], true);
	m.cat(")");
	m.AddOrder("F_SUBJECT");

	TQuery r(str(m));
	while (r.Read())
	{
		TDescSubject *subject = new TDescSubject;
		*subject << r;
		subjects.Add(subject);
	}

	m.Begin("T_KIND", "F_KIND, F_NAME");
	m.AddChar("F_TYPE", types[0]);
	m.AddOrder("F_ORDER");

	TQuery q(str(m));
	while (q.Read())
	{
		TDescKind *descKind = new TDescKind;
		*descKind << q;

		for (int i = 0; i < subjects.Count(); i++)
		{
			if (subjects[i]->Kind(descKind->kind) == descKind->kind)
			{
				TDescSubject *tDescSubject = new TDescSubject;
				*tDescSubject << subjects[i];
				descKind->subjects.Add(tDescSubject);
			}
		}

		if (kinds.Count() == 1)
		{
			strmove(descKind->name + 7, descKind->name);
			memcpy(descKind->name, "Â ò.÷. ", 7);
		}

		kinds.Add(descKind);
	}
#endif  // APPEAL
}

void TDescBookGroup::CountReCap(TDescLawsuit *lawsuit)
{
	if (lawsuit->finalizer)
	{
		for (int i = 0; i < lawsuit->sessions.Count(); i++)
		{
			if (lawsuit->sessions[i]->print)
			{
			#if COURT_TYPE == COURT_APPEAL
				kinds[0]->appealKinds[0]++;
				SetAppealKindCount(lawsuit->key.type, lawsuit->kind, lawsuit->sourceKind, lawsuit->subject,
					lawsuit->quick);
			#else  // APPEAL
				kinds[0]->count++;
				SetKindCount(lawsuit->kind, lawsuit->subject);
			#endif  // APPEAL
			}
		}
	}
	else
	{
	#if COURT_TYPE == COURT_APPEAL
		kinds[0]->appealKinds[0]++;
		SetAppealKindCount(lawsuit->key.type, lawsuit->kind, lawsuit->sourceKind, lawsuit->subject, lawsuit->quick);
	#else  // APPEAL
		kinds[0]->count++;
		SetKindCount(lawsuit->kind, lawsuit->subject);
	#endif  // APPEAL
	}
}

#if COURT_TYPE == COURT_APPEAL
enum { P2 = 1, P3, P4, P5, P6, P7, P8, P9, P10, P11 };
// ----- KIND_ ---------------------------------------------------------------
void TDescBookGroup::SetAppealKindCount(char type, char kind, char sourceKind, int subject, long quick)
{
	if (type == TYPE_PUNISHMENT_LAW)
	{
		int *count = kinds[0]->appealKinds;

		if (kind == KIND_2ND_COMMON_PUNISHMENT_LAW)
		{
			count[P2]++;

			if (strchr("1l", sourceKind))
				count[P3]++;
			else if (strchr("5L", sourceKind))
				count[P4]++;
			else if (strchr("Qq", sourceKind))
				count[P5]++;
			else
				count[P6]++;
		}
		else if (kind == KIND_2ND_PRIVATE_PUNISHMENT_LAW)
		{
			count[P7]++;

			if (sourceKind == KIND_CANCEL_ENACTMENT && subject != 9004)
				count[P8]++;
			else if (subject == 9002 || subject == 9003)
				count[P9]++;
			else if (subject == 9001)
				count[P10]++;
			else
				count[P11]++;
		}
	}
	else
	{
		for (const TAppealKind *aKind = type == TYPE_PUNISHMENT_LAW ? appealPunishmentKinds : appealKinds; aKind->kinds;
			aKind++)
		{
			bool count = false;

			if (aKind->flags == LAW_KIND)
				count = strchr(aKind->kinds, kind);
			else if (aKind->flags == NOT_USE_FLAGS)
				count = strchr(aKind->kinds, sourceKind);
			else
			{
				if (strchr(aKind->kinds, sourceKind))
				{
					if ((aKind->flags & SUBJECT) == SUBJECT)
						count = aKind->subject_1 == subject || aKind->subject_2 == subject;
					else if ((aKind->flags & SUBJECT_NOT) == SUBJECT_NOT)
						count = aKind->subject_1 != subject || aKind->subject_2 == subject;
					else
						count = ((aKind->flags & SUBJECT_NONE) == SUBJECT_NONE);

					if (count)
					{
						if ((aKind->flags & QUICK) == QUICK)
							count = ((quick & QUICK_QUICK) == QUICK_QUICK);
						else if ((aKind->flags & QUICK_NOT) == QUICK_NOT)
							count = ((quick & QUICK_QUICK) != QUICK_QUICK);
						else
							count = ((aKind->flags & QUICK_NONE) == QUICK_NONE);
					}
				}
			}

			if (count)
				kinds[0]->appealKinds[aKind->index]++;
		}
	}
}
#else  // APPEAL
void TDescBookGroup::SetKindCount(char kind, int subject)
{
	for (int i = 1; i < kinds.Count(); i++)
	{
		if (kinds[i]->kind == kind)
		{
			for (int j = 0; j < kinds[i]->subjects.Count(); j++)
			{
				if (kinds[i]->subjects[j]->subject == subject)
				{
					kinds[i]->subjects[j]->count++;
					break;
				}
			}

		    kinds[i]->count++;
		    break;
		}
	}
}
#endif  // APPEAL

#define descGroup OFFICE_GROUP_CAST(TDescBookGroup)

static void desc_book_instance(mstr &m, TGroup *group)
{
	if (descGroup->instance != BOOK_BOTH_INST)
		m.cat(descGroup->instance == BOOK_1ST_INST ? "ÏÚÐÂÎÈÍÑÒÀÍÖÈÎÍÍÈ" : "ÂÚÇÇÈÂÍÈ/ÊÀÑÀÖÈÎÍÍÈ");
}

static void desc_book_type(mstr &m, TGroup *group)
{
	char types[2];	// required?

	types[0] = descGroup->types[0];
	types[1] = '\0';
	print_types_list(m, types, false);
	m.upper();
}

#undef descGroup

static const offield title_fields[] =
{
	{ "F_INSTANCE", desc_book_instance },
	{ "F_TYPE", desc_book_type },
	{ NULL, NULL }
};

static const offield desc_fields[]=
{
	{ "F_LAWSUIT_NO", keygroup_flag_no },
	{ "F_QUICK_SHORT", origin_quick_short },
	{ "F_KIND", desc_book_law_kind },
	{ "F_COMPOSITION", desc_book_law_composition },
	{ "F_INDOC_DESCRIPTION", desc_book_indoc_description },
	{ "F_SUBJECT_NAME", desc_book_subject_name },
	{ "F_TEXT", desc_book_text },
	{ "F_DAMN", desc_book_damn },
	{ "F_SUBJECT", desc_book_subject },
	//
	{ "F_SUBCOUNT_1", desc_book_subcount_1 },
	{ "F_SUBCOUNT_2", desc_book_subcount_2 },
	{ "F_SUBCOUNT_3", desc_book_subcount_3 },
	{ "F_INDOC_1", desc_book_indoc_1 },
	{ "F_INDOC_2", desc_book_indoc_2 },
	{ "F_1INST_CONNECT", desc_book_1inst_connect },
	{ "F_LAWSUIT_DATE", container_date },
	{ "F_JUDGES", desc_book_judges },
	{ "F_SIDES", desc_book_sides },
	{ "F_SUITORS", desc_book_suitors },
	{ "F_DEFENDANTS", desc_book_defendants },
	{ "F_SESSIONS", desc_book_sessions },
	{ "F_SESSION_DATE", desc_book_session_date },
	{ "F_DECISION_DATE", desc_book_decision_date },
	{ "F_RESULT", desc_book_result },
	{ "*F_RESULT_C", desc_book_result_c_value },
	{ "*F_RESULT_P", desc_book_result_p_value },
	{ "*F_APP_RESULT_", desc_book_app_result_value },
	{ "F_APPEAL", desc_book_appeal },
	{ "F_APPEAL_RETURN", desc_book_appeal_return },
	{ "F_APPEAL_RESULT", desc_book_appeal_result },
	{ "F_APPEAL_RETURN_DATE", desc_book_appeal_return_date },
	{ "F_AGE_1M", desc_book_age_1m },
	{ "F_AGE_3M", desc_book_age_3m },
	{ "F_AGE_6M", desc_book_age_6m },
	{ "F_AGE_1Y", desc_book_age_1y },
	{ "F_AGE_UP1Y", desc_book_age_up1y },
	{ "F_REGISTER", desc_book_register },	// 2011:264 LRQ: not used
	{ "F_ARCHIVE_DATE", desc_book_archive_date },
	{ "F_ARCHIVE_NO", desc_book_archive_no },
	{ "F_ARCHIVE_LINK_NO", desc_book_archive_link_no },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
	{ "F_LAWSUIT_RETURN", desc_book_lawsuit_return },
#endif  // APPEAL || AREAL || ADMIN || MILITARY
	{ NULL, NULL }
};

class TDescBook : public TBook
{
public:
	TDescBook(TWindow *parent, TDescBookGroup *group);

protected:
	virtual TGroup *Process(TQuery &q);
	virtual void Markers();
	virtual void EmitRow(TGroup *group);
	TDescBookGroup *DescGroup() { return (TDescBookGroup *) group; }

	mstr decisionFields;
	char name[SIZE_OF_BASE_NAME];
	const char *tr2start;
	const char *tr3start;
	const char *tr3break1;
	const char *tr3break2;
	const char *tr3break3;
	const char *tr3break4;
	const char *tr3final;
	const char *tr2final;

	TDescLawsuit *prevLawsuit;	// 2017:156 +related
	void AddEmits(TDescLawsuit *lawsuit, const char *tr1, const char *tr2, const char *tr3, const char *tr4);
};

TDescBook::TDescBook(TWindow *parent, TDescBookGroup *group)
	: TBook(parent, group, name, desc_fields, title_fields, BOOK_FIXED_DATES),
		decisionFields("F_NO, F_YEAR, F_TYPE, F_DATE, F_KIND, F_FROM_DATE, F_RESULT, F_RETURNED, F_EVENT_NO, "
			"F_REGISTER, F_PAGE, F_VOLUME, F_PARTITION, F_RESULT, F_SESSION_KIND"), prevLawsuit(NULL)
{
	group->CreateName(name);

	if (group->eventText)
		decisionFields.cat(", F_TEXT");
}

TGroup *TDescBook::Process(TQuery &q)
{
	TDescLawsuit *lawsuit = new TDescLawsuit();

	*lawsuit << group;	// options
	*lawsuit << q;
	lawsuit->LoadSessions(str(decisionFields));

	if (DescGroup()->reCap)
		DescGroup()->CountReCap(lawsuit);

	return lawsuit;
}

void TDescBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr2start = office.Find("tr2 start");
	tr3start = office.Find("tr3 start");
	tr3break1 = office.Find("tr3 break 1");
	tr3break2 = office.Find("tr3 break 2");
	tr3break3 = office.Find("tr3 break 3");

	if (DescGroup()->instance == 1 && group->types[0] !=
#if COURT_TYPE == COURT_ADMIN
		TYPE_TRICKY_LAW
#else  // ADMIN
		TYPE_PUNISHMENT_LAW
#endif  // ADMIN
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
		&& group->types[0] != TYPE_ADMIN_LAW
#endif  // ADMIN || AREAL
	)
	{
		tr3break4 = office.Find("tr3 break 4");
		tr3final = office.Find("tr3 final");
	}
	else
		tr3break4 = tr3final = NULL;

	tr2final = office.Find("tr2 final");
	tr1final = office.Find("tr1 final");
}

void TDescBook::AddEmits(TDescLawsuit *lawsuit, const char *tr1,  const char *tr2,const char *tr3, const char *tr4)
{
	const offield *descFields = desc_fields;

	if (tr1 == tr2start)  // nasty hack to avoid same law #
	{
		if (prevLawsuit && lawsuit->key == prevLawsuit->key)
			descFields += 9;

		prevLawsuit = lawsuit;
	}

	for (int i = 0; i < 2; i++)
		if (i ? tr3 : tr1 && i ? tr4 : tr2)
			office.Emit(i ? tr3 : tr1, i ? tr4 : tr2, descFields, lawsuit);

	office.Emit(tr2final, tr1final, desc_fields, lawsuit);	// tr2final is really before tr1final
}

void TDescBook::EmitRow(TGroup *group)
{
	TDescLawsuit *lawsuit = (TDescLawsuit *) group;
	bool firstElement = true;

	lawsuit->LoadSideWinds();

	if (lawsuit->finalizer)
	{
		for (int i = 0; i < lawsuit->sessions.Count(); i++)
		{
			if (lawsuit->sessions[i]->print)
			{
				lawsuit->SetData(i);

				if (firstElement)
				{
					AddEmits(lawsuit, tr2start, tr2final, NULL, NULL);
					firstElement = false;
				}
				else
					AddEmits(lawsuit, tr3start, tr3break3, tr3break4, tr3final);

				for (int j = 0; j < lawsuit->sessions[i]->decisions.Count(); j++)
				{
					if (j)
					{
						lawsuit->SetData(i, j);
						AddEmits(lawsuit, tr3break1, tr3break3, tr3break4, tr3final);
					}

					for (int r = 1; r < lawsuit->sessions[i]->decisions[j]->outRegs.Count(); r++)
					{
						lawsuit->SetOutReg(i, j, r);
						AddEmits(lawsuit, tr3break2, tr3break3, NULL, NULL);
					}
				}
			}

			if (lawsuit->finalizer == 1)
				break;
		}
	}
	else
		AddEmits(lawsuit, tr2start, tr2final, NULL, NULL);
}

// ----- TDescBookReCap ------------------------------------------------------
class TDescBookReCap : public TBaseBook
{
public:
	TDescBookReCap(TWindow *parent, TDescBookGroup *group);

protected:
	char name[SIZE_OF_BASE_NAME];
	const char *tr1start;
	const char *tr1final;
	const char *tr2start;
	const char *tr2final;

	virtual void Markers();
	virtual void EmitBody();
	TDescBookGroup *DescGroup() { return (TDescBookGroup *) group; }
};

TDescBookReCap::TDescBookReCap(TWindow *parent, TDescBookGroup *group)
	: TBaseBook(parent, group, name, reCap_fields, title_fields, BOOK_FIXED_DATES)
{
	group->ReCapName(name);
}

void TDescBookReCap::Markers()
{
	tr1start = office.Find("tr1 start");

	if (CourtType != COURT_APPEAL)
	{
		tr2start = office.Find("tr2 start");
		tr2final = office.Find("tr2 final");
	}

	tr1final = office.Find("tr1 final");
}

void TDescBookReCap::EmitBody()
{
	office.Emit(NULL, tr1start, book_fields, group);

	for (int i = 0; i < DescGroup()->kinds.Count(); i++)
	{
		if (CourtType == COURT_APPEAL)
			office.Emit(tr1start, tr1final, body_fields, DescGroup()->kinds[0]);
		else
		{
			TDescKind *descKind = DescGroup()->kinds[i];

			if (i < 2)
				office.Emit(tr1start, tr2start, body_fields, descKind);
			else
				office.Emit(tr2start, tr2final, body_fields, descKind);

			for (int n = 0; n < descKind->subjects.Count(); n++)
			{
				descKind->subjectIndex = n + 1;
				office.Emit(tr2final, tr1final, body_fields, descKind);
			}
		}
	}

	office.Emit(tr1final, NULL, book_fields, group);
}

// ----- functions -----------------------------------------------------------
static void desc_book(TWindow *parent, TDescBookGroup *tGroup)
{
	for (int i = 0; i < 2; i++)
	{
		if (!(i ? tGroup->instance_2nd : tGroup->instance_1st))
			continue;

		TDescBookGroup group;

		group << tGroup;
		group.instance = i + 1;

		if (group.reCap)
			group.LoadKinds();

	#if COURT_TYPE == COURT_ADMIN
		if (group.instance_1st && group.instance_2nd && *group.types == TYPE_ADMIN_LAW && group.byUnitedFormat)
		{
			group.instance = 3;
			i = 2;
		}
		else if (group.byUnitedFormat)
			i = 2;
	#endif  // ADMIN

		TDescBook(parent, &group).Execute(group.createBook ? BOOK_EXEC_HEADER : BOOK_EXEC_NULL);

		if (group.reCap)
			TDescBookReCap(parent, &group).Execute();
	}
}

#include "DescBookDialog.h"
#include "DescBookDialog.cpp"

void desc_book(TWindow *parent)
{
	TDescBookGroup tGroup;

	if (TDescBookDialog(parent, &tGroup).Execute() == IDOK)
		desc_book(parent, &tGroup);
}
